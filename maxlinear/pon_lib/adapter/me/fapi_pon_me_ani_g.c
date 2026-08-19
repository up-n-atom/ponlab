/*****************************************************************************
 *
 * Copyright (c) 2022 - 2026 MaxLinear, Inc.
 * Copyright (c) 2017 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include "pon_adapter.h"
#include "pon_adapter_system.h"
#include "omci/me/pon_adapter_ani_g.h"

#include "../fapi_pon_pa_common.h"
#include "fapi_pon.h"

/*
 * PON Adapter wrappers and structures
 */

static enum pon_adapter_errno
dba_mode_get(void *ll_handle, uint16_t me_id, uint8_t *dba_mode)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode pon_ret;
	struct pon_cap caps;

	UNUSED(me_id);

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_cap_get(pon_ctx, &caps);
	pthread_mutex_unlock(&ctx->lock);
	if (pon_ret != PON_STATUS_OK) {
		dbg_err_fn_ret(fapi_pon_cap_get, pon_ret);
		return pon_fapi_to_pa_error(pon_ret);
	}

	/* Only one bit mode can be active at the same time */
	if (caps.features & PON_FEATURE_DBAM1)
		*dba_mode = PA_ANIG_DBA_PB_MODE_0_1;
	else if (caps.features & PON_FEATURE_DBAM0)
		*dba_mode = PA_ANIG_DBA_PB_MODE_0_ONLY;
	else
		*dba_mode = PA_ANIG_DBA_PB_MODE_NOT_SUPPORTED;
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
sr_indication_get(void *ll_handle, uint16_t me_id, uint8_t *indication)
{
	UNUSED(me_id);
	UNUSED(ll_handle);

	*indication = 1;

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
total_tcon_num_get(void *ll_handle, uint16_t me_id, uint16_t *num)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode pon_ret;
	struct pon_cap caps;

	UNUSED(me_id);

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_cap_get(pon_ctx, &caps);
	pthread_mutex_unlock(&ctx->lock);
	if (pon_ret != PON_STATUS_OK) {
		dbg_err_fn_ret(fapi_pon_cap_get, pon_ret);
		return pon_fapi_to_pa_error(pon_ret);
	}

	*num = caps.alloc_ids;
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
response_time_get(void *ll_handle, uint16_t me_id, uint16_t *ont_response_time)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode pon_ret;
	struct pon_gpon_status gpon_status;

	UNUSED(me_id);

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_gpon_status_get(pon_ctx, &gpon_status);
	pthread_mutex_unlock(&ctx->lock);
	if (pon_ret != PON_STATUS_OK) {
		dbg_err_fn_ret(fapi_pon_gpon_status_get, pon_ret);
		return pon_fapi_to_pa_error(pon_ret);
	}

	*ont_response_time = gpon_status.onu_resp_time;
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno onu_id_get(void *ll_handle, uint32_t *onu_id)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode pon_ret;
	struct pon_gpon_status gpon_status;

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_gpon_status_get(pon_ctx, &gpon_status);
	pthread_mutex_unlock(&ctx->lock);
	if (pon_ret != PON_STATUS_OK) {
		dbg_err_fn_ret(fapi_pon_gpon_status_get, pon_ret);
		return pon_fapi_to_pa_error(pon_ret);
	}

	*onu_id = gpon_status.onu_id;
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
supply_voltage_get(void *ll_handle, uint16_t me_id, uint16_t *voltage)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode pon_ret;
	struct pon_optic_status optic_status;

	UNUSED(me_id);

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_optic_status_get(pon_ctx, &optic_status,
					ctx->cfg.optic.tx_power_scale);
	pthread_mutex_unlock(&ctx->lock);
	if (pon_ret != PON_STATUS_OK)
		*voltage = 0;
	else
		/* Unit in OMCI (20mV) is 200 times larger than unit in FAPI
		 * (100uV). To convert FAPI to OMCI value it has to be divided
		 * by 200.
		 */
		*voltage = optic_status.voltage / 200;

	return PON_ADAPTER_SUCCESS;
}

/* Function to get rx power level in dBm units.
 * Value returned by [*level] has 0.002dBm/LSB granularity
 */
static enum pon_adapter_errno
signal_lvl_rx_get(void *ll_handle, uint16_t me_id, int16_t *level)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode pon_ret;
	struct pon_optic_status optic_status;

	UNUSED(me_id);

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_optic_status_get(pon_ctx, &optic_status,
					ctx->cfg.optic.tx_power_scale);
	pthread_mutex_unlock(&ctx->lock);
	if (pon_ret != PON_STATUS_OK)
		*level = DMI_POWER_ZERO;
	else
		*level = optic_status.rx_power;

	return PON_ADAPTER_SUCCESS;
}

/* Function to get rx power level in dBu [dBmicro] units.
 * Value returned by [*level] has 0.002dBu/LSB granularity
 */
static enum pon_adapter_errno
signal_lvl_rx_dbu_get(void *ll_handle, uint16_t me_id, int16_t *level)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode pon_ret;
	struct pon_optic_status optic_status;
	float dbm;
	float dbu;

	UNUSED(me_id);

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_optic_status_get(pon_ctx, &optic_status,
					ctx->cfg.optic.tx_power_scale);
	pthread_mutex_unlock(&ctx->lock);
	if (pon_ret != PON_STATUS_OK) {
		*level = DMI_POWER_ZERO;
	} else {
		/* get dBm from integer 0.002dBm/LSB */
		dbm = optic_status.rx_power / 500.0;
		/* dBm -> dBu */
		dbu = dbm + 30;
		/* division to get required granularity 0.002dBu/LSB */
		*level = dbu / 0.002;
	}
	return PON_ADAPTER_SUCCESS;
}

/* Function to get tx power in dBm units
 * with 0.002dBm/LSB granularity
 */
static enum pon_adapter_errno
signal_lvl_tx_get(void *ll_handle, uint16_t me_id, int16_t *level)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode pon_ret;
	struct pon_optic_status optic_status;

	UNUSED(me_id);

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_optic_status_get(pon_ctx, &optic_status,
					ctx->cfg.optic.tx_power_scale);
	pthread_mutex_unlock(&ctx->lock);
	if (pon_ret != PON_STATUS_OK)
		*level = DMI_POWER_ZERO;
	else
		*level = optic_status.tx_power;

	return PON_ADAPTER_SUCCESS;
}

/* Function to get tx power in dBu [dBmicro] units
 * with 0.002dBu/LSB granularity
 */
static enum pon_adapter_errno
signal_lvl_tx_dbu_get(void *ll_handle, uint16_t me_id, int16_t *level)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode pon_ret;
	struct pon_optic_status optic_status;
	float dbm;
	float dbu;

	UNUSED(me_id);

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_optic_status_get(pon_ctx, &optic_status,
					ctx->cfg.optic.tx_power_scale);
	pthread_mutex_unlock(&ctx->lock);
	if (pon_ret != PON_STATUS_OK) {
		*level = DMI_POWER_ZERO;
	} else {
		/* get dBm from integer 0.002dBm/LSB */
		dbm = optic_status.tx_power / 500.0;
		/* dBm -> dBu */
		dbu = dbm + 30;
		/* division to get required granularity 0.002dBu/LSB */
		*level = dbu / 0.002;

		/* The upper calculation is provided for clear understanding.
		 * Faster calculation proposition:
		 * *level = optic_status.tx_power + 30 * 500;
		 */
	}
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
bias_current_get(void *ll_handle, uint16_t me_id, uint16_t *bias_current)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode pon_ret;
	struct pon_optic_status optic_status;

	UNUSED(me_id);

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_optic_status_get(pon_ctx, &optic_status,
					ctx->cfg.optic.tx_power_scale);
	pthread_mutex_unlock(&ctx->lock);
	if (pon_ret != PON_STATUS_OK)
		*bias_current = 0;
	else
		*bias_current = optic_status.bias;

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
temperature_get(void *ll_handle, uint16_t me_id, int16_t *temperature)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode pon_ret;
	struct pon_optic_status optic_status;

	UNUSED(me_id);

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_optic_status_get(pon_ctx, &optic_status,
					ctx->cfg.optic.tx_power_scale);
	pthread_mutex_unlock(&ctx->lock);
	if (pon_ret != PON_STATUS_OK)
		*temperature = 0;
	else
		*temperature = optic_status.temperature;

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
ani_g_update(void *ll_handle, uint16_t me_id,
	     const struct pon_adapter_ani_g_update_data *update_data)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct fapi_pon_wrapper_cfg *cfg = &ctx->cfg;
	struct fapi_pon_ani_g_data *ani_g_data = &ctx->ani_g_data;
	enum fapi_pon_errorcode pon_ret = PON_STATUS_OK;
	struct pon_gtc_cfg pon_gtc_cfg;
	enum pon_adapter_errno ret;

	UNUSED(me_id);

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_gtc_cfg_get(ctx->pon_ctx, &pon_gtc_cfg);
	if (pon_ret != PON_STATUS_OK) {
		ret = pon_fapi_to_pa_error(pon_ret);
		goto out;
	}

	pon_gtc_cfg.sf_threshold = update_data->sf_threshold;
	pon_gtc_cfg.sd_threshold = update_data->sd_threshold;

	pthread_mutex_lock(&ani_g_data->lock);
	ani_g_data->update_status = true;

	if (update_data->lower_optic_thr == 0xFF)
		ani_g_data->lower_optic_thr =
			cfg->lower_receive_optical_threshold * 500;
	else
		ani_g_data->lower_optic_thr =
			update_data->lower_optic_thr * -250; /* 500 * -0.5 */

	if (update_data->upper_optic_thr == 0xFF)
		ani_g_data->upper_optic_thr =
			cfg->upper_receive_optical_threshold * 500;
	else
		ani_g_data->upper_optic_thr =
			update_data->upper_optic_thr * -250; /* 500 * -0.5 */

	if (update_data->lower_tx_power_thr == 0x81)
		ani_g_data->lower_tx_power_thr =
			cfg->lower_transmit_power_threshold * 500;
	else
		ani_g_data->lower_tx_power_thr =
			(int8_t)update_data->lower_tx_power_thr *
			250; /* 500 * 0.5 */

	if (update_data->upper_tx_power_thr == 0x81)
		ani_g_data->upper_tx_power_thr =
			cfg->upper_transmit_power_threshold * 500;
	else
		ani_g_data->upper_tx_power_thr =
			(int8_t)update_data->upper_tx_power_thr *
			250; /* 500 * 0.5 */

	pthread_mutex_unlock(&ani_g_data->lock);

	/** TODO: Add missing handling of
	 *	- gem_block_len
	 */

	pon_ret = fapi_pon_gtc_cfg_set(ctx->pon_ctx, &pon_gtc_cfg);
	if (pon_ret != PON_STATUS_OK) {
		ret = pon_fapi_to_pa_error(pon_ret);
		goto out;
	}

	ret = PON_ADAPTER_SUCCESS;
out:
	pthread_mutex_unlock(&ctx->lock);
	return ret;
}

enum pon_adapter_errno pon_ani_g_alarm_recheck(struct fapi_pon_wrapper_ctx *ctx)
{
	struct fapi_pon_ani_g_data *ani_g_data = &ctx->ani_g_data;

	pthread_mutex_lock(&ani_g_data->lock);
	ani_g_data->update_status = true;
	pthread_mutex_unlock(&ani_g_data->lock);

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno ani_g_create(void *ll_handle, uint16_t me_id)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum pon_adapter_errno ret;

	UNUSED(me_id);

	pthread_mutex_lock(&ctx->lock);
	ret = pon_pa_ani_g_alarm_check_start(ctx);
	pthread_mutex_unlock(&ctx->lock);
	return ret;
}

static enum pon_adapter_errno ani_g_destroy(void *ll_handle, uint16_t me_id)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum pon_adapter_errno ret;

	UNUSED(me_id);

	pthread_mutex_lock(&ctx->lock);
	ret = pon_pa_ani_g_alarm_check_stop(ctx);
	pthread_mutex_unlock(&ctx->lock);
	return ret;
}

static const struct pon_adapter_ani_g_attr_data_ops attr_data = {
	.dba_mode_get = dba_mode_get,
	.sr_indication_get = sr_indication_get,
	.total_tcon_num_get = total_tcon_num_get,
	.response_time_get = response_time_get,
	.onu_id_get = onu_id_get,
	.supply_voltage_get = supply_voltage_get,
};

static const struct pon_adapter_ani_g_optic_signal_lvl_ops signal_lvl = {
	.rx_get = signal_lvl_rx_get,
	.rx_dbu_get = signal_lvl_rx_dbu_get,
	.tx_get = signal_lvl_tx_get,
	.tx_dbu_get = signal_lvl_tx_dbu_get,
};

static const struct pon_adapter_ani_g_laser_ops laser = {
	.bias_current_get = bias_current_get,
	.temperature_get = temperature_get,
};

const struct pon_adapter_ani_g_ops pon_pa_ani_g_ops = {
	.create = ani_g_create,
	.update = ani_g_update,
	.destroy = ani_g_destroy,
	.attr_data = &attr_data,
	.signal_lvl = &signal_lvl,
	.laser = &laser,
};

/** @} */
