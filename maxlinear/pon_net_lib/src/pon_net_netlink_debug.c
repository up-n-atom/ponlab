/*****************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <netlink/route/tc.h>
#include <netlink/route/link.h>
#include <netlink/route/action.h>
#include <netlink/route/act/gact.h>
#include <netlink/route/act/vlan.h>
#include <netlink/route/act/mirred.h>
#include <netlink/route/act/colmark.h>
#include <netlink/route/act/police.h>
#include <netlink/route/act/skbedit.h>
#include <netlink/route/class.h>
#include <netlink/route/classifier.h>
#include <netlink/route/cls/flower.h>
#include <netlink/route/qdisc/drr.h>
#include <netlink/route/qdisc/prio.h>
#include <netlink/route/qdisc/red.h>
#include <netlink/route/qdisc/tbf.h>
#include <linux/if_ether.h>
#include <linux/tc_act/tc_vlan.h>
#include <string.h>
#ifdef LINUX
#include <arpa/inet.h>
#endif

#include "pon_net_netlink_debug.h"
#include "pon_net_netlink.h"
#include "pon_net_debug.h"
#include "pon_net_common.h"

#ifndef TC_H_CLSACT
#define TC_H_CLSACT	TC_H_INGRESS
#define TC_H_MIN_INGRESS	0xFFF2U
#define TC_H_MIN_EGRESS		0xFFF3U
#endif

#ifndef TCA_CLS_FLAGS_SKIP_HW
#define TCA_CLS_FLAGS_SKIP_HW	(1 << 0)
#endif

#ifndef TCA_CLS_FLAGS_SKIP_SW
#define TCA_CLS_FLAGS_SKIP_SW	(1 << 1)
#endif

#define TC_COOKIE_MAX_SIZE 16

/* Debug level, on which the debug prints in this file will be displayed */
#define LVL DBG_PRN

static void print_protocol(uint32_t proto)
{
	switch (proto) {
	case ETH_P_ALL:
		dbg_printf(LVL, "all");
		break;
	case ETH_P_8021Q:
		dbg_printf(LVL, "802.1q");
		break;
	case ETH_P_8021AD:
		dbg_printf(LVL, "802.1ad");
		break;
	case ETH_P_IP:
		dbg_printf(LVL, "ip");
		break;
	case ETH_P_IPV6:
		dbg_printf(LVL, "ipv6");
		break;
	case ETH_P_ARP:
		dbg_printf(LVL, "arp");
		break;
	default:
		dbg_printf(LVL, "0x%x", proto);
		break;
	}
}

static void print_handle(uint32_t handle)
{
	uint16_t hi = (uint16_t)((handle >> 16) & 0xFFFF);
	uint16_t lo = (uint16_t)(handle & 0xFFFF);

	if (lo)
		dbg_printf(LVL, "%x:%x", hi, lo);
	else
		dbg_printf(LVL, "%x:", hi);
}

/* callers need to take lock */
static void print_dev(struct nl_cache *link_cache,
		      struct rtnl_tc *tc)
{
	char ifname[16] = { 0 };
	int ifindex = rtnl_tc_get_ifindex(tc);

	if (rtnl_link_i2name(link_cache, ifindex,
			     ifname, sizeof(ifname))) {
		dbg_printf(LVL, " dev %s", ifname);
	} else {
		dbg_printf(LVL, " dev %d", ifindex);
	}
}

static void print_parent(struct rtnl_tc *tc)
{
	uint32_t tmp;

	tmp = rtnl_tc_get_parent(tc);
	if (tmp == TC_H_MAKE(TC_H_CLSACT, TC_H_MIN_INGRESS))
		dbg_printf(LVL, " ingress");
	else if (tmp == TC_H_MAKE(TC_H_CLSACT, TC_H_MIN_EGRESS))
		dbg_printf(LVL, " egress");
	else if (tmp == TC_H_ROOT)
		dbg_printf(LVL, " root");
	else  {
		dbg_printf(LVL, " parent ");
		print_handle(tmp);
	}
}

