/*
 * Copyright (C) 2020-2024 MaxLinear, Inc.
 * Copyright (C) 2019-2020 Intel Corporation
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

#ifndef _PKTPRS_H_
#define _PKTPRS_H_

#include <linux/etherdevice.h>
#include <linux/if_vlan.h>
#include <linux/if_pppox.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/sctp.h>
#include <linux/icmp.h>
#include <linux/icmpv6.h>
#include <linux/igmp.h>
#include <linux/l2tp.h>
#include <linux/ppp_defs.h>
#include <net/mpls.h>
#include <net/ip.h>
#include <net/ipv6.h>
#include <net/gre.h>
#include <net/geneve.h>
#include <net/vxlan.h>
#include <net/xfrm.h>
#include <linux/skbuff.h>

/**
 * @enum pktprs_proto
 * @brief protocol IDs
 */
enum pktprs_proto {
	PKTPRS_PROTO_MAC,
	PKTPRS_PROTO_VLAN0,
	PKTPRS_PROTO_VLAN1,
	PKTPRS_PROTO_VLAN2,
	PKTPRS_PROTO_VLAN3,
	PKTPRS_PROTO_VLAN4,
	PKTPRS_PROTO_PPPOE,
	PKTPRS_PROTO_MPLS0,
	PKTPRS_PROTO_MPLS1,
	PKTPRS_PROTO_MPLS2,
	PKTPRS_PROTO_MPLS3,
	PKTPRS_PROTO_IPV4,
	PKTPRS_PROTO_IPV6,
	PKTPRS_PROTO_HOP_OPT,
	PKTPRS_PROTO_DEST_OPT,
	PKTPRS_PROTO_ROUT_OPT,
	PKTPRS_PROTO_FRAG_OPT,
	PKTPRS_PROTO_L2TP_OIP,
	PKTPRS_PROTO_UDP,
	PKTPRS_PROTO_ESP,
	PKTPRS_PROTO_AUTH,
	PKTPRS_PROTO_VXLAN,
	PKTPRS_PROTO_GENEVE,
	PKTPRS_PROTO_L2TP_OUDP,
	PKTPRS_PROTO_PPP,
	PKTPRS_PROTO_TCP,
	PKTPRS_PROTO_ICMP,
	PKTPRS_PROTO_ICMP6,
	PKTPRS_PROTO_GRE,
	PKTPRS_PROTO_IGMP,
	PKTPRS_PROTO_SCTP,
	PKTPRS_PROTO_PAYLOAD,
	PKTPRS_PROTO_LAST = PKTPRS_PROTO_PAYLOAD,
	/* PKTPRS_PROTO_MAX = 64 (depends on proto_bmap size) */
};

/**
 * @brief Returns protocol string description
 * @param proto protocol id
 * @return const char* const
 */
#define PKTPRS_PROTO_STR(p) \
	(p == PKTPRS_PROTO_MAC       ? "MAC"           : \
	(p == PKTPRS_PROTO_VLAN0     ? "VLAN0"         : \
	(p == PKTPRS_PROTO_VLAN1     ? "VLAN1"         : \
	(p == PKTPRS_PROTO_VLAN2     ? "VLAN2"         : \
	(p == PKTPRS_PROTO_VLAN3     ? "VLAN3"         : \
	(p == PKTPRS_PROTO_VLAN4     ? "VLAN4"         : \
	(p == PKTPRS_PROTO_PPPOE     ? "PPPOE"         : \
	(p == PKTPRS_PROTO_MPLS0     ? "MPLS0"         : \
	(p == PKTPRS_PROTO_MPLS1     ? "MPLS1"         : \
	(p == PKTPRS_PROTO_MPLS2     ? "MPLS2"         : \
	(p == PKTPRS_PROTO_MPLS3     ? "MPLS3"         : \
	(p == PKTPRS_PROTO_IPV4      ? "IPV4"          : \
	(p == PKTPRS_PROTO_IPV6      ? "IPV6"          : \
	(p == PKTPRS_PROTO_HOP_OPT   ? "IPV6 HOP_OPT"  : \
	(p == PKTPRS_PROTO_DEST_OPT  ? "IPV6 DEST_OPT" : \
	(p == PKTPRS_PROTO_ROUT_OPT  ? "IPV6 ROUT_OPT" : \
	(p == PKTPRS_PROTO_FRAG_OPT  ? "IPV6 FRAG_OPT" : \
	(p == PKTPRS_PROTO_L2TP_OIP  ? "IPV6 L2TP_OIP" : \
	(p == PKTPRS_PROTO_UDP       ? "UDP"           : \
	(p == PKTPRS_PROTO_ESP       ? "ESP"           : \
	(p == PKTPRS_PROTO_AUTH      ? "AUTH"          : \
	(p == PKTPRS_PROTO_VXLAN     ? "VXLAN"         : \
	(p == PKTPRS_PROTO_GENEVE    ? "GENEVE"        : \
	(p == PKTPRS_PROTO_L2TP_OUDP ? "L2TP_OUDP"     : \
	(p == PKTPRS_PROTO_PPP       ? "PPP"           : \
	(p == PKTPRS_PROTO_TCP       ? "TCP"           : \
	(p == PKTPRS_PROTO_ICMP      ? "ICMP"          : \
	(p == PKTPRS_PROTO_ICMP6     ? "ICMP6"         : \
	(p == PKTPRS_PROTO_GRE       ? "GRE"           : \
	(p == PKTPRS_PROTO_IGMP      ? "IGMP"          : \
	(p == PKTPRS_PROTO_SCTP      ? "SCTP"          : \
	(p == PKTPRS_PROTO_PAYLOAD   ? "PAYLOAD"       : \
	("INVALID")))))))))))))))))))))))))))))))))

/**
 * @enum pktprs_hdr_level
 * @brief header level
 */
enum pktprs_hdr_level {
	PKTPRS_HDR_LEVEL0,
	PKTPRS_HDR_LEVEL1,
	PKTPRS_HDR_LEVEL2,
	PKTPRS_HDR_LEVEL_LAST = PKTPRS_HDR_LEVEL2,
};
#define PKTPRS_HDR_LEVEL_NUM (PKTPRS_HDR_LEVEL_LAST + 1)

#define MAX_VLAN_HDRS_SUPPORTED 5
#define MAX_MPLS_HDRS_SUPPORTED 4

/**
 * @struct pktprs_proto_info
 * @brief protocol information
 */
struct pktprs_proto_info {
	u8 off;
	u8 nxt;
};

/**
 * @define PKTPRS_HDR_BUF_SIZE
 * @brief header buffer size in bytes
 */
#define PKTPRS_HDR_BUF_SIZE 255

/**
 * @define pktprs extension name to register to mxl_skb_ext module
 */
#define PKTPRS_EXT_NAME     "pktprs"

/**
 * @struct pktprs_hdr
 * @brief packet parser header information
 */
struct pktprs_hdr {
	/*! header buffer */
	unsigned char buf[PKTPRS_HDR_BUF_SIZE];

	/*! header buffer size */
	u8 buf_sz;

	/*! protocol info per protocol per level */
	struct pktprs_proto_info proto_info[PKTPRS_PROTO_LAST + 1]
					   [PKTPRS_HDR_LEVEL_NUM];

	/*! protocols bitmap per level */
	unsigned long proto_bmap[PKTPRS_HDR_LEVEL_NUM];

	/*! device index */
	int ifindex;
};

/**
 * @struct pktprs_desc
 * @brief packet parser descriptor
 */
struct pktprs_desc {
	struct pktprs_hdr *rx;
	struct pktprs_hdr *tx;
	struct sk_buff *skb;
};

/**
 * @enum pktprs_type
 * @brief parsing type
 */
enum pktprs_type {
	PKTPRS_ETH_TX,
	PKTPRS_ETH_RX,
	PKTPRS_IPV4_TX,
	PKTPRS_IPV4_RX,
	PKTPRS_IPV6_TX,
	PKTPRS_IPV6_RX,
	PKTPRS_TYPE_MAX = 7,
};

/**
 * @enum pktprs_cb_type
 * @brief parsing callback type
 */
enum pktprs_cb_type {
	PKTPRS_RX,
	PKTPRS_TX,
	PKTPRS_RXTX,
	PKTPRS_DROP,
	PKTPRS_RXTX_DROP,
};

/**
 * @union proto_ptr
 * @brief protocol header pointer
 */
union proto_ptr {
	struct ethhdr *eth;
	struct vlan_hdr *vlan;
	struct pppoe_hdr *pppoe;
	struct iphdr *ipv4;
	struct ipv6hdr *ipv6;
	struct ipv6_opt_hdr *opt;
	struct frag_hdr *frag;
	struct udphdr *udp;
	struct ip_esp_hdr *esp;
	struct tcphdr *tcp;
	struct icmphdr *icmp;
	struct icmp6hdr *icmp6;
	struct gre_base_hdr *gre;
	union l2tphdr *l2tp;
	struct sctphdr *sctp;
	struct vxlanhdr *vxlan;
	struct genevehdr *geneve;
};

/*
 * L2TP stuff
 */
#define L2TP_HDRFLAG_T    0x8000
#define L2TP_HDRFLAG_L    0x4000
#define L2TP_HDRFLAG_S    0x0800
#define L2TP_HDRFLAG_O    0x0200
#define L2TP_HDRFLAG_P    0x0100

#define L2TP_HDR_VER_MASK   0x000F
#define L2TP_HDR_FLAGS_MASK 0xFFF0
#define L2TP_HDR_VER_2      0x0002
#define L2TP_HDR_VER_3      0x0003

/**
 * @struct l2tp_ctr_hdr
 * @brief l2tp control header
 */
struct l2tp_ctr_hdr {
	__be16 flags;
	__be16 length;
	__be32 ctr_con_id;
	__be16 ns;
	__be16 nr;
};

/**
 * @struct l2tp_v2_len_hdr
 * @brief l2tp V2 header with lentgh option
 */
struct l2tp_v2_len_hdr {
	__be16 flags;
	__be16 length;
	__be16 tunnel;
	__be16 sess_id;
};

/**
 * @struct l2tp_v2_hdr
 * @brief l2tp V2 header w/o options
 */
struct l2tp_v2_hdr {
	__be16 flags;
	__be16 tunnel;
	__be16 sess_id;
};

/**
 * @struct l2tp_v3_hdr
 * @brief l2tp V3 header
 */
struct l2tp_v3_hdr {
	__be16 flags;
	__be16 res;
	__be32 sess_id;
};

/**
 * @struct l2tp_v3_oip_hdr
 * @brief l2tp V3 over IP header
 */
struct l2tp_v3_oip_hdr {
	__be32 sess_id;
	struct l2tp_ctr_hdr ctr;
};

/**
 * @union l2tphdr
 * @brief l2tp header union
 */
union l2tphdr {
	struct l2tp_ctr_hdr ctr;
	struct l2tp_v2_len_hdr v2_len;
	struct l2tp_v2_hdr v2;
	struct l2tp_v3_hdr v3_oudp;
	struct l2tp_v3_oip_hdr v3_oip;
};

/*
 * HELPERS
 */

#define PKTPRS_PROTO_NEXT(h, p, l) ((h)->proto_info[(p)][(l)].nxt)
#define PKTPRS_IS_LVL_VALID(l) ((l) >= 0 && (l) < PKTPRS_HDR_LEVEL_NUM)
#define PKTPRS_IS_PROTO_VALID(p) (p < PKTPRS_PROTO_LAST + 1)

#define PKTPRS_IS_MAC(h, l)       \
	test_bit(PKTPRS_PROTO_MAC,      &(h)->proto_bmap[l])

#define PKTPRS_IS_VLAN0(h, l)     \
	test_bit(PKTPRS_PROTO_VLAN0,    &(h)->proto_bmap[l])

#define PKTPRS_IS_VLAN1(h, l)     \
	test_bit(PKTPRS_PROTO_VLAN1,    &(h)->proto_bmap[l])

#define PKTPRS_IS_VLAN2(h, l)     \
	test_bit(PKTPRS_PROTO_VLAN2,    &(h)->proto_bmap[l])

#define PKTPRS_IS_VLAN3(h, l)     \
	test_bit(PKTPRS_PROTO_VLAN3,    &(h)->proto_bmap[l])

#define PKTPRS_IS_VLAN4(h, l)     \
	test_bit(PKTPRS_PROTO_VLAN4,    &(h)->proto_bmap[l])

#define PKTPRS_IS_PPPOE(h, l)     \
	test_bit(PKTPRS_PROTO_PPPOE,    &(h)->proto_bmap[l])

#define PKTPRS_IS_MPLS0(h, l)     \
	test_bit(PKTPRS_PROTO_MPLS0,    &(h)->proto_bmap[l])

#define PKTPRS_IS_MPLS1(h, l)     \
	test_bit(PKTPRS_PROTO_MPLS1,    &(h)->proto_bmap[l])

#define PKTPRS_IS_MPLS2(h, l)     \
	test_bit(PKTPRS_PROTO_MPLS2,    &(h)->proto_bmap[l])

#define PKTPRS_IS_MPLS3(h, l)     \
	test_bit(PKTPRS_PROTO_MPLS3,    &(h)->proto_bmap[l])

#define PKTPRS_IS_IPV4(h, l)      \
	test_bit(PKTPRS_PROTO_IPV4,     &(h)->proto_bmap[l])

#define PKTPRS_IS_IPV6(h, l)      \
	test_bit(PKTPRS_PROTO_IPV6,     &(h)->proto_bmap[l])

#define PKTPRS_IS_UDP(h, l)       \
	test_bit(PKTPRS_PROTO_UDP,      &(h)->proto_bmap[l])

#define PKTPRS_IS_TCP(h, l)       \
	test_bit(PKTPRS_PROTO_TCP,      &(h)->proto_bmap[l])

#define PKTPRS_IS_SCTP(h, l)      \
	test_bit(PKTPRS_PROTO_SCTP,     &(h)->proto_bmap[l])

#define PKTPRS_IS_VXLAN(h, l)     \
	test_bit(PKTPRS_PROTO_VXLAN,    &(h)->proto_bmap[l])

#define PKTPRS_IS_ICMP(h, l)      \
	test_bit(PKTPRS_PROTO_ICMP,     &(h)->proto_bmap[l])

#define PKTPRS_IS_ICMP6(h, l)     \
	test_bit(PKTPRS_PROTO_ICMP6,    &(h)->proto_bmap[l])

#define PKTPRS_IS_ESP(h, l)       \
	test_bit(PKTPRS_PROTO_ESP,      &(h)->proto_bmap[l])

#define PKTPRS_IS_AUTH(h, l)      \
	test_bit(PKTPRS_PROTO_AUTH,     &(h)->proto_bmap[l])

#define PKTPRS_IS_GRE(h, l)       \
	test_bit(PKTPRS_PROTO_GRE,      &(h)->proto_bmap[l])

#define PKTPRS_IS_IGMP(h, l)      \
	test_bit(PKTPRS_PROTO_IGMP,     &(h)->proto_bmap[l])

#define PKTPRS_IS_L2TP_OIP(h, l)  \
	test_bit(PKTPRS_PROTO_L2TP_OIP, &(h)->proto_bmap[l])

#define PKTPRS_IS_L2TP_OUDP(h, l) \
	test_bit(PKTPRS_PROTO_L2TP_OUDP, &(h)->proto_bmap[l])

#define PKTPRS_IS_PPP(h, l)       \
	test_bit(PKTPRS_PROTO_PPP,      &(h)->proto_bmap[l])

#define PKTPRS_IS_GENEVE(h, l)    \
	test_bit(PKTPRS_PROTO_GENEVE,   &(h)->proto_bmap[l])

#define PKTPRS_IS_HOP_OPT(h, l)   \
	test_bit(PKTPRS_PROTO_HOP_OPT,  &(h)->proto_bmap[l])

#define PKTPRS_IS_DEST_OPT(h, l)  \
	test_bit(PKTPRS_PROTO_DEST_OPT, &(h)->proto_bmap[l])

#define PKTPRS_IS_ROUT_OPT(h, l)  \
	test_bit(PKTPRS_PROTO_ROUT_OPT, &(h)->proto_bmap[l])

#define PKTPRS_IS_FRAG_OPT(h, l)  \
	test_bit(PKTPRS_PROTO_FRAG_OPT, &(h)->proto_bmap[l])

#define PKTPRS_IS_VLAN(h, l, i)   \
	test_bit(PKTPRS_PROTO_VLAN0 + (i),  &(h)->proto_bmap[l])

#define PKTPRS_VLAN_EXIST(h, l)   \
	((h)->proto_bmap[l] & (BIT(PKTPRS_PROTO_VLAN0) | \
			       BIT(PKTPRS_PROTO_VLAN1) | \
			       BIT(PKTPRS_PROTO_VLAN2) | \
			       BIT(PKTPRS_PROTO_VLAN3) | \
			       BIT(PKTPRS_PROTO_VLAN4)))

#define PKTPRS_IS_MPLS(h, l, i)   \
	test_bit(PKTPRS_PROTO_MPLS0 + (i),  &(h)->proto_bmap[l])

#define PKTPRS_MPLS_EXIST(h, l)   \
	((h)->proto_bmap[l] & (BIT(PKTPRS_PROTO_MPLS0) | \
			       BIT(PKTPRS_PROTO_MPLS1) | \
			       BIT(PKTPRS_PROTO_MPLS2) | \
			       BIT(PKTPRS_PROTO_MPLS3)))

#define PKTPRS_IS_LEVEL(h, l)     ((h)->proto_bmap[l])

#define PKTPRS_IS_MULTI_LEVEL(h)  \
	(PKTPRS_IS_LEVEL(h, PKTPRS_HDR_LEVEL0) && \
	PKTPRS_IS_LEVEL(h, PKTPRS_HDR_LEVEL1))

#define PKTPRS_IS_IP(h, l)        \
	(PKTPRS_IS_IPV4(h, l) || PKTPRS_IS_IPV6(h, l))

#define PKTPRS_IS_MULTI_IP(h)     \
	(PKTPRS_IS_IP(h, PKTPRS_HDR_LEVEL0) && \
	PKTPRS_IS_IP(h, PKTPRS_HDR_LEVEL1))

#define PKTPRS_IS_DSLITE(h)       \
	(PKTPRS_IS_IPV6(h, PKTPRS_HDR_LEVEL0) && \
	 ((PKTPRS_PROTO_NEXT(h, PKTPRS_PROTO_IPV6, PKTPRS_HDR_LEVEL0) == \
	   PKTPRS_PROTO_IPV4) ||  \
	  (PKTPRS_PROTO_NEXT(h, PKTPRS_PROTO_HOP_OPT, PKTPRS_HDR_LEVEL0) == \
	   PKTPRS_PROTO_IPV4) ||  \
	  (PKTPRS_PROTO_NEXT(h, PKTPRS_PROTO_DEST_OPT, PKTPRS_HDR_LEVEL0) == \
	   PKTPRS_PROTO_IPV4) ||  \
	  (PKTPRS_PROTO_NEXT(h, PKTPRS_PROTO_ROUT_OPT, PKTPRS_HDR_LEVEL0) == \
	   PKTPRS_PROTO_IPV4) ||  \
	  (PKTPRS_PROTO_NEXT(h, PKTPRS_PROTO_FRAG_OPT, PKTPRS_HDR_LEVEL0) == \
	   PKTPRS_PROTO_IPV4)))

#define PKTPRS_IS_SIXRD(h)        \
	(PKTPRS_IS_IPV4(h, PKTPRS_HDR_LEVEL0) &&  \
	(PKTPRS_PROTO_NEXT(h, PKTPRS_PROTO_IPV4, PKTPRS_HDR_LEVEL0) == \
	PKTPRS_PROTO_IPV6))

#define PKTPRS_IS_L2_GRE(h)       \
	(PKTPRS_IS_GRE(h, PKTPRS_HDR_LEVEL0) && \
	(PKTPRS_PROTO_NEXT(h, PKTPRS_PROTO_GRE, PKTPRS_HDR_LEVEL0) == \
	PKTPRS_PROTO_MAC))

#define PKTPRS_IS_IP_GRE(h)       \
	(PKTPRS_IS_GRE(h, PKTPRS_HDR_LEVEL0) && \
	(PKTPRS_PROTO_NEXT(h, PKTPRS_PROTO_GRE, PKTPRS_HDR_LEVEL0) == \
	PKTPRS_PROTO_IPV4 || \
	PKTPRS_PROTO_NEXT(h, PKTPRS_PROTO_GRE, PKTPRS_HDR_LEVEL0) == \
	PKTPRS_PROTO_IPV6))

#define PKTPRS_IS_ESP_OIP(h)      \
	(PKTPRS_IS_ESP(h, PKTPRS_HDR_LEVEL0) && \
	!PKTPRS_IS_UDP(h, PKTPRS_HDR_LEVEL0))

#define PKTPRS_IS_ESP_OUDP(h)     \
	(PKTPRS_IS_ESP(h, PKTPRS_HDR_LEVEL0) && \
	PKTPRS_IS_UDP(h, PKTPRS_HDR_LEVEL0))

/**
 * @brief get the protocol header offset
 * @param h parser header
 * @param p protocol ID
 * @param l header level
 * @return s16 protocol header offset
 */
static inline s16 pktprs_hdr_off(struct pktprs_hdr *h, enum pktprs_proto p,
				 enum pktprs_hdr_level l)
{
	if (!h || !PKTPRS_IS_PROTO_VALID(p) || !PKTPRS_IS_LVL_VALID(l))
		return -1;
	if (!test_bit(p, &h->proto_bmap[l]))
		return -1;

	return h->proto_info[p][l].off;
}

/**
 * @brief Get IP header (IPv4 or IPv6) offset
 * @param h parser header
 * @param l header level
 * @return s16 protocol header offset, -1 if no IP header exist
 */
static inline int pktprs_ip_hdr_off(struct pktprs_hdr *h, enum pktprs_hdr_level l)
{
	if (PKTPRS_IS_IPV4(h, l))
		return pktprs_hdr_off(h, PKTPRS_PROTO_IPV4, l);
	else if (PKTPRS_IS_IPV6(h, l))
		return pktprs_hdr_off(h, PKTPRS_PROTO_IPV6, l);

	return -1;
}

/**
 * @brief get the header size up to the given protocol (including)
 * @param h parser header
 * @param p protocol ID
 * @param l header level
 * @return s16 protocol header size
 */
static inline s16 pktprs_hdr_sz(struct pktprs_hdr *h, enum pktprs_proto p,
				enum pktprs_hdr_level l)
{
	u8 lvl, off;

	if (!h || !PKTPRS_IS_PROTO_VALID(p) || !PKTPRS_IS_LVL_VALID(l))
		return 0;
	if (!test_bit(p, &h->proto_bmap[l]))
		return 0;

	for (lvl = l; lvl < PKTPRS_HDR_LEVEL_NUM; lvl++) {
		off = pktprs_hdr_off(h, PKTPRS_PROTO_NEXT(h, p, l), lvl);
		if (off > h->proto_info[p][l].off)
			return off;
	}

	return 0;
}

/**
 * @brief get the header size up to the IP header (IPv4 or IPv6) (including)
 * @param h parser header
 * @param l header level
 * @return s16 protocol header offset, -1 if no IP header exist
 */
static inline int pktprs_ip_hdr_sz(struct pktprs_hdr *h, enum pktprs_hdr_level l)
{
	if (PKTPRS_IS_IPV4(h, l))
		return pktprs_hdr_sz(h, PKTPRS_PROTO_IPV4, l);
	else if (PKTPRS_IS_IPV6(h, l))
		return pktprs_hdr_sz(h, PKTPRS_PROTO_IPV6, l);

	return -1;
}

/**
 * @brief get the protocol header pointer
 * @param h parser header
 * @param p protocol ID
 * @param l header level
 * @return void * protocol header pointer
 */
static inline void *pktprs_hdr(struct pktprs_hdr *h, enum pktprs_proto p,
			       enum pktprs_hdr_level l)
{
	if (!h || !PKTPRS_IS_PROTO_VALID(p) || !PKTPRS_IS_LVL_VALID(l))
		return NULL;
	if (!test_bit(p, &h->proto_bmap[l]))
		return NULL;

	return &h->buf[h->proto_info[p][l].off];
}

/**
 * @brief get the ethernet header pointer
 * @param h parser header
 * @param l header level
 * @return struct ethhdr * ethernet header pointer
 */
static inline struct ethhdr *pktprs_eth_hdr(struct pktprs_hdr *h,
					    enum pktprs_hdr_level l)
{
	return (struct ethhdr *)pktprs_hdr(h, PKTPRS_PROTO_MAC, l);
}

/**
 * @brief get the vlan header pointer
 * @param h parser header
 * @param l header level
 * @param id vlan index inside the header (0 for external)
 * @return struct vlan_hdr * vlan header pointer
 */
static inline struct vlan_hdr *pktprs_vlan_hdr(struct pktprs_hdr *h,
					       enum pktprs_hdr_level l, int id)
{
	if (id >= MAX_VLAN_HDRS_SUPPORTED)
		return NULL;

	return (struct vlan_hdr *)pktprs_hdr(h, PKTPRS_PROTO_VLAN0 + id, l);
}

/**
 * @brief get the mpls header pointer
 * @param h parser header
 * @param l header level
 * @param id mpls index inside the header
 * @return struct mpls_shim_hdr * mpls header pointer
 */
static inline struct mpls_shim_hdr *pktprs_mpls_hdr(struct pktprs_hdr *h,
						 enum pktprs_hdr_level l, int id)
{
	if (id >= MAX_MPLS_HDRS_SUPPORTED)
		return NULL;

	return (struct mpls_shim_hdr *)pktprs_hdr(h, PKTPRS_PROTO_MPLS0 + id, l);
}

/**
 * @brief get the ipv4 header pointer
 * @param h parser header
 * @param l header level
 * @return struct iphdr * ipv4 header pointer
 */
static inline struct iphdr *pktprs_ipv4_hdr(struct pktprs_hdr *h,
					    enum pktprs_hdr_level l)
{
	return (struct iphdr *)pktprs_hdr(h, PKTPRS_PROTO_IPV4, l);
}

/**
 * @brief get the ipv6 header pointer
 * @param h parser header
 * @param l header level
 * @return struct ipv6hdr * ipv6 header pointer
 */
static inline struct ipv6hdr *pktprs_ipv6_hdr(struct pktprs_hdr *h,
					      enum pktprs_hdr_level l)
{
	return (struct ipv6hdr *)pktprs_hdr(h, PKTPRS_PROTO_IPV6, l);
}

/**
 * @brief get the udp header pointer
 * @param h parser header
 * @param l header level
 * @return struct udphdr * udp header pointer
 */
static inline struct udphdr *pktprs_udp_hdr(struct pktprs_hdr *h,
					    enum pktprs_hdr_level l)
{
	return (struct udphdr *)pktprs_hdr(h, PKTPRS_PROTO_UDP, l);
}

/**
 * @brief get the tcp header pointer
 * @param h parser header
 * @param l header level
 * @return struct tcphdr * tcp header pointer
 */
static inline struct tcphdr *pktprs_tcp_hdr(struct pktprs_hdr *h,
					    enum pktprs_hdr_level l)
{
	return (struct tcphdr *)pktprs_hdr(h, PKTPRS_PROTO_TCP, l);
}

/**
 * @brief get the icmp header pointer
 * @param h parser header
 * @param l header level
 * @return struct icmphdr * icmp header pointer
 */
static inline struct icmphdr *pktprs_icmp_hdr(struct pktprs_hdr *h,
					      enum pktprs_hdr_level l)
{
	return (struct icmphdr *)pktprs_hdr(h, PKTPRS_PROTO_ICMP, l);
}

/**
 * @brief get the icmpV6 header pointer
 * @param h parser header
 * @param l header level
 * @return struct icmp6hdr * icmpV6 header pointer
 */
static inline struct icmp6hdr *pktprs_icmp6_hdr(struct pktprs_hdr *h,
						enum pktprs_hdr_level l)
{
	return (struct icmp6hdr *)pktprs_hdr(h, PKTPRS_PROTO_ICMP6, l);
}

/**
 * @brief get the igmp header pointer
 * @param h parser header
 * @param l header level
 * @return struct igmphdr * igmp header pointer
 */
static inline struct igmphdr *pktprs_igmp_hdr(struct pktprs_hdr *h,
					      enum pktprs_hdr_level l)
{
	return (struct igmphdr *)pktprs_hdr(h, PKTPRS_PROTO_IGMP, l);
}

/**
 * @brief get the esp header pointer
 * @param h parser header
 * @param l header level
 * @return struct ip_esp_hdr * esp header pointer
 */
static inline struct ip_esp_hdr *pktprs_esp_hdr(struct pktprs_hdr *h,
						enum pktprs_hdr_level l)
{
	return (struct ip_esp_hdr *)pktprs_hdr(h, PKTPRS_PROTO_ESP, l);
}

/**
 * @brief get the auth header pointer
 * @param h parser header
 * @param l header level
 * @return struct ip_auth_hdr * auth header pointer
 */
static inline struct ip_auth_hdr *pktprs_auth_hdr(struct pktprs_hdr *h,
						  enum pktprs_hdr_level l)
{
	return (struct ip_auth_hdr *)pktprs_hdr(h, PKTPRS_PROTO_AUTH, l);
}

/**
 * @brief get the gre header pointer
 * @param h parser header
 * @param l header level
 * @return struct gre_base_hdr * gre header pointer
 */
static inline struct gre_base_hdr *pktprs_gre_hdr(struct pktprs_hdr *h,
						  enum pktprs_hdr_level l)
{
	return (struct gre_base_hdr *)pktprs_hdr(h, PKTPRS_PROTO_GRE, l);
}

/**
 * @brief get the pppoe header pointer
 * @param h parser header
 * @param l header level
 * @return struct pppoe_hdr * pppoe header pointer
 */
static inline struct pppoe_hdr *pktprs_pppoe_hdr(struct pktprs_hdr *h,
						 enum pktprs_hdr_level l)
{
	return (struct pppoe_hdr *)pktprs_hdr(h, PKTPRS_PROTO_PPPOE, l);
}

/**
 * @brief get the geneve header pointer
 * @param h parser header
 * @param l header level
 * @return struct genevehdr * geneve header pointer
 */
static inline struct genevehdr *pktprs_geneve_hdr(struct pktprs_hdr *h,
						  enum pktprs_hdr_level l)
{
	return (struct genevehdr *)pktprs_hdr(h, PKTPRS_PROTO_GENEVE, l);
}

/**
 * @brief get the sctp header pointer
 * @param h parser header
 * @param l header level
 * @return struct sctphdr * sctp header pointer
 */
static inline struct sctphdr *pktprs_sctp_hdr(struct pktprs_hdr *h,
					      enum pktprs_hdr_level l)
{
	return (struct sctphdr *)pktprs_hdr(h, PKTPRS_PROTO_SCTP, l);
}

/**
 * @brief get the vxlan header pointer
 * @param h parser header
 * @param l header level
 * @return struct vxlanhdr * vxlan header pointer
 */
static inline struct vxlanhdr *pktprs_vxlan_hdr(struct pktprs_hdr *h,
						enum pktprs_hdr_level l)
{
	return (struct vxlanhdr *)pktprs_hdr(h, PKTPRS_PROTO_VXLAN, l);
}

/**
 * @brief get the ipv6 hop by hop extention header pointer
 * @param h parser header
 * @param l header level
 * @return struct ipv6_opt_hdr * ipv6 hop by hop extention header pointer
 */
static inline struct ipv6_opt_hdr *pktprs_hop_opt_hdr(struct pktprs_hdr *h,
						      enum pktprs_hdr_level l)
{
	return (struct ipv6_opt_hdr *)pktprs_hdr(h, PKTPRS_PROTO_HOP_OPT, l);
}

/**
 * @brief get the ipv6 dest extention header pointer
 * @param h parser header
 * @param l header level
 * @return struct ipv6_opt_hdr * ipv6 dest extention header pointer
 */
static inline struct ipv6_opt_hdr *pktprs_dest_opt_hdr(struct pktprs_hdr *h,
						       enum pktprs_hdr_level l)
{
	return (struct ipv6_opt_hdr *)pktprs_hdr(h, PKTPRS_PROTO_DEST_OPT, l);
}

/**
 * @brief get the ipv6 rout extention header pointer
 * @param h parser header
 * @param l header level
 * @return struct ipv6_opt_hdr * ipv6 rout extention header pointer
 */
static inline struct ipv6_opt_hdr *pktprs_rout_opt_hdr(struct pktprs_hdr *h,
						       enum pktprs_hdr_level l)
{
	return (struct ipv6_opt_hdr *)pktprs_hdr(h, PKTPRS_PROTO_ROUT_OPT, l);
}

/**
 * @brief get the ipv6 frag extention header pointer
 * @param h parser header
 * @param l header level
 * @return struct frag_hdr * ipv6 frag extention header pointer
 */
static inline struct frag_hdr *pktprs_frag_opt_hdr(struct pktprs_hdr *h,
						   enum pktprs_hdr_level l)
{
	return (struct frag_hdr *)pktprs_hdr(h, PKTPRS_PROTO_FRAG_OPT, l);
}

/**
 * @brief get the l2tp over ip header pointer
 * @param h parser header
 * @param l header level
 * @return union l2tphdr * l2tp header pointer
 */
static inline union l2tphdr *pktprs_l2tp_oip_hdr(struct pktprs_hdr *h,
						 enum pktprs_hdr_level l)
{
	return (union l2tphdr *)pktprs_hdr(h, PKTPRS_PROTO_L2TP_OIP, l);
}

/**
 * @brief get the header size up to the l2tp over ip (included)
 * @param h parser header
 * @param l header level
 * @return s16
 */
static inline s16 pktprs_l2tp_oip_hdr_sz(struct pktprs_hdr *h,
					enum pktprs_hdr_level l)
{
	return pktprs_hdr_sz(h, PKTPRS_PROTO_L2TP_OIP, l);
}

/**
 * @brief get the l2tp over ip header offset
 * @param h parser header
 * @param l header level
 * @return s16
 */
static inline s16 pktprs_l2tp_oip_hdr_off(struct pktprs_hdr *h,
					enum pktprs_hdr_level l)
{
	return pktprs_hdr_off(h, PKTPRS_PROTO_L2TP_OIP, l);
}

/**
 * @brief get the l2tp over udp header pointer
 * @param h parser header
 * @param l header level
 * @return union l2tphdr * l2tp header pointer
 */
static inline union l2tphdr *pktprs_l2tp_oudp_hdr(struct pktprs_hdr *h,
						  enum pktprs_hdr_level l)
{
	return (union l2tphdr *)pktprs_hdr(h, PKTPRS_PROTO_L2TP_OUDP, l);
}

/**
 * @brief get the header size up to the l2tp over udp (included)
 * @param h parser header
 * @param l header level
 * @return s16
 */
static inline s16 pktprs_l2tp_oudp_hdr_sz(struct pktprs_hdr *h,
					  enum pktprs_hdr_level l)
{
	return pktprs_hdr_sz(h, PKTPRS_PROTO_L2TP_OUDP, l);
}

/**
 * @brief get the l2tp over udp header offset
 * @param h parser header
 * @param l header level
 * @return s16
 */
static inline s16 pktprs_l2tp_oudp_hdr_off(struct pktprs_hdr *h,
					   enum pktprs_hdr_level l)
{
	return pktprs_hdr_off(h, PKTPRS_PROTO_L2TP_OUDP, l);
}

/**
 * @brief get the ppp header pointer
 * @param h parser header
 * @param l header level
 * @return unsigned char * ppp header pointer
 */
static inline unsigned char *pktprs_ppp_hdr(struct pktprs_hdr *h,
					    enum pktprs_hdr_level l)
{
	return (unsigned char *)pktprs_hdr(h, PKTPRS_PROTO_PPP, l);
}

/**
 * @brief check if the header include ip frag
 * @param h parser header
 * @param l header level
 * @return bool true if include ip(v4/v6) frag
 */
static inline bool pktprs_is_ip_frag(struct pktprs_hdr *h,
				     enum pktprs_hdr_level l)
{
	if (!h)
		return false;

	if (PKTPRS_IS_IPV4(h, l))
		return ip_is_fragment(pktprs_ipv4_hdr(h, l));
	if (PKTPRS_IS_FRAG_OPT(h, l))
		return ((pktprs_frag_opt_hdr(h, l)->frag_off &
			htons(IP6_OFFSET | IP6_MF)) != 0);

	return false;
}

/**
 * @brief check if the header include ipv4 first frag
 * @param h parser header
 * @param l header level
 * @return bool true if include ipv4 first frag
 */
static inline bool pktprs_ipv4_first_frag(struct pktprs_hdr *h,
					  enum pktprs_hdr_level l)
{
	if (!h)
		return false;

	if (!PKTPRS_IS_IPV4(h, l))
		return false;
	return ((pktprs_ipv4_hdr(h, l)->frag_off & htons(IP_MF)) &&
		!(pktprs_ipv4_hdr(h, l)->frag_off & htons(IP_OFFSET)));
}

/**
 * @brief check if the header include ipv6 first frag
 * @param h parser header
 * @param l header level
 * @return bool true if include ipv6 first frag
 */
static inline bool pktprs_ipv6_first_frag(struct pktprs_hdr *h,
					  enum pktprs_hdr_level l)
{
	if (!h)
		return false;

	if (!PKTPRS_IS_FRAG_OPT(h, l))
		return false;
	return ((pktprs_frag_opt_hdr(h, l)->frag_off & htons(IP6_MF)) &&
		!(pktprs_frag_opt_hdr(h, l)->frag_off & htons(IP6_OFFSET)));
}

/**
 * @brief check if the header include ip(v4/v6) first frag
 * @param h parser header
 * @param l header level
 * @return bool true if include ip(v4/v6) first frag
 */
static inline bool pktprs_first_frag(struct pktprs_hdr *h,
				     enum pktprs_hdr_level l)
{
	if (!h)
		return false;

	return pktprs_ipv4_first_frag(h, l) || pktprs_ipv6_first_frag(h, l);
}

/**
 * @brief check if the header include ip(v4/v6) multicast ip
 * @param h parser header
 * @param l header level
 * @return bool true if include ip(v4/v6) multicast ip
 */
static inline bool pktprs_is_ip_mcast(struct pktprs_hdr *h,
				      enum pktprs_hdr_level l)
{
	if (!h)
		return false;

	if (PKTPRS_IS_IPV4(h, l)) {
		if (ipv4_is_multicast(pktprs_ipv4_hdr(h, l)->daddr) &&
		    !ipv4_is_local_multicast(pktprs_ipv4_hdr(h, l)->daddr))
			return true;

	}

#if IS_ENABLED(CONFIG_IPV6)
	if (PKTPRS_IS_IPV6(h, l)) {
		if (ipv6_addr_is_multicast(&pktprs_ipv6_hdr(h, l)->daddr) &&
		    !ipv6_addr_is_ll_all_nodes(&pktprs_ipv6_hdr(h, l)->daddr))
			return true;
	}
#endif

	return false;
}

static inline bool pktprs_is_l2tpv2(struct pktprs_hdr *h,
				    enum pktprs_hdr_level l)
{
	if (!h)
		return false;

	if (PKTPRS_IS_L2TP_OUDP(h, l)) {
		if (L2TP_HDR_VER_2 ==
		    (pktprs_l2tp_oudp_hdr(h, l)->v2.flags & L2TP_HDR_VER_MASK))
			return true;
	}

	return false;
}

/**
 * @brief get num vlan headers
 * @param h parser header
 * @param l header level
 * @return num vlan headers
 */
static inline u16 pktprs_num_vlan_hdrs(struct pktprs_hdr *h,
				       enum pktprs_hdr_level l)
{
	u16 num = 0;
	u16 i;

	if (!h)
		return 0;

	for (i = 0; i < MAX_VLAN_HDRS_SUPPORTED; i++)
		if (PKTPRS_IS_VLAN(h, l, i))
			num++;

	return num;
}

/**
 * @brief get the last ether type
 *        copied from eth hdr if not vlan, or from last vlan hdr if vlan exists
 * @param h parser header
 * @param l header level
 * @return last ether type
 */
static inline u16 pktprs_last_ethtype_get(struct pktprs_hdr *h,
					  enum pktprs_hdr_level l)
{
	u16 num_vlans;

	if (!PKTPRS_IS_MAC(h, l))
		return 0;

	num_vlans = pktprs_num_vlan_hdrs(h, l);
	if (num_vlans)
		return ntohs(pktprs_vlan_hdr(h, l,
				num_vlans - 1)->h_vlan_encapsulated_proto);
	else
		return ntohs(pktprs_eth_hdr(h, l)->h_proto);
}

/**
 * @brief get the header information from the skb cookie
 * @param skb packet skb
 * @return struct pktprs_hdr * header information
 */
struct pktprs_hdr *pktprs_skb_hdr_get(struct sk_buff *skb);

/**
 * @brief register to the packet parser callback
 * @param nb notifier true
 * @param t type of callback
 * @return int 0 for success
 */
int pktprs_register(struct notifier_block *nb, enum pktprs_cb_type t);

/**
 * @brief unregister from the packet parser callback
 * @param nb notifier true
 * @param t type of callback
 */
void pktprs_unregister(struct notifier_block *nb, enum pktprs_cb_type t);

/**
 * @brief add device for the parser operation
 * @param dev net device
 * @return int 0 for success
 */
int pktprs_dev_add(struct net_device *dev);

/**
 * @brief remove device from the parser operation
 * @param dev net device
 * @return int 0 for success
 */
int pktprs_dev_remove(struct net_device *dev);

/**
 * @brief parse a given skb
 * @param skb skb to parse
 * @param dev net device
 * @param prs_from start parsing type
 */
void pktprs_do_parse(struct sk_buff *skb, struct net_device *dev,
		     enum pktprs_type prs_from);

/**
 * @brief parse a given skb and get the parsed header directly
 * @note using this API is NOT required to register with cb as a user
 * @param skb skb to parse
 * @param dev net device
 * @param h the parsing header to fill
 * @param prs_from start parsing type
 * @return int 0 for success
 */
int pktprs_do_parse_direct(struct sk_buff *skb, struct net_device *dev,
			   struct pktprs_hdr *h, enum pktprs_type prs_from);

/**
 * @brief remove protocol from header
 * @param h parser header
 * @param p protocol ID to remove
 * @param l header level
 * @return int 0 for success
 */
int pktprs_proto_remove(struct pktprs_hdr *h, enum pktprs_proto p,
			enum pktprs_hdr_level l);

/**
 * @brief remove all protocols after protocol p
 * @param h parser header
 * @param p protocol ID to start remove after it
 * @param l header level
 * @return int 0 for success
 */
int pktprs_proto_remove_after(struct pktprs_hdr *h, enum pktprs_proto p,
			      enum pktprs_hdr_level l);

/**
 * @brief Remove ipv6 header including all extensions
 * @param h parser header
 * @param l header level
 * @return int 0 for success, error code otherwise
 */
int pktprs_ipv6_remove(struct pktprs_hdr *h, enum pktprs_hdr_level l);

/**
 * @brief get the last ipv6 protocol offset from the start
 *        ipv6 header
 * @param ip6 pointer to an ipv6 header
 * @return u16 next protocol pointer
 */
u16 pktprs_ip6_proto_off(struct ipv6hdr *ip6);

/**
 * @brief Remove the IP header (IPv4 or IPv6) from the header
 * @param h the header
 * @param l header level
 * @return int 0 for success, error code otherwise
 */
static inline int pktprs_ip_remove(struct pktprs_hdr *h, enum pktprs_hdr_level l)
{
	if (PKTPRS_IS_IPV4(h, l))
		return pktprs_proto_remove(h, PKTPRS_PROTO_IPV4, l);
	else
		return pktprs_ipv6_remove(h, l);
}

/**
 * @brief print pktprs header
 * @param h pktprs header
 * @param t parse type
 */
void pktprs_pr_hdr(struct pktprs_hdr *h, enum pktprs_type t);

/**
 * @brief print igmp header
 * @param igmp header pointer
 */
void igmp_print(struct igmphdr *igmp);

/**
 * @brief print icmp header
 * @param icmp header pointer
 */
void icmp_print(struct icmphdr *icmp);

/**
 * @brief print icmp6 header
 * @param icmp6 header pointer
 */
void icmp6_print(struct icmp6hdr *icmp6);

/**
 * @brief print l2tp over udp header
 * @param l2tp header pointer
 */
void l2tp_oudp_print(union l2tphdr *l2tp);

/**
 * @brief print l2tp over ip header
 * @param l2tp header pointer
 */
void l2tp_oip_print(union l2tphdr *l2tp);

/**
 * @brief print ppp header
 * @param ppp header pointer
 */
void ppp_print(unsigned char *ppp);

/**
 * @brief print tcp header
 * @param tcp header pointer
 */
void tcp_print(struct tcphdr *tcp);

/**
 * @brief print udp header
 * @param udp header pointer
 */
void udp_print(struct udphdr *udp);

/**
 * @brief print geneve header
 * @param geneve header pointer
 */
void geneve_print(struct genevehdr *geneve);

/**
 * @brief print gre header
 * @param gre header pointer
 */
void gre_print(struct gre_base_hdr *gre);

/**
 * @brief print esp header
 * @param esp header pointer
 */
void esp_print(struct ip_esp_hdr *esp);

/**
 * @brief print auth header
 * @param auth header pointer
 */
void auth_print(struct ip_auth_hdr *auth);

/**
 * @brief print sctp header
 * @param sctp header pointer
 */
void sctp_print(struct sctphdr *sctp);

/**
 * @brief print vxlan header
 * @param vxlan header pointer
 */
void vxlan_print(struct vxlanhdr *vxlan);

/**
 * @brief print ipv6 routing extention header
 * @param opt header pointer
 */
void ipv6_rout_opt_print(struct ipv6_opt_hdr *opt);

/**
 * @brief print ipv6 destination extention header
 * @param opt header pointer
 */
void ipv6_dest_opt_print(struct ipv6_opt_hdr *opt);

/**
 * @brief print ipv6 hop by hop extention header
 * @param opt header pointer
 */
void ipv6_hop_opt_print(struct ipv6_opt_hdr *opt);

/**
 * @brief print ipv6 frag extention header
 * @param frag header pointer
 */
void ipv6_frag_opt_print(struct frag_hdr *frag);

/**
 * @brief print ipv6 header
 * @param ip6 header pointer
 */
void ipv6_print(struct ipv6hdr *ip6);

/**
 * @brief print ipv4 header
 * @param ip header pointer
 */
void ipv4_print(struct iphdr *ip);

/**
 * @brief print pppoe header
 * @param pppoe header pointer
 */
void pppoe_print(struct pppoe_hdr *pppoe);

/**
 * @brief print vlan header
 * @param vlan header pointer
 */
void vlan_print(struct vlan_hdr *vlan);

/**
 * @brief print eth header
 * @param eth header pointer
 */
void eth_print(struct ethhdr *eth);

#endif /* _PKTPRS_H_ */
