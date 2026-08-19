/*
 * Copyright (C) 2021-2024 MaxLinear, Inc.
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
 * Description: DUT PP micro-controllers Debug FS Interface
 */

#define pr_fmt(fmt) "[PP_UC_DBG]:%s:%d: " fmt, __func__, __LINE__

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/debugfs.h>
#include <linux/parser.h>

#include "pp_debugfs_common.h"
#include "uc.h"
#include "uc_dut.h"

#ifdef UC_DUT_ENABLE
/**
 * @brief main uc debugfs dir
 */
static struct dentry *dut_dbgfs;
static u64 *dut_ddr_addr = NULL;
static u32 dut_ddr_size = 0;
static bool g_is_egr = true;

static s32 dut_alloc_ddr_buff(u32 ddr_size, u32 ddr_val, bool is_egr);

#define DUT_MAX_DDR_ALLOC (0x8000)
#define DUT_DEFAULT_BUFFER_SIZE (2048)
#define DUT_800MHZ_TICKS_TO_MSEC (800000)
#define DUT_800MHZ_TICKS_TO_USEC (800)
#define DUT_800MHZ_TICKS_TO_NANO_SEC (8)
/**
 * @brief print test results
 * @param uc_is_egr select the uc cluster egress =1/ingress=0
 * @param cpu_mask cpus to show - 0x1 to 0xF
 * @return None
 */
void _dut_show_results(bool is_egr, u32 cpu_mask)
{
	struct dut_info info;
	u8 i, stat;
	u8 max_cpus;
	int rt;
	
	rt = uc_ccu_maxcpus_get(UC_IS_EGR, &max_cpus);
	
	for (i = 0 ; i < max_cpus; i++) {
		if ((0x1 << i) & cpu_mask) {
			uc_dut_stats_read(&info, i, is_egr);
			if ((info.run) || (info.on_progress)) {
				pr_info("%s CPU %d\n", is_egr ? "EGRESS" : "INGRESS", i);
				for (stat = 0 ; stat < MAX_SUPPORTED_STATS; stat++) {
					if (info.dut_stats.stats[stat])
						pr_info("%3d %12u\n", stat, info.dut_stats.stats[stat]);
				}
				if (info.on_progress)
					pr_info("Test on progress\n");
				else 	
					pr_info("Test result %d\n", info.result);
				if (!info.result) {
					pr_info("Test name : %s\n", info.name);
					pr_info("Test duration : %d ticks, %u msec, %u usec, %u nano sec\n\n",
						info.period, (u32)(info.period)/DUT_800MHZ_TICKS_TO_MSEC, 
						(u32)((info.period)/DUT_800MHZ_TICKS_TO_USEC), 
						(u32)((u64)(((info.period)*10)/DUT_800MHZ_TICKS_TO_NANO_SEC)));
				}
			}
		}
	}
}

enum uc_dut_run_opts {
	uc_dut_run_set_tok_help = 1,
	uc_dut_run_test = 2,
	uc_dut_run_param = 3,
	uc_dut_run_iterations = 4,
	uc_dut_run_verbose = 5,
	uc_dut_run_wait = 6,
	uc_dut_run_flags = 7,
	uc_dut_run_size = 8,
	uc_dut_run_cpu_mask = 9,
	uc_dut_run_is_egr = 10
};

static const match_table_t uc_dut_run_set_tokens = {
	{ uc_dut_run_set_tok_help, "help" },
	{ uc_dut_run_test, "test=%u" },
	{ uc_dut_run_param, "param=%u" },
	{ uc_dut_run_iterations, "iter=%u" },
	{ uc_dut_run_verbose, "verbose" },
	{ uc_dut_run_wait, "wait=%u" },
	{ uc_dut_run_size, "size=%u" },
	{ uc_dut_run_flags, "flags=%x" },
	{ uc_dut_run_cpu_mask, "cpu_mask=%x" },
	{ uc_dut_run_is_egr, "is_egr=%u" },
	{ 0 },
};

static void uc_dut_run_print_help(void)
{
	pr_info("set DUT command and activate it <test>[flags][cpu_mask][size][param][verbose][iter][wait]\n");
	pr_info(" 	test     : test number according to known tests\n");
	pr_info(" 	flags    : test flags for used by the uc\n");
	pr_info(" 	cpu_mask : run test on cpu 0 to 3  - if not defined will runon cpu 0\n");
	pr_info(" 	size     : test size\n");
	pr_info(" 	param    : set paramters for the test if needed, multiple parameters can be set\n");
	pr_info(" 	verbose  : set verbose mode to give mor information during the test (0/1)\n");
	pr_info(" 	iter     : number of iteration to run for that test\n");
	pr_info(" 	wait     : wait till end of test (0/1 - Default is 1)\n");
	pr_info(" Example : echo test=1 size=1000 flags=0x1 param=100 param=200 verbose = 1 iter=5 wait=1 > dut\n");
}

/**
 * @brief set run test
 * @return None
 */
static void uc_dut_run_set(char *args, void *data)
{
	substring_t substr[MAX_OPT_ARGS];
	enum uc_dut_run_opts opt;
	char *tok;
	u32 num = 0;
	s32 val;
	u64 val64;
	bool wait = true;
	struct dut_cmd dut_cmd = {0};
	bool is_egr = true;
	
	while ((tok = strsep(&args, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, uc_dut_run_set_tokens, substr);
		switch (opt) {
		case uc_dut_run_set_tok_help:
			uc_dut_run_print_help();
			return;
		case uc_dut_run_test:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			dut_cmd.type = (u32)val; 	
			break;
		case uc_dut_run_wait:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			wait = (bool)val; 	
			break;	
		case uc_dut_run_param:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			if (num < MAX_SUPPORTED_PARAMS)	{
				dut_cmd.dut_params.params[num++] = (u32)val;	
				dut_cmd.params++;
			} else {
				pr_err("number of supported params : %d\n", MAX_SUPPORTED_PARAMS);	
			}
			break;
		case uc_dut_run_verbose:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			dut_cmd.verbose = (u8)val;
			break;
		case uc_dut_run_iterations:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			dut_cmd.iterations = (u32)val;
			break;
		case uc_dut_run_flags:
			if (match_u64(&substr[0], &val64))
				goto opt_parse_err;
			dut_cmd.flags = (u32)val64;
			break;	
		case uc_dut_run_size:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;
			dut_cmd.size = val;
			break;	
		case uc_dut_run_cpu_mask:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;	
			dut_cmd.cpu_mask = (val & 0xF);
			break;
		case uc_dut_run_is_egr:
			if (match_int(&substr[0], &val))
				goto opt_parse_err;	
			is_egr = val;
			break;			
		default:
			pr_info("Error: Invalid option '%s'\n", tok);
			uc_dut_run_print_help();
			return;
		}
	}

	if (dut_cmd.cpu_mask == 0) {
		dut_cmd.cpu_mask = 0x1;
	}

	if (!dut_ddr_addr) {
		if (dut_alloc_ddr_buff(DUT_DEFAULT_BUFFER_SIZE, 0, is_egr))
			pr_err("DDR alloc buffer failed, test can be impacted\n");

	}

	if (uc_dut_cmd_send(&dut_cmd, wait, is_egr)) {
		pr_err("Failed to config DUT\n");
	}

	_dut_show_results(is_egr, dut_cmd.cpu_mask);
	return;	

opt_parse_err:
	pr_err("Failed to parse %s value\n", tok);
	return;	
}

/**
 * @brief get run test
 * @return None
 */
static void uc_dut_run_show(struct seq_file *f)
{
	_dut_show_results(g_is_egr, 0xf);
}

PP_DEFINE_DEBUGFS(uc_dut_run, uc_dut_run_show,
		  uc_dut_run_set);

enum uc_dut_load_opts {
	uc_dut_set_cpu_tok_help = 1,
	uc_dut_cpu_0 = 2,
	uc_dut_cpu_1 = 3,
	uc_dut_cpu_2 = 4,
	uc_dut_cpu_3 = 5,
	uc_dut_load_is_egr = 6
};

static const match_table_t uc_dut_load_set_tokens = {
	{ uc_dut_set_cpu_tok_help, "help" },
	{ uc_dut_cpu_0, "cpu0=%u" },
	{ uc_dut_cpu_1, "cpu1=%u" },
	{ uc_dut_cpu_2, "cpu2=%u" },
	{ uc_dut_cpu_3, "cpu3=%u" },
	{ uc_dut_load_is_egr, "is_egr=%u" },
	{ 0 },
};

/**
 * @brief load a new DUT image to cluster
 * @return None
 */
static void uc_dut_load(char *args, void *data)
{
	u32 cpu[4];
	substring_t substr[MAX_OPT_ARGS];
	enum uc_dut_load_opts opt;
	char *tok;
	s32 val;
	u32 is_egr=1;

	while ((tok = strsep(&args, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, uc_dut_load_set_tokens, substr);
		switch (opt) {
			case uc_dut_set_cpu_tok_help:
				break;
			case uc_dut_cpu_0:
				if (match_int(&substr[0], &val))
					goto opt_parse_err;
				cpu[0] = (u32)val; 
				break;
			case uc_dut_cpu_1:
				if (match_int(&substr[0], &val))
					goto opt_parse_err;
				cpu[1] = (u32)val; 
				break;
			case uc_dut_cpu_2:
				if (match_int(&substr[0], &val))
					goto opt_parse_err;
				cpu[2] = (u32)val; 
				break;
			case uc_dut_cpu_3:
				if (match_int(&substr[0], &val))
					goto opt_parse_err;
				cpu[3] = (u32)val; 
				break;
			case uc_dut_load_is_egr:
				if (match_int(&substr[0], &val))
					goto opt_parse_err;
				is_egr = (u32)val; 
				break;	
			default:
				break;	
		}
	}
	
	if (uc_dut_load_image(cpu, is_egr)) {
		pr_err("failed to load DUT FW\n");
	}
	
	return;

opt_parse_err:
	pr_err("Failed to parse %s value\n", tok);
	return;	
}

PP_DEFINE_DEBUGFS(uc_dut_load, NULL,
		  uc_dut_load);

enum uc_dut_entry_opts {
	uc_dut_entry_tok_help = 1,
	uc_dut_entry_index = 2,
	uc_dut_entry_size = 3,
	uc_dut_entry_test = 4,
	uc_dut_entry_param = 5,
	uc_dut_entry_is_egr = 6,
};

static const match_table_t uc_dut_entry_set_tokens = {
	{ uc_dut_entry_tok_help, "help" },
	{ uc_dut_entry_index, "index=%u" },
	{ uc_dut_entry_size, "size=%u" },
	{ uc_dut_entry_test, "test=%u" },
	{ uc_dut_entry_param, "param=%u" },
	{ uc_dut_entry_is_egr, "is_egr=%u" },
	{ 0 }
};

static void uc_dut_entry_help(void)
{
	pr_info("set DUT table entry <test><size>[param-0][param-1]\n");
	pr_info("Example : echo index=5 test=1 size param=100 param=200 > entry\n");
}

static void uc_dut_entries_set(char *args, void *data)
{
	substring_t substr[MAX_OPT_ARGS];
	enum uc_dut_entry_opts opt;
	struct dut_test_table_entry entry = {0};
	char *tok;
	s32 val;
	u32 entry_index = 0xFF;
	u32 num = 0;
	bool is_egr = true;

	while ((tok = strsep(&args, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, uc_dut_entry_set_tokens, substr);
		switch (opt) {
			case uc_dut_entry_tok_help:
				uc_dut_entry_help();
				break;
			case uc_dut_entry_index:
				if (match_int(&substr[0], &val))
					goto opt_parse_err;
				entry_index = (u32)val; 
				if (entry_index >= DUT_MAX_TABLE_ENTRIES) {
					pr_err("number of supported entries : %d\n", DUT_MAX_TABLE_ENTRIES);
					return;
				}
				break;	
			case uc_dut_entry_size:
				if (match_int(&substr[0], &val))
					goto opt_parse_err;
				entry.size = (u32)val; 
				break;
			case uc_dut_entry_test:
				if (match_int(&substr[0], &val))
					goto opt_parse_err;
				entry.type = (u32)val; 
				break;
			case uc_dut_entry_param:
				if (match_int(&substr[0], &val))
					goto opt_parse_err;
				if (num < DUT_TABLE_TEST_ENTRY_PARAMS)	{
					entry.param[num++] = (u32)val;	
				} else {
					pr_err("number of supported params : %d\n", DUT_TABLE_TEST_ENTRY_PARAMS);
					return;	
				}
				break;
			case uc_dut_entry_is_egr:
				if (match_int(&substr[0], &val))
					goto opt_parse_err;
				is_egr = (u32)val; 
				break;	
			default:
				pr_info("Error: Invalid option '%s'\n", tok);
				uc_dut_entry_help();
				return;	
		}
	}

	if (entry_index >= 0xFF) {
		pr_err("entry index not valid\n");
		return;	
	}
	
	if (uc_dut_entry_set(entry_index, &entry, is_egr)) {
		pr_err("Failed to set entry \n");
	}

	return;

opt_parse_err:
	pr_err("Failed to parse %s value\n", tok);
	return;	
}

static void uc_dut_entries_show(struct seq_file *f)
{
	u32 i;
	char *line ="=================================================";
	struct dut_test_table_entry entry;

	for (i = 0 ; i < DUT_MAX_TABLE_ENTRIES; i++) {
		uc_dut_entry_get(i, &entry, g_is_egr);
		if (entry.type == TEST_INVALID)
			break;
		if (i == 0)	{
			pr_info("| ENTRY | TYPE | SIZE |   PARAM-0  |   PARAM-1  |\n");
			pr_info("%s\n", line);
		}
		pr_info("| %5d | %4d | %4d | 0x%-8x | 0x%-8x |\n", i, entry.type, entry.size, entry.param[0], entry.param[1]);
	}

	if (i > 0) 
		pr_info("%s\n", line);
}

PP_DEFINE_DEBUGFS(uc_dut_entry, uc_dut_entries_show,
		  uc_dut_entries_set);

enum uc_dut_ddr_opts {
	uc_dut_ddr_tok_help = 1,
	uc_dut_ddr_size = 2,
	uc_dut_ddr_val = 3,
	uc_dut_ddr_is_egr = 4
};

static const match_table_t uc_dut_set_ddr_tokens = {
	{ uc_dut_ddr_tok_help, "help" },
	{ uc_dut_ddr_size, "size=%u" },
	{ uc_dut_ddr_val, "val=%x" },
	{ uc_dut_ddr_is_egr, "is_egr=%u" },
	{ 0 }
};

static void uc_dut_ddr_help(void)
{
	pr_info("allocate DDR buffer <size>[val]\n");
	pr_info("Example : echo size=2048 val=0x12345678 > ddr_buff\n");
}

static s32 dut_alloc_ddr_buff(u32 ddr_size, u32 ddr_val, bool is_egr)
{
	if (dut_ddr_addr) {
		pr_info("free ddr_addr 0x%llx\n", (u64)dut_ddr_addr);
		kfree((void *)dut_ddr_addr);
		dut_ddr_addr = NULL;
		dut_ddr_size = 0;
	}

	if (ddr_size == 0) {
		uc_dut_set_buff_address((u64)0, is_egr);
		return 0;
	}

	dut_ddr_size = ddr_size;
	dut_ddr_addr = kzalloc(dut_ddr_size, GFP_KERNEL);	
	uc_dut_set_buff_address((u64)dut_ddr_addr, is_egr);

	if (dut_ddr_addr) {
		memset(dut_ddr_addr, ddr_val, (dut_ddr_size>>1));
		pr_info("ddr_addr 0x%llx\n", (u64)dut_ddr_addr);
	} else {
		pr_err("DUT : failed to alloc ddr buffer for test\n");
		return -1;
	}

	return 0;
}

static void uc_dut_set_ddr_buff(char *args, void *data)
{
	substring_t substr[MAX_OPT_ARGS];
	enum uc_dut_ddr_opts opt;
	char *tok;
	s32 ddr_val = 0xAA, ddr_size = 0;
	bool is_egr = true;
	s32 val;

	while ((tok = strsep(&args, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, uc_dut_set_ddr_tokens, substr);
		switch (opt) {
			case uc_dut_ddr_tok_help:
				uc_dut_ddr_help();
				return;
			case uc_dut_ddr_size:
				if (match_int(&substr[0], &ddr_size))
					goto opt_parse_err;
				if (ddr_size > DUT_MAX_DDR_ALLOC) {
					pr_err("MAX DDR allocation size : %d\n", DUT_MAX_DDR_ALLOC);
					return;		
				}
				break;
			case uc_dut_ddr_val:
				if (match_int(&substr[0], &ddr_val))
					goto opt_parse_err;
				
				break;	
			case uc_dut_ddr_is_egr:
				if (match_int(&substr[0], &val))
					goto opt_parse_err;
				is_egr = val;
				break;		
			default:
				pr_info("Error: Invalid option '%s'\n", tok);
				uc_dut_ddr_help();
				return;
		}
	}

	if (dut_alloc_ddr_buff(ddr_size, ddr_val, is_egr)) 
		pr_err("DDR alloc buffer failed, test can be impacted\n");
	
	return;

opt_parse_err:
	pr_err("Failed to parse %s value\n", tok);
	return;		
}

static void uc_dut_show_ddr_buff(struct seq_file *f)
{
	pr_info("DDR Buffer address 0x%llx size %d\n", (u64)dut_ddr_addr, dut_ddr_size);
}

PP_DEFINE_DEBUGFS(uc_dut_ddr_buff, uc_dut_show_ddr_buff,
		  uc_dut_set_ddr_buff);		  

static void uc_dut_show_release(struct seq_file *f)
{
	if (!uc_dut_check_release(g_is_egr))
		uc_dut_get_tests_names(f, g_is_egr);
}

PP_DEFINE_DEBUGFS(uc_dut_release, uc_dut_show_release,
		  NULL);

static void uc_dut_uc_type_set(char *cmd_buf, void *data)
{
	u8 uc_type;

	if (unlikely(sscanf(cmd_buf, "%hhu",
			    &uc_type) != 1)) {
		pr_err("Command format error\n");
		pr_err("<uc cluster 0=ING, 1=EGR>\n");
		return;
	}

	g_is_egr = (bool)uc_type;
}

static void uc_dut_uc_type_show(struct seq_file *f)
{
	pr_info("cluster type 0x%d\n", g_is_egr);
}

PP_DEFINE_DEBUGFS(uc_dut_uc_type, uc_dut_uc_type_show,
		  uc_dut_uc_type_set);

static struct debugfs_file uc_dut_dbg_files[] = {
	{"run", &PP_DEBUGFS_FOPS(uc_dut_run) },
	{"load", &PP_DEBUGFS_FOPS(uc_dut_load) },
	{"entry", &PP_DEBUGFS_FOPS(uc_dut_entry) },
	{"ddr_buff", &PP_DEBUGFS_FOPS(uc_dut_ddr_buff) },
	{"rel", &PP_DEBUGFS_FOPS(uc_dut_release) },
	{"set_uc_type", &PP_DEBUGFS_FOPS(uc_dut_uc_type) },
};

s32 uc_dut_dbg_init(struct dentry *parent)
{
	/* uc dut debugfs dir */
	return pp_debugfs_create(parent, "uc_dut", &dut_dbgfs, uc_dut_dbg_files,
				 ARRAY_SIZE(uc_dut_dbg_files), NULL);
}

void uc_dut_dbg_clean(void)
{
	debugfs_remove_recursive(dut_dbgfs);
	dut_dbgfs = NULL;
}

#endif /* UC_DUT_ENABLE */
