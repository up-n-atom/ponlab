// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2022, MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/
#include <linux/module.h>
#include <linux/types.h>	/* size_t */
#include <linux/inetdevice.h>
#include <linux/if_vlan.h>
#include <net/datapath_api.h>
#include <net/datapath_proc_api.h>
#include "datapath.h"
#include "datapath_instance.h"
#include "datapath_swdev.h"
#include "datapath_trace.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

static int dp_event(struct notifier_block *this, unsigned long event,
			void *ptr);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
static struct notifier_block dp_dev_notifier = {
	dp_event, /*handler*/
	NULL,
	0
};
#endif

int register_netdev_notifier(u32 flag)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	return register_netdevice_notifier(&dp_dev_notifier);
#else
	return 0;
#endif
}

int unregister_netdev_notifier(u32 flag)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	return unregister_netdevice_notifier(&dp_dev_notifier);
#else
	return 0;
#endif
}

struct netdev_evt {
	int event;
	char *str;
} netdev_evt_list[] = {
	{NETDEV_UP, "UP"},
	{NETDEV_DOWN, "DOWN"},
	{NETDEV_REBOOT, "REBOOT"},
	{NETDEV_CHANGE, "Change"},
	{NETDEV_REGISTER, "REGISTER"},
	{NETDEV_UNREGISTER, "UNREGISTER"},
	{NETDEV_CHANGEMTU, "MTU"},
	{NETDEV_CHANGEADDR, "CHANGE_ADDR"},
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 15)
	{NETDEV_PRE_CHANGEADDR, "PRE_ADDR"},
#endif
	{NETDEV_GOING_DOWN, "GOING_DOWN"},
	{NETDEV_CHANGENAME, "CHANGE_NAME"},
	{NETDEV_FEAT_CHANGE, "FEAT_CHANG"},
	{NETDEV_BONDING_FAILOVER, "BONDING_FAILOVER"},
	{NETDEV_PRE_UP, "PRE_UP"},
	{NETDEV_PRE_TYPE_CHANGE, "PRE_TYPE_CHANGE"},
	{NETDEV_POST_TYPE_CHANGE, "POST_TYPE_CHANGE"},
	{NETDEV_POST_INIT, "POST_INIT"},
	{NETDEV_RELEASE, "RELEASE"},
	{NETDEV_NOTIFY_PEERS, "NOTIFY_PEERS"},
	{NETDEV_JOIN, "JOIN"},
	{NETDEV_CHANGEUPPER, "CHANGEUPPER"},
	{NETDEV_RESEND_IGMP, "RESEND_IGMP"},
	{NETDEV_PRECHANGEMTU,  "PRECHANGEMTU"},
	{NETDEV_CHANGEINFODATA, "CHANGEINFODATA"},
	{NETDEV_BONDING_INFO, "BONDING_INFO"},
	{NETDEV_PRECHANGEUPPER, "PRECHANGEUPPER"},
	{NETDEV_CHANGELOWERSTATE, "CHANGELOWERSTATE"},
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0))
	{NETDEV_UDP_TUNNEL_PUSH_INFO, "UDP_TUNNEL_PUSH_INFO"},
	{NETDEV_UDP_TUNNEL_DROP_INFO, "UDP_TUNNEL_DROP_INFO"},
#endif
	{NETDEV_CHANGE_TX_QUEUE_LEN, "CHANGE_TX_QUEUE_LEN"},
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0))
	{NETDEV_CVLAN_FILTER_PUSH_INFO, "CVLAN_FILTER_PUSH_INFO"},
	{NETDEV_CVLAN_FILTER_DROP_INFO, "CVLAN_FILTER_DROP_INFO"},
	{NETDEV_SVLAN_FILTER_PUSH_INFO, "SVLAN_FILTER_PUSH_INFO"},
	{NETDEV_SVLAN_FILTER_DROP_INFO, "SVLAN_FILTER_DROP_INFO"}
#endif
};

char *get_netdev_evt_name(int event)
{
	int i;

	for (i = 0;  i < ARRAY_SIZE(netdev_evt_list); i++) {
		if (event == netdev_evt_list[i].event)
			return netdev_evt_list[i].str;
	}
	return "unknown";
}

static void _dp_set_cpu_mac(
	bool reset, u8 *addr, const char *name, int inst, int bp, int fid,
	struct inst_info *prop_info)
{
	if (reset)
		prop_info->dp_mac_reset(bp, fid, inst, addr);
	else
		prop_info->dp_mac_set(bp, fid, inst, addr);

	DP_DEBUG(DP_DBG_FLAG_SWDEV,
		 "%s: %s-%02x:%02x:%02x:%02x:%02x:%02x with inst/fid=%d/%d\n",
		 reset ? "dp_mac_reset" : "dp_mac_set  ",
		 name,
		 addr[0], addr[1], addr[2], addr[3], addr[4], addr[5],
		 inst, fid);
}

