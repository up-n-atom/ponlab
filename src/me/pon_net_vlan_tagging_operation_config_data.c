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
#include <pon_adapter.h>

#include "../pon_net_common.h"
#include "../pon_net_debug.h"
#include "../pon_net_netlink.h"
#include <omci/me/pon_adapter_vlan_tagging_operation_config_data.h>

static enum pon_adapter_errno
vlan_tagging_operation_config_data_update(void *ll_handle,
	uint16_t me_id,
	const struct pa_vlan_tagging_operation_config_data_update_data
		*update_data)
{
	/* struct pon_net_context *ctx = ll_handle; */
	/* enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS; */

	dbg_in_args("%p, %u, %p", ll_handle, me_id, update_data);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
vlan_tagging_operation_config_data_destroy(void *ll_handle,
					   const uint16_t me_id)
{
	/* struct pon_net_context *ctx = ll_handle; */

	dbg_in_args("%p, %u", ll_handle, me_id);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pa_vlan_tagging_operation_config_data_ops
vlan_tagging_operation_config_data_ops = {
	.update = vlan_tagging_operation_config_data_update,
	.destroy = vlan_tagging_operation_config_data_destroy
};
