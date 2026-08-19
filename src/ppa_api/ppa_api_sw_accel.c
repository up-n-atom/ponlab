/*******************************************************************************
**
** FILE NAME		: ppa_api_sw_accel.c
** PROJECT		: PPA
** MODULES		: PPA API (Routing/Bridging Acceleration APIs)
**
** DATE			: 12 Sep 2013
** AUTHOR		: Lantiq
** DESCRIPTION		: Function to offload CPU and increase Performance
**			once PPE sessions are exhausted.
** COPYRIGHT : Copyright © 2020-2025 MaxLinear, Inc.
**             Copyright (c) 2013
**             Lantiq Deutschland GmbH
**             Am Campeon 3; 85579 Neubiberg, Germany
**
**	 For licensing information, see the file 'LICENSE' in the root folder of
**	 this software module.
**
** HISTORY
** $Date	$Author					$Comment
** 12 Sep 2013	Kamal Eradath				Initiate Version
** 14 Nov 2013	Kamal Eradath				Ported to kernel 3.10
** 14 Mar 2018	Kamal Eradath		Software accleration Stack AL
*******************************************************************************/
/*
 *	Common Head File
 */
#include <linux/version.h>
#include <generated/autoconf.h>
#include <linux/ip.h>
#include <linux/swap.h>
#include <linux/if_vlan.h>
#include <net/ip.h>
#include <net/route.h>
#include <net/protocol.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/ip_tunnels.h>
#include <net/xfrm.h>
#include <net/tcp.h>
#if IS_ENABLED(CONFIG_IPV6)
#include <net/inet6_connection_sock.h>
#include <linux/ipv6.h>
#include <net/ipv6.h>
#include <net/ip6_tunnel.h>
#endif
/*
 *	PPA Specific Head File
 */
#include <net/ppa/ppa_api_common.h>
#include <net/ppa/ppa_api.h>
#include <net/ppa/ppa_hook.h>
#include <net/ppa/ppa_drv_wrapper.h>
#include <net/ppa/ppa_hal_wrapper.h>
#include <net/ppa/ppa_stack_al.h>
#include "ppa_api_misc.h"
#include "ppa_api_netif.h"
#include "ppa_api_session.h"
#include "ppa_api_hal_selector.h"
#include "ppa_api_core.h"
#include "ppa_api_tools.h"
#include "ppa_api_sw_accel.h"

volatile uint8_t g_sw_fastpath_enabled=0;
#if IS_ENABLED(CONFIG_PPA_TCP_LITEPATH)
volatile uint8_t g_tcp_litepath_enabled=0;
#endif
#if IS_ENABLED(CONFIG_PPA_UDP_LITEPATH)
volatile uint8_t g_udp_litepath_enabled=0;
#endif

#define UDP_HDR_LEN	8
#define IPV4_HDR_LEN	20	/*assuming no option fields are present*/
#define IPV6_HDR_LEN	40
#define ETH_HLEN	14	/* Total octets in header.	 */

#define PROTO_FAMILY_IP		2
#define PROTO_FAMILY_IPV6	10

#define PPPOE_HLEN	8
#define PPPOE_IPV4_TAG	0x0021
#define PPPOE_IPV6_TAG	0x0057
#define VLAN_HLEN	4
#define PPP_LEN		4
#define L2TP_HLEN	38 /* IPv4=20 + UDP=8 + L2TP_HDR=6 + PPP_HDR=4 */
#define L2TP_HDR_LEN	6
#define L2TP_UDP_PORT	1701

/* L2TP header constants */
#ifndef L2TP_HDRFLAG_L
#define L2TP_HDRFLAG_L	0x4000
#endif
#ifndef L2TP_HDRFLAG_S
#define L2TP_HDRFLAG_S	0x0800
#endif
#ifndef L2TP_HDRFLAG_O
#define L2TP_HDRFLAG_O	0x0200
#endif

#define PARSER_OFFSET_NUM	40

#define PARSER_PPPOE_OFFSET_IDX		14
#define PARSER_IPV4_OUTER_OFFSET_IDX	15
#define PARSER_IPV6_OUTER_OFFSET_IDX	16
#define PARSER_IPV4_INNER_OFFSET_IDX	17
#define PARSER_IPV6_INNER_OFFSET_IDX	18

/*kamal this definition need to be put in a common header file
 this is the structure of flag header filled by switch and ppe */
struct flag_header {
	/*	0 - 39h*/
	unsigned char	offset[PARSER_OFFSET_NUM];
	/*	40 - 43h*/
	unsigned int	res1;			/* bit 32-63 : Reserved for future use*/
	/*	43 - 47h*/
	unsigned short	is_lro_excep		:1; /* bit 31*/
	unsigned short	is_l2tp_data		:1; /* bit 30*/
	unsigned short	is_ip2_udp		:1; /* bit 29*/
	unsigned short 	is_inner_ipv6_ext	:1; /* bit 28 : FLAG_2IPv6EXT*/
	unsigned short 	is_eapol		:1; /* bit 27 : FLAG_EAPOL*/
	unsigned short 	is_ip_frag		:1; /* bit 26 : FLAG_IPFRAG*/
	unsigned short 	is_tcp_ack		:1; /* bit 25 : FLAG_TCPACK*/
	unsigned short 	is_outer_ipv6_ext	:1; /* bit 24 : FLAG_1IPv6EXT*/
	unsigned short 	is_ipv4_option		:1; /* bit 23 : FLAG_IPv4OPT*/
	unsigned short 	is_igmp			:1; /* bit 22 : FLAG_IGMP*/
	unsigned short 	is_udp			:1; /* bit 21 : FLAG_UDP*/
	unsigned short 	is_tcp			:1; /* bit 20 : FLAG_TCP*/
	unsigned short 	is_rt_excep		:1; /* bit 19 : FLAG_ROUTEXP*/
	unsigned short 	is_inner_ipv6		:1; /* bit 18 : FLAG_2IPv6*/
	unsigned short 	is_inner_ipv4		:1; /* bit 17 : FLAG_2IPv4*/
	unsigned short 	is_outer_ipv6		:1; /* bit 16 : FLAG_1IPv6*/
	unsigned short 	is_outer_ipv4		:1; /* bit 15 : FLAG_1IPv4*/
	unsigned short 	is_pppoes		:1; /* bit 14 : FLAG_PPPoE*/
	unsigned short 	is_snap_encap		:1; /* bit 13 : FLAG_SNAP*/
	unsigned short 	is_vlan			:4; /* bit 9-12 : FLAG_1TAG0, FLAG_1TAG1, FLAG_1TAG2, FLAG_1TAG3*/
	unsigned short 	is_spec_tag		:1; /* bit 8 : FLAG_ITAG*/
	unsigned short 	res2			:2; /* bit 6-7 : Reserved for future use*/
	unsigned short 	is_gre_key		:1; /* bit 5*/
	unsigned short 	is_len_encap		:1; /* bit 4*/
	unsigned short 	is_gre			:1; /* bit 3*/
	unsigned short 	is_capwap		:1; /* bit 2*/
	unsigned short 	is_parser_err		:1; /* bit 1*/
	unsigned short 	is_wol			:1; /* bit 0*/
} __attribute__((packed));

#define IsSoftwareAccelerated(flags)	((flags) & SESSION_ADDED_IN_SW)
#define IsPppoeSession(flags)		((flags) & SESSION_VALID_PPPOE )
#define IsLanSession(flags)		((flags) & SESSION_LAN_ENTRY )
#define IsValidVlanIns(flags)		((flags) & SESSION_VALID_VLAN_INS )
#define IsValidOutVlanIns(flags)	((flags) & SESSION_VALID_OUT_VLAN_INS)
#define IsIpv6Session(flags)		((flags) & SESSION_IS_IPV6)
#define IsTunneledSession(flags)	((flags) & (SESSION_TUNNEL_DSLITE | SESSION_TUNNEL_6RD | SESSION_VALID_PPPOL2TP))
#define IsDsliteSession(flags)		((flags) & SESSION_TUNNEL_DSLITE )
#define Is6rdSession(flags)		((flags) & SESSION_TUNNEL_6RD)
#define IsL2TPSession(flags)		((flags) & SESSION_VALID_PPPOL2TP)
#define IsValidNatIP(flags)		((flags) & SESSION_VALID_NAT_IP)
#define IsBridgedSession(flags)		((flags) & SESSION_FLAG2_BRIDGED_SESSION) 
#define IsGreSession(flags)		((flags) & SESSION_FLAG2_GRE)

static DEFINE_PER_CPU(PPA_HAL_STATS, rtstats);
#define PPA_HAL_RTSTATS_INC(field) raw_cpu_inc(rtstats.field)
#define PPA_HAL_RTSTATS_DEC(field) raw_cpu_dec(rtstats.field)

static int flag_header_ipv6( struct flag_header *pFlagHdr, 
					const unsigned char* data,
					unsigned char data_offset );
static int flag_header_ipv4( struct flag_header *pFlagHdr, 
					const unsigned char* data,
					unsigned char data_offset);
static int flag_header_l2tp( struct flag_header *pFlagHdr,
                              const unsigned char* data,
                              unsigned char data_offset);

typedef unsigned int	uint32_t;
typedef unsigned short	uint16_t;
extern int ppa_find_session_from_skb(PPA_BUF* skb, uint8_t pf, struct uc_session_node **pp_item);
extern void ppa_session_put(struct uc_session_node* p_item);
#if IS_ENABLED(CONFIG_PPA_TCP_LITEPATH) || IS_ENABLED(CONFIG_PPA_UDP_LITEPATH)
extern int ppa_sw_litepath_local_deliver(struct sk_buff *skb);
extern uint32_t (*get_litepath_port_hook)(void);
int32_t (*hw_litepath_xmit_fn)(PPA_SKBUF *) = NULL;
EXPORT_SYMBOL(hw_litepath_xmit_fn);
#endif
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
extern int lpdev_tx(struct sk_buff *);
static uint32_t pp_hw_version;
#endif /* IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM) */

static int32_t ip_ttl_default = 64;

/*Enable below macro to dump the skb*/
#define DBG_DUMP_SKB 0
#if DBG_DUMP_SKB
static int p_flg = 0;
#endif
static void swa_dump_skb(uint8_t *ptr, int len, int flag) 
{
#if DBG_DUMP_SKB
	char *tmp_buf = NULL;
	int i, offset;
	p_flg++;
	
	if (flag || ((p_flg % 10) == 0)) {

		tmp_buf = kmalloc(512, GFP_KERNEL);
		if (!tmp_buf)
			return;
	
		for (i = 0, offset=0; i < len; i++) {
			/*print format control */
			if ((i % 16) == 0)
				offset += sprintf(tmp_buf + offset, "\n");

			offset += sprintf(tmp_buf + offset, "0x%02x ", *(ptr+i));
					
		}
	
		if (offset) {
			offset += sprintf(tmp_buf + offset, "\n");
			printk(KERN_INFO"%s", tmp_buf);
		}
		kfree(tmp_buf);
		p_flg=0;
	}
#endif
}

unsigned short swa_sw_out_header_len(uint32_t flags, uint32_t flag2,
					unsigned short *ethtype) /* ETH type of outgoing packet */
{
	uint16_t headerlength = 0;
	*ethtype = ETH_P_IP;

	/* for local in/out traffic taking SW + HW Path */
	if (flags & SESSION_ADDED_IN_HW)
		goto HW_LITEPATH;

	if (!IsBridgedSession(flag2)) {

		if( IsDsliteSession(flags) ) {
		/* Handle DS-Lite Tunneled sessions */ 
			if( IsLanSession(flags ) ) {
				headerlength += IPV6_HDR_LEN;
				*ethtype = ETH_P_IPV6; 
			}
		} else if( Is6rdSession(flags) ) {
		/* Handle DS-Lite Tunneled sessions */ 
			if(IsLanSession(flags)) {
				headerlength += IPV4_HDR_LEN;
			} else {
				*ethtype = ETH_P_IPV6; 
			}
		} else if (flag2 & SESSION_FLAG2_IPIP) {
			if (IsLanSession(flags))
				headerlength += IPV4_HDR_LEN;
		} else if (flag2 & SESSION_FLAG2_IP6IP6) {
			if (IsLanSession(flags))
				headerlength += (IPV6_HDR_LEN + SZ_8);
#if defined(L2TP_CONFIG) && L2TP_CONFIG
		} else if (IsL2TPSession(flags)) {
			if (IsLanSession(flags))
				headerlength += L2TP_HLEN;
#endif
		}
		if( IsLanSession(flags) && IsPppoeSession(flags) ) {
			headerlength += PPPOE_HLEN;
			*ethtype = ETH_P_PPP_SES;
		}
	}

	if( IsValidVlanIns(flags) ) {
		headerlength += VLAN_HLEN;
		*ethtype = ETH_P_8021Q;
	}

	if (IsValidOutVlanIns(flags)) {
		headerlength += VLAN_HLEN;
		*ethtype = ETH_P_8021Q;
	}

HW_LITEPATH:

	if (!(flags & SESSION_TX_ITF_IPOA_PPPOA_MASK)) {
		headerlength += ETH_HLEN;	/* mac header offset */
	} else {
		*ethtype = 0;
	}

	return headerlength;	 
}

