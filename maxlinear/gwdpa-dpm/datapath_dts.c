// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2025, MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <net/datapath_api.h>
#include <net/datapath_proc_api.h>
#include "datapath.h"

DP_NO_OPTIMIZE_PUSH

#define DTS_PATH "dpm_qos"
struct qos_setting_match qos_setting_match[] = {
	/*cpu, eth-lan, eth_wan, wlan, dsl, direct, gpon, docsis default */
	{"cpu",           DP_F_CPU,                            0},
	{"eth_lan",       DP_F_FAST_ETH_LAN,                   1},
	{"eth_wan",       DP_F_FAST_ETH_WAN,                   2},
	{"wlan",          DP_F_FAST_WLAN | DP_F_FAST_WLAN_EXT, 3},
	{"dsl",           DP_F_FAST_DSL,                       4},
	{"direct",        DP_F_DIRECT,                         5},
	{"gpon",          DP_F_GPON,                           6},
	{"docsis",        DP_F_DOCSIS,                         7},
	{"vuni",          DP_F_VUNI,                           8},

	/*Note:
	  1) intermediate queues is for the path:
	           QOS->CQM DEQ->DMA->GSWIP/IPPU->PP->...
	  2) Here we assume all intermediate queues are created by DPM only,
	     not qos_tc, otherwise dp_node_link_add cannot properly detect
	     which qos category should be applied for queue->sched cause
	     since there is no QOS/CQM DEQ port information yet.
	  3) override method for qos_id value in DTS if really required
	     -----------------------------------------------------------------
	    | controller_id (8bits) | port_id: 8bits | channel_id: 16bits     |
	  4) here flag is zero to indidate intermediate queues
	 */
	{"default_inter", 0,                                   9},

	/* the last entry will be applied if no matching */
	{"default",      -1,                                   10}

};

#define MAX_QOS_CATAGORY     ((int)(ARRAY_SIZE(qos_setting_match)))
#define MAX_QOS_ID_PER_CATAGORY  13

struct dp_qos_setting dp_qos_cfg[DP_MAX_INST][MAX_QOS_CATAGORY][MAX_QOS_ID_PER_CATAGORY];
char dts_path_or_name[100] = DTS_PATH; /* for debugfs to dump some dts node */
static int dflt_system_idx = -1; /* system default index in dp_qos_cfg */
static int dflt_inter_idx = -1; /* intermediate default index in dp_qos_cfg */

#if IS_ENABLED(CONFIG_OF)

/* for intermediate queue qos_id override setting */
inline static bool match_qos_inter(struct qos_cqm_info *cqm_deq, u32 qos_id)
{
	struct qos_cqm_info *tmp = (struct qos_cqm_info *)&qos_id;

	return (tmp->base == cqm_deq->base);
}

static int match_keywords(int inst, struct dp_qos_setting *setting)
{
	int i, idx, j;
	u32 sub_idx = 0;

	for (i = 0; i < ARRAY_SIZE(qos_setting_match); i++) {
		if (dp_strncmpi(setting->category,
			        qos_setting_match[i].catagory,
			        strlen(qos_setting_match[i].catagory) + 1) == 0)
			break;
	}
	if (i >= ARRAY_SIZE(qos_setting_match)) {
		pr_err("categoray not valid: '%s' qos_id=%x ?\n",
		        setting->category, setting->qos_id);
		return -1;
	}
	idx = qos_setting_match[i].idx_catagory;
	sub_idx = setting->qos_id;
	if (qos_setting_match[i].flag != 0) { /* non-intermediate queue's qos */
		if (sub_idx >= MAX_QOS_ID_PER_CATAGORY) {
			pr_err("wrong qos_id=%d for non-intermediate queues\n",
			       sub_idx);
			return -1;
		}
		if (dp_qos_cfg[inst][idx][sub_idx].valid &&
		    qos_setting_match[i].flag != -1) {
			pr_err("DPM: dp_qos_cfg[%d][%d][%d] already set for category %s\n",
			       inst, idx, setting->qos_id,
			       setting->category);
			return -1;
		}
	} else if (sub_idx != 0) { /* for intermediate queue's qos:
				    * array size not big enough to
				    * directly use DMA_ID as index.
				    * So workaround here
				    */
		for (j = 1; j < MAX_QOS_ID_PER_CATAGORY; j++) {
			if (dp_qos_cfg[inst][idx][j].valid) {
				if (match_qos_inter(&dp_qos_cfg[inst][idx][j].cqm_deq,
						    setting->qos_id)) {
					pr_err("duplicated qos setting: %s %d\n",
						setting->category,
						setting->qos_id);
					return -1;
				}
			}
			else {
				break;
			}
		}
		if (j >= MAX_QOS_ID_PER_CATAGORY) {
			pr_err("no free entry to support more than %d entries\n",
			        MAX_QOS_ID_PER_CATAGORY);
			return -1;
		}
		sub_idx = j;
	}
	dp_qos_cfg[inst][idx][sub_idx].valid = true;
	dp_qos_cfg[inst][idx][sub_idx].flag = qos_setting_match[i].flag;
	dp_qos_cfg[inst][idx][sub_idx].node_name = setting->node_name;
	dp_qos_cfg[inst][idx][sub_idx].category = qos_setting_match[i].catagory;
	dp_qos_cfg[inst][idx][sub_idx].qos_id = setting->qos_id;
	dp_qos_cfg[inst][idx][sub_idx].wred_en = setting->wred_en;
	dp_qos_cfg[inst][idx][sub_idx].codel_en = setting->codel_en;
	dp_qos_cfg[inst][idx][sub_idx].qlen = setting->qlen;
	dp_qos_cfg[inst][idx][sub_idx].category_idx = idx;
	dp_qos_cfg[inst][idx][sub_idx].sub_category_idx = sub_idx;

	return 0;
}

int dp_datapath_dts_parse(void)
{
	struct device_node *node;
	struct device_node *child;
	const char *name;
	struct dp_qos_setting setting;

	node = of_find_node_by_name(NULL, DTS_PATH);
	if (!node) {
		pr_info("Not find node '%s'\n", DTS_PATH);
		return 0;
	}
	if (!of_device_is_available(node)) {
		pr_info("dts %s is disabled\n", DTS_PATH);
		return 0; /* the qos node is disabled */
	}

	for_each_child_of_node(node, child) {
		dp_memset(&setting, 0, sizeof(setting));
		name = child->name;
		setting.category = (void *)of_get_property(child, "category", NULL);
		if (!setting.category) {
			pr_err("not found category\n");
			continue;
		}
		if (!of_device_is_available(child))
			continue; /* not enabled */
		if (of_property_read_u32(child, "queue_size", &setting.qlen)) {
			pr_err("not found queue_size for %s node\n", node->name);
			continue;
		}
		of_property_read_u32(child, "qos_id", &setting.qos_id);
		of_property_read_u32(child, "wred", &setting.wred_en);
		of_property_read_u32(child, "codel", &setting.codel_en);
		setting.node_name = name;

		match_keywords(0, &setting);
	}
	pr_info("DPM DTS parsing done\n");
	return 0;
}

static void dump_dts_property(struct seq_file *s, const struct device_node *node,
			  const struct property *prop, int depth)
{
	const void *data;
	char prefix[64] = {0};
	bool printable = true;
	const char *str;
	const char *s1;
	const u8 *bytes;
	int len, i, num_zero = 0;

	dp_memset(prefix, ' ', depth * 2);
	prefix[depth * 2] = '\0';

	data = of_get_property(node, prop->name, &len);
	if (!data)
		return;
	if (dp_strncmpi(prop->name, "name", strlen("name") + 1) == 0)
		return; /* no need to print property name again */

	seq_printf(s, "%s%s = ", prefix, prop->name);

	// Detect empty string
	if (len == 1 && ((char *)data)[0] == '\0') {
		seq_printf(s, "%s \"\"\n", prefix);
		return;
	}

	// Detect zero integer
	if (len == 4 && *((__be32 *)data) == cpu_to_be32(0)) {
		seq_puts(s, "<0>\n");
		return;
	}

	// Detect printable string or string list
	str = data;
	for (i = 0; i < len; i++) {
		char c = str[i];
		if (c == '\0')
			num_zero++;
		if ((c < 32 || c > 126) && c != '\0')
			printable = false;
	}

	/* workaround: 0x20000000 may be regarded as printable.
	 * Need further check. Of course, still possible wrong detection
	 */
	if (printable && str[len - 1] == '\0') {
		if ((len == 4) && num_zero > 1) /* regard as integer */
			printable = false;
	}

	/* string or string list */
	if (printable && str[len - 1] == '\0') {
		/* string list */
		int first = 1;
		s1 = str;
		while ((s1 - str) < len) {
			if (first) {
				seq_printf(s, "\"%s\"", s1);
				first = 0;
			} else {
				seq_printf(s, ",\"%s\"", s1);
			}
			s1 += strlen(s1) + 1;
		}
		seq_puts(s, "\n");
		return;
	}

	/* Detect int array */
	if (len % 4 == 0) {
		const __be32 *arr = data;
		int count = len / 4;
		seq_puts(s, "<");
		for (i = 0; i < count; i++) {
			if (i == 0)
				seq_printf(s, "%d", be32_to_cpu(arr[i]));
			else
				seq_printf(s, " %d", be32_to_cpu(arr[i]));
		}
		seq_puts(s, ">\n");
		return;
	}

	// Otherwise treat as byte array
	bytes = data;
	seq_puts(s, "<");
	for (i = 0; i < len; i++) {
		if (i== 0)
			seq_printf(s, "%02x", bytes[i]);
		else
			seq_printf(s, " %02x", bytes[i]);
	}
	seq_puts(s, ">\n");
}

static void dump_dts_node_recursive(struct seq_file *s,
				const struct device_node *node, int depth)
{
	struct device_node *child;
	struct property *prop;
	char prefix[64] = {0};
	const char *label;

	dp_memset(prefix, ' ', depth * 2);
	prefix[depth * 2] = '\0';

	label = of_get_property(node, "label", NULL);
	if (label)
		seq_printf(s, "%s%s:%s {\n", prefix, label, node->name);
	else
		seq_printf(s, "%s%s {\n", prefix, node->name);

	for_each_property_of_node(node, prop)
		dump_dts_property(s, node, prop, depth + 1);

	for_each_child_of_node(node, child)
		dump_dts_node_recursive(s, child, depth + 1);
	seq_printf(s, "%s}\n", prefix);
}

void proc_qos_raw_dts_dump(struct seq_file *s)
{
	struct device_node *node;

	node = of_find_node_by_path(dts_path_or_name);
	if (!node) {
		node = of_find_node_by_name(NULL, dts_path_or_name);
		if (!node) {
			seq_printf(s, "Not find node or path: %s\n",
				   dts_path_or_name);
			return;
		}
	}
	seq_printf(s, "dts_path_or_name=%s\n", dts_path_or_name);
	dump_dts_node_recursive(s, node, 0);
}

ssize_t proc_dts_raw_write(struct file *file, const char *buf, size_t count,
			   loff_t *ppos)
{
	u16 len;
	char str[64];
	char *param_list[20 * 2];
	unsigned int num;

	len = (sizeof(str) > count) ? count : sizeof(str) - 1;
	len -= dp_copy_from_user(str, buf, len);
	str[len] = 0;
	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));
	if (!param_list[0] || (strlen(param_list[0]) <= 1))
		param_list[0] = "/";
	dp_strlcpy(dts_path_or_name, param_list[0], sizeof(dts_path_or_name));

	pr_info("dts_path_or_name is changed to %s\n", dts_path_or_name);
	return count;
}
#endif

