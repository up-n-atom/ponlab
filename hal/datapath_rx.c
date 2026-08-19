// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2024, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <linux/bitfield.h>
#include <linux/kernel.h>
#include <net/datapath_api.h>

#include "../datapath.h"
#include "datapath_misc.h"
#include "../datapath_trace.h"
#include "../datapath_rx.h"
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
#include "datapath_ppv4_session.h"
#endif
#if IS_ENABLED(CONFIG_PPA_API_SW_FASTPATH)
#include <net/ppa/ppa_api.h>
#endif

#define PRE_L2_LEN_IN_BYTES	16

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

static void rx_info(int dpid, int gpid, int vap, char *str)
{
	if (!(dp_dbg_flag & DP_DBG_FLAG_DUMP_RX))
		return;

	if (!printk_ratelimit())
		return;

	if (gpid != -1)
		DP_DEBUG(DP_DBG_FLAG_DUMP_RX, "gpid=%d ", gpid);
	DP_DEBUG(DP_DBG_FLAG_DUMP_RX, "dpid=%d vap=%d %s", dpid, vap, str);
}

static void rx_dbg_hdr(char *parser, int parser_len, struct pmac_rx_hdr *pmac,
		       char *prel2, int prel2_len)
{
	struct inst_info *dp_info = get_dp_prop_info(0);

	if (!(dp_dbg_flag & DP_DBG_FLAG_DUMP_RX))
		return;

	if (!printk_ratelimit())
		return;

	if ((dp_dbg_flag & DP_DBG_FLAG_DUMP_RX_PASER) && parser_len) {
		DP_DEBUG(DP_DBG_FLAG_DUMP_RX, "parser hdr size = %d\n",
			 parser_len);
		dump_parser_flag(parser);
	}

	if ((dp_dbg_flag & DP_DBG_FLAG_DUMP_RX_PMAC) && pmac)
		dp_info->dump_rx_pmac(pmac);

	if (dp_dbg_flag & DP_DBG_FLAG_DUMP_RX)
		dp_dump_raw_data(prel2, prel2_len, "Pre L2 Header");
}

static void rx_dbg(struct sk_buff *skb, struct dma_rx_desc_0 *desc0,
		   struct dma_rx_desc_1 *desc1, struct dma_rx_desc_2 *desc2,
		   struct dma_rx_desc_3 *desc3, bool org)
{
	struct inst_info *dp_info = get_dp_prop_info(0);
	int data_len, offset;

	data_len = skb->len > dp_print_len ? skb->len : dp_print_len;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	offset = desc3->field.byte_offset;
#else
	offset = desc2->field.byte_offset;
#endif

	if (!(dp_dbg_flag & DP_DBG_FLAG_DUMP_RX))
		return;

	if (!printk_ratelimit())
		return;

	DP_DEBUG(DP_DBG_FLAG_DUMP_RX,
		 "dp_rx:skb->data=0x%px Loc=%x offset=%d skb->len=%d\n",
		 skb->data, desc2->field.data_ptr, offset, skb->len);

	if (dp_dbg_flag & DP_DBG_FLAG_DUMP_RX_DESCRIPTOR)
		dp_info->dump_rx_dma_desc(desc0, desc1, desc2, desc3);

	if (dp_dbg_flag & DP_DBG_FLAG_DUMP_RX_DATA) {
		if (org)
			dp_dump_raw_data((char *)skb->data, data_len,
					 "Original Data");
		else
			dp_dump_raw_data((char *)skb->data, data_len,
					 "Updated Data");
	}
}

static int get_header_len(struct sk_buff *skb, int *hdr_len,
			  struct pmac_rx_hdr **pmac)
{
	char *parser = NULL, *prel2 = NULL;
	int parser_len = 0, prel2_len = 0;
	struct dma_rx_desc_1 *desc_1;

	desc_1 = (struct dma_rx_desc_1 *)&skb->DW1;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	parser_len = parser_enabled(desc_1->field.ep, desc_1);
	if (parser_len)
		parser = skb->data;

	*pmac = (struct pmac_rx_hdr *)(skb->data + parser_len);
	*hdr_len = parser_len;
#else
	if (desc_1->field.pmac)
		*pmac = (struct pmac_rx_hdr *)skb->data;

	prel2_len = desc_1->field.pmac ? sizeof(struct pmac_rx_hdr) : 0;
	/* PreL2 length includes PMAC length */
	prel2_len += ((desc_1->field.pre_l2 - desc_1->field.pmac) *
		      PRE_L2_LEN_IN_BYTES);

	if (prel2_len)
		prel2 = skb->data + sizeof(struct pmac_rx_hdr);
	*hdr_len = prel2_len;
#endif

	if (unlikely(dp_dbg_flag))
		rx_dbg_hdr(parser, parser_len, *pmac, prel2, prel2_len);

	return DP_SUCCESS;
}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
static int get_gpid(struct sk_buff *skb, int *gpid)
{
	int inst = 0, cpu_gpid_start, range;
	struct pmac_port_info *dp_port;
	struct dma_rx_desc_1 *desc_1;
	struct inst_info *dp_info;
#if IS_ENABLED(CONFIG_MXL_SKB_EXT)
	struct pp_desc *pp_desc;
#endif

	dp_info = get_dp_prop_info(inst);
	dp_port = get_dp_port_info(inst, CPU_PORT);
	desc_1 = (struct dma_rx_desc_1 *)&skb->DW1;

	/* If egflag is 1 gpid = desc port
	 * If egflag is 0 and desc.port is from 16 to 23, gpid = ud port
	 * If egflag is 0 and desc.port not from 16 to 23, gpid = desc port
	 */
	*gpid = desc_1->field.ep;
	if (!desc_1->field.redir) {
		range = dp_port->gpid_num - dp_info->cap.max_num_spl_conn;
		cpu_gpid_start = dp_port->gpid_base;
		if (*gpid >= cpu_gpid_start &&
		    *gpid < cpu_gpid_start + range) {
#if IS_ENABLED(CONFIG_MXL_SKB_EXT)
			pp_rx_pkt_hook(skb);
			pp_desc = pp_pkt_desc_get(skb);
			if (!pp_desc) {
				pr_err("DPM: pp_pkt_desc_get fail\n");
				return DP_FAILURE;
			}

			*gpid = pp_desc->ud.rx_port;
#else
			*gpid = pp_get_rx_port(skb->buf_base);
#endif
		}
	}

	return DP_SUCCESS;
}

static int create_dflt_egress_session(int inst, struct sk_buff *skb, int gpid,
				      int vap, u8 classid,
				      struct dp_subif_info *sif)
{
	struct dp_session sess = {0};
#if IS_ENABLED(CONFIG_MXL_SKB_EXT)
	struct pp_desc *pp_desc = pp_pkt_desc_get(skb);
#else
	char *buf_base = (char *)skb->buf_base;
#endif

	if (sif->dfl_sess[classid] < 0) {
		sess.inst = inst;
		sess.in_port = gpid;
		sess.eg_port = sif->gpid;
		sess.qid = sif->qid;
		sess.vap = vap;
		sess.class = classid;

#if IS_ENABLED(CONFIG_MXL_SKB_EXT)
		if (!pp_desc)
			return DP_FAILURE;
		sess.h1 = pp_desc->ud.hash_h1;
		sess.h2 = pp_desc->ud.hash_h2;
		sess.sig = pp_desc->ud.hash_sig;
#else
		sess.sig = pp_get_signature(buf_base);
		pp_get_hash(buf_base, &sess.h1, &sess.h2);
#endif

		DP_DEBUG(DP_DBG_FLAG_DUMP_RX,
			 "to create def_eg_session for GPID=%d h1=%x h2=%x sig=%x\n",
			 gpid, sess.h1, sess.h2, sess.sig);

		if (dp_add_default_egress_sess(&sess, 0)) {
			return DP_FAILURE;
		}
		sif->dfl_sess[classid] = sess.sess_id;
	} else {
		DP_DEBUG(DP_DBG_FLAG_DUMP_RX,
			 "Default egress session already created\n");
	}

	return DP_SUCCESS;
}
#endif

static int dp_lct_pkt_cnt(struct pmac_port_info *dp_port, bool unicast,
			  const struct sk_buff *skb, int offset)
{
	struct dp_lct_rx_cnt *stats;
	int len = skb->len + ETH_FCS_LEN - offset;

	spin_lock_bh(&dp_port->mib_cnt_lock);

	stats = dp_port->lct_rx_cnt;
	if (!stats) {
		spin_unlock_bh(&dp_port->mib_cnt_lock);
		return DP_SUCCESS;
	}
	/* update pointer to multicast/broadcast counter storage */
	if (!unicast)
		stats++;

	DP_DEBUG(DP_DBG_FLAG_DBG, "dp_lct_pkt_cnt: len=%d\n", len);

	if (len <= 64)
		stats->dp_64BytePkts++;
	else if ((len <= 127) && (len >= 65))
		stats->dp_127BytePkts++;
	else if ((len <= 255) && (len >= 128))
		stats->dp_255BytePkts++;
	else if ((len <= 511) && (len >= 256))
		stats->dp_511BytePkts++;
	else if ((len <= 1023) && (len >= 512))
		stats->dp_1023BytePkts++;
	else
		stats->dp_MaxBytePkts++;

	if (unicast) {
		stats->dp_UnicastPkts++;
	} else {
		if (is_broadcast_ether_addr(skb->data + offset))
			stats->dp_BroadcastPkts++;
		else
			stats->dp_MulticastPkts++;
	}

	stats->dp_GoodPkts++;
	stats->dp_GoodBytes += len;

	spin_unlock_bh(&dp_port->mib_cnt_lock);

	return DP_SUCCESS;
}

/* For multicast/broadcast pkt, clone and send skb to LCT dev
 * For unicast pkt, send to LCT dev if addr matched
 * Return: 0 for unicast pkt if addr matched and skb is consumed otherwise
 * return -1
 */
static int dp_handle_lct(struct pmac_port_info *dp_port,
			 struct sk_buff *skb, dp_rx_fn_t rx_fn)
{
	int vap, offset, res = -1;
	struct dp_subif_info *sif;
	struct sk_buff *lct_skb;
	struct dev_mib *mib;

	vap = dp_port->lct_idx;
	sif = get_dp_port_subif(dp_port, vap);
	mib = get_dp_port_subif_mib(sif);
	/* Change to LCT dev */
	skb->dev = sif->netif;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	offset = 0;
#else
	/* For PRX, skb->data contains pmac header */
	offset = sizeof(struct pmac_rx_hdr);
#endif

	/* "multicast" includes the broadcast packets */
	if (is_multicast_ether_addr(&skb->data[offset])) {
		DP_DEBUG(DP_DBG_FLAG_PAE, "LCT mcast or broadcast\n");
		if ((STATS_GET(sif->rx_flag) <= 0)) {
			DP_DEBUG(DP_DBG_FLAG_DUMP_RX,
				 "Pkt drop, LCT rx disabled");
		} else {
			lct_skb = skb_clone(skb, GFP_ATOMIC);
			if (!lct_skb) {
				pr_err("DPM: LCT mcast/bcast skb clone fail\n");
			} else {
				UP_STATS(mib->rx_fn_rxif_pkt);
				MIB_G_STATS_INC(rx_rxif_clone);
				dp_lct_pkt_cnt(dp_port, false, lct_skb, offset);
				DP_DEBUG(DP_DBG_FLAG_PAE, "pkt sent lct(%s)\n",
					 lct_skb->dev->name);
				rx_fn(lct_skb->dev, NULL, lct_skb,
				      lct_skb->len);
			}
		}
	} else if (!dp_memcmp(skb->data + offset,
			   skb->dev->dev_addr, ETH_ALEN)) {
		DP_DEBUG(DP_DBG_FLAG_PAE, "LCT unicast\n");
		DP_DEBUG(DP_DBG_FLAG_PAE, "pkt sent lct(%s)\n",
			 skb->dev->name);
		if ((STATS_GET(sif->rx_flag) <= 0)) {
			DP_DEBUG(DP_DBG_FLAG_DUMP_RX,
				 "Pkt drop, LCT rx disabled");
			UP_STATS(mib->rx_fn_dropped);
			MIB_G_STATS_INC(rx_drop);
			dev_kfree_skb_any(skb);
		} else {
			dp_lct_pkt_cnt(dp_port, true, skb, offset);
			UP_STATS(mib->rx_fn_rxif_pkt);
			MIB_G_STATS_INC(rx_rxif_pkts);
			rx_fn(skb->dev, NULL, skb, skb->len);
		}
		res = 0;
	}

	return res;
}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
static dp_subif_t dp_rx_subif[DP_MAX_CPU];
#endif
/* This function will always return DP_SUCCESS for now,
 * as all errors will be handle here.
 */
int _dp_rx(struct sk_buff *skb, u32 flags)
{
	int inst = 0, hdr_len, vap = 0, dpid = 0, subif, tx_flag = 0, gpid = -1;
	struct pmac_port_info *dp_port;
	struct dma_rx_desc_0 *desc_0;
	struct dma_rx_desc_1 *desc_1;
	struct dma_rx_desc_2 *desc_2;
	struct dma_rx_desc_3 *desc_3;
	struct inst_info *dp_info;
	struct dp_subif_info *sif = NULL;
	struct net_device *dev;
	struct dev_mib *mib;
	struct pmac_rx_hdr *pmac = NULL;
	int ret = 0;
	struct dp_rx_hook_data hd = {0};

	dp_port = get_dp_port_info(inst, CPU_PORT);

	get_header_len(skb, &hdr_len, &pmac);

	desc_0 = (struct dma_rx_desc_0 *)&skb->DW0;
	desc_1 = (struct dma_rx_desc_1 *)&skb->DW1;
	desc_2 = (struct dma_rx_desc_2 *)&skb->DW2;
	desc_3 = (struct dma_rx_desc_3 *)&skb->DW3;

	subif =  desc_0->field.dest_sub_if_id;
	dp_info = get_dp_prop_info(inst);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	if (get_gpid(skb, &gpid))
		goto RX_ERR;

	dpid = get_dpid_from_gpid(inst, gpid);
	tx_flag = desc_1->field.redir;
#else
	dpid = desc_1->field.ep;
	if (dpid == PMAC_CPU_ID) {
		/* Need to check for src pmac port */
		dpid = desc_1->field.ip;
		subif = desc_1->field.session_id;
	} else {
		tx_flag = 1;
	}

	/* If PMAC Header EXT=1, Ingress Port is in PMAC Hdr Byte 7 IGP_EGP
	 * Ingress Subif is also in PMAC Header, tested with EXTPoint=2
	 */
	if (pmac && pmac->ext) {
		tx_flag = 0;
		/* extraction point 1 & 2 go to ingress dev */
		if (PMAC_RECID_GET_EXT_PNT(pmac->record_id_lsb) < 2) {
			dpid = pmac->igp_egp;
			subif = (pmac->src_dst_subif_id_lsb |
				 (pmac->src_dst_subif_id_msb << 8));
		} else {
			dpid = desc_1->field.ep;
			subif =  desc_0->field.dest_sub_if_id;
		}
	}

	if (unlikely(!dpid)) {
		rx_dbg(skb, desc_0, desc_1, desc_2, desc_3, true);
		goto RX_ERR;
	}
#endif

	if (unlikely(dpid >= dp_info->cap.max_num_dp_ports)) {
		DP_DEBUG(DP_DBG_FLAG_DUMP_RX, "wrong port=%u\n", dpid);
		goto RX_ERR;
	}

	dp_port = get_dp_port_info(inst, dpid);
	if (dp_port->status == PORT_FREE) {
		rx_info(dpid, gpid, vap, "dropped as port is free\n");
		goto RX_DROP;
	}

	vap = GET_VAP(subif, dp_port->vap_offset, dp_port->vap_mask);
	sif = get_dp_port_subif(dp_port, vap);
	mib = get_dp_port_subif_mib(sif);
	hd.rx_cmn.inst = inst;
	hd.rx_cmn.portid = dpid;
	hd.rx_cmn.alloc_flags = dp_port->alloc_flags;
	hd.rx_cmn.subif = sif->subif;
	hd.rx_fn = sif->rx_fn;
	if (!hd.rx_fn) {
		rx_info(dpid, gpid, vap, "dropped as rx_fn NULL\n");
		DP_DEBUG(DP_DBG_FLAG_DUMP_RX,
			 "Desc: D0: %08x D1: %08x D2: %08x D3: %08x\n",
			 desc_0->all, desc_1->all,
			 desc_2->all, desc_3->all);
		goto RX_DROP;
	}

	if (unlikely(dp_dbg_flag)) {
		rx_info(dpid, gpid, vap, "\n");
		rx_dbg(skb, desc_0, desc_1, desc_2, desc_3, true);
	}

	if ((STATS_GET(sif->rx_flag) <= 0) && !(dp_port->lct_idx)) {
		rx_info(dpid, gpid, vap, "pkt drop, rx disabled\n");
		UP_STATS(mib->rx_fn_dropped);
		goto RX_DROP;
	}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	/* Handles special GPID */
	if (IS_SPECIAL_GPID(gpid)) {
		dp_subif_t *rx_subif;

		pr_err("Should not get special gpid %d\n", gpid);
		rx_subif = &dp_rx_subif[smp_processor_id()];
		if (create_dflt_egress_session(inst, skb, gpid, vap,
					       desc_1->field.classid, sif)) {
			goto RX_ERR;
		}

		/* Use special GPID for memory ports */
		desc_1->field.ep = gpid;
		desc_1->field.redir = 1;

		rx_subif->subif = sif->subif;
		rx_subif->port_id = dp_port->port_id;

		ret = dp_xmit(sif->netif, rx_subif, skb, skb->len,
			      DP_TX_BYPASS_FLOW | DP_TX_BYPASS_QOS);
		if (!ret) {
			UP_STATS(mib->tx_cbm_pkt);
			MIB_G_STATS_INC(tx_pkts);
		}
		return ret;
	}
#endif

	dev = sif->netif;

	if (!dev && !is_dsl(dp_port)) {
		UP_STATS(mib->rx_fn_dropped);
		goto RX_DROP;
	}
	/* The timestamp comes after the FCS on rx. The packets look like this:
	 * payload + FCS + TS. So the FCS needs to be removed after the TS.
	 */
	dp_rx_handle_timetag(inst, pmac, dp_port, skb);
	dp_rx_handle_fcs(dp_port, sif->subif, skb);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	/* Remove PMAC and PreL2 Flag before send to network stack */
	desc_1->all &= rx_desc_mask[1];
#endif

	/* Clear some fields according to DMA template */
	desc_3->all &= rx_desc_mask[3];

	skb->dev = dev;
	skb->priority = desc_1->field.classid;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
	/* If switch h/w acceleration is enabled,setting of this bit
	 * avoid forwarding duplicate packets from linux
	 */
	if (sif->swdev_en && sif->fid)
		skb->offload_fwd_mark = 1;
#endif

	/* Remove header from skb
	 * For PRX, only parser header will be removed by DP,
	 * pmac header need to be removed by upper layer
	 * For LGM, pmac + prel2 header will be removed by DP
	 */
	if (hdr_len) {
		skb_pull(skb, hdr_len);
#if IS_ENABLED(CONFIG_PPA_API_SW_FASTPATH)
		skb->mark |= FLG_PPA_PROCESSED;
#endif
	}

	if (unlikely(dp_dbg_flag))
		rx_dbg(skb, desc_0, desc_1, desc_2, desc_3, false);

	if (!tx_flag) {
		if (dp_port->lct_idx) {
			if (!dp_handle_lct(dp_port, skb, hd.rx_fn)) {
				trace_dp_rx(DP_SUCCESS, dpid, gpid, sif, vap,
							desc_0, desc_1, desc_2, desc_3, skb, pmac);
				return DP_SUCCESS;
			}
		}
		if (STATS_GET(sif->rx_flag) <= 0) {
			rx_info(dpid, gpid, vap, "pkt drop, rx disabled\n");
			UP_STATS(mib->rx_fn_dropped);
			goto RX_DROP;
		}
		ret = dp_rx_hook_call(dev, NULL, skb, &hd);
		if (likely(ret != DP_RX_HOOK_FN_CONTINUE)) {
			UP_STATS(mib->rx_fn_rxif_pkt);
			MIB_G_STATS_INC(rx_rxif_pkts);
		} else {
			rx_info(dpid, gpid, vap, "pkt drop, rx hook stray skb.\n");
			UP_STATS(mib->rx_fn_dropped);
			goto RX_DROP;
		}
	} else {
		if (STATS_GET(sif->rx_flag) <= 0) {
			rx_info(dpid, gpid, vap, "pkt drop, rx disabled\n");
			UP_STATS(mib->rx_fn_dropped);
			goto RX_DROP;
		}
		ret = dp_rx_hook_call(NULL, dev, skb, &hd);
		if (likely(ret != DP_RX_HOOK_FN_CONTINUE)) {
			UP_STATS(mib->rx_fn_txif_pkt);
			MIB_G_STATS_INC(rx_txif_pkts);
		} else {
			rx_info(dpid, gpid, vap, "pkt drop, rx hook stray skb.\n");
			UP_STATS(mib->rx_fn_dropped);
			goto RX_DROP;
		}
	}
	trace_dp_rx(DP_SUCCESS, dpid, gpid, sif, vap, desc_0, desc_1,
				desc_2, desc_3, skb, pmac);

	return DP_SUCCESS;

RX_ERR:
	UP_STATS(dp_port->rx_err_drop);

RX_DROP:
	MIB_G_STATS_INC(rx_drop);

	dev_kfree_skb_any(skb);
	trace_dp_rx(DP_FAILURE, dpid, gpid, sif, vap, desc_0, desc_1, desc_2,
				desc_3, skb, pmac);

	return DP_SUCCESS;
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP
