/*****************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <linux/if_ether.h>

#ifdef LINUX
#include <errno.h>
#include <unistd.h>
#include <net/if.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <linux/if_packet.h>
#include <arpa/inet.h>
#include <linux/filter.h>
#endif

#include <netlink/netlink.h>
#include <netlink/route/link.h>

#include <pon_adapter_errno.h>
#include <omci/pon_adapter_mcc.h>

#include "pon_net_mcc_core.h"
#include "pon_net_common.h"
#include "pon_net_debug.h"
#include "pon_net_netlink.h"

#ifdef MCC_DRV_ENABLE
#include "pon_mcc.h"
#include "pon_mcc_ikm.h"
#endif

#define INSERTION_NETDEV	"ins0"

#define VLAN_S			4U
#define PROTO_S			2

#define FILTER_DOES_NOT_MATCH	0
#define FILTER_MATCHES		1

#define VLAN_PRESENT		1

#define MOP_VLAN_TPID		0x8100

/** Structure to parse the VLAN tag inside of an Ethernet packet */
struct vlan {
	/** Tag Control Information (TCI),
	 * contains:
	 *	Priority code point (PCP:3),
	 *	Drop eligible indicator (DEI:1),
	 *	and VLAN identifier (VID:12),
	 */
	uint16_t h_vid_pcp;
	/** Ethernet type following the VLAN tag.
	 *  this can be next vlan TPID or ETHERTYPE
	 */
	uint16_t h_proto;
};

/** Private data structure added to each received packet which is forwarded
 *  to general mcc handling.
 */
struct pon_mcc_priv {
	int ifindex;
	uint16_t buffsize;
};

#define HEX_BYTES_PER_LINE	16
#define HEX_CHARS_PER_BYTE	3
#define HEX_CHARS_PER_LINE	(HEX_BYTES_PER_LINE * HEX_CHARS_PER_BYTE + 1)

/* Printout/dump MCC packet */
#ifndef DUMP_MCC_PACKET_ENABLED
#define pon_net_mcc_pkt_dump(data, len, inf)
#else
static void pon_net_mcc_pkt_dump(const uint8_t *data, const uint32_t len,
					const struct pa_mcc_pkt_info *inf)
{
	int i = 0, bytes = (int)len, stamp = 0;
	char line[HEX_CHARS_PER_LINE], *s;

	s = line;
	while (--bytes >= 0) {
		snprintf(s, HEX_CHARS_PER_BYTE + 1, " %02X", *data++);
		s += HEX_CHARS_PER_BYTE;
		i++;
		if (i >= HEX_BYTES_PER_LINE) {
			dbg_prn("\t0x%04X: %s\n", stamp, line);
			i = 0;
			s = line;
			stamp += HEX_BYTES_PER_LINE;
		}
	}
	if (i) {
		*s = '\0';
		dbg_prn("\t0x%04X: %s\n", stamp, line);
	}
}
#endif /* DUMP_MCC_PACKET_ENABLED */

#ifdef LINUX
static enum pon_adapter_errno init_nl_sock(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p", ctx);

	ctx->mcc_nl_sock = nl_socket_alloc();
	if (!ctx->mcc_nl_sock) {
		FN_ERR_RET(0, nl_socket_alloc, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	ret = nl_connect(ctx->mcc_nl_sock, NETLINK_ROUTE);
	if (ret) {
		nl_socket_free(ctx->mcc_nl_sock);
		FN_ERR_RET(ret, nl_connect, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno exit_nl_sock(struct pon_net_context *ctx)
{
	dbg_in_args("%p", ctx);

	nl_socket_free(ctx->mcc_nl_sock);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Creating a filter for IGMP or supported MLD types
 *
 * To generate the packet-matching code as a C program fragment, use the
 * following command: <tcpdump -dd 'filter'>
 * (to use it, remove the comments (#) and put all on one line):
 *
 * Note: The outer VLAN tag is not detected via filter "ether" but
 * contained in the "auxdata" metadata structure instead. Hence, the four bytes
 * of the VLAN tag must not be considered here!
 *
 * # untagged or tagged IGMP (no VLAN tag considered)
 * (igmp) or
 * # untagged or tagged MLD (no VLAN tag considered)
 * (icmp6 and (ether[54]=0x82 or ether[54]=0x83 or ether[54]=0x84 or
 *		ether[54]=0x8f)) or
 * # untagged or tagged MLD with hop-by-hop header (no VLAN tag considered)
 * (ether[12:2]=0x86dd and ether[20]=0x0 and
 *	(ether[54]=0x3a or ether[54]=0x2c) and
 *	(ether[62]=0x82 or ether[62]=0x83 or ether[62]=0x84 or ether[62]=0x8f))
 * or
 * # double tagged IGMP (one VLAN tag considered)
 * ((ether[12:2]=0x8100 or ether[12:2]=0x88A8) and
 *	(ether[16:2]=0x800 and ether[27]=2)) or
 * # double tagged MLD (one VLAN tag considered)
 * ((ether[12:2]=0x8100 or ether[12:2]=0x88A8) and ether[16:2]=0x86dd and
 *	(ether[24]=0x3a or ether[24]=0x2c) and
 *	(ether[58]=0x82 or ether[58]=0x83 or ether[58]=0x84 or ether[58]=0x8f))
 * or
 * # double tagged MLD with hop-by-hop header (one VLAN tag considered)
 * ((ether[12:2]=0x8100 or ether[12:2]=0x88A8) and ether[16:2]=0x86dd and
 *	ether[24]=0x0 and (ether[58]=0x3a or ether[58]=0x2c) and
 *	(ether[66]=0x82 or ether[66]=0x83 or ether[66]=0x84 or ether[66]=0x8f))
 *
 * The packet-matching code can be generated in a human readable form by
 * <tcpdump -d 'filter'>:
 * (000) ldh      [12]
 * (001) jeq      #0x800           jt 2    jf 4
 * (002) ldb      [23]
 * (003) jeq      #0x2             jt 58   jf 15
 * (004) jeq      #0x86dd          jt 5    jf 15
 * (005) ldb      [20]
 * (006) jeq      #0x3a            jt 10   jf 7
 * (007) jeq      #0x2c            jt 8    jf 15
 * (008) ldb      [54]
 * (009) jeq      #0x3a            jt 10   jf 15
 * (010) ldb      [54]
 * (011) jeq      #0x82            jt 58   jf 12
 * (012) jeq      #0x83            jt 58   jf 13
 * (013) jeq      #0x84            jt 58   jf 14
 * (014) jeq      #0x8f            jt 58   jf 15
 * (015) ldh      [12]
 * (016) jeq      #0x86dd          jt 17   jf 24
 * (017) ldb      [20]
 * (018) jeq      #0x0             jt 19   jf 59
 * (019) ldb      [54]
 * (020) jeq      #0x3a            jt 22   jf 21
 * (021) jeq      #0x2c            jt 22   jf 59
 * (022) ldb      [62]
 * (023) jeq      #0x82            jt 58   jf 55
 * (024) jeq      #0x8100          jt 26   jf 25
 * (025) jeq      #0x88a8          jt 26   jf 59
 * (026) ldh      [16]
 * (027) jeq      #0x800           jt 28   jf 30
 * (028) ldb      [27]
 * (029) jeq      #0x2             jt 58   jf 30
 * (030) ldh      [12]
 * (031) jeq      #0x8100          jt 33   jf 32
 * (032) jeq      #0x88a8          jt 33   jf 59
 * (033) ldh      [16]
 * (034) jeq      #0x86dd          jt 35   jf 43
 * (035) ldb      [24]
 * (036) jeq      #0x3a            jt 38   jf 37
 * (037) jeq      #0x2c            jt 38   jf 43
 * (038) ldb      [58]
 * (039) jeq      #0x82            jt 58   jf 40
 * (040) jeq      #0x83            jt 58   jf 41
 * (041) jeq      #0x84            jt 58   jf 42
 * (042) jeq      #0x8f            jt 58   jf 43
 * (043) ldh      [12]
 * (044) jeq      #0x8100          jt 46   jf 45
 * (045) jeq      #0x88a8          jt 46   jf 59
 * (046) ldh      [16]
 * (047) jeq      #0x86dd          jt 48   jf 59
 * (048) ldb      [24]
 * (049) jeq      #0x0             jt 50   jf 59
 * (050) ldb      [58]
 * (051) jeq      #0x3a            jt 53   jf 52
 * (052) jeq      #0x2c            jt 53   jf 59
 * (053) ldb      [66]
 * (054) jeq      #0x82            jt 58   jf 55
 * (055) jeq      #0x83            jt 58   jf 56
 * (056) jeq      #0x84            jt 58   jf 57
 * (057) jeq      #0x8f            jt 58   jf 59
 * (058) ret      #262144
 * (059) ret      #0
 *
 * The below array corresponds to the commands.
 * It is generated by <tcpdump -dd 'filter'>
 * (from script to overcome the maximal input length of the shell):
 */
static struct sock_filter bpf_code[] = {
	{ 0x28, 0, 0, 0x0000000c },
	{ 0x15, 0, 2, 0x00000800 },
	{ 0x30, 0, 0, 0x00000017 },
	{ 0x15, 54, 11, 0x00000002 },
	{ 0x15, 0, 10, 0x000086dd },
	{ 0x30, 0, 0, 0x00000014 },
	{ 0x15, 3, 0, 0x0000003a },
	{ 0x15, 0, 7, 0x0000002c },
	{ 0x30, 0, 0, 0x00000036 },
	{ 0x15, 0, 5, 0x0000003a },
	{ 0x30, 0, 0, 0x00000036 },
	{ 0x15, 46, 0, 0x00000082 },
	{ 0x15, 45, 0, 0x00000083 },
	{ 0x15, 44, 0, 0x00000084 },
	{ 0x15, 43, 0, 0x0000008f },
	{ 0x28, 0, 0, 0x0000000c },
	{ 0x15, 0, 7, 0x000086dd },
	{ 0x30, 0, 0, 0x00000014 },
	{ 0x15, 0, 40, 0x00000000 },
	{ 0x30, 0, 0, 0x00000036 },
	{ 0x15, 1, 0, 0x0000003a },
	{ 0x15, 0, 37, 0x0000002c },
	{ 0x30, 0, 0, 0x0000003e },
	{ 0x15, 34, 31, 0x00000082 },
	{ 0x15, 1, 0, 0x00008100 },
	{ 0x15, 0, 33, 0x000088a8 },
	{ 0x28, 0, 0, 0x00000010 },
	{ 0x15, 0, 2, 0x00000800 },
	{ 0x30, 0, 0, 0x0000001b },
	{ 0x15, 28, 0, 0x00000002 },
	{ 0x28, 0, 0, 0x0000000c },
	{ 0x15, 1, 0, 0x00008100 },
	{ 0x15, 0, 26, 0x000088a8 },
	{ 0x28, 0, 0, 0x00000010 },
	{ 0x15, 0, 8, 0x000086dd },
	{ 0x30, 0, 0, 0x00000018 },
	{ 0x15, 1, 0, 0x0000003a },
	{ 0x15, 0, 5, 0x0000002c },
	{ 0x30, 0, 0, 0x0000003a },
	{ 0x15, 18, 0, 0x00000082 },
	{ 0x15, 17, 0, 0x00000083 },
	{ 0x15, 16, 0, 0x00000084 },
	{ 0x15, 15, 0, 0x0000008f },
	{ 0x28, 0, 0, 0x0000000c },
	{ 0x15, 1, 0, 0x00008100 },
	{ 0x15, 0, 13, 0x000088a8 },
	{ 0x28, 0, 0, 0x00000010 },
	{ 0x15, 0, 11, 0x000086dd },
	{ 0x30, 0, 0, 0x00000018 },
	{ 0x15, 0, 9, 0x00000000 },
	{ 0x30, 0, 0, 0x0000003a },
	{ 0x15, 1, 0, 0x0000003a },
	{ 0x15, 0, 6, 0x0000002c },
	{ 0x30, 0, 0, 0x00000042 },
	{ 0x15, 3, 0, 0x00000082 },
	{ 0x15, 2, 0, 0x00000083 },
	{ 0x15, 1, 0, 0x00000084 },
	{ 0x15, 0, 1, 0x0000008f },
	{ 0x6, 0, 0, 0x00040000 },
	{ 0x6, 0, 0, 0x00000000 },
};

static const struct sock_fprog mcc_bpf = {
	.len = ARRAY_SIZE(bpf_code),
	.filter = bpf_code,
};
#endif

static enum pon_adapter_errno pon_net_mcc_init(void *ll_handle,
					       uint32_t *max_ports)
{
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_config *cfg = &ctx->cfg;
#ifdef LINUX
	int aux_val = 1;
	int err;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p", ll_handle, max_ports);

	/* This socket listens on all interfaces (flag + no bind) */
	ctx->mcc_s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (ctx->mcc_s < 0) {
		FN_ERR_RET(ctx->mcc_s, socket, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	/* Request the auxdata with the outer VLAN tag */
	err = setsockopt(ctx->mcc_s, SOL_PACKET, PACKET_AUXDATA, &aux_val,
			 sizeof(aux_val));
	if (err < 0) {
		FN_ERR_RET(err, setsockopt, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	err = setsockopt(ctx->mcc_s, SOL_SOCKET, SO_ATTACH_FILTER, &mcc_bpf,
			 sizeof(mcc_bpf));
	if (err < 0) {
		FN_ERR_RET(err, setsockopt, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	ret = init_nl_sock(ctx);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, init_nl_sock, ret);
		return PON_ADAPTER_ERROR;
	}

	/* Get the reference (ifindex) of the netdev for packet insertion */
	ctx->mcc_ins_ifindex = netlink_name_to_ifindex(ctx->netlink,
						       INSERTION_NETDEV);
	if (!ctx->mcc_ins_ifindex) {
		FN_ERR_NL(0, rtnl_link_name2i, INSERTION_NETDEV);
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
	err = netlink_netdevice_state_set(ctx->netlink, INSERTION_NETDEV, 1);
	if (err != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(err, netlink_netdevice_state_set, err);
		return err;
	}

	ctx->mcc_running = true;
#endif

	/* Set number of LAN ports */
	*max_ports = cfg->max_ports;

#ifdef MCC_DRV_ENABLE
	if (sem_init(&ctx->multicast_enabled_event, 0, 0)) {
		dbg_err_fn(sem_init);
		dbg_out_ret("%d", PON_ADAPTER_ERR_NO_DATA);
		return PON_ADAPTER_ERROR;
	}
#endif /* MCC_DRV_ENABLE */

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno pon_net_mcc_shutdown(void *ll_handle)
{
#ifdef LINUX
	struct pon_net_context *ctx = ll_handle;

	dbg_in_args("%p", ll_handle);

	exit_nl_sock(ctx);

	close(ctx->mcc_s);

#ifdef MCC_DRV_ENABLE
	sem_destroy(&ctx->multicast_enabled_event);
#endif /* MCC_DRV_ENABLE */

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
#endif
	return PON_ADAPTER_SUCCESS;
}

/* This parses the Ethernet packet and fills up the info parameter
 * "proto" is in network order
 */
static enum pon_adapter_errno parse_ethernet(uint16_t proto, uint8_t *msg,
					     size_t len,
					     struct pa_mcc_pkt_info *info)
{
	struct vlan *vlan_hdr;
	struct ip *ip_hdr;

	dbg_in_args("0x%04x, %p, %zu, %p", ntohs(proto), msg, len, info);

	switch (ntohs(proto)) {
	case ETH_P_8021Q:
	case ETH_P_8021AD:
		vlan_hdr = (struct vlan *)msg;

		if (len <= sizeof(*vlan_hdr)) {
			dbg_out_ret("%d", PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}

		info->offset_iph += (unsigned int)sizeof(*vlan_hdr);

		return parse_ethernet(vlan_hdr->h_proto,
				      msg + sizeof(*vlan_hdr),
				      len - sizeof(*vlan_hdr), info);
	case ETH_P_IP:
		ip_hdr = (struct ip *)msg;

		if (len <= sizeof(*ip_hdr)) {
			dbg_out_ret("%d", PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}

		if (ip_hdr->ip_p == IPPROTO_IGMP) {
			dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
			return PON_ADAPTER_SUCCESS;
		}

		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	case ETH_P_IPV6:
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}
	dbg_out_ret("%d", PON_ADAPTER_ERROR);
	return PON_ADAPTER_ERROR;
}

#ifdef LINUX
/* ignore warnings generated by the CMSG_* macros */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
static struct tpacket_auxdata *pon_net_mcc_get_tpacket(struct msghdr *msgh)
{
	struct cmsghdr *cmsg;

	for (cmsg = CMSG_FIRSTHDR(msgh); cmsg; cmsg = CMSG_NXTHDR(msgh, cmsg)) {
		if (cmsg->cmsg_level == SOL_PACKET
		    && cmsg->cmsg_type == PACKET_AUXDATA) {
			return (struct tpacket_auxdata *)CMSG_DATA(cmsg);
		}
	}
	return NULL;
}
#pragma GCC diagnostic pop

static int is_valid_downstream_interface(int ifindex)
{
	static const char * const prefixes[] = {
		"gem",
		"pmapper",
		"sw"
	};
	char ifname[IF_NAMESIZE];
	unsigned int i;
	bool ret = false;

	if (ifindex < 0 || !if_indextoname((unsigned int)ifindex, ifname)) {
		dbg_err_fn(if_indextoname);
		dbg_out_ret("%d", false);
		return false;
	}

	for (i = 0; i < ARRAY_SIZE(prefixes); i++)
		if (strncmp(ifname, prefixes[i],
			    strnlen_s(prefixes[i], IF_NAMESIZE)) == 0) {
			ret = true;
			break;
		}

	dbg_out_ret("%d", ret);
	return ret;
}

static int pon_net_mcc_receive_pkg(struct pon_net_context *ctx, uint8_t *msg,
				   uint16_t len, struct pa_mcc_pkt_info *info)
{
	int size;
	struct sockaddr_ll sockaddr = { 0 };
	struct ethhdr *ether_hdr;
	struct pon_mcc_priv *priv = (struct pon_mcc_priv *)&info->meta_info;
	struct msghdr msgh = {0,};
	struct iovec msg_iov = {0,};
	struct tpacket_auxdata *tpacket;
	char cmsghdr[CMSG_SPACE(sizeof(*tpacket))];
	uint16_t *vlan_ptr;
	int vlan;
	int err;
	uint8_t lan_idx;

	dbg_in_args("%p %p %i %p", ctx, msg, len, info);

	msg_iov.iov_base = msg;
	msg_iov.iov_len = len;

	msgh.msg_name = &sockaddr;
	msgh.msg_namelen = sizeof(sockaddr);
	msgh.msg_iov = &msg_iov;
	msgh.msg_iovlen = 1; /* number of iov elements */
	msgh.msg_control = &cmsghdr;
	msgh.msg_controllen = sizeof(cmsghdr);

	size = (int)recvmsg(ctx->mcc_s, &msgh, 0);
	if (size == -1) {
		FN_ERR_RET(errno, recvmsg, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	/* do not pass OUTGOING packets to process by omci daemon */
	if (sockaddr.sll_pkttype == PACKET_OUTGOING)
		return PON_ADAPTER_EAGAIN;

	lan_idx = ifindex_to_lan_index(ctx, sockaddr.sll_ifindex);
	if (lan_idx != 0xFF) {
		info->dir_us = true;
		info->port_idx = lan_idx;
	} else if (is_valid_downstream_interface(sockaddr.sll_ifindex)) {
		info->dir_us = false;
	} else {
		dbg_out_ret("%d", PON_ADAPTER_EAGAIN);
		return PON_ADAPTER_EAGAIN;
	}

	/* for at least Ethernet + VLAN header */
	if (size < (int)(sizeof(*ether_hdr) + 2)) {
		dbg_out_ret("%d", PON_ADAPTER_EAGAIN);
		return PON_ADAPTER_EAGAIN;
	}

	ether_hdr = (struct ethhdr *)msg;
	info->offset_iph = sizeof(*ether_hdr);

	err = parse_ethernet(ether_hdr->h_proto,
			     msg + sizeof(*ether_hdr),
			     (size_t)size - sizeof(*ether_hdr), info);
	if (err != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", PON_ADAPTER_EAGAIN);
		return PON_ADAPTER_EAGAIN;
	}

	tpacket = pon_net_mcc_get_tpacket(&msgh);
	if (!tpacket) {
		dbg_wrn("no auxdata found\n");
		return PON_ADAPTER_EAGAIN;
	}

	/*
	 * If we have an outer VLAN, move the data 4 bytes to the back and add
	 * the VLAN data after the Ethernet header. This works on a uint16_t
	 * because the VLAN data we get is also in uint16_t. The last 2 bytes
	 * of ether_hdr are the tag which was modified.
	 */
	vlan_ptr = (uint16_t *)(msg + sizeof(*ether_hdr) - 2);
	/*
	 * Linux 3.0 introduced the TP_STATUS_VLAN_VALID status flag to
	 * indicate that a VLAN tag was added, for older kernel versions we
	 * just assume that VLAN ID 0, means no VLAN.
	 */
#ifdef TP_STATUS_VLAN_VALID
	if (tpacket->tp_status & TP_STATUS_VLAN_VALID) {
#else
	if (tpacket->tp_vlan_tci) {
#endif
		if (size + 4 > len) {
			dbg_wrn("no space for VLAN tag\n");
			return PON_ADAPTER_EAGAIN;
		}
		memmove(vlan_ptr + 2, vlan_ptr,
			(size_t)size - sizeof(*ether_hdr) + 2);
		size += 4;
		info->offset_iph += 4;

		/*
		 * Linux 3.14 introduced the TP_STATUS_VLAN_TPID_VALID bit
		 * and the tp_vlan_tpid member, only do this check if this is
		 * defined and otherwise assume VLAN tpid 0x8100.
		 */
#ifdef TP_STATUS_VLAN_TPID_VALID
		if (tpacket->tp_status & TP_STATUS_VLAN_TPID_VALID)
			vlan_ptr[0] = htons(tpacket->tp_vlan_tpid);
		else
			vlan_ptr[0] = htons(0x8100);
#else
		vlan_ptr[0] = htons(0x8100);
#endif
		vlan_ptr[1] = htons(tpacket->tp_vlan_tci);

		vlan = tpacket->tp_vlan_tci & 0xfff;
	} else {
		vlan = PA_MCC_VLAN_UNTAGGED;
	}

	info->cvid = (uint16_t)vlan;
	/* TODO: Fix VLAN ID after handling */
	/* TODO: Add VLAN translation */
	info->svid = (uint16_t)vlan;

	priv->ifindex = sockaddr.sll_ifindex;

	dbg_out_ret("%i", size);
	return size;
}
#endif

static enum pon_adapter_errno pon_net_mcc_pkt_receive
		(void *ll_handle, uint8_t *msg, uint16_t *len,
		 struct pa_mcc_pkt_info *info)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
#ifdef LINUX
	struct pon_net_context *ctx = ll_handle;
	fd_set rfds;
	struct timeval tv = {0};
	struct pon_mcc_priv *priv = (struct pon_mcc_priv *)&info->meta_info;
	int err;

	dbg_in_args("%p, %p, %p, %p", ll_handle, msg, len, info);

	/* check if this interface function is properly called */
	if (!msg || !len) {
		ret = PON_ADAPTER_ERR_PTR_INVALID;
		dbg_out_ret("%d", ret);
		return ret;
	}

	/* save the packet buffer size to use for vlan handling */
	priv->buffsize = *len;

#ifdef MCC_DRV_ENABLE
	/* If multicast is disabled ignore all packet which may arrive:
	 * - without checking socket just pause the thread until multicast
	 *   is enabled
	 * - after multicast is enabled exit to higher level where a next
	 *   packet receiving will start from the beginning in 'event loop'
	 * - if the 'wait' function returns an error then notify about
	 *   this and behave as if the thread was awaken correctly
	 *
	 *TODO: If possible configure the hardware such that no unwanted
	 * packets are forwarded to the software, to avoid such waiting.
	 */
	if (!ctx->is_multicast_enabled) {
		if (sem_wait(&ctx->multicast_enabled_event) != 0)
			dbg_err_fn(sem_wait);

		return PON_ADAPTER_ERROR;
	}
#endif

	while (true) {
		FD_ZERO(&rfds);
		FD_SET(ctx->mcc_s, &rfds);

		tv.tv_sec = 1;
		tv.tv_usec = 0;

		err = select(ctx->mcc_s + 1, &rfds, NULL, NULL, &tv);
		if (err == -1) {
			FN_ERR_RET(err, select, err);
			return PON_ADAPTER_ERROR;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion" /* for FD_ISSET */
		} else if (err && FD_ISSET(ctx->mcc_s, &rfds)) {
#pragma GCC diagnostic pop
			err = pon_net_mcc_receive_pkg(ctx, msg, *len, info);
			if (err == PON_ADAPTER_EAGAIN)
				continue;
			if (err < 0) {
				dbg_out_ret("%d", ret);
				return err;
			}

#ifdef MCC_DRV_ENABLE
			/* Drop the packet if multicasting is disabled */
			if (!ctx->is_multicast_enabled)
				continue;
#endif

			/* tell the caller how many bytes were read */
			*len = (uint16_t)err;
			break;
		}
		if (!ctx->mcc_running) {
			dbg_out_ret("%d", PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}
	}

#else
	int i;

	for (i = 0; i < 100; i++) {
		Sleep(10);
		pthread_testcancel();
	}
	ret = PON_ADAPTER_ERROR;
#endif
	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno pon_net_mcc_pkt_receive_cancel(void *ll_handle)
{
	struct pon_net_context *ctx = ll_handle;

	dbg_in_args("%p", ll_handle);

	ctx->mcc_running = false;

#ifdef MCC_DRV_ENABLE
	if (sem_post(&ctx->multicast_enabled_event))
		dbg_err_fn(sem_post);
#endif

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno pon_net_mcc_pkt_send(void *ll_handle,
					   const uint8_t *msg,
					   const uint16_t len,
					   const struct pa_mcc_pkt_info *info)
{
	enum pon_adapter_errno ret = PON_ADAPTER_ERROR;
#ifdef LINUX
	struct pon_net_context *ctx = ll_handle;
	struct sockaddr_ll sockaddr = {0,};
	ssize_t size;
	struct ethhdr *ether_hdr = (struct ethhdr *)msg;
	struct pon_mcc_priv *priv = (struct pon_mcc_priv *)info->meta_info;
	struct msghdr msghdr;
	struct cmsghdr *cmsg;
	struct iovec iov;
	union {
		char ifi[CMSG_SPACE(sizeof(int))];
		struct cmsghdr align;
	} u;

	dbg_in_args("%p, %p, %u, %p", ll_handle, msg, len, info);

	sockaddr.sll_family = AF_PACKET;
	/* No endianness conversion here as the value is expected in BE
	 * and it is already stored in this byte order.
	 */
	sockaddr.sll_protocol = ether_hdr->h_proto;
	if (memcpy_s(sockaddr.sll_addr, sizeof(sockaddr.sll_addr),
			ether_hdr->h_dest, sizeof(ether_hdr->h_dest))) {
		dbg_err_fn(memcpy_s);
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
	sockaddr.sll_halen = sizeof(ether_hdr->h_dest);
	sockaddr.sll_ifindex = ctx->mcc_ins_ifindex;

	iov.iov_base = (void *)msg;
	iov.iov_len = len;
	memset(&msghdr, 0, sizeof(msghdr));
	msghdr.msg_name = &sockaddr;
	msghdr.msg_namelen = sizeof(sockaddr);
	msghdr.msg_control = u.ifi;
	msghdr.msg_controllen = CMSG_LEN(sizeof(unsigned int));
	msghdr.msg_flags = 0;
	msghdr.msg_iov = &iov;
	msghdr.msg_iovlen = 1;

	/* CMSG hdr */
	cmsg = CMSG_FIRSTHDR(&msghdr);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SO_MARK;
	cmsg->cmsg_len = CMSG_LEN(sizeof(unsigned int));
	/* Pass the ifindex of the original receive interface */
	*(int *)CMSG_DATA(cmsg) = priv->ifindex;

	size = sendmsg(ctx->mcc_s, &msghdr, 0);
	if (size == len)
		ret = PON_ADAPTER_SUCCESS;

	dbg_out_ret("%d", ret);
#endif
	return ret;
}

static enum pon_adapter_errno pon_net_fid_get(void *ll_handle,
					      const uint16_t o_vid,
					      uint8_t *fid)
{
	dbg_in_args("%p, %u, %p", ll_handle, o_vid, fid);

	/* TODO: Fid depends on outer VLAN information and should be implemented
	 * later.
	 */
	*fid = 0;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno pon_net_vlan_unaware_mode_set(void *ll_handle,
							    const bool enable)
{
	dbg_in_args("%p, %d", ll_handle, enable);

	/* TODO: Current implementation in GSWIP Multicast forwarding table does
	 * not support VLAN based forwarding. Needs to be implemented later.
	 */

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

#ifdef MCC_DRV_ENABLE
static bool is_mc_ipv6(const union pa_mcc_ip_addr *ip)
{
	return ip->ipv6[0] == 0xFF;
}

static const struct {
	/* Key in the mapper */
	int mapper_key;
	/* Type of uni interface, to obtain name for */
	uint8_t type;
} unis_to_try[] = {
	{MAPPER_PPTPETHERNETUNI_MEID_TO_IDX, UNI_PPTP_MC},
	{MAPPER_VEIP_MEID_TO_IDX, UNI_VEIP_MC},
};

/* get Linux interface index of multicast device for either UNI or VEIP */
static unsigned int
lport_to_ifindex(struct pon_net_context *ctx, uint16_t lport)
{
	unsigned int i = 0;

	for (i = 0; i < ARRAY_SIZE(unis_to_try); ++i) {
		enum pon_adapter_errno ret;
		struct mapper *m = ctx->mapper[unis_to_try[i].mapper_key];
		uint8_t type = unis_to_try[i].type;
		uint32_t unused;
		char ifname[IF_NAMESIZE] = "";

		/* Just check if corresponding ME has been created */
		ret = mapper_id_get(m, &unused, lport);
		if (ret != PON_ADAPTER_SUCCESS)
			continue;

		ret = pon_net_uni_ifname_get(ctx, type, lport, ifname,
					     sizeof(ifname));
		if (ret != PON_ADAPTER_SUCCESS)
			break;

		return if_nametoindex(ifname);
	}

	return 0;
}
#endif

#ifdef LINUX
#ifdef MCC_DRV_ENABLE
/* Check if given interface is added to a bridge */
static enum pon_adapter_errno
check_master(struct pon_net_context *ctx, int ifindex,
	     bool *is_slave)
{
	int err;
	struct rtnl_link *link;

	dbg_in_args("%p, %d, %p", ctx, ifindex, is_slave);

	err = rtnl_link_get_kernel(ctx->mcc_nl_sock, ifindex, NULL, &link);
	if (err || !link) {
		FN_ERR_RET(err, rtnl_link_get_kernel, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	*is_slave = !!rtnl_link_get_master(link);

	rtnl_link_put(link);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}
#endif /* MCC_DRV_ENABLE */
#endif /* LINUX */

static enum pon_adapter_errno __pon_net_fwd_update(void *ll_handle,
					const uint8_t fid,
					const bool include_enable,
					const uint16_t bridge_id,
					const uint8_t lan_port,
					const union pa_mcc_ip_addr *da,
					const struct pa_mcc_src_filter *sf,
					const uint8_t filter_size)
{
#ifdef LINUX
#ifdef MCC_DRV_ENABLE
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret;
	uint8_t src_addr[PON_MCC_ADDR6SZ];
	uint8_t grp_addr[PON_MCC_ADDR6SZ];
	uint32_t bridge_idx;
	uint8_t proto;
	uint32_t ifindex;
	int i;
	bool is_slave;
#endif

	dbg_in_args("%p, %u, %d, %u, %u, %p, %p, %u", ll_handle, fid,
		include_enable, bridge_id, lan_port, da, sf, filter_size);

#ifdef MCC_DRV_ENABLE
	/* get bridge index - maybe needed for future use, currently not used */
	ret = mapper_index_get(ctx->mapper[MAPPER_MACBRIDGE_MEID_TO_IDX],
			       bridge_id, &bridge_idx);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, mapper_index_get, ret);
		return ret;
	}

	ifindex = lport_to_ifindex(ctx, lan_port);
	if (ifindex == 0)
		return PON_ADAPTER_SUCCESS;

	/* Check if the lan_port is registered to a bridge. */
	ret = check_master(ctx, (int)ifindex, &is_slave);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, check_master, ret);
		return ret;
	}
	/* If the lan_port is not registered to a bridge
	 * do not try to update table entry for the lan_port
	 */
	if (!is_slave) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	/* check if group address is IPv4 or IPv6 */
	if (is_mc_ipv6(da)) {
		if (memcpy_s(grp_addr, sizeof(grp_addr),
			     da->ipv6, sizeof(da->ipv6))) {
			dbg_err_fn(memcpy_s);
			dbg_out_ret("%d", PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}
		proto = MCC_L3_PROTO_IPV6;
	} else {
		if (memcpy_s(grp_addr, sizeof(grp_addr),
			     da->ipv4, sizeof(da->ipv4))) {
			dbg_err_fn(memcpy_s);
			dbg_out_ret("%d", PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}
		proto = MCC_L3_PROTO_IPV4;
	}

	/* add entry to forwarding table */
	if (filter_size > 0) {
		for (i = 0; i < filter_size; i++) {
			if (proto == MCC_L3_PROTO_IPV4) {
				if (memcpy_s(src_addr, sizeof(src_addr),
					     sf[i].sa.ipv4,
					     sizeof(sf[i].sa.ipv4))) {
					dbg_err_fn(memcpy_s);
					dbg_out_ret("%d", PON_ADAPTER_ERROR);
					return PON_ADAPTER_ERROR;
				}
			} else {
				if (memcpy_s(src_addr, sizeof(src_addr),
					     sf[i].sa.ipv6,
					     sizeof(sf[i].sa.ipv6))) {
					dbg_err_fn(memcpy_s);
					dbg_out_ret("%d", PON_ADAPTER_ERROR);
					return PON_ADAPTER_ERROR;
				}
			}
			dbg_prn("pon_net_mcc_core.c: fwd_update: tbl_entry_add IGMPv3: ifindex = %d, proto = %u, src_addr = %p, grp_addr = %p, flt_mode = %d\n",
				ifindex, proto, src_addr, grp_addr,
				SRC_FLT_MODE_INCLUDE);

			ret = mcl_table_entry_add(ifindex, proto,
						  grp_addr, src_addr,
						  SRC_FLT_MODE_INCLUDE);
			if (ret != 0) {
				dbg_err("Could not add entry to the table\n");
				return ret;
			}

			dbg_prn("pon_net_mcc_core.c: fwd_update: tbl_entry_add IGMPv3: ret = %d\n",
				ret);
		}
	} else {

		dbg_prn("pon_net_mcc_core.c: fwd_update: tbl_entry_add IGMPv2: ifindex = %d, proto = %u, grp_addr = %p, flt_mode = %d\n",
			ifindex, proto, grp_addr, SRC_FLT_MODE_EXCLUDE);

		ret = mcl_table_entry_add(ifindex, proto,
					  grp_addr, NULL,
					  SRC_FLT_MODE_EXCLUDE);
		if (ret != 0) {
			dbg_err("Could not add entry to the table\n");
			return ret;
		}

		dbg_prn("pon_net_mcc_core.c: fwd_update: tbl_entry_add IGMPv2: ret = %d\n",
			ret);
	}

#endif /* MCC_DRV_ENABLE */
#endif /* LINUX */
	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno pon_net_fwd_update(void *ll_handle,
					const uint8_t fid,
					const bool include_enable,
					const uint16_t bridge_id,
					const uint8_t port_map,
					const union pa_mcc_ip_addr *da,
					const struct pa_mcc_src_filter *sf,
					const uint8_t filter_size)
{
	enum pon_adapter_errno ret;
	uint8_t i;

	dbg_in_args("%p, %u, %d, %u, %u, %p, %p, %u", ll_handle, fid,
		include_enable, bridge_id, port_map, da, sf, filter_size);

	/* configure ports included in port_map */
	for (i = 0; i < LAN_PORT_MAX; i++) {
		if (port_map & (1 << i)) {
			ret = __pon_net_fwd_update(ll_handle, fid,
					include_enable, bridge_id,
					i, da, sf, filter_size);
			if (ret) {
				FN_ERR_RET(ret, __pon_net_fwd_update, ret);
				return ret;
			}
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno pon_net_port_add(void *ll_handle,
					const enum pa_mcc_dir dir,
					const uint8_t lan_port,
					const uint8_t fid,
					const union pa_mcc_ip_addr *ip)
{
#ifdef LINUX
#ifdef MCC_DRV_ENABLE
	uint8_t grp_addr[PON_MCC_ADDR6SZ];
	uint8_t proto;
	uint32_t ifindex;
	int ret;
	struct pon_net_context *ctx = ll_handle;
	bool is_slave;
#endif /* MCC_DRV_ENABLE */

	dbg_in_args("%p, %d, %u, %u, %p", ll_handle, dir, lan_port, fid, ip);

#ifdef MCC_DRV_ENABLE
	ifindex = lport_to_ifindex(ll_handle, lan_port);
	if (ifindex == 0)
		return PON_ADAPTER_SUCCESS;

	/* Check if the lan_port is registered to a bridge. */
	ret = check_master(ctx, (int)ifindex, &is_slave);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_is_slave, ret);
		return ret;
	}

	/* If the lan_port is not registered to a bridge
	 * do not try to add table entry for the lan_port
	 */
	if (!is_slave) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	/* check if group address is IPv4 or IPv6 */
	if (is_mc_ipv6(ip)) {
		if (memcpy_s(grp_addr, sizeof(grp_addr),
			ip->ipv6, sizeof(ip->ipv6))) {
			dbg_err_fn(memcpy_s);
				dbg_out_ret("%d", PON_ADAPTER_ERROR);
				return PON_ADAPTER_ERROR;
		}
		proto = MCC_L3_PROTO_IPV6;
	} else {
		if (memcpy_s(grp_addr, sizeof(grp_addr),
			ip->ipv4, sizeof(ip->ipv4))) {
			dbg_err_fn(memcpy_s);
				dbg_out_ret("%d", PON_ADAPTER_ERROR);
				return PON_ADAPTER_ERROR;
		}
		proto = MCC_L3_PROTO_IPV4;
	}

	dbg_prn("pon_net_mcc_core.c: port_add: tbl_entry_add: ifindex = %d, proto = %u, grp_addr = %p, flt_mode = %d\n",
		ifindex, proto, grp_addr, SRC_FLT_MODE_NA);

	/* add entry to forwarding table */
	ret = mcl_table_entry_add(ifindex, proto,
				  grp_addr, NULL,
				  SRC_FLT_MODE_NA);
	if (ret != 0) {
		dbg_err("Could not add entry to the table\n");
		return ret;
	}

	dbg_prn("pon_net_mcc_core.c: port_add: tbl_entry_add: ret = %d\n",
		ret);
#endif /* MCC_DRV_ENABLE */
#endif /* LINUX */
	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno pon_net_port_remove(void *ll_handle,
						const uint8_t lan_port,
						const uint8_t fid,
						const union pa_mcc_ip_addr *ip)
{
#ifdef LINUX
#ifdef MCC_DRV_ENABLE
	uint8_t grp_addr[PON_MCC_ADDR6SZ];
	uint8_t proto;
	uint32_t ifindex;
	int ret;
#endif

	dbg_in_args("%p, %u, %u, %p", ll_handle, lan_port, fid, ip);

#ifdef MCC_DRV_ENABLE
	ifindex = lport_to_ifindex(ll_handle, lan_port);
	if (ifindex == 0)
		return PON_ADAPTER_SUCCESS;

	/* check if group address is IPv4 or IPv6 */
	if (is_mc_ipv6(ip)) {
		if (memcpy_s(grp_addr, sizeof(grp_addr),
			ip->ipv6, sizeof(ip->ipv6))) {
			dbg_err_fn(memcpy_s);
				dbg_out_ret("%d", PON_ADAPTER_ERROR);
				return PON_ADAPTER_ERROR;
		}
		proto = MCC_L3_PROTO_IPV6;
	} else {
		if (memcpy_s(grp_addr, sizeof(grp_addr),
			ip->ipv4, sizeof(ip->ipv4))) {
			dbg_err_fn(memcpy_s);
				dbg_out_ret("%d", PON_ADAPTER_ERROR);
				return PON_ADAPTER_ERROR;
		}
		proto = MCC_L3_PROTO_IPV4;
}

	dbg_prn("pon_net_mcc_core.c: port_remove: tbl_entry_del: ifindex = %d, proto = %u, grp_addr = %p, flt_mode = %d\n",
		ifindex, proto, grp_addr, SRC_FLT_MODE_NA);

	/* remove entry from forwarding table */
	ret = mcl_table_entry_del(ifindex, proto,
				  grp_addr, NULL,
				  SRC_FLT_MODE_NA);
	if (ret != 0) {
		dbg_err("Could not add entry to the table\n");
		return ret;
	}

	dbg_prn("pon_net_mcc_core.c: port_remove: tbl_entry_del: ret = %d\n",
		ret);
#endif /* MCC_DRV_ENABLE */
#endif /* LINUX */
	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno pon_net_port_activity_get(void *ll_handle,
						const uint8_t lan_port,
						const uint8_t fid,
						const union pa_mcc_ip_addr *ip,
						bool *is_active)
{
	dbg_in_args("%p, %u, %u, %p, %p", ll_handle, lan_port, fid, ip,
		is_active);

	/* TODO: needs to be fixed first by the B step, A step does not
	 * provide information about port activity. Should be implemented later
	 */
	*is_active = true;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pa_omci_mcc_ops pon_net_omci_mcc_ops = {
	.init = &pon_net_mcc_init,
	.shutdown = &pon_net_mcc_shutdown,
	.pkt_receive = &pon_net_mcc_pkt_receive,
	.pkt_receive_cancel = &pon_net_mcc_pkt_receive_cancel,
	.pkt_send = &pon_net_mcc_pkt_send,
	.fid_get = &pon_net_fid_get,
	.vlan_unaware_mode_set = &pon_net_vlan_unaware_mode_set,
	.fwd_update = &pon_net_fwd_update,
	.port_add = &pon_net_port_add,
	.port_remove = &pon_net_port_remove,
	.port_activity_get = &pon_net_port_activity_get,
};
