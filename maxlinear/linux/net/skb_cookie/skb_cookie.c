// SPDX-License-Identifier: GPL-2.0
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

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/hash.h>
#include <linux/hashtable.h>
#include <linux/spinlock.h>
#include <linux/skb_cookie.h>
#include "skb_cookie_internal.h"

/**
 * @define calculate hash on the SKB pointer
 */
#define HASH_SKB(s) hash_ptr((s), SKB_EXT_HASH_BITS)

/**
 * @struct skb_ext_cookie
 * @brief skb extension data structure
 */
struct skb_ext_cookie {
	/*! array of MAX_COOKIE_HANDLES cookies */
	skb_cookie        cookie[MAX_COOKIE_HANDLES];

	/*! array of MAX_COOKIE_HANDLES destractor callback */
	void              (*destructor[MAX_COOKIE_HANDLES])(skb_cookie);

	/*! array of MAX_COOKIE_HANDLES reference counter pointers */
	atomic_t          *refc_ptr[MAX_COOKIE_HANDLES];

	/*! skb pointer (lookup key) */
	struct sk_buff    *skb;

	/*! hash table node */
	struct hlist_node node;

	/*! number of active cookies */
	u32               users;
};

static struct skb_ext_db *db;

/**
 * @brief allocate skb extension entry
 * @note The GFP_ATOMIC flag instructs the memory allocator
 *       never to block. Need to use this flag in situations
 *       where it cannot sleep where it must remain atomic such
 *       as interrupt handlers, bottom halves and process
 *       context code that is holding a lock
 * @note NUMA_NO_NODE need to be chosen when numa node should be
 *       used is unknown
 * @return struct skb_ext_cookie* allocated antry, NULL for an error
 */
static inline struct skb_ext_cookie *__se_alloc(void)
{
	return kmem_cache_alloc_node(db->skb_ext_cache,
				     GFP_ATOMIC | __GFP_ZERO,
				     NUMA_NO_NODE);
}

/**
 * @brief free skb extension entry
 * @param se skb extension entry to free
 */
static inline void __se_free(struct skb_ext_cookie *se)
{
	kmem_cache_free(db->skb_ext_cache, se);
}

/**
 * @brief add new skb extension
 * @note protected by spin_lock_bh
 * @param skb skb pointer
 * @param bkt hash table bucket of given pointer
 * @return struct skb_ext_cookie* new skb extension entry, NULL for an
 *         error
 */
static struct skb_ext_cookie *__se_add(struct sk_buff *skb, s32 bkt)
{
	struct skb_ext_cookie *se = __se_alloc();

	if (unlikely(!se)) {
		pr_err("skb extension alloc error, no memory\n");
		goto out;
	}

	se->skb = skb;
	spin_lock_bh(&db->bkt_lock[bkt]);
	hlist_add_head(&se->node, &db->hasht[bkt]);
	spin_unlock_bh(&db->bkt_lock[bkt]);
	__se_add_stat(&db->stat, bkt);

out:
	return se;
}

/**
 * @brief delete skb extension entry node from bucket list
 * @param se skb extension entry
 */
static inline void __se_node_del(struct skb_ext_cookie *se)
{
	hlist_del(&se->node);
}

/**
 * @brief delete skb extension entry
 * @note protected by spin_lock_bh
 * @param se skb extension entry for deletion
 * @param bkt hash bucket index
 */
static void __se_del(struct skb_ext_cookie *se, s32 bkt)
{
	spin_lock_bh(&db->bkt_lock[bkt]);
	__se_node_del(se);
	spin_unlock_bh(&db->bkt_lock[bkt]);
	__se_del_stat(&db->stat, bkt);
	__se_free(se);
}

/**
 * @brief search skb pointer in hash table
 * @note protected by spin_lock_bh
 * @param skb pointer for search
 * @param bkt hash calculation result
 * @return struct skb_ext_cookie* skb extension entry result, NULL if
 *         not found
 */
static struct skb_ext_cookie *__se_lookup(const struct sk_buff *skb, s32 bkt)
{
	struct skb_ext_cookie *se, *se_out = NULL;

	spin_lock_bh(&db->bkt_lock[bkt]);
	hlist_for_each_entry(se, &db->hasht[bkt], node) {
		if (skb == se->skb) {
			se_out = se;
			break;
		}
	}
	spin_unlock_bh(&db->bkt_lock[bkt]);

	/* it is safe to unlock the bucket now since
	 * no other context can hold this SKB
	 */

	return se_out;
}

/**
 * @brief allocate skb cookie reference counter
 * @note The GFP_ATOMIC flag instructs the memory allocator
 *       never to block. Need to use this flag in situations
 *       where it cannot sleep where it must remain atomic such
 *       as interrupt handlers, bottom halves and process
 *       context code that is holding a lock
 * @note NUMA_NO_NODE need to be chosen when numa node should be
 *       used is unknown
 * @return u32* allocated antry, NULL for an error
 */
static atomic_t *__refc_alloc(void)
{
	atomic_t *refc_ptr = kmem_cache_alloc_node(db->cookie_refc_cache,
						   GFP_ATOMIC, NUMA_NO_NODE);

	if (unlikely(!refc_ptr))
		goto out;

	atomic_set(refc_ptr, 1);
	__refc_alloc_stat(&db->stat);

out:
	return refc_ptr;
}

/**
 * @brief free skb cookie reference counter
 * @param refc skb extension entry to free
 */
static void __refc_free(atomic_t *refc_ptr)
{
	kmem_cache_free(db->cookie_refc_cache, refc_ptr);
	__refc_free_stat(&db->stat);
}

/**
 * @brief free specific cookie from skb extension this function
 *        handling the destructor and refc update
 * @param se skb extension entry
 * @param handle cookie handle
 */
static void __cookie_free(struct skb_ext_cookie *se, s32 handle)
{
	if (se->destructor[handle]) {
		if (!se->refc_ptr[handle]) {
			(*se->destructor[handle])(se->cookie[handle]);
		} else if (atomic_dec_and_test(se->refc_ptr[handle])) {
			__refc_free(se->refc_ptr[handle]);
			(*se->destructor[handle])(se->cookie[handle]);
		}
		se->destructor[handle] = NULL;
		se->refc_ptr[handle]   = NULL;
	}
	se->cookie[handle] = 0;
	se->users--;
}

/**
 * @brief copy skb extension to a cloned entry this function
 *        handling the refc update
 * @param dst destination skb extension entry
 * @param src source skb extension entry
 * @return s32 0 for success
 */
static s32 __se_copy(struct skb_ext_cookie *dst, struct skb_ext_cookie *src)
{
	u32 hndl;

	FOR_EACH_SKB_EXT_HANDLE(hndl) {
		if (!src->cookie[hndl])
			continue;
		dst->cookie[hndl] = src->cookie[hndl];
		if (!src->destructor[hndl])
			continue;

		if (!src->refc_ptr[hndl]) {
			src->refc_ptr[hndl] = __refc_alloc();
			if (unlikely(!src->refc_ptr[hndl])) {
				pr_err("skb extension refc alloc error, no memory\n");
				return -ENOMEM;
			}
		}
		atomic_inc(src->refc_ptr[hndl]);
		dst->refc_ptr[hndl]   = src->refc_ptr[hndl];
		dst->destructor[hndl] = src->destructor[hndl];
	}

	dst->users = src->users;

	return 0;
}

/**
 * @brief allocate cookie handle
 * @return struct cookie_handle* allocated handle entry, NULL
 *         for an error
 */
static struct cookie_handle *__hndl_alloc(void)
{
	u32 i;

	FOR_EACH_SKB_EXT_HANDLE(i) {
		if (!db->c_handle[i].enable)
			return &db->c_handle[i];
	}

	return NULL;
}

/**
 * @brief clean handle resources
 * @note protected by spin_lock_bh
 * @param handle cookie handle
 */
static void __hndl_clean(s32 handle)
{
	u32 bkt;
	struct skb_ext_cookie *se;
	struct hlist_node *t;

	FOR_EACH_SKB_EXT_BUCKET(bkt) {
		spin_lock_bh(&db->bkt_lock[bkt]);
		hlist_for_each_entry_safe(se, t, &db->hasht[bkt], node) {
			if (se->cookie[handle]) {
				__cookie_free(se, handle);
				if (!se->users) {
					__se_node_del(se);
					__se_del_stat(&db->stat, bkt);
					__se_free(se);
				}
			}
		}
		spin_unlock_bh(&db->bkt_lock[bkt]);
	}
}

/**
 * @brief free a cookie handle entry
 * @param handle cookie handle
 * @return s32 0 for success
 */
static s32 __hndl_free(s32 handle)
{
	if (!db->c_handle[handle].enable)
		return -EINVAL; /* cookie HANDLE disabled */

	db->c_handle[handle].enable = false;
	memset(db->c_handle[handle].name, 0, SKBCNAMSIZ);
	/* clean all resources from hasht */
	__hndl_clean(handle);
	__hndl_free_stat(&db->stat);

	return 0;
}

/**
 * @brief handler for skb_copy_hdr events
 * @param skb skb pointer
 * @param new_skb cloned skb pointer
 */
static void __skb_copy_hdr_handler(struct sk_buff *skb, struct sk_buff *new_skb)
{
	struct skb_ext_cookie *se, *se_new;
	s32 bkt;

	__copy_skb_stat(&db->stat);

	/* search original SKB in hash table */
	se = __se_lookup(skb, HASH_SKB(skb));
	if (!se)
		return;

	bkt = HASH_SKB(new_skb);
	se_new = __se_add(new_skb, bkt);
	if (unlikely(!se_new))
		return;

	if (unlikely(__se_copy(se_new, se)))
		__se_del(se_new, bkt);
}

/**
 * @brief handler for skb_clone events
 * @param skb skb pointer
 */
static void __skb_release_handler(struct sk_buff *skb)
{
	s32 hndl, bkt;
	struct skb_ext_cookie *se;


	bkt = HASH_SKB(skb);
	se = __se_lookup(skb, bkt);
	if (!se)
		return;

	__release_skb_stat(&db->stat);

	FOR_EACH_SKB_EXT_HANDLE(hndl) {
		if (!se->cookie[hndl])
			continue;
		__cookie_free(se, hndl);
		if (!se->users) {
			__se_del(se, bkt);
			return;
		}
	}
}

/**
 * @brief skb notification handler
 * @param unused
 * @param event event id
 * @param skb skb pointer
 * @return s32 0 for success
 */
static s32 skb_event(struct notifier_block *unused,
		     unsigned long event,
		     void *skb_list)
{
	struct sk_buff *skb = ((struct sk_buff **)skb_list)[0];
	/* only exist for events where with 2 skbs, e.g. clone */
	struct sk_buff *newskb = ((struct sk_buff **)skb_list)[1];

	if (event == SKB_RELEASE) {
		__skb_release_handler(skb);
	} else if (event == SKB_COPY_HDR) {
		/* if new skb holding an extension, release it first */
		__skb_release_handler(newskb);
		__skb_copy_hdr_handler(skb, newskb);
	}

	return 0;
}

static struct notifier_block skb_notifier = {
	.notifier_call = skb_event,
};

/**
 * @brief initialized module database
 * @return s32 0 for success
 */
static s32 __db_init(void)
{
	u32 idx;

	hash_init(db->hasht);
	spin_lock_init(&db->handle_lock);

	FOR_EACH_SKB_EXT_BUCKET(idx)
		spin_lock_init(&db->bkt_lock[idx]);

	FOR_EACH_SKB_EXT_HANDLE(idx)
		db->c_handle[idx].handle = idx;

	return 0;
}

/**
 * @brief exit module database
 */
static void __db_exit(void)
{
	u32 idx;

	spin_lock_bh(&db->handle_lock);
	/* go over all handles and clean all */
	FOR_EACH_SKB_EXT_HANDLE(idx)
		__hndl_free(idx);
	spin_unlock_bh(&db->handle_lock);
}

s32 register_skb_cookie(s32 *c_handle, const char *name)
{
	struct cookie_handle *new_handle;
	s32 hnd;

	if (unlikely(!db)) {
		pr_err("skb extension module uninitiated\n");
		return -EPERM;
	}

	if (unlikely(!c_handle || !name))
		return -EINVAL;

	spin_lock_bh(&db->handle_lock);
	/* check if another cookie type with same name already exists */
	hnd = skb_cookie_get_handle_by_name(name);
	if (unlikely(!COOKIE_HANDLE_ERR(hnd))) {
		spin_unlock_bh(&db->handle_lock);
		return -EEXIST;
	}

	new_handle = __hndl_alloc();
	if (!new_handle) {
		spin_unlock_bh(&db->handle_lock);
		pr_err("Failed to allocate handle\n");
		return -ENOMEM;
	}

	strncpy(new_handle->name, name, SKBCNAMSIZ);
	*c_handle = new_handle->handle;
	new_handle->enable = true;
	spin_unlock_bh(&db->handle_lock);
	__hndl_add_stat(&db->stat);

	return 0;
}
EXPORT_SYMBOL(register_skb_cookie);

s32 unregister_skb_cookie(const s32 c_handle)
{
	s32 ret;

	if (unlikely(!db)) {
		pr_err("skb extension module uninitiated\n");
		return -EPERM;
	}

	if (unlikely(COOKIE_HANDLE_ERR(c_handle)))
		return -EINVAL;

	spin_lock_bh(&db->handle_lock);
	ret = __hndl_free(c_handle);
	spin_unlock_bh(&db->handle_lock);

	return ret;
}
EXPORT_SYMBOL(unregister_skb_cookie);

s32 skb_cookie_attach(struct sk_buff *skb, const s32 c_handle,
		      const skb_cookie cookie, const void *destructor)
{
	struct skb_ext_cookie *se;
	s32 bkt;

	/* check incoming parameters */
	if (!skb || COOKIE_HANDLE_ERR(c_handle) || !cookie)
		return -EINVAL;

	if (!db->c_handle[c_handle].enable)
		return -EINVAL; /* cookie HANDLE disabled */

	bkt = HASH_SKB(skb);
	/* search in database */
	se = __se_lookup(skb, bkt);
	if (!se) {
		/* not found - add new one */
		se = __se_add(skb, bkt);
		if (unlikely(!se))
			return -ENOMEM;
	} else if (se->cookie[c_handle]) {
		/* cookie type exists, check if cookie is identical */
		if (unlikely(se->cookie[c_handle] == cookie))
			return -EEXIST; /* same cookie, nothing to do */
		/* release the old one */
		__cookie_free(se, c_handle);
	}

	/* extension exists and in place, update the cookie */
	se->users++;
	se->cookie[c_handle]     = cookie;
	se->destructor[c_handle] = destructor;

	return 0;
}
EXPORT_SYMBOL(skb_cookie_attach);

const skb_cookie skb_cookie_get(const struct sk_buff *skb, const s32 c_handle)
{
	struct skb_ext_cookie *se;
	skb_cookie cookie = 0;

	/* check incoming parameters */
	if (!skb || COOKIE_HANDLE_ERR(c_handle))
		goto out;

	if (unlikely(!db->c_handle[c_handle].enable))
		goto out; /* cookie HANDLE disabled */

	se = __se_lookup(skb, HASH_SKB(skb));
	if (se)
		cookie = se->cookie[c_handle];

out:
	return cookie;
}
EXPORT_SYMBOL(skb_cookie_get);

s32 skb_cookie_get_handle_by_name(const char *name)
{
	u32 hndl;

	FOR_EACH_SKB_EXT_HANDLE(hndl) {
		if (!strncmp(db->c_handle[hndl].name, name, SKBCNAMSIZ))
			return hndl;
	}

	return -EINVAL;
}
EXPORT_SYMBOL(skb_cookie_get_handle_by_name);

s32 skb_cookie_release(const struct sk_buff *skb, const s32 c_handle)
{
	struct skb_ext_cookie *se;
	s32 bkt;

	/* check incoming parameters */
	if (unlikely(!skb || COOKIE_HANDLE_ERR(c_handle)))
		return -EINVAL;

	if (unlikely(!db->c_handle[c_handle].enable))
		return -EINVAL; /* cookie HANDLE disabled */

	bkt = HASH_SKB(skb);
	se = __se_lookup(skb, bkt);
	if (!(se && se->cookie[c_handle]))
		return -ENOENT;

	__cookie_free(se, c_handle);
	if (!se->users)
		__se_del(se, bkt);

	return 0;
}
EXPORT_SYMBOL(skb_cookie_release);

/**
 * @brief module init
 * @note SLAB_HWCACHE_ALIGN means - Align objs on cache lines
 * @note SLAB_PANIC means - Panic if kmem_cache_create() fails
 * @return s32 __init 0 on success, other error code on failure
 */
static s32 __init skb_cookie_mod_init(void)
{
	db = kzalloc(sizeof(*db), GFP_KERNEL);
	if (unlikely(!db))
		goto error;

	db->skb_ext_cache =
		kmem_cache_create("skb_ext_obj", sizeof(struct skb_ext_cookie), 0,
				  SLAB_HWCACHE_ALIGN | SLAB_MEM_SPREAD |
				  SLAB_TEMPORARY | SLAB_PANIC, NULL);
	if (unlikely(!db->skb_ext_cache))
		goto unregister_db_alloc;

	db->cookie_refc_cache =
		kmem_cache_create("skb_ext_refc", sizeof(atomic_t), 0,
				  SLAB_HWCACHE_ALIGN | SLAB_MEM_SPREAD |
				  SLAB_TEMPORARY | SLAB_PANIC, NULL);
	if (unlikely(!db->cookie_refc_cache))
		goto unregister_skbext_cache;

	if (unlikely(__db_init()))
		goto unregister_refc_cache;

	if (unlikely(skb_ext_dbg_init(db)))
		goto unregister_db;

	if (register_skb_notifier(&skb_notifier)) {
		pr_err("skb notifier registration failed\n");
		goto unregister_debugfs;
	}

	return 0;

unregister_debugfs:
	skb_ext_dbg_clean();
unregister_db:
	__db_exit();
unregister_refc_cache:
	kmem_cache_destroy(db->cookie_refc_cache);
unregister_skbext_cache:
	kmem_cache_destroy(db->skb_ext_cache);
unregister_db_alloc:
	kfree(db);
error:
	return -1;
}
postcore_initcall(skb_cookie_mod_init);

/**
 * @brief module exit
 */
static void __exit skb_cookie_mod_exit(void)
{
	unregister_skb_notifier(&skb_notifier);
	skb_ext_dbg_clean();
	__db_exit();
	kmem_cache_destroy(db->cookie_refc_cache);
	kmem_cache_destroy(db->skb_ext_cache);
	kfree(db);

	pr_info("%s: exit done\n", __func__);
}
module_exit(skb_cookie_mod_exit);

MODULE_AUTHOR("Elad Fernandes <elad.fernandes@maxlinear.com>");
MODULE_DESCRIPTION(MOD_NAME);
MODULE_LICENSE("GPL");
MODULE_VERSION(MOD_VERSION);
