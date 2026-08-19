// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2020 - 2024 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/fs.h>
#include <uapi/linux/stat.h> /* S_IRUSR */

#include "qos_tc_ext_vlan.h"
#include "qos_tc_flower.h"
#include "qos_tc_pce.h"
#include "qos_tc_police.h"
#include "qos_tc_qmap.h"
#include "qos_tc_qos.h"
#include "qos_tc_trap.h"
#include "qos_tc_vlan_filter.h"
#include "qos_tc_skbedit.h"
#include "qos_tc_extract_cfm.h"

struct qos_tc_debugfs_entry {
	const char *filename;
	void (*read)(struct seq_file *file, void *ctx);
};

static int qos_tc_read(struct seq_file *file, void *unused)
{
	const struct qos_tc_debugfs_entry *entry = file->private;

	entry->read(file, NULL);
	return 0;
}

static int qos_tc_open(struct inode *inode, struct file *file)
{
	return single_open(file, qos_tc_read, inode->i_private);
}

static const struct file_operations qos_tc_fops = {
	.llseek = seq_lseek,
	.open = qos_tc_open,
	.owner = THIS_MODULE,
	.read = seq_read,
	.release = single_release,
};

static struct qos_tc_debugfs_entry entries[] = {
	{ .filename = "tc_ext_vlan_storage",
	  .read = qos_tc_ext_vlan_debugfs },
	{ .filename = "qos_tc_storage",
	  .read = qos_tc_storage_debugfs },
	{ .filename = "tc_pce_list",
	  .read = qos_tc_pce_list_debugfs },
	{ .filename = "policer_list",
	  .read = qos_tc_policer_list_debugfs },
	{ .filename = "tc_class_list",
	  .read = qos_tc_class_list_debugfs },
	{ .filename = "port_list",
	  .read = qos_tc_port_list_debugfs },
	{ .filename = "tc_trap_storage",
	 .read = qos_tc_trap_storage_debugfs },
	{ .filename = "tc_vlan_filter_storage",
	 .read = qos_tc_vlan_filter_storage_debugfs },
	{ .filename = "tc_skbedit_storage",
	 .read = qos_tc_skbedit_debugfs },
	{ .filename = "tc_cfm",
	  .read = qos_tc_cfm_debugfs },
};

#define DEBUGFS_DIR "qos_tc"
static struct dentry *dir;
static struct dentry *debugfs_files[ARRAY_SIZE(entries)];

int qos_tc_debugfs_init(void)
{
	unsigned int i = 0;

	dir = debugfs_create_dir(DEBUGFS_DIR, 0);
	if (!dir)
		return -ENODEV;

	for (i = 0; i < ARRAY_SIZE(entries); ++i) {
		struct qos_tc_debugfs_entry *entry = &entries[i];
		struct dentry **file = &debugfs_files[i];

		*file = debugfs_create_file(entry->filename, S_IRUSR, dir,
					    entry, &qos_tc_fops);
		if (!*file) {
			pr_err("Cannot create debugfs file %s\n", entry->filename);
			goto cleanup;
		}
	}

	return 0;

cleanup:
	debugfs_remove_recursive(dir);
	return -ENOENT;
}

void qos_tc_debugfs_exit(void)
{
	debugfs_remove_recursive(dir);
}
