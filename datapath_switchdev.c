/*******************************************************************************
	 Copyright (c) 2022 - 2023 MaxLinear, Inc.

 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/
#include <linux/version.h>
#include <linux/etherdevice.h>
#include <linux/if_bridge.h>
#include <linux/netdevice.h>
#include <linux/workqueue.h>
#include <net/switchdev.h>
#include <net/datapath_api.h>
#include "datapath.h"
#include "datapath_switchdev.h"
#include "datapath_br_vlan.h"
#include "datapath_instance.h"
#include "datapath_trace.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)

static struct list_head swdev_evt_data; /* switchdev event data */
struct dp_switchdev_event_work {
	struct work_struct work;
	struct switchdev_notifier_fdb_info fdb_info;
	struct dp_swdev_data *dev_data;
	unsigned long event;
};

/* Had better return -EOPNOTSUPP or 0 to avoid call trace dump */
int dp_ndo_get_port_parent_id2(struct net_device *dev,
			       struct netdev_phys_item_id *ppid)
{
	struct net_device *br_dev;
	struct br_info *br_info;
	int ret = -EOPNOTSUPP;
	u32 parent_id;

	if (dp_mod_exiting)
		return ret;
	if (!dev) {
		pr_err("DPM: %s: bridge port dev NULL\n", __func__);
		return ret;
	}
	br_dev = netdev_master_upper_dev_get(dev);
	if (!br_dev) /* not under any bridge */ {
		DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s not in any bridge\n", dev->name);
		return ret;
	}

	br_info = dp_swdev_bridge_entry_lookup(br_dev);
	if (!br_info) {
		return ret;
	}
	if (br_info->fid < 0) {
		if (!br_info->num_dev) {
			/* if num_dev under bridge is 0 then fid=-1 is ok. we can
			 * assume the dp_br_addif event may not yet received.
			 */
			DP_DEBUG(DP_DBG_FLAG_SWDEV, "DPM: %s: bridge_port_dev: %s, bridge_dev: %s, br_info->num_dev: %d, fid=%d\n"
					"                                May be dp_br_addif event not yet received.\n",
					__func__, dev->name, br_dev->name, br_info->num_dev, br_info->fid);
		} else {
			/* if num_dev != 0 under a bridge and still fid=-1 then
			 * its error. As it should have valid fid
			 */
			pr_err("DPM: %s: bridge_port_dev: %s, bridge_dev: %s, br_info->num_dev: %d, wrong fid=%d\n",
					__func__, dev->name, br_dev->name, br_info->num_dev, br_info->fid);
		}
		return ret;
	}
	parent_id = SWDEV_PARENT_ID(br_info->inst, br_info->fid);
	/* use dp instance + GSWIP bridge ID as parent port hardware id */
	ppid->id_len = sizeof(parent_id);
	dp_memcpy(&ppid->id, &parent_id, ppid->id_len);
	DP_DEBUG(DP_DBG_FLAG_SWDEV,
		 "bridge_port_dev=%s bridge_dev=%s, fid=%d done\n", dev->name,
		 br_dev->name,
		 br_info->fid);

	return 0;
}

/* Had better return -EOPNOTSUPP or 0 to avoid call trace dump */
int dp_ndo_get_phys_port_id2(struct net_device *dev,
			     struct netdev_phys_item_id *ppid)
{
	struct dp_dev *dp_dev;
	int ret = -EOPNOTSUPP;

	DP_DEBUG(DP_DBG_FLAG_SWDEV, "dev %s %s...\n", dev->name,
		 rtnl_is_locked() ? "rtnl_locked" : "rtnl_unlocked");
	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev) {
		pr_err("DPM: %s: dp_dev: %s\n", __func__, dev->name);
		return ret;
	}
	if (dp_dev->bp <= 0) {
		pr_info("DPM: %s: bp=%d: %s\n", __func__, dp_dev->bp,
		       dev->name);
		return ret;
	}
	ppid->id_len = snprintf(ppid->id, sizeof(ppid->id), "%d", dp_dev->bp);
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "dev %s done with port_id=%d\n", dev->name, dp_dev->bp);
	return 0;
}

/* Had better return -EOPNOTSUPP or 0 to avoid call trace dump */
int dp_ndo_get_phys_port_name2(struct net_device *dev,
			       char *name, size_t len)
{
	int err;

	DP_DEBUG(DP_DBG_FLAG_SWDEV, "dev port name %s %s\n", dev->name,
		 rtnl_is_locked() ? "rtnl_locked" : "rtnl_unlocked");

	err = dp_strscpy(name, dev->name, len);

	if (err >= len)
		return -EOPNOTSUPP;
	return 0;
}

