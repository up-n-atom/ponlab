// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2022, MaxLinear, Inc.
 * Copyright 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <net/switch_api/gsw_flow_ops.h>
#include <net/datapath_api.h>
#include <net/datapath_api_vlan.h>
#include "datapath.h"
#include "datapath_instance.h"
#include "datapath_swdev.h"
#include "datapath_pce.h"
#include "datapath_ioctl.h"
#include <linux/if_bridge.h>
#include "datapath_br_vlan.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

#if (IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV) && \
	IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31))

#define MAX_FID		64 /* Only 6 bits for FiD */

enum BR_VLAN_FLAG {
	BR_VLAN_LIST_TOP_ENTRY = BIT(0),
	BR_VLAN_DEREGISTER = BIT(1)
};

/* offset in bytes
 * ops_cb is only for set, not for rest
 */
int dp_set_br_netdev_ops(struct br_info *br_info, void *ops_cb,
					   int offset, u32 flag)
{
	struct net_device *dev;
	int err = DP_FAILURE;

	if (!br_info) {
		pr_err("DPM: %s invalid dev\n", __func__);
		return err;
	}
	dev = br_info->dev;
	dp_ops_set((void **)&dev->netdev_ops, offset,
		   sizeof(br_info->new_devops),
		   (void **)&br_info->old_dev_ops,
		   &br_info->new_devops,
		   ops_cb,
		   &br_info->netdev_ops_num, &br_info->all_ops_cnt, flag);
	return err;
}

static int dp_del_vlan_entry(struct br_info *br_info,
			     struct vlist_entry *vlist_entry, dp_subif_t *subif,
			     u16 vlanid, int flags);

/* Check Bridge have the entry added through Self
 * if vlanid=0, returns first entry
 * if valid vlanid, returns entry pointed by VLAN ID
 */
struct vlist_entry *get_vlist_entry_in_br(struct br_info *br_info,
					  u16 vlanid, int flags)
{
	struct vlist_entry *list_entry = NULL;

	/* Returning the top entry in list or Return the exact matching Entry */
	if (flags & BR_VLAN_LIST_TOP_ENTRY) {
		list_for_each_entry(list_entry, &br_info->br_vlan_list, list) {
			if (!list_entry->vlan_entry)
				continue;
			return list_entry;
		}
	} else {
		list_for_each_entry(list_entry, &br_info->br_vlan_list, list) {
			if (!list_entry->vlan_entry ||
			    (vlanid != list_entry->vlan_entry->vlan_id))
				continue;
			return list_entry;
		}
	}
	return NULL;
}

/* Check through all Bridge Member port have the entry added through Master
 * if have return the entry
 */
struct vlist_entry *get_vlist_entry_in_all_bp(struct br_info *br_info,
					      u16 vlanid)
{
	struct vlist_entry *list_entry = NULL;
	struct bridge_member_port *bmp;

	/* Check Bridge Member port have the entry added through Master */
	list_for_each_entry(bmp, &br_info->bp_list, list) {
		list_for_each_entry(list_entry, &bmp->bport_vlan_list, list) {
			if (!list_entry->vlan_entry ||
			    (vlanid != list_entry->vlan_entry->vlan_id))
				continue;
			return list_entry;
		}
	}
	return NULL;
}

/* Get the List Head for the BP Vlan List */
struct list_head *get_vlan_entry_head_in_bp(struct br_info *br_info,
					    int bport)
{
	struct bridge_member_port *bmp;

	list_for_each_entry(bmp, &br_info->bp_list, list) {
		if (bmp->bportid != bport)
			continue;
		return &bmp->bport_vlan_list;
	}
	return NULL;
}

/* Check through Bridge Member port specified have the entry added
 * through Master if have return the entry
 */
struct vlist_entry *get_vlist_entry_in_bp(struct br_info *br_info,
					  int bport, u16 vlanid, int flags)
{
	struct vlist_entry *list_entry = NULL;
	struct bridge_member_port *bmp;

	/* Returning the top entry in list or Return the exact matching Entry */
	if (flags & BR_VLAN_LIST_TOP_ENTRY) {
		list_for_each_entry(bmp, &br_info->bp_list, list) {
			if (bmp->bportid != bport)
				continue;
			list_for_each_entry(list_entry, &bmp->bport_vlan_list,
					    list) {
				if (!list_entry->vlan_entry)
					continue;
				return list_entry;
			}
		}
	} else {
		list_for_each_entry(bmp, &br_info->bp_list, list) {
			if (bmp->bportid != bport)
				continue;
			list_for_each_entry(list_entry, &bmp->bport_vlan_list,
					    list) {
				if (!list_entry->vlan_entry ||
				    (vlanid != list_entry->vlan_entry->vlan_id))
					continue;
				return list_entry;
			}
		}
	}
	return NULL;
}

/* Get pce entry from vlan entry */
struct pce_entry *get_pce_entry_from_ventry(struct vlan_entry *ventry,
					    int portid, int subifid)
{
	struct pce_entry *pentry;
	struct dp_subblk_info *info;

	list_for_each_entry(pentry, &ventry->pce_list, list) {
		if (!pentry->blk_info)
			continue;
		if (pentry->blk_info->region == GSW_PCE_RULE_CTP) {
			info = &pentry->blk_info->info;
			if ((info->portid == portid) &&
			    (info->subif == subifid))
				return pentry;
		}
	}
	return NULL;
}

/* Get pce entry from Bp */
static
struct pce_entry *get_pce_entry_from_bp(struct br_info *br_info,
					dp_subif_t *subif, int vlanid)
{
	struct bridge_member_port *bmp;
	struct vlist_entry *v;
	struct pce_entry *pce;
	struct pmac_port_info *port_info;
	int vap;

	port_info = get_dp_port_info(subif->inst, subif->port_id);
	vap = GET_VAP(subif->subif, port_info->vap_offset,
			      port_info->vap_mask);

	list_for_each_entry(bmp, &br_info->bp_list, list) {
		if (bmp->bportid != subif->bport)
			continue;
		list_for_each_entry(v, &bmp->bport_vlan_list, list) {
			if (!v->vlan_entry ||
			    vlanid != v->vlan_entry->vlan_id)
				continue;
			pce = get_pce_entry_from_ventry(v->vlan_entry,
							subif->port_id,
							vap);
			if (pce)
				return pce;
		}
	}
	return NULL;
}

