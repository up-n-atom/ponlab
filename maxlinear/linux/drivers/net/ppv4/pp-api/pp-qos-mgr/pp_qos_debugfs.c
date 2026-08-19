/*
 * Copyright (C) 2020-2025 MaxLinear, Inc.
 * Copyright (C) 2017-2020 Intel Corporation
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
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: Packet Processor QoS Driver
 */

#define pr_fmt(fmt) "[PP_QOS_DBG]:%s:%d: " fmt, __func__, __LINE__

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include "pp_debugfs_common.h"
#include "pp_qos_common.h"
#include "pp_qos_utils.h"
#include "pp_qos_fw.h"
#include "pp_regs.h"
#include "infra.h"
#include "pp_desc.h"
#include "qos_wred_regs.h"

#define PP_QOS_DEBUGFS_DIR "ppv4_qos"
#define AQM_FRACTIONAL_BITS (24)

static u16 g_node;

/**
 * @brief AQM debugfs dir
 */
struct aqm_dbgfs_db {
	u32 dbg_aqm_context_id;
	u32 qdev_id;
};

void remove_node(char *cmd_buf, void *data)
{
	struct pp_qos_dev *qdev;
	char node_type[16];
	u32 count, id = 0;

	qdev = pp_qos_dev_open((unsigned long)data);
	if (sscanf(cmd_buf, "%10s %u", node_type, &id) != 2) {
		QOS_LOG_ERR("sscanf err\n");
		return;
	}

	if (!strncmp(node_type, "port", strlen("port"))) {
		pp_qos_port_remove(qdev, id);
	} else if (!strncmp(node_type, "sched", strlen("sched"))) {
		pp_qos_sched_remove(qdev, id);
	} else if (!strncmp(node_type, "queue", strlen("queue"))) {
		pp_qos_queue_remove(qdev, id);
	} else if (!strncmp(node_type, "cont_queue", strlen("cont_queue"))) {
		if (sscanf(cmd_buf, "%16s %u %u", node_type, &id,
			   &count) != 3) {
			QOS_LOG_ERR("sscanf err\n");
			return;
		}
		pp_qos_contiguous_queue_remove(qdev, id, count);
	} else {
		QOS_LOG_ERR("Type %s not supported\n", node_type);
	}
}

void remove_node_help(struct seq_file *f)
{
	seq_puts(f, "<port/sched/queue> <node id>\n");
	seq_puts(f, "<cont_queue> <start id> <count>\n");
}

PP_DEFINE_DEBUGFS(remove, remove_node_help, remove_node);

void allocate_node(char *cmd_buf, void *data)
{
	struct pp_qos_dev *qdev;
	u32 phy = 0, id, count;
	char node_type[16];
	u32 *ids, *rlms;
	s32 rc;

	qdev = pp_qos_dev_open((unsigned long)data);
	if (sscanf(cmd_buf, "%16s", node_type) != 1) {
		QOS_LOG_ERR("sscanf err\n");
		return;
	}

	if (!strncmp(node_type, "port", 6)) {
		if (sscanf(cmd_buf, "%16s %u", node_type, &phy) != 2) {
			QOS_LOG_ERR("sscanf err\n");
			return;
		}
		pp_qos_port_allocate(qdev, phy, &id);
	} else if (!strncmp(node_type, "sched", strlen("sched"))) {
		pp_qos_sched_allocate(qdev, &id);
	} else if (!strncmp(node_type, "queue_id_phy",
			    strlen("queue_id_phy"))) {
		pp_qos_queue_allocate_id_phy(qdev, &id, &phy);
		QOS_LOG_INFO("Allocated id %u rlm %u\n", id, phy);
		return;
	} else if (!strncmp(node_type, "queue", strlen("queue"))) {
		pp_qos_queue_allocate(qdev, &id);
	} else if (!strncmp(node_type, "cont_queue", strlen("cont_queue"))) {
		if (sscanf(cmd_buf, "%16s %u", node_type, &count) != 2) {
			QOS_LOG_ERR("sscanf err\n");
			return;
		}
		ids = kzalloc(qdev->init_params.reserved_queues * sizeof(u32),
			      GFP_KERNEL);
		if (ptr_is_null(ids))
			return;
		rlms = kzalloc(qdev->init_params.reserved_queues * sizeof(u32),
			       GFP_KERNEL);
		if (ptr_is_null(rlms)) {
			kfree(ids);
			return;
		}

		rc = pp_qos_contiguous_queue_allocate(qdev, ids, rlms, count);
		if (!rc) {
			QOS_LOG_INFO("Alloc %u contq start @ id %u rlm %u\n",
				     count, *ids, *rlms);
		}

		kfree(ids);
		kfree(rlms);
		return;
	} else {
		QOS_LOG_ERR("Type %s not supported\n", node_type);
		return;
	}

	QOS_LOG_INFO("Allocated id %u\n", id);
}

void allocate_node_help(struct seq_file *f)
{
	seq_printf(f, "<port/sched/queue> <port phy (%#x for automatic phy)>\n",
		   ALLOC_PORT_ID);
	seq_puts(f, "<cont_queue> <count>\n");
	seq_puts(f, "<queue_id_phy>\n");
}

PP_DEFINE_DEBUGFS(allocate, allocate_node_help, allocate_node);

void queue_flush(char *cmd_buf, void *data)
{
	struct pp_qos_dev *qdev;
	u32 id = 0;

	qdev = pp_qos_dev_open((unsigned long)data);

	if (kstrtou32(cmd_buf, 10, &id)) {
		QOS_LOG_ERR("kstrtou32 err\n");
		return;
	}

	qos_queue_flush(qdev, id);
}

void queue_flush_help(struct seq_file *f)
{
	seq_puts(f, "<queue id>\n");
}

PP_DEFINE_DEBUGFS(flush, queue_flush_help, queue_flush);

void add_shared_bwl_group(char *cmd_buf, void *data)
{
	struct pp_qos_dev *qdev;
	u32 max_burst;
	u32 id = 0;

	qdev = pp_qos_dev_open((unsigned long)data);

	if (kstrtou32(cmd_buf, 10, &max_burst)) {
		QOS_LOG_ERR("kstrtou32 err\n");
		return;
	}

	qos_shared_limit_group_add(qdev, max_burst, &id);

	QOS_LOG_INFO("id %u, max_burst %u\n", id, max_burst);
}

void add_shared_bwl_group_help(struct seq_file *f)
{
	seq_puts(f, "<max_burst>\n");
}

PP_DEFINE_DEBUGFS(shared_grp_add, add_shared_bwl_group_help,
		  add_shared_bwl_group);

void remove_shared_bwl_group(char *cmd_buf, void *data)
{
	struct pp_qos_dev *qdev;
	u32 id = 0;

	qdev = pp_qos_dev_open((unsigned long)data);

	if (kstrtou32(cmd_buf, 10, &id)) {
		QOS_LOG_ERR("kstrtou32 err\n");
		return;
	}

	qos_shared_limit_group_remove(qdev, id);
}

void remove_shared_bwl_group_help(struct seq_file *f)
{
	seq_puts(f, "<bwl group id to remove>\n");
}

PP_DEFINE_DEBUGFS(shared_grp_del, remove_shared_bwl_group_help,
		  remove_shared_bwl_group);

struct dbg_prop {
	char          field[32];
	char          desc[128];
	u8            data_type; /* 0 - normal, 1 - pointer */
	u32           *dest;
	unsigned long **pdest;
};

struct dbg_props_cbs {
	s32 (*first_prop_cb)(struct pp_qos_dev *qdev,
			     char *field,
			     u32 val,
			     void *user_data);

	s32 (*done_props_cb)(struct pp_qos_dev *qdev,
			     u32 val, void *user_data);
};

void qos_dbg_props(char *cmd_buf,
		   struct pp_qos_dev *qdev,
		   struct dbg_props_cbs *cbs,
		   struct dbg_prop props[],
		   u16 num_props,
		   void *user_data)
{
	u8 *field;
	char *tok, *ptr, *pval;
	unsigned long res;
	u16 i, num_changed = 0;
	u32 id = PP_QOS_INVALID_ID;
	u32 len, first_prop = 1;
	s32 ret;

	field = kzalloc(PP_DBGFS_WR_STR_MAX, GFP_KERNEL);
	if (!field)
		return;

	ptr = (char *)cmd_buf;

	while ((tok = strsep(&ptr, " \t\n\r")) != NULL) {
		if (tok[0] == '\0')
			continue;

		ret = strscpy(field, tok, PP_DBGFS_WR_STR_MAX);
		if (ret < 0) {
			QOS_LOG_ERR("Strscpy failed (%d)\n", ret);
			goto out;
		}

		pval = strchr(field, '=');
		if (!pval) {
			QOS_LOG_ERR("Wrong format for prop %s\n", tok);
			goto out;
		}

		*pval = '\0';
		pval++;

		ret = kstrtoul(pval, 0, &res);
		if (ret) {
			QOS_LOG_ERR("kstrtoul failure (%d)\n", ret);
			goto out;
		}

		if (first_prop) {
			first_prop = 0;
			id = res;
			if (cbs && cbs->first_prop_cb &&
			    cbs->first_prop_cb(qdev, field, res,
					       user_data)) {
				QOS_LOG_ERR("first_prop_cb failed\n");
				goto out;
			}
		}

		for (i = 0; i < num_props ; i++) {
			len = max(strlen(props[i].field), strlen(field));
			if (!strncmp(field, props[i].field, len)) {
				if (props[i].data_type == 0)
					*(props[i].dest) = res;
				else
					*(props[i].pdest) = (void *)res;
				num_changed++;
				break;
			}
		}

		if (i == num_props)
			QOS_LOG_ERR("Not supported field %s", field);
	}

	if (id != PP_QOS_INVALID_ID) {
		/* If only logical id was set, print current configuration */
		if (num_changed == 1) {
			QOS_LOG_INFO("Current configuration:\n");

			for (i = 0; i < num_props ; i++) {
				if (props[i].data_type == 0) {
					QOS_LOG_INFO("%-30s%u\n",
						     props[i].field,
						     *props[i].dest);
				} else {
					QOS_LOG_INFO("%-30s%#lx\n",
						     props[i].field,
						     (ulong)(*props[i].pdest));
				}
			}

			goto out;
		}

		if (cbs && cbs->done_props_cb) {
			if (cbs->done_props_cb(qdev, id, user_data)) {
				QOS_LOG_ERR("done_props_cb failed\n");
				goto out;
			}
		}
	}

out:
	kfree(field);
}

void qos_dbg_props_help(struct seq_file *f, const char *name,
			const char *format, struct dbg_prop props[],
			u16 num_props)
{
	u32 idx;

	seq_printf(f, "<---- %s---->\n", name);
	seq_printf(f, "[FORMAT] %s\n", format);
	seq_puts(f, "[FORMAT] If only id is set, operation is get conf\n");
	seq_puts(f, "Supported fields\n");

	for (idx = 0; idx < num_props ; idx++)
		seq_printf(f, "%-30s%s\n", props[idx].field, props[idx].desc);
}

static void dbg_add_prop(struct dbg_prop *props, u16 *pos, u16 size,
			 const char *name, const char *desc, u32 *dest)
{
	s32 ret;

	if (*pos >= size) {
		QOS_LOG_ERR("pos %d >= size %d", *pos, size);
		return;
	}

	ret = strscpy(props[*pos].field, name, sizeof(props[*pos].field));
	if (ret < 0) {
		QOS_LOG_ERR("Strscpy failed (%d)\n", ret);
		return;
	}

	ret = strscpy(props[*pos].desc, desc, sizeof(props[*pos].desc));
	if (ret < 0) {
		QOS_LOG_ERR("Strscpy failed (%d)\n", ret);
		return;
	}

	props[*pos].data_type = 0;
	props[*pos].dest = dest;

	(*pos)++;
}

static void dbg_add_prop_ptr(struct dbg_prop *props, u16 *pos, u16 size,
			     const char *name, const char *desc,
			     unsigned long **dest)
{
	s32 ret;

	if (*pos >= size) {
		QOS_LOG_ERR("pos %d >= size %d", *pos, size);
		return;
	}

	ret = strscpy(props[*pos].field, name, sizeof(props[*pos].field));
	if (ret < 0) {
		QOS_LOG_ERR("Strscpy failed (%d)\n", ret);
		return;
	}

	ret = strscpy(props[*pos].desc, desc, sizeof(props[*pos].desc));
	if (ret < 0) {
		QOS_LOG_ERR("Strscpy failed (%d)\n", ret);
		return;
	}

	props[*pos].data_type = 1;
	props[*pos].pdest = dest;

	(*pos)++;
}

static u16 create_port_props(struct dbg_prop *props, u16 size,
			     u32 *id, struct pp_qos_port_conf *pconf)
{
	u16 num = 0;

	dbg_add_prop(props, &num, size, "port",
		     "Logical id. Must exist as the first property!", id);
	dbg_add_prop(props, &num, size, "bw", "Limit in kbps (80kbps steps)",
		     &pconf->common_prop.bandwidth_limit);
	dbg_add_prop(props, &num, size, "shared",
		     "Shared bw group: 1-511 (0 for remove group)",
		     &pconf->common_prop.shared_bw_group);
	dbg_add_prop(props, &num, size, "max_burst",
		     "Defines the max quantas that can be accumulated (1 << shift)",
		     &pconf->common_prop.max_burst);
	dbg_add_prop(props, &num, size, "arb",
		     "Arbitration: 0 - WSP, 1 - WRR",
		     &pconf->port_parent_prop.arbitration);
	dbg_add_prop(props, &num, size, "be",
		     "Best effort enable: best effort scheduling is enabled",
		     &pconf->port_parent_prop.best_effort_enable);
	dbg_add_prop_ptr(props, &num, size, "r_addr",
			 "Ring address", (ulong **)&pconf->ring_address);
	dbg_add_prop(props, &num, size, "r_size",
		     "Ring size", &pconf->ring_size);
	dbg_add_prop(props, &num, size, "pkt_cred",
		     "Packet credit: 0 - byte credit, 1 - packet credit",
		     &pconf->packet_credit_enable);
	dbg_add_prop(props, &num, size, "cred", "Port credit", &pconf->credit);
	dbg_add_prop(props, &num, size, "dis",
		     "Disable port tx", &pconf->disable);
	dbg_add_prop(props, &num, size, "green_threshold",
		     "Egress bytes green threshold", &pconf->green_threshold);
	dbg_add_prop(props, &num, size, "yellow_threshold",
		     "Egress bytes yellow threshold",
		     &pconf->yellow_threshold);
	dbg_add_prop(props, &num, size, "enhanced_wsp",
		     "Enhanced WSP",
		     &pconf->enhanced_wsp);

	return num;
}

static s32 port_first_prop_cb(struct pp_qos_dev *qdev,
			      char *field,
			      u32 val,
			      void *user_data)
{
	struct qos_node *node;
	bool configured = false;

	/* Make sure first property is the port id */
	if (strncmp(field, "port", strlen("port"))) {
		QOS_LOG_ERR("First prop (%s) must be port\n", field);
		return -EINVAL;
	}

	if (is_node_configured(qdev, val, &configured))
		return -EINVAL;

	if (!configured)
		goto set_default;

	node = get_conform_node(qdev, val, NULL);
	if (node_port(node))
		return pp_qos_port_conf_get(qdev, val, user_data);

set_default:
	pp_qos_port_conf_set_default(user_data);
	return 0;
}

static s32 port_done_props_cb(struct pp_qos_dev *qdev,
			      u32 val, void *user_data)
{
	if (pp_qos_port_set(qdev, val, user_data) != 0) {
		QOS_LOG_ERR("pp_qos_port_set failed (id %u)", val);
		return -EINVAL;
	}

	return 0;
}

void port(char *cmd_buf, void *data)
{
	struct pp_qos_port_conf conf;
	struct dbg_props_cbs cbs = {port_first_prop_cb, port_done_props_cb};
	u32 id = PP_QOS_INVALID_ID;
	u16 num_props;
	struct dbg_prop *props;
	struct pp_qos_dev *qdev;

	qdev = pp_qos_dev_open((unsigned long)data);
	props = kmalloc_array(DBG_MAX_PROPS, sizeof(struct dbg_prop),
			      GFP_KERNEL | __GFP_ZERO);
	if (!props)
		return;

	num_props = create_port_props(props, DBG_MAX_PROPS, &id, &conf);
	qos_dbg_props(cmd_buf, qdev, &cbs, props, num_props, &conf);
	kfree(props);
}

void port_help(struct seq_file *f)
{
	struct pp_qos_port_conf conf;
	const char *name = "set port";
	const char *format =
		"echo port=[logical_id] [field]=[value]... > port";
	struct dbg_prop *props;
	u16 num_props;
	u32 id = PP_QOS_INVALID_ID;

	props = kmalloc_array(DBG_MAX_PROPS, sizeof(struct dbg_prop),
			      GFP_KERNEL);
	if (!props)
		return;

	num_props = create_port_props(props, DBG_MAX_PROPS, &id, &conf);
	qos_dbg_props_help(f, name, format, props, num_props);
	kfree(props);
}

PP_DEFINE_DEBUGFS(port_set, port_help, port);

static u16 create_sched_props(struct dbg_prop *props, u16 size,
			      u32 *id, struct pp_qos_sched_conf *pconf)
{
	u16 num = 0;

	dbg_add_prop(props, &num, size, "sched",
		     "Logical id. Must exist as the first property!", id);
	dbg_add_prop(props, &num, size, "bw", "Limit in kbps (80kbps steps)",
		     &pconf->common_prop.bandwidth_limit);
	dbg_add_prop(props, &num, size, "shared",
		     "Shared bw group: 1-511 (0 for remove group)",
		     &pconf->common_prop.shared_bw_group);
	dbg_add_prop(props, &num, size, "max_burst",
		     "Defines the max quantas that can be accumulated (1 << shift)",
		     &pconf->common_prop.max_burst);
	dbg_add_prop(props, &num, size, "arb",
		     "Arbitration: 0 - WSP, 1 - WRR",
		     &pconf->sched_parent_prop.arbitration);
	dbg_add_prop(props, &num, size, "be",
		     "Best effort enable: best effort scheduling is enabled",
		     &pconf->sched_parent_prop.best_effort_enable);
	dbg_add_prop(props, &num, size, "parent", "logical parent id",
		     &pconf->sched_child_prop.parent);
	dbg_add_prop(props, &num, size, "priority",
		     "priority (0-7) in WSP",
		     &pconf->sched_child_prop.priority);
	dbg_add_prop(props, &num, size, "wrr_weight", "percentage from parent",
		     &pconf->sched_child_prop.wrr_weight);

	return num;
}

static s32 sched_first_prop_cb(struct pp_qos_dev *qdev,
			       char *field,
			       u32 val,
			       void *user_data)
{
	struct qos_node *node;
	bool configured = false;

	/* Make sure first property is the sched id */
	if (strncmp(field, "sched", strlen("sched"))) {
		QOS_LOG_ERR("First prop (%s) must be sched\n", field);
		return -EINVAL;
	}

	if (is_node_configured(qdev, val, &configured))
		return -EINVAL;

	if (!configured)
		goto set_default;

	node = get_conform_node(qdev, val, NULL);
	if (node_sched(node))
		return pp_qos_sched_conf_get(qdev, val, user_data);

set_default:
	pp_qos_sched_conf_set_default(user_data);
	return 0;
}

static s32 sched_done_props_cb(struct pp_qos_dev *qdev,
			       u32 val, void *user_data)
{
	if (pp_qos_sched_set(qdev, val, user_data) != 0) {
		QOS_LOG_ERR("pp_qos_sched_set failed (id %u)", val);
		return -EINVAL;
	}

	return 0;
}

void sched(char *cmd_buf, void *data)
{
	struct pp_qos_sched_conf conf;
	struct dbg_props_cbs cbs = {sched_first_prop_cb, sched_done_props_cb};
	u32 id = PP_QOS_INVALID_ID;
	u16 num_props;
	struct dbg_prop *props;
	struct pp_qos_dev *qdev;

	qdev = pp_qos_dev_open((unsigned long)data);
	props = kmalloc_array(DBG_MAX_PROPS, sizeof(struct dbg_prop),
			      GFP_KERNEL);
	if (!props)
		return;

	num_props = create_sched_props(props, DBG_MAX_PROPS, &id, &conf);

	qos_dbg_props(cmd_buf, qdev, &cbs, props, num_props, &conf);
	kfree(props);
}

void sched_help(struct seq_file *f)
{
	struct pp_qos_sched_conf conf;
	const char *name = "set sched";
	const char *format =
		"echo sched=[logical_id] [field]=[value]... > sched";
	struct dbg_prop *props;
	u16 num_props;
	u32 id = PP_QOS_INVALID_ID;

	props = kmalloc_array(DBG_MAX_PROPS, sizeof(struct dbg_prop),
			      GFP_KERNEL);
	if (!props)
		return;

	num_props = create_sched_props(props, DBG_MAX_PROPS, &id, &conf);
	qos_dbg_props_help(f, name, format, props, num_props);
	kfree(props);
}

PP_DEFINE_DEBUGFS(sched_set, sched_help, sched);

static u16 create_queue_props(struct dbg_prop *props, u16 size,
			      u32 *id, struct pp_qos_queue_conf *pconf)
{
	u16 num = 0;

	dbg_add_prop(props, &num, size, "queue",
		     "Logical id. Must exist as the first property!", id);
	dbg_add_prop(props, &num, size, "bw", "Limit in kbps (80kbps steps)",
		     &pconf->common_prop.bandwidth_limit);
	dbg_add_prop(props, &num, size, "shared",
		     "Shared bw group: 1-511 (0 for remove group)",
		     &pconf->common_prop.shared_bw_group);
	dbg_add_prop(props, &num, size, "max_burst",
		     "Defines the max quantas that can be accumulated (1 << shift)",
		     &pconf->common_prop.max_burst);
	dbg_add_prop(props, &num, size, "parent", "logical parent id",
		     &pconf->queue_child_prop.parent);
	dbg_add_prop(props, &num, size, "priority",
		     "priority (0-7) in WSP",
		     &pconf->queue_child_prop.priority);
	dbg_add_prop(props, &num, size, "wrr_weight", "percentage from parent",
		     &pconf->queue_child_prop.wrr_weight);
	dbg_add_prop(props, &num, size, "blocked", "drop enqueued packets",
		     &pconf->blocked);
	dbg_add_prop(props, &num, size, "wred_enable", "enable wred drops",
		     &pconf->wred_enable);
	dbg_add_prop(props, &num, size, "wred_fixed_drop_prob",
		     "fixed prob instead of slope",
		     &pconf->wred_fixed_drop_prob_enable);
	dbg_add_prop(props, &num, size, "min_avg_green",
		     "Wred start of the slope area",
		     &pconf->wred_min_avg_green);
	dbg_add_prop(props, &num, size, "max_avg_green",
		     "Wred end of the slope area",
		     &pconf->wred_max_avg_green);
	dbg_add_prop(props, &num, size, "slope_green", "Wred 0-100 scale",
		     &pconf->wred_slope_green);
	dbg_add_prop(props, &num, size, "fixed_drop_prob_green",
		     "Wred fixed drop rate",
		     &pconf->wred_fixed_drop_prob_green);
	dbg_add_prop(props, &num, size, "min_avg_yellow",
		     "Wred start of the slope area",
		     &pconf->wred_min_avg_yellow);
	dbg_add_prop(props, &num, size, "max_avg_yellow",
		     "Wred end of the slope area",
		     &pconf->wred_max_avg_yellow);
	dbg_add_prop(props, &num, size, "slope_yellow", "Wred 0-100 scale",
		     &pconf->wred_slope_yellow);
	dbg_add_prop(props, &num, size, "fixed_drop_prob_yellow",
		     "Wred fixed drop rate",
		     &pconf->wred_fixed_drop_prob_yellow);
	dbg_add_prop(props, &num, size, "min_guaranteed",
		     "Wred min guaranteed for this queue",
		     &pconf->wred_min_guaranteed);
	dbg_add_prop(props, &num, size, "max_allowed",
		     "Wred max allowed for this queue",
		     &pconf->wred_max_allowed);
	dbg_add_prop(props, &num, size, "codel_en",
		     "Enable codel for this queue",
		     &pconf->codel_en);
	dbg_add_prop(props, &num, size, "eir",
		     "Excess Information Rate",
		     &pconf->eir);
	dbg_add_prop(props, &num, size, "ebs",
		     "Excess Burst Size",
		     &pconf->ebs);
	dbg_add_prop(props, &num, size, "fast",
		     "Is fast q",
		     &pconf->is_fast_q);

	return num;
}

static s32 queue_first_prop_cb(struct pp_qos_dev *qdev,
			       char *field,
			       u32 val,
			       void *user_data)
{
	struct qos_node *node;
	bool configured = false;

	/* Make sure first property is the queue id */
	if (strncmp(field, "queue", strlen("queue"))) {
		QOS_LOG_ERR("First prop (%s) must be queue\n", field);
		return -EINVAL;
	}

	if (is_node_configured(qdev, val, &configured))
		return -EINVAL;

	if (!configured)
		goto set_default;

	node = get_conform_node(qdev, val, NULL);
	if (node_queue(node))
		return pp_qos_queue_conf_get(qdev, val, user_data);

set_default:
	pp_qos_queue_conf_set_default(user_data);
	return 0;
}

static s32 queue_done_props_cb(struct pp_qos_dev *qdev,
			       u32 val, void *user_data)
{
	if (pp_qos_queue_set(qdev, val, user_data) != 0) {
		QOS_LOG_ERR("pp_qos_queue_set failed (id %u)", val);
		return -EINVAL;
	}

	return 0;
}

void queue(char *cmd_buf, void *data)
{
	struct pp_qos_queue_conf conf;
	struct dbg_props_cbs cbs = {queue_first_prop_cb, queue_done_props_cb};
	u32 id = PP_QOS_INVALID_ID;
	u16 num_props;
	struct dbg_prop *props;
	struct pp_qos_dev *qdev;

	qdev = pp_qos_dev_open((unsigned long)data);
	props = kmalloc_array(DBG_MAX_PROPS, sizeof(struct dbg_prop),
			      GFP_KERNEL);
	if (!props)
		return;

	num_props = create_queue_props(props, DBG_MAX_PROPS, &id, &conf);

	qos_dbg_props(cmd_buf, qdev, &cbs, props, num_props, &conf);
	kfree(props);
}

void queue_help(struct seq_file *f)
{
	struct pp_qos_queue_conf conf;
	const char *name = "set queue";
	const char *format =
		"echo queue=[logical_id] [field]=[value]... > queue";
	struct dbg_prop *props;
	u16 num_props;
	u32 id = PP_QOS_INVALID_ID;

	props = kmalloc_array(DBG_MAX_PROPS, sizeof(struct dbg_prop),
			      GFP_KERNEL);
	if (!props)
		return;

	num_props = create_queue_props(props, DBG_MAX_PROPS, &id, &conf);
	qos_dbg_props_help(f, name, format, props, num_props);
	kfree(props);
}

PP_DEFINE_DEBUGFS(queue_set, queue_help, queue);

static s32 syncq_first_prop_cb(struct pp_qos_dev *qdev, char *field, u32 val,
			       void *user_data)
{
	struct qos_node *node;
	bool configured = false;

	/* Make sure first property is the queue id */
	if (strncmp(field, "queue", strlen("queue"))) {
		QOS_LOG_ERR("First prop (%s) must be queue\n", field);
		return -EINVAL;
	}

	if (is_node_configured(qdev, val, &configured))
		return -EINVAL;

	if (!configured)
		goto set_default;

	node = get_conform_node(qdev, val, NULL);
	if (node_syncq(node)) {
		QOS_LOG_ERR("sync queue get isn't supported\n");
		return -EINVAL;
	}

set_default:
	pp_qos_queue_conf_set_default(user_data);
	return 0;
}

static s32 syncq_done_props_cb(struct pp_qos_dev *qdev, u32 val,
			       void *user_data)
{
	u32 rlm;

	if (qos_sync_queue_add(qdev, val, &rlm, user_data) != 0) {
		QOS_LOG_ERR("qos_sync_queue_add failed (id %u)", val);
		return -EINVAL;
	}

	return 0;
}

void syncq(char *cmd_buf, void *data)
{
	struct pp_qos_queue_conf conf;
	struct dbg_props_cbs cbs = {syncq_first_prop_cb, syncq_done_props_cb};
	u32 id = PP_QOS_INVALID_ID;
	u16 num_props;
	struct dbg_prop *props;
	struct pp_qos_dev *qdev;

	qdev = pp_qos_dev_open((unsigned long)data);
	props = kmalloc_array(DBG_MAX_PROPS, sizeof(struct dbg_prop),
			      GFP_KERNEL);
	if (!props)
		return;

	num_props = create_queue_props(props, DBG_MAX_PROPS, &id, &conf);

	qos_dbg_props(cmd_buf, qdev, &cbs, props, num_props, &conf);
	kfree(props);
}

void syncq_help(struct seq_file *f)
{
	struct pp_qos_queue_conf conf;
	const char *name = "add sync queue";
	const char *format =
		"echo queue=[logical_id] [field]=[value]... > syncq";
	struct dbg_prop *props;
	u16 num_props;
	u32 id = PP_QOS_INVALID_ID;

	props = kmalloc_array(DBG_MAX_PROPS, sizeof(struct dbg_prop),
			      GFP_KERNEL);
	if (!props)
		return;

	num_props = create_queue_props(props, DBG_MAX_PROPS, &id, &conf);
	qos_dbg_props_help(f, name, format, props, num_props);
	kfree(props);
}

PP_DEFINE_DEBUGFS(syncq_add, syncq_help, syncq);

static int fw_logger_get(void *data, u64 *val)
{
	struct pp_qos_dev *qdev = pp_qos_dev_open((unsigned long)data);

	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	print_fw_log(qdev);

	return 0;
}

static int fw_logger_set(void *data, u64 val)
{
	struct pp_qos_dev *qdev = pp_qos_dev_open((unsigned long)data);
	s32    ret;

	if (!capable(CAP_SYS_PACCT))
		return -EPERM;

	QOS_LOG_INFO("fw_logger_set setting new fw logger level %u\n",
		     (u32)val);

	switch (val) {
	case 0:
		create_init_logger_cmd(qdev, UC_LOGGER_LEVEL_INFO,
				       UC_LOGGER_MODE_NONE);
		break;
	case UC_LOGGER_LEVEL_FATAL:
	case UC_LOGGER_LEVEL_ERROR:
	case UC_LOGGER_LEVEL_WARNING:
	case UC_LOGGER_LEVEL_INFO:
	case UC_LOGGER_LEVEL_DEBUG:
	case UC_LOGGER_LEVEL_DUMP_REGS:
		create_init_logger_cmd(qdev, (int)val,
				       UC_LOGGER_MODE_WRITE_HOST_MEM);
		break;
	default:
		QOS_LOG_INFO("Not supported fw logger level");
		QOS_LOG_INFO("Optional levels:\n");
		QOS_LOG_INFO("None: 0\n");
		QOS_LOG_INFO("Fatal: %d\n", UC_LOGGER_LEVEL_FATAL);
		QOS_LOG_INFO("Error: %d\n", UC_LOGGER_LEVEL_ERROR);
		QOS_LOG_INFO("Warning: %d\n", UC_LOGGER_LEVEL_WARNING);
		QOS_LOG_INFO("Info: %d\n", UC_LOGGER_LEVEL_INFO);
		QOS_LOG_INFO("Debug: %d\n", UC_LOGGER_LEVEL_DEBUG);
		QOS_LOG_INFO("Register Dump: %d\n", UC_LOGGER_LEVEL_DUMP_REGS);
		break;
	}

	update_cmd_id(&qdev->drvcmds);
	ret = transmit_cmds(qdev);
	if (ret)
		QOS_LOG_ERR("transmit_cmds failed (%d)\n", ret);

	return 0;
}

DEFINE_SIMPLE_ATTRIBUTE(dbg_fw_logger_fops, fw_logger_get,
			fw_logger_set, "%llu\n");

/**
 * @brief dump node information
 */
void pp_qos_dbg_node_show(struct seq_file *s)
{
	struct pp_qos_dev *qdev;
	const char *typename;
	u32 phy;
	u32 id;
	u32 i;
	static const char *const types[] = {"Port", "Sched", "Queue",
		"Unknown"};
	static const char *const yesno[] = {"No", "Yes"};
	s32 rc;
	struct pp_qos_node_info info = {0, };

	qdev = pp_qos_dev_open((unsigned long)s->private);
	QOS_LOG_INFO("node_show called\n");
	if (qdev) {
		if (unlikely(!qos_device_ready(qdev))) {
			seq_puts(s, "Device is not ready !!!!\n");
			return;
		}

		id = g_node;
		phy = get_phy_from_id(qdev->mapping, id);
		if (unlikely(!QOS_PHY_VALID(phy))) {
			seq_printf(s, "Invalid id %u\n", id);
			return;
		}
		rc = qos_get_node_info(qdev, id, &info);
		if (rc) {
			seq_printf(s, "Could not get info for node %u!!!!\n",
				   id);
			return;
		}

		if (info.type >=  PPV4_QOS_NODE_TYPE_PORT &&
		    info.type <= PPV4_QOS_NODE_TYPE_QUEUE)
			typename = types[info.type];
		else
			typename = types[3];

		seq_printf(s, "%u(%u) - %s: internal node(%s)\n",
			   id, phy,
			   typename,
			   yesno[!!info.is_internal]);

		if (info.preds[0].phy != PPV4_QOS_INVALID) {
			seq_printf(s, "%u(%u)", id, phy);
			for (i = 0; i < 6; ++i) {
				if (info.preds[i].phy == PPV4_QOS_INVALID)
					break;
				seq_printf(s, " ==> %u(%u)",
					   info.preds[i].id,
					   info.preds[i].phy);
			}
			seq_puts(s, "\n");
		}

		if (info.children[0].phy != PPV4_QOS_INVALID) {
			for (i = 0; i < 8; ++i) {
				if (info.children[i].phy == PPV4_QOS_INVALID)
					break;
				seq_printf(s, "%u(%u) ",
					   info.children[i].id,
					   info.children[i].phy);
			}
			seq_puts(s, "\n");
		}

		if (info.type == PPV4_QOS_NODE_TYPE_QUEUE)  {
			seq_printf(s, "Physical queue: %u\n",
				   info.queue_physical_id);
			seq_printf(s, "Port: %u\n", info.port);
		}

		seq_printf(s, "Bandwidth: %u Kbps\n", info.bw_limit);
	}
}

PP_DEFINE_DEBUGFS(node_show, pp_qos_dbg_node_show, NULL);

/**
 * @brief dump node statistics
 */
void pp_qos_dbg_stat_show(struct seq_file *s)
{
	struct pp_qos_dev *qdev;
	struct pp_qos_queue_stat qstat;
	struct pp_qos_port_stat pstat;
	struct qos_node *node;
	u32 phy;
	u32 id;

	qdev = pp_qos_dev_open((unsigned long)s->private);
	QOS_LOG_INFO("node_show called\n");
	if (unlikely(!qdev)) {
		seq_puts(s, "qdev Null\n");
		return;
	}

	if (unlikely(!qos_device_ready(qdev))) {
		seq_puts(s, "Device is not ready\n");
		return;
	}

	id = g_node;
	phy = get_phy_from_id(qdev->mapping, id);
	if (unlikely(!QOS_PHY_VALID(phy))) {
		seq_printf(s, "Invalid id %u\n", id);
		return;
	}

	node = get_node_from_phy(qdev->nodes, phy);

	if (unlikely(!node_used(node))) {
		seq_printf(s, "Node %u is unused\n", id);
		return;
	}

	seq_printf(s, "%u(%u) - ", id, phy);
	if (node_queue(node)) {
		seq_puts(s, "Queue\n");
		memset(&qstat, 0, sizeof(qstat));
		if (pp_qos_queue_stat_get(qdev, id, &qstat) == 0) {
			seq_printf(s, "queue_average_size_bytes:%u\n",
				   qstat.queue_average_size_bytes);
		
			seq_printf(s, "queue_packets_occupancy:%u\n",
				   qstat.queue_packets_occupancy);

			seq_printf(s, "queue_bytes_occupancy:%u\n",
				   qstat.queue_bytes_occupancy);

			seq_printf(s, "total_packets_accepted:%u\n",
				   qstat.total_packets_accepted);

			seq_printf(s, "total_packets_dropped:%u\n",
				   qstat.total_packets_dropped);

			seq_printf(s, "total_packets_red_dropped:%u\n",
				   qstat.total_packets_red_dropped);

			seq_printf(s, "total_bytes_accepted:%llu\n",
				   qstat.total_bytes_accepted);

			seq_printf(s, "total_bytes_dropped:%llu\n",
				   qstat.total_bytes_dropped);
		} else {
			seq_puts(s, "Could not obtained statistics\n");
		}
	} else if (node_port(node)) {
		seq_puts(s, "Port\n");
		memset(&pstat, 0, sizeof(pstat));
		if (pp_qos_port_stat_get(qdev, id, &pstat) == 0) {
			seq_printf(s, "total_green_bytes in port's queues:%u\n",
				   pstat.total_green_bytes);

			seq_printf(s, "total_yellow_bytes in port's queues:%u\n",
				   pstat.total_yellow_bytes);

			seq_printf(s, "back pressure status:%u\n",
				   pstat.debug_back_pressure_status);

			seq_printf(s, "Actual packet credit:%u\n",
				   pstat.debug_actual_packet_credit);

			seq_printf(s, "Actual byte credit:%u\n",
				   pstat.debug_actual_byte_credit);
		} else {
			seq_puts(s, "Could not obtained statistics\n");
		}
	} else {
		seq_puts(s, "Node is not a queue or port, no statistics\n");
	}
}

PP_DEFINE_DEBUGFS(stats, pp_qos_dbg_stat_show, NULL);

/**
 * @brief dump queue statistics
 */
void pp_qos_dbg_queues_stats_show(struct seq_file *s)
{
	struct pp_qos_dev *qdev;

	qdev = pp_qos_dev_open((unsigned long)s->private);
	if (unlikely(!qdev)) {
		seq_puts(s, "qdev Null\n");
		return;
	}

	if (unlikely(!qos_device_ready(qdev))) {
		seq_puts(s, "Device is not ready\n");
		return;
	}

	pp_stats_show_seq(s, sizeof(struct queue_stats),
			  qdev->init_params.max_queues, qos_queues_stats_get,
			  qos_queues_stats_show, qdev);
}

/**
 * @brief reset queues statistics
 */
void pp_qos_dbg_queues_stats_reset(char *cmd_buf, void *data)
{
	struct pp_qos_dev *qdev;
	u32 is_reset;

	if ((sscanf(cmd_buf, "%u ", &is_reset) == 1) && (is_reset == 0)) {
		qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
		if (unlikely(ptr_is_null(qdev)))
			return;
		pp_qos_stats_reset(qdev);
		return;
	}

	QOS_LOG_INFO("\nqos_queues_stats help:\n");
	QOS_LOG_INFO("cat pp/qos_queues_stats      : Display statistics\n");
	QOS_LOG_INFO("echo 0 > pp/qos_queues_stats : Reset statistics\n");
}

PP_DEFINE_DEBUGFS(queues_stats, pp_qos_dbg_queues_stats_show,
		  pp_qos_dbg_queues_stats_reset);

/**
 * @brief dump queue statistics in bytes
 */
void pp_qos_dbg_queues_stats_bytes_show(struct seq_file *s)
{
	struct pp_qos_dev *qdev;

	qdev = pp_qos_dev_open((unsigned long)s->private);
	if (unlikely(!qdev)) {
		seq_puts(s, "qdev Null\n");
		return;
	}

	if (unlikely(!qos_device_ready(qdev))) {
		seq_puts(s, "Device is not ready\n");
		return;
	}

	pp_stats_show_bytes_seq(s, sizeof(struct queue_stats),
				qdev->init_params.max_queues,
				qos_queues_stats_get,
				qos_queues_stats_bytes_show, qdev);
}

/**
 * @brief reset queues statistics
 */
void pp_qos_dbg_queues_stats_bytes_reset(char *cmd_buf, void *data)
{
	struct pp_qos_dev *qdev;
	u32 is_reset;

	if ((sscanf(cmd_buf, "%u ", &is_reset) == 1) && is_reset == 0) {
		qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
		if (unlikely(ptr_is_null(qdev)))
			return;
		pp_qos_stats_reset(qdev);
		return;
	}

	QOS_LOG_INFO("\nqos_queues_stats_bytes help:\n");
	QOS_LOG_INFO("cat pp/qos_queues_stats_bytes      : Display statistics\n");
	QOS_LOG_INFO("echo 0 > pp/qos_queues_stats_bytes : Reset statistics\n");
}

PP_DEFINE_DEBUGFS(queues_stats_bytes, pp_qos_dbg_queues_stats_bytes_show,
		  pp_qos_dbg_queues_stats_bytes_reset);

/**
 * @brief dump queues pps statistics
 */
void pp_qos_dbg_queues_pps_show(struct seq_file *s)
{
	struct pp_qos_dev *qdev;

	qdev = pp_qos_dev_open((unsigned long)s->private);
	if (unlikely(!qdev)) {
		seq_puts(s, "qdev Null\n");
		return;
	}

	if (unlikely(!qos_device_ready(qdev))) {
		seq_puts(s, "Device is not ready\n");
		return;
	}

	pp_pps_show_seq(s, sizeof(struct queue_stats),
			qdev->init_params.max_queues, qos_queues_stats_get,
			qos_queues_stats_diff, qos_queues_stats_show, qdev);
}

PP_DEFINE_DEBUGFS(queues_pps, pp_qos_dbg_queues_pps_show, NULL);

/**
 * @brief dump queues pps statistics
 */
void pp_qos_dbg_queues_bps_show(struct seq_file *s)
{
	struct pp_qos_dev *qdev;

	qdev = pp_qos_dev_open((unsigned long)s->private);
	if (unlikely(!qdev)) {
		seq_puts(s, "qdev Null\n");
		return;
	}

	if (unlikely(!qos_device_ready(qdev))) {
		seq_puts(s, "Device is not ready\n");
		return;
	}

	pp_bps_show_seq(s, sizeof(struct queue_stats),
			qdev->init_params.max_queues, qos_queues_stats_get,
			qos_queues_stats_bps_diff, qos_queues_stats_bytes_show,
			qdev);
}

PP_DEFINE_DEBUGFS(queues_bps, pp_qos_dbg_queues_bps_show, NULL);

/**
 * @brief dump complete qos tree
 */
void pp_qos_dbg_tree_show(struct seq_file *s)
{
	struct pp_qos_dev *qdev;

	qdev = pp_qos_dev_open((unsigned long)s->private);
	QOS_LOG_INFO("tree_show called\n");
	if (unlikely(!qdev)) {
		seq_puts(s, "qdev Null\n");
		return;
	}

	if (unlikely(!qos_device_ready(qdev))) {
		seq_puts(s, "Device is not ready\n");
		return;
	}

	qos_dbg_tree_show_locked(qdev, true, s);
}

PP_DEFINE_DEBUGFS(tree, pp_qos_dbg_tree_show, NULL);

/**
 * @brief dump complete qos tree
 */
void pp_qos_dbg_full_tree_show(struct seq_file *s)
{
	struct pp_qos_dev *qdev;

	qdev = pp_qos_dev_open((unsigned long)s->private);
	QOS_LOG_INFO("tree_show called\n");
	if (unlikely(!qdev)) {
		seq_puts(s, "qdev Null\n");
		return;
	}

	if (unlikely(!qos_device_ready(qdev))) {
		seq_puts(s, "Device is not ready\n");
		return;
	}

	qos_dbg_tree_show_locked(qdev, false, s);
}

PP_DEFINE_DEBUGFS(full_tree, pp_qos_dbg_full_tree_show, NULL);

void pp_qos_dbg_tree_remove(struct seq_file *s)
{
	u32 node_id, total_occupancy;
	struct qos_node *node;
	struct pp_qos_dev *qdev;
	struct pp_qos_queue_stat stats;
	u32 idx;
	s32 node_phy;

	qdev = pp_qos_dev_open((unsigned long)s->private);
	if (unlikely(!qos_device_ready(qdev))) {
		seq_puts(s, "Device is not ready\n");
		return;
	}

	/* Iterate through all queue nodes */
	for (node_phy = NUM_OF_NODES - 1; node_phy >= 0; --node_phy) {
		node = get_node_from_phy(qdev->nodes, node_phy);
		if (node_queue(node) && !node_syncq(node)) {
			node_id = get_id_from_phy(qdev->mapping, node_phy);
			pp_qos_queue_block(qdev, node_id);
		}
	}

	memset(&stats, 0, sizeof(stats));

	/* Read stats */
	for (idx = 0; idx < 100; ++idx) {
		total_occupancy = 0;
		for (node_phy = 0; node_phy < NUM_OF_NODES; ++node_phy) {
			node = get_node_from_phy(qdev->nodes, node_phy);

			if (!node_queue(node))
				continue;

			node_id = get_id_from_phy(qdev->mapping, node_phy);
			pp_qos_queue_stat_get(qdev, node_id, &stats);
			total_occupancy += stats.queue_packets_occupancy;
			if (stats.queue_packets_occupancy) {
				QOS_LOG_INFO("Queue %u has %u packet occ\n",
					     node_id,
					     stats.queue_packets_occupancy);
			}
		}

		if (total_occupancy != 0)
			qos_dbg_qm_stat_show(qdev, NULL);
		else
			break;

		msleep(10);
	}

	if (total_occupancy != 0) {
		QOS_LOG_ERR("Cannot remove tree while occupancy=%u\n",
			    total_occupancy);
		qos_dbg_qm_stat_show(qdev, NULL);
		return;
	}

	delete_all_syncq(qdev);

	/* Iterate through all queue nodes */
	for (node_phy = NUM_OF_NODES - 1; node_phy >= 0; --node_phy) {
		node = get_node_from_phy(qdev->nodes, node_phy);
		if (node_queue(node)) {
			node_id = get_id_from_phy(qdev->mapping, node_phy);
			pp_qos_queue_remove(qdev, node_id);
		}
	}

	/* Iterate through all port nodes */
	for (node_phy = 0; node_phy < NUM_OF_NODES; ++node_phy) {
		node = get_node_from_phy(qdev->nodes, node_phy);
		if (node_port(node)) {
			node_id = get_id_from_phy(qdev->mapping, node_phy);
			pp_qos_port_remove(qdev, node_id);
		}
	}

	qos_pools_clean(qdev);
	qos_pools_init(qdev, qdev->init_params.max_ports,
		       qdev->init_params.max_queues,
		       qdev->init_params.reserved_queues,
		       qdev->init_params.num_syncqs);
}

PP_DEFINE_DEBUGFS(destroy_tree, pp_qos_dbg_tree_remove, NULL);

/**
 * @brief dump complete qos tree
 */
void pp_qos_dbg_qm_stat_show(struct seq_file *s)
{
	struct pp_qos_dev *qdev;

	qdev = pp_qos_dev_open((unsigned long)s->private);
	if (unlikely(!qdev)) {
		seq_puts(s, "qdev Null\n");
		return;
	}

	if (unlikely(!qos_device_ready(qdev))) {
		seq_puts(s, "Device is not ready\n");
		return;
	}

	qos_dbg_qm_stat_show(qdev, s);
}

/**
 * @brief reset queues statistics
 */
void pp_qos_dbg_qm_stat_reset(char *cmd_buf, void *data)
{
	struct pp_qos_dev *qdev;
	u32 is_reset;

	if ((sscanf(cmd_buf, "%u ", &is_reset) == 1) && (is_reset == 0)) {
		qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
		if (unlikely(ptr_is_null(qdev)))
			return;
		pp_qos_stats_reset(qdev);
		return;
	}

	QOS_LOG_INFO("\npp/qos0/qstat help:\n");
	QOS_LOG_INFO("cat pp/qos0/qstat      : Display statistics\n");
	QOS_LOG_INFO("echo 0 > pp/qos0/qstat : Reset statistics\n");
}

PP_DEFINE_DEBUGFS(qm_stats, pp_qos_dbg_qm_stat_show, pp_qos_dbg_qm_stat_reset);

/**
 * @brief dump general qos info
 */
void pp_qos_dbg_gen_show(struct seq_file *s)
{
	struct qos_drv_stats stats;
	struct pp_qos_dev *qdev;
	u32 major, minor, build;

	qdev = pp_qos_dev_open((unsigned long)s->private);
	if (unlikely(ptr_is_null(qdev)))
		return;

	if (unlikely(!qos_device_ready(qdev))) {
		seq_puts(s, "Device is not ready\n");
		return;
	}

	if (unlikely(qos_drv_stats_get(qdev, &stats))) {
		seq_puts(s, "failed to get driver stats\n");
		return;
	}

	seq_printf(s, "Driver version: %s\n", PPV4_QOS_DRV_VER);

	if (pp_qos_get_fw_version(qdev, &major, &minor, &build) == 0) {
		seq_printf(s, "FW version:\tmajor(%u) minor(%u) build(%u)\n",
			   major, minor, build);
	} else {
		seq_puts(s, "Could not obtain FW version\n");
	}

	seq_printf(s, "Used nodes:\t%u\n", stats.active_nodes);
	seq_printf(s, "Ports:\t\t%u\n", stats.active_ports);
	seq_printf(s, "Scheds:\t\t%u\n", stats.active_sched);
	seq_printf(s, "Queues:\t\t%u\n", stats.active_queues);
	seq_printf(s, "Reserved Queues:\t%u\n", stats.active_reserved_queues);
	seq_printf(s, "Internals:\t%u\n", stats.active_internals);

	seq_printf(s, "Command queue watermark:\n\tcmd_q: %u\n\tpend_q: %u\n",
		   (u32)cmd_queue_watermark_get(qdev->drvcmds.cmdq),
		   (u32)cmd_queue_watermark_get(qdev->drvcmds.pendq));
	seq_printf(s, "WRED resources:\t%u\n",
		   qdev->init_params.wred_total_avail_resources);
	seq_printf(s, "Num fast queues:\t%u\n",
		   qdev->num_fast_queues);
	seq_printf(s, "AQM engine:\t%u (%s)\n",
		   qdev->init_params.aqm_engine,
		   (qdev->init_params.aqm_engine == PP_AQM_SW
			? "PP_AQM_SW"
			: "PP_AQM_HW"));
}

PP_DEFINE_DEBUGFS(gen_show, pp_qos_dbg_gen_show, NULL);

#ifndef PP_QOS_DISABLE_CMDQ
/**
 * @brief send custom command to FW
 */
void dbg_cmd_write(char *cmd_buf, void *data)
{
	struct pp_qos_dev *qdev;
	unsigned long dst;
	u32 cmd_idx;
	u32 cmd_type;
	u32 cmd_flags;
	u32 cmd_len;
	u32 cmd_params[5];

	qdev = pp_qos_dev_open((unsigned long)data);
	dst = qdev->init_params.fwcom.cmdbuf;

	if (sscanf(cmd_buf, "%u %u %u",
		   &cmd_type, &cmd_flags, &cmd_len) != 3) {
		QOS_LOG_ERR("sscanf err - minimum 3 params\n");
		return;
	}

	PP_REG_WR32_INC(dst, cmd_type);
	QOS_LOG_INFO("Wrote 0x%x\n", cmd_type);
	PP_REG_WR32_INC(dst, cmd_flags);
	QOS_LOG_INFO("Wrote 0x%x\n", cmd_flags);
	PP_REG_WR32_INC(dst, cmd_len);
	QOS_LOG_INFO("Wrote 0x%x\n", cmd_len);

	switch (cmd_len - 3) {
	case 0:
	break;
	case 1:
		if (sscanf(cmd_buf, "%u %u %u %u",
			   &cmd_type,
			   &cmd_flags,
			   &cmd_len,
			   &cmd_params[0]) != 4) {
			QOS_LOG_ERR("sscanf err\n");
			return;
		}
	break;
	case 2:
		if (sscanf(cmd_buf, "%u %u %u %u %u",
			   &cmd_type,
			   &cmd_flags,
			   &cmd_len,
			   &cmd_params[0],
			   &cmd_params[1]) != 5) {
			QOS_LOG_ERR("sscanf err\n");
			return;
		}
	break;
	case 3:
		if (sscanf(cmd_buf, "%u %u %u %u %u %u",
			   &cmd_type,
			   &cmd_flags,
			   &cmd_len,
			   &cmd_params[0],
			   &cmd_params[1],
			   &cmd_params[2]) != 6) {
			QOS_LOG_ERR("sscanf err\n");
			return;
		}
	break;
	case 4:
		if (sscanf(cmd_buf, "%u %u %u %u %u %u %u",
			   &cmd_type,
			   &cmd_flags,
			   &cmd_len,
			   &cmd_params[0],
			   &cmd_params[1],
			   &cmd_params[2],
			   &cmd_params[3]) != 7) {
			QOS_LOG_ERR("sscanf err\n");
			return;
		}
	break;
	case 5:
		if (sscanf(cmd_buf, "%u %u %u %u %u %u %u %u",
			   &cmd_type,
			   &cmd_flags,
			   &cmd_len,
			   &cmd_params[0],
			   &cmd_params[1],
			   &cmd_params[2],
			   &cmd_params[3],
			   &cmd_params[4]) != 8) {
			QOS_LOG_ERR("sscanf err\n");
			return;
		}
	break;
	default:
		QOS_LOG_INFO("len %d is not supported (range is 3 - 8)\n",
			     cmd_len);
		return;
	}

	if ((cmd_len - 3) > 5) {
		QOS_LOG_INFO("len %d is not supported (range is 3 - 8)\n",
			     cmd_len);
		return;
	}

	for (cmd_idx = 0; cmd_idx < cmd_len - 3; cmd_idx++) {
		PP_REG_WR32_INC(dst, cmd_params[cmd_idx]);
		QOS_LOG_INFO("Wrote 0x%x\n", cmd_params[cmd_idx]);
	}

	signal_uc(qdev);
}

PP_DEFINE_DEBUGFS(cmd, NULL, dbg_cmd_write);
#endif

/**
 * @brief advanced debug features
 */
void ctrl_set(char *cmd_buf, void *data)
{
	s32    ret;
	u32    choise;
	struct pp_qos_dev *qdev;

	qdev = pp_qos_dev_open((unsigned long)data);
	if (unlikely(!qdev)) {
		QOS_LOG_ERR("Error: qdev NULL\n");
		return;
	}

	if (kstrtou32(cmd_buf, 10, &choise)) {
		QOS_LOG_ERR("kstrtou32 err\n");
		goto ctrl_set_done;
	}

	switch (choise) {
	case 1:
		QOS_LOG_INFO("loading FW\n");
		ret = load_qos_firmware(qdev, QOS_FIRMWARE_FILE);
		if (ret) {
			QOS_LOG_ERR("load_qos_firmware failed (%d)\n", ret);
			goto ctrl_set_done;
		}
	break;

	case 2:
#ifndef PP_QOS_DISABLE_CMDQ
		QOS_LOG_INFO("Sending init logger cmd (Info)\n");
		create_init_logger_cmd(qdev,
				       UC_LOGGER_LEVEL_INFO,
				       UC_LOGGER_MODE_WRITE_HOST_MEM);
		update_cmd_id(&qdev->drvcmds);
		ret = transmit_cmds(qdev);
		if (ret)
			QOS_LOG_ERR("transmit_cmds failed (%d)\n", ret);
#endif
		break;
	case 3:
#ifndef PP_QOS_DISABLE_CMDQ
		QOS_LOG_INFO("Sending init logger cmd (Debug)\n");
		create_init_logger_cmd(qdev,
				       UC_LOGGER_LEVEL_DEBUG,
				       UC_LOGGER_MODE_WRITE_HOST_MEM);
		update_cmd_id(&qdev->drvcmds);
		ret = transmit_cmds(qdev);
		if (ret)
			QOS_LOG_ERR("transmit_cmds failed (%d)\n", ret);
#endif
		break;
	case 4:
#ifndef PP_QOS_DISABLE_CMDQ
		QOS_LOG_INFO("Sending init logger cmd (Off)\n");
		create_init_logger_cmd(qdev,
				       UC_LOGGER_LEVEL_INFO,
				       UC_LOGGER_MODE_NONE);
		update_cmd_id(&qdev->drvcmds);
		ret = transmit_cmds(qdev);
		if (ret)
			QOS_LOG_ERR("transmit_cmds failed (%d)\n", ret);
#endif
		break;
	case 5:
		QOS_LOG_INFO("printing logger\n");
		print_fw_log(qdev);
	break;

	case 6:
		QOS_LOG_INFO("Sending init qos cmd\n");
		create_init_qos_cmd(qdev);
		update_cmd_id(&qdev->drvcmds);
		ret = transmit_cmds(qdev);
		if (ret)
			QOS_LOG_ERR("transmit_cmds failed (%d)\n", ret);
	break;

	case 7:
		check_sync_with_fw(qdev, true);
	break;

	default:
		QOS_LOG_INFO("unknown option\n");
		break;
	}

ctrl_set_done:
	return;
}

void ctrl_set_help(struct seq_file *f)
{
	seq_puts(f, "1: Load Firmware\n");
	seq_puts(f, "2: Set FW Logger (Info)\n");
	seq_puts(f, "3: Set FW Logger (Debug)\n");
	seq_puts(f, "4: Set FW Logger (Off)\n");
	seq_puts(f, "5: Print FW Logger\n");
	seq_puts(f, "6: Init QoS\n");
	seq_puts(f, "7: Check Sync With FW\n");
}

PP_DEFINE_DEBUGFS(ctrl, ctrl_set_help, ctrl_set);

/**
 * @brief read hw table entry (will be dumped to logger)
 */
void read_table_entry(char *cmd_buf, void *data)
{
	struct pp_qos_dev *qdev;
	u32 physical_id;
	u32 table_id;
	s32 ret;

	qdev = pp_qos_dev_open((unsigned long)data);
	if (unlikely(!qdev)) {
		QOS_LOG_ERR("Error: qdev NULL\n");
		return;
	}

	if (sscanf(cmd_buf, "%u %u",
		   &physical_id, &table_id) != 2) {
		QOS_LOG_ERR("sscanf err\n");
		goto read_table_entry_done;
	}

#ifndef PP_QOS_DISABLE_CMDQ
	ret = qos_table_entry_get(qdev, table_id, physical_id);
	if (ret) {
		QOS_LOG_ERR("failed to get table entry %d phy %d\n",
				table_id, physical_id);
		return;
	}
#endif

read_table_entry_done:
	return;
}

void read_table_entry_help(struct seq_file *f)
{
	seq_puts(f, "<physical node/rlm> <table id>\n");
	seq_puts(f, "<table-id NODE(0)/BR(1)/BWL(2)/RLM(5)SBWL(6)/>\n");
}

PP_DEFINE_DEBUGFS(table_entry, read_table_entry_help, read_table_entry);

/**
 * @brief push descriptor to qos queue
 */
void queue_push(char *cmd_buf, void *data)
{
	struct pp_qos_dev *qdev;
	unsigned long addr;
	u32 logical_id, size, color, policy, pool, port, data_off;

	qdev = pp_qos_dev_open((unsigned long)data);
	if (unlikely(!qdev)) {
		QOS_LOG_ERR("Error: qdev NULL\n");
		return;
	}

	if (sscanf(cmd_buf, "%u %u %u %u %u %u %u %lx",
		   &logical_id,
		   &size,
		   &pool,
		   &policy,
		   &color,
		   &port,
		   &data_off,
		   &addr) != 8) {
		QOS_LOG_ERR("sscanf err\n");
		goto queue_push_done;
	}

	qos_descriptor_push(qdev, logical_id, size, pool, policy, color, port,
			    data_off, addr);

queue_push_done:
	return;
}

void queue_push_help(struct seq_file *f)
{
	seq_puts(f, "<q logic id> <size> <pool> <policy>"
		    " <color> <tx port> <data off> <addr>\n");
}

PP_DEFINE_DEBUGFS(queue_push, queue_push_help, queue_push);

/**
 * @brief pop descriptor from qos queue
 */
void queue_pop(char *cmd_buf, void *data)
{
	struct pp_qos_dev *qdev;
#ifdef CONFIG_SOC_LGM
	struct pp_desc desc;
#endif
	u32 descriptor[4];
	u32 logical_id;

	qdev = pp_qos_dev_open((unsigned long)data);
	if (unlikely(!qdev)) {
		QOS_LOG_ERR("Error: qdev NULL\n");
		return;
	}

	if (sscanf(cmd_buf, "%u",
		   &logical_id) != 1) {
		QOS_LOG_ERR("sscanf err\n");
		return;
	}

	qos_descriptor_pop(qdev, logical_id, descriptor);

	QOS_LOG_INFO("descriptor[0]: %#x\n", descriptor[0]);
	QOS_LOG_INFO("descriptor[1]: %#x\n", descriptor[1]);
	QOS_LOG_INFO("descriptor[2]: %#x\n", descriptor[2]);
	QOS_LOG_INFO("descriptor[3]: %#x\n", descriptor[3]);

#ifdef CONFIG_SOC_LGM
	pp_desc_top_decode(&desc, (struct pp_hw_desc *)&descriptor[0]);
	pp_desc_top_dump(&desc);
#endif
}

void queue_pop_help(struct seq_file *f)
{
	seq_puts(f, "<queue logical id>\n");
}

PP_DEFINE_DEBUGFS(queue_pop, queue_pop_help, queue_pop);

/**
 * @brief mcmda copy
 */
void mcdma_copy(char *cmd_buf, void *data)
{
	struct pp_qos_dev *qdev;
	u32 src, dst, size;
	s32 ret;

	qdev = pp_qos_dev_open((unsigned long)data);
	if (unlikely(!qdev)) {
		QOS_LOG_ERR("Error: qdev NULL\n");
		return;
	}

	if (sscanf(cmd_buf, "%x %x %u",
		   &src, &dst, &size) != 3) {
		QOS_LOG_ERR("sscanf err\n");
		goto mcdma_copy_done;
	}

	create_mcdma_copy_cmd(qdev, src, dst, size);
	update_cmd_id(&qdev->drvcmds);
	ret = transmit_cmds(qdev);
	if (unlikely(ret))
		QOS_LOG_ERR("transmit_cmds failed (%d)\n", ret);

mcdma_copy_done:
	return;
}

void mcdma_copy_help(struct seq_file *f)
{
	seq_puts(f, "<src address> <dst address> <size>"
		    "(addresses should be ddr contiguous)\n");
}

PP_DEFINE_DEBUGFS(mcdma_copy, mcdma_copy_help, mcdma_copy);

#ifndef CONFIG_SOC_LGM
/**
 * @brief read the ewsp cfg from cmd_buf
 * @return 0 on success, error code otherwise
 */
static s32 parse_ewsp_cfg(char *cmd_buf, struct wsp_helper_cfg *cfg)
{
	if (sscanf(cmd_buf, "%u %u %u", &cfg->enable_prx, &cfg->timeout,
		   &cfg->byte_threshold_prx) != 3) {
		QOS_LOG_ERR("sscanf err\n");
		return -EINVAL;
	}
	return 0;
}
#else
/**
 * @brief read the ewsp cfg from cmd_buf
 * @return 0 on success, error code otherwise
 */
static s32 parse_ewsp_cfg(char *buf, struct wsp_helper_cfg *cfg)
{
	if (sscanf(buf, "%u", &cfg->timeout) != 1) {
		QOS_LOG_ERR("sscanf err\n");
		return -EINVAL;
	}

	return 0;
}
#endif

/**
 * @brief wsp helper, set enhanced WSP parameters
 */
void ewsp_helper(char *cmd_buf, void *data)
{
	struct pp_qos_dev *qdev;
	struct wsp_helper_cfg cfg = { 0 };
	s32 ret;

	qdev = pp_qos_dev_open((unsigned long)data);
	if (unlikely(!qdev)) {
		QOS_LOG_ERR("Error: qdev NULL\n");
		return;
	}
	if (parse_ewsp_cfg(cmd_buf, &cfg))
		return;

	memcpy(&qdev->ewsp_db.cfg, &cfg, sizeof(cfg));
	create_set_wsp_helper_cmd(qdev, &cfg);
	update_cmd_id(&qdev->drvcmds);
	ret = transmit_cmds(qdev);
	if (unlikely(ret))
		QOS_LOG_ERR("transmit_cmds failed (%d)\n", ret);
}

void ewsp_helper_help(struct seq_file *f)
{
#ifndef CONFIG_SOC_LGM
	seq_puts(f, "<enable> <microseconds timeout> <byte threshold>\n");
#else
	seq_puts(f, "<intervals delay>\n");
#endif
}

PP_DEFINE_DEBUGFS(ewsp_helper, ewsp_helper_help, ewsp_helper);

#ifndef CONFIG_SOC_LGM
/**
 * @brief wsp helper reset, reset Enhanced WSP statistics
 */
void ewsp_helper_reset(char *cmd_buf, void *data)
{
	struct pp_qos_dev *qdev;
	struct wsp_stats_t stats;
	s32 ret;

	qdev = pp_qos_dev_open((unsigned long)data);
	if (unlikely(!qdev)) {
		QOS_LOG_ERR("Error: qdev NULL\n");
		return;
	}

	create_get_wsp_helper_stats_cmd(qdev, true, &stats);
	update_cmd_id(&qdev->drvcmds);
	ret = transmit_cmds(qdev);
	if (unlikely(ret))
		QOS_LOG_ERR("transmit_cmds failed (%d)\n", ret);

	return;
}
/**
 * @brief wsp helper stats, dump Enhanced WSP statistics
 */
void ewsp_helper_stats(struct seq_file *f)
{
	struct pp_qos_dev *qdev;
	struct wsp_stats_t stats;
	s32 ret;

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(!qdev)) {
		QOS_LOG_ERR("Error: qdev NULL\n");
		return;
	}

	create_get_wsp_helper_stats_cmd(qdev, false, &stats);
	update_cmd_id(&qdev->drvcmds);
	ret = transmit_cmds(qdev);
	if (unlikely(ret))
		QOS_LOG_ERR("transmit_cmds failed (%d)\n", ret);

	seq_puts(f, "Wsp helper cfg:\n");
	seq_puts(f, "===========================\n");
	seq_printf(f, "enable: %u\n", qdev->ewsp_db.cfg.enable_prx);
	seq_printf(f, "timeout_microseconds: %u\n", qdev->ewsp_db.cfg.timeout);
	seq_printf(f, "byte_threshold: %u\n",
		   qdev->ewsp_db.cfg.byte_threshold_prx);

	seq_puts(f, "\n--------------------------\n");

	seq_puts(f, "Wsp helper stats:\n");
	seq_puts(f, "===========================\n");
	seq_printf(f, "num_queues: %u\n", qdev->ewsp_db.num_queues);
	seq_printf(f, "num_timeouts: %u\n", stats.num_timeouts);
	seq_printf(f, "num_toggles: %u\n", stats.num_toggles_wm);
	seq_printf(f, "num_toggles_wm: %u\n", stats.num_toggles_wm);
	seq_printf(f, "num_iterations: %u\n", stats.num_iterations_wm);
	seq_printf(f, "num_iterations_wm: %u\n", stats.num_iterations_wm);
}
PP_DEFINE_DEBUGFS(ewsp_helper_stats, ewsp_helper_stats, ewsp_helper_reset);
#else
/**
 * @brief wsp helper stats, dump Enhanced WSP statistics
 */
void ewsp_helper_stats(struct seq_file *f)
{
	struct pp_qos_dev *qdev;

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(!qdev)) {
		QOS_LOG_ERR("Error: qdev NULL\n");
		return;
	}

	seq_puts(f, "Wsp helper cfg:\n");
	seq_puts(f, "===========================\n");
	seq_printf(f, "timeout delay: %u\n", qdev->ewsp_db.cfg.timeout);

	seq_puts(f, "\n--------------------------\n");

	seq_puts(f, "Wsp helper stats:\n");
	seq_puts(f, "===========================\n");
	seq_printf(f, "num_queues: %u\n", qdev->ewsp_db.num_queues);
}
PP_DEFINE_DEBUGFS(ewsp_helper_stats, ewsp_helper_stats, NULL);
#endif


/**
 * @brief Set Qos modules registers write logs on/off
 */
static s32 mod_reg_log_en_set(void *data, u64 val)
{
	struct pp_qos_dev *qdev;
	s32 ret;

	qdev = pp_qos_dev_open((unsigned long)data);
	if (ptr_is_null(qdev))
		return -EINVAL;

	create_mod_log_bmap_set_cmd(qdev, (u32)val);
	update_cmd_id(&qdev->drvcmds);
	ret = transmit_cmds(qdev);
	if (unlikely(ret))
		QOS_LOG_ERR("transmit_cmds failed (%d)\n", ret);

	return ret;
}

/**
 * @brief Get Qos modules registers write logs status
 */
static s32 mod_reg_log_en_get(void *data, u64 *val)
{
	u32 bmap;
	struct pp_qos_dev *qdev;
	s32 ret;

	qdev = pp_qos_dev_open((unsigned long)data);
	if (ptr_is_null(qdev))
		return -EINVAL;

	create_mod_log_bmap_get_cmd(qdev, &bmap);
	update_cmd_id(&qdev->drvcmds);
	ret = transmit_cmds(qdev);
	if (unlikely(ret))
		QOS_LOG_ERR("transmit_cmds failed (%d)\n", ret);
	*val = (u64)bmap;

	return ret;
}

PP_DEFINE_DBGFS_ATT_FMT(mod_reg_log_en, mod_reg_log_en_get, mod_reg_log_en_set,
			"%llx\n");

/**
 * @brief configure CoDel
 */
void codel_cfg(char *cmd_buf, void *data)
{
	struct pp_qos_dev *qdev;
	struct pp_qos_codel_cfg cfg;
	s32 ret;

	qdev = pp_qos_dev_open((unsigned long)data);
	if (unlikely(!qdev)) {
		QOS_LOG_ERR("Error: qdev NULL\n");
		return;
	}

	if (unlikely(!qos_device_ready(qdev))) {
		QOS_LOG_ERR("Device is not ready\n");
		return;
	}

	if (sscanf(cmd_buf, "%u %u",
		   &cfg.target_delay_msec,
		   &cfg.interval_time_msec) != 2) {
		QOS_LOG_ERR("sscanf err - 2 params\n");
		goto done;
	}

	ret = pp_qos_codel_cfg_set(qdev, &cfg);
	if (unlikely(ret)) {
		QOS_LOG_ERR("pp_qos_codel_cfg_set failed\n");
		goto done;
	}

done:
	return;
}

/**
 * @brief print configure CoDel
 */
static void codel_cfg_show(struct seq_file *f)
{
	struct pp_qos_dev *qdev;
	struct pp_qos_codel_cfg cfg;
	s32 ret;

	qdev = pp_qos_dev_open((unsigned long)f->private);
	if (unlikely(!qdev)) {
		QOS_LOG_ERR("Error: qdev NULL\n");
		return;
	}

	if (unlikely(!qos_device_ready(qdev))) {
		QOS_LOG_ERR("Device is not ready\n");
		return;
	}

	ret = pp_qos_codel_cfg_get(qdev, &cfg);
		if (unlikely(ret)) {
			QOS_LOG_ERR("pp_qos_codel_cfg_get failed\n");
			return;
		}

	seq_printf(f, "target_delay_msec         = %u\n", cfg.target_delay_msec);
	seq_printf(f, "interval_time_msec        = %u\n\n", cfg.interval_time_msec);

	seq_puts(f, "echo <target_delay_msec>"
		" <interval_time_msec>  > qos0/codel_cfg\n");
}

PP_DEFINE_DEBUGFS(codel_cfg, codel_cfg_show, codel_cfg);

/**
 * @brief configure CoDel
 */
void codel_stats(char *cmd_buf, void *data)
{
	const struct qos_node *node;
	struct pp_qos_dev *qdev;
	struct pp_qos_codel_stat cstats;
	u32 node_id, node_phy;

	qdev = pp_qos_dev_open((unsigned long)data);
	if (unlikely(!qdev)) {
		QOS_LOG_ERR("Error: qdev NULL\n");
		return;
	}

	if (unlikely(!qos_device_ready(qdev))) {
		QOS_LOG_ERR("Device is not ready\n");
		return;
	}

	memset(&cstats, 0, sizeof(cstats));
	if (sscanf(cmd_buf, "%u %d", &node_id, &cstats.reset) != 2) {
		QOS_LOG_ERR("sscanf err - 2 params\n");
		goto done;
	}

	node_phy = get_phy_from_id(qdev->mapping, node_id);
	if (unlikely(!QOS_PHY_VALID(node_phy))) {
		QOS_LOG_ERR("Invalid id %u\n", node_id);
		return;
	}

	node = get_node_from_phy(qdev->nodes, node_phy);
	if (!node_queue(node)) {
		QOS_LOG_ERR("not a queue node, node %u\n", node_id);
		return;
	}

	if (unlikely(pp_qos_codel_queue_stat_get(qdev, node_id, &cstats))) {
		QOS_LOG_ERR("codel_stat_get(%u) failed\n", node_id);
		return;
	}

	QOS_LOG_INFO(" Dropped packets            : %u\n",
		     cstats.packets_dropped);
	QOS_LOG_INFO(" Dropped bytes              : %u\n",
		     cstats.bytes_dropped);
	QOS_LOG_INFO(" Max sojourn time   [mSec]  : %u\n",
		     cstats.max_sojourn_time);
	QOS_LOG_INFO(" Min sojourn time   [mSec]  : %u\n",
		     cstats.min_sojourn_time);
	QOS_LOG_INFO(" Total sojourn time [mSec]  : %u\n",
		     cstats.total_packets);
	QOS_LOG_INFO(" Max packet size            : %u\n",
		     cstats.max_packet_size);
	QOS_LOG_INFO(" Total packets              : %u\n",
		     cstats.total_packets);

done:
	return;
}

void codel_stats_help(struct seq_file *f)
{
	seq_puts(f, "<queue node id> <1=reset stats>\n");
}

PP_DEFINE_DEBUGFS(codel_stats, codel_stats_help, codel_stats);

static int __gnode_set(void *data, u64 val)
{
	g_node = (u16)val;

	return 0;
}

static int __gnode_get(void *data, u64 *val)
{
	*val = (u64)g_node;

	return 0;
}

PP_DEFINE_DBGFS_ATT(gnode, __gnode_get, __gnode_set);

void pp_qos_dbg_quanta_get(struct seq_file *s)
{
	struct pp_qos_dev *qdev;
	u32 quanta;

	qdev = pp_qos_dev_open((unsigned long)s->private);
	if (unlikely(ptr_is_null(qdev)))
		return;

	if (unlikely(!qos_device_ready(qdev))) {
		seq_puts(s, "Device is not ready\n");
		return;
	}

	if (pp_qos_get_quanta(qdev, &quanta) == 0)
		seq_printf(s, "quanta: %u\n", quanta);
	else
		seq_puts(s, "pp_qos_get_quanta failed\n");
}

PP_DEFINE_DEBUGFS(quanta, pp_qos_dbg_quanta_get, NULL);

void pp_qos_dbg_system_stats_show(struct seq_file *s)
{
	struct pp_qos_dev *qdev;
	struct system_stats_s sys_stats = {0};
	s32 ret = 0;

	qdev = pp_qos_dev_open((unsigned long)s->private);
	if (unlikely(ptr_is_null(qdev)))
		return;

	if (unlikely(!qos_device_ready(qdev))) {
		seq_puts(s, "Device is not ready\n");
		return;
	}

	create_get_sys_info_cmd(qdev, &sys_stats, false);
	update_cmd_id(&qdev->drvcmds);
	ret = transmit_cmds(qdev);
	if (ret) {
		QOS_LOG_ERR("transmit_cmds failed (%d)\n", ret);
		return;
	}

	seq_puts(s, "System stats:\n");
	seq_puts(s, "=============\n");
	seq_printf(s, "tscd_infinite_loop_error_occurred: %u\n",
		   sys_stats.tscd_infinite_loop_error_occurred);
	seq_printf(s, "tscd_bwl_update_error_occurred: %u\n",
		   sys_stats.tscd_bwl_update_error_occurred);
	seq_printf(s, "tscd_quanta: %u\n", sys_stats.tscd_quanta);

#if IS_ENABLED(CONFIG_SOC_LGM)
	seq_printf(s, "wred_fake_pops: %u\n", sys_stats.wred_fake_pops);
	seq_printf(s, "wred_pop_underflow_count: %u\n",
			sys_stats.wred_pop_underflow_count);
	seq_printf(s, "wred_pop_underflow_sum: %u\n",
			sys_stats.wred_pop_underflow_sum);
	seq_printf(s, "wred_last_push_address_high: %#x\n",
			sys_stats.wred_last_push_address_high);
	seq_printf(s, "wred_last_push_address_low: %#x\n",
			sys_stats.wred_last_push_address_low);
	seq_printf(s, "wred_last_push_drop: %u\n",
			sys_stats.wred_last_push_drop);
	seq_printf(s, "wred_last_push_info_q_id: %u\n",
			sys_stats.wred_last_push_info_q_id);
	seq_printf(s, "wred_last_push_info_color: %u\n",
			sys_stats.wred_last_push_info_color);
	seq_printf(s, "wred_last_push_info_pkt_size: %u\n",
			sys_stats.wred_last_push_info_pkt_size);
	seq_printf(s, "wred_last_pop_info_q_id: %u\n",
			sys_stats.wred_last_pop_info_q_id);
	seq_printf(s, "wred_last_pop_info_fake: %u\n",
			sys_stats.wred_last_pop_info_fake);
	seq_printf(s, "wred_last_pop_info_color: %u\n",
			sys_stats.wred_last_pop_info_color);
	seq_printf(s, "wred_last_pop_info_pkt_size: %u\n",
			sys_stats.wred_last_pop_info_pkt_size);
#endif

	seq_printf(s, "txmgr_bp_status_ports_0_31: %#x\n",
		   sys_stats.txmgr_bp_status_ports_0_31);
	seq_printf(s, "txmgr_bp_status_ports_32_63: %#x\n",
		   sys_stats.txmgr_bp_status_ports_32_63);
	seq_printf(s, "txmgr_bp_status_ports_64_95: %#x\n",
		   sys_stats.txmgr_bp_status_ports_64_95);
	seq_printf(s, "txmgr_bp_status_ports_96_127: %#x\n",
		   sys_stats.txmgr_bp_status_ports_96_127);
#if IS_ENABLED(CONFIG_SOC_LGM)
	seq_printf(s, "txmgr_bp_status_ports_128_159: %#x\n",
		   sys_stats.txmgr_bp_status_ports_128_159);
	seq_printf(s, "txmgr_bp_status_ports_160_191: %#x\n",
		   sys_stats.txmgr_bp_status_ports_160_191);
	seq_printf(s, "txmgr_bp_status_ports_192_223: %#x\n",
		   sys_stats.txmgr_bp_status_ports_192_223);
	seq_printf(s, "txmgr_bp_status_ports_224_255: %#x\n",
		   sys_stats.txmgr_bp_status_ports_224_255);
	seq_printf(s, "dfs_irq_cnt: %u\n", sys_stats.dfs_irq_cnt);
	seq_printf(s, "dfs_false_irq_cnt: %u\n", sys_stats.dfs_false_irq_cnt);
#endif
}

PP_DEFINE_DEBUGFS(system_stats, pp_qos_dbg_system_stats_show, NULL);

/**
 * @brief Dump qos stats
 */
void qos_global_stats_show(struct seq_file *f)
{
	struct pp_qos_dev *qdev;
	struct pp_qos_stats stats;
	debug_counters_t dbg_stats;
	u32 i;
	u32 debug_cnt = 0;

	qdev = pp_qos_dev_open((unsigned long)f->private);
	if (ptr_is_null(qdev)) {
		seq_puts(f, "qos dev is not open\n");
		return;
	}
	if (!qos_device_ready(qdev)) {
		seq_puts(f, "Device is not ready\n");
		return;
	}

	if (pp_qos_stats_get(qdev, &stats)) {
		seq_puts(f, "failed to get qos's stats\n");
		return;
	}

	if (qos_get_debug_stats(qdev, &dbg_stats)) {
		seq_puts(f, "failed to get qos's stats\n");
		return;
	}

	seq_puts(f, " Qos global stats:\n");
	seq_puts(f, " =====================\n");
	seq_printf(f, "Rx            = %u\n", (u32)stats.pkts_rcvd);
	seq_printf(f, "Tx            = %u\n", (u32)stats.pkts_transmit);
	seq_printf(f, "Drop          = %u\n", (u32)stats.pkts_dropped);

	for (i = 0; i < 64 ; i++) {
		if (dbg_stats.debug[i]) {
			debug_cnt++;
			if (debug_cnt == 1) {
				seq_puts(f, " Qos debug stats:\n");
				seq_puts(f, " =====================\n");
			}
			seq_printf(f, "Debug %d           = %u\n", i,
				   (u32)dbg_stats.debug[i]);
		}
	}
}

PP_DEFINE_DEBUGFS(qos_global, qos_global_stats_show, NULL);

extern void create_get_version_cmd(const struct pp_qos_dev *qdev,
					struct fw_cmd_get_version *ver);
void qos_version(struct seq_file *f)
{
	struct pp_qos_dev *qdev;
	struct fw_cmd_get_version ver = {0};
	s32 ret;

	qdev = pp_qos_dev_open((unsigned long)f->private);
	if (ptr_is_null(qdev))
		return;
	create_get_version_cmd(qdev, &ver);
	update_cmd_id(&qdev->drvcmds);
	ret = transmit_cmds(qdev);
	if (unlikely(ret))
		QOS_LOG_ERR("transmit_cmds failed (%d)\n", ret);
	else
		seq_printf(f, "QOS     : MAJOR[%d], MINOR[%d], BUILD[%d]\n",
				ver.major, ver.minor, ver.build);
	return;
}

PP_DEFINE_DEBUGFS(qos_ver, qos_version, NULL);

void aqm_cfg_show(struct seq_file *f)
{
	wred_aqm_db_t *aqm_db;
	struct pp_qos_dev *qdev;
	struct aqm_dbgfs_db *db = f->private;
	wred_aqm_conf_t *conf;

	if (ptr_is_null(db))
		return;

	qdev = pp_qos_dev_open((unsigned long)db->qdev_id);
	if (ptr_is_null(qdev))
		return;

	aqm_db = kzalloc(sizeof(*aqm_db), GFP_KERNEL);
	if (!aqm_db)
		return;

	conf = &aqm_db->aqm_ctx[db->dbg_aqm_context_id].aqm_conf;
	qos_get_aqm_info(qdev, aqm_db);

	seq_printf(f, "AQM CFG \n");

	if (conf->enable) {
		seq_printf(f, "enable %d\n", conf->enable);
		seq_printf(f, "%-30s  %u\n", "buffer_size", conf->buffer_size);
		seq_printf(f, "%-30s  %u\n", "num_queues", conf->num_queues);
		seq_printf(f, "%-30s  %u\n", "llsf", conf->llsf);
		seq_printf(f, "%-30s  %u\n", "coupled_sf", conf->coupled_sf);
		seq_printf(f, "%-30s  %u\n", "num_hist_bins", conf->num_hist_bins);
		if (!conf->llsf) {
			seq_printf(f, "%-30s  %u\n", "msr", conf->msr);
			if (conf->coupled_sf == WRED_AQM_NUM_CONTEXTS) {
				seq_printf(f, "%-30s  %u\n", "peak_rate", conf->peak_rate);
				seq_printf(f, "CONTEXT %d is single AQM\n",
					   db->dbg_aqm_context_id);
			} else {
				seq_printf(f, "%-30s  %u\n", "msr_l", conf->msr_l);
				seq_printf(f, "%-30s  %u\n", "weight", conf->weight);
				seq_printf(f, "%-30s  %u\n", "coupling_factor",
					   conf->coupling_factor);
				seq_printf(f, "CONTEXT %d is classic AQM\n",
					   db->dbg_aqm_context_id);
			}
		} else {
			seq_printf(f, "CONTEXT %d is LLD\n", db->dbg_aqm_context_id);
		}
	} else {
		seq_printf(f, "CONTEXT %d is disabled\n",
				db->dbg_aqm_context_id);
	}

	kfree(aqm_db);
	return;
}

PP_DEFINE_DEBUGFS(aqm_cfg, aqm_cfg_show, NULL);

void aqm_enable_show(struct seq_file *f)
{
	wred_aqm_db_t *aqm_db;
	struct pp_qos_dev *qdev;
	struct aqm_dbgfs_db *db = f->private;

	if (ptr_is_null(db))
		return;

	qdev = pp_qos_dev_open((unsigned long)db->qdev_id);
	if (ptr_is_null(qdev))
		return;

	aqm_db = kzalloc(sizeof(*aqm_db), GFP_KERNEL);
	if (!aqm_db)
		return;

	qos_get_aqm_info(qdev, aqm_db);

	seq_printf(f, "AQM engine is %u (%s)\n",
		   aqm_db->aqm_engine,
		   aqm_db->aqm_engine == PP_AQM_SW ? "PP_AQM_SW" : "PP_AQM_HW");

	seq_printf(f, "ENABLE CONTEXTS BITMAP: 0x%x\n",
		   aqm_db->wred_aqm_enable_bitmap);

	kfree(aqm_db);
	return;
}

PP_DEFINE_DEBUGFS(aqm_enable, aqm_enable_show, NULL);

void aqm_dbg_show(struct seq_file *f)
{
	wred_aqm_db_t *aqm_db;
	struct pp_qos_dev *qdev;
	struct aqm_dbgfs_db *db = f->private;
	wred_aqm_dbg_t *aqm_dbg;

	if (ptr_is_null(db))
		return;

	qdev = pp_qos_dev_open((unsigned long)db->qdev_id);
	if (ptr_is_null(qdev))
		return;

	aqm_db = kzalloc(sizeof(*aqm_db), GFP_KERNEL);
	if (!aqm_db)
		return;

	qos_get_aqm_info(qdev, aqm_db);

	aqm_dbg = &aqm_db->aqm_ctx[db->dbg_aqm_context_id].aqm_dbg;
	if (aqm_db->aqm_ctx[db->dbg_aqm_context_id].aqm_conf.enable) {
		seq_printf(f, "AQM DBG INFO \n");
		seq_printf(f, "%-30s %d\n", "prev_queue_length",
				aqm_dbg->prev_queue_length);
		seq_printf(f, "%-30s %u\n", "prev_msrtokens",
				aqm_dbg->prev_msrtokens);
		seq_printf(f, "%-30s %u\n", "prev_burst_allowance",
				aqm_dbg->prev_burst_allowance);
		seq_printf(f, "%-30s %u\n", "prev_ctrl_path_cond",
				aqm_dbg->prev_ctrl_path_cond);
		seq_printf(f, "%-30s 0x%x\n", "prev_qdelay_status",
				aqm_dbg->prev_qdelay_status);
		seq_printf(f, "%-30s %u\n", "prev_burst_state",
				aqm_dbg->prev_burst_state);
		seq_printf(f, "%-30s %u\n", "prev_drop_pkt_cnt",
				aqm_dbg->prev_drop_pkt_cnt);
		seq_printf(f, "%-30s %u\n", "prev_drop_byte_cnt",
				aqm_dbg->prev_drop_byte_cnt);
		seq_printf(f, "%-30s %u\n", "prev_burst_allow_val",
				aqm_dbg->prev_burst_allow_val);
		seq_printf(f, "%-30s %u\n", "interrupt_count",
				aqm_dbg->interrupt_count);
	} else {
		seq_printf(f, "CONTEXT %d is disabled\n",
				db->dbg_aqm_context_id);
	}

	kfree(aqm_db);
	return;
}

PP_DEFINE_DEBUGFS(aqm_dbg, aqm_dbg_show, NULL);

void sfid_set(char *cmd_buf, void *data)
{
	struct aqm_dbgfs_db *db = data;

	if (ptr_is_null(db)) {
		pr_err("aqm db ptr null\n");
		return;
	}

	if (kstrtou32(cmd_buf, 10, &db->dbg_aqm_context_id))
		pr_info("failed to parse '%s'\n", cmd_buf);

	if (db->dbg_aqm_context_id >= WRED_AQM_NUM_CONTEXTS) {
		db->dbg_aqm_context_id = 0;
		pr_err("wrong value, context limited to %u\n",
		       WRED_AQM_NUM_CONTEXTS);
		return;
	}

	pr_err("context num : %u\n", db->dbg_aqm_context_id);
}

void sfid_show(struct seq_file *f)
{
	struct aqm_dbgfs_db *db = f->private;

	if (ptr_is_null(db))
		return;

	seq_printf(f, "context num : %u\n", db->dbg_aqm_context_id);
}

PP_DEFINE_DEBUGFS(aqm_sfid, sfid_show, sfid_set);

s32 pp_qos_aqm_sf_stats_get(void *_stats, u32 num_stats, void *data)
{
	struct pp_qos_dev *qdev;
	struct pp_qos_aqm_sf_stats *stats;
	wred_aqm_db_t *aqm_db;
	wred_aqm_ctx_t *ctx;
	s32 ret = 0;
	u32 sf;

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(ptr_is_null(qdev)))
		return -EINVAL;

	aqm_db = kzalloc(sizeof(*aqm_db), GFP_KERNEL);
	if (!aqm_db)
		return -EINVAL;

	ret = qos_get_aqm_info(qdev, aqm_db);
	stats = (struct pp_qos_aqm_sf_stats *)_stats;
	for (sf = 0; sf < num_stats; sf++) {
		ctx = &aqm_db->aqm_ctx[sf];
		stats[sf].occupancy = PP_REG_RD32
			(PP_QOS_WRED_AQM_OCCUPANCY_CNT_REG_IDX(sf));
		stats[sf].burst_state = PP_REG_RD32
			(PP_QOS_WRED_AQM_BURST_STATE_REG_IDX(sf));
		stats[sf].drop_prob = PP_REG_RD32
			(PP_QOS_WRED_AQM_DROP_PROB_REG_IDX(sf));
		stats[sf].hist_updates = ctx->hist_updates;
		stats[sf].max_latency = ctx->max_latency;
		stats[sf].last_total_accepts = ctx->last_total_accepts;
	}

	return ret;
}

s32 pp_qos_aqm_sf_stats_diff(void *pre, u32 num_pre, void *post,
				 u32 num_post, void *delta, u32 num_delta,
				 void *data)
{
	struct pp_qos_aqm_sf_stats *__pre, *__post, *__delta;
	u32 i;

	if (unlikely(ptr_is_null(pre) || ptr_is_null(post) ||
		     ptr_is_null(delta)))
		return -EINVAL;

	__pre   = pre;
	__post  = post;
	__delta = delta;
	for (i = 0; i < num_pre; i++)
		U64_STRUCT_DIFF(&__pre[i], &__post[i], &__delta[i]);

	return 0;
}

s32 pp_qos_aqm_sf_stats_show(char *buf, size_t sz, size_t *n, void *s,
				 u32 num, void *data)
{
	struct pp_qos_aqm_sf_stats *stats, *it;
	u32 sf;
	char **str;
	static const char *const cntrs_str[] = {
		"occupancy", "burst_state", "drop_prob",
		"hist_updates", "max_latency", "last_total_accepts"
	};

	pr_buf(buf, sz, *n, "\n");
	pr_buf_cat(buf, sz, *n,
			"|====================================================================================================|\n");
	pr_buf_cat(buf, sz, *n,
			"|                                         AQM QOS SF Statistics                                      |\n");
	pr_buf_cat(buf, sz, *n,
			"|====================================================================================================|\n");
	pr_buf_cat(buf, sz, *n,
			"| %-2s ", "SF");
	for_each_arr_entry(str, cntrs_str, ARRAY_SIZE(cntrs_str))
		pr_buf_cat(buf, sz, *n, "| %-12s ", *str);
	pr_buf_cat(buf, sz, *n, "|\n");
	pr_buf_cat(buf, sz, *n,
			"|----+--------------+--------------+--------------+--------------+--------------+--------------------|\n");

	stats = (struct pp_qos_aqm_sf_stats *)s;
	for (sf = 0; sf < num; sf++) {
		it = &stats[sf];
		pr_buf_cat(buf, sz, *n, "| %2u ", sf);
		pr_buf_cat(buf, sz, *n, "| %12u ", it->occupancy);
		pr_buf_cat(buf, sz, *n, "| %12u ", it->burst_state);
		pr_buf_cat(buf, sz, *n, "| %12u ", it->drop_prob);
		pr_buf_cat(buf, sz, *n, "| %12u ", it->hist_updates);
		pr_buf_cat(buf, sz, *n, "| %12u ", it->max_latency);
		pr_buf_cat(buf, sz, *n, "| %18u ", it->last_total_accepts);
		pr_buf_cat(buf, sz, *n, "|\n");
	}
	pr_buf_cat(buf, sz, *n,
			"|====================================================================================================|\n");
	pr_buf_cat(buf, sz, *n,
		   "NOTE: drop_prob value is fixed point (from HW), for "
		   "accurate value should be divided by %u\n",
		   1 << AQM_FRACTIONAL_BITS);
	return 0;
}

void pp_qos_dbg_aqm_sf_stats_show(struct seq_file *f)
{
	pp_stats_show_seq(f, sizeof(struct pp_qos_aqm_sf_stats),
			  PP_QOS_MAX_SERVICE_FLOWS,
			  pp_qos_aqm_sf_stats_get,
			  pp_qos_aqm_sf_stats_show, NULL);
}

PP_DEFINE_DEBUGFS(aqm_sf_stats, pp_qos_dbg_aqm_sf_stats_show, NULL);

void pp_qos_dbg_aqm_sf_pps_show(struct seq_file *f)
{
	pp_pps_show_seq(f, sizeof(struct pp_qos_aqm_sf_stats),
			PP_QOS_MAX_SERVICE_FLOWS,
			pp_qos_aqm_sf_stats_get,
			pp_qos_aqm_sf_stats_diff,
			pp_qos_aqm_sf_stats_show, NULL);
}

PP_DEFINE_DEBUGFS(aqm_sf_pps, pp_qos_dbg_aqm_sf_pps_show, NULL);

static struct debugfs_file qos_debugfs_files[] = {
	{"ver", &PP_DEBUGFS_FOPS(qos_ver)},
	{"node", &PP_DEBUGFS_FOPS(gnode)},
	{"nodeinfo", &PP_DEBUGFS_FOPS(node_show)},
	{"read_table_entry", &PP_DEBUGFS_FOPS(table_entry)},
	{"queues_stats", &PP_DEBUGFS_FOPS(queues_stats)},
	{"qstat", &PP_DEBUGFS_FOPS(qm_stats)},
	{"queues_pps", &PP_DEBUGFS_FOPS(queues_pps)},
	{"stat", &PP_DEBUGFS_FOPS(stats)},
	{"tree", &PP_DEBUGFS_FOPS(tree)},
	{"full_tree", &PP_DEBUGFS_FOPS(full_tree)},
	{"destroy_tree", &PP_DEBUGFS_FOPS(destroy_tree)},
	{"fw_logger", &dbg_fw_logger_fops},
	{"ctrl", &PP_DEBUGFS_FOPS(ctrl)},
	{"geninfo", &PP_DEBUGFS_FOPS(gen_show)},
	{"codel_cfg", &PP_DEBUGFS_FOPS(codel_cfg)},
	{"codel_stats", &PP_DEBUGFS_FOPS(codel_stats)},
	{"mcdma_copy", &PP_DEBUGFS_FOPS(mcdma_copy)},
	{"queue", &PP_DEBUGFS_FOPS(queue_set)},
	{"syncq", &PP_DEBUGFS_FOPS(syncq_add)},
	{"sched", &PP_DEBUGFS_FOPS(sched_set)},
	{"shared_grp_add", &PP_DEBUGFS_FOPS(shared_grp_add)},
	{"shared_grp_rem", &PP_DEBUGFS_FOPS(shared_grp_del)},
	{"port", &PP_DEBUGFS_FOPS(port_set)},
	{"push_desc", &PP_DEBUGFS_FOPS(queue_push)},
	{"pop_desc", &PP_DEBUGFS_FOPS(queue_pop)},
	{"allocate", &PP_DEBUGFS_FOPS(allocate)},
	{"remove", &PP_DEBUGFS_FOPS(remove)},
	{"flush", &PP_DEBUGFS_FOPS(flush)},
	{"quanta", &PP_DEBUGFS_FOPS(quanta)},
	{"mod_reg_log_en", &PP_DEBUGFS_FOPS(mod_reg_log_en)},
	{"ewsp_helper", &PP_DEBUGFS_FOPS(ewsp_helper)},
	{"ewsp_helper_stats", &PP_DEBUGFS_FOPS(ewsp_helper_stats)},
	{"system_stats", &PP_DEBUGFS_FOPS(system_stats)},
	{"global", &PP_DEBUGFS_FOPS(qos_global)},
#ifndef PP_QOS_DISABLE_CMDQ
	{"cmd", &PP_DEBUGFS_FOPS(cmd)},
#endif
};

static struct debugfs_file qos_root_debugfs_files[] = {
	{"qos_queues_stats", &PP_DEBUGFS_FOPS(queues_stats)},
	{"qos_queues_stats_bytes", &PP_DEBUGFS_FOPS(queues_stats_bytes)},
	{"qos_queues_pps", &PP_DEBUGFS_FOPS(queues_pps)},
	{"qos_queues_bps", &PP_DEBUGFS_FOPS(queues_bps)},
	{"qos_tree", &PP_DEBUGFS_FOPS(tree)},
};

static struct debugfs_file aqm_debugfs_files[] = {
	{"sfidx", &PP_DEBUGFS_FOPS(aqm_sfid)},
	{"enable", &PP_DEBUGFS_FOPS(aqm_enable)},
	{"cfg", &PP_DEBUGFS_FOPS(aqm_cfg)},
	{"dbg", &PP_DEBUGFS_FOPS(aqm_dbg)},
	{"sf_stats", &PP_DEBUGFS_FOPS(aqm_sf_stats)},
	{"sf_pps", &PP_DEBUGFS_FOPS(aqm_sf_pps)},
};

#define MAX_DIR_NAME 11
s32 qos_dbg_module_init(struct pp_qos_dev *qdev)
{
	char dirname[MAX_DIR_NAME];
	struct aqm_dbgfs_db *db;
	s32  ret = 0;

	QOS_LOG_DEBUG("\n");

	db = kzalloc(sizeof(*db), GFP_KERNEL);
	if (!db) {
		pr_err("Failed to allocate aqm debugfs memory of size %zu\n",
			sizeof(*db));
		return -ENOMEM;
	}

	snprintf(dirname, MAX_DIR_NAME, "qos%d", qdev->id);
	ret = pp_debugfs_create(qdev->init_params.dbgfs, dirname, &qdev->dbgfs,
				qos_debugfs_files,
				ARRAY_SIZE(qos_debugfs_files),
				(void *)(unsigned long)qdev->id);
	if (unlikely(ret))
		goto done;

	/* create subset in the root directory */
	ret = pp_debugfs_create(qdev->init_params.dbgfs, NULL, NULL,
				qos_root_debugfs_files,
				ARRAY_SIZE(qos_root_debugfs_files),
				(void *)(unsigned long)qdev->id);

	if (unlikely(ret))
		goto done;

	db->qdev_id = qdev->id;
	db->dbg_aqm_context_id = 0;

	/* Create main dps dir under port_mgr dir */
	ret = pp_debugfs_create(qdev->dbgfs, "aqm", NULL,
				aqm_debugfs_files,
				ARRAY_SIZE(aqm_debugfs_files), db);
	if (unlikely(ret)) {
		pr_err("Failed to create aqm debugfs dir\n");
		goto done;
	}

done:
	if (unlikely(ret)) {
		pr_err("Failed to create qos debugfs dir\n");
		kfree(db);
	}

	return ret;
}

void qos_dbg_module_clean(struct pp_qos_dev *qdev)
{
	QOS_LOG_DEBUG("qdev->dbgfs %p\n", qdev->dbgfs);
	debugfs_remove_recursive(qdev->dbgfs);
}


