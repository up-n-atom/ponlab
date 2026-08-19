// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2025, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 * Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 * Copyright 2012 - 2014 Lantiq Deutschland GmbH
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <linux/types.h>
#include <linux/ip.h>
#include <linux/kernel.h>
#include <net/ip.h>
#include <net/datapath_api.h>
#include <net/datapath_proc_api.h>
#include "datapath.h"
#include "hal/datapath_misc.h"
#if IS_ENABLED(CONFIG_PPA_API_SW_FASTPATH) || \
	IS_ENABLED(CONFIG_LTQ_PPA_API_SW_FASTPATH)
#include <net/ppa/ppa_api.h>
#endif

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

#define BUF_SIZE 2000
struct hlist_head dp_subif_list[DP_SUBIF_LIST_HASH_SIZE];
static struct kmem_cache *cache_subif_list;

char *parser_flags_str[] = {
	"PARSER_FLAGS_NO",
	"PARSER_FLAGS_END",
	"PARSER_FLAGS_CAPWAP",
	"PARSER_FLAGS_GRE",
	"PARSER_FLAGS_LEN",
	"PARSER_FLAGS_GREK",
	"PARSER_FLAGS_NN1",
	"PARSER_FLAGS_NN2",

	"PARSER_FLAGS_ITAG",
	"PARSER_FLAGS_1VLAN",
	"PARSER_FLAGS_2VLAN",
	"PARSER_FLAGS_3VLAN",
	"PARSER_FLAGS_4VLAN",
	"PARSER_FLAGS_SNAP",
	"PARSER_FLAGS_PPPOES",
	"PARSER_FLAGS_1IPV4",

	"PARSER_FLAGS_1IPV6",
	"PARSER_FLAGS_2IPV4",
	"PARSER_FLAGS_2IPV6",
	"PARSER_FLAGS_ROUTEXP",
	"PARSER_FLAGS_TCP",
	"PARSER_FLAGS_1UDP",
	"PARSER_FLAGS_IGMP",
	"PARSER_FLAGS_IPV4OPT",

	"PARSER_FLAGS_IPV6EXT",
	"PARSER_FLAGS_TCPACK",
	"PARSER_FLAGS_IPFRAG",
	"PARSER_FLAGS_EAPOL",
	"PARSER_FLAGS_2IPV6EXT",
	"PARSER_FLAGS_2UDP",
	"PARSER_FLAGS_L2TPNEXP",
	"PARSER_FLAGS_LROEXP",

	"PARSER_FLAGS_L2TP",
	"PARSER_FLAGS_GRE_VLAN1",
	"PARSER_FLAGS_GRE_VLAN2",
	"PARSER_FLAGS_GRE_PPPOE",
	"PARSER_FLAGS_BYTE4_BIT4_UNDEF",
	"PARSER_FLAGS_BYTE4_BIT5_UNDEF",
	"PARSER_FLAGS_BYTE4_BIT6_UNDEF",
	"PARSER_FLAGS_BYTE4_BIT7_UNDEF",

	"PARSER_FLAGS_BYTE5_BIT0_UNDEF",
	"PARSER_FLAGS_BYTE5_BIT1_UNDEF",
	"PARSER_FLAGS_BYTE5_BIT2_UNDEF",
	"PARSER_FLAGS_BYTE5_BIT3_UNDEF",
	"PARSER_FLAGS_BYTE5_BIT4_UNDEF",
	"PARSER_FLAGS_BYTE5_BIT5_UNDEF",
	"PARSER_FLAGS_BYTE5_BIT6_UNDEF",
	"PARSER_FLAGS_BYTE5_BIT7_UNDEF",

	"PARSER_FLAGS_BYTE6_BIT0_UNDEF",
	"PARSER_FLAGS_BYTE6_BIT1_UNDEF",
	"PARSER_FLAGS_BYTE6_BIT2_UNDEF",
	"PARSER_FLAGS_BYTE6_BIT3_UNDEF",
	"PARSER_FLAGS_BYTE6_BIT4_UNDEF",
	"PARSER_FLAGS_BYTE6_BIT5_UNDEF",
	"PARSER_FLAGS_BYTE6_BIT6_UNDEF",
	"PARSER_FLAGS_BYTE6_BIT7_UNDEF",

	"PARSER_FLAGS_BYTE7_BIT0_UNDEF",
	"PARSER_FLAGS_BYTE7_BIT1_UNDEF",
	"PARSER_FLAGS_BYTE7_BIT2_UNDEF",
	"PARSER_FLAGS_BYTE7_BIT3_UNDEF",
	"PARSER_FLAGS_BYTE7_BIT4_UNDEF",
	"PARSER_FLAGS_BYTE7_BIT5_UNDEF",
	"PARSER_FLAGS_BYTE7_BIT6_UNDEF",
	"PARSER_FLAGS_BYTE7_BIT7_UNDEF",

	/*Must be put at the end of the enum */
	"PARSER_FLAGS_MAX"
};

void dump_parser_flag(char *buf)
{
	int i, j;
	unsigned char *pflags = buf + PKT_PMAC_OFFSET - 1;
	__maybe_unused unsigned char *poffset = buf;
	char *p;
	int len;

	if (!buf) {
		pr_err("DPM: %s buf NULL\n", __func__);
		return;
	}
	p = dp_kzalloc(BUF_SIZE, GFP_ATOMIC);
	if (!p)
		return;

	/* one TCP example: offset
	 * offset 0
	 *  00 3a 00 00 00 00 00 00 00 00 00 00 00 00 00 0e
	 * 00 00 00 16 22 00 00 00 00 00 00 00 00 00 00 2e
	 * 00 00 00 00 00 00 00 00
	 * flags: FLAG_L2TPFLAG_NO
	 * 00 00 00 00 80 18 80 00
	 */
	DP_DUMP("paser flag at 0x%px: ", buf);
	len = 0;
	for (i = 0; i < 8; i++)
		len += snprintf(p + len, (BUF_SIZE - len), "%02x ", *(pflags - 7 + i));
	DP_DUMP("%s\n", p);
	DP_DUMP("parser flag: ");
	len = 0;
	for (i = 0; i < 8; i++)
		len += snprintf(p + len, (BUF_SIZE - len), "%02x ", *(pflags - i));
	DP_DUMP("%s(reverse)\n", p);

	for (i = 0; i < PASAR_FLAGS_NUM; i++) {	/*8 flags per byte */
		for (j = 0; j < 8; j++) {	/*8 bits per byte */
			if ((i * 8 + j) >= PASER_FLAGS_MAX)
				break;

			if ((*(pflags - i)) & (1 << j)) {	/*flag is set */
				if ((i * 8 + j) < PASAR_OFFSETS_NUM)
					DP_DUMP("  Flag %02d offset=%02d: %s\n",
						 i * 8 + j,
						 *(poffset + i * 8 + j),
						 parser_flags_str[i * 8 + j]);
				else
					DP_DUMP("  Flag %02d %s (No offset)\n",
						 i * 8 + j,
						 parser_flags_str[i * 8 + j]);
			}
		}
	}
	kfree(p);
}

/*will be used at any context */
void dp_dump_raw_data(const void *buf, int len, char *prefix_str)
{
	if (!buf)
		return;

	DP_DUMP("%s in hex at 0x%px\n",
		prefix_str ? (char *)prefix_str : "Data", buf);

	print_hex_dump(KERN_INFO, "", DUMP_PREFIX_OFFSET, 16, 1, buf, len, false);
}
EXPORT_SYMBOL(dp_dump_raw_data);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
/* parse packet and get ip_offset/tcp_h_offset/type based on skb kernel APIs
 * return: 0 - found udp/tcp header, -1 - not found or not supported
 *   Note: skb->data points to pmac header, not L2 MAC header
 */
int ip_offset_hw_adjust;

int get_offset_clear_chksum(struct sk_buff *skb, u32 *ip_offset,
			    u32 *tcp_h_offset, u32 *tcp_type)
{
	struct iphdr *iph;
	struct ipv6hdr *ip6h;

	if (skb->encapsulation) {
		if (skb->inner_network_header) {
			iph = inner_ip_hdr(skb);
			if (iph && iph->version == 4) {
				goto ipv4;
			}
			else if (iph && iph->version == 6) {
				ip6h = inner_ipv6_hdr(skb);
				goto ipv6;
			}
		}
	} else {
		iph = ip_hdr(skb);
		if(iph && iph->version == 4)
			goto ipv4;
		else if(iph && iph->version == 6)  {
			ip6h = ipv6_hdr(skb);
			goto ipv6;
		}
	}
	return -1;
ipv4:
	if (ip_is_fragment(iph))
		return -1;
	if (iph->protocol == IPPROTO_TCP)
		*tcp_type = TCP_OVER_IPV4;
	else if (iph->protocol == IPPROTO_UDP)
		*tcp_type = UDP_OVER_IPV4;
	else
		return -1;
	goto out;
ipv6:
	if (ip6h->nexthdr == NEXTHDR_TCP)
		*tcp_type = TCP_OVER_IPV6;
	else if (ip6h->nexthdr == NEXTHDR_UDP)
		*tcp_type = UDP_OVER_IPV6;
	else
		return -1;
out:
	if (skb->encapsulation) {
		*ip_offset = (u32)skb_inner_network_offset(skb);
		*tcp_h_offset = skb_inner_network_header_len(skb);
	} else {
		*ip_offset = (u32)skb_network_offset(skb);
		*tcp_h_offset = skb_network_header_len(skb);
	}
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG_PROTOCOL_PARSE)
	DP_DEBUG(DP_DBG_FLAG_DUMP_TX_SUM,
		 "%s: tcp_type=%u ip_offset=%u tcp_h_offset=%u encap=%s\n",
		 __func__, *tcp_type, *ip_offset, *tcp_h_offset,
		 skb->encapsulation ? "yes" : "no");
#endif
	return 0;
}

#else /* CONFIG_DPM_DATAPATH_HAL_GSWIP31 or GRX500 */

#define PROTOCOL_IPIP 4
#define PROTOCOL_TCP 6
#define PROTOCOL_UDP 17
#define PROTOCOL_ENCAPSULATED_IPV6 41
#define PROTOCOL_ROUTING 43
#define PROTOCOL_NONE 59
#define PROTOCOL_IPV6_FRAGMENT 44

#define TWO_MAC_SIZE 12
#define VLAN_HDR_SIZE  4
#define PPPOE_HDR_SIZE  8
#define IPV6_HDR_SIZE  40
#define IPV6_EXTENSION_SIZE 8

#define IP_CHKSUM_OFFSET_IPV4 10
#define UDP_CHKSUM_OFFSET 6
#define TCP_CHKSUM_OFFSET 16
/*Workaround: Currently need to includes PMAC
 *although spec said it starts from mac address. ?
 */
struct ip_hdr_info {
	u8 ip_ver;
	u8 proto;		/*udp/tcp */
	u16 ip_offset;		/*this offset is based on L2 MAC header */
	u16 udp_tcp_offset;	/*this offset is based on ip header */
	u16 next_ip_hdr_offset;	/*0 - means no next valid ip header.*/
	/* Based on current IP header */
	u8 is_fragment;		/*0 means non fragmented packet */
};

/*input p: pointers to ip header
 * output info:
 * return: 0:  it is UDP/TCP packet
 * -1: not UDP/TCP
 */
#define DP_IP_VER4 4
#define DP_IP_VER6 6
int get_ip_hdr_info(u8 *pdata, int len, struct ip_hdr_info *info)
{
	int ip_hdr_size;
	u8 *p = pdata;
	struct iphdr *iphdr = (struct iphdr *)pdata;

	dp_memset((void *)info, 0, sizeof(*info));
	info->ip_ver = p[0] >> 4;

	if (info->ip_ver == DP_IP_VER4) {	/*ipv4 */
		ip_hdr_size = (p[0] & 0xf) << 2;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG_PROTOCOL_PARSE)
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
			 "IPV4 pkt with protocol 0x%x with ip hdr size %d\n",
			 p[9], ip_hdr_size);
#endif
		info->proto = p[9];

		if ((info->proto == PROTOCOL_UDP) ||
		    (info->proto == PROTOCOL_TCP)) {
			if ((iphdr->frag_off & IP_MF) ||
			    (iphdr->frag_off & IP_OFFSET)) {
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG_PROTOCOL_PARSE)
				DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
					 "frag pkt:off=%x,IP_MF=%x,IP_OFFSET=%x\n",
					 iphdr->frag_off, IP_MF, IP_OFFSET);
#endif
				info->udp_tcp_offset = (p[0] & 0x0f) << 2;
				info->is_fragment = 1;
				return -1;
			}
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG_PROTOCOL_PARSE)
			DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
				 "%s packet with src/dst port:%u/%u\n",
				 (p[9] ==
				  PROTOCOL_UDP) ? "UDP" : "TCP",
				 *(unsigned short *)(pdata +
						     ip_hdr_size),
				 *(unsigned short *)(pdata +
						     ip_hdr_size +
						     2));
#endif
			info->udp_tcp_offset = (p[0] & 0x0f) << 2;
			return 0;
		} else if (p[9] == PROTOCOL_ENCAPSULATED_IPV6) {
			/*6RD */
			info->next_ip_hdr_offset = (p[0] & 0x0f) << 2;
			return 0;
		}
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG_PROTOCOL_PARSE)
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
			 "Not supported extension hdr:0x%x\n", p[9]);
#endif
		return -1;
	} else if (info->ip_ver == DP_IP_VER6) {	/*ipv6 */
		int ip_hdr_size;
		u8 next_hdr;
		u8 udp_tcp_h_offset;
		u8 first_extension = 1;

		ip_hdr_size = IPV6_HDR_SIZE;
		udp_tcp_h_offset = IPV6_HDR_SIZE;
		next_hdr = p[6];
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
		if (dp_dbg_flag & DP_DBG_FLAG_DUMP_TX) {
			int i;

			DP_DUMP("IPV6 packet with next hdr:0x%x\n", next_hdr);
			DP_DUMP(" src IP: ");
			for (i = 0; i < 16; i++)
				DP_DUMP("%02x%s", pdata[8 + i],
					 (i != 15) ? ":" : " ");

			DP_DUMP("\n");

			DP_DUMP(" Dst IP: ");

			for (i = 0; i < 16; i++)
				DP_DUMP("%02x%s", pdata[24 + i],
					 (i != 15) ? ":" : " ");

			DP_DUMP("\n");
		}
#endif
		while (1) {
			/*Next Header: UDP/TCP */
			if ((next_hdr == PROTOCOL_UDP) ||
			    (next_hdr == PROTOCOL_TCP)) {
				info->proto = next_hdr;

				if (!first_extension)
					udp_tcp_h_offset +=
						IPV6_EXTENSION_SIZE + p[1];

				info->udp_tcp_offset = udp_tcp_h_offset;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG_PROTOCOL_PARSE)
				DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
					 "IP6 UDP:src/dst port:%u/%u udp_tcp_off=%d\n",
					 *(unsigned short *)(pdata +
							     udp_tcp_h_offset),
					 *(unsigned short *)(pdata +
							     udp_tcp_h_offset
							     + 2),
					 udp_tcp_h_offset);
#endif
				return 0;
			} else if (next_hdr == PROTOCOL_IPIP) {	/*dslite */
				if (!first_extension)
					udp_tcp_h_offset +=
						IPV6_EXTENSION_SIZE + p[1];

				info->next_ip_hdr_offset = udp_tcp_h_offset;
				return 0;
			} else if (next_hdr == PROTOCOL_IPV6_FRAGMENT) {
				info->is_fragment = 1;
				return -1;
			}
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG_PROTOCOL_PARSE)
			DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
				 "Skip extension hdr:0x%x\n", next_hdr);
#endif
			if ((next_hdr == PROTOCOL_NONE) ||
			    (next_hdr == PROTOCOL_ENCAPSULATED_IPV6))
				break;

			if (first_extension) {
				/*skip ip header */
				p += IPV6_HDR_SIZE;
				first_extension = 0;
			} else {
				/*TO NEXT */
				udp_tcp_h_offset +=
					IPV6_EXTENSION_SIZE + p[1];
				p += IPV6_EXTENSION_SIZE + p[1];
			}
			next_hdr = p[0];
			if (udp_tcp_h_offset > len) {
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG_PROTOCOL_PARSE)
				DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
					 "\n- Wrong IPV6 packet header ?\n");
#endif
				break;
			}
		}
	}

	/*not supported protocol */
	return -1;
}

/* parse protol and get the ip_offset/tcp_h_offset and its type
 * based on skb_inner_network_header/skb_network_header/
 *           skb_inner_transport_header/skb_transport_header
 * return: 0-found udp/tcp header, -1 - not found  udp/tcp header
 *  Note: skb->data points to pmac header, not L2 MAC header;
 */
int ip_offset_hw_adjust;

int get_offset_clear_chksum(struct sk_buff *skb, u32 *ip_offset,
			    u32 *tcp_h_offset, u16 *l3_csum_off,
			    u16 *l4_csum_off, u32 *tcp_type)
{
	struct iphdr *iph;
	struct tcphdr *tcph;
	struct udphdr *udph;
	unsigned char *l4_p;

	if (skb->ip_summed != CHECKSUM_PARTIAL) {
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG_PROTOCOL_PARSE)
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX_SUM,
			 "No need HW checksum Support\n");
#endif
		return -1;
	}

	if (skb->encapsulation) {
		iph = (struct iphdr *)skb_inner_network_header(skb);
		*ip_offset =
			(uint32_t)(skb_inner_network_header(skb) - skb->data);
		*tcp_h_offset =
			(uint32_t)(skb_inner_transport_header(skb) -
				   skb_inner_network_header(skb));
		l4_p = skb_inner_transport_header(skb);
	} else {
		iph = (struct iphdr *)skb_network_header(skb);
		*ip_offset = (uint32_t)(skb_network_header(skb) - skb->data);
		*tcp_h_offset =
			(uint32_t)(skb_transport_header(skb) -
				   skb_network_header(skb));
		l4_p = skb_transport_header(skb);
	}
	if (((int)(*ip_offset) <= 0) || ((int)(*tcp_h_offset) <= 0)) {
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG_PROTOCOL_PARSE)
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX_SUM,
			 "Wrong IP offset(%d) <= 0 OR TCP/UDP offset(%d) <= 0\n",
			 (int)(*ip_offset), (int)(*tcp_h_offset));
#endif
		return -1;
	}

	if (iph->protocol == IPPROTO_UDP) {
		if (iph->version == DP_IP_VER4) {
			*tcp_type = UDP_OVER_IPV4;
			iph->check = 0;	/*clear original ip checksum */
		} else if (iph->version == DP_IP_VER6) {
			*tcp_type = UDP_OVER_IPV6;
		} else { /*wrong ver*/
			return -1;
		}
		udph = (struct udphdr *)l4_p;
		udph->check = 0; /*clear original UDP checksum */
	} else if (iph->protocol == IPPROTO_TCP) {
		if (iph->version == DP_IP_VER4) {
			*tcp_type = TCP_OVER_IPV4;
			iph->check = 0;	/*clear original ip checksum */
		} else if (iph->version == DP_IP_VER6) {
			*tcp_type = TCP_OVER_IPV6;
		} else {
			return -1;
		}
		tcph = (struct tcphdr *)l4_p;
		tcph->check = 0;	/*clear original UDP checksum */
	}
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG_PROTOCOL_PARSE)
	DP_DEBUG(DP_DBG_FLAG_DUMP_TX_SUM, "Found tcp_type=%u ip_offset=%u\n",
		 *tcp_type, *ip_offset);
#endif
	return 0;
}
#endif

char *dp_skb_csum_str(struct sk_buff *skb)
{
	if (!skb)
		return "NULL";
	if (skb->ip_summed == CHECKSUM_PARTIAL)
		return "HW Checksum";
	if (skb->ip_summed == CHECKSUM_NONE)
		return "SW Checksum";
	return "Unknown";
}

int low_10dec(u64 x)
{
	char buf[26];
	char *p;
	int len;

	snprintf(buf, sizeof(buf), "%llu", x);
	len = strlen(buf);
	if (len >= 10)
		p = buf + len - 10;
	else
		p = buf;

	return dp_atoi(p);
}

int high_10dec(u64 x)
{
	char buf[26];
	int len;

	snprintf(buf, sizeof(buf), "%llu", x);
	len = strlen(buf);
	if (len >= 10)
		buf[len - 10] = 0;
	else
		buf[0] = 0;

	return dp_atoi(buf);
}

int dp_ingress_ctp_tc_map_set2(struct dp_tc_cfg *tc, int flag)
{
	struct dp_meter_subif *mtr_subif;
	struct inst_info *dp_info;
	int res;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!tc) {
		pr_err("DPM: %s: tc_cfg is NULL\n", __func__);
		return DP_FAILURE;
	}

	mtr_subif = dp_kzalloc(sizeof(*mtr_subif), GFP_ATOMIC);
	if (!mtr_subif)
		return DP_FAILURE;

	if (dp_get_netif_subifid(tc->dev, NULL, NULL, NULL,
				 &mtr_subif->subif, 0)) {
		DP_DEBUG(DP_DBG_FLAG_DBG, "get subifid fail(%s)\n",
			 tc->dev ? tc->dev->name : "NULL");
		kfree(mtr_subif);
		return DP_FAILURE;
	}
	mtr_subif->inst =  mtr_subif->subif.inst;
	dp_info = get_dp_prop_info(mtr_subif->inst);

	if (!dp_info->dp_ctp_tc_map_set) {
		kfree(mtr_subif);
		return DP_FAILURE;
	}
	res = dp_info->dp_ctp_tc_map_set(tc, flag, mtr_subif);
	kfree(mtr_subif);
	return res;
}
EXPORT_SYMBOL(dp_ingress_ctp_tc_map_set2);

int dp_meter_alloc2(int inst, int *meterid, int flag)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!meterid || is_invalid_inst(inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(inst);

	if (!dp_info->dp_meter_alloc)
		return DP_FAILURE;

	return dp_info->dp_meter_alloc(inst, meterid, flag);
}
EXPORT_SYMBOL(dp_meter_alloc2);

