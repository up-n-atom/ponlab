/*****************************************************************************
 *
 * Copyright (c) 2020 MaxLinear, Inc.
 * Copyright (c) 2018 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <netlink/route/link.h>
#include <netlink/route/qdisc.h>
#include <pon_adapter.h>

#include "../pon_net_common.h"
#include "../pon_net_debug.h"
#include "../pon_net_netlink.h"
#include <omci/me/pon_adapter_traffic_scheduler.h>

static enum pon_adapter_errno create(void *ll_handle, uint16_t me_id,
	const struct pa_traffic_scheduler_update_data *update_data)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_traffic_scheduler ts = {
		.me_id = me_id,
		.tcont_ptr = update_data->tcont_ptr,
		.traffic_scheduler_ptr = update_data->traffic_scheduler_ptr,
		.policy = update_data->policy,
		.priority_weight = update_data->priority_weight
	};

	dbg_in_args("%p, %u, %p", ll_handle, me_id, update_data);

	ret = pon_net_me_list_write(&ctx->me_list,
				    PON_CLASS_ID_TRAFFIC_SCHEDULER,
				    me_id, &ts, sizeof(ts));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_write, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno update(void *ll_handle, uint16_t me_id,
	const struct pa_traffic_scheduler_update_data *update_data)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_traffic_scheduler ts = {
		.me_id = me_id,
		.tcont_ptr = update_data->tcont_ptr,
		.traffic_scheduler_ptr = update_data->traffic_scheduler_ptr,
		.policy = update_data->policy,
		.priority_weight = update_data->priority_weight
	};

	dbg_in_args("%p, %u, %p", ll_handle, me_id, update_data);

	ret = pon_net_me_list_write(&ctx->me_list,
				    PON_CLASS_ID_TRAFFIC_SCHEDULER,
				    me_id, &ts, sizeof(ts));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_write, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno destroy(void *ll_handle, uint16_t me_id)
{
	struct pon_net_context *ctx = ll_handle;

	dbg_in_args("%p, %u", ll_handle, me_id);

	pon_net_me_list_remove(&ctx->me_list, PON_CLASS_ID_TRAFFIC_SCHEDULER,
			       me_id);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pa_traffic_scheduler_ops traffic_scheduler_ops = {
	.update = update,
	.destroy = destroy,
	.create = create,
};
