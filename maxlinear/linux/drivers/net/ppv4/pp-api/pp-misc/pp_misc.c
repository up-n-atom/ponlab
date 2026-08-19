/*
 * Copyright (C) 2020-2025 MaxLinear, Inc.
 * Copyright (C) 2018-2020 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2, as published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR APARTICULARPURPOSE.See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public
 * License along with this program; if not,see
 * <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: PP Misc Driver
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/sort.h>
#include <soc/mxl/mxl_skb_ext.h>
#include <linux/platform_data/lgm_epu.h>
#include <linux/pktprs.h>
#include <linux/spinlock.h>
#include <linux/pp_qos_api.h>
#include <linux/pp_api.h>
#include "pp_common.h"
#include "pp_dma.h"
#include "pp_misc.h"
#include "infra.h"
#include "uc.h"
#include "checker.h"
#include "pp_desc.h"
#include "pp_misc_internal.h"
#include "pp_session_mgr.h"
#include "pp_port_mgr.h"

/**
 * @define BILLION
 */
#define BILLION               (1000000000)

/**
 * @define LLD Max QL Score
 */
#define LLD_MAX_QLSCORE       (5 * BILLION)

/**
 * @define LLD default QL score in usec (from MULPI C.2.2.9.17.9)
 */
#define LLD_DEFAULT_QLSCORE         (4000)

/**
 * @define LLD_MAX_FRAME_SZ
 */
#define LLD_MAX_FRAME_SZ      2000

/**
 * @define microsecond to nanosecond
 */
#define US_2_NS(_us_)         (1000 * _us_)

/**
 * @define LLD_MAX_FLOOR (from MULPI Annex O)
 */
#define LLD_MAX_FLOOR         (65535000)

/**
 * @define LLD_SCALE_DOWN_FACTOR for QL calculation
 */
#define LLD_SCALE_DOWN_FACTOR     (1024)

struct pp_sf_entry {
	struct pp_qos_aqm_lld_sf_config sf_cfg;
	bool                            enabled;

	/*! LLD FW context (0 - (PP_MAX_ASF-1)) */
	u8                              fw_lld_ctx;
};

/**
 * @struct pp_misc_db
 * @brief Packet Processor misc driver database
 */
struct pp_misc_db {
	/*! ext_id to the PP ext for attaching ext on RX hook */
	u32                pp_ext_id;

	/*! misc driver stats */
	struct misc_stats  stats;

	/*! network functions enabled bitmap */
	ulong              nf_en;

	/*! network functions */
	struct pp_nf_info  nf[PP_NF_NUM];

	/*! max prints for brief command */
	u32 dbg_print_cnt;

	/*! Ingore clock freq' updates */
	u32 dbg_ignore_clock_freq_updates;

	/* lock */
	spinlock_t lock;

	/* whitelist GPID group id */
	u32 wl_grp_id;

	/* SF configuration */
	struct pp_sf_entry sf_entry[PP_QOS_MAX_SERVICE_FLOWS];

	/* Used LLD contexts */
	ulong              used_fw_lld_ctx[BITS_TO_LONGS(PP_MAX_ASF)];

	/* AQM engine type */
	u8	aqm_engine;
};

/**
 * @brief return misc_db from device's private data
 */
static inline struct pp_misc_db *get_misc_db(void)
{
	struct pp_dev_priv *dev_priv = pp_priv_get();

	if (unlikely(ptr_is_null(dev_priv)))
		return NULL;

	return (struct pp_misc_db *)dev_priv->misc_db;
}

static int pm_notifier(struct notifier_block *nb, unsigned long e, void *data);
static struct notifier_block nb = { .notifier_call = pm_notifier};

s32 misc_stats_get(struct misc_stats *stats)
{
	atomic_t *db_cnt, *cnt;
	struct pp_misc_db *db = get_misc_db();

	if (ptr_is_null(db) || ptr_is_null(stats))
		return -EINVAL;

	cnt = (atomic_t *)stats;
	for_each_struct_mem(&db->stats, db_cnt, cnt++)
		atomic_set(cnt, atomic_read(db_cnt));

	return 0;
}

s32 misc_stats_reset(void)
{
	atomic_t *cnt;
	struct pp_misc_db *db = get_misc_db();

	if (ptr_is_null(db))
		return -EINVAL;

	for_each_struct_mem(&db->stats, cnt)
		atomic_set(cnt, 0);

	return 0;
}

/**
 * @brief Get Packet Processor version
 * @param v pp_version type buffer to write version to
 * @param t version type
 * @return s32 0 on success, -EINVAL in case of null version
 *         buffer
 */
s32 pp_version_get(struct pp_version *v, enum pp_version_type t)
{
	if (unlikely(!pp_is_ready()))
		return -EPERM;
	if (unlikely(!v))
		return -EINVAL;

	memset(v, 0, sizeof(*v));
	switch (t) {
	case PP_VER_TYPE_DRV:
		v->major     = PP_VER_MAJOR;
		v->major_mid = PP_VER_MAJOR_MID;
		v->mid       = PP_VER_MID;
		v->minor_mid = PP_VER_MINOR_MID;
		v->minor     = PP_VER_MINOR;
		break;
	case PP_VER_TYPE_FW:
		/* TODO: get PP FW version */
		break;
	case PP_VER_TYPE_HW:
		v->major = infra_version_get();
		break;
	default:
		pr_err("Invalid PP version type %u\n", t);
		return -EINVAL;
	}

	return 0;
}
EXPORT_SYMBOL(pp_version_get);

s32 pp_version_show(char *buf, size_t sz, size_t *n, struct pp_version *drv,
		    struct pp_version *fw, struct pp_version *hw)
{
	if (unlikely(ptr_is_null(buf) || ptr_is_null(n) || ptr_is_null(drv) ||
		     ptr_is_null(fw) || ptr_is_null(hw)))
		return -EINVAL;

	pr_buf(buf, sz, *n, "  %-25s: \t%u.%u.%u.%u.%u\n", "PP driver version",
	       drv->major, drv->major_mid, drv->mid, drv->minor_mid,
	       drv->minor);

	pr_buf_cat(buf, sz, *n, "  %-25s: \t%u.%u.%u.%u.%u\n", "PP FW version",
		   fw->major, fw->major_mid, fw->mid, fw->minor_mid, fw->minor);

	pr_buf_cat(buf, sz, *n, "  %-25s: \t%u.%u.%u.%u.%u\n", "PP HW version",
		   hw->major, hw->major_mid, hw->mid, hw->minor_mid, hw->minor);

	return 0;
}

s32 pp_nf_set(enum pp_nf_type type, struct pp_nf_info *nf, void *data)
{
	struct pp_qos_queue_info q_info;
	struct pp_qos_dev *qdev;
	u16 uc_qos_port, uc_q, cycl2_q;
	s32 dflt_excp, ret = 0;
	struct pp_misc_db *db = get_misc_db();

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	if (unlikely(ptr_is_null(nf) || ptr_is_null(db))) {
		ret = -EINVAL;
		goto out;
	}

	if (unlikely(!PP_IS_NF_VALID(type))) {
		pr_err("invalid nf type %d\n", type);
		ret = -EINVAL;
		goto out;
	}

	if (unlikely(type != PP_NF_REASSEMBLY &&
		     !__pp_is_port_valid(nf->pid))) {
		pr_err("invalid nf port %hu\n", nf->pid);
		ret = -EINVAL;
		goto out;
	}

	/* get the nf physical queue */
	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(ptr_is_null(qdev)))
		return -EINVAL;

	ret = pp_qos_queue_info_get(qdev, nf->q, &q_info);
	if (unlikely(ret)) {
		pr_err("Failed getting queue %u info\n", nf->q);
		goto out;
	}

	uc_qos_port = q_info.port_id;
	uc_q = q_info.physical_id;

	ret = pp_qos_queue_info_get(qdev, nf->cycl2_q, &q_info);
	if (unlikely(ret)) {
		pr_err("Failed getting queue %u info\n", nf->cycl2_q);
		goto out;
	}

	cycl2_q = q_info.physical_id;
	dflt_excp = pmgr_dflt_hif_id_get();
	if (dflt_excp < 0) {
		pr_err("Default host interface isn't configured\n");
		return -EPERM;
	}

	ret = uc_nf_set(type, nf->pid, nf->subif, uc_qos_port,
			cycl2_q, uc_q, dflt_excp, data);
	if (unlikely(ret)) {
		pr_err("Failed Setting NF %d in uc\n", type);
		goto out;
	}

	/* update checker */
	ret = chk_nf_set(uc_q, type);
	if (unlikely(ret)) {
		pr_err("Failed Setting queue %hu for nf %d\n",
		       uc_q, type);
		goto out;
	}

	/* store nf info */
	set_bit(type, &db->nf_en);
	memcpy(&db->nf[type], nf, sizeof(*nf));

	/* Notify session manager of the new NF added */
	ret = smgr_nf_set(type, nf, uc_q, cycl2_q);
out:
	return ret;
}
EXPORT_SYMBOL(pp_nf_set);

s32 pp_nf_get(enum pp_nf_type type, struct pp_nf_info *nf)
{
	struct pp_misc_db *db = get_misc_db();

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	if (unlikely(ptr_is_null(nf) || ptr_is_null(db)))
		return -EINVAL;

	if (unlikely(!PP_IS_NF_VALID(type))) {
		pr_err("invalid nf type %d\n", type);
		return -EINVAL;
	}

	memcpy(nf, &db->nf[type], sizeof(*nf));

	return 0;
}
EXPORT_SYMBOL(pp_nf_get);

bool pp_misc_is_nf_en(enum pp_nf_type nf)
{
	struct pp_misc_db *db = get_misc_db();

	if (ptr_is_null(db))
		return -EINVAL;

	return test_bit(nf, &db->nf_en);
}

struct pp_desc *pp_pkt_desc_get(struct sk_buff *skb)
{
	struct pp_misc_db *db = get_misc_db();

	if (unlikely(!pp_is_ready()))
		return NULL;
	if (unlikely(ptr_is_null(db) || ptr_is_null(skb)))
		return NULL;

	return ((struct pp_desc *)mxl_skb_ext_find(skb, db->pp_ext_id));
}
EXPORT_SYMBOL(pp_pkt_desc_get);

s32 pp_hal_stats_get(struct pp_hal_stats *stats)
{
	struct pp_global_stats gl_stats;
	s32 ret;

	if (ptr_is_null(stats))
		return -EINVAL;

	memset(stats, 0, sizeof(*stats));
	ret = port_dist_stats_get(&gl_stats.port_dist_stats);
	if (ret) {
		pr_err("port_dist_stats_get failed\n");
		return ret;
	}

	ret = rpb_stats_get(&gl_stats.rpb_stats);
	if (ret) {
		pr_err("rpb_stats_get failed\n");
		return ret;
	}

	ret = rx_dma_stats_get(&gl_stats.rx_dma_stats);
	if (ret) {
		pr_err("rx_dma_stats_get failed\n");
		return ret;
	}

	stats->rx_inline_packets   = gl_stats.port_dist_stats.packets;
	stats->tx_inline_packets   = gl_stats.rx_dma_stats.tx_pkts;
	stats->drop_inline_packets = gl_stats.rx_dma_stats.drop_pkts +
				     gl_stats.rpb_stats.total_packets_dropped;

	return 0;
}
EXPORT_SYMBOL(pp_hal_stats_get);

void pp_rx_pkt_hook(struct sk_buff *skb)
{
	struct pp_desc *desc = NULL;
	struct pp_misc_db *db = get_misc_db();

	if (unlikely(!db))
		return;
	if (unlikely(!skb)) {
		atomic_inc(&db->stats.rx_hook_null_skb_cnt);
		return;
	}
	atomic_inc(&db->stats.rx_hook_skb_cnt);

	/* Allocate pp_desc ext on the SKB */
	desc = mxl_skb_ext_add(skb, db->pp_ext_id);
	if (unlikely(!desc)) {
		pr_debug("Failed to add PP descriptor ext to skb, sz = %u\n",
		       (u32)sizeof(*desc));
		return;
	}

#if IS_ENABLED(CONFIG_MXL_CBM_SKB) || IS_ENABLED(CONFIG_DIRECTCONNECT_DP_API)
	if (pp_desc_decode(desc, (struct pp_hw_desc *)skb->buf_base)) {
		pr_err("Failed to decode PP descriptor, skb 0x%p\n", skb);
		mxl_skb_ext_del(skb, db->pp_ext_id);
		return;
	}
#endif

	/* Count last slow path packets received for debug */
	if (desc->lsp_pkt)
		atomic_inc(&db->stats.rx_hook_lspp_pkt_cnt);
}
EXPORT_SYMBOL(pp_rx_pkt_hook);

void pp_tx_pkt_hook(struct sk_buff *skb, u16 pid)
{
	struct pp_desc *desc;
	struct pp_misc_db *db = get_misc_db();

	if (ptr_is_null(db))
		return;

	if (unlikely(!skb)) {
		atomic_inc(&db->stats.tx_hook_null_skb_cnt);
		return;
	}

	atomic_inc(&db->stats.tx_hook_skb_cnt);
	desc = pp_pkt_desc_get(skb);
	if (!desc)
		return;

	/* if this is the last slow path packet,
	 * signal the sync queue mechanism
	 */
	if (desc->lsp_pkt)
		smgr_sq_lspp_rcv(desc->ud.hash_sig);
}
EXPORT_SYMBOL(pp_tx_pkt_hook);

s32 pp_misc_sf_set(u8 sf_id, struct pp_qos_aqm_lld_sf_config *sf_cfg)
{
	struct lld_ctx_cfg uc_lld_cfg = { 0 };
	struct aqm_ctx_cfg uc_aqm_cfg = { 0 };
	struct pp_qos_dev  *qdev;
	struct pp_misc_db *db = get_misc_db();
	s32 ret = 0;
	u8  lld_ctx = PP_MAX_ASF;
	u8  hist_idx;
	u32 floor;
	u32 critical_ql_score_ns;

	if (sf_id >= PP_QOS_MAX_SERVICE_FLOWS) {
		pr_err("sf_id %u is invalid\n", sf_id);
		return -EINVAL;
	}

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (ptr_is_null(qdev))
		return -ENODEV;

	ret = pp_qos_aqm_lld_sf_set(qdev, sf_id, sf_cfg);
	if (unlikely(ret)) {
		pr_err("pp_qos_aqm_lld_sf_set failed\n");
		return ret;
	}

	/* In case of aqm_sw, uc also needs aqm parameters */
	if (db->aqm_engine == PP_AQM_SW) {
		uc_aqm_cfg.aqm_cfg.valid = 1;
		uc_aqm_cfg.aqm_cfg.sf_id = sf_id;
		uc_aqm_cfg.aqm_cfg.peak_rate = sf_cfg->cfg.aqm_cfg.peak_rate;
		uc_aqm_cfg.aqm_cfg.msr = sf_cfg->cfg.aqm_cfg.msr;
		uc_aqm_cfg.aqm_cfg.latency_target_ms =
			sf_cfg->cfg.aqm_cfg.latency_target_ms;
		uc_aqm_cfg.aqm_cfg.buffer_size = sf_cfg->buffer_size;

		uc_aqm_cfg.hist_cfg.num_hist_bins = sf_cfg->num_hist_bins;
		for (hist_idx = 0; hist_idx < sf_cfg->num_hist_bins;
			 hist_idx++) {
			uc_aqm_cfg.hist_cfg.bin_edges[hist_idx] =
				sf_cfg->bin_edges[hist_idx];
		}

		ret = uc_aqm_ctx_set(&uc_aqm_cfg);
		if (unlikely(ret)) {
			pr_err("uc_aqm_ctx_set failed\n");
			pp_qos_aqm_lld_sf_remove(qdev, sf_id,
						 &db->sf_entry[sf_id].sf_cfg);
			return ret;
		}
	}

	if (!sf_cfg->llsf)
		goto done;

	/* LL SF */
	if (db->sf_entry[sf_id].enabled &&
	    db->sf_entry[sf_id].fw_lld_ctx != PP_MAX_ASF) {
	    /* SF exists, use the same context and update the rest */
	    lld_ctx = db->sf_entry[sf_id].fw_lld_ctx;
	    pr_debug("LLD SF %u exists, reusing context %u\n", sf_id, lld_ctx);
	} else {
	    if (db->sf_entry[sf_id].enabled &&
			db->sf_entry[sf_id].fw_lld_ctx == PP_MAX_ASF) {
			pr_err("LLD SF %u exist but fw context is not set, "
				   "assign new context\n", sf_id);
	    }

		lld_ctx = find_first_zero_bit(db->used_fw_lld_ctx, PP_MAX_ASF);
		if (lld_ctx >= PP_MAX_ASF) {
			pr_err("No space for new LLD context\n");
			/* no free entries available */
			pp_qos_aqm_lld_sf_remove(qdev, sf_id,
						&db->sf_entry[sf_id].sf_cfg);
			return -ENOSPC;
		}
	}

	uc_lld_cfg.lld_cfg.valid = 1;
	uc_lld_cfg.lld_cfg.lld_ctx_id = lld_ctx;
	uc_lld_cfg.lld_cfg.sf_id = sf_id;
	uc_lld_cfg.lld_cfg.buffer_size = sf_cfg->buffer_size;
	uc_lld_cfg.lld_cfg.coupled_sf = sf_cfg->coupled_sf;
	uc_lld_cfg.lld_cfg.iaqm_en = sf_cfg->cfg.lld_cfg.iaqm_en;
	uc_lld_cfg.lld_cfg.qp_en = sf_cfg->cfg.lld_cfg.qp_en;

	/* According to Annex O */
	if (sf_cfg->amsr == 0 && sf_cfg->msr_l == 0)
		uc_lld_cfg.lld_cfg.max_rate = 0;
	else if (sf_cfg->amsr == 0 && sf_cfg->msr_l != 0)
		uc_lld_cfg.lld_cfg.max_rate = sf_cfg->msr_l;
	else if (sf_cfg->amsr != 0 && sf_cfg->msr_l == 0)
		uc_lld_cfg.lld_cfg.max_rate = sf_cfg->amsr;
	else if (sf_cfg->amsr != 0 && sf_cfg->msr_l != 0)
		uc_lld_cfg.lld_cfg.max_rate = min(sf_cfg->amsr, sf_cfg->msr_l);

	if (uc_lld_cfg.lld_cfg.max_rate) {
		floor = (u32)((u64)(2 * 8 * LLD_MAX_FRAME_SZ * (u64)BILLION) /
			uc_lld_cfg.lld_cfg.max_rate);
		floor = (LLD_MAX_FLOOR < floor) ? LLD_MAX_FLOOR : floor;
	} else {
		floor = 0;
	}
	uc_lld_cfg.lld_cfg.maxth_ns = US_2_NS(sf_cfg->cfg.lld_cfg.maxth_us);
	uc_lld_cfg.lld_cfg.lg_aging = sf_cfg->cfg.lld_cfg.lg_aging;
	uc_lld_cfg.lld_cfg.range_ns = 1 << sf_cfg->cfg.lld_cfg.lg_range;
	uc_lld_cfg.lld_cfg.minth_ns = (uc_lld_cfg.lld_cfg.maxth_ns - uc_lld_cfg.lld_cfg.range_ns) >
		floor ? (uc_lld_cfg.lld_cfg.maxth_ns - uc_lld_cfg.lld_cfg.range_ns) : floor;

	uc_lld_cfg.lld_cfg.maxth_ns = uc_lld_cfg.lld_cfg.minth_ns + uc_lld_cfg.lld_cfg.range_ns;
	uc_lld_cfg.lld_cfg.maxth_ns = min(uc_lld_cfg.lld_cfg.maxth_ns, LLD_MAX_FLOOR);

	/* critical_ql taken from configuration if exist, otherwise according to
	 * maxth
	 */
	uc_lld_cfg.lld_cfg.critical_ql_ns = sf_cfg->cfg.lld_cfg.critical_ql_us
		? US_2_NS(sf_cfg->cfg.lld_cfg.critical_ql_us)
		: uc_lld_cfg.lld_cfg.maxth_ns / LLD_SCALE_DOWN_FACTOR;

	/* critical_ql_score taken from configuration if exist, otherwise set to
	 * default
	 */
	critical_ql_score_ns = sf_cfg->cfg.lld_cfg.critical_ql_score_us
		? US_2_NS(sf_cfg->cfg.lld_cfg.critical_ql_score_us)
		: US_2_NS(LLD_DEFAULT_QLSCORE) / LLD_SCALE_DOWN_FACTOR;

	uc_lld_cfg.lld_cfg.critical_qL_product = critical_ql_score_ns *
					  uc_lld_cfg.lld_cfg.critical_ql_ns;

	uc_lld_cfg.lld_cfg.vq_interval = sf_cfg->cfg.lld_cfg.vq_interval;
	uc_lld_cfg.lld_cfg.vq_ewma_alpha = sf_cfg->cfg.lld_cfg.vq_ewma_alpha;

	uc_lld_cfg.hist_cfg.num_hist_bins = sf_cfg->num_hist_bins;

	for (hist_idx = 0; hist_idx < sf_cfg->num_hist_bins; hist_idx++)
		uc_lld_cfg.hist_cfg.bin_edges[hist_idx] = sf_cfg->bin_edges[hist_idx];

	ret = uc_lld_ctx_set(&uc_lld_cfg);
	if (unlikely(ret)) {
		pr_err("uc_lld_ctx_set failed\n");
		pp_qos_aqm_lld_sf_remove(qdev, sf_id,
					 &db->sf_entry[sf_id].sf_cfg);
		return ret;
	}
	set_bit(lld_ctx, db->used_fw_lld_ctx);

done:
	/* Store SF config in db */
	memcpy(&db->sf_entry[sf_id].sf_cfg, sf_cfg, sizeof(*sf_cfg));
	db->sf_entry[sf_id].enabled = true;
	db->sf_entry[sf_id].fw_lld_ctx = lld_ctx;

	return 0;
}
EXPORT_SYMBOL(pp_misc_sf_set);

s32 pp_lld_allowed_aq_set(u8 sf_id, u32 allowed_aq)
{
	struct pp_misc_db *db = get_misc_db();
	struct lld_allowed_aq_set_cmd cmd;

	if (sf_id >= PP_QOS_MAX_SERVICE_FLOWS ||
	    !db->sf_entry[sf_id].enabled ||
	    !test_bit(db->sf_entry[sf_id].fw_lld_ctx, db->used_fw_lld_ctx)) {
		pr_err("sf_id %u is invalid or already configured\n", sf_id);
		return -EINVAL;
	}

	if (!db->sf_entry[sf_id].sf_cfg.llsf) {
		pr_err("Trying to set allowedAQ for non llsf %u\n", sf_id);
		return -EINVAL;
	}

	cmd.ctx = db->sf_entry[sf_id].fw_lld_ctx;
	cmd.allowed_aq = allowed_aq;

	return uc_lld_allowed_aq_set(&cmd);
}
EXPORT_SYMBOL(pp_lld_allowed_aq_set);

s32 pp_misc_check_queue_lld_sf(u16 dst_q, bool *lld_sf)
{
	struct pp_qos_queue_info q_info;
	struct pp_qos_dev *qdev;
	u8 lld_ctx = PP_MAX_ASF;
	u16 coupled_queue = PP_QOS_INVALID_ID;
	s32 ret;

	if (unlikely(ptr_is_null(lld_sf)))
		return -EINVAL;

	*lld_sf = false;

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	/* get the nf physical queue */
	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(ptr_is_null(qdev)))
		return -EINVAL;

	ret = pp_qos_queue_info_get(qdev, dst_q, &q_info);
	if (unlikely(ret)) {
		pr_err("Failed getting queue %u info\n", dst_q);
		return ret;
	}
	ret = pp_misc_get_lld_info_by_q((u16)q_info.physical_id, &lld_ctx, &coupled_queue);
	if (unlikely(ret)) {
		pr_err("Failed to get lld queue info\n");
		return ret;
	}

	if (lld_ctx != PP_MAX_ASF && coupled_queue != PP_QOS_INVALID_ID)
		*lld_sf = true;

	return 0;
}
EXPORT_SYMBOL(pp_misc_check_queue_lld_sf);

s32 pp_misc_sf_remove(u8 sf_id)
{
	struct pp_misc_db *db = get_misc_db();
	struct lld_ctx_cfg uc_lld_cfg = { 0 };
	struct aqm_ctx_cfg uc_aqm_cfg = { 0 };
	struct pp_qos_dev  *qdev;
	s32 ret;

	if (sf_id >= PP_QOS_MAX_SERVICE_FLOWS ||
	    !db->sf_entry[sf_id].enabled) {
		pr_err("sf_id %u is invalid or not configured\n", sf_id);
		return -EINVAL;
	}

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (ptr_is_null(qdev))
		return -ENODEV;

	ret = pp_qos_aqm_lld_sf_remove(qdev, sf_id,
		&db->sf_entry[sf_id].sf_cfg);
	if (unlikely(ret)) {
		pr_err("pp_qos_aqm_lld_sf_remove failed\n");
		return ret;
	}

	/* In case of aqm_sw, reset aqm parameters in uc */
	if (db->aqm_engine == PP_AQM_SW) {
		memset(&uc_aqm_cfg, 0, sizeof(uc_aqm_cfg));
		ret = uc_aqm_ctx_set(&uc_aqm_cfg);
		if (unlikely(ret)) {
			pr_err("uc_aqm_ctx_set failed\n");
			return ret;
		}
	}

	if (db->sf_entry[sf_id].sf_cfg.llsf) {
		if (!test_bit(db->sf_entry[sf_id].fw_lld_ctx,
		    db->used_fw_lld_ctx)) {
			pr_err("bit %u is not set\n",
				db->sf_entry[sf_id].fw_lld_ctx);
			return -EINVAL;
		}

		uc_lld_cfg.lld_cfg.valid = 0;
		uc_lld_cfg.lld_cfg.lld_ctx_id = db->sf_entry[sf_id].fw_lld_ctx;
		ret = uc_lld_ctx_set(&uc_lld_cfg);
		if (unlikely(ret)) {
			pr_err("uc_lld_ctx_set failed\n");
			return ret;
		}
		clear_bit(uc_lld_cfg.lld_cfg.lld_ctx_id, db->used_fw_lld_ctx);
	}

	/* reset SF config in db */
	memset(&db->sf_entry[sf_id].sf_cfg, 0,
		sizeof(struct pp_qos_aqm_lld_sf_config));
	db->sf_entry[sf_id].enabled = false;
	db->sf_entry[sf_id].fw_lld_ctx = PP_MAX_ASF;

	return 0;
}
EXPORT_SYMBOL(pp_misc_sf_remove);

s32 pp_misc_sf_conf_get(u8 sf_id, struct pp_qos_aqm_lld_sf_config *sf_cfg)
{
	struct pp_misc_db *db = get_misc_db();

	if (sf_id >= PP_QOS_MAX_SERVICE_FLOWS) {
		pr_err("sf_id %u is invalid\n", sf_id);
		return -EINVAL;
	}

	memcpy(sf_cfg, &db->sf_entry[sf_id].sf_cfg, sizeof(*sf_cfg));

	return 0;
}
EXPORT_SYMBOL(pp_misc_sf_conf_get);

s32 pp_misc_sf_hist_get(u8 sf_id, struct pp_sf_hist_stat *hist, bool reset)
{
	struct pp_misc_db *db = get_misc_db();
	struct pp_qos_dev  *qdev;
	s32 ret;

	if (unlikely(ptr_is_null(hist)))
		return -EINVAL;

	if (sf_id >= PP_QOS_MAX_SERVICE_FLOWS) {
		pr_err("sf_id %u is invalid\n", sf_id);
		return -EINVAL;
	}

	if (!db->sf_entry[sf_id].enabled) {
		pr_err("SF is disabled. sf_id %u\n", sf_id);
		return -EINVAL;
	}

	if (db->sf_entry[sf_id].sf_cfg.num_hist_bins == 0) {
		/* histogram not enabled for SF */
		return -ENOENT;
	}

	if (db->sf_entry[sf_id].sf_cfg.llsf) {
		ret = uc_sf_hist_get(sf_id, hist, reset);
		if (unlikely(ret))
			return ret;
	} else { /* In AQM SW mode, classic histogram is taken from UC */
		if (db->aqm_engine == PP_AQM_SW) {
			ret = uc_sf_hist_get(sf_id, hist, reset);
			if (unlikely(ret))
				return ret;
		} else {  /* In AQM HW histogram is from qos fw */
			qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
			if (ptr_is_null(qdev))
				return -ENODEV;

			ret = pp_qos_sf_hist_get(qdev, sf_id, hist, reset);
			if (unlikely(ret))
				return ret;
		}
	}
	return 0;
}
EXPORT_SYMBOL(pp_misc_sf_hist_get);

s32 pp_misc_sf_lld_counters_get(u8 sf_id, struct pp_lld_stats *stats)
{
	struct pp_misc_db *db = get_misc_db();
	struct lld_sf_stats sf_lld_stats;
	s32 ret;

	if (sf_id >= PP_QOS_MAX_SERVICE_FLOWS) {
		pr_err("sf_id %u is invalid\n", sf_id);
		return -EINVAL;
	}

	if (!db->sf_entry[sf_id].enabled ||
	    !db->sf_entry[sf_id].sf_cfg.llsf) {
		pr_err("sf_id %u is disabled or not lld type\n", sf_id);
		return -EINVAL;
	}

	if (unlikely(ptr_is_null(stats)))
		return -EINVAL;

	memset(&sf_lld_stats, 0, sizeof(struct lld_sf_stats));

	ret = uc_lld_per_sf_stats_get(db->sf_entry[sf_id].fw_lld_ctx, &sf_lld_stats);
	if (unlikely(ret))
		pr_err("uc_lld_per_sf_stats_get failed\n");

	memcpy(stats, &sf_lld_stats, sizeof(struct lld_sf_stats));

	return 0;
}
EXPORT_SYMBOL(pp_misc_sf_lld_counters_get);

s32 pp_misc_aqm_sw_sf_counters_get(u8 sf_id, struct PP_AQM_SW_stats *stats)
{
	struct pp_misc_db *db = get_misc_db();
	struct aqm_sw_sf_stats uc_stats;
	s32 ret;

	if (!db || !stats)
		return -EINVAL;

	if (sf_id >= PP_QOS_MAX_SERVICE_FLOWS) {
		pr_err("sf_id %u is invalid\n", sf_id);
		return -EINVAL;
	}

	if (!db->sf_entry[sf_id].enabled) {
		pr_err("sf_id %u is disabled\n", sf_id);
		return -EINVAL;
	}

	memset(&uc_stats, 0, sizeof(struct aqm_sw_sf_stats));

	ret = uc_aqm_sw_per_sf_stats_get(sf_id, &uc_stats);
	if (unlikely(ret))
		pr_err("pp_misc_aqm_sw_sf_counters_get failed\n");

	memcpy(stats, &uc_stats, sizeof(struct aqm_sw_sf_stats));

	return 0;
}
EXPORT_SYMBOL(pp_misc_aqm_sw_sf_counters_get);

s32 pp_misc_fw_lld_ctx_get(u8 sf_id, u8* ctx)
{
	struct pp_misc_db *db = get_misc_db();

	if (unlikely(ptr_is_null(ctx)))
		return -EINVAL;

	if (sf_id >= PP_QOS_MAX_SERVICE_FLOWS) {
		pr_err("sf_id %u is invalid\n", sf_id);
		return -EINVAL;
	}

	*ctx = db->sf_entry[sf_id].fw_lld_ctx;

	return 0;
}
EXPORT_SYMBOL(pp_misc_fw_lld_ctx_get);

s32 pp_misc_get_sf_indx_by_q(u16 queue, u16 *sf_indx)
{
	struct pp_misc_db *db = get_misc_db();
	struct pp_qos_dev *qdev;
	struct pp_qos_aqm_lld_sf_config *cfg;
	u16 sf_ind;
	u16 q_ind;

	if (!sf_indx || !db)
		return -EINVAL;

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (ptr_is_null(qdev))
		return -ENODEV;

	/* Find the SF which contains the queue */
	for (sf_ind = 0; sf_ind < PP_QOS_MAX_SERVICE_FLOWS; sf_ind++) {
		if (!db->sf_entry[sf_ind].enabled)
			continue;

		cfg = &db->sf_entry[sf_ind].sf_cfg;
		for (q_ind = 0; q_ind < cfg->num_queues; q_ind++) {
			if (cfg->queue[q_ind].id == queue)
				goto found;
		}
	}

	*sf_indx = PP_QOS_MAX_SERVICE_FLOWS;
	return 0;

found:
	*sf_indx = sf_ind;
	return 0;
}
EXPORT_SYMBOL(pp_misc_get_sf_indx_by_q);

s32 pp_misc_get_lld_info_by_q(u16 queue, u8 *lld_ctx, u16 *coupled_queue)
{
	struct pp_misc_db *db = get_misc_db();
	s32 rc = 0;
	u16 sf_ind;
	u16 q_ind;
	struct pp_qos_aqm_lld_sf_config *cfg;
	u16 id;
	struct pp_qos_dev *qdev;

	if (unlikely(ptr_is_null(lld_ctx)) ||
	    unlikely(ptr_is_null(coupled_queue))) {
		rc = -EINVAL;
		goto out;
	}

	/* if no LLD configured, no need to search */
	if (bitmap_empty(db->used_fw_lld_ctx, PP_MAX_ASF))
		goto not_found;

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (ptr_is_null(qdev))
		return -ENODEV;

	id = pp_qos_queue_id_get(qdev, queue);
	/* Find the SF which contains the queue */
	for (sf_ind = 0; sf_ind < PP_QOS_MAX_SERVICE_FLOWS; sf_ind++) {
		if (!db->sf_entry[sf_ind].enabled || !db->sf_entry[sf_ind].sf_cfg.llsf)
			continue;

		cfg = &db->sf_entry[sf_ind].sf_cfg;
		for (q_ind = 0; q_ind < cfg->num_queues; q_ind++) {
			if (cfg->queue[q_ind].id == id)
				goto found;
		}
	}

not_found:
	*lld_ctx = PP_MAX_ASF;
	*coupled_queue = PP_QOS_INVALID_ID;
	goto out;

found:
	*lld_ctx = db->sf_entry[sf_ind].fw_lld_ctx;
	if (cfg->coupled_sf < PP_QOS_MAX_SERVICE_FLOWS &&
	    db->sf_entry[cfg->coupled_sf].enabled)
		*coupled_queue =
			db->sf_entry[cfg->coupled_sf].sf_cfg.queue[q_ind].id;
out:
	return rc;
}
EXPORT_SYMBOL(pp_misc_get_lld_info_by_q);

s32 pp_misc_pci_ready_set(bool state, u32 bar_addr)
{
	struct pp_qos_dev *qdev;
	struct pp_qos_pci_addr pci_addr = {0};
	s32 ret = 0;

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (ptr_is_null(qdev))
		return -ENODEV;

	/* Actual PCI address is set only in case function
	   is called with state true - when pci device is ready
	   otherwise passed address to fw is zero */
	if (state)
		/* msrtoken in argus is taken from rate limit token bucket reg
		DMAC_US_INGR_SFM_RL1_BUCKET_LEVEL_REG - address 0x2B0_8274 */
		pci_addr.msrtoken_addr = 0x02B08274 + bar_addr;

	ret = pp_qos_pci_addr_set(qdev, &pci_addr);
	if (unlikely(ret)) {
		pr_err("pp_qos_pci_addr_set failed\n");
		return ret;
	}

	return ret;
}
EXPORT_SYMBOL(pp_misc_pci_ready_set);

s32 pp_misc_set_aqm_engine(u8 aqm_engine)
{
	struct pp_qos_dev *qdev;
	struct pp_misc_db *db = get_misc_db();
	s32 ret = 0;

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (ptr_is_null(qdev))
		return -ENODEV;

	ret = pp_qos_set_aqm_engine(qdev, aqm_engine);
	if (unlikely(ret)) {
		pr_err("pp_qos_set_aqm_engine failed\n");
		return ret;
	}

	qdev->init_params.aqm_engine = aqm_engine;
	db->aqm_engine = aqm_engine;

	return ret;
}
EXPORT_SYMBOL(pp_misc_set_aqm_engine);

s32 pp_misc_get_aqm_engine(u8 *aqm_engine)
{
	struct pp_misc_db *db = get_misc_db();
	s32 ret = 0;

	if (unlikely(ptr_is_null(aqm_engine)))
		return -EINVAL;

	*aqm_engine = db->aqm_engine;

	return ret;
}
EXPORT_SYMBOL(pp_misc_get_aqm_engine);

s32 pp_resource_stats_show(char *buf, size_t sz, size_t *n)
{
	struct pp_bmgr_policy_stats policy_stats;
	struct pp_bmgr_pool_params pool_cfg;
	struct pp_bmgr_pool_stats pool_stats;
	struct rpb_hw_stats rpb_hw_stats;
	struct pp_bmgr_init_param bmgr_cfg;
	u32 i, rpb_mem_sz, tmp;
	s32 ret;
	char pools[64];

	if (unlikely(ptr_is_null(buf) || ptr_is_null(n)))
		return -EINVAL;

	ret = rpb_hw_stats_get(&rpb_hw_stats);
	if (unlikely(ret)) {
		pr_err("rpb_hw_stats_get failed\n");
		return ret;
	}

	ret = rpb_memory_sz_get(&rpb_mem_sz);
	if (unlikely(ret)) {
		pr_err("rpb_memory_sz_get failed\n");
		return ret;
	}

	ret = pp_bmgr_config_get(&bmgr_cfg);
	if (unlikely(ret)) {
		pr_err("pp_bmgr_config_get failed\n");
		return ret;
	}

	pr_buf(buf, sz, *n, "\n");
	pr_buf_cat(buf, sz, *n,
		   "+----------------------------------------------------------------------------------------+\n");
	pr_buf_cat(buf, sz, *n, "| %-86s |\n",
		   "          Packet Processor Resources");
	pr_buf_cat(buf, sz, *n,
		   "+--------|-------------|-------------|---------------|---------------|-------------------+\n");

	/* Policies stats */
	pr_buf_cat(buf, sz, *n,
		   "| %-6s | %-11s | %-11s | %-13s | %-13s | %-17s |\n",
		   "Policy", "Null ctr", "Max Allowed", "Min Guarantee",
		   "Pools Mapping", "Allocated Buffers");
	pr_buf_cat(buf, sz, *n,
		   "+--------|-------------|-------------|---------------|---------------|-------------------+\n");
	for (i = 0; i < bmgr_cfg.max_policies; i++) {
		if (!pp_bmgr_is_policy_active(i))
			continue;

		pp_bmgr_policy_stats_get(&policy_stats, i);
		pr_buf(pools, sizeof(pools), tmp, "%*pbl", PP_BM_MAX_POOLS,
		       policy_stats.pools_bmap);

		pr_buf_cat(buf, sz, *n,
			   "| %-6d | %11d | %11d | %13d | %-13s | %17d |\n", i,
			   policy_stats.policy_null_ctr,
			   policy_stats.policy_max_allowed,
			   policy_stats.policy_min_guaranteed, pools,
			   policy_stats.policy_alloc_buff);
	}

	pr_buf_cat(buf, sz, *n,
		   "+--------+-------------+-------------+---------------+---------------+-------------------+\n");

	/* Pools stats */
	pr_buf_cat(buf, sz, *n, "| %-6s | %-11s | %-11s | %-13s |\n", "Pool",
		   "Buffer Size", "Num Buffers", "Used Buffers");
	pr_buf_cat(buf, sz, *n,
		   "+--------|-------------|-------------|---------------+\n");

	for (i = 0; i < bmgr_cfg.max_pools; i++) {
		ret = bm_pool_stats_get(&pool_stats, i);
		if (unlikely(ret))
			continue;

		ret = pp_bmgr_pool_conf_get(i, &pool_cfg);
		if (unlikely(ret))
			continue;

		pr_buf_cat(buf, sz, *n, "| %-6u | %11u | %11u | %13u |\n", i,
			   pool_cfg.size_of_buffer, pool_stats.pool_size,
			   pool_stats.pool_allocated_ctr);
	}

	pr_buf_cat(buf, sz, *n,
		   "+--------+-------------+-------------+---------------+\n");

	/* Pools stats */
	pr_buf_cat(buf, sz, *n, "| %-34s | %12u%% |\n",
		   "RPB Memory Utilization",
		   (rpb_hw_stats.total_bytes_used * 100) / rpb_mem_sz);
	pr_buf_cat(buf, sz, *n, "| %-34s | %13u |\n", "RPB Bytes Used",
		   rpb_hw_stats.total_bytes_used);
	pr_buf_cat(buf, sz, *n, "| %-34s | %13u |\n", "RPB Packets Used",
		   rpb_hw_stats.total_pkts_used);
	pr_buf_cat(buf, sz, *n,
		   "+------------------------------------+---------------+\n");
	/* Genpool stats */
	pr_buf_cat(buf, sz, *n, "| %-34s | %13zu |\n",
		   "PP IOC Gen Pool Size (Bytes)",
		   pp_dma_get_pool_size(true));
	pr_buf_cat(buf, sz, *n, "| %-34s | %13zu |\n",
		   "PP IOC Gen Pool Used (Bytes)",
		   pp_dma_get_pool_size(true) - pp_dma_get_pool_avail(true));
	pr_buf_cat(buf, sz, *n, "| %-34s | %13zu |\n",
		   "PP NIOC Gen Pool Size (Bytes)",
		   pp_dma_get_pool_size(false));
	pr_buf_cat(buf, sz, *n, "| %-34s | %13zu |\n",
		   "PP NIOC Gen Pool Used (Bytes)",
		   pp_dma_get_pool_size(false) - pp_dma_get_pool_avail(false));
	pr_buf_cat(buf, sz, *n,
		   "+------------------------------------+---------------+\n");

	return 0;
}

void pp_global_stats_reset(void)
{
	struct pp_qos_dev *qdev;

	port_dist_reset_stat();
	rpb_stats_reset();
	prsr_stats_reset();
	cls_stats_reset();
	chk_stats_reset();
	mod_stats_reset();
	rx_dma_stats_reset();
	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	pp_qos_stats_reset(qdev);
	smgr_sq_dbg_stats_get(NULL, true);
	uc_stats_reset();
	uc_tdox_stats_reset();
	uc_ingress_stats_reset();
}

void pp_driver_stats_reset(void)
{
	pp_smgr_stats_reset();
	pmgr_stats_reset();
	misc_stats_reset();
}

s32 pp_global_stats_diff(void *pre, u32 num_pre, void *post, u32 num_post,
			 void *delta, u32 num_delta, void *data)
{
	struct pp_global_stats *__pre, *__post, *__delta;

	if (unlikely(ptr_is_null(pre) || ptr_is_null(post) ||
		     ptr_is_null(delta)))
		return -EINVAL;

	__pre   = pre;
	__post  = post;
	__delta = delta;
	U32_STRUCT_DIFF(&__pre->chk_stats, &__post->chk_stats,
			&__delta->chk_stats);
	U32_STRUCT_DIFF(&__pre->rpb_stats, &__post->rpb_stats,
			&__delta->rpb_stats);
	U32_STRUCT_DIFF(&__pre->cls_stats, &__post->cls_stats,
			&__delta->cls_stats);
	U32_STRUCT_DIFF(&__pre->mod_stats, &__post->mod_stats,
			&__delta->mod_stats);
	U32_STRUCT_DIFF(&__pre->qos_stats, &__post->qos_stats,
			&__delta->qos_stats);
	U32_STRUCT_DIFF(&__pre->parser_stats, &__post->parser_stats,
			&__delta->parser_stats);
	U32_STRUCT_DIFF(&__pre->syncq_stats, &__post->syncq_stats,
			&__delta->syncq_stats);
	U32_STRUCT_DIFF(&__pre->tdox_stats, &__post->tdox_stats,
			&__delta->tdox_stats);
	U32_STRUCT_DIFF(&__pre->ing_stats, &__post->ing_stats,
			&__delta->ing_stats);
	U64_STRUCT_DIFF(&__pre->port_dist_stats, &__post->port_dist_stats,
			&__delta->port_dist_stats);
	U64_STRUCT_DIFF(&__pre->rx_dma_stats, &__post->rx_dma_stats,
			&__delta->rx_dma_stats);
	U64_STRUCT_DIFF(&__pre->mcast_stats, &__post->mcast_stats,
			&__delta->mcast_stats);
	U64_STRUCT_DIFF(&__pre->reass_stats, &__post->reass_stats,
			&__delta->reass_stats);
	U64_STRUCT_DIFF(&__pre->frag_stats, &__post->frag_stats,
			&__delta->frag_stats);
	U64_STRUCT_DIFF(&__pre->remark_stats, &__post->remark_stats,
			&__delta->remark_stats);
	U64_STRUCT_DIFF(&__pre->ipsec_stats, &__post->ipsec_stats,
			&__delta->ipsec_stats);
	U64_STRUCT_DIFF(&__pre->aqm_lld_stats, &__post->aqm_lld_stats,
			&__delta->aqm_lld_stats);
	U64_STRUCT_DIFF(&__pre->lro_stats, &__post->lro_stats,
			&__delta->lro_stats);
	U64_STRUCT_DIFF(&__pre->egr_stats, &__post->egr_stats,
			&__delta->egr_stats);

	return 0;
}

s32 pp_global_stats_get(void *stats, u32 num_stats, void *data)
{
	struct pp_global_stats *__stats = stats;
	s32 ret;

	if (unlikely(ptr_is_null(stats) || ptr_is_null(data)))
		return -EINVAL;

	ret = port_dist_stats_get(&__stats->port_dist_stats);
	if (unlikely(ret))
		pr_err("port_dist_stats_get failed\n");

	ret = rpb_stats_get(&__stats->rpb_stats);
	if (unlikely(ret))
		pr_err("rpb_stats_get failed\n");

	ret = prsr_stats_get(&__stats->parser_stats);
	if (unlikely(ret))
		pr_err("prsr_stats_get failed\n");

	ret = cls_stats_get(&__stats->cls_stats);
	if (unlikely(ret))
		pr_err("cls_stats_get failed\n");

	ret = chk_stats_get(&__stats->chk_stats);
	if (unlikely(ret))
		pr_err("chk_stats_get failed\n");

	ret = mod_stats_get(&__stats->mod_stats);
	if (unlikely(ret))
		pr_err("mod_stats_get failed\n");

	ret = rx_dma_stats_get(&__stats->rx_dma_stats);
	if (unlikely(ret))
		pr_err("rx_dma_stats_get failed\n");

	ret = pp_qos_stats_get(data, &__stats->qos_stats);
	if (unlikely(ret))
		pr_err("pp_qos_stats_get failed\n");

	ret = uc_mcast_stats_get(&__stats->mcast_stats);
	if (unlikely(ret))
		pr_err("uc_mcast_stats_get failed\n");

	ret = uc_frag_stats_get(&__stats->frag_stats);
	if (unlikely(ret))
		pr_err("uc_frag_stats_get failed\n");

	ret = uc_remark_stats_get(&__stats->remark_stats);
	if (unlikely(ret))
		pr_err("uc_remark_stats_get failed\n");

	ret = uc_lro_stats_get(&__stats->lro_stats);
	if (unlikely(ret))
		pr_err("uc_lro_stats_get failed\n");

	ret = smgr_sq_dbg_stats_get(&__stats->syncq_stats, false);
	if (unlikely(ret))
		pr_err("smgr_sq_dbg_stats_get failed\n");

	ret = uc_reass_stats_get(&__stats->reass_stats);
	if (unlikely(ret))
		pr_err("uc_reass_stats_get failed\n");

	ret = uc_ipsec_stats_get(&__stats->ipsec_stats);
	if (unlikely(ret))
		pr_err("uc_ipsec_stats_get failed\n");

	ret = uc_lld_total_stats_get(&__stats->aqm_lld_stats);
	if (unlikely(ret))
		pr_err("uc_lld_total_stats_get failed\n");

	ret = uc_tdox_stats_get(&__stats->tdox_stats);
	if (unlikely(ret))
		pr_err("uc_tdox_stats_get failed\n");

	ret = uc_ingress_stats_get(&__stats->ing_stats);
	if (unlikely(ret))
		pr_err("uc_ingress_stats_get failed\n");

	ret = uc_egr_global_stats_get(&__stats->egr_stats);
	if (unlikely(ret))
		pr_err("uc_egr_global_stats_get failed\n");

	return 0;
}

s32 pp_brief_stats_get(void *stats, u32 num_stats, void *data)
{
	struct pp_active_sess_stats *sess_stats;
	s32    ret;
	u32    n_bits, i, sess;
	ulong  *bmap;

	sess_stats = (struct pp_active_sess_stats *)stats;

	if (unlikely(ptr_is_null(stats)))
		return -EINVAL;

	/* allocate bitmap for all sessions */
	ret = smgr_sessions_bmap_alloc(&bmap, &n_bits);
	if (unlikely(ret))
		return ret;

	/* get all opened sessions */
	ret = smgr_open_sessions_bmap_get(bmap, n_bits);
	if (unlikely(ret))
		goto done;

	/* iterate over all sessions */
	i = 0;
	for_each_set_bit(sess, bmap, n_bits) {
		if (i == num_stats)
			break;
		sess_stats[i].id = sess;
		pp_session_stats_get(sess_stats[i].id, &sess_stats[i].stats);
		i++;
	}

done:
	kfree(bmap);
	return ret;
}

static struct pp_active_sess_stats
__find_stat_by_id(struct pp_active_sess_stats *stats, u32 num, u32 id)
{
	u32 i;
	struct pp_active_sess_stats tmp = {0};

	tmp.id = id;
	if (unlikely(ptr_is_null(stats)) || (!num))
		return tmp;

	for (i = 0 ; i < num ; i++)
		if (stats[i].id == id) {
			memcpy(&tmp.stats, &stats[i].stats, sizeof(tmp.stats));
			break;
		}

	return tmp;
}

s32 pp_brief_stats_diff(void *pre, u32 num_pre, void *post, u32 num_post,
			 void *delta, u32 num_delta, void *data)
{
	int i, j;
	struct pp_active_sess_stats *__pre, *__post, *__delta, sess_stats;

	if (unlikely(ptr_is_null(pre) || ptr_is_null(post) ||
		     ptr_is_null(delta)))
		return -EINVAL;

	__pre   = pre;
	__post  = post;
	__delta = delta;

	for (i = 0, j = 0; i < num_pre && j < num_delta; i++)
		if (__pre[i].id == __post[i].id) {
			U64_STRUCT_DIFF(&__pre[i].stats,
			&__post[i].stats,
			&__delta[j++].stats);
		} else {
			/* if not match, we will try to find session in the
			 * pre. If not exists, it means that this is a new
			 * session and we will copy counters and id
			 */
			sess_stats = __find_stat_by_id(__pre, num_pre,
			__post[i].id);
			U64_STRUCT_DIFF(&sess_stats, &__post[i].stats,
			&__delta[j].stats);
			__delta[j++].id = __post[i].id;
		}
	return 0;
}

static s32 sess_stat_cmp_dec(const void *a, const void *b)
{
	const struct pp_active_sess_stats *aa, *bb;

	aa = (const struct pp_active_sess_stats *)a;
	bb = (const struct pp_active_sess_stats *)b;

	if (aa->stats.packets > bb->stats.packets)
		return -1;
	if (aa->stats.packets < bb->stats.packets)
		return 1;
	return 0;
}

s32 pp_brief_stats_show(char *buf, size_t sz, size_t *n, void *stats,
				u32 num_stats, void *data)
{
	struct sess_db_info info;
	struct pp_active_sess_stats *sess_stats;
	struct pp_misc_db *db = get_misc_db();
	u16 dst_q;
	int    i, j;
	char   l3Type[10], l4Prot[10];

	if (ptr_is_null(db))
		return -EINVAL;

	sess_stats = (struct  pp_active_sess_stats *) stats;

	if (unlikely(ptr_is_null(buf) || ptr_is_null(n) || ptr_is_null(stats)))
		return -EINVAL;

	/* Sort the active sessions */
	sort(sess_stats, num_stats, sizeof(*sess_stats), sess_stat_cmp_dec,
	     NULL);

	pr_buf(buf, sz, *n,
	       " +--------+----------+--------+---------+----------+---------+---------+------------+\n");
	pr_buf_cat(buf, sz, *n,
	       " | id     |  PPS     | Mbps   | In GPID | Out GPID | L3 Type | L4 Prot | Dest Queue |\n");

	/* Print only active sessions */
	j = min(db->dbg_print_cnt, num_stats);
	for (i = 0; i < j; i++) {
		if (!sess_stats[i].stats.packets)
			break;

		if (smgr_session_info_get(sess_stats[i].id, &info))
			continue;
		if (smgr_session_dest_queue_get(sess_stats[i].id, &dst_q))
			continue;
#ifdef CONFIG_DEBUG_FS
		if (PKTPRS_IS_IPV4(&info.rx, PKTPRS_HDR_LEVEL0))
			strcpy(l3Type, "IPv4");
		else if (PKTPRS_IS_IPV6(&info.rx, PKTPRS_HDR_LEVEL0))
			strcpy(l3Type, "IPv6");
		else
#endif
		{
			strcpy(l3Type, "N/A");
		}

#ifdef CONFIG_DEBUG_FS
		if (PKTPRS_IS_L2TP_OUDP(&info.rx, PKTPRS_HDR_LEVEL0) ||
		    PKTPRS_IS_L2TP_OIP(&info.rx, PKTPRS_HDR_LEVEL0))
			strcpy(l4Prot, "L2TP");
		else if (PKTPRS_IS_ESP(&info.rx, PKTPRS_HDR_LEVEL0))
			strcpy(l4Prot, "ESP");
		else if (PKTPRS_IS_TCP(&info.rx, PKTPRS_HDR_LEVEL0))
			strcpy(l4Prot, "TCP");
		else if (PKTPRS_IS_UDP(&info.rx, PKTPRS_HDR_LEVEL0))
			strcpy(l4Prot, "UDP");
		else
#endif
		{
			strcpy(l4Prot, "N/A");
		}

		pr_buf_cat(buf, sz, *n,
	       		" +--------+----------+--------+---------+----------+---------+---------+------------+\n");
		pr_buf_cat(buf, sz, *n,
		" | %-6u | %-8llu | %-6llu | %-7u | %-8u | %-8s| %-8s| %-11u|\n",
		   sess_stats[i].id, sess_stats[i].stats.packets,
		   sess_stats[i].stats.bytes*8/1000/1000,
		   info.in_port, info.eg_port, l3Type, l4Prot, dst_q);
	}

	pr_buf_cat(buf, sz, *n,
	       " +--------+----------+--------+---------+----------+---------+---------+------------+\n");
	return 0;
}

void pp_misc_set_brief_cnt(u32 cnt)
{
	struct pp_misc_db *db = get_misc_db();

	if (ptr_is_null(db))
		return;

	db->dbg_print_cnt = cnt ? cnt : 1;
}

s32 pp_driver_stats_diff(void *pre, u32 num_pre, void *post, u32 num_post,
			 void *delta, u32 num_delta, void *data)
{
	struct pp_driver_stats *__pre, *__post, *__delta;

	if (unlikely(ptr_is_null(pre) || ptr_is_null(post) ||
		     ptr_is_null(delta)))
		return -EINVAL;

	__pre   = pre;
	__post  = post;
	__delta = delta;

	U32_STRUCT_DIFF(&__pre->pmgr_stats, &__post->pmgr_stats,
			&__delta->pmgr_stats);
	U32_STRUCT_DIFF(&__pre->bmgr_stats, &__post->bmgr_stats,
			&__delta->bmgr_stats);
	U32_STRUCT_DIFF(&__pre->qos_stats, &__post->qos_stats,
			&__delta->qos_stats);
	U32_STRUCT_DIFF(&__pre->syncq_stats, &__post->syncq_stats,
			&__delta->syncq_stats);
	ATOMIC_STRUCT_DIFF(&__pre->smgr_stats, &__post->smgr_stats,
			   &__delta->smgr_stats);
	ATOMIC_STRUCT_DIFF(&__pre->misc_stats, &__post->misc_stats,
			   &__delta->misc_stats);

	return 0;
}

s32 pp_driver_stats_get(void *stats, u32 num_stats, void *data)
{
	struct pp_driver_stats *__stats = stats;
	s32 ret;

	if (unlikely(ptr_is_null(stats) || ptr_is_null(data)))
		return -EINVAL;

	ret = pmgr_stats_get(&__stats->pmgr_stats);
	if (unlikely(ret)) {
		pr_err("pmgr_stats_get failed\n");
		return ret;
	}

	ret = pp_smgr_stats_get(&__stats->smgr_stats);
	if (unlikely(ret)) {
		pr_err("pp_smgr_stats_get failed\n");
		return ret;
	}

	ret = smgr_sq_dbg_stats_get(&__stats->syncq_stats, false);
	if (unlikely(ret)) {
		pr_err("smgr_sq_dbg_stats_get failed\n");
		return ret;
	}

	ret = pp_bmgr_stats_get(&__stats->bmgr_stats);
	if (unlikely(ret)) {
		pr_err("pp_bmgr_stats_get failed\n");
		return ret;
	}

	ret = misc_stats_get(&__stats->misc_stats);
	if (unlikely(ret)) {
		pr_err("misc_stats_get failed\n");
		return ret;
	}

	ret = qos_drv_stats_get(data, &__stats->qos_stats);
	if (unlikely(ret)) {
		pr_err("qos_drv_stats_get failed\n");
		return ret;
	}

	return 0;
}

s32 pp_driver_stats_show(char *buf, size_t sz, size_t *n, void *stats,
			 u32 num_stats, void *data)
{
	struct pp_driver_stats *st = stats;
	struct qos_drv_stats *qos;
	struct smgr_stats    *smgr;
	struct smgr_sq_stats *synq;
	struct pmgr_stats    *pmgr;
	struct bmgr_stats    *bmgr;
	struct misc_stats    *misc;

	if (unlikely(ptr_is_null(buf) || ptr_is_null(n) || ptr_is_null(stats)))
		return -EINVAL;

	smgr = &st->smgr_stats;
	synq = &st->syncq_stats;
	pmgr = &st->pmgr_stats;
	bmgr = &st->bmgr_stats;
	misc = &st->misc_stats;
	qos  = &st->qos_stats;

	pr_buf(buf, sz, *n,
	       " +---------------------------------------------------------------------------------------+\n");
	pr_buf_cat(buf, sz, *n,
		   " |                          Packet Processor Driver Statistics                           |\n");
	pr_buf_cat(buf, sz, *n,
		   " +------------------------------+------------+------------------------------+------------+\n");

	pr_buf_cat(buf, sz, *n, " | %-28s | %-10u | %-28s | %-10u |\n",
		   "Open Sessions", atomic_read(&smgr->sess_open),
		   "Open Sessions High Watermark",
		   atomic_read(&smgr->sess_open_hi_wm));

	pr_buf_cat(buf, sz, *n, " | %-28s | %-10u | %-28s | %-10u |\n",
		   "Active Ports", pmgr->port_count, "Active Host IF",
		   pmgr->hif_dps_count);

	pr_buf_cat(buf, sz, *n, " | %-28s | %-10u | %-28s | %-10u |\n",
		   "Active Policies", bmgr->active_policies, "Active Pools",
		   bmgr->active_pools);

	pr_buf_cat(buf, sz, *n, " | %-28s | %-10u | %-28s | %-10u |\n",
		   "Active QOS Nodes", qos->active_nodes, "Active QOS Ports",
		   qos->active_ports);

	pr_buf_cat(buf, sz, *n, " | %-28s | %-10u | %-28s | %-10u |\n",
		   "Active QOS Queues", qos->active_queues,
		   "Active QOS Schedulers", qos->active_sched);

	pr_buf_cat(buf, sz, *n, " | %-28s | %-10u | %-28s | %-10u |\n",
		   "Active Sync Queues", synq->active,
		   "Free Sync Queues", synq->total - synq->active);

	pr_buf_cat(buf, sz, *n, " | %-28s | %-10u | %-28s | %-10u |\n",
		   "Session Create Requests",
		   atomic_read(&smgr->sess_create_req), "Sessions Created",
		   atomic_read(&smgr->sess_created));

	pr_buf_cat(buf, sz, *n, " | %-28s | %-10u | %-28s | %-10u |\n",
		   "Session Delete Requests",
		   atomic_read(&smgr->sess_delete_req), "Deleted Sessions",
		   atomic_read(&smgr->sess_deleted));

	pr_buf_cat(buf, sz, *n, " | %-28s | %-10u | %-28s | %-10u |\n",
		   "RX Hook SKBs", atomic_read(&misc->rx_hook_skb_cnt),
		   "TX Hook SKBs", atomic_read(&misc->tx_hook_skb_cnt));

	pr_buf_cat(buf, sz, *n, " | %-28s | %-10u | %-28s | %-10s |\n",
		   "RX Hook SKBs with LSPP Mark",
		   atomic_read(&misc->rx_hook_lspp_pkt_cnt), "", "");

	pr_buf_cat(buf, sz, *n,
		   " +------------------------------+------------+------------------------------+------------+\n");
	pr_buf_cat(buf, sz, *n,
		   " |                                         Errors                                        |\n");
	pr_buf_cat(buf, sz, *n,
		   " +------------------------------+------------+------------------------------+------------+\n");
	pr_buf_cat(buf, sz, *n, " | %-28s | %-10u | %-28s | %-10u |\n",
		   "Session Create Fail", atomic_read(&smgr->sess_create_fail),
		   "Session Delete Fail", atomic_read(&smgr->sess_delete_fail));
	pr_buf_cat(buf, sz, *n, " | %-28s | %-10u | %-28s | %-10u |\n",
		   "RX Hook NULL SKBs",
		   atomic_read(&misc->rx_hook_null_skb_cnt),
		   "TX Hook NULL SKBs",
		   atomic_read(&misc->tx_hook_null_skb_cnt));
	pr_buf_cat(buf, sz, *n, " | %-28s | %-10u | %-28s | %-10u |\n",
		   "Invalid Arguments", atomic_read(&smgr->invalid_args),
		   "Unsupported Protocols",
		   atomic_read(&smgr->sess_not_supported));
	pr_buf_cat(buf, sz, *n,
		   " +------------------------------+------------+------------------------------+------------+\n");
	pr_buf_cat(buf, sz, *n, "\n");

	return 0;
}

s32 pp_hal_global_stats_show(char *buf, size_t sz, size_t *n, void *stats,
			     u32 num_stats, void *data)
{
	struct pp_global_stats      *st = stats;
	struct pp_stats             *port_dist;
	struct rpb_stats            *rpb;
	struct prsr_stats           *parser;
	struct cls_stats            *cls;
	struct chk_stats            *chk;
	struct mod_stats            *mod;
	struct rx_dma_stats         *rx_dma;
	struct pp_qos_stats         *qos;
	struct mcast_stats          *mcast;
	struct reassembly_stats     *reass;
	struct frag_stats           *frag;
	struct ipsec_stats          *ipsec;
	struct aqm_lld_global_stats *lld;
	struct tdox_uc_stats        *tdox;
	struct remarking_stats      *remark;
	struct lro_stats            *lro;
	struct ing_stats            *ing;
	struct egr_glb_stats        *egr;
	u64 reass_err = 0, *u64_it;
	u32 parser_err = 0, *u32_it;

	if (unlikely(ptr_is_null(buf) || ptr_is_null(n) || ptr_is_null(stats)))
		return -EINVAL;

	port_dist = &st->port_dist_stats;
	rpb       = &st->rpb_stats;
	parser    = &st->parser_stats;
	cls       = &st->cls_stats;
	chk       = &st->chk_stats;
	mod       = &st->mod_stats;
	rx_dma    = &st->rx_dma_stats;
	qos       = &st->qos_stats;
	mcast     = &st->mcast_stats;
	reass     = &st->reass_stats;
	frag      = &st->frag_stats;
	ipsec     = &st->ipsec_stats;
	lld       = &st->aqm_lld_stats;
	remark    = &st->remark_stats;
	lro       = &st->lro_stats;
	tdox      = &st->tdox_stats;
	ing       = &st->ing_stats;
	egr       = &st->egr_stats;

	for_each_struct_mem(&reass->err, u64_it)
		reass_err += *u64_it;

	for_each_struct_mem(&parser->err, u32_it)
		parser_err += *u32_it;

	/* prints all statistics */
	pr_buf_cat(buf, sz, *n,
		   "+---------------------------+-------------------------+-------------------------+------------------------+\n");
	pr_buf_cat(buf, sz, *n,
		   "| Port Distributor          | RPB                     | Parser                  | Classifier             |\n");
	pr_buf_cat(buf, sz, *n,
		   "+---------------------------+-------------------------+-------------------------+------------------------+\n");
	pr_buf_cat(buf, sz, *n,
		   "| Rx            %10u  | Rx          %10u  | Rx          %10u  | Rx         %10u  |\n",
		   (u32)port_dist->packets, (u32)rpb->pkts_rcvd_from_port_dist,
		   parser->rx_valid_pkts,
		   cls->pkts_rcvd_from_parser + cls->pkts_rcvd_from_uc);

	pr_buf_cat(buf, sz, *n,
		"|                           | Drop        %10u  | Tx          %10u  | Rx(Parser) %10u  |\n",
		(u32)rpb->total_packets_dropped, parser->tx_pkts,
		cls->pkts_rcvd_from_parser);

	pr_buf_cat(buf, sz, *n,
		   "|                           |                         | Tx(Cls)     %10u  | Rx(uC)     %10u  |\n",
		   parser->tx_pkts_to_cls, cls->pkts_rcvd_from_uc);
	pr_buf_cat(buf, sz, *n,
		   "|                           |                         | Tx(uC)      %10u  | Search     %10u  |\n",
		   parser->tx_pkts_to_uc, cls->total_lu_requests);
	pr_buf_cat(buf, sz, *n,
		   "|                           |                         | Errors      %10u  | Match      %10u  |\n",
		   parser_err, cls->total_lu_matches);
	pr_buf_cat(buf, sz, *n,
		   "+---------------------------+-------------------------+-------------------------+------------------------+\n");

	pr_buf_cat(buf, sz, *n,
		   "| Checker                   | Modifier                | Rx-Dma                  | QoS                    |\n");
	pr_buf_cat(buf, sz, *n,
		   "+---------------------------+-------------------------+-------------------------+------------------------+\n");
	pr_buf_cat(buf, sz, *n,
		   "| Rx            %10u  | Rx          %10u  | Rx          %10u  | Rx         %10u  |\n",
		   chk->pkts_rcvd_from_cls,
		   mod->pkts_rcvd_from_chk + mod->pkts_rcvd_from_uc,
		   (u32)rx_dma->rx_pkts, (u32)qos->pkts_rcvd);

	pr_buf_cat(buf, sz, *n,
		"| Exception Cnt %10u  | Rx(Checker) %10u  | Tx          %10u  | Tx         %10u  |\n",
		chk->exception_pkt_cnt, mod->pkts_rcvd_from_chk,
		(u32)rx_dma->tx_pkts, (u32)qos->pkts_transmit);
	pr_buf_cat(buf, sz, *n,
		"| Dflt session  %10u  | Rx(uC)      %10u  | Drop        %10u  | Drop       %10u  |\n",
		chk->dflt_session_pkt_cnt, mod->pkts_rcvd_from_uc,
		(u32)rx_dma->drop_pkts, (u32)qos->pkts_dropped);

	pr_buf_cat(buf, sz, *n,
		   "|                           | Modified    %10u  | Recycled    %10u  |                        |\n",
		   mod->pkts_modified, (u32)rpb->total_packets_rx_dma_recycled);

	pr_buf_cat(buf, sz, *n,
		   "+---------------------------+-------------------------+-------------------------+------------------------+\n");

	pr_buf_cat(buf, sz, *n,
		   "| Multicast                 | Reassembly              | Fragmentation           | IPSec                  |\n");
	pr_buf_cat(buf, sz, *n,
		   "+---------------------------+-------------------------+-------------------------+------------------------+\n");
	pr_buf_cat(buf, sz, *n,
		   "| Rx            %10llu  | Rx          %10llu  | Rx          %10llu  | Rx         %10llu  |\n",
		   mcast->rx_pkt, reass->rx_pkts, frag->rx_pkt, ipsec->rx_pkt);
	pr_buf_cat(buf, sz, *n,
		   "| Tx            %10llu  | Tx          %10llu  | Tx          %10llu  | Tx         %10llu  |\n",
		   mcast->tx_pkt, reass->tx_pkts, frag->tx_pkt, ipsec->tx_pkt);
	pr_buf_cat(buf, sz, *n,
		   "| Drop          %10llu  | Drop        %10llu  | Drop        %10llu  | Errors     %10llu  |\n",
		   mcast->drop_pkt, reass->err.dropped, frag->total_drops,
		   ipsec->error_pkt);
	pr_buf_cat(buf, sz, *n,
		   "| Mirror TX     %10llu  | Reassembled %10llu  | Bmgr drops  %10llu  |                        |\n",
		   mcast->mirror_tx_pkt, reass->reassembled, frag->bmgr_drops);
	pr_buf_cat(buf, sz, *n,
		   "| Mirror Drop   %10llu  | Accelerated %10llu  | Df drops    %10llu  |                        |\n",
		   mcast->mirror_drop_pkt, reass->accelerated, frag->df_drops);
	pr_buf_cat(buf, sz, *n,
		   "|                           | Diverted    %10llu  | Max frags dr%10llu  |                        |\n",
		   reass->diverted, frag->max_frags_drops);
	pr_buf_cat(buf, sz, *n,
		   "|                           | Starvation  %10llu  |                         |                        |\n",
		   reass->err.frags_starv);
	pr_buf_cat(buf, sz, *n,
		   "|                           | Errors      %10llu  |                         |                        |\n",
		   reass_err);

	pr_buf_cat(buf, sz, *n,
		   "+---------------------------+-------------------------+-------------------------+------------------------+\n");

	pr_buf_cat(buf, sz, *n,
		   "| TDOX                      | INGRESS                 | EGRESS-FAT              | Remarking              |\n");
	pr_buf_cat(buf, sz, *n,
		   "+---------------------------+-------------------------+-------------------------+------------------------+\n");
	pr_buf_cat(buf, sz, *n,
		   "| Rx            %10u  | Rx-Checker  %10u  | Buf OOB      %10llu | Rx          %10llu |\n",
		   tdox->stage1, ing->rx_checker, egr->bm_buf_oob,
		   remark->rx_pkt);
	pr_buf_cat(buf, sz, *n,
		   "| Tx            %10u  | Rx-Parser   %10u  | Buf Rev OOB  %10llu | Tx          %10llu |\n",
		   (tdox->forward + tdox->expired), ing->rx_parser,
		   egr->bm_buf_rev_oob, remark->tx_pkt);
	pr_buf_cat(buf, sz, *n,
		   "| Rx-S2         %10u  | Frag        %10u  | Job Null Buf %10llu | Remarked    %10llu |\n",
		   tdox->stage2, ing->rx_frag, egr->null_job_null_buf,
		   remark->remarked_pkt);
	pr_buf_cat(buf, sz, *n,
		   "| Stored        %10u  | Frag_to_eg  %10u  |                         | Errors      %10llu |\n",
		   tdox->stored, ing->rx_frag_eg, remark->error_pkt);
	pr_buf_cat(buf, sz, *n,
		   "| Forward       %10u  | Wl          %10u  |                         |                        |\n",
		   tdox->forward, ing->rx_wl);
	pr_buf_cat(buf, sz, *n,
		   "| Suppress      %10u  | Lro         %10u  |                         |                        |\n",
		   (tdox->stored + tdox->aggressive), ing->rx_lro);
	pr_buf_cat(buf, sz, *n,
		   "| Ack ratio     %10u  | Rx-host     %10u  |                         |                        |\n",
		   tdox->forward ? (tdox->recycle / tdox->forward) : 0, ing->rx_host);
	pr_buf_cat(buf, sz, *n,
		   "| Aggressive    %10u  |                         |                         |                        |\n",
		   tdox->aggressive);

	pr_buf_cat(buf, sz, *n,
		   "+---------------------------+-------------------------+-------------------------+------------------------+\n");

	pr_buf_cat(buf, sz, *n,
		   "| LLD                       | LRO                     | SW AQM / Buffer control |                        |\n");
	pr_buf_cat(buf, sz, *n,
		   "+---------------------------+-------------------------+-------------------------+------------------------+\n");
	pr_buf_cat(buf, sz, *n,
		   "| Rx            %10llu  | Rx          %10llu  | AQM Rx       %10llu |                        |\n",
		   lld->rx_pkt, lro->rx_pkt, lld->aqm_sw_aggr.rx_pkt);
	pr_buf_cat(buf, sz, *n,
		   "| Ctx Err       %10llu  | Tx          %10llu  | Buf ctrl drop%10llu |                        |\n",
		   lld->ctx_error_pkt, lro->tx_pkt,
		   lld->aqm_sw_aggr.bc_drop_pkt);
	pr_buf_cat(buf, sz, *n,
		   "| Gen Err       %10llu  | Aggregated  %10llu  | AQM drop     %10llu |                        |\n",
		   lld->error_pkt, lro->agg_pkt,
		   lld->aqm_sw_aggr.aqm_drop_pkt);

	pr_buf_cat(buf, sz, *n,
		   "| LLD Rx        %10llu  | Exception   %10llu  | AQM Tx       %10llu |                        |\n",
			lld->lld_aggr.rx_pkt, lro->exp_pkt,
			lld->aqm_sw_aggr.tx_pkt);
	pr_buf_cat(buf, sz, *n,
		   "| LLD Rx ECT0   %10llu  | Drop        %10llu  |                         |                        |\n",
			lld->lld_aggr.rx_ect0_pkt, lro->drop_pkt);
	pr_buf_cat(buf, sz, *n,
		   "| LLD Rx ECT1   %10llu  | Error       %10llu  |                         |                        |\n",
			lld->lld_aggr.rx_ect1_pkt, lro->error_pkt);
	pr_buf_cat(buf, sz, *n,
		   "| LLD Rx CE     %10llu  |                         |                         |                        |\n",
		   lld->lld_aggr.rx_ce_pkt);
	pr_buf_cat(buf, sz, *n,
		   "| LLD Tx        %10llu  |                         |                         |                        |\n",
		   lld->lld_aggr.tx_pkt);
	pr_buf_cat(buf, sz, *n,
		   "| LLD Mark      %10llu  |                         |                         |                        |\n",
		   lld->lld_aggr.mark_pkt);
	pr_buf_cat(buf, sz, *n,
		   "| LLD Sanction  %10llu  |                         |                         |                        |\n",
		   lld->lld_aggr.sanction_pkt);
	pr_buf_cat(buf, sz, *n,
		   "| LLD Drop      %10llu  |                         |                         |                        |\n",
		   lld->lld_aggr.drop_pkt);
	pr_buf_cat(buf, sz, *n,
		   "+---------------------------+-------------------------+-------------------------+------------------------+\n\n");

	return 0;
}

s32 pp_global_stats_show(char *buf, size_t sz, size_t *n, void *stats,
			 u32 num_stats, void *data)
{
	struct pp_global_stats      *st = stats;
	struct pp_stats             *port_dist;
	struct rpb_stats            *rpb;
	struct prsr_stats           *parser;
	struct cls_stats            *cls;
	struct chk_stats            *chk;
	struct mod_stats            *mod;
	struct rx_dma_stats         *rx_dma;
	struct pp_qos_stats         *qos;
	struct mcast_stats          *mcast;
	struct smgr_sq_stats        *syncq;
	struct frag_stats           *frag;

	if (unlikely(ptr_is_null(buf) || ptr_is_null(n) || ptr_is_null(stats)))
		return -EINVAL;

	port_dist = &st->port_dist_stats;
	rpb       = &st->rpb_stats;
	parser    = &st->parser_stats;
	cls       = &st->cls_stats;
	chk       = &st->chk_stats;
	mod       = &st->mod_stats;
	rx_dma    = &st->rx_dma_stats;
	qos       = &st->qos_stats;
	mcast     = &st->mcast_stats;
	syncq     = &st->syncq_stats;
	frag      = &st->frag_stats;

	/* prints all statistics */
	pr_buf(buf, sz, *n,
	       "+-----------------------------------------------------------------------------------------------------+\n");
	pr_buf_cat(buf, sz, *n,
		   "|           Packet Processor Statistics                                                               |\n");
	pr_buf_cat(buf, sz, *n,
		   "+---------------------------+----------------------+---------------------------+----------------------+\n");

	pr_buf_cat(buf, sz, *n, "| %-25s | %-20llu | %-25s | %-20llu |\n",
		   "PP Rx Packets", port_dist->packets, "QoS Rx Packets",
		   qos->pkts_rcvd);
	pr_buf_cat(buf, sz, *n, "| %-25s | %-20llu | %-25s | %-20llu |\n",
		   "PP Tx Packets", rx_dma->tx_pkts, "QoS Tx Packets",
		   qos->pkts_transmit);
	pr_buf_cat(buf, sz, *n, "| %-25s | %-20llu | %-25s | %-20llu |\n",
		   "PP Rx Bytes", port_dist->bytes, "QoS Rx Bytes",
		   qos->bytes_rcvd);
	pr_buf_cat(buf, sz, *n, "| %-25s | %-20s | %-25s | %-20llu |\n", "", "",
		   "QoS Tx Bytes", qos->bytes_transmit);
	pr_buf_cat(buf, sz, *n, "| %-25s | %-20llu | %-25s | %-20llu |\n",
		   "PP Total Packets Drop",
		   rx_dma->drop_pkts + rpb->total_packets_dropped,
		   "QoS Total Packets Drop", qos->pkts_dropped);

	pr_buf_cat(buf, sz, *n,
		   "+---------------------------+----------------------+---------------------------+----------------------+\n");
	pr_buf_cat(buf, sz, *n, "| %-25s | %-20u | %-25s | %-20llu |\n",
		   "Accelerated Packets", chk->accelerated_pkts,
		   "TX Fragmented Packets", frag->tx_pkt);
	pr_buf_cat(buf, sz, *n, "| %-25s | %-20u | %-25s | %-20u |\n",
		   "Non-Accelerated Packets",
		   chk->pkts_rcvd_from_cls - chk->accelerated_pkts,
		   "TX   Syncq Packets", syncq->packets_accepted);
	pr_buf_cat(buf, sz, *n, "| %-25s | %-20llu | %-25s | %-20u |\n",
		   "RX Multicast Packets", mcast->rx_pkt,
		   "Drop Syncq Packets", syncq->packets_dropped);
	pr_buf_cat(buf, sz, *n, "| %-25s | %-20llu | %-25s | %-20llu |\n",
		   "TX Multicast Packets", mcast->tx_pkt,
		   "Drop Multicast Packets", mcast->drop_pkt);
	pr_buf_cat(buf, sz, *n,
		   "+---------------------------+----------------------+---------------------------+----------------------+\n");
	pr_buf_cat(buf, sz, *n, "| %-99s |\n", "          Drop Reasons");
	pr_buf_cat(buf, sz, *n,
		   "+---------------------------+----------------------+---------------------------+----------------------+\n");

	pr_buf_cat(buf, sz, *n, "| %-25s | %-20llu | %-25s | %-20llu |\n",
		   "Early Drop (RPB)", rpb->total_packets_dropped,
		   "QoS WRED Drop", qos->wred_pkts_dropped);
	pr_buf_cat(buf, sz, *n, "| %-25s | %-20llu | %-25s | %-20llu |\n",
		   "Null Buffer Drop", rx_dma->hw.bmgr_pkt_drop,
		   "QoS Codel Drop", qos->codel_pkts_dropped);
	pr_buf_cat(buf, sz, *n, "| %-25s | %-20llu | %-25s | %-20llu |\n",
		   "PPRS Drop", rx_dma->hw.pre_pp_pkt_drop, "Parser Drop",
		   rx_dma->hw.parser_pkt_drop + rx_dma->hw.parser_uc_pkt_drop +
			   rx_dma->hw.parser_pkt_drop);
	pr_buf_cat(buf, sz, *n, "| %-25s | %-20llu | %-25s | %-20llu |\n",
		   "Classifier Drop",
		   rx_dma->hw.hw_classifier_pkt_drop +
			   rx_dma->hw.classifier_uc_pkt_drop,
		   "Checker Drop", rx_dma->hw.checker_pkt_drop);
	pr_buf_cat(buf, sz, *n, "| %-25s | %-20llu | %-25s | %-20llu |\n",
		   "Modifier Drop", rx_dma->hw.robustness_violation_drop,
		   "No Policy Drop", rx_dma->hw.bmgr_no_match_pkt_drop);
	pr_buf_cat(buf, sz, *n, "| %-25s | %-20llu | %-25s | %-20llu |\n",
		   "FSQM Drop",
		   rx_dma->hw.fsqm_pkt_len_max_drop +
			   rx_dma->hw.fsqm_null_ptr_counter,
		   "Zero Len Drop", rx_dma->hw.zero_len_drop);
	pr_buf_cat(buf, sz, *n,
		   "+---------------------------+----------------------+---------------------------+----------------------+\n");
	pr_buf_cat(buf, sz, *n, "| %-99s |\n", "          Error Packets");
	pr_buf_cat(buf, sz, *n,
		   "+---------------------------+----------------------+---------------------------+----------------------+\n");
	pr_buf_cat(buf, sz, *n, "| %-25s | %-20u | %-25s | %-20u |\n",
		   "IP Length", parser->err.ip_len_err, "No Payload",
		   parser->err.payload_err);
	pr_buf_cat(buf, sz, *n, "| %-25s | %-20u | %-25s | %-20u |\n",
		   "Protocols Overflow", parser->err.proto_overflow,
		   "Protocol Error",
		   parser->err.proto_len_err + parser->err.hdr_len_err);
	pr_buf_cat(buf, sz, *n, "| %-25s | %-20u | %-25s | %-20u |\n",
		   "Unsupported Last ETH Type", parser->err.last_eth_err,
		   "RPB Error", parser->err.rpb_err);
	pr_buf_cat(buf, sz, *n,
		   "+---------------------------+----------------------+---------------------------+----------------------+\n");

	return 0;
}

#ifdef CONFIG_DEBUG_FS
u32 ignore_clk_updates_get(void)
{
	struct pp_misc_db *db = get_misc_db();

	if (ptr_is_null(db))
		return -EINVAL;

	return db->dbg_ignore_clock_freq_updates;
}

void ignore_clk_updates_set(u32 ignore_updates)
{
	struct pp_misc_db *db = get_misc_db();

	if (ptr_is_null(db))
		return;

	db->dbg_ignore_clock_freq_updates = ignore_updates;
}
#endif

/**
 * @brief Init pp misc driver database and update dev_priv->misc_db
 * @param dev
 * @param dev_priv
 * @param init_param
 * @return s32 0 on success
 */
static s32 __pp_misc_db_init(struct device *dev, struct pp_dev_priv *dev_priv,
					struct pp_misc_init_param *init_param)
{
	struct pp_qos_dev *qdev;
	struct pp_misc_db *db;
	u8 i;

	db = devm_kzalloc(dev, sizeof(*db), GFP_KERNEL);
	if (unlikely(!db)) {
		pr_err("Failed to allocate misc db memory of size %zu\n",
								sizeof(*db));
		return -ENOMEM;
	}

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (ptr_is_null(qdev))
		return -ENODEV;

	spin_lock_init(&db->lock);
	db->pp_ext_id = MXL_SKB_EXT_INVALID;

	/* Limit max printout to 10 lines by default */
	db->dbg_print_cnt = 10;
	/* Do not ignore updates by default */
	db->dbg_ignore_clock_freq_updates = 0;
	db->wl_grp_id = PMGR_GPID_GRP_INVALID;

	/* updateing the pointer to hold the db */
	dev_priv->misc_db = db;

	for (i = 0; i < PP_QOS_MAX_SERVICE_FLOWS; i++)
		db->sf_entry[i].fw_lld_ctx = PP_MAX_ASF;

	/* set aqm_engine according to qos init */
	db->aqm_engine = qdev->init_params.aqm_engine;

	return 0;
}

/**
 * @brief Called to update upon clock freq' changes
 * @param nb notifier block
 * @param e new clock EPU_ADP_DFS_LVL_H | EPU_ADP_DFS_LVL_M | EPU_ADP_DFS_LVL_L
 * @param data not relevant
 * @return s32
 */
static int pm_notifier(struct notifier_block *nb, unsigned long e, void *data)
{
	struct pp_misc_db *db = get_misc_db();
	struct pp_qos_dev *qdev;
	s32 ret = 0;

	if (ptr_is_null(db))
		return -EPERM;

	/* in case we would like to ignore updates, then just return */
	if (db->dbg_ignore_clock_freq_updates) {
		pr_debug("DBG: PPv4 driver ignores clock freq update\n");
		return 0;
	}

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (ptr_is_null(qdev))
		return -ENODEV;

	/* due to EPU HW resposible for dynamic clock changes via
	 * HW, the QoS FW get an interrupt when the clock gets changed,
	 * and responsible for configuring all relevant modules in PP.
	 */
	ret = qos_clock_update(qdev);

	return 0;
}

s32 pp_host_cpu_info_init(struct pp_cpu_info *cpu, unsigned int num_cpus)
{
	struct pp_misc_db *db = get_misc_db();
	s32 id, ret;

	if (ptr_is_null(db))
		return -EPERM;

	ret = pmgr_protected_grp_host_cpu_info_set(cpu, num_cpus, &id);
	if (!ret)
		db->wl_grp_id = id;

	return ret;
}
EXPORT_SYMBOL(pp_host_cpu_info_init);

s32 pp_whitelist_rule_add(u32 prio, struct pp_whitelist_field *fields, u32 cnt)
{
	struct pp_misc_db *db = get_misc_db();

	if (ptr_is_null(db))
		return -EPERM;

	return pp_gpid_group_rule_add(db->wl_grp_id, prio, fields, cnt);
}
EXPORT_SYMBOL(pp_whitelist_rule_add);

s32 pp_whitelist_rule_del(u32 prio, struct pp_whitelist_field *fields, u32 cnt)
{
	struct pp_misc_db *db = get_misc_db();

	if (ptr_is_null(db))
		return -EPERM;

	return pp_gpid_group_rule_del(db->wl_grp_id, prio, fields, cnt);
}
EXPORT_SYMBOL(pp_whitelist_rule_del);

s32 pp_misc_whitelist_grp_id_get(void)
{
	struct pp_misc_db *db = get_misc_db();

	if (ptr_is_null(db))
		return -EPERM;

	return db->wl_grp_id;
}

s32 pp_dpl_hash_bit_enable(u32 index)
{
	struct pp_misc_db *db = get_misc_db();

	if (ptr_is_null(db))
		return -EINVAL;

	spin_lock_bh(&db->lock);
	uc_ing_hash_bit_enable(index);
	spin_unlock_bh(&db->lock);
	return 0;
}
EXPORT_SYMBOL(pp_dpl_hash_bit_enable);

s32 pp_dpl_hash_bit_disable(u32 index)
{
	struct pp_misc_db *db = get_misc_db();

	if (ptr_is_null(db))
		return -EINVAL;

	spin_lock_bh(&db->lock);
	uc_ing_hash_bit_disable(index);
	spin_unlock_bh(&db->lock);
	return 0;
}
EXPORT_SYMBOL(pp_dpl_hash_bit_disable);

s32 pp_dpl_hash_bit_reset(void)
{
	struct pp_misc_db *db = get_misc_db();

	if (ptr_is_null(db))
		return -EINVAL;

	spin_lock_bh(&db->lock);
	uc_ing_hash_bit_reset();
	spin_unlock_bh(&db->lock);
	return 0;
}
EXPORT_SYMBOL(pp_dpl_hash_bit_reset);

s32 pp_misc_init(struct device *dev)
{
	s32 ret;
	struct pp_dev_priv *dev_priv;
	struct pp_misc_init_param *init_param;
	struct pp_misc_db *db;

	if (unlikely(ptr_is_null(dev)))
		return -EINVAL;

	dev_priv = pp_priv_get();
	if (unlikely(ptr_is_null(dev_priv)))
		return -EINVAL;

	init_param = &dev_priv->dts_cfg.misc_params;
	if (!init_param->valid)
		return -EINVAL;

	pr_debug("base addr %#lx\n", (ulong)init_param->ppv4_base);

	/* database */
	ret = __pp_misc_db_init(dev, dev_priv, init_param);
	if (unlikely(ret)) {
		pr_err("Failed to init database, ret %d\n", ret);
		return ret;
	}
	db = (struct pp_misc_db*)dev_priv->misc_db;

	/* SI fields encoding/decoding init */
	pp_si_init();

	/* PP mxl_skb_ext */
	db->pp_ext_id = mxl_skb_ext_register(PP_DESC_EXT_NAME,
						sizeof(struct pp_desc));
	if (db->pp_ext_id == MXL_SKB_EXT_INVALID) {
		pr_err("Failed registering to mxl skb ext, get MXL_SKB_EXT_INVALID\n");
		return -EINVAL;
	}

	ret = pp_misc_sysfs_init(init_param->sysfs);
	if (unlikely(ret)) {
		pr_err("Failed to init sysfs stuff, ret %d\n", ret);
		return ret;
	}

	ret = epu_adp_lvl_notify_register(&nb);
	if (unlikely(ret))
		pr_err("Failed to epu_adp_lvl_notify_register\n");

	/* debug */
	ret = pp_misc_dbg_init(init_param);
	if (unlikely(ret)) {
		pr_err("Failed to init debugfs, ret %d\n", ret);
		return ret;
	}

	return 0;
}

void pp_misc_exit(void)
{
	struct pp_dev_priv *dev_priv = pp_priv_get();

	if (!dev_priv)
		return;

	/* clean debug stuff */
	pp_misc_dbg_clean();

	pp_misc_sysfs_clean();

	dev_priv->misc_db = NULL;
	epu_adp_lvl_notify_unregister(&nb);

	pr_debug("done\n");
}

