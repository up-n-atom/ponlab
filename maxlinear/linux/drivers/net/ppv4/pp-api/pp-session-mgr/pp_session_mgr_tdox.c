/*
 * Copyright (C) 2023-2024 MaxLinear, Inc.
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
 * Description: PP session manager turbodox module
 */

#define pr_fmt(fmt) "[PP_SESS_MGR_TDOX]:%s:%d: " fmt, __func__, __LINE__

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/errno.h>
#include <linux/list.h>
#include <linux/jiffies.h>
#include <net/tcp.h>
#include <linux/pktprs.h>

#include "uc.h"
#include "checker.h"
#include "pp_session_mgr.h"
#include "pp_session_mgr_internal.h"

#define TDOX_DFLT_EN (true)
#define TDOX_SUPP_MAX UC_MAX_TDOX_SESSIONS
#define TDOX_PRIO_MAX (1024)
#define TDOX_CAND_MAX (512)

/* TDOX Evaluation Intervals */
#define TDOX_MIN_EVAL_TIME_SEC (1)
#define TDOX_INIT_EVAL_TIME_SEC (TDOX_MIN_EVAL_TIME_SEC)
#define TDOX_SUPP_EVAL_TIME_SEC (TDOX_MIN_EVAL_TIME_SEC * 3)
#define TDOX_PRIO_EVAL_TIME_SEC (TDOX_MIN_EVAL_TIME_SEC * 10)
#define TDOX_CAND_EVAL_TIME_SEC (TDOX_MIN_EVAL_TIME_SEC * 10)

#define TDOX_SUPP_PPS_LO_THR (600)
#define TDOX_SUPP_EMPT_THR (TDOX_SUPP_MAX >> 1)
#define TDOX_SUPP_FULL_THR (TDOX_SUPP_EMPT_THR + (TDOX_SUPP_EMPT_THR >> 1))

/**
 * @define Number of required entries
 */
#define TDOX_DB_NUM_ENTRIES (TDOX_PRIO_MAX + TDOX_SUPP_MAX + TDOX_CAND_MAX)

/**
 * @define Number of required packets for first evaluation
 */
#define TDOX_MIN_PKTS_TH (32)

/**
 * @brief tdox local parameters
 */
struct tdox_info {
	u32 sess_id;
	u16 supp_id;
	u8 supp_id_alloc; /* is supp id allocated */
	u8 prio_en; /* is prio enabled/supported */
	u32 last_iter;
	u64 last_pkts;
	u64 last_bytes;
	u16 low_queue; /* phys low prio queue from the session */
	u16 high_queue; /* phys high prio queue from the session */
	u16 threshold;
	bool ts;
	bool revert_q;
	bool is_docsis;
	bool lro;
};

enum tdox_state {
	TDOX_FREE, /*! free for allocation */
	TDOX_PEND, /*! pending for session creation */
	TDOX_INIT, /*! in init list */
	TDOX_CAND, /*! in candidate list */
	TDOX_PRIO, /*! in priority list */
	TDOX_SUPP, /*! in suppression list */
	TDOX_LRO,  /*! in lro list */
};

/**
 * @brief tdox database entry definition
 */
struct tdox_entry {
	u16 id;
	enum tdox_state state;
	struct tdox_info info; /* entry info */
	struct list_head node; /* list node */
};

struct tdox_supp_id {
	u16 id;
	struct list_head node;
};

#define TDOX_STATE_STR(s)            \
	((s) == TDOX_FREE ? "FREE" : \
	((s) == TDOX_PEND ? "PEND" : \
	((s) == TDOX_INIT ? "INIT" : \
	((s) == TDOX_CAND ? "CAND" : \
	((s) == TDOX_PRIO ? "PRIO" : \
	((s) == TDOX_SUPP ? "SUPP" : \
	((s) == TDOX_LRO  ? "LRO"  : \
	("Unknown"))))))))

/**
 * @brief tdox database definitions
 */
struct smgr_tdox_db {
	bool enable;
	spinlock_t lock;

	/*! linux delayed work instance */
	struct workqueue_struct *wq;
	struct delayed_work dwork_eval;
	u32 wq_iter;
	bool wq_running;

	/*! data entries */
	struct tdox_entry ent[TDOX_DB_NUM_ENTRIES];
	struct tdox_supp_id supp_ids[TDOX_SUPP_MAX];
	u16 nf_q;
	bool nf_enabled;
	bool supp_full;

	/*! lists */
	struct list_head free_list;
	struct list_head init_list;
	struct list_head supp_list;
	struct list_head prio_list;
	struct list_head cand_list;
	struct list_head lro_list;
	struct list_head free_supp_ids;

	/*! tdox db stats */
	struct smgr_tdox_stats stats;

	/* config interval */
	u32 init_interval;
	u32 supp_interval;
	u32 prio_interval;
	u32 cand_interval;

	/*! tdox params */
	struct smgr_tdox_conf conf;
};

/**
 * @brief return tdox_db from smgr_db
 */
static inline struct smgr_tdox_db *tdox_db_get(void)
{
	struct smgr_database *smgr_db = smgr_get_db();

	if (ptr_is_null(smgr_db))
		return NULL;
	return (struct smgr_tdox_db *)smgr_db->tdox_db;
}

static inline void tdox_ent_dbg(struct tdox_entry *ent)
{
	pr_debug("ent=%u, sess_id=%u, supp_id=%u, state=%s, lowQ[phy]=%u, highQ[phy]=%u, threshold=%u\n",
		 ent->id, ent->info.sess_id, ent->info.supp_id,
		 TDOX_STATE_STR(ent->state), ent->info.low_queue,
		 ent->info.high_queue, ent->info.threshold);
}

static s32 tdox_rec_create(struct smgr_tdox_db *db, struct tdox_entry *ent)
{
	struct tdox_record_create_cmd cmd;
	s32 ret = 0;

	tdox_ent_dbg(ent);
	if (ent->info.supp_id >= TDOX_SUPP_MAX) {
		pr_err("invalid supp id %u\n", ent->info.supp_id);
		return -EINVAL;
	}

	if (!ent->info.lro) {
		cmd.low_queue = ent->info.low_queue;
		cmd.high_queue = ent->info.high_queue;
		cmd.nf_q = db->nf_q;
		cmd.ts_flag = ent->info.ts;
		cmd.is_docsis = ent->info.is_docsis;
	}
	cmd.sess_id = ent->info.sess_id;
	cmd.tdox_id = ent->info.supp_id;
	cmd.lro_flag = ent->info.lro;

	/* TBD - add lro params */
	/* send command */
	ret = uc_egr_mbox_cmd_send(UC_CMD_TDOX_CREATE, 0, (const void *)&cmd,
				   sizeof(cmd), NULL, 0);
	if (ret)
		pr_err("UC_CMD_TDOX_CREATE %u command failed\n", ent->id);

	return ret;
}

static void tdox_rec_remove(struct smgr_tdox_db *db, struct tdox_entry *ent)
{
	struct tdox_record_remove_cmd cmd;

	tdox_ent_dbg(ent);
	if (ent->info.supp_id >= TDOX_SUPP_MAX) {
		pr_err("invalid supp id %u\n", ent->info.supp_id);
		return;
	}

	cmd.tdox_id = ent->info.supp_id;
	cmd.lro_flag = ent->info.lro;

	/* send command */
	if (uc_egr_mbox_cmd_send(UC_CMD_TDOX_REMOVE, 0, (const void *)&cmd,
	    sizeof(cmd), NULL, 0))
		pr_err("UC_CMD_TDOX_REMOVE %u cmd failed\n", ent->info.supp_id);
}

static void tdox_switch_dst_q(struct smgr_tdox_db *db,
			      struct tdox_entry *ent, bool high)
{
	u16 q;

	q = high ? ent->info.high_queue : ent->info.low_queue;
	chk_session_dsi_q_update(ent->info.sess_id, q);
	db->stats.dbg.switch_q++;
}

static s32 tdox_alloc_supp_id(struct smgr_tdox_db *db, struct tdox_entry *ent)
{
	struct tdox_supp_id *supp = NULL;

	supp = list_first_entry_or_null(&db->free_supp_ids,
					struct tdox_supp_id, node);
	if (!supp) {
		pr_debug("no free supp id\n");
		db->stats.dbg.supp_list_full++;
		return -EBUSY;
	}
	__list_del_entry(&supp->node);
	db->stats.supp_ids_cnt++;
	ent->info.supp_id = supp->id;
	ent->info.supp_id_alloc = 1;
	if (!db->supp_full && db->stats.supp_ids_cnt == TDOX_SUPP_FULL_THR) {
		db->supp_full = true;
		db->stats.dbg.supp_full_state++;
		pr_debug("start supp full state\n");
	}

	return 0;
}

static void tdox_supp_id_free(struct smgr_tdox_db *db, struct tdox_entry *ent)
{
	if (!ent->info.supp_id_alloc)
		return;

	/* First TDOX_MAX_NON_AGGRESSIVE_SESSIONS TDOX sessions are regular
	 * sessions, while all the rest are using an aggressive
	 * approach of dropping the pkt instead of storing it.
	 * Since we want to use the non-aggressive mode in higher priority,
	 * we are returning the session id to the beginning
	 * of the list if this is a regular session (alloc is done from head)
	 */
	if (ent->info.supp_id < TDOX_MAX_NON_AGGRESSIVE_SESSIONS)
		list_add(&db->supp_ids[ent->info.supp_id].node,
			 &db->free_supp_ids);
	else
		list_add_tail(&db->supp_ids[ent->info.supp_id].node,
			      &db->free_supp_ids);
	db->stats.supp_ids_cnt--;
	ent->info.supp_id_alloc = 0;
	ent->info.supp_id = TDOX_SUPP_MAX;
	if (db->supp_full && db->stats.supp_ids_cnt == TDOX_SUPP_EMPT_THR) {
		db->supp_full = false;
		pr_debug("stop supp full state\n");
	}
}

static void tdox_clean_ent(struct tdox_entry *ent)
{
	memset(&ent->info, 0, sizeof(struct tdox_info));
	ent->info.sess_id = U32_MAX;
	ent->info.supp_id = TDOX_SUPP_MAX;
}

static void tdox_del_from_list(struct smgr_tdox_db *db, struct tdox_entry *ent)
{
	switch (ent->state) {
	case TDOX_PEND:
		return;
	case TDOX_FREE:
		pr_debug("remove from free list\n");
		db->stats.free_list_cnt--;
		break;
	case TDOX_INIT:
		pr_debug("remove from init list\n");
		db->stats.init_list_cnt--;
		break;
	case TDOX_SUPP:
		pr_debug("remove from supp list\n");
		db->stats.supp_list_cnt--;
		/* in session deletion, no need to switch the queue */
		if (ent->info.revert_q)
			tdox_switch_dst_q(db, ent, false);
		tdox_rec_remove(db, ent);
		tdox_supp_id_free(db, ent);
		break;
	case TDOX_PRIO:
		pr_debug("remove from prio list\n");
		db->stats.prio_list_cnt--;
		/* in session deletion, no need to switch the queue */
		if (ent->info.revert_q)
			tdox_switch_dst_q(db, ent, false);
		ent->info.prio_en = 0;
		break;
	case TDOX_CAND:
		pr_debug("remove from cand list\n");
		db->stats.cand_list_cnt--;
		break;
	case TDOX_LRO:
		pr_debug("remove from lro list\n");
		db->stats.lro_list_cnt--;
		tdox_rec_remove(db, ent);
		tdox_supp_id_free(db, ent);
		break;
	default:
		pr_err("invalid src list\n");
		return;
	}

	/* delete from current list */
	__list_del_entry(&ent->node);
}

static void tdox_ent_free(struct smgr_tdox_db *db, struct tdox_entry *ent)
{
	tdox_del_from_list(db, ent);
	tdox_supp_id_free(db, ent);
	db->stats.free_list_cnt++;
	tdox_clean_ent(ent);
	list_add_tail(&ent->node, &db->free_list);
	ent->state = TDOX_FREE;
}

s32 smgr_tdox_session_stats_get(u32 id, struct pp_stats *stats)
{
	struct pp_dsi dsi;
	s32 ret;

	if (ptr_is_null(stats))
		return -EINVAL;

	/* decode the si */
	ret = smgr_session_dsi_get(id, &dsi);
	if (ret)
		return ret;
	stats->bytes   = dsi.bytes_cnt;
	stats->packets = dsi.pkts_cnt;

	return 0;
}

static s32 tdox_avg_pkt_sz_get(struct smgr_tdox_db *db, struct tdox_entry *ent,
			       u32 *avg_psz, u64 *pkts, u32 *pps)
{
	struct pp_stats stats;
	u64 bytes;
	u32 iter_sec;

	if (!db || !ent || !avg_psz || !pkts || !pps)
		return -EINVAL;

	if (smgr_tdox_session_stats_get(ent->info.sess_id, &stats)) {
		pr_err("failed to get session %d stats\n", ent->info.sess_id);
		/* free entry */
		tdox_ent_free(db, ent);
		return -EINVAL;
	}

	/* find avg packets size */
	*pkts = stats.packets - ent->info.last_pkts;
	bytes = stats.bytes - ent->info.last_bytes;
	*avg_psz = *pkts ? bytes / *pkts : 0;
	/* find packets per second */
	iter_sec = (db->wq_iter - ent->info.last_iter) / TDOX_MIN_EVAL_TIME_SEC;
	*pps = iter_sec ? *pkts / iter_sec : 0;

	ent->info.last_iter = db->wq_iter;
	ent->info.last_pkts = stats.packets;
	ent->info.last_bytes = stats.bytes;

	pr_debug("ent id %u pkts %llu bytes %llu avg_psz %d pps %d\n",
		 ent->id, *pkts, bytes, *avg_psz, *pps);

	return 0;
}

static bool is_supp_pps_qualify(struct smgr_tdox_db *db, u32 pps)
{
	pr_debug("pps=%u\n", pps);

	/* in supp_full state, pps threshold is enforced, to utilize suppressed
		entries with entries which has higher ack rate and can better benefite tdox */
	if (db->supp_full && pps < TDOX_SUPP_PPS_LO_THR)
		return false;

	return true;
}

static s32 tdox_supp_add(struct smgr_tdox_db *db, struct tdox_entry *ent,
			 u32 pps)
{
	pr_debug("add to supp list\n");

	if (!ent->info.supp_id_alloc || !db->nf_enabled)
		return -EPERM;

	if (!is_supp_pps_qualify(db, pps)) {
		/* if entry doesn't get suppressed, no need to hold the supp_id allocation */
		tdox_supp_id_free(db, ent);
		return -EPERM;
	}

	if (tdox_rec_create(db, ent))
		return -EINVAL;

	ent->info.revert_q = false;
	tdox_del_from_list(db, ent);
	ent->info.revert_q = true;
	db->stats.supp_list_cnt++;
	list_add_tail(&ent->node, &db->supp_list);
	ent->state = TDOX_SUPP;

	return 0;
}

static s32 tdox_prio_add(struct smgr_tdox_db *db, struct tdox_entry *ent)
{
	struct pp_hw_si hw_si = { 0 };
	struct pp_si si = { 0 };

	pr_debug("add to prio list\n");

	if (!ent->info.prio_en)
		return -EPERM;
	if (db->stats.prio_list_cnt >= TDOX_PRIO_MAX) {
		db->stats.dbg.prio_list_full++;
		return -EPERM;
	}
	
	/* 	entry which moves to prioriy state, needs to update the session info
		with dest queue to high queue by turning off the psi bit in case of docsis */
	if (ent->info.is_docsis) {
		/* Get session SI */
		if (unlikely(smgr_session_si_get(ent->info.sess_id, &si))) {
			pr_err("couldn't get si for session %u", ent->info.sess_id);
			return -EINVAL;
		}
		
		if (si.si_ps_sz) /* verify psi exist */
		{
			si.ud[0] &= ~1;
			/* encode si to hw si */
			if (unlikely(pp_si_encode(&hw_si, &si))) {
				pr_err("couldn't encode si for session %u", ent->info.sess_id);
				return -EINVAL;
			}
			/* update session with new hw si */
			if (unlikely(__smgr_session_update(ent->info.sess_id, &hw_si))) {
				pr_err("couldn't update hw si for session %u", ent->info.sess_id);
				return -EINVAL;
			}
		}
	}

	tdox_switch_dst_q(db, ent, true);
	ent->info.revert_q = false; /* queue already updated above */
	tdox_del_from_list(db, ent);
	ent->info.revert_q = true;
	db->stats.prio_list_cnt++;
	list_add_tail(&ent->node, &db->prio_list);
	ent->state = TDOX_PRIO;

	return 0;
}

static s32 tdox_cand_add(struct smgr_tdox_db *db, struct tdox_entry *ent)
{
	pr_debug("add to cand list\n");
	if (db->stats.cand_list_cnt >= TDOX_CAND_MAX) {
		db->stats.dbg.cand_list_full++;
		return -EPERM;
	}

	/* note: in case entry moves to candidate from priority, the queue should be seto to low queue
		however since in the next cycle of tdox_eval_cand_list() the entry either :
			1. moves to suppress - currently not support, see note in tdox_eval_prio_list().
			2. moves to priority - then the queue will be raised to high anyway.
			3. entry is freed if none of the above happens - no meaning to set the queue.
		so currenly the queue is not updated since the entry stay in tdox_eval_cand_list() one
		cycle at most and then one of the above will occur anyway */

	tdox_del_from_list(db, ent);
	db->stats.cand_list_cnt++;
	list_add_tail(&ent->node, &db->cand_list);
	ent->state = TDOX_CAND;

	return 0;
}

static s32 tdox_lro_add(struct smgr_tdox_db *db, struct tdox_entry *ent)
{
	pr_debug("add to lro list\n");

	if (tdox_rec_create(db, ent)) {
		pr_info("failed to create record\n");
		return -EINVAL;
	}

	db->stats.lro_list_cnt++;
	list_add_tail(&ent->node, &db->lro_list);
	ent->state = TDOX_LRO;

	return 0;
}

static void tdox_ent_init(struct smgr_tdox_db *db, struct tdox_entry *ent)
{
	db->stats.init_list_cnt++;
	list_add_tail(&ent->node, &db->init_list);
	ent->state = TDOX_INIT;
}

/**
 * @brief Going through all new TCP sessions for first time, In case the after
 * the first 32 packets the average packet size is below the threshold:
 * 1. if applicable to suppression - add to suppression
 * 2. if applicable to priority - add to priority
 * 3. if packet average size is above threshold - add to candidate list
 * @param db tdox db
 */
static void tdox_eval_init_list(struct smgr_tdox_db *db)
{
	struct tdox_entry *ent, *tmp;
	u32 pps, avg_psz = 0;
	u64 pkts;
	bool ack;

	spin_lock_bh(&db->lock);
	/* Iterate through init list */
	list_for_each_entry_safe(ent, tmp, &db->init_list, node) {
		if (tdox_avg_pkt_sz_get(db, ent, &avg_psz, &pkts, &pps))
			continue;
		if (pkts < TDOX_MIN_PKTS_TH)
			continue; /* keep for next time */

		ack = avg_psz > ent->info.threshold ? false : true;
		if (ack && !tdox_supp_add(db, ent, pps))
			continue; /* moved to supp */
		else if (ack && !tdox_prio_add(db, ent))
			continue; /* moved to prio */
		else if (!tdox_cand_add(db, ent))
			continue; /* moved to cand */
		tdox_ent_free(db, ent);
	}
	spin_unlock_bh(&db->lock);
}

/**
 * @brief Going through all tdox entries in suppression list
 * If not applicable to suppression anymore - move to candidate list
 * @param db tdox db
 */
static void tdox_eval_supp_list(struct smgr_tdox_db *db)
{
	struct tdox_entry *ent, *tmp;
	u32 pps, avg_psz = 0;
	u64 pkts;
	bool ack;

	spin_lock_bh(&db->lock);
	list_for_each_entry_safe(ent, tmp, &db->supp_list, node) {
		if (ent->info.last_iter == db->wq_iter)
			continue; /* skip in case its just inserted */
		if (tdox_avg_pkt_sz_get(db, ent, &avg_psz, &pkts, &pps))
			continue;

		ack = avg_psz > ent->info.threshold ? false : true;
		if (ack && is_supp_pps_qualify(db, pps))
			continue; /* keep in supp */
		else if (ack && !tdox_prio_add(db, ent))
			continue; /* moved to prio */
		else if (!tdox_cand_add(db, ent))
			continue; /* moved to cand */
		tdox_ent_free(db, ent);
	}
	spin_unlock_bh(&db->lock);
}

/**
 * @brief Going through all tdox entries in priority list
 * If not applicable to priority anymore - move to candidate list
 * If applicable tp suppression and free supp entry available - add to supp
 * @param db tdox db
 */
static void tdox_eval_prio_list(struct smgr_tdox_db *db)
{
	struct tdox_entry *ent, *tmp;
	u32 pps, avg_psz = 0;
	u64 pkts;
	bool ack;

	spin_lock_bh(&db->lock);
	/* Iterate through priority list */
	list_for_each_entry_safe(ent, tmp, &db->prio_list, node) {
		if (ent->info.last_iter == db->wq_iter)
			continue; /* skip in case its just inserted */
		if (tdox_avg_pkt_sz_get(db, ent, &avg_psz, &pkts, &pps))
			continue;

		ack = avg_psz > ent->info.threshold ? false : true;
		if (ack && !tdox_supp_add(db, ent, pps))
			/* TBD - not supported since supp_id allocation was released when moving to prio,
				currently from prio entries can't move back to suppressed */
			continue; /* moved to supp */
		else if (ack)
			continue; /* keep in prio */
		else if (!tdox_cand_add(db, ent))
			continue; /* moved to cand */
		tdox_ent_free(db, ent);
	}
	spin_unlock_bh(&db->lock);
}

/**
 * @brief Going through all tdox entries in candidate list,
 * If applicable to suppression - add to suppression
 * If applicable to priority - add to priority
 * @param db tdox db
 */
static void tdox_eval_cand_list(struct smgr_tdox_db *db)
{
	struct tdox_entry *ent, *tmp;
	u32 pps, avg_psz = 0;
	u64 pkts;
	bool ack;

	spin_lock_bh(&db->lock);
	list_for_each_entry_safe(ent, tmp, &db->cand_list, node) {
		if (ent->info.last_iter == db->wq_iter)
			continue; /* skip in case its just inserted */
		if (tdox_avg_pkt_sz_get(db, ent, &avg_psz, &pkts, &pps))
			continue;

		ack = avg_psz > ent->info.threshold ? false : true;
		if (ack && !tdox_supp_add(db, ent, pps))
			continue; /* moved to supp */
		else if (ack && !tdox_prio_add(db, ent))
			continue; /* moved to prio */
		else
			/* from candidate the entry either moved to supp/pri or deleted */
			tdox_ent_free(db, ent);
	}
	spin_unlock_bh(&db->lock);
}

static void tdox_restart_work(struct smgr_tdox_db *db)
{
	db->wq_running = true;
	queue_delayed_work(db->wq, &db->dwork_eval,
			   msecs_to_jiffies(TDOX_MIN_EVAL_TIME_SEC * 1000));
}

/**
 * @brief Main TCP Ack Filtering workq,
 * Monitor the tdox entries and move between lists
 * @param work work
 */
static void tdox_eval_work(struct work_struct *work)
{
	struct smgr_tdox_db *db = tdox_db_get();

	if (ptr_is_null(db))
		return;

	db->wq_iter++;

	if ((db->wq_iter % db->init_interval) == 0)
		tdox_eval_init_list(db);
	if ((db->wq_iter % db->supp_interval) == 0)
		tdox_eval_supp_list(db);
	if ((db->wq_iter % db->prio_interval) == 0)
		tdox_eval_prio_list(db);
	if ((db->wq_iter % db->cand_interval) == 0)
		tdox_eval_cand_list(db);

	spin_lock_bh(&db->lock);
	if (db->stats.free_list_cnt == TDOX_DB_NUM_ENTRIES)
		db->wq_running = false;
	else
		tdox_restart_work(db);
	spin_unlock_bh(&db->lock);
}

bool smgr_tdox_enable_get(void)
{
	struct smgr_tdox_db *db = tdox_db_get();
	bool en;

	if (!db)
		return false;

	spin_lock_bh(&db->lock);
	en = db->enable;
	spin_unlock_bh(&db->lock);

	return en;
}

static void tdox_disable_all_entries(struct smgr_tdox_db *db)
{
	struct tdox_entry *ent, *tmp;

	/* go over all lists and move to free list */
	list_for_each_entry_safe(ent, tmp, &db->init_list, node)
		tdox_ent_free(db, ent);
	list_for_each_entry_safe(ent, tmp, &db->supp_list, node)
		tdox_ent_free(db, ent);
	list_for_each_entry_safe(ent, tmp, &db->prio_list, node)
		tdox_ent_free(db, ent);
	list_for_each_entry_safe(ent, tmp, &db->cand_list, node)
		tdox_ent_free(db, ent);
	list_for_each_entry_safe(ent, tmp, &db->lro_list, node)
		tdox_ent_free(db, ent);
}

void smgr_tdox_enable_set(bool enable)
{
	struct smgr_tdox_db *db = tdox_db_get();

	if (ptr_is_null(db))
		return;

	spin_lock_bh(&db->lock);
	if (!enable)
		tdox_disable_all_entries(db);
	db->enable = enable;
	spin_unlock_bh(&db->lock);
}

s32 smgr_tdox_start(struct sess_info *s)
{
	struct smgr_tdox_db *db = tdox_db_get();
	u16 tdox_id = s->db_ent->info.tdox_id;

	if (tdox_id >= TDOX_DB_NUM_ENTRIES)
		return 0;

	if (ptr_is_null(db))
		return -EINVAL;

	spin_lock_bh(&db->lock);
	if (!db->enable)
		goto unlock;
	if (db->ent[tdox_id].state == TDOX_LRO)
		goto unlock;
	tdox_ent_init(db, &db->ent[tdox_id]);
	if (!db->wq_running)
		tdox_restart_work(db);
unlock:
	spin_unlock_bh(&db->lock);

	return 0;
}

static struct tdox_entry *tdox_entry_alloc(struct smgr_tdox_db *db)
{
	struct tdox_entry *ent;

	ent = list_first_entry_or_null(&db->free_list, struct tdox_entry, node);
	if (!ent) {
		pr_debug("no free tdox entry\n");
		return NULL;
	}
	__list_del_entry(&ent->node);
	db->stats.free_list_cnt--;
	ent->state = TDOX_PEND;

	return ent;
}

static bool tdox_tcp_hdr_info_get(struct sess_info *s, u16 *thr, u8 *ts)
{
	struct tcphdr *tcph;
	s16 hdr_sz, optlen;
	u8 hdr_lvl;
	u8 *ptr;

	if (SESS_RX_IS_OUTER_TCP(s))
		hdr_lvl = HDR_OUTER;
	else if (SESS_RX_IS_INNER_TCP(s))
		hdr_lvl = HDR_INNER;
	else
		return -EINVAL;

	hdr_sz = pktprs_hdr_sz(SESS_RX_PKT(s), PKTPRS_PROTO_TCP, hdr_lvl);
	if (!hdr_sz)
		return -EINVAL;
	*thr = hdr_sz + 40;

	/* Suppression support only timestamp TCP opt (nop-nop-ts) */
	tcph = pktprs_tcp_hdr(SESS_RX_PKT(s), hdr_lvl);
	if (!tcph)
		return -EINVAL;
	optlen = (tcph->doff * 4) - sizeof(struct tcphdr);
	if (optlen != TCPOLEN_TSTAMP_ALIGNED)
		return 0;
	ptr = (u8 *)(tcph + 1);
	if (*(__be32 *)ptr == htonl((TCPOPT_NOP << 24) |
				    (TCPOPT_NOP << 16) |
				    (TCPOPT_TIMESTAMP << 8) |
				    TCPOLEN_TIMESTAMP))
		*ts = 1;

	return 0;
}

static void tdox_entry_prepare(struct sess_info *s)
{
	struct smgr_tdox_db *db = tdox_db_get();
	struct tdox_entry *ent;
	u16 phyq, thr;
	u8 ts;

	if (ptr_is_null(db) || !db->enable)
		return;

	if (tdox_tcp_hdr_info_get(s, &thr, &ts)) {
		pr_debug("failed to get the tcp header info\n");
		return;
	}

	spin_lock_bh(&db->lock);
	ent = tdox_entry_alloc(db);
	if (!ent)
		goto unlock;

	ent->info.high_queue = s->si.dst_q;

	if (SESS_ARGS_IS_FLAG_ON(s, PP_SESS_FLAG_SLRO_INFO_BIT)) {
		if (!tdox_alloc_supp_id(db, ent)) {
			if (ent->info.supp_id >= 32) {
				pr_debug("No lro record!!!\n");
				tdox_ent_free(db, ent);
				goto unlock;
			}
			SESS_FLAG_SET(s->db_ent, SESS_FLAG_LRO);
			s->si.tdox_flow = ent->info.supp_id;
			ent->info.lro = true;
			ent->info.sess_id = s->db_ent->info.sess_id;
			s->db_ent->info.tdox_id = ent->id;
			if (tdox_lro_add(db, ent)) {
				tdox_ent_free(db, ent);
				goto unlock;
			}
			s->si.dst_q = db->nf_q;
		} else {
			/* no supp entry, release entry */
			tdox_ent_free(db, ent);
			goto unlock;
		}
	}

	if (SESS_ARGS_IS_FLAG_ON(s, PP_SESS_FLAG_TDOX_PRIO_BIT) &&
	    s->args->dst_q_high != s->args->dst_q &&
	    s->args->dst_q_high != -1) {
		if (smgr_get_queue_phy_id(s->args->dst_q_high, &phyq)) {
			tdox_ent_free(db, ent);
			goto unlock;
		}
		ent->info.high_queue = phyq;
		ent->info.prio_en = 1;
	}

	if (SESS_ARGS_IS_FLAG_ON(s, PP_SESS_FLAG_TDOX_SUPP_BIT)) {
		if (SESS_ARGS_IS_FLAG_ON(s, PP_SESS_FLAG_DOCSIS_BIT))
			ent->info.is_docsis = true;

		if (!tdox_alloc_supp_id(db, ent)) {
			SESS_FLAG_SET(s->db_ent, SESS_FLAG_TDOX_SUPP);
			s->si.tdox_flow = ent->info.supp_id;
		} else if (!ent->info.prio_en || ent->info.is_docsis) {
			/* no supp entry and prio disabled, release entry */
			tdox_ent_free(db, ent);
			goto unlock;
		}
	}
	ent->info.low_queue = s->si.dst_q; /* phy id */
	ent->info.threshold = thr;
	ent->info.ts = !!ts;
	ent->info.sess_id = s->db_ent->info.sess_id;
	s->db_ent->info.tdox_id = ent->id;
unlock:
	spin_unlock_bh(&db->lock);
}

void smgr_si_tdox_set(struct sess_info *s)
{
	if (ptr_is_null(s))
		return;

	s->db_ent->info.tdox_id = U16_MAX;
	if (SESS_ARGS_IS_FLAG_ON(s, PP_SESS_FLAG_LRO_INFO_BIT)) {
		s->si.tdox_flow = s->args->lro_info;
		return;
	}
	s->si.tdox_flow = SMGR_INVALID_TDOX_SESS_ID;
	if (s->si.seg_en)
		return; /* not allowed for segmented buffers */

	if (SESS_ARGS_IS_FLAG_OFF(s, PP_SESS_FLAG_TDOX_SUPP_BIT) &&
	    SESS_ARGS_IS_FLAG_OFF(s, PP_SESS_FLAG_TDOX_PRIO_BIT) &&
		SESS_ARGS_IS_FLAG_OFF(s, PP_SESS_FLAG_SLRO_INFO_BIT))
			return; /* tdox and lro disabled */

	if (!SESS_RX_PKT(s) || !SESS_TX_PKT(s))
		return; /* no parsing */

	if (!SESS_RX_IS_OUTER_TCP(s) && !SESS_RX_IS_INNER_TCP(s))
		return; /* not TCP */

	if (pktprs_first_frag(SESS_RX_PKT(s), HDR_OUTER) ||
	    pktprs_first_frag(SESS_RX_PKT(s), HDR_INNER) ||
	    pktprs_first_frag(SESS_TX_PKT(s), HDR_OUTER) ||
	    pktprs_first_frag(SESS_TX_PKT(s), HDR_INNER))
		return; /* frag not supported */

	if (SESS_ARGS_IS_FLAG_ON(s, PP_SESS_FLAG_DOCSIS_BIT) &&
	   !SESS_ARGS_IS_FLAG_ON(s, PP_SESS_FLAG_TDOX_SUPP_BIT))
		return; /* for docsis port: not support prio without supp */

	tdox_entry_prepare(s);
}

void smgr_tdox_session_remove(struct sess_db_entry *sess_db)
{
	struct smgr_tdox_db *db = tdox_db_get();
	u16 tid;

	if (ptr_is_null(sess_db) || ptr_is_null(db))
		return;

	tid = sess_db->info.tdox_id;
	if (tid >= TDOX_DB_NUM_ENTRIES)
		return;

	spin_lock_bh(&db->lock);
	if (sess_db->info.sess_id != db->ent[tid].info.sess_id)
		goto unlock; /* entry already recycled by the eval process */
	/* session removed, no point to udpate the queue */
	db->ent[tid].info.revert_q = false;
	tdox_ent_free(db, &db->ent[tid]);
unlock:
	spin_unlock_bh(&db->lock);
}

s32 smgr_tdox_conf_set(struct smgr_tdox_conf *conf)
{
	struct smgr_tdox_db *db = tdox_db_get();
	s32 ret = 0;

	if (ptr_is_null(db))
		return -EINVAL;

	/* update egress db */
	ret = uc_egr_mbox_cmd_send(UC_CMD_TDOX_CONFIG_SET, 0, (const void *)conf,
				   sizeof(*conf), NULL, 0);

	if (ret)
		goto err;

	/* Update local db */
	spin_lock_bh(&db->lock);
	memcpy(&db->conf, conf, sizeof(*conf));
	spin_unlock_bh(&db->lock);

	return 0;

err:
	pr_err("failed to set tdox timer config\n");
	return ret;
}

s32 smgr_tdox_conf_get(struct smgr_tdox_conf *conf)
{
	struct smgr_tdox_db *db = tdox_db_get();

	if (ptr_is_null(conf))
		return -EINVAL;

	if (ptr_is_null(db))
		return -EINVAL;

	spin_lock_bh(&db->lock);
	memcpy(conf, &db->conf, sizeof(*conf));
	spin_unlock_bh(&db->lock);

	return 0;
}

s32 smgr_tdox_debug_read_entry(struct seq_file *f, u32 index)
{
	struct smgr_tdox_db *db = tdox_db_get();
	struct tdox_entry *ent;
	u32 start = 0;
	u32 end = TDOX_DB_NUM_ENTRIES;
	u16 supp = 0, cand = 0, init = 0, supp_aggr = 0, supp_norm = 0;
	u16 lro = 0, pend = 0, prio = 0, free = 0;

	if (ptr_is_null(db))
		return -EINVAL;

	if (index != U32_MAX) {
		if (index >= TDOX_DB_NUM_ENTRIES)
			return -EINVAL;
		start = index;
		end = index + 1;
	}

	for (; start < end; start++) {
		ent = &db->ent[start];
		switch (ent->state) {
			case TDOX_FREE:
				free++;
				continue;
			case TDOX_PEND:
				pend++;
				break;
			case TDOX_INIT:
				init++;
				break;
			case TDOX_CAND:
				cand++;
				break;
			case TDOX_PRIO:
				prio++;
				break;
			case TDOX_SUPP:
				supp++;
				if (ent->info.supp_id < TDOX_MAX_NON_AGGRESSIVE_SESSIONS)
					supp_norm++;
				else
					supp_aggr++;
				break;
			case TDOX_LRO:
				lro++;
				break;
			default:
				break;
		}
		seq_printf(f,
			"ent=%-4u, sess_id=%-5u, supp_id=%-3u[%-4s], state=%-4s, lowQ[phy]=%-3u, "
			"highQ[phy]=%-3u, threshold=%-3u, is_docsis=%u\n",
			ent->id, ent->info.sess_id, ent->info.supp_id,
			(ent->info.supp_id == TDOX_SUPP_MAX ?
				" NA " : (ent->info.supp_id < TDOX_MAX_NON_AGGRESSIVE_SESSIONS ?
				"Norm" : "Aggr")),
			TDOX_STATE_STR(ent->state), ent->info.low_queue,
			ent->info.high_queue, ent->info.threshold, ent->info.is_docsis);
		}
		seq_printf(f,
			"\nTotal: active:%u | free=%u pend=%u init=%u cand=%u prio=%u "
			"supp=%u (normal=%u aggressive=%u) lro=%u | supp full state=%u\n",
			(init + cand + prio + supp), free, pend, init, cand, prio, supp,
			supp_norm, supp_aggr, lro, db->supp_full);
	return 0;
}

s32 smgr_tdox_stats_get(struct smgr_tdox_stats *stats,
			struct smgr_tdox_uc_stats *uc_stats, bool reset_stats)
{
	struct smgr_tdox_db *db = tdox_db_get();

	if (ptr_is_null(db) || ptr_is_null(stats) || ptr_is_null(uc_stats))
		return -EINVAL;

	if (sizeof(struct smgr_tdox_uc_stats) !=
			sizeof(struct tdox_uc_stats)) {
		pr_err("failed to get tdox counters, size missmatch\n");
		return -EINVAL;
	}

	if (uc_egr_mbox_cmd_send(UC_CMD_TDOX_STATS, reset_stats, NULL, 0,
				 uc_stats, sizeof(*uc_stats)))
		pr_err("failed to get tdox counters\n");

	if (reset_stats) {
		uc_egr_tdox_stats_reset(uc_stats->dccm_addr, sizeof(*uc_stats));
		memset(&db->stats.dbg, 0, sizeof(struct tdox_dbg_stats));
		return 0;
	}
	memcpy(stats, &db->stats, sizeof(*stats));

	return 0;
}

void smgr_tdox_nf_set(u16 phyq)
{
	struct smgr_tdox_db *db = tdox_db_get();

	if (ptr_is_null(db))
		return;

	spin_lock_bh(&db->lock);
	db->nf_q = phyq;
	db->nf_enabled = true;
	spin_unlock_bh(&db->lock);
}

s32 smgr_tdox_tout_set(u32 tout)
{
	struct smgr_tdox_db *db = tdox_db_get();
	struct smgr_tdox_conf conf;

	if (ptr_is_null(db))
		return -EINVAL;

	if (smgr_tdox_conf_get(&conf)) {
		pr_err("failed to get tdox config\n");
		return -EINVAL;
	}

	if (tout >= TDOX_TIMER_MAX_TIMEOUT_USEC) {
		pr_err("invalid tdox timeout %u\n", tout);
		return -EINVAL;
	}

	conf.timeout = tout;

	return smgr_tdox_conf_set(&conf);
}

s32 smgr_tdox_init(struct device *dev, struct smgr_database *smgr_db)
{
	struct smgr_tdox_db *db;
	u32 i, flags = WQ_MEM_RECLAIM | WQ_UNBOUND;

	if (ptr_is_null(dev) || ptr_is_null(smgr_db))
		return -EINVAL;

	/* allocate tdox db */
	db = devm_kzalloc(dev, sizeof(*db), GFP_KERNEL);
	if (!db) {
		pr_err("Failed to allocate database memory\n");
		return -ENOMEM;
	}
	db->wq = alloc_workqueue("%s", flags, 2, "TDOX WQ");
	if (!db->wq) {
		pr_err("Failed to create TDOX work queue");
		return -ENOMEM;
	}

	/* updating smgr_db to hold db */
	smgr_db->tdox_db = db;

	spin_lock_init(&db->lock);
	INIT_LIST_HEAD(&db->free_list);
	INIT_LIST_HEAD(&db->free_supp_ids);
	INIT_LIST_HEAD(&db->init_list);
	INIT_LIST_HEAD(&db->supp_list);
	INIT_LIST_HEAD(&db->prio_list);
	INIT_LIST_HEAD(&db->cand_list);
	INIT_LIST_HEAD(&db->lro_list);

	/* init suppression db entries */
	for (i = 0; i < TDOX_SUPP_MAX; i++) {
		INIT_LIST_HEAD(&db->supp_ids[i].node);
		db->supp_ids[i].id = i;
		list_add_tail(&db->supp_ids[i].node, &db->free_supp_ids);
	}
	/* init tdox db entries */
	for (i = 0; i < TDOX_DB_NUM_ENTRIES; i++) {
		/* init sessions lists nodes */
		INIT_LIST_HEAD(&db->ent[i].node);
		db->ent[i].id = i;
		db->ent[i].state = TDOX_PEND;
		tdox_ent_free(db, &db->ent[i]);
	}

	db->init_interval = TDOX_INIT_EVAL_TIME_SEC;
	db->supp_interval = TDOX_SUPP_EVAL_TIME_SEC;
	db->prio_interval = TDOX_PRIO_EVAL_TIME_SEC;
	db->cand_interval = TDOX_CAND_EVAL_TIME_SEC;
	INIT_DELAYED_WORK(&db->dwork_eval, tdox_eval_work);
	db->wq_running = false;
	db->enable = TDOX_DFLT_EN;

	db->conf.timeout          = TDOX_TIMER_TIMEOUT_USEC;
	db->conf.max_reach_target = TDOX_MAX_REACH_TARGET;
	db->conf.max_supp_ratio   = TDOX_MAX_SUPP_RATIO;
	db->conf.max_supp_bytes   = TDOX_MAX_SUPP_BYTES;

	return 0;
}

void smgr_tdox_exit(void)
{
	struct device *dev = pp_dev_get();
	struct smgr_database *smgr_db = smgr_get_db();
	struct smgr_tdox_db *db = tdox_db_get();

	if (!dev || !smgr_db || !db)
		return;

	if (db->wq)
		destroy_workqueue(db->wq);
	devm_kfree(dev, db);
	smgr_db->tdox_db = NULL;
}