int dp_meter_add2(struct net_device *dev, struct dp_meter_cfg *meter, int flag)
{
	struct dp_meter_subif *mtr_subif;
	struct inst_info *dp_info;
	int res;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!dev || !meter) {
		pr_err("DPM: %s failed: dev or meter_cfg can not be NULL\n", __func__);
		return DP_FAILURE;
	}

	mtr_subif = dp_kzalloc(sizeof(*mtr_subif), GFP_ATOMIC);
	if (!mtr_subif)
		return DP_FAILURE;
	if ((flag & DP_METER_ATTACH_CTP) ||
	    (flag & DP_METER_ATTACH_BRPORT) ||
	    (flag & DP_METER_ATTACH_PCE)) {
		if (dp_get_netif_subifid(dev, NULL, NULL,
					 NULL, &mtr_subif->subif, 0)) {
			DP_DEBUG(DP_DBG_FLAG_DBG,
				 "get subifid fail:%s\n",
				 dev ? dev->name : "NULL");
			kfree(mtr_subif);
			return DP_FAILURE;
		}
		mtr_subif->inst =  mtr_subif->subif.inst;
	} else if (flag & DP_METER_ATTACH_BRIDGE) {
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
		mtr_subif->fid = dp_get_fid_by_dev(dev, &mtr_subif->inst);
		if (mtr_subif->fid < 0) {
			pr_err("DPM: fid less then 0\n");
			kfree(mtr_subif);
			return DP_FAILURE;
		}
#endif
	} else {
		pr_err("DPM: Meter Flag not set\n");
		kfree(mtr_subif);
		return DP_FAILURE;
	}

	dp_info = get_dp_prop_info(mtr_subif->inst);

	if (!dp_info->dp_meter_add) {
		kfree(mtr_subif);
		return DP_FAILURE;
	}
	res = dp_info->dp_meter_add(dev, meter, flag, mtr_subif);
	kfree(mtr_subif);
	return res;
}
EXPORT_SYMBOL(dp_meter_add2);

int dp_meter_del2(struct net_device *dev, struct dp_meter_cfg *meter, int flag)
{
	struct dp_meter_subif *mtr_subif;
	struct inst_info *dp_info;
	int res;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!dev || !meter) {
		pr_err("DPM: %s failed: dev or meter_cfg can not be NULL\n", __func__);
		return DP_FAILURE;
	}

	mtr_subif = dp_kzalloc(sizeof(*mtr_subif), GFP_ATOMIC);
	if (!mtr_subif)
		return DP_FAILURE;

	if ((flag & DP_METER_ATTACH_CTP) ||
	    (flag & DP_METER_ATTACH_BRPORT) ||
	    (flag & DP_METER_ATTACH_PCE)) {
		if (dp_get_netif_subifid(dev, NULL, NULL,
					 NULL, &mtr_subif->subif, 0)) {
			DP_DEBUG(DP_DBG_FLAG_DBG,
				 "get subifid fail:%s\n",
				 dev ? dev->name : "NULL");
			kfree(mtr_subif);
			return DP_FAILURE;
		}
		mtr_subif->inst = mtr_subif->subif.inst;
	} else if (flag & DP_METER_ATTACH_BRIDGE) {
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV)
		mtr_subif->fid = dp_get_fid_by_dev(dev, &mtr_subif->inst);
		if (mtr_subif->fid < 0) {
			pr_err("DPM: fid less then 0\n");
			kfree(mtr_subif);
			return DP_FAILURE;
		}
#endif
	} else {
		pr_err("DPM: Meter Flag not set\n");
		kfree(mtr_subif);
		return DP_FAILURE;
	}

	dp_info = get_dp_prop_info(mtr_subif->inst);
	if (!dp_info->dp_meter_del) {
		kfree(mtr_subif);
		return DP_FAILURE;
	}
	res = dp_info->dp_meter_del(dev, meter, flag, mtr_subif);
	kfree(mtr_subif);
	return res;
}
EXPORT_SYMBOL(dp_meter_del2);

void dp_subif_reclaim(struct rcu_head *rp)
{
	struct dp_subif_cache *dp_subif =
		container_of(rp, struct dp_subif_cache, rcu);

	kmem_cache_free(cache_subif_list, dp_subif);
}

u32 dp_subif_hash(struct net_device *dev)
{
	unsigned long index;

	index = (unsigned long)dev;
	/*Note: it is 4K alignment. Need tune later */
	return (u32)((index >>
		      DP_SUBIF_LIST_HASH_SHIFT) % DP_SUBIF_LIST_HASH_SIZE);
}

int dp_subif_list_init(void)
{
	int i;

	for (i = 0; i < DP_SUBIF_LIST_HASH_SIZE; i++)
		INIT_HLIST_HEAD(&dp_subif_list[i]);

	cache_subif_list = kmem_cache_create("dp_subif_list",
					     sizeof(struct dp_subif_cache),
					     0, SLAB_HWCACHE_ALIGN, NULL);
	if (!cache_subif_list)
		return -ENOMEM;
	return 0;
}

void dp_subif_list_free(void)
{
	kmem_cache_destroy(cache_subif_list);
}

struct dp_subif_cache *dp_subif_lookup_safe(struct hlist_head *head,
	const struct net_device *dev, void *data)
{
	struct dp_subif_cache *item;
	struct hlist_node *n;

	hlist_for_each_entry_safe(item, n, head, hlist) {
		if (item->dev == dev)
			return item;
	}
	return NULL;
}

int32_t dp_del_subif(struct net_device *netif, void *data, dp_subif_t *subif,
		     char *subif_name, u32 flags)
{
	struct dp_subif_cache *dp_subif;
	u32 idx;

	idx = dp_subif_hash(netif);
	dp_subif = dp_subif_lookup_safe(&dp_subif_list[idx], netif, data);
	if (!dp_subif) {
		pr_err("DPM: %s:Failed dp_subif_lookup: %s\n", __func__,
		       netif ? netif->name : "NULL");
		return -1;
	}
	hlist_del_rcu(&dp_subif->hlist);
	call_rcu_bh(&dp_subif->rcu, dp_subif_reclaim);
	DP_DEBUG(DP_DBG_FLAG_REG,
		 "deleted dev %s(subif: %s) from rcu subif list\n",
		 netif ? netif->name : "NULL", subif_name);
	return 0;
}

int32_t dp_update_subif(struct net_device *netif, void *data,
			dp_subif_t *subif, char *subif_name, u32 flags,
			dp_get_netif_subifid_fn_t subifid_fn_t)
{
	struct dp_subif_cache *dp_subif_new, *dp_subif;
	u32 idx;

	idx = dp_subif_hash(netif);
	dp_subif = dp_subif_lookup_safe(&dp_subif_list[idx], netif, data);
	if (!dp_subif) { /*alloc new */
		dp_subif = kmem_cache_zalloc(cache_subif_list, GFP_ATOMIC);
		if (!dp_subif)
			return -1;
		dp_memcpy(&dp_subif->subif, subif, sizeof(dp_subif_t));
		dp_subif->data = (u8 *)data;
		dp_subif->dev = netif;
		if (subif_name)
			dp_strlcpy(dp_subif->name, subif_name,
				   sizeof(dp_subif->name));
		dp_subif->subif_fn = subifid_fn_t;
		hlist_add_head_rcu(&dp_subif->hlist, &dp_subif_list[idx]);
		DP_DEBUG(DP_DBG_FLAG_REG,
			 "added dev %s(subif: %s) to rcu subif list\n",
			 netif ? netif->name : "NULL", subif_name);
	} else {
		dp_subif_new = kmem_cache_zalloc(cache_subif_list, GFP_ATOMIC);
		if (!dp_subif_new)
			return -1;
		dp_memcpy(&dp_subif_new->subif, subif, sizeof(dp_subif_t));
		dp_subif_new->data = (u8 *)data;
		dp_subif_new->dev = netif;
		if (subif_name)
			dp_strlcpy(dp_subif_new->name, subif_name,
				   sizeof(dp_subif->name));
		dp_subif_new->subif_fn = subifid_fn_t;
		hlist_replace_rcu(&dp_subif->hlist,
				  &dp_subif_new->hlist);
		call_rcu_bh(&dp_subif->rcu, dp_subif_reclaim);
		DP_DEBUG(DP_DBG_FLAG_REG,
			 "updated dev %s(subif: %s) to rcu subif list\n",
			 netif ? netif->name : "NULL", subif_name);
	}
	return 0;
}

int32_t dp_sync_subifid(struct net_device *dev, char *subif_name,
			dp_subif_t *subif_id, struct dp_subif_data *data,
			u32 flags)
{
	struct pmac_port_info *port;
	int res = DP_FAILURE;

	port = get_dp_port_info(subif_id->inst, subif_id->port_id);

	/*check flag for register / deregister to update/del */
	if (flags & DP_F_DEREGISTER) {
		/* subif info not required for data->ctp_dev */
		res = dp_get_subifid_for_update(subif_id->inst, dev,
						&subif_id[0], flags);
		if (res)
			pr_err("DPM: %s:dp_get_subifid_for_update dereg fail(%s)err:%d\n",
			       __func__, data->ctp_dev->name, res);
	} else {
		res = dp_get_subifid_for_update(subif_id->inst, dev,
						&subif_id[0], 0);
		if (res) {
			pr_err("DPM: %s:dp_get_subifid_for_update fail (%s)(err:%d)\n",
			       __func__, dev->name ? dev->name : "NULL", res);
			return res;
		}
		if (data && data->ctp_dev) {
			subif_id[1].port_id = subif_id[0].port_id;
			res = dp_get_subifid_for_update(subif_id->inst,
							data->ctp_dev,
							&subif_id[1], 0);
			if (res) {
				pr_err("DPM: %s:dp_get_subifid_for_update 1 fail(%s)err:%d\n",
				       __func__, data->ctp_dev->name, res);
				return res;
			}
		}
	}
	res = DP_SUCCESS;
	return res;
}

int32_t dp_sync_subifid_priv(struct net_device *dev, char *subif_name,
			     dp_subif_t *subif_id, struct dp_subif_data *data,
			     u32 flags, dp_get_netif_subifid_fn_t subifid_fn,
			     int f_notif, bool no_notify)
{
	void *subif_data = NULL;
	struct pmac_port_info *port;

	port = get_dp_port_info(subif_id->inst, subif_id->port_id);

	/* Note: workaround to set dummy subif_data via subif_name for DSL case.
	 *       During dp_get_netif_subifID, subif_data is used to get its PVC
	 *       information.
	 * Later VRX518/618 need to provide valid subif_data in order to support
	 * multiple DSL instances during dp_register_subif_ext
	 */
	if (is_dsl(port) && !dev)
		subif_data = (void *)subif_name;
	/*check flag for register / deregister to update/del */
	if (flags & DP_F_DEREGISTER) {
		if (data && data->ctp_dev)
			if (dp_del_subif(data->ctp_dev, subif_data,
					 &subif_id[1], NULL, flags))
				return DP_FAILURE;

		if (subif_id[0].subif_num) {
			if (dp_update_subif(dev, subif_data, &subif_id[0],
					    subif_name, flags, subifid_fn))
				return DP_FAILURE;
		} else {
			if (dp_del_subif(dev, subif_data, &subif_id[0],
					 subif_name, flags))
				return DP_FAILURE;
		}
	} else {
		if (subif_id[0].subif_num) {
			dp_update_subif(dev, subif_data, &subif_id[0],
					subif_name, flags, subifid_fn);

			DP_DEBUG(DP_DBG_FLAG_NOTIFY, "inst: %d, bport: %d, "
				"bport->ref_cnt: %d, f_notif: %d\n",
				subif_id->inst, subif_id->bport,
				get_dp_bp_info(subif_id->inst,subif_id->bport)->ref_cnt,
				f_notif);
			if ((get_dp_bp_info(subif_id->inst,
					    subif_id->bport)->ref_cnt <= 1)
					    && (f_notif) && !no_notify)
				dp_notifier_invoke(subif_id->inst, dev,
						   subif_id->port_id,
						   subif_id->subif, NULL,
						   DP_EVENT_REGISTER_SUBIF);

			if (data && data->ctp_dev) {
				if (dp_update_subif(data->ctp_dev, subif_data,
						    &subif_id[1], data->ctp_dev->name, flags,
						    subifid_fn))
					return DP_FAILURE;
				if (!f_notif || no_notify)
					goto exit;
				dp_notifier_invoke(subif_id->inst, data->ctp_dev,
						   subif_id->port_id,
						   subif_id[1].subif, NULL,
						   DP_EVENT_REGISTER_SUBIF);
			}
		}
	}
exit:
	return DP_SUCCESS;
}

int dp_update_shared_bp_to_subif(int inst, struct net_device *netif, int bp,
				 int portid)
{
	struct pmac_port_info *p_info;
	struct dp_subif_info *sif;
	struct bp_pmapper *bp_info;
	int i;

	DP_DEBUG(DP_DBG_FLAG_REG, "\n");
	p_info = get_dp_port_info(inst, portid);
	if (p_info->status != PORT_SUBIF_REGISTERED)
		return DP_FAILURE;

	DP_DEBUG(DP_DBG_FLAG_REG, "port:%d bp:%d\n", portid, bp);
	bp_info = get_dp_bp_info(inst, bp);
	/* search sub-interfaces/VAP */
	for (i = 0; i < p_info->subif_max; i++) {
		sif = get_dp_port_subif(p_info, i);

		if (!sif->flags)
			continue;

		if (sif->bp == bp) {
			sif->domain_id = bp_info->domain_id;
			sif->domain_members = bp_info->domain_member;
		}
	}
	return DP_SUCCESS;
}

/**
 * get_dma_chan_idx - Get available dma chan index from dp_dma_chan_tbl.
 * @inst: DP instance.
 * @dma_chan: DMA channels with core/port/channel information
 * Description: Find free dma channel index from dp_dma_chan_tbl.
 * Return: Base idx on success DP_FAILURE on failure.
 */
int get_dma_chan_idx(int inst, u32 dma_chan)
{
	u8 cid, pid;
	u16 nid;

	dp_dma_parse_id(dma_chan, &cid, &pid, &nid);
	if (cid >= DP_MAX_DMA_CORE) {
		pr_err("DPM: wrong dma controller id %d: Should < %d\n",
		       cid, DP_MAX_DMA_CORE);
		return DP_FAILURE;
	}
	if (pid >= DP_MAX_DMA_PORT) {
		pr_err("DPM: wrong dma port id %d: Should < %d\n",
		       pid, DP_MAX_DMA_PORT);
		return DP_FAILURE;
	}
	if (nid >= DP_MAX_DMA_CH) {
		pr_err("DPM: wrong dma channle id %d. Should < %d\n",
		       nid, DP_MAX_DMA_CH);
		return DP_FAILURE;
	}
	/* reserved offset zero for not valid offset: so here start from 1 */
	return (cid * (int)DP_MAX_DMA_PORT + pid) * DP_MAX_DMA_CH + nid + 1;

}

/**
 * alloc_dma_chan_tbl: Dynamic allocation of dp_dma_chan_tbl.
 * @inst: DP instance.
 * Return: DP_SUCCESS on success DP_FAILURE on failure.
 */
u32 alloc_dma_chan_tbl(int inst)
{
	dp_dma_chan_tbl[inst] = dp_kzalloc(sizeof(struct dma_chan_info) *
					   DP_MAX_DMA_CORE * DP_MAX_DMA_PORT *
					   DP_MAX_DMA_CH, GFP_ATOMIC);
	if (!dp_dma_chan_tbl[inst])
		return DP_FAILURE;
	return DP_SUCCESS;
}

/**
 * alloc_dp_port_info: Dynamic allocation of alloc_dp_port_info.
 * @inst: DP instance.
 * Return: DP_SUCCESS on success DP_FAILURE on failure.
 */
u32 alloc_dp_port_subif_info(int inst)
{
	int port_id;
	int max_dp_ports;	/* max dp ports */
	int max_subif;		/* max subif per port */
	struct inst_info *info = NULL;

	if (is_invalid_inst(inst))
		return DP_FAILURE;

	/* Retrieve the hw capabilities */
	info = get_dp_prop_info(inst);
	max_dp_ports = info->cap.max_num_dp_ports;
	max_subif = info->cap.max_num_subif_per_port;

	dp_port_info[inst] = dp_kzalloc((sizeof(struct pmac_port_info) *
					max_dp_ports), GFP_ATOMIC);
	if (!dp_port_info[inst])
		return DP_FAILURE;
	for (port_id = 0; port_id < max_dp_ports; port_id++) {
		struct dp_subif_info *sifs;
		int i;

		sifs = dp_kzalloc(sizeof(struct dp_subif_info) * max_subif,
				  GFP_ATOMIC);
		if (!sifs) {
			pr_err("DPM: Failed for dp_kzalloc: %zu bytes\n",
			       max_subif * sizeof(struct dp_subif_info));
			while (--port_id >= 0)
				kfree(get_dp_port_info(inst,
							port_id)->subif_info);
			return DP_FAILURE;
		}
		for (i = 0; i < max_subif; i++)
			INIT_LIST_HEAD(&sifs[i].logic_dev);
		get_dp_port_info(inst, port_id)->subif_info = sifs;
		spin_lock_init(&get_dp_port_info(inst, port_id)->mib_cnt_lock);
	}
	return DP_SUCCESS;
}

/**
 * free_dma_chan_tbl: Free dp_dma_chan_tbl.
 * @inst: DP instance.
 */
void free_dma_chan_tbl(int inst)
{
	/* free dma chan tbl */
	kfree(dp_dma_chan_tbl[inst]);
}

/**
 * free_dp_port_subif_info: free port subif info.
 * @inst: DP instance.
 */
void free_dp_port_subif_info(int inst)
{
	int port_id;
	int max_dp_ports;
	struct pmac_port_info *port_info;
	struct inst_info *info = NULL;

	/* Retrieve the hw capabilities */
	info = get_dp_prop_info(inst);
	max_dp_ports = info->cap.max_num_dp_ports;

	if (dp_port_info[inst]) {
		for (port_id = 0; port_id < max_dp_ports; port_id++) {
			port_info = get_dp_port_info(inst, port_id);
			kfree(port_info->subif_info);
		}
		kfree(dp_port_info[inst]);
	}
}

int do_tx_hwtstamp(int inst, int dpid, struct sk_buff *skb)
{
	struct mac_ops *ops;
	int rec_id = 0;

	if (!(skb_shinfo(skb)->tx_flags & SKBTX_HW_TSTAMP))
		return 0;
	ops = dp_port_prop[inst].mac_ops[dpid];
	if (!ops) {
		pr_err("DPM: %s: mac_ops is NULL\n", __func__);
		return -EINVAL;
	}

	rec_id = ops->do_tx_hwts(ops, skb);
	if (rec_id < 0) {
		pr_err("DPM: %s: do_tx_hwts failed\n", __func__);
		return -EINVAL;
	}

	return rec_id;
}

/**
 * dp_get_dma_chan_num - Get number of dma_channel
 * @inst: DP instance.
 * @ep: DP portid.
 * Return: number of dma_channel valid for each subif
 * By default dma_ch_num is 1, dma_ch_offset is same for
 * all DEQ port
 * Note: so far there are only 3 different DMA CH mapping use case to device
 *       under one dp port/lpid:
 * 1) no DMA to device: like ACA device
 * 2) all CQM dequeue ports maps to one single DMA channel:
  *     For example: PON, 64 dequeue ports sharing one single DMA CH via PON IP
  *     LAN Ethernet Port: 1 dequeue port use one DMA channel
 * 3) Each dequeue port has its own unique DMA CH and not sharing with other
 *    CQM dequeue ports, like G.INT
 */
int dp_get_dma_ch_num(int inst, int ep, int num_deq_port)
{
	int i;
	struct pmac_port_info *port_info = get_dp_port_info(inst, ep);
	struct cqm_port_info *deq_pinfo;
	u32 dma_ch_num = 0, dma_ch = 0;

	for (i = 0; i < num_deq_port; i++) {
		deq_pinfo = get_dp_deqport_info(inst, port_info->deq_ports[i]);
		if (deq_pinfo->f_dma_ch) {
			if (dma_ch_num == 0) {
				dma_ch = deq_pinfo->dma_chan;
				dma_ch_num++;
			} else {
				if (dma_ch != deq_pinfo->dma_chan) {
					/* if dma_ch_offset is diff then
					 * each DEQ port map to 1 DMA
					 * (for example G.INT case)
					 */
					dma_ch = deq_pinfo->dma_chan;
					dma_ch_num++;
				}
			}
		}
	}
	return dma_ch_num;
}

int dp_add_subif_spl_dev(int inst, struct net_device *dev,
				   char *subif_name, dp_subif_t *subif_id,
				   struct dp_subif_data *data, u32 flags)
{
	struct pmac_port_info *port_info;
	struct cqm_port_info *cqm_info;
	cbm_queue_map_entry_t *lookup;
	u8 cid, pid;
	u16 nid, ctp_start;
	int spl_dev_port_id, ret, i, j;
	struct dp_subif_info *sif;

	if (!(data->flag_ops & (DP_SUBIF_CPU | DP_SUBIF_REINSERT))) {
		DP_DEBUG(DP_DBG_FLAG_REG,
			 "%s: unsupported flags for dev %s\n",
			 __func__, dev ? dev->name : "NULL");
		return DP_FAILURE;
	}

	if (data->flag_ops & DP_SUBIF_CPU) {
		spl_dev_port_id = CPU_PORT;
		port_info = get_dp_port_info(inst, spl_dev_port_id);
		if (!port_info) {
			pr_err("DPM: %s: get_dp_port_info failed dev %s port id %d\n",
			       __func__, dev ? dev->name : "NULL",
			       spl_dev_port_id);
			return DP_FAILURE;
		}

		DP_CB(inst, get_itf_start_end)(port_info->itf_info, &ctp_start,
					       NULL);
		subif_id->port_id = spl_dev_port_id;
		subif_id->subif = 0;
		subif_id->data_flag = data->flag_ops;
		subif_id->inst = inst;
		subif_id->alloc_flag = port_info->alloc_flags;
		subif_id->lookup_mode = port_info->cqe_lu_mode;
		subif_id->gsw_mode = port_info->gsw_mode;
		subif_id->ctp_base = ctp_start;
		subif_id->subif_num = 0;
		subif_id->num_q = 0;
		j = 0;
		for (i = 0; i < CQM_MAX_CPU * MAX_PORTS_PER_CPU; i++) {
			sif = get_dp_port_subif(port_info, i);
			if (!sif->flags)
				continue;
			subif_id->subif_list[j] = sif->subif;
			subif_id->def_qlist[j] = sif->qid;
			subif_id->subif_flag[j] = flags;
			subif_id->subif_num++;
			subif_id->num_q++;
			if (j == 0) { /* interface design limitation for CPU */
				subif_id->ctp = i;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
				subif_id->gpid = sif->gpid;
#endif
				subif_id->subif_groupid = i;
			}
			j++;
		}

		DP_DEBUG(DP_DBG_FLAG_REG,
			 "%s: CPU subif for dev %s port id %u\n",
			 __func__, dev ? dev->name : "NULL", spl_dev_port_id);

		return dp_update_subif(dev, data, subif_id, subif_name, flags,
				       NULL);
	} else if (data->flag_ops & DP_SUBIF_REINSERT) {
		cqm_info = get_dp_deqport_info(inst, reinsert_deq_port[inst]);
		dp_dma_parse_id(cqm_info->dma_chan, &cid, &pid, &nid);
		spl_dev_port_id = dp_get_pmac_id(inst, cid);

		/* Update external subif struct for reins port */
		subif_id->port_id = spl_dev_port_id;
		subif_id->subif = -1;
		subif_id->data_flag = data->flag_ops;
		subif_id->inst = inst;
		subif_id->def_qid = cqm_info->first_qid;
		subif_id->subif_num = 1;

		DP_DEBUG(DP_DBG_FLAG_REG,
			 "%s: reinsert subif for dev %s port id %u\n",
			 __func__, dev ? dev->name : "NULL", spl_dev_port_id);

		ret = dp_update_subif(dev, data, subif_id, subif_name, flags,
				      NULL);
		if (ret)
			return ret;

		if (is_soc_lgm(inst)) {
			/* update lookup mode to CQE_LU_MODE2 */
			lookup = dp_kzalloc(sizeof(*lookup), GFP_ATOMIC);
			if (!lookup)
				return DP_FAILURE;

			lookup->ep = spl_dev_port_id;
			if (CBM_OPS(inst, cqm_mode_table_set,
				    dp_port_prop[inst].cbm_inst,
				    lookup, CQE_LU_MODE2,
				    CBM_QUEUE_MAP_F_MPE1_DONTCARE |
				    CBM_QUEUE_MAP_F_MPE2_DONTCARE)) {
				pr_err("DPM: %s: cqm_mode_table_set failed\n",
				       __func__);
				kfree(lookup);
				return DP_FAILURE;
			}
			kfree(lookup);
			subif_id->lookup_mode = CQE_LU_MODE2;
		} else if (is_soc_prx(inst)) {
			port_info = get_dp_port_info(inst, spl_dev_port_id);
			subif_id->lookup_mode = port_info->cqe_lu_mode;
		}
		/* set default queue map for reinsert qid */
		_dp_set_subif_q_lookup_tbl(NULL, 0, 0, inst);
	}
	return 0;
}

int dp_del_subif_spl_dev(int inst, struct net_device *dev,
				   char *subif_name, dp_subif_t *subif_id,
				   struct dp_subif_data *data, u32 flags)
{
	if (data->flag_ops & DP_SUBIF_REINSERT)
		_dp_reset_subif_q_lookup_tbl(NULL, 0, inst);

	if ((data->flag_ops & (DP_SUBIF_CPU | DP_SUBIF_REINSERT)) &&
	    flags & DP_F_DEREGISTER)
		/* deregister/remove dummy subif from RCU subif */
		return dp_del_subif(dev, data, subif_id, subif_name, flags);

	DP_DEBUG(DP_DBG_FLAG_REG, "%s: unsupported flags for dev %s\n",
		 __func__, dev ? dev->name : "NULL");
	return DP_FAILURE;
}

u32 dp_get_tx_cbm_pkt2(int inst, int port_id, int subif_id_grp)
{
	struct dp_subif_info *sif;
	struct dev_mib *mib;
	struct pmac_port_info *port;
	s32 cnt_tx_cbm_pkt;

	if (inst >= dp_inst_num)
		return 0;

	port = get_dp_port_info(inst, port_id);
	if (!port)
		return 0;
	if (port->status == PORT_FREE)
		return 0;

	if (subif_id_grp >= port->num_subif)
		return 0;

	sif = get_dp_port_subif(port, subif_id_grp);
	mib = get_dp_port_subif_mib(sif);

	if (!sif->flags)
		return 0;

	cnt_tx_cbm_pkt = STATS_GET(mib->tx_cbm_pkt);

	return cnt_tx_cbm_pkt;
}
EXPORT_SYMBOL(dp_get_tx_cbm_pkt2);

int dp_strncmpi2(const char *s1, const char *s2, size_t n)
{
	if (!s1 || !s2)
		return 1;
	return strncasecmp(s1, s2, n);
}
EXPORT_SYMBOL(dp_strncmpi2);

void dp_replace_ch2(char *p, int len, char orig_ch, char new_ch)
{
	int i;

	if (p)
		for (i = 0; i < len; i++) {
			if (p[i] == orig_ch)
				p[i] = new_ch;
		}
}
EXPORT_SYMBOL(dp_replace_ch2);

u64 dp_atoull(unsigned char *str)
{
	unsigned long long v = 0;
	char *p = NULL;
	int res;

	if (!str)
		return v;
	dp_replace_ch(str, strlen(str), '.', 0);
	dp_replace_ch(str, strlen(str), ' ', 0);
	dp_replace_ch(str, strlen(str), '\r', 0);
	dp_replace_ch(str, strlen(str), '\n', 0);
	if (str[0] == 0)
		return v;
	if (str[0] == 'b' || str[0] == 'B') {
		p = str + 1;
		res = kstrtoull(p, 2, &v); /* binary */
	} else if ((str[0] == '0') && ((str[1] == 'x') || (str[1] == 'X'))) {
		p = str + 2;
		res = kstrtoull(p, 16, &v); /* hex */
	} else {
		p = str;
		res = kstrtoll(p, 10, &v); /* dec */
	}
	if (res)
		v = 0;

	return (u64)v;
}
EXPORT_SYMBOL(dp_atoull);

int dp_atoi2(unsigned char *str)
{
	return (int)dp_atoull(str);
}
EXPORT_SYMBOL(dp_atoi2);

/*Split buffer to multiple segment with seperater space.
 *And put pointer to array[].
 *By the way, original buffer will be overwritten with '\0' at some place.
 */
int dp_split_buffer2(char *buffer, char *array[], int max_param_num)
{
	int i = 0;

	if (!array)
		return 0;
	dp_memset(array, 0, sizeof(array[0]) * max_param_num);
	if (!buffer)
		return 0;
	while ((array[i] = dp_strsep(&buffer, " \n")) != NULL) {
		size_t len = strlen(array[i]);

		dp_replace_ch(array[i], len, ' ', 0);
		dp_replace_ch(array[i], len, '\r', 0);
		dp_replace_ch(array[i], len, '\n', 0);
		len = strlen(array[i]);
		if (!len)
			continue;
		i++;
		if (i == max_param_num)
			break;
	}

	return i;
}
EXPORT_SYMBOL(dp_split_buffer2);

static struct ctp_dev *dp_ctp_dev_list_lookup(struct list_head *head,
					      struct net_device *dev)
{
	struct ctp_dev *pos;

	list_for_each_entry(pos, head, list) {
		if (pos->dev == dev)
			return pos;
	}
	return NULL;
}

int dp_ctp_dev_list_add(struct list_head *head, struct net_device *dev, int bp,
			int vap)
{
	struct ctp_dev *ctp_dev_list;

	ctp_dev_list = dp_ctp_dev_list_lookup(head, dev);
	if (!ctp_dev_list) {
		ctp_dev_list = dp_kzalloc(sizeof(*ctp_dev_list), GFP_ATOMIC);
		if (!ctp_dev_list) {
			pr_err("DPM: %s alloc ctp_dev_list fail\n", __func__);
			return DP_FAILURE;
		}
		ctp_dev_list->dev = dev;
		ctp_dev_list->bp = bp;
		ctp_dev_list->ctp = vap;
		DP_DEBUG(DP_DBG_FLAG_DBG, "add ctp dev list\n");
		list_add(&ctp_dev_list->list, head);
	} else {
		DP_DEBUG(DP_DBG_FLAG_DBG, "ctp dev list exist: %s\n",
			 ctp_dev_list->dev ? ctp_dev_list->dev->name : "NULL");
	}
	return DP_SUCCESS;
}

int dp_ctp_dev_list_del(struct list_head *head, struct net_device *dev)
{
	struct ctp_dev *ctp_dev_list;

	ctp_dev_list = dp_ctp_dev_list_lookup(head, dev);
	if (!ctp_dev_list) {
		pr_err("DPM: %s ctp dev(%s) not found\n", __func__,
		       dev ? dev->name : "NULL");
		return DP_FAILURE;
	}
	DP_DEBUG(DP_DBG_FLAG_DBG, "del ctp dev list\n");
	list_del(&ctp_dev_list->list);
	kfree(ctp_dev_list);
	return DP_SUCCESS;
}

struct cbm_tx_push *get_dq_tx_push_info(struct cbm_cpu_port_data *p,
						int inst, int i, int j)
{
	struct inst_info *info = NULL;

	if (is_invalid_inst(inst))
		return NULL;
	/* Retrieve the hw capabilities */
	info = get_dp_prop_info(inst);
	if (j >= info->cap.max_port_per_cpu)
		return NULL;

	return &p->dq_tx_push_info[i][j];
}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
void dump_cpu_data(int inst, struct cbm_cpu_port_data *p)
{
	int i, j;
	struct cbm_tx_push *dq_tx_push_info;

	if (!(dp_dbg_flag & DP_DBG_FLAG_REG))
		return ;
	DP_DUMP("dump_cpu_data:\n");
	DP_DUMP("--------------\n");
	for (i = 0; i < CQM_MAX_CPU; i++) {
		for (j = 0; j < PORTS_PER_CPU; j++) {
			dq_tx_push_info = get_dq_tx_push_info(p, inst, i, j);
			if (!dq_tx_push_info)
				continue;
			if (dq_tx_push_info->deq_port < 0)
				continue;
			DP_DUMP("   dq_tx_push_info[%d][%d]: \n", i, j);
			DP_DUMP("      deq_port        = %d\n",
				 dq_tx_push_info->deq_port);
			DP_DUMP("      txpush_addr_qos = %lx\n",
				(unsigned long) dq_tx_push_info->txpush_addr_qos);
			DP_DUMP("      txpush_addr     = %lx\n",
				(unsigned long) dq_tx_push_info->txpush_addr);
#if !IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
			DP_DUMP("      igp_id          = %d\n",
				 dq_tx_push_info->igp_id);
			DP_DUMP("      f_igp_qos       = %d\n",
				 dq_tx_push_info->f_igp_qos);
#endif
		}
	}
	DP_DUMP("   flush info:\n");
	DP_DUMP("      flush.deq             = %d\n",
		p->dq_tx_flush_info.deq_port);
	DP_DUMP("      flush.txpush_addr_qos = %lx\n",
		 (unsigned long) p->dq_tx_flush_info.txpush_addr_qos);
	DP_DUMP("      flush.txpush_addr     = %lx\n",
		 (unsigned long) p->dq_tx_flush_info.txpush_addr);
#if !IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
	DP_DUMP("      flush.igp_id          = %d\n",
		p->dq_tx_flush_info.igp_id);
	DP_DUMP("      flush.f_igp_qos       = %d\n",
		p->dq_tx_flush_info.f_igp_qos);
#endif
	DP_DUMP("   reinsert info:\n");
	DP_DUMP("      reinser.deq             = %d\n",
		p->re_insertion.deq_port);
	DP_DUMP("      reinser.txpush_addr_qos = %lx\n",
		 (unsigned long) p->re_insertion.txpush_addr_qos);
	DP_DUMP("      reinser.txpush_addr     = %lx\n",
		 (unsigned long) p->re_insertion.txpush_addr);
	DP_DUMP("      reinser.dma_chan        = %lx\n",
		 (unsigned long) p->re_insertion.dma_chan);
	DP_DUMP("      reinser.deq_port_num    = %lx\n",
		 (unsigned long) p->re_insertion.deq_port_num);
}
#else
void dump_cpu_data(int inst, struct cbm_cpu_port_data *p)
{
}
#endif /* CONFIG_DPM_DATAPATH_DBG */

void dp_trace_pr(unsigned long ip, const char *format, ...)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG) && IS_ENABLED(CONFIG_TRACING)
	va_list args;
	int len;

	if (!log_buf) {
		pr_err("DPM: %s: log buf NULL\n", __func__);
		return;
	}
	va_start(args, format);
	len = vsnprintf(log_buf, log_buf_len - 1, format, args);
	va_end(args);
	__trace_puts(ip, log_buf, len);
#endif /* CONFIG_DPM_DATAPATH_DBG */
}

char *dp_strsep(char **stringp, const char *delim)
{
	char *str, *p = (char *)*stringp;

	while (1) {
		str = strsep(&p, delim);
		if (!p) {
			break;
		}
		if (strlen(str) == 0)
			continue;
		break;
	}
	*stringp = p;
	return str;
}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DEBUGFS)
void dp_dump_debugfs_qos_all(void)
{
	proc_qos_init(NULL);
	DP_DUMP_DEBUGFS_QOS_ALL();
}
EXPORT_SYMBOL(dp_dump_debugfs_qos_all);
#endif

/* may multiple flags */
int alloc_flag_str(int flag, char *buf, int buf_len)
{
	int i, len = 0;

	if (!buf)
		return -1;

	buf[0] = '\0';
	if (flag == -1) {
		dp_strlcpy(buf, "-1", buf_len);
		return 0;
	} else if (flag == 0) {
		dp_strlcpy(buf, "0", buf_len);
		return 0;
	}
	/* need support multiple flags */
	for (i = 0; i < get_dp_port_type_str_size(); i++) {
		if (!(flag & dp_port_flag[i]))
			continue;
		/* check buffer size */
		if (len + strlen(dp_port_type_str[i]) + 2 >= buf_len)
			break; /* buffer not enough */

		dp_strlcpy(buf + len, dp_port_type_str[i],
		           buf_len - len);
		len += strlen(dp_port_type_str[i]);

		buf[len] = '|';
		len++;

		buf[len] = '\0';
	}

	/* remove last | character */
	if (len > 0 && buf[len - 1] == '|')
		buf[len - 1] = '\0';
	return 0;
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP
