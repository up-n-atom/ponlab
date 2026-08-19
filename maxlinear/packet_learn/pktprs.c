/*
 * Copyright (C) 2020-2025 MaxLinear, Inc.
 * Copyright (C) 2020 Intel Corporation
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
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: Packet header parsing
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/bitops.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/atomic.h>
#include <linux/notifier.h>
#include <linux/netdevice.h>
#include <linux/inetdevice.h>
#include <linux/in.h>
#include <linux/skbuff.h>
#include <soc/mxl/mxl_skb_ext.h>
#include <linux/netfilter.h>
#include <linux/pktprs.h>
#ifdef CONFIG_XFRM
#include <crypto/aead.h>
#endif
#if IS_ENABLED(CONFIG_L2TP)
#include <../net/l2tp/l2tp_core.h>
#endif
#include <net/netevent.h>

#include "pktprs_internal.h"

#define MOD_NAME "packet parser"

/* special UDP destination ports */
#define UDP_DPORT_L2TP   1701
#define UDP_DPORT_ESP    4500
#define UDP_DPORT_GENEVE 6081
#define UDP_DPORT_VXLAN  4789

/* MPLS next hdr */
#define MPLS_MAC         0
#define MPLS_IP          4
#define MPLS_IP6         6

#define PARSE_PROTOCOL_TYPE_GET(proto, dir)	\
({						\
	int type_ret;				\
	switch (ntohs(proto)) {			\
	case ETH_P_IP:				\
		type_ret = PKTPRS_IPV4_##dir;	\
		break;				\
	case ETH_P_IPV6:			\
		type_ret = PKTPRS_IPV6_##dir;	\
		break;				\
	default:				\
		type_ret = PKTPRS_TYPE_MAX;	\
		break;				\
	}					\
	type_ret;				\
})

/**
 * @struct pktprs_db
 * @brief packet parser database
 */
struct pktprs_db {
	struct list_head dev_list;
	spinlock_t lock;
	u32 skb_ext_id;
	atomic_t rx_users;
	atomic_t tx_users;
	atomic_t rxtx_users;
	atomic_t drop_users;
	struct parse_stats stats;
};

/**
 * @struct pktprs_dev
 * @brief packet parser device
 */
struct pktprs_dev {
	struct net_device *dev;
	struct list_head link;
	struct packet_type ptype;
};

/**
 * @struct parse_info
 * @brief packet parser information
 */
struct parse_info {
	struct pktprs_hdr *hdr;
	enum pktprs_hdr_level lvl;
	u8 proto;
	u8 buf_off;
	u8 hdr_sz;
	u16 next_proto;
	u8 *prev_nxt;
	struct sk_buff *skb;
};

#define CURRENT_HDR(p) (&(p)->hdr->buf[(p)->buf_off])

static struct pktprs_db *db;
static RAW_NOTIFIER_HEAD(parse_done_chain);
static void parse_transport_hdr(struct parse_info *p);
static void parse_network_hdr(struct parse_info *p);

/**
 * @brief allocate pktprs header memory on the SKB, use for RX
 * @return struct pktprs_hdr * allocated header (NULL if failed)
 */
static inline struct pktprs_hdr *rx_hdr_skb_add(struct sk_buff *skb)
{
	struct pktprs_hdr *h = mxl_skb_ext_add(skb, db->skb_ext_id);

	if (!h) {
		atomic_inc(&db->stats.hdr_alloc_failed);
		pr_debug("hdr allocation failed\n");
		return h;
	}
	memset(h, 0, sizeof(*h));

	return h;
}


/**
 * @brief remove the header information from skb
 * @param skb packet skb
 */
static inline void hdr_skb_remove(struct sk_buff *skb)
{
	mxl_skb_ext_del(skb, db->skb_ext_id);
}

struct pktprs_hdr *pktprs_skb_hdr_get(struct sk_buff *skb)
{
	return (struct pktprs_hdr *)mxl_skb_ext_find(skb, db->skb_ext_id);
}
EXPORT_SYMBOL(pktprs_skb_hdr_get);

/**
 * @brief increment the parse info level
 * @param p parse info
 * @return int 0 for success
 */
static int lvl_inc(struct parse_info *p)
{
	if (p->lvl >= PKTPRS_HDR_LEVEL_LAST) {
		pr_debug("level overflow\n");
		atomic_inc(&db->stats.level_ovflw);
		return -EINVAL;
	}

	p->lvl++;
	return 0;
}

/**
 * @brief add a protocol to the parser header
 * @param p parser information
 * @return int 0 for success
 */
static int proto_add(struct parse_info *p)
{
	if (p->buf_off + p->hdr_sz > p->hdr->buf_sz) {
		atomic_inc(&db->stats.hdr_buf_ovflw);
		p->next_proto = 0;
		return -ENOMEM;
	}
	if (p->hdr_sz == 0) {
		atomic_inc(&db->stats.error_pkt);
		p->next_proto = 0;
		return -EINVAL;
	}
	if (p->prev_nxt)
		*p->prev_nxt = p->proto; /* set the preliminary next protocol */
	p->prev_nxt = &p->hdr->proto_info[p->proto][p->lvl].nxt;
	set_bit(p->proto, &p->hdr->proto_bmap[p->lvl]);
	p->hdr->proto_info[p->proto][p->lvl].off = p->buf_off;
	p->buf_off += p->hdr_sz;
	atomic_inc(&db->stats.proto[p->proto]);
	/* currently we set it as payload,
	 * the next proto (if exist) will update
	 */
	*p->prev_nxt = PKTPRS_PROTO_PAYLOAD;
	p->hdr->proto_info[PKTPRS_PROTO_PAYLOAD][p->lvl].off = p->buf_off;

	return 0;
}

/**
 * @brief parse ethernet protocol
 * @param p parser information
 */
static void parse_eth(struct parse_info *p)
{
	struct ethhdr *hdr = (struct ethhdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_MAC;
	p->hdr_sz = sizeof(*hdr);
	p->next_proto = ntohs(hdr->h_proto);
	if (proto_add(p))
		return;
	parse_network_hdr(p);
}

/**
 * @brief parse vlan protocol
 * @param p parser information
 */
static void parse_vlan(struct parse_info *p)
{
	u8 i;
	struct vlan_hdr *hdr = (struct vlan_hdr *)CURRENT_HDR(p);

	for (i = 0; i < MAX_VLAN_HDRS_SUPPORTED; i++) {
		if (!PKTPRS_IS_VLAN(p->hdr, p->lvl, i))
			break;
	}
	if (i == MAX_VLAN_HDRS_SUPPORTED) {
		atomic_inc(&db->stats.vlan_ovflw);
		return;
	}

	p->proto = PKTPRS_PROTO_VLAN0 + i;
	p->hdr_sz = sizeof(*hdr);
	p->next_proto = ntohs(hdr->h_vlan_encapsulated_proto);
	if (proto_add(p))
		return;
	parse_network_hdr(p);
}

/**
 * @brief parse ipv6 protocol
 * @param p parser information
 */
static void parse_ip6(struct parse_info *p)
{
	struct ipv6hdr *hdr = (struct ipv6hdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_IPV6;
	p->hdr_sz = sizeof(*hdr);
	p->next_proto = hdr->nexthdr;
	if (proto_add(p))
		return;
	parse_transport_hdr(p);
}

/**
 * @brief parse ipv4 protocol
 * @param p parser information
 */
static void parse_ip(struct parse_info *p)
{
	struct iphdr *hdr = (struct iphdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_IPV4;
	p->hdr_sz = hdr->ihl << 2;
	p->next_proto = hdr->protocol;
	if (proto_add(p))
		return;
	parse_transport_hdr(p);
}

/**
 * @brief parse mpls next header protocol
 * @param p parser information
 */
static void parse_mpls_next_hdr(struct parse_info *p)
{
	u8 *hdr = (u8 *)CURRENT_HDR(p);

	switch (*hdr >> 4) {
	case MPLS_IP:
		parse_ip(p);
		break;
	case MPLS_IP6:
		parse_ip6(p);
		break;
	case MPLS_MAC:
		if (lvl_inc(p))
			return;
		/* skip PW eth CW */
		p->buf_off += 4;
		parse_eth(p);
		break;
	default:
		atomic_inc(&db->stats.unsupp_proto);
		break;
	}
}

/**
 * @brief parse mpls protocol
 * @param p parser information
 */
static void parse_mpls(struct parse_info *p)
{
	u8 i;
	struct mpls_shim_hdr *hdr = (struct mpls_shim_hdr *)CURRENT_HDR(p);

	for (i = 0; i < MAX_MPLS_HDRS_SUPPORTED; i++) {
		if (!PKTPRS_IS_MPLS(p->hdr, p->lvl, i))
			break;
	}
	if (i == MAX_MPLS_HDRS_SUPPORTED) {
		atomic_inc(&db->stats.mpls_ovflw);
		return;
	}

	p->proto = PKTPRS_PROTO_MPLS0 + i;
	p->hdr_sz = MPLS_HLEN;

	if(proto_add(p))
		return;

	if (ntohl(hdr->label_stack_entry) & BIT(MPLS_LS_S_SHIFT))
		parse_mpls_next_hdr(p);
	else {
		p->next_proto = ETH_P_MPLS_UC;
		parse_network_hdr(p);
	}
}

#if IS_ENABLED(CONFIG_L2TP)
/**
 * @brief parse ppp protocol
 * @param p parser information
 */
static void parse_ppp(struct parse_info *p)
{
	unsigned char *hdr = (unsigned char *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_PPP;
	p->hdr_sz = PPP_HDRLEN;
	p->next_proto = PPP_PROTOCOL(hdr);

	if (proto_add(p))
		return;

	switch (p->next_proto) {
	case PPP_IP:
		if (lvl_inc(p))
			return;
		parse_ip(p);
		break;
	case PPP_IPV6:
		if (lvl_inc(p))
			return;
		parse_ip6(p);
		break;
	default:
		/* payload */
		atomic_inc(&db->stats.unsupp_proto);
		break;
	}
}

/**
 * @brief parse l2tp next header protocol
 * @param p parser information
 */
static void parse_l2tp_next_hdr(struct parse_info *p)
{
	if (proto_add(p))
		return;

	switch (p->next_proto) {
	case L2TP_PWTYPE_ETH_VLAN:
	case L2TP_PWTYPE_ETH:
		if (lvl_inc(p))
			return;
		parse_eth(p);
		break;
	case L2TP_PWTYPE_PPP:
		parse_ppp(p);
		break;
	case L2TP_PWTYPE_IP:
		if (lvl_inc(p))
			return;
		parse_ip(p);
		break;
	case L2TP_PWTYPE_NONE:
		break;
	default:
		atomic_inc(&db->stats.unsupp_proto);
		break;
	}
}

/**
 * @brief find the L2TP tunnel by the device pointer and tunnel ID
 * @note caller is rensponsible to call l2tp_tunnel_dec_refcount later
 * @param dev device
 * @param tid tunnel ID from packet header
 * @return struct l2tp_tunnel * l2tp tunnel pointer, NULL if not found
 */
static struct l2tp_tunnel *pktprs_l2tp_tunnel_find(struct net_device *dev,
						   u32 tid)
{
	struct l2tp_tunnel *tunnel = NULL;
	u32 i;

	pr_debug("search l2tp tunnel by id %u\n", tid);

	if (!dev)
		return NULL;

	for (i = 0;;) {
		tunnel = l2tp_tunnel_get_nth(dev_net(dev), i++);
		if (!tunnel)
			break;
		/* check if match to the tunnel id - l2tp rx packet */
		if (tunnel->tunnel_id == tid) {
			pr_debug("l2tp tunnel %s: %u found (local)\n",
				 tunnel->name, tid);
			break;
		}
		/* check if match to the peer tunnel id - l2tp tx packet */
		if (tunnel->peer_tunnel_id == tid) {
			pr_debug("l2tp tunnel %s: %u found (peer)\n",
				 tunnel->name, tid);
			break;
		}
		l2tp_tunnel_dec_refcount(tunnel);
	}

	return tunnel;
}

/**
 * @brief find the L2TP session by the tunnel pointer and session ID
 * @note caller is rensponsible to call l2tp_session_dec_refcount later
 * @param tunnel l2tp tunnel
 * @param sid session ID from packet header
 * @return struct l2tp_session * l2tp session pointer, NULL if not found
 */
static struct l2tp_session *pktprs_l2tp_session_find(struct l2tp_tunnel *tunnel,
						     u32 sid)
{
	struct l2tp_session *session = NULL;
	u32 i;

	pr_debug("search l2tp session by id %u\n", sid);

	if (!tunnel)
		return NULL;

	/* search using peer tunnel id, usually for tx packets */
	for (i = 0;;) {
		session = l2tp_session_get_nth(tunnel, i++);
		if (!session)
			break;
		if (session->session_id == sid) {
			pr_debug("l2tp session %s: %u found (local)\n",
				 session->name, sid);
			break;
		}
		if (session->peer_session_id == sid) {
			pr_debug("l2tp session %s: %u found (peer)\n",
				 session->name, sid);
			break;
		}
		l2tp_session_dec_refcount(session);
	}

	return session;
}

/**
 * @brief get the L2TPv3 session by the session ID
 * @note caller is rensponsible to call l2tp_session_dec_refcount later
 * @param dev device
 * @param sid session ID from packet header
 * @return struct l2tp_session * l2tp session pointer, NULL if not found
 */
static struct l2tp_session *pktprs_l2tpv3_session_get(struct net_device *dev,
						      u32 sid)
{
	struct l2tp_session *session = NULL;
	struct l2tp_tunnel *tunnel;
	u32 t = 0;

	if (!dev)
		return NULL;

	/* for rx, the session id is local, try to find in local l2tp db */
	session = l2tp_session_get(dev_net(dev), sid);
	if (session) {
		pr_debug("l2tp session %s: %u found (local)\n",
				 session->name, sid);
		return session;
	}

	/* for tx, the session id might be found as peer session id
	 * go over all tunnels and search this session as peer session
	 * for l2tp over ip session id is globally unique
	 */
	for (t = 0;;) {
		tunnel = l2tp_tunnel_get_nth(dev_net(dev), t++);
		if (!tunnel)
			break;
		session = pktprs_l2tp_session_find(tunnel, sid);
		l2tp_tunnel_dec_refcount(tunnel);
		if (session) {
			pr_debug("l2tp session %s: %u found (peer)\n",
				 session->name, sid);
			return session;
		}
	}

	return NULL;
}

/**
 * @brief get the L2TPv2 session by the tunnel ID and session ID
 * @note caller is rensponsible to call l2tp_session_dec_refcount later
 * @param dev device
 * @param tid tunnel ID from packet header
 * @param sid session ID from packet header
 * @return struct l2tp_session * l2tp session pointer, NULL if not found
 */
static struct l2tp_session *pktprs_l2tpv2_session_get(struct net_device *dev,
						      u32 tid, u32 sid)
{
	struct l2tp_session *session;
	struct l2tp_tunnel *tunnel;

	/* in L2TPv2, the tunnel id is available
	 * from header, find by the tunnel id
	 */
	tunnel = pktprs_l2tp_tunnel_find(dev, tid);
	if (!tunnel)
		return NULL;

	/* tunnel found, now search the session id */
	session = pktprs_l2tp_session_find(tunnel, sid);
	/* release the tunnel refcount */
	l2tp_tunnel_dec_refcount(tunnel);

	return session;
}
#endif

/**
 * @brief parse l2tp over udp protocol
 * @param p parser information
 */
static void parse_l2tp_oudp(struct parse_info *p)
{
#if IS_ENABLED(CONFIG_L2TP)
	u8 ver;
	u32 sid, tid = 0;
	struct l2tp_session *session = NULL;
	union l2tphdr *l2tp = (union l2tphdr *)CURRENT_HDR(p);
	unsigned char *next_hdr;

	ver = ntohs(l2tp->ctr.flags) & L2TP_HDR_VER_MASK;
	if (ver != L2TP_HDR_VER_2 && ver != L2TP_HDR_VER_3)
		return; /* unknown */

	p->hdr_sz = 0;
	p->proto = PKTPRS_PROTO_L2TP_OUDP;
	if (ntohs(l2tp->ctr.flags) & L2TP_HDRFLAG_T) {
		/* control packet */
		p->hdr_sz = ntohs(l2tp->ctr.length);
		proto_add(p);
		return;
	}

	/* data packet - get session id */
	if (ver == L2TP_HDR_VER_2) {
		if (ntohs(l2tp->v2.flags) & L2TP_HDRFLAG_L) {
			sid = ntohs(l2tp->v2_len.sess_id);
			tid = ntohs(l2tp->v2_len.tunnel);
			p->hdr_sz += sizeof(l2tp->v2_len.length);
		} else {
			sid = ntohs(l2tp->v2.sess_id);
			tid = ntohs(l2tp->v2.tunnel);
		}
		session = pktprs_l2tpv2_session_get(p->skb->dev, tid, sid);
	} else {
		sid = ntohl(l2tp->v3_oudp.sess_id);
		session = pktprs_l2tpv3_session_get(p->skb->dev, sid);
	}

	if (session) {
		p->hdr_sz += session->hdr_len;
		p->next_proto = session->pwtype;
		l2tp_session_dec_refcount(session);
	} else {
		/* ppp over l2tpv2 bypass */
		if (ver != L2TP_HDR_VER_2) {
			pr_debug("l2tpv3 session %d not found\n", sid);
			return;
		}

		if (!p->hdr_sz)
			p->hdr_sz = sizeof(struct l2tp_v2_hdr);

		next_hdr = (unsigned char *)l2tp + p->hdr_sz;
		if (next_hdr[0] != PPP_ALLSTATIONS || next_hdr[1] != PPP_UI) {
			pr_debug("ppp over l2tpv2 not found\n");
			return;
		}
		p->next_proto = L2TP_PWTYPE_PPP;
	}

	parse_l2tp_next_hdr(p);
#endif
}

/**
 * @brief parse l2tp over ip protocol
 * @param p parser information
 */
static void parse_l2tp_oip(struct parse_info *p)
{
#if IS_ENABLED(CONFIG_L2TP)
	struct l2tp_session *session;
	union l2tphdr *l2tp = (union l2tphdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_L2TP_OIP;
	/* control packet */
	if (!l2tp->v3_oip.sess_id) {
		p->hdr_sz = l2tp->v3_oip.ctr.length;
		proto_add(p);
		return;
	}

	/* data packet */
	session = pktprs_l2tpv3_session_get(p->skb->dev,
					    ntohl(l2tp->v3_oip.sess_id));
	if (!session) {
		pr_debug("l2tp session %d not found\n",
			 ntohl(l2tp->v3_oip.sess_id));
		return;
	}

	p->hdr_sz = session->hdr_len;
	p->next_proto = session->pwtype;
	l2tp_session_dec_refcount(session);
	parse_l2tp_next_hdr(p);
#endif
}

/**
 * @brief parse pppoe protocol
 * @param p parser information
 */
static void parse_pppoe(struct parse_info *p)
{
	struct pppoe_hdr *hdr = (struct pppoe_hdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_PPPOE;
	p->hdr_sz = PPPOE_SES_HLEN;
	p->next_proto = ntohs(*(__be16 *)hdr->tag);
	if (hdr->ver != 1 || hdr->type != 1) {
		atomic_inc(&db->stats.unsupp_proto);
		return;
	}

	if (proto_add(p))
		return;

	switch (p->next_proto) {
	case PPP_IP:
		parse_ip(p);
		break;
	case PPP_IPV6:
		parse_ip6(p);
		break;
	default:
		/* payload */
		atomic_inc(&db->stats.unsupp_proto);
		break;
	}
}

/**
 * @brief parse ipv6 hop extention protocol
 * @param p parser information
 */
static void parse_hop_opt(struct parse_info *p)
{
	struct ipv6_opt_hdr *hdr = (struct ipv6_opt_hdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_HOP_OPT;
	p->hdr_sz = ipv6_optlen(hdr);
	p->next_proto = hdr->nexthdr;
	if (proto_add(p))
		return;
	parse_transport_hdr(p);
}

/**
 * @brief parse ipv6 dest extention protocol
 * @param p parser information
 */
static void parse_dest_opt(struct parse_info *p)
{
	struct ipv6_opt_hdr *hdr = (struct ipv6_opt_hdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_DEST_OPT;
	p->hdr_sz = ipv6_optlen(hdr);
	p->next_proto = hdr->nexthdr;
	if (proto_add(p))
		return;
	parse_transport_hdr(p);
}

/**
 * @brief parse ipv6 rout extention protocol
 * @param p parser information
 */
static void parse_rout_opt(struct parse_info *p)
{
	struct ipv6_rt_hdr *hdr = (struct ipv6_rt_hdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_ROUT_OPT;
	p->hdr_sz = ipv6_optlen(hdr);
	p->next_proto = hdr->nexthdr;
	if (proto_add(p))
		return;
	parse_transport_hdr(p);
}

/**
 * @brief parse ipv6 frag extention protocol
 * @param p parser information
 */
static void parse_frag_opt(struct parse_info *p)
{
	struct frag_hdr *hdr = (struct frag_hdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_FRAG_OPT;
	p->hdr_sz = sizeof(*hdr);
	p->next_proto = hdr->nexthdr;
	if (proto_add(p))
		return;
	parse_transport_hdr(p);
}

/**
 * @brief parse auth protocol
 * @param p parser information
 */
static void parse_auth(struct parse_info *p)
{
	struct ip_auth_hdr *hdr = (struct ip_auth_hdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_AUTH;
	p->hdr_sz = ipv6_authlen(hdr);
	p->next_proto = hdr->nexthdr;
	if (proto_add(p))
		return;
	parse_transport_hdr(p);
}

/**
 * @brief parse esp protocol
 * @param p parser information
 */
static void parse_esp(struct parse_info *p)
{
	struct ip_esp_hdr *hdr = (struct ip_esp_hdr *)CURRENT_HDR(p);
#ifdef CONFIG_XFRM
	struct xfrm_state   *xs;
	struct xfrm_offload *xo;
#endif
	p->proto = PKTPRS_PROTO_ESP;
	p->hdr_sz = sizeof(*hdr);
	p->next_proto = 0;

#ifdef CONFIG_XFRM
	/* check if sec_path is available*/
	if (secpath_exists(p->skb)) {
		/* if state is available add the ivsize */
		xs = xfrm_input_state(p->skb);
		if (xs)
			p->hdr_sz += crypto_aead_ivsize(xs->data);
		/* if offload is available set next protocol */
		xo = xfrm_offload(p->skb);
		if (xo)
			p->next_proto = xo->proto;
	}
#endif
	if (proto_add(p))
		return;

	if (p->next_proto)
		parse_transport_hdr(p);
}

/**
 * @brief parse geneve protocol
 * @param p parser information
 */
static void parse_geneve(struct parse_info *p)
{
	struct genevehdr *hdr = (struct genevehdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_GENEVE;
	p->hdr_sz = sizeof(*hdr);
	proto_add(p);
}

/**
 * @brief parse vxlan protocol
 * @param p parser information
 */
static void parse_vxlan(struct parse_info *p)
{
	struct vxlanhdr *hdr = (struct vxlanhdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_VXLAN;
	p->hdr_sz = sizeof(*hdr);
	proto_add(p);

	if (lvl_inc(p))
		return;

	parse_eth(p);
}

/**
 * @brief parse udp protocol
 * @param p parser information
 */
static void parse_udp(struct parse_info *p)
{
	struct udphdr *hdr = (struct udphdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_UDP;
	p->hdr_sz = sizeof(*hdr);
	p->next_proto = ntohs(hdr->dest);
	if (proto_add(p))
		return;

	switch (p->next_proto) {
	case UDP_DPORT_L2TP:
		parse_l2tp_oudp(p);
		break;
	case UDP_DPORT_ESP:
		if (lvl_inc(p))
			return;
		parse_esp(p);
		break;
	case UDP_DPORT_GENEVE:
		parse_geneve(p);
		break;
	case UDP_DPORT_VXLAN:
		parse_vxlan(p);
		break;
	default:
		break;
	}
}

/**
 * @brief parse tcp protocol
 * @param p parser information
 */
static void parse_tcp(struct parse_info *p)
{
	struct tcphdr *hdr = (struct tcphdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_TCP;
	p->hdr_sz = hdr->doff << 2;
	proto_add(p);
}

/**
 * @brief parse icmp protocol
 * @param p parser information
 */
static void parse_icmp(struct parse_info *p)
{
	struct icmphdr *hdr = (struct icmphdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_ICMP;
	p->hdr_sz = sizeof(*hdr);
	proto_add(p);
}

/**
 * @brief parse icmp6 protocol
 * @param p parser information
 */
static void parse_icmp6(struct parse_info *p)
{
	struct icmp6hdr *hdr = (struct icmp6hdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_ICMP6;
	p->hdr_sz = sizeof(*hdr);
	proto_add(p);
}

/**
 * @brief parse sctp protocol
 * @param p parser information
 */
static void parse_sctp(struct parse_info *p)
{
	struct sctphdr *hdr = (struct sctphdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_SCTP;
	p->hdr_sz = sizeof(*hdr);
	proto_add(p);
}

/**
 * @brief parse igmp protocol
 * @param p parser information
 */
static void parse_igmp(struct parse_info *p)
{
	struct igmphdr *hdr = (struct igmphdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_IGMP;
	p->hdr_sz = sizeof(*hdr);
	proto_add(p);
}

/**
 * @brief parse gre protocol
 * @param p parser information
 */
static void parse_gre(struct parse_info *p)
{
	struct gre_base_hdr *hdr = (struct gre_base_hdr *)CURRENT_HDR(p);

	p->proto = PKTPRS_PROTO_GRE;
	p->hdr_sz = sizeof(*hdr);
	p->next_proto = ntohs(hdr->protocol);

	if (hdr->flags & GRE_CSUM)
		p->hdr_sz += 4;
	if (hdr->flags & GRE_KEY)
		p->hdr_sz += 4;
	if (hdr->flags & GRE_SEQ)
		p->hdr_sz += 4;

	if (proto_add(p))
		return;

	if (lvl_inc(p))
		return;
	parse_network_hdr(p);
}

/**
 * @brief parse network protocol
 * @param p parser information
 */
static void parse_network_hdr(struct parse_info *p)
{
	switch (p->next_proto) {
	case ETH_P_TEB:
		parse_eth(p);
		break;
	case ETH_P_8021Q:
	case ETH_P_8021AD:
		parse_vlan(p);
		break;
	case ETH_P_PPP_SES:
		parse_pppoe(p);
		break;
	case ETH_P_MPLS_UC:
		parse_mpls(p);
		break;
	case ETH_P_IP:
		parse_ip(p);
		break;
	case ETH_P_IPV6:
		parse_ip6(p);
		break;
	default:
		/* payload */
		atomic_inc(&db->stats.unsupp_proto);
		break;
	}
}

/**
 * @brief parse transport protocol
 * @param p parser information
 */
static void parse_transport_hdr(struct parse_info *p)
{
	switch (p->next_proto) {
	case IPPROTO_UDP:
		parse_udp(p);
		break;
	case IPPROTO_TCP:
		parse_tcp(p);
		break;
	case IPPROTO_ICMP:
		parse_icmp(p);
		break;
	case IPPROTO_ICMPV6:
		parse_icmp6(p);
		break;
	case IPPROTO_ESP:
		parse_esp(p);
		break;
	case IPPROTO_AH:
		parse_auth(p);
		break;
	case IPPROTO_IPIP:
		if (lvl_inc(p))
			return;
		parse_ip(p);
		break;
	case IPPROTO_IPV6:
		if (lvl_inc(p))
			return;
		parse_ip6(p);
		break;
	case IPPROTO_GRE:
		parse_gre(p);
		break;
	case IPPROTO_SCTP:
		parse_sctp(p);
		break;
	case IPPROTO_IGMP:
		parse_igmp(p);
		break;
	case IPPROTO_L2TP:
		parse_l2tp_oip(p);
		break;
	/* IPV6 NextHeader */
	case NEXTHDR_HOP:
		parse_hop_opt(p);
		break;
	case NEXTHDR_DEST:
		parse_dest_opt(p);
		break;
	case NEXTHDR_ROUTING:
		parse_rout_opt(p);
		break;
	case NEXTHDR_FRAGMENT:
		parse_frag_opt(p);
		break;
	case NEXTHDR_NONE:
		break;
	default:
		atomic_inc(&db->stats.unsupp_proto);
		break;
	}
}

/**
 * @brief copy the packet header start from ethernet header
 * @param skb packet skb
 * @param h header information
 */
static void hdr_copy_from_eth(struct sk_buff *skb, struct pktprs_hdr *h)
{
	struct vlan_ethhdr *veth;
	u16 hdr_sz = skb_end_pointer(skb) - skb_mac_header(skb);

	if (skb_vlan_tag_present(skb)) {
		h->buf_sz = min_t(u16, hdr_sz + VLAN_HLEN, PKTPRS_HDR_BUF_SIZE);
		/* copy the header to off VLAN_HLEN */
		memcpy(&h->buf[VLAN_HLEN], eth_hdr(skb), h->buf_sz - VLAN_HLEN);
		/* move the mac address to the buffer head */
		memmove(h->buf, &h->buf[VLAN_HLEN], ETH_ALEN * 2);
		/* restore the vlan header from the skb */
		veth = (struct vlan_ethhdr *)h->buf;
		veth->h_vlan_proto = skb->vlan_proto;
		veth->h_vlan_TCI = htons(skb_vlan_tag_get(skb));
	} else {
		h->buf_sz = min_t(u16, hdr_sz, PKTPRS_HDR_BUF_SIZE);
		memcpy(h->buf, skb_mac_header(skb), h->buf_sz);
	}
}

/**
 * @brief copy the packet header start from ipv4 header
 * @param skb packet skb
 * @param h header information
 */
static void hdr_copy_from_ipv4(struct sk_buff *skb, struct pktprs_hdr *h)
{
	u16 hdr_sz = skb_end_pointer(skb) - (unsigned char *)ip_hdr(skb);

	h->buf_sz = min_t(u16, hdr_sz, PKTPRS_HDR_BUF_SIZE);
	memcpy(h->buf, ip_hdr(skb), h->buf_sz);
}

/**
 * @brief copy the packet header start from ipv6 header
 * @param skb packet skb
 * @param h header information
 */
static void hdr_copy_from_ipv6(struct sk_buff *skb, struct pktprs_hdr *h)
{
	u16 hdr_sz = skb_end_pointer(skb) - (unsigned char *)ipv6_hdr(skb);

	h->buf_sz = min_t(u16, hdr_sz, PKTPRS_HDR_BUF_SIZE);
	memcpy(h->buf, ipv6_hdr(skb), h->buf_sz);
}

/**
 * @brief parse the header
 * @param skb packet skb
 * @param dev net device
 * @param h header information
 * @param prs_type parsing type
 */
static void hdr_parse(struct sk_buff *skb, struct net_device *dev,
		      struct pktprs_hdr *h, enum pktprs_type prs_type)
{
	struct parse_info p;

	memset(&p, 0, sizeof(p));
	p.hdr = h;
	p.lvl = PKTPRS_HDR_LEVEL0;
	p.skb = skb;
	if (dev)
		h->ifindex = dev->ifindex;

	/* Parse the skb */
	switch (prs_type) {
	case PKTPRS_ETH_RX:
	case PKTPRS_ETH_TX:
		hdr_copy_from_eth(skb, h);
		parse_eth(&p);
		break;
	case PKTPRS_IPV4_RX:
	case PKTPRS_IPV4_TX:
		hdr_copy_from_ipv4(skb, h);
		parse_ip(&p);
		break;
	case PKTPRS_IPV6_RX:
	case PKTPRS_IPV6_TX:
		hdr_copy_from_ipv6(skb, h);
		parse_ip6(&p);
		break;
	default:
		pr_debug("Invalid parse type %u\n", prs_type);
		break;
	}
	set_bit(PKTPRS_PROTO_PAYLOAD, &h->proto_bmap[p.lvl]);
	h->buf_sz = p.buf_off;
}

/**
 * @brief notify the parsing information
 * @param desc parsing descriptor
 * @param prs_type parsing type
 */
static void desc_notify(struct pktprs_desc *desc, enum pktprs_type prs_type)
{
	/* Send notification */
	switch (prs_type) {
	case PKTPRS_ETH_RX:
	case PKTPRS_IPV4_RX:
	case PKTPRS_IPV6_RX:
		if (atomic_read(&db->rx_users)) {
			raw_notifier_call_chain(&parse_done_chain, PKTPRS_RX,
						desc);
		}
		if (!atomic_read(&db->rxtx_users) &&
		    !atomic_read(&db->drop_users)) {
			hdr_skb_remove(desc->skb);
			break;
		}
		break;
	case PKTPRS_ETH_TX:
	case PKTPRS_IPV4_TX:
	case PKTPRS_IPV6_TX:
		if (atomic_read(&db->tx_users)) {
			raw_notifier_call_chain(&parse_done_chain, PKTPRS_TX,
						desc);
		}
		if (atomic_read(&db->rxtx_users) && desc->rx) {
			raw_notifier_call_chain(&parse_done_chain, PKTPRS_RXTX,
						desc);
		}
		desc->tx = NULL;
		break;
	default:
		pr_debug("Invalid parse type %u\n", prs_type);
		break;
	}
}

/**
 * @brief get the parsing type
 * @param skb packet skb
 * @param ptype packet type
 * @return int parsing type
 */
static int parse_type_get(struct sk_buff *skb, struct packet_type *ptype)
{
	if (!ptype)
		return skb->pkt_type; /* User/Local packet */

	/* dev PCAP packet */
	if (skb->dev && !dev_is_mac_header_xmit(skb->dev)) {
		if (skb->pkt_type == PACKET_OUTGOING)
			return PARSE_PROTOCOL_TYPE_GET(skb->protocol, TX);

		return PARSE_PROTOCOL_TYPE_GET(skb->protocol, RX);
	}

	if (skb->pkt_type == PACKET_OUTGOING)
		return PKTPRS_ETH_TX;

	return PKTPRS_ETH_RX;
}

/**
 * @brief prepare for packet parsing
 * @param skb packet skb
 * @param prs_type parsing type
 * @param desc parsing descriptor
 * @param tx_hdr ptr to hdr for TX
 * @return int parsing type
 */
static int parse_prepare(struct sk_buff *skb, enum pktprs_type prs_type,
			 struct pktprs_desc *desc, struct pktprs_hdr *tx_hdr)
{
	desc->skb = skb;
	desc->rx = NULL;
	desc->tx = NULL;

	switch (prs_type) {
	case PKTPRS_ETH_RX:
		if (skb_is_gso(skb))
			return -EPERM;
		/* fallthrough */
	case PKTPRS_IPV4_RX:
	case PKTPRS_IPV6_RX:
		/* if no users for RX - skip */
		if (!atomic_read(&db->rxtx_users) &&
		    !atomic_read(&db->rx_users)   &&
		    !atomic_read(&db->drop_users))
			return -EPERM;
		desc->rx = pktprs_skb_hdr_get(skb);
		if (desc->rx) {
			pr_debug("rx ext already exist\n");
			return -EPERM;
		}
		atomic_inc(&db->stats.rx_hook);
		/* allocating hdr extention on the SKB */
		desc->rx = rx_hdr_skb_add(skb);
		if (!desc->rx)
			return -EPERM;
		break;
	case PKTPRS_ETH_TX:
	case PKTPRS_IPV4_TX:
	case PKTPRS_IPV6_TX:
		/* if no users for TX - skip */
		if (!atomic_read(&db->tx_users) &&
		    !atomic_read(&db->rxtx_users))
			return -EPERM;
		/* for RXTX users, need to fetch the RX hdr */
		if (atomic_read(&db->rxtx_users)) {
			desc->rx = pktprs_skb_hdr_get(skb);
			if (!desc->rx) {
				pr_debug("no ext\n");
				if (!atomic_read(&db->tx_users))
					return -EPERM;
			}
		}
		atomic_inc(&db->stats.tx_hook);
		/* case of TX: using the local hdr from 'parse_skb' function */
		desc->tx = tx_hdr;
		break;
	default:
		return -EPERM;
	}

	return 0;
}

static void desc_parse(struct net_device *dev, struct pktprs_desc *desc,
		       enum pktprs_type prs_type)
{
	switch (prs_type) {
	case PKTPRS_ETH_RX:
	case PKTPRS_IPV4_RX:
	case PKTPRS_IPV6_RX:
		hdr_parse(desc->skb, dev, desc->rx, prs_type);
		break;
	case PKTPRS_ETH_TX:
	case PKTPRS_IPV4_TX:
	case PKTPRS_IPV6_TX:
		hdr_parse(desc->skb, dev, desc->tx, prs_type);
		break;
	default:
		pr_debug("Invalid parse type %u\n", prs_type);
		break;
	}
}

/**
 * @brief parse skb packet
 * @note this is the dev pack callback
 * @param skb packet skb
 * @param dev net device
 * @param ptype packet type
 * @param orig_dev original net device
 * @return int NET_RX_SUCCESS
 */
static int parse_skb(struct sk_buff *skb,
		     struct net_device *dev,
		     struct packet_type *ptype,
		     struct net_device *orig_dev)
{
	struct pktprs_hdr tx_hdr = { 0 };
	struct pktprs_desc desc;
	enum pktprs_type prs_type;

	/* get the parsing type */
	prs_type = parse_type_get(skb, ptype);

	/* prepare the parsing */
	if (parse_prepare(skb, prs_type, &desc, &tx_hdr))
		goto out;

	/* parse the header */
	desc_parse(dev, &desc, prs_type);

	/* notify results */
	desc_notify(&desc, prs_type);

out:
	/* consume skb for dev PCAP packets */
	if (ptype)
		consume_skb(skb);

	return NET_RX_SUCCESS;
}

u16 pktprs_ip6_proto_off(struct ipv6hdr *ip6)
{
	unsigned char *data = (unsigned char *) ip6;
	u8 nexthdr = ip6->nexthdr;
	u16 nexthdr_off = offsetof(struct ipv6hdr, nexthdr);
	u16 off = sizeof(*ip6);
	struct ipv6_opt_hdr *hdr;

	while (ipv6_ext_hdr(nexthdr) && nexthdr != NEXTHDR_NONE) {
		hdr = (struct ipv6_opt_hdr *)(data + off);
		if (nexthdr == NEXTHDR_FRAGMENT)
			off += sizeof(struct frag_hdr);
		else if (nexthdr == NEXTHDR_AUTH)
			off += ipv6_authlen(hdr);
		else
			off += ipv6_optlen(hdr);

		nexthdr_off = (char *)&hdr->nexthdr - (char *)ip6;
		nexthdr = hdr->nexthdr;
	}

	return nexthdr_off;
}
EXPORT_SYMBOL(pktprs_ip6_proto_off);

/**
 * @brief get the ipv6 next protocol type
 * @param skb skb pointer
 * @return __u8 ipv6 next protocol
 */
static __u8 ip6_proto_get(struct sk_buff *skb)
{
	u8 *nhdr_off = (u8 *)ipv6_hdr(skb) +
		pktprs_ip6_proto_off(ipv6_hdr(skb));

	return *nhdr_off;
}

/**
 * @brief handler to parse packets from local hooks
 * @param priv unused
 * @param skb skb pointer
 * @param state nf hook state
 * @return int 0 for success
 */
static unsigned int pktprs_local_handler(void *priv, struct sk_buff *skb,
					 const struct nf_hook_state *state)
{
	u8 orig_ptype = skb->pkt_type;
	u8 nexthdr;
	u16 dport;

	switch (state->pf) {
	case NFPROTO_IPV4:
		if (ipv4_is_multicast(ip_hdr(skb)->daddr))
			goto out;
		nexthdr = ip_hdr(skb)->protocol;
		skb->pkt_type = PKTPRS_IPV4_RX;
		break;
	case NFPROTO_IPV6:
		if (ipv6_addr_is_multicast(&ipv6_hdr(skb)->daddr))
			goto out;
		nexthdr = ip6_proto_get(skb);
		skb->pkt_type = PKTPRS_IPV6_RX;
		break;
	default:
		goto out;
	}

	switch (nexthdr) {
	case IPPROTO_IPIP:
	case IPPROTO_IPV6:
	case IPPROTO_ESP:
	case IPPROTO_GRE:
	case IPPROTO_L2TP:
		goto out;
	case IPPROTO_UDP:
		dport = ntohs(udp_hdr(skb)->dest);
		switch (dport) {
		case UDP_DPORT_L2TP:
		case UDP_DPORT_ESP:
		case UDP_DPORT_GENEVE:
		case UDP_DPORT_VXLAN:
			goto out;
		default:
			break;
		}
	default:
		break;
	}

	if (state->hook == NF_INET_LOCAL_IN)
		skb->pkt_type = PKTPRS_ETH_TX;

	parse_skb(skb, NULL, NULL, NULL);
	atomic_inc(&db->stats.local_hook);

out:
	skb->pkt_type = orig_ptype;
	return NF_ACCEPT;
}

static struct nf_hook_ops pktprs_ops[] __read_mostly = {
	{
		.hook     = pktprs_local_handler,
		.pf       = NFPROTO_IPV4,
		.hooknum  = NF_INET_LOCAL_IN,
		.priority = INT_MAX,
	},
	{
		.hook     = pktprs_local_handler,
		.pf       = NFPROTO_IPV6,
		.hooknum  = NF_INET_LOCAL_IN,
		.priority = INT_MAX,
	},
	{
		.hook     = pktprs_local_handler,
		.pf       = NFPROTO_IPV4,
		.hooknum  = NF_INET_LOCAL_OUT,
		.priority = INT_MIN,
	},
	{
		.hook     = pktprs_local_handler,
		.pf       = NFPROTO_IPV6,
		.hooknum  = NF_INET_LOCAL_OUT,
		.priority = INT_MIN,
	},
};

/**
 * @brief handler for netevents, use to trigger DROP event or to remove rx info
 * @param unused unused
 * @param event event type
 * @param ptr skb pointer
 * @return int NOTIFY_DONE
 */
static int netevent_handler(struct notifier_block *unused,
			    unsigned long event, void *ptr)
{
	struct pktprs_desc desc;
	struct ethhdr *eth;
	struct sk_buff *skb = (struct sk_buff *)ptr;

	if (!atomic_read(&db->rx_users) && !atomic_read(&db->rxtx_users) &&
	    !atomic_read(&db->drop_users))
		return NOTIFY_DONE;

	if (!skb)
		return NOTIFY_DONE;

	switch (event) {
	case NETEVENT_BR_FLOOD:
	case NETEVENT_NF_HOOK_SLOW_DROP:
	case NETEVENT_EBT_DO_TABLE_DROP:
	case NETEVENT_IPT_DO_TABLE_DROP:
	case NETEVENT_IP6T_DO_TABLE_DROP:
	case NETEVENT_NF_REINJECT_DROP:
		if (skb_mac_header_was_set(skb)) {
			eth = eth_hdr(skb);
			if (eth) {
				/* Check for multicast (not broadcast) */
				if (!is_broadcast_ether_addr(eth->h_dest) &&
					is_multicast_ether_addr(eth->h_dest))
					break;
				if (event == NETEVENT_BR_FLOOD) {
					hdr_skb_remove(skb);
					atomic_inc(&db->stats.free_skbext);
					break;
				}
			}
		}
		/* fallthrough */
	case NETEVENT_IPV4_MARTIAN_SRC:
	case NETEVENT_IPV4_MARTIAN_DST:
		if (!atomic_read(&db->drop_users))
			break;
		/* Restore SKB ext */
		desc.rx = pktprs_skb_hdr_get(skb);
		if (desc.rx) {
			desc.tx = NULL;
			desc.skb = skb;
			raw_notifier_call_chain(&parse_done_chain, PKTPRS_DROP,
						&desc);
			hdr_skb_remove(skb);
			atomic_inc(&db->stats.free_skbext);
		}
		break;
	}

	return NOTIFY_DONE;
}

static struct notifier_block netevent_nb = {
	.notifier_call = netevent_handler
};

int pktprs_stats_get(struct parse_stats *stats)
{
	if (!stats)
		return -EINVAL;

	spin_lock_bh(&db->lock);
	memcpy(stats, &db->stats, sizeof(*stats));
	spin_unlock_bh(&db->lock);
	return 0;
}

void pktprs_pr_dev(void)
{
	struct pktprs_dev *pdev;

	pr_info("registered devices:");
	spin_lock_bh(&db->lock);
	list_for_each_entry(pdev, &db->dev_list, link) {
		if (pdev->dev)
			pr_info("%s\n", pdev->dev->name);
	}
	spin_unlock_bh(&db->lock);
}

void pktprs_do_parse(struct sk_buff *skb, struct net_device *dev,
		     enum pktprs_type prs_from)
{
	u8 orig_ptype = skb->pkt_type;

	skb->pkt_type = prs_from;
	parse_skb(skb, dev, NULL, NULL);
	skb->pkt_type = orig_ptype;
	atomic_inc(&db->stats.custom_hook);
}
EXPORT_SYMBOL(pktprs_do_parse);

int pktprs_do_parse_direct(struct sk_buff *skb, struct net_device *dev,
			   struct pktprs_hdr *h, enum pktprs_type prs_from)
{
	if (!skb || !h)
		return -EINVAL;

	hdr_parse(skb, dev, h, prs_from);
	atomic_inc(&db->stats.direct_hook);
	return 0;
}
EXPORT_SYMBOL(pktprs_do_parse_direct);

int pktprs_register(struct notifier_block *nb, enum pktprs_cb_type t)
{
	if (!nb)
		return -EINVAL;

	switch (t) {
	case PKTPRS_RX:
		atomic_inc(&db->rx_users);
		break;
	case PKTPRS_TX:
		atomic_inc(&db->tx_users);
		break;
	case PKTPRS_RXTX:
		atomic_inc(&db->rxtx_users);
		break;
	case PKTPRS_DROP:
		atomic_inc(&db->drop_users);
		break;
	case PKTPRS_RXTX_DROP:
		atomic_inc(&db->rxtx_users);
		atomic_inc(&db->drop_users);
		break;
	default:
		return -EINVAL;
	}

	return raw_notifier_chain_register(&parse_done_chain, nb);
}
EXPORT_SYMBOL(pktprs_register);

void pktprs_unregister(struct notifier_block *nb, enum pktprs_cb_type t)
{
	if (!nb)
		return;

	switch (t) {
	case PKTPRS_RX:
		if (atomic_read(&db->rx_users))
			atomic_dec(&db->rx_users);
		break;
	case PKTPRS_TX:
		if (atomic_read(&db->tx_users))
			atomic_dec(&db->tx_users);
		break;
	case PKTPRS_RXTX:
		if (atomic_read(&db->rxtx_users))
			atomic_dec(&db->rxtx_users);
		break;
	case PKTPRS_DROP:
		if (atomic_read(&db->drop_users))
			atomic_dec(&db->drop_users);
		break;
	case PKTPRS_RXTX_DROP:
		if (atomic_read(&db->rxtx_users))
			atomic_dec(&db->rxtx_users);
		if (atomic_read(&db->drop_users))
			atomic_dec(&db->drop_users);
		break;
	default:
		return;
	}

	raw_notifier_chain_unregister(&parse_done_chain, nb);
}
EXPORT_SYMBOL(pktprs_unregister);

void igmp_print(struct igmphdr *igmp)
{
	if (!igmp)
		return;

	pr_info("    IGMP: type=%hhu[0x%x] code=%hhu[0x%x] csum=%d[0x%x] group=%u[0x%x]\n",
		igmp->type, igmp->type, igmp->code, igmp->code,
		ntohs(igmp->csum), ntohs(igmp->csum),
		ntohl(igmp->group), ntohl(igmp->group));
}
EXPORT_SYMBOL(igmp_print);

void icmp_print(struct icmphdr *icmp)
{
	if (!icmp)
		return;

	pr_info("    ICMP: type=%hhu[0x%x] code=%hhu[0x%x] checksum=%d[0x%x] id=%hu[0x%x] sequence=%hu[0x%x]\n",
		icmp->type, icmp->type, icmp->code, icmp->code,
		ntohs(icmp->checksum), ntohs(icmp->checksum),
		ntohs(icmp->un.echo.id), ntohs(icmp->un.echo.id),
		ntohs(icmp->un.echo.sequence), ntohs(icmp->un.echo.sequence));
}
EXPORT_SYMBOL(icmp_print);

void icmp6_print(struct icmp6hdr *icmp6)
{
	if (!icmp6)
		return;

	pr_info("    ICMP6: icmp6_type=%hhu[0x%x] icmp6_type=%hhu[0x%x] icmp6_cksum=%d[0x%x] identifier=%hu[0x%x] sequence=%hu[0x%x]\n",
		icmp6->icmp6_type, icmp6->icmp6_type, icmp6->icmp6_type,
		icmp6->icmp6_code, ntohs(icmp6->icmp6_cksum),
		ntohs(icmp6->icmp6_cksum),
		ntohs(icmp6->icmp6_dataun.u_echo.identifier),
		ntohs(icmp6->icmp6_dataun.u_echo.identifier),
		ntohs(icmp6->icmp6_dataun.u_echo.sequence),
		ntohs(icmp6->icmp6_dataun.u_echo.sequence));
}
EXPORT_SYMBOL(icmp6_print);

void l2tp_oudp_print(union l2tphdr *l2tp)
{
	u32 t_flag, l_flag, s_flag, o_flag, p_flag;
	u8 ver;

	if (!l2tp)
		return;

	t_flag = (ntohs(l2tp->v2.flags) & L2TP_HDRFLAG_T) ? 1 : 0;
	l_flag = (ntohs(l2tp->v2.flags) & L2TP_HDRFLAG_L) ? 1 : 0;
	s_flag = (ntohs(l2tp->v2.flags) & L2TP_HDRFLAG_S) ? 1 : 0;
	o_flag = (ntohs(l2tp->v2.flags) & L2TP_HDRFLAG_O) ? 1 : 0;
	p_flag = (ntohs(l2tp->v2.flags) & L2TP_HDRFLAG_P) ? 1 : 0;
	ver    = (ntohs(l2tp->v2.flags) & L2TP_HDR_VER_MASK);

	pr_info("    L2TP_V%u OUDP: T=%u(%s) L=%u S=%u O=%u P=%u ", ver, t_flag,
		t_flag ? "control" : "data", l_flag, s_flag, o_flag, p_flag);

	if (t_flag) {
		pr_info("length=%u[%uBytes] control_conn_id=%u[%#x] ns=%u[%#x] nr=%u[%#x]\n",
			ntohs(l2tp->ctr.length),
			ntohs(l2tp->ctr.length) << 3,
			ntohl(l2tp->ctr.ctr_con_id),
			ntohl(l2tp->ctr.ctr_con_id),
			ntohs(l2tp->ctr.ns), ntohs(l2tp->ctr.ns),
			ntohs(l2tp->ctr.nr), ntohs(l2tp->ctr.nr));
	} else if (l_flag) {
		if (ver == L2TP_HDR_VER_2) {
			pr_info("length=%hu[%uBytes] tunnel_id=%hu[0x%x] session_id=%hu[0x%x]\n",
				ntohs(l2tp->v2_len.length),
				ntohs(l2tp->v2_len.length) << 3,
				ntohs(l2tp->v2_len.tunnel),
				ntohs(l2tp->v2_len.tunnel),
				ntohs(l2tp->v2_len.sess_id),
				ntohs(l2tp->v2_len.sess_id));
		} else {
			pr_info("session_id=%u[0x%x]\n",
				ntohl(l2tp->v3_oudp.sess_id),
				ntohl(l2tp->v3_oudp.sess_id));
		}
	} else {
		if (ver == L2TP_HDR_VER_2) {
			pr_info("tunnel_id=%hu[0x%x] session_id=%hu[0x%x]\n",
				ntohs(l2tp->v2.tunnel), ntohs(l2tp->v2.tunnel),
				ntohs(l2tp->v2.sess_id),
				ntohs(l2tp->v2.sess_id));
		} else {
			pr_info("session_id=%u[0x%x]\n",
				ntohl(l2tp->v3_oudp.sess_id),
				ntohl(l2tp->v3_oudp.sess_id));
		}
	}
}
EXPORT_SYMBOL(l2tp_oudp_print);

void l2tp_oip_print(union l2tphdr *l2tp)
{
	if (!l2tp)
		return;

	pr_info("    L2TPV3 OIP: session_id=%u[0x%x]\n",
		ntohl(l2tp->v3_oip.sess_id), ntohl(l2tp->v3_oip.sess_id));
	if (!l2tp->v3_oip.sess_id) {
		pr_info("                length=%hu[%uBytes] control_conn_id=%u[0x%x] ns=%hu[0x%x] nr=%hu[0x%x]\n",
			ntohs(l2tp->v3_oip.ctr.length),
			ntohs(l2tp->v3_oip.ctr.length) << 3,
			ntohl(l2tp->v3_oip.ctr.ctr_con_id),
			ntohl(l2tp->v3_oip.ctr.ctr_con_id),
			ntohs(l2tp->v3_oip.ctr.ns), ntohs(l2tp->v3_oip.ctr.ns),
			ntohs(l2tp->v3_oip.ctr.nr), ntohs(l2tp->v3_oip.ctr.nr));
	}
}
EXPORT_SYMBOL(l2tp_oip_print);

void ppp_print(unsigned char *ppp)
{
	if (!ppp)
		return;

	pr_info("    PPP: ppp_protocol=%hu[0x%x]\n",
		PPP_PROTOCOL(ppp), PPP_PROTOCOL(ppp));
}
EXPORT_SYMBOL(ppp_print);

void tcp_print(struct tcphdr *tcp)
{
	if (!tcp)
		return;

	pr_info("    TCP: source=%hu[0x%x] dest=%hu[0x%x] seq=%u[0x%x] ack_seq=%u[0x%x] doff=%hu[%uBytes] fin=%hu syn=%hu rst=%hu psh=%hu ack=%hu urg=%hu ece=%hu cwr=%hu window=%hu[0x%x] check=%d[0x%x] urg_ptr=%hu[0x%x]\n",
		ntohs(tcp->source), ntohs(tcp->source), ntohs(tcp->dest),
		ntohs(tcp->dest), ntohl(tcp->seq), ntohl(tcp->seq),
		ntohl(tcp->ack_seq), ntohl(tcp->ack_seq), tcp->doff,
		tcp->doff << 2, tcp->fin, tcp->syn, tcp->rst, tcp->psh,
		tcp->ack, tcp->urg, tcp->ece, tcp->cwr, ntohs(tcp->window),
		ntohs(tcp->window), ntohs(tcp->check), ntohs(tcp->check),
		ntohs(tcp->urg_ptr), ntohs(tcp->urg_ptr));
}
EXPORT_SYMBOL(tcp_print);

void udp_print(struct udphdr *udp)
{
	if (!udp)
		return;

	pr_info("    UDP: source=%hu[0x%x] dest=%hu[0x%x] len=%hu[0x%x] check=%d[0x%x]\n",
		ntohs(udp->source), ntohs(udp->source), ntohs(udp->dest),
		ntohs(udp->dest), ntohs(udp->len), ntohs(udp->len),
		ntohs(udp->check), ntohs(udp->check));
}
EXPORT_SYMBOL(udp_print);

void geneve_print(struct genevehdr *geneve)
{
	if (!geneve)
		return;

	pr_info("    GENEVE: opt_len=%hhu[0x%x] ver=%hhu critical==%hhu oam=%hhu proto_type=%hu[0x%x] vni=%u[0x%x]\n",
		geneve->opt_len, geneve->opt_len, geneve->ver, geneve->critical,
		geneve->oam, ntohs(geneve->proto_type),
		ntohs(geneve->proto_type),
		geneve->vni[2] << 16 | geneve->vni[1] << 8 | geneve->vni[0],
		geneve->vni[2] << 16 | geneve->vni[1] << 8 | geneve->vni[0]);
}
EXPORT_SYMBOL(geneve_print);

void gre_print(struct gre_base_hdr *gre)
{
	if (!gre)
		return;

	pr_info("    GRE: flags=%hu[0x%x] protocol=%hu[0x%x]\n",
		ntohs(gre->flags), ntohs(gre->flags), ntohs(gre->protocol),
		ntohs(gre->protocol));
}
EXPORT_SYMBOL(gre_print);

void esp_print(struct ip_esp_hdr *esp)
{
	if (!esp)
		return;

	pr_info("    ESP: spi=%u[0x%x] seq_no=%u[0x%x]\n",
		ntohl(esp->spi), ntohl(esp->spi), ntohl(esp->seq_no),
		ntohl(esp->seq_no));
}
EXPORT_SYMBOL(esp_print);

void auth_print(struct ip_auth_hdr *auth)
{
	if (!auth)
		return;

	pr_info("    AUTH: nexthdr=%hhu[0x%x] hdrlen=%hhu[0x%x] spi=%u[0x%x] seq_no=%u[0x%x]\n",
		auth->nexthdr, auth->nexthdr, auth->hdrlen, auth->hdrlen,
		ntohl(auth->spi), ntohl(auth->spi), ntohl(auth->seq_no),
		ntohl(auth->seq_no));
}
EXPORT_SYMBOL(auth_print);

void sctp_print(struct sctphdr *sctp)
{
	if (!sctp)
		return;

	pr_info("    SCTP: source=%u[%#x] dest=%u[%#x] vtag=%u[%#x] checksum=%u[%#x]\n",
		ntohs(sctp->source), ntohs(sctp->source), ntohs(sctp->dest),
		ntohs(sctp->dest), ntohl(sctp->vtag), ntohl(sctp->vtag),
		cpu_to_le32(sctp->checksum), cpu_to_le32(sctp->checksum));
}
EXPORT_SYMBOL(sctp_print);

void vxlan_print(struct vxlanhdr *vxlan)
{
	if (!vxlan)
		return;

	pr_info("    VXLAN: vx_flags=%u[0x%x] vx_vni=%u[0x%x]\n",
		ntohl(vxlan->vx_flags), ntohl(vxlan->vx_flags),
		ntohl(vxlan->vx_vni), ntohl(vxlan->vx_vni));
}
EXPORT_SYMBOL(vxlan_print);

void ipv6_rout_opt_print(struct ipv6_opt_hdr *opt)
{
	if (!opt)
		return;

	pr_info("    IPV6_DEST_OPT: nexthdr=%hhu[0x%x] hdrlen=%hhu[%uBytes]\n",
		opt->nexthdr, opt->nexthdr, opt->hdrlen, ipv6_optlen(opt));
}
EXPORT_SYMBOL(ipv6_rout_opt_print);

void ipv6_dest_opt_print(struct ipv6_opt_hdr *opt)
{
	if (!opt)
		return;

	pr_info("    IPV6_DEST_OPT: nexthdr=%hhu[0x%x] hdrlen=%hhu[%uBytes]\n",
		opt->nexthdr, opt->nexthdr, opt->hdrlen, ipv6_optlen(opt));
}
EXPORT_SYMBOL(ipv6_dest_opt_print);

void ipv6_hop_opt_print(struct ipv6_opt_hdr *opt)
{
	if (!opt)
		return;

	pr_info("    IPV6_HOP_OPT: nexthdr=%hhu[0x%x] hdrlen=%hhu[%uBytes]\n",
		opt->nexthdr, opt->nexthdr, opt->hdrlen, ipv6_optlen(opt));
}
EXPORT_SYMBOL(ipv6_hop_opt_print);

void ipv6_frag_opt_print(struct frag_hdr *frag)
{
	if (!frag)
		return;

	pr_info("    IPV6_FRAG_OPT: nexthdr=%hhu[0x%x] frag_off=%hu[0x%x] identification=%u[0x%x]\n",
		frag->nexthdr, frag->nexthdr, ntohs(frag->frag_off),
		ntohs(frag->frag_off), ntohl(frag->identification),
		ntohl(frag->identification));
}
EXPORT_SYMBOL(ipv6_frag_opt_print);

void ipv6_print(struct ipv6hdr *ip6)
{
	if (!ip6)
		return;

	pr_info("    IPV6: priority=%hhu[0x%x] version= %hhu[0x%x] flow_lbl=%u[0x%x] payload_len=%hu[0x%x] nexthdr=%hhu[0x%x] hop_limit=%hhu[0x%x]\n",
		ip6->priority, ip6->priority, ip6->version, ip6->version,
		ip6->flow_lbl[2] << 16 | ip6->flow_lbl[1] << 8 |
		ip6->flow_lbl[0],
		ip6->flow_lbl[2] << 16 | ip6->flow_lbl[1] << 8 |
		ip6->flow_lbl[0],
		ntohs(ip6->payload_len), ntohs(ip6->payload_len), ip6->nexthdr,
		ip6->nexthdr, ip6->hop_limit, ip6->hop_limit);
	pr_info("          saddr=%pI6 daddr=%pI6\n", &ip6->saddr, &ip6->daddr);
}
EXPORT_SYMBOL(ipv6_print);

void ipv4_print(struct iphdr *ip)
{
	if (!ip)
		return;

	pr_info("    IPV4: ihl=%hhu[%uBytes] version=%hhu tos=%hhu[0x%x] tot_len=%hu[0x%x] id=%hu[0x%x] frag_off=%hu[0x%x] ttl=%hhu[0x%x] protocol=%hhu[0x%x] check=%d[0x%x]\n",
		ip->ihl, ip->ihl << 2, ip->version, ip->tos, ip->tos,
		ntohs(ip->tot_len), ntohs(ip->tot_len), ntohs(ip->id),
		ntohs(ip->id), ntohs(ip->frag_off), ntohs(ip->frag_off),
		ip->ttl, ip->ttl, ip->protocol, ip->protocol, ntohs(ip->check),
		ntohs(ip->check));
	pr_info("          saddr=%pI4 daddr=%pI4\n", &ip->saddr, &ip->daddr);
}
EXPORT_SYMBOL(ipv4_print);

void pppoe_print(struct pppoe_hdr *pppoe)
{
	if (!pppoe)
		return;

	pr_info("    PPPOE: type=%u[%#x] ver=%u[0x%#x] code=%u[%#x] sid=%u[%#x] length=%u[%#x]\n",
		pppoe->type, pppoe->type, pppoe->ver, pppoe->ver, pppoe->code,
		pppoe->code, ntohs(pppoe->sid), ntohs(pppoe->sid),
		ntohs(pppoe->length), ntohs(pppoe->length));
}
EXPORT_SYMBOL(pppoe_print);

void vlan_print(struct vlan_hdr *vlan)
{
	if (!vlan)
		return;

	pr_info("    VLAN: TCI=%hu[0x%x] encapsulated_proto=%hu[0x%x]\n",
		ntohs(vlan->h_vlan_TCI), ntohs(vlan->h_vlan_TCI),
		ntohs(vlan->h_vlan_encapsulated_proto),
		ntohs(vlan->h_vlan_encapsulated_proto));
}
EXPORT_SYMBOL(vlan_print);

void eth_print(struct ethhdr *eth)
{
	if (!eth)
		return;

	pr_info("    ETH: h_dest=%pM h_source=%pM h_proto=0x%x\n",
		eth->h_dest, eth->h_source, ntohs(eth->h_proto));
}
EXPORT_SYMBOL(eth_print);

static struct pktprs_dev *pdev_lookup(struct net_device *dev)
{
	struct pktprs_dev *pdev;

	list_for_each_entry(pdev, &db->dev_list, link) {
		if (pdev->dev == dev)
			return pdev;
	}

	return NULL;
}

int pktprs_dev_add(struct net_device *dev)
{
	struct pktprs_dev *pdev;

	if (!db)
		return -EPERM;
	if (!dev)
		return -EINVAL;

	pr_debug("add device %s\n", dev->name);

	if (!list_empty(&dev->adj_list.lower)) {
		pr_err("device %s is not parent device\n", dev->name);
		return -EPERM;
	}

	spin_lock_bh(&db->lock);
	pdev = pdev_lookup(dev);
	if (pdev) {
		pr_debug("device %s already added\n", dev->name);
		spin_unlock_bh(&db->lock);
		return 0;
	}

	pdev = kzalloc(sizeof(*pdev), GFP_ATOMIC);
	if (!pdev) {
		spin_unlock_bh(&db->lock);
		return -ENOMEM;
	}

	pdev->dev = dev;
	list_add(&pdev->link, &db->dev_list);
	spin_unlock_bh(&db->lock);
	pdev->ptype.type = htons(ETH_P_ALL);
	pdev->ptype.func = parse_skb;
	pdev->ptype.dev = dev;
	dev_add_pack(&pdev->ptype);
	atomic_inc(&db->stats.num_devices);

	pr_debug("device %s added\n", dev->name);

	return 0;
}
EXPORT_SYMBOL(pktprs_dev_add);

int pktprs_dev_remove(struct net_device *dev)
{
	struct pktprs_dev *pdev;

	if (!db)
		return -EPERM;
	if (!dev)
		return -EINVAL;

	pr_debug("remove device %s\n", dev->name);

	spin_lock_bh(&db->lock);
	pdev = pdev_lookup(dev);
	if (!pdev) {
		spin_unlock_bh(&db->lock);
		return -EINVAL;
	}
	list_del(&pdev->link);
	spin_unlock_bh(&db->lock);
	dev_remove_pack(&pdev->ptype);
	atomic_dec(&db->stats.num_devices);

	pr_debug("device %s removed\n", dev->name);
	kfree(pdev);

	return 0;
}
EXPORT_SYMBOL(pktprs_dev_remove);

static void dbg_pr_proto(struct pktprs_hdr *h, enum pktprs_proto p,
			 enum pktprs_hdr_level l)
{
	void *phdr = pktprs_hdr(h, p, l);

	if (!phdr)
		return;

	switch (p) {
	case PKTPRS_PROTO_MAC:
		eth_print(phdr);
		break;
	case PKTPRS_PROTO_VLAN0:
	case PKTPRS_PROTO_VLAN1:
	case PKTPRS_PROTO_VLAN2:
	case PKTPRS_PROTO_VLAN3:
	case PKTPRS_PROTO_VLAN4:
		vlan_print(phdr);
		break;
	case PKTPRS_PROTO_PPPOE:
		pppoe_print(phdr);
		break;
	case PKTPRS_PROTO_IPV4:
		ipv4_print(phdr);
		break;
	case PKTPRS_PROTO_IPV6:
		ipv6_print(phdr);
		break;
	case PKTPRS_PROTO_HOP_OPT:
		ipv6_hop_opt_print(phdr);
		break;
	case PKTPRS_PROTO_DEST_OPT:
		ipv6_dest_opt_print(phdr);
		break;
	case PKTPRS_PROTO_ROUT_OPT:
		ipv6_rout_opt_print(phdr);
		break;
	case PKTPRS_PROTO_FRAG_OPT:
		ipv6_frag_opt_print(phdr);
		break;
	case PKTPRS_PROTO_UDP:
		udp_print(phdr);
		break;
	case PKTPRS_PROTO_TCP:
		tcp_print(phdr);
		break;
	case PKTPRS_PROTO_ICMP:
		icmp_print(phdr);
		break;
	case PKTPRS_PROTO_ICMP6:
		icmp6_print(phdr);
		break;
	case PKTPRS_PROTO_ESP:
		esp_print(phdr);
		break;
	case PKTPRS_PROTO_AUTH:
		auth_print(phdr);
		break;
	case PKTPRS_PROTO_GRE:
		gre_print(phdr);
		break;
	case PKTPRS_PROTO_IGMP:
		igmp_print(phdr);
		break;
	case PKTPRS_PROTO_L2TP_OIP:
		l2tp_oip_print(phdr);
		break;
	case PKTPRS_PROTO_L2TP_OUDP:
		l2tp_oudp_print(phdr);
		break;
	case PKTPRS_PROTO_PPP:
		ppp_print(phdr);
		break;
	case PKTPRS_PROTO_SCTP:
		sctp_print(phdr);
		break;
	case PKTPRS_PROTO_VXLAN:
		vxlan_print(phdr);
		break;
	case PKTPRS_PROTO_GENEVE:
		geneve_print(phdr);
		break;
	default:
		break;
	}
}

static void dbg_pr_hdr(struct pktprs_hdr *h)
{
	int l, p;

	for (l = 0; l < PKTPRS_HDR_LEVEL_NUM; l++) {
		if (!PKTPRS_IS_LEVEL(h, l))
			return;
		for (p = 0; p <= PKTPRS_PROTO_LAST; p++)
			dbg_pr_proto(h, p, l);
		pr_info("\n");
	}
}

void pktprs_pr_hdr(struct pktprs_hdr *h, enum pktprs_type t)
{
	struct net_device *dev = NULL;

	if (!h)
		return;

	pr_info("\n");
	dev = dev_get_by_index(&init_net, h->ifindex);
	if (t == PKTPRS_ETH_RX || t == PKTPRS_IPV4_RX || t == PKTPRS_IPV6_RX)
		pr_info("RX Packet parsing (%s):\n", dev ? dev->name : "NODEV");
	else
		pr_info("TX Packet parsing (%s):\n", dev ? dev->name : "NODEV");
	if (dev)
		dev_put(dev);
	if (PKTPRS_IS_LEVEL(h, PKTPRS_HDR_LEVEL0))
		dbg_pr_hdr(h);
	else
		pr_info("No header\n");
}
EXPORT_SYMBOL(pktprs_pr_hdr);

int pktprs_proto_remove(struct pktprs_hdr *h, enum pktprs_proto p,
			enum pktprs_hdr_level l)
{
	u8 off, lvl, proto, next, p_sz = 0;

	if (!h || !PKTPRS_IS_PROTO_VALID(p) || !PKTPRS_IS_LVL_VALID(l))
		return -1;
	if (!test_bit(p, &h->proto_bmap[l]))
		return -1;

	/* find the prev proto and update the next proto */
	for (lvl = l; lvl >= 0; lvl--) {
		for (proto = 0; proto < p; proto++) {
			if (h->proto_info[proto][lvl].nxt == p) {
				h->proto_info[proto][lvl].nxt =
					h->proto_info[p][l].nxt;
				goto prev_found;
			}
		}
	}
	return -1;

prev_found:
	next = h->proto_info[p][l].nxt;
	/* find the proto header size */
	for (lvl = l; lvl < PKTPRS_HDR_LEVEL_NUM; lvl++) {
		off = pktprs_hdr_off(h, next, lvl);
		if (off > h->proto_info[p][l].off) {
			p_sz = off - h->proto_info[p][l].off;
			goto next_found;
		}
	}
	return -1;

next_found:
	off = h->proto_info[p][l].off;
	/* go over all the next protocols and update the offset */
	for (; lvl < PKTPRS_HDR_LEVEL_NUM; lvl++) {
		for (proto = 0; proto <= PKTPRS_PROTO_LAST; proto++) {
			if (h->proto_info[proto][lvl].off > off)
				h->proto_info[proto][lvl].off -= p_sz;
		}
	}

	/* reduce the buf_sz */
	h->buf_sz -= p_sz;

	/* remove the header from buffer */
	memmove(&h->buf[h->proto_info[p][l].off],
			&h->buf[p_sz + h->proto_info[p][l].off],
			h->buf_sz - h->proto_info[p][l].off);
	clear_bit(p, &h->proto_bmap[l]);

	return 0;
}
EXPORT_SYMBOL(pktprs_proto_remove);

int pktprs_proto_remove_after(struct pktprs_hdr *h, enum pktprs_proto p,
			      enum pktprs_hdr_level l)
{
	u8 off, lvl, proto, next;
	u8 new_hdr_sz = 0;

	if (!h || !PKTPRS_IS_PROTO_VALID(p) || !PKTPRS_IS_LVL_VALID(l))
		return -1;

	if (!test_bit(p, &h->proto_bmap[l]))
		return -1;

	proto = p;
	lvl = l;
	next = h->proto_info[proto][lvl].nxt;
	while (next != PKTPRS_PROTO_LAST &&
	       proto != PKTPRS_PROTO_LAST) {
		off = h->proto_info[next][lvl].off;
		/* Check if next proto exists and comes after current proto */
		if (!test_bit(next, &h->proto_bmap[lvl]) ||
		    off < h->proto_info[proto][lvl].off) {
			if (++lvl == PKTPRS_HDR_LEVEL_NUM)
				return -1;
			continue;
		}

		clear_bit(next, &h->proto_bmap[lvl]);
		proto = next;
		next = h->proto_info[proto][lvl].nxt;
		if (!new_hdr_sz)
			new_hdr_sz = off;
	}

	if (new_hdr_sz) {
		/* Update payload offset */
		h->proto_info[PKTPRS_PROTO_PAYLOAD][lvl].off = new_hdr_sz;

		/* reduce the buf_sz */
		h->buf_sz = new_hdr_sz;
	}

	return 0;
}
EXPORT_SYMBOL(pktprs_proto_remove_after);

int pktprs_ipv6_remove(struct pktprs_hdr *h, enum pktprs_hdr_level l)
{
	if (!h) {
		pr_err("PKTPRS: NULL header\n");
		return -EINVAL;
	}

	if (!PKTPRS_IS_LVL_VALID(l)) {
		pr_err("PKTPRS: invalid level %u\n", l);
		return -EINVAL;
	}

	if (PKTPRS_IS_IPV6(h, l))
		pktprs_proto_remove(h, PKTPRS_PROTO_IPV6, l);
	if (PKTPRS_IS_HOP_OPT(h, l))
		pktprs_proto_remove(h, PKTPRS_PROTO_HOP_OPT, l);
	if (PKTPRS_IS_DEST_OPT(h, l))
		pktprs_proto_remove(h, PKTPRS_PROTO_DEST_OPT, l);
	if (PKTPRS_IS_FRAG_OPT(h, l))
		pktprs_proto_remove(h, PKTPRS_PROTO_FRAG_OPT, l);
	if (PKTPRS_IS_ROUT_OPT(h, l))
		pktprs_proto_remove(h, PKTPRS_PROTO_ROUT_OPT, l);

	return 0;
}
EXPORT_SYMBOL(pktprs_ipv6_remove);

static int netdev_handler(struct notifier_block *unused,
			  unsigned long event, void *ptr)
{
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);

	if ((event == NETDEV_RELEASE) || (event == NETDEV_UNREGISTER)) {
		pr_debug("[DEV_EVENT]: dev %s removing netdev\n", dev->name);
		pktprs_dev_remove(dev);
	}

	return 0;
}

static struct notifier_block dev_notifier __read_mostly = {
	.notifier_call = netdev_handler,
};

static void stats_reset(void)
{
	int i;

	for (i = 0; i <= PKTPRS_PROTO_LAST; i++)
		atomic_set(&db->stats.proto[i], 0);

	atomic_set(&db->stats.error_pkt, 0);
	atomic_set(&db->stats.level_ovflw, 0);
	atomic_set(&db->stats.hdr_buf_ovflw, 0);
	atomic_set(&db->stats.vlan_ovflw, 0);
	atomic_set(&db->stats.unsupp_proto, 0);
	atomic_set(&db->stats.free_skbext, 0);
	atomic_set(&db->stats.num_devices, 0);
	atomic_set(&db->stats.hdr_alloc_failed, 0);
	atomic_set(&db->stats.unsupp_gre, 0);
	atomic_set(&db->stats.custom_hook, 0);
	atomic_set(&db->stats.direct_hook, 0);
	atomic_set(&db->stats.local_hook, 0);
	atomic_set(&db->stats.rx_hook, 0);
	atomic_set(&db->stats.tx_hook, 0);
}

static int __init pktprs_init(void)
{
	db = kzalloc(sizeof(*db), GFP_KERNEL);
	if (!db)
		goto error;

	atomic_set(&db->rx_users, 0);
	atomic_set(&db->tx_users, 0);
	atomic_set(&db->rxtx_users, 0);
	atomic_set(&db->drop_users, 0);
	stats_reset();

	/* register skbext */
	db->skb_ext_id = mxl_skb_ext_register(PKTPRS_EXT_NAME,
						 sizeof(struct pktprs_hdr));
	if (db->skb_ext_id == MXL_SKB_EXT_INVALID) {
		pr_err("skbext registration failed\n");
		goto free_mem;
	}

	/* register nf_local hooks */
	if (nf_register_net_hooks(&init_net, pktprs_ops,
				  ARRAY_SIZE(pktprs_ops))) {
		pr_err("nf hooks registration failed\n");
		goto free_mem;
	}

	/* init debugfs */
	dbg_init();

	/* register netevent notifier*/
	if (register_netevent_notifier(&netevent_nb)) {
		pr_err("netevent registration failed\n");
		goto unregister_nfhook;
	}

	/* register netdevice notifier */
	if (register_netdevice_notifier(&dev_notifier) < 0) {
		pr_err("netdevice notifier registration error\n");
		goto unregister_netevent;
	}

	/* init spinlock */
	spin_lock_init(&db->lock);
	/* init device list */
	INIT_LIST_HEAD(&db->dev_list);

	return 0;

unregister_netevent:
	unregister_netevent_notifier(&netevent_nb);
unregister_nfhook:
	nf_unregister_net_hooks(&init_net, pktprs_ops, ARRAY_SIZE(pktprs_ops));
free_mem:
	kfree(db);
error:
	return -EINVAL;
}

static void __exit pktprs_exit(void)
{
	if (!db)
		return;

	dbg_clean();
	unregister_netdevice_notifier(&dev_notifier);
	unregister_netevent_notifier(&netevent_nb);
	nf_unregister_net_hooks(&init_net, pktprs_ops, ARRAY_SIZE(pktprs_ops));
	kfree(db);
	db = NULL;
}

MODULE_AUTHOR("Intel Corporation");
MODULE_DESCRIPTION(MOD_NAME);
MODULE_LICENSE("GPL v2");

module_init(pktprs_init);
module_exit(pktprs_exit);
