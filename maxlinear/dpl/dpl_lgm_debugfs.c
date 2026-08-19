/*
 * Copyright (C) 2020-2024 MaxLinear, Inc.
 * Copyright (C) 2020 Intel Corporation
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
 * Description: dpl module debugfs
 */

#define pr_fmt(fmt) "dpl_lgm_dbg: %s:%d: " fmt, __func__, __LINE__

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/debugfs.h>
#include <linux/parser.h>
#include <linux/pp_api.h>
#include <linux/dpl.h>
#include "dpl_lgm_internal.h"
#include "dpl_monitor.h"

#define DBGFS_STR_MAX   (1024)

enum set_action_opts {
	set_action_opt_action = 1,
	set_action_opt_type,
	set_action_opt_attr,
	set_action_opt_val,
	set_action_opt_seq_iter,
	set_action_opt_iter,
	set_action_opt_dep
};

static const match_table_t set_action_tokens = {
	{set_action_opt_action,     "action=%u"},
	{set_action_opt_type,       "type=%u"},
	{set_action_opt_attr,       "attr=%u"},
	{set_action_opt_val,        "val=%u"},
	{set_action_opt_seq_iter,   "seq_iter=%u"},
	{set_action_opt_iter,       "iter=%u"},
	{set_action_opt_dep,        "dep=%u"},
	{ 0 },
};

static int __dbg_set_action_set_rd(struct seq_file *f, void *offset)
{
	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	pr_info("\n");
	pr_info("Configure action. Example:\n");
	pr_info("  echo action=0 type=1 attr=0 val=800 iter=5 dep=4 > set_action\n");
	pr_info("  action - 0 - %u\n", DPL_LGM_ACTION_MAX - 1);
	pr_info("         - [%u] %s\n", DPL_LGM_ACTION_WHITE_LIST,
		"White list");
	pr_info("         - [%u] %s\n", DPL_LGM_ACTION_L2_CLASS,
		"L2 classification");
	pr_info("         - [%u] %s\n",
		DPL_LGM_ACTION_INC_MIN_PKT_FOR_SESS_CREATE,
		"Inc min packets for session creation");
	pr_info("         - [%u] %s\n",
		DPL_LGM_ACTION_RESTRICT_DFLT_QUEUES_BW_LIMIT,
		"Restrict BW-Limit on default CPU queues under attack");

	pr_info("  type    - On = %u; OFF = %u\n", THR_ON, THR_OFF);
	pr_info("  attr    - 0 - %u\n", DPL_ATTR_MAX - 1);
	pr_info("          - [%u] %s\n", DPL_PPS, "PPS");
	pr_info("          - [%u] %s\n", DPL_DPS, "DPS");
	pr_info("          - [%u] %s\n", DPL_SPS, "SPS");
	pr_info("          - [%u] %s\n", DPL_SUT, "SUT");
	pr_info("          - [%u] %s\n", DPL_CPU, "CPU");
	pr_info("  val     - threshold value\n");
	pr_info("  seq_val - number of continues iterations\n");
	pr_info("  iter    - number of iterations\n");
	pr_info("  dep     - dependency attribute\n");
	pr_info("\n");

	return 0;
}

/**
 * @brief configure action
 */
void __dpl_set_action_set(char *args, void *data)
{
	substring_t substr[32];
	enum set_action_opts opt;
	char *tok;
	int ret, val;
	/* Default values */
	unsigned int action = 0;
	unsigned int type = 0;
	unsigned int attr = 0;
	unsigned int threshold = 0;
	unsigned int seq_iter = 0;
	unsigned int iter = 0;
	unsigned int dep = 0;

	/* iterate over user arguments */
	while ((tok = strsep(&args, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, set_action_tokens, substr);
		switch (opt) {
		case set_action_opt_action:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret) || action >= DPL_LGM_ACTION_MAX)
				goto parse_err;
			action = (unsigned int)val;
			break;
		case set_action_opt_type:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			type = (unsigned int)val;
			break;
		case set_action_opt_attr:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			attr = (unsigned int)val;
			break;
		case set_action_opt_val:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			threshold = (unsigned int)val;
			break;
		case set_action_opt_seq_iter:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			seq_iter = (unsigned int)val;
			break;
		case set_action_opt_iter:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			iter = (unsigned int)val;
			break;
		case set_action_opt_dep:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			dep = (unsigned int)val;
			break;
		default:
			goto parse_err;
		}
	}

	dpl_set_action(action, type, attr, threshold, seq_iter, iter, dep, true);

	return;

parse_err:
	pr_err("failed to parse %s\n", tok);
}

static int __dbg_set_action_set_open(struct inode *inode, struct file *file)
{
	return single_open(file, __dbg_set_action_set_rd, inode->i_private);
}

static ssize_t __dbg_set_action_set_wr(struct file *file,
				       const char __user *buf,
				       size_t count, loff_t *pos)
{
	int ret;
	char cmd_buf[DBGFS_STR_MAX];

	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	if ((sizeof(cmd_buf) - 1) < count)
		return -E2BIG;
	ret = simple_write_to_buffer(cmd_buf, count, pos, buf, count);
	if (unlikely(ret != count))
		return ret >= 0 ? -EIO : ret;
	cmd_buf[count - 1] = '\0';
	__dpl_set_action_set(cmd_buf, file->f_inode->i_private);
	return count;
}

static const struct file_operations dbg_set_action_fops = {
	.open    = __dbg_set_action_set_open,
	.read    = seq_read,
	.write   = __dbg_set_action_set_wr,
	.llseek  = seq_lseek,
	.release = single_release,
};

enum protect_opts {
	protect_opt_help = 1,
	protect_opt_operation,
	protect_opt_dev
};

static const match_table_t protect_tokens = {
	{protect_opt_help,       "help"},
	{protect_opt_operation,  "op=%u"},
	{protect_opt_dev,        "dev=%s"},
	{ 0 },
};

enum operation {
	operation_protect,
	operation_unprotect,
	operation_max
};

static int __dbg_dev_protect_set_rd(struct seq_file *f, void *offset)
{
	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	dpl_protected_devs_dump();

	pr_info(" \n");
	pr_info("  Add/Remove device protection Usage:\n");
	pr_info("  echo op=0 dev=eth1 > dev_protect\n");
	pr_info("  op   - 0 - Add / 1- Remove\n");
	pr_info("  dev - device name\n");

	return 0;
}

static int __dbg_dev_protect_set_open(struct inode *inode, struct file *file)
{
	return single_open(file, __dbg_dev_protect_set_rd, inode->i_private);
}

/**
 * @brief configure dev_protects
 */
static void __dpl_dev_protect_set(char *args, void *data)
{
	substring_t substr[32];
	enum protect_opts opt;
	char *tok, name[IFNAMSIZ];
	int ret, val;
	enum operation op = operation_max;
	struct net_device *dev;

	/* iterate over user arguments */
	while ((tok = strsep(&args, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, protect_tokens, substr);
		switch (opt) {
		case protect_opt_help:
			__dbg_dev_protect_set_rd(NULL, NULL);
			break;
		case protect_opt_operation:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret) || val >= operation_max)
				goto parse_err;
			op = (unsigned int)val;
			break;
		case protect_opt_dev:
			match_strlcpy(name, substr, sizeof(name));
			break;
		default:
			goto parse_err;
		}
	}

	dev = dev_get_by_name(&init_net, name);
	if (!dev) {
		pr_err("can't get the net_device from name = %s\n", name);
		return;
	}

	/* set dev_protect */
	if (op == operation_protect)
		dpl_dev_protect(dev);
	else if (op == operation_unprotect)
		dpl_dev_unprotect(dev);

	dev_put(dev);

	return;

parse_err:
	pr_err("failed to parse %s\n", tok);
}

static ssize_t __dbg_dev_protect_set_wr(struct file *file,
					 const char __user *buf,
					 size_t count, loff_t *pos)
{
	int ret;
	char cmd_buf[DBGFS_STR_MAX];

	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	if ((sizeof(cmd_buf) - 1) < count)
		return -E2BIG;
	ret = simple_write_to_buffer(cmd_buf, count, pos, buf, count);
	if (unlikely(ret != count))
		return ret >= 0 ? -EIO : ret;
	cmd_buf[count - 1] = '\0';
	__dpl_dev_protect_set(cmd_buf, file->f_inode->i_private);
	return count;
}

static const struct file_operations dbg_dev_protect_fops = {
	.open    = __dbg_dev_protect_set_open,
	.read    = seq_read,
	.write   = __dbg_dev_protect_set_wr,
	.llseek  = seq_lseek,
	.release = single_release,
};

enum test_opts {
	test_opt_help = 1,
	test_opt_operation,
	test_opt_port,
	test_opt_queue
};

static const match_table_t test_tokens = {
	{test_opt_help,       "help"},
	{test_opt_operation,  "op=%u"},
	{test_opt_port,       "port=%u"},
	{test_opt_queue,      "queue=%u"},
	{ 0 },
};

static int __dbg_test_set_rd(struct seq_file *f, void *offset)
{
	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	seq_puts(f, "\n");

	seq_puts(f, " DPL platform test. Usage:\n");
	seq_puts(f, "  echo op=1 queue=1 > test\n");
	seq_printf(f, "  op %u - dump dp info\n",
		   test_operation_dump_dp_info);
	seq_printf(f, "  op %u - Dump protected ports\n",
		   test_operation_dump_protected);
	seq_printf(f, "  op %u - White list enable\n",
		   test_operation_white_list_en);
	seq_printf(f, "  op %u - White list disable\n",
		   test_operation_white_list_dis);
	seq_printf(f, "  op %u - DPL Enable\n",
		   test_operation_dpl_enable);
	seq_printf(f, "  op %u - Dump mon params\n",
		   test_operation_dump_mon_params);
	seq_printf(f, "  op %u - Dump Actions state\n",
		   test_operation_dump_actions_state);
	seq_puts(f, "  port  - port gpid\n");
	seq_puts(f, "  queue - queue logical id\n");
	seq_puts(f, "\n");

	return 0;
}

/**
 * @brief configure test
 */
static void __dpl_test_set(char *args, void *data)
{
	substring_t substr[32];
	enum test_opts opt;
	char *tok;
	int ret, val;
	unsigned int port = UINT_MAX;
	unsigned int queue = UINT_MAX;
	enum test_operation op = test_operation_max;

	pr_info("DPL platform test\n");

	/* iterate over user arguments */
	while ((tok = strsep(&args, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, test_tokens, substr);
		switch (opt) {
		case test_opt_help:
			__dbg_test_set_rd(NULL, NULL);
			break;
		case test_opt_operation:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret) || val >= test_operation_max)
				goto parse_err;
			op = (unsigned int)val;
			break;
		case test_opt_port:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret) || val >= PP_MAX_PORT)
				goto parse_err;
			port = (unsigned int)val;
			break;
		case test_opt_queue:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			queue = (unsigned int)val;
			break;
		default:
			goto parse_err;
		}
	}

	if (op < test_operation_max)
		dpl_test(op, port, queue);
	else
		pr_err("invalid params\n");

	return;

parse_err:
	pr_err("failed to parse %s\n", tok);
}

static int __dbg_test_set_open(struct inode *inode, struct file *file)
{
	return single_open(file, __dbg_test_set_rd,
			   inode->i_private);
}

static ssize_t __dbg_test_set_wr(struct file *file,
					 const char __user *buf,
					 size_t count, loff_t *pos)
{
	int ret;
	char cmd_buf[DBGFS_STR_MAX];

	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	if ((sizeof(cmd_buf) - 1) < count)
		return -E2BIG;
	ret = simple_write_to_buffer(cmd_buf, count, pos, buf, count);
	if (unlikely(ret != count))
		return ret >= 0 ? -EIO : ret;
	cmd_buf[count - 1] = '\0';
	__dpl_test_set(cmd_buf, file->f_inode->i_private);
	return count;
}

static const struct file_operations dbg_test_fops = {
	.open    = __dbg_test_set_open,
	.read    = seq_read,
	.write   = __dbg_test_set_wr,
	.llseek  = seq_lseek,
	.release = single_release,
};

static int __dbg_cpu_info_set_rd(struct seq_file *f, void *offset)
{
	unsigned int i, j;
	struct dpl_cpu_info cpu_info[CQM_MAX_CPU];
	unsigned int num_cpus;

	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	seq_puts(f, "\n");

	dpl_retrieve_cpu_info(cpu_info, &num_cpus);

	seq_printf(f, "num cpus %u\n", num_cpus);

	for (i = 0; i < num_cpus; i++) {
		seq_printf(f, "CPU %u:\n", i);
		for (j = 0; j < cpu_info[i].num_q; j++) {
			seq_printf(f, "\tq %u (port %u, gpid %u, dpl owner %u, is bw changed %d",
				   cpu_info[i].queue[j].queue,
				   cpu_info[i].queue[j].port,
				   cpu_info[i].queue[j].gpid,
				   cpu_info[i].queue[j].dpl_owner,
				   (int)cpu_info[i].queue[j].is_bw_changed);
			if (cpu_info[i].queue[j].is_bw_changed)
				seq_printf(f, ", dflt_bw %u",
					   cpu_info[i].queue[j].dflt_bw);
			seq_puts(f, ")\n");
		}
	}

	return 0;
}

static int __dbg_cpu_info_set_open(struct inode *inode, struct file *file)
{
	return single_open(file, __dbg_cpu_info_set_rd,
			   inode->i_private);
}

static const struct file_operations dbg_cpu_info_fops = {
	.open    = __dbg_cpu_info_set_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = single_release,
};

enum wl_rule_opts {
	wl_rule_opt_help = 1,
	wl_rule_opt_priority,
	wl_rule_opt_src_mac,
	wl_rule_opt_dst_mac,
	wl_rule_opt_eth_type,
	wl_rule_opt_src_ip,
	wl_rule_opt_dst_ip,
	wl_rule_opt_src_ipv6,
	wl_rule_opt_dst_ipv6,
	wl_rule_opt_ip_proto,
	wl_rule_opt_src_port,
	wl_rule_opt_dst_port,
	wl_rule_opt_icmp_type_code,
};

static const match_table_t wl_rule_add_tokens = {
	{wl_rule_opt_help, "help"},
	{wl_rule_opt_priority, "priority=%u"},
	{wl_rule_opt_src_mac, "src_mac=%s"},
	{wl_rule_opt_dst_mac, "dst_mac=%s"},
	{wl_rule_opt_eth_type, "eth_type=%u"},
	{wl_rule_opt_src_ip, "src_ip=%s"},
	{wl_rule_opt_dst_ip, "dst_ip=%s"},
	{wl_rule_opt_src_ipv6, "src_ipv6=%s"},
	{wl_rule_opt_dst_ipv6, "dst_ipv6=%s"},
	{wl_rule_opt_ip_proto, "ip_proto=%u"},
	{wl_rule_opt_src_port, "src_port=%u"},
	{wl_rule_opt_dst_port, "dst_port=%u"},
	{wl_rule_opt_icmp_type_code, "icmp=%u"},
	{ 0 },
};

static int rule_help(char *buf, size_t sz, size_t *n)
{
	*n = snprintf(buf + *n, sz - *n, "\n");
	*n +=
		snprintf(buf + *n, sz - *n, " DPL add/delete whitelist rule. Usage:\n");
	*n += snprintf(buf + *n, sz - *n,
				   "  echo op1=<value> ... opN=<value> > add_rule\n");
	*n += snprintf(buf + *n, sz - *n,
				   "  echo op1=<value> ... opN=<value> > del_rule\n");
	*n += snprintf(buf + *n, sz - *n, " Options:\n");
	*n += snprintf(buf + *n, sz - *n, "\t help\n");
	*n += snprintf(buf + *n, sz - *n, "\t priority=<value>\n");
	*n += snprintf(buf + *n, sz - *n, "\t src_mac=XX:XX:XX:XX:XX:XX\n");
	*n += snprintf(buf + *n, sz - *n, "\t dst_mac=XX:XX:XX:XX:XX:XX\n");
	*n += snprintf(buf + *n, sz - *n, "\t eth_type=<value>\n");
	*n += snprintf(buf + *n, sz - *n, "\t src_ip=X.X.X.X\n");
	*n += snprintf(buf + *n, sz - *n, "\t dst_ip=X.X.X.X\n");
	*n += snprintf(buf + *n, sz - *n, "\t src_ipv6=X:X:X:X:X:X:X:X\n");
	*n += snprintf(buf + *n, sz - *n, "\t dst_ipv6=X:X:X:X:X:X:X:X\n");
	*n += snprintf(buf + *n, sz - *n, "\t ip_proto=<value>\n");
	*n += snprintf(buf + *n, sz - *n, "\t src_port=<value>\n");
	*n += snprintf(buf + *n, sz - *n, "\t dst_port=<value>\n");
	*n += snprintf(buf + *n, sz - *n, " Notes:\n");
	*n += snprintf(buf + *n, sz - *n,
				   "\tIPv6 address short form is not supported\n");

	return 0;
}

static int dbg_whitelist_rule_help(void)
{
	char buf[2000];
	size_t n = 0;

	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	rule_help(buf, sizeof(buf), &n);
	pr_info("%s\n", buf);

	return 0;
}

static ssize_t dpl_whitelist_rule_wr(struct file *file,
				     const char __user *buf,
				     size_t count, loff_t *pos)
{
	struct dpl_whitelist_field *field, fields[DPL_FLD_COUNT] = { 0 };
	substring_t substr[32];
	enum wl_rule_opts opt;
	char *args, cmd_buf[256] = { 0 };
	char *tok, tmp[128];
	int ret, val, fields_cnt = 0;
	int priority = INT_MAX;
	u8 *mac, *ip;
	u16 *ipv6;
	bool add = (bool)file->f_inode->i_private;

	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	if ((sizeof(cmd_buf) - 1) < count)
		return -E2BIG;

	ret = simple_write_to_buffer(cmd_buf, count, pos, buf, count);
	if (ret != count)
		return -EIO;

	args = &cmd_buf[0];
	/* iterate over user arguments */
	while ((tok = strsep(&args, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, wl_rule_add_tokens, substr);
		field = &fields[fields_cnt++];
		switch (opt) {
		case wl_rule_opt_help:
			dbg_whitelist_rule_help();
			return count;
		case wl_rule_opt_priority:
			ret = match_int(&substr[0], &priority);
			if (ret)
				goto parse_err;
			fields_cnt--;
			break;
		case wl_rule_opt_src_mac:
		case wl_rule_opt_dst_mac:
			if (opt == wl_rule_opt_src_mac) {
				mac = field->src_mac;
				field->type = DPL_FLD_SRC_MAC;
			} else {
				mac = field->dst_mac;
				field->type = DPL_FLD_DST_MAC;
			}

			ret = match_strlcpy(tmp, &substr[0], sizeof(tmp));
			if (!ret)
				goto parse_err;
			ret = sscanf(tmp, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
				     &mac[0], &mac[1], &mac[2], &mac[3],
				     &mac[4], &mac[5]);
			if (ret != 6)
				goto parse_err;
			break;
		case wl_rule_opt_eth_type:
			ret = match_int(&substr[0], &val);
			if (ret)
				goto parse_err;
			field->eth_type = htons((u16)val);
			field->type = DPL_FLD_ETH_TYPE;
			break;
		case wl_rule_opt_src_ip:
		case wl_rule_opt_dst_ip:
			if (opt == wl_rule_opt_src_ip) {
				ip = (u8 *)&field->src_ip.v4;
				field->type = DPL_FLD_IPV4_SRC_IP;
			} else {
				ip = (u8 *)&field->dst_ip.v4;
				field->type = DPL_FLD_IPV4_DST_IP;
			}

			ret = match_strlcpy(tmp, &substr[0], sizeof(tmp));
			if (!ret)
				goto parse_err;
			ret = sscanf(tmp, "%hhu.%hhu.%hhu.%hhu",
				     &ip[0], &ip[1], &ip[2], &ip[3]);
			if (ret != 4)
				goto parse_err;
			break;
		case wl_rule_opt_src_ipv6:
		case wl_rule_opt_dst_ipv6:
			if (opt == wl_rule_opt_src_ipv6) {
				ipv6 = (u16 *)&field->src_ip.v6;
				field->type = DPL_FLD_IPV6_SRC_IP;
			} else {
				ipv6 = (u16 *)&field->dst_ip.v6;
				field->type = DPL_FLD_IPV6_DST_IP;
			}

			ret = match_strlcpy(tmp, &substr[0], sizeof(tmp));
			if (!ret)
				goto parse_err;
			ret = sscanf(tmp, "%hx:%hx:%hx:%hx:%hx:%hx:%hx:%hx",
				     &ipv6[0], &ipv6[1], &ipv6[2], &ipv6[3],
				     &ipv6[4], &ipv6[5], &ipv6[6], &ipv6[7]);
			if (ret != 8)
				goto parse_err;
			ipv6[0] = htons(ipv6[0]);
			ipv6[1] = htons(ipv6[1]);
			ipv6[2] = htons(ipv6[2]);
			ipv6[3] = htons(ipv6[3]);
			ipv6[4] = htons(ipv6[4]);
			ipv6[5] = htons(ipv6[5]);
			ipv6[6] = htons(ipv6[6]);
			ipv6[7] = htons(ipv6[7]);
			break;
		case wl_rule_opt_ip_proto:
			ret = match_int(&substr[0], &val);
			if (ret)
				goto parse_err;
			field->ip_proto = val;
			field->type = DPL_FLD_IP_PROTO;
			break;
		case wl_rule_opt_src_port:
			ret = match_int(&substr[0], &val);
			if (ret)
				goto parse_err;
			field->src_port = htons((u16)val);
			field->type = DPL_FLD_L4_SRC_PORT;
			break;
		case wl_rule_opt_dst_port:
			ret = match_int(&substr[0], &val);
			if (ret)
				goto parse_err;
			field->dst_port = htons((u16)val);
			field->type = DPL_FLD_L4_DST_PORT;
			break;
		case wl_rule_opt_icmp_type_code:
			ret = match_int(&substr[0], &val);
			if (ret)
				goto parse_err;
			field->icmp_type_code = htons((u16)val);
			field->type = DPL_FLD_ICMP_TYPE_CODE;
			break;
		default:
			goto parse_err;
		}
	}

	if (add)
		ret = dpl_whitelist_rule_add(priority, fields, fields_cnt);
	else
		ret = dpl_whitelist_rule_del(priority, fields, fields_cnt);
	if (ret)
		pr_info("Failed to %s rule, ret %d\n",
			(add ? "add" : "delete"), ret);

	return count;

parse_err:
	pr_err("failed to parse %s, ret %d\n", tok, ret);
	return count;
}

static int dbg_whitelist_rule_help_seq(struct seq_file *f, void *offset)
{
	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	rule_help(f->buf, f->size, &f->count);

	return 0;
}

static int dbg_whitelist_rule_open(struct inode *inode, struct file *file)
{
	return single_open(file, dbg_whitelist_rule_help_seq, inode->i_private);
}

static const struct file_operations dbg_wl_add_rule_fops = {
	.open    = dbg_whitelist_rule_open,
	.read    = seq_read,
	.write   = dpl_whitelist_rule_wr,
	.llseek  = seq_lseek,
	.release = single_release,
};

static const struct file_operations dbg_wl_del_rule_fops = {
	.open    = dbg_whitelist_rule_open,
	.read    = seq_read,
	.write   = dpl_whitelist_rule_wr,
	.llseek  = seq_lseek,
	.release = single_release,
};

static int dbg_whitelist_rules_show(struct seq_file *f, void *offset)
{
	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	seq_puts(f, " ==================================================\n");
	seq_puts(f, " |             DPL Whitelist Rules                |\n");
	pp_uc_whitelist_rules_show(f);

	return 0;
}

static int dbg_whitelist_rules_open(struct inode *inode, struct file *file)
{
	return single_open(file, dbg_whitelist_rules_show, inode->i_private);
}

static const struct file_operations dbg_wl_rules_fops = {
	.open    = dbg_whitelist_rules_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = single_release,
};

static int dbg_version_show(struct seq_file *f, void *offset)
{
	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	seq_printf(f, "dpl version = %s\n", DPL_VERSION);

	return 0;
}

static int dbg_ver_open(struct inode *inode, struct file *file)
{
	return single_open(file, dbg_version_show, inode->i_private);
}

static const struct file_operations dbg_ver_fops = {
	.open    = dbg_ver_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = single_release,
};

int dpl_lgm_dbg_clean(struct dentry *parent_dir)
{
	debugfs_remove_recursive(parent_dir);

	return 0;
}

int dpl_lgm_dbg_init(struct dentry **parent_dir)
{
	struct dentry *dir;
	struct dentry *dent;
	struct dentry *wl_dir;

	if (unlikely(!debugfs_initialized())) {
		pr_err("debugfs not initialized yet\n");
		return -EPERM;
	}

	*parent_dir = debugfs_create_dir("dpl", NULL);
	if (IS_ERR_OR_NULL(*parent_dir)) {
		pr_err("debugfs_create_dir dpl failed\n");
		return PTR_ERR(parent_dir);
	}

	dir = debugfs_create_dir("lgm", *parent_dir);
	if (IS_ERR_OR_NULL(dir)) {
		pr_err("debugfs_create_dir lgm failed\n");
		dent = dir;
		goto fail;
	}

	dent = debugfs_create_file_unsafe("dev_protect", 0600, dir, NULL,
					  &dbg_dev_protect_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("failed to create dev_protect debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file_unsafe("set_action", 0600, dir, NULL,
					  &dbg_set_action_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("failed to create set_action debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file_unsafe("test", 0600, dir, NULL,
					  &dbg_test_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("failed to create test debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file_unsafe("cpu_info", 0600, dir, NULL,
					  &dbg_cpu_info_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("failed to create cpu info debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file_unsafe("version", 0600, dir, NULL,
					  &dbg_ver_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("failed to create version debugfs file\n");
		goto fail;
	}

	/* White list entries */
	wl_dir = debugfs_create_dir("whitelist", dir);
	if (IS_ERR_OR_NULL(dir)) {
		pr_err("debugfs_create_dir lgm failed\n");
		dent = dir;
		goto fail;
	}

	dent = debugfs_create_file_unsafe("add_rule", 0600, wl_dir, (void *)true,
					  &dbg_wl_add_rule_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("failed to create add_rule debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file_unsafe("del_rule", 0600, wl_dir, (void *)false,
					  &dbg_wl_del_rule_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("failed to create del_rule debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file_unsafe("rules", 0600, wl_dir, NULL,
					  &dbg_wl_rules_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("failed to create rules debugfs file\n");
		goto fail;
	}

	return 0;

fail:
	pr_err("debugfs_create_file failed\n");
	dpl_lgm_dbg_clean(*parent_dir);
	return PTR_ERR(dent);
}