int dp_set_cpu_mac(struct net_device *dev, bool reset)
{
	struct br_info *br_info;
	struct inst_info *prop_info = NULL;

	if (!dev->dev_addr)
		return -1;
	br_info = dp_swdev_bridge_entry_lookup(dev);
	if (!br_info || br_info->fid < 0 || br_info->inst < 0)
		return -1;
	prop_info = get_dp_prop_info(br_info->inst);
	if (!prop_info->dp_mac_reset || !prop_info->dp_mac_set)
		return -1;
	if (reset) {
		if (!br_info->f_mac_add) /* not added to GSWIP yet */
			return -1;
		_dp_set_cpu_mac(true, dev->dev_addr, dev->name,
				br_info->inst, 0, br_info->fid,
				prop_info);
		dp_memset(br_info->br_mac, 0, sizeof(br_info->br_mac));
		br_info->f_mac_add = false;
		return 0;
	}
	
	if (br_info->f_mac_add) {/* already added to GSWIP */
		if (dp_memcmp(br_info->br_mac, dev->dev_addr, ETH_ALEN) == 0)
			return -1;

		/* Bridge mac address will change during brctl delif/addif. */
		_dp_set_cpu_mac(true, br_info->br_mac, dev->name,
				br_info->inst, 0, br_info->fid, prop_info);
	}

	_dp_set_cpu_mac(false, dev->dev_addr, dev->name,
			br_info->inst, 0, br_info->fid, prop_info);
	dp_memcpy(br_info->br_mac, dev->dev_addr, sizeof(br_info->br_mac));
	br_info->f_mac_add = true;
	return 0;
}

int dp_event_normal(struct notifier_block *this, unsigned long event,
			   void *ptr)
{
	struct net_device *dev;
	dp_subif_t *subif = NULL;
	struct netdev_notifier_changeupper_info *info;
	bool f_dp_dev = false; /* registerd to dpm or not */
	bool f_br_dev = false;

	dev = netdev_notifier_info_to_dev(ptr);
	if (!dev)
		return NOTIFY_DONE;
	if (dev->addr_len != ETH_ALEN) /*only support ethernet */
		return NOTIFY_DONE;
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "event=%s(%ld): %s:\n",
		 get_netdev_evt_name(event),
		 event,
		 dev->name);
	trace_dp_netdev_event(event, dev);
	DP_LIB_LOCK(&dp_lock);
	subif = kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif) {
		DP_LIB_UNLOCK(&dp_lock);
		return 0;
	}
	if (!netif_is_bridge_master(dev)) {
		if (dp_get_netif_subifid(dev, NULL, NULL, NULL, subif, 0)) {
			DP_DEBUG(DP_DBG_FLAG_SWDEV,
				 "%s not dpm-registered yet\n", dev->name);
		} else {
			f_dp_dev = true;
		}
	} else {
		f_br_dev = true;
	}

	switch (event) {
	case NETDEV_REGISTER:
		if (f_br_dev) { /* add to bridge list */
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
			dp_add_br(dev);
#endif
		} else { /* add to dev list */
#ifndef DP_NOT_USE_NETDEV_REGISTER
			dp_add_dev(dev);
#endif
		}
		break;
	case NETDEV_UNREGISTER:
		if (f_br_dev) { /* remove from bridge list */
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
			dp_del_br(dev);
#endif
		} else { /*  remove from dev list */
			dp_del_dev(dev, DP_DEL_DEV_NETDEV_UNREGISTER);
		}
		break;
	case NETDEV_CHANGEUPPER:
		info = ptr;
		if (netif_is_bridge_master(info->upper_dev)) {
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
			dp_update_dev_br(
				dev,
				info->upper_dev,
				info->linking ? DP_BR_JOIN: DP_BR_LEAVE);
			DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s %s bridge %s\n",
				 dev->name,
				 info->linking ? "join" : "leave",
				 info->upper_dev->name);
#endif
		}
		break;
	case NETDEV_CHANGEADDR:
		/* add new bridge mac address */
		if (f_br_dev)
			dp_set_cpu_mac(dev, false);
		break;
	default:
		break;
	}

	if (subif)
		kfree(subif);
	DP_LIB_UNLOCK(&dp_lock);
	return NOTIFY_OK;
}

/* DPM initization not done yet.
 * But here notifier is enabled to monitor netdevice notifier
 */
int dp_event_simple(struct notifier_block *this, unsigned long event,
			    void *ptr)
{
	struct net_device *dev;

	dev = netdev_notifier_info_to_dev(ptr);
	if (!dev)
		return NOTIFY_DONE;
	if (dev->dev_addr && (dev->addr_len != ETH_ALEN)) /*only support ethernet */
		return NOTIFY_DONE;
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "dev %s: %s(%ld)\n",
		 dev->name,
		 get_netdev_evt_name(event),
		 event);
	trace_dp_netdev_event(event, dev);
	DP_LIB_LOCK(&dp_lock);

	switch (event) {
	case NETDEV_REGISTER:
		if (netif_is_bridge_master(dev)) {/* add to bridge list */
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
			dp_add_br(dev);
#endif
		} else { /* add to dev list */
#ifndef DP_NOT_USE_NETDEV_REGISTER
			dp_add_dev(dev);
#endif
		}
		break;
	case NETDEV_UNREGISTER:
		if (netif_is_bridge_master(dev)) { /* remove from bridge list */
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
			dp_del_br(dev);
#endif
		} else { /*  remove from dev list */
			dp_del_dev(dev, DP_DEL_DEV_NETDEV_UNREGISTER);
		}
		break;
	default:
		break;
	}
	DP_LIB_UNLOCK(&dp_lock);

	return NOTIFY_DONE;
}

int dp_event(struct notifier_block *this, unsigned long event, void *ptr)
{
	if (likely(dp_init_ok))
		return dp_event_normal(this, event, ptr);
	return dp_event_simple(this, event, ptr);
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP
