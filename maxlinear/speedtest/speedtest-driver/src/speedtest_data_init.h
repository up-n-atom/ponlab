// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright © 2022 MaxLinear, Inc.
 *
 *****************************************************************************/

/*!
 * Function declarations to allocate pages and for data initialization
 */

#ifndef __SPEEDTEST_DATA_INIT_H_
#define __SPEEDTEST_DATA_INIT_H_

#include <linux/list.h>

void init_pages(struct list_head *page_list);
void deallocate_pages(struct list_head *page_list);
int allocate_pages(struct list_head *page_list, uint64_t size, int8_t *page_order);

#endif //__SPEEDTEST_DATA_INIT_H_