/* Get pce entry from all vlan entries */
static
struct pce_entry *get_pce_entry_all_ventry(struct br_info *br_info,
					   int portid, int subifid)
{
	struct pce_entry *pentry = NULL;
	struct vlist_entry *v = NULL;
	struct bridge_member_port *bmp;

	list_for_each_entry(bmp, &br_info->bp_list, list) {
		list_for_each_entry(v, &bmp->bport_vlan_list, list) {
			if (!v->vlan_entry)
				continue;
			pentry = get_pce_entry_from_ventry(v->vlan_entry,
							   portid, subifid);
			if (pentry)
				return pentry;
		}
	}
	return NULL;
}

int dp_register_br_vlan_ops(struct br_info *br_info, int flags)
{
	int offset;
	int flag = DP_OPS_NETDEV | flags;

	if (!br_info)
		return DP_FAILURE;

	/* modify ndo_ or restore original ndo based on flags */
	offset = offsetof(const struct net_device_ops, ndo_bridge_setlink);
	dp_set_br_netdev_ops(br_info, &dp_ndo_bridge_setlink, offset, flag);

	offset = offsetof(const struct net_device_ops, ndo_bridge_dellink);
	dp_set_br_netdev_ops(br_info, &dp_ndo_bridge_dellink, offset, flag);

	return DP_SUCCESS;
}

/* Copy the Bridge Settings of bridge to the new Vlan Aware FiD's */
static int dp_gsw_bridge_cfg_copy(int inst, u16 src_fid, u16 dst_fid)
{
	GSW_return_t ret = DP_SUCCESS;
	GSW_BRIDGE_config_t *brcfg;
	struct core_ops *gsw_handle;

	DP_DEBUG(DP_DBG_FLAG_BR_VLAN, "%s: src_fid=%d dst_fid=%d\n",
		 __func__, src_fid, dst_fid);

	gsw_handle = dp_port_prop[inst].ops[0];
	if (!gsw_handle)
		return -1;

	brcfg = dp_kzalloc(sizeof(*brcfg), GFP_ATOMIC);
	if (!brcfg)
		return DP_FAILURE;

	brcfg->nBridgeId = src_fid;
	ret = gsw_handle->gsw_brdg_ops.Bridge_ConfigGet(gsw_handle, brcfg);
	if (ret) {
		kfree(brcfg);
		return ret;
	}

	brcfg->nBridgeId = dst_fid;
	ret = gsw_handle->gsw_brdg_ops.Bridge_ConfigSet(gsw_handle, brcfg);
	if (ret) {
		kfree(brcfg);
		return ret;
	}

	DP_DEBUG(DP_DBG_FLAG_BR_VLAN, "FID(%d to %d) copy success\n",
		 src_fid, dst_fid);
	kfree(brcfg);
	return ret;
}

/* Free the Bridge FiD created in GSWIP */
static int deallocate_br_vlan_fid(struct br_info *br_info,
				  u16 vlanid, u16 fid)
{
	struct inst_info *dp_info = get_dp_prop_info(0);
	int ret = 0;

	ret = dp_info->swdev_free_brcfg(0, fid);
	if (ret) {
		pr_err("DPM: %s: swdev_free_brcfg Failed for FiD %d\n",
		       __func__, fid);
		return DP_FAILURE;
	}

	br_info->num_fid--;
	return DP_SUCCESS;
}

/* Allocate new VLAN Aware Bridge FiD in GSWIP */
static int allocate_br_vlan_fid(struct br_info *br_info, u16 vlanid)
{
	u16 fid = 0;
	struct inst_info *dp_info = get_dp_prop_info(0);
	int new_brid;

	DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
		 "%s: Br=%s Br_Fid=%d vlanid=%d num_vlan=%d num_fid=%d\n",
		 __func__, br_info->dev->name, br_info->fid, vlanid,
		 br_info->num_vlan, br_info->num_fid);

	if (br_info->num_vlan > br_info->max_vlan_limit) {
		pr_err("DPM: %s: Cannot Support more than %d VLAN ID's \n",
		       __func__, br_info->max_vlan_limit);
		return DP_FAILURE;
	}

	if (br_info->num_fid > br_info->max_brid_limit) {
		pr_err("DPM: %s: Cannot Support more than %d Bridge ID's \n",
		       __func__, br_info->max_brid_limit);
		return DP_FAILURE;
	}

	new_brid = dp_info->swdev_alloc_bridge_id(br_info->inst);
	if (new_brid <= 0) {
		pr_err("DPM: %s: Switch bridge alloc failed\n", __func__);
		return DP_FAILURE;
	}

	fid = new_brid;

	if (new_brid > MAX_FID) {
		dp_info->swdev_free_brcfg(br_info->inst, fid);
		pr_err("DPM: %s: VLAN Aware support max 64 FiD (VlanID + BriID)\n",
		       __func__);
		return DP_FAILURE;
	}

	if (dp_gsw_bridge_cfg_copy(br_info->inst, br_info->fid, fid)) {
		dp_info->swdev_free_brcfg(br_info->inst, fid);
		pr_err("DPM: %s: Bridge Cfg Copy for Fid %d to Fid %d failed\n",
		       __func__, br_info->fid, fid);
		return DP_FAILURE;
	}

	br_info->num_fid++;
	return fid;
}

static bool is_pmapper_vlan_aware(int inst, int bp)
{
	struct bp_pmapper *bp_info;
	struct net_device *br_dev;
	struct br_info *br_info;
	struct vlist_entry *list_entry = NULL;
	struct bridge_member_port *bmp;

	bp_info = get_dp_bp_info(inst, bp);
	rcu_read_lock();
	br_dev = netdev_master_upper_dev_get_rcu(bp_info->dev);
	rcu_read_unlock();
	if (!br_dev) {
		DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
			 "%s: pmapper dev(%s) not in bridge\n", __func__,
			 bp_info->dev->name ? bp_info->dev->name : "NULL");
		return false;
	}

	br_info = dp_swdev_bridge_entry_lookup(br_dev);
	if (!br_info) {
		DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
			 "%s: br_info not added to DPM\n", __func__);
		return false;
	}

	if (!br_info->br_vlan_en) {
		DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
			 "%s: br=%s is not VLAN Aware\n",
			 __func__, br_dev->name ? br_dev->name : "NULL");
		return false;
	}

	/* Check if Bridge Member port have any VLAN entry added */
	list_for_each_entry(bmp, &br_info->bp_list, list) {
		if (bmp->bportid != bp)
			continue;
		list_for_each_entry(list_entry, &bmp->bport_vlan_list, list) {
			if (!list_entry->vlan_entry)
				continue;
			if (list_entry->vlan_entry->vlan_id)
				return true;
		}
	}
	return false;
}

