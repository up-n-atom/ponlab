/*
 * Copyright (C) 2020-2023 MaxLinear, Inc.
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
 * Description: dpl monitor debugfs
 */

#define pr_fmt(fmt) "dpl_mon_dbg: %s:%d: " fmt, __func__, __LINE__

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/debugfs.h>
#include <linux/parser.h>

#include "dpl_monitor.h"

#define DBGFS_STR_MAX   (1024)

static struct dentry *dir;
static struct dpl_monitor_param param;
static unsigned long long attr_val[DPL_ATTR_MAX] = {0};

static void attr0_get_cb(unsigned long long *val)
{
	pr_debug("attr0 read\n");
	*val = attr_val[0];
}

static void attr1_get_cb(unsigned long long *val)
{
	pr_debug("attr1 read\n");
	*val = attr_val[1];
}

static void attr2_get_cb(unsigned long long *val)
{
	pr_debug("attr2 read\n");
	*val = attr_val[2];
}

static void attr3_get_cb(unsigned long long *val)
{
	pr_debug("attr3 read\n");
	*val = attr_val[3];
}

static void act0_enable(void)
{
	pr_info("act0 enable\n");
}
static void act0_disable(void)
{
	pr_info("act0 disable\n");
}

static void act1_enable(void)
{
	pr_info("act1 enable\n");
}
static void act1_disable(void)
{
	pr_info("act1 disable\n");
}

static void act2_enable(void)
{
	pr_info("act2 enable\n");
}
static void act2_disable(void)
{
	pr_info("act2 disable\n");
}

static void act3_enable(void)
{
	pr_info("act3 enable\n");
}
static void act3_disable(void)
{
	pr_info("act3 disable\n");
}

enum thresholds_opts {
	thresholds_opt_reset = 1,
	thresholds_opt_action,
	thresholds_opt_type,
	thresholds_opt_attr,
	thresholds_opt_val,
	thresholds_opt_seq_iter,
	thresholds_opt_iter,
	thresholds_opt_dep
};

static const match_table_t thresholds_tokens = {
	{thresholds_opt_reset,      "reset"},
	{thresholds_opt_action,     "action=%u"},
	{thresholds_opt_type,       "type=%u"},
	{thresholds_opt_attr,       "attr=%u"},
	{thresholds_opt_val,        "val=%u"},
	{thresholds_opt_seq_iter,   "seq_iter=%u"},
	{thresholds_opt_iter,       "iter=%u"},
	{thresholds_opt_dep,        "dep=%u"}
};

static int __dbg_thresholds_set_rd(struct seq_file *f, void *offset)
{
	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	pr_info("\n");
	pr_info(" Configure action thresholds. Example:\n");
	pr_info("  echo action=0 type=1 attr=0 val=800 iter=5 dep=1 > thresholds\n");
	pr_info("  action - 0 - %u\n", DPL_ACTION_MAX);
	pr_info("  type   - On = %u; OFF = %u\n", THR_ON, THR_OFF);
	pr_info("  attr   - 0 - %u\n", DPL_ATTR_MAX);
	pr_info("  val    - threshold value\n");
	pr_info("  iter   - number of continues iterations\n");
	pr_info("  iter   - number of iterations\n");
	pr_info("  dep    - dependency attribute\n");
	pr_info("  To RESET param: echo reset > thresholds\n");
	pr_info("\n");

	dpl_mon_param_dump(&param);

	return 0;
}

static bool is_action_enabled(struct dpl_monitor_param *dpl_param,
			      unsigned int action)
{
	unsigned int ind;
	unsigned int ind2;

	for (ind = 0; ind < DPL_ATTR_MAX; ind++)
		for (ind2 = 0; ind2 < DPL_THR_TYPE_MAX; ind2++)
			if (dpl_param->act[action].thr[ind2][ind].val)
				return true;

	return false;
}

static void set_action_cbs(struct dpl_monitor_param *dpl_param,
		    unsigned int action,
		    void (*enable)(void),
		    void (*disable)(void))
{
	if (is_action_enabled(dpl_param, action)) {
		dpl_param->act[action].enable = enable;
		dpl_param->act[action].disable = disable;
	} else {
		dpl_param->act[action].enable = NULL;
		dpl_param->act[action].disable = NULL;
	}
}

/**
 * @brief configure thresholds
 */
static void __dpl_thresholds_set(char *args, void *data)
{
	substring_t substr[32];
	enum thresholds_opts opt;
	char *tok;
	int ret, val;
	/* Default values */
	unsigned int action = 0;
	unsigned int type = 0;
	unsigned int attr = 0;

	/* iterate over user arguments */
	while ((tok = strsep(&args, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, thresholds_tokens, substr);
		switch (opt) {
		case thresholds_opt_reset:
			memset(&param, 0, sizeof(param));
			return;
		case thresholds_opt_action:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			action = (unsigned int)val;
			break;
		case thresholds_opt_type:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			type = (unsigned int)val;
			break;
		case thresholds_opt_attr:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			attr = (unsigned int)val;
			break;
		case thresholds_opt_val:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			param.act[action].thr[type][attr].val =
				(unsigned int)val;
			break;
		case thresholds_opt_seq_iter:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			param.act[action].thr[type][attr].num_seq_iter =
				(unsigned int)val;
			break;
		case thresholds_opt_iter:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			param.act[action].thr[type][attr].num_iter =
				(unsigned int)val;
			break;
		case thresholds_opt_dep:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			param.act[action].thr[type][attr].dependency =
				(unsigned int)val;
			break;
		default:
			goto parse_err;
		}
	}

	dpl_mon_unregister();

	param.attr_get[0].get_cb = attr0_get_cb;
	param.attr_get[1].get_cb = attr1_get_cb;
	param.attr_get[2].get_cb = attr2_get_cb;
	param.attr_get[3].get_cb = attr3_get_cb;

	set_action_cbs(&param, 0, act0_enable, act0_disable);
	set_action_cbs(&param, 1, act1_enable, act1_disable);
	set_action_cbs(&param, 2, act2_enable, act2_disable);
	set_action_cbs(&param, 3, act3_enable, act3_disable);

	if (!dpl_mon_register(&param))
		pr_info("register done\n");

	dpl_mon_param_dump(&param);

	return;

parse_err:
	pr_err("failed to parse %s\n", tok);
}

static int __dbg_thresholds_set_open(struct inode *inode, struct file *file)
{
	return single_open(file, __dbg_thresholds_set_rd, inode->i_private);
}

static ssize_t __dbg_thresholds_set_wr(struct file *file,
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
	__dpl_thresholds_set(cmd_buf, file->f_inode->i_private);
	return count;
}
static const struct file_operations dbg_thresholds_fops = {
	.open    = __dbg_thresholds_set_open,
	.read    = seq_read,
	.write   = __dbg_thresholds_set_wr,
	.llseek  = seq_lseek,
	.release = single_release,
};

enum attr_opts {
	attr_opt_help = 1,
	attr_opt_attr,
	attr_opt_val
};

static const match_table_t attr_tokens = {
	{attr_opt_help,     "help"},
	{attr_opt_attr,     "attr=%u"},
	{attr_opt_val,      "val=%u"}
};

static int __dbg_attr_set_rd(struct seq_file *f, void *offset)
{
	unsigned int ind = 0;

	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	pr_info("\n");

	/* print all attributes */
	for (ind = 0; ind < DPL_ATTR_MAX; ind++)
		pr_info("attr %u, val %llu\n", ind, attr_val[ind]);

	pr_info(" For setting attr 0, for example, use:\n");
	pr_info("  echo attr=0 val=1000 > attr\n");
	pr_info("  attr   - 0 - %u\n", DPL_ATTR_MAX);
	pr_info("  val    - attrbiute value\n");
	pr_info("\n");

	return 0;
}

/**
 * @brief configure attributes
 */
static void __dpl_attr_set(char *args, void *data)
{
	substring_t substr[32];
	enum attr_opts opt;
	char *tok;
	int ret, val;
	unsigned int value = UINT_MAX;
	unsigned int attr = DPL_ATTR_MAX;

	/* iterate over user arguments */
	while ((tok = strsep(&args, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, attr_tokens, substr);
		switch (opt) {
		case attr_opt_help:
			__dbg_attr_set_rd(NULL, NULL);
			break;
		case attr_opt_attr:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret) || val >= DPL_ATTR_MAX)
				goto parse_err;
			attr = (unsigned int)val;
			break;
		case attr_opt_val:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			value = (unsigned int)val;
			break;
		default:
			goto parse_err;
		}
	}

	/* set attr */
	if (value != UINT_MAX) {
		attr_val[attr] = value;
		pr_info("Set attr %u -> val %llu\n", attr, attr_val[attr]);
	}

	return;

parse_err:
	pr_err("failed to parse %s\n", tok);
}

static int __dbg_attr_set_open(struct inode *inode, struct file *file)
{
	return single_open(file, __dbg_attr_set_rd, inode->i_private);
}

static ssize_t __dbg_attr_set_wr(struct file *file,
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
	__dpl_attr_set(cmd_buf, file->f_inode->i_private);
	return count;
}
static const struct file_operations dbg_attr_fops = {
	.open    = __dbg_attr_set_open,
	.read    = seq_read,
	.write   = __dbg_attr_set_wr,
	.llseek  = seq_lseek,
	.release = single_release,
};

static int __dbg_status_set_rd(struct seq_file *f, void *offset)
{
	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	seq_printf(f, "monitor is %s!\n", dpl_mon_is_enable() ? "ON" : "OFF");
	seq_puts(f, "to enable/disable: echo 1/0 > status\n");
	return 0;
}


static int __dbg_status_set_open(struct inode *inode, struct file *file)
{
	return single_open(file, __dbg_status_set_rd, inode->i_private);
}

static void __dpl_status_set(char *args, void *data)
{
	bool val;

	if (kstrtobool(args, &val)) {
		pr_err("invalid args! to enable/disable: echo 1/0 > status\n");
		return;
	}
	dpl_mon_set_enable(val);
}

static ssize_t __dbg_status_set_wr(struct file *file,
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
	__dpl_status_set(cmd_buf, file->f_inode->i_private);
	return count;
}

static const struct file_operations dbg_status_fops = {
	.open    = __dbg_status_set_open,
	.read    = seq_read,
	.write   = __dbg_status_set_wr,
	.llseek  = seq_lseek,
	.release = single_release,
};

int mon_dbg_clean(void)
{
	debugfs_remove_recursive(dir);
	dir = NULL;

	return 0;
}

int mon_dbg_init(struct dentry *parent_dir)
{
	struct dentry *dent;

	if (unlikely(!debugfs_initialized())) {
		pr_err("debugfs not initialized yet\n");
		return -EPERM;
	}

	dir = debugfs_create_dir("mon", parent_dir);
	if (IS_ERR_OR_NULL(dir)) {
		pr_err("debugfs_create_dir failed\n");
		return -ENOENT;
	}

	dent = debugfs_create_file_unsafe("attr", 0600, dir, NULL,
					  &dbg_attr_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("failed to create attr debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file_unsafe("thresholds", 0600, dir, NULL,
					  &dbg_thresholds_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("failed to create thresholds debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file_unsafe("status", 0600, dir, NULL,
					  &dbg_status_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("failed to create status debugfs file\n");
		goto fail;
	}

	return 0;

fail:
	pr_err("debugfs_create_file failed\n");
	mon_dbg_clean();
	return -ENOENT;
}
