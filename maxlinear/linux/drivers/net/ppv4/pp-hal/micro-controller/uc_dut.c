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
 * Description: DUT PP micro-controllers Driver
 */
#define pr_fmt(fmt) "[PP_UC_DUT]:%s:%d: " fmt, __func__, __LINE__

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/bitops.h>
#include <linux/pp_api.h>

#include "pp_regs.h"
#include "uc.h"
#include "uc_regs.h"
#include "uc_internal.h"
#include "uc_dut.h"

#define UC_EGR_DUT_ELF       "egress_dut.elf"
#define UC_ING_DUT_ELF       "ingress_dut.elf"

extern s32 uc_load_init(const char *name, struct uc_egr_init_params *egr, 
		struct uc_cpu_params *cpu_params, u8 max_cpu, bool is_egr);
struct uc_egr_init_params g_dut_init;
static bool dut_enable = false;
/**
 * @brief DUT command response max retries 
 */
#define PP_DUT_RSP_MAX_RETRIES (1000000)
#define UC_TYPE_SSRAM(is_egr, offset) is_egr ?	\
		UC_SSRAM(EGRESS, offset) : UC_SSRAM(INGRESS, offset)
#define UC_TYPE_DCCM(is_egr, cpu, offset)  is_egr ?	\
	(UC_DCCM(EGRESS, cpu, offset)) : (UC_DCCM(INGRESS, cpu, offset))

/**
 * @brief send event to uc to activate test 
 * @param idx event id to send
 * @param is_egr : egress (1) or ingress (0)
 */
void uc_issue_soft_event_to_uc(u8 idx, bool is_egr)
{
	u64 addr;

	if (is_egr) 
		addr = PP_UC_EGR_SHACC_HWEVENT_SOFT_EVENT_F_REG_IDX(idx);	
	else
		addr = PP_UC_ING_SHACC_HWEVENT_SOFT_EVENT_F_REG_IDX(idx);
	
	if (idx < ING_SOFT_EVENT_MAX) 
		PP_REG_WR32(addr, true);
	else 
		pr_err("Event index %d not supported\n", idx);
	
}

/**
 * @brief read and check magic number from uc to verify structur alignment
 * @param is_egr : egress (1) or ingress (0)
 */
s32 uc_dut_check_magic_num(bool is_egr)
{
	u64 addr;
	u32 magic;

	addr = UC_TYPE_SSRAM(is_egr, EGR_DUT_MAGIC_NUM_ADDR);

	magic = PP_REG_RD32(addr);

	if (DUT_MAGIC_NUM != magic) {
		pr_err("magic num not aligned 0x%x\n", magic);
		return -1;
	}

	return 0;
}

/**
 * @brief set global ddr buffer address to use by different tests
 * @param buff_addr : buffer address to set
 * @param is_egr : egress (1) or ingress (0)
 */
s32 uc_dut_set_buff_address(u64 buff_addr, bool is_egr)
{
	u64 addr;

	addr = UC_TYPE_SSRAM(is_egr, EGR_DUT_BUFF_ADDR);	

	memcpy_toio((void *)addr, &buff_addr, sizeof(buff_addr));

	return 0;
}

/**
 * @brief check release alignment 
 * @param is_egr : egress cluster (1) or ingress cluster (0)
 */
s32 uc_dut_check_release(bool is_egr)
{
	u32 release; 
	u64 addr;

	addr = UC_TYPE_SSRAM(is_egr, EGR_DUT_RELEASE_ADDR);
	
	release = PP_REG_RD32(addr);
	if (DUT_RELEASE != release) {
		pr_err("DUT release is not alligned Host %d FW %d\n", 
		DUT_RELEASE, release);
		return -1;	
	}

	pr_info("DUT release %d\n", release);
	return 0;
}

/**
 * @brief print tests supported by uc
 * @param is_egr : egress cluster (1) or ingress cluster (0)
 */
