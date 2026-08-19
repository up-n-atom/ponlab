// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2023 MaxLinear, Inc.
 * Copyright (C) 2016-2020 Intel Corporation
 * Copyright (C) 2014-2015 Lantiq Beteiligungs-GmbH & Co. KG
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2, as published
 * by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#define pr_fmt(fmt)		"MCH: "fmt

#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/netdevice.h>
#include <linux/netlink.h>
#include <linux/if_vlan.h>
#include <net/netlink.h>
#include <net/net_namespace.h>
#include <linux/string.h>
#include <linux/rtnetlink.h>
#include <net/mcast_helper_api.h>

#define MCH_SUCCESS		(0)
#define MCH_FAILURE		(-1)

#define MCAST_PROCESS_NAME	"mcastd"
#define IF_INFO_SIZE		NLMSG_ALIGN(sizeof(struct ifinfomsg))
#define NLMSG_TOTAL_SIZE	(IF_INFO_SIZE + nla_total_size(MAX_ADDR_LEN))

#define IS_ACCL_CB(f)		(f & MCH_F_DIRECTPATH)

struct _mcast_accl_cb_t {
	/* Number of callback */
	int num_cb;
	/* Module callback function pointer */
	Mcast_module_callback_t __rcu cb;
} mcast_accl_cb;

typedef struct _mcast_callback_t {
	/* Member netdevice */
	struct net_device *net_dev;
	/* Kernel module name */
	struct module *mod_name;
	/* Module callback function pointer */
	Mcast_module_callback_t __rcu cb;
	/* Module callback flag - MCH_F_* */
	uint32_t flag;
	/* mcast_callback interface map list */
	struct list_head list;
	/* RCU head */
	struct rcu_head rcu;
} mcast_callback_t;

static DEFINE_MUTEX(mch_cb_list_lock);
LIST_HEAD(mch_callback_list_g);

#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
extern void br_multicast_handle_new_sta(struct net_device *dev, const u8 *src);
#endif

static int
mcast_helper_send_netlink_msg_user(struct net_device *dev,
				   int type, int queryflag, void *data)
{
	int ret = MCH_FAILURE;
	struct sk_buff *skb;
	struct nlmsghdr *nlh;
	struct ifinfomsg *ifm;
	struct net *net = dev_net(dev);
	int pid = 0;
	bool pid_found = false;
	struct task_struct *task;

	for_each_process(task) {
		/* Compare the process name with each of the
		 * task struct process name.
		 */
		if (strstr(task->comm, MCAST_PROCESS_NAME) == NULL)
			continue;
		pid = task->pid;
		pid_found = true;
		break;
	}

	if (pid_found == false)
		return ret;

	if (!dev)
		return ret;

	skb = nlmsg_new(NLMSG_TOTAL_SIZE, GFP_ATOMIC);
	if (!skb) {
		pr_err("SKB allocation for IPv4 failure.\n");
		return ret;
	}
	nlh = nlmsg_put(skb, 0, 0, type, IF_INFO_SIZE, 0);
	if (nlh == NULL) {
		kfree_skb(skb);
		pr_err("failed to put NL message.\n");
		return ret;
	}
	nlh->nlmsg_type = type;
	ifm = nlmsg_data(nlh);
	ifm->ifi_family = AF_UNSPEC;
	ifm->__ifi_pad = 0;
	ifm->ifi_type = dev->type;
	ifm->ifi_index = dev->ifindex;
	if (type == RTM_NEWLINK)
		ifm->ifi_flags = dev_get_flags(dev) | IFF_UP;
	if (queryflag)
		ifm->ifi_flags |= IFF_SLAVE;
	ifm->ifi_change = 0;

	if (data) {
		/* Send STA's MAC which got disconnected */
		ret = nla_put(skb, NLA_BINARY, ETH_ALEN, data);
		if (ret) {
			kfree_skb(skb);
			pr_err("failed to put NL binary message.\n");
			return ret;
		}
	}

	ret = netlink_unicast(net->rtnl, skb, pid, MSG_DONTWAIT);

	if (ret)
		return MCH_FAILURE;
	else
		return MCH_SUCCESS;
}

static int
mcast_helper_reg_callback(struct net_device *dev, Mcast_module_callback_t *cb,
			  struct module *mod_name, unsigned int flags)
{
	mcast_callback_t *mc_cb_rec = NULL;

	if (dev == NULL)
		return MCH_FAILURE;

	mc_cb_rec = kmalloc(sizeof(mcast_callback_t), GFP_ATOMIC);
	if (mc_cb_rec == NULL)
		return MCH_FAILURE;

	mc_cb_rec->net_dev = dev;
	mc_cb_rec->mod_name = mod_name;
	mc_cb_rec->flag = flags;
	INIT_LIST_HEAD(&mc_cb_rec->list);
	mutex_lock(&mch_cb_list_lock);
	rcu_assign_pointer(mc_cb_rec->cb, (Mcast_module_callback_t)cb);
	if (IS_ACCL_CB(flags)) {
		/* Register callback from accelerate module */
		if (!mcast_accl_cb.num_cb)
			rcu_assign_pointer(mcast_accl_cb.cb,
					   (Mcast_module_callback_t)cb);
		mcast_accl_cb.num_cb++;
	}
	list_add_tail_rcu(&mc_cb_rec->list, &mch_callback_list_g);
	mutex_unlock(&mch_cb_list_lock);
	if ((flags & MCH_F_FW_RESET) == MCH_F_FW_RESET)
		mcast_helper_send_netlink_msg_user(dev, RTM_NEWLINK, 0, NULL);

	return MCH_SUCCESS;
}

static int
mcast_helper_dereg_callback(struct net_device *dev, Mcast_module_callback_t *cb,
			    struct module *mod_name, unsigned int flags)

{
	struct list_head *liter = NULL;
	struct list_head *gliter = NULL;
	mcast_callback_t *mc_cb_rec = NULL;

	if (dev == NULL)
		return MCH_FAILURE;

	mutex_lock(&mch_cb_list_lock);
	list_for_each_safe(liter, gliter, &mch_callback_list_g) {
		mc_cb_rec = list_entry(liter, mcast_callback_t, list);
		if (mc_cb_rec->net_dev != dev)
			continue;
		if (strncmp(mod_name->name,
		    mc_cb_rec->mod_name->name, MODULE_NAME_LEN))
			continue;
		list_del_rcu(&mc_cb_rec->list);
		kfree_rcu(mc_cb_rec, rcu);
		if (IS_ACCL_CB(flags)) {
			/* Deregister callback for accelerate module */
			mcast_accl_cb.num_cb--;
			if (!mcast_accl_cb.num_cb)
				rcu_assign_pointer(mcast_accl_cb.cb, NULL);
		}
		mutex_unlock(&mch_cb_list_lock);
		if ((flags & MCH_F_FW_RESET) == MCH_F_FW_RESET)
			mcast_helper_send_netlink_msg_user(dev, RTM_DELLINK,
							   0, NULL);
		return MCH_SUCCESS;
	}
	mutex_unlock(&mch_cb_list_lock);

	return MCH_FAILURE;
}

/** Register callback function **/
void mcast_helper_register_module(
		struct net_device *dev,		/* Registered dev e.g. wlan0 */
		struct module *mod_name,	/* Kernel Module Name */
		char *addl_name,		/* Optional Additional Name */
		Mcast_module_callback_t *cb,	/* Callback Function */
		void *data,			/* Variable input data */
		unsigned int flags)		/* Flag - MCH_F_* */
{
	if (dev == NULL)
		return;

	if (flags & MCH_F_REGISTER) {
		mcast_helper_reg_callback(dev, cb, mod_name, flags);
	} else if (flags & MCH_F_DEREGISTER) {
		mcast_helper_dereg_callback(dev, cb, mod_name, flags);
	} else if (flags & MCH_F_NEW_STA) {
#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
		br_multicast_handle_new_sta(dev, data);
#endif
		mcast_helper_send_netlink_msg_user(dev, RTM_NEWLINK, 1, NULL);
	} else if (flags & MCH_F_DISCONN_MAC) {
		if (data == NULL)
			pr_info("MAC is NULL - flag : MCH_F_DISCONN_MAC\n");
		else
			mcast_helper_send_netlink_msg_user(dev, RTM_DELLINK, 0, data);
	}
}
EXPORT_SYMBOL(mcast_helper_register_module);

int mcast_helper_invoke_callback(unsigned int grpidx, struct net_device *dev,
				 void *mc_stream, unsigned int flag)
{
	int ret = MCH_SUCCESS;
	struct net_device *vap_dev = dev;
	mcast_callback_t *mc_cb_rec = NULL;
	unsigned int passflag;
	Mcast_module_callback_t mod_cb;

	if (dev == NULL) {
		rcu_read_lock();
		mod_cb = rcu_dereference(mcast_accl_cb.cb);
		if (mod_cb && grpidx && (flag & MCH_CB_F_DEL)) {
			/* Delete inactive stream from HW */
			mod_cb(grpidx, NULL, mc_stream, MCH_CB_F_DEL);
		} else {
			ret = MCH_FAILURE;
		}
		rcu_read_unlock();
		return ret;
	}

	if (is_vlan_dev(dev))
		vap_dev = vlan_dev_real_dev(dev);

	rcu_read_lock();
	list_for_each_entry_rcu(mc_cb_rec, &mch_callback_list_g, list) {
		if ((mc_cb_rec->net_dev != dev) && (mc_cb_rec->net_dev != vap_dev))
			continue;
		mod_cb = rcu_dereference(mc_cb_rec->cb);
		if (mod_cb == NULL)
			continue;
		passflag = flag;
		if ((mc_cb_rec->flag & MCH_F_UPDATE_MAC_ADDR)) {
			/* WLAN driver needs UPDATE for host tracking */
			if (flag & MCH_CB_F_DEL_UPD) {
				passflag = MCH_CB_F_UPD;
			} else if (flag & MCH_CB_F_DROP) {
				/* Converting DEL + DROP to DEL action */
				passflag = MCH_CB_F_DEL;
			}
			/* Device must be VAP interface even for VLAN iface */
			mod_cb(grpidx, vap_dev, mc_stream, passflag);
		} else {
			if (flag == MCH_CB_F_UPD)
				passflag = MCH_CB_F_ADD;
			if (flag != MCH_CB_F_DEL_UPD)
				mod_cb(grpidx, dev, mc_stream, passflag);
		}
	}
	rcu_read_unlock();

	return ret;
}
EXPORT_SYMBOL(mcast_helper_invoke_callback);
