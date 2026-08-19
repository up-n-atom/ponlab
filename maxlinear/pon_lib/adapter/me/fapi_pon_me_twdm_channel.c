/*****************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include "pon_adapter.h"
#include "pon_adapter_system.h"
#include "omci/me/pon_adapter_twdm_channel.h"

#include "../fapi_pon_pa_common.h"
#include "fapi_pon.h"

#define MAX_CHANNEL_ID 15

static enum pon_adapter_errno is_ch_active_get(void *ll_handle,
					       uint8_t wlch_id,
					       bool *is_ch_active)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode pon_ret = PON_STATUS_OK;
	struct pon_twdm_channel_profile twdm_channel_profile;
	enum pon_adapter_errno ret;
	uint8_t i;

	*is_ch_active = false;

	pthread_mutex_lock(&ctx->lock);

	for (i = 0; i <= MAX_CHANNEL_ID; i++) {
		pon_ret = fapi_pon_twdm_channel_profile_status_get(ctx->pon_ctx,
						i, &twdm_channel_profile);
		if (pon_ret != PON_STATUS_OK)
			continue;
		if (twdm_channel_profile.ds_valid &&
		    twdm_channel_profile.us_valid &&
		    twdm_channel_profile.dswlch_id == wlch_id &&
		    twdm_channel_profile.uswlch_id == wlch_id) {
			*is_ch_active = true;
			ret = PON_ADAPTER_SUCCESS;
			goto out;
		}
	}
	ret = PON_ADAPTER_SUCCESS;
out:
	pthread_mutex_unlock(&ctx->lock);
	return ret;
}

static enum pon_adapter_errno operational_ch_get(void *ll_handle,
						 uint8_t *ds_ch_index,
						 uint8_t *us_ch_index)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode pon_ret = PON_STATUS_OK;
	struct pon_twdm_status twdm_status;

	pthread_mutex_lock(&ctx->lock);
	pon_ret = fapi_pon_twdm_status_get(ctx->pon_ctx, &twdm_status);
	pthread_mutex_unlock(&ctx->lock);
	if (pon_ret != PON_STATUS_OK)
		return PON_ADAPTER_ERROR;

	*ds_ch_index = twdm_status.ds_ch_index;
	*us_ch_index = twdm_status.us_ch_index;
	return PON_ADAPTER_SUCCESS;
}

const struct pa_twdm_channel_ops pon_pa_twdm_channel_ops = {
	.is_ch_active_get = is_ch_active_get,
	.operational_ch_get = operational_ch_get,
};
