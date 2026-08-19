/*****************************************************************************
 *
 * Copyright (c) 2020 - 2021 MaxLinear, Inc.
 * Copyright (c) 2019 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <pon_adapter.h>

#include "../pon_net_common.h"
#include "../pon_net_debug.h"
#include "../pon_net_netlink.h"
#include "../pon_net_ethtool.h"
#include "../pon_net_uni.h"
#include <omci/me/pon_adapter_virtual_ethernet_interface_point.h>
#include <omci/me/pon_adapter_mac_bridge_port_config_data.h>

static enum pon_adapter_errno
virtual_eth_interface_point_create(void *ll_handle, uint16_t me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u", ll_handle, me_id);

	ret = pon_net_uni_create(ll_handle, PON_CLASS_ID_VEIP, me_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_uni_create, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
virtual_eth_interface_point_destroy(void *ll_handle, uint16_t me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u", ll_handle, me_id);

	ret = pon_net_uni_destroy(ll_handle, PON_CLASS_ID_VEIP, me_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_uni_destroy, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
virtual_eth_interface_point_admin_state_lock(void *ll_handle, uint16_t me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u", ll_handle, me_id);

	ret = pon_net_tp_state_set(ll_handle, PA_BP_TP_TYPE_VEIP, me_id,
				   ADMIN_STATE_LOCKED);

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
virtual_eth_interface_point_admin_state_unlock(void *ll_handle, uint16_t me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u", ll_handle, me_id);

	ret = pon_net_tp_state_set(ll_handle, PA_BP_TP_TYPE_VEIP, me_id,
				   ADMIN_STATE_UNLOCKED);

	dbg_out_ret("%d", ret);
	return ret;
}

const struct pa_virtual_ethernet_interface_point_ops
	virtual_ethernet_interface_point_ops = {
		.lock = virtual_eth_interface_point_admin_state_lock,
		.unlock = virtual_eth_interface_point_admin_state_unlock,
		.create = virtual_eth_interface_point_create,
		.destroy = virtual_eth_interface_point_destroy,
};
