/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
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

#ifndef __CQM_DEBUGFS_H_
#define __CQM_DEBUGFS_H_
#include <asm/div64.h>
#include <linux/ctype.h>
#include <linux/time.h>
#include <linux/jiffies.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/sort.h>
#include <linux/skbuff.h>
#include <linux/version.h>

#define get_val(val, mask, offset) (((val) & (mask)) >> (offset))
#define CONFIG_MXL_CQM_DBG

enum CQM_DBG_FLAG {
	CQM_DBG_FLAG_DBG = BIT(0),
	CQM_DBG_FLAG_TX_DESC = BIT(1),
	CQM_DBG_FLAG_TX_PATH = BIT(2),
	CQM_DBG_FLAG_QID_MAP = BIT(3),
	CQM_DBG_FLAG_BUFF_ALLOC = BIT(4),
	CQM_DBG_FLAG_CPU_BUFF = BIT(5),
	CQM_DBG_FLAG_BUFF_RTN = BIT(6),
	CQM_DBG_FLAG_DMA_CTRL = BIT(7),
	CQM_DBG_FLAG_DMA_PORT = BIT(8),
	CQM_DBG_FLAG_PON_PORT = BIT(9),
	CQM_DBG_FLAG_DC_PORT = BIT(10),
	CQM_DBG_FLAG_CPU_PORT = BIT(11),
	CQM_DBG_FLAG_MTU = BIT(12),
	CQM_DBG_FLAG_FE_POLICY = BIT(13),
	CQM_DBG_FLAG_Q_FLUSH = BIT(14),
	CQM_DBG_FLAG_PORT_FLUSH = BIT(15),
	CQM_DBG_FLAG_RX = BIT(16),
	CQM_DBG_FLAG_DP_INTF = BIT(17),
	CQM_DBG_FLAG_MARKING = BIT(18),
	CQM_DBG_FLAG_MARKING_BUFF = BIT(19),
	CQM_DBG_FLAG_API = BIT(20),
	CQM_DBG_FLAG_RX_DP = BIT(21),
	CQM_DBG_FLAG_ENABLE_ALL = 0xFFFFFFFF,

	/*Note, once add a new entry here int the enum,
	 *need to add new item in below macro CQM_F_FLAG_LIST
	 */
};

#if defined(CONFIG_MXL_CQM_DBG)
extern u32 cqm_dbg_flag;
extern u32 cqm_print_num_en;
extern u32 cqm_max_print_num;
#define CQM_DEBUG(flags, fmt, arg...)  do { \
if (unlikely((cqm_dbg_flag & (flags)) && (((cqm_print_num_en) && \
	(cqm_max_print_num)) || !cqm_print_num_en))) {\
	pr_info(fmt, ##arg); \
	if ((cqm_print_num_en) && \
	    (cqm_max_print_num)) \
		cqm_max_print_num--; \
	} \
} while (0)
#define CQM_DEBUG_RETURN_ON_UNSET(flags) do { \
	if (!(cqm_dbg_flag & (flags))) { \
		return; \
	} \
} while (0)
#else
#define CQM_DEBUG(flags, fmt, arg...)
#define CQM_DEBUG_RETURN_ON_UNSET(flags)
#endif
u32 epon_mode_reg_get(void);
void cqm_desc_get(struct cbm_desc *desc_p, u32 pid, u32 enq_flag);
#endif