int proc_qos_cfg_dump(struct seq_file *s, int pos)
{
	int inst = 0;
	int i;
	char *align= "  ";
	char flag_str[64];
	char qos_id_buf[20];

	if (!capable(CAP_SYS_PACCT))
		return -1;

	if (pos == 0)
		dp_sprintf(s, "%9s %20s %13s %24s %10s %4s %5s %s\n",
			   "index", "dts_node_name", "category", "qos_id(num/cqm_deq_base)", "queue_size",
			   "wred","codel", "flag");

	for (i = 0; i < MAX_QOS_ID_PER_CATAGORY; i++) {
		if (!dp_qos_cfg[inst][pos][i].valid)
			continue;
		if (!dp_qos_cfg[inst][pos][i].category)
			continue;
		if (i == 0)
			align = "";
		alloc_flag_str(dp_qos_cfg[inst][pos][i].flag,
			       flag_str, sizeof(flag_str));

		if ((dp_qos_cfg[inst][pos][i].flag == 0) &&
		    (dp_qos_cfg[inst][pos][i].qos_id != 0)) /* intemediate qos */
			sprintf(qos_id_buf, "0x%x(%d/%d)",
				dp_qos_cfg[inst][pos][i].qos_id,
				dp_qos_cfg[inst][pos][i].cqm_deq.num,
				dp_qos_cfg[inst][pos][i].cqm_deq.base);
		else
			sprintf(qos_id_buf, "0x%x",dp_qos_cfg[inst][pos][i].qos_id);

		if (i == 0) {
			if (dp_qos_cfg[inst][pos][i].flag == -1) /* system default */
				dp_sprintf(s, "%-2d/%-2d(**) %20s %13s %-24s %10u %4u %5u %s\n",
					pos, i,
					dp_qos_cfg[inst][pos][i].node_name,
					dp_qos_cfg[inst][pos][i].category,
					qos_id_buf,
					dp_qos_cfg[inst][pos][i].qlen,
					dp_qos_cfg[inst][pos][i].wred_en,
					dp_qos_cfg[inst][pos][i].codel_en,
					flag_str);
			else  /* default qos setting for each category */
				dp_sprintf(s, "%-2d/%-2d(*)  %20s %13s %-24s %10u %4u %5u %s\n",
					pos, i,
					dp_qos_cfg[inst][pos][i].node_name,
					dp_qos_cfg[inst][pos][i].category,
					qos_id_buf,
					dp_qos_cfg[inst][pos][i].qlen,
					dp_qos_cfg[inst][pos][i].wred_en,
					dp_qos_cfg[inst][pos][i].codel_en,
					flag_str);
		} else {
			dp_sprintf(s, "   %-2d     %20s %13s %-24s %10u %4u %5u %s\n",
				i,
				dp_qos_cfg[inst][pos][i].node_name,
				dp_qos_cfg[inst][pos][i].category,
				qos_id_buf,
				dp_qos_cfg[inst][pos][i].qlen,
				dp_qos_cfg[inst][pos][i].wred_en,
				dp_qos_cfg[inst][pos][i].codel_en,
				flag_str);
		}
	}

	if (pos == MAX_QOS_CATAGORY - 1) {
		dp_sprintf(s, "maximum number of supported category: %d\n",
			   MAX_QOS_CATAGORY);
		dp_sprintf(s, "maximum number of supported qos_id per category: %d\n",
			   MAX_QOS_ID_PER_CATAGORY);
	}

	if (!dp_seq_has_overflowed(s)) {
		pos++;
		if (pos >= MAX_QOS_CATAGORY)
			pos = -1;
	}
	return pos;
}

void proc_qos_category_dump(struct seq_file *s)
{
	char flag_str[64];
	int i;

	for (i = 0; i < ARRAY_SIZE(qos_setting_match); i++) {
		alloc_flag_str(qos_setting_match[i].flag,
			       flag_str, sizeof(flag_str));
		seq_printf(s, "[%d] catagory=%-13s idx=%d flag=%s\n",
			i,
			qos_setting_match[i].catagory,
			qos_setting_match[i].idx_catagory,
			flag_str);
	}
}

void init_qos_setting(void)
{
	int i;

	dp_memset(dp_qos_cfg, 0, sizeof(dp_qos_cfg));

	for (i = 0; i < ARRAY_SIZE(qos_setting_match); i++) {
		if ((qos_setting_match[i].flag == -1) || /* system default */
		    (qos_setting_match[i].flag == 0)) { /* intermediate qos */
			dp_qos_cfg[0][i][0].valid = true;
			dp_qos_cfg[0][i][0].category = qos_setting_match[i].catagory;
			dp_qos_cfg[0][i][0].qlen = 1024;
			dp_qos_cfg[0][i][0].wred_en = 0;
			dp_qos_cfg[0][i][0].codel_en = 0;

			if (qos_setting_match[i].flag == 0)
				dflt_inter_idx = i;
			else if (qos_setting_match[i].flag == -1)
				dflt_system_idx = i;
		}
	}
#if IS_ENABLED(CONFIG_OF)
	dp_datapath_dts_parse();
#endif
	if (dflt_inter_idx < 0)
		pr_err("dpm: wrong dflt_inter_idx: %d\n", dflt_inter_idx);
	if (dflt_system_idx < 0)
		pr_err("dpm: wrong dflt_system_idx: %d\n", dflt_system_idx);

}

struct dp_qos_setting* dp_get_qos_cfg(int inst, int dp_port, int alloc_flag, u32 qos_id)
{
	int i, category_idx = -1, sub_category_idx = -1;

	i = ffs(DP_F_DEV_END);
	alloc_flag &= GENMASK(i - 1, 0);
	pr_info("to match alloc_flag=0x%x\n", alloc_flag);

	if (qos_id >= MAX_QOS_ID_PER_CATAGORY)
		goto SYS_DEFAULT;

	for (i = 0; i < MAX_QOS_CATAGORY; i++) {
		/* match qos_id first */
		if (dp_qos_cfg[inst][i][qos_id].valid &&
		    (dp_qos_cfg[inst][i][qos_id].flag & alloc_flag)) {
			category_idx = i;
			sub_category_idx = qos_id;
			goto EXIT;
		}

		/* then match category only */
		if (qos_id &&
		    dp_qos_cfg[inst][i][0].valid &&
		    (dp_qos_cfg[inst][i][0].flag & alloc_flag)) {
			category_idx = i;
			sub_category_idx = 0;
			goto EXIT;
		}
	}

SYS_DEFAULT:
	/* use system default */
	category_idx = dflt_system_idx;
	sub_category_idx = 0;
EXIT:
	DP_DEBUG(DP_DBG_FLAG_DBG,
		 "%s match category qos_cfg[%d][%d][%d] for dp_port=%d alloc_flags=0x%x, qos_id=%d\n",
	         __func__, inst, category_idx, sub_category_idx,
	        dp_port, alloc_flag, qos_id);
	return &dp_qos_cfg[inst][category_idx][sub_category_idx];
}

int dp_qos_get_q_global_parms2(int inst, int dp_port, int alloc_flag,
			       u32 qos_id, struct dp_qos_q_parms *parms)
{
	struct dp_qos_setting *dqs;

	dqs = dp_get_qos_cfg(inst, dp_port, alloc_flag, qos_id);

	parms->wred_en = dqs->wred_en;
	parms->codel_en = dqs->codel_en;
	parms->qlen = dqs->qlen;

	return 0;
}
EXPORT_SYMBOL(dp_qos_get_q_global_parms2);

/* note: for intermediate path, its qos_id is in fact cqm dequeue port/ring
 *       it has to match each entry one by one and cannot use it as index to
 *       direct access
 */
struct dp_qos_setting* dp_get_inter_qos_cfg(int inst, u32 deq_port)
{
	int i;
	int category_idx = -1, sub_category_idx = 0;
	int min, max;

	if (deq_port != 0) {
		for (i = 1; i < MAX_QOS_ID_PER_CATAGORY; i++) {
			/* try to match unique specified qos_id only */
			if (!dp_qos_cfg[inst][dflt_inter_idx][i].valid)
				continue;
			min = dp_qos_cfg[inst][dflt_inter_idx][i].cqm_deq.base;
			max = dp_qos_cfg[inst][dflt_inter_idx][i].cqm_deq.base +
			      dp_qos_cfg[inst][dflt_inter_idx][i].cqm_deq.num;
			if ((deq_port >= min) && (deq_port < max)) {
				category_idx = dflt_inter_idx;
				sub_category_idx = i;
				goto EXIT;
			}
		}
	}

	if (dp_qos_cfg[inst][dflt_inter_idx][0].valid) {
		category_idx = dflt_inter_idx;
		sub_category_idx = i;
		goto EXIT;
	}

	category_idx = dflt_system_idx;
	sub_category_idx = 0;

EXIT:
	DP_DEBUG(DP_DBG_FLAG_DBG,
		 "%s: match category qos_cfg[%d][%d][%d] for deq_port=%d\n",
	         __func__, inst, category_idx, sub_category_idx, deq_port);
	return &dp_qos_cfg[inst][dflt_inter_idx][0];;
}


DP_NO_OPTIMIZE_POP

