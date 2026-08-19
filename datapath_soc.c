// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2021, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <net/datapath_api.h>
#include "datapath.h"
#include "datapath_instance.h"
#include "datapath_swdev.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

int request_dp(u32 flag)
{
	struct dp_inst_info info;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32) || \
	IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	int i = 0;
	u32 mac_ifcnt = gsw_get_mac_subifcnt(0);
#endif
	struct core_ops *gsw_ops;
	GSW_register_t reg;

	gsw_ops = gsw_get_swcore_ops(0);
	if (!gsw_ops) {
		pr_err("DPM: %s swcore ops is NULL\n", __func__);
		return DP_FAILURE;
	}

	reg.nRegAddr = GSWIP_VER_REG_OFFSET;
	if (gsw_ops->gsw_common_ops.RegisterGet(gsw_ops, &reg))
		return DP_FAILURE;

	if (reg.nData == 0x031) {
		info.type = GSWIP31_TYPE;
		info.ver = GSWIP31_VER;
	} else if (reg.nData == 0x032) {
		info.type = GSWIP32_TYPE;
		info.ver = GSWIP32_VER;
	} else if (reg.nData == 0x030) {
		info.type = GSWIP30_TYPE;
		info.ver = GSWIP30_VER;
	}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32) || \
	IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	info.ops[0] = gsw_get_swcore_ops(0);
	info.ops[1] = gsw_get_swcore_ops(0);
	if ((mac_ifcnt + 2) > ARRAY_SIZE(info.mac_ops)) {
		pr_err("DPM: %s mac count more than defined size\n", __func__);
		return DP_FAILURE;
	}
	info.mac_ops[0] = NULL;
	info.mac_ops[1] = NULL;
	for (i = 0; i < mac_ifcnt; i++)
		info.mac_ops[i + 2] = gsw_get_mac_ops(0, i + 2);
#else
	info.ops[0] = gsw_get_swcore_ops(0);
	info.ops[1] = gsw_get_swcore_ops(1);
#endif
	info.cbm_inst = 0;
	info.qos_inst = 0;
	if (dp_request_inst(&info, flag)) {
		pr_err("DPM: dp_request_inst failed\n");
		return -1;
	}
	return 0;
}

int register_dp_cap(u32 flag)
{
	register_dp_capability(0);
	return 0;
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP