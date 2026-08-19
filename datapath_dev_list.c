// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2022, MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <linux/types.h>
#include <linux/if_vlan.h>
#include <net/datapath_api.h>
#include <net/datapath_proc_api.h>
#include "datapath.h"
#include "datapath_instance.h"
#include "datapath_swdev.h"
#include "datapath_ioctl.h"
#include "datapath_switchdev.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

void dp_ndo_uninit(struct net_device *dev)
{
	struct dp_dev *dp_dev;

	if (!dev)
		return;
	DP_LIB_LOCK(&dp_lock);
	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev) {
		DP_LIB_UNLOCK(&dp_lock);
		pr_err("DPM: %s: dp_dev_lookup fail for %s\n", __func__, dev->name);
		return;
	}
	if (dp_dev->old_dev_ops && dp_dev->old_dev_ops->ndo_uninit) {
		DP_LIB_UNLOCK(&dp_lock);
		/* Note: need unlock in case ndo_uninit trigger DP APIs,
		 * like try to de-register subif from dpm
		 */
		dp_dev->old_dev_ops->ndo_uninit(dev);
		dp_dev = NULL;

		/* lock again */
		DP_LIB_LOCK(&dp_lock);
	}

	/* delete this dev from dev list */
	dp_del_dev(dev, DP_DEL_DEV_NDO_UNINIT);

	/* santity check whether really deleted or not from dp_dev list */
	dp_dev = dp_dev_lookup(dev);
	if (dp_dev)
		pr_err("DPM: %s fail: %s ndo_uninit trigger with %s=%d %s=%d\n",
		       __func__, dev->name,
		       "all_ops_cnt", dp_dev->all_ops_cnt,
		       "count", dp_dev->count);
	DP_LIB_UNLOCK(&dp_lock);
	DP_DEBUG(DP_DBG_FLAG_OPS, "%s %s ndo_uninit done with netdev_ops=0x%px\n",
		 __func__, dev->name, dev->netdev_ops);
}

void dp_free_remaining_dev_list(void)
{
	struct hlist_head *head;
	struct dp_dev *pos;
	struct hlist_node *n;
	u32 idx;

	for (idx = 0; idx < ARRAY_SIZE(dp_dev_list); idx++) {
		head = &dp_dev_list[idx];

		hlist_for_each_entry_safe(pos, n, head, hlist) {
			if (pos->count > 0) {
				pr_err("DPM: %s:%s still register to dpm yet\n",
				       __func__, pos->dev->name);
				continue;
			}
			if (pos->all_ops_cnt > 0) {
				pr_err("DPM: %s:%s still with all_ops_cnt=%d\n",
				       __func__, pos->dev->name,
				       pos->all_ops_cnt);
				continue;
			}
			hlist_del(&pos->hlist);
			kmem_cache_free(cache_dev_list, pos);
		}
	}
}

int dp_set_netdev_ndo_uninit(struct dp_dev *dp_dev, u32 f_reset)
{
	void *ndo_cb = &dp_ndo_uninit;
	int ndo_offset = offsetof(const struct net_device_ops, ndo_uninit);
	u32 flag = DP_OPS_NETDEV;

	if (!f_reset) { /*sanity check */
		if (dp_ndo_uninit == dp_dev->dev->netdev_ops->ndo_uninit) {
			pr_err("why device %s's ndo_uninit already set to dp_ndo_uninit\n",
			       dp_dev->dev->name);
			return -1;
		}
	} else if (dp_ndo_uninit != dp_dev->dev->netdev_ops->ndo_uninit) {
		pr_err("why device %s's ndo_uninit not set to dp_ndo_uninit yet\n",
		       dp_dev->dev->name);
		return -1;
	}

	if (f_reset)
		flag |= DP_OPS_RESET;
	return dp_set_net_dev_ops_priv(dp_dev->dev, ndo_cb, ndo_offset, flag,
				       "dpm-ndo_uninit");
}

/* br_flag: DP_BR_NOCHANGE, DP_BR_JOIN, DP_BR_LEAVE, ... */
int dp_br_addif(struct net_device *br_dev, struct net_device *dev)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	struct br_info *br_info;
	struct bridge_member_port *pos;
	struct dp_dev *dp_dev;
	int ret = 0;

	DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s %s ...\n", br_dev->name, dev->name);
	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev) {
		DP_DEBUG(DP_DBG_FLAG_SWDEV, "dp_dev_lookup fail: %s\n",
			 dev->name);
		ret = -1;
		goto exit;
	}
	DP_SWDEV_LOCK(&dp_swdev_lock);
	br_info = dp_swdev_bridge_entry_lookup(br_dev);
	if (!br_info) {
		pr_err("DPM: %s: why dp_swdev_bridge_entry_lookup fail: %s\n",
		       __func__, br_dev->name);
		goto unlock;
	}
	if (!dp_dev->count) {
		DP_DEBUG(DP_DBG_FLAG_SWDEV,
			 "%s not dpm-registered yet, so dont add intf to bridge\n",
			 dev->name);
		goto unlock;
	}

	/* Avoid duplicate:
	 * Lookup if bridge_port_dev already exist, if yes do nothing
	 */
	list_for_each_entry(pos, &br_info->bp_list, list) {
		if (pos->dev == dev) {
			DP_DEBUG(DP_DBG_FLAG_SWDEV,
				 "bridge_port_dev: %s already exists in bridge_dev: %s, may be re-registering to DPM\n",
				 dev->name, br_dev->name);
			_dp_alloc_fid_misc(dp_dev, br_info);
			goto unlock;
		}
	}

	pos = dp_kzalloc(sizeof(*pos), GFP_ATOMIC);
	if (!pos) {
		ret = -1;
		goto unlock;
	}

	INIT_LIST_HEAD(&pos->bport_vlan_list);
	pos->dev = dev;

	/* link bridge_member_port to br_info */
	pos->br_info = br_info;

	/* link bridge_member_port to dp_dev */
	pos->dp_dev = dp_dev;

	br_info->num_dev++;
	if (dp_dev->count) /* registered to dpm */ {
		if (dp_dev->f_ctp_dev) {
			pr_err("DPM: %s: ctp device %s with in bridge?\n", __func__,
			       dp_dev->dev->name);
			kfree(pos);
			ret = -1;
			goto unlock;
		}
	}

	/* link dp_dev to br_info/br_member_pos */
	dp_dev->br_info = br_info;
	dp_dev->br_member_port = pos;

	list_add(&pos->list, &br_info->bp_list);

	_dp_alloc_fid_misc(dp_dev, br_info);
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s %s done\n", br_dev->name, dev->name);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 0) && IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	DP_SWDEV_UNLOCK(&dp_swdev_lock);
	switchdev_bridge_port_offload(dev, dev, NULL, NULL, NULL, true, NULL);
	return 0;
#endif
unlock:
	DP_SWDEV_UNLOCK(&dp_swdev_lock);
exit:
	return ret;
#endif
	return 0;
}

int dp_br_delif(struct net_device *br_dev,
		     struct net_device *dev)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	struct br_info *br_info;
	struct bridge_member_port *pos;
	struct dp_dev *dp_dev;
	int ret = 0;

	DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s %s ...\n", br_dev->name, dev->name);
	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev) {
		DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s: dp_dev_lookup fail: %s\n",
			 __func__, dev->name);
		ret = -1;
		goto exit;
	}
	DP_SWDEV_LOCK(&dp_swdev_lock);
	br_info = dp_dev->br_info;
	/* unlink dp_dev to br_info/br_member_port */
	dp_dev->br_info = NULL;
	dp_dev->br_member_port = NULL;

	/* remove dev from br_info->dp_list */
	if (!br_info) {
		pr_err("DPM: %s: br_info NULL although %s in %s\n",
		       __func__, dev->name, br_dev->name);
		goto unlock;
	}
	list_for_each_entry(pos, &br_info->bp_list, list) {
		if (pos->dev != dev)
			continue;
		/* update br_info counter */
		br_info->num_dev--;
		list_del(&pos->list);

		if (!br_info->num_dev && (br_info->inst >= 0) &&
		    (br_info->fid >= 0))
			_dp_free_fid(dp_dev, br_info);
		kfree(pos);
		DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s %s done\n", br_dev->name,
			 dev->name);
		goto found;
	}
	pr_err("DPM: %s: no match dev foudn in br_info->bp_list: %s in br %s\n",
	       __func__, dev->name, br_dev->name);
	goto unlock;
found:
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 0) && IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	DP_SWDEV_UNLOCK(&dp_swdev_lock);
	switchdev_bridge_port_unoffload(dev, NULL, NULL, NULL);
	return 0;
#endif
unlock:
	DP_SWDEV_UNLOCK(&dp_swdev_lock);
exit:
	return ret;
#endif
	return 0;
}

/* This API will be called only in netdevice notifer */
int auto_register_vlan(dp_subif_t *subif, struct net_device *dev)
{
	struct net_device *base;
	struct pmac_port_info *p_info;
	struct dp_subif_data data = {0};
	struct bp_pmapper *bp_info;

	if (!is_vlan_dev(dev))
		return DP_FAILURE;

	base = get_base_dev(dev, -1);
	if (!base)
		base = dev;
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "base dev name:%s\n", base->name);

	if (dp_get_netif_subifid(base, NULL, NULL, NULL, subif, 0)) {
		pr_err("DPM: %s get subif fail for %s\n",
		       __func__, base->name);
		return DP_FAILURE;
	}

	subif->subif = -1;
	p_info = get_dp_port_info(subif->inst, subif->port_id);
	bp_info = get_dp_bp_info(subif->inst, subif->bport);
	data.flag_ops |= DP_SUBIF_BR_DOMAIN;
	data.domain_id = bp_info->domain_id;
	data.domain_members = bp_info->domain_member;
	if (dp_register_subif_ext(subif->inst, p_info->owner, dev,
				  dev->name, subif, &data,
				  DP_F_SUBIF_LOGICAL)) {
		pr_err("DPM: %s register subif-%s fail\n", __func__, dev->name);
		return DP_FAILURE;
	}
	return DP_SUCCESS;
}

/* This API will be called only in netdevice notifer */
int auto_deregister_vlan(struct net_device *dev)
{
	dp_subif_t *subif;
	struct pmac_port_info *p_info;

	if (!is_vlan_dev(dev))
		return DP_FAILURE;
	subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif)
		return DP_FAILURE;
	p_info = get_dp_port_info(subif->inst, subif->port_id);
	if (dp_get_netif_subifid(dev, NULL, NULL, NULL, subif, 0)) {
		kfree(subif);
		pr_err("DPM: %s get subif fail for %s\n",
		       __func__, dev->name);
		return DP_FAILURE;
	}
	if (dp_register_subif_ext(subif->inst, p_info->owner, dev,
				  dev->name, subif, NULL,
				  DP_F_SUBIF_LOGICAL)) {
		kfree(subif);
		pr_err("DPM: %s register subif-%s fail\n", __func__, dev->name);
		return DP_FAILURE;
	}
	kfree(subif);
	return DP_SUCCESS;
}

void dflt_dp_dev(struct dp_dev *dp_dev)
{
	dp_dev->inst = -1;
	dp_dev->ep = -1;
	dp_dev->bp = -1;
	/*dp_dev->ctp = -1; */ /* dont' use in new design */
	dp_dev->fid = -1;
}

 /* This API is called when this device is registered to network stack via
  * netdev event, like NETDEV_REGISTER
  */
int dp_add_dev(struct net_device *dev)
{
	struct dp_dev *dp_dev;
	int idx;

	if (!dev) {
		pr_err("DPM: %s dev is NULL\n", __func__);
		return DP_FAILURE;
	}
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "to add dev to devlist %s\n", dev->name);
	dp_dev = dp_dev_lookup(dev);
	if (dp_dev) {
#ifndef DP_NOT_USE_NETDEV_REGISTER
		if (dp_dev->f_add_dev_wa) {
			dp_dev->f_add_dev_wa = false;
			return DP_SUCCESS;
		}
#endif
		pr_err("DPM: %s: why %s already exist\n", __func__, dev->name);
		return DP_FAILURE;
	}
	dp_dev = kmem_cache_zalloc(cache_dev_list, GFP_ATOMIC);
	if (!dp_dev)
		return DP_FAILURE;

	dflt_dp_dev(dp_dev);
	dp_dev->dev = dev;
	dp_dev->bp_learning_limit = GSWIP_LEARN_LIMIT_PORT_MAX;
	dp_dev->learning_limit_en = 1;
	INIT_LIST_HEAD(&dp_dev->subif_list);
	idx = dp_dev_hash(dev);
	hlist_add_head(&dp_dev->hlist, &dp_dev_list[idx]);
	dp_set_netdev_ndo_uninit(dp_dev, 0);
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "add new dev: %s done\n", dev->name);
	return DP_SUCCESS;
}