/* Copies FirstFlowEntryIndex to all its CTP ports in GSWIP.
 * It is mainly for pmapper case
 */
int dp_gsw_copy_ctp_cfg(int inst, int bp, struct dp_subblk_info *subblk_info,
			int portid)
{
	struct ctp_dev *ctp_entry;
	struct bp_pmapper *bp_info;
	GSW_CTP_portConfig_t *ctpcfg;
	struct core_ops *gsw_ops;
	int flow_index;

	gsw_ops = dp_port_prop[inst].ops[0];

	DP_DEBUG(DP_DBG_FLAG_BR_VLAN, "bp=%d, is pmapper\n", bp);

	/* check if pmapper is VLAN aware before PCE block copy to other CTP */
	if (!is_pmapper_vlan_aware(inst, bp)) {
		DP_DEBUG(DP_DBG_FLAG_BR_VLAN, "pmapper is not vlan aware\n");
		return DP_SUCCESS;
	}

	bp_info = get_dp_bp_info(inst, bp);
	if ((!bp_info) || (!bp_info->ref_cnt))
		return DP_FAILURE;

	ctpcfg = dp_kzalloc(sizeof(*ctpcfg), GFP_ATOMIC);
	if (!ctpcfg)
		return DP_FAILURE;
	/* Retrieve the CTP dev list for the pmapper bridge port */
	list_for_each_entry(ctp_entry, &bp_info->ctp_dev, list) {
		if (!ctp_entry->dev)
			continue;
		ctpcfg->nLogicalPortId = portid;
		ctpcfg->nSubIfIdGroup = ctp_entry->ctp;
		if (gsw_ops->gsw_ctp_ops.CTP_PortConfigGet(gsw_ops, ctpcfg)) {
			pr_err("DPM: CTP(%d) CfgGet fail for bp=%d for ep=%d\n",
			       ctp_entry->ctp, bp, portid);
			kfree(ctpcfg);
			return DP_FAILURE;
		}
		/* if FlowEntryIndex exists, dont configure */
		if (ctpcfg->nFirstFlowEntryIndex) {
			/* store flow index into BP pmapper table  to use during
			 * register/update subif flow when subblk info is not
			 * valid
			 */
			bp_info->ctp_flow_index = ctpcfg->nFirstFlowEntryIndex;
			continue;
		}
		/* subblk_info is NULL, if CTP config copy is called from
		 * DP register/update subif info
		 */
		if (subblk_info)
			flow_index = subblk_info->subblk_firstidx;
		else
			flow_index = bp_info->ctp_flow_index;
		DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
			 "set flow entry(%d) into ctp=%d\n",
			 flow_index, ctp_entry->ctp);
		ctpcfg->nFirstFlowEntryIndex = flow_index;
		ctpcfg->eMask = GSW_CTP_PORT_CONFIG_FLOW_ENTRY;
		if (gsw_ops->gsw_ctp_ops.CTP_PortConfigSet(gsw_ops, ctpcfg)) {
			pr_err("DPM: CTP(%d) CfgSet fail for bp=%d for ep=%d\n",
			       ctp_entry->ctp, bp, portid);
			kfree(ctpcfg);
			return DP_FAILURE;
		}
	}
	kfree(ctpcfg);
	return DP_SUCCESS;
}

/* Update current PCE rule index with enable/disable */
static int dp_update_pce_rule(struct pce_entry *pentry,
			      struct vlan_entry *ventry,
			      enum DP_PCE_RULE_UPDATE update)
{
	GSW_PCE_rule_t *pce;
	int ret = DP_FAILURE;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_BR_VLAN) {
		if (update == DP_PCE_RULE_EN)
			DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
				 "%s pce idx (%d)\n",
				 "Enable", pentry->idx);
		else if (update == DP_PCE_RULE_DIS)
			DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
				 "%s pce idx (%d)\n",
				 "Disable", pentry->idx);
	}
#endif

	pce = dp_kzalloc(sizeof(*pce), GFP_ATOMIC);
	if (!pce)
		return -1;

	pce->logicalportid = pentry->blk_info->info.portid;
	pce->subifidgroup = pentry->blk_info->info.subif;
	pce->region = pentry->blk_info->region;
	pce->pattern.nIndex = pentry->idx;

	if (dp_pce_rule_get_priv(pentry->blk_info, pce)) {
		pr_err("DPM: %s: Get PCE rule %d failed\n",
		       __func__, pce->pattern.nIndex);
		goto EXIT;
	}

	/* PCE struct is reset except for pattern and action after get */
	pce->logicalportid = pentry->blk_info->info.portid;
	pce->subifidgroup = pentry->blk_info->info.subif;
	pce->region = pentry->blk_info->region;
	pce->pattern.nIndex = pentry->idx;

	/* Allow update for VLAN ID and FiD in PCE rule */
	if (update == DP_PCE_RULE_REWRITE) {
		pce->pattern.nSLAN_Vid = ventry->vlan_id;
		pce->action.nFId = ventry->fid;
	}

	if (dp_pce_rule_update_priv(pentry->blk_info, pce, update)) {
		pr_err("DPM: %s: Updating PCE rule %d failed\n",
		       __func__, pce->pattern.nIndex);
		goto EXIT;
	}

	if (update == DP_PCE_RULE_EN || update == DP_PCE_RULE_REWRITE)
		pentry->disable = false;
	else if (update == DP_PCE_RULE_DIS)
		pentry->disable = true;

	ret = DP_SUCCESS;
EXIT:
	kfree(pce);
	return ret;
}

/* Delete rule */
static int dp_del_rule(struct vlan_entry *v, struct pce_entry *p)
{
	GSW_PCE_ruleDelete_t pce_del;

	DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
		 "%s pce idx (%d) for vid:%d\n",
		 "Delete", p->idx, v->vlan_id);

	pce_del.logicalportid = p->blk_info->info.portid;
	pce_del.subifidgroup = p->blk_info->info.subif;
	pce_del.region = p->blk_info->region;
	pce_del.nIndex = p->idx;

	if (dp_pce_rule_del_priv(p->blk_info, &pce_del)) {
		pr_err("DPM: %s for pceidx %d Failed\n",
		       "dp_pce_rule_del_priv", p->idx);
		return DP_FAILURE;
	}
	list_del(&p->list);
	kfree(p->blk_info);
	kfree(p);

	return 0;
}

/* Alloc PCE BLK for CTP region and create a new pce rule */
static int dp_add_rule(struct br_info *br_info, struct vlan_entry *ventry,
		       dp_subif_t *subif)
{
	GSW_PCE_rule_t *pce_rule;
	struct dp_subblk_info *blk_info;
	int idx = 0;
	struct pce_entry *pce = NULL, *pce_entry = NULL;
	struct pmac_port_info *port_info;
	int vap;

	DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
		 "Creating PCE rule Bport %d vlanid %d\n",
		 subif->bport, ventry->vlan_id);

	pce = dp_kzalloc(sizeof(*pce), GFP_ATOMIC);
	if (!pce)
		return DP_FAILURE;

	pce->blk_info = dp_kzalloc(sizeof(*pce->blk_info), GFP_ATOMIC);
	if (!pce->blk_info) {
		kfree(pce);
		return DP_FAILURE;
	}

	pce_rule = dp_kzalloc(sizeof(*pce_rule), GFP_ATOMIC);
	if (!pce_rule)
		goto ERR_EXIT;

	port_info = get_dp_port_info(subif->inst, subif->port_id);
	vap = GET_VAP(subif->subif, port_info->vap_offset, port_info->vap_mask);

	/* if already have in any vlan enty use the existing pce blk info */
	pce_entry = get_pce_entry_all_ventry(br_info, subif->port_id, vap);
	if (pce_entry)
		dp_memcpy(pce->blk_info, pce_entry->blk_info,
			  sizeof(struct dp_pce_blk_info));

	blk_info = &pce->blk_info->info;
	blk_info->subif = vap;
	blk_info->subblk_size = VLAN_PCE_MAX_SUBBLK_SIZE;
	blk_info->portid = subif->port_id;
	/* Setting of blk_info->bp field is valid only for Pmapper device since
	 * pceidx needs to be copied to all CTP/gem under them
	 * This information is later used during reset of pceidx from CTP Port
	 */
	if (subif->flag_pmapper)
		blk_info->bp = subif->bport;

	strlcpy(blk_info->subblk_name, "dp_br_vlan",
		sizeof(blk_info->subblk_name));

	pce->blk_info->region = GSW_PCE_RULE_CTP;

	pce_rule->logicalportid = subif->port_id;
	pce_rule->subifidgroup = vap;

	/* pce_rule pattern setting */
	pce_rule->region = GSW_PCE_RULE_CTP;
	pce_rule->pattern.bEnable = 1;
	pce_rule->pattern.bSubIfIdEnable = 1;
	pce_rule->pattern.eSubIfIdType = GSW_PCE_SUBIFID_TYPE_BRIDGEPORT;
	pce_rule->pattern.nSubIfId = subif->bport;
	pce_rule->pattern.bSLAN_Vid = 1;
	pce_rule->pattern.nSLAN_Vid = ventry->vlan_id;

	/* action setting */
	pce_rule->action.bFidEnable = 1;
	pce_rule->action.nFId = ventry->fid;

	idx = dp_pce_rule_add_priv(pce->blk_info, pce_rule);
	kfree(pce_rule);

	if (idx < 0) {
		pr_err("DPM: %s: dp_pce_rule_add_priv for Vlan %d port %d failed\n",
		       __func__, ventry->vlan_id, subif->port_id);
		goto ERR_EXIT;
	}

	pce->idx = idx;
	list_add(&pce->list, &ventry->pce_list);

	/* If FiD is not yet created mark this PCE rule as disabled */
	if (!ventry->fid) {
		if (dp_update_pce_rule(pce, ventry, DP_PCE_RULE_DIS)) {
			dp_del_rule(ventry, pce);
			return DP_FAILURE;
		}
	}

	DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
		 "For vlan_id %d added PCE Rule %d and Status (%s)\n",
		 ventry->vlan_id, pce->idx,
		 pce->disable ? "Disable" : "Enable");

	return idx;

ERR_EXIT:
	kfree(pce->blk_info);
	kfree(pce);

	return -1;
}

/* Add a new pce rule, If new vlan FID is created
 * For Master go through the exact bp and add rule or enable rule
 * For Self go through all the pce entries and Enable all entries
 * in the vlan entry
 * subif is NULL for Self
 */
static int dp_add_vlan_pce_rule(struct vlan_entry *ventry,
				struct br_info *br_info, dp_subif_t *subif,
				int flags)
{
	struct pce_entry *p;

	if (flags & BRIDGE_FLAGS_MASTER) {
		/* Add new pce rule for this bridge port/Enable it */
		p = get_pce_entry_from_bp(br_info, subif, ventry->vlan_id);
		if (!p) {
			if (dp_add_rule(br_info, ventry, subif) < 0)
				return DP_FAILURE;
		} else {
			if (p->disable) {
				if (dp_update_pce_rule(p, ventry,
						       DP_PCE_RULE_EN))
					return DP_FAILURE;
			}
		}
	} else if (flags & BRIDGE_FLAGS_SELF) {
		list_for_each_entry(p, &ventry->pce_list, list) {
			if (!p->blk_info)
				continue;
			if (p->disable) {
				/* There is a possibility that FiD is generated
				 * later so need to rewrite the rule to update
				 * fid
				 */
				if (dp_update_pce_rule(p, ventry,
						       DP_PCE_RULE_REWRITE))
					return DP_FAILURE;
			}
		}
	}
	return DP_SUCCESS;
}

/* Add a VLAN Entry, If already have in BP or Bridge, reuse the VLAN entry
 * In 1 Bridge Each VLAN have only 1 uniqueue FiD
 * NewFID = VlanID + BridgeID
 */
static struct vlan_entry *dp_add_vlan_entry(struct br_info *br_info,
		struct vlist_entry *br_ventry,
		struct vlist_entry *bp_ventry,
		dp_subif_t *subif,
		u16 vlanid, int flags)
{
	struct vlist_entry *vlist_entry, *curr_bp_ventry = NULL;
	struct vlan_entry *ventry;
	struct list_head *vhead;
	int fid = 0;
	bool alloc_fid = false;

	DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
		 "dev=%s vlanid=%d flags=%08x\n",
		 br_info->dev->name, vlanid, flags);

	if (flags & BRIDGE_FLAGS_MASTER)
		curr_bp_ventry = get_vlist_entry_in_bp(br_info, subif->bport,
						       vlanid, 0);
	if (br_ventry && bp_ventry && curr_bp_ventry) {
		DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
			 "No Need to add both br & bport have the entry\n");
		return DP_SUCCESS;
	}

	vlist_entry = dp_kzalloc(sizeof(struct vlist_entry), GFP_ATOMIC);
	if (!vlist_entry)
		return NULL;

	/* if bridge or bridge member port have the VLAN Entry
	 * Add to the corresponding list increase ref_cnt and return Entry
	 */
	if (flags & BRIDGE_FLAGS_SELF)
		vhead = &br_info->br_vlan_list;
	else
		vhead = get_vlan_entry_head_in_bp(br_info, subif->bport);

	if (!vhead) {
		kfree(vlist_entry);
		return NULL;
	}

	/* Bridge Port or Bridge dont have the VLAN Entry allocate a new one */
	if (!br_ventry && !bp_ventry) {
		DP_DEBUG(DP_DBG_FLAG_BR_VLAN, "Allocating a new entry\n");
		ventry = dp_kzalloc(sizeof(struct vlan_entry), GFP_ATOMIC);
		if (!ventry) {
			kfree(vlist_entry);
			return NULL;
		}

		vlist_entry->vlan_entry = ventry;
		vlist_entry->vlan_entry->vlan_id = vlanid;
		INIT_LIST_HEAD(&vlist_entry->vlan_entry->pce_list);
		/* BR already have, now going to create BP */
	} else if (br_ventry && !bp_ventry) {
		DP_DEBUG(DP_DBG_FLAG_BR_VLAN, "Allocating a BP entry\n");
		vlist_entry->vlan_entry = br_ventry->vlan_entry;

		if (!vlist_entry->vlan_entry->fid)
			alloc_fid = true;

		/* Some BP already have, now we are going to create BR */
	} else if (!br_ventry && bp_ventry) {
		DP_DEBUG(DP_DBG_FLAG_BR_VLAN, "Allocating a BR entry\n");
		vlist_entry->vlan_entry = bp_ventry->vlan_entry;

		if (flags & BRIDGE_FLAGS_SELF && !vlist_entry->vlan_entry->fid)
			alloc_fid = true;

		/* BR already have, Some other BP in br have,
		 * but current BP don't have
		 */
	} else if (br_ventry && bp_ventry && !curr_bp_ventry) {
		DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
			 "Allocating a bp entry for current bp\n");
		vlist_entry->vlan_entry = br_ventry->vlan_entry;

		if (!vlist_entry->vlan_entry->fid)
			alloc_fid = true;
	} else {
		pr_err("DPM: %s: Impossible alloc both BP and BR have VLAN Entry\n",
		       __func__);
		kfree(vlist_entry);
		return NULL;
	}

	/* FiD is created only when 1 Self + 1 Master */
	if (alloc_fid) {
		/* Allocate a unique FID for this VLAN ID */
		fid = allocate_br_vlan_fid(br_info, vlanid);
		if (fid < 0) {
			kfree(vlist_entry);
			return NULL;
		}
		vlist_entry->vlan_entry->fid = fid;
	}
	vlist_entry->vlan_entry->ref_cnt++;
	list_add(&vlist_entry->list, vhead);

	/* Add new pce rule for this vid if not created this will be added
	 * only for a bridge member port
	 */
	if (dp_add_vlan_pce_rule(vlist_entry->vlan_entry, br_info, subif,
				 flags)) {
		vlist_entry->vlan_entry->ref_cnt--;
		list_del(&vlist_entry->list);
		kfree(vlist_entry);
		pr_err("DPM: %s: dp_add_vlan_pce_rule: Failed\n", __func__);
		return NULL;
	}

	return vlist_entry->vlan_entry;
}

/* Delete a pce rule
 * For Master go through the exact bp and delete rule or disable rule
 * For Self go through all the pce entries and disable or delete rule
 * subif is NULL for Self
 */
static int dp_del_vlan_pce_rule(struct vlan_entry *ventry,
				struct br_info *br_info, dp_subif_t *subif,
				int flags)
{
	struct pce_entry *p;
	struct list_head *t, *q;

	if (flags & BRIDGE_FLAGS_MASTER) {
		p = get_pce_entry_from_bp(br_info, subif, ventry->vlan_id);
		if (!p)
			return DP_SUCCESS;

		/* Disable the PCE rule if it is enabled else delete */
		if (!p->disable) {
			if (dp_update_pce_rule(p, ventry, DP_PCE_RULE_DIS))
				return DP_FAILURE;
		} else {
			if (dp_del_rule(ventry, p))
				return DP_FAILURE;
		}
	} else if (flags & BRIDGE_FLAGS_SELF) {
		/* Delete all the pce rules which is not monitored by anyone */
		list_for_each_safe(q, t, &ventry->pce_list) {
			p = list_entry(q, struct pce_entry, list);
			if (!p->blk_info)
				continue;
			if (p->disable) {
				if (dp_del_rule(ventry, p))
					return DP_FAILURE;
			}
		}
		/* Disable all the PCE rules which are active */
		list_for_each_entry(p, &ventry->pce_list, list) {
			if (!p->blk_info)
				continue;
			if (!p->disable) {
				if (dp_update_pce_rule(p, ventry,
						       DP_PCE_RULE_DIS))
					return DP_FAILURE;
			}
		}
	}
	return DP_SUCCESS;
}

/* Deleting a bridge member VLAN Entry and FID
 * Bport=bp only in case of Master
 * Bport=0 only in case of Self
 */
static int dp_del_vlan_entry(struct br_info *br_info,
			     struct vlist_entry *vlist_entry, dp_subif_t *subif,
			     u16 vlanid, int flags)
{
	struct vlan_entry *ventry;

	/* if Entry or Ref Count is not present this VLAN ID doesn't exist */
	if (!vlist_entry || !vlist_entry->vlan_entry ||
	    !vlist_entry->vlan_entry->ref_cnt) {
		pr_err("DPM: %s: VLAN ID %d is not present in %s\n", __func__,
		       vlanid, br_info->dev->name);
		return DP_SUCCESS;
	}

	ventry = vlist_entry->vlan_entry;

	DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
		 "%s entry in Br=%s vlanid=%d original ref_cnt=%d\n",
		 (ventry->ref_cnt > 1) ? "ref--" : "Delete",
		 br_info->dev->name, ventry->vlan_id,
		 ventry->ref_cnt);

	/* Disable pce rule index for this vid */
	if (dp_del_vlan_pce_rule(ventry, br_info, subif, flags)) {
		pr_err("DPM: %s: %s: Br=%s Bp=%d vlanid=%d Failed\n",
		       __func__, "dp_del_vlan_pce_rule",
		       br_info->dev->name, subif ? subif->bport : 0,
		       ventry->vlan_id);
		return DP_FAILURE;
	}

	/* if usage > 1, Cannot delete now!! decrease ref count and delete the
	 * entry from list of either br or bridge member
	 */
	if (ventry->ref_cnt > 1) {
		ventry->ref_cnt--;
		list_del(&vlist_entry->list);
		kfree(vlist_entry);
		return DP_SUCCESS;
	}

	/* Deallocate VLAN aware FiD, if FiD is > 0 */
	if (ventry->fid) {
		if (deallocate_br_vlan_fid(br_info, vlanid, ventry->fid)) {
			pr_err("DPM: %s: %s: Br=%s Bp=%d vlanid=%d Failed\n",
			       __func__, "deallocate_br_vlan_fid",
			       br_info->dev->name, subif ? subif->bport : 0,
			       ventry->vlan_id);
			return DP_FAILURE;
		}
	}

	/* Decrease ref count and delete the VLAN ID Entry */
	ventry->ref_cnt--;
	list_del(&vlist_entry->list);
	kfree(ventry);
	kfree(vlist_entry);

	br_info->num_vlan--;

	DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
		 "Vlan ID %d removed on bridge %s\n",
		 vlanid, br_info->dev->name);
	return DP_SUCCESS;
}

/* for bridge vlan add with self flag, for example,
 * bridge vlan add dev br0 vid 100 self
 */
static int dp_br_vlan_self(struct br_info *br_info, u16 vlanid,
				   int flags)
{
	int ret = DP_FAILURE;
	struct vlan_entry *ventry;
	struct vlist_entry *vlist_entry, *br_ventry, *bp_ventry;

	/* if not bridge return error */
	if (!netif_is_bridge_master(br_info->dev))
		return ret;
	DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
		 "bridge=%s vlanid=%d flags=%08x\n",
		 br_info->dev->name, vlanid, flags);

	if (flags & BR_VLAN_DEREGISTER) {
		vlist_entry = get_vlist_entry_in_br(br_info, vlanid, 0);
		ret = dp_del_vlan_entry(br_info, vlist_entry, 0, vlanid,
					BRIDGE_FLAGS_SELF);
		goto EXIT;
	}

	/* Dont allow multiple commands to add multiple entries in list */
	br_ventry = get_vlist_entry_in_br(br_info, vlanid, 0);
	if (br_ventry) {
		DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
			 "Vlan ID %d is already present in Br %s\n",
			 vlanid, br_info->dev->name);
		goto EXIT;
	}

	bp_ventry = get_vlist_entry_in_all_bp(br_info, vlanid);

	/* if not present will allocate a new one */
	ventry = dp_add_vlan_entry(br_info, br_ventry, bp_ventry, 0, vlanid,
				   BRIDGE_FLAGS_SELF);
	if (!ventry) {
		pr_err("DPM: %s: Failed to Add vland id %d to br %s\n",
		       __func__, vlanid, br_info->dev->name);
		goto EXIT;
	}
	DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
		 "VLAN Id %d added for bridge %s Fid %d ref_cnt %d\n",
		 ventry->vlan_id, br_info->dev->name, ventry->fid,
		 ventry->ref_cnt);

	br_info->num_vlan++;
	ret = DP_SUCCESS;
EXIT:
	return ret;
}

/* for bridge vlan add with master flag, for example,
 * bridge vlan add dev eth1 vid 100 master
 */
static int dp_br_vlan_master(struct br_info *br_info,
				     struct net_device *dev,
				     u16 vlanid, int flags)
{
	dp_subif_t *subif;
	struct vlan_entry *ventry;
	int ret = DP_FAILURE;
	struct vlist_entry *vlist_entry, *br_ventry, *bp_ventry;

	subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif)
		return DP_FAILURE;
	if (dp_get_netif_subifid(dev, NULL, NULL, NULL, subif, 0)) {
		kfree(subif);
		pr_err("DPM: %s: dp_get_netif_subifid failed\n", __func__);
		return DP_FAILURE;
	}

	DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
		 "Br=%s bport=%d vlanid=%d flags=%08x\n",
		 br_info->dev->name, subif->bport, vlanid, flags);

	if (flags & BR_VLAN_DEREGISTER) {
		vlist_entry = get_vlist_entry_in_bp(br_info, subif->bport,
						    vlanid, 0);
		ret = dp_del_vlan_entry(br_info, vlist_entry, subif,
					vlanid, BRIDGE_FLAGS_MASTER);
		goto EXIT;
	}

	/* Dont allow multiple commands to add multiple entries in list */
	bp_ventry = get_vlist_entry_in_bp(br_info, subif->bport, vlanid, 0);
	if (bp_ventry) {
		DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
			 "Vlan ID %d is already present in Bp %d of Br %s\n",
			 vlanid, subif->bport, br_info->dev->name);
		goto EXIT;
	}

	bp_ventry = get_vlist_entry_in_all_bp(br_info, vlanid);
	br_ventry = get_vlist_entry_in_br(br_info, vlanid, 0);

	/* For every member port bport will be unique */
	ventry = dp_add_vlan_entry(br_info, br_ventry, bp_ventry,
				   subif, vlanid, BRIDGE_FLAGS_MASTER);
	if (!ventry) {
		pr_err("DPM: %s: Failed to Add vland id %d to dev %s in br %s\n",
		       __func__, vlanid, dev->name, br_info->dev->name);
		goto EXIT;
	}

	DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
		 "Vlan ID %d Enabled Iface=%s br=%s NewFiD=%d RefCnt=%d\n",
		 ventry->vlan_id, dev->name, br_info->dev->name,
		 ventry->fid, ventry->ref_cnt);

	ret = DP_SUCCESS;
EXIT:
	kfree(subif);
	return ret;
}

/* Parse netlink message to get VLAN ID and Bridge Flags for Master and Self */
static int parse_netlink_msg(struct nlmsghdr *nlh, u16 *vid,
			     u16 *br_flags)
{
	struct nlattr *attr, *br_spec, *protinfo;
	int rem;
	struct bridge_vlan_info *vinfo = NULL;

	protinfo = nlmsg_find_attr(nlh, sizeof(struct ifinfomsg),
				   IFLA_PROTINFO);
	if (protinfo)
		return DP_SUCCESS;

	br_spec = nlmsg_find_attr(nlh, sizeof(struct ifinfomsg), IFLA_AF_SPEC);
	if (!br_spec)
		return -EINVAL;

	nla_for_each_nested(attr, br_spec, rem) {
		if ((nla_type(attr) != IFLA_BRIDGE_FLAGS) &&
		    (nla_type(attr) != IFLA_BRIDGE_VLAN_INFO))
			continue;

		if (nla_type(attr) == IFLA_BRIDGE_VLAN_INFO) {
			if (nla_len(attr) != sizeof(struct bridge_vlan_info))
				return -EINVAL;
			vinfo = nla_data(attr);
			if (vinfo)
				*vid = vinfo->vid;
		}

		if (nla_type(attr) == IFLA_BRIDGE_FLAGS) {
			if (nla_len(attr) < sizeof(u16))
				return -EINVAL;
			*br_flags = nla_get_u16(attr);
		}
	}

	return DP_SUCCESS;
}

/* for bridge device ndo hacking.
 * This will be called for bridge add dev eth0 vid 100
 * By this time bridge interface is added to bridge
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 15, 15)
int dp_ndo_bridge_setlink(struct net_device *dev, struct nlmsghdr *nlh,
				     u16 flags)
#else
int dp_ndo_bridge_setlink(struct net_device *dev, struct nlmsghdr *nlh,
			  u16 flags, struct netlink_ext_ack *extack)
#endif
{
	u16 vid = 0;
	u16 br_flags = 0;
	struct net_device *br_dev;
	struct br_info *br_info;
	struct dp_dev *dp_dev;
	int ret = -EINVAL;
	ndo_bridge_setlink setlink;

	if (!dev || !nlh)
		return -EINVAL;

	if (netif_is_bridge_master(dev)) {
		br_dev = dev;
	} else if (netif_is_bridge_port(dev)) {
		br_dev = netdev_master_upper_dev_get(dev);
	} else {
		pr_err("DPM: %s: wrongly trigger for %s\n", __func__, dev->name);
		return -EOPNOTSUPP;
	}

	/* note: br_flag value should be BRIDGE_FLAGS_SELF or
	 *       BRIDGE_FLAGS_MASTER. In fact parameter flags already has
	 *       similar setting.
	 *       If with BRIDGE_FLAGS_SELF, dev is bridge device.
	 *       If with BRIDGE_FLAGS_MASTER, dev is bridge port device, but
	 *          ndo_bridge_setlink is from bridge's.
	 *       For detail, refer to kernel design rtnl_bridge_setlink API
	 */
	if (parse_netlink_msg(nlh, &vid, &br_flags)) {
		pr_err("DPM: %s: Error in parsing netlink msg for %s flags=0x%x\n",
		       __func__, dev->name, flags);
		return -EINVAL;
	}
	DP_DEBUG(DP_DBG_FLAG_BR_VLAN, "dev=%s vid=%d br_flags=0x%x flag=0x%x:%s%s\n",
		 dev->name, vid, br_flags, flags,
		 flags & BRIDGE_FLAGS_SELF ? "self" : "",
		 flags & BRIDGE_FLAGS_MASTER ? "master" : "");
	DP_LIB_LOCK(&dp_lock);
	if (flags & BRIDGE_FLAGS_MASTER) {
		if (!(dp_dev = dp_dev_lookup(dev))) { /* just for sanity check */
			pr_err("DPM: %s: dp_dev NULL for %s\n", __func__, dev->name);
			goto ERR_EXIT;
		}
		if (!dp_dev->count) {
			pr_err("DPM: %s: dp_dev->count zero for %s\n", __func__,
			       dev->name);
			goto ERR_EXIT;
		}
	}
	br_info = dp_swdev_bridge_entry_lookup(br_dev);
	if (!br_info) {
		pr_err("DPM: %s: bridge %s is not found by DPM yet\n", __func__,
		       br_dev->name);
		goto ERR_EXIT;
	}
	if (vid && flags) {
		if (dp_br_vlan_enabled(br_info->dev)) {
			br_info->br_vlan_en = true;
			if (br_flags & BRIDGE_FLAGS_SELF) {
				dp_br_vlan_self(br_info, vid, 0);
			} else if(br_flags & BRIDGE_FLAGS_MASTER) {
				dp_br_vlan_master(br_info, dev, vid, 0);
			} else {
				pr_err("DPM: %s: %s wrong br_flag=0x%x\n",
				       __func__, br_info->dev->name, br_flags);
				goto ERR_EXIT;
			}
		} else {
			DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
				 "%s: %s vlan aware not enabled\n", __func__,
				 br_info->dev->name);
		}
	}
	setlink = br_info->old_dev_ops->ndo_bridge_setlink;
	DP_LIB_UNLOCK(&dp_lock);
	/* Note: must unlock before call original saved ndo_xxx,
	 *       otherwise it will trigger dpm ndo_ again and cause double lock
	 */
	if (setlink)
		ret = CALL_SETLINK(setlink);
	else
		ret = 0;
	return ret;
ERR_EXIT:
	DP_LIB_UNLOCK(&dp_lock);
	return ret;
}

int dp_ndo_bridge_dellink(struct net_device *dev, struct nlmsghdr *nlh,
			  u16 flags)
{
	int ret = -EINVAL;
	u16 vid = 0;
	u16 br_flags = 0;
	struct net_device *br_dev;
	struct br_info *br_info;
	struct dp_dev *dp_dev = NULL;
	int (*dellink)(struct net_device *dev, struct nlmsghdr *nlh, u16 flags);

	if (!dev || !nlh)
		return -EINVAL;

	if (netif_is_bridge_master(dev)) {
		br_dev = dev;
	} else if (netif_is_bridge_port(dev)) {
		br_dev = netdev_master_upper_dev_get(dev);
	} else {
		pr_err("DPM: %s: wrongly trigger for %s\n", __func__, dev->name);
		return -EOPNOTSUPP;
	}

	/* note: br_flag value should be BRIDGE_FLAGS_SELF or
	 *       BRIDGE_FLAGS_MASTER. In fact parameter flags already has
	 *       similar setting.
	 *       If with BRIDGE_FLAGS_SELF, dev is bridge device.
	 *       If with BRIDGE_FLAGS_MASTER, dev is bridge port device, but
	 *          ndo_bridge_setlink is from bridge's.
	 *       For detail, refer to kernel design rtnl_bridge_setlink API
	 */
	if (parse_netlink_msg(nlh, &vid, &br_flags)) {
		pr_err("DPM: %s: Error in parsing netlink message\n", __func__);
		return -EINVAL;
	}
	DP_DEBUG(DP_DBG_FLAG_BR_VLAN, "dev=%s vid=%d br_flags=0x%x flag=0x%x:%s%s\n",
		 dev->name, vid, br_flags, flags,
		 flags & BRIDGE_FLAGS_SELF ? "self" : "",
		 flags & BRIDGE_FLAGS_MASTER ? "master" : "");

	DP_LIB_LOCK(&dp_lock);
	if (flags & BRIDGE_FLAGS_MASTER) {
		if (!(dp_dev = dp_dev_lookup(dev))) { /* just for sanity check */
			pr_err("DPM: %s: dp_dev NULL for %s\n", __func__, dev->name);
			goto ERR_EXIT;
		}
		if (!dp_dev->count) {
			pr_err("DPM: %s: dp_dev->count zero for %s\n", __func__,
			       dev->name);
			goto ERR_EXIT;
		}
	}
	br_info = dp_swdev_bridge_entry_lookup(br_dev);
	if (!br_info) {
		pr_err("DPM: %s: bridge %s is not found by DPM yet\n", __func__,
		       br_dev->name);
		goto ERR_EXIT;
	}
	if (vid && flags) {
		int vlan_flag = BR_VLAN_DEREGISTER;
		if (br_info->br_vlan_en) {
			if (br_flags & BRIDGE_FLAGS_SELF) {
				dp_br_vlan_self(br_info, vid, vlan_flag);
			} else if(br_flags & BRIDGE_FLAGS_MASTER) {
				dp_br_vlan_master(br_info, dev, vid, vlan_flag);
			} else {
				pr_err("DPM: %s: %s wrong br_flag=0x%x\n",
				       __func__, br_info->dev->name, br_flags);
				goto ERR_EXIT;
			}
		} else {
			DP_DEBUG(DP_DBG_FLAG_BR_VLAN, "%s: %s vlan aware not enabled\n", __func__,
			       br_info->dev->name);
		}
	}
	dellink = br_info->old_dev_ops->ndo_bridge_dellink;
	DP_LIB_UNLOCK(&dp_lock);

	/* Note: must unlock before call original saved ndo_xxx,
	 *       otherwise it will trigger dpm ndo_ again and cause double lock
	 */
	if (dellink)
		ret = dellink(dev, nlh, flags);
	else
		ret = 0;
	return ret;
ERR_EXIT:
	DP_LIB_UNLOCK(&dp_lock);
	return ret;
}

/* FiD for VLAN can be only 6 bits, Here we need to know
 * maxvlanid - Max VLAN ID supported per Bridge
 * br_dev: bridge device
 */
int dp_set_br_vlan_limit(struct net_device *br_dev, u32 maxvlanid)
{
	int ret = DP_FAILURE;
	struct br_info *br_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if ((maxvlanid > MAX_FID) || (maxvlanid == 0)) {
		pr_err("DPM: VLAN Aware support max 64 FiD (VlanID + BriID)\n");
		return DP_FAILURE;
	}

	if (!netif_is_bridge_master(br_dev)) {
		pr_err("DPM: %s: br vlan limit set for wrong dev %s\n",
		       __func__, br_dev->name);
		return DP_FAILURE;
	}

	DP_LIB_LOCK(&dp_lock);
	br_info = dp_swdev_bridge_entry_lookup(br_dev);
	if (!br_info) {
		pr_err("DPM: %s: dev %s is not registered to DPM\n", __func__,
		       br_dev->name);
		goto EXIT;
	}

	br_info->max_vlan_limit = maxvlanid;
	br_info->max_brid_limit =
			1 << ((ilog2(MAX_FID) - ilog2(maxvlanid)) - 1);

	DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
		 "max_vlan_limit=%d max_brid_limit=%d\n",
		 br_info->max_vlan_limit, br_info->max_vlan_limit);

	ret = 0;
EXIT:
	DP_LIB_UNLOCK(&dp_lock);
	return ret;
}
EXPORT_SYMBOL(dp_set_br_vlan_limit);
#else
int dp_register_br_vlan_ops(struct br_info *br_info, int flags)
{
	return 0;
}

int dp_gsw_copy_ctp_cfg(int inst, int bp, struct dp_subblk_info *subblk_info,
			int portid)
{
	return 0;
}
struct pce_entry *get_pce_entry_from_ventry(struct vlan_entry *ventry,
					    int portid, int subifid)
{
	return NULL;
}

#endif /* CONFIG_DPM_DATAPATH_SWITCHDEV */

/* disable optimization in debug mode: pop */

DP_NO_OPTIMIZE_POP

