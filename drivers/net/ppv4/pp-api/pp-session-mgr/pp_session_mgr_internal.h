/*
 * Copyright (C) 2020-2025 MaxLinear, Inc.
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
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: PP session manager internal definitions
 */

#ifndef __PP_SESSION_MGR_INTERNAL_H__
#define __PP_SESSION_MGR_INTERNAL_H__

#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/bitops.h>
#include <linux/pp_api.h>
#ifdef CONFIG_DEBUG_FS
#include <linux/pktprs.h>
#endif

#include "pp_si.h"
#include "pp_fv.h"
#include "uc.h"
#include "pp_session_mgr.h"
#include "pp_dev.h"

/**
 * @define DDR cache line size in bytes
 */
#define CACHE_LINE_SIZE 64

/**
 * @define Maximum number of sessions supported by the PP HW, derived
 *        from the SI session index field which is 3 bytes where
 *        0xffffff cannot be used
 */
#define SMGR_MAX_HW_SESSIONS    (0xFFFFFE)
/**
 * @define Invalid session id
 */
#define SMGR_INVALID_SESS_ID    (0xFFFFFF)

/**
 * @enum session manager sessions flags, these flags are set in the
 *       session db entry
 * @ROUTED specifying if session is a routed session, session is
 *         considered to be a routed session if the mac destination
 *         address has changed from ingress to egress
 * @SYNCQ specifying if session has a sync q attached
 * @MTU_CHCK specifying if MTU should be checked on the session
 * @MCAST_GRP specifying if session is a multicast group session
 *            (first cycle)
 * @MCAST_DST specifying if session is a multicast dst session
 *            (second cycle)
 * @TDOX_SUPP specifying if session support ack suppression
 * @REMARK specifying if session supports dscp remarking
 * @LLD    specifying if session is a LLD session
 * @LRO    specifying if this is LRO session
 * @AQM_SW specifying if SW AQM should be enabled for this session
 */
enum smgr_sess_flags {
	SESS_FLAG_ROUTED,
	SESS_FLAG_SYNCQ,
	SESS_FLAG_MTU_CHCK,
	SESS_FLAG_MCAST_GRP,
	SESS_FLAG_MCAST_DST,
	SESS_FLAG_TDOX_SUPP,
	SESS_FLAG_REMARK,
	SESS_FLAG_LLD,
	SESS_FLAG_LRO,
	SESS_FLAG_AQM_SW,
	SMGR_FLAGS_NUM,
};

/**
 * @brief Shortcuts for session entry flags operations
 * @param e session db entry pointer (struct sess_db_entry)
 * @param f flag
 */
#define SESS_FLAG_SET(e, f)                         set_bit(f, &(e)->info.flags)
#define SESS_FLAG_CLR(e, f)                       clear_bit(f, &(e)->info.flags)

/**
 * @brief Shortcuts for session entry flags testing
 * @param s session db entry pointer (struct sess_db_entry)
 * @param f flag
 */
#define SESS_IS_FLAG_ON(e, f)                      test_bit(f, &(e)->info.flags)
#define SESS_IS_FLAG_OFF(e, f)                    !test_bit(f, &(e)->info.flags)

/**
 * @brief Shortcuts for session args flags testing
 * @param s session info pointer
 * @param f flag
 */
#define SESS_ARGS_IS_FLAG_ON(s, f)                test_bit(f, &(s)->args->flags)
#define SESS_ARGS_IS_FLAG_OFF(s, f)              !test_bit(f, &(s)->args->flags)

#define HDR_OUTER                                              PKTPRS_HDR_LEVEL0
#define HDR_INNER                                              PKTPRS_HDR_LEVEL1

#define L2_MASK                                 (BIT_ULL(PKTPRS_PROTO_MAC)   | \
                                                 BIT_ULL(PKTPRS_PROTO_VLAN0) | \
                                                 BIT_ULL(PKTPRS_PROTO_VLAN1) | \
                                                 BIT_ULL(PKTPRS_PROTO_VLAN2) | \
                                                 BIT_ULL(PKTPRS_PROTO_VLAN3) | \
                                                 BIT_ULL(PKTPRS_PROTO_VLAN4) | \
                                                 BIT_ULL(PKTPRS_PROTO_PPPOE) | \
                                                 BIT_ULL(PKTPRS_PROTO_PAYLOAD))

#define IPV6_OPT_MASK                        (BIT_ULL(PKTPRS_PROTO_HOP_OPT)  | \
                                              BIT_ULL(PKTPRS_PROTO_DEST_OPT) | \
                                              BIT_ULL(PKTPRS_PROTO_ROUT_OPT) | \
                                              BIT_ULL(PKTPRS_PROTO_FRAG_OPT))

/**
 * @brief Shortcut for accessing the session's rx packet header
 * @param s session info pointer
 */
#define SESS_RX_PKT(s)                      ((struct pktprs_hdr *)(s)->args->rx)

/**
 * @brief Shortcut for accessing the session's tx packet header
 * @param s session info pointer
 */
#define SESS_TX_PKT(s)                      ((struct pktprs_hdr *)(s)->args->tx)

/**
 * @brief Shortcut for accessing the session's packets protocols bitmaps
 * @param s session info pointer
 */
#define SESS_RX_OUTER_BMAP(s)              SESS_RX_PKT(s)->proto_bmap[HDR_OUTER]
#define SESS_RX_INNER_BMAP(s)              SESS_RX_PKT(s)->proto_bmap[HDR_INNER]
#define SESS_TX_OUTER_BMAP(s)              SESS_TX_PKT(s)->proto_bmap[HDR_OUTER]
#define SESS_TX_INNER_BMAP(s)              SESS_TX_PKT(s)->proto_bmap[HDR_INNER]

/**
 * @brief Shortcut for accessing the session's packets protocols bitmaps w/o l2
 * @param s session info pointer
 */
#define SESS_RX_OUTER_BMAP_NO_L2(s)      (SESS_RX_OUTER_BMAP(s) & ~L2_MASK)
#define SESS_RX_INNER_BMAP_NO_L2(s)      (SESS_RX_INNER_BMAP(s) & ~L2_MASK)
#define SESS_TX_OUTER_BMAP_NO_L2(s)      (SESS_TX_OUTER_BMAP(s) & ~L2_MASK)
#define SESS_TX_INNER_BMAP_NO_L2(s)      (SESS_TX_INNER_BMAP(s) & ~L2_MASK)

/**
 * @brief Shortcut for accessing the session's packets protocols bitmaps w/o l2
 *        and w/o ipv6 header extensions
 * @param s session info pointer
 */
#define SESS_RX_OUTER_BMAP_NO_L2_NO_IPV6_OPT(s)                                \
	(SESS_RX_OUTER_BMAP(s) & ~(L2_MASK | IPV6_OPT_MASK))
#define SESS_RX_INNER_BMAP_NO_L2_NO_IPV6_OPT(s)                                \
	(SESS_RX_INNER_BMAP(s) & ~(L2_MASK | IPV6_OPT_MASK))

/**
 * @brief Shortcuts for checking if session is mac
 * @param s session info pointer
 */
#define SESS_RX_IS_OUTER_MAC(s)       PKTPRS_IS_MAC(SESS_RX_PKT(s), HDR_OUTER)
#define SESS_TX_IS_OUTER_MAC(s)       PKTPRS_IS_MAC(SESS_TX_PKT(s), HDR_OUTER)
#define SESS_RX_IS_INNER_MAC(s)       PKTPRS_IS_MAC(SESS_RX_PKT(s), HDR_INNER)
#define SESS_TX_IS_INNER_MAC(s)       PKTPRS_IS_MAC(SESS_TX_PKT(s), HDR_INNER)

/**
 * @brief Shortcuts for checking if session is pppoe
 * @param s session info pointer
 */
#define SESS_RX_IS_OUTER_PPPOE(s)     PKTPRS_IS_PPPOE(SESS_RX_PKT(s), HDR_OUTER)
#define SESS_TX_IS_OUTER_PPPOE(s)     PKTPRS_IS_PPPOE(SESS_TX_PKT(s), HDR_OUTER)
#define SESS_RX_IS_INNER_PPPOE(s)     PKTPRS_IS_PPPOE(SESS_RX_PKT(s), HDR_INNER)
#define SESS_TX_IS_INNER_PPPOE(s)     PKTPRS_IS_PPPOE(SESS_TX_PKT(s), HDR_INNER)

/**
 * @brief Shortcuts for checking if session is mpls
 * @param s session info pointer
 */
#define SESS_RX_IS_OUTER_MPLS(s)      PKTPRS_MPLS_EXIST(SESS_RX_PKT(s), HDR_OUTER)
#define SESS_TX_IS_OUTER_MPLS(s)      PKTPRS_MPLS_EXIST(SESS_TX_PKT(s), HDR_OUTER)

/**
 * @brief Shortcuts for checking if session is ipv4
 * @param s session info pointer
 */
#define SESS_RX_IS_OUTER_V4(s)         PKTPRS_IS_IPV4(SESS_RX_PKT(s), HDR_OUTER)
#define SESS_TX_IS_OUTER_V4(s)         PKTPRS_IS_IPV4(SESS_TX_PKT(s), HDR_OUTER)
#define SESS_RX_IS_INNER_V4(s)         PKTPRS_IS_IPV4(SESS_RX_PKT(s), HDR_INNER)
#define SESS_TX_IS_INNER_V4(s)         PKTPRS_IS_IPV4(SESS_TX_PKT(s), HDR_INNER)
#define SESS_RX_IS_OUTER_V6(s)         PKTPRS_IS_IPV6(SESS_RX_PKT(s), HDR_OUTER)
#define SESS_TX_IS_OUTER_V6(s)         PKTPRS_IS_IPV6(SESS_TX_PKT(s), HDR_OUTER)
#define SESS_RX_IS_INNER_V6(s)         PKTPRS_IS_IPV6(SESS_RX_PKT(s), HDR_INNER)
#define SESS_TX_IS_INNER_V6(s)         PKTPRS_IS_IPV6(SESS_TX_PKT(s), HDR_INNER)

#define SESS_TX_OUTER_V4_NEXT(s)                                               \
	PKTPRS_PROTO_NEXT(SESS_TX_PKT(s), PKTPRS_PROTO_IPV4, HDR_OUTER)

#define SESS_TX_OUTER_V6_NEXT(s)                                               \
	PKTPRS_PROTO_NEXT(SESS_TX_PKT(s), PKTPRS_PROTO_IPV6, HDR_OUTER)

#define SESS_TX_OUTER_IP_NEXT(s)                                               \
	(SESS_TX_IS_OUTER_V4(s) ? SESS_TX_OUTER_V4_NEXT(s) :                   \
					SESS_TX_OUTER_V6_NEXT(s))

#define SESS_RX_OUTER_V4_NEXT(s)                                               \
	PKTPRS_PROTO_NEXT(SESS_RX_PKT(s), PKTPRS_PROTO_IPV4, HDR_OUTER)

#define SESS_RX_OUTER_V6_NEXT(s)                                               \
	PKTPRS_PROTO_NEXT(SESS_RX_PKT(s), PKTPRS_PROTO_IPV6, HDR_OUTER)

#define SESS_RX_OUTER_IP_NEXT(s)                                               \
	(SESS_RX_IS_OUTER_V4(s) ? SESS_RX_OUTER_V4_NEXT(s) :                   \
					SESS_RX_OUTER_V6_NEXT(s))

/**
 * @brief Shortcuts for checking if session is ip
 * @param s session info pointer
 */
#define SESS_RX_IS_OUTER_IP(s) \
(SESS_RX_IS_OUTER_V4(s) || SESS_RX_IS_OUTER_V6(s))
#define SESS_TX_IS_OUTER_IP(s) \
(SESS_TX_IS_OUTER_V4(s) || SESS_TX_IS_OUTER_V6(s))
#define SESS_RX_IS_INNER_IP(s) \
(SESS_RX_IS_INNER_V4(s) || SESS_RX_IS_INNER_V6(s))
#define SESS_TX_IS_INNER_IP(s) \
(SESS_TX_IS_INNER_V4(s) || SESS_TX_IS_INNER_V6(s))

