/*****************************************************************************
 *
 * Copyright (c) 2020 - 2026 MaxLinear, Inc.
 * Copyright (c) 2017 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <pon_adapter.h>

#include "../pon_net_common.h"
#include "../pon_net_debug.h"
#include <omci/me/pon_adapter_mac_bridge_port_bridge_table_data.h>

static enum pon_adapter_errno create(void *ll_handle, uint16_t me_id)
{
	dbg_in_args("%p, %u", ll_handle, me_id);
	/* TODO: Implement this. For now it is here just to avoid SEGFAULTs */

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno destroy(void *ll_handle, uint16_t me_id)
{
	dbg_in_args("%p, %u", ll_handle, me_id);
	/* TODO: Implement this. For now it is here just to avoid SEGFAULTs */

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}
static enum pon_adapter_errno get(
	void *ll_handle,
	uint16_t me_id,
	uint16_t bridge_me_id,
	unsigned int *entry_num,
	struct pa_mac_bp_bridge_table_data_entry **entries)
{
	dbg_in_args("%p, %u, %u, %p, %p", ll_handle, me_id, bridge_me_id,
		    entry_num, entries);
	/* TODO: Implement this. For now it is here just to avoid SEGFAULTs */

	*entry_num = 0;
	*entries = NULL;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno free_table(void *ll_handle, uint16_t me_id,
			uint16_t bridge_me_id,
			unsigned int *entry_num,
			struct pa_mac_bp_bridge_table_data_entry **entries)
{
	dbg_in_args("%p, %u, %u, %p, %p", ll_handle, me_id, bridge_me_id,
		    entry_num, entries);
	dbg_out();

	return PON_ADAPTER_SUCCESS;
}

const struct pa_mac_bp_bridge_table_data_ops mac_bp_bridge_table_data_ops = {
	.create = create,
	.destroy = destroy,
	.get = get,
	.free = free_table
};
