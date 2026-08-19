/*
 * Copyright (C) 2020-2021 MaxLinear, Inc.
 * Copyright (C) 2018-2020 Intel Corporation
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
 * Description: PP hal modifier driver
 */

#ifndef __MODIFIER_H__
#define __MODIFIER_H__

#include <linux/types.h>
#include <linux/bitops.h>
#include <linux/init.h>
#include <linux/debugfs.h>

#include "pp_common.h"

/**
 * @struct Modifier module init parameters
 * @valid params valid
 * @mod_base modifier base address
 * @mod_ram_base modifier ram base address
 * @dbgfs debugfs parent directory
 */
struct pp_mod_init_param {
	bool valid;
	u64  mod_base;
	u64  mod_ram_base;
	struct dentry *dbgfs;
};

#ifdef CONFIG_SOC_LGM
/**
 * @enum modifier recipe type
 * @MOD_RCP_CUSTOM custom recipe
 */
enum mod_rcp_type {
	/*! Recipe for no-modifications sessions, mainly for counting
	 *  no-modification sessions on a different counter than exception
	 *  sessions
	 */
	MOD_RCP_NO_MOD,
	/*! Recipe for header replacement ONLY, use mainly when L3 is unknown */
	MOD_RCP_HDR_RPLC,
	/*! Recipe for replacing all layers up to L3 (included) */
	MOD_RCP_HDR_L3_RPLC,
	/*! Recipe for replacing all layers up to L4 (included) */
	MOD_RCP_HDR_L4_RPLC,
	/*! Recipe for nat changes in ipv4 w/o tunnels */
	MOD_RCP_IPV4_NAT,
	/*! Recipe for nat changes in ipv4 and tcp/udp w/o tunnels */
	MOD_RCP_IPV4_NAPT,
	/*! Recipe for ipv6 routed w/o tunnels */
	MOD_RCP_IPV6_ROUTED,
	/*! Recipe for ipv4 to ipv6 translation - MAP-T US */
	MOD_RCP_IPV4_IPV6_TRANS,
	/*! Recipe for ipv6 to ipv4 translation - MAP-T DS */
	MOD_RCP_IPV6_IPV4_TRANS,
	/*! Recipe for encapsulating tunnel for ipv4(inner) over ipv4(outer)
	 *  with nat changes in the inner ipv4 header
	 */
	MOD_RCP_IPV4_IPV4_ENCAP,
	/*! Recipe for encapsulating tunnel for ipv4-esp(inner) over ipv4(outer)
	 *  with nat changes in the inner ipv4 header
	 */
	MOD_RCP_ESP_IPV4_IPV4_ENCAP,
	/*! Recipe for encapsulating tunnel for ipv4(inner) over ipv6(outer)
	 *  with nat changes in the inner ipv4 header
	 */
	MOD_RCP_IPV6_IPV4_ENCAP,
	/*! Recipe for encapsulating tunnel for ipv4-esp(inner) over ipv6(outer)
	 *  with nat changes in the inner ipv4 header
	 */
	MOD_RCP_ESP_IPV6_IPV4_ENCAP,
	/*! Recipe for encapsulating tunnel for ipv6(inner) over ipv6(outer)
	 *  with routed inner ipv6 header
	 */
	MOD_RCP_IPV6_IPV6_ENCAP,
	/*! Recipe for encapsulating tunnel for ipv6(inner) over ipv4(outer)
	 *  with nat changes in the inner ipv6 header
	 */
	MOD_RCP_IPV4_IPV6_ENCAP,
	/*! Recipe for decapsulating tunnel over ipv4/ipv6 with inner
	 *  [ipv4][tcp/udp] packet
	 */
	MOD_RCP_IPV4_OFF3_DECP,
	/*! Recipe for decapsulating tunnel over ipv4/ipv6 with inner
	 *  [ipv4][esp] packet
	 */
	MOD_RCP_IPV4_OFF3_DECP_NAT,
	/*! Recipe for decapsulating IPoGRE tunnel over ipv4/ipv6 with inner
	 *  [ipv4][tcp/udp] packet
	 */
	MOD_RCP_IPOGRE_DECP_IPV4_NAPT,
	/*! Recipe for decapsulating IPoGRE tunnel over ipv4/ipv6 with inner
	 *  [ipv4][esp] packet
	 */
	MOD_RCP_IPOGRE_DECP_IPV4,
	/*! Recipe for decapsulating tunnel over ipv4/ipv6 with inner
	 *  [ipv6][tcp/udp] packet
	 */
	MOD_RCP_IPV6_OFF3_DECP,
	/*! Recipe for decapsulating IPoGRE tunnel over ipv4/ipv6 with inner
	 *  [ipv6][tcp/udp] packet
	 */
	MOD_RCP_IPOGRE_DECP_IPV6_ROUTED,
	/*! Recipe for decapsulating DsLite tunnel with tcp/udp */
	MOD_RCP_DSLITE_DECP_NAPT,
	/*! Recipe for decapsulating 6RD tunnel over ipv4/ipv6 with inner
	 *  [ipv6][tcp/udp] packet
	 */
	MOD_RCP_6RD_DECP_ROUTED,
	/*! Recipe for encapsulating ESP tunnel/transport mode over ipv4(outer)
	 *  without nat changes in the inner headers
	 */
	MOD_RCP_ESP_V4_ENCAP,
	/*! Recipe for encapsulating ESP tunnel/transport mode over ipv4(outer)
	 *  with L3 header replacement and without nat changes in the
	 *  inner headers
	 */
	MOD_RCP_ESP_V4_ENCAP_L3_RPLC,
	/*! Recipe for ESP tunnel/transport mode over ipv4(outer) bypass
	 *	without nat changes, only total length update
	 */
	MOD_RCP_ESP_V4_BPAS,
	/*! Recipe for encapsulating ESP tunnel/transport mode over ipv6(outer)
	 *  without nat changes in the inner headers
	 */
	MOD_RCP_ESP_V6_ENCAP,
	/*! Recipe for encapsulating ESP tunnel/transport mode over ipv6(outer)
	 *  without nat changes in the inner headers
	 */
	MOD_RCP_ESP_V6_ENCAP_L3_RPLC,
	/*! Recipe for ESP tunnel/transport mode over ipv6(outer) bypass
	 *  with L3 header replacement and without nat changes,
	 *  only total length update
	 */
	MOD_RCP_ESP_V6_BPAS,
	/*! Recipe for decapsulating L2TP IP tunnel over ipv4/ipv6 with inner
	 *  [ipv4][tcp/udp] packet
	 */
	MOD_RCP_L2TPIP_DECP_IPV4_NAPT,
	/*! Recipe for decapsulating L2TP IP tunnel over ipv4/ipv6 with inner
	 *  [ipv6][tcp/udp] packet
	 */
	MOD_RCP_L2TPIP_DECP_IPV6_ROUTED,
	/*! Recipe for encapsulating VXLAN IPV4 tunnel over ipv4
	 * [l2][ipv4][tcp/udp] --> [l2][ipv4][udp][vxlan][l2][ipv4][tcp/udp]
	 */
	MOD_RCP_VXLANV4_V4_ENCAP,
	/*! Recipe for encapsulating VXLAN IPV6 tunnel over ipv4
	 * [l2][ipv6][tcp/udp] --> [l2][ipv4][udp][vxlan][l2][ipv6][tcp/udp]
	 */
	MOD_RCP_VXLANV4_V6_ENCAP,
	/*! Recipe for encapsulating VXLAN IPV4 tunnel over ipv6
	 * [l2][ipv4][tcp/udp] --> [l2][ipv6][udp][vxlan][l2][ipv4][tcp/udp]
	 */
	MOD_RCP_VXLANV6_V4_ENCAP,
	/*! Recipe for encapsulating VXLAN IPV6 tunnel over ipv6
	 * [l2][ipv6][tcp/udp] --> [l2][ipv6][udp][vxlan][l2][ipv6][tcp/udp]
	 */
	MOD_RCP_VXLANV6_V6_ENCAP,
	/*! Recipe for encapsulating VXLAN IPV4 tunnel over ipv4 CSUM disbaled
	 * [l2][ipv4][tcp/udp] --> [l2][ipv4][udp][vxlan][l2][ipv4][tcp/udp]
	 */
	MOD_RCP_VXLANV4_V4_ENCAP_NO_CS,
	/*! Recipe for encapsulating VXLAN IPV6 tunnel over ipv4 CSUM disabled
	 * [l2][ipv6][tcp/udp] --> [l2][ipv4][udp][vxlan][l2][ipv6][tcp/udp]
	 */
	MOD_RCP_VXLANV4_V6_ENCAP_NO_CS,

	/*! Recipe for encapsulating UDP tunnel
	 *  [l2][ipv4][tcp/udp] --> [l2][ipv4][udp][tunnel][l2][ipv4][tcp/udp]
	 */
	MOD_RCP_V4_IN_V4_UDP_ENCAP,
	/*! Recipe for encapsulating UDP tunnel
	 *  [l2][ipv4][tcp/udp] --> [l2][ipv6][udp][tunnel][l2][ipv4][tcp/udp]
	 */
	MOD_RCP_V4_IN_V6_UDP_ENCAP,
	/*! Recipe for encapsulating UDP tunnel
	 *  [l2][ipv6][tcp/udp] --> [l2][ipv4][udp][tunnel][l2][ipv6][tcp/udp]
	 */
	MOD_RCP_V6_IN_V4_UDP_ENCAP,
	/*! Recipe for encapsulating UDP tunnel
	 *  [l2][ipv6][tcp/udp] --> [l2][ipv6][udp][tunnel][l2][ipv6][tcp/udp]
	 */
	MOD_RCP_V6_IN_V6_UDP_ENCAP,
	/*! Recipe for encapsulating UDP tunnel with l2tp length update
	 *  [l2][ipv4][tcp/udp] --> [l2][ipv4][udp][tunnel][l2][ipv4][tcp/udp]
	 */
	MOD_RCP_V4_IN_V4_UDP_L2TP_ENCAP,
	/*! Recipe for encapsulating UDP tunnel with l2tp length update
	 *  [l2][ipv4][tcp/udp] --> [l2][ipv6][udp][tunnel][l2][ipv6][tcp/udp]
	 */
	MOD_RCP_V4_IN_V6_UDP_L2TP_ENCAP,
	/*! Recipe for encapsulating UDP tunnel with l2tp length update
	 *  [l2][ipv6][tcp/udp] --> [l2][ipv4][udp][tunnel][l2][ipv4][tcp/udp]
	 */
	MOD_RCP_V6_IN_V4_UDP_L2TP_ENCAP,
	/*! Recipe for encapsulating UDP tunnel with l2tp length update
	 *  [l2][ipv6][tcp/udp] --> [l2][ipv6][udp][tunnel][l2][ipv6][tcp/udp]
	 */
	MOD_RCP_V6_IN_V6_UDP_L2TP_ENCAP,

	/*! Custom recipe for debug, KEEP THIS AS THE LAST RECIPE */
	MOD_RCP_CUSTOM,
	MOD_RCP_LAST    = MOD_RCP_CUSTOM,
	MOD_RCPS_NUM,
	MOD_RCP_NOP     = 0x7F,
	MOD_RCP_IDX_MAX = 0x80,
	MOD_NO_RCP      = 0xFF,
};

/**
 * modification bitmap, keep aligned with mod_flag_str
 */
enum mod_flags {
	/*! IPv4 header exist */
	MOD_IPV4_BIT,
	/*! IPv6 header exist */
	MOD_IPV6_BIT,
	/*! New header IPv4 modification needed, length, ttl, and checksum */
	MOD_NHDR_IPV4_BIT,
	/*! New header IPv6 modification needed, length and hop limit */
	MOD_NHDR_IPV6_BIT,
	/*! New header UDP length and checksum update needed */
	MOD_NHDR_UDP_BIT,
	/*! New header L2TP modifications needed, length */
	MOD_NHDR_L2TP_BIT,
	/*! header replacement, due to mac addresses changes,
	 *  VLAN or/and PPPoE was added/removed to/from the outer header
	 */
	MOD_HDR_RPLC_BIT,
	/*! Replace all layers up to L3 (including) with new header */
	MOD_HDR_L3_RPLC_BIT,
	/*! Replace all layers up to L4 (including) with new header */
	MOD_HDR_L4_RPLC_BIT,
	/*! pppoe was added/removed to/from the inner packet */
	MOD_INNER_PPPOE_BIT,
	/*! IPV4 NAT tos/IP was modified and TTL was decremented */
	MOD_IPV4_NAT_BIT,
	/*! IPV6 hop limit was decremented */
	MOD_IPV6_HOPL_BIT,
	/*! IPv4 to IPv6 translation is needed - MAP-T US */
	MOD_IPV4_IPV6_TRANS_BIT,
	/*! IPv6 to IPv4 translation is needed - MAP-T DS */
	MOD_IPV6_IPV4_TRANS_BIT,
	/*! L4 UDP/TCP was modified */
	MOD_L4_NAPT_BIT,
	/*! ICPM_V4 ID modified */
	MOD_ICMP4_ID_BIT,
	/*! EoGRE tunnel was removed */
	MOD_EOGRE_DECP_BIT,
	/*! EoGRE over IPv4 tunnel was added */
	MOD_EOGRE_V4_ENCP_BIT,
	/*! EoGRE over IPv6 tunnel was added */
	MOD_EOGRE_V6_ENCP_BIT,
	/*! IPoGRE tunnel was removed */
	MOD_IPOGRE_DECP_BIT,
	/*! IPoGRE over IPv4 tunnel was added */
	MOD_IPOGRE_V4_ENCP_BIT,
	/*! IPoGRE over IPv6 tunnel was added */
	MOD_IPOGRE_V6_ENCP_BIT,
	/*! DsLite tunnel was removed */
	MOD_DSLITE_DECP_BIT,
	/*! DsLite tunnel was added */
	MOD_DSLITE_ENCP_BIT,
	/*! Eth over MPLS was removed (ipv4 inner)*/
	MOD_MPLS_V4_DECP_BIT,
	/*! Eth over MPLS was removed (ipv6 inner)*/
	MOD_MPLS_V6_DECP_BIT,
	/*! 6RD tunnel was added */
	MOD_6RD_ENCP_BIT,
	/*! 6RD tunnel was removed */
	MOD_6RD_DECP_BIT,
	/*! ESP tunnel bypass */
	MOD_ESP_TUNNEL_BPAS_BIT,
	/*! ESP transport bypass */
	MOD_ESP_TRANSPORT_BPAS_BIT,
	/*! ESP V4 tunnel was added */
	MOD_ESP_TUNNEL_ENCP_BIT,
	/*! ESP V6 transport was added */
	MOD_ESP_TRANSPORT_ENCP_BIT,
	/*! L2TP V2 UDP was removed */
	MOD_L2TPUDP_DECP_BIT,
	/*! L2TPIP was removed */
	MOD_L2TPIP_DECP_BIT,
	/*! udp tunnel was removed */
	MOD_UDP_DECP_BIT,
	/*! udp tunnel was added */
	MOD_UDP_ENCP_BIT,
	/*! UDP CSUM disabled was added */
	MOD_UDP_CSUM_DISABLE_BIT,

