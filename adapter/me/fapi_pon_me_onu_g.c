/*****************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 * Copyright (c) 2018 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include "pon_adapter.h"
#include "pon_adapter_system.h"
#include "omci/me/pon_adapter_onu_g.h"

#include "../fapi_pon_pa_common.h"
#include "fapi_pon.h"

/*
 * PON Adapter wrappers and structures
 */

static enum pon_adapter_errno
oper_state_get(void *ll_handle, uint8_t *state)
{
	UNUSED(ll_handle);

	if (state == NULL)
		return PON_ADAPTER_ERR_PTR_INVALID;

	*state = PON_ADAPTER_OPER_STATE_ENABLED;

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
ext_tc_opt_get(void *ll_handle, uint16_t *bitmask)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode ret;
	struct pon_cap caps;

	if (bitmask == NULL)
		return PON_ADAPTER_ERR_PTR_INVALID;

	pthread_mutex_lock(&ctx->lock);
	ret = fapi_pon_cap_get(pon_ctx, &caps);
	pthread_mutex_unlock(&ctx->lock);
	if (ret != PON_STATUS_OK) {
		dbg_err_fn_ret(fapi_pon_cap_get, ret);
		return pon_fapi_to_pa_error(ret);
	}

	*bitmask = (caps.features & PON_FEATURE_ANXC ?
					TC_LAYER_OPTIONS_BIT_1 : 0) |
		   (caps.features & PON_FEATURE_ANXD ?
					TC_LAYER_OPTIONS_BIT_2 : 0);

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
update(void *ll_handle, uint16_t me_id,
	uint8_t battery_backup, uint8_t traff_mgmt_opt)
{
	UNUSED(ll_handle);
	UNUSED(me_id);
	UNUSED(battery_backup);
	UNUSED(traff_mgmt_opt);

	return PON_ADAPTER_SUCCESS;
}

const struct pa_onu_g_ops pon_pa_onu_g_ops = {
	.oper_state_get = oper_state_get,
	.ext_tc_opt_get = ext_tc_opt_get,
	.update = update,
};

/** @} */
