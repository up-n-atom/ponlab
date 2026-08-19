/*****************************************************************************
 *
 * Copyright (c) 2020 - 2025 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <linux/errno.h>
#include "drv_pon_mbox_counters_conv.h"
#include "drv_pon_mbox.h"

int pon_mbox_cnt_gtc_fw2pon(struct ponfw_gtc_counters *in,
			    struct pon_mbox_gtc_counters *out)
{
	if (!in || !out)
		return -EINVAL;

	out->bip_errors = in->ds_bip_err;

	out->disc_gem_frames = (__u64)in->dis_gem_hi << 32 | in->dis_gem_lo;

	out->gem_hec_errors_uncorr =
	    (__u64)in->gem_hec_err_hi << 32 | in->gem_hec_err_lo;

	out->bytes_corr = (__u64)in->corr_bytes_hi << 32 | in->corr_bytes_lo;

	out->fec_codewords_corr =
	    (__u64)in->corr_fec_cw_hi << 32 | in->corr_fec_cw_lo;

	out->fec_codewords_uncorr =
	    (__u64)in->uncorr_fec_cw_hi << 32 | in->uncorr_fec_cw_lo;

	out->total_frames =
	    (__u64)in->frames_total_hi << 32 | in->frames_total_lo;

	out->fec_sec = (__u64)in->fec_sec;

	out->gem_idle = (__u64)in->idle_gem_hi << 32 | in->idle_gem_lo;

	out->ploam_crc_errors = (__u64)in->ploam_crc_err;

	return 0;
}

int pon_mbox_cnt_gtc_enh_fw2pon(struct ponfw_gtc_enhanced_counters *in,
				struct pon_mbox_gtc_counters *out)
{
	if (!in || !out)
		return -EINVAL;

	out->bwmap_hec_errors_corr =
	    (__u64)in->bwmap_err_corr_hi << 32 | in->bwmap_err_corr_lo;

	out->gem_hec_errors_corr =
	    (__u64)in->gem_hec_err_corr_hi << 32 | in->gem_hec_err_corr_lo;

	out->lods_events = (__u64)in->total_lods_lof;

	out->dg_time = (__u64)in->dg_time;

	return 0;
}

int pon_mbox_cnt_xgtc_fw2pon(struct ponfw_xgtc_counters *in,
			     struct pon_mbox_xgtc_counters *out)
{
	if (!in || !out)
		return -EINVAL;

	out->psbd_hec_err_uncorr =
	    (__u64)in->psbd_err_uncorr_hi << 32 | in->psbd_err_uncorr_lo;

	out->psbd_hec_err_corr =
	    (__u64)in->psbd_err_corr_hi << 32 | in->psbd_err_corr_lo;

	out->fs_hec_err_uncorr =
	    (__u64)in->fs_err_uncorr_hi << 32 | in->fs_err_uncorr_lo;

	out->fs_hec_err_corr =
	    (__u64)in->fs_err_corr_hi << 32 | in->fs_err_corr_lo;

	out->lost_words =
	    (__u64)in->lost_hec_wc_hi << 32 | in->lost_hec_wc_lo;

	out->ploam_mic_err = in->ploam_mic_err;

	return 0;
}

int pon_mbox_cnt_gem_port_fw2pon(struct ponfw_gem_port_counters *in,
				 struct pon_mbox_gem_port_counters *out)
{
	if (!in || !out)
		return -EINVAL;

	out->tx_frames = ((__u64)in->tx_gem_fram_hi << 32 |
				    in->tx_gem_fram_lo);

	out->tx_fragments = ((__u64)in->tx_gem_frag_hi << 32 |
				       in->tx_gem_frag_lo);

	out->tx_bytes = ((__u64)in->tx_bytes_hi << 32 |
				   in->tx_bytes_lo);

	out->rx_frames = ((__u64)in->rx_gem_fram_hi << 32 |
				    in->rx_gem_fram_lo);

	out->rx_fragments = ((__u64)in->rx_gem_frag_hi << 32 |
				       in->rx_gem_frag_lo);

	out->rx_bytes = ((__u64)in->rx_bytes_hi << 32 |
				   in->rx_bytes_lo);

	out->key_errors = ((__u64)in->ds_xgem_key_err_hi << 32 |
				     in->ds_xgem_key_err_lo);
	return 0;
}

int pon_mbox_cnt_alloc_id_fw2pon(struct ponfw_alloc_id_counters *in,
				 struct pon_alloc_counters *out)
{
	if (!in || !out)
		return -EINVAL;

	out->allocations = (__u64)in->rec_tcont_alloc_hi << 32 |
				     in->rec_tcont_alloc_lo;

	out->idle = (__u64)in->us_gem_idle_hi << 32 |
			      in->us_gem_idle_lo;

	return 0;
}

int pon_mbox_cnt_alloc_bw_fw2pon(struct ponfw_alloc_bw *in,
				 struct pon_alloc_counters *out)
{
	if (!in || !out)
		return -EINVAL;

	out->us_bw = (__u64)in->us_bw_hi << 32 | in->us_bw_lo;

	return 0;
}

int pon_mbox_cnt_alloc_lost_fw2pon(struct ponfw_alloc_lost_counters *in,
				   struct pon_alloc_discard_counters *out)
{
	if (!in || !out)
		return -EINVAL;

	out->disc[0] = (__u64)in->discard0_hi << 32 | in->discard0_lo;
	out->disc[1] = (__u64)in->discard1_hi << 32 | in->discard1_lo;
	out->disc[2] = (__u64)in->discard2_hi << 32 | in->discard2_lo;
	out->disc[3] = (__u64)in->discard3_hi << 32 | in->discard3_lo;
	out->disc[4] = (__u64)in->discard4_hi << 32 | in->discard4_lo;
	out->disc[5] = (__u64)in->discard5_hi << 32 | in->discard5_lo;
	out->disc[6] = (__u64)in->discard6_hi << 32 | in->discard6_lo;
	out->disc[7] = (__u64)in->discard7_hi << 32 | in->discard7_lo;

	out->rule[2] = in->rule2;
	out->rule[4] = in->rule4;
	out->rule[6] = in->rule6;
	out->rule[7] = in->rule7;
	out->rule[8] = in->rule8;
	out->rule[9] = in->rule9;
	out->rule[10] = in->rule10;
	out->rule[11] = in->rule11;
	out->rule[12] = in->rule12;
	out->rule[13] = in->rule13;
	out->rule[14] = in->rule14;
	out->rule[15] = in->rule15;
	out->rule[16] = in->rule16;

	return 0;
}

int pon_mbox_cnt_rx_eth_fw2pon(struct ponfw_rx_eth_counters *in,
			       struct pon_eth_counters *out)
{
	if (!in || !out)
		return -EINVAL;

	out->bytes =
	    (__u64)in->rx_eth_bytes_hi << 32 | in->rx_eth_bytes_lo;
	out->frames_lt_64 =
	    (__u64)in->rx_eth_fr_lt64_hi << 32 | in->rx_eth_fr_lt64_lo;
	out->frames_64 =
	    (__u64)in->rx_eth_fr_eq64_hi << 32 | in->rx_eth_fr_eq64_lo;
	out->frames_65_127 =
	    (__u64)in->rx_eth_fr_lt128_hi << 32 | in->rx_eth_fr_lt128_lo;
	out->frames_128_255 =
	    (__u64)in->rx_eth_fr_lt256_hi << 32 | in->rx_eth_fr_lt256_lo;
	out->frames_256_511 =
	    (__u64)in->rx_eth_fr_lt512_hi << 32 | in->rx_eth_fr_lt512_lo;
	out->frames_512_1023 =
	    (__u64)in->rx_eth_fr_lt1k_hi << 32 | in->rx_eth_fr_lt1k_lo;
	out->frames_1024_1518 =
	    (__u64)in->rx_eth_fr_lt1k5_hi << 32 | in->rx_eth_fr_lt1k5_lo;
	out->frames_gt_1518 =
	    (__u64)in->rx_eth_fr_gt1k5_hi << 32 | in->rx_eth_fr_gt1k5_lo;
	out->frames_fcs_err =
	    (__u64)in->rx_eth_fcs_err_hi << 32 | in->rx_eth_fcs_err_lo;
	out->bytes_fcs_err =
	    (__u64)in->rx_eth_fcs_bytes_hi << 32 | in->rx_eth_fcs_bytes_lo;
	out->frames_too_long =
	    (__u64)in->rx_eth_too_long_hi << 32 | in->rx_eth_too_long_lo;

	return 0;
}

int pon_mbox_cnt_tx_eth_fw2pon(struct ponfw_tx_eth_counters *in,
			       struct pon_eth_counters *out)
{
	if (!in || !out)
		return -EINVAL;

	out->bytes =
	    (__u64)in->tx_eth_bytes_hi << 32 | in->tx_eth_bytes_lo;
	out->frames_lt_64 =
	    (__u64)in->tx_eth_fr_lt64_hi << 32 | in->tx_eth_fr_lt64_lo;
	out->frames_64 =
	    (__u64)in->tx_eth_fr_eq64_hi << 32 | in->tx_eth_fr_eq64_lo;
	out->frames_65_127 =
	    (__u64)in->tx_eth_fr_lt128_hi << 32 | in->tx_eth_fr_lt128_lo;
	out->frames_128_255 =
	    (__u64)in->tx_eth_fr_lt256_hi << 32 | in->tx_eth_fr_lt256_lo;
	out->frames_256_511 =
	    (__u64)in->tx_eth_fr_lt512_hi << 32 | in->tx_eth_fr_lt512_lo;
	out->frames_512_1023 =
	    (__u64)in->tx_eth_fr_lt1k_hi << 32 | in->tx_eth_fr_lt1k_lo;
	out->frames_1024_1518 =
	    (__u64)in->tx_eth_fr_lt1k5_hi << 32 | in->tx_eth_fr_lt1k5_lo;
	out->frames_gt_1518 =
	    (__u64)in->tx_eth_fr_gt1k5_hi << 32 | in->tx_eth_fr_gt1k5_lo;
	/* The following counters are only applicable for RX direction.
	 * Since this function handles TX direction, these fields are set to 0
	 * to indicate that FCS errors, FCS error bytes, and frames too long
	 * are not counted or relevant for transmitted frames.
	 */
	out->frames_fcs_err = 0;
	out->bytes_fcs_err = 0;
	out->frames_too_long = 0;

	return 0;
}

int pon_mbox_cnt_twdm_lods_fw2pon(struct ponfw_twdm_lods_counters *in,
				  struct pon_mbox_twdm_lods_counters *out)
{
	if (!in || !out)
		return -EINVAL;

	out->events_all = (__u64)in->lods;

	out->react_oper = (__u64)in->react_oper;
	out->react_prot = (__u64)in->react_prot;
	out->react_disc = (__u64)in->react_disc;

	out->rest_oper = (__u64)in->rest_oper;
	out->rest_prot = (__u64)in->rest_prot;
	out->rest_disc = (__u64)in->rest_disc;

	return 0;
}

int pon_mbox_cnt_twdm_optic_pl_fw2pon(
	struct ponfw_twdm_onu_optic_pl_counters *in,
	struct pon_mbox_twdm_optic_pl_counters *out)
{
	if (!in || !out)
		return -EINVAL;

	out->rejected = (__u64)in->cpl_rejected;
	out->incomplete = (__u64)in->cpl_incomplete;
	out->complete = (__u64)in->cpl_complete;

	return 0;
}

int pon_mbox_cnt_twdm_tc_fw2pon(struct ponfw_twdm_tc_counters *in,
				struct pon_mbox_twdm_tc_counters *out)
{
	unsigned int i = 0;
	unsigned int min_size;

	if (!in || !out)
		return -EINVAL;

	min_size = ARRAY_SIZE(out->tc_);

	/* Ensure we do not read out of bounds from in->tc_ */
	if (ARRAY_SIZE(in->tc_) < min_size)
		min_size = ARRAY_SIZE(in->tc_);

	for (i = 0; i < min_size; i++)
		out->tc_[i] = (__u64)in->tc_[i];

	return 0;
}

int pon_mbox_cnt_xgtc_ploam_ds_fw2pon(
	struct ponfw_xgtc_ploam_ds_counters *in,
	struct pon_mbox_xgtc_ploam_ds_counters *out)
{
	if (!in || !out)
		return -EINVAL;

	out->bst_profile = (__u64)in->bst_profile;
	out->ass_onu = (__u64)in->ass_onu;
	out->rng_time = (__u64)in->rng_time;
	out->deact_onu = (__u64)in->deact_onu;
	out->dis_ser = (__u64)in->dis_ser;
	out->req_reg = (__u64)in->req_reg;
	out->ass_alloc = (__u64)in->ass_alloc;
	out->key_ctrl = (__u64)in->key_ctrl;
	out->slp_allow = (__u64)in->slp_allow;
	out->calib_req = (__u64)in->calib_req;
	out->adj_tx_wl = (__u64)in->adj_tx_wl;
	out->tune_ctrl = (__u64)in->tune_ctrl;
	out->sys_profile = (__u64)in->sys_profile;
	out->ch_profile = (__u64)in->ch_profile;
	out->prot_control = (__u64)in->prot_control;
	out->chg_pw_lvl = (__u64)in->chg_pw_lvl;
	out->pw_cons = (__u64)in->pw_cons;
	out->rate_ctrl = (__u64)in->rate_ctrl;
	out->reboot_onu = (__u64)in->reboot_onu;
	out->unknown = (__u64)in->unknown;
	out->adj_tx_wl_fail = (__u64)in->adj_tx_wl_fail;
	out->tune_req = (__u64)in->tune_req;
	out->tune_compl = (__u64)in->tune_compl;

	return 0;
}

int pon_mbox_cnt_gtc_ploam_ds_fw2pon(struct ponfw_gtc_ploam_ds_counters *in,
				     struct pon_mbox_gtc_ploam_ds_counters *out)
{
	if (!in || !out)
		return -EINVAL;

	out->us_ovh = (__u64)in->us_ovh;
	out->ass_onu = (__u64)in->ass_onu;
	out->rng_time = (__u64)in->rng_time;
	out->deact_onu = (__u64)in->deact_onu;
	out->dis_ser = (__u64)in->dis_ser;
	out->enc_gem_pid = (__u64)in->enc_gem_pid;
	out->req_pw = (__u64)in->req_pw;
	out->ass_alloc = (__u64)in->ass_alloc;
	out->no_msg = (__u64)in->no_msg;
	out->popup = (__u64)in->popup;
	out->req_key = (__u64)in->req_key;
	out->cfg_port = (__u64)in->cfg_port;
	out->phy_ee = (__u64)in->phy_ee;
	out->chg_pl = (__u64)in->chg_pl;
	out->pst = (__u64)in->pst;
	out->ber = (__u64)in->ber;
	out->key_swtime = (__u64)in->key_swtime;
	out->ext_burst = (__u64)in->ext_burst;
	out->pon_id = (__u64)in->pon_id;
	out->swift_popup = (__u64)in->swift_popup;
	out->rng_adj = (__u64)in->rng_adj;
	out->slp_allow = (__u64)in->slp_allow;
	out->unknown = (__u64)in->unknown;

	return 0;
}

int pon_mbox_cnt_gtc_ploam_us_fw2pon(struct ponfw_gtc_ploam_us_counters *in,
				     struct pon_mbox_gtc_ploam_us_counters *out)
{
	if (!in || !out)
		return -EINVAL;

	out->ser_onu = (__u64)in->ser_onu;
	out->password = (__u64)in->password;
	out->dyg_gasp = (__u64)in->dyg_gasp;
	out->no_msg = (__u64)in->no_msg;
	out->enc_key = (__u64)in->enc_key;
	out->phy_ee = (__u64)in->phy_ee;
	out->pst_msg = (__u64)in->pst_msg;
	out->rem_err = (__u64)in->rem_err;
	out->ack = (__u64)in->ack;
	out->slp_req = (__u64)in->slp_req;

	return 0;
}

int pon_mbox_cnt_xgtc_ploam_us_fw2pon(
	struct ponfw_xgtc_ploam_us_counters *in,
	struct pon_mbox_xgtc_ploam_us_counters *out)
{
	if (!in || !out)
		return -EINVAL;

	out->ser_onu = (__u64)in->ser_onu;
	out->reg = (__u64)in->reg;
	out->key_rep = (__u64)in->key_rep;
	out->ack = (__u64)in->ack;
	out->slp_req = (__u64)in->slp_req;
	out->tun_res = (__u64)in->tun_res;
	out->pw_cons = (__u64)in->pw_cons;
	out->rate_resp = (__u64)in->rate_resp;
	out->cpl_err = (__u64)in->cpl_err;
	out->tun_res_an = (__u64)in->tun_res_an;
	out->tun_res_crb = (__u64)in->tun_res_crb;

	return 0;
}
