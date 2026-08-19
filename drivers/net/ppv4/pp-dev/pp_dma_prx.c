/*
 * Copyright (C) 2022 MaxLinear, Inc.
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
 * Description: PP Driver DMA API for PRX Platform
 */

#include <linux/kernel.h>
#include <linux/dma-mapping.h>
#include <linux/types.h>
#include "pp_dma.h"
#include "pp_common.h"

#ifdef pr_fmt
#undef pr_fmt
#define pr_fmt(fmt) "[PP_DMA]:%s:%d: " fmt, __func__, __LINE__
#endif

void pp_dma_exit(struct platform_device *pdev)
{
}

s32 pp_dma_init(struct platform_device *pdev)
{
	return 0;
}

void *pp_dma_alloc(ssize_t size, dma_addr_t *paddr, unsigned long attr)
{
	void *vaddr;

	if (!paddr) {
		pr_err("no phys address, abort.\n");
		return NULL;
	}

	/* For PRX (DMA mask 32) the default is none IOC memory */
	vaddr = dmam_alloc_coherent(pp_dev_get(), size, paddr,
				    GFP_KERNEL | __GFP_ZERO);
	pr_debug("pp allocated buffer virt=%ps phys=%pad size=%zu\n",
		 vaddr, paddr, size);

	return vaddr;
}

void pp_dma_free(void *vaddr, ssize_t size)
{
	dma_addr_t paddr = __pa(vaddr);

	pr_debug("pp free buffer virt=%ps phys=%pad size=%zu\n", vaddr, &paddr,
		 size);
	dmam_free_coherent(pp_dev_get(), size, vaddr, paddr);
}

void pp_cache_writeback(void *vaddr, size_t size)
{
	dma_map_single(pp_dev_get(), vaddr, size, DMA_TO_DEVICE);
	pr_debug("pp dma wb done virt=%ps size=%zu\n", vaddr, size);
}

void pp_cache_invalidate(void *vaddr, size_t size)
{
	dma_map_single(pp_dev_get(), vaddr, size, DMA_FROM_DEVICE);
	pr_debug("pp dma inv done virt=%ps size=%zu\n", vaddr, size);
}
