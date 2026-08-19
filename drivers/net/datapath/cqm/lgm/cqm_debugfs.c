// SPDX-License-Identifier: GPL-2.0
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

#include "cqm_debugfs.h"
#include <net/datapath_proc_api.h>
#include <net/mxl_cbm_api.h>
#include "cqm.h"
#include "../cqm_common.h"

#define PRINTK			pr_err
#define CQM_DEBUGFS_DBG		"/sys/kernel/debug/cqm/dbg"
/* match state */
#define PATTERN_MATCH_INIT	0
#define PATTERN_MATCH_START	1
#define PATTERN_MATCH_FAIL	2
#define PATTERN_MATCH_PASS	3
#define ENTRY_FILLED		0
#define ENTRY_USED		1
#define LOOKUP_FIELD_BITS	13
#define EGFLAG_BIT		12 /* egflag bit */
#define EP_BIT			8  /* EP port starting bit */
#define C_ARRAY_SIZE		20
#define CARE_FLAG		0
#define CARE_NOT_FLAG		1
/*store result */
#define MAX_PATTERN_NUM		1024

#if defined(CONFIG_MXL_CQM_DBG)
u32 cqm_max_print_num = -1;
u32 cqm_print_num_en;
#endif
u32 cqm_dbg_flag;
static int lookup_mask_n;
static int pattern_match_flag;	/*1--start matching  2--failed, 3---match 0k */
static unsigned char lookup_mask1[LOOKUP_FIELD_BITS];
static int c_tmp_data[C_ARRAY_SIZE];
static int lookup_match_num;
static unsigned short lookup_match_mask[MAX_PATTERN_NUM];
/*save tmp_index */
static unsigned short lookup_match_index[MAX_PATTERN_NUM];
/*save tmp_index */
static unsigned char lookup_match_qid[MAX_PATTERN_NUM];
static int tmp_pattern_port_id;
static int left_n;
/*10 bits lookup table except 4 bits EP */
static unsigned char lookup_tbl_flags[BIT(LOOKUP_FIELD_BITS)];
static char *dcp_dma_desc[DLY_PORT_COUNT] = {
	"DOCSIS FIFO 0",
	"DOCSIS FIFO 1",
	"DOCSIS FIFO 2",
	"DOCSIS FIFO 3",
	"DOCSIS FIFO 4",
	"DOCSIS FIFO 5",
	"DOCSIS FIFO 6",
	"DOCSIS FIFO 7",
	"FSQM Q4",
	"FSQM Q5",
	"FSQM Q6",
	"FSQM Q7",
	"FSQM Q0",
	"FSQM Q1",
	"FSQM Q2",
	"FSQM Q3"
};

#define CQM_DBG_ENUM_OR_STRING(name, short_name) short_name

#define CQM_DBG_FLAG_LIST {\
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_DBG, "dbg"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_TX_DESC, "tx_desc"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_TX_PATH, "tx_path"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_QID_MAP, "qid_map"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_BUFF_ALLOC, "buff_alloc"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_CPU_BUFF, "cpu_buff"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_BUFF_RTN, "buff_rtn"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_DMA_CTRL, "dma_ctrl"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_DMA_PORT, "dma_port"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_PON_PORT, "pon_port"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_DC_PORT, "dc_port"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_CPU_PORT, "cpu_port"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_MTU, "mtu"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_FE_POLICY, "fe_policy"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_Q_FLUSH, "q_flush"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_PORT_FLUSH, "port_flush"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_RX, "rx"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_DP_INTF, "dp_intf"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_MARKING, "marking"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_MARKING_BUFF, "marking_buff"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_API, "api"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_RX_DP, "rx_dp"), \
		CQM_DBG_ENUM_OR_STRING(CQM_DBG_FLAG_ENABLE_ALL, "enable_all")\
	}

#undef CQM_DBG_ENUM_OR_STRING
#define CQM_DBG_ENUM_OR_STRING(name, short_name) short_name
char *cqm_dbg_flag_str[] = CQM_DBG_FLAG_LIST;

#undef CQM_DBG_ENUM_OR_STRING
#define CQM_DBG_ENUM_OR_STRING(name, short_name) name
u32 cqm_dbg_flag_list[] = CQM_DBG_FLAG_LIST;

#define LIST_ALL_CASES(t, mask, not_care) \
	for (t[0] = 0;  t[0] < ((mask[0] == (not_care)) ? 2 : 1); t[0]++) \
	for (t[1] = 0;  t[1] < ((mask[1] == (not_care)) ? 2 : 1); t[1]++) \
	for (t[2] = 0;  t[2] < ((mask[2] == (not_care)) ? 2 : 1); t[2]++) \
	for (t[3] = 0;  t[3] < ((mask[3] == (not_care)) ? 2 : 1); t[3]++) \
	for (t[4] = 0;  t[4] < ((mask[4] == (not_care)) ? 2 : 1); t[4]++) \
	for (t[5] = 0;  t[5] < ((mask[5] == (not_care)) ? 2 : 1); t[5]++) \
	for (t[6] = 0;  t[6] < ((mask[6] == (not_care)) ? 2 : 1); t[6]++) \
	for (t[7] = 0;  t[7] < ((mask[7] == (not_care)) ? 2 : 1); t[7]++) \
	for (t[8] = 0;  t[8] < 1; t[8]++) \
	for (t[9] = 0;  t[9] < 1; t[9]++) \
	for (t[10] = 0; t[10] < 1; t[10]++) \
	for (t[11] = 0; t[11] < 1; t[11]++) \
	for (t[12] = 0; t[12] < ((mask[12] == (not_care)) ? 2 : 1); t[12]++)

#define proc_printf(s, fmt, arg...) \
	do { \
		if (!(s)) \
			pr_info(fmt, ##arg); \
		else \
			seq_printf((s), fmt, ##arg); \
	} while (0)

typedef void (*cqm_dbg_single_callback_t) (struct seq_file *);
typedef int (*cqm_dbg_callback_t) (struct seq_file *, int);
typedef int (*cqm_dbg_init_callback_t) (void);
typedef ssize_t(*cqm_dbg_write_callback_t) (struct file *file,
					     const char __user *input,
					     size_t size, loff_t *loff);

struct cqm_dbg_file_entry {
	cqm_dbg_callback_t multi_callback;
	cqm_dbg_single_callback_t single_callback;
	int pos;
	int single_call_only;
};

struct cqm_dbg_entry {
	char *name;
	cqm_dbg_single_callback_t single_callback;
	cqm_dbg_callback_t multi_callback;
	cqm_dbg_init_callback_t init_callback;
	cqm_dbg_write_callback_t write_callback;
	const struct file_operations ops;
};

static void cqm_dp_set_lookup_qid_via_index(struct cbm_lookup *info);
static int cqm_dp_get_lookup_qid_via_index(struct cbm_lookup *info);
static void combine_util(const int *arr, int *data, int start, int end,
			 int index, int r);
static int check_pattern(int *data, int r);
static void cqm_lookup_table_via_qid(int qid);
static void cqm_lookup_table_remap(int old_q, int new_q);
static int cqm_find_pattern(int port_id, struct seq_file *s, int qid);
static int cqm_get_dont_care_lookup(char *s);
static void cqm_lookup_table_recursive(int k, int tmp_index,
				       int set_flag, int qid);
static int cqm_dbg_init_cb(void);
static int cqm_dbg_open(struct inode *inode, struct file *file);
static int cqm_dbg_release(struct inode *inode, struct file *file);

/***************debugfs start******************************/
static ssize_t print_cqm_dbg_cntrs_write(struct file *file,
					 const char __user *buf,
					 size_t count, loff_t *ppos)
{
	int len;
	char str[64], *p = (char *)str, *param_list[5] = { 0 };
	int num = 0;

	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;
	if (!len)
		return count;

	#ifndef KERNEL_5_15_DEP_MODULE_NOT_PORTED
	num = dp_split_buffer(p, param_list, ARRAY_SIZE(param_list));
	#endif
	if (num == 1 && (strncmp(param_list[0], "help", 4) == 0)) {
		PRINTK("Help: Please use:\n");
		PRINTK("cat /sys/kernel/debug/cqm/pkt_count\n");
	}
	return count;
}

static int print_cqm_dbg_cntrs(struct seq_file *s, void *v)
{
	int i, j;
	u64 rx_cnt_t = 0, tx_cnt_t = 0, free_cnt_t = 0, alloc_cnt_t = 0;
	u64 ring_buff_cnt_t = 0, lro_err_cnt_t = 0, free_frag_cnt_t = 0;

	seq_puts(s, "Print CQEM debug counters:\n");
	seq_puts(s, "\t\t\trx\t\ttx\t\tfree\t\tfree_frag\tlro_err_free\talloc\t\tring_b\n");
	for (i = 0; i < CQM_MAX_POLICY_NUM; i++) {
		for (j = 0; j < CQM_MAX_POOL_NUM; j++) {
			rx_cnt_t = STATS_GET(cqm_dbg_cntrs[i][j].rx_cnt);
			tx_cnt_t = STATS_GET(cqm_dbg_cntrs[i][j].tx_cnt);
			free_cnt_t = STATS_GET(cqm_dbg_cntrs[i][j].free_cnt);
			free_frag_cnt_t = STATS_GET(cqm_dbg_cntrs[i][j].free_frag_cnt);
			lro_err_cnt_t = STATS_GET(cqm_dbg_cntrs[i][j].free_lro_err_cnt);
			alloc_cnt_t = STATS_GET(cqm_dbg_cntrs[i][j].alloc_cnt);
			ring_buff_cnt_t = STATS_GET(cqm_dbg_cntrs[i]
						    [j].dma_ring_buff_cnt);

			if (rx_cnt_t || tx_cnt_t || free_cnt_t || lro_err_cnt_t ||
			    alloc_cnt_t || ring_buff_cnt_t || free_frag_cnt_t) {
				seq_printf(s, "policy (%3d) pool( %2d):", i, j);

				seq_printf(s, "\t0x%08llx", rx_cnt_t);
				seq_printf(s, "\t0x%08llx", tx_cnt_t);
				seq_printf(s, "\t0x%08llx", free_cnt_t);
				seq_printf(s, "\t0x%08llx", free_frag_cnt_t);
				seq_printf(s, "\t0x%08llx", lro_err_cnt_t);
				seq_printf(s, "\t0x%08llx", alloc_cnt_t);
				seq_printf(s, "\t0x%08llx\n", ring_buff_cnt_t);
			}
		}
	}
	return 0;
}

static int cqm_dbg_cntr_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, print_cqm_dbg_cntrs, inode->i_private);
}

static const struct file_operations cqm_dbg_cntrs_fops = {
	.open = cqm_dbg_cntr_open,
	.read = seq_read,
	.write = print_cqm_dbg_cntrs_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static ssize_t cqm_ls_write(struct file *file, const char __user *buf,
			    size_t count, loff_t *ppos)
{
	int len, num = 0;
	char str[64], *p = (char *)str, *param_list[5] = { 0 };

	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;
	if (!len)
		return count;
	num = dp_split_buffer(p, param_list, ARRAY_SIZE(param_list));
	if (num == 1 && (strncmp(param_list[0], "help", 4) == 0))
		PRINTK("Help: Please use: cat /sys/kernel/debug/cqm/cqmls\n");
	return count;
}

static int cqm_ls_seq_read(struct seq_file *s, void *v)
{
	void *ls_base = cqm_get_ls_base();
	void *ls_phy_base = cqm_get_ls_phy_base();
	phys_addr_t ls_addr, p_base = (phys_addr_t)ls_phy_base;
	u32 reg_r_data = 0, q_len = 0, q_full = 0, q_empty = 0, q_cnt = 0;
	u32 ls_pid = 0;

	seq_printf(s, "# CQM_LOAD_SPREADER 0x%llx\n", p_base);
	seq_puts(s, "# ls_pid\t # ls_addr\t # q__len\t # q__full\t");
	seq_puts(s, "# q_empty\t # cntval\n");
	for (ls_pid = 0; ls_pid < 8; ls_pid++) {
		reg_r_data = cbm_r32(ls_base + CBM_LS_PORT(ls_pid, status));
		ls_addr = p_base + CBM_LS_PORT(ls_pid, status);
		q_len = get_val(reg_r_data, LS_STATUS_PORT0_QUEUE_LEN_MASK,
				LS_STATUS_PORT0_QUEUE_LEN_POS);
		q_full = get_val(reg_r_data, LS_STATUS_PORT0_QUEUE_FULL_MASK,
				 LS_STATUS_PORT0_QUEUE_FULL_POS);
		q_empty = get_val(reg_r_data, LS_STATUS_PORT0_QUEUE_EMPTY_MASK,
				  LS_STATUS_PORT0_QUEUE_EMPTY_POS);
		q_cnt = get_val(reg_r_data, LS_STATUS_PORT0_CNT_VAL_MASK,
				LS_STATUS_PORT0_CNT_VAL_POS);
		seq_printf(s, "# port %d\t #0x%llx\t #%08d\t #%08d\t #%08d\t",
			   ls_pid, ls_addr, q_len, q_full, q_empty);
		seq_printf(s, "# 0x%08x\n", q_cnt);
	}
	return 0;
}

static int cqm_ls_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cqm_ls_seq_read, inode->i_private);
}