/* call from NETDEV_CHANGEUPPER */
int dp_update_dev_br(struct net_device *dev,
			struct net_device *br_dev,
			int br_flag)
{
	if (br_flag == DP_BR_JOIN) { /* from NETDEV_CHANGEUPPER */
		dp_br_addif(br_dev, dev);
		return DP_SUCCESS;
	}
	if (br_flag == DP_BR_LEAVE) { /* from NETDEV_CHANGEUPPER */
		dp_br_delif(br_dev, dev);
		return DP_SUCCESS;
	}
	return DP_SUCCESS;
}

/* make sure inst/ep/bp is already correct in dp_dev */
int _dp_alloc_fid_misc(struct dp_dev *dp_dev,
				struct br_info *br_info)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	struct inst_info *inst_info;
	int br_id, i;
	struct inst_property *prop;
	struct pmac_port_info *port;
	struct dp_subif_info *sif;

	if (!dp_dev || !br_info)
		return DP_FAILURE;

	if (!dp_dev->count) /* non dpm supported device */
		return DP_SUCCESS;

	if (dp_dev->inst < 0) {
		pr_err("DPM: %s: wrong inst=%d\n", __func__, dp_dev->inst);
		return DP_FAILURE;
	}
	if (dp_dev->ep < 0) {
		pr_err("DPM: %s: wrong ep=%d\n", __func__, dp_dev->ep);
		return DP_FAILURE;
	}
	if (!dp_dev->br_member_port) {
		pr_err("DPM: %s: br_member_port NULL for dev:%s\n",
		       __func__, dp_dev->dev->name);
		return DP_FAILURE;
	}

	if (!br_info->num_dev) /* no device under this bridge register to dpm yet */
		return DP_SUCCESS;

	if(br_info->fid <= 0) {
		inst_info= get_dp_prop_info(dp_dev->inst);
		br_id = inst_info->swdev_alloc_bridge_id(dp_dev->inst);
		if (br_id < 0) {
			pr_err("DPM: %s: Switch bridge alloc failed:%s\n", __func__,
				br_info->dev->name);
			return DP_FAILURE;
		}
		inst_info->swdev_bridge_cfg_set(dp_dev->inst, br_id);
		br_info->fid = br_id;
		dp_set_cpu_mac(br_info->dev, false);
	}
	/* update all sif */
	port = get_dp_port_info(dp_dev->inst, dp_dev->ep);
	prop = &dp_port_prop[dp_dev->inst];
	for (i = 0; i < port->subif_max; i++) {
		if (get_dp_port_subif(port, i)->netif == dp_dev->dev) {
			sif = get_dp_port_subif(port, i);
			if (dp_dev->dev->flags & IFF_UP)
				sif->fid = br_info->fid;
		}
	}

	/* later need further update sif rcu link list and logic dev */

	/* update fid for dp_dev */
	dp_dev->fid = br_info->fid;
	if (dp_dev->br_info != br_info) {
		pr_err("DPM: %s: wrong br_info pointer: %s\n", __func__,
		       dp_dev->dev->name);
		return DP_FAILURE;
	}

	/* update br_info from dp_dev */
	if (dp_dev->count) {/* already register to dpm */
		if (br_info->inst < 0) {
			br_info->inst = dp_dev->inst;
			br_info->dp_port = dp_dev->ep;
		}
	}

	/* update br_member_port information */
	dp_dev->br_member_port->dp_port = dp_dev->ep;
	dp_dev->br_member_port->bportid = dp_dev->bp;
#endif
	return DP_SUCCESS;
}

int _dp_free_fid(struct dp_dev *dp_dev, struct br_info *br_info)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	struct inst_info *i_info;

	if (br_info->inst < 0)
		return DP_SUCCESS;
	if (br_info->fid <= 0)
		return DP_SUCCESS;
	dp_set_cpu_mac(br_info->dev, true);
	i_info = get_dp_prop_info(br_info->inst);
	if (i_info->swdev_free_brcfg)
		i_info->swdev_free_brcfg(0, br_info->fid);

	dp_dev->fid = -1;

	/* save last fid */
	br_info->last_fid = br_info->fid;
	br_info->fid = -1;
	br_info->inst = -1;
	br_info->dp_port = -1;

	DP_DEBUG(DP_DBG_FLAG_SWDEV, "Freed fid=%d for %s\n",
		 br_info->last_fid,
		 br_info->dev->name);
