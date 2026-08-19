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
 * Description: PP Driver DMA API by genpool implementation
 */

#include <linux/kernel.h>
#include <linux/dma-mapping.h>
#include <linux/types.h>
#include <linux/genalloc.h>
#include <soc/mxl/mxl_sec.h>
#include <soc/mxl/mxl_gen_pool.h>
#include "pp_dma.h"
#include "pp_common.h"

#ifdef pr_fmt
#undef pr_fmt
#define pr_fmt(fmt) "[PP_DMA]:%s:%d: " fmt, __func__, __LINE__
#endif

/* @TODO remove me */
#define DMA_ATTR_NON_CONSISTENT		(1UL << 3)
struct pp_genpool {
	struct gen_pool *pool;
	unsigned long vaddr;
	dma_addr_t paddr;
	ssize_t size;
};

static struct pp_genpool pp_ioc_pool;
static struct pp_genpool pp_nioc_pool;

/**
 * @brief get the platform gen pool
 * @param data mxl pool allocation data
 * @return struct gen_pool* platform gen pool, NULL if failed
 */
static struct gen_pool *platform_pool_get(struct mxl_pool_alloc_data *data)
{
	struct gen_pool *p = NULL;

	memset(data, 0, sizeof(*data));
	data->dev = pp_dev_get();
	data->sai = -1;
	data->perm = FW_READ_WRITE;
	p = of_gen_pool_get(data->dev->of_node, "rwpool", 0);
	if (!p)
		pr_err("failed to get the platform pool\n");

	return p;
}

/**
 * @brief get the platform gen pool
 * @param pool pp pool to create
 * @param vaddr virtual address of the buffer to add
 * @param paddr physical address of the buffer to add
 * @param size size of the buffer to add
 * @return s32 0 on success, error code otherwise
 */
static s32 pp_gen_pool_create(struct pp_genpool *pool, unsigned long vaddr,
			      dma_addr_t paddr, ssize_t size)
{
	s32 ret;

	pool->size = size;
	pool->paddr = paddr;
	pool->vaddr = vaddr;
	pool->pool = gen_pool_create(ilog2(SZ_4K), -1);
	if (!pool->pool) {
		pr_err("pool create failed\n");
		return -EINVAL;
	}
	/* add the buffer to the new pool */
	ret = gen_pool_add_virt(pool->pool, pool->vaddr, pool->paddr,
				pool->size, -1);
	if (ret) {
		pr_err("failed to add a new chunk of memory to the pool\n");
		gen_pool_destroy(pool->pool);
		pool->pool = NULL;
		return ret;
	}

	pr_debug("buffer virt=0x%lx phys=%pad size=%zu added to pool\n",
		 pool->vaddr, &pool->paddr, pool->size);

	return 0;
}

void pp_dma_exit(struct platform_device *pdev)
{
	struct mxl_pool_alloc_data data;
	struct gen_pool *platform_pool;

	/* destroy ioc pool */
	if (pp_ioc_pool.pool) {
		gen_pool_destroy(pp_ioc_pool.pool);
		pp_ioc_pool.pool = NULL;
	}

	platform_pool = platform_pool_get(&data);
	if (!platform_pool)
		return;

	/* free the buffer */
	if (pp_ioc_pool.size > 0)
		gen_pool_free(platform_pool, (unsigned long)pp_ioc_pool.vaddr,
							pp_ioc_pool.size);

	/* destroy nioc pool */
	if (pp_nioc_pool.pool) {
		gen_pool_destroy(pp_nioc_pool.pool);
		pp_nioc_pool.pool = NULL;
	}

	/* free the buffer */
	if (pdev) {
		dma_unmap_single_attrs(&pdev->dev, pp_nioc_pool.paddr,
				pp_nioc_pool.size, DMA_TO_DEVICE,
				DMA_ATTR_NON_CONSISTENT);
	}

	if (pp_nioc_pool.size > 0)
		gen_pool_free(platform_pool, pp_nioc_pool.vaddr,
							pp_nioc_pool.size);
}

s32 pp_dma_init(struct platform_device *pdev)
{
	struct mxl_pool_alloc_data data;
	struct gen_pool *platform_pool = NULL;
	unsigned long ioc_vaddr, nioc_vaddr;
	dma_addr_t ioc_paddr, nioc_paddr;
	struct pp_dev_priv *priv;
	ssize_t ioc_sz;  /* ioc memory to allocate */
	ssize_t nioc_sz; /* nioc memory to allocate */
	s32 ret;

	if (unlikely(ptr_is_null(pdev)))
		return -EINVAL;

	priv = platform_get_drvdata(pdev);
	if (unlikely(ptr_is_null(priv)))
		return -EINVAL;

	ioc_sz = priv->dts_cfg.dma_ioc_sz;
	nioc_sz = priv->dts_cfg.dma_nioc_sz;

	ret = dma_set_mask_and_coherent(pp_dev_get(), DMA_BIT_MASK(36));
	if (ret) {
		pr_err("Set dma bitmask to 36 failed!\n");
		return ret;
	}

	platform_pool = platform_pool_get(&data);
	if (!platform_pool)
		return -EINVAL;

	/* allocate the IOC buffer from platform r/w pool */
	ioc_vaddr = mxl_soc_pool_alloc(platform_pool, ioc_sz, &data);
	if (!ioc_vaddr) {
		pr_err("failed to allocate ioc buf from platform pool\n");
		return -EINVAL;
	}
	/* map to phys address */
	ioc_paddr = __pa(ioc_vaddr);
	pr_debug("buffer virt=0x%lx phys=%pad size=%zu allocated from platform pool\n",
		 ioc_vaddr, &ioc_paddr, ioc_sz);

	/* create the IOC pp gen_pool */
	ret = pp_gen_pool_create(&pp_ioc_pool, ioc_vaddr, ioc_paddr, ioc_sz);
	if (ret)
		goto free_ioc_mem;

	/* allocate the NIOC buffer from platform r/w pool */
	data.opt = MXL_FW_OPT_USE_NONCOHERENT;
	nioc_vaddr = mxl_soc_pool_alloc(platform_pool, nioc_sz, &data);
	if (!nioc_vaddr) {
		pr_err("failed to allocate nioc buf from platform pool\n");
		goto free_ioc_pool;
	}

	/* map the buffer as NIOC */
	nioc_paddr = dma_map_single_attrs(pp_dev_get(), (void *)nioc_vaddr,
					  nioc_sz, DMA_TO_DEVICE,
					  DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(pp_dev_get(), nioc_paddr)) {
		pr_err("DMA map failed\n");
		goto free_nioc_mem;
	}

	pr_debug("buffer virt=0x%lx phys=%pad size=%zu allocated from platform pool\n",
		 nioc_vaddr, &nioc_paddr, nioc_sz);

	/* create the NIOC pp gen_pool */
	ret = pp_gen_pool_create(&pp_nioc_pool, nioc_vaddr, nioc_paddr,
				 nioc_sz);
	if (ret)
		goto unmap_nioc_mem;

	return 0;

unmap_nioc_mem:
	dma_unmap_single_attrs(pp_dev_get(), nioc_paddr, nioc_sz, DMA_TO_DEVICE,
			       DMA_ATTR_NON_CONSISTENT);
free_nioc_mem:
	gen_pool_free(platform_pool, nioc_vaddr, nioc_sz);
free_ioc_pool:
	gen_pool_destroy(pp_ioc_pool.pool);
	pp_ioc_pool.pool = NULL;
free_ioc_mem:
	gen_pool_free(platform_pool, ioc_vaddr, ioc_sz);

	return ret;
}

void *pp_dma_alloc(ssize_t size, dma_addr_t *paddr, unsigned long attr)
{
	void *vaddr;
	struct pp_genpool *pool;

	pr_debug("pp genpool alloc size=%zu\n", size);

	if (!paddr) {
		pr_err("no phys address, abort.\n");
		return NULL;
	}

	/* select pool ioc/nioc */
	if (attr & PP_DMA_NONECOHERENT)
		pool = &pp_nioc_pool;
	else
		pool = &pp_ioc_pool;

	if (!pool->pool) {
		pr_err("pp pool is not allocated\n");
		return NULL;
	}

	/* verify requested size is available */
	if (size > gen_pool_avail(pool->pool)) {
		pr_err("pp gen pool is too small, pool_size=%zu, available=%zu, requested=%zu\n",
			gen_pool_size(pool->pool), gen_pool_avail(pool->pool),
			size);
		return NULL;
	}

	/* allocate from pool */
	vaddr = gen_pool_dma_alloc(pool->pool, size, paddr);
	if (!vaddr) {
		pr_err("failed to allocate from pp pool\n");
		return NULL;
	}

	pr_debug("pp genpool allocated buffer virt=%ps phys=%pad size=%zu, pool_size=%zu, available=%zu\n",
		 vaddr, paddr, size, gen_pool_size(pool->pool),
		 gen_pool_avail(pool->pool));

	return vaddr;
}

void pp_dma_free(void *vaddr, ssize_t size)
{
	unsigned long v = (unsigned long)vaddr;

	pr_debug("pp genpool free virt=0x%lx size=%zu\n", v, size);

	if (pp_ioc_pool.pool && gen_pool_has_addr(pp_ioc_pool.pool, v, size))
		gen_pool_free(pp_ioc_pool.pool, v, size);
	else if (pp_nioc_pool.pool &&
		 gen_pool_has_addr(pp_nioc_pool.pool, v, size))
		gen_pool_free(pp_nioc_pool.pool, v, size);
	else
		pr_err("address %ps is not from pp pool\n", vaddr);
}

void pp_cache_writeback(void *vaddr, size_t size)
{
	dma_addr_t paddr;

	if (pp_nioc_pool.pool &&
	    gen_pool_has_addr(pp_nioc_pool.pool, (unsigned long)vaddr, size)) {
		paddr = gen_pool_virt_to_phys(pp_nioc_pool.pool,
					      (unsigned long)vaddr);
		dma_sync_single_for_device(pp_dev_get(), paddr, size,
					   DMA_TO_DEVICE);
		pr_debug("pp genpool wb done virt=%ps phys=%pad size=%zu\n",
			 vaddr, &paddr, size);
	}
}

void pp_cache_invalidate(void *vaddr, size_t size)
{
	dma_addr_t paddr;

	if (pp_nioc_pool.pool &&
	    gen_pool_has_addr(pp_nioc_pool.pool, (unsigned long)vaddr, size)) {
		paddr = gen_pool_virt_to_phys(pp_nioc_pool.pool,
					      (unsigned long)vaddr);
		dma_sync_single_for_cpu(pp_dev_get(), paddr, size,
					DMA_FROM_DEVICE);
		pr_debug("pp genpool inv done virt=%ps phys=%pad size=%zu\n",
			 vaddr, &paddr, size);
	}
}

ssize_t pp_dma_get_pool_size(bool is_ioc)
{
	if (is_ioc && pp_ioc_pool.pool)
		return gen_pool_size(pp_ioc_pool.pool);
	else if (!is_ioc && pp_nioc_pool.pool)
		return gen_pool_size(pp_nioc_pool.pool);

	return 0;
}

ssize_t pp_dma_get_pool_avail(bool is_ioc)
{
	if (is_ioc && pp_ioc_pool.pool)
		return gen_pool_avail(pp_ioc_pool.pool);
	else if (!is_ioc && pp_nioc_pool.pool)
		return gen_pool_avail(pp_nioc_pool.pool);

	return 0;
}
