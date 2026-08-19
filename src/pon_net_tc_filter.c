/*****************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>

#include "pon_net_debug.h"
#include "pon_net_common.h"

#include "pon_net_tc_filter.h"

void pon_net_tc_filter_array_clear(struct pon_net_tc_filter_array *array)
{
	dbg_in_args("%p", array);

	free(array->filters);
	array->filters = NULL;
	array->count = 0;
	array->capacity = 0;

	dbg_out();
}

enum pon_adapter_errno
pon_net_tc_filter_array_copy(struct pon_net_tc_filter_array *dst,
			     const struct pon_net_tc_filter_array *src)

{
	unsigned int count = src->count;
	struct pon_net_tc_filter *filters = NULL;
	int ret;

	dbg_in_args("%p, %p", dst, src);

	if (count) {
		filters = calloc(count, sizeof(*filters));
		if (!filters) {
			FN_ERR_RET(0, calloc, PON_ADAPTER_ERR_NO_MEMORY);
			return PON_ADAPTER_ERR_NO_MEMORY;
		}
	}

	ret = memcpy_s(filters, sizeof(*filters) * count,
		       src->filters, sizeof(*src->filters) * count);
	if (ret) {
		free(filters);
		FN_ERR_RET(ret, memcpy_s, PON_ADAPTER_ERR_MEM_ACCESS);
		return PON_ADAPTER_ERR_MEM_ACCESS;
	}

	free(dst->filters);
	dst->filters = filters;
	dst->count = count;
	dst->capacity = count;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

void pon_net_tc_filter_array_init(struct pon_net_tc_filter_array *array)
{
	dbg_in_args("%p", array);

	memset(array, 0, sizeof(*array));

	dbg_out();
}

void pon_net_tc_filter_array_exit(struct pon_net_tc_filter_array *array)
{
	dbg_in_args("%p", array);

	pon_net_tc_filter_array_clear(array);

	dbg_out();
}

void pon_net_tc_filter_array_pop(struct pon_net_tc_filter_array *array)

{
	dbg_in_args("%p", array);

	if (array->count)
		array->count--;

	dbg_out();
}

enum pon_adapter_errno
pon_net_tc_filter_array_add(struct pon_net_tc_filter_array *array,
			    const struct pon_net_tc_filter *tc_filter)
{
	unsigned int count = array->count;
	unsigned int capacity = array->capacity;
	struct pon_net_tc_filter *filters = array->filters;

	dbg_in_args("%p, %p", array, tc_filter);

	if (count == capacity) {
		capacity = capacity ? 2 * capacity : 8;
		filters = realloc(filters, sizeof(*filters) * capacity);
		if (!filters) {
			FN_ERR_RET(0, realloc, PON_ADAPTER_ERR_NO_MEMORY);
			return PON_ADAPTER_ERR_NO_MEMORY;
		}
	}

	filters[count++] = *tc_filter;

	array->count = count;
	array->capacity = capacity;
	array->filters = filters;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}
