// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2021, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/etherdevice.h>
#include <linux/if_bridge.h>
#include <net/datapath_api.h>
#include "../datapath_swdev.h"
#include "../datapath.h"
#include "../datapath_instance.h"
#include "datapath_misc.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
int dp_swdev_alloc_bridge_id(int inst)
{
	GSW_BRIDGE_alloc_t br;
	struct core_ops *gsw_ops;

	gsw_ops = dp_port_prop[inst].ops[GSWIP_R];
	if (gsw_ops->gsw_brdg_ops.Bridge_Alloc(gsw_ops, &br)) {
		pr_err("DPM: %s Failed to get a FID\n", __func__);
		return DP_FAILURE;
	}
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "FID=%d\n", br.nBridgeId);
	return br.nBridgeId;
}

int dp_swdev_get_dp_port_from_bp(struct br_info *br_item, int bport,
				 u32 *priv_flag)
{
	struct bridge_member_port *bport_list = NULL;

	list_for_each_entry(bport_list, &br_item->bp_list, list) {
		if (bport_list->bportid == bport) {
			*priv_flag = bport_list->dev_priv_flag;
			return bport_list->dp_port;
		}
	}

	return DP_FAILURE;
}

/* bp_member - Bridge port to be configured to cur_bport's port map
 * cur_bp - Bport which is getting configured
 * Note: domain validity check not require for CPU BP,
 * hairpin mode configuration
 */
bool is_valid_domain(int inst, int curr_bp, int bp_member)
{
	struct bp_pmapper *curr_bp_info;
	struct bp_pmapper *bp_mem_info;

	curr_bp_info = get_dp_bp_info(inst, curr_bp);
	bp_mem_info = get_dp_bp_info(inst, bp_member);

	if ((bp_member == CPU_BP) || (bp_member == curr_bp))
		return true;

	DP_DEBUG(DP_DBG_FLAG_SWDEV, "domain_member:0x%x domain_id:%d\n",
		 curr_bp_info->domain_member,
		 bp_mem_info->domain_id);
	/* check BP member's domain id present in current BP domain member */
	if (curr_bp_info->domain_member & (1 << bp_mem_info->domain_id))
		return true;

	return false;
}

/* bport     - Bridge member port in the bport list
 * cfg_bport - Bridge port to be configured to cur_bport port map
 * cur_bport - Bport which is getting configured, only for debug
 * isolate   - 1 - isolate feature is set, otherwise 0
 * priv_flag - bp dev priv flags (like IFF_NO_QUEUE)
 * loop_dis  - If set do not add this dp port's bp into other bp's member
 *			   port map
 * bport_cfg - Gswip structure for the currently configured bridge port
 * reset	 - 1 means bridge config reset, otherwise setting
 */
static
void dp_cfg_br_portmap(int inst, struct bridge_member_port *bport,
		       int cfg_bport, int cur_bport, bool isolate, u32 priv_flag,
		       u16 loop_dis, int dp_port, GSW_BRIDGE_portConfig_t *bport_cfg,
		       int reset)
{
	struct bp_pmapper *bp_info;

	bp_info = get_dp_bp_info(inst, cur_bport);

	if (reset || isolate) {
		DP_DEBUG(DP_DBG_FLAG_SWDEV,
			 "UnSet BP=%d to BP=%d\n", cfg_bport, cur_bport);
		UNSET_BP_MAP(bport_cfg->nBridgePortMap, cfg_bport);
		/* reset bridge member port info from DP bridge port table */
		UNSET_BP_MAP(bp_info->bp_member_map, cfg_bport);

		return;
	}

	/* if member to be added for current bp,
	 * share same dp_port with no loop set or with IFF_NO_QUEUE set
	 * then don't add the member to its portmap
	 */
	if ((priv_flag & IFF_NO_QUEUE) ||
	    (loop_dis && bport->dp_port == dp_port)) {
		DP_DEBUG(DP_DBG_FLAG_SWDEV,
			 "UnSet BP=%d to BP=%d\n", cfg_bport, cur_bport);
		UNSET_BP_MAP(bport_cfg->nBridgePortMap, cfg_bport);
		/* reset bridge member port info from DP bridge port table */
		UNSET_BP_MAP(bp_info->bp_member_map, cfg_bport);
	} else {
		DP_DEBUG(DP_DBG_FLAG_SWDEV,
			 "Set BP=%d to BP=%d\n", cfg_bport, cur_bport);
		if (is_valid_domain(inst, cur_bport, cfg_bport))
			SET_BP_MAP(bport_cfg->nBridgePortMap, cfg_bport);
		/* Add bridge member port info to DP bridge port table */
		SET_BP_MAP(bp_info->bp_member_map, cfg_bport);
	}

	return;
}

/* Go through all other bridge ports in list other than cur bridge port,
 * Reset=0
 *     Unset all other bridge ports from the current bridge port if loop_dis=1
 *     Set all other bridge ports to the current bridge port if loop_dis=0
 *     if hairpin=1
 *			set current bridge port to the current bridge port portmap
 * Reset=1
 *     Unset all other bridge ports from the current bridge port
 *     if hairpin=1,
 *			unset current bridge port from the crrent bridge port portmap
 */
static
int dp_cfg_own_bridgeport(struct br_info *br_item, int inst, int bport,
			  int reset)
{
	struct bridge_member_port *bport_list = NULL;
	GSW_BRIDGE_portConfig_t *brportcfg;
	struct core_ops *gsw_ops;
	struct brdgport_ops *gsw_bp;
	int dp_port = -1;
	u32 priv_flag = 0;
	struct pmac_port_info *p_info;

	p_info = get_dp_port_info(br_item->inst, br_item->dp_port);

	/* In reset case, entry is already removed from bridge port list */
	if (!reset) {
		dp_port = dp_swdev_get_dp_port_from_bp(br_item, bport, &priv_flag);
		if (dp_port < 0) {
			pr_err("DPM: %s: portid:%d for bport=%d invalid\n",
			       __func__, dp_port, bport);
			return DP_FAILURE;
		}
	}

	gsw_ops = dp_port_prop[inst].ops[GSWIP_R];
	gsw_bp = &gsw_ops->gsw_brdgport_ops;
	brportcfg = dp_kzalloc(sizeof(*brportcfg), GFP_ATOMIC);
	if (!brportcfg)
		return DP_FAILURE;
	/* To set other bridge port members to the current bport */
	brportcfg->nBridgePortId = bport;
	brportcfg->eMask = GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP;
	if (gsw_bp->BridgePort_ConfigGet(gsw_ops, brportcfg)) {
		kfree(brportcfg);
		DP_DEBUG(DP_DBG_FLAG_SWDEV,
			 "BridgePort_ConfigGet fail:bp=%d\n", bport);
		return DP_FAILURE;
	}

	if (p_info->bp_hairpin_cap) {
		if (reset) {
			UNSET_BP_MAP(brportcfg->nBridgePortMap, bport);
		} else {
			if (is_valid_domain(inst, bport, bport))
				SET_BP_MAP(brportcfg->nBridgePortMap, bport);
		}

		DP_DEBUG(DP_DBG_FLAG_SWDEV,
			 "Hairpin %s BP=%d to BP=%d\n",
			 reset ? "Unset" : "Set",
			 bport, bport);
	}

	list_for_each_entry(bport_list, &br_item->bp_list, list) {
		if (bport_list->bportid == bport)
			continue;
		dp_cfg_br_portmap(br_item->inst, bport_list, bport_list->bportid,
				  bport, 0, bport_list->dev_priv_flag,
				  p_info->loop_dis, dp_port, brportcfg, reset);
	}

	if (reset)
		brportcfg->nBridgeId = CPU_FID; /* reset of FID */
	else
		brportcfg->nBridgeId = br_item->fid;
	brportcfg->eMask = GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_ID |
			   GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP |
			   GSW_BRIDGE_PORT_CONFIG_MASK_MAC_LEARNING_LIMIT;

	/* Limit the MAC learning on each port by default to 254 entries.
	 * The PRX300 SDL TMR requires us to limit this by default.
	 */
	brportcfg->nMacLearningLimit = GSWIP_LEARN_LIMIT_PORT_MAX;
	brportcfg->bMacLearningLimitEnable = 1;
	if (gsw_bp->BridgePort_ConfigSet(gsw_ops, brportcfg)) {
		kfree(brportcfg);
		pr_err("DPM: %s: Fail in allocating/configuring bridge port\n",
		       __func__);
		return DP_FAILURE;
	}
	kfree(brportcfg);
	return DP_SUCCESS;
}

/* Go through all other bridge ports in list other than cur bridge port,
 * Reset=0
 *     Unset currrent bridge port from all other bridge port if loop_dis=1
 *     Set currrent bridge port from all other bridge port if loop_dis=0
 *	   if isolate=1
 *			Unset current bridge port from all other bridge ports if isolate=1
 * Reset=1
 *     Unset current bridge port from all other bridge ports in list
 */
static
int dp_cfg_other_bridgeport(struct br_info *br_item, int inst,
			    int bport, bool isolate, int reset)
{
	struct bridge_member_port *bport_list = NULL;
	GSW_BRIDGE_portConfig_t *brportcfg;
	struct core_ops *gsw_ops;
	struct brdgport_ops *gsw_bp;
	int dp_port = -1;
	u32 priv_flag = 0;
	struct pmac_port_info *p_info;

	p_info = get_dp_port_info(br_item->inst, br_item->dp_port);

	/* In reset case, entry is already removed from bridge port list */
	if (!reset) {
		dp_port = dp_swdev_get_dp_port_from_bp(br_item, bport, &priv_flag);
		if (dp_port < 0) {
			pr_err("DPM: %s portid:%d for bport=%d invalid\n",
			       __func__, br_item->dp_port, bport);
			return DP_FAILURE;
		}
	}

	gsw_ops = dp_port_prop[inst].ops[GSWIP_R];
	gsw_bp = &gsw_ops->gsw_brdgport_ops;
	brportcfg = dp_kmalloc(sizeof(*brportcfg), GFP_ATOMIC);
	if (!brportcfg)
		return DP_FAILURE;
	/* To set other member portmap with current bridge port */
	list_for_each_entry(bport_list, &br_item->bp_list, list) {
		if (bport_list->bportid == bport)
			continue;

		dp_memset(brportcfg, 0, sizeof(GSW_BRIDGE_portConfig_t));
		brportcfg->nBridgePortId = bport_list->bportid;
		brportcfg->eMask = GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_ID |
				   GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP;
		if (gsw_bp->BridgePort_ConfigGet(gsw_ops, brportcfg)) {
			kfree(brportcfg);
			pr_err("DPM: %s fail in getting br port config\r\n", __func__);
			return DP_FAILURE;
		}

		dp_cfg_br_portmap(br_item->inst, bport_list, bport,
				  bport_list->bportid, isolate,
				  priv_flag, p_info->loop_dis, dp_port,
				  brportcfg, reset);

		brportcfg->nBridgeId = br_item->fid;
		brportcfg->eMask = GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_ID |
				   GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP;
		if (gsw_bp->BridgePort_ConfigSet(gsw_ops, brportcfg)) {
			kfree(brportcfg);
			pr_err("DPM: %s Fail alloc/cfg bridge port\n", __func__);
			return DP_FAILURE;
		}
	}
	kfree(brportcfg);
	return DP_SUCCESS;
}

int dp_swdev_bridge_port_cfg_set(struct br_info *br_item,
				 int inst, int bport)
{
	/* To set other bridge port members to the current bport */
	if (dp_cfg_own_bridgeport(br_item, inst, bport, 0)) {
		pr_err("DPM: %s fail in dp_cfg_own_bridge\n", __func__);
		return DP_FAILURE;
	}

	/* To set other member portmap with current bridge port map */
	if (dp_cfg_other_bridgeport(br_item, inst, bport, 0, 0)) {
		pr_err("DPM: %s fail in dp_cfg_other_bridge\n", __func__);
		return DP_FAILURE;
	}

	return DP_SUCCESS;
}

int dp_swdev_bridge_port_cfg_reset(struct br_info *br_item,
				   int inst, int bport)
{
	struct bridge_member_port *bport_list = NULL;
	int cnt = 0;

	list_for_each_entry(bport_list, &br_item->bp_list, list) {
		if (bport_list->bportid)
			cnt++;
	}

	/* Unset all other bridge ports from the current bridge port portmap*/
	if (dp_cfg_own_bridgeport(br_item, inst, bport, 1)) {
		pr_err("DPM: %s fail in dp_cfg_own_bridge\n", __func__);
		return DP_FAILURE;
	}

	/* Unset current bridge port from all other bridge ports portmap */
	if (dp_cfg_other_bridgeport(br_item, inst, bport, 0, 1)) {
		pr_err("DPM: %s fail in dp_cfg_other_bridge\n", __func__);
		return DP_FAILURE;
	}

	/* Remove bridge entry if no member in port map of
	 * current bport except CPU port
	 */
	if (cnt == 0)
		return DEL_BRENTRY;

	return DP_SUCCESS;
}

int dp_swdev_bridge_cfg_set(int inst, u16 fid)
{
	GSW_BRIDGE_config_t brcfg = {0};
	GSW_BRIDGE_alloc_t br = {0};
	struct core_ops *gsw_ops;

	gsw_ops = dp_port_prop[inst].ops[GSWIP_R];
	brcfg.nBridgeId = fid;
	brcfg.eMask = GSW_BRIDGE_CONFIG_MASK_FORWARDING_MODE;
	brcfg.eForwardBroadcast = GSW_BRIDGE_FORWARD_FLOOD;
	brcfg.eForwardUnknownMulticastNonIp = GSW_BRIDGE_FORWARD_FLOOD;
	brcfg.eForwardUnknownUnicast = GSW_BRIDGE_FORWARD_FLOOD;
	if (gsw_ops->gsw_brdg_ops.Bridge_ConfigSet(gsw_ops, &brcfg)) {
		pr_err("DPM: %s: Fail to set bridge id(%d)\n", __func__,
		       brcfg.nBridgeId);
		br.nBridgeId = fid;
		gsw_ops->gsw_brdg_ops.Bridge_Free(gsw_ops, &br);
		return DP_FAILURE;
	}
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "FID(%d) cfg success for inst %d\n",
		 fid, inst);

	return DP_SUCCESS;
}

int dp_swdev_bridge_port_flags_set(struct br_info *br_item, int inst, int bport,
				   unsigned long flags)
{
	GSW_BRIDGE_portConfig_t *bpcfg;
	struct core_ops *gsw_ops;
	struct brdgport_ops *gsw_bp;
	unsigned long learning;

	bpcfg = dp_kzalloc(sizeof(*bpcfg), GFP_ATOMIC);
	if (!bpcfg)
		return DP_FAILURE;
	gsw_ops = dp_port_prop[inst].ops[GSWIP_R];
	gsw_bp = &gsw_ops->gsw_brdgport_ops;

	bpcfg->nBridgePortId = bport;
	bpcfg->eMask |= GSW_BRIDGE_PORT_CONFIG_MASK_MC_SRC_MAC_LEARNING |
			GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP;
	if (gsw_bp->BridgePort_ConfigGet(gsw_ops, bpcfg)) {
		kfree(bpcfg);
		pr_err("DPM: %s: bridge port config get failed\r\n", __func__);
		return DP_FAILURE;
	}

	bpcfg->eMask = 0;

	bpcfg->bMcDestIpLookupDisable = !(flags & BR_MCAST_DEST_LOOKUP);
	bpcfg->bMcSrcIpLookupEnable = !!(flags & BR_MCAST_SRC_LOOKUP);
	bpcfg->eMask |= GSW_BRIDGE_PORT_CONFIG_MASK_MC_DEST_IP_LOOKUP |
			GSW_BRIDGE_PORT_CONFIG_MASK_MC_SRC_IP_LOOKUP;

	learning = !bpcfg->bSrcMacLearningDisable ? BR_LEARNING : 0;
	if ((learning ^ flags) & BR_LEARNING) {
		bpcfg->bSrcMacLearningDisable = !bpcfg->bSrcMacLearningDisable;
		bpcfg->eMask |= GSW_BRIDGE_PORT_CONFIG_MASK_MC_SRC_MAC_LEARNING;
	}
	if (gsw_bp->BridgePort_ConfigSet(gsw_ops, bpcfg)) {
		kfree(bpcfg);
		pr_err("DPM: %s: config bridge port failed\r\n", __func__);
		return DP_FAILURE;
	}
	kfree(bpcfg);
	return DP_SUCCESS;
}

int dp_swdev_bridge_port_flags_get(int inst, int bport, unsigned long *flags)
{
	GSW_BRIDGE_portConfig_t *bpcfg;
	struct core_ops *gsw_ops;
	struct brdgport_ops *gsw_bp;

	gsw_ops = dp_port_prop[inst].ops[GSWIP_R];
	gsw_bp = &gsw_ops->gsw_brdgport_ops;
	bpcfg = dp_kzalloc(sizeof(*bpcfg), GFP_ATOMIC);
	if (!bpcfg)
		return DP_FAILURE;
	bpcfg->nBridgePortId = bport;
	bpcfg->eMask |= GSW_BRIDGE_PORT_CONFIG_MASK_MC_SRC_MAC_LEARNING |
			GSW_BRIDGE_PORT_CONFIG_MASK_MC_DEST_IP_LOOKUP |
			GSW_BRIDGE_PORT_CONFIG_MASK_MC_SRC_IP_LOOKUP;
	if (gsw_bp->BridgePort_ConfigGet(gsw_ops, bpcfg)) {
		kfree(bpcfg);
		pr_err("DPM: %s: bridge port config get failed\r\n", __func__);
		return DP_FAILURE;
	}

	*flags |= !bpcfg->bSrcMacLearningDisable ? BR_LEARNING : 0;
	*flags |= !bpcfg->bMcDestIpLookupDisable ? 0 : BR_MCAST_DEST_LOOKUP;
	*flags |= !bpcfg->bMcSrcIpLookupEnable ? BR_MCAST_SRC_LOOKUP : 0;
	kfree(bpcfg);
	return 0;
}

#ifndef DP_SWITCHDEV_NO_HACKING
int dp_swdev_port_learning_limit_set(int inst, int bport, int learning_limit,
				     struct dp_dev *dp_dev)
{
	GSW_BRIDGE_portConfig_t *bpcfg;
	struct core_ops *gsw_ops;
	struct brdgport_ops *gsw_bp;

	gsw_ops = dp_port_prop[inst].ops[GSWIP_R];
	gsw_bp = &gsw_ops->gsw_brdgport_ops;
	bpcfg = dp_kzalloc(sizeof(*bpcfg), GFP_ATOMIC);
	if (!bpcfg)
		return DP_FAILURE;
	bpcfg->nBridgePortId = bport;
	bpcfg->eMask |= GSW_BRIDGE_PORT_CONFIG_MASK_MAC_LEARNING_LIMIT;
	if (gsw_bp->BridgePort_ConfigGet(gsw_ops, bpcfg)) {
		kfree(bpcfg);
		pr_err("DPM: %s: bridge port config get failed\r\n", __func__);
		return DP_FAILURE;
	}

	bpcfg->eMask |= GSW_BRIDGE_PORT_CONFIG_MASK_MAC_LEARNING_LIMIT;
	if (learning_limit == BRPORT_LEARNING_LIMIT_UNLIMITED) {
		bpcfg->bMacLearningLimitEnable = 0;
		dp_dev->learning_limit_en = 0;
		/* Learning limit is not enabled thus the value of
		 * nMacLearningLimit makes no difference; the switch
		 * will set it to 0xff no matter what we set (here we set
		 * 0xfe for example)
		 */
		bpcfg->nMacLearningLimit = GSWIP_LEARN_LIMIT_PORT_MAX;
		dp_dev->bp_learning_limit = GSWIP_LEARN_LIMIT_PORT_MAX;
	} else {
		/* The check below is rendundant. Values which are not allowed
		 * are rejected already on higher level
		 */
		if (learning_limit > GSWIP_LEARN_LIMIT_PORT_MAX)
			learning_limit = GSWIP_LEARN_LIMIT_PORT_MAX;
		bpcfg->nMacLearningLimit = learning_limit;
		bpcfg->bMacLearningLimitEnable = 1;
		dp_dev->bp_learning_limit = bpcfg->nMacLearningLimit;
		dp_dev->learning_limit_en = bpcfg->bMacLearningLimitEnable;
	}

	if (gsw_bp->BridgePort_ConfigSet(gsw_ops, bpcfg)) {
		kfree(bpcfg);
		pr_err("DPM: %s: config bridge port failed\r\n", __func__);
		return DP_FAILURE;
	}
	kfree(bpcfg);
	return DP_SUCCESS;
}

int dp_swdev_bridge_mcast_flood(int inst, int br_id,
				enum br_mcast_flood_mode mcast_flood)
{
	GSW_BRIDGE_config_t *br_cfg;
	struct core_ops *gsw_ops;
	struct brdgport_ops *gsw_bp;
	GSW_return_t ret;

	gsw_ops = dp_port_prop[inst].ops[GSWIP_R];
	gsw_bp = &gsw_ops->gsw_brdgport_ops;
	br_cfg = dp_kzalloc(sizeof(*br_cfg), GFP_ATOMIC);
	if (!br_cfg)
		return DP_FAILURE;

	br_cfg->nBridgeId = br_id;
	br_cfg->eMask = GSW_BRIDGE_CONFIG_MASK_FORWARDING_MODE;

	ret = gsw_ops->gsw_brdg_ops.Bridge_ConfigGet(gsw_ops, br_cfg);
	if (ret != GSW_statusOk) {
		pr_err("DPM: %s: Bridge_ConfigGet returned %d\n", __func__, ret);
		kfree(br_cfg);
		return -EIO;
	}

	/* set bridge parameters */
	br_cfg->nBridgeId = br_id;
	switch (mcast_flood) {
	case BR_MCAST_FLOOD_ALL:
		br_cfg->eForwardUnknownMulticastNonIp = GSW_BRIDGE_FORWARD_FLOOD;
		br_cfg->eForwardUnknownMulticastIp = GSW_BRIDGE_FORWARD_FLOOD;
		break;
	case BR_MCAST_FLOOD_DISCARD:
		br_cfg->eForwardUnknownMulticastNonIp = GSW_BRIDGE_FORWARD_DISCARD;
		br_cfg->eForwardUnknownMulticastIp = GSW_BRIDGE_FORWARD_DISCARD;
		break;
	case BR_MCAST_FLOOD_CPU:
		br_cfg->eForwardUnknownMulticastNonIp = GSW_BRIDGE_FORWARD_CPU;
		br_cfg->eForwardUnknownMulticastIp = GSW_BRIDGE_FORWARD_CPU;
		break;
	default:
		pr_err("DPM: %s: unsupported mcast_flood option: %d\n", __func__,
		       mcast_flood);
		break;
	}
	br_cfg->eMask = GSW_BRIDGE_CONFIG_MASK_FORWARDING_MODE;

	ret = gsw_ops->gsw_brdg_ops.Bridge_ConfigSet(gsw_ops, br_cfg);
	if (ret != GSW_statusOk) {
		pr_err("DPM: %s: Bridge_ConfigSet returned %d\n", __func__, ret);
		kfree(br_cfg);
		return -EIO;
	}
	kfree(br_cfg);
	return DP_SUCCESS;
}
#endif /* DP_SWITCHDEV_NO_HACKING */

int dp_swdev_free_brcfg(int inst, u16 fid)
{
	GSW_BRIDGE_alloc_t br = {0};
	struct core_ops *gsw_ops;

	gsw_ops = dp_port_prop[inst].ops[GSWIP_R];
	br.nBridgeId = fid;
	if (gsw_ops->gsw_brdg_ops.Bridge_Free(gsw_ops, &br)) {
		pr_err("DPM: %s Fail to free bridge id(%d)\n", __func__, br.nBridgeId);
		return DP_FAILURE;
	}
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "FID(%d) freed for inst:%d\n",
		 fid, inst);

	return DP_SUCCESS;
}

int dp_gswip_ext_vlan(int inst, int vap, int ep)
{
	struct core_ops *gsw_ops;
	struct ext_vlan_info *vlan;
	struct vlan_prop vlan_prop = {0};
	struct pmac_port_info *port;
	struct logic_dev *tmp = NULL;
	int flag = 0, ret, i = 0;
	int v1 = 0, v2 = 0;
	struct dp_subif_info *sif;

	gsw_ops = dp_port_prop[inst].ops[GSWIP_R];
	port = get_dp_port_info(inst, ep);
	vlan = dp_kzalloc(sizeof(*vlan), GFP_ATOMIC);
	if (!vlan)
		return 0;
	vlan->vlan2_list = dp_kzalloc(sizeof(*vlan->vlan2_list), GFP_ATOMIC);
	if (!vlan->vlan2_list)
		goto exit;
	vlan->vlan1_list = dp_kzalloc(sizeof(*vlan->vlan1_list), GFP_ATOMIC);
	if (!vlan->vlan1_list)
		goto exit;
	sif = get_dp_port_subif(port, vap);
	list_for_each_entry(tmp, &sif->logic_dev, list) {
		DP_DEBUG(DP_DBG_FLAG_SWDEV, "tmp dev name: %s\n",
			 tmp->dev ? tmp->dev->name : "NULL");
		if (!tmp->dev) {
			pr_err("DPM: %s tmp->dev is NULL\n", __func__);
			goto exit;
		}
		ret = dp_swdev_chk_bport_in_br(tmp->dev, tmp->bp, inst);
		if (ret)
			continue;
		get_vlan_via_dev(tmp->dev, &vlan_prop);
		if (vlan_prop.num == 2) {
			DP_DEBUG(DP_DBG_FLAG_SWDEV,
				 "VLAN In-proto=%x vid=%d out-proto=%x vid=%d\n",
				 vlan_prop.in_proto,
				 vlan_prop.in_vid,
				 vlan_prop.out_proto,
				 vlan_prop.out_vid);
			vlan->vlan2_list[v2].outer_vlan.vid = vlan_prop.out_vid;
			vlan->vlan2_list[v2].outer_vlan.tpid =
				vlan_prop.out_proto;
			vlan->vlan2_list[v2].ether_type = 0;
			vlan->vlan2_list[v2].inner_vlan.vid = vlan_prop.in_vid;
			vlan->vlan2_list[v2].inner_vlan.tpid =
				vlan_prop.in_proto;
			vlan->vlan2_list[v2].bp = tmp->bp;
			v2 += 1;
		} else if (vlan_prop.num == 1) {
			DP_DEBUG(DP_DBG_FLAG_SWDEV,
				 "outer VLAN proto=%x, vid=%d\n",
				 vlan_prop.out_proto,
				 vlan_prop.out_vid);
			vlan->vlan1_list[v1].outer_vlan.vid = vlan_prop.out_vid;
			vlan->vlan1_list[v1].outer_vlan.tpid =
				vlan_prop.out_proto;
			vlan->vlan1_list[v1].bp = tmp->bp;
			v1 += 1;
		}
		i += 1;
	}
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "vlan1=%d vlan2=%d total vlan int=%d\n",
		 v1, v2, i);
	vlan->n_vlan1 = v1;
	vlan->n_vlan2 = v2;
	vlan->bp = sif->bp;
	vlan->logic_port = port->port_id;
	/* subif value */
	vlan->subif_grp = GET_VAP(sif->subif, port->vap_offset, port->vap_mask);
	if (sif->swdev_priv)
		vlan->priv = sif->swdev_priv;
	else
		vlan->priv = NULL;
	ret = set_gswip_ext_vlan(gsw_ops, vlan, flag);
	if (ret == 0)
		sif->swdev_priv = vlan->priv;
	else
		pr_err("DPM: %s set gswip ext vlan return error\n", __func__);

exit:
	kfree(vlan->vlan2_list);
	kfree(vlan->vlan1_list);
	kfree(vlan);

	/* return -EIO from GSWIP but later cannot fail swdev */
	return DP_SUCCESS;
}
#endif

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP
