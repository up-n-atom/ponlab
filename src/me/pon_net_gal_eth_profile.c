/*****************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include "pon_adapter.h"
#include "omci/me/pon_adapter_gal_ethernet_profile.h"

#include "../pon_net_common.h"
#include "../pon_net_debug.h"
#include "pon_net_gem_port_network_ctp.h"

static enum pon_adapter_errno update(void *ll_handle, uint16_t me_id,
				     uint16_t max_gem_payload_size,
				     uint16_t gem_port_id)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint16_t gem_ctp_meid;
	struct netlink_gem gem;

	dbg_in_args("%p, %u, %u, %u", ll_handle, me_id, max_gem_payload_size,
		    gem_port_id);

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

	ret = pon_net_gem_max_payload_size_set(ctx, gem_ctp_meid,
					       max_gem_payload_size);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_gem_max_payload_size_set, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno destroy(void *ll_handle, uint16_t me_id)
{
	dbg_in_args("%p, %u", ll_handle, me_id);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pa_gal_eth_profile_ops gal_eth_profile_ops = {
	.update = update,
	.destroy = destroy,
};

/** @} */
