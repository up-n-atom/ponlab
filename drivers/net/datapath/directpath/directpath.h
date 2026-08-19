// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2023-2024 MaxLinear, Inc.
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

#ifndef DIRECTPATH_H
#define DIRECTPATH_H

/*! @brief DPDP device port specific information */
struct dpdp_port_info {
	struct module		*owner;
	struct net_device	*dev;
	int			dp_port;
	int			port_active:1;
	struct dp_cb		dp_ops;
	int			max_subif;
	refcount_t		port_users;
	refcount_t		users;
};

/*! @brief DPDP device statistics */
struct dpdp_stats {
	u64	rx_fn_rxif_pkts;
	u64	rx_fn_rxif_drop;
	u64	rx_fn_txif_pkts;
	u64	rx_fn_txif_drop;
	u64	tx_fwd_offload_pkts;
	u64	tx_fwd_offload_errs;
	u64	tx_qos_offload_pkts;
	u64	tx_qos_offload_errs;
};

/*! @brief DPDP device structure for a network device */
struct dpdp_device {
	struct hlist_node	hlist;
	struct rcu_head		rcu;
	struct net_device	*dev;
	u8			dummy_laddr[ETH_ALEN]; /* dummy local address */
	u8			dummy_raddr[ETH_ALEN]; /* dummy remote address */
	netdev_features_t	delta_features;
	struct dp_subif		dp_subif;
	int			subif_active:1;
	struct dpdp_stats __percpu *stats;
	struct dpdp_port_info	*pinfo;
	refcount_t		users;
};

typedef void dump_cb_t(struct dpdp_device *dp_dev, int bkt, void *data);
void dpdp_device_dump(dump_cb_t *cb, void *data);

void directpath_debugfs_init(void);
void directpath_debugfs_exit(void);

#endif /* DIRECTPATH_API_H */
