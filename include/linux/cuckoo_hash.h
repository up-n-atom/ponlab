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
 * Description: PP Classifier cuckoo hash header file
 */

#ifndef __CUCKOO_HASH_H__
#define __CUCKOO_HASH_H__
#include <linux/types.h>

/**
 * @define maximum number of bits for hash results
 */
#define CUCKOO_HASH_MAX_BITS          (32)

/**
 * @define maximum bucket size (last value is reserved for
 *         internal mark)
 */
#define CUCKOO_HASH_MAX_BKT_SIZE      (U8_MAX - 1)

/**
 * @struct cuckoo_hash_item
 * @key item key
 * @key_len length of item key
 * @value item value
 * @h1 hash1 function result
 * @h2 hash2 function result
 * @sig signature hash function result
 */
struct cuckoo_hash_item {
	void *key;
	u32  key_len;
	void *value;
	u32  h1;
	u32  h2;
	u32  sig;
};

/**
 * @enum cuckoo_hash_action_type
 * @ITEM_ADDED item added action
 * @ITEM_MOVED item moved action
 * @ITEM_REMOVED item removed action
 */
enum cuckoo_hash_action_type {
	ITEM_ADDED,
	ITEM_MOVED,
	ITEM_REMOVED,
};

/**
 * @struct cuckoo_hash_action_type
 * @type action type
 * @item cuckoo hash item
 * @new_tbl new table index
 * @new_slot new slot index
 * @old_tbl old table index
 * @old_slot old slot index
 */
struct cuckoo_hash_action {
	enum cuckoo_hash_action_type type;
	struct cuckoo_hash_item      item;
	u8 new_tbl;
	u8 new_slot;
	u8 old_tbl;
	u8 old_slot;
};

/**
 * @typedef cuckoo hash event callback prototype
 */
typedef void (*cuckoo_hash_event_cb_t)(const struct cuckoo_hash_action *);

/**
 * @typedef cuckoo hash key compare callback prototype
 * @key1: key to search
 * @key2: compared key from DB
 * @sz: key size in bytes
 * @return Returns zero if the keys are equal
 */
typedef int (*cuckoo_hash_key_compare_cb_t)(void *key1, void *key2, u32 sz);

/**
 * @struct cuckoo_hash_param
 * @hash_bits number of bits for the hash function result
 * @bucket_size number of buckets in each hash entry
 * @max_replace maximum number of replacements during an insert
 *      operation
 * @dbg_hist_sz size of debug histogram (0 for disable)
 * @lock_enable if to use an internal locker to protect critical
 *      code
 * @hash_compute if to compute the hash results internally
 * @lu_on_insert if to do lookup before insreting an item
 * @event_cb event callback
 * @keycmp_cb key compare callback - NULL will use memcmp as default
 */
struct cuckoo_hash_param {
	u8 hash_bits;
	u8 bucket_size;
	u8 max_replace;
#ifdef CONFIG_CUCKOO_HASH_DEBUG
	u8 dbg_hist_sz;
#endif
	bool lock_enable;
	bool hash_compute;
	bool lu_on_insert;
	cuckoo_hash_event_cb_t event_cb;
	cuckoo_hash_key_compare_cb_t keycmp_cb;
};

/**
 * @struct cuckoo_hash_tbl_stats
 * @occ table occupancy
 * @wmark table occupancy watermark
 */
struct cuckoo_hash_tbl_stats {
	u32 occ;
	u32 wmark;
};

/**
 * @struct cuckoo_hash_stats
 * @t1 table1 statistics
 * @t2 table2 statistics
 * @ins_req number of insert requests
 * @ins_done number of successful inserts
 * @rmv_req number of remove requests
 * @rmv_done number of successful removes
 */
struct cuckoo_hash_stats {
	struct cuckoo_hash_tbl_stats t1;
	struct cuckoo_hash_tbl_stats t2;
	u32 ins_req;
	u32 ins_done;
	u32 rmv_req;
	u32 rmv_done;
	u32 lu_req;
	u32 lu_done;
	u32 rplc_total;
	u32 rplc_wmark;
};

/**
 * @brief create a cuckoo hash context
 * @param param cuckoo hash parameters
 * @return void*__must_check returned the cuckoo hash context,
 *         NULL if failed
 */
extern void * __must_check cuckoo_hash_create(struct cuckoo_hash_param *param);

/**
 * @brief destroy the cuckoo hash context
 * @param hash cuckoo hash context
 * @return s32 0 for success, -EINVAL otherwise
 */
extern s32 cuckoo_hash_destroy(void *hash);

/**
 * @brief lookup for an item
 * @param hash cuckoo hash context
 * @param item item for searching 
 * @note in case the item was found, the item->value will be
 *       filled inside the item by the function
 * @return s32 EEXIST if item found, ENOENT if item not found,
 *         -EINVAL for parameters error
 */
extern s32 cuckoo_hash_lookup(void *hash, struct cuckoo_hash_item *item);

/**
 * @brief lookup for an item. Verbose version - returns also the slot and table
 * @param hash cuckoo hash context
 * @param item item for searching
 * @param tbl_idx if item found, return the table index
 * @param slot_idx if item found, return the slot index
 * @note in case the item was found, the item->value will be
 *       filled inside the item by the function
 * @return s32 EEXIST if item found, ENOENT if item not found,
 *         -EINVAL for parameters error
 */
extern s32 cuckoo_hash_lookup_verbose(void *hash,
				      struct cuckoo_hash_item *item,
				      u8 *tbl_idx,
				      u8 *slot_idx);

/**
 * @brief insert a cuckoo hash item
 * @param hash cuckoo hash context
 * @param item item to insert
 * @return s32 0 for success, error code otherwise
 */
extern s32 cuckoo_hash_insert(void *hash, struct cuckoo_hash_item *item);

/**
 * @brief remove a cuckoo hash item
 * @param hash cuckoo hash context
 * @param item item to remove
 * @return s32 0 for success, error code otherwise
 */
extern s32 cuckoo_hash_remove(void *hash, struct cuckoo_hash_item *item);

/**
 * @brief flush all table
 * @param hash cuckoo hash context
 * @return s32 0 for success, error code otherwise
 */
extern s32 cuckoo_hash_flush(void *hash);

/**
 * @brief dump the cuckoo hash tables (debug only)
 * @param hash cuckoo hash context
 * @param first_bkt bucket to start from
 * @param last_bkt last bucket
 * @note last_bucket must be >= first_bkt
 */
extern void cuckoo_hash_tbl_dump(void *hash, u32 first_bkt, u32 last_bkt);

/**
 * @brief dump the cuckoo hash configuration (debug only)
 * @param hash cuckoo hash context
 */
extern void cuckoo_hash_cfg_dump(void *hash);

/**
 * @brief get a cuckoo hash statistics (debug only)
 * @note return -EPERM if CONFIG_CUCKOO_HASH_DEBUG undefined
 * @param hash cuckoo hash context
 * @param stats cuckoo hash statistics
 * @return s32 0 for success, error code otherwise
 */
extern s32 cuckoo_hash_stats_get(void *hash, struct cuckoo_hash_stats *stats);

/**
 * @brief dump the cuckoo hash statistics (debug only)
 * @note do nothing if CONFIG_CUCKOO_HASH_DEBUG undefined
 * @param hash cuckoo hash context
 */
extern void cuckoo_hash_stats_dump(void *hash);
#endif /* __CUCKOO_HASH_H__ */
