/*
 * Copyright (C) 2020-2024 MaxLinear, Inc.
 * Copyright (C) 2018-2020 Intel Corporation
 *
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
 * Description: PP hal modifier recipes
 */

#define pr_fmt(fmt) "[PP_MODIFIER_RECIPES]: %s:%d: " fmt, __func__, __LINE__

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/if_pppox.h>
#include <linux/tcp.h>
#include <linux/udp.h>

#include "pp_common.h"
#include "parser_protocols.h"
#include "modifier.h"
#include "modifier_internal.h"

/* flags for better code readability */
#define DO_L2TP_LEN true
#define DO_NAPT true
#define DO_PPPOE_ENCAP true

/* supported recipes description */
static const char * const mod_rcp_str[MOD_RCPS_NUM + 2] = {
	[MOD_RCP_NO_MOD]                  = "No modification",
	[MOD_RCP_HDR_RPLC]                = "Header replacement",
	[MOD_RCP_HDR_L3_RPLC]             = "Header L3 replacement",
	[MOD_RCP_HDR_L4_RPLC]             = "Header L4 replacement",
	[MOD_RCP_IPV4_NAT]                = "IPv4 NAT",
	[MOD_RCP_IPV4_NAPT]               = "IPv4 NAPT",
	[MOD_RCP_IPV6_ROUTED]             = "IPv6 routed",
	[MOD_RCP_IPV4_IPV6_TRANS]         = "IPv4 to IPv6 Translation",
	[MOD_RCP_IPV6_IPV4_TRANS]         = "IPv6 to IPv4 Translation",
	[MOD_RCP_IPV4_IPV4_ENCAP]         = "ipv4 over ipv4 encapsulation",
	[MOD_RCP_ESP_IPV4_IPV4_ENCAP]     = "ESP bypass ipv4 over ipv4 encapsulation",
	[MOD_RCP_IPV6_IPV4_ENCAP]         = "ipv4 over ipv6 encapsulation",
	[MOD_RCP_ESP_IPV6_IPV4_ENCAP]     = "ESP bypass ipv4 over ipv6 encapsulation",
	[MOD_RCP_IPV6_IPV6_ENCAP]         = "ipv6 over ipv6 encapsulation",
	[MOD_RCP_IPV4_IPV6_ENCAP]         = "ipv6 over ipv4 encapsulation",
	[MOD_RCP_IPV4_OFF3_DECP]          = "L3 offset decap inner ipv4 NAPT",
	[MOD_RCP_IPV4_OFF3_DECP_NAT]      = "L3 offset decap inner ipv4 NAT",
	[MOD_RCP_IPOGRE_DECP_IPV4_NAPT]   = "IPoGRE decap inner ipv4 NAPT",
	[MOD_RCP_IPOGRE_DECP_IPV4]        = "IPoGRE decap inner esp",
	[MOD_RCP_IPV6_OFF3_DECP]          = "L3 offset decap inner ipv6 NAPT",
	[MOD_RCP_IPOGRE_DECP_IPV6_ROUTED] = "IPoGRE decap inner ipv6 NAPT",
	[MOD_RCP_DSLITE_DECP_NAPT]        = "DsLite decap NAPT",
	[MOD_RCP_6RD_DECP_ROUTED]         = "6RD decap",
	[MOD_RCP_ESP_V4_ENCAP]            = "ESP over ipv4 encapsulation",
	[MOD_RCP_ESP_V4_ENCAP_L3_RPLC]    = "ESP over ipv4 encapsulation L3 replacement",
	[MOD_RCP_ESP_V4_BPAS]             = "ESP over ipv4 bypass",
	[MOD_RCP_ESP_V6_ENCAP]            = "ESP over ipv6 encapsulation",
	[MOD_RCP_ESP_V6_ENCAP_L3_RPLC]    = "ESP over ipv6 encapsulation L3 replacement",
	[MOD_RCP_ESP_V6_BPAS]             = "ESP over ipv6 bypass",
	[MOD_RCP_L2TPIP_DECP_IPV4_NAPT]   = "L2TP IP decap inner ipv4 NAPT",
	[MOD_RCP_L2TPIP_DECP_IPV6_ROUTED] = "L2TP IP decap inner ipv6 NAPT",
	[MOD_RCP_VXLANV4_V4_ENCAP]        = "VXLAN_V4 V4 encap",
	[MOD_RCP_VXLANV4_V6_ENCAP]        = "VXLAN_V4 V6 encap",
	[MOD_RCP_VXLANV6_V4_ENCAP]        = "VXLAN_V6 V4 encap",
	[MOD_RCP_VXLANV6_V6_ENCAP]        = "VXLAN_V6 V6 encap",
	[MOD_RCP_VXLANV4_V4_ENCAP_NO_CS]  = "VXLAN_V4 V4 encap no csum",
	[MOD_RCP_VXLANV4_V6_ENCAP_NO_CS]  = "VXLAN_V4 V6 encap no csum",
	[MOD_RCP_V4_IN_V4_UDP_ENCAP]      = "V4 in V4 UDP encap",
	[MOD_RCP_V4_IN_V6_UDP_ENCAP]      = "V4 in V6 UDP encap",
	[MOD_RCP_V6_IN_V4_UDP_ENCAP]      = "V6 in V4 UDP encap",
	[MOD_RCP_V6_IN_V6_UDP_ENCAP]      = "V6 in V6 UDP encap",
	[MOD_RCP_V4_IN_V4_UDP_L2TP_ENCAP] = "V4 in V4 UDP with L2TP Len encap",
	[MOD_RCP_V4_IN_V6_UDP_L2TP_ENCAP] = "V4 in V6 UDP with L2TP Len encap",
	[MOD_RCP_V6_IN_V4_UDP_L2TP_ENCAP] = "V6 in V4 UDP with L2TP Len encap",
	[MOD_RCP_V6_IN_V6_UDP_L2TP_ENCAP] = "V6 in V6 UDP with L2TP Len encap",

	[MOD_RCP_CUSTOM]                  = "CUSTOM",
	[MOD_RCPS_NUM]                    = "NOP",
	[MOD_RCPS_NUM + 1]                = "INVALID",
};

/* modification flags description */
static const char * const mod_flag_str[MOD_FLAG_NUM + 1] = {
	[MOD_IPV4_BIT]               = "IPV4",
	[MOD_IPV6_BIT]               = "IPV6",
	[MOD_NHDR_IPV4_BIT]          = "NHDR_IPV4",
	[MOD_NHDR_IPV6_BIT]          = "NHDR_IPV6",
	[MOD_NHDR_UDP_BIT]           = "NHDR_UDP",
	[MOD_NHDR_L2TP_BIT]          = "NHDR_L2TP",
	[MOD_HDR_RPLC_BIT]           = "HDR RPLC",
	[MOD_HDR_L3_RPLC_BIT]        = "HDR L3 RPLC",
	[MOD_HDR_L4_RPLC_BIT]        = "HDR L4 RPLC",
	[MOD_INNER_PPPOE_BIT]        = "PPPoE INNER",
	[MOD_IPV4_NAT_BIT]           = "IPv4 NAT",
	[MOD_IPV6_HOPL_BIT]          = "IPv6 HOPL",
	[MOD_IPV4_IPV6_TRANS_BIT]    = "IPv4 to IPv6 Trans",
	[MOD_IPV6_IPV4_TRANS_BIT]    = "IPv6 to IPv4 Trans",
	[MOD_L4_NAPT_BIT]            = "L4 NAPT",
	[MOD_ICMP4_ID_BIT]           = "ICMPv4 ID",
	[MOD_EOGRE_DECP_BIT]         = "EoGRE decap",
	[MOD_EOGRE_V4_ENCP_BIT]      = "EoGREv4 encap",
	[MOD_EOGRE_V6_ENCP_BIT]      = "EoGREv6 encap",
	[MOD_IPOGRE_DECP_BIT]        = "IPoGRE decap",
	[MOD_IPOGRE_V4_ENCP_BIT]     = "IPoGREv4 encap",
	[MOD_IPOGRE_V6_ENCP_BIT]     = "IPoGREv6 encap",
	[MOD_DSLITE_DECP_BIT]        = "DsLite decap",
	[MOD_DSLITE_ENCP_BIT]        = "DsLite encap",
	[MOD_MPLS_V4_DECP_BIT]       = "EoMPLS IPv4 decap",
	[MOD_MPLS_V6_DECP_BIT]       = "EoMPLS IPv6 decap",
	[MOD_6RD_ENCP_BIT]           = "6RD encap",
	[MOD_6RD_DECP_BIT]           = "6RD decap",
	[MOD_ESP_TUNNEL_BPAS_BIT]    = "ESP tunnel bypass",
	[MOD_ESP_TRANSPORT_BPAS_BIT] = "ESP transport bypass",
	[MOD_ESP_TUNNEL_ENCP_BIT]    = "ESP tunnel encap",
	[MOD_ESP_TRANSPORT_ENCP_BIT] = "ESP transport encap",
	[MOD_L2TPUDP_DECP_BIT]       = "L2TPUDP decap",
	[MOD_L2TPIP_DECP_BIT]        = "L2TPIP decap",
	[MOD_UDP_DECP_BIT]           = "UDP decap",
	[MOD_UDP_ENCP_BIT]           = "UDP encap",
	[MOD_FLAG_NUM]               = "INVALID",
};

#define MOD_IPV4_MSK               BIT(MOD_IPV4_BIT)
#define MOD_IPV6_MSK               BIT(MOD_IPV6_BIT)
#define MOD_NHDR_IPV4_MSK          BIT(MOD_NHDR_IPV4_BIT)
#define MOD_NHDR_IPV6_MSK          BIT(MOD_NHDR_IPV6_BIT)
#define MOD_NHDR_UDP_MSK           BIT(MOD_NHDR_UDP_BIT)
#define MOD_NHDR_L2TP_MSK          BIT(MOD_NHDR_L2TP_BIT)
#define MOD_HDR_RPLC_MSK           BIT(MOD_HDR_RPLC_BIT)
#define MOD_HDR_L3_RPLC_MSK        BIT(MOD_HDR_L3_RPLC_BIT)
#define MOD_HDR_L4_RPLC_MSK        BIT(MOD_HDR_L4_RPLC_BIT)
#define MOD_INNER_PPPOE_MSK        BIT(MOD_INNER_PPPOE_BIT)
#define MOD_IPV4_NAT_MSK           BIT(MOD_IPV4_NAT_BIT)
#define MOD_IPV6_HOPL_MSK          BIT(MOD_IPV6_HOPL_BIT)
#define MOD_IPV4_IPV6_TRANS_MSK    BIT(MOD_IPV4_IPV6_TRANS_BIT)
#define MOD_IPV6_IPV4_TRANS_MSK    BIT(MOD_IPV6_IPV4_TRANS_BIT)
#define MOD_L4_NAPT_MSK            BIT(MOD_L4_NAPT_BIT)
#define MOD_ICMP4_ID_MSK           BIT(MOD_ICMP4_ID_BIT)
#define MOD_EOGRE_DECP_MSK         BIT(MOD_EOGRE_DECP_BIT)
#define MOD_EOGRE_V4_ENCP_MSK      BIT(MOD_EOGRE_V4_ENCP_BIT)
#define MOD_EOGRE_V6_ENCP_MSK      BIT(MOD_EOGRE_V6_ENCP_BIT)
#define MOD_IPOGRE_DECP_MSK        BIT(MOD_IPOGRE_DECP_BIT)
#define MOD_IPOGRE_V4_ENCP_MSK     BIT(MOD_IPOGRE_V4_ENCP_BIT)
#define MOD_IPOGRE_V6_ENCP_MSK     BIT(MOD_IPOGRE_V6_ENCP_BIT)
#define MOD_DSLITE_DECP_MSK        BIT(MOD_DSLITE_DECP_BIT)
#define MOD_DSLITE_ENCP_MSK        BIT(MOD_DSLITE_ENCP_BIT)
#define MOD_MPLS_V4_DECP_MSK	   BIT(MOD_MPLS_V4_DECP_BIT)
#define MOD_MPLS_V6_DECP_MSK	   BIT(MOD_MPLS_V6_DECP_BIT)
#define MOD_6RD_ENCP_MSK           BIT(MOD_6RD_ENCP_BIT)
#define MOD_6RD_DECP_MSK           BIT(MOD_6RD_DECP_BIT)
#define MOD_ESP_TUNNEL_BPAS_MSK    BIT(MOD_ESP_TUNNEL_BPAS_BIT)
#define MOD_ESP_TRANSPORT_BPAS_MSK BIT(MOD_ESP_TRANSPORT_BPAS_BIT)
#define MOD_ESP_TUNNEL_ENCP_MSK    BIT(MOD_ESP_TUNNEL_ENCP_BIT)
#define MOD_ESP_TRANSPORT_ENCP_MSK BIT(MOD_ESP_TRANSPORT_ENCP_BIT)
#define MOD_L2TPUDP_DECP_MSK       BIT(MOD_L2TPUDP_DECP_BIT)
#define MOD_L2TPIP_DECP_MSK        BIT(MOD_L2TPIP_DECP_BIT)
#define MOD_UDP_ENCAP_MSK          BIT(MOD_UDP_ENCP_BIT)
#define MOD_UDP_DECAP_MSK          BIT(MOD_UDP_DECP_BIT)
#define MOD_UDP_CSUM_DISABLE_MSK   BIT(MOD_UDP_CSUM_DISABLE_BIT)

enum rcp_hdr_ip_type {
	RCP_HDR_IPV4,
	RCP_HDR_IPV6,
	RCP_HDR_NO_IP,
};

/**
 * @brief Supported modification definition
 */
struct supp_mod {
	ulong flags;  /*! modification flags */
	u8    recipe; /*! recipe id */
};

/**
 * @brief Array of all supported modification types
 */
static const struct supp_mod supp_mod_types[] = {
	{ /* no modification */
		.flags  = 0,
		.recipe = MOD_RCP_NO_MOD
	},
	{ /* Header replacement */
		.flags  = MOD_HDR_RPLC_MSK,
		.recipe = MOD_RCP_HDR_RPLC
	},
	{ /* Header L3 replacement */
		.flags  = MOD_HDR_L3_RPLC_MSK,
		.recipe = MOD_RCP_HDR_L3_RPLC
	},
	{ /* Header L4 replacement */
		.flags  = MOD_HDR_L4_RPLC_MSK,
		.recipe = MOD_RCP_HDR_L4_RPLC
	},
	{ /* IPv4 NAT */
		.flags  = MOD_IPV4_NAT_MSK,
		.recipe = MOD_RCP_IPV4_NAT
	},
	{ /* IPv4 NAPT */
		.flags  = MOD_IPV4_NAT_MSK | MOD_L4_NAPT_MSK,
		.recipe = MOD_RCP_IPV4_NAPT
	},
	{ /* IPv6 NAT */
		.flags  = MOD_IPV6_HOPL_MSK,
		.recipe = MOD_RCP_IPV6_ROUTED
	},
	/* ================================================================ */
	/*          IPv4(inner) over IPv6(outer) Encapsulation              */
	/* ================================================================ */
	{ /* DsLite tunnel encapsulation */
		.flags  = MOD_DSLITE_ENCP_MSK | MOD_IPV4_NAT_MSK |
			  MOD_L4_NAPT_MSK,
		.recipe = MOD_RCP_IPV6_IPV4_ENCAP
	},
	{ /* ETH IPv4 over GRE over IPv6 encapsulation */
		.flags  = MOD_EOGRE_V6_ENCP_MSK | MOD_IPV4_NAT_MSK |
			  MOD_L4_NAPT_MSK,
		.recipe = MOD_RCP_IPV6_IPV4_ENCAP
	},
	{ /* IPv4 over GRE over IPv6 encapsulation */
		.flags  = MOD_IPOGRE_V6_ENCP_MSK | MOD_IPV4_NAT_MSK |
			  MOD_L4_NAPT_MSK,
		.recipe = MOD_RCP_IPV6_IPV4_ENCAP
	},
	{ /* ETH IPv4 over GRE over IPv6 encapsulation */
		.flags  = MOD_EOGRE_V6_ENCP_MSK | MOD_IPV4_NAT_MSK,
		.recipe = MOD_RCP_ESP_IPV6_IPV4_ENCAP
	},
	{ /* IPv4 over GRE over IPv6 encapsulation */
		.flags  = MOD_IPOGRE_V6_ENCP_MSK | MOD_IPV4_NAT_MSK,
		.recipe = MOD_RCP_ESP_IPV6_IPV4_ENCAP
	},
	/* ================================================================ */
	/*          IPv6(inner) over IPv6(outer) Encapsulation              */
	/* ================================================================ */
	{ /* ETH IPv6 over GRE over IPv6 encapsulation */
		.flags  = MOD_EOGRE_V6_ENCP_MSK | MOD_IPV6_HOPL_MSK,
		.recipe = MOD_RCP_IPV6_IPV6_ENCAP
	},
	{ /* IPv6 over GRE over IPv6 encapsulation */
		.flags  = MOD_IPOGRE_V6_ENCP_MSK | MOD_IPV6_HOPL_MSK,
		.recipe = MOD_RCP_IPV6_IPV6_ENCAP
	},
	/* ================================================================ */
	/*          IPv6(inner) over IPv4(outer) Encapsulation              */
	/* ================================================================ */
	{ /* 6RD tunnel encapsulation */
		.flags	= MOD_6RD_ENCP_MSK | MOD_IPV6_HOPL_MSK,
		.recipe = MOD_RCP_IPV4_IPV6_ENCAP
	},
	{ /* IPv6 over GRE over IPv4 encapsulation */
		.flags  = MOD_IPOGRE_V4_ENCP_MSK | MOD_IPV6_HOPL_MSK,
		.recipe = MOD_RCP_IPV4_IPV6_ENCAP
	},
	{ /* ETH IPv6 over GRE over IPv4 encapsulation */
		.flags  = MOD_EOGRE_V4_ENCP_MSK | MOD_IPV6_HOPL_MSK,
		.recipe = MOD_RCP_IPV4_IPV6_ENCAP
	},
	/* ================================================================ */
	/*          IPv4(inner) over IPv4(outer) Encapsulation              */
	/* ================================================================ */
	{ /* IPv4 over GRE over IPv4 encapsulation */
		.flags  = MOD_IPOGRE_V4_ENCP_MSK | MOD_IPV4_NAT_MSK |
			  MOD_L4_NAPT_MSK,
		.recipe = MOD_RCP_IPV4_IPV4_ENCAP
	},
	{ /* ETH IPv4 over GRE over IPv4 encapsulation */
		.flags  = MOD_EOGRE_V4_ENCP_MSK | MOD_IPV4_NAT_MSK |
			  MOD_L4_NAPT_MSK,
		.recipe = MOD_RCP_IPV4_IPV4_ENCAP
	},
	{ /* IPv4 over GRE over IPv4 encapsulation */
		.flags  = MOD_IPOGRE_V4_ENCP_MSK | MOD_IPV4_NAT_MSK,
		.recipe = MOD_RCP_ESP_IPV4_IPV4_ENCAP
	},
	{ /* ETH IPv4 over GRE over IPv4 encapsulation */
		.flags  = MOD_EOGRE_V4_ENCP_MSK | MOD_IPV4_NAT_MSK,
		.recipe = MOD_RCP_ESP_IPV4_IPV4_ENCAP
	},
	/* ================================================================ */
	/*         ESP tunnel mode over IPv4(outer) Encapsulation           */
	/* ================================================================ */
	{
		.flags  = MOD_ESP_TUNNEL_ENCP_MSK | MOD_HDR_RPLC_MSK |
			  MOD_IPV4_MSK,
		.recipe = MOD_RCP_ESP_V4_ENCAP
	},
	/* ================================================================ */
	/*         ESP tunnel mode over IPv4(outer) Bypass                  */
	/* ================================================================ */
	{
		.flags  = MOD_ESP_TUNNEL_BPAS_MSK | MOD_HDR_RPLC_MSK |
			  MOD_IPV4_MSK,
		.recipe = MOD_RCP_ESP_V4_BPAS
	},
	/* ================================================================ */
	/*         ESP tunnel mode over IPv6(outer) Encapsulation           */
	/* ================================================================ */
	{
		.flags  = MOD_ESP_TUNNEL_ENCP_MSK | MOD_HDR_RPLC_MSK |
			  MOD_IPV6_MSK,
		.recipe = MOD_RCP_ESP_V6_ENCAP
	},
	/* ================================================================ */
	/*         ESP tunnel mode over IPv6(outer) Bypass                  */
	/* ================================================================ */
	{
		.flags  = MOD_ESP_TUNNEL_BPAS_MSK | MOD_HDR_RPLC_MSK |
			  MOD_IPV6_MSK,
		.recipe = MOD_RCP_ESP_V6_BPAS
	},
	/* ================================================================ */
	/*         ESP transport mode over IPv4(outer) Bypass               */
	/* ================================================================ */
	{
		.flags  = MOD_ESP_TRANSPORT_BPAS_MSK | MOD_HDR_RPLC_MSK |
			  MOD_IPV4_MSK,
		.recipe = MOD_RCP_ESP_V4_BPAS
	},
	/* ================================================================ */
	/*         ESP transport mode over IPv4(outer) Encapsulation        */
	/* ================================================================ */
	{
		.flags  = MOD_ESP_TRANSPORT_ENCP_MSK | MOD_HDR_RPLC_MSK |
			  MOD_IPV4_MSK,
		.recipe = MOD_RCP_ESP_V4_ENCAP
	},
	/* ================================================================ */
	/*         ESP transport mode + L2TP over UDP over                  */
	/*         IPv4(outer) Encapsulation                                */
	/* ================================================================ */
	{
		.flags  = MOD_ESP_TRANSPORT_ENCP_MSK | MOD_IPV4_MSK |
			  MOD_IPV4_NAT_MSK | MOD_L4_NAPT_MSK |
			  MOD_NHDR_IPV4_MSK | MOD_NHDR_L2TP_MSK |
			  MOD_NHDR_UDP_MSK,
		.recipe = MOD_RCP_V4_IN_V4_UDP_L2TP_ENCAP
	},
	{
		.flags  = MOD_ESP_TRANSPORT_ENCP_MSK | MOD_IPV4_MSK |
			  MOD_IPV4_NAT_MSK | MOD_L4_NAPT_MSK |
			  MOD_NHDR_IPV4_MSK | MOD_NHDR_UDP_MSK,
		.recipe = MOD_RCP_V4_IN_V4_UDP_ENCAP
	},
	/* ================================================================ */
	/*         ESP transport mode over IPv4(outer) Encapsulation        */
	/*         with L3 header replacement                               */
	/* ================================================================ */
	{
		.flags  = MOD_ESP_TRANSPORT_ENCP_MSK | MOD_HDR_L3_RPLC_MSK |
			  MOD_IPV4_MSK | MOD_IPV4_NAT_MSK | MOD_L4_NAPT_MSK,
		.recipe = MOD_RCP_ESP_V4_ENCAP_L3_RPLC
	},
	/* ================================================================ */
	/*         ESP transport mode over IPv6(outer) Bypass               */
	/* ================================================================ */
	{
		.flags  = MOD_ESP_TRANSPORT_BPAS_MSK | MOD_HDR_RPLC_MSK |
			  MOD_IPV6_MSK,
		.recipe = MOD_RCP_ESP_V6_BPAS
	},

	/* ================================================================ */
	/*         ESP transport mode over IPv6(outer) Encapsulation        */
	/* ================================================================ */
	{
		.flags  = MOD_ESP_TRANSPORT_ENCP_MSK | MOD_HDR_RPLC_MSK |
			  MOD_IPV6_MSK,
		.recipe = MOD_RCP_ESP_V6_ENCAP
	},

	/* ================================================================ */
	/*         ESP transport mode + L2TP over UDP over                  */
	/*         IPv6(outer) Encapsulation                                */
	/* ================================================================ */
	{
		.flags  = MOD_ESP_TRANSPORT_ENCP_MSK | MOD_IPV6_MSK |
			  MOD_IPV6_HOPL_MSK | MOD_NHDR_IPV6_MSK |
			  MOD_NHDR_L2TP_MSK | MOD_NHDR_UDP_MSK,
		.recipe = MOD_RCP_V6_IN_V6_UDP_L2TP_ENCAP
	},
	{
		.flags  = MOD_ESP_TRANSPORT_ENCP_MSK | MOD_IPV6_MSK |
			  MOD_IPV6_HOPL_MSK | MOD_NHDR_IPV6_MSK |
			  MOD_NHDR_UDP_MSK,
		.recipe = MOD_RCP_V6_IN_V6_UDP_ENCAP
	},

	/* ================================================================ */
	/*         ESP transport mode over IPv6(outer) Encapsulation        */
	/*         with L3 header replacement                               */
	/* ================================================================ */
	{
		.flags  = MOD_ESP_TRANSPORT_ENCP_MSK | MOD_HDR_L3_RPLC_MSK |
			  MOD_IPV6_MSK | MOD_IPV6_HOPL_MSK,
		.recipe = MOD_RCP_ESP_V6_ENCAP_L3_RPLC
	},

	/* ================================================================ */
	/*                    GRE Decapsulation                             */
	/* ================================================================ */
	{ /* ETH IPv4 over GRE over IPv4/6 decapsulation */
		.flags  = MOD_EOGRE_DECP_MSK | MOD_IPV4_NAT_MSK |
			  MOD_L4_NAPT_MSK,
		.recipe = MOD_RCP_IPV4_OFF3_DECP
	},
	{ /* ETH IPv4 over GRE over IPv4/6 decapsulation */
		.flags  = MOD_EOGRE_DECP_MSK | MOD_IPV4_NAT_MSK,
		.recipe = MOD_RCP_IPV4_OFF3_DECP_NAT
	},
	{ /* IPv4 over GRE over IPv4/6 decapsulation */
		.flags  = MOD_IPOGRE_DECP_MSK | MOD_IPV4_NAT_MSK |
			  MOD_L4_NAPT_MSK,
		.recipe = MOD_RCP_IPOGRE_DECP_IPV4_NAPT
	},
	{ /* ESP over GRE over IPv4/6 decapsulation */
		.flags  = MOD_IPOGRE_DECP_MSK | MOD_IPV4_NAT_MSK,
		.recipe = MOD_RCP_IPOGRE_DECP_IPV4
	},
	{ /* ETH IPv6 over GRE over IPv4/6 decapsulation */
		.flags  = MOD_EOGRE_DECP_MSK | MOD_IPV6_HOPL_MSK,
		.recipe = MOD_RCP_IPV6_OFF3_DECP
	},
	{ /* IPv6 over GRE over IPv4/6 decapsulation */
		.flags  = MOD_IPOGRE_DECP_MSK | MOD_IPV6_HOPL_MSK,
		.recipe = MOD_RCP_IPOGRE_DECP_IPV6_ROUTED
	},
	/* ================================================================ */
	/*                    DSLITE Decapsulation                          */
	/* ================================================================ */
	{ /* Dslite tunnel decapsulation */
		.flags  = MOD_DSLITE_DECP_MSK | MOD_IPV4_NAT_MSK |
			  MOD_L4_NAPT_MSK,
		.recipe = MOD_RCP_DSLITE_DECP_NAPT
	},
	/* ================================================================ */
	/*                    6RD Decapsulation                             */
	/* ================================================================ */
	{ /* 6RD inner IPv6 decapsulation */
		.flags  = MOD_6RD_DECP_MSK | MOD_IPV6_HOPL_MSK,
		.recipe = MOD_RCP_6RD_DECP_ROUTED
	},
	/* ================================================================ */
	/*                    L2TP Decapsulation                            */
	/* ================================================================ */
	{ /* Inner IPv6 over L2TP UDP  */
		.flags  = MOD_L2TPUDP_DECP_MSK | MOD_IPV6_HOPL_MSK,
		.recipe = MOD_RCP_IPV6_OFF3_DECP
	},
	{ /* Inner IPv4 NAPT over L2TP UDP  */
		.flags  = MOD_L2TPUDP_DECP_MSK | MOD_IPV4_NAT_MSK |
			  MOD_L4_NAPT_MSK,
		.recipe = MOD_RCP_IPV4_OFF3_DECP
	},
	{ /* Inner IPv6 over L2TP IP  */
		.flags	= MOD_L2TPIP_DECP_MSK | MOD_IPV6_HOPL_MSK,
		.recipe = MOD_RCP_L2TPIP_DECP_IPV6_ROUTED
	},
	{ /* Inner IPv4 NAPT over L2TP IP  */
		.flags	= MOD_L2TPIP_DECP_MSK | MOD_IPV4_NAT_MSK |
			  MOD_L4_NAPT_MSK,
		.recipe = MOD_RCP_L2TPIP_DECP_IPV4_NAPT
	},
	/* ================================================================ */
	/*                    L2TP UDP Encapsulation                        */
	/* ================================================================ */
	{
		.flags  = MOD_IPV4_NAT_MSK | MOD_L4_NAPT_MSK |
			  MOD_NHDR_IPV4_MSK | MOD_NHDR_UDP_MSK,
		.recipe = MOD_RCP_V4_IN_V4_UDP_ENCAP,
	},
	{
		.flags  = MOD_IPV4_NAT_MSK | MOD_L4_NAPT_MSK |
			  MOD_NHDR_IPV6_MSK | MOD_NHDR_UDP_MSK,
		.recipe = MOD_RCP_V4_IN_V6_UDP_ENCAP,
	},
	{
		.flags  = MOD_IPV6_HOPL_MSK | MOD_NHDR_IPV4_MSK |
			  MOD_NHDR_UDP_MSK,
		.recipe = MOD_RCP_V6_IN_V4_UDP_ENCAP,
	},
	{
		.flags  = MOD_IPV6_HOPL_MSK | MOD_NHDR_IPV6_MSK |
			  MOD_NHDR_UDP_MSK,
		.recipe = MOD_RCP_V6_IN_V6_UDP_ENCAP,
	},
	{
		.flags  = MOD_IPV4_NAT_MSK | MOD_L4_NAPT_MSK |
			  MOD_NHDR_IPV4_MSK | MOD_NHDR_UDP_MSK |
			  MOD_NHDR_L2TP_MSK,
		.recipe = MOD_RCP_V4_IN_V4_UDP_L2TP_ENCAP,
	},
	{
		.flags  = MOD_IPV4_NAT_MSK | MOD_L4_NAPT_MSK |
			  MOD_NHDR_IPV6_MSK | MOD_NHDR_UDP_MSK |
			  MOD_NHDR_L2TP_MSK,
		.recipe = MOD_RCP_V4_IN_V6_UDP_L2TP_ENCAP,
	},
	{
		.flags  = MOD_IPV6_HOPL_MSK | MOD_NHDR_IPV4_MSK |
			  MOD_NHDR_UDP_MSK | MOD_NHDR_L2TP_MSK,
		.recipe = MOD_RCP_V6_IN_V4_UDP_L2TP_ENCAP,
	},
	{
		.flags  = MOD_IPV6_HOPL_MSK | MOD_NHDR_IPV6_MSK |
			  MOD_NHDR_UDP_MSK | MOD_NHDR_L2TP_MSK,
		.recipe = MOD_RCP_V6_IN_V6_UDP_L2TP_ENCAP,
	},
	/* ================================================================ */
	/*                    L2TP IP Encapsulation                         */
	/* ================================================================ */
	{
		.flags  = MOD_IPV4_NAT_MSK | MOD_L4_NAPT_MSK |
			  MOD_NHDR_IPV4_MSK,
		.recipe = MOD_RCP_IPV4_IPV4_ENCAP,
	},
	{
		.flags  = MOD_IPV4_NAT_MSK | MOD_L4_NAPT_MSK |
			  MOD_NHDR_IPV6_MSK,
		.recipe = MOD_RCP_IPV6_IPV4_ENCAP
	},
	/* ================================================================ */
	/*                    VXLAN                                         */
	/* ================================================================ */
	{ /* Inner IPv4 NAPT over VXLAN IPV4  */
		.flags	= MOD_UDP_ENCAP_MSK | MOD_IPV4_NAT_MSK |
			MOD_NHDR_IPV4_MSK | MOD_L4_NAPT_MSK,
		.recipe = MOD_RCP_VXLANV4_V4_ENCAP
	},
	{ /* Inner IPv6 over VXLAN IPV4  */
		.flags	= MOD_UDP_ENCAP_MSK | MOD_IPV6_HOPL_MSK |
			  MOD_NHDR_IPV4_MSK,
		.recipe = MOD_RCP_VXLANV4_V6_ENCAP
	},
	{ /* Inner IPv4 NAPT over VXLAN IPV6  */
		.flags	= MOD_UDP_ENCAP_MSK | MOD_NHDR_IPV6_MSK |
			  MOD_IPV4_NAT_MSK | MOD_L4_NAPT_MSK,
		.recipe = MOD_RCP_VXLANV6_V4_ENCAP
	},
	{ /* Inner IPv6 over VXLAN IPV6  */
		.flags	= MOD_UDP_ENCAP_MSK | MOD_NHDR_IPV6_MSK |
			  MOD_IPV6_HOPL_MSK,
		.recipe = MOD_RCP_VXLANV6_V6_ENCAP
	},
	{ /* Inner IPv6 over VXLAN IPV6/V4 Decapsulation */
		.flags	= MOD_UDP_DECAP_MSK | MOD_IPV6_HOPL_MSK,
		.recipe = MOD_RCP_IPV6_OFF3_DECP
	},
	{ /* Inner IPv4 NAPT over VXLAN IPV4/6 Decapsulation */
		.flags	= MOD_UDP_DECAP_MSK | MOD_IPV4_NAT_MSK |
			  MOD_L4_NAPT_MSK,
		.recipe = MOD_RCP_IPV4_OFF3_DECP
	},
	{ /* Inner IPv4 NAPT over VXLAN IPV4 NO CSUM support  */
		.flags	= MOD_UDP_ENCAP_MSK | MOD_UDP_CSUM_DISABLE_MSK |
			  MOD_IPV4_NAT_MSK | MOD_L4_NAPT_MSK,
		.recipe = MOD_RCP_VXLANV4_V4_ENCAP_NO_CS
	},
	{ /* Inner IPv6 over VXLAN IPV4 NO CS*/
		.flags	= MOD_UDP_ENCAP_MSK | MOD_UDP_CSUM_DISABLE_MSK |
			  MOD_IPV6_HOPL_MSK,
		.recipe = MOD_RCP_VXLANV4_V6_ENCAP_NO_CS
	},
	/* ================================================================ */
	/*                    MAP-T US/DS                                   */
	/* ================================================================ */
	{
		/* IPv4 to IPv6 translation - MAP-T US */
		.flags  = MOD_IPV4_MSK | MOD_NHDR_IPV6_MSK |
			  MOD_HDR_L3_RPLC_MSK | MOD_IPV4_IPV6_TRANS_MSK |
			  MOD_L4_NAPT_MSK,
		.recipe = MOD_RCP_IPV4_IPV6_TRANS, /* MOD_RCP_IPV6_IPV4_ENCAP */
	},
	{
		/* IPv6 to IPv4 translation - MAP-T DS */
		.flags  = MOD_IPV6_MSK | MOD_NHDR_IPV4_MSK |
			  MOD_HDR_L3_RPLC_MSK | MOD_IPV6_IPV4_TRANS_MSK |
			  MOD_L4_NAPT_MSK,
		.recipe = MOD_RCP_IPV6_IPV4_TRANS, /* MOD_RCP_IPV4_IPV6_ENCAP */
	},
	/* ================================================================ */
	/*                    EoMPLS Decapsulation                          */
	/* ================================================================ */
	{ /* ETH over MPLS IPv4 decapsulation */
		.flags  = MOD_IPV4_NAT_MSK | MOD_L4_NAPT_MSK |
			  MOD_MPLS_V4_DECP_MSK,
		.recipe = MOD_RCP_DSLITE_DECP_NAPT,
	},
	{ /* ETH over MPLS IPv6 decapsulation */
		.flags  = MOD_MPLS_V6_DECP_MSK | MOD_IPV6_HOPL_MSK,
		.recipe = MOD_RCP_6RD_DECP_ROUTED
	},
};

/* shortcuts to copy fields to new header */
#define ipv4_fld_to_nhdr(rcp, reg, fld)                                        \
	mod_reg_to_new_hdr_cpy((rcp), reg, MOD_PKT_OFF_L3_EXT,                 \
			       offsetof(struct iphdr, fld),                    \
			       sizeof_field(struct iphdr, fld))

#define ipv6_fld_to_nhdr(rcp, reg, fld)                                        \
	mod_reg_to_new_hdr_cpy((rcp), reg, MOD_PKT_OFF_L3_EXT,                 \
			       offsetof(struct ipv6hdr, fld),                  \
			       sizeof_field(struct ipv6hdr, fld))

#define pppoe_fld_to_nhdr(rcp, reg, fld)                                       \
	mod_reg_to_new_hdr_cpy((rcp), reg, EU_SI_SEL_PPPOE_OFF,                \
			       offsetof(struct pppoe_hdr, fld),                \
			       sizeof_field(struct pppoe_hdr, fld))

#define udp_fld_to_nhdr(rcp, reg, fld)                                         \
	mod_reg_to_new_hdr_cpy((rcp), reg, EU_SI_SEL_HDR_L4_OFF,               \
			       offsetof(struct udphdr, fld),                   \
			       sizeof_field(struct udphdr, fld))

#define l2tpudp_fld_to_nhdr(rcp, reg, off, sz)                                 \
	mod_reg_to_new_hdr_cpy((rcp), reg, EU_SI_SEL_HDR_L4_OFF,               \
			       sizeof(struct udphdr) + off, sz)

s32 mod_recipe_select(unsigned long mod_bmap, u8 *rcp)
{
	u32 i, n;
	char str[128] = { 0 };

	if (ptr_is_null(rcp))
		return -EINVAL;

	pr_debug("mod_bmap %#lx\n", mod_bmap);
	for (i = 0; i < ARRAY_SIZE(supp_mod_types); i++) {
		if (supp_mod_types[i].flags != mod_bmap)
			continue;

		*rcp = supp_mod_types[i].recipe;
		pr_debug("recipe %s(%u) selected\n",
			 mod_rcp_to_str(*rcp), *rcp);
		return 0;
	}

	pr_buf(str, sizeof(str), n, "Modification %#lx isn't supported\n",
	       mod_bmap);
	for_each_set_bit(i, &mod_bmap, MOD_FLAG_NUM)
		pr_buf_cat(str, sizeof(str), n, "[%s]", mod_flag_to_str(i));
	pr_debug("%s\n", str);

	return -EPROTONOSUPPORT;
}

const char * const mod_rcp_to_str(u32 rcp_idx)
{
	if (rcp_idx == MOD_NO_RCP || rcp_idx == MOD_RCP_NOP)
		return mod_rcp_str[MOD_RCPS_NUM];

	if (unlikely(rcp_idx > MOD_RCP_LAST))
		return mod_rcp_str[MOD_RCPS_NUM + 1];

	return mod_rcp_str[rcp_idx];
}

const char * const mod_flag_to_str(u32 flag)
{
	if (unlikely(flag > MOD_FLAG_LAST)) {
		pr_err("error: invalid flag id, %d\n", flag);
		return mod_flag_str[MOD_FLAG_NUM];
	}

	return mod_flag_str[flag];
}

static s32 mod_rcp_no_mod_set(s32 rcp_idx)
{
	struct mod_recipe rcp;
	s32 ret;

	if (unlikely(rcp_idx > MOD_RCP_LAST)) {
		pr_err("error: invalid recipe id, %d\n", rcp_idx);
		return -EINVAL;
	}

	pr_debug("Configuring recipe %s(%u)\n", mod_rcp_to_str(rcp_idx),
		 rcp_idx);

	memset(&rcp, 0, sizeof(rcp));
	rcp.index = rcp_idx;
	ret = mod_rcp_complete(&rcp);
	if (unlikely(ret)) {
		pr_err("Failed to create recipe %s(%d)\n",
		       mod_rcp_to_str(rcp_idx), rcp_idx);
	}

	return ret;
}

static s32 mod_rcp_hdr_rplc_set(s32 rcp_idx, bool pppoe_update,
				enum mod_eu_stw_mux_sel strip_l3_off)
{
	struct mod_recipe rcp;
	s32 ret = 0;
	s32 pkt_l3_len     = DPU_RCP_REG0_B0;
	s32 si_l3_ttl_diff = DPU_RCP_REG1_B0;
	s32 pppoe_diff     = DPU_RCP_REG1_B1;
	s32 pppoe_len      = DPU_RCP_REG1_B2;

	/* USE registers R12 - R15 with extra caution as they are
	 * the registers to compare to for the alu conditions
	 * R12 - in used by ALU0 for pppoe and l4 csum calculation and MUST
	 *       be zero in the time of the calculation
	 * R13 - not used
	 * R14 - not used
	 * R15 - not used
	 */

	if (unlikely(rcp_idx > MOD_RCP_LAST)) {
		pr_err("error: invalid recipe id, %d\n", rcp_idx);
		return -EINVAL;
	}

	pr_debug("Configuring recipe %s(%u)\n", mod_rcp_to_str(rcp_idx),
		 rcp_idx);
	memset(&rcp, 0, sizeof(rcp));
	rcp.index = rcp_idx;

