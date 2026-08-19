/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_DMA_MAPPING_H
#define _ASM_X86_DMA_MAPPING_H

/*
 * IOMMU interface. See Documentation/core-api/dma-api-howto.rst and
 * Documentation/core-api/dma-api.rst for documentation.
 */

#include <linux/scatterlist.h>
#include <asm/io.h>
#include <asm/swiotlb.h>

extern int iommu_merge;
extern int panic_on_overflow;

extern const struct dma_map_ops *dma_ops;

static inline const struct dma_map_ops *get_arch_dma_ops(struct bus_type *bus)
{
	return dma_ops;
}

#ifdef CONFIG_DMA_MIXCOHERENT

/* This is only added for backward compatibility with 4.19 dma mixcoherent spec.
 * It needs to be removed once spec is updated for 5.15.
 */
#define DMA_ATTR_NON_CONSISTENT		(1UL << 3)

extern const struct dma_map_ops mixcoherent_dma_ops;

static inline dma_addr_t ioc_addr_to_nioc_addr(dma_addr_t phy_addr)
{
	dma_addr_t addr = phy_addr;

	if (phy_addr & BIT_ULL(34)) {
		addr &= ~BIT_ULL(34);
		addr |= BIT_ULL(32) | BIT_ULL(33) | BIT_ULL(35);
	} else {
		addr |= BIT_ULL(34) | BIT_ULL(35);
	}

	return addr;
}

static inline dma_addr_t nioc_addr_to_ioc_addr(dma_addr_t ioc_addr)
{
	dma_addr_t addr = ioc_addr;

	if (ioc_addr & BIT_ULL(35)) {
		if (ioc_addr & BIT_ULL(34)) {
			addr &= ~(BIT_ULL(34) | BIT_ULL(35));
		} else {
			addr &= ~(BIT_ULL(32) | BIT_ULL(33) | BIT_ULL(35));
			addr |= BIT_ULL(34);
		}
	}

	return addr;
}
#endif

#endif
