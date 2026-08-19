/*****************************************************************************
 *
 * Copyright (c) 2020 - 2021 MaxLinear, Inc.
 * Copyright (c) 2019 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "pon_net_pool.h"
#include "pon_net_common.h"
#include "pon_net_debug.h"

struct index_freed {
	/* Next free index */
	struct index_freed *next;
	/* Free index */
	uint32_t index;
};

struct pon_net_pool {
	/** List of freed indexes */
	struct index_freed *freed;
	/** Number of entries in freed list */
	unsigned int num_freed;
	/** Next index that this pool will generate */
	uint32_t next_index;
	/**
	 * Offset is added to every generated number.
	 * This is the 'min' parameter provided in create.
	 * The 'next_index' starts from 0, this is why we need to
	 * add offset.
	 */
	uint32_t offset;
	/** Maximum number of indexes this pool can generate*/
	uint32_t max_num_indexes;
};

unsigned int pon_net_pool_available(const struct pon_net_pool *pool)
{
	return pool->max_num_indexes - pool->next_index + pool->num_freed;
}

struct pon_net_pool *pon_net_pool_create(uint32_t min, uint32_t max)
{
	struct pon_net_pool *pool = NULL;

	dbg_in();

	if (min > max) {
		dbg_out_ret("%p", NULL);
		return NULL;
	}

	pool = calloc(1, sizeof(*pool));
	if (!pool) {
		dbg_out_ret("%p", NULL);
		return NULL;
	}

	pool->offset = min;
	pool->max_num_indexes = max - min + 1;

	dbg_out_ret("%p", pool);
	return pool;
}

static enum pon_adapter_errno pool_get(struct pon_net_pool *pool,
				       uint32_t *value)
{
	struct index_freed *freed = pool->freed;
	uint32_t index = 0;

	dbg_in_args("%p, %p", pool, value);

	if (pool->num_freed) {
		pool->freed = freed->next;
		index = freed->index;
		free(freed);
		pool->num_freed--;
	} else {
		index = pool->next_index;
		if (pool->next_index >= pool->max_num_indexes) {
			dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_AVAIL);
			return PON_ADAPTER_ERR_NOT_AVAIL;
		}

		pool->next_index++;
	}

	index += pool->offset;

	*value = index;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Compares two uint32_t numbers - needed for qsort() */
static int compare_u32(const void *__a, const void *__b)
{
	const uint32_t *a = __a, *b = __b;

	if (*a < *b)
		return -1;
	if (*a > *b)
		return 1;
	return 0;
}

enum pon_adapter_errno pon_net_pool_get_many(struct pon_net_pool *pool,
					     uint32_t *values,
					     unsigned int count)
{
	unsigned int i;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %u", pool, values, count);

	if (pon_net_pool_available(pool) < count) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_AVAIL);
		return PON_ADAPTER_ERR_NOT_AVAIL;
	}

	for (i = 0; i < count; ++i) {
		ret = pool_get(pool, &values[i]);
		if (ret != PON_ADAPTER_SUCCESS) {
			/* We print an error here, because if pon_net_pool_get()
			 * fails while pon_net_pool_available() reported that */
			FN_ERR_RET(ret, pool_get, ret);
			return ret;
		}
	}

	/* Values are promised to be sorted */
	qsort(values, count, sizeof(*values), compare_u32);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Checks if all numbers in 'values' array belong
 * to this 'pool' */
static bool check_indexes(struct pon_net_pool *pool,
			  unsigned int count,
			  const uint32_t *values)
{
	unsigned int i = 0;
	struct index_freed *f = NULL;

	dbg_in_args("%p, %u, %p", pool, count, values);

	for (i = 0; i < count; ++i) {
		if (values[i] < pool->offset) {
			dbg_err("the value (%u) does not belong to the pool\n",
				values[i]);
			dbg_out_ret("%d", false);
			return false;
		}

		if (values[i] - pool->offset >= pool->max_num_indexes) {
			dbg_err("the value (%u) does not belong to the pool\n",
				values[i]);
			dbg_out_ret("%d", false);
			return false;
		}

		if (values[i] - pool->offset >= pool->next_index) {
			dbg_err("unmatched free - the value (%u) has already been freed or has not been allocated yet\n",
				values[i]);
			dbg_out_ret("%d", false);
			return false;
		}

		for (f = pool->freed; f; f = f->next) {
			if (f->index == values[i] - pool->offset) {
				dbg_err("double free - the value (%u) has already been freed\n",
					values[i]);
				return false;
			}
		}
	}

	dbg_out_ret("%d", true);
	return true;
}

/* Deallocate list items starting from 'freed' until 'end'
 * is reached ('end' is _not_ deallocated) */
static void clear_freed_until(struct index_freed *freed,
			      const struct index_freed *end)
{
	dbg_in_args("%p, %p", freed, end);

	struct index_freed *next = NULL;
	struct index_freed *i = NULL;

	for (i = freed; i != end; i = next) {
		next = i->next;
		free(i);
	}

	dbg_out();
}

enum pon_adapter_errno pon_net_pool_put_many(struct pon_net_pool *pool,
					     const uint32_t *values,
					     unsigned int count)
{
	struct index_freed *freed = pool->freed;
	uint32_t next_index = pool->next_index;
	unsigned int i = 0;
	unsigned int num_freed = pool->num_freed;

	dbg_in_args("%p, %p, %u", pool, values, count);

	if (!check_indexes(pool, count, values)) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	for (i = 0; i < count; ++i) {
		/* We iterate in reverse order, because if values array
		 * were previously allocated with pon_net_pool_get_many(),
		 * we have a some chance to avoid adding entries to
		 * "freed" list */
		uint32_t index = values[count - i - 1] - pool->offset;
		struct index_freed *item;

		if (index + 1 == next_index) {
			next_index--;
			continue;
		}

		item = malloc(sizeof(*freed));
		if (!item) {
			/* Clear new 'freed' nodes that we allocated
			 * until this point */
			clear_freed_until(freed, pool->freed);
			FN_ERR_RET(0, malloc, PON_ADAPTER_ERR_NO_MEMORY);
			return PON_ADAPTER_ERR_NO_MEMORY;
		}

		item->next = freed;
		item->index = index;
		freed = item;
		num_freed++;
	}

	pool->freed = freed;
	pool->num_freed = num_freed;
	pool->next_index = next_index;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

void pon_net_pool_clear(struct pon_net_pool *pool)
{

	dbg_in_args("%p", pool);

	clear_freed_until(pool->freed, NULL);

	pool->freed = NULL;
	pool->num_freed = 0;
	pool->next_index = 0;

	dbg_out();
}

void pon_net_pool_destroy(struct pon_net_pool *pool)
{
	dbg_in_args("%p", pool);

	if (!pool) {
		dbg_out();
		return;
	}

	pon_net_pool_clear(pool);
	free(pool);
	dbg_out();
}