/**
 * @brief Shortcuts for checking if session is udp
 * @param s session info pointer
 */
#define SESS_RX_IS_OUTER_UDP(s)         PKTPRS_IS_UDP(SESS_RX_PKT(s), HDR_OUTER)
#define SESS_TX_IS_OUTER_UDP(s)         PKTPRS_IS_UDP(SESS_TX_PKT(s), HDR_OUTER)
#define SESS_RX_IS_INNER_UDP(s)         PKTPRS_IS_UDP(SESS_RX_PKT(s), HDR_INNER)
#define SESS_TX_IS_INNER_UDP(s)         PKTPRS_IS_UDP(SESS_TX_PKT(s), HDR_INNER)

/**
 * @brief Shortcuts for checking if session is tcp
 * @param s session info pointer
 */
#define SESS_RX_IS_OUTER_TCP(s)         PKTPRS_IS_TCP(SESS_RX_PKT(s), HDR_OUTER)
#define SESS_TX_IS_OUTER_TCP(s)         PKTPRS_IS_TCP(SESS_TX_PKT(s), HDR_OUTER)
#define SESS_RX_IS_INNER_TCP(s)         PKTPRS_IS_TCP(SESS_RX_PKT(s), HDR_INNER)
#define SESS_TX_IS_INNER_TCP(s)         PKTPRS_IS_TCP(SESS_TX_PKT(s), HDR_INNER)

/**
 * @brief Shortcuts for checking if session is esp
 * @param s session info pointer
 */
#define SESS_RX_IS_OUTER_ESP(s)         PKTPRS_IS_ESP(SESS_RX_PKT(s), HDR_OUTER)
#define SESS_TX_IS_OUTER_ESP(s)         PKTPRS_IS_ESP(SESS_TX_PKT(s), HDR_OUTER)
#define SESS_RX_IS_INNER_ESP(s)         PKTPRS_IS_ESP(SESS_RX_PKT(s), HDR_INNER)
#define SESS_TX_IS_INNER_ESP(s)         PKTPRS_IS_ESP(SESS_TX_PKT(s), HDR_INNER)

#define SESS_TX_ESP_NEXT(s) \
PKTPRS_PROTO_NEXT(SESS_TX_PKT(s), PKTPRS_PROTO_ESP, HDR_OUTER)

/**
 * @brief Shortcuts for checking if session is supported L4
 * @param s session info pointer
 */
#define SESS_RX_IS_OUTER_L4(s)                                                 \
	(SESS_RX_IS_OUTER_UDP(s) || SESS_RX_IS_OUTER_TCP(s))
#define SESS_TX_IS_OUTER_L4(s)                                                 \
	(SESS_TX_IS_OUTER_UDP(s) || SESS_TX_IS_OUTER_TCP(s))
#define SESS_RX_IS_INNER_L4(s)                                                 \
	(SESS_RX_IS_INNER_UDP(s) || SESS_RX_IS_INNER_TCP(s))
#define SESS_TX_IS_INNER_L4(s)                                                 \
	(SESS_TX_IS_INNER_UDP(s) || SESS_TX_IS_INNER_TCP(s))

/**
 * @brief Shortcuts for checking if session is l2 gre
 * @param s session info pointer
 */
#define SESS_RX_IS_L2_GRE(s)                    PKTPRS_IS_L2_GRE(SESS_RX_PKT(s))
#define SESS_TX_IS_L2_GRE(s)                    PKTPRS_IS_L2_GRE(SESS_TX_PKT(s))
#define SESS_IS_L2_GRE_ENCP(s)                                                 \
	(!SESS_RX_IS_L2_GRE(s) && SESS_TX_IS_L2_GRE(s))
#define SESS_IS_L2_GRE_DECP(s)                                                 \
	(SESS_RX_IS_L2_GRE(s) && !SESS_TX_IS_L2_GRE(s))

/**
 * @brief Shortcuts for checking if session is ip gre
 * @param s session info pointer
 */
#define SESS_RX_IS_IP_GRE(s)                    PKTPRS_IS_IP_GRE(SESS_RX_PKT(s))
#define SESS_TX_IS_IP_GRE(s)                    PKTPRS_IS_IP_GRE(SESS_TX_PKT(s))
#define SESS_IS_IP_GRE_ENCP(s)                                                 \
	(!SESS_RX_IS_IP_GRE(s) && SESS_TX_IS_IP_GRE(s))
#define SESS_IS_IP_GRE_DECP(s)                                                 \
	(SESS_RX_IS_IP_GRE(s) && !SESS_TX_IS_IP_GRE(s))

/**
 * @brief Shortcuts for checking if session is dslite
 * @param s session info pointer
 */
#define SESS_RX_IS_DSLITE(s)                    PKTPRS_IS_DSLITE(SESS_RX_PKT(s))
#define SESS_TX_IS_DSLITE(s)                    PKTPRS_IS_DSLITE(SESS_TX_PKT(s))

/**
 * @brief Shortcuts for checking if session is vxlan
 * @param s session info pointer
 */
#define SESS_RX_IS_VXLAN(s)           PKTPRS_IS_VXLAN(SESS_RX_PKT(s), HDR_OUTER)
#define SESS_TX_IS_VXLAN(s)           PKTPRS_IS_VXLAN(SESS_TX_PKT(s), HDR_OUTER)

/**
 * @brief Shortcuts for checking if session is sixrd
 * @param s session info pointer
 */
#define SESS_RX_IS_SIXRD(s)                      PKTPRS_IS_SIXRD(SESS_RX_PKT(s))
#define SESS_TX_IS_SIXRD(s)                      PKTPRS_IS_SIXRD(SESS_TX_PKT(s))

/**
 * @brief Shortcuts for checking if session is l2tp over udp
 * @param s session info pointer
 */
#define SESS_RX_IS_L2TP_OUDP(s)   PKTPRS_IS_L2TP_OUDP(SESS_RX_PKT(s), HDR_OUTER)
#define SESS_TX_IS_L2TP_OUDP(s)   PKTPRS_IS_L2TP_OUDP(SESS_TX_PKT(s), HDR_OUTER)
#define SESS_IS_L2TP_OUDP_ENCP(s)                                              \
	(!SESS_RX_IS_L2TP_OUDP(s) && SESS_TX_IS_L2TP_OUDP(s))

/**
 * @brief Shortcuts for checking if session is ppp
 * @param s session info pointer
 */
#define SESS_RX_IS_PPP(s)               PKTPRS_IS_PPP(SESS_RX_PKT(s), HDR_OUTER)
#define SESS_TX_IS_PPP(s)               PKTPRS_IS_PPP(SESS_TX_PKT(s), HDR_OUTER)

/**
 * @brief Shortcuts for supported L3 bypass sessions
 */
#define SESS_BYPASS_IPV4_TCP           (BIT_ULL(PKTPRS_PROTO_IPV4) | \
                                        BIT_ULL(PKTPRS_PROTO_TCP))

#define SESS_BYPASS_IPV6_TCP           (BIT_ULL(PKTPRS_PROTO_IPV6) | \
                                        BIT_ULL(PKTPRS_PROTO_TCP))

#define SESS_BYPASS_IPV4_UDP           (BIT_ULL(PKTPRS_PROTO_IPV4) | \
                                        BIT_ULL(PKTPRS_PROTO_UDP))

#define SESS_BYPASS_IPV6_UDP           (BIT_ULL(PKTPRS_PROTO_IPV6) | \
                                        BIT_ULL(PKTPRS_PROTO_UDP))

#define SESS_BYPASS_IPV4_UDP_VXLAN     (BIT_ULL(PKTPRS_PROTO_IPV4) | \
                                        BIT_ULL(PKTPRS_PROTO_UDP)  | \
                                        BIT_ULL(PKTPRS_PROTO_VXLAN))

#define SESS_BYPASS_IPV6_UDP_VXLAN     (BIT_ULL(PKTPRS_PROTO_IPV6) | \
                                        BIT_ULL(PKTPRS_PROTO_UDP)  | \
                                        BIT_ULL(PKTPRS_PROTO_VXLAN))

#define SESS_BYPASS_IPV4_L2TP_OUDP     (BIT_ULL(PKTPRS_PROTO_IPV4)      | \
                                        BIT_ULL(PKTPRS_PROTO_UDP)       | \
                                        BIT_ULL(PKTPRS_PROTO_L2TP_OUDP) | \
                                        BIT_ULL(PKTPRS_PROTO_PPP))

#define SESS_BYPASS_IPV6_L2TP_OUDP     (BIT_ULL(PKTPRS_PROTO_IPV6)      | \
                                        BIT_ULL(PKTPRS_PROTO_UDP)       | \
                                        BIT_ULL(PKTPRS_PROTO_L2TP_OUDP) | \
                                        BIT_ULL(PKTPRS_PROTO_PPP))

#define SESS_BYPASS_IPV4_GRE           (BIT_ULL(PKTPRS_PROTO_IPV4) | \
                                        BIT_ULL(PKTPRS_PROTO_GRE))

#define SESS_BYPASS_IPV6_GRE           (BIT_ULL(PKTPRS_PROTO_IPV6) | \
                                        BIT_ULL(PKTPRS_PROTO_GRE))

#define SESS_BYPASS_IPV4_ESP           (BIT_ULL(PKTPRS_PROTO_IPV4) | \
                                        BIT_ULL(PKTPRS_PROTO_ESP))

#define SESS_BYPASS_IPV6_ESP           (BIT_ULL(PKTPRS_PROTO_IPV6) | \
                                        BIT_ULL(PKTPRS_PROTO_ESP))

#define SESS_BYPASS_IPV4                BIT_ULL(PKTPRS_PROTO_IPV4)

#define SESS_BYPASS_IPV6                BIT_ULL(PKTPRS_PROTO_IPV6)

#define SESS_BYPASS_ESP                 BIT_ULL(PKTPRS_PROTO_ESP)

/**
 * @enum Session manager mode
 * @disable disable mode, no sessions are created in this mode
 * @enable enable mode
 */
enum smgr_state {
	SMGR_DISABLE,
	SMGR_ENABLE,
	SMGR_STATES_NUM,
};

/**
 * @enum inact_scan_state
 * @brief Inactive sessions scan states
 */
enum inact_scan_state {
	INACT_SCAN_DISABLE,
	INACT_SCAN_IDLE,
	INACT_SCAN_BUSY,
};

/**
 * @brief tdox debug statistics
 */
struct tdox_dbg_stats {
	/*! Debug */
	u32 supp_list_full;
	u32 prio_list_full;
	u32 cand_list_full;
	u32 supp_full_state;
	u32 switch_q;
};

/**
 * @struct smgr_tdox_stats
 * @brief tdox statistics
 */
struct smgr_tdox_stats {
	u32 free_list_cnt;
	u32 init_list_cnt;
	u32 supp_list_cnt;
	u32 prio_list_cnt;
	u32 cand_list_cnt;
	u32 lro_list_cnt;
	u32 supp_ids_cnt;
	struct tdox_dbg_stats dbg;
};

struct smgr_tdox_uc_stats {
	u32 dccm_addr;
	u32 stage1;
	u32 stage2;
	u32 stored;
	u32 forward;
	u32 aggressive;
	u32 low_timers;
	u32 high_timers;
	u32 expired;
	u32 create;
	u32 remove;
	u32 recycle;
	u32 recycle_next_op;
	u32 errors;
	u32 max_ratio;
	u32 max_supp;
	u32 max_reach_target;
	u32 timer_cancel;
	u32 s2_fwd1;
	u32 s2_fwd2;
	u32 set_timer;
};

/**
 * @brief tdox conf parameters definitions
 */
struct smgr_tdox_conf {
	u32 timeout;
	u32 max_reach_target;
	u32 max_supp_ratio;
	u32 max_supp_bytes;
};

/**
 * @brief lro conf parameters definitions
 */
struct smgr_lro_conf {
	u32 pid;
	u16 tx_q;
	u16 max_agg_bytes;
	u16 max_agg_pkts;
	u16 headroom;
	u16 tailroom;
	u16 subif;
	u16 policy;
	u16 pool;
};

/**
 * @struct pp_lro_ps
 * @brief LRO PS data
 */
struct pp_lro_ps {
	u32 tcp_seq : 24,
		l3_off  : 8;
	u8 pl_off   : 7,
	   ip_ver   : 1;
	u8 fid;
} __packed;

/**
 * @brief Session manager database definitions
 * @n_sessions number of sessions that the session manager supports
 * @lock spin lock
 * @open_map bitmap specifying which sessions are currently
 *        opened
 * @ipv4_map bitmap specifying which session are ipv4 sessions
 * @ipv6_map bitmap specifying which session are ipv6 sessions
 * @tcp_map bitmap specifying which session are tcp sessions
 * @udp_map bitmap specifying which session are udp sessions
 * @inact_map bitmap of size n_sessions bits to use for scanning for
 *            inactive sessions
 * @inact_map_phys
 * @inact_map_sz inactive bitmap size in bytes
 * @scan_state inactive sessions scan state
 * @state session manager state, disable/enable
 * @free_list free session list head
 * @sessions sessions array
 * @stats session manager statistics
 * @in_ports per ingress ports associated sessions list
 * @eg_ports per egress ports associated sessions list
 * @ev_args_cache event arguments cache
 * @work_args_cache async work arguments cache
 */
struct smgr_database {
	u32 n_sessions;
	spinlock_t lock; /*! locks database and HW accesses */
	ulong *open_bmap;
	ulong *ipv4_bmap;
	ulong *ipv6_bmap;
	ulong *tcp_bmap;
	ulong *udp_bmap;
	ulong *inact_bmap;
	dma_addr_t inact_bmap_phys;
	size_t inact_bmap_sz;
	enum inact_scan_state scan_state;
	enum smgr_state state;
	enum pp_accl_mode accl_mode;
	struct list_head free_list;
	struct sess_db_entry *sessions;
	struct smgr_stats stats;
	struct list_head in_port[PP_MAX_PORT];
	struct list_head eg_port[PP_MAX_PORT];
	struct kmem_cache *work_args_cache;
	struct workqueue_struct *workq;
	struct workqueue_struct *workq_hi;
	enum smgr_frag_mode frag_mode;
	bool open_frag_sess;
	bool open_lld_sess;
	bool syncq_en;
	void *tdox_db;
	void *sq_db;
	void *mcast_db;
	void *lro_db;
};

/**
 * @brief Session manager session database entry definition
 * @info session info
 * @free_node list node for attaching the session to the free sessions
 *            list
 * @in_port_node list note for attaching the session to ingress port
 *               sessions list
 * @in_port_node list note for attaching the session to ingress port
 *               sessions list
 */
struct sess_db_entry {
	struct sess_db_info info;
	struct list_head free_node;
	struct list_head in_port_node;
	struct list_head eg_port_node;
} __aligned(CACHE_LINE_SIZE);

/**
 * @struct sess_info
 * @brief Session info definition, it defines the input
 *        information received by the user to create the session
 *        and all the information collected along the session
 *        create flow
 */
struct sess_info {
	/*! session create args received by the user */
	struct pp_sess_create_args *args;
	/*! session database entry */
	struct sess_db_entry       *db_ent;
	/*! egress port configuration */
	struct pp_port_cfg eg_port_cfg;
	/*! ingress port configuration */
	struct pp_port_cfg in_port_cfg;
	/*! host si structure which is being built along the flow */
	struct pp_si                si;
	/*! hw si structure which was encoded using the host si, this is
	 *  the si which is used to the create the actual session in the HW
	 */
	struct pp_hw_si             hw_si;
	/*! pointer for pointing to the right modification flags variable
	 *  along the session create flow
	 */
	ulong                      *mod_flags;
	/*! Struct defines the info required for nat modifications */
	struct {
		u8  rx_lvl;
		u8  tx_lvl;
	} nat;
	/*! Specify if the session is consider to be bypassed.
	 *  L2 modification only allowed.
	 */
	u8 l3_bypass;

	u8 res;
};

/**
 * @define FRAG_INFO_FLAG defines, to be used with struct frag_info.flags
 */
#define FRAG_INFO_FLAG_INT_DF           (BIT(0))
#define FRAG_INFO_FLAG_EXT_DF           (BIT(1))
#define FRAG_INFO_FLAG_IGNORE_INT_DF    (BIT(2))
#define FRAG_INFO_FLAG_IGNORE_EXT_DF    (BIT(3))
#define FRAG_INFO_FLAG_FRAG_EXT         (BIT(4))
#define FRAG_INFO_FLAG_IPV4             (BIT(5))
#define FRAG_INFO_FLAG_PPPOE            (BIT(6))
#define FRAG_INFO_FLAG_REMARK           (BIT(7))

/**
 * struct frag_info - Fragmentation info
 */
struct frag_info {
	/*! flags */
	u8	flags;

	/*! l3 offset. In case of inner fragmentation,
	 *  the offset will be to the inner l3 hdr */
	u8	l3_off;

	/*! Port's max_pkt_size */
	u16	max_pkt_size;
} __packed;

/**
 * @define REMARK_INFO_FLAG defines, used with struct remark_info.flags
 */
#define REMARK_INFO_FLAG_OUT_IPV4   (BIT(0))
#define REMARK_INFO_FLAG_INNER_IP   (BIT(1))
#define REMARK_INFO_FLAG_INNER_IPV4 (BIT(2))

/**
 * struct remark_info - Fragmentation info
 */
struct remark_info {
	/*! flags*/
	u8	flags;

	/*! offset of first l3 header */
	u8	out_l3_off;

	/*! offset of second l3 header (if exist) */
	u8	innr_l3_off;

	/*! dscp remarking value */
	u8	dscp;
} __packed;

/**
 * struct si_ud_frag_remark_info - Remarking and/or Fragmentation info
 *
 * This structure defines the remarking and fragmentation information saved
 * in the si ud right after the ps (Starts at si.ud + PP_PS_REGION_SZ).
 * Used in the fragmentation/remarking uc
 */
struct si_ud_frag_remark_info {
	/*! Destination queue */
	u16 dst_q;

	/*! fragmentation fields */
	struct frag_info frag;

	/*! remarking fields */
	struct remark_info remark;
} __packed;

/**
 * @define LLD_INFO_FLAG defines, used with struct struct si_ud_aqm_lld_info {
.flags
 */
#define AQM_LLD_INFO_FLAG_OUT_IPV4       (BIT(0))
#define AQM_LLD_INFO_FLAG_AQM_SW         (BIT(1))
#define AQM_LLD_INFO_FLAG_LLD_PKT        (BIT(2))

/**
 * struct si_aqm_lld_bc_info - SW AQM/LLD ctrl info
 *
 * This structure defines the LLD/buffer control information saved
 * in the si ud right after the ps (Starts at si.ud + PP_PS_REGION_SZ).
 * Used in the LLD uc
 */
struct si_ud_aqm_lld_info {
	/*! Service Flow ID */
	u32 sf_id;
	/*! Destination queue */
	u16 dst_q;
	/*! classic queue (for sunction)*/
	u16 dst_cq;
	/*! flags */
	u8  flags;
	/*! Outer L3 offset */
	u8  l3_off;
	/*! LLD Context - total supported contexts is 8 (0 - 7) */
	u8  lld_ctx_id;
} __packed;

/**
 * struct si_ud_reass_info - Reassembly info
 *
 * This structure defines the reassembly information saved in the si ud
 * right after the ps (Starts at si.ud + PP_PS_REGION_SZ).
 * Used in the egress uc for reassembly
 */
struct si_ud_reass_info {
	/*! egress port headroom */
	u16 eg_port_hr;

	/*! egress port tailroom */
	u16 eg_port_tr;

	/*! base policy used by the rx dma */
	u8 base_policy;

	/*! port's policies bitmap specifying which policy is enabled (1)
	 *  or disabled (0) */
	u8  policies_map;

	/*! flags */
	u8 flags;
} __packed;

/**
 * @brief Test whether a session is routed session.<br>
 *        Routed session is a session where the ingress destination
 *        mac address is different then the egress one.
 * @param s the session to test
 * @return bool true if the session is routed, false otherwise
 */
static inline bool smgr_is_sess_routed(const struct sess_db_info *s)
{
	if (ptr_is_null(s))
		return false;

	return test_bit(SESS_FLAG_ROUTED, &s->flags);
}

/**
 * @brief Test whether a session is bridged session.<br>
 *        Bridged session is a session where the ingress destination
 *        mac address is equal to the egress one
 * @param s the session to test
 * @return bool true if the session is bridged, false otherwise
 */
static inline bool smgr_is_sess_bridged(const struct sess_db_info *s)
{
	if (ptr_is_null(s))
		return false;

	return !test_bit(SESS_FLAG_ROUTED, &s->flags);
}

/**
 * @brief Test whether a session is multicast group session
 * @param s the session to test
 * @return bool true if the session is multicast group, false
 *         otherwise
 */
static inline bool smgr_is_sess_mcast_grp(const struct sess_db_info *s)
{
	if (ptr_is_null(s))
		return false;

	return test_bit(SESS_FLAG_MCAST_GRP, &s->flags);
}

/**
 * @brief Test whether a session is multicast dst session
 * @param s the session to test
 * @return bool true if the session is multicast dst, false
 *         otherwise
 */
static inline bool smgr_is_sess_mcast_dst(const struct sess_db_info *s)
{
	if (ptr_is_null(s))
		return false;

	return test_bit(SESS_FLAG_MCAST_DST, &s->flags);
}

/**
 * @brief Test whether a session is L3 bypass session
 *        L3 Bypass session is a session where the ingress and
 *        egress packets are identical from L3 throughout the entire header
 *        (only L2 modifications allowed)
 * @param s the session to test
 * @return bool true if the session is bypass, false otherwise
 */
static inline bool smgr_is_sess_l3_bypass(const struct sess_info *s)
{
	if (ptr_is_null(s))
		return false;

	return !!s->l3_bypass;
}

/**
 * @brief Check whether a given acceleration mode is configured
 * @param accl_mode acceleration mode to check
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_accl_mode(u8 accl_mode)
{
	u8 mode;

	if (pp_accl_mode_get(&mode))
		goto error;

	if (mode == accl_mode)
		return true;

error:
	return false;
}

/**
 * @brief Test whether session id is valid or not
 * @note Valid session id doesn't mean the session exist
 * @param id session id to test
 * @return bool true if the id is a valid session id
 */
bool smgr_is_sess_id_valid(u32 id);

/**
 * @brief Get inactive sessions scan state (enable/disable)
 * @return bool true in case it is enabled, false otherwise
 */
bool smgr_sessions_scan_state_get(void);

/**
 * @brief Set the inactive sessions scan state
 * @param en enable/disable scan
 * @return s32 0 on success, error code otherwise
 */
s32 smgr_sessions_scan_state_set(bool en);

/**
 * @brief Get session SI
 * @param id session id
 * @param si buffer to write the si
 * @return s32 0 on success, error code otherwise
 */
s32 smgr_session_si_get(u32 id, struct pp_si *si);

/**
 * @brief Get session dynamic SI
 * @param id session id
 * @param dsi buffer to write the dsi
 * @return s32 0 on success, error code otherwise
 */
s32 smgr_session_dsi_get(u32 id, struct pp_dsi *dsi);

/**
 * @brief Get bitmap indicating which sessions uses the specified SGC
 * @param grp SGC group index
 * @param cntr SGC index
 * @param bmap bitmap buffer to save the sessions list
 * @param n_bits bitmap size in bits
 */
s32 smgr_sgc_sessions_bmap_get(u8 grp, u16 cntr, ulong *bmap, u32 n_bits);

/**
 * @brief Shortcut for converting QoS logical ID to physical ID
 * @param logical the logical ID
 * @param physical result physical ID
 * @return s32 0 on success, error code otherwise
 */
s32 smgr_get_queue_phy_id(u16 logical, u16 *physical);

/**
 * @brief Update a PP session without lock - internal
 * @param sess_id session id to update
 * @param si new si
 * @return s32 0 on success update, error code otherwise
 */
s32 __smgr_session_update(u32 sess_id, struct pp_hw_si *hw_si);

/**
 * @brief Update a PP session (with lock)
 * @param sess_id session id to update
 * @param si new si
 * @return s32 0 on success update, error code otherwise
 */
s32 smgr_session_update(u32 sess_id, struct pp_hw_si *hw_si);
/* ========================================================================== */
/*                       Synchronization queues                               */
/* ========================================================================== */
/**
 * @brief allocate new syncq
 * @note exported internally only for debufs
 * @param session session id
 * @param dst_queue_id dst queue id
 * @param sig hash signature
 * @return s32 0 on success, error code otherwise
 */
s32 sq_alloc(u32 session, u32 dst_queue_id, u32 sig);

/**
 * @brief allocate new syncq
 * @note called before the session is added to the hw,
 *       after the allocation the queue is available to use
 * @param sess session entry
 * @return s32 0 on success, error code otherwise
 */
s32 smgr_sq_alloc(struct sess_info *sess);

/**
 * @brief start the sync queue state machine
 * @note exported internally only for debufs
 * @param session session id
 * @return s32 0 on success, error code otherwise
 */
s32 sq_start(u32 session);
/**
 * @brief start the sync queue state machine
 * @note called after the session was added in HW
 * @param sess session entry
 * @return s32 0 on success, error code otherwise
 */
s32 smgr_sq_start(struct sess_info *sess);

/**
 * @brief delete the synch queue
 * @note exported internally only for debufs
 * @param session session id
 * @return s32 0 on success, error code otherwise
 */
s32 sq_del(u32 session);

/**
 * @brief delete the synch queue
 * @note called when session is being deleted
 * @param ent session db entry
 * @return s32 0 on success, error code otherwise
 */
s32 smgr_sq_del(struct sess_db_entry *ent);

/**
 * @brief print all the sync queue from database
 * @note debug only
 */
void smgr_sq_dbg_dump(void);

/**
 * @brief get the "lspp rcv timeout" in micro sec units
 * @note debug only
 * @note lspp: Last Slow Path Packet
 * @param tout timeout
 */
void smgr_sq_dbg_lspp_rcv_tout_get(u32 *tout);

/**
 * @brief set the "lspp rcv timeout" in micro sec units
 * @note debug only
 * @note lspp: Last Slow Path Packet
 * @param tout timeout
 */
void smgr_sq_dbg_lspp_rcv_tout_set(u32  tout);

/**
 * @brief get the "done timeout" in micro sec units
 * @note debug only
 * @param tout timeout
 */
void smgr_sq_dbg_done_tout_get(u32 *tout);

/**
 * @brief set the "done timeout" in micro sec units
 * @note debug only
 * @param tout timeout
 */
void smgr_sq_dbg_done_tout_set(u32  tout);

/**
 * @brief get the "lspp sent timeout" in micro sec units
 * @note debug only
 * @note lspp: Last Slow Path Packet
 * @param tout timeout
 */
void smgr_sq_dbg_lspp_sent_tout_get(u32 *tout);

/**
 * @brief set the "lspp sent timeout" in micro sec units
 * @note debug only
 * @note lspp: Last Slow Path Packet
 * @param tout timeout
 */
void smgr_sq_dbg_lspp_sent_tout_set(u32  tout);

/**
 * @brief get the max qlen
 * @param qlen queue length
 */
void smgr_sq_dbg_qlen_get(u32 *qlen);

/**
 * @brief set the max qlen
 * @param qlen queue length
 */
void smgr_sq_dbg_qlen_set(u32 qlen);

/**
 * @brief exit the sync queue module
 */
void smgr_sq_exit(void);

/* ========================================================================== */
/*                               Multicast                                    */
/* ========================================================================== */
/**
 * @brief Track session in the multicast group db
 * @param sess session to track
 */
void smgr_mcast_sess_track(struct sess_info *sess);

/**
 * @brief Untrack session from the multicast group db
 * @param sess session to untrack
 */
void smgr_mcast_sess_untrack(struct sess_db_entry *ent);

/**
 * @brief Test whether session args mcast info is valid, mcast
 *        info is considered to be invalid when the user set one
 *        of the mcast flags and the mcast info is
 *        invalid, another invalid option is to set more than
 *        one mcast flag
 * @param args the args to test
 * @return bool true in case mcast info is valid, false
 *         otherwise
 */
bool smgr_mcast_is_info_valid(struct pp_sess_create_args *args);

/**
 * @brief update the session args for multicast sessions
 *        oob information for mcast dst sessions
 *        ud information for mcast group sessions
 * @param sess session info
 * @return s32 0 on success, error code otherwise
 */
s32 smgr_mcast_sess_args_update(struct sess_info *sess);

/**
 * @brief Prepare a session multicast database entry
 * @param sess session info to save the session db entry
 * @note Caller MUST acquire database lock
 */
void smgr_mcast_sess_ent_prepare(struct sess_info *sess);

/**
 * @brief Perform session multicast lookup based on the
 *        multicast information (group or dst)
 * @param sess session info
 * @return s32 -EEXIST in case session was found, 0 if session wasn't
 *         found, error code otherwise
 */
s32 smgr_mcast_sess_lookup(struct sess_info *sess);

/**
 * @brief Helper function to initialize multicast info
 * @param dev device
 * @param invalid_sess_id invalid session index
 * @return s32 0 on success, error code otherwise
 */
s32 smgr_mcast_init(struct device *dev, u32 invalid_sess_id);

/**
 * @brief Clean multicast resources
 */
void smgr_mcast_exit(void);

/* ========================================================================== */
/*                               Turbodox                                     */
/* ========================================================================== */

/**
 * @brief Helper function to initialize tdox manager
 * @param dev device to use for all memories allocations
 * @param smgr_db holds tdox_db
 * @return s32 0 on success, error code otherwise
 */
s32 smgr_tdox_init(struct device *dev, struct smgr_database *smgr_db);

/**
 * @brief Set the uc nf physical queue id
 * @param phyq tdox uc nf physical queue id
 */
void smgr_tdox_nf_set(u16 phyq);

/**
 * @brief Set the tdox timeout
 * @param tout tdox timeout
 * @return s32 0 on success, error code otherwise
 */
s32 smgr_tdox_tout_set(u32 tout);

void smgr_si_tdox_set(struct sess_info * s);

/**
 * @brief Fetching tdox record info and sending mailbox to uC
 * @param s session info
 * @return 0 if success
 */
s32 smgr_tdox_start(struct sess_info *s);

/**
 * @brief Remove tdox entry
 * @param sess session db entry
 */
void smgr_tdox_session_remove(struct sess_db_entry *sess_db);

/**
 * @brief set tdox conf parameters
 * @param tdox conf
 */
s32 smgr_tdox_conf_set(struct smgr_tdox_conf *conf);

/**
 * @brief get tdox conf parameters
 * @param tdox conf
 */
s32 smgr_tdox_conf_get(struct smgr_tdox_conf *params);

/**
 * @brief read tdox entry for debug
 * @param file
 * @param entry index
 */
s32 smgr_tdox_debug_read_entry(struct seq_file *f, u32 index);

/**
 * @brief Set tdox enable state
 */
void smgr_tdox_enable_set(bool enable);

/**
 * @brief Get tdox enable state
 */
bool smgr_tdox_enable_get(void);

/**
 * @brief Get tdox stats
 * @param stats statistics structure
 * @return 0 if success
 */
s32 smgr_tdox_stats_get(struct smgr_tdox_stats *stats,
			struct smgr_tdox_uc_stats *uc_stats, bool reset_stats);

/**
 * @brief Clean tdox resources
 */
void smgr_tdox_exit(void);

/**
 * @brief return smgr_db from device's private data
 */
static inline struct smgr_database *smgr_get_db(void)
{
	struct pp_dev_priv *dev_priv = pp_priv_get();

	if (ptr_is_null(dev_priv))
		return NULL;

	return (struct smgr_database *)dev_priv->smgr_db;
}

/* ========================================================================== */
/*                                    LRO                                     */
/* ========================================================================== */

/**
 * @brief Helper function to initialize lro manager
 * @param dev device to use for all memories allocations
 * @param smgr_db holds tdox_db
 * @return s32 0 on success, error code otherwise
 */
s32 smgr_lro_init(struct device *dev, struct smgr_database *smgr_db);

/**
 * @brief Set the uc nf physical queue id
 * @param gpid lp dev gpid
 * @param subif lp dev subif
 * @param phyq lro uc nf physical queue id
 */
s32 smgr_lro_nf_set(u16 gpid, u16 subif, u16 phyq);

/**
 * @brief set lro conf parameters
 * @param lro conf
 */
s32 smgr_lro_conf_set(struct smgr_lro_conf *conf);

/**
 * @brief get lro conf parameters
 * @param lro conf
 */
s32 smgr_lro_conf_get(struct smgr_lro_conf *conf);

/**
 * @brief Clean lro resources
 */
void smgr_lro_exit(void);

#ifdef CONFIG_DEBUG_FS

/**
 * @brief Set PP session manager state (enable/disable)
 * @param state new state
 * @return s32 0 on successful state change, error code otherwise
 */
s32 smgr_state_set(enum smgr_state state);

/**
 * @brief Get session manager current state
 * @return enum smgr_state
 */
enum smgr_state smgr_state_get(void);

/**
 * @brief Set PP session manager syncq (enable/disable)
 * @param enable enable/disable
 * @return s32 0 on successful, error code otherwise
 */
s32 smgr_syncq_set(bool enable);

/**
 * @brief Get session manager syncq state
 * @return true/false
 */
bool smgr_syncq_get(void);

/**
 * @brief Open next sessions as frag sessions
 * @param enable enable - 1 / disable - 0
 * @return s32 0 on successful set, error code otherwise
 */
s32 smgr_open_frag_sess_set(bool enable);

/**
 * @brief Get open frag sess flag status
 * @return true/false according to open_frag_sess
 */
bool smgr_open_frag_sess_get(void);

/**
 * @brief Open next sessions as lld sessions
 * @param enable enable - 1 / disable - 0
 * @return s32 0 on successful set, error code otherwise
 */
s32 smgr_open_lld_sess_set(bool enable);

/**
 * @brief Get open lld sess flag status
 * @return true/false according to open_lld_sess
 */
bool smgr_open_lld_sess_get(void);

/**
 * @brief Get nf status in smgr
 * @param nf nf type
 * @return true if nf enabled
 */
bool smgr_is_nf_en(enum pp_nf_type nf);

/**
 * @brief Session manager debug init
 * @return s32 0 for success, non-zero otherwise
 */
s32 smgr_dbg_init(struct device *dev, struct dentry *parent);

/**
 * @brief create debugfs directory for tdox commands
 * @return s32 0 for success, non-zero otherwise
 */
s32 smgr_tdox_create_debugfs(struct dentry *dbgfs);

/**
 * @brief create debugfs directory for lro commands
 * @return s32 0 for success, non-zero otherwise
 */
s32 smgr_lro_create_debugfs(struct dentry *dbgfs);

/**
 * @brief Session manager debug cleanup
 * @return s32 0 for success, non-zero otherwise
 */
s32 smgr_dbg_clean(void);

#else /* !CONFIG_DEBUG_FS */
static inline s32 smgr_dbg_init(struct device *dev, struct dentry *parent)
{
	return 0;
}

static inline s32 smgr_dbg_clean(void)
{
	return 0;
}
#endif /* CONFIG_DEBUG_FS */
#endif /* __PP_SESSION_MGR_INTERNAL_H__ */