#if IS_ENABLED(CONFIG_PPA_TCP_LITEPATH) || IS_ENABLED(CONFIG_PPA_UDP_LITEPATH)
static struct dst_entry * swa_get_pkt_dst(PPA_BUF *skb, PPA_NETIF* netif)
{
	struct dst_entry *dst=NULL;
	
	if(((dst=skb_dst(skb))!=NULL) && (dst->obsolete != DST_OBSOLETE_NONE)) {
		return dst;
	} else {
		int err = ip_route_input_noref(skb, ip_hdr(skb)->daddr, ip_hdr(skb)->saddr,
								ip_hdr(skb)->tos, netif);
		if (unlikely(err)) {
			return NULL;
		}
		
		if(((dst=skb_dst(skb))!=NULL) && (dst->obsolete != DST_OBSOLETE_NONE)) {
			return (void*) dst;
		} else {
			return NULL;
		}
	}	
}
#endif

#if !IS_ENABLED(CONFIG_X86_INTEL_LGM) && !IS_ENABLED(CONFIG_SOC_LGM)
static int get_dslite_tunnel_header(PPA_NETIF *dev, struct ipv6hdr *ip6hdr)
{
	struct ip6_tnl *t;

	if(dev->type != ARPHRD_TUNNEL6 ){
		return -1;
	}
	t = (struct ip6_tnl *)netdev_priv(dev);

	memset(ip6hdr, 0, sizeof(*ip6hdr));
	ip6hdr->version = 6;
	ip6hdr->hop_limit = t->parms.hop_limit;
	ip6hdr->nexthdr = IPPROTO_IPIP;
#if IS_ENABLED(CONFIG_IPV6)
	ipv6_addr_copy(&ip6hdr->saddr, &t->parms.laddr);
	ipv6_addr_copy(&ip6hdr->daddr, &t->parms.raddr);
#endif
	return 0;
}

static int get_6rd_tunnel_header(PPA_NETIF *dev, struct iphdr* iph)
{
	struct ip_tunnel *t;

	if(dev->type != ARPHRD_SIT ){
		return -1;
	}

	t = (struct ip_tunnel *)netdev_priv(dev);
	memset(iph, 0, sizeof(struct iphdr));
	iph->version	= 4;
	iph->protocol	= IPPROTO_IPV6;
	iph->ihl	= 5;
	iph->ttl	= ip_ttl_default;
	iph->saddr	= t->parms.iph.saddr;
	iph->daddr	= 0; /* Don't use tunnel destination address; Later,
				it is selected based on IPv6 dst address. */
	return 0;
}
#endif

#if defined(L2TP_CONFIG) && L2TP_CONFIG
#if !IS_ENABLED(CONFIG_X86_INTEL_LGM) && !IS_ENABLED(CONFIG_SOC_LGM)
static uint32_t ppa_form_l2tp_tunnel(const struct uc_session_node *p_item,
		uint8_t* hdr, unsigned isIPv6)
{
	struct iphdr iph;
	struct udphdr udph;
	uint32_t outer_srcip = 0;
	uint32_t outer_dstip = 0;

	/* adding IP header to templet buffer */
	ppa_pppol2tp_get_src_addr(p_item->tx_if, &outer_srcip);
	ppa_pppol2tp_get_dst_addr(p_item->tx_if, &outer_dstip);
	form_IPv4_header(&iph, outer_srcip, outer_dstip, 17, L2TP_HLEN); /* need to check from where can i get protocol */
	memcpy(hdr, &iph, sizeof(struct iphdr));
	hdr += IPV4_HDR_LEN;

	/* adding UDP header to templet buffer */
	form_UDP_header(&udph, L2TP_UDP_PORT, L2TP_UDP_PORT, 18);
	memcpy(hdr, &udph, sizeof(struct udphdr));
	hdr += UDP_HDR_LEN;

	/* adding L2TP header to templet buffer */
	*((uint16_t *)hdr) = htons(0x0002);
	*((uint16_t*)(hdr + 2)) = htons(p_item->pkt.pppol2tp_tunnel_id); /* copying l2tp tunnel_id @ appropriate offset */
	*((uint16_t*)(hdr + 4)) = htons(p_item->pkt.pppol2tp_session_id) ; /* copying l2tp session_id @ appropriate offset */
	hdr += L2TP_HDR_LEN;

	/* adding ppp header to templet buffer */
	*(hdr) = 0xff;
	*(hdr + 1) = 0x03;
	if (isIPv6)
		*((uint16_t*)(hdr + 2)) = htons(PPPOE_IPV6_TAG);
	else
		*((uint16_t*)(hdr + 2)) = htons(PPPOE_IPV4_TAG);

	return L2TP_HLEN;
}
#endif /*IS_ENABLED(CONFIG_X86_INTEL_LGM)*/
#endif

/* This function reads the necessary information for software acceleation from	skb and updates the p_item 
*
*/
int32_t swac_update_session_meta(PPA_SESSMETA_INFO *metainfo)
{
	int ret = PPA_SUCCESS;
	unsigned short tlen = 0;
	unsigned short proto_type;
	unsigned isIPv6 = 0;
	unsigned char* hdr;
	struct netif_info *tx_ifinfo = (struct netif_info *) metainfo->txif_info;
	struct uc_session_node *p_item = (struct uc_session_node *)metainfo->p_item;
	
	t_sw_hdr swaHdr;
	t_sw_hdr *p_swaHdr;

	if (!p_item)
		return PPA_FAILURE;

	ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d\n", __FUNCTION__, __LINE__);
	/* Following sessions are excluded from software acceleration */
	/* No complementary processing for routing sessions */
	if ((p_item->flags & SESSION_ADDED_IN_HW) && !(p_item->flag2 & SESSION_FLAG2_CPU_BOUND)) {
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT, "%s %d\n", __FUNCTION__, __LINE__);
		return PPA_FAILURE;
	}

#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
	if ((p_item->flags & SESSION_TUNNEL_ESP) || IsTunneledSession(p_item->flags)
		|| IsGreSession(p_item->flag2)
		|| (p_item->flag2 & SESSION_FLAG2_XLAT)) {
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d\n", __FUNCTION__, __LINE__);
		return PPA_FAILURE;
	}

	/*In case of LGM application litepath supports full HW offload*/
	if (p_item->flag2 & SESSION_FLAG2_CPU_BOUND) {
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT, "%s %d\n", __FUNCTION__, __LINE__);
		return PPA_FAILURE;
	}
#endif /* IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM) */

	/* GRE sessions (skip processing) */
	if (IsGreSession(p_item->flag2)) {
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d\n", __FUNCTION__, __LINE__);
		return PPA_SUCCESS;
	}

	/*if the header is already allocated return*/
	if (p_item->session_meta) {
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d\n", __FUNCTION__, __LINE__);
		return PPA_SUCCESS;
	}

	/*allocate memory for thesw_acc_hdr datastructure*/
	memset(&swaHdr, 0, sizeof(t_sw_hdr));

	/*default tx handler */
	if (hw_litepath_xmit_fn && (p_item->flags & SESSION_ADDED_IN_HW)) {
		swaHdr.tx_handler = hw_litepath_xmit_fn;
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT, "tx handler is hw_litepath_xmit\n");
	} else {
		swaHdr.tx_handler = &dev_queue_xmit;
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT, "tx handler is dev_queue_xmit fn\n");
	}

	if(tx_ifinfo){
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d\n", __FUNCTION__, __LINE__);
		/*get the actual txif */
		swaHdr.tx_if = ppa_get_netif_by_net(tx_ifinfo->net, tx_ifinfo->phys_netif_name);
#if IS_ENABLED(CONFIG_PPA_RT_SESS_LEARNING)
		if( tx_ifinfo->flags & NETIF_BRIDGE) {
			if ( (ret = ppa_get_br_dst_port(swaHdr.tx_if, metainfo->skb, &swaHdr.tx_if)) != PPA_SUCCESS ){
				return PPA_FAILURE;
			}
		}
#endif
	} 
	
	if (IsIpv6Session(p_item->flags))
		isIPv6 = 1;

	/* 
	 * Find the length of the header to be uppended 
	 */
	if (swaHdr.tx_if && !dev_is_mac_header_xmit(swaHdr.tx_if)) {
		proto_type = ntohs(metainfo->skb->protocol);
		tlen = 0;
	} else {
		tlen = swa_sw_out_header_len(p_item->flags, p_item->flag2, &proto_type);
	}

	if (IsBridgedSession(p_item->flag2)) {
		swaHdr.network_offset = tlen;
		swaHdr.transport_offset = tlen + ((isIPv6)?IPV6_HDR_LEN:IPV4_HDR_LEN);
		swaHdr.tot_hdr_len = tlen;
		swaHdr.type = SW_ACC_TYPE_BRIDGED;
#if IS_ENABLED(CONFIG_PPA_TCP_LITEPATH) || IS_ENABLED(CONFIG_PPA_UDP_LITEPATH)
	/*local session = CPU_BOUND */
	} else if((p_item->flag2 & SESSION_FLAG2_CPU_IN) && !tx_ifinfo) {
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d\n", __FUNCTION__, __LINE__);
		swaHdr.network_offset = tlen;
		swaHdr.transport_offset = tlen + ((isIPv6)?IPV6_HDR_LEN:IPV4_HDR_LEN);
		swaHdr.tot_hdr_len =	swaHdr.transport_offset;
#if IS_ENABLED(CONFIG_LTQ_TOE_DRIVER)
		if(p_item->flag2 & SESSION_FLAG2_LRO) {
			swaHdr.type = SW_ACC_TYPE_LTCP;
		} else {
			swaHdr.type = SW_ACC_TYPE_LTCP_LRO;
		}
#else
		swaHdr.type = SW_ACC_TYPE_LTCP;
#endif 
		/* rx_if will be bridge interface; we save it for setting skb->dev in the accelerated path*/
		swaHdr.tx_if = ppa_get_pkt_src_if(metainfo->skb);
		/* skb->dst to be stored here for forwarding.		*/
		swaHdr.dst = swa_get_pkt_dst(metainfo->skb, swaHdr.tx_if);
		dst_hold(swaHdr.dst);
		/*default special tx handler for tcp local in traffic*/
		swaHdr.tx_handler = &ppa_sw_litepath_local_deliver;
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d network_offset=%d transport_offset=%d swaHdr.tot_hdr_len=%d\n",
			__FUNCTION__, __LINE__, swaHdr.network_offset, swaHdr.transport_offset,
			swaHdr.tot_hdr_len);
		swa_dump_skb(metainfo->skb->data, 128, 0);
#endif /* IS_ENABLED(CONFIG_PPA_TCP_LITEPATH) */
	} else if ((p_item->flag2 & SESSION_FLAG2_IPIP) ||
		  (p_item->flag2 & SESSION_FLAG2_IP6IP6)) {

		swaHdr.tot_hdr_len = tlen;
#if defined(L2TP_CONFIG) && L2TP_CONFIG
		if (IsL2TPSession(p_item->flags)) {
			swaHdr.type = SW_ACC_TYPE_L2TP;
			if (IsLanSession(p_item->flags)) {
				swaHdr.network_offset = tlen - L2TP_HLEN;
				swaHdr.transport_offset = swaHdr.network_offset + IPV4_HDR_LEN;
			} else {
				swaHdr.network_offset = tlen;
				swaHdr.transport_offset = tlen + ((isIPv6) ? IPV6_HDR_LEN : IPV4_HDR_LEN);
			}
		} else
#endif
		if( IsDsliteSession(p_item->flags) ) {
			swaHdr.type = SW_ACC_TYPE_DSLITE;
			if( IsLanSession(p_item->flags) ) {
				swaHdr.network_offset = tlen - IPV6_HDR_LEN;
				swaHdr.transport_offset = tlen ; /* transport header: inner IPv4 */
				isIPv6 = 1;
			} else {
				swaHdr.network_offset = tlen;
				swaHdr.transport_offset = tlen + IPV4_HDR_LEN;
			}
		} else if (p_item->flag2 & SESSION_FLAG2_IPIP) {
			/* ipip tunnel */
			swaHdr.type = SW_ACC_TYPE_IPIP;
			if (IsLanSession(p_item->flags)) {
				swaHdr.network_offset = tlen - IPV4_HDR_LEN;
				/* transport header points to inner IPv4 */
				swaHdr.transport_offset = tlen;
				isIPv6 = 0;
			} else {
				swaHdr.network_offset = tlen;
				swaHdr.transport_offset = tlen + IPV4_HDR_LEN;
			}
		} else if (p_item->flag2 & SESSION_FLAG2_IP6IP6) {
			/* 6in6 tunnel */
			swaHdr.type = SW_ACC_TYPE_IP6IP6;
			if (IsLanSession(p_item->flags)) {
				/* 8B Destination Options for IPv6 */
				swaHdr.network_offset = tlen - (IPV6_HDR_LEN + SZ_8);
				/* transport header points to inner IPv6 */
				swaHdr.transport_offset = tlen;
				isIPv6 = 1;
			} else {
				swaHdr.network_offset = tlen;
				swaHdr.transport_offset = tlen + IPV6_HDR_LEN;
			}
		} else {
			/* 6rd tunnel */
			swaHdr.type = SW_ACC_TYPE_6RD;
			if( IsLanSession(p_item->flags) ) {
				swaHdr.network_offset = tlen - IPV4_HDR_LEN;
				swaHdr.transport_offset = tlen ; /* transport header:inner IPv4 */
				isIPv6 = 0;
			} else {
				swaHdr.network_offset = tlen;
				swaHdr.transport_offset = tlen + IPV6_HDR_LEN;
			}
		}
	} else {
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d\n", __FUNCTION__, __LINE__);
		if (p_item->flag2 & SESSION_FLAG2_FRAG) {
			ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT, "(%s:%d) No SWA fragmented pkts\n",
				  __FUNCTION__, __LINE__);
			return PPA_FAILURE;
		}
		/* IPV4/IPV6 session */
		swaHdr.network_offset = swaHdr.transport_offset = tlen;
		if( isIPv6 ) {
			swaHdr.type = SW_ACC_TYPE_IPV6;
			swaHdr.transport_offset += IPV6_HDR_LEN;
		} else {
			swaHdr.type = SW_ACC_TYPE_IPV4;
			swaHdr.transport_offset += IPV4_HDR_LEN;
		}
		/*
		* Since copying original IPV4/IPV6 from skb, so need to allocate memory
		* for network header. While accelerating, the network header is also
		* copied to skb. Copying network header into skb can be avoided if
		* NATing is done during acceleration
		*/
		swaHdr.tot_hdr_len =	swaHdr.transport_offset;
	}

	/* 
	* Allocate memory
	* Now software header + header to be copied is allocated in contineous
	 * memory
	 */
	p_swaHdr = (t_sw_hdr*)ppa_malloc(sizeof(t_sw_hdr)+swaHdr.tot_hdr_len);
	if (p_swaHdr == NULL) {
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d\n", __FUNCTION__, __LINE__);
		return PPA_ENOMEM;
	}
	memcpy(p_swaHdr, &swaHdr, sizeof(t_sw_hdr));

	hdr = p_swaHdr->hdr;

#if IS_ENABLED(CONFIG_PPA_EXT_PKT_LEARNING)
	/* In case of LGM we have the final EG skb at this point*/
	memcpy(hdr, metainfo->skb->data, swaHdr.tot_hdr_len);
#else
#if IS_ENABLED(CONFIG_PPA_TCP_LITEPATH) || IS_ENABLED(CONFIG_PPA_UDP_LITEPATH)
	if (swaHdr.type != SW_ACC_TYPE_LTCP
#if IS_ENABLED(CONFIG_LTQ_TOE_DRIVER) && CONFIG_LTQ_TOE_DRIVER 
		&& swaHdr.type != SW_ACC_TYPE_LTCP_LRO 
#endif
	)	/* no template buffer needed for local traffic */
#endif
	{
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d\n", __FUNCTION__, __LINE__);
		/*construct the datalink header*/
		if( !(p_item->flags & SESSION_TX_ITF_IPOA_PPPOA_MASK) )	/* put ethernet header */
		{
			if (IsBridgedSession(p_item->flag2)) {
				memcpy(hdr, eth_hdr(metainfo->skb), ETH_ALEN*2);
			} else {
				/*get the MAC address of txif
				in case of l2nat we need the mac address of the lower device, not the bridge*/
				if (tx_ifinfo && tx_ifinfo->flags & NETIF_L2NAT) {
					ppa_get_netif_hwaddr(swaHdr.tx_if, hdr + ETH_ALEN, 1);
				} else {
					ppa_get_netif_hwaddr(p_item->tx_if, hdr + ETH_ALEN, 1);
				}

				memcpy(hdr, p_item->pkt.dst_mac, ETH_ALEN);
			}
			hdr += ETH_ALEN*2;
	
			if (!(p_item->flags & SESSION_ADDED_IN_HW) && IsValidOutVlanIns(p_item->flags)) {
				*((uint32_t*)(hdr)) = htonl(p_item->pkt.out_vlan_tag);
				hdr += VLAN_HLEN;
			}
			if (!(p_item->flags & SESSION_ADDED_IN_HW) && IsValidVlanIns(p_item->flags)) {
				*((uint16_t*)(hdr)) = htons(ETH_P_8021Q); 
				*((uint16_t*)(hdr+2)) = htons(p_item->pkt.new_vci); 
				hdr += VLAN_HLEN;
			}

			if (!(p_item->flags & SESSION_ADDED_IN_HW) && IsLanSession(p_item->flags) && IsPppoeSession(p_item->flags)) {
				proto_type = ETH_P_PPP_SES;
			}
			else if (isIPv6) {
				proto_type = ETH_P_IPV6;
			} else {
				proto_type = ETH_P_IP;
			}
		
			*((uint16_t*)(hdr)) = htons(proto_type);
			hdr += 2; /* Two bytes for ETH protocol field */

			/* construct pppoe header for non-HW path */
			if (!(p_item->flags & SESSION_ADDED_IN_HW) && IsLanSession(p_item->flags) && IsPppoeSession(p_item->flags)) {
				/*struct swa_pppoe_hdr *ppphdr; //Make use of this struct*/
				*((uint16_t*)(hdr)) = htons(0x1100);
				*((uint16_t*)(hdr+2)) = p_item->pkt.pppoe_session_id; //sid
				/* payload length: Actual payload length will be updated in data path */
				*((uint16_t*)(hdr+4)) = 0x0000;
				if(isIPv6) { /* ppp type ipv6*/
					*((uint16_t*)(hdr+6)) = htons(PPPOE_IPV6_TAG);
				} else {
					*((uint16_t*)(hdr+6)) = htons(PPPOE_IPV4_TAG);
				}
				hdr += PPPOE_HLEN;
			}
		}
		if (IsBridgedSession(p_item->flag2))
			goto hdr_done;

		/*Now 'hdr' should point to network header*/
		if (!IsTunneledSession( p_item->flags)) {
			/*copy the network header to the buffer*/
			memcpy(p_swaHdr->hdr + p_swaHdr->network_offset,
				metainfo->skb->data, (p_swaHdr->transport_offset)-(p_swaHdr->network_offset));	
			if( p_item->pkt.nat_src_ip.ip && IsValidNatIP(p_item->flags) && (isIPv6 = 0) ) {
				if( IsLanSession(p_item->flags) ) {	
					/*replace source ip*/
					memcpy(p_swaHdr->hdr + p_swaHdr->network_offset + 12,
					       &p_item->pkt.nat_src_ip.ip, 4);
				} else {
					/*replace destination ip*/
					memcpy(p_swaHdr->hdr + p_swaHdr->network_offset + 16,
					       &p_item->pkt.nat_src_ip.ip, 4);
				}
			}
		} else if (IsLanSession(p_item->flags)) {
			/* Add Tunnel header here */
			if (IsDsliteSession(p_item->flags)) {
				struct ipv6hdr ip_6hdr;
				get_dslite_tunnel_header(p_item->tx_if,&ip_6hdr);
				memcpy(p_swaHdr->hdr+p_swaHdr->network_offset,
				       &ip_6hdr, sizeof(ip_6hdr));
			} else if (Is6rdSession(p_item->flags)) {
				struct iphdr iph;
				get_6rd_tunnel_header(p_item->tx_if, &iph);
				iph.daddr = p_item->pkt.sixrd_daddr;
#ifdef CONFIG_PPA_PP_LEARNING
					iph.check = ip_fast_csum((unsigned char *)&iph, iph.ihl);
#endif
				memcpy(p_swaHdr->hdr+p_swaHdr->network_offset, &iph, sizeof(iph));
			}
#if defined(L2TP_CONFIG) && L2TP_CONFIG
			else if (IsL2TPSession(p_item->flags))
				ppa_form_l2tp_tunnel(p_item, hdr, isIPv6);
#endif
		}
	}

hdr_done:	
#endif /* CONFIG_PPA_EXT_PKT_LEARNING */
	p_item->session_meta=p_swaHdr;
	swa_dump_skb(p_swaHdr->hdr, swaHdr.tot_hdr_len, 1);
	ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d allocated swacc template buffer\n", __FUNCTION__, __LINE__);
 
	return ret;
}

int32_t swac_add_routing_entry(PPA_ROUTING_INFO *route) 
{

	struct uc_session_node *p_item = (struct uc_session_node *)route->p_item;
	if(!p_item) {
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d add swacc session add failed\n", __FUNCTION__, __LINE__);
		PPA_HAL_RTSTATS_INC(uc_dropped_sess);
		return PPA_FAILURE;
	}

#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
	/*In case of LGM application litepath supports full HW offload*/
	if (p_item->flag2 & SESSION_FLAG2_CPU_BOUND) {
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT, "%s %d\n", __FUNCTION__, __LINE__);
		return PPA_FAILURE;
	}
#endif /* IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM) */

#if IS_ENABLED(CONFIG_PPA_TCP_LITEPATH) || IS_ENABLED(CONFIG_PPA_UDP_LITEPATH)
	/* local session = CPU_BOUND*/
	if( (p_item->flag2 & SESSION_FLAG2_CPU_BOUND) ) {
	/* in case of local in traffic*/
	/* session can be sw accelerated IFF	1: session is added to LRO 2: session cannot be added to lro */
	/* in case of local out traffic SESSION_FLAG2_ADD_HW_FAIL will always be set*/
#if IS_ENABLED(CONFIG_LTQ_TOE_DRIVER)
		if(!(p_item->flag2 & SESSION_FLAG2_LRO) && !(p_item->flag2 & SESSION_FLAG2_ADD_HW_FAIL)) {
			/*return PPA_FAILURE;*/
		}
#endif
		if (((p_item->flag2 & SESSION_FLAG2_CONTAINER) || (p_item->flag2 & SESSION_FLAG2_VETH)) && !IsIpv6Session(p_item->flags))
			goto SKIP_PPPOE_TUNNEL_CHECK;

		/* FIXME : tcp litepath does not support traffic or any tunnelled traffic*/
		if (IsTunneledSession(p_item->flags) || IsGreSession(p_item->flag2)) {
			p_item->flags |= SESSION_NOT_ACCELABLE; /*to avoid hitting the complete learning path again*/
			ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d add swacc session add failed\n", __FUNCTION__, __LINE__);
			return PPA_FAILURE;
		}

SKIP_PPPOE_TUNNEL_CHECK:
		if(!g_tcp_litepath_enabled) {
			ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d add swacc session add failed\n", __FUNCTION__, __LINE__);
			PPA_HAL_RTSTATS_INC(uc_dropped_sess);
			return PPA_FAILURE;
		}
	}
#endif 
	/* Skip the sessions that do not have the software acceleration header created */
	if(p_item->session_meta == NULL) {
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d add swacc session add failed\n", __FUNCTION__, __LINE__);
		PPA_HAL_RTSTATS_INC(uc_dropped_sess);
		return PPA_FAILURE;
	}

	if(p_item->flags & SESSION_ADDED_IN_SW) {
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d session already in swa\n", __FUNCTION__, __LINE__);
		return PPA_SUCCESS;
	}

	if(g_sw_fastpath_enabled) { 
		p_item->flags |= SESSION_ADDED_IN_SW;
	} else {
		return PPA_FAILURE;
	}

	if (p_item->session)
		test_and_set_bit(IPS_OFFLOAD_BIT, &p_item->session->status);

	/* check for ttl default value change*/
	ip_ttl_default = ppa_get_ip_ttl_default();
	ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d added swacc session add succeeded\n", __FUNCTION__, __LINE__);
	if (IsIpv6Session(p_item->flags))
		PPA_HAL_RTSTATS_INC(curr_uc_ipv6_session);
	else
		PPA_HAL_RTSTATS_INC(curr_uc_ipv4_session);
	return PPA_SUCCESS;
}

static void del_swah(struct uc_session_node *p_item)
{
	if (p_item->session_meta) {
#if IS_ENABLED(CONFIG_PPA_TCP_LITEPATH) || IS_ENABLED(CONFIG_PPA_UDP_LITEPATH)
		t_sw_hdr *swa = NULL;

		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d del swah\n", __FUNCTION__, __LINE__);
		swa = (t_sw_hdr*)(p_item->session_meta);
		if((p_item->flag2 & SESSION_FLAG2_CPU_IN) && (swa->dst)) {
			ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d\n", __FUNCTION__, __LINE__);
			dst_release(swa->dst);
		}
#endif
		ppa_free(p_item->session_meta);
		p_item->session_meta = NULL;
	}
	ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d del swah\n", __FUNCTION__, __LINE__);
}

void swac_del_routing_entry(PPA_ROUTING_INFO *route)
{
	struct uc_session_node *p_item = (struct uc_session_node *)route->p_item;

	if(!p_item) return;
	del_swah(p_item);

	if( (p_item->flags & SESSION_ADDED_IN_SW) ) {
		p_item->flags &= ~SESSION_ADDED_IN_SW;
	}

	if (p_item->session)
		clear_bit(IPS_OFFLOAD_BIT, &p_item->session->status);

	ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d deleted swacc session\n", __FUNCTION__, __LINE__);
	if (IsIpv6Session(p_item->flags))
		PPA_HAL_RTSTATS_DEC(curr_uc_ipv6_session);
	else
		PPA_HAL_RTSTATS_DEC(curr_uc_ipv4_session);
}

#if defined(L2TP_CONFIG) && L2TP_CONFIG
static int flag_header_l2tp( struct flag_header *pFlagHdr,
                              const unsigned char* data,
                              unsigned char data_offset)
{
	int isValid = 1;
	uint16_t l2tp_hdr_flags;
	uint8_t l2tp_opt_len = 0;
	uint8_t l2tp_tot_len = 0;
	uint16_t ppp_proto;

	pFlagHdr->is_l2tp_data = 1;

	/* Calculate L2TP optional length from header flags */
	l2tp_hdr_flags = ntohs(*((__be16 *)data));
	if (l2tp_hdr_flags & L2TP_HDRFLAG_L)
		l2tp_opt_len += 2;
	if (l2tp_hdr_flags & L2TP_HDRFLAG_S)
		l2tp_opt_len += 4;
	if (l2tp_hdr_flags & L2TP_HDRFLAG_O)
		l2tp_opt_len += 2;

	l2tp_tot_len = (L2TP_HDR_LEN + l2tp_opt_len + PPP_LEN);
	data_offset += l2tp_tot_len;
	ppp_proto = ntohs(*((uint16_t *)(data + l2tp_tot_len - 2)));
	switch (ppp_proto) {
	case PPPOE_IPV4_TAG:
		pFlagHdr->is_inner_ipv6 = 0;
		pFlagHdr->offset[PARSER_IPV6_INNER_OFFSET_IDX] = 0;
		pFlagHdr->is_inner_ipv4 = 1;
		pFlagHdr->offset[PARSER_IPV4_INNER_OFFSET_IDX] = data_offset;
		isValid = flag_header_ipv4(pFlagHdr, (data + l2tp_tot_len), data_offset);
		break;
	case PPPOE_IPV6_TAG:
		pFlagHdr->is_inner_ipv4 = 0;
		pFlagHdr->offset[PARSER_IPV4_INNER_OFFSET_IDX] = 0;
		pFlagHdr->is_inner_ipv6 = 1;
		pFlagHdr->offset[PARSER_IPV6_INNER_OFFSET_IDX] = data_offset;
		isValid = flag_header_ipv6(pFlagHdr, (data + l2tp_tot_len), data_offset);
		break;
	default:
		isValid = 0;
		break;
	}

	return isValid;
}
#endif

static int flag_header_ipv4( struct flag_header *pFlagHdr, const unsigned char* data,
					unsigned char data_offset)
{
	int isValid=1;
	struct iphdr *iph = (struct iphdr*)(data);
	struct tcphdr *tcph;
#if defined(L2TP_CONFIG) && L2TP_CONFIG
	struct udphdr *udph;
#endif

	if (iph->frag_off & htons(IP_MF|IP_OFFSET))
		return 0;

	switch(iph->protocol) {
	case IPPROTO_UDP: {
#if defined(L2TP_CONFIG) && L2TP_CONFIG
		udph = (struct udphdr *)(data + IPV4_HDR_LEN);
		if ((udph->source == L2TP_UDP_PORT) && (udph->dest == L2TP_UDP_PORT)) {
			data_offset += (IPV4_HDR_LEN + UDP_HDR_LEN);
			isValid = flag_header_l2tp(pFlagHdr, (data + IPV4_HDR_LEN + UDP_HDR_LEN), data_offset);
		} else
#endif
			pFlagHdr->is_udp = 1;
		break;
	}
	case IPPROTO_TCP: {
		pFlagHdr->is_tcp = 1;
		tcph = (struct tcphdr *)(data + IPV4_HDR_LEN);
		if(tcph->rst||tcph->fin) {
			pFlagHdr->is_rt_excep = 1;
		}
		break;
	}
	case IPPROTO_IPV6: {
		data_offset += sizeof(*iph);
		pFlagHdr->is_inner_ipv4 = 0;
		pFlagHdr->offset[PARSER_IPV4_INNER_OFFSET_IDX] = 0;
		pFlagHdr->is_inner_ipv6 = 1;
		pFlagHdr->offset[PARSER_IPV6_INNER_OFFSET_IDX] = data_offset;
		isValid=flag_header_ipv6(pFlagHdr,data+sizeof(*iph), data_offset);
		break;
	}
	case IPPROTO_IPIP: {
		data_offset += sizeof(*iph);
		pFlagHdr->is_inner_ipv6 = 0;
		pFlagHdr->offset[PARSER_IPV6_INNER_OFFSET_IDX] = 0;
		pFlagHdr->is_inner_ipv4 = 1;
		pFlagHdr->offset[PARSER_IPV4_INNER_OFFSET_IDX] = data_offset;
		isValid = flag_header_ipv4(pFlagHdr, data+sizeof(*iph), data_offset);
		break;
	}
	default:
		isValid = 0;
		break;
	}
	return isValid;
}

static int flag_header_ipv6( struct flag_header *pFlagHdr, 
				const unsigned char* data,
				unsigned char data_offset )
{
	int isValid=1;
	struct ipv6hdr *ip6h = (struct ipv6hdr*)(data);

	switch(ip6h->nexthdr){
	case IPPROTO_UDP:
		pFlagHdr->is_udp = 1;
		break;
	case IPPROTO_TCP: {
		struct tcphdr *tcph;

		pFlagHdr->is_tcp = 1;
		tcph = (struct tcphdr *)(data + IPV6_HDR_LEN);
		if(tcph->rst||tcph->fin) 
			pFlagHdr->is_rt_excep = 1;
		break;
	}
	case IPPROTO_IPIP: {
		data_offset += sizeof(*ip6h);
		pFlagHdr->is_inner_ipv6 = 0;
		pFlagHdr->offset[PARSER_IPV6_INNER_OFFSET_IDX] = 0;
		pFlagHdr->is_inner_ipv4 = 1;
		pFlagHdr->offset[PARSER_IPV4_INNER_OFFSET_IDX] = data_offset;
		isValid=flag_header_ipv4(pFlagHdr,data+sizeof(*ip6h), data_offset);
		break;
	}
	//case NEXTHDR_DEST: // for DS-Lite
	//	data_offset += sizeof(*ip6h);
	//	data += sizeof(*ip6h);
	//	if ()
	//	ipv6_optlen(data);
	//	break;
	default:
		isValid = 0;
		break;
	}
	return isValid;
}

static int set_flag_header( struct flag_header *pFlagHdr,
				unsigned short ethType,
				const unsigned char* data,
				unsigned char data_offset)
{
	int isValid=1;
	switch(ntohs(ethType)) {
	case ETH_P_IP: {
		pFlagHdr->is_outer_ipv4 = 1;
		pFlagHdr->offset[PARSER_IPV4_OUTER_OFFSET_IDX] = data_offset;
		isValid=flag_header_ipv4(pFlagHdr, data, data_offset);
			break;
	}
	case ETH_P_IPV6: {
		pFlagHdr->is_outer_ipv6 = 1;
		pFlagHdr->offset[PARSER_IPV6_OUTER_OFFSET_IDX] = data_offset;
		isValid=flag_header_ipv6(pFlagHdr, data, data_offset);
		break;
	}
	case ETH_P_PPP_SES: {
		pFlagHdr->is_pppoes = 1;
		pFlagHdr->offset[PARSER_PPPOE_OFFSET_IDX] = data_offset;
		data_offset += 8;
		if((*(unsigned short*)(data+6)) == htons(PPPOE_IPV4_TAG)) {
			pFlagHdr->is_outer_ipv4 = 1;
			pFlagHdr->offset[PARSER_IPV4_OUTER_OFFSET_IDX] = data_offset;
			isValid=flag_header_ipv4(pFlagHdr, data + 8, data_offset);
			} else if((*(unsigned short*)(data + 6)) == htons(PPPOE_IPV6_TAG)) {
			pFlagHdr->is_outer_ipv6 = 1;
			pFlagHdr->offset[PARSER_IPV6_OUTER_OFFSET_IDX] = data_offset;
			isValid=flag_header_ipv6(pFlagHdr, data + 8, data_offset);
		} else {
			isValid=0;
		}
		break;
	}
	case ETH_P_8021Q: {
		pFlagHdr->is_vlan = 1;
		isValid = set_flag_header(pFlagHdr,*(unsigned short*)(data+2),data+4, data_offset+4);
		break;
	}
	default:
		isValid=0;
		break;
	}
	return isValid;
}

#if IS_ENABLED(CONFIG_SOC_GRX500)
static unsigned char *skb_data_begin(PPA_SKBUF *skb)
{
	struct dma_rx_desc_2 *desc_2 = (struct dma_rx_desc_2 *)&((struct sk_buff *)skb)->DW2;
	struct dma_rx_desc_3 *desc_3 = (struct dma_rx_desc_3 *)&((struct sk_buff *)skb)->DW3;

	return desc_2 ? (desc_2->field.data_ptr ?  (unsigned char *)(desc_2->field.data_ptr + desc_3->field.byte_offset) : NULL) : NULL;
}
#endif

static unsigned char *get_skb_flag_header(PPA_SKBUF *skb)
{
#if IS_ENABLED(CONFIG_SOC_GRX500)
	return (skb_data_begin(skb));
#else
	return NULL;
#endif
}

static unsigned int IsSoftwareAccelerable(struct flag_header *flg_hdr)
{
	if(!flg_hdr) return 0;
	/*if the packet is UDP or is TCP and not RST or FIN*/
	return ( ( flg_hdr->is_udp || flg_hdr->is_tcp ) && !flg_hdr->is_rt_excep );
}

static unsigned int get_ip_inner_offset(struct flag_header *flg_hdr)
{
	unsigned int ip_inner_offset;

	if (flg_hdr->is_inner_ipv4) {
		ip_inner_offset = flg_hdr->offset[PARSER_IPV4_INNER_OFFSET_IDX]; 
	} else if (flg_hdr->is_inner_ipv6) {
		ip_inner_offset = flg_hdr->offset[PARSER_IPV6_INNER_OFFSET_IDX]; 
	} else if (flg_hdr->is_outer_ipv4) {
		ip_inner_offset = flg_hdr->offset[PARSER_IPV4_OUTER_OFFSET_IDX]; 
	} else {
		ip_inner_offset = flg_hdr->offset[PARSER_IPV6_OUTER_OFFSET_IDX]; 
	}
	return ip_inner_offset;
}

static unsigned char get_pf(struct flag_header *flg_hdr)
{
	unsigned char pf;

	if (flg_hdr->is_inner_ipv4) {
		pf = PROTO_FAMILY_IP;
	} else if (flg_hdr->is_inner_ipv6) {
		pf = PROTO_FAMILY_IPV6;
	} else if (flg_hdr->is_outer_ipv4) {
		pf = PROTO_FAMILY_IP;
	} else {
		pf = PROTO_FAMILY_IPV6;
	}
	return pf;
}

static int sw_mod_ipv4_skb( PPA_SKBUF *skb, 
							struct uc_session_node *p_item)
{
	t_sw_hdr	*swa;
	struct iphdr org_iph;
	struct iphdr *iph=NULL;
	
	swa = (t_sw_hdr*)(p_item->session_meta);
	memcpy(&org_iph, skb->data, sizeof(org_iph));
	/* skb has enough headroom available */


	/* set the skb->data to the point where we can copy the new header which includes (ETH+VLAN*+PPPoE*+IP) 
	*optional	*/
	skb_push(skb, swa->tot_hdr_len - IPV4_HDR_LEN);

	/* copy the header buffer to the packet*/
	memcpy(skb->data, swa->hdr, swa->tot_hdr_len);
	/* set the skb pointers porperly*/
	skb_set_mac_header(skb, 0);
	skb_set_network_header(skb, swa->network_offset); 
	skb_set_transport_header(skb, swa->transport_offset);
	/* point to ip header*/
	iph = (struct iphdr *)skb_network_header(skb);

	/* decrement the original ttl update in the packet*/
	iph->ttl = org_iph.ttl-1; 
	/* update the id with original id*/
	iph->id = org_iph.id; 

	/* Update the ToS for DSCP remarking */
	if ( (p_item->flags & SESSION_VALID_NEW_DSCP) )
	iph->tos |= ((p_item->pkt.new_dscp) << 2);

	iph->tot_len = org_iph.tot_len; 
	/* calculate header checksum*/
	iph->check = 0;
	iph->check = ip_fast_csum((unsigned char *)iph, iph->ihl); 
		
	/* calculate tcp/udp checksum as the pseudo header has changed*/
	/* we compute only partial checksum using the original value of saddr daddr and port*/
	switch(iph->protocol) {
	case IPPROTO_UDP: { 
		struct udphdr *udph;

		udph = (struct udphdr *)skb_transport_header(skb);
		if (udph->check) {
			inet_proto_csum_replace4(&udph->check, skb, org_iph.saddr, iph->saddr, 1);
			inet_proto_csum_replace4(&udph->check, skb, org_iph.daddr, iph->daddr, 1);
			if (p_item->pkt.nat_port && (p_item->flags & SESSION_VALID_NAT_PORT)) {
				if ((p_item->flags & SESSION_LAN_ENTRY)) {
					inet_proto_csum_replace2(&udph->check, skb, udph->source, p_item->pkt.nat_port, 0);
					udph->source = p_item->pkt.nat_port;
				} else {
					inet_proto_csum_replace2(&udph->check, skb, udph->dest, p_item->pkt.nat_port, 0);
					udph->dest = p_item->pkt.nat_port;
				}
			}
		} else if (p_item->pkt.nat_port && (p_item->flags & SESSION_VALID_NAT_PORT)) {
			if ((p_item->flags & SESSION_LAN_ENTRY))
				udph->source = p_item->pkt.nat_port;
			else
				udph->dest = p_item->pkt.nat_port;
		}
		break;
	}
	case IPPROTO_TCP: {
		struct tcphdr *tcph;

		tcph = (struct tcphdr *)skb_transport_header(skb);
		inet_proto_csum_replace4(&tcph->check, skb, org_iph.saddr, iph->saddr, 1);
		inet_proto_csum_replace4(&tcph->check, skb, org_iph.daddr, iph->daddr, 1);
		if( p_item->pkt.nat_port && (p_item->flags & SESSION_VALID_NAT_PORT)) { 
			if( p_item->flags & SESSION_LAN_ENTRY ) {
				inet_proto_csum_replace2(&tcph->check, skb, tcph->source, p_item->pkt.nat_port, 0);
				tcph->source = p_item->pkt.nat_port;
			} else {
				inet_proto_csum_replace2(&tcph->check, skb, tcph->dest, p_item->pkt.nat_port, 0);
				tcph->dest = p_item->pkt.nat_port;
			}
		}
		break;
	}
	default:
		break;
	}
	return org_iph.tot_len;
}

static int sw_mod_ipv6_skb( PPA_SKBUF *skb, struct uc_session_node *p_item)
{
	t_sw_hdr	*swa;
	struct ipv6hdr	org_ip6;
	struct ipv6hdr	*ip6h;

	memcpy(&org_ip6, (struct ipv6hdr *)skb->data, sizeof(org_ip6));

	swa = (t_sw_hdr*)(p_item->session_meta);
	/* skb has enough headroom is available */

	/* set the skb->data to the point where we can copy the new header
	 which includes (ETH+VLAN*+PPPoE*+IP)	*/
	skb_push(skb, swa->transport_offset - IPV6_HDR_LEN);

	/* copy the header buffer to the packet*/
	memcpy(skb->data, swa->hdr, swa->transport_offset);
	/* set the skb pointers porperly*/
	skb_set_mac_header(skb, 0);
	skb_set_network_header(skb, swa->network_offset);
	skb_set_transport_header(skb, swa->transport_offset);
	ip6h = (struct ipv6hdr *)skb_network_header(skb);
	
	ip6h->hop_limit = org_ip6.hop_limit-1;
	ip6h->payload_len = org_ip6.payload_len;

#if defined IPV6_NAT
	/* this is needed iff there is any ipv6 nat functionality.*/
	switch(ip6h->nexthdr) {
	case IPPROTO_UDP:{	
		struct udphdr *udph;

		udph = (struct udphdr *)skb_transport_header(skb);

		if (udph->check) {
			inet_proto_csum_replace4(&udph->check, skb, org_ip6.saddr.ip[0], ip6h->saddr.ip[0], 1);
			inet_proto_csum_replace4(&udph->check, skb, org_ip6.saddr.ip[1], ip6h->saddr.ip[1], 1);
			inet_proto_csum_replace4(&udph->check, skb, org_ip6.saddr.ip[2], ip6h->saddr.ip[2], 1);
			inet_proto_csum_replace4(&udph->check, skb, org_ip6.saddr.ip[3], ip6h->saddr.ip[3], 1);

			inet_proto_csum_replace4(&udph->check, skb, org_ip6.daddr.ip[0], ip6h->daddr.ip[0], 1);
			inet_proto_csum_replace4(&udph->check, skb, org_ip6.daddr.ip[1], ip6h->daddr.ip[1], 1);
			inet_proto_csum_replace4(&udph->check, skb, org_ip6.daddr.ip[2], ip6h->daddr.ip[2], 1);
			inet_proto_csum_replace4(&udph->check, skb, org_ip6.daddr.ip[3], ip6h->daddr.ip[3], 1);

			if (p_item->pkt.nat_port && (p_item->flags & SESSION_VALID_NAT_PORT)) {
				if ((p_item->flags & SESSION_LAN_ENTRY)) {
					inet_proto_csum_replace2(&udph->check, skb, udph->source, p_item->pkt.nat_port, 0);
					udph->source = p_item->pkt.nat_port;
				} else {
					inet_proto_csum_replace2(&udph->check, skb, udph->dest, p_item->pkt.nat_port, 0);
					udph->dest = p_item->pkt.nat_port;
				}
			}
		} else if (p_item->pkt.nat_port && (p_item->flags & SESSION_VALID_NAT_PORT)) {
			if ((p_item->flags & SESSION_LAN_ENTRY))
				udph->source = p_item->pkt.nat_port;
			else
				udph->dest = p_item->pkt.nat_port;
		}
		break;
	}
	case IPPROTO_TCP: {
		struct tcphdr *tcph;
		
		tcph = (struct tcphdr *)skb_transport_header(skb);

		inet_proto_csum_replace4(&tcph->check, skb, org_ip6.saddr.ip[0], ip6h->saddr.ip[0], 1);
		inet_proto_csum_replace4(&tcph->check, skb, org_ip6.saddr.ip[1], ip6h->saddr.ip[1], 1);
		inet_proto_csum_replace4(&tcph->check, skb, org_ip6.saddr.ip[2], ip6h->saddr.ip[2], 1);
		inet_proto_csum_replace4(&tcph->check, skb, org_ip6.saddr.ip[3], ip6h->saddr.ip[3], 1);
		
		inet_proto_csum_replace4(&tcph->check, skb, org_ip6.daddr.ip[0], ip6h->daddr.ip[0], 1);
		inet_proto_csum_replace4(&tcph->check, skb, org_ip6.daddr.ip[1], ip6h->daddr.ip[1], 1);
		inet_proto_csum_replace4(&tcph->check, skb, org_ip6.daddr.ip[2], ip6h->daddr.ip[2], 1);
		inet_proto_csum_replace4(&tcph->check, skb, org_ip6.daddr.ip[3], ip6h->daddr.ip[3], 1);

		if( p_item->pkt.nat_port && (p_item->flags & SESSION_VALID_NAT_PORT)) {
			if( p_item->flags & SESSION_LAN_ENTRY ) {
				inet_proto_csum_replace2(&tcph->check, skb, tcph->source, p_item->pkt.nat_port, 0);
				tcph->source = p_item->pkt.nat_port;
			} else {
				inet_proto_csum_replace2(&tcph->check, skb, tcph->dest, p_item->pkt.nat_port, 0);
											tcph->dest = p_item->pkt.nat_port;
			}
		}
		break;
	}
	default:
		break;
	}
#endif /*IPV6_NAT*/
	return htons(ntohs(ip6h->payload_len) + IPV6_HDR_LEN);
}

static int sw_mod_dslite_skb( PPA_SKBUF *skb, struct uc_session_node *p_item)
{
	t_sw_hdr *swa;
	struct iphdr org_iph;
	struct iphdr *iph;
	int ret = 0;

	swa = (t_sw_hdr*)(p_item->session_meta);

	memcpy(&org_iph, skb->data, sizeof(org_iph));

	/* copy the header buffer to the packet*/
	skb_push(skb, swa->tot_hdr_len);
	memcpy(skb->data, swa->hdr, swa->tot_hdr_len);
	skb_set_mac_header(skb, 0);
	skb_set_network_header(skb, swa->network_offset);
	skb_set_transport_header(skb, swa->transport_offset);
	
	if( IsLanSession(p_item->flags) ) {
		struct ipv6hdr *ip6h;

		ip6h = (struct ipv6hdr *)skb_network_header(skb);
		ip6h->payload_len = org_iph.tot_len ;
		ret = htons(ntohs(org_iph.tot_len) + IPV6_HDR_LEN);
		iph = (struct iphdr *)skb_transport_header(skb);
	} else {
		ret = org_iph.tot_len;
		iph = (struct iphdr *)skb_network_header(skb);
	}

	/* Decrment iph ttl */	
	iph->ttl--;
	iph->check = 0;
	iph->check = ip_fast_csum((unsigned char *)iph, iph->ihl);
	/* TODO: DSCP marking ??? */
	return ret;
}

static int sw_mod_ipip_skb(PPA_SKBUF *skb, struct uc_session_node *p_item)
{
	t_sw_hdr *swa;
	struct iphdr org_iph;
	struct iphdr *iph;
	int ret = 0;

	swa = (t_sw_hdr *)(p_item->session_meta);

	memcpy(&org_iph, skb->data, sizeof(org_iph));

	/*copy the header buffer to the packet*/
	skb_push(skb, swa->tot_hdr_len);
	memcpy(skb->data, swa->hdr, swa->tot_hdr_len);
	skb_set_mac_header(skb, 0);
	skb_set_network_header(skb, swa->network_offset);
	skb_set_transport_header(skb, swa->transport_offset);

	if (IsLanSession(p_item->flags)) {
		struct iphdr *iph1;
		iph1 = (struct iphdr *)skb_network_header(skb);
		ret = iph1->tot_len = htons(ntohs(org_iph.tot_len) + IPV4_HDR_LEN);

		iph1->check = 0;
		iph1->check = ip_fast_csum((unsigned char *)iph1, iph1->ihl);
		iph = (struct iphdr *)skb_transport_header(skb);
	} else {
		ret = ntohs(org_iph.tot_len);
		iph = (struct iphdr *)skb_network_header(skb);
	}

	/* Decrment iph ttl */
	iph->ttl--;
	iph->check = 0;
	iph->check = ip_fast_csum((unsigned char *)iph, iph->ihl);
	/* TODO: DSCP marking ??? */
	return ret;
}

static int sw_mod_ip6ip6_skb(PPA_SKBUF *skb, struct uc_session_node *p_item)
{
	t_sw_hdr *swa;
	struct ipv6hdr org_ip6h;
	struct ipv6hdr *ip6h;
	int ret = 0;

	swa = (t_sw_hdr *)(p_item->session_meta);

	memcpy(&org_ip6h, skb->data, sizeof(org_ip6h));

	/*copy the header buffer to the packet*/
	skb_push(skb, swa->tot_hdr_len);
	memcpy(skb->data, swa->hdr, swa->tot_hdr_len);
	skb_set_mac_header(skb, 0);
	skb_set_network_header(skb, swa->network_offset);
	skb_set_transport_header(skb, swa->transport_offset);

	if (IsLanSession(p_item->flags)) {
		struct ipv6hdr *ip6h1;

		ip6h1 = (struct ipv6hdr *)skb_network_header(skb);
		ip6h1->payload_len = htons(ntohs(org_ip6h.payload_len)+IPV6_HDR_LEN+SZ_8);
		ret = ntohs(ip6h1->payload_len) + IPV6_HDR_LEN + SZ_8;

		ip6h = (struct ipv6hdr *)skb_transport_header(skb);
	} else {
		ret = htons(ntohs(org_ip6h.payload_len) + IPV6_HDR_LEN + SZ_8);
		ip6h = (struct ipv6hdr *)skb_network_header(skb);
	}

	/* Decrement hop limit */
	ip6h->hop_limit--;
	/* TODO: DSCP marking ??? */
	return ret;
}

static int sw_mod_6rd_skb( PPA_SKBUF *skb, struct uc_session_node *p_item)
{
	t_sw_hdr	*swa;
	struct ipv6hdr org_ip6h;
	struct ipv6hdr *ip6h;
	int ret = 0;

	swa = (t_sw_hdr*)(p_item->session_meta);

	memcpy(&org_ip6h, skb->data, sizeof(org_ip6h));
	
	/*copy the header buffer to the packet*/
	skb_push(skb, swa->tot_hdr_len);
	memcpy(skb->data, swa->hdr, swa->tot_hdr_len);
	skb_set_mac_header(skb, 0);
	skb_set_network_header(skb, swa->network_offset);
	skb_set_transport_header(skb, swa->transport_offset);
	
	if( IsLanSession(p_item->flags) ) {
	
		struct iphdr *iph;
	
		iph = (struct iphdr *)skb_network_header(skb);
		ret = iph->tot_len = htons(ntohs(org_ip6h.payload_len) + IPV6_HDR_LEN + IPV4_HDR_LEN);

		iph->check = 0;
		iph->check = ip_fast_csum((unsigned char *)iph, iph->ihl);
		ip6h = (struct ipv6hdr*)skb_transport_header(skb);
	} else {
		ret = htons(ntohs(org_ip6h.payload_len) + IPV6_HDR_LEN);
		ip6h = (struct ipv6hdr *)skb_network_header(skb);
	}

	/* Decrement hop limit */
		ip6h->hop_limit--;
	/* TODO: DSCP marking ??? */
	return ret;
}

