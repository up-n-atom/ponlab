// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2022, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 * Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 * Copyright 2012 - 2014 Lantiq Deutschland GmbH
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DEBUGFS)
#include <net/datapath_proc_api.h>
#include <net/datapath_api.h>
#include <linux/list.h>
#include "../datapath.h"
#include "datapath_misc.h"

#define PROC_PARSER "parser"
#define PROC_GPID "gpid_hal"
#define PROC_DEQ_HAL "deq_hal"
#define PROC_DEQ_CHILD_HAL "deq_child_hal"
#define PROC_SCHED "sched_hal"
#define PROC_SCHED_CHILD "sched_child_hal"
#define PROC_QUEUE "queue_hal"
#define PROC_NODE "node_hal"
#define PROC_MIB_COUNT	"mib_count"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

static void proc_parser_read(struct seq_file *s)
{
	const char *str[] = {"cpu", "mpe1", "mpe2", "mpe3"};
	s8 parser[4];
	int i;

	if (!capable(CAP_NET_ADMIN))
		return;

	dp_gsw_get_parser(&parser[0], &parser[1], &parser[2], &parser[3]);
	for (i = 0; i < ARRAY_SIZE(str); i++)
		dp_sprintf(s, "%-4s : %s with parser size = %d bytes\n",
			   str[i], parser_flag_str(parser[i]),
			   parser_size_via_index(i));
}

static void set_parser(char **parser, int parser_num, s8 val)
{
	s8 cpu, mpe1, mpe2, mpe3, flag = 0;
	int i;

	dp_gsw_get_parser(&cpu, &mpe1, &mpe2, &mpe3);
	for (i = 1; i < parser_num; i++) {
		if (!strncmp(parser[i], "cpu", strlen(parser[i]))) {
			cpu = val;
			flag |= F_MPE_NONE;
		} else if (!strncmp(parser[i], "mpe1", strlen(parser[i]))) {
			mpe1 = val;
			flag |= F_MPE1_ONLY;
		} else if (!strncmp(parser[i], "mpe2", strlen(parser[i]))) {
			mpe2 = val;
			flag |= F_MPE2_ONLY;
		} else if (!strncmp(parser[i], "mpe3", strlen(parser[i]))) {
			mpe3 = val;
			flag |= F_MPE1_MPE2;
		}
	}

	if (!flag) {
		flag = F_MPE_NONE | F_MPE1_ONLY | F_MPE2_ONLY | F_MPE1_MPE2;
		cpu = val;
		mpe1 = val;
		mpe2 = val;
		mpe3 = val;
	}

	pr_info("flag=0x%x mpe3/2/1/cpu=%d/%d/%d/%d\n",
		flag, mpe3, mpe2, mpe1, cpu);

	dp_gsw_set_parser(flag, cpu, mpe1, mpe2, mpe3);
}

