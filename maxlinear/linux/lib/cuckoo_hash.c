/*
 * Copyright (C) 2020-2021 MaxLinear, Inc.
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
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: Cuckoo Hash Implementation
 */

#include <linux/types.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/hash.h>
#include <linux/jhash.h>
#include <linux/cuckoo_hash.h>

/**
 * @brief used to mark the user item during insert process
 */
#define INVALID_SLOT_IDX             (CUCKOO_HASH_MAX_BKT_SIZE + 1)

/**
 * @brief goes trough all the active slots in bucket
 */
#define BKT_FOREACH_ACT_SLOT(s, b)   list_for_each_entry((s), &(b)->act_list, \
				     node)

/**
 * @brief gets the first free slot from bucket
 */
#define BKT_FREE_SLOT_GET(b)         list_first_entry(&(b)->inact_list,       \
				     struct cuckoo_hash_slot, node)

/**
 * @brief gets the first active slot from bucket
 */
#define BKT_ACT_SLOT_GET(b)          list_first_entry(&(b)->act_list,         \
				     struct cuckoo_hash_slot, node)

/**
 * @brief goes trough all the active slots in bucket safe
 *        (during the loop, entries can be removed from the
 *        list)
 */
#define BKT_FOREACH_ACT_SLOT_SAFE(s, n, b) list_for_each_entry_safe((s), (n), \
					   &(b)->act_list, node)

/**
 * @brief check if bucket is full (all slots are active)
 */
#define BKT_IS_FULL(b)               list_empty(&(b)->inact_list)

/**
 * @brief activate a slot in bucket
 */
#define SLOT_ACTIVATE(s, b)          list_move(&(s)->node, &(b)->act_list)

/**
 * @brief inactivate a slot in bucket
 */
#define SLOT_INACTIVATE(s, b)        list_move(&(s)->node, &(b)->inact_list)

/**
 * @brief copy item data structure
 */
#define ITEM_COPY(dst, src)          memcpy(dst, src, sizeof(*dst))

/**
 * @brief returne the inverse table
 * TABLE1 --> TABLE2
 * TABLE2 --> TABLE1
 */
#define TBL_INVERSE(t)               (!(t))

/**
 * @brief minimum size of action log, must be 3 since we can add
 *        3 actions to log in each replacement iteration
 */
#define ACT_LOG_MIN_SZ               (3)

/**
 * @struct cuckoo_hash_slot
 * @item cuckoo hash item
 * @node list node
 * @slot_idx slot index in bucket
 */
struct cuckoo_hash_slot {
	struct cuckoo_hash_item item;
	struct list_head        node;
	u8                      slot_idx;
};

/**
 * @struct cuckoo_hash_bucket
 * @slot slots array
 * @act_list slots active list
 * @inact_list slots inactive list
 */
struct cuckoo_hash_bucket {
	struct cuckoo_hash_slot *slot;
	struct list_head        act_list;
	struct list_head        inact_list;
};

/**
 * @struct cuckoo_hash
 * @t1 hash table 1
 * @t2 hash table 2
 * @tbl_sz size of each table
 * @bkt_sz bucket size
 * @act_log action log buffer
 * @act_log_len action log length
 * @event_cb event callback
 * @keycmp_cb key compare callback
 * @hash_lock hash intenal lock enable
 * @lock cuckoo hash locker
 * @hash_mask mask for the hash results
 * @rplc_max maximum replacements allowed
 * @lu_insr if to lookup before insert
 * @hash_comp if to compute hash internally
 * @stats cuckoo hash statistics
 * @dbg_hist_sz size of debug histogram (debug)
 * @ins_hist insert histogram (debug)
 * @full_hist table full histogram (debug)
 */
struct cuckoo_hash {
	struct cuckoo_hash_bucket *t1;
	struct cuckoo_hash_bucket *t2;
	u32                       tbl_sz;
	u32                       bkt_sz;
	struct cuckoo_hash_action *act_log;
	u32                       act_log_len;
	cuckoo_hash_event_cb_t    event_cb;
	cuckoo_hash_key_compare_cb_t keycmp_cb;
	bool                      hash_lock;
	spinlock_t                lock;
	u32                       hash_mask;
	u32                       rplc_max;
	bool                      lu_insr;
	bool                      hash_comp;
#ifdef CONFIG_CUCKOO_HASH_DEBUG
	struct cuckoo_hash_stats  stats;
	u32                       dbg_hist_sz;
	u32                       *ins_hist;
	u32                       *full_hist;
#endif /* CONFIG_CUCKOO_HASH_DEBUG */
};

/**
 * @enum tbl_sel
 * @TABLE1 select hash table 1
 * @TABLE2 select hash table 2
 * @SINGLE_TABLE select a single table
 * @BOTH_TABLES select both tables
 */
enum tbl_sel {
	TABLE1       = 0,
	TABLE2       = 1,
	SINGLE_TABLE = 1,
	BOTH_TABLES  = 2,
};

/**
 * @brief lock the cuckoo hash tables
 * @param hash cuckoo hash context
 */
static inline void __lock(struct cuckoo_hash *hash)
{
	spin_lock_bh(&hash->lock);
}

/**
 * @brief unlock the cuckoo hash tables
 * @param hash cuckoo hash context
 */
static inline void __unlock(struct cuckoo_hash *hash)
{
	spin_unlock_bh(&hash->lock);
}

/**
 * @brief compute 3 hash results on a given item
 * @param hash cuckoo hash context
 * @param item cuckoo hash item
 */
static inline void __compute_hash(struct cuckoo_hash *hash,
				  struct cuckoo_hash_item *item)
{
	u32 hash_res, mask = hash->hash_mask;

