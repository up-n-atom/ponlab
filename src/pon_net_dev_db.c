/*****************************************************************************
 *
 * Copyright (c) 2021 - 2022 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "pon_net_pool.h"
#include "pon_net_dev_db.h"
#include "pon_net_common.h"
#include "pon_net_debug.h"

/* Create specific pon_net_dev_info from def */
static struct pon_net_dev_info *
dev_info_create(const char *ifname, const struct pon_net_dev_def *def)
{
	unsigned int i;
	struct pon_net_dev_info *info;
	unsigned int max_pool_id = 0;

	dbg_in_args("%p", def);

	if (!def->pool_cfgs_count) {
		dbg_out_ret("%p", NULL);
		return NULL;
	}

	if (strlen(ifname) >= sizeof(info->ifname)) {
		dbg_out_ret("%p", NULL);
		return NULL;
	}

	info = calloc(1, sizeof(*info));
	if (!info) {
		FN_ERR_RET(0, calloc, 0);
		return NULL;
	}

	for (i = 0; i < def->pool_cfgs_count; ++i)
		if (def->pool_cfgs[i].id > max_pool_id)
			max_pool_id = def->pool_cfgs[i].id;

	info->handle_pools_count = max_pool_id + 1;

	info->handle_pools =
	    calloc(info->handle_pools_count, sizeof(*info->handle_pools));
	if (!info->handle_pools) {
		free(info);
		FN_ERR_RET(0, calloc, 0);
		return NULL;
	}

	for (i = 0; i < def->pool_cfgs_count; ++i) {
		unsigned int id = def->pool_cfgs[i].id;

		if (def->pool_cfgs[i].is_static)
			continue;

		if (info->handle_pools[id]) {
			dbg_err("pool %u is already configured\n", id);
			goto cleanup;
		}

		info->handle_pools[id] = pon_net_pool_create(
		    def->pool_cfgs[i].min, def->pool_cfgs[i].max);
		if (!info->handle_pools[id]) {
			dbg_err_fn_ret(pon_net_pool_create, 0);
			goto cleanup;
		}
	}

	info->def = def;
	snprintf(info->ifname, sizeof(info->ifname), "%s", ifname);

	dbg_out_ret("%p", info);
	return info;

cleanup:
	for (i = 0; i < info->handle_pools_count; ++i)
		pon_net_pool_destroy(info->handle_pools[i]);
	free(info->handle_pools);
	free(info);
	dbg_out_ret("%p", NULL);
	return NULL;
}

static void dev_info_destroy(struct pon_net_dev_info *info)
{
	unsigned int i = 0;

	if (!info)
		return;

	for (i = 0; i < info->handle_pools_count; ++i)
		pon_net_pool_destroy(info->handle_pools[i]);

	free(info->handle_pools);
	free(info);
}

/** A database of network device information */
struct pon_net_dev_db {
	/**
	 * Dynamic array of network device infos. It is sorted by dev->ifname
	 */
	struct pon_net_dev_info **devs;
	/** Number of entries in devs array */
	unsigned int devs_count;
	/**
	 * The 'devs' array capacity. When 'devs_count' reaches capacity the
	 * 'devs_capacity' will be reallocated
	 */
	unsigned int devs_capacity;
};

/* Comparator used for bsearch() */
static int compare_dev_ifname(const void *ifname, const void *__dev)
{
	const struct pon_net_dev_info *const *dev = __dev;

	return strncmp(ifname, (*dev)->ifname, sizeof((*dev)->ifname));
}

/* Comparator used for qsort() */
static int compare_devices(const void *__a, const void *b)
{
	const struct pon_net_dev_info *const *a = __a;

	return compare_dev_ifname((*a)->ifname, b);
}

/* Get index for network device info in 'devs' array by 'ifname' */
static int find_dev_idx(struct pon_net_dev_db *db, const char *ifname)
{
	struct pon_net_dev_info **found;
	int idx;

	dbg_in_args("%p, \"%s\"", db, ifname);

	found = bsearch(ifname, db->devs, db->devs_count,
			sizeof(db->devs[0]), compare_dev_ifname);
	if (!found) {
		dbg_out_ret("%d", -1);
		return -1;
	}

	idx = (int)(found - db->devs);

	dbg_out_ret("%d", idx);
	return idx;
}

/* Sort devs array so that we can use bsearch() on it */
static void sort_db(struct pon_net_dev_db *db)
{
	qsort(db->devs, db->devs_count, sizeof(db->devs[0]), compare_devices);
}

#define GROW_FACTOR 2
/* If there is no more space in 'devs' array, then it will grow the 'devs' array
 * so that we can add elements to it. After this function succeeds it is fine
 * to add ONE element to the 'devs' array */
static enum pon_adapter_errno grow_devs_if_needed(struct pon_net_dev_db *db)
{
	dbg_in_args("%p", db);

	if (db->devs_count == db->devs_capacity) {
		struct pon_net_dev_info **devs;
		unsigned int devs_capacity = GROW_FACTOR * db->devs_capacity;

		devs = realloc(db->devs, sizeof(*db->devs) * devs_capacity);
		if (!devs) {
			FN_ERR_RET(0, realloc, PON_ADAPTER_ERR_NO_MEMORY);
			return PON_ADAPTER_ERR_NO_MEMORY;
		}

		db->devs = devs;
		db->devs_capacity = devs_capacity;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

#define INITIAL_CAPACITY 8
struct pon_net_dev_db *pon_net_dev_db_create(void)
{
	struct pon_net_dev_db *db;

	dbg_in();

	db = calloc(1, sizeof(*db));
	if (!db) {
		dbg_out_ret("%p", NULL);
		return NULL;
	}

	db->devs_capacity = INITIAL_CAPACITY;

	db->devs = calloc(db->devs_capacity, sizeof(*db->devs));
	if (!db->devs) {
		dbg_err_fn_ret(calloc, 0);
		free(db);
		dbg_out_ret("%p", NULL);
		return NULL;
	}

	dbg_out_ret("%p", db);
	return db;
}

enum pon_adapter_errno pon_net_dev_db_add(struct pon_net_dev_db *db,
					  const char *ifname,
					  const struct pon_net_dev_def *def)
{
	enum pon_adapter_errno ret;
	struct pon_net_dev_info *info;

	dbg_in_args("%p, %p, %p", db, ifname, def);

	info = pon_net_dev_db_get(db, ifname);
	if (info) {
		/* If an entry already exists, we have a problem in the calling
		 * code - best to fail as soon as possible */
		if (info->def == def)
			dbg_wrn("Attempt to add new dev_db entry for '%s', but it already exists.\n",
				ifname);
		else
			dbg_err("Attempt to add new dev_db entry for '%s', but it already exists and it is of a different kind.\n",
				ifname);
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	ret = grow_devs_if_needed(db);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, grow_devs, ret);
		return ret;
	}

	info = dev_info_create(ifname, def);
	if (!info) {
		FN_ERR_RET(0, dev_info_create, PON_ADAPTER_ERR_NO_MEMORY);
		return PON_ADAPTER_ERR_NO_MEMORY;
	}

	/* The call to grow_devs() made sure that we have enough space */
	db->devs[db->devs_count++] = info;

	sort_db(db);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

struct pon_net_dev_info *pon_net_dev_db_get(struct pon_net_dev_db *db,
					    const char *ifname)
{
	int idx;

	dbg_in_args("%p, %p", db, ifname);

	idx = find_dev_idx(db, ifname);
	if (idx == -1) {
		dbg_out_ret("%p", NULL);
		return NULL;
	}

	dbg_out_ret("%p", db->devs[idx]);
	return db->devs[idx];
}

void pon_net_dev_db_del(struct pon_net_dev_db *db, const char *ifname)
{
	int idx;

	dbg_in_args("%p, %p", db, ifname);

	idx = find_dev_idx(db, ifname);
	if (idx == -1) {
		/* Nothing to delete */
		dbg_out();
		return;
	}

	dev_info_destroy(db->devs[idx]);

	/* We delete by swapping with last. But after we swap, the
	 * 'devs' array is not sorted anymore, therefore we need to sort
	 * it again */
	db->devs[idx] = db->devs[db->devs_count - 1];
	db->devs_count--;
	sort_db(db);

	dbg_out();
}

void pon_net_dev_db_destroy(struct pon_net_dev_db *db)
{
	unsigned int i;

	if (!db)
		return;

	for (i = 0; i < db->devs_count; ++i)
		dev_info_destroy(db->devs[i]);

	free(db->devs);
	free(db);
}

enum pon_adapter_errno pon_net_dev_db_gen(struct pon_net_dev_db *db,
					  const char *ifname,
					  unsigned int pool_id,
					  uint32_t *handles, unsigned int count)
{
	enum pon_adapter_errno ret;
	struct pon_net_dev_info *info;
	struct pon_net_pool *pool;

	dbg_in_args("%p, %p, %d, %p, %u", db, ifname, pool_id, handles, count);

	info = pon_net_dev_db_get(db, ifname);
	if (!info) {
		dbg_err("pon_net_dev_db_get() failed on [%s] with %d\n", ifname,
			0);
		dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	if (pool_id >= info->handle_pools_count) {
		dbg_err("Pool '%d' is not configured\n", pool_id);
		dbg_out_ret("%d", PON_ADAPTER_ERR_OUT_OF_BOUNDS);
		return PON_ADAPTER_ERR_OUT_OF_BOUNDS;
	}

	pool = info->handle_pools[pool_id];
	if (!pool) {
		dbg_err("Pool '%d' is not configured\n", pool_id);
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	ret = pon_net_pool_get_many(pool, handles, count);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_pool_get_many, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno pon_net_dev_db_put(struct pon_net_dev_db *db,
					  const char *ifname,
					  unsigned int pool_id,
					  const uint32_t *handles,
					  unsigned int count)
{
	enum pon_adapter_errno ret;
	struct pon_net_dev_info *info;
	struct pon_net_pool *pool;

	dbg_in_args("%p, %p, %d, %p, %u", db, ifname, pool_id, handles, count);

	info = pon_net_dev_db_get(db, ifname);
	if (!info) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	if (pool_id >= info->handle_pools_count) {
		dbg_err("Pool '%d' is not configured\n", pool_id);
		dbg_out_ret("%d", PON_ADAPTER_ERR_OUT_OF_BOUNDS);
		return PON_ADAPTER_ERR_OUT_OF_BOUNDS;
	}

	pool = info->handle_pools[pool_id];
	if (!pool) {
		if (!pool_id) {
			dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
			return PON_ADAPTER_SUCCESS;
		}
		dbg_err("Pool '%d' is not configured\n", pool_id);
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	ret = pon_net_pool_put_many(pool, handles, count);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_pool_put_many, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno pon_net_dev_db_map(struct pon_net_dev_db *db,
					  const char *ifname,
					  unsigned int pool_id,
					  uint32_t key,
					  uint32_t *value)
{
	struct pon_net_dev_info *info;
	unsigned int i;
	const struct pon_net_dev_pool_cfg *cfg = NULL;
	unsigned int count;

	dbg_in_args("%p, %p, %u, %u, %p", db, ifname, pool_id, key, value);

	info = pon_net_dev_db_get(db, ifname);
	if (!info) {
		FN_ERR_RET(0, pon_net_dev_db_get, PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	for (i = 0; i < info->def->pool_cfgs_count; ++i) {
		cfg = &info->def->pool_cfgs[i];

		if (cfg->id == pool_id)
			break;
	}

	if (i == info->def->pool_cfgs_count) {
		dbg_err("Pool '%d' is not configured\n", pool_id);
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	if (!cfg->is_static) {
		dbg_err("Pool '%d' is not configured as static\n", pool_id);
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	count = cfg->max - cfg->min + 1;
	if (key >= count) {
		dbg_err("Key '%u' can't be mapped to [%u;%u] range\n", key,
			cfg->min, cfg->max);
		dbg_out_ret("%d", PON_ADAPTER_ERR_OUT_OF_BOUNDS);
		return PON_ADAPTER_ERR_OUT_OF_BOUNDS;
	}

	if (value)
		*value = cfg->min + key;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

int pon_net_dev_db_foreach(struct pon_net_dev_db *db,
			   int (*fn)(struct pon_net_dev_info *info, void *arg),
			   void *arg)
{
	unsigned int i = 0;
	int ret;

	dbg_in_args("%p, %p, %p", db, fn, arg);

	for (i = 0; i < db->devs_count; ++i) {
		ret = fn(db->devs[i], arg);
		if (ret)
			return ret;
	}

	dbg_out_ret("%d", 0);
	return 0;
}

enum pon_adapter_errno
pon_net_dev_db_gen_many(struct pon_net_dev_db *db,
			const struct pon_net_dev_db_alloc *allocations,
			unsigned int num_allocations)
{
	unsigned int i;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %u", db, allocations, num_allocations);

	for (i = 0; i < num_allocations; ++i) {
		ret = pon_net_dev_db_gen(db,
					 allocations[i].ifname,
					 allocations[i].pool_id,
					 allocations[i].dst,
					 allocations[i].count);
		if (ret != PON_ADAPTER_SUCCESS) {
			enum pon_adapter_errno ret2;

			dbg_err_fn_ret(pon_net_dev_db_gen, ret);

			/* Deallocate handles and prios allocated so far */
			while (i--) {
				ret2 = pon_net_dev_db_put(
				    db, allocations[i].ifname,
				    allocations[i].pool_id,
				    allocations[i].dst,
				    allocations[i].count);
				if (ret2 != PON_ADAPTER_SUCCESS)
					dbg_err_fn_ret(pon_net_dev_db_put,
						       ret2);
			}

			dbg_out_ret("%d", ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

void
pon_net_dev_db_put_many(struct pon_net_dev_db *db,
			const struct pon_net_dev_db_alloc *allocations,
			unsigned int num_allocations)
{
	unsigned int i;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %u", db, allocations, num_allocations);

	for (i = 0; i < num_allocations; ++i) {
		ret = pon_net_dev_db_put(db,
					 allocations[i].ifname,
					 allocations[i].pool_id,
					 allocations[i].dst,
					 allocations[i].count);
		/*
		 * If PON_ADAPTER_ERR_NOT_FOUND is returned, then
		 * it means that the device is not in db, therefore
		 * there is nothing to clean up and we do not
		 * need to report an error
		 */
		if (ret != PON_ADAPTER_SUCCESS &&
		    ret != PON_ADAPTER_ERR_NOT_FOUND)
			dbg_err_fn_ret(pon_net_dev_db_put, ret);
	}

	dbg_out();
}