ssize_t proc_parser_write(struct file *file, const char *buf,
			  size_t count, loff_t *ppos)
{
	int num, pce_rule_id, inst = 0;
	static GSW_PCE_rule_t *pce;
	struct core_ops *gsw_handle;
	struct tflow_ops *gsw_tflow;
	char *param_list[15] = {NULL};
	char *str, *p;

	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	str = dp_kzalloc(count + 1, GFP_ATOMIC);
	if (!str)
		return -ENOMEM;

	pce = dp_kzalloc(sizeof(*pce), GFP_ATOMIC);
	if (!pce) {
		kfree(str);
		return -ENOMEM;
	}

	if (dp_copy_from_user(str, buf, count)) {
		kfree(str);
		kfree(pce);
		return -EFAULT;
	}

	gsw_handle = dp_port_prop[inst].ops[GSWIP_R];
	gsw_tflow = &gsw_handle->gsw_tflow_ops;
	p = str;
	for (num = 0; num < ARRAY_SIZE(param_list); num++) {
		if (!strlen(p))
			break;
		param_list[num] = dp_strsep(&p, " \n");
		if (!p)
			break;
	}

	if (!strcasecmp(param_list[0], "enable")) {
		set_parser(param_list, num, 2);
	} else if (!strcasecmp(param_list[0], "disable")) {
		set_parser(param_list, num, 0);
	} else if (!strcasecmp(param_list[0], "refresh")) {
		dp_gsw_get_parser(NULL, NULL, NULL, NULL);
		pr_info("value:cpu=%d mpe1=%d mpe2=%d mpe3=%d\n",
			pinfo[0].v, pinfo[1].v, pinfo[2].v, pinfo[3].v);
		pr_info("size :cpu=%d mpe1=%d mpe2=%d mpe3=%d\n",
			pinfo[0].size, pinfo[1].size,
			pinfo[2].size, pinfo[3].size);
	} else if (!strcasecmp(param_list[0], "mark")) {
		int flag = dp_atoi(param_list[1]);

		pce_rule_id = dp_atoi(param_list[2]);

		if (flag < 0)
			flag = 0;
		else if (flag > 3)
			flag = 3;
		pr_info("eProcessPath_Action set to %d\n", flag);
		/*: All packets set to same mpe flag as specified */
		pce->pattern.bEnable = 1;
		pce->pattern.nIndex = pce_rule_id;

		pce->pattern.bParserFlagMSB_Enable = 1;
		/* rule.pce.pattern.nParserFlagMSB = 0x0021; */
		pce->pattern.nParserFlagMSB_Mask = 0xffff;
		pce->pattern.bParserFlagLSB_Enable = 1;
		/* rule.pce.pattern.nParserFlagLSB = 0x0000; */
		pce->pattern.nParserFlagLSB_Mask = 0xffff;
		/* rule.pce.pattern.eDstIP_Select = 2; */

		pce->pattern.nDstIP_Mask = 0xffffffff;

		pce->action.bRtDstIpMaskCmp_Action = 1;
		pce->action.bRtSrcIpMaskCmp_Action = 1;
		pce->action.bRtDstPortMaskCmp_Action = 1;
		pce->action.bRtSrcPortMaskCmp_Action = 1;

		pce->action.bRMON_Action = 1;
		pce->action.bRoutExtId_Action = 1;
		pce->action.bRtCtrlEna_Action = 1;
		pce->action.bRtAccelEna_Action = 1;
		pce->action.eProcessPath_Action = flag;

		if (gsw_tflow->TFLOW_PceRuleWrite(gsw_handle, pce))
			pr_err("DPM: PCE rule add fail: GSW_PCE_RULE_WRITE\n");
	} else if (!strcasecmp(param_list[0], "unmark")) {
		/*: All packets set to same mpe flag as specified */
		pce_rule_id = dp_atoi(param_list[1]);
		pce->pattern.nIndex = pce_rule_id;
		if (gsw_tflow->TFLOW_PceRuleWrite(gsw_handle, pce))
			pr_err("DPM: PCE rule add fail:GSW_PCE_RULE_WRITE\n");
	} else {
		pr_info("Usage: echo %s [cpu] [mpe1] [mpe2] [mpe3] > parser\n",
			"<enable/disable>");
		pr_info("Usage: echo <refresh> parser\n");

		pr_info("Usage: echo %s > parser\n",
			"mark eProcessPath_Action_value(0~3) pce_rule_id");
		pr_info("Usage: echo unmark pce_rule_id > parser\n");
	}

	kfree(str);
	kfree(pce);

	return count;
}

static char *get_bp_member_string(int inst, u16 bp, int bp_num, char *buf,
				  int len)
{
	GSW_BRIDGE_portConfig_t *bp_cfg;
	struct core_ops *gsw_handle;
	struct brdgport_ops *gsw_bp;
	int i, ret, idx = 0;

	buf[0] = 0;
	gsw_handle = dp_port_prop[inst].ops[GSWIP_L];
	gsw_bp = &gsw_handle->gsw_brdgport_ops;
	bp_cfg = dp_kzalloc(sizeof(*bp_cfg), GFP_ATOMIC);
	if (!bp_cfg)
		return buf;
	bp_cfg->nBridgePortId = bp;
	bp_cfg->eMask = GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP |
		       GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_ID;
	ret = gsw_bp->BridgePort_ConfigGet(gsw_handle, bp_cfg);
	if (ret != GSW_statusOk) {
		pr_err("DPM: Failed to get bridge port's member for bridgeport=%d\n",
		       bp_cfg->nBridgePortId);
		kfree(bp_cfg);
		return buf;
	}

	for (i = 0; i < bp_num; i++) {
		if (GET_BP_MAP(bp_cfg->nBridgePortMap, i)) {
			snprintf(&buf[idx], len - idx, "%d ", i);
			idx = strlen(buf);
		}
	}

	snprintf(&buf[idx], len - idx, "Fid=%d(hw)", bp_cfg->nBridgeId);
	kfree(bp_cfg);
	return buf;
}

#define FID_LEN	16
/* proc_print_ctp_bp_info is a callback API, not a standalone proc API */
int proc_print_ctp_bp_info(struct seq_file *s, int inst,
			   struct pmac_port_info *port,
			   int subif_index, u32 flag)
{
	struct dp_subif_info *sif = get_dp_port_subif(port, subif_index);
	char *buf, tmp[8] = {0};
	struct logic_dev *ldev;
	struct dp_cap *cap;
	int bp = sif->bp;
	int len, bp_num;

	cap = dp_kzalloc(sizeof(*cap), GFP_ATOMIC);
	if (!cap)
		return DP_FAILURE;
	cap->inst = inst;
	dp_get_cap(cap, 0);
	bp_num = cap->max_num_bridge_port;

	kfree(cap);

	/* Get max digit for bridge port
	 * Assuming it is not more than 7 digits
	 */
	snprintf(tmp, 7, "%d ", bp_num);
	len = bp_num * strlen(tmp) + FID_LEN;

	buf = dp_kzalloc(len, GFP_ATOMIC);
	if (!buf)
		return 0;

	dp_sprintf(s, "           bp=%d(member:%s/%d(sif))\n",
		   bp, get_bp_member_string(inst, bp, bp_num, buf, len),
		   sif->fid);
	list_for_each_entry(ldev, &sif->logic_dev, list) {
		dp_sprintf(s, "             %s: bp=%d(member:%s)\n",
			   ldev->dev->name, ldev->bp,
			   get_bp_member_string(inst, ldev->bp, bp_num, buf,
			   len));
	}

	kfree(buf);

	return 0;
}

int proc_gpid_dump(struct seq_file *s, int pos)
{
	int inst = 0;
	struct dp_gpid_map_table *m = &HAL(inst)->gp_dp_map[pos];

	if (!capable(CAP_SYS_PACCT))
		return -1;
	if (!dp_init_ok) {
		pr_err("DPM: dp not initialize yet\n");
		return -1;
	}
	if (pos == 0)
		dp_sprintf(s, "%4s %4s %4s %8s\n",
			   "gpid", "ref", "dpid", "subif");
	if (!m->ref_cnt)
		goto EXIT;
	dp_sprintf(s, "%04d %04d %04d %08x\n",
		   pos, m->ref_cnt, m->dpid, m->subif);
EXIT:
	if (pos == MAX_GPID - 1)
		dp_sprintf(s, "%4s %4s %4s %8s\n",
			   "gpid", "ref", "dpid", "subif");

	if (!dp_seq_has_overflowed(s))
		pos++;
	if (pos > MAX_GPID - 1)
		pos = -1; /*end of the loop */
	return pos;
}

int proc_deq_hal_dump(struct seq_file *s, int pos)
{
	int inst = 0;
	struct cqm_deq_stat *m = &HAL(inst)->deq_port_stat[pos];

	if (!capable(CAP_SYS_PACCT))
		return -1;
	if (!dp_init_ok) {
		pr_err("DPM: dp not initialize yet\n");
		return -1;
	}

	if (!m->disabled && !m->child_num)
		goto EXIT;
	if (pos == 0)
		dp_sprintf(s, "%10s %10s %10s %10s\n",
			   "deq_port", "status", "num_child", "node_id");
	dp_sprintf(s, "%10d %10s %10d %10d\n",
		   pos,
		   m->disabled ? "disable" : "enable",
		   m->child_num,
		   m->node_id);
EXIT:
	if (pos == DP_MAX_PPV4_PORT - 1)
		dp_sprintf(s, "%10s %10s %10s %10s\n",
			   "deq_port", "status", "num_child", "node_id");
	if (!dp_seq_has_overflowed(s))
		pos++;
	if (pos > DP_MAX_PPV4_PORT - 1)
		pos = -1; /*end of the loop */
	return pos;
}

int proc_deq_child_hal_dump(struct seq_file *s, int pos)
{
	int inst = 0;
	struct cqm_deq_stat *m = &HAL(inst)->deq_port_stat[pos];
	struct pp_sch_stat *m1 = NULL;
	int i, print = 0;
	char *str = "?????";

	if (!capable(CAP_SYS_PACCT))
		return -1;
	if (!dp_init_ok) {
		pr_err("DPM: dp not initialize yet\n");
		return -1;
	}

	if (m->disabled)
		goto EXIT;

	dp_sprintf(s,
		   "CqmDeqPort/NodeID: %2d/%d, %8s, ActiveNumChild: %d\n",
		   pos, m->node_id,
		   m->disabled ? "disabled" : "enabled",
		   m->child_num);
	m1 = &HAL(inst)->qos_sch_stat[m->node_id];
	if (m->child_num != m1->child_num) {
		pr_err("DPM: %s, CqmDeqPort/NodeID: %d/%d, [%d != %d] something wrong!\n",
				__func__, pos, m->node_id,
				m->child_num, m1->child_num);
		goto EXIT;
	}

	for (i = 0; i < MAX_PP_CHILD_PER_NODE; i++) {
		print = 0;
		if (m1->child[i].type == DP_NODE_UNKNOWN)
			print = 1;
		dp_sprintf(s, "    child[%d]: node_type: %s, node_id: %4d, node_stat: %s\n",
				i, print ? str : node_type_str(m1->child[i].type),
				m1->child[i].node_id,
				node_stat_str(m1->child[i].flag));
	}

EXIT:
	if (!dp_seq_has_overflowed(s))
		pos++;
	if (pos > DP_MAX_PPV4_PORT - 1)
		pos = -1; /*end of the loop */
	return pos;
}

int proc_sched_child_hal_dump(struct seq_file *s, int pos)
{
	int inst = 0;
	struct pp_sch_stat *m = &HAL(inst)->qos_sch_stat[pos];
	int i, print = 0;
	char *str = "?????";

	if (!capable(CAP_SYS_PACCT))
		return -1;
	if (!dp_init_ok) {
		pr_err("DPM: dp not initialize yet\n");
		return -1;
	}
	if (!S_NODE(m->node.type))
		goto EXIT;

	dp_sprintf(s, "schedId: %d numOfChildsActive: %d maxNumOfChilds: %d\n",
			pos, m->child_num, MAX_PP_CHILD_PER_NODE);
	for (i = 0; i < MAX_PP_CHILD_PER_NODE; i++) {
		print = 0;
		if (m->child[i].type == DP_NODE_UNKNOWN)
			print = 1;
		dp_sprintf(s, "    child[%d]: node_type: %5s, node_id: %4d, node_stat: %s\n",
				i, print ? str : node_type_str(m->child[i].type),
				m->child[i].node_id,
				node_stat_str(m->child[i].flag));
	}
EXIT:
	if (!dp_seq_has_overflowed(s))
		pos++;
	if (pos > DP_MAX_NODES - 1)
		pos = -1; /*end of the loop */
	return pos;
}
EXPORT_SYMBOL(proc_sched_child_hal_dump);

int proc_sched_hal_dump(struct seq_file *s, int pos)
{
	int inst = 0;
	struct pp_sch_stat *m = &HAL(inst)->qos_sch_stat[pos];

	if (!capable(CAP_SYS_PACCT))
		return -1;
	if (!dp_init_ok) {
		pr_err("DPM: dp not initialize yet\n");
		return -1;
	}
	if (!S_NODE(m->node.type))
		goto EXIT;

	if (!m->c_flag && !m->p_flag && m->child_num)
		goto EXIT;
	if (pos == 0)
		dp_sprintf(s, "%6s %6s %6s %6s %6s %10s %10s %10s\n",
			   "sched", "c_flag", "c_flag", "p_flag", "p_flag",
			   "num_child", "parent_type", "parent_id");
	dp_sprintf(s,  "%6d %6d %6s %6d %6s %10d %10s %10d\n",
		   pos,
		   m->c_flag,
		   node_stat_str(m->c_flag),
		   m->p_flag,
		   node_stat_str(m->p_flag),
		   m->child_num,
		   node_type_str(m->parent.type),
		   m->parent.node_id);
EXIT:
	if (pos == DP_MAX_NODES - 1)
		dp_sprintf(s, "%6s %6s %6s %6s %6s %10s %10s %10s\n",
			   "sched", "c_flag", "c_flag", "p_flag", "p_flag",
			   "num_child", "parent_type", "parent_id");
	if (!dp_seq_has_overflowed(s))
		pos++;
	if (pos > DP_MAX_NODES - 1)
		pos = -1; /*end of the loop */
	return pos;
}
EXPORT_SYMBOL(proc_sched_hal_dump);

int proc_node_hal_dump(struct seq_file *s, int pos)
{
	int inst = 0;
	struct hal_priv *priv = HAL(inst);
	struct pp_sch_stat *m = &HAL(inst)->qos_sch_stat[pos]; /* node/sched */
	struct pp_queue_stat *q; /* queue */
	struct cqm_deq_stat *p; /* port */

	if (!capable(CAP_SYS_PACCT))
		return -1;
	if (!dp_init_ok) {
		pr_err("DPM: dp not initialize yet\n");
		return -1;
	}
	if (pos == 0)
		dp_sprintf(s, "%9s %8s %12s %12s %8s %8s %8s\n",
			   "node_id", "type", "c_flag", "p_flag",
			   "parent_type", "parent_node", "child");
	if (!m->used) {
		if(m->node.type || m->c_flag || m->p_flag || m->child_num) {
			/* maybe wrong */
			dp_sprintf(s, "%d type=%d c_flag=%d p_flag=%d child_num=%d\n", 
				   pos, (int)m->node.type,
				   (int)m->c_flag, (int)m->p_flag, m->child_num);
		}
		goto EXIT;
	}

	if (Q_NODE(m->node.type)) {
		q = &priv->qos_queue_stat[m->node.id.q_id];
		dp_sprintf(s,  "%4d/%4d %8s %12s %12s %8s %8d %8s\n",
			   m->node.id.q_id, pos,
			   node_type_str(m->node.type),
			   node_stat_str(q->flag),
			   "-",
			   node_type_str(m->parent.type),
			   m->parent.node_id,
			   "-");
	} else if (S_NODE(m->node.type)) {
		dp_sprintf(s,  "%4s/%4d %8s %12s %12s %8s %8d %8d\n",
			   "", pos,
			   node_type_str(m->node.type),
			   node_stat_str(m->c_flag),
			   node_stat_str(m->p_flag),
			   node_type_str(m->parent.type),
			   m->parent.node_id,
			   m->child_num);
	} else if (P_NODE(m->node.type)) {
		p = &priv->deq_port_stat[m->node.id.cqm_deq_port];
		dp_sprintf(s,  "%4d/%4d %8s %12s %12s %8s %8s %8d\n",
			   m->node.id.cqm_deq_port, pos,
			   node_type_str(m->node.type),
			   node_stat_str(p->flag),
			   "-",
			   "-",
			   "-",
			   m->child_num);
	} else {
		dp_sprintf(s,  "%4s/%4d %8d %8s %8s %8s %8s %8s\n",
			   "",
			   pos,
			   m->node.type,
			   "-",
			   "-",
			   "-",
			   "-",
			   "-");
	}
EXIT:
	if (pos == DP_MAX_NODES - 1)
		dp_sprintf(s, "%9s %8s %12s %12s %8s %8s %8s\n",
			   "node_id", "type", "c_flag", "p_flag",
			   "parent_type", "parent_node", "child");

	if (!dp_seq_has_overflowed(s))
		pos++;
	if (pos > DP_MAX_NODES - 1)
		pos = -1; /*end of the loop */
	return pos;
}
EXPORT_SYMBOL(proc_node_hal_dump);

