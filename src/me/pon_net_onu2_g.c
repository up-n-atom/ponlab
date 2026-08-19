/*****************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <pon_adapter.h>

#include "../pon_net_common.h"
#include "../pon_net_debug.h"
#include "pon_net_onu2_g.h"
#include <omci/me/pon_adapter_onu2_g.h>

struct onu2_g {
	uint16_t priority_queue_scale_factor;
};

static enum pon_adapter_errno
update(void *ll_handle, uint16_t me_id, struct pa_onu2_g_update *update_data)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_context *ctx = ll_handle;
	struct onu2_g onu2_g = {
		.priority_queue_scale_factor =
			update_data->priority_queue_scale_factor,
	};

	dbg_in_args("%p, %u, %p", ll_handle, me_id, update_data);

	ret = pon_net_me_list_write(&ctx->me_list, PON_CLASS_ID_ONU2_G, me_id,
				    &onu2_g, sizeof(onu2_g));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_write, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

#define DEFAULT_SCALE_FACTOR 1
uint16_t pon_net_priority_queue_scale_factor_get(struct pon_net_context *ctx)
{
	struct onu2_g onu2_g = {0};
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p", ctx);

	ret = pon_net_me_list_read(&ctx->me_list, PON_CLASS_ID_ONU2_G, 0,
				   &onu2_g, sizeof(onu2_g));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%u", DEFAULT_SCALE_FACTOR);
		return DEFAULT_SCALE_FACTOR;
	}

	dbg_out_ret("%u", onu2_g.priority_queue_scale_factor);
	return onu2_g.priority_queue_scale_factor;
}

static enum pon_adapter_errno destroy(void *ll_handle, uint16_t me_id)
{
	struct pon_net_context *ctx = ll_handle;

	dbg_in_args("%p, %u", ll_handle, me_id);

	pon_net_me_list_remove(&ctx->me_list, PON_CLASS_ID_ONU2_G, me_id);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pa_onu2_g_ops pon_net_onu2_g_ops = {
	.update = update,
	.destroy = destroy,
};
