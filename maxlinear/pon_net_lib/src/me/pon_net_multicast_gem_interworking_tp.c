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
#include "pon_net_multicast_gem_interworking_tp.h"
#include <omci/me/pon_adapter_multicast_gem_interworking_tp.h>

/* Data for Multicast GEM Port Interworking TP ME */
struct pon_net_multicast_gem_itp {
	/* GEM Port Network CTP ME ID */
	uint16_t gem_ctp_me_id;
};

enum pon_adapter_errno
pon_net_multicast_gem_interworking_tp_ctp_get(struct pon_net_context *ctx,
					      uint16_t me_id,
					      uint16_t *gem_ctp_me_id)
{
	struct pon_net_multicast_gem_itp *mc_gem;

	dbg_in_args("%p, %u, %p", ctx, me_id, gem_ctp_me_id);

	mc_gem = pon_net_me_list_get_data(
	    &ctx->me_list, PON_CLASS_ID_MULTICAST_GEM_ITP, me_id);
	if (!mc_gem) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	if (gem_ctp_me_id)
		*gem_ctp_me_id = mc_gem->gem_ctp_me_id;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Add Multicast GEM Port Network ITP to the ME list */
static enum pon_adapter_errno
create_me_list_entry(struct pon_net_me_list *me_list, uint16_t me_id,
		     uint16_t ctp_ptr)
{
	enum pon_adapter_errno ret;
	struct pon_net_multicast_gem_itp mc_gem = {
		.gem_ctp_me_id = ctp_ptr,
	};

	dbg_in_args("%p, %u, %u", me_list, me_id, ctp_ptr);

	ret =
	    pon_net_me_list_write(me_list, PON_CLASS_ID_MULTICAST_GEM_ITP,
				  me_id, &mc_gem, sizeof(mc_gem));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_write, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno mc_gem_itp_update(void *ll_handle,
						uint16_t me_id,
						uint16_t ctp_ptr)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct netlink_gem gem;

	dbg_in_args("%p, %u, %u", ll_handle, me_id, ctp_ptr);

	/*
	 * If the Multicast GEM Port Interworking TP has already been created,
	 * then do nothing
	 */
	if (pon_net_me_list_exists(&ctx->me_list,
				   PON_CLASS_ID_MULTICAST_GEM_ITP, me_id)) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	netlink_gem_defaults(&gem);
	gem.mc = true;

	ret = netlink_gem_update(ctx->netlink, ctp_ptr, &gem);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_gem_update, ret);
		return ret;
	}

	ret = create_me_list_entry(&ctx->me_list, me_id, ctp_ptr);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, create_me_list_entry, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno mc_gem_itp_destroy(void *ll_handle,
						 uint16_t me_id)
{
	struct pon_net_context *ctx = ll_handle;

	dbg_in_args("%p, %u", ll_handle, me_id);

	pon_net_me_list_remove(&ctx->me_list, PON_CLASS_ID_MULTICAST_GEM_ITP,
			       me_id);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pa_mc_gem_itp_ops mc_gem_itp_ops = {
	.update = mc_gem_itp_update,
	.destroy = mc_gem_itp_destroy
};
