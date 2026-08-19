/*
 * Copyright (C) 2020-2025 MaxLinear, Inc.
 * Copyright (C) 2019-2020 Intel Corporation
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
 * Description: PP session manager driver
 */

#define pr_fmt(fmt) "[PP_SMGR]:%s:%d: " fmt, __func__, __LINE__

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/bitops.h>
#include <linux/bitmap.h>
#include <linux/bug.h>
#include <linux/ipv6.h>
#include <net/ipv6.h>
#include <net/ip.h>
#include <linux/if_pppox.h>
#include <linux/if_vlan.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/workqueue.h>
#include <linux/jiffies.h>
#include <net/checksum.h>
#include <linux/pp_api.h>
#include <linux/pktprs.h>

#include "pp_desc.h"
#include "pp_si.h"
#include "pp_fv.h"
#include "uc.h"
#include "pp_common.h"
#include "pp_dma.h"
#include "classifier.h"
#include "checker.h"
#include "modifier.h"
#include "parser.h"
#include "pp_buffer_mgr.h"
#include "pp_port_mgr.h"
#include "pp_session_mgr.h"
#include "pp_session_mgr_internal.h"
#include "pp_misc.h"

/**
 * @struct base_args
 * @brief Basic session manager work arguments
 */
struct base_args {
	struct delayed_work dwork;    /*! linux delayed work instance */
	unsigned long       req_id;   /*! user request id */
	unsigned long       req_prio; /*! user request priority */
	pp_cb               cb;       /*! callback for sending request result */
};

/**
 * @struct Session create work arguments
 * @base mandatory basic work argument
 * @rx rx packet header
 * @tx tx packet header
 * @args session create args reference
 * @args_buf session create args buffer used in async mode to save
 *       the user args in order to work in async mode
 * @cb_args callback arguments, in sync request, this used by the
 *          session manager to return the answer to the user, in async
 *          request, it returned directly to the user
 */
struct sess_cr_work_args {
	struct base_args              base;
	struct pktprs_hdr             rx;
	struct pktprs_hdr             tx;
	struct pp_sess_create_args   *args;
	struct pp_sess_create_args    args_buf;
	struct pp_sess_create_cb_args cb_args;
};

/**
 * @struct Session delete work arguments
 * @base mandatory basic work argument
 * @cb_args callback arguments, in sync request, this used by the
 *          session manager to return the answer to the user, in async
 *          request, it returned directly to the user
 * @sess_id session id to delete
 */
struct sess_del_work_args {
	struct base_args              base;
	struct pp_sess_delete_cb_args cb_args;
	u32                           sess_id;
};

/**
 * @struct Port's sessions flush work arguments
 * @base mandatory basic work argument
 * @cb_args callback arguments, in sync request, this used by the
 *          session manager to return the answer to the user, in async
 *          request, it returned directly to the user
 * @port_id PP port id
 */
struct port_flush_work_args {
	struct base_args             base;
	struct pp_port_flush_cb_args cb_args;
	u16                          port_id;
};

/**
 * @struct inactive_work_args
 * @brief Get inactive sessions list work arguments
 *        HW is able to scan maximum of 'CHK_CRWLR_MAX_ITR_SIZE'
 *        sessions in 1 iteration, the sessions manager will scan all
 *        sessions by scheduling delayed tasks where each task will
 *        initiate a crawler scan and also schedule the next one. the
 *        last task will gather all the sessions indexes got from the
 *        crawler HW and will return an answer to the user
 */
struct inactive_work_args {
	struct base_args base;       /*! mandatory basic work argument*/
	u32             base_id;     /*! session id to start the scan from */
	u32             it_sz;       /*! number of sessions to scan per
				      * iteration
				      */
	u32             n_sessions;  /*! total number of sessions to scan */
	u32             delay;       /*! time in microseconds between
				      * iterations
				      */
	dma_addr_t      bmap_phys;   /*! bitmap for receiving stalled
				      *  sessions from checker
				      */
	size_t          bmap_sz;     /* bitmap size in bytes */
	u32            *inact;       /*! user's array of inactive sessions */
	u32             inact_n;     /*! user's array number of entries */
	u32             inact_found; /*! number of inactive sessions found */
};

/**
 * @brief Session manager work arguments for all types of works, this
 *        is used only for creating 1 cache for all types of works
 */
union work_args {
	struct base_args            base;
	struct sess_cr_work_args    cr;
	struct sess_del_work_args   del;
	struct port_flush_work_args flush;
	struct inactive_work_args   inactive;
};

/**
 * @brief Shortcuts for accessing the session's nat packets, nat
 *        packets are the packets which the nat changes are based on.
 * @param s session info pointer (struct sess_info)
 */

#define SESS_RX_NAT_LVL(s)                                     ((s)->nat.rx_lvl)
#define SESS_TX_NAT_LVL(s)                                     ((s)->nat.tx_lvl)

/**
 * @brief Shortcuts for session modification flags operations
 * @param s session info pointer (struct sess_info)
 * @param f flag
 */
#define SESS_MOD_FLAG_SET(s, f)                     set_bit(f,   (s)->mod_flags)
#define SESS_MOD_FLAG_CLR(s, f)                     clear_bit(f, (s)->mod_flags)
#define SESS_MOD_FLAG_TEST(s, f)                    test_bit(f, (s)->mod_flags)

/**
 * @brief Shortcuts for testing session flags
 * @param s session info pointer (struct sess_info)
 * @param f flag
 */
#define __smgr_is_sess_routed(s)         smgr_is_sess_routed(&(s)->db_ent->info)
#define __smgr_is_sess_bridged(s)        smgr_is_sess_bridged(&(s)->db_ent->info)
#define __smgr_is_sess_l3_bypass(s)      smgr_is_sess_l3_bypass(s)

/* functions declarations */
static s32 __smgr_port_flush(u16 pid);
static inline bool smgr_is_l2tpudp_sess_supported(struct sess_info *s);
static inline bool __smgr_is_ip_nat_addr_changed(struct sess_info *s);

/* Shortcuts for database accesses */
#define DB_FREE_LIST_GET_NODE(_db) \
	list_first_entry(&(_db)->free_list, struct sess_db_entry, free_node)
#define DB_FREE_LIST_ADD_NODE(_db, _ent) \
	list_add_tail(&(_ent)->free_node, &(_db)->free_list)
#define DB_FREE_LIST_DEL_NODE(_ent) \
	list_del_init(&(_ent)->free_node)
#define DB_IN_PORT_LIST_ADD_NODE(_db, _ent) \
	list_add_tail(&(_ent)->in_port_node, \
		      &(_db)->in_port[(_ent)->info.in_port])
#define DB_EG_PORT_LIST_ADD_NODE(_db, _ent) \
	list_add_tail(&(_ent)->eg_port_node, \
		      &(_db)->eg_port[(_ent)->info.eg_port])
#define DB_IN_PORT_LIST_DEL_NODE(_ent) \
	list_del_init(&(_ent)->in_port_node)
#define DB_EG_PORT_LIST_DEL_NODE(_ent) list_del_init(&(_ent)->eg_port_node)

/* ========================================================================== */
/*                               Shortcuts                                    */
/* ========================================================================== */

#define pr_pp_req(req, fmt, ...)                                               \
do {                                                                           \
	if (req)                                                               \
		pr_debug("req_id %lu, req_prio %lu, cb %p, " fmt,              \
			 req->req_id, req->req_prio, req->cb, ##__VA_ARGS__);  \
	else                                                                   \
		pr_debug("req NULL, " fmt, ##__VA_ARGS__);                     \
} while (0)

/**
 * @brief Acquire session manager lock
 */
static inline void __smgr_lock(void)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	/* we are not allowed to run in irq context */
	WARN(in_irq(), "session manager shouldn't be used from irq context\n");
	spin_lock_bh(&db->lock);
}

/**
 * @brief Release session manager lock
 */
static inline void __smgr_unlock(void)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	spin_unlock_bh(&db->lock);
}

/**
 * @brief Test whether the session manager is enable or not
 * @return bool true in case he is, false otherwise
 */
static inline bool __smgr_is_enable(void)
{
	struct smgr_database *db = smgr_get_db();

	if (!db)
		return false;

	return (pp_is_ready() && db->state == SMGR_ENABLE);
}

/**
 * @brief Test whether the session manager is disable or not
 * @return bool true in case he is, false otherwise
 */
static inline bool __smgr_is_disable(void)
{
	return !__smgr_is_enable();
}

/**
 * @brief Test whether session exist or not
 * @note Existing session doesn't mean he is active
 * @param id session id
 * @return bool true is session exist, false otherwise
 */
static inline bool __smgr_is_sess_exist(u32 id)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return false;

	if (unlikely(!smgr_is_sess_id_valid(id)))
		return false;

	if (likely(test_bit(id, db->open_bmap)))
		return true;

	pr_err("session %u doesn't exist %pS %pS\n", id, __builtin_return_address(0), __builtin_return_address(1));
	return false;
}

/**
 * @brief Get session database entry reference
 * @param id session id
 * @return struct sess_db_entry* entry reference on success, NULL
 *         otherwise
 */
static inline struct sess_db_entry *__smgr_get_sess_db_ent(u32 id)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db) || !smgr_is_sess_id_valid(id))
		return NULL;

	return &db->sessions[id];
}

/**
 * @brief Dispatch session manager work, schedule work queue in async
 *        mode or call the work function directly in sync mode
 * @param req pp request
 * @param args work base args
 * @param delay delay in nanoseconds before queueing the work
 */
static inline void __smgr_dispatch_work(struct pp_request *req,
					struct base_args *args,
					ulong delay)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	if (PP_IS_ASYNC_REQUEST(req)) {
		/* convert delay to jiffies */
		delay = usecs_to_jiffies(delay);
		pr_debug("dispatching work in %lu jiffies\n", delay);
		/* async request */
		if (req->req_prio)
			queue_delayed_work(db->workq_hi, &args->dwork, delay);
		else
			queue_delayed_work(db->workq, &args->dwork, delay);
	} else {
		/* sync request */
		args->dwork.work.func(&args->dwork.work);
	}
}

/**
 * @brief Allocate session work arguments
 * @return struct sess_cr_args* valid session work args pointer if
 *         available, NULL otherwise
 */
static inline union work_args * __must_check
__smgr_work_args_prepare(struct pp_request *req, work_func_t func)
{
	union work_args *args = NULL;
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return NULL;

	args = (union work_args *)
		kmem_cache_alloc_node(db->work_args_cache,
				      GFP_ATOMIC, NUMA_NO_NODE);
	if (likely(args)) {
		memset(args, 0, sizeof(*args));
		if (req) {
			args->base.cb       = req->cb;
			args->base.req_id   = req->req_id;
			args->base.req_prio = req->req_prio;
		}
		/* always set func so we can use it later */
		if (PP_IS_ASYNC_REQUEST(req))
			INIT_DELAYED_WORK(&args->base.dwork, func);
		else
			args->base.dwork.work.func = func;

		return args;
	}

	atomic_inc(&db->stats.work_args_cache_err);
	pr_err("Failed to allocate session argument list\n");

	return args;
}

s32 smgr_get_queue_phy_id(u16 logical, u16 *physical)
{
	struct pp_qos_queue_info q_info;
	struct pp_qos_dev *qdev;
	s32 ret = 0;

	if (unlikely(ptr_is_null(physical)))
		return -EINVAL;

	/* get qos device */
	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(ptr_is_null(qdev)))
		return -EINVAL;

	ret = pp_qos_queue_info_get(qdev, logical, &q_info);
	if (unlikely(ret)) {
		pr_err("Failed to get queue %u info\n", logical);
		return ret;
	}

	*physical = (u16)q_info.physical_id;
	return ret;
}

/**
 * @brief Shortcut for testing whether session is a tunnel termination
 *        or not, USE only in the session create flow
 * @param s the session to test
 * @return bool true if the sess is tunnel termination, false
 *         otherwise
 */
static inline bool __smgr_is_tunn_term_sess(struct sess_info *s)
{
	/* In case of IPsec (UDP-encap) over GRE term. 
		both ing and eg packets are multi-level */
	if (SESS_IS_IP_GRE_DECP(s) || SESS_IS_L2_GRE_DECP(s) ||
		SESS_IS_IP_GRE_ENCP(s) || SESS_IS_L2_GRE_ENCP(s))
		return true;
	
	return (PKTPRS_IS_MULTI_LEVEL(SESS_RX_PKT(s)) !=
		PKTPRS_IS_MULTI_LEVEL(SESS_TX_PKT(s)));
}

/**
 * @brief Shortcut for testing whether session is a tunnel bypass
 *        or not, USE only in the session create flow
 * @param s the session to test
 * @return bool true if the sess is tunnel bypass, false
 *         otherwise
 */
static inline bool __smgr_is_tunn_bypass_sess(struct sess_info *s)
{
	/* In case of IPsec (UDP-encap) over GRE term. 
		both ing and eg packets are multi-level */
	if (SESS_IS_IP_GRE_DECP(s) || SESS_IS_L2_GRE_DECP(s) ||
		SESS_IS_IP_GRE_ENCP(s) || SESS_IS_L2_GRE_ENCP(s))
		return false;

	return (PKTPRS_IS_MULTI_LEVEL(SESS_RX_PKT(s)) &&
		PKTPRS_IS_MULTI_LEVEL(SESS_TX_PKT(s)));
}

/**
 * @brief Check whether a session includes inner IP layer at
 *        ingress and missing inner IP layer at egress
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_iptun_decp_sess(struct sess_info *s)
{
	if (SESS_IS_IP_GRE_DECP(s) || SESS_IS_L2_GRE_DECP(s))
		return true;

	return (PKTPRS_IS_MULTI_IP(SESS_RX_PKT(s)) &&
		!PKTPRS_IS_MULTI_IP(SESS_TX_PKT(s)));
}

/**
 * @brief Check whether a session missing inner IP layer at
 *        ingress and includes inner IP layer at egress
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_iptun_encp_sess(struct sess_info *s)
{
	if (SESS_IS_IP_GRE_ENCP(s) || SESS_IS_L2_GRE_ENCP(s))
		return true;

	return (!PKTPRS_IS_MULTI_IP(SESS_RX_PKT(s)) &&
		PKTPRS_IS_MULTI_IP(SESS_TX_PKT(s)) &&
		!SESS_TX_IS_OUTER_ESP(s));
}

/**
 * @brief Check whether a session is EoMPLS decapsulation
 *        where outer layer at ingress includes
 *        only MAC and MPLS headers
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_eompls_decp_sess(struct sess_info *s)
{
	if (__smgr_is_tunn_term_sess(s) &&
	    SESS_RX_IS_OUTER_MPLS(s) &&
	    !SESS_RX_IS_OUTER_IP(s))
		return true;

	return false;
}

/**
 * @brief Check whether a session is EoMPLS encapsulation
 *        where outer layer at egress includes
 *        only MAC and MPLS headers
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_eompls_encp_sess(struct sess_info *s)
{
	if (__smgr_is_tunn_term_sess(s) &&
	    SESS_TX_IS_OUTER_MPLS(s) &&
	    !SESS_TX_IS_OUTER_IP(s))
		return true;

	return false;
}

static inline bool __smgr_is_first_frag_bypass(struct sess_info *s, u8 lvl)
{
	if (pktprs_first_frag(SESS_RX_PKT(s), lvl) &&
	    pktprs_first_frag(SESS_TX_PKT(s), lvl))
		return true;

	return false;
}

/**
 * @brief Check whether a session is RX first frag
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_rx_first_frag(struct sess_info *s)
{
	if (SESS_RX_PKT(s) &&
	    (pktprs_first_frag(SESS_RX_PKT(s), HDR_OUTER) ||
	     pktprs_first_frag(SESS_RX_PKT(s), HDR_INNER)))
		return true;

	return false;
}

/**
 * @brief Check whether a session support fragments acceleration
 * @param s the session
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_frag_accl_sess(struct sess_info *s)
{
	if (__smgr_is_first_frag_bypass(s, HDR_OUTER))
		return true;

	if (PKTPRS_IS_MULTI_LEVEL(SESS_RX_PKT(s)) &&
	    __smgr_is_first_frag_bypass(s, HDR_INNER))
		return true;

	return false;
}

/**
 * @brief Test whether the inner ipv6 extension header has changed
 * @param s the session to test
 * @param lvl header level (inner/outer)
 * @return bool true if changed
 */
static inline bool
__smgr_is_ipv6_opt_changed(struct sess_info *s, u8 rx_lvl, u8 tx_lvl)
{
	if (!PKTPRS_IS_IPV6(SESS_RX_PKT(s), rx_lvl) ||
	    !PKTPRS_IS_IPV6(SESS_TX_PKT(s), tx_lvl))
		return false;

	if (PKTPRS_IS_HOP_OPT(SESS_RX_PKT(s), rx_lvl) !=
	    PKTPRS_IS_HOP_OPT(SESS_TX_PKT(s), tx_lvl))
		return true;
	if (PKTPRS_IS_DEST_OPT(SESS_RX_PKT(s), rx_lvl) !=
	    PKTPRS_IS_DEST_OPT(SESS_TX_PKT(s), tx_lvl))
		return true;
	if (PKTPRS_IS_ROUT_OPT(SESS_RX_PKT(s), rx_lvl) !=
	    PKTPRS_IS_ROUT_OPT(SESS_TX_PKT(s), tx_lvl))
		return true;

	return false;
}

/**
 * @brief Check whether a session is pppoe bypass or not
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_pppoe_bypass(struct sess_info *s)
{
	/* is pppoe header on both directions and session is bridged */
	if (SESS_RX_IS_OUTER_PPPOE(s) && SESS_TX_IS_OUTER_PPPOE(s) &&
	    __smgr_is_sess_bridged(s))
		return true;

	return false;
}

/**
 * @brief Check whether a session is pppoe inner encapsulation or not
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_pppoe_inner_encap(struct sess_info *s)
{
	/* is pppoe header was added on the inner packet */
	if (!SESS_RX_IS_INNER_PPPOE(s) && SESS_TX_IS_INNER_PPPOE(s))
		return true;

	return false;
}

/**
 * @brief Check whether a session is pppoe outer encapsulation or not
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_pppoe_outer_encap(struct sess_info *s)
{
	/* is pppoe header was added on the outer packet */
	if (!SESS_RX_IS_OUTER_PPPOE(s) && SESS_TX_IS_OUTER_PPPOE(s))
		return true;

	return false;
}

/**
 * @brief Check whether a session is pppoe encapsulation or not
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_pppoe_encap(struct sess_info *s)
{
	/* is pppoe header was added on the outer packet */
	if (__smgr_is_pppoe_outer_encap(s))
		return true;
	/* is pppoe header was added on the inner packet */
	if (__smgr_is_pppoe_inner_encap(s))
		return true;

	return false;
}

static bool __smgr_is_pppoe_supported(struct sess_info *s)
{
	bool rx_code_supported = true;
	bool tx_code_supported = true;

	/* check the TX pppoe code */
	if (SESS_TX_IS_OUTER_PPPOE(s)) {
		if (pktprs_pppoe_hdr(SESS_TX_PKT(s), HDR_OUTER)->code)
			tx_code_supported = false;
	} else if (SESS_TX_IS_INNER_PPPOE(s)) {
		if (pktprs_pppoe_hdr(SESS_TX_PKT(s), HDR_INNER)->code)
			tx_code_supported = false;
	}

	/* check the RX pppoe code */
	if (SESS_RX_IS_OUTER_PPPOE(s)) {
		if (pktprs_pppoe_hdr(SESS_RX_PKT(s), HDR_OUTER)->code)
			rx_code_supported = false;
	} else if (SESS_RX_IS_INNER_PPPOE(s)) {
		if (pktprs_pppoe_hdr(SESS_RX_PKT(s), HDR_INNER)->code)
			rx_code_supported = false;
	}

	/* RX & TX supported */
	if (rx_code_supported && tx_code_supported)
		return true;
	/* Bypass - supported */
	if (!rx_code_supported && !tx_code_supported)
		return true;

	return false;
}

/**
 * @brief Test whether a packet is reassembled
 * @param s
 * @param lvl
 * @return bool true if it is, false otherwise
 */
static inline bool is_reass_pkt(struct sess_info *s, enum pktprs_hdr_level lvl)
{
	return (pktprs_first_frag(SESS_RX_PKT(s), lvl) &&
		!pktprs_first_frag(SESS_TX_PKT(s), lvl));
}

/**
 * @brief Test whether a packet is fragmented
 * @param s
 * @param lvl
 * @return bool true if it is, false otherwise
 */
static inline bool is_frag_pkt(struct sess_info *s, enum pktprs_hdr_level lvl)
{
	return (!pktprs_first_frag(SESS_RX_PKT(s), lvl) &&
		pktprs_first_frag(SESS_TX_PKT(s), lvl));
}

/**
 * @brief Test whether fragments can be supported for the session
 * @param s the session
 * @return bool true if it is supported, false otherwise
 */
static inline bool __smgr_is_frag_supported(struct sess_info *s)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return false;

	/* no frags, supported */
	if (!pktprs_is_ip_frag(SESS_RX_PKT(s), HDR_OUTER) &&
	    !pktprs_is_ip_frag(SESS_RX_PKT(s), HDR_INNER) &&
	    !pktprs_is_ip_frag(SESS_TX_PKT(s), HDR_OUTER) &&
	    !pktprs_is_ip_frag(SESS_TX_PKT(s), HDR_INNER))
		return true;

	/* outer or inner reassembly */
	if (is_reass_pkt(s, HDR_OUTER) || is_reass_pkt(s, HDR_INNER)) {
		/* supported only if reassembly NF is enabled */
		return pp_misc_is_nf_en(PP_NF_REASSEMBLY);
	}

	/* frag acceleration */
	if (__smgr_is_frag_accl_sess(s)) {
		/* supported only if reassembly NF is enabled */
		return pp_misc_is_nf_en(PP_NF_REASSEMBLY);
	}

	/* outer or inner fragmentation with tunnel */
	if (is_frag_pkt(s, HDR_OUTER) || is_frag_pkt(s, HDR_INNER)) {
		/* Tunnel encapsulation */
		if (__smgr_is_iptun_encp_sess(s) ||
		    __smgr_is_pppoe_encap(s) ||
			db->open_frag_sess)
			/* supported only if frag NF is enabled */
			return pp_misc_is_nf_en(PP_NF_FRAGMENTER);

		/* ESP Tunnel */
		if (is_frag_pkt(s, HDR_OUTER) &&
		    (SESS_TX_OUTER_IP_NEXT(s) == PKTPRS_PROTO_ESP))
			/* supported only if frag NF is enabled */
			return pp_misc_is_nf_en(PP_NF_FRAGMENTER);
	}

	return false;
}

/**
 * for ESP sessions the support is for two cases:
 * 1. esp encapsulation:
 * rx - mac-vlan(opt)-ipv4-udp/tcp
 * tx - mac-vlan(opt)-ipv4-esp-IV(opt)-ipv4-udp/tcp (clear text)
 * @note NAT is not supported
 * 2. esp bypass:
 * rx - mac-vlan(opt)-pppoe(opt)-ipv4-esp
 * tx - mac-vlan(opt)-pppoe(opt)-ipv4-esp
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_esp_supported(struct sess_info *s)
{
	struct iphdr *txip, *rxip;

	/* UDP NAT bypass, tunnel or transport mode, we don't really know
	 * cause we don't know what comes after the esp header
	 */
	if (SESS_RX_IS_INNER_ESP(s) && SESS_TX_IS_INNER_ESP(s) &&
	    SESS_TX_OUTER_IP_NEXT(s) == PKTPRS_PROTO_UDP) {
		pr_debug("esp udp nat bypass\n");
		/* this session needs to use regular UDP napt recipe
		 * so we don't set MOD_IPV4_BIT or MOD_IPV6_BIT as in
		 * other ESP cases
		 */
		return true;
	}

	/* for all cases tx must include esp */
	if (!SESS_TX_IS_OUTER_ESP(s))
		return false;

	/* tunnel mode decapsulation 1st session where outer ip doesn't exist */
	if (SESS_RX_IS_OUTER_ESP(s) && SESS_RX_IS_OUTER_IP(s) &&
	    !SESS_TX_IS_OUTER_IP(s)) {
		SESS_MOD_FLAG_SET(s, MOD_HDR_L3_RPLC_BIT);
		return true;
	}

	/* for all the following cases tx must include outer ip */
	if (!SESS_TX_IS_OUTER_IP(s))
		return false;

	/* for all the following cases esp must be after the outer ip */
	if (SESS_TX_OUTER_IP_NEXT(s) != PKTPRS_PROTO_ESP)
		return false;

	if (SESS_RX_IS_OUTER_V4(s))
		SESS_MOD_FLAG_SET(s, MOD_IPV4_BIT);
	else if (SESS_RX_IS_OUTER_V6(s))
		SESS_MOD_FLAG_SET(s, MOD_IPV6_BIT);

	/* encapsulation - the only session we have clear text is outbound
	 * 1st session tx packet
	 */
	if (!SESS_RX_IS_OUTER_ESP(s)) {
		/* don't allow mix IP layers */
		if ((SESS_TX_IS_OUTER_V4(s) && SESS_TX_IS_INNER_V6(s)) ||
		    (SESS_TX_IS_OUTER_V6(s) && SESS_TX_IS_INNER_V4(s))) {
			pr_debug("esp unsupported ip mix\n");
			return false;
		}

		switch (SESS_TX_ESP_NEXT(s)) {
		case PKTPRS_PROTO_IPV4:
		case PKTPRS_PROTO_IPV6:
			SESS_MOD_FLAG_SET(s, MOD_ESP_TUNNEL_ENCP_BIT);
			pr_debug("esp tunnel mode\n");
			break;
		/* transport cases, in case we have IP tunnel encapsulated
		 * by the ESP we must keep the ip from the ingress packet,
		 * otherwise, we just replace it with egress ip using the new
		 * header
		 */
		case PKTPRS_PROTO_UDP:
		case PKTPRS_PROTO_TCP:
			if (SESS_IS_L2TP_OUDP_ENCP(s)) {
				if (!smgr_is_l2tpudp_sess_supported(s)) {
					pr_debug("unsupported l2tp over esp\n");
					return false;
				}
				pr_debug("l2tp over esp session\n");
			} else {
				SESS_MOD_FLAG_SET(s, MOD_HDR_L3_RPLC_BIT);
				pr_debug("esp transport mode hdr l3 replace\n");
			}
			/* fallthrough */
		case PKTPRS_PROTO_GRE:
		case PKTPRS_PROTO_L2TP_OIP:
			SESS_MOD_FLAG_SET(s, MOD_ESP_TRANSPORT_ENCP_BIT);
			pr_debug("esp transport mode\n");
			break;
		default:
			pr_debug("unsupported esp next proto %u\n",
				 SESS_TX_ESP_NEXT(s));
			return false;
		}

		if (SESS_RX_IS_OUTER_V6(s)) {
			pr_debug("esp v6 supported\n");
			return true;
		}
		if (!SESS_TX_IS_INNER_IP(s)) {
			pr_debug("esp no-ip tunnel supported\n");
			return true;
		}
		if (SESS_IS_L2TP_OUDP_ENCP(s)) {
			pr_debug("esp l2tp encap tunnel supported\n");
			return true;
		}

		/* do not support nat on inner packet */
		rxip = pktprs_ipv4_hdr(SESS_RX_PKT(s), HDR_OUTER);
		txip = pktprs_ipv4_hdr(SESS_TX_PKT(s), HDR_INNER);
		if (txip->saddr == rxip->saddr) {
			pr_debug("esp v4 supported\n");
			return true;
		}

		pr_debug("esp encapsulation unsupported\n");
		return false;
	}

	/**************************************************************
	 * Bypass cases:
	 * -----------------------------------
	 * 1. outbound encapsulation 2nd session, tunnel/transport mode.
	 * 2. inbound decapsulation 1st session, transport mode.
	 * 2. bypass with VLAN/PPPoE added/removed/unchanged.
	 *************************************************************/

	/* don't allow UDP before ESP for now and don't allow
	 * inner packets changes in these cases
	 */
	if (SESS_RX_OUTER_IP_NEXT(s) == PKTPRS_PROTO_ESP &&
	    SESS_RX_INNER_BMAP(s) == SESS_TX_INNER_BMAP(s)) {
		if (SESS_ARGS_IS_FLAG_ON(s, PP_SESS_FLAG_ESP_TRANSPORT_BIT)) {
			SESS_MOD_FLAG_SET(s, MOD_ESP_TRANSPORT_BPAS_BIT);
			pr_debug("esp transport bypass\n");
		} else {
			SESS_MOD_FLAG_SET(s, MOD_ESP_TUNNEL_BPAS_BIT);
			pr_debug("esp tunnel bypass\n");
		}
		return true;
	}

	return false;
}