	MOD_FLAG_LAST = MOD_UDP_CSUM_DISABLE_BIT,
	MOD_FLAG_NUM
};

struct mod_stats {
	u32 pkts_rcvd_from_chk;
	u32 pkts_rcvd_from_uc;
	u32 pkts_modified;
};

/**
 * @brief select the recipe type by the modification bitmap
 * @param mod_bmap modification bitmap
 * @param rcp modifier recipe type
 * @return s32 0 on success, error code otherwise
 */
s32 mod_recipe_select(unsigned long mod_bmap, u8 *rcp);

/**
 * @brief Get the packet statistics
 * @param stats
 * @return s32 0 on success, error code otherwise
 */
s32 mod_stats_get(struct mod_stats *stats);

/**
 * @brief Reset modifier statistics
 */
void mod_stats_reset(void);

/**
 * @brief Get modifier recipe description
 * @param rcp_idx recipe index
 * @return const char*const recipe description string, for invalid
 *         indexes "invalid" string is returned
 */
const char * const mod_rcp_to_str(u32 rcp_idx);

/**
 * @brief Get modifier flag description
 * @param flag flag index
 * @return const char*const flag description string, for invalid
 *         indexes "invalid" string is returned
 */
const char * const mod_flag_to_str(u32 flag);

/**
 * @brief Module init function
 * @param init_params initial parameters
 * @return s32 0 on success, error code otherwise
 */
s32 mod_init(struct pp_mod_init_param *init_param);

/**
 * @brief Module exit function, clean all resources
 * @return void
 */
void mod_exit(void);
#else
static inline s32 mod_init(struct pp_mod_init_param *init_param)
{
	if (init_param->valid)
		return -EINVAL;
	else
		return 0;
}

static inline void mod_exit(void)
{
}
#endif /* CONFIG_SOC_LGM */
#endif /* __MODIFIER_H__ */
