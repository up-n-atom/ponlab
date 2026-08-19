/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2021-2022 MaxLinear, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _MXL_GEN_POOL_H
#define _MXL_GEN_POOL_H

enum fw_perm;
struct gen_pool;
struct gen_pool_chunk;

struct mxl_pool_free_data {
	struct device *dev;
	int sai;
	unsigned long vaddr;
	size_t size;
	enum fw_perm perm;
	unsigned long opt;
};

struct mxl_pool_alloc_data {
	struct device *dev;
	int sai;
	unsigned long opt;
	enum fw_perm perm;
};

/* genpool event IDs */
enum {
	NOC_GENPL_EVENT_CPU_POOL,
	NOC_GENPL_EVENT_RW_POOL,
	NOC_GENPL_EVENT_ICC_POOL,
	NOC_GENPL_EVENT_RO_POOL,
	NOC_GENPL_EVENT_SYS_POOL,
	NOC_GENPL_EVENT_GENPOOL,
	NOC_GENPL_EVENT_MAX,
};

/* genpool event data */
struct mxl_pool_event_data {
	unsigned long paddr;
	size_t size;
};

#if IS_ENABLED(CONFIG_MXL_SEC_GEN_POOL)
void mxl_soc_pool_free(struct gen_pool *pool,
		       struct gen_pool_chunk *chunk, void *data);
#if 0
/* This API is deprecated, instead use mxl_soc_pool_alloc */

unsigned long
mxl_soc_pool_algo(unsigned long *map, unsigned long size,
		  unsigned long start, unsigned int nr, void *data,
		  struct gen_pool *pool, unsigned long start_addr);
#endif
int mxl_noc_genpl_notifier_blocking_chain(unsigned long val, void *v);
unsigned long mxl_soc_pool_alloc(struct gen_pool *pool, size_t size, void *allocdata);
#else
static inline int mxl_noc_genpl_notifier_blocking_chain(unsigned long val,
							void *v)
{
	return NOTIFY_OK;
}
#endif /* IS_ENABLED(CONFIG_MXL_SEC_GEN_POOL) */

#endif /* _MXL_GEN_POOL_H */
