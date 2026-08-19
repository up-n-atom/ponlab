/*
 * Copyright (C) 2020-2025 MaxLinear, Inc.
 * Copyright (C) 2020 Intel Corporation
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2, as published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR APARTICULARPURPOSE.See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public
 * License along with this program; if not,see
 * <http://www.gnu.org/licenses/>.
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: Packet header parsing definitions
 */

#ifndef _PKTPRS_INTERNAL_H_
#define _PKTPRS_INTERNAL_H_

struct parse_stats {
	atomic_t proto[PKTPRS_PROTO_LAST + 1];
	atomic_t error_pkt;
	atomic_t level_ovflw;
	atomic_t hdr_buf_ovflw;
	atomic_t vlan_ovflw;
	atomic_t mpls_ovflw;
	atomic_t unsupp_proto;
	atomic_t free_skbext;
	atomic_t num_devices;
	atomic_t hdr_alloc_failed;
	atomic_t unsupp_gre;
	atomic_t custom_hook;
	atomic_t direct_hook;
	atomic_t local_hook;
	atomic_t rx_hook;
	atomic_t tx_hook;
};

/**
 * @brief get pktprs statistics
 * @param stats statistics
 * @return int 0 for success
 */
int pktprs_stats_get(struct parse_stats *stats);

/**
 * @brief print pktprs registered devices
 */
void pktprs_pr_dev(void);

#ifdef CONFIG_DEBUG_FS
void dbg_init(void);
void dbg_clean(void);
#else /* !CONFIG_DEBUG_FS */
static inline dbg_init(void)
{
}
static inline dbg_clean(void)
{
}
#endif /* CONFIG_DEBUG_FS */
#endif /* _PKTPRS_INTERNAL_H_ */