/**
 * @brief Test whether a changes in the ipv6 extension headers are supported
 * @param s the session to test
 * @return bool true if supported
 */
static inline bool __smgr_is_ipv6_ext_supported(struct sess_info *s)
{
	if (__smgr_is_iptun_decp_sess(s)) {
		/* for ip tunnel decapsulation, the interesting case
		 * is only where ipv6 in the ingress inner and egress outer
		 */
		return !__smgr_is_ipv6_opt_changed(s, HDR_INNER, HDR_OUTER);
	} else if (__smgr_is_iptun_encp_sess(s)) {
		/* for ip tunnel encapsulation, the interesting case
		 * is only where ipv6 in the ingress outer and egress inner
		 */
		return !__smgr_is_ipv6_opt_changed(s, HDR_OUTER, HDR_INNER);
	}

	if (!__smgr_is_ipv6_opt_changed(s, HDR_OUTER, HDR_OUTER) &&
	    !__smgr_is_ipv6_opt_changed(s, HDR_INNER, HDR_INNER))
		return true;

	return false;
}

/**
 * @brief Check whether a session is EoGRE termination session
 *        ONLY w/o any other tunnels
 * @note this API also set the mod_flags to avoid double
 *       checking later
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_eogre_term_sess(struct sess_info *s)
{
	/* decapsulation */
	if (SESS_RX_IS_L2_GRE(s)   &&
	    !SESS_TX_IS_L2_GRE(s)  &&
		 !pktprs_gre_hdr(SESS_RX_PKT(s), HDR_OUTER)->flags &&
	    SESS_RX_IS_INNER_IP(s) &&
	    (SESS_RX_IS_INNER_L4(s) || SESS_RX_IS_INNER_ESP(s))) {
		SESS_MOD_FLAG_SET(s, MOD_EOGRE_DECP_BIT);
		return true;
	}
	/* encapsulation */
	if (SESS_TX_IS_L2_GRE(s)   &&
	    !SESS_RX_IS_L2_GRE(s)  &&
		 !pktprs_gre_hdr(SESS_TX_PKT(s), HDR_OUTER)->flags &&
	    SESS_TX_IS_INNER_IP(s) &&
	    (SESS_TX_IS_INNER_L4(s) || SESS_TX_IS_INNER_ESP(s))) {
		if (SESS_TX_IS_OUTER_V4(s))
			SESS_MOD_FLAG_SET(s, MOD_EOGRE_V4_ENCP_BIT);
		else if (SESS_TX_IS_OUTER_V6(s))
			SESS_MOD_FLAG_SET(s, MOD_EOGRE_V6_ENCP_BIT);
		return true;
	}

	return false;
}

/**
 * @brief Check whether a session is IPoGRE termination session
 *        ONLY w/o any other tunnels
 * @note this API also set the mod_flags to avoid double
 *       checking later
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_ipogre_term_sess(struct sess_info *s)
{
	/* decapsulation */
	if (SESS_RX_IS_IP_GRE(s)  &&
	    !SESS_TX_IS_IP_GRE(s) &&
		 !pktprs_gre_hdr(SESS_RX_PKT(s), HDR_OUTER)->flags &&
	    (SESS_RX_IS_INNER_L4(s) || SESS_RX_IS_INNER_ESP(s))) {
		SESS_MOD_FLAG_SET(s, MOD_IPOGRE_DECP_BIT);
		return true;
	}
	/* encapsulation */
	if (SESS_TX_IS_IP_GRE(s)  &&
	    !SESS_RX_IS_IP_GRE(s) &&
		 !pktprs_gre_hdr(SESS_TX_PKT(s), HDR_OUTER)->flags &&
	    (SESS_TX_IS_INNER_L4(s) || SESS_TX_IS_INNER_ESP(s))) {
		if (SESS_TX_IS_OUTER_V4(s))
			SESS_MOD_FLAG_SET(s, MOD_IPOGRE_V4_ENCP_BIT);
		else if (SESS_TX_IS_OUTER_V6(s))
			SESS_MOD_FLAG_SET(s, MOD_IPOGRE_V6_ENCP_BIT);
		return true;
	}

	return false;
}

/**
 * @brief Check whether a session is DSLITE termination session
 *        ONLY w/o any other tunnels
 * @note this API also set the mod_flags to avoid double
 *       checking later
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_dslite_term_sess(struct sess_info *s)
{
	/* decapsulation */
	if (SESS_RX_IS_DSLITE(s)  &&
	    !SESS_TX_IS_DSLITE(s) &&
	    SESS_RX_IS_INNER_L4(s)) {
		SESS_MOD_FLAG_SET(s, MOD_DSLITE_DECP_BIT);
		return true;
	}
	/* encapsulation */
	if (SESS_TX_IS_DSLITE(s)  &&
	    !SESS_RX_IS_DSLITE(s) &&
	    SESS_TX_IS_INNER_L4(s)) {
		SESS_MOD_FLAG_SET(s, MOD_DSLITE_ENCP_BIT);
		return true;
	}

	return false;
}

/**
 * @brief Check whether a session is vxlan termination session
 *        ONLY w/o any other tunnels
 * @note this API also set the mod_flags to avoid double
 *       checking later
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_vxlan_term_sess(struct sess_info *s)
{
	struct udphdr *udphdr;
	/* decapsulation */
	if (SESS_RX_IS_VXLAN(s)  &&
		!SESS_TX_IS_VXLAN(s) &&
		SESS_RX_IS_INNER_L4(s)) {
		SESS_MOD_FLAG_SET(s, MOD_UDP_DECP_BIT);
		return true;
	}
	/* encapsulation */
	if (SESS_TX_IS_VXLAN(s)  &&
		!SESS_RX_IS_VXLAN(s) &&
		SESS_TX_IS_INNER_L4(s)) {
		if (SESS_TX_IS_INNER_UDP(s)) {
			udphdr = pktprs_udp_hdr(SESS_TX_PKT(s),
					HDR_INNER);
			if (udphdr->check == 0) {
				/* csum zero is not supported unless
				also the outer udp csum is zero */
				if (SESS_TX_IS_OUTER_V4(s)) {
					udphdr = pktprs_udp_hdr(SESS_TX_PKT(s),
							HDR_OUTER);
					if (udphdr->check != 0) {
						/*
						special case when outer udp
						support csum and inner
						udp checksum disbaled
						*/
						return false;
					}
				} else {
					/* IPV6 not support csum zero */
					return false;
				}
			}
		}
		if (SESS_TX_IS_OUTER_V4(s)) {
			udphdr = pktprs_udp_hdr(SESS_TX_PKT(s), HDR_OUTER);
			if (udphdr->check == 0)
				SESS_MOD_FLAG_SET(s, MOD_UDP_CSUM_DISABLE_BIT);
			SESS_MOD_FLAG_SET(s, MOD_NHDR_IPV4_BIT);
		} else if (SESS_TX_IS_OUTER_V6(s)) {
			SESS_MOD_FLAG_SET(s, MOD_NHDR_IPV6_BIT);
		}
		SESS_MOD_FLAG_SET(s, MOD_UDP_ENCP_BIT);
		return true;
	}

	return false;
}

/**
 * @brief Check whether a session is 6RD termination session
 *        ONLY w/o any other tunnels
 * @note this API also set the mod_flags to avoid double
 *       checking later
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_6rd_term_sess(struct sess_info *s)
{
	/* decapsulation */
	if (SESS_RX_IS_SIXRD(s)  &&
	    !SESS_TX_IS_SIXRD(s) &&
	    SESS_RX_IS_INNER_L4(s)) {
		SESS_MOD_FLAG_SET(s, MOD_6RD_DECP_BIT);
		return true;
	}
	/* encapsulation */
	if (SESS_TX_IS_SIXRD(s)  &&
	    !SESS_RX_IS_SIXRD(s) &&
	    SESS_TX_IS_INNER_L4(s)) {
		SESS_MOD_FLAG_SET(s, MOD_6RD_ENCP_BIT);
		return true;
	}

	return false;
}

/**
 * @brief Check whether a session is l2tp over udp
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool smgr_is_l2tpudp_sess(struct sess_info *s)
{
	return SESS_RX_IS_L2TP_OUDP(s) || SESS_TX_IS_L2TP_OUDP(s);
}

/**
 * @brief Check whether a session is a supported L2TP UDP session
 *        Currently, we support L2TP over UDP with PPP right after and
 *        w/o any other tunnels
 * @note this function checks only l2tp over udp termination sessions
 *       bypass sessions are not supported
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool smgr_is_l2tpudp_sess_supported(struct sess_info *s)
{
	union l2tphdr *l2tp;
	struct udphdr *udp;
	struct pktprs_hdr *hdr = NULL;
	u16 flags, ver, hdr_sz, allowed;

	if (SESS_RX_IS_L2TP_OUDP(s) && SESS_TX_IS_L2TP_OUDP(s)) {
		pr_debug("unsupported l2tp over udp bypass\n");
		return false;
	}

	if (SESS_RX_IS_L2TP_OUDP(s))
		hdr = SESS_RX_PKT(s);
	else if (SESS_TX_IS_L2TP_OUDP(s))
		hdr = SESS_TX_PKT(s);

	if (!hdr) {
		pr_debug("Failed to parse l2tp header\n");
		return false;
	}

	l2tp = pktprs_l2tp_oudp_hdr(hdr, HDR_OUTER);
	if (!l2tp) {
		/* really fatal if this occurs */
		pr_debug("l2tp header is null with l2tp flag on!!!\n");
		return false;
	}

	/* get the version */
	ver = ntohs(l2tp->v2.flags) & L2TP_HDR_VER_MASK;
	/* get the flags */
	flags = ntohs(l2tp->v2.flags) & L2TP_HDR_FLAGS_MASK;

	/* set allowed flags and max supported header size based
	 * on the version
	 */
	switch (ver) {
	case L2TP_HDR_VER_2:
		/* only allowed flag is length */
		allowed = L2TP_HDRFLAG_L;
		/* */
		break;
	case L2TP_HDR_VER_3:
		/* no flags are allowed in v3 */
		allowed = 0;
		/* l2tpv3 doesn't have flags which changes the size of the
		 * header so we need to calculate the header size and make
		 * sure we can support it.
		 */
		hdr_sz = pktprs_l2tp_oudp_hdr_sz(hdr, HDR_OUTER) -
			 pktprs_l2tp_oudp_hdr_off(hdr, HDR_OUTER);
		/* cookie isn't supported as it has a variable length, but,
		 * l2-specific does exist by default when configuring l2tpv3
		 * so its better to support only the case it exists.
		 */
		if (hdr_sz != L2TPV3_OUDP_HDR_SIZE) {
			pr_debug("Unsupported l2tpv3 over udp header size %u\n",
			       hdr_sz);
			return false;
		}
		if (!l2tp->v3_oudp.sess_id) {
			pr_debug("cannot accelerate l2tpv3 over udp control message\n");
			return false;
		}
		break;
	default:
		pr_debug("Un-Unsupported l2tp over udp version, %u\n", ver);
		return false;
	}

	/* verify unsupported flags aren't enabled
	 * we only support length bit
	 */
	if (flags & ~allowed) {
		pr_debug("unsupported l2tpv%u over udp flags, flags %#x\n", ver,
			 flags);
		return false;
	}

	/* decapsulation v2/v3 */
	if (SESS_RX_IS_L2TP_OUDP(s)                &&
	    SESS_RX_IS_PPP(s)                      &&
	    !PKTPRS_IS_MULTI_LEVEL(SESS_TX_PKT(s)) &&
	    SESS_RX_IS_INNER_IP(s)                 &&
	    SESS_RX_IS_INNER_L4(s)) {
		SESS_MOD_FLAG_SET(s, MOD_L2TPUDP_DECP_BIT);
		pr_debug("l2tpv%u over udp decapsulation\n", ver);
		return true;
	}

	/* encapsulation v2/v3 */
	if (SESS_TX_IS_L2TP_OUDP(s)) {
		/* we cannot support inner L4 which isn't UDP or TCP
		 * when we need to support checksum in the outer UDP
		 */
		udp = pktprs_udp_hdr(SESS_TX_PKT(s), HDR_OUTER);
		if (udp->check && !SESS_TX_IS_INNER_UDP(s) &&
		    !SESS_TX_IS_INNER_TCP(s)) {
			pr_debug("unsupported l2tp over udp checksum\n");
			return false;
		}

		if (SESS_TX_IS_PPP(s) &&
		    !PKTPRS_IS_MULTI_LEVEL(SESS_RX_PKT(s)) &&
		    SESS_TX_IS_INNER_IP(s)) {
			if (SESS_TX_IS_OUTER_V4(s)) {
				SESS_MOD_FLAG_SET(s, MOD_NHDR_IPV4_BIT);
				pr_debug("l2tpv%u over ipv4 udp encapsulation\n",
					 ver);
			} else if (SESS_TX_IS_OUTER_V6(s)) {
				SESS_MOD_FLAG_SET(s, MOD_NHDR_IPV6_BIT);
				pr_debug("l2tpv%u over ipv udp encapsulation\n",
					 ver);
			}

			SESS_MOD_FLAG_SET(s, MOD_NHDR_UDP_BIT);
			if (flags & L2TP_HDRFLAG_L)
				SESS_MOD_FLAG_SET(s, MOD_NHDR_L2TP_BIT);
			return true;
		}
	}

	return false;
}

/**
 * @brief Check whether a session is l2tp over ip
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_l2tpip_sess(struct sess_info *s)
{
	return PKTPRS_IS_L2TP_OIP(SESS_RX_PKT(s), HDR_OUTER) ||
	       PKTPRS_IS_L2TP_OIP(SESS_TX_PKT(s), HDR_OUTER);
}

/**
 * @brief Check whether an L2TPIP session is supported
 *        In general, we only support termination sessions, w/o cookie
 *        in the header.
 * @note this API also sets some modification flags
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_l2tpip_sess_supported(struct sess_info *s)
{
	union l2tphdr *l2tp;
	struct pktprs_hdr *hdr = NULL;
	u16 hdr_sz;

	/* decapsulation */
	if (PKTPRS_IS_L2TP_OIP(SESS_RX_PKT(s), HDR_OUTER) &&
	    PKTPRS_IS_L2TP_OIP(SESS_TX_PKT(s), HDR_OUTER)) {
		pr_debug("unsupported l2tpv3 over ip bypass session\n");
		return false;
	}

	/* decapsulation */
	if (PKTPRS_IS_L2TP_OIP(SESS_RX_PKT(s), HDR_OUTER))
		hdr = SESS_RX_PKT(s);
	/* encapsulation */
	else if (PKTPRS_IS_L2TP_OIP(SESS_TX_PKT(s), HDR_OUTER))
		hdr = SESS_TX_PKT(s);

	if (!hdr) {
		pr_debug("Failed to parse l2tp header\n");
		return false;
	}

	l2tp = pktprs_l2tp_oip_hdr(hdr, HDR_OUTER);
	if (!l2tp) {
		/* really fatal if this occurs */
		pr_debug("l2tp header is null with l2tp flag on!!!\n");
		return false;
	}

	hdr_sz = pktprs_l2tp_oip_hdr_sz(hdr, HDR_OUTER) -
		 pktprs_l2tp_oip_hdr_off(hdr, HDR_OUTER);

	/* we don't support cookie cause its size varies
	 * we cannot accelerate control messages (0 session id)
	 * but we do support l2-specific as its the default configuration
	 * for l2tpv3 tunnels, hence, header size must be equal to 8 bytes
	 */
	if (hdr_sz != L2TPV3_OIP_HDR_LEN) {
		pr_debug("Unsupported l2tpv3 over ip header size %u\n", hdr_sz);
		return false;
	}
	if (!l2tp->v3_oip.sess_id) {
		pr_debug("cannot acceleratable l2tpv3 over ip control message\n");
		return false;
	}

	/* currently, we only support l2tpv3 over IP which encapsulate
	 * mac packets.
	 * to support other protocols, a different design should be applied
	 * as the only place we can tell what is the protocol after the l2tp
	 * is in the l2tp database inside the l2tp kernel module
	 */

	/* decapsulation */
	if (PKTPRS_IS_L2TP_OIP(SESS_RX_PKT(s), HDR_OUTER) &&
	    PKTPRS_IS_MAC(SESS_RX_PKT(s), HDR_INNER) &&
	    SESS_RX_IS_INNER_IP(s) &&
	    SESS_RX_IS_INNER_L4(s)) {
		SESS_MOD_FLAG_SET(s, MOD_L2TPIP_DECP_BIT);
		pr_debug("l2tpv3 over ip decapsulation\n");
		return true;
	}

	/* encapsulation */
	if (PKTPRS_IS_L2TP_OIP(SESS_TX_PKT(s), HDR_OUTER) &&
	    PKTPRS_IS_MAC(SESS_TX_PKT(s), HDR_INNER) &&
	    SESS_TX_IS_INNER_IP(s) &&
	    SESS_TX_IS_INNER_L4(s)) {
		SESS_MOD_FLAG_SET(s, MOD_IPV4_NAT_BIT);
		if (SESS_TX_IS_OUTER_V4(s)) {
			SESS_MOD_FLAG_SET(s, MOD_NHDR_IPV4_BIT);
			pr_debug("l2tpv3 over ipv4 encapsulation\n");
		} else if (SESS_TX_IS_OUTER_V6(s)) {
			SESS_MOD_FLAG_SET(s, MOD_NHDR_IPV6_BIT);
			pr_debug("l2tpv3 over ipv6 encapsulation\n");
		}
		return true;
	}

	return false;
}

/**
 * @brief Check whether an EoMPLS session is supported
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_eompls_sess_supported(struct sess_info *s)
{
	/* decapsulation */
	if (!SESS_RX_IS_OUTER_IP(s) &&
	    SESS_RX_IS_OUTER_MPLS(s) &&
	    SESS_RX_IS_INNER_MAC(s) &&
	    SESS_RX_IS_INNER_IP(s) &&
	    SESS_RX_IS_INNER_L4(s) &&
	    !SESS_TX_IS_OUTER_MPLS(s)) {
		if (SESS_RX_IS_INNER_V4(s)) {
			pr_debug("Eth over MPLS ipv4 decapsulation\n");
			SESS_MOD_FLAG_SET(s, MOD_MPLS_V4_DECP_BIT);
		} else {
			pr_debug("Eth over MPLS ipv6 decapsulation\n");
			SESS_MOD_FLAG_SET(s, MOD_MPLS_V6_DECP_BIT);
		}
		return true;
	}

	/* encapsulation */
	if (!SESS_TX_IS_OUTER_IP(s) &&
	    SESS_TX_IS_OUTER_MPLS(s) &&
	    SESS_TX_IS_INNER_MAC(s) &&
	    SESS_TX_IS_INNER_IP(s) &&
	    SESS_TX_IS_INNER_L4(s) &&
	    !SESS_RX_IS_OUTER_MPLS(s)) {
		pr_debug("Eth over MPLS encapsulation\n");
		return true;
	}

	return false;
}

#define MOCA_CTP_PROTO 0xFFFF

/**
 * @brief Check whether a session is Moca CTP
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_moca_ctp_sess(struct sess_info *s)
{
	u16 last_type_rx;
	u16 last_type_tx;
	/*
	 * Moca CTP packets includes:
	 *	MAC + (optional vlan header/s), no IP
	 *	and next proto is 0xFFFF
	 */
	last_type_rx = pktprs_last_ethtype_get(SESS_RX_PKT(s), HDR_OUTER);
	last_type_tx = pktprs_last_ethtype_get(SESS_TX_PKT(s), HDR_OUTER);
	if (last_type_rx == MOCA_CTP_PROTO && last_type_tx == MOCA_CTP_PROTO) {
		pr_debug("Session is Moca CTP\n");
		return true;
	}

	return false;
}

/**
 * @brief Check whether a session is ESP bypass
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_esp_bypass_sess(struct sess_info *s)
{
	return (SESS_RX_IS_OUTER_ESP(s) && SESS_TX_IS_OUTER_ESP(s)) ||
	       (SESS_RX_IS_INNER_ESP(s) && SESS_TX_IS_INNER_ESP(s));
}

/**
 * @brief Check whether a session is ESP
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_esp_sess(struct sess_info *s)
{
	if (__smgr_is_esp_bypass_sess(s))
		return true;

	if (SESS_ARGS_IS_FLAG_OFF(s, PP_SESS_FLAG_VPN_BIT))
		return false;

	return SESS_RX_IS_OUTER_ESP(s) || SESS_TX_IS_OUTER_ESP(s) ||
	       SESS_RX_IS_INNER_ESP(s) || SESS_TX_IS_INNER_ESP(s);
}

/**
 * @brief Check whether a session is ESP encapsulation
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_esp_encp_sess(struct sess_info *s)
{
	return (!SESS_RX_IS_OUTER_ESP(s) &&
			!SESS_RX_IS_INNER_ESP(s) &&
			SESS_TX_IS_OUTER_ESP(s));
}

/**
 * @brief Check whether esp session needs nat modification
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_esp_nat_sess(struct sess_info *s)
{
	/* non-tunnel or l2tp over udp traffic encapsulated
	 * with ESP transport mode
	 */
	if (__smgr_is_esp_encp_sess(s) &&
	       SESS_ARGS_IS_FLAG_ON(s, PP_SESS_FLAG_ESP_TRANSPORT_BIT) &&
	       ((!PKTPRS_IS_MULTI_IP(SESS_TX_PKT(s)) &&
	       SESS_RX_IS_OUTER_V4(s) &&
	       SESS_TX_IS_OUTER_V4(s)) ||
	       SESS_IS_L2TP_OUDP_ENCP(s)))
		return true;

	/* bypass with UDP NAT */
	if (SESS_RX_IS_INNER_ESP(s) && SESS_TX_IS_INNER_ESP(s) &&
	    SESS_RX_OUTER_IP_NEXT(s) == PKTPRS_PROTO_UDP)
		return true;

	/* bypass without NAT-T */
	if (SESS_RX_IS_OUTER_ESP(s) && SESS_TX_IS_OUTER_ESP(s) &&
	    __smgr_is_ip_nat_addr_changed(s))
		return true;

	/* IP over GRE termination */
	if (SESS_RX_IS_INNER_ESP(s) && SESS_TX_IS_OUTER_ESP(s) &&
		(SESS_IS_IP_GRE_ENCP(s) || SESS_IS_IP_GRE_DECP(s)))
			return true;

	/* Eth over GRE termination */
	if (SESS_RX_IS_INNER_ESP(s) && SESS_TX_IS_OUTER_ESP(s) &&
		(SESS_IS_L2_GRE_ENCP(s) || SESS_IS_L2_GRE_DECP(s)))
			return true;

	return false;
}

/**
 * @brief Check whether a session mac addresses has changed
 * @param s the session to test
 * @param lvl header level (inner/outer)
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_mac_changed(struct sess_info *s, u8 lvl)
{
	/* dst mac changed? */
	if (PKTPRS_IS_MAC(SESS_RX_PKT(s), lvl) &&
	    PKTPRS_IS_MAC(SESS_TX_PKT(s), lvl)) {
		return !!memcmp(&pktprs_eth_hdr(SESS_RX_PKT(s), lvl)->h_dest,
				&pktprs_eth_hdr(SESS_TX_PKT(s), lvl)->h_dest,
				ETH_ALEN);
	}

	/* mac added or removed? */
	if (PKTPRS_IS_MAC(SESS_RX_PKT(s), lvl) !=
	    PKTPRS_IS_MAC(SESS_TX_PKT(s), lvl))
		return true;

	/* no changes */
	return false;
}

static inline bool __smgr_is_vlan_changed(struct sess_info *s, u8 lvl)
{
	u8 i;

	if (!PKTPRS_VLAN_EXIST(SESS_RX_PKT(s), lvl) &&
	    !PKTPRS_VLAN_EXIST(SESS_TX_PKT(s), lvl))
		return false;

	for (i = 0; i < MAX_VLAN_HDRS_SUPPORTED; i++) {
		if (!PKTPRS_IS_VLAN(SESS_RX_PKT(s), lvl, i) &&
		    !PKTPRS_IS_VLAN(SESS_TX_PKT(s), lvl, i))
			return false;
		/* vlan was added or removed? */
		if (PKTPRS_IS_VLAN(SESS_RX_PKT(s), lvl, i) !=
		    PKTPRS_IS_VLAN(SESS_TX_PKT(s), lvl, i))
			return true;
		/* vlan value changed? */
		if (pktprs_vlan_hdr(SESS_RX_PKT(s), lvl, i)->h_vlan_TCI !=
		    pktprs_vlan_hdr(SESS_TX_PKT(s), lvl, i)->h_vlan_TCI)
			return true;
	}

	return false;
}

/**
 * @brief Check whether a session outer pppoe has been
 *        added/removed/changed
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_pppoe_changed(struct sess_info *s, u8 lvl)
{
	/* no pppoe? */
	if (!PKTPRS_IS_PPPOE(SESS_RX_PKT(s), lvl) &&
	    !PKTPRS_IS_PPPOE(SESS_TX_PKT(s), lvl))
		return false;

	/* pppoe session id changed? */
	if (PKTPRS_IS_PPPOE(SESS_RX_PKT(s), lvl) &&
	    PKTPRS_IS_PPPOE(SESS_TX_PKT(s), lvl)) {
		return pktprs_pppoe_hdr(SESS_RX_PKT(s), lvl)->sid !=
		       pktprs_pppoe_hdr(SESS_TX_PKT(s), lvl)->sid;
	}

	/* pppoe header was added or removed? */
	return true;
}

static inline bool __smgr_is_mpls_changed(struct sess_info *s, u8 lvl)
{
	u8 i;

	if (!PKTPRS_MPLS_EXIST(SESS_RX_PKT(s), lvl) &&
	    !PKTPRS_MPLS_EXIST(SESS_TX_PKT(s), lvl))
		return false;

	for (i = 0; i < MAX_MPLS_HDRS_SUPPORTED; i++) {
		if (!PKTPRS_IS_MPLS(SESS_RX_PKT(s), lvl, i) &&
		    !PKTPRS_IS_MPLS(SESS_TX_PKT(s), lvl, i))
			return false;
		/* mpls was added or removed? */
		if (PKTPRS_IS_MPLS(SESS_RX_PKT(s), lvl, i) !=
		    PKTPRS_IS_MPLS(SESS_TX_PKT(s), lvl, i))
			return true;
	}

	return false;
}

static inline bool __smgr_is_outer_l2_changed(struct sess_info *s)
{
	return __smgr_is_mac_changed(s, HDR_OUTER)   ||
	       __smgr_is_vlan_changed(s, HDR_OUTER)  ||
	       __smgr_is_pppoe_changed(s, HDR_OUTER) ||
	       __smgr_is_mpls_changed(s, HDR_OUTER);
}

/**
 * @brief Check whether a session nat packets are ipv4, the nat
 *        packets are the packets that needs to compare for doing the
 *        nat
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_ipv4_nat(struct sess_info *s)
{
	return PKTPRS_IS_IPV4(SESS_RX_PKT(s), SESS_RX_NAT_LVL(s)) &&
	       PKTPRS_IS_IPV4(SESS_TX_PKT(s), SESS_TX_NAT_LVL(s));
}

/**
 * @brief Check whether the session's nat packets are ipv6, the nat
 *        packets are the packets that needs to compare for doing the
 *        nat
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_ipv6_nat(struct sess_info *s)
{
	return PKTPRS_IS_IPV6(SESS_RX_PKT(s), SESS_RX_NAT_LVL(s)) &&
	       PKTPRS_IS_IPV6(SESS_TX_PKT(s), SESS_TX_NAT_LVL(s));
}

/**
 * @brief Check whether the session's is map-t where ipv4 was translated
 *        to ipv6 or vice versa without any other IP tunnels
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool smgr_is_mapt_nat_sess(struct sess_info *s)
{
	return ((SESS_RX_IS_OUTER_V4(s) && SESS_TX_IS_OUTER_V6(s)) ||
		(SESS_RX_IS_OUTER_V6(s) && SESS_TX_IS_OUTER_V4(s))) &&
	       !PKTPRS_IS_MULTI_IP(SESS_RX_PKT(s)) &&
	       !PKTPRS_IS_MULTI_IP(SESS_TX_PKT(s));
}

/**
 * @brief Check whether the map-t session is supported
 * @note Assumes the session is map-t verified by smgr_is_mapt_nat_sess
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool smgr_is_mapt_sess_supported(struct sess_info *s)
{
	/* support only map-t alone w/o other IP tunnels for now... */
	if (!PKTPRS_IS_MULTI_IP(SESS_RX_PKT(s)) &&
	    !PKTPRS_IS_MULTI_IP(SESS_TX_PKT(s))) {
		if (SESS_RX_IS_OUTER_V4(s)) {
			SESS_MOD_FLAG_SET(s, MOD_IPV4_BIT);
			SESS_MOD_FLAG_SET(s, MOD_IPV4_IPV6_TRANS_BIT);
			pr_debug("map-t ipv4 to ipv6 translation session\n");
		} else {
			SESS_MOD_FLAG_SET(s, MOD_IPV6_BIT);
			SESS_MOD_FLAG_SET(s, MOD_IPV6_IPV4_TRANS_BIT);
			pr_debug("map-t ipv6 to ipv4 translation session\n");
		}
		SESS_MOD_FLAG_SET(s, MOD_HDR_L3_RPLC_BIT);

		return true;
	}

	pr_debug("unsupported multi ip map-t session\n");
	return false;
}

/**
 * @brief Check whether a nat ipv4 session ip addresses has changed
 * @param s the session to test
 * @return bool true if they did, false otherwise
 */
static inline bool __smgr_is_ipv4_nat_addr_changed(struct sess_info *s)
{
	/* source address changed? */
	if (pktprs_ipv4_hdr(SESS_RX_PKT(s), SESS_RX_NAT_LVL(s))->saddr !=
	    pktprs_ipv4_hdr(SESS_TX_PKT(s), SESS_TX_NAT_LVL(s))->saddr)
		return true;
	/* dest address changed? */
	if (pktprs_ipv4_hdr(SESS_RX_PKT(s), SESS_RX_NAT_LVL(s))->daddr !=
	    pktprs_ipv4_hdr(SESS_TX_PKT(s), SESS_TX_NAT_LVL(s))->daddr)
		return true;

	return false;
}

/**
 * @brief Check whether a nat ipv6 session ip addresses has changed
 * @param s the session to test
 * @return bool true if they did, false otherwise
 */
static inline bool __smgr_is_ipv6_nat_addr_changed(struct sess_info *s)
{
	/* source address changed? */
	if (memcmp(&pktprs_ipv6_hdr(SESS_RX_PKT(s), SESS_RX_NAT_LVL(s))->saddr,
		   &pktprs_ipv6_hdr(SESS_TX_PKT(s), SESS_TX_NAT_LVL(s))->saddr,
		   sizeof(struct in6_addr)))
		return true;
	/* dest address changed? */
	if (memcmp(&pktprs_ipv6_hdr(SESS_RX_PKT(s), SESS_RX_NAT_LVL(s))->daddr,
		   &pktprs_ipv6_hdr(SESS_TX_PKT(s), SESS_TX_NAT_LVL(s))->daddr,
		   sizeof(struct in6_addr)))
		return true;

	return false;
}

/**
 * @brief Check whether the session's nat ip addresses has changed
 * @param s the session to test
 * @return bool true if they did, false otherwise
 */
static inline bool __smgr_is_ip_nat_addr_changed(struct sess_info *s)
{
	/* IPv4 */
	if (__smgr_is_ipv4_nat(s))
		return __smgr_is_ipv4_nat_addr_changed(s);
	/* IPv6 */
	if (__smgr_is_ipv6_nat(s))
		return __smgr_is_ipv6_nat_addr_changed(s);

	return false;
}

/**
 * @brief Check whether the session's is l3 bypass
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool __smgr_is_l3_bypass_sess(struct sess_info *s)
{
	int rx_l3_off;
	int tx_l3_off;
	u8  hdr_sz;

	/* L2 modifications allowed */
	if ((SESS_RX_OUTER_BMAP_NO_L2(s) != SESS_TX_OUTER_BMAP_NO_L2(s)) ||
		(SESS_RX_INNER_BMAP_NO_L2(s) != SESS_TX_INNER_BMAP_NO_L2(s)))
		return false;

	/* VPN bypass session will be handled later on */
	if (SESS_ARGS_IS_FLAG_ON(s, PP_SESS_FLAG_VPN_BIT))
		return false;

	rx_l3_off = pktprs_ip_hdr_off(SESS_RX_PKT(s), HDR_OUTER);
	tx_l3_off = pktprs_ip_hdr_off(SESS_TX_PKT(s), HDR_OUTER);

	if (rx_l3_off > 0 && tx_l3_off > 0) {
		/* Hdr size should be equal for rx and tx */
		hdr_sz = SESS_RX_PKT(s)->buf_sz - rx_l3_off;

		/* Check if rx and tx headres are equals for L3 */
		if (memcmp(&SESS_RX_PKT(s)->buf[rx_l3_off],
					&SESS_TX_PKT(s)->buf[tx_l3_off], hdr_sz))
			return false;
	}

	return true;
}

/**
 * @brief Check whether unknown L3/L4 protocols should be supported
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool smgr_is_unknown_proto_supported(struct sess_info *s)
{
	if (!SESS_RX_IS_OUTER_MAC(s) || !SESS_TX_IS_OUTER_MAC(s))
		return false;

	if (!SESS_RX_IS_OUTER_IP(s)) {
		/* In case of MAC packet with unknown l3 mode, supported */
		if (__smgr_is_accl_mode(PP_ACCL_MODE_UNKNOWN_L3))
			return true;
		else
			return false;
	}

	if (!(SESS_RX_IS_OUTER_L4(s) || SESS_RX_IS_OUTER_ESP(s))) {
		/* In case of IP packet with unknown l4 mode, supported */
		if (__smgr_is_accl_mode(PP_ACCL_MODE_UNKNOWN_L4))
			return true;
	}

	return false;
}

/**
 * @brief Check whether the bypass session is supported
 * @note Assumes the session is bypass verified by smgr_is_bypass_sess
 * @note Since we already verified the Rx and Tx headers are the same
 *       We can only look at one of them, also IPv6 extensions are ignored for
 *       bypass sessions.
 * @param s the session to test
 * @return bool true if it is, false otherwise
 */
static inline bool smgr_is_l3_bypass_sess_supported(struct sess_info *s)
{
	switch (SESS_RX_OUTER_BMAP_NO_L2_NO_IPV6_OPT(s)) {
	/* TCP and IPSec bypass - supported */
	case SESS_BYPASS_IPV4_TCP:
	case SESS_BYPASS_IPV6_TCP:
	case SESS_BYPASS_IPV4_ESP:
	case SESS_BYPASS_IPV6_ESP:
		goto supported;

	case SESS_BYPASS_IPV4_UDP:
	case SESS_BYPASS_IPV6_UDP:
		switch (SESS_RX_INNER_BMAP_NO_L2_NO_IPV6_OPT(s)) {
		/* UDP bypass or IPSec over UDP - supported */
		case 0:
		case SESS_BYPASS_ESP:
			goto supported;

		default:
			break;
		}
		break;

	/* VXLAN bypass - supported */
	case SESS_BYPASS_IPV4_UDP_VXLAN:
	case SESS_BYPASS_IPV6_UDP_VXLAN:
		goto supported;

	/* GRE/IPinIP bypass - supported */
	case SESS_BYPASS_IPV4:
	case SESS_BYPASS_IPV6:
	case SESS_BYPASS_IPV4_GRE:
	case SESS_BYPASS_IPV6_GRE:
		switch (SESS_RX_INNER_BMAP_NO_L2_NO_IPV6_OPT(s)) {
		case SESS_BYPASS_IPV4_UDP:
		case SESS_BYPASS_IPV6_UDP:
		case SESS_BYPASS_IPV4_TCP:
		case SESS_BYPASS_IPV6_TCP:
		case SESS_BYPASS_IPV4_ESP:
		case SESS_BYPASS_IPV6_ESP:
			goto supported;
		default:
			break;
		}
		break;

	/* L2TP over UDP bypass - supported */
	case SESS_BYPASS_IPV4_L2TP_OUDP:
	case SESS_BYPASS_IPV6_L2TP_OUDP:
		switch (SESS_RX_INNER_BMAP_NO_L2_NO_IPV6_OPT(s)) {
		case SESS_BYPASS_IPV4_UDP:
		case SESS_BYPASS_IPV6_UDP:
		case SESS_BYPASS_IPV4_TCP:
		case SESS_BYPASS_IPV6_TCP:
			goto supported;
		default:
			break;
		}
		break;

	default:
		break;
	}

	pr_debug("un-supported L3 bypass session\n");
	return false;

supported:
	pr_debug("L3 bypass session supported\n");
	s->l3_bypass = true;
	return true;
}

/* ========================================================================== */
/*                               Stats                                        */
/* ========================================================================== */
s32 pp_smgr_stats_get(struct smgr_stats *stats)
{
	atomic_t *db_cnt, *cnt;
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(stats))
		return -EINVAL;

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	cnt = (atomic_t *)stats;
	for_each_struct_mem(&db->stats, db_cnt, cnt++)
		atomic_set(cnt, atomic_read(db_cnt));

	return 0;
}

s32 pp_smgr_stats_reset(void)
{
	u32 free, open;
	struct smgr_database *db = smgr_get_db();

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	__smgr_lock();
	free = atomic_read(&db->stats.sess_free);
	open = atomic_read(&db->stats.sess_open);

	memset(&db->stats, 0, sizeof(db->stats));

	atomic_set(&db->stats.sess_free, free);
	atomic_set(&db->stats.sess_open, open);

	__smgr_unlock();
	return 0;
}

s32 pp_accl_mode_set(u8 mode)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return -EPERM;

	if (mode >= PP_ACCL_MODE_NUM)
		return -EINVAL;

	__smgr_lock();

	db->accl_mode = mode;

	__smgr_unlock();

	return 0;
}
EXPORT_SYMBOL(pp_accl_mode_set);

s32 pp_accl_mode_get(u8 *mode)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db) || ptr_is_null(mode))
		return -EPERM;

	__smgr_lock();

	*mode = db->accl_mode;

	__smgr_unlock();

	return 0;
}
EXPORT_SYMBOL(pp_accl_mode_get);

/**
 * @brief Update stats upon session create request
 */
static inline void __smgr_stats_sess_cr_req(struct smgr_database *db)
{
	atomic_inc(&db->stats.sess_create_req);
}

/**
 * @brief Update stats upon session delete request
 */
static inline void __smgr_stats_sess_del_req(struct smgr_database *db)
{
	atomic_inc(&db->stats.sess_delete_req);
}

/**
 * @brief Update stats upon port flush request
 */
static inline void __smgr_stats_port_flush_req(struct smgr_database *db)
{
	atomic_inc(&db->stats.port_flush_req);
}

/**
 * @brief Update stats upon port flush request
 */
static inline void __smgr_stats_flush_all_req(struct smgr_database *db)
{
	atomic_inc(&db->stats.flush_all_req);
}

/**
 * @brief Update stats upon port flush request
 */
static inline void __smgr_stats_inactive_req(struct smgr_database *db)
{
	atomic_inc(&db->stats.inactive_req);
}

/**
 * @brief Update stats upon session created
 */
static inline void __smgr_stats_sess_created(struct sess_info *s)
{
	struct smgr_stats *stats;
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	stats = &db->stats;
	atomic_inc(&stats->sess_created);
	atomic_inc(&stats->sess_open);
	atomic_dec(&stats->sess_free);

	if (atomic_read(&stats->sess_open_hi_wm) <
	    atomic_read(&stats->sess_open)) {
		atomic_set(&stats->sess_open_hi_wm,
			   atomic_read(&stats->sess_open));
	}

	/* TBD: update session type based stats: IPv4, IPv6, TCP, UDP */
}

/**
 * @brief Update stats upon session deleted
 */
static inline void __smgr_stats_sess_deleted(struct sess_db_entry *ent)
{
	struct smgr_stats *stats;
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	stats = &db->stats;
	atomic_inc(&stats->sess_deleted);
	atomic_dec(&stats->sess_open);
	atomic_inc(&stats->sess_free);
	/* TBD: update session type based stats: IPv4, IPv6, TCP, UDP */
}

/**
 * @brief Update stats upon port flushed
 */
static inline void __smgr_stats_port_flushed(struct smgr_database *db)
{
	atomic_inc(&db->stats.port_flushed);
}

/**
 * @brief Update stats upon flush all done
 */
static inline void __smgr_stats_inactive_done(struct smgr_database *db)
{
	atomic_inc(&db->stats.inactive_done);
}

/**
 * @brief Update stats upon flush all done
 */
static inline void __smgr_stats_flush_all_done(struct smgr_database *db)
{
	atomic_inc(&db->stats.flush_all_done);
}

/**
 * @brief Update stats upon invalid session create args error
 */
static inline void __smgr_stats_invalid_args(void)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	atomic_inc(&db->stats.invalid_args);
}

/**
 * @brief Update stats upon session create failure
 */
static inline void __smgr_stats_sess_cr_failed(struct sess_info *sess)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	atomic_inc(&db->stats.sess_create_fail);
}

/**
 * @brief Update stats upon session delete failure
 */
static inline void __smgr_stats_sess_del_failed(void)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	atomic_inc(&db->stats.sess_delete_fail);
}

/**
 * @brief Update stats upon session update failure
 */
static inline void __smgr_stats_sess_update_failed(void)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	atomic_inc(&db->stats.sess_update_fail);
}

/**
 * @brief Update stats upon port flush failure
 */
static inline void __smgr_stats_port_flush_failed(struct smgr_database *db)
{
	atomic_inc(&db->stats.port_flush_fail);
}

/**
 * @brief Update stats upon flush all failure
 */
static inline void __smgr_stats_flush_all_failed(struct smgr_database *db)
{
	atomic_inc(&db->stats.flush_all_fail);
}

/**
 * @brief Update stats upon flush all failure
 */
static inline void __smgr_stats_inactive_failed(struct smgr_database *db)
{
	atomic_inc(&db->stats.inactive_fail);
}

/**
 * @brief Update stats upon session lookup failure
 */
static inline void __smgr_stats_sess_lu_fail(struct sess_info *s)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	atomic_inc(&db->stats.sess_lu_fail);
}

/**
 * @brief Update stats upon successful session lookup
 */
static inline void __smgr_stats_sess_lu_succ(struct sess_info *s)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	atomic_inc(&db->stats.sess_lu_succ);
}

/**
 * @brief Update stats upon session lookup error
 */
static inline void __smgr_stats_sess_lu_err(struct sess_info *s)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	atomic_inc(&db->stats.sess_lookup_err);
}

/**
 * @brief Update stats upon a request to create session for
 *        unsupported packet
 */
static inline void __smgr_stats_sess_no_supported(struct sess_info *s)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	atomic_inc(&db->stats.sess_not_supported);
}

/**
 * @brief Update stats upon si create error
 */
static inline void __smgr_stats_sess_si_create_err(struct sess_info *s)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	atomic_inc(&db->stats.sess_si_create_err);
}

/**
 * @brief Update stats upon hal layer error
 */
static inline void __smgr_stats_hal_err(void)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	atomic_inc(&db->stats.hal_err);
}

/**
 * @brief Update stats upon hw sessions table full error
 */
static inline void __smgr_stats_hw_table_full(void)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	atomic_inc(&db->stats.hw_table_full);
}

/**
 * @brief Update stats upon resource busy
 */
static inline void __smgr_stats_resource_busy(void)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	atomic_inc(&db->stats.resource_busy);
}

/* ========================================================================== */
/*                               MISC                                         */
/* ========================================================================== */
s32 smgr_sessions_bmap_alloc(ulong **bmap, u32 *n_bits)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return -EPERM;
	if (ptr_is_null(bmap) || ptr_is_null(n_bits))
		return -EINVAL;

	*bmap = kcalloc(BITS_TO_LONGS(db->n_sessions), sizeof(**bmap),
			GFP_KERNEL);
	if (unlikely(!*bmap)) {
		pr_err("Failed to allocate bitmap for %u sessions\n",
		       db->n_sessions);
		return -ENOMEM;
	}
	*n_bits = db->n_sessions;

	return 0;
}

s32 smgr_sessions_arr_alloc(u32 **sessions, u32 *n_sessions)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return -EPERM;

	if (unlikely(ptr_is_null(sessions) || ptr_is_null(n_sessions)))
		return -EINVAL;

	*sessions = kcalloc(db->n_sessions, sizeof(**sessions), GFP_KERNEL);
	if (unlikely(!*sessions)) {
		pr_err("Failed to allocate sessions array for %u sessions\n",
		       db->n_sessions);
		return -ENOMEM;
	}
	*n_sessions = db->n_sessions;

	return 0;
}

bool smgr_is_sess_id_valid(u32 id)
{
	struct smgr_database *db = smgr_get_db();

	if (unlikely(!pp_is_ready()))
		return false;

	if (likely(id < db->n_sessions))
		return true;

	pr_err("invalid session id %u, max id %u\n",
	       id, db->n_sessions - 1);
	return false;
}

s32 smgr_session_si_get(u32 id, struct pp_si *si)
{
	struct pp_hw_si hw_si;
	s32 ret;

	if (unlikely(!smgr_is_sess_id_valid(id)))
		return -EINVAL;

	ret = cls_session_si_get(id, &hw_si);
	if (unlikely(ret))
		return ret;

	return pp_si_decode(si, &hw_si);
}

s32 smgr_session_dsi_get(u32 id, struct pp_dsi *dsi)
{
	struct pp_hw_dsi hw_dsi;
	s32 ret;

	if (unlikely(!smgr_is_sess_id_valid(id)))
		return -EINVAL;

	ret = chk_session_dsi_get(id, &hw_dsi);
	if (unlikely(ret))
		return ret;

	return pp_dsi_decode(dsi, &hw_dsi);
}

s32 pp_max_sessions_get(u32 *max_sessions)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(max_sessions))
		return -EINVAL;

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	*max_sessions = db->n_sessions;
	return 0;
}
EXPORT_SYMBOL(pp_max_sessions_get);

s32 pp_open_sessions_get(u32 *open_sessions)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(open_sessions))
		return -EINVAL;

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	*open_sessions = atomic_read(&db->stats.sess_open);
	return 0;
}
EXPORT_SYMBOL(pp_open_sessions_get);

s32 pp_created_sessions_get(u32 *created_sessions)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(created_sessions))
		return -EINVAL;

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	*created_sessions = atomic_read(&db->stats.sess_created);

	return 0;
}
EXPORT_SYMBOL(pp_created_sessions_get);

s32 pp_session_stats_get(u32 id, struct pp_stats *stats)
{
	struct pp_dsi dsi;
	s32 ret;

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	if (unlikely(ptr_is_null(stats)))
		return -EINVAL;

	__smgr_lock();
	if (unlikely(!__smgr_is_sess_exist(id))) {
		ret = -EINVAL;
		goto unlock;
	}
	/* decode the si */
	ret = smgr_session_dsi_get(id, &dsi);
	if (unlikely(ret))
		goto unlock;

	stats->bytes   = dsi.bytes_cnt;
	stats->packets = dsi.pkts_cnt;
unlock:
	__smgr_unlock();
	return ret;
}
EXPORT_SYMBOL(pp_session_stats_get);

s32 pp_session_stats_mod(u32 id, enum pp_stats_op act, u8 pkts, u32 bytes)
{
	bool exception = false;
	s32 ret;

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	__smgr_lock();
	if (unlikely(!__smgr_is_sess_exist(id))) {
		ret = -EINVAL;
		goto unlock;
	}

	switch (act) {
	case PP_STATS_SUB:
		ret = chk_session_stats_mod(id, false, pkts, bytes, exception);
		break;
	case PP_STATS_ADD:
		ret = chk_session_stats_mod(id, true, pkts, bytes, exception);
		break;
	case PP_STATS_RESET:
		ret = chk_session_stats_reset(id);
		break;
	default:
		pr_err("Invalid operation %u\n", act);
		ret = -EINVAL;
	}

	if (unlikely(ret))
		pr_err("failed to %s session %u counters\n",
		       PP_STATS_OP_STR(act), id);

unlock:
	__smgr_unlock();
	return ret;
}
EXPORT_SYMBOL(pp_session_stats_mod);

s32 pp_session_dst_queue_update(u32 id, u16 dst_q)
{
	u16 phy_dst_q;
	s32 ret;

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	__smgr_lock();
	if (unlikely(!__smgr_is_sess_exist(id))) {
		ret = -EINVAL;
		goto unlock;
	}
	ret = smgr_get_queue_phy_id(dst_q, &phy_dst_q);
	if (unlikely(ret))
		goto unlock;

	ret = chk_session_dsi_q_update(id, phy_dst_q);
	if (unlikely(ret))
		pr_err("failed to update session %u dest queue to %u\n",
		       id, dst_q);

unlock:
	__smgr_unlock();
	return ret;
}
EXPORT_SYMBOL(pp_session_dst_queue_update);

s32 pp_session_active_state_get(u32 id, bool *active)
{
	struct pp_dsi dsi;
	s32 ret;

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	if (unlikely(ptr_is_null(active)))
		return -EINVAL;

	__smgr_lock();
	if (unlikely(!__smgr_is_sess_exist(id))) {
		ret = -EINVAL;
		goto unlock;
	}
	/* decode the si */
	ret = smgr_session_dsi_get(id, &dsi);
	if (unlikely(ret))
		goto unlock;

	*active = dsi.active;
unlock:
	__smgr_unlock();
	return ret;
}
EXPORT_SYMBOL(pp_session_active_state_get);

s32 pp_exception_pkts_get(u64 *pkts)
{
	if (!pp_is_ready())
		return -EPERM;

	return chk_exceptions_pkts_get(pkts);
}
EXPORT_SYMBOL(pp_exception_pkts_get);

s32 pp_session_size_get(size_t *sz)
{
	if (unlikely(ptr_is_null(sz)))
		return -EINVAL;

	*sz = sizeof(struct pp_hw_si);
	return 0;
}
EXPORT_SYMBOL(pp_session_size_get);

s32 pp_session_hash_get(u32 id, struct pp_hash *hash)
{
	struct sess_db_entry *sess;
	s32 ret = 0;

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	if (unlikely(ptr_is_null(hash)))
		return -EINVAL;

	__smgr_lock();
	if (unlikely(!__smgr_is_sess_exist(id))) {
		ret = -EINVAL;
		goto unlock;
	}

	sess = __smgr_get_sess_db_ent(id);
	if (unlikely(!sess)) {
		ret = -EPERM;
		goto unlock;
	}

	memcpy(hash, &sess->info.hash, sizeof(*hash));
unlock:
	__smgr_unlock();
	return ret;
}
EXPORT_SYMBOL(pp_session_hash_get);

s32 pp_session_dbg_hash_get(struct pp_hash *hash)
{
	return cls_dbg_hash_get(hash);
}
EXPORT_SYMBOL(pp_session_dbg_hash_get);

s32 smgr_open_sessions_bmap_get(ulong *bmap, u32 n_bits)
{
	u32 sz;
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return -EPERM;
	if (ptr_is_null(bmap))
		return -EINVAL;

	__smgr_lock();
	/* copy as many sessions as we can */
	sz = sizeof(*db->open_bmap) *
	     BITS_TO_LONGS(min_t(u32, db->n_sessions, n_bits));
	memcpy(bmap, db->open_bmap, sz);
	__smgr_unlock();

	return 0;
}

s32 smgr_session_info_get(u32 id, struct sess_db_info *info)
{
	struct sess_db_entry *ent;
	s32 ret = 0;

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	if (unlikely(ptr_is_null(info)))
		return -EINVAL;

	__smgr_lock();
	if (!__smgr_is_sess_exist(id)) {
		ret = -EINVAL;
		goto unlock;
	}

	ent = __smgr_get_sess_db_ent(id);

	if (unlikely(ptr_is_null(ent)))
		return -EINVAL;

	memcpy(info, &ent->info, sizeof(ent->info));
unlock:
	__smgr_unlock();
	return  ret;
}

s32 smgr_session_dest_queue_get(u32 id, u16 *dest_q)
{
	struct pp_dsi dsi;
	s32 ret = 0;

	if (ptr_is_null(dest_q))
		return -EINVAL;

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	__smgr_lock();
	if (!__smgr_is_sess_exist(id)) {
		ret = -EINVAL;
		goto unlock;
	}

	ret = smgr_session_dsi_get(id, &dsi);
	if (!ret)
		*dest_q = dsi.dst_q;

unlock:
	__smgr_unlock();
	return  ret;
}

s32 smgr_syncq_set(bool enable)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return -EINVAL;

	db->syncq_en = enable;

	pr_debug("Set syncq %s\n", BOOL2STR(db->syncq_en));

	return 0;
}

bool smgr_syncq_get(void)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return false;

	return db->syncq_en;
}

s32 smgr_state_set(enum smgr_state state)
{
	struct smgr_database *db = smgr_get_db();

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	if (unlikely(!(state >= SMGR_DISABLE &&
		       state <= SMGR_ENABLE))) {
		pr_err("invalid state %u\n", state);
		return -EINVAL;
	}

	__smgr_lock();
	if (state == SMGR_DISABLE)
		__smgr_port_flush(PP_MAX_PORT);
	db->state = state;
	__smgr_unlock();
	pr_debug("PP Session Manager %s\n", state == SMGR_ENABLE ?
		 "enabled" : "disabled");

	return 0;
}

enum smgr_state smgr_state_get(void)
{
	struct smgr_database *db = smgr_get_db();

	if (!pp_is_ready())
		return SMGR_DISABLE;

	return db->state;
}

s32 smgr_open_frag_sess_set(bool enable)
{
	struct smgr_database *db = smgr_get_db();

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	db->open_frag_sess = enable;

	pr_debug("Set open_frag_sess %s\n", BOOL2STR(db->open_frag_sess));

	return 0;
}

bool smgr_open_frag_sess_get(void)
{
	struct smgr_database *db = smgr_get_db();

	if (!pp_is_ready())
		return false;

	return db->open_frag_sess;
}

s32 smgr_open_lld_sess_set(bool enable)
{
	struct smgr_database *db = smgr_get_db();

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	db->open_lld_sess = enable;

	pr_debug("Set open_lld_sess %s\n", BOOL2STR(db->open_lld_sess));

	return 0;
}

bool smgr_open_lld_sess_get(void)
{
	struct smgr_database *db = smgr_get_db();

	if (!pp_is_ready())
		return false;

	return db->open_lld_sess;
}

s32 smgr_sessions_scan_state_set(bool en)
{
	struct smgr_database *db = smgr_get_db();

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	__smgr_lock();
	if (en) {
		if (db->scan_state == INACT_SCAN_DISABLE)
			db->scan_state = INACT_SCAN_IDLE;
	} else {
		db->scan_state = INACT_SCAN_DISABLE;
	}
	__smgr_unlock();
	pr_debug("PP Session Manager Inactive Scan %s\n", BOOL2EN(en));

	return 0;
}

bool smgr_sessions_scan_state_get(void)
{
	struct smgr_database *db = smgr_get_db();

	if (!pp_is_ready())
		return false;

	return db->scan_state == INACT_SCAN_DISABLE ? false : true;
}

/* ========================================================================== */
/*                               Session Create                               */
/* ========================================================================== */

/**
 * @brief Save some extra stuff in the database for better debugging
 * @param s the session
 */
static inline void __smgr_sess_debug(struct sess_info *s)
{
#ifdef CONFIG_DEBUG_FS
	struct sess_db_info *info = &s->db_ent->info;

	/* save ingress packet info */
	if (SESS_RX_PKT(s))
		memcpy(&info->rx, SESS_RX_PKT(s), sizeof(info->rx));
	/* save egress packet info */
	if (SESS_TX_PKT(s))
		memcpy(&info->tx, SESS_TX_PKT(s), sizeof(info->tx));
#endif
}

/**
 * @brief Test whether session args pointers are valid, args pointers
 *        is considered to be invalid in case that ingress packet
 *        isn't NULL but egress packet is or vice versa
 * @param args args to verify
 * @return bool true in case all pointers are ok, false otherwise
 */
static bool __smgr_is_sess_args_ptrs_valid(struct pp_sess_create_args *args)
{
	bool res = true;

	if (unlikely(ptr_is_null(args)))
		return false;

	if (unlikely(args->rx && !args->tx)) {
		pr_err("ingress packet without egress packet\n");
		res = false;
	}

	if (unlikely(!args->rx && args->tx)) {
		pr_err("egress packet without ingress packet\n");
		res = false;
	}

	return res;
}

/**
 * @brief Test whether session classification info is valid or not
 *        Session's classification info is considered to be valid in
 *        case all fields length combined doesn't exceeds the maximum
 *        allowed
 * @param args session args
 * @return bool true in case the classification info is valid, false
 *         otherwise
 */
static bool __smgr_is_cls_info_valid(struct sess_info *s)
{
	struct pp_sess_create_args *args = s->args;
	struct pp_port_cls_info *port_cls;
	struct pp_port_cls_data *dcls;
	unsigned long mask;
	bool res = true;
	u32 i;

	/* for multicast dst sessions, session manager
	 * will set the cls info internally
	 */
	if (test_bit(PP_SESS_FLAG_MCAST_DST_BIT, &args->flags))
		return true;

	port_cls = &s->in_port_cfg.rx.cls;
	dcls = &args->cls;

	if (unlikely(dcls->n_flds != port_cls->n_flds)) {
		pr_err("session classification is different from port classification\n");
		pr_err("session n_flds = %u, port n_flds = %u\n",
		       dcls->n_flds, port_cls->n_flds);
		res = false;
	}

	for (i = 0; i < dcls->n_flds; i++) {
		if (!port_cls->cp[i].copy_size)
			continue;
		mask = GENMASK(port_cls->cp[i].copy_size - 1, 0);
		if (unlikely(!__pp_mask_value_valid(mask, dcls->fld_data[i]))) {
			pr_err("field data %#x at index %u doesn't match copy_size of %u\n",
			       dcls->fld_data[i], i,
			       port_cls->cp[i].copy_size);
			res = false;
		}
	}

	return res;
}

/**
 * @brief Test whether session create parameters are valid or not
 * @param args the args to check
 * @return bool true if args are valid, false otherwise
 */
static bool __smgr_is_sess_args_valid(struct sess_info *s)
{
	struct pp_sess_create_args *args = s->args;
	bool res = true;

	if (unlikely(!__smgr_is_sess_args_ptrs_valid(args)))
		return false;
	if (unlikely(!__pp_is_color_valid(args->color)))
		res = false;
	if (unlikely(!__smgr_is_cls_info_valid(s)))
		res = false;
	if (unlikely(!smgr_mcast_is_info_valid(args)))
		res = false;
	if (false == res)
		__smgr_stats_invalid_args();

	return res;
}

/**
 * @brief Calculate packet processor field vector size based on the
 *        learning field vector information
 *        the calculation is pretty simple, if a layer exist we set
 *        the size to be everything from the start of the field vector
 *        up to that layer including the layer itself
 * @param s session info to save the real fv size
 * @return 0 always
 */
static s32 __smgr_calc_fv_real_sz(struct sess_info *s)
{
	/* oob always exist */
	s->si.fv_sz = sizeof(s->si.fv.oob);
	if (!SESS_RX_PKT(s))
		goto roundup;

	/* outer l2 */
	if (PKTPRS_IS_MAC(SESS_RX_PKT(s), HDR_OUTER))
		s->si.fv_sz = offsetof(struct pp_fv, first.l3);
	/* outer l3 */
	if (SESS_RX_IS_OUTER_IP(s))
		s->si.fv_sz = offsetof(struct pp_fv, first.l4);
	/* outer l4 */
	if (SESS_RX_IS_OUTER_L4(s) || SESS_RX_IS_OUTER_ESP(s))
		s->si.fv_sz = offsetof(struct pp_fv, tunn);
	/* tunnel */
	if (PKTPRS_IS_VXLAN(SESS_RX_PKT(s), HDR_OUTER)  ||
	    PKTPRS_IS_GENEVE(SESS_RX_PKT(s), HDR_OUTER) ||
	    PKTPRS_IS_L2TP_OUDP(SESS_RX_PKT(s), HDR_OUTER) ||
	    PKTPRS_IS_PPPOE(SESS_RX_PKT(s), HDR_OUTER) ||
	    PKTPRS_IS_PPPOE(SESS_RX_PKT(s), HDR_INNER))
		s->si.fv_sz = offsetof(struct pp_fv, second);
	/* inner l2 */
	if (PKTPRS_IS_MAC(SESS_RX_PKT(s), HDR_INNER))
		s->si.fv_sz = offsetof(struct pp_fv, second.l3);
	/* inner l3 */
	if (SESS_RX_IS_INNER_IP(s))
		s->si.fv_sz = offsetof(struct pp_fv, second.l4);
	/* inner l4 */
	if (SESS_RX_IS_INNER_L4(s))
		s->si.fv_sz = sizeof(s->si.fv);

roundup:
	s->si.fv_sz = roundup(s->si.fv_sz, 16);
	return 0;
}

/**
 * @brief Helper function for building the pp field vector out of band
 *        info for the ingress packet
 * @param s session info
 */
static void __smgr_fv_oob_set(struct sess_info *s)
{
	struct pp_fv_oob *oob = &s->si.fv.oob;
	struct pp_port_cls_data *dcls;
	struct pp_port_cls_info *port_cls;
	u8 ext_proto, int_proto, i;
	unsigned long mask, start, end;

	/* set ingress port */
	oob->in_pid = s->args->in_port;
	/* set classification data */
	port_cls = &s->in_port_cfg.rx.cls;
	dcls = &s->args->cls;
	for (start = 0, i = 0; i < dcls->n_flds; i++) {
		end    = start + port_cls->cp[i].copy_size - 1;
		mask   = GENMASK(end, start);
		start += port_cls->cp[i].copy_size;
		oob->stw_data = PP_FIELD_MOD(mask, dcls->fld_data[i],
					     oob->stw_data);
		pr_debug("data %#x, end %lu, start %lu, mask %#lx, stw_data %#x\n",
			 dcls->fld_data[i], end, start, mask, oob->stw_data);
	}
	/* To support oob sessions (sessions with field vector only with
	 * oob info) we need to set external and internal exactly as
	 * the parser will do, PRSR_PROTO_NO_PARSE
	 */
	if (!SESS_RX_PKT(s)) {
		ext_proto = PP_FV_OTHER_L3_UNKNOWN_L4;
		int_proto = PP_FV_UNKNOWN_L3;
		goto done;
	}

	/* set proto info */
	ext_proto = pp_fv_proto_set(SESS_RX_PKT(s), HDR_OUTER);
	int_proto = pp_fv_proto_set(SESS_RX_PKT(s), HDR_INNER);

done:
	if (ext_proto != PP_FV_UNKNOWN_L3 &&
	    int_proto == PP_FV_UNKNOWN_L3) {
		int_proto = ext_proto;
		ext_proto = PP_FV_UNKNOWN_L3;
	}

	oob->proto_info = PP_FV_OOB_EXT_PROTO_INFO_SET(oob->proto_info,
						       ext_proto);
	oob->proto_info = PP_FV_OOB_INT_PROTO_INFO_SET(oob->proto_info,
						       int_proto);

	pr_debug("ext_proto %u, int_proto %u, proto_info %#x\n",
		 ext_proto, int_proto, oob->proto_info);
}

static s32 hash_calc(struct sess_info *sess, void *fv, u32 fv_sz,
		     struct pp_hash *hash)
{
	if (SESS_ARGS_IS_FLAG_OFF(sess, PP_SESS_FLAG_INTERNAL_HASH_CALC_BIT))
		return 0;

	return cls_hash_calc(fv, fv_sz, hash);
}

/**
 * @brief Preform session lookup based on the ingress packet
 * @param s session info
 * @return s32 -EEXIST in case session was found, 0 if session wasn't
 *         found, error code otherwise
 */
static s32 __smgr_sess_lookup(struct sess_info *s)
{
	struct pp_sess_create_args *args = s->args;
	struct pp_fv lu_fv;
	s32 ret = 0;
	u32 id;

	/* check if the group/dst exist for multicast sessions */
	ret = smgr_mcast_sess_lookup(s);
	if (unlikely(ret))
		goto done;

	/* convert the learning ingress fv to pp fv */
	ret = pp_fv_build(SESS_RX_PKT(s), &s->si.fv);
	if (unlikely(ret))
		goto done;

	__smgr_fv_oob_set(s);
	/* calculate the real size of the field vector */
	ret = __smgr_calc_fv_real_sz(s);
	if (unlikely(ret))
		goto done;
	pr_debug("fv sz set to %u\n", s->si.fv_sz);

	/* prepare field vector for the lookup, the field vector is
	 * swapped in the SI so we swap it before we lookup
	 */
	__buf_swap(&lu_fv, sizeof(lu_fv), &s->si.fv, s->si.fv_sz);

	ret = hash_calc(s, &s->si.fv, s->si.fv_sz, &args->hash);
	if (unlikely(ret))
		goto done;

	/* lookup */
	ret = cls_session_lookup(&lu_fv, s->si.fv_sz, &args->hash, &id);
	if (unlikely(ret))
		goto done;

	if (id == CLS_SESSION_INVALID)
		/* session do not exist, done */
		goto done;

	/* just make sure the result from classifier is good */
	if (unlikely(!__smgr_is_sess_exist(id))) {
		pr_debug("session id %u returned from classifier does not exist\n",
		         id);
		ret = -EFAULT;
		goto done;
	}

	/* get the reference to the session entry */
	s->db_ent = __smgr_get_sess_db_ent(id);
	ret = -EEXIST;

done:
	if (ret == (-EEXIST))
		__smgr_stats_sess_lu_succ(s);
	else if (unlikely(ret))
		__smgr_stats_sess_lu_err(s);
	else
		__smgr_stats_sess_lu_fail(s);

	return ret;
}

/**
 * @brief Test if a session is supported for acceleration
 * @param s the session
 * @return bool true in case the can be accelerated, false otherwise
 */
static bool __smgr_is_sess_supported(struct sess_info *s)
{
	/* support no packets based sessions */
	if (!SESS_RX_PKT(s) || !SESS_TX_PKT(s))
		return true;

	pr_debug("rx: outer bmap %#lx, inner bmap %#lx\n",
		 SESS_RX_OUTER_BMAP(s), SESS_RX_INNER_BMAP(s));
	pr_debug("tx: outer bmap %#lx, inner bmap %#lx\n",
		 SESS_TX_OUTER_BMAP(s), SESS_TX_INNER_BMAP(s));

	if (!SESS_RX_IS_OUTER_IP(s) &&
		!__smgr_is_accl_mode(PP_ACCL_MODE_UNKNOWN_L3)) {
		if (__smgr_is_moca_ctp_sess(s))
			return true;
		if (!SESS_RX_IS_OUTER_MPLS(s)) {
			pr_debug("rx outer packet unsupported l3\n");
			goto not_supported;
		}
	}

	if (!__smgr_is_pppoe_supported(s)) {
		pr_debug("unsupported pppoe\n");
		goto not_supported;
	}

	if (!__smgr_is_frag_supported(s)) {
		pr_debug("unsupported IP frag\n");
		goto not_supported;
	}

	if (__smgr_is_l3_bypass_sess(s)) {
		if (smgr_is_unknown_proto_supported(s))
			return true;
		if (smgr_is_l3_bypass_sess_supported(s)) {
			pr_debug("L3 Bypass session");
			return true;
		}
		pr_debug("unsupported bypass session\n");
		goto not_supported;
	}

	/* for ESP we have a special case where outer IP doesn't exist */
	if (!SESS_TX_IS_OUTER_ESP(s) && !SESS_TX_IS_OUTER_IP(s) &&
	    !SESS_TX_IS_OUTER_MPLS(s)) {
		pr_debug("tx outer packet unsupported l3\n");
		goto not_supported;
	}

	if (!__smgr_is_ipv6_ext_supported(s)) {
		pr_debug("unsupported modifications in the ipv6 extension header\n");
		goto not_supported;
	}

	if (smgr_is_mapt_nat_sess(s)) {
		if (smgr_is_mapt_sess_supported(s))
			return true;
		pr_debug("unsupported map-t session\n");
		goto not_supported;
	}

	/* ESP */
	if (__smgr_is_esp_sess(s)) {
		if (__smgr_is_esp_supported(s))
			return true;
		pr_debug("unsupported esp session\n");
		goto not_supported;
	}

	/* tunnel bypass? supported */
	if (__smgr_is_tunn_bypass_sess(s)) {
		pr_debug("tunnel bypass session\n");
		return true;
	}

	/* no tunnel termination? check l4 */
	if (!__smgr_is_tunn_term_sess(s)) {
		if (!SESS_RX_IS_OUTER_L4(s)) {
			pr_debug("non tunnel termination ingrss outer packet unsupported l4\n");
			goto not_supported;
		}
		if (!SESS_TX_IS_OUTER_L4(s)) {
			pr_debug("non tunnel termination egress outer packet unsupported l4\n");
			goto not_supported;
		}
		pr_debug("non tunnel termination session\n");
		return true;
	}

	/* tunnel termination */
	/* DSLITE */
	if (__smgr_is_dslite_term_sess(s)) {
		pr_debug("dslite termination session\n");
		return true;
	}

	/* tunnel termination */
	/* VXLAN */
	if (__smgr_is_vxlan_term_sess(s)) {
		pr_debug("vxlan termination session\n");
		return true;
	}

	/* 6RD */
	if (__smgr_is_6rd_term_sess(s)) {
		pr_debug("6rd termination session\n");
		return true;
	}

	/* EoGRE */
	if (__smgr_is_eogre_term_sess(s)) {
		pr_debug("eogre termination session\n");
		return true;
	}

	/* IPoGRE */
	if (__smgr_is_ipogre_term_sess(s)) {
		pr_debug("ipogre termination session\n");
		return true;
	}

	/* L2TPUDP */
	if (smgr_is_l2tpudp_sess(s)) {
		if (smgr_is_l2tpudp_sess_supported(s)) {
			pr_debug("l2tp over udp session\n");
			return true;
		}
		pr_debug("unsupported l2tp over UDP session\n");
		goto not_supported;
	}

	/* L2TPIP */
	if (__smgr_is_l2tpip_sess(s)) {
		if (__smgr_is_l2tpip_sess_supported(s)) {
			pr_debug("l2tp over ip session\n");
			return true;
		}
		pr_debug("unsupported l2tp over ip session\n");
		goto not_supported;
	}

	/* EoMPLS */
	if (__smgr_is_eompls_sess_supported(s)) {
		pr_debug("EoMPLS session\n");
		return true;
	}

	/* don't support all other tunnels terminations */
not_supported:
	pr_debug("unsupported session\n");
	return false;
}

/**
 * @brief Prepare a session database entry for a newly created session
 * @param s session info to save the session db entry
 * @return s32 0 on successful session entry allocation, error
 *        code otherwise
 * @note Caller MUST acquire database lock
 */
static inline s32 __smgr_sess_ent_prepare(struct sess_info *s)
{
	struct pp_sess_create_args *args = s->args;
	struct sess_db_entry *sess_ent;
	struct smgr_database *db = smgr_get_db();
	u32 sess_id;

	if (ptr_is_null(db))
		return -EPERM;

	/* get the first entry from the free list */
	if (unlikely(list_empty(&db->free_list))) {
		pr_debug("No free session available\n");
		return -ENOSPC;
	}
	sess_ent = DB_FREE_LIST_GET_NODE(db);
	DB_FREE_LIST_DEL_NODE(sess_ent);

	/* Reset the info struct before using it */
	sess_id = sess_ent->info.sess_id;
	memset(&sess_ent->info, 0, sizeof(sess_ent->info));

	/* Prepare the entry */
	memcpy(&sess_ent->info.hash, &args->hash, sizeof(args->hash));
	sess_ent->info.sess_id   = sess_id;
	sess_ent->info.in_port   = args->in_port;
	sess_ent->info.eg_port   = args->eg_port;
	sess_ent->info.fv_sz     = s->si.fv_sz;
	sess_ent->info.mod_flags = *s->mod_flags;
	s->mod_flags             = &sess_ent->info.mod_flags;
	s->db_ent                = sess_ent;

	smgr_mcast_sess_ent_prepare(s);
	return 0;
}

/**
 * @brief Free session database entry back to the free list
 * @param ent session's database entry
 * @return s32 0 on successful session entry allocation, error
 *        code otherwise
 * @note Assume the function is called under session manager lock
 */
static s32 __smgr_sess_ent_free(struct sess_db_entry *ent)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db) || ptr_is_null(ent))
		return -EINVAL;

	/* Remove tdox entry if linked to this session */
	smgr_tdox_session_remove(ent);

	/* add the session back to the free list */
	DB_FREE_LIST_ADD_NODE(db, ent);

	return 0;
}

/**
 * @brief Set session's flags, the flags will be saved in the session
 *        db entry
 * @param s session info
 * @return s32 0 on success, error code otherwise
 */
static void __smgr_sess_flags_set(struct sess_info *s)
{
	struct smgr_database *db = smgr_get_db();
	u8 aqm_engine = PP_AQM_HW;

	if (ptr_is_null(db) || !SESS_RX_PKT(s) || !SESS_TX_PKT(s))
		return;

	if (__smgr_is_mac_changed(s, HDR_OUTER))
		SESS_FLAG_SET(s->db_ent, SESS_FLAG_ROUTED);

	if (db->syncq_en && SESS_ARGS_IS_FLAG_ON(s, PP_SESS_FLAG_SYNCQ_BIT))
		SESS_FLAG_SET(s->db_ent, SESS_FLAG_SYNCQ);

	/* Fragmentation should be potentially done when adding a tunnel
	 * In case we have ESP, we support fragmentation only in case this is
	 * a by-pass session or was explicitly asked by the user (PPA)
	 */
	if (pp_misc_is_nf_en(PP_NF_FRAGMENTER) && !SESS_RX_IS_OUTER_TCP(s)) {
		if (__smgr_is_iptun_encp_sess(s) ||
		    (__smgr_is_pppoe_encap(s) && !SESS_TX_IS_OUTER_ESP(s)) ||
		    test_bit(PP_SESS_FLAG_MTU_CHCK_BIT, &s->args->flags) ||
		    db->open_frag_sess) {
			SESS_FLAG_SET(s->db_ent, SESS_FLAG_MTU_CHCK);
			pr_debug("SESS_FLAG_MTU_CHCK is set\n");
		}
	}

	if (SESS_ARGS_IS_FLAG_ON(s, PP_SESS_FLAG_MCAST_GRP_BIT))
		SESS_FLAG_SET(s->db_ent, SESS_FLAG_MCAST_GRP);
	else if (SESS_ARGS_IS_FLAG_ON(s, PP_SESS_FLAG_MCAST_DST_BIT))
		SESS_FLAG_SET(s->db_ent, SESS_FLAG_MCAST_DST);

	if (SESS_ARGS_IS_FLAG_ON(s, PP_SESS_FLAG_REMARK_BIT))
		SESS_FLAG_SET(s->db_ent, SESS_FLAG_REMARK);

	if (pp_misc_is_nf_en(PP_NF_IPSEC_LLD)) {
		pp_misc_get_aqm_engine(&aqm_engine);
		if (SESS_ARGS_IS_FLAG_ON(s, PP_SESS_FLAG_AQM_BIT)) {
			if (aqm_engine == PP_AQM_SW)
				SESS_FLAG_SET(s->db_ent, SESS_FLAG_AQM_SW);
		} else if (db->open_lld_sess ||
			   SESS_ARGS_IS_FLAG_ON(s, PP_SESS_FLAG_LLD_BIT)) {
			SESS_FLAG_SET(s->db_ent, SESS_FLAG_LLD);
			if (aqm_engine == PP_AQM_SW)
				SESS_FLAG_SET(s->db_ent, SESS_FLAG_AQM_SW);

			if (SESS_ARGS_IS_FLAG_ON(s, PP_SESS_FLAG_TDOX_SUPP_BIT))
				clear_bit(PP_SESS_FLAG_TDOX_SUPP_BIT,
					  &s->args->flags);
		}
	}
}

/**
 * @brief Set the bitmaps and headers to use for NAT modifications
 * @param s the session
 */
static inline void __smgr_nat_info_init(struct sess_info *s)
{
	if (!SESS_RX_PKT(s) || !SESS_TX_PKT(s))
		return;

	/* start by setting outer packets */
	pr_debug("set both nat packets to outer\n");
	s->nat.rx_lvl = HDR_OUTER;
	s->nat.tx_lvl = HDR_OUTER;

	if (__smgr_is_iptun_decp_sess(s)) {
		pr_debug("iptun decap: set nat ingress to inner\n");
		s->nat.rx_lvl = HDR_INNER;
	} else if (__smgr_is_iptun_encp_sess(s)) {
		pr_debug("iptun encap: set nat egress to inner\n");
		s->nat.tx_lvl = HDR_INNER;
	} else if (__smgr_is_esp_nat_sess(s)) {
		if (SESS_TX_IS_INNER_IP(s)) {
			/* not all esp that require nat changes has inner IP
			 * e.g. trasport mode with UDP/TCP
			 */
			pr_debug("esp encap: set nat egress to inner\n");
			s->nat.tx_lvl = HDR_INNER;
		}
	} else if (__smgr_is_eompls_decp_sess(s)) {
		pr_debug("mpls decap: set nat ingress to inner\n");
		s->nat.rx_lvl = HDR_INNER;
	} else if (__smgr_is_eompls_encp_sess(s)) {
		pr_debug("mpls encap: set nat egress to inner\n");
		s->nat.tx_lvl = HDR_INNER;
	}
}

/**
 * @brief Set all si l4 related info, in general, we "always" do l4
 *        nat to spare recipes
 * @param s session info
 */
static void __smgr_si_napt_set(struct sess_info *s, __sum16 l3_csum)
{
	struct pp_si_sce *sce = &s->si.sce;
	struct pp_si_dpu *dpu = &s->si.dpu;
	__sum16 l4_csum = l3_csum;
	struct udphdr *rx_l4;
	struct udphdr *tx_l4;

	/* do napt only for TCP and UDP, TBD: ICMP */
	if (!(PKTPRS_IS_UDP(SESS_RX_PKT(s), SESS_RX_NAT_LVL(s)) &&
	     PKTPRS_IS_UDP(SESS_TX_PKT(s), SESS_TX_NAT_LVL(s))) &&
	    !(PKTPRS_IS_TCP(SESS_RX_PKT(s), SESS_RX_NAT_LVL(s)) &&
	     PKTPRS_IS_TCP(SESS_TX_PKT(s), SESS_TX_NAT_LVL(s))))
		return;

	sce->valid = true;
	/* set napt flag */
	SESS_MOD_FLAG_SET(s, MOD_L4_NAPT_BIT);
	/* set the offset to the checksum field */
	if (PKTPRS_IS_UDP(SESS_TX_PKT(s), SESS_TX_NAT_LVL(s))) {
		/* UDP */
		rx_l4 = pktprs_udp_hdr(SESS_RX_PKT(s), SESS_RX_NAT_LVL(s));
		tx_l4 = pktprs_udp_hdr(SESS_TX_PKT(s), SESS_TX_NAT_LVL(s));
		dpu->lyr_fld_off = offsetof(struct udphdr, check);
	} else {
		/* TCP */
		rx_l4 = (struct udphdr *)pktprs_tcp_hdr(SESS_RX_PKT(s),
							SESS_RX_NAT_LVL(s));
		tx_l4 = (struct udphdr *)pktprs_tcp_hdr(SESS_TX_PKT(s),
							SESS_TX_NAT_LVL(s));
		dpu->lyr_fld_off = offsetof(struct tcphdr, check);
	}

	/* calc checksum for the ports */
	if (rx_l4->source != tx_l4->source)
		csum_replace2(&l4_csum, rx_l4->source, tx_l4->source);
	if (rx_l4->dest != tx_l4->dest)
		csum_replace2(&l4_csum, rx_l4->dest, tx_l4->dest);
	sce->l4_csum_delta = ntohs(l4_csum);
	/* for udp we save the csum zero value so the recipe will
	 * not calculate the csum in case it is disabled (set to zero)
	 */
	if (PKTPRS_IS_UDP(SESS_TX_PKT(s), SESS_TX_NAT_LVL(s)))
		sce->l4_csum_zero = sce->l4_csum_delta;

	/* save ports */
	sce->new_dst_port = ntohs(tx_l4->dest);
	sce->new_src_port = ntohs(tx_l4->source);
}

/**
 * @brief Set all si ipv4 related info
 * @param s session info
 */
static s32 __smgr_si_ipv4_nat_set(struct sess_info *s)
{
	struct pp_si *si = &s->si;
	__sum16 l3_csum = 0;
	struct iphdr *rx_ip, *tx_ip;

	rx_ip = pktprs_ipv4_hdr(SESS_RX_PKT(s), SESS_RX_NAT_LVL(s));
	tx_ip = pktprs_ipv4_hdr(SESS_TX_PKT(s), SESS_TX_NAT_LVL(s));

	SESS_MOD_FLAG_SET(s, MOD_IPV4_NAT_BIT);
	si->sce.valid = true;

	/* set l3 csum delta */
	if (rx_ip->saddr != tx_ip->saddr)
		csum_replace4(&l3_csum, rx_ip->saddr, tx_ip->saddr);
	if (rx_ip->daddr != tx_ip->daddr)
		csum_replace4(&l3_csum, rx_ip->daddr, tx_ip->daddr);
	/* do l4 nat */
	__smgr_si_napt_set(s, l3_csum);
	/* ttl */
	if (__smgr_is_sess_routed(s)) {
		si->sce.ttl_diff = rx_ip->ttl - tx_ip->ttl;
		/* set IPv4 routed session ttl flag and update the l3 csum
		 * the checksum is calculated on both ttl and protocol fields
		 * and ttl is the MSB on the combined u16 word so every change
		 * in the ttl is in 0x100 granularity so decrementing ttl by 1
		 * will outcome with a change of 0x100 in the checksum
		 */
		csum_replace2(&l3_csum, htons(si->sce.ttl_diff << 8), htons(0));
	}

	/* to reduce number of recipes in the modifier we always
	 * modify the ip addresses, in esp encapsulation the IPs
	 * are copied via the new header except where inner ip exist
	 */
	if (!(__smgr_is_esp_encp_sess(s) && !SESS_TX_IS_INNER_IP(s))) {
		si->dpu.nat_sz = sizeof_field(struct pp_fv_ipv4, saddr) +
				 sizeof_field(struct pp_fv_ipv4, daddr);
		si->bce.nat.v4.saddr = tx_ip->saddr;
		si->bce.nat.v4.daddr = tx_ip->daddr;
	}

	/* dscp/tos */
	si->sce.dscp  = tx_ip->tos;
	if (rx_ip->tos != tx_ip->tos)
		csum_replace2(&l3_csum, htons(rx_ip->tos), htons(tx_ip->tos));

	/* add user's checksum delta */
	csum_replace2(&l3_csum, 0, ~s->args->ip_csum_delta);
	si->sce.l3_csum_delta = ntohs(l3_csum);

	return 0;
}

/**
 * @brief Set all si ipv6 related info
 * @note Not fully implemented
 * @param s session info
 */
static s32 __smgr_si_ipv6_nat_set(struct sess_info *s)
{
	struct pp_si *si = &s->si;

	/* set IPv6 routed session hop list flag */
	SESS_MOD_FLAG_SET(s, MOD_IPV6_HOPL_BIT);
	si->sce.valid = true;
	if (__smgr_is_sess_routed(s))
		si->sce.ttl_diff = 1;
	else
		si->sce.ttl_diff = 0;

	/* if the source or dest address didn't changed, do nothing */
	if (!__smgr_is_ipv6_nat_addr_changed(s))
		return 0;

	pr_debug("IPv6 NAT isn't supported\n");
	return -EPROTONOSUPPORT;
}

/**
 * @brief Mainly for calculating the L3 checksum diff, also save some stuff
 *        in the SI, e.g. TTL/HOP Limit diff
 * @param s the session
 * @return s32 return 0 when no error occur, error code otherwise
 */
static s32 smgr_si_mapt_nat_set(struct sess_info *s)
{
	struct pp_si *si = &s->si;
	struct ipv6hdr *ipv6;
	struct iphdr *ip;
	__sum16 l3_csum = 0;
	u32 i;

	si->sce.valid = true;

	if (SESS_RX_IS_OUTER_V4(s)) {
		/* IPv4 to IPv6 translation */
		ip = pktprs_ipv4_hdr(SESS_RX_PKT(s), SESS_RX_NAT_LVL(s));
		ipv6 = pktprs_ipv6_hdr(SESS_TX_PKT(s), SESS_TX_NAT_LVL(s));

		/* remove the ipv4 ip addresses */
		csum_replace4(&l3_csum, ip->saddr, 0);
		csum_replace4(&l3_csum, ip->daddr, 0);

		/* add the ipv6 ip addresses */
		for (i = 0; i < ARRAY_SIZE(ipv6->saddr.s6_addr32); i++)
			csum_replace4(&l3_csum, 0, ipv6->saddr.s6_addr32[i]);

		for (i = 0; i < ARRAY_SIZE(ipv6->daddr.s6_addr32); i++)
			csum_replace4(&l3_csum, 0, ipv6->daddr.s6_addr32[i]);

		/* do l4 nat now, cause l4 checksum only includes the
		 * pseudo header from the ip
		 */
		__smgr_si_napt_set(s, l3_csum);

		/* ttl */
		if (__smgr_is_sess_routed(s))
			si->sce.ttl_diff = ip->ttl - ipv6->hop_limit;
	} else {
		/* IPv6 to IPv4 translation */
		ipv6 = pktprs_ipv6_hdr(SESS_RX_PKT(s), SESS_RX_NAT_LVL(s));
		ip = pktprs_ipv4_hdr(SESS_TX_PKT(s), SESS_TX_NAT_LVL(s));

		/* remove the ipv6 ip addresses */
		for (i = 0; i < ARRAY_SIZE(ipv6->saddr.s6_addr32); i++)
			csum_replace4(&l3_csum, ipv6->saddr.s6_addr32[i], 0);

		for (i = 0; i < ARRAY_SIZE(ipv6->daddr.s6_addr32); i++)
			csum_replace4(&l3_csum, ipv6->daddr.s6_addr32[i], 0);

		/* remove the ipv4 ip addresses */
		csum_replace4(&l3_csum, 0, ip->saddr);
		csum_replace4(&l3_csum, 0, ip->daddr);

		__smgr_si_napt_set(s, l3_csum);

		if (__smgr_is_sess_routed(s))
			si->sce.ttl_diff = ip->ttl - ipv6->hop_limit;
	}

	return 0;
}

/**
 * @brief Add NAT info to the session SI, only L3 and L4 NAT is
 *        handled here, L2 NAT is already handled by the new header
 *        To save HW recipes, we always do NAT for IPv4
 * @param s the session
 */
static s32 __smgr_si_nat_set(struct sess_info *s)
{
	/* if the session is bridged and not tunnel termination,
	 * or if the session is esp (TBD: adding esp NAT support)
	 * no nat/napt is needed
	 */
	if ((__smgr_is_sess_bridged(s) && !__smgr_is_tunn_term_sess(s)) ||
	    (SESS_TX_IS_OUTER_ESP(s) && !__smgr_is_esp_nat_sess(s)) ||
		(__smgr_is_sess_l3_bypass(s)))
		return 0;

	if (__smgr_is_sess_bridged(s) &&
	    __smgr_is_ip_nat_addr_changed(s)) {
		pr_debug("Nat on bridge session isn't supported\n");
		return -EPROTONOSUPPORT;
	}

	/* from that point on we always do L2 header replacement so we clear the
	 * flag to make the modifier recipes bitmaps a bit more simpler
	 */
	SESS_MOD_FLAG_CLR(s, MOD_HDR_RPLC_BIT);

	if (__smgr_is_ipv4_nat(s))
		return __smgr_si_ipv4_nat_set(s);
	else if (__smgr_is_ipv6_nat(s))
		return __smgr_si_ipv6_nat_set(s);
	else if (smgr_is_mapt_nat_sess(s))
		return smgr_si_mapt_nat_set(s);

	pr_err("unsupported l3 protocol ?\n");
	return -EPROTONOSUPPORT;
}

/**
 * @brief Update the ipv4 header template
 * @param sess session
 */
static void smgr_newhdr_ipv4_update(struct sess_info *s)
{
	struct pp_si *si = &s->si;
	struct iphdr *iphdr;

	iphdr = (struct iphdr *)(si->bce.nhdr + si->dpu.nhdr_l3_off);

	iphdr->id       = 0;             /* will be set by the modifier */
	iphdr->tot_len  = 0;             /* will be set by the modifier */
	iphdr->frag_off &= htons(IP_DF); /* preserve only the DF bit    */
	ip_send_check(iphdr);            /* calculate the new checksum  */
	si->sce.nhdr_csum = ntohs(iphdr->check);
	pr_debug("nhdr_csum = 0x%x l3 offset %d\n",
		 si->sce.nhdr_csum, si->dpu.nhdr_l3_off);
}

/**
 * @brief Update the ipv6 header template
 * @param sess session
 */
static void smgr_newhdr_ipv6_update(struct sess_info *s)
{
	struct pp_si *si = &s->si;
	struct ipv6hdr *ip6;

	/* we set the length in the header to zero to detect issues
	 * where modifier recipes don't update the ipv6
	 * length properly and aren't tested properly
	 * by setting the length to zero we make sure the modified
	 * packets will be dropped
	 */
	ip6 = (struct ipv6hdr *)(si->bce.nhdr + si->dpu.nhdr_l3_off);
	ip6->payload_len = 0;
}

/**
 * @brief Set the new header for ESP sessions
 * @note assume that ESP exist on egress outer header
 * @param s the session info
 */
static void __smgr_si_esp_newhdr_set(struct sess_info *s)
{
	struct pp_si_dpu *dpu = &s->si.dpu;
	struct pp_si_sce *sce = &s->si.sce;

	/* tunnel mode outbound encapsulation session, the 1st session */
	if (!SESS_RX_IS_OUTER_ESP(s)) {
		/* no inner IP means this is transport mode,
		 * encapsulating non-tunnels packet (udp, tcp, e.g.).
		 * in this case, we copy from tx everything up to the l4
		 */
		if (!SESS_TX_IS_INNER_IP(s)) {
			dpu->nhdr_sz = pktprs_hdr_sz(
				SESS_TX_PKT(s), PKTPRS_PROTO_ESP, HDR_OUTER);
			/* replace the outer ip header also */
			sce->tot_len_diff =
				dpu->nhdr_sz -
				pktprs_ip_hdr_sz(SESS_TX_PKT(s), HDR_OUTER);
		} else {
			/* for all other cases, we copy from tx everything
			 * up to the inner ip
			 */
			dpu->nhdr_sz =
				pktprs_ip_hdr_off(SESS_TX_PKT(s), HDR_INNER);

			/* tot_len_diff = diff between inner ip and outer ip
			 * offsets
			 */
			sce->tot_len_diff =
				dpu->nhdr_sz -
				pktprs_ip_hdr_off(SESS_TX_PKT(s), HDR_OUTER);
		}
		/* above diff calculation includes the IP header size
		 * as well, for ipv6 the length doesn't include the ip header
		 * size so we remove it
		 */
		if (SESS_TX_IS_OUTER_V6(s))
			sce->tot_len_diff -= sizeof(struct ipv6hdr);

		return;
	}

	/* tunnel mode inbound decapsulation, 1st session */
	if (!SESS_TX_IS_OUTER_IP(s)) {
		dpu->nhdr_sz = pktprs_hdr_off(SESS_TX_PKT(s), PKTPRS_PROTO_ESP,
					      HDR_OUTER);
		return;
	}

	/* tunnel/transport mode outbound encapsulation session, 2nd session
	 * transport mode inbound decapsulation, 1st session
	 * real bypass session where we don't do esp termination
	 */
	dpu->nhdr_sz = pktprs_ip_hdr_off(SESS_TX_PKT(s), HDR_OUTER);
	if (SESS_TX_IS_OUTER_V6(s))
		/* the recipe calculates the payload length
		 * using packet length and l3 offset
		 * so for ipv6 we need to substruct
		 * the ipv6 header length as payload
		 * length should not include it
		 */
		sce->tot_len_diff = sizeof(struct ipv6hdr);
}

/**
 * @brief Set the new header for MAP-T sessions
 * @note currently, this API is implemented to support only when no
 *       other IP tunnel exists, thus, only outer IP header exists
 *       on both ingress and egress packets.
 * @param s the session info
 */
static void smgr_si_mapt_newhdr_set(struct sess_info *s)
{
	struct pp_si_dpu *dpu = &s->si.dpu;
	struct pp_si_sce *sce = &s->si.sce;

	/* In map-t in both US/DS we need to replace the outer IP so new header
	 * includes the outer IP as well,
	 * also, we need to reduce ipv4 header length in US (v4 to v6)
	 * and to add ipv4 header length in DS (v6 to v4) to the total length
	 */
	dpu->nhdr_sz = pktprs_ip_hdr_sz(SESS_TX_PKT(s), HDR_OUTER);
	sce->tot_len_diff = sizeof(struct iphdr);
	/* nhdr_l3_off already pointing to the ipv4/ipv6 header */
	if (SESS_TX_IS_OUTER_V4(s))
		SESS_MOD_FLAG_SET(s, MOD_NHDR_IPV4_BIT);
	else if (SESS_TX_IS_OUTER_V6(s))
		SESS_MOD_FLAG_SET(s, MOD_NHDR_IPV6_BIT);
}

/**
 * @brief Set info required by vxlan session
 * @param s the session
 */
static void smgr_vxlan_newhdr_set(struct sess_info *s)
{
	u16 temp;
	u32 csum32 = 0;
	u16 csum16 = 0;
	struct iphdr *tx_ip;
	struct ipv6hdr *tx_ip6;
	struct pp_si_sce *sce = &s->si.sce;
	u8 offset;
	struct udphdr *udp;
	u8 inner_offset = 0;
	u8 outer_offset;
	u8 csum_bytes_len;

	udp = pktprs_udp_hdr(SESS_TX_PKT(s), HDR_OUTER);
	udp->len = 0;
	udp->check = 0;

	if (SESS_TX_IS_OUTER_V4(s))
		sce->new_l3_hdr_sz = sizeof(struct iphdr);

	if (SESS_MOD_FLAG_TEST(s, MOD_UDP_CSUM_DISABLE_BIT))
		return;

	if (SESS_TX_IS_INNER_V4(s)) {
		inner_offset = pktprs_hdr_off(SESS_TX_PKT(s),
						PKTPRS_PROTO_IPV4, HDR_INNER);
		tx_ip = pktprs_ipv4_hdr(SESS_TX_PKT(s), HDR_INNER);
		csum32 = (u32)csum_tcpudp_magic(tx_ip->saddr, tx_ip->daddr,
				0, 0, 0);
		/* compensate for protocol type and length diff*/
		if (PKTPRS_IS_TCP(SESS_RX_PKT(s), HDR_OUTER)) {
			temp = (pktprs_hdr_off(SESS_TX_PKT(s),
				PKTPRS_PROTO_TCP, HDR_INNER)-
				pktprs_hdr_off(SESS_TX_PKT(s),
					PKTPRS_PROTO_UDP, HDR_OUTER));
			temp += (IPPROTO_UDP -IPPROTO_TCP);
		} else {
			temp = (pktprs_hdr_off(SESS_TX_PKT(s),
				PKTPRS_PROTO_UDP, HDR_INNER)-
				pktprs_hdr_off(SESS_TX_PKT(s),
				PKTPRS_PROTO_UDP, HDR_OUTER));
		}
		csum16 = csum16_add(csum16, htons(temp));
	} else if (SESS_TX_IS_INNER_V6(s)) {
		inner_offset = pktprs_hdr_off(SESS_TX_PKT(s),
						PKTPRS_PROTO_IPV6, HDR_INNER);
		tx_ip6 = pktprs_ipv6_hdr(SESS_TX_PKT(s), HDR_INNER);
		csum32 = csum_add(csum32, *((u32*)tx_ip6));
		if (PKTPRS_IS_TCP(SESS_RX_PKT(s), HDR_OUTER)) {
			temp = (IPPROTO_UDP -IPPROTO_TCP);
			csum16 = csum16_add(csum16, htons(temp));
		}
	}

	outer_offset = pktprs_hdr_off(SESS_TX_PKT(s), PKTPRS_PROTO_UDP,
			HDR_OUTER);
	csum_bytes_len = inner_offset - outer_offset;
	if (SESS_TX_IS_OUTER_V6(s)) {
		csum_bytes_len += sizeof(tx_ip6->saddr) +
				 sizeof(tx_ip6->daddr);
		offset = pktprs_hdr_off(SESS_TX_PKT(s), PKTPRS_PROTO_IPV6,
			HDR_OUTER) + offsetof(struct ipv6hdr, saddr);
	} else {
		csum_bytes_len += sizeof(tx_ip->saddr) +
				 sizeof(tx_ip->daddr);
		offset = pktprs_hdr_off(SESS_TX_PKT(s), PKTPRS_PROTO_IPV4,
			HDR_OUTER) + offsetof(struct iphdr, saddr);
	}

	if (offset < PKTPRS_HDR_BUF_SIZE) {
		csum32 = csum_partial(&SESS_TX_PKT(s)->buf[offset],
						csum_bytes_len, csum32);
		csum32 = csum_add(csum32, (u32)csum16);
		csum32 = csum_fold(csum32);
		sce->new_l4_csum  = htons((u16)csum32);
	}
}

/**
 * @brief Update UDP tunnel new header info
 * @note this function changes ONLY the SI new header buffer, it doesn't
 *       change the packet parse packets
 * @param s the session
 */
static void smgr_udp_newhdr_update(struct sess_info *s)
{
	struct pp_si *si = &s->si;
	struct l2tp_v2_len_hdr *l2tp;
	struct iphdr *ip;
	struct ipv6hdr *ip6, *ip6nhdr;
	struct udphdr *udp;
	bool do_udp_csum;
	__sum16 csum;
	u8 l2tp_off;

	si->dpu.nhdr_l4_off =
		pktprs_hdr_off(SESS_TX_PKT(s), PKTPRS_PROTO_UDP, HDR_OUTER);
	/* calculate the l3 header size to be reduced from the new header
	 * l3 length for the new header udp length calculation
	 */
	si->sce.new_l3_hdr_sz = si->dpu.nhdr_l4_off - si->dpu.nhdr_l3_off;
	if (SESS_TX_IS_OUTER_V6(s))
		si->sce.new_l3_hdr_sz -= sizeof(struct ipv6hdr);

	/* Update the UDP header with a zero length so it will be updated
	 * by modifier recipe per packet
	 */

	udp = (struct udphdr *)(si->bce.nhdr + si->dpu.nhdr_l4_off);
	do_udp_csum = !!(udp->check);
	if (do_udp_csum) {
		csum = udp->check;
		csum_replace2(&csum, udp->len, 0);
		udp->check = csum;
	}
	udp->len = 0;

	/* from here, we update the outer udp checksum according to all the
	 * inner layers per packet changes, e.g. TTL, length
	 */
	if (SESS_MOD_FLAG_TEST(s, MOD_NHDR_L2TP_BIT)) {
		/* clear L2TP header length to make sure it is being updated
		 * per packet correctly by the modifier recipe
		 */
		l2tp_off = pktprs_hdr_off(SESS_TX_PKT(s),
					  PKTPRS_PROTO_L2TP_OUDP, HDR_OUTER);
		l2tp = (struct l2tp_v2_len_hdr *)(si->bce.nhdr + l2tp_off);
		ip = pktprs_ipv4_hdr(SESS_TX_PKT(s), HDR_INNER);
		si->sce.l2tp_diff = l2tp->length - ip->tot_len;
		if (do_udp_csum)
			csum_replace2(&csum, l2tp->length, 0);
		l2tp->length = 0;
	}

	/* if no udp checksum update is needed, we are done */
	if (!do_udp_csum)
		return;

	if (SESS_TX_IS_INNER_V6(s)) {
		ip6 = pktprs_ipv6_hdr(SESS_TX_PKT(s), HDR_OUTER);
		ip6nhdr =
			(struct ipv6hdr *)(si->bce.nhdr + si->dpu.nhdr_l3_off);
		csum_replace2(&csum, ip6->payload_len, ip6nhdr->payload_len);
		/* for IPv4 it is not needed cause it has its own
		 * checksum, and every inner protocol with own checksum
		 * can be calculated as zero including everything his
		 * own checksum includes
		 * in IPv4 case its only the IPv4 header
		 */
	}

	si->sce.new_l4_csum = ntohs(csum);
}

/**
 * @brief Set the new header info in the session si
 * @param s the session
 */
static void __smgr_si_newhdr_set(struct sess_info *s)
{
	struct pp_si_bce *bce = &s->si.bce;
	struct pp_si_dpu *dpu = &s->si.dpu;
	struct pp_si_sce *sce = &s->si.sce;

	if (SESS_TX_IS_OUTER_IP(s))
		dpu->nhdr_l3_off = pktprs_ip_hdr_off(SESS_TX_PKT(s), HDR_OUTER);
	else if (__smgr_is_eompls_encp_sess(s))
		dpu->nhdr_l3_off = pktprs_ip_hdr_off(SESS_TX_PKT(s), HDR_INNER);
	else
		dpu->nhdr_l3_off = pktprs_hdr_off(
			SESS_TX_PKT(s), PKTPRS_PROTO_PAYLOAD, HDR_OUTER);

	s->si.pkt_len_diff = SESS_TX_PKT(s)->buf_sz - SESS_RX_PKT(s)->buf_sz;
	sce->tot_len_diff = 0;

	/* IPv6 fragments contains frag header extension option,
	 * it should be removed/added in reassembly/fragmantation sessions.
	 * but it is being done by the FW and not by the session(modifier HW)
	 * so it should not be reflected in the session.
	 */
	if (SESS_RX_IS_OUTER_V6(s)) {
		if (is_frag_pkt(s, HDR_OUTER))
			s->si.pkt_len_diff -= sizeof(struct frag_hdr);
		else if (is_reass_pkt(s, HDR_OUTER))
			s->si.pkt_len_diff += sizeof(struct frag_hdr);

		sce->tot_len_diff += sizeof(struct ipv6hdr);
	}

	if (__smgr_is_iptun_encp_sess(s)) {
		/* ip tunnel encapsulation session */
		/* nhdr size is the tx header up to the inner ip offset */
		dpu->nhdr_sz = pktprs_ip_hdr_off(SESS_TX_PKT(s), HDR_INNER);
		sce->tot_len_diff += dpu->nhdr_sz - dpu->nhdr_l3_off;

		if (SESS_TX_IS_OUTER_V6(s))
			sce->tot_len_diff -= sizeof(struct ipv6hdr);
	} else if (SESS_TX_IS_OUTER_ESP(s) &&
			!__smgr_is_sess_l3_bypass(s) &&
			!__smgr_is_iptun_decp_sess(s)) {
		__smgr_si_esp_newhdr_set(s);
	} else if (smgr_is_mapt_nat_sess(s)) {
		smgr_si_mapt_newhdr_set(s);
	} else if (__smgr_is_outer_l2_changed(s)) {
		/* Bypass with L2 changes, routing and tunnel decapsulation sessions */
		dpu->nhdr_sz = dpu->nhdr_l3_off;
	} else {
		/* no modification session */
		dpu->nhdr_sz = 0;
		dpu->nhdr_l3_off = 0;
		sce->tot_len_diff = 0;
	}

	/* if no new header, done */
	if (!dpu->nhdr_sz)
		return;

	/* copy the new header to SI bce region */
	dpu->nhdr_sz = min3(dpu->nhdr_sz, SESS_TX_PKT(s)->buf_sz,
			    (u8)sizeof(bce->nhdr));

	if (SESS_MOD_FLAG_TEST(s, MOD_UDP_ENCP_BIT))
		smgr_vxlan_newhdr_set(s);

	memcpy(bce->nhdr, SESS_TX_PKT(s)->buf, dpu->nhdr_sz);

	/* if new header including ipv4, update the ip layer */
	if (dpu->nhdr_sz > dpu->nhdr_l3_off) {
		if (SESS_TX_IS_OUTER_V4(s))
			smgr_newhdr_ipv4_update(s);
		else if (SESS_TX_IS_OUTER_V6(s) &&
			 (SESS_MOD_FLAG_TEST(s, MOD_NHDR_UDP_BIT) ||
			  smgr_is_mapt_nat_sess(s)))
			/* this should be done for every tunnel with outer
			 * IPv6 header.
			 * but it is a bit risky w/o proper regression tests.
			 * So we do it only for L2TP and MAP-T for now.
			 */
			smgr_newhdr_ipv6_update(s);
	}

	if (SESS_MOD_FLAG_TEST(s, MOD_NHDR_UDP_BIT))
		smgr_udp_newhdr_update(s);
}

/**
 * @brief Set the LRO info in SI UD
 * @param s the session
 */
static s32 __smgr_lro_si_ud_set(struct sess_info *s)
{
	struct pp_lro_ps ps;
	bool hdr_lvl;
	u16 off;

	if (test_bit(PP_SESS_FLAG_PS_VALID_BIT, &s->args->flags)) {
		/* Should not happen, will be overwitten */
		pr_debug("User PS is valid\n");
	}

	s->si.tmpl_ud_sz = PP_TEMPLATE_UD_SZ;
	/* set the paylod off and the l3 offset, rest will be set by the hw/fw */
	hdr_lvl = HDR_OUTER;
	if (SESS_TX_IS_INNER_TCP(s))
		hdr_lvl = HDR_INNER;

	ps.tcp_seq = 0;
	off = pktprs_ip_hdr_off(SESS_TX_PKT(s), hdr_lvl);
	ps.l3_off = off & 0xFF;
	off = pktprs_hdr_sz(SESS_TX_PKT(s), PKTPRS_PROTO_TCP, hdr_lvl);
	ps.pl_off = off & 0x7F;
	ps.ip_ver = SESS_RX_IS_OUTER_V4(s);
	ps.fid = s->si.tdox_flow & 0x7F;

	set_bit(PP_SESS_FLAG_PS_VALID_BIT, &s->args->flags);
	s->si.si_ps_sz = PP_PS_REGION_SZ;
	s->si.ps_off = 0;
	memcpy(s->si.ud, &ps, sizeof(ps));

	return 0;
}

/**
 * @brief set remark_info struct values
 * @param si Session information
 * @param remark struct to update
 * @return 0 on Success, error code otherwise.
 */
static s32 __smgr_remark_set(struct sess_info *s, struct remark_info *remark)
{
	struct pp_nf_info nf_info;
	u16 phys_q;
	s32 ret = 0;

	if (!PKTPRS_IS_IP(SESS_TX_PKT(s), HDR_OUTER)) {
		pr_debug("supporting remarking only with ip protocol\n");
		return -EINVAL;
	}

	if (!s->si.tbm_en_map) {
		pr_debug("No tbm is enabled with dscp remarking session\n");
		return -EINVAL;
	}

	if (PKTPRS_IS_IPV4(SESS_TX_PKT(s), HDR_OUTER))
		remark->flags |= REMARK_INFO_FLAG_OUT_IPV4;

	if (PKTPRS_IS_IP(SESS_TX_PKT(s), HDR_INNER)) {
		remark->flags |= REMARK_INFO_FLAG_INNER_IP;
		if (PKTPRS_IS_IPV4(SESS_TX_PKT(s), HDR_INNER))
			remark->flags |= REMARK_INFO_FLAG_INNER_IPV4;
	}

	remark->dscp = s->args->remark_dscp;
	remark->out_l3_off = pktprs_ip_hdr_off(SESS_TX_PKT(s), HDR_OUTER);
	remark->innr_l3_off = pktprs_ip_hdr_off(SESS_TX_PKT(s), HDR_INNER);

	ret = pp_nf_get(PP_NF_REMARKING, &nf_info);
	if (unlikely(ret)) {
		pr_err("pp_nf_get err %d\n", ret);
		return ret;
	}
	ret = smgr_get_queue_phy_id(nf_info.q, &phys_q);
	if (unlikely(ret)) {
		pr_err("smgr_get_queue_phy_id err %d\n", nf_info.q);
		return ret;
	}
	s->si.dst_q = phys_q;

	pr_debug("flags %#x, dscp %d, out_l3_off %d, innr_l3_off %d\n",
		 remark->flags, remark->dscp, remark->out_l3_off,
		 remark->innr_l3_off);

	return ret;
}

/**
 * @brief Set the frag info in SI UD
 * @param s the session
 */
static s32 __smgr_frag_si_ud_set(struct sess_info *s)
{
	struct si_ud_frag_remark_info info = { 0 };
	struct pp_nf_info nf_info;
	struct smgr_database *db = smgr_get_db();
	u16 phys_q;
	s32 ret = 0;

	if (ptr_is_null(db))
		return -EINVAL;

	s->si.tmpl_ud_sz = PP_TEMPLATE_UD_SZ;
	info.dst_q = s->si.dst_q;
	info.frag.max_pkt_size = s->eg_port_cfg.tx.max_pkt_size;

	/* Set relevant flag bits for the uc */
	if (test_bit(PP_SESS_FLAG_IGNORE_INT_DFRAG_BIT, &s->args->flags))
		info.frag.flags |= FRAG_INFO_FLAG_IGNORE_INT_DF;
	if (test_bit(PP_SESS_FLAG_IGNORE_EXT_DFRAG_BIT, &s->args->flags))
		info.frag.flags |= FRAG_INFO_FLAG_IGNORE_EXT_DF;
	if (!pktprs_first_frag(SESS_TX_PKT(s), HDR_INNER))
		info.frag.flags |= FRAG_INFO_FLAG_FRAG_EXT;

	/* Update l3 offset.
	 * In case of inner frag, take the offset from the learning
	 */
	if (info.frag.flags & FRAG_INFO_FLAG_FRAG_EXT) {
		info.frag.l3_off = s->si.dpu.nhdr_l3_off;
		if (SESS_TX_IS_OUTER_V4(s)) {
			info.frag.flags |= FRAG_INFO_FLAG_IPV4;
			if (pktprs_ipv4_hdr(SESS_TX_PKT(s),
					    HDR_OUTER)->frag_off &
					    htons(IP_DF))
				info.frag.flags |= FRAG_INFO_FLAG_EXT_DF;
		}
	} else {
		info.frag.l3_off = pktprs_ip_hdr_off(SESS_TX_PKT(s), HDR_INNER);
		if (SESS_TX_IS_INNER_V4(s)) {
			info.frag.flags |= FRAG_INFO_FLAG_IPV4;
			if (pktprs_ipv4_hdr(SESS_TX_PKT(s),
					    HDR_INNER)->frag_off &
					    htons(IP_DF))
				info.frag.flags |= FRAG_INFO_FLAG_INT_DF;
		}
	}

	if (__smgr_is_pppoe_encap(s))
		info.frag.flags |= FRAG_INFO_FLAG_PPPOE;

	if (db->frag_mode == SMGR_FRAG_MODE_CHECK_MTU_IN_UC) {
		ret = pp_nf_get(PP_NF_FRAGMENTER, &nf_info);
		if (unlikely(ret)) {
			pr_err("pp_nf_get err %d\n", ret);
			return ret;
		}
		ret = smgr_get_queue_phy_id(nf_info.q, &phys_q);
		if (unlikely(ret)) {
			pr_err("smgr_get_queue_phy_id err %d\n", nf_info.q);
			return ret;
		}
		s->si.dst_q = phys_q;
	} else if (SESS_IS_FLAG_ON(s->db_ent, SESS_FLAG_REMARK)) {
		if (!__smgr_remark_set(s, &info.remark))
			info.frag.flags |= FRAG_INFO_FLAG_REMARK;
	}

	pr_debug("flags %#x, q %d (%d), max_pkt_size %d, l3_off %d\n",
		 info.frag.flags, info.dst_q, s->si.dst_q,
		 info.frag.max_pkt_size, info.frag.l3_off);

	memcpy(s->si.ud + PP_PS_REGION_SZ, &info, sizeof(info));
	s->si.si_ud_sz = PP_PS_REGION_SZ + sizeof(info);

	return ret;
}

/**
 * @brief Set the remark info in SI UD
 * @param s the session
 */
static s32 __smgr_remark_si_ud_set(struct sess_info *s)
{
	struct si_ud_frag_remark_info info = { 0 };
	struct smgr_database *db = smgr_get_db();
	s32 ret = 0;

	if (ptr_is_null(db))
		return -EINVAL;

	s->si.tmpl_ud_sz = PP_TEMPLATE_UD_SZ;
	info.dst_q = s->si.dst_q;
	/* if remark is failed, creating session without remarking */
	if (__smgr_remark_set(s, &info.remark))
		return 0;

	pr_debug("q %d (%d)\n", info.dst_q, s->si.dst_q);

	memcpy(s->si.ud + PP_PS_REGION_SZ, &info, sizeof(info));
	s->si.si_ud_sz = PP_PS_REGION_SZ + sizeof(info);

	return ret;
}

/**
 * @brief Set the LLD info in SI UD or AQM for AQM_SW
 * @param s the session
 */
static s32 __smgr_aqm_lld_si_ud_set(struct sess_info *s)
{
	struct si_ud_aqm_lld_info info = { 0 };
	struct pp_qos_queue_info q_info;
	struct pp_qos_dev *qdev;
	struct pp_nf_info nf_info;
	enum pktprs_hdr_level lvl = HDR_OUTER;
	u8 lld_ctx;
	u16 coupled_queue;
	u16 phys_q;
	u16 sf_indx;
	s32 ret = 0;

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(ptr_is_null(qdev)))
		return -EINVAL;

	/* handle ud in case packet is lld type - queues & flags */
	if (SESS_IS_FLAG_ON(s->db_ent, SESS_FLAG_LLD)) {
		ret = pp_misc_get_lld_info_by_q(s->si.dst_q, &lld_ctx,
						&coupled_queue);
		if (unlikely(ret)) {
			pr_err("Failed to get lld info\n");
			return ret;
		}

		if (lld_ctx == PP_MAX_ASF || coupled_queue == PP_QOS_INVALID_ID) {
			pr_err("failure on lld_ctx %d or coupled_queue %d\n",
				lld_ctx, coupled_queue);
			return -EINVAL;
		}

		ret = pp_qos_queue_info_get(qdev, coupled_queue, &q_info);
		if (unlikely(ret)) {
			pr_err("Failed to get queue %u info\n", coupled_queue);
			return ret;
		}
		info.dst_cq = q_info.physical_id;
		info.lld_ctx_id = lld_ctx;

		if (PKTPRS_IS_MULTI_LEVEL(SESS_TX_PKT(s)))
			lvl = HDR_INNER;

		if (!PKTPRS_IS_IP(SESS_TX_PKT(s), lvl)) {
			pr_err("pkt is not IP\n");
			return -EINVAL;
		}

		if (PKTPRS_IS_IPV4(SESS_TX_PKT(s), lvl))
			info.flags |= AQM_LLD_INFO_FLAG_OUT_IPV4;

		info.l3_off = pktprs_ip_hdr_off(SESS_TX_PKT(s), lvl);

		info.flags |= AQM_LLD_INFO_FLAG_LLD_PKT;
	}
	/* set flag in case AQM enabled */
	if (SESS_IS_FLAG_ON(s->db_ent, SESS_FLAG_AQM_SW))
		info.flags |= AQM_LLD_INFO_FLAG_AQM_SW;

	/* Rest are common for both lld and classic aqm packets */
	if (pp_misc_get_sf_indx_by_q(s->args->dst_q, &sf_indx)) {
		pr_err("Failed to get sf index\n");
		return -EINVAL;
	}

	ret = pp_nf_get(PP_NF_IPSEC_LLD, &nf_info);
	if (unlikely(ret)) {
		pr_err("pp_nf_get err %d\n", ret);
		return ret;
	}

	ret = smgr_get_queue_phy_id(nf_info.q, &phys_q);
	if (unlikely(ret)) {
		pr_err("smgr_get_queue_phy_id err %d\n", nf_info.q);
		return ret;
	}

	s->si.tmpl_ud_sz = PP_TEMPLATE_UD_SZ;
	info.dst_q = s->si.dst_q; /* save original dest q */
	s->si.dst_q = phys_q;     /* set nf queue */
	info.sf_id = sf_indx;

	pr_debug("q %u (%u) cq %u sf_id %u lld_ctx %u l3_off %u flags %u\n",
		 info.dst_q, s->si.dst_q, info.dst_cq, info.sf_id,
		 info.lld_ctx_id, info.l3_off, info.flags);

	memcpy(s->si.ud + PP_PS_REGION_SZ, &info, sizeof(info));
	s->si.si_ud_sz = PP_PS_REGION_SZ + sizeof(info);

	return ret;
}

static s32 __smgr_reass_si_ud_set(struct sess_info *s)
{
	struct si_ud_reass_info reass_info = {0};
	bool inner_reass, outer_reass;

	s->si.tmpl_ud_sz = PP_TEMPLATE_UD_SZ;
	s->si.ps_copy = true;

	reass_info.eg_port_hr = s->eg_port_cfg.tx.headroom_size;
	reass_info.eg_port_tr = s->eg_port_cfg.tx.tailroom_size;
	reass_info.base_policy = (u8)s->eg_port_cfg.tx.base_policy;
	reass_info.policies_map = s->eg_port_cfg.tx.policies_map;
	reass_info.flags = 0;
	if (s->eg_port_cfg.tx.wr_desc)
		reass_info.flags |= BUFF_EX_FLAG_WR_DESC;
	if (s->eg_port_cfg.tx.prel2_en)
		reass_info.flags |= BUFF_EX_FLAG_PREL2_EN;

	outer_reass = is_reass_pkt(s, HDR_OUTER) ||
		      SESS_ARGS_IS_FLAG_ON(s, PP_SESS_FLAG_OUTER_REASS_FORCE);
	inner_reass = is_reass_pkt(s, HDR_INNER);
	/* align the reassembly flags to the ones set by
	 * the parser in the UD
	 * reminder: the parser first set the inner flags
	 */
	if (inner_reass || (outer_reass && !SESS_RX_IS_INNER_IP(s)))
		reass_info.flags |= REASS_INFO_FLAG_INT_REASS;

	if (outer_reass)
		reass_info.flags |= REASS_INFO_FLAG_EXT_REASS;

	if (s->si.sgc_en_map)
		reass_info.flags |= REASS_INFO_FLAG_SGC_VALID;

	memcpy(s->si.ud + PP_PS_REGION_SZ, &reass_info, sizeof(reass_info));
	s->si.si_ud_sz = PP_PS_REGION_SZ + sizeof(reass_info);

	return 0;
}

/**
 * @brief Set the UD info into the session si
 * @param s the session
 */
static s32 __smgr_si_ud_set(struct sess_info *s)
{
	s32 ret = 0;

	/* set the PS parameters */
	if (test_bit(PP_SESS_FLAG_PS_VALID_BIT, &s->args->flags)) {
		s->si.si_ps_sz = PP_PS_REGION_SZ;
		s->si.ps_off = 0;
		memcpy(s->si.ud, &s->args->ps, sizeof(s->args->ps));
	} else {
		s->si.si_ps_sz = 0;
		s->si.ps_off = PP_INVALID_PS_OFF;
	}

	/* set the PS copy bit if needed */
	s->si.ps_copy = test_bit(PP_SESS_FLAG_PS_COPY_BIT, &s->args->flags);
	/* set the tmpl ud size */
	s->si.tmpl_ud_sz = s->args->tmp_ud_sz;

	if (!SESS_RX_PKT(s) || !SESS_TX_PKT(s))
		goto out;

	/* if this is a session for fragments, make sure UD will be copied
	 * for the egress UC, otherwise, use the user arguments
	 */
	if (__smgr_is_rx_first_frag(s) ||
	    SESS_ARGS_IS_FLAG_ON(s, PP_SESS_FLAG_OUTER_REASS_FORCE)) {
		/* reassembly or frag bypass */
		ret = __smgr_reass_si_ud_set(s);
		if (unlikely(ret))
			return ret;
	} else if (SESS_IS_FLAG_ON(s->db_ent, SESS_FLAG_MTU_CHCK)) {
		/* fragmentation */
		if (SESS_IS_FLAG_ON(s->db_ent, SESS_FLAG_LLD))
		    pr_debug("frag is enabled. Ignoring LLD\n");
		ret = __smgr_frag_si_ud_set(s);
		if (unlikely(ret))
			return ret;
	} else if (SESS_IS_FLAG_ON(s->db_ent, SESS_FLAG_LLD) ||
		SESS_IS_FLAG_ON(s->db_ent, SESS_FLAG_AQM_SW)) {
		pr_debug("LLD or SW AQM is enabled\n");
		ret = __smgr_aqm_lld_si_ud_set(s);
		if (unlikely(ret))
			return ret;
	} else if (SESS_IS_FLAG_ON(s->db_ent, SESS_FLAG_TDOX_SUPP)) {
		s->si.tmpl_ud_sz = PP_TEMPLATE_UD_SZ;
	} else if (SESS_IS_FLAG_ON(s->db_ent, SESS_FLAG_REMARK)) {
		/* remarking can work alone or with frag NF only */
		if (SESS_IS_FLAG_ON(s->db_ent, SESS_FLAG_LLD))
		    pr_debug("Remark is enabled. Ignoring LLD\n");
		ret = __smgr_remark_si_ud_set(s);
		if (unlikely(ret))
			return ret;
	} else if (SESS_IS_FLAG_ON(s->db_ent, SESS_FLAG_LRO)) {
		ret = __smgr_lro_si_ud_set(s);
		if (unlikely(ret))
			return ret;
	}

out:
	/* in case of seg, UD is not allowed, clear the seg_en */
	if (s->si.seg_en && (s->si.si_ud_sz || s->args->tmp_ud_sz)) {
		pr_debug("seg is not allowed when UD is required\n");
		s->si.seg_en = 0;
	}

	return 0;
}

/**
 * @brief Set flags in the session si
 * @param s the session
 */
static void __smgr_si_flags_set(struct sess_info *s)
{
	ulong chck_flags = SESS_CHCK_DFLT_FLAGS;
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	if (pp_misc_is_nf_en(PP_NF_REASSEMBLY))
		set_bit(SI_CHCK_FLAG_DVRT_FRAG, &chck_flags);

	if (!SESS_RX_PKT(s) || !SESS_TX_PKT(s))
		goto set_checker_flags;

	if (test_bit(SESS_FLAG_ROUTED, &s->db_ent->info.flags))
		set_bit(SI_CHCK_FLAG_DVRT_TTL_EXP, &chck_flags);

	if (test_bit(SESS_FLAG_MTU_CHCK, &s->db_ent->info.flags)) {
		if (db->frag_mode == SMGR_FRAG_MODE_CHECK_MTU_IN_CHECKER)
			set_bit(SI_CHCK_FLAG_DVRT_MTU_OVFL, &chck_flags);
	}

	if (test_bit(SESS_FLAG_LRO, &s->db_ent->info.flags))
		set_bit(SI_CHCK_FLAG_SEND2FW, &chck_flags);

set_checker_flags:
	/* save flags in the si */
	s->si.chck_flags = (u16)chck_flags;
}

/**
 * @brief Set session's PPPoE header modification info
 * @param s the session
 */
static void __smgr_pppoe_mod_set(struct sess_info *s)
{
	struct pp_si_sce *sce = &s->si.sce;
	struct pp_si_dpu *dpu = &s->si.dpu;

	sce->valid = true;

	/* enable pppoe length fix only in case we add pppoe header (encap)
	 * or if this is bypass but the pppoe got changed somehow
	 * for esp case, we only enable it on the bypass sessions cause
	 * only in these session we can fix it
	 */
	if ((__smgr_is_pppoe_outer_encap(s) && !__smgr_is_esp_encp_sess(s)) ||
	    (__smgr_is_pppoe_bypass(s) &&
	    (__smgr_is_outer_l2_changed(s) || __smgr_is_esp_bypass_sess(s)))) {
		/* pppoe encapsulation, set the sce parameters so that
		 * the recipe will calculate the pppoe header length and
		 * will update the new header accordingly
		 */
		sce->pppoe_diff = 2; /* for the eth last protocol */
		sce->l2_org_val = 0;
		/* in case we have ipv6 we must add
		 * the ipv6 header length also to the pppoe_diff
		 * cause its length field doesn't include it
		 */
		if (SESS_TX_IS_OUTER_V6(s)) {
			if (__smgr_is_iptun_encp_sess(s))
				sce->pppoe_diff += sizeof(struct ipv6hdr);
			dpu->lyr_fld_off =
				offsetof(struct ipv6hdr, payload_len);
		} else {
			dpu->lyr_fld_off = offsetof(struct iphdr, tot_len);
		}
		/* set the pppoe header offset */
		dpu->pppoe_off = pktprs_hdr_off(SESS_TX_PKT(s),
						PKTPRS_PROTO_PPPOE,
						HDR_OUTER);
		return;
	}

	if (__smgr_is_pppoe_inner_encap(s)) {
		/* pppoe encapsulation, set the sce parameters so that
		 * the recipe will calculate the pppoe header length and
		 * will update the new header accordingly
		 */
		sce->pppoe_diff = 2; /* for the eth last protocol */
		sce->l2_org_val = 0;
		dpu->pppoe_off = pktprs_hdr_off(SESS_TX_PKT(s),
						PKTPRS_PROTO_PPPOE,
						HDR_INNER);
		return;
	}

	/* not pppoe encapsulation or bypass, to reduce number of recipes, we
	 * always do the pppoe length calculations, but when we don't
	 * have pppoe encapsulation, we need to set the right values
	 * so the recipe will overwrite the new header with the same
	 * value so it will remain unchanged.
	 * we read the first 2 bytes of the l2 and rewrite them
	 */
	dpu->pppoe_off  = 0;
	sce->pppoe_diff = 0;
	sce->l2_org_val =
		be16_to_cpup((__be16 *)(SESS_TX_PKT(s)->buf + dpu->pppoe_off +
					offsetof(struct pppoe_hdr, length)));
}

/**
 * @brief Set session's L2 modification info
 * @param s the session
 */
static void __smgr_si_l2_mod_set(struct sess_info *s)
{
	/* turn on l2 replace flag only if l3 and l4 are off */
	if (!SESS_MOD_FLAG_TEST(s, MOD_HDR_L3_RPLC_BIT) &&
	    !SESS_MOD_FLAG_TEST(s, MOD_HDR_L4_RPLC_BIT)) {
		if (__smgr_is_outer_l2_changed(s) ||
			(!__smgr_is_sess_l3_bypass(s) && SESS_TX_IS_OUTER_ESP(s)))
			SESS_MOD_FLAG_SET(s, MOD_HDR_RPLC_BIT);
	}

	if (__smgr_is_pppoe_changed(s, HDR_INNER))
		SESS_MOD_FLAG_SET(s, MOD_INNER_PPPOE_BIT);

	__smgr_pppoe_mod_set(s);
}

/**
 * @brief Set SI's modification info
 * @param s the session
 */
static s32 __smgr_si_mod_info_set(struct sess_info *s)
{
	s32 ret;

	if (!(SESS_RX_PKT(s) && SESS_TX_PKT(s)))
		return 0;

	if (!__smgr_is_sess_bridged(s) && __smgr_is_frag_accl_sess(s)) {
		pr_debug("Fragments acceleration supported only for bridge sessions\n");
		return -EPROTONOSUPPORT;
	}

	__smgr_si_l2_mod_set(s);
	ret = __smgr_si_nat_set(s);
	if (unlikely(ret))
		return ret;
	__smgr_si_newhdr_set(s);

	return ret;
}

/**
 * @brief __smgr_si_df_mask_set() - Sets dont frag mask info
 * @param si Session information
 * @param: User flags
 *
 * The function configures the DF mask bits according to the user input.
 * In frag mode checker, we want to mask the internal DF,
 * so that the checker will divert the packets to the uc anyway.
 * In that case, fragmenter uc can check where to fragment based on
 * the learning output.
 * The real value from the user is copied to the SI UD in both modes
 *
 * @return 0 on Success.
 */
static s32 __smgr_si_df_mask_set(struct pp_si *si, unsigned long flags)
{
	u32 int_df_mask = 1;
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return -EINVAL;

	si->ext_df_mask = test_bit(PP_SESS_FLAG_IGNORE_EXT_DFRAG_BIT, &flags);

	if (db->frag_mode == SMGR_FRAG_MODE_CHECK_MTU_IN_UC) {
		int_df_mask = test_bit(PP_SESS_FLAG_IGNORE_INT_DFRAG_BIT,
				       &flags);
	}

	si->int_df_mask = int_df_mask;

	return 0;
}

/**
 * @brief Create an si structure for the HW based on the specified
 *        session
 * @param s the session
 * @return s32 0 on success, error code otherwise
 */
static s32 __smgr_si_create(struct sess_info *s)
{
	struct pp_sess_create_args *args;
	struct pp_si *si;
	s32 ret;

	args = s->args;
	si   = &s->si;

	/* set session id first, cause some of the below APIs use it */
	si->sess_id = s->db_ent->info.sess_id;
	ret = smgr_get_queue_phy_id(s->args->dst_q, &s->si.dst_q);
	if (unlikely(ret))
		goto done;
	ret = pp_si_sgc_info_set(args->sgc, ARRAY_SIZE(args->sgc), si);
	if (unlikely(ret))
		goto done;
	ret = pp_si_tbm_info_set(args->tbm, ARRAY_SIZE(args->tbm), si);
	if (unlikely(ret))
		goto done;
	ret = pp_si_port_info_set(args->in_port, &s->in_port_cfg, args->eg_port,
				  &s->eg_port_cfg, &s->si);
	if (unlikely(ret))
		goto done;
	ret = __smgr_si_mod_info_set(s);
	if (unlikely(ret))
		goto done;
	ret = __smgr_si_df_mask_set(si, args->flags);
	if (unlikely(ret))
		goto done;

	si->color      = args->color;
	si->fsqm_prio  = args->fsqm_prio;
	si->bce_ext    = false; /* not supported in PPv4 */
	si->trim_l3_id = SI_NO_TRIM_VALUE;

	smgr_si_tdox_set(s);
	__smgr_si_flags_set(s);

	ret = __smgr_si_ud_set(s);
	if (unlikely(ret))
		goto done;

	ret = mod_recipe_select(s->db_ent->info.mod_flags, &s->si.recipe_idx);
	if (unlikely(ret))
		goto done;

	ret = pp_si_encode(&s->hw_si, &s->si);
done:
	if (unlikely(ret)) {
		pr_debug("failed to create si\n");
		__smgr_stats_sess_si_create_err(s);
	}
	return ret;
}

/**
 * @brief Track session in relevant bitmaps and lists
 * @param s session to track
 */
static inline void __smgr_sess_track(struct sess_info *s)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	set_bit(s->db_ent->info.sess_id, db->open_bmap);
	DB_IN_PORT_LIST_ADD_NODE(db, s->db_ent);
	DB_EG_PORT_LIST_ADD_NODE(db, s->db_ent);
	smgr_mcast_sess_track(s);
}

/**
 * @brief Create a PP session, in case the session will be created,
 *        'sess' structure will be filled with all the session
 *        information like session id, db entry, si and so on.
 * @param s session info
 * @return s32 -EEXIST in case session already exist, 0 on success
 *         creation, error code otherwise
 */
static s32 __smgr_sess_create(struct sess_info *s)
{
	s32 ret = 0;

	ret = pp_port_get(s->args->eg_port, &s->eg_port_cfg);
	if (unlikely(ret)) {
		pr_err("Failed to get port%u info, error %d\n",
		       s->args->eg_port, ret);
		goto err;
	}

	ret = pp_port_get(s->args->in_port, &s->in_port_cfg);
	if (unlikely(ret)) {
		pr_err("Failed to get port%u info, error %d\n",
		       s->args->in_port, ret);
		goto err;
	}

	if (unlikely(!__smgr_is_sess_args_valid(s))) {
		ret = -EINVAL;
		goto err;
	}

	if (unlikely(!__smgr_is_sess_supported(s))) {
		ret = -EPROTONOSUPPORT;
		goto err;
	}

	__smgr_lock();
	ret = __smgr_sess_lookup(s);
	if (ret == -EEXIST)
		goto unlock;
	else if (unlikely(ret))
		goto err_unlock;

	ret = __smgr_sess_ent_prepare(s);
	if (unlikely(ret))
		goto err_unlock;

	__smgr_nat_info_init(s);
	__smgr_sess_flags_set(s);

	ret = __smgr_si_create(s);
	if (unlikely(ret))
		goto err_unlock;

	smgr_sq_alloc(s);
	ret = cls_session_add(&s->hw_si, &s->args->hash);
	if (unlikely(ret)) {
		pr_debug("failed to create session in HW\n");
		if (-ENOSPC == ret)
			__smgr_stats_hw_table_full();
		else
			__smgr_stats_hal_err();
		goto err_unlock;
	}
	smgr_sq_start(s);
	smgr_tdox_start(s);

	pr_debug("session %u created\n", s->db_ent->info.sess_id);
	__smgr_stats_sess_created(s);

	__smgr_sess_track(s);

	__smgr_sess_debug(s);
unlock:
	__smgr_unlock();
	return ret;

err_unlock:
	/* if we already allocated a session entry, free it */
	if (s->db_ent) {
		__smgr_sess_ent_free(s->db_ent);
		s->db_ent = NULL;
	}
	__smgr_unlock();
err:
	pr_debug("session create failed, ret = %d\n", ret);
	if (ret == -EPROTONOSUPPORT)
		__smgr_stats_sess_no_supported(s);
	__smgr_stats_sess_cr_failed(s);
	return ret;
}

s32 __smgr_session_update(u32 sess_id, struct pp_hw_si *hw_si)
{
	struct sess_db_entry *ent;
	struct sess_db_info *info;
	s32 ret = 0;

	pr_debug("updating session %u\n", sess_id);

	if (unlikely(!__smgr_is_sess_exist(sess_id))) {
		__smgr_stats_invalid_args();
		ret = -ENOENT;
		goto done;
	}

	ent = __smgr_get_sess_db_ent(sess_id);
	if (unlikely(!ent)) {
		pr_crit("Session exist with DB no entry!!!\n");
		ret = -ENOENT;
		goto done;
	}
	info = &ent->info;

	/* Update session in classifier */
	ret = cls_session_update(sess_id, info->fv_sz, &info->hash, hw_si);
	if (unlikely(ret)) {
		__smgr_stats_hal_err();
		pr_err("update session %u from HW failed\n", sess_id);
		goto done;
	}

	pr_debug("session %u was updated\n", sess_id);

done:
	if (unlikely(ret)) {
		pr_err("Failed to update session %u, ret = %d\n",
		       sess_id, ret);
		__smgr_stats_sess_update_failed();
	}

	return ret;
}

s32 smgr_session_update(u32 sess_id, struct pp_hw_si *hw_si)
{
	s32 ret = 0;

	__smgr_lock();

	ret = __smgr_session_update(sess_id, hw_si);

	__smgr_unlock();

	return ret;
}

/**
 * @brief Session create work to be schedule using a work queue in
 *        async mode or to be called directly in sync mode
 * @param work work to do
 */
static void __smgr_sess_create_work(struct work_struct *work)
{
	struct pp_sess_create_cb_args *cb_args;
	struct sess_cr_work_args      *_work;
	struct sess_info              sess;
	struct smgr_database          *db = smgr_get_db();
	s32 ret;
	ulong tmp_mod_flags; /* temp modification flags to use before
			      * allocating session entry
			      */

	if (ptr_is_null(work))
		return;

	_work = container_of(work, struct sess_cr_work_args, base.dwork.work);
	if (unlikely(__smgr_is_disable())) {
		pr_err("PP disabled\n");
		ret = -EPERM;
		goto callback;
	}

	memset(&sess, 0, sizeof(sess));
	tmp_mod_flags  = 0;
	sess.args      = _work->args;
	sess.mod_flags = &tmp_mod_flags;

	/* try to create the session */
	ret = __smgr_sess_create(&sess);

callback:
	/* ref to callback args struct */
	cb_args = &_work->cb_args;
	cb_args->base.ret = ret;
	if (likely(!ret))
		cb_args->sess_id  = sess.db_ent->info.sess_id;

	/* set callback parameters and notify user and free the memory,
	 * in sync request, when callback is null,
	 * the memory is freed by pp_session_create
	 */
	if (_work->base.cb) {
		cb_args->base.req_id = _work->base.req_id;
		cb_args->base.ev     = PP_SESS_CREATE;
		_work->base.cb(&cb_args->base);
		kmem_cache_free(db->work_args_cache, _work);
	}
}

/**
 * @brief Prepare session create arguments for session create work
 * @param req session create request
 * @return struct sess_cr_args* valid session create args pointer if
 *         available, NULL otherwise
 */
static struct sess_cr_work_args *
__smgr_sess_cr_args_prepare(struct pp_request *req,
			    struct pp_sess_create_args *sess_args)
{
	struct sess_cr_work_args *work_args = NULL;

	pr_pp_req(req, "");

	work_args = (struct sess_cr_work_args *)
		__smgr_work_args_prepare(req, __smgr_sess_create_work);
	if (unlikely(!work_args))
		return work_args;

	/* set arguments */
	if (PP_IS_SYNC_REQUEST(req)) {
		work_args->args = sess_args;
		return work_args;
	}

	/* in async mode, we must copy all arguments */
	memcpy(&work_args->args_buf, sess_args, sizeof(*sess_args));
	work_args->args = &work_args->args_buf;

	if (sess_args->rx) {
		memcpy(&work_args->rx, sess_args->rx,
		       sizeof(work_args->rx));
		work_args->args->rx = &work_args->rx;
	}
	if (sess_args->tx) {
		memcpy(&work_args->tx, sess_args->tx,
		       sizeof(work_args->tx));
		work_args->args->tx = &work_args->tx;
	}

	return work_args;
}

s32 pp_session_create(struct pp_sess_create_args *args, u32 *sess_id,
		      struct pp_request *req)
{
	struct sess_cr_work_args *wargs;
	struct smgr_database *db = smgr_get_db();
	s32 ret = 0;

	if (unlikely(__smgr_is_disable())) {
		pr_err("PP disabled\n");
		return -EPERM;
	}

	__smgr_stats_sess_cr_req(db);
	if (unlikely(ptr_is_null(args))) {
		__smgr_stats_invalid_args();
		__smgr_stats_sess_cr_failed(NULL);
		return -EINVAL;
	}

	/* in sync request, user must provide a pointer to save the session id*/
	if (unlikely(PP_IS_SYNC_REQUEST(req) && !sess_id)) {
		pr_err("sess_id null pointer in sync mode\n");
		__smgr_stats_invalid_args();
		__smgr_stats_sess_cr_failed(NULL);
		return -EINVAL;
	}

	wargs = __smgr_sess_cr_args_prepare(req, args);
	if (unlikely(!wargs)) {
		__smgr_stats_sess_cr_failed(NULL);
		return -ENOMEM;
	}

	__smgr_dispatch_work(req, &wargs->base, 0);

	/* sync request - copy the session id back to the user's pointer
	 * and return the return code
	 */
	if (PP_IS_SYNC_REQUEST(req)) {
		*sess_id = wargs->cb_args.sess_id;
		ret      = wargs->cb_args.base.ret;
		kmem_cache_free(db->work_args_cache, wargs);
	}

	return ret;
}
EXPORT_SYMBOL(pp_session_create);

/* ========================================================================== */
/*                               Session Delete                               */
/* ========================================================================== */
/**
 * @brief Untrack session from relevant bitmaps and lists
 * @param sess session to untrack
 */
static inline void __smgr_sess_untrack(struct sess_db_entry *ent)
{
	struct smgr_database *db = smgr_get_db();

	if (ptr_is_null(db))
		return;

	clear_bit(ent->info.sess_id, db->open_bmap);
	DB_IN_PORT_LIST_DEL_NODE(ent);
	DB_EG_PORT_LIST_DEL_NODE(ent);
	smgr_mcast_sess_untrack(ent);
}

/**
 * @brief Delete an existing PP session
 * @note MUST called with lock held
 * @param sess_id session id to delete
 * @return s32 0 on success deletion, error code otherwise
 */
static s32 __smgr_sess_delete(u32 sess_id)
{
	struct sess_db_entry *ent;
	struct sess_db_info  *info;
	struct pp_si si;
	s32 ret = 0;
	u32 i;

	pr_debug("deleting session %u\n", sess_id);
	if (unlikely(!__smgr_is_sess_exist(sess_id))) {
		__smgr_stats_invalid_args();
		ret = -ENOENT;
		goto done;
	}

	ent = __smgr_get_sess_db_ent(sess_id);
	if (unlikely(!ent)) {
		pr_crit("Session exist with DB no entry!!!\n");
		ret = -ENOENT;
		goto done;
	}
	info = &ent->info;

	/* detach the sync queue entry (if exist) */
	smgr_sq_del(ent);

	/* Get session SI */
	ret = smgr_session_si_get(sess_id, &si);
	if (unlikely(ret)) {
		__smgr_stats_hal_err();
		pr_err("failed to get session %u SI\n", sess_id);
		goto done;
	}

	/* detach the session from all the SGCs */
	for (i = 0; i < ARRAY_SIZE(si.sgc); i++)
		if (si.sgc[i] != PP_SGC_INVALID)
			chk_sgc_session_detach(i, si.sgc[i]);

	/* detach the session from all the TBMs */
	for (i = 0; i < ARRAY_SIZE(si.tbm); i++) {
		if (si.tbm[i] != PP_TBM_INVALID)
			chk_dual_tbm_session_detach(si.tbm[i]);
	}

	ret = cls_session_del(sess_id, info->fv_sz, &info->hash);
	if (unlikely(ret)) {
		__smgr_stats_hal_err();
		if (ret == -ENOENT)
			pr_err("session %u doesn't exist in HW!!!\n", sess_id);
		else
			pr_err("delete session %u from HW failed\n", sess_id);
		goto done;
	}

	__smgr_stats_sess_deleted(ent);
	__smgr_sess_untrack(ent);
	__smgr_sess_ent_free(ent);
	pr_debug("session %u deleted\n", sess_id);

done:
	if (unlikely(ret)) {
		pr_err("Failed to delete session %u, ret = %d\n",
		       sess_id, ret);
		__smgr_stats_sess_del_failed();
	}
	return ret;
}

/**
 * @brief Session delete work to be schedule using a work queue in
 *        async mode or to be called directly in sync mode
 * @param work work to do
 */
static void __smgr_sess_delete_work(struct work_struct *work)
{
	struct pp_sess_delete_cb_args *cb_args;
	struct sess_del_work_args     *_work;
	struct smgr_database          *db = smgr_get_db();
	s32 ret;

	if (ptr_is_null(work))
		return;

	_work = container_of(work, struct sess_del_work_args, base.dwork.work);
	if (unlikely(__smgr_is_disable())) {
		pr_err("PP disabled\n");
		ret = -EPERM;
		goto callback;
	}

	/* try to delete the session */
	__smgr_lock();
	ret = __smgr_sess_delete(_work->sess_id);
	__smgr_unlock();

callback:
	cb_args = &_work->cb_args;
	cb_args->base.ret = ret;
	cb_args->sess_id  = _work->sess_id;

	/* set callback parameters and notify user and free the memory,
	 * in sync request, when callback is null,
	 * the memory is freed by pp_session_delete
	 */
	if (_work->base.cb) {
		cb_args->base.req_id = _work->base.req_id;
		cb_args->base.ev     = PP_SESS_DELETE;
		_work->base.cb(&cb_args->base);
		kmem_cache_free(db->work_args_cache, _work);
	}
}

/**
 * @brief Prepare session delete arguments for the work function
 * @return struct sess_del_work_args* valid session delete args pointer if
 *         available, NULL otherwise
 */
static struct sess_del_work_args *
__smgr_sess_del_args_prepare(struct pp_request *req, u32 sess_id)
{
	struct sess_del_work_args *args = NULL;

	pr_pp_req(req, "sess_id %u\n", sess_id);

	args = (struct sess_del_work_args *)
		__smgr_work_args_prepare(req, __smgr_sess_delete_work);
	if (unlikely(!args))
		return args;

	/* set arguments */
	args->sess_id = sess_id;

	return args;
}

/**
 * @brief delete pp session
 * @param req session delete request
 * @return s32 return 0 for success
 */
s32 pp_session_delete(u32 sess_id, struct pp_request *req)
{
	struct sess_del_work_args *wargs;
	struct smgr_database *db = smgr_get_db();
	s32 ret = 0;

	if (unlikely(__smgr_is_disable())) {
		pr_err("PP disabled\n");
		return -EPERM;
	}

	__smgr_stats_sess_del_req(db);
	wargs = __smgr_sess_del_args_prepare(req, sess_id);
	if (unlikely(!wargs)) {
		__smgr_stats_sess_del_failed();
		return -ENOMEM;
	}

	__smgr_dispatch_work(req, &wargs->base, 0);

	/* sync request - return the real return code and free the memory */
	if (PP_IS_SYNC_REQUEST(req)) {
		ret = wargs->cb_args.base.ret;
		kmem_cache_free(db->work_args_cache, wargs);
	}

	return ret;
}
EXPORT_SYMBOL(pp_session_delete);

/* ========================================================================== */
/*                            Port Sessions Flush                             */
/* ========================================================================== */
/**
 * @brief Flush port's sessions
 * @note MUST called with lock held
 * @param pid port id, use 'PP_MAX_PORT' to flush all ports
 * @return s32 0 on success, error code otherwise
 */
static s32 __smgr_port_flush(u16 pid)
{
	struct sess_db_entry *ent, *tmp;
	struct smgr_database *db = smgr_get_db();
	u16 start, end, i;
	bool flush_all;
	s32 ret = 0;

	if (ptr_is_null(db))
		return -EINVAL;

	flush_all = (pid == PP_MAX_PORT);
	if (flush_all) {
		start = 0;
		end   = PP_MAX_PORT - 1;
	} else {
		if (!pmgr_port_is_active(pid)) {
			pr_err("port %u isn't active\n", pid);
			__smgr_stats_invalid_args();
			ret = -EFAULT;
			goto done;
		}
		start = pid;
		end = pid;
	}

	pr_debug("pid %u, start %u, end %u\n", pid, start, end);

	for (i = start; i <= end; i++) {
		if (!pmgr_port_is_active(i))
			continue;

		pr_debug("flushing port %u\n", i);
		/* flush ingress port sessions */
		list_for_each_entry_safe(ent, tmp, &db->in_port[i],
					 in_port_node) {
			ret = __smgr_sess_delete(ent->info.sess_id);
			if (unlikely(ret))
				pr_err("failed to delete port %u session id %u\n",
				       i, ent->info.sess_id);
		}
		/* flush egress port sessions */
		list_for_each_entry_safe(ent, tmp, &db->eg_port[i],
					 eg_port_node) {
			ret = __smgr_sess_delete(ent->info.sess_id);
			if (unlikely(ret))
				pr_err("failed to delete port %u session id %u\n",
				       i, ent->info.sess_id);
		}

		pr_debug("port %u flushed\n", i);
		if (!flush_all)
			__smgr_stats_port_flushed(db);
	}
	if (flush_all)
		__smgr_stats_flush_all_done(db);
	return ret;

done:
	if (flush_all)
		__smgr_stats_flush_all_failed(db);
	else
		__smgr_stats_port_flush_failed(db);
	pr_err("fail to flush port %u sessions, ret = %d\n", pid, ret);
	return ret;
}

/**
 * @brief Flush all ports work to be schedule using a work queue in
 *        async mode or to be called directly in sync mode
 * @param work work to do
 */
static void __smgr_port_flush_work(struct work_struct *work)
{
	struct pp_port_flush_cb_args *cb_args;
	struct port_flush_work_args  *_work;
	struct smgr_database         *db= smgr_get_db();
	s32 ret;

	if (ptr_is_null(work))
		return;

	_work = container_of(work, struct port_flush_work_args,
			     base.dwork.work);
	if (unlikely(__smgr_is_disable())) {
		pr_err("PP disabled\n");
		ret = -EPERM;
		goto callback;
	}

	/* try to flush the port's sessions */
	__smgr_lock();
	ret = __smgr_port_flush(_work->port_id);
	__smgr_unlock();

callback:
	cb_args = &_work->cb_args;
	cb_args->port_id     = _work->port_id;
	cb_args->base.ret    = ret;

	/* set callback parameters and notify user and free the memory,
	 * in sync request, when callback is null,
	 * the memory is freed by pp_port_flush or pp_flush_all
	 */
	if (_work->base.cb) {
		cb_args->base.req_id = _work->base.req_id;
		cb_args->base.ev     = (_work->port_id == PP_MAX_PORT) ?
					PP_FLUSH_ALL : PP_PORT_FLUSH;
		_work->base.cb(&cb_args->base);
		kmem_cache_free(db->work_args_cache, _work);
	}
}

/**
 * @brief Prepare port flush arguments for the work function
 * @param req async request
 * @param port_id port id to flush
 * @return struct port_flush_work_args* valid sport flush args
 *         pointer if available, NULL otherwise
 */
static struct port_flush_work_args *
__smgr_port_flush_args_prepare(struct pp_request *req, u16 port_id)
{
	struct port_flush_work_args *args = NULL;

	pr_pp_req(req, "port_id %u\n", port_id);

	args = (struct port_flush_work_args *)
		__smgr_work_args_prepare(req, __smgr_port_flush_work);
	if (unlikely(!args))
		return args;

	/* set arguments */
	args->port_id = port_id;

	return args;
}

s32 pp_port_flush(u16 port_id, struct pp_request *req)
{
	struct port_flush_work_args *wargs;
	struct smgr_database *db = smgr_get_db();
	s32 ret = 0;

	if (unlikely(__smgr_is_disable())) {
		pr_err("PP disabled\n");
		return -EPERM;
	}

	__smgr_stats_port_flush_req(db);
	wargs = __smgr_port_flush_args_prepare(req, port_id);
	if (unlikely(!wargs)) {
		__smgr_stats_port_flush_failed(db);
		return -ENOMEM;
	}

	__smgr_dispatch_work(req, &wargs->base, 0);

	/* sync request - return the real return code and free the memory */
	if (PP_IS_SYNC_REQUEST(req)) {
		ret = wargs->cb_args.base.ret;
		kmem_cache_free(db->work_args_cache, wargs);
	}

	return ret;
}
EXPORT_SYMBOL(pp_port_flush);

/* ========================================================================== */
/*                            Flush All Sessions                              */
/* ========================================================================== */
s32 pp_flush_all(struct pp_request *req)
{
	struct port_flush_work_args *wargs;
	struct smgr_database *db = smgr_get_db();
	s32 ret = 0;

	if (unlikely(__smgr_is_disable())) {
		pr_err("PP disabled\n");
		return -EPERM;
	}

	__smgr_stats_flush_all_req(db);
	/* we use port flush with PP_MAX_PORT to flush all ports */
	wargs = __smgr_port_flush_args_prepare(req, PP_MAX_PORT);
	if (unlikely(!wargs)) {
		__smgr_stats_flush_all_failed(db);
		return -ENOMEM;
	}

	__smgr_dispatch_work(req, &wargs->base, 0);

	/* sync request - return the real return code and free the memory */
	if (PP_IS_SYNC_REQUEST(req)) {
		ret = wargs->cb_args.base.ret;
		kmem_cache_free(db->work_args_cache, wargs);
	}

	return ret;
}
EXPORT_SYMBOL(pp_flush_all);

/* ========================================================================== */
/*                            Inactive List Sessions                          */
/* ========================================================================== */

/**
 * @brief Convert stalled sessions bitmap created by the crawler into
 *        an array with real sessions indexes, we can start converting
 *        the sessions only from the second scheduled work so we scan
 *        the previous chunk
 * @param w what to do
 */
static void __smgr_inactive_sess_list_get(struct inactive_work_args *w)
{
	struct smgr_database *db = smgr_get_db();
	ulong *map;
	u32 id;

	if (ptr_is_null(db))
		return;

	map = db->inact_bmap;
	id = w->base_id - w->it_sz;
	pr_debug("converting sessions [%u - %u)\n", id, w->base_id);

	pp_cache_invalidate(db->inact_bmap, db->inact_bmap_sz);
	for_each_set_bit_from(id, map, w->n_sessions) {
		w->inact[w->inact_found++] = id;
		/* did we use all entries ?*/
		if (w->inact_found == w->inact_n || id == w->base_id)
			break;
	}
}

/**
 * @brief Start scanning for inactive session in a specific block
 *        specified by the work argument and schedule next iteration
 *        work
 * @param w what to scan
 * @return s32 0 on success, error code otherwise
 */
static inline s32 __smgr_inactive_scan_start(struct inactive_work_args *w)
{
	s32 ret = 0;

	pr_debug("Start crawler scan for sessions %u - %u\n",
		 w->base_id, w->base_id + w->it_sz - 1);

	/* start crawler */
	ret = chk_crawler_start(w->base_id, w->it_sz, w->bmap_phys, w->bmap_sz);
	if (unlikely(ret)) {
		__smgr_stats_hal_err();
		pr_err("failed to start crawler scan, ret %d\n", ret);
	}

	return ret;
}

/**
 * @brief Get inactive sessions work to be schedule using a work queue
 *        in async mode or to be called directly in sync mode
 * @param work work to do
 */
static void __smgr_inactive_work(struct work_struct *work)
{
	struct pp_inactive_list_cb_args cb_args = { 0 };
	struct inactive_work_args *w;
	struct pp_request req;
	struct smgr_database *db = smgr_get_db();
	u32 tries = 0;
	s32 ret;

	if (ptr_is_null(work))
		return;

	w = container_of(work, struct inactive_work_args, base.dwork.work);
	if (unlikely(__smgr_is_disable())) {
		pr_err("PP disabled\n");
		ret = -EPERM;
		goto callback;
	}

	/* wait to crawler to finish */
	ret = chk_crawler_idle_wait(&tries);
	atomic_add(tries, &db->stats.crwlr_idle_wait);
	if (unlikely(ret)) {
		__smgr_stats_hal_err();
		goto callback;
	}

	/* did we scan all sessions? */
	if (w->base_id < w->n_sessions) {
		ret = __smgr_inactive_scan_start(w);
		if (unlikely(ret))
			goto callback;
	}

	/* from second work and above, while the checker is scanning the new
	 * chunk, we populate the user's array with sessions indexes we found
	 * so far
	 */
	if (w->base_id)
		__smgr_inactive_sess_list_get(w);

	/* do we need to schedule more works? */
	if (w->base_id < w->n_sessions && w->inact_found < w->inact_n) {
		/* prepare work for next scan */
		w->base_id  += w->it_sz;
		/* create dummy request */
		req.cb       = w->base.cb;
		req.req_id   = w->base.req_id;
		req.req_prio = w->base.req_prio;
		/* dispatch */
		pr_debug("Scheduling next scan, base_id %u\n", w->base_id);
		__smgr_dispatch_work(&req, &w->base, w->delay);
		return;
	}

	__smgr_stats_inactive_done(db);
	ret = 0;

callback:
	cb_args.inact_sess  = w->inact;
	cb_args.n_sessions  = w->inact_found;
	cb_args.base.ret    = ret;
	cb_args.base.req_id = w->base.req_id;
	cb_args.base.ev     = PP_INACTIVE_LIST;
	w->base.cb(&cb_args.base);
	/* free memory and restore scan state back to idle */
	kmem_cache_free(db->work_args_cache, w);
	__smgr_lock();
	if (db->scan_state != INACT_SCAN_DISABLE)
		db->scan_state = INACT_SCAN_IDLE;
	__smgr_unlock();
}

/**
 * @brief Prepare getting inactive sessions list work arguments
 * @return struct inactive_work_args* valid pointer if available, NULL
 *         otherwise
 */
static struct inactive_work_args *
__smgr_inactive_args_prepare(struct pp_request *req, u32 *inact, u32 inact_sz)
{
	struct inactive_work_args *args = NULL;
	struct smgr_database *db = smgr_get_db();
	u32 delay;
	u32 it_sz;

	if (ptr_is_null(db))
		return NULL;

	it_sz = min(CHK_CRWLR_MAX_ITR_SIZE, db->n_sessions);
	/* scan_time_get will also fix it_sz to be the real number of
	 * session that will be scanned in each iteration
	 */
	if (chk_scan_time_get(&it_sz, &delay))
		return NULL;

	pr_pp_req(req, "inact %p, inact_sz %u, it_sz %u, delay %u us\n",
		  inact, inact_sz, it_sz, delay);

	args = (struct inactive_work_args *)
		__smgr_work_args_prepare(req, __smgr_inactive_work);
	if (unlikely(!args))
		return args;

	/* set arguments for scanning all sessions */
	args->base_id     = 0;
	args->n_sessions  = db->n_sessions;
	args->it_sz       = it_sz;
	args->bmap_phys   = db->inact_bmap_phys;
	args->bmap_sz     = db->inact_bmap_sz;
	args->inact       = inact;
	args->inact_n     = inact_sz;
	args->inact_found = 0;
	args->delay       = (ulong)delay;

	return args;
}

s32 pp_inactive_sessions_get(struct pp_request *req, u32 *inact, u32 n)
{
	struct inactive_work_args *wargs;
	struct smgr_database *db = smgr_get_db();
	struct pp_inactive_list_cb_args cb_args = { 0 };
	s32 ret = 0;

	if (unlikely(__smgr_is_disable())) {
		pr_err("PP disabled\n");
		return -EPERM;
	}

	__smgr_stats_inactive_req(db);
	if (unlikely(!(!ptr_is_null(req) &&
		       !ptr_is_null(req->cb) &&
		       !ptr_is_null(inact)))) {
		__smgr_stats_invalid_args();
		ret = -EINVAL;
		goto done;

	}

	if (unlikely(n == 0)) {
		pr_err("zero array size\n");
		__smgr_stats_invalid_args();
		ret = -EINVAL;
		goto done;
	}

	__smgr_lock();
	if (db->scan_state == INACT_SCAN_DISABLE) {
		__smgr_unlock();
		pr_debug("PP inactive sessions scan is disabled\n");
		/* when the scan is disabled, just call the user callback
		 * with an empty array of inactive sessions
		 */
		cb_args.inact_sess  = inact;
		cb_args.n_sessions  = 0;
		cb_args.base.ret    = 0;
		cb_args.base.req_id = req->req_id;
		cb_args.base.ev     = PP_INACTIVE_LIST;
		req->cb(&cb_args.base);
		return 0;
	}

	if (db->scan_state == INACT_SCAN_BUSY) {
		pr_err("scan in progress, cannot start new one\n");
		__smgr_stats_resource_busy();
		ret = -EBUSY;
		__smgr_unlock();
		goto done;
	}
	db->scan_state = INACT_SCAN_BUSY;
	__smgr_unlock();

	memset(db->inact_bmap, 0, db->inact_bmap_sz); /* init bitmap */
	pp_cache_writeback(db->inact_bmap, db->inact_bmap_sz);

	wargs = __smgr_inactive_args_prepare(req, inact, n);
	if (unlikely(!wargs)) {
		ret = -ENOMEM;
		__smgr_lock();
		db->scan_state = INACT_SCAN_IDLE;
		__smgr_unlock();
		goto done;
	}
	__smgr_dispatch_work(req, &wargs->base, 0);

done:
	if (unlikely(ret))
		__smgr_stats_inactive_failed(db);

	return ret;
}
EXPORT_SYMBOL(pp_inactive_sessions_get);

/* ========================================================================== */
/*                            Session Group Counters                          */
/* ========================================================================== */

s32 smgr_sgc_sessions_bmap_get(u8 grp, u16 cntr, ulong *bmap, u32 n_bits)
{
	struct pp_si si = { 0 };
	u32 sess;
	s32 ret;

	if (unlikely(ptr_is_null(bmap) || !n_bits))
		return -EINVAL;

	if (unlikely(!chk_sgc_is_group_valid(grp)))
		return -EINVAL;

	/* get all opened sessions */
	ret = smgr_open_sessions_bmap_get(bmap, n_bits);
	if (unlikely(ret))
		return ret;

	/* iterate over all sessions and check who is using the sgc */
	for_each_set_bit(sess, bmap, n_bits) {
		ret = smgr_session_si_get(sess, &si);
		if (unlikely(ret)) {
			pr_err("failed to get session %u si, ret %d\n", sess,
			       ret);
			return ret;
		}

		/* if no counter is used in the group or it is no
		 * the counter, clear the session bit
		 */
		if (!test_bit(grp, &si.sgc_en_map) || si.sgc[grp] != cntr)
			clear_bit(sess, bmap);
	}

	return 0;
}

s32 pp_sgc_sessions_get(u8 grp, u16 cntr, u32 *sessions, u32 *n_sessions)
{
	ulong *bmap;
	u32 sess, n_bits, i;
	s32 ret;

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	if (unlikely(ptr_is_null(sessions) || ptr_is_null(n_sessions)))
		return -EINVAL;

	/* allocate bitmap for all sessions */
	ret = smgr_sessions_bmap_alloc(&bmap, &n_bits);
	if (unlikely(ret))
		return ret;

	/* get all sgc sessions in bitmap form */
	ret = smgr_sgc_sessions_bmap_get(grp, cntr, bmap, n_bits);
	if (unlikely(ret))
		goto done;

	/* convert the bitmap into indexes */
	i = 0;
	for_each_set_bit(sess, bmap, n_bits) {
		/* break if we reach the end of the user array */
		if (*n_sessions <= i)
			break;
		sessions[i++] = sess;
	}
	*n_sessions = i;

done:
	kfree(bmap);
	return ret;
}

s32 pp_sgc_group_reserve(u8 grp, u32 *owner)
{
	if (unlikely(!pp_is_ready()))
		return -EPERM;

	return chk_sgc_group_reserve(grp, owner);
}

s32 pp_sgc_group_unreserve(u8 grp, u32 owner)
{
	if (unlikely(!pp_is_ready()))
		return -EPERM;

	return chk_sgc_group_unreserve(grp, owner);
}

s32 pp_sgc_alloc(u32 owner, u8 grp, u16 *counters, u16 count)
{
	s32 ret = 0;
	u32 i;

	if (unlikely(!pp_is_ready()))
		return -EPERM;
	if (unlikely(ptr_is_null(counters)))
		return -EINVAL;
	if (unlikely(count == 0)) {
		pr_err("cannot allocate 0 counters!\n");
		return -EINVAL;
	}

	/* try to allocate */
	for (i = 0; i < count; i++) {
		ret = chk_sgc_alloc(owner, grp, &counters[i]);
		if (likely(ret == 0))
			continue;
		pr_err("failed to allocate SGC counter on group %u\n", grp);
		goto alloc_fail;
	}

	return ret;
alloc_fail:
	/* free the ones we manage to allocate */
	while (i--)
		chk_sgc_free(owner, grp, counters[i]);

	return ret;
}
EXPORT_SYMBOL(pp_sgc_alloc);

s32 pp_sgc_free(u32 owner, u8 grp, u16 *counters, u16 count)
{
	s32 ret = 0;
	u32 i;

	if (unlikely(!pp_is_ready()))
		return -EPERM;
	if (unlikely(ptr_is_null(counters)))
		return -EINVAL;
	if (unlikely(count == 0)) {
		pr_err("cannot free 0 counters!\n");
		return -EINVAL;
	}

	/* free the counters */
	for (i = 0; i < count; i++) {
		ret = chk_sgc_free(owner, grp, counters[i]);
		if (unlikely(ret))
			pr_err("failed to free SGC counter [%u][%04u]\n",
			       grp, counters[i]);
	}

	return ret;
}
EXPORT_SYMBOL(pp_sgc_free);

s32 pp_sgc_get(u8 grp, u16 sgc_id, struct pp_stats *stats, u32 *num_sessions)
{
	if (unlikely(!pp_is_ready()))
		return -EPERM;
	if (unlikely(ptr_is_null(stats)))
		return -EINVAL;

	return chk_sgc_get(grp, sgc_id, &stats->packets, &stats->bytes,
			   num_sessions);
}
EXPORT_SYMBOL(pp_sgc_get);

s32 pp_sgc_mod(u32 owner, u8 grp, u16 sgc_id, enum pp_stats_op op, u32 pkts,
	       u32 bytes)
{
	if (unlikely(!pp_is_ready()))
		return -EPERM;

	return chk_sgc_mod(owner, grp, sgc_id, op, pkts, bytes);
}
EXPORT_SYMBOL(pp_sgc_mod);

/* ========================================================================== */
/*                            Token Bucket Meter                              */
/* ========================================================================== */

s32 pp_dual_tbm_alloc(u16 *idx, struct pp_dual_tbm *cfg)
{
	s32 ret;
	u16 _idx;

	if (unlikely(!pp_is_ready()))
		return -EPERM;

	if (unlikely(ptr_is_null(idx)))
		return -EINVAL;

	ret = chk_dual_tbm_alloc(&_idx);
	if (unlikely(ret)) {
		pr_err("failed to allocate dual tbm, ret %d\n", ret);
		return ret;
	}

	/* if cfg isn't provided, we are done */
	if (!cfg)
		goto done;

	/* cfg provided, try to set the tbm configuration */
	ret = chk_dual_tbm_set(_idx, cfg);
	if (unlikely(ret)) {
		pr_err("failed to set tbm %u configuration, ret %d\n", _idx,
		       ret);
		chk_dual_tbm_free(_idx);
		return ret;
	}

done:
	*idx = _idx;
	return ret;
}
EXPORT_SYMBOL(pp_dual_tbm_alloc);

s32 pp_dual_tbm_free(u16 idx)
{
	if (unlikely(!pp_is_ready()))
		return -EPERM;

	return chk_dual_tbm_free(idx);
}
EXPORT_SYMBOL(pp_dual_tbm_free);

s32 pp_dual_tbm_get(u16 idx, struct pp_dual_tbm *cfg)
{
	if (unlikely(!pp_is_ready()))
		return -EPERM;

	return chk_dual_tbm_get(idx, cfg);
}
EXPORT_SYMBOL(pp_dual_tbm_get);

s32 pp_dual_tbm_set(u16 idx, struct pp_dual_tbm *cfg)
{
	if (unlikely(!pp_is_ready()))
		return -EPERM;

	return chk_dual_tbm_set(idx, cfg);
}
EXPORT_SYMBOL(pp_dual_tbm_set);

/* ========================================================================== */
/*                                  TurboDox                                  */
/* ========================================================================== */
s32 pp_tdox_tout_set(u32 tout)
{
	if (unlikely(!pp_is_ready()))
		return -EPERM;

	return smgr_tdox_tout_set(tout);
}
EXPORT_SYMBOL(pp_tdox_tout_set);

/* ========================================================================== */
/*                                Module APIs                                 */
/* ========================================================================== */
/**
 * @brief Helper function initialize all session manager ports info
 * @param db session manager db
 * @return s32 0 on success, error code otherwise
 */
static void __smgr_db_ports_init(struct smgr_database *db)
{
	u32 i;

	for (i = 0; i < PP_MAX_PORT; i++) {
		/* ingress ports sessions lists */
		INIT_LIST_HEAD(&db->in_port[i]);
		/* egress ports sessions lists */
		INIT_LIST_HEAD(&db->eg_port[i]);
	}
}

/**
 * @brief Helper function to initialize session manager sessions info
 * @param db session manager db
 * @return s32 0 on success, error code otherwise
 */
static s32 __smgr_db_sessions_init(struct device *dev, struct smgr_database *db)
{
	struct sess_db_entry *sess;
	u32 i;

	/* init free sessions list */
	INIT_LIST_HEAD(&db->free_list);

	/* allocate sessions db entries */
	db->sessions = pp_devm_kvcalloc(dev, db->n_sessions,
					sizeof(*db->sessions), GFP_KERNEL);
	if (unlikely(!db->sessions)) {
		pr_err("Failed to allocate %u sessions memory\n",
		       db->n_sessions);
		return -ENOMEM;
	}

	for (i = 0; i < db->n_sessions; i++) {
		sess = &db->sessions[i];
		/* set session id */
		sess->info.sess_id = i;
		/* init sessions lists nodes */
		INIT_LIST_HEAD(&sess->free_node);
		INIT_LIST_HEAD(&sess->in_port_node);
		INIT_LIST_HEAD(&sess->eg_port_node);
		/* add the session to the free list */
		DB_FREE_LIST_ADD_NODE(db, sess);
	}
	atomic_set(&db->stats.sess_free, db->n_sessions);

	return 0;
}

/**
 * @brief Helper function to initialize database bitmaps
 * @param dev dev to use for memory allocations
 * @param db session manager db
 * @return s32 0 on success, error code otherwise
 */
static s32 __smgr_db_maps_init(struct device *dev, struct smgr_database *db)
{
	/* allocate open sessions bitmap */
	db->open_bmap = devm_kcalloc(dev, BITS_TO_LONGS(db->n_sessions),
				     sizeof(*db->open_bmap), GFP_KERNEL);
	if (unlikely(!db->open_bmap)) {
		pr_err("Failed to allocate busy sessions bitmap\n");
		return -ENOMEM;
	}
	/* allocate ipv4 sessions bitmap */
	db->ipv4_bmap = devm_kcalloc(dev, BITS_TO_LONGS(db->n_sessions),
				     sizeof(*db->ipv4_bmap), GFP_KERNEL);
	if (unlikely(!db->ipv4_bmap)) {
		pr_err("Failed to allocate ipv4 sessions bitmap\n");
		return -ENOMEM;
	}
	/* allocate ipv6 sessions bitmap */
	db->ipv6_bmap = devm_kcalloc(dev, BITS_TO_LONGS(db->n_sessions),
				     sizeof(*db->ipv6_bmap), GFP_KERNEL);
	if (unlikely(!db->ipv6_bmap)) {
		pr_err("Failed to allocate ipv6 sessions bitmap\n");
		return -ENOMEM;
	}
	/* allocate tcp sessions bitmap */
	db->tcp_bmap = devm_kcalloc(dev, BITS_TO_LONGS(db->n_sessions),
				    sizeof(*db->tcp_bmap),  GFP_KERNEL);
	if (unlikely(!db->tcp_bmap)) {
		pr_err("Failed to allocate tcp sessions bitmap\n");
		return -ENOMEM;
	}
	/* allocate udp sessions bitmap */
	db->udp_bmap = devm_kcalloc(dev, BITS_TO_LONGS(db->n_sessions),
				    sizeof(*db->udp_bmap),  GFP_KERNEL);
	if (unlikely(!db->udp_bmap)) {
		pr_err("Failed to allocate udp sessions bitmap\n");
		return -ENOMEM;
	}

	return 0;
}

/**
 * @brief Helper function to initialize database caches
 * @param db session manager db
 * @return s32 0 on success, error code otherwise
 */
static s32 __smgr_db_caches_init(struct smgr_database *db)
{
	/* allocate work args cache */
	db->work_args_cache =
		kmem_cache_create("smgr_work_args", sizeof(union work_args), 0,
				  SLAB_HWCACHE_ALIGN | SLAB_MEM_SPREAD |
				  SLAB_TEMPORARY | SLAB_PANIC, NULL);
	if (unlikely(!db->work_args_cache)) {
		pr_err("Allocating work args cache failed\n");
		return -ENOMEM;
	}

	return 0;
}

/**
 * @brief Helper function to initialize database stuff for async work mode
 * @param db session manager db
 * @return s32 0 on success, error code otherwise
 */
static s32 __smgr_db_async_init(struct smgr_database *db)
{
	u32 flags = WQ_MEM_RECLAIM | WQ_UNBOUND;

	/* Create work queue */
	db->workq = alloc_workqueue("%s", flags, 2, "Session Manager");
	if (!db->workq) {
		pr_err("Failed to create work queue");
		return -ENOMEM;
	}

	/* Create high priority work queue */
	flags |= WQ_HIGHPRI;
	db->workq_hi = alloc_workqueue("%s", flags, 2, "Session Manager High");
	if (!db->workq_hi) {
		pr_err("Failed to create high work queue");
		return -ENOMEM;
	}
	return 0;
}

s32 smgr_nf_set(enum pp_nf_type type, struct pp_nf_info *nf, u16 phyq, u16 cycl2_q)
{
	switch (type) {
	case PP_NF_REASSEMBLY:
		break;
	case PP_NF_TURBODOX:
		smgr_tdox_nf_set(phyq);
		smgr_lro_nf_set(nf->pid, nf->subif, cycl2_q);
		break;
	case PP_NF_FRAGMENTER:
		break;
	case PP_NF_MULTICAST:
		break;
	case PP_NF_IPSEC_LLD:
		break;
	case PP_NF_REMARKING:
		break;
	default:
		pr_err("Unknown nf type %u\n", type);
		return -EINVAL;
	}

	return 0;
}

/**
 * @brief Initialized session manager database and update dev_priv->smgr_db
 * @param dev device to use for all memories allocations, we allocate using
 *        device structure just for extra safety in case we didn't free
 *        some memory when the PP module goes down
 * @param dev_priv to hold smgr_database in pp_dev_priv
 * @param init_param
 * @return s32 0 for success, non-zero otherwise
 */
static s32 __smgr_db_init(struct device *dev, struct pp_dev_priv *dev_priv,
			  struct pp_smgr_init_param *init_param)
{
	s32 ret;
	struct smgr_database *db;

	pr_debug("Session manager init db\n");

	/* verify number of required sessions is can be supported by HW
	 * before doing anything
	 */
	if (unlikely(init_param->num_sessions > SMGR_MAX_HW_SESSIONS)) {
		pr_err("To much sessions to support, max is %u\n",
			SMGR_MAX_HW_SESSIONS);
		return -E2BIG;
	}

	/* allocate database structure */
	db = devm_kzalloc(dev, sizeof(*db), GFP_KERNEL);
	if (unlikely(!db)) {
		pr_err("Failed to allocate session_mgr_db memory of size %zu\n",
			sizeof(*db));
		return -ENOMEM;
	}
	/* updateing the dev_priv to hold the db */
	dev_priv->smgr_db = db;

	/* save number of sessions */
	db->n_sessions = init_param->num_sessions;
	/* enable by default */
	db->state = SMGR_ENABLE;
	db->syncq_en = true;
	db->frag_mode = init_param->frag_mode;
	/* disable inactive scan */
	db->scan_state = INACT_SCAN_IDLE;
	/* default acceleration mode */
	db->accl_mode = init_param->accl_mode;
	/* init lock */
	spin_lock_init(&db->lock);
	/* allocate bitmap for HW crawler */
	/* roundup session number to fit crawler hw limitations */
	db->inact_bmap_sz =
		max(CHK_CRWLR_MIN_ITR_SIZE * 2, db->n_sessions);
	db->inact_bmap_sz = roundup_pow_of_two(db->inact_bmap_sz);
	db->inact_bmap_sz =
		BITS_TO_LONGS(db->inact_bmap_sz) * sizeof(ulong);

	db->inact_bmap_sz = PAGE_ALIGN(db->inact_bmap_sz);
	db->inact_bmap = pp_dma_alloc(db->inact_bmap_sz, &db->inact_bmap_phys,
				      0);
	if (!db->inact_bmap) {
		pr_err("Failed to allocate %zu bytes for inactive sessions map\n",
		       db->inact_bmap_sz);
		return -ENOMEM;
	}

	pr_debug("inact_map_phys %pad, inact_map_sz %zu\n",
			 &db->inact_bmap_phys, db->inact_bmap_sz);

	/* init all database bitmaps */
	ret = __smgr_db_maps_init(dev, db);
	if (unlikely(ret))
		return ret;
	/* sessions entries init */
	ret = __smgr_db_sessions_init(dev, db);
	if (unlikely(ret))
		return ret;
	/* tdox sessions entries init */
	ret = smgr_tdox_init(dev, db);
	if (unlikely(ret))
		return ret;
	/* lro sessions entries init */
	ret = smgr_lro_init(dev, db);
	if (unlikely(ret))
		return ret;
	/* caches */
	ret = __smgr_db_caches_init(db);
	if (unlikely(ret))
		return ret;
	/* async mode */
	ret = __smgr_db_async_init(db);
	if (unlikely(ret))
		return ret;
	/* ports info init */
	__smgr_db_ports_init(db);

	return 0;
}

/**
 * @brief Destroy database
 */
static void __smgr_db_destroy(void)
{
	struct pp_dev_priv *dev_priv = pp_priv_get();
	struct device *dev = pp_dev_get();
	struct smgr_database *db = NULL;

	if (!dev || !dev_priv || !dev_priv->smgr_db)
		return;

	db = dev_priv->smgr_db;

	if (likely(db->workq))
		destroy_workqueue(db->workq);
	if (likely(db->workq_hi))
		destroy_workqueue(db->workq_hi);
	if (likely(db->sessions))
		pp_devm_kvfree(dev, db->sessions);
	if (likely(db->open_bmap))
		devm_kfree(dev, db->open_bmap);
	if (likely(db->ipv4_bmap))
		devm_kfree(dev, db->ipv4_bmap);
	if (likely(db->ipv6_bmap))
		devm_kfree(dev, db->ipv6_bmap);
	if (likely(db->tcp_bmap))
		devm_kfree(dev, db->tcp_bmap);
	if (likely(db->udp_bmap))
		devm_kfree(dev, db->udp_bmap);

	kmem_cache_destroy(db->work_args_cache);
	pp_dma_free(db->inact_bmap, db->inact_bmap_sz);

	devm_kfree(dev, db);
	dev_priv->smgr_db = NULL;
}

s32 pp_smgr_init(struct device *dev)
{
	s32 ret = 0;
	struct pp_dev_priv *dev_priv;
	struct pp_smgr_init_param *init_param;

	if (ptr_is_null(dev))
		return -EINVAL;

	dev_priv = pp_priv_get();
	if (ptr_is_null(dev_priv))
		return -EINVAL;

	init_param = &dev_priv->dts_cfg.smgr_params;
	if (!init_param->valid)
		return -EINVAL;

	pr_debug("PP session manager init start, num_sess %u\n",
		 init_param->num_sessions);

	/* Init database */
	ret = __smgr_db_init(dev, dev_priv, init_param);
	if (unlikely(ret)) {
		pr_err("Failed to initialized module db\n");
		goto done;
	}

	/* multicast init */
	ret = smgr_mcast_init(dev, init_param->num_sessions);
	if (unlikely(ret)) {
		pr_err("Failed to initialized multicast db\n");
		goto done;
	}

	/* Init debug */
	ret = smgr_dbg_init(dev, init_param->dbgfs);
	if (unlikely(ret)) {
		pr_err("Failed to initialized module debugfs\n");
		goto done;
	}

done:
	pr_debug("PP session manager init %s, ret = %d\n",
		 ret ? "failed" : "done", ret);
	return ret;
}

void pp_smgr_exit(void)
{
	pr_debug("PP session manager exit start\n");

	/* Exit syncq */
	smgr_sq_exit();
	/* Clean debug */
	smgr_dbg_clean();
	/* Clean multicast */
	smgr_mcast_exit();
	/* Clean tdox */
	smgr_tdox_exit();
	/* Clean lro */
	smgr_lro_exit();

	/* Free database memory */
	__smgr_db_destroy();

	pr_debug("PP session manager exit done\n");
}