#endif
	return DP_SUCCESS;
}

struct dp_dev_subif *dev_sif_lookup(struct list_head *head, int subif)
{
	struct dp_dev_subif *pos;

	list_for_each_entry(pos, head, list) {
		if (pos->subif  == subif)
			return pos;
	}
	return NULL;
}

/* must call this API from register_subif/update_subif flow
 * note: later change ctp to subif in new design
*/
int dp_inc_dev(struct dp_subif_info *sif,
		    struct net_device *dev, int inst,
		    int ep, int bp, int ctp, u32 flag,
		    bool f_ctp_dev)
{
	struct dp_dev *dp_dev;
	struct inst_info *i_info;
	struct dp_dev_subif *dev_sif = NULL;
	int ret = DP_FAILURE;

	if (inst < 0) {
		pr_err("DPM: %s: wrong inst=%d: %s\n", __func__, inst, dev->name);
		return DP_FAILURE;
	}
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s inst=%d ep=%d bp=%d subif=0x%x %s sif=0x%px...\n",
		 dev->name, inst, ep, bp, ctp, f_ctp_dev ? "ctp_dev" : "",
		 sif);
	if (!dev) {
		pr_err("DPM: %s dev is NULL\n", __func__);
		return DP_FAILURE;
	}
	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev) {
		/* this workaround is required according to current ethernet
		 * driver design:
		 *  Ethernet driver call kernel API register_netdevice ->
		 *   1.dev->netdev_ops->ndo_init(dev); -> call dp_register_subif
		 *   2....
		 *   3.call_netdevice_notifiers(NETDEV_REGISTER, dev);
		 *   That means dp_regsiter_subif is called before the event of
		 *   NETDEV_REGISTER
		 */
		DP_DEBUG(DP_DBG_FLAG_SWDEV,
			 "%s: dp_dev %s not exist and need workaround\n",
			 __func__, dev->name);
		dp_add_dev(dev);
		dp_dev = dp_dev_lookup(dev);
		if (!dp_dev) {
			pr_err("DPM: %s: %s not exist\n", __func__, dev->name);
			goto EXIT;
		}
#ifndef DP_NOT_USE_NETDEV_REGISTER
		dp_dev->f_add_dev_wa = true;
#endif
	}
	/* sanity check */
	dev_sif = dev_sif_lookup(&dp_dev->subif_list, ctp);
	if (dev_sif) {
		pr_err("DPM: %s: why %s already exist in subif_list?\n",
		       __func__, dp_dev->dev->name);
		return DP_FAILURE;
	}
	dev_sif = dp_kzalloc(sizeof(*dev_sif), GFP_KERNEL);
	if (!dev_sif)
		return DP_FAILURE;
	/* update dp_dev information */
	dp_dev->inst = inst;
	dp_dev->ep = ep;
	dp_dev->bp = bp;
	dp_dev->f_ctp_dev = f_ctp_dev;
	dp_dev->swdev_en = sif->swdev_en;
	/* dp_dev->ctp = ctp; */ /* dont' use in new design */

	/* update dev_sif */
	dev_sif->sif = sif;
	dev_sif->subif = ctp;
	if (!f_ctp_dev)
		sif->dp_dev = dp_dev;

	if (is_vlan_dev(dev)) {
		dev_sif->logic_dev = logic_dev_lookup(&sif->logic_dev, dev);
		if (!dev_sif->logic_dev) {
			pr_err("DPM: %s: why not found logical dev %s\n", __func__,
			       dev->name);
			goto EXIT;
		}
		if (dp_dev->count >= 1) {
			pr_err("DPM: %s: can't support vlan on pmappder dev %s\n",
			       __func__, dev->name);
			goto EXIT;
		}
	}
	list_add(&dev_sif->list, &dp_dev->subif_list);
	dp_dev->count++; /* for pmapper case, it will add multiple times */

	/* need check with PON team whether need hack ndo_ for gem port dev */
	if (f_ctp_dev) {
		/* exit here to skip set_dev_ops */
		return DP_SUCCESS;
	}

	if (dp_dev->count == 1) {
		if (set_dev_ops(dp_dev, dev, flag) < 0) {
			pr_err("DPM: %s %d, Failed\n", __func__, __LINE__);
			goto EXIT;
		}
	}

	i_info = get_dp_prop_info(inst);
	if (i_info->vlan_aware_flag) {
		if (!dp_dev->br_info) {
			DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s not in bridge yet\n",
				 dev->name);
		} else {
		DP_DEBUG(DP_DBG_FLAG_SWDEV, "need tune br_vlan ops later\n");
#ifdef BR_VLAN
			if (i_info->vlan_aware_flag)
				dp_register_bport_vlan(br_info, dev,
						       dp_dev->bp, 0);
#endif
		}
	}

	/* workaround: if this dev already in bridge before register to dpm */
	if (netif_is_bridge_port(dev)) {
		struct net_device *br_dev;

		rcu_read_lock();
		br_dev = netdev_master_upper_dev_get_rcu(dev);
		rcu_read_unlock();
		if (br_dev)
			dp_br_addif(br_dev, dev);
	}

	if (dp_dev->br_info && (dp_dev->count == 1)) {
		DP_DUMP("Need workaround to dp_swdev_stp_forward: %s in br %s\n",
			dp_dev->dev->name, dp_dev->br_info->dev->name);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
		DP_SWDEV_LOCK(&dp_swdev_lock);
		dp_swdev_stp_forward(dp_dev);
		DP_SWDEV_UNLOCK(&dp_swdev_lock);
#endif
	}

	DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s inst=%d ep=%d bp=%d subif=%x %s done\n",
		 dev->name, inst, ep, bp, ctp, f_ctp_dev ? "ctp_dev" : "");
	return DP_SUCCESS;
EXIT:
	kfree(dev_sif);
	return ret;
}

/* must call this API from register_subif/update_subif flow.
 * Note: it will decrease the counter, but not free the memory.
 *       The memory will be freed in Net Notifier flow
 *       this subif_info content maybe already deleted and we cannot read it
 *       from sif any more
 *       note: later change ctp to subif in new design
 */
int dp_dec_dev(struct net_device *dev, int inst,
		    int ep, u16 ctp, u32 flag)
{
	struct dp_dev *dp_dev;
	struct dp_dev_subif *dev_sif;

	DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s inst=%d ep=%d subif=%x ...\n",
		 dev->name, inst, ep, ctp);
	if (!dev)
		return DP_FAILURE;

	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev) {
		pr_err("DPM: %s: Failed to dp_dev_lookup: %s\n", __func__,
		       dev->name);
		return -1;
	}
	if (dp_dev->count <= 0) {
		pr_err("DPM: %s: Count(%d) should > 0(%s)\n", __func__,
		       dp_dev->count, dev->name);
		return -1;
	}
	if (dp_dev->inst != inst) {
		pr_err("DPM: %s: inst not same:%d_%d(%s)\n", __func__, dp_dev->inst,
		       inst, dev->name);
		return -1;
	}
	if (dp_dev->ep != ep) {
		pr_err("DPM: %s: ep not same:%d_%d(%s)\n", __func__, dp_dev->ep, ep,
		       dev->name);
		return -1;
	}

	/* Check added to remove pmapper device from dp_dev list
	 * if pmapper device is unregistered with last gem
	 */
	dev_sif = dev_sif_lookup(&dp_dev->subif_list, ctp);
	if (!dev_sif) {
		pr_err("DPM: %s: not found %s via subif=%x\n",
		       __func__, dev->name, ctp);
		return -1;
	}
	/* last count and need reset something.
	 * Note, need reset before count--
	 */
	if (/*!dp_dev->f_ctp_dev && */dp_dev->count == 1) {
		/* need update GSWIP bp members */
		if (dp_dev->br_info) {
			if (dp_dev->f_ctp_dev)
				pr_warn("DPM: warning ctp_dev %s in br %s?\n", dev->name,
					dp_dev->br_info->dev->name);
			DP_DUMP("Need workaround to dp_swdev_stp_disable: %s in br %s\n",
				dp_dev->dev->name, dp_dev->br_info->dev->name);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
			DP_SWDEV_LOCK(&dp_swdev_lock);
			dp_swdev_stp_disable(dp_dev);
			DP_SWDEV_UNLOCK(&dp_swdev_lock);
#endif
		}
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV_xxx)
		if (netif_is_bridge_port(dev) &&
		    !(flag & DP_F_SUBIF_LOGICAL)) {
			struct net_device *br_dev;
			struct inst_info *dp_info;
			int inst;
			u8 *addr;

			inst = dp_dev->inst;
			addr = (u8 *)dev->dev_addr;
			dp_info = get_dp_prop_info(inst);
			if (dp_info->dp_mac_reset)
				dp_info->dp_mac_reset(0, dp_dev->fid,
						      inst, addr);
			rcu_read_lock();
			br_dev = netdev_master_upper_dev_get_rcu(dev);
			rcu_read_unlock();
			if (br_dev)
				dp_del_br_if(dev, br_dev, inst,
						dp_dev->bp);
		}
#endif
		if (reset_dev_ops(dp_dev, dev) < 0) {
			pr_err("DPM: %s %d, Failed\n", __func__, __LINE__);
			return DP_FAILURE;
		}
		DP_DEBUG(DP_DBG_FLAG_SWDEV, "reset_dev_ops for %s\n",
			 dev->name);
		dflt_dp_dev(dp_dev);
	}

	list_del(&dev_sif->list);
	kfree(dev_sif);
	dp_dev->count--;
	if (dp_dev->count == 0) {
		if (!list_empty(&dp_dev->subif_list)) {
			pr_err("DPM: %s:why subif_list not empty but count 0: %s\n",
			       __func__, dp_dev->dev->name);
		}
		if (dp_dev->f_del_dev_wa) {
			/* This workraound is needed if dpm_de-reguster_subif is called
			 * after dpm receive NETDEV_UNREGISTER event
			 */
			DP_DEBUG(DP_DBG_FLAG_SWDEV, "dp_del_dev workaround...%s\n",
				 dev->name);
			dp_del_dev(dev, DP_DEL_DEV_DEC_DEV);
		}
	}
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s inst=%d ep=%d subif=%x done\n",
		 dev->name, inst, ep, ctp);
	return 0;
}

static char *dp_get_del_dev_reason(u32 flag)
{
	if (flag & DP_DEL_DEV_NETDEV_UNREGISTER)
		return "NETDEV_UNREGISTER";
	if (flag & DP_DEL_DEV_NDO_UNINIT)
		return "ndo_uninit";
	if (flag & DP_DEL_DEV_DEC_DEV)
		return "dp_dec_dev";
	return "unknown";
}

/* Must call this API from Net notifier flow when this device is freed */
int dp_del_dev(struct net_device *dev, u32 flag)
{
	struct dp_dev *dp_dev;

	if (!dev)
		return DP_FAILURE;

	DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s ...\n", dev->name);
	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev) {
		return DP_SUCCESS;
	}
	if (dp_dev->count > 0) {
		if (flag & DP_DEL_DEV_NETDEV_UNREGISTER) {
			/* NETDEV_UNREGISTER event is trigged but this device not
			 * unregistered to dpm yet.
			 * In this case, it will wait for ndo_uninit workaround
			 */
			return DP_SUCCESS;
		} else if (flag & DP_DEL_DEV_NDO_UNINIT ) {
			pr_err("DPM: %s fail: %s ndo_uninit trigger but still regiser to dpm with %s=%d\n",
			       __func__, dev->name,
			       "count", dp_dev->count);
			/* not free from dp_dev list */
			return DP_SUCCESS;
		}
	}
	dp_set_netdev_ndo_uninit(dp_dev, 1);
	if (dp_dev->all_ops_cnt > 0) {
		pr_err("DPM: %s: %s %s=%d %s=%d\n",
		       __func__, dev->name,
		       "all_ops_cnt", dp_dev->all_ops_cnt,
		       dp_get_del_dev_reason(flag), flag);
		/* Found PPA does not reset ndo_get_stats64 at all even this
		 * dev is unregistered from network stack already
		 * So here DPM proceed to free this dp_dev. Of course, caller
		 * need to fix it later
		 */
	}

#ifdef need_check_later
	/*TODO: Need further check later whether need workaroud or not */
	if (dp_dev->br_info) {
		/* dp_register_bport_vlan(dp_dev->br_dev, dev, dp_dev->bp,
				       DP_OPS_RESET);
		 */

	}
#endif

	DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s done addr=0x%px\n", dev->name, dp_dev);
	hlist_del(&dp_dev->hlist);
	kmem_cache_free(cache_dev_list, dp_dev);

	return DP_SUCCESS;
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP

