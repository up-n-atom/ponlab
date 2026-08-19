/*
 * Copyright (C) 2020-2025 MaxLinear, Inc.
 * Copyright (C) 2017-2020 Intel Corporation
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
 * Description: Packet Processor QoS Driver
 */

#define pr_fmt(fmt) "[PP_QOS_FW]:%s:%d: " fmt, __func__, __LINE__

#include <linux/time.h>
#include <linux/bitops.h>
#include <linux/init.h>
#include "qos_uc_misc_regs.h"
#include "infra.h"
#include "pp_common.h"
#include "pp_dma.h"
#include "pp_qos_common.h"
#include "pp_qos_utils.h"
#include "pp_qos_fw.h"
#include "pp_regs.h"
#include "checker.h"
#include "elf_parser.h"
#include <soc/mxl/mxl_sec.h>

#define FW_DCCM_START                   (0xF0000000)
#define MAX_FW_CMD_SIZE                 (sizeof(union uc_qos_cmd_s))
#define ELF_MAX_SECTIONS                (64)
#ifndef CONFIG_SOC_LGM
#define QOS_EWSP_DEFAULT_BYTE_THRESHOLD_PRX (8000)
#define QOS_EWSP_DEFAULT_TIMEOUT_US_PRX     (128)
#else
#define QOS_EWSP_DEFAULT_INTERVAL_LGM       (50000)
#define CODEL_DFLT_TARGET_DELAY_MSEC        (5)
#define CODEL_DFLT_INTERVAL_TIME_MSEC       (100)
#endif
#define TSCD_KBPS_TO_CREDIT_MULTIPLIER (500ULL)
#define TSCD_KBPS_TO_CREDIT(kbps, ups)                                         \
	(DIV_ROUND_DOWN_ULL(                                                   \
		((unsigned long long)(kbps) * TSCD_KBPS_TO_CREDIT_MULTIPLIER), \
		(ups)))
#define TSCD_CREDIT_TO_KBPS(credits, ups)                                      \
	(DIV_ROUND_DOWN_ULL(                                                   \
		((unsigned long long)(credits) * (unsigned long long)(ups)),   \
		TSCD_KBPS_TO_CREDIT_MULTIPLIER))
u64 qos_misc_base_addr;

static bool qos_fw_log;

struct ppv4_qos_fw_hdr {
	u32 major;
	u32 minor;
	u32 build;
};

struct ppv4_qos_fw_sec {
	u32 dst;
	u32 size;
};

/* TBD: remove this enum after
* cleaning the FW A-step code
*/
enum fw_soc_rev {
	FW_SOC_REV_A = 0,
	FW_SOC_REV_B,
	FW_SOC_REV_UNKNOWN
};

static void copy_section_ddr(unsigned long dst, const void *_src, u32 size)
{
	u32 i;
	const u32 *src;

	src = (u32 *)_src;

	for (i = size; i > 3; i -= 4) {
		*(u32 *)dst = cpu_to_le32(*src++);
		dst += 4;
	}

	/* Section size must be aligned to 2 */
	if (i == 1 || i == 3)
		QOS_LOG_ERR("Size %u not aligned to 2 (i=%u)\n", size, i);

	/* Copy last u16 if exists */
	if (i == 2)
		*(u32 *)dst = cpu_to_le16(*(u16 *)src);
}

static void copy_section_qos(unsigned long dst, const void *_src, u32 size)
{
	u32 i;
	const u32 *src;

	src = (u32 *)_src;

	for (i = size; i > 3; i -= 4) {
		PP_REG_WR32(dst, cpu_to_le32(*src++));
		dst += 4;
	}

	/* Section size must be aligned to 2 */
	if (i == 1 || i == 3)
		QOS_LOG_ERR("Size %u not aligned to 2 (i=%u)\n", size, i);

	/* Copy last u16 if exists */
	if (i == 2)
		PP_REG_WR32(dst, cpu_to_le16(*(u16 *)src));
}

static void calc_elf_sections_sizes(struct elf_sec secs[], u16 num_secs,
				    u32 *total_sz,
				    u32 *data_sz, u32 *data_addr,
				    u32 *stack_sz, u32 *stack_addr,
				    u32* base_addr)
{
	u16 ind;

	for (ind = 0; ind < num_secs; ind++) {
		/* the addr field isn't in sequence after the previous one,
		 * and we must keep all sections in same address(offset)
		 * as defined by the elf file, so the total size is where
		 * the last section that should be copied ends.
		 */
		if (secs[ind].need_copy)
			*total_sz = secs[ind].addr + secs[ind].size;

		if (!strncmp(secs[ind].name, ".data", 5)) {
			*data_sz = secs[ind].size;
			*data_addr = secs[ind].addr;
		}
		else if (!strncmp(secs[ind].name, ".stack", 6)) {
			*stack_sz = secs[ind].size;
			*stack_addr = secs[ind].addr;
		}
		else if (!strncmp(secs[ind].name, ".base", 5))
			*base_addr = secs[ind].addr;
	}
}

static void copy_sections(struct elf_sec secs[],
			  u16 num_secs,
			  void *virt_txt,
			  void *virt_data,
			  void *virt_stack,
			  dma_addr_t phys_txt,
			  dma_addr_t phys_data,
			  dma_addr_t phys_stack,
			  unsigned long ivt,
			  u32 stack_sz,
			  u32 is_in_dccm)
{
	u16 ind;
	void *src;
	unsigned long dst;
	bool copy_to_dccm = false;
	u32 ivt_tbl[3] = {0};

	for (ind = 0; ind < num_secs; ind++) {
		if (!secs[ind].need_copy)
			continue;

		src = secs[ind].data;

		if (!strncmp(secs[ind].name, ".data", 5)) {
			dst = (unsigned long)virt_data;
			copy_to_dccm = !!is_in_dccm;
		} else {
			dst = (unsigned long)(virt_txt) + secs[ind].addr;
			copy_to_dccm = false;
		}

		if (!strncmp(secs[ind].name, ".vectors", 8)) {
			ivt_tbl[0] = le32_to_cpu(*(u32 *)src) + phys_txt;
			ivt_tbl[1] = phys_stack;
			ivt_tbl[2] = phys_data;

			/* Copy to QoS */
			PP_REG_WR32_INC(ivt, ivt_tbl[0]);
			PP_REG_WR32_INC(ivt, ivt_tbl[1]);
			PP_REG_WR32_INC(ivt, ivt_tbl[2]);
		}

		pr_debug("Section %s: COPY %llu bytes from %#lx[%#x %#x..] to %#lx DCCM %d\n",
			 secs[ind].name, secs[ind].size,
			 (unsigned long)src, *(u32 *)src,
			 *(u32 *)((unsigned long)src + 4), dst, copy_to_dccm);

		if (copy_to_dccm)
			copy_section_qos(dst, src, secs[ind].size);
		else
			copy_section_ddr(dst, src, secs[ind].size);
	}

	/* memset stack area */
	memset_io(virt_stack, 0, stack_sz);
}

/**
 * @brief Adds a new entry to a given fat table
 * The qos fat table is capable of translating 32 bits addresses to 32-36
 * bits addresses.
 * e.g. DDR address 0xC5000_0000 can be accessed by the FW by setting
 * fat entry 0x5 to 0xC5, other entries can be used as well.
 * So when FW access 0x5000_0000, the FAT will translate it to 0xC5000_0000
 * e.g. PP address is 0xF000_0000, this address is reserved in the QoS FW, to
 * workaround it, we can use a fat entry to map the PP to a different region
 * when accessed from the QoS FW.
 * We set entry 0x2 to 0x0F, The FW will access the PP addresses via 0x2000_0000
 * and the FAT will translate it to 0xF000_0000
 *
 * @param qdev the qos device
 * @param addr the address to add to the fat
 * @return s32 0 on succuss, error code otherwise
 */
static s32 fat_entry_add(struct pp_qos_dev *qdev, u64 addr)
{
	/* the MSB byte we want to translate to */
	u8 to = FIELD_GET(GENMASK_ULL(35, 28), addr);
	/* the MSB nibble we want to translate from (the address the fw uses)
	 * this will always be 0x0X, X is for 0-F
	 */
	u8 from;

	/* find an empty entry to use, first we try the lsb nibble entry */
	from = to & 0xF; /* extract the lsb nibble */
	if (!test_bit(from, qdev->fw_fat_bmap)) {
		/* entry is not used, use it */
		goto set_entry;
	}
	pr_debug("addr %#llx, entry %u is used\n", addr, from);

	/* lsb nibble entry is used, find a free one */
	from = find_first_zero_bit(qdev->fw_fat_bmap, FW_FAT_TBL_SIZE);
	if (from >= FW_FAT_TBL_SIZE)
		/* no free entries available */
		return -ENOSPC;

set_entry:
	qdev->fw_fat[from] = to;
	set_bit(from, qdev->fw_fat_bmap);
	pr_debug("addr %#llx added to entry %u\n", addr, from);

	return 0;
}

/**
 * @brief Translate a given address based on the given fat entry
 * The translation is done for bits [35-28]
 *
 * @param qdev the qo device
 * @param addr address to translate
 * @return u64 the translated is succuss, zero otherwise
 */
static u64 fat_addr_trans(struct pp_qos_dev *qdev, u64 addr)
{
	u64 t_addr;
	u8 entry;
	u8 msb = FIELD_GET(GENMASK_ULL(35, 28), addr);

	for_each_set_bit(entry, qdev->fw_fat_bmap, FW_FAT_TBL_SIZE) {
		if (qdev->fw_fat[entry] != msb)
			continue;

		t_addr = PP_FIELD_MOD(GENMASK_ULL(35, 28), entry, addr);
		pr_debug("address %#llx ==> address %#llx\n", addr, t_addr);
		return t_addr;
	}

	/* entry not found */
	pr_debug("Failed to translate address %#llx\n", addr);
	return addr;
}

/* the qos fat translate addresses of 32 bits to 32-36 bits addresses */
static s32 configure_at(struct pp_qos_dev *qdev)
{
	u32 reg, i;
	u64 addr;

	/* make sure that each allocated memory block fits under 1 fat
	 * entry size, fat entry resolution is (2^28)-1 --> 256MB
	 */
	if (QOS_ELF_FW_IMAGE_SZ > FW_FAT_ENT_WIN_SZ) {
		pr_err("QOS ELF memory too big for FAT entry\n");
		return -E2BIG;
	}
	if (qdev->hwmem.fw_ddr_sz > FW_FAT_ENT_WIN_SZ) {
		pr_err("QOS FW memory too big for FAT entry\n");
		return -E2BIG;
	}

	/* set all entries as free */
	bitmap_zero(qdev->fw_fat_bmap, FW_FAT_TBL_SIZE);

	/* qos internal addressing entries, this will make entries 0,E,F
	 * not to change the address
	 */
	fat_entry_add(qdev, 0x00000000ULL);
	fat_entry_add(qdev, 0xE0000000ULL);
	fat_entry_add(qdev, 0xF0000000ULL);
	/* entry to access PP addresses */
	fat_entry_add(qdev, qdev->init_params.bm_base);
	/* entry for qos fw code buffer */
	fat_entry_add(qdev, (u64)qdev->fw_txt_phys);
	/* entry for qos fw ddr buffer */
	fat_entry_add(qdev, (u64)qdev->hwmem.fw_ddr_phys);

	/* all other entries are set to use same ddr area as the fw code */
	addr = qdev->fw_txt_phys & ~GENMASK_ULL(31, 28);
	for_each_clear_bit(i, qdev->fw_fat_bmap, FW_FAT_TBL_SIZE)
		fat_entry_add(qdev, (addr | (i << 28)));

	/* configure the table in HW */
	for (i = 0; i < ARRAY_SIZE(qdev->fw_fat); i++) {
		if (i >= 0 && i <= 3)
			addr = PP_QOS_MISC_UC_AT0_CTRL_REG;
		else if (i >= 4 && i <= 7)
			addr = PP_QOS_MISC_UC_AT1_CTRL_REG;
		else if (i >= 8 && i <= 11)
			addr = PP_QOS_MISC_UC_AT2_CTRL_REG;
		else if (i >= 12 && i <= 15)
			addr = PP_QOS_MISC_UC_AT3_CTRL_REG;

		reg = PP_REG_RD32(addr);
		reg &= ~(0xFF << ((i % 4) * BITS_PER_BYTE));
		reg |= (u32)qdev->fw_fat[i] << ((i % 4) * BITS_PER_BYTE);
		PP_REG_WR32(addr, reg);
	}

	pr_debug("set the AT 0-3 reg, val 0x%x\n",
		 PP_REG_RD32(PP_QOS_MISC_UC_AT0_CTRL_REG));
	pr_debug("set the AT 4-7 reg, val 0x%x\n",
		 PP_REG_RD32(PP_QOS_MISC_UC_AT1_CTRL_REG));
	pr_debug("set the AT 8-B reg, val 0x%x\n",
		 PP_REG_RD32(PP_QOS_MISC_UC_AT2_CTRL_REG));
	pr_debug("set the AT C-F reg, val 0x%x\n",
		 PP_REG_RD32(PP_QOS_MISC_UC_AT3_CTRL_REG));

	return 0;
}

/* This function loads the firmware.
 * The firmware is built from a header which holds the major, minor
 * and build numbers.
 * Following the header are sections. Each section is composed of
 * header which holds the memory destination where this section's
 * data should be copied and the size of this section in bytes.
 * After the header comes section's data which is a stream of uint32
 * words.
 * The memory destination on section header designates offset relative
 * to either ddr (a.k.a external) or qos (a.k.a) spaces. Offsets higher
 * than FW_DDR_LOWEST refer to ddr space.
 *
 * Firmware is little endian.
 *
 * When firmware runs it writes 0xCAFECAFE to offset FW_OK_OFFSET of ddr
 * space.
 */
static s32 do_load_firmware(struct pp_qos_dev *qdev,
			    const struct ppv4_qos_fw *fw,
			    unsigned long qos_uc_base)
{
	dma_addr_t phys_txt, phys_data, phys_stack; /* End of stack! */
	struct fw_sec_info *sec_info;
	struct fw_ver host_fwver;
	struct elf_sec *secs;
	u32 alloc_size, raw_version, align = 4;
	u16 num_secs;
	u32 total_sz = 0;
	u32 data_sz = 0;
	u32 stack_sz = 0;
	u32 stack_addr = 0;
	u32 base_addr = 0;
	u32 data_addr = 0;
	void *virt_data, *virt_txt, *virt_stack;
	s32 ret = 0;

	phys_txt = qdev->fw_txt_phys;
	virt_txt = qdev->fw_txt_virt;
	sec_info = &qdev->init_params.fw_sec_data_stack;
	secs = kmalloc_array(ELF_MAX_SECTIONS, sizeof(struct elf_sec),
			     GFP_KERNEL);
	if (!secs)
		return -ENOMEM;

	if (elf_parse(fw->data, fw->size, secs, ELF_MAX_SECTIONS, &num_secs)) {
		QOS_LOG_ERR("ELF parse error!\n");
		kfree(secs);
		return -ENOEXEC;
	}

	calc_elf_sections_sizes(secs, num_secs, &total_sz,
				&data_sz, &data_addr,
				&stack_sz, &stack_addr,
				&base_addr);

	/* No room for data and stack as defined from DTS */
	if ((data_sz + stack_sz) > sec_info->max_size) {
		QOS_LOG_ERR("Need to alloc %u bytes while dts limits to %u\n",
			    (data_sz + stack_sz), sec_info->max_size);
		kfree(secs);
		return -ENOMEM;
	}

	QOS_LOG_DEBUG("=====> fw_data_stack_off %d, %d, %d\n",
		      sec_info->is_in_dccm,
		      sec_info->dccm_offset,
		      sec_info->max_size);

	/* Is data stack sections in DCCM */
	if (sec_info->is_in_dccm) {
		alloc_size = ALIGN(total_sz, align);
	} else {
		alloc_size = ALIGN(total_sz, align) +
			     ALIGN(data_sz, align) +
			     ALIGN(stack_sz, align);
	}
	QOS_LOG_DEBUG("=====> alloc_size %u\n", alloc_size);

	if (alloc_size > QOS_ELF_FW_IMAGE_SZ) {
		QOS_LOG_ERR("fw size [%u] is larger than buffer size [%u]\n",
			    alloc_size, (u32)QOS_ELF_FW_IMAGE_SZ);
		kfree(secs);
		return -ENOMEM;
	}

	if (IS_ENABLED(CONFIG_SOC_LGM)) {
		/* set the address translation in ARC HW */
		ret = configure_at(qdev);
		if (ret)
			return ret;

		phys_txt = fat_addr_trans(qdev, phys_txt);
	}

	if (sec_info->is_in_dccm) {
		phys_data = FW_DCCM_START;
		virt_data = (void *)(qos_uc_base + (data_addr - base_addr));
	} else {
		phys_data = phys_txt + ALIGN(total_sz, align);
		virt_data = (void *)((unsigned long)virt_txt +
				     ALIGN(total_sz, align));
	}

	phys_stack = FW_DCCM_START + (stack_addr - base_addr) + stack_sz;
	virt_stack = (void *)(qos_uc_base + (stack_addr - base_addr));

	pr_debug("Text %#lx [%#lx] (%d), Data %#lx [%#lx] (%d), Stack %#lx (%d)\n",
		 (unsigned long)phys_txt, (unsigned long)(virt_txt), total_sz,
		 (unsigned long)phys_data, (unsigned long)virt_data, data_sz,
		 (unsigned long)phys_stack, stack_sz);

	copy_sections(secs, num_secs, virt_txt, virt_data, virt_stack, phys_txt,
		      phys_data, phys_stack, qos_uc_base + 0x8000,
		      stack_sz, sec_info->is_in_dccm);

	kfree(secs);

	pp_cache_writeback(virt_txt, alloc_size);

	PP_REG_WR32((ulong)&qdev->init_params.dccm_db->wakeup, 0);
	ret = wake_uc();
	if (ret) {
		pr_err("wake_uc failed\n");
		return ret;
	}
	QOS_LOG_DEBUG("waked fw\n");

	if (pp_reg_poll_x((ulong)&qdev->init_params.dccm_db->wakeup, U32_MAX,
			  FW_OK_CODE, PP_REG_MAX_RETRIES)) {
		QOS_LOG_ERR("FW OK value is 0x%x\n", FW_OK_CODE);
		return -ENODEV;
	}

	QOS_LOG_DEBUG("FW is running :)\n");

	host_fwver.major = UC_VERSION_MAJOR;
	host_fwver.minor = UC_VERSION_MINOR;

	raw_version = PP_REG_RD32((ulong)&qdev->init_params.dccm_db->version);
	qdev->fwver.major = raw_version & 0xF;
	qdev->fwver.minor = (raw_version & 0xFF00) >> 8;
	qdev->fwver.build = (raw_version & 0xFF0000) >> 16;

	QOS_LOG_INFO("FW [v %u.%u.%u] is running soc rev %d)\n",
		     qdev->fwver.major, qdev->fwver.minor,
		     qdev->fwver.build, FW_SOC_REV_B);

	if ((host_fwver.major != qdev->fwver.major) ||
	    (host_fwver.minor != qdev->fwver.minor)) {
		QOS_LOG_ERR("Ver mismatch: FW [%u.%u.%u]. Host [%u.%u]\n",
			    qdev->fwver.major, qdev->fwver.minor,
			    qdev->fwver.build, host_fwver.major,
			    host_fwver.minor);
	}

	qdev->init_params.fwcom.cmdbuf_sz =
		PP_REG_RD32((ulong)&qdev->init_params.dccm_db->cmd_buf_size);
	qdev->init_params.fwcom.cmdbuf = (u64)(ulong)qdev->init_params.dccm_db +
		(PP_REG_RD32((ulong)&qdev->init_params.dccm_db->cmd_buf_offset));
	qdev->init_params.fwcom.rspbuf_sz =
		PP_REG_RD32((ulong)&qdev->init_params.dccm_db->cmd_rsp_sz);
	qdev->init_params.fwcom.rspbuf = (u64)(ulong)qdev->init_params.dccm_db +
		(PP_REG_RD32((ulong)&qdev->init_params.dccm_db->cmd_rsp_offset));
	qdev->init_params.fwcom.gaurd = (u64)(ulong)qdev->init_params.dccm_db +
		(PP_REG_RD32((ulong)&qdev->init_params.dccm_db->gaurd_offset));
	qdev->init_params.fwcom.dbg_cnt =
		(debug_counters_t *)((ulong)qdev->init_params.dccm_db +
		(PP_REG_RD32((ulong)&qdev->init_params.dccm_db->cnt_offset)));
	qdev->init_params.fwcom.aqm_db =
		(wred_aqm_db_t *)((ulong)qdev->init_params.dccm_db +
		(PP_REG_RD32((ulong)&qdev->init_params.dccm_db->aqm_offset)));

	QOS_LOG_DEBUG("FW : rspbuf (0x%llx), rspbuf_sz (%zu))\n",
		     qdev->init_params.fwcom.rspbuf,
		     qdev->init_params.fwcom.rspbuf_sz);
	QOS_LOG_DEBUG("FW : cmdbuf (0x%llx), cmdbuf_sz (%zu))\n",
		     qdev->init_params.fwcom.cmdbuf,
		     qdev->init_params.fwcom.cmdbuf_sz);
	QOS_LOG_DEBUG("FW : Guard addr (0x%llx) cnt addr 0x%llx\n",
		     qdev->init_params.fwcom.gaurd,
		     (u64)(ulong)qdev->init_params.fwcom.dbg_cnt);


	return ret;
}

s32 wake_uc(void)
{
	u32 wake_status;

	wake_status = PP_REG_RD32(PP_QOS_MISC_UC_ARC_CTRL_REG);
	if (wake_status == PP_QOS_MISC_UC_ARC_CTRL_ARC_RUN_MSK) {
		pr_err("uC already waked\n");
		return -EINVAL;
	}

	PP_REG_WR32(PP_QOS_MISC_UC_ARC_CTRL_REG,
			PP_QOS_MISC_UC_ARC_CTRL_ARC_RUN_MSK);

	return 0;
}

bool is_uc_awake(void)
{
	u32 wake_status;

	wake_status = PP_REG_RD32(PP_QOS_MISC_UC_ARC_CTRL_REG);
	if (wake_status == 4)
		return true;

	return false;
}

#ifndef PP_QOS_DISABLE_CMDQ
#define CMD_INT_RESPONSE_SZ        4

/* @brief driver internal command id - for debug */
enum cmd_type {
	CMD_TYPE_INIT_LOGGER,
	CMD_TYPE_INIT_QOS,
	CMD_TYPE_MOVE_QUEUE,
	CMD_TYPE_MOVE_SCHED,
	CMD_TYPE_ADD_PORT,
	CMD_TYPE_SET_PORT,
	CMD_TYPE_REM_PORT,
	CMD_TYPE_ADD_SCHED,
	CMD_TYPE_SET_SCHED,
	CMD_TYPE_REM_SCHED,
	CMD_TYPE_ADD_QUEUE,
	CMD_TYPE_SET_QUEUE,
	CMD_TYPE_REM_QUEUE,
	CMD_TYPE_GET_QUEUE_STATS,
	CMD_TYPE_GET_QM_STATS,
	CMD_TYPE_SET_AQM_SF,
	CMD_TYPE_SET_CODEL_CFG,
	CMD_TYPE_GET_CODEL_STATS,
	CMD_TYPE_GET_PORT_STATS,
	CMD_TYPE_PUSH_DESC,
	CMD_TYPE_POP_DESC,
	CMD_TYPE_MCDMA_COPY,
	CMD_TYPE_GET_NODE_INFO,
	CMD_TYPE_READ_TABLE_ENTRY,
	CMD_TYPE_REM_SHARED_GROUP,
	CMD_TYPE_ADD_SHARED_GROUP,
	CMD_TYPE_SET_SHARED_GROUP,
	CMD_TYPE_FLUSH_QUEUE,
	CMD_TYPE_GET_SYSTEM_INFO,
	CMD_TYPE_UPDATE_PORT_TREE,
	CMD_TYPE_SUSPEND_PORT_TREE,
	CMD_TYPE_GET_QUANTA,
	CMD_TYPE_SET_WSP_HELPER,
	CMD_TYPE_GET_WSP_HELPER_STATS,
	CMD_TYPE_SET_MOD_LOG_BMAP,
	CMD_TYPE_GET_MOD_LOG_BMAP,
	CMD_TYPE_INTERNAL,
	CMD_TYPE_GET_VERSION,
	CMD_TYPE_GET_HIST_STATS,
	CMD_TYPE_AQM_Q_TO_CTX,
	CMD_TYPE_SET_PCI_ADDR,
	CMD_TYPE_SET_AQM_ENGINE,
	CMD_TYPE_LAST
};

static const char *const cmd_str[] = {
	[CMD_TYPE_INIT_LOGGER] = "CMD_TYPE_INIT_LOGGER",
	[CMD_TYPE_INIT_QOS] = "CMD_TYPE_INIT_QOS",
	[CMD_TYPE_MOVE_QUEUE] = "CMD_TYPE_MOVE_QUEUE",
	[CMD_TYPE_MOVE_SCHED] = "CMD_TYPE_MOVE_SCHED",
	[CMD_TYPE_ADD_PORT] = "CMD_TYPE_ADD_PORT",
	[CMD_TYPE_SET_PORT] = "CMD_TYPE_SET_PORT",
	[CMD_TYPE_REM_PORT] = "CMD_TYPE_REM_PORT",
	[CMD_TYPE_ADD_SCHED] = "CMD_TYPE_ADD_SCHED",
	[CMD_TYPE_SET_SCHED] = "CMD_TYPE_SET_SCHED",
	[CMD_TYPE_REM_SCHED] = "CMD_TYPE_REM_SCHED",
	[CMD_TYPE_ADD_QUEUE] = "CMD_TYPE_ADD_QUEUE",
	[CMD_TYPE_SET_QUEUE] = "CMD_TYPE_SET_QUEUE",
	[CMD_TYPE_REM_QUEUE] = "CMD_TYPE_REM_QUEUE",
	[CMD_TYPE_GET_QUEUE_STATS] = "CMD_TYPE_GET_QUEUE_STATS",
	[CMD_TYPE_GET_QM_STATS] = "CMD_TYPE_GET_QM_STATS",
	[CMD_TYPE_SET_AQM_SF] = "CMD_TYPE_SET_AQM_SF",
	[CMD_TYPE_SET_CODEL_CFG] = "CMD_TYPE_SET_CODEL_CFG",
	[CMD_TYPE_GET_CODEL_STATS] = "CMD_TYPE_GET_CODEL_STATS",
	[CMD_TYPE_GET_PORT_STATS] = "CMD_TYPE_GET_PORT_STATS",
	[CMD_TYPE_PUSH_DESC] = "CMD_TYPE_PUSH_DESC",
	[CMD_TYPE_POP_DESC] = "CMD_TYPE_POP_DESC",
	[CMD_TYPE_MCDMA_COPY] = "CMD_TYPE_MCDMA_COPY",
	[CMD_TYPE_GET_NODE_INFO] = "CMD_TYPE_GET_NODE_INFO",
	[CMD_TYPE_READ_TABLE_ENTRY] = "CMD_TYPE_READ_TABLE_ENTRY",
	[CMD_TYPE_REM_SHARED_GROUP] = "CMD_TYPE_REM_SHARED_GROUP",
	[CMD_TYPE_ADD_SHARED_GROUP] = "CMD_TYPE_ADD_SHARED_GROUP",
	[CMD_TYPE_SET_SHARED_GROUP] = "CMD_TYPE_SET_SHARED_GROUP",
	[CMD_TYPE_FLUSH_QUEUE] = "CMD_TYPE_FLUSH_QUEUE",
	[CMD_TYPE_GET_SYSTEM_INFO] = "CMD_TYPE_GET_SYSTEM_INFO",
	[CMD_TYPE_UPDATE_PORT_TREE] = "CMD_TYPE_UPDATE_PORT_TREE",
	[CMD_TYPE_SUSPEND_PORT_TREE] = "CMD_TYPE_SUSPEND_PORT_TREE",
	[CMD_TYPE_GET_QUANTA] = "CMD_TYPE_GET_QUANTA",
	[CMD_TYPE_SET_WSP_HELPER] = "CMD_TYPE_SET_WSP_HELPER",
	[CMD_TYPE_GET_WSP_HELPER_STATS] = "CMD_TYPE_GET_WSP_HELPER_STATS",
	[CMD_TYPE_SET_MOD_LOG_BMAP] = "CMD_TYPE_SET_MOD_LOG_BMAP",
	[CMD_TYPE_GET_MOD_LOG_BMAP] = "CMD_TYPE_GET_MOD_LOG_BMAP",
	[CMD_TYPE_INTERNAL] = "CMD_TYPE_INTERNAL",
	[CMD_TYPE_GET_VERSION] = "CMD_TYPE_GET_VERSION",
	[CMD_TYPE_GET_HIST_STATS] = "CMD_TYPE_GET_HIST_STATS",
	[CMD_TYPE_AQM_Q_TO_CTX] = "CMD_TYPE_AQM_Q_TO_CTX",
	[CMD_TYPE_SET_PCI_ADDR] = "CMD_TYPE_SET_PCI_ADDR",
	[CMD_TYPE_SET_AQM_ENGINE] = "CMD_TYPE_SET_AQM_ENGINE",
};

/******************************************************************************/
/*                         Driver commands structures	                      */
/******************************************************************************/
struct cmd {
	u32           id; /* command sequence id - for debug */
#define CMD_FLAGS_POST_PROCESS_DCCM BIT_ULL(0) /*Post process data from DCCM buff */
	u32           flags;
	enum cmd_type type; /* command type - for debug */
	size_t        len; /* Command length */
	void          *response; /* response address */
	u32           int_response_offset; /* response offset in command */
	u32           response_sz; /* num bytes of response */
	unsigned long pos; /* Command offset in FW buffer */
} __attribute__((packed));

struct cmd_internal {
	struct cmd base;
} __attribute__((packed));

struct cmd_init_logger {
	struct cmd base;
	struct fw_cmd_init_logger fw;
} __attribute__((packed));

struct cmd_init_qos {
	struct cmd base;
	struct fw_cmd_init_qos fw;
} __attribute__((packed));

struct cmd_move_sched {
	struct cmd base;
	struct fw_cmd_move_sched fw;
} __attribute__((packed));

struct cmd_move_queue {
	struct cmd base;
	struct fw_cmd_move_queue fw;
} __attribute__((packed));

struct cmd_remove_queue {
	struct cmd base;
	struct fw_cmd_remove_queue fw;
} __attribute__((packed));

struct cmd_remove_port {
	struct cmd base;
	struct fw_cmd_remove_port fw;
} __attribute__((packed));

struct cmd_remove_sched {
	struct cmd base;
	struct fw_cmd_remove_sched fw;
} __attribute__((packed));

struct cmd_add_port {
	struct cmd base;
	struct fw_cmd_add_port fw;
} __attribute__((packed));

struct cmd_set_port {
	struct cmd base;
	struct fw_cmd_set_port fw;
} __attribute__((packed));

struct cmd_add_sched {
	struct cmd base;
	struct fw_cmd_add_sched fw;
} __attribute__((packed));

struct cmd_set_sched {
	struct cmd base;
	struct fw_cmd_set_sched fw;
} __attribute__((packed));

struct cmd_add_queue {
	struct cmd base;
	struct fw_cmd_add_queue fw;
} __attribute__((packed));

struct cmd_set_queue {
	struct cmd base;
	struct fw_cmd_set_queue fw;
} __attribute__((packed));

struct cmd_flush_queue {
	struct cmd base;
	struct fw_cmd_flush_queue fw;
} __attribute__((packed));

struct cmd_set_aqm_sf {
	struct cmd base;
	struct fw_cmd_set_aqm_sf fw;
} __attribute__((packed));

struct cmd_set_codel_cfg {
	struct cmd base;
	struct fw_cmd_set_codel fw;
} __attribute__((packed));

struct cmd_push_desc {
	struct cmd base;
	struct fw_cmd_push_desc fw;
} __attribute__((packed));

struct cmd_pop_desc {
	struct cmd base;
	struct fw_cmd_pop_desc fw;
} __attribute__((packed));

struct cmd_mcdma_copy {
	struct cmd base;
	struct fw_cmd_mcdma_copy fw;
} __attribute__((packed));

struct cmd_read_table_entry {
	struct cmd base;
	struct fw_cmd_read_table_entry fw;
} __attribute__((packed));

struct stub_cmd {
	struct cmd cmd;
	u32 data;
} __attribute__((packed));

struct cmd_add_shared_group {
	struct cmd base;
	struct fw_cmd_add_bwl_group fw;
} __attribute__((packed));

struct cmd_set_shared_group {
	struct cmd base;
	struct fw_cmd_set_bwl_group fw;
} __attribute__((packed));

struct cmd_remove_shared_group {
	struct cmd base;
	struct fw_cmd_rem_bwl_group fw;
} __attribute__((packed));

struct cmd_get_queue_stats {
	struct cmd base;
	struct fw_cmd_get_queue_stats fw;
} __attribute__((packed));

struct cmd_get_qm_stats {
	struct cmd base;
	struct fw_cmd_get_qm_stats fw;
} __attribute__((packed));

struct cmd_get_codel_stat {
	struct cmd base;
	struct fw_cmd_get_codel_stats fw;
} __attribute__((packed));

struct cmd_get_port_stats {
	struct cmd base;
	struct fw_cmd_get_port_stats fw;
} __attribute__((packed));

struct cmd_get_hist_stats {
	struct cmd base;
	struct fw_cmd_get_hist_stats fw;
} __attribute__((packed));

struct cmd_get_node_info {
	struct cmd base;
	struct fw_cmd_get_node_info fw;
} __attribute__((packed));

struct cmd_get_system_info {
	struct cmd base;
	struct fw_cmd_get_system_info fw;
} __attribute__((packed));

struct cmd_update_port_tree {
	struct cmd base;
	struct fw_cmd_update_port_tree fw;
} __attribute__((packed));

struct cmd_suspend_port_tree {
	struct cmd base;
	struct fw_cmd_suspend_port_tree fw;
} __attribute__((packed));

struct cmd_get_quanta {
	struct cmd base;
	struct fw_cmd_get_quanta fw;
} __attribute__((packed));

struct cmd_mod_log_bmap {
	struct cmd base;
	struct fw_cmd_mod_log_bmap fw;
} __attribute__((packed));

struct cmd_set_wsp_helper {
	struct cmd base;
	struct fw_cmd_wsp_helper fw;
} __attribute__((packed));

struct cmd_get_wsp_helper_stats {
	struct cmd base;
	struct fw_cmd_wsp_helper_stats fw;
} __attribute__((packed));

struct cmd_aqm_q_to_ctx {
	struct cmd base;
	struct fw_cmd_aqm_q_to_ctx fw;
} __attribute__((packed));

struct cmd_set_pci_addr {
	struct cmd base;
	struct fw_cmd_set_pci_addr fw;
} __attribute__((packed));

struct cmd_set_aqm_engine {
	struct cmd base;
	struct fw_cmd_set_aqm_engine fw;
} __attribute__((packed));

struct cmd_get_version {
	struct cmd base;
	struct fw_cmd_get_version ver;
} __attribute__((packed));

union driver_cmd {
	struct cmd                     cmd;
	struct stub_cmd                stub;
	struct cmd_init_logger         init_logger;
	struct cmd_init_qos            init_qos;
	struct cmd_move_queue          move_queue;
	struct cmd_move_sched          move_sched;
	struct cmd_remove_queue        rem_queue;
	struct cmd_remove_port         rem_port;
	struct cmd_remove_sched        rem_sched;
	struct cmd_add_port            add_port;
	struct cmd_set_port            set_port;
	struct cmd_add_sched           add_sched;
	struct cmd_set_sched           set_sched;
	struct cmd_add_queue           add_queue;
	struct cmd_set_queue           set_queue;
	struct cmd_get_queue_stats     queue_stats;
	struct cmd_get_qm_stats        qm_stats;
	struct cmd_set_aqm_sf          set_aqm_sf;
	struct cmd_set_codel_cfg       set_codel_cfg;
	struct cmd_get_codel_stat      codel_stats;
	struct cmd_get_port_stats      port_stats;
	struct cmd_add_shared_group    add_shared_group;
	struct cmd_set_shared_group    set_shared_group;
	struct cmd_remove_shared_group remove_shared_group;
	struct cmd_push_desc           pushd;
	struct cmd_get_node_info       node_info;
	struct cmd_read_table_entry    read_table_entry;
	struct cmd_flush_queue         flush_queue;
	struct cmd_get_system_info     sys_info;
	struct cmd_update_port_tree    update_port_tree;
	struct cmd_suspend_port_tree   suspend_port_tree;
	struct cmd_get_quanta          get_quanta;
	struct cmd_mod_log_bmap        mod_log_bmap;
	struct cmd_set_wsp_helper      set_wsp_helper;
	struct cmd_get_wsp_helper_stats get_wsp_helper_stats;
	struct cmd_internal            internal;
	struct cmd_get_hist_stats      hist_stats;
	struct cmd_aqm_q_to_ctx        aqm_q_to_ctx;
} __attribute__((packed));

/******************************************************************************/
/*                                 FW CMDS                                    */
/******************************************************************************/

struct fw_set_common {
	u32 valid;
	s32 suspend;
	u32 bw_limit;
	u32 shared_bw_group;
	u32 max_burst;
	u32 port_phy; /* Relevant only for queue */
};

struct fw_set_parent {
	u32 valid;
	s32 best_effort_enable;
	u16 first;
	u16 last;
	u16 first_wrr;
};

struct fw_set_child {
	u32 valid;
	u32 bw_share;
	u32 preds[QOS_MAX_PREDECESSORS];
};

struct fw_set_port {
	u32 valid;
	u32 ring_addr_h;
	u32 ring_addr_l;
	size_t ring_size;
	s32 active;
	u32 green_threshold;
	u32 yellow_threshold;
};

struct fw_set_queue {
	u32 valid;
	u32 rlm;
	s32 active;
	u32 disable;
	u32 wred_min_avg_green;
	u32 wred_max_avg_green;
	u32 wred_slope_green;
	u32 wred_min_avg_yellow;
	u32 wred_max_avg_yellow;
	u32 wred_slope_yellow;
	u32 wred_min_guaranteed;
	u32 wred_max_allowed;
	u32 wred_fixed_drop_prob_green;
	u32 wred_fixed_drop_prob_yellow;
	s32 codel_en;
	u32 is_fast_q;
};

struct fw_internal {
	struct fw_set_common common;
	struct fw_set_parent parent;
	struct fw_set_child  child;
	union {
		struct fw_set_port port;
		struct fw_set_queue queue;
	} type_data;
	u32 suspend_ports_index;
	u32 num_suspend_ports;
	u32 suspend_ports[PP_QOS_MAX_PORTS];
	u32 pushed;
};

/******************************************************************************/
/*                                FW wrappers                                 */
/******************************************************************************/

static void set_common(struct pp_qos_dev *qdev,
		       const struct pp_qos_common_node_properties *conf,
		       struct fw_set_common *common, u64 modified, u32 phy)
{
	struct qos_node *node;
	u32 valid;

	valid = 0;

	node = get_node_from_phy(qdev->nodes, phy);
	if (unlikely(!node)) {
		QOS_LOG_ERR("get_node_from_phy(%u) returned NULL\n", phy);
		return;
	}

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_BANDWIDTH_LIMIT)) {
		QOS_BITS_SET(valid, TSCD_NODE_CONF_BW_LIMIT);
		if (node_queue(node))
			common->bw_limit = node->bandwidth_limit;
		else
			common->bw_limit = conf->bandwidth_limit;
	}

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_SHARED_GROUP_ID)) {
		QOS_BITS_SET(valid, TSCD_NODE_CONF_SHARED_BWL_GROUP);
		common->shared_bw_group = conf->shared_bw_group;
	}

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_MAX_BURST)) {
		QOS_BITS_SET(valid, TSCD_NODE_CONF_MAX_BURST);
		common->max_burst = conf->max_burst;
	}

	common->valid = valid;
}

static void
update_arbitration(u32 *valid,
		   const struct parent_node_properties *parent_node_prop,
		   struct fw_set_parent *parent)
{
	/* Set parent direct children */
	QOS_BITS_SET(*valid,
		     TSCD_NODE_CONF_FIRST_CHILD |
		     TSCD_NODE_CONF_LAST_CHILD  |
		     TSCD_NODE_CONF_FIRST_WRR_NODE);

	if (parent_node_prop->arbitration == PP_QOS_ARBITRATION_WSP)
		parent->first_wrr = 0;
	else
		parent->first_wrr = parent_node_prop->first_child_phy;

	parent->first = parent_node_prop->first_child_phy;
	parent->last = parent_node_prop->first_child_phy +
		parent_node_prop->num_of_children - 1;
}

static void set_parent(const struct pp_qos_parent_node_properties *conf,
		       const struct parent_node_properties *parent_node_prop,
		       struct fw_set_parent *parent, u64 modified)
{
	u32 valid;

	valid = 0;
	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_ARBITRATION))
		update_arbitration(&valid, parent_node_prop, parent);

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_BEST_EFFORT)) {
		QOS_BITS_SET(valid, TSCD_NODE_CONF_BEST_EFFORT_ENABLE);
		parent->best_effort_enable = conf->best_effort_enable;
	}
	parent->valid = valid;
}

static void set_child(const struct pp_qos_child_node_properties *conf,
		      const struct qos_node *node,
		      struct fw_set_child *child, u64 modified)
{
	u32 valid = 0;

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_BW_WEIGHT)) {
		QOS_BITS_SET(valid, TSCD_NODE_CONF_NODE_WEIGHT);
		child->bw_share = node->child_prop.bw_weight;
	}

	/* Should be changed. Currently both are using bw_share variable */
	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_SHARED_GROUP_ID)) {
		QOS_BITS_SET(valid, TSCD_NODE_CONF_SHARED_BWL_GROUP);
		child->bw_share = conf->wrr_weight;
	}

	child->valid = valid;
}

static void set_port(const struct pp_qos_port_conf *conf,
		     struct fw_set_port *port, u64 modified)
{
	u32 valid = 0;

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_RING_SIZE)) {
		QOS_BITS_SET(valid, PORT_CONF_RING_SIZE);
		port->ring_size = conf->ring_size;
	}

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_RING_ADDRESS)) {
		QOS_BITS_SET(valid, PORT_CONF_RING_ADDRESS_LOW);
		QOS_BITS_SET(valid, PORT_CONF_RING_ADDRESS_HIGH);
		port->ring_addr_l = (u32)conf->ring_address;
		port->ring_addr_h = (u32)((u64)conf->ring_address >> 32);
	}

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_DISABLE)) {
		QOS_BITS_SET(valid, PORT_CONF_ACTIVE);
		port->active = !conf->disable;
	}

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_PORT_GREEN_THRESHOLD)) {
		QOS_BITS_SET(valid, PORT_CONF_GREEN_THRESHOLD);
		port->green_threshold = conf->green_threshold;
	}

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_PORT_YELLOW_THRESHOLD)) {
		QOS_BITS_SET(valid, PORT_CONF_YELLOW_THRESHOLD);
		port->yellow_threshold = conf->yellow_threshold;
	}
	port->valid = valid;
}

static void set_queue(const struct pp_qos_queue_conf *conf,
		      struct fw_set_queue *queue, u64 modified)
{
	u32 valid = 0;
	u32 disable = 0;

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_WRED_MAX_ALLOWED)) {
		QOS_BITS_SET(valid, WRED_QUEUE_CONF_MAX_ALLOWED);
		queue->wred_max_allowed = conf->wred_max_allowed;
	}

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_BLOCKED)) {
		QOS_BITS_SET(valid, WRED_QUEUE_CONF_ACTIVE_Q);
		QOS_BITS_SET(valid, WRED_QUEUE_CONF_MAX_ALLOWED);
		queue->active = !conf->blocked;
		if (queue->active) {
			queue->wred_max_allowed =
				conf->wred_max_allowed;
		} else {
			queue->wred_max_allowed = 0;
		}
	}

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_WRED_ENABLE)) {
		QOS_BITS_SET(valid, WRED_QUEUE_CONF_DISABLE);
		if (!conf->wred_enable)
			QOS_BITS_SET(disable, WRED_FLAGS_DISABLE_WRED);
	}
	if (QOS_BITS_IS_SET(modified,
			    QOS_MODIFIED_WRED_FIXED_DROP_PROB_ENABLE)) {
		QOS_BITS_SET(valid, WRED_QUEUE_CONF_DISABLE);
		if (conf->wred_fixed_drop_prob_enable)
			QOS_BITS_SET(disable, WRED_FLAGS_FIXED_PROBABILITY);
	}
	queue->disable = disable;

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_WRED_FIXED_GREEN_PROB)) {
		QOS_BITS_SET(valid, WRED_QUEUE_CONF_FIXED_GREEN_DROP_P);
		queue->wred_fixed_drop_prob_green =
			conf->wred_fixed_drop_prob_green;
	}

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_WRED_FIXED_YELLOW_PROB)) {
		QOS_BITS_SET(valid, WRED_QUEUE_CONF_FIXED_YELLOW_DROP_P);
		queue->wred_fixed_drop_prob_yellow =
			conf->wred_fixed_drop_prob_yellow;
	}

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_WRED_MIN_YELLOW)) {
		QOS_BITS_SET(valid, WRED_QUEUE_CONF_MIN_AVG_YELLOW);
		queue->wred_min_avg_yellow = conf->wred_min_avg_yellow;
	}

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_WRED_MAX_YELLOW)) {
		QOS_BITS_SET(valid, WRED_QUEUE_CONF_MAX_AVG_YELLOW);
		queue->wred_max_avg_yellow = conf->wred_max_avg_yellow;
	}

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_WRED_SLOPE_YELLOW)) {
		QOS_BITS_SET(valid, WRED_QUEUE_CONF_SLOPE_YELLOW);
		queue->wred_slope_yellow = conf->wred_slope_yellow;
	}

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_WRED_MIN_GREEN)) {
		QOS_BITS_SET(valid, WRED_QUEUE_CONF_MIN_AVG_GREEN);
		queue->wred_min_avg_green = conf->wred_min_avg_green;
	}

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_WRED_MAX_GREEN)) {
		QOS_BITS_SET(valid, WRED_QUEUE_CONF_MAX_AVG_GREEN);
		queue->wred_max_avg_green = conf->wred_max_avg_green;
	}

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_WRED_SLOPE_GREEN)) {
		QOS_BITS_SET(valid, WRED_QUEUE_CONF_SLOPE_GREEN);
		queue->wred_slope_green = conf->wred_slope_green;
	}

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_WRED_MIN_GUARANTEED)) {
		QOS_BITS_SET(valid, WRED_QUEUE_CONF_MIN_GUARANTEED);
		queue->wred_min_guaranteed = conf->wred_min_guaranteed;
	}

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_CODEL)) {
		QOS_BITS_SET(valid, TXMGR_QUEUE_CODEL_EN);
		queue->codel_en = conf->codel_en;
	}

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_FAST_Q)) {
		QOS_BITS_SET(valid, QM_FAST_Q);
		queue->is_fast_q = conf->is_fast_q;
	}

	queue->valid = valid;
}

/******************************************************************************/
/*                         Driver functions                                   */
/******************************************************************************/
#ifndef CONFIG_SOC_LGM
static s32 alloc_fw_buf(struct pp_qos_dev *qdev, struct mxl_fw_dl_data *data)
{
	qdev->fw_txt_virt =
		dmam_alloc_coherent(pp_dev_get(), QOS_ELF_FW_IMAGE_SZ,
				    &qdev->fw_txt_phys, GFP_KERNEL);
	
	if (!qdev->fw_txt_virt)	{
		QOS_LOG_ERR("failed to alloc firmware memory\n");
		return -ENOMEM;
	}

	return 0;
}
#else
static s32 alloc_fw_buf(struct pp_qos_dev *qdev, struct mxl_fw_dl_data *data)
{	
	unsigned long attr = 0;

	/* if need to use the fw ddr access as none coherent
	 * need to add the below:
	 * 1. data->opt |= MXL_FW_OPT_USE_NONCOHERENT;
	 * 2. attr = DMA_ATTR_NON_CONSISTENT;
	 */
	qdev->fw_txt_virt = mxl_soc_alloc_firmware_buf(pp_dev_get(),
						       QOS_ELF_FW_IMAGE_SZ,
						       data);
	if (!qdev->fw_txt_virt)	{
		QOS_LOG_ERR("failed to alloc firmware memory\n");
		return -ENOMEM;
	}
	qdev->fw_txt_phys = dma_map_single_attrs(pp_dev_get(),
						 qdev->fw_txt_virt,
						 QOS_ELF_FW_IMAGE_SZ,
						 DMA_TO_DEVICE, attr);
	if (dma_mapping_error(pp_dev_get(), qdev->fw_txt_phys)) {
		QOS_LOG_ERR("DMA map failed\n");
		qos_fw_release(qdev);
		return -EIO;
	}

	return 0;
}
#endif

s32 load_qos_firmware(struct pp_qos_dev *qdev, const char *name)
{
	s32 err;
	const struct firmware *firmware;
	struct ppv4_qos_fw fw;
	struct mxl_fw_dl_data data = {0};

	QOS_LOG_DEBUG("\n");

	if (is_uc_awake()) {
		pr_info("Firmware already loaded\n");
		return 0;
	}

	data.sai = -1;
	data.opt = MXL_FW_OPT_SKIP_FWRULE | MXL_FW_OPT_RW_HW_FWRULE;

	err = alloc_fw_buf(qdev, &data);
	if (err)
		return err;

	if (mxl_soc_request_firmware_into_buf(&firmware, name,
					      pp_dev_get(), qdev->fw_txt_virt,
					      QOS_ELF_FW_IMAGE_SZ, &data)) {
		QOS_LOG_ERR("failed to load firmware: %s\n", name);
		qos_fw_release(qdev);
		return -EIO;
	}
	fw.size = firmware->size;
	fw.data = firmware->data;
	QOS_LOG_DEBUG("virt 0x%lx phy 0x%llx allocSize = %d\n ",
		      (unsigned long)qdev->fw_txt_virt,
		      (u64)qdev->fw_txt_phys, QOS_ELF_FW_IMAGE_SZ);

	err = do_load_firmware(qdev, &fw, qdev->init_params.qos_uc_base);

	/* Change the rule to read only */
	mxl_soc_request_protect_buf(pp_dev_get(), qdev->fw_txt_virt,
				    QOS_ELF_FW_IMAGE_SZ, &data);
	mxl_soc_request_protect_buf_from_dev(pp_dev_get(), qdev->fw_txt_virt,
					     QOS_ELF_FW_IMAGE_SZ, &data);
	mxl_soc_release_firmware(firmware, pp_dev_get(), NULL, 0, &data);

	if (err)
		qos_fw_release(qdev);

	return err;
}

static int __init qos_fw_log_en(char *str)
{
	QOS_LOG_INFO("PPv4 QoS FW Logger is enabled\n");
	qos_fw_log = true;

	return 0;
}

early_param("qosFwLog", qos_fw_log_en);

#ifndef CONFIG_SOC_LGM
static void init_wsp_helper_cfg(struct pp_qos_dev *qdev,
				struct wsp_helper_cfg *cfg)
{
	cfg->enable_prx = qdev->init_params.enhanced_wsp;
	cfg->timeout = QOS_EWSP_DEFAULT_TIMEOUT_US_PRX;
	cfg->byte_threshold_prx = QOS_EWSP_DEFAULT_BYTE_THRESHOLD_PRX;
}
#else
static void init_wsp_helper_cfg(struct pp_qos_dev *qdev,
				struct wsp_helper_cfg *cfg)
{
	if (qdev->init_params.enhanced_wsp)
		cfg->timeout = QOS_EWSP_DEFAULT_INTERVAL_LGM;
}
#endif

s32 qos_fw_start(struct pp_qos_dev *qdev)
{
	struct wsp_helper_cfg *ewsp_cfg;
	s32 ret;

	/* fix fw addresses based on fat configuration */
	qdev->hwmem.fw_logger_ddr_phys =
		fat_addr_trans(qdev, qdev->hwmem.fw_logger_ddr_phys);
	qdev->hwmem.fw_stats_ddr_phys =
		fat_addr_trans(qdev, qdev->hwmem.fw_stats_ddr_phys);
	qdev->hwmem.max_allow_ddr_phys =
		fat_addr_trans(qdev, qdev->hwmem.max_allow_ddr_phys);
	qdev->hwmem.wsp_queues_ddr_phys =
		fat_addr_trans(qdev, qdev->hwmem.wsp_queues_ddr_phys);
	qdev->hwmem.bwl_ddr_phys =
		fat_addr_trans(qdev, qdev->hwmem.bwl_ddr_phys);
	qdev->hwmem.sbwl_ddr_phys =
		fat_addr_trans(qdev, qdev->hwmem.sbwl_ddr_phys);
	qdev->hwmem.dma_buff_phys =
		fat_addr_trans(qdev, qdev->hwmem.dma_buff_phys);

	if (qos_fw_log) {
		create_init_logger_cmd(qdev, UC_LOGGER_LEVEL_INFO,
				       UC_LOGGER_MODE_WRITE_HOST_MEM);
	} else {
		create_init_logger_cmd(qdev, UC_LOGGER_LEVEL_DEFAULT,
				       UC_LOGGER_MODE_WRITE_HOST_MEM);
	}

	create_init_qos_cmd(qdev);

	ewsp_cfg = &qdev->ewsp_db.cfg;
	memset(ewsp_cfg, 0, sizeof(*ewsp_cfg));
	init_wsp_helper_cfg(qdev, ewsp_cfg);
	create_set_wsp_helper_cmd(qdev, ewsp_cfg);

	update_cmd_id(&qdev->drvcmds);
	ret = transmit_cmds(qdev);
	if (unlikely(ret))
		return ret;

	qdev->initialized = true;

	return 0;
}

s32 qos_fw_init(struct pp_qos_dev *qdev)
{
	s32 ret;

	ret = load_qos_firmware(qdev, QOS_FIRMWARE_FILE);
	if (unlikely(ret))
		return ret;

	return qos_fw_start(qdev);
}

void qos_fw_release(struct pp_qos_dev *qdev)
{
	struct mxl_fw_dl_data data = {0};

	if (!qdev->fw_txt_virt)
		return;

	data.sai = -1;
	mxl_soc_free_firmware_buf(pp_dev_get(), qdev->fw_txt_virt,
				  QOS_ELF_FW_IMAGE_SZ, &data);
}

/* Following functions creates commands in driver fromat to be stored at
 * drivers queues before sending to firmware
 */

/* Extract ancestors of node from driver's DB */
void fill_preds(const struct pp_nodes *nodes,
		       u32 phy, u32 *preds, size_t size)
{
	u32 i;
	const struct qos_node *node;

	i = 0;
	memset(preds, 0x0, size * sizeof(u32));
	node = get_const_node_from_phy(nodes, phy);
	if (unlikely(!node)) {
		QOS_LOG_ERR("get_const_node_from_phy(%u) returned NULL\n",
			    phy);
		return;
	}

	while (node_child(node) && (i < size)) {
		preds[i] = node->child_prop.parent_phy;
		node = get_const_node_from_phy(nodes,
					       node->child_prop.parent_phy);
		i++;
	}
}

static void cmd_init(const struct pp_qos_dev *qdev,
		     struct cmd *cmd, enum cmd_type type, size_t len,
		     u32 flags, void *response, u32 int_response_offset,
		     u32 response_sz)
{
	cmd->flags = flags;
	cmd->type = type;
	cmd->len = len;
	cmd->id = qdev->drvcmds.cmd_id;
	cmd->response = response;
	cmd->int_response_offset = int_response_offset;
	cmd->response_sz = response_sz;

	if ((flags & CMD_FLAGS_POST_PROCESS_DCCM)
		 && !response)
		QOS_LOG_ERR("POST PROCESS Ext without response ptr\n");

	if ((flags & (CMD_FLAGS_POST_PROCESS_DCCM))
	     && !response_sz)
		QOS_LOG_ERR("POST PROCESS flag without response size\n");
}

void create_init_logger_cmd(struct pp_qos_dev *qdev,
			    enum uc_logger_level level,
			    enum uc_logger_mode mode)
{
	struct cmd_init_logger cmd;

	cmd_init(qdev, &cmd.base, CMD_TYPE_INIT_LOGGER,
		 sizeof(cmd), 0, NULL, 0, 0);
	cmd.fw.base.type = UC_QOS_CMD_INIT_UC_LOGGER;

	cmd.fw.write_idx_addr = (u32)qdev->hwmem.fw_logger_ddr_phys +
		PPV4_QOS_LOGGER_WRITE_IDX_OFFSET;

	cmd.fw.read_idx_addr = (u32)qdev->hwmem.fw_logger_ddr_phys +
		PPV4_QOS_LOGGER_READ_IDX_OFFSET;

	cmd.fw.ring_addr = (u32)qdev->hwmem.fw_logger_ddr_phys +
		PPV4_QOS_LOGGER_RING_OFFSET;

	cmd.fw.mode = mode;
	cmd.fw.level = level;
	cmd.fw.num_of_msgs =
		(qdev->hwmem.fw_logger_sz - PPV4_QOS_LOGGER_HEADROOM_SIZE)
		/ PPV4_QOS_LOGGER_MSG_SIZE;
	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_INIT_LOGGER\n", qdev->drvcmds.cmd_id);
	cmd_queue_put(qdev->drvcmds.cmdq, (u8 *)&cmd, sizeof(cmd));
}

void create_get_version_cmd(const struct pp_qos_dev *qdev, struct fw_cmd_get_version *ver)
{
	struct cmd_get_version cmd;

	cmd_init(qdev, &cmd.base, CMD_TYPE_GET_VERSION,
		 sizeof(cmd), CMD_FLAGS_POST_PROCESS_DCCM, (void *)ver,
		 0, sizeof(struct fw_cmd_get_version));
		//sizeof(cmd), 0, NULL, 0, 0);
	cmd.ver.base.type = UC_QOS_CMD_GET_FW_VERSION;

	cmd_queue_put(qdev->drvcmds.cmdq, (u8 *)&cmd, sizeof(cmd));
}

void create_init_qos_cmd(struct pp_qos_dev *qdev)
{
	struct cmd_init_qos cmd;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	cmd_init(qdev, &cmd.base, CMD_TYPE_INIT_QOS,
		 sizeof(cmd), 0, NULL, 0, 0);
	cmd.fw.base.type = UC_QOS_CMD_INIT_QOS;

	if (IS_ENABLED(CONFIG_SOC_LGM))
		cmd.fw.qm_ddr_start = qdev->hwmem.qm_ddr_phys >> 4;
	else
		cmd.fw.qm_ddr_start = qdev->hwmem.qm_ddr_phys;
	cmd.fw.qm_num_pages =
		qdev->init_params.wred_total_avail_resources /
		PPV4_QOS_DESC_IN_PAGE;
	cmd.fw.wred_total_avail_resources =
		qdev->init_params.wred_total_avail_resources;
	cmd.fw.wred_prioritize_pop = qdev->init_params.wred_prioritize_pop;
	cmd.fw.wred_avg_q_size_p = qdev->init_params.wred_p_const;
	cmd.fw.wred_max_q_size = qdev->init_params.wred_max_q_size;
	cmd.fw.num_of_ports = qdev->init_params.max_ports;
	cmd.fw.hw_clk = pp_dev_hw_clk_get();
	cmd.fw.bm_base = qdev->init_params.bm_base;
	cmd.fw.soc_rev = FW_SOC_REV_B;
	cmd.fw.bwl_ddr_base = (u32)qdev->hwmem.bwl_ddr_phys;
	cmd.fw.sbwl_ddr_base = (u32)qdev->hwmem.sbwl_ddr_phys;
	cmd.fw.wsp_queues_ddr_base = (u32)qdev->hwmem.wsp_queues_ddr_phys;
	cmd.fw.aqm_engine = qdev->init_params.aqm_engine;

#ifdef CONFIG_PPV4_HW_MOD_REGS_LOGS
	cmd.fw.mod_reg_log_en = CONFIG_PPV4_HW_MOD_REGS_LOGS_EN & UC_MOD_ALL;
#else
	cmd.fw.mod_reg_log_en = 0;
#endif
#ifdef CONFIG_SOC_LGM
	cmd.fw.tbm_prescale_addr =
		fat_addr_trans(qdev, chk_tbm_prescaler_phys_addr_get());
	cmd.fw.chk_crawler_prescale_addr =
		fat_addr_trans(qdev, chk_crawler_prescaler_phys_addr_get());
	cmd.fw.egress_uc_prescale_addr =
		fat_addr_trans(qdev, uc_egr_cnt64_tmr_ticks_phys_get());
	cmd.fw.egress_uc_tdox_timer_addr =
		fat_addr_trans(qdev, uc_egr_tdox_tmr_ticks_phys_get());
	cmd.fw.codel_target = CODEL_DFLT_TARGET_DELAY_MSEC;
	cmd.fw.codel_interval = CODEL_DFLT_INTERVAL_TIME_MSEC;
	cmd.fw.egress_uc_aqm_info_addr =
		fat_addr_trans(qdev, uc_egr_lld_aqm_info_phys_addr_get());

	/* updating CoDel DB */
	qdev->codel_cfg.target_delay_msec = cmd.fw.codel_target;
	qdev->codel_cfg.interval_time_msec = cmd.fw.codel_interval;
#endif
	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_INIT_QOS\n", qdev->drvcmds.cmd_id);
	cmd_queue_put(qdev->drvcmds.cmdq, (u8 *)&cmd, sizeof(cmd));
}

void create_move_cmd(struct pp_qos_dev *qdev, u16 dst, u16 src, u16 dst_port)
{
	struct cmd_move_queue queue_cmd = { 0 };
	struct cmd_move_sched sched_cmd = { 0 };
	const struct qos_node *node;
	enum cmd_type type;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	/* If moving from tmp node, use the stored information */
	if (src == PP_QOS_TMP_NODE)
		node = get_const_node_from_phy(qdev->nodes, src);
	else
		node = get_const_node_from_phy(qdev->nodes, dst);

	if (unlikely(!node)) {
		QOS_LOG_ERR("get_const_node_from_phy(%u) returned NULL\n",
			    dst);
		return;
	}

	add_suspend_port(qdev, dst_port);

	if (node->type == TYPE_QUEUE) {
		cmd_init(qdev, &queue_cmd.base, CMD_TYPE_MOVE_QUEUE,
			 sizeof(queue_cmd), 0, NULL, 0, 0);
		queue_cmd.fw.base.type = UC_QOS_CMD_MOVE_QUEUE;
		type = CMD_TYPE_MOVE_QUEUE;
		queue_cmd.fw.src = src;
		queue_cmd.fw.dst = dst;
		queue_cmd.fw.dst_port = dst_port;
		queue_cmd.fw.rlm = node->data.queue.rlm;
		queue_cmd.fw.is_alias = node->data.queue.is_alias;
		fill_preds(qdev->nodes, dst, queue_cmd.fw.preds,
			   QOS_MAX_PREDECESSORS);
		cmd_queue_put(qdev->drvcmds.cmdq,
			      (u8 *)&queue_cmd,
			      sizeof(queue_cmd));
	} else if (node->type == TYPE_SCHED) {
		cmd_init(qdev, &sched_cmd.base, CMD_TYPE_MOVE_SCHED,
			 sizeof(sched_cmd), 0, NULL, 0, 0);
		sched_cmd.fw.base.type = UC_QOS_CMD_MOVE_SCHEDULER;
		type = CMD_TYPE_MOVE_SCHED;
		sched_cmd.fw.src = src;
		sched_cmd.fw.dst = dst;
		fill_preds(qdev->nodes, dst, sched_cmd.fw.preds,
			   QOS_MAX_PREDECESSORS);
		cmd_queue_put(qdev->drvcmds.cmdq, (u8 *)&sched_cmd,
			      sizeof(sched_cmd));
	} else {
		QOS_LOG_ERR("Node type %u does not support move operation\n",
			    node->type);
		return;
	}

	QOS_LOG_DEBUG("cmd %u: %s %u-->%u type:%d, port:%u\n",
		      qdev->drvcmds.cmd_id, cmd_str[type],
		      src, dst, node->type, dst_port);
}

void create_remove_node_cmd(struct pp_qos_dev *qdev,
			    enum node_type ntype, u32 phy, u32 rlm)
{
	struct cmd_remove_queue queue_cmd;
	struct cmd_remove_port port_cmd;
	struct cmd_remove_sched sched_cmd;
	enum cmd_type ctype;
	const struct qos_node *node;
	void *cmd;
	u32 cmd_size;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	node = get_const_node_from_phy(qdev->nodes, phy);
	if (unlikely(!node)) {
		QOS_LOG_ERR("get_const_node_from_phy(%u) returned NULL\n", phy);
		return;
	}

	switch (ntype) {
	case TYPE_PORT:
		ctype = CMD_TYPE_REM_PORT;
		cmd_init(qdev, &port_cmd.base, ctype,
			 sizeof(port_cmd), 0, NULL, 0, 0);
		port_cmd.fw.base.type = UC_QOS_CMD_REM_PORT;
		port_cmd.fw.phy = phy;
		cmd = (void *)&port_cmd;
		cmd_size = sizeof(port_cmd);
		break;
	case TYPE_SCHED:
		ctype = CMD_TYPE_REM_SCHED;
		cmd_init(qdev, &sched_cmd.base, ctype,
			 sizeof(sched_cmd), 0, NULL, 0, 0);
		sched_cmd.fw.base.type = UC_QOS_CMD_REM_SCHEDULER;
		sched_cmd.fw.phy = phy;
		cmd = (void *)&sched_cmd;
		cmd_size = sizeof(sched_cmd);
		break;
	case TYPE_QUEUE:
		ctype = CMD_TYPE_REM_QUEUE;
		cmd_init(qdev, &queue_cmd.base, ctype,
			 sizeof(queue_cmd), 0, NULL, 0, 0);
		queue_cmd.fw.base.type = UC_QOS_CMD_REM_QUEUE;
		queue_cmd.fw.phy = phy;
		queue_cmd.fw.rlm = rlm;
		queue_cmd.fw.is_alias = node->data.queue.is_alias;
		queue_cmd.fw.is_fast_q = node->data.queue.is_fast_q;
		cmd = (void *)&queue_cmd;
		cmd_size = sizeof(queue_cmd);
		break;
	case TYPE_UNKNOWN:
		QOS_ASSERT(0, "Unexpected unknown type\n");
		return;
	default:
		QOS_LOG_ERR("illegal node type %d\n", ntype);
		return;
	}

	QOS_LOG_DEBUG("cmd %u: %s %u rlm %u alias %u\n",
		      qdev->drvcmds.cmd_id,
		      cmd_str[ctype], phy, rlm,
		      node->data.queue.is_alias);

	if ((ctype != CMD_TYPE_REM_PORT) && (!node_syncq(node)))
		add_suspend_port(qdev, get_port(qdev->nodes, phy));

	cmd_queue_put(qdev->drvcmds.cmdq, cmd, cmd_size);
}

u32 tscd_get_normalized_bw_limit(struct pp_qos_dev *qdev, u32 bw_limit)
{
	u32 credit_inc;
	u32 num_updates_per_sec;

	num_updates_per_sec = PP_REG_RD32((ulong)&qdev->init_params.dccm_db->num_updates_per_sec);
	if (num_updates_per_sec) {
		credit_inc = TSCD_KBPS_TO_CREDIT(bw_limit, num_updates_per_sec);
		return TSCD_CREDIT_TO_KBPS(credit_inc, num_updates_per_sec);
	}

	return 0;
}

static void create_add_port_cmd(struct pp_qos_dev *qdev,
				const struct pp_qos_port_conf *conf, u32 phy)
{
	struct cmd_add_port cmd = { 0 };
	struct qos_node *node;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_ADD_PORT, sizeof(cmd),
		 0, NULL, 0, 0);

	cmd.fw.base.type = UC_QOS_CMD_ADD_PORT;
	cmd.fw.port_phy = phy;
	cmd.fw.active = !conf->disable;
	cmd.fw.params.first_child = 0;
	cmd.fw.params.last_child = 0;
	cmd.fw.params.bw_limit = conf->common_prop.bandwidth_limit;
	cmd.fw.params.best_effort = conf->port_parent_prop.best_effort_enable;
	cmd.fw.params.first_wrr_node = 0;
	cmd.fw.params.shared_bwl_entry = conf->common_prop.shared_bw_group;
	cmd.fw.params.max_burst = conf->common_prop.max_burst;
	cmd.fw.disable_byte_credit = conf->packet_credit_enable;
	cmd.fw.ring_params.txmgr_ring_sz = conf->ring_size;
	cmd.fw.ring_params.txmgr_ring_addr_h =
		(u32)((u64)conf->ring_address >> 32);
	cmd.fw.ring_params.txmgr_ring_addr_l =
		(u32)conf->ring_address;
	cmd.fw.txmgr_initial_port_credit = conf->credit;
	cmd.fw.green_thr = conf->green_threshold;
	cmd.fw.yellow_thr = conf->yellow_threshold;

	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_ADD_PORT %u\n",
		      qdev->drvcmds.cmd_id, phy);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));

	node = get_node_from_phy(qdev->nodes, phy);
	if (unlikely(!node)) {
		QOS_LOG_ERR("get_node_from_phy(%u) returned NULL\n", phy);
		return;
	}
	node->actual_bandwidth_limit =
		tscd_get_normalized_bw_limit(qdev, cmd.fw.params.bw_limit);
}

static s32 _create_set_port_cmd(struct pp_qos_dev *qdev,
				 u32 phy,
				 u64 modified,
				 struct fw_set_common *common,
				 struct fw_set_parent *parent,
				 struct fw_set_port *port)
{
	struct cmd_set_port cmd;
	struct qos_node *node;

	if ((common->valid | parent->valid | port->valid) == 0) {
		QOS_LOG_DEBUG("IGNORING EMPTY CMD_TYPE_SET_PORT phy %d\n", phy);
		return -EINVAL;
	}

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_SET_PORT, sizeof(cmd), 0, NULL,
		 0, 0);

	cmd.fw.base.type = UC_QOS_CMD_SET_PORT;
	cmd.fw.port_phy = phy;
	cmd.fw.valid = common->valid | parent->valid;

	if (common->valid) {
		cmd.fw.params.shared_bwl_entry = common->shared_bw_group;
		cmd.fw.params.bw_limit = common->bw_limit;
		cmd.fw.params.max_burst = common->max_burst;
	}

	if (parent->valid)
		cmd.fw.params.best_effort = parent->best_effort_enable;

	if (port->valid) {
		cmd.fw.port_valid = port->valid;
		cmd.fw.ring_params.txmgr_ring_sz = port->ring_size;
		cmd.fw.ring_params.txmgr_ring_addr_l = port->ring_addr_l;
		cmd.fw.ring_params.txmgr_ring_addr_h = port->ring_addr_h;
		cmd.fw.active = port->active;
		cmd.fw.green_thr = port->green_threshold;
		cmd.fw.yellow_thr = port->yellow_threshold;
	}

	if (parent->first > parent->last) {
		parent->first = 0;
		parent->last = 0;
		parent->first_wrr = 0;
	}

	if (QOS_BITS_IS_SET(common->valid, TSCD_NODE_CONF_SUSPEND_RESUME))
		cmd.fw.suspend = common->suspend;

	if (QOS_BITS_IS_SET(parent->valid, TSCD_NODE_CONF_FIRST_CHILD))
		cmd.fw.params.first_child = parent->first;

	if (QOS_BITS_IS_SET(parent->valid, TSCD_NODE_CONF_LAST_CHILD))
		cmd.fw.params.last_child = parent->last;

	if (QOS_BITS_IS_SET(parent->valid, TSCD_NODE_CONF_FIRST_WRR_NODE))
		cmd.fw.params.first_wrr_node = parent->first_wrr;

	add_suspend_port(qdev, phy);
	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));

	if (QOS_BITS_IS_SET(common->valid, TSCD_NODE_CONF_BW_LIMIT)) {
		node = get_node_from_phy(qdev->nodes, phy);
		if (unlikely(!node)) {
			QOS_LOG_ERR("get_node_from_phy(%u) returned NULL\n", phy);
			return -EINVAL;
		}
		node->actual_bandwidth_limit =
			tscd_get_normalized_bw_limit(qdev, cmd.fw.params.bw_limit);
	}

	return 0;
}

s32 create_set_port_cmd(struct pp_qos_dev *qdev,
			 const struct pp_qos_port_conf *conf,
			 u32 phy, u64 modified)
{
	struct fw_set_common common;
	struct fw_set_parent parent;
	struct fw_set_port port;
	struct qos_node *node;
	s32 ret;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return -ENODEV;

	/* Only external parameters may change here */
	memset(&common, 0, sizeof(struct fw_set_common));
	memset(&parent, 0, sizeof(struct fw_set_parent));
	memset(&port, 0, sizeof(struct fw_set_port));

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_NODE_TYPE)) {
		create_add_port_cmd(qdev, conf, phy);
	} else {
		node = get_node_from_phy(qdev->nodes, phy);
		if (unlikely(!node)) {
			QOS_LOG_ERR("get_node_from_phy(%u) returned NULL\n", phy);
			return -EINVAL;
		}
		set_common(qdev, &conf->common_prop, &common, modified, phy);
		set_parent(&conf->port_parent_prop, &node->parent_prop,
			   &parent, modified);
		set_port(conf, &port, modified);
		ret = _create_set_port_cmd(qdev, phy, modified,
				     &common, &parent, &port);
		if (ret)
			return ret;
	}

	if (!QOS_BITS_IS_SET(modified, QOS_MODIFIED_NODE_TYPE))
		QOS_LOG_DEBUG("cmd %u: CMD_TYPE_SET_PORT %u\n",
			      qdev->drvcmds.cmd_id, phy);

	return 0;
}

static void create_add_sched_cmd(struct pp_qos_dev *qdev,
				 const struct pp_qos_sched_conf *conf,
				 u32 phy)
{
	struct cmd_add_sched cmd;
	struct qos_node *node;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_ADD_SCHED, sizeof(cmd),
		 0, NULL, 0, 0);

	cmd.fw.base.type = UC_QOS_CMD_ADD_SCHEDULER;
	cmd.fw.phy = phy;
	cmd.fw.first_child = 0;
	cmd.fw.last_child = 0;
	cmd.fw.bw_limit = conf->common_prop.bandwidth_limit;
	cmd.fw.best_effort = conf->sched_parent_prop.best_effort_enable;
	cmd.fw.first_wrr_node = 0;
	cmd.fw.shared_bwl_entry = conf->common_prop.shared_bw_group;
	cmd.fw.max_burst = conf->common_prop.max_burst;
	fill_preds(qdev->nodes, phy, cmd.fw.preds, QOS_MAX_PREDECESSORS);

	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_ADD_SCHED %u\n",
		      qdev->drvcmds.cmd_id, phy);

	add_suspend_port(qdev, get_port(qdev->nodes, phy));
	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));

	node = get_node_from_phy(qdev->nodes, phy);
	if (unlikely(!node)) {
		QOS_LOG_ERR("get_node_from_phy(%u) returned NULL\n", phy);
		return;
	}
	node->actual_bandwidth_limit =
		tscd_get_normalized_bw_limit(qdev, cmd.fw.bw_limit);
}

static void _create_set_sched_cmd(struct pp_qos_dev *qdev,
				  u32 phy,
				  u64 modified,
				  struct fw_set_common *common,
				  struct fw_set_parent *parent,
				  struct fw_set_child *child)
{
	struct cmd_set_sched cmd;
	struct qos_node *node;
	u32 pred_id;

	if ((common->valid | parent->valid | child->valid) == 0) {
		QOS_LOG_DEBUG("ignoring empty %s (phy %u)\n",
			 cmd_str[CMD_TYPE_SET_SCHED], phy);
		return;
	}

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_SET_SCHED, sizeof(cmd), 0, NULL,
		 0, 0);

	cmd.fw.base.type = UC_QOS_CMD_SET_SCHEDULER;
	cmd.fw.phy = phy;

	cmd.fw.best_effort = parent->best_effort_enable;
	cmd.fw.node_weight = child->bw_share;
	cmd.fw.valid = common->valid | parent->valid | child->valid;

	if (common->valid) {
		cmd.fw.bw_limit = common->bw_limit;
		cmd.fw.shared_bwl_entry = common->shared_bw_group;
		cmd.fw.max_burst = common->max_burst;
	}

	if (child->valid) {
		for (pred_id = 0; pred_id < QOS_MAX_PREDECESSORS; ++pred_id)
			cmd.fw.preds[pred_id] = child->preds[pred_id];
	}

	if (parent->first > parent->last) {
		parent->first = 0;
		parent->last = 0;
	}

	if (QOS_BITS_IS_SET(common->valid, TSCD_NODE_CONF_SUSPEND_RESUME))
		cmd.fw.suspend = common->suspend;

	if (QOS_BITS_IS_SET(parent->valid, TSCD_NODE_CONF_FIRST_CHILD))
		cmd.fw.first_child = parent->first;

	if (QOS_BITS_IS_SET(parent->valid, TSCD_NODE_CONF_LAST_CHILD))
		cmd.fw.last_child = parent->last;

	if (QOS_BITS_IS_SET(parent->valid, TSCD_NODE_CONF_FIRST_WRR_NODE))
		cmd.fw.first_wrr_node = parent->first_wrr;


	add_suspend_port(qdev, get_port(qdev->nodes, phy));
	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));

	if (QOS_BITS_IS_SET(common->valid, TSCD_NODE_CONF_BW_LIMIT)) {
		node = get_node_from_phy(qdev->nodes, phy);
		if (unlikely(!node)) {
			QOS_LOG_ERR("get_node_from_phy(%u) returned NULL\n", phy);
			return;
		}
		node->actual_bandwidth_limit =
			tscd_get_normalized_bw_limit(qdev, cmd.fw.bw_limit);
	}
}

void create_set_sched_cmd(struct pp_qos_dev *qdev,
			  const struct pp_qos_sched_conf *conf,
			  const struct qos_node *node,
			  u64 modified)
{
	struct fw_set_common common;
	struct fw_set_parent parent;
	struct fw_set_child child;
	u32 phy = get_phy_from_node(qdev->nodes, node);

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	/* Only external parameters may change here */
	memset(&common, 0, sizeof(struct fw_set_common));
	memset(&parent, 0, sizeof(struct fw_set_parent));
	memset(&child, 0, sizeof(struct fw_set_child));

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_NODE_TYPE)) {
		create_add_sched_cmd(qdev, conf, phy);
	} else {
		set_common(qdev, &conf->common_prop, &common, modified, phy);
		set_parent(&conf->sched_parent_prop, &node->parent_prop,
			   &parent, modified);
		set_child(&conf->sched_child_prop, node, &child, modified);
		_create_set_sched_cmd(qdev, phy, modified,
				      &common, &parent, &child);
	}

	if (!QOS_BITS_IS_SET(modified, QOS_MODIFIED_NODE_TYPE)) {
		QOS_LOG_DEBUG("cmd %u: CMD_TYPE_SET_SCHED %u\n",
			      qdev->drvcmds.cmd_id, phy);
	}
}

static void create_add_queue_cmd(struct pp_qos_dev *qdev,
				 const struct pp_qos_queue_conf *conf,
				 u32 phy, u32 rlm,
				 bool is_aliased, bool is_orphaned)
{
	struct cmd_add_queue   cmd;
	struct qos_node *node;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	node = get_node_from_phy(qdev->nodes, phy);
	if (unlikely(!node)) {
		QOS_LOG_ERR("get_node_from_phy(%u) returned NULL\n", phy);
		return;
	}

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_ADD_QUEUE,
		 sizeof(cmd), 0, NULL, 0, 0);

	cmd.fw.base.type = UC_QOS_CMD_ADD_QUEUE;
	cmd.fw.phy = phy;
	if (is_orphaned)
		cmd.fw.port = 0;
	else
		cmd.fw.port = get_port(qdev->nodes, phy);

	cmd.fw.rlm = rlm;
	cmd.fw.bw_limit = conf->common_prop.bandwidth_limit;
	cmd.fw.shared_bwl_entry = conf->common_prop.shared_bw_group;
	cmd.fw.max_burst = conf->common_prop.max_burst;

	if (!is_orphaned) {
		fill_preds(qdev->nodes, phy, cmd.fw.preds,
			   QOS_MAX_PREDECESSORS);
	}

	cmd.fw.params.active = !conf->blocked;

	if (!conf->wred_enable) {
		QOS_BITS_SET(cmd.fw.params.disable_flags,
			     WRED_FLAGS_DISABLE_WRED);
	}

	if (conf->wred_fixed_drop_prob_enable) {
		QOS_BITS_SET(cmd.fw.params.disable_flags,
			     WRED_FLAGS_FIXED_PROBABILITY);
	}

	cmd.fw.params.fixed_green_drop_p = conf->wred_fixed_drop_prob_green;
	cmd.fw.params.fixed_yellow_drop_p = conf->wred_fixed_drop_prob_yellow;
	cmd.fw.params.min_avg_yellow = conf->wred_min_avg_yellow;
	cmd.fw.params.max_avg_yellow = conf->wred_max_avg_yellow;
	cmd.fw.params.slope_yellow = conf->wred_slope_yellow;
	cmd.fw.params.min_avg_green = conf->wred_min_avg_green;
	cmd.fw.params.max_avg_green = conf->wred_max_avg_green;
	cmd.fw.params.slope_green = conf->wred_slope_green;
	cmd.fw.params.min_guaranteed = conf->wred_min_guaranteed;
	cmd.fw.params.max_allowed = conf->blocked ? 0 : conf->wred_max_allowed;
	cmd.fw.params.is_alias = is_aliased;
	cmd.fw.params.is_fast_q = node->data.queue.is_fast_q;
	cmd.fw.params.codel_en = conf->codel_en;

	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_ADD_QUEUE %u\n",
		      qdev->drvcmds.cmd_id, phy);

	if (!is_orphaned)
		add_suspend_port(qdev, cmd.fw.port);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));

	node->actual_bandwidth_limit =
			tscd_get_normalized_bw_limit(qdev, cmd.fw.bw_limit);
}

void create_add_sync_queue_cmd(struct pp_qos_dev *qdev,
			       const struct pp_qos_queue_conf *conf,
			       u32 phy, u32 rlm, u64 modified)
{
	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	create_add_queue_cmd(qdev, conf, phy, rlm, false, true);
	suspend_node(qdev, phy, 1);
}

static void _create_set_queue_cmd(struct pp_qos_dev *qdev, u32 phy,
				  u32 rlm, bool is_aliased, u64 modified,
				  struct fw_set_common *common,
				  struct fw_set_child *child,
				  struct fw_set_queue *queue,
				  bool is_orphaned)
{
	struct cmd_set_queue cmd;
	struct qos_node *node;
	u32 pred_id;

	if ((common->valid | child->valid | queue->valid) == 0) {
		QOS_LOG_DEBUG("IGNORING EMPTY CMD_TYPE_SET_QUEUE\n");
		return;
	}

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_SET_QUEUE,
		 sizeof(cmd), 0, NULL, 0, 0);

	cmd.fw.base.type = UC_QOS_CMD_SET_QUEUE;
	cmd.fw.phy = phy;
	cmd.fw.rlm = rlm;
	cmd.fw.params.is_alias = is_aliased;
	cmd.fw.valid = common->valid | child->valid;

	if (QOS_BITS_IS_SET(common->valid, TSCD_NODE_CONF_SUSPEND_RESUME))
		cmd.fw.suspend = common->suspend;

	if (common->valid) {
		cmd.fw.bw_limit = common->bw_limit;
		cmd.fw.shared_bwl_entry = common->shared_bw_group;
		cmd.fw.max_burst = common->max_burst;
	}

	if (child->valid) {
		cmd.fw.node_weight = child->bw_share;
		for (pred_id = 0; pred_id < QOS_MAX_PREDECESSORS; ++pred_id)
			cmd.fw.preds[pred_id] = child->preds[pred_id];
	}

	if (!is_orphaned)
		cmd.fw.port = get_port(qdev->nodes, phy);

	if (queue->valid) {
		cmd.fw.queue_valid = queue->valid;
		cmd.fw.params.active = queue->active;
		cmd.fw.params.disable_flags = queue->disable;
		cmd.fw.params.fixed_green_drop_p =
			queue->wred_fixed_drop_prob_green;
		cmd.fw.params.fixed_yellow_drop_p =
			queue->wred_fixed_drop_prob_yellow;
		cmd.fw.params.min_avg_yellow = queue->wred_min_avg_yellow;
		cmd.fw.params.max_avg_yellow = queue->wred_max_avg_yellow;
		cmd.fw.params.slope_yellow = queue->wred_slope_yellow;
		cmd.fw.params.min_avg_green = queue->wred_min_avg_green;
		cmd.fw.params.max_avg_green = queue->wred_max_avg_green;
		cmd.fw.params.slope_green = queue->wred_slope_green;
		cmd.fw.params.min_guaranteed = queue->wred_min_guaranteed;
		cmd.fw.params.max_allowed = queue->wred_max_allowed;
		cmd.fw.params.codel_en = queue->codel_en;
		cmd.fw.params.is_fast_q = queue->is_fast_q;
	}

	if (!is_orphaned)
		add_suspend_port(qdev, get_port(qdev->nodes, phy));

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));

	if (QOS_BITS_IS_SET(common->valid, TSCD_NODE_CONF_BW_LIMIT)) {
		node = get_node_from_phy(qdev->nodes, phy);
		if (unlikely(!node)) {
			QOS_LOG_ERR("get_node_from_phy(%u) returned NULL\n", phy);
			return;
		}
		node->actual_bandwidth_limit =
			tscd_get_normalized_bw_limit(qdev, cmd.fw.bw_limit);
	}
}

void create_set_queue_cmd(struct pp_qos_dev *qdev,
			  const struct pp_qos_queue_conf *conf,
			  u32 phy, const struct qos_node *node,
			  u64 modified, bool is_orphaned)
{
	struct fw_set_common common;
	struct fw_set_child child;
	struct fw_set_queue queue;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	/* Only external parameters may change here */
	memset(&common, 0, sizeof(struct fw_set_common));
	memset(&child, 0, sizeof(struct fw_set_child));
	memset(&queue, 0, sizeof(struct fw_set_queue));

	if (QOS_BITS_IS_SET(modified, QOS_MODIFIED_NODE_TYPE)) {
		create_add_queue_cmd(qdev, conf, phy, node->data.queue.rlm,
				     node->data.queue.is_alias, is_orphaned);
	} else {
		set_common(qdev, &conf->common_prop, &common, modified, phy);
		set_child(&conf->queue_child_prop, node, &child, modified);
		set_queue(conf, &queue, modified);
		_create_set_queue_cmd(qdev, phy, node->data.queue.rlm,
				      node->data.queue.is_alias, modified,
				      &common, &child, &queue, is_orphaned);
	}

	if (!QOS_BITS_IS_SET(modified, QOS_MODIFIED_NODE_TYPE)) {
		QOS_LOG_DEBUG("cmd %u: CMD_TYPE_SET_QUEUE %u\n",
			      qdev->drvcmds.cmd_id, phy);
	}
}

void suspend_node(struct pp_qos_dev *qdev, u32 phy, u32 suspend)
{
	struct qos_node *node;
	struct fw_set_common common;
	struct fw_set_parent parent;
	struct fw_set_child child;
	struct fw_set_port port;
	struct fw_set_queue queue;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	memset(&common, 0, sizeof(struct fw_set_common));
	memset(&parent, 0, sizeof(struct fw_set_parent));
	memset(&child, 0, sizeof(struct fw_set_child));
	memset(&port, 0, sizeof(struct fw_set_port));
	memset(&queue, 0, sizeof(struct fw_set_queue));

	common.valid = TSCD_NODE_CONF_SUSPEND_RESUME;
	common.suspend = suspend;

	node = get_node_from_phy(qdev->nodes, phy);
	if (unlikely(!node)) {
		QOS_LOG_ERR("get_node_from_phy(%u) returned NULL\n", phy);
		return;
	}

	switch (node->type) {
	case TYPE_PORT:
		_create_set_port_cmd(qdev, phy, 0,
				     &common, &parent, &port);
	break;

	case TYPE_SCHED:
		_create_set_sched_cmd(qdev, phy, 0,
				      &common, &parent, &child);
	break;

	case TYPE_QUEUE:
		_create_set_queue_cmd(qdev, phy, node->data.queue.rlm,
				      node->data.queue.is_alias, 0,
				      &common, &child, &queue,
				      node_new_syncq(node));
	break;

	default:
		QOS_LOG_ERR("Unnode type for phy %u\n", phy);
	}
}

void create_flush_queue_cmd(struct pp_qos_dev *qdev, u32 rlm)
{
	struct cmd_flush_queue cmd;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_FLUSH_QUEUE,
		 sizeof(cmd), 0, NULL, 0, 0);
	cmd.fw.base.type = UC_QOS_CMD_FLUSH_QUEUE;
	cmd.fw.rlm = rlm;

	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_FLUSH_QUEUE %u\n",
		      qdev->drvcmds.cmd_id, rlm);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

static void create_update_port_tree_cmd(struct pp_qos_dev *qdev, u32 phy)
{
	struct cmd_update_port_tree cmd;
	u32 bitmap_word[QOS_MAX_NUM_QUEUES >> 5];
	u32 num_active_queues;
	u32 queue_idx;
	u32 word_idx;
	u32 bit_in_word;
	u16 *queues;
	s32 ret;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_UPDATE_PORT_TREE,
		 sizeof(cmd), 0, NULL, 0, 0);
	cmd.fw.base.type = UC_QOS_CMD_UPDATE_PORT_TREE;
	cmd.fw.phy = phy;
	cmd.fw.max_allowed_addr = (u32)qdev->hwmem.max_allow_ddr_phys;

	queues = qdev->rlms_container;
	memset(queues, 0, sizeof(u16) * QOS_MAX_NUM_QUEUES);
	memset(bitmap_word, 0, sizeof(bitmap_word));

	get_active_port_rlms(qdev, phy, queues, QOS_MAX_NUM_QUEUES,
			     &num_active_queues);

	ret = store_queue_list_max_allowed(qdev, queues, num_active_queues);
	if (unlikely(ret)) {
		QOS_LOG_ERR("Failed setting queues max allowed buffer\n");
		return;
	}

	for (queue_idx = 0; queue_idx < num_active_queues; queue_idx++) {
		word_idx = *queues >> 5;
		bit_in_word = *queues - (32 * word_idx);
		bitmap_word[word_idx] |= BIT_ULL(bit_in_word);
		queues++;
	}

	for (word_idx = 0; word_idx < QOS_MAX_NUM_QUEUES >> 5; word_idx++)
		cmd.fw.bitmap[word_idx] = bitmap_word[word_idx];

	update_wsp_info(qdev);

	/* Update BWL buffer before tree update */
	update_bwl_buffer(qdev, phy);
	qdev->init_params.fwcom.clear_bwl_db = 1;

	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_UPDATE_PORT_TREE %u\n",
		      qdev->drvcmds.cmd_id, phy);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

static void create_suspend_port_tree_cmd(struct pp_qos_dev *qdev, u32 phy)
{
	struct cmd_suspend_port_tree cmd;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_SUSPEND_PORT_TREE,
		 sizeof(cmd), 0, NULL, 0, 0);
	cmd.fw.base.type = UC_QOS_CMD_SUSPEND_PORT_TREE;
	cmd.fw.phy = phy;

	QOS_LOG_DEBUG("cmd %u: UC_QOS_CMD_SUSPEND_PORT_TREE %u\n",
		      qdev->drvcmds.cmd_id, phy);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

void create_set_wsp_helper_cmd(struct pp_qos_dev *qdev,
			       struct wsp_helper_cfg *cfg)
{
	struct cmd_set_wsp_helper cmd;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_SET_WSP_HELPER,
		 sizeof(cmd), 0, NULL, 0, 0);
	cmd.fw.base.type = UC_QOS_CMD_WSP_HELPER_SET;
	memcpy(&cmd.fw.cfg, cfg, sizeof(*cfg));

	QOS_LOG_DEBUG("cmd %u: UC_QOS_CMD_WSP_HELPER_SET\n",
		      qdev->drvcmds.cmd_id);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
	update_wsp_info(qdev);
}

static s32 __check_rsp_size(struct pp_qos_dev *qdev, size_t rsp_size)
{
	if (rsp_size > qdev->init_params.fwcom.rspbuf_sz) {
		pr_err("MSG wasn't sent: rsp size (%zu) larger then buffer(%zu)",
		       rsp_size, qdev->init_params.fwcom.rspbuf_sz);
		return -EINVAL;
	}

	return 0;
}
#ifndef CONFIG_SOC_LGM
void create_get_wsp_helper_stats_cmd(struct pp_qos_dev *qdev,
				     bool reset, struct wsp_stats_t *stats)
{
	struct cmd_get_wsp_helper_stats cmd;
	size_t rsp_size = sizeof(struct wsp_stats_t);

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	if (__check_rsp_size(qdev, rsp_size))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_GET_WSP_HELPER_STATS, sizeof(cmd),
		 CMD_FLAGS_POST_PROCESS_DCCM, (void *)stats, 0,
		 rsp_size);
	cmd.fw.base.type = UC_QOS_CMD_WSP_HELPER_STATS_GET;
	cmd.fw.reset = reset;

	QOS_LOG_DEBUG("cmd %u: UC_QOS_CMD_WSP_HELPER_STATS_GET\n",
		      qdev->drvcmds.cmd_id);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}
#endif

void update_parent(struct pp_qos_dev *qdev, u32 phy)
{
	struct fw_set_common common;
	struct fw_set_parent parent;
	struct fw_set_child child;
	struct fw_set_port port;
	const struct qos_node *node;
	const struct parent_node_properties *parent_prop;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	node = get_const_node_from_phy(qdev->nodes, phy);
	if (unlikely(!node)) {
		QOS_LOG_ERR("get_const_node_from_phy(%u) returned NULL\n",
			    phy);
		return;
	}
	parent_prop = &node->parent_prop;

	memset(&common, 0, sizeof(struct fw_set_common));
	memset(&parent, 0, sizeof(struct fw_set_parent));
	memset(&child, 0, sizeof(struct fw_set_child));
	memset(&port, 0, sizeof(struct fw_set_port));

	update_arbitration(&parent.valid, parent_prop, &parent);

	common.valid = 0;
	if (node->type == TYPE_PORT)
		_create_set_port_cmd(qdev, phy, 0, &common, &parent, &port);
	else if (node->type == TYPE_SCHED)
		_create_set_sched_cmd(qdev, phy, 0, &common, &parent, &child);
}

void update_preds(struct pp_qos_dev *qdev, u32 phy, bool queue_port_changed)
{
	struct fw_set_common common;
	struct fw_set_child child;
	struct fw_set_parent parent;
	struct fw_set_queue queue;
	const struct qos_node *node;

	/* Only external parameters may change here */
	memset(&common, 0, sizeof(struct fw_set_common));
	memset(&child, 0, sizeof(struct fw_set_child));
	memset(&parent, 0, sizeof(struct fw_set_parent));
	memset(&queue, 0, sizeof(struct fw_set_queue));

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	node = get_const_node_from_phy(qdev->nodes, phy);
	if (unlikely(!node)) {
		QOS_LOG_ERR("get_const_node_from_phy(%u) returned NULL\n",
			    phy);
		return;
	}

	child.valid = TSCD_NODE_CONF_PREDECESSOR_0 |
		TSCD_NODE_CONF_PREDECESSOR_1 |
		TSCD_NODE_CONF_PREDECESSOR_2 |
		TSCD_NODE_CONF_PREDECESSOR_3 |
		TSCD_NODE_CONF_PREDECESSOR_4 |
		TSCD_NODE_CONF_PREDECESSOR_5;

	fill_preds(qdev->nodes, phy, child.preds, QOS_MAX_PREDECESSORS);

	switch (node->type) {
	case TYPE_SCHED:
		_create_set_sched_cmd(qdev, phy, 0,
				      &common, &parent, &child);
		break;
	case TYPE_QUEUE:
		if (queue_port_changed) {
			common.valid |= TSCD_NODE_CONF_SET_PORT_TO_QUEUE;
			common.port_phy = node->data.queue.port_phy;
		}

		_create_set_queue_cmd(qdev, phy, node->data.queue.rlm,
				      node->data.queue.is_alias, 0,
				      &common, &child, &queue, false);
		break;
	default:
		QOS_LOG_ERR("illegal node type %d\n", node->type);
		return;
	}
}

void create_get_port_stats_cmd(struct pp_qos_dev *qdev, u32 phy,
			       struct port_stats_s *pstat, bool reset_stats)
{
	struct cmd_get_port_stats cmd;
	size_t rsp_size = sizeof(struct port_stats_s);

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	if (__check_rsp_size(qdev, rsp_size))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_GET_PORT_STATS,
		 sizeof(cmd), CMD_FLAGS_POST_PROCESS_DCCM, (void *)pstat,
		 0, rsp_size);
	cmd.fw.base.type = UC_QOS_CMD_GET_PORT_STATS;
	cmd.fw.phy = phy;
	if (reset_stats)
		cmd.fw.reset = PORT_STATS_CLEAR_ALL;
	else
		cmd.fw.reset = PORT_STATS_CLEAR_NONE;

	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_GET_PORT_STATS %u\n",
		      qdev->drvcmds.cmd_id, phy);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

void create_get_hist_stats_cmd(struct pp_qos_dev *qdev, u8 sf_id,
			       struct hist_stats_s *hstat, bool reset)
{
	struct cmd_get_hist_stats cmd;
	size_t rsp_size = sizeof(struct hist_stats_s);

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	if (__check_rsp_size(qdev, rsp_size))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_GET_HIST_STATS,
		 sizeof(cmd), CMD_FLAGS_POST_PROCESS_DCCM, (void *)hstat,
		 0, rsp_size);
	cmd.fw.base.type = UC_QOS_CMD_GET_HIST_STATS;
	cmd.fw.sf = sf_id;
	cmd.fw.reset = reset;

	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_GET_HIST_STATS %u\n",
		      qdev->drvcmds.cmd_id, sf_id);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

void create_get_queue_stats_cmd(struct pp_qos_dev *qdev,
				u32 phy, u32 rlm,
				struct queue_stats_s *qstat,
				bool reset)
{
	struct cmd_get_queue_stats cmd;
	size_t rsp_size = sizeof(struct queue_stats_s);

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	if (__check_rsp_size(qdev, rsp_size))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_GET_QUEUE_STATS,
		 sizeof(cmd), CMD_FLAGS_POST_PROCESS_DCCM, (void *)qstat,
		 0, rsp_size);
	cmd.fw.base.type = UC_QOS_CMD_GET_QUEUE_STATS;
	cmd.fw.phy = phy;
	cmd.fw.rlm = rlm;

	if (reset) {
		cmd.fw.reset = QUEUE_STATS_CLEAR_Q_AVG_SIZE_BYTES |
			QUEUE_STATS_CLEAR_DROP_P_YELLOW |
			QUEUE_STATS_CLEAR_DROP_P_GREEN |
			QUEUE_STATS_CLEAR_TOTAL_BYTES_ADDED |
			QUEUE_STATS_CLEAR_TOTAL_ACCEPTS |
			QUEUE_STATS_CLEAR_TOTAL_DROPS |
			QUEUE_STATS_CLEAR_TOTAL_DROPPED_BYTES |
			QUEUE_STATS_CLEAR_TOTAL_RED_DROPS;
	} else {
		cmd.fw.reset = QUEUE_STATS_CLEAR_NONE;
	}

	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_GET_QUEUE_STATS %u\n",
		      qdev->drvcmds.cmd_id, phy);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

void create_get_qm_stats_cmd(struct pp_qos_dev *qdev, u32 rlm,
			     struct qm_info *qstat)
{
	struct cmd_get_qm_stats cmd;
	size_t rsp_size = sizeof(struct qm_info);

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	if (__check_rsp_size(qdev, rsp_size))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_GET_QM_STATS,
		 sizeof(cmd), CMD_FLAGS_POST_PROCESS_DCCM, (void *)qstat,
		 0, rsp_size);
	cmd.fw.base.type = UC_QOS_CMD_GET_ACTIVE_QUEUES_STATS;
	cmd.fw.first_rlm = rlm;
	cmd.fw.last_rlm = rlm + NUM_Q_ON_QUERY - 1;

	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_GET_QM_STATS\n",
		      qdev->drvcmds.cmd_id);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

void create_aqm_sf_set_cmd(struct pp_qos_dev *qdev, u8 sf_id, u8 enable,
			   struct pp_qos_aqm_lld_sf_config *sf_cfg,
			   u32 *queue_id)
{
	struct cmd_set_aqm_sf cmd;
	u32 queue_idx;
	u32 hist_idx;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_SET_AQM_SF,
		 sizeof(cmd), 0, NULL, 0, 0);
	cmd.fw.base.type = UC_QOS_CMD_SET_AQM_SF;
	cmd.fw.aqm_context = sf_id;
	cmd.fw.enable = enable;
	cmd.fw.latency_target_ms = sf_cfg->cfg.aqm_cfg.latency_target_ms;
	cmd.fw.peak_rate = sf_cfg->cfg.aqm_cfg.peak_rate;
	cmd.fw.msr = sf_cfg->cfg.aqm_cfg.msr;
	cmd.fw.buffer_size = sf_cfg->buffer_size;
	cmd.fw.num_rlms = sf_cfg->num_queues;
	cmd.fw.coupled_sf = sf_cfg->coupled_sf;
	cmd.fw.llsf = sf_cfg->llsf;
	cmd.fw.coupling_factor = sf_cfg->coupling_factor;
	cmd.fw.amsr = sf_cfg->amsr;
	cmd.fw.msr_l = sf_cfg->msr_l;
	cmd.fw.weight = sf_cfg->weight;
	cmd.fw.num_hist_bins = sf_cfg->num_hist_bins;

	for (queue_idx = 0; queue_idx < sf_cfg->num_queues; queue_idx++)
		cmd.fw.rlms[queue_idx] = queue_id[queue_idx];

	for (hist_idx = 0; hist_idx < sf_cfg->num_hist_bins; hist_idx++)
		cmd.fw.bin_edges[hist_idx] = sf_cfg->bin_edges[hist_idx];

	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_SET_AQM_SF %u\n",
		      qdev->drvcmds.cmd_id, sf_id);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

void create_codel_cfg_set_cmd(struct pp_qos_dev *qdev,
			      struct pp_qos_codel_cfg *cfg)
{
	struct cmd_set_codel_cfg cmd;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_SET_CODEL_CFG,
		 sizeof(cmd), 0, NULL, 0, 0);
	cmd.fw.base.type = UC_QOS_CMD_SET_CODEL_CFG;
	cmd.fw.target_delay_msec = cfg->target_delay_msec;
	cmd.fw.interval_time_msec = cfg->interval_time_msec;

	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_SET_CODEL_CFG\n",
		      qdev->drvcmds.cmd_id);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

void create_get_codel_stats_cmd(struct pp_qos_dev *qdev, u32 rlm,
				struct codel_qstats_s *qstat, bool reset_stats)
{
	struct cmd_get_codel_stat cmd;
	size_t rsp_size = sizeof(struct codel_qstats_s);

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	if (__check_rsp_size(qdev, rsp_size))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_GET_CODEL_STATS,
		 sizeof(cmd), CMD_FLAGS_POST_PROCESS_DCCM, (void *)qstat,
		 0, rsp_size);

	cmd.fw.base.type = UC_QOS_CMD_GET_CODEL_QUEUE_STATS;
	cmd.fw.rlm = rlm;
	cmd.fw.reset = reset_stats;

	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_GET_CODEL_STATS rlm %u\n",
		      qdev->drvcmds.cmd_id, rlm);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

void create_get_node_info_cmd(struct pp_qos_dev *qdev, u32 phy,
			      struct hw_node_info_s *info)
{
	struct cmd_get_node_info cmd;
	size_t rsp_size = sizeof(struct hw_node_info_s);

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	if (__check_rsp_size(qdev, rsp_size))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_GET_NODE_INFO,
		 sizeof(cmd), CMD_FLAGS_POST_PROCESS_DCCM, (void *)info,
		 0, rsp_size);
	cmd.fw.base.type = UC_QOS_CMD_GET_NODE_INFO;
	cmd.fw.phy = phy;

	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_GET_NODE_INFO %u\n",
		      qdev->drvcmds.cmd_id, phy);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

void create_get_table_entry_cmd(struct pp_qos_dev *qdev, u32 phy,u32 table_type,
				union host_read_entry_t *read_entry,
				u32 entry_size)
{
	struct cmd_read_table_entry cmd;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_READ_TABLE_ENTRY,
		 sizeof(cmd), CMD_FLAGS_POST_PROCESS_DCCM, (void*)read_entry,
		 0, entry_size);
	cmd.fw.base.type = UC_QOS_CMD_DEBUG_READ_NODE;
	cmd.fw.phy = phy;
	cmd.fw.table_type = table_type;

	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_READ_TABLE_ENTRY %u from phy %u\n",
		      qdev->drvcmds.cmd_id, table_type, phy);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

void create_get_sys_info_cmd(struct pp_qos_dev *qdev,
			     struct system_stats_s *info,
			     bool dump_entries)
{
	struct cmd_get_system_info cmd;
	size_t rsp_size = sizeof(struct system_stats_s);

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	if (__check_rsp_size(qdev, rsp_size))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_GET_SYSTEM_INFO,
		 sizeof(cmd), CMD_FLAGS_POST_PROCESS_DCCM, (void *)info,
		 0, rsp_size);
	cmd.fw.base.type = UC_QOS_CMD_GET_SYSTEM_STATS;
	cmd.fw.dump = dump_entries;

	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_GET_SYSTEM_INFO\n",
		      qdev->drvcmds.cmd_id);
	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

void create_mod_log_bmap_set_cmd(struct pp_qos_dev *qdev, u32 bmap)
{
	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;
	PP_REG_WR32((ulong)&qdev->init_params.dccm_db->mod_reg_log_en, bmap);
}

void create_mod_log_bmap_get_cmd(struct pp_qos_dev *qdev, u32 *bmap)
{
	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	*bmap = PP_REG_RD32((ulong)&qdev->init_params.dccm_db->mod_reg_log_en);
}

void create_add_shared_group_cmd(struct pp_qos_dev *qdev, u32 id,
				 u32 max_burst)
{
	struct cmd_add_shared_group cmd;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_ADD_SHARED_GROUP,
		 sizeof(cmd), 0, NULL, 0, 0);
	cmd.fw.base.type = UC_QOS_CMD_ADD_SHARED_BW_LIMIT_GROUP;
	cmd.fw.id = id;
	cmd.fw.max_burst = max_burst;

	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_ADD_SHARED_GROUP %u\n",
		      qdev->drvcmds.cmd_id, id);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

void create_set_shared_group_cmd(struct pp_qos_dev *qdev, u32 id,
				 u32 max_burst)
{
	struct cmd_add_shared_group cmd;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_SET_SHARED_GROUP,
		 sizeof(cmd), 0, NULL, 0, 0);
	cmd.fw.base.type = UC_QOS_CMD_SET_SHARED_BW_LIMIT_GROUP;
	cmd.fw.id = id;
	cmd.fw.max_burst = max_burst;

	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_SET_SHARED_GROUP %u\n",
		      qdev->drvcmds.cmd_id, id);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

void create_remove_shared_group_cmd(struct pp_qos_dev *qdev, u32 id)
{
	struct cmd_remove_shared_group cmd;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_REM_SHARED_GROUP,
		 sizeof(cmd), 0, NULL, 0, 0);
	cmd.fw.base.type = UC_QOS_CMD_REM_SHARED_BW_LIMIT_GROUP;
	cmd.fw.id = id;

	QOS_LOG_DEBUG("cmd %u: CMD_TYPE_REM_SHARED_GROUP %u\n",
		      qdev->drvcmds.cmd_id, id);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

void create_push_desc_cmd(struct pp_qos_dev *qdev, u32 queue, u32 size,
			  u32 pool, u32 policy, u32 color, u32 gpid,
			  u32 data_off, u32 addr)
{
	struct cmd_push_desc cmd;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_PUSH_DESC,
		 sizeof(cmd), 0, NULL, 0, 0);
	cmd.fw.base.type = UC_QOS_CMD_DEBUG_PUSH_DESC;
	cmd.fw.rlm = queue;
	cmd.fw.size = size;
	cmd.fw.pool = pool;
	cmd.fw.policy = policy;
	cmd.fw.color = color;
	cmd.fw.gpid = gpid;
	cmd.fw.data_offset = data_off;
	cmd.fw.addr = addr;

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

void create_pop_desc_cmd(struct pp_qos_dev *qdev, u32 queue, u32 addr)
{
	struct cmd_pop_desc cmd;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_POP_DESC,
		 sizeof(cmd), 0, NULL, 0, 0);
	cmd.fw.base.type = UC_QOS_CMD_DEBUG_POP_DESC;
	cmd.fw.rlm = queue;
	cmd.fw.addr = addr;

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

void create_mcdma_copy_cmd(struct pp_qos_dev *qdev, u32 src, u32 dst, u32 sz)
{
	struct cmd_mcdma_copy cmd;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_MCDMA_COPY,
		 sizeof(cmd), 0, NULL, 0, 0);
	cmd.fw.base.type = UC_QOS_CMD_DEBUG_MCDMA_COPY;
	cmd.fw.src_addr = src;
	cmd.fw.dst_addr = dst;
	cmd.fw.size = sz;

	qdev->init_params.fwcom.wait_on_complete = 1;
	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

void create_aqm_q_to_ctx_cmd(struct pp_qos_dev *qdev, u32 op, u32 queue, u32 ctx)
{
	struct cmd_aqm_q_to_ctx cmd;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_AQM_Q_TO_CTX,
		 sizeof(cmd), 0, NULL, 0, 0);
	cmd.fw.base.type = UC_QOS_CMD_AQM_Q_TO_CTX;
	cmd.fw.op = op;
	cmd.fw.qid = queue;
	cmd.fw.ctx = ctx;

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

void create_set_pci_addr_cmd(struct pp_qos_dev *qdev, struct pp_qos_pci_addr *pci_addr)
{
	struct cmd_set_pci_addr cmd;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_SET_PCI_ADDR,
		 sizeof(cmd), 0, NULL, 0, 0);
	cmd.fw.base.type = UC_QOS_CMD_SET_PCI_ADDR;
	cmd.fw.pci_addr.msrtoken_addr = pci_addr->msrtoken_addr;

	QOS_LOG_DEBUG("cmd %u: UC_QOS_CMD_SET_PCI_ADDR\n",
		      qdev->drvcmds.cmd_id);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

void create_set_aqm_engine_cmd(struct pp_qos_dev *qdev, u8 aqm_engine)
{
	struct cmd_set_aqm_engine cmd;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	memset(&cmd, 0, sizeof(cmd));
	cmd_init(qdev, &cmd.base, CMD_TYPE_SET_AQM_ENGINE,
		 sizeof(cmd), 0, NULL, 0, 0);
	cmd.fw.base.type = UC_QOS_CMD_SET_AQM_ENGINE;
	cmd.fw.aqm_engine = (u32)aqm_engine;

	QOS_LOG_DEBUG("cmd %u: UC_QOS_CMD_SET_AQM_ENGINE\n",
		      qdev->drvcmds.cmd_id);

	cmd_queue_put(qdev->drvcmds.cmdq, &cmd, sizeof(cmd));
}

/******************************************************************************/
/*                         FW write functions                                 */
/******************************************************************************/
void add_suspend_port(struct pp_qos_dev *qdev, u32 port_id)
{
	struct fw_internal *internals;
	struct qos_node *node;
	u32 i;

	node = get_node_from_phy(qdev->nodes, port_id);
	if (unlikely(!node)) {
		QOS_LOG_ERR("get_node_from_phy(%u) returned NULL\n", port_id);
		return;
	}

	QOS_ASSERT(node_port(node), "Node %u is not a port\n", port_id);
	internals = qdev->fwbuf;

	for (i = 0; i < internals->num_suspend_ports; ++i) {
		if (internals->suspend_ports[i] == port_id)
			return;
	}

	QOS_ASSERT(internals->num_suspend_ports < qdev->init_params.max_ports,
		   "Suspend ports buffer is full\n");
	internals->suspend_ports[internals->num_suspend_ports] = port_id;
	++(internals->num_suspend_ports);

	create_suspend_port_tree_cmd(qdev, port_id);
}

s32 init_fwdata_internals(struct pp_qos_dev *qdev)
{
	QOS_LOG_DEBUG("\n");

	qdev->fwbuf = kzalloc(sizeof(struct fw_internal), GFP_KERNEL);
	if (qdev->fwbuf) {
		memset(qdev->fwbuf, 0, sizeof(struct fw_internal));
		return 0;
	}
	return -EBUSY;
}

void clean_fwdata_internals(struct pp_qos_dev *qdev)
{
	if (qdev) {
		kfree(qdev->fwbuf);
		qdev->fwbuf = NULL;
	}
}

static ulong __fw_cmd_wr(ulong buf, void *cmd, u32 cmd_sz, u32 flags)
{
	struct uc_qos_cmd_base *fw_base = (struct uc_qos_cmd_base *)cmd;

	fw_base->flags = flags;
	fw_base->num_params = (cmd_sz - sizeof(*fw_base)) / sizeof(u32);

	memcpy_toio((void *)buf, cmd, cmd_sz);

	return (ulong)((char *)buf + cmd_sz);
}

/* Signal firmware to read and executes commands from cmd
 * buffer.
 * This is done by using the mailbox bundle.
 * Refer to driver's design document for further information
 * Since this is the only signal that is sent from driver to firmware
 * the value of DRV_SIGNAL is insignificant
 */
#define DRV_SIGNAL (2U)
#define QOS_IRC1_BASE 0x220
#define QOS_MBX_IRC_BASE 0x260
void signal_uc(struct pp_qos_dev *qdev)
{
	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	/* signal using the SW register in the bundle */
	PP_REG_WR32(qdev->init_params.fwcom.mbx_to_uc + QOS_MBX_IRC_BASE + 0x4,
		    DRV_SIGNAL);
}

#define IRC1_DFS_BIT 12
void signal_uc_clk_chng(struct pp_qos_dev *qdev)
{
	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	/* signal using the SW register in the bundle */
	PP_REG_WR32(qdev->init_params.fwcom.mbx_to_uc + QOS_IRC1_BASE + 0x4,
		    BIT_ULL(IRC1_DFS_BIT));
}

/******************************************************************************/
/*                                Engine                                      */
/******************************************************************************/

/* Commands that are marked with POST_PROCESS reach
 * here for further processing before return to client
 */
static s32 post_process_from_dccm(struct pp_qos_dev *qdev,
				 union driver_cmd *dcmd)
{
	struct qm_info *qminfo_ptr;
	s32 ret = 0;
	u32 num_queues;
	void *addr;
	u32 size;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return -EINVAL;

	if (!dcmd->cmd.response) {
		QOS_LOG_ERR("Response ptr is NULL for %s\n",
			cmd_str[dcmd->cmd.type]);
		ret = -EINVAL;
		goto out;
	}

	addr = (void*)(ulong)qdev->init_params.fwcom.rspbuf;
	size = dcmd->cmd.response_sz;
	switch (dcmd->cmd.type) {
	case CMD_TYPE_GET_QM_STATS:
		qminfo_ptr = (struct qm_info *)addr;
		num_queues = (PP_REG_RD32(&qminfo_ptr->num_queues));
		if (num_queues)
			size = sizeof(qminfo_ptr->qstat[0]) * num_queues;
		else {
			qminfo_ptr = (struct qm_info *)dcmd->cmd.response;
			qminfo_ptr->num_queues = 0;
			size = 0;
		}
		break;
	case CMD_TYPE_GET_QUEUE_STATS:
	case CMD_TYPE_GET_CODEL_STATS:
	case CMD_TYPE_GET_PORT_STATS:
	case CMD_TYPE_GET_SYSTEM_INFO:
	case CMD_TYPE_GET_NODE_INFO:
	case CMD_TYPE_GET_WSP_HELPER_STATS:
	case CMD_TYPE_READ_TABLE_ENTRY:
	case CMD_TYPE_GET_VERSION:
	case CMD_TYPE_GET_HIST_STATS:
		break;
	default:
		QOS_ASSERT(0, "Unexpected cmd %d for post process\n"
			, dcmd->cmd.type);
		ret = -EINVAL;
		goto out;
	}

	if (size)
		memcpy_fromio(dcmd->cmd.response, addr, size);

	if (size)
		memcpy_fromio(dcmd->cmd.response, addr, size);

out:
	return ret;
}

/* Go over all commands on pending queue until cmd id
 * is changed or queue is empty
 * (refer to driver design document to learn more about cmd id).
 * On current implementation it is expected that pending queue contain
 * firmware commands for a single client command, therefore queue should
 * become empty before cmd id is changed.
 *
 * For each command wait until firmware signals
 * completion before continue to next command.
 * Completion status for each command is polled PP_REG_DEFAULT_RETRIES
 * times.
 * If command have not completed after all that polls -
 * function asserts.
 *
 */
s32 check_completion(struct pp_qos_dev *qdev)
{
	u32 expected_flags = UC_CMD_FLAG_UC_DONE | UC_CMD_FLAG_UC_ERROR |
			     UC_CMD_FLAG_UC_FATAL;
	u32 val, idcur, attempts = 0, popped, read_len = 0;
	struct fw_internal *internals;
	union driver_cmd dcmd;
	struct hw_mem *hwmem;
	unsigned long pos;
	size_t len;
	s32 ret;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return -ENODEV;

	hwmem = &qdev->hwmem;
	popped = 0;
	idcur = 0;
	read_len = cmd_queue_peek(qdev->drvcmds.pendq, &dcmd.cmd,
				  sizeof(struct cmd));
	while (read_len != 0) {
		pos = dcmd.stub.cmd.pos;
		len = dcmd.cmd.len;
		pos += 4;

		ret = pp_raw_reg_poll(pos, expected_flags, 0,
				      PP_REG_MAX_RETRIES, &attempts,
				      &val, PP_POLL_NE);
		if (ret) {
			pr_err("FW is not responding, polling offset %#lx for cmd type %s, (received 0x%x)\n",
			       pos, cmd_str[dcmd.cmd.type], val);
#ifdef DUMP_LOG_ON_FAILURE
			qos_dbg_tree_show(qdev, false, NULL);
			print_fw_log(qdev);
#endif
			return -ENODEV;
		}

		if (val & (UC_CMD_FLAG_UC_ERROR | UC_CMD_FLAG_UC_FATAL)) {
			QOS_ASSERT(0,
				   "FW signaled error, polling offset %#lx, cmd type %s\n",
				   pos, cmd_str[dcmd.cmd.type]);
#ifdef DUMP_LOG_ON_FAILURE
			qos_dbg_tree_show(qdev, false, NULL);
			print_fw_log(qdev);
#endif
			return -ENODEV;
		}

		if (val & UC_CMD_FLAG_UC_DONE)
			QOS_LOG_DEBUG("cmd %s (attempts %u)\n",
				      cmd_str[dcmd.cmd.type], attempts);

		ret = cmd_queue_get(qdev->drvcmds.pendq, &dcmd.stub, len);
		QOS_ASSERT(ret == len,
			   "Command queue does not contain a full command\n");
		if (dcmd.cmd.flags & CMD_FLAGS_POST_PROCESS_DCCM) {
			ret = post_process_from_dccm(qdev, &dcmd);
			if (ret)
				return ret;
		}

		++popped;

		read_len = cmd_queue_peek(qdev->drvcmds.pendq,
					  &dcmd.cmd,
					  sizeof(struct cmd));
	}

	internals = qdev->fwbuf;
	QOS_ASSERT(popped == internals->pushed,
		   "Expected to pop %u msgs from pending queue but popped %u\n",
		   internals->pushed, popped);
	QOS_ASSERT(cmd_queue_is_empty(qdev->drvcmds.pendq),
		   "Driver's pending queue is not empty\n");
	internals->pushed = 0;

	if (qdev->init_params.fwcom.clear_bwl_db == 1) {
		qdev->init_params.fwcom.clear_bwl_db = 0;
		memset(hwmem->bwl_ddr_virt, 0, hwmem->bwl_sz);
		pp_cache_writeback(qdev->hwmem.bwl_ddr_virt, qdev->hwmem.bwl_sz);
		memset(hwmem->sbwl_ddr_virt, 0, hwmem->sbwl_sz);
		pp_cache_writeback(qdev->hwmem.sbwl_ddr_virt, qdev->hwmem.sbwl_sz);
	}

	QOS_LOG_DEBUG("(attempts %u) time =%u, count %u cmd_id = %u\n",
		attempts,
		PP_REG_RD32((ulong)&qdev->init_params.dccm_db->cmd_time),
		PP_REG_RD32((ulong)&qdev->init_params.dccm_db->cmds_in_burst),
		PP_REG_RD32((ulong)&qdev->init_params.dccm_db->cmd_count));

	return 0;
}

static void swap_msg(char *msg)
{
	u32 i;
	u32 *cur;

	cur = (u32 *)msg;

	for (i = 0; i < 32; ++i)
		cur[i] = le32_to_cpu(cur[i]);
}

void print_fw_log(struct pp_qos_dev *qdev)
{
	char msg[PPV4_QOS_LOGGER_MSG_SIZE];
	u32  num_messages;
	u32  max_messages;
	u32  msg_idx;
	u32  *msg_word;
	u32  msg_word_idx;
	unsigned long msg_addr;
	unsigned long addr, cur;
	struct hw_mem *hwmem = &qdev->hwmem;

	/* invalidate the cache */
	pp_cache_invalidate(hwmem->fw_logger_ddr_virt, hwmem->fw_logger_sz);

	max_messages =
		(hwmem->fw_logger_sz - PPV4_QOS_LOGGER_HEADROOM_SIZE) /
		PPV4_QOS_LOGGER_MSG_SIZE;
	addr = (unsigned long)(hwmem->fw_logger_ddr_virt);
	num_messages = *(u32 *)(addr + PPV4_QOS_LOGGER_WRITE_IDX_OFFSET);

	/* check if wraparound occur */
	if ((num_messages - hwmem->fw_logger_read_count) > max_messages)
		hwmem->fw_logger_read_count = num_messages - max_messages;

	cur = (unsigned long)(hwmem->fw_logger_ddr_virt) +
		PPV4_QOS_LOGGER_RING_OFFSET;

	pr_info("FW Total messages Printed : %u\n", num_messages);
	pr_info("Host total messages read  : %u\n",
		hwmem->fw_logger_read_count);
	pr_info("Messages to print         : %u\n",
		num_messages - hwmem->fw_logger_read_count);
	pr_info("Log buffer size           : %u\n", max_messages);
	pr_info("Buffer base               : %#lx\n", cur);
	pr_info("==============================================\n");

	while (hwmem->fw_logger_read_count < num_messages) {
		msg_idx = hwmem->fw_logger_read_count % max_messages;
		for (msg_word_idx = 0; msg_word_idx < 32; ++msg_word_idx) {
			msg_word = (u32 *)(msg) + msg_word_idx;
			msg_addr = cur +
				(PPV4_QOS_LOGGER_MSG_SIZE * msg_idx) +
				(msg_word_idx * 4);
			*msg_word = *(u32 *)(msg_addr);
		}

		swap_msg(msg);
		msg[PPV4_QOS_LOGGER_MSG_SIZE - 1] = '\0';
		pr_info("[ARC]: %s\n", msg);
		hwmem->fw_logger_read_count++;
	}
}

s32 qos_get_debug_stats(struct pp_qos_dev *qdev, debug_counters_t *dbg_stats)
{
	if (qdev->init_params.fwcom.dbg_cnt)
		memcpy_fromio(dbg_stats, qdev->init_params.fwcom.dbg_cnt,
				sizeof(debug_counters_t));
	return 0;
}

s32 qos_get_aqm_info(struct pp_qos_dev *qdev, wred_aqm_db_t *aqm_db)
{
	if (qdev->init_params.fwcom.aqm_db)
		memcpy_fromio(aqm_db, qdev->init_params.fwcom.aqm_db,
				sizeof(wred_aqm_db_t));
	return 0;
}

/* Post commands to be pushed at the end of cmdq */
static void pre_enqueue_cmds(struct pp_qos_dev *qdev)
{
	struct fw_internal *intern = qdev->fwbuf;
	u32 i, id;

	if (!intern->num_suspend_ports)
		return;

	/* Enqueue update tree command for all relevant ports */
	for (i = intern->suspend_ports_index; i <
	      intern->num_suspend_ports; ++i) {
		/* In case port was suspended, and it was removed,  */
		/* don't Resume it (Otherwise driver and fw will be */
		/* out of sync) */
		id = get_id_from_phy(qdev->mapping,
				     intern->suspend_ports[i]);
		if (!QOS_ID_VALID(id))
			continue;
		create_update_port_tree_cmd(qdev, intern->suspend_ports[i]);
	}

	/* No pending suspended ports - Reset counters */
	intern->suspend_ports_index = 0;
	intern->num_suspend_ports = 0;
}

/* Take all commands from driver cmd queue, translate them to
 * firmware format and put them on firmware queue.
 * When finish signal firmware.
 */
void enqueue_cmds(struct pp_qos_dev *qdev)
{
	struct fw_internal *intern = qdev->fwbuf;
	struct fw_cmd_multiple_cmds multiple_cmd;
	unsigned long cur, prev, start;
	union driver_cmd dcmd;
	u32 flags, pushed, read_len = 0;
	s32 remain, rc;
	u32 cmdbuf_sz;
	size_t len;

	if (PP_QOS_DEVICE_IS_ASSERT(qdev))
		return;

	cmdbuf_sz = qdev->init_params.fwcom.cmdbuf_sz;
	start = qdev->init_params.fwcom.cmdbuf;

	pushed = 0;
	cur = start;
	prev = start;

	multiple_cmd.base.type = UC_QOS_CMD_MULTIPLE_COMMANDS;
	cur = __fw_cmd_wr(prev, &multiple_cmd, sizeof(multiple_cmd), 0);

	remain = cmdbuf_sz - (cur - start);

	/* Add to cmdq tail: queue restart and port update */
	pre_enqueue_cmds(qdev);

	read_len = cmd_queue_peek(qdev->drvcmds.cmdq,
				  &dcmd.cmd,
				  sizeof(struct cmd));

	while ((remain >= MAX_FW_CMD_SIZE) && (read_len != 0)) {
		len = dcmd.cmd.len;
		rc = cmd_queue_get(qdev->drvcmds.cmdq, &dcmd.stub, len);
		QOS_ASSERT(rc == len,
			   "Command queue does not contain a full command\n");
		prev = cur;
		dcmd.stub.cmd.pos = cur;

		if (dcmd.cmd.response_sz)
			qdev->init_params.fwcom.wait_on_complete = 1;

		/* Verify command type is valid */
		if (dcmd.cmd.type >= CMD_TYPE_LAST) {
			QOS_LOG_ERR("Unexpected msg type %d\n", dcmd.cmd.type);
			goto cmd_peek;
		}

		/* Write command to the uC command buffer */
		cur = __fw_cmd_wr(prev, &dcmd.stub.data,
				  dcmd.cmd.len - sizeof(dcmd.cmd),
				  0);

		/* Push command to pend queue */
		if (cur != prev) {
			cmd_queue_put(qdev->drvcmds.pendq, &dcmd.stub,
				      dcmd.stub.cmd.len);
			++pushed;
			remain = cmdbuf_sz - (cur - start);
		}

cmd_peek:
		read_len = cmd_queue_peek(qdev->drvcmds.cmdq, &dcmd.cmd,
					  sizeof(struct cmd));
	} /* while (read_len != 0) */

	if (pushed) {
		intern->pushed = pushed;
		prev += 4;
		flags = PP_REG_RD32(prev);
		QOS_BITS_SET(flags, UC_CMD_FLAG_MULTIPLE_COMMAND_LAST);
		PP_REG_WR32(prev, flags);

		/* Read start & end of command buffer to avoid race with FW */
		PP_REG_RD32(start);
		PP_REG_RD32(cur);

		if ((cmdbuf_sz - remain) > qdev->drvcmds.cmdbuf_watermark) {
			pr_debug("New cmdbuf watermark %u (old %u) (remain %d)\n",
				 cmdbuf_sz - remain,
				 qdev->drvcmds.cmdbuf_watermark, remain);
			qdev->drvcmds.cmdbuf_watermark = cmdbuf_sz - remain;
		}

		signal_uc(qdev);
	}

	if ((remain < MAX_FW_CMD_SIZE) && (read_len != 0)) {
		pr_err("ERROR Batch Command Too Big to handle\n");
	}

	if ((cur - start) > cmdbuf_sz) {
		pr_err("Batch Command Overflow: %lu (cur %#lx - start %#lx)\n",
		       cur - start,
		       (unsigned long)pp_virt_to_phys((void *)cur),
		       (unsigned long)pp_virt_to_phys((void *)start));
	}

	/* Verify Guard Regions */
	if (pp_reg_poll_x(qdev->init_params.fwcom.gaurd ,
			  GENMASK_ULL(31, 0), PPV4_QOS_UC_GUARD_MAGIC, 1)) {
		QOS_LOG_ERR("Command Buffer Overflow\n");
	}
}
#else
s32 qos_fw_init(struct pp_qos_dev *qdev)
{
	qdev->initialized = true;

	return 0;
}
#endif
