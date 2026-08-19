/*
 * Copyright (C) 2020-2025 MaxLinear, Inc.
 * Copyright (C) 2019-2020 Intel Corporation
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
 * Description: PP Egress micro-controllers mailbox
 */

#define pr_fmt(fmt) "[PP_UC_MBOX]:%s:%d: " fmt, __func__, __LINE__

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
#include "uc_dut.h"
#include "uc_internal.h"
#include "infra.h"

/**
 * @brief the size of the command buffer inside the uc job
 */
#define UC_MBOX_CMD_BUFF_SZ  (1024)

/**
 * @brief the command buffer pointer from the uc point of view
 */
#define UC_MBOX_CMD_BUFF_ADR_UC(q_off, in_q_off) (0xF0000000 + q_off + in_q_off)

/**
 * @brief the command buffer pointer from the host point of view
 */
#define UC_MBOX_CMD_BUFF_ADR_HOST(q_off, in_q_off) \
	(UC_DCCM(EGRESS, 0, in_q_off) + q_off)

/**
 * @brief the mailbox command job index mask
 */
#define UC_MBOX_CMD_IDX_MSK  0x7

/**
 * @brief If job < 4, cmd will be copied to offset 1k in the queue.
 *        Else, cmd will be copied to the start of the queue.
 */
#define UC_MBOX_JOB_IDX_2_Q_OFFSET(job)  (job < 4 ? 1024 : 0)

/**
 * @brief uc internal memory queue offset
 */
#define UC_MBOX_QUEUE_OFF    0x800

/**
 * @brief uc internal memory cpu offset
 */
#define UC_MBOX_CPU_OFF      0x40000

/**
 * @brief mailbox command job index (0..7)
 */
static u8 cmd_idx = 0;

/**
 * @brief mailbox queue offset
 */
static u32 cmd_q_off;

/**
 * @brief locks database and HW accesses
 */
static spinlock_t mbox_lock;

const char *cmd_type_str[UC_CMD_CNT + 1] = {
	[UC_CMD_SYNC] = "Sync",
	[UC_CMD_INIT] = "Init",
	[UC_CMD_VERSION] = "Version",
	[UC_CMD_LOGGER_RESET] = "Logger_Reset",
	[UC_CMD_LOGGER_LEVEL_SET] = "Logger_Level_Set",
	[UC_CMD_MCAST_PID] = "Mcast_Pid",
	[UC_CMD_MCAST_QUEUE] = "Mcast_Queue",
	[UC_CMD_MCAST_DST] = "Mcast_Dst",
	[UC_CMD_MCAST_SESS_MIRRORING] = "Mcast_Sess_Mirroring",
	[UC_CMD_IPSEC_INFO] = "Ipsec_Info",
	[UC_CMD_REASSEMBLY_INFO] = "Reassembly_Info",
	[UC_CMD_REASSEMBLY_TIMEOUT_THR] = "Reassembly_Timeout_Thr",
	[UC_CMD_AUX_REG_RD] = "Aux_Reg_Rd",
	[UC_CMD_AUX_REG_WR] = "Aux_Reg_Wr",
	[UC_CMD_LRO_CONF_SET] = "Lro_Conf_Set",
	[UC_CMD_LRO_CONF_GET] = "Lro_Conf_Get",
	[UC_CMD_TDOX_CREATE] = "Tdox_Create",
	[UC_CMD_TDOX_REMOVE] = "Tdox_Remove",
	[UC_CMD_TDOX_STATS] = "Tdox_Stats",
	[UC_CMD_CHK_MBX] = "Chk_Mbx",
	[UC_CMD_CLS_MBX] = "Cls_Mbx",
	[UC_CMD_TDOX_CONFIG_SET] = "Tdox_Config_Set",
	[UC_CMD_TDOX_CONFIG_GET] = "Tdox_Config_Get",
	[UC_CMD_LLD_CTX_CFG_SET] = "LLD_Context_Config",
	[UC_CMD_AQM_CTX_CFG_SET] = "AQM_Context_Config",
	[UC_CMD_LLD_ALLOWED_AQ_SET] = "LLD_Allowed_AQ_Set",
	[UC_CMD_AQM_LLD_HIST_GET] = "AQM_LLD_Hist_Get",
	[UC_CMD_CNT] = "Invalid"
};

/**
 * @brief Acquire mbox lock
 */
static inline void __mbox_lock(void)
{
	pr_debug("caller %ps\n", __builtin_return_address(0));

	/* we are not allowed to run in irq context */
	WARN(in_irq(), "uc mbox shouldn't be used from irq context\n");
	spin_lock_bh(&mbox_lock);
}

/**
 * @brief Release mbox lock
 */
static inline void __mbox_unlock(void)
{
	pr_debug("caller %ps\n", __builtin_return_address(0));

	spin_unlock_bh(&mbox_lock);
}

static bool __cmd_is_done(void)
{
	u64 reg;
	char *err[MBOX_CMD_CNT] = {
		"Success",
		"Busy",
		"Unsupported Command",
		"Invalid Param",
		"Invalid Length",
	};

	reg = UC_B_CCU_GPREG_REG(EGRESS, CMD_DONE_REG_IDX);

	/* wait for command done signal */
	if (pp_reg_poll(reg, U8_MAX, MBOX_CMD_SUCCESS)) {
		pr_err("UC command failed: %s (%u)\n", err[PP_REG_RD32(reg)],
		       PP_REG_RD32(reg));
		return false;
	}

	return true;
}

static void __cmd_q_off_set(void)
{
	u32 val;
	u8  qid, cid;

	val = PP_REG_RD32(UC_B_CCU_GPREG_REG(EGRESS, CMD_DONE_REG_IDX));

	qid = (val >> 8)  & U8_MAX;
	cid = (val >> 16) & U8_MAX;

	cmd_q_off = qid * UC_MBOX_QUEUE_OFF + cid * UC_MBOX_CPU_OFF;
	pr_debug("cmd qid=%hhu cid=%hhu q_off=%u\n", qid, cid, cmd_q_off);
}

static s32 __cmd_send(u32 type, u32 param, u32 addr, u16 len)
{
	u32 flags = BIT(WR_FLAG_IS_DATA);

	/* write the massage format to the HW interface */
	PP_REG_WR32(UC_B_CCU_GPREG_REG(EGRESS, CMD_DONE_REG_IDX),
			MBOX_CMD_BUSY);
	PP_REG_WR32(UC_EGR_B_MBOX_PORT_WR_DATA0(UC_MBOX_PORT),   type);
	PP_REG_WR32(UC_EGR_B_MBOX_PORT_WR_DATA1(UC_MBOX_PORT),   param);
	PP_REG_WR32(UC_EGR_B_MBOX_PORT_WR_ADDRESS(UC_MBOX_PORT), addr);
	/* trigger the command */
	PP_REG_WR32(UC_EGR_B_MBOX_PORT_WR_LENGTH_FLAGS(UC_MBOX_PORT),
			(flags << 16) | len);

	/* verify command is done */
	if (unlikely(!__cmd_is_done()))
		return -EPERM;

	if (type == UC_CMD_SYNC)
		__cmd_q_off_set();

	return 0;
}

static bool __cmd_is_valid(enum uc_mbox_cmd_type type)
{
	if (type > UC_CMD_LAST || type < UC_CMD_FIRST) {
		pr_err("invalid command type %u\n", (u32)type);
		return false;
	}

	return true;
}

static bool __cmd_is_buff_len_valid(u16 len)
{
	if (len > UC_MBOX_CMD_BUFF_SZ) {
		pr_err("buffer to write too big %hu, max is %u\n", len,
		       (u32)UC_MBOX_CMD_BUFF_SZ);
		return false;
	}

	return true;
}

static void uc_egr_check_for_exception(void)
{
	uc_check_for_exception();
}

static const char *cmd_type_to_str(enum uc_mbox_cmd_type t)
{
	if (!__cmd_is_valid(t))
		return cmd_type_str[UC_CMD_CNT];

	return cmd_type_str[t];
}

s32 uc_egr_mbox_cmd_send(enum uc_mbox_cmd_type type, u32 param,
			 const void *in_buf, u16 in_len,
			 void *out_buf, u16 out_len)
{
	s32 ret;
	void *cmd_buff;
	u16 in_q_off;
	u32 uc_addr;

	/* in case we work in dut mode dont send the mbox msg */
	if (uc_dut_enable())
		return 0;

	pr_debug("uc mbox cmd %s(%u), param %u, in_buf %ps, in_len %u, out_buf %ps, out_len %u\n",
		 cmd_type_to_str(type), type, param, in_buf, in_len, out_buf,
		 out_len);

	if (!uc_is_cluster_valid(UC_IS_EGR))
		return -EPERM;

	if (!__cmd_is_valid(type))
		return -EINVAL;

	if (!__cmd_is_buff_len_valid(in_len) ||
	    !__cmd_is_buff_len_valid(out_len))
		return -EINVAL;

	__mbox_lock();

	/* Calc the offset inside the queue */
	in_q_off = UC_MBOX_JOB_IDX_2_Q_OFFSET((cmd_idx & UC_MBOX_CMD_IDX_MSK));
	uc_addr = UC_MBOX_CMD_BUFF_ADR_UC(cmd_q_off, in_q_off);
	cmd_buff = (void *)UC_MBOX_CMD_BUFF_ADR_HOST(cmd_q_off, in_q_off);

	/* copy data to FW buffer */
	if (in_buf && in_len)
		memcpy(cmd_buff, in_buf, in_len);

	/* send the command */
	ret = __cmd_send(type, param, uc_addr, in_len);
	if (ret) {
		uc_egr_check_for_exception();
		goto unlock;
	}

	/* Increase the cmd idx only if no error to keep the host-fw sync */
	cmd_idx++;

	/* copy data to caller buffer */
	if (out_buf && out_len)
		memcpy(out_buf, cmd_buff, out_len);

unlock:
	__mbox_unlock();
	return ret;
}

s32 uc_egr_mbox_init(void)
{
	/* init lock */
	spin_lock_init(&mbox_lock);

	PP_REG_WR32(UC_EGR_B_WRITER_PORT_ENABLE(UC_MBOX_PORT), 1);

	return uc_egr_mbox_cmd_send(UC_CMD_SYNC, 0, NULL, 0, NULL, 0);
}

void uc_egr_mbox_exit(void)
{
}