ssize_t proc_node_hal_write(struct file *file, const char *buf,
			  size_t count, loff_t *ppos)
{
	int node_id = 0;
	char *str;
	struct hal_priv *priv = HAL(0);
	struct pp_sch_stat *m;

	if (!capable(CAP_NET_ADMIN))
		return count;
	if (!dp_init_ok) {
		pr_err("DPM: dp not initialize yet\n");
		return count;
	}

	str = dp_kzalloc(count + 1, GFP_ATOMIC);
	if (!str)
		return count;
	if (dp_copy_from_user(str, buf, count))
		goto EXIT;
	node_id = dp_atoi(str);
	if ((node_id < 0) || (node_id >= ARRAY_SIZE(priv->qos_sch_stat))) {
		pr_err("wrong node_id=%d\n", node_id);
		goto EXIT;
	}
	m = &priv->qos_sch_stat[node_id];

	dp_sprintf(NULL, "%9s %4s %8s %12s %12s %8s %8s %8s\n",
		   "phy/node", "used", "type", "c_flag", "p_flag",
		   "parent_type", "parent_node", "child");

	dp_sprintf(NULL,  "%4d/%4d %4d %8s %12s %12s %8s %8d %8d\n",
			   m->node.id.q_id,
			   node_id,
			   m->used,
			   node_type_str(m->node.type),
			   node_stat_str(m->c_flag),
			   node_stat_str(m->p_flag),
			   node_type_str(m->parent.type),
			   m->parent.node_id,
			   m->child_num);
EXIT:
	kfree(str);

	return count;
}


int proc_queue_hal_dump(struct seq_file *s, int pos)
{
	int inst = 0;
	struct pp_queue_stat *m = &HAL(inst)->qos_queue_stat[pos];

	if (!capable(CAP_SYS_PACCT))
		return -1;
	if (!dp_init_ok) {
		pr_err("DPM: dp not initialize yet\n");
		return -1;
	}
	if (pos == 0)
		dp_sprintf(s, "%10s %10s %10s %6s %10s %6s\n",
			   "qid", "status", "dp_port", "deq_port", "resv_idx", "blocked");
	if (!m->flag)
		goto EXIT;
	dp_sprintf(s,  "%5d/%4d %10s %10d %6d %10d %6d\n",
		   pos, m->node_id,
		   node_stat_str(m->flag),
		   m->dp_port,
		   m->deq_port,
		   m->resv_idx,
		   m->blocked);
EXIT:
	if (pos == DP_MAX_QUEUE_NUM - 1)
		dp_sprintf(s, "%10s %10s %10s %6s %10s %6s\n",
			   "qid", "status", "dp_port", "deq_port", "resv_idx", "blocked");
	if (!dp_seq_has_overflowed(s))
		pos++;
	if (pos > DP_MAX_QUEUE_NUM - 1)
		pos = -1; /*end of the loop */
	return pos;
}

static struct dp_proc_entry dp_proc_entries[] = {
	/* name single_callback multi_callback init_callback write_callback */
	{PROC_PARSER, proc_parser_read, NULL, NULL, proc_parser_write},
	{PROC_GPID, NULL, proc_gpid_dump, NULL, NULL},
	{PROC_DEQ_HAL, NULL, proc_deq_hal_dump, NULL, NULL},
	{PROC_DEQ_CHILD_HAL, NULL, proc_deq_child_hal_dump, NULL, NULL},
	{PROC_SCHED, NULL, proc_sched_hal_dump, NULL, NULL},
	{PROC_SCHED_CHILD, NULL, proc_sched_child_hal_dump, NULL, NULL},
	{PROC_QUEUE, NULL, proc_queue_hal_dump, NULL, NULL},
	{PROC_NODE, NULL, proc_node_hal_dump, NULL, proc_node_hal_write},
	{PROC_MIB_COUNT, NULL, proc_mib_count_dump, proc_mib_count_init, proc_mib_count_write},
	/* last place holder */
	{NULL, NULL, NULL, NULL, NULL}
};

int dp_sub_proc_install(void)
{
	int i;

	if (!dp_proc_node) {
		pr_err("DPM: %s fail\n", __func__);
		return 0;
	}

	for (i = 0; i < ARRAY_SIZE(dp_proc_entries); i++)
		dp_proc_entry_create(dp_proc_node, &dp_proc_entries[i]);
	return 0;
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP

#endif
