// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2022, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <linux/bitfield.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/etherdevice.h>
#include <net/datapath_api.h>
#include "../datapath.h"
#include "../datapath_tx.h"
#include "../datapath_instance.h"
#include "../datapath_trace.h"
#include "datapath_misc.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

static int dp_reinsert_pkt_cnt(struct pmac_port_info *dp_info, int vap,
			       struct sk_buff *skb)
{
	struct dp_reinsert_count *stats;
	int len = skb->len + ETH_FCS_LEN;

	DP_DEBUG(DP_DBG_FLAG_DBG, "len=%d vap=%d\n", len, vap);
	stats = &get_dp_port_subif(dp_info, vap)->reins_cnt;
	spin_lock_bh(&dp_info->mib_cnt_lock);
	if (len <= 64) {
		stats->dp_64BytePkts++;
	} else if ((len <= 127) && (len >= 65)) {
		stats->dp_127BytePkts++;
	} else if ((len <= 255) && (len >= 128)) {
		stats->dp_255BytePkts++;
	} else if ((len <= 511) && (len >= 256)) {
		stats->dp_511BytePkts++;
	} else if ((len <= 1023) && (len >= 512)) {
		stats->dp_1023BytePkts++;
	} else if ((len <= 1518) && (len >= 1024)) {
		stats->dp_MaxBytePkts++;
	} else if (len >= 1519) {
		stats->dp_OversizeGoodPkts++;
	}

	if (is_broadcast_ether_addr(skb->data))
		stats->dp_BroadcastPkts++;
	else if (is_multicast_ether_addr(skb->data))
		stats->dp_MulticastPkts++;
	else
		stats->dp_UnicastPkts++;

	stats->dp_GoodPkts++;
	stats->dp_GoodBytes += len;

	spin_unlock_bh(&dp_info->mib_cnt_lock);

	return DP_SUCCESS;
}

static int dp_set_reinsert_dma_pmac(struct sk_buff *skb,
				    struct dp_tx_common *cmn)
{
	int inst = 0;
	struct dma_rx_desc_0 *desc_0 = (struct dma_tx_desc_0 *)&skb->DW0;
	struct dma_rx_desc_1 *desc_1 = (struct dma_tx_desc_1 *)&skb->DW1;
	struct dma_rx_desc_3 *desc_3 = (struct dma_tx_desc_3 *)&skb->DW3;
	struct pmac_tx_hdr *pmac = (struct pmac_tx_hdr	*)cmn->pmac;
	struct cqm_port_info *cqm_info;
	u8 cid, pid;
	u16 nid;
	int src_port, src_subif;

	src_port = desc_1->field.ep;
	src_subif = desc_0->field.dest_sub_if_id;
	desc_3->field.data_len = skb->len;

	DP_CB(inst, set_dma_pmac_reins_templ)(desc_0, desc_1, pmac);
	desc_1->field.classid = (skb->priority >= 15) ? 15 : skb->priority;
	/* For Re-Insertion LGM CPU -> DMA -> GSWIP use DMA Ctrl 2 Tx Channel 1
	 * For Re-Insertion Prx CPU -> DMA -> GSWIP use DMA Ctrl 1 Tx Channel 16
	 * PortID in Dma Descriptor will be used by SDMA
	 * Real Re-Insertion IGP used for PCE processing is in PMAC Header
	 */
	cqm_info = get_dp_deqport_info(inst, reinsert_deq_port[0]);
	dp_dma_parse_id(cqm_info->dma_chan, &cid, &pid, &nid);
	desc_1->field.ep = dp_get_pmac_id(inst, cid);

	SET_PMAC_IGP_EGP(pmac, src_port);
	DP_CB(inst, set_pmac_subif)(pmac, src_subif);
	cmn->pmac_len = PMAC_TX_HDR_SIZE;

	return DP_SUCCESS;
}

static void dp_set_chksum(struct pmac_tx_hdr *pmac, u32 tcp_type,
			  u32 ip_offset, u32 tcp_h_offset)
{
	pmac->tcp_type = tcp_type;
	pmac->ip_offset = ip_offset >> 1;
	pmac->tcp_h_offset = tcp_h_offset >> 2;
}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
static int pp_tx(struct sk_buff *skb, struct dp_tx_common *cmn, void *p)
{
	pp_tx_pkt_hook(skb, ((struct dma_tx_desc_1 *)&skb->DW1)->field.ep);
	return DP_TX_FN_CONTINUE;
}
#endif

int dp_tx_init(int inst)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	if (IS_ENABLED(CONFIG_PPV4) && IS_ENABLED(CONFIG_MXL_SKB_EXT))
		dp_tx_register_process(DP_TX_PP, pp_tx, NULL);
#endif

	return dp_tx_update_list();
}

static bool can_offload_csum(struct sk_buff *skb,
			     struct dp_tx_common_ex *ex,
			     struct dma_rx_desc_0 *desc_0,
			     struct dma_rx_desc_1 *desc_1)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	struct dp_checksum_info *csum = &ex->csum_info;
#endif

	/* Checksum offload is not supported in HW for GSWIP31 */
	if (is_soc_prx(0))
		return false;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	if (unlikely(get_offset_clear_chksum(skb, &csum->ip_offset,
					     &csum->tcp_h_offset,
					     &csum->tcp_type))) {
#if (IS_ENABLED(CONFIG_DPM_DATAPATH_EXTRA_DEBUG))
		DP_INFO("packet can't do hw checksum\n");
#endif
		return false;
	}
#endif

	return true;
}

static void process_alloc_flag(struct sk_buff *skb,
			       struct dp_tx_common_ex *ex,
			       struct dma_rx_desc_0 *desc_0,
			       struct dma_rx_desc_1 *desc_1)
{
	unsigned long bits = ex->port->alloc_flags;
	int i, class;
	struct pmac_tx_hdr *pmac = dp_tx_get_pmac(ex);

	for_each_set_bit(i, &bits, __bf_shf(DP_F_DEV_END)) {
		switch (i) {
		case __bf_shf(DP_F_FAST_DSL):
			if (ex->cmn.flags & DP_TX_DSL_FCS)
				ex->tmpl = TEMPL_OTHERS;
			else
				ex->cmn.pmac_len = 0;
			break;
		case __bf_shf(DP_F_FAST_WLAN):
			ex->cmn.pmac_len = 0;
			break;
		case __bf_shf(DP_F_EPON):
			/* For EPON subifid set as Deq Port Idx + Class */
			if (skb->priority <= ex->sif->num_qid)
				class = skb->priority;
			else
				class = ex->sif->num_qid - 1;
			ex->cmn.subif = ex->sif->cqm_port_idx + class;
			desc_0->field.dest_sub_if_id = ex->cmn.subif;
			DP_CB(ex->cmn.inst, set_pmac_subif)(pmac, ex->cmn.subif);
			break;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
		case __bf_shf(DP_F_VUNI):
			ex->cmn.flags &= ~DP_TX_BYPASS_QOS;
			break;
#endif
		default:
			break;
		}
	}
}

static void update_pmac_and_descs(struct sk_buff *skb,
				  struct dp_tx_common_ex *ex,
				  struct dma_rx_desc_0 *desc_0,
				  struct dma_rx_desc_1 *desc_1,
				  int rec_id)
{
	struct pmac_tx_hdr *pmac = dp_tx_get_pmac(ex);
	struct dp_checksum_info *csum = &ex->csum_info;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	volatile const struct pmac_port_info *pinfo;
#endif

	/* reset all descriptors dw0 and dw1 */
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	desc_0->all &= tx_desc_mask[0];
#endif
	desc_1->all &= tx_desc_mask[1];

	DP_CB(ex->cmn.inst, get_dma_pmac_templ)(ex->tmpl, pmac, desc_0,
						desc_1, ex->port);

	if (ex->tmpl == TEMPL_CHECKSUM ||
	    ex->tmpl == TEMPL_CHECKSUM_PTP)
		dp_set_chksum(pmac, csum->tcp_type, csum->ip_offset,
			      csum->tcp_h_offset);

	if (ex->tmpl == TEMPL_PTP ||
	    ex->tmpl == TEMPL_CHECKSUM_PTP)
		pmac->record_id_msb = rec_id;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	if (likely(desc_1->field.pmac)) {
		DP_CB(ex->cmn.inst, set_pmac_subif)(pmac, ex->cmn.subif);
		ex->cmn.pmac_len = sizeof(struct pmac_tx_hdr);
	}
#else
	/* Subifid is taken from PMAC header for Egress Port 2/3/4 */
	if (likely(ex->cmn.pmac_len))
		DP_CB(ex->cmn.inst, set_pmac_subif)(pmac, ex->cmn.subif);
	else
		dp_memset(ex->cmn.pmac, 0, sizeof(struct pmac_tx_hdr));
#endif

	desc_1->field.classid = (skb->priority >= 15) ? 15 : skb->priority;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	if (ex->sif->f_spl_gpid)
		desc_1->field.ep = ex->port->gpid_spl;
	else
		desc_1->field.ep = ex->sif->gpid;
#endif

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	/* workaround to avoid packet accumulating in GSWIP
	 * if color is critial (0), change to green (1) to allow drop
	 */
	if (!desc_1->field.color)
		desc_1->field.color = 1;

	/* ClassID need to be there in DW0 for QoS Desc 31:28,
	 * this is common for all ports except DOCSIS and desc formats
	 */
	pinfo = ex->sif->port_info;
	if (pinfo && pinfo->oob_class_size != 0)
		desc_0->qos_fmt.classid = desc_1->field.classid;

	/* For Streaming ports in LGM B-step PSBKEEP=1
	 * Need to copy EgFlag to DW0[27] and H flag to DW0[26]
	 * to make sure DMA to QoS Desc conversion is proper
	 * if PSBKEEP=0, HW will copy H, class and EgFlag to DW0[31:26]
	 */
	if (is_stream_port(ex->port->alloc_flags)) {
		desc_0->qos_fmt.egflag = desc_1->field.redir;
		desc_0->qos_fmt.h_mode = desc_1->field.header_mode;
	}
#endif

}

static void set_dma_desc(struct sk_buff *skb, struct dp_tx_common_ex *ex)
{
	struct dma_rx_desc_0 *desc_0 = (struct dma_tx_desc_0 *)&skb->DW0;
	struct dma_rx_desc_1 *desc_1 = (struct dma_tx_desc_1 *)&skb->DW1;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	struct dma_rx_desc_2 *desc_2 = (struct dma_tx_desc_2 *)&skb->DW2;
#endif
	struct dma_rx_desc_3 *desc_3 = (struct dma_tx_desc_3 *)&skb->DW3;
	int rec_id = -1;

	ex->tmpl = TEMPL_NORMAL;
	ex->cmn.pmac_len = 0;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	/* No PMAC for WAVE500 and DSL by default except bonding case */
	if (unlikely(NO_NEED_PMAC(ex->port->alloc_flags)))
		ex->cmn.pmac_len = 0;
	else
		ex->cmn.pmac_len = sizeof(struct pmac_tx_hdr);
#endif

	process_alloc_flag(skb, ex, desc_0, desc_1);

	if (skb->ip_summed == CHECKSUM_PARTIAL) {
		if (can_offload_csum(skb, ex, desc_0, desc_1))
			ex->tmpl = TEMPL_CHECKSUM;
		else
			skb_checksum_help(skb);
	}

	if (ex->port->f_ptp) {
		rec_id = do_tx_hwtstamp(ex->cmn.inst, ex->port->port_id, skb);
		if (rec_id >= 0) {
			/* if Checksum need for PTP packet */
			if (ex->tmpl == TEMPL_CHECKSUM)
				ex->tmpl = TEMPL_CHECKSUM_PTP;
			else
				ex->tmpl = TEMPL_PTP;
		}
	}

	/* keep DIC bit to support test tool */
	if (desc_3->field.dic)
		desc_3->field.dic = 1;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	if (ex->cmn.flags & DP_TX_OAM) /* OAM */
		desc_3->field.pdu_type = 1;

	desc_2->field.data_ptr = (unsigned long) skb->data;
#endif
	desc_3->field.data_len = skb->len;

	update_pmac_and_descs(skb, ex, desc_0, desc_1, rec_id);
}

int _dp_tx(struct sk_buff *skb, struct dp_tx_common *cmn)
{
	struct dp_tx_common_ex *ex =
		container_of(cmn, struct dp_tx_common_ex, cmn);
	int ret = 0, vap;

	vap = GET_VAP(ex->cmn.subif, ex->port->vap_offset, ex->port->vap_mask);
	if (unlikely(dp_dbg_flag))
		dp_tx_dbg("\nOrig", skb, ex);

	trace_dp_tx(ex->port->port_id, (struct dma_tx_desc_0 *)&skb->DW0,
				(struct dma_tx_desc_1 *)&skb->DW1,
				(struct dma_tx_desc_2 *)&skb->DW2,
				(struct dma_tx_desc_3 *)&skb->DW3, skb, true, vap);
	if (ex->cmn.flags & DP_TX_WITH_PMAC) {
		trace_dp_tx_pmac((struct pmac_tx_hdr *)((unsigned long)skb->data - PMAC_TX_HDR_SIZE), true);
	}
	if (likely(!(ex->cmn.flags & (DP_TX_BYPASS_FLOW | DP_TX_INSERT)))) {
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
		if (ex->sif->f_spl_gpid)
			ex->cmn.flags |= DP_TX_BYPASS_QOS;
#endif
		set_dma_desc(skb, ex);
	}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	/*Set toe_tc value from subif_info*/
	cmn->toe_tc = ex->sif->toe_tc;
#endif

	/* suppose caller already store pmac header from the address of
	 * skb->data-16 to skb->data-1 if with flag DP_TX_WITH_PMAC
	 */
	if (ex->cmn.flags & DP_TX_WITH_PMAC) {
		cmn->pmac_len = sizeof(struct pmac_tx_hdr);
		dp_memcpy(cmn->pmac,
			  (u8 *)((unsigned long)skb->data - PMAC_TX_HDR_SIZE),
			  PMAC_TX_HDR_SIZE);
		DP_CB(ex->cmn.inst, set_pmac_subif)((struct pmac_tx_hdr *)
						    cmn->pmac, ex->cmn.subif);
	}

	if (ex->cmn.flags & DP_TX_INSERT) {
		dp_set_reinsert_dma_pmac(skb, cmn);
		dp_reinsert_pkt_cnt(ex->port, vap, skb);
	}

	if (unlikely(dp_dbg_flag))
		dp_tx_dbg("\nAfter", skb, ex);

	trace_dp_tx(ex->port->port_id, (struct dma_tx_desc_0 *)&skb->DW0,
				(struct dma_tx_desc_1 *)&skb->DW1,
				(struct dma_tx_desc_2 *)&skb->DW2,
				(struct dma_tx_desc_3 *)&skb->DW3, skb, false, vap);
	trace_dp_tx_pmac(dp_tx_get_pmac(ex), false);

	ret = dp_tx_start(skb, cmn);

	return ret;
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP
