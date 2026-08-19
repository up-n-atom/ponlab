/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2020-2024 MaxLinear, Inc.
 * Copyright (C) 2016-2020 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2, as published
 * by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/bitmap.h>
#include <linux/skbuff.h>

#ifndef _INTEL_MCPY_H
#define _INTEL_MCPY_H

enum {
	MCPY_CHUNK_512 = 0,
	MCPY_CHUNK_1K,
	MCPY_CHUNK_2K,
	MCPY_CHUNK_4K,
};

#define MCPY_PORT_CNT		8

#define MCPY_DST_FLG_OFF	3
#define MCPY_SRC_FLG_OFF	4
#define MCPY_SZ_FLG_OFF		5
#define MCPY_INT_FLG_OFF	6
#define MCPY_ALGN_FLG_OFF	7
#define MCPY_WAIT_FLG_OFF	8

#define MCPY_CHUNK_SZ_MASK      7
#define MCPY_DST_PADDR		BIT(MCPY_DST_FLG_OFF)
#define MCPY_SRC_PADDR		BIT(MCPY_SRC_FLG_OFF)
#define MCPY_CHUNK_SIZE		BIT(MCPY_SZ_FLG_OFF)
#define MCPY_INT_ENABLE		BIT(MCPY_INT_FLG_OFF)
#define MCPY_ADDR_ALGN		BIT(MCPY_ALGN_FLG_OFF)
#define MCPY_WAIT_BEF_NEXT_COPY	BIT(MCPY_WAIT_FLG_OFF)

/*!
 *@brief  The API is for HW memcpy
 *@param[in] dst - Pointer to destination buffer
 *@param[in] src - Pointer to source buffer
 *@param[in] pid - HW memcpy port id
 *@param[in] len - Number of bytes to copy
 *@param[in] flag - Bitmap flag to indicate memcpy options
 *@return 0 if OK / -EINVAL if error
 */
int intel_hw_mcpy(void *dst, void *src, unsigned int pid,
		  unsigned int len, unsigned long flag);

/*!
 *@brief  The API is for reserving HW memcpy port
 *@return HW memcpy port if OK / -1 if error
 */
int mcpy_get_pid(void);

/*!
 *@brief  The API is for releasing HW memcpy port
 *@param[in] pid - HW memcpy port id
 */
void mcpy_free_pid(u32 pid);

#endif /* _INTEL_MCPY_H */
