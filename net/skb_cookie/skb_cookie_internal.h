/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2017-2020 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2, as published
 * by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SKB_EXTENSION_INTERNAL_H__
#define __SKB_EXTENSION_INTERNAL_H__

#include <linux/slab.h>
#include <linux/hashtable.h>
#include <linux/spinlock.h>
#include <linux/atomic.h>
#include <linux/skb_cookie.h>

/**
 * @define This flag enabling the module statistics
 * @note Comment for disabling module statistics
 */
#define SKBEXT_MOD_STATS

#define MOD_NAME             "skb cookie extension"
#define MOD_VERSION          "0.1.1"

/**
 * @define number of bits for hash result
 */
#define SKB_EXT_HASH_BITS    10

/**
 * @define hash table size (number of buckets)
 */
#define SKB_EXT_HASHT_SIZE   (1 << SKB_EXT_HASH_BITS)

/**
 * @define number of supported cookie handles (cookie types)
 */
#define MAX_COOKIE_HANDLES   5

/**
 * @define check if cookie handle is invalid
 */
#define COOKIE_HANDLE_ERR(h) (((h) < 0) || ((h) >= MAX_COOKIE_HANDLES))

/**
 * @define going trough all the hash buckets
 */
#define FOR_EACH_SKB_EXT_BUCKET(b) for (b = 0; b < SKB_EXT_HASHT_SIZE; b++)

/**
 * @define going trough all the cookie handles
 */
#define FOR_EACH_SKB_EXT_HANDLE(h) for (h = 0; h < MAX_COOKIE_HANDLES; h++)

/**
 * @struct cookie_handle
 * @brief cookie handle entry
 */
struct cookie_handle {
	/*! cookie type handle */
	s32  handle;

	/*! name of cookie type (defined be user)*/
	char name[SKBCNAMSIZ];

	/*! if this handle is enable in DB */
	bool enable;
};

/**
 * @struct skb_ext_mod_stat
 * @brief module statistics
 */
struct skb_ext_mod_stat {
	/*! active skb extension entries */
	atomic_t act;

	/*! maximum number of active skb extension entries */
	atomic_t max;

	/*! active skb extension entries in bucket */
	atomic_t bkt_act[SKB_EXT_HASHT_SIZE];

	/*! maximum number of active skb extension entries in bucket */
	atomic_t bkt_max[SKB_EXT_HASHT_SIZE];

	/*! number of skb copy hdr events */
	atomic_t copy_hdr_events;

	/*! number of skb release events */
	atomic_t release_events;

	/*! number of skb extensions removed from skb */
	atomic_t removed;

	/*! number of skb extensions added to skb */
	atomic_t added;

	/*! number of add to a non-empty bucket op */
	atomic_t bkt_collision;

	/*! number of an active handles (users) */
	atomic_t act_handles;

	/*! number of allocated refc */
	atomic_t ext_refc_alloc;

	/*! number of deallocated refc */
	atomic_t ext_refc_free;
};

/**
 * @struct skb_ext_db
 * @brief skb extension module database
 */
struct skb_ext_db {
	/*! skb extensions cache */
	struct kmem_cache      *skb_ext_cache;

	/*! cookies reference count cache */
	struct kmem_cache      *cookie_refc_cache;

	/*! cookie handles */
	struct cookie_handle    c_handle[MAX_COOKIE_HANDLES];

	/*! cookie handles lock */
	spinlock_t              handle_lock;

	/*! skb extensions hash table */
	DECLARE_HASHTABLE(hasht, SKB_EXT_HASH_BITS);

	/*! buckets lock */
	spinlock_t              bkt_lock[SKB_EXT_HASHT_SIZE];

	/*! statistics */
	struct skb_ext_mod_stat stat;
};

#ifdef CONFIG_DEBUG_FS
s32 skb_ext_dbg_init(struct skb_ext_db *db);
void skb_ext_dbg_clean(void);
#else /* !CONFIG_DEBUG_FS */
static inline s32 skb_ext_dbg_init(struct skb_ext_db *db)
{
	return 0;
}

static inline void skb_ext_dbg_clean(void)
{
}
#endif /* CONFIG_DEBUG_FS */

#ifdef SKBEXT_MOD_STATS
/**
 * @brief update statistics in copy skb event
 * @param stat module statistics
 */
static inline void __copy_skb_stat(struct skb_ext_mod_stat *stat)
{
	atomic_inc(&stat->copy_hdr_events);
}

/**
 * @brief update statistics in release skb event
 * @param stat module statistics
 */
static inline void __release_skb_stat(struct skb_ext_mod_stat *stat)
{
	atomic_inc(&stat->release_events);
}

/**
 * @brief update statistics while handle is added
 * @param stat module statistics
 */
static inline void __hndl_add_stat(struct skb_ext_mod_stat *stat)
{
	atomic_inc(&stat->act_handles);
}

/**
 * @brief update statistics while handle is deleted
 * @param stat module statistics
 */
static inline void __se_del_stat(struct skb_ext_mod_stat *stat, s32 bkt)
{
	atomic_inc(&stat->removed);
	atomic_dec(&stat->act);
	atomic_dec(&stat->bkt_act[bkt]);
}

/**
 * @brief update statistics while skb extension is added
 * @param stat module statistics
 * @param bkt the bucket who hold the entry
 */
static inline void __se_add_stat(struct skb_ext_mod_stat *stat, s32 bkt)
{
	atomic_inc(&stat->added);
	atomic_inc(&stat->act);
	atomic_set(&stat->max,
		   max(atomic_read(&stat->act), atomic_read(&stat->max)));
	if (atomic_read(&stat->bkt_act[bkt]))
		atomic_inc(&stat->bkt_collision);
	atomic_inc(&stat->bkt_act[bkt]);
	atomic_set(&stat->bkt_max[bkt], max(atomic_read(&stat->bkt_act[bkt]),
					    atomic_read(&stat->bkt_max[bkt])));
}

/**
 * @brief update statistics while handle is released
 * @param stat module statistics
 */
static inline void __hndl_free_stat(struct skb_ext_mod_stat *stat)
{
	atomic_dec(&stat->act_handles);
}

/**
 * @brief update statistics while reference count is allocated
 * @param stat module statistics
 */
static inline void __refc_alloc_stat(struct skb_ext_mod_stat *stat)
{
	atomic_inc(&stat->ext_refc_alloc);
}

/**
 * @brief update statistics while reference count is freed
 * @param stat module statistics
 */
static inline void __refc_free_stat(struct skb_ext_mod_stat *stat)
{
	atomic_inc(&stat->ext_refc_free);
}

#else /* !SKBEXT_MOD_STATS */
static inline void __copy_skb_stat(struct skb_ext_mod_stat *stat)
{
}

static inline void __release_skb_stat(struct skb_ext_mod_stat *stat)
{
}

static inline void __hndl_add_stat(struct skb_ext_mod_stat *stat)
{
}

static inline void __se_del_stat(struct skb_ext_mod_stat *stat, s32 bkt)
{
}

static inline void __se_add_stat(struct skb_ext_mod_stat *stat, s32 bkt)
{
}

static inline void __hndl_free_stat(struct skb_ext_mod_stat *stat)
{
}

static inline void __refc_alloc_stat(struct skb_ext_mod_stat *stat)
{
}

static inline void __refc_free_stat(struct skb_ext_mod_stat *stat)
{
}
#endif /* SKBEXT_MOD_STATS */

#endif /* __SKB_EXTENSION_INTERNAL_H__ */