	if (pppoe_update) {
		/*==========================================*/
		/* copy from sce mux to dpu registers (sce) */
		/*==========================================*/
		/* Fetch SI info: ttl, pppoe diff and pppoe length fields */
		ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l3_ttl_diff,
					      SCE_SI_SEL_TTL);

		/*===========================================*/
		/* copy from packet to dpu registers (store) */
		/*===========================================*/
		/* store L3 total_len */
		ret |= mod_pkt_to_reg_cpy(&rcp, pkt_l3_len, strip_l3_off,
					  MOD_SI_PKT_OFF,
					  sizeof_field(struct iphdr, tot_len));

		/*=========================*/
		/*      ALU operations     */
		/*=========================*/
		/* sync to the IP len field */
		ret |= mod_alu_synch(EU_ALU0, &rcp, strip_l3_off,
				     MOD_SI_PKT_OFF,
				     sizeof_field(struct iphdr, tot_len));

		/* pppoe_len = pppoe_diff ? pppoe_diff + l3 len : l2_org_val */
		ret |= mod_alu_add_cond(EU_ALU0, &rcp, pppoe_diff, 1,
					pkt_l3_len, 2, pppoe_len,
					EU_ALU_L0_BIG_R2, EU_ALU_COND_MET);
	}

	/*============================================*/
	/* copy from si bce (sram) to packet (insrep) */
	/*============================================*/
	/* replace old header with new header */
	ret |= mod_hdr_rplc(&rcp, BCE_OFF_0, strip_l3_off);

	/*======================================================*/
	/* copy from dpu registers to packet new header (reps1) */
	/*======================================================*/
	if (pppoe_update)
		ret |= mod_reg_to_new_hdr_cpy(&rcp, pppoe_len,
					      EU_SI_SEL_PPPOE_OFF,
					      offsetof(struct pppoe_hdr,
						       length),
					      sizeof_field(struct pppoe_hdr,
							   length));

	/*=================================================*/
	/* complete recipe with nop command and 'last' bit */
	/*=================================================*/
	ret |= mod_rcp_complete(&rcp);
	if (unlikely(ret)) {
		pr_err("Failed to create recipe %s(%d)\n",
		       mod_rcp_to_str(rcp_idx), rcp_idx);
	}

	return ret;
}

/**
 * @brief Create recipe for IPv4 traffic with NAT changes in L3 and L4
 * @param rcp_idx recipe index to create
 * @param l3_off offset to the l3 offset field in the status word
 *               all prior layers will be trimmed, l3 NAT changes will
 *               be done on that layer, l4 NAT changes will done on
 *               the following tcp/udp header
 * @param l4_napt specify if l4 napt changes are needed in the
 *                    recipe
 * @param nhdr new header ip type
 * @param pppoe_encap specify if pppoe encapsulation should be supported
 * @return s32 0 on success, error code otherwise
 */
static s32 mod_rcp_ipv4_routed_set(s32 rcp_idx, enum mod_eu_stw_mux_sel l3_off,
				   bool l4_napt, enum rcp_hdr_ip_type nhdr,
				   bool pppoe_encap)
{
	struct mod_recipe rcp;
	s32 ret = 0;

	/* ================= */
	/* Map dpu registers */
	/* ================= */
	s32 si_l4_csum_delta   = DPU_RCP_REG0_B0;
	s32 si_l3_csum_delta   = DPU_RCP_REG0_B2;
	s32 si_l3_tot_len_diff = DPU_RCP_REG1_B0;
	s32 si_l3_tos          = DPU_RCP_REG1_B1;
	s32 si_nhdr_csum       = DPU_RCP_REG1_B2;
	s32 si_l3_ttl_diff     = DPU_RCP_REG2_B0;
	s32 pppoe_diff         = DPU_RCP_REG2_B1;
	s32 pppoe_len          = DPU_RCP_REG2_B2;
	s32 si_l4_ports        = DPU_RCP_REG3_B0;
	s32 pkt_l3_csum        = DPU_RCP_REG4_B0;
	s32 pkt_l3_ttl         = DPU_RCP_REG4_B3;
	s32 pkt_l4_csum        = DPU_RCP_REG5_B0;
	s32 si_l4_csum_zero    = DPU_RCP_REG7_B0;
	s32 stw_pkt_len        = DPU_RCP_REG8_B0;
	s32 stw_off_l3         = DPU_RCP_REG9_B0;
	s32 nhdr_l3_tot_len    = DPU_RCP_REG9_B2;
	s32 nhdr_id            = DPU_RCP_REG10_B0;
	s32 pkt_l3_tot_len     = DPU_RCP_REG13_B0_COND;

	/* USE registers R12 - R15 with extra caution as they are
	 * the registers to compare to for the alu conditions
	 * R12 - in used by ALU0 for pppoe and l4 csum calculation and MUST
	 *       be zero in the time of the calculation
	 * R13 - in used by ALU0 for none-tunnel total len adaptation,
	 *       used to calculate the total len from packet len only in case
	 *        the result is smaller than the original value,
	 *       this condition protecting the padding use case
	 *       but supporting the IPSec inbound 2nd round modification
	 * R14 - not used
	 * R15 - not used
	 */

	if (unlikely(rcp_idx > MOD_RCP_LAST)) {
		pr_err("error: invalid recipe id, %d\n", rcp_idx);
		return -EINVAL;
	}

	pr_debug("Configuring recipe %s(%u)\n", mod_rcp_to_str(rcp_idx),
		 rcp_idx);
	memset(&rcp, 0, sizeof(rcp));
	rcp.index = rcp_idx;

	/*==========================================*/
	/* copy from sce mux to dpu registers (sce) */
	/*==========================================*/
	/* Fetch SI info: L3 csum delta & L4 checksum delta */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l4_csum_delta,
				      SCE_SI_SEL_L4_CSUM_DELTA);

	/* Fetch SI info: total length diff, tos, and new header csum */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l3_tot_len_diff,
				      SCE_SI_SEL_TOT_LEN_DIFF);

	/* Fetch SI info: ttl, pppoe diff and pppoe length fields */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l3_ttl_diff, SCE_SI_SEL_TTL);

	/* Fetch SI info: L4 ports and l4 csum zero */
	if (l4_napt) {
		ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l4_ports,
					      SCE_SI_SEL_NEW_SRC_PORT);
		ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l4_csum_zero,
					      SCE_SI_SEL_L4_CSUM_ZERO);
	}

	/*======================*/
	/* STW info: packet len */
	/*======================*/
	ret |= mod_sce_mux_to_reg_cpy(&rcp, stw_pkt_len, SCE_STW_SEL_PKT_LEN);
	/*======================*/
	/* STW info: offset L3  */
	/*======================*/
	ret |= mod_sce_mux_to_reg_cpy(&rcp, stw_off_l3, SCE_STW_SEL_L3_OFF0);

	/*===========================================*/
	/* copy from packet to dpu registers (store) */
	/*===========================================*/
	/* store L3 total_len */
	/* @TODO: can't store total len and id fields both in a
	 * single command since the next command should store
	 * the ttl field and the 'stall' functionality is not
	 * working as expected
	 */
	ret |= mod_pkt_to_reg_cpy(&rcp, pkt_l3_tot_len, l3_off, 0, 4);

	/* store L3 TTL and L3 checksum */
	ret |= mod_pkt_to_reg_cpy(&rcp, pkt_l3_csum, l3_off,
				  offsetof(struct iphdr, ttl),
				  sizeof_field(struct iphdr, ttl) +
				  sizeof_field(struct iphdr, protocol) +
				  sizeof_field(struct iphdr, check));

	/* store L4 checksum */
	if (l4_napt) {
		ret |= mod_pkt_to_reg_cpy(&rcp, pkt_l4_csum,
					  STW_NEXT_LAYER(l3_off),
					  MOD_SI_PKT_OFF,
					  sizeof_field(struct udphdr, check));
	}

	/*=========================*/
	/*      ALU operations     */
	/*=========================*/

	/* |---------------------------------|---------------------------------|
	 * |      ALU0                       |      ALU1                       |
	 * |---------------------------------|---------------------------------|
	 * | sync to ip csum                 |                                 |
	 * |---------------------------------|---------------------------------|
	 * | ID inc (nhdr v4)                | sync to L4 dport (napt)         |
	 * |                                 | sync to ip csum (no napt)       |
	 * |---------------------------------|---------------------------------|
	 * | pkt ip csum + si delta          | TTL decrement                   |
	 * |---------------------------------|---------------------------------|
	 * | pkt ip csum + old tot len       | pkt l4 csum step 1              |
	 * | (non-tunnel)                    |                                 |
	 * |---------------------------------|---------------------------------|
	 * | pkt ip len re-calculate         | pkt l4 csum step 2              |
	 * | (non-tunnel)                    |                                 |
	 * |---------------------------------|---------------------------------|
	 * | pkt ip csum - new tot len       | csum ID (nhdr v4)               |
	 * | (non-tunnel)                    |                                 |
	 * |---------------------------------|---------------------------------|
	 * | nhdr tot len                    | csum tot len (nhdr v4)          |
	 * |   (nhdr or pppoe_encap)         |                                 |
	 * |---------------------------------|---------------------------------|
	 * | pppoe pkt len (pppoe_encap)     |                                 |
	 * |---------------------------------|---------------------------------|
	 */

	/* Synchronize ALU0 to latest fetched information of STORE command */
	ret |= mod_alu_synch(EU_ALU0, &rcp, l3_off,
			     offsetof(struct iphdr, check),
			     sizeof_field(struct iphdr, check));
	if (l4_napt) {
		/* Synchronize ALU1 to L4 checksum field,
		 * The offset value is taken from the SI and it will be
		 * Different value for UDP and TCP
		 */
		ret |= mod_alu_synch(EU_ALU1, &rcp, STW_NEXT_LAYER(l3_off),
				     MOD_SI_PKT_OFF,
				     sizeof_field(struct udphdr, check));
	} else {
		ret |= mod_alu_synch(EU_ALU1, &rcp, l3_off,
				     offsetof(struct iphdr, check),
				     sizeof_field(struct iphdr, check));
	}

	if (nhdr == RCP_HDR_IPV4) {
		ret |= mod_alu_rand(EU_ALU0, &rcp, nhdr_id,
				    sizeof_field(struct iphdr, id));
	}

	/* Recalculate L3 checksum */
	ret |= mod_alu_csum16_add(EU_ALU0, &rcp, si_l3_csum_delta, pkt_l3_csum,
				  pkt_l3_csum);

	/* fix total length and checksum only for non-tunnels recipes */
	if (nhdr == RCP_HDR_NO_IP && l3_off == EU_STW_SEL_L3_OFF0) {
		/* remove old total length */
		ret |= mod_alu_csum16_add(EU_ALU0, &rcp, pkt_l3_tot_len,
					  pkt_l3_csum, pkt_l3_csum);

		/* calculate the new total length for cases where
		 * the result is smaller than the original total len.
		 * this is used for 2nd round of IPSec inbound where
		 * the ESP was removed but external IP is still including
		 * the ESP header size.
		 * for case of short packets with padding we dont want to
		 * update the total len.
		 * (where calculated_total_len > pkt_total_len)
		 */
		ret |= mod_alu_sub_cond(EU_ALU0, &rcp, stw_pkt_len, 2,
					stw_off_l3, 1, pkt_l3_tot_len,
					EU_ALU_RES_SML_R3, EU_ALU_COND_MET);

		/* add new total length */
		ret |= mod_alu_csum16_sub(EU_ALU0, &rcp, pkt_l3_tot_len,
					  pkt_l3_csum, pkt_l3_csum);
	}

	/* Calculate new header total length */
	if (nhdr != RCP_HDR_NO_IP || pppoe_encap) {
		ret |= mod_alu_add(EU_ALU0, &rcp, si_l3_tot_len_diff, 1,
				   pkt_l3_tot_len, 2, nhdr_l3_tot_len);

		/* pppoe_len = pppoe_diff ? pppoe_diff + l3 len : l2_org_val */
		if (pppoe_encap) {
			ret |= mod_alu_add_cond(EU_ALU0, &rcp, pppoe_diff, 1,
						nhdr_l3_tot_len, 2, pppoe_len,
						EU_ALU_L0_BIG_R2,
						EU_ALU_COND_MET);
		}
	}

	/* decrement TTL */
	ret |= mod_alu_sub(EU_ALU1, &rcp, pkt_l3_ttl,
			   sizeof_field(struct iphdr, ttl), si_l3_ttl_diff,
			   sizeof_field(struct iphdr, ttl), pkt_l3_ttl);

	/* Recalculate L4 checksum */
	if (l4_napt) {
		/* in general, the checksum calculation should be as follow:
		 *   udp: csum = csum ? csum + csum_delta : csum
		 *   tcp: csum = csum + csum_delta
		 * instead, we have csum_zero value which is set as follow
		 *   udp: csum_zero = csum_delta
		 *   tcp: csum_zero = 0
		 * and the calculation is as follow for both:
		 *   csum = csum + csum_delta
		 *   csum = (csum - csum_zero) ? csum : csum - csum_zero;
		 */
		ret |= mod_alu_csum16_add(EU_ALU1, &rcp, si_l4_csum_delta,
					  pkt_l4_csum, pkt_l4_csum);
		ret |= mod_alu_csum16_sub_cond(EU_ALU1, &rcp, si_l4_csum_zero,
					       pkt_l4_csum, pkt_l4_csum,
					       EU_ALU_RES_EQ_R2,
					       EU_ALU_COND_MET);
	}

	if (nhdr == RCP_HDR_IPV4) {
		/* Update the external L3 csum due to global id */
		ret |= mod_alu_csum16_sub(EU_ALU1, &rcp, nhdr_id,
					  si_nhdr_csum, si_nhdr_csum);

		/* Update the external L3 csum due to total len */
		ret |= mod_alu_csum16_sub(EU_ALU1, &rcp, nhdr_l3_tot_len,
					  si_nhdr_csum, si_nhdr_csum);
	}

	/*==========================================*/
	/* copy from dpu registers to packet (reps) */
	/*==========================================*/
	/* Replace L3 TOS */
	ret |= mod_reg_to_hdr_cpy(&rcp, si_l3_tos, l3_off,
				  offsetof(struct iphdr, tos),
				  sizeof_field(struct iphdr, tos));

	/* update this only on non-tunnels recipes */
	if (nhdr == RCP_HDR_NO_IP && l3_off == EU_STW_SEL_L3_OFF0) {
		/* Replace L3 total length */
		ret |= mod_reg_to_hdr_cpy(&rcp, pkt_l3_tot_len, l3_off,
					offsetof(struct iphdr, tot_len),
					sizeof_field(struct iphdr, tot_len));
	}

	/* Replace L3 TTL */
	ret |= mod_reg_to_hdr_cpy(&rcp, pkt_l3_ttl, l3_off,
				  offsetof(struct iphdr, ttl),
				  sizeof_field(struct iphdr, ttl));

	/* Replace L3 CSUM */
	ret |= mod_reg_to_hdr_cpy(&rcp, pkt_l3_csum, l3_off,
				  offsetof(struct iphdr, check),
				  sizeof_field(struct iphdr, check));

	if (l4_napt) {
		/* Replace L4 PORTS */
		ret |= mod_reg_to_hdr_cpy(&rcp, si_l4_ports,
					  STW_NEXT_LAYER(l3_off),
					  offsetof(struct udphdr, source),
					  sizeof_field(struct udphdr, source) +
					  sizeof_field(struct udphdr, dest));

		/* Replace L4 CSUM */
		ret |= mod_reg_to_hdr_cpy(&rcp, pkt_l4_csum,
					  STW_NEXT_LAYER(l3_off),
					  MOD_SI_PKT_OFF,
					  sizeof_field(struct udphdr, check));
	}

	/*============================================*/
	/* copy from si bce (sram) to packet (insrep) */
	/*============================================*/
	/* replace old L2 with new header */
	ret |= mod_hdr_rplc(&rcp, BCE_OFF_16, l3_off);

	/* replace old L3 IPs with new IPs */
	ret |= mod_bce_ip_nat_cpy(&rcp, BCE_OFF_0, l3_off);

	/*======================================================*/
	/* copy from dpu registers to packet new header (reps1) */
	/*======================================================*/
	if (pppoe_encap)
		ret |= mod_reg_to_new_hdr_cpy(&rcp, pppoe_len,
					      EU_SI_SEL_PPPOE_OFF,
					      offsetof(struct pppoe_hdr,
						       length),
					      sizeof_field(struct pppoe_hdr,
							   length));

	if (nhdr == RCP_HDR_IPV6) {
		/* Replace L3 payload length with inner total length */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, nhdr_l3_tot_len,
					      MOD_PKT_OFF_L3_EXT,
					      offsetof(struct ipv6hdr,
						       payload_len),
					      sizeof_field(struct ipv6hdr,
							   payload_len));

	} else if (nhdr == RCP_HDR_IPV4) {
		/* Update ipv4 id */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, nhdr_id,
					      MOD_PKT_OFF_L3_EXT,
					      offsetof(struct iphdr, id),
					      sizeof_field(struct iphdr, id));

		/* Update ipv4 total length */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, nhdr_l3_tot_len,
					      MOD_PKT_OFF_L3_EXT,
					      offsetof(struct iphdr, tot_len),
					      sizeof_field(struct iphdr,
							   tot_len));

		/* Replace L3 CSUM */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, si_nhdr_csum,
					      MOD_PKT_OFF_L3_EXT,
					      offsetof(struct iphdr, check),
					      sizeof_field(struct iphdr,
							   check));
	}

	/*=================================================*/
	/* complete recipe with nop command and 'last' bit */
	/*=================================================*/
	ret |= mod_rcp_complete(&rcp);
	if (unlikely(ret)) {
		pr_err("Failed to create recipe %s(%d)\n",
		       mod_rcp_to_str(rcp_idx), rcp_idx);
	}

	return ret;
}

/**
 * @brief Creates recipe for ipv6 routed traffic
 * @param rcp_idx Recipe index to create
 * @param l3_off Offset to the L3 layer in the status word
 *               where HOP_LIMIT should be performed
 * @param nhdr New IP header. NO_IP for flows without encapsulation
 * @param pppoe_encap specify if pppoe encapsulation should be supported
 * @return s32 0 on success, error code otherwise
 */
static s32 mod_rcp_ipv6_routed_set(s32 rcp_idx, enum mod_eu_stw_mux_sel l3_off,
				   enum rcp_hdr_ip_type nhdr,
				   bool pppoe_encap)
{
	struct mod_recipe rcp;
	s32 ret = 0;

	/*===================*/
	/* Map dpu registers */
	/*===================*/
	s32 si_ttl_diff      = DPU_RCP_REG0_B0;
	s32 pppoe_diff       = DPU_RCP_REG0_B1;
	s32 pppoe_len        = DPU_RCP_REG0_B2;
	s32 si_tot_len_diff  = DPU_RCP_REG1_B0;
	s32 si_nhdr_csum     = DPU_RCP_REG1_B2;
	s32 pkt_hop_limit    = DPU_RCP_REG2_B0;
	/* s32 pkt_nexthdr      = DPU_RCP_REG2_B1; */
	s32 pkt_payload_len  = DPU_RCP_REG2_B2;
	s32 stw_off_l3       = DPU_RCP_REG3_B0;
	s32 stw_pkt_len      = DPU_RCP_REG4_B0;
	s32 nhdr_payload_len = DPU_RCP_REG4_B2;
	s32 zero             = DPU_RCP_REG5_B0;
	s32 nhdr_id          = DPU_RCP_REG6_B0;
	/* USE registers R12 - R15 with extra caution as they are
	 * the registers to compare to for the alu conditions
	 * R14 - in used by ALU1 for pppoe and l4 csum calculation and MUST
	 *       be zero in the time of the calculation
	 * R15 - in used by ALU1 for none-tunnel total len adaptation,
	 *       used to calculate the total len from packet len only in case
	 *        the result is smaller than the original value,
	 *       this condition protecting the padding use case
	 *       but supporting the IPSec inbound 2nd round modification
	 */

	s32 cond_payload_len = DPU_RCP_REG15_B0_COND;

	if (unlikely(rcp_idx > MOD_RCP_LAST)) {
		pr_err("error: invalid recipe id, %d\n", rcp_idx);
		return -EINVAL;
	}

	pr_debug("Configuring recipe %s(%u)\n", mod_rcp_to_str(rcp_idx),
		 rcp_idx);
	memset(&rcp, 0, sizeof(rcp));
	rcp.index = rcp_idx;

	/*==========================================*/
	/* copy from sce mux to dpu registers (sce) */
	/*==========================================*/
	/* Fetch SI info: total length diff, tos, and new header csum */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_tot_len_diff,
				      SCE_SI_SEL_TOT_LEN_DIFF);

	/* Fetch SI info: hop limit diff, pppoe diff and pppoe length fields */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_ttl_diff, SCE_SI_SEL_TTL);

	/* Fetch SI info: zero value */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, zero, SCE_SI_SEL_ZERO);

	/* Fetch STW info: l3 offset */
	if (nhdr == RCP_HDR_NO_IP && l3_off == EU_STW_SEL_L3_OFF0) {
		ret |= mod_sce_mux_to_reg_cpy(&rcp, stw_off_l3,
					      SCE_STW_SEL_L3_OFF0);

		/* Fetch STW info: packet len */
		ret |= mod_sce_mux_to_reg_cpy(&rcp, stw_pkt_len,
					      SCE_STW_SEL_PKT_LEN);
	}

	/*===========================================*/
	/* copy from packet to dpu registers (store) */
	/*===========================================*/
	/* store hop limit, next header (not used), and payload length
	 * we always fetch all 3 cause we anyway need the hop limit
	 */
	ret |= mod_pkt_to_reg_cpy(&rcp, pkt_hop_limit, l3_off,
				  offsetof(struct ipv6hdr, payload_len),
				  sizeof_field(struct ipv6hdr, payload_len) +
				  sizeof_field(struct ipv6hdr, nexthdr) +
				  sizeof_field(struct ipv6hdr, hop_limit));

	/*=========================*/
	/*      ALU operations     */
	/*=========================*/

	/*=====================================*/
	/* For recipe with NEW HEADER (Tunnel) */
	/*=====================================*/
	/*  |-------------------|----------------------------------------|
	 *  |       ALU0        |     ALU1                               |
	 *  |-------------------|----------------------------------------|
	 *  | sync              |                                        |
	 *  | ID inc (nhdr v4)  | sync                                   |
	 *  | csum ID (nhdr v4) | cond_pay_len = pkt_pay_len + 0         |
	 *  | HOPL dec          | nhdr_pay_len = cond_pay_len + len_diff |
	 *  |                   | pppoe len (pppoe)                      |
	 *  |                   | csum pkt len (nhdr v4)                 |
	 *  |-------------------|----------------------------------------|
	 */
	/*=============================================*/
	/* For recipe without NEW HEADER (None-Tunnel) */
	/*=============================================*/
	/*  |-----------|------------------------------------------------------|
	 *  | ALU0      | ALU1                                                 |
	 *  | NON-TUNNEL| NON-TUNNEL                                           |
	 *  |-----------|------------------------------------------------------|
	 *  | sync      |                                                      |
	 *  | HOPL dec  | sync                                                 |
	 *  |           | cond_pay_len = pkt_pay_len + 0                       |
	 *  |           | pkt_len = pkt_len - offL3                            |
	 *  |           | cond_pay_len = min(pkt_len - len_diff, cond_pay_len) |
	 *  |           | nhdr_pay_len = cond_pay_len + len_diff (pppoe)       |
	 *  |           | pppoe len (pppoe)                                    |
	 *  |-----------|------------------------------------------------------|
	 */

	/* Synchronize ALU to latest fetched information of STORE command */
	ret |= mod_alu_synch(EU_ALU0, &rcp, l3_off,
			     offsetof(struct ipv6hdr, hop_limit),
			     sizeof_field(struct ipv6hdr, hop_limit));

	ret |= mod_alu_synch(EU_ALU1, &rcp, l3_off,
			     offsetof(struct ipv6hdr, hop_limit),
			     sizeof_field(struct ipv6hdr, hop_limit));


	/* for V4 update csum, id */
	if (nhdr == RCP_HDR_IPV4) {
		ret |= mod_alu_rand(EU_ALU0, &rcp, nhdr_id,
				    sizeof_field(struct iphdr, id));

		/* Update the external L3 csum due to global id */
		ret |= mod_alu_csum16_sub(EU_ALU0, &rcp, nhdr_id,
					si_nhdr_csum, si_nhdr_csum);
	}

	/* decrement hop limit */
	ret |= mod_alu_sub(EU_ALU0, &rcp, pkt_hop_limit,
			   sizeof_field(struct ipv6hdr, hop_limit), si_ttl_diff,
			   sizeof_field(struct ipv6hdr, hop_limit),
			   pkt_hop_limit);

	/* Copy the payload_len to the condition register */
	ret |= mod_alu_add(EU_ALU1, &rcp, pkt_payload_len, 2, zero, 1,
			   cond_payload_len);

	/* calculate payload length based on packet length from descriptor */
	if (nhdr == RCP_HDR_NO_IP && l3_off == EU_STW_SEL_L3_OFF0) {

		ret |= mod_alu_sub(EU_ALU1, &rcp, stw_pkt_len, 2,
				   stw_off_l3, 1, stw_pkt_len);

		/* IPv6 payload length excludes the IPv6 header itself, and,
		 * for none-tunnels packets total length diff should
		 * always be equal to the size of ipv6 header
		 * so we reduce it here
		 *
		 * New paylaod length calculation is being done only for cases
		 * where the result is smaller than the original total len.
		 * this is used for 2nd round of IPSec inbound where
		 * the ESP was removed but external IP is still including
		 * the ESP header size.
		 * for case of short packets with padding we dont want to
		 * update the payload length field.
		 * (where calculated_payload_len > cond_payload_len)
		 */
		ret |= mod_alu_sub_cond(EU_ALU1, &rcp, stw_pkt_len, 2,
					si_tot_len_diff, 1, cond_payload_len,
					EU_ALU_RES_SML_R3, EU_ALU_COND_MET);
	}

	/* Calculate new header total and/or pppoe length */
	if (nhdr != RCP_HDR_NO_IP || pppoe_encap) {
		/* add session total len diff */
		ret |= mod_alu_add(EU_ALU1, &rcp, cond_payload_len, 2,
				   si_tot_len_diff, 1, nhdr_payload_len);

		/* pppoe_len = pppoe_diff ? pppoe_diff + l3 len : l2_org_val */
		if (pppoe_encap)
			ret |= mod_alu_add_cond(EU_ALU1, &rcp, pppoe_diff, 1,
						nhdr_payload_len, 2, pppoe_len,
						EU_ALU_L0_BIG_R2,
						EU_ALU_COND_MET);
	}

	/* for V4 update csum, id */
	if (nhdr == RCP_HDR_IPV4) {
		/* Calculate external L3 csum due to total len.
		 * this MUST be in sync with ALU1 operations
		 * Note: This assumes the total length in the new ipv4
		 *       template header is zero
		 */
		ret |= mod_alu_csum16_sub(EU_ALU1, &rcp, nhdr_payload_len,
					si_nhdr_csum, si_nhdr_csum);
	}

	/*==========================================*/
	/* copy from dpu registers to packet (reps) */
	/*==========================================*/
	/* Replace L3 packet length */
	if (nhdr == RCP_HDR_NO_IP && l3_off == EU_STW_SEL_L3_OFF0) {
		ret |= mod_reg_to_hdr_cpy(&rcp, cond_payload_len, l3_off,
					  offsetof(struct ipv6hdr, payload_len),
					  sizeof_field(struct ipv6hdr,
						       payload_len));
	}
	/* Replace L3 hop limit */
	ret |= mod_reg_to_hdr_cpy(&rcp, pkt_hop_limit, l3_off,
				  offsetof(struct ipv6hdr, hop_limit),
				  sizeof_field(struct ipv6hdr, hop_limit));

	/*============================================*/
	/* copy from si bce (sram) to packet (insrep) */
	/*============================================*/
	/* replace old L2 with new header */
	ret |= mod_hdr_rplc(&rcp, BCE_OFF_0, l3_off);

	/*======================================================*/
	/* copy from dpu registers to packet new header (reps1) */
	/*======================================================*/
	if (pppoe_encap)
		ret |= mod_reg_to_new_hdr_cpy(&rcp, pppoe_len,
					      EU_SI_SEL_PPPOE_OFF,
					      offsetof(struct pppoe_hdr,
						       length),
					      sizeof_field(struct pppoe_hdr,
							   length));

	if (nhdr == RCP_HDR_IPV6) {
		/* Replace payload length with inner payload length */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, nhdr_payload_len,
					      MOD_PKT_OFF_L3_EXT,
					      offsetof(struct ipv6hdr,
						       payload_len),
					      sizeof_field(struct ipv6hdr,
							   payload_len));

	} else if (nhdr == RCP_HDR_IPV4) {
		/* Update ipv4 id */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, nhdr_id,
					      MOD_PKT_OFF_L3_EXT,
					      offsetof(struct iphdr, id),
					      sizeof_field(struct iphdr, id));

		/* Update ipv4 total length */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, nhdr_payload_len,
					      MOD_PKT_OFF_L3_EXT,
					      offsetof(struct iphdr, tot_len),
					      sizeof_field(struct iphdr,
							   tot_len));

		/* Replace L3 CSUM */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, si_nhdr_csum,
					      MOD_PKT_OFF_L3_EXT,
					      offsetof(struct iphdr, check),
					      sizeof_field(struct iphdr,
							   check));
	}

	/*=================================================*/
	/* complete recipe with nop command and 'last' bit */
	/*=================================================*/
	ret |= mod_rcp_complete(&rcp);
	if (unlikely(ret)) {
		pr_err("Failed to create recipe %s(%d)\n",
		       mod_rcp_to_str(rcp_idx), rcp_idx);
	}

	return ret;
}

/**
 * @brief Creates recipe for esp traffic
 * @param rcp_idx Recipe index to create
 * @param nhdr New IP header. NO_IP for flows without encapsulation
 * @return s32 0 on success, error code otherwise
 */
static s32 mod_rcp_esp_set(s32 rcp_idx, enum rcp_hdr_ip_type nhdr,
			   enum mod_eu_stw_mux_sel strip_l3_off,
			   bool pppoe_encap, bool l4_napt)
{
	struct mod_recipe rcp;
	s32 ret = 0;
	enum mod_eu_stw_mux_sel l3_off = EU_STW_SEL_L3_OFF0;

	/* ================= */
	/* Map dpu registers */
	/* ================= */
	s32 stw_pkt_len        = DPU_RCP_REG0_B0;
	s32 stw_off_l3         = DPU_RCP_REG1_B0;
	s32 si_l3_tot_len_diff = DPU_RCP_REG2_B0;
	s32 si_nhdr_csum       = DPU_RCP_REG2_B2;
	s32 pkt_l3_csum        = DPU_RCP_REG3_B0;
	s32 pkt_l3_tot_len     = DPU_RCP_REG4_B0;
	s32 pppoe_diff         = DPU_RCP_REG5_B0;
	s32 pppoe_len          = DPU_RCP_REG5_B1;
	s32 si_l4_csum_delta   = DPU_RCP_REG6_B0;
	s32 pkt_l4_csum        = DPU_RCP_REG6_B2;
	s32 si_l4_ports        = DPU_RCP_REG7_B0;
	s32 nhdr_id            = DPU_RCP_REG8_B0;

	/* USE registers R12 - R15 with extra caution as they are
	 * the registers to compare to for the alu conditions
	 * R12 - in used by ALU0 for pppoe and MUST be zero in
	 *       the time of the calculation
	 * R13 - not used
	 * R14 - not used
	 * R15 - not used
	 */

	if (unlikely(rcp_idx > MOD_RCP_LAST)) {
		pr_err("error: invalid recipe id, %d\n", rcp_idx);
		return -EINVAL;
	}

	pr_debug("Configuring recipe %s(%u)\n", mod_rcp_to_str(rcp_idx),
		 rcp_idx);
	memset(&rcp, 0, sizeof(rcp));
	rcp.index = rcp_idx;

	/*==========================================*/
	/* copy from sce mux to dpu registers (sce) */
	/*==========================================*/
	/* Fetch SI info: L3 csum delta & L4 checksum delta */
	if (l4_napt) {
		ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l4_csum_delta,
					      SCE_SI_SEL_L4_CSUM_DELTA);
		ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l4_ports,
					      SCE_SI_SEL_NEW_SRC_PORT);
	}
	/*======================*/
	/* STW info: packet len */
	/*======================*/
	ret |= mod_sce_mux_to_reg_cpy(&rcp, stw_pkt_len, SCE_STW_SEL_PKT_LEN);
	/*======================*/
	/* STW info: offset L3  */
	/*======================*/
	ret |= mod_sce_mux_to_reg_cpy(&rcp, stw_off_l3, SCE_STW_SEL_L3_OFF0);
	/*========================*/
	/* SI info: tot_len_diff  */
	/*========================*/
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l3_tot_len_diff,
				      SCE_SI_SEL_TOT_LEN_DIFF);

	/* Fetch SI info: pppoe diff and pppoe length fields */
	if (pppoe_encap)
		ret |= mod_sce_mux_to_reg_cpy(&rcp, pppoe_diff,
					      SCE_SI_SEL_PPPOE_DIFF);

	/*===========================================*/
	/* copy from packet to dpu registers (store) */
	/*===========================================*/
	if (nhdr == RCP_HDR_NO_IP || pppoe_encap) {
		/*====================*/
		/* PKT: L3 total_len  */
		/*====================*/
		ret |= mod_pkt_to_reg_cpy(&rcp, pkt_l3_tot_len, l3_off,
					  offsetof(struct iphdr, tot_len),
					  sizeof_field(struct iphdr, tot_len));

		/*===================*/
		/* PKT: L3 checksum  */
		/*===================*/
		if (nhdr == RCP_HDR_NO_IP)
			ret |= mod_pkt_to_reg_cpy(&rcp, pkt_l3_csum, l3_off,
						  offsetof(struct iphdr, check),
						  sizeof_field(struct iphdr,
							       check));
	}

	if (l4_napt) {
		ret |= mod_pkt_to_reg_cpy(
				&rcp, pkt_l4_csum, STW_NEXT_LAYER(l3_off),
				MOD_SI_PKT_OFF,
				sizeof_field(struct udphdr, check));
	}

	/*=========================*/
	/*      ALU operations     */
	/*=========================*/
	/*  |--------------------------------|--------------------------------|
	 *  |  ALU0 for RCP_HDR_NO_IP    |  ALU0 for RCP_HDR_IPV4       |
	 *  |--------------------------------|--------------------------------|
	 *  | sync                           | off_l3 = pkt_len - off_l3      |
	 *  | off_l3 = pkt_len - off_l3      | pkt_len = off_l3 + tot_len_diff|
	 *  | pkt_len = off_l3 + tot_len_diff| inc global_id                  |
	 *  | csum += old total length       | csum -= global_id              |
	 *  | csum -= pkt_len (new tot_len)  | csum -= pkt_len (new tot_len)  |
	 *  | pppoe len (pppoe_encap)        | pppoe len (pppoe_encap)        |
	 *  | l4 csum calculation            |                                |
	 *  |--------------------------------|--------------------------------|
	 */
	/* Synchronize ALU to last fetched info of STORE command */
	if (l4_napt) {
		ret |= mod_alu_synch(EU_ALU0, &rcp, STW_NEXT_LAYER(l3_off),
				     MOD_SI_PKT_OFF,
				     sizeof_field(struct udphdr, check));
	} else if (nhdr == RCP_HDR_NO_IP) {
		ret |= mod_alu_synch(EU_ALU0, &rcp, l3_off,
				     offsetof(struct iphdr, check),
				     sizeof_field(struct iphdr, check));
	}

	/*=======================================*/
	/* stw_off_l3 = stw_pkt_len - stw_off_l3 */
	/*=======================================*/
	ret |= mod_alu_sub(EU_ALU0, &rcp, stw_pkt_len, 2, stw_off_l3, 2,
			   stw_off_l3);
	/*===============================================*/
	/* stw_pkt_len = stw_off_l3 - si_l3_tot_len_diff */
	/*===============================================*/
	ret |= mod_alu_add(EU_ALU0, &rcp, stw_off_l3, 2,
			   si_l3_tot_len_diff, 1, stw_pkt_len);
	if (nhdr == RCP_HDR_IPV4) {
		/*================*/
		/* nhdr_id = rand */
		/*================*/
		ret |= mod_alu_rand(EU_ALU0, &rcp, nhdr_id,
				    sizeof_field(struct iphdr, id));
		/*=========================*/
		/* si_nhdr_csum -= nhdr_id */
		/*=========================*/
		ret |= mod_alu_csum16_sub(EU_ALU0, &rcp, nhdr_id,
					  si_nhdr_csum, si_nhdr_csum);
		/*=============================*/
		/* si_nhdr_csum -= stw_pkt_len */
		/*=============================*/
		ret |= mod_alu_csum16_sub(EU_ALU0, &rcp, stw_pkt_len,
					  si_nhdr_csum, si_nhdr_csum);
	} else if (nhdr == RCP_HDR_NO_IP) {
		/*===============================*/
		/* pkt_l3_csum += pkt_l3_tot_len */
		/*===============================*/
		ret |= mod_alu_csum16_add(EU_ALU0, &rcp, pkt_l3_tot_len,
					  pkt_l3_csum, pkt_l3_csum);
		/*============================*/
		/* pkt_l3_csum -= stw_pkt_len */
		/*============================*/
		ret |= mod_alu_csum16_sub(EU_ALU0, &rcp, stw_pkt_len,
					  pkt_l3_csum, pkt_l3_csum);
	}

	/* Update L4 checksum */
	if (l4_napt) {
		/* packet csum += csum ? si_csum_delta
		 * we do not support zero csum for UDP, we always
		 * calculate the csum under ESP tunnel
		 */
		ret |= mod_alu_csum16_add(EU_ALU0, &rcp, si_l4_csum_delta,
					  pkt_l4_csum, pkt_l4_csum);
	}

	/* pppoe_len = pppoe_diff ? pppoe_diff + stw len : l2_org_val */
	if (pppoe_encap) {
		ret |= mod_alu_add_cond(EU_ALU0, &rcp, pppoe_diff, 1,
					stw_pkt_len, 2, pppoe_len,
					EU_ALU_L0_BIG_R2,
					EU_ALU_COND_MET);
	}

	/*==========================================*/
	/* copy from dpu registers to packet (reps0) */
	/*==========================================*/
	if (nhdr == RCP_HDR_NO_IP) {
		/* Replace L3 total length */
		ret |= mod_reg_to_hdr_cpy(&rcp, stw_pkt_len, l3_off,
					  offsetof(struct iphdr, tot_len),
					  sizeof_field(struct iphdr, tot_len));
		/* Replace L3 CSUM */
		ret |= mod_reg_to_hdr_cpy(&rcp, pkt_l3_csum, l3_off,
					  offsetof(struct iphdr, check),
					  sizeof_field(struct iphdr, check));
	}
	/* update l4 header */
	if (l4_napt) {
		/* Replace L4 PORTS */
		ret |= mod_reg_to_hdr_cpy(&rcp, si_l4_ports,
					  STW_NEXT_LAYER(l3_off),
					  offsetof(struct udphdr, source),
					  sizeof_field(struct udphdr, source) +
					  sizeof_field(struct udphdr, dest));

		/* Replace L4 CSUM */
		ret |= mod_reg_to_hdr_cpy(&rcp, pkt_l4_csum,
					  STW_NEXT_LAYER(l3_off),
					  MOD_SI_PKT_OFF,
					  sizeof_field(struct udphdr, check));
	}

	/*============================================*/
	/* copy from si bce (sram) to packet (insrep) */
	/*============================================*/
	/* replace old L2 with new header */
	ret |= mod_hdr_rplc(&rcp, BCE_OFF_0, strip_l3_off);

	/*======================================================*/
	/* copy from dpu registers to packet new header (reps1) */
	/*======================================================*/
	if (pppoe_encap)
		ret |= mod_reg_to_new_hdr_cpy(
			&rcp, pppoe_len, EU_SI_SEL_PPPOE_OFF,
			offsetof(struct pppoe_hdr, length),
			sizeof_field(struct pppoe_hdr, length));

	if (nhdr == RCP_HDR_IPV4) {
		/* Update ipv4 id */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, nhdr_id,
					      MOD_PKT_OFF_L3_EXT,
					      offsetof(struct iphdr, id),
					      sizeof_field(struct iphdr, id));

		/* Update ipv4 total length */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, stw_pkt_len,
					      MOD_PKT_OFF_L3_EXT,
					      offsetof(struct iphdr, tot_len),
					      sizeof_field(struct iphdr,
							   tot_len));

		/* Replace L3 CSUM */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, si_nhdr_csum,
					      MOD_PKT_OFF_L3_EXT,
					      offsetof(struct iphdr, check),
					      sizeof_field(struct iphdr,
							   check));
	}

	/*=================================================*/
	/* complete recipe with nop command and 'last' bit */
	/*=================================================*/
	ret |= mod_rcp_complete(&rcp);
	if (unlikely(ret)) {
		pr_err("Failed to create recipe %s(%d)\n",
		       mod_rcp_to_str(rcp_idx), rcp_idx);
	}

	return ret;
}

