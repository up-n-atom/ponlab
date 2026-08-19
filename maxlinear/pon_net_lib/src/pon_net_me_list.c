/*****************************************************************************
 *
 * Copyright (c) 2021 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
#include <stdlib.h>
#include <string.h>

#include "pon_net_me_list.h"
#include "pon_net_common.h"
#include "pon_net_debug.h"

void pon_net_me_list_init(struct pon_net_me_list *list)
{
	dbg_in_args("%p", list);
	SLIST_INIT(list);
	dbg_out();
}

enum pon_adapter_errno
pon_net_me_list_write(struct pon_net_me_list *list,
		      uint16_t class_id,
		      uint16_t me_id,
		      const void *data,
		      unsigned int data_size)
{
	struct pon_net_me_list_item *i;
	void *copy;

	dbg_in_args("%p, %u, %u, %p", list, class_id, me_id, data);

	copy = malloc(data_size);
	if (!copy) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_NO_MEMORY);
		return PON_ADAPTER_ERR_NO_MEMORY;
	}
	if (memcpy_s(copy, data_size, data, data_size)) {
		dbg_err_fn(memcpy_s);
		free(copy);
		dbg_out_ret("%d", PON_ADAPTER_ERR_NO_DATA);
		return PON_ADAPTER_ERR_NO_DATA;
	}

	SLIST_FOREACH(i, list, entries) {
		if (i->class_id == class_id && i->me_id == me_id) {
			free(i->data);
			i->data = copy;
			dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
			return PON_ADAPTER_SUCCESS;
		}
	}

	i = calloc(1, sizeof(*i));
	if (!i) {
		free(copy);
		dbg_out_ret("%d", PON_ADAPTER_ERR_NO_MEMORY);
		return PON_ADAPTER_ERR_NO_MEMORY;
	}

	i->class_id = class_id;
	i->me_id = me_id;
	i->data = copy;

	SLIST_INSERT_HEAD(list, i, entries);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_net_me_list_read(struct pon_net_me_list *list,
		     uint16_t class_id,
		     uint16_t me_id,
		     void *data,
		     unsigned int data_size)
{
	struct pon_net_me_list_item *i;

	dbg_in_args("%p, %u, %u, %p, %u", list, class_id, me_id, data,
		    data_size);

	SLIST_FOREACH(i, list, entries) {
		if (i->class_id == class_id && i->me_id == me_id) {
			if (data && data_size) {
				if (memcpy_s(data, data_size,
					i->data, data_size)) {
					dbg_err_fn(memcpy_s);
					dbg_out_ret("%d", PON_ADAPTER_ERROR);
					return PON_ADAPTER_ERROR;
				}
			}
			dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
			return PON_ADAPTER_SUCCESS;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_FOUND);
	return PON_ADAPTER_ERR_NOT_FOUND;
}

void *
pon_net_me_list_get_data(struct pon_net_me_list *list,
			 uint16_t class_id,
			 uint16_t me_id)
{
	struct pon_net_me_list_item *i;

	dbg_in_args("%p, %u, %u", list, class_id, me_id);

	SLIST_FOREACH(i, list, entries) {
		if (i->class_id == class_id && i->me_id == me_id) {
			dbg_out_ret("%p", i->data);
			return i->data;
		}
	}

	dbg_out_ret("%p", NULL);
	return NULL;
}

bool pon_net_me_list_exists(struct pon_net_me_list *list, uint16_t class_id,
			    uint16_t me_id)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %u", list, class_id, me_id);

	ret = pon_net_me_list_read(list, class_id, me_id, NULL, 0);
	if (ret != PON_ADAPTER_SUCCESS && ret != PON_ADAPTER_ERR_NOT_FOUND) {
		FN_ERR_RET(ret, pon_net_me_list_read, ret);
		return ret;
	}

	dbg_out_ret("%d", ret == PON_ADAPTER_SUCCESS);
	return ret == PON_ADAPTER_SUCCESS;
}

void pon_net_me_list_remove(struct pon_net_me_list *list,
			    uint16_t class_id,
			    uint16_t me_id)
{
	struct pon_net_me_list_item *i;

	dbg_in_args("%p, %u, %u", list, class_id, me_id);

	SLIST_FOREACH(i, list, entries) {
		if (i->class_id == class_id && i->me_id == me_id) {
			SLIST_REMOVE(list, i, pon_net_me_list_item, entries);
			free(i->data);
			free(i);
			break;
		}
	}

	dbg_out();
}

void pon_net_me_list_exit(struct pon_net_me_list *list)
{
	struct pon_net_me_list_item *i;

	dbg_in_args("%p", list);

	/* We can't use SLIST_FOREACH() here, because it is not
	   safe to remove elements from the list while iterating
	   over it with SLIST_FOREACH()  */
	while (!SLIST_EMPTY(list)) {
		i = SLIST_FIRST(list);
		SLIST_REMOVE_HEAD(list, entries);
		free(i->data);
		free(i);
	}

	dbg_out();
}