static void print_cookie(struct rtnl_act *act)
{
	unsigned char cookie[TC_COOKIE_MAX_SIZE] = { 0 };
	unsigned int i = 0;

	if (rtnl_act_get_cookie(act, cookie, ARRAY_SIZE(cookie)))
		return;

	dbg_printf(LVL, " cookie ");
	for (i = 0; i < ARRAY_SIZE(cookie); ++i)
		dbg_printf(LVL, "%02x", cookie[i]);
}

static void print_vlan_details(struct rtnl_act *act)
{
	uint16_t tmp16;
	uint8_t tmp8;

	if (rtnl_vlan_get_vlan_id(act, &tmp16) == 0)
		dbg_printf(LVL, " id %hu", tmp16);
	if (rtnl_vlan_get_vlan_prio(act, &tmp8) == 0)
		dbg_printf(LVL, " priority %hhu", tmp8);
	if (rtnl_vlan_get_protocol(act, &tmp16) == 0)
		dbg_printf(LVL, " protocol 0x%hx", ntohs(tmp16));
}

static void print_skbedit_details(struct rtnl_act *act)
{
	uint32_t tmp32;

	if (rtnl_skbedit_get_priority(act, &tmp32) == 0)
		dbg_printf(LVL, " priority %u", tmp32);
}

/* callers need to take lock */
void netlink_debug_tc_flower_filter_add(struct nl_cache *link_cache,
					struct rtnl_cls *cls)
{
	uint64_t tmp64;
	uint32_t tmp;
	uint16_t tmp16;
	uint8_t tmp8;
	char indev[16] = { 0 };
	struct rtnl_act *act;
	char *kind = NULL;
	int action;
	char ifname[IF_NAMESIZE] = { 0 };
	int ifindex;

	if (libponnet_dbg_lvl > LVL)
		return;

	dbg_naked(LVL, "tc filter add");

	print_dev(link_cache, TC_CAST(cls));

	print_parent(TC_CAST(cls));
	dbg_printf(LVL, " handle ");
	dbg_printf(LVL, "0x%x", rtnl_tc_get_handle(TC_CAST(cls)));

	dbg_printf(LVL, " protocol ");
	print_protocol(rtnl_cls_get_protocol(cls));

	if (rtnl_cls_get_prio(cls))
		dbg_printf(LVL, " prio %u", rtnl_cls_get_prio(cls));

	dbg_printf(LVL, " flower");

	if (rtnl_flower_get_flags(cls, &tmp) == 0) {
		if (tmp & TCA_CLS_FLAGS_SKIP_HW)
			dbg_printf(LVL, " skip_hw");

		if (tmp & TCA_CLS_FLAGS_SKIP_SW)
			dbg_printf(LVL, " skip_sw");
	}

	if (rtnl_flower_get_vlan_id(cls, &tmp) == 0)
		dbg_printf(LVL, " vlan_id %u", tmp);

	if (rtnl_flower_get_vlan_prio(cls, &tmp) == 0)
		dbg_printf(LVL, " vlan_prio %u", tmp);

	if (rtnl_flower_get_vlan_eth_type(cls, &tmp16) == 0) {
		dbg_printf(LVL, " vlan_ethtype ");
		print_protocol(tmp16);
	}

	if (rtnl_flower_get_cvlan_id(cls, &tmp) == 0)
		dbg_printf(LVL, " cvlan_id %u", tmp);

	if (rtnl_flower_get_cvlan_prio(cls, &tmp) == 0)
		dbg_printf(LVL, " cvlan_prio %u", tmp);

	if (rtnl_flower_get_cvlan_eth_type(cls, &tmp16) == 0) {
		dbg_printf(LVL, " cvlan_ethtype ");
		print_protocol(tmp16);
	}

	if (rtnl_flower_get_ip_proto(cls, &tmp8) == 0)
		dbg_printf(LVL, " ip_proto 0x%hhx", tmp8);

	if (rtnl_flower_get_icmpv6_type(cls, &tmp8) == 0)
		dbg_printf(LVL, " type %u", tmp8);

	if (rtnl_flower_get_classid(cls, &tmp) == 0) {
		dbg_printf(LVL, " classid ");
		print_handle(tmp);
	}

