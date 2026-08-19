/*****************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <pon_adapter.h>
#include <omci/me/pon_adapter_onu_g.h>

#include "../pon_net_common.h"
#include "../pon_net_debug.h"
#include "pon_net_onu_g.h"

struct onu_g {
	uint8_t battery_backup;
	uint8_t traff_mgmt_opt;
};

/** Check if the 'option' is a valid traffic management option */
static bool traffic_management_option_is_valid(uint8_t option)
{
	switch (option) {
	case PA_TMO_PRIO:
	case PA_TMO_RATE:
	case PA_TMO_PRIO_RATE:
		return true;
	default:
		return false;
	}
}

static const uint8_t queue_lookup_modes[] = {
	[PA_TMO_PRIO] = NETLINK_QUEUE_LOOKUP_SUBIF_ID,
	[PA_TMO_RATE] = NETLINK_QUEUE_LOOKUP_SUBIF_ID_TC_3BIT,
	[PA_TMO_PRIO_RATE] = NETLINK_QUEUE_LOOKUP_SUBIF_ID,
};

/* Sets queue lookup mode in the ethernet driver */
static enum pon_adapter_errno set_queue_lookup_mode(struct pon_net_context *ctx,
						    uint8_t tmo)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u", ctx, tmo);

	ret = netlink_set_queue_lookup_mode(ctx->netlink,
					    queue_lookup_modes[tmo]);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_set_queue_lookup_mode, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

#define DEFAULT_TRAFF_MGMT_OPT PA_TMO_PRIO

/** Returns traffic management option value set on the ONU-G */
uint8_t pon_net_traffic_management_option_get(struct pon_net_context *ctx)
{
	struct onu_g onu_g = { 0 };
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p", ctx);

	ret = pon_net_me_list_read(&ctx->me_list, PON_CLASS_ID_ONU_G, 0, &onu_g,
				   sizeof(onu_g));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%u", DEFAULT_TRAFF_MGMT_OPT);
		return DEFAULT_TRAFF_MGMT_OPT;
	}

	dbg_out_ret("%u", onu_g.traff_mgmt_opt);
	return onu_g.traff_mgmt_opt;
}

/** Gets called when ONU-G is created */
static enum pon_adapter_errno create(void *ll_handle, uint16_t me_id,
				     uint8_t battery_backup,
				     uint8_t traff_mgmt_opt)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct onu_g onu_g = { 0 };

	dbg_in_args("%p, %u, %u, %u", ll_handle, me_id, battery_backup,
		    traff_mgmt_opt);

	if (!traffic_management_option_is_valid(traff_mgmt_opt)) {
		dbg_err("Invalid traffic management option: %u\n",
			traff_mgmt_opt);
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	onu_g.battery_backup = battery_backup;
	onu_g.traff_mgmt_opt = traff_mgmt_opt;

	ret = set_queue_lookup_mode(ctx, traff_mgmt_opt);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, set_queue_lookup_mode, ret);
		return ret;
	}

	ret = pon_net_me_list_write(&ctx->me_list, PON_CLASS_ID_ONU_G, me_id,
				    &onu_g, sizeof(onu_g));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_write, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/** Gets called when ONU-G is destroyed */
static enum pon_adapter_errno destroy(void *ll_handle, uint16_t me_id)
{
	struct pon_net_context *ctx = ll_handle;

	dbg_in_args("%p, %u", ll_handle, me_id);

	pon_net_me_list_remove(&ctx->me_list, PON_CLASS_ID_ONU_G, me_id);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/** Gets called when ONU-G is updated */
static enum pon_adapter_errno update(void *ll_handle, uint16_t me_id,
				     uint8_t battery_backup,
				     uint8_t traff_mgmt_opt)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct onu_g onu_g = { 0 };

	dbg_in_args("%p, %u, %u, %u", ll_handle, me_id, battery_backup,
		    traff_mgmt_opt);

	if (!traffic_management_option_is_valid(traff_mgmt_opt)) {
		dbg_err("Invalid traffic management option: %u\n",
			traff_mgmt_opt);
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	pon_net_me_list_read(&ctx->me_list, PON_CLASS_ID_ONU_G, me_id, &onu_g,
			     sizeof(onu_g));

	onu_g.battery_backup = battery_backup;
	onu_g.traff_mgmt_opt = traff_mgmt_opt;

	ret = set_queue_lookup_mode(ctx, traff_mgmt_opt);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, set_queue_lookup_mode, ret);
		return ret;
	}

	ret = pon_net_me_list_write(&ctx->me_list, PON_CLASS_ID_ONU_G, me_id,
				    &onu_g, sizeof(onu_g));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_write, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pa_onu_g_ops pon_net_onu_g_ops = {
	.create = create,
	.update = update,
	.destroy = destroy,
};
