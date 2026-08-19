// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2021 - 2024 MaxLinear, Inc.
 *
 *****************************************************************************/
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/version.h>
#include <linux/jiffies.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/mutex.h>
#include <net/datapath_api.h>
#include <net/datapath_api_qos.h>
#include <net/datapath_proc_api.h>
#include <net/switch_api/lantiq_gsw_flow.h>
#include "dp_oam.h"
#include "dp_oam_transceive.h"

#define DRV_NAME	"mod_dp_oam"
static DEFINE_MUTEX(storage_lock);

#define PCE_SUBBLK_SIZE_GLB 8
#define PCE_SUBBLK_SIZE_CTP 16
LIST_HEAD(dp_oam_storage);

struct pce_rule_storage {
	/** Logical Port Id. The valid range is hardware dependent */
	u32 logicalportid;
	/** Sub interface ID group,
	 *  The valid range is hardware/protocol dependent
	 */
	u32 subifidgroup;
	/** PCE TABLE Region */
	GSW_PCE_RuleRegion_t	region;
	/** Sub-Block Type ID */
	int	subblk_id;
	/** Rule arguments */
	struct dp_oam_args args;
	struct list_head list;
};

/* See 802.1ag, ITU-T G.8013/Y.1731 for CFM header */
#define GSW_VERSION_MASK	0x1f00
#define GSW_MDL_VERSION_MASK	0xff00

static void pce_default_set(GSW_PCE_rule_t *pce_rule)
{
	pce_rule->pattern.bEnable = 1;
	pr_debug("%s: bEnable = %d\n", __func__,
		 pce_rule->pattern.bEnable);
}

#define ETH_P_CFM 0x8902
static void pce_ethtype_cfm(GSW_PCE_rule_t *pce_rule)
{
	pce_rule->pattern.bEtherTypeEnable = 1;
	pr_debug("%s: bEtherTypeEnable = %d\n", __func__,
		 pce_rule->pattern.bEtherTypeEnable);

	pce_rule->pattern.nEtherType = ntohs(ETH_P_CFM);
	pr_debug("%s: nEtherType = %#x\n", __func__,
		 pce_rule->pattern.nEtherType);
}

static void pce_portmap_set(GSW_PCE_rule_t *pce_rule,
			    bool drop)
{
	pce_rule->action.ePortMapAction = drop ?
		GSW_PCE_ACTION_PORTMAP_DISCARD :
		GSW_PCE_ACTION_PORTMAP_REGULAR;
	pr_debug("%s: bEnable = %x\n", __func__,
		 pce_rule->action.ePortMapAction);
}

/* Special tags are deactivated on LAN ports if CONFIG_MXL_ETHSW_NOSPTAG is set.
 * The match on subifid should not be used in this case as it cannot match.
 */
static void pce_netdev_set_subif(GSW_PCE_rule_t *pce_rule,
				 dp_subif_t *dp_subif)
{
	if (!IS_ENABLED(CONFIG_MXL_ETHSW_NOSPTAG) ||
	    (dp_subif->alloc_flag & DP_F_FAST_ETH_LAN))
		return;

	pce_rule->pattern.eSubIfIdType = GSW_PCE_SUBIFID_TYPE_GROUP;
	pce_rule->pattern.bSubIfIdEnable = 1;
	pce_rule->pattern.nSubIfId = dp_subif->subif_groupid;
}

static int pce_netdev_set(GSW_PCE_rule_t *pce_rule, int ifindex,
			  bool global, int *bp)
{
	struct net_device *dev;
	dp_subif_t dp_subif = {0};
	int ret;

	dev = dev_get_by_index(&init_net, ifindex);
	if (!dev) {
		netdev_err(dev, "%s: dev_get_by_index failed\n",
			   __func__);
		return -ENODEV;
	}

	ret = dp_get_netif_subifid(dev, NULL, NULL, NULL, &dp_subif, 0);
	dev_put(dev);
	if (ret != DP_SUCCESS) {
		pr_err("%s: dp_get_netif_subifid failed: %d\n",
		       __func__, ret);
		return ret;
	}

	pce_rule->logicalportid = dp_subif.port_id;
	pce_rule->pattern.bPortIdEnable = 1;
	pce_rule->pattern.nPortId = dp_subif.port_id;

	pr_debug("%s: logicalportid = %d\n", __func__,
		 pce_rule->logicalportid);
	pr_debug("%s: bPortIdEnable = %d\n", __func__,
		 pce_rule->pattern.bPortIdEnable);
	pr_debug("%s: nPortId = %d\n", __func__,
		 pce_rule->pattern.nPortId);

	/* Configure on port */
	if (global)
		return 0;

	if (dp_subif.flag_pmapper) {
		pce_rule->pattern.eSubIfIdType =
			GSW_PCE_SUBIFID_TYPE_BRIDGEPORT;
		pce_rule->subifidgroup = dp_subif.subif_groupid;
		pce_rule->pattern.bSubIfIdEnable = 1;
		pce_rule->pattern.nSubIfId = dp_subif.bport;
		*bp = dp_subif.bport;
	} else {
		pce_rule->subifidgroup = dp_subif.subif_groupid;
		pce_netdev_set_subif(pce_rule, &dp_subif);
	}

	pr_debug("%s: subifidgroup = %d\n", __func__,
		 pce_rule->subifidgroup);
	pr_debug("%s: bSubIfIdEnable = %d\n", __func__,
		 pce_rule->pattern.bSubIfIdEnable);
	pr_debug("%s: nSubIfId = %d\n", __func__,
		 pce_rule->pattern.nSubIfId);

	return 0;
}

static void pce_level_set(GSW_PCE_rule_t *pce_rule,
			  u8 level)
{
	pce_rule->pattern.bAppDataMSB_Enable = 1;
	pr_debug("%s: bAppDataMSB_Enable = %d\n", __func__,
		 pce_rule->pattern.bAppDataMSB_Enable);

	pce_rule->pattern.nAppDataMSB |= ((level & 0x7) << 13);
	pr_debug("%s: nAppDataMSB = %#x\n", __func__,
		 pce_rule->pattern.nAppDataMSB);
}

static void pce_level_bitmask_set(GSW_PCE_rule_t *pce_rule)
{
	pce_rule->pattern.bAppDataMSB_Enable = 1;
	pr_debug("%s: bAppDataMSB_Enable = %d\n", __func__,
		 pce_rule->pattern.bAppDataMSB_Enable);

	pce_rule->pattern.bAppMaskRangeMSB_Select = 2;
	pr_debug("%s: bAppMaskRangeMSB_Select = %d\n", __func__,
		 pce_rule->pattern.bAppMaskRangeMSB_Select);

	pce_rule->pattern.nAppMaskRangeMSB = 0x1FFF;
	pr_debug("%s: nAppMaskRangeMSB = %#x\n", __func__,
		 pce_rule->pattern.nAppMaskRangeMSB);
}

static void pce_level_range_set(GSW_PCE_rule_t *pce_rule,
				u8 level)
{
	if (level < 1 || level > 8)
		return;

	pce_rule->pattern.bAppDataMSB_Enable = 1;
	pr_debug("%s: bAppDataMSB_Enable = %d\n", __func__,
		 pce_rule->pattern.bAppDataMSB_Enable);

	pce_rule->pattern.bAppMaskRangeMSB_Select = 1;
	pr_debug("%s: bAppMaskRangeMSB_Select = %d\n", __func__,
		 pce_rule->pattern.bAppMaskRangeMSB_Select);

	pce_rule->pattern.nAppMaskRangeMSB = 0x1FFF | ((level - 1) << 13);
	pr_debug("%s: nAppMaskRangeMSB = %#x\n", __func__,
		 pce_rule->pattern.nAppMaskRangeMSB);
}

static void pce_opcode_set(GSW_PCE_rule_t *pce_rule,
			   int opcode)
{
	pce_rule->pattern.bAppDataMSB_Enable = 1;
	pr_debug("%s: bAppDataMSB_Enable = %d\n", __func__,
		 pce_rule->pattern.bAppDataMSB_Enable);

	pce_rule->pattern.nAppDataMSB |= (opcode & 0xFF);
	pr_debug("%s: nAppDataMSB = %#x\n", __func__,
		 pce_rule->pattern.nAppDataMSB);
}

static void pce_opcode_mask_set(GSW_PCE_rule_t *pce_rule, u16 mask)
{
	pce_rule->pattern.bAppMaskRangeMSB_Select = 2;
	pr_debug("%s: bAppMaskRangeMSB_Select = %d\n", __func__,
		 pce_rule->pattern.bAppMaskRangeMSB_Select);

	pce_rule->pattern.nAppMaskRangeMSB = mask;
	pr_debug("%s: nAppMaskRangeMSB = %#x\n", __func__,
		 pce_rule->pattern.nAppMaskRangeMSB);
}

static void pce_oam_set(GSW_PCE_rule_t *pce_rule, bool en)
{
	pce_rule->action.bOamEnable = en ? 1 : 0;
	pr_debug("%s: bOamEnable = %d\n", __func__,
		 pce_rule->action.bOamEnable);
}

static void pce_extract_set(GSW_PCE_rule_t *pce_rule,
			    enum dp_oam_ext_pts ext_pt)
{
	pce_rule->action.bExtractEnable = 1;
	pr_debug("%s: bExtractEnable = %d\n", __func__,
		 pce_rule->action.bExtractEnable);

	pce_rule->action.nInsExtPoint = ext_pt;
	pr_debug("%s: nInsExtPoint = %#x\n", __func__,
		 pce_rule->action.nInsExtPoint);

	pce_oam_set(pce_rule, true);
}

static void vlan_id_set(GSW_PCE_rule_t *pce_rule,
			u16 vlan_id)
{
	pce_rule->pattern.bSLAN_Vid = 1;
	pr_debug("%s: bSLAN_Vid = %#x\n", __func__,
		 pce_rule->pattern.bSLAN_Vid);
	pce_rule->pattern.nSLAN_Vid = vlan_id;
	pr_debug("%s: nSLAN_Vid = %#x\n", __func__,
		 pce_rule->pattern.nSLAN_Vid);
}

static void counter_id_set(GSW_PCE_rule_t *pce_rule,
			   u32 counter_id)
{
	pce_rule->action.bRMON_Action = 1;
	pr_debug("%s: bRMON_Action = %d\n", __func__,
		 pce_rule->action.bRMON_Action);
	pce_rule->action.nRMON_Id = counter_id;
	pr_debug("%s: nRMON_Id = %u\n", __func__,
		 pce_rule->action.nRMON_Id);
}

static void rmon_id_set(GSW_PCE_rule_t *pce_rule,
			u32 rmon_id,
			u32 offset,
			bool rmon_tx,
			bool rmon_bypass)
{
	pce_rule->action.nOamFlowId = rmon_id;
	pr_debug("%s: nOamFlowId = %u\n", __func__,
		 pce_rule->action.nOamFlowId);
	pce_rule->action.bTxFlowCnt = rmon_tx;
	pr_debug("%s: bTxFlowCnt = %d\n", __func__,
		 pce_rule->action.bTxFlowCnt);
	/* Get trasmit packet counter after QoS queue */
	pce_rule->action.bPceBypassPath = rmon_bypass;
	pr_debug("%s: bPceBypassPath = %d\n", __func__,
		 pce_rule->action.bPceBypassPath);
	pce_rule->action.nPktUpdateOffset = offset;
	pr_debug("%s: nPktUpdateOffset = %u\n", __func__,
		 pce_rule->action.nPktUpdateOffset);
	pce_oam_set(pce_rule, true);
}

static void timestamp_set(GSW_PCE_rule_t *pce_rule,
			  enum dp_oam_time_format format,
			  long long compensation,
			  u32 offset)
{
	pce_rule->action.eTimestampAction = GSW_PCE_ACTION_TIMESTAMP_STORED;
	pr_debug("%s: eTimestampAction = %u\n", __func__,
		 pce_rule->action.eTimestampAction);
	pce_rule->action.nTimeComp = compensation;
	pr_debug("%s: nTimeComp = %lld\n", __func__,
		 pce_rule->action.nTimeComp);
	pce_rule->action.eTimeFormat = format;
	pr_debug("%s: eTimeFormat = %d\n", __func__,
		 pce_rule->action.eTimeFormat);
	pce_rule->action.nPktUpdateOffset = offset;
	pr_debug("%s: nPktUpdateOffset = %u\n", __func__,
		 pce_rule->action.nPktUpdateOffset);
	pce_oam_set(pce_rule, true);
}

static int pce_reinsert_disable(GSW_PCE_rule_t *pce_rule, int ifindex)
{
	struct net_device *dev;
	dp_subif_t subif;
	int ret;

	dev = dev_get_by_index(&init_net, ifindex);
	if (!dev) {
		netdev_err(dev, "%s: dev_get_by_index failed\n",
			   __func__);
		return -ENODEV;
	}

	ret = dp_get_netif_subifid(dev, NULL, NULL, NULL, &subif, 0);
	dev_put(dev);
	if (ret < 0) {
		netdev_dbg(dev, "%s: subif idx get failed\n", __func__);
		return ret;
	}

	pce_rule->pattern.bInsertionFlag_Enable = 1;
	if (IS_ENABLED(CONFIG_MXL_ETHSW_NOSPTAG) &&
	    (subif.alloc_flag & DP_F_FAST_ETH_LAN))
		pce_rule->pattern.nInsertionFlag = GSW_PCE_INSERT_MATCH_NO_STAG;
	else
		pce_rule->pattern.nInsertionFlag =
			GSW_PCE_INSERT_MATCH_NO_INSERT_FLAG;

	return 0;
}

static void pce_counter_exclude_level(GSW_PCE_rule_t *pce_rule, u8 level)
{
	pce_rule->pattern.bAppDataMSB_Enable = 1;
	pr_debug("%s: bAppDataMSB_Enable = %d\n", __func__,
		 pce_rule->pattern.bAppDataMSB_Enable);
	pce_rule->pattern.bAppMSB_Exclude = 1;
	pr_debug("%s: bAppMSB_Exclude = %d\n", __func__,
		 pce_rule->pattern.bAppMSB_Exclude);
	pce_rule->pattern.bAppMSB_Exclude = 1;
	pr_debug("%s: bAppMSB_Exclude = %d\n", __func__,
		 pce_rule->pattern.bAppMSB_Exclude);
	pce_rule->pattern.bAppMaskRangeMSB_Select = 2;
	pr_debug("%s: bAppMaskRangeMSB_Select = %d\n", __func__,
		 pce_rule->pattern.bAppMaskRangeMSB_Select);
	pce_rule->pattern.nAppMaskRangeMSB = 0x1fff;
	pr_debug("%s: nAppMaskRangeMSB = %#x\n", __func__,
		 pce_rule->pattern.nAppMaskRangeMSB);
	pce_rule->pattern.nAppDataMSB |= ((level & 0x7) << 13);
	pr_debug("%s: nAppDataMSB = %#x\n", __func__,
		 pce_rule->pattern.nAppDataMSB);
}

static void untagged_set(GSW_PCE_rule_t *pce_rule)
{
	pce_rule->pattern.bSLAN_Vid = 1;
	pce_rule->pattern.nSLAN_Vid = 0;
	pce_rule->pattern.bSVidRange_Select = 1;
	pce_rule->pattern.nOuterVidRange = 4095;
	pce_rule->pattern.bOuterVid_Original = 0;
	pce_rule->pattern.bSLANVid_Exclude = 1;
	pr_debug("%s: match tagged nSLAN_Vid = %u nOuterVidRange = %u\n",
		 __func__,
		 pce_rule->pattern.nSLAN_Vid,
		 pce_rule->pattern.nOuterVidRange);
}

static int storage_add(GSW_PCE_rule_t *pce_rule,
		       struct dp_pce_blk_info *pce_blk_info,
		       struct dp_oam_args *args)
{
	struct pce_rule_storage *pce_rule_storage;

	pce_rule_storage = kzalloc(sizeof(*pce_rule_storage), GFP_KERNEL);
	if (!pce_rule_storage)
		return -ENOMEM;

	pce_rule_storage->logicalportid = pce_rule->logicalportid;
	pce_rule_storage->subifidgroup = pce_rule->subifidgroup;
	pce_rule_storage->region = pce_blk_info->region;
	pce_rule_storage->subblk_id = pce_blk_info->info.subblk_id;
	memcpy(&pce_rule_storage->args, args, sizeof(*args));

	mutex_lock(&storage_lock);
	list_add_tail(&pce_rule_storage->list, &dp_oam_storage);
	mutex_unlock(&storage_lock);

	return 0;
}

static int pce_rule_add(GSW_PCE_rule_t *pce_rule,
			struct dp_oam_args *args,
			int bp)
{
	struct dp_pce_blk_info *pce_blk_info;
	struct pce_rule_storage *p;
	int index;
	int ret;

	pce_blk_info = kzalloc(sizeof(*pce_blk_info), GFP_ATOMIC);
	if (!pce_blk_info) {
		kfree(pce_rule);
		return -ENOMEM;
	}

	if (args->global) {
		pce_rule->region = GSW_PCE_RULE_COMMMON;
		pce_blk_info->info.subblk_size = PCE_SUBBLK_SIZE_GLB;
	} else {
		pce_rule->region = GSW_PCE_RULE_CTP;
		pce_blk_info->info.subblk_size = PCE_SUBBLK_SIZE_CTP;
	}
	pce_blk_info->region = pce_rule->region;
	pce_blk_info->info.portid = pce_rule->logicalportid;
	pce_blk_info->info.subif = pce_rule->subifidgroup;
	pce_blk_info->info.prio = args->prio;
	pce_blk_info->info.bp = bp;

	/* Find if subblock id to be used exists */
	list_for_each_entry(p, &dp_oam_storage, list) {
		if (p->region == GSW_PCE_RULE_CTP &&
		    pce_rule->region == GSW_PCE_RULE_CTP &&
		    p->logicalportid == pce_rule->logicalportid &&
		    p->subifidgroup == pce_rule->subifidgroup) {
			pce_blk_info->info.subblk_id = p->subblk_id;
			break;
		}
		/* Always reuse global sub-block */
		if (p->region == GSW_PCE_RULE_COMMMON &&
		    p->region == pce_rule->region) {
			pce_blk_info->info.subblk_id = p->subblk_id;
			break;
		}
	}

	index = dp_pce_rule_add(pce_blk_info, pce_rule);
	if (index < 0) {
		pr_err("%s: dp_pce_rule_add failed err %d\n",
		       __func__, index);
		kfree(pce_blk_info);
		return -EINVAL;
	}
	pr_debug("%s: pce rule added sub-blk %u with index %d for ctp: %d\n",
		 __func__, pce_blk_info->info.subblk_id,
		 index, pce_rule->subifidgroup);
	args->index = index;

	ret = storage_add(pce_rule, pce_blk_info, args);

	kfree(pce_blk_info);
	return ret;
}

int dp_oam_act_add(struct dp_oam_args *args)
{
	GSW_PCE_rule_t *pce_rule;
	int bp = 0;
	int ret;

	pce_rule = kzalloc(sizeof(*pce_rule), GFP_ATOMIC);
	if (!pce_rule)
		return -ENOMEM;

	pce_default_set(pce_rule);
	if (!test_bit(DP_OAM_ACT_COUNTER, args->act))
		pce_ethtype_cfm(pce_rule);

	if (args->dev_en) {
		ret = pce_netdev_set(pce_rule, args->ifindex,
				     args->global, &bp);
		if (ret != DP_SUCCESS)
			goto dp_oam_act_add_cleanup;
	} else {
		/* Configure for all ports */
		args->global = true;
	}

	if (test_bit(DP_OAM_ACT_EXTRACT, args->act)) {
		pce_extract_set(pce_rule, args->ext_pt);
		pce_portmap_set(pce_rule, false);
	}
	if (test_bit(DP_OAM_ACT_DROP, args->act))
		pce_portmap_set(pce_rule, true);
	if (test_bit(DP_OAM_ACT_COUNTER, args->act)) {
		ret = dp_pce_rmon_get(&args->counter_id);
		if (ret != DP_SUCCESS)
			goto dp_oam_act_add_cleanup;
		counter_id_set(pce_rule, args->counter_id);
	}
	if (test_bit(DP_OAM_ACT_RMON, args->act))
		rmon_id_set(pce_rule, args->counter_id, args->rmon_offset,
			    args->rmon_tx, args->rmon_bypass);
	if (test_bit(DP_OAM_ACT_TIMESTAMP, args->act))
		timestamp_set(pce_rule, args->ts_format, args->ts_comp,
			      args->ts_offset);

	if (args->opcode_en)
		pce_opcode_set(pce_rule, args->opcode);
	else if (test_bit(DP_OAM_ACT_EXTRACT, args->act) ||
		 test_bit(DP_OAM_ACT_DROP, args->act))
		pce_level_bitmask_set(pce_rule);

	if (args->level_en) {
		if (test_bit(DP_OAM_ACT_DROP, args->act)) {
			pce_level_range_set(pce_rule, args->level);
			ret = pce_reinsert_disable(pce_rule, args->ifindex);
			if (ret != DP_SUCCESS)
				goto dp_oam_act_add_cleanup;
		} else if (test_bit(DP_OAM_ACT_COUNTER, args->act)) {
			pce_counter_exclude_level(pce_rule, args->level);
		} else {
			pce_level_set(pce_rule, args->level);
		}
	}

	if (args->opcode_en && !args->level_en)
		pce_opcode_mask_set(pce_rule, GSW_MDL_VERSION_MASK);

	if (args->opcode_en && args->level_en)
		pce_opcode_mask_set(pce_rule, GSW_VERSION_MASK);

	if (args->vlan_en)
		vlan_id_set(pce_rule, args->vlan_id);

	if (!args->vlan_en && args->untagged)
		untagged_set(pce_rule);

	ret = pce_rule_add(pce_rule, args, bp);
	if (ret != DP_SUCCESS) {
		if (test_bit(DP_OAM_ACT_COUNTER, args->act))
			dp_pce_rmon_free(args->counter_id);
	}

dp_oam_act_add_cleanup:
	kfree(pce_rule);
	return ret;
}
EXPORT_SYMBOL(dp_oam_act_add);

static int pce_rule_del(struct pce_rule_storage *p)
{
	struct dp_pce_blk_info pce_blk_info = {0};
	GSW_PCE_ruleDelete_t *pce_rule;
	int ret;

	pce_rule = kzalloc(sizeof(*pce_rule), GFP_ATOMIC);
	if (!pce_rule)
		return -ENOMEM;

	pce_rule->logicalportid = p->logicalportid;
	pr_debug("%s: logicalportid  = %d\n", __func__,
		 pce_rule->logicalportid);
	pce_blk_info.info.portid = p->logicalportid;
	pr_debug("%s: info.portid  = %d\n", __func__,
		 pce_blk_info.info.portid);

	pce_rule->subifidgroup = p->subifidgroup;
	pr_debug("%s: subifidgroup  = %d\n", __func__,
		 pce_rule->subifidgroup);
	pce_blk_info.info.subif = p->subifidgroup;
	pr_debug("%s: info.subif  = %d\n", __func__,
		 pce_blk_info.info.subif);

	pce_rule->region = p->region;
	pr_debug("%s: region  = %d\n", __func__,
		 pce_rule->region);
	pce_blk_info.region = p->region;

	pce_rule->nIndex = p->args.index;
	pr_debug("%s: nIndex  = %d\n", __func__,
		 pce_rule->nIndex);

	pce_blk_info.info.subblk_id = p->subblk_id;
	pr_debug("%s: subblk_id  = %d\n", __func__,
		 pce_blk_info.info.subblk_id);

	ret = dp_pce_rule_del(&pce_blk_info, pce_rule);
	if (ret < 0) {
		pr_err("%s: dp_pce_rule_del failed err %d\n",
		       __func__, ret);
		kfree(pce_rule);
		return ret;
	}
	pr_debug("%s: deleted pce rule with index %d\n", __func__,
		 pce_rule->nIndex);

	kfree(pce_rule);

	return 0;
}

int dp_oam_act_del(struct dp_oam_args *args)
{
	struct pce_rule_storage *p;
	int ret = 0;

	mutex_lock(&storage_lock);
	list_for_each_entry(p, &dp_oam_storage, list) {
		if (args->ifindex == p->args.ifindex &&
		    args->index == p->args.index) {
			ret = pce_rule_del(p);
			if (ret)
				goto act_del_cleanup;
			if (test_bit(DP_OAM_ACT_COUNTER, p->args.act)) {
				ret = dp_pce_rmon_free(p->args.counter_id);
				if (ret)
					goto act_del_cleanup;
			}
			list_del(&p->list);
			kfree(p);
			break;
		}
	}

act_del_cleanup:
	mutex_unlock(&storage_lock);
	return ret;
}
EXPORT_SYMBOL(dp_oam_act_del);

#define PATH_PROC "/sys/kernel/debug/dp_oam/dbg"
static void dp_oam_proc_write_help(void)
{
	pr_info("usage: echo add/del {args} > %s\n", PATH_PROC);
	pr_info("args:\n");
	pr_info("global - rule will configured in global table\n");
	pr_info("ext_pt {val} - configure extraction rule\n");
	pr_info("drop - configure drop rule\n");
	pr_info("dev {ifindex} - rule will on specified subif, on port if global is set\n");
	pr_info("level {val} - OAM level\n");
	pr_info("opcode {val} - OAM opcode\n");
	pr_info("vlan_id {val} - VLAN id\n");
	pr_info("untaggged - match only untagged frames\n");
	pr_info("prio {val} - Rule priority\n");
	pr_info("counter - rule will be configured as counter rule\n");
	pr_info("rmon {counter_id} - rule will be configured as OAM counter rule, associated with previosuly configured counter rule\n");
	pr_info("rmon_tx - OAM counter rule will be configured in TX direction, RX by default\n");
	pr_info("rmon_bypass - Enable OAM counter rule before sending\n");
	pr_info("rmon_offset {val} - OAM counter will be inserted at specified offset\n");
	pr_info("timestamp - rule will be configured as timestamping rule\n");
	pr_info("ts_format {val} - Timestamp format: {0} DIGITAL_10B, {1} BINARY_10B, {2} DIGITAL_8B, {3} BINARY_8B\n");
	pr_info("ts_comp {val} - Timestamp compensation: -4,294,967,295 ~ 4,294,967,295\n");
	pr_info("ts_offset {val} - Timestamp will be inserted at specified offset\n");
	pr_info("index {val} - Index to be removed\n");
	pr_info("transceive {enable {level}/disable} - Enable/disable LMR and DMR receive and trasmit simulation using netfilter hooks\n");
	pr_info("tx {ifi} {level} {opcode} {vid} - send packet\n");
}

struct pce_write {
	char str[200];
	char *param_list[32];
};

static ssize_t debugfs_write(struct file *file, const char *ubuf,
			     size_t count, loff_t *ppos)
{
	struct dp_oam_args args = {0};
	struct pce_write *p = NULL;
	struct tx_data data = { 0 };
	int num, i = 0, res;
	bool del = false;
	bool rx_ts = false;
	size_t len;
	u8 level = 0;

	p = kzalloc(sizeof(*p), GFP_ATOMIC);
	if (!p)
		return count;

	len = (sizeof(p->str) > count) ? count : sizeof(p->str) - 1;
	len -= copy_from_user(p->str, ubuf, len);
	p->str[len] = 0;

	num = dp_split_buffer(p->str, p->param_list, ARRAY_SIZE(p->param_list));
	while (1) {
		if (num <= 1 || (!dp_strncmpi(p->param_list[i], "help",
					      strlen("help") + 1))) {
			kfree(p);
			dp_oam_proc_write_help();
			return count;
		} else if (!dp_strncmpi(p->param_list[i], "transceive",
					strlen("transceive") + 1)) {
			if (!dp_strncmpi(p->param_list[i + 1], "enable",
					 strlen("enable") + 1)) {
				if (p->param_list[i + 2])
					level = dp_atoi(p->param_list[i + 2]);
				if (!dp_strncmpi(p->param_list[i + 3],
						 "rx_ts", strlen("rx_ts") + 1))
					rx_ts = true;

				dp_oam_transceive_load(level, rx_ts);
			}
			if (!dp_strncmpi(p->param_list[i + 1], "disable",
					 strlen("disable") + 1)) {
				dp_oam_transceive_unload();
			}
			kfree(p);
			return count;
		} else if (!dp_strncmpi(p->param_list[i], "tx",
					strlen("tx") + 1)) {
			if (p->param_list[1 + 1])
				data.ifi = dp_atoi(p->param_list[i + 1]);
			if (p->param_list[i + 2])
				data.level = dp_atoi(p->param_list[i + 2]);
			if (p->param_list[i + 3])
				data.opcode = dp_atoi(p->param_list[i + 3]);
			if (p->param_list[i + 4])
				data.vid = dp_atoi(p->param_list[i + 4]);
			dp_oam_tx(&data);
			return count;
		} else if (!dp_strncmpi(p->param_list[i], "add",
					strlen("add") + 1)) {
			del = false;
			i++;
		} else if (!dp_strncmpi(p->param_list[i], "del",
					strlen("del") + 1)) {
			del = true;
			i++;
		} else if (!dp_strncmpi(p->param_list[i], "global",
					strlen("global") + 1)) {
			args.global = true;
			i++;
		} else if (!dp_strncmpi(p->param_list[i], "drop",
					strlen("drop") + 1)) {
			set_bit(DP_OAM_ACT_DROP, args.act);
			i++;
		} else if (!dp_strncmpi(p->param_list[i], "dev",
					strlen("dev") + 1)) {
			if (p->param_list[i + 1]) {
				args.dev_en = true;
				args.ifindex = dp_atoi(p->param_list[i + 1]);
			}
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "level",
					strlen("level") + 1)) {
			if (p->param_list[i + 1]) {
				args.level_en = true;
				args.level = dp_atoi(p->param_list[i + 1]);
			}
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "ext_pt",
					strlen("ext_pt") + 1)) {
			if (p->param_list[i + 1]) {
				set_bit(DP_OAM_ACT_EXTRACT, args.act);
				args.ext_pt = dp_atoi(p->param_list[i + 1]);
			}
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "opcode",
					strlen("opcode") + 1)) {
			if (p->param_list[i + 1]) {
				args.opcode_en = true;
				args.opcode = dp_atoi(p->param_list[i + 1]);
			}
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "vlan_id",
					strlen("vlan_id") + 1)) {
			if (p->param_list[i + 1]) {
				args.vlan_en = true;
				args.vlan_id = dp_atoi(p->param_list[i + 1]);
			}
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "prio",
					strlen("prio") + 1)) {
			if (p->param_list[i + 1])
				args.prio = dp_atoi(p->param_list[i + 1]);
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "index",
					strlen("index") + 1)) {
			if (p->param_list[i + 1])
				args.index = dp_atoi(p->param_list[i + 1]);
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "counter",
					strlen("counter") + 1)) {
			set_bit(DP_OAM_ACT_COUNTER, args.act);
			i++;
		} else if (!dp_strncmpi(p->param_list[i], "rmon",
					strlen("rmon") + 1)) {
			if (p->param_list[i + 1]) {
				set_bit(DP_OAM_ACT_RMON, args.act);
				args.counter_id = dp_atoi(p->param_list[i + 1]);
			}
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "rmon_tx",
					strlen("rmon_tx") + 1)) {
			args.rmon_tx = true;
			i++;
		} else if (!dp_strncmpi(p->param_list[i], "rmon_bypass",
					strlen("rmon_bypass") + 1)) {
			args.rmon_bypass = true;
			i++;
		} else if (!dp_strncmpi(p->param_list[i], "rmon_offset",
					strlen("rmon_offset") + 1)) {
			if (p->param_list[i + 1])
				args.rmon_offset =
					dp_atoi(p->param_list[i + 1]);
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "timestamp",
					strlen("timestamp") + 1)) {
			set_bit(DP_OAM_ACT_TIMESTAMP, args.act);
			i++;
		} else if (!dp_strncmpi(p->param_list[i], "ts_comp",
					strlen("ts_comp") + 1)) {
			if (p->param_list[i + 1]) {
				res = kstrtoll(p->param_list[i + 1], 10,
					       &args.ts_comp);
				if (res)
					args.ts_comp = 0;
			}
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "ts_offset",
					strlen("ts_offset") + 1)) {
			if (p->param_list[i + 1])
				args.ts_offset =
					dp_atoi(p->param_list[i + 1]);
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "ts_format",
					strlen("ts_format") + 1)) {
			if (p->param_list[i + 1])
				args.ts_format =
					dp_atoi(p->param_list[i + 1]);
			i += 2;
		} else if (!dp_strncmpi(p->param_list[i], "untagged",
					strlen("untagged") + 1)) {
			args.untagged = true;
			i++;
		} else {
			break;
		}

		if (i >= num)
			break;
	}

	if (del)
		(void)dp_oam_act_del(&args);
	else
		(void)dp_oam_act_add(&args);

	kfree(p);
	return count;
}

int debugfs_read(struct seq_file *file, void *ctx)
{
	struct pce_rule_storage *p;
	struct dp_oam_args *a;
	int i = 0;

	list_for_each_entry(p, &dp_oam_storage, list) {
		a = &p->args;
		seq_printf(file, "DP_OAM_ACTION[%d]:\n", i);
		if (a->dev_en)
			seq_printf(file, "ifindex = %d\n", a->ifindex);
		seq_printf(file, "global = %d\n", a->global);
		seq_printf(file, "act = %lx\n", *a->act);
		if (test_bit(DP_OAM_ACT_EXTRACT, a->act))
			seq_printf(file, "ext_pt = %d\n", a->ext_pt);
		seq_printf(file, "level_en = %d\n", a->level_en);
		if (a->level_en)
			seq_printf(file, "level = %u\n", a->level);
		seq_printf(file, "opcode_en = %d\n", a->opcode_en);
		if (a->opcode_en)
			seq_printf(file, "opcode = %u\n", a->opcode);
		seq_printf(file, "vlan_en = %d\n", a->vlan_en);
		if (a->vlan_en)
			seq_printf(file, "vlan_id = %u\n", a->vlan_id);
		if (a->untagged)
			seq_puts(file, "untagged\n");
		if (test_bit(DP_OAM_ACT_COUNTER, a->act))
			seq_printf(file, "counter_id = %u\n", a->counter_id);
		if (test_bit(DP_OAM_ACT_RMON, a->act)) {
			seq_printf(file, "counter_id = %u\n", a->counter_id);
			seq_printf(file, "rmon_tx = %d\n", a->rmon_tx);
			seq_printf(file, "rmon_offset = %u\n", a->rmon_offset);
		}
		seq_printf(file, "prio = %u\n", a->prio);
		seq_printf(file, "index = %u\n", a->index);
		i++;
	}

	return 0;
}

static int debugfs_open(struct inode *inode, struct file *file)
{
	return single_open(file, debugfs_read, inode->i_private);
}

static const struct file_operations dp_oam_fops = {
	.owner = THIS_MODULE,
	.llseek = seq_lseek,
	.release = single_release,
	.open = debugfs_open,
	.read = seq_read,
	.write = debugfs_write,
};

#define DEBUGFS_DIR "dp_oam"
#define DEBUGFS_FILE "dbg"
static struct dentry *dir;

static int __init dp_oam_init(void)
{
	struct dentry *file;

	dir = debugfs_create_dir(DEBUGFS_DIR, NULL);
	if (!dir) {
		pr_err("%s: debugfs_create_dir failed\n", __func__);
		return -1;
	}

	file = debugfs_create_file(DEBUGFS_FILE, 0644, dir,
				   NULL, &dp_oam_fops);
	if (!file) {
		debugfs_remove_recursive(dir);
		return -ENOENT;
	}

	dp_oam_transceive_init();

	dp_oam_tc_hook_fn_set();

	return 0;
}

static void __exit dp_oam_destroy(void)
{
	struct pce_rule_storage *p, *n;

	list_for_each_entry_safe(p, n, &dp_oam_storage, list) {
		(void)pce_rule_del(p);
		list_del(&p->list);
		kfree(p);
	}

	debugfs_remove_recursive(dir);

	dp_oam_tc_hook_fn_unset();
}

module_init(dp_oam_init);
module_exit(dp_oam_destroy);
MODULE_LICENSE("GPL");
MODULE_ALIAS_RTNL_LINK(DRV_NAME);
