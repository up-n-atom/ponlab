/*
 * Copyright (C) 2021-2025 MaxLinear, Inc.
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
 * Description: dpl h file
 */

#ifndef _DPL_H_
#define _DPL_H_

#include <linux/types.h>

struct net_device;

#define DPL_VOICE_DATA_PRIO 0
#define DPL_VOICE_CRTL_PRIO 0
#define DPL_CTRL_PRIO       2
#define DPL_MNGMNT_PRIO     4

/**
 * @brief Enumeration for PP white list fields types
 */
enum dpl_whitelist_field_type {
	DPL_FLD_SRC_MAC,
	DPL_FLD_DST_MAC,
	DPL_FLD_ETH_TYPE,
	DPL_FLD_IPV4_SRC_IP,
	DPL_FLD_IPV4_DST_IP,
	DPL_FLD_IPV6_SRC_IP,
	DPL_FLD_IPV6_DST_IP,
	DPL_FLD_IP_PROTO,
	DPL_FLD_L4_SRC_PORT,
	DPL_FLD_L4_DST_PORT,
	DPL_FLD_ICMP_TYPE_CODE,
	DPL_FLD_COUNT
};

struct dpl_whitelist_field {
	enum dpl_whitelist_field_type type;
	union {
		u8 ip_proto; /*! ip/ipv6 next protocol */
		u8 dst_mac[ETH_ALEN]; /*! destination MAC address !*/
		u8 src_mac[ETH_ALEN]; /*! source MAC address !*/
		__be16 eth_type; /*! ethernet type */
		__be16 src_port; /*! L4 source port */
		__be16 dst_port; /*! L4 destination port */
		__be16 icmp_type_code; /*! icmp type & code */
		union {
			struct in6_addr v6; /*! ipv6 source address */
			__be32 v4; /*! ipv4 source address */
		} src_ip;
		union {
			struct in6_addr v6; /*! ipv6 destination address */
			__be32 v4; /*! ipv4 destination address */
		} dst_ip;
	};
};

/**
 * @brief Add net_device to DPL to protect the device against attack
 * @param dev net_device to protect
 * @return int 0 on success, error code otherwise
 */
int dpl_dev_protect(struct net_device *dev);

/**
 * @brief remove the net_device from DPL
 * @param dev net_device to unprotect
 * @return int 0 on success, error code otherwise
 */
int dpl_dev_unprotect(struct net_device *dev);

/**
 * @brief Add new white list rule to prioritize important packets
 *        over regular packets on the host path
 * @param prio priority, used for choosing destination queue for packets
 *             who matched the rule.
 *             valid values are from 0 (highest) to 7 (lowest)
 * @param fields fields array to compare to the the packets
 *               e.g. ip address, udp port, mac address etc...
 * @param cnt number of fields
 *
 * White rules are used to protect the host when a DoS attack occurs
 * by prioritizing packets that was matched to one of the rules
 * to queues with high priority
 * @return int 0 on success, error code otherwise
 */
int dpl_whitelist_rule_add(unsigned int prio, struct dpl_whitelist_field *fields,
			   unsigned int cnt);

/**
 * @brief Delete an existing white rule, imput parameters here
 *        should be the exact ones that were used in dpl_whitelist_rule_add
 * @param prio priority
 * @param fields fields array
 * @param cnt number of fields
 * @return int 0 on success, error code otherwise
 */
int dpl_whitelist_rule_del(unsigned int prio, struct dpl_whitelist_field *fields,
			   unsigned int cnt);

#endif