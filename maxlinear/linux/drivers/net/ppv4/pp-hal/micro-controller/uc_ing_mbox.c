/*
 * Copyright (C) 2021 MaxLinear, Inc.
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
 * Description: PP micro-controllers Debug FS Interface
 */

#define pr_fmt(fmt) "[PP_UC_DBG]:%s:%d: " fmt, __func__, __LINE__

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/dma-mapping.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/bitops.h>

#include "pp_regs.h"
#include "pp_common.h"
#include "uc_regs.h"
#include "uc.h"
#include "uc_internal.h"

/**
 * @define Max MBOX retries till response is ready
 */
#define PP_ING_MBOX_MAX_RETRIES (1000)

void uc_ing_issue_soft_event_to_uc(u8 idx)
{
	u64 addr;

	if (idx < ING_SOFT_EVENT_MAX) {
		addr = PP_UC_ING_SHACC_HWEVENT_SOFT_EVENT_F_REG_IDX(idx);
		PP_REG_WR32(addr, true);
	} else {
		pr_err("Invalid index %d", idx);
	}
}

bool uc_ing_is_soft_event_handled_by_uc(u8 idx)
{
	u64 addr;

	if (idx < ING_SOFT_EVENT_MAX) {
		addr = PP_UC_ING_SHACC_HWEVENT_SOFT_EVENT_F_REG_IDX(idx);
		if (PP_REG_RD32(addr))
			return false;
		else
			return true;
	} else {
		pr_err("Invalid index %d", idx);
		return false;
	}
}

s32 uc_ing_host_mbox_cmd_send(struct uc_ing_cmd *msg, bool wait_for_done)
{
	u64 rspAddr = UC_SSRAM(INGRESS, ING_MBOX_MSG_RSP);
	u64 msgAddr = UC_SSRAM(INGRESS, ING_MBOX_MSG_BASE);

	if (ptr_is_null(msg))
		return -EINVAL;

	if (unlikely(pp_reg_poll_x(rspAddr, 0x1,
				   true, PP_ING_MBOX_MAX_RETRIES))) {
		pr_err("Failed polling on ingress mbox\n");
		return -EINVAL;
	}

	memcpy_toio((void *)msgAddr, msg, sizeof(*msg));

	uc_ing_issue_soft_event_to_uc(ING_SOFT_EVENT_MBOX);

	if (wait_for_done) {
		if (unlikely(pp_reg_poll_x(rspAddr, 0x1,
				   true, PP_ING_MBOX_MAX_RETRIES))) {
			pr_err("Failed polling on ingress mbox response\n");
			return -EINVAL;
		}
	}

	return 0;
}

s32 uc_ing_host_mbox_cmd_stats(struct ing_host_mbox_stat *stat)
{
	u64 addr = UC_SSRAM(INGRESS, ING_MBOX_STAT_BASE);

	if (ptr_is_null(stat))
		return -EINVAL;

	memcpy_fromio((void *)stat, (void *)addr, sizeof(*stat));

	return 0;
}
