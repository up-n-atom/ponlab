// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2024 MaxLinear, Inc.
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

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/tee_drv.h>
#include <linux/mutex.h>
#include <linux/errno.h>
#include <linux/genalloc.h>

#include <linux/icc/drv_icc.h>
#include <private.h>

/* private pool manager operations */
static int priv_mgr_pool_alloc(struct tee_shm_pool_mgr *poolm, struct tee_shm *shm, size_t size)
{
	return 0;
}

static void priv_mgr_pool_free(struct tee_shm_pool_mgr *poolm, struct tee_shm *shm)
{
}

static void priv_mgr_pool_destroy(struct tee_shm_pool_mgr *poolm)
{
}

static const struct tee_shm_pool_mgr_ops priv_mgr_pool_ops = {
	.alloc = priv_mgr_pool_alloc,
	.free = priv_mgr_pool_free,
	.destroy_poolmgr = priv_mgr_pool_destroy
};


/* dma pool manager operations */
static int dma_mgr_pool_alloc(struct tee_shm_pool_mgr *poolm, struct tee_shm *shm, size_t size)
{
	unsigned int order = get_order(size);
	unsigned long va;

	pr_debug("%s() requested memory size:%lu\n", __func__, size);

	va = __get_free_pages(GFP_KERNEL | __GFP_ZERO, order);
	if (!va)
		return -ENOMEM;

	shm->kaddr = (void *)va;
	shm->paddr = __pa(va);
	shm->size = PAGE_SIZE << order;
	return 0;
}

static void dma_mgr_pool_free(struct tee_shm_pool_mgr *poolm, struct tee_shm *shm)
{
	pr_debug("%s() released memory size:%lu\n", __func__, shm->size);

	free_pages((unsigned long)shm->kaddr, get_order(shm->size));
	shm->kaddr = NULL;
	shm->paddr = (phys_addr_t)NULL;
}

static void dma_mgr_pool_destroy(struct tee_shm_pool_mgr *poolm)
{
	kfree(poolm);
}

static const struct tee_shm_pool_mgr_ops dma_mgr_pool_ops = {
	.alloc = dma_mgr_pool_alloc,
	.free = dma_mgr_pool_free,
	.destroy_poolmgr = dma_mgr_pool_destroy
};

static struct tee_shm_pool_mgr *shm_pool_manager_alloc(struct mxltee_driver *driver, bool dma)
{
	struct tee_shm_pool_mgr *manager = kzalloc(sizeof(struct tee_shm_pool_mgr), GFP_KERNEL);

	if (IS_ERR_OR_NULL(manager)) {
		pr_err("shared memory manager allocation failed\n");
		return ERR_PTR(-ENOMEM);
	}
	if (dma) {
		manager->ops = &dma_mgr_pool_ops;
		manager->private_data = driver;
	} else {
		manager->ops = &priv_mgr_pool_ops;
	}
	return manager;
}

/**
 * mxltee_register_shm_pool() - registers the shared memory pool
 */
struct tee_shm_pool *mxltee_register_shm_pool(struct mxltee_driver *driver)
{
	struct tee_shm_pool_mgr *dma_mgr = NULL;
	struct tee_shm_pool_mgr *priv_mgr = NULL;
	struct tee_shm_pool *shm_pool = NULL;

	dma_mgr =  shm_pool_manager_alloc(driver, true);
	if (IS_ERR_OR_NULL(dma_mgr))
		return ERR_PTR(-ENOMEM);

	priv_mgr =  shm_pool_manager_alloc(driver, false);
	if (IS_ERR_OR_NULL(priv_mgr)) {
		tee_shm_pool_mgr_destroy(dma_mgr);
		return ERR_PTR(-ENOMEM);
	}

	shm_pool = tee_shm_pool_alloc(priv_mgr, dma_mgr);
	if (IS_ERR_OR_NULL(shm_pool)) {
		tee_shm_pool_mgr_destroy(priv_mgr);
		tee_shm_pool_mgr_destroy(dma_mgr);
	}

	return shm_pool;
}
