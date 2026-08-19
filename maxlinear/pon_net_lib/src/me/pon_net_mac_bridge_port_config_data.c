/*****************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/** \addtogroup PON_NET_MAC_BRIDGE_PORT_CONFIG_DATA
 *
 *  @{
 */

/**
 * \file
 * This file implements the functionality of connecting network devices
 * to bridges.
 *
 * Whenever the higher layer creates a MAC Bridge Port Configuration Data ME,
 * the implementation in this file will connect a network device that
 * corresponds to the termination point that the MAC Bridge Port Configuration
 * Data ME points to.
 *
 * The connection to the bridge is performed using `RT_NETLINK` messages, send
 * using \ref netlink_netdevice_enslave() function. The equivalent iproute
 * command to this function is:
 *
 *	ip link set $dev master $br
 *
 * Where `$dev` is the termination point and `$br` is the bridge.
 *
 * Every termination point type requires dedicated handling. The struct \ref
 * termination_point contains function pointers and attributes, which are
 * specific to the handling of particular termination point type.
 */

#include <stdio.h>
#include <net/if.h>

#include <netlink/route/link.h>
#include <pon_adapter.h>

#include "../pon_net_netlink.h"
#include "../pon_net_common.h"
#include "../pon_net_debug.h"
#include "pon_net_gem_port_network_ctp.h"
#include "pon_net_gem_interworking_tp.h"
#include "pon_net_mcc_core.h"
#include "pon_net_dev_db.h"
#include "pon_net_dev_defs.h"
#include "pon_net_vlan_flow.h"
#include "pon_net_vlan_aware_bridging.h"
#include "pon_net_ext_vlan.h"
#include "pon_net_mac_bridge_port_config_data.h"
#include "pon_net_multicast_gem_interworking_tp.h"
#include "pon_net_traffic_descriptor.h"
#include "pon_net_vlan_tagging_filter_data.h"
#include "pon_net_mac_bridge_port_filter_preassign_table.h"
#include <omci/me/pon_adapter_mac_bridge_port_config_data.h>
#include <omci/me/pon_adapter_gem_port_network_ctp.h>
#include <omci/me/pon_adapter_ext_vlan.h>

#ifdef MCC_DRV_ENABLE
#include "pon_mcc.h"
#include "pon_mcc_ikm.h"
#endif

/* For given MAC Bridge Port Configuration Data me_id it returns
   The TP Type and TP Ptr of associated termination point */
enum pon_adapter_errno pon_net_tp_get(struct pon_net_context *ctx,
				      const uint16_t me_id, uint8_t *tp_type,
				      uint16_t *tp_ptr)
{
	enum pon_adapter_errno ret;
	struct pon_net_bridge_port_config bp = {0};

	dbg_in_args("%p, %u, %p, %p", ctx, me_id, tp_type, tp_ptr);

	ret = pon_net_me_list_read(&ctx->me_list,
				   PON_CLASS_ID_BRIDGE_PORT_CONFIG, me_id,
				   &bp, sizeof(bp));
	if (ret == PON_ADAPTER_SUCCESS) {
		*tp_type = bp.tp_type;
		*tp_ptr = bp.tp_ptr;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno pon_net_bp_ifname_get(struct pon_net_context *ctx,
					     uint16_t me_id,
					     char *ifname,
					     size_t size)
{
	struct pon_net_bridge_port_config *bp;
	int len;

	dbg_in_args("%p, %u, %p, %zu", ctx, me_id, ifname, size);

	bp = pon_net_me_list_get_data(&ctx->me_list,
				   PON_CLASS_ID_BRIDGE_PORT_CONFIG, me_id);
	if (!bp) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	len = snprintf(ifname, size, "%s", bp->ifname);
	if (len >= (int)size) {
		FN_ERR_RET(len, snprintf, PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
reconnect(struct pon_net_context *ctx,
	  bool (*should_reconnect)(struct pon_net_context *ctx,
				  const struct pon_net_bridge_port_config *bp));

/* Set learning limit value on given bridge port */
static enum pon_adapter_errno
bport_learning_limit_set(struct pon_net_context *ctx,
			 const struct pon_net_bridge_port_config *bp_config)
{
	char ifname[IF_NAMESIZE];
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", ctx, bp_config);

	ret = pon_net_tp_ifname_get(ctx, bp_config->tp_type, bp_config->tp_ptr,
				    ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_tp_ifname_get, ret);
		return ret;
	}

	ret = netlink_bport_learning_limit_set(ctx->netlink, ifname,
					       bp_config->mac_learning_depth);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_bport_learning_limit_set, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
tp_uni_create(struct pon_net_context *ctx,
	      struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", ctx, bp);

	ret = pon_net_tp_ifname_get(ctx, bp->tp_type, bp->tp_ptr, bp->ifname,
				    sizeof(bp->ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_tp_ifname_get, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
tp_uni_update(struct pon_net_context *ctx,
	      struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", ctx, bp);

	ret = bport_learning_limit_set(ctx, bp);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, bport_learning_limit_set, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

uint8_t pon_net_gem_dir_get(struct pon_net_context *ctx,
			    uint16_t tp_ptr)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_gem_port_net_ctp gem_ctp = {0};
	uint32_t id;

	dbg_in_args("%p, %d", ctx, tp_ptr);

	ret = mapper_index_get(ctx->mapper[MAPPER_GEMPORTITP_MEID_TO_ID],
			       tp_ptr, &id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, mapper_index_get, 0);
		return 0;
	}

	ret = mapper_id_get(ctx->mapper[MAPPER_GEMPORTCTP_MEID_TO_ID],
			    &id, id);

	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, mapper_id_get, 0);
		return 0;
	}

	ret = pon_net_me_list_read(&ctx->me_list,
				   PON_CLASS_ID_GEM_PORT_NET_CTP_DATA,
				   (uint16_t) id,
				   &gem_ctp,
				   sizeof(gem_ctp));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_read, 0);
		return 0;
	}

	dbg_out_ret("%d", gem_ctp.dir);
	return gem_ctp.dir;
}

static bool multicast_gems(struct pon_net_context *ctx,
			   const struct pon_net_bridge_port_config *bp);

/*
 * Callback for reconnect() that will cause it to reconnect only broadcast gems
 */
static bool broadcast_gems(struct pon_net_context *ctx,
			   const struct pon_net_bridge_port_config *bp)
{
	uint16_t tp_ptr = bp->tp_ptr;

	return bp->tp_type == PA_BP_TP_TYPE_GEM &&
	pon_net_gem_dir_get(ctx, tp_ptr) == PA_GEMPORTNETCTP_DIR_DOWNSTREAM;
}

static enum pon_adapter_errno
tp_uni_destroy(struct pon_net_context *ctx,
	       struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", ctx, bp);

	ret = reconnect(ctx, multicast_gems);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, reconnect, ret);
		return ret;
	}

	ret = reconnect(ctx, broadcast_gems);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, reconnect, ret);
		return ret;
	}

	/*
	 * Refresh VLAN aware forwarding for all bridge ports
	 *
	 * We need to call this to trigger vlan forwarding update
	 * on P-Mapper bridge port, because it uses UNI connected
	 * to a bridge (in case of VLAN flow forwarding).
	 */
	ret = pon_net_bp_vlan_forwarding_update(ctx, NULL, 0);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_bp_vlan_forwarding_update, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
tp_uni_connect(struct pon_net_context *ctx,
	       struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", ctx, bp);

	/*
	 * Refresh VLAN aware forwarding for all bridge ports
	 *
	 * We need to call this to trigger vlan forwarding update
	 * on P-Mapper bridge port, because it uses UNI connected
	 * to a bridge (in case of VLAN flow forwarding).
	 */
	ret = pon_net_bp_vlan_forwarding_update(ctx, NULL, 0);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_bp_vlan_forwarding_update, ret);
		return ret;
	}

	ret = netlink_netdevice_enslave(ctx->netlink, bp->br_ifname,
					bp->ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_netdevice_enslave, ret);
		return ret;
	}

	ret = bport_learning_limit_set(ctx, bp);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, bport_learning_limit_set, ret);
		return ret;
	}

	ret = reconnect(ctx, multicast_gems);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, reconnect, ret);
		return ret;
	}

	ret = reconnect(ctx, broadcast_gems);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, reconnect, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
tp_uni_reconnect(struct pon_net_context *ctx,
	       struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", ctx, bp);

	ret = netlink_netdevice_enslave(ctx->netlink, bp->br_ifname,
					bp->ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_netdevice_enslave, ret);
		return ret;
	}

	ret = bport_learning_limit_set(ctx, bp);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, bport_learning_limit_set, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
tp_uni_disconnect(struct pon_net_context *ctx,
		  struct pon_net_bridge_port_config *bp,
		  bool permanent)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p, %d", ctx, bp, permanent);

	ret = pon_net_bp_vlan_forwarding_cleanup(ctx, &bp->me_id, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_bp_vlan_forwarding_cleanup, ret);
		return ret;
	}

	ret = netlink_netdevice_enslave(ctx->netlink, "", bp->ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_netdevice_enslave, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
tp_gem_itp_create(struct pon_net_context *ctx,
		  struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", ctx, bp);

	ret = pon_net_tp_ifname_get(ctx, bp->tp_type, bp->tp_ptr, bp->ifname,
				    sizeof(bp->ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_tp_ifname_get, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
tp_gem_itp_update(struct pon_net_context *ctx,
		  struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", ctx, bp);

	ret = bport_learning_limit_set(ctx, bp);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, bport_learning_limit_set, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
tp_gem_itp_destroy(struct pon_net_context *ctx,
		   struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint8_t gem_dir;

	dbg_in_args("%p, %p", ctx, bp);

	gem_dir = pon_net_gem_dir_get(ctx, bp->tp_ptr);
	if (gem_dir < PA_GEMPORTNETCTP_DIR_UPSTREAM) {
		FN_ERR_RET(ret, get_gem_dir, ret);
		return ret;
	}

	if (gem_dir == PA_GEMPORTNETCTP_DIR_DOWNSTREAM) {
		ret = reconnect(ctx, broadcast_gems);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, reconnect, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Determine a broadcast device name (e.g. eth0_0_3), that is connected to the
 * bridge, instead of a broadcast GEM Port Network CTP, when broadcast GEM
 * sharing between two bridges is configured
 */
static enum pon_adapter_errno
bc_ifname_get(struct pon_net_context *ctx,
	      struct pon_net_bridge_port_config *bc_bp,
	      char *bc_ifname, unsigned int capacity)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	const uint16_t class_id = PON_CLASS_ID_BRIDGE_PORT_CONFIG;
	struct pon_net_me_list_item *item = NULL;
	static const uint8_t tp_type_to_bc[] = {
		[PA_BP_TP_TYPE_PPTP_UNI] = UNI_PPTP_BC,
		[PA_BP_TP_TYPE_VEIP] = UNI_VEIP_BC,
	};
	char ifname[IF_NAMESIZE] = "";

	dbg_in_args("%p, %p, %p, %u", ctx, bc_bp, bc_ifname, capacity);

	snprintf(bc_ifname, capacity, "%s", "");

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		struct pon_net_bridge_port_config *bp = item->data;
		uint16_t uni_idx = 0;

		if (bp->tp_type != PA_BP_TP_TYPE_PPTP_UNI &&
		    bp->tp_type != PA_BP_TP_TYPE_VEIP)
			continue;

		if (bp->bridge_me_id != bc_bp->bridge_me_id)
			continue;

		ret = pon_net_uni_lport_get(bp->tp_ptr, &uni_idx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_uni_lport_get, ret);
			return ret;
		}

		ret = pon_net_uni_ifname_get(ctx, tp_type_to_bc[bp->tp_type],
					     uni_idx, ifname, capacity);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_uni_ifname_get, ret);
			return ret;
		}

		if (strncpy_s(bc_ifname, capacity, ifname, IF_NAMESIZE)) {
			dbg_err_fn(strncpy_s);
			dbg_out_ret("%d", PON_ADAPTER_ERR_NO_DATA);
			return PON_ADAPTER_ERR_NO_DATA;
		}

		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_FOUND);
	return PON_ADAPTER_ERR_NOT_FOUND;
}

/*
 * Change bp->br_ifname to bridge ifname of "first" bridge port.
 * The "first" bridge port is the bridge port, for which bp->first was set to
 * "true". It was set to "true" whenever that bridge port was the only
 * connection of it's device to the bridge or when the previous "first" bridge
 * port was removed.
 */
static enum pon_adapter_errno
use_first_bridge_port_bridge_ifname(struct pon_net_context *ctx,
				    struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	const uint16_t class_id = PON_CLASS_ID_BRIDGE_PORT_CONFIG;
	struct pon_net_me_list_item *item = NULL;

	dbg_in_args("%p, %p", ctx, bp);

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		struct pon_net_bridge_port_config *bp_config = item->data;

		if (bp_config->tp_type != bp->tp_type)
			continue;

		if (bp_config->real_tp_ptr != bp->real_tp_ptr)
			continue;

		if (!bp_config->first)
			continue;

		ret = pon_net_bridge_ifname_get(ctx, bp_config->bridge_me_id,
						bp->br_ifname,
						sizeof(bp->br_ifname));
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_out_ret("%d", ret);
			return ret;
		}

		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = pon_net_bridge_ifname_get(ctx, bp->bridge_me_id, bp->br_ifname,
					sizeof(bp->br_ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
tp_gem_itp_connect(struct pon_net_context *ctx,
		   struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE] = "";
	bool gem_connected = false;
	bool bc_connected = false;
	uint8_t gem_dir;

	dbg_in_args("%p, %p", ctx, bp);

	gem_dir = pon_net_gem_dir_get(ctx, bp->tp_ptr);
	if (gem_dir < PA_GEMPORTNETCTP_DIR_UPSTREAM) {
		FN_ERR_RET(ret, get_gem_dir, ret);
		return ret;
	}

	if (gem_dir == PA_GEMPORTNETCTP_DIR_DOWNSTREAM) {
		ret = use_first_bridge_port_bridge_ifname(ctx, bp);
		if (ret == PON_ADAPTER_ERR_NOT_FOUND)
			return PON_ADAPTER_SUCCESS;
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, use_first_bridge_port_bridge_ifname,
				   ret);
			return ret;
		}

		if (bp->first) {
			ret = pon_net_tp_ifname_get(ctx, bp->tp_type,
						    bp->tp_ptr, ifname,
						    sizeof(ifname));
			if (ret != PON_ADAPTER_SUCCESS) {
				dbg_err_fn_ret(pon_net_tp_ifname_get, ret);
				goto err;
			}

			ret = netlink_netdevice_enslave(ctx->netlink,
							bp->br_ifname,
							ifname);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, netlink_netdevice_enslave, ret);
				return ret;
			}

			ret = bport_learning_limit_set(ctx, bp);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, bport_learning_limit_set, ret);
				return ret;
			}
		} else {
			ret = bc_ifname_get(ctx, bp, bp->ifname,
					    sizeof(bp->ifname));
			if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
				dbg_prn("UNI is not in a bridge, broadcast device will not be connected");
				/* return success, because adding broadcast gem
				 * to bridge with no UNI connected to it is one
				 * of the supported and expected scenarios */
				dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
				return PON_ADAPTER_SUCCESS;
			} else if (ret != PON_ADAPTER_SUCCESS) {
				dbg_err_fn_ret(bc_ifname_get, ret);
				goto err;
			}

			ret = netlink_netdevice_enslave(ctx->netlink,
							bp->br_ifname,
							bp->ifname);
			if (ret != PON_ADAPTER_SUCCESS) {
				dbg_err_fn_ret(netlink_netdevice_enslave, ret);
				goto err;
			}

			bc_connected = true;

			ret = netlink_netdevice_state_set(ctx->netlink,
							  bp->ifname, 1);
			if (ret != PON_ADAPTER_SUCCESS) {
				dbg_err_fn_ret(netlink_netdevice_state_set,
					       ret);
				goto err;
			}
		}

		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = netlink_netdevice_enslave(ctx->netlink, bp->br_ifname,
					bp->ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_netdevice_enslave, ret);
		return ret;
	}

	ret = bport_learning_limit_set(ctx, bp);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, bport_learning_limit_set, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

err:
	if (bc_connected)
		netlink_netdevice_enslave(ctx->netlink, "", bp->ifname);
	if (gem_connected)
		netlink_netdevice_enslave(ctx->netlink, "", ifname);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
tp_gem_itp_disconnect(struct pon_net_context *ctx,
		      struct pon_net_bridge_port_config *bp,
		      bool permanent)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE] = "";
	uint8_t gem_dir;

	dbg_in_args("%p, %p, %d", ctx, bp, permanent);

	gem_dir = pon_net_gem_dir_get(ctx, bp->tp_ptr);
	if (gem_dir < PA_GEMPORTNETCTP_DIR_UPSTREAM) {
		FN_ERR_RET(ret, get_gem_dir, ret);
		return ret;
	}

	if (gem_dir == PA_GEMPORTNETCTP_DIR_DOWNSTREAM) {
		if (bp->first) {
			ret = pon_net_tp_ifname_get(ctx, bp->tp_type,
						    bp->tp_ptr, ifname,
						    sizeof(ifname));
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, pon_net_tp_ifname_get, ret);
				return ret;
			}

			ret = netlink_netdevice_enslave(ctx->netlink, "",
							ifname);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, netlink_netdevice_enslave, ret);
				return ret;
			}
		}

		if (strnlen_s(bp->ifname, IF_NAMESIZE) == 0) {
			dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
			return PON_ADAPTER_SUCCESS;
		}

		ret = netlink_netdevice_enslave(ctx->netlink, "", bp->ifname);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_netdevice_enslave, ret);
			return ret;
		}
	} else {
		ret = netlink_netdevice_enslave(ctx->netlink, "", bp->ifname);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_netdevice_enslave, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
tp_gem_itp_real_tp_ptr(struct pon_net_context *ctx, uint16_t tp_ptr,
		       uint16_t *real_tp_ptr)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %p", ctx, tp_ptr, real_tp_ptr);

	ret = pon_net_gem_interworking_tp_ctp_get(ctx, tp_ptr, real_tp_ptr);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_gem_interworking_tp_ctp_get, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
mc_ifname_get(struct pon_net_context *ctx,
	      const struct pon_net_bridge_port_config *mc_bp,
	      char *mc_ifname, char *mc_ifname2, unsigned int capacity)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	const uint16_t class_id = PON_CLASS_ID_BRIDGE_PORT_CONFIG;
	struct pon_net_me_list_item *item = NULL;
	static const uint8_t tp_type_to_mc[] = {
		[PA_BP_TP_TYPE_PPTP_UNI] = UNI_PPTP_MC,
		[PA_BP_TP_TYPE_VEIP] = UNI_VEIP_MC,
	};
	char ifname[IF_NAMESIZE] = "";

	dbg_in_args("%p, %p, %p, %p, %u", ctx, mc_bp, mc_ifname, mc_ifname2,
		    capacity);

	snprintf(mc_ifname, capacity, "%s", "");
	snprintf(mc_ifname2, capacity, "%s", "");

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		struct pon_net_bridge_port_config *bp = item->data;
		uint16_t uni_idx = 0;

		if (bp->tp_type != PA_BP_TP_TYPE_PPTP_UNI &&
		    bp->tp_type != PA_BP_TP_TYPE_VEIP)
			continue;

		if (bp->bridge_me_id != mc_bp->bridge_me_id)
			continue;

		ret = pon_net_uni_lport_get(bp->tp_ptr, &uni_idx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_uni_lport_get, ret);
			return ret;
		}

		ret = pon_net_uni_ifname_get(ctx, tp_type_to_mc[bp->tp_type],
					     uni_idx, ifname, capacity);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_uni_ifname_get, ret);
			return ret;
		}

		if (strnlen_s(mc_ifname, capacity) == 0) {
			if (strncpy_s(mc_ifname, capacity, ifname,
				      IF_NAMESIZE)) {
				dbg_err_fn(strncpy_s);
				dbg_out_ret("%d", PON_ADAPTER_ERR_NO_DATA);
				return PON_ADAPTER_ERR_NO_DATA;
			}
		} else {
			if (strncpy_s(mc_ifname2, capacity, ifname,
				      IF_NAMESIZE)) {
				dbg_err_fn(strncpy_s);
				dbg_out_ret("%d", PON_ADAPTER_ERR_NO_DATA);
				return PON_ADAPTER_ERR_NO_DATA;
			}
			break;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
tp_mc_gem_itp_update(struct pon_net_context *ctx,
		     struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", ctx, bp);

	ret = bport_learning_limit_set(ctx, bp);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, bport_learning_limit_set, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno map_handles(struct pon_net_context *ctx,
					  struct netlink_filter *filter,
					  unsigned int idx)
{
	enum pon_adapter_errno ret;
	uint32_t handle, prio;

	dbg_in_args("%p, %p, %d", ctx, filter, idx);

	ret = pon_net_dev_db_map(ctx->db, filter->device,
				 PON_NET_HANDLE_MCC_DROP_INGRESS, idx, &handle);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_map, ret);
		return ret;
	}

	ret = pon_net_dev_db_map(ctx->db, filter->device,
				 PON_NET_PRIO_MCC_DROP_INGRESS, idx, &prio);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_map, ret);
		return ret;
	}

	filter->handle = handle;
	filter->prio = (int)prio;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno filter_add(struct pon_net_context *ctx,
					 struct netlink_filter *filter,
					 unsigned int idx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %u", ctx, filter, idx);

	ret = map_handles(ctx, filter, idx);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, map_handles, ret);
		return ret;
	}

	netlink_filter_del(ctx->netlink, filter);
	ret = netlink_filter_add(ctx->netlink, filter);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_add, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno filter_del(struct pon_net_context *ctx,
					 struct netlink_filter *filter,
					 unsigned int idx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %u", ctx, filter, idx);

	ret = map_handles(ctx, filter, idx);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, map_handles, ret);
		return ret;
	}

	netlink_filter_del(ctx->netlink, filter);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
mc_unspec_drop_filters(struct pon_net_context *ctx, char *ifname,
		enum pon_adapter_errno (*fn)(struct pon_net_context *ctx,
				 struct netlink_filter *filter,
				 unsigned int idx))
{
	enum pon_adapter_errno ret;
	struct netlink_filter filter;
	unsigned int idx = 0;

	dbg_in_args("%p, %s", ctx, ifname);

	ret = netlink_filter_mc_unspec_ipv4_drop(&filter, ifname, ETH_P_IP);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_filter_mc_unspec_ipv4_drop, ret);
		goto err;
	}

	ret = fn(ctx, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(fn, ret);
		goto err;
	}

	ret =
	    netlink_filter_mc_unspec_ipv4_drop(&filter, ifname, ETH_P_8021Q);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_filter_mc_unspec_ipv4_drop, ret);
		goto err;
	}

	ret = fn(ctx, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(fn, ret);
		goto err;
	}

	ret =
	    netlink_filter_mc_unspec_ipv6_drop(&filter, ifname, ETH_P_IPV6);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_filter_mc_unspec_ipv6_drop, ret);
		goto err;
	}

	ret = fn(ctx, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(fn, ret);
		goto err;
	}

	ret =
	    netlink_filter_mc_unspec_ipv6_drop(&filter, ifname, ETH_P_8021Q);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_filter_mc_unspec_ipv6_drop, ret);
		goto err;
	}

	ret = fn(ctx, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(fn, ret);
		goto err;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

err:
	mc_unspec_drop_filters(ctx, ifname, filter_del);
	dbg_out_ret("%d", ret);
	return ret;
}

static
enum pon_adapter_errno mc_unspec_drop_enable(struct pon_net_context *ctx,
					     char *ifname)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %s", ctx, ifname);

	ret = mc_unspec_drop_filters(ctx, ifname, filter_add);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, mc_unspec_drop_filters, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static void mc_unspec_drop_disable(struct pon_net_context *ctx, char *ifname)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %s", ctx, ifname);

	ret = mc_unspec_drop_filters(ctx, ifname, filter_del);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(mc_unspec_drop_filters, ret);

	dbg_out();
}

static const struct termination_point *tp_get(uint8_t tp_type);

static enum pon_adapter_errno
tp_ext_vlan_me_id(struct pon_net_context *ctx,
		  const struct pon_net_bridge_port_config *bp,
		  uint16_t *ext_vlan_me_id);

static enum pon_adapter_errno
mc_gem_reapply_ext_vlan(struct pon_net_context *ctx,
			struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret;
	uint16_t ext_vlan_me_id;

	dbg_in_args("%p, %p", ctx, bp);

	ret = tp_ext_vlan_me_id(ctx, bp, &ext_vlan_me_id);
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, tp_ext_vlan_me_id, ret);
		return ret;
	}

	ret = pon_net_ext_vlan_reapply(ctx, ext_vlan_me_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_ext_vlan_reapply, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Return the number of connections that particular real termination point
 * has to any bridge
 */
static unsigned int count_connections(struct pon_net_me_list *list,
				      uint8_t tp_type, uint16_t real_tp_ptr)
{
	const uint16_t class_id = PON_CLASS_ID_BRIDGE_PORT_CONFIG;
	struct pon_net_me_list_item *item = NULL;
	unsigned int count = 0;

	dbg_in_args("%p, %u, %u", list, tp_type, real_tp_ptr);

	pon_net_me_list_foreach_class(list, class_id, item) {
		struct pon_net_bridge_port_config *bp = item->data;

		if (bp->tp_type != tp_type)
			continue;
		if (bp->real_tp_ptr != real_tp_ptr)
			continue;
		if (bp->connected)
			count++;
	}

	return count;
}

static enum pon_adapter_errno
multicast_bridge_ifname_get(struct pon_net_context *ctx,
			    const struct pon_net_bridge_port_config *bp,
			    char *ifname, unsigned int capacity)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int err;
	int len;
	unsigned int bridge_idx;

	dbg_in_args("%p, %p, %p, %u", ctx, bp, ifname, capacity);

	err = memset_s(ifname, capacity, 0, capacity);
	if (err) {
		FN_ERR_RET(err, memset_s, PON_ADAPTER_ERR_MEM_ACCESS);
		return PON_ADAPTER_ERR_MEM_ACCESS;
	}

	ret = mapper_index_get(ctx->mapper[MAPPER_MACBRIDGE_MEID_TO_IDX],
			       bp->bridge_me_id, &bridge_idx);
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, mapper_index_get, ret);
		return ret;
	}

	len = snprintf(ifname, capacity, "%s", PON_IFNAME_BRIDGE_MULTICAST);
	if (len >= (int)capacity) {
		FN_ERR_RET(len, snprintf, PON_ADAPTER_ERR_OUT_OF_BOUNDS);
		return PON_ADAPTER_ERR_OUT_OF_BOUNDS;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Change bp->br_ifname, to multicast bridge ifname.
 * The multicast bridge is the bridge to which all multicast devices which
 * model the shared connection are connected, if gem is shared between multiple
 * bridges.
 */
static enum pon_adapter_errno
use_multicast_bridge_ifname(struct pon_net_context *ctx,
		       struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE];
	int err;

	dbg_in_args("%p, %p", ctx, bp);

	ret = multicast_bridge_ifname_get(ctx, bp, ifname, sizeof(ifname));
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, multicast_bridge_ifname_get, ret);
		return ret;
	}

	err = strncpy_s(bp->br_ifname, sizeof(bp->br_ifname), ifname,
			sizeof(ifname));
	if (err) {
		FN_ERR_RET(err, strncpy_s, PON_ADAPTER_ERR_MEM_ACCESS);
		return PON_ADAPTER_ERR_MEM_ACCESS;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Callback for reconnect() that will cause it to reconnect only multicast gems,
 * and only those for which bridge or MC UNI devices actually changed
 */
static bool multicast_gems(struct pon_net_context *ctx,
			   const struct pon_net_bridge_port_config *bp)
{
	char new_br_ifname[IF_NAMESIZE] = "";
	char new_ifname[IF_NAMESIZE] = "";
	char new_ifname2[IF_NAMESIZE] = "";
	enum pon_adapter_errno ret;

	if (bp->tp_type != PA_BP_TP_TYPE_MC_GEM)
		return false;

	ret = multicast_bridge_ifname_get(ctx, bp, new_br_ifname,
					  sizeof(new_br_ifname));
	if (ret != PON_ADAPTER_ERR_NOT_FOUND && ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(multicast_bridge_ifname_get, ret);
		return false;
	}

	ret = mc_ifname_get(ctx, bp, new_ifname, new_ifname2,
			    sizeof(new_ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(mc_ifname_get, ret);
		return false;
	}

	if (strcmp(new_ifname, bp->ifname) == 0 &&
	    strcmp(new_ifname2, bp->ifname2) == 0 &&
	    strcmp(new_br_ifname, bp->br_ifname) == 0)
		return false;

	return true;
}

static enum pon_adapter_errno
tp_mc_gem_itp_connect(struct pon_net_context *ctx,
		      struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
#ifdef MCC_DRV_ENABLE
	int gem_ifindex, br_ifindex;
#endif
	char ifname[IF_NAMESIZE] = "";
	bool gem_connected = false;
	bool gem_up = false;
	bool mc_connected = false;
	bool mc2_connected = false;
	bool mc_drop_set = false;
	unsigned int num_connections;

	dbg_in_args("%p, %p", ctx, bp);

	ret = use_multicast_bridge_ifname(ctx, bp);
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, use_multicast_bridge_ifname, ret);
		return ret;
	}

	num_connections =
	    count_connections(&ctx->me_list, bp->tp_type, bp->real_tp_ptr);

	if (num_connections == 0) {
		ret = pon_net_tp_ifname_get(ctx, bp->tp_type, bp->tp_ptr,
					    ifname, sizeof(ifname));
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(pon_net_tp_ifname_get, ret);
			goto err;
		}

		ret = mc_unspec_drop_enable(ctx, ifname);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(mc_unspec_drop_enable, ret);
			goto err;
		}

		mc_drop_set = true;

		ret = netlink_netdevice_enslave(ctx->netlink, bp->br_ifname,
						ifname);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(netlink_netdevice_enslave, ret);
			goto err;
		}

		gem_connected = true;

		ret = netlink_netdevice_state_set(ctx->netlink, ifname, 1);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(netlink_netdevice_state_set, ret);
			goto err;
		}

		gem_up = true;

		ret = bport_learning_limit_set(ctx, bp);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(bport_learning_limit_set, ret);
			goto err;
		}

#ifdef MCC_DRV_ENABLE
		gem_ifindex = netlink_netdevice_ifindex_get(ctx->netlink,
							    ifname);
		if (!gem_ifindex) {
			dbg_err("netlink_netdevice_ifindex_get(%s) failed\n",
				ifname);
			ret = PON_ADAPTER_ERROR;
			goto err;
		}
		ctx->mc_gem_ifindex = gem_ifindex;

		br_ifindex = netlink_netdevice_ifindex_get(ctx->netlink,
							   bp->br_ifname);
		if (!br_ifindex) {
			dbg_err("netlink_netdevice_ifindex_get(%s) failed\n",
				bp->br_ifname);
			ret = PON_ADAPTER_ERROR;
			goto err;
		}
		ctx->mc_br_ifindex = br_ifindex;
		pon_net_mcc_multicast_enable(ctx);
#endif
	}

	ret = mc_ifname_get(ctx, bp, bp->ifname, bp->ifname2,
			    sizeof(bp->ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(mc_ifname_get, ret);
		goto err;
	}

	if (strnlen_s(bp->ifname, sizeof(bp->ifname)) != 0) {
		ret = netlink_netdevice_enslave(ctx->netlink, bp->br_ifname,
						bp->ifname);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(netlink_netdevice_enslave, ret);
			goto err;
		}

		mc_connected = true;

		ret = netlink_netdevice_state_set(ctx->netlink, bp->ifname, 1);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(netlink_netdevice_state_set, ret);
			goto err;
		}
	} else {
		dbg_prn("UNI is not in a bridge, multicast device will not be connected");
	}

	if (strnlen_s(bp->ifname2, sizeof(bp->ifname)) != 0) {
		ret = netlink_netdevice_enslave(ctx->netlink, bp->br_ifname,
						bp->ifname2);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(netlink_netdevice_enslave, ret);
			goto err;
		}

		mc2_connected = true;

		ret = netlink_netdevice_state_set(ctx->netlink,
						  bp->ifname2, 1);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(netlink_netdevice_state_set, ret);
			goto err;
		}
	}

	/*
	 * Refresh Extended VLAN configuration. This will apply the
	 * Extended VLAN rules which were pending until MC GEM was
	 * connected to the bridge.
	 */
	ret = mc_gem_reapply_ext_vlan(ctx, bp);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, mc_gem_reapply_ext_vlan, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

err:
	if (mc2_connected)
		netlink_netdevice_enslave(ctx->netlink, "", bp->ifname2);
	if (mc_connected)
		netlink_netdevice_enslave(ctx->netlink, "", bp->ifname);
	if (gem_up)
		netlink_netdevice_state_set(ctx->netlink, ifname, 0);
	if (gem_connected)
		netlink_netdevice_enslave(ctx->netlink, "", ifname);
	if (mc_drop_set)
		mc_unspec_drop_disable(ctx, ifname);

	dbg_out_ret("%d", ret);
	return ret;
}

/* Delete all multicast entries which belong to given ifindex */
static enum pon_adapter_errno delete_multicast_entries(int ifindex)
{
#ifdef MCC_DRV_ENABLE
	int err;
#endif

	dbg_in_args("%d", ifindex);

	if (ifindex <= 0) {
		dbg_err("ifindex must be positive\n");
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

#ifdef MCC_DRV_ENABLE
	err = mcl_table_entry_del((uint32_t)ifindex, MCC_L3_PROTO_IPV4, NULL,
				  NULL, SRC_FLT_MODE_NA);
	if (err) {
		FN_ERR_RET(err, mcl_table_entry_del, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
#endif
	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Delete all multicast entries which belong to device specified by ifname */
static enum pon_adapter_errno
delete_multicast_entries_by_ifname(struct pon_net_netlink *netlink,
				   const char *ifname)
{
	int ifindex;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", netlink, ifname);

	ifindex = netlink_netdevice_ifindex_get(netlink, ifname);
	if (ifindex <= 0) {
		FN_ERR_RET(ifindex, netlink_netdevice_ifindex_get,
			   PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	ret = delete_multicast_entries(ifindex);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, delete_multicast_entries, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
tp_mc_gem_itp_disconnect(struct pon_net_context *ctx,
			 struct pon_net_bridge_port_config *bp,
			 bool permanent)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE] = "";
	unsigned int num_connections;

	dbg_in_args("%p, %p, %d", ctx, bp, permanent);

	num_connections =
	    count_connections(&ctx->me_list, bp->tp_type, bp->real_tp_ptr);

	if (num_connections == 1) {
#ifdef MCC_DRV_ENABLE
		pon_net_mcc_multicast_disable(ctx);
		ctx->mc_gem_ifindex = 0;
		ctx->mc_br_ifindex = 0;
#endif

		ret = pon_net_tp_ifname_get(ctx, bp->tp_type, bp->tp_ptr,
					    ifname, sizeof(ifname));
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_tp_ifname_get, ret);
			return ret;
		}

		if (netlink_netdevice_exists(ctx->netlink, ifname)) {
			ret =
			    netlink_netdevice_enslave(ctx->netlink, "", ifname);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, netlink_netdevice_enslave, ret);
				return ret;
			}

			mc_unspec_drop_disable(ctx, ifname);
		}
	}

	/*
	 * We must delete L3 table entries, while we are still in the bridge,
	 * but only if this is the disconnect done just before complete bridge
	 * port destroy (this is why we check permanent variable)
	 */
	if (permanent && strnlen_s(bp->ifname, IF_NAMESIZE)) {
		ret = delete_multicast_entries_by_ifname(ctx->netlink,
							 bp->ifname);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, delete_multicast_entries_by_ifname,
				   ret);
			return ret;
		}
	}
	if (permanent && strnlen_s(bp->ifname2, IF_NAMESIZE)) {
		ret = delete_multicast_entries_by_ifname(ctx->netlink,
							 bp->ifname2);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, delete_multicast_entries_by_ifname,
				   ret);
			return ret;
		}
	}

	if (strnlen_s(bp->ifname, IF_NAMESIZE)) {
		ret = netlink_netdevice_enslave(ctx->netlink, "", bp->ifname);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_netdevice_enslave, ret);
			return ret;
		}
	}

	if (strnlen_s(bp->ifname2, IF_NAMESIZE)) {
		ret = netlink_netdevice_enslave(ctx->netlink, "", bp->ifname2);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_netdevice_enslave, ret);
			return ret;
		}
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	snprintf(bp->ifname, sizeof(bp->ifname), "%s", "");
	snprintf(bp->ifname2, sizeof(bp->ifname2), "%s", "");

	/*
	 * After we disconnect the MC GEM from the bridge, we must trigger
	 * remove the Extended VLAN rules for it, because if it is no longer
	 * in the bridge, it is not related to ani multicast UNI interface.
	 * We also need to restore the UNI Extended VLAN rules, which were
	 * removed from this interface when MC GEM Extended VLAN was originally
	 * configured.
	 */
	ret = mc_gem_reapply_ext_vlan(ctx, bp);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, mc_gem_reapply_ext_vlan, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
tp_mc_gem_itp_destroy(struct pon_net_context *ctx,
		      struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", ctx, bp);

	ret = reconnect(ctx, multicast_gems);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, reconnect, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
tp_mc_gem_itp_real_tp_ptr(struct pon_net_context *ctx, uint16_t tp_ptr,
			  uint16_t *real_tp_ptr)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %p", ctx, tp_ptr, real_tp_ptr);

	ret = pon_net_multicast_gem_interworking_tp_ctp_get(ctx, tp_ptr,
							    real_tp_ptr);
	if (ret) {
		FN_ERR_RET(ret, pon_net_multicast_gem_interworking_tp_ctp_get,
			   ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/* if VLAN Tagging Filter data was created before pmapper was added
   to the bridge, then this function will update filtering  */
static enum pon_adapter_errno
pmapper_reapply_filtering(struct pon_net_context *ctx, uint16_t pmapper_me_id,
			  uint16_t mac_bp_me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char pmapper_ifname[IF_NAMESIZE] = "";
	int should_reapply = 0;
	bool pmapper_has_gem = false;

	dbg_in_args("%p, %u, %u", ctx, pmapper_me_id, mac_bp_me_id);

	ret = pon_net_tp_ifname_get(ctx,
				    PA_BP_TP_TYPE_PMAP,
				    pmapper_me_id,
				    pmapper_ifname,
				    sizeof(pmapper_ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_tp_ifname_get, ret);
		return ret;
	}

	ret = netlink_is_slave(ctx->netlink, pmapper_ifname, &should_reapply);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_is_slave, ret);
		return ret;
	}

	if (should_reapply == 0) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = netlink_pmapper_has_gem_ports(ctx->netlink, pmapper_ifname,
					    &pmapper_has_gem);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_pmapper_has_gem_ports, ret);
		return ret;
	}

	if (!pmapper_has_gem) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = pon_net_vlan_filtering_update(ctx, mac_bp_me_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_vlan_filtering_update, ret);
		return ret;
	}

	ret = pon_net_bp_vlan_forwarding_update(ctx, &mac_bp_me_id, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_bp_vlan_forwarding_update, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
tp_pmapper_create(struct pon_net_context *ctx,
		  struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", ctx, bp);

	ret = pon_net_tp_ifname_get(ctx, bp->tp_type, bp->tp_ptr,
				    bp->ifname, sizeof(bp->ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_tp_ifname_get, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
tp_pmapper_update(struct pon_net_context *ctx,
		  struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", ctx, bp);

	ret = bport_learning_limit_set(ctx, bp);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, bport_learning_limit_set, ret);
		return ret;
	}
	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
connect_pmapper(struct pon_net_context *ctx, const char *br_ifname,
		const char *pmapper_ifname)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	bool pmapper_has_gem = false;

	dbg_in_args("%p, \"%s\", \"%s\"", ctx, br_ifname, pmapper_ifname);

	ret = netlink_pmapper_has_gem_ports(ctx->netlink, pmapper_ifname,
					    &pmapper_has_gem);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_pmapper_has_gem_ports, ret);
		return ret;
	}

	dbg_msg("Pmapper has gem: %d\n", pmapper_has_gem);
	if (!pmapper_has_gem) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = netlink_netdevice_enslave(ctx->netlink, br_ifname,
					pmapper_ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_detdevice_enslave, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
tp_pmapper_connect(struct pon_net_context *ctx,
		   struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", ctx, bp);

	ret = connect_pmapper(ctx, bp->br_ifname, bp->ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, connect_pmapper, ret);
		return ret;
	}

	/* if VLAN Tagging Filter data was created before pmapper was added
	   to the bridge, then we will create the tc-filters now */
	ret = pmapper_reapply_filtering(ctx, bp->tp_ptr, bp->me_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pmapper_reapply_filtering, ret);
		return ret;
	}

	ret = bport_learning_limit_set(ctx, bp);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, bport_learning_limit_set, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
tp_pmapper_disconnect(struct pon_net_context *ctx,
		      struct pon_net_bridge_port_config *bp,
		      bool permanent)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p, %d", ctx, bp, permanent);

	ret = pon_net_bp_vlan_forwarding_cleanup(ctx, &bp->me_id, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_bp_vlan_forwarding_cleanup, ret);
		return ret;
	}

	ret = pon_net_vlan_filtering_cleanup(ctx, bp->me_id);
	if (ret) {
		FN_ERR_RET(ret, pon_net_vlan_filtering_cleanup, ret);
		return ret;
	}

	ret = netlink_netdevice_enslave(ctx->netlink, "", bp->ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_netdevice_enslave, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
tp_pmapper_reconnect(struct pon_net_context *ctx,
		     struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pmapper *pmapper;

	dbg_in_args("%p, %p", ctx, bp);

	pmapper = pon_net_me_list_get_data(&ctx->me_list, PON_CLASS_ID_PMAPPER,
					   bp->tp_ptr);
	if (!pmapper) {
		FN_ERR_RET(0, pon_net_me_list_get_data,
			   PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	if (pmapper->needs_reconnect) {
		ret = tp_pmapper_disconnect(ctx, bp, false);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, tp_pmapper_disconnect, ret);
			return ret;
		}
		pmapper->needs_reconnect = false;
	}

	ret = tp_pmapper_connect(ctx, bp);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, tp_pmapper_connect, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
tp_iphost_connect(struct pon_net_context *ctx,
		  struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", ctx, bp);

	/*
	 * Refresh VLAN aware forwarding for all bridge ports
	 *
	 * We need to call this to trigger vlan forwarding update
	 * on P-Mapper bridge port, because it uses IP Host connected
	 * to a bridge (in case of VLAN flow forwarding).
	 */
	ret = pon_net_bp_vlan_forwarding_update(ctx, NULL, 0);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_bp_vlan_forwarding, ret);
		return ret;
	}

	ret = pon_net_tp_ifname_get(ctx, bp->tp_type, bp->tp_ptr, bp->ifname,
				    sizeof(bp->ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_tp_ifname_get, ret);
		return ret;
	}

	/* Connect LAN interface used for IP Host reinsert */
	ret = netlink_netdevice_enslave(ctx->netlink, bp->br_ifname,
					bp->ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_netdevice_enslave, ret);
		return ret;
	}

	ret = netlink_netdevice_state_set(ctx->netlink, bp->ifname, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		netlink_netdevice_enslave(ctx->netlink, "", bp->ifname);
		FN_ERR_RET(ret, netlink_netdevice_state_set, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
tp_iphost_disconnect(struct pon_net_context *ctx,
		     struct pon_net_bridge_port_config *bp,
		     bool permanent)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p, %d", ctx, bp, permanent);

	ret = pon_net_bp_vlan_forwarding_cleanup(ctx, &bp->me_id, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_bp_vlan_forwarding_update, ret);
		return ret;
	}

	ret = netlink_netdevice_enslave(ctx->netlink, "", bp->ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_netdevice_enslave, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
tp_iphost_destroy(struct pon_net_context *ctx,
		  struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", ctx, bp);

	/*
	 * Refresh VLAN aware forwarding for all bridge ports
	 *
	 * We need to call this to trigger vlan forwarding update
	 * on P-Mapper bridge port, because it uses UNI connected
	 * to a bridge (in case of VLAN flow forwarding).
	 */
	ret = pon_net_bp_vlan_forwarding_update(ctx, NULL, 0);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_bp_vlan_forwarding_update, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/**
 * Group of callbacks to be called when specific type of termination
 * point is created/deleted/updated.
 */
struct termination_point {
	/**
	 * Called when MAC Bridge Port ME is created, before it is added to
	 * me_list
	 *
	 * \param[in] ctx    PON Network Library context pointer
	 * \param[in] bp     Bridge port
	 *
	 * \return returns value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*create)(struct pon_net_context *ctx,
		  struct pon_net_bridge_port_config *bp);

	/**
	 * Is called when MAC Bridge Port ME is updated
	 *
	 * \param[in] ctx    PON Network Library context pointer
	 * \param[in] bp     Bridge port
	 *
	 * \return returns value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*update)(struct pon_net_context *ctx,
		  struct pon_net_bridge_port_config *bp);

	/**
	 * Connect underlying device to a bridge
	 *
	 * \param[in] ctx    PON Network Library context pointer
	 * \param[in] bp     Bridge port
	 *
	 * \return returns value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*connect)(struct pon_net_context *ctx,
		  struct pon_net_bridge_port_config *bp);

	/**
	 * Disconnect underlying device from a bridge
	 *
	 * \param[in] ctx        PON Network Library context pointer
	 * \param[in] bp         Bridge port
	 * \param[in] permanent  True, if the disconnect is performed just
	 *                       before destroying the bridge port, with no
	 *                       intention of connecting the port again.
	 *
	 * \return returns value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*disconnect)(struct pon_net_context *ctx,
		  struct pon_net_bridge_port_config *bp, bool permanent);

	/** Reconnect underlying device to a bridge
	 *
	 * \param[in] ctx    PON Network Library context pointer
	 * \param[in] bp     Bridge port
	 *
	 * \return returns value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*reconnect)(struct pon_net_context *ctx,
		  struct pon_net_bridge_port_config *bp);

	/**
	 * Called when MAC Bridge Port ME is deleted, after it is removed
	 * from me_list
	 *
	 * \param[in] ctx    PON Network Library context pointer
	 * \param[in] bp     Bridge port
	 *
	 * \return returns value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*destroy)(struct pon_net_context *ctx,
		  struct pon_net_bridge_port_config *bp);

	/**
	 * Get VLAN forwarding parameters. If not implemented, then no
	 * vlan forwarding will be configured on this type of bridge port
	 *
	 * \param[in]  ctx    PON Network Library context pointer
	 * \param[in]  bp     Bridge port
	 * \param[out] fwd    VLAN forwarding configuration
	 *
	 * \return returns value as follows:
	 * - PON_ADAPTER_SUCCESS: If successful, VLAN forwarding will be
	 *                        configured with options specified in 'fwd'
	 * - PON_ADAPTER_ERR_NOT_FOUND: VLAN forwarding will be configured
	 *                              with options specified in 'fwd'
	 * - Other: An error code in case of error.
	 */
	enum pon_adapter_errno (*vlan_forwarding)(
					struct pon_net_context *ctx,
					struct pon_net_bridge_port_config *bp,
					struct pon_net_vlan_forwarding *fwd);

	/**
	 * Get real TP ptr for the tp_ptr
	 *
	 * Termination points like GEM Port Interworking TP or Multicast
	 * GEM Port Interworking TP are proxies that connect GEM Port
	 * Network CTP to the bridge. For such cases the function will return
	 * ME ID that is actually being connected to the bridge.
	 */
	enum pon_adapter_errno (*real_tp_ptr)(struct pon_net_context *ctx,
					      uint16_t tp_ptr,
					      uint16_t *real_tp_ptr);

	/**
	 * Which Extended VLAN association type, this termination point type
	 * is related to. This is needed to retrieve information about
	 * Extended VLAN needed to setup VLAN forwarding.
	 */
	enum pon_adapter_ext_vlan_associated_me association_type;
};

static enum pon_adapter_errno
tp_ext_vlan_me_id(struct pon_net_context *ctx,
		  const struct pon_net_bridge_port_config *bp,
		  uint16_t *ext_vlan_me_id)
{
	enum pon_adapter_errno ret;
	const struct termination_point *tp;
	uint16_t me_id;

	dbg_in_args("%p, %p, %p", ctx, bp, ext_vlan_me_id);

	tp = tp_get(bp->tp_type);
	if (!tp) {
		FN_ERR_RET(0, tp_get, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	/* First try to get the Ext. Vlan for this bridge port */
	ret = pon_net_ext_vlan_me_id_get(ctx, PA_EXT_VLAN_MAC_BP_CFG_DATA,
					 bp->me_id, &me_id);
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		/* If that fails, then try to get Ext. Vlan for the
		 * ME pointed by bridge port */
		ret = pon_net_ext_vlan_me_id_get(ctx, tp->association_type,
						 bp->tp_ptr, &me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			if (ret != PON_ADAPTER_ERR_NOT_FOUND)
				dbg_err_fn_ret(pon_net_ext_vlan_me_id_get, ret);
			dbg_out_ret("%d", ret);
			return ret;
		}
	}
	if (ret != PON_ADAPTER_SUCCESS) {
		if (ret != PON_ADAPTER_ERR_NOT_FOUND)
			dbg_err_fn_ret(pon_net_ext_vlan_me_id_get, ret);
		dbg_out_ret("%d", ret);
		return ret;
	}

	if (ext_vlan_me_id)
		*ext_vlan_me_id = me_id;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Get VLAN forwarding configuration from VLAN Tagging Filter Data */
static enum pon_adapter_errno
tp_vlan_forwarding(struct pon_net_context *ctx,
		   struct pon_net_bridge_port_config *bp,
		   struct pon_net_vlan_forwarding *fwd)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %p", ctx, bp, fwd);

	ret = pon_net_vlan_tagging_filter_data_vlan_forwarding_get(ctx,
								   bp->me_id,
								   fwd);
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		dbg_out_ret("%d", ret);
		return ret;
	}
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret,
			   pon_net_vlan_tagging_filter_data_vlan_forwarding_get,
			   ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Get VLAN forwarding configuration from Ext. Vlan */
static enum pon_adapter_errno
tp_ext_vlan_forwarding(struct pon_net_context *ctx,
		       struct pon_net_bridge_port_config *bp,
		       struct pon_net_vlan_forwarding *fwd)
{
	enum pon_adapter_errno ret;
	const struct termination_point *tp;
	uint16_t me_id;

	dbg_in_args("%p, %p, %p", ctx, bp, fwd);

	tp = tp_get(bp->tp_type);
	if (!tp) {
		FN_ERR_RET(0, tp_get, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	ret = tp_ext_vlan_me_id(ctx, bp, &me_id);
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		dbg_out_ret("%d", ret);
		return ret;
	}
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, tp_ext_vlan_me_id, ret);
		return ret;
	}

	/* Get VLAN forwarding configuration from Ext. VLAN ME */
	ret = pon_net_ext_vlan_vlan_forwarding_get(ctx, me_id, fwd);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_ext_vlan_forwarding_get, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Get VLAN forwarding configuration for P-Mapper, skip update if
 * P-Mapper is not really in the bridge
 */
static enum pon_adapter_errno
tp_pmapper_vlan_forwarding(struct pon_net_context *ctx,
			   struct pon_net_bridge_port_config *bp,
			   struct pon_net_vlan_forwarding *fwd)
{
	enum pon_adapter_errno ret;
	int should_update;

	dbg_in_args("%p, %p, %p", ctx, bp, fwd);

	/*
	 * We should only update pmapper filters when it is
	 * enslaved to a bridge
	 */
	ret = netlink_is_slave(ctx->netlink, bp->ifname, &should_update);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_is_slave, ret);
		return ret;
	}

	if (!should_update) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	ret = tp_vlan_forwarding(ctx, bp, fwd);
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		dbg_out_ret("%d", ret);
		return ret;
	}
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, tp_vlan_forwarding, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/*
 * Get VLAN forwarding configuration for GEM, but do not configure
 * it on Broadcast GEMs
 */
static enum pon_adapter_errno
tp_gem_vlan_forwarding(struct pon_net_context *ctx,
		       struct pon_net_bridge_port_config *bp,
		       struct pon_net_vlan_forwarding *fwd)
{
	enum pon_adapter_errno ret;
	uint8_t gem_dir;

	dbg_in_args("%p, %p, %p", ctx, bp, fwd);

	gem_dir = pon_net_gem_dir_get(ctx, bp->tp_ptr);
	if (gem_dir < PA_GEMPORTNETCTP_DIR_UPSTREAM) {
		FN_ERR_RET((int)gem_dir, pon_net_gem_dir_get,
			   PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
	/* Do not configure VLAN forwarding if GEM is dowstream only */
	if (gem_dir == PA_GEMPORTNETCTP_DIR_DOWNSTREAM) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	ret = tp_vlan_forwarding(ctx, bp, fwd);
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		dbg_out_ret("%d", ret);
		return ret;
	}
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, tp_vlan_forwarding, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/*
 * Configure VLAN forwarding using information from Extended VLAN, but
 * only if VLAN aware bridging is enabled
 */
static enum pon_adapter_errno
tp_ext_vlan_forwarding_if_vlan_aware_bridging(struct pon_net_context *ctx,
			   struct pon_net_bridge_port_config *bp,
			   struct pon_net_vlan_forwarding *fwd)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %p", ctx, bp, fwd);

	/* If vlan forwarding option is enabled, then do nothing */
	if (ctx->cfg.vlan_forwarding) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	ret = tp_ext_vlan_forwarding(ctx, bp, fwd);
	if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
		dbg_out_ret("%d", ret);
		return ret;
	}
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, tp_ext_vlan_forwarding, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Handling of new TP types should be added here */
static const struct termination_point termination_points[] = {
	[PA_BP_TP_TYPE_PPTP_UNI] = {
		.create = tp_uni_create,
		.update = tp_uni_update,
		.destroy = tp_uni_destroy,
		.connect = tp_uni_connect,
		.disconnect = tp_uni_disconnect,
		.reconnect = tp_uni_reconnect,
		.vlan_forwarding =
			tp_ext_vlan_forwarding_if_vlan_aware_bridging,
		.association_type = PA_EXT_VLAN_PPTP_ETH_UNI,
	},
	[PA_BP_TP_TYPE_VEIP] = {
		.create = tp_uni_create,
		.update = tp_uni_update,
		.destroy = tp_uni_destroy,
		.connect = tp_uni_connect,
		.disconnect = tp_uni_disconnect,
		.reconnect = tp_uni_reconnect,
		.vlan_forwarding =
			tp_ext_vlan_forwarding_if_vlan_aware_bridging,
		.association_type = PA_EXT_VLAN_VEIP,
	},
	[PA_BP_TP_TYPE_MC_GEM] = {
		.update = tp_mc_gem_itp_update,
		.connect = tp_mc_gem_itp_connect,
		.disconnect = tp_mc_gem_itp_disconnect,
		.destroy = tp_mc_gem_itp_destroy,
		.association_type = PA_EXT_VLAN_MC_GEM_ITP,
		.real_tp_ptr = tp_mc_gem_itp_real_tp_ptr,
	},
	[PA_BP_TP_TYPE_GEM] = {
		.create = tp_gem_itp_create,
		.update = tp_gem_itp_update,
		.destroy = tp_gem_itp_destroy,
		.connect = tp_gem_itp_connect,
		.disconnect = tp_gem_itp_disconnect,
		.vlan_forwarding = tp_gem_vlan_forwarding,
		.association_type = PA_EXT_VLAN_GEM_ITP,
		.real_tp_ptr = tp_gem_itp_real_tp_ptr,
	},
	[PA_BP_TP_TYPE_PMAP] = {
		.create = tp_pmapper_create,
		.update = tp_pmapper_update,
		.connect = tp_pmapper_connect,
		.disconnect = tp_pmapper_disconnect,
		.reconnect = tp_pmapper_reconnect,
		.vlan_forwarding = tp_pmapper_vlan_forwarding,
		.association_type = PA_EXT_VLAN_IEEE_P_MAPPER,
	},
	[PA_BP_TP_TYPE_IP_HOST] = {
		.connect = tp_iphost_connect,
		.disconnect = tp_iphost_disconnect,
		.destroy = tp_iphost_destroy,
		.association_type = PA_EXT_VLAN_IP_HOST_CONFIG_DATA,
	},
};

static const struct termination_point *tp_get(uint8_t tp_type)
{
	if (tp_type >= ARRAY_SIZE(termination_points))
		return NULL;

	return &termination_points[tp_type];
}

enum pon_adapter_errno
pon_net_association_type_to_tp_type(uint16_t association_type, uint8_t *tp_type)
{
	uint8_t type;

	dbg_in_args("%u, %p", association_type, tp_type);

	for (type = 0; type < ARRAY_SIZE(termination_points); ++type) {
		if (termination_points[type].association_type ==
		    association_type) {
			if (tp_type)
				*tp_type = type;
			dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
			return PON_ADAPTER_SUCCESS;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_FOUND);
	return PON_ADAPTER_ERR_NOT_FOUND;
}

static bool has_first(struct pon_net_me_list *list,
		      uint8_t tp_type, uint16_t real_tp_ptr)
{
	const uint16_t class_id = PON_CLASS_ID_BRIDGE_PORT_CONFIG;
	struct pon_net_me_list_item *item = NULL;

	dbg_in_args("%p, %u, %u", list, tp_type, real_tp_ptr);

	pon_net_me_list_foreach_class(list, class_id, item) {
		struct pon_net_bridge_port_config *bp = item->data;

		if (bp->tp_type != tp_type)
			continue;

		if (bp->real_tp_ptr != real_tp_ptr)
			continue;

		if (bp->first) {
			dbg_out_ret("%d", true);
			return true;
		}
	}

	dbg_out_ret("%d", false);
	return false;
}

static enum pon_adapter_errno
tp_create(struct pon_net_context *ctx,
	  struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	const struct termination_point *tp = tp_get(bp->tp_type);

	dbg_in_args("%p, %p", ctx, bp);

	if (!tp) {
		dbg_prn("Unsupported TP type: %u\n", bp->tp_type);
		dbg_out_ret("%d", ret);
		return ret;
	}

	if (tp->create)
		ret = tp->create(ctx, bp);

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
tp_update(struct pon_net_context *ctx,
	  struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	const struct termination_point *tp = tp_get(bp->tp_type);

	dbg_in_args("%p, %p", ctx, bp);

	if (!tp) {
		dbg_prn("Unsupported TP type: %u\n", bp->tp_type);
		dbg_out_ret("%d", ret);
		return ret;
	}

	if (tp->update)
		ret = tp->update(ctx, bp);

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
tp_destroy(struct pon_net_context *ctx,
	   struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	const struct termination_point *tp = tp_get(bp->tp_type);

	dbg_in_args("%p, %p", ctx, bp);

	if (!tp) {
		dbg_prn("Unsupported TP type: %u\n", bp->tp_type);
		dbg_out_ret("%d", ret);
		return ret;
	}

	if (tp->destroy)
		ret = tp->destroy(ctx, bp);

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
tp_connect(struct pon_net_context *ctx,
	   struct pon_net_bridge_port_config *bp)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	const struct termination_point *tp = tp_get(bp->tp_type);

	dbg_in_args("%p, %p", ctx, bp);

	if (!tp) {
		dbg_prn("Unsupported TP type: %u\n", bp->tp_type);
		dbg_out_ret("%d", ret);
		return ret;
	}

	if (tp->connect)
		ret = tp->connect(ctx, bp);

	bp->connected = true;

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
tp_disconnect(struct pon_net_context *ctx,
	      struct pon_net_bridge_port_config *bp,
	      bool permanent)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	const struct termination_point *tp = tp_get(bp->tp_type);

	dbg_in_args("%p, %p, %d", ctx, bp, permanent);

	if (!tp) {
		dbg_prn("Unsupported TP type: %u\n", bp->tp_type);
		dbg_out_ret("%d", ret);
		return ret;
	}

	if (tp->disconnect)
		ret = tp->disconnect(ctx, bp, permanent);

	bp->connected = false;

	dbg_out_ret("%d", ret);
	return ret;
}

/*
 * Activate VLAN flow forwarding or VLAN aware bridging, depending on
 * configuration option
 */
static enum pon_adapter_errno
vlan_forwarding_update(struct pon_net_context *ctx, uint16_t me_id,
		       const uint16_t *tci_list, uint8_t tci_list_len,
		       uint16_t tci_mask, bool untagged)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %p, %u, %u, %d", ctx, me_id, tci_list,
		    tci_list_len, tci_mask, untagged);

	if (ctx->cfg.vlan_forwarding) {
		/* Configure VLAN flow forwarding */
		ret = pon_net_vlan_forwarding_update(ctx, me_id, tci_list,
						     tci_list_len, tci_mask,
						     untagged);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_vlan_forwarding_update, ret);
			return ret;
		}
	} else {
		/* Configure VLAN aware bridging */
		ret = pon_net_vlan_aware_bridging_update(ctx->vab,
							 &ctx->me_list,
							 me_id, tci_list,
							 tci_list_len, ctx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_vlan_aware_bridging_update,
				   ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Clear VLAN flow forwarding or VLAN aware bridging, depending on configuration
 * option
 */
static enum pon_adapter_errno
vlan_forwarding_clear(struct pon_net_context *ctx, uint16_t me_id)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u", ctx, me_id);

	if (ctx->cfg.vlan_forwarding) {
		/* Clean up VLAN flow forwarding */
		ret = pon_net_vlan_flow_clear_all(ctx, me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_vlan_flow_clear_all, ret);
			return ret;
		}
	} else {
		ret = pon_net_vlan_aware_bridging_clear(ctx->vab, me_id, ctx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_vlan_aware_bridging_clear, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_net_bp_vlan_forwarding_update(struct pon_net_context *ctx, uint16_t *me_ids,
				  unsigned int me_ids_len)
{
	struct pon_net_me_list_item *item;
	struct pon_net_vlan_forwarding vlan_forwarding;
	struct pon_net_bridge_port_config *bp;
	uint16_t class_id = PON_CLASS_ID_BRIDGE_PORT_CONFIG;
	uint16_t me_id;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	const struct termination_point *tp;

	dbg_in_args("%p, %p, %u", ctx, me_ids, me_ids_len);

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		bp = item->data;
		me_id = item->me_id;

		/* We can update everything, or just selected MEs */
		if (me_ids &&
		    !pon_net_me_id_in_array(me_id, me_ids, me_ids_len))
			continue;

		tp = tp_get(bp->tp_type);
		if (!tp) {
			FN_ERR_RET(0, tp_get, PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}

		if (!tp->vlan_forwarding)
			continue;

		ret = tp->vlan_forwarding(ctx, bp, &vlan_forwarding);
		if (ret == PON_ADAPTER_ERR_NOT_FOUND) {
			/*
			 * There is no VLAN forwarding configured for
			 * this bridge port - we skip it
			 */
			continue;
		}
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, vlan_forwarding, ret);
			return ret;
		}

		/* Update VLAN forwarding */
		ret = vlan_forwarding_update(ctx,
					me_id,
					vlan_forwarding.tci_list,
					vlan_forwarding.tci_list_len,
					vlan_forwarding.tci_mask,
					vlan_forwarding.untagged_bridging);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, vlan_forwarding_update, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

unsigned int pon_net_bp_all_get(struct pon_net_context *ctx, uint16_t br_me_id,
				uint16_t *me_ids, unsigned int me_ids_capacity)
{
	unsigned int len = 0;
	struct pon_net_me_list_item *item;
	struct pon_net_bridge_port_config *bp;
	uint16_t class_id = PON_CLASS_ID_BRIDGE_PORT_CONFIG;

	dbg_in_args("%p, %u, %p, %u", ctx, br_me_id, me_ids, me_ids_capacity);

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		bp = item->data;
		if (bp->bridge_me_id != br_me_id)
			continue;

		if (len < me_ids_capacity)
			me_ids[len] = item->me_id;
		len++;
	}

	dbg_out_ret("%u", len);
	return len;
}

enum pon_adapter_errno
pon_net_bp_vlan_forwarding_cleanup(struct pon_net_context *ctx,
				   uint16_t *me_ids, unsigned int me_ids_len)
{
	struct pon_net_me_list_item *item;
	struct pon_net_bridge_port_config *bp;
	uint16_t class_id = PON_CLASS_ID_BRIDGE_PORT_CONFIG;
	uint16_t me_id;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	const struct termination_point *tp;

	dbg_in_args("%p, %p, %u", ctx, me_ids, me_ids_len);

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		bp = item->data;
		me_id = item->me_id;

		/* We can update everything, or just selected MEs */
		if (me_ids &&
		    !pon_net_me_id_in_array(me_id, me_ids, me_ids_len))
			continue;

		tp = tp_get(bp->tp_type);
		if (!tp) {
			FN_ERR_RET(0, tp_get, PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}

		if (!tp->vlan_forwarding)
			continue;

		/* Update VLAN forwarding */
		ret = vlan_forwarding_clear(ctx, me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, vlan_forwarding_clear, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno pon_net_bp_me_id_get(struct pon_net_context *ctx,
					    uint8_t tp_type, uint16_t tp_ptr,
					    uint16_t *me_id)
{
	struct pon_net_me_list_item *item;
	struct pon_net_bridge_port_config *bp;
	uint16_t class_id = PON_CLASS_ID_BRIDGE_PORT_CONFIG;

	dbg_in_args("%p, %u, %u, %p", ctx, tp_type, tp_ptr, me_id);

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		bp = item->data;
		if (bp->tp_type == tp_type && bp->tp_ptr == tp_ptr) {
			if (me_id)
				*me_id = item->me_id;
			dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
			return PON_ADAPTER_SUCCESS;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_FOUND);
	return PON_ADAPTER_ERR_NOT_FOUND;
}

/*
 * Call tp_disconnect() followed by tp_connect() for those bridge ports for
 * which should_reconnect() returns true.
 *
 * Instead of connecting one GEM to multiple bridges (which would not be
 * possible in Linux) we choose one "main" bridge, connect a gem to it once and
 * "simulate" remaining connections by adding eth0_X_Y interfaces to the "main"
 * bridge.
 *
 * Which eth0_X_Y interface is used depends on the UNI port connected to these
 * other bridges. Hence, if UNI bridge port is destroyed or changed, we must
 * call reconnect() function to rebuild these connections, because after the
 * change to the UNI bridge port they may look different.
 */
static enum pon_adapter_errno
reconnect(struct pon_net_context *ctx,
	  bool (*should_reconnect)(struct pon_net_context *ctx,
				   const struct pon_net_bridge_port_config *bp))
{
	const uint16_t class_id = PON_CLASS_ID_BRIDGE_PORT_CONFIG;
	struct pon_net_me_list_item *item = NULL;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %p", ctx, should_reconnect);

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		struct pon_net_bridge_port_config *bp = item->data;

		if (!should_reconnect(ctx, bp))
			continue;

		ret = tp_disconnect(ctx, bp, false);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, tp_disconnect, ret);
			return ret;
		}

		if (!has_first(&ctx->me_list, bp->tp_type, bp->real_tp_ptr))
			bp->first = true;

		ret = tp_connect(ctx, bp);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, tp_connect, ret);
			return ret;
		}
	}

	dbg_out_ret("%u", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_net_bp_reconnect_multicast_gems(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p", ctx);

	ret = reconnect(ctx, multicast_gems);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, reconnect, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return ret;
}

static enum pon_adapter_errno
update_preassign_tables(struct pon_net_context *ctx, uint16_t br_me_id)
{
	uint16_t *me_ids;
	unsigned int me_ids_len;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u", ctx, br_me_id);

	me_ids_len = pon_net_bp_all_get(ctx, br_me_id, NULL, 0);
	if (!me_ids_len || me_ids_len > BRIDGE_PORT_MAX) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	me_ids = calloc(me_ids_len, sizeof(*me_ids));
	if (!me_ids) {
		FN_ERR_RET(0, malloc, PON_ADAPTER_ERR_NO_MEMORY);
		return PON_ADAPTER_ERR_NO_MEMORY;
	}

	if (pon_net_bp_all_get(ctx, br_me_id, me_ids, me_ids_len) !=
	    me_ids_len) {
		ret = PON_ADAPTER_ERR_NO_MEMORY;
		dbg_err("unexpected return value from pon_net_bp_all_get()");
		goto err_free_me_ids;
	}

	ret = pon_net_mac_bridge_port_filter_preassign_table_update(ctx, me_ids,
								    me_ids_len);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(
		    pon_net_mac_bridge_port_filter_preassign_table_update, ret);
		goto err_free_me_ids;
	}

err_free_me_ids:
	free(me_ids);

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno update(void *ll_handle,
		    const uint16_t me_id,
		    const struct pa_mac_bp_config_data_upd_data *update_data)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint32_t bridge_idx;
	struct pon_net_bridge_port_config bp = { 0 };
	const uint16_t class_id = PON_CLASS_ID_BRIDGE_PORT_CONFIG;
	bool create = false;
	const struct termination_point *tp;

	dbg_in_args("%p, %u, %p", ll_handle, me_id, update_data);

	if (!update_data) {
		dbg_err("NULL pointer update_data\n");
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	ret = mapper_index_get(ctx->mapper[MAPPER_MACBRIDGE_MEID_TO_IDX],
				 update_data->bridge_id_ptr, &bridge_idx);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, mapper_index_get, ret);
		return ret;
	}

	ret = pon_net_me_list_read(&ctx->me_list, class_id, me_id, &bp,
				   sizeof(bp));
	if (ret == PON_ADAPTER_SUCCESS) {
		if (bp.tp_type != update_data->tp_type ||
		    bp.tp_ptr != update_data->tp_ptr) {
			/* tp_type or tp_ptr_changed. It should not happen. */
			dbg_err("TP type or TP ptr change is not supported\n");
			dbg_out_ret("%d", PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}
		if (bp.bridge_me_id != update_data->bridge_id_ptr) {
			/* tp_type or tp_ptr_changed. It should not happen. */
			dbg_err("Bridge change not supported\n");
			dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
			return PON_ADAPTER_ERR_INVALID_VAL;
		}
	}

	if (ret == PON_ADAPTER_ERR_NOT_FOUND)
		create = true;

	bp.tp_ptr = update_data->tp_ptr;
	bp.tp_type = update_data->tp_type;

	tp = tp_get(bp.tp_type);

	bp.real_tp_ptr = bp.tp_ptr;
	if (tp && tp->real_tp_ptr) {
		ret = tp->real_tp_ptr(ctx, bp.tp_ptr, &bp.real_tp_ptr);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, real_tp_ptr, ret);
			return ret;
		}
	}

	bp.mac_learning_depth = update_data->mac_learning_depth;
	bp.bridge_me_id = update_data->bridge_id_ptr;
	bp.outbound_td_ptr = update_data->outbound_td_ptr;
	bp.inbound_td_ptr = update_data->inbound_td_ptr;
	bp.bridge_idx = bridge_idx;
	bp.me_id = me_id;

	ret = pon_net_bridge_ifname_get(ctx, bp.bridge_me_id,
					bp.br_ifname, sizeof(bp.br_ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_bridge_ifname_get, ret);
		return ret;
	}

	if (create)
		bp.first = !has_first(&ctx->me_list, bp.tp_type,
				      bp.real_tp_ptr);
	if (create) {
		ret = tp_create(ctx, &bp);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_out_ret("%d", ret);
			return ret;
		}
	} else {
		ret = tp_update(ctx, &bp);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_out_ret("%d", ret);
			return ret;
		}
	}

	ret = pon_net_me_list_write(&ctx->me_list, class_id, me_id,
				    &bp, sizeof(bp));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	if (create) {
		struct pon_net_bridge_port_config *bport =
		    pon_net_me_list_get_data(&ctx->me_list, class_id, me_id);

		if (!bport) {
			FN_ERR_RET(0, pon_net_me_list_get_data,
				   PON_ADAPTER_ERR_NOT_FOUND);
			return PON_ADAPTER_ERR_NOT_FOUND;
		}

		ret = tp_connect(ctx, bport);
		if (ret != PON_ADAPTER_SUCCESS) {
			bp = *bport;
			pon_net_me_list_remove(&ctx->me_list, class_id, me_id);
			tp_destroy(ctx, &bp);

			dbg_out_ret("%d", ret);
			return ret;
		}
	}

	ret = pon_net_police_update_bp(ctx, me_id, NETLINK_FILTER_DIR_INGRESS);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_police_update_bp, ret);
		return ret;
	}
	ret = pon_net_police_update_bp(ctx, me_id, NETLINK_FILTER_DIR_EGRESS);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_police_update_bp, ret);
		return ret;
	}

	ret = update_preassign_tables(ctx, bp.bridge_me_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, update_preassign_tables, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return ret;
}

static enum pon_adapter_errno destroy(void *ll_handle,
	     uint16_t me_id,
	     const struct pa_mac_bp_config_data_destroy_data *destroy_data)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_context *ctx = ll_handle;
	const uint16_t class_id = PON_CLASS_ID_BRIDGE_PORT_CONFIG;
	struct pon_net_bridge_port_config *bp = NULL;
	struct pon_net_bridge_port_config bp_copy = {0};

	dbg_in_args("%p, %u, %p", ll_handle, me_id, destroy_data);

	bp = pon_net_me_list_get_data(&ctx->me_list, class_id, me_id);
	if (!bp) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = tp_disconnect(ctx, bp, true);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, tp_disconnect, ret);
		return ret;
	}

	bp_copy = *bp;

	pon_net_me_list_remove(&ctx->me_list, class_id, me_id);

	/* Destroy expects bp to be removed from me_list */
	ret = tp_destroy(ctx, &bp_copy);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, tp_destroy, ret);
		return ret;
	}

	ret = update_preassign_tables(ctx, bp_copy.bridge_me_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, update_preassign_tables, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno bp_connect(void *ll_handle,
	uint16_t bridge_me_id,
	uint16_t bp_me_id,
	uint8_t tp_type,
	uint16_t tp_ptr)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint32_t bridge_idx;
	const uint16_t class_id = PON_CLASS_ID_BRIDGE_PORT_CONFIG;
	struct pon_net_bridge_port_config *bp;
	const struct termination_point *tp;

	dbg_in_args("%p, %u, %u, %u, %u", ll_handle, bridge_me_id, bp_me_id,
		tp_type, tp_ptr);

	ret = mapper_index_get(ctx->mapper[MAPPER_MACBRIDGE_MEID_TO_IDX],
			       bridge_me_id,
			       &bridge_idx);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, mapper_index_get, ret);
		return ret;
	}

	bp = pon_net_me_list_get_data(&ctx->me_list, class_id, bp_me_id);
	if (!bp) {
		FN_ERR_RET(0, pon_net_me_list_get_data,
			   PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	if (bp->tp_type != tp_type ||
	    bp->tp_ptr != tp_ptr) {
		/* tp_type or tp_ptr_changed. It should not happen. */
		dbg_err("TP type or TP ptr change is not supported\n");
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	if (bp->bridge_me_id != bridge_me_id) {
		/* tp_type or tp_ptr_changed. It should not happen. */
		dbg_err("Bridge change not supported\n");
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	tp = tp_get(bp->tp_type);
	if (tp) {
		if (tp->reconnect) {
			ret = tp->reconnect(ctx, bp);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, tp->reconnect, ret);
				return ret;
			}
		} else {
			ret = tp_disconnect(ctx, bp, false);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, tp_disconnect, ret);
				return ret;
			}

			ret = tp_connect(ctx, bp);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, tp_connect, ret);
				return ret;
			}
		}
	} else {
		dbg_wrn("Unsupported TP type: %u\n", bp->tp_type);
	}

	ret = update_preassign_tables(ctx, bp->bridge_me_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, update_preassign_tables, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

const struct pa_mac_bp_config_data_ops mac_bp_config_data_ops = {
	.update = update,
	.destroy = destroy,
	.connect = bp_connect
};

/** @} */ /* PON_NET_MAC_BRIDGE_PORT_CONFIG_DATA */