/**
 * @brief Creates recipe for esp traffic
 * @param rcp_idx Recipe index to create
 * @param nhdr New IP header. NO_IP for flows without encapsulation
 * @return s32 0 on success, error code otherwise
 */
static s32 mod_rcp_esp_ipv6_set(s32 rcp_idx, enum rcp_hdr_ip_type nhdr,
				enum mod_eu_stw_mux_sel strip_l3_off,
				bool pppoe_encap)
{
	struct mod_recipe rcp;
	s32 ret = 0;
	enum mod_eu_stw_mux_sel l3_off = EU_STW_SEL_L3_OFF0;

	/* ================= */
	/* Map dpu registers */
	/* ================= */
	s32 stw_pkt_len        = DPU_RCP_REG0_B0;
	s32 stw_off_l3         = DPU_RCP_REG1_B0;
	s32 si_l3_tot_len_diff = DPU_RCP_REG2_B0;
	s32 pkt_l3_tot_len     = DPU_RCP_REG4_B0;
	s32 pppoe_diff         = DPU_RCP_REG5_B0;
	s32 pppoe_len          = DPU_RCP_REG5_B1;

	/* USE registers R12 - R15 with extra caution as they are
	 * the registers to compare to for the alu conditions
	 * R12 - in used by ALU0 for pppoe and MUST be zero in
	 *       the time of the calculation
	 * R13 - not used
	 * R14 - not used
	 * R15 - not used
	 */

	if (unlikely(rcp_idx > MOD_RCP_LAST)) {
		pr_err("error: invalid recipe id, %d\n", rcp_idx);
		return -EINVAL;
	}

	pr_debug("Configuring recipe %s(%u)\n", mod_rcp_to_str(rcp_idx),
		 rcp_idx);
	memset(&rcp, 0, sizeof(rcp));
	rcp.index = rcp_idx;

	/*==========================================*/
	/* copy from sce mux to dpu registers (sce) */
	/*==========================================*/
	/*======================*/
	/* STW info: packet len */
	/*======================*/
	ret |= mod_sce_mux_to_reg_cpy(&rcp, stw_pkt_len, SCE_STW_SEL_PKT_LEN);
	/*======================*/
	/* STW info: offset L3  */
	/*======================*/
	ret |= mod_sce_mux_to_reg_cpy(&rcp, stw_off_l3, SCE_STW_SEL_L3_OFF0);
	/*========================*/
	/* SI info: tot_len_diff  */
	/*========================*/
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l3_tot_len_diff,
				      SCE_SI_SEL_TOT_LEN_DIFF);

	/* Fetch SI info: pppoe diff and pppoe length fields */
	if (pppoe_encap)
		ret |= mod_sce_mux_to_reg_cpy(&rcp, pppoe_diff,
					      SCE_SI_SEL_PPPOE_DIFF);

	/*===========================================*/
	/* copy from packet to dpu registers (store) */
	/*===========================================*/
	if (nhdr == RCP_HDR_NO_IP || pppoe_encap) {
		/*=====================*/
		/* PKT: L3 payload len */
		/*=====================*/
		ret |= mod_pkt_to_reg_cpy(&rcp, pkt_l3_tot_len, l3_off,
					  offsetof(struct ipv6hdr, payload_len),
					  sizeof_field(struct ipv6hdr,
						       payload_len));
	}

	/*=========================*/
	/*      ALU operations     */
	/*=========================*/
	/*  |--------------------------------|--------------------------------|
	 *  |  ALU0 for RCP_HDR_NO_IP    |  ALU0 for RCP_HDR_IPV6       |
	 *  |--------------------------------|--------------------------------|
	 *  | sync                           | off_l3 = pkt_len - off_l3      |
	 *  | off_l3 = pkt_len - off_l3      | pkt_len = off_l3 + tot_len_diff|
	 *  | pkt_len = off_l3 + tot_len_diff| pppoe len (pppoe_encap)        |
	 *  | pppoe len (pppoe_encap)        |                                |
	 *  |--------------------------------|--------------------------------|
	 */
	if (nhdr == RCP_HDR_NO_IP || pppoe_encap) {
		/* Synchronize ALU to last fetched info of STORE command */
		ret |= mod_alu_synch(EU_ALU0, &rcp, l3_off,
				     offsetof(struct ipv6hdr, payload_len),
				     sizeof_field(struct ipv6hdr, payload_len));
	}
	/*=======================================*/
	/* stw_off_l3 = stw_pkt_len - stw_off_l3 */
	/*=======================================*/
	ret |= mod_alu_sub(EU_ALU0, &rcp, stw_pkt_len, 2, stw_off_l3, 1,
			   stw_off_l3);

	/*===============================================*/
	/* stw_pkt_len = stw_off_l3 - si_l3_tot_len_diff */
	/*===============================================*/
	ret |= mod_alu_sub(EU_ALU0, &rcp, stw_off_l3, 2, si_l3_tot_len_diff, 1,
			   stw_pkt_len);

	/* pppoe_len = pppoe_diff ? pppoe_diff + stw len : l2_org_val */
	if (pppoe_encap) {
		ret |= mod_alu_add_cond(EU_ALU0, &rcp, pppoe_diff, 1,
					stw_pkt_len, 2, pppoe_len,
					EU_ALU_L0_BIG_R2,
					EU_ALU_COND_MET);
	}

	/*==========================================*/
	/* copy from dpu registers to packet (reps0) */
	/*==========================================*/
	if (nhdr == RCP_HDR_NO_IP) {
		/* Replace L3 total length */
		ret |= mod_reg_to_hdr_cpy(&rcp, stw_pkt_len, l3_off,
					  offsetof(struct ipv6hdr, payload_len),
					  sizeof_field(struct ipv6hdr,
						       payload_len));
	}

	/*============================================*/
	/* copy from si bce (sram) to packet (insrep) */
	/*============================================*/
	/* replace old L2 with new header */
	ret |= mod_hdr_rplc(&rcp, BCE_OFF_0, strip_l3_off);

	/*======================================================*/
	/* copy from dpu registers to packet new header (reps1) */
	/*======================================================*/
	if (pppoe_encap)
		ret |= mod_reg_to_new_hdr_cpy(
			&rcp, pppoe_len, EU_SI_SEL_PPPOE_OFF,
			offsetof(struct pppoe_hdr, length),
			sizeof_field(struct pppoe_hdr, length));

	if (nhdr == RCP_HDR_IPV6) {
		/* Update ipv6 total length */
		ret |= mod_reg_to_new_hdr_cpy(
			&rcp, stw_pkt_len, MOD_PKT_OFF_L3_EXT,
			offsetof(struct ipv6hdr, payload_len),
			sizeof_field(struct ipv6hdr, payload_len));
	}

	/*=================================================*/
	/* complete recipe with nop command and 'last' bit */
	/*=================================================*/
	ret |= mod_rcp_complete(&rcp);
	if (unlikely(ret)) {
		pr_err("Failed to create recipe %s(%d)\n",
		       mod_rcp_to_str(rcp_idx), rcp_idx);
	}

	return ret;
}

static s32 mod_rcp_ipv6_vxlan_routed_set(s32 rcp_idx, enum mod_eu_stw_mux_sel l3_off,
				   enum rcp_hdr_ip_type nhdr,
				   bool pppoe_encap, bool csum_enable)
{
	struct mod_recipe rcp;
	s32 ret = 0;

	/*===================*/
	/* Map dpu registers */
	/*===================*/
	s32 si_ttl_diff      = DPU_RCP_REG0_B0;
	s32 pppoe_diff       = DPU_RCP_REG0_B1;
	s32 pppoe_len        = DPU_RCP_REG0_B2;
	s32 si_tot_len_diff  = DPU_RCP_REG1_B0;
	s32 si_nhdr_csum     = DPU_RCP_REG1_B2;
	s32 pkt_hop_limit    = DPU_RCP_REG2_B0;
	s32 pkt_payload_len  = DPU_RCP_REG2_B2;
	s32 nhdr_payload_len = DPU_RCP_REG4_B2;
	s32 nhdr_id          = DPU_RCP_REG5_B0;

	/* VXLAN L4 updates */
	s32 outer_l4_new_csum	= DPU_RCP_REG10_B0;
	s32 outer_l3_hdr_size   = DPU_RCP_REG10_B2;
	s32 outer_l4_total_len	= DPU_RCP_REG11_B0;
	/* DO NOT USE registers R12 and R14 as they are used as R2 for
	 * the alu conditions and MUST be zero
	 */

	if (unlikely(rcp_idx > MOD_RCP_LAST)) {
		pr_err("error: invalid recipe id, %d\n", rcp_idx);
		return -EINVAL;
	}

	pr_debug("Configuring recipe %s(%u)\n", mod_rcp_to_str(rcp_idx),
		 rcp_idx);
	memset(&rcp, 0, sizeof(rcp));
	rcp.index = rcp_idx;

	/*==========================================*/
	/* copy from sce mux to dpu registers (sce) */
	/*==========================================*/
	/* Fetch SI info: total length diff, tos, and new header csum */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_tot_len_diff,
				      SCE_SI_SEL_TOT_LEN_DIFF);

	/* Fetch SI info: hop limit diff, pppoe diff and pppoe length fields */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_ttl_diff, SCE_SI_SEL_TTL);

	if (csum_enable)
		/* VXLAN - read checksum from si */
		ret |= mod_sce_mux_to_reg_cpy(&rcp, outer_l4_new_csum,
					      SCE_SI_SEL_L4_CSUM);
	/*===========================================*/
	/* copy from packet to dpu registers (store) */
	/*===========================================*/
	/* store hop limit, next header (not used), and payload length
	 * we always fetch all 3 cause we anyway need the hop limit
	 */
	ret |= mod_pkt_to_reg_cpy(&rcp, pkt_hop_limit, l3_off,
				  offsetof(struct ipv6hdr, payload_len),
				  sizeof_field(struct ipv6hdr, payload_len) +
				  sizeof_field(struct ipv6hdr, nexthdr) +
				  sizeof_field(struct ipv6hdr, hop_limit));

	/*=========================*/
	/*      ALU operations     */
	/*=========================*/
	/*  |------------------------------- |-------------------------------|
	 *  |      ALU0                      |      ALU1                     |
	 *  |------------------------------- |-------------------------------|
	 *  | sync                           |                               |
	 *  | HOPL decrement                 | sync (nhdr or pppoe)          |
	 *  | ID inc (nhdr v4)               | payload len fix (non-tunnels) |
	 *  | csum ID (nhdr v4)              | payload len fix (non-tunnels) |
	 *  | csum pkt len (nhdr v4)         | nhdr pkt len (nhdr or pppoe)  |
	 *  |                                | pppoe len (nhdr or pppoe)     |
	 *  |------------------------        |-outer_l4_total_len------------|
	 *  |------------------------        |-outer_l4_new_csum-1-----------|
	 *  |------------------------        |-outer_l4_new_csum-2-----------|
	 *  |------------------------        |-outer_l4_new_csum-3-----------|
	 */
	/* Synchronize ALU to latest fetched information of STORE command */
	ret |= mod_alu_synch(EU_ALU0, &rcp, l3_off,
			     offsetof(struct ipv6hdr, hop_limit),
			     sizeof_field(struct ipv6hdr, hop_limit));

	if (nhdr != RCP_HDR_NO_IP || pppoe_encap)
		ret |= mod_alu_synch(EU_ALU1, &rcp, l3_off,
				     offsetof(struct ipv6hdr, hop_limit),
				     sizeof_field(struct ipv6hdr, hop_limit));

	/* decrement hop limit */
	ret |= mod_alu_sub(EU_ALU0, &rcp, pkt_hop_limit,
			   sizeof_field(struct ipv6hdr, hop_limit), si_ttl_diff,
			   sizeof_field(struct ipv6hdr, hop_limit),
			   pkt_hop_limit);

	/* Calculate new header total and/or pppoe length */
	if (nhdr != RCP_HDR_NO_IP || pppoe_encap) {
		/* add session total len diff */
		ret |= mod_alu_add(EU_ALU1, &rcp, pkt_payload_len, 2,
				   si_tot_len_diff, 1, nhdr_payload_len);

		/* pppoe_len = pppoe_diff ? pppoe_diff + l3 len : l2_org_val */
		if (pppoe_encap)
			ret |= mod_alu_add_cond(EU_ALU1, &rcp, pppoe_diff, 1,
						nhdr_payload_len, 2, pppoe_len,
						EU_ALU_L0_BIG_R2,
						EU_ALU_COND_MET);
	}

	/* for V4 update csum, id */
	if (nhdr == RCP_HDR_IPV4) {
		ret |= mod_alu_rand(EU_ALU0, &rcp, nhdr_id,
				    sizeof_field(struct iphdr, id));

		/* Update the external L3 csum due to global id */
		ret |= mod_alu_csum16_sub(EU_ALU0, &rcp, nhdr_id,
					  si_nhdr_csum, si_nhdr_csum);

		/* Calculate external L3 csum due to total len.
		 * this MUST be in sync with ALU1 operations
		 * Note: This assumes the total length in the new ipv4
		 *       template header is zero
		 */
		ret |= mod_alu_csum16_sub(EU_ALU0, &rcp, nhdr_payload_len,
					  si_nhdr_csum, si_nhdr_csum);
	}

	/* VXLAN L4 CS update */
	/* calculate Outer L4 length  =  L3 length - L3 header size */
	ret |= mod_alu_sub(EU_ALU1, &rcp, nhdr_payload_len, 2,
			outer_l3_hdr_size, 2, outer_l4_total_len);
	/* for full explaination about outer L4 CS calculation
		see document */
	if (csum_enable) {
		/* Add Outer L4 to L4 CSUM */
		ret |= mod_alu_csum16_sub(EU_ALU1, &rcp, outer_l4_total_len,
				outer_l4_new_csum, outer_l4_new_csum);
		/* Add Outer L4 to L4 CSUM */
		ret |= mod_alu_csum16_sub(EU_ALU1, &rcp, outer_l4_total_len,
				outer_l4_new_csum, outer_l4_new_csum);
		/* Add hop limit to L4 CSUM */
		ret |= mod_alu_csum16_sub(EU_ALU1, &rcp, pkt_hop_limit,
				outer_l4_new_csum, outer_l4_new_csum);
	}

	/*==========================================*/
	/* copy from dpu registers to packet (reps) */
	/*==========================================*/
	/* Replace L3 hop limit */
	ret |= mod_reg_to_hdr_cpy(&rcp, pkt_hop_limit, l3_off,
				  offsetof(struct ipv6hdr, hop_limit),
				  sizeof_field(struct ipv6hdr, hop_limit));

	/*============================================*/
	/* copy from si bce (sram) to packet (insrep) */
	/*============================================*/
	/* replace old L2 with new header */
	ret |= mod_hdr_rplc(&rcp, BCE_OFF_0, l3_off);

	/*======================================================*/
	/* copy from dpu registers to packet new header (reps1) */
	/*======================================================*/
	if (pppoe_encap)
		ret |= mod_reg_to_new_hdr_cpy(&rcp, pppoe_len,
					      EU_SI_SEL_PPPOE_OFF,
					      offsetof(struct pppoe_hdr,
						       length),
					      sizeof_field(struct pppoe_hdr,
							   length));

	if (nhdr == RCP_HDR_IPV6) {
		/* Replace payload length with inner payload length */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, nhdr_payload_len,
					      MOD_PKT_OFF_L3_EXT,
					      offsetof(struct ipv6hdr,
						       payload_len),
					      sizeof_field(struct ipv6hdr,
							   payload_len));
	} else if (nhdr == RCP_HDR_IPV4) {
		/* Update ipv4 id */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, nhdr_id,
					      MOD_PKT_OFF_L3_EXT,
					      offsetof(struct iphdr, id),
					      sizeof_field(struct iphdr, id));

		/* Update ipv4 total length */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, nhdr_payload_len,
					      MOD_PKT_OFF_L3_EXT,
					      offsetof(struct iphdr, tot_len),
					      sizeof_field(struct iphdr,
							   tot_len));

		/* Replace L3 CSUM */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, si_nhdr_csum,
					      MOD_PKT_OFF_L3_EXT,
					      offsetof(struct iphdr, check),
					      sizeof_field(struct iphdr,
							   check));
	}

	/* VXLAN  L4 UDP checksum and length set */
	if (nhdr == RCP_HDR_IPV6) {
		/* set outer L4 length field */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, outer_l4_total_len,
					MOD_PKT_OFF_L3_EXT,
					sizeof(struct ipv6hdr) +
					offsetof(struct udphdr, len),
					sizeof_field(struct udphdr,
						len));
		/* set outer L4 checksum field */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, outer_l4_new_csum,
					MOD_PKT_OFF_L3_EXT,
					sizeof(struct ipv6hdr) +
					offsetof(struct udphdr, check),
					sizeof_field(struct udphdr,
						check));
	} else {
		/* set outer L4 length field */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, outer_l4_total_len,
					MOD_PKT_OFF_L3_EXT,
					sizeof(struct iphdr) +
					offsetof(struct udphdr, len),
					sizeof_field(struct udphdr,
						len));
		/* set outer L4 checksum field */
		if (csum_enable)
			ret |= mod_reg_to_new_hdr_cpy(&rcp, outer_l4_new_csum,
					MOD_PKT_OFF_L3_EXT,
					sizeof(struct iphdr) +
					offsetof(struct udphdr, check),
					sizeof_field(struct udphdr,
						check));
	}

	/*=================================================*/
	/* complete recipe with nop command and 'last' bit */
	/*=================================================*/
	ret |= mod_rcp_complete(&rcp);
	if (unlikely(ret)) {
		pr_err("Failed to create recipe %s(%d)\n",
		       mod_rcp_to_str(rcp_idx), rcp_idx);
	}

	return ret;
}

