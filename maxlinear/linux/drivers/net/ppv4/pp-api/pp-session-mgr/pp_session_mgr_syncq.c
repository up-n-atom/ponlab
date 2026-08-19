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
 * Description: PP session manager synchronization queues module
 */

#define pr_fmt(fmt) "[PP_SESS_MGR_SQ]:%s:%d: " fmt, __func__, __LINE__

#include <linux/kernel.h>     /* For pr_err/pr_debug/...   */
#include <linux/types.h>      /* For standard types        */
#include <linux/spinlock.h>   /* For DB lock               */
#include <linux/errno.h>      /* For the EINVAL/EEXIST/... */
#include <linux/bug.h>        /* For WARN()                */
#include <linux/list.h>       /* For free/act lists        */
#include <linux/hrtimer.h>    /* For sync/done hr timers   */
#include <linux/ktime.h>      /* For ktime                 */
#include <linux/pp_qos_api.h> /* For QoS manager APIs      */

#include "checker.h"          /* For syncq HW table        */
#include "pp_qos_utils.h"     /* For QoS syncq APIs        */
#include "pp_common.h"
#include "pp_session_mgr.h"
#include "pp_session_mgr_internal.h"

/**
 * @brief hrtimer_try_to_cancel function return values
 */
#define  HRTIMER_WAS_NOT_ACTIVE 0
#define  HRTIMER_WAS_ACTIVE     1
#define  HRTIMER_CB_IS_RUNNING -1

/**
 * @define SQ_LSPP_RCV_TO
 * @brief timeout in us, to recive the lspp in TX hook
 */
#define SQ_LSPP_RCV_TO  100000

/**
 * @define SQ_LSPP_SENT_TO
 * @brief timeout in us, to wait for the lspp to be enqueued + dequeued.
 * @note LSPP: Last Slow Path Packet
 */
#define SQ_LSPP_SENT_TO 1000

/**
 * @define SQ_DONE_TO
 * @brief timeout in us, after that the syncq will be released
 */
#define SQ_DONE_TO      300000

/**
 * @define SQ_DFLT_QLEN
 * @brief maximum sync queue length
 */
#define SQ_DFLT_QLEN 0x1000

/**
 * @define SQ_PR
 * @brief printing a synch queue entry
 */
#define SQ_PR(pr_func, sq) \
	pr_func("SQ[%02hhu] SESS[%05u] Q_ID[%04u] DST_Q[%04u] STATE[%s]\n", \
		(sq)->id, (sq)->session, (sq)->queue_id, (sq)->dst_queue_id, \
		__sq_state_str((sq)->state))

/**
 * @define SQ_FOREACH_ACTIVE_ENTRY
 * @brief go over all the active sq entries from db
 */
#define SQ_FOREACH_ACTIVE_ENTRY(db, sq) \
	list_for_each_entry((sq), &(db)->act_list, node)

/**
 * @define SQ_FREE_ENTRY_SET
 * @brief move the sq entry to the free list
 */
#define SQ_FREE_ENTRY_SET(db, sq) \
	list_move_tail(&(sq)->node, &(db)->free_list)

/**
 * @define SQ_IS_DB_FULL
 * @brief check if the sq free list is emty
 */
#define SQ_IS_DB_FULL(db) \
	list_empty(&(db)->free_list)

/**
 * @define SQ_FREE_ENTRY_GET
 * @brief get the first free sq entry
 */
#define SQ_FREE_ENTRY_GET(db) \
	list_first_entry(&(db)->free_list, struct sq_entry, node)

/**
 * @define SQ_ACTIVE_ENTRY_SET
 * @brief move the sq entry to the active list
 */
#define SQ_ACTIVE_ENTRY_SET(db, sq) \
	list_move(&(sq)->node, &(db)->act_list)

/**
 * @define SQ_FREE_ENTRY_INIT
 * @brief add the sq entry to the free list
 */
#define SQ_FREE_ENTRY_INIT(db, sq) \
	list_add_tail(&(sq)->node, &(db)->free_list)

/**
 * @enum sq_state
 * @brief this is the sync queue state machine, this enum
 *        described the syncq entry life cyncle
 */
enum sq_state {
	/*! syncq is not attached to any session, located in the free list */
	SQ_STATE_IDLE,

	/*! syncq was allocated but the timer not activated */
	SQ_STATE_ALLOCATED,

	/*! syncq is pending on lspp or "lspp_rcv_timer" */
	SQ_STATE_STARTED,

	/*! lspp sent from CPU, syncq is pending on "lspp_sent_timer" */
	SQ_STATE_LSPP_SENT,

	/*! syncq process was done and pending on "done_timer" */
	SQ_STATE_SYNC_DONE,

	SQ_STATE_NUM,
};

/**
 * @struct sq_entry
 * @brief sync queue entry
 */
struct sq_entry {
	/*! entry id */
	u8                id;

	/*! entry state (enum sq_state) */
	enum sq_state     state;

	/*! attached session id */
	u32               session;

	/*! attached hash signature */
	u32               hash_sig;

	/*! the synchronization queue id */
	u32               queue_id;

	/*! the synchronization queue (phy) */
	u32               queue_phy;

	/*! the original destination queue id */
	u32               dst_queue_id;

	/*! timer to wait for lspp to cuptrue in TX hook */
	struct hrtimer    lspp_rcv_timer;

	/*! lspp is sent, timer for "sync" event */
	struct hrtimer    lspp_sent_timer;

	/*! timer for "done" event */
	struct hrtimer    done_timer;

	/*! entry list node */
	struct list_head  node;
};

/**
 * @struct sq_database
 * @brief sync queue module database
 */
struct sq_database {
	/*! number of synch queues supported */
	u32                  n_sq;

	/*! locks database and HW accesses */
	spinlock_t           lock;

	/*! free entries list */
	struct list_head     free_list;

	/*! active entries list */
	struct list_head     act_list;

	/*! sync queues array */
	struct sq_entry      *sq;

	/*! sync queues stats */
	struct smgr_sq_stats stats;

	/*! QoS device for using the QoS APIs */
	struct pp_qos_dev    *qdev;

	/*! lspp recive event timeout */
	u32                  sq_lspp_rcv_to;

	/*! lspp sent event timeout */
	u32                  sq_lspp_sent_to;

	/*! done event timeout */
	u32                  sq_done_to;

	/*! maximum sync queue length (max packets allowed) */
	u32                  sq_qlen;

	/*! string state of sq */
	const char *sq_state_str[SQ_STATE_NUM + 1];
};

static struct sq_entry *__sq_get_by_session(u32 session);
static struct sq_entry *__sq_get_by_hash_sig(u32 sig);
static struct sq_entry *__sq_get_by_dstq(u32 dst_queue_id);
static inline void      __sq_free(struct sq_entry *sq);
static void             __sq_del(struct sq_entry *sq);
static s32              __sq_prepare(u32 id);
static void             __sq_stats_update(struct sq_entry *sq);
static void             __sq_next_state(struct sq_entry *sq);
static void             __sq_timers_setup(struct sq_entry *sq);
static void             __sq_queue_connect(struct sq_entry *sq);
static s32              __sq_queue_disconnect(struct sq_entry *sq);
static s32              __sq_queue_alloc(struct sq_entry *sq);
static s32              __sq_queue_add_aqm_context(struct sq_entry *sq);
static s32              __sq_queue_rem_aqm_context(struct sq_entry *sq);

/*****************************************************************************/
/*                      internal sq operations                               */
/*****************************************************************************/
/**
 * @brief return sq_db from smgr_db
 */
static inline struct sq_database *get_sq_db(void)
{
	struct smgr_database *smgr_db = smgr_get_db();

	if (ptr_is_null(smgr_db))
		return NULL;

	return (struct sq_database *)smgr_db->sq_db;
}
/**
 * @brief Acquire syncq lock
 */
static inline void __sq_lock(void)
{
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return;

	pr_debug("caller %ps\n", __builtin_return_address(0));
	/* we are not allowed to run in irq context */
	WARN(in_irq(), "smgr syncq shouldn't be used from irq context\n");
	spin_lock_bh(&db->lock);
}

/**
 * @brief Release syncq lock
 */
static inline void __sq_unlock(void)
{
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return;

	pr_debug("caller %ps\n", __builtin_return_address(0));
	spin_unlock_bh(&db->lock);
}

/**
 * @brief Init the values of sq_state_str to the matching enums
 * @param db sq's db
 */
static inline void __sq_state_str_init(struct sq_database *db)
{
	db->sq_state_str[SQ_STATE_IDLE]      = "IDLE";
	db->sq_state_str[SQ_STATE_ALLOCATED] = "ALLOCATED";
	db->sq_state_str[SQ_STATE_STARTED]   = "STARTED";
	db->sq_state_str[SQ_STATE_LSPP_SENT] = "LSPP_SENT";
	db->sq_state_str[SQ_STATE_SYNC_DONE] = "SYNC_DONE";
	db->sq_state_str[SQ_STATE_NUM]       = "INVALID_STATE";
}

/**
 * @brief Get the syncq state string
 * @param s syncq state
 * @return const char* syncq state string
 */
static inline const char *__sq_state_str(enum sq_state s)
{
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return NULL;

	if (s < SQ_STATE_NUM)
		return db->sq_state_str[s];

	return db->sq_state_str[SQ_STATE_NUM];
}

/**
 * @brief printing a synch queue entry for debug
 * @param sq syncq entry
 */
static inline void __sq_debug(struct sq_entry *sq)
{
	SQ_PR(pr_debug, sq);
}

/**
 * @brief printing a synch queue entry for debug
 * @param sq syncq entry
 */
static inline void __sq_print(struct sq_entry *sq)
{
	SQ_PR(pr_info, sq);
}

/**
 * @brief printing a timer callback event for debug
 */
static inline void __sq_timer_cb_debug(void)
{
	pr_debug("TIME[%llu] timer callback\n", ktime_to_ns(ktime_get()));
}

static inline void __sq_timer_restart_debug(u32 tout)
{
	pr_debug("TIME[%llu] start timer for %u uSEC\n",
		 ktime_to_ns(ktime_get()), tout);
}

/**
 * @brief restart the timer for recive the lspp timeout
 * @param sq syncq entry
 */
static inline void __sq_lspp_rcv_to_restart(struct sq_entry *sq)
{
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return;

	__sq_timer_restart_debug(db->sq_lspp_rcv_to);
	hrtimer_start(&sq->lspp_rcv_timer,
		      ktime_set(0, db->sq_lspp_rcv_to * NSEC_PER_USEC),
		      HRTIMER_MODE_REL_SOFT);
}

/**
 * @brief restart the timer for lspp sent timeout
 * @param sq syncq entry
 */
static inline void __sq_lspp_sent_to_restart(struct sq_entry *sq)
{
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return;

	__sq_timer_restart_debug(db->sq_lspp_sent_to);
	hrtimer_start(&sq->lspp_sent_timer,
		      ktime_set(0, db->sq_lspp_sent_to * NSEC_PER_USEC),
		      HRTIMER_MODE_REL_SOFT);
}

/**
 * @brief restart the timer for done timeout
 * @param sq syncq entry
 */
static inline void __sq_done_to_restart(struct sq_entry *sq)
{
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return;

	__sq_timer_restart_debug(db->sq_done_to);
	hrtimer_start(&sq->done_timer,
		      ktime_set(0, db->sq_done_to * NSEC_PER_USEC),
		      HRTIMER_MODE_REL_SOFT);
}

/**
 * @brief Get the syncq by the session id
 * @param session session id
 * @return struct sq_entry* syncq entry
 */
static struct sq_entry *__sq_get_by_session(u32 session)
{
	struct sq_entry *sq;
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return NULL;

	pr_debug("session %u\n", session);
	SQ_FOREACH_ACTIVE_ENTRY(db, sq) {
		if (sq->session == session)
			return sq;
	}

	return NULL;
}

/**
 * @brief Get the syncq by the hash signature
 * @param sig hash signature
 * @return struct sq_entry* syncq entry
 */
static struct sq_entry *__sq_get_by_hash_sig(u32 sig)
{
	struct sq_entry *sq;
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return NULL;

	pr_debug("hash signature %u\n", sig);
	SQ_FOREACH_ACTIVE_ENTRY(db, sq) {
		if (sq->hash_sig == sig)
			return sq;
	}

	return NULL;
}

/**
 * @brief Get the syncq by the dst queue id
 * @param dst_queue_id dst queue id
 * @return struct sq_entry* syncq entry
 */
static struct sq_entry *__sq_get_by_dstq(u32 dst_queue_id)
{
	struct sq_entry *sq;
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return NULL;

	pr_debug("dst_queue_id %u\n", dst_queue_id);
	SQ_FOREACH_ACTIVE_ENTRY(db, sq) {
		if (sq->dst_queue_id == dst_queue_id)
			return sq;
	}

	return NULL;
}

/**
 * @brief Release the syncq entry
 * @note called under lock
 * @param sq syncq entry
 */
static inline void __sq_free(struct sq_entry *sq)
{
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return;

	__sq_debug(sq);
	/* reset the sync queue properties */
	sq->state        = SQ_STATE_IDLE;
	sq->session      = 0;
	sq->dst_queue_id = 0;
	/* move the entry to the free list */
	SQ_FREE_ENTRY_SET(db, sq);
}

/**
 * @brief Delete the syncq entry
 * @note this API replacing the syncq entry to a FREE entry
 * @param sq syncq entry
 */
static void __sq_del(struct sq_entry *sq)
{
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return;

	__sq_debug(sq);
	switch (sq->state) {
	case SQ_STATE_STARTED:
		sq->state = SQ_STATE_LSPP_SENT;
		/* fallthrough */
	case SQ_STATE_LSPP_SENT:
		__sq_next_state(sq);
		/* fallthrough */
	case SQ_STATE_SYNC_DONE:
		/* let the session handle to use new sq */
		sq->session = U32_MAX;
		/* nothing to do, let the handler to do the job */
		break;
	default:
		/* SQ_STATE_IDLE */
		/* SQ_STATE_ALLOCATED */
		pr_err("sq %hhu invalid state %s\n",
		       sq->id, __sq_state_str(sq->state));
		db->stats.err_invalid_state++;
		break;
	}
}

/**
 * @brief Prepare syncq entry
 * @param id synchq entry id
 * @return s32 0 on success, error code otherwise
 */
static s32 __sq_prepare(u32 id)
{
	struct sq_database *db = get_sq_db();
	struct sq_entry *sq;
	s32 ret;

	if (ptr_is_null(db))
		return -EINVAL;

	sq = &db->sq[id];

	pr_debug("id %u\n", id);
	/* set syncq id */
	sq->id = id;
	/* setup the timers */
	__sq_timers_setup(sq);
	/* init syncq list node */
	INIT_LIST_HEAD(&sq->node);
	/* allocate the QoS resources */
	ret = __sq_queue_alloc(sq);
	if (unlikely(ret)) {
		pr_err("failed to allocate sync queue %u\n", id);
		return ret;
	}
	/* sync queue is ready - initiate the state */
	sq->state = SQ_STATE_IDLE;
	/* add the syncq to the free list */
	SQ_FREE_ENTRY_INIT(db, sq);

	return 0;
}

static void __sq_stats_update(struct sq_entry *sq)
{
	struct pp_qos_queue_stat stat;
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return;

	__sq_debug(sq);
	stat.reset = 1;
	/* get the sync queue statistics */
	if (unlikely(pp_qos_queue_stat_get(db->qdev, sq->queue_id, &stat))) {
		pr_err("failed to get syncq (%u) statistics\n", sq->queue_id);
		return;
	}

	db->stats.packets_accepted  += stat.total_packets_accepted;
	db->stats.bytes_accepted    += stat.total_bytes_accepted;
	db->stats.packets_dropped   += stat.total_packets_dropped;
	db->stats.bytes_dropped     += stat.total_bytes_dropped;
}

/**
 * @brief Take the syncq entry to the next state
 * @param sq syncq entry
 */
static void __sq_next_state(struct sq_entry *sq)
{
	struct sq_database *db = get_sq_db();
	int hrtimer_ret;

	if (ptr_is_null(db))
		return;

	__sq_debug(sq);
	switch (sq->state) {
	case SQ_STATE_IDLE:
		/* the sq was allocated to a new session:
		 * update the new state
		 * mepped all the matched packets to the syncq by updating
		 * the checker synch queues table.
		 */
		sq->state = SQ_STATE_ALLOCATED;
		chk_sq_entry_enable(sq->id, sq->session, sq->queue_phy);
		db->stats.allocated++;
		db->stats.active++;
		break;

	case SQ_STATE_ALLOCATED:
		/* the session was created in HW:
		 * update the new state
		 * start the timer for letting the lspp (Last Slow Path Packet)
		 * the chance to be captured in the CPU tx
		 */
		sq->state = SQ_STATE_STARTED;
		__sq_lspp_rcv_to_restart(sq);
		break;

	case SQ_STATE_STARTED:
		/* lspp (Last Slow Path Packet) recieved:
		 * restart the timer for "sq_lspp_sent_to" time,
		 * let the last packet the chance to be enqueued
		 * update the new state
		 */
		hrtimer_ret = hrtimer_try_to_cancel(&sq->lspp_rcv_timer);
		if (hrtimer_ret != HRTIMER_WAS_ACTIVE)
			pr_debug("sq %hhu lspp_rcv_timer was not active! ret=%d\n",
				  sq->id, hrtimer_ret);
		hrtimer_ret = hrtimer_try_to_cancel(&sq->lspp_sent_timer);
		if (hrtimer_ret != HRTIMER_WAS_NOT_ACTIVE)
			pr_err("sq %hhu lspp_sent_timer was not supposed to be active! ret=%d\n",
			       sq->id, hrtimer_ret);
		__sq_lspp_sent_to_restart(sq);
		sq->state = SQ_STATE_LSPP_SENT;
		break;

	case SQ_STATE_LSPP_SENT:
		/* time to start the synchronization process:
		 * stop the "sync" timer,
		 * update the QoS tree to connect the syncq
		 * mepped all the matched packets to the original dst queue
		 * start the "done" timeout
		 * update the new state
		 */
		hrtimer_ret = hrtimer_try_to_cancel(&sq->lspp_rcv_timer);
		if (hrtimer_ret != HRTIMER_WAS_NOT_ACTIVE)
			pr_debug("sq %hhu lspp_rcv_timer was not active! ret=%d\n",
				 sq->id, hrtimer_ret);
		hrtimer_ret = hrtimer_try_to_cancel(&sq->lspp_sent_timer);
		if (hrtimer_ret != HRTIMER_CB_IS_RUNNING)
			pr_debug("sq %hhu lspp_sent_timer cb isn't running! ret=%d\n",
				 sq->id, hrtimer_ret);

		__sq_queue_connect(sq);
		chk_sq_entry_disable(sq->id);
		sq->state = SQ_STATE_SYNC_DONE;
		__sq_done_to_restart(sq);
		break;

	case SQ_STATE_SYNC_DONE:
		/* start the revert process:
		 * get sync queue statisics
		 * try to revert the syncq
		 * if the operation failed / queue is not empty yet,
		 * then, restart the timer and try next time
		 * release the sq entry
		 */
		__sq_stats_update(sq);
		if (unlikely(__sq_queue_disconnect(sq))) {
			db->stats.err_q_disconnect_failed++;
			__sq_done_to_restart(sq);
			break;
		}
		__sq_queue_rem_aqm_context(sq);
		__sq_free(sq);
		db->stats.freed++;
		db->stats.active--;
		break;

	default:
		pr_err("sq %hhu state %s\n", sq->id, __sq_state_str(sq->state));
		db->stats.err_invalid_state++;
		return;
	}
	__sq_debug(sq);
}

/*****************************************************************************/
/*                       QoS queues operations                               */
/*****************************************************************************/
/**
 * @brief Synchronized the syncq, connect the syncq to the QoS
 *        tree
 * @param sq syncq entry
 */
static void __sq_queue_connect(struct sq_entry *sq)
{
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return;

	__sq_debug(sq);
	if (unlikely(qos_sync_queue_connect(db->qdev, sq->queue_id,
					    sq->dst_queue_id))) {
		pr_crit("failed to synchronized the queues in QoS\n");
	}
}

/**
 * @brief Revert the syncq, disconnect the syncq from the QoS
 *        tree
 * @param sq syncq entry
 * @return s32 0 on success, error code otherwise
 */
static s32 __sq_queue_disconnect(struct sq_entry *sq)
{
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return -EINVAL;

	__sq_debug(sq);
	return qos_sync_queue_disconnect(db->qdev, sq->queue_id);
}

/**
 * @brief Allocate a QoS queue for the syncq entry
 * @param sq syncq entry
 * @return s32 0 on success, error code otherwise
 */
static s32 __sq_queue_alloc(struct sq_entry *sq)
{
	struct pp_qos_queue_conf conf;
	struct sq_database *db = get_sq_db();
	s32 ret;

	if (ptr_is_null(db))
		return -EINVAL;

	__sq_debug(sq);
	/* allocate the queue */
	ret = pp_qos_queue_allocate(db->qdev, &sq->queue_id);
	if (unlikely(ret)) {
		pr_err("failed to allocate QoS queue\n");
		return ret;
	}

	pp_qos_queue_conf_set_default(&conf);
	conf.is_fast_q = 0;
	conf.wred_max_allowed = db->sq_qlen;
	ret = qos_sync_queue_add(db->qdev, sq->queue_id, &sq->queue_phy, &conf);
	if (unlikely(ret)) {
		pr_err("failed to add QoS synch queue %u\n", sq->queue_id);
		return ret;
	}

	pr_debug("sync queue added to QoS, node-id[%u], qid[%u]\n",
		 sq->queue_id, sq->queue_phy);

	return 0;
}

/**
 * @brief Add the syncq to AQM context if needed
 * @param sq syncq entry
 */
static s32 __sq_queue_add_aqm_context(struct sq_entry *sq)
{
	struct sq_database *db = get_sq_db();
	u16 sf_indx;
	s32 ret = 0;

	if (ptr_is_null(db))
		return -EINVAL;

	pp_misc_get_sf_indx_by_q(sq->dst_queue_id, &sf_indx);
	if (sf_indx != PP_QOS_MAX_SERVICE_FLOWS) 
		ret = qos_aqm_q_to_ctx(db->qdev, WRED_CTX_ADD_QUEUE, sq->id, sf_indx);
	
	return ret;
}

/**
 * @brief Remove the syncq from AQM context if needed
 * @param sq syncq entry
 */
static s32 __sq_queue_rem_aqm_context(struct sq_entry *sq)
{
	struct sq_database *db = get_sq_db();
	u16 sf_indx;
	s32 ret = 0;

	if (ptr_is_null(db))
		return -EINVAL;

	pp_misc_get_sf_indx_by_q(sq->dst_queue_id, &sf_indx);
	if (sf_indx != PP_QOS_MAX_SERVICE_FLOWS) 
		ret = qos_aqm_q_to_ctx(db->qdev, WRED_CTX_REM_QUEUE, sq->id, sf_indx);
	
	return ret;
}

/*****************************************************************************/
/*                       timer callback routines                             */
/*****************************************************************************/

/**
 * @brief Done timer handler
 * @param timer the timer object
 */
static enum hrtimer_restart __sq_done_cb_timer(struct hrtimer *timer)
{
	struct sq_entry *sq = container_of(timer, struct sq_entry, done_timer);
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return HRTIMER_NORESTART;

	__sq_timer_cb_debug();
	__sq_debug(sq);
	__sq_lock();
	if (sq->state != SQ_STATE_SYNC_DONE) {
		pr_err("sq %hhu invalid state %s for done event\n",
		       sq->id, __sq_state_str(sq->state));
		db->stats.err_invalid_state++;
		goto unlock;
	}
	__sq_next_state(sq);

unlock:
	__sq_unlock();

	return HRTIMER_NORESTART;
}

/**
 * @brief lspp to timer handler
 * @param timer the timer object
 */
static enum hrtimer_restart __sq_lspp_sent_cb_timer(struct hrtimer *timer)
{
	struct sq_database *db = get_sq_db();
	struct sq_entry *sq =
		container_of(timer, struct sq_entry, lspp_sent_timer);

	if (ptr_is_null(db))
		return HRTIMER_NORESTART;

	__sq_timer_cb_debug();
	__sq_debug(sq);
	__sq_lock();
	if (sq->state == SQ_STATE_LSPP_SENT) {
		__sq_next_state(sq);
	} else {
		pr_err("sq %hhu invalid state %s for sync event\n", sq->id,
		       __sq_state_str(sq->state));
		db->stats.err_invalid_state++;
	}
	__sq_unlock();

	return HRTIMER_NORESTART;
}

/**
 * @brief lspp expired handler
 * @param timer the timer object
 */
static enum hrtimer_restart __sq_lspp_rcv_cb_timer(struct hrtimer *timer)
{
	struct sq_database *db = get_sq_db();
	struct sq_entry *sq =
		container_of(timer, struct sq_entry, lspp_rcv_timer);

	if (ptr_is_null(db))
		return HRTIMER_NORESTART;

	__sq_timer_cb_debug();
	__sq_debug(sq);
	__sq_lock();
	/* in case the lspp already arrived, do nothing */
	if (sq->state == SQ_STATE_LSPP_SENT) {
		pr_debug("sq %hhu lspp arrived in %s state!\n", sq->id,
			 __sq_state_str(sq->state));
		goto unlock;
	}

	if (sq->state == SQ_STATE_STARTED) {
		sq->state = SQ_STATE_LSPP_SENT;
		db->stats.lspp_timeout_events++;
		__sq_next_state(sq);
	} else {
		pr_err("sq %hhu invalid state %s for tout lspp event\n", sq->id,
		       __sq_state_str(sq->state));
		db->stats.err_invalid_state++;
	}
unlock:
	__sq_unlock();

	return HRTIMER_NORESTART;
}

/**
 * @brief Setup the syncq timers
 * @param sq syncq entry
 */
static void __sq_timers_setup(struct sq_entry *sq)
{
	hrtimer_init(&sq->lspp_rcv_timer, CLOCK_MONOTONIC,
		     HRTIMER_MODE_REL_SOFT);
	sq->lspp_rcv_timer.function = __sq_lspp_rcv_cb_timer;

	hrtimer_init(&sq->lspp_sent_timer, CLOCK_MONOTONIC,
		     HRTIMER_MODE_REL_SOFT);
	sq->lspp_sent_timer.function = __sq_lspp_sent_cb_timer;

	hrtimer_init(&sq->done_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL_SOFT);
	sq->done_timer.function = __sq_done_cb_timer;
}

/*****************************************************************************/
/*                           external APIs                                   */
/*****************************************************************************/
s32 sq_alloc(u32 session, u32 dst_queue_id, u32 sig)
{
	struct sq_entry *sq;
	struct sq_database *db = get_sq_db();
	s32 ret = 0;

	if (ptr_is_null(db))
		return -EINVAL;

	pr_debug("session %u, dst_queue_id %u\n", session, dst_queue_id);

	/* lock the syncq db */
	__sq_lock();

	/* if dst queue already hold an entry, abort */
	sq = __sq_get_by_dstq(dst_queue_id);
	if (sq) {
		pr_debug("syncq exist for dst queue %u, abort\n", dst_queue_id);
		ret = -EEXIST;
		goto unlock;
	}

	/* if no available entries, abort */
	if (SQ_IS_DB_FULL(db)) {
		pr_debug("no available sync queue, abort\n");
		ret = -ENOSPC;
		goto unlock;
	}
	/* get a free syncq entry */
	sq = SQ_FREE_ENTRY_GET(db);
	SQ_ACTIVE_ENTRY_SET(db, sq);
	/* set the session info */
	sq->session      = session;
	sq->dst_queue_id = dst_queue_id;
	sq->hash_sig = sig;
	__sq_queue_add_aqm_context(sq);
	/* go to next state --> SQ_STATE_ALLOCATED */
	__sq_next_state(sq);

unlock:
	__sq_unlock();
	return ret;
}

s32 smgr_sq_alloc(struct sess_info *sess)
{
	if (!test_bit(SESS_FLAG_SYNCQ, &sess->db_ent->info.flags))
		return 0;

	return sq_alloc(sess->db_ent->info.sess_id, sess->args->dst_q,
			sess->args->hash.sig);
}

s32 sq_start(u32 session)
{
	struct sq_entry *sq;
	struct sq_database *db = get_sq_db();
	s32 ret = 0;

	if (ptr_is_null(db))
		return -EINVAL;

	pr_debug("session %u\n", session);

	__sq_lock();
	/* check if sq entry was allocated for this session */
	sq = __sq_get_by_session(session);
	if (!sq) {
		/* no attached sync queue, abort */
		ret = -ENOENT;
		goto unlock;
	}

	if (unlikely(sq->state != SQ_STATE_ALLOCATED)) {
		pr_err("invalid sq %hhu state for session %u, state %s\n",
		       sq->id, session, __sq_state_str(sq->state));
		db->stats.err_invalid_state++;
		ret = -EPIPE;
		goto unlock;
	}
	/* go to next state --> SQ_STATE_STARTED */
	__sq_next_state(sq);

unlock:
	__sq_unlock();
	return ret;
}

s32 smgr_sq_start(struct sess_info *sess)
{
	if (!test_bit(SESS_FLAG_SYNCQ, &sess->db_ent->info.flags))
		return 0;

	return sq_start(sess->db_ent->info.sess_id);
}

s32 sq_del(u32 session)
{
	struct sq_entry *sq;
	s32 ret = 0;

	pr_debug("session %u\n", session);

	__sq_lock();
	sq = __sq_get_by_session(session);
	if (!sq) {
		/* no attached sync queue, abort */
		pr_debug("no sync queue for session %u, abort\n", session);
		ret = -ENOENT;
		goto unlock;
	}
	__sq_del(sq);
unlock:
	__sq_unlock();
	return ret;
}

s32 smgr_sq_del(struct sess_db_entry *ent)
{
	if (!test_bit(SESS_FLAG_SYNCQ, &ent->info.flags))
		return 0;

	return sq_del(ent->info.sess_id);
}

void smgr_sq_lspp_rcv(u32 hash_sig)
{
	struct sq_entry *sq;
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return;

	pr_debug("hash_sig %u\n", hash_sig);

	__sq_lock();
	/* check if sq entry exist for this hash */
	sq = __sq_get_by_hash_sig(hash_sig);
	if (unlikely(!sq))
		goto unlock;

	switch (sq->state) {
	case SQ_STATE_ALLOCATED:
		sq->state = SQ_STATE_STARTED;
		/* fallthrough */
	case SQ_STATE_STARTED:
		/* this is the normal flow,
		 * go to next state --> SQ_STATE_LSPP_SENT
		 */
		__sq_next_state(sq);
		break;
	case SQ_STATE_SYNC_DONE:
		/* too late, wait for done event */
		break;
	default:
		pr_err("sq %hhu invalid state %s\n",
		       sq->id, __sq_state_str(sq->state));
		db->stats.err_invalid_state++;
		goto unlock;
	}

unlock:
	__sq_unlock();
}

s32 smgr_sq_dbg_stats_get(struct smgr_sq_stats *stats, bool reset)
{
	u32 active, total;
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return -EINVAL;

	if (stats)
		memcpy(stats, &db->stats, sizeof(*stats));
	if (reset) {
		active = db->stats.active;
		total  = db->stats.total;
		memset(&db->stats, 0, sizeof(db->stats));
		db->stats.active = active;
		db->stats.total  = total;
	}

	return 0;
}

void smgr_sq_dbg_dump(void)
{
	struct sq_database *db = get_sq_db();
	u32 i;

	if (ptr_is_null(db))
		return;

	for (i = 0; i < db->n_sq; i++)
		__sq_print(&db->sq[i]);
}

void smgr_sq_dbg_lspp_rcv_tout_get(u32 *tout)
{
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db) || ptr_is_null(tout))
		return;

	*tout = db->sq_lspp_rcv_to;
}

void smgr_sq_dbg_lspp_rcv_tout_set(u32 tout)
{
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return;

	db->sq_lspp_rcv_to = tout;
}

void smgr_sq_dbg_done_tout_get(u32 *tout)
{
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db) || ptr_is_null(tout))
		return;

	*tout = db->sq_done_to;
}

void smgr_sq_dbg_done_tout_set(u32 tout)
{
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return;

	db->sq_done_to = tout;
}

void smgr_sq_dbg_lspp_sent_tout_get(u32 *tout)
{
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db) || ptr_is_null(tout))
		return;

	*tout = db->sq_lspp_sent_to;
}

void smgr_sq_dbg_lspp_sent_tout_set(u32 tout)
{
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return;

	db->sq_lspp_sent_to = tout;
}

void smgr_sq_dbg_qlen_get(u32 *qlen)
{
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db) || ptr_is_null(qlen))
		return;

	*qlen = db->sq_qlen;
}

void smgr_sq_dbg_qlen_set(u32 qlen)
{
	struct sq_database *db = get_sq_db();

	if (ptr_is_null(db))
		return;

	db->sq_qlen = qlen;
}

s32 smgr_sq_init(struct device *dev)
{
	struct pp_dev_priv *dev_priv;
	struct pp_smgr_init_param *param;
	struct smgr_database *smgr_db = smgr_get_db();
	struct sq_database *db;
	u32 i, n_sq;

	if (ptr_is_null(dev) || ptr_is_null(smgr_db))
		return -EINVAL;

	dev_priv = dev_get_drvdata(dev);
	if (ptr_is_null(dev_priv))
		return -EINVAL;

	param = &dev_priv->dts_cfg.smgr_params;
	n_sq = param->num_syncqs;

	/* Allocate syncq database */
	db = devm_kzalloc(dev, sizeof(*db), GFP_KERNEL);
	if (unlikely(!db)) {
		pr_err("Failed to allocate sync queues databse\n");
		return -ENOMEM;
	}
	/* updating smgr_db to hold db */
	smgr_db->sq_db = db;


	if (n_sq) {
		db->sq = devm_kcalloc(dev, n_sq, sizeof(*db->sq), GFP_KERNEL);
		if (unlikely(!db->sq)) {
			pr_err("Failed to allocate %u sync queues memory\n", n_sq);
			return -ENOMEM;
		}
	}

	/* init lock */
	spin_lock_init(&db->lock);

	/* init syncq lists */
	INIT_LIST_HEAD(&db->free_list);
	INIT_LIST_HEAD(&db->act_list);

	/* get qos device */
	db->qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(ptr_is_null(db->qdev)))
		return -EPERM;

	/* init the timeout values */
	db->sq_lspp_rcv_to  = SQ_LSPP_RCV_TO;
	db->sq_lspp_sent_to = SQ_LSPP_SENT_TO;
	db->sq_done_to      = SQ_DONE_TO;
	db->sq_qlen         = SQ_DFLT_QLEN;

	/* prepare each syncq entry */
	for (i = 0; i < n_sq; i++) {
		if (likely(!__sq_prepare(i)))
			continue;
		pr_notice("failed to allocate %u sync queues, only %hu are supported\n",
			  n_sq, i);
		break;
	}

	/* set the number of sync queues supported */
	db->n_sq        = i;
	db->stats.total = i;

	__sq_state_str_init(db);

	return 0;
}

void smgr_sq_exit(void)
{
	struct device *dev = pp_dev_get();
	struct smgr_database *smgr_db = smgr_get_db();
	struct sq_database *sq_db;
	u32 i;

	if (!dev || !smgr_db || !smgr_db->sq_db)
		return;

	sq_db = smgr_db->sq_db;

	__sq_lock();
	for (i = 0; i < sq_db->n_sq; i++) {
		/* cancel all timers */
		hrtimer_try_to_cancel(&sq_db->sq[i].lspp_rcv_timer);
		hrtimer_try_to_cancel(&sq_db->sq[i].lspp_sent_timer);
		hrtimer_try_to_cancel(&sq_db->sq[i].done_timer);
	}
	__sq_unlock();

	if (sq_db->sq)
		devm_kfree(dev, sq_db->sq);
	devm_kfree(dev, sq_db);
	smgr_db->sq_db = NULL;
}
