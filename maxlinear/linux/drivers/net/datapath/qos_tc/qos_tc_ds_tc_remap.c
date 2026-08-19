// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2019-2020 Intel Corporation
 *
 *****************************************************************************/

#include <linux/netdevice.h>
#include <net/switch_api/gsw_types.h>
#include <net/switch_api/lantiq_gsw.h>
#include <net/switch_api/gsw_dev.h>
#include <net/datapath_api_vlan.h>
#include <net/datapath_api.h>
#include "qos_tc_flower.h"
#include "qos_tc_qos.h"



int qos_tc_mappings_init(void)
{
	struct core_ops *gsw_handle = NULL;
	GSW_return_t ret = 0;

	GSW_QoS_DSCP_ClassCfg_t dscp2tc = {
		.nTrafficClass = {
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
	};
	GSW_QoS_SVLAN_PCP_ClassCfg_t spcp2tc = {
		.nTrafficClass = { 0, 1, 2, 3, 4, 5, 6, 7 },
	};

	gsw_handle = gsw_get_swcore_ops(0);
	if (!gsw_handle) {
		pr_err("%s: Open SWAPI device FAILED!\n", __func__);
		return -EINVAL;
	}

	ret = gsw_handle->gsw_qos_ops.QoS_DSCP_ClassSet(gsw_handle, &dscp2tc);
	if (ret != GSW_statusOk) {
		pr_err("%s: QoS_DSCP_ClassSet failed: %d\n", __func__, ret);
		return ret;
	}

	ret = gsw_handle->gsw_qos_ops.QoS_SVLAN_PCP_ClassSet(gsw_handle,
							     &spcp2tc);
	if (ret != GSW_statusOk) {
		pr_err("%s: QoS_SVLAN_PCP_ClassSet err: %d\n", __func__, ret);
		return ret;
	}

	return ret;
}
