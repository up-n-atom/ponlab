/*****************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
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
#include "pon_net_mac_bridge_port_config_data.h"
#include <omci/me/pon_adapter_mac_bridge_service_profile.h>

static enum pon_adapter_errno
bridge_netdev_create_force(struct pon_net_context *ctx,
			   const char *ifname)
{
	enum pon_adapter_errno ret;
	static const struct netlink_bridge bridge_params = {
		.mask = NETLINK_BRIDGE_vlan_filtering |
			NETLINK_BRIDGE_vlan_stats_enabled |
			NETLINK_BRIDGE_vlan_protocol,
		.vlan_filtering = true,
		.vlan_stats_enabled = true,
		.vlan_protocol = ETH_P_8021Q,
	};

	dbg_in_args("%p, \"%s\"", ctx, ifname);

	netlink_netdevice_destroy(ctx->netlink, ifname);
	/* Only pass bridge parameters if vlan_forwarding option is disabled. */
	ret = netlink_bridge_create(ctx->netlink,
				    ifname,
				    ctx->cfg.vlan_forwarding ?
					NULL : &bridge_params);
	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno init(void *ll_handle, const uint16_t me_id)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE] = "";
	int len;

	dbg_in_args("%p, %u", ll_handle, me_id);

	len = snprintf(ifname, sizeof(ifname), PON_IFNAME_BRIDGE, me_id);
	if (len >= (int)sizeof(ifname)) {
		FN_ERR_RET(len, snprintf, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	ret = mapper_explicit_map(ctx->mapper[MAPPER_MACBRIDGE_MEID_TO_IDX],
				  me_id,
				  me_id);

	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(mapper_explicit_map, ret);
		goto err_mapper;
	}

	ret = bridge_netdev_create_force(ctx, ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(bridge_netdev_create_force, ret);
		goto err_bridge;
	}

	ret = netlink_netdevice_state_set(ctx->netlink, ifname, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_netdevice_state_set, ret);
		goto err_set_up;
	}

	dbg_out_ret("%d", ret);
	return ret;

err_set_up:
	netlink_netdevice_destroy(ctx->netlink, ifname);
err_bridge:
	mapper_id_remove(ctx->mapper[MAPPER_MACBRIDGE_MEID_TO_IDX], me_id);
err_mapper:
	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno update(void *ll_handle,
	     const uint16_t me_id,
	     const struct pa_mac_bridge_service_profile_upd_data *update_data)
{
	dbg_in_args("%p, %u, %p", ll_handle, me_id, update_data);

	/* TODO: Implement this function */

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* This checks if the given ME is from the Multicast bridge and deletes the
 * reference in the mc_br_ifindex attribute if this is the multicast ME.
 * Other code now knows that the Multicast bridge is gone.
 *
 * Errors are only logged to not prevent the removing of the device.
 */
#ifdef MCC_DRV_ENABLE
static void remove_mc_bridge_idx(struct pon_net_context *ctx,
				 const uint16_t me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char br_ifname[IF_NAMESIZE];
	int br_ifindex;

	ret = pon_net_bridge_ifname_get(ctx, me_id, br_ifname,
					sizeof(br_ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_bridge_ifname_get, ret);
		return;
	}

	br_ifindex = netlink_netdevice_ifindex_get(ctx->netlink, br_ifname);
	if (!br_ifindex) {
		dbg_err_fn(netlink_netdevice_ifindex_get);
		return;
	}
	if (ctx->mc_br_ifindex == br_ifindex)
		ctx->mc_br_ifindex = 0;
}
#else
static void remove_mc_bridge_idx(struct pon_net_context *ctx,
				 const uint16_t me_id)
{
	(void)ctx;
	(void)me_id;
}
#endif /* MCC_DRV_ENABLE */

/* Remove VLAN forwarding */
static void cleanup_vlan_forwarding(struct pon_net_context *ctx, uint16_t me_id)
{
	enum pon_adapter_errno ret;
	unsigned int len;
	uint16_t *me_ids;

	dbg_in_args("%p, %u", ctx, me_id);

	/* Get number of bridge ports connected to this bridge */
	len = pon_net_bp_all_get(ctx, me_id, NULL, 0);

	if (!len || len > BRIDGE_PORT_MAX)
		goto err_len;

	/* Get bridge port ME Ids connected to this bridge */
	me_ids = calloc(len, sizeof(*me_ids));
	if (!me_ids) {
		dbg_err_fn_ret(calloc, 0);
		goto err_malloc;
	}
	if (pon_net_bp_all_get(ctx, me_id, me_ids, len) != len) {
		dbg_err("unexpected return value from pon_net_bp_all_get()");
		goto err_bp_all_get;
	}

	/* Clean up the VLAN forwarding for this bridge */
	ret = pon_net_bp_vlan_forwarding_cleanup(ctx, me_ids, len);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_bp_vlan_forwarding_cleanup, ret);
		goto err_bp_vlan_forwarding_cleanup;
	}

err_bp_vlan_forwarding_cleanup:
err_bp_all_get:
	free(me_ids);
err_malloc:
err_len:
	dbg_out();
}

static enum pon_adapter_errno destroy(void *ll_handle, const uint16_t me_id)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret;
	char ifname[IF_NAMESIZE] = "";
	int len;

	dbg_in_args("%p, %u", ll_handle, me_id);

	len = snprintf(ifname, sizeof(ifname), PON_IFNAME_BRIDGE, me_id);
	if (len >= (int)sizeof(ifname)) {
		FN_ERR_RET(len, snprintf, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	remove_mc_bridge_idx(ctx, me_id);

	cleanup_vlan_forwarding(ctx, me_id);
	netlink_netdevice_destroy(ctx->netlink, ifname);
	mapper_id_remove(ctx->mapper[MAPPER_MACBRIDGE_MEID_TO_IDX], me_id);

	/*
	 * This may be the "main" bridge for some multicast GEM, therefore
	 * we must reconnect the multicast GEMs so that the "main" bridge
	 * for them can be updated.
	 */
	ret = pon_net_bp_reconnect_multicast_gems(ctx);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_bp_reconnect_multicast_gems, ret);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno port_count_get(void *ll_handle,
					     const uint16_t me_id,
					     uint8_t *port_count)
{
	dbg_in_args("%p, %u, %p", ll_handle, me_id, port_count);

	/* TODO: Implement this function */
	*port_count = 0;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pa_mac_bridge_service_profile_ops
mac_bridge_service_profile_ops = {
	.init = init,
	.update = update,
	.destroy = destroy,
	.port_count_get = port_count_get
};