s32 uc_dut_get_tests_names(struct seq_file *f, bool is_egr)
{
	u32 i;
	u64 addr;
	u32 names_ptr;
	u32 names_size;
	u32 entries;
	struct dut_test_info *test_info;
	u32 desc_size = 512;
	char *desc_ptr;
	void *alloc_ptr;

	addr = UC_TYPE_SSRAM(is_egr, EGR_DUT_NAME_PTR_ADDR);
	memcpy_fromio((void *)&names_ptr, (void *)addr, sizeof(names_ptr));

	addr = UC_TYPE_SSRAM(is_egr, EGR_DUT_NAME_SIZE_ADDR);
	memcpy_fromio((void *)&names_size, (void *)addr, sizeof(names_size));	
	alloc_ptr = kzalloc(names_size, GFP_KERNEL);
	if (!alloc_ptr) {
		pr_err("failed to allocated memory\n");
		return -ENOMEM;
	}

	test_info = (struct dut_test_info *)alloc_ptr;
	addr = UC_TYPE_SSRAM(is_egr, (names_ptr & 0xFFFF));
	memcpy_fromio((void *)test_info, (void *)addr, names_size);
	entries = names_size/sizeof(*test_info);
	if (entries) {
		desc_ptr = kzalloc(desc_size, GFP_KERNEL);
		if (desc_ptr) {
			seq_puts(f, "| Test-Id |   Test Name  | Test Description  \n");
			seq_puts(f, "=============================================\n");
			for (i = 0 ; i < entries ; i++) {
				addr = UC_TYPE_SSRAM(is_egr, 
						(test_info->name & 0xFFFF));
				memcpy_fromio((void *)desc_ptr, 
					(void *)addr, test_info->name_len);
				seq_printf(f, "| %-7d | %-15.s | ", 
					test_info->test_id , desc_ptr);
				addr = UC_TYPE_SSRAM(is_egr, 
						(test_info->desc & 0xFFFF));	
				memcpy_fromio((void *)desc_ptr,
				(void *)addr, test_info->desc_len);
				seq_printf(f, "%-128.s | \n", desc_ptr);
				test_info++;
			}
			seq_puts(f, "=============================================\n");

			kfree(desc_ptr);
		}
	}

	kfree(alloc_ptr);
	return 0;
}

/**
 * @brief set entry in table as part of runing a list of tests
 * @param entry_index : entry index to write to
 * @param entry : entry data to write to table
 * @param is_egr : egress cluster (1) or ingress cluster (0)
 */
s32 uc_dut_entry_set(u32 entry_index, struct dut_test_table_entry *entry, 
			bool is_egr)
{
	u64 addr;

	if (ptr_is_null(entry))
		return -EINVAL;

	addr = UC_TYPE_SSRAM(is_egr, EGR_DUT_ENTRY_BASE);
	addr = addr + (entry_index * sizeof(struct dut_test_table_entry));
	memcpy_toio((void *)addr, entry, sizeof(*entry));

	return 0;
}

/**
 * @brief get entry from table of entries.
 * @param entry_index : entry index to read from table
 * @param entry : entry ptr.
 * @param is_egr : egress cluster (1) or ingress cluster (0)
 */
s32 uc_dut_entry_get(u32 entry_index, struct dut_test_table_entry *entry, 
			bool is_egr)
{
	u64 addr;

	if (ptr_is_null(entry))
		return -EINVAL;
	if (entry_index >= DUT_MAX_TABLE_ENTRIES) {
		pr_err("unsupported index, max index value %u ",
			DUT_MAX_TABLE_ENTRIES);
		return -EINVAL;
	}

	addr = UC_TYPE_SSRAM(is_egr, EGR_DUT_ENTRY_BASE);
	addr = addr + (entry_index * sizeof(struct dut_test_table_entry));
	memcpy_fromio((void *)entry,
		(void *)addr, sizeof(*entry));
	
	return 0;
}

/**
 * @brief read stats from cpu dccm
 * @param info : return info structure copied from cpu DCCM
 * @param cid : cid in cluster
 * @param is_egr : egress cluster (1) or ingress cluster (0)
 */
s32 uc_dut_stats_read(struct dut_info *info, u32 cid, bool is_egr)
{
	u64 addr;

	if (ptr_is_null(info))
		return -EINVAL;

	addr = UC_TYPE_DCCM(is_egr, cid, EGR_CPU_DUT_STAT_OFF);

	memcpy_fromio((void *)info, (void *)addr, sizeof(*info));
	return 0;
}

/**
 * @brief send command to cluster 
 * @param cmd : command structure contain all the information for the test
 * @param wait_for_done : wait for ack from cpu or return immediately 
 * @param is_egr : egress cluster (1) or ingress cluster (0)
 */
s32 uc_dut_cmd_send(struct dut_cmd *cmd, bool wait_for_done, bool is_egr)
{
	u64 rspAddr;
	u64 msgAddr;
	u8 event;
	u8 i;

	if (ptr_is_null(cmd))
		return -EINVAL;

	rspAddr = UC_TYPE_SSRAM(is_egr, EGR_DUT_MSG_RSP);
	msgAddr = UC_TYPE_SSRAM(is_egr, EGR_DUT_MSG_BASE);
	event = EG_SOFT_EVENT_DUT_0;

	if (unlikely(pp_reg_poll_x(rspAddr, 0x1,
				   true, PP_DUT_RSP_MAX_RETRIES))) {
		pr_err("Failed polling on DUT response 0x%llx\n", rspAddr);
		return -EINVAL;
	}

	memcpy_toio((void *)msgAddr, cmd, sizeof(*cmd));

	for (i = 0 ; i < UC_CPUS_MAX ; i++) {
		if (cmd->cpu_mask & (0x1 << i))
			uc_issue_soft_event_to_uc(event + i, is_egr);
	}

	if (wait_for_done) {
		if (unlikely(pp_reg_poll_x(rspAddr, 0x1,
				   true, PP_DUT_RSP_MAX_RETRIES))) {
			pr_err("Failed polling on DUT response\n");
			return -EINVAL;
		}
	}

	return 0;
}

/**
 * @brief return if dut was enabled
 */
bool uc_dut_enable(void)
{
	return dut_enable;
}

/**
 * @brief save egress init information
 */
s32 uc_dut_save_egr_init(struct uc_egr_init_params *egr)
{
	memcpy(&g_dut_init, egr, sizeof(*egr));
	return 0;
}

/**
 * @brief load DUT image to cluster and init it.
 * @param is_egr : egress cluster (1) or ingress cluster (0)
 */
s32 uc_dut_load_image(u32 *cpu_profiles, bool is_egr)
{
	s32 ret = 0;
	u32 cid;
	char *file;

	if (is_egr)
		file = UC_EGR_DUT_ELF;
	else
		file = UC_ING_DUT_ELF;

	for (cid = 0; cid < g_dut_init.cpus.max_cpu; cid++) 
		g_dut_init.cpus.cpu_prof[cid] = cpu_profiles[cid];
	
	/* reset BOOT GPREG */
	uc_ccu_gpreg_set(is_egr, 0, 0);
	/* enable dut init sequence identify it is eg/ing */
	uc_ccu_gpreg_set(is_egr, 1, is_egr);

	for (cid = 0; cid < g_dut_init.cpus.max_cpu; cid++)
		uc_ccu_enable_set(is_egr, cid, false);

	ret = uc_load_init(file, &g_dut_init, 
				&g_dut_init.cpus,
				g_dut_init.cpus.max_cpu, 
				is_egr);
	if (unlikely(ret)) {
		pr_err("failed to load DUT file\n");
		return ret;
	}

	if (is_egr)
		dut_enable = true;
	
	if (!uc_dut_check_magic_num(is_egr)) 
		pr_err("!!!!!!!!!! %s TEST FW LOADDED !!!!!!!!!!!!\n", file);
	else
		pr_err("Loaded unaligned DUT FW\n");	
	return 0;
}
