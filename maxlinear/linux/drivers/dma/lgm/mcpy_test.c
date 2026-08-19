// SPDX-License-Identifier: GPL-2.0
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

#include <linux/clk.h>
#include <linux/completion.h>
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/dmaengine.h>
#include <linux/dma-mapping.h>
#define MCPY_HW_SUPPORT
#ifdef MCPY_HW_SUPPORT
#include <linux/dma/intel_mcpy.h>
#endif
#include <linux/err.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <asm/tsc.h>
#include <asm/special_insns.h>

static void *src_virt;
static phys_addr_t src_phy;
static void *dst_virt;
static phys_addr_t dst_phy;
static u64 min_time = (u64)ULLONG_MAX;
static u64 max_time;
static u64 avg_time;
#define MCPY_PAGE_SIZE 4096

#define MCPY_ROUND_PAGE(x) ((((x) + MCPY_PAGE_SIZE - 1) / MCPY_PAGE_SIZE) * MCPY_PAGE_SIZE)
#define TSC_DELTA(e, s) ((e) > (s)?(e)-(s):ULLONG_MAX - (s) + (e))
#define LGM_TO_LOWER_MAP(p)  (p & (~(0xCLLU << 32)))

static unsigned int mcpy_type;
static unsigned long mcpy_src = 0xECD00000;
static unsigned long mcpy_dst = 0xBFF40000;
static unsigned int mcpy_len = 0x1000;
static unsigned int mcpy_map_mode;
static unsigned int mcpy_num = 1000;
static unsigned int mcpy_memremap_mode;
static unsigned int mcpy_hwflags;
static unsigned int mcpy_flush_src = 1;
static unsigned int mcpy_irq_dis;

inline u64 sw_memcpy(void *dst, void *src, u32 n)
{
	u64 start, end;
	unsigned long flags;

	if (mcpy_irq_dis)
		local_irq_save(flags);
	start = rdtsc_ordered();
	memcpy(dst, src, n);
	end = rdtsc_ordered();
	if (mcpy_irq_dis)
		local_irq_restore(flags);

	end = TSC_DELTA(end, start);
	return end;
}

#define MMIO_LGM_START	0xC0000000
#define MMIO_LGM_END	0xFFFFFFFF

#define IS_MMIO_RANGE(p)	((u64)p >= MMIO_LGM_START && (u64)p <= MMIO_LGM_END)
#define TO_UPPER_MAP(p)		(IS_MMIO_RANGE(p) ? (p) : (void *)((u64)p | (0xCULL << 32)))

inline u64 mmio_memcpy(void *dst, void *src, u32 n)
{
	u64 start, end, from;
	unsigned long flags;

	if (IS_MMIO_RANGE(src))
		from = 1;

	printk_once("%s: Using %s\n", __func__, from ? "memcpy_fromio" : "memcpy_toio");

	if (mcpy_irq_dis)
		local_irq_save(flags);
	if (from) {
		start = rdtsc_ordered();
		memcpy_fromio(dst, src, n);
		end = rdtsc_ordered();
	} else {
		start = rdtsc_ordered();
		memcpy_toio(dst, src, n);
		end = rdtsc_ordered();
	}
	if (mcpy_irq_dis)
		local_irq_restore(flags);

	end = TSC_DELTA(end, start);
	return end;
}

inline u64 hw_memcpy(void *dst, void *src, u32 n)
{
	u64 start, end;
	int pid;
	unsigned long flags;

#ifdef MCPY_HW_SUPPORT
	pid = mcpy_get_pid();
	if (!mcpy_hwflags)
		src = TO_UPPER_MAP(src);
#else
	pr_info("Warning: HW Memcpy not supported by module! Defaulting to SW memcpy!\n");
#endif
	if (mcpy_irq_dis)
		local_irq_save(flags);
	start = rdtsc_ordered();
#ifdef MCPY_HW_SUPPORT
	intel_hw_mcpy(dst, src, pid, n, mcpy_hwflags);
#else
	memcpy(dst, src, n);
#endif
	end = rdtsc_ordered();
	if (mcpy_irq_dis)
		local_irq_restore(flags);

	end = TSC_DELTA(end, start);
	mcpy_free_pid(pid);
	return end;
}

#define TSC_TO_NSECS(tscval) (tscval*1000000/(tsc_khz))

static void dump_buf(void *src, void *dst, unsigned int len)
{
	unsigned int i;
	unsigned char *s, *d;

	s = (unsigned char *)src;
	d = (unsigned char *)dst;
	if (len > 32)
		len = 32;

	pr_info("\nBuf Cmp [Src:Dst] -----\n");
	for (i = 0; i < len; i++) {
		pr_cont("%0x:%0x ", s[i], d[i]);
		if (!((i+1) % 16))
			pr_info(" ");
	}
	pr_info("\n-----------------------\n");
}

static int cmp_addr_to_str(unsigned char chr, char *msg, void *addr, unsigned int len)
{
	unsigned char *p;
	unsigned int i;
#define PAT_BUF_LEN 128
	unsigned char buf[PAT_BUF_LEN];

	if (len >= PAT_BUF_LEN)
		len = PAT_BUF_LEN - 1;

	memset(buf, chr, len);

	if (memcmp(addr, buf, len)) {
		p = (char *)addr;
		pr_warn("%s", msg);
		for (i = 0; i < len; i++) {
			pr_cont("%0x", p[i]);
			if (!((i+1) % 8))
				pr_cont(" ");
			if (!(i+1) % 32)
				pr_info(" "); /* newline */
		}
		return -1;
	}
	return 0;
}

static int mcpy_test_init(void)
{
	int ret = 0;
	u64 end = 0;
	u32 i;
	unsigned int mcpy_page_len;

	mcpy_page_len = MCPY_ROUND_PAGE(mcpy_len);
	pr_info("mcpy_len [%d]; After round, mcpy_page_len = %d\n", mcpy_len, mcpy_page_len);
	if (mcpy_src == 0) {
		src_virt = kmalloc(mcpy_len, GFP_KERNEL);
		src_phy = __pa(src_virt);
	} else {
		src_phy = (phys_addr_t)mcpy_src;
		if (mcpy_map_mode)
			src_virt = ioremap(LGM_TO_LOWER_MAP(src_phy), mcpy_page_len);
		else
			src_virt = memremap(LGM_TO_LOWER_MAP(src_phy), mcpy_page_len,
			(mcpy_memremap_mode?MEMREMAP_WT:MEMREMAP_WB));
	}
	if (mcpy_dst == 0) {
		dst_virt = kmalloc(mcpy_len, GFP_KERNEL);
		dst_phy = __pa(dst_virt);
	} else {
		dst_phy = (phys_addr_t)mcpy_dst;
		if (mcpy_map_mode)
			dst_virt = ioremap(LGM_TO_LOWER_MAP(dst_phy), mcpy_page_len);
		else
			dst_virt = memremap(LGM_TO_LOWER_MAP(dst_phy), mcpy_page_len,
					    (mcpy_memremap_mode ? MEMREMAP_WT : MEMREMAP_WB));
	}
	pr_info("Iterations: [%d], mcpy_map_mode = [%d:%s-%s]\n", mcpy_num, mcpy_map_mode,
	mcpy_map_mode?"ioremap":"memremap", mcpy_map_mode?" ":mcpy_memremap_mode?"WT":"WB");
	pr_info("[mcpy_test] mcpy_src [0x%llx], src_virt [0x%llx], src_phy [0x%llx]\n",
		(u64)mcpy_src, (u64)src_virt, (u64)src_phy);
	pr_info("[mcpy_test] mcpy_dst [0x%llx], dst_virt [0x%llx], dst_phy [0x%llx]\n",
		(u64)mcpy_dst, (u64)dst_virt, (u64)dst_phy);
	pr_info("[mcpy_test] mcpy_type [%d:%s], mcpy_len [%d:0x%x]!\n",
		mcpy_type, mcpy_type == 0?"SW":(mcpy_type == 1)?"HW":"MMIO",
		mcpy_len, mcpy_len);
	if (!dst_phy || !src_phy || !dst_virt || !src_virt)
		pr_info("Error in remap issues ...!\n");

	/* Memset the dst region, so that we can confirm that the copy happened correctly */
	memset(dst_virt, 0x5A, mcpy_len);
	cmp_addr_to_str(0x5A, "memset to dst failed! dst_virt:", dst_virt, 16);
	memset(src_virt, 0xC7, mcpy_len);
	cmp_addr_to_str(0xC7, "memset to src failed! src_virt:", src_virt, 16);
	clflush_cache_range(dst_virt, mcpy_len); /* Get it out of cache, before starting write */
	clflush_cache_range(src_virt, mcpy_len); /* Get it out of cache, before starting read */

	for (i = 0; i < mcpy_num; i++) {
		if (mcpy_type == 0) {
			end = sw_memcpy(dst_virt, src_virt, mcpy_len);
			if (mcpy_flush_src)
				clflush_cache_range(src_virt, mcpy_len); /* Get it out of cache, before next iter */
			clflush_cache_range(dst_virt, mcpy_len); /* Get it out of cache, before next iter */
		} else if (mcpy_type == 1) {
			end = hw_memcpy((void *)dst_phy, (void *)src_phy, mcpy_len);
			/* Only 1 side strictly needed, but lets do both to equalize time comparison */
			if (mcpy_flush_src)
				clflush_cache_range(src_virt, mcpy_len); /* Get it out of cache, before next iter */
			clflush_cache_range(dst_virt, mcpy_len); /* Get it out of cache, before next iter */
		} else {
			end = mmio_memcpy(dst_virt, src_virt, mcpy_len);
			/* Only 1 side strictly needed, but lets do both to equalize time comparison */
			if (mcpy_flush_src)
				clflush_cache_range(src_virt, mcpy_len); /* Get it out of cache, before next iter */
			clflush_cache_range(dst_virt, mcpy_len); /* Get it out of cache, before next iter */
		}
		avg_time += end;
		if (end > max_time)
			max_time = end;
		else if (end < min_time)
			min_time = end;
	}

	pr_info("Avg Time taken for copy of [%d] bytes : [0x%llx:%llu ns]\n",
		mcpy_len, avg_time/mcpy_num, TSC_TO_NSECS(avg_time/mcpy_num));
	pr_info("Min Time taken for copy of [%d] bytes : [0x%llx:%llu ns]\n",
		mcpy_len, min_time, TSC_TO_NSECS(min_time));
	pr_info("Max Time taken for copy of [%d] bytes : [0x%llx:%llu ns]\n",
		mcpy_len, max_time, TSC_TO_NSECS(max_time));

	/* Validate the Copy */
	if (memcmp(src_virt, dst_virt, mcpy_len)) {
		ret = -EINVAL;
		pr_info("Copy Mismatch between src and dest! Failed!\n");
		dump_buf(src_virt, dst_virt, mcpy_len);
	}

	return(ret);
}

void mcpy_test_exit(void)
{
	/* Free if any buffer kmalloc-ed */
	if (mcpy_src)
		iounmap(src_virt);
	else
		kfree(src_virt);

	if (mcpy_dst)
		iounmap(dst_virt);
	else
		kfree(dst_virt);
}

module_param(mcpy_type, uint, 0644);
MODULE_PARM_DESC(mcpy_type, "Select mcpy type : SW-0, HW-1, MMIO-2");
module_param(mcpy_src, ulong, 0644);
MODULE_PARM_DESC(mcpy_src, "mcpy src addr : ");
module_param(mcpy_dst, ulong, 0644);
MODULE_PARM_DESC(mcpy_dst, "mcpy dst addr : ");
module_param(mcpy_len, uint, 0644);
MODULE_PARM_DESC(mcpy_len, "mcpy len : ");
module_param(mcpy_map_mode, uint, 0644);
MODULE_PARM_DESC(mcpy_map_mode, "mcpy map mode : memremap-0, ioremap - 1");
module_param(mcpy_num, uint, 0644);
MODULE_PARM_DESC(mcpy_num, "mcpy #iterations : default 1000 ");
module_param(mcpy_memremap_mode, uint, 0644);
MODULE_PARM_DESC(mcpy_memremap_mode, "mcpy memremap mode : WB-0, WT-1");
module_param(mcpy_hwflags, uint, 0644);
MODULE_PARM_DESC(mcpy_hwflags, "mcpy hwflags for HW Memcpy Op ");
module_param(mcpy_flush_src, uint, 0644);
MODULE_PARM_DESC(mcpy_flush_src, "mcpy Cache Flush Src Buffer: 0-No, 1-Yes");
module_param(mcpy_irq_dis, uint, 0644);
MODULE_PARM_DESC(mcpy_irq_dis, "mcpy IRQ disabled profile: 0-No, 1-Yes");

MODULE_AUTHOR("Ritesh Banerjee");
MODULE_DESCRIPTION("MXL LGM MCPY Test Module");
MODULE_LICENSE("GPL v2");
module_init(mcpy_test_init);
module_exit(mcpy_test_exit);

