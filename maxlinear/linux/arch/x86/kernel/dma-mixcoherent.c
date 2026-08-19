// SPDX-License-Identifier: GPL-2.0
/*
 * DMA mix coherent operation
 *
 * Copyright (C) 2021 Maxlinear Corporation.
 *
 * This adds implementation of DMA mix coherent operations that allows
 * both non-coherent and/or coherent operations for I/O.
 * The purpose of this dma ops is to provide a hybrid of IO-coherent
 * & non-coherent access to those devices that need it.
 * In order to use this ops, driver need to:
 * - remove 'dma-coherent' property from its node DT
 *    OR
 * - install ops manually via set_dma_ops(dev, &mixcoherent_dma_ops)
 *
 * After that, DMA_ATTR_NON_CONSISTENT flag can be passed in each
 * dma op to specify whether coherent/non-coherent access is needed.
 */
#include <linux/dma-direct.h>
#include <linux/dma-mapping.h>
#include <linux/highmem.h>

#include <asm/set_memory.h>

#define dma_to_page(dev, addr)	pfn_to_page(PFN_DOWN(addr))

static void *dma_mixcoherent_alloc(struct device *dev, size_t size,
				   dma_addr_t *dma_handle, gfp_t gfp,
				   unsigned long attrs)
{
	void *addr;
	unsigned int num_pages;

	addr = dma_direct_alloc(dev, size, dma_handle, gfp, attrs);
	if (!addr)
		return NULL;

	if (attrs & DMA_ATTR_NON_CONSISTENT) {
		/* for backward compatibility */
		if (dma_get_mask(dev) >= DMA_BIT_MASK(36))
			*dma_handle = ioc_addr_to_nioc_addr(*dma_handle);
	} else if (attrs & DMA_ATTR_WRITE_COMBINE) {
		num_pages = PAGE_ALIGN(size) >> PAGE_SHIFT;
		set_memory_wc((unsigned long)addr, num_pages);
	}

	return addr;
}

static void dma_mixcoherent_free(struct device *dev, size_t size,
				 void *cpu_addr, dma_addr_t dma_addr,
				 unsigned long attrs)
{
	unsigned int num_pages;

	/* for backward compatibility */
	if (attrs & DMA_ATTR_NON_CONSISTENT) {
		dma_addr = nioc_addr_to_ioc_addr(dma_addr);
	} else if (attrs & DMA_ATTR_WRITE_COMBINE) {
		num_pages = PAGE_ALIGN(size) >> PAGE_SHIFT;
		set_memory_wb((unsigned long)cpu_addr, num_pages);
	}

	dma_direct_free(dev, size, cpu_addr, dma_addr, attrs);
}

static struct page *dma_mixcoherent_alloc_pages(struct device *dev, size_t size,
						dma_addr_t *dma_handle,
						enum dma_data_direction dir,
						gfp_t gfp)
{
	void *addr;

	addr = dma_mixcoherent_alloc(dev, size, dma_handle, gfp, 0);
	if (!addr)
		return NULL;

	*dma_handle = ioc_addr_to_nioc_addr(*dma_handle);

	return virt_to_page(addr);
}

static void dma_mixcoherent_free_pages(struct device *dev, size_t size,
				       struct page *page, dma_addr_t dma_addr,
				       enum dma_data_direction dir)
{
	dma_addr = nioc_addr_to_ioc_addr(dma_addr);
	dma_mixcoherent_free(dev, size, page_address(page), dma_addr, 0);
}

static void __dma_page_sync(struct device *dev, struct page *page,
			    unsigned long off, size_t size,
			    unsigned long attrs)
{
	size_t left = size;
	unsigned long pfn;

	pfn = page_to_pfn(page) + off / PAGE_SIZE;
	off %= PAGE_SIZE;

	do {
		void *vaddr;
		size_t len = left;

		page = pfn_to_page(pfn);
		if (PageHighMem(page)) {
			if (len + off > PAGE_SIZE)
				len = PAGE_SIZE - off;

			vaddr = kmap_atomic(page);
			clflush_cache_range(vaddr, len);
			kunmap_atomic(vaddr);
		} else {
			vaddr = page_address(page) + off;
			clflush_cache_range(vaddr, len);
		}
		off = 0;
		pfn++;
		left -= len;
	} while (left);
}

static void dma_mixcoherent_sync_dma(struct device *dev, phys_addr_t paddr,
				     size_t size, enum dma_data_direction dir)
{
	unsigned int offset = paddr & (PAGE_SIZE - 1);
	struct page *page = dma_to_page(dev, paddr - offset);

	__dma_page_sync(dev, page, offset, size, 0);
}

static void dma_mixcoherent_sync_single_for_device(struct device *dev,
						   dma_addr_t addr, size_t size,
						   enum dma_data_direction dir)
{
	addr = nioc_addr_to_ioc_addr(addr);
	dma_mixcoherent_sync_dma(dev, dma_to_phys(dev, addr), size, dir);
}

static void dma_mixcoherent_sync_sg_for_device(struct device *dev,
					       struct scatterlist *sgl,
					       int nents,
					       enum dma_data_direction dir)
{
	struct scatterlist *sg;
	int i;

	for_each_sg(sgl, sg, nents, i)
		dma_mixcoherent_sync_single_for_device(dev, sg_dma_address(sg),
						       sg_dma_len(sg), dir);
}

static void dma_mixcoherent_sync_single_for_cpu(struct device *dev,
						dma_addr_t addr, size_t size,
						enum dma_data_direction dir)
{
	addr = nioc_addr_to_ioc_addr(addr);
	dma_mixcoherent_sync_dma(dev, dma_to_phys(dev, addr), size, dir);
}

static void dma_mixcoherent_sync_sg_for_cpu(struct device *dev,
					    struct scatterlist *sgl, int nents,
					    enum dma_data_direction dir)
{
	struct scatterlist *sg;
	int i;
	dma_addr_t addr;

	for_each_sg(sgl, sg, nents, i) {
		addr = nioc_addr_to_ioc_addr(sg_dma_address(sg));
		dma_mixcoherent_sync_dma(dev, addr, sg_dma_len(sg), dir);
	}
}

static int dma_mixcoherent_mmap(struct device *dev, struct vm_area_struct *vma,
				void *cpu_addr, dma_addr_t dma_addr,
				size_t size, unsigned long attrs)
{
	int ret = -ENXIO;
#ifdef CONFIG_MMU
	unsigned long user_count = (vma->vm_end - vma->vm_start) >> PAGE_SHIFT;
	unsigned long count = PAGE_ALIGN(size) >> PAGE_SHIFT;
	unsigned long pfn = page_to_pfn(virt_to_page(cpu_addr));
	unsigned long off = vma->vm_pgoff;

	if ((DMA_ATTR_WRITE_COMBINE & attrs)) {
		vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
	} else if ((DMA_ATTR_NON_CONSISTENT & attrs) == 0) {
		/* for backward compatibility */
		vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
		if (dma_mmap_from_dev_coherent(dev, vma, cpu_addr, size, &ret))
			return ret;
	}

	if (off < count && user_count <= (count - off)) {
		ret = remap_pfn_range(vma, vma->vm_start,
				      pfn + off,
				      user_count << PAGE_SHIFT,
				      vma->vm_page_prot);
	}
#endif	/* CONFIG_MMU */
	return ret;
}

static inline void _dma_direct_sync_single_for_cpu(struct device *dev,
						   dma_addr_t addr, size_t size,
						   enum dma_data_direction dir)
{
	phys_addr_t paddr = dma_to_phys(dev, addr);

	if (!dev_is_dma_coherent(dev)) {
		arch_sync_dma_for_cpu(paddr, size, dir);
		arch_sync_dma_for_cpu_all();
	}

	if (unlikely(is_swiotlb_buffer(dev, paddr)))
		swiotlb_sync_single_for_cpu(dev, paddr, size, dir);

	if (dir == DMA_FROM_DEVICE)
		arch_dma_mark_clean(paddr, size);
}

static inline dma_addr_t _dma_direct_map_page(struct device *dev,
					      struct page *page,
					      unsigned long offset, size_t size,
					      enum dma_data_direction dir,
					      unsigned long attrs)
{
	phys_addr_t phys = page_to_phys(page) + offset;
	dma_addr_t dma_addr = phys_to_dma(dev, phys);

	if (is_swiotlb_force_bounce(dev))
		return swiotlb_map(dev, phys, size, dir, attrs);

	if (unlikely(!dma_capable(dev, dma_addr, size, true))) {
		if (swiotlb_force != SWIOTLB_NO_FORCE)
			return swiotlb_map(dev, phys, size, dir, attrs);

		dev_WARN_ONCE(dev, 1,
			      "DMA addr %pad+%zu overflow (mask %llx, bus limit %llx).\n",
			      &dma_addr, size, *dev->dma_mask, dev->bus_dma_limit);
		return DMA_MAPPING_ERROR;
	}

	if (!dev_is_dma_coherent(dev) && !(attrs & DMA_ATTR_SKIP_CPU_SYNC))
		arch_sync_dma_for_device(phys, size, dir);
	return dma_addr;
}

static inline void _dma_direct_unmap_page(struct device *dev, dma_addr_t addr,
					  size_t size,
					  enum dma_data_direction dir,
					  unsigned long attrs)
{
	phys_addr_t phys = dma_to_phys(dev, addr);

	if (!(attrs & DMA_ATTR_SKIP_CPU_SYNC))
		_dma_direct_sync_single_for_cpu(dev, addr, size, dir);

	if (unlikely(is_swiotlb_buffer(dev, phys)))
		swiotlb_tbl_unmap_single(dev, phys, size, dir, attrs);
}

static void _dma_direct_unmap_sg(struct device *dev, struct scatterlist *sgl,
				 int nents, enum dma_data_direction dir,
				 unsigned long attrs)
{
	struct scatterlist *sg;
	int i;

	for_each_sg(sgl, sg, nents, i)
		_dma_direct_unmap_page(dev, sg->dma_address, sg_dma_len(sg),
				       dir, attrs);
}

static int _dma_direct_map_sg(struct device *dev, struct scatterlist *sgl,
			      int nents, enum dma_data_direction dir,
			      unsigned long attrs)
{
	int i;
	struct scatterlist *sg;

	for_each_sg(sgl, sg, nents, i) {
		sg->dma_address = _dma_direct_map_page(dev, sg_page(sg),
						       sg->offset, sg->length,
						       dir, attrs);
		if (sg->dma_address == DMA_MAPPING_ERROR)
			goto out_unmap;
		sg_dma_len(sg) = sg->length;
	}

	return nents;

out_unmap:
	_dma_direct_unmap_sg(dev, sgl, i, dir, attrs | DMA_ATTR_SKIP_CPU_SYNC);
	return -EIO;
}

static dma_addr_t dma_mixcoherent_map_page(struct device *dev,
					   struct page *page,
					   unsigned long offset, size_t size,
					   enum dma_data_direction dir,
					   unsigned long attrs)
{
	dma_addr_t addr;

	addr = _dma_direct_map_page(dev, page, offset, size, dir, attrs);

	/* for backward compatibility */
	if (attrs & DMA_ATTR_NON_CONSISTENT) {
		if (!dma_mapping_error(dev, addr) &&
		    !(attrs & DMA_ATTR_SKIP_CPU_SYNC))
			dma_mixcoherent_sync_dma(dev,
						 page_to_phys(page) + offset,
						 size, dir);

		if (dma_get_mask(dev) >= DMA_BIT_MASK(36))
			addr = ioc_addr_to_nioc_addr(addr);
	}

	return addr;
}

static void dma_mixcoherent_unmap_page(struct device *dev, dma_addr_t handle,
				       size_t size, enum dma_data_direction dir,
				       unsigned long attrs)
{
	/* for backward compatibility */
	if (attrs & DMA_ATTR_NON_CONSISTENT) {
		handle = nioc_addr_to_ioc_addr(handle);

		if (!(DMA_ATTR_SKIP_CPU_SYNC & attrs))
			dma_mixcoherent_sync_dma(dev, dma_to_phys(dev, handle),
						 size, dir);
	}
}

static int dma_mixcoherent_map_sg(struct device *dev, struct scatterlist *sg,
				  int nents, enum dma_data_direction dir,
				  unsigned long attrs)
{
	int i;
	struct scatterlist *s;

	nents = _dma_direct_map_sg(dev, sg, nents, dir, attrs);

	/* for backward compatibility */
	if (attrs & DMA_ATTR_NON_CONSISTENT) {
		if (!(DMA_ATTR_SKIP_CPU_SYNC & attrs))
			dma_mixcoherent_sync_sg_for_device(dev, sg, nents, dir);

		if (dma_get_mask(dev) >= DMA_BIT_MASK(36)) {
			for_each_sg(sg, s, nents, i)
				sg_dma_address(s) =
					ioc_addr_to_nioc_addr(sg_dma_address(s));
		}
	}

	return nents;
}

static void dma_mixcoherent_unmap_sg(struct device *dev, struct scatterlist *sg,
				     int nents, enum dma_data_direction dir,
				     unsigned long attrs)
{
	int i;
	struct scatterlist *s;

	/* for backward compatibility */
	if (attrs & DMA_ATTR_NON_CONSISTENT) {
		for_each_sg(sg, s, nents, i)
			sg_dma_address(s) =
				nioc_addr_to_ioc_addr(sg_dma_address(s));

		if (!(DMA_ATTR_SKIP_CPU_SYNC & attrs))
			dma_mixcoherent_sync_sg_for_device(dev, sg, nents, dir);
	}
}

const struct dma_map_ops mixcoherent_dma_ops = {
	.alloc			= dma_mixcoherent_alloc,
	.free			= dma_mixcoherent_free,
	.alloc_pages		= dma_mixcoherent_alloc_pages,
	.free_pages		= dma_mixcoherent_free_pages,
	.map_page		= dma_mixcoherent_map_page,
	.map_sg			= dma_mixcoherent_map_sg,
	.dma_supported		= dma_direct_supported,
	.mmap			= dma_mixcoherent_mmap,
	.unmap_page		= dma_mixcoherent_unmap_page,
	.unmap_sg		= dma_mixcoherent_unmap_sg,
	.sync_single_for_device = dma_mixcoherent_sync_single_for_device,
	.sync_sg_for_device	= dma_mixcoherent_sync_sg_for_device,
	.sync_single_for_cpu	= dma_mixcoherent_sync_single_for_cpu,
	.sync_sg_for_cpu	= dma_mixcoherent_sync_sg_for_cpu,
};
EXPORT_SYMBOL(mixcoherent_dma_ops);
