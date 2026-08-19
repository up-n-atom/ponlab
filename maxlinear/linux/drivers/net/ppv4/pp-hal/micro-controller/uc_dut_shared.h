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
 * Description: DUT PPV4 FW and HOST CPU common definitions
 */
#ifndef __PP_UC_DUT_SHARED_H__
#define __PP_UC_DUT_SHARED_H__

#define MAX_SUPPORTED_PARAMS (10)
#define MAX_SUPPORTED_STATS (10)
#define DUT_MAX_TABLE_ENTRIES (32)
#define DUT_TABLE_TEST_ENTRY_PARAMS (2)
/* MAGIC number is synced with FW */
#define DUT_MAGIC_NUM 0xA123B456
/* release should be sync with FW */
#define DUT_RELEASE (1)

typedef enum {
	TEST_INVALID,
	TEST_RUN_TABLE,
	TEST_SKIP,
	TEST_CYCLES,
	TEST_DMA_DDR_2_DDR,
	TEST_DMA_DDR_2_DCCM,
	TEST_DMA_DCCM_2_DDR,
	TEST_DMA_DCCM_2_DCCM,
	TEST_DIRECT_DDR,
	TEST_TASK_SWITCH,
	TEST_POP_BUFF,
	TEST_PUSH_BUFF,
	TEST_POP_BURST_BUFF,
	TEST_NUM_SUPPORTED
} dut_type_e;

struct dut_test_info {
	const u32 test_id;	/* test id from 1 to max test supported */
	const u32 name_len;	/* test name string length */
	const u32 desc_len;	/* test description string length */
	const u32 name;		/* test name string pointer */
	const u32 desc;		/* test description string pointer */
};

struct dut_params {
	u64 params[MAX_SUPPORTED_PARAMS];
};

struct dut_stats {
	u32 stats[MAX_SUPPORTED_STATS];
};

struct dut_cmd{
	u8 start;		/* start test */
	u8 cpu_mask;		/* enable cpus */
	u8 verbose;		/* set verbose level if supported */
	u8 params;		/* number of manadatory params */ 
	u32 iterations;		/* number of test iterations */
	dut_type_e type;	/* test type */
	u32 flags;		/* test flags */
	u32 size;               /* test size */
	u32 rsp_valid;		/* rsp valid */
	struct dut_params dut_params; /* test parameters */
};

struct dut_info {
	struct dut_params dut_params;	/* copy of comand params */
	struct dut_stats dut_stats;	/* statistics from test  */
	u32 period;			/* test period in cpu ticks */
	s32 result;			/* result of test 0=OK*/
	u8 run;				/* test was activated */
	u8 on_progress;			/* test on progress for long tests */
	char name[32];			/* short name of test */
} ;

struct dut_local_db {
	u32 release;	/* release help user to identify the current code*/
	u32 test_names_ptr;	/* pointer to tests name and description */
	u32 test_names_size;	/* size of names DB */
	u32 max_test_idx;	/* max tests */
	u64 buff;		/* buffer address  */
	u64 buff_burst[16];	/* buffer address allocated by burst */
	u64 pool;		/* pool that buffer was allocated from */
};

struct dut_test_table_entry {
	u32 type;	/* test type */
	u32 size;	/* size used by each test differently use code or wiki*/
	u32 param[DUT_TABLE_TEST_ENTRY_PARAMS]; /* parameters used by each test*/
};

struct dut_sram {
	struct dut_cmd dut_cmd; 
	struct dut_local_db local_db;
	struct dut_test_table_entry table_entry_db[DUT_MAX_TABLE_ENTRIES];
	u32 sram_magic_num;
};

enum uc_dut_eg_soft_event {
	EG_SOFT_EVENT_DUT_0 = 9,	/* cpu 0 */
	EG_SOFT_EVENT_DUT_1 = 10,	/* cpu 1 */
	EG_SOFT_EVENT_DUT_2 = 11,	/* cpu 2 */
	EG_SOFT_EVENT_DUT_3 = 12,	/* cpu 3 */
	EG_SOFT_EVENT_DUT_SEC_0 = 13,   /* scondary task on cpu 0 */
	EG_SOFT_EVENT_DUT_MAX = 32
};

#endif/* __PP_UC_DUT_SHARED_H__ */ 