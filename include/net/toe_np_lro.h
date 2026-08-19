/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2018-2020 Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TOE_NP_LRO_H_VT4QZ6TX
#define TOE_NP_LRO_H_VT4QZ6TX

#include <net/datapath_api.h>

struct toe_dev;

enum LRO_TYPE {
	LRO_TYPE_TCP,
	LRO_TYPE_UDP,
	LRO_TYPE_MPTCP,
	LRO_TYPE_TCPV6,
	LRO_TYPE_UDPV6,
	LRO_TYPE_MPTCPV6,
	LRO_TYPE_MAX,
};

/**
 * struct lro_ops - LRO LRO functions
 * @toe: toe device
 * @lro_start: start LRO fid
 * @lro_stop: stop LRO fid
 * @lro_set_threshold: aggregation when payload size larger than the threshold
 * @lro_set_timeout: LRO timeout, default is 10milliseconds
 * @lro_get_cap: supported packet types, bitmap of LRO_TYPE
 *
 * TOE driver will call dp_register_lro() to register all LRO ops.
 * TSO functions will be registered by calling dp_register_txpath().
 *
 */
struct lro_ops {
	struct toe_dev *toe;

	int (*lro_start)(struct toe_dev *toe, int fid, enum LRO_TYPE type);

	void (*lro_stop)(struct toe_dev *toe, int fid);

	void (*lro_set_threshold)(struct toe_dev *toe, u16 tcp_payload_len);

	void (*lro_set_timeout)(struct toe_dev *toe, u32 microseconds);

	unsigned long (*lro_get_cap)(struct toe_dev *toe);

	unsigned int (*get_gso_max_size)(struct toe_dev *toe);

	unsigned int (*get_nr_txq)(struct toe_dev *toe);

	int (*find_free_lro_port)(struct toe_dev *toe,
				  const struct cpumask *cpumask);

	int (*cfg_netdev_feature)(struct toe_dev *toe, struct net_device *dev,
				  bool en);
};

/**
 * dp_register_lro_ops() - register LRO ops
 */
static inline void dp_register_lro_ops(struct lro_ops *ops)
{
	dp_register_ops(0, DP_OPS_LRO, ops);
}

static inline struct lro_ops *dp_get_lro_ops(void)
{
	return dp_get_ops(0, DP_OPS_LRO);
}
#endif /* end of include guard: TOE_NP_LRO_H_VT4QZ6TX */
