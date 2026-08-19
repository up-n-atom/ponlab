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
 * Description: PP Driver DMA API definitions
 */

#ifndef __PP_DMA_H__
#define __PP_DMA_H__

#include <linux/kernel.h>
#include <linux/types.h>
#include "pp_dev.h"

/* DMA none coherent flag */
#define PP_DMA_NONECOHERENT BIT(0)

/**
 * @brief Allocate memory by DMA for pp drivers
 * @param sz memory size in bytes
 * @param paddr physical address
 * @param attr optional flags as PP_DMA_NONECOHERENT
 * @return void* virtual address of allocated memory, NULL in failure
 */
void *pp_dma_alloc(ssize_t sz, dma_addr_t *paddr, unsigned long attr);

/**
 * @brief Free memory by DMA for pp drivers
 * @param vaddr virtual address
 * @param sz memory size in bytes
 */
void pp_dma_free(void *vaddr, ssize_t sz);

/**
 * @brief Cache writeback (for nonecoherent address)
 * @param vaddr virtual address
 * @param sz memory size in bytes
 */
void pp_cache_writeback(void *vaddr, size_t size);

/**
 * @brief Cache invalidate (for nonecoherent address)
 * @param vaddr virtual address
 * @param sz memory size in bytes
 */
void pp_cache_invalidate(void *vaddr, size_t size);

/**
 * @brief Init pp dma
 * @param pdev platform device
 * @return s32 0 on success, error code otherwise
 */
s32 pp_dma_init(struct platform_device *pdev);

/**
 * @brief Exit pp dma
 * @param pdev platform device
 */
void pp_dma_exit(struct platform_device *pdev);

#if IS_ENABLED(CONFIG_SOC_LGM)
/**
 * @brief Get the pool size
 * @param is_ioc true for the ioc pool size false for the nioc pool size
 * @return ssize_t pool size in bytes
 */
ssize_t pp_dma_get_pool_size(bool is_ioc);

/**
 * @brief Get the pool available bytes
 * @param is_ioc true for the ioc pool size false for the nioc pool size
 * @return ssize_t pool available bytes
 */
ssize_t pp_dma_get_pool_avail(bool is_ioc);
#endif /* IS_ENABLED(CONFIG_SOC_LGM) */
#endif /* __PP_DMA_H__ */
