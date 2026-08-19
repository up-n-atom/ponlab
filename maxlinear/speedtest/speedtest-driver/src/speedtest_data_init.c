// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright © 2022 MaxLinear, Inc.
 *
 *****************************************************************************/
/*!
 * This file contains implementation of page allocation API's
 */

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/mm.h>

#include "speedtest_driver.h"
#include "speedtest_data_init.h"
#include "speedtest_helpers.h"

void init_pages(struct list_head *page_list)
{
	struct page_node *pnode = NULL;

	if (!page_list || list_empty(page_list)) {
		return;
	}

	list_for_each_entry(pnode, page_list, list_node) {
		memset(page_address(pnode->page), 0x00,
				PAGE_SIZE_BY_ORDER(pnode->porder));
	}

	return;
}

void deallocate_pages(struct list_head *page_list)
{
	struct page_node *pnode = NULL;
	struct list_head *iter = NULL;
	struct list_head *safe_iter = NULL;

	if (!page_list || list_empty(page_list)) {
		return;
	}

	list_for_each_safe(iter, safe_iter, page_list) {
		pnode = list_entry(iter, struct page_node, list_node);
		if (pnode && pnode->page) {
			put_page(pnode->page);
			pnode->page = NULL;
		}
		list_del(iter);
		if (pnode) {
			kfree(pnode);
			pnode = NULL;
		}
	}
	return;
}

int allocate_pages(struct list_head *page_list, uint64_t buffer_size, int8_t *page_order)
{
	struct page_node *new_node = NULL;
	struct page *pg = NULL;
	int num_of_pages = 0;
	int ret = -EINVAL;

	if ((!page_list) || (buffer_size <= 0)) {
		return ret;
	}
	if (!list_empty(page_list)) {
		return -EINVAL;
	}

	while (buffer_size > 0) {
		new_node = kmalloc(sizeof(struct page_node), GFP_KERNEL);
		if (!new_node) {
			ret = -ENOMEM;
			goto end;
		}

		if ((*page_order) > 0) {
			pg = alloc_pages(__GFP_COMP, (*page_order));
		} else {
			pg = alloc_page(GFP_KERNEL);
		}
		if (!pg) {
			kfree(new_node);
			if ((*page_order) < 0) {
				ret = -EFAULT;
				goto end;
			}
			(*page_order) --;
			continue;
		}

		new_node->porder = (*page_order);
		new_node->page = pg;
		if (buffer_size < PAGE_SIZE_BY_ORDER(*page_order)) {
			new_node->dsize = buffer_size;
		} else {
			new_node->dsize = PAGE_SIZE_BY_ORDER(*page_order);
		}
		ret = buffer_size - PAGE_SIZE_BY_ORDER(*page_order);
		if (ret < 0)
			buffer_size = 0;
		else
			buffer_size -= PAGE_SIZE_BY_ORDER(*page_order);
		list_add_tail(&new_node->list_node, page_list);
		num_of_pages ++;
	}

	return num_of_pages;

end:
	deallocate_pages(page_list);
	return ret;
}
