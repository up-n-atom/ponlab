// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2023, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 * Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 * Copyright 2012 - 2014 Lantiq Deutschland GmbH
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <net/datapath_api.h>
#include "../datapath.h"
#include "../datapath_instance.h"
#include "../datapath_pce.h"
#include "datapath_ppv4.h"
#include "datapath_misc.h"
#include "datapath_tx.h"
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
#include "datapath_ppv4_session.h"
#include "datapath_spl_conn.h"
#endif

#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
#include "datapath_switchdev.h"
#endif

#include <linux/if_vlan.h>
#include <linux/bitfield.h>
#include <linux/pp_api.h>

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

#define DP_GSWIP_CRC_DISABLE 1
#define DP_GSWIP_CRC_ENABLE 0
#define DUMP_BUF_SIZE 100

/* Regard DC rxout QOS queue/egp are shared by multiple DC. So we use CPU_PORT
 * as their dpid.
 * By the way, so far CQM driver does not support QOS queue yet since by default
 * it is in QOS bypass mode for DC RXout Ring path
 */
int shared_dc_deq_dpid = CPU_PORT;
static int dev_platform_set_aca_rxout_queue(int inst, u8 ep,
							    uint32_t flags);
static struct pon_spl_cfg pon_cfg[] = {
	{
		.flag = DP_F_DEREGISTER,
		.ingress = 0,
		.egress = 1,
		.crc_check = DP_GSWIP_CRC_ENABLE,
		.fcs_gen = GSW_CRC_PAD_INS_EN,
		.flow_ctrl = GSW_FLOW_RXTX,
		.link_sts = GSW_PORT_LINK_AUTO,
		.tx_spl_tag = TX_SPTAG_REMOVE,
		.rx_time_stamp = RX_TIME_NOTS,
		.rx_spl_tag = RX_SPTAG_INSERT,
		.rx_fcs = RX_FCS_REMOVE,
		.pmac = PMAC_2,
		.qid = 2
	},
	{
		.flag = DP_F_FAST_ETH_WAN | DP_F_GPON | DP_F_EPON,
		.ingress = 1,
		.egress = 1,
		.crc_check = DP_GSWIP_CRC_DISABLE,
		.fcs_gen = GSW_CRC_PAD_INS_DIS,
		.flow_ctrl = GSW_FLOW_OFF,
		.link_sts = GSW_PORT_LINK_UP,
		.tx_spl_tag = TX_SPTAG_REPLACE,
		.rx_time_stamp = RX_TIME_NO_INSERT,
		.rx_spl_tag = RX_SPTAG_NO_INSERT,
		.rx_fcs = 0,
		.pmac = PMAC_1,
		.qid = 28
	},
};

static void init_dma_desc_mask(void)
{
	struct dma_rx_desc_1 *dw1_rx = (struct dma_rx_desc_1 *)&rx_desc_mask[1];
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	struct dma_rx_desc_2 *dw2_rx = (struct dma_rx_desc_2 *)&rx_desc_mask[2];
#endif
	struct dma_rx_desc_3 *dw3_rx = (struct dma_rx_desc_3 *)&rx_desc_mask[3];

	struct dma_rx_desc_0 *dw0_tx = (struct dma_tx_desc_0 *)&tx_desc_mask[0];
	struct dma_rx_desc_1 *dw1_tx = (struct dma_tx_desc_1 *)&tx_desc_mask[1];

	dw1_rx->all = 0xFFFFFFFF;
	dw0_tx->all = 0;
	dw1_tx->all = 0;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	dw1_rx->field.pmac = 0;
	dw1_rx->field.pre_l2 = 0;
	dw1_tx->field.lro_type = 0x1;
	dw2_rx->field.byte_offset = 0;
	dw0_tx->field.dest_sub_if_id = 0xFFFF;
	dw1_tx->field.src_pool = 0xF;
	/* DW0 Bit16 is used for PON Multicast to differentiate US and DS traffic
	 * For PRX300 redirect bit is carried from GSWIP to CPU,
	 * so this fix is not required for PRX300
	 */
	dw0_tx->field.dw0bit16 = 1;
#elif IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	dw3_rx->field.byte_offset = 0;

	dw1_rx->field.dec = 0;
	dw1_rx->field.enc = 0;
	dw1_rx->field.mpe2 = 0;
	dw1_rx->field.mpe1 = 0;

	dw0_tx->field.dest_sub_if_id = 0x7FFF;
	dw1_tx->field.mpe1 = 0x1;
#endif
	dw3_rx->all = 0xFFFFFFFF;
	dw3_rx->field.own = 0;
	dw3_rx->field.c = 0;
	dw3_rx->field.sop = 0;
	dw3_rx->field.eop = 0;
	dw3_rx->field.dic = 0;

	dw0_tx->field.flow_id = 0xFF;
	dw1_tx->field.color = 0x3;
	dw1_tx->field.ep = 0xF;
}

static
void set_desc_pmac_bits(struct dma_tx_desc_0 *dw0,
			struct dma_tx_desc_1 *dw1, struct pmac_tx_hdr *pmac,
			u32 flags, int val)
{
	int i = 0;
	unsigned long bits = flags;

	for_each_set_bit(i, &bits, __bf_shf(DESC_PMAC_FLAG_END)) {
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
		if (BIT(i) == DESC_MPE1)
			dw1->field.mpe1 = val;
		if (BIT(i) == DESC_MPE2)
			dw1->field.mpe2 = val;
		if (BIT(i) == DESC_ENC)
			dw1->field.enc = val;
		if (BIT(i) == DESC_DEC)
			dw1->field.dec = val;
		if (BIT(i) == DESC_IGP)
			dw1->field.ip = val;
		if (BIT(i) == DESC_REDIR)
			dw0->field.redir = val;
		if (BIT(i) == P_IGP_MSB)
			pmac->igp_msb = val;
#elif IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
		if (BIT(i) == DESC_REDIR)
			dw1->field.redir = val;
		if (BIT(i) == DESC_PMAC)
			dw1->field.pmac = val;
		if (BIT(i) == DESC_PREL2)
			dw1->field.pre_l2 = val;
#endif
		if (BIT(i) == DESC_EGP)
			dw1->field.ep = val;
		if (BIT(i) == DESC_COLR)
			dw1->field.color = val;
		if (BIT(i) == DESC_CLASSID)
			dw1->field.classid = val;
		if (BIT(i) == DESC_DW0_ALL)
			dw0->all = val;
		if (BIT(i) == DESC_DW1_ALL)
			dw1->all = val;
		if (BIT(i) == P_INS)
			pmac->ins = val;
		if (BIT(i) == P_IGP_EGP)
			pmac->igp_egp = val;
		if (BIT(i) == P_CLS_EN)
			pmac->class_en = val;
		if (BIT(i) == P_TCP_CSUM)
			pmac->tcp_chksum = val;
		if (BIT(i) == P_PTP)
			pmac->ptp = val;
	}
}

static
void dma_pmac_templ(struct pmac_port_info *dp_info, int templ, u32 flags,
		    int val)
{
	struct dma_tx_desc_0 *dw0 =
		(struct dma_tx_desc_0 *)&dp_info->desc_dw_templ[0][templ];
	struct dma_tx_desc_1 *dw1 =
		(struct dma_tx_desc_1 *)&dp_info->desc_dw_templ[1][templ];
	struct pmac_tx_hdr *pmac = &dp_info->pmac_template[templ];

	set_desc_pmac_bits(dw0, dw1, pmac, flags, val);
}

static
void dma_mask_templ(struct pmac_port_info *dp_info, int templ, u32 flags,
		    int val)
{
	struct dma_tx_desc_0 *dw0 =
		(struct dma_rx_desc_0 *)&dp_info->desc_dw_mask[0][templ];
	struct dma_tx_desc_1 *dw1 =
		(struct dma_rx_desc_1 *)&dp_info->desc_dw_mask[1][templ];

	set_desc_pmac_bits(dw0, dw1, NULL, flags, val);
}

static
void set_dma_pmac_reins_templ(struct dma_tx_desc_0 *desc_0,
			      struct dma_tx_desc_1 *desc_1,
			      struct pmac_tx_hdr *pmac)

{
	u32 flags = 0;

	/* reset all descriptors dw0 and dw1 */
	desc_0->all &= tx_desc_mask[0];
	desc_1->all &= tx_desc_mask[1];
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	/* ClassID need to be there in DW0 for QoS Desc 31:28,
	 * this is common for all ports and desc formats
	 */
	desc_0->qos_fmt.classid = desc_1->field.classid;
	flags = P_INS | DESC_PMAC | DESC_PREL2 | DESC_COLR;
#elif IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	flags = P_INS | DESC_ENC;
#endif
	set_desc_pmac_bits(desc_0, desc_1, pmac, flags, 1);
}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)

/* Note:
 * final tx_dma0 = (tx_dma0 & dma0_mask_template) | dma0_template
 * final tx_dma1 = (tx_dma1 & dma1_mask_template) | dma1_template
 * final tx_pmac = pmac_template
 * Note: only instance zero need set dma/pmac fields
 */

static void init_dma_pmac_template(int portid, u32 flags)
{
	int i = 0, j = 0;
	struct pmac_port_info *dp_info = get_dp_port_info(0, portid);
	unsigned long bits = flags;

	dp_memset(dp_info->desc_dw_templ, 0, sizeof(dp_info->desc_dw_templ));
	dp_memset(dp_info->desc_dw_mask, 0, sizeof(dp_info->desc_dw_mask));
	dp_memset(dp_info->pmac_template, 0, sizeof(dp_info->pmac_template));

	for (i = 0; i < MAX_TEMPLATE; i++) {
		dma_mask_templ(dp_info, i, DESC_DW0_ALL | DESC_DW1_ALL,
			       0xFFFFFFFF);

		/* Set Redirect=1 for all templates */
		dma_pmac_templ(dp_info, i, DESC_REDIR | P_CLS_EN, 1);
		dma_pmac_templ(dp_info, i, (P_IGP_MSB | P_IGP_EGP), portid);
	}
	/* enable checksum offloading
	 * For template checksum, PMAC=1 and PreL2=1
	 */
	dma_pmac_templ(dp_info, TEMPL_CHECKSUM,
		       (P_TCP_CSUM | DESC_PMAC | DESC_PREL2), 1);

	/* Any device can do re-insertion */
	dma_pmac_templ(dp_info, TEMPL_INSERT,
		(P_INS | DESC_PMAC | DESC_PREL2), 1);

	for_each_set_bit(i, &bits, __bf_shf(DP_F_DEV_END)) {
		switch (i) {
		case __bf_shf(DP_F_FAST_ETH_LAN):
		case __bf_shf(DP_F_FAST_ETH_WAN):
		case __bf_shf(DP_F_GPON):
		case __bf_shf(DP_F_EPON):
		case __bf_shf(DP_F_GINT):
			for (j = 1; j < MAX_TEMPLATE; j++)
				dma_pmac_templ(dp_info, j,
					       (DESC_PMAC | DESC_PREL2), 1);

			dma_pmac_templ(dp_info, TEMPL_PTP, P_PTP, 1);
			dma_pmac_templ(dp_info, TEMPL_INSERT, P_INS, 1);
			dma_pmac_templ(dp_info, TEMPL_CHECKSUM_PTP,
				       (P_PTP | P_TCP_CSUM), 1);
			dma_mask_templ(dp_info, TEMPL_INSERT, DESC_REDIR, 0);
			dma_pmac_templ(dp_info, TEMPL_INSERT, DESC_REDIR, 0);

			break;
		case __bf_shf(DP_F_VUNI):
			for (j = 0; j < MAX_TEMPLATE; j++) {
				dma_mask_templ(dp_info, j, DESC_REDIR, 0);
				dma_pmac_templ(dp_info, j, DESC_REDIR, 0);
			}

			break;
		case __bf_shf(DP_F_ACA):
			dma_pmac_templ(dp_info, TEMPL_NORMAL, DESC_REDIR, 1);
			dma_pmac_templ(dp_info, TEMPL_NORMAL, DESC_EGP, portid);
			break;
		default:
			break;
		}
	}
}

static
void print_dma_desc(struct dma_rx_desc_0 *desc_0,
		    struct dma_rx_desc_1 *desc_1,
		    struct dma_rx_desc_2 *desc_2,
		    struct dma_rx_desc_3 *desc_3)
{
	DP_DUMP(" DMA Descriptor:D0=0x%08x D1=0x%08x D2=0x%08x D3=0x%08x\n",
		*(u32 *)desc_0, *(u32 *)desc_1,
		*(u32 *)desc_2, *(u32 *)desc_3);
	DP_DUMP("  DW0:%s=%d %s=%d %s=%d %s=0x%04x\n",
		"dw0bit31", desc_0->field.dw0bit31,
		"flow_id", desc_0->field.flow_id,
		"dw0bit16", desc_0->field.dw0bit16,
		"subif", desc_0->field.dest_sub_if_id);
	DP_DUMP("  DW1: %s=%d %s=%d %s=%d %s=%d %s=%d %s=%d %s=%d %s=%d %s=%d",
		"redir", desc_1->field.redir,
		"header_mode", desc_1->field.header_mode,
		"pmac", desc_1->field.pmac,
		"ts", desc_1->field.ts,
		"pre_l2", desc_1->field.pre_l2,
		"classen", desc_1->field.classen,
		"fcs", desc_1->field.fcs,
		"pkt_type", desc_1->field.pkt_type,
		"src_pool", desc_1->field.src_pool);

	DP_DUMP(" %s=%d %s=%d %s=%d %s=%d %s=%d %s=%d\n",
		"dec", desc_1->field.dec,
		"enc", desc_1->field.enc,
		"lro_type", desc_1->field.lro_type,
		"color", desc_1->field.color,
		"port", desc_1->field.ep,
		"classid", desc_1->field.classid);

	DP_DUMP("  DW2:%s=0x%08x %s=%d\n",
		"data_ptr", desc_2->field.data_ptr,
		"ByteOffset", desc_2->field.byte_offset);

	DP_DUMP("  DW3: %s=%d %s=%d %s=%d %s=%d %s=%d %s=0x%08x %s=%d %s=%d %s=%d\n",
		"own", desc_3->field.own,
		"c", desc_3->field.c,
		"sop", desc_3->field.sop,
		"eop", desc_3->field.eop,
		"dic", desc_3->field.dic,
		"haddr", desc_3->field.haddr,
		"sp", desc_3->field.sp,
		"pool_policy", desc_3->field.pool_policy,
		"data_len", desc_3->field.data_len);
}

static
void print_lookup_cfg(struct dma_rx_desc_0 *desc_0,
		      struct dma_rx_desc_1 *desc_1,
		      struct dma_rx_desc_2 *desc_2,
		      struct dma_rx_desc_3 *desc_3)
{
	int inst = 0;
	int lookup;
	int dp_port;
	struct hal_priv *priv = HAL(inst);
	struct cbm_lookup cbm_lookup;
	struct pmac_port_info *port_info;

	dp_port = priv->gp_dp_map[desc_1->field.ep].dpid; /* get lpid */
	port_info = get_dp_port_info(inst, dp_port);
	if (port_info->cqe_lu_mode == CQE_LU_MODE0) {
		/* Eg Lpid[3:0] Sub_If_Id[13:8] Class[1:0] */
		lookup = ((desc_0->field.dest_sub_if_id >> 8) << 2) |
			 ((desc_1->field.classid & 0x3)) |
			 ((dp_port & 0x0F) << 8);
	} else if (port_info->cqe_lu_mode == CQE_LU_MODE1) {
		/* Eg Lpid[3:0] Sub_If_Id[7:0] */
		lookup = (desc_0->field.dest_sub_if_id) |
			 ((dp_port & 0x0F) << 8);
	} else if (port_info->cqe_lu_mode == CQE_LU_MODE2) {
		/* Eg Lpid[3:0] Sub_If_Id[11:8] Class[3:0] */
		lookup = ((desc_0->field.dest_sub_if_id >> 8) << 4) |
			 (desc_1->field.classid) |
			 ((dp_port & 0x0F) << 8);
	} else if (port_info->cqe_lu_mode == CQE_LU_MODE3) {
		/* Eg Lpid[3:0] Sub_If_Id[4:0] Class[2:0] */
		lookup = ((desc_0->field.dest_sub_if_id & 0x1F) << 3) |
			 (desc_1->field.classid & 0x7) |
			 ((dp_port & 0x0F) << 8);
	} else if (port_info->cqe_lu_mode == CQE_LU_MODE4) {
		/* Eg Lpid[3:0] Class[1:0] Sub_If_Id[5:0] */
		lookup = ((desc_0->field.dest_sub_if_id & 0x1F) << 3) |
			 (desc_1->field.classid & 0x7) |
			 ((dp_port & 0x0F) << 8);
	} else if (port_info->cqe_lu_mode == CQE_LU_MODE5) {
		/* Eg Lpid[3:0] Sub_If_Id[15:8] */
		lookup = ((desc_0->field.dest_sub_if_id & 0xFF00) >> 8) |
			 ((dp_port & 0x0F) << 8);
	} else if (port_info->cqe_lu_mode == CQE_LU_MODE6) {
		/* Eg Lpid[3:0] Sub_If_Id[1:0] Class[3:0] Color[1:0] */
		lookup = ((desc_0->field.dest_sub_if_id & 0x2) << 6) |
			 ((desc_1->field.classid) << 2) |
			 ((dp_port & 0x0F) << 8);
	} else if (port_info->cqe_lu_mode == CQE_LU_MODE7) {
		/* Eg Lpid[3:0] Sub_If_Id[14:8] Class[0] */
		lookup = ((desc_0->field.dest_sub_if_id >> 8) << 1) |
			 (desc_1->field.classid & 0x1) |
			 ((dp_port & 0x0F) << 8);
	} else {
		DP_DUMP("Invalid Lookup Mode: %d\n", port_info->cqe_lu_mode);
		return;
	}
	cbm_lookup.index = lookup;
	cbm_lookup.egflag = desc_1->field.redir;
	DP_DUMP("  lookup index=0x%x qid=%d for gpid=%u\n", lookup,
		dp_get_lookup_qid_via_index(inst, &cbm_lookup),
		desc_1->field.ep);
}

static void dump_rx_pmac(struct pmac_rx_hdr *pmac)
{
	int i, l;
	unsigned char *p = (char *)pmac;
	unsigned char *buf;

	if (!pmac) {
		pr_err("DPM: %s: pmac NULL ??\n", __func__);
		return;
	}

	buf = dp_kzalloc(DUMP_BUF_SIZE, GFP_ATOMIC);
	if (!buf)
		return;
	l = snprintf(buf, DUMP_BUF_SIZE - 1, "PMAC at 0x%px: ", p);
	for (i = 0; i < 8; i++)
		l += snprintf(buf + l, DUMP_BUF_SIZE - l - 1, "0x%02x ", p[i]);
	l += snprintf(buf + l, DUMP_BUF_SIZE - l - 1, "\n");
	DP_DUMP("%s", buf);

	/*byte 0 */
	DP_DUMP("  byte 0:%s=%d %s=%d\n",
		"ver_done", pmac->ver_done,
		"ip_offset", pmac->ip_offset);
	/*byte 1 */
	DP_DUMP("  byte 1:%s=%d %s=%d\n",
		"tcp_h_offset", pmac->tcp_h_offset,
		"tcp_type", pmac->tcp_type);
	/*byte 2 */
	DP_DUMP("  byte 2:%s=%d %s=%d %s=%d\n",
		"class", pmac->class,
		"res", pmac->res2,
		"src_dst_subif_id_14_12", pmac->src_dst_subif_id_14_12);
	/*byte 3 */
	DP_DUMP("  byte 3:%s=%d %s=%d %s=%d %s=%d %s=%d %s=%d\n",
		"pkt_type", pmac->pkt_type,
		"ext", pmac->ext,
		"ins", pmac->ins,
		"pre_12", pmac->pre_l2,
		"oam", pmac->oam,
		"res32", pmac->res32);
	/*byte 4 */
	DP_DUMP("  byte 4:%s=%d %s=%d %s=%d %s=%d\n",
		"fcs", pmac->fcs,
		"ptp", pmac->ptp,
		"one_step", pmac->one_step,
		"src_dst_subif_id_msb", pmac->src_dst_subif_id_msb);
	/*byte 5 */
	DP_DUMP("  byte 5:%s=%d\n",
		"src_sub_inf_id2", pmac->src_dst_subif_id_lsb);
	/*byte 6 */
	DP_DUMP("  byte 6:%s=%d\n",
		"record_id_msb", pmac->record_id_msb);
	/*byte 7 */
	DP_DUMP("  byte 7:%s=%d %s=%d\n",
		"record_id_lsb", pmac->record_id_lsb,
		"igp_egp", pmac->igp_egp);
	kfree(buf);
}

static void dump_tx_pmac(struct pmac_tx_hdr *pmac)
{
	int i, l;
	unsigned char *p = (char *)pmac;
	unsigned char *buf;

	if (!pmac) {
		pr_err("DPM: %s pmac NULL ??\n", __func__);
		return;
	}

	buf = dp_kzalloc(DUMP_BUF_SIZE, GFP_ATOMIC);
	if (!buf)
		return;

	l = snprintf(buf, DUMP_BUF_SIZE - 1, "PMAC at 0x%px: ", p);
	for (i = 0; i < 8; i++)
		l += snprintf(buf + l, DUMP_BUF_SIZE - l - 1, "0x%02x ", p[i]);
	snprintf(buf + l, DUMP_BUF_SIZE - l - 1, "\n");
	DP_DUMP("%s", buf);

	/*byte 0 */
	DP_DUMP("  byte 0:%s=%d %s=%d\n",
		"tcp_chksum", pmac->tcp_chksum,
		"ip_offset", pmac->ip_offset);
	/*byte 1 */
	DP_DUMP("  byte 1:tcp_h_offset=%d tcp_type=%d\n", pmac->tcp_h_offset,
		pmac->tcp_type);
	/*byte 2 */
	DP_DUMP("  byte 2:%s=%d %s=%d\n",
		"igp_msb", pmac->src_dst_subif_id_14_12,
		"res", pmac->res2);
	/*byte 3 */
	DP_DUMP("  byte 3:%s=%d %s=%d %s=%d %s=%d %s=%d %s=%d %s=%d\n",
		"pkt_type", pmac->pkt_type,
		"ext", pmac->ext,
		"ins", pmac->ins,
		"res3", pmac->res3,
		"oam", pmac->oam,
		"lrnmd", pmac->lrnmd,
		"class_en", pmac->class_en);
	/*byte 4 */
	DP_DUMP("  byte 4:%s=%d %s=%d %s=%d %s=%d\n",
		"fcs_ins_dis", pmac->fcs_ins_dis,
		"ptp", pmac->ptp,
		"one_step", pmac->one_step,
		"src_dst_subif_id_msb", pmac->src_dst_subif_id_msb);
	/*byte 5 */
	DP_DUMP("  byte 5:%s=%d\n",
		"src_dst_subif_id_lsb", pmac->src_dst_subif_id_lsb);
	/*byte 6 */
	DP_DUMP("  byte 6:%s=%d\n",
		"record_id_msb", pmac->record_id_msb);
	/*byte 7 */
	DP_DUMP("  byte 7:%s=%d %s=%d\n",
		"record_id_lsb", pmac->record_id_lsb,
		"igp_egp", pmac->igp_egp);
	kfree(buf);
}

/* Explicitely configure reserved GPID<->LPID mapping in CQM */
static int dp_cqm_set_reserved_gpid_map(int inst)
{
	struct cbm_gpid_lpid *cbm_gpid;
	int i;

	cbm_gpid = dp_kzalloc(sizeof(*cbm_gpid), GFP_ATOMIC);
	if (!cbm_gpid)
		return DP_FAILURE;
	cbm_gpid->cbm_inst = inst;
	for (i = DP_RES_GPID_LPID_START; i <= DP_RES_GPID_LPID_END; i++) {
		cbm_gpid->gpid = i;
		cbm_gpid->lpid = i;
		if (dp_cqm_gpid_lpid_map(inst, cbm_gpid)) {
			kfree(cbm_gpid);
			return DP_FAILURE;
		}
	}

	kfree(cbm_gpid);
	return 0;
}

static void dp_init_reserved_gpid_map(int inst)
{
	struct hal_priv *priv = HAL(inst);
	int i;

	for (i = DP_RES_GPID_LPID_START; i <= DP_RES_GPID_LPID_END; i++)
		priv->gp_dp_map[i].dpid = i;
}

static int dp_set_io_port(int inst, int dpid, int vap, int type)
{
	return dp_update_hostif(inst, dpid, vap, type);
}

#elif IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
/* Note:
 * final tx_dma0 = (tx_dma0 & dma0_mask_template) | dma0_template
 * final tx_dma1 = (tx_dma1 & dma1_mask_template) | dma1_template
 * final tx_pmac = pmac_template
 * Note: only instance zero need set dma/pmac fields
 */
static void init_dma_pmac_template(int portid, u32 flags)
{
	int i = 0, j = 0;
	struct pmac_port_info *dp_info = get_dp_port_info(0, portid);
	unsigned long bits = flags;

	for (i = 0; i < MAX_TEMPLATE; i++) {
		for (j = 0; j < 4; j++) {
			dp_memset(&dp_info->desc_dw_templ[j][i], 0, sizeof(u32));
			dp_memset(&dp_info->desc_dw_mask[j][i], 0, sizeof(u32));
		}
		dp_memset(&dp_info->pmac_template[i], 0, sizeof(u32));

		dma_mask_templ(dp_info, i, DESC_DW0_ALL | DESC_DW1_ALL,
			       0xFFFFFFFF);
	}

	for_each_set_bit(i, &bits, __bf_shf(DP_F_DEV_END)) {
		switch (i) {
		case __bf_shf(DP_F_FAST_ETH_LAN):
		case __bf_shf(DP_F_FAST_ETH_WAN):
		case __bf_shf(DP_F_GPON):
		case __bf_shf(DP_F_EPON):
		case __bf_shf(DP_F_GINT):

			for (j = 0; j < MAX_TEMPLATE; j++) {
				dma_pmac_templ(dp_info, j,
					       DESC_REDIR | P_CLS_EN, 1);
				dma_pmac_templ(dp_info, j, P_IGP_EGP, portid);
			}
			dma_pmac_templ(dp_info, TEMPL_PTP, P_PTP, 1);

			/* To have a specific Q for Reinsertion lookup is like
			 * ENC=1, DEC=0, MPE1=0, MPE2=0 EGP=0, REDIR=0
			 * EP is set as 0 to select Lookup Mode 0
			 */
			dma_mask_templ(dp_info, TEMPL_INSERT,
				       (DESC_REDIR | DESC_DEC | DESC_MPE1 |
					DESC_MPE2 | DESC_EGP), 0);
			dma_pmac_templ(dp_info, TEMPL_INSERT, DESC_REDIR, 0);
			dma_pmac_templ(dp_info, TEMPL_INSERT,
				       P_INS | DESC_ENC, 1);
			break;
		case __bf_shf(DP_F_VUNI):

			dma_mask_templ(dp_info, TEMPL_NORMAL, DESC_REDIR, 0);
			dma_pmac_templ(dp_info, TEMPL_NORMAL, DESC_REDIR, 0);
			dma_pmac_templ(dp_info, TEMPL_NORMAL,
				       (DESC_MPE2 | DESC_MPE1), 1);
			dma_pmac_templ(dp_info, TEMPL_NORMAL,
				       (DESC_EGP | DESC_IGP), portid);
			dma_pmac_templ(dp_info, TEMPL_NORMAL,
				       P_IGP_EGP | P_IGP_MSB, portid);
			break;
		case __bf_shf(DP_F_FAST_DSL):

			/* For normal single DSL upstream, there is no pmac
			 * at all
			 */
			dma_mask_templ(dp_info, TEMPL_NORMAL, DESC_ENC, 0);
			dma_pmac_templ(dp_info, TEMPL_NORMAL,
				       (DESC_DEC | DESC_MPE2), 1);

			/* DSL with checksum support */
			dma_pmac_templ(dp_info, TEMPL_CHECKSUM, P_IGP_MSB,
				       portid);
			dma_pmac_templ(dp_info, TEMPL_CHECKSUM,
				       (P_TCP_CSUM | DESC_DEC | DESC_ENC |
					DESC_MPE2 | P_CLS_EN | DESC_REDIR), 1);

			/* Bonding DSL  FCS Support via GSWIP */
			dma_pmac_templ(dp_info, TEMPL_OTHERS,
				       (P_TCP_CSUM | DESC_DEC | DESC_ENC |
					DESC_MPE2 | P_CLS_EN | DESC_REDIR), 1);
			dma_pmac_templ(dp_info, TEMPL_OTHERS,
				       P_IGP_EGP | P_IGP_MSB, portid);
			break;
		case __bf_shf(DP_F_DIRECT):
			/* Note: we cannot support GSWIP checksum offloading
			 * since all mpe2/mpe1 conbination are used up and
			 * difficult to map queue now.
			 * Here we need to set Descriptor and pmac header
			 * descriptor: MPE2=1 MPE1=1, redirect=0, EP/IP=portid
			 * pmac: enable traffic class
			 */
			dma_pmac_templ(dp_info, TEMPL_NORMAL,
				       DESC_MPE1 | DESC_MPE2 | P_CLS_EN, 1);
			dma_pmac_templ(dp_info, TEMPL_NORMAL,
				       DESC_IGP | DESC_EGP, portid);
			break;

		default:
			dma_pmac_templ(dp_info, TEMPL_CHECKSUM, P_IGP_MSB,
				       PMAC_CPU_ID);
			dma_pmac_templ(dp_info, TEMPL_CHECKSUM, P_IGP_EGP,
				       portid);
			dma_pmac_templ(dp_info, TEMPL_CHECKSUM,
				       (P_TCP_CSUM | DESC_DEC | DESC_ENC |
					DESC_MPE2 | P_CLS_EN | DESC_REDIR), 1);
			dma_pmac_templ(dp_info, TEMPL_NORMAL,
				       P_IGP_MSB | P_IGP_EGP, portid);
			dma_pmac_templ(dp_info, TEMPL_NORMAL, P_CLS_EN, 1);
			break;
		}
	}
}

static
void print_dma_desc(struct dma_rx_desc_0 *desc_0,
		    struct dma_rx_desc_1 *desc_1,
		    struct dma_rx_desc_2 *desc_2,
		    struct dma_rx_desc_3 *desc_3)
{
	DP_DUMP(" DMA Descriptor:D0=0x%08x D1=0x%08x D2=0x%08x D3=0x%08x\n",
		*(u32 *)desc_0, *(u32 *)desc_1,
		*(u32 *)desc_2, *(u32 *)desc_3);
	DP_DUMP("  DW0:%s=%d %s=%d %s=%d %s=%d %s=%d %s=0x%04x\n",
		"redir", desc_0->field.redir,
		"res", desc_0->field.resv,
		"tunl", desc_0->field.tunnel_id,
		"flow", desc_0->field.flow_id,
		"ether", desc_0->field.eth_type,
		"subif", desc_0->field.dest_sub_if_id);
	DP_DUMP("  DW1:%s=%d %s=%d %s=%d %s=%d %s=%d %s=%d %s=%d\n",
		"sess/src_subif", desc_1->field.session_id,
		"tcp_err", desc_1->field.tcp_err,
		"nat", desc_1->field.nat,
		"dec", desc_1->field.dec,
		"enc", desc_1->field.enc,
		"mpe2", desc_1->field.mpe2,
		"mpe1", desc_1->field.mpe1);
	DP_DUMP("      %s=%02d %s=%02d %s=%02d %s=%02d\n",
		"color", desc_1->field.color,
		"ep", desc_1->field.ep,
		"ip", desc_1->field.ip,
		"classid", desc_1->field.classid);
	DP_DUMP("  DW2:%s=0x%08x\n",
		"data_ptr", desc_2->field.data_ptr);
	DP_DUMP("  DW3:%s=%d %s=%d %s=%d %s=%d %s=%d %s=%d\n",
		"own", desc_3->field.own,
		"c", desc_3->field.c,
		"sop", desc_3->field.sop,
		"eop", desc_3->field.eop,
		"dic", desc_3->field.dic,
		"pdu_type", desc_3->field.pdu_type);
	DP_DUMP("      %s=%d %s=%d %s=%d %s=%d %s=%d\n",
		"offset", desc_3->field.byte_offset,
		"policy", desc_3->field.policy,
		"res", desc_3->field.res,
		"pool", desc_3->field.pool,
		"len", desc_3->field.data_len);
}

static
void print_lookup_cfg(struct dma_rx_desc_0 *desc_0,
		      struct dma_rx_desc_1 *desc_1,
		      struct dma_rx_desc_2 *desc_2,
		      struct dma_rx_desc_3 *desc_3)
{
	int inst = 0;
	int lookup;
	struct cbm_lookup cbm_lookup;
	struct pmac_port_info *port_info;

	port_info = get_dp_port_info(inst, desc_1->field.ep);
	if (port_info->cqe_lu_mode == CQE_LU_MODE0) {
		/* Flow[7:6] DEC ENC MPE2 MPE1 EP Class */
		lookup = ((desc_0->field.flow_id >> 6) << 12) |
			 ((desc_1->field.dec) << 11) |
			 ((desc_1->field.enc) << 10) |
			 ((desc_1->field.mpe2) << 9) |
			 ((desc_1->field.mpe1) << 8) |
			 ((desc_1->field.ep) << 4) |
			 desc_1->field.classid;
	} else if (port_info->cqe_lu_mode == CQE_LU_MODE1) {
		/*Subif[7:4] MPE2 MPE1 EP Subif[3:0] */
		lookup = ((desc_0->field.dest_sub_if_id >> 4) << 10) |
			 ((desc_1->field.mpe2) << 9) |
			 ((desc_1->field.mpe1) << 8) |
			 ((desc_1->field.ep) << 4) |
			 (desc_0->field.dest_sub_if_id & 0xf);
	} else if (port_info->cqe_lu_mode == CQE_LU_MODE2) {
		/*Subif[7:4] MPE2 MPE1 EP Class */
		lookup = ((desc_0->field.dest_sub_if_id >> 4) << 10) |
			 ((desc_1->field.mpe2) << 9) |
			 ((desc_1->field.mpe1) << 8) |
			 ((desc_1->field.ep) << 4) |
			 desc_1->field.classid;
	} else {
		/*Subif[4:1] MPE2 MPE1 EP Subif[0:0] Class[2:0] */
		lookup = (((desc_0->field.dest_sub_if_id >> 1) & 0xf) << 10) |
			 ((desc_1->field.mpe2) << 9) |
			 ((desc_1->field.mpe1) << 8) |
			 ((desc_1->field.ep) << 4) |
			 ((desc_0->field.dest_sub_if_id & 0x1) << 3) |
			 (desc_1->field.classid & 7); /*lower 3 bits*/
	}
	cbm_lookup.index = lookup;
	cbm_lookup.egflag = 0;
	DP_DUMP("  lookup index=0x%x qid=%d\n", lookup,
		dp_get_lookup_qid_via_index(inst, &cbm_lookup));
}

static void dump_rx_pmac(struct pmac_rx_hdr *pmac)
{
	int i, l;
	unsigned char *p = (char *)pmac;
	unsigned char *buf;

	if (!pmac) {
		pr_err("DPM: %s: pmac NULL ??\n", __func__);
		return;
	}
	buf = dp_kzalloc(DUMP_BUF_SIZE, GFP_ATOMIC);
	if (!buf)
		return;

	l = snprintf(buf, DUMP_BUF_SIZE - 1, "PMAC at 0x%px: ", p);
	for (i = 0; i < 8; i++)
		l += snprintf(buf + l, DUMP_BUF_SIZE - l - 1, "0x%02x ", p[i]);
	l += snprintf(buf + l, DUMP_BUF_SIZE - l - 1, "\n");
	DP_DUMP("%s", buf);

	/*byte 0 */
	DP_DUMP("  byte 0:%s=%d %s=%d %s=%d\n",
		"res", pmac->res0,
		"ver_done", pmac->ver_done,
		"ip_offset", pmac->ip_offset);
	/*byte 1 */
	DP_DUMP("  byte 1:%s=%d %s=%d\n",
		"tcp_h_offset", pmac->tcp_h_offset,
		"tcp_type", pmac->tcp_type);
	/*byte 2 */
	DP_DUMP("  byte 2:%s=%d %s=%d\n",
		"class", pmac->class,
		"res", pmac->res2);
	/*byte 3 */
	DP_DUMP("  byte 3:%s=%d %s=%d %s=%d %s=%d %s=%d %s=%d\n",
		"pkt_type", pmac->pkt_type,
		"ext", pmac->ext,
		"ins", pmac->ins,
		"res31", pmac->res31,
		"oam", pmac->oam,
		"res32", pmac->res32);
	/*byte 4 */
	DP_DUMP("  byte 4:%s=%d %s=%d %s=%d %s=%d\n",
		"res", pmac->res4,
		"ptp", pmac->ptp,
		"one_step", pmac->one_step,
		"src_dst_subif_id_msb", pmac->src_dst_subif_id_msb);
	/*byte 5 */
	DP_DUMP("  byte 5:%s=%d\n",
		"src_sub_inf_id2", pmac->src_dst_subif_id_lsb);
	/*byte 6 */
	DP_DUMP("  byte 6:%s=%d\n",
		"record_id_msb", pmac->record_id_msb);
	/*byte 7 */
	DP_DUMP("  byte 7:%s=%d %s=%d\n",
		"record_id_lsb", pmac->record_id_lsb,
		"igp_egp", pmac->igp_egp);
	kfree(buf);
}

static void dump_tx_pmac(struct pmac_tx_hdr *pmac)
{
	int i, l;
	unsigned char *p = (char *)pmac;
	unsigned char *buf;

	if (!pmac) {
		pr_err("DPM: %s pmac NULL ??\n", __func__);
		return;
	}
	buf = dp_kzalloc(DUMP_BUF_SIZE, GFP_ATOMIC);
	if (!buf)
		return;

	l = snprintf(buf, DUMP_BUF_SIZE - 1, "PMAC at 0x%px: ", p);
	for (i = 0; i < 8; i++)
		l += snprintf(buf + l, DUMP_BUF_SIZE - l - 1, "0x%02x ", p[i]);
	snprintf(buf + l, DUMP_BUF_SIZE - l - 1, "\n");
	DP_DUMP("%s", buf);
	/*byte 0 */
	DP_DUMP("  byte 0:%s=%d %s=%d %s=%d\n",
		"res", pmac->res1,
		"tcp_chksum", pmac->tcp_chksum,
		"ip_offset", pmac->ip_offset);
	/*byte 1 */
	DP_DUMP("  byte 1:%s=%d %s=%d\n",
		"tcp_h_offset", pmac->tcp_h_offset,
		"tcp_type", pmac->tcp_type);
	/*byte 2 */
	DP_DUMP("  byte 2:%s=%d %s=%d\n",
		"igp_msb", pmac->igp_msb,
		"res", pmac->res2);
	/*byte 3 */
	DP_DUMP("  byte 3:%s=%d %s=%d %s=%d %s=%d %s=%d %s=%d %s=%d\n",
		"pkt_type", pmac->pkt_type,
		"ext", pmac->ext,
		"ins", pmac->ins,
		"res3", pmac->res3,
		"oam", pmac->oam,
		"lrnmd", pmac->lrnmd,
		"class_en", pmac->class_en);
	/*byte 4 */
	DP_DUMP("  byte 4:%s=%d %s=%d %s=%d %s=%d\n",
		"fcs_ins_dis", pmac->fcs_ins_dis,
		"ptp", pmac->ptp,
		"one_step", pmac->one_step,
		"src_dst_subif_id_msb", pmac->src_dst_subif_id_msb);
	/*byte 5 */
	DP_DUMP("  byte 5:%s=%d\n",
		"src_dst_subif_id_lsb", pmac->src_dst_subif_id_lsb);
	/*byte 6 */
	DP_DUMP("  byte 6:%s=%d\n",
		"record_id_msb", pmac->record_id_msb);
	/*byte 7 */
	DP_DUMP("  byte 7:%s=%d %s=%d\n",
		"record_id_lsb", pmac->record_id_lsb,
		"igp_egp", pmac->igp_egp);
	kfree(buf);
}

#endif

#define EGFLAG_BIT  12 /* egflag bit */

int dp_get_lookup_qid_via_index(int inst, struct cbm_lookup *info)
{
	int qid;

	if (!info)
		return 0;
	qid = CBM_OPS(inst, get_lookup_qid_via_index, info);
	DP_DEBUG(DP_DBG_FLAG_LOOKUP, "get egflag=%d index=0x%x qid=%d\n",
		 info->egflag, info->index, qid);
	return qid;
}

static enum pp_min_tx_pkt_len get_min_pkt_len_cfg(u32 len)
{
	static u32 min_pkt_len[PP_NUM_TX_MIN_PKT_LENS] = {
		PP_MIN_TX_PKT_LEN_VAL_NONE,
		PP_MIN_TX_PKT_LEN_VAL_60B,
		PP_MIN_TX_PKT_LEN_VAL_64B,
		PP_MIN_TX_PKT_LEN_VAL_128B,
	};
	int i;

	for (i = 0; i < PP_NUM_TX_MIN_PKT_LENS; i++) {
		if (len <= min_pkt_len[i])
			break;
	}
	return i;
}

static int subif_platform_change_mtu(int inst, int portid, int subif_ix,
				     u32 mtu)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	return dp_subif_pp_change_mtu(inst, portid, subif_ix, mtu);
#else
	return 0;
#endif
}

void dump_rx_dma_desc(struct dma_rx_desc_0 *desc_0,
		      struct dma_rx_desc_1 *desc_1,
		      struct dma_rx_desc_2 *desc_2,
		      struct dma_rx_desc_3 *desc_3)
{
	if (!desc_0 || !desc_1 || !desc_2 || !desc_3) {
		pr_err("DPM: %s: rx desc_0/1/2/3 NULL\n", __func__);
		return;
	}

	print_dma_desc(desc_0, desc_1, desc_2, desc_3);
}

void dump_tx_dma_desc(struct dma_tx_desc_0 *desc_0,
		      struct dma_tx_desc_1 *desc_1,
		      struct dma_tx_desc_2 *desc_2,
		      struct dma_tx_desc_3 *desc_3)
{
	if (!desc_0 || !desc_1 || !desc_2 || !desc_3) {
		pr_err("DPM: %s: tx desc_0/1/2/3 NULL\n", __func__);
		return;
	}

	print_dma_desc(desc_0, desc_1, desc_2, desc_3);
	print_lookup_cfg(desc_0, desc_1, desc_2, desc_3);
}

int alloc_q_to_port(struct ppv4_q_sch_port *info, u32 flag)
{
	struct dp_node_link link = {0};
	struct hal_priv *priv = HAL(info->inst);
	struct cqm_port_info *deq;

	link.cqm_deq_port.cqm_deq_port = info->cqe_deq;
	link.dp_port = info->dp_port;
	link.inst = info->inst;
	link.node_id.q_id = DP_NODE_AUTO_ID;
	link.node_type = DP_NODE_QUEUE;
	link.p_node_id.cqm_deq_port = info->cqe_deq;
	link.p_node_type = DP_NODE_PORT;
	link.arbi = ARBITRATION_PARENT;
	link.prio_wfq = 0;

	if (dp_node_link_add(&link, 0)) {
		pr_err("DPM: %s: dp_node_link_add fail: deq_port=%d\n", __func__,
		       info->cqe_deq);
		return DP_FAILURE;
	}

	info->qid = link.node_id.q_id;
	info->q_node = priv->qos_queue_stat[info->qid].node_id;
	info->port_node = priv->deq_port_stat[info->cqe_deq].node_id;

	deq = get_dp_deqport_info(info->inst, link.p_node_id.cqm_deq_port);
	deq->f_first_qid = 1;
	deq->first_qid = link.node_id.q_id;
	deq->qid[0] = link.node_id.q_id;

	DP_DEBUG(DP_DBG_FLAG_QOS,
		 "%s: qid=%d p_node_id=%d for cqm port=%d\n",
		 __func__, link.node_id.q_id, link.p_node_id.cqm_deq_port,
		 info->cqe_deq);
	return DP_SUCCESS;
}

#define REINSERT BIT(1)

static
int alloc_cpu_q(int inst, struct cbm_cpu_port_data *cpu_data,
		struct ppv4_q_sch_port *q_port, int cpu_idx,
		int port_per_cpu, u32 flag)
{
	struct cqm_port_info *c_info;
	struct dp_cap *cap;
	struct cbm_tx_push *tx_push = NULL;
	struct cbm_dp_alloc_data *re_insertion;
	struct cqm_port_info *deq;
	struct pmac_port_info *cpu_port;

	if (cpu_idx >= CQM_MAX_CPU)
		return DP_FAILURE;

	cap = &get_dp_prop_info(inst)->cap;

	q_port->inst = inst;
	q_port->dp_port = PMAC_CPU_ID;

	tx_push = &cpu_data->dq_tx_push_info[cpu_idx][port_per_cpu];

	if (flag & REINSERT) {
		re_insertion = &cpu_data->re_insertion;
		q_port->cqe_deq = re_insertion->deq_port;
		q_port->tx_pkt_credit = re_insertion->tx_pkt_credit;
		q_port->tx_ring_addr = (void *)re_insertion->txpush_addr_qos;
		q_port->tx_ring_addr_push = (void *)re_insertion->txpush_addr;
		q_port->tx_ring_size = re_insertion->tx_ring_size;

		/* set dts_qos cfg */
		deq = get_dp_deqport_info(inst, re_insertion->deq_port);
		deq->dts_qos = dp_get_inter_qos_cfg(inst, re_insertion->deq_port);
	} else {
		/* All CPU ports enabled have valid CQM Deq port otherwise -1 */
		if (tx_push->deq_port == -1)
			return 1;
		q_port->cqe_deq = tx_push->deq_port;
		q_port->tx_pkt_credit = tx_push->tx_pkt_credit;
		q_port->tx_ring_addr = (void *)tx_push->txpush_addr_qos;
		q_port->tx_ring_addr_push = (void *)tx_push->txpush_addr;
		q_port->tx_ring_size = tx_push->tx_ring_size;

		/* set dts_qos cfg */
		cpu_port = get_dp_port_info(inst, CPU_PORT);
		deq = get_dp_deqport_info(inst, q_port->cqe_deq);
		deq->dts_qos = cpu_port->dts_qos;
	}

	c_info = get_dp_deqport_info(inst, q_port->cqe_deq);

	/* Store Ring Info */
	c_info->tx_pkt_credit = q_port->tx_pkt_credit;
	c_info->txpush_addr = (void *)q_port->tx_ring_addr_push;
	c_info->txpush_addr_qos = (void *)q_port->tx_ring_addr;
	c_info->tx_ring_size = q_port->tx_ring_size;
	c_info->dp_port[CPU_PORT] = 1;
	c_info->ref_cnt++;

	/* Store CPU type when MPEFW is selected */
	if (is_soc_prx(inst) && !(flag & REINSERT)) {
		if (tx_push->type == DP_F_DEQ_MPE)
			c_info->cpu_type = DP_DATA_PORT_MPE;
	}

	/* fake CTP for CPU port to store its qid */
	q_port->ctp = cpu_idx;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_QOS) {
		if (!(flag & REINSERT))
			DP_DUMP("cpu(%d) deq_port=%d", cpu_idx, tx_push->deq_port);
		else
			DP_DUMP("reinsert deq_port=%d", re_insertion->deq_port);

		DP_DUMP("  ring_address[%d]=0x%px\n",
			q_port->cqe_deq, c_info->txpush_addr);
		DP_DUMP("  ring_address_push[%d]=0x%px\n",
			q_port->cqe_deq, c_info->txpush_addr_qos);
		DP_DUMP("  ring_size[%d]=%d\n",
			q_port->cqe_deq, c_info->tx_ring_size);
		DP_DUMP("  credit[%d]=%d\n",
			q_port->cqe_deq, c_info->tx_pkt_credit);
	}
#endif

	if (alloc_q_to_port(q_port, 0)) {
		pr_err("DPM: %s: alloc_q_to_port fail for dp_port=%d\n",
		       __func__, q_port->dp_port);
		return DP_FAILURE;
	}

	return DP_SUCCESS;
}

/* Alloc and Enable of Re-Insertion Port is done in CQM by default */
static
int dp_reinsert_q_set(int inst, struct pmac_port_info *port_info,
		      struct cbm_cpu_port_data *cpu_data)
{
	struct cbm_dp_alloc_data *re_insertion;
	struct ppv4_q_sch_port q_port = {0};
	struct cqm_port_info *c_info;
	cbm_queue_map_entry_t *lookup;

	lookup = dp_kzalloc(sizeof(*lookup), GFP_ATOMIC);
	if (!lookup)
		return DP_FAILURE;
	re_insertion = &cpu_data->re_insertion;

	if (!is_deqport_valid(re_insertion->deq_port)) {
		kfree(lookup);
		return DP_FAILURE;
	}
	if (alloc_cpu_q(inst, cpu_data, &q_port, PMAC_CPU_ID, 0, REINSERT)) {
		kfree(lookup);
		return DP_FAILURE;
	}

	c_info = get_dp_deqport_info(inst, q_port.cqe_deq);
	c_info->f_dma_ch = 1;
	c_info->dma_chan = re_insertion->dma_chan;
	c_info->first_qid = q_port.qid;
	c_info->q_node = q_port.q_node;
	reinsert_deq_port[inst] = q_port.cqe_deq;
	dflt_q_re_insert[inst] = q_port.qid;

	/* Alloc and Enable of Re-Insertion Port is done in CQM by default */
	DP_DEBUG(DP_DBG_FLAG_REG,
		 "Re-Insertion deq_port=%d Initialized to QiD %d\n",
		 re_insertion->deq_port, q_port.qid);

	kfree(lookup);
	return DP_SUCCESS;
}

static
void save_subif_info(int inst, struct cbm_cpu_port_data *cpu_data,
		     struct ppv4_q_sch_port *q_port,
		     struct pmac_port_info *cpu_port,
		     u32 cpu_idx, u32 port_per_cpu)
{
	int vap;
	struct dp_subif_info *subif_info;
	struct dp_cap *cap;

	cap = &get_dp_prop_info(inst)->cap;

	vap = cap->max_port_per_cpu * cpu_idx + port_per_cpu;
	subif_info = get_dp_port_subif(cpu_port, vap);
	subif_info->flags = PORT_DEV_REGISTERED;
	subif_info->subif = SET_VAP(vap, cpu_port->vap_offset,
				    cpu_port->vap_mask);
	subif_info->num_qid = 1;
	subif_info->qid = q_port->qid;
	subif_info->q_node[0] = q_port->q_node;
	subif_info->qos_deq_port[0] = q_port->port_node;
	subif_info->cqm_deq_port[0] = q_port->cqe_deq;
	subif_info->cqm_port_idx = q_port->cqe_deq;
	subif_info->num_egp = 1;
	subif_info->cqm_port_idx = vap;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	subif_info->tx_policy_base =
		cpu_data->policy_base[cpu_idx][port_per_cpu];
	subif_info->tx_policy_num =
		cpu_data->policy_num[cpu_idx][port_per_cpu];
	subif_info->rx_policy_base = subif_info->tx_policy_base;
	subif_info->rx_policy_num = subif_info->tx_policy_num;
	subif_info->type = cpu_data->type[cpu_idx][port_per_cpu];
	subif_info->igp_id =
		cpu_data->dq_tx_push_info[cpu_idx][port_per_cpu].igp_id;
	subif_info->tx_pkt_credit =
		cpu_data->dq_tx_push_info[cpu_idx][port_per_cpu].tx_pkt_credit;
	subif_info->prel2_len = 1;

	subif_info->spl_conn_type = DP_NON_SPL;
#endif
	cpu_port->alloc_flags = DP_F_CPU;
	subif_info->port_info = cpu_port;

	subif_info->flags = 1;
}

static int dev_platform_set(int inst, u8 ep, struct dp_dev_data *data,
			    uint32_t flags)
{
	struct gsw_itf *itf;
	struct hal_priv *priv = (struct hal_priv *)dp_port_prop[inst].priv_hal;
	struct pmac_port_info *port_info = get_dp_port_info(inst, ep);

	if (!priv) {
		pr_err("DPM: %s: priv is NULL\n", __func__);
		return DP_FAILURE;
	}
	if (flags & DP_F_DEREGISTER)
		dp_free_deq_port(inst, ep, data, flags);

	itf = dp_gsw_assign_ctp(inst, ep, priv->bp_def, flags, data);
	port_info->itf_info = itf;

	dp_node_reserve(inst, ep, data, flags);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	if (gpid_port_assign(inst, ep, data, flags)) {
		pr_err("DPM: gpid_port_assign failed\n");
		return DP_FAILURE;
	}
	dev_platform_set_aca_rxout_queue(inst, ep, flags);
#endif

	/* set streaming port related deq port qos cfg */
	if (!(port_info->alloc_flags & DP_F_ACA)) {
		int i;
		struct cqm_port_info *deq;

		for (i = 0; i < port_info->deq_port_num; i++) {
			deq = get_dp_deqport_info(inst, port_info->deq_ports[i]);
			deq->dts_qos = port_info->dts_qos;
		}
	}

	return DP_SUCCESS;
}

static int platform_map_to_drop_q(int inst)
{
	struct hal_priv *priv = (struct hal_priv *)dp_port_prop[inst].priv_hal;
	struct ppv4_queue *q;

	q = dp_kzalloc(sizeof(*q), GFP_ATOMIC);
	if (!q)
		return DP_FAILURE;
	/* Allocate a drop queue */
	if (priv->ppv4_drop_q < 0) {
		q->parent = 0;
		q->inst = inst;
		if (dp_pp_alloc_queue(q)) {
			kfree(q);
			return DP_FAILURE;
		}

		priv->ppv4_drop_q = q->qid;
	}
	dflt_q_drop[inst] = priv->ppv4_drop_q;
	_dp_reset_q_lookup_tbl(inst);

	kfree(q);
	return DP_SUCCESS;
}

static int cpu_mode_table_cfg(int inst)
{
	struct pmac_port_info *cpu_port;
	const struct ctp_assign *cpu_assign;
	cbm_queue_map_entry_t *lookup;
	u32 mode_flags = 0;
	int cbm_inst = dp_port_prop[inst].cbm_inst;

	cpu_port = get_dp_port_info(inst, CPU_PORT);
	cpu_assign = dp_gsw_get_ctp_assign(inst, DP_F_CPU);
	if (!cpu_assign) {
		pr_err("DPM: %s: cpu_assign NULL\n", __func__);
		return DP_FAILURE;
	}
	cpu_port->cqe_lu_mode = cpu_assign->cqe_lu_md;
	lookup = dp_kzalloc(sizeof(*lookup), GFP_ATOMIC);
	if (!lookup)
		return DP_FAILURE;
	lookup->ep = PMAC_CPU_ID;
	lookup->sub_if_id_mask_bits = -1;

	if (is_soc_lgm(inst))
		mode_flags = CQM_QMAP_F_EP_ONLY;
	else if (is_soc_prx(inst))
		mode_flags = CBM_QUEUE_MAP_F_MPE1_DONTCARE |
			     CBM_QUEUE_MAP_F_MPE2_DONTCARE;

	if (CBM_OPS(inst, cqm_mode_table_set, cbm_inst, lookup,
		    cpu_port->cqe_lu_mode, mode_flags)) {
		kfree(lookup);
		pr_err("DPM: %s: cqm_mode_table_set failed\n", __func__);
		return DP_FAILURE;
	}

	kfree(lookup);
	return DP_SUCCESS;
}

static int dp_cpu_netdev(int inst, int cpu_id, int vap, int idx, int flag)
{
	char net_name[20];
	dp_subif_t *subif_sync = dp_kmalloc(sizeof(*subif_sync), GFP_KERNEL);
	struct pmac_port_info *cpu_port = get_dp_port_info(inst, CPU_PORT);
	struct dp_subif_info *sif;
	struct net_device *netif = NULL;


	if (!subif_sync)
		return -1;
	sprintf(net_name, "cpu%d_%d_%d", cpu_id, vap, inst);
	dp_memset(subif_sync, 0, sizeof(*subif_sync));
	subif_sync->inst = inst;
	sif = get_dp_port_subif(cpu_port, vap);
	if (!sif->flags)
		return -1;

	/* copy common port flag */
	subif_sync->subif_port_cmn = cpu_port->subif_port_cmn;

	subif_sync->subif_common.def_qlist[0] = sif->qid;
	subif_sync->subif_common.num_q =  1;
	subif_sync->data_flag = idx ?
				DP_SUBIF_CPU_LOW_PRI : DP_SUBIF_CPU_HIGH_PRI;
	subif_sync->subif_list[0] = sif->subif;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	subif_sync->gpid_list[0] = sif->gpid;
#endif
	subif_sync->subif_flag[0] = sif->subif_flag;
	subif_sync->subif_num = 1;
	
	netif = dp_create_netdev(net_name);
	if (!netif) {
		kfree(subif_sync);
		pr_err("dpm: dp_create_netdev failed\n");
		return -1;
	}
	sif->netif = netif;

	subif_sync->subif_common.bport = 0;
	subif_sync->subif_common.subif_groupid = vap;
	subif_sync->type = sif->type;

	if (!subif_sync->subif_num) {
		kfree(subif_sync);
		pr_err("dpm: dp_cpu_netdev failed\n");
		return -1;
	}

	if (dp_update_subif(netif, NULL, subif_sync, netif->name, 0, NULL)) {
		kfree(subif_sync);
		pr_err("DPM: %s: dp_update_subif failed: %s\n", __func__,
		       sif->netif->name);
		return -1;
	}

	kfree(subif_sync);
	return 0;
}
static int dp_platform_queue_set(int inst, u32 flag)
{
	struct {
		struct cbm_cpu_port_data cpu_data;
		struct ppv4_q_sch_port q_port;
		struct cbm_dp_en_data en_data;
	} *p;
	int cpu_id, cpu_port_num, i;
	u8 f_cpu_q = 0;
	int res;
	struct hal_priv *priv = (struct hal_priv *)dp_port_prop[inst].priv_hal;
	struct pmac_port_info *cpu_port;
	int cbm_inst = dp_port_prop[inst].cbm_inst;
	struct dp_cap *cap;
	u32 flags = 0;
	struct dp_subif_info *subif_info;
	int vap = 0;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	int vap_saved = 0;
	struct dp_subif_info *subif_info_next = NULL;
	struct dp_dflt_hostif hostif = {0};
#endif

	cpu_port = get_dp_port_info(inst, CPU_PORT);
	if (flag & DP_PLATFORM_DE_INIT) {
		pr_err("DPM: %s: Need to free resoruce in the future\n", __func__);
		return DP_SUCCESS;
	}
	cpu_port->alloc_flags = DP_F_CPU;
	dp_gsw_set_port_lu_md(inst, CPU_PORT, DP_F_CPU);

	if (platform_map_to_drop_q(inst)) {
		pr_err("DPM: %s: platform_map_to_drop_q failed\n", __func__);
		return DP_FAILURE;
	}

	if (cpu_mode_table_cfg(inst)) {
		pr_err("DPM: %s: cpu_mode_table_cfg failed\n", __func__);
		return DP_FAILURE;
	}

	cpu_port->status = PORT_SUBIF_REGISTERED;

	p = dp_kzalloc(sizeof(*p), GFP_ATOMIC);
	if (!p)
		return DP_FAILURE;
	/* Alloc queue/scheduler/port per CPU port */
	p->cpu_data.dp_inst = inst;
	p->cpu_data.cbm_inst = cbm_inst;

	if (is_soc_lgm(inst))
		flags = 1;

	if (CBM_OPS(inst, cbm_cpu_port_get, &p->cpu_data, flags)) {
		kfree(p);
		pr_err("DPM: %s fail for CPU Port. Why ???\n", "cbm_cpu_port_get");
		return DP_FAILURE;
	}
	dump_cpu_data(inst, &p->cpu_data);

	cpu_port->rx_policy_base = p->cpu_data.policy_base[0][0];
	cpu_port->rx_policy_num = p->cpu_data.policy_num[0][0];
	cpu_port->tx_policy_base = p->cpu_data.policy_base[0][0];
	cpu_port->tx_policy_num = p->cpu_data.policy_num[0][0];

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	cpu_port->rx_cpu_rmap = p->cpu_data.rmap;
#endif /* end of CONFIG_DPM_DATAPATH_HAL_GSWIP32 && CONFIG_RFS_ACCEL */

	dev_platform_set(inst, CPU_PORT, NULL, 0);
	if (is_soc_lgm(inst)) {
		/* update gpid status table */
		for (i = 0; i < cpu_port->gpid_num; i++) {
			priv->gp_dp_map[cpu_port->gpid_base + i].subif =
				SET_VAP(i, cpu_port->vap_offset,
					cpu_port->vap_mask);
			get_dp_port_subif(cpu_port, i)->gpid =
				cpu_port->gpid_base + i;
		}
	}

	cpu_port->deq_port_base = 0;
	cpu_port->dts_qos = dp_get_qos_cfg(inst, CPU_PORT, DP_F_CPU, 0);
	dp_cpu_init_ok = 1;
	cap = &get_dp_prop_info(inst)->cap;
	if (cap->max_cpu > CQM_MAX_CPU)
		cap->max_cpu = CQM_MAX_CPU;
	if (cap->max_port_per_cpu > MAX_PORTS_PER_CPU)
		cap->max_port_per_cpu = MAX_PORTS_PER_CPU;
	/* For PRX Max CPU 4 and max_port_per_cpu is 1
	 * For LGM Max CPU 4 and max_port_per_cpu is 2
	 */
	cpu_port->deq_port_num = 0;
	for (cpu_id = 0; cpu_id < cap->max_cpu; cpu_id++) {
		for (cpu_port_num = 0; cpu_port_num < cap->max_port_per_cpu;
		     cpu_port_num++) {
			vap = cap->max_port_per_cpu * cpu_id + cpu_port_num;
			if (p->cpu_data.dq_tx_push_info[cpu_id][cpu_port_num].deq_port < 0) {
				/* workaround here to make cat dp/qos works */
				cpu_port->deq_port_num++;
				cpu_port->deq_ports[vap] = -1;
				continue;
			}
			cpu_port->deq_ports[vap] =
				p->cpu_data.dq_tx_push_info[cpu_id][cpu_port_num].deq_port;
			cpu_port->deq_port_num++;

			res = alloc_cpu_q(inst, &p->cpu_data, &p->q_port, cpu_id,
					   cpu_port_num, 0);
			if (res > 0) {
				/* cqm dequeue port is not initialized, ie,
				 * those CPU is offline
				 */
				cpu_port->deq_ports[vap] = -1;
				cpu_port->deq_port_num--;
				continue;
			} else if (res < 0)
				return DP_FAILURE;
			save_subif_info(inst, &p->cpu_data, &p->q_port, cpu_port,
					cpu_id, cpu_port_num);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
			/* Create a dummy subif for DPDK port */
			if (p->cpu_data.type[cpu_id][cpu_port_num] ==
			    CBM_CORE_DPDK) {
				vap_saved = vap;
				subif_info_next =
					get_dp_port_subif(cpu_port, vap + 1);
				subif_info_next->flags = PORT_DEV_REGISTERED;
				subif_info_next->qid = p->q_port.qid;
				subif_info_next->type =
					p->cpu_data.type[cpu_id][cpu_port_num];
				subif_info_next->subif++;
				vap = p->q_port.cqe_deq;
			}

			if (dp_add_pp_gpid(inst, CPU_PORT, vap,
					   cpu_port->gpid_base + vap, 0, 0)) {
				kfree(p);
				pr_err("DPM: dp_alloc_pp_gpid fail for CPU VAP=%d\n",
				       vap);
				return DP_FAILURE;
			}
#endif
			/* Map all CPU port's lookup to one of default CPU's
			 * 1st queue only
			 */
			subif_info = get_dp_port_subif(cpu_port, vap);
			subif_info->port_info = cpu_port;
			if (!f_cpu_q && cpu_id == p->cpu_data.default_cpu) {
				/* only run 1 times per DP instance herre */
				f_cpu_q = 1;
				dflt_q_cpu[inst] = p->q_port.qid;
				dflt_cpu_vap[inst] = vap;
				_dp_init_subif_q_map_rules(subif_info, 0);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
				hostif.inst = inst;
				hostif.gpid = cpu_port->gpid_base + vap;
				hostif.qid = p->q_port.qid;
				hostif.color = PP_COLOR_GREEN;
				if (dp_add_dflt_hostif(&hostif, 0)) {
					kfree(p);
					pr_err("DPM: %s fail for CPU VAP=%d\n",
					       "dp_add_dflt_hostif", vap);
					return -1;
				}
#endif
			}
			/* Note: CPU port no DMA and
			 * don't set en_data.dma_chnl_init to 1
			 */
			p->en_data.cbm_inst = cbm_inst;
			p->en_data.dp_inst = inst;
			p->en_data.deq_port = p->q_port.cqe_deq;
			if (CBM_OPS(inst, cbm_dp_enable, NULL, PMAC_CPU_ID,
				    &p->en_data, 0)) {
				pr_err("DPM: %s: Fail to enable CPU[%d]\n",
				       __func__, p->en_data.deq_port);
				kfree(p);
				return DP_FAILURE;
			}
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
			if (p->cpu_data.type[cpu_id][cpu_port_num] ==
			    CBM_CORE_DPDK && subif_info_next) {
				if (vap == vap_saved)
					subif_info_next->gpid =
						subif_info->gpid;
				else
					subif_info->gpid =
						subif_info_next->gpid;
				cpu_port_num++;
			}
#endif
			dp_cpu_netdev(inst, cpu_id, vap, cpu_port_num, flag);
		}
	}

	/* LGM: Re-Insertion use a New Qid -> Deq Port 28 -> Dma2Tx Ch 1 */
	/* PRX: Re-Insertion use a New Qid -> Deq Port 23 -> Dma1Tx Ch 16 */
	if (dp_reinsert_q_set(inst, cpu_port, &p->cpu_data)) {
		kfree(p);
		pr_err("DPM: %s: DPM Fail to enable re-insertion Q\n", __func__);
		return DP_FAILURE;
	}

	/* set first cpu queue map if needed
	 * In fact, CPU queue mapping is only for FLM, not LGM since LGM's CPU
	 * queue mapping rule is empty
	 */
	subif_info = get_dp_port_subif(cpu_port, dflt_cpu_vap[inst]);
	_dp_set_subif_q_lookup_tbl(subif_info, 0, 0, -1);

	kfree(p);
	return DP_SUCCESS;
}

void dump_hal_priv_info(int inst, struct hal_priv *priv)
{
	DP_DUMP("--hal_priv info[%d]--\n", inst);
	DP_DUMP("deq_port_stat =%lx\n", (unsigned long) priv->deq_port_stat);
	DP_DUMP("qos_queue_stat=%lx\n", (unsigned long) priv->qos_queue_stat);
	DP_DUMP("qos_sch_stat  =%lx\n", (unsigned long) priv->qos_sch_stat);
	DP_DUMP("resv          =%lx\n", (unsigned long) priv->resv);
}

static int dp_platform_reset(int inst, u32 flag)
{
	struct hal_priv *priv;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	struct pmac_port_info *pi = get_dp_port_info(inst, CPU_PORT);
#endif
	struct inst_property *dp_prop = get_dp_port_prop(inst);

	dev_platform_set(inst, CPU_PORT, NULL, DP_F_DEREGISTER);

	priv = (struct hal_priv *)dp_prop->priv_hal;
	if (priv->bp_def)
		dp_gsw_free_bp(inst, priv->bp_def);
	init_ppv4_qos(inst, flag); /* de-initialize qos */
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	free_gpid(inst, pi->gpid_base, pi->gpid_num, pi->gpid_spl);
	pi->gpid_num = 0;
#endif
	kfree(dp_prop->priv_hal);
	dp_prop->priv_hal = NULL;
	return DP_SUCCESS;
}

static int dp_platform_set(int inst, u32 flag)
{
	GSW_QoS_portRemarkingCfg_t *port_remark = NULL;
	struct core_ops *gsw_ops;
	struct qos_ops *gsw_qos;
	struct hal_priv *priv;
	struct pmac_port_info *pi = get_dp_port_info(inst, CPU_PORT);
	struct inst_property *dp_prop = get_dp_port_prop(inst);
	struct bp_pmapper *bp_info;
	struct dp_subif_info *sif;
	int i;

	if (flag & DP_PLATFORM_DE_INIT) /* de-initialize */
		return dp_platform_reset(inst, flag);

	/* For initialize */
	dp_prop->priv_hal = dp_kzalloc(sizeof(*priv), GFP_ATOMIC);
	if (ZERO_OR_NULL_PTR(dp_prop->priv_hal))
		return DP_FAILURE;
	priv = (struct hal_priv *)dp_prop->priv_hal;
	priv->inst = inst;
	/* Set CQM deq port initial status to disbled in our DB*/
	for (i = 0; i < DP_MAX_PPV4_PORT; i++) {
		priv->deq_port_stat[i].disabled = 1;
	}
	if (!dp_prop->ops[0] || !dp_prop->ops[1]) {
		pr_err("DPM: %s: Why GSWIP handle Zero\n", __func__);
		goto ERROR;
	}
	gsw_ops = dp_prop->ops[0];
	gsw_qos = &gsw_ops->gsw_qos_ops;
	if (!inst) {
		/*only inst zero need DMA descriptor */
		init_dma_desc_mask();
		init_dma_pmac_template(CPU_PORT, flag);
		dp_sub_proc_install();
	}

	sif = get_dp_port_subif(pi, 0);
	sif->bp = CPU_BP;
	pi->alloc_flags = DP_F_CPU;
	pi->inst_prop = get_dp_port_prop(inst);
	pi->inst = inst;
	sif->mac_learn_dis = DP_MAC_LEARNING_DIS;
	INIT_LIST_HEAD(&sif->logic_dev);

	priv->bp_def = dp_gsw_alloc_bp(inst, CPU_PORT, CPU_SUBIF,
				       CPU_FID, CPU_BP, 0);
	if (priv->bp_def < 0) {
		pr_err("DPM: %s: dp_gsw_alloc_bp failed\n", __func__);
		goto ERROR;
	}
	dflt_bp[inst] = priv->bp_def;
	/* update domain info with default setting for CPU BP */
	bp_info = get_dp_bp_info(inst, sif->bp);
	sif->domain_id = 0;
	sif->domain_members = DP_DFLT_DOMAIN_MEMBER;
	bp_info->domain_id = sif->domain_id;
	bp_info->domain_member = sif->domain_members;
	DP_DEBUG(DP_DBG_FLAG_DBG, "bp_def[%d]=%d\n",
		 inst, priv->bp_def);

	if (dp_gsw_get_parser(NULL, NULL, NULL, NULL)) {
		pr_err("DPM: %s: dp_get_gsw_parser fail\n", __func__);
		goto ERROR;
	}

	port_remark = dp_kzalloc(sizeof(*port_remark), GFP_ATOMIC);
	if (!port_remark)
		goto ERROR;
	/* disable egress VLAN modification for CPU port */
	port_remark->nPortId = 0;
	if (gsw_qos->QoS_PortRemarkingCfgGet(gsw_ops, port_remark)) {
		pr_err("DPM: %s: GSW_QOS_PORT_REMARKING_CFG_GET failed\n",
		       __func__);
		goto ERROR;
	}

	port_remark->bPCP_EgressRemarkingEnable = 0;
	if (gsw_qos->QoS_PortRemarkingCfgSet(gsw_ops, port_remark)) {
		pr_err("DPM: %s: GSW_QOS_PORT_REMARKING_CFG_SET failed\n",
		       __func__);
		goto ERROR;
	}

	if (init_ppv4_qos(inst, flag)) {
		pr_err("DPM: %s: init_ppv4_qos fail\n", __func__);
		goto ERROR;
	}

	if (dp_platform_queue_set(inst, flag)) {
		pr_err("DPM: %s: dp_platform_queue_set fail\n", __func__);
		goto ERROR;
	}

	if (dp_gsw_color_table_set(inst)) {
		pr_err("DPM: %s: dp_gsw_color_table_set Failed\n",
		       __func__);
		goto ERROR;
	}

	if (dp_gsw_dis_cpu_vlan_md(inst)) {
		pr_err("DPM: %s: dp_gsw_dis_cpu_vlan_md fail\n", __func__);
		goto ERROR;
	}

	if (dp_tx_init(inst)) {
		pr_err("DPM: %s: dp_tx_init fail\n", __func__);
		goto ERROR;
	}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	/* Initialize sererved GPID<->LPID mapping in gp_dp_map table */
	dp_init_reserved_gpid_map(inst);

	/* Explicitely configure reserved GPID<->LPID mapping in CQM.
	 * GSWIP HW already has this mapping, so no need to configure
	 * for GSWIP
	 */
	if (dp_cqm_set_reserved_gpid_map(inst)) {
		pr_err("DPM: %s: dp_cqm_set_default_gpid_map failed\n",
		       __func__);
		goto ERROR;
	}
#endif
	kfree(port_remark);
	return DP_SUCCESS;

ERROR:
	if (dp_prop->priv_hal) {
		kfree(dp_prop->priv_hal);
		dp_prop->priv_hal = NULL;
	}
	if (port_remark)
		kfree(port_remark);
	return DP_FAILURE;
}

/* API to enable GSWIP PCE processing for PON port
 *
 * Note - Later if need to consider de-registration & reset of GSWIP
 * Qos_QueuePort table, store original qid information to DPM port info
 * using traffic class 0 qid
 */
static int dp_gsw_pce_enable_pon(int inst, int ep, u32 flags)
{
	struct qos_ops *gsw_qos;
	GSW_QoS_queuePort_t *q_map;
	int i = 0;
	struct core_ops *gsw_ops = dp_port_prop[inst].ops[0];

	gsw_qos = &gsw_ops->gsw_qos_ops;

	q_map = dp_kzalloc(sizeof(*q_map), GFP_ATOMIC);
	if (!q_map)
		return DP_FAILURE;
	/* Disable Ingress PCE Bypass for Port (PON/Eth) TC 0 .. 15 */
	for (i = 0; i <= 15; i++) {
		q_map->nPortId = ep;
		q_map->nTrafficClassId = i;
		if (gsw_qos->QoS_QueuePortGet(gsw_ops, q_map)) {
			kfree(q_map);
			pr_err("DPM: %s: Fail in QoS_QueuePortGet\n", __func__);
			return DP_FAILURE;
		}
		if (!(flags & DP_F_DEREGISTER))
			q_map->bEnableIngressPceBypass = 0;
		else {
			q_map->bEnableIngressPceBypass = 1;
			q_map->nRedirectPortId = PMAC_2;
		}
		if (gsw_qos->QoS_QueuePortSet(gsw_ops, q_map)) {
			kfree(q_map);
			pr_err("DPM: %s: Fail in QoS_QueuePortSet\n", __func__);
			return DP_FAILURE;
		}
	}
	kfree(q_map);
	return DP_SUCCESS;
}

/* API to redirect packet to PMAC 1 in gswip second loop for PON HGU */
static int dp_gsw_secloop_cfg(int inst, int ep, u32 flags)
{
	struct qos_ops *gsw_qos;
	GSW_QoS_queuePort_t *q_map;
	int i = 0;
	struct core_ops *gsw_ops = dp_port_prop[inst].ops[0];
	struct pon_spl_cfg *cfg = &pon_cfg[0];

	for (i = 0; i < ARRAY_SIZE(pon_cfg); i++) {
		/* will handle de_register here according to flags */
		if ((pon_cfg[i].flag & flags) == flags) {
			cfg = &pon_cfg[i];
			break;
		}
	}

	gsw_qos = &gsw_ops->gsw_qos_ops;

	q_map = dp_kzalloc(sizeof(*q_map), GFP_ATOMIC);
	if (!q_map)
		return DP_FAILURE;
	/* EP=2 remapped to Q 28 .. Q 31 and redirect to Pmac 1 */
	/* if PCE is bypassed, Q is based on reduced traffic class 2 bits
	 * if PCE is not bypassed which is the GSWIP second loop case, traffic
	 * class is 4 bits
	 */
	for (i = 0; i <= 15; i++) {
		q_map->nPortId = ep;
		q_map->nTrafficClassId = i;
		if (gsw_qos->QoS_QueuePortGet(gsw_ops, q_map)) {
			kfree(q_map);
			pr_err("DPM: %s: Fail in QoS_QueuePortGet\n", __func__);
			return DP_FAILURE;
		}

		/* Currently QID is hardcoded, later when
		 * GSWIP default config is ready QID can get from GSWIP
		 * Qos Queue port Get API
		 * traffic class 0 - 3 use Q28 to Q31, and 4 - 15 use Q31 all
		 * redirect to PMAC 1
		 */
		if (i >= 3)
			q_map->nQueueId = cfg->qid + 3;
		else
			q_map->nQueueId = cfg->qid + i;

		if (flags & DP_F_DEREGISTER)
			q_map->nQueueId = cfg->qid;

		q_map->nRedirectPortId = cfg->pmac;
		if (gsw_qos->QoS_QueuePortSet(gsw_ops, q_map)) {
			kfree(q_map);
			pr_err("DPM: %s: Fail in QoS_QueuePortSet\n", __func__);
			return DP_FAILURE;
		}
	}
	kfree(q_map);
	return DP_SUCCESS;
}

static int pon_config(int inst, int ep, struct dp_port_data *data,
		      u32 flags)
{
	struct core_ops *gsw_ops;
	struct common_ops *gsw_com;
	struct mac_ops *mac_ops;
	GSW_CPU_PortCfg_t *cpu_port_cfg;
	struct pon_spl_cfg *cfg = &pon_cfg[0];
	int i;

	for (i = 0; i < ARRAY_SIZE(pon_cfg); i++) {
		/* will handle de_register here according to flags */
		if ((pon_cfg[i].flag & flags) == flags) {
			cfg = &pon_cfg[i];
			break;
		}
	}

	mac_ops = dp_port_prop[inst].mac_ops[ep];
	gsw_ops = dp_port_prop[inst].ops[GSWIP_L];
	gsw_com = &gsw_ops->gsw_common_ops;

	cpu_port_cfg = dp_kzalloc(sizeof(*cpu_port_cfg), GFP_ATOMIC);
	if (!cpu_port_cfg)
		return DP_FAILURE;

	if (gsw_com->CPU_PortCfgGet(gsw_ops, cpu_port_cfg)) {
		kfree(cpu_port_cfg);
		pr_err("DPM: %s: fail in getting CPU port config\n", __func__);
		return DP_FAILURE;
	}
	/* Setting Egress and Ingress Special Tag */
	cpu_port_cfg->nPortId = ep;
	cpu_port_cfg->bSpecialTagIngress = cfg->ingress;
	cpu_port_cfg->bSpecialTagEgress = cfg->egress;
	if (gsw_com->CPU_PortCfgSet(gsw_ops, cpu_port_cfg)) {
		kfree(cpu_port_cfg);
		pr_err("DPM: %s: Fail in configuring CPU port\n", __func__);
		return DP_FAILURE;
	}

	if (is_soc_lgm(inst)) {
		if (dp_gsw_pce_enable_pon(inst, ep, flags)) {
			kfree(cpu_port_cfg);
			return DP_FAILURE;
		}
		if (dp_gsw_secloop_cfg(inst, ep, flags)) {
			kfree(cpu_port_cfg);
			return DP_FAILURE;
		}
	}

	/* Rx CRC check. Value '0'-enable, '1'-disable */
	mac_ops->set_rx_crccheck(mac_ops, cfg->crc_check);

	/* TX FCS generation*/
	if (data->flag_ops & DP_F_DATA_FCS_DISABLE)
		mac_ops->set_fcsgen(mac_ops, cfg->fcs_gen);

	/* RX/TX Flow control */
	mac_ops->set_flow_ctl(mac_ops, cfg->flow_ctrl);

	/* Replace Tx Special Tag for PON registration */
	mac_ops->mac_op_cfg(mac_ops, cfg->tx_spl_tag);

	/* Indicate GSWIP that packet coming from PON have timestamp
	 * In acceleration path, GSWIP can remove the timestamp
	 * during registration
	 */
	mac_ops->mac_op_cfg(mac_ops, cfg->rx_time_stamp);

	/* PON Interface always have a Special Tag from PON -> Xgmac
	 * so should disable the Dummy Special Tag for PON registration
	 */
	mac_ops->mac_op_cfg(mac_ops, cfg->rx_spl_tag);

	/* If PON IP keeps the FCS towards the SoC then the MAC
	 * should not remove the FCS. The FCS is then removed by
	 * the FDMA and FCS is recalculated if packet was modified
	 * in thus way.
	 */
	if (data->flag_ops & DP_F_REM_FCS) {
		mac_ops->mac_op_cfg(mac_ops, RX_FCS_NO_REMOVE);
		mac_ops->mac_op_cfg(mac_ops, TX_FCS_REMOVE);
	}

	if (flags & DP_F_DEREGISTER)
		mac_ops->mac_op_cfg(mac_ops, cfg->rx_fcs);

	/* Reset the MAC, without this reset the downstream from the PON IP
	 * will not work when the MAC is not reset in U-Boot before.
	 */
	mac_ops->soft_restart(mac_ops);

	/* Force the link to the PON IP into up state.
	 * The XGMAC on LGM does not auto detect that it is up.
	 * Without this no traffic or OMCI will pass to the XGAMC.
	 */
	mac_ops->set_link_sts(mac_ops, cfg->link_sts);

	kfree(cpu_port_cfg);
	return DP_SUCCESS;
}

/* Generic API for GSWIP ingress PCE config
 * For now using cfg only for VUNI port
 * if IGP=vUNI/vANI Ingress PCE Bypass disable
 * and if EGP=vUNI/vANI redirect to Pmac_2 to PPv4
 */
static int dp_gsw_pce_enable(int inst, int ep, struct dp_port_data *data,
			     u32 flags)
{
	struct core_ops *gsw_ops;
	struct qos_ops *gsw_qos;
	int j = 0;
	GSW_QoS_queuePort_t *q_map;
	bool ig_bypass = false;
	u32 qid = 0;

	gsw_ops = dp_port_prop[inst].ops[GSWIP_L];
	gsw_qos = &gsw_ops->gsw_qos_ops;

	/* Disable Ingress PCE Bypass for vUNI TC 0 .. 15
	 * EP=12 remapped to Q 12 .. Q 15 and redirect to Pmac 2 and to PPv4
	 */
	q_map = dp_kzalloc(sizeof(*q_map), GFP_ATOMIC);
	if (!q_map)
		return DP_FAILURE;

	/* De-register */
	if (flags & DP_F_DEREGISTER)
		ig_bypass = true;

	for (j = 0; j <= 15; j++) {
		q_map->nPortId = ep;
		q_map->nTrafficClassId = j;
		if (gsw_qos->QoS_QueuePortGet(gsw_ops, q_map)) {
			kfree(q_map);
			pr_err("DPM: %s: Fail in QoS_QueuePortGet\n", __func__);
			return DP_FAILURE;
		}

		q_map->bEnableIngressPceBypass = ig_bypass;
		q_map->nRedirectPortId = PMAC_2;
		if (j == 0)
			qid = q_map->nQueueId;
		else
			q_map->nQueueId = qid;
		if (gsw_qos->QoS_QueuePortSet(gsw_ops, q_map)) {
			kfree(q_map);
			pr_err("DPM: %s: Fail in QoS_QueuePortSet\n", __func__);
			return DP_FAILURE;
		}
	}
	kfree(q_map);
	return DP_SUCCESS;
}

static int dp_port_spl_cfg(int inst, int ep, struct dp_port_data *data,
			   u32 flags)
{
	struct pmac_port_info *port_info = get_dp_port_info(inst, ep);

	if (is_xpon(port_info->alloc_flags))
		pon_config(inst, ep, data, flags);
	else if (is_soc_lgm(inst) && port_info->alloc_flags & DP_F_VUNI)
		dp_gsw_pce_enable(inst, ep, data, flags);

	if (is_soc_lgm(inst) && (data->flag_ops & DP_F_DATA_PCE_PATH_EN)) {
		if (dp_gsw_pce_enable(inst, ep, data, flags))
			return DP_FAILURE;
	}

	if (is_soc_lgm(inst) && (data->flag_ops & DP_F_DATA_PON_HGU_SEC_LOOP)) {
		if (dp_gsw_secloop_cfg(inst, ep, flags))
			return DP_FAILURE;
	}

	return DP_SUCCESS;
}

static int port_platform_reset(int inst, u8 ep,
					struct dp_port_data *data, u32 flags)
{
	struct pmac_port_info *port_info = get_dp_port_info(inst, ep);
	struct cqm_port_info *deq_pinfo;
	int i;

	dp_port_spl_cfg(inst, ep, data, flags);

	if ((port_info->alloc_flags & DP_F_ACA))
		return DP_SUCCESS;

	/* need reset cqm_port_info if no one is using it any more.
	 * For ACA case, it is done during dp_deregister_dev stage via rx ring
	 * So here only handle non-ACA case
	 */
	for (i = 0; i < port_info->deq_port_num; i++) {
		deq_pinfo = get_dp_deqport_info(inst, port_info->deq_ports[i]);
		if (deq_pinfo->ref_cnt)
			continue;
		dp_memset(deq_pinfo, 0, sizeof(*deq_pinfo));
	}

	return DP_SUCCESS;
}

static int port_platform_set(int inst, u8 ep, struct dp_port_data *data,
			     u32 flags)
{
	int i;
	cbm_queue_map_entry_t *lookup;
	struct hal_priv *priv = (struct hal_priv *)dp_port_prop[inst].priv_hal;
	struct pmac_port_info *port_info = get_dp_port_info(inst, ep);
	u32 dma_chan, dma_ch_base;
	struct cqm_port_info *deq_pinfo;

	if (!priv) {
		pr_err("DPM: %s: priv is NULL\n", __func__);
		return DP_FAILURE;
	}
	if (flags & DP_F_DEREGISTER)
		return port_platform_reset(inst, ep, data, flags);

	dp_gsw_set_port_lu_md(inst, ep, flags);
	DP_DEBUG(DP_DBG_FLAG_QOS, "inst: %d, priv=0x%px deq_port_stat=0x%px qdev=0x%px\n",
		 inst, priv,
		 priv ? priv->deq_port_stat : NULL,
		 priv ? priv->qdev : NULL);

	if ((is_xpon(flags)) || (data->flag_ops & DP_F_DATA_NO_LOOP))
		port_info->loop_dis = 1;

	if (flags & DP_F_FAST_WLAN)
		port_info->bp_hairpin_cap = 1;

	if (port_info->deq_port_num) {
		/* For ACA case, we don't know whether its deqeue port yet
		 * since it can be non-continous dequeue port like wave700
		 * alloc_port() deq_port_num = 0 for ACA/DC device
		 *    eg: DSL, Wifi/WAV
		 * alloc_port() deq_port_num > 0 for non ACA/DC device
		 *    eg: DocSiS, GPON, EPON and ETH
		 */
		dma_chan =  port_info->dma_chan;
		dma_ch_base = port_info->dma_chan_tbl_idx;
		for (i = 0; i < port_info->deq_port_num; i++) {
			deq_pinfo = get_dp_deqport_info(inst, port_info->deq_ports[i]);
			deq_pinfo->txpush_addr = port_info->txpush_addr +
						 (port_info->tx_ring_offset * i);
			deq_pinfo->txpush_addr_qos = port_info->txpush_addr_qos +
						     (port_info->tx_ring_offset * i);
			deq_pinfo->tx_ring_size = port_info->tx_ring_size;
			deq_pinfo->tx_pkt_credit = port_info->tx_pkt_credit;

			/* For G.INT num_dma_chan 8 or 16, for other device 1 */
			if (port_info->num_dma_chan > 1) {
				deq_pinfo->f_dma_ch = 1;
				deq_pinfo->dma_chan = dma_chan++;
				deq_pinfo->dma_ch_offset = dma_ch_base + i;
			} else if (port_info->num_dma_chan == 1) {
				deq_pinfo->f_dma_ch = 1;
				deq_pinfo->dma_chan = dma_chan;
				deq_pinfo->dma_ch_offset = dma_ch_base;
			} else {
				deq_pinfo->dma_chan = 0;
				deq_pinfo->dma_ch_offset = 0;
			}
		}
	}

	lookup = dp_kzalloc(sizeof(*lookup), GFP_ATOMIC);
	if (!lookup)
		return DP_FAILURE;
	lookup->ep = ep;

	/* Set all mode based on MPE1/2 to same single mode as specified */
	if (CBM_OPS(inst, cqm_mode_table_set, dp_port_prop[inst].cbm_inst, lookup,
		    port_info->cqe_lu_mode,
		    CBM_QUEUE_MAP_F_MPE1_DONTCARE |
		    CBM_QUEUE_MAP_F_MPE2_DONTCARE)) {
		pr_err("DPM: %s: cqm_mode_table_set failed\n", __func__);
		kfree(lookup);
		return DP_FAILURE;
	}

	dp_port_spl_cfg(inst, ep, data, flags);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (DP_DBG_FLAG_QOS & dp_dbg_flag) {
		for (i = 0; i < port_info->deq_port_num; i++) {
			deq_pinfo = get_dp_deqport_info(inst,
				port_info->deq_ports[i]);
			DP_DUMP("   CQM deq_port[%-3d]: %s = %2d, %s = %3d, %s = 0x%px,\n"
				"%-22s%s = %2d, %s = %3d, %s = 0x%px,\n"
				"%-22s%s = %2d\n",
				port_info->deq_ports[i],
				"f_dma_ch  ", deq_pinfo->f_dma_ch,
				"tx_pkt_credit", deq_pinfo->tx_pkt_credit,
				"tx_push ", deq_pinfo->txpush_addr,
				"", "dma_ch_off", deq_pinfo->dma_ch_offset,
				"tx_ring_size ", deq_pinfo->tx_ring_size,
				"qos_push", deq_pinfo->txpush_addr_qos,
				"", "dma_chan  ", deq_pinfo->dma_chan);
		}
	}
#endif
	kfree(lookup);
	return DP_SUCCESS;
}

static int set_ctp_bp(int inst, int ctp, int portid, int bp,
		      struct subif_platform_data *data)
{
	GSW_CTP_portConfig_t *ctp_cfg;
	struct core_ops *gsw_ops;
	struct inst_info *i_info;
	struct pmac_port_info *port_info;

	ctp_cfg = dp_kzalloc(sizeof(*ctp_cfg), GFP_ATOMIC);
	if (!ctp_cfg)
		return DP_FAILURE;
	port_info = get_dp_port_info(inst, portid);
	i_info = &dp_port_prop[inst].info;
	gsw_ops = dp_port_prop[inst].ops[GSWIP_L];

	ctp_cfg->nLogicalPortId = portid;
	ctp_cfg->nSubIfIdGroup = ctp;
	ctp_cfg->eMask = GSW_CTP_PORT_CONFIG_MASK_BRIDGE_PORT_ID;
	ctp_cfg->nBridgePortId = bp;
	if (gsw_ops->gsw_ctp_ops.CTP_PortConfigSet(gsw_ops, ctp_cfg)) {
		pr_err("DPM: %s: Failed to CTP(%d)'s bridge port=%d for ep=%d\n",
		       __func__, ctp, bp, portid);
		kfree(ctp_cfg);
		return DP_FAILURE;
	}
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	if (!i_info->vlan_aware_flag) {
		kfree(ctp_cfg);
		return DP_SUCCESS;
	}
#endif

	if (!data || !data->subif_data) {
		pr_err("DPM: %s: data NULL or subif_data NULL\n", __func__);
		kfree(ctp_cfg);
		return DP_SUCCESS;
	}

	if (!data->subif_data->ctp_dev) {
		kfree(ctp_cfg);
		return DP_SUCCESS;
	}

	/* Copy first flow entry index incase of VLAN aware Pmapper dev */
	if (dp_gsw_copy_ctp_cfg(inst, bp, NULL,	portid)) {
		DP_DEBUG(DP_DBG_FLAG_REG,
			 "%s: gsw_ctp_cfg copy bp=%d not required\n",
			 __func__, bp);
		kfree(ctp_cfg);
		return DP_SUCCESS;
	}
	kfree(ctp_cfg);
	return DP_SUCCESS;
}

static int reset_ctp_bp(int inst, int ctp, int portid, int bp)
{
	GSW_CTP_portConfig_t *ctp_cfg;
	struct core_ops *gsw_ops = dp_port_prop[inst].ops[GSWIP_L];
	struct pmac_port_info *port_info;
	struct dp_subif_info *sif;
	struct hal_priv *priv = (struct hal_priv *)dp_port_prop[inst].priv_hal;

	ctp_cfg = dp_kzalloc(sizeof(*ctp_cfg), GFP_ATOMIC);
	if (!ctp_cfg)
		return DP_FAILURE;
	port_info = get_dp_port_info(inst, portid);
	sif = get_dp_port_subif(port_info, ctp);

	ctp_cfg->nLogicalPortId = portid;
	ctp_cfg->nSubIfIdGroup = ctp;
	ctp_cfg->nBridgePortId = priv->bp_def; /* change back to bp_def */
	ctp_cfg->eMask = GSW_CTP_PORT_CONFIG_MASK_BRIDGE_PORT_ID;
	if (gsw_ops->gsw_ctp_ops.CTP_PortConfigSet(gsw_ops, ctp_cfg)) {
		pr_err("DPM: %s: Failed to reset CTP(%d)'s bridge port=%d for ep=%d\n",
		       __func__, ctp, bp, portid);
		kfree(ctp_cfg);
		return DP_FAILURE;
	}
	kfree(ctp_cfg);
	return DP_SUCCESS;
}

static int dp_get_subif_share_bp(int inst, int portid)
{
	struct pmac_port_info *port_info;
	struct dp_subif_info *sif;
	int i;

	port_info = get_dp_port_info(inst, portid);
	for (i = 0; i < port_info->subif_max; i++) {
		sif = get_dp_port_subif(port_info, i);
		if (sif->flags)
			return sif->bp;
	}

	return DP_FAILURE;
}

static
int alloc_q(int inst, struct ppv4_q_sch_port *q_port,
	    struct cqm_port_info *deq_pinfo)
{
	struct q_info *q_info;

	DP_DEBUG(DP_DBG_FLAG_QOS,
		 "Queue decision: %s\n", "auto_new_queue");

	if (alloc_q_to_port(q_port, 0)) {
		pr_err("DPM: %s: %s fail for dp_port=%d\n",
		       __func__, "alloc_q_to_port", q_port->dp_port);
		return DP_FAILURE;
	}

	q_info = get_dp_q_info(inst, q_port->qid);
	if (q_info->flag) {
		pr_err("DPM: %s: Why dp_q_tbl[%d][%d].flag =%d%s?\n",
		       __func__, inst, q_port->qid, q_info->flag, ":expect 0");
		return DP_FAILURE;
	}

	if (q_info->ref_cnt) {
		pr_err("DPM: %s: Why dp_q_tbl[%d][%d].ref_cnt =%d%s?\n",
		       __func__, inst, q_port->qid, q_info->ref_cnt,
		       ":expect 0");
		return DP_FAILURE;
	}

	/* update queue table */
	q_info->flag = 1;
	q_info->need_free = 1;
	q_info->ref_cnt = 1;
	q_info->q_node_id = q_port->q_node;
	q_info->cqm_dequeue_port = q_port->cqe_deq;

	if (!deq_pinfo->f_first_qid) {
		deq_pinfo->first_qid = q_port->qid;
		deq_pinfo->f_first_qid = 1;
	}

	DP_DEBUG(DP_DBG_FLAG_REG,
		 "%s: %s=%d %s=%d q[%d].cnt=%d\n",
		 "new_queue_alloc",
		 "dp_port", q_port->dp_port,
		 "vap", q_port->ctp,
		 q_port->qid, get_dp_q_info(inst, q_port->qid)->ref_cnt);

	return DP_SUCCESS;
}

static
int free_q(int inst, int dp_port, int qid, struct cqm_port_info *deq_pinfo,
	      int cqm_deq_port)
{
	struct q_info *q_info;
	struct dp_node_alloc *node;

	q_info = get_dp_q_info(inst, qid);
	if (!q_info->need_free)
		return DP_FAILURE;

	if (!q_info->ref_cnt) {
		pr_err("DPM: %s: Why dp_q_tbl[%d][%d].ref_cnt Zero:expect > 0\n",
		       __func__, inst, qid);
		return DP_FAILURE;
	}
	node = dp_kzalloc(sizeof(*node), GFP_ATOMIC);
	if (!node)
		return DP_FAILURE;

	q_info->ref_cnt--;
	if (q_info->flag && !q_info->ref_cnt) {
		DP_DEBUG(DP_DBG_FLAG_REG, "Free qid %d\n", qid);
		node->id.q_id = qid;
		/*if no subif using this queue, need to delete it*/
		node->inst = inst;
		node->dp_port = dp_port;
		node->type = DP_NODE_QUEUE;
		dp_node_free(node, 0);

		/*update dp_q_tbl*/
		q_info->flag = 0;
		q_info->need_free = 0;
	} else {
		kfree(node);
		DP_DEBUG(DP_DBG_FLAG_QOS, "q_id[%d] dont need to be freed\n",
			 qid);
		return DP_FAILURE;
	}

	if (deq_pinfo->f_first_qid && deq_pinfo->first_qid == qid) {
		deq_pinfo->f_first_qid = 0;
		deq_pinfo->first_qid = 0;
		DP_DEBUG(DP_DBG_FLAG_QOS, "q_id[%d] is freed\n", qid);
	}
	if (!deq_pinfo->ref_cnt) {
		node->id.cqm_deq_port = cqm_deq_port;
		/*if no subif using this queue, need to delete it*/
		node->inst = inst;
		node->dp_port = dp_port;
		node->type = DP_NODE_PORT;
		dp_node_free(node, 0);
		DP_DEBUG(DP_DBG_FLAG_QOS,
			 "cqm_port[%d] is freed\n", cqm_deq_port);
	} else {
		DP_DEBUG(DP_DBG_FLAG_QOS,
			 "cqm_port[%d] dont need to be freed\n", cqm_deq_port);
	}

	kfree(node);
	return DP_SUCCESS;
}

static int deq_update_info(struct dp_subif_upd_info *info)
{
	int old_cqm_deq_idx, old_num_cqm_deq, old_cqm_deq_port;
	int i;
	static cbm_queue_map_entry_t *lookup;
	struct cqm_port_info *deq_pinfo;
	int subif_ix;
	u32 dma_ch_offset = 0;
	struct ppv4_q_sch_port *q_port;
	struct dp_subif_info *sif;
	struct pmac_port_info *port_info;
	struct q_info *q_info;

	port_info = get_dp_port_info(info->inst, info->dp_port);
	subif_ix = GET_VAP(info->subif, port_info->vap_offset,
			   port_info->vap_mask);
	sif = get_dp_port_subif(port_info, subif_ix);
	old_cqm_deq_idx = sif->cqm_port_idx;
	old_num_cqm_deq = sif->num_qid;

	q_port = dp_kzalloc(sizeof(*q_port), GFP_ATOMIC);
	if (!q_port)
		return DP_FAILURE;
	q_port->inst = info->inst;
	q_port->dp_port = info->dp_port;
	q_port->ctp = subif_ix;

	lookup = dp_kzalloc(sizeof(*lookup), GFP_ATOMIC);
	if (!lookup) {
		kfree(q_port);
		return DP_FAILURE;
	}
	/* update new deq info */
	for (i = 0; i < info->new_num_cqm_deq; i++) {
		q_port->cqe_deq = port_info->deq_ports[i + info->new_cqm_deq_idx];

		deq_pinfo = get_dp_deqport_info(info->inst, q_port->cqe_deq);

		/* allocate new queue (example), if tcont is changed */
		if (!deq_pinfo->ref_cnt) {
			if (alloc_q(info->inst, q_port, deq_pinfo)) {
				kfree(lookup);
				kfree(q_port);
				return DP_FAILURE;
			}
		} else {
			q_port->qid = deq_pinfo->first_qid;
			q_info = get_dp_q_info(info->inst, q_port->qid);
			q_port->q_node = q_info->q_node_id;

			get_dp_q_info(info->inst, q_port->qid)->ref_cnt++;
		}

		dma_ch_offset = deq_pinfo->dma_ch_offset;

		DP_DEBUG(DP_DBG_FLAG_REG,
			 "cqm_p[%d].cnt=%d\n", q_port->cqe_deq,
			 deq_pinfo->ref_cnt);
		/* update subif table with new info*/
		sif->qid_list[i] = q_port->qid;
		sif->q_node[i] = q_port->q_node;
		sif->qos_deq_port[i] = q_port->port_node;
		sif->cqm_deq_port[i] = q_port->cqe_deq;
		if (!(info->flags & DP_F_UPDATE_NO_Q_MAP)) {
			/* Map this port's lookup to its 1st queue only */
			/* reset q map for this subif */
			_dp_reset_subif_q_lookup_tbl(sif, i, -1);
			/* set default queue map again */
			_dp_set_subif_q_lookup_tbl(sif, i, 0, -1);
		}
		/* enable cqm DEQ port*/
		if (dp_cbm_deq_port_enable(port_info->owner, info->inst,
					   info->dp_port, q_port->cqe_deq,
					   info->new_num_cqm_deq, 0,
					   dma_ch_offset)) {
			kfree(lookup);
			kfree(q_port);
			return DP_FAILURE;
		}

		deq_pinfo->ref_cnt++;
		if (port_info->num_dma_chan)
			atomic_inc(&(dp_dma_chan_tbl[info->inst] +
				     dma_ch_offset)->ref_cnt);
	}

	/* remove old deq port info */
	for (i = 0; i < old_num_cqm_deq; i++) {
		old_cqm_deq_port = port_info->deq_ports[old_cqm_deq_idx + i];
		deq_pinfo = get_dp_deqport_info(info->inst,
						old_cqm_deq_port);

		deq_pinfo->ref_cnt--;
		free_q(info->inst, info->dp_port, deq_pinfo->first_qid,
		       deq_pinfo, old_cqm_deq_port);

		dma_ch_offset = deq_pinfo->dma_ch_offset;
		if (port_info->num_dma_chan)
			atomic_dec(&(dp_dma_chan_tbl[info->inst] +
				     dma_ch_offset)->ref_cnt);
		if (dp_cbm_deq_port_enable(port_info->owner, info->inst,
					   info->dp_port, old_cqm_deq_port,
					   old_num_cqm_deq, CBM_PORT_F_DISABLE,
					   dma_ch_offset)) {
			kfree(q_port);
			kfree(lookup);
			return DP_FAILURE;
		}
	}
	sif->cqm_port_idx = info->new_cqm_deq_idx;
	sif->num_qid = info->new_num_cqm_deq;

	kfree(q_port);
	kfree(lookup);
	return DP_SUCCESS;
}

static
void subif_pmapper_cfg(int inst, struct subif_platform_data *data,
		       int bp, int portid, int subif_idx, int flags)
{
	struct bp_pmapper *bp_info;
	struct pmac_port_info *port_info;
	struct dp_subif_info *sif;

	port_info = get_dp_port_info(inst, portid);
	sif = get_dp_port_subif(port_info, subif_idx);

	bp_info = get_dp_bp_info(inst, bp);

	if (flags & DP_F_DEREGISTER) {
		dp_ctp_dev_list_del(&bp_info->ctp_dev, sif->ctp_dev);
		sif->ctp_dev = NULL;
		bp_info->ref_cnt--;
		if (!bp_info->ref_cnt) {
			bp_info->dev = NULL;
			bp_info->flag = 0;
			DP_DEBUG(DP_DBG_FLAG_REG,
				 "ctp ref_cnt become zero:%s\n",
				 sif->device_name);
		}
	} else {
		DP_DEBUG(DP_DBG_FLAG_REG,
			 "dp_bp_tbl[%d][%d]=%s current ref_cnt=%d\n",
			 inst, bp, data->dev->name, bp_info->ref_cnt);
		if (!bp_info->ref_cnt)
			INIT_LIST_HEAD(&bp_info->ctp_dev);
		bp_info->dev = data->dev;
		bp_info->ref_cnt++;
		bp_info->flag = 1;
		sif->ctp_dev = data->subif_data->ctp_dev;
		dp_ctp_dev_list_add(&bp_info->ctp_dev, sif->ctp_dev, bp,
				    subif_idx);
	}

	DP_DEBUG(DP_DBG_FLAG_REG,
		 "inst=%d subif_ix=%d pmapper.cnt=%d\n",
		 inst, subif_idx, bp_info->ref_cnt);
}

static int subif_bp_cfg(int inst, struct subif_platform_data *data,
			int portid, int subif_idx, int flags)
{
	struct pmac_port_info *port_info;
	struct dp_subif_info *sif;
	int bp = -1;
	struct bp_pmapper *bp_info;
	bool bp_cfg = false;
	int vap = 0;

	port_info = get_dp_port_info(inst, portid);
	sif = get_dp_port_subif(port_info, subif_idx);

	/* CTP Max = 1 for DSL case now */
	if ((port_info->ctp_max == 1) &&
	    (port_info->alloc_flags & DP_F_FAST_DSL)) {
		if (!port_info->num_subif)
			bp_cfg = true;
	} else {
		vap = subif_idx;
		bp_cfg = true;
	}

	/* De-Register */
	if (flags & DP_F_DEREGISTER) {
		bp = sif->bp;
		if (sif->ctp_dev)
			subif_pmapper_cfg(inst, data, bp, portid, subif_idx,
					  flags);
		if (bp_cfg) {
			if (reset_ctp_bp(inst, vap, portid, bp))
				return DP_FAILURE;

			bp_info = get_dp_bp_info(inst, bp);
			if (!bp_info->ref_cnt) {
				bp_info->domain_id = 0;
				bp_info->domain_member = DP_DFLT_DOMAIN_MEMBER;
				/* Reset the bridge member port list */
				dp_memset(bp_info->bp_member_map, 0,
					  sizeof(bp_info->bp_member_map));
				if (dp_gsw_free_bp(inst, bp))
					return DP_FAILURE;
			}
		}
		return DP_SUCCESS;
	}

	/* Register */
	if (data->subif_data->ctp_dev)
		bp = bp_pmapper_get(inst, data->dev);

	if (bp < 0) {
		sif->mac_learn_dis = data->subif_data->mac_learn_disable;
		if (port_info->ctp_max == 1 && port_info->num_subif &&
		    (port_info->alloc_flags & DP_F_FAST_DSL))
			bp = dp_get_subif_share_bp(inst, portid);
		else
			bp = dp_gsw_alloc_bp(inst, portid, subif_idx,
					     CPU_FID, CPU_BP,
					     data->subif_data->flag_ops);
		if (bp < 0) {
			pr_err("DPM: %s: Fail to alloc bridge port\n", __func__);
			return DP_FAILURE;
		}
	}

	sif->bp = bp;

	if (data->subif_data->ctp_dev)
		subif_pmapper_cfg(inst, data, bp, portid, subif_idx, flags);

	bp_info = get_dp_bp_info(inst, bp);
	if (data->subif_data->flag_ops & DP_SUBIF_BR_DOMAIN) {
		sif->domain_id = data->subif_data->domain_id;
		sif->domain_members = data->subif_data->domain_members;
	} else {
		sif->domain_id = 0;
		sif->domain_members = DP_DFLT_DOMAIN_MEMBER;
	}
	bp_info->domain_id = sif->domain_id;
	bp_info->domain_member = sif->domain_members;
	dp_memset(bp_info->bp_member_map, 0, sizeof(bp_info->bp_member_map));

	if (bp_cfg)
		set_ctp_bp(inst, vap, portid, bp, data);

	return DP_SUCCESS;
}

static int subif_hw_set(int inst, int portid, int subif_ix,
			struct subif_platform_data *data, u32 flags)
{
	struct ppv4_q_sch_port *q_port;
	int deq_port_idx = 0, i = 0, j, k;
	int dma_ch_offset = 0;
	struct pmac_port_info *port_info;
	struct dp_subif_info *sif;
	struct hal_priv *priv = HAL(inst);
	struct cqm_port_info *deq_pinfo;
	struct q_info *q_info;
	int subif = 0;
	int tmp_deqpid;
	struct dp_gpid_tx_info *gpid_info;
	u16 ctp_base, max_ctp;
	u8 found;

	if (!data || !data->subif_data) {
		pr_err("DPM: %s: data NULL or subif_data NULL\n", __func__);
		return DP_FAILURE;
	}
	if (!dp_dma_chan_tbl[inst]) {
		pr_err("DPM: %s: dp_dma_chan_tbl[%d] NULL\n", __func__, inst);
		return DP_FAILURE;
	}

	if (flags & DP_F_SUBIF_LOGICAL) {
		pr_err("DPM: %s: need more for logical dev??\n", __func__);
		return DP_SUCCESS;
	}

	if (!priv) {
		pr_err("DPM: %s: priv NULL\n", __func__);
		return DP_FAILURE;
	}

	if (data->subif_data->flag_ops & DP_SUBIF_DEQPORT_NUM &&
	    data->subif_data->num_deq_port > DP_MAX_DEQ_PER_SUBIF) {
		pr_err("DPM: %s: %s(%d), cannot be more than max Q per subif %d\n",
		       __func__, "deq_port", data->subif_data->num_deq_port,
		       DP_MAX_DEQ_PER_SUBIF);
		return DP_FAILURE;
	}

	port_info = get_dp_port_info(inst, portid);
	sif = get_dp_port_subif(port_info, subif_ix);

	if (data->subif_data)
		deq_port_idx = data->subif_data->deq_port_idx;

	if (port_info->deq_port_num < deq_port_idx + 1) {
		pr_err("DPM: %s: Wrong deq_port_idx(%d), should < %d\n",
		       __func__, deq_port_idx, port_info->deq_port_num);
		return DP_FAILURE;
	}

	if (subif_bp_cfg(inst, data, portid, subif_ix, flags)) {
		pr_err("DPM: %s: Bp Config failed\n", __func__);
		return DP_FAILURE;
	}

	if (is_soc_lgm(inst)) {
		subif = SET_VAP(subif_ix, port_info->vap_offset,
				port_info->vap_mask);

		gpid_info = &data->subif_data->gpid_tx_info;

		if (gpid_info->f_min_pkt_len)
			sif->min_pkt_len = gpid_info->min_pkt_len;
		else
			sif->min_pkt_len = 0;

		sif->min_pkt_len_cfg = get_min_pkt_len_cfg(sif->min_pkt_len);
		if (sif->min_pkt_len_cfg >= PP_NUM_TX_MIN_PKT_LENS) {
			pr_err("DPM: %s: unsupported min_pkt_len = %u\n",
			       __func__, sif->min_pkt_len);
			return DP_FAILURE;
		}
	}

	data->act = 0;

	q_port = dp_kzalloc(sizeof(*q_port), GFP_ATOMIC);
	if (!q_port)
		return DP_FAILURE;
	/* QUEUE_CFG if needed */
	q_port->cqe_deq = port_info->deq_ports[deq_port_idx];

	_DP_DEBUG(DP_DBG_FLAG_QOS, "DPM: %s: %s=%d %s=0x%px %s=0x%px %s=0x%px %s=%d\n",
		 __func__,
		 "cqe_deq", q_port->cqe_deq,
		 "priv", priv,
		 "deq_port_stat", priv ? priv->deq_port_stat : NULL,
		 "qdev", priv ? priv->qdev : NULL,
		 "inst", inst);

	for (i = 0; i < data->subif_data->num_deq_port; i++) {
		q_port->cqe_deq =
			port_info->deq_ports[deq_port_idx + i];

		deq_pinfo = get_dp_deqport_info(inst, q_port->cqe_deq);

		q_port->tx_pkt_credit = deq_pinfo->tx_pkt_credit;
		q_port->tx_ring_addr = deq_pinfo->txpush_addr_qos;
		q_port->tx_ring_addr_push = deq_pinfo->txpush_addr;
		q_port->tx_ring_size = deq_pinfo->tx_ring_size;
		q_port->inst = inst;
		q_port->dp_port = portid;
		q_port->ctp = subif_ix;

		dma_ch_offset = deq_pinfo->dma_ch_offset;
		if (!(data->subif_data->flag_ops &
		      (DP_SUBIF_SPECIFIC_Q | DP_SUBIF_AUTO_NEW_Q))) {
			if (!deq_pinfo->f_first_qid)
				data->subif_data->flag_ops |= DP_SUBIF_AUTO_NEW_Q;
		}

		if (data->subif_data->flag_ops & DP_SUBIF_AUTO_NEW_Q) {
			if (alloc_q(inst, q_port, deq_pinfo)) {
				kfree(q_port);
				return DP_FAILURE;
			}
		} else if (data->subif_data->flag_ops & DP_SUBIF_SPECIFIC_Q) {
			DP_DEBUG(DP_DBG_FLAG_QOS,
				 "Queue decision: %s\n", "specified_queue");

			/* Single GEM or 1 Subif have only 1 Qid
			 * Multiple GEM or Multiple Subif can have same Qid
			 */
			q_info = get_dp_q_info(inst, data->subif_data->q_id);

			if (!q_info->flag) {
				q_info->need_free = 0; /* caller Q,so no free */
				q_info->cqm_dequeue_port = q_port->cqe_deq;
			} else {
				/* Multiple subif can have same Q, we increment
				 * Q reference count if Queue created by DPM
				 */
				q_info->ref_cnt++;
			}

			q_port->qid = data->subif_data->q_id;
			q_port->q_node = q_info->q_node_id;
		} else {
			DP_DEBUG(DP_DBG_FLAG_QOS,
				 "Queue decision:%s\n", "shared_queue");

			/* auto sharing queue: if go to here,
			 * it means sharing queue
			 * is ready and it is created by previous
			 * dp_register_subif_ext
			 */

			/* get already stored q_node_id/qos_port id to q_port */
			q_port->qid = deq_pinfo->first_qid;
			q_info = get_dp_q_info(inst, q_port->qid);
			q_port->q_node = q_info->q_node_id;
			get_dp_q_info(inst, q_port->qid)->ref_cnt++;
			q_port->port_node = priv->deq_port_stat[q_port->cqe_deq].node_id;
		}

		deq_pinfo->ref_cnt++;

		if (port_info->num_dma_chan)
			atomic_inc(&(dp_dma_chan_tbl[inst] +
				     dma_ch_offset)->ref_cnt);

		_DP_DEBUG(DP_DBG_FLAG_REG,
			 "DPM: %s: %s=%d %s=%d q[%d].cnt=%d cqm_p[%d].cnt=%d %s=%d\n",
			 __func__,
			 "dp_port", portid,
			 "vap", subif_ix,
			 q_port->qid, get_dp_q_info(inst, q_port->qid)->ref_cnt,
			 q_port->cqe_deq, deq_pinfo->ref_cnt,
			 "tx_dma_chan ref",
			 atomic_read(&(dp_dma_chan_tbl[inst] +
				       dma_ch_offset)->ref_cnt));

		/* first CTP */
		if (deq_pinfo->ref_cnt == 1)
			data->act = TRIGGER_CQE_DP_ENABLE;

		/* Increase the txring.deqport and rxring.deqport refcnt
		 * for the case of 2 type deq ports(meaning deq ports, DC deq
		 * ports) coexist.
		 * Eg: docsis case
		 */
		for (j = 0; j < port_info->num_tx_ring; j++)
		{
			tmp_deqpid = port_info->tx_ring[j].tx_deq_port;
			found = 0;
			for (k = 0; k < port_info->deq_port_num; k++)
			{
				if (port_info->deq_ports[k] == tmp_deqpid) {
					found = 1;
					break;
				}
			}
			if (!found) {
				/*found = 0; Not found in deq_ports[x], so new port*/
				deq_pinfo = get_dp_deqport_info(inst, tmp_deqpid);
				deq_pinfo->ref_cnt++;
			}
		}

		/* May be rxring.deq_port ref count inc is not useful
		 * as it goes to GSWIP. but anyway incrementing for now.
		 * TODO Need to revisit
		 */
		for (j = 0; j < port_info->num_rx_ring; j++)
		{
			/* There may have 8 Dequeue port and 8 DMA to GSIWP to serve diff priority*/
			for (k = 0; k < port_info->rx_ring[j].num_out_cqm_deq_port; k++) {
				tmp_deqpid = port_info->rx_ring[j].out_cqm_deq_port_id + k;
				deq_pinfo = get_dp_deqport_info(inst, tmp_deqpid);
				deq_pinfo->ref_cnt++;
			}
		}

		/* update caller dp_subif_data.q_id with allocated queue num */
		data->subif_data->q_id = q_port->qid;
		/* update subif table */
		sif->qid_list[i] = q_port->qid;
		sif->q_node[i] = q_port->q_node;
		sif->qos_deq_port[i] = q_port->port_node;
		sif->cqm_deq_port[i] = q_port->cqe_deq;
		sif->cqm_port_idx = deq_port_idx;
		port_info->subif_info[subif_ix].cqm_port_idx = deq_port_idx;
		DP_CB(inst, get_itf_start_end)(port_info->itf_info, &ctp_base, &max_ctp);
		sif->ctp_base = ctp_base;
		sif->ctp = (ctp_base + subif_ix) < max_ctp ?
				ctp_base + subif_ix : max_ctp;
	}
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	if (dp_subif_pp_set(inst, portid, subif_ix,
			    data->subif_data->flag_ops)) {
		pr_err("DPM: %s: dp_subif_pp_set fail for dpid/vap=%d/%d\n",
		       __func__, portid, subif_ix);
		kfree(q_port);
		return DP_FAILURE;
	}
#endif
	kfree(q_port);
	return DP_SUCCESS;
}

static int subif_hw_reset(int inst, int portid, int subif_ix,
			  struct subif_platform_data *data, u32 flags)
{
	int qid, idx, i, k;
	int cqm_deq_port;
	int dma_ch_offset;
	struct pmac_port_info *port_info = get_dp_port_info(inst, portid);
	struct dp_subif_info *sif = get_dp_port_subif(port_info, subif_ix);
	struct cqm_port_info *deq_pinfo;
	struct bp_pmapper *bp_info;
	struct q_info *q_info;
	u8 found;

	if (!dp_dma_chan_tbl[inst]) {
		pr_err("DPM: %s: dp_dma_chan_tbl[%d] NULL\n", __func__, inst);
		return DP_FAILURE;
	}

	/* Note: If use old flow, it does not work for Wave700 sharing txin case
	 *       , ie, reset ctp/bp/GPID multiple times
	 *       Not remember why original code is done like this flow, is it for
	 *       G.INT special handling, one subif use multiple queue ??
	 *       Current move to here from below for loop of num_qid
	 */
	bp_info = get_dp_bp_info(inst, sif->bp);
#ifndef USE_OLD_FLOW
	if (sif->ctp_dev && !bp_info->ref_cnt) {
		pr_err("DPM: %s: Why dp_bp_tbl[%d][%d].ref_cnt =%d\n",
		       __func__, inst, sif->bp, bp_info->ref_cnt);
		return DP_FAILURE;
	}
	if (subif_bp_cfg(inst, data, portid, subif_ix, flags)) {
		pr_err("DPM: %s: subif_bp_cfg reset fail: portid=%d vap=%d\n",
		       __func__, portid, subif_ix);
		return DP_FAILURE;
	}
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	if (dp_subif_pp_reset(inst, portid, subif_ix,
			      sif->data_flag_ops)) {
		pr_err("DPM: %s: dp_subif_pp_reset fail: portid=%d vap=%d\n",
		       __func__, portid, subif_ix);
		return DP_FAILURE;
	}
#endif
#endif
	for (idx = 0; idx < sif->num_qid; idx++) {
		/*note: later need to check virtual device de-registration
		 *      which not required to set/reset q_map
		 *      By the way, if no EPON HW Limitation,
		 *      _dp_reset_subif_q_lookup_tbl should not be in the queue
		 *      for loop here
		 */
		_dp_reset_subif_q_lookup_tbl(sif, idx, -1);
		qid = sif->qid_list[idx];
		cqm_deq_port = sif->cqm_deq_port[idx];
		deq_pinfo = get_dp_deqport_info(inst, cqm_deq_port);
		dma_ch_offset = deq_pinfo->dma_ch_offset;
		q_info = get_dp_q_info(inst, qid);

		/* santity check table */
		if (!deq_pinfo->ref_cnt) {
			pr_err("DPM: %s: Why dp_deq_port_tbl[%d][%d].ref_cnt Zero\n",
			       __func__, inst, cqm_deq_port);
			return DP_FAILURE;
		}
#ifdef USE_OLD_FLOW
		if (sif->ctp_dev && !bp_info->ref_cnt) {
			pr_err("DPM: %s: Why dp_bp_tbl[%d][%d].ref_cnt =%d\n",
			       __func__, inst, sif->bp, bp_info->ref_cnt);
			return DP_FAILURE;
		}
#endif
		/* update queue/port/sched/bp_pmapper table's ref_cnt */
		deq_pinfo->ref_cnt--;

		/* Decrease the txring.deqport and rxring.deqport refcnt
		 * for the case of 2 type deq ports(meaning deq ports, DC deq
		 * ports) coexist.
		 * Eg: docsis case
		 */
		for (i = 0; i < port_info->num_tx_ring; i++)
		{
			found = 0;
			for (k = 0; k < port_info->deq_port_num; k++)
			{
				if (port_info->deq_ports[k] ==
						port_info->tx_ring[i].tx_deq_port) {
					found = 1;
					break;
				}
			}
			if (!found) {
				/*found = 0; Not found in deq_ports[x], so new port*/
				deq_pinfo = get_dp_deqport_info(inst, port_info->tx_ring[i].tx_deq_port);
				deq_pinfo->ref_cnt--;
			}
		}

		/* May be rxring.deq_port ref count inc is not useful
		 * as it goes to GSWIP. but anyway decrementing for now.
		 * TODO Need to revisit
		 */
		for (i = 0; i < port_info->num_rx_ring; i++)
		{
			/* There may have 8 Dequeue port and 8 DMA to GSIWP to serve diff priority*/
			for (k = 0; k < port_info->rx_ring[i].num_out_cqm_deq_port; k++) {
				get_dp_deqport_info(inst, port_info->rx_ring[i].out_cqm_deq_port_id + k)->ref_cnt--;
			}
		}

		if (port_info->num_dma_chan)
			atomic_dec(&(dp_dma_chan_tbl[inst] +
				     dma_ch_offset)->ref_cnt);

#ifdef USE_OLD_FLOW
		/* Cannot reset BP for all qid */
		if (!idx) {
			if (subif_bp_cfg(inst, data, portid, subif_ix, flags))
				return DP_FAILURE;
		}
#endif
		cqm_deq_port = q_info->cqm_dequeue_port;
		deq_pinfo = get_dp_deqport_info(inst, cqm_deq_port);

		free_q(inst, portid, qid, deq_pinfo, cqm_deq_port);

		DP_DEBUG(DP_DBG_FLAG_REG,
			 "%s:%s=%d %s=%d q[%d].cnt=%d cqm_p[%d].cnt=%d %s=%d\n",
			 __func__,
			 "dp_port", portid,
			 "vap", subif_ix,
			 qid, q_info->ref_cnt,
			 cqm_deq_port, deq_pinfo->ref_cnt,
			 "tx_dma_chan_ref",
			 atomic_read(&(dp_dma_chan_tbl[inst] +
				       dma_ch_offset)->ref_cnt));

		if (!port_info->num_subif && deq_pinfo->ref_cnt) {
			pr_err("DPM: %s: num_subif(%d) not match %s[%d][%d].ref_cnt\n",
			       __func__,
			       port_info->num_subif,
			       "dp_deq_port", inst, cqm_deq_port);
			return DP_FAILURE;
		}
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
#ifdef USE_OLD_FLOW
		if (dp_subif_pp_reset(inst, portid, subif_ix,
				      sif->data_flag_ops)) {
			pr_err("DPM: %s: dp_subif_pp_reset failed\n", __func__);
			return DP_FAILURE;
		}
#endif
#endif
	}
	return DP_SUCCESS;
}

/*Set basic BP/CTP */
static int subif_platform_set(int inst, int portid, int subif_ix,
			      struct subif_platform_data *data, u32 flags)
{
	if (flags & DP_F_DEREGISTER)
		return subif_hw_reset(inst, portid, subif_ix, data, flags);
	return subif_hw_set(inst, portid, subif_ix, data, flags);
}

static int supported_logic_dev(int inst, struct net_device *dev,
			       char *subif_name)
{
	return is_vlan_dev(dev);
}

static int subif_platform_set_unexplicit(int inst, int port_id,
		struct logic_dev *logic_dev, u32 flags)
{
	if (flags & DP_F_DEREGISTER) {
		dp_gsw_free_bp(inst, logic_dev->bp);
	} else {
		logic_dev->bp =
			dp_gsw_alloc_bp(inst, port_id, logic_dev->ctp,
					CPU_FID, CPU_BP, flags);
	}

	return DP_SUCCESS;
}

static int dp_update_current_bp_map(int inst, int bp)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	GSW_BRIDGE_portConfig_t *brportcfg;
	struct core_ops *gsw_ops;
	struct brdgport_ops *gsw_bp;
	int i;
	struct bp_pmapper *bp_info;

	gsw_ops = dp_port_prop[inst].ops[GSWIP_R];
	gsw_bp = &gsw_ops->gsw_brdgport_ops;
	brportcfg = dp_kzalloc(sizeof(*brportcfg), GFP_ATOMIC);
	if (!brportcfg)
		return DP_FAILURE;

	/* update bridge port members based on domain info to the current bport */
	brportcfg->nBridgePortId = bp;
	brportcfg->eMask = GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP;
	if (gsw_bp->BridgePort_ConfigGet(gsw_ops, brportcfg)) {
		kfree(brportcfg);
		DP_DEBUG(DP_DBG_FLAG_REG,
			 "BridgePort_ConfigGet fail:bp=%d\n", bp);
		return DP_FAILURE;
	}

	bp_info = get_dp_bp_info(inst, bp);
	for (i = 0; i < MAX_BP_NUM; i++) {
		if (GET_BP_MAP(bp_info->bp_member_map, i)) {
			if (is_valid_domain(inst, bp, i))
				SET_BP_MAP(brportcfg->nBridgePortMap, i);
			else
				UNSET_BP_MAP(brportcfg->nBridgePortMap, i);
		}
	}

	brportcfg->eMask = GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP;
	if (gsw_bp->BridgePort_ConfigSet(gsw_ops, brportcfg)) {
		kfree(brportcfg);
		pr_err("DPM: %s Fail to configure bridge port\n", __func__);
		return DP_FAILURE;
	}

	kfree(brportcfg);
#endif
	return DP_SUCCESS;
}

static int dp_update_bp_map(int inst, int bp, int cfg_bport)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	GSW_BRIDGE_portConfig_t *brportcfg;
	struct core_ops *gsw_ops;
	struct brdgport_ops *gsw_bp;

	gsw_ops = dp_port_prop[inst].ops[GSWIP_R];
	gsw_bp = &gsw_ops->gsw_brdgport_ops;
	brportcfg = dp_kzalloc(sizeof(*brportcfg), GFP_ATOMIC);
	if (!brportcfg)
		return DP_FAILURE;

	/* update bridge port members based on domain info to the current bport */
	brportcfg->nBridgePortId = bp;
	brportcfg->eMask = GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP;
	if (gsw_bp->BridgePort_ConfigGet(gsw_ops, brportcfg)) {
		kfree(brportcfg);
		DP_DEBUG(DP_DBG_FLAG_REG,
			 "BridgePort_ConfigGet fail:bp=%d\n", bp);
		return DP_FAILURE;
	}

	if (is_valid_domain(inst, bp, cfg_bport)) {
		SET_BP_MAP(brportcfg->nBridgePortMap, cfg_bport);
	} else {
		UNSET_BP_MAP(brportcfg->nBridgePortMap, cfg_bport);
	}

	brportcfg->eMask = GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP;
	if (gsw_bp->BridgePort_ConfigSet(gsw_ops, brportcfg)) {
		kfree(brportcfg);
		pr_err("DPM: %s Fail to configure bridge port\n", __func__);
		return DP_FAILURE;
	}

	kfree(brportcfg);
#endif
	return DP_SUCCESS;
}

int dp_cfg_domain_for_bp_member(int inst, int bp)
{
	int ret = DP_FAILURE, i;
	struct bp_pmapper *bp_info;

	bp_info = get_dp_bp_info(inst, bp);
	for (i = 0; i < MAX_BP_NUM; i++) {
		if (GET_BP_MAP(bp_info->bp_member_map, i)) {
			/* update bridge member port map with current bp */
			ret = dp_update_bp_map(inst, i, bp);
			if (ret)
				goto exit;
		}
	}
	/* update current bridge port map */
	ret = dp_update_current_bp_map(inst, bp);

exit:
	return ret;
}

static int dp_ctp_tc_map_set(struct dp_tc_cfg *tc, int flag,
			     struct dp_meter_subif *mtr_subif)
{
	struct core_ops *gsw_ops = dp_port_prop[mtr_subif->inst].ops[0];
	struct ctp_ops *gsw_ctp;
	GSW_CTP_portConfig_t *ctp_tc_cfg;

	if (!mtr_subif) {
		pr_err("DPM: %s: mtr_subif struct NULL\n", __func__);
		return DP_FAILURE;
	}

	if (mtr_subif->subif.flag_pmapper) {
		pr_err("DPM: %s: Cannot support ctp tc set for pmmapper dev(%s)\n",
		       __func__,
		       tc->dev ? tc->dev->name : "NULL");
		return DP_FAILURE;
	}

	ctp_tc_cfg = dp_kzalloc(sizeof(*ctp_tc_cfg), GFP_ATOMIC);
	if (!ctp_tc_cfg)
		return DP_FAILURE;
	gsw_ctp = &gsw_ops->gsw_ctp_ops;
	ctp_tc_cfg->nLogicalPortId = mtr_subif->subif.port_id;
	ctp_tc_cfg->nSubIfIdGroup = mtr_subif->subif.subif;

	if (gsw_ctp->CTP_PortConfigGet(gsw_ops, ctp_tc_cfg)) {
		pr_err("DPM: %s: Failed to get CTP info for %s=%d %s=%d\n",
		       __func__,
		       "ep", mtr_subif->subif.port_id,
		       "subif", mtr_subif->subif.subif);
		kfree(ctp_tc_cfg);
		return DP_FAILURE;
	}

	ctp_tc_cfg->eMask = GSW_CTP_PORT_CONFIG_MASK_FORCE_TRAFFIC_CLASS;
	ctp_tc_cfg->nDefaultTrafficClass = tc->tc;
	if (tc->force)
		ctp_tc_cfg->bForcedTrafficClass = tc->force;
	else
		ctp_tc_cfg->bForcedTrafficClass = 0;

	if (gsw_ctp->CTP_PortConfigSet(gsw_ops, ctp_tc_cfg)) {
		pr_err("DPM: %s: CTP tc set fail for %s=%d %s=%d %s=%d %s=%d\n",
		       __func__,
		       "ep", mtr_subif->subif.port_id,
		       "subif", mtr_subif->subif.subif,
		       "tc", tc->tc,
		       "force", tc->force);
		kfree(ctp_tc_cfg);
		return DP_FAILURE;
	}
	kfree(ctp_tc_cfg);
	return DP_SUCCESS;
}

int dp_get_reinsert_cnt2(int inst, int dp_port, int vap, int flag,
			struct dp_reinsert_count *dp_reins_count)
{
	struct pmac_port_info *port_info = get_dp_port_info(inst, dp_port);
	struct dp_subif_info *sif = get_dp_port_subif(port_info, vap);

	spin_lock_bh(&port_info->mib_cnt_lock);
	dp_memcpy(dp_reins_count, &sif->reins_cnt,	sizeof(*dp_reins_count));
	DP_DEBUG(DP_DBG_FLAG_DBG, "Reinsert counters\n");
	spin_unlock_bh(&port_info->mib_cnt_lock);

	return DP_SUCCESS;
}

int dp_get_lct_cnt2(int inst, int dp_port, int flag,
		   u32 *lct_idx, struct dp_lct_rx_cnt *dp_lct_rx_count)
{
	struct pmac_port_info *port_info = get_dp_port_info(inst, dp_port);
	struct dp_lct_rx_cnt *stats;

	spin_lock_bh(&port_info->mib_cnt_lock);
	stats = port_info->lct_rx_cnt;

	if (!stats) {
		spin_unlock_bh(&port_info->mib_cnt_lock);
		return DP_ERR_NULL_DATA;
	}
	*lct_idx = port_info->lct_idx;
	dp_memcpy(dp_lct_rx_count, stats, 2 * sizeof(*dp_lct_rx_count));
	DP_DEBUG(DP_DBG_FLAG_DBG, "LCT rx counters\n");
	spin_unlock_bh(&port_info->mib_cnt_lock);

	return DP_SUCCESS;
}

static int not_valid_rx_ep(int ep)
{
	return (((ep >= 3) && (ep <= 6)) || (ep == 2) || (ep > 15));
}

static void set_pmac_subif(struct pmac_tx_hdr *pmac, int32_t subif)
{
	pmac->src_dst_subif_id_lsb = subif & 0xff;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	pmac->src_dst_subif_id_msb = (subif >> 8) & 0x0f;
	pmac->src_dst_subif_id_msb |= (((subif & 0x8000) >> 15) << 4);
	pmac->src_dst_subif_id_14_12 = (subif & 0x7000) >> 12;
#elif IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	pmac->src_dst_subif_id_msb = (subif >> 8) & 0x1f;
#endif
}

static void get_dma_pmac_templ(int index, struct pmac_tx_hdr *pmac,
			       struct dma_tx_desc_0 *desc_0,
			       struct dma_tx_desc_1 *desc_1,
			       struct pmac_port_info *dp_info)
{
	struct dma_tx_desc_0 *dma0_mask =
		(struct dma_tx_desc_0 *)&dp_info->desc_dw_mask[0][index];
	struct dma_tx_desc_1 *dma1_mask =
		(struct dma_tx_desc_1 *)&dp_info->desc_dw_mask[1][index];
	struct dma_tx_desc_0 *dma0_templ =
		(struct dma_tx_desc_0 *)&dp_info->desc_dw_templ[0][index];
	struct dma_tx_desc_1 *dma1_templ =
		(struct dma_tx_desc_1 *)&dp_info->desc_dw_templ[1][index];

	if (likely(pmac))
		dp_memcpy(pmac, &dp_info->pmac_template[index], sizeof(*pmac));

	desc_0->all = (desc_0->all & dma0_mask->all) | dma0_templ->all;
	desc_1->all = (desc_1->all & dma1_mask->all) | dma1_templ->all;
}

static int check_csum_cap(void)
{
	return DP_SUCCESS;
}

static int get_itf_start_end(struct gsw_itf *itf_info, u16 *start, u16 *end)
{
	if (!itf_info)
		return DP_FAILURE;
	if (start)
		*start = itf_info->start;
	if (end)
		*end = itf_info->end;

	return 0;
}

static int dev_platform_set_aca_rxout_queue(int inst, u8 ep, uint32_t flags)
{
	struct ppv4_q_sch_port *q_port;
	struct cqm_port_info *deq;
	struct pmac_port_info *port = get_dp_port_info(inst, ep);
	struct dp_rx_ring_egp *egp;
	int i, j, ret = DP_FAILURE;

	if (!(port->alloc_flags & DP_F_ACA))
		return DP_SUCCESS;
	q_port = dp_kzalloc(sizeof(*q_port), GFP_ATOMIC);
	if (!q_port)
		return DP_FAILURE;
	if (flags & DP_F_DEREGISTER) {
		for (i = 0; i < port->num_rx_ring; i++) {
			for (j = 0; j < port->rx_ring[i].num_egp; j++) {
				/* free queue/deq of rxout ring in QOS mode */
				q_port->cqe_deq = port->rx_ring[i].egp[j].deq_port;
				deq = get_dp_deqport_info(inst, q_port->cqe_deq);
				if (deq->ref_cnt) /* already alloc */
					deq->ref_cnt--;
				else {
					pr_err("DPM: %s: deq->ref_cnt zeor\n",
					       __func__);
					goto EXIT;
				}
				if (deq->ref_cnt)
					continue;
				free_q(inst, CPU_PORT, deq->first_qid,
				       deq, q_port->cqe_deq);
				dp_memset(deq, 0, sizeof(*deq));
			}
		}
		goto EXIT;
	}

	for (i = 0; i < port->num_rx_ring; i++) {
		for (j = 0; j < port->rx_ring[i].num_egp; j++) {
			q_port->cqe_deq = port->rx_ring[i].egp[j].deq_port;
			deq = get_dp_deqport_info(inst, q_port->cqe_deq);
			egp = &port->rx_ring[i].egp[j];
			if (deq->ref_cnt) /* queue already alloc */ {
				if (deq->txpush_addr != egp->txpush_addr) {
					pr_err("DPM: %s: txpush_addr wrong:0x%px!=0x%px\n",
					       __func__,
					       deq->txpush_addr,
					       egp->txpush_addr);
					goto EXIT;
				}
				if (deq->txpush_addr_qos != egp->txpush_addr_qos) {
					pr_err("DPM: %s: txpush_addr_qos wrong:0x%px!=0x%px\n",
					       __func__,
					       deq->txpush_addr_qos,
					       egp->txpush_addr_qos);
					goto EXIT;
				}
				if (deq->tx_ring_size != egp->tx_ring_size) {
					pr_err("DPM: %s: tx_ring_size wrong:%d!=%d\n",
					       __func__,
					       deq->tx_ring_size,
					       egp->tx_ring_size);
					goto EXIT;
				}
				if (deq->tx_pkt_credit != egp->tx_pkt_credit) {
					pr_err("DPM: %s: tx_pkt_credit wrong:%d!=%d\n",
					       __func__,
					       deq->tx_pkt_credit,
					       egp->tx_pkt_credit);
					goto EXIT;
				}
				if (deq->dp_port[shared_dc_deq_dpid] == 0) {
					pr_err("DPM: %s: txpush_addr wrong for dpid: %d has refcnt: 0\n",
					       __func__,
					       shared_dc_deq_dpid);
					goto EXIT;
				}
				deq->ref_cnt++;

				continue;
			}
			/* update to deq table */
			deq->txpush_addr = egp->txpush_addr;
			deq->txpush_addr_qos = egp->txpush_addr_qos;
			deq->tx_ring_size = egp->tx_ring_size;
			deq->tx_pkt_credit = egp->tx_pkt_credit;
			deq->dp_port[shared_dc_deq_dpid] = 1;
			deq->dts_qos = dp_get_inter_qos_cfg(inst, q_port->cqe_deq);

			/* create queue/port */
			q_port->tx_pkt_credit = deq->tx_pkt_credit;
			q_port->tx_ring_addr = deq->txpush_addr_qos;
			q_port->tx_ring_addr_push = deq->txpush_addr;
			q_port->tx_ring_size = deq->tx_ring_size;
			q_port->inst = inst;
			/* This RXOUT EGP may share with other DC port.
			 * So here purposely put under CPU Port
			 */
			q_port->dp_port = shared_dc_deq_dpid;
			q_port->ctp = 0;

			if (alloc_q(inst, q_port, deq)) {
				goto EXIT;
			}
			deq->ref_cnt++;
			deq->flag = CQM_PORT_INFO_DC_RX_Q;
		}
	}
	ret = DP_SUCCESS;
EXIT:

	kfree(q_port);
	return ret;
}

int dp_get_deq_port_flag(int inst, int deq_port)
{
	struct cqm_port_info *deq_pinfo;

	deq_pinfo = get_dp_deqport_info(inst, deq_port);
	return deq_pinfo->flag;
}

int register_dp_capability(int flag)
{
	struct dp_hw_cap *cap;

	cap = dp_kzalloc(sizeof(*cap), GFP_ATOMIC);
	if (!cap)
		return DP_FAILURE;
	cap->info.dp_platform_set = dp_platform_set;
	cap->info.port_platform_set = port_platform_set;
	cap->info.dev_platform_set = dev_platform_set;
	cap->info.subif_platform_set_unexplicit = subif_platform_set_unexplicit;
	cap->info.init_dma_pmac_template = init_dma_pmac_template;
	cap->info.subif_platform_set = subif_platform_set;
	cap->info.subif_platform_change_mtu = subif_platform_change_mtu;
	cap->info.not_valid_rx_ep = not_valid_rx_ep;
	cap->info.set_pmac_subif = set_pmac_subif;
	cap->info.check_csum_cap = check_csum_cap;
	cap->info.get_dma_pmac_templ = get_dma_pmac_templ;
	cap->info.set_dma_pmac_reins_templ = set_dma_pmac_reins_templ;
	cap->info.get_itf_start_end = get_itf_start_end;
	cap->info.dump_rx_dma_desc = dump_rx_dma_desc;
	cap->info.dump_tx_dma_desc = dump_tx_dma_desc;
	cap->info.dump_rx_pmac = dump_rx_pmac;
	cap->info.dump_tx_pmac = dump_tx_pmac;
	cap->info.supported_logic_dev = supported_logic_dev;
	cap->info.dp_pmac_set = dp_gsw_set_pmac;
	cap->info.dp_set_gsw_parser = dp_gsw_set_parser;
	cap->info.dp_get_gsw_parser = dp_gsw_get_parser;
	cap->info.dp_qos_platform_set = qos_platform_set;
	cap->info.dp_set_gsw_pmapper = dp_gsw_set_pmapper;
	cap->info.dp_get_gsw_pmapper = dp_gsw_get_pmapper;
	cap->info.dp_ctp_tc_map_set = dp_ctp_tc_map_set;
	cap->info.dp_meter_alloc = dp_gsw_alloc_meter;
	cap->info.dp_meter_add = dp_gsw_add_meter;
	cap->info.dp_meter_del = dp_gsw_del_meter;
	cap->info.dp_set_bp_attr = dp_gsw_set_bp_attr;

	cap->info.dp_alloc_bridge_port = dp_gsw_alloc_bp;
	cap->info.dp_free_bridge_port = dp_gsw_free_bp;
	cap->info.dp_deq_update_info = deq_update_info;
	cap->info.dp_set_ctp_bp = set_ctp_bp;
	cap->info.dp_get_queue_mib = dp_get_queue_mib;
	cap->info.dp_cfg_domain_for_bp_member = dp_cfg_domain_for_bp_member;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	cap->info.swdev_flag = 1;
	cap->info.swdev_alloc_bridge_id = dp_swdev_alloc_bridge_id;
	cap->info.swdev_free_brcfg = dp_swdev_free_brcfg;
	cap->info.swdev_bridge_cfg_set = dp_swdev_bridge_cfg_set;
	cap->info.swdev_bridge_port_cfg_reset = dp_swdev_bridge_port_cfg_reset;
	cap->info.swdev_bridge_port_cfg_set = dp_swdev_bridge_port_cfg_set;
	cap->info.swdev_bridge_port_flags_set = dp_swdev_bridge_port_flags_set;
	cap->info.swdev_bridge_port_flags_get = dp_swdev_bridge_port_flags_get;
	cap->info.dp_mac_set = dp_gsw_add_mac_entry;
	cap->info.dp_mac_reset = dp_gsw_del_mac_entry;
	cap->info.dp_cfg_vlan = dp_gswip_ext_vlan; /* for symmetric VLAN */
#endif
	cap->info.cap.hw_tso = 0;
	cap->info.cap.hw_gso = 0;
	dp_strlcpy(cap->info.cap.qos_eng_name, "ppv4",
		   sizeof(cap->info.cap.qos_eng_name));
	dp_strlcpy(cap->info.cap.pkt_eng_name, "mpe",
		   sizeof(cap->info.cap.pkt_eng_name));
	cap->info.cap.max_num_queues = DP_MAX_QUEUE_NUM;
	cap->info.cap.max_num_scheds = DP_MAX_NODES;
	cap->info.cap.max_num_qos_ports = DP_MAX_PPV4_PORT;
	cap->info.cap.max_num_deq_ports = DP_MAX_CQM_DEQ;
	cap->info.cap.max_num_subif_per_port = MAX_SUBIF_PER_PORT;
	cap->info.cap.max_cpu = CQM_MAX_CPU;
	cap->info.dp_tc_vlan_set = tc_vlan_set;
	cap->info.dp_rx = _dp_rx;
	cap->info.dp_tx = _dp_tx;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	cap->info.cap.max_port_per_cpu = 2;
	cap->info.cap.max_num_spl_conn = MAX_SPL_CONN_CNT;
	cap->info.dp_spl_conn = _dp_spl_conn;
	cap->info.dp_spl_conn_get = _dp_spl_conn_get;
	cap->info.dp_set_io_port = dp_set_io_port;
	cap->info.dp_get_port_vap_mib = dp_get_port_vap_mib;
#elif IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	cap->info.cap.max_port_per_cpu = 1;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	cap->info.vlan_aware_flag = 1;
#ifndef DP_SWITCHDEV_NO_HACKING
	cap->info.swdev_port_learning_limit_set =
		dp_swdev_port_learning_limit_set;
	cap->info.swdev_bridge_mcast_flood =
		dp_swdev_bridge_mcast_flood;
#endif
#endif
	/* The full ethtool statistics support is provided in mib_counters
	 * kernel module where special callback fn is registered for the
	 * statistics purpose
	 */
	cap->info.dp_net_dev_get_ethtool_stats = NULL;
#endif
	cap->info.proc_print_ctp_bp_info = proc_print_ctp_bp_info;
	cap->info.dp_get_deq_port_flag = dp_get_deq_port_flag;

	if (dp_get_gswip_cap(cap, flag)) {
		kfree(cap);
		pr_err("DPM: %s: dp_get_gswip_cap fail\n", __func__);
		return DP_FAILURE;
	}

	if (register_dp_hw_cap(cap, flag)) {
		kfree(cap);
		pr_err("DPM: %s: Why register_dp_hw_cap fail\n", __func__);
		return DP_FAILURE;
	}
	kfree(cap);
	return DP_SUCCESS;
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP
