/*****************************************************************************
 *
 * Copyright (c) 2020 MaxLinear, Inc.
 * Copyright (c) 2018 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <pon_adapter.h>

#include "../pon_net_common.h"
#include "../pon_net_debug.h"
#include <omci/me/pon_adapter_vendor_emop.h>

static enum pon_adapter_errno mc_ds_ext_vlan_set(void *ll_handle,
			const uint16_t me_id,
			const uint16_t lan_idx,
			const uint8_t control_mode,
			const uint8_t vlans_num,
			const struct pa_vendor_emop_vlan_trans_table
				*vlan_table)
{
	dbg_in_args("%p, %u, %u, %u, %u, %p", ll_handle, me_id, lan_idx,
		    control_mode, vlans_num, vlan_table);
	/* TODO: Implement this. For now it is here just to avoid SEGFAULTs */

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno mc_ds_ext_vlan_clear(void *ll_handle,
					const uint16_t me_id,
					const uint16_t lan_idx)
{
	dbg_in_args("%p, %u, %u", ll_handle, me_id, lan_idx);
	/* TODO: Implement this. For now it is here just to avoid SEGFAULTs */

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pon_adapter_vendor_emop_ops vendor_emop_ops = {
	.mc_ds_ext_vlan_set = mc_ds_ext_vlan_set,
	.mc_ds_ext_vlan_clear = mc_ds_ext_vlan_clear
};
