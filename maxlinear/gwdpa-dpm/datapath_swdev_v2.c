// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2022, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/
#include <linux/if_bridge.h>
#include <linux/if_vlan.h>
#include <linux/ethtool.h>
#include <linux/types.h>
#include <net/rtnetlink.h>
#include <net/datapath_api.h>
#include <net/datapath_proc_api.h>

#include "datapath.h"
#include "datapath_instance.h"
#include "datapath_ioctl.h"
#include "datapath_swdev.h"
#include "datapath_trace.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

/* Note: this file is for Linux Kernel 4.x and tested with 4.9 and 4.19 only.
 *       If later no need to support kernel 4.x, this file should be removed
 */

#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0)

static int dp_swdev_port_attr_bridge_flags_get(
	struct net_device *dev,
	unsigned long *flags)
{
	int ret;
	struct switchdev_brport_flags flag;
	flag.mask = *flags;
	flag.val = *flags;
	ret = _dp_port_attr_br_flags_get(dev, &flag);
	*flags = flag.mask;
	return ret;
}

static int dp_swdev_port_attr_bridge_flags_set(struct net_device *dev,
					       unsigned long flags,
					       struct switchdev_trans *trans)
{
	struct inst_info *i_info;
	dp_subif_t *subif;
	struct net_device *br_dev;
	struct br_info *br_info;
	int ret = -EOPNOTSUPP;

	br_dev = netdev_master_upper_dev_get(dev);
	if (!br_dev)
		return -1;

	if (switchdev_trans_ph_prepare(trans))
		return DP_SUCCESS;

	subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif)
		return DP_FAILURE;
	if (dp_get_netif_subifid(dev, NULL, NULL, NULL, subif, 0)) {
		DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s: get subif failed for %s\n",
			 __func__, dev->name);
		kfree(subif);
		return -EINVAL;
	}

	i_info = &dp_port_prop[subif->inst].info;
	if (!i_info->swdev_bridge_port_flags_set) {
		kfree(subif);
		return ret;
	}

	br_info = dp_swdev_bridge_entry_lookup(br_dev);
	if (!br_info)
		goto EXIT;

	ret = i_info->swdev_bridge_port_flags_set(br_info, subif->inst,
			subif->bport, flags);
EXIT:
	kfree(subif);
	return ret;
}

#ifndef DP_SWITCHDEV_NO_HACKING
static int dp_swdev_port_attr_learning_limit_set(struct net_device *dev,
						 int learning_limit,
						 struct switchdev_trans *trans)
{
	if (switchdev_trans_ph_prepare(trans))
		return DP_SUCCESS;
	return _dp_swdev_port_attr_learning_limit_set(dev,
						      learning_limit);
}

static int dp_swdev_bridge_attr_mcast_flood(struct net_device *bridge,
					    enum br_mcast_flood_mode mcast_flood,
					    struct switchdev_trans *trans)
{
	if (switchdev_trans_ph_prepare(trans))
		return DP_SUCCESS;

	return _dp_swdev_bridge_attr_mcast_flood(bridge, mcast_flood);
}
#endif /* DP_SWITCHDEV_NO_HACKING */

int dp_swdev_port_attr_set2(struct net_device *dev,
				  const struct switchdev_attr *attr,
				  struct switchdev_trans *trans)
{
	struct dp_dev *dp_dev;
	int ret = -EOPNOTSUPP;

	if (dp_mod_exiting)
		return ret;
	DP_SWDEV_LOCK(&dp_swdev_lock);
	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev)
		goto EXIT;
	if (!dp_dev->count)
		goto EXIT;
	if (!dp_dev->br_info)
		goto EXIT;

	switch (attr->id) {
	case SWITCHDEV_ATTR_ID_PORT_STP_STATE:
		if (attr->u.stp_state == BR_STATE_FORWARDING)
			/* STP STATE forwading or ifconfig UP - add bridge*/
			ret = dp_swdev_stp_forward(dp_dev);
		else if (attr->u.stp_state == BR_STATE_DISABLED)
			ret = dp_swdev_stp_disable(dp_dev);
		else
			DP_DEBUG(DP_DBG_FLAG_SWDEV,
				 "PORT_STP_STATE id=%d-%s: %s\n",
				 attr->u.stp_state,
				 get_stp_stat_str(attr->u.stp_state),
				 dev->name);
		trace_dp_swdev_event(NULL, attr->u.stp_state, dev, attr);
		break;
	case SWITCHDEV_ATTR_ID_PORT_BRIDGE_FLAGS:
		ret = dp_swdev_port_attr_bridge_flags_set(dev,
							  attr->u.brport_flags,
							  trans);
		trace_dp_swdev_event(NULL, attr->u.brport_flags, dev, attr);
		break;
#ifndef DP_SWITCHDEV_NO_HACKING
	case SWITCHDEV_ATTR_ID_BRIDGE_MCAST_FLOOD:
		ret = dp_swdev_bridge_attr_mcast_flood(attr->orig_dev,
						       attr->u.mcast_flood,
						       trans);
#endif
		break;
	case SWITCHDEV_ATTR_ID_BRIDGE_AGEING_TIME:
		/* ret = dp_swdev_port_attr_bridge_ageing_time_set(
		 * dp_swdev_port,
		 * attr->u.ageing_time,
		 * trans);
		 */
		break;
	case SWITCHDEV_ATTR_ID_BRIDGE_VLAN_FILTERING:
		/* ret = dp_swdev_port_attr_bridge_br_vlan_set(dev,
		 * attr->orig_dev,trans);
		 */
		 break;

#ifndef DP_SWITCHDEV_NO_HACKING
	case SWITCHDEV_ATTR_ID_PORT_LEARNING_LIMIT:
		ret = dp_swdev_port_attr_learning_limit_set(
			dev, attr->u.learning_limit, trans);
		  break;
#endif
	default:
		break;
	}
EXIT:
	DP_SWDEV_UNLOCK(&dp_swdev_lock);
	return ret;
}

int dp_swdev_port_attr_get2(struct net_device *dev,
				  struct switchdev_attr *attr)
{
	struct net_device *br_dev;
	struct br_info *br_info;
	int ret = 0;

	if (dp_mod_exiting)
		return ret;
	br_dev = netdev_master_upper_dev_get(attr->orig_dev);
	if (!br_dev)
		return -EOPNOTSUPP;
	DP_SWDEV_LOCK(&dp_swdev_lock);
	switch (attr->id) {
	case SWITCHDEV_ATTR_ID_PORT_PARENT_ID:
		/* Default err return value "-EOPNOTSUPP"cannot be set as
		 * this blocks bridgeport offload_fwd_mark setting at
		 * linux bridge level("nbp_switchdev_mark_set")
		 */
		br_info = dp_swdev_bridge_entry_lookup(br_dev);
		if (!br_info) {
			goto EXIT;
		}
		attr->u.ppid.id_len = sizeof(br_info->fid);
		dp_memcpy(&attr->u.ppid.id, &br_info->fid, attr->u.ppid.id_len);

		DP_DEBUG(DP_DBG_FLAG_SWDEV_DETAIL,
			 "SWITCHDEV_ATTR_ID_PORT_PARENT_ID:%s fid=%d\n",
			 attr->orig_dev ? attr->orig_dev->name : "NULL",
			 br_info->fid);
		break;
	case SWITCHDEV_ATTR_ID_PORT_BRIDGE_FLAGS:
		DP_DEBUG(DP_DBG_FLAG_SWDEV,"SWITCHDEV_ATTR_ID_PORT_BRIDGE_FLAGS:%s\n",
			 dev->name);
		ret = dp_swdev_port_attr_bridge_flags_get(
			dev, &attr->u.brport_flags);
		break;
	case SWITCHDEV_ATTR_ID_PORT_BRIDGE_FLAGS_SUPPORT:
		DP_DEBUG(DP_DBG_FLAG_SWDEV,"SWITCHDEV_ATTR_ID_PORT_BRIDGE_FLAGS_SUPPORT:%s\n",
			 dev->name);
		attr->u.brport_flags_support = BR_HAIRPIN_MODE | BR_ISOLATED |\
					       BR_LEARNING |\
					       BR_MCAST_DEST_LOOKUP |\
					       BR_MCAST_SRC_LOOKUP;
		break;
	default:
		DP_DEBUG(DP_DBG_FLAG_SWDEV,"not supported id=%d:%s\n", attr->id,
			 dev->name);
		break;
	}
EXIT:
	DP_SWDEV_UNLOCK(&dp_swdev_lock);
	return ret;
}

int dp_swdev_port_obj_add2(struct net_device *dev,
				 const struct switchdev_obj *obj,
				 struct switchdev_trans *trans)
{
	return 0;
}

int dp_swdev_port_obj_del2(struct net_device *dev,
				 const struct switchdev_obj *obj)
{
	return 0;
}

/* This function registers the created port in datapath to switchdev */
int dp_register_switchdev_ops(struct net_device *dev, int reset)
{
	struct dp_dev *dp_dev;
	static const int offset_swdev[] = {
		offsetof(struct switchdev_ops, switchdev_port_attr_get),
		offsetof(struct switchdev_ops, switchdev_port_attr_set),
		offsetof(struct switchdev_ops, switchdev_port_obj_add),
		offsetof(struct switchdev_ops, switchdev_port_obj_del)
	};
	static const char *memo_swdev[] = {
		"dpm-switchdev_port_attr_get",
		"dpm-switchdev_port_attr_set",
		"dpm-switchdev_port_obj_add",
		"dpm-switchdev_port_obj_del"
	};
	static const void *cb_swdev[] = {
		&dp_swdev_port_attr_get, &dp_swdev_port_attr_set,
		&dp_swdev_port_obj_add, &dp_swdev_port_obj_del
	};
	static const int offset_netdev[] = {
		offsetof(struct net_device_ops, ndo_bridge_setlink),
		offsetof(struct net_device_ops, ndo_bridge_dellink)
	};
	static const char *memo_netdev[] = {
		"dpm-ndo_bridge_setlink",
		"dpm-ndo_bridge_dellink"
	};
	static const void *cb_netdev[] = {
		&dp_bp_ndo_br_setlink,
		&dp_bp_ndo_br_dellink
	};
	u32 flag;
	struct dp_dev_subif *dev_sif;
	int i, ret = DP_SUCCESS;

	DP_DEBUG(DP_DBG_FLAG_OPS, "switchdev_ops %s for %s ...\n",
		 reset ? "reset" : "update", dev->name);
	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev) {
		pr_err("DPM: %s: not found dev %s\n", __func__, dev->name);
		return DP_FAILURE;
	}
	dev_sif = list_first_entry(&dp_dev->subif_list, struct dp_dev_subif,
				   list);
	if (!dev_sif) {
		pr_err("DPM: %s: dev_sif NULL:%s\n", __func__, dev->name);
		return DP_FAILURE;
	}

	DP_SWDEV_LOCK(&dp_swdev_lock);
	if (dev_sif->sif->swdev_en) {
		flag = DP_OPS_SWITCHDEV;
		if (reset)
			flag |= DP_OPS_RESET;

		/* switchdev ops register */
		for (i = 0; i < ARRAY_SIZE(offset_swdev); i++) {
			if (dp_set_net_dev_ops_priv(dp_dev->dev, cb_swdev[i],
						    offset_swdev[i], flag,
						    memo_swdev[i])) {
				pr_err("DPM: %s failed to %s ops[%d] for %s\n",
				       __func__,
				       reset ? "de-register" : "reigster",
				       i,
				       dev->name);
				ret = DP_FAILURE;
				goto exit;
			}
		}
		DP_DEBUG(DP_DBG_FLAG_OPS,
			 "switchdev_ops %s for %s done with all_ops_cnt=%d\n",
			 reset ? "reset" : "update", dev->name,
			 dp_dev->all_ops_cnt);

		flag = DP_OPS_NETDEV;
		if (reset)
			flag |= DP_OPS_RESET;

		/* switchdev ops register */
		for (i = 0; i < ARRAY_SIZE(offset_netdev); i++) {
			if (dp_set_net_dev_ops_priv(dp_dev->dev, cb_netdev[i],
						    offset_netdev[i], flag,
						    memo_netdev[i])) {
				pr_err("DPM: %s failed to %s ops[%d] for %s\n",
				       __func__,
				       reset ? "de-register" : "reigster",
				       i,
				       dev->name);
				ret = DP_FAILURE;
				goto exit;
			}
		}
		DP_DEBUG(DP_DBG_FLAG_OPS,
			 "netdev_ops %s for %s done with all_ops_cnt=%d\n",
			 reset ? "reset" : "update", dev->name,
			 dp_dev->all_ops_cnt);
	}
exit:
	DP_SWDEV_UNLOCK(&dp_swdev_lock);
	return ret;
}
#endif /* LINUX_VERSION_CODE */
#endif /* CONFIG_DPM_DATAPATH_SWITCHDEV */

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP
