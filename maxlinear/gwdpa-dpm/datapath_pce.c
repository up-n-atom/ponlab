// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2022, MaxLinear, Inc.
 * Copyright 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <linux/list.h>
#include <linux/list_sort.h>
#include <net/datapath_api.h>
#include "datapath.h"
#include "datapath_pce.h"
#include <net/datapath_proc_api.h>	/* for proc api */

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

/* PCE Flow Engine Common Region start index */
#define PCE_TFCR_START_IDX	0x46E

/* PCE Flow Engine Common Region Entry Numbers */
#define PCE_TFCR_NUM		0x46F

/* PCE Flow Engine Common Region Entry Numbers */
#define PCE_MAX_ENTRIES		512

/* Invalid Sub-Block specified as 0,
 * Passing this value means Framework will create a sub-block and pass back
 * the ID
 */
#define DP_INVALID_SUB_BLK 0

/* Global PCE Rule struct */
struct dp_pce_rule pce_rule;

/* DP PCE Sub-Block rule storage */
struct dp_pce_rule_storage {
	struct list_head list;
	/* DP index for users
	 * - permament, assigned during creation
	 */
	int sw_idx;
	/* Switch index
	 * - fleeting, might change due to sorting
	 */
	int hw_idx;
	/* Rule priority */
	int prio;
	/* PCE rule data */
	GSW_PCE_rule_t pce;
};

/* There are 2 purpose for this API
 * 1. checking whether continuous free from start index to size
 * 2. if not free, increment the start_index upto the next free element,
 *    it maynot be same as size
 */
static bool contiguous_free_idx(u32 *entries, int *start_idx, int size)
{
	u32 i;
	int idx = *start_idx;
	u32 not_continuous = 0;

	for (i = idx; i < (idx + size); i++) {
		if (entries[i]) {
			(*start_idx)++;
			not_continuous = 1;
		}
	}

	if (not_continuous)
		return false;
	return true;
}

/* Check whether entries from subblk_firstidx to subblk_size is free
 * subblk_firstidx - from 0 .. blk max
 * subblk_size - from 0 .. blk max
 */
static int check_subblk_entry_avail(struct dp_blk_entry *blk_entry,
				    struct dp_subblk_info *blk_info)
{
	struct dp_subblk_entry *subblk_entry = NULL;
	u32 *rules;
	int i = 0, start_free_idx = blk_info->subblk_firstidx;
	int start_idx = 0;

	DP_DEBUG(DP_DBG_FLAG_PCE,
		 "subblk_size: %d, subblk_firstidx: %d\n",
		 blk_info->subblk_size, blk_info->subblk_firstidx);

	if (blk_info->subblk_firstidx > blk_entry->blk_rules_max)
		return -1;

	if ((blk_info->subblk_firstidx + blk_info->subblk_size) >
	    blk_entry->blk_rules_max)
		return -1;

	rules = dp_kzalloc((sizeof(u32) * blk_entry->blk_rules_max), GFP_ATOMIC);
	if (!rules)
		return -1;

	/* Mark the entries as used */
	list_for_each_entry(subblk_entry, &blk_entry->subblk_list, list) {
		start_idx = subblk_entry->subblk_firstindex -
			    blk_entry->blk_firstindex;
		for (i = start_idx;
		     i < (start_idx + subblk_entry->subblk_rules_max);
		     i++) {
			rules[i] = 1;
		}
	}

	if (!contiguous_free_idx(rules, &start_free_idx, blk_info->subblk_size)) {
		kfree(rules);
		return -1;
	}

	kfree(rules);

	DP_DEBUG(DP_DBG_FLAG_PCE, "start_free_idx: %d\n",
		 (blk_entry->blk_firstindex + blk_info->subblk_firstidx));

	return (blk_entry->blk_firstindex + blk_info->subblk_firstidx);
}

/* Return the first free continuous entry inside the block */
static int get_subblk_free_idx(struct dp_blk_entry *blk_entry, int subblk_size)
{
	struct dp_subblk_entry *subblk_entry;
	u32 *rules;
	int i = 0, start_free_idx = -1;
	int start_idx = 0, idx = 0;

	DP_DEBUG(DP_DBG_FLAG_PCE, "subblk_size: %d\n", subblk_size);

	rules = dp_kzalloc((sizeof(u32) * blk_entry->blk_rules_max), GFP_ATOMIC);
	if (!rules)
		return -1;

	/* Mark the entries as used */
	list_for_each_entry(subblk_entry, &blk_entry->subblk_list, list) {
		start_idx = subblk_entry->subblk_firstindex - blk_entry->blk_firstindex;
		for (i = start_idx;
		     i < (start_idx + subblk_entry->subblk_rules_max);
		     i++) {
			rules[i] = 1;
		}
	}

	/* Find the first continuous free index */
	while ((idx + subblk_size) <= blk_entry->blk_rules_max) {
		if (!contiguous_free_idx(rules, &idx, subblk_size))
			continue;
		start_free_idx = idx;
		break;
	}

	kfree(rules);

	DP_DEBUG(DP_DBG_FLAG_PCE, "%d: start_free_idx: %d\n",
		 __LINE__,
		 (blk_entry->blk_firstindex + start_free_idx));

	if (start_free_idx == -1)
		return -1;
	else
		return (blk_entry->blk_firstindex + start_free_idx);
}

/* For Global if Type matches get the entry
 * For CTP, based on portid and subif id get the block
 */
static struct dp_blk_entry *get_blk_entry(struct dp_subblk_info *blk_info,
					  GSW_PCE_RuleRegion_t region)
{
	struct dp_blk_entry *blk_entry;
	struct dp_subblk_info *c_blk_info;

	list_for_each_entry(blk_entry, &pce_rule.blk_list, list) {
		if (blk_entry->info.region != region)
			continue;

		if (region == GSW_PCE_RULE_CTP) {
			c_blk_info = &blk_entry->info.info;
			if ((c_blk_info->portid == blk_info->portid) &&
			    (c_blk_info->subif == blk_info->subif))
				return blk_entry;
		} else {
			return blk_entry;
		}
	}

	return NULL;
}

/* Get the sub-block based on the PCE Type ID */
static struct dp_subblk_entry *get_subblk_entry(struct dp_blk_entry *blk_entry,
						int subblk_id)
{
	struct dp_subblk_entry *subblk_entry;

	/* if sub-block id is <=0, need to create a new sub-block entry */
	if (subblk_id <= DP_INVALID_SUB_BLK)
		return NULL;

	list_for_each_entry(subblk_entry, &blk_entry->subblk_list, list) {
		if (subblk_entry->subblk_id != subblk_id)
			continue;
		return subblk_entry;
	}

	return NULL;
}

static int set_bp_pce_flow_index(struct dp_blk_entry *blk_entry,
				 GSW_PCE_rule_alloc_t *alloc)
{
	struct core_ops *ops;
	struct dp_subblk_info *blk_info = &blk_entry->info.info;
	struct bp_pmapper *bp_info;
	GSW_CTP_portConfig_t *ctp;
	struct ctp_dev *ctp_entry;

	bp_info = get_dp_bp_info(blk_entry->info.inst, blk_info->bp);
	if ((!bp_info) || (!bp_info->ref_cnt)) {
		pr_err("DPM: %s: get_dp_bp_info failed, bp: %u\n",
		       __func__, blk_info->bp);
		return -1;
	}

	ops = gsw_get_swcore_ops(0);
	if (!ops)
		return -1;
	ctp = dp_kzalloc(sizeof(*ctp), GFP_ATOMIC);
	if (!ctp)
		return -1;

	ctp->nLogicalPortId = blk_info->portid;
	if (alloc)
		ctp->nFirstFlowEntryIndex = alloc->blockid;
	else
		ctp->nFirstFlowEntryIndex = CTP_FLOW_ENTRY_IDX_RESET;
	ctp->eMask = GSW_CTP_PORT_CONFIG_FLOW_ENTRY;

	/* Retrieve the CTP dev list for the pmapper bridge port */
	list_for_each_entry(ctp_entry, &bp_info->ctp_dev, list) {
		if (!ctp_entry->dev)
			continue;

		ctp->nSubIfIdGroup = ctp_entry->ctp;
		if (ops->gsw_ctp_ops.CTP_PortConfigSet(ops, ctp)) {
			pr_err("DPM: %s: CfgSet failed, ctp: %u, nFirstIdx: %u\n",
			       __func__, ctp->nSubIfIdGroup,
			       ctp->nFirstFlowEntryIndex);
			kfree(ctp);
			return -1;
		}
		DP_DEBUG(DP_DBG_FLAG_PCE,
			 "CTPPortCfgSet Succ nSubifIdGroup:%u BP:%u %s%u\n",
			 ctp->nSubIfIdGroup, ctp->nBridgePortId,
			 "Lpid:", ctp->nLogicalPortId);
	}
	kfree(ctp);

	return 0;
}

static int set_ctp_pce_flow_index(struct dp_blk_entry *blk_entry,
				  GSW_PCE_rule_alloc_t *alloc)
{
	struct core_ops *ops = gsw_get_swcore_ops(0);
	struct dp_subblk_info *blk_info = &blk_entry->info.info;
	GSW_CTP_portConfig_t *ctp;

	if (!ops)
		return -1;

	ctp = dp_kzalloc(sizeof(*ctp), GFP_ATOMIC);
	if (!ctp)
		return -1;

	ctp->nLogicalPortId = blk_info->portid;
	ctp->nSubIfIdGroup = blk_info->subif;
#ifndef USE_NEW_ALLOC_HANDLE /* New handle failed in LGM_PON model. need co-debug with pon team later */
	if (alloc)
		ctp->nFirstFlowEntryIndex = alloc->blockid;
	else
		ctp->nFirstFlowEntryIndex = CTP_FLOW_ENTRY_IDX_RESET;
	ctp->eMask = GSW_CTP_PORT_CONFIG_FLOW_ENTRY;

	if (ops->gsw_ctp_ops.CTP_PortConfigSet(ops, ctp)) {
		pr_err("DPM: %s: CfgSet failed, Lpid: %u, nFirstIdx: %u\n",
		       __func__, ctp->nLogicalPortId,
		       ctp->nFirstFlowEntryIndex);
		kfree(ctp);
		return -1;
	}
#else
	if (alloc) {
		ctp->eMask = GSW_CTP_PORT_CONFIG_FLOW_ENTRY;
		ctp->nFirstFlowEntryIndex = alloc->blockid;
		if (ops->gsw_ctp_ops.CTP_PortConfigSet(ops, ctp)) {
			pr_err("DPM: %s: CfgSet failed, Lpid: %u, nFirstIdx: %u\n",
			       __func__, ctp->nLogicalPortId,
			       ctp->nFirstFlowEntryIndex);
			kfree(ctp);
			return -1;
		}
	} else {
		ctp->eMask = GSW_CTP_PORT_CONFIG_FLOW_ENTRY;
		if (ops->gsw_ctp_ops.CTP_PortConfigGet(ops, ctp)) {
			pr_err("DPM: %s: CTP_PortConfigGet for %s=%d %s=%d\n",
			       __func__,
			       "nLogicalPortId", ctp->nLogicalPortId,
			       "nSubIfIdGroup", ctp->nSubIfIdGroup);
			kfree(ctp);
			return -1;
		}
		/* Note: here is CTP_PortConfigReset, not CTP_PortConfigSet */
		ctp->eMask = GSW_CTP_PORT_CONFIG_FLOW_ENTRY;
		if (ops->gsw_ctp_ops.CTP_PortConfigReset(ops, ctp)) {
			pr_err("DPM: %s: CfgSet failed, Lpid: %u, nFirstIdx: %u\n",
			       __func__, ctp->nLogicalPortId,
			       ctp->nFirstFlowEntryIndex);
			kfree(ctp);
			return -1;
		}
	}
#endif
	DP_DEBUG(DP_DBG_FLAG_PCE,
		 "CTP_PortCfgSet success, Lpid: %u, EntryIdx: %u\n",
		 ctp->nLogicalPortId,
		 ctp->nFirstFlowEntryIndex);
	kfree(ctp);

	return 0;
}

static int dp_set_pce_flow_index(struct dp_blk_entry *blk_entry,
				 GSW_PCE_rule_alloc_t *alloc)
{
	if (blk_entry->info.info.bp > 0)
		return set_bp_pce_flow_index(blk_entry, alloc);

	return set_ctp_pce_flow_index(blk_entry, alloc);
}

/* Add a block, If already existing return the entry
 * blk_info : Information passed by User
 * blk_size : Global Block size from GSWIP register,
 *            For CTP, block size is passed during alloc
 * Return   : Block Entry Allocated/Already present
 */
static struct dp_blk_entry *blk_add(struct dp_pce_blk_info *pce_blk_info,
				    int blk_size)
{
	GSW_PCE_rule_alloc_t alloc = {0};
	struct dp_blk_entry *blk_entry;
	struct core_ops *ops = gsw_get_swcore_ops(0);
	struct dp_subblk_info *blk_info;
	int ret;

	if (!ops)
		return NULL;

	blk_info = &pce_blk_info->info;

	/* if already have return the entry */
	blk_entry = get_blk_entry(blk_info, pce_blk_info->region);
	if (blk_entry)
		return blk_entry;

	blk_entry = dp_kzalloc(sizeof(*blk_entry), GFP_ATOMIC);
	if (!blk_entry)
		return NULL;

	blk_entry->info.region = pce_blk_info->region;
	dp_memcpy(&blk_entry->info.info, blk_info, sizeof(*blk_info));

	/* Alloc is used only for CTP region not for Global Rule */
	alloc.num_of_rules = blk_size;
	if (pce_blk_info->region == GSW_PCE_RULE_CTP) {
		if (ops->gsw_tflow_ops.TFLOW_PceRuleAlloc(ops, &alloc)) {
			pr_err("DPM: %s: TFLOW_PceRuleAlloc failed\n", __func__);
			kfree(blk_entry);
			return NULL;
		}

		DP_DEBUG(DP_DBG_FLAG_PCE,
			 "%s: RuleAlloc success, rules: %u, blk_id: %u\n",
			 __func__, alloc.num_of_rules, alloc.blockid);

		ret = dp_set_pce_flow_index(blk_entry, &alloc);
		if (ret) {
			ops->gsw_tflow_ops.TFLOW_PceRuleFree(ops, &alloc);
			kfree(blk_entry);
			return NULL;
		}
	} else {
		alloc.blockid = pce_rule.glbl_firstindex;
		alloc.num_of_rules = pce_rule.glbl_rules_max;
	}

	blk_entry->blk_firstindex = alloc.blockid;
	blk_entry->blk_rules_max = alloc.num_of_rules;

	snprintf(blk_entry->blk_name, sizeof(blk_entry->blk_name), "blk-%d",
		 blk_entry->blk_firstindex);

	INIT_LIST_HEAD(&blk_entry->subblk_list);
	list_add(&blk_entry->list, &pce_rule.blk_list);

	DP_DEBUG(DP_DBG_FLAG_PCE,
		 "%s %s %d %s %d %s %s %s %d RuleMax %d RuleUsed %d\n",
		 "Success",
		 "Blk Portid",
		 blk_entry->info.info.portid,
		 "Subif",
		 blk_entry->info.info.subif,
		 "Region",
		 (blk_entry->info.region == GSW_PCE_RULE_CTP) ?
		 "CTP" : "GLOBAL",
		 "FirstIdx",
		 blk_entry->blk_firstindex,
		 blk_entry->blk_rules_max,
		 blk_entry->blk_rules_used);
	return blk_entry;
}

/* Delete a block
 * blk_entry : Block Entry Allocated
 * Return    : -1 on Failure, 0 on Success
 */
static int blk_del(struct dp_blk_entry *blk_entry)
{
	GSW_PCE_rule_alloc_t alloc = {0};
	struct core_ops *ops = gsw_get_swcore_ops(0);

	if (!ops)
		return -1;

	DP_DEBUG(DP_DBG_FLAG_PCE,
		 "%s %d %s %d %s %s FirstIdx %d RuleMax %d RuleUsed %d\n",
		 "Blk Portid",
		 blk_entry->info.info.portid,
		 "Subif",
		 blk_entry->info.info.subif,
		 "Region",
		 (blk_entry->info.region == GSW_PCE_RULE_CTP) ?
		 "CTP" : "GLOBAL",
		 blk_entry->blk_firstindex,
		 blk_entry->blk_rules_max,
		 blk_entry->blk_rules_used);

	if (blk_entry->blk_rules_used) {
		DP_DEBUG(DP_DBG_FLAG_PCE,
			 "Someone Still using this %s blk, Cannot del\n",
			 (blk_entry->info.region == GSW_PCE_RULE_CTP) ?
			 "CTP" : "Global");
		return DP_SUCCESS;
	}

	if (blk_entry->info.region == GSW_PCE_RULE_CTP) {
		if (dp_set_pce_flow_index(blk_entry, NULL)) {
			pr_err("DPM: %s: reset CTP pce flow index failed\n",
			       __func__);
			return -1;
		}

		alloc.blockid = blk_entry->blk_firstindex;
		if (ops->gsw_tflow_ops.TFLOW_PceRuleFree(ops, &alloc)) {
			pr_err("DPM: %s: TFLOW_PceRuleFree failed,subblk_id:%u\n",
			       __func__, alloc.blockid);
			return -1;
		}
	}

	/* Free the block entry */
	list_del(&blk_entry->list);
	kfree(blk_entry);

	return DP_SUCCESS;
}

/* Add a Sub-Block, If already existing return the entry
 * blk_entry : Block Entry Allocated
 * info      : Block Information passed by User
 * Return    : Sub-Block Entry Allocated/Already present
 */
static struct dp_subblk_entry *subblk_add(struct dp_blk_entry *blk_entry,
					  struct dp_subblk_info *blk_info)
{
	struct dp_subblk_entry *subblk_entry;
	int rules_left = 0, firstindex = 0;
	struct core_ops *ops = gsw_get_swcore_ops(0);
	GSW_PCE_GlobalBitMap_t gbitmap = {0};

	if (!ops)
		return NULL;
	/* if already have return the entry */
	subblk_entry = get_subblk_entry(blk_entry, blk_info->subblk_id);
	if (subblk_entry)
		return subblk_entry;

	/* Give a valid sub-block size, if not given will assume as 1 */
	if (blk_info->subblk_size <= 0) {
		DP_DEBUG(DP_DBG_FLAG_PCE,
			 "Give a valid Sub-Block Size %d, assuming as 1\n",
			 blk_info->subblk_size);
		blk_info->subblk_size = 1;
	}

	rules_left = blk_entry->blk_rules_max - blk_entry->blk_rules_used;

	if (blk_info->subblk_size > rules_left) {
		pr_err("DPM: %s: No Space to add sub-block Already %d/%d used\n",
		       __func__, blk_entry->blk_rules_used,
		       blk_entry->blk_rules_max);
		return NULL;
	}

	/* Allocate and block num of rules in Global entries
	 * Sub-block Free index will be found out by GSWIP API entry
	 */
	if (blk_entry->info.region == GSW_PCE_RULE_COMMMON) {
		gbitmap.num_of_rules = blk_info->subblk_size;
		if (ops->gsw_tflow_ops.TFLOW_GlobalRule_BitMapAlloc(ops, &gbitmap)) {
			pr_err("DPM: %s: For Global rules cont %d space not avail\n",
			       __func__, blk_info->subblk_size);
			return NULL;
		}
		firstindex = gbitmap.base_index;
	} else {
		/* if subblk_firstidx > 0, Use this as first index to allocate
		 * else, find a free index
		 */
		if (blk_info->subblk_firstidx > 0) {
			firstindex = check_subblk_entry_avail(blk_entry, blk_info);
			if (firstindex == -1) {
				pr_err("DPM: %s: Sub-Blk specified FirstIdx %d not avail\n",
				       __func__, blk_info->subblk_firstidx);
				return NULL;
			}
		} else {
			firstindex = get_subblk_free_idx(blk_entry, blk_info->subblk_size);
			if (firstindex == -1) {
				pr_err("DPM: %s: No continuous Space Avail %d/%d used\n",
				       __func__,
				       blk_entry->blk_rules_used,
				       blk_entry->blk_rules_max);
				return NULL;
			}
		}
	}

	subblk_entry = dp_kzalloc(sizeof(*subblk_entry), GFP_ATOMIC);
	if (!subblk_entry)
		return NULL;

	subblk_entry->subblk_firstindex = firstindex;
	subblk_entry->subblk_rules_max = blk_info->subblk_size;

	/* While adding create a sub-blk id and return back in same struct
	 * subblk first idx will be a unique number always use that as ID
	 */
	blk_info->subblk_id = subblk_entry->subblk_firstindex + 1;
	blk_info->subblk_firstidx = subblk_entry->subblk_firstindex;

	subblk_entry->subblk_id = blk_info->subblk_id;

	if (!strlen(blk_info->subblk_name)) {
		snprintf(subblk_entry->subblk_name, sizeof(subblk_entry->subblk_name),
			"subblk-%d", subblk_entry->subblk_id);
	} else {
		dp_strlcpy(subblk_entry->subblk_name, blk_info->subblk_name,
			   sizeof(subblk_entry->subblk_name));
	}

	blk_entry->blk_rules_used += blk_info->subblk_size;

	/* User can explicity change the protection mode during delete
	 * if 1 - this sub-blk won't be removed even after all rules deleted
	 * if 0 - this sub-blk will be removed after all rules removed
	 */
	subblk_entry->subblk_protected = blk_info->subblk_protected;

	INIT_LIST_HEAD(&subblk_entry->rules_list);
	list_add(&subblk_entry->list, &blk_entry->subblk_list);

	DP_DEBUG(DP_DBG_FLAG_PCE,
		 "%s SubBlk FirstIdx %d RuleMax %d RuleUsed %d\n",
		 "Success",
		 subblk_entry->subblk_firstindex,
		 subblk_entry->subblk_rules_max,
		 subblk_entry->subblk_rules_used);
	return subblk_entry;
}

/* Delete a sub-block
 * blk_entry    : Block Entry Allocated
 * subblk_entry : Sub-Block Entry Allocated
 * Return       : -1 on Failure, 0 on Success
 */
static int subblk_del(struct dp_blk_entry *blk_entry,
		      struct dp_subblk_entry *subblk_entry)
{
	struct core_ops *ops = gsw_get_swcore_ops(0);
	GSW_PCE_GlobalBitMap_t gbitmap = {0};

	if (!ops)
		return -1;

	DP_DEBUG(DP_DBG_FLAG_PCE,
		 "%s %d %s %s %s %d %s %d %s %d %s %d\n",
		 "SubBlkId", subblk_entry->subblk_id,
		 "Name", subblk_entry->subblk_name,
		 "FirstIdx", subblk_entry->subblk_firstindex,
		 "RuleMax", subblk_entry->subblk_rules_max,
		 "RuleUsed", subblk_entry->subblk_rules_used,
		 "protected", subblk_entry->subblk_protected);

	/* Delete the sub-block only when all rules are removed and sub-block is
	 * not protected
	 */
	if (!subblk_entry->subblk_rules_used &&
	    !subblk_entry->subblk_protected) {
		if (blk_entry->info.region == GSW_PCE_RULE_COMMMON) {
			/* Mark all the subblk rules as free free in GSWIP API */
			gbitmap.num_of_rules = subblk_entry->subblk_rules_max;
			gbitmap.base_index = subblk_entry->subblk_firstindex;
			if (ops->gsw_tflow_ops.TFLOW_GlobalRule_BitMapFree(ops, &gbitmap)) {
				pr_err("DPM: %s: TFLOW_GlobalRule_BitMapFree failed\n", __func__);
				return -1;
			}
		}
		blk_entry->blk_rules_used -= subblk_entry->subblk_rules_max;
		list_del(&subblk_entry->list);
		kfree(subblk_entry);
	}

	return DP_SUCCESS;
}

/* This function shifts up PCE rules in hardware in order to overwrite
 * hardware index after rule deletion.
 */
static int pce_rules_list_remove(struct dp_blk_entry *blk_entry,
				 struct dp_subblk_entry *subblk_entry,
				 GSW_PCE_ruleDelete_t *pce)
{
	struct core_ops *ops = gsw_get_swcore_ops(0);
	struct dp_pce_rule_storage *p;
	int index = subblk_entry->subblk_firstindex -
		    blk_entry->blk_firstindex;

	if (!ops)
		return -1;
	list_for_each_entry(p, &subblk_entry->rules_list, list) {
		if (p->hw_idx == index) {
			index++;
			continue;
		}
		p->pce.pattern.nIndex = index;
		if (ops->gsw_tflow_ops.TFLOW_PceRuleWrite(ops, &p->pce)) {
			pr_err("DPM: %s: TFLOW_PceRuleWite failed, nIndex: %u\n",
			       __func__, p->pce.pattern.nIndex);
			return -1;
		}
		DP_DEBUG(DP_DBG_FLAG_PCE,
			 "Success Pce Rule Added to HW Idx %d\n",
			 p->pce.pattern.nIndex);
		p->hw_idx = index++;
	}

	pce->nIndex = index;
	if (ops->gsw_tflow_ops.TFLOW_PceRuleDelete(ops, pce)) {
		pr_err("DPM: %s: TFLOW_PceRule Delete failed, nIndex: %u\n",
		       __func__, pce->nIndex);
		return -1;
	}

	return 0;
}

/* This function is used to remove and update Sub-block rules list. */
static int pce_rule_remove(struct dp_blk_entry *blk_entry,
			  struct dp_subblk_entry *subblk_entry,
			  GSW_PCE_ruleDelete_t *pce)
{
	struct dp_pce_rule_storage *p;
	bool found = false;
	int ret;

	list_for_each_entry(p, &subblk_entry->rules_list, list) {
		if (p->sw_idx == pce->nIndex) {
			found = true;
			break;
		}
	}
	if (!found) {
		DP_DEBUG(DP_DBG_FLAG_PCE,
			 "Entry to delete not found!\n" );
		return DP_SUCCESS;
	}

	list_del(&p->list);
	kfree(p);
	subblk_entry->subblk_rules_used--;

	ret = pce_rules_list_remove(blk_entry, subblk_entry, pce);
	if (ret) {
		pr_err("DPM: %s: pce_rules_add_rev_apply failed! %d\n",
		       __func__, ret);
		return ret;
	}

	return DP_SUCCESS;
}

/* Pce Rule Operations
 * pce_rule     : Pce rule Entry to del
 * subblk_entry : Sub-Block on which rule need to be deleted
 */
static int pce_rule_del(GSW_PCE_ruleDelete_t *pce,
			struct dp_blk_entry *blk_entry,
			struct dp_subblk_entry *subblk_entry)
{
	struct core_ops *ops = gsw_get_swcore_ops(0);
	int sw_idx = pce->nIndex;
	int ret;

	if (!ops)
		return -1;

	if (pce->nIndex > subblk_entry->subblk_rules_max) {
		pr_err("DPM: %s: PCE Index for Block %d should be within 0 to %d, nIndex: %u\n",
		       __func__, subblk_entry->subblk_id,
		       subblk_entry->subblk_rules_max,
		       sw_idx);
		return -1;
	}

	if (!subblk_entry->subblk_rules_used) {
		DP_DEBUG(DP_DBG_FLAG_PCE,
			 "Entries in sub-blk freed, subblk protect %d\n",
			 subblk_entry->subblk_protected);
		return DP_SUCCESS;
	}

	DP_DEBUG(DP_DBG_FLAG_PCE, "%s %d %s %d %s %d\n",
		 "subblk_firstindex", subblk_entry->subblk_firstindex,
		 "blk_firstindex", blk_entry->blk_firstindex,
		 "nIndex", sw_idx);

	ret = pce_rule_remove(blk_entry, subblk_entry, pce);
	if (ret) {
		pr_err("DPM: %s: pce_rule_remove failed! %d\n", __func__, ret);
		return -1;
	}

	/* update sub-block bitmask control */
	dp_clear_bit(sw_idx, subblk_entry->_used);

	DP_DEBUG(DP_DBG_FLAG_PCE, "Success Pce Rule Del from %s %d %s %d\n",
		 "nIndex", sw_idx,
		 "Sub-Block rules used", subblk_entry->subblk_rules_used);

	return DP_SUCCESS;
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP

/* This function returns Sub-Block relative index */
static int pce_rule_sw_idx_get(struct dp_subblk_entry *subblk_entry)
{
	int i;
	u32 start = 0;
	u32 end = subblk_entry->subblk_rules_max;

	DP_DEBUG(DP_DBG_FLAG_PCE, "%s %u %s %u\n",
		 "start", start,
		 "end", end);

	for (i = start; i < end; i++) {
		if (test_bit(i, subblk_entry->_used))
			continue;
		return i;
	}

	return DP_FAILURE;
}

/* This function refresh rules in hardware after list is updated */
static int pce_rules_list_apply(struct dp_blk_entry *blk_entry,
				struct dp_subblk_entry *subblk_entry,
				int force_rewrite)
{
	struct core_ops *ops = gsw_get_swcore_ops(0);
	struct dp_pce_rule_storage *p;
	int first_index = subblk_entry->subblk_firstindex -
			  blk_entry->blk_firstindex;
	/* Start from highest rule index */
	int current_index = first_index + subblk_entry->subblk_rules_used - 1;

	if (!ops)
		return -1;
	/* Reapply rules from new end index to not disrupt traffic */
	list_for_each_entry_reverse(p, &subblk_entry->rules_list, list) {
		if (current_index < first_index) {
			pr_err("DPM: %s: tried to access index outside subblock, index: %d, subblock first index: %d\n",
			       __func__, current_index, first_index);
			return -1;
		}

		/* Skip writing this rule to the hardware if the index did not
		 * change. If the index was given in force_rewrite write it to
		 * the hardware anyway.
		 */
		if (current_index == p->hw_idx && p->hw_idx != force_rewrite) {
			current_index--;
			continue;
		}
		p->pce.pattern.nIndex = current_index;
		if (ops->gsw_tflow_ops.TFLOW_PceRuleWrite(ops, &p->pce)) {
			pr_err("DPM: %s: TFLOW_PceRuleWite failed, nIndex: %u\n",
			       __func__, p->pce.pattern.nIndex);
			return -1;
		}
		DP_DEBUG(DP_DBG_FLAG_PCE,
			 "Success Pce Rule Added to HW Idx %d\n",
			 p->pce.pattern.nIndex);
		p->hw_idx = current_index--;
	}

	return 0;
}

/* This function is used to sort rules list according to priority. */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 13, 0) && \
    !(LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 70) && \
      LINUX_VERSION_CODE < KERNEL_VERSION(5, 11, 0))
int pce_rule_prio_cmp(void *priv, struct list_head *lh_a,
		      struct list_head *lh_b)
#else
int pce_rule_prio_cmp(void *priv, const struct list_head *lh_a,
		      const struct list_head *lh_b)
#endif
{
	struct dp_pce_rule_storage *la = list_entry(lh_a,
		struct dp_pce_rule_storage, list);
	struct dp_pce_rule_storage *lb = list_entry(lh_b,
		struct dp_pce_rule_storage, list);

	if (la->prio > lb->prio)
		return 1;

	if (la->prio < lb->prio)
		return -1;

	return 0;
}

/* This function is used to store and apply Sub-block rules list. */
static int pce_rule_store(struct dp_blk_entry *blk_entry,
			  struct dp_subblk_entry *subblk_entry,
			  GSW_PCE_rule_t *pce,
			  int sw_idx, int prio)
{
	struct dp_pce_rule_storage *rule;
	int ret;

	rule = dp_kzalloc(sizeof(*rule), GFP_KERNEL);
	if (!rule)
		return -ENOMEM;

	rule->sw_idx = sw_idx;
	rule->hw_idx = -1;
	rule->prio = prio;
	dp_memcpy(&rule->pce, pce, sizeof(*pce));

	list_add_tail(&rule->list, &subblk_entry->rules_list);
	list_sort(NULL, &subblk_entry->rules_list, pce_rule_prio_cmp);
	subblk_entry->subblk_rules_used++;

	ret = pce_rules_list_apply(blk_entry, subblk_entry, -1);
	if (ret) {
		subblk_entry->subblk_rules_used--;
		list_del(&rule->list);
		kfree(rule);
		pr_err("DPM: %s: pce_rules_list_apply failed! %d\n",
		       __func__, ret);
		return ret;
	}

	return 0;
}

/* Pce Rule Operations
 * pce_rule     : Pce rule Entry to add
 * subblk_entry : Sub-Block on which rule need to be added
 * ret : Sub-Block relative index on which rule was added
 */
static int pce_rule_add(GSW_PCE_rule_t *pce,
			struct dp_blk_entry *blk_entry,
			struct dp_subblk_entry *subblk_entry,
			int prio)
{
	struct core_ops *ops = gsw_get_swcore_ops(0);
	int sw_idx, ret;

	if (!ops)
		return -1;

	DP_DEBUG(DP_DBG_FLAG_PCE, "%s %d %s %d %s %d %s %d\n",
		 "subblk_firstindex", subblk_entry->subblk_firstindex,
		 "blk_firstindex", blk_entry->blk_firstindex,
		 "subblk_rules_used", subblk_entry->subblk_rules_used,
		 "subblk_rules_max", subblk_entry->subblk_rules_max);

	pce->pattern.bEnable = 1;

	/* prevent bitmask overflow */
	if (subblk_entry->subblk_rules_max > PCE_SUBBLK_SIZE_MAX) {
		pr_err("DPM: %s: Sub-block max rules(%u) > max default(%d)\n",
		       __func__, subblk_entry->subblk_rules_max,
		       PCE_SUBBLK_SIZE_MAX);
		return -1;
	}

	sw_idx = pce_rule_sw_idx_get(subblk_entry);
	if (sw_idx < 0) {
		pr_err("DPM: %s: Sub-block sw_idx_get failed!\n", __func__);
		return -1;
	}

	ret = pce_rule_store(blk_entry, subblk_entry, pce, sw_idx, prio);
	if (ret) {
		pr_err("DPM: %s: pce_rule_store failed! %d\n", __func__, ret);
		return -1;
	}

	/* update sub-block bitmask control */
	set_bit(sw_idx, subblk_entry->_used);

	DP_DEBUG(DP_DBG_FLAG_PCE, "Success Pce Rule Added to DP Idx %d\n",
		 sw_idx);

	/* save sub-block relative index for user inside PCE structure */
	pce->pattern.nIndex = sw_idx;

	return sw_idx;
}

static int pce_rule_hw_idx_get(struct dp_subblk_entry *subblk_entry,
			       int sw_idx)
{
	struct dp_pce_rule_storage *p;

	list_for_each_entry_reverse(p, &subblk_entry->rules_list, list) {
		if (p->sw_idx == sw_idx)
			return p->hw_idx;
	}

	return -1;
}

/* Pce Rule Operations
 * pce_rule     : Pce rule Entry to Read
 * subblk_entry : Sub-Block on which rule need to be read
 */
static int pce_rule_get(GSW_PCE_rule_t *pce,
			struct dp_blk_entry *blk_entry,
			struct dp_subblk_entry *subblk_entry)
{
	struct core_ops *ops = gsw_get_swcore_ops(0);
	int hw_idx;

	if (!ops)
		return -1;

	if (pce->pattern.nIndex > subblk_entry->subblk_rules_max) {
		pr_err("DPM: %s: PCE Index for Block %d should be within 0 to %d, nIndex: %u\n",
		       __func__, subblk_entry->subblk_id,
		       subblk_entry->subblk_rules_max,
		       pce->pattern.nIndex);
		return -1;
	}

	DP_DEBUG(DP_DBG_FLAG_PCE, "%s %d %s %d %s %d\n",
		 "subblk_firstindex", subblk_entry->subblk_firstindex,
		 "blk_firstindex", blk_entry->blk_firstindex,
		 "nIndex", pce->pattern.nIndex);

	hw_idx = pce_rule_hw_idx_get(subblk_entry, pce->pattern.nIndex);
	if (hw_idx < 0) {
		pr_err("DPM: %s: pce_rule_hw_idx_get failed, sw_idx: %d\n",
		       __func__, hw_idx);
		return -1;
	}
	pce->pattern.nIndex = hw_idx;

	if (ops->gsw_tflow_ops.TFLOW_PceRuleRead(ops, pce)) {
		pr_err("DPM: %s: TFLOW_PceRuleRead failed, nIndex: %u\n",
		       __func__, pce->pattern.nIndex);
		return -1;
	}

	DP_DEBUG(DP_DBG_FLAG_PCE, "Success Pce Rule Get from HW Idx %d\n",
		 pce->pattern.nIndex);

	return DP_SUCCESS;
}

/* Pce Rule update Operations
 * pce_rule     : Pce rule Entry to update
 * subblk_entry : Sub-Block on which rule need to be read
 * prio         : New priority of PCE rule (DP_PCE_RULE_REWRITE only)
 * update       : Update operation Enable/Disable or Rewrite Rule
 */
static int pce_rule_update(GSW_PCE_rule_t *pce,
			   struct dp_blk_entry *blk_entry,
			   struct dp_subblk_entry *subblk_entry,
			   int prio,
			   enum DP_PCE_RULE_UPDATE update)
{
	struct core_ops *ops = gsw_get_swcore_ops(0);
	struct dp_pce_rule_storage *p;
	bool found = false;
	int ret;

	if (!ops)
		return -1;

	if (update <= DP_PCE_RULE_NOCHANGE) {
		pr_err("DPM: %s: update value is no change or wrong %d\n",
		       __func__, update);
		return -1;
	}

	if (pce->pattern.nIndex > subblk_entry->subblk_rules_max) {
		pr_err("DPM: %s: PCE Index for Block %d should be within 0 to %d, nIndex: %u\n",
		       __func__, subblk_entry->subblk_id,
		       subblk_entry->subblk_rules_max,
		       pce->pattern.nIndex);
		return -1;
	}

	DP_DEBUG(DP_DBG_FLAG_PCE, "%s %d %s %d %s %d %s %d\n",
		 "subblk_firstindex", subblk_entry->subblk_firstindex,
		 "blk_firstindex", blk_entry->blk_firstindex,
		 "nIndex", pce->pattern.nIndex,
		 "Update", update);

	list_for_each_entry_reverse(p, &subblk_entry->rules_list, list) {
		if (p->sw_idx == pce->pattern.nIndex) {
			found = true;
			break;
		}
	}
	if (!found) {
		pr_err("DPM: %s: SW index not found, nIndex: %u\n",
		       __func__, pce->pattern.nIndex);
		return -1;
	}
	pce->pattern.nIndex = p->hw_idx;

	switch (update) {
	case DP_PCE_RULE_EN:
		if (ops->gsw_tflow_ops.TFLOW_PceRuleEnable(ops, pce)) {
			pr_err("DPM: %s: TFLOW_PceRuleEnable failed, nIndex: %u\n",
			       __func__, pce->pattern.nIndex);
			return -1;
		}
		break;
	case DP_PCE_RULE_DIS:
		if (ops->gsw_tflow_ops.TFLOW_PceRuleDisable(ops, pce)) {
			pr_err("DPM: %s: TFLOW_PceRuleDisable failed, nIndex: %u\n",
			       __func__, pce->pattern.nIndex);
			return -1;
		}
		break;
	case DP_PCE_RULE_REWRITE:
		dp_memcpy(&p->pce, pce, sizeof(*pce));
		p->prio = prio;
		list_sort(NULL, &subblk_entry->rules_list, pce_rule_prio_cmp);
		ret = pce_rules_list_apply(blk_entry, subblk_entry,
					   p->pce.pattern.nIndex);
		if (ret) {
			pr_err("DPM: %s: pce_rules_list_apply failed! %d\n",
			       __func__, ret);
			return -1;
		}
		break;
	default:
		break;
	}

	DP_DEBUG(DP_DBG_FLAG_PCE, "Success Pce Rule update for HW Idx %d\n",
		 pce->pattern.nIndex);

	return DP_SUCCESS;
}

/* API will create Block and Sub-Block
 * Pass proper portid, subif, subblock id, subblock size, region
 * return -1 fail, 0 - Success
 */
int dp_pce_blk_create(struct dp_pce_blk_info *pce_blk_info)
{
	struct pmac_port_info *port_info;
	struct dp_blk_entry *blk_entry;
	struct dp_subblk_entry *subblk_entry;
	struct dp_subblk_info *blk_info;
	int blk_size = 0, ret = DP_FAILURE;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!pce_blk_info) {
		pr_err("DPM: %s: blk_info 0x%px Null\n", __func__, pce_blk_info);
		return DP_FAILURE;
	}

	blk_info = &pce_blk_info->info;
	port_info = get_dp_port_info(pce_blk_info->inst, blk_info->portid);

	if (pce_blk_info->region == GSW_PCE_RULE_CTP) {
		blk_size = port_info->blk_size;
		if (!blk_size)
			blk_size = PCE_MAX_BLK_SIZE_CTP;
	} else {
		blk_size = pce_rule.glbl_rules_max;
	}

	DP_DEBUG(DP_DBG_FLAG_PCE,
		 "Block Size %d Region %d Portid %d Subif %d\n",
		 blk_size, pce_blk_info->region, blk_info->portid,
		 blk_info->subif);

	DP_LIB_LOCK(&dp_lock);
	blk_entry = blk_add(pce_blk_info, blk_size);
	if (!blk_entry) {
		pr_err("DPM: %s: No Block present or created\n", __func__);
		goto EXIT;
	}

	/* Check whether sub-block is created, if not create */
	subblk_entry = subblk_add(blk_entry, blk_info);
	if (!subblk_entry) {
		pr_err("DPM: %s: No Sub-Block present or created\n", __func__);
		blk_del(blk_entry);
		goto EXIT;
	}

	ret = DP_SUCCESS;

	DP_DEBUG(DP_DBG_FLAG_PCE,
		 "%s %s %s %d %s %d %s %d %s %s %d %s %d %s %d\n",
		 "Success",
		 "Blk",
		 "FirstIndex", blk_entry->blk_firstindex,
		 "RuleMax", blk_entry->blk_rules_max,
		 "RuleUsed", blk_entry->blk_rules_used,
		 "Sub-Block",
		 "FirstIndex", subblk_entry->subblk_firstindex,
		 "RuleMax", subblk_entry->subblk_rules_max,
		 "RuleUsed", subblk_entry->subblk_rules_used);
EXIT:
	DP_LIB_UNLOCK(&dp_lock);
	return ret;
}
EXPORT_SYMBOL(dp_pce_blk_create);

/* API will delete All the pce entries, sub-block and blk
 * Pass proper portid, subif, subblock id, region and protected or not
 * return -1 fail, 0 - Success
 */
int dp_pce_blk_del(struct dp_pce_blk_info *pce_blk_info)
{
	struct dp_blk_entry *blk_entry;
	struct dp_subblk_entry *subblk_entry;
	struct dp_subblk_info *blk_info;
	int ret = DP_FAILURE, i, rules_used;
	GSW_PCE_ruleDelete_t pce;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}
	if (!pce_blk_info) {
		pr_err("DPM: %s: blk_info 0x%px Null\n", __func__, pce_blk_info);
		return DP_FAILURE;
	}

	blk_info = &pce_blk_info->info;

	DP_DEBUG(DP_DBG_FLAG_PCE,
		 "%s %s %s %d %s %d %s %d %s %d %s %d\n",
		 "Region", (pce_blk_info->region == GSW_PCE_RULE_CTP) ?
		 "CTP" : "GLOBAL",
		 "Portid", blk_info->portid,
		 "Subif", blk_info->subif,
		 "subblk_size", blk_info->subblk_size,
		 "Protected", blk_info->subblk_protected,
		 "FirstIndex", blk_info->subblk_firstidx);

	DP_LIB_LOCK(&dp_lock);
	blk_entry = get_blk_entry(blk_info, pce_blk_info->region);
	if (!blk_entry) {
		pr_err("DPM: %s: Cannot find the blk_entry for portid=%d subif=%d\n",
		       __func__, blk_info->portid, blk_info->subif);
		goto EXIT;
	}

	subblk_entry = get_subblk_entry(blk_entry, blk_info->subblk_id);
	if (!subblk_entry) {
		pr_err("DPM: %s: Cannot find the subblk_entry for subblk_id=%d\n",
		       __func__, blk_info->subblk_id);
		goto EXIT;
	}

	DP_DEBUG(DP_DBG_FLAG_PCE,
		 "subblk_entry->subblk_rules_used %d\n",
		 subblk_entry->subblk_rules_used);

	pce.logicalportid = blk_info->portid;
	pce.subifidgroup = blk_info->subif;
	pce.region = pce_blk_info->region;

	rules_used = subblk_entry->subblk_rules_used;
	for (i = 0; i < rules_used; i++) {
		pce.nIndex = i;
		if (pce_rule_del(&pce, blk_entry, subblk_entry)) {
			pr_err("DPM: %s: pce_rule_del %d failed\n", __func__,
			       pce.nIndex);
			goto EXIT;
		}
	}

	/* User can explicity change the protection mode during delete
	 * if 1 - this sub-blk wont be removed even after all rules deleted
	 * if 0 - this sub-blk will be removed after all rules removed
	 */
	subblk_entry->subblk_protected = blk_info->subblk_protected;
	subblk_del(blk_entry, subblk_entry);

	if (blk_del(blk_entry)) {
		pr_err("DPM: %s: blk_del failed\n", __func__);
		goto EXIT;
	}

	ret = DP_SUCCESS;
EXIT:
	DP_LIB_UNLOCK(&dp_lock);
	DP_DEBUG(DP_DBG_FLAG_PCE, "%s\n",
		 (ret == DP_SUCCESS) ? "Success" : "Fail");
	return ret;
}
EXPORT_SYMBOL(dp_pce_blk_del);

/* PCE rule add API to call within DPM without lock */
int dp_pce_rule_add_priv(struct dp_pce_blk_info *pce_blk_info,
			 GSW_PCE_rule_t *pce)
{
	struct dp_subblk_entry *subblk_entry;
	struct core_ops *ops;
	struct dp_subblk_info *blk_info;
	struct pmac_port_info *port_info;
	struct dp_blk_entry *blk_entry;
	int blk_size = 0, ret = DP_FAILURE;

	if (!pce || !pce_blk_info) {
		pr_err("DPM: %s: pce_rule 0x%px pce_blk_info 0x%px Failed\n",
		       __func__, pce, pce_blk_info);
		return ret;
	}
	blk_info = &pce_blk_info->info;

	ops = gsw_get_swcore_ops(0);
	if (!ops)
		return ret;

	if (!blk_info->portid)
		blk_info->portid = pce->logicalportid;

	if (!blk_info->subif)
		blk_info->subif = pce->subifidgroup;

	if (!pce_blk_info->region)
		pce_blk_info->region = pce->region;

	/* Check whether block is created by someone else, if not create
	 * Block is only for CTP not Global
	 */
	port_info = get_dp_port_info(pce_blk_info->inst, blk_info->portid);

	if (pce_blk_info->region == GSW_PCE_RULE_CTP) {
		blk_size = port_info->blk_size;
		if (!blk_size)
			blk_size = PCE_MAX_BLK_SIZE_CTP;
	} else {
		blk_size = pce_rule.glbl_rules_max;
	}

	DP_DEBUG(DP_DBG_FLAG_PCE,
		 "%s %d %s %s %s %d %s %d %s %d %s %d %s %d\n",
		 "block Size", blk_size,
		 "Region", (pce_blk_info->region == GSW_PCE_RULE_CTP) ?
		 "CTP" : "GLOBAL",
		 "Portid", blk_info->portid,
		 "Subif", blk_info->subif,
		 "subblk_size", blk_info->subblk_size,
		 "Protected", blk_info->subblk_protected,
		 "FirstIndex", blk_info->subblk_firstidx);

	blk_entry = blk_add(pce_blk_info, blk_size);
	if (!blk_entry) {
		pr_err("DPM: %s: No Block present or created\n", __func__);
		goto EXIT;
	}

	/* Check whether sub-block is created, if not create */
	subblk_entry = subblk_add(blk_entry, blk_info);
	if (!subblk_entry) {
		pr_err("DPM: %s: No Sub-Block present or created\n", __func__);
		blk_del(blk_entry);
		goto EXIT;
	}

	if (subblk_entry->subblk_rules_max ==
	    subblk_entry->subblk_rules_used) {
		pr_err("DPM: %s: Sub-block already Full %d/%d\n", __func__,
		       subblk_entry->subblk_rules_used,
		       subblk_entry->subblk_rules_max);
		goto EXIT;
	}

	/* Add PCE rule inside sub-block entry */
	ret = pce_rule_add(pce, blk_entry, subblk_entry, blk_info->prio);
	if (ret < 0) {
		pr_err("DPM: %s: pce_rule_add failed\n", __func__);
		subblk_del(blk_entry, subblk_entry);
		blk_del(blk_entry);
		goto EXIT;
	}

	DP_DEBUG(DP_DBG_FLAG_PCE,
		 "%s %s %s %d %s %d %s %d %s %d %s %s %d %s %d %s %d\n",
		 (ret == DP_FAILURE) ? "fail" : "Success",
		 "Blk",
		 "FirstIndex", blk_entry->blk_firstindex,
		 "RuleMax", blk_entry->blk_rules_max,
		 "RuleUsed", blk_entry->blk_rules_used,
		 "PceIdx", ret,
		 "Sub-Block",
		 "FirstIndex", subblk_entry->subblk_firstindex,
		 "RuleMax", subblk_entry->subblk_rules_max,
		 "RuleUsed", subblk_entry->subblk_rules_used);
EXIT:
	return ret;
}

/* Add the PCE rule inside Sub-Block
 * API will create Block, Sub-Block and Store PCE rule in HW
 * Pass proper portid, subif, subblock id, subblock size, region
 * Returns back the PCE Index where rule is added
 */
int dp_pce_rule_add2(struct dp_pce_blk_info *pce_blk_info,
		    GSW_PCE_rule_t *pce)
{
	int ret = DP_FAILURE;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	DP_LIB_LOCK(&dp_lock);
	ret = dp_pce_rule_add_priv(pce_blk_info, pce);
	DP_LIB_UNLOCK(&dp_lock);

	return ret;
}
EXPORT_SYMBOL(dp_pce_rule_add2);

/* PCE rule delete API to call within DPM without lock */
int dp_pce_rule_del_priv(struct dp_pce_blk_info *pce_blk_info,
			 GSW_PCE_ruleDelete_t *pce)
{
	struct dp_blk_entry *blk_entry;
	struct dp_subblk_entry *subblk_entry;
	struct dp_subblk_info *blk_info;
	int ret = DP_FAILURE;

	if (!pce || !pce_blk_info) {
		pr_err("DPM: %s: pce_rule 0x%px pce_blk_info 0x%px Failed\n",
		       __func__, pce, pce_blk_info);
		return ret;
	}
	blk_info = &pce_blk_info->info;

	if (pce->nIndex > pce_rule.hw_rules_max) {
		pr_err("DPM: %s: PCE Index %d is Invalid\n", __func__,
		       pce->nIndex);
		return ret;
	}

	if (!blk_info->portid)
		blk_info->portid = pce->logicalportid;

	if (!blk_info->subif)
		blk_info->subif = pce->subifidgroup;

	if (!pce_blk_info->region)
		pce_blk_info->region = pce->region;

	DP_DEBUG(DP_DBG_FLAG_PCE,
		 "Region %d Portid %d Subif %d Block ID %d, protected %d\n",
		 pce_blk_info->region, blk_info->portid,
		 blk_info->subif, blk_info->subblk_id,
		 blk_info->subblk_protected);

	blk_entry = get_blk_entry(blk_info, pce_blk_info->region);
	if (!blk_entry) {
		pr_err("DPM: %s: Cannot find the blk_entry for portid=%d subif=%d\n",
		       __func__, blk_info->portid, blk_info->subif);
		goto EXIT;
	}

	subblk_entry = get_subblk_entry(blk_entry, blk_info->subblk_id);
	if (!subblk_entry) {
		pr_err("DPM: %s: Cannot find the subblk_entry for subblk_id=%d\n",
		       __func__, blk_info->subblk_id);
		goto EXIT;
	}

	if (pce_rule_del(pce, blk_entry, subblk_entry)) {
		pr_err("DPM: %s: pce_rule_del failed\n", __func__);
		goto EXIT;
	}

	/* User can explicity change the protection mode during delete
	 * if 1 - this sub-blk wont be removed even after all rules deleted
	 * if 0 - this sub-blk will be removed after all rules removed
	 */
	subblk_entry->subblk_protected = blk_info->subblk_protected;
	subblk_del(blk_entry, subblk_entry);

	if (blk_del(blk_entry)) {
		pr_err("DPM: %s: blk_del failed\n", __func__);
		goto EXIT;
	}

	ret = DP_SUCCESS;
EXIT:
	DP_DEBUG(DP_DBG_FLAG_PCE, "%s\n",
		 (ret == DP_SUCCESS) ? "Success" : "Fail");
	return ret;
}

/* Delete the PCE rule inside Sub-Block
 * Pass Subblk_Id,
 * Pass portid and subifid if it is CTP
 * Pass Index to delete in sub-block specify in pce->pattern.nIndex
 */
int dp_pce_rule_del2(struct dp_pce_blk_info *pce_blk_info,
		    GSW_PCE_ruleDelete_t *pce)
{
	int ret = DP_FAILURE;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	DP_LIB_LOCK(&dp_lock);
	ret = dp_pce_rule_del_priv(pce_blk_info, pce);
	DP_LIB_UNLOCK(&dp_lock);

	return ret;
}
EXPORT_SYMBOL(dp_pce_rule_del2);

/* PCE rule get API to call within DPM without lock */
int dp_pce_rule_get_priv(struct dp_pce_blk_info *pce_blk_info,
			 GSW_PCE_rule_t *pce)
{
	struct dp_blk_entry *blk_entry;
	struct dp_subblk_entry *subblk_entry;
	struct dp_subblk_info *blk_info;
	int ret = DP_FAILURE;

	if (!pce || !pce_blk_info) {
		pr_err("DPM: %s: Pce Rule Info to delete is Null\n", __func__);
		return ret;
	}
	blk_info = &pce_blk_info->info;

	if (pce->pattern.nIndex > pce_rule.hw_rules_max) {
		pr_err("DPM: %s: PCE Index %d is Invalid\n", __func__,
		       pce->pattern.nIndex);
		return ret;
	}

	if (!blk_info->portid)
		blk_info->portid = pce->logicalportid;

	if (!blk_info->subif)
		blk_info->subif = pce->subifidgroup;

	if (!pce_blk_info->region)
		pce_blk_info->region = pce->region;

	blk_entry = get_blk_entry(blk_info, pce_blk_info->region);
	if (!blk_entry) {
		pr_err("DPM: %s: Cannot find the blk_entry for portid=%d subif=%d\n",
		       __func__, blk_info->portid, blk_info->subif);
		goto EXIT;
	}

	DP_DEBUG(DP_DBG_FLAG_PCE,
		 "%s %d %s %s %s %d %s %d %s %d\n",
		 "Pce Idx", pce->pattern.nIndex,
		 "Region",
		 (blk_entry->info.region == GSW_PCE_RULE_CTP) ?
		 "CTP" : "GLOBAL",
		 "Portid", blk_entry->info.info.portid,
		 "Subifid", blk_entry->info.info.subif,
		 "Block ID", blk_info->subblk_id);

	subblk_entry = get_subblk_entry(blk_entry, blk_info->subblk_id);
	if (!subblk_entry) {
		pr_err("DPM: %s: Cannot find the subblk_entry for subblk_id=%d\n",
		       __func__, blk_info->subblk_id);
		goto EXIT;
	}

	if (pce_rule_get(pce, blk_entry, subblk_entry)) {
		pr_err("DPM: %s: pce_rule_get failed\n", __func__);
		goto EXIT;
	}

	ret = DP_SUCCESS;
	DP_DEBUG(DP_DBG_FLAG_PCE,
		 "%s %s %d %s %s %s %d %s %d %s %s %d %s %d %s %d\n",
		 (ret == DP_SUCCESS) ? "Success" : "Fail",
		 "Pce Idx", pce->pattern.nIndex,
		 "Region",
		 (blk_entry->info.region == GSW_PCE_RULE_CTP) ?
		 "CTP" : "GLOBAL",
		 "Portid", blk_entry->info.info.portid,
		 "Subifid", blk_entry->info.info.subif,
		 "Sub-Blk",
		 "FirstIdx", subblk_entry->subblk_firstindex,
		 "RuleMax", subblk_entry->subblk_rules_max,
		 "RuleUsed", subblk_entry->subblk_rules_used);
EXIT:
	return ret;
}

/* Get the PCE rule inside Sub-Block
 * Pass Subblk_Id,
 * Pass portid and subifid if it is CTP
 * Pass Index to get in sub-block specify in pce->pattern.nIndex
 */
int dp_pce_rule_get(struct dp_pce_blk_info *pce_blk_info,
		    GSW_PCE_rule_t *pce)
{
	int ret = DP_FAILURE;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	DP_LIB_LOCK(&dp_lock);
	ret = dp_pce_rule_get_priv(pce_blk_info, pce);
	DP_LIB_UNLOCK(&dp_lock);

	return ret;
}
EXPORT_SYMBOL(dp_pce_rule_get);

/* PCE rule update API to call within DPM without lock */
int dp_pce_rule_update_priv(struct dp_pce_blk_info *pce_blk_info,
			    GSW_PCE_rule_t *pce, enum DP_PCE_RULE_UPDATE update)
{
	struct dp_blk_entry *blk_entry;
	struct dp_subblk_entry *subblk_entry;
	struct dp_subblk_info *blk_info;
	int ret = DP_FAILURE;

	if (!pce || !pce_blk_info) {
		pr_err("DPM: %s: Pce Rule Info to delete is Null\n", __func__);
		return ret;
	}
	blk_info = &pce_blk_info->info;

	if (pce->pattern.nIndex > pce_rule.hw_rules_max) {
		pr_err("DPM: %s: PCE Index %d is Invalid\n", __func__,
		       pce->pattern.nIndex);
		return ret;
	}

	if (!blk_info->portid)
		blk_info->portid = pce->logicalportid;

	if (!blk_info->subif)
		blk_info->subif = pce->subifidgroup;

	if (!pce_blk_info->region)
		pce_blk_info->region = pce->region;

	blk_entry = get_blk_entry(blk_info, pce_blk_info->region);
	if (!blk_entry) {
		pr_err("DPM: %s: Cannot find the blk_entry for portid=%d subif=%d\n",
		       __func__, blk_info->portid, blk_info->subif);
		goto EXIT;
	}

	DP_DEBUG(DP_DBG_FLAG_PCE,
		 "%s %d %s %s %s %d %s %d %s %d\n",
		 "Pce Idx", pce->pattern.nIndex,
		 "Region",
		 (blk_entry->info.region == GSW_PCE_RULE_CTP) ?
		 "CTP" : "GLOBAL",
		 "Portid", blk_entry->info.info.portid,
		 "Subifid", blk_entry->info.info.subif,
		 "Block ID", blk_info->subblk_id);

	subblk_entry = get_subblk_entry(blk_entry, blk_info->subblk_id);
	if (!subblk_entry) {
		pr_err("DPM: %s: Cannot find the subblk_entry for subblk_id=%d\n",
		       __func__, blk_info->subblk_id);
		goto EXIT;
	}

	if (pce_rule_update(pce, blk_entry, subblk_entry, blk_info->prio, update)) {
		pr_err("DPM: %s: pce_rule_get failed\n", __func__);
		goto EXIT;
	}

	ret = DP_SUCCESS;
	DP_DEBUG(DP_DBG_FLAG_PCE,
		 "%s %s %d %s %s %s %d %s %d %s %s %d %s %d %s %d\n",
		 (ret == DP_SUCCESS) ? "Success" : "Fail",
		 "Pce Idx", pce->pattern.nIndex,
		 "Region",
		 (blk_entry->info.region == GSW_PCE_RULE_CTP) ?
		 "CTP" : "GLOBAL",
		 "Portid", blk_entry->info.info.portid,
		 "Subifid", blk_entry->info.info.subif,
		 "Sub-Blk",
		 "FirstIdx", subblk_entry->subblk_firstindex,
		 "RuleMax", subblk_entry->subblk_rules_max,
		 "RuleUsed", subblk_entry->subblk_rules_used);
EXIT:
	return ret;
}

/* Get the PCE rule inside Sub-Block
 * Pass Subblk_Id,
 * Pass portid and subifid if it is CTP
 * Pass Index to get in sub-block specify in pce->pattern.nIndex
 */
int dp_pce_rule_update2(struct dp_pce_blk_info *pce_blk_info,
			GSW_PCE_rule_t *pce, enum DP_PCE_RULE_UPDATE update)
{
	int ret = DP_FAILURE;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	DP_LIB_LOCK(&dp_lock);
	ret = dp_pce_rule_update_priv(pce_blk_info, pce, update);
	DP_LIB_UNLOCK(&dp_lock);

	return ret;
}
EXPORT_SYMBOL(dp_pce_rule_update2);

/* Initialize the PCE rules table
 * Get the Global Rules firstIndex and Global Rules Max from GSWIP
 */
int dp_init_pce(void)
{
	struct core_ops *ops;
	GSW_register_t reg;

	ops = gsw_get_swcore_ops(0);
	if (!ops)
		return -1;

	reg.nRegAddr = PCE_TFCR_NUM;
	ops->gsw_common_ops.RegisterGet(ops, &reg);
	pce_rule.glbl_rules_max = reg.nData * 4;

	reg.nRegAddr = PCE_TFCR_START_IDX;
	ops->gsw_common_ops.RegisterGet(ops, &reg);
	pce_rule.glbl_firstindex = reg.nData;

	pce_rule.hw_rules_max = PCE_MAX_ENTRIES;

	INIT_LIST_HEAD(&pce_rule.blk_list);

	return 0;
}

int proc_pce_dump(struct seq_file *s, int pos)
{
	struct dp_blk_entry *blk_entry;
	struct dp_subblk_entry *subblk_entry;
	int subblk = 0, i = 0, blk = 0;

	dp_sputs(s, "\n");
	dp_sprintf(s, "%-10s%-10s%-10d\n", "PCE-Rule",
		   "HW Max", pce_rule.hw_rules_max);
	dp_sputs(s, "\n");
	dp_sprintf(s, "%-10s%-10d%-10d\n",
		   "Global", pce_rule.glbl_firstindex,
		   pce_rule.glbl_rules_max);
	dp_sputs(s, "\n");

	blk = 0;
	list_for_each_entry(blk_entry, &pce_rule.blk_list, list) {
		if (blk == 0) {
			dp_sprintf(s,
				   "%-10s%-10s%-15s%-10s%-10s%-10s%-10s%-10s%-10s\n",
				   "Block", "", "Block Name", "Region", "PortID",
				   "SubIfId", "FirstIdx", "RuleMax",
				   "RuleAllocated");

			for (i = 0; i < 100; i++)
				dp_sputs(s, "=");
			dp_sputs(s, "\n");
			blk = 1;
		}
		dp_sprintf(s,
			   "%-10s%-10s%-15s%-10s%-10d%-10d%-10d%-10d%-10d\n",
			   "", "",
			   blk_entry->blk_name,
			   (blk_entry->info.region == GSW_PCE_RULE_CTP) ?
			   "CTP" : "GLOBAL",
			   blk_entry->info.info.portid,
			   blk_entry->info.info.subif,
			   blk_entry->blk_firstindex,
			   blk_entry->blk_rules_max,
			   blk_entry->blk_rules_used);
	}

	dp_sputs(s, "\n");
	subblk = 0;
	list_for_each_entry(blk_entry, &pce_rule.blk_list, list) {
		dp_sputs(s, "\n");
		if (subblk == 0) {
			dp_sprintf(s,
				   "%-10s%-10s%-10s%-10s%-20s%-10s%-10s%-10s%-10s%-10s\n",
				   "Sub-Block", "", "Region", "Blk Name",
				   "Sub-Blk Name", "ID",
				   "FirstIdx", "RuleMax", "RuleUsed",
				   "Protected");

			subblk = 1;
			for (i = 0; i < 110; i++)
				dp_sputs(s, "=");
			dp_sputs(s, "\n");
		}
		list_for_each_entry(subblk_entry, &blk_entry->subblk_list, list) {
			dp_sprintf(s,
				   "%-10s%-10s%-10s%-10s%-20s%-10d%-10d%-10d%-10d%-10d\n",
				   "", "",
				   (blk_entry->info.region == GSW_PCE_RULE_CTP) ?
				   "CTP" : "GLOBAL",
				   blk_entry->blk_name,
				   subblk_entry->subblk_name,
				   subblk_entry->subblk_id,
				   subblk_entry->subblk_firstindex,
				   subblk_entry->subblk_rules_max,
				   subblk_entry->subblk_rules_used,
				   subblk_entry->subblk_protected);
		}
	}
	dp_sputs(s, "\n");
	return -1;
}

enum _pce_ops {
	PCE_ADD = 0,
	PCE_DEL,
	PCE_GET,
	PCE_BLK_CRE,
	PCE_BLK_DEL,
};

#define PATH_PCE "/sys/kernel/debug/dp/pce"

struct pce_write {
	char str[100];
	char *param_list[16];
	struct dp_pce_blk_info pce_blk_info;
	GSW_PCE_ruleDelete_t pce_del;
	char subblk_name[32];
};
ssize_t proc_pce_write(struct file *file, const char *buf, size_t count,
		       loff_t *ppos)
{
	struct pce_write *p = NULL;
	int num, i = 0;
	size_t len;
	int portid = 0, subifid = 0, region = 0, pceidx = 0, blkid = 0;
	int subblkize = 0, protected = 0, subblk_firstidx = 0;
	int op = -1;
	struct dp_subblk_info *blk_info;
	GSW_PCE_rule_t *pce;
	struct dp_blk_entry *blk_entry;
	struct dp_subblk_entry *subblk_entry;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;
	p = dp_kzalloc(sizeof(*p), GFP_ATOMIC);
	if (!p)
		return count;
	len = (sizeof(p->str) > count) ? count : sizeof(p->str) - 1;
	len -= dp_copy_from_user(p->str, buf, len);
	p->str[len] = 0;

	num = dp_split_buffer(p->str, p->param_list, ARRAY_SIZE(p->param_list));
	i = 0;

	while (1) {
		if (num <= 1 || (!dp_strncmpi(p->param_list[i], "help",
					      strlen("help")))) {
			goto help;
		} else if (!dp_strncmpi(p->param_list[i], "blk_cre",
					strlen("blk_cre"))) {
			op = PCE_BLK_CRE;
			i++;
		} else if (!dp_strncmpi(p->param_list[i], "blk_del",
					strlen("blk_del"))) {
			op = PCE_BLK_DEL;
			i++;
		} else if (!dp_strncmpi(p->param_list[i], "add",
					strlen("add"))) {
			op = PCE_ADD;
			i++;
		} else if (!dp_strncmpi(p->param_list[i], "del",
					strlen("del"))) {
			op = PCE_DEL;
			i++;
		} else if (!dp_strncmpi(p->param_list[i], "get",
					strlen("get"))) {
			op = PCE_GET;
			i++;
		} else if (!dp_strncmpi(p->param_list[i], "portid",
					strlen("portid"))) {
			if (p->param_list[i + 1])
				portid = dp_atoi(p->param_list[i + 1]);
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "subifid",
					strlen("subifid"))) {
			if (p->param_list[i + 1])
				subifid = dp_atoi(p->param_list[i + 1]);
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "region",
					strlen("region"))) {
			if (p->param_list[i + 1])
				region = dp_atoi(p->param_list[i + 1]);
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "pceidx",
					strlen("pceidx"))) {
			if (p->param_list[i + 1])
				pceidx = dp_atoi(p->param_list[i + 1]);
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "subblksize",
					strlen("subblksize"))) {
			if (p->param_list[i + 1])
				subblkize = dp_atoi(p->param_list[i + 1]);
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "blkid",
					strlen("blkid"))) {
			if (p->param_list[i + 1])
				blkid = dp_atoi(p->param_list[i + 1]);
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "protected",
					strlen("protected"))) {
			if (p->param_list[i + 1])
				protected = dp_atoi(p->param_list[i + 1]);
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "subblk_name",
					strlen("subblk_name"))) {
			dp_strlcpy(p->subblk_name, p->param_list[i + 1],
				   sizeof(p->subblk_name));
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "subblk_firstidx",
					strlen("subblk_firstidx"))) {
			if (p->param_list[i + 1])
				subblk_firstidx = dp_atoi(p->param_list[i + 1]);
			i += 2;
		} else {
			break;
		}

		if (i >= num)
			break;
	}

	pce = dp_kzalloc(sizeof(*pce), GFP_ATOMIC);
	if (!pce) {
		kfree(p);
		return -1;
	}
	blk_info = &p->pce_blk_info.info;

	blk_info->subblk_size = subblkize;
	blk_info->portid = portid;
	blk_info->subif = subifid;

	dp_strlcpy(blk_info->subblk_name, p->subblk_name,
		   sizeof(blk_info->subblk_name));

	pce->logicalportid = portid;
	pce->subifidgroup = subifid;
	pce->region = region;

	p->pce_del.logicalportid = portid;
	p->pce_del.subifidgroup = subifid;
	p->pce_del.region = region;

	blk_entry = get_blk_entry(blk_info, region);
	if (blk_entry) {
		subblk_entry = get_subblk_entry(blk_entry, blkid);
		p->pce_blk_info.region = blk_entry->info.region;

		if (subblk_entry) {
			DP_DEBUG(DP_DBG_FLAG_PCE,
				 "%s %s %s %d %s %d %s %d\n",
				 "Region",
				 (blk_entry->info.region == GSW_PCE_RULE_CTP) ?
				 "CTP" : "GLOBAL",
				 "Portid", blk_entry->info.info.portid,
				 "Subifid", blk_entry->info.info.subif,
				 "Block ID", subblk_entry->subblk_id);
		}

		if (subblk_entry)
			dp_memcpy(blk_info, &blk_entry->info.info,
				  sizeof(struct dp_subblk_info));
	}

	blk_info->subblk_id = blkid;
	blk_info->subblk_protected = protected;
	blk_info->subblk_firstidx = subblk_firstidx;

	DP_DEBUG(DP_DBG_FLAG_PCE,
		 "%s %s %s %d %s %d %s %d %s %d %s %d\n",
		 "Region",
		 (region == GSW_PCE_RULE_CTP) ? "CTP" : "GLOBAL",
		 "Portid", blk_info->portid,
		 "Subifid", blk_info->subif,
		 "subblk_protected", blk_info->subblk_protected,
		 "subblk_firstidx", blk_info->subblk_firstidx,
		 "Block ID", blk_info->subblk_id);

	switch (op) {
	case PCE_BLK_CRE:
		dp_pce_blk_create(&p->pce_blk_info);
		break;
	case PCE_ADD:
		dp_pce_rule_add(&p->pce_blk_info, pce);
		break;
	case PCE_DEL:
		p->pce_del.nIndex = pceidx;
		dp_pce_rule_del(&p->pce_blk_info, &p->pce_del);
		break;
	case PCE_GET:
		pce->pattern.nIndex = pceidx;
		dp_pce_rule_get(&p->pce_blk_info, pce);
		break;
	case PCE_BLK_DEL:
		dp_pce_blk_del(&p->pce_blk_info);
		break;
	default:
		break;
	};

	kfree(pce);
	kfree(p);
	return count;
help:
	pr_info("usage:\n");
	pr_info("  echo blk_cre portid x subifid y region z blkid a");
	pr_info(" subblksize c protected x subblk_name X > %s\n", PATH_PCE);
	pr_info("  echo blk_del portid x subifid y region z blkid a");
	pr_info(" subblksize c protected x > %s\n", PATH_PCE);
	pr_info("  echo add portid x subifid y region z pceidx b blkid a");
	pr_info(" subblksize b protected x subblk_name X > %s\n", PATH_PCE);
	pr_info("  echo del portid x subifid y region z pceidx b blkid a");
	pr_info(" subblksize b protected x > %s\n", PATH_PCE);
	pr_info("  echo get portid x subifid y region z pceidx b blkid a");
	pr_info(" subblksize b > %s\n", PATH_PCE);
	kfree(p);
	return count;
}

static int dp_pce_rmon_get_priv(int *id)
{
	int i;

	for (i = 1; i < PCE_RMON_SIZE_MAX; i++) {
		if (test_bit(i, pce_rule._rmon_used))
			continue;
		DP_DEBUG(DP_DBG_FLAG_PCE, "reserved rmon_id %d\n", i);
		set_bit(i, pce_rule._rmon_used);
		*id = i;
		return DP_SUCCESS;
	}

	DP_DEBUG(DP_DBG_FLAG_PCE, "free rmon_id not found!\n");
	return DP_FAILURE;
}

/* Reserve RMON counter id. */
int dp_pce_rmon_get2(int *id)
{
	int ret = DP_FAILURE;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	DP_LIB_LOCK(&dp_lock);
	ret = dp_pce_rmon_get_priv(id);
	DP_LIB_UNLOCK(&dp_lock);

	return ret;
}
EXPORT_SYMBOL(dp_pce_rmon_get2);

static int dp_pce_rmon_free_priv(int id)
{
	if (!test_bit(id, pce_rule._rmon_used)) {
		DP_DEBUG(DP_DBG_FLAG_PCE, "rmon_id %d not found!\n", id);
		return DP_FAILURE;
	}

	dp_clear_bit(id, pce_rule._rmon_used);
	DP_DEBUG(DP_DBG_FLAG_PCE, "freed rmon_id %d\n", id);
	return DP_SUCCESS;
}

/* Release RMON counter id. */
int dp_pce_rmon_free2(int id)
{
	int ret = DP_FAILURE;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	DP_LIB_LOCK(&dp_lock);
	ret = dp_pce_rmon_free_priv(id);
	DP_LIB_UNLOCK(&dp_lock);

	return ret;
}
EXPORT_SYMBOL(dp_pce_rmon_free2);