static s32 mod_rcp_vxlan_routed_set(s32 rcp_idx, enum mod_eu_stw_mux_sel l3_off,
				   bool l4_napt, enum rcp_hdr_ip_type nhdr,
				   bool pppoe_encap, bool csum_enable)
{
	struct mod_recipe rcp;
	s32 ret = 0;

	/* ================= */
	/* Map dpu registers */
	/* ================= */
	s32 si_l4_csum_delta   = DPU_RCP_REG0_B0;
	s32 si_l3_csum_delta   = DPU_RCP_REG0_B2;
	s32 si_l3_tot_len_diff = DPU_RCP_REG1_B0;
	s32 si_l3_tos          = DPU_RCP_REG1_B1;
	s32 si_nhdr_csum       = DPU_RCP_REG1_B2;
	s32 si_l3_ttl_diff     = DPU_RCP_REG2_B0;
	s32 pppoe_diff         = DPU_RCP_REG2_B1;
	s32 pppoe_len          = DPU_RCP_REG2_B2;
	s32 si_l4_ports        = DPU_RCP_REG3_B0;
	s32 pkt_l3_csum        = DPU_RCP_REG4_B0;
	s32 pkt_l3_ttl         = DPU_RCP_REG4_B3;
	s32 pkt_l4_csum        = DPU_RCP_REG5_B2;
	s32 si_l4_csum_zero    = DPU_RCP_REG7_B0;
	s32 stw_pkt_len        = DPU_RCP_REG8_B0;
	s32 stw_off_l3         = DPU_RCP_REG9_B0;
	s32 nhdr_l3_tot_len    = DPU_RCP_REG9_B2;
	s32 nhdr_id            = DPU_RCP_REG10_B0;

	s32 pkt_l3_tot_len     = DPU_RCP_REG13_B0_COND;
	/* VXLAN - L4 parameters  */
	s32 outer_l4_new_csum	= DPU_RCP_REG10_B0;
	s32 outer_l3_hdr_size   = DPU_RCP_REG10_B2;
	s32 outer_l4_total_len	= DPU_RCP_REG11_B0;

	/* USE registers R12 - R15 with extra caution as they are
	 * the registers to compare to for the alu conditions
	 * R12 - in used by ALU0 for pppoe and l4 csum calculation and MUST
	 *       be zero in the time of the calculation
	 * R13 - in used by ALU0 for none-tunnel total len adaptation,
	 *       used to calculate the total len from packet len only in case
	 *        the result is smaller than the original value,
	 *       this condition protecting the padding use case
	 *       but supporting the IPSec inbound 2nd round modification
	 * R14 - not used
	 * R15 - not used
	 */

	if (unlikely(rcp_idx > MOD_RCP_LAST)) {
		pr_err("error: invalid recipe id, %d\n", rcp_idx);
		return -EINVAL;
	}

	pr_debug("Configuring recipe %s(%u)\n", mod_rcp_to_str(rcp_idx),
		 rcp_idx);
	memset(&rcp, 0, sizeof(rcp));
	rcp.index = rcp_idx;

	/*==========================================*/
	/* copy from sce mux to dpu registers (sce) */
	/*==========================================*/
	/* Fetch SI info: L3 csum delta & L4 checksum delta */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l4_csum_delta,
				      SCE_SI_SEL_L4_CSUM_DELTA);

	/* Fetch SI info: total length diff, tos, and new header csum */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l3_tot_len_diff,
				      SCE_SI_SEL_TOT_LEN_DIFF);

	/* Fetch SI info: ttl, pppoe diff and pppoe length fields */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l3_ttl_diff, SCE_SI_SEL_TTL);

	/* Fetch SI info: L4 ports and l4 csum zero */
	if (l4_napt) {
		ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l4_ports,
					      SCE_SI_SEL_NEW_SRC_PORT);
		ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l4_csum_zero,
					      SCE_SI_SEL_L4_CSUM_ZERO);
	}


	/*======================*/
	/* STW info: packet len */
	/*======================*/
	ret |= mod_sce_mux_to_reg_cpy(&rcp, stw_pkt_len, SCE_STW_SEL_PKT_LEN);
	/*======================*/
	/* STW info: offset L3  */
	/*======================*/
	ret |= mod_sce_mux_to_reg_cpy(&rcp, stw_off_l3, SCE_STW_SEL_L3_OFF0);

	/* New L4 CSUM */
	if (csum_enable)
		ret |= mod_sce_mux_to_reg_cpy(&rcp, outer_l4_new_csum,
			SCE_SI_SEL_L4_CSUM);

	/*===========================================*/
	/* copy from packet to dpu registers (store) */
	/*===========================================*/
	/* store L3 total_len */
	/* @TODO: can't store total len and id fields both in a
	 * single command since the next command should store
	 * the ttl field and the 'stall' functionality is not
	 * working as expected
	 */
	ret |= mod_pkt_to_reg_cpy(&rcp, pkt_l3_tot_len, l3_off, 0, 4);

	/* store L3 TTL and L3 checksum */
	ret |= mod_pkt_to_reg_cpy(&rcp, pkt_l3_csum, l3_off,
				  offsetof(struct iphdr, ttl),
				  sizeof_field(struct iphdr, ttl) +
				  sizeof_field(struct iphdr, protocol) +
				  sizeof_field(struct iphdr, check));

	/* store L4 checksum */
	if (l4_napt) {
		ret |= mod_pkt_to_reg_cpy(&rcp, pkt_l4_csum,
					  STW_NEXT_LAYER(l3_off),
					  MOD_SI_PKT_OFF,
					  sizeof_field(struct udphdr, check));

	}

	/*=========================*/
	/*      ALU operations     */
	/*=========================*/

	/* |---------------------------------|---------------------------------|
	 * |      ALU0                       |      ALU1                       |
	 * |---------------------------------|---------------------------------|
	 * | sync to ip csum                 |   sync to L4 dport (napt)       |
	 * |                                 |   sync to ip csum (no napt)     |
	 * |---------------------------------|---------------------------------|
	 * | ID inc (nhdr v4)                |   TTL decrement                 |
	 * |---------------------------------|---------------------------------|
	 * | pkt ip csum + si delta          |   pkt l4 csum step 1            |
	 * |---------------------------------|---------------------------------|
	 * | pkt ip csum + old tot len       |   pkt l4 csum step 2            |
	 * | (non-tunnel)                    |                                 |
	 * |---------------------------------|---------------------------------|
	 * | pkt ip len re-calculate         |   csum ID (nhdr v4)             |
	 * | (non-tunnel)                    |                                 |
	 * |---------------------------------|---------------------------------|
	 * | pkt ip csum - new tot len       |   csum tot len (nhdr v4)        |
	 * | (non-tunnel)                    |                                 |
	 * |---------------------------------|---------------------------------|
	 * | nhdr tot len                    |   outer_l4_total_len update     |
	 * |   (nhdr or pppoe_encap)         |                                 |
	 * |---------------------------------|---------------------------------|
	 * | pppoe pkt len (pppoe_encap)     |   outer_l4_new_csum update      |
	 * |---------------------------------|---------------------------------|
	 */

	/* Synchronize ALU0 to latest fetched information of STORE command */
	ret |= mod_alu_synch(EU_ALU0, &rcp, l3_off,
			     offsetof(struct iphdr, check),
			     sizeof_field(struct iphdr, check));
	if (l4_napt) {
		/* Synchronize ALU1 to L4 checksum field,
		 * The offset value is taken from the SI and it will be
		 * Different value for UDP and TCP
		 */
		ret |= mod_alu_synch(EU_ALU1, &rcp, STW_NEXT_LAYER(l3_off),
				     MOD_SI_PKT_OFF,
				     sizeof_field(struct udphdr, check));
	} else {
		ret |= mod_alu_synch(EU_ALU1, &rcp, l3_off,
				     offsetof(struct iphdr, check),
				     sizeof_field(struct iphdr, check));
	}

	if (nhdr == RCP_HDR_IPV4) {
		ret |= mod_alu_rand(EU_ALU0, &rcp, nhdr_id,
				    sizeof_field(struct iphdr, id));
	}

	/* Recalculate L3 checksum */
	ret |= mod_alu_csum16_add(EU_ALU0, &rcp, si_l3_csum_delta, pkt_l3_csum,
				  pkt_l3_csum);

	/* Calculate new header total length */
	if (nhdr != RCP_HDR_NO_IP || pppoe_encap) {
		ret |= mod_alu_add(EU_ALU0, &rcp, si_l3_tot_len_diff, 1,
				   pkt_l3_tot_len, 2, nhdr_l3_tot_len);

		/* pppoe_len = pppoe_diff ? pppoe_diff + l3 len : l2_org_val */
		if (pppoe_encap) {
			ret |= mod_alu_add_cond(EU_ALU0, &rcp, pppoe_diff, 1,
						nhdr_l3_tot_len, 2, pppoe_len,
						EU_ALU_L0_BIG_R2,
						EU_ALU_COND_MET);
		}
	}

	/* decrement TTL */
	ret |= mod_alu_sub(EU_ALU1, &rcp, pkt_l3_ttl,
			   sizeof_field(struct iphdr, ttl), si_l3_ttl_diff,
			   sizeof_field(struct iphdr, ttl), pkt_l3_ttl);

	/* Recalculate L4 checksum */
	if (l4_napt) {
		/* in general, the checksum calculation should be as follow:
		 *   udp: csum = csum ? csum + csum_delta : csum
		 *   tcp: csum = csum + csum_delta
		 * instead, we have csum_zero value which is set as follow
		 *   udp: csum_zero = csum_delta
		 *   tcp: csum_zero = 0
		 * and the calculation is as follow for both:
		 *   csum = csum + csum_delta
		 *   csum = (csum - csum_zero) ? csum : csum - csum_zero;
		 */
		ret |= mod_alu_csum16_add(EU_ALU1, &rcp, si_l4_csum_delta,
					  pkt_l4_csum, pkt_l4_csum);
		ret |= mod_alu_csum16_sub_cond(EU_ALU1, &rcp, si_l4_csum_zero,
					       pkt_l4_csum, pkt_l4_csum,
					       EU_ALU_RES_EQ_R2,
					       EU_ALU_COND_MET);
	}

	if (nhdr == RCP_HDR_IPV4) {
		/* Update the external L3 csum due to global id */
		ret |= mod_alu_csum16_sub(EU_ALU1, &rcp, nhdr_id,
					  si_nhdr_csum, si_nhdr_csum);

		/* Update the external L3 csum due to total len */
		ret |= mod_alu_csum16_sub(EU_ALU1, &rcp, nhdr_l3_tot_len,
					  si_nhdr_csum, si_nhdr_csum);
	}

	/* VXLAN L4 UDP updates */
	/* Update Outer L4 Length  L4_length = L3_total_length - L3 header length */
	ret |= mod_alu_sub(EU_ALU1, &rcp, nhdr_l3_tot_len,
			sizeof_field(struct iphdr, tot_len),
			outer_l3_hdr_size, sizeof_field(struct iphdr, tot_len),
			outer_l4_total_len);
	/* Update L4 CSUM with L4 length*/
	if (csum_enable)
		ret |= mod_alu_csum16_sub(EU_ALU1, &rcp, outer_l4_total_len,
			outer_l4_new_csum, outer_l4_new_csum);

	/*==========================================*/
	/* copy from dpu registers to packet (reps) */
	/*==========================================*/
	/* Replace L3 TOS */
	ret |= mod_reg_to_hdr_cpy(&rcp, si_l3_tos, l3_off,
				  offsetof(struct iphdr, tos),
				  sizeof_field(struct iphdr, tos));

	/* Replace L3 TTL */
	ret |= mod_reg_to_hdr_cpy(&rcp, pkt_l3_ttl, l3_off,
				  offsetof(struct iphdr, ttl),
				  sizeof_field(struct iphdr, ttl));

	/* Replace L3 CSUM */
	ret |= mod_reg_to_hdr_cpy(&rcp, pkt_l3_csum, l3_off,
				  offsetof(struct iphdr, check),
				  sizeof_field(struct iphdr, check));

	if (l4_napt) {
		/* Replace L4 PORTS */
		ret |= mod_reg_to_hdr_cpy(&rcp, si_l4_ports,
					  STW_NEXT_LAYER(l3_off),
					  offsetof(struct udphdr, source),
					  sizeof_field(struct udphdr, source) +
					  sizeof_field(struct udphdr, dest));

		/* Replace L4 CSUM */
		ret |= mod_reg_to_hdr_cpy(&rcp, pkt_l4_csum,
					  STW_NEXT_LAYER(l3_off),
					  MOD_SI_PKT_OFF,
					  sizeof_field(struct udphdr, check));
	}

	/*============================================*/
	/* copy from si bce (sram) to packet (insrep) */
	/*============================================*/
	/* replace old L2 with new header */
	ret |= mod_hdr_rplc(&rcp, BCE_OFF_16, l3_off);

	/* replace old L3 IPs with new IPs */
	ret |= mod_bce_ip_nat_cpy(&rcp, BCE_OFF_0, l3_off);

	/*======================================================*/
	/* copy from dpu registers to packet new header (reps1) */
	/*======================================================*/
	if (pppoe_encap)
		ret |= mod_reg_to_new_hdr_cpy(&rcp, pppoe_len,
					      EU_SI_SEL_PPPOE_OFF,
					      offsetof(struct pppoe_hdr,
						       length),
					      sizeof_field(struct pppoe_hdr,
							   length));

	if (nhdr == RCP_HDR_IPV6) {
		/* Replace L3 payload length with inner total length */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, nhdr_l3_tot_len,
					      MOD_PKT_OFF_L3_EXT,
					      offsetof(struct ipv6hdr,
						       payload_len),
					      sizeof_field(struct ipv6hdr,
							   payload_len));
	} else if (nhdr == RCP_HDR_IPV4) {
		/* Update ipv4 id */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, nhdr_id,
					      MOD_PKT_OFF_L3_EXT,
					      offsetof(struct iphdr, id),
					      sizeof_field(struct iphdr, id));

		/* Update ipv4 total length */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, nhdr_l3_tot_len,
					      MOD_PKT_OFF_L3_EXT,
					      offsetof(struct iphdr, tot_len),
					      sizeof_field(struct iphdr,
							   tot_len));

		/* Replace L3 CSUM */
		ret |= mod_reg_to_new_hdr_cpy(&rcp, si_nhdr_csum,
					      MOD_PKT_OFF_L3_EXT,
					      offsetof(struct iphdr, check),
					      sizeof_field(struct iphdr,
							   check));

	}

	/* VXLAN Replace External L4 CSUM */
	if (nhdr == RCP_HDR_IPV6) {
		ret |= mod_reg_to_new_hdr_cpy(&rcp, outer_l4_total_len,
					MOD_PKT_OFF_L3_EXT,
					sizeof(struct ipv6hdr) +
						offsetof(struct udphdr, len),
					sizeof_field(struct udphdr, len));
		ret |= mod_reg_to_new_hdr_cpy(&rcp, outer_l4_new_csum,
					MOD_PKT_OFF_L3_EXT,
					sizeof(struct ipv6hdr) +
						offsetof(struct udphdr, check),
					sizeof_field(struct udphdr, check));
	} else {
		ret |= mod_reg_to_new_hdr_cpy(&rcp, outer_l4_total_len,
					MOD_PKT_OFF_L3_EXT,
					sizeof(struct iphdr) +
						offsetof(struct udphdr, len),
					sizeof_field(struct udphdr, len));
		if (csum_enable)
			ret |= mod_reg_to_new_hdr_cpy(&rcp, outer_l4_new_csum,
					MOD_PKT_OFF_L3_EXT,
					sizeof(struct iphdr) +
						offsetof(struct udphdr, check),
					sizeof_field(struct udphdr, check));
	}

	/*=================================================*/
	/* complete recipe with nop command and 'last' bit */
	/*=================================================*/
	ret |= mod_rcp_complete(&rcp);
	if (unlikely(ret)) {
		pr_err("Failed to create recipe %s(%d)\n",
		       mod_rcp_to_str(rcp_idx), rcp_idx);
	}

	return ret;
}

/**
 * @brief Create a recipe for UDP tunnel encapsulation for both ip and ipv6
 *        inner headers and both ip and ipv6 outer headers
 *        e.g. L2TP, VXLAN.
 * @param rcp_idx recipe index
 * @param l4_napt specify if napt is needed in inner L4
 * @param pkt_ip ingress packet ip type
 * @param nhdr_ip new header ip type
 * @param do_l2tp_len specify if the l2tp length needs to be updated
 * @return s32
 */
