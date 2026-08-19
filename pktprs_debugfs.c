/*
 * Copyright (C) 2020-2025 MaxLinear, Inc.
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
 * Description: Packet header parsing debugfs
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/debugfs.h>
#include <linux/netdevice.h>
#include <linux/in.h>
#include <linux/pktprs.h>

#include "pktprs_internal.h"

static struct dentry *dir;

static bool dump;

static int dbg_stats_show(struct seq_file *s, void *unused)
{
	struct parse_stats stats;
	int i;

	if (!capable(CAP_SYS_PACCT))
		return 0;

	pktprs_stats_get(&stats);
	seq_printf(s, "error packets        : %u\n",
		   atomic_read(&stats.error_pkt));
	seq_printf(s, "level overflow       : %u\n",
		   atomic_read(&stats.level_ovflw));
	seq_printf(s, "header overflow      : %u\n",
		   atomic_read(&stats.hdr_buf_ovflw));
	seq_printf(s, "vlan overflow        : %u\n",
		   atomic_read(&stats.vlan_ovflw));
	seq_printf(s, "mpls overflow        : %u\n",
		   atomic_read(&stats.mpls_ovflw));
	seq_printf(s, "unsupported protocol : %u\n",
		   atomic_read(&stats.unsupp_proto));
	seq_printf(s, "free skbext (rxhdr)  : %u\n",
		   atomic_read(&stats.free_skbext));
	seq_printf(s, "num devices          : %u\n",
		   atomic_read(&stats.num_devices));
	seq_printf(s, "hdr allocation failed: %u\n",
		   atomic_read(&stats.hdr_alloc_failed));
	seq_printf(s, "unsupported GRE      : %u\n",
		   atomic_read(&stats.unsupp_gre));
	seq_printf(s, "custom hook          : %u\n",
		   atomic_read(&stats.custom_hook));
	seq_printf(s, "direct hook          : %u\n",
		   atomic_read(&stats.direct_hook));
	seq_printf(s, "local hook           : %u\n",
		   atomic_read(&stats.local_hook));
	seq_printf(s, "RX hook              : %u\n",
		   atomic_read(&stats.rx_hook));
	seq_printf(s, "TX hook              : %u\n",
		   atomic_read(&stats.tx_hook));

	for (i = 0; i <= PKTPRS_PROTO_LAST; i++) {
		seq_printf(s, "proto %15s: %u\n",
			   PKTPRS_PROTO_STR(i), atomic_read(&stats.proto[i]));
	}

	return 0;
}

static int dbg_stats_open(struct inode *inode, struct file *file)
{
	return single_open(file, dbg_stats_show, NULL);
}

static const struct file_operations dbg_stats_fops = {
	.open = dbg_stats_open,
	.read = seq_read,
	.release = single_release,
};

static int parse_done_cb(struct notifier_block *unused, unsigned long event,
			 void *cb_desc)
{
	struct pktprs_desc *d = (struct pktprs_desc *)cb_desc;

	if (dump) {
		pktprs_pr_hdr(d->rx, PKTPRS_ETH_RX);
		pktprs_pr_hdr(d->tx, PKTPRS_ETH_TX);
	}

	return 0;
}

static struct notifier_block parse_done_notifier = {
	.notifier_call = parse_done_cb,
};

static int dbg_register(void *data, u64 val)
{
	if (!capable(CAP_NET_ADMIN))
		return 0;

	if (val) {
		if (!pktprs_register(&parse_done_notifier, PKTPRS_RXTX))
			pr_info("register done\n");
	} else {
		pktprs_unregister(&parse_done_notifier, PKTPRS_RXTX);
		pr_info("unregister done\n");
	}

	return 0;
}
DEFINE_DEBUGFS_ATTRIBUTE(dbg_register_fops, NULL, dbg_register, "%llu\n");

static int dbg_dump_set(void *data, u64 val)
{
	if (!capable(CAP_NET_ADMIN))
		return 0;

	dump = !!val;
	pr_info("%s callback prints\n", val? "enable" : "disable");

	return 0;
}
DEFINE_DEBUGFS_ATTRIBUTE(dbg_dump_fops, NULL, dbg_dump_set, "%llu\n");

static ssize_t device_set(struct file *file, const char __user *buf,
			  size_t count, loff_t *pos)
{
	struct net_device *netdev;
	char *netdev_name = NULL;
	long word_cnt = 0;
	char *cmd[4] = {NULL, NULL, NULL, NULL};
	char *pch;
	char lbuf[32];
	char *lbuf_ptr = lbuf;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	if (count >= sizeof(lbuf))
		return -EINVAL;

	if (copy_from_user(lbuf, buf, count))
		return -EFAULT;

	lbuf[count] = '\0';

	pch = strsep(&lbuf_ptr, " ");
	for (word_cnt = 0 ; (word_cnt < 4) && (pch != NULL) ; word_cnt++) {
		cmd[word_cnt] = pch;
		pch = strsep(&lbuf_ptr, " ");
	}

	if (word_cnt > 2) {
		pr_err("Too many arguments\n");
		return -EINVAL;
	}

	if (cmd[1]) {
		netdev_name = cmd[1];
		if (netdev_name[strlen(netdev_name)-1] == '\n')
			netdev_name[strlen(netdev_name)-1] = '\0';
	}

	if (strncasecmp(cmd[0], "ENABLE", sizeof("ENABLE")-1) == 0) {
		if (!netdev_name) {
			pr_err("please provide netdevice name\n");
			return -EINVAL;
		}
		netdev = dev_get_by_name(&init_net, netdev_name);
		if (netdev) {
			pktprs_dev_add(netdev);
			pr_info("device %s added\n", netdev->name);
			dev_put(netdev);
		} else {
			pr_err("failed to get dev (%s)\n", netdev_name);
		}
	} else if (strncasecmp(cmd[0], "DISABLE", sizeof("DISABLE")-1) == 0) {
		if (!netdev_name) {
			pr_err("please provide netdevice name\n");
			return -EINVAL;
		}
		netdev = dev_get_by_name(&init_net, netdev_name);
		if (netdev) {
			pktprs_dev_remove(netdev);
			pr_info("device %s removed\n", netdev->name);
			dev_put(netdev);
		} else {
			pr_err("failed to get dev (%s)\n", netdev_name);
		}
	} else {
		pr_info("invalid action %s\n", cmd[0]);
	}

	pktprs_pr_dev();

	return count;
}

static const struct file_operations dbg_device_fops = {
	.open    = simple_open,
	.write   = device_set,
	.llseek  = default_llseek,
};

void dbg_clean(void)
{
	debugfs_remove_recursive(dir);
	dir = NULL;
}

void dbg_init(void)
{
	struct dentry *dent;

	if (unlikely(!debugfs_initialized())) {
		pr_err("debugfs not initialized yet\n");
		return;
	}

	dir = debugfs_create_dir("pktprs", NULL);
	if (IS_ERR_OR_NULL(dir)) {
		pr_err("debugfs_create_dir failed\n");
		return;
	}

	dent = debugfs_create_file("stats", 0400, dir, NULL, &dbg_stats_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("failed to create stats debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file("device", 0600, dir, NULL, &dbg_device_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("failed to create device debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file_unsafe("dump", 0600, dir, NULL,
					  &dbg_dump_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("failed to create dump debugfs file\n");
		goto fail;
	}

	dent = debugfs_create_file_unsafe("register", 0600, dir, NULL,
					  &dbg_register_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("failed to create register debugfs file\n");
		goto fail;
	}

	return;

fail:
	pr_err("debugfs_create_file failed\n");
	dbg_clean();
}
