// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2022, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

/* MAC Transition Handling (MTH) MACDB driver */

/* Includes */
#include <linux/module.h>
#include <linux/version.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/kthread.h>
#include <linux/if_vlan.h>
#include <linux/jhash.h>
#include <net/netlink.h>
#include <linux/debugfs.h>

#if IS_ENABLED(CONFIG_PPA)
#include <net/ppa/ppa_api.h>
#endif /* CONFIG_PPA */

#include "mth_macdb.h"

#define MACDB_HASH_BITS 8
#define MACDB_HASH_SIZE (1 << MACDB_HASH_BITS)

struct macdb_entry {
	struct hlist_node hlist;
	unsigned char addr[ETH_ALEN];
	struct net_device *dev;
	struct net_device *realdev;
	struct rcu_head rcu;
};

static struct socket *nl_sock;
static struct task_struct *evt_rx_thread;

struct hlist_head macdb_hash[MACDB_HASH_SIZE];
static struct kmem_cache *macdb_cache __read_mostly;
static u32 macdb_salt __read_mostly;
static RAW_NOTIFIER_HEAD(db_chain);
static struct dentry *mth_dbgfs;

static inline int mac_hash(const unsigned char *mac)
{
	/* use 1 byte of OUI and 3 bytes of NIC */
	u32 key = get_unaligned((u32 *)(mac + 2));

	return jhash_1word(key, macdb_salt) & (MACDB_HASH_SIZE - 1);
}

static struct macdb_entry *macdb_find(struct hlist_head *head,
				      const unsigned char *addr)
{
	struct macdb_entry *m;

	hlist_for_each_entry(m, head, hlist) {
		if (ether_addr_equal(m->addr, addr))
			return m;
	}
	return NULL;
}

static struct macdb_entry *macdb_add(struct hlist_head *head,
				     const unsigned char *addr,
				     struct net_device *dev,
				     struct net_device *realdev)
{
	struct macdb_entry *m;

	m = kmem_cache_alloc(macdb_cache, GFP_ATOMIC);
	if (m) {
		memcpy(m->addr, addr, ETH_ALEN);
		m->dev = dev;
		m->realdev = realdev;
		hlist_add_head_rcu(&m->hlist, head);
	}
	return m;
}

static void macdb_free_rcu(struct rcu_head *head)
{
	struct macdb_entry *m =
		container_of(head, struct macdb_entry, rcu);
	kmem_cache_free(macdb_cache, m);
}

static inline void macdb_delete(struct macdb_entry *m)
{
	hlist_del_rcu(&m->hlist);
	call_rcu(&m->rcu, macdb_free_rcu);
}

static inline void macdb_delete_sync(struct macdb_entry *m)
{
	hlist_del(&m->hlist);
	kmem_cache_free(macdb_cache, m);
}

static void macdb_flush(void)
{
	int i;

	for (i = 0; i < MACDB_HASH_SIZE; i++) {
		struct macdb_entry *m;
		struct hlist_node *n;

		hlist_for_each_entry_safe(m, n, &macdb_hash[i], hlist)
			macdb_delete_sync(m);
	}
}

static inline void notify_macdb_event(enum mth_db_event_type type,
				      u8 *addr, struct net_device *dev,
				      struct net_device *realdev)
{
	struct mth_db_event_info info = {0};

	pr_debug("MTH: Notify macdb event:%d for mac:%pM dev:%s realdev:%s\n",
		 type, addr, dev->name, realdev->name);

	memcpy(&info.mac, addr, ETH_ALEN);
	info.dev = dev;
	info.realdev = realdev;
	raw_notifier_call_chain(&db_chain, type, &info);

#if IS_ENABLED(CONFIG_PPA)
	switch (type) {
	case MTH_MACDB_DEL:
		/* Call platform hook to flush acceleration entries
		 * for the MAC (except GRX)
		 */
		if (!IS_ENABLED(CONFIG_PPA_BR_MAC_LEARNING) &&
		    ppa_hook_disconn_if_fn)
			ppa_hook_disconn_if_fn(dev, NULL, addr, 0);
		break;
	default:
		break;
	}
#endif /* CONFIG_PPA */
}

static inline void notify_fdb_event(enum mth_db_event_type type, u16 nlmsg_type,
				    u8 *addr, struct net_device *dev,
				    struct net_device *realdev)
{
	struct mth_db_event_info info = {0};

	pr_debug("MTH: Notify brfdb event:%d for nlmsg_type:%d mac:%pM"
		 " dev:%s realdev:%s\n", type, nlmsg_type,
		 addr, dev->name, realdev->name);