	if (rtnl_flower_get_indev(cls, indev, sizeof(indev)) == 0)
		dbg_printf(LVL, " indev %s", indev);

	act = rtnl_flower_get_action(cls);
	while (act) {
		kind = rtnl_tc_get_kind(TC_CAST(act));
		if (kind && strcmp(kind, "gact") == 0)  {
			dbg_printf(LVL, " action");
			action = rtnl_gact_get_action(act);
			if (action == TC_ACT_OK)
				dbg_printf(LVL, " pass");
			else if (action == TC_ACT_SHOT)
				dbg_printf(LVL, " drop");
			else if (action == TC_ACT_TRAP)
				dbg_printf(LVL, " trap");
			else
				dbg_printf(LVL, " %d", action);

			print_cookie(act);
		} else if (kind && strcmp(kind, "vlan") == 0) {
			dbg_printf(LVL, " action vlan");
			if (rtnl_vlan_get_mode(act, &action) == 0) {
				switch (action) {
				case TCA_VLAN_ACT_PUSH:
					dbg_printf(LVL, " push");
					print_vlan_details(act);
					break;
				case TCA_VLAN_ACT_POP:
					dbg_printf(LVL, " pop");
					break;
				case TCA_VLAN_ACT_MODIFY:
					dbg_printf(LVL, " modify");
					print_vlan_details(act);
					break;
				default:
					dbg_printf(LVL, " %d", action);
					break;
				}
			}
			print_cookie(act);
		} else if (kind && strcmp(kind, "mirred") == 0) {
			dbg_printf(LVL, " action mirred");
			/* direction and action */
			action = rtnl_mirred_get_action(act);
			switch (action) {
			case TCA_EGRESS_MIRROR:
				dbg_printf(LVL, " egress mirror");
				break;
			case TCA_EGRESS_REDIR:
				dbg_printf(LVL, " egress redirect");
				break;
			case TCA_INGRESS_MIRROR:
				dbg_printf(LVL, " ingress mirror");
				break;
			case TCA_INGRESS_REDIR:
				dbg_printf(LVL, " ingress redirect");
				break;
			default:
				dbg_printf(LVL, " %u", action);
				break;
			}
			/* index (specific policy instance id) */
			tmp = (uint32_t)rtnl_mirred_get_policy(act);
			if (tmp != 0)
				dbg_printf(LVL, " index %d", tmp);
			/* device name */
			ifindex = (int)rtnl_mirred_get_ifindex(act);
			if (rtnl_link_i2name(link_cache, ifindex,
					     ifname, sizeof(ifname)))
				dbg_printf(LVL, " dev %s", ifname);
			else
				dbg_printf(LVL, " dev %d", ifindex);
		} else if (kind && strcmp(kind, "colmark") == 0) {
			dbg_printf(LVL, " action colmark");

			if (rtnl_colmark_get_mode(act, &tmp) == 0) {
				if (tmp == 1)
					dbg_printf(LVL, " blind");
				else
					dbg_printf(LVL, " mode %u", tmp);
			}

			if (rtnl_colmark_get_drop_precedence(act, &tmp) == 0) {
				dbg_printf(LVL, " marker");
				switch (tmp) {
				case COLMARK_NO_MARKING:
					dbg_printf(LVL, " none");
					break;
				case COLMARK_INTERNAL:
					dbg_printf(LVL, " internal");
					break;
				case COLMARK_DEI:
					dbg_printf(LVL, " dei");
					break;
				case COLMARK_PCP_8P0D:
					dbg_printf(LVL, " pcp_8p0d");
					break;
				case COLMARK_PCP_7P1D:
					dbg_printf(LVL, " pcp_7p1d");
					break;
				case COLMARK_PCP_6P2D:
					dbg_printf(LVL, " pcp_6p2d");
					break;
				case COLMARK_PCP_5P3D:
					dbg_printf(LVL, " pcp_5p3d");
					break;
				case COLMARK_DSCP_AF:
					dbg_printf(LVL, " dscp_af");
					break;
				default:
					dbg_printf(LVL, " %u", tmp);
				}
			}

			if (rtnl_colmark_get_meter_type(act, &tmp) == 0) {
				dbg_printf(LVL, " mtype");
				switch (tmp) {
				case COLMARK_SRTCM:
					dbg_printf(LVL, " srTCM");
					break;
				case COLMARK_TRTCM:
					dbg_printf(LVL, " trTCM");
					break;
				default:
					dbg_printf(LVL, " %u", tmp);
				}
			}
		} else if (kind && strcmp(kind, "police") == 0) {
			dbg_printf(LVL, " action police");

			tmp64 = rtnl_police_get_rate(act);
			dbg_printf(LVL, " rate %" PRIu64, tmp64);

			tmp64 = rtnl_police_get_peakrate(act);
			dbg_printf(LVL, " peakrate %" PRIu64, tmp64);

			tmp = rtnl_police_get_burst(act);
			dbg_printf(LVL, " burst %u", tmp);

			tmp = rtnl_police_get_mtu(act);
			dbg_printf(LVL, " mtu %u", tmp);

			action = rtnl_police_get_action(act);
			switch (action) {
			case TC_POLICE_UNSPEC:
				dbg_printf(LVL, " unspecified");
				break;
			case TC_POLICE_OK:
				dbg_printf(LVL, " ok");
				break;
			case TC_POLICE_RECLASSIFY:
				dbg_printf(LVL, " reclassify");
				break;
			case TC_POLICE_SHOT:
				dbg_printf(LVL, " shot");
				break;
			case TC_POLICE_PIPE:
				/* pipe is printed separately */
				break;
			}
		} else if (kind && strcmp(kind, "skbedit") == 0) {
			dbg_printf(LVL, " skbedit");
			print_skbedit_details(act);
			print_cookie(act);
		} else {
			dbg_printf(LVL, " action %s", kind);
		}

		/* get next action from list */
		act = rtnl_act_next(act);

		if (act)
			dbg_printf(LVL, " pipe");
	}

	dbg_printf(LVL, "\n");
}

/* callers need to take lock */
void netlink_debug_tc_flower_filter_del(struct nl_cache *link_cache,
					struct rtnl_cls *cls)
{
	if (libponnet_dbg_lvl > LVL)
		return;

	dbg_naked(LVL, "tc filter del");
	print_dev(link_cache, TC_CAST(cls));
	dbg_printf(LVL, " handle ");
	dbg_printf(LVL, "0x%x", rtnl_tc_get_handle(TC_CAST(cls)));
	dbg_printf(LVL, " protocol ");
	print_protocol(rtnl_cls_get_protocol(cls));

	if (rtnl_cls_get_prio(cls))
		dbg_printf(LVL, " prio %u", rtnl_cls_get_prio(cls));

	print_parent(TC_CAST(cls));
	dbg_printf(LVL, " flower");

	dbg_printf(LVL, "\n");
}

/* callers need to take lock */
void netlink_debug_tc_qdisc_add(struct nl_cache *link_cache,
				struct rtnl_qdisc *qdisc)
{
	const char *kind;
	int i;
	int tmp;
	uint32_t tmp32;

	if (libponnet_dbg_lvl > LVL)
		return;

	kind = rtnl_tc_get_kind(TC_CAST(qdisc));

	dbg_naked(LVL, "tc qdisc add");

	print_dev(link_cache, TC_CAST(qdisc));

	if (kind && strcmp(kind, "clsact") == 0) {
		dbg_printf(LVL, " clsact");
	} else {
		print_parent(TC_CAST(qdisc));
		dbg_printf(LVL, " handle ");
		print_handle(rtnl_tc_get_handle(TC_CAST(qdisc)));

		dbg_printf(LVL, " %s", kind);
	}

	if (kind && strcmp(kind, "prio") == 0) {
		int bands;
		uint8_t *map;

		bands = rtnl_qdisc_prio_get_bands(qdisc);
		dbg_printf(LVL, " bands %d", bands);

		map = rtnl_qdisc_prio_get_priomap(qdisc);
		if (map) {
			dbg_printf(LVL, " priomap");
			for (i = 0; i < bands; i++)
				dbg_printf(LVL, " %u", map[i]);
		}
	}

	if (kind && strcmp(kind, "red") == 0) {
		int burst;
		/* limit bytes */
		tmp = rtnl_qdisc_red_get_limit(qdisc);
		if (tmp >= 0)
			dbg_printf(LVL, " limit %d", tmp);
		/* [ min bytes ] */
		tmp = rtnl_qdisc_red_get_min(qdisc);
		if (tmp >= 0)
			dbg_printf(LVL, " min %d", tmp);
		burst = 2 * tmp;
		/* [ max bytes ] */
		tmp = rtnl_qdisc_red_get_max(qdisc);
		if (tmp >= 0)
			dbg_printf(LVL, " max %d", tmp);
		burst += tmp;
		burst = burst / 3000;
		/* avpkt bytes - obligatory - but not implemented in libnl */
		dbg_printf(LVL, " avpkt 1k");
		/* [ burst packets ] - always in pair with avpkt */
		dbg_printf(LVL, " burst %d", burst);
		/* [ ecn ] not implemented */
		/* [ harddrop ] not implemented */
		/* [ nodrop ] not implemented */
		/* [ bandwidth rate ] not implemented */
		/* [ probability chance ] */
		tmp32 = (uint32_t)rtnl_qdisc_red_get_max_p(qdisc);
		dbg_printf(LVL, " probability %.3f",
			   (double)tmp32 / ((double)0x100000000UL));
		/* [ adaptive ] - not implemented */
	}

	if (kind && strcmp(kind, "tbf") == 0) {
		int rate, peakrate, limit;
		/* Rate in bytes per  second */
		rate = rtnl_qdisc_tbf_get_rate(qdisc);
		if (rate >= 0)
			dbg_printf(LVL, " rate %d", rate / 1024);
		limit = rtnl_qdisc_tbf_get_limit(qdisc);
		if (limit >= 0)
			dbg_printf(LVL, " limit %d", limit);
		peakrate = rtnl_qdisc_tbf_get_peakrate(qdisc);
		if (peakrate >= 0)
			dbg_printf(LVL, " peakrate %d", peakrate / 1024);
	}

	dbg_printf(LVL, "\n");
}

/* callers need to take lock */
void netlink_debug_tc_qdisc_del(struct nl_cache *link_cache,
				struct rtnl_qdisc *qdisc)
{
	uint32_t handle = 0;

	if (libponnet_dbg_lvl > LVL)
		return;

	dbg_naked(LVL, "tc qdisc del");
	print_dev(link_cache, TC_CAST(qdisc));
	print_parent(TC_CAST(qdisc));
	if (rtnl_tc_get_handle(TC_CAST(qdisc))) {
		dbg_printf(LVL, " handle ");
		handle = rtnl_tc_get_handle(TC_CAST(qdisc));

		dbg_printf(LVL, "%x:", (handle >> 16) & 0xFFFF);
	}
	dbg_printf(LVL, "\n");
}

/* callers need to take lock */
void netlink_debug_tc_class_add(struct nl_cache *link_cache,
				struct rtnl_class *class)
{
	const char *kind;
	uint32_t tmp = 0;
	int ret;

	if (libponnet_dbg_lvl > LVL)
		return;

	dbg_naked(LVL, "tc class add");
	print_dev(link_cache, TC_CAST(class));
	print_parent(TC_CAST(class));
	dbg_printf(LVL, " handle ");
	print_handle(rtnl_tc_get_handle(TC_CAST(class)));

	kind = rtnl_tc_get_kind(TC_CAST(class));
	if (kind)
		dbg_printf(LVL, " %s", kind);

	if (kind && strcmp(kind, "drr") == 0) {
		ret = rtnl_drr_get_quantum(class, &tmp);
		if (!ret)
			dbg_printf(LVL, " quantum %dk", tmp / 1024);
		else
			dbg_printf(LVL, " quantum (error %d)", ret);
	} else if (kind) {
		dbg_printf(LVL, " ...");
	}

	dbg_printf(LVL, "\n");
}

/* callers need to take lock */
void netlink_debug_tc_class_del(struct nl_cache *link_cache,
				struct rtnl_class *class)
{
	if (libponnet_dbg_lvl > LVL)
		return;

	dbg_naked(LVL, "tc class del");
	print_dev(link_cache, TC_CAST(class));
	if (rtnl_tc_get_handle(TC_CAST(class))) {
		dbg_printf(LVL, " handle ");
		dbg_printf(LVL, "0x%x", rtnl_tc_get_handle(TC_CAST(class)));
	}
	dbg_printf(LVL, "\n");
}

/* TODO: This should accept rtnl_link and not netlink_gem */
void netlink_debug_ip_link_gem(const char *ifname, const char *action,
			       const struct netlink_gem *gem)
{
	if (libponnet_dbg_lvl > LVL)
		return;

	if (strcmp(action, "add") == 0)
		dbg_naked(LVL, "ip link add link pon0 %s type gem", ifname);
	else
		dbg_naked(LVL, "ip link set %s type gem", ifname);

	if (gem->tcont_meid != NETLINK_GEM_UNUSED)
		dbg_printf(LVL, " tcont tcont%d", gem->tcont_meid);

	if (gem->port_id != NETLINK_GEM_UNUSED)
		dbg_printf(LVL, " id %d", gem->port_id);

	if (gem->traffic_type != NETLINK_GEM_UNUSED) {
		dbg_printf(LVL, " traffic_type");
		switch (gem->traffic_type) {
		case NETLINK_GEM_TRAFFIC_TYPE_ETH:
			dbg_printf(LVL, " eth");
			break;
		case NETLINK_GEM_TRAFFIC_TYPE_OMCI:
			dbg_printf(LVL, " omci");
			break;
		case NETLINK_GEM_TRAFFIC_TYPE_ETHM:
			dbg_printf(LVL, " ethm");
			break;
		}
	}

	if (gem->dir != NETLINK_GEM_UNUSED) {
		dbg_printf(LVL, " dir");
		switch (gem->dir) {
		case NETLINK_GEM_DIR_DIS:
			dbg_printf(LVL, " dis");
			break;
		case NETLINK_GEM_DIR_US:
			dbg_printf(LVL, " us");
			break;
		case NETLINK_GEM_DIR_DS:
			dbg_printf(LVL, " ds");
			break;
		case NETLINK_GEM_DIR_BI:
			dbg_printf(LVL, " bi");
			break;
		}
	}

	if (gem->enc != NETLINK_GEM_UNUSED) {
		dbg_printf(LVL, " enc");
		switch (gem->enc) {
		case NETLINK_GEM_ENC_DIS:
			dbg_printf(LVL, " dis");
			break;
		case NETLINK_GEM_ENC_UC:
			dbg_printf(LVL, " uc");
			break;
		case NETLINK_GEM_ENC_BC:
			dbg_printf(LVL, " bc");
			break;
		case NETLINK_GEM_ENC_DSEN:
			dbg_printf(LVL, " dsen");
			break;
		}
	}

	if (gem->mc != NETLINK_GEM_UNUSED)
		dbg_printf(LVL, " mc %s", gem->mc ? "en" : "dis");

	if (gem->max_size != NETLINK_GEM_UNUSED)
		dbg_printf(LVL, " max_size %d", gem->max_size);

	dbg_printf(LVL, "\n");
}

void netlink_debug_ip_link_add_tcont(const char *pon0,
				     const char *name,
				     unsigned int tcont_id)
{
	if (libponnet_dbg_lvl > LVL)
		return;

	dbg_naked(LVL, "ip link add link %s %s type tcont id %d\n",
		  pon0, name, tcont_id);
}

void netlink_debug_ip_link_add_gem_tcont(const char *pon0,
					 const char *name,
					 unsigned int gem_id,
					 const char *tcont)
{

	if (libponnet_dbg_lvl > LVL)
		return;

	dbg_naked(LVL, "ip link add link %s %s type gem id %d tcont %s\n",
		  pon0, name, gem_id, tcont);
}

void netlink_debug_ip_link_add_pmapper(const char *pon0,
				       const char *name,
				       int pcpdef)
{
	if (libponnet_dbg_lvl > LVL)
		return;

	dbg_naked(LVL, "ip link add link %s %s type pmapper pcpdef %d\n",
		  pon0, name, pcpdef);
}

void netlink_debug_ip_link_set_pmapper(const char *name,
				       uint16_t *gem_ctp_me_ids,
				       unsigned int count,
				       uint8_t *dscp_to_pbit)
{
	unsigned int i = 0;

	if (libponnet_dbg_lvl > LVL)
		return;

	dbg_naked(LVL, "ip link set dev %s type pmapper", name);
	for (i = 0; i < count; i++) {
		char ifname[IF_NAMESIZE];
		/* 0xFFFF is a NULL pointer, as specified in OMCI */
		if (gem_ctp_me_ids[i] == 0xFFFF) {
			dbg_printf(LVL, " pcp %d drop", i);
		} else {
			snprintf(ifname, sizeof(ifname), PON_IFNAME_GEM,
				 gem_ctp_me_ids[i]);

			dbg_printf(LVL, " pcp %d %s", i, ifname);
		}

	}

	if (dscp_to_pbit)
		for (i = 0; i < DSCP_MAX; ++i)
			dbg_printf(LVL, " dscp %u %u", i, dscp_to_pbit[i]);

	dbg_printf(LVL, "\n");
}

void netlink_debug_ip_link_set(const char *name, const char *options)
{
	if (libponnet_dbg_lvl > LVL)
		return;

	dbg_naked(LVL, "ip link set dev %s %s\n", name, options);
}

void netlink_debug_ip_link_del(const char *name)
{
	if (libponnet_dbg_lvl > LVL)
		return;

	dbg_naked(LVL, "ip link del dev %s\n", name);
}

void netlink_debug_ip_link_bridge_add(const char *name,
				      const struct netlink_bridge *bridge)
{
	if (libponnet_dbg_lvl > LVL)
		return;

	dbg_naked(LVL, "ip link add dev %s type bridge", name);

	if (bridge) {
		if (bridge->mask & NETLINK_BRIDGE_vlan_filtering)
			dbg_printf(LVL, " vlan_filtering %d",
				   bridge->vlan_filtering ? 1 : 0);
		if (bridge->mask & NETLINK_BRIDGE_vlan_protocol) {
			dbg_printf(LVL, " vlan_protocol ");
			print_protocol(bridge->vlan_protocol);
		}
		if (bridge->mask & NETLINK_BRIDGE_vlan_stats_enabled)
			dbg_printf(LVL, " vlan_stats_enabled %d",
				   bridge->vlan_stats_enabled ? 1 : 0);
	}

	dbg_printf(LVL, "\n");
}

void netlink_debug_ip_link_set_mtu(const char *name, unsigned int mtu)
{
	if (libponnet_dbg_lvl > LVL)
		return;

	dbg_naked(LVL, "ip link set dev %s mtu %u\n", name, mtu);
}

void netlink_debug_ip_link_set_macaddr(const char *name,
				       const uint8_t if_mac[ETH_ALEN])
{
	if (libponnet_dbg_lvl > LVL)
		return;

	dbg_naked(LVL, "ip link set dev %s address %02X:%02X:%02X:%02X:%02X:%02X\n",
		  name, if_mac[0], if_mac[1], if_mac[2], if_mac[3], if_mac[4],
		  if_mac[5]);
}

void netlink_debug_ip_link_set_master(const char *master, const char *slave)
{
	if (libponnet_dbg_lvl > LVL)
		return;

	if (strnlen_s(master, IF_NAMESIZE) > 0)
		dbg_naked(LVL, "ip link set dev %s master %s\n", slave, master);
	else
		dbg_naked(LVL, "ip link set dev %s nomaster\n", slave);

}

void netlink_debug_bridge_vlan(const char *ifname, uint16_t vlan, bool self,
			       bool del)
{
	if (libponnet_dbg_lvl > LVL)
		return;

	dbg_naked(LVL, "bridge vlan %s dev %s vid %u %s\n", del ? "del" : "add",
		  ifname, vlan, self ? "self" : "master");
}

void netlink_debug_bridge_link_set(const char *ifname, const char *options)
{
	if (libponnet_dbg_lvl > LVL)
		return;

	dbg_naked(LVL, "bridge link set dev %s %s\n", ifname, options);
}
