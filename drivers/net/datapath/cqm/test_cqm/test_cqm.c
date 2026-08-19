// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2023 MaxLinear, Inc.
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

#include <linux/debugfs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/seq_file.h>
#include <net/mxl_cbm_api.h>
#include <net/datapath_api.h>
#include <net/datapath_proc_api.h>
#include <linux/kstrtox.h>
#include "cqm.h"

#define CQM_TEST_DEBUGFS_PATH "test_cqm"
#define CQM_TEST_MAX_INTF 8
#define STR_LEN	256
#define PARAM_LEN 20

#define CQM_TRACE(fmt, ...) do { printk(KERN_DEBUG "%s(%d): \t" fmt, __func__, __LINE__, ##__VA_ARGS__); } while (0)
#define CQM_INFO(fmt, ...) do { printk(KERN_INFO "%s(%d): \t" fmt, __func__, __LINE__, ##__VA_ARGS__); } while (0)
#define CQM_ERR(fmt, ...) do { printk(KERN_ERR "%s(%d): \t" fmt, __func__, __LINE__, ##__VA_ARGS__); } while (0)

struct test_cqm_dbg_entry {
	char *name;
	const struct file_operations ops;
};

struct cbm_dp_port_alloc {
	struct module *owner;
	struct net_device *dev;
	u32 dev_port;
	s32 dp_port;
	struct cbm_dp_alloc_data *data;
	u32 flags;
};

struct cbm_dp_port_alloc_complete {
	struct module *owner;
	struct net_device *dev;
	u32 dev_port;
	s32 dp_port;
	struct cbm_dp_alloc_complete_data *data;
	u32 flags;
};

struct cbm_dp_port_enable {
	struct module *owner;
	s32 dp_port;
	struct cbm_dp_en_data *data;
	u32 flags;
	u32 alloc_flags;
};

struct cqm_ctrl *g_cqm_ctrl;
struct dentry *g_test_cqm_debugfs;

struct cbm_dp_alloc_data *g_dp_alloc_data[CQM_TEST_MAX_INTF];
struct dp_port_data *g_port_data[CQM_TEST_MAX_INTF];
struct cbm_dp_port_alloc *g_dp_port_alloc[CQM_TEST_MAX_INTF];

struct cbm_dp_alloc_complete_data *g_dp_alloc_complete_data[CQM_TEST_MAX_INTF];
struct dp_dev_data *g_dev_data[CQM_TEST_MAX_INTF];
struct cbm_dp_port_alloc_complete *g_dp_port_alloc_complete[CQM_TEST_MAX_INTF];

struct cbm_dp_en_data *g_dp_en_data[CQM_TEST_MAX_INTF];
struct dp_subif_data *g_subif_data[CQM_TEST_MAX_INTF];
struct cbm_dp_port_enable *g_dp_port_enable[CQM_TEST_MAX_INTF];

static int g_index;
static int g_ring_index;
static int g_prep;

/*************************************************************************************
 ***  forward function declarations ***
 ************************************************************************************/
static int cbm_dp_port_alloc_data_open(struct inode *inode, struct file *file);
static ssize_t cbm_dp_port_alloc_data_write(struct file *file,
					    const char __user *buf,
					    size_t count, loff_t *ppos);
static int cbm_dp_port_alloc_open(struct inode *inode, struct file *file);
static ssize_t cbm_dp_port_alloc_write(struct file *file,
				       const char __user *buf,
				       size_t count, loff_t *ppos);
static int cbm_dp_port_alloc_complete_data_open(struct inode *inode, struct file *file);
static ssize_t
cbm_dp_port_alloc_complete_data_write(struct file *file,
				      const char __user *buf,
				      size_t count, loff_t *ppos);
static int cbm_dp_port_alloc_complete_open(struct inode *inode, struct file *file);
static ssize_t
cbm_dp_port_alloc_complete_write(struct file *file,
				 const char __user *buf,
				 size_t count, loff_t *ppos);
static int cbm_dp_port_enable_data_open(struct inode *inode, struct file *file);
static ssize_t
cbm_dp_port_enable_data_write(struct file *file,
			      const char __user *buf,
			      size_t count, loff_t *ppos);
static int cbm_dp_port_enale_open(struct inode *inode, struct file *file);
static ssize_t
cbm_dp_port_enale_write(struct file *file,
			const char __user *buf,
			size_t count, loff_t *ppos);
static ssize_t cbm_dp_port_dealloc_write(struct file *file,
					 const char __user *buf,
					 size_t count, loff_t *ppos);
static int cbm_index_open(struct inode *inode, struct file *file);
static ssize_t
cbm_index_write(struct file *file,
		const char __user *buf,
		size_t count, loff_t *ppos);
static int cbm_ring_index_open(struct inode *inode, struct file *file);
static ssize_t
cbm_ring_index_write(struct file *file,
		     const char __user *buf,
		     size_t count, loff_t *ppos);
static int prep_open(struct inode *inode, struct file *file);
static ssize_t prep_write(struct file *file,
			  const char __user *buf,
			  size_t count, loff_t *ppos);
static void test_cqm_free_mem(void);
static int test_cqm_alloc_mem(void);

/*************************************************************************************
 ***  fops and test_cqm_dbg_entries ***
 ************************************************************************************/
static const struct file_operations cbm_dp_port_alloc_data_fops = {
	.open = cbm_dp_port_alloc_data_open,
	.read = seq_read,
	.write = cbm_dp_port_alloc_data_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static const struct file_operations cbm_dp_port_alloc_fops = {
	.open = cbm_dp_port_alloc_open,
	.read = seq_read,
	.write = cbm_dp_port_alloc_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static const struct file_operations cbm_dp_port_alloc_complete_data_fops = {
	.open = cbm_dp_port_alloc_complete_data_open,
	.read = seq_read,
	.write = cbm_dp_port_alloc_complete_data_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static const struct file_operations cbm_dp_port_alloc_complete_fops = {
	.open = cbm_dp_port_alloc_complete_open,
	.read = seq_read,
	.write = cbm_dp_port_alloc_complete_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static const struct file_operations cbm_dp_port_enable_data_fops = {
	.open = cbm_dp_port_enable_data_open,
	.read = seq_read,
	.write = cbm_dp_port_enable_data_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static const struct file_operations cbm_dp_port_enale_fops = {
	.open = cbm_dp_port_enale_open,
	.read = seq_read,
	.write = cbm_dp_port_enale_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static const struct file_operations cbm_dp_port_dealloc_fops = {
	.open = cbm_dp_port_alloc_open,
	.read = seq_read,
	.write = cbm_dp_port_dealloc_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static const struct file_operations cbm_index_fops = {
	.open = cbm_index_open,
	.read = seq_read,
	.write = cbm_index_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static const struct file_operations cbm_ring_index_fops = {
	.open = cbm_ring_index_open,
	.read = seq_read,
	.write = cbm_ring_index_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static const struct file_operations prep_fops = {
	.open = prep_open,
	.read = seq_read,
	.write = prep_write,
	.llseek = seq_lseek,
	.release = single_release,
};

static struct test_cqm_dbg_entry test_cqm_dbg_entries[] = {
	{"dp_port_alloc_data", cbm_dp_port_alloc_data_fops},
	{"dp_port_alloc", cbm_dp_port_alloc_fops},
	{"dp_port_alloc_complete_data", cbm_dp_port_alloc_complete_data_fops},
	{"dp_port_alloc_complete", cbm_dp_port_alloc_complete_fops},
	{"dp_port_enable_data", cbm_dp_port_enable_data_fops},
	{"dp_port_enable", cbm_dp_port_enale_fops},
	{"dp_port_dealloc", cbm_dp_port_dealloc_fops},
	{"index", cbm_index_fops},
	{"ring_index", cbm_ring_index_fops},
	{"prep", prep_fops},
};

static int test_cqm_parse_user_buf(const char __user *buf,
				   size_t count,
				   char *str,
				   char *param_list[],
				   int max_param_num,
				   int *num)
{
	int len;

	if (count >= STR_LEN) {
		CQM_ERR("count %lu is greater than str length %d\n",
			count, STR_LEN);
		return -ENOMEM;
	}

	memset(str, '\0', sizeof(char) * STR_LEN);

	len = (STR_LEN > count) ? count : (STR_LEN - 1);
	len -= copy_from_user(str, buf, len);

	*num = dp_split_buffer(str, param_list, max_param_num);
	return 0;
}

/*************************************************************************************
 *** cbm_dp_port_alloc_data_fops ***
 ************************************************************************************/
static void cbm_dp_port_alloc_data_help(void)
{
	CQM_INFO("echo help|reset|(struct_variable value) > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[0].name);
	CQM_INFO("help: print this help function\n");
	CQM_INFO("reset: reset the struct cbm_dp_port_alloc_data to default values\n");
	CQM_INFO("(struct_variable value): change variable value in struct cbm_dp_port_alloc_data.\n");
	CQM_INFO("\t\t For example, to change value of cbm_dp_port_alloc_data.dp_inst, do following:\n");
	CQM_INFO("\t\t echo dp_inst 1 > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[0].name);
}

static void
cbm_dp_port_alloc_data_dump_dp_port_data(struct seq_file *s,
					 struct dp_port_data *data)
{
	if (!data) {
		seq_puts(s, "struct dp_port_data is null\n");
		return;
	}

	seq_puts(s, "struct dp_port_data:\n");
	seq_printf(s, "\t flag_ops: %d\n", data->flag_ops);
	seq_printf(s, "\t resv_num_port: %u\n", data->resv_num_port);
	seq_printf(s, "\t start_port_no: %u\n", data->start_port_no);
	seq_printf(s, "\t num_resv_q: %d\n", data->num_resv_q);
	seq_printf(s, "\t num_resv_sched: %d\n", data->num_resv_sched);
	seq_printf(s, "\t deq_port_base: %d\n", data->deq_port_base);
	seq_printf(s, "\t deq_num: %d\n", data->deq_num);
	seq_printf(s, "\t link_speed_cap: %u\n", data->link_speed_cap);
	seq_printf(s, "\t n_wib_credit_pkt: %u\n", data->n_wib_credit_pkt);
}

static void
cbm_dp_port_alloc_data_dump(struct seq_file *s,
			    struct cbm_dp_alloc_data *data)
{
	if (!data) {
		seq_puts(s, "struct cbm_dp_alloc_data is null\n");
		return;
	}

	seq_puts(s, "struct cbm_dp_alloc_data:\n");
	seq_printf(s, "\t dp_inst: %d\n", data->dp_inst);
	seq_printf(s, "\t cbm_inst: %d\n", data->cbm_inst);
	seq_printf(s, "\t flags: 0x%x\n", data->flags);
	seq_printf(s, "\t dp_port: %u\n", data->dp_port);
	seq_printf(s, "\t deq_port_num: %u\n", data->deq_port_num);
	seq_printf(s, "\t deq_port: %u\n", data->deq_port);
	seq_printf(s, "\t dma_chan: %u\n", data->dma_chan);
	seq_printf(s, "\t tx_pkt_credit: %u\n", data->tx_pkt_credit);
	seq_printf(s, "\t tx_b_credit: %u\n", data->tx_b_credit);
	seq_printf(s, "\t txpush_addr_qos: %pS\n", data->txpush_addr_qos);
	seq_printf(s, "\t txpush_addr: %pS\n", data->txpush_addr);
	seq_printf(s, "\t tx_ring_size: %u\n", data->tx_ring_size);
	seq_printf(s, "\t tx_ring_offset: %u\n", data->tx_ring_offset);
	seq_printf(s, "\t tx_ring_addr_txpush: %pS\n", data->tx_ring_addr_txpush);
	seq_printf(s, "\t num_dma_chan: %u\n", data->num_dma_chan);
	cbm_dp_port_alloc_data_dump_dp_port_data(s, data->data);
}

static void
cbm_dp_port_alloc_data_init(int idx, struct cbm_dp_alloc_data *data)
{
	memset(g_port_data[idx], 0, sizeof(struct dp_port_data));
	memset(data, 0, sizeof(struct cbm_dp_alloc_data));
	data->data = g_port_data[idx];
}

static ssize_t cbm_dp_port_alloc_data_write(struct file *file,
					    const char __user *buf,
					    size_t count, loff_t *ppos)
{
	char *param_list[PARAM_LEN];
	char str[STR_LEN];
	int idx;
	int num;

	if (!g_prep) {
		CQM_ERR("Buffers not allocated. echo 1 to prep\n");
		return count;
	}

	if (test_cqm_parse_user_buf(buf, count, str, param_list, PARAM_LEN, &num)) {
		CQM_ERR("Failed to parse input buffer\n");
		return count;
	}

	if (!strncmp(param_list[0], "help", 4)) {
		cbm_dp_port_alloc_data_help();
		return count;
	} else if (!strncmp(param_list[0], "reset", 5)) {
		cbm_dp_port_alloc_data_init(g_index, g_dp_alloc_data[g_index]);
		return count;
	} else if (num % 2) {
		CQM_ERR("num: %d count: %lu str: %s\n", num, count, str);
		cbm_dp_port_alloc_data_help();
		return count;
	}

	for (idx = 0; idx < num; idx += 2) {
		CQM_TRACE("variable:%s value: %s\n", param_list[idx], param_list[idx + 1]);
		if (!strncmp(param_list[idx], "dp_inst", strlen("dp_inst")))
			g_dp_alloc_data[g_index]->dp_inst = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "cbm_inst", strlen("cbm_inst")))
			g_dp_alloc_data[g_index]->cbm_inst = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "flags", strlen("flags")))
			g_dp_alloc_data[g_index]->flags = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "dp_port", strlen("dp_port")))
			g_dp_alloc_data[g_index]->dp_port = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "deq_port_num", strlen("deq_port_num")))
			g_dp_alloc_data[g_index]->deq_port_num = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "deq_port", strlen("deq_port")))
			g_dp_alloc_data[g_index]->deq_port = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "dma_chan", strlen("dma_chan")))
			g_dp_alloc_data[g_index]->dma_chan = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_pkt_credit", strlen("tx_pkt_credit")))
			g_dp_alloc_data[g_index]->tx_pkt_credit = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_b_credit", strlen("tx_b_credit")))
			g_dp_alloc_data[g_index]->tx_b_credit = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_ring_size", strlen("tx_ring_size")))
			g_dp_alloc_data[g_index]->tx_ring_size = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_ring_offset", strlen("tx_ring_offset")))
			g_dp_alloc_data[g_index]->tx_ring_offset = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "num_dma_chan", strlen("num_dma_chan")))
			g_dp_alloc_data[g_index]->num_dma_chan = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.flag_ops", strlen("data.flag_ops")))
			g_dp_alloc_data[g_index]->data->flag_ops = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.resv_num_port", strlen("data.resv_num_port")))
			g_dp_alloc_data[g_index]->data->resv_num_port = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.start_port_no", strlen("data.start_port_no")))
			g_dp_alloc_data[g_index]->data->start_port_no = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.num_resv_q", strlen("data.num_resv_q")))
			g_dp_alloc_data[g_index]->data->num_resv_q = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.num_resv_sched", strlen("data.num_resv_sched")))
			g_dp_alloc_data[g_index]->data->num_resv_sched = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.deq_port_base", strlen("data.deq_port_base")))
			g_dp_alloc_data[g_index]->data->deq_port_base = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.deq_num", strlen("data.deq_num")))
			g_dp_alloc_data[g_index]->data->deq_num = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.link_speed_cap", strlen("data.link_speed_cap")))
			g_dp_alloc_data[g_index]->data->link_speed_cap = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.n_wib_credit_pkt", strlen("data.n_wib_credit_pkt")))
			g_dp_alloc_data[g_index]->data->n_wib_credit_pkt = (u32)dp_atoi(param_list[idx+1]);
		else
			CQM_ERR("member %s not implemented\n", param_list[idx]);
	}

	return count;
}

static int cbm_dp_port_alloc_data_read(struct seq_file *s, void *v)
{
	if (!g_prep) {
		seq_puts(s, "Buffers not allocated. echo 1 to prep\n");
		return -ENOMEM;
	}
	cbm_dp_port_alloc_data_dump(s, g_dp_alloc_data[g_index]);
	return 0;
}

static int cbm_dp_port_alloc_data_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cbm_dp_port_alloc_data_read, inode->i_private);
}

/*************************************************************************************
 *** cbm_dp_port_alloc_fops ***
 ************************************************************************************/
static void cbm_dp_port_alloc_help(void)
{
	CQM_INFO("echo help|(parameter value) > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[1].name);
	CQM_INFO("help: print this help function\n");
	CQM_INFO("(parameter value): call cbm_dp_port_alloc API with parameters. E.g.\n");
	CQM_INFO("\t\t echo dev_port 0 dp_port 0 flags 0x400808 > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[1].name);
	CQM_INFO("\t\t echo dev_port 0 dp_port 12 flags 1 > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[1].name);
}

static void
cbm_dp_port_alloc_dump(struct seq_file *s,
		       struct cbm_dp_port_alloc *data)
{
	if (!data) {
		seq_puts(s, "struct cbm_dp_port_alloc is null\n");
		return;
	}

	seq_puts(s, "struct cbm_dp_port_alloc:\n");
	seq_printf(s, "\t dev_port: %u\n", data->dev_port);
	seq_printf(s, "\t dp_port: %d\n", data->dp_port);
	seq_printf(s, "\t flags: 0x%x\n", data->flags);
	cbm_dp_port_alloc_data_dump(s, data->data);
}

static void
cbm_dp_port_alloc_init(int idx, struct cbm_dp_port_alloc *data)
{
	data->owner = THIS_MODULE;
	data->dev = NULL;
	data->dev_port = 0;
	data->dp_port = 0;
	data->data = g_dp_alloc_data[idx];
	data->flags = 0;
}

static ssize_t cbm_dp_port_alloc_write(struct file *file,
				       const char __user *buf,
				       size_t count, loff_t *ppos)
{
	char *param_list[PARAM_LEN];
	char str[STR_LEN];
	int idx;
	int num;
	s32 ret;

	if (!g_prep) {
		CQM_ERR("Buffers not allocated. echo 1 to prep\n");
		return count;
	}

	if (test_cqm_parse_user_buf(buf, count, str, param_list, PARAM_LEN, &num)) {
		CQM_ERR("Failed to parse input buffer\n");
		return count;
	}

	if (!strncmp(param_list[0], "help", 4)) {
		cbm_dp_port_alloc_help();
		return count;
	} else if (num % 2) {
		CQM_ERR("num: %d count: %lu str: %s\n", num, count, str);
		cbm_dp_port_alloc_help();
		return count;
	}

	for (idx = 0; idx < num; idx += 2) {
		CQM_TRACE("variable:%s value: %s\n", param_list[idx], param_list[idx + 1]);
		if (!strncmp(param_list[idx], "dev_port", strlen("dev_port")))
			g_dp_port_alloc[g_index]->dev_port = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "dp_port", strlen("dp_port")))
			g_dp_port_alloc[g_index]->dp_port = (s32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "flags", strlen("flags")))
			g_dp_port_alloc[g_index]->flags = (u32)dp_atoi(param_list[idx + 1]);
		else
			CQM_ERR("member %s not implemented\n", param_list[idx]);
	}

	if (g_cqm_ctrl && g_cqm_ctrl->cqm_ops && g_cqm_ctrl->cqm_ops->cbm_dp_port_alloc) {
		ret = g_cqm_ctrl->cqm_ops->cbm_dp_port_alloc(g_dp_port_alloc[g_index]->owner,
							     g_dp_port_alloc[g_index]->dev,
							     g_dp_port_alloc[g_index]->dev_port,
							     g_dp_port_alloc[g_index]->dp_port,
							     g_dp_port_alloc[g_index]->data,
							     g_dp_port_alloc[g_index]->flags);
		if (ret)
			CQM_ERR("cbm_dp_port_alloc failed\n");
		else
			CQM_INFO("cbm_dp_port_alloc passed\n");
	}

	return count;
}

static int cbm_dp_port_alloc_read(struct seq_file *s, void *v)
{
	if (!g_prep) {
		seq_puts(s, "Buffers not allocated. echo 1 to prep\n");
		return -ENOMEM;
	}

	cbm_dp_port_alloc_dump(s, g_dp_port_alloc[g_index]);
	return 0;
}

static int cbm_dp_port_alloc_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cbm_dp_port_alloc_read, inode->i_private);
}

/*************************************************************************************
 *** cbm_dp_port_alloc_complete_data_fops ***
 ************************************************************************************/
static void cbm_dp_port_alloc_complete_data_help(void)
{
	CQM_INFO("echo help|reset|(struct_variable value) > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[2].name);
	CQM_INFO("help: print this help function\n");
	CQM_INFO("reset: reset the struct cbm_dp_alloc_complete_data to default values\n");
	CQM_INFO("(struct_variable value): change variable value in struct cbm_dp_alloc_complete_data.\n");
	CQM_INFO("\t\t For example, to change value of cbm_dp_alloc_complete_data.num_rx_ring, do following:\n");
	CQM_INFO("\t\t echo num_rx_ring 1 > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[2].name);
}

static void
cbm_dp_port_alloc_complete_data_dump_dp_rx_ring(struct seq_file *s,
						int idx,
						struct dp_rx_ring *data)
{
	if (!data) {
		seq_printf(s, "\t rx_ring[%d] is null\n", idx);
		return;
	}
	seq_printf(s, "\t rx_ring[%d].out_enq_ring_size: %d\n", idx, data->out_enq_ring_size);
	seq_printf(s, "\t rx_ring[%d].out_enq_port_id: %u\n", idx, data->out_enq_port_id);
	seq_printf(s, "\t rx_ring[%d].out_enq_paddr: %pS\n", idx, data->out_enq_paddr);
	seq_printf(s, "\t rx_ring[%d].out_enq_port_id: %u\n", idx, data->out_enq_port_id);
	seq_printf(s, "\t rx_ring[%d].out_dma_ch_to_gswip: %u\n", idx, data->out_dma_ch_to_gswip);
	seq_printf(s, "\t rx_ring[%d].num_out_tx_dma_ch: %u\n", idx, data->num_out_tx_dma_ch);
	seq_printf(s, "\t rx_ring[%d].out_cqm_deq_port_id: %u\n", idx, data->out_cqm_deq_port_id);
	seq_printf(s, "\t rx_ring[%d].num_out_cqm_deq_port: %u\n", idx, data->num_out_cqm_deq_port);
	seq_printf(s, "\t rx_ring[%d].in_alloc_ring_size: %d\n", idx, data->in_alloc_ring_size);
	seq_printf(s, "\t rx_ring[%d].num_pkt: %u\n", idx, data->num_pkt);
	seq_printf(s, "\t rx_ring[%d].rx_pkt_size: %d\n", idx, data->rx_pkt_size);
	seq_printf(s, "\t rx_ring[%d].rx_policy_base: %u\n", idx, data->rx_policy_base);
	seq_printf(s, "\t rx_ring[%d].policy_num: %u\n", idx, data->policy_num);
	seq_printf(s, "\t rx_ring[%d].rx_poolid: %u\n", idx, data->rx_poolid);
	seq_printf(s, "\t rx_ring[%d].prefill_pkt_num: %d\n", idx, data->prefill_pkt_num);
	seq_printf(s, "\t rx_ring[%d].out_msg_mode: %d\n", idx, data->out_msg_mode);
	seq_printf(s, "\t rx_ring[%d].out_qos_mode: %d\n", idx, data->out_qos_mode);
	seq_printf(s, "\t rx_ring[%d].num_egp: %d\n", idx, data->num_egp);
	seq_printf(s, "\t rx_ring[%d].bpress_out: %d\n", idx, data->bpress_out);
	seq_printf(s, "\t rx_ring[%d].bpress_alloc: %d\n", idx, data->bpress_alloc);
}

static void
cbm_dp_port_alloc_complete_data_dump_dp_tx_ring(struct seq_file *s,
						int idx,
						struct dp_tx_ring *data)
{
	if (!data) {
		seq_printf(s, "\t tx_ring[%d] is null\n", idx);
		return;
	}
	seq_printf(s, "\t tx_ring[%d].in_deq_ring_size: %d\n", idx, data->in_deq_ring_size);
	seq_printf(s, "\t tx_ring[%d].out_free_ring_size: %d\n", idx, data->out_free_ring_size);
	seq_printf(s, "\t tx_ring[%d].num_tx_pkt: %u\n", idx, data->num_tx_pkt);
	seq_printf(s, "\t tx_ring[%d].tx_pkt_size: %d\n", idx, data->tx_pkt_size);
	seq_printf(s, "\t tx_ring[%d].txout_policy_base: %d\n", idx, data->txout_policy_base);
	seq_printf(s, "\t tx_ring[%d].policy_num: %d\n", idx, data->policy_num);
	seq_printf(s, "\t tx_ring[%d].tx_poolid: %u\n", idx, data->tx_poolid);
	seq_printf(s, "\t tx_ring[%d].f_out_auto_free: %u\n", idx, data->f_out_auto_free);
	seq_printf(s, "\t tx_ring[%d].tx_deq_port: %u\n", idx, data->tx_deq_port);
	seq_printf(s, "\t tx_ring[%d].not_valid: %d\n", idx, data->not_valid);
	seq_printf(s, "\t tx_ring[%d].bpress_in: %d\n", idx, data->bpress_in);
	seq_printf(s, "\t tx_ring[%d].bpress_free: %d\n", idx, data->bpress_free);
}

static void
cbm_dp_port_alloc_complete_data_dump_umpt_port_ctl(struct seq_file *s,
						   int idx,
						   struct umt_port_ctl *data)
{
	if (!data) {
		seq_printf(s, "\t umt[%d].ctrl is null\n", idx);
		return;
	}
	seq_printf(s, "\t umt[%d].ctrl.id: %d\n", idx, data->id);
	seq_printf(s, "\t umt[%d].ctrl.daddr: %llx\n", idx, data->daddr);
	seq_printf(s, "\t umt[%d].ctrl.msg_interval: %u\n", idx, data->msg_interval);
	seq_printf(s, "\t umt[%d].ctrl.msg_mode: %d\n", idx, data->msg_mode);
	seq_printf(s, "\t umt[%d].ctrl.cnt_mode: %d\n", idx, data->cnt_mode);
	seq_printf(s, "\t umt[%d].ctrl.sw_msg: %d\n", idx, data->sw_msg);
	seq_printf(s, "\t umt[%d].ctrl.rx_msg_mode: %d\n", idx, data->rx_msg_mode);
	seq_printf(s, "\t umt[%d].ctrl.enable: %u\n", idx, data->enable);
	seq_printf(s, "\t umt[%d].ctrl.fflag: %lu\n", idx, data->fflag);
}

static void
cbm_dp_port_alloc_complete_data_dump_umpt_port_res(struct seq_file *s,
						   int idx,
						   struct umt_port_res *data)
{
	if (!data) {
		seq_printf(s, "\t umt[%d].res is null\n", idx);
		return;
	}
	seq_printf(s, "\t umt[%d].res.dma_id: %u\n", idx, data->dma_id);
	seq_printf(s, "\t umt[%d].res.dma_ch_num: %u\n", idx, data->dma_ch_num);
	seq_printf(s, "\t umt[%d].res.cqm_enq_pid: %u\n", idx, data->cqm_enq_pid);
	seq_printf(s, "\t umt[%d].res.cqm_dq_pid: %u\n", idx, data->cqm_dq_pid);
	seq_printf(s, "\t umt[%d].res.rx_src: %d\n", idx, data->rx_src);
}

static void
cbm_dp_port_alloc_complete_data_dump_dp_umt_port(struct seq_file *s,
						 int idx,
						 struct dp_umt_port *data)
{
	if (!data) {
		seq_printf(s, "\t umt[%d] is null\n", idx);
		return;
	}
	cbm_dp_port_alloc_complete_data_dump_umpt_port_ctl(s, idx, &data->ctl);
	cbm_dp_port_alloc_complete_data_dump_umpt_port_res(s, idx, &data->res);
}

static void
cbm_dp_port_alloc_complete_data_dump_dp_dev_data(struct seq_file *s,
						 struct dp_dev_data *data)
{
	int idx;

	if (!data) {
		seq_puts(s, "struct dp_dev_data is null\n");
		return;
	}

	seq_puts(s, "struct dp_dev_data:\n");
	seq_printf(s, "\t flag_ops: 0x%x\n", data->flag_ops);
	seq_printf(s, "\t num_rx_ring: %u\n", data->num_rx_ring);
	seq_printf(s, "\t num_tx_ring: %u\n", data->num_tx_ring);
	seq_printf(s, "\t num_umt_port: %u\n", data->num_umt_port);

	for (idx = 0; idx < DP_RX_RING_NUM; idx++)
		cbm_dp_port_alloc_complete_data_dump_dp_rx_ring(s, idx, &data->rx_ring[idx]);
	for (idx = 0; idx < DP_TX_RING_NUM; idx++)
		cbm_dp_port_alloc_complete_data_dump_dp_tx_ring(s, idx, &data->tx_ring[idx]);
	for (idx = 0; idx < DP_TX_RING_NUM; idx++)
		cbm_dp_port_alloc_complete_data_dump_dp_umt_port(s, idx, &data->umt[idx]);

	seq_printf(s, "\t max_ctp: %u\n", data->max_ctp);
	seq_printf(s, "\t max_gpid: %u\n", data->max_gpid);
	seq_printf(s, "\t num_resv_q: %d\n", data->num_resv_q);
	seq_printf(s, "\t num_resv_sched: %u\n", data->num_resv_sched);
	seq_printf(s, "\t qos_resv_q_base: %u\n", data->qos_resv_q_base);
	seq_printf(s, "\t wib_tx_phy_addr: 0x%llx\n", data->wib_tx_phy_addr);
	seq_printf(s, "\t bm_policy_res_id: %u\n", data->bm_policy_res_id);
}

static void
cbm_dp_port_alloc_complete_data_dump(struct seq_file *s,
				     struct cbm_dp_alloc_complete_data *data)
{
	if (!data) {
		seq_puts(s, "struct cbm_dp_alloc_complete_data is null\n");
		return;
	}

	seq_puts(s, "struct cbm_dp_alloc_complete_data:\n");
	seq_printf(s, "\t num_rx_ring: %u\n", data->num_rx_ring);
	seq_printf(s, "\t num_tx_ring: %u\n", data->num_tx_ring);
	seq_printf(s, "\t num_umt_port: %u\n", data->num_umt_port);
	seq_printf(s, "\t enable_cqm_meta: %u\n", data->enable_cqm_meta);
	seq_printf(s, "\t qid_base: %d\n", data->qid_base);
	seq_printf(s, "\t num_qid: %d\n", data->num_qid);
	seq_printf(s, "\t bm_policy_res_id: %u\n", data->bm_policy_res_id);
	seq_printf(s, "\t tx_policy_base: %d\n", data->tx_policy_base);
	seq_printf(s, "\t tx_policy_num: %d\n", data->tx_policy_num);

	cbm_dp_port_alloc_complete_data_dump_dp_dev_data(s, data->data);
}

static void
cbm_dp_port_alloc_complete_data_init(int idx, struct cbm_dp_alloc_complete_data *data)
{
	int id;

	memset(g_dev_data[idx], 0, sizeof(struct dp_dev_data));
	memset(data, 0, sizeof(struct cbm_dp_alloc_complete_data));

	data->data = g_dev_data[idx];

	for (id = 0; id < DP_RX_RING_NUM; id++)
		data->rx_ring[id] = &data->data->rx_ring[id];

	for (id = 0; id < DP_RX_RING_NUM; id++)
		data->tx_ring[id] = &data->data->tx_ring[id];
}

static ssize_t
cbm_dp_port_alloc_complete_data_write(struct file *file,
				      const char __user *buf,
				      size_t count, loff_t *ppos)
{
	char *param_list[PARAM_LEN];
	char str[STR_LEN];
	int idx;
	int num;

	if (!g_prep) {
		CQM_ERR("Buffers not allocated. echo 1 to prep\n");
		return count;
	}

	if (test_cqm_parse_user_buf(buf, count, str, param_list, PARAM_LEN, &num)) {
		CQM_ERR("Failed to parse input buffer\n");
		return count;
	}

	if (!strncmp(param_list[0], "help", 4)) {
		cbm_dp_port_alloc_complete_data_help();
		return count;
	} else if (!strncmp(param_list[0], "reset", 5)) {
		cbm_dp_port_alloc_complete_data_init(g_index, g_dp_alloc_complete_data[g_index]);
		return count;
	} else if (num % 2) {
		CQM_ERR("num: %d count: %lu str: %s\n", num, count, str);
		cbm_dp_port_alloc_complete_data_help();
		return count;
	}

	for (idx = 0; idx < num; idx += 2) {
		CQM_TRACE("variable:%s value: %s\n", param_list[idx], param_list[idx + 1]);
		if (!strncmp(param_list[idx], "num_rx_ring", strlen("num_rx_ring")))
			g_dp_alloc_complete_data[g_index]->num_rx_ring = (u8)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "num_tx_ring", strlen("num_tx_ring")))
			g_dp_alloc_complete_data[g_index]->num_tx_ring = (u8)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "num_umt_port", strlen("num_umt_port")))
			g_dp_alloc_complete_data[g_index]->num_umt_port = (u8)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_ring.out_enq_ring_size", strlen("rx_ring.out_enq_ring_size")))
			g_dp_alloc_complete_data[g_index]->rx_ring[g_ring_index]->out_enq_ring_size = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_ring.out_enq_port_id", strlen("rx_ring.out_enq_port_id")))
			g_dp_alloc_complete_data[g_index]->rx_ring[g_ring_index]->out_enq_port_id = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_ring.out_dma_ch_to_gswip", strlen("rx_ring.out_dma_ch_to_gswip")))
			g_dp_alloc_complete_data[g_index]->rx_ring[g_ring_index]->out_dma_ch_to_gswip = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_ring.num_out_tx_dma_ch", strlen("rx_ring.num_out_tx_dma_ch")))
			g_dp_alloc_complete_data[g_index]->rx_ring[g_ring_index]->num_out_tx_dma_ch = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_ring.out_cqm_deq_port_id", strlen("rx_ring.out_cqm_deq_port_id")))
			g_dp_alloc_complete_data[g_index]->rx_ring[g_ring_index]->out_cqm_deq_port_id = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_ring.num_out_cqm_deq_port", strlen("rx_ring.num_out_cqm_deq_port")))
			g_dp_alloc_complete_data[g_index]->rx_ring[g_ring_index]->num_out_cqm_deq_port = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_ring.in_alloc_ring_size", strlen("rx_ring.in_alloc_ring_size")))
			g_dp_alloc_complete_data[g_index]->rx_ring[g_ring_index]->in_alloc_ring_size = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_ring.num_pkt", strlen("rx_ring.num_pkt")))
			g_dp_alloc_complete_data[g_index]->rx_ring[g_ring_index]->num_pkt = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_ring.rx_pkt_size", strlen("rx_ring.rx_pkt_size")))
			g_dp_alloc_complete_data[g_index]->rx_ring[g_ring_index]->rx_pkt_size = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_ring.rx_policy_base", strlen("rx_ring.rx_policy_base")))
			g_dp_alloc_complete_data[g_index]->rx_ring[g_ring_index]->rx_policy_base = (u16)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_ring.policy_num", strlen("rx_ring.policy_num")))
			g_dp_alloc_complete_data[g_index]->rx_ring[g_ring_index]->policy_num = (u8)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_ring.rx_poolid", strlen("rx_ring.rx_poolid")))
			g_dp_alloc_complete_data[g_index]->rx_ring[g_ring_index]->rx_poolid = (u16)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_ring.prefill_pkt_num", strlen("rx_ring.prefill_pkt_num")))
			g_dp_alloc_complete_data[g_index]->rx_ring[g_ring_index]->prefill_pkt_num = (u16)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_ring.out_msg_mode", strlen("rx_ring.out_msg_mode")))
			g_dp_alloc_complete_data[g_index]->rx_ring[g_ring_index]->out_msg_mode = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_ring.out_qos_mode", strlen("rx_ring.out_qos_mode")))
			g_dp_alloc_complete_data[g_index]->rx_ring[g_ring_index]->out_qos_mode = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_ring.num_egp", strlen("rx_ring.num_egp")))
			g_dp_alloc_complete_data[g_index]->rx_ring[g_ring_index]->num_egp = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_ring.bpress_out", strlen("rx_ring.bpress_out")))
			g_dp_alloc_complete_data[g_index]->rx_ring[g_ring_index]->bpress_out = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_ring.bpress_alloc", strlen("rx_ring.bpress_alloc")))
			g_dp_alloc_complete_data[g_index]->rx_ring[g_ring_index]->bpress_alloc = dp_atoi(param_list[idx + 1]);

		else if (!strncmp(param_list[idx], "tx_ring.in_deq_ring_size", strlen("tx_ring.in_deq_ring_size")))
			g_dp_alloc_complete_data[g_index]->tx_ring[g_ring_index]->in_deq_ring_size = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_ring.out_free_ring_size", strlen("tx_ring.out_free_ring_size")))
			g_dp_alloc_complete_data[g_index]->tx_ring[g_ring_index]->out_free_ring_size = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_ring.num_tx_pkt", strlen("tx_ring.num_tx_pkt")))
			g_dp_alloc_complete_data[g_index]->tx_ring[g_ring_index]->num_tx_pkt = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_ring.tx_pkt_size", strlen("tx_ring.tx_pkt_size")))
			g_dp_alloc_complete_data[g_index]->tx_ring[g_ring_index]->tx_pkt_size = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_ring.txout_policy_base", strlen("tx_ring.txout_policy_base")))
			g_dp_alloc_complete_data[g_index]->tx_ring[g_ring_index]->txout_policy_base = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_ring.policy_num", strlen("tx_ring.policy_num")))
			g_dp_alloc_complete_data[g_index]->tx_ring[g_ring_index]->policy_num = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_ring.tx_poolid", strlen("tx_ring.tx_poolid")))
			g_dp_alloc_complete_data[g_index]->tx_ring[g_ring_index]->tx_poolid = (u16)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_ring.f_out_auto_free", strlen("tx_ring.f_out_auto_free")))
			g_dp_alloc_complete_data[g_index]->tx_ring[g_ring_index]->f_out_auto_free = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_ring.tx_deq_port", strlen("tx_ring.tx_deq_port")))
			g_dp_alloc_complete_data[g_index]->tx_ring[g_ring_index]->tx_deq_port = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_ring.not_valid", strlen("tx_ring.not_valid")))
			g_dp_alloc_complete_data[g_index]->tx_ring[g_ring_index]->not_valid = (bool)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_ring.bpress_in", strlen("tx_ring.bpress_in")))
			g_dp_alloc_complete_data[g_index]->tx_ring[g_ring_index]->bpress_in = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_ring.bpress_free", strlen("tx_ring.bpress_free")))
			g_dp_alloc_complete_data[g_index]->tx_ring[g_ring_index]->bpress_free = dp_atoi(param_list[idx + 1]);

		else if (!strncmp(param_list[idx], "enable_cqm_meta", strlen("enable_cqm_meta")))
			g_dp_alloc_complete_data[g_index]->enable_cqm_meta = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "qid_base", strlen("qid_base")))
			g_dp_alloc_complete_data[g_index]->qid_base = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "num_qid", strlen("num_qid")))
			g_dp_alloc_complete_data[g_index]->num_qid = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "bm_policy_res_id", strlen("bm_policy_res_id")))
			g_dp_alloc_complete_data[g_index]->bm_policy_res_id = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_policy_base", strlen("tx_policy_base")))
			g_dp_alloc_complete_data[g_index]->tx_policy_base = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_policy_num", strlen("tx_policy_num")))
			g_dp_alloc_complete_data[g_index]->tx_policy_num = dp_atoi(param_list[idx + 1]);

		else if (!strncmp(param_list[idx], "data.flag_ops", strlen("data.flag_ops")))
			g_dp_alloc_complete_data[g_index]->data->flag_ops = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.num_rx_ring", strlen("data.num_rx_ring")))
			g_dp_alloc_complete_data[g_index]->data->num_rx_ring = (u8)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.num_tx_ring", strlen("data.num_tx_ring")))
			g_dp_alloc_complete_data[g_index]->data->num_tx_ring = (u8)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.num_umt_port", strlen("data.num_umt_port")))
			g_dp_alloc_complete_data[g_index]->data->num_umt_port = (u8)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.enable_cqm_meta", strlen("data.enable_cqm_meta")))
			g_dp_alloc_complete_data[g_index]->data->enable_cqm_meta = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.max_ctp", strlen("data.max_ctp")))
			g_dp_alloc_complete_data[g_index]->data->max_ctp = (u16)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.max_gpid", strlen("data.max_gpid")))
			g_dp_alloc_complete_data[g_index]->data->max_gpid = (u16)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.num_resv_q", strlen("data.num_resv_q")))
			g_dp_alloc_complete_data[g_index]->data->num_resv_q = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.num_resv_sched", strlen("data.num_resv_sched")))
			g_dp_alloc_complete_data[g_index]->data->num_resv_sched = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.qos_resv_q_base", strlen("data.qos_resv_q_base")))
			g_dp_alloc_complete_data[g_index]->data->qos_resv_q_base = (u16)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.wib_tx_phy_addr", strlen("data.wib_tx_phy_addr")))
			g_dp_alloc_complete_data[g_index]->data->wib_tx_phy_addr = (phys_addr_t)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.bm_policy_res_id", strlen("data.bm_policy_res_id")))
			g_dp_alloc_complete_data[g_index]->data->bm_policy_res_id = (u32)dp_atoi(param_list[idx + 1]);

		else if (!strncmp(param_list[idx], "data.umt.ctl.id", strlen("data.umt.ctl.id")))
			g_dp_alloc_complete_data[g_index]->data->umt[g_ring_index].ctl.id = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.umt.ctl.daddr", strlen("data.umt.ctl.daddr")))
			g_dp_alloc_complete_data[g_index]->data->umt[g_ring_index].ctl.daddr = (dma_addr_t)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.umt.ctl.msg_interval", strlen("data.umt.ctl.msg_interval")))
			g_dp_alloc_complete_data[g_index]->data->umt[g_ring_index].ctl.msg_interval = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.umt.ctl.msg_mode", strlen("data.umt.ctl.msg_mode")))
			g_dp_alloc_complete_data[g_index]->data->umt[g_ring_index].ctl.msg_mode = (enum umt_msg_mode)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.umt.ctl.cnt_mode", strlen("data.umt.ctl.cnt_mode")))
			g_dp_alloc_complete_data[g_index]->data->umt[g_ring_index].ctl.cnt_mode = (enum umt_cnt_mode)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.umt.ctl.sw_msg", strlen("data.umt.ctl.sw_msg")))
			g_dp_alloc_complete_data[g_index]->data->umt[g_ring_index].ctl.sw_msg = (enum umt_sw_msg)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.umt.ctl.rx_msg_mode", strlen("data.umt.ctl.rx_msg_mode")))
			g_dp_alloc_complete_data[g_index]->data->umt[g_ring_index].ctl.rx_msg_mode = (enum umt_rx_msg_mode)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.umt.ctl.enable", strlen("data.umt.ctl.enable")))
			g_dp_alloc_complete_data[g_index]->data->umt[g_ring_index].ctl.enable = (unsigned int)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.umt.ctl.fflag", strlen("data.umt.ctl.fflag")))
			g_dp_alloc_complete_data[g_index]->data->umt[g_ring_index].ctl.fflag = (unsigned long)dp_atoi(param_list[idx + 1]);

		else if (!strncmp(param_list[idx], "data.umt.res.dma_id", strlen("data.umt.res.dma_id")))
			g_dp_alloc_complete_data[g_index]->data->umt[g_ring_index].res.dma_id = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.umt.res.dma_ch_num", strlen("data.umt.res.dma_ch_num")))
			g_dp_alloc_complete_data[g_index]->data->umt[g_ring_index].res.dma_ch_num = (unsigned int)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.umt.res.cqm_enq_pid", strlen("data.umt.res.cqm_enq_pid")))
			g_dp_alloc_complete_data[g_index]->data->umt[g_ring_index].res.cqm_enq_pid = (unsigned int)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.umt.res.cqm_dq_pid", strlen("data.umt.res.cqm_dq_pid")))
			g_dp_alloc_complete_data[g_index]->data->umt[g_ring_index].res.cqm_dq_pid = (unsigned int)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.umt.res.rx_src", strlen("data.umt.res.rx_src")))
			g_dp_alloc_complete_data[g_index]->data->umt[g_ring_index].res.rx_src = (enum umt_rx_src)dp_atoi(param_list[idx + 1]);

		else
			CQM_ERR("member %s not implemented\n", param_list[idx]);
	}

	return count;
}

static int cbm_dp_port_alloc_complete_data_read(struct seq_file *s, void *v)
{
	if (!g_prep) {
		seq_puts(s, "Buffers not allocated. echo 1 to prep\n");
		return -ENOMEM;
	}

	cbm_dp_port_alloc_complete_data_dump(s, g_dp_alloc_complete_data[g_index]);
	return 0;
}

static int cbm_dp_port_alloc_complete_data_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cbm_dp_port_alloc_complete_data_read, inode->i_private);
}

/*************************************************************************************
 *** cbm_dp_port_alloc_complete_fops ***
 ************************************************************************************/
static void cbm_dp_port_alloc_complete_help(void)
{
	CQM_INFO("echo help|(parameter value) > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[3].name);
	CQM_INFO("help: print this help function\n");
	CQM_INFO("(parameter value): call cbm_dp_port_alloc_complete API with parameters. E.g.\n");
	CQM_INFO("\t\t echo dev_port 0 dp_port 0 flags 0x400808 > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[3].name);
	CQM_INFO("\t\t echo dev_port 0 dp_port 12 flags 1 > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[3].name);
}

static void
cbm_dp_port_alloc_complete_dump(struct seq_file *s,
				struct cbm_dp_port_alloc_complete *data)
{
	if (!data) {
		seq_puts(s, "struct cbm_dp_port_alloc_complete is null\n");
		return;
	}

	seq_puts(s, "struct cbm_dp_port_alloc_complete:\n");
	seq_printf(s, "\t dev_port: %u\n", data->dev_port);
	seq_printf(s, "\t dp_port: %d\n", data->dp_port);
	seq_printf(s, "\t flags: 0x%x\n", data->flags);
	cbm_dp_port_alloc_complete_data_dump(s, data->data);
}

static void
cbm_dp_port_alloc_complete_init(int idx, struct cbm_dp_port_alloc_complete *data)
{
	data->owner = THIS_MODULE;
	data->dev = NULL;
	data->dev_port = 0;
	data->dp_port = 0;
	data->data = g_dp_alloc_complete_data[idx];
	data->flags = 0;
}

static ssize_t
cbm_dp_port_alloc_complete_write(struct file *file,
				 const char __user *buf,
				 size_t count, loff_t *ppos)
{
	char *param_list[PARAM_LEN];
	char str[STR_LEN];
	int idx;
	int num;
	s32 ret;

	if (!g_prep) {
		CQM_ERR("Buffers not allocated. echo 1 to prep\n");
		return count;
	}

	if (test_cqm_parse_user_buf(buf, count, str, param_list, PARAM_LEN, &num)) {
		CQM_ERR("Failed to parse input buffer\n");
		return count;
	}

	if (!strncmp(param_list[0], "help", 4)) {
		cbm_dp_port_alloc_complete_help();
		return count;
	} else if (num % 2) {
		CQM_ERR("num: %d count: %lu str: %s\n", num, count, str);
		cbm_dp_port_alloc_complete_help();
		return count;
	}

	for (idx = 0; idx < num; idx += 2) {
		CQM_TRACE("variable:%s value: %s\n", param_list[idx], param_list[idx + 1]);
		if (!strncmp(param_list[idx], "dev_port", strlen("dev_port")))
			g_dp_port_alloc_complete[g_index]->dev_port = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "dp_port", strlen("dp_port")))
			g_dp_port_alloc_complete[g_index]->dp_port = (s32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "flags", strlen("flags")))
			g_dp_port_alloc_complete[g_index]->flags = (u32)dp_atoi(param_list[idx + 1]);
		else
			CQM_ERR("member %s not implemented\n", param_list[idx]);
	}

	if (g_cqm_ctrl && g_cqm_ctrl->cqm_ops && g_cqm_ctrl->cqm_ops->cbm_dp_port_alloc_complete) {
		ret = g_cqm_ctrl->cqm_ops->cbm_dp_port_alloc_complete(g_dp_port_alloc_complete[g_index]->owner,
								      g_dp_port_alloc_complete[g_index]->dev,
								      g_dp_port_alloc_complete[g_index]->dev_port,
								      g_dp_port_alloc_complete[g_index]->dp_port,
								      g_dp_port_alloc_complete[g_index]->data,
								      g_dp_port_alloc_complete[g_index]->flags);
		if (ret)
			CQM_ERR("cbm_dp_port_alloc_complete failed\n");
		else
			CQM_INFO("cbm_dp_port_alloc_complete passed\n");
	}

	return count;
}

static int cbm_dp_port_alloc_complete_read(struct seq_file *s, void *v)
{
	if (!g_prep) {
		seq_puts(s, "Buffers not allocated. echo 1 to prep\n");
		return -ENOMEM;
	}

	cbm_dp_port_alloc_complete_dump(s, g_dp_port_alloc_complete[g_index]);
	return 0;
}

static int cbm_dp_port_alloc_complete_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cbm_dp_port_alloc_complete_read, inode->i_private);
}

/*************************************************************************************
 *** cbm_dp_port_enable_data_fops ***
 ************************************************************************************/
static void cbm_dp_port_enable_data_help(void)
{
	CQM_INFO("echo help|reset|(struct_variable value) > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[4].name);
	CQM_INFO("help: print this help function\n");
	CQM_INFO("reset: reset the struct cbm_dp_en_data to default values\n");
	CQM_INFO("(struct_variable value): change variable value in struct cbm_dp_en_data.\n");
	CQM_INFO("\t\t For example, to change value of cbm_dp_en_data.dp_inst, do following:\n");
	CQM_INFO("\t\t echo dp_inst 1 > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[4].name);
}

static void
cbm_dp_port_enable_data_dump_dp_subif_data(struct seq_file *s,
					   struct dp_subif_data *data)
{
	if (!data) {
		seq_puts(s, "struct dp_subif_data is null\n");
		return;
	}

	seq_puts(s, "struct dp_subif_data:\n");
	seq_printf(s, "\t deq_port_idx: %d\n", data->deq_port_idx);
	seq_printf(s, "\t flag_ops: %d\n", data->flag_ops);
	seq_printf(s, "\t q_id: %d\n", data->q_id);
	seq_printf(s, "\t f_policy: %d\n", data->f_policy);
	seq_printf(s, "\t tx_pkt_size: %u\n", data->tx_pkt_size);
	seq_printf(s, "\t tx_policy_base: %u\n", data->tx_policy_base);
	seq_printf(s, "\t tx_policy_num: %u\n", data->tx_policy_num);
	seq_printf(s, "\t rx_policy_base: %u\n", data->rx_policy_base);
	seq_printf(s, "\t rx_policy_num: %u\n", data->rx_policy_num);
	seq_printf(s, "\t mac_learn_disable: %u\n", data->mac_learn_disable);
	seq_printf(s, "\t num_deq_port: %u\n", data->num_deq_port);
	seq_printf(s, "\t rx_en_flag: %u\n", data->rx_en_flag);
	seq_printf(s, "\t bm_policy_res_id: %u\n", data->bm_policy_res_id);
	seq_printf(s, "\t swdev_en_flag: %d\n", data->swdev_en_flag);
	seq_printf(s, "\t domain_id: %d\n", data->domain_id);
	seq_printf(s, "\t domain_members: %d\n", data->domain_members);
}

static void
cbm_dp_port_enable_data_dump(struct seq_file *s,
			     struct cbm_dp_en_data *data)
{
	if (!data) {
		seq_puts(s, "struct cbm_dp_en_data is null\n");
		return;
	}

	seq_puts(s, "struct cbm_dp_en_data:\n");
	seq_printf(s, "\t dp_inst: %d\n", data->dp_inst);
	seq_printf(s, "\t cbm_inst: %d\n", data->cbm_inst);
	seq_printf(s, "\t deq_port: %d\n", data->deq_port);
	seq_printf(s, "\t num_deq_port: %d\n", data->num_deq_port);
	seq_printf(s, "\t dma_chnl_init: %u\n", data->dma_chnl_init);
	seq_printf(s, "\t f_policy: %u\n", data->f_policy);
	seq_printf(s, "\t tx_policy_num: %d\n", data->tx_policy_num);
	seq_printf(s, "\t tx_policy_base: %u\n", data->tx_policy_base);
	seq_printf(s, "\t tx_ring_size: %d\n", data->tx_ring_size);
	seq_printf(s, "\t rx_policy_num: %d\n", data->rx_policy_num);
	seq_printf(s, "\t rx_policy_base: %u\n", data->rx_policy_base);
	seq_printf(s, "\t num_dma_chan: %u\n", data->num_dma_chan);
	seq_printf(s, "\t bm_policy_res_id: %d\n", data->bm_policy_res_id);
	cbm_dp_port_enable_data_dump_dp_subif_data(s, data->data);
}

static void
cbm_dp_port_enable_data_init(int idx, struct cbm_dp_en_data *data)
{
	memset(g_subif_data[idx], 0, sizeof(struct dp_subif_data));
	memset(data, 0, sizeof(struct cbm_dp_en_data));
	data->data = g_subif_data[idx];
}

static ssize_t
cbm_dp_port_enable_data_write(struct file *file,
			      const char __user *buf,
			      size_t count, loff_t *ppos)
{
	char *param_list[PARAM_LEN];
	char str[STR_LEN];
	int idx;
	int num;

	if (!g_prep) {
		CQM_ERR("Buffers not allocated. echo 1 to prep\n");
		return count;
	}

	if (test_cqm_parse_user_buf(buf, count, str, param_list, PARAM_LEN, &num)) {
		CQM_ERR("Failed to parse input buffer\n");
		return count;
	}

	if (!strncmp(param_list[0], "help", 4)) {
		cbm_dp_port_enable_data_help();
		return count;
	} else if (!strncmp(param_list[0], "reset", 5)) {
		cbm_dp_port_enable_data_init(g_index, g_dp_en_data[g_index]);
		return count;
	} else if (num % 2) {
		CQM_ERR("num: %d count: %lu str: %s\n", num, count, str);
		cbm_dp_port_enable_data_help();
		return count;
	}

	for (idx = 0; idx < num; idx += 2) {
		CQM_TRACE("variable:%s value: %s\n", param_list[idx], param_list[idx + 1]);
		if (!strncmp(param_list[idx], "dp_inst", strlen("dp_inst")))
			g_dp_en_data[g_index]->dp_inst = (u8)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "cbm_inst", strlen("cbm_inst")))
			g_dp_en_data[g_index]->cbm_inst = (u8)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "deq_port", strlen("deq_port")))
			g_dp_en_data[g_index]->deq_port = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "num_deq_port", (u32)strlen("num_deq_port")))
			g_dp_en_data[g_index]->num_deq_port = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "dma_chnl_init", strlen("dma_chnl_init")))
			g_dp_en_data[g_index]->dma_chnl_init = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "f_policy", strlen("f_policy")))
			g_dp_en_data[g_index]->f_policy = (u8)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_policy_num", strlen("tx_policy_num")))
			g_dp_en_data[g_index]->tx_policy_num = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_policy_base", strlen("tx_policy_base")))
			g_dp_en_data[g_index]->tx_policy_base = (u16)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "tx_ring_size", strlen("tx_ring_size")))
			g_dp_en_data[g_index]->tx_ring_size = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_policy_num", strlen("rx_policy_num")))
			g_dp_en_data[g_index]->rx_policy_num = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "rx_policy_base", strlen("rx_policy_base")))
			g_dp_en_data[g_index]->rx_policy_base = (u16)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "num_dma_chan", strlen("num_dma_chan")))
			g_dp_en_data[g_index]->num_dma_chan = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "bm_policy_res_id", strlen("bm_policy_res_id")))
			g_dp_en_data[g_index]->bm_policy_res_id = (u32)dp_atoi(param_list[idx + 1]);

		else if (!strncmp(param_list[idx], "data.deq_port_idx", strlen("data.deq_port_idx")))
			g_dp_en_data[g_index]->data->deq_port_idx = (s8)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.flag_ops", strlen("data.flag_ops")))
			g_dp_en_data[g_index]->data->flag_ops = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.q_id", strlen("data.q_id")))
			g_dp_en_data[g_index]->data->q_id = (u8)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.f_policy", strlen("data.f_policy")))
			g_dp_en_data[g_index]->data->f_policy = (u8)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.tx_pkt_size", strlen("data.tx_pkt_size")))
			g_dp_en_data[g_index]->data->tx_pkt_size = (u16)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.tx_policy_num", strlen("data.tx_policy_num")))
			g_dp_en_data[g_index]->data->tx_policy_num = (u8)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.rx_policy_base", strlen("data.rx_policy_base")))
			g_dp_en_data[g_index]->data->rx_policy_base = (u16)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.rx_policy_num", strlen("data.rx_policy_num")))
			g_dp_en_data[g_index]->data->rx_policy_num = (u8)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.txin_ring_size", strlen("data.txin_ring_size")))
			g_dp_en_data[g_index]->data->txin_ring_size = dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.mac_learn_disable", strlen("data.mac_learn_disable")))
			g_dp_en_data[g_index]->data->mac_learn_disable = (u16)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.num_deq_port", strlen("data.num_deq_port")))
			g_dp_en_data[g_index]->data->num_deq_port = (u16)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.rx_en_flag", strlen("data.rx_policrx_en_flagy_num")))
			g_dp_en_data[g_index]->data->rx_en_flag = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.bm_policy_res_id", strlen("data.bm_policy_res_id")))
			g_dp_en_data[g_index]->data->bm_policy_res_id = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.swdev_en_flag", strlen("data.swdev_en_flag")))
			g_dp_en_data[g_index]->data->swdev_en_flag = (u16)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.domain_id", strlen("data.domain_id")))
			g_dp_en_data[g_index]->data->domain_id = (u8)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "data.domain_members", strlen("data.domain_members")))
			g_dp_en_data[g_index]->data->domain_members = (u32)dp_atoi(param_list[idx + 1]);
		else
			CQM_ERR("member %s not implemented\n", param_list[idx]);
	}

	return count;
}

static int cbm_dp_port_enable_data_read(struct seq_file *s, void *v)
{
	if (!g_prep) {
		seq_puts(s, "Buffers not allocated. echo 1 to prep\n");
		return -ENOMEM;
	}

	cbm_dp_port_enable_data_dump(s, g_dp_en_data[g_index]);
	return 0;
}

static int cbm_dp_port_enable_data_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cbm_dp_port_enable_data_read, inode->i_private);
}

/*************************************************************************************
 *** cbm_dp_port_enale_fops ***
 ************************************************************************************/
static void cbm_dp_port_enale_help(void)
{
	CQM_INFO("echo help|(parameter value) > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[5].name);
	CQM_INFO("help: print this help function\n");
	CQM_INFO("(parameter value): call cbm_dp_port_enable API with parameters. E.g.\n");
	CQM_INFO("\t\t echo dp_port 12 flags 0x400808 > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[5].name);
}

static void
cbm_dp_port_enale_dump(struct seq_file *s,
		       struct cbm_dp_port_enable *data)
{
	if (!data) {
		seq_puts(s, "struct cbm_dp_port_enable is null\n");
		return;
	}

	seq_puts(s, "struct cbm_dp_port_enable:\n");
	seq_printf(s, "\t dp_port: %d\n", data->dp_port);
	seq_printf(s, "\t flags: 0x%x\n", data->flags);
	seq_printf(s, "\t alloc_flags: 0x%x\n", data->alloc_flags);
	cbm_dp_port_enable_data_dump(s, data->data);
}

static void
cbm_dp_port_enale_init(int idx, struct cbm_dp_port_enable *data)
{
	data->owner = THIS_MODULE;
	data->dp_port = 0;
	data->data = g_dp_en_data[idx];
	data->flags = 0;
	data->alloc_flags = 0;
}

static ssize_t
cbm_dp_port_enale_write(struct file *file,
			const char __user *buf,
			size_t count, loff_t *ppos)
{
	char *param_list[PARAM_LEN];
	char str[STR_LEN];
	int idx;
	int num;
	s32 ret;

	if (!g_prep) {
		CQM_ERR("Buffers not allocated. echo 1 to prep\n");
		return count;
	}

	if (test_cqm_parse_user_buf(buf, count, str, param_list, PARAM_LEN, &num)) {
		CQM_ERR("Failed to parse input buffer\n");
		return count;
	}

	if (!strncmp(param_list[0], "help", 4)) {
		cbm_dp_port_enale_help();
		return count;
	} else if (num % 2) {
		CQM_ERR("num: %d count: %lu str: %s\n", num, count, str);
		cbm_dp_port_enale_help();
		return count;
	}

	for (idx = 0; idx < num; idx += 2) {
		CQM_TRACE("variable:%s value: %s\n", param_list[idx], param_list[idx + 1]);
		if (!strncmp(param_list[idx], "dp_port", strlen("dp_port")))
			g_dp_port_enable[g_index]->dp_port = (s32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "flags", strlen("flags")))
			g_dp_port_enable[g_index]->flags = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "alloc_flags", strlen("alloc_flags")))
			g_dp_port_enable[g_index]->alloc_flags = (u32)dp_atoi(param_list[idx + 1]);
		else
			CQM_ERR("member %s not implemented\n", param_list[idx]);
	}

	if (g_cqm_ctrl && g_cqm_ctrl->cqm_ops && g_cqm_ctrl->cqm_ops->cbm_dp_enable) {
		ret = g_cqm_ctrl->cqm_ops->cbm_dp_enable(g_dp_port_enable[g_index]->owner,
							 g_dp_port_enable[g_index]->dp_port,
							 g_dp_port_enable[g_index]->data,
							 g_dp_port_enable[g_index]->flags);
		if (ret)
			CQM_ERR("cbm_dp_enable failed\n");
		else
			CQM_INFO("cbm_dp_enable passed\n");
	}

	return count;
}

static int cbm_dp_port_enale_read(struct seq_file *s, void *v)
{
	if (!g_prep) {
		seq_puts(s, "Buffers not allocated. echo 1 to prep\n");
		return -ENOMEM;
	}

	cbm_dp_port_enale_dump(s, g_dp_port_enable[g_index]);
	return 0;
}

static int cbm_dp_port_enale_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cbm_dp_port_enale_read, inode->i_private);
}

/*************************************************************************************
 *** cbm_dp_port_dealloc_fops ***
 ************************************************************************************/
static void cbm_dp_port_dealloc_help(void)
{
	CQM_INFO("echo help|(parameter value) > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[6].name);
	CQM_INFO("help: print this help function\n");
	CQM_INFO("(parameter value): call cbm_dp_port_alloc API with parameters. E.g.\n");
	CQM_INFO("\t\t echo dev_port 0 dp_port 0 flags 0x400808 > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[6].name);
	CQM_INFO("\t\t echo dev_port 0 dp_port 12 flags 1 > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[6].name);
}

static ssize_t cbm_dp_port_dealloc_write(struct file *file,
					 const char __user *buf,
					 size_t count, loff_t *ppos)
{
	char *param_list[PARAM_LEN];
	char str[STR_LEN];
	int idx;
	int num;
	s32 ret;

	if (!g_prep) {
		CQM_ERR("Buffers not allocated. echo 1 to prep\n");
		return count;
	}

	if (test_cqm_parse_user_buf(buf, count, str, param_list, PARAM_LEN, &num)) {
		CQM_ERR("Failed to parse input buffer\n");
		return count;
	}

	if (!strncmp(param_list[0], "help", 4)) {
		cbm_dp_port_dealloc_help();
		return count;
	} else if (num % 2) {
		CQM_ERR("num: %d count: %lu str: %s\n", num, count, str);
		cbm_dp_port_dealloc_help();
		return count;
	}

	for (idx = 0; idx < num; idx += 2) {
		CQM_TRACE("variable:%s value: %s\n", param_list[idx], param_list[idx + 1]);
		if (!strncmp(param_list[idx], "dev_port", strlen("dev_port")))
			g_dp_port_alloc[g_index]->dev_port = (u32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "dp_port", strlen("dp_port")))
			g_dp_port_alloc[g_index]->dp_port = (s32)dp_atoi(param_list[idx + 1]);
		else if (!strncmp(param_list[idx], "flags", strlen("flags")))
			g_dp_port_alloc[g_index]->flags = (u32)dp_atoi(param_list[idx + 1]);
		else
			CQM_ERR("member %s not implemented\n", param_list[idx]);
	}

	if (g_cqm_ctrl && g_cqm_ctrl->cqm_ops && g_cqm_ctrl->cqm_ops->cbm_dp_port_dealloc) {
		ret = g_cqm_ctrl->cqm_ops->cbm_dp_port_dealloc(g_dp_port_alloc[g_index]->owner,
							       g_dp_port_alloc[g_index]->dev_port,
							       g_dp_port_alloc[g_index]->dp_port,
							       g_dp_port_alloc[g_index]->data,
							       g_dp_port_alloc[g_index]->flags);
		if (ret)
			CQM_ERR("cbm_dp_port_dealloc failed\n");
		else
			CQM_INFO("cbm_dp_port_dealloc passed\n");
	}

	return count;
}

/*************************************************************************************
 *** cbm_index_fops ***
 ************************************************************************************/
static void cbm_index_help(void)
{
	CQM_INFO("echo help|index > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[7].name);
	CQM_INFO("help: print this help function\n");
	CQM_INFO("index: index to update struct and perform API, 0-%d\n", CQM_TEST_MAX_INTF);
}

static ssize_t
cbm_index_write(struct file *file,
		const char __user *buf,
		size_t count, loff_t *ppos)
{
	char *param_list[PARAM_LEN];
	char str[STR_LEN];
	int idx;
	int num;

	if (test_cqm_parse_user_buf(buf, count, str, param_list, PARAM_LEN, &num)) {
		CQM_ERR("Failed to parse input buffer\n");
		return count;
	}

	if (!strncmp(param_list[0], "help", 4)) {
		cbm_index_help();
		return count;
	}

	idx = dp_atoi(param_list[0]);
	if (idx < 0 || idx >= CQM_TEST_MAX_INTF) {
		CQM_ERR("idx %d is invalid\n", idx);
		return count;
	}

	g_index = idx;
	CQM_ERR("g_index: %d\n", g_index);

	return count;
}

static int cbm_index_read(struct seq_file *s, void *v)
{
	seq_printf(s, "g_index: %d\n", g_index);
	return 0;
}

static int cbm_index_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cbm_index_read, inode->i_private);
}

/*************************************************************************************
 *** cbm_index_fops ***
 ************************************************************************************/
static void cbm_ring_index_help(void)
{
	CQM_INFO("echo help|index > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[8].name);
	CQM_INFO("help: print this help function\n");
	CQM_INFO("index: ring index to rx_ring or tx_ring. 0-%d\n", DP_RX_RING_NUM);
}

static ssize_t
cbm_ring_index_write(struct file *file,
		     const char __user *buf,
		     size_t count, loff_t *ppos)
{
	char *param_list[PARAM_LEN];
	char str[STR_LEN];
	int idx;
	int num;

	if (test_cqm_parse_user_buf(buf, count, str, param_list, PARAM_LEN, &num)) {
		CQM_ERR("Failed to parse input buffer\n");
		return count;
	}

	if (!strncmp(param_list[0], "help", 4)) {
		cbm_ring_index_help();
		return count;
	}

	idx = dp_atoi(param_list[0]);
	if (idx < 0 || idx >= DP_RX_RING_NUM) {
		CQM_ERR("idx %d is invalid\n", idx);
		return count;
	}

	g_ring_index = idx;
	CQM_ERR("g_ring_index: %d\n", g_ring_index);

	return count;
}

static int cbm_ring_index_read(struct seq_file *s, void *v)
{
	seq_printf(s, "g_ring_index: %d\n", g_ring_index);
	return 0;
}

static int cbm_ring_index_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, cbm_ring_index_read, inode->i_private);
}

/*************************************************************************************
 *** prep_fops ***
 ************************************************************************************/
static void prep_help(void)
{
	CQM_INFO("echo help|prep > /sys/kernel/debug/%s/%s\n",
		 CQM_TEST_DEBUGFS_PATH, test_cqm_dbg_entries[9].name);
	CQM_INFO("help: print this help function\n");
	CQM_INFO("prep: 1. Needed before other APIs\n");
}

static ssize_t prep_write(struct file *file,
			  const char __user *buf,
			  size_t count, loff_t *ppos)
{
	char *param_list[PARAM_LEN];
	char str[STR_LEN];
	int prep;
	int num;

	if (test_cqm_parse_user_buf(buf, count, str, param_list, PARAM_LEN, &num)) {
		CQM_ERR("Failed to parse input buffer\n");
		return count;
	}

	if (!strncmp(param_list[0], "help", 4)) {
		prep_help();
		return count;
	}

	prep = dp_atoi(param_list[0]);
	if (prep < 0) {
		CQM_ERR("idx %d is invalid\n", prep);
		return count;
	}

	g_prep = prep;
	CQM_ERR("g_prep: %d\n", g_prep);
	if (g_prep)
		test_cqm_alloc_mem();
	else
		test_cqm_free_mem();

	return count;
}

static int prep_read(struct seq_file *s, void *v)
{
	seq_printf(s, "g_prep: %d\n", g_prep);
	return 0;
}

static int prep_open(struct inode *inode, struct file *file)
{
	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;
	return single_open(file, prep_read, inode->i_private);
}

/*************************************************************************************
 *** end of ops ***
 ************************************************************************************/

static int test_cqm_debugfs_init(void)
{
	struct dentry *file;
	u8 i;

	g_test_cqm_debugfs = debugfs_create_dir(CQM_TEST_DEBUGFS_PATH, NULL);

	for (i = 0; i < sizeof(test_cqm_dbg_entries) / sizeof(struct test_cqm_dbg_entry);
	     i++) {
		file = debugfs_create_file(test_cqm_dbg_entries[i].name,
					   0400, g_test_cqm_debugfs,
					   NULL,
					   &test_cqm_dbg_entries[i].ops);
		if (!file) {
			debugfs_remove_recursive(g_test_cqm_debugfs);
			return -ENOMEM;
		}
	}

	return 0;
}

static void test_cqm_debugfs_term(void)
{
	debugfs_remove_recursive(g_test_cqm_debugfs);
}

static void test_cqm_free_mem(void)
{
	int idx;

	for (idx = 0; idx < CQM_TEST_MAX_INTF; idx++) {
		if (g_dp_alloc_data[idx])
			kfree(g_dp_alloc_data[idx]);
		if (g_port_data[idx])
			kfree(g_port_data[idx]);
		if (g_dp_port_alloc[idx])
			kfree(g_dp_port_alloc[idx]);
		if (g_dp_alloc_complete_data[idx])
			kfree(g_dp_alloc_complete_data[idx]);
		if (g_dev_data[idx])
			kfree(g_dev_data[idx]);
		if (g_dp_port_alloc_complete[idx])
			kfree(g_dp_port_alloc_complete[idx]);
		if (g_dp_en_data[idx])
			kfree(g_dp_en_data[idx]);
		if (g_subif_data[idx])
			kfree(g_subif_data[idx]);
		if (g_dp_port_enable[idx])
			kfree(g_dp_port_enable[idx]);
	}

	g_prep = 0;
}

static int test_cqm_alloc_mem(void)
{
	int idx;

	for (idx = 0; idx < CQM_TEST_MAX_INTF; idx++) {
		g_dp_alloc_data[idx] = (struct cbm_dp_alloc_data *)kmalloc(sizeof(struct cbm_dp_alloc_data), GFP_ATOMIC);
		g_port_data[idx] = (struct dp_port_data *)kmalloc(sizeof(struct dp_port_data), GFP_ATOMIC);
		g_dp_port_alloc[idx] = (struct cbm_dp_port_alloc *)kmalloc(sizeof(struct cbm_dp_port_alloc), GFP_ATOMIC);
		g_dp_alloc_complete_data[idx] = (struct cbm_dp_alloc_complete_data *)kmalloc(sizeof(struct cbm_dp_alloc_complete_data), GFP_ATOMIC);
		g_dev_data[idx] = (struct dp_dev_data *)kmalloc(sizeof(struct dp_dev_data), GFP_ATOMIC);
		g_dp_port_alloc_complete[idx] = (struct cbm_dp_port_alloc_complete *)kmalloc(sizeof(struct cbm_dp_port_alloc_complete), GFP_ATOMIC);
		g_dp_en_data[idx] = (struct cbm_dp_en_data *)kmalloc(sizeof(struct cbm_dp_en_data), GFP_ATOMIC);
		g_subif_data[idx] = (struct dp_subif_data *)kmalloc(sizeof(struct dp_subif_data), GFP_ATOMIC);
		g_dp_port_enable[idx] = (struct cbm_dp_port_enable *)kmalloc(sizeof(struct cbm_dp_port_enable), GFP_ATOMIC);
		if (!g_dp_alloc_data[idx] ||
		    !g_port_data[idx] ||
		    !g_dp_port_alloc[idx] ||
		    !g_dp_alloc_complete_data[idx] ||
		    !g_dev_data[idx] ||
		    !g_dp_port_alloc_complete[idx] ||
		    !g_dp_en_data[idx] ||
		    !g_subif_data[idx] ||
		    !g_dp_port_enable[idx]) {
			CQM_ERR("Failed to allocate memory\n");
			test_cqm_free_mem();
			return -ENOMEM;
		}

		cbm_dp_port_alloc_data_init(idx, g_dp_alloc_data[idx]);
		cbm_dp_port_alloc_init(idx, g_dp_port_alloc[idx]);

		cbm_dp_port_alloc_complete_data_init(idx, g_dp_alloc_complete_data[idx]);
		cbm_dp_port_alloc_complete_init(idx, g_dp_port_alloc_complete[idx]);

		cbm_dp_port_enable_data_init(idx, g_dp_en_data[idx]);
		cbm_dp_port_enale_init(idx, g_dp_port_enable[idx]);
	}

	/* Set variable g_prep to indicate memory has been allocated */
	g_prep = 1;
	return 0;
}

static int test_cqm_get_cqm_drv_data(char *compatible)
{
	struct platform_device *pdev;
	struct device_node *node;

	/* Due to CQM implementation, this mechanism requires CQM
	 * driver to find the platform device with compitble
	 * device node and set cqm_ctrl as the driver data.
	 * Otherwise, this test module will not be able to find
	 * the cqm_ops.
	 */
	node = of_find_compatible_node(NULL, NULL, compatible);
	if (!node) {
		CQM_ERR("Unable to find CQM node\n");
		return -ENODEV;
	}
	of_node_put(node);

	pdev = of_find_device_by_node(node);
	if (!pdev) {
		CQM_ERR("Unable to find CQM platform device\n");
		return -ENODEV;
	}
	g_cqm_ctrl = (struct cqm_ctrl *)platform_get_drvdata(pdev);
	if (!g_cqm_ctrl) {
		CQM_ERR("Unable to find CQM platform driver data\n");
		return -ENODEV;
	}
	return 0;
}

static int __init test_cqm_init(void)
{
	int ret = 0;

	/* Get cqm_ops via CQM driver data */
	ret = test_cqm_get_cqm_drv_data("mxl,lgm-cqm");
	if (ret) {
		CQM_ERR("Failed to get CQM driver data\n");
		return ret;
	}

	ret = test_cqm_alloc_mem();
	if (ret) {
		CQM_ERR("Failed to allocate memory\n");
		return ret;
	}

	/* Initialise debugfs */
	ret = test_cqm_debugfs_init();
	if (ret) {
		CQM_ERR("Failed to init debug fs\n");
		return ret;
	}
	CQM_TRACE("test_cqm init!\n");

	return ret;
}

static void __exit test_cqm_exit(void)
{
	test_cqm_debugfs_term();
	test_cqm_free_mem();
	CQM_TRACE("test_cqm exit\n");
}

module_init(test_cqm_init);
module_exit(test_cqm_exit);

MODULE_AUTHOR("Leipo Yan");
MODULE_DESCRIPTION("CQM unit test module");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0.0");
