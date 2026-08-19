/*****************************************************************************
 *
 * Copyright (c) 2022 - 2023 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include "pon_adapter.h"
#include "pon_adapter_config.h"
#include "pon_adapter_system.h"
#include "omci/me/pon_adapter_twdm_system_profile.h"

#include "../fapi_pon_pa_common.h"
#include "fapi_pon.h"

static enum pon_adapter_errno
ploam_timeouts_set(struct fapi_pon_wrapper_ctx *ctx, struct pon_timeout_cfg *data)
{
	int err, i;
	enum pon_adapter_errno error;
	/* 12 characters are sufficient to represent a positive 32 bit decimal
	 * integer value and the null termination
	 */
	char value_str[12];
	bool commit = false;
	struct {
		uint32_t value;
		char *name;
	} param_names[] = {
		/* only write these to UCI */
		{data->ploam_timeout_2, "ploam_timeout_2"},
		{data->ploam_timeout_3, "ploam_timeout_3"},
		/* don't update  timeout 4/5 in UCI, but use as lower limit */
		{data->ploam_timeout_cpi, "ploam_timeout_cpi"},
	};

	for (i = 0; i < ARRAY_SIZE(param_names); i++) {
		err = sprintf_s(value_str, sizeof(value_str), "%u",
				param_names[i].value);
		if (err < 0) {
			dbg_err("can't buffer %u to string\n",
				param_names[i].value);
			return PON_ADAPTER_ERR_INVALID_VAL;
		}
		/* commit only with the last entry */
		if (i + 1 == ARRAY_SIZE(param_names))
			commit = true;
		error = pon_pa_config_write(ctx, "gpon", "ploam",
					    param_names[i].name, value_str,
					    commit);
		if (error != PON_ADAPTER_SUCCESS)
			return error;
	}

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
ch_part_index_set(struct fapi_pon_wrapper_ctx *ctx, uint8_t ch_part_index)
{
	enum fapi_pon_errorcode ret;
	struct fapi_pon_wrapper_cfg *cfg = &ctx->cfg;
	int err;
	enum pon_adapter_errno error;
	char str_cpi[4];

	/* Allowed range of the Channel Partition Index is 0..15 */
	if (ch_part_index & ~0xF)
		return PON_ADAPTER_ERR_OUT_OF_BOUNDS;

	/* Send the update message to the firmware (TWDM_CONFIG) */
	ret = fapi_pon_twdm_cpi_set(ctx->pon_ctx, ch_part_index);
	if (ret != PON_STATUS_OK)
		return pon_fapi_to_pa_error(ret);

	/* Update the context config cache with the new value of
	 * the Channel Partition Index
	 */
	cfg->twdm.ch_partition_index = ch_part_index;

	err = sprintf_s(str_cpi, sizeof(str_cpi), "%u", ch_part_index);
	if (err < 0) {
		dbg_err("can't convert Channel Partition Index %u to string\n",
			ch_part_index);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	/* Store the Channel Partition Index value in config */
	error = pon_pa_config_write(ctx, "optic", "twdm",
				    "ch_partition_index",
				    str_cpi, true);
	if (error != PON_ADAPTER_SUCCESS)
		return error;

	return PON_ADAPTER_SUCCESS;
}

/*
 * PON Adapter wrappers and structures
 */

static enum pon_adapter_errno
update(void *ll_handle, uint16_t me_id,
		const struct pa_twdm_system_profile_data *update_data)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode pon_ret = PON_STATUS_OK;
	struct pon_timeout_cfg timeout_cfg;
	enum pon_adapter_errno ret;

	UNUSED(me_id);

	pthread_mutex_lock(&ctx->lock);

	pon_ret = fapi_pon_timeout_cfg_get(ctx->pon_ctx, &timeout_cfg);
	if (pon_ret != PON_STATUS_OK) {
		ret = pon_fapi_to_pa_error(pon_ret);
		goto out;
	}

	timeout_cfg.ploam_timeout_cpi = update_data->ch_part_waiv_timer;
	/* TO2 .. TO5 in [1ms] scaling  <-- OMCI timer in [125us] scaling */
	timeout_cfg.ploam_timeout_2 = update_data->lods_reinit_timer / 8;
	timeout_cfg.ploam_timeout_3 = update_data->lods_prot_timer / 8;
	timeout_cfg.ploam_timeout_4 = update_data->ds_tuning_timer / 8;
	timeout_cfg.ploam_timeout_5 = update_data->us_tuning_timer / 8;

	/* US/DS tuning timeouts must not be lowered beyond UCI defaults */
	if (timeout_cfg.ploam_timeout_4 < ctx->cfg.ploam_timeout_4) {
		ret = PON_ADAPTER_ERR_OMCI_ME_ATTR_INVALID;
		goto out;
	}
	if (timeout_cfg.ploam_timeout_5 < ctx->cfg.ploam_timeout_5) {
		ret = PON_ADAPTER_ERR_OMCI_ME_ATTR_INVALID;
		goto out;
	}
	pon_ret = fapi_pon_timeout_cfg_set(ctx->pon_ctx, &timeout_cfg);
	if (pon_ret != PON_STATUS_OK) {
		ret = pon_fapi_to_pa_error(pon_ret);
		goto out;
	}

	ret = ch_part_index_set(ctx, update_data->ch_part_index);
	if (ret != PON_ADAPTER_SUCCESS)
		goto out;

	/* write new values to UCI */
	ret = ploam_timeouts_set(ctx, &timeout_cfg);

out:
	pthread_mutex_unlock(&ctx->lock);
	return ret;
}

static enum pon_adapter_errno
current_data_get(void *ll_handle,
		struct pa_twdm_system_profile_data *current_data)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct fapi_pon_wrapper_cfg *cfg = &ctx->cfg;
	enum fapi_pon_errorcode pon_ret = PON_STATUS_OK;
	struct pon_timeout_cfg timeout_cfg;
	struct pon_gpon_cfg gpon_cfg;
	uint8_t ch_count = 0;
	enum pon_adapter_errno ret;
#ifndef __GNUC__
	uint8_t i;
#endif

	pthread_mutex_lock(&ctx->lock);

	pon_ret = fapi_pon_gpon_cfg_get(ctx->pon_ctx, &gpon_cfg);
	if (pon_ret != PON_STATUS_OK) {
		ret = pon_fapi_to_pa_error(pon_ret);
		goto out;
	}
	if (gpon_cfg.mode != PON_MODE_989_NGPON2_2G5 &&
	    gpon_cfg.mode != PON_MODE_989_NGPON2_10G) {
		ret = PON_ADAPTER_ERR_OMCI_ME_NOT_SUPPORTED;
		goto out;
	}

#ifdef __GNUC__
	ch_count = __builtin_popcount(ctx->cfg.twdm_channel_mask);
#else
	for (i = 0; i < sizeof(ctx->cfg.twdm_channel_mask) * 8; i++) {
		if (ctx->cfg.twdm_channel_mask & (1 << i))
			ch_count++;
	}
#endif

	current_data->total_twdm_ch_num = ch_count;

	/* get CPI value from config */
	current_data->ch_part_index = cfg->twdm.ch_partition_index;

	pon_ret = fapi_pon_timeout_cfg_get(ctx->pon_ctx, &timeout_cfg);
	if (pon_ret != PON_STATUS_OK) {
		ret = pon_fapi_to_pa_error(pon_ret);
		goto out;
	}

	current_data->ch_part_waiv_timer = timeout_cfg.ploam_timeout_cpi;
	/* OMCI timer in [125us] scaling <-- TO2 .. TO5 in [1ms] scaling */
	current_data->lods_reinit_timer =  timeout_cfg.ploam_timeout_2 * 8;
	current_data->lods_prot_timer =    timeout_cfg.ploam_timeout_3 * 8;
	current_data->ds_tuning_timer =    timeout_cfg.ploam_timeout_4 * 8;
	current_data->us_tuning_timer =    timeout_cfg.ploam_timeout_5 * 8;

	ret = PON_ADAPTER_SUCCESS;
out:
	pthread_mutex_unlock(&ctx->lock);
	return ret;
}

const struct pa_twdm_system_profile_ops pon_pa_twdm_system_profile_ops = {
	.update =           update,
	.current_data_get = current_data_get,
};