int dp_register_switchdev_ops(struct net_device *dev, int reset)
{
	int offset;
	u32 flag = DP_OPS_NETDEV;
	struct dp_dev_subif *dev_sif;
	struct dp_dev *dp_dev;

	DP_DEBUG(DP_DBG_FLAG_OPS, "switchdev_ops %s for %s\n",
			 reset ? "reset" : "update", dev->name);
	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev)
		return DP_FAILURE;

	if (!reset) {
		if (dp_ndo_get_phys_port_id == dev->netdev_ops->ndo_get_phys_port_id) {
			pr_err("why device %s's ndo_get_phys_port_id already set to dp_ndo_get_phys_port_id\n",
			       dev->name);
			return DP_FAILURE;
		}
	} else {
		if (!dp_dev->f_ctp_dev &&
		    dp_ndo_get_phys_port_id != dev->netdev_ops->ndo_get_phys_port_id) {
			pr_err("why device %s's ndo_get_phys_port_id(%px) has not set to dp_ndo_get_phys_port_id(%px) yet\n",
			       dev->name,
			       dev->netdev_ops->ndo_get_phys_port_id,
			       dp_ndo_get_phys_port_id);
			return DP_FAILURE;
		}
	}

	dev_sif = list_first_entry(&dp_dev->subif_list, struct dp_dev_subif,
				   list);
	if (!dev_sif->sif->swdev_en)
		return DP_FAILURE;
	if (reset)
		flag |= DP_OPS_RESET;

	DP_SWDEV_LOCK(&dp_swdev_lock);
	offset = offsetof(const struct net_device_ops, ndo_get_port_parent_id);
	dp_set_net_dev_ops_priv(dev, &dp_ndo_get_port_parent_id, offset, flag,
				"dpm-ndo_get_port_parent_id");
	DP_DEBUG(DP_DBG_FLAG_OPS, "ndo_get_port_parent_id update for %s\n",
		 dev->name);

	offset = offsetof(const struct net_device_ops, ndo_get_phys_port_id);
	dp_set_net_dev_ops_priv(dev, &dp_ndo_get_phys_port_id, offset, flag,
				"dpm-ndo_get_phys_port_id");

	offset = offsetof(const struct net_device_ops, ndo_get_phys_port_name);
	dp_set_net_dev_ops_priv(dev, &dp_ndo_get_phys_port_name, offset, flag,
				"dpm-ndo_get_phys_port_name");

	offset = offsetof(const struct net_device_ops, ndo_bridge_setlink);
	dp_set_net_dev_ops_priv(dev, &dp_bp_ndo_br_setlink, offset, flag,
				"dpm-ndo_bridge_setlink");

	offset = offsetof(const struct net_device_ops, ndo_bridge_dellink);
	dp_set_net_dev_ops_priv(dev, &dp_bp_ndo_br_dellink, offset, flag,
				"dpm-ndo_bridge_dellink");
	DP_SWDEV_UNLOCK(&dp_swdev_lock);

	return DP_SUCCESS;
}

static struct dp_swdev_data *get_alloc_dev_swdev_data(
	struct net_device *dev)
{
	struct dp_swdev_data *data;

	data = dp_kzalloc(sizeof(*data), GFP_ATOMIC);
	if (!data)
		return NULL;
	if (dp_get_netif_subifid((struct net_device *)dev,
				  NULL, NULL, NULL, &data->subif, 0)) {
		kfree(data);
		pr_err("DPM: %s get subif fail, dev (%s) not registered\n",
		       __func__, dev->name);
		return NULL;
	}
	data->dev = dev;
	return data;

}

static bool dp_swdev_port_dev_check(const struct net_device *dev)
{
	return dp_valid_netif(dev);
}

static int dp_port_stp_state_set(struct dp_swdev_data *dev_data,
					    u8 state)
{
	switch (state) {
	case BR_STATE_FORWARDING:
		break;
	case BR_STATE_LEARNING:
		break;
	case BR_STATE_DISABLED:
		break;
	case BR_STATE_LISTENING:
	case BR_STATE_BLOCKING:
		return 0;
	default:
		return -EOPNOTSUPP;
	}
	dp_gswip_stp_set(dev_data, get_swdev_port_id(dev_data),
			 0, state);
	return 0;
}

static int dp_port_attr_vlan_filter_set(struct dp_swdev_data *dev_data,
					struct net_device *dev, bool vlan_filter)
{
	struct core_ops *gsw_ops;
	int vap;
	struct pmac_port_info *dp_port;
	struct dp_subif_info *sif = NULL;
	GSW_BRIDGE_portConfig_t *bpcfg = dp_kzalloc(sizeof(*bpcfg), GFP_ATOMIC);

	if (!bpcfg)
		return DP_FAILURE;

	if (!is_soc_lgm(dev_data->subif.inst)) {
		DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
			 "h/w vlan aware supported only on URX platform\n");
		kfree(bpcfg);
		return DP_SUCCESS;
	}

	dp_port = get_dp_port_info(dev_data->subif.inst,
				   dev_data->subif.port_id);
	vap = GET_VAP(dev_data->subif.subif, dp_port->vap_offset,
		      dp_port->vap_mask);
	sif = get_dp_port_subif(dp_port, vap);
	if (!sif->swdev_en) {
		DP_DEBUG(DP_DBG_FLAG_BR_VLAN, "swdev disabled on dev:%s\n",
			 dev->name ? dev->name : "NULL");
		kfree(bpcfg);
		return DP_SUCCESS;
	}

	gsw_ops = dp_port_prop[dev_data->subif.inst].ops[0];
	bpcfg->nBridgePortId = get_swdev_port_id(dev_data);
	if (!vlan_filter) {
		/* vlan_filtering disable on bp */
		if (sif->vlan_aware_en) {
			sif->vlan_aware_en = false;
			dp_gswip_vlan_mac_learning_en(bpcfg, false);
			if (gsw_ops->gsw_brdgport_ops.
				BridgePort_ConfigSet(gsw_ops, bpcfg)) {
				pr_err("DPM: %s: Fail in BridgePortCfgSet:\n",
				       __func__);
			}
			DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
				 "vlan aware disable on dev:%s(%d)\n",
				 dev->name, get_swdev_port_id(dev_data));
		}
	} else {
		/* vlan_filtering enable on bp */
		if (!sif->vlan_aware_en) {
			sif->vlan_aware_en = true;
			dp_gswip_vlan_mac_learning_en(bpcfg, true);
			if (gsw_ops->gsw_brdgport_ops.
				BridgePort_ConfigSet(gsw_ops, bpcfg)) {
				pr_err("DPM: %s: Fail in BridgePortCfgSet:\n",
				       __func__);
			}
			DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
				 "vlan aware enable on dev:%s(%d)\n",
				 dev->name, get_swdev_port_id(dev_data));
		}
	}
	kfree(bpcfg);
	return DP_SUCCESS;
}

static int dp_port_attr_br_flags_set(struct dp_swdev_data *dev_data,
	struct net_device *orig_dev,
	struct switchdev_brport_flags brport_flags)
{
	bool unreg_mcast_add = false;
	int ret = -EOPNOTSUPP;

	if (brport_flags.mask &
	    (BR_LEARNING | BR_MCAST_DEST_LOOKUP | BR_MCAST_SRC_LOOKUP)) {
		ret = dp_gswip_port_attr_bridge_flags_set(orig_dev,
							  brport_flags.val);
		if (ret) {
			DP_INFO("Setting flags for port %u failed\n",
				get_swdev_port_id(dev_data));
			return ret;
		}
	}

	if (brport_flags.mask & BR_MCAST_FLOOD)
		unreg_mcast_add = true;
	DP_DEBUG(DP_DBG_FLAG_SWDEV,
		 "BR_MCAST_FLOOD: %d port %u: %s\n",
		 unreg_mcast_add, get_swdev_port_id(dev_data),
		 unreg_mcast_add ? "unreg_add=true" : "unreg_add=false");
	if (dp_dbg_flag & DP_DBG_FLAG_SWDEV)
		dump_bp_flag(brport_flags);

	dp_gswip_set_unreg_mcast(dev_data, get_swdev_port_id(dev_data),
				 unreg_mcast_add);

	return 0;
}

static int dp_port_attr_br_flags_pre_set(
	struct dp_swdev_data *dev_data,
	struct switchdev_brport_flags flags)
{
	/* return _dp_port_attr_br_flags_get(dev_data->dev, &flags); */
	return 0;
}

static int dp_port_attr_set(struct net_device *dev, const void *ctx,
			    const struct switchdev_attr *attr,
			    struct netlink_ext_ack *extack)
{
	struct dp_swdev_data *dev_data = get_alloc_dev_swdev_data(dev);
	int ret = -EHWPOISON;

	if (!dev_data)
		return ret;

	switch (attr->id) {
	case SWITCHDEV_ATTR_ID_PORT_PRE_BRIDGE_FLAGS:
		DP_DEBUG(DP_DBG_FLAG_SWDEV,
			 "PORT_PRE_BRIDGE_FLAGS(id=%u) port=%u dev=%s %s\n",
			 attr->id,
			 get_swdev_port_id(dev_data),
			 dev->name,
			 rtnl_is_locked() ? "rtnl_locked" : "rtnl_unlocked");
#if 0 /* I think it is wrong. switchdev_trans_ph_prepare */
		ret = dp_port_attr_br_flags_pre_set(dev_data,
						    attr->u.brport_flags);
#endif
		ret = 0;
		break;
	case SWITCHDEV_ATTR_ID_PORT_STP_STATE:
		DP_DEBUG(DP_DBG_FLAG_SWDEV,
			 "PORT_STP_STATE(id=%u) port=%u dev=%s %s\n",
			 attr->id,
			 get_swdev_port_id(dev_data),
			 dev->name,
			 rtnl_is_locked() ? "rtnl_locked" : "rtnl_unlocked");
		ret = dp_port_stp_state_set(dev_data, attr->u.stp_state);
		trace_dp_swdev_event(NULL, attr->u.stp_state, dev, attr);
		break;
	case SWITCHDEV_ATTR_ID_PORT_BRIDGE_FLAGS:
		DP_DEBUG(DP_DBG_FLAG_SWDEV,
			 "PORT_BRIDGE_FLAGS(id=%u) port=%u dev=%s %s\n",
			 attr->id,
			 get_swdev_port_id(dev_data),
			 dev->name,
			 rtnl_is_locked() ? "rtnl_locked" : "rtnl_unlocked");
		ret = dp_port_attr_br_flags_set(dev_data, attr->orig_dev,
						attr->u.brport_flags);
		trace_dp_swdev_event(NULL, attr->u.brport_flags.val, dev, attr);
		break;
	case SWITCHDEV_ATTR_ID_BRIDGE_VLAN_FILTERING:
		DP_DEBUG(DP_DBG_FLAG_SWDEV,
			 "BRIDGE_VLAN_FILTER(id=%u) port=%u dev=%s %s\n",
			 attr->id,
			 get_swdev_port_id(dev_data),
			 dev->name,
			 rtnl_is_locked() ? "rtnl_locked" : "rtnl_unlocked");
		ret = dp_port_attr_vlan_filter_set(dev_data, dev,
						   attr->u.vlan_filtering);
		trace_dp_swdev_event(NULL, attr->u.vlan_filtering, dev, attr);
		break;
#ifndef DP_SWITCHDEV_NO_HACKING
	case SWITCHDEV_ATTR_ID_PORT_LEARNING_LIMIT:
		DP_DEBUG(DP_DBG_FLAG_SWDEV,
			 "BRIDGE_PORT_LEARNING_LIMIT(id=%u) port=%u dev=%s %s\n",
			 attr->id,
			 get_swdev_port_id(dev_data),
			 dev->name,
			 rtnl_is_locked() ? "rtnl_locked" : "rtnl_unlocked");
		ret = _dp_swdev_port_attr_learning_limit_set(dev, attr->u.learning_limit);
		trace_dp_swdev_event(NULL, attr->u.learning_limit, dev, attr);
		break;
#endif

#ifndef DP_SWITCHDEV_NO_HACKING
	case SWITCHDEV_ATTR_ID_BRIDGE_MCAST_FLOOD:
		DP_DEBUG(DP_DBG_FLAG_SWDEV,
			 "BRIDGE_MCAST_FLOOD(id=%u) port=%u dev=%s %s\n",
			 attr->id,
			 get_swdev_port_id(dev_data),
			 dev->name,
			 rtnl_is_locked() ? "rtnl_locked" : "rtnl_unlocked");
		ret = _dp_swdev_bridge_attr_mcast_flood(dev, attr->u.mcast_flood);
		trace_dp_swdev_event(NULL, attr->u.mcast_flood, dev, attr);
		break;
#endif
	default:
		DP_DEBUG(DP_DBG_FLAG_SWDEV,
			 "id=%u port=%u dev=%s %s\n",
			 attr->id,
			 get_swdev_port_id(dev_data),
			 dev->name,
			 rtnl_is_locked() ? "rtnl_locked" : "rtnl_unlocked");
		ret = -EOPNOTSUPP;
		break;
	}

	kfree(dev_data);
	return ret;
}

static u16 dp_get_pvid(struct dp_swdev_data *dev_data)
{
	u32 pvid;
	pvid = get_swdev_port_id(dev_data);
	return pvid;
}

static void dp_set_pvid(struct dp_swdev_data *dev_data, u16 vid, bool cfi, u32 cos)
{
	DP_DEBUG(DP_DBG_FLAG_SWDEV,
		 "dev=%s vid=%u cfi=%d cos=%u\n",
		 dev_data->dev->name,
		 vid, cfi, cos);
}

static int dp_port_vlan_add(struct dp_swdev_data *dev_data, bool untag, bool pvid,
			      u16 vid, struct net_device *orig_dev)
{
	bool cpu_port = netif_is_bridge_master(orig_dev);
	int unreg_mcast_mask = 0;
	int reg_mcast_mask = 0;
	int untag_mask = 0;
	int port_mask;
	int ret = 0;
	u32 flags;

	if (cpu_port) {
		port_mask = BIT(DP_CPU_LPID);
		flags = orig_dev->flags;
		unreg_mcast_mask = port_mask;
	} else {
		port_mask = BIT(get_swdev_port_id(dev_data));
		flags = dev_data->dev->flags;
	}

	if (flags & IFF_MULTICAST)
		reg_mcast_mask = port_mask;

	if (untag)
		untag_mask = port_mask;

	ret = dp_gswip_vlan_add_modify(dev_data, vid, port_mask, untag_mask,
				       reg_mcast_mask, unreg_mcast_mask);
	if (ret) {
		DP_INFO("Unable to add vlan\n");
		return ret;
	}

	if (cpu_port)
		dp_gswip_add_ucast(dev_data, dev_data->dev->dev_addr,
				   DP_CPU_LPID, 0, vid);
	if (!pvid)
		return ret;

	dp_set_pvid(dev_data, vid, 0, 0);

	DP_INFO("VID add: %s: vid:%u ports:%X\n",
		dev_data->dev->name, vid, port_mask);
	return ret;
}

static int dp_port_vlan_del(struct dp_swdev_data *dev_data, u16 vid,
			      struct net_device *orig_dev)
{
	bool cpu_port = netif_is_bridge_master(orig_dev);
	int port_mask;
	int ret = 0;

	if (cpu_port)
		port_mask = BIT(DP_CPU_LPID);
	else
		port_mask = BIT(get_swdev_port_id(dev_data));

	ret = dp_gswip_del_vlan(dev_data, vid, port_mask);
	if (ret != 0)
		return ret;

	/* We don't care for the return value here, error is returned only if
	 * the unicast entry is not present
	 */
	if (cpu_port)
		dp_gswip_del_ucast(dev_data, dev_data->dev->dev_addr,
				   DP_CPU_LPID, 0, vid);

	if (vid == dp_get_pvid(dev_data))
		dp_set_pvid(dev_data, 0, 0, 0);

	/* We don't care for the return value here, error is returned only if
	 * the multicast entry is not present
	 */
	dp_gswip_del_mcast(dev_data, dev_data->dev->broadcast,
			   port_mask, 0, vid);
	DP_INFO("VID del: %s: vid:%u ports:%X\n",
		dev_data->dev->name, vid, port_mask);

	return ret;
}

static int dp_port_vlans_add(struct dp_swdev_data *dev_data,
			       const struct switchdev_obj_port_vlan *vlan)
{
	bool untag = vlan->flags & BRIDGE_VLAN_INFO_UNTAGGED;
	struct net_device *orig_dev = vlan->obj.orig_dev;
	bool cpu_port = netif_is_bridge_master(orig_dev);
	bool pvid = vlan->flags & BRIDGE_VLAN_INFO_PVID;
	u16 vid;

	DP_INFO("VID add: %s: vid:%u flags:%X\n",
		dev_data->dev->name, vlan->vid, vlan->flags);

	if (cpu_port && !(vlan->flags & BRIDGE_VLAN_INFO_BRENTRY))
		return 0;

	for (vid = vlan->vid; vid <= vlan->vid; vid++) {
		int err;

		err = dp_port_vlan_add(dev_data, untag, pvid, vid, orig_dev);
		if (err)
			return err;
	}

	return 0;
}

static int dp_port_vlans_del(struct dp_swdev_data *dev_data,
			       const struct switchdev_obj_port_vlan *vlan)

{
	struct net_device *orig_dev = vlan->obj.orig_dev;
	u16 vid;

	for (vid = vlan->vid; vid <= vlan->vid; vid++) {
		int err;

		err = dp_port_vlan_del(dev_data, vid, orig_dev);
		if (err)
			return err;
	}

	return 0;
}

static int dp_port_mdb_add(struct dp_swdev_data *dev_data,
			     struct switchdev_obj_port_mdb *mdb)

{
	struct net_device *orig_dev = mdb->obj.orig_dev;
	bool cpu_port = netif_is_bridge_master(orig_dev);
	int port_mask;
	int err;

	if (cpu_port)
		port_mask = BIT(DP_CPU_LPID);
	else
		port_mask = BIT(get_swdev_port_id(dev_data));

	err = dp_gswip_add_mcast(dev_data, mdb->addr, port_mask,
				 0, mdb->vid, 0);
	DP_INFO("MDB add: %s: vid %u: 0x%pxM  ports: %X\n",
		dev_data->dev->name, mdb->vid, mdb->addr, port_mask);

	return err;
}

static int dp_port_mdb_del(struct dp_swdev_data *dev_data,
			     struct switchdev_obj_port_mdb *mdb)

{
	struct net_device *orig_dev = mdb->obj.orig_dev;
	bool cpu_port = netif_is_bridge_master(orig_dev);
	int del_mask;
	int err;

	if (cpu_port)
		del_mask = BIT(DP_CPU_LPID);
	else
		del_mask = BIT(get_swdev_port_id(dev_data));

	err = dp_gswip_del_mcast(dev_data, mdb->addr, del_mask,
				 0, mdb->vid);
	DP_INFO("MDB del: %s: vid %u: 0x%pxM  ports: %X\n",
		dev_data->dev->name, mdb->vid, mdb->addr, del_mask);

	return err;
}

static int dp_port_obj_add(struct net_device *dev, const void *ctx,
			     const struct switchdev_obj *obj,
			     struct netlink_ext_ack *extack)
{
	int err = 0;

#ifdef need_check_later
	struct switchdev_obj_port_vlan *vlan = SWITCHDEV_OBJ_PORT_VLAN(obj);
	struct dp_swdev_data *dev_data = get_alloc_dev_swdev_data(dev);

	DP_DEBUG(DP_DBG_FLAG_SWDEV, "obj_add: id=%s dev=%s\n",
		 get_swdev_obj_add_name(obj->id), dev->name);

	if (!dev_data) {
		pr_err("DPM: %s: get_alloc_dev_swdev_data fail: %s\n",
		       __func__, dev->name);
		return NOTIFY_DONE;
	}
	switch (obj->id) {
	case SWITCHDEV_OBJ_ID_PORT_VLAN: /* add VLAN */
		err = dp_port_vlans_add(dev_data, vlan);
		break;
	case SWITCHDEV_OBJ_ID_PORT_MDB: /* Multicast: so far no need to support */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0))
	case SWITCHDEV_OBJ_ID_HOST_MDB:
#endif
		/*err = dp_port_mdb_add(dev_data, mdb); */
		break;
	default:
		err = -EOPNOTSUPP;
		break;
	}
	kfree(dev_data);
#endif
	return err;
}

static int dp_port_obj_del(struct net_device *dev, const void *ctx,
			     const struct switchdev_obj *obj)
{
	int err = 0;

#ifdef need_check_later
	struct switchdev_obj_port_vlan *vlan = SWITCHDEV_OBJ_PORT_VLAN(obj);
	struct switchdev_obj_port_mdb *mdb = SWITCHDEV_OBJ_PORT_MDB(obj);
	struct dp_swdev_data *dev_data = get_alloc_dev_swdev_data(dev);

	DP_DEBUG(DP_DBG_FLAG_SWDEV, "obj_del: id=%s dev=%s\n",
		 get_swdev_obj_add_name(obj->id), dev->name);

	if (!dev_data) {
		pr_err("DPM: %s: get_alloc_dev_swdev_data fail: %s\n",
		       __func__, dev->name);
		return NOTIFY_DONE;
	}
	switch (obj->id) {
	case SWITCHDEV_OBJ_ID_PORT_VLAN: /* vlan */
		err = dp_port_vlans_del(dev_data, vlan);
		break;
	case SWITCHDEV_OBJ_ID_PORT_MDB: /* multicast */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0))
	case SWITCHDEV_OBJ_ID_HOST_MDB: /* multicast */
#endif
		err = dp_port_mdb_del(dev_data, mdb);
		break;
	default:
		err = -EOPNOTSUPP;
		break;
	}
	kfree(dev_data);
#endif
	return err;
}

static void dp_fdb_offload_notify(struct net_device *dev,
				    struct switchdev_notifier_fdb_info *rcv)
{
	struct switchdev_notifier_fdb_info info;

	info.addr = rcv->addr;
	info.vid = rcv->vid;
	info.offloaded = true;
	call_switchdev_notifiers(SWITCHDEV_FDB_OFFLOADED,
				 dev, &info.info, NULL);
}

static void dp_switchdev_event_work(struct work_struct *work)
{
	struct dp_switchdev_event_work *switchdev_work =
		container_of(work, struct dp_switchdev_event_work, work);
	struct dp_swdev_data *dev_data = switchdev_work->dev_data;
	struct switchdev_notifier_fdb_info *fdb;
	int port = get_swdev_port_id(dev_data);

	rtnl_lock();
	switch (switchdev_work->event) {
	case SWITCHDEV_FDB_ADD_TO_DEVICE:
		fdb = &switchdev_work->fdb_info;

		DP_DEBUG(DP_DBG_FLAG_SWDEV,
			 "dp_fdb_add MAC=0x%pxM vid=%u flags=%u %u--port=%d\n",
			 fdb->addr, fdb->vid, fdb->added_by_user,
			 fdb->offloaded, port);

		if (!fdb->added_by_user)
			break;
#ifdef need_check_later
		if (memcmp(priv->mac_addr, (u8 *)fdb->addr, ETH_ALEN) == 0)
			port = DP_CPU_LPID;
#endif
		dp_gswip_add_ucast(dev_data, (u8 *)fdb->addr, port,
				   fdb->vid ? 0 : 0, fdb->vid);
		dp_fdb_offload_notify(dev_data->dev, fdb);
		break;
	case SWITCHDEV_FDB_DEL_TO_DEVICE:
		fdb = &switchdev_work->fdb_info;

		DP_DEBUG(DP_DBG_FLAG_SWDEV,
			 "dp_fdb_del MAC=0x%pxM vid=%u flags=%u %u--port=%d\n",
			fdb->addr, fdb->vid, fdb->added_by_user,
			fdb->offloaded, port);

		if (!fdb->added_by_user)
			break;
#ifdef need_check_later
		if (memcmp(priv->mac_addr, (u8 *)fdb->addr, ETH_ALEN) == 0)
			port = DP_CPU_LPID;
#endif
		dp_gswip_del_ucast(dev_data, (u8 *)fdb->addr, port,
				   fdb->vid ? 0 : 0, fdb->vid);
		break;
	default:
		break;
	}
	rtnl_unlock();

	kfree(switchdev_work->fdb_info.addr);
	dev_put(dev_data->dev);
	kfree(switchdev_work->dev_data);
	kfree(switchdev_work);
}

static int dp_switchdev_event(struct notifier_block *unused,
				unsigned long event, void *ptr)
{
	struct net_device *dev = switchdev_notifier_info_to_dev(ptr);
	struct switchdev_notifier_fdb_info *fdb_info = ptr;
	struct dp_switchdev_event_work *switchdev_work = NULL;
	struct dp_swdev_data *dev_data = NULL;
	int err;

	if (!dp_init_ok)
		return 0;
	DP_DEBUG(DP_DBG_FLAG_SWDEV,"event type=%s dev=%s: %s\n",
		 get_swdev_event_name(event), dev->name,
		 rtnl_is_locked() ? "rtnl_locked" : "rtnl_unlocked");

	if (event == SWITCHDEV_PORT_ATTR_SET) {
		err = switchdev_handle_port_attr_set(dev, ptr,
						     dp_swdev_port_dev_check,
						     dp_port_attr_set);
		return notifier_from_errno(err);
	}

	return NOTIFY_DONE; /* return & Disable bridge fdb feature for now */

	if (!dp_swdev_port_dev_check(dev)) {
		DP_DEBUG(DP_DBG_FLAG_SWDEV,"event type=%s: dev=%s not valid\n",
			 get_swdev_event_name(event), dev->name);
		return NOTIFY_DONE;
	}

	dev_data = get_alloc_dev_swdev_data(dev);
	if (!dev_data) {
		pr_err("DPM: %s: get_alloc_dev_swdev_data fail: %s\n",
		       __func__, dev->name);
		goto err_addr_alloc;
	}
	switchdev_work = dp_kzalloc(sizeof(*switchdev_work), GFP_ATOMIC);
	if (!switchdev_work) {
		pr_err("DPM: dpm: %s switchdev_work NULL\n", __func__);
		goto err_addr_alloc;
	}

	INIT_WORK(&switchdev_work->work, dp_switchdev_event_work);
	switchdev_work->dev_data = dev_data;
	switchdev_work->event = event;

	switch (event) {
	case SWITCHDEV_FDB_ADD_TO_DEVICE:
	case SWITCHDEV_FDB_DEL_TO_DEVICE:
		dp_memcpy(&switchdev_work->fdb_info, ptr,
			  sizeof(switchdev_work->fdb_info));
		switchdev_work->fdb_info.addr = dp_kzalloc(ETH_ALEN,
							   GFP_ATOMIC);
		if (!switchdev_work->fdb_info.addr)
			goto err_addr_alloc;
		ether_addr_copy((u8 *)switchdev_work->fdb_info.addr,
				fdb_info->addr);
		dev_hold(dev);
		break;
	default:
		DP_DEBUG(DP_DBG_FLAG_SWDEV, "skip event type=%s dev=%s\n",
			 get_swdev_event_name(event), dev->name);
		kfree(switchdev_work);
		kfree(dev_data);
		return NOTIFY_DONE;
	}

	/* workqueue function dp_switchdev_event_work */
	queue_work(system_long_wq, &switchdev_work->work);

	return NOTIFY_DONE;

err_addr_alloc:
	kfree(dev_data);
	kfree(switchdev_work);
	return NOTIFY_BAD;
}

static struct notifier_block dp_switchdev_notifier = {
	.notifier_call = dp_switchdev_event,
};

static int dp_switchdev_blocking_event(struct notifier_block *unused,
					 unsigned long event, void *ptr)
{
	struct net_device *dev = switchdev_notifier_info_to_dev(ptr);
	int err;

	if (!dp_init_ok)
		return 0;
		;
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "event type=%s dev=%s: %s\n",
		 get_swdev_event_name(event), dev->name,
		 rtnl_is_locked() ? "rtnl_locked" : "rtnl_unlocked");

	DP_SWDEV_LOCK(&dp_swdev_lock);

	switch (event) {
	case SWITCHDEV_PORT_OBJ_ADD:
		err = switchdev_handle_port_obj_add(dev, ptr,
						    dp_swdev_port_dev_check,
						    dp_port_obj_add);
		break;
	case SWITCHDEV_PORT_OBJ_DEL:
		err = switchdev_handle_port_obj_del(dev, ptr,
						    dp_swdev_port_dev_check,
						    dp_port_obj_del);
		break;
	case SWITCHDEV_PORT_ATTR_SET:
		err = switchdev_handle_port_attr_set(dev, ptr,
						     dp_swdev_port_dev_check,
						     dp_port_attr_set);
		break;
	default:
		DP_DEBUG(DP_DBG_FLAG_SWDEV, " skip event type=%s dev=%s\n",
			 get_swdev_event_name(event), dev->name);
		err = 0;
		break;
	}
	DP_SWDEV_UNLOCK(&dp_swdev_lock);

	return notifier_from_errno(err);
}

static struct notifier_block dp_switchdev_bl_notifier = {
	.notifier_call = dp_switchdev_blocking_event,
};

int dp_switchdev_register_notifiers(void)
{
	int ret = 0;

	INIT_LIST_HEAD(&swdev_evt_data);
	ret = register_switchdev_notifier(&dp_switchdev_notifier);
	if (ret) {
		pr_err("DPM: register switchdev notifier fail ret:%d\n", ret);
		return ret;
	}

	ret = register_switchdev_blocking_notifier(&dp_switchdev_bl_notifier);
	if (ret) {
		pr_err("DPM: register switchdev blocking notifier ret:%d\n", ret);
		unregister_switchdev_notifier(&dp_switchdev_notifier);
	}

	return ret;
}

void dp_switchdev_unregister_notifiers(void)
{
	unregister_switchdev_blocking_notifier(&dp_switchdev_bl_notifier);
	unregister_switchdev_notifier(&dp_switchdev_notifier);
}
#endif /* LINUX_VERSION_CODE */
#else /* CONFIG_DPM_DATAPATH_SWITCHDEV */
int dp_register_switchdev_ops(struct net_device *dev, int reset)
{
	return 0;
}
#endif /* end of CONFIG_DPM_DATAPATH_SWITCHDEV */

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP
