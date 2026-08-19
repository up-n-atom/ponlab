// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2021, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 * Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 * Copyright 2012 - 2014 Lantiq Deutschland GmbH
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <linux/bitfield.h>
#include <linux/kernel.h>
#include <net/datapath_api.h>
#include "../datapath.h"
#include "datapath_misc.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

#define BP_CFG(bp_cfg, _index, id, bflag) ({ \
	       typeof(_index) (index) = (_index); \
	       (bp_cfg)->bEgressSubMeteringEnable[(index)] = bflag; \
	       (bp_cfg)->nEgressTrafficSubMeterId[(index)] = id; })

#define BR_CFG(br_cfg, _index, id, bflag) ({\
	       typeof(_index) (index) = (_index); \
	       (br_cfg)->bSubMeteringEnable[(index)] = bflag; \
	       (br_cfg)->nTrafficSubMeterId[(index)] = id; })

#define METER_CIR(cir)  div_u64(cir, 1000)
#define METER_PIR(pir)  div_u64(pir, 1000)

#define MAX_TC		16
#define MAX_FLOW	4
#define SWDEV_DIS	0
#define SWDEV_EN	1

#define GSW_MPE1		BIT(0)
#define GSW_MPE2		BIT(1)
#define GSW_REDUCED_TC		GENMASK(3, 2)
#define GSW_ENC			BIT(2)
#define GSW_DEC			BIT(3)
#define ETHSW_CLK		0x1
#define SLL3MEM_EN		BIT(0)

/* FIXME: Need to support multiple instances */
static struct gsw_itf itf_assign[PMAC_MAX_NUM] = {0};

enum {
	CTP_DEFAULT,
	CTP_CPU,
	CTP_GPON,
	CTP_EPON,
	CTP_GINT,
	CTP_FAST_ETH_WAN,
	CTP_FAST_ETH_LAN,
	CTP_FAST_ETH_LAN_EXP,
	CTP_FAST_WLAN,
	CTP_FAST_WLAN_EXT,
	CTP_VUNI,
	CTP_FAST_DSL,
	CTP_DOCSIS,
	CTP_DIRECTPATH,
	CTP_MAX,
};

#define CTP_ASSIGN_MASK GENMASK(17, 0)

static const struct ctp_assign ctp_assign_info[3][CTP_MAX] = {
	{{ 0 }},
	/* GSWIP 31 */
	{
		[CTP_DEFAULT]
		{ GSW_LOGICAL_PORT_8BIT_WLAN, 8, 8, 0, CQE_LU_MODE2, SWDEV_EN,
			VAP_8BIT_OFFSET, VAP_8BIT_MASK_31 },
		/* dummy CPU subif configuration with minimal CTP reservation
		 * in order to simplify dpm code.
		 * Although CPU reserve 4 subif to display 4 CPU dequeue port
		 * information via cat dp/port. But only subif 0 is really used.
		 */
		[CTP_CPU]
		{ GSW_LOGICAL_PORT_8BIT_WLAN, ALL_CPU_CTP_NUM, ALL_CPU_CTP_NUM,
			0, CQE_LU_MODE0, SWDEV_DIS, VAP_8BIT_OFFSET,
			VAP_8BIT_MASK_31 },
		[CTP_GPON]
		{ GSW_LOGICAL_PORT_GPON, 256, 256, 0, CQE_LU_MODE1, SWDEV_EN,
			VAP_NONWLAN_8BIT_OFFSET, VAP_NONWLAN_8BIT_MASK },
		[CTP_EPON]
		{ GSW_LOGICAL_PORT_EPON, 64, 64, 0, CQE_LU_MODE1, SWDEV_EN,
			VAP_NONWLAN_8BIT_OFFSET, VAP_NONWLAN_8BIT_MASK },
		[CTP_GINT]
		{ GSW_LOGICAL_PORT_GINT, 16, 16, 0, CQE_LU_MODE1, SWDEV_EN,
			VAP_NONWLAN_8BIT_OFFSET, VAP_NONWLAN_8BIT_MASK},
		[CTP_FAST_ETH_WAN]
		{ GSW_LOGICAL_PORT_8BIT_WLAN, 8, 8, 0, CQE_LU_MODE2, SWDEV_EN,
			VAP_8BIT_OFFSET, VAP_8BIT_MASK_31 },
		[CTP_FAST_ETH_LAN]
		{ GSW_LOGICAL_PORT_8BIT_WLAN, 4, 4, 0, CQE_LU_MODE2, SWDEV_EN,
			VAP_8BIT_OFFSET, VAP_8BIT_MASK_31 },
		[CTP_FAST_ETH_LAN_EXP]
		{ GSW_LOGICAL_PORT_8BIT_WLAN, 8, 8, 0, CQE_LU_MODE2, SWDEV_EN,
			VAP_8BIT_OFFSET, VAP_8BIT_MASK_31 },
		[CTP_VUNI]
		{ GSW_LOGICAL_PORT_8BIT_WLAN, 2, 2, 0, CQE_LU_MODE2, SWDEV_EN,
			VAP_8BIT_OFFSET, VAP_8BIT_MASK_31 },
		[CTP_FAST_WLAN]
		{ GSW_LOGICAL_PORT_8BIT_WLAN, 16, 16, 0, CQE_LU_MODE2, SWDEV_EN,
			VAP_8BIT_OFFSET, VAP_8BIT_MASK_31 },
		[CTP_FAST_DSL]
		{ GSW_LOGICAL_PORT_8BIT_WLAN, 1, 16, 0, CQE_LU_MODE0, SWDEV_EN,
			VAP_8BIT_OFFSET, VAP_8BIT_MASK_31 },
		[CTP_DIRECTPATH]
		{ GSW_LOGICAL_PORT_8BIT_WLAN, 4, 4, 0, CQE_LU_MODE2, SWDEV_EN,
			VAP_8BIT_OFFSET, VAP_8BIT_MASK_31 },
		[CTP_FAST_WLAN_EXT]
		{ GSW_LOGICAL_PORT_9BIT_WLAN, 8, 8, 0, CQE_LU_MODE2, SWDEV_EN,
			VAP_9BIT_OFFSET, VAP_9BIT_MASK_31 }
	},
	/* GSWIP 32 */
	{
		[CTP_DEFAULT]
		{ GSW_LOGICAL_PORT_8BIT_WLAN, 8, 8, 8, CQE_LU_MODE2, SWDEV_DIS,
			VAP_8BIT_OFFSET, VAP_8BIT_MASK_32, 8, 7, 4 },
		[CTP_CPU]
		{ GSW_LOGICAL_PORT_9BIT_WLAN, ALL_CPU_CTP_NUM, ALL_CPU_CTP_NUM,
			ALL_CPU_CTP_NUM, CQE_LU_MODE2, SWDEV_DIS,
			VAP_9BIT_OFFSET, VAP_9BIT_MASK_32, 9, 6, 4},
		[CTP_GPON]
		{ GSW_LOGICAL_PORT_GPON, 128, 128, 1, CQE_LU_MODE1, SWDEV_EN,
			VAP_NONWLAN_8BIT_OFFSET, VAP_NONWLAN_8BIT_MASK, 0, 8, 4 },
		[CTP_EPON]
		{ GSW_LOGICAL_PORT_EPON, 64, 64, 128, CQE_LU_MODE1, SWDEV_EN,
			VAP_NONWLAN_8BIT_OFFSET, VAP_NONWLAN_8BIT_MASK, 0, 8, 4 },
		[CTP_GINT]
		{ GSW_LOGICAL_PORT_GINT, 16, 16, 8, CQE_LU_MODE1, SWDEV_DIS,
			VAP_NONWLAN_8BIT_OFFSET, VAP_NONWLAN_8BIT_MASK, 0, 8, 4 },
		[CTP_FAST_ETH_WAN]
		{ GSW_LOGICAL_PORT_8BIT_WLAN, 8, 8, 8, CQE_LU_MODE2, SWDEV_DIS,
			VAP_8BIT_OFFSET, VAP_8BIT_MASK_32, 8, 7, 4 },
		[CTP_FAST_ETH_LAN]
		{ GSW_LOGICAL_PORT_8BIT_WLAN, 2, 2, 2, CQE_LU_MODE2, SWDEV_DIS,
			VAP_8BIT_OFFSET, VAP_8BIT_MASK_32, 8, 7, 4 },
		[CTP_FAST_ETH_LAN_EXP]
		{ GSW_LOGICAL_PORT_8BIT_WLAN, 8, 8, 8, CQE_LU_MODE2, SWDEV_DIS,
			VAP_8BIT_OFFSET, VAP_8BIT_MASK_32, 8, 7, 4 },
		[CTP_VUNI]
		{ GSW_LOGICAL_PORT_8BIT_WLAN, 2, 2, 1, CQE_LU_MODE2, SWDEV_EN,
			VAP_8BIT_OFFSET, VAP_8BIT_MASK_32, 8, 7, 4},
		[CTP_FAST_WLAN]
		{ GSW_LOGICAL_PORT_8BIT_WLAN, 16, 16, 16, CQE_LU_MODE2, SWDEV_DIS,
			VAP_8BIT_OFFSET, VAP_8BIT_MASK_32, 8, 7, 4 },
		[CTP_FAST_DSL]
		{ GSW_LOGICAL_PORT_8BIT_WLAN, 1, 16, 1, CQE_LU_MODE2, SWDEV_DIS,
			VAP_8BIT_OFFSET, VAP_4BIT_MASK_32, 3, 4, 4 },
		[CTP_DIRECTPATH]
		{ GSW_LOGICAL_PORT_8BIT_WLAN, 4, 4, 4, CQE_LU_MODE2, SWDEV_DIS,
			VAP_8BIT_OFFSET, VAP_8BIT_MASK_32, 8, 7, 4 },
		[CTP_FAST_WLAN_EXT]
		{ GSW_LOGICAL_PORT_9BIT_WLAN, 8, 8, 8, CQE_LU_MODE2, SWDEV_DIS,
			VAP_9BIT_OFFSET, VAP_9BIT_MASK_32, 9, 6, 4 },
		[CTP_DOCSIS]
		{ GSW_LOGICAL_PORT_DOCSIS, 36, 36, 36, CQE_LU_MODE4, SWDEV_DIS,
			VAP_NONWLAN_6BIT_OFFSET, VAP_NONWLAN_6BIT_MASK, 0, 6, 0 }
	}
};

const struct ctp_assign *dp_gsw_get_ctp_assign(int inst, int flags)
{
	struct inst_info *inst_info = get_dp_prop_info(inst);
	int i;

	if (inst_info->type == GSWIP31_TYPE ||
	    inst_info->type == GSWIP32_TYPE) {
		i = inst_info->type;
	} else {
		pr_err("DPM: %s: unsupported type %d\n", __func__, inst_info->type);
		return NULL;
	}

	switch (flags & CTP_ASSIGN_MASK) {
	case DP_F_CPU:
		return &ctp_assign_info[i][CTP_CPU];
	case DP_F_GPON:
		return &ctp_assign_info[i][CTP_GPON];
	case DP_F_EPON:
		return &ctp_assign_info[i][CTP_EPON];
	case DP_F_GINT:
		return &ctp_assign_info[i][CTP_GINT];
	case DP_F_FAST_ETH_WAN:
		return &ctp_assign_info[i][CTP_FAST_ETH_WAN];
	case DP_F_FAST_ETH_LAN:
		return &ctp_assign_info[i][CTP_FAST_ETH_LAN];
	case DP_F_FAST_ETH_LAN | DP_F_ALLOC_EXPLICIT_SUBIFID:
		return &ctp_assign_info[i][CTP_FAST_ETH_LAN_EXP];
	case DP_F_VUNI:
		return &ctp_assign_info[i][CTP_VUNI];
	case DP_F_FAST_WLAN:
		return &ctp_assign_info[i][CTP_FAST_WLAN];
	case DP_F_FAST_WLAN_EXT:
		return &ctp_assign_info[i][CTP_FAST_WLAN_EXT];
	case DP_F_FAST_DSL:
		return &ctp_assign_info[i][CTP_FAST_DSL];
	case DP_F_DOCSIS:
		return &ctp_assign_info[i][CTP_DOCSIS];
	case DP_F_DIRECT:
		return &ctp_assign_info[i][CTP_DIRECTPATH];
	default:
		pr_err("DPM: %s: no matching device type %08x\n", __func__, flags);
		return NULL;
	}
}

static char *ctp_md_str(GSW_LogicalPortMode_t type)
{
	switch (type) {
	case GSW_LOGICAL_PORT_8BIT_WLAN:
		return "8BIT_WLAN";
	case GSW_LOGICAL_PORT_9BIT_WLAN:
		return "9BIT_WLAN";
	case GSW_LOGICAL_PORT_GPON:
		return "GPON";
	case GSW_LOGICAL_PORT_EPON:
		return "EPON";
	case GSW_LOGICAL_PORT_GINT:
		return "GINT";
	case GSW_LOGICAL_PORT_DOCSIS:
		return "DOCSIS";
	case GSW_LOGICAL_PORT_OTHER:
		return "OTHER";
	default:
		return "UNDEF";
	}
}

#if defined(CONFIG_DPM_DATAPATH_DBG)
static void dump_pmac_igcfg(u32 ep, GSW_PMAC_Ig_Cfg_t *igcfg)
{
	int i;

	if ((dp_dbg_flag & DP_DBG_FLAG_DBG)) {
		DP_INFO("\nPort %d igcfg pmac configuration:\n", ep);
		DP_DUMP("nPmacId=%d\n", igcfg->nPmacId);
		DP_DUMP("nTxDmaChanId=%d\n", igcfg->nTxDmaChanId);
		DP_DUMP("bErrPktsDisc=%d\n", igcfg->bErrPktsDisc);
		DP_DUMP("bPmapDefault=%d\n", igcfg->bPmapDefault);
		DP_DUMP("bPmapEna=%d\n", igcfg->bPmapEna);
		DP_DUMP("bClassDefault=%d\n", igcfg->bClassDefault);
		DP_DUMP("bClassEna=%d\n", igcfg->bClassEna);
		DP_DUMP("bSubIdDefault=%d\n", igcfg->eSubId);
		DP_DUMP("bSpIdDefault=%d\n", igcfg->bSpIdDefault);
		DP_DUMP("bPmacPresent=%d\n", igcfg->bPmacPresent);
		DP_DUMP("defPmacHdr=");

		for (i = 0; i < ARRAY_SIZE(igcfg->defPmacHdr); i++)
			DP_DUMP("0x%x ", igcfg->defPmacHdr[i]);

		DP_DUMP("\n");
	}
}

static void dump_pmac_egcfg(u32 ep, GSW_PMAC_Eg_Cfg_t *egcfg)
{
	if ((dp_dbg_flag & DP_DBG_FLAG_DBG)) {
		DP_DUMP("   %s Port %d egcfg pmac configuration: "
			"tc: [%d/%d], flow:[%d/%d]\n", __func__,
			ep, egcfg->nBslTrafficClass, MAX_TC,
			egcfg->nFlowIDMsb, MAX_FLOW);
		DP_DUMP("      nPmacId          = %2d, nRxDmaChanId = %2d, bRemL2Hdr   = %2d\n",
			egcfg->nPmacId, egcfg->nRxDmaChanId, egcfg->bRemL2Hdr);
		DP_DUMP("      numBytesRem      = %2d, bFcsEna      = %2d, bPmacEna    = %2d\n",
			egcfg->numBytesRem, egcfg->bFcsEna, egcfg->bPmacEna);
		DP_DUMP("      bResDW1Ena       = %2d, nResDW1      = %2d\n"
			"      bRes1DW0Ena      = %2d, nRes1DW0     = %2d\n",
			egcfg->bResDW1Enable, egcfg->nResDW1,
			egcfg->bRes1DW0Enable, egcfg->nRes1DW0);
		DP_DUMP("      bRes2DW0Ena      = %2d, nRes2DW0     = %2d\n",
			egcfg->bRes2DW0Enable, egcfg->nRes2DW0);
		DP_DUMP("      nTrafficClass    = %2d, bTCEnable    = %2d, nDestPortId = %2d\n",
			egcfg->nTrafficClass, egcfg->bTCEnable,
			egcfg->nDestPortId);
		DP_DUMP("      nBslTrafficClass = %2d, nFlowIDMsb   = %2d, bDecFlag    = %2d\n",
			egcfg->nBslTrafficClass, egcfg->nFlowIDMsb,
			egcfg->bDecFlag);
		DP_DUMP("      bEncFlag         = %2d, bMpe1Flag    = %2d, bMpe2Flag   = %2d\n",
			egcfg->bEncFlag, egcfg->bMpe1Flag, egcfg->bMpe2Flag);
	}
}
#endif

static void _update_pmac_ingress_cfg(int inst, u32 ep, dp_pmac_cfg_t *pmac_cfg,
				     GSW_PMAC_Ig_Cfg_t *igcfg)
{
	struct core_ops *gsw_ops = dp_port_prop[inst].ops[0];
	ingress_pmac_t ig_pmac = pmac_cfg->ig_pmac;
	int pmac_hdr = ffs(IG_PMAC_F_PMACHDR1), i;
	u32 flags = pmac_cfg->ig_pmac_flags;

	/* Read PMAC ingress config from GSWIP */
	gsw_ops->gsw_pmac_ops.Pmac_Ig_CfgGet(gsw_ops, igcfg);

	/* Update PMAC ingress config */
	if (flags & IG_PMAC_F_ERR_DISC)
		igcfg->bErrPktsDisc = ig_pmac.err_disc;

	if (flags & IG_PMAC_F_PRESENT)
		igcfg->bPmacPresent = ig_pmac.pmac;

	if (flags & IG_PMAC_F_SUBIF)
		igcfg->eSubId = ig_pmac.def_pmac_subifid;

	if (flags & IG_PMAC_F_SPID)
		igcfg->bSpIdDefault = ig_pmac.def_pmac_src_port;

	if (flags & IG_PMAC_F_CLASSENA)
		igcfg->bClassEna = ig_pmac.def_pmac_en_tc;

	if (flags & IG_PMAC_F_CLASS)
		igcfg->bClassDefault = ig_pmac.def_pmac_tc;

	if (flags & IG_PMAC_F_PMAPENA)
		igcfg->bPmapEna = ig_pmac.def_pmac_en_pmap;

	if (flags & IG_PMAC_F_PMAP)
		igcfg->bPmapDefault = ig_pmac.def_pmac_pmap;

	for (i = 0; i < ARRAY_SIZE(igcfg->defPmacHdr); i++) {
		if (flags & BIT(pmac_hdr++))
			igcfg->defPmacHdr[i] = ig_pmac.def_pmac_hdr[i];
	}

	/* Set PMAC ingress config to GSWIP */
	gsw_ops->gsw_pmac_ops.Pmac_Ig_CfgSet(gsw_ops, igcfg);

#if defined(CONFIG_DPM_DATAPATH_DBG)
	dump_pmac_igcfg(ep, igcfg);
#endif
}

static int update_pmac_ingress_cfg(int inst, u32 ep, dp_pmac_cfg_t *pmac_cfg)
{
	struct pmac_port_info *port_info;
	GSW_PMAC_Ig_Cfg_t *igcfg;
	struct dp_rx_ring *rx_ring;
	u8 cid, pid;
	int i, j;
	u16 nid;

	port_info = get_dp_port_info(inst, ep);
	igcfg = dp_kzalloc(sizeof(*igcfg), GFP_ATOMIC);
	if (!igcfg)
		return DP_FAILURE;

	/* Check for DC port */
	if (port_info->alloc_flags & DP_F_ACA) {
		if (!port_info->num_rx_ring) {
			kfree(igcfg);
			pr_err("DPM: %s no DMA chan: port %d ig pmac\n",
			       __func__, ep);
			return DP_FAILURE;
		}

		for (i = 0; i < port_info->num_rx_ring; i++) {
			rx_ring = &port_info->rx_ring[i];
			dp_dma_parse_id(rx_ring->out_dma_ch_to_gswip,
					&cid, &pid, &nid);
			igcfg->nPmacId = dp_get_pmac_id(inst, cid);
			for (j = 0; j < rx_ring->num_out_tx_dma_ch; j++) {
				igcfg->nTxDmaChanId = nid + j;
				_update_pmac_ingress_cfg(inst, ep, pmac_cfg,
							 igcfg);
			}
		}
	} else {
		/* Streaming port */
		dp_dma_parse_id(port_info->dma_chan, &cid, &pid, &nid);
		igcfg->nPmacId = dp_get_pmac_id(inst, cid);
		if (!port_info->num_dma_chan) {
			kfree(igcfg);
			pr_err("DPM: %s no DMA chan: port %d ig pmac\n",
			       __func__, ep);
			return DP_FAILURE;
		}

		for (i = 0; i < port_info->num_dma_chan; i++) {
			igcfg->nTxDmaChanId = nid + i;
			_update_pmac_ingress_cfg(inst, ep, pmac_cfg, igcfg);
		}
	}
	kfree(igcfg);
	return DP_SUCCESS;
}

static int update_pmac_egress_cfg(int inst, u32 ep, dp_pmac_cfg_t *pmac_cfg)
{
	struct core_ops *gsw_ops = dp_port_prop[inst].ops[0];
	struct {
		GSW_PMAC_Glbl_Cfg_t glbcfg;
		GSW_QoS_queuePort_t q_map;
		GSW_PMAC_Eg_Cfg_t egcfg;
		egress_pmac_t eg_pmac;
		u32 flags;
		int i;
		int j;
	} *p;

	p = dp_kzalloc(sizeof(*p), GFP_ATOMIC);
	if (!p)
		return DP_FAILURE;
	p->eg_pmac = pmac_cfg->eg_pmac;
	p->flags = pmac_cfg->eg_pmac_flags;
	p->q_map.nPortId = ep;
	if (gsw_ops->gsw_qos_ops.QoS_QueuePortGet(gsw_ops, &p->q_map)) {
		kfree(p);
		pr_err("DPM: %s fail to get egress PMAC ID\n", __func__);
		return DP_FAILURE;
	}
	p->egcfg.nPmacId = p->q_map.nRedirectPortId;
	p->egcfg.nDestPortId = ep;
	DP_DEBUG(DP_DBG_FLAG_DBG, "bProcFlagsSelect=%u\n",
		 p->egcfg.bProcFlagsSelect);

	if (gsw_ops->gsw_pmac_ops.Pmac_Gbl_CfgGet(gsw_ops, &p->glbcfg)) {
		kfree(p);
		pr_err("DPM: %s GSW_PMAC_GLBL_CFG_GET fail\n", __func__);
		return DP_FAILURE;
	}

	for (p->i = 0; p->i < MAX_TC; p->i++) {
		for (p->j = 0; p->j < MAX_FLOW; p->j++) {
			dp_memset(&p->egcfg, 0, sizeof(GSW_PMAC_Eg_Cfg_t));
			p->egcfg.nPmacId = p->q_map.nRedirectPortId;
			p->egcfg.nDestPortId = ep;
			p->egcfg.nBslTrafficClass = p->i;
			switch (p->glbcfg.eProcFlagsEgCfg) {
			/* Use reduced traffic class (saturated to 3)
			 * and flags (MPE1, MPE2)
			 */
			case GSW_PMAC_PROC_FLAGS_MIX:
				p->egcfg.bProcFlagsSelect = 1;
				p->egcfg.nTrafficClass = (p->i & 0xC) >> 2;
				p->egcfg.bMpe2Flag = (p->i & 0x2) >> 1;
				p->egcfg.bMpe1Flag = (p->i & 0x1);
				break;
			/* Use flags (MPE1, MPE2, DEC, ENC) */
			case GSW_PMAC_PROC_FLAGS_FLAG:
				p->egcfg.bProcFlagsSelect = 1;
				p->egcfg.bDecFlag = (p->i & 0x8) >> 3;
				p->egcfg.bEncFlag = (p->i & 0x4) >> 2;
				p->egcfg.bMpe2Flag = (p->i & 0x2) >> 1;
				p->egcfg.bMpe1Flag = (p->i & 0x1);
				break;
			/* Use traffic class as full 4 bits */
			default:
				p->egcfg.bProcFlagsSelect = 0;
				p->egcfg.nTrafficClass = p->i;
				break;
			}

			p->egcfg.nFlowIDMsb = p->j;

			gsw_ops->gsw_pmac_ops.Pmac_Eg_CfgGet(gsw_ops, &p->egcfg);

			if (p->flags & EG_PMAC_F_FCS)
				p->egcfg.bFcsEna = p->eg_pmac.fcs;

			if (p->flags & EG_PMAC_F_L2HDR_RM) {
				p->egcfg.bRemL2Hdr = p->eg_pmac.rm_l2hdr;
				p->egcfg.numBytesRem = p->eg_pmac.num_l2hdr_bytes_rm;
			}

			if (p->flags & EG_PMAC_F_PMAC)
				p->egcfg.bPmacEna = p->eg_pmac.pmac;

			if (p->flags & EG_PMAC_F_RXID)
				p->egcfg.nRxDmaChanId = p->eg_pmac.rx_dma_chan;

			if (p->flags & EG_PMAC_F_REDIREN)
				p->egcfg.bRedirEnable = p->eg_pmac.redir;

			if (p->flags & EG_PMAC_F_BSLSEG)
				p->egcfg.bBslSegmentDisable = p->eg_pmac.bsl_seg;

			if (p->flags & EG_PMAC_F_RESDW1EN)
				p->egcfg.bResDW1Enable = p->eg_pmac.res_endw1;

			if (p->flags & EG_PMAC_F_RESDW1)
				p->egcfg.nResDW1 = p->eg_pmac.res_dw1;

			if (p->flags & EG_PMAC_F_RES1DW0EN)
				p->egcfg.bRes1DW0Enable = p->eg_pmac.res1_endw0;

			if (p->flags & EG_PMAC_F_RES1DW0)
				p->egcfg.nRes1DW0 = p->eg_pmac.res1_dw0;

			if (p->flags & EG_PMAC_F_RES2DW0EN)
				p->egcfg.bRes2DW0Enable = p->eg_pmac.res2_endw0;

			if (p->flags & EG_PMAC_F_RES2DW0)
				p->egcfg.nRes2DW0 = p->eg_pmac.res2_dw0;

			if (p->flags & EG_PMAC_F_DECFLG)
				p->egcfg.bDecFlag = p->eg_pmac.dec_flag;

			if (p->flags & EG_PMAC_F_ENCFLG)
				p->egcfg.bEncFlag = p->eg_pmac.enc_flag;

			if (p->flags & EG_PMAC_F_MPE1FLG)
				p->egcfg.bMpe1Flag = p->eg_pmac.mpe1_flag;

			if (p->flags & EG_PMAC_F_MPE2FLG)
				p->egcfg.bMpe2Flag = p->eg_pmac.mpe2_flag;

			gsw_ops->gsw_pmac_ops.Pmac_Eg_CfgSet(gsw_ops, &p->egcfg);

#if defined(CONFIG_DPM_DATAPATH_DBG)
			dump_pmac_egcfg(ep, &p->egcfg);
#endif
		}
	}
	kfree(p);
	return DP_SUCCESS;
}

int dp_gsw_set_pmac(int inst, u32 ep, dp_pmac_cfg_t *pmac_cfg)
{
	if (!pmac_cfg) {
		pr_err("DPM: %s: pmac_cfg is NULL\n", __func__);
		return DP_FAILURE;
	}

	if (pmac_cfg->ig_pmac_flags) {
		if (update_pmac_ingress_cfg(inst, ep, pmac_cfg))
			return DP_FAILURE;
	}

	if (pmac_cfg->eg_pmac_flags) {
		if (update_pmac_egress_cfg(inst, ep, pmac_cfg))
			return DP_FAILURE;
	}

	return DP_SUCCESS;
}

/* FIXME: Need to support multiple instances */
int dp_gsw_set_parser(u8 flag, u8 cpu, u8 mpe1, u8 mpe2, u8 mpe3)
{
	struct core_ops *gsw_ops = dp_port_prop[0].ops[0];
	GSW_CPU_PortCfg_t cpu_cfg = {0};
	GSW_register_t reg = {0};

	if (gsw_ops->gsw_common_ops.CPU_PortCfgGet(gsw_ops, &cpu_cfg)) {
		pr_err("DPM: %s: GSW_CPU_PORT_CFG_GET fail\n", __func__);
		return DP_FAILURE;
	}

	DP_DEBUG(DP_DBG_FLAG_DBG, "Org flag=0x%x cpu=%d mpe1/2/3=%d/%d/%d\n",
		 flag, cpu_cfg.eNoMPEParserCfg, cpu_cfg.eMPE1ParserCfg,
		 cpu_cfg.eMPE2ParserCfg, cpu_cfg.eMPE1MPE2ParserCfg);
	DP_DEBUG(DP_DBG_FLAG_DBG, "New flag=0x%x cpu=%d mpe1/2/3=%d/%d/%d\n",
		 flag, cpu, mpe1, mpe2, mpe3);

	if (flag & F_MPE_NONE)
		cpu_cfg.eNoMPEParserCfg = cpu;

	if (flag & F_MPE1_ONLY)
		cpu_cfg.eMPE1ParserCfg = mpe1;

	if (flag & F_MPE2_ONLY)
		cpu_cfg.eMPE2ParserCfg = mpe2;

	if (flag & F_MPE1_MPE2)
		cpu_cfg.eMPE1MPE2ParserCfg = mpe3;

	if (gsw_ops->gsw_common_ops.CPU_PortCfgSet(gsw_ops, &cpu_cfg)) {
		pr_err("DPM: %s: GSW_CPU_PORT_CFG_SET fail\n", __func__);
		return DP_FAILURE;
	}

	/* FIXME: GSW should check this */
	/* GSWIP will write parser flags only when SLL memory is enable */
	reg.nRegAddr = ETHSW_CLK;
	gsw_ops->gsw_common_ops.RegisterGet(gsw_ops, &reg);
	reg.nData |= SLL3MEM_EN;
	gsw_ops->gsw_common_ops.RegisterSet(gsw_ops, &reg);

	dp_parser_info_refresh(cpu_cfg.eNoMPEParserCfg,
			       cpu_cfg.eMPE1ParserCfg,
			       cpu_cfg.eMPE2ParserCfg,
			       cpu_cfg.eMPE1MPE2ParserCfg,
			       0);

	return DP_SUCCESS;
}

/* FIXME: Need to support multiple instances */
int dp_gsw_get_parser(u8 *cpu, u8 *mpe1, u8 *mpe2, u8 *mpe3)
{
	struct core_ops *gsw_ops = dp_port_prop[0].ops[0];
	GSW_CPU_PortCfg_t *cpu_cfg;

	cpu_cfg = dp_kzalloc(sizeof(*cpu_cfg), GFP_ATOMIC);
	if (!cpu_cfg)
		return DP_FAILURE;
	if (gsw_ops->gsw_common_ops.CPU_PortCfgGet(gsw_ops, cpu_cfg)) {
		kfree(cpu_cfg);
		pr_err("DPM: %s: GSW_CPU_PORT_CFG_GET fail\n", __func__);
		return DP_FAILURE;
	}

	dp_parser_info_refresh(cpu_cfg->eNoMPEParserCfg,
			       cpu_cfg->eMPE1ParserCfg,
			       cpu_cfg->eMPE2ParserCfg,
			       cpu_cfg->eMPE1MPE2ParserCfg,
			       1);

	if (cpu) {
		*cpu = cpu_cfg->eNoMPEParserCfg;
		DP_DEBUG(DP_DBG_FLAG_DBG, "  cpu=%d\n", *cpu);
	}

	if (mpe1) {
		*mpe1 = cpu_cfg->eMPE1ParserCfg;
		DP_DEBUG(DP_DBG_FLAG_DBG, "  mpe1=%d\n", *mpe1);
	}

	if (mpe2) {
		*mpe2 = cpu_cfg->eMPE2ParserCfg;
		DP_DEBUG(DP_DBG_FLAG_DBG, "  mpe2=%d\n", *mpe2);
	}

	if (mpe3) {
		*mpe3 = cpu_cfg->eMPE1MPE2ParserCfg;
		DP_DEBUG(DP_DBG_FLAG_DBG, "  mpe3=%d\n", *mpe3);
	}

	kfree(cpu_cfg);
	return DP_SUCCESS;
}

/* FIXME: Need to support multiple instances */
int dp_gsw_reset_mib(int dev, u32 flag)
{
	struct core_ops *gsw_ops = dp_port_prop[0].ops[0];
	GSW_RMON_clear_t rmon_clear;

	rmon_clear.eRmonType = GSW_RMON_ALL_TYPE;
	if (gsw_ops->gsw_rmon_ops.RMON_Clear(gsw_ops, &rmon_clear)) {
		pr_err("DPM: Failed GSW_RMON_CLEAR for GSW_RMON_ALL_TYPE\n");
		return DP_FAILURE;
	}

	return DP_SUCCESS;
}

bool need_ctp_port_assign(int inst, u8 dp_port)
{
	if (dp_port)
		return true;

	/* Note for dp_port zeor (CPU Port)
	 * For PRX: CPU CTP should be zero and no need to change GSWIP API
	  *         default setting with CTP zero.
	 * For LGM, CPU CTP should be two since PMAC2 linked to PPv4.
	 *          So we need to change it from default zero to 2.
	 */
	if (!is_soc_prx(inst))
		return true;
	return false;
}

/* Return allocated ctp info */
struct gsw_itf *dp_gsw_assign_ctp(int inst, u8 ep, int bp_def, u32 flags,
				  struct dp_dev_data *data)
{
	struct pmac_port_info *port_info = get_dp_port_info(inst, ep);
	struct core_ops *gsw_ops = dp_port_prop[inst].ops[0];
	const struct ctp_assign *ctp_assign;
	GSW_CTP_portAssignment_t ctp = {0};
	struct ctp_ops *gsw_ctp;
	struct dp_cap *cap;

	gsw_ctp = &gsw_ops->gsw_ctp_ops;

	if (flags & DP_F_DEREGISTER) { /* de-registration */
		if (need_ctp_port_assign(inst, ep)) {
			DP_DEBUG(DP_DBG_FLAG_DBG, "Free CTP Port for ep=%d\n",
				 ep);
			ctp.nLogicalPortId = ep;
			ctp.eMode = itf_assign[ep].mode;
			ctp.nFirstCtpPortId = itf_assign[ep].start;
			ctp.nNumberOfCtpPort = itf_assign[ep].n;
			if (gsw_ctp->CTP_PortAssignmentFree(gsw_ops, &ctp))
				pr_err("DPM: %s %s:ep=%d blk=%d mode=%d\n",
					__func__, "CTP_PortAssignmentFree fail",
					ep, ctp.nNumberOfCtpPort, ctp.eMode);
		}
		/* return NULL for de-register case */
		return NULL;
	}

	/* registration */
	ctp_assign = dp_gsw_get_ctp_assign(inst, port_info->alloc_flags);
	if (!ctp_assign) {
		ctp_assign = dp_gsw_get_ctp_assign(inst, CTP_DEFAULT);
		if (!ctp_assign)
			return NULL;
	}

	if (data && data->max_ctp)
		ctp.nNumberOfCtpPort  = data->max_ctp;
	else
		ctp.nNumberOfCtpPort  = ctp_assign->max_ctp;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	if (data && data->flag_ops & DP_F_DEV_EMODE)
		ctp.eMode = data->emode; /* override default */
	else
#endif
		ctp.eMode = ctp_assign->emode;
	ctp.nBridgePortId = bp_def;
	ctp.nLogicalPortId = ep;

	if (need_ctp_port_assign(inst, ep)) {
		if (gsw_ctp->CTP_PortAssignmentAlloc(gsw_ops, &ctp)) {
			pr_err("DPM: %s %s: ep=%d blk size=%d mode=%s\n",
				__func__, "CTP_PortAssignmentAlloc fail:",
				ep, ctp.nNumberOfCtpPort,
				ctp_md_str(ctp_assign->emode));
			return NULL;
		}
	}
	DP_DEBUG(DP_DBG_FLAG_DBG, "Assign ep=%d with eMode=%d ctp_max:%d\n",
		 ep, ctp_assign->emode, ctp.nNumberOfCtpPort);

	itf_assign[ep].mode = ctp.eMode;
	itf_assign[ep].n = ctp.nNumberOfCtpPort;
	itf_assign[ep].start = ctp.nFirstCtpPortId;
	itf_assign[ep].end = ctp.nFirstCtpPortId + ctp.nNumberOfCtpPort - 1;
	itf_assign[ep].ep = ep;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	if (data && data->flag_ops & DP_F_DEV_LOOKUP_MODE) {
		/* override default */
		cbm_queue_map_entry_t lookup = {0};

		itf_assign[ep].cqe_mode = data->cqe_lu_md;
		port_info->cqe_lu_mode = data->cqe_lu_md;

		/* overridge CQM lookup mode which was set in port_platform_set(...) */
		lookup.ep = ep;
		if (CBM_OPS(inst, cqm_mode_table_set, dp_port_prop[inst].cbm_inst, &lookup,
			    data->cqe_lu_md,
			    CBM_QUEUE_MAP_F_MPE1_DONTCARE |
			    CBM_QUEUE_MAP_F_MPE2_DONTCARE)) {
			pr_err("DPM: %s: cqm_mode_table_set failed\n", __func__);
			return NULL;
		}
	}
#endif

	if (data && data->max_subif) {
		if (data->max_subif < ctp.nNumberOfCtpPort) {
			pr_err("DPM: Error: %s %d given max_subif(%d) < nCTP(%d)\n",
			      __func__, __LINE__, data->max_subif,
			      ctp.nNumberOfCtpPort);
			return NULL;
		}
		port_info->subif_max = data->max_subif;
	} else {
		port_info->subif_max =
			ctp_assign->max_subif >= ctp.nNumberOfCtpPort ?
			ctp_assign->max_subif : ctp.nNumberOfCtpPort;
	}

	port_info->ctp_max = ctp.nNumberOfCtpPort;

	cap = &get_dp_prop_info(inst)->cap;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	/* HW capaility have only 3 modes 8-bit, 9-bit or
	 * Non-WLAN mode(8-bit[GPON, EPON and G.INT] and 6-bit[DOCSIS])
	 */
	if (data && data->flag_ops & DP_F_DEV_EMODE) {
		/* tune vap_offset/mask */
		if (ctp.eMode == GSW_LOGICAL_PORT_8BIT_WLAN) {
			port_info->vap_offset = VAP_8BIT_OFFSET;
			port_info->vap_mask = VAP_8BIT_MASK_31;
		} else if (ctp.eMode == GSW_LOGICAL_PORT_9BIT_WLAN) {
			port_info->vap_offset = VAP_9BIT_OFFSET;
			port_info->vap_mask = VAP_9BIT_MASK_31;
		} else {
			/* GPON, EPON, G.INT take this mode */
			port_info->vap_offset = VAP_NONWLAN_8BIT_OFFSET;
			port_info->vap_mask = VAP_NONWLAN_8BIT_MASK;
		}
		/* override the default emode */
		port_info->gsw_mode = ctp.eMode;
	} else
#endif
	{
		port_info->vap_offset = ctp_assign->vap_offset;
		port_info->vap_mask = ctp_assign->vap_mask;
	}
	port_info->oob_subif_offset = ctp_assign->oob_subif_offset;
	port_info->oob_subif_size = ctp_assign->oob_subif_size;
	port_info->oob_class_size = ctp_assign->oob_class_size;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	port_info->swdev_en = ctp_assign->swdev_en;
#endif

	return &itf_assign[ep];
}

int dp_gsw_set_port_lu_md(int inst, u8 ep, u32 flags)
{
	struct pmac_port_info *port_info = get_dp_port_info(inst, ep);
	const struct ctp_assign *ctp_assign;

	ctp_assign = dp_gsw_get_ctp_assign(inst, port_info->alloc_flags);
	if (!ctp_assign) {
		ctp_assign = dp_gsw_get_ctp_assign(inst, CTP_DEFAULT);
		if (!ctp_assign)
			return DP_FAILURE;
	}

	port_info->cqe_lu_mode = ctp_assign->cqe_lu_md;
	port_info->gsw_mode = ctp_assign->emode;

	return DP_SUCCESS;
}

/* Allocate a bridge port with specified bridge ID (FID)
 * and hardcoded CPU port member
 */
int dp_gsw_alloc_bp(int inst, int ep, int subif_ix, int fid, int bp_member,
			    int flags)
{
	struct pmac_port_info *port_info = get_dp_port_info(inst, ep);
	struct core_ops *gsw_ops = dp_port_prop[inst].ops[0];
	struct dp_subif_info *subif_info;
	struct brdgport_ops *gsw_bp;
	struct local {
		GSW_BRIDGE_portAlloc_t bp_alloc;
		GSW_BRIDGE_portConfig_t bp_cfg;
	};
	struct local *l;
	int res;

	gsw_bp = &gsw_ops->gsw_brdgport_ops;

	l = dp_kzalloc(sizeof(*l), GFP_ATOMIC);
	if (!l)
		return DP_FAILURE;
	/* Allocate a free bridge port */
	if (gsw_bp->BridgePort_Alloc(gsw_ops, &l->bp_alloc)) {
		kfree(l);
		pr_err("DPM: %s GSW_BRIDGE_PORT_CONFIG_GET fail\n", __func__);
		return DP_FAILURE;
	}
	/* Set this new bridge port with specified bridge ID(fid)
	 * and bridge port map
	 */
	l->bp_cfg.nBridgePortId = l->bp_alloc.nBridgePortId;
	l->bp_cfg.nDestLogicalPortId = ep;
	l->bp_cfg.nDestSubIfIdGroup = subif_ix;
	/* By default disable src mac learning for registered
	 * non CPU bridge port with DP
	 */
	subif_info = get_dp_port_subif(port_info, subif_ix);
	if (subif_info->mac_learn_dis == DP_MAC_LEARNING_DIS)
		l->bp_cfg.bSrcMacLearningDisable = 1;
	else
		l->bp_cfg.bSrcMacLearningDisable = 0;
	l->bp_cfg.eMask = GSW_BRIDGE_PORT_CONFIG_MASK_MC_SRC_MAC_LEARNING |
			  GSW_BRIDGE_PORT_CONFIG_MASK_EGRESS_CTP_MAPPING |
			  GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP |
			  GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_ID;
	l->bp_cfg.nBridgeId = fid;
	if (!(flags & DP_SUBIF_BP_CPU_DISABLE))
		SET_BP_MAP(l->bp_cfg.nBridgePortMap, bp_member); /* CPU */
	if (gsw_bp->BridgePort_ConfigSet(gsw_ops, &l->bp_cfg)) {
		pr_err("DPM: %s GSW_BRIDGE_PORT_CONFIG_SET fail", __func__);
		pr_err("DPM:  to set bridge id(%d) and port map for bp= %d\n",
		       fid, l->bp_cfg.nBridgePortId);
		goto err;
	}

	/* Add this bridge port to CPU bridge port's member.
	 * Need read back first
	 */
	l->bp_cfg.nBridgePortId = bp_member;
	l->bp_cfg.eMask = GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP;
	if (gsw_bp->BridgePort_ConfigGet(gsw_ops, &l->bp_cfg)) {
		pr_err("DPM: %s GSW_BRIDGE_PORT_CONFIG_GET fail", __func__);
		pr_err("DPM:  to get bridge port's member for bridgeport=%d\n",
		       l->bp_cfg.nBridgePortId);
		goto err;
	}

	if (!(flags & DP_SUBIF_BP_CPU_DISABLE))
		SET_BP_MAP(l->bp_cfg.nBridgePortMap, l->bp_alloc.nBridgePortId);
	if (gsw_bp->BridgePort_ConfigSet(gsw_ops, &l->bp_cfg)) {
		pr_err("DPM: %s GSW_BRIDGE_PORT_CONFIG_SET fail", __func__);
		pr_err("DPM:  to set bridge port's member for bridgeport=%d\n",
		       l->bp_cfg.nBridgePortId);
		goto err;
	}
	res = l->bp_alloc.nBridgePortId;
	if (res >= 0) {
		dp_bp_tbl[inst][res].f_alloc = 1;
		n_dp_bp++;
	}
	kfree(l);
	return res;

err:
	gsw_bp->BridgePort_Free(gsw_ops, &l->bp_alloc);
	kfree(l);

	return DP_FAILURE;
}

/* Free one GSWIP bridge port
 * First read out its port member
 * according to this port memeber, from this deleing bridge port
 * from its member's member Free this bridge port
 */
int dp_gsw_free_bp(int inst, int bp)
{
	struct core_ops *gsw_ops = dp_port_prop[inst].ops[0];
	struct brdgport_ops *gsw_bp = NULL;
	int i, j;
	struct local {
		GSW_BRIDGE_portAlloc_t bp_alloc;
		GSW_BRIDGE_portConfig_t bp_cfg;
		GSW_BRIDGE_portConfig_t tmp2;
	};
	struct local *l = NULL;

	if (bp == CPU_BP) {
		pr_err("DPM: %s why free CPU_BP\n", __func__);
		return DP_SUCCESS;
	}

	gsw_bp = &gsw_ops->gsw_brdgport_ops;

	l = dp_kzalloc(sizeof(*l), GFP_ATOMIC);
	if (!l) {
		pr_err("DPM: %s dp_kzalloc\n", __func__);
		return DP_FAILURE;
	}

	/* Read out this delting bridge port's member */
	l->bp_cfg.nBridgePortId = bp;
	l->bp_cfg.eMask = GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP;
	if (gsw_bp->BridgePort_ConfigGet(gsw_ops, &l->bp_cfg)) {
		pr_err("DPM: %s GSW_BRIDGE_PORT_CONFIG_GET: %d\n",
		       __func__, bp);
		goto err;
	}

	/* Remove this delting bridgeport from other bridge port's member*/
	for (i = 0; i < ARRAY_SIZE(l->bp_cfg.nBridgePortMap); i++) {
		for (j = 0; j < 16; j++) {
			if (!(l->bp_cfg.nBridgePortMap[i] & BIT(j)))
				continue; /*not member bit set */
			dp_memset(l->tmp2.nBridgePortMap, 0,
				  sizeof(l->tmp2.nBridgePortMap));
			l->tmp2.eMask =
				GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP;
			l->tmp2.nBridgePortId = i * 16 + j;
			if (gsw_bp->BridgePort_ConfigGet(gsw_ops, &l->tmp2)) {
				pr_err("DPM: %s GSW_BRIDGE_PORT_CONFIG_GET fail\n",
				       __func__);
				goto exit;
			}

			UNSET_BP_MAP(l->tmp2.nBridgePortMap, bp);
			if (gsw_bp->BridgePort_ConfigSet(gsw_ops, &l->tmp2)) {
				pr_err("DPM: %s GSW_BRIDGE_PORT_CONFIG_SET fail\n",
				       __func__);
				goto exit;
			}
		}
	}
err:
	l->bp_alloc.nBridgePortId = bp;
	if (gsw_bp->BridgePort_Free(gsw_ops, &l->bp_alloc))
		pr_err("DPM: %s GSW_BRIDGE_PORT_FREE: %d\n", __func__, bp);
	else {
		dp_bp_tbl[inst][bp].f_alloc = 0;
		dp_bp_tbl[inst][bp].flag = 0;
		dp_bp_tbl[inst][bp].dev = 0;
		n_dp_bp--;
	}
exit:
	kfree(l);

	return DP_SUCCESS;
}

int dp_gsw_add_mac_entry(int bp, int fid, int inst, u8 *addr)
{
	struct core_ops *gsw_ops = dp_port_prop[inst].ops[0];
	GSW_MAC_tableAdd_t mac_tbl = {0};

	mac_tbl.nFId = fid;
	mac_tbl.nPortId = bp;
	mac_tbl.bStaticEntry = 1;
	SET_BP_MAP(mac_tbl.nPortMap, bp);
	dp_memcpy(mac_tbl.nMAC, addr, GSW_MAC_ADDR_LEN);
	if (gsw_ops->gsw_swmac_ops.MAC_TableEntryAdd(gsw_ops, &mac_tbl)) {
		pr_err("DPM: GSW_MAC_TABLE_ENTRY_ADD fail\n");
		return DP_FAILURE;
	}

	return DP_SUCCESS;
}

int dp_gsw_del_mac_entry(int bp, int fid, int inst, u8 *addr)
{
	struct core_ops *gsw_ops = dp_port_prop[inst].ops[0];
	GSW_MAC_tableQuery_t *mac_query;
	GSW_MAC_tableRemove_t *mac_tbl;
	struct swmac_ops *gsw_swmac;

	gsw_swmac = &gsw_ops->gsw_swmac_ops;
	mac_query = dp_kzalloc(sizeof(*mac_query), GFP_ATOMIC);
	if (!mac_query)
		return DP_FAILURE;
	mac_query->nFId = fid;
	dp_memcpy(mac_query->nMAC, addr, GSW_MAC_ADDR_LEN);
	if (gsw_swmac->MAC_TableEntryQuery(gsw_ops, mac_query)) {
		kfree(mac_query);
		pr_err("DPM: GSW_MAC_TABLE_ENTRY_QUERY fail\n");
		return DP_FAILURE;
	}

	mac_tbl = dp_kzalloc(sizeof(*mac_tbl), GFP_ATOMIC);
	if (!mac_tbl) {
		kfree(mac_query);
		return DP_FAILURE;
	}
	mac_tbl->nFId = fid;
	dp_memcpy(mac_tbl->nMAC, addr, GSW_MAC_ADDR_LEN);
	if (gsw_swmac->MAC_TableEntryRemove(gsw_ops, mac_tbl)) {
		kfree(mac_query);
		kfree(mac_tbl);
		pr_err("DPM: GSW_MAC_TABLE_ENTRY_REMOVE fail\n");
		return DP_FAILURE;
	}

	kfree(mac_query);
	kfree(mac_tbl);
	return DP_SUCCESS;
}

int dp_gsw_dis_cpu_vlan_md(int inst)
{
	struct core_ops *gsw_ops = dp_port_prop[inst].ops[0];
	GSW_QoS_portRemarkingCfg_t *cfg;

	cfg = dp_kzalloc(sizeof(*cfg), GFP_ATOMIC);
	if (!cfg)
		return DP_FAILURE;

	if (gsw_ops->gsw_qos_ops.QoS_PortRemarkingCfgGet(gsw_ops, cfg)) {
		kfree(cfg);
		pr_err("DPM: GSW_QOS_PORT_REMARKING_CFG_GET fail\n");
		return DP_FAILURE;
	}

	cfg->bPCP_EgressRemarkingEnable = LTQ_FALSE;
	if (gsw_ops->gsw_qos_ops.QoS_PortRemarkingCfgSet(gsw_ops, cfg)) {
		kfree(cfg);
		pr_err("DPM: GSW_QOS_PORT_REMARKING_CFG_SET fail\n");
		return DP_FAILURE;
	}

	kfree(cfg);
	return DP_SUCCESS;
}

int dp_gsw_set_pmapper(int inst, int bp, int ep,
		       struct dp_pmapper *mapper, u32 flag)
{
	struct pmac_port_info *port_info = get_dp_port_info(inst, ep);
	struct core_ops *gsw_ops = dp_port_prop[inst].ops[0];
	GSW_BRIDGE_portConfig_t *bp_cfg;
	struct brdgport_ops *gsw_bp;
	int i, ctp, idx = 0;

	bp_cfg = dp_kzalloc(sizeof(*bp_cfg), GFP_ATOMIC);
	if (!bp_cfg)
		return DP_FAILURE;
	gsw_bp = &gsw_ops->gsw_brdgport_ops;

	bp_cfg->ePmapperMappingMode = mapper->mode;
	bp_cfg->nDestLogicalPortId = ep;
	bp_cfg->nBridgePortId = bp;
	bp_cfg->bPmapperEnable = 1;

	DP_DEBUG(DP_DBG_FLAG_DBG,
		 "Set pmapper bp %d inst %d ep %d\n",
		 bp, inst, ep);

	/* Copy subif information into pmapper list */
	if (mapper->def_ctp != DP_PMAPPER_DISCARD_CTP)
		ctp = GET_VAP(mapper->def_ctp, port_info->vap_offset,
			      port_info->vap_mask);
	else
		ctp = PMAPPER_DISC_CTP;
	bp_cfg->sPmapper.nDestSubIfIdGroup[0] = ctp;

	for (i = 0; i < DP_PMAP_PCP_NUM; i++) {
		if (mapper->pcp_map[i] != DP_PMAPPER_DISCARD_CTP)
			ctp = GET_VAP(mapper->pcp_map[i],
				      port_info->vap_offset,
				      port_info->vap_mask);
		else
			ctp = PMAPPER_DISC_CTP;

		bp_cfg->sPmapper.nDestSubIfIdGroup[++idx] = ctp;
	}

	for (i = 0; i < DP_PMAP_DSCP_NUM; i++) {
		if (mapper->dscp_map[i] != DP_PMAPPER_DISCARD_CTP)
			ctp = GET_VAP(mapper->dscp_map[i],
				      port_info->vap_offset,
				      port_info->vap_mask);
		else
			ctp = PMAPPER_DISC_CTP;
		bp_cfg->sPmapper.nDestSubIfIdGroup[++idx] = ctp;
	}
	bp_cfg->eMask = GSW_BRIDGE_PORT_CONFIG_MASK_EGRESS_CTP_MAPPING;

	DP_DEBUG(DP_DBG_FLAG_DBG,
		 "GSW_BRIDGE_PORT_CONFIG_SET mode %d enable %d eMask 0x%x\n",
		 bp_cfg->ePmapperMappingMode, bp_cfg->bPmapperEnable,
		 bp_cfg->eMask);

	if (gsw_bp->BridgePort_ConfigSet(gsw_ops, bp_cfg)) {
		kfree(bp_cfg);
		pr_err("DPM: %s GSW_BRIDGE_PORT_CONFIG_SET fail\n", __func__);
		return DP_FAILURE;
	}
	kfree(bp_cfg);
	return DP_SUCCESS;
}

int dp_gsw_get_pmapper(int inst, int bp, int ep, struct dp_pmapper *mapper,
		       u32 flag)
{
	struct pmac_port_info *info = get_dp_port_info(inst, ep);
	struct core_ops *gsw_ops = dp_port_prop[inst].ops[0];
	GSW_BRIDGE_portConfig_t *bp_cfg;
	struct brdgport_ops *gsw_bp;
	struct hal_priv *priv;
	int i, idx = 0;
	u16 dest;

	bp_cfg = dp_kzalloc(sizeof(*bp_cfg), GFP_ATOMIC);
	if (!bp_cfg)
		return DP_FAILURE;
	priv = (struct hal_priv *)dp_port_prop[inst].priv_hal;
	gsw_bp = &gsw_ops->gsw_brdgport_ops;
	bp_cfg->nBridgePortId = bp;
	bp_cfg->nDestLogicalPortId = ep;
	bp_cfg->eMask = GSW_BRIDGE_PORT_CONFIG_MASK_EGRESS_CTP_MAPPING;
	DP_DEBUG(DP_DBG_FLAG_DBG, "Get bp %d inst %d ep %d\n",
		 bp, inst, ep);

	if (gsw_bp->BridgePort_ConfigGet(gsw_ops, bp_cfg)) {
		kfree(bp_cfg);
		pr_err("DPM: %s GSW_BRIDGE_PORT_CONFIG_GET fail\n", __func__);
		return DP_FAILURE;
	}

	DP_DEBUG(DP_DBG_FLAG_DBG,
		 "%s GSW_BRIDGE_PORT_CONFIG_GET mode %d enable %d\n",
		 __func__, bp_cfg->ePmapperMappingMode, bp_cfg->bPmapperEnable);

	if (!bp_cfg->bPmapperEnable) {
		kfree(bp_cfg);
		pr_err("DPM: %s pmapper not enabled\n", __func__);
		return DP_FAILURE;
	}

	mapper->pmapper_id = bp_cfg->sPmapper.nPmapperId;
	mapper->mode = bp_cfg->ePmapperMappingMode;

	dest = bp_cfg->sPmapper.nDestSubIfIdGroup[0];
	if (dest == PMAPPER_DISC_CTP)
		mapper->def_ctp = DP_PMAPPER_DISCARD_CTP;
	else
		mapper->def_ctp = SET_VAP(dest, info->vap_offset,
					  info->vap_mask);
	for (i = 0; i < DP_PMAP_PCP_NUM; i++) {
		dest = bp_cfg->sPmapper.nDestSubIfIdGroup[++idx];
		if (dest == PMAPPER_DISC_CTP)
			mapper->pcp_map[i] = DP_PMAPPER_DISCARD_CTP;
		else
			mapper->pcp_map[i] = SET_VAP(dest, info->vap_offset,
						     info->vap_mask);
	}
	for (i = 0; i < DP_PMAP_DSCP_NUM; i++) {
		dest = bp_cfg->sPmapper.nDestSubIfIdGroup[++idx];
		if (dest == PMAPPER_DISC_CTP)
			mapper->dscp_map[i] = DP_PMAPPER_DISCARD_CTP;
		else
			mapper->dscp_map[i] = SET_VAP(dest, info->vap_offset,
						      info->vap_mask);
	}
	kfree(bp_cfg);
	return DP_SUCCESS;
}

int dp_gsw_alloc_meter(int inst, int *meterid, int flag)
{
	struct core_ops *gsw_ops =  dp_port_prop[inst].ops[0];
	GSW_QoS_meterCfg_t meter_cfg = {0};
	struct qos_ops *gsw_qos;

	if (!meterid) {
		pr_err("DPM: %s meterid NULL\n", __func__);
		return DP_FAILURE;
	}

	gsw_qos = &gsw_ops->gsw_qos_ops;

	if (flag == DP_F_DEREGISTER && *meterid >= 0) {
		meter_cfg.nMeterId = *meterid;
		if (gsw_qos->QOS_MeterFree(gsw_ops, &meter_cfg)) {
			pr_err("DPM: %s GSW_QOS_METER_FREE fail\n", __func__);
			return DP_FAILURE;
		}

		return DP_SUCCESS;
	}

	if (gsw_qos->QOS_MeterAlloc(gsw_ops, &meter_cfg)) {
		pr_err("DPM: %s GSW_QOS_METER_ALLOC fail\n", __func__);
		*meterid = -1;
		return DP_FAILURE;
	}

	*meterid = meter_cfg.nMeterId;

	return DP_SUCCESS;
}

static int dp_set_col_mark(struct net_device *dev,
			   struct dp_meter_cfg *mtr_cfg, int flag,
			   struct dp_meter_subif *mtr_subif)
{
	struct core_ops *gsw_ops = dp_port_prop[mtr_subif->inst].ops[0];
	struct local {
		GSW_BRIDGE_portConfig_t bp_cfg;
		GSW_CTP_portConfig_t ctp_cfg;
	};
	struct local *l;
	struct pmac_port_info *port_info;
	struct brdgport_ops *gsw_bp;
	struct ctp_ops *gsw_ctp;

	if (!mtr_cfg || !mtr_subif) {
		pr_err("DPM: %s mtr_cfg/mtr_subif is NULL\n", __func__);
		return DP_FAILURE;
	}
	l = dp_kzalloc(sizeof(*l), GFP_ATOMIC);
	if (!l)
		return DP_FAILURE;
	gsw_ctp = &gsw_ops->gsw_ctp_ops;
	gsw_bp = &gsw_ops->gsw_brdgport_ops;

	/* CTP port */
	if (flag & DP_METER_ATTACH_CTP) {
		if (mtr_subif->subif.flag_pmapper) {
			kfree(l);
			pr_err("DPM: %s Cannot use CTP when pmapper is enable\n",
			       __func__);
			return DP_FAILURE;
		}

		port_info = get_dp_port_info(mtr_subif->subif.inst,
					     mtr_subif->subif.port_id);
		l->ctp_cfg.nLogicalPortId = mtr_subif->subif.port_id;
		l->ctp_cfg.nSubIfIdGroup  = GET_VAP(mtr_subif->subif.subif,
						    port_info->vap_offset,
						    port_info->vap_mask);
		if (gsw_ctp->CTP_PortConfigGet(gsw_ops, &l->ctp_cfg)) {
			kfree(l);
			pr_err("DPM: %s GSW_CTP_PORT_CONFIG_GET fail\n", __func__);
			return DP_FAILURE;
		}

		if (mtr_cfg->dir == DP_DIR_INGRESS) {
			l->ctp_cfg.eMask = GSW_CTP_PORT_CONFIG_INGRESS_MARKING;
			l->ctp_cfg.eIngressMarkingMode = mtr_cfg->mode;
		} else if (mtr_cfg->dir == DP_DIR_EGRESS) {
			if (mtr_cfg->mode != DP_INTERNAL) {
				l->ctp_cfg.eMask =
				    GSW_CTP_PORT_CONFIG_EGRESS_MARKING_OVERRIDE;
				l->ctp_cfg.bEgressMarkingOverrideEnable = 1;
				l->ctp_cfg.eEgressMarkingModeOverride =
								  mtr_cfg->mode;
			} else {
				kfree(l);
				DP_DEBUG(DP_DBG_FLAG_PAE,
					 "%s invalid mode for egress\n",
					 __func__);
				return DP_FAILURE;
			}
		} else {
			kfree(l);
			return DP_FAILURE;
		}

		if (gsw_ctp->CTP_PortConfigSet(gsw_ops, &l->ctp_cfg)) {
			kfree(l);
			pr_err("DPM: %s GSW_CTP_PORT_CONFIG_SET fail\n", __func__);
			return DP_FAILURE;
		}
	}

	/* Bridge port */
	if (flag & DP_METER_ATTACH_BRPORT) {
		if (!mtr_subif->subif.flag_bp) {
			kfree(l);
			pr_err("DPM: %s flag_bp value 0\n", __func__);
			return DP_FAILURE;
		}

		l->bp_cfg.nBridgePortId = mtr_subif->subif.bport;
		if (gsw_bp->BridgePort_ConfigGet(gsw_ops, &l->bp_cfg)) {
			kfree(l);
			pr_err("DPM: %s GSW_BRIDGE_PORT_CONFIG_GET fail\n",
			       __func__);
			return DP_FAILURE;
		}

		if (mtr_cfg->dir == DP_DIR_EGRESS) {
			pr_err("DPM: %s no egress color marking for bridge port\n",
			       __func__);
			kfree(l);
			return DP_FAILURE;
		} else if (mtr_cfg->dir == DP_DIR_INGRESS) {
			l->bp_cfg.eMask =
				GSW_BRIDGE_PORT_CONFIG_MASK_INGRESS_MARKING;
			l->bp_cfg.eIngressMarkingMode = mtr_cfg->mode;
		} else {
			kfree(l);
			pr_err("DPM: %s invalid color mark dir\n", __func__);
			return DP_FAILURE;
		}

		if (gsw_bp->BridgePort_ConfigSet(gsw_ops, &l->bp_cfg)) {
			kfree(l);
			pr_err("DPM: %s failed GSW_BRIDGE_PORT_CONFIG_SET\n",
			       __func__);
			return DP_FAILURE;
		}
	}
	kfree(l);
	return DP_SUCCESS;
}

struct dp_meter_pce_info {
	struct dp_pce_blk_info pce_meter_blk;
	int pce_index;
};

struct dp_meter_pce_info meter_pce_info = {
	.pce_meter_blk = {
		.info = {
			.subblk_size = 1,
			.subblk_name = "dp_action_meter_1",
		},
		.region = GSW_PCE_RULE_COMMMON,
	},
};

int dp_gsw_add_meter(struct net_device *dev,  struct dp_meter_cfg  *mtr_cfg,
		     int flag, struct dp_meter_subif *mtr_subif)
{
	struct core_ops *gsw_ops = dp_port_prop[mtr_subif->inst].ops[0];
	GSW_QoS_meterCfg_t meter_cfg = {0};
	GSW_BRIDGE_portConfig_t *bp_cfg = NULL;
	GSW_CTP_portConfig_t *ctp_cfg = NULL;
	GSW_BRIDGE_config_t *br_cfg = NULL;
	GSW_PCE_rule_t *pce_rule = NULL;
	struct tflow_ops *gsw_tflow;
	struct brdgport_ops *gsw_bp;
	struct brdg_ops *gsw_brdg;
	struct qos_ops *gsw_qos;
	struct ctp_ops *gsw_ctp;
	int ret = DP_FAILURE;

	if (!mtr_cfg || !mtr_subif) {
		pr_err("DPM: %s mtr_cfg/mtr_subif NULL\n", __func__);
		return ret;
	}

	if (flag & DP_COL_MARKING)
		return dp_set_col_mark(dev, mtr_cfg, flag, mtr_subif);

	switch (mtr_cfg->type) {
	case srTCM:
		meter_cfg.eMtrType = GSW_QOS_Meter_srTCM;
		break;
	case trTCM:
		meter_cfg.eMtrType = GSW_QOS_Meter_trTCM;
		break;
	default:
		pr_err("DPM: %s invalid meter type\n", __func__);
		return DP_FAILURE;
	}

	gsw_tflow = &gsw_ops->gsw_tflow_ops;
	gsw_bp = &gsw_ops->gsw_brdgport_ops;
	gsw_brdg = &gsw_ops->gsw_brdg_ops;
	gsw_qos = &gsw_ops->gsw_qos_ops;
	gsw_ctp = &gsw_ops->gsw_ctp_ops;

	meter_cfg.nColourBlindMode = mtr_cfg->col_mode;
	meter_cfg.nPiRate = METER_PIR(mtr_cfg->pir);
	meter_cfg.nRate = METER_CIR(mtr_cfg->cir);
	meter_cfg.nMeterId = mtr_cfg->meter_id;
	meter_cfg.nCbs = mtr_cfg->cbs;
	meter_cfg.nEbs = mtr_cfg->pbs;
	meter_cfg.bEnable = 1;

	if (gsw_qos->QoS_MeterCfgSet(gsw_ops, &meter_cfg)) {
		pr_err("DPM: %s GSW_QOS_METER_CFG_SET fail\n", __func__);
		return DP_FAILURE;
	}

	if (flag & DP_METER_ATTACH_PCE) {
		pce_rule = dp_kzalloc(sizeof(*pce_rule), GFP_ATOMIC);
		if (!pce_rule)
			goto err;

		/* Action setting */
		pce_rule->action.eMeterAction = GSW_PCE_ACTION_METER_1;
		pce_rule->action.nMeterId =  mtr_cfg->meter_id;
		meter_pce_info.pce_index =
			dp_pce_rule_add(&meter_pce_info.pce_meter_blk,
					pce_rule);
		if (meter_pce_info.pce_index == DP_FAILURE) {
			pr_err("DPM: %s failed to add pce_rule\n", __func__);
			goto err;
		}
	}

	/* CTP port */
	if (flag & DP_METER_ATTACH_CTP) {
		struct pmac_port_info *port_info;

		if (mtr_subif->subif.flag_pmapper) {
			pr_err("DPM: %s cannot use ctp when pmapper is enable\n",
			       __func__);
			goto err;
		}

		ctp_cfg = dp_kzalloc(sizeof(*ctp_cfg), GFP_ATOMIC);
		if (!ctp_cfg)
			goto err;

		port_info = get_dp_port_info(mtr_subif->subif.inst,
					     mtr_subif->subif.port_id);
		ctp_cfg->nLogicalPortId = mtr_subif->subif.port_id;
		ctp_cfg->nSubIfIdGroup  = GET_VAP(mtr_subif->subif.subif,
						  port_info->vap_offset,
						  port_info->vap_mask);
		if (gsw_ctp->CTP_PortConfigGet(gsw_ops, ctp_cfg)) {
			pr_err("DPM: %s GSW_CTP_PORT_CONFIG_GET fail\n", __func__);
			goto err;
		}

		if (mtr_cfg->dir == DP_DIR_EGRESS) {
			ctp_cfg->eMask = GSW_CTP_PORT_CONFIG_EGRESS_METER;
			ctp_cfg->bEgressMeteringEnable = 1;
			ctp_cfg->nEgressTrafficMeterId =  mtr_cfg->meter_id;
		} else if (mtr_cfg->dir == DP_DIR_INGRESS) {
			ctp_cfg->eMask = GSW_CTP_PORT_CONFIG_INGRESS_METER;
			ctp_cfg->bIngressMeteringEnable = 1;
			ctp_cfg->nIngressTrafficMeterId =  mtr_cfg->meter_id;
		} else {
			pr_err("DPM: %s invalid meter dir\n", __func__);
			goto err;
		}

		if (gsw_ctp->CTP_PortConfigSet(gsw_ops, ctp_cfg)) {
			pr_err("DPM: %s GSW_CTP_PORT_CONFIG_SET fail\n", __func__);
			goto err;
		}
	}

	/* Bridge port */
	if (flag & DP_METER_ATTACH_BRPORT) {
		if (!mtr_subif->subif.flag_bp) {
			pr_err("DPM: %s flag_bp value is 0\n", __func__);
			goto err;
		}

		bp_cfg = dp_kzalloc(sizeof(*bp_cfg), GFP_ATOMIC);
		if (!bp_cfg)
			goto err;

		bp_cfg->nBridgePortId = mtr_subif->subif.bport;
		if (mtr_cfg->dir == DP_DIR_EGRESS)
			bp_cfg->eMask =
				GSW_BRIDGE_PORT_CONFIG_MASK_EGRESS_SUB_METER;
		else
			bp_cfg->eMask =
				GSW_BRIDGE_PORT_CONFIG_MASK_INGRESS_METER;
		if (gsw_bp->BridgePort_ConfigGet(gsw_ops, bp_cfg)) {
			pr_err("DPM: %s GSW_BRIDGE_PORT_CONFIG_GET fail\n",
			       __func__);
			goto err;
		}

		if (mtr_cfg->dir == DP_DIR_EGRESS) {
			switch (mtr_cfg->dp_pce.flow) {
			case DP_UKNOWN_UNICAST:
				BP_CFG(bp_cfg,
				       GSW_BRIDGE_PORT_EGRESS_METER_UNKNOWN_UC,
				       mtr_cfg->meter_id, 1);
				break;
			case DP_MULTICAST:
				BP_CFG(bp_cfg,
				       GSW_BRIDGE_PORT_EGRESS_METER_MULTICAST,
				       mtr_cfg->meter_id, 1);
				break;
			case DP_BROADCAST:
				BP_CFG(bp_cfg,
				       GSW_BRIDGE_PORT_EGRESS_METER_BROADCAST,
				       mtr_cfg->meter_id, 1);
				break;
			default:
				BP_CFG(bp_cfg,
				       GSW_BRIDGE_PORT_EGRESS_METER_OTHERS,
				       mtr_cfg->meter_id, 1);
			}
		} else if (mtr_cfg->dir == DP_DIR_INGRESS) {
			bp_cfg->bIngressMeteringEnable = 1;
			bp_cfg->nIngressTrafficMeterId = mtr_cfg->meter_id;
		} else {
			pr_err("DPM: %s invalid meter dir\n", __func__);
			goto err;
		}

		if (gsw_bp->BridgePort_ConfigSet(gsw_ops, bp_cfg)) {
			pr_err("DPM: %s GSW_BRIDGE_PORT_CONFIG_SET fail\n",
			       __func__);
			goto err;
		}
	} else if (flag & DP_METER_ATTACH_BRIDGE) {
		br_cfg = dp_kzalloc(sizeof(*br_cfg), GFP_ATOMIC);
		if (!br_cfg)
			goto err;

		br_cfg->nBridgeId = mtr_subif->fid;
		if (gsw_brdg->Bridge_ConfigGet(gsw_ops, br_cfg)) {
			pr_err("DPM: %s GSW_BRIDGE_CONFIG_GET fail\n", __func__);
			goto err;
		}

		br_cfg->eMask = GSW_BRIDGE_CONFIG_MASK_SUB_METER;

		switch (mtr_cfg->dp_pce.flow) {
		case DP_UKNOWN_UNICAST:
			BR_CFG(br_cfg, GSW_BRIDGE_PORT_EGRESS_METER_UNKNOWN_UC,
			       mtr_cfg->meter_id, 1);
			break;
		case DP_MULTICAST:
			BR_CFG(br_cfg, GSW_BRIDGE_PORT_EGRESS_METER_MULTICAST,
			       mtr_cfg->meter_id, 1);
			break;
		case DP_BROADCAST:
			BR_CFG(br_cfg, GSW_BRIDGE_PORT_EGRESS_METER_BROADCAST,
			       mtr_cfg->meter_id, 1);
			break;
		default:
			pr_err("DPM: %s meter flow invalid\n", __func__);
			goto err;
		}

		if (gsw_brdg->Bridge_ConfigSet(gsw_ops, br_cfg)) {
			pr_err("DPM: %s GSW_BRIDGE_CONFIG_SET fail\n", __func__);
			goto err;
		}
	}

	ret = DP_SUCCESS;

err:
	kfree(br_cfg);
	kfree(bp_cfg);
	kfree(ctp_cfg);
	kfree(pce_rule);

	return ret;
}

int dp_gsw_del_meter(struct net_device *dev,  struct dp_meter_cfg  *mtr_cfg,
		     int flag, struct dp_meter_subif *mtr_subif)
{
	struct core_ops *gsw_ops = dp_port_prop[mtr_subif->inst].ops[0];
	GSW_BRIDGE_portConfig_t *bp_cfg = NULL;
	GSW_PCE_ruleDelete_t *pce_rule = NULL;
	GSW_CTP_portConfig_t *ctp_cfg = NULL;
	GSW_BRIDGE_config_t *br_cfg = NULL;
	struct brdgport_ops *gsw_bp;
	struct tflow_ops *gsw_tflow;
	struct brdg_ops *gsw_brdg;
	struct ctp_ops *gsw_ctp;
	int ret = DP_FAILURE;

	if (!mtr_cfg || !mtr_subif) {
		pr_err("DPM: %s mtr_cfg/mtr_subif NULL\n", __func__);
		return DP_FAILURE;
	}

	gsw_tflow = &gsw_ops->gsw_tflow_ops;
	gsw_bp = &gsw_ops->gsw_brdgport_ops;
	gsw_brdg = &gsw_ops->gsw_brdg_ops;
	gsw_ctp = &gsw_ops->gsw_ctp_ops;

	if (mtr_cfg->dir & DP_METER_ATTACH_PCE) {
		pce_rule = dp_kzalloc(sizeof(*pce_rule), GFP_ATOMIC);
		if (!pce_rule)
			goto err;

		pce_rule->nIndex = meter_pce_info.pce_index;
		if (dp_pce_rule_del(&meter_pce_info.pce_meter_blk, pce_rule))
			goto err;
	}

	if (flag & DP_METER_ATTACH_CTP) {
		struct pmac_port_info *port_info;

		if (mtr_subif->subif.flag_pmapper) {
			pr_err("DPM: %s flag_pmapper is set\n", __func__);
			goto err;
		}

		ctp_cfg = dp_kzalloc(sizeof(*ctp_cfg), GFP_ATOMIC);
		if (!ctp_cfg)
			goto err;

		port_info = get_dp_port_info(mtr_subif->subif.inst,
					     mtr_subif->subif.port_id);
		ctp_cfg->nLogicalPortId = mtr_subif->subif.port_id;
		ctp_cfg->nSubIfIdGroup = GET_VAP(mtr_subif->subif.subif,
						 port_info->vap_offset,
						 port_info->vap_mask);
		if (mtr_cfg->dir == DP_DIR_EGRESS) {
			ctp_cfg->nEgressTrafficMeterId =  mtr_cfg->meter_id;
		} else if (mtr_cfg->dir == DP_DIR_INGRESS) {
			ctp_cfg->nIngressTrafficMeterId =  mtr_cfg->meter_id;
		} else {
			pr_err("DPM: %s invalid meter dir\n", __func__);
			goto err;
		}

		if (gsw_ctp->CTP_PortConfigGet(gsw_ops, ctp_cfg)) {
			pr_err("DPM: %s GSW_CTP_PORT_CONFIG_GET fail\n", __func__);
			goto err;
		}

		if (mtr_cfg->dir == DP_DIR_EGRESS) {
			ctp_cfg->eMask = GSW_CTP_PORT_CONFIG_EGRESS_METER;
			ctp_cfg->bEgressMeteringEnable = 0;
		} else {
			ctp_cfg->eMask = GSW_CTP_PORT_CONFIG_INGRESS_METER;
			ctp_cfg->bIngressMeteringEnable = 0;
		}

		if (gsw_ctp->CTP_PortConfigSet(gsw_ops, ctp_cfg)) {
			pr_err("DPM: %s GSW_CTP_PORT_CONFIG_SET fail\n", __func__);
			goto err;
		}
	}

	if (flag & DP_METER_ATTACH_BRPORT) {
		if (!mtr_subif->subif.flag_bp) {
			pr_err("DPM: %s flag_bp is 0\n", __func__);
			goto err;
		}

		bp_cfg = dp_kzalloc(sizeof(*bp_cfg), GFP_ATOMIC);
		if (!bp_cfg)
			goto err;

		bp_cfg->nBridgePortId = mtr_subif->subif.bport;
		if (mtr_cfg->dir == DP_DIR_EGRESS)
			bp_cfg->eMask =
				GSW_BRIDGE_PORT_CONFIG_MASK_EGRESS_SUB_METER;
		else
			bp_cfg->eMask =
				GSW_BRIDGE_PORT_CONFIG_MASK_INGRESS_METER;
		if (gsw_bp->BridgePort_ConfigGet(gsw_ops, bp_cfg)) {
			pr_err("DPM: %s GSW_BRIDGE_PORT_CONFIG_GET fail\n",
			       __func__);
			goto err;
		}

		if (mtr_cfg->dir == DP_DIR_EGRESS) {
			switch (mtr_cfg->dp_pce.flow) {
			case DP_UKNOWN_UNICAST:
				BP_CFG(bp_cfg,
				       GSW_BRIDGE_PORT_EGRESS_METER_UNKNOWN_UC,
				       mtr_cfg->meter_id, 0);
				break;
			case DP_MULTICAST:
				BP_CFG(bp_cfg,
				       GSW_BRIDGE_PORT_EGRESS_METER_MULTICAST,
				       mtr_cfg->meter_id, 0);
				break;
			case DP_BROADCAST:
				BP_CFG(bp_cfg,
				       GSW_BRIDGE_PORT_EGRESS_METER_BROADCAST,
				       mtr_cfg->meter_id, 0);
				break;
			default:
				BP_CFG(bp_cfg,
				       GSW_BRIDGE_PORT_EGRESS_METER_OTHERS,
				       mtr_cfg->meter_id, 0);
			}
		} else if (mtr_cfg->dir == DP_DIR_INGRESS) {
			bp_cfg->bIngressMeteringEnable = 0;
		} else {
			pr_err("DPM: %s invalid meter dir\n", __func__);
			goto err;
		}

		if (gsw_bp->BridgePort_ConfigSet(gsw_ops, bp_cfg)) {
			pr_err("DPM: %s GSW_BRIDGE_PORT_CONFIG_SET fail\n",
			       __func__);
			goto err;
		}
	}
	if (flag & DP_METER_ATTACH_BRIDGE) {
		br_cfg = dp_kzalloc(sizeof(*br_cfg), GFP_ATOMIC);
		if (!br_cfg)
			goto err;

		br_cfg->nBridgeId = mtr_subif->fid;
		if (gsw_brdg->Bridge_ConfigGet(gsw_ops, br_cfg)) {
			pr_err("DPM: %s GSW_BRIDGE_CONFIG_GET fail\n", __func__);
			goto err;
		}

		br_cfg->eMask = GSW_BRIDGE_CONFIG_MASK_SUB_METER;
		switch (mtr_cfg->dp_pce.flow) {
		case DP_UKNOWN_UNICAST:
			BR_CFG(br_cfg,
			       GSW_BRIDGE_PORT_EGRESS_METER_UNKNOWN_UC,
			       mtr_cfg->meter_id, 0);
			break;
		case DP_MULTICAST:
			BR_CFG(br_cfg,
			       GSW_BRIDGE_PORT_EGRESS_METER_MULTICAST,
			       mtr_cfg->meter_id, 0);
			break;
		case DP_BROADCAST:
			BR_CFG(br_cfg,
			       GSW_BRIDGE_PORT_EGRESS_METER_BROADCAST,
			       mtr_cfg->meter_id, 0);
			break;
		default:
			pr_err("DPM: %s meter flow invalid\n", __func__);
			goto err;
		}

		if (gsw_brdg->Bridge_ConfigSet(gsw_ops, br_cfg)) {
			pr_err("DPM: %s GSW_BRIDGE_CONFIG_SET fail\n", __func__);
			goto err;
		}
	}

	ret = DP_SUCCESS;
err:
	kfree(br_cfg);
	kfree(bp_cfg);
	kfree(ctp_cfg);
	kfree(pce_rule);

	return ret;
}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
static int set_gsw_gpid_map(int inst, u8 ep, int gpid_base, int gpid_num,
			    int gpid_spl, u32 flags)
{
	struct core_ops *gsw_ops = dp_port_prop[inst].ops[0];
	GSW_LPID_to_GPID_Assignment_t lp_gp_assign = {0};
	GSW_GPID_to_LPID_Assignment_t gp_lp_assign = {0};
	struct gpid_ops *gsw_gpid;
	int i;

	gsw_gpid = &gsw_ops->gsw_gpid_ops;

	DP_DEBUG(DP_DBG_FLAG_REG, "gpid base=%d num=%d spl gpid=%d\n",
		 gpid_base, gpid_num, gpid_spl);

	lp_gp_assign.nLogicalPortId = ep;
	if (gsw_gpid->LpidToGpid_AssignmentGet(gsw_ops, &lp_gp_assign)) {
		pr_err("DPM: %s GSW_LPID_TO_GPID_ASSIGNMENT_GET fail:\n", __func__);
		pr_err("DPM:    lpid (%d) -> gpid (%d)\n", ep, gpid_base);
		return DP_FAILURE;
	}

	/* During de-register assign gpid same as lpid */
	if (flags & DP_F_DEREGISTER) {
		lp_gp_assign.nFirstGlobalPortId = ep;
		/* reset the setting */
		lp_gp_assign.nValidBits = GENMASK(2, 0);
		
	} else {
		lp_gp_assign.nFirstGlobalPortId = gpid_base;
	}

	lp_gp_assign.nNumberOfGlobalPort = gpid_num;
	
	/* Set lpid to gpid */
	if (gsw_gpid->LpidToGpid_AssignmentSet(gsw_ops, &lp_gp_assign)) {
		pr_err("DPM: %s GSW_LPID_TO_GPID_ASSIGNMENT_SET fail:\n", __func__);
		pr_err("DPM:    lpid (%d) -> gpid (%d)\n", ep, gpid_base);
		return DP_FAILURE;
	}

	/* Set gpid to lpid */
	_DP_DEBUG(DP_DBG_FLAG_REG, "   gpid (N) <-> lpid (M) table:\n");
	_DP_DEBUG(DP_DBG_FLAG_REG, "   ----------------------------\n");
	for (i = 0; i < gpid_num; i++) {
		gp_lp_assign.nGlobalPortId = gpid_base + i;
		if (gsw_gpid->GpidToLpid_AssignmentGet(gsw_ops,
						       &gp_lp_assign)) {
			pr_err("DPM: %s GSW_GPID_TO_LPID_ASSIGNMENT_SET fail:",
			       __func__);
			pr_err("DPM:    gpid (%d)\n", gp_lp_assign.nGlobalPortId);
			return DP_FAILURE;
		}

		if (flags & DP_F_DEREGISTER)
			gp_lp_assign.nLogicalPortId = 0;
		else
			gp_lp_assign.nLogicalPortId = ep;

		if (gsw_gpid->GpidToLpid_AssignmentSet(gsw_ops,
						       &gp_lp_assign)) {
			pr_err("DPM: %s GSW_GPID_TO_LPID_ASSIGNMENT_SET fail:",
			       __func__);
			pr_err("DPM:    gpid (%d) -> lpid (%d)\n",
			       gpid_base + i, ep);
			return DP_FAILURE;
		}

		_DP_DEBUG(DP_DBG_FLAG_REG, "      gpid (%d) <-> lpid (%d)\n",
			 gpid_base + i, ep);
	}

	if (gpid_spl <= 0)
		return DP_SUCCESS;

	/* Set special gpid to lpid */
	gp_lp_assign.nGlobalPortId = gpid_spl;
	if (gsw_gpid->GpidToLpid_AssignmentGet(gsw_ops, &gp_lp_assign)) {
		pr_err("DPM: %s GSW_GPID_TO_LPID_ASSIGNMENT_GET fail:", __func__);
		pr_err("DPM:    gpid (%d)\n", gpid_spl);
		return DP_FAILURE;
	}

	if (flags & DP_F_DEREGISTER)
		gp_lp_assign.nLogicalPortId = 0;
	else
		gp_lp_assign.nLogicalPortId = ep;

	if (gsw_gpid->GpidToLpid_AssignmentSet(gsw_ops, &gp_lp_assign)) {
		pr_err("DPM: %s GSW_GPID_TO_LPID_ASSIGNMENT_SET fail:", __func__);
		pr_err("DPM:    gpid (%d) -> lpid (%d)\n",
		       gpid_spl, gp_lp_assign.nLogicalPortId);
		return DP_FAILURE;
	}

	return DP_SUCCESS;
}

static int set_cqm_gpid_map(int inst, u8 ep, int gpid_base, int gpid_num,
			    int gpid_spl, u32 flags)
{
	struct cbm_gpid_lpid cbm_gpid = {0};
	int i = 0;

	cbm_gpid.cbm_inst = dp_port_prop[inst].cbm_inst;

	if (flags & DP_F_DEREGISTER)
		cbm_gpid.lpid = 0;
	else
		cbm_gpid.lpid = ep;

	cbm_gpid.gpid = gpid_base;

	for (i = 0; i < gpid_num; i++) {
		cbm_gpid.gpid = gpid_base + i;
		if (dp_cqm_gpid_lpid_map(inst, &cbm_gpid))
			return DP_FAILURE;
	}

	if (gpid_spl) {
		cbm_gpid.gpid = gpid_spl;
		if (dp_cqm_gpid_lpid_map(inst, &cbm_gpid))
			return DP_FAILURE;
	}

	return DP_SUCCESS;
}

int dp_gpid_free(int inst, u8 ep, u32 flags)
{
	struct pmac_port_info *port = get_dp_port_info(inst, ep);
	int gpid_base, gpid_num, gpid_spl;

	gpid_base = port->gpid_base;
	gpid_num = port->gpid_num;
	gpid_spl = port->gpid_spl;

	/* To del spl gpid for non-streaming port(hence vap == -1) */
	if (dp_del_pp_gpid(inst, ep, -1)) {
		pr_err("DPM: %s fail to del spl gpid for dport(%d)\n", __func__, ep);
		return DP_FAILURE;
	}

	if (set_gsw_gpid_map(inst, ep, gpid_base, gpid_num, gpid_spl, flags))
		return DP_FAILURE;

	if (set_cqm_gpid_map(inst, ep, gpid_base, gpid_num, gpid_spl, flags))
		return DP_FAILURE;

	if (free_gpid(inst, gpid_base, gpid_num, gpid_spl))
		return DP_FAILURE;

	return DP_SUCCESS;
}

int gpid_port_assign(int inst, u8 ep, struct dp_dev_data *data, u32 flags)
{
	struct pmac_port_info *port = get_dp_port_info(inst, ep);
	int gpid_base, gpid_num, i, gpid_spl = -1;
	const struct ctp_assign *ctp_assign;
	struct hal_priv *priv;

	priv = (struct hal_priv *)dp_port_prop[inst].priv_hal;
	if (!priv) {
		pr_err("DPM: %s priv is NULL\n", __func__);
		return DP_FAILURE;
	}

	flags |= port->alloc_flags;
	if (flags & DP_F_DEREGISTER) {
		if (dp_gpid_free(inst, ep, flags))
			return DP_FAILURE;
		return DP_SUCCESS;
	}

	ctp_assign = dp_gsw_get_ctp_assign(inst, port->alloc_flags);
	if (!ctp_assign)
		return DP_FAILURE;

	gpid_num = ctp_assign->max_gpid;
	if (data && data->max_gpid)
		gpid_num = data->max_gpid;
	gpid_base = alloc_gpid(inst, DP_DYN_GPID, gpid_num, ep);
	if (gpid_base == DP_FAILURE) {
		pr_err("DPM: %s fail to allocate %d gpid for ep %d\n",
		       __func__, gpid_num, ep);
		return DP_FAILURE;
	}

	if (!is_stream_port(flags)) {
		gpid_spl = alloc_gpid(inst, DP_SPL_GPID, 1, ep);
		if (gpid_spl == DP_FAILURE) {
			pr_err("DPM: %s fail to allocate spl gpid for ep %d\n",
			       __func__, ep);
			return DP_FAILURE;
		}
		DP_DEBUG(DP_DBG_FLAG_REG, "Alloc spl gpid %d for dpid: %d\n", gpid_spl, ep);

		if (ep != CPU_PORT) {
			/**
			 * For CPU port we dont config spl gpid here, we config at
			 * VOICE spl_conn register
			 */
			if (dp_add_pp_gpid(inst, ep, 0, gpid_spl, 1, 0)) {
				pr_err("DPM: %s fail to add spl gpid %d for ep %d\n",
				       __func__, gpid_spl, ep);
				return DP_FAILURE;
			}
			DP_DEBUG(DP_DBG_FLAG_REG, "Add spl gpid %d to PP\n", gpid_spl);
		}
		priv->gp_dp_map[gpid_spl].dpid = ep;
	}

	if (set_gsw_gpid_map(inst, ep, gpid_base, gpid_num, gpid_spl, flags))
		return DP_FAILURE;

	if (set_cqm_gpid_map(inst, ep, gpid_base, gpid_num, gpid_spl, flags))
		return DP_FAILURE;

	port->gpid_base = gpid_base;
	port->gpid_num = gpid_num;
	port->gpid_spl = gpid_spl;

	/* Update table */
	for (i = 0; i < gpid_num; i++)
		priv->gp_dp_map[gpid_base + i].dpid = ep;

	return DP_SUCCESS;
}
#endif

int dp_gsw_set_bp_attr(struct dp_bp_attr *bp_attr, int bp, uint32_t flag)
{
	struct core_ops *gsw_ops = dp_port_prop[bp_attr->inst].ops[0];
	GSW_BRIDGE_portConfig_t *bp_cfg;

	bp_cfg = dp_kzalloc(sizeof(*bp_cfg), GFP_ATOMIC);
	if (!bp_cfg)
		return DP_FAILURE;
	bp_cfg->nBridgePortId = bp;
	bp_cfg->eMask = GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP;

	if (gsw_ops->gsw_brdgport_ops.BridgePort_ConfigGet(gsw_ops, bp_cfg)) {
		kfree(bp_cfg);
		pr_err("DPM: %s GSW_BRIDGE_PORT_CONFIG_GET fail\n", __func__);
		return DP_FAILURE;
	}

	if (bp_attr->en)
		SET_BP_MAP(bp_cfg->nBridgePortMap, CPU_PORT);
	else
		UNSET_BP_MAP(bp_cfg->nBridgePortMap, CPU_PORT);

	if (gsw_ops->gsw_brdgport_ops.BridgePort_ConfigSet(gsw_ops, bp_cfg)) {
		kfree(bp_cfg);
		pr_err("DPM: %s GSW_BRIDGE_PORT_CONFIG_SET fail\n", __func__);
		return DP_FAILURE;
	}
	kfree(bp_cfg);
	return DP_SUCCESS;
}

#define Y GSW_DROP_PRECEDENCE_YELLOW
#define G GSW_DROP_PRECEDENCE_GREEN

GSW_QoS_colorRemarkingEntry_t color_remark[] = {
	{
		GSW_MARKING_PCP_8P0D,
		{0, 2, 4, 6, 8, 10, 12, 14, 1, 3, 5, 7, 9, 11, 13, 15},
	},
	{
		GSW_MARKING_PCP_7P1D,
		{0, 2, 4, 6, 10, 10, 12, 14, 1, 3, 5, 7, 9, 9, 13, 15},
	},
	{
		GSW_MARKING_PCP_6P2D,
		{0, 2, 6, 6, 10, 10, 12, 14, 1, 3, 5, 5, 9, 9, 13, 15},
	},
	{
		GSW_MARKING_PCP_5P3D,
		{2, 2, 6, 6, 10, 10, 12, 14, 1, 1, 5, 5, 9, 9, 13, 15},
	},
	{
		GSW_MARKING_DSCP_AF,
		{10, 18, 26, 34, 34, 34, 34, 34, 12, 20, 28, 36, 36, 36, 36, 36},
	},
};

GSW_QoS_colorMarkingEntry_t color_mark[] = {
	{
		GSW_MARKING_PCP_5P3D,
		{0, 0, 0, 0, 2, 2, 2, 2, 4, 4, 4, 4, 6, 6, 7, 7},
		{Y, Y, G, Y, Y, Y, G, Y, Y, Y, G, Y, G, Y, G, Y},
	},
	{
		GSW_MARKING_PCP_6P2D,
		{0, 0, 1, 1, 2, 2, 2, 2, 4, 4, 4, 4, 6, 6, 7, 7},
		{G, Y, G, Y, Y, Y, G, Y, Y, Y, G, Y, G, Y, G, Y},
	},
	{
		GSW_MARKING_PCP_7P1D,
		{0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4, 4, 6, 6, 7, 7},
		{G, Y, G, Y, G, Y, G, Y, Y, Y, G, Y, G, Y, G, Y},
	},
	{
		GSW_MARKING_PCP_8P0D,
		{0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7},
		{G, Y, G, Y, G, Y, G, Y, G, Y, G, Y, G, Y, G, Y},
	},
	{
		GSW_MARKING_DSCP_AF,
		{
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 2, 0,
			2, 0, 2, 0, 0, 0, 3, 0, 3, 0, 3, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0
		},
		{
			Y, Y, Y, Y, Y, Y, Y, Y, Y, Y, G, Y, Y, Y, Y, Y,
			Y, Y, G, Y, Y, Y, Y, Y, Y, Y, G, Y, Y, Y, Y, Y,
			Y, Y, G, Y, Y, Y, Y, Y, Y, Y, Y, Y, Y, Y, Y, Y,
			Y, Y, Y, Y, Y, Y, Y, Y, Y, Y, Y, Y, Y, Y, Y, Y
		},
	},
};

int dp_gsw_color_table_set(int inst)
{
	struct core_ops *gsw_ops = dp_port_prop[inst].ops[0];
	GSW_QoS_colorRemarkingEntry_t *col_remark;
	GSW_QoS_colorMarkingEntry_t *col_mark;
	struct qos_ops *gsw_qos;
	int remark_size = ARRAY_SIZE(color_remark);
	int mark_size = ARRAY_SIZE(color_mark);
	int i = 0;

	gsw_qos = &gsw_ops->gsw_qos_ops;
	col_remark = dp_kzalloc(sizeof(*col_remark), GFP_ATOMIC);
	if (!col_remark)
		return DP_FAILURE;
	col_mark = dp_kzalloc(sizeof(*col_mark), GFP_ATOMIC);
	if (!col_mark) {
		kfree(col_remark);
		return DP_FAILURE;
	}

	for (i = 0; i < remark_size; i++) {
		dp_memset(col_remark, 0, sizeof(GSW_QoS_colorRemarkingEntry_t));
		dp_memcpy(col_remark, &color_remark[i],
			  sizeof(GSW_QoS_colorRemarkingEntry_t));
		if (gsw_qos->QOS_ColorReMarkingTableSet(gsw_ops, col_remark)) {
			kfree(col_remark);
			kfree(col_mark);
			pr_err("DPM: %s: GSW_QOS_COLOR_REMARKING_CFG_SET failed\n",
			       __func__);
			return DP_FAILURE;
		}
	}

	for (i = 0; i < mark_size; i++) {
		dp_memset(col_mark, 0, sizeof(GSW_QoS_colorMarkingEntry_t));
		dp_memcpy(col_mark, &color_mark[i],
			  sizeof(GSW_QoS_colorMarkingEntry_t));
		if (gsw_qos->QOS_ColorMarkingTableSet(gsw_ops, col_mark)) {
			kfree(col_remark);
			kfree(col_mark);
			pr_err("DPM: %s: GSW_QOS_COLOR_REMARKING_CFG_SET failed\n",
			       __func__);
			return DP_FAILURE;
		}
	}

	kfree(col_remark);
	kfree(col_mark);
	return DP_SUCCESS;
}

int dp_get_gswip_cap(struct dp_hw_cap *cap, int flag)
{
	struct core_ops *gsw_ops;
	GSW_cap_t *parm;
	GSW_register_t *reg;

	gsw_ops = gsw_get_swcore_ops(0);
	if (!gsw_ops) {
		pr_err("DPM: %s swcore ops is NULL\n", __func__);
		return DP_FAILURE;
	}

	reg = dp_kzalloc(sizeof(*reg), GFP_ATOMIC);
	if (!reg)
		return DP_FAILURE;
	reg->nRegAddr = GSWIP_VER_REG_OFFSET;
	if (gsw_ops->gsw_common_ops.RegisterGet(gsw_ops, reg)) {
		kfree(reg);
		return DP_FAILURE;
	}

	if (reg->nData == 0x031) {
		cap->info.type = GSWIP31_TYPE;
		cap->info.ver = GSWIP31_VER;
	} else if (reg->nData == 0x032) {
		cap->info.type = GSWIP32_TYPE;
		cap->info.ver = GSWIP32_VER;
	}

	parm = dp_kzalloc(sizeof(*parm), GFP_ATOMIC);
	if (!parm) {
		kfree(reg);
		return DP_FAILURE;
	}
	/* Max number of CTP */
	parm->nCapType = GSW_CAP_TYPE_CTP;
	gsw_ops->gsw_common_ops.CapGet(gsw_ops, parm);
	cap->info.cap.max_num_subif = parm->nCap;

	/* Max number of Bridge Port */
	parm->nCapType = GSW_CAP_TYPE_BRIDGE_PORT;
	gsw_ops->gsw_common_ops.CapGet(gsw_ops, parm);
	cap->info.cap.max_num_bridge_port = parm->nCap;

	/* Max number of Physical Ethernet Port */
	parm->nCapType = GSW_CAP_TYPE_PORT;
	gsw_ops->gsw_common_ops.CapGet(gsw_ops, parm);
	cap->info.cap.max_eth_port = parm->nCap;

	/* Max number of Virtual Ethernet Port */
	parm->nCapType = GSW_CAP_TYPE_VIRTUAL_PORT;
	gsw_ops->gsw_common_ops.CapGet(gsw_ops, parm);
	cap->info.cap.max_virt_eth_port = parm->nCap;

	/* Total number of DP ports */
	cap->info.cap.max_num_dp_ports = cap->info.cap.max_eth_port +
					 cap->info.cap.max_virt_eth_port;

	cap->info.cap.max_num_learn_limit_port = GSWIP_LEARN_LIMIT_PORT_MAX;

	cap->info.cap.tx_hw_chksum = 0;
	cap->info.cap.rx_hw_chksum = 0;
	cap->info.cap.hw_ptp = 1;

	DP_DEBUG(DP_DBG_FLAG_REG,
		 "max_num_subif                     = %d\n"
		 "max_num_bridge_port               = %d\n"
		 "max_eth_port                      = %d\n"
		 "max_virt_eth_port                 = %d\n"
		 "max_num_dp_ports                  = %d\n",
		 cap->info.cap.max_num_subif,
		 cap->info.cap.max_num_bridge_port,
		 cap->info.cap.max_eth_port,
		 cap->info.cap.max_virt_eth_port,
		 cap->info.cap.max_num_dp_ports);

	kfree(reg);
	kfree(parm);
	return DP_SUCCESS;
}

/* API to check whether need queue map workaround:
 * Currently there is subif mode of 9-bit station ID in GSWIP, but no matched
 * CQM queue lookup mode for it. We need workaround to set bit8 to don't care
 */
bool subif_bit8_workaround(struct dp_subif_info *subif_info)
{
	if (!subif_info->port_info) {
		pr_err("DPM: subif_info->port_info NULL\n");
		return false;
	}
	if (!is_soc_lgm(subif_info->port_info->inst))
		return false;
	if (subif_info->port_info->gsw_mode != GSW_LOGICAL_PORT_9BIT_WLAN)
		return false;
	if ((subif_info->port_info->cqe_lu_mode != CQE_LU_MODE0) &&
	    (subif_info->port_info->cqe_lu_mode != CQE_LU_MODE2))
		return false;
	return true;
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP
