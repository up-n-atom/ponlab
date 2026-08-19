/*****************************************************************************
 *
 * Copyright (c) 2022 - 2024 MaxLinear, Inc.
 * Copyright (c) 2018 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include "pon_adapter.h"
#include "pon_adapter_system.h"
#include "omci/me/pon_adapter_olt_g.h"

#include "../fapi_pon_pa_common.h"
#include "fapi_pon.h"

/** PON OLT Vendor ID identifiers for different vendors */
static const char pon_olt_vendor_id[PON_OLT_LAST + 1][5] = {
	/** OLT type unknown */
	[PON_OLT_UNKNOWN] = {0},
	/** OLT type Adtran (any type and HW/SW version) */
	[PON_OLT_ADTRAN] = "ADTN",
	/** Calix OLT, does not report an identifier */
	[PON_OLT_CALIX] = {0},
	/** OLT type Ciena (any type and HW/SW version) */
	[PON_OLT_CIENA] = "CIEN",
	/** OLT type Dasan/Zhone (any type and HW/SW version) */
	[PON_OLT_DZS] = "DZSI",
	/** OLT type Huawei (any type and HW/SW version) */
	[PON_OLT_HUAWEI] = "HWTC",
	/** OLT type Nokia (any type and HW/SW version) */
	[PON_OLT_NOKIA] = "ALCL",
	/** OLT type Tibit (any type and HW/SW version) */
	[PON_OLT_TIBIT] = "TBIT",
	/** ZTE OLT, does not report an identifier */
	[PON_OLT_ZTE] = {0},
};

static enum olt_type
olt_type_by_vendor_get(struct fapi_pon_wrapper_ctx *ctx,
		       const struct pa_olt_g_update_data *update_data)
{
	unsigned int i;
	enum olt_type ret = PON_OLT_UNKNOWN;
	int result;

	UNUSED(ctx);

	if (!update_data)
		goto out;

	/* OLT Equipment ID, currently unused
	 * update_data->olt_equipment_id;
	 */
	/* OLT Version, currently unused
	 * update_data->olt_version;
	 */

	/* compare the OLT Vendor ID */
	for (i = 0; i < ARRAY_SIZE(pon_olt_vendor_id); i++) {
		int memcmp;

		if (pon_olt_vendor_id[i][0] == 0)
			continue;

		/* olt_vendor_id provided via the PON adapter interface is
		 * according to OMCI definition not a string, but contains
		 * 4 byte values, which but usually represent characters.
		 * Anyways a \0 termination is not given, so we don't use
		 * string operations.
		 * size1 must be >= size2
		 */
		memcmp = memcmp_s(pon_olt_vendor_id[i],
				  sizeof(pon_olt_vendor_id[i]),
				  update_data->olt_vendor_id,
				  sizeof(update_data->olt_vendor_id),
				  &result);
		if (memcmp == EOK && result == 0)
			break;
	}

	if (i >= ARRAY_SIZE(pon_olt_vendor_id))
		goto out;
	else
		ret = i;

out:
	return ret;
}

static enum pon_adapter_errno
olt_type_set(struct fapi_pon_wrapper_ctx *ctx,
	     const struct pa_olt_g_update_data *update_data)
{
	enum fapi_pon_errorcode pon_ret = PON_STATUS_OK;
	enum pon_adapter_errno ret;
	struct pon_olt_type olt_type;

	/* evaluate update_data */
	olt_type.type = olt_type_by_vendor_get(ctx, update_data);

	pon_ret = fapi_pon_olt_type_set(ctx->pon_ctx, &olt_type,
					ctx->cfg.iop_mask);
	if (pon_ret != PON_STATUS_OK) {
		ret = pon_fapi_to_pa_error(pon_ret);
		goto out;
	}

	ret = PON_ADAPTER_SUCCESS;
out:
	return ret;
}

static enum pon_adapter_errno
tod_sync_set(struct fapi_pon_wrapper_ctx *ctx,
	     const struct pa_olt_g_update_data *update_data)
{
	enum fapi_pon_errorcode pon_ret = PON_STATUS_OK;
	enum pon_adapter_errno ret;
	struct pon_gpon_status gpon_status;
	struct pon_gpon_tod_sync gpon_tod_sync;
	double eqd_ps, resp_time_ps, delta_ps, delta_factor, eqd_unit_ps;
	int32_t tod_offset_pico_seconds;
	long delta_ns;

	pon_ret = fapi_pon_gpon_status_get(ctx->pon_ctx, &gpon_status);
	if (pon_ret != PON_STATUS_OK) {
		ret = pon_fapi_to_pa_error(pon_ret);
		goto out;
	}

	/* all bytes 0 is a "null" value, ignore */
	if ((update_data->multiframe_count |
		update_data->tod_extended_seconds |
		update_data->tod_seconds |
		update_data->tod_nano_seconds) == 0) {
		ret = PON_ADAPTER_SUCCESS;
		goto out;
	}

	switch (ctx->cfg.mode) {
	case PON_MODE_984_GPON:
		delta_factor = ctx->cfg.tod_asymm_corr_gpon;
		eqd_unit_ps = 1e6/1244.16;
		tod_offset_pico_seconds = ctx->cfg.tod_offset_pico_seconds_2g5;
		break;
	case PON_MODE_987_XGPON:
	case PON_MODE_9807_XGSPON:
		delta_factor = ctx->cfg.tod_asymm_corr_xgspon;
		eqd_unit_ps = 1e6/2488.32;
		tod_offset_pico_seconds = ctx->cfg.tod_offset_pico_seconds_10g;
		break;
	case PON_MODE_989_NGPON2_2G5:
	case PON_MODE_989_NGPON2_10G:
		delta_factor = ctx->cfg.tod_asymm_corr_ngpon2;
		eqd_unit_ps = 1e6/2488.32;
		tod_offset_pico_seconds = ctx->cfg.tod_offset_pico_seconds_10g;
		break;
	default:
		/* invalid mode */
		ret = PON_ADAPTER_ERR_DRV;
		goto out;
	}

	pon_ret = fapi_pon_gpon_tod_sync_get(ctx->pon_ctx, &gpon_tod_sync);
	if (pon_ret != PON_STATUS_OK) {
		ret = pon_fapi_to_pa_error(pon_ret);
		goto out;
	}

	eqd_ps = gpon_status.eq_del * eqd_unit_ps;
	resp_time_ps = gpon_status.onu_resp_time * 1000.0;
	delta_ps = (eqd_ps + resp_time_ps) * delta_factor +
		tod_offset_pico_seconds;
	delta_ns = (long)(delta_ps / 1000);

	gpon_tod_sync.multiframe_count = update_data->multiframe_count;
	gpon_tod_sync.tod_seconds = update_data->tod_seconds;
	gpon_tod_sync.tod_extended_seconds = update_data->tod_extended_seconds;
	gpon_tod_sync.tod_nano_seconds = update_data->tod_nano_seconds;

	/* subtract delta with handling underflow */
	if (delta_ns <= gpon_tod_sync.tod_nano_seconds) {
		gpon_tod_sync.tod_nano_seconds -= delta_ns;
	} else {
		/* move a second to nano-seconds */
		gpon_tod_sync.tod_nano_seconds +=
			((1000 * 1000 * 1000) - delta_ns);
		if (gpon_tod_sync.tod_seconds >= 1) {
			gpon_tod_sync.tod_seconds -= 1;
		} else {
			gpon_tod_sync.tod_seconds += 0xFFFFFFFF;
			gpon_tod_sync.tod_extended_seconds -= 1;
		}
	}

	pon_ret = fapi_pon_gpon_tod_sync_set(ctx->pon_ctx, &gpon_tod_sync);
	if (pon_ret != PON_STATUS_OK) {
		ret = pon_fapi_to_pa_error(pon_ret);
		goto out;
	}

	ret = PON_ADAPTER_SUCCESS;
out:
	return ret;
}

/*
 * PON Adapter wrappers and structures
 */
static enum pon_adapter_errno
olt_g_update(void *ll_handle,
	     uint16_t me_id,
	     const struct pa_olt_g_update_data *update_data)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum pon_adapter_errno ret;

	UNUSED(me_id);

	pthread_mutex_lock(&ctx->lock);

	/* setting proper interop bits may be essential for further operation,
	 * call it first
	 */
	ret = olt_type_set(ctx, update_data);
	if (ret != PON_ADAPTER_SUCCESS)
		goto out;

	ret = tod_sync_set(ctx, update_data);
	if (ret != PON_ADAPTER_SUCCESS)
		goto out;

	ret = PON_ADAPTER_SUCCESS;
out:
	pthread_mutex_unlock(&ctx->lock);
	return ret;
}

const struct pa_olt_g_ops pon_pa_olt_g_ops = {
	.update = olt_g_update,
};
