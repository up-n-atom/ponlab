/*****************************************************************************
 *
 * Copyright (c) 2021 - 2025 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include "pon_adapter.h"
#include "omci/me/pon_adapter_enhanced_tc_pmhd.h"
#include "omci/me/pon_adapter_fec_pmhd.h"
#include "omci/me/pon_adapter_gem_port_network_ctp.h"
#include "omci/me/pon_adapter_gem_port_net_ctp_pmhd.h"
#include "omci/me/pon_adapter_management_pmhd.h"
#include "omci/me/pon_adapter_twdm_channel_xgem_pmhd.h"
#include "omci/me/pon_adapter_twdm_channel_phy_lods_pmhd.h"
#include "omci/me/pon_adapter_twdm_channel_ploam_pmhd.h"
#include "omci/me/pon_adapter_twdm_channel_tuning_pmhd.h"
#include "../fapi_pon_pa_common.h"
#include "fapi_pon.h"

/*
 * PON Adapter wrappers and structures
 */

static enum pon_adapter_errno fec_cnt_get(void *ll_handle, uint16_t me_id,
					  uint64_t *cnt_corrected_bytes,
					  uint64_t *cnt_corrected_code_words,
					  uint64_t *cnt_uncorrected_code_words,
					  uint64_t *cnt_total_code_words,
					  uint16_t *cnt_fec_seconds)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode ret;
	struct pon_fec_counters fec_counters;
	uint8_t dswlch_id = 0;
	uint8_t pon_mode = PON_MODE_UNKNOWN;

	pthread_mutex_lock(&ctx->lock);

	ret = fapi_pon_mode_get(pon_ctx, &pon_mode);
	if (ret) {
		pthread_mutex_unlock(&ctx->lock);

		return pon_fapi_to_pa_error(ret);
	}

	if (pon_mode == PON_MODE_989_NGPON2_2G5 ||
	    pon_mode == PON_MODE_989_NGPON2_10G) {
		dswlch_id = me_id & 0xFF;
		ret = fapi_pon_twdm_fec_counters_get(pon_ctx, dswlch_id,
						     &fec_counters);
	} else {
		ret = fapi_pon_fec_counters_get(pon_ctx, &fec_counters);
	}

	pthread_mutex_unlock(&ctx->lock);
	if (ret)
		return pon_fapi_to_pa_error(ret);

	*cnt_corrected_bytes = fec_counters.bytes_corr;
	*cnt_corrected_code_words = fec_counters.words_corr;
	*cnt_uncorrected_code_words = fec_counters.words_uncorr;
	*cnt_total_code_words = fec_counters.words;
	*cnt_fec_seconds = (uint16_t)fec_counters.seconds;

	return PON_ADAPTER_SUCCESS;
}

const struct pa_fec_pmhd_ops pon_pa_fec_pmhd_ops = {
	.cnt_get = fec_cnt_get,
};

static enum pon_adapter_errno gem_port_net_ctp_update(
	void *ll_handle, const uint16_t me_id,
	const struct pa_gem_port_net_ctp_update_data *upd_data)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct mapper *mapper = ctx->mapper[MAPPER_GEMPORTCTP_MEID_TO_ID];
	enum pon_adapter_errno ret;

	pthread_mutex_lock(&ctx->lock);
	/* unconditionally remove possible previous mapping */
	mapper_id_remove(mapper, me_id);

	/* set new/updated mapping */
	ret = mapper_explicit_map(mapper, me_id, upd_data->gem_port_id);
	pthread_mutex_unlock(&ctx->lock);
	/* if we have an error here, it is always because of wrong values */
	if (ret)
		return PON_ADAPTER_ERR_INVALID_VAL;

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno gem_port_net_ctp_destroy(
	void *ll_handle, const uint16_t me_id,
	const struct pa_gem_port_net_ctp_destroy_data *dst_data)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;

	UNUSED(dst_data);

	pthread_mutex_lock(&ctx->lock);
	mapper_id_remove(ctx->mapper[MAPPER_GEMPORTCTP_MEID_TO_ID], me_id);
	pthread_mutex_unlock(&ctx->lock);

	return PON_ADAPTER_SUCCESS;
}

/* GEM Port Network CTP is needed here only to get the dependency between me_id
 * and gem_port_id, which is needed to get the related counters.
 * If something more for this ME needs to be implemented, this should be moved
 * to a new file.
 */
const struct pa_gem_port_net_ctp_ops pon_pa_gem_port_net_ctp_ops = {
	.update = gem_port_net_ctp_update,
	.destroy = gem_port_net_ctp_destroy,
};

static enum pon_adapter_errno gem_port_net_ctp_cnt_get(
	void *ll_handle, uint16_t me_id, uint64_t *tx_gem_frames,
	uint64_t *rx_gem_frames, uint64_t *rx_payload_bytes,
	uint64_t *tx_payload_bytes, uint32_t *key_errors)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	struct mapper *mapper = ctx->mapper[MAPPER_GEMPORTCTP_MEID_TO_ID];
	enum fapi_pon_errorcode err;
	enum pon_adapter_errno ret;
	struct pon_gem_port_counters gem_port_counters = { 0 };
	uint32_t gem_port_id = 0;

	pthread_mutex_lock(&ctx->lock);
	ret = mapper_index_get(mapper, me_id, &gem_port_id);
	if (ret)
		goto out;

	err = fapi_pon_gem_port_counters_get(pon_ctx, gem_port_id,
					     &gem_port_counters);
	if (err) {
		ret = pon_fapi_to_pa_error(err);
		goto out;
	}

	*tx_gem_frames =
		gem_port_counters.tx_frames + gem_port_counters.tx_fragments;
	*rx_gem_frames =
		gem_port_counters.rx_frames + gem_port_counters.rx_fragments;
	*rx_payload_bytes = gem_port_counters.rx_bytes;
	*tx_payload_bytes = gem_port_counters.tx_bytes;
	*key_errors = (uint32_t)gem_port_counters.key_errors;

	ret = PON_ADAPTER_SUCCESS;
out:
	pthread_mutex_unlock(&ctx->lock);
	return ret;
}

const struct pa_gem_port_net_ctp_pmhd_ops pon_pa_gem_port_net_ctp_pmhd_ops = {
	.cnt_get = gem_port_net_ctp_cnt_get,
};

static enum pon_adapter_errno management_ds_cnt(
	void *ll_handle, struct pa_management_ds_cnt *props)
{
	struct pon_ploam_ds_counters ploam_counters = {
		0,
	};
	struct pon_xgtc_counters gtx_counters = {
		0,
	};
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode err;
	enum pon_adapter_errno ret;

	pthread_mutex_lock(&ctx->lock);
	err = fapi_pon_ploam_ds_counters_get(pon_ctx, &ploam_counters);
	if (err) {
		ret = pon_fapi_to_pa_error(err);
		goto out;
	}
	err = fapi_pon_xgtc_counters_get(pon_ctx, &gtx_counters);
	if (err) {
		ret = pon_fapi_to_pa_error(err);
		goto out;
	}

	props->ploam_mic_errors = gtx_counters.ploam_mic_err;
	props->all_ds = ploam_counters.all;
	props->profile = ploam_counters.burst_profile;
	props->ranging_time = ploam_counters.ranging_time;
	props->deactivate_onu = ploam_counters.deact_onu;
	props->disable_serial_number = ploam_counters.disable_ser_no;
	props->request_registration = ploam_counters.req_reg;
	props->assign_allocid = ploam_counters.assign_alloc_id;
	props->key_control = ploam_counters.key_control;
	props->sleep_allow = ploam_counters.sleep_allow;
	props->assign_onu_id = ploam_counters.assign_onu_id;

	ret = PON_ADAPTER_SUCCESS;
out:
	pthread_mutex_unlock(&ctx->lock);
	return ret;
}

static enum pon_adapter_errno management_us_cnt(
	void *ll_handle, struct pa_management_us_cnt *props)
{
	struct pon_ploam_us_counters ploam_counters = {
		0,
	};
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode err;

	pthread_mutex_lock(&ctx->lock);
	err = fapi_pon_ploam_us_counters_get(pon_ctx, &ploam_counters);
	pthread_mutex_unlock(&ctx->lock);
	if (err)
		return pon_fapi_to_pa_error(err);

	props->all_us = ploam_counters.all;
	props->serial_number = ploam_counters.ser_no;
	props->registration = ploam_counters.reg;
	props->key_report = ploam_counters.key_rep;
	props->acknowledge = ploam_counters.ack;
	props->sleep_request = ploam_counters.sleep_req;

	return PON_ADAPTER_SUCCESS;
}

const struct pa_management_cnt_ops pon_pa_mngmt_cnt_ops = {
	.management_ds_cnt = management_ds_cnt,
	.management_us_cnt = management_us_cnt,
};

/** Get XGEM TC counters */
static enum pon_adapter_errno enhanced_tc_counters_get(
	void *ll_handle, struct pa_enhanced_tc_counters *props)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode err;
	enum pon_adapter_errno ret;
	struct pon_xgtc_counters xgtc_cnt = {
		0,
	};
	struct pon_gem_port_counters gem_port_cnt = {
		0,
	};

	if (!ctx)
		return PON_ADAPTER_ERR_INVALID_VAL;

	pthread_mutex_lock(&ctx->lock);
	err = fapi_pon_xgtc_counters_get(ctx->pon_ctx, &xgtc_cnt);
	if (err) {
		ret = pon_fapi_to_pa_error(err);
		goto out;
	}
	err = fapi_pon_gem_all_counters_get(ctx->pon_ctx, &gem_port_cnt);
	if (err) {
		ret = pon_fapi_to_pa_error(err);
		goto out;
	}

	props->psbd_hec_err_uncorr = xgtc_cnt.psbd_hec_err_uncorr;
	props->fs_hec_err_uncorr = xgtc_cnt.fs_hec_err_uncorr;
	props->burst_profile_err = xgtc_cnt.burst_profile_err;
	props->tx_frames = gem_port_cnt.tx_frames;
	props->tx_fragments = gem_port_cnt.tx_fragments;
	props->lost_words = xgtc_cnt.lost_words;
	props->ploam_mic_err = xgtc_cnt.ploam_mic_err;
	props->key_errors = gem_port_cnt.key_errors;
	props->xgem_hec_err_uncorr = xgtc_cnt.xgem_hec_err_uncorr;
	props->tx_bytes = gem_port_cnt.tx_bytes;
	props->rx_bytes = gem_port_cnt.rx_bytes;
	props->rx_frames = gem_port_cnt.rx_frames;
	props->rx_fragments = gem_port_cnt.rx_fragments;

	ret = PON_ADAPTER_SUCCESS;
out:
	pthread_mutex_unlock(&ctx->lock);
	return ret;
}

/** Get XGPON/XGSPON PHY/LODS */
static enum pon_adapter_errno
enhanced_tc_lods_counters_get(void *ll_handle,
			      struct pa_enhanced_tc_counters *props)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode err;
	enum pon_adapter_errno ret;
	struct pon_xgspon_lods_counters lods_cnt = {0};

	if (!ctx)
		return PON_ADAPTER_ERR_INVALID_VAL;

	pthread_mutex_lock(&ctx->lock);
	err = fapi_pon_xgspon_lods_counters_get(ctx->pon_ctx, &lods_cnt);
	pthread_mutex_unlock(&ctx->lock);

	if (err) {
		ret = pon_fapi_to_pa_error(err);
		goto out;
	}

	props->lods = lods_cnt.lods_events_all;
	props->lods_rest = lods_cnt.lods_restored_oper;
	props->lods_react = lods_cnt.lods_reactivation;
	ret = PON_ADAPTER_SUCCESS;
out:
	return ret;
}

const struct pa_enhanced_tc_pmhd_ops pon_pa_enhanced_tc_pmhd_ops = {
	.enhanced_tc_counters_get = enhanced_tc_counters_get,
	.lods_counters_get = enhanced_tc_lods_counters_get,
};

/** Get XGEM PMHD */
static enum pon_adapter_errno
twdm_channel_xgem_pmhd_get(void *ll_handle, uint16_t me_id,
			   struct pa_twdm_channel_xgem_pmhd *props)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode err;
	enum pon_adapter_errno ret;
	uint8_t dswlch_id = me_id & 0xFF;
	struct pon_twdm_xgtc_counters xgtc_cnt = {0};
	struct pon_gem_port_counters gem_port_cnt = {0};

	if (!ctx)
		return PON_ADAPTER_ERR_INVALID_VAL;

	pthread_mutex_lock(&ctx->lock);
	err = fapi_pon_twdm_xgtc_counters_get(ctx->pon_ctx,
					      dswlch_id, &xgtc_cnt);
	if (err) {
		ret = pon_fapi_to_pa_error(err);
		goto out;
	}

	err = fapi_pon_twdm_xgem_all_counters_get(ctx->pon_ctx,
						  dswlch_id, &gem_port_cnt);
	if (err) {
		ret = pon_fapi_to_pa_error(err);
		goto out;
	}

	props->t_tx_frames = gem_port_cnt.tx_frames;
	props->t_tx_fragments = gem_port_cnt.tx_fragments;
	props->t_rx_frames = gem_port_cnt.rx_frames;
	props->rx_frames_hdr_hec_err = xgtc_cnt.xgem_hec_err_uncorr;
	props->fs_words_lost_hdr_hec_err = xgtc_cnt.lost_words;
	props->encrypt_key_err = gem_port_cnt.key_errors;
	props->t_tx_bytes_non_idle_frames = gem_port_cnt.tx_bytes;
	props->t_rx_bytes_non_idle_frames = gem_port_cnt.rx_bytes;

	ret = PON_ADAPTER_SUCCESS;
out:
	pthread_mutex_unlock(&ctx->lock);
	return ret;
}

const struct pa_twdm_channel_xgem_pmhd_ops pon_pa_twdm_channel_xgem_pmhd_ops = {
	.cnt_get = twdm_channel_xgem_pmhd_get,
};

/** Get TWDM PHY/LODS PMHD */
static enum pon_adapter_errno
twdm_channel_phy_lods_pmhd_get(void *ll_handle, uint16_t me_id,
			       struct pa_twdm_channel_phy_lods_pmhd *props)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode err;
	enum pon_adapter_errno ret;
	uint8_t dswlch_id = me_id & 0xFF;
	struct pon_twdm_xgtc_counters xgtc_cnt = {0};

	if (!ctx)
		return PON_ADAPTER_ERR_INVALID_VAL;

	pthread_mutex_lock(&ctx->lock);
	err = fapi_pon_twdm_xgtc_counters_get(ctx->pon_ctx,
					      dswlch_id, &xgtc_cnt);
	pthread_mutex_unlock(&ctx->lock);
	if (err) {
		ret = pon_fapi_to_pa_error(err);
		goto out;
	}

	props->total_words = xgtc_cnt.words;
	props->bip32_errors = xgtc_cnt.bip_errors;
	props->psbd_hec_err_corr = xgtc_cnt.psbd_hec_err_corr;
	props->psbd_hec_err_uncorr = xgtc_cnt.psbd_hec_err_uncorr;
	props->fs_hec_err_corr = xgtc_cnt.fs_hec_err_corr;
	props->fs_hec_err_uncorr = xgtc_cnt.fs_hec_err_uncorr;
	props->lods_events_all = xgtc_cnt.lods_events_all;
	props->lods_restored_oper = xgtc_cnt.lods_restored_oper;
	props->lods_restored_prot = xgtc_cnt.lods_restored_prot;
	props->lods_restored_disc = xgtc_cnt.lods_restored_disc;
	props->lods_reactivation = xgtc_cnt.lods_reactivation;
	props->lods_reactivation_prot = xgtc_cnt.lods_reactivation_prot;
	props->lods_reactivation_disc = xgtc_cnt.lods_reactivation_disc;

	ret = PON_ADAPTER_SUCCESS;
out:
	return ret;
}

const struct pa_twdm_channel_phy_lods_pmhd_ops
pon_pa_twdm_channel_phy_lods_pmhd_ops = {
	.cnt_get = twdm_channel_phy_lods_pmhd_get,
};

/** Get TWDM Channel PLOAM PMHD (Part 1) */
static enum pon_adapter_errno
twdm_channel_ploam_pmhd1_get(void *ll_handle, uint16_t me_id,
			   struct pa_twdm_channel_ploam_pmhd1 *props)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode err;
	enum pon_adapter_errno ret;
	uint8_t dswlch_id = me_id & 0xFF;
	struct pon_twdm_ploam_ds_counters ploam_ds_cnt = {0};

	if (!ctx)
		return PON_ADAPTER_ERR_INVALID_VAL;

	pthread_mutex_lock(&ctx->lock);
	err = fapi_pon_twdm_ploam_ds_counters_get(ctx->pon_ctx,
						  dswlch_id, &ploam_ds_cnt);
	if (err) {
		ret = pon_fapi_to_pa_error(err);
		goto out;
	}

	props->ploam_mic_errors = ploam_ds_cnt.mic_err;
	props->ds_ploam_msg_cnt = ploam_ds_cnt.all;
	props->rng_time_msg_cnt = ploam_ds_cnt.ranging_time;
	props->prot_ctrl_msg_cnt = ploam_ds_cnt.protection;
	props->adj_tx_wl_msg_cnt = ploam_ds_cnt.tx_wavelength;
	props->adj_tx_wl_adj_amplitude = 0; /* not supported by HW */

	ret = PON_ADAPTER_SUCCESS;
out:
	pthread_mutex_unlock(&ctx->lock);

	return ret;
}

/** Get TWDM Channel PLOAM PMHD (Part 2) */
static enum pon_adapter_errno
twdm_channel_ploam_pmhd2_get(void *ll_handle, uint16_t me_id,
			   struct pa_twdm_channel_ploam_pmhd2 *props)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode err;
	enum pon_adapter_errno ret;
	uint8_t dswlch_id = me_id & 0xFF;
	struct pon_twdm_ploam_ds_counters ploam_ds_cnt = {0};

	if (!ctx)
		return PON_ADAPTER_ERR_INVALID_VAL;

	pthread_mutex_lock(&ctx->lock);
	err = fapi_pon_twdm_ploam_ds_counters_get(ctx->pon_ctx,
						  dswlch_id, &ploam_ds_cnt);
	if (err) {
		ret = pon_fapi_to_pa_error(err);
		goto out;
	}

	props->sys_profile_msg_cnt = ploam_ds_cnt.system_profile;
	props->ch_profile_msg_cnt = ploam_ds_cnt.channel_profile;
	props->burst_profile_msg_cnt = ploam_ds_cnt.burst_profile;
	props->ass_onu_msg_cnt = ploam_ds_cnt.assign_onu_id;
	props->uns_adj_tx_wl_req = ploam_ds_cnt.tx_wavelength_err;
	props->deact_onu_msg_cnt = ploam_ds_cnt.deact_onu;
	props->dis_serial_msg_cnt = ploam_ds_cnt.disable_ser_no;
	props->req_reg_msg_cnt = ploam_ds_cnt.req_reg;
	props->ass_alloc_id_msg_cnt = ploam_ds_cnt.assign_alloc_id;
	props->key_ctrl_msg_cnt = ploam_ds_cnt.key_control;
	props->slp_allow_msg_cnt = ploam_ds_cnt.sleep_allow;
	props->tune_req_msg_cnt = ploam_ds_cnt.tuning_request;
	props->tune_compl_msg_cnt = ploam_ds_cnt.tuning_complete;
	props->calib_req_msg_cnt = ploam_ds_cnt.cal_req;

	ret = PON_ADAPTER_SUCCESS;
out:
	pthread_mutex_unlock(&ctx->lock);

	return ret;
}

/** Get TWDM Channel PLOAM PMHD (Part 3) */
static enum pon_adapter_errno
twdm_channel_ploam_pmhd3_get(void *ll_handle, uint16_t me_id,
			   struct pa_twdm_channel_ploam_pmhd3 *props)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode err;
	enum pon_adapter_errno ret;
	uint8_t dswlch_id = me_id & 0xFF;
	struct pon_twdm_ploam_us_counters ploam_us_cnt = {0};

	if (!ctx)
		return PON_ADAPTER_ERR_INVALID_VAL;

	pthread_mutex_lock(&ctx->lock);
	err = fapi_pon_twdm_ploam_us_counters_get(ctx->pon_ctx,
						  dswlch_id, &ploam_us_cnt);
	if (err) {
		ret = pon_fapi_to_pa_error(err);
		goto out;
	}

	props->us_ploam_msg_cnt = ploam_us_cnt.all;
	props->ser_onu_inb_msg_cnt = 0;		/* not supported */
	props->ser_onu_amcc_msg_cnt = 0;	/* not supported */
	props->reg_msg_cnt = ploam_us_cnt.reg;
	props->key_rep_msg_cnt = ploam_us_cnt.key_rep;
	props->ack_msg_cnt = ploam_us_cnt.ack;
	props->sleep_req_msg_cnt = ploam_us_cnt.sleep_req;
	props->tune_resp_an_msg_cnt = ploam_us_cnt.tuning_resp_ack_nack;
	props->tune_resp_crb_msg_cnt =
			ploam_us_cnt.tuning_resp_complete_rollback;
	props->pwr_cons_msg_cnt = ploam_us_cnt.power_rep;
	props->cpl_err_param_err = ploam_us_cnt.cpl_err;

	ret = PON_ADAPTER_SUCCESS;
out:
	pthread_mutex_unlock(&ctx->lock);

	return ret;
}

const struct pa_twdm_channel_ploam_pmhd_ops
		pon_pa_twdm_channel_ploam_pmhd_ops = {
	.cnt_get_pmhd1 = twdm_channel_ploam_pmhd1_get,
	.cnt_get_pmhd2 = twdm_channel_ploam_pmhd2_get,
	.cnt_get_pmhd3 = twdm_channel_ploam_pmhd3_get,
};

/** Get TWDM Channel Tuning PMHD (Part 1) */
static enum pon_adapter_errno
twdm_channel_tuning_pmhd1_get(void *ll_handle, uint16_t me_id,
			      struct pa_twdm_channel_tuning_pmhd1 *props)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode err;
	enum pon_adapter_errno ret;
	uint8_t dswlch_id = me_id & 0xFF;
	struct pon_twdm_tuning_counters tuning_cnt = {0};

	if (!ctx)
		return PON_ADAPTER_ERR_INVALID_VAL;

	pthread_mutex_lock(&ctx->lock);
	err = fapi_pon_twdm_tuning_counters_get(ctx->pon_ctx,
						dswlch_id, &tuning_cnt);
	if (err) {
		ret = pon_fapi_to_pa_error(err);
		goto out;
	}

	props->tcr_rx = tuning_cnt.counters[0];
	props->tcr_tx = tuning_cnt.counters[1];
	props->tcr_rej_int_sfc = tuning_cnt.counters[2];
	props->tcr_rej_ds = tuning_cnt.counters[3];
	props->tcr_rej_us = tuning_cnt.counters[10];
	props->tcr_ful_reac = tuning_cnt.counters[18];
	props->tcr_fail_tar_not_fnd = tuning_cnt.counters[19];
	props->tcr_fail_tar_no_fb = tuning_cnt.counters[20];
	props->tcr_res_reac_disc = 0; /* n.a. */
	/* tuning_cnt.counters[21]; not used */
	props->tcr_rb_com_ds = tuning_cnt.counters[22];
	props->tcr_rb_ds = tuning_cnt.counters[23];
	props->tcr_rb_us = tuning_cnt.counters[26];
	props->tcr_fail_reac = tuning_cnt.counters[33];

	ret = PON_ADAPTER_SUCCESS;
out:
	pthread_mutex_unlock(&ctx->lock);

	return ret;
}

/** Get TWDM Channel Tuning PMHD (Part 2) */
static enum pon_adapter_errno
twdm_channel_tuning_pmhd2_get(void *ll_handle, uint16_t me_id,
			      struct pa_twdm_channel_tuning_pmhd2 *props)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode err;
	enum pon_adapter_errno ret;
	uint8_t dswlch_id = me_id & 0xFF;
	struct pon_twdm_tuning_counters tuning_cnt = {0};

	if (!ctx)
		return PON_ADAPTER_ERR_INVALID_VAL;

	pthread_mutex_lock(&ctx->lock);
	err = fapi_pon_twdm_tuning_counters_get(ctx->pon_ctx,
						dswlch_id, &tuning_cnt);
	if (err) {
		ret = pon_fapi_to_pa_error(err);
		goto out;
	}

	props->tcr_rej_ds_albl = tuning_cnt.counters[4];
	props->tcr_rej_ds_void = tuning_cnt.counters[5];
	props->tcr_rej_ds_part = tuning_cnt.counters[6];
	props->tcr_rej_ds_tunr = tuning_cnt.counters[7];
	props->tcr_rej_ds_lnrt = tuning_cnt.counters[8];
	props->tcr_rej_ds_lncd = tuning_cnt.counters[9];
	props->tcr_rej_us_albl = tuning_cnt.counters[11];
	props->tcr_rej_us_void = tuning_cnt.counters[12];
	props->tcr_rej_us_tunr = tuning_cnt.counters[13];
	props->tcr_rej_us_clbr = tuning_cnt.counters[14];
	props->tcr_rej_us_lktp = tuning_cnt.counters[15];
	props->tcr_rej_us_lnrt = tuning_cnt.counters[16];
	props->tcr_rej_us_lncd = tuning_cnt.counters[17];

	ret = PON_ADAPTER_SUCCESS;
out:
	pthread_mutex_unlock(&ctx->lock);

	return ret;
}

/** Get TWDM Channel Tuning PMHD (Part 3) */
static enum pon_adapter_errno
twdm_channel_tuning_pmhd3_get(void *ll_handle, uint16_t me_id,
			      struct pa_twdm_channel_tuning_pmhd3 *props)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode err;
	enum pon_adapter_errno ret;
	uint8_t dswlch_id = me_id & 0xFF;
	struct pon_twdm_tuning_counters tuning_cnt = {0};

	if (!ctx)
		return PON_ADAPTER_ERR_INVALID_VAL;

	pthread_mutex_lock(&ctx->lock);
	err = fapi_pon_twdm_tuning_counters_get(ctx->pon_ctx,
						dswlch_id, &tuning_cnt);
	if (err) {
		ret = pon_fapi_to_pa_error(err);
		goto out;
	}

	props->tcr_rb_ds_albl = tuning_cnt.counters[24];
	props->tcr_rb_ds_lktp = tuning_cnt.counters[25];
	props->tcr_rb_us_albl = tuning_cnt.counters[27];
	props->tcr_rb_us_void = tuning_cnt.counters[28];
	props->tcr_rb_us_tunr = tuning_cnt.counters[29];
	props->tcr_rb_us_lktp = tuning_cnt.counters[30];
	props->tcr_rb_us_lnrt = tuning_cnt.counters[31];
	props->tcr_rb_us_lncd = tuning_cnt.counters[32];

	ret = PON_ADAPTER_SUCCESS;
out:
	pthread_mutex_unlock(&ctx->lock);

	return ret;
}

const struct pa_twdm_channel_tuning_pmhd_ops
		pon_pa_twdm_channel_tuning_pmhd_ops = {
	.cnt_get_pmhd1 = twdm_channel_tuning_pmhd1_get,
	.cnt_get_pmhd2 = twdm_channel_tuning_pmhd2_get,
	.cnt_get_pmhd3 = twdm_channel_tuning_pmhd3_get,
};

/** @} */
