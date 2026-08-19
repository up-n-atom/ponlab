/*****************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2018 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <netlink/route/link.h>
#include <pon_adapter.h>

#include "../pon_net_common.h"
#include "../pon_net_debug.h"
#include "../pon_net_netlink.h"
#include "pon_net_gem_port_network_ctp.h"
#include "pon_net_gem_interworking_tp.h"
#include <omci/me/pon_adapter_gem_interworking_tp.h>

enum pon_adapter_errno
pon_net_gem_interworking_tp_ctp_get(struct pon_net_context *ctx, uint16_t me_id,
				    uint16_t *gem_ctp_me_id)
{
	uint32_t id;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %p", ctx, me_id, gem_ctp_me_id);

	ret = mapper_index_get(ctx->mapper[MAPPER_GEMPORTITP_MEID_TO_ID],
			       me_id, &id);

	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	ret = mapper_id_get(ctx->mapper[MAPPER_GEMPORTCTP_MEID_TO_ID],
			    &id, id);

	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	if (gem_ctp_me_id)
		*gem_ctp_me_id = (uint16_t)id;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
gem_itp_update(void *ll_handle,
	uint16_t me_id,
	uint16_t gem_port_id, struct pa_bridge_data *bridge,
	uint16_t max_gem_payload_size,
	const struct pa_gem_interworking_tp_update_data *upd_data)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint16_t gem_ctp_meid;
	struct netlink_gem gem;
	uint32_t idx;

	dbg_in_args("%p, %u, %u, %p, %u, %p", ll_handle, me_id, gem_port_id,
		    bridge, max_gem_payload_size, upd_data);

	netlink_gem_defaults(&gem);

	/* It only makes sense to create GEM Interworking TP if corresponding
	   GEM Port CTP exists */
	ret = pon_net_pa_mapper_id_get_uint16(
		ctx->mapper[MAPPER_GEMPORTCTP_MEID_TO_ID],
		&gem_ctp_meid, gem_port_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_pa_mapper_id_get_uint16,
			   PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	ret = mapper_index_get(ctx->mapper[MAPPER_GEMPORTITP_MEID_TO_ID],
			       me_id, &idx);
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		/* ME is being created */
		ret = mapper_explicit_map(
				ctx->mapper[MAPPER_GEMPORTITP_MEID_TO_ID],
				me_id, gem_port_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, mapper_explicit_map, PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}
	}


	/* The interface is enabled only if a valid interworking option has
	   been assigned to the GEM Interworking TP.  Valid options are
	   bridged LAN, 802.1p mapper, and downstream broadcast.  */
	if (upd_data->interworking_option == PA_GEM_ITP_MAC_BRIDGE_LAN) {

		/* In case of bridge LAN we should check if service profile
		   ptr points to MAC Bridge Service Profile ME */
		ret =
		    mapper_index_get(ctx->mapper[MAPPER_MACBRIDGE_MEID_TO_IDX],
				     upd_data->service_profile_pointer,
				     &idx);
		if (ret != PON_ADAPTER_SUCCESS)
			netlink_gem_state_set(ctx->netlink, gem_ctp_meid, 0);
		else
			netlink_gem_state_set(ctx->netlink, gem_ctp_meid, 1);
	} else if (upd_data->interworking_option == PA_GEM_ITP_P_MAPPER) {
		netlink_gem_state_set(ctx->netlink, gem_ctp_meid, 1);
	} else if (upd_data->interworking_option ==
		   PA_GEM_ITP_DOWNSTREAM_BROADCAST) {
		netlink_gem_state_set(ctx->netlink, gem_ctp_meid, 1);
	} else {
		netlink_gem_state_set(ctx->netlink, gem_ctp_meid, 0);
	}

	ret = pon_net_gem_max_payload_size_set(ctx, gem_ctp_meid,
					       max_gem_payload_size);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_gem_max_payload_size_set, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno gem_itp_destroy(void *ll_handle,
					      uint16_t me_id,
					      uint16_t gem_port_id,
					      uint8_t interworking_option,
					      uint16_t service_profile_pointer)
{
	struct pon_net_context *ctx = ll_handle;

	dbg_in_args("%p, %u, %u, %u, %u", ll_handle, me_id, gem_port_id,
		    interworking_option, service_profile_pointer);

	mapper_id_remove(ctx->mapper[MAPPER_GEMPORTITP_MEID_TO_ID], me_id);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pa_gem_interworking_tp_ops gem_itp_ops = {
	.update = gem_itp_update,
	.destroy = gem_itp_destroy
};