	hash_res  = jhash(item->key, item->key_len, 0);
	item->h1  = mask & hash_res;
	item->sig = hash_32(hash_res, 32);
	item->h2  = mask & jhash(item->key, item->key_len, item->sig);
}

/**
 * @brief get a cuckoo hash bucket
 * @param hash cuckoo hash context
 * @param item cuckoo hash item
 * @param tbl slected hash table
 * @return struct cuckoo_hash_bucket* cuckoo hash bucket
 */
static inline struct cuckoo_hash_bucket *
__bkt_get(struct cuckoo_hash *hash, struct cuckoo_hash_item *item,
	  enum tbl_sel tbl)
{
	return ((tbl == TABLE1) ? &hash->t1[item->h1 & hash->hash_mask] :
				  &hash->t2[item->h2 & hash->hash_mask]);
}

#ifdef CONFIG_CUCKOO_HASH_DEBUG
static inline void __stats_ins_req(struct cuckoo_hash *hash)
{
	hash->stats.ins_req++;
}

static inline void __stats_lu_done(struct cuckoo_hash *hash)
{
	hash->stats.lu_done++;
}

static inline void __stats_lu_req(struct cuckoo_hash *hash)
{
	hash->stats.lu_req++;
}

static inline void __stats_rmv_req(struct cuckoo_hash *hash)
{
	hash->stats.rmv_req++;
}

static inline void __stats_rplc_update(struct cuckoo_hash *hash)
{
	hash->stats.rplc_total += hash->act_log_len - 1;
	hash->stats.rplc_wmark =
		(hash->stats.rplc_wmark > hash->act_log_len - 1) ?
		hash->stats.rplc_wmark : hash->act_log_len - 1;
}

static inline void __stats_ins_hist_update(struct cuckoo_hash *hash)
{
	u32 hist_range, hist_idx;

	if (!hash->dbg_hist_sz)
		return;

	hist_range = hash->tbl_sz * hash->bkt_sz * 2 / hash->dbg_hist_sz;
	hist_idx = min(hash->dbg_hist_sz - 1,
		       hist_range ? (hash->stats.t1.occ + hash->stats.t2.occ)
		       / hist_range : 0);
	hash->ins_hist[hist_idx]++;
}

static inline void __stats_full_hist_update(struct cuckoo_hash *hash)
{
	u32 hist_range, hist_idx;

	if (!hash->dbg_hist_sz)
		return;

	hist_range = hash->tbl_sz * hash->bkt_sz * 2 / hash->dbg_hist_sz;
	hist_idx = min(hash->dbg_hist_sz - 1,
		       hist_range ? (hash->stats.t1.occ + hash->stats.t2.occ)
		       / hist_range : 0);
	hash->full_hist[hist_idx]++;
}

static inline void __stats_ins_done(struct cuckoo_hash *hash, u8 tbl_idx)
{
	struct cuckoo_hash_tbl_stats *tbl_stats =
		(tbl_idx == TABLE1) ? &hash->stats.t1 : &hash->stats.t2;

	hash->stats.ins_done++;
	__stats_ins_hist_update(hash);
	if (++tbl_stats->occ > tbl_stats->wmark)
		tbl_stats->wmark = tbl_stats->occ;
}

static inline void __stats_rmv_done(struct cuckoo_hash *hash, u8 tbl_idx)
{
	struct cuckoo_hash_tbl_stats *tbl_stats =
		(tbl_idx == TABLE1) ? &hash->stats.t1 : &hash->stats.t2;

	hash->stats.rmv_done++;
	tbl_stats->occ--;
}

static inline void __stats_reset(struct cuckoo_hash *hash)
{
	memset(&hash->stats,    0, sizeof(hash->stats));
	memset(hash->full_hist, 0,
	       hash->dbg_hist_sz * sizeof(*hash->full_hist));
	memset(hash->ins_hist,  0,
	       hash->dbg_hist_sz * sizeof(*hash->ins_hist));
}

static inline s32 __stats_hist_alloc(struct cuckoo_hash *hash)
{
	hash->ins_hist = NULL;
	hash->full_hist = NULL;

	if (!hash->dbg_hist_sz)
		return 0;

	hash->ins_hist = kcalloc(hash->dbg_hist_sz, sizeof(*hash->ins_hist),
				 GFP_KERNEL);

	if (unlikely(!hash->ins_hist))
		return -ENOMEM;

	hash->full_hist = kcalloc(hash->dbg_hist_sz, sizeof(*hash->full_hist),
				  GFP_KERNEL);

	if (unlikely(!hash->full_hist)) {
		kfree(hash->ins_hist);
		return -ENOMEM;
	}

	return 0;
}

static inline void __stats_hist_free(struct cuckoo_hash *hash)
{
	kfree(hash->ins_hist);
	kfree(hash->full_hist);
}

s32 cuckoo_hash_stats_get(void *hash, struct cuckoo_hash_stats *stats)
{
	struct cuckoo_hash *__hash = (struct cuckoo_hash *)hash;

	if (unlikely(!(__hash && stats)))
		return -EINVAL;

	if (__hash->hash_lock)
		__lock(__hash);
	memcpy(stats, &__hash->stats, sizeof(*stats));
	if (__hash->hash_lock)
		__unlock(__hash);

	return 0;
}
EXPORT_SYMBOL(cuckoo_hash_stats_get);

void cuckoo_hash_stats_dump(void *hash)
{
	struct cuckoo_hash *__hash = (struct cuckoo_hash *)hash;
	char buf[64];
	u64 hist_range;
	u32 i;

	if (unlikely(!__hash))
		return;

	if (__hash->hash_lock)
		__lock(__hash);

	pr_info("\n");
	pr_info("+-----------------------------------------+\n");
	pr_info("|          Cuckoo Hash Counters           |\n");
	pr_info("+-----------------------------------------+\n");
	pr_info("|         Counter         |     Value     |\n");
	pr_info("+=========================+===============+\n");
	pr_info("| %-24s| %-14u|\n", "Insert request", __hash->stats.ins_req);
	pr_info("+-------------------------+---------------+\n");
	pr_info("| %-24s| %-14u|\n", "Insert success", __hash->stats.ins_done);
	pr_info("+-------------------------+---------------+\n");
	pr_info("| %-24s| %-14u|\n", "Insert success \%",
		__hash->stats.ins_req ?
		(__hash->stats.ins_done * 100 / __hash->stats.ins_req) : 100);
	pr_info("+-------------------------+---------------+\n");
	pr_info("| %-24s| %-14u|\n", "Remove request", __hash->stats.rmv_req);
	pr_info("+-------------------------+---------------+\n");
	pr_info("| %-24s| %-14u|\n", "Remove success", __hash->stats.rmv_done);
	pr_info("+-------------------------+---------------+\n");
	pr_info("| %-24s| %-14u|\n", "Remove success \%",
		__hash->stats.rmv_req ?
		(__hash->stats.rmv_done * 100 / __hash->stats.rmv_req) : 100);
	pr_info("+-------------------------+---------------+\n");
	pr_info("| %-24s| %-14u|\n", "Lookup request", __hash->stats.lu_req);
	pr_info("+-------------------------+---------------+\n");
	pr_info("| %-24s| %-14u|\n", "Lookup success", __hash->stats.lu_done);
	pr_info("+-------------------------+---------------+\n");
	pr_info("| %-24s| %-14u|\n", "Lookup success \%",
		__hash->stats.lu_req ?
		(__hash->stats.lu_done * 100 / __hash->stats.lu_req) : 100);
	pr_info("+-------------------------+---------------+\n");
	pr_info("| %-24s| %-14u|\n", "Total replace", __hash->stats.rplc_total);
	pr_info("+-------------------------+---------------+\n");
	pr_info("| %-24s| %-14u|\n", "Replace watermark",
		__hash->stats.rplc_wmark);
	pr_info("+-------------------------+---------------+\n");

	pr_info("\n");
	pr_info("+---------------------------------------------------------+\n");
	pr_info("|                     Tables Occupancy                    |\n");
	pr_info("+---------------------------------------------------------+\n");
	pr_info("|         Counter         |    T1 Value   |    T2 Value   |\n");
	pr_info("+=========================+===============+===============+\n");
	pr_info("| %-24s| %-14u| %-14u|\n", "Occupancy",
		__hash->stats.t1.occ, __hash->stats.t2.occ);
	pr_info("+-------------------------+---------------+---------------+\n");
	pr_info("| %-24s| %-14u| %-14u|\n", "Watermark",
		__hash->stats.t1.wmark, __hash->stats.t2.wmark);
	pr_info("+-------------------------+---------------+---------------+\n");
	pr_info("\n");

	if (!__hash->dbg_hist_sz) {
		if (__hash->hash_lock)
			__unlock(__hash);
		return;
	}

	hist_range = __hash->tbl_sz * __hash->bkt_sz * 2 / __hash->dbg_hist_sz;
	pr_info("\n");
	pr_info("+---------------------------------------------------------+\n");
	pr_info("|                 Cuckoo Hash Histogram                   |\n");
	pr_info("+---------------------------------------------------------+\n");
	pr_info("|     Tables Occupancy    |   At Insert   | At Table Full |\n");
	pr_info("+=========================+===============+===============+\n");

	for (i = 0; i < __hash->dbg_hist_sz; i++) {
		scnprintf(buf, sizeof(buf), "%llu..%llu",
			  hist_range * i, hist_range * (i + 1));
		pr_info("| %-24s|%-15u|%-15u|\n",
			buf, __hash->ins_hist[i], __hash->full_hist[i]);
		pr_info("+-------------------------+---------------+---------------+\n");
	}
	pr_info("\n");

	if (__hash->hash_lock)
		__unlock(__hash);
}
EXPORT_SYMBOL(cuckoo_hash_stats_dump);
#else /* !CONFIG_CUCKOO_HASH_DEBUG */
static inline void __stats_ins_req(struct cuckoo_hash *hash)
{
}

static inline void __stats_lu_done(struct cuckoo_hash *hash)
{
}

static inline void __stats_lu_req(struct cuckoo_hash *hash)
{
}

static inline void __stats_rmv_req(struct cuckoo_hash *hash)
{
}

static inline void __stats_rplc_update(struct cuckoo_hash *hash)
{
}

static inline void __stats_ins_hist_update(struct cuckoo_hash *hash)
{
}

static inline void __stats_full_hist_update(struct cuckoo_hash *hash)
{
}

static inline void __stats_ins_done(struct cuckoo_hash *hash, u8 tbl_idx)
{
}

static inline void __stats_rmv_done(struct cuckoo_hash *hash, u8 tbl_idx)
{
}

static inline void __stats_reset(struct cuckoo_hash *hash)
{
}

static inline s32 __stats_hist_alloc(struct cuckoo_hash *hash)
{
	return 0;
}

static inline void __stats_hist_free(struct cuckoo_hash *hash)
{
}

s32 cuckoo_hash_stats_get(void *hash, struct cuckoo_hash_stats *stats)
{
	pr_info("CUCKOO HASH: statistics operation is not available\n");
	return -EPERM;
}
EXPORT_SYMBOL(cuckoo_hash_stats_get);

void cuckoo_hash_stats_dump(void *hash)
{
	pr_info("CUCKOO HASH: statistics dump operation is not available\n");
}
EXPORT_SYMBOL(cuckoo_hash_stats_dump);
#endif /* CONFIG_CUCKOO_HASH_DEBUG */

/**
 * @brief add action to the action log
 * @param hash cuckoo hash context
 * @param item cuckoo hash item
 * @param slot the new slot to insert the item (NULL for
 *        ITEM_REMOVED)
 * @param tbl hash table
 * @param act_id action index on the action log
 */
static void __act_add(struct cuckoo_hash *hash, struct cuckoo_hash_item *item,
		      struct cuckoo_hash_slot *slot, enum tbl_sel tbl,
		      u8 act_id)
{
	struct cuckoo_hash_slot *prv_slot =
		container_of(item, struct cuckoo_hash_slot, item);

	hash->act_log_len++;
	ITEM_COPY(&hash->act_log[act_id].item, item);

	if (slot) {
		if (prv_slot->slot_idx == INVALID_SLOT_IDX) {
			/* User item */
			hash->act_log[act_id].type = ITEM_ADDED;
		} else {
			/* Moved item */
			hash->act_log[act_id].type = ITEM_MOVED;
			hash->act_log[act_id].old_tbl = TBL_INVERSE(tbl);
			hash->act_log[act_id].old_slot = prv_slot->slot_idx;
		}
		hash->act_log[act_id].new_tbl = tbl;
		hash->act_log[act_id].new_slot = slot->slot_idx;
	} else {
		/* Removed item */
		hash->act_log[act_id].type = ITEM_REMOVED;
		hash->act_log[act_id].old_tbl = tbl;
		hash->act_log[act_id].old_slot = prv_slot->slot_idx;
	}
}

/**
 * @brief check if the signature is already exist in a bucket
 * @param bkt cuckoo hash bucket
 * @param sig signature to search
 * @return bool true if the signature exist, false otherwise
 */
static bool __sig_is_exist(struct cuckoo_hash_bucket *bkt, u32 sig)
{
	struct cuckoo_hash_slot *slot;

	BKT_FOREACH_ACT_SLOT(slot, bkt)
		if (slot->item.sig == sig)
			return true;

	return false;
}

/**
 * @brief insert slot to a possible bucket (no replacements)
 * @param hash cuckoo hash context
 * @param slot slot to insert
 * @param num_tbls number of possible hash tables
 * @param tbl table to start with
 * @param act_id action index to fill if operation success
 * @return s32 0 for success, -ENOSPC for "table full"
 */
static s32 __slot_insert(struct cuckoo_hash *hash,
			 struct cuckoo_hash_slot *slot,
			 u8 num_tbls, enum tbl_sel tbl,
			 u8 act_id)
{
	struct cuckoo_hash_bucket *bkt;
	struct cuckoo_hash_slot *new_slot;

	while (num_tbls--) {
		bkt = __bkt_get(hash, &slot->item, tbl);
		if (!BKT_IS_FULL(bkt) &&
		    !__sig_is_exist(bkt, slot->item.sig)) {
			new_slot = BKT_FREE_SLOT_GET(bkt);
			__act_add(hash, &slot->item, new_slot, tbl, act_id);
			return 0;
		}
		tbl = TBL_INVERSE(tbl);
	}

	return -ENOSPC;
}

/**
 * @brief insert slot to a possible bucket when the possible
 *        buckets are full (need a single replacement)
 * @param hash cuckoo hash context
 * @param slot slot to insert
 * @param num_tbls number of possible hash tables
 * @param tbl table to start with
 * @param act_id action index to fill if operation success
 * @return s32 0 for success, -ENOSPC for "table full"
 */
static s32 __slot_mv_insert(struct cuckoo_hash *hash,
			    struct cuckoo_hash_slot *slot,
			    u8 num_tbls, enum tbl_sel tbl,
			    u8 act_id)
{
	struct cuckoo_hash_bucket *bkt;
	struct cuckoo_hash_slot *new_slot;
	enum tbl_sel inv_tbl = TBL_INVERSE(tbl);

	if (!hash->rplc_max)
		return -ENOSPC;

	while (num_tbls--) {
		bkt = __bkt_get(hash, &slot->item, tbl);
		BKT_FOREACH_ACT_SLOT(new_slot, bkt) {
			if (__slot_insert(hash, new_slot, SINGLE_TABLE,
					  inv_tbl, act_id + 1))
				continue;
			__act_add(hash, &slot->item, new_slot, tbl, act_id);
			return 0;
		}
		tbl     = TBL_INVERSE(tbl);
		inv_tbl = TBL_INVERSE(inv_tbl);
	}

	return -ENOSPC;
}

/**
 * @brief insert slot to a possible bucket when the possible
 *        buckets are full and the possible buckets to all
 *        candidates are full (need more then a single
 *        replacement)
 * @param hash cuckoo hash context
 * @param slot slot to insert
 * @param num_tbls number of possible hash tables
 * @param tbl table to start with
 * @param act_id action index to fill if operation success
 * @return s32 0 for success, -ENOSPC for "table full"
 */
static s32 __slot_rplc_insert(struct cuckoo_hash *hash,
			      struct cuckoo_hash_slot *slot,
			      u8 num_tbls, enum tbl_sel tbl,
			      u8 act_id)
{
	struct cuckoo_hash_bucket *bkt;
	struct cuckoo_hash_item *curr_item = &slot->item;
	struct cuckoo_hash_slot *new_slot;
	enum tbl_sel inv_tbl = TBL_INVERSE(tbl);
	u32 rplc = 0;

	while (hash->rplc_max > rplc++) {
		while (num_tbls--) {
			bkt = __bkt_get(hash, curr_item, tbl);
			BKT_FOREACH_ACT_SLOT(new_slot, bkt) {
				if (__slot_mv_insert(hash, new_slot,
						     SINGLE_TABLE, inv_tbl,
						     act_id + 1))
					continue;
				__act_add(hash, curr_item, new_slot,
					  tbl, act_id);
				return 0;
			}
			tbl     = TBL_INVERSE(tbl);
			inv_tbl = TBL_INVERSE(inv_tbl);
		}
		/* Force replacement */
		new_slot = BKT_ACT_SLOT_GET(bkt);
		__act_add(hash, curr_item, new_slot, inv_tbl, act_id++);
		curr_item = &new_slot->item;
	}

	__stats_full_hist_update(hash);
	return -ENOSPC;
}

/**
 * @brief insert item into cuckoo hash tables
 * @param hash cuckoo hash context
 * @param item item to insert
 * @return s32 0 for success, -ENOSPC for "table full"
 */
static s32 __item_insert(struct cuckoo_hash *hash,
			 struct cuckoo_hash_item *item)
{
	struct cuckoo_hash_slot slot;

	ITEM_COPY(&slot.item, item);
	slot.slot_idx = INVALID_SLOT_IDX;

	if ((__slot_insert(hash, &slot, BOTH_TABLES, TABLE1, 0))    &&
	    (__slot_mv_insert(hash, &slot, BOTH_TABLES, TABLE1, 0)) &&
	    (__slot_rplc_insert(hash, &slot, BOTH_TABLES, TABLE1, 0)))
		return -ENOSPC;

	return 0;
}

/**
 * @brief execute a single action on the tables
 * @param hash cuckoo hash context
 * @param act_id the index of the required action on the action
 *        log buffer
 */
static void __act_execute(struct cuckoo_hash *hash, u8 act_id)
{
	struct cuckoo_hash_bucket *bkt;
	struct cuckoo_hash_slot *slot;
	u8 tbl_idx, slot_idx;
	bool remove_act = hash->act_log[act_id].type == ITEM_REMOVED;

	if (remove_act) {
		/* Action type is remove */
		tbl_idx = hash->act_log[act_id].old_tbl;
		slot_idx = hash->act_log[act_id].old_slot;
	} else {
		tbl_idx = hash->act_log[act_id].new_tbl;
		slot_idx = hash->act_log[act_id].new_slot;
	}

	/* Get bucket */
	bkt = __bkt_get(hash, &hash->act_log[act_id].item, tbl_idx);
	/* Get slot */
	slot = &bkt->slot[slot_idx];
	if (remove_act) {
		SLOT_INACTIVATE(slot, bkt); /* Move slot to inactive list */
		__stats_rmv_done(hash, tbl_idx);
	} else {
		/* Copy the item from log to slot */
		ITEM_COPY(&slot->item, &hash->act_log[act_id].item);
		/* Last action in log is the inactive slot - activate it */
		if (act_id == hash->act_log_len - 1) {
			SLOT_ACTIVATE(slot, bkt);
			__stats_ins_done(hash, tbl_idx);
		}
	}
}

/**
 * @brief execute the action log
 * @param hash cuckoo hash context
 */
static void __log_execute(struct cuckoo_hash *hash)
{
	u32 idx = hash->act_log_len;

	if (unlikely(!idx)) {
		pr_err("CUCKOO HASH ERROR: action log is empty\n");
		return;
	}

	/* Do the action */
	while (idx > 0)
		__act_execute(hash, --idx);

	/* Update stats */
	__stats_rplc_update(hash);

	/* Send event */
	if (hash->event_cb) {
		idx = hash->act_log_len;
		while (idx > 0)
			hash->event_cb(&hash->act_log[--idx]);
	}
}

/**
 * @brief lookup for a slot in the cuckoo hash by a given item
 * @param hash cuckoo hash context
 * @param item item for searching
 * @param remove if to remove the item in case of match
 * @param tbl_idx if item found (and not NULL), return the table index
 * @return struct cuckoo_hash_slot* if match found return the
 *         slot, return NULL otherwise
 */
static struct cuckoo_hash_slot *__slot_lookup(struct cuckoo_hash *hash,
					      struct cuckoo_hash_item *item,
					      bool remove,
					      u8 *tbl_idx)
{
	struct cuckoo_hash_slot *slot;
	struct cuckoo_hash_bucket *bkt;
	enum tbl_sel tbl = TABLE1;
	u8 num_tbls = BOTH_TABLES;

	while (num_tbls--) {
		bkt = __bkt_get(hash, item, tbl);
		BKT_FOREACH_ACT_SLOT(slot, bkt) {
			if (item->sig != slot->item.sig)
				continue;
			/* Sig match - compare key length */
			if (item->key_len != slot->item.key_len)
				continue;
			/* Sig and length match - compare full key */
			if (hash->keycmp_cb) {
				if (hash->keycmp_cb(item->key,
						    slot->item.key,
						    slot->item.key_len))
					continue;
			} else if (memcmp(item->key, slot->item.key,
					  slot->item.key_len)) {
				continue;
			}
			/* Full match */
			if (remove)
				__act_add(hash, &slot->item, NULL, tbl, 0);
			if (tbl_idx)
				*tbl_idx = tbl;
			return slot;
		}
		tbl = TBL_INVERSE(tbl);
	}

	return NULL;
}

/**
 * @brief deallocate buckets memory
 * @param hash cuckoo hash context
 */
static void __tbl_bkt_free(struct cuckoo_hash *hash)
{
	u32 bkt;

	for (bkt = 0; bkt < hash->tbl_sz; bkt++) {
		kfree(hash->t1[bkt].slot);
		kfree(hash->t2[bkt].slot);
	}
}

/**
 * @brief allocate buckets memory
 * @param hash cuckoo hash context
 * @return s32 0 for success, -ENOMEM otherwise
 */
static s32 __tbl_bkt_alloc(struct cuckoo_hash *hash)
{
	u32 bkt, slot_idx;

	/* Allocate TABLE1 buckets */
	for (bkt = 0; bkt < hash->tbl_sz; bkt++) {
		hash->t1[bkt].slot = kcalloc(hash->bkt_sz,
					     sizeof(*hash->t1[bkt].slot),
					     GFP_KERNEL);
		if (unlikely(!hash->t1[bkt].slot))
			goto t1_bkt_fail;
		INIT_LIST_HEAD(&hash->t1[bkt].act_list);
		INIT_LIST_HEAD(&hash->t1[bkt].inact_list);
		for (slot_idx = 0; slot_idx < hash->bkt_sz; slot_idx++) {
			hash->t1[bkt].slot[slot_idx].slot_idx = slot_idx;
			list_add_tail(&hash->t1[bkt].slot[slot_idx].node,
				      &hash->t1[bkt].inact_list);
		}
	}

	/* Allocate TABLE2 buckets */
	for (bkt = 0; bkt < hash->tbl_sz; bkt++) {
		hash->t2[bkt].slot = kcalloc(hash->bkt_sz,
					     sizeof(*hash->t2[bkt].slot),
					     GFP_KERNEL);
		if (unlikely(!hash->t2[bkt].slot))
			goto t2_bkt_fail;
		INIT_LIST_HEAD(&hash->t2[bkt].act_list);
		INIT_LIST_HEAD(&hash->t2[bkt].inact_list);
		for (slot_idx = 0; slot_idx < hash->bkt_sz; slot_idx++) {
			hash->t2[bkt].slot[slot_idx].slot_idx = slot_idx;
			list_add_tail(&hash->t2[bkt].slot[slot_idx].node,
				      &hash->t2[bkt].inact_list);
		}
	}

	return 0;

t2_bkt_fail:
	while (bkt)
		kfree(hash->t2[--bkt].slot);
	bkt = hash->tbl_sz;
t1_bkt_fail:
	while (bkt)
		kfree(hash->t1[--bkt].slot);

	return -ENOMEM;
}

/**
 * @brief deallocate tables memory
 * @param hash cuckoo hash context
 */
static void __tbl_free(struct cuckoo_hash *hash)
{
	/* Free TABLE1 */
	kfree(hash->t1);
	/* Free TABLE2 */
	kfree(hash->t2);
}

/**
 * @brief allocate tables memory
 * @param hash cuckoo hash context
 * @return s32 0 for success, -ENOMEM otherwise
 */
static s32 __tbl_alloc(struct cuckoo_hash *hash)
{
	/* Allocate TABLE1 */
	hash->t1 = kcalloc(hash->tbl_sz, sizeof(*hash->t1), GFP_KERNEL);
	if (unlikely(!hash->t1))
		return -ENOMEM;

	/* Allocate TABLE2 */
	hash->t2 = kcalloc(hash->tbl_sz, sizeof(*hash->t1), GFP_KERNEL);
	if (unlikely(!hash->t2)) {
		kfree(hash->t1);
		return -ENOMEM;
	}

	return 0;
}

s32 cuckoo_hash_remove(void *hash, struct cuckoo_hash_item *item)
{
	struct cuckoo_hash *__hash = (struct cuckoo_hash *)hash;
	s32 ret = 0;

	if (unlikely(!(__hash && item && item->key))) {
		pr_err("CUCKOO HASH ERROR: invalid cuckoo hash request\n");
		return -EINVAL;
	}

	if (__hash->hash_comp)
		__compute_hash(__hash, item);

	if (__hash->hash_lock)
		__lock(__hash);

	__stats_rmv_req(__hash);
	__hash->act_log_len = 0;
	if (!__slot_lookup(__hash, item, true, NULL)) {
		ret = -ENOENT;
		goto unlock;
	}

	__log_execute(__hash);

unlock:
	if (__hash->hash_lock)
		__unlock(__hash);

	return ret;
}
EXPORT_SYMBOL(cuckoo_hash_remove);

s32 cuckoo_hash_lookup(void *hash, struct cuckoo_hash_item *item)
{
	return cuckoo_hash_lookup_verbose(hash, item, NULL, NULL);
}
EXPORT_SYMBOL(cuckoo_hash_lookup);

s32 cuckoo_hash_lookup_verbose(void *hash,
			       struct cuckoo_hash_item *item,
			       u8 *tbl_idx,
			       u8 *slot_idx)
{
	struct cuckoo_hash *__hash = (struct cuckoo_hash *)hash;
	struct cuckoo_hash_slot *slot = NULL;
	s32 ret = ENOENT;

	if (unlikely(!(__hash && item && item->key))) {
		pr_err("CUCKOO HASH ERROR: invalid cuckoo hash request\n");
		return -EINVAL;
	}

	if (__hash->hash_comp)
		__compute_hash(__hash, item);

	if (__hash->hash_lock)
		__lock(__hash);

	__stats_lu_req(__hash);
	slot = __slot_lookup(__hash, item, false, tbl_idx);

	if (slot) {
		item->value = slot->item.value;
		if (slot_idx)
			*slot_idx = slot->slot_idx;
		ret = EEXIST;
		__stats_lu_done(__hash);
	}

	if (__hash->hash_lock)
		__unlock(__hash);

	return ret;
}
EXPORT_SYMBOL(cuckoo_hash_lookup_verbose);

s32 cuckoo_hash_insert(void *hash, struct cuckoo_hash_item *item)
{
	struct cuckoo_hash *__hash = (struct cuckoo_hash *)hash;
	s32 ret = 0;

	if (unlikely(!(__hash && item && item->key))) {
		pr_err("CUCKOO HASH ERROR: invalid cuckoo hash request\n");
		return -EINVAL;
	}

	if (__hash->hash_comp)
		__compute_hash(__hash, item);

	if (__hash->hash_lock)
		__lock(__hash);

	__stats_ins_req(__hash);
	__hash->act_log_len = 0;
	if (__hash->lu_insr && __slot_lookup(__hash, item, false, NULL)) {
		ret = -EEXIST;
		goto unlock;
	}

	if (unlikely(__item_insert(__hash, item))) {
		ret = -ENOSPC;
		goto unlock;
	}

	__log_execute(__hash);

unlock:
	if (__hash->hash_lock)
		__unlock(__hash);

	return ret;
}
EXPORT_SYMBOL(cuckoo_hash_insert);

s32 cuckoo_hash_flush(void *hash)
{
	struct cuckoo_hash *__hash = (struct cuckoo_hash *)hash;
	struct cuckoo_hash_slot *slot, *next;
	struct cuckoo_hash_bucket *bkt;
	u8 num_tbls = BOTH_TABLES;
	enum tbl_sel tbl = TABLE1;
	u32 i;

	if (unlikely(!__hash))
		return -EINVAL;

	if (__hash->hash_lock)
		__lock(__hash);

	while (num_tbls--) {
		for (i = 0; i < __hash->tbl_sz; i++) {
			bkt = ((tbl == TABLE1) ?
			       &__hash->t1[i & __hash->hash_mask] :
			       &__hash->t2[i & __hash->hash_mask]);
			BKT_FOREACH_ACT_SLOT_SAFE(slot, next, bkt) {
				/* Move slot to inactive list */
				SLOT_INACTIVATE(slot, bkt);
			}
		}
		tbl = TBL_INVERSE(tbl);
	}

	__stats_reset(__hash);

	if (__hash->hash_lock)
		__unlock(__hash);

	return 0;
}
EXPORT_SYMBOL(cuckoo_hash_flush);

void * __must_check cuckoo_hash_create(struct cuckoo_hash_param *param)
{
	struct cuckoo_hash *__hash;

	if (unlikely(!param)) {
		pr_err("CUCKOO HASH ERROR: failed to create hash, invalid param\n");
		goto failed;
	}

	if (unlikely(param->hash_bits > CUCKOO_HASH_MAX_BITS)) {
		pr_err("CUCKOO HASH ERROR: failed to create hash, bits[%hhu] max[%u]\n",
		       param->hash_bits, CUCKOO_HASH_MAX_BITS);
		goto failed;
	}

	if (unlikely(param->bucket_size > CUCKOO_HASH_MAX_BKT_SIZE)) {
		pr_err("CUCKOO HASH ERROR: failed to create hash, bkt_sz[%hhu] max[%u]\n",
		       param->bucket_size, CUCKOO_HASH_MAX_BKT_SIZE);
		goto failed;
	}

	/* Allocate cuckoo hash context */
	__hash = kzalloc(sizeof(*__hash), GFP_KERNEL);
	if (unlikely(!__hash))
		goto hash_alloc_failed;

	__hash->tbl_sz      = 1 << param->hash_bits;
	__hash->hash_mask   = __hash->tbl_sz - 1;
	__hash->bkt_sz      = param->bucket_size ? param->bucket_size : 1;
	__hash->hash_lock   = param->lock_enable;
	__hash->hash_comp   = param->hash_compute;
	__hash->lu_insr     = param->lu_on_insert;
	__hash->event_cb    = param->event_cb;
	__hash->keycmp_cb   = param->keycmp_cb;
	__hash->rplc_max    = param->max_replace ? param->max_replace : 1;
#ifdef CONFIG_CUCKOO_HASH_DEBUG
	__hash->dbg_hist_sz = param->dbg_hist_sz;
#endif /* CONFIG_CUCKOO_HASH_DEBUG */

	/* Allocate tables */
	if (unlikely(__tbl_alloc(__hash)))
		goto tbl_alloc_failed;

	/* Allocate buckets */
	if (unlikely(__tbl_bkt_alloc(__hash)))
		goto bkt_alloc_failed;

	/* Allocate action log */
	__hash->act_log = kcalloc(__hash->rplc_max + ACT_LOG_MIN_SZ,
				  sizeof(*__hash->act_log), GFP_KERNEL);
	if (unlikely(!__hash->act_log))
		goto act_log_alloc_failed;

	if (unlikely(__stats_hist_alloc(__hash)))
		goto dbg_hist_alloc_failed;

	if (__hash->hash_lock)
		spin_lock_init(&__hash->lock);

	return __hash;

dbg_hist_alloc_failed:
	kfree(__hash->act_log);
act_log_alloc_failed:
	__tbl_bkt_free(__hash);
bkt_alloc_failed:
	__tbl_free(__hash);
tbl_alloc_failed:
	kfree(__hash);
hash_alloc_failed:
	pr_err("CUCKOO HASH ERROR: failed to create cuckoo hash, out of memory\n");
failed:
	return NULL;
}
EXPORT_SYMBOL(cuckoo_hash_create);

s32 cuckoo_hash_destroy(void *hash)
{
	struct cuckoo_hash *__hash = (struct cuckoo_hash *)hash;

	if (unlikely(!__hash))
		return -EINVAL;

	if (__hash->hash_lock)
		__lock(__hash);

	__stats_hist_free(__hash);
	kfree(__hash->act_log);
	__tbl_bkt_free(__hash);
	__tbl_free(__hash);

	if (__hash->hash_lock)
		__unlock(__hash);

	kfree(__hash);

	return 0;
}
EXPORT_SYMBOL(cuckoo_hash_destroy);

void cuckoo_hash_tbl_dump(void *hash, u32 first_bkt, u32 last_bkt)
{
	struct cuckoo_hash *__hash = (struct cuckoo_hash *)hash;
	struct cuckoo_hash_slot *slot;
	struct cuckoo_hash_bucket *bkt;
	u32 i;

	if (unlikely(!__hash))
		return;

	if (unlikely(last_bkt < first_bkt)) {
		pr_err("CUCKOO HASH ERROR: last bucket (%u) must be >= to first bucket (%u)\n",
		       last_bkt, first_bkt);
		return;
	}

	if (unlikely(last_bkt > __hash->hash_mask)) {
		pr_err("CUCKOO HASH ERROR: last bucket (%u) out of hash range (%u)\n",
		       last_bkt, __hash->hash_mask);
		return;
	}

	if (__hash->hash_lock)
		__lock(__hash);

	pr_info("\n");
	pr_info("+------------------------------------------------------+\n");
	pr_info("|                       TABLE 1                        |\n");
	pr_info("+------------------------------------------------------+\n");
	pr_info("|  Bucket  | Slot Idx |    H1    |    H2    |   SIG    |\n");
	pr_info("+==========+==========+==========+==========+==========+\n");
	for (i = first_bkt; i <= last_bkt; i++) {
		bkt = &__hash->t1[i & __hash->hash_mask];
		if (list_empty(&bkt->act_list))
			continue;
		BKT_FOREACH_ACT_SLOT(slot, bkt) {
			pr_info("|%-10u|%-10hhu|0x%-8X|0x%-8X|0x%-8X|\n",
				i, slot->slot_idx, slot->item.h1,
				slot->item.h2, slot->item.sig);
		}
		pr_info("+----------+----------+----------+----------+----------+\n");
	}

	pr_info("\n");
	pr_info("+------------------------------------------------------+\n");
	pr_info("|                       TABLE 2                        |\n");
	pr_info("+------------------------------------------------------+\n");
	pr_info("|  Bucket  | Slot Idx |    H1    |    H2    |   SIG    |\n");
	pr_info("+==========+==========+==========+==========+==========+\n");
	for (i = first_bkt; i <= last_bkt; i++) {
		bkt = &__hash->t2[i & __hash->hash_mask];
		if (list_empty(&bkt->act_list))
			continue;
		BKT_FOREACH_ACT_SLOT(slot, bkt) {
			pr_info("|%-10u|%-10hhu|0x%-8X|0x%-8X|0x%-8X|\n",
				i, slot->slot_idx, slot->item.h1,
				slot->item.h2, slot->item.sig);
		}
		pr_info("+----------+----------+----------+----------+----------+\n");
	}
	pr_info("\n");

	if (__hash->hash_lock)
		__unlock(__hash);
}
EXPORT_SYMBOL(cuckoo_hash_tbl_dump);

void cuckoo_hash_cfg_dump(void *hash)
{
	struct cuckoo_hash *__hash = (struct cuckoo_hash *)hash;

	if (unlikely(!__hash))
		return;

	if (__hash->hash_lock)
		__lock(__hash);

	pr_info("\n");
	pr_info("+-----------------------------------------+\n");
	pr_info("|          Cuckoo Hash Config             |\n");
	pr_info("+-----------------------------------------+\n");
	pr_info("|         Counter         |     Value     |\n");
	pr_info("+=========================+===============+\n");
	pr_info("| %-24s| %-14u|\n", "Tables size", __hash->tbl_sz);
	pr_info("+-------------------------+---------------+\n");
	pr_info("| %-24s| %-14u|\n", "Bucket size", __hash->bkt_sz);
	pr_info("+-------------------------+---------------+\n");
	pr_info("| %-24s| %-14u|\n", "Max replacements", __hash->rplc_max);
	pr_info("+-------------------------+---------------+\n");
	pr_info("| %-24s| %-14s|\n", "Event callback",
		__hash->event_cb ? "Enable" : "Disable");
	pr_info("+-------------------------+---------------+\n");
	pr_info("| %-24s| %-14s|\n", "Hash lock",
		__hash->hash_lock ? "Enable" : "Disable");
	pr_info("+-------------------------+---------------+\n");
	pr_info("| %-24s| %-14s|\n", "Hash compute",
		__hash->hash_comp ? "Enable" : "Disable");
	pr_info("+-------------------------+---------------+\n");
	pr_info("| %-24s| %-14s|\n", "Lookup on insert",
		__hash->lu_insr ? "Enable" : "Disable");
	pr_info("+-------------------------+---------------+\n");
	pr_info("| %-24s| %-14s|\n", "Hash compute",
		__hash->hash_comp ? "Enable" : "Disable");
	pr_info("+-------------------------+---------------+\n");
#ifdef CONFIG_CUCKOO_HASH_DEBUG
	pr_info("| %-24s| %-14u|\n", "Histogram size", __hash->dbg_hist_sz);
	pr_info("+-------------------------+---------------+\n");
#endif /* CONFIG_CUCKOO_HASH_DEBUG */
	pr_info("\n");

	if (__hash->hash_lock)
		__unlock(__hash);
}
EXPORT_SYMBOL(cuckoo_hash_cfg_dump);
