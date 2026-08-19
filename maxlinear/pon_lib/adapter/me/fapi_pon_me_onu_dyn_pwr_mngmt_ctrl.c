/*****************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 * Copyright (c) 2017 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include "pon_adapter.h"
#include "pon_adapter_system.h"
#include "omci/me/pon_adapter_onu_dyn_pwr_mngmt_ctrl.h"

#include "../fapi_pon_pa_common.h"
#include "fapi_pon.h"

static uint32_t features_to_pa_mode(uint32_t features)
{
	uint32_t pa_mode = 0;

	if (features & PON_FEATURE_WSLP)
		pa_mode |=
		      PA_ONU_DYN_PWR_MNGMT_CTRL_CYCLIC_WATCHFUL_SLEEP_MODE_MASK;

	if (features & PON_FEATURE_CSLP)
		pa_mode |= PA_ONU_DYN_PWR_MNGMT_CTRL_CYCLIC_SLEEP_MODE_MASK;

	if (features & PON_FEATURE_DOZE)
		pa_mode |= PA_ONU_DYN_PWR_MNGMT_CTRL_DOZE_MODE_MASK;

	return pa_mode;
}

static enum pon_adapter_errno select_fapi_psm_mode(uint8_t onu_supported_modes,
						   uint8_t olt_selected_modes,
						   uint8_t *chosen_mode)
{
	uint8_t mode = onu_supported_modes & olt_selected_modes;

	if (olt_selected_modes == 0) {
		*chosen_mode = 0;
		return PON_ADAPTER_SUCCESS;
	}

	if (mode & PA_ONU_DYN_PWR_MNGMT_CTRL_CYCLIC_WATCHFUL_SLEEP_MODE_MASK) {
		*chosen_mode = PON_PSM_CONFIG_MODE_WSL;
		return PON_ADAPTER_SUCCESS;
	}

	if (mode & PA_ONU_DYN_PWR_MNGMT_CTRL_CYCLIC_SLEEP_MODE_MASK) {
		*chosen_mode = PON_PSM_CONFIG_MODE_CSL;
		return PON_ADAPTER_SUCCESS;
	}

	if (mode & PA_ONU_DYN_PWR_MNGMT_CTRL_DOZE_MODE_MASK) {
		*chosen_mode = PON_PSM_CONFIG_MODE_DOZE;
		return PON_ADAPTER_SUCCESS;
	}

	return PON_ADAPTER_ERR_NOT_SUPPORTED;
}

/* Returns a mask of supported PSM modes (value) */
static enum pon_adapter_errno
capabilities_get(struct fapi_pon_wrapper_ctx *ctx, uint8_t *value)
{
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode pon_ret = PON_STATUS_OK;
	struct pon_cap pon_cap;

	if (!value)
		return PON_ADAPTER_ERR_DRV;

	/* Try to deduce which PSM modes are available from PON mode */
	switch (ctx->cfg.mode) {
	case PON_MODE_9807_XGSPON:
	case PON_MODE_989_NGPON2_2G5:
	case PON_MODE_989_NGPON2_10G:
		*value =
	   PA_ONU_DYN_PWR_MNGMT_CTRL_CYCLIC_WATCHFUL_SLEEP_MODE_MASK;
		return PON_ADAPTER_SUCCESS;
	case PON_MODE_987_XGPON:
	case PON_MODE_984_GPON:
		*value = PA_ONU_DYN_PWR_MNGMT_CTRL_DOZE_MODE_MASK |
		  PA_ONU_DYN_PWR_MNGMT_CTRL_CYCLIC_SLEEP_MODE_MASK |
	   PA_ONU_DYN_PWR_MNGMT_CTRL_CYCLIC_WATCHFUL_SLEEP_MODE_MASK;
		return PON_ADAPTER_SUCCESS;
	default:
		/* fallback to fapi_pon_cap_get() */
		break;
	}

	/* fallback to fapi_pon_cap_get() if previous method failed */
	pon_ret = fapi_pon_cap_get(pon_ctx, &pon_cap);
	if (pon_ret != PON_STATUS_OK) {
		dbg_err_fn_ret(fapi_pon_cap_get, pon_ret);
		return pon_fapi_to_pa_error(pon_ret);
	}

	*value = features_to_pa_mode(pon_cap.features);

	return PON_ADAPTER_SUCCESS;
}

/*
 * PON Adapter wrappers and structures
 */

static enum pon_adapter_errno pwr_reduction_mngmt_cap_get(void *ll_handle,
							  uint16_t me_id,
							  uint8_t *value)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum pon_adapter_errno ret;

	UNUSED(me_id);

	pthread_mutex_lock(&ctx->lock);
	ret = capabilities_get(ctx, value);
	pthread_mutex_unlock(&ctx->lock);
	return ret;
}

static enum pon_adapter_errno itransinit_get(void *ll_handle,
					     uint16_t me_id,
					     uint16_t *value)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode pon_ret = PON_STATUS_OK;
	struct pon_cap pon_cap;

	UNUSED(me_id);

	if (!value)
		return PON_ADAPTER_ERR_DRV;

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_cap_get(ctx->pon_ctx, &pon_cap);
	pthread_mutex_unlock(&ctx->lock);

	if (pon_ret != PON_STATUS_OK) {
		dbg_err_fn_ret(fapi_pon_cap_get, pon_ret);
		return pon_fapi_to_pa_error(pon_ret);
	}

	*value = (uint16_t)pon_cap.itransinit;

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno itxinit_get(void *ll_handle,
					  uint16_t me_id,
					  uint16_t *value)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode pon_ret = PON_STATUS_OK;
	struct pon_cap pon_cap;

	UNUSED(me_id);

	if (!value)
		return PON_ADAPTER_ERR_DRV;

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_cap_get(ctx->pon_ctx, &pon_cap);
	pthread_mutex_unlock(&ctx->lock);

	if (pon_ret != PON_STATUS_OK) {
		dbg_err_fn_ret(fapi_pon_cap_get, pon_ret);
		return pon_fapi_to_pa_error(pon_ret);
	}

	*value = (uint16_t)pon_cap.itxinit;

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
onu_dyn_pwr_mngmt_ctrl_update(void *ll_handle, uint16_t me_id,
	     const struct pa_onu_dyn_pwr_mngmt_ctrl_update_data *data)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode pon_ret = PON_STATUS_OK;
	enum pon_adapter_errno ret;
	struct pon_psm_cfg pon_psm_cfg = { 0 };
	struct pon_psm_cfg old_pon_psm_cfg = { 0 };
	uint8_t onu_supported_modes;
	uint8_t olt_selected_modes;
	uint8_t chosen_mode = 0;

	UNUSED(me_id);

	pthread_mutex_lock(&ctx->lock);

	fapi_pon_psm_cfg_get(ctx->pon_ctx, &old_pon_psm_cfg);

	ret = capabilities_get(ctx, &onu_supported_modes);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(capabilities_get, ret);
		goto out;
	}

	olt_selected_modes = data->pwr_reduction_mngmt_mode;

	ret = select_fapi_psm_mode(onu_supported_modes,
				   olt_selected_modes,
				   &chosen_mode);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(select_fapi_psm_mode, ret);
		goto out;
	}

	pon_psm_cfg.enable = chosen_mode ? 1 : 0;
	pon_psm_cfg.mode = chosen_mode;
	pon_psm_cfg.min_aware_interval = data->min_aware_interval;
	pon_psm_cfg.min_active_held_interval = data->min_active_held_interval;

	if (chosen_mode == PON_PSM_CONFIG_MODE_DOZE || chosen_mode == 0)
		pon_psm_cfg.max_doze_interval =
			data->max_sleep_interval_doze_ext;

	if (chosen_mode == PON_PSM_CONFIG_MODE_CSL || chosen_mode == 0)
		pon_psm_cfg.max_rx_off_interval =
			data->max_sleep_interval_cyclic_sleep_ext;

	if (chosen_mode == PON_PSM_CONFIG_MODE_WSL || chosen_mode == 0)
		pon_psm_cfg.max_sleep_interval = data->max_sleep_interval;

	pon_ret = fapi_pon_psm_cfg_set(ctx->pon_ctx, &pon_psm_cfg);
	if (pon_ret != PON_STATUS_OK) {
		dbg_err("fapi_pon_psm_cfg_set() failed with %i. Attempting to revert changes...",
			pon_ret);
		pon_ret = fapi_pon_psm_cfg_set(ctx->pon_ctx, &old_pon_psm_cfg);
		if (pon_ret == PON_STATUS_OK)
			dbg_err("ok\n");
		else
			dbg_err("failed with %i\n", pon_ret);

		ret = PON_ADAPTER_ERR_DRV;
		goto out;
	}

	ret = PON_ADAPTER_SUCCESS;
out:
	pthread_mutex_unlock(&ctx->lock);
	return ret;
}

static const struct pa_onu_dyn_pwr_mngmt_ctrl_attr_data_ops attr_data = {
	.pwr_reduction_mngmt_cap_get = pwr_reduction_mngmt_cap_get,
	.itransinit_get = itransinit_get,
	.itxinit_get = itxinit_get,
};

const struct pa_onu_dyn_pwr_mngmt_ctrl_ops pon_pa_onu_dyn_pwr_mngmt_ctrl_ops = {
	.update = onu_dyn_pwr_mngmt_ctrl_update,
	.attr_data = &attr_data
};