static int sw_mod_bridged_skb( PPA_SKBUF *skb, struct uc_session_node *p_item)
{
	int ret;
	t_sw_hdr	*swa;

	swa = (t_sw_hdr*)(p_item->session_meta);
	
	ret = skb->len+swa->tot_hdr_len;

	skb_push(skb, swa->tot_hdr_len);
	memcpy(skb->data, swa->hdr, swa->tot_hdr_len);

	skb_set_mac_header(skb, 0);
	skb_set_network_header(skb, swa->network_offset);
	skb_set_transport_header(skb, swa->transport_offset);
	/*dump_packet("SW-out",skb->data,64);*/

	return ret;
}

#if IS_ENABLED(CONFIG_PPA_TCP_LITEPATH) || IS_ENABLED(CONFIG_PPA_UDP_LITEPATH)
static int sw_mod_ltcp(PPA_BUF *skb, t_sw_hdr *swa)
{
	uint32_t len = 0;

	/*set the dst*/
	if(swa->dst->obsolete != DST_OBSOLETE_NONE) {
		dst_hold(swa->dst);
		skb_dst_set(skb, swa->dst);
	} else {
		dst_release(swa->dst);
		swa->dst = swa_get_pkt_dst(skb, swa->tx_if); /* skb->dst to be stored here for forwarding.*/
		if (!swa->dst)
			return 0;
		dst_hold(swa->dst);
	}

	/*set packet length and packet type = PACKET_HOST*/
	if (ip_hdr(skb)->version == 6) {
		skb->len = ntohs(ipv6_hdr(skb)->payload_len) + sizeof(struct ipv6hdr);
		skb->protocol = htons(ETH_P_IPV6);
		IP6CB(skb)->nhoff = offsetof(struct ipv6hdr, nexthdr);
	} else {
		skb->len = ntohs(ip_hdr(skb)->tot_len);
		skb->protocol = htons(ETH_P_IP);
	}
	len = skb->len;
	skb->pkt_type = PACKET_HOST;

	/*set transport header */
	skb_set_transport_header(skb, (swa->transport_offset - swa->network_offset));
	/*set the skb->data point to the transport header*/
	skb_pull(skb, skb_network_header_len(skb));

	/*set the iif*/
	skb->dev = swa->tx_if;
	skb->skb_iif = skb->dev->ifindex;

	return len;
}

#if IS_ENABLED(CONFIG_LTQ_TOE_DRIVER) 
static int sw_mod_ltcp_skb_lro(PPA_BUF *skb, struct uc_session_node *p_item)
{
	t_sw_hdr	*swa;
	struct iphdr *iph;
	struct tcphdr *tcph;
	__be32 orig_daddr;

	swa = (t_sw_hdr*)(p_item->session_meta);

	/*set the skb pointers porperly*/
	skb_set_network_header(skb, 0); 
	skb_set_transport_header(skb, (swa->transport_offset - swa->network_offset));

	iph = (struct iphdr *)skb_network_header(skb);

	/*if SESSION_FLAG2_LRO is not already set try adding the session to LRO*/
	if( !(p_item->flag2 & (SESSION_FLAG2_LRO|SESSION_FLAG2_ADD_HW_FAIL))){
		if ( ppa_lro_entry_criteria(p_item, skb, 0) == PPA_SUCCESS) {
			p_item->flags &= ~SESSION_ADDED_IN_SW;
			return 0; /* force the packet back through stack to do lro learning*/
		}
	} else {
		swa->type = SW_ACC_TYPE_LTCP;
	} 

	if (p_item->flags & SESSION_VALID_NAT_IP) {
		orig_daddr = iph->daddr;
		memcpy(&(iph->daddr), &(p_item->pkt.nat_src_ip.ip), 4);
		if (p_item->flag2 & SESSION_FLAG2_NON_ETHWAN_SESSION) {
			iph->ttl--;
			iph->check = 0;
			iph->check = ip_fast_csum((unsigned char *)iph, iph->ihl);
			tcph = (struct tcphdr *)skb_transport_header(skb);
			inet_proto_csum_replace4(&tcph->check, skb, orig_daddr, p_item->pkt.nat_src_ip.ip, 1);
		}
	}

	/*Do necessary packet modifications*/
	return sw_mod_ltcp(skb, swa);
}
#endif /*CONFIG_LTQ_TOE_DRIVER*/

static int sw_mod_ltcp_skb(PPA_BUF *skb, struct uc_session_node *p_item)
{
	t_sw_hdr *swa = (t_sw_hdr *)(p_item->session_meta);

	/* set the skb pointers porperly */
	skb_set_network_header(skb, 0); 

	/* Do necessary packet modifications */
	return sw_mod_ltcp(skb, swa);
}
#endif

#if defined(L2TP_CONFIG) && L2TP_CONFIG
static int sw_mod_l2tp_skb(PPA_SKBUF *skb,
                           struct uc_session_node *p_item)
{
	int out_len = 0;
	t_sw_hdr *swa;
	struct iphdr org_iph, *iph, *out_iph;
	struct ipv6hdr *ip6h;
	struct udphdr *udph;
	uint16_t orig_len = 0;
	uint16_t out_iph_id = 0;
	bool is_nat_flag = false;

	swa = (t_sw_hdr*)(p_item->session_meta);

	skb_push(skb, swa->tot_hdr_len);
	memcpy(skb->data, swa->hdr, swa->tot_hdr_len);

	if (IsIpv6Session(p_item->flags)) {
		ip6h = (struct ipv6hdr *)skb_network_header(skb);

		/* Decrement hop limit */
		ip6h->hop_limit--;

		orig_len = ntohs(ip6h->payload_len) + IPV6_HDR_LEN;
	} else {
		iph = (struct iphdr *)skb_network_header(skb);

		/* NAT present */
		if (p_item->pkt.nat_src_ip.ip && IsValidNatIP(p_item->flags)) {
			/*copy original IP header to backup*/
			memcpy(&org_iph, iph, sizeof(org_iph));

			/* replace source ip since its LAN session */
			if (IsLanSession(p_item->flags))
				memcpy(&iph->saddr, &p_item->pkt.nat_src_ip.ip, 4);

			/* replace destination ip */
			else
				memcpy(&iph->daddr, &p_item->pkt.nat_src_ip.ip, 4);

			is_nat_flag = true;
		}

		/* Decrement ttl of inner ip and recalculate checksum as ttl is changed */
		iph->ttl--;
		iph->check = 0;
		iph->check = ip_fast_csum((unsigned char *)iph, iph->ihl);

		if (is_nat_flag) {
			/* calculate tcp/udp checksum as the pseudo header has changed
			 * we compute only partial checksum using the original value of saddr daddr and port */
			switch (iph->protocol) {
			case IPPROTO_UDP: {
				struct udphdr *udph;

				udph = (struct udphdr *)((__u32 *)iph + iph->ihl);
				if (udph->check) {
					inet_proto_csum_replace4(&udph->check, skb, org_iph.saddr, iph->saddr, 1);
					inet_proto_csum_replace4(&udph->check, skb, org_iph.daddr, iph->daddr, 1);
					if (p_item->pkt.nat_port && (p_item->flags & SESSION_VALID_NAT_PORT)) {
						if ((p_item->flags & SESSION_LAN_ENTRY)) {
							inet_proto_csum_replace2(&udph->check, skb, udph->source, p_item->pkt.nat_port, 0);
							udph->source = p_item->pkt.nat_port;
						} else {
							inet_proto_csum_replace2(&udph->check, skb, udph->dest, p_item->pkt.nat_port, 0);
							udph->dest = p_item->pkt.nat_port;
						}
					}
				} else if (p_item->pkt.nat_port && (p_item->flags & SESSION_VALID_NAT_PORT)) {
					if ((p_item->flags & SESSION_LAN_ENTRY))
						udph->source = p_item->pkt.nat_port;
					else
						udph->dest = p_item->pkt.nat_port;
				}
				break;
			}
			case IPPROTO_TCP: {
				struct tcphdr *tcph;

				tcph = (struct tcphdr *)((__u32 *)iph + iph->ihl);
				inet_proto_csum_replace4(&tcph->check, skb, org_iph.saddr, iph->saddr, 1);
				inet_proto_csum_replace4(&tcph->check, skb, org_iph.daddr, iph->daddr, 1);
				if (p_item->pkt.nat_port && (p_item->flags & SESSION_VALID_NAT_PORT)) {
					if (p_item->flags & SESSION_LAN_ENTRY) {
						inet_proto_csum_replace2(&tcph->check, skb, tcph->source, p_item->pkt.nat_port, 0);
						tcph->source = p_item->pkt.nat_port;
					} else {
						inet_proto_csum_replace2(&tcph->check, skb, tcph->dest, p_item->pkt.nat_port, 0);
						tcph->dest = p_item->pkt.nat_port;
					}
				}
				break;
			}
			default:
				break;
			}
		}

		orig_len = ntohs(iph->tot_len);
		out_iph_id = iph->id;
	}

	if (IsLanSession(p_item->flags)) {

		/*Move to outer IP header and copy the id from original ip header */
		/*calculate the header checksum for outer ip which we inserted*/
		udph = (struct udphdr *)(skb->data+swa->transport_offset);
		udph->len += htons(orig_len);

		out_iph = (struct iphdr *)(skb->data+swa->network_offset);
		out_iph->tot_len = skb->len-swa->network_offset;
		out_iph->id = out_iph_id ? out_iph_id : out_iph->id;
		out_iph->check = 0;
		out_iph->check = ip_fast_csum((unsigned char *)out_iph, out_iph->ihl);

		out_len = out_iph->tot_len;
	} else
		out_len = skb->len;

	/* reset the skb mac, network and transport header */
	skb_set_mac_header(skb, 0);
	skb_set_network_header(skb, swa->network_offset);
	skb_set_transport_header(skb, swa->transport_offset);

	// set the iif
	skb->dev = swa->tx_if;
	skb->skb_iif = skb->dev->ifindex;

	return out_len;
}
#endif

typedef int (*sw_acc_type_fn)(PPA_BUF *skb, struct uc_session_node *p_item);
static sw_acc_type_fn afn_SoftAcceleration[SW_ACC_TYPE_MAX] = {
	sw_mod_ipv4_skb,
	sw_mod_ipv6_skb,
	sw_mod_6rd_skb,
	sw_mod_dslite_skb,
	sw_mod_bridged_skb,
#if IS_ENABLED(CONFIG_PPA_TCP_LITEPATH) || IS_ENABLED(CONFIG_PPA_UDP_LITEPATH)
	sw_mod_ltcp_skb,
#if IS_ENABLED(CONFIG_LTQ_TOE_DRIVER)
	sw_mod_ltcp_skb_lro,
#endif
#endif
#if defined(L2TP_CONFIG) && L2TP_CONFIG
	sw_mod_l2tp_skb,
#endif
	sw_mod_ipip_skb,
	sw_mod_ip6ip6_skb,
};

static int get_time_in_sec(void)
{		 
	return (jiffies + HZ / 2) / HZ;
}

static int swa_get_session_from_skb(PPA_SKBUF *skb, unsigned char pf,
				    struct uc_session_node **pp_item)
{
	skb_reset_network_header((struct sk_buff *)skb);
/*this API will work only if PPA_SKBUF == PPA_BUF*/
	return ppa_find_session_from_skb(skb, pf, pp_item);
}

int32_t ppa_do_sw_acceleration(PPA_SKBUF *skb)
{
	struct flag_header *flg_hdr = NULL, flghdr = {0};
	unsigned int ppa_processed=0;			 
	long int ret=PPA_FAILURE;
	unsigned int data_offset;
	struct uc_session_node *p_item=NULL;
	t_sw_hdr	*swa=NULL;
	unsigned int orighdrlen, totlen=0;
	PPA_SKBUF* skb2;
	unsigned char *data;
	struct pppoe_hdr *ppphdr;
	unsigned int reqHeadRoom;

	if(ppa_is_pkt_loopback(skb)) {
		goto normal_path;
	}

	/* datapath driver marks the packet coming from PPE with this flag in skb->mark */	
	ppa_processed = !!(skb->mark & FLG_PPA_PROCESSED);
	skb->mark &= ~FLG_PPA_PROCESSED;
	if (IS_ENABLED(CONFIG_SOC_GRX500) && ppa_processed) {
		flg_hdr = (struct flag_header *) get_skb_flag_header(skb);
	} else {
	/*
	 * By default we support only packets from interfaces registred with PPA 
	 * which has flag header.
	 * The below code is to handle the packets from directpath interfaces which
	 * are directly passed to software acceleration. 
	 */
		data_offset = ETH_HLEN;
		 
		data = skb->data;	
		if (!set_flag_header(&flghdr, skb->protocol, data, data_offset))
			goto normal_path;
		flg_hdr = &flghdr;
	}

	if (flg_hdr && IsSoftwareAccelerable(flg_hdr)) {
	/* 
	 * skb->data curently pointing to end of mac header
	 * if there is vlan header or pppoe header we need to skip them and 
	 * point to the begining of network header	
	 */
		data_offset = get_ip_inner_offset(flg_hdr) - ETH_HLEN;
		/*
	 	* If IPv4 packet is a fragmented, let the stack process it
	 	*/
		/* NOTE : It may be handled in is_rt_excep flag */
		if( flg_hdr->is_inner_ipv4 && flg_hdr->is_ip_frag )
			goto normal_path;

		if (data_offset > 0)
			if (!skb_pull(skb, data_offset))
				data_offset = 0;
		/* If sessions exist try to accelerate the packet */

		if( PPA_SESSION_EXISTS == swa_get_session_from_skb(skb, get_pf(flg_hdr), &p_item)) {
			/* 
			 * Can the session be accelaratable ? 
			 *	- Session must be added into sotware path 
			 */ 
			if ((IsGreSession(p_item->flag2)) ||
				!(IsSoftwareAccelerated(p_item->flags)) ||
				((skb->len > p_item->mtu)
#if IS_ENABLED(CONFIG_LTQ_TOE_DRIVER)
					&& !(p_item->flag2 & SESSION_FLAG2_LRO) 
#endif
			)) {
				goto skip_accel;
			}

			swa = (t_sw_hdr*)(p_item->session_meta);
			if(!swa) {
				goto skip_accel;
			}
			if(!(p_item->flag2 & SESSION_FLAG2_CPU_BOUND)) { 

				/* If headroom is not enough, increase the headroom */
				orighdrlen = get_ip_inner_offset(flg_hdr);
				orighdrlen += (get_pf(flg_hdr) == PROTO_FAMILY_IP) ? IPV4_HDR_LEN
										   : IPV6_HDR_LEN;
				if (orighdrlen < swa->tot_hdr_len) {

					reqHeadRoom = swa->tot_hdr_len - orighdrlen;
					if (skb_headroom(skb) < reqHeadRoom) {
			
						skb2 = skb_realloc_headroom(skb, reqHeadRoom);
						if (skb2 == NULL) {
							/* Drop the packet */
							PPA_SKB_FREE(skb);
							skb = NULL;
							ret = PPA_SUCCESS; /*Must return success*/
							goto skip_accel;
						}
						if (skb->sk)
							skb_set_owner_w(skb2, skb->sk);
						PPA_SKB_FREE(skb);
						skb = skb2;
					}
				}
			}
			if(swa->type < SW_ACC_TYPE_MAX && !(totlen = afn_SoftAcceleration[swa->type](skb,p_item))) { 
				goto skip_accel;
			} 

			if( !IsBridgedSession(p_item->flag2) &&	
				IsLanSession(p_item->flags) && IsPppoeSession(p_item->flags) ) {
			
				/*update ppp header with length of the ppp payload*/
				ppphdr = (struct pppoe_hdr *)(skb->data + (swa->network_offset - PPPOE_HLEN));
				ppphdr->length = htons(ntohs(totlen) + 2); /* ip payload length + ppp header length*/
			}
		
			/* set the destination dev*/
			skb->dev=swa->tx_if;
			/* Mark update for Software queuing */
			skb->mark = p_item->pkt.mark;
#ifdef CONFIG_NETWORK_EXTMARK
			ppa_set_skb_extmark(skb, p_item->pkt.extmark | FILTERTAP_MASK, U32_MAX);
#endif
			skb->priority = p_item->pkt.priority;
			/* update the packet counter */
			totlen = skb->len;

			/* If CONFIG_IPV4_IPV6_COUNTER_SUPPORT is enabled update packet
			 * in session. Else update the byte count.
			 */
			p_item->host_bytes += totlen;
#if IS_ENABLED(CONFIG_IPV4_IPV6_COUNTER_SUPPORT)
			p_item->acc_bytes += 1;
#else
			p_item->acc_bytes += totlen;
#endif /* CONFIG_IPV4_IPV6_COUNTER_SUPPORT */

			/* update last hit time pf the session */
			p_item->last_hit_time = get_time_in_sec();	

			/* queue the packet for transmit*/
			/*session tx handler */
			swa->tx_handler(skb);
			ppa_session_put(p_item);
			return PPA_SUCCESS;
		}
skip_accel:
		if(p_item)
			ppa_session_put(p_item);

		if( skb && data_offset) 
			skb_push(skb,data_offset);

		return ret;
	}
normal_path:
	return PPA_FAILURE;
}

int32_t sw_fastpath_enable(uint32_t f_enable, uint32_t flags)
{
	g_sw_fastpath_enabled=f_enable;
	return PPA_SUCCESS;
}
		
int32_t get_sw_fastpath_status(uint32_t *f_enable, uint32_t flags)
{
	if( f_enable )
		*f_enable = g_sw_fastpath_enabled;
	return PPA_SUCCESS;
}

#if IS_ENABLED(CONFIG_PPA_TCP_LITEPATH)
int32_t tcp_litepath_enable(uint32_t f_enable, uint32_t flags)
{
	g_tcp_litepath_enabled=f_enable;
	return PPA_SUCCESS;
}
		
int32_t get_tcp_litepath_status(uint32_t *f_enable, uint32_t flags)
{
	if( f_enable )
		*f_enable = g_tcp_litepath_enabled;
	return PPA_SUCCESS;
}
#endif

#if IS_ENABLED(CONFIG_PPA_UDP_LITEPATH)
int32_t udp_litepath_enable(uint32_t f_enable, uint32_t flags)
{
	g_udp_litepath_enabled=f_enable;
	return PPA_SUCCESS;
}
		
int32_t get_udp_litepath_status(uint32_t *f_enable, uint32_t flags)
{
	if( f_enable )
		*f_enable = g_udp_litepath_enabled;
	return PPA_SUCCESS;
}
#endif

int32_t sw_fastpath_send(PPA_SKBUF *skb) 
{
	if (g_sw_fastpath_enabled) {
		return ppa_do_sw_acceleration(skb);
	} 

	return PPA_FAILURE;
}

#if IS_ENABLED(CONFIG_PPA_TCP_LITEPATH) || IS_ENABLED(CONFIG_PPA_UDP_LITEPATH)
static int sw_update_iph(PPA_SKBUF *skb, int* offset, unsigned char *pf)
{ 
	struct rtable *rt = NULL;
	struct iphdr *iph = NULL;
	struct inet_sock *inet = NULL;
	struct sock *sk = NULL;
	struct net *net = NULL;
	char dummy_ip_hdr[] = { 0x45, 0x00, 0x00, 0x00, 0x00, 0x01, 0x40, 0x00,
				0x3f, 0x00, 0x00, 0x00, 0xc0, 0xa8, 0x01, 0x01,
				0xc0, 0xa8, 0x01, 0x02 };

	if (unlikely((skb==NULL || skb->sk==NULL))) {
		pr_err("skb or sk is null..\n");
		return PPA_FAILURE;
	}

	rt = skb_rtable(skb);
	sk = skb->sk;
	net = sock_net(sk);
	inet = inet_sk(sk);

	/* Skip the loopback packets */
	if(inet->inet_saddr==inet->inet_daddr) {
		return PPA_FAILURE;
	}

	*offset = sizeof(struct iphdr);
	skb_push(skb, sizeof(struct iphdr));
	skb_reset_network_header(skb);
	/*This is needed in case the packet is directly sent for HW acceleration*/
	memcpy(skb->data, dummy_ip_hdr, sizeof(dummy_ip_hdr));

	*pf = PF_INET;
	iph = ip_hdr(skb);
	iph->saddr = inet->inet_saddr;
	iph->daddr = inet->inet_daddr;
	iph->protocol = sk->sk_protocol;
	iph->tot_len = htons(skb->len);
	iph->ttl = ip_ttl_default;
	iph->frag_off = htons(IP_DF);
	iph->ihl = 5;
	iph->tos = inet->tos;

	ip_select_ident_segs(net, skb, sk, (skb_shinfo(skb)->gso_segs ?: 1));
	skb->protocol = htons(ETH_P_IP);

	skb->priority = sk->sk_priority;
	skb->mark = sk->sk_mark;
	return PPA_SUCCESS;
}

#if IS_ENABLED(CONFIG_IPV6)
static int inet6_csk_route_socket(struct sock *sk, struct flowi6 *fl6)
{
	struct inet_sock *inet = inet_sk(sk);
	struct ipv6_pinfo *np = inet6_sk(sk);

	if (unlikely(np == NULL)) {
		pr_err("ipv6 info in sock is null!!\n");
		return PPA_FAILURE;
	}
	memset(fl6, 0, sizeof(*fl6));
	fl6->flowi6_proto = sk->sk_protocol;
	fl6->daddr = sk->sk_v6_daddr;
	fl6->saddr = np->saddr;
	fl6->flowlabel = np->flow_label;
	IP6_ECN_flow_xmit(sk, fl6->flowlabel);
	fl6->flowi6_oif = sk->sk_bound_dev_if;
	fl6->flowi6_mark = sk->sk_mark;
	fl6->fl6_sport = inet->inet_sport;
	fl6->fl6_dport = inet->inet_dport;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 10, 0)
	fl6->flowi6_uid = sk->sk_uid;
#endif
	return PPA_SUCCESS;
}

static int sw_update_iph6(PPA_SKBUF *skb, int *offset, unsigned char *pf)
{
	struct ipv6hdr *ip6h = NULL;
	struct ipv6_pinfo *ip6_info = NULL;
	struct flowi6 fl6;
	struct sock *sk = NULL;
	struct net *net = NULL;
	int ret;

	sk = skb->sk;
	ip6_info = inet6_sk(sk);
	if (unlikely(ip6_info == NULL)) {
		pr_err("ipv6 info in sock is null!!\n");
		return PPA_FAILURE;
	}

	ret = inet6_csk_route_socket(sk, &fl6);
	if (ret != PPA_SUCCESS) 
		return ret;

	net = sock_net(sk);
	*offset = sizeof(struct ipv6hdr);
	skb_push(skb, sizeof(struct ipv6hdr));
	skb_reset_network_header(skb);

	*pf = PF_INET6;
	ip6h = ipv6_hdr(skb);
	ip6_flow_hdr(ip6h, ip6_info->tclass,
		     ip6_make_flowlabel(net, skb, fl6.flowlabel, true, &fl6));
	ip6h->payload_len = htons(skb->len - sizeof(struct ipv6hdr));
	ip6h->nexthdr = sk->sk_protocol;
	ip6h->hop_limit = ip6_info->hop_limit;
	ip6h->saddr = ip6_info->saddr;
	ip6h->daddr = sk->sk_v6_daddr;
	skb->protocol = htons(ETH_P_IPV6);
	skb->priority = sk->sk_priority;
	skb->mark = sk->sk_mark;

	return PPA_SUCCESS;
}
#endif /* CONFIG_IPV6 */

static int32_t sw_litepath_send_skb(PPA_SKBUF *skb, uint8_t pf)
{
	int32_t ret = PPA_FAILURE;
	uint32_t totlen = 0;
	struct uc_session_node *p_item = NULL;
	t_sw_hdr *swa = NULL;

	if (swa_get_session_from_skb(skb, pf, &p_item) != PPA_SESSION_EXISTS)
		return PPA_FAILURE;

	if (!IsSoftwareAccelerated(p_item->flags) ||
	    !(p_item->flag2 & SESSION_FLAG2_CPU_OUT))
		goto skip_accel;

	swa = (t_sw_hdr *)p_item->session_meta;
	if (skb_headroom(skb) < swa->network_offset)
		goto skip_accel;

	/* packet modification*/
	if (swa->type < SW_ACC_TYPE_MAX)
		totlen = afn_SoftAcceleration[swa->type](skb, p_item);

	if (!totlen)
		goto skip_accel;

	if (IsPppoeSession(p_item->flags)) {
		struct pppoe_hdr *ppphdr;

		ppphdr = (struct pppoe_hdr *)
			(skb->data + (swa->network_offset - PPPOE_HLEN));
		ppphdr->length = htons(ntohs(totlen) + 2);
	}

	swa_dump_skb(skb->data, 64, 0);

	skb->priority = p_item->pkt.priority;
	skb->mark = p_item->pkt.mark;
#ifdef CONFIG_NETWORK_EXTMARK
	ppa_set_skb_extmark(skb, p_item->pkt.extmark | FILTERTAP_MASK, U32_MAX);
#endif

	p_item->host_bytes += skb->len;
	p_item->acc_bytes += skb->len;
	p_item->last_hit_time = get_time_in_sec();

	skb->dev = swa->tx_if;
	swa->tx_handler(skb);
	ret = PPA_SUCCESS;

skip_accel:
	ppa_session_put(p_item);

	return ret;
}

#if IS_ENABLED(CONFIG_PPA_TCP_LITEPATH)
int32_t sw_litepath_tcp_send_skb(PPA_SKBUF *skb)
{
	int32_t offset = 0;
	unsigned char pf = 0;
	int ret;
	const struct inet_connection_sock *icsk = inet_csk(skb->sk);

#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
	if (!skb->sk->sk_hw_learnt)
		return PPA_FAILURE;
#endif
	
	/*printk(KERN_INFO"\n\n%s %d %px len=%d datalen=%d\n",__FUNCTION__,__LINE__, skb, skb->len, skb->data_len);
	swa_dump_skb(skb->data, 64, 0);*/
	skb_reset_transport_header(skb);
	/* put the dummy ip header */
	if (icsk->icsk_af_ops->queue_xmit == ip_queue_xmit) {
		ret = sw_update_iph(skb, &offset, &pf);
#if IS_ENABLED(CONFIG_IPV6)
	} else if (icsk->icsk_af_ops->queue_xmit == inet6_csk_xmit) {
		ret = sw_update_iph6(skb, &offset, &pf);
#endif
	} else {
		pr_err("[%s:%d] sock is neither IPv4 nor IPv6\n",
		       __func__, __LINE__);
		return PPA_FAILURE;
	}

	if (ret == PPA_SUCCESS) {
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
		/*Call the hardware offload before lookup*/
		if (unlikely(lpdev_tx(skb)))
			goto skip_accel;
		return PPA_SUCCESS;
#endif
		if (!sw_litepath_send_skb(skb, pf))
			return PPA_SUCCESS;
	}
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
skip_accel:
#endif
	if (offset)
		skb_pull(skb, offset);

	return PPA_FAILURE;
}
				 
int32_t sw_litepath_tcp_send(PPA_SKBUF *skb) 
{
	/*In LGM we need to first check whether Litepath HW offload is possible*/
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
	if (g_tcp_litepath_enabled)
		return sw_litepath_tcp_send_skb(skb);
#else
	if (g_sw_fastpath_enabled && g_tcp_litepath_enabled)
		return sw_litepath_tcp_send_skb(skb);
#endif
	return PPA_FAILURE;
}
#endif /* CONFIG_PPA_TCP_LITEPATH */

#if IS_ENABLED(CONFIG_PPA_UDP_LITEPATH)
int32_t sw_litepath_udp_send_skb(PPA_SKBUF *skb)
{
	uint8_t ip_ver;
	int32_t ret = PPA_FAILURE;

	if (skb->sk->sk_hw_learnt) {
		skb->priority = skb->sk->sk_priority;
		skb->mark = skb->sk->sk_mark;
		ip_ver = ip_hdr(skb)->version;
		if (ip_ver == 4) {
			ip_hdr(skb)->tot_len = htons(skb->len);
			skb->protocol = htons(ETH_P_IP);
		}
#if IS_ENABLED(CONFIG_IPV6)
		else if (ip_ver == 6) {
			ipv6_hdr(skb)->payload_len =
				htons(skb->len - sizeof(struct ipv6hdr));
			skb->protocol = htons(ETH_P_IPV6);
		}
#endif
		else {
			ppa_debug(DBG_ENABLE_MASK_ERR,
				  "%s %d: Unsupported ip version 0x%x!",
				  __func__, __LINE__, ip_ver);
			return PPA_FAILURE;
		}
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
		/* Call the hardware offload now */
		if (lpdev_tx(skb) == PPA_SUCCESS)
			return PPA_SUCCESS;
#else
	} else {
		uint8_t pf = 0;

		ip_ver = ip_hdr(skb)->version;
		if (ip_ver == 6) {
			skb->protocol = htons(ETH_P_IPV6);
			pf = PF_INET6;
		} else {
			ip_hdr(skb)->tot_len = htons(skb->len);
			skb->protocol = htons(ETH_P_IP);
			pf = PF_INET;
		}
		ret = sw_litepath_send_skb(skb, pf);
#endif
	}

	return ret;
}

int32_t sw_litepath_udp_send(PPA_SKBUF *skb)
{
	/*In LGM we need to first check whether Litepath HW offload is possible*/
	if (g_udp_litepath_enabled)
		return sw_litepath_udp_send_skb(skb);

	return PPA_FAILURE;
}
#endif /* CONFIG_PPA_UDP_LITEPATH */
#endif /* CONFIG_PPA_TCP_LITEPATH || CONFIG_PPA_UDP_LITEPATH */

int32_t sw_fastpath_send(PPA_SKBUF *skb);
int32_t get_sw_fastpath_status(uint32_t *f_enable, uint32_t flags);
int32_t sw_fastpath_enable(uint32_t f_enable, uint32_t flags);
#if IS_ENABLED(CONFIG_PPA_TCP_LITEPATH)
int32_t tcp_litepath_enable(uint32_t f_enable, uint32_t flags);
int32_t get_tcp_litepath_status(uint32_t *f_enable, uint32_t flags);
#endif

static void get_hal_stats(PPA_HAL_STATS *stat)
{
	unsigned int cpu;
	const PPA_HAL_STATS *rt;

	memset(stat, 0, sizeof(PPA_HAL_STATS));
	stat->max_uc_session = -1;
	stat->max_mc_session = -1;
	stat->max_uc_ipv4_session = -1;
	stat->max_uc_ipv6_session = -1;
	for_each_possible_cpu(cpu) {
		rt = per_cpu_ptr(&rtstats, cpu);
		stat->curr_uc_ipv4_session += rt->curr_uc_ipv4_session;
		stat->curr_uc_ipv6_session += rt->curr_uc_ipv6_session;
		stat->uc_dropped_sess += rt->uc_dropped_sess;
	}
}

static int32_t swac_hal_generic_hook(PPA_GENERIC_HOOK_CMD cmd, void *buffer, uint32_t flag)
{
	uint32_t res = PPA_SUCCESS;	

	switch (cmd) {
	case PPA_GENERIC_HAL_INIT: /*init HAL*/ {
		res = ppa_drv_register_cap(SESS_IPV4, 4, SWAC_HAL);
		if (res != PPA_SUCCESS)
			pr_err("ppa_drv_register_cap returned failure for capability SESS_IPV4!!!\n");	

		res = ppa_drv_register_cap(SESS_IPV6, 4, SWAC_HAL);
		if (res != PPA_SUCCESS)
			pr_err("ppa_drv_register_cap returned failure for capability SESS_IPV6!!!\n");	

		res = ppa_drv_register_cap(SESS_LOCAL_IN, 4, SWAC_HAL);
		if (res != PPA_SUCCESS)
			pr_err("ppa_drv_register_cap returned failure for capability SESS_LOCAL_IN!!!\n");

		res = ppa_drv_register_cap(SESS_LOCAL_OUT, 4, SWAC_HAL);
		if (res != PPA_SUCCESS)
			pr_err("ppa_drv_register_cap returned failure for capability SESS_LOCAL_OUT!!!\n");

		res = ppa_drv_register_cap(TUNNEL_6RD, 4, SWAC_HAL);
		if (res != PPA_SUCCESS)
			pr_err("ppa_drv_register_cap returned failure for capability TUNNEL_6RD!!!\n");

		res = ppa_drv_register_cap(TUNNEL_DSLITE, 4, SWAC_HAL);
		if (res != PPA_SUCCESS)
			pr_err("ppa_drv_register_cap returned failure for capability TUNNEL_DSLITE!!!\n");

		res = ppa_drv_register_cap(TUNNEL_L2TP_US, 4, SWAC_HAL);
		if (res != PPA_SUCCESS)
			pr_err("ppa_drv_register_cap returned failure for capability TUNNEL_L2TP_US!!!\n");

		res = ppa_drv_register_cap(TUNNEL_L2TP_DS, 4, SWAC_HAL);
		if (res != PPA_SUCCESS)
			pr_err("ppa_drv_register_cap returned failure for capability TUNNEL_L2TP_DS!!!\n");

		printk("SWAC Init Success\n");
		return res;
	}
	case PPA_GENERIC_HAL_EXIT: /*EXIT HAL*/ {
		ppa_drv_deregister_cap(SESS_IPV4, SWAC_HAL);
		ppa_drv_deregister_cap(SESS_IPV6, SWAC_HAL);
		ppa_drv_deregister_cap(SESS_LOCAL_IN, SWAC_HAL);
		ppa_drv_deregister_cap(SESS_LOCAL_OUT, SWAC_HAL);
		ppa_drv_deregister_cap(TUNNEL_6RD, SWAC_HAL);
		ppa_drv_deregister_cap(TUNNEL_DSLITE, SWAC_HAL);
		ppa_drv_deregister_cap(TUNNEL_L2TP_US, SWAC_HAL);
		ppa_drv_deregister_cap(TUNNEL_L2TP_DS, SWAC_HAL);
		return res;
	} 
	case PPA_GENERIC_HAL_UPDATE_SESS_META: {
		if(g_sw_fastpath_enabled) {
			PPA_SESSMETA_INFO *metainfo = (PPA_SESSMETA_INFO*)buffer;
			if(!metainfo) return PPA_FAILURE;
			ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d\n", __FUNCTION__, __LINE__);
			return swac_update_session_meta(metainfo);
		} else { 
			return PPA_FAILURE;
		}
	}
	case PPA_GENERIC_HAL_CLEAR_SESS_META: {
		struct uc_session_node *p_item = (struct uc_session_node *)buffer;
		if(!p_item) return PPA_FAILURE;
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d\n", __FUNCTION__, __LINE__);
		del_swah(p_item); 
		return PPA_SUCCESS;
	} 
	case PPA_GENERIC_HAL_ADD_ROUTE_ENTRY: {
		if(g_sw_fastpath_enabled) {
			PPA_ROUTING_INFO *route=(PPA_ROUTING_INFO *)buffer;
			if(!route) return PPA_FAILURE;
			ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d\n", __FUNCTION__, __LINE__);
			return swac_add_routing_entry(route);
		} else {
			return PPA_FAILURE;
		}
	}
	case PPA_GENERIC_HAL_DEL_ROUTE_ENTRY: {
		PPA_ROUTING_INFO *route=(PPA_ROUTING_INFO *)buffer;
		if(!route) return PPA_FAILURE;
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT,"%s %d\n", __FUNCTION__, __LINE__);
		swac_del_routing_entry(route);
		return PPA_SUCCESS;
	}
    case PPA_GENERIC_HAL_SET_ACC_ENABLE: {
			PPA_ACC_ENABLE *cfg = (PPA_ACC_ENABLE *)buffer;
			if (!cfg)
				return PPA_FAILURE;

			if (cfg->f_enable == PPA_ACC_MODE_NONE)
				sw_fastpath_enable(0, 0);
			else
				sw_fastpath_enable(1, 0);

			return PPA_SUCCESS;
	}
    case PPA_GENERIC_HAL_GET_ACC_ENABLE: {
			PPA_ACC_ENABLE *cfg = (PPA_ACC_ENABLE *)buffer;
			if (!cfg)
				return PPA_FAILURE;

			get_sw_fastpath_status(&cfg->f_enable, 0);

			return PPA_SUCCESS;
	}

	case PPA_GENERIC_HAL_GET_STATS:{
			PPA_HAL_STATS *stat = (PPA_HAL_STATS *)buffer;

			if (!stat)
				return PPA_FAILURE;

			get_hal_stats(stat);

			return PPA_SUCCESS;
	}
	default:
		return PPA_FAILURE;
	}
	return res;
}

static void swac_register_hal(void)
{
	/*printk("Register SWACC HAL to PPA.\n");*/
	ppa_drv_generic_hal_register(SWAC_HAL, swac_hal_generic_hook);
}

int	ppa_sw_init(void)
{
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
	struct pp_version ver;
#endif
	ppa_sw_fastpath_enable_hook = sw_fastpath_enable;
	ppa_get_sw_fastpath_status_hook = get_sw_fastpath_status;
	ppa_sw_fastpath_send_hook = sw_fastpath_send;
#if IS_ENABLED(CONFIG_PPA_TCP_LITEPATH)
	ppa_tcp_litepath_enable_hook = tcp_litepath_enable;
	ppa_get_tcp_litepath_status_hook = get_tcp_litepath_status;
	ppa_sw_litepath_tcp_send_hook = sw_litepath_tcp_send; 
	g_tcp_litepath_enabled = 1;
#endif
#if IS_ENABLED(CONFIG_X86_INTEL_LGM) || IS_ENABLED(CONFIG_SOC_LGM)
	if (unlikely(pp_version_get(&ver, PP_VER_TYPE_HW))) {
		pr_err("Failed to get PP version\n");
		return PPA_FAILURE;
	} else {
		pp_hw_version = ver.major;
	}
#endif
#if IS_ENABLED(CONFIG_PPA_UDP_LITEPATH)
	ppa_udp_litepath_enable_hook = udp_litepath_enable;
	ppa_get_udp_litepath_status_hook = get_udp_litepath_status;
	ppa_sw_litepath_udp_send_hook = sw_litepath_udp_send;
	g_udp_litepath_enabled = 1;
#endif
	swac_register_hal();

	g_sw_fastpath_enabled = 1;
	return 0;
}

void ppa_sw_exit(void)
{
	ppa_sw_fastpath_enable_hook = NULL;
	ppa_get_sw_fastpath_status_hook = NULL;
	ppa_sw_fastpath_send_hook = NULL;
#if IS_ENABLED(CONFIG_PPA_TCP_LITEPATH)
	ppa_sw_litepath_tcp_send_hook = NULL; 
	ppa_tcp_litepath_enable_hook = NULL;
	ppa_get_tcp_litepath_status_hook = NULL;
	g_tcp_litepath_enabled = 0;
#endif
#if IS_ENABLED(CONFIG_PPA_UDP_LITEPATH)
	ppa_udp_litepath_enable_hook = NULL;
	ppa_get_udp_litepath_status_hook = NULL;
	ppa_sw_litepath_udp_send_hook = NULL;
	g_udp_litepath_enabled = 0;
#endif
	ppa_drv_generic_hal_deregister(SWAC_HAL);
	g_sw_fastpath_enabled = 0;
}

