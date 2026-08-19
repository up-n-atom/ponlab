// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2024, MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/
#include <net/datapath_api.h>
#include "../datapath.h"
#include "datapath_misc.h"

int dp_get_port_vap_mib(dp_subif_t *subif, void *priv,
			struct rtnl_link_stats64 *net_mib, u32 flag)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
	struct core_ops *gsw_ops = dp_port_prop[0].ops[0];
	GSW_RMON_CTP_cnt_t ctp_cnt;
	int i;
	u16 ctp_base, max_ctp, vap;
	struct pmac_port_info *port_info;

	if (!net_mib || !subif ||  subif->port_id  < 0 ||
	    subif->port_id >= dp_port_prop[0].info.cap.max_num_dp_ports)
		return DP_FAILURE;

	port_info = get_dp_port_info(0,  subif->port_id);
	DP_CB(0, get_itf_start_end)(port_info->itf_info, &ctp_base, &max_ctp);
	memset(net_mib, 0, sizeof(*net_mib));
	for (i = 0; i < subif->subif_num; i++) {
		vap = GET_VAP(subif->subif_list[i], port_info->vap_offset, port_info->vap_mask);
		ctp_cnt.nCTP = ctp_base + vap;
		if (gsw_ops->gsw_rmon_ops.RMON_CTP_Get(gsw_ops, &ctp_cnt)) {
			pr_err("DPM: Failed RMON_CTP_Get for %d\n", ctp_cnt.nCTP);
			return DP_FAILURE;
		}

		net_mib->rx_packets += ctp_cnt.nRxPkts;
		net_mib->tx_packets += ctp_cnt.nTxPkts;
		net_mib->rx_bytes += ctp_cnt.nRxBytes;
		net_mib->tx_bytes += ctp_cnt.nTxBytes;
		net_mib->rx_errors += ctp_cnt.nRxErrors;
		net_mib->rx_dropped += ctp_cnt.nRxDrops;
		net_mib->tx_dropped += ctp_cnt.nTxDrops;
	}

	return DP_SUCCESS;
#else
	return DP_FAILURE;
#endif
}