	memcpy(&info.mac, addr, ETH_ALEN);
	info.dev = dev;
	info.realdev = realdev;
	raw_notifier_call_chain(&db_chain, type, &info);

#if defined(CONFIG_X86_PUMA7) || defined(CONFIG_ARM_AVALANCHE_SOC)
	/* Call platform hook to sync acceleration entries
	 * for the MAC (PUMA specific)
	 */
	avalanche_pp_fdb_event(nlmsg_type, addr, dev, realdev);
#endif
}

static int mth_rx_event(void *data)
{
	int32_t ret = 0;
	struct sock *sk = (struct sock *)data;
	struct sk_buff *skb;
	struct nlmsghdr *nlh;
	struct ndmsg *ndm;
	struct nlattr *tb[NDA_MAX+1];
	uint8_t *addr;
	struct net_device *dev, *old_dev;
	struct net_device *realdev, *old_realdev;
	bool added, deleted;
	struct hlist_head *macdb_head;
	struct macdb_entry *m;
	enum mth_db_event_type fdb_type;

	do {
		/* Receive the data packet (blocking) */
		skb = skb_recv_datagram(sk, 0, 0, &ret);
		if (!skb) {
			pr_debug("MTH: Invalid netlink data!\n");
			continue;
		}

		/* Validate netlink message */
		nlh = (struct nlmsghdr *)skb->data;
		if (!nlh || !nlmsg_ok(nlh, skb->len)) {
			pr_debug("MTH: Invalid netlink header data!\n");
			goto drop;
		}

		/*
		 * FIXME: For the routed interface,
		 *        ndm_family=AF_INET/AF_INET6 can be used
		 */
		ndm = nlmsg_data(nlh);
		if (((nlh->nlmsg_type != RTM_NEWNEIGH) &&
		     (nlh->nlmsg_type != RTM_DELNEIGH)) ||
		    (ndm->ndm_family != AF_BRIDGE) ||
		    (ndm->ndm_state & NUD_PERMANENT)) {
			pr_debug("MTH: nlmsg with other nlmsg_type:%d ndm_family:%d"
				 " ndm_state:%d!\n", nlh->nlmsg_type,
				 ndm->ndm_family, ndm->ndm_state);
			goto drop;
		}

		/* Extract associated MAC */
#if KERNEL_VERSION(4, 11, 0) < LINUX_VERSION_CODE
		ret = nlmsg_parse(nlh, sizeof(*ndm), tb, NDA_MAX, NULL, NULL);
#else
		ret = nlmsg_parse(nlh, sizeof(*ndm), tb, NDA_MAX, NULL);
#endif
		if (ret < 0) {
			pr_debug("MTH: nlmsg with invalid data\n");
			goto drop;
		}

		if (!tb[NDA_LLADDR] || nla_len(tb[NDA_LLADDR]) != ETH_ALEN) {
			pr_debug("MTH: nlmsg with invalid address!\n");
			goto drop;
		}

		addr = nla_data(tb[NDA_LLADDR]);

		/* Extract associated interface */
		dev = __dev_get_by_index(&init_net, ndm->ndm_ifindex);
		if (dev == NULL) {
			pr_debug("MTH: nlmsg with unknown ifindex!\n");
			goto drop;
		}

		/* Extract underlying real interface (only VLAN supported) */
		realdev = dev;
		if (is_vlan_dev(dev))
			realdev = vlan_dev_real_dev(dev);

		added = deleted = false;
		macdb_head = &macdb_hash[mac_hash(addr)];
		m = macdb_find(macdb_head, addr);
		switch (nlh->nlmsg_type) {
		case RTM_NEWNEIGH:
			pr_debug("MTH: RTM_NEWNEIGH event for mac:%pM on dev:%s\n",
				 addr, dev->name);

			if (likely(!m)) {
				macdb_add(macdb_head, addr, dev, realdev);
				added = true;
			} else if (likely(dev != m->dev)) {
				old_dev = m->dev;
				old_realdev = m->realdev;
				m->dev = dev;
				m->realdev = realdev;

				added = deleted = true;
			}

			fdb_type = MTH_BRFDB_ADD;
			break;

		case RTM_DELNEIGH:
			pr_debug("MTH: RTM_DELNEIGH event for mac:%pM on dev:%s\n",
				 addr, dev->name);

			if (likely(!m || (dev == m->dev))) {
				if (likely(m)) {
					old_dev = m->dev;
					old_realdev = m->realdev;
					macdb_delete(m);
					deleted = true;
				}
			}

			fdb_type = MTH_BRFDB_DEL;
			break;
		}

		if (deleted) {
			notify_macdb_event(MTH_MACDB_DEL, addr,
					   old_dev, old_realdev);
		}
		if (added)
			notify_macdb_event(MTH_MACDB_ADD, addr, dev, realdev);
		notify_fdb_event(fdb_type, nlh->nlmsg_type, addr, dev, realdev);

drop:
		skb_free_datagram(sk, skb);
	} while (!kthread_should_stop());

	return 0;
}

int mth_register_db_notifier(struct notifier_block *nb)
{
	return raw_notifier_chain_register(&db_chain, nb);
}
EXPORT_SYMBOL(mth_register_db_notifier);

int mth_unregister_db_notifier(struct notifier_block *nb)
{
	return raw_notifier_chain_unregister(&db_chain, nb);
}
EXPORT_SYMBOL(mth_unregister_db_notifier);

#if KERNEL_VERSION(4, 16, 0) > LINUX_VERSION_CODE
#define DEFINE_SHOW_ATTRIBUTE(__name)					\
static int __name ## _open(struct inode *inode, struct file *file)	\
{									\
	return single_open(file, __name ## _show, inode->i_private);	\
}									\
									\
static const struct file_operations __name ## _fops = {			\
	.owner		= THIS_MODULE,					\
	.open		= __name ## _open,				\
	.read		= seq_read,					\
	.llseek		= seq_lseek,					\
	.release	= single_release,				\
}
#endif

static int dbgfs_macdb_show(struct seq_file *seq, void *v)
{
	int i;

	if (!capable(CAP_NET_ADMIN)) {
		pr_err("Read Permission denied");
		return 0;
	}

	seq_printf(seq, "%-17s\t%-16s\n", "mac addr", "iface (base)");

	rcu_read_lock();
	for (i = 0; i < MACDB_HASH_SIZE; i++) {
		struct macdb_entry *m;

		hlist_for_each_entry_rcu(m, &macdb_hash[i], hlist) {
			seq_printf(seq, "%pM\t%s (%s)\n",
				m->addr, m->dev->name, m->realdev->name);
		}
	}
	rcu_read_unlock();
	return 0;
}
DEFINE_SHOW_ATTRIBUTE(dbgfs_macdb);

static int32_t mth_debugfs_init(void)
{
	mth_dbgfs = debugfs_create_dir("mth", NULL);
	if (IS_ERR_OR_NULL(mth_dbgfs))
		return -ENOMEM;

	debugfs_create_file("macdb", 0400, mth_dbgfs, NULL, &dbgfs_macdb_fops);
	return 0;
}

static void mth_debugfs_exit(void)
{
	debugfs_remove_recursive(mth_dbgfs);
	mth_dbgfs = NULL;
}

static __init int32_t mth_macdb_init(void)
{
	int32_t ret;
	struct sockaddr_nl addr = {0};

	/* Create a macdb */
	macdb_cache = kmem_cache_create("macdb_cache",
					sizeof(struct macdb_entry),
					0,
					SLAB_HWCACHE_ALIGN, NULL);
	if (!macdb_cache) {
		ret = -ENOMEM;
		goto err_out;
	}

	get_random_bytes(&macdb_salt, sizeof(macdb_salt));

	/* Create a netlink socket */
	ret = sock_create_kern(&init_net, AF_NETLINK, SOCK_RAW,
			       NETLINK_ROUTE, &nl_sock);
	if (ret) {
		pr_err("sock_create_kern() failed (ret:%d)!\n", ret);
		goto err_out_db;
	}

	addr.nl_family  = AF_NETLINK;
	addr.nl_pid     = 0;
	addr.nl_groups  = 1 << (RTNLGRP_NEIGH - 1);

	ret = kernel_bind(nl_sock, (struct sockaddr *)&addr, sizeof(addr));
	if (ret) {
		pr_err("kernel_bind() failed (ret:%d)!\n", ret);
		goto err_out_sock;
	}

	/* Start a event receiver thread */
	evt_rx_thread = kthread_run(mth_rx_event, nl_sock->sk, "evt_rx");
	if (IS_ERR(evt_rx_thread)) {
		pr_err("failed to start event rx thread!");
		ret = -1;
		goto err_out_sock;
	}

	mth_debugfs_init();
	return 0;

err_out_sock:
	sock_release(nl_sock);
	nl_sock = NULL;

err_out_db:
	kmem_cache_destroy(macdb_cache);
	macdb_cache = NULL;

err_out:
	return ret;
}

static __exit void mth_macdb_exit(void)
{
	mth_debugfs_exit();
	if (evt_rx_thread) {
		kthread_stop(evt_rx_thread);
		evt_rx_thread = NULL;
	}

	sock_release(nl_sock);
	nl_sock = NULL;

	macdb_flush();
	/* Wait for completion of call_rcu()'s (macdb_free_rcu) */
	rcu_barrier();
	kmem_cache_destroy(macdb_cache);
}

module_init(mth_macdb_init);
module_exit(mth_macdb_exit);
MODULE_LICENSE("GPL");