static const struct file_operations cqm_ls_fops = {
	.open = cqm_ls_open,
	.read = seq_read,
	.write = cqm_ls_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static ssize_t cqm_ctrl_write(struct file *file, const char __user *buf,
			      size_t count, loff_t *ppos)
{
	int len, num = 0;
	char str[64], *p = (char *)str, *param_list[5] = { 0 };

	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;
	if (!len)
		return count;

	#ifndef KERNEL_5_15_DEP_MODULE_NOT_PORTED
	num = dp_split_buffer(p, param_list, ARRAY_SIZE(param_list));
	#endif
	if (num == 1 && (strncmp(param_list[0], "help", 4) == 0))
		PRINTK("Help: cat /sys/kernel/debug/cqm/cqmctrl\n");
	return count;
}

static int cqm_ctrl_seq_read(struct seq_file *s, void *v)
{
	u32 i, offset;
	void *ctrl_base = cqm_get_ctrl_base();
	void *ctrl_phy_base = cqm_get_ctrl_phy_base();
	phys_addr_t p_base = (phys_addr_t)ctrl_phy_base;

	for (i = 0; i < 8; i++) {
		offset = i * 0x40;
		seq_printf(s, "\n# Name: CBM_IRNCR_%d \t CBM_IRNICR_%d \t"
			   "CBM_IRNEN_%d\n", i, i, i);
		seq_printf(s, "# Addr: 0x%llx \t 0x%llx \t 0x%llx\n",
			   p_base + CBM_IRNCR_0 + offset,
			   p_base + CBM_IRNICR_0 + offset,
			   p_base + CBM_IRNEN_0 + offset);
		seq_printf(s, "# Val:  0x%08x \t 0x%08x \t 0x%08x\n\n",
			   cbm_r32(ctrl_base + CBM_IRNCR_0 + offset),
			   cbm_r32(ctrl_base + CBM_IRNICR_0 + offset),
			   cbm_r32(ctrl_base + CBM_IRNEN_0 + offset));
		seq_printf(s, "# Name: IGP_IRNCR_%d \t IGP_IRNICR_%d \t"
			   "IGP_IRNEN_%d\n", i, i, i);
		seq_printf(s, "# Addr: 0x%llx \t 0x%llx \t 0x%llx\n",
			   p_base + IGP_IRNCR_0 + offset,
			   p_base + IGP_IRNICR_0 + offset,
			   p_base + IGP_IRNEN_0 + offset);
		seq_printf(s, "# Val:  0x%08x \t 0x%08x \t 0x%08x\n\n",
			   cbm_r32(ctrl_base + IGP_IRNCR_0 + offset),
			   cbm_r32(ctrl_base + IGP_IRNICR_0 + offset),
			   cbm_r32(ctrl_base + IGP_IRNEN_0 + offset));
		seq_printf(s, "# Name: EGP_IRNCR_%d \t EGP_IRNICR_%d \t"
			   "EGP_IRNEN_%d\n", i, i, i);
		seq_printf(s, "# Addr: 0x%llx \t 0x%llx \t 0x%llx\n",
			   p_base + EGP_IRNCR_0 + offset,
			   p_base + EGP_IRNICR_0 + offset,
			   p_base + EGP_IRNEN_0 + offset);
		seq_printf(s, "# Val:  0x%08x \t 0x%08x \t 0x%08x\n\n",
			   cbm_r32(ctrl_base + EGP_IRNCR_0 + offset),
			   cbm_r32(ctrl_base + EGP_IRNICR_0 + offset),
			   cbm_r32(ctrl_base + EGP_IRNEN_0 + offset));
	}
	seq_puts(s, "# Name: VERSION_REG\tCBM_BUF_SIZE0_7\tCBM_BUF_SIZE8_15\t"
		 "CBM_PB_BASE\n");
	seq_printf(s, "# Addr: 0x%llx \t 0x%llx \t 0x%llx \t 0x%llx\n",
		   p_base + CBM_VERSION_REG,
		   p_base + CBM_BUF_SIZE0_7,
		   p_base + CBM_BUF_SIZE8_15,
		   p_base + CBM_PB_BASE);
	seq_printf(s, "# Val: 0x%08x \t 0x%08x \t 0x%08x \t 0x%08x\n\n",
		   cbm_r32(ctrl_base + CBM_VERSION_REG),
		   cbm_r32(ctrl_base + CBM_BUF_SIZE0_7),
		   cbm_r32(ctrl_base + CBM_BUF_SIZE8_15),
		   cbm_r32(ctrl_base + CBM_PB_BASE));
	seq_puts(s, "# Name: CBM_CTRL \t CBM_LL_DBG \t CBM_BSL_CTRL\n");
	seq_printf(s, "# Addr: 0x%llx \t 0x%llx \t 0x%llx\n",
		   p_base + CBM_CTRL,
		   p_base + CBM_LL_DBG,
		   p_base + CBM_BSL_CTRL);
	seq_printf(s, "# Val:  0x%08x \t 0x%08x \t 0x%08x\n\n",
		   cbm_r32(ctrl_base + CBM_CTRL),
		   cbm_r32(ctrl_base + CBM_LL_DBG),
		   cbm_r32(ctrl_base + CBM_BSL_CTRL));
	seq_puts(s, "# Name: CBM_QID_MODE_SEL_REG_0 CBM_QID_MODE_SEL_REG_1\n");
	seq_printf(s, "# Addr: 0x%llx \t 0x%llx\n",
		   p_base + CBM_QID_MODE_SEL_REG_0,
		   p_base + CBM_QID_MODE_SEL_REG_0 + 4);
	seq_printf(s, "# Val:  0x%08x \t 0x%08x\n\n",
		   cbm_r32(ctrl_base + CBM_QID_MODE_SEL_REG_0),
		   cbm_r32(ctrl_base + CBM_QID_MODE_SEL_REG_0 + 4));
	seq_puts(s, "# Name: CBM_BM_BASE \t CBM_WRED_BASE\n");
	seq_printf(s, "# Addr: 0x%llx \t 0x%llx\n",
		   p_base + CBM_BM_BASE,
		   p_base + CBM_WRED_BASE);
	seq_printf(s, "# Val:  0x%08x \t 0x%08x\n\n",
		   cbm_r32(ctrl_base + CBM_BM_BASE),
		   cbm_r32(ctrl_base + CBM_WRED_BASE));
	seq_puts(s, "# Name: CBM_QPUSH_BASE \t CBM_TX_CREDIT_BASE\n");
	seq_printf(s, "# Addr: 0x%llx \t 0x%llx\n",
		   p_base + CBM_QPUSH_BASE,
		   p_base + CBM_TX_CREDIT_BASE);
	seq_printf(s, "# Val:  0x%08x \t 0x%08x\n\n",
		   (cbm_r32(ctrl_base + CBM_QPUSH_BASE)),
		   (cbm_r32(ctrl_base + CBM_TX_CREDIT_BASE)));
	seq_puts(s, "# Name: CPU_POOL_ADDR \t CPU_POOL_NUM \t CPU_POOL_CNT\n");
	seq_printf(s, "# Addr: 0x%llx \t 0x%llx \t 0x%llx\n",
		   p_base + CBM_CPU_POOL_BUF_RTRN_START_ADDR,
		   p_base + CBM_CPU_POOL_BUF_ALW_NUM,
		   p_base + CBM_CPU_POOL_ENQ_CNT);
	seq_printf(s, "# Val:  0x%08x \t 0x%08x \t 0x%08x\n\n",
		   cbm_r32(ctrl_base + CBM_CPU_POOL_BUF_RTRN_START_ADDR),
		   cbm_r32(ctrl_base + CBM_CPU_POOL_BUF_ALW_NUM),
		   cbm_r32(ctrl_base + CBM_CPU_POOL_ENQ_CNT));
	seq_puts(s, "# Name: CPU_POOL_BUF \t CPU_POOL_ENQ_DEC\n");
	seq_printf(s, "# Addr: 0x%llx \t 0x%llx\n",
		   p_base + CBM_CPU_POOL_BUF_RTRN_CNT,
		   p_base + CBM_CPU_POOL_ENQ_DEC);
	seq_printf(s, "# Val:  0x%08x \t 0x%08x\n\n",
		   cbm_r32(ctrl_base + CBM_CPU_POOL_BUF_RTRN_CNT),
		   cbm_r32(ctrl_base + CBM_CPU_POOL_ENQ_DEC));
	return 0;
}

static int cqm_ctrl_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cqm_ctrl_seq_read, inode->i_private);
}

static const struct file_operations cqm_ctrl_fops = {
	.open = cqm_ctrl_open,
	.read = seq_read,
	.write = cqm_ctrl_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static inline void disp_deq_pon_reg(void *deq_base, u32 pid)
{
	u32 desc = 0, dw = 0;
	void *deq_phy_base = cqm_get_deq_phy_base();
	phys_addr_t p_base = (phys_addr_t)deq_phy_base;

	PRINTK("# Name: CFG_PON_EGP_%02d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_PON_PORT(pid, cfg),
	       cbm_r32(deq_base +  DQ_PON_PORT(pid, cfg)));
	PRINTK("# Name: DQPC_PON_EGP_%02d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_PON_PORT(pid, dqpc),
	       cbm_r32(deq_base +  DQ_PON_PORT(pid, dqpc)));
	PRINTK("# Name: IRNCR_PON_EGP_%02d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_PON_PORT(pid, irncr),
	       cbm_r32(deq_base +  DQ_PON_PORT(pid, irncr)));
	PRINTK("# Name: IRNICR_PON_EGP_%02d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_PON_PORT(pid, irnicr),
	       cbm_r32(deq_base +  DQ_PON_PORT(pid, irnicr)));
	PRINTK("# Name: IRNEN_PON_EGP_%02d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_PON_PORT(pid, irnen),
	       cbm_r32(deq_base +  DQ_PON_PORT(pid, irnen)));
	PRINTK("# Name: DPTR_PON_EGP_%02d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_PON_PORT(pid, dptr),
	       cbm_r32(deq_base +  DQ_PON_PORT(pid, dptr)));
	if (pid == 75) {
		PRINTK("#Name: BPRC_PON_EGP_%2d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
		       pid, p_base + DQ_PON_PORT(pid, bprc),
		       cbm_r32(deq_base +  DQ_PON_PORT(pid, bprc)));
	}
	PRINTK("# Name: PTR_RTN_DW2_PON_%02d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_PON_PORT(pid, ptr_rtn_dw2d0),
	       cbm_r32(deq_base +  DQ_PON_PORT(pid, ptr_rtn_dw2d0)));
	PRINTK("# Name: PTR_RTN_PON_DW3_%02d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_PON_PORT(pid, ptr_rtn_dw3d0),
	       cbm_r32(deq_base +  DQ_PON_PORT(pid, ptr_rtn_dw3d0)));
	for (desc = 0; desc < 8; desc++) {
		for (dw = 0; dw < 4; dw++) {
			PRINTK("# Name: DESC%0d_%d_PON_EGP_%02d\tAddr: 0x%llx\t"
			       "Val:  0x%08x\n", dw, desc, pid, (p_base) +
			       (dw * 4) + DQ_PON_PORT(pid, desc[desc].desc0),
			       cbm_r32(deq_base + (dw * 4) +
			       DQ_PON_PORT(pid, desc[desc].desc0)));
		}
	}
	for (desc = 0; desc < 8; desc++) {
		for (dw = 0; dw < 4; dw++) {
			PRINTK("# Name: DESC%d_%d_PON_EGP_S_%2d\tAddr: 0x%llx\t"
			       "Val:  0x%08x\n", dw, desc, pid, (p_base) +
			       (dw * 4) + DQ_PON_PORT(pid, segdesc[desc].desc0),
			       cbm_r32(deq_base + (dw * 4) +
			       DQ_PON_PORT(pid, segdesc[desc].desc0)));
		}
	}
}

static inline void disp_deq_dma_reg(void *deq_base, u32 pid)
{
	void *deq_phy_base = cqm_get_deq_phy_base();
	phys_addr_t p_base = (phys_addr_t)deq_phy_base;

	PRINTK("# Name: CFG_DMA_EGP_%02d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_DMA_PORT(pid, cfg),
	       cbm_r32(deq_base +  DQ_DMA_PORT(pid, cfg)));
	PRINTK("# Name: DQPC_DMA_EGP_%02d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_DMA_PORT(pid, dqpc),
	       cbm_r32(deq_base +  DQ_DMA_PORT(pid, dqpc)));
	PRINTK("# Name: IRNCR_DMA_EGP_%02d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_DMA_PORT(pid, irncr),
	       cbm_r32(deq_base +  DQ_DMA_PORT(pid, irncr)));
	PRINTK("# Name: IRNICR_DMA_EGP_%02d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_DMA_PORT(pid, irnicr),
	       cbm_r32(deq_base +  DQ_DMA_PORT(pid, irnicr)));
	PRINTK("# Name: IRNEN_DMA_EGP_%02d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_DMA_PORT(pid, irnen),
	       cbm_r32(deq_base +  DQ_DMA_PORT(pid, irnen)));
	PRINTK("# Name: DPTR_DMA_EGP_%02d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_DMA_PORT(pid, dptr),
	       cbm_r32(deq_base +  DQ_DMA_PORT(pid, dptr)));
}

static inline void disp_deq_voice_reg(void *deq_base, u32 pid)
{
	void *deq_phy_base = cqm_get_deq_phy_base();
	phys_addr_t p_base = (phys_addr_t)deq_phy_base;

	PRINTK("# Name: CFG_ACA_EGP_%02d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_VOICE_PORT(pid, cfg),
	       cbm_r32(deq_base +  DQ_VOICE_PORT(pid, cfg)));
	PRINTK("# Name: DQPC_CPU_EGP_%02d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_VOICE_PORT(pid, dqpc),
	       cbm_r32(deq_base +  DQ_VOICE_PORT(pid, dqpc)));
	PRINTK("# Name: IRNCR_WCPU_EGP_%02d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_VOICE_PORT(pid, irncr),
	       cbm_r32(deq_base +  DQ_VOICE_PORT(pid, irncr)));
	PRINTK("# Name: IRNICR_WCPU_EGP_%02d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_VOICE_PORT(pid, irnicr),
	       cbm_r32(deq_base +  DQ_VOICE_PORT(pid, irnicr)));
	PRINTK("# Name: IRNEN_WCPU_EGP_%02d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_VOICE_PORT(pid, irnen),
	       cbm_r32(deq_base +  DQ_VOICE_PORT(pid, irnen)));
	PRINTK("# Name: DPTR_VCPU_EGP_%02d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_VOICE_PORT(pid, dptr),
	       cbm_r32(deq_base +  DQ_VOICE_PORT(pid, dptr)));
	PRINTK("# Name: BPRC_CPU_EGP_%02d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_VOICE_PORT(pid, bprc),
	       cbm_r32(deq_base +  DQ_VOICE_PORT(pid, bprc)));
	PRINTK("# Name: BRPTR_VCPU_EGP_%02d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_VOICE_PORT(pid, brptr),
	       cbm_r32(deq_base +  DQ_VOICE_PORT(pid, brptr)));
	PRINTK("# Name: PTR_RTN_CPU_DW2_%02d_0\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_VOICE_PORT(pid, ptr_rtn_dw2d0),
	       cbm_r32(deq_base +  DQ_VOICE_PORT(pid, ptr_rtn_dw2d0)));
	PRINTK("# Name: PTR_RTN_CPU_DW3_%02d_0\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_VOICE_PORT(pid, ptr_rtn_dw3d0),
	       cbm_r32(deq_base +  DQ_VOICE_PORT(pid, ptr_rtn_dw3d0)));
	PRINTK("# Name: PTR_RTN_CPU_DW2_%02d_1\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_VOICE_PORT(pid, ptr_rtn_dw2d1),
	       cbm_r32(deq_base +  DQ_VOICE_PORT(pid, ptr_rtn_dw2d1)));
	PRINTK("# Name: PTR_RTN_CPU_DW3_%02d_1\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_VOICE_PORT(pid, ptr_rtn_dw3d1),
	       cbm_r32(deq_base +  DQ_VOICE_PORT(pid, ptr_rtn_dw3d1)));
	PRINTK("# Name: PTR_RTN_CPU_DW2_%02d_2\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_VOICE_PORT(pid, ptr_rtn_dw2d2),
	       cbm_r32(deq_base +  DQ_VOICE_PORT(pid, ptr_rtn_dw2d2)));
	PRINTK("# Name: PTR_RTN_CPU_DW3_%02d_2\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_VOICE_PORT(pid, ptr_rtn_dw3d2),
	       cbm_r32(deq_base +  DQ_VOICE_PORT(pid, ptr_rtn_dw3d2)));
	PRINTK("# Name: PTR_RTN_ACA_DW2_%02d_3\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_VOICE_PORT(pid, ptr_rtn_dw2d3),
	       cbm_r32(deq_base +  DQ_VOICE_PORT(pid, ptr_rtn_dw2d3)));
	PRINTK("# Name: PTR_RTN_ACA_DW3_%02d_3\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_VOICE_PORT(pid, ptr_rtn_dw3d3),
	       cbm_r32(deq_base +  DQ_VOICE_PORT(pid, ptr_rtn_dw3d3)));
}

static inline void disp_deq_aca_reg(void *deq_base, u32 pid)
{
	void *deq_phy_base = cqm_get_deq_phy_base();
	phys_addr_t p_base = (phys_addr_t)deq_phy_base;

	PRINTK("# Name: CFG_ACA_EGP_%02d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_DC_PORT(pid, cfg),
	       cbm_r32(deq_base +  DQ_DC_PORT(pid, cfg)));
	PRINTK("# Name: DQPC_CPU_EGP_%02d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_DC_PORT(pid, dqpc),
	       cbm_r32(deq_base +  DQ_DC_PORT(pid, dqpc)));
	PRINTK("# Name: IRNCR_WCPU_EGP_%02d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_DC_PORT(pid, irncr),
	       cbm_r32(deq_base +  DQ_DC_PORT(pid, irncr)));
	PRINTK("# Name: IRNICR_WCPU_EGP_%02d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_DC_PORT(pid, irnicr),
	       cbm_r32(deq_base +  DQ_DC_PORT(pid, irnicr)));
	PRINTK("# Name: IRNEN_WCPU_EGP_%02d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_DC_PORT(pid, irnen),
	       cbm_r32(deq_base +  DQ_DC_PORT(pid, irnen)));
	PRINTK("# Name: DPTR_CPU_EGP_%02d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_DC_PORT(pid, dptr),
	       cbm_r32(deq_base +  DQ_DC_PORT(pid, dptr)));
	PRINTK("# Name: BPRC_CPU_EGP_%d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_DC_PORT(pid, bprc),
	       cbm_r32(deq_base + DQ_DC_PORT(pid, bprc)));
	PRINTK("# Name: BRPTR_SCPU_EGP_%02d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_DC_PORT(pid, brptr),
	       cbm_r32(deq_base +  DQ_DC_PORT(pid, brptr)));
	PRINTK("# Name: PTR_RTN_ACA_DW2_EGP_%02d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_DC_PORT(pid, ptr_rtn_dw2d0),
	       cbm_r32(deq_base +  DQ_DC_PORT(pid, ptr_rtn_dw2d0)));
	PRINTK("# Name: PTR_RTN_ACA_DW3_EGP_%02d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_DC_PORT(pid, ptr_rtn_dw3d0),
	       cbm_r32(deq_base +  DQ_DC_PORT(pid, ptr_rtn_dw3d0)));
}

static inline void disp_deq_cpu_reg(void *deq_base, u32 pid)
{
	u32 desc = 0, dw = 0;
	void *deq_phy_base = cqm_get_deq_phy_base();
	phys_addr_t p_base = (phys_addr_t)deq_phy_base;

	PRINTK("# Name: CFG_CPU_EGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_CPU_PORT(pid, cfg),
	       cbm_r32(deq_base + DQ_CPU_PORT(pid, cfg)));
	PRINTK("# Name: DQPC_CPU_EGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_CPU_PORT(pid, dqpc),
	       cbm_r32(deq_base + DQ_CPU_PORT(pid, dqpc)));
	PRINTK("# Name: IRNCR_CPU_EGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_CPU_PORT(pid, irncr),
	       cbm_r32(deq_base + DQ_CPU_PORT(pid, irncr)));
	PRINTK("# Name: IRNICR_CPU_EGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_CPU_PORT(pid, irnicr),
	       cbm_r32(deq_base + DQ_CPU_PORT(pid, irnicr)));
	PRINTK("# Name: IRNEN_CPU_EGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_CPU_PORT(pid, irnen),
	       cbm_r32(deq_base + DQ_CPU_PORT(pid, irnen)));
	PRINTK("# Name: DPTR_CPU_EGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_CPU_PORT(pid, dptr),
	       cbm_r32(deq_base + DQ_CPU_PORT(pid, dptr)));
	PRINTK("# Name: BPRC_CPU_EGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_CPU_PORT(pid, bprc),
	       cbm_r32(deq_base + DQ_CPU_PORT(pid, bprc)));
	PRINTK("# Name: PTR_RTN_CPU_DW2_EGP_%d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_CPU_PORT(pid, ptr_rtn_dw2d0),
	       cbm_r32(deq_base + DQ_CPU_PORT(pid, ptr_rtn_dw2d0)));
	PRINTK("# Name: PTR_RTN_CPU_DW3_EGP_%d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + DQ_CPU_PORT(pid, ptr_rtn_dw3d0),
	       cbm_r32(deq_base + DQ_CPU_PORT(pid, ptr_rtn_dw3d0)));
	for (desc = 0; desc < 2; desc++) {
		for (dw = 0; dw < 4; dw++) {
			PRINTK("# Name: DESC%0d_%0dCPU_EGP_%02d\tAddr: 0x%llx\t"
			       "Val:  0x%08x\n", dw, desc, pid, (p_base) +
			       (dw * 4) + DQ_CPU_PORT(pid, desc[desc].desc0),
			       cbm_r32(deq_base + (dw * 4) +
			       DQ_CPU_PORT(pid, desc[desc].desc0)));
		}
	}
}

static ssize_t cqm_deq_write(struct file *file, const char __user *buf,
			     size_t count, loff_t *ppos)
{
	u32 input = 0, num = 0;
	int len = 0;
	char str[64], *p = (char *)str, *param_list[5] = { 0 };
	void *deq_base = cqm_get_deq_base();
	void *deq_phy_base = cqm_get_deq_phy_base();
	phys_addr_t p_base = (phys_addr_t)deq_phy_base;
	struct dqm_pid_s *dq_id_p = cqm_get_dqm_pid_cfg();

	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;
	if (!len)
		return count;

	#ifndef KERNEL_5_15_DEP_MODULE_NOT_PORTED
	num = dp_split_buffer(p, param_list, ARRAY_SIZE(param_list));
	#endif
	if (num == 1 && (strncmp(param_list[0], "help", 4) == 0)) {
		PRINTK("echo port_num > /sys/kernel/debug/cqm/cqmdeq\n");
		PRINTK("It will display if the port_num <= 138\n");
		PRINTK("CPU=0~18\tACA/DC=19~26\tDMA=27~74\tPON=75~138\n");
	} else if (num == 1) {
		#ifndef KERNEL_5_15_DEP_MODULE_NOT_PORTED
		input = dp_atoi(param_list[0]);
		#endif
		PRINTK("# Name: CBM_DQM_CTRL\t\tAddr: 0x%llx\tVal: 0x%8x\n",
		       p_base + CBM_DQM_CTRL,
		       cbm_r32(deq_base + CBM_DQM_CTRL));
		if (input >= dq_id_p->cpu_start &&
		    input <= dq_id_p->lro_end) {
			disp_deq_cpu_reg(deq_base, input);
		} else if ((input >= dq_id_p->aca_start) &&
			 (input <= dq_id_p->aca_end)) {
			disp_deq_aca_reg(deq_base, input);
		} else if ((input >= dq_id_p->voice_start) &&
			 (input <= dq_id_p->voice_end)) {
			disp_deq_voice_reg(deq_base, input);
		} else if ((input >= dq_id_p->dma2_start) &&
			 (input <= dq_id_p->dma0_end)) {
			disp_deq_dma_reg(deq_base, input);
		} else if ((input >= dq_id_p->pon_start) &&
			 (input <= dq_id_p->pon_end)) {
			disp_deq_pon_reg(deq_base, input);
		} else if ((input >= dq_id_p->docsis_start) &&
			 (input <= dq_id_p->docsis_end)) {
			disp_deq_pon_reg(deq_base, input);
		} else {
			PRINTK("Wrong Param(try): see help\n");
			return count;
		}
	} else {
		PRINTK("Wrong Param(try): see help");
	}
	return count;
}

static int cqm_deq_seq_read(struct seq_file *s, void *v)
{
	void *deq_base = cqm_get_deq_base();
	void *deq_phy_base = cqm_get_deq_phy_base();
	phys_addr_t p_base = (phys_addr_t)deq_phy_base;

	seq_puts(s, "# Name: CBM_DQM_CTRL\t");
	seq_printf(s, "Addr: 0x%llx\t", p_base);
	seq_printf(s, "Val:  0x%08x\n", cbm_r32(deq_base));
	return 0;
}

static int cqm_deq_seq_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cqm_deq_seq_read, inode->i_private);
}

static const struct file_operations cqm_deq_fops = {
	.open = cqm_deq_seq_open,
	.read = seq_read,
	.write = cqm_deq_write,
	.llseek = seq_lseek,
	.release = seq_release,
};

static inline void disp_enq_help(void)
{
	PRINTK("To display CQM enq register try\n");
	PRINTK("echo port_num > /sys/kernel/debug/cqm/cqmenq\n");
	PRINTK("To display CQM enq ctrl registers try\n");
	PRINTK("echo ctrl > /sys/kernel/debug/cqm/cqmenq\n");
	PRINTK("To display CQM enq ovh registers try\n");
	PRINTK("echo ovh > /sys/kernel/debug/cqm/cqmenq\n");
	PRINTK("To display CQM enq threshold enable registers try\n");
	PRINTK("echo thres_en > /sys/kernel/debug/cqm/cqmenq\n");
}

static inline void disp_enq_thres(void *enq_base)
{
	u32 pid = 0, end = 8;
	void *enq_phy_base = cqm_get_enq_phy_base();
	phys_addr_t p_base = (phys_addr_t)enq_phy_base;

	enq_base += EP_THRSD_EN_0;
	for (pid = 0; pid < end; pid++) {
		PRINTK("# EP_THRD_EN_%02d\t Addr: %0llx\t Val: %02d\n", pid,
		       p_base + (pid * 4), cbm_r32(enq_base + (pid * 4)));
	}
}

static inline void disp_enq_ovh(void *enq_base)
{
	u32 pid = 0, end = 16;
	void *enq_phy_base = cqm_get_enq_phy_base();
	phys_addr_t p_base = (phys_addr_t)enq_phy_base;

	enq_base += OVH_0;
	for (pid = 0; pid < end; pid++) {
		PRINTK("\n# OVH_%02d\t Addr: %0llx\t Val: %02d\n", pid,
		       p_base + (pid * 4), cbm_r32(enq_base + (pid * 4)));
	}
}

static inline void disp_enq_cpu_reg(void *enq_base, int pid)
{
	u32 desc = 0, dw = 0;
	void *enq_phy_base = cqm_get_enq_phy_base();
	phys_addr_t p_base = (phys_addr_t)enq_phy_base;

	PRINTK("# Name: CFG_CPU_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_CPU_PORT(pid, cfg),
	       cbm_r32(enq_base + EQ_CPU_PORT(pid, cfg)));
	PRINTK("# Name: EQPC_CPU_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_CPU_PORT(pid, eqpc),
	       cbm_r32(enq_base + EQ_CPU_PORT(pid, eqpc)));
	for (dw = 0; dw < 4; dw++) {
		PRINTK("# Name: DISC_DESC%d_CPU_IGP_%d\tAddr: 0x%llx\t"
		       "Val: 0x%08x\n", dw, pid,
		       p_base + (dw * 4) + EQ_CPU_PORT(pid, disc.desc0),
		       cbm_r32(enq_base + (dw * 4) +
		       EQ_CPU_PORT(pid, disc.desc0)));
	}
	PRINTK("# Name: IRNICR_CPU_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_CPU_PORT(pid, irnicr),
	       cbm_r32(enq_base + EQ_CPU_PORT(pid, irnicr)));
	PRINTK("# Name: IRNEN_CPU_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_CPU_PORT(pid, irnen),
	       cbm_r32(enq_base + EQ_CPU_PORT(pid, irnen)));
	PRINTK("# Name: DICC_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_CPU_PORT(pid, dicc),
	       cbm_r32(enq_base + EQ_CPU_PORT(pid, dicc)));
	PRINTK("# Name: DROP_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_CPU_PORT(pid, drop),
	       cbm_r32(enq_base + EQ_CPU_PORT(pid, drop)));
	PRINTK("# Name: NPBPC_CPU_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_CPU_PORT(pid, npbpc),
	       cbm_r32(enq_base + EQ_CPU_PORT(pid, npbpc)));
	PRINTK("# Name: NS0PC_CPU_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_CPU_PORT(pid, ns0pc),
	       cbm_r32(enq_base + EQ_CPU_PORT(pid, ns0pc)));
	PRINTK("# Name: NS1PC_CPU_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_CPU_PORT(pid, ns1pc),
	       cbm_r32(enq_base + EQ_CPU_PORT(pid, ns1pc)));
	PRINTK("# Name: NS2PC_CPU_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_CPU_PORT(pid, ns2pc),
	       cbm_r32(enq_base + EQ_CPU_PORT(pid, ns2pc)));
	PRINTK("# Name: NS3PC_CPU_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_CPU_PORT(pid, ns3pc),
	       cbm_r32(enq_base + EQ_CPU_PORT(pid, ns3pc)));
	PRINTK("# Name: DCNTR_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_CPU_PORT(pid, dcntr),
	       cbm_r32(enq_base + EQ_CPU_PORT(pid, dcntr)));
	for (desc = 0; desc < 2; desc++) {
		for (dw = 0; dw < 4; dw++) {
			PRINTK("# Name: DESC%0d_%0d_CPU_IGP_%2d\tAddr: 0x%llx\t"
			       "Val:  0x%08x\n", desc, dw, pid,
			       p_base + (desc * 16) + (dw * 4) +
			       EQ_CPU_PORT(pid, desc0.desc0),
			       cbm_r32(enq_base + (desc * 16) + (dw * 4) +
			       EQ_CPU_PORT(pid, desc0.desc0)));
		}
	}
}

static inline void disp_enq_dc_reg(void *enq_base, int pid)
{
	u32 dw = 0;
	void *enq_phy_base = cqm_get_enq_phy_base();
	phys_addr_t p_base = (phys_addr_t)enq_phy_base;

	PRINTK("# Name: CFG_DC_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DC_PORT(pid, cfg),
	       cbm_r32(enq_base + EQ_DC_PORT(pid, cfg)));
	PRINTK("# Name: DESC_CONV_DC_IGP_%d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DC_PORT(pid, desc_conv),
	       cbm_r32(enq_base + EQ_DC_PORT(pid, desc_conv)));
	PRINTK("# Name: EQPC_DC_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DC_PORT(pid, eqpc),
	       cbm_r32(enq_base + EQ_DC_PORT(pid, eqpc)));
	for (dw = 0; dw < 4; dw++) {
		PRINTK("# Name: DISC_DESC%d_DC_IGP_%d\tAddr: 0x%llx\t"
		       "Val: 0x%08x\n", pid, dw,
		       p_base + (dw * 4) + EQ_DC_PORT(pid, disc.desc0),
		       cbm_r32(enq_base + (dw * 4) +
		       EQ_DC_PORT(pid, disc.desc0)));
	}
	PRINTK("# Name: IRNCR_DC_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DC_PORT(pid, irncr),
	       cbm_r32(enq_base + EQ_DC_PORT(pid, irncr)));
	PRINTK("# Name: IRNICR_DC_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DC_PORT(pid, irnicr),
	       cbm_r32(enq_base + EQ_DC_PORT(pid, irnicr)));
	PRINTK("# Name: IRNEN_DC_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DC_PORT(pid, irnen),
	       cbm_r32(enq_base + EQ_DC_PORT(pid, irnen)));
	PRINTK("# Name: DPTR_DC_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DC_PORT(pid, dptr),
	       cbm_r32(enq_base + EQ_DC_PORT(pid, dptr)));
	PRINTK("# Name: DROP_DC_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DC_PORT(pid, drop),
	       cbm_r32(enq_base + EQ_DC_PORT(pid, drop)));
	PRINTK("# Name: NBPC_DC_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DC_PORT(pid, npbpc),
	       cbm_r32(enq_base + EQ_DC_PORT(pid, npbpc)));
	PRINTK("# Name: DEVQOS_Q0_7_DC_IGP_%d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DC_PORT(pid, devqmap0to7),
	       cbm_r32(enq_base + EQ_DC_PORT(pid, devqmap0to7)));
	PRINTK("# Name: DEVQOS_Q8_15_DC_IGP_%d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DC_PORT(pid, devqmap8to15),
	       cbm_r32(enq_base + EQ_DC_PORT(pid, devqmap8to15)));
	PRINTK("# Name: DCNTR_DC_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DC_PORT(pid, dcntr),
	       cbm_r32(enq_base + EQ_DC_PORT(pid, dcntr)));
}

static inline void disp_enq_dma_reg(void *enq_base, int pid)
{
	u32 dw = 0;
	void *enq_phy_base = cqm_get_enq_phy_base();
	phys_addr_t p_base = (phys_addr_t)enq_phy_base;

	PRINTK("# Name: CFG_DMA_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DMA_PORT(pid, cfg),
	       cbm_r32(enq_base + EQ_DMA_PORT(pid, cfg)));
	PRINTK("# Name: EQPC_DMA_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DMA_PORT(pid, eqpc),
	       cbm_r32(enq_base + EQ_DMA_PORT(pid, eqpc)));
	for (dw = 0; dw < 4; dw++) {
		PRINTK("# Name: DISC_DESC%d_DMA_IGP_%d\tAddr: 0x%llx\t"
		       "Val:  0x%8x\n", dw, pid, p_base + (dw * 4) +
			EQ_DMA_PORT(pid, disc.desc0), cbm_r32(enq_base +
			(dw * 4) + EQ_DMA_PORT(pid, disc.desc0)));
	}
	PRINTK("# Name: IRNCR_DMA_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DMA_PORT(pid, irncr),
	       cbm_r32(enq_base + EQ_DMA_PORT(pid, irncr)));
	PRINTK("# Name: IRNICR_DMA_IGP_%d\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DMA_PORT(pid, irnicr),
	       cbm_r32(enq_base + EQ_DMA_PORT(pid, irnicr)));
	PRINTK("# Name: IRNEN_DMA_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DMA_PORT(pid, irnen),
	       cbm_r32(enq_base + EQ_DMA_PORT(pid, irnen)));
	PRINTK("# Name: DPTR_DMA_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DMA_PORT(pid, dptr),
	       cbm_r32(enq_base + EQ_DMA_PORT(pid, dptr)));
	PRINTK("# Name: DICC_DMA_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DMA_PORT(pid, dicc),
	       cbm_r32(enq_base + EQ_DMA_PORT(pid, dicc)));
	PRINTK("# Name: DROP_DMA_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DMA_PORT(pid, drop),
	       cbm_r32(enq_base + EQ_DMA_PORT(pid, drop)));
	PRINTK("# Name: DCNTR_DMA_IGP_%d\t\tAddr: 0x%llx\tVal: 0x%08x\n",
	       pid, p_base + EQ_DMA_PORT(pid, dcntr),
	       cbm_r32(enq_base + EQ_DMA_PORT(pid, dcntr)));
	for (dw = 0; dw < 4; dw++) {
		PRINTK("# Name: DROP_DESC%d_DMA_IGP_%d\tAddr: 0x%llx\t"
		       "Val:  0x%8x\n", dw, pid, p_base + (dw * 4) +
			EQ_DMA_PORT(pid, dropdesc.desc0),
			cbm_r32(enq_base + (dw * 4) +
			EQ_DMA_PORT(pid, dropdesc.desc0)));
	}
}

static ssize_t cqm_enq_write(struct file *file, const char __user *buf,
			     size_t count, loff_t *ppos)
{
	u32 input = 0, num = 0;
	int len;
	char str[64], *p = (char *)str, *param_list[5] = { 0 };
	void *enq_base = cqm_get_enq_base();
	void *enq_phy_base = cqm_get_enq_phy_base();
	phys_addr_t p_base = (phys_addr_t)enq_phy_base;
	struct eqm_pid_s *enq_id_p = cqm_get_eqm_pid_cfg();

	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;
	if (!len)
		return count;
	num = dp_split_buffer(p, param_list, ARRAY_SIZE(param_list));
	if (num == 1 && (strncmp(param_list[0], "help", 4) == 0)) {
		disp_enq_help();
	} else if (num == 1 && (strncmp(param_list[0], "ctrl", 4) == 0)) {
		PRINTK("# CQEM_ENQ_BASE=0x%llx\t ", p_base);
		PRINTK("# Name: CBM_EQM_CTRL\tAddr: 0x%llx\tVal: 0x%08x\n",
		       p_base + CBM_EQM_CTRL,
		       cbm_r32(enq_base + CBM_EQM_CTRL));
	} else if (num == 1 && (strncmp(param_list[0], "ovh", 3) == 0)) {
		disp_enq_ovh(enq_base);
	} else if (num == 1 && (strncmp(param_list[0], "thres_en", 8) == 0)) {
		disp_enq_thres(enq_base);
	} else if (num == 1) {
		input = dp_atoi(param_list[0]);
		if (input >= enq_id_p->cpu_start && input <=
			enq_id_p->qosbp_end) {
			disp_enq_cpu_reg(enq_base, input);
		} else if (input >= enq_id_p->aca_start && input <=
			enq_id_p->aca_end) {
			disp_enq_dc_reg(enq_base, input);
		} else if (input >= enq_id_p->dma0_start &&
			 input <= enq_id_p->rxdma) {
			disp_enq_dma_reg(enq_base, input);
		} else {
			PRINTK("\nWrong port num (try again[0-33])\n");
			return count;
		}
	} else {
		PRINTK("\nWrong Param(try):\n");
		PRINTK("echo port > /sys/kernel/debug/cqm/cqmenq\n");
	}
	return count;
}

static int cqm_enq_seq_read(struct seq_file *s, void *v)
{
	void *enq_base = cqm_get_enq_base();
	void *enq_phy_base = cqm_get_enq_phy_base();
	phys_addr_t p_base = (phys_addr_t)enq_phy_base;

	seq_puts(s, "\n# Name: CBM_EQM_CTRL \t");
	seq_printf(s, "Addr: 0x%llx\t", p_base + CBM_EQM_CTRL);
	seq_printf(s, "Val:  0x%08x\n", cbm_r32(enq_base + CBM_EQM_CTRL));
	return 0;
}

static int cqm_enq_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cqm_enq_seq_read, inode->i_private);
}

static const struct file_operations cqm_enq_fops = {
	.open = cqm_enq_open,
	.read = seq_read,
	.write = cqm_enq_write,
	.llseek = seq_lseek,
	.release = seq_release,
};

static ssize_t cqm_dqpc_write(struct file *file, const char __user *buf,
			      size_t count, loff_t *ppos)
{
	int len, num = 0;
	char str[64], *p = (char *)str, *param_list[5] = { 0 };

	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;

	if (!len)
		return count;

	#ifndef KERNEL_5_15_DEP_MODULE_NOT_PORTED
	num = dp_split_buffer(p, param_list, ARRAY_SIZE(param_list));
	#endif
	if (num == 1 && (strncmp(param_list[0], "help", 4) == 0))
		PRINTK("cat /sys/kernel/debug/cqm/cqmdqpc\n");
	return count;
}

static int cqm_dqpc_seq_read(struct seq_file *s, void *v)
{
	u32 pid = 0;
	u32 start = 0;
	u32 end = 0;
	void *deq_base = cqm_get_deq_base();
	void *p_base = cqm_get_deq_phy_base();
	struct dqm_pid_s *dq_id_p = cqm_get_dqm_pid_cfg();

	start = dq_id_p->cpu_start; end = dq_id_p->voice_end;
	for (pid = start; pid <= end; pid++) {
		if (cbm_r32(deq_base + DQ_CPU_PORT(pid, dqpc))) {
			seq_printf(s, "# DQPC_CPU_EGP_%02d\t", pid);
			seq_printf(s, "# Addr: %pS\t",
				   p_base + DQ_CPU_PORT(pid, dqpc));
			seq_printf(s, "# Val: 0x%08x\n",
				   cbm_r32(deq_base + DQ_CPU_PORT(pid, dqpc)));
		}
	}
	start = dq_id_p->aca_start; end = dq_id_p->aca_end;
	for (pid = start; pid <= end; pid++) {
		if (cbm_r32(deq_base + DQ_DC_PORT(pid, dqpc))) {
			seq_printf(s, "# DQPC_DC_EGP_%02d\t", pid);
			seq_printf(s, "# Addr: %pS\t",
				   p_base + DQ_DC_PORT(pid, dqpc));
			seq_printf(s, "# Val: 0x%08x\n",
				   cbm_r32(deq_base + DQ_DC_PORT(pid, dqpc)));
		}
	}
	start = dq_id_p->dma2_start; end = dq_id_p->dma0_end;
	for (pid = start; pid <= end; pid++) {
		if (cbm_r32(deq_base + DQ_DMA_PORT(pid, dqpc))) {
			seq_printf(s, "# DQPC_DMA_EGP_%02d\t", pid);
			seq_printf(s, "# Addr: %pS\t",
				   p_base + DQ_DMA_PORT(pid, dqpc));
			seq_printf(s, "# Val: 0x%08x\n", cbm_r32(deq_base +
				   DQ_DMA_PORT(pid, dqpc)));
		}
	}
	start = dq_id_p->pon_start; end = dq_id_p->pon_end;
	for (pid = start; pid <= end; pid++) {
		if (cbm_r32(deq_base + DQ_PON_PORT(pid, dqpc))) {
			seq_printf(s, "# DQPC_PON_EGP_%02d\t", pid);
			seq_printf(s, "# Addr: %pS\t",
				   p_base + DQ_PON_PORT(pid, dqpc));
			seq_printf(s, "# Val: 0x%08x\n",
				   cbm_r32(deq_base + DQ_PON_PORT(pid, dqpc)));
		}
	}
	start = dq_id_p->docsis_start; end = dq_id_p->docsis_end;
	for (pid = start; pid <= end; pid++) {
		if (cbm_r32(deq_base + DQ_PON_PORT(pid, dqpc))) {
			seq_printf(s, "# DQPC_PON_EGP_%02d\t", pid);
			seq_printf(s, "# Addr: %pS\t",
				   p_base + DQ_PON_PORT(pid, dqpc));
			seq_printf(s, "# Val: 0x%08x\n",
				   cbm_r32(deq_base + DQ_PON_PORT(pid, dqpc)));
		}
	}
	return 0;
}

static int cqm_dqpc_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cqm_dqpc_seq_read, inode->i_private);
}

static const struct file_operations cqm_dqpc_fops = {
	.open = cqm_dqpc_open,
	.read = seq_read,
	.write = cqm_dqpc_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static ssize_t cqm_eqpc_write(struct file *file, const char __user *buf,
			      size_t count, loff_t *ppos)
{
	int len, num = 0;
	char str[64], *p = (char *)str, *param_list[5] = { 0 };

	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;
	if (!len)
		return count;

	#ifndef KERNEL_5_15_DEP_MODULE_NOT_PORTED
	num = dp_split_buffer(p, param_list, ARRAY_SIZE(param_list));
	#endif
	if (num == 1 && (strncmp(param_list[0], "help", 4) == 0))
		PRINTK("Help: cat /sys/kernel/debug/cqm/cqmeqpc\n");
	return count;
}

static int cqm_eqpc_seq_read(struct seq_file *s, void *v)
{
	u32 pid = 0, start = 0, end = 0;
	void *enq_base = cqm_get_enq_base();
	void *p_base = cqm_get_enq_phy_base();
	struct eqm_pid_s *enq_id_p = cqm_get_eqm_pid_cfg();

	start = enq_id_p->cpu_start; end = enq_id_p->qosbp_end;
	for (pid = start; pid <= end; pid++) {
		if (cbm_r32(enq_base + EQ_CPU_PORT(pid, eqpc))) {
			seq_printf(s, "# EQPC_CPU_IGP_%02d\t", pid);
			seq_printf(s, "# Addr: %pS\t",
				   p_base + EQ_CPU_PORT(pid, eqpc));
			seq_printf(s, "# Val: 0x%08x\n",
				   cbm_r32(enq_base + EQ_CPU_PORT(pid, eqpc)));
		}
	}
	start = enq_id_p->aca_start; end = enq_id_p->aca_end;
	for (pid = start; pid <= end; pid++) {
		if (cbm_r32(enq_base + EQ_DC_PORT(pid, eqpc))) {
			seq_printf(s, "# EQPC_DC_IGP_%02d\t", pid);
			seq_printf(s, "# Addr: %pS\t",
				   p_base + EQ_DC_PORT(pid, eqpc));
			seq_printf(s, "# Val: 0x%08x\n",
				   cbm_r32(enq_base + EQ_DC_PORT(pid, eqpc)));
		}
	}
	start = enq_id_p->dma0_start; end = enq_id_p->rxdma;
	for (pid = start; pid <= end; pid++) {
		if (cbm_r32(enq_base + EQ_DMA_PORT(pid, eqpc))) {
			seq_printf(s, "# EQPC_DMA_IGP_%02d\t", pid);
			seq_printf(s, "# Addr: %pS\t",
				   p_base + EQ_DMA_PORT(pid, eqpc));
			seq_printf(s, "# Val: 0x%08x\t\n",
				   cbm_r32(enq_base + EQ_DMA_PORT(pid, eqpc)));
		}
	}
	return 0;
}

static int cqm_eqpc_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cqm_eqpc_seq_read, inode->i_private);
}

static const struct file_operations cqm_eqpc_fops = {
	.open = cqm_eqpc_open,
	.read = seq_read,
	.write = cqm_eqpc_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static ssize_t cqm_ofsc_write(struct file *file, const char __user *buf,
			      size_t count, loff_t *ppos)
{
	int len, num = 0;
	char str[64], *p = (char *)str, *param_list[5] = { 0 };

	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;
	if (!len)
		return count;

	#ifndef KERNEL_5_15_DEP_MODULE_NOT_PORTED
	num = dp_split_buffer(p, param_list, ARRAY_SIZE(param_list));
	#endif

	if (num == 1 && (strncmp(param_list[0], "help", 4) == 0)) {
		PRINTK("Help: Please use:\n");
		PRINTK("cat /sys/kernel/debug/cqm/ofsc\n");
	}
	return count;
}

static int cqm_ofsc_read(struct seq_file *s, void *v)
{
	seq_printf(s, "# %8s\t", "OFSC");
	seq_printf(s, "0x%08x\n", get_fsqm_ofsc());
	return 0;
}

static int cqm_ofsc_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cqm_ofsc_read, inode->i_private);
}

static const struct file_operations cqm_ofsc_fops = {
	.open = cqm_ofsc_open,
	.read = seq_read,
	.write = cqm_ofsc_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static int cqm_get_lookup_qid_via_index(struct cbm_lookup *info)
{
	int qid;

	if (!info)
		return 0;
	if (info->index & BIT(EGFLAG_BIT))
		info->egflag = 1;
	else
		info->egflag = 0;
	info->index &= BIT(EGFLAG_BIT) - 1;
	qid = get_lookup_qid_via_index(info);
	pr_debug("get egflag=%d index=0x%x qid=%d\n",
		 info->egflag, info->index, qid);
	return qid;
}

/* arr[]  ---> Input Array
 * data[] ---> Temporary array to store current combination
 * start & end ---> Staring and Ending indexes in arr[]
 * index  ---> Current index in data[]
 * r ---> Size of a combination to be printed
 *
 */
static void combine_util(const int *arr, int *data, int start, int end,
			 int index, int r)
{
	int i;

	/* Current combination is ready to be printed, print it */
	if (left_n <= 0)
		return;
	if (index == r) {/*Find one pattern with specified don't care flag */

		check_pattern(data, r);
		/*find a don't care case and need further check */

		return;
	}
	/* replace index with all possible elements. The condition */
	/* "end-i+1 >= r-index" makes sure that including one element */
	/* at index will make a combination with remaining elements */
	/* at remaining positions */
	for (i = start; i <= end && end - i + 1 >= r - index; i++) {
		data[index] = arr[i];
		if (left_n <= 0)
			break;
		combine_util(arr, data, i + 1, end, index + 1, r);
	}
}

/* The main function that prints all combinations of size r*/
/* in arr[] of size n. This function mainly uses combine_util()*/
static void c_not_care_walkthrought(const int *arr, int n, int r)
{
	/* A temporary array data[] to store all combination one by one */

	/* Print all combination using temprary array 'data[]' */
	combine_util(arr, c_tmp_data, 0, n - 1, 0, r);
}

/*Note: when call this API, for those cared bits,
 * its value already set in tmp_index.
 */
static void lookup_pattern_match(int tmp_index)
{
	int i;
	int qid;
	static int first_qid;
	int t[LOOKUP_FIELD_BITS] = { 0 };
	int index;
	struct cbm_lookup lookup;

	pr_debug("trying with tmp_index=0x%x with lookup_match_num=%d\n",
		 tmp_index, lookup_match_num);

	pattern_match_flag = PATTERN_MATCH_INIT;
	lookup_match_index[lookup_match_num] = tmp_index;

	LIST_ALL_CASES(t, lookup_mask1, CARE_NOT_FLAG) {
		index = tmp_index;
		for (i = 0; i < LOOKUP_FIELD_BITS; i++)
			index |= (t[i] << i);

		if (lookup_tbl_flags[index] == ENTRY_USED) {
			pattern_match_flag = PATTERN_MATCH_FAIL;
			goto END;
		}
		lookup.index = index;
		lookup.egflag = 0;
		qid = cqm_get_lookup_qid_via_index(&lookup);

		if (pattern_match_flag == PATTERN_MATCH_INIT) {
			pattern_match_flag = PATTERN_MATCH_START;
			first_qid = qid;
		} else if (first_qid != qid) {
			pattern_match_flag = PATTERN_MATCH_FAIL;
			goto END;
		}
	}

END:
	/*save the result if necessary here */
	if (pattern_match_flag == PATTERN_MATCH_START) {
		/*pass since still not fail yet */
		pattern_match_flag = PATTERN_MATCH_PASS;

		/*mark the entries */
		LIST_ALL_CASES(t, lookup_mask1, CARE_NOT_FLAG) {
			index = tmp_index;
			for (i = 0; i < LOOKUP_FIELD_BITS; i++)
				index |= (t[i] << i);
			if (lookup_tbl_flags[index] == ENTRY_USED)
				pr_err("why already used\n");
			else
				lookup_tbl_flags[index] = ENTRY_USED;
		}
		/*save status */
		lookup_match_qid[lookup_match_num] = first_qid;
		lookup_match_mask[lookup_match_num] = 0;
		for (i = 0; i < LOOKUP_FIELD_BITS; i++)
			if (lookup_mask1[i])
				lookup_match_mask[lookup_match_num] |= 1 << i;
		lookup_match_num++;
		pr_debug("left_n=%d lookup_mask_n=%d. Need reduce=%d\n",
			 left_n, lookup_mask_n, (1 << lookup_mask_n));
		left_n -= (1 << lookup_mask_n);
	} else {
		/*failed */
	}
}

/*k--number of don't care flags
 */
static int list_care_combination(int tmp_index)
{
	int i, index;
	int t[14] = { 0 };

	LIST_ALL_CASES(t, lookup_mask1, CARE_FLAG) {
		index = tmp_index;
		for (i = 0; i < LOOKUP_FIELD_BITS; i++)
			index |= (t[i] << i);
		lookup_pattern_match(index);
	}

	return 0;
}

/*based on the don't care list, we try to find the all possible pattern:
 *for example: bit 13 and bit 11 don't care.
 *data---the flag index list which is don't care
 *r -- the flag index length
 */
static int check_pattern(int *data, int r)
{
	int i;

	memset(lookup_mask1, 0, sizeof(lookup_mask1));

	for (i = 0; i < r; i++)
		lookup_mask1[data[i]] = CARE_NOT_FLAG;

	lookup_mask_n = r;
	pattern_match_flag = 0;

	list_care_combination(tmp_pattern_port_id << EP_BIT);
	return 0;
}

static void cqm_print_title(struct seq_file *s, int ep, int mode, int qid)
{
	if (mode == 0)
		proc_printf(s,
			    "%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s %-6s\n",
			    "EG", "EP3",
			    "EP2", "EP1", "EP0",
			    "S13", "S12", "S11", "S10", "S9", "S8",
			    "CLS1", "CLS0",
			    "QID", "Mode0");

	else if (mode == 1)
		proc_printf(s,
			    "%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s %-6s\n",
			    "EG", "EP3",
			    "EP2", "EP1", "EP0",
			    "S7", "S6", "S5", "S4", "S3", "S2", "S1", "S0",
			    "QID", "Mode1");

	else if (mode == 2)
		proc_printf(s,
			    "%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s %-6s\n",
			    "EG", "EP3",
			    "EP2", "EP1", "EP0",
			    "S11", "S10", "S9", "S8",
			    "CLS3", "CLS2", "CLS1", "CLS0",
			    "QID", "Mode2");

	else if (mode == 3)
		proc_printf(s,
			    "%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s %-6s\n",
			    "EG", "EP3",
			    "EP2", "EP1", "EP0",
			    "S4", "S3", "S2", "S1", "S0",
			    "CLS2", "CLS1", "CLS0",
			    "QID", "Mode3");
	else if (mode == 4)
		proc_printf(s,
			    "%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s %-6s\n",
			    "EG", "EP3",
			    "EP2", "EP1", "EP0",
			    "CLS1", "CLS0",
			    "S5", "S4", "S3", "S2", "S1", "S0",
			    "QID", "Mode4");
	else if (mode == 5)
		proc_printf(s,
			    "%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s %-6s\n",
			    "EG", "EP3",
			    "EP2", "EP1", "EP0",
			    "S15", "S14", "S13", "S12",
			    "S11", "S10", "S9", "S8",
			    "QID", "Mode5");

	else if (mode == 6)
		proc_printf(s,
			    "%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s %-6s\n",
			    "EG", "EP3",
			    "EP2", "EP1", "EP0",
			    "S1", "S0",
			    "CLS3", "CLS2", "CLS1", "CLS0", "COL1", "COL0",
			    "QID", "Mode6");

	else if (mode == 7)
		proc_printf(s,
			    "%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s%5s %-6s\n",
			    "EG", "EP3",
			    "EP2", "EP1", "EP0",
			    "S14", "S13", "S12", "S11", "S10", "S9", "S8",
			    "CLS0",
			    "QID", "Mode7");
	else
		return;
}

/*qid: -1: match all queues
 *      >=0: only match the specified queue
 */

int cqm_find_pattern(int port_id, struct seq_file *s, int qid)
{
	cbm_queue_map_entry_t entry;
	int r, i, j, n, data_index, ret;
	int f = 0;
	int mode;
	static const int arr[] = {
		12, /*11, 10, 9, 8,*/ 7, 6, 5, 4, 3, 2, 1, 0 };
	char data_str[13];
	char data_d;
	int match = 0;

	entry.ep = port_id;
	ret = cqm_mode_table_get(0, &mode, &entry, 0);
	if (ret != 0)
		return ret;

	n = ARRAY_SIZE(arr);
	left_n = 1 << (LOOKUP_FIELD_BITS - 4); /* maximum lookup entries */
	lookup_match_num = 0;
	tmp_pattern_port_id = port_id;
	memset(lookup_tbl_flags, 0, sizeof(lookup_tbl_flags));

	/*list all pattern, ie, don't care numbers from 10 to 1 */
	for (r = n; r >= 0; r--) {
		if (left_n <= 0)
			break;
		c_not_care_walkthrought(arr, n, r);
		match = lookup_match_num;
		pr_debug("left_n=%d\n", left_n);

		if (!left_n)
			break;
	}

	for (i = 0; i < match; i++) {
		if (qid >= 0 && qid != lookup_match_qid[i])
			continue;
		if (!f) {
			f = 1;
			cqm_print_title(s, tmp_pattern_port_id, mode, qid);
		}
		data_index = 0;
		if (lookup_match_qid[i] == 0)
			continue;

		for (j = LOOKUP_FIELD_BITS - 1; j >= 0; j--) {
			if ((lookup_match_mask[i] >> j) & 1) {
				data_str[data_index] = 'x';
				data_index++;
			} else {
				data_d = (lookup_match_index[i] >> j & 1) +
					'0';
				data_str[data_index] = data_d;
				data_index++;
			}
		}
		proc_printf(s,
			    "%5c%5c%5c%5c%5c%5c%5c%5c%5c%5c%5c%5c%5c%5d ->(0x%04x)\n",
			    data_str[0], data_str[1], data_str[2], data_str[3],
			    data_str[4], data_str[5], data_str[6], data_str[7],
			    data_str[8], data_str[9], data_str[10],
			    data_str[11], data_str[12], lookup_match_qid[i],
			    lookup_match_index[i]);
	}
	if (s && seq_has_overflowed(s))
		return -1;

	return 0;
}

static int cqm_qid_queue_map_read(struct seq_file *s, int pos)
{
	if (cqm_find_pattern(pos, s, -1) < 0)
		return pos;
	pos++;
	if (pos >= 16)
		pos = -1;
	return pos;
}

/*return 0: get correct bit mask
 * -1: no
 */
static u8 lookup_flags2[LOOKUP_FIELD_BITS];
static u8 lookup_mask2[LOOKUP_FIELD_BITS];
int cqm_get_dont_care_lookup(char *s)
{
	int len, i, j;
	int flag = 0;

	if (!s)
		return -1;
	len = strlen(s);
	#ifndef KERNEL_5_15_DEP_MODULE_NOT_PORTED
	dp_replace_ch(s, strlen(s), ' ', 0);
	dp_replace_ch(s, strlen(s), '\r', 0);
	dp_replace_ch(s, strlen(s), '\n', 0);
	#endif
	if (s[0] == 0)
		return -1;
	memset(lookup_flags2, 0, sizeof(lookup_flags2));
	memset(lookup_mask2, 0, sizeof(lookup_mask2));
	if ((s[0] != 'b') && (s[0] != 'B'))
		return -1;

	if (len >= LOOKUP_FIELD_BITS + 1)
		len = LOOKUP_FIELD_BITS + 1;
	for (i = len - 1, j = 0; i >= 1; i--, j++) {
		if ((s[i] == 'x') || (s[i] == 'X')) {
			lookup_mask2[j] = 1;
			flag = 1;
		} else if (('0' <= s[i]) && (s[i] <= '9')) {
			lookup_flags2[j] = s[i] - '0';
		} else if (('A' <= s[i]) && (s[i] <= 'F')) {
			lookup_flags2[j] = s[i] - 'A' + 10;
		} else if (('a' <= s[i]) && (s[i] <= 'f')) {
			lookup_flags2[j] = s[i] - '1' + 10;
		} else {
			return -1;
		}
	}
	if (flag) {
		CQM_DEBUG(CQM_DBG_FLAG_QID_MAP, "\nGet lookup flag: ");
		for (i = LOOKUP_FIELD_BITS - 1; i >= 0; i--) {
			if (lookup_mask2[i])
				CQM_DEBUG(CQM_DBG_FLAG_QID_MAP, "x");
			else
				CQM_DEBUG(CQM_DBG_FLAG_QID_MAP, "%d",
					  lookup_flags2[i]);
		}
		CQM_DEBUG(CQM_DBG_FLAG_QID_MAP, "\n");

		return 0;
	} else {
		return -1;
	}
}

void cqm_lookup_table_recursive(int k, int tmp_index, int set_flag, int qid)
{
	int i;
	struct cbm_lookup lookup = {0};

	if (k < 0) {	/*finish recursive and start real read/set action */
		if (set_flag) {
			lookup.index = tmp_index;
			lookup.qid = qid;
			cqm_dp_set_lookup_qid_via_index(&lookup);

			CQM_DEBUG(CQM_DBG_FLAG_QID_MAP,
				  "Set lookup[%05u/0x%04x] ->     queue[%d]\n",
				  tmp_index, tmp_index, qid);

		} else {
			lookup.index = tmp_index;
			qid = cqm_dp_get_lookup_qid_via_index(&lookup);
			pr_info("Get lookup[%05u/0x%04x] ->     queue[%d]\n",
				tmp_index, tmp_index, qid);
		}
		return;
	}

	if (lookup_mask2[k]) {
		for (i = 0; i < 2; i++)
			cqm_lookup_table_recursive(k - 1, tmp_index + (i << k),
						   set_flag, qid);
		return;
	}

	cqm_lookup_table_recursive(k - 1, tmp_index + (lookup_flags2[k] << k),
				   set_flag, qid);
}

static int cqm_dp_get_lookup_qid_via_index(struct cbm_lookup *info)
{
	int qid;

	if (!info)
		return 0;
	if (info->index & BIT(EGFLAG_BIT))
		info->egflag = 1;
	else
		info->egflag = 0;
	info->index &= BIT(EGFLAG_BIT) - 1;
	qid = get_lookup_qid_via_index(info);

	pr_debug("get egflag=%d index=0x%x qid=%d\n",
		 info->egflag, info->index, qid);

	return qid;
}

static void cqm_dp_set_lookup_qid_via_index(struct cbm_lookup *info)
{
	if (!info)
		return;
	if (info->index & BIT(EGFLAG_BIT))
		info->egflag = 1;
	else
		info->egflag = 0;
	info->index &= BIT(EGFLAG_BIT) - 1;
	set_lookup_qid_via_index(info);

	CQM_DEBUG(CQM_DBG_FLAG_QID_MAP, "set egflag=%d index=0x%x qid=%d\n",
		  info->egflag, info->index, info->qid);
}

void cqm_lookup_table_via_qid(int qid)
{
	u32 index, tmp, i, j, k, f = 0;
	struct cbm_lookup lookup;

	CQM_DEBUG(CQM_DBG_FLAG_QID_MAP,
		  "Try to find all lookup flags mapped to qid %d\n", qid);

	for (i = 0; i < 16; i++) { /*ep: 4 bits */
		for (j = 0; j < 256; j++) { /*selected fields: 8 bits */
			for (k = 0; k < 2; k++) {/*eg flag: 1 bit */
				index = (k << EGFLAG_BIT) | (i << EP_BIT) | j;
				lookup.index = index;
				tmp = cqm_dp_get_lookup_qid_via_index(&lookup);
				if (tmp != qid)
					continue;
				f = 1;
				pr_info("Get lookup[%05u 0x%04x]%s[%d]\n",
					index, index,
					" ->     queue", qid);
			}
		}
	}
	if (!f)
		pr_err("No mapping to queue id %d yet ?\n", qid);
}

void cqm_lookup_table_remap(int old_q, int new_q)
{
	u32 index, tmp, i, j, k, f = 0;
	struct cbm_lookup lookup;

	CQM_DEBUG(CQM_DBG_FLAG_QID_MAP,
		  "Try to remap lookup flags mapped from old_q %d to new_q %d\n",
		  old_q, new_q);

	for (i = 0; i < 16; i++) { /* ep */
		for (j = 0; j < 256; j++) { /*  select fields */
			for (k = 0; k < 2; k++) { /* eg flag */
				index = (k << EGFLAG_BIT) | (i << EP_BIT) | j;
				lookup.index = index;
				tmp = cqm_dp_get_lookup_qid_via_index(&lookup);
				if (tmp != old_q)
					continue;
				lookup.index = index;
				lookup.qid = new_q;
				cqm_dp_set_lookup_qid_via_index(&lookup);
				f = 1;

				CQM_DEBUG(CQM_DBG_FLAG_QID_MAP,
					  "Remap lookup[%05u 0x%04x] %s[%d]\n",
					  index, index,
					  "->     queue", new_q);
			}
		}
	}
	if (!f)
		pr_info("No mapping to queue id %d yet\n", new_q);
}

ssize_t cqm_get_qid_via_index32(struct file *file, const char *buf,
				size_t count, loff_t *ppos)
{
	int err = 0, len = 0;
	char data[100];
	unsigned int lookup_index;
	unsigned int qid = 0;
	char *param_list[10];
	int num;
	struct cbm_lookup lookup;

	len = (count >= sizeof(data)) ? (sizeof(data) - 1) : count;
	CQM_DEBUG(CQM_DBG_FLAG_QID_MAP, "len=%d\n", len);

	if (len <= 0) {
		err = -EFAULT;
		pr_err("Wrong len value (%d)\n", len);
		return count;
	}

	if (copy_from_user(data, buf, len)) {
		err = -EFAULT;
		pr_err("copy_from_user fail");
		return count;
	}

	data[len - 1] = 0; /* Make string */

	#ifndef KERNEL_5_15_DEP_MODULE_NOT_PORTED
	num = dp_split_buffer(data, param_list, ARRAY_SIZE(param_list));
	#endif

	if (num <= 1)
		goto help;
	if (!param_list[1])
		goto help;

	#ifndef KERNEL_5_15_DEP_MODULE_NOT_PORTED
	lookup_index = dp_atoi(param_list[1]);

	if ((dp_strncmpi(param_list[0], "set", strlen("set")) == 0) ||
	    (dp_strncmpi(param_list[0], "write", strlen("write")) == 0)) {
		if (!param_list[2]) {
			pr_err("wrong command\n");
			return count;
		}
		qid = dp_atoi(param_list[2]);
		/*workaround for mask support */
		if (cqm_get_dont_care_lookup(param_list[1]) == 0) {
			cqm_lookup_table_recursive(LOOKUP_FIELD_BITS - 1, 0, 1,
						   qid);
			return count;
		}
		pr_info("Set to queue[%u] done\n", qid);
		lookup.index = lookup_index;
		lookup.qid = qid;
		cqm_dp_set_lookup_qid_via_index(&lookup);
		return count;
	} else if ((dp_strncmpi(param_list[0], "get", strlen("get")) == 0) ||
		   (dp_strncmpi(param_list[0], "read", strlen("read")) == 0)) {
		if (cqm_get_dont_care_lookup(param_list[1]) == 0) {
			cqm_lookup_table_recursive(LOOKUP_FIELD_BITS - 1,
						   0, 0, 0);
			return count;
		}
		lookup.index = lookup_index;
		qid = cqm_dp_get_lookup_qid_via_index(&lookup);
		pr_info("Get lookup[%05u 0x%04x] ->     queue[%u]\n",
			lookup_index, lookup_index, qid);
		return count;
	} else if (dp_strncmpi(param_list[0], "find",
		   strlen("find") + 1) == 0) {
		/*read out its all flags for specified qid */
		int i;

		qid = dp_atoi(param_list[1]);
		for (i = 0; i < 16; i++)
			cqm_find_pattern(i, NULL, qid);
		return count;
	} else if (dp_strncmpi(param_list[0], "find2",
		   strlen("find2") + 1) == 0) {
		/*read out its all flags for specified qid */
		qid = dp_atoi(param_list[1]);
		cqm_lookup_table_via_qid(qid);
		return count;
	} else if (dp_strncmpi(param_list[0], "remap", strlen("remap")) == 0) {
		int old_q = dp_atoi(param_list[1]);
		int new_q = dp_atoi(param_list[2]);

		cqm_lookup_table_remap(old_q, new_q);
		pr_info("remap queue[%d] to queue[%d] done\n",
			old_q, new_q);
		return count;
	}  else if (dp_strncmpi(param_list[0], "test",
		   strlen("test") + 1) == 0) {
		cbm_queue_map_entry_t lookup = {0};
		int qid = dp_atoi(param_list[1]);
		u32 flag = CBM_QUEUE_MAP_F_FLOWID_L_DONTCARE |
			   CBM_QUEUE_MAP_F_FLOWID_H_DONTCARE |
			   CBM_QUEUE_MAP_F_SUBIF_DONTCARE |
			   CBM_QUEUE_MAP_F_EN_DONTCARE |
			   CBM_QUEUE_MAP_F_DE_DONTCARE |
			   CBM_QUEUE_MAP_F_MPE1_DONTCARE |
			   CBM_QUEUE_MAP_F_MPE2_DONTCARE |
			   CBM_QUEUE_MAP_F_TC_DONTCARE |
			   CBM_QUEUE_MAP_F_COLOR_DONTCARE;

		lookup.egflag = 0;
		lookup.ep = 3;
		cbm_queue_map_set(0, qid,
				  &lookup, flag);
		pr_info("cbm_queue_map_set ep=%d egflag=%d qid=%d flag=0x%x\n",
			lookup.ep, lookup.egflag, qid, flag);
		lookup.egflag = 1;
		cbm_queue_map_set(0, qid,
				  &lookup, flag);
		pr_info("cbm_queue_map_set ep=%d egflag=%d qid=%d flag=0x%x\n",
			lookup.ep, lookup.egflag, qid, flag);
		return count;
	} else if (dp_strncmpi(param_list[0], "ep",
		   strlen("ep") + 1) == 0) {
		int ep = dp_atoi(param_list[1]);

		if (ep >= MAX_QID_MAP_EP_NUM) {
			pr_info("ep is out of range\n");
			return count;
		}

		cqm_find_pattern(ep, NULL, -1);
		return count;
	}
#else
	return count;
#endif

	goto help;
help:
	pr_info("Usage: echo set lookup_flags queue_id > qid_queue_map\n");
	pr_info("     : echo ep <x> > qid_queue_map\n");
	pr_info("     : echo get lookup_flags > qid_queue_map\n");
	pr_info("     : echo find  <x> > qid_queue_map\n");
	pr_info("     : echo find2 <x> > qid_queue_map\n");
	pr_info("     : echo remap <old_q> <new_q> > qid_queue_map\n");
	pr_info("     : echo test <qid> > qid_queue_map\n");
	pr_info("  Hex example: echo set 0x10 10 > qid_queue_map\n");
	pr_info("  Dec:example: echo set 16 10 > qid_queue_map\n");
	pr_info("  Bin:example: echo set b10000 10 > qid_queue_map\n");
	pr_info("%s: echo set b1xxxx 10 > qid_queue_map\n",
		"Special for BIN(Don't care bit)");
	return count;
}

static ssize_t cqm_qid_queue_map_write(struct file *file,
				       const char __user *buffer,
				       size_t count, loff_t *ppos)
{
	int ret;

	ret = cqm_get_qid_via_index32(file, buffer,
				      count, ppos);
	return ret;
}

static const struct file_operations cqm_qid_queue_map_fops = {
	.owner = THIS_MODULE,
	.open = cqm_dbg_open,
	.read = seq_read,
	.write = cqm_qid_queue_map_write,
	.llseek = seq_lseek,
	.release = cqm_dbg_release,
};

static int cpm_lpid_to_mode_read(struct seq_file *s, void *v)
{
	cbm_queue_map_entry_t entry;
	u32 lpid, mode;

	seq_puts(s, "lpid <-> mode mapping table\n");
	seq_puts(s, "---------------\n");
	seq_puts(s, "| lpid | mode |\n");
	for (lpid = 0; lpid <= 15; lpid++) {
		entry.ep = lpid;
		cqm_mode_table_get(0, &mode, &entry, 0);
		seq_puts(s, "---------------\n");
		seq_printf(s, "| %4u | %4u |\n", lpid, mode);
	}
	seq_puts(s, "To change the setting: echo <lpid> <mode> > lpid_to_mode\n");
	return 0;
}

static int cpm_lpid_to_mode_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cpm_lpid_to_mode_read, inode->i_private);
}

