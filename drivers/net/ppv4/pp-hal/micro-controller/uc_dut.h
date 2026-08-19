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
 * Description: DUT PPV4 local definitions
 */

#ifndef __PP_UC_DUT_H__
#define __PP_UC_DUT_H__

#include "uc_dut_shared.h"

/**
 *  enable DUT 
 */
#define  UC_DUT_ENABLE

#define DCCM_EGRESS_ADDR(cpu, offset)	 (UC_DCCM(EGRESS, cpu, offset))

#define EGR_DUT_SSRAM_OFF (0x7800)
#define EGR_DUT_MSG_BASE                                                  \
	(EGR_DUT_SSRAM_OFF + offsetof(struct dut_sram, dut_cmd))
#define EGR_DUT_MSG_RSP                                                  \
	(EGR_DUT_SSRAM_OFF + offsetof(struct dut_cmd, rsp_valid))
#define EGR_DUT_ENTRY_BASE                                                  \
	(EGR_DUT_SSRAM_OFF + offsetof(struct dut_sram, table_entry_db))
#define EGR_DUT_BUFF_ADDR                                                  \
	(EGR_DUT_SSRAM_OFF + offsetof(struct dut_sram, local_db.buff))
#define EGR_DUT_MAGIC_NUM_ADDR                                                  \
	(EGR_DUT_SSRAM_OFF + offsetof(struct dut_sram, sram_magic_num))
#define EGR_DUT_RELEASE_ADDR                                                  \
	(EGR_DUT_SSRAM_OFF + offsetof(struct dut_sram, local_db.release))
#define EGR_DUT_NAME_PTR_ADDR                                                  \
	(EGR_DUT_SSRAM_OFF + offsetof(struct dut_sram, local_db.test_names_ptr))
#define EGR_DUT_NAME_SIZE_ADDR                                                  \
	(EGR_DUT_SSRAM_OFF + offsetof(struct dut_sram, local_db.test_names_size))
#define EGR_DUT_DCCM_OFF (0x3F00)	
#define EGR_CPU_DUT_STAT_OFF                                                   \
	(EGR_DUT_DCCM_OFF + offsetof(struct dut_info, dut_params))	

s32 uc_dut_entry_get(u32 entry_index, struct dut_test_table_entry *entry, 
			bool is_egr);
s32 uc_dut_entry_set(u32 entry_index, struct dut_test_table_entry *entry, 
			bool is_egr);
s32 uc_dut_stats_read(struct dut_info *info, u32 cid, bool is_egr);
s32 uc_dut_cmd_send(struct dut_cmd *cmd, bool wait_for_done, bool is_egr);
s32 uc_dut_set_buff_address(u64 buff_addr, bool is_egr);
s32 uc_dut_check_magic_num(bool is_egr);
s32 uc_dut_check_release(bool is_egr);
s32 uc_dut_get_tests_names(struct seq_file *f, bool is_egr);
s32 uc_dut_save_egr_init(struct uc_egr_init_params *egr);
s32 uc_dut_load_image(u32 *cpu, bool is_egr);
bool uc_dut_enable(void);
#endif /* __PP_UC_DUT_H__ */

