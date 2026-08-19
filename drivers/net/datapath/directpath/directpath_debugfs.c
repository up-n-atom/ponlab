// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2023 MaxLinear, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/debugfs.h>
#include <net/directpath_api.h>
#include "directpath.h"

#define DPDP_SUBIF_MASK 0xffff

static struct dentry *dir;

static void dump_cb(struct dpdp_device *dp_dev, int bkt, void *data)
{
	int cpu;
	struct seq_file *s = (struct seq_file *)data;
	struct dpdp_stats *stats;
	struct dpdp_stats tmp = {0};

	for_each_possible_cpu(cpu) {
		stats = per_cpu_ptr(dp_dev->stats, cpu);
		tmp.rx_fn_rxif_pkts += stats->rx_fn_rxif_pkts;
		tmp.rx_fn_rxif_drop += stats->rx_fn_rxif_drop;
		tmp.rx_fn_txif_pkts += stats->rx_fn_txif_pkts;
		tmp.rx_fn_txif_drop += stats->rx_fn_txif_drop;
		tmp.tx_fwd_offload_pkts += stats->tx_fwd_offload_pkts;
		tmp.tx_fwd_offload_errs += stats->tx_fwd_offload_errs;
		tmp.tx_qos_offload_pkts += stats->tx_qos_offload_pkts;
		tmp.tx_qos_offload_errs += stats->tx_qos_offload_errs;
	}

	seq_printf(s, "| %-15s |   %2d/%04x  | fwd_offload_pkts=%-13lld | rxif_pkts=%-20lld |\n",
		   netdev_name(dp_dev->dev), dp_dev->pinfo->dp_port,
		   (dp_dev->dp_subif.subif & DPDP_SUBIF_MASK),
		   tmp.tx_fwd_offload_pkts, tmp.rx_fn_rxif_pkts);
	seq_printf(s, "| %-15s | %-10s | fwd_offload_errs=%-13lld | rxif_drop=%-20lld |\n",
		   "", "", tmp.tx_fwd_offload_errs, tmp.rx_fn_rxif_drop);
	seq_printf(s, "| %-15s | %-10s | qos_offload_pkts=%-13lld | txif_pkts=%-20lld |\n",
		   "", "", tmp.tx_qos_offload_pkts, tmp.rx_fn_txif_pkts);
	seq_printf(s, "| (%3d->%-3d)%-5s | %-10s | qos_offload_errs=%-13lld | txif_drop=%-20lld |\n",
		   dp_dev->dp_subif.gpid, bkt, "", "",
		   tmp.tx_qos_offload_errs, tmp.rx_fn_txif_drop);
	seq_puts(s, "+-----------------+------------+--------------------------------+--------------------------------+\n");
}

static int dpdp_dev_show(struct seq_file *s, void *unused)
{
	if (!capable(CAP_SYS_PACCT))
		return -EACCES;

	seq_puts(s, "+-----------------+------------+--------------------------------+--------------------------------+\n");
	seq_printf(s, "| %-15s | %-10s | %-30s | %-30s |\n",
		   "dev", "", "", "");
	seq_printf(s, "| %-15s | %-10s | %-30s | %-30s |\n",
		   "(key->bkt)", "port/subif", "to platform", "from platform");
	seq_puts(s, "+-----------------+------------+--------------------------------+--------------------------------+\n");
	dpdp_device_dump(dump_cb, s);
	return 0;
}

DEFINE_SHOW_ATTRIBUTE(dpdp_dev);

void directpath_debugfs_exit(void)
{
	debugfs_remove_recursive(dir);
	dir = NULL;
}

void directpath_debugfs_init(void)
{
	struct dentry *dent;

	if (unlikely(!debugfs_initialized())) {
		pr_err("debugfs not initialized yet\n");
		return;
	}

	dir = debugfs_create_dir("dpdp", NULL);
	if (IS_ERR_OR_NULL(dir)) {
		pr_err("debugfs_create_dir() failed for dpdp dir\n");
		return;
	}

	dent = debugfs_create_file("dev", 0600, dir, NULL, &dpdp_dev_fops);
	if (IS_ERR_OR_NULL(dent)) {
		pr_err("debugfs_create_file() failed for dev file\n");
		goto out;
	}

	return;

out:
	directpath_debugfs_exit();
}
