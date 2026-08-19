// SPDX-License-Identifier: BSD-3-Clause OR GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 ******************************************************************************/

#ifndef DP_MIB_COUNTERS
#define DP_MIB_COUNTERS

/*!
 *@brief mib_cnt_get_ethtool_stats_31 - function used by callback
 * dp_net_dev_get_ethtool_stats to retrieve counters' values specyfic for
 * gswip31
 *@param[in] dev: net device
 *@param[out] stats: for dumping NIC-specific statistics
 *@param[out] data: for dumping counters values sequence
 */
void mib_cnt_get_ethtool_stats_31(struct net_device *dev,
				  struct ethtool_stats *stats, u64 *data);

#endif