static s32 mod_rcp_udp_tunn_set(s32 rcp_idx, enum rcp_hdr_ip_type pkt_ip,
				enum rcp_hdr_ip_type nhdr_ip,
				bool do_l2tp_len)
{
	struct mod_recipe rcp;
	enum mod_eu_stw_mux_sel l3_off = EU_STW_SEL_L3_OFF0;
	s32 ret = 0;

	/* fields fetched from si/stw mux */
	s32 si_rsrv             = DPU_RCP_REG0_B0;
	s32 si_l2tp_diff        = DPU_RCP_REG0_B1;
	s32 si_nhdr_l4_csum     = DPU_RCP_REG0_B2;

	s32 si_nhdr_l3_size     = DPU_RCP_REG1_B0;
	s32 si_l4_csum_zero     = DPU_RCP_REG1_B2;

	s32 si_l4_csum_delta    = DPU_RCP_REG2_B0;
	s32 si_l3_csum_delta    = DPU_RCP_REG2_B2;

	s32 si_l3_tot_len_diff  = DPU_RCP_REG3_B0;
	s32 si_l3_tos           = DPU_RCP_REG3_B1;
	s32 si_nhdr_l3_csum     = DPU_RCP_REG3_B2;

	s32 si_l4_ports         = DPU_RCP_REG4_B0;/* source and dest ports */

	s32 si_l3_ttl_diff      = DPU_RCP_REG5_B0;
	s32 si_pppoe_diff       = DPU_RCP_REG5_B1;
	s32 si_pppoe_len        = DPU_RCP_REG5_B2; /* AKA L2 original value */

	/* fields calculated for the new header */
	s32 nhdr_l3_len         = DPU_RCP_REG6_B0;
	s32 nhdr_l4_len         = DPU_RCP_REG6_B2;
	s32 nhdr_l2tp_len       = DPU_RCP_REG7_B0;
	s32 pkt_l4_csum         = DPU_RCP_REG7_B2;

	/* fields for ipv6 and ipv4, Use R8-R9 based on packet ip type*/
	s32 pkt_l3_csum;
	s32 pkt_l3_ttl = 0;
	s32 pkt_l3_nexthdr;
	s32 pkt_l3_proto;
	s32 pkt_l3_len = 0;

	s32 nhdr_id             = DPU_RCP_REG10_B0;

	/* USE registers R12 - R15 with extra caution as they are
	 * the registers to compare to for the alu conditions
	 * R12 - MUST be zero, used for comparing new header l4 checksum to zero
	 * R13 - not used
	 * R14 - MUST be zero, used for comparing new header l4 checksum to zero
	 * R15 - not used
	 */

	if (unlikely(rcp_idx > MOD_RCP_LAST)) {
		pr_err("error: invalid recipe id, %d\n", rcp_idx);
		return -EINVAL;
	}

	pr_debug("Configuring recipe %s(%u)\n", mod_rcp_to_str(rcp_idx),
		 rcp_idx);
	memset(&rcp, 0, sizeof(rcp));
	rcp.index = rcp_idx;

	/* packets fields - defined them based on the packet ip type to
	 * make fetching from the packet more efficient
	 */
	if (pkt_ip == RCP_HDR_IPV4) {
		pkt_l3_csum  = DPU_RCP_REG8_B0;
		pkt_l3_proto = DPU_RCP_REG8_B2;
		pkt_l3_ttl   = DPU_RCP_REG8_B3;
		pkt_l3_len   = DPU_RCP_REG9_B0;
	} else if (pkt_ip == RCP_HDR_IPV6) {
		pkt_l3_ttl     = DPU_RCP_REG8_B0;
		pkt_l3_nexthdr = DPU_RCP_REG8_B1;
		pkt_l3_len     = DPU_RCP_REG8_B2;
	}


	/*==========================================*/
	/* copy from sce mux to dpu registers (sce) */
	/*==========================================*/

	/* Fetch SI info: L2TP diff and new header L4 checksum fields */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_rsrv, SCE_SI_SEL_RSRV);

	/* Fetch SI info: new header l3 size and l4 checksum zero fields */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_nhdr_l3_size,
					SCE_SI_SEL_L3_HDR_SZ);

	/* Fetch SI info: L3 csum delta & L4 checksum delta */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l4_csum_delta,
				      SCE_SI_SEL_L4_CSUM_DELTA);

	/* Fetch SI info: total length diff, tos, and new header csum */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l3_tot_len_diff,
				      SCE_SI_SEL_TOT_LEN_DIFF);

	/* Fetch SI info: L4 ports and l4 csum zero */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l4_ports,
				      SCE_SI_SEL_NEW_SRC_PORT);

	/* Fetch SI info: ttl, pppoe diff and pppoe length fields */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l3_ttl_diff, SCE_SI_SEL_TTL);

	/*===========================================*/
	/* copy from packet to dpu registers (store) */
	/*===========================================*/
	/* store L3 total_len */

	if (pkt_ip == RCP_HDR_IPV4) {
		/* @TODO: can't store total len and id fields both in a
		 * single command since the next command should store
		 * the ttl field and the 'stall' functionality is not
		 * working as expected
		 */
		ret |= mod_pkt_to_reg_cpy(&rcp, pkt_l3_len, l3_off, 0, 4);

		/* store L3 TTL and L3 checksum */
		ret |= mod_pkt_to_reg_cpy(
			&rcp, pkt_l3_csum, l3_off, offsetof(struct iphdr, ttl),
			sizeof_field(struct iphdr, ttl) +
				sizeof_field(struct iphdr, protocol) +
				sizeof_field(struct iphdr, check));
	} else {
		ret |= mod_pkt_to_reg_cpy(
			&rcp, pkt_l3_ttl, l3_off,
			offsetof(struct ipv6hdr, payload_len),
			sizeof_field(struct ipv6hdr, payload_len) +
				sizeof_field(struct ipv6hdr, nexthdr) +
				sizeof_field(struct ipv6hdr, hop_limit));
	}

	/* store L4 checksum */
	ret |= mod_pkt_to_reg_cpy(&rcp, pkt_l4_csum, STW_NEXT_LAYER(l3_off),
				  MOD_SI_PKT_OFF,
				  sizeof_field(struct udphdr, check));

	/*=========================*/
	/*      ALU operations     */
	/*=========================*/

	/* =================================================================== |
	 *    ALU operations - Inner V4 NHDR V4 + NAPT + PPPoE ENCAP           |
	 * =================================================================== |
	 * |---------------------------------|---------------------------------|
	 * |      ALU0                       |      ALU1                       |
	 * |---------------------------------|---------------------------------|
	 * | sync to l4 csum                 |                                 |
	 * | Inner ipv4 csum + si delta      | sync to l4 csum                 |
	 * | Inner ipv4 ttl + si diff        | inner l4 csum + si delta        |
	 * | nhdr l3 length calc             | inner l4 csum cmp si csum zero  |
	 * | nhdr l4 length calc             | nhdr pppoe length calc          |
	 * | nhdr id increment               | nhdr udp csum + length          |
	 * | nhdr l3 csum + length           | nhdr udp csum + length          |
	 * | nhdr l3 csum + id               | nhdr l2tp length calc           |
	 * |                                 | nhdr udp csum + l2tp length     |
	 * |---------------------------------|---------------------------------|
	 * =================================================================== |
	 *    ALU operations - Inner V4 NHDR V6 + NAPT + PPPoE ENCAP           |
	 * =================================================================== |
	 * |---------------------------------|---------------------------------|
	 * |      ALU0                       |      ALU1                       |
	 * |---------------------------------|---------------------------------|
	 * | sync to l4 csum                 |                                 |
	 * | Inner ipv4 csum + si delta      | sync to l4 csum                 |
	 * | Inner ipv4 ttl + si diff        | inner l4 csum + si delta        |
	 * | nhdr l3 length calc             | inner l4 csum cmp si csum zero  |
	 * | nhdr l4 length calc             | nhdr pppoe length calc          |
	 * |                                 | nhdr udp csum + length          |
	 * |                                 | nhdr udp csum + length          |
	 * |                                 | nhdr l2tp length calc           |
	 * |                                 | nhdr udp csum + l2tp length     |
	 * |---------------------------------|---------------------------------|
	 * =================================================================== |
	 *    ALU operations - Inner V6 NHDR V4 + NAPT + PPPoE ENCAP           |
	 * =================================================================== |
	 * |---------------------------------|---------------------------------|
	 * |      ALU0                       |      ALU1                       |
	 * |---------------------------------|---------------------------------|
	 * | sync to l4 csum                 |                                 |
	 * | nhdr udp csum + inner l3 length | sync to l4 csum                 |
	 * | Inner ipv6 hop limit + si diff  | inner l4 csum + si delta        |
	 * | nhdr l3 length calc             | inner l4 csum cmp si csum zero  |
	 * | nhdr l4 length calc             | nhdr pppoe length calc          |
	 * | nhdr id increment               | nhdr udp csum + length          |
	 * | nhdr l3 csum + length           | nhdr udp csum + length          |
	 * | nhdr l3 csum + id               | nhdr l2tp length calc           |
	 * |                                 | nhdr udp csum + l2tp length     |
	 * |---------------------------------|---------------------------------|
	 * =================================================================== |
	 *    ALU operations - Inner V6 NHDR V6 + NAPT + PPPoE ENCAP           |
	 * =================================================================== |
	 * |---------------------------------|---------------------------------|
	 * |      ALU0                       |      ALU1                       |
	 * |---------------------------------|---------------------------------|
	 * | sync to l4 csum                 |                                 |
	 * | nhdr udp csum + inner l3 length | sync to l4 csum                 |
	 * | Inner ipv4 ttl + si diff        | inner l4 csum + si delta        |
	 * | nhdr l3 length calc             | inner l4 csum cmp si csum zero  |
	 * | nhdr l4 length calc             | nhdr pppoe length calc          |
	 * |                                 | nhdr udp csum + length          |
	 * |                                 | nhdr udp csum + length          |
	 * |                                 | nhdr l2tp length calc           |
	 * |                                 | nhdr udp csum + l2tp length     |
	 * |---------------------------------|---------------------------------|
	 */

	/* sync both ALUs to L4 checksum */
	ret |= mod_alu_synch(EU_ALU0, &rcp, STW_NEXT_LAYER(l3_off),
			     MOD_SI_PKT_OFF,
			     sizeof_field(struct udphdr, check));

	if (pkt_ip == RCP_HDR_IPV4) {
		/* Calculate inner ip l3 checksum */
		ret |= mod_alu_csum16_add(EU_ALU0, &rcp, si_l3_csum_delta,
					  pkt_l3_csum, pkt_l3_csum);
	} else if (pkt_ip == RCP_HDR_IPV6) {
		ret |= mod_alu_csum16_sub_cond(EU_ALU0, &rcp, pkt_l3_len,
					  si_nhdr_l4_csum, si_nhdr_l4_csum,
					  EU_ALU_L0_BIG_R2, EU_ALU_COND_MET);
	}

	/* decrement inner ip ttl */
	ret |= mod_alu_sub(EU_ALU0, &rcp, pkt_l3_ttl,
			   sizeof_field(struct iphdr, ttl), si_l3_ttl_diff,
			   sizeof_field(struct iphdr, ttl), pkt_l3_ttl);

	/* Calculate new header ip total length */
	ret |= mod_alu_add(EU_ALU0, &rcp, si_l3_tot_len_diff, 1, pkt_l3_len,
			   sizeof_field(struct iphdr, tot_len), nhdr_l3_len);

	/* Calculate new header udp length */
	ret |= mod_alu_sub(EU_ALU0, &rcp, nhdr_l3_len,
			   sizeof_field(struct iphdr, tot_len), si_nhdr_l3_size,
			   sizeof_field(struct iphdr, tot_len), nhdr_l4_len);

	if (nhdr_ip == RCP_HDR_IPV4) {
		ret |= mod_alu_rand(EU_ALU0, &rcp, nhdr_id,
				    sizeof_field(struct iphdr, id));

		/* Update the new header ip csum with total len */
		ret |= mod_alu_csum16_sub(EU_ALU0, &rcp, nhdr_l3_len,
					  si_nhdr_l3_csum, si_nhdr_l3_csum);

		/* Update the new header ip csum with id */
		ret |= mod_alu_csum16_sub(EU_ALU0, &rcp, nhdr_id,
					  si_nhdr_l3_csum, si_nhdr_l3_csum);
	}

	ret |= mod_alu_synch(EU_ALU1, &rcp, STW_NEXT_LAYER(l3_off),
			     MOD_SI_PKT_OFF,
			     sizeof_field(struct udphdr, check));

	/* Recalculate L4 checksum */
	/* in general, the checksum calculation should be as follow:
	 *   udp: csum = csum ? csum + csum_delta : csum
	 *   tcp: csum = csum + csum_delta
	 * instead, we have csum_zero value which is set as follow
	 *   udp: csum_zero = csum_delta
	 *   tcp: csum_zero = 0
	 * and the calculation is as follow for both:
	 *   csum = csum + csum_delta
	 *   csum = (csum - csum_zero) ? csum : csum - csum_zero;
	 *
	 * all of this to support zero udp csum
	 */
	ret |= mod_alu_csum16_add(EU_ALU1, &rcp, si_l4_csum_delta, pkt_l4_csum,
				  pkt_l4_csum);
	ret |= mod_alu_csum16_sub_cond(EU_ALU1, &rcp, si_l4_csum_zero,
				       pkt_l4_csum, pkt_l4_csum,
				       EU_ALU_RES_EQ_R2, EU_ALU_COND_MET);



	/* Update L4 CSUM with L4 length*/
	ret |= mod_alu_csum16_sub_cond(EU_ALU1, &rcp, nhdr_l4_len,
				       si_nhdr_l4_csum, si_nhdr_l4_csum,
				       EU_ALU_L0_BIG_R2, EU_ALU_COND_MET);
	ret |= mod_alu_csum16_sub_cond(EU_ALU1, &rcp, nhdr_l4_len,
				       si_nhdr_l4_csum, si_nhdr_l4_csum,
				       EU_ALU_L0_BIG_R2, EU_ALU_COND_MET);

	/* Calculate new header pppoe length -->
	 * pppoe_len = pppoe_diff ? pppoe_diff + l3 len : l2_org_val
	 */
	ret |= mod_alu_add_cond(EU_ALU1, &rcp, si_pppoe_diff, 1, nhdr_l3_len, 2,
				si_pppoe_len, EU_ALU_L0_BIG_R2,
				EU_ALU_COND_MET);

	if (do_l2tp_len) {
		/* Calculate L2TP total length */
		ret |= mod_alu_add(EU_ALU1, &rcp, si_l2tp_diff, 1, pkt_l3_len,
				   sizeof_field(struct iphdr, tot_len),
				   nhdr_l2tp_len);
		/* Add l2tp length to the outer udp checksum */
		ret |= mod_alu_csum16_sub_cond(EU_ALU1, &rcp, nhdr_l2tp_len,
					       si_nhdr_l4_csum, si_nhdr_l4_csum,
					       EU_ALU_L0_BIG_R2,
					       EU_ALU_COND_MET);
	}

	/*==========================================*/
	/* copy from dpu registers to packet (reps) */
	/*==========================================*/
	if (pkt_ip == RCP_HDR_IPV4) {
		/* Replace L3 TOS */
		ret |= mod_reg_to_hdr_cpy(&rcp, si_l3_tos, l3_off,
					  offsetof(struct iphdr, tos),
					  sizeof_field(struct iphdr, tos));

		/* Replace L3 TTL */
		ret |= mod_reg_to_hdr_cpy(&rcp, pkt_l3_ttl, l3_off,
					  offsetof(struct iphdr, ttl),
					  sizeof_field(struct iphdr, ttl));

		/* Replace L3 CSUM */
		ret |= mod_reg_to_hdr_cpy(&rcp, pkt_l3_csum, l3_off,
					  offsetof(struct iphdr, check),
					  sizeof_field(struct iphdr, check));
	} else {
		/* Replace L3 hop limit */
		ret |= mod_reg_to_hdr_cpy(&rcp, pkt_l3_ttl, l3_off,
				  offsetof(struct ipv6hdr, hop_limit),
				  sizeof_field(struct ipv6hdr, hop_limit));
		/* Replace L3 hop limit */
		ret |= mod_reg_to_hdr_cpy(&rcp, pkt_l3_len, l3_off,
				  offsetof(struct ipv6hdr, payload_len),
				  sizeof_field(struct ipv6hdr, payload_len));
	}

	/* Replace L4 PORTS */
	ret |= mod_reg_to_hdr_cpy(&rcp, si_l4_ports, STW_NEXT_LAYER(l3_off),
				  offsetof(struct udphdr, source),
				  sizeof_field(struct udphdr, source) +
					  sizeof_field(struct udphdr, dest));

	/* Replace L4 CSUM */
	ret |= mod_reg_to_hdr_cpy(&rcp, pkt_l4_csum, STW_NEXT_LAYER(l3_off),
				  MOD_SI_PKT_OFF,
				  sizeof_field(struct udphdr, check));

	/*============================================*/
	/* copy from si bce (sram) to packet (insrep) */
	/*============================================*/
	/* replace old L2 with new header */
	ret |= mod_hdr_rplc(&rcp, BCE_OFF_16, l3_off);

	/* replace old L3 IPs with new IPs */
	if (pkt_ip == RCP_HDR_IPV4)
		ret |= mod_bce_ip_nat_cpy(&rcp, BCE_OFF_0, l3_off);

	/*======================================================*/
	/* copy from dpu registers to packet new header (reps1) */
	/*======================================================*/
	ret |= pppoe_fld_to_nhdr(&rcp, si_pppoe_len, length);

	if (nhdr_ip == RCP_HDR_IPV6) {
		/* Replace L3 payload length with inner total length */
		ret |= ipv6_fld_to_nhdr(&rcp, nhdr_l3_len, payload_len);
	} else if (nhdr_ip == RCP_HDR_IPV4) {
		/* Update id */
		ret |= ipv4_fld_to_nhdr(&rcp, nhdr_id, id);
		/* Update total length */
		ret |= ipv4_fld_to_nhdr(&rcp, nhdr_l3_len, tot_len);
		/* Update CSUM */
		ret |= ipv4_fld_to_nhdr(&rcp, si_nhdr_l3_csum, check);
	}

	/* Update the outer UDP length and checksum */
	ret |= udp_fld_to_nhdr(&rcp, nhdr_l4_len, len);
	ret |= udp_fld_to_nhdr(&rcp, si_nhdr_l4_csum, check);

	/* Update the l2tp length */
	if (do_l2tp_len)
		ret |= l2tpudp_fld_to_nhdr(&rcp, nhdr_l2tp_len,
					   L2TPV2_OPT_LEN_OFF,
					   L2TPV2_OPT_LEN_LEN);

	/*=================================================*/
	/* complete recipe with nop command and 'last' bit */
	/*=================================================*/
	ret |= mod_rcp_complete(&rcp);
	if (unlikely(ret)) {
		pr_err("Failed to create recipe %s(%d)\n",
		       mod_rcp_to_str(rcp_idx), rcp_idx);
	}

	return ret;
}

/**
 * @brief Create recipe for IP translation.
 *        IPv4 to IPv6 and vice versa
 * @param rcp_idx recipe index to create
 * @param pkt_ip packet ip type
 * @return s32 0 on success, error code otherwise
 */
s32 mod_rcp_ip_translation_set(s32 rcp_idx, enum rcp_hdr_ip_type pkt_ip,
			       bool pppoe_encap)
{
	enum mod_eu_stw_mux_sel l3_off, l4_off, strip_off;
	enum rcp_hdr_ip_type nhdr_ip;
	struct mod_recipe rcp = { .index = rcp_idx };
	s32 ret = 0;
	bool l4_napt = DO_NAPT; /* always do napt */
	u8 alu0_sync_off, alu0_sync_sz;
	u8 alu1_sync_off, alu1_sync_sz, alu_sync_lyr;

	/* ================= */
	/* Map dpu registers */
	/* ================= */
	s32 si_l4_csum_delta   = DPU_RCP_REG0_B0;
	s32 si_l4_csum_zero    = DPU_RCP_REG1_B0;
	s32 si_l3_tot_len_diff = DPU_RCP_REG2_B0;
	// s32 si_l3_tos          = DPU_RCP_REG2_B1; // not used
	s32 si_nhdr_csum       = DPU_RCP_REG2_B2;
	s32 si_l3_ttl_diff     = DPU_RCP_REG3_B0;
	s32 si_pppoe_diff      = DPU_RCP_REG3_B1;
	s32 si_pppoe_len       = DPU_RCP_REG3_B2;
	s32 si_l4_ports        = DPU_RCP_REG4_B0;
	s32 pkt_l3_len         = DPU_RCP_REG6_B0;

	s32 pkt_l4_csum        = DPU_RCP_REG7_B0;
	s32 nhdr_l3_len        = DPU_RCP_REG7_B2;
	s32 stw_pkt_len        = DPU_RCP_REG8_B0;
	s32 stw_off_l3         = DPU_RCP_REG9_B0;
	s32 nhdr_id            = DPU_RCP_REG10_B0;
	/* USE registers R12 - R15 with extra caution as they are
	 * the registers to compare to for the alu conditions
	 * R12 - in used by ALU0 for pppoe and l4 csum calculation and MUST
	 *       be zero in the time of the calculation
	 * R13 - not used
	 * R14 - not used
	 * R15 - not used
	 */

	if (unlikely(rcp_idx > MOD_RCP_LAST)) {
		pr_err("error: invalid recipe id, %d\n", rcp_idx);
		return -EINVAL;
	}

	/* set the currect new header */
	switch (pkt_ip) {
	case RCP_HDR_IPV4:
		nhdr_ip = RCP_HDR_IPV6;
		break;
	case RCP_HDR_IPV6:
		nhdr_ip = RCP_HDR_IPV4;
		break;
	default:
		pr_err("invalid pkt_ip type %u\n", pkt_ip);
		return -EINVAL;
	}

	pr_debug("Configuring recipe %s(%u)\n", mod_rcp_to_str(rcp_idx),
		 rcp_idx);

	/** we assume that we always do the translation on the outer IP
	 *  so we read all necessary fields from outer L3 and strip
	 *  all layers including outer L3
	 */
	l3_off = EU_STW_SEL_L3_OFF0; /* outer L3 offset */
	l4_off = EU_STW_SEL_L3_OFF1; /* outer L4 offset, UDP/TCP/ICMP */
	strip_off = EU_STW_SEL_L3_OFF1; /* outer L4 offset, UDP/TCP/ICMP */

	/*==========================================*/
	/* copy from sce mux to dpu registers (sce) */
	/*==========================================*/
	/* Fetch SI info: L3 csum delta & L4 checksum delta */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l4_csum_delta,
				      SCE_SI_SEL_L4_CSUM_DELTA);

	/* Fetch SI info: total length diff, tos, and new header csum */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l3_tot_len_diff,
				      SCE_SI_SEL_TOT_LEN_DIFF);

	/* Fetch SI info: ttl, pppoe diff and pppoe length fields */
	ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l3_ttl_diff, SCE_SI_SEL_TTL);

	/* Fetch SI info: L4 ports and l4 csum zero */
	if (l4_napt) {
		ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l4_ports,
					      SCE_SI_SEL_NEW_SRC_PORT);
		ret |= mod_sce_mux_to_reg_cpy(&rcp, si_l4_csum_zero,
					      SCE_SI_SEL_L4_CSUM_ZERO);
	}

	/*======================*/
	/* STW info: packet len */
	/*======================*/
	ret |= mod_sce_mux_to_reg_cpy(&rcp, stw_pkt_len, SCE_STW_SEL_PKT_LEN);
	/*======================*/
	/* STW info: offset L3  */
	/*======================*/
	ret |= mod_sce_mux_to_reg_cpy(&rcp, stw_off_l3, SCE_STW_SEL_L3_OFF0);

	/*===========================================*/
	/* copy from packet to dpu registers (store) */
	/*===========================================*/
	/* store L3 total_len/payload length */
	if (pkt_ip == RCP_HDR_IPV4) {
		/* store total length */
		ret |= mod_pkt_to_reg_cpy(&rcp, pkt_l3_len, l3_off,
					  offsetof(struct iphdr, tot_len),
					  sizeof_field(struct iphdr, tot_len));

		alu0_sync_off = offsetof(struct iphdr, tot_len);
		alu0_sync_sz = sizeof_field(struct iphdr, tot_len);
	} else if (pkt_ip == RCP_HDR_IPV6) {
		/* store total length */
		ret |= mod_pkt_to_reg_cpy(&rcp, pkt_l3_len, l3_off,
					  offsetof(struct ipv6hdr, payload_len),
					  sizeof_field(struct ipv6hdr,
						       payload_len));

		alu0_sync_off = offsetof(struct ipv6hdr, payload_len);
		alu0_sync_sz = sizeof_field(struct ipv6hdr, payload_len);
	}

	/* store L4 checksum */
	if (l4_napt) {
		ret |= mod_pkt_to_reg_cpy(&rcp, pkt_l4_csum, l4_off,
					  MOD_SI_PKT_OFF,
					  sizeof_field(struct udphdr, check));
		/* Synchronize ALU1 to L4 checksum field,
		 * The offset value is taken from the SI and it will be
		 * different value for UDP and TCP
		 */
		alu1_sync_off = MOD_SI_PKT_OFF;
		alu1_sync_sz  = sizeof_field(struct udphdr, check);
		alu_sync_lyr  = l4_off;
	} else {
		alu1_sync_off = alu0_sync_off;
		alu1_sync_sz  = alu0_sync_sz;
		alu_sync_lyr  = l3_off;
	}

	/* =================================================================== |
	 *          ALU operations - NHDR V4 + NAPT + PPPoE ENCAP              |
	 * =================================================================== |
	 * |---------------------------------|---------------------------------|
	 * |      ALU0                       |      ALU1                       |
	 * |---------------------------------|---------------------------------|
	 * | sync to ip length               |                                 |
	 * | nhdr l3 length calc             | sync to ip length/L4 csum       |
	 * | ID increment                    | packet L4 CSUM update           |
	 * | nhdr L3 CSUM length update      | packet L4 CSUM update           |
	 * | nhdr L3 CSUM ID update          |                                 |
	 * | nhdr pppoe length calc          |                                 |
	 * |---------------------------------|---------------------------------|
	 * =================================================================== |
	 *          ALU operations - NHDR V6 + NAPT + PPPoE ENCAP              |
	 * =================================================================== |
	 * |---------------------------------|---------------------------------|
	 * |      ALU0                       |      ALU1                       |
	 * |---------------------------------|---------------------------------|
	 * | sync to ip length               |                                 |
	 * | nhdr l3 length calc             | sync to ip length/L4 dport      |
	 * | nhdr pppoe length calc          | packet L4 CSUM update           |
	 * |                                 | packet L4 CSUM update           |
	 * |---------------------------------|---------------------------------|
	 */

	/* Synchronize ALU0 to latest fetched information of STORE command */
	ret |= mod_alu_synch(EU_ALU0, &rcp, l3_off,
			     alu0_sync_off, alu0_sync_sz);
	ret |= mod_alu_synch(EU_ALU1, &rcp, alu_sync_lyr,
			     alu1_sync_off, alu1_sync_sz);

	if (nhdr_ip == RCP_HDR_IPV4)
		/* Calculate new header total length */
		ret |= mod_alu_add(EU_ALU0, &rcp, pkt_l3_len, 2,
				   si_l3_tot_len_diff, 1, nhdr_l3_len);
	else if (nhdr_ip == RCP_HDR_IPV6)
		ret |= mod_alu_sub(EU_ALU0, &rcp, pkt_l3_len, 2,
				   si_l3_tot_len_diff, 1, nhdr_l3_len);

	if (nhdr_ip == RCP_HDR_IPV4) {
		ret |= mod_alu_rand(EU_ALU0, &rcp, nhdr_id,
				    sizeof_field(struct iphdr, id));

		/* Update the external L3 csum due to total len */
		ret |= mod_alu_csum16_sub(EU_ALU0, &rcp, nhdr_l3_len,
					  si_nhdr_csum, si_nhdr_csum);

		/* Update the external L3 csum due to global id */
		ret |= mod_alu_csum16_sub(EU_ALU0, &rcp, nhdr_id,
					  si_nhdr_csum, si_nhdr_csum);
	}

	/* pppoe_len = pppoe_diff ? pppoe_diff + l3 len : l2_org_val */
	if (pppoe_encap) {
		ret |= mod_alu_add_cond(EU_ALU0, &rcp, si_pppoe_diff, 1,
					nhdr_l3_len, 2, si_pppoe_len,
					EU_ALU_L0_BIG_R2,
					EU_ALU_COND_MET);
	}

	/* Recalculate L4 checksum */
	if (l4_napt) {
		/* in general, the checksum calculation should be as follow:
		 *   udp: csum = csum ? csum + csum_delta : csum
		 *   tcp: csum = csum + csum_delta
		 * instead, we have csum_zero value which is set as follow
		 *   udp: csum_zero = csum_delta
		 *   tcp: csum_zero = 0
		 * and the calculation is as follow for both:
		 *   csum = csum + csum_delta
		 *   csum = (csum - csum_zero) ? csum : csum - csum_zero;
		 */
		ret |= mod_alu_csum16_add(EU_ALU1, &rcp, si_l4_csum_delta,
					  pkt_l4_csum, pkt_l4_csum);
		ret |= mod_alu_csum16_sub_cond(EU_ALU1, &rcp, si_l4_csum_zero,
					       pkt_l4_csum, pkt_l4_csum,
					       EU_ALU_RES_EQ_R2,
					       EU_ALU_COND_MET);
	}

	/*==========================================*/
	/* copy from dpu registers to packet (reps) */
	/*==========================================*/
	if (l4_napt) {
		/* Replace L4 PORTS */
		ret |= mod_reg_to_hdr_cpy(&rcp, si_l4_ports, l4_off,
					  offsetof(struct udphdr, source),
					  sizeof_field(struct udphdr, source) +
						  sizeof_field(struct udphdr,
							       dest));

		/* Replace L4 CSUM */
		ret |= mod_reg_to_hdr_cpy(&rcp, pkt_l4_csum, l4_off,
					  MOD_SI_PKT_OFF,
					  sizeof_field(struct udphdr, check));
	}

	/*============================================*/
	/* copy from si bce (sram) to packet (insrep) */
	/*============================================*/
	/* replace everything upto L3 included with the new header */
	ret |= mod_hdr_rplc(&rcp, BCE_OFF_0, strip_off);

	/*======================================================*/
	/* copy from dpu registers to packet new header (reps1) */
	/*======================================================*/
	if (pppoe_encap)
		ret |= pppoe_fld_to_nhdr(&rcp, si_pppoe_len, length);

	if (nhdr_ip == RCP_HDR_IPV6) {
		/* Update payload length */
		ret |= ipv6_fld_to_nhdr(&rcp, nhdr_l3_len, payload_len);
	} else if (nhdr_ip == RCP_HDR_IPV4) {
		/* Update id */
		ret |= ipv4_fld_to_nhdr(&rcp, nhdr_id, id);
		/* Update total length */
		ret |= ipv4_fld_to_nhdr(&rcp, nhdr_l3_len, tot_len);
		/* Update CSUM */
		ret |= ipv4_fld_to_nhdr(&rcp, si_nhdr_csum, check);
	}

	/*=================================================*/
	/* complete recipe with nop command and 'last' bit */
	/*=================================================*/
	ret |= mod_rcp_complete(&rcp);
	if (unlikely(ret)) {
		pr_err("Failed to create recipe %s(%d)\n",
		       mod_rcp_to_str(rcp_idx), rcp_idx);
	}

	return ret;
}

s32 mod_rcp_init(void)
{
	s32 ret;

	BUILD_BUG_ON_MSG(MOD_FLAG_NUM >= BITS_PER_LONG,
			 "TOO MUCH modifier bits");

	/* config no-modification recipe */
	ret = mod_rcp_no_mod_set(MOD_RCP_NO_MOD);
	if (unlikely(ret))
		goto err;

	/* config recipe L2 replacing only */
	ret = mod_rcp_hdr_rplc_set(MOD_RCP_HDR_RPLC, true, EU_STW_SEL_L3_OFF0);
	if (unlikely(ret))
		goto err;

	/* Recipe for replacing all layers up to L3 (included) */
	ret = mod_rcp_hdr_rplc_set(MOD_RCP_HDR_L3_RPLC, false,
				   EU_STW_SEL_L3_OFF1);
	if (unlikely(ret))
		goto err;

	/* Recipe for replacing all layers up to L4 (included) */
	ret = mod_rcp_hdr_rplc_set(MOD_RCP_HDR_L4_RPLC, false,
				   EU_STW_SEL_L3_OFF2);
	if (unlikely(ret))
		goto err;

	/* config recipe IPV4 NAT without L4 modification
	 * [l2][ipv4][l4] => ipv4 layer offset is first
	 */
	ret = mod_rcp_ipv4_routed_set(MOD_RCP_IPV4_NAT, EU_STW_SEL_L3_OFF0,
				      false, RCP_HDR_NO_IP, true);
	if (unlikely(ret))
		goto err;

	/* config recipe IPV4 NAT with UDP modification
	 * [l2][ipv4][tcp/udp] => ipv4 layer offset is first
	 * for tcp/udp we also specify we need l4 nat
	 */
	ret = mod_rcp_ipv4_routed_set(MOD_RCP_IPV4_NAPT, EU_STW_SEL_L3_OFF0,
				      true, RCP_HDR_NO_IP, true);
	if (unlikely(ret))
		goto err;

	/* config recipe IPV6 decrement hop limit modification */
	ret = mod_rcp_ipv6_routed_set(MOD_RCP_IPV6_ROUTED, EU_STW_SEL_L3_OFF0,
				      RCP_HDR_NO_IP, true);
	if (unlikely(ret))
		goto err;

	/* config recipe tunnel v4 in v4 encapsulation */
	ret = mod_rcp_ipv4_routed_set(MOD_RCP_IPV4_IPV4_ENCAP,
				      EU_STW_SEL_L3_OFF0, true, RCP_HDR_IPV4,
				      true);
	if (unlikely(ret))
		goto err;

	/* config recipe tunnel v4 in v4 encapsulation */
	ret = mod_rcp_ipv4_routed_set(MOD_RCP_ESP_IPV4_IPV4_ENCAP,
				      EU_STW_SEL_L3_OFF0, false, RCP_HDR_IPV4,
				      true);
	if (unlikely(ret))
		goto err;

	/* config recipe tunnel v4 in v6 encapsulation with nat
	 * changes in the inner v4 and udp/tcp
	 */
	ret = mod_rcp_ipv4_routed_set(MOD_RCP_IPV6_IPV4_ENCAP,
				      EU_STW_SEL_L3_OFF0, true, RCP_HDR_IPV6,
				      true);
	if (unlikely(ret))
		goto err;

	/* config recipe tunnel esp bypass v4 in v6 encapsulation
	 * with nat changes in the inner v4 
	 */
	ret = mod_rcp_ipv4_routed_set(MOD_RCP_ESP_IPV6_IPV4_ENCAP,
				      EU_STW_SEL_L3_OFF0, false, RCP_HDR_IPV6,
				      true);
	if (unlikely(ret))
		goto err;

	/* config recipe tunnel v6 in v6 encapsulation with inner
	 * routed ipv6 and udp/tcp
	 */
	ret = mod_rcp_ipv6_routed_set(MOD_RCP_IPV6_IPV6_ENCAP,
				      EU_STW_SEL_L3_OFF0, RCP_HDR_IPV6, true);
	if (unlikely(ret))
		goto err;

	/* config recipe tunnel v6 in v4 encapsulation */
	ret = mod_rcp_ipv6_routed_set(MOD_RCP_IPV4_IPV6_ENCAP,
				      EU_STW_SEL_L3_OFF0, RCP_HDR_IPV4, true);
	if (unlikely(ret))
		goto err;

	/* config recipe eogre v4 in v6/v4 decapsulation with nat
	 * changes in the inner v4 and udp/tcp
	 * [l2][ipv4/6][gre][l2][ipv4][tcp/udp] => inner ipv4 offset is fourth
	 * for tcp/udp we also specify we need l4 nat
	 */
	ret = mod_rcp_ipv4_routed_set(MOD_RCP_IPV4_OFF3_DECP,
				      EU_STW_SEL_L3_OFF3, true,
				      RCP_HDR_NO_IP, false);
	if (unlikely(ret))
		goto err;

	/* config recipe eogre v4 in v6/v4 decapsulation with nat
	 * changes in the inner v4 and udp/tcp
	 * [l2][ipv4/6][gre][l2][ipv4]esp] => inner ipv4 offset is fourth
	 * for tcp/udp we also specify we need l4 nat
	 */
	ret = mod_rcp_ipv4_routed_set(MOD_RCP_IPV4_OFF3_DECP_NAT,
				      EU_STW_SEL_L3_OFF3, false,
				      RCP_HDR_NO_IP, false);
	if (unlikely(ret))
		goto err;

	/* config recipe ipogre v4 in v6/v4 decapsulation with nat
	 * changes in the inner v4 and udp/tcp
	 * [l2][ipv4/6][gre][ipv4][tcp/udp] => inner ipv4 offset is third
	 * for tcp/udp we also specify we need l4 nat
	 */
	ret = mod_rcp_ipv4_routed_set(MOD_RCP_IPOGRE_DECP_IPV4_NAPT,
				      EU_STW_SEL_L3_OFF2, true,
				      RCP_HDR_NO_IP, false);
	if (unlikely(ret))
		goto err;

	/* config recipe ipogre v4 in v6/v4 decapsulation with nat
	 * changes in the inner v4
	 * [l2][ipv4/6][gre][ipv4][esp] => inner ipv4 offset is third
	 * for tcp/udp we also specify we need l4 nat
	 */
	ret = mod_rcp_ipv4_routed_set(MOD_RCP_IPOGRE_DECP_IPV4,
				      EU_STW_SEL_L3_OFF2, false,
				      RCP_HDR_NO_IP, false);
	if (unlikely(ret))
		goto err;

	/* config recipe eogre v6 in v6/v4 decapsulation with hop limit
	 * decrement in the inner v6
	 * [l2][ipv4/6][gre][l2][ipv6][tcp/udp] => inner ipv6 offset is fourth
	 */
	ret = mod_rcp_ipv6_routed_set(MOD_RCP_IPV6_OFF3_DECP,
				      EU_STW_SEL_L3_OFF3, RCP_HDR_NO_IP,
				      false);
	if (unlikely(ret))
		goto err;

	/* config recipe ipogre v6 in v6/v4 decapsulation with hop limit
	 * decrement in the inner v6
	 * [l2][ipv4/6][gre][ipv6][tcp/udp] => inner ipv6 offset is third
	 */
	ret = mod_rcp_ipv6_routed_set(MOD_RCP_IPOGRE_DECP_IPV6_ROUTED,
				      EU_STW_SEL_L3_OFF2, RCP_HDR_NO_IP,
				      false);
	if (unlikely(ret))
		goto err;

	/* config recipe dslite decapsulation with nat
	 * changes in the inner L3 and udp/tcp
	 * [l2][ipv6][ipv4][tcp/udp] => inner L3 offset is second
	 * for tcp/udp we also specify we need l4 nat
	 */
	ret = mod_rcp_ipv4_routed_set(MOD_RCP_DSLITE_DECP_NAPT,
				      EU_STW_SEL_L3_OFF1, true,
				      RCP_HDR_NO_IP, false);
	if (unlikely(ret))
		goto err;

	/* config recipe 6RD decapsulation
	 * [l2][ipv4][ipv6][tcp/udp] => inner L3 offset is second
	 * for tcp/udp we also specify we need l4 nat
	 */
	ret = mod_rcp_ipv6_routed_set(MOD_RCP_6RD_DECP_ROUTED,
				      EU_STW_SEL_L3_OFF1, RCP_HDR_NO_IP,
				      false);
	if (unlikely(ret))
		goto err;

	/* config recipe ESP over ipv4 encapsulation */
	ret = mod_rcp_esp_set(MOD_RCP_ESP_V4_ENCAP, RCP_HDR_IPV4,
			      EU_STW_SEL_L3_OFF0, false, false);
	if (unlikely(ret))
		goto err;

	/* config recipe ESP over ipv4 encapsulation with L3 replacement */
	ret = mod_rcp_esp_set(MOD_RCP_ESP_V4_ENCAP_L3_RPLC, RCP_HDR_IPV4,
			      EU_STW_SEL_L3_OFF1, false, true);
	if (unlikely(ret))
		goto err;

	/* config recipe ESP over ipv4 bypass */
	ret = mod_rcp_esp_set(MOD_RCP_ESP_V4_BPAS, RCP_HDR_NO_IP,
			      EU_STW_SEL_L3_OFF0, true, false);
	if (unlikely(ret))
		goto err;

	/* config recipe ESP over ipv6 encapsulation */
	ret = mod_rcp_esp_ipv6_set(MOD_RCP_ESP_V6_ENCAP, RCP_HDR_IPV6,
				   EU_STW_SEL_L3_OFF0, false);
	if (unlikely(ret))
		goto err;

	/* config recipe ESP over ipv6 encapsulation with L3 replacement */
	ret = mod_rcp_esp_ipv6_set(MOD_RCP_ESP_V6_ENCAP_L3_RPLC, RCP_HDR_IPV6,
				   EU_STW_SEL_L3_OFF1, false);
	if (unlikely(ret))
		goto err;

	/* config recipe ESP over ipv6 bypass */
	ret = mod_rcp_esp_ipv6_set(MOD_RCP_ESP_V6_BPAS, RCP_HDR_NO_IP,
				   EU_STW_SEL_L3_OFF0, true);
	if (unlikely(ret))
		goto err;

	/* config recipe L2TP IP in v6/v4 decapsulation with nat
	 * changes in the inner v4 and udp/tcp
	 * [l2][ipv4/6][l2tp][mac][ipv4][tcp/udp] => inner ipv4
	 * offset is third. For tcp/udp we also specify we need l4 nat
	 */
	ret = mod_rcp_ipv4_routed_set(MOD_RCP_L2TPIP_DECP_IPV4_NAPT,
				      EU_STW_SEL_L3_OFF3, true,
				      RCP_HDR_NO_IP, false);
	if (unlikely(ret))
		goto err;

	/* config recipe L2TP IP in v6/v4 decapsulation with hop limit
	 * decrement in the inner v6
	 * [l2][ipv4/6][l2tp][mac][ipv6][tcp/udp] => inner ipv6
	 * offset is third
	 */
	ret = mod_rcp_ipv6_routed_set(MOD_RCP_L2TPIP_DECP_IPV6_ROUTED,
				      EU_STW_SEL_L3_OFF3, RCP_HDR_NO_IP,
				      false);
	if (unlikely(ret))
		goto err;

	/*
		recipe VXLAN tunnel encapsulation IPV4 Over IPV6
		[L2][IPV4] --> [L2][IPV6][UDP][VXLAN][L2][IPV4]
	*/
	ret = mod_rcp_vxlan_routed_set(MOD_RCP_VXLANV6_V4_ENCAP,
				      EU_STW_SEL_L3_OFF0, true, RCP_HDR_IPV6,
				      true, true);
	if (unlikely(ret))
		goto err;

	/*
		recipe VXLAN tunnel encapsulation IPV4 Over IPV4
		[L2][IPV4] --> [L2][IPV4][UDP][VXLAN][L2][IPV4]
	*/
	ret = mod_rcp_vxlan_routed_set(MOD_RCP_VXLANV4_V4_ENCAP,
				      EU_STW_SEL_L3_OFF0, true, RCP_HDR_IPV4,
				      true, true);
	if (unlikely(ret))
		goto err;

	/*
		recipe VXLAN tunnel encapsulation IPV4 Over IPV4
		[L2][IPV6] --> [L2][IPV6][UDP][VXLAN][L2][IPV6]
	*/
	ret = mod_rcp_ipv6_vxlan_routed_set(MOD_RCP_VXLANV6_V6_ENCAP,
				      EU_STW_SEL_L3_OFF0, RCP_HDR_IPV6,
				      true, true);
	if (unlikely(ret))
		goto err;

	/*
		recipe VXLAN tunnel encapsulation IPV6 Over IPV4
		[L2][IPV6] --> [L2][IPV4][UDP][VXLAN][L2][IPV6]
	*/
	ret = mod_rcp_ipv6_vxlan_routed_set(MOD_RCP_VXLANV4_V6_ENCAP,
				      EU_STW_SEL_L3_OFF0, RCP_HDR_IPV4,
				      true, true);
	if (unlikely(ret))
		goto err;

	/*
		recipe VXLAN tunnel encapsulation IPV6 Over IPV4, CSUM Disabled
		[L2][IPV6] --> [L2][IPV4][UDP][VXLAN][L2][IPV6]
	*/
	ret = mod_rcp_ipv6_vxlan_routed_set(MOD_RCP_VXLANV4_V6_ENCAP_NO_CS,
				      EU_STW_SEL_L3_OFF0, RCP_HDR_IPV4,
				      true, false);
	if (unlikely(ret))
		goto err;

	/*
		recipe VXLAN tunnel encapsulation IPV4 Over IPV4 CSUM disabled
		[L2][IPV4] --> [L2][IPV4][UDP][VXLAN][L2][IPV4]
	*/
	ret = mod_rcp_vxlan_routed_set(MOD_RCP_VXLANV4_V4_ENCAP_NO_CS,
				      EU_STW_SEL_L3_OFF0, true, RCP_HDR_IPV4,
				      true, false);
	if (unlikely(ret))
		goto err;

	ret = mod_rcp_udp_tunn_set(MOD_RCP_V4_IN_V4_UDP_ENCAP, RCP_HDR_IPV4,
				   RCP_HDR_IPV4, !DO_L2TP_LEN);
	if (unlikely(ret))
		goto err;

	ret = mod_rcp_udp_tunn_set(MOD_RCP_V4_IN_V6_UDP_ENCAP, RCP_HDR_IPV4,
				   RCP_HDR_IPV6, !DO_L2TP_LEN);
	if (unlikely(ret))
		goto err;

	ret = mod_rcp_udp_tunn_set(MOD_RCP_V6_IN_V4_UDP_ENCAP, RCP_HDR_IPV6,
				   RCP_HDR_IPV4, !DO_L2TP_LEN);
	if (unlikely(ret))
		goto err;

	ret = mod_rcp_udp_tunn_set(MOD_RCP_V6_IN_V6_UDP_ENCAP, RCP_HDR_IPV6,
				   RCP_HDR_IPV6, !DO_L2TP_LEN);
	if (unlikely(ret))
		goto err;

	ret = mod_rcp_udp_tunn_set(MOD_RCP_V4_IN_V4_UDP_L2TP_ENCAP,
				   RCP_HDR_IPV4, RCP_HDR_IPV4, DO_L2TP_LEN);
	if (unlikely(ret))
		goto err;

	ret = mod_rcp_udp_tunn_set(MOD_RCP_V4_IN_V6_UDP_L2TP_ENCAP,
				   RCP_HDR_IPV4, RCP_HDR_IPV6, DO_L2TP_LEN);
	if (unlikely(ret))
		goto err;

	ret = mod_rcp_udp_tunn_set(MOD_RCP_V6_IN_V4_UDP_L2TP_ENCAP,
				   RCP_HDR_IPV6, RCP_HDR_IPV4, DO_L2TP_LEN);
	if (unlikely(ret))
		goto err;

	ret = mod_rcp_udp_tunn_set(MOD_RCP_V6_IN_V6_UDP_L2TP_ENCAP,
				   RCP_HDR_IPV6, RCP_HDR_IPV6, DO_L2TP_LEN);
	if (unlikely(ret))
		goto err;

	/* config recipe MAP-T for translation from ipv4 to ipv6, do NAPT
	 * and PPPoE encapsulation
	 */
	ret = mod_rcp_ip_translation_set(MOD_RCP_IPV4_IPV6_TRANS,
					 RCP_HDR_IPV4, DO_PPPOE_ENCAP);
	if (unlikely(ret))
		goto err;

	/* config recipe MAP-T for translation from ipv6 to ipv4, do NAPT */
	ret = mod_rcp_ip_translation_set(MOD_RCP_IPV6_IPV4_TRANS,
					 RCP_HDR_IPV6, !DO_PPPOE_ENCAP);
	if (unlikely(ret))
		goto err;


	return 0;

err:
	pr_err("failed to init modifier recipes\n");
	return ret;
}