static ssize_t cpm_lpid_to_mode_write(struct file *file,
				      const char __user *buffer,
				      size_t count, loff_t *ppos)
{
	struct cqm_ctrl *pctrl = file_inode(file)->i_private;
	cbm_queue_map_entry_t entry;
	u32 lpid, mode;
	int len = count;
	char str[32] = {0};

	len = min(count, sizeof(str));
	if (copy_from_user(str, buffer, len)) {
		dev_err(pctrl->dev, "fail to read from user %d\n", len);
		return count;
	}

	if (strcmp(str, "help") == 0)
		goto __write_help;

	if (sscanf(str, "%u %u", &lpid, &mode) != 2) {
		PRINTK("error : received lpid = %d, mode = %d\n",
		       lpid, mode);
		goto __write_help;
	}

	if (lpid > 15 || mode > 7) {
		PRINTK("input out of range, lpid = %d, mode = %d\n",
		       lpid, mode);
		goto __write_help;
	}

	PRINTK("received lpid = %d and mode = %d\n", lpid, mode);
	entry.ep = lpid;
	cqm_mode_table_set(0, &entry, mode, 0);
	return count;

__write_help:
	PRINTK("to change the setting: echo <lpid> <mode> > lpid_to_mode\n");
	return count;
}

static const struct file_operations lpid_to_mode_fops = {
	.open = cpm_lpid_to_mode_open,
	.read = seq_read,
	.write = cpm_lpid_to_mode_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static int cqm_strncmpi(const char *s1, const char *s2, size_t n)
{
	if (!s1 || !s2)
		return 1;
	return strncasecmp(s1, s2, n);
}

static ssize_t cqm_dbg_flag_write(struct file *file,
				  const char __user *buf,
				  size_t count, loff_t *ppos)
{
	int len, i, j;
	char str[64];
	int num;
	char *param_list[20];
	int f_enable;

	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;
	#ifndef KERNEL_5_15_DEP_MODULE_NOT_PORTED
	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));
	#endif

	if (cqm_strncmpi(param_list[0], "enable", strlen("enable")) == 0)
		f_enable = 1;
	else if (cqm_strncmpi(param_list[0], "disable", strlen("disable")) == 0)
		f_enable = 0;
	else
		goto help;

	if (!param_list[1]) {	/*no parameter after enable or disable */
		if (f_enable)
			cqm_dbg_flag = 0xFFFFFFFF;
		else
			cqm_dbg_flag = 0;
		goto EXIT;
	}

	for (i = 1; i < num; i++) {
		for (j = 0; j < (ARRAY_SIZE(cqm_dbg_flag_str)) - 1; j++)
			if (cqm_strncmpi(param_list[i],
					 cqm_dbg_flag_str[j],
					 strlen(cqm_dbg_flag_str[j]) + 1)
					 == 0) {
				if (f_enable == 1)
					cqm_dbg_flag |= cqm_dbg_flag_list[j];
				else
					cqm_dbg_flag &= ~cqm_dbg_flag_list[j];
				break;
			}
	}

EXIT:
	return count;
help:
	PRINTK("use below command to enable/disable debug prints:\n");
	PRINTK("echo enable dbg_flag > %s\n", CQM_DEBUGFS_DBG);
	PRINTK("echo disable dbg_flag > %s\n", CQM_DEBUGFS_DBG);
	PRINTK("available dbg_flag:\n");
	for (i = 1; i < sizeof(cqm_dbg_flag_list) / sizeof(u32); i++) {
		if (cqm_dbg_flag_str[i])
			PRINTK("%12s  ", cqm_dbg_flag_str[i]);
	}
	PRINTK("\n");
	return count;
}

static int cpm_dbg_flag_read(struct seq_file *s, void *v)
{
	u32 i, enable;

	seq_puts(s, "dbg flag status:\n");
	for (i = 1; i < sizeof(cqm_dbg_flag_list) / sizeof(u32); i++) {
		if (cqm_dbg_flag_str[i]) {
			if ((cqm_dbg_flag & cqm_dbg_flag_list[i]) ==
			    cqm_dbg_flag_list[i])
				enable = 1;
			else
				enable = 0;
			seq_printf(s, "%12s  enable: %d\n",
				   cqm_dbg_flag_str[i], enable);
			}
	}
	return 0;
}

static int cqm_dbg_flag_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cpm_dbg_flag_read, inode->i_private);
}

static const struct file_operations cqm_dbg_flag_fops = {
	.open = cqm_dbg_flag_open,
	.read = seq_read,
	.write = cqm_dbg_flag_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static int cqm_qid_num_entries_read(struct seq_file *s, void *v)
{
	u32  qid, num_entries0, num_entries1;
	cbm_queue_map_entry_t *qmap_entry = NULL;

	seq_puts(s, "----------------------------------------\n");
	seq_puts(s, "| qid  |    egress0    |    egress1    |\n");
	seq_puts(s, "----------------------------------------\n");
	for (qid = 0; qid < 512; qid++) {
		if (cbm_queue_map_get(0, (s32)qid,
				      &num_entries0, &qmap_entry, 0)) {
			seq_printf(s,
				   "Error detected when querying qid = %d\n",
				   qid);
			return 0;
		}

		cbm_queue_map_buf_free(qmap_entry);

		if (cbm_queue_map_get(0, (s32)qid,
				      &num_entries1, &qmap_entry, 0)) {
			seq_printf(s,
				   "Error detected when querying qid = %d\n",
				   qid);
			return 0;
		}

		cbm_queue_map_buf_free(qmap_entry);
		if (!num_entries0 && !num_entries1)
			continue;
		seq_printf(s, "| %3d  |      %4d     |      %4d     |\n",
			   qid, num_entries0, num_entries1);
	}
	return 0;
}

static int cqm_qid_num_entries_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cqm_qid_num_entries_read, inode->i_private);
}

static const struct file_operations qid_num_entries_fops = {
	.open = cqm_qid_num_entries_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int cqm_epon_queue_read(struct seq_file *s, void *v)
{
	int startq, endq, base_port;
	u32 epon_mode_reg_value = epon_mode_reg_get();

	startq = epon_mode_reg_value & EPON_EPON_MODE_REG_STARTQ_MASK;
	endq = (epon_mode_reg_value & EPON_EPON_MODE_REG_ENDQ_MASK)
		>> EPON_EPON_MODE_REG_ENDQ_POS;
	base_port = (epon_mode_reg_value
		& EPON_EPON_MODE_REG_EPONBASEPORT_MASK)
		>> EPON_EPON_MODE_REG_EPONBASEPORT_POS;

	seq_puts(s, "EPON_QUEUE_SETTING:\n");
	seq_printf(s, "base port:	%d\n", base_port);
	seq_printf(s, "starting qid:	%d\n", startq);
	seq_printf(s, "total num:	%d\n", 1 + endq - startq);
	return 0;
}

static int cqm_epon_queue_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cqm_epon_queue_read, inode->i_private);
}

static const struct file_operations cqm_epon_queue_fops = {
	.open = cqm_epon_queue_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int cqm_qid_to_ep_read(struct seq_file *s, void *v)
{
	int port, qid_i;

	seq_puts(s, "qid to ep port mapping table\n");
	seq_puts(s, "---------------\n");
	seq_puts(s, "|  qid | port |\n");

	for (qid_i = 0; qid_i < MAX_QOS_QUEUES; qid_i++) {
		cqm_qid2ep_map_get(qid_i, &port);
		if (port != 0) {
			seq_puts(s, "---------------\n");
			seq_printf(s, "| %4u | %4u |\n", qid_i, port);
		}
	}
	return 0;
}

static int cqm_qid_to_ep_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cqm_qid_to_ep_read, inode->i_private);
}

static const struct file_operations cqm_qid_to_ep_fops = {
	.open = cqm_qid_to_ep_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int cqm_hr_tr_info_read(struct seq_file *s, void *v)
{
	u32 i;
	struct hr_tr_tbl *p = get_hr_tr_tbl(1);

	seq_puts(s, "----------------------------------------------\n");
	seq_puts(s, "|    Egress Port    |   Headroom |   Tailroom |\n");
	seq_puts(s, "----------------------------------------------\n");

	for (i = 0; i < DP_SPL_MAX; i++, p++) {
		seq_printf(s, "|%18s |     %6u |     %6u |\n", p->name,
			   p->headroom, p->tailroom);
		seq_puts(s, "----------------------------------------------\n");
	}

	p = get_hr_tr_tbl(0);
	for (i = 0; i < MAX_SUP_DEVICE; i++, p++) {
		seq_printf(s, "|%18s |     %6u |     %6u |\n", p->name,
			   p->headroom, p->tailroom);
		seq_puts(s, "----------------------------------------------\n");
	}
	return 0;
}

static int cqm_hr_tr_info_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cqm_hr_tr_info_read, inode->i_private);
}

static const struct file_operations cqm_hr_tr_info_fops = {
	.open = cqm_hr_tr_info_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int cqm_enq_port_info_read(struct seq_file *s, void *v)
{
	int pid;
	struct cqm_eqm_port_info *eqp_info;
	void *enq_base = cqm_get_enq_base();
	u32 eqreq, bp_en, qos, eqpc_reg, drop_reg;
	u32 port_type;
	struct eqm_pid_s *enq_id_p = cqm_get_eqm_pid_cfg();

	seq_puts(s, "--EQM PORT TYPE--\n");
	seq_printf(s, "EQM_CPU_TYPE:	%3d : %3d\n",
		   enq_id_p->cpu_start, enq_id_p->cpu_end);
	seq_printf(s, "EQM_VM_TYPE:	%3d : %3d\n",
		   enq_id_p->vm_start, enq_id_p->voice_end);
	seq_printf(s, "EQM_ACA_TYPE:	%3d : %3d\n",
		   enq_id_p->aca_start, enq_id_p->aca_end);
	seq_printf(s, "EQM_DMA_TYPE:	%3d : %3d\n",
		   enq_id_p->dma0_start, enq_id_p->rxdma);
	seq_puts(s, "--EQM PORT ID--\n");
	seq_printf(s, "EQM_CPU_START_ID:	%3d :	%3d\n",
		   enq_id_p->cpu_start, enq_id_p->cpu_end);
	seq_printf(s, "EQM_VM_START_ID:	%3d :	%3d\n",
		   enq_id_p->vm_start, enq_id_p->vm_end);
	seq_printf(s, "EQM_VOICE_START_ID:	%3d :	%3d\n",
		   enq_id_p->voice_start, enq_id_p->voice_end);
	seq_printf(s, "EQM_QBYP_START_ID:	%3d :	%3d\n",
		   enq_id_p->qosbp_start, enq_id_p->qosbp_end);
	seq_printf(s, "EQM_DC_START_ID:	%3d :	%3d\n",
		   enq_id_p->aca_start, enq_id_p->aca_end);
	seq_printf(s, "EQM_DMA0_START_ID:	%3d :	%3d\n",
		   enq_id_p->dma0_start, enq_id_p->dma0_end);
	seq_printf(s, "EQM_TOE_TSO_ID:		%3d\n",
		   enq_id_p->tso);
	seq_printf(s, "EQM_TOE_LRO_ID:		%3d :	%3d\n",
		   enq_id_p->lro_start, enq_id_p->lro_end);
	seq_printf(s, "EQM_NGNA_RXDMA_ID:	%3d\n",
		   enq_id_p->rxdma);

	seq_puts(s, "--------------------------------------------------------------------------------------\n");
	seq_puts(s, "|port id|    port_type   | En | BackPressure | QOS_BYPASS |    EQPC    |    DROP    |\n");
	seq_puts(s, "--------------------------------------------------------------------------------------\n");

	for (pid = enq_id_p->cpu_start; pid <= enq_id_p->cpu_end; pid++) {
		port_type = 0;
		eqreq = get_val(cbm_r32(enq_base + EQ_CPU_PORT(pid, cfg)),
				CFG_CPU_IGP_0_EQREQ_MASK,
				CFG_CPU_IGP_0_EQREQ_POS);
		bp_en = get_val(cbm_r32(enq_base + EQ_CPU_PORT(pid, cfg)),
				CFG_CPU_IGP_0_BP_EN_MASK,
				CFG_CPU_IGP_0_BP_EN_POS);
		qos = get_val(cbm_r32(enq_base + EQ_CPU_PORT(pid, cfg)),
			      CFG_CPU_IGP_0_QOSBYPSEN_MASK,
			      CFG_CPU_IGP_0_QOSBYPSEN_POS);
		eqpc_reg = cbm_r32(enq_base + EQ_CPU_PORT(pid, eqpc));
		drop_reg = cbm_r32(enq_base + EQ_CPU_PORT(pid, drop));
		seq_printf(s, "| %5u | %14s | %2d | %12d | %7d    | 0x%08x | 0x%08x |\n",
			   pid, "CPU", eqreq, bp_en, qos, eqpc_reg, drop_reg);
	}

	for (pid = enq_id_p->qosbp_start; pid <= enq_id_p->qosbp_end; pid++) {
		port_type = 0;
		eqreq = get_val(cbm_r32(enq_base + EQ_CPU_PORT(pid, cfg)),
				CFG_CPU_IGP_12_EQREQ_MASK,
				CFG_CPU_IGP_12_EQREQ_POS);
		bp_en = get_val(cbm_r32(enq_base + EQ_CPU_PORT(pid, cfg)),
				CFG_CPU_IGP_12_BP_EN_MASK,
				CFG_CPU_IGP_12_BP_EN_POS);
		qos = get_val(cbm_r32(enq_base + EQ_CPU_PORT(pid, cfg)),
			      CFG_CPU_IGP_12_QOSBYPSEN_MASK,
			      CFG_CPU_IGP_12_QOSBYPSEN_POS);
		eqpc_reg = cbm_r32(enq_base + EQ_CPU_PORT(pid, eqpc));
		drop_reg = cbm_r32(enq_base + EQ_CPU_PORT(pid, drop));
		seq_printf(s, "| %5u | %14s | %2d | %12d | %7d    | 0x%08x | 0x%08x |\n",
			   pid, "CPU - Qos BYP", eqreq, bp_en,
			   qos, eqpc_reg, drop_reg);
	}

	for (pid = enq_id_p->aca_start; pid <= enq_id_p->aca_end; pid++) {
		eqreq = get_val(cbm_r32(enq_base + EQ_DC_PORT(pid, cfg)),
				CFG_DC_IGP_16_EQREQ_MASK,
				CFG_DC_IGP_16_EQREQ_POS);
		bp_en = get_val(cbm_r32(enq_base + EQ_DC_PORT(pid, cfg)),
				CFG_DC_IGP_16_BP_EN_MASK,
				CFG_DC_IGP_16_BP_EN_POS);
		qos = get_val(cbm_r32(enq_base + EQ_DC_PORT(pid, cfg)),
			      CFG_DC_IGP_16_QOSBYPSEN_MASK,
			      CFG_DC_IGP_16_QOSBYPSEN_POS);
		eqpc_reg = cbm_r32(enq_base + EQ_DC_PORT(pid, eqpc));
		drop_reg = cbm_r32(enq_base + EQ_DC_PORT(pid, drop));
		eqp_info = cqm_get_eqp_info_p(pid);
		seq_printf(s, "| %5u | %14s | %2d | %12d | %7d    | 0x%08x | 0x%08x |\n",
			   pid, "DC", eqreq, bp_en, qos, eqpc_reg, drop_reg);
	}

	for (pid = enq_id_p->dma0_start; pid <= enq_id_p->rxdma; pid++) {
		eqreq = get_val(cbm_r32(enq_base + EQ_DMA_PORT(pid, cfg)),
				CFG_DMA_IGP_24_EQREQ_MASK,
				CFG_DMA_IGP_24_EQREQ_POS);
		bp_en = get_val(cbm_r32(enq_base + EQ_DMA_PORT(pid, cfg)),
				CFG_DMA_IGP_24_BP_EN_MASK,
				CFG_DMA_IGP_24_BP_EN_POS);
		eqpc_reg = cbm_r32(enq_base + EQ_DMA_PORT(pid, eqpc));
		drop_reg = cbm_r32(enq_base + EQ_DMA_PORT(pid, drop));
		seq_printf(s, "| %5u | %14s | %2d | %12d |       x    | 0x%08x | 0x%08x |\n",
			   pid, "DMA", eqreq, bp_en, eqpc_reg, drop_reg);
	}

	seq_puts(s, "\nDC ENQ PORT rx ring info\n");
	seq_puts(s, "----------------------------------------\n");
	seq_puts(s, "|port_id|rxout ring_size|rxin ring_size|\n");
	seq_puts(s, "----------------------------------------\n");

	for (pid = enq_id_p->aca_start; pid <= enq_id_p->aca_end; pid++) {
		eqp_info = cqm_get_eqp_info_p(pid);
		if (eqp_info)
			seq_printf(s, "|%7d| %13u | %12u |\n",
				   pid,
				   eqp_info->eq_info.num_desc,
				   eqp_info->eq_info.num_alloc_desc);
	}

	seq_puts(s, "\nDMA ENQ PORT DMA CHANNEL info\n");
	seq_puts(s, "----------------------------------------\n");
	seq_puts(s, "|port_id|dma_ch|ctrl|init_type|num_desc|\n");
	seq_puts(s, "----------------------------------------\n");

	for (pid = enq_id_p->dma0_start; pid <= enq_id_p->rxdma; pid++) {
		eqp_info = cqm_get_eqp_info_p(pid);
		if (eqp_info)
			seq_printf(s, "|%7d| %4u | %2u | %7u | %6u |\n",
				   pid,
				   eqp_info->dma_dt_ch,
				   eqp_info->dma_dt_ctrl,
				   eqp_info->dma_dt_init_type,
				   eqp_info->eq_info.num_desc);
	}
	return 0;
}

static int cqm_enq_port_info_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cqm_enq_port_info_read, inode->i_private);
}

static const struct file_operations cqm_enq_port_info_fops = {
	.open = cqm_enq_port_info_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int cqm_deq_port_info_read(struct seq_file *s, void *v)
{
	int pid;
	struct cqm_dqm_port_info *deqp_info;
	void *deq_base = cqm_get_deq_base();
	u32 dqreq, bp_en, qos, dqpc_reg;
	u32 port_type, hflag, hmode, egflag;
	struct dqm_pid_s *dq_id_p = cqm_get_dqm_pid_cfg();

	seq_puts(s, "--DQM PORT TYPE--\n");
	seq_printf(s, "DQM_CPU_TYPE:	%3d : %3d\n",
		   dq_id_p->cpu_start, dq_id_p->lro_end);
	seq_printf(s, "DQM_ACA_TYPE:	%3d : %3d\n",
		   dq_id_p->aca_start, dq_id_p->aca_end);
	seq_printf(s, "DQM_DMA_TYPE:	%3d : %3d\n",
		   dq_id_p->dma2_start, dq_id_p->dma0_end);
	seq_printf(s, "DQM_PON_TYPE:	%3d : %3d\n",
		   dq_id_p->pon_start, dq_id_p->pon_end);
	seq_printf(s, "DQM_DOCSIS_TYPE:	%3d : %3d\n",
		   dq_id_p->docsis_start, dq_id_p->docsis_end);
	seq_printf(s, "DQM_PP_NF_TYPE:	%3d : %3d\n",
		   dq_id_p->ppnf_start, dq_id_p->ppnf_end);
	seq_printf(s, "DQM_VUNI_TYPE:	%3d : %3d\n",
		   dq_id_p->vuni_start, dq_id_p->vuni_end);
	seq_puts(s, "--DQM PORT ID--\n");
	seq_printf(s, "DQM_CPU_START_ID:	%3d : %3d\n",
		   dq_id_p->cpu_start, dq_id_p->cpu_end);
	seq_printf(s, "DQM_VM_START_ID:	%3d : %3d\n",
		   dq_id_p->vm_start, dq_id_p->vm_end);
	seq_printf(s, "DQM_LRO_START_ID:	%3d : %3d\n",
		   dq_id_p->lro_start, dq_id_p->lro_end);
	seq_printf(s, "DQM_VOICE_START_ID:	%3d : %3d\n",
		   dq_id_p->voice_start, dq_id_p->voice_end);
	seq_printf(s, "DQM_ACA_START_ID:	%3d : %3d\n",
		   dq_id_p->aca_start, dq_id_p->aca_end);
	seq_printf(s, "DQM_DMA2_START_ID:	%3d : %3d\n",
		   dq_id_p->dma2_start, dq_id_p->dma0_end);
	seq_printf(s, "DQM_CPU_QOSBP_START_ID:	%3d : %3d\n",
		   dq_id_p->qosbp_start, dq_id_p->qosbp_end);
	seq_printf(s, "DQM_PON_START_ID:	%3d : %3d\n",
		   dq_id_p->pon_start, dq_id_p->pon_end);
	seq_printf(s, "DQM_DOCSIS_START_ID:	%3d : %3d\n",
		   dq_id_p->docsis_start, dq_id_p->docsis_end);
	seq_printf(s, "DQM_PP_NF_START_ID:	%3d : %3d\n",
		   dq_id_p->ppnf_start, dq_id_p->ppnf_end);
	seq_printf(s, "DQM_VUNI_START_ID:	%3d : %3d\n",
		   dq_id_p->vuni_start, dq_id_p->vuni_end);
	seq_printf(s, "DQM_VPN_START_ID:	%3d : %3d\n",
		   dq_id_p->vpn_start, dq_id_p->vpn_end);
	seq_puts(s, "--------------------------------------------------------------\n");
	seq_puts(s, "|port id|port_type| En | BackPressure |    DQPC    |FIFO Size|\n");
	seq_puts(s, "--------------------------------------------------------------\n");

	for (pid = dq_id_p->cpu_start; pid <= dq_id_p->lro_end; pid++) {
		port_type = DQM_CPU_TYPE;
		dqreq = get_val(cbm_r32(deq_base + DQ_CPU_PORT(pid, cfg)),
				CFG_CPU_EGP_0_DQREQ_MASK,
				CFG_CPU_EGP_0_DQREQ_POS);
		if (!dqreq)
			continue;
		bp_en = get_val(cbm_r32(deq_base + DQ_CPU_PORT(pid, cfg)),
				CFG_CPU_EGP_0_BFBPEN_MASK,
				CFG_CPU_EGP_0_BFBPEN_POS);
		dqpc_reg = cbm_r32(deq_base + DQ_CPU_PORT(pid, dqpc));

		deqp_info = cqm_get_dqp_info_p(pid);
		if (deqp_info)
			seq_printf(s, "| %5u | %7s | %2d | %12d | 0x%08x |  %5d  |\n",
				   pid, "CPU", dqreq,
				   bp_en, dqpc_reg,
				   deqp_info->deq_info.num_desc);
	}
	seq_puts(s, "-----------------------------------------------------------------------------------\n");
	seq_puts(s, "|port id|port_type| En | BackPressure |    DQPC    |txout ring_size|txin ring_size|\n");
	seq_puts(s, "-----------------------------------------------------------------------------------\n");

	for (pid = dq_id_p->aca_start; pid <= dq_id_p->aca_end; pid++) {
		dqreq = get_val(cbm_r32(deq_base + DQ_DC_PORT(pid, cfg)),
				CFG_ACA_EGP_19_DQREQ_MASK,
				CFG_ACA_EGP_19_DQREQ_POS);
		if (!dqreq)
			continue;
		bp_en = get_val(cbm_r32(deq_base + DQ_DC_PORT(pid, cfg)),
				CFG_ACA_EGP_19_BFBPEN_MASK,
				CFG_ACA_EGP_19_BFBPEN_POS);
		dqpc_reg = cbm_r32(deq_base + DQ_DC_PORT(pid, dqpc));

		deqp_info = cqm_get_dqp_info_p(pid);

		if (deqp_info)
			seq_printf(s, "| %5u | %7s | %2d | %12d | 0x%08x | %12u  | %11u  |\n",
				   pid, "ACA", dqreq, bp_en, dqpc_reg,
				   deqp_info->deq_info.num_free_burst,
				   deqp_info->deq_info.num_desc);
	}

	seq_puts(s, "-------------------------------------------------------------------------------------------\n");
	seq_puts(s, "|port id|port_type| En | QOS_BYPASS |    DQPC    |HFLAG|HMODE|EGFLAG|dma_ch|ctrl|ring size|\n");
	seq_puts(s, "-------------------------------------------------------------------------------------------\n");

	for (pid = dq_id_p->dma2_start; pid <= dq_id_p->dma0_end; pid++) {
		dqreq = get_val(cbm_r32(deq_base + DQ_DMA_PORT(pid, cfg)),
				CFG_DMA_EGP_27_DQREQ_MASK,
				CFG_DMA_EGP_27_DQREQ_POS);
		if (!dqreq)
			continue;
		deqp_info = cqm_get_dqp_info_p(pid);

		qos = get_val(cbm_r32(deq_base + DQ_DMA_PORT(pid, cfg)),
			      CFG_DMA_EGP_27_QBYPSEN_MASK,
			      CFG_DMA_EGP_27_QBYPSEN_POS);
		dqpc_reg = cbm_r32(deq_base + DQ_DMA_PORT(pid, dqpc));

		hflag = get_val(cbm_r32(deq_base + DQ_DMA_PORT(pid, cfg)),
				CFG_DMA_EGP_27_HFLAG_MASK,
				CFG_DMA_EGP_27_HFLAG_POS);
		hmode = get_val(cbm_r32(deq_base + DQ_DMA_PORT(pid, cfg)),
				CFG_DMA_EGP_27_HMODE_MASK,
				CFG_DMA_EGP_27_HMODE_POS);
		egflag = get_val(cbm_r32(deq_base + DQ_DMA_PORT(pid, cfg)),
				 CFG_DMA_EGP_27_EGFLAG_MASK,
				 CFG_DMA_EGP_27_EGFLAG_POS);

		if (deqp_info)
			seq_printf(s, "| %5u | %7s | %2d |      %2d    | 0x%08x | %3d | %3d |  %3d | %4u | %2u | %6u |\n",
				   pid, "DMA", dqreq, qos, dqpc_reg, hflag,
				   hmode, egflag,
				   deqp_info->dma_dt_ch,
				   deqp_info->dma_dt_ctrl,
				   deqp_info->deq_info.num_desc);
	}

	seq_puts(s, "-------------------------------------\n");
	seq_puts(s, "|port id|port_type| En |    DQPC    |\n");
	seq_puts(s, "-------------------------------------\n");
	for (pid = dq_id_p->pon_start; pid <= dq_id_p->pon_end; pid++) {
		dqreq = get_val(cbm_r32(deq_base + DQ_PON_PORT(pid, cfg)),
				CFG_PON_EGP_75_DQREQ_MASK,
				CFG_PON_EGP_75_DQREQ_POS);
		dqpc_reg = cbm_r32(deq_base + DQ_PON_PORT(pid, dqpc));
		if (!dqreq)
			continue;

		seq_printf(s, "| %5u | %7s | %2d | 0x%08x |\n",
			   pid, "PON", dqreq, dqpc_reg);
	}
	for (pid = dq_id_p->docsis_start; pid <= dq_id_p->docsis_end; pid++) {
		dqreq = get_val(cbm_r32(deq_base + DQ_PON_PORT(pid, cfg)),
				CFG_PON_EGP_75_DQREQ_MASK,
				CFG_PON_EGP_75_DQREQ_POS);
		dqpc_reg = cbm_r32(deq_base + DQ_PON_PORT(pid, dqpc));
		if (!dqreq)
			continue;

		seq_printf(s, "| %5u | %7s | %2d | 0x%08x |\n",
			   pid, "PON", dqreq, dqpc_reg);
	}


	return 0;
}

static int cqm_deq_port_info_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cqm_deq_port_info_read, inode->i_private);
}

static const struct file_operations cqm_deq_port_info_fops = {
	.open = cqm_deq_port_info_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int cqm_desc_read(struct seq_file *s, void *v)
{
	u32 i;
	struct cbm_desc enq_desc;
	struct cbm_desc deq_desc;

	seq_puts(s, "Last Enqueued Descriptor for each cpu\n");
	for_each_online_cpu(i) {
		cqm_desc_get(&enq_desc, i, 1);
		seq_printf(s,
			   "cpu %d : 0x%08x 0x%08x 0x%08x 0x%08x\n", i,
			   enq_desc.desc0,
			   enq_desc.desc1,
			   enq_desc.desc2,
			   enq_desc.desc3);
	}

	seq_puts(s, "Last Dequeued Descriptor for each cpu\n");
	for_each_online_cpu(i) {
		cqm_desc_get(&deq_desc, i, 0);
		seq_printf(s,
			   "cpu %d : 0x%08x 0x%08x 0x%08x 0x%08x\n", i,
			   deq_desc.desc0,
			   deq_desc.desc1,
			   deq_desc.desc2,
			   deq_desc.desc3);
	}
	return 0;
}

static int cqm_desc_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cqm_desc_read, inode->i_private);
}

static const struct file_operations cqm_desc_fops = {
	.open = cqm_desc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

int cqm_dbg_init_cb(void)
{
	return 0;
}

static void *cqm_seq_start(struct seq_file *s, loff_t *pos)
{
	struct cqm_dbg_file_entry *p = s->private;

	if (p->pos < 0)
		return NULL;

	return p;
}

static void *cqm_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	struct cqm_dbg_file_entry *p = s->private;

	*pos = p->pos;

	if (p->pos >= 0)
		return p;
	else
		return NULL;
}

static void cqm_seq_stop(struct seq_file *s, void *v)
{
}

static int cqm_seq_show(struct seq_file *s, void *v)
{
	struct cqm_dbg_file_entry *p = s->private;

	if (p->pos >= 0) {
		if (p->multi_callback) {
			p->pos = p->multi_callback(s, p->pos);
		} else if (p->single_callback) {
			p->single_callback(s);
			p->pos = -1;
		}
	}
	return 0;
}

static const struct seq_operations cqm_seq_ops = {
	.start = cqm_seq_start,
	.next = cqm_seq_next,
	.stop = cqm_seq_stop,
	.show = cqm_seq_show
};

void cqm_dummy_single_show(struct seq_file *s)
{
	seq_puts(s, "Cat Not implemented yet !\n");
}

static int cqm_dbg_open(struct inode *inode, struct file *file)
{
	struct seq_file *s;
	struct cqm_dbg_file_entry *p;
	struct cqm_dbg_entry *entry;
	int ret;

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	ret = seq_open(file, &cqm_seq_ops);
	if (ret)
		return ret;

	s = file->private_data;
	p = kmalloc(sizeof(*p), GFP_KERNEL);

	if (!p) {
		(void)seq_release(inode, file);
		return -ENOMEM;
	}
	memset(p, 0, sizeof(*p));

	entry = inode->i_private;

	if (entry->multi_callback)
		p->multi_callback = entry->multi_callback;
	if (entry->single_callback)
		p->single_callback = entry->single_callback;
	else
		p->single_callback = cqm_dummy_single_show;

	if (entry->init_callback)
		p->pos = entry->init_callback();
	else
		p->pos = 0;

	s->private = p;
	return 0;
}

static int cqm_dbg_release(struct inode *inode, struct file *file)
{
	struct seq_file *s;

	s = file->private_data;
	kfree(s->private);
	return seq_release(inode, file);
}

static const char  * const lpid_config_wan_mode_strings[] = {
	[CQM_LPID_WAN_BIT_ETH] = "ETH",
	[CQM_LPID_WAN_BIT_PON] = "PON",
	[CQM_LPID_WAN_BIT_DOCSIS] = "DOCSIS",
	[CQM_LPID_WAN_BIT_DSL] = "DSL",
	[CQM_LPID_WAN_BIT_MAX] = "invalid",
};

static int cqm_lpid_config_read(struct seq_file *s, void *v)
{
	struct cqm_lpid_config_s *lc = cqm_get_lpid_config();
	int idx;

	if (!lc) {
		seq_puts(s, "Unable to get lpid_config\n");
		return 0;
	}

	seq_printf(s, "port mode: 0x%lx\n", lc->cqm_lpid_port_mode);
	seq_puts(s, "wan mode: [");
	for (idx = 0; idx < CQM_LPID_WAN_BIT_MAX; idx++)
		if (lc->cqm_lpid_wan_mode & BIT(idx))
			seq_printf(s, "%s ",
				   lpid_config_wan_mode_strings[idx]);
	seq_puts(s, "]\n");

	return 0;
}

static int cqm_lpid_config_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cqm_lpid_config_read, inode->i_private);
}

static const struct file_operations cqm_lpid_config_fops = {
	.open = cqm_lpid_config_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static ssize_t cqm_deq_dly_cnt_write(struct file *file, const char __user *buf,
				     size_t count, loff_t *ppos)
{
	struct deq_dma_delay_s *deq_dma_delay = cqm_get_deq_dma_delay();
	u32 deq_port = 0, delay = 0, num = 0;
	int len = 0;
	char str[64], *p = (char *)str, *param_list[5] = { 0 };
	int idx;

	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= copy_from_user(str, buf, len);
	str[len] = 0;
	if (!len)
		return count;

	num = dp_split_buffer(p, param_list, ARRAY_SIZE(param_list));
	if (num == 1 && (strncmp(param_list[0], "help", 4) == 0)) {
		PRINTK("echo deq deq_port delay > /sys/kernel/debug/cqm/deq_dly_cnt\n");
	} else if (num == 3 && (strncmp(param_list[0], "deq", 3) == 0)) {
		deq_port = dp_atoi(param_list[1]);
		delay = dp_atoi(param_list[2]);
		for (idx = 0; idx < DLY_PORT_COUNT; idx++)
			if (deq_dma_delay[idx].deq_port == deq_port) {
				deq_dma_delay[idx].delay = delay;
				break;
			}
		if (idx >= DLY_PORT_COUNT)
			PRINTK("Wrong Param(try): deq port out of range %d\n",
			       deq_port);
		else
			cqm_update_deq_dma_delay(idx);
	} else {
		PRINTK("Wrong Param(try): see help");
	}
	return count;
}

static int cqm_deq_dly_seq_read(struct seq_file *s, void *v)
{
	struct deq_dma_delay_s *deq_dma_delay = cqm_get_deq_dma_delay();
	u32 step_pos_start = DEQ_DLY_CNT_DCP_POMA(0, STEP128, POS);
	u32 step_pos_end = DEQ_DLY_CNT_DCP_POMA(0, STEP1, POS);
	u32 pid, idx, reg_val, deq_port, count, step;
	void *deq_base = cqm_get_deq_base();

	seq_puts(s, "---------------------------------------------------------------------------------\n");
	seq_puts(s, "| DCP | DEQ port | Mapped DMA info | count | step | delay cycles | ovflow | En |\n");
	seq_puts(s, "|     |          |                 |       |      | (count*step) | ovflow | En |\n");
	seq_puts(s, "--------------------------------------------------------------------------------\n");
	for (pid = 0; pid < DLY_PORT_COUNT; pid++) {
		reg_val = cbm_r32(deq_base + DEQ_DLY_CNT_DCP(pid));
		deq_port = 0;
		for (idx = 0; idx < DLY_PORT_COUNT; idx++)
			if (deq_dma_delay[idx].dcp == pid) {
				deq_port = deq_dma_delay[idx].deq_port;
				break;
			}
		count = (reg_val & DEQ_DLY_CNT_DCP_POMA(0, COUNT, MASK)) >>
			DEQ_DLY_CNT_DCP_POMA(0, COUNT, POS);
		step = (reg_val & GENMASK(step_pos_end, step_pos_start)) <<
		       (BITS_PER_TYPE(typeof(reg_val)) - 1 - step_pos_end);
		step = bitrev32(step);
		step = step ? step : (1 << (step_pos_end - step_pos_start + 1));

		seq_printf(s, "| %3d | %8d | %15s | %5d | %4d | %12d | %6d | %2d |\n",
			   pid, deq_port, dcp_dma_desc[pid],
			   count, step, count * step,
			   get_val(reg_val, DEQ_DLY_CNT_DCP_POMA(0, OVFLOW, MASK),
				   DEQ_DLY_CNT_DCP_POMA(0, OVFLOW, POS)),
			   get_val(reg_val, DEQ_DLY_CNT_DCP_POMA(0, DEQ_DLY_EN, MASK),
				   DEQ_DLY_CNT_DCP_POMA(0, DEQ_DLY_EN, POS)));
	}
	return 0;
}

static int cqm_deq_dly_cnt_seq_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cqm_deq_dly_seq_read, inode->i_private);
}

static const struct file_operations cqm_deq_dly_cnt_fops = {
	.open = cqm_deq_dly_cnt_seq_open,
	.read = seq_read,
	.write = cqm_deq_dly_cnt_write,
	.llseek = seq_lseek,
	.release = seq_release,
};

static struct cqm_dbg_entry cqm_dbg_entries[] = {
	{"qid_queue_map", NULL, cqm_qid_queue_map_read, cqm_dbg_init_cb,
	 cqm_qid_queue_map_write, cqm_qid_queue_map_fops},
	{"eqpc", NULL, NULL, NULL, cqm_eqpc_write, cqm_eqpc_fops},
	{"dqpc", NULL, NULL, NULL, cqm_dqpc_write, cqm_dqpc_fops},
	{"enq", NULL, NULL, NULL, cqm_enq_write, cqm_enq_fops},
	{"deq", NULL, NULL, NULL, cqm_deq_write, cqm_deq_fops},
	{"desc", NULL, NULL, NULL, NULL, cqm_desc_fops},
	{"ctrl", NULL, NULL, NULL, cqm_ctrl_write, cqm_ctrl_fops},
	{"cqm_ls", NULL, NULL, NULL, cqm_ls_write, cqm_ls_fops},
	{"ofsc", NULL, NULL, NULL, cqm_ofsc_write, cqm_ofsc_fops},
	{"pkt_count", NULL, NULL, NULL,
	 print_cqm_dbg_cntrs_write, cqm_dbg_cntrs_fops},
	{"lpid_to_mode", NULL, NULL, NULL,
	 cpm_lpid_to_mode_write, lpid_to_mode_fops},
	{"epon_queue", NULL, NULL, NULL, NULL, cqm_epon_queue_fops},
	{"enq_port_info", NULL, NULL, NULL, NULL, cqm_enq_port_info_fops},
	{"deq_port_info", NULL, NULL, NULL, NULL, cqm_deq_port_info_fops},
	{"hr_tr_info", NULL, NULL, NULL, NULL, cqm_hr_tr_info_fops},
	{"dbg", NULL, NULL, NULL, cqm_dbg_flag_write, cqm_dbg_flag_fops},
	{"qid_num_entries", NULL, NULL, NULL, NULL, qid_num_entries_fops},
	{"qid_to_ep", NULL, NULL, NULL, NULL, cqm_qid_to_ep_fops},
	{"lpid_config", NULL, NULL, NULL, NULL, cqm_lpid_config_fops},
	{"deq_dly_cnt", NULL, NULL, NULL, cqm_deq_dly_cnt_write, cqm_deq_dly_cnt_fops},
};

int cqm_debugfs_init(struct cqm_ctrl *pctrl)
{
	char cqm_dir[64] = {0};
	struct dentry *file;
	u8 i;

	strlcpy(cqm_dir, pctrl->name, sizeof(cqm_dir));
	pctrl->debugfs = debugfs_create_dir(cqm_dir, NULL);

	for (i = 0; i < sizeof(cqm_dbg_entries) / sizeof(struct cqm_dbg_entry);
	     i++) {
		file = debugfs_create_file(cqm_dbg_entries[i].name,
					   0400, pctrl->debugfs,
					   &cqm_dbg_entries[i],
					   &cqm_dbg_entries[i].ops);
		if (!file) {
			debugfs_remove_recursive(pctrl->debugfs);
			return -ENOMEM;
		}
	}

	return 0;
}
EXPORT_SYMBOL(cqm_debugfs_init);

static int __init cqm_dbg_flag_set(char *str)
{
	int flags = 0;

	if (get_option(&str, &flags)) {
		pr_info("cqm_dbg_flag: 0x%x\n", flags);
		cqm_dbg_flag = flags;
	} else {
		pr_info("cqm_dbg_flag: invalid flag\n");
	}

	return 0;
}

early_param("cqm_dbg", cqm_dbg_flag_set);

/****************debugfs end ***************************/
