// SPDX-License-Identifier: GPL-2.0
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

#include <asm/dma-mapping.h>
#include <linux/genalloc.h>
#include <linux/kernel.h>
#include <linux/notifier.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <soc/mxl/mxl_sec.h>
#include <soc/mxl/mxl_gen_pool.h>
#include <dt-bindings/soc/mxl,firewall.h>

/**
 * struct lgm_gen_pool: pool info
 * @list: list node of pool
 * @np: device node of pool in DT
 * @pool: gen_pool pointer
 * @vaddr: virtual start address of pool
 * @paddr: physical start address of pool
 * @size: pool size
 * @perm: permssion of MEMAXI
 */
struct lgm_gen_pool {
	struct list_head list;
	struct device_node *np;
	struct gen_pool *pool;
	unsigned long vaddr;
	dma_addr_t paddr;
	size_t size;
	enum fw_perm perm;
};

/**
 * struct lgm_gen_pool_dev: driver private structure
 * @dev: device pointer
 * @pool_hdr: list header of lgm_gen_pool list
 * @align: pool align, SZ_4K in LGM
 * @total_size_align: total pool size alignment,
 *                    used in firewall memory layout re-arrangement
 *                    it is required to align with CONFIG_CMA_ALIGNMENT
 * @vaddr: virtual start address of whole memory pools
 * @paddr: physical start address of whole memory pools
 * @size:  size of sum of memory pool
 * @rw_size: total size of rw permission pools
 * @ro_size: total size of read only permission pools
 * @nb: notify chain to get pool address and size
 */
struct lgm_gen_pool_dev {
	struct device *dev;
	struct list_head pool_hdr;
	unsigned int align;
	unsigned int total_size_align;
	unsigned long vaddr;
	dma_addr_t paddr;
	size_t size;
	size_t rw_size;
	size_t ro_size;
	struct notifier_block nb;
};

static unsigned long cma_size;
static size_t cma_alignment = SZ_4K;

/**
 * Get the cma size from kernel parameter
 */
static int __init lgm_cma_memsize_setup(char *p)
{
	cma_size = memparse(p, &p);
	cma_alignment = BIT(CONFIG_CMA_ALIGNMENT) * SZ_4K;

	return 0;
}
__setup("cma=", lgm_cma_memsize_setup);

static int lgm_gen_pool_add_rule(struct device *dev, int sai,
				 phys_addr_t start_addr, size_t size,
				 unsigned int perm, unsigned int act,
				 unsigned long opt)
{
	struct fw_nb_data fwdata = {0};
	int ret;

	fwdata.dev = dev;
	fwdata.sai = sai;
	fwdata.size = size;
	if (opt & MXL_FW_OPT_USE_NONCOHERENT)
		fwdata.start_addr = ioc_addr_to_nioc_addr(start_addr);
	else
		fwdata.start_addr = start_addr;
	fwdata.permission = perm;

	ret = mxl_fw_notifier_blocking_chain(act, &fwdata);

	return notifier_to_errno(ret);
}

void mxl_soc_pool_free(struct gen_pool *pool,
		       struct gen_pool_chunk *chunk, void *data)
{
	struct mxl_pool_free_data *usrdata = data;
	unsigned long vaddr = usrdata->vaddr;
	size_t size = usrdata->size;
	phys_addr_t start_addr;

	if (chunk->start_addr > vaddr || chunk->end_addr < vaddr)
		return;

	gen_pool_free(pool, vaddr, size);
	if (!usrdata || (usrdata->opt & MXL_FW_OPT_SKIP_HW_FWRULE))
		return;

	start_addr = chunk->phys_addr + vaddr - chunk->start_addr;
	lgm_gen_pool_add_rule(usrdata->dev, usrdata->sai, start_addr,
			      size, usrdata->perm, NOC_FW_EVENT_DEL,
			      usrdata->opt);
}
EXPORT_SYMBOL_GPL(mxl_soc_pool_free);

unsigned long mxl_soc_pool_alloc(struct gen_pool *pool, size_t size, void *allocdata)
{
	unsigned long addr;
	struct mxl_pool_alloc_data *usrdata = allocdata;

	if (!pool || !usrdata)
		return 0;

	addr = gen_pool_alloc_algo(pool, size, gen_pool_first_fit, NULL);
	if (!addr)
		return addr;

	if (usrdata->opt & MXL_FW_OPT_SKIP_HW_FWRULE)
		return addr;

	if (lgm_gen_pool_add_rule(usrdata->dev, usrdata->sai, __pa(addr), size,
				  usrdata->perm, NOC_FW_EVENT_ADD,
				  usrdata->opt)) {
		gen_pool_free(pool, addr, size);
		return 0;
	}

	return addr;
}
EXPORT_SYMBOL_GPL(mxl_soc_pool_alloc);

#if 0
/* This API is deprecated, instead use mxl_soc_pool_alloc */

unsigned long
mxl_soc_pool_algo(unsigned long *map, unsigned long size,
		  unsigned long start, unsigned int nr, void *data,
		  struct gen_pool *pool, unsigned long start_addr)
{
	unsigned long addr;
	struct mxl_pool_alloc_data *usrdata = data;
	struct gen_pool_chunk *chunk;
	int order = pool->min_alloc_order;

	addr = gen_pool_first_fit(map, size, start, nr,
				  data, pool, start_addr);

	rcu_read_lock();
	list_for_each_entry_rcu(chunk, &pool->chunks, next_chunk) {
		if (chunk->start_addr == start_addr)
			break;
	}
	rcu_read_unlock();

	if (!usrdata || (usrdata->opt & MXL_FW_OPT_SKIP_HW_FWRULE))
		return addr;

	lgm_gen_pool_add_rule(usrdata->dev, usrdata->sai,
			      chunk->phys_addr + (addr << order),
			      nr << order, usrdata->perm, NOC_FW_EVENT_ADD,
			      usrdata->opt);

	return addr;
}
EXPORT_SYMBOL_GPL(mxl_soc_pool_algo);
#endif

static int lgm_gen_pool_dt_probe(struct lgm_gen_pool_dev *genpl_dev)
{
	struct device_node *np, *child;
	u32 pool_size;
	struct lgm_gen_pool *pool = NULL;
	struct device *dev = genpl_dev->dev;
	unsigned int align = genpl_dev->align;

	np = genpl_dev->dev->of_node;

	for_each_child_of_node(np, child) {
		if (of_property_read_u32(child, "size", &pool_size)) {
			dev_err(dev, "np(%s) doesn't have size!\n",
				child->name);
			continue;
		}

		pool = kmalloc(sizeof(*pool), GFP_KERNEL);
		if (!pool)
			return -ENOMEM;

		if (of_property_read_u32(child, "perm", &pool->perm)) {
			dev_err(dev, "np(%s) doesn't have perm!\n",
				child->name);
			kfree(pool);
			pool = NULL;
			continue;
		}
		pool->np = child;
		pool->size = ALIGN(pool_size, align);
		genpl_dev->size += pool->size;
		list_add_tail(&pool->list, &genpl_dev->pool_hdr);
	}
	if (!pool) {
		dev_err(dev, "dt doesn't find np\n");
		return -ENOMEM;
	}
	pool_size = ALIGN(genpl_dev->size, genpl_dev->total_size_align);
	/* enlarge the last pool if we have alignment adjustment for total size */
	pool->size += pool_size - genpl_dev->size;
	genpl_dev->size = pool_size;

	return 0;
}

/**
 * noc firewall requires genpool memory to be arranged by end of the CMA region.
 * To achieve this, genpool driver allocate all CMA memory, then free the unused
 * memory to make the pool memory on the end of CMA region
 */
static int lgm_gen_pool_alloc_mem(struct lgm_gen_pool_dev *genpl_dev)
{
	struct device *dev = genpl_dev->dev;
	void *p, *p_rest = NULL;
	size_t off, rest_mem_size;
	dma_addr_t rest_paddr;

	if (cma_size) {
		rest_mem_size = cma_size - genpl_dev->size;
		p_rest = dma_alloc_coherent(dev, rest_mem_size,
					    &rest_paddr, GFP_KERNEL);
		if (!p_rest)
			return -ENOMEM;
	}
	p = dma_alloc_coherent(dev, genpl_dev->size,
			       &genpl_dev->paddr, GFP_KERNEL);
	if (!p)
		return -ENOMEM;

	dev_dbg(dev, "vaddr: 0x%lx, aligned vaddr: 0x%lx, size: 0x%lx\n",
		(unsigned long)p, ALIGN((unsigned long)p, genpl_dev->align),
		genpl_dev->size);

	if (p_rest)
		dma_free_coherent(dev, rest_mem_size, p_rest, rest_paddr);

	off = ALIGN((unsigned long)p, genpl_dev->align) - (unsigned long)p;
	if (off) {
		genpl_dev->paddr += off;
		genpl_dev->vaddr = (unsigned long)p + off;
		genpl_dev->size -= off;

	} else {
		genpl_dev->vaddr = (unsigned long)p;
	}

	return 0;
}

static int lgm_gen_pool_create(struct lgm_gen_pool_dev *genpl_dev)
{
	struct device *dev = genpl_dev->dev;
	struct lgm_gen_pool *pool;
	unsigned long vaddr = genpl_dev->vaddr;
	dma_addr_t paddr = genpl_dev->paddr;
	int ret;

	list_for_each_entry(pool, &genpl_dev->pool_hdr, list) {
		pool->pool = devm_gen_pool_create(dev, ilog2(genpl_dev->align),
						  -1, pool->np->name);
		if (!pool->pool) {
			dev_err(dev, "Failed to create pool for %s\n",
				pool->np->name);
			return -EINVAL;
		}
		dev_dbg(dev, "Pool %s created\n", pool->np->name);

		ret = gen_pool_add_virt(pool->pool, vaddr, paddr,
					pool->size, -1);
		if (ret) {
			dev_err(dev, "Failed add memory for pool %s\n",
				pool->np->name);
			gen_pool_destroy(pool->pool);
			return -EINVAL;
		}
		dev_dbg(dev, "pool vaddr: 0x%lx, paddr: 0x%llx, size: 0x%lx\n",
			vaddr, paddr, pool->size);

		pool->paddr = paddr;
		vaddr += pool->size;
		paddr += pool->size;

		if (pool->perm == FW_READ_WRITE)
			genpl_dev->rw_size += pool->size;
		else
			genpl_dev->ro_size += pool->size;
	}

	dev_dbg(dev, "genpool rw pool:(0x%llx, 0x%lx), ro pool:(0x%llx, 0x%lx)\n",
		genpl_dev->paddr, genpl_dev->rw_size,
		genpl_dev->paddr + genpl_dev->rw_size, genpl_dev->ro_size);

	return 0;
}

/**
 * This function has assumption that gools of same rule are grouped together
 * It create MEMAXI rules for POOLs
 */
static void __maybe_unused lgm_gen_pool_rule_create(struct lgm_gen_pool_dev *genpl_dev)
{
	struct device *dev = genpl_dev->dev;
	struct lgm_gen_pool *pool, *prepool = NULL;
	dma_addr_t paddr = genpl_dev->paddr;
	size_t size = 0;

	list_for_each_entry(pool, &genpl_dev->pool_hdr, list) {
		if (!prepool || prepool->perm == pool->perm) {
			size += pool->size;
			prepool = pool;
			continue;
		}

		if (lgm_gen_pool_add_rule(dev, LGM_SAI_MEMAXI, paddr, size,
					  prepool->perm,
					  NOC_FW_EVENT_ADD_SPL, 0)) {
			dev_err(genpl_dev->dev,
				"Failed to create MEMAXI rule\n");
		}
		paddr += size;
		size = pool->size;
		prepool = pool;
	}
	if (!prepool) {
		dev_err(genpl_dev->dev, "Failed to get prepool value\n");
		return;
	}
	if (lgm_gen_pool_add_rule(dev, LGM_SAI_MEMAXI, paddr, size,
				  prepool->perm, NOC_FW_EVENT_ADD_SPL, 0)) {
		dev_err(genpl_dev->dev, "Failed to create MEMAXI rule\n");
	}
}

static BLOCKING_NOTIFIER_HEAD(genpl_blocking_chain);

int mxl_noc_genpl_notifier_blocking_chain(unsigned long val, void *v)
{
	return blocking_notifier_call_chain(&genpl_blocking_chain, val, v);
}
EXPORT_SYMBOL_GPL(mxl_noc_genpl_notifier_blocking_chain);

/**
 * return pool base address and size based on pool name
 * This api provide event to pool name mappping
 * Note: pool name must be same as DT pool name
 */
static int genpl_blocking_notify(struct notifier_block *nb, unsigned long event,
			      void *data)
{
	struct lgm_gen_pool_dev *genpl_dev;
	struct lgm_gen_pool *pool;
	struct mxl_pool_event_data *pdata = data;
	int ret = -EINVAL;
	const char *pool_name[] = {
		"cpu_pool",
		"rw_pool",
		"icc_pool",
		"ro_pool",
		"sys_pool",
	};

	if (!pdata)
		return notifier_from_errno(ret);

	genpl_dev = container_of(nb, struct lgm_gen_pool_dev, nb);
	if (event == NOC_GENPL_EVENT_GENPOOL) {
		pdata->paddr = genpl_dev->paddr;
		pdata->size = genpl_dev->size;

		return NOTIFY_OK;
	}

	/* looking for the perticular pool */
	if (event >= array_size(sizeof(pool_name), sizeof(pool_name[0])))
		return notifier_from_errno(ret);

	list_for_each_entry(pool, &genpl_dev->pool_hdr, list) {
		if (strcmp(pool->np->name, pool_name[event]) == 0) {
			pdata->paddr = pool->paddr;
			pdata->size = pool->size;

			return NOTIFY_OK;
		}
	}

	return notifier_from_errno(ret);
}

static int lgm_gen_pool_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct lgm_gen_pool_dev *genpl_dev;
	int ret;

	genpl_dev = devm_kzalloc(dev, sizeof(*genpl_dev), GFP_KERNEL);
	if (!genpl_dev)
		return -ENOMEM;

	genpl_dev->dev = dev;
	genpl_dev->align = SZ_4K;
	genpl_dev->total_size_align = cma_alignment;
	INIT_LIST_HEAD(&genpl_dev->pool_hdr);
	platform_set_drvdata(pdev, genpl_dev);

	ret = lgm_gen_pool_dt_probe(genpl_dev);
	if (ret)
		return ret;

	dma_set_mask(dev, DMA_BIT_MASK(36));
	dma_set_coherent_mask(dev, DMA_BIT_MASK(35)); /* maximum coherent address is up to 0x43fffffff */

	ret = lgm_gen_pool_alloc_mem(genpl_dev);
	if (ret)
		return ret;

	lgm_gen_pool_create(genpl_dev);
	//lgm_gen_pool_rule_create(genpl_dev);
	genpl_dev->nb.notifier_call = genpl_blocking_notify;
	blocking_notifier_chain_register(&genpl_blocking_chain, &genpl_dev->nb);

	dev_info(dev, "gen pool driver loaded!\n");
	return 0;
}

static const struct of_device_id lgm_gen_pool_dt_ids[] = {
	{.compatible = "mxl,lgm-gen-pool" },
	{ },
};

static struct platform_driver lgm_gen_pool_driver = {
	.probe = lgm_gen_pool_probe,
	.driver = {
		.name = "mxl-gen-pool",
		.of_match_table = of_match_ptr(lgm_gen_pool_dt_ids),
		/* Firewall driver cannot be removed. */
		.suppress_bind_attrs = true,
	},
};

static int __init lgm_gen_pool_drv_register(void)
{
	return platform_driver_register(&lgm_gen_pool_driver);
}
arch_initcall(lgm_gen_pool_drv_register);
