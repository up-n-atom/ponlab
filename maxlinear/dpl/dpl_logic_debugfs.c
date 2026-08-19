/*
 * Copyright (C) 2023-2025 MaxLinear, Inc.
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
 * Description: dpl logic debugfs
 */

#define pr_fmt(fmt) "dpl_logic_dbg: %s:%d: " fmt, __func__, __LINE__

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/debugfs.h>
#include <linux/parser.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/bitmap.h>

#include "dpl_logic.h"
#include "dpl_logic_internal.h"

#define DBGFS_STR_MAX   (1024)

static struct dentry *dir;

enum hash_bit_opts {
	hash_bit_opt_help = 1,
	hash_bit_opt_operation = 2,
	hash_bit_opt_reset = 3,
	hash_bit_opt_index
};

static const match_table_t hash_bit_tokens = {
	{hash_bit_opt_help,     "help"},
	{hash_bit_opt_operation,"op=%u"},
	{hash_bit_opt_reset,	"reset"},
	{hash_bit_opt_index,  	"id=%u"},
	{ 0 },
};

enum hash_bit_operation {
	hash_bit_operation_disable,
	hash_bit_operation_enable,
	hash_bit_operation_max
};

/**
 * @brief hash_bit configure manually for debug purpose
 */
static int __dbg_hash_bit_set_rd(struct seq_file *f, void *offset)
{
	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	pr_info("\n");

	pr_info(" Enable/Disable hash bit entry. Usage:\n");
	pr_info("  echo op=0 id=0 > cfg\n");
	pr_info("  op   - 0 - Disable / 1- Enable\n");
	pr_info("  id - 0 to MAX table size\n");
	pr_info("\n");

	return 0;
}

static void __dpl_hash_bit_add_wr(char *args, void *data)
{
	substring_t substr[32];
	enum hash_bit_opts opt;
	char *tok;
	int ret, val;
	unsigned int index = UINT_MAX;
	bool reset = false;
	enum hash_bit_operation op = hash_bit_operation_max;

	/* iterate over user arguments */
	while ((tok = strsep(&args, " \t\n")) != NULL) {
		if (!*tok)
			continue;
		opt = match_token(tok, hash_bit_tokens, substr);
		switch (opt) {
		case hash_bit_opt_help:
			__dbg_hash_bit_set_rd(NULL, NULL);
			return;
		case hash_bit_opt_operation:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret) || val >= hash_bit_operation_max)
				goto parse_err;
			op = (unsigned int)val;
			break;
		case hash_bit_opt_index:
			ret = match_int(&substr[0], &val);
			if (unlikely(ret))
				goto parse_err;
			index = (unsigned int)val;
			break;
		case hash_bit_opt_reset:
			reset = true;
			break;
		default:
			goto parse_err;
		}
	}

	if (op < hash_bit_operation_max && index != UINT_MAX) {
		if (op == hash_bit_operation_enable)
			dpl_logic_hash_bit_ind_enable(index);
		else
			dpl_logic_hash_bit_ind_disable(index);
	} else if ((reset == true) && (hash_bit_opt_reset == opt)) {
		dpl_logic_hash_bit_reset();
	} else {
		pr_err("invalid params\n");
	}

	return;

parse_err:
	pr_err("failed to parse %s\n", tok);
}

static ssize_t __dbg_hash_bit_set_wr(struct file *file,
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
	__dpl_hash_bit_add_wr(cmd_buf, file->f_inode->i_private);
	return count;
}

static int __dbg_hash_bit_set_open(struct inode *inode, struct file *file)
{
	return single_open(file, __dbg_hash_bit_set_rd,
			   inode->i_private);
}

static const struct file_operations dbg_hash_bit_fops = {
	.open    = __dbg_hash_bit_set_open,
	.read    = seq_read,
	.write   = __dbg_hash_bit_set_wr,
	.llseek  = seq_lseek,
	.release = single_release,
};

static int dbg_hash_bit_entries_show(struct seq_file *f, void *offset)
{
	unsigned int i, ret;	
	unsigned long *hash_bit;
	unsigned long long *timestamp, curr_time = get_jiffies_64();
	unsigned int num_entries;

	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	num_entries = dpl_logic_num_entries_get();

	hash_bit = bitmap_zalloc(num_entries, GFP_KERNEL);
	if (!hash_bit)
		return -ENOMEM;

	timestamp = kcalloc(num_entries, sizeof(*timestamp), GFP_KERNEL);
	if (!timestamp) {
		bitmap_free(hash_bit);
		return -ENOMEM;
	}

	ret = dpl_logic_hash_bit_get(hash_bit, timestamp, num_entries);
	if (ret)
		return ret;

	seq_printf(f, "dpl_logic num bit hash entries = %u\n", num_entries);
	seq_printf(f, "dpl_logic max used entries = %u\n",
		   DPL_LOGIC_MAX_USED_HASH_ENTRIES(num_entries));
	seq_puts(f, " ==================================================\n");
	seq_puts(f, " |          DPL Logic Hash Bit Enabled indexs      |\n");
	seq_puts(f, " | index | time (ms) |\n");
	seq_puts(f, " +-------+-----------+\n");
	for_each_set_bit(i, hash_bit, num_entries)
		seq_printf(f, " | %5d | %9u |\n", i,
				    jiffies_to_msecs(curr_time - timestamp[i]));

	kfree(timestamp);
	bitmap_free(hash_bit);

	return 0;
}

static int dbg_hash_bit_entries_open(struct inode *inode, struct file *file)
{
	return single_open(file, dbg_hash_bit_entries_show, inode->i_private);
}

static const struct file_operations dbg_hash_bit_list_fops = {
	.open    = dbg_hash_bit_entries_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = single_release,
};

static int __dbg_status_set_rd(struct seq_file *f, void *offset)
{
	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	seq_printf(f, "dpl logic is %s!\n", dpl_logic_is_enable() ? "ON" : "OFF");
	seq_puts(f, "to enable/disable dpl logic: echo 1/0 > status\n");
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
		pr_err("invalid args! to enable/disable dpl logic: echo 1/0 > status\n");
		return;
	}

	if (val)
		dpl_logic_hash_bit_activate();
	else
		dpl_logic_hash_bit_deactivate();
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

static int dbg_old_entry_timeout_show(struct seq_file *f, void *offset)
{
	unsigned int tout;
	dpl_logic_old_entry_to_get(&tout);
	seq_printf(f, "old entry timeout: %u\n", tout);
	return 0;
}

static int dbg_old_entry_timeout_open(struct inode *inode, struct file *file)
{
	return single_open(file, dbg_old_entry_timeout_show, inode->i_private);
}

static ssize_t dbg_old_entry_timeout_write(struct file *file,
					   const char __user *buf,
					   size_t count, loff_t *ppos)
{
	char cmd_buf[DBGFS_STR_MAX];
	unsigned int val;

	if (count >= sizeof(cmd_buf))
		return -E2BIG;

	if (copy_from_user(cmd_buf, buf, count))
		return -EFAULT;

	cmd_buf[count] = '\0';

	if (kstrtouint(cmd_buf, 10, &val))
		return -EINVAL;

	dpl_logic_old_entry_to_set(val);
	return count;
}

static const struct file_operations dbg_old_entry_timeout_fops = {
    .open = dbg_old_entry_timeout_open,
    .read = seq_read,
    .write = dbg_old_entry_timeout_write,
    .llseek = seq_lseek,
    .release = single_release,
};

static int dbg_cleaner_interval_show(struct seq_file *f, void *offset)
{
	unsigned int tout;
	dpl_logic_cleaner_interval_get(&tout);
	seq_printf(f, "cleaner interval: %u\n", tout);
	return 0;
}

static int dbg_cleaner_interval_open(struct inode *inode, struct file *file)
{
	return single_open(file, dbg_cleaner_interval_show, inode->i_private);
}

static ssize_t dbg_cleaner_interval_write(struct file *file,
					const char __user *buf,
					size_t count, loff_t *ppos)
{
	char cmd_buf[DBGFS_STR_MAX];
	unsigned int val;

	if (count >= sizeof(cmd_buf))
		return -E2BIG;

	if (copy_from_user(cmd_buf, buf, count))
		return -EFAULT;

	cmd_buf[count] = '\0';

	if (kstrtouint(cmd_buf, 10, &val))
		return -EINVAL;

	dpl_logic_cleaner_interval_set(val);
	return count;
}

static const struct file_operations dbg_cleaner_interval_fops = {
    .open = dbg_cleaner_interval_open,
    .read = seq_read,
    .write = dbg_cleaner_interval_write,
    .llseek = seq_lseek,
    .release = single_release,
};

int dpl_logic_dbg_clean(void)
{
	debugfs_remove_recursive(dir);
	dir = NULL;

	return 0;
}

int dpl_logic_dbg_init(struct dentry *parent_dir)
{
	struct dentry *dent;

	if (!debugfs_initialized()) {
		pr_err("debugfs not initialized yet\n");
		return -EPERM;
	}

	dir = debugfs_create_dir("logic", parent_dir);
	if (IS_ERR_OR_NULL(dir)) {
		pr_err("debugfs_create_dir failed\n");
		return -ENOENT;
	}

	dent = debugfs_create_file_unsafe("cfg", 0600, dir, NULL,
					  &dbg_hash_bit_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("failed to create cfg debugfs file\n");
		goto fail;
	}
	dent = debugfs_create_file_unsafe("entries", 0600, dir, NULL,
					  &dbg_hash_bit_list_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("failed to create entries debugfs file\n");
		goto fail;
	}
	dent = debugfs_create_file_unsafe("status", 0600, dir, NULL,
					  &dbg_status_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("failed to create status debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file_unsafe("old_entry_timeout", 0600, dir, NULL,
					  &dbg_old_entry_timeout_fops);
	if (IS_ERR_OR_NULL(dent))
	{
		pr_err("failed to create old_entry_timeout debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file_unsafe("cleaner_interval", 0600, dir, NULL,
					  &dbg_cleaner_interval_fops);
	if (IS_ERR_OR_NULL(dent))
	{
		pr_err("failed to create cleaner_interval debugfs file\n");
		goto fail;
	}

	return 0;

fail:
	pr_err("debugfs_create_file failed\n");
	dpl_logic_dbg_clean();
	return -ENOENT;
}
