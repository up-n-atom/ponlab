// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2023, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <net/datapath_api.h>
#include <net/datapath_api_qos.h>
#include <linux/pp_qos_api.h>
#include "../datapath.h"
#include "datapath_misc.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

#define LATE_FREE_DEQ_PORT 1

/* workaround since PPv4 in FLM/LGM is using different name */
#ifndef QOS_DEAFULT_MAX_BURST
#define QOS_DEAFULT_MAX_BURST QOS_DEFAULT_MAX_BURST
#endif

#define q_node_chk(x, y)				\
({							\
	typeof(x) x_ = (x);				\
	typeof(y) y_ = (y);				\
	(!is_qid_valid(y_) || is_q_node_free(x_, y_));	\
})

static struct limit_map shaper_limit_maps[] = {
	{QOS_NO_BANDWIDTH_LIMIT, DP_NO_SHAPER_LIMIT},
	{QOS_MAX_BANDWIDTH_LIMIT, DP_MAX_SHAPER_LIMIT}
};

static struct arbi_map arbi_maps[] = {
	{PP_QOS_ARBITRATION_WSP, ARBITRATION_WSP},
	{PP_QOS_ARBITRATION_WRR, ARBITRATION_WRR},
	{PP_QOS_ARBITRATION_WFQ, ARBITRATION_WFQ},
	{PP_QOS_ARBITRATION_WRR, ARBITRATION_NULL}
};

static void dp_wred_def(struct pp_qos_queue_conf *cfg)
{
	cfg->wred_enable = 0;
	cfg->wred_max_allowed = DEF_QRED_MAX_ALLOW;
	cfg->wred_min_guaranteed = DEF_QRED_MIN_ALLOW;
}

int qos_platform_set(int cmd_id, void *node, int flag)
{
	struct dp_node_link *node_link = (struct dp_node_link *)node;
	struct hal_priv *priv;
	int res;

	priv = HAL(node_link->inst);
	if (!priv) {
		pr_err("DPM: DP HAL not initialized yet\n");
		return DP_FAILURE;
	}
	if (!priv->qdev) {
		pr_err("DPM: qdev NULL with inst=%d\n", node_link->inst);
		return DP_FAILURE;
	}

	spin_lock_bh(&priv->qos_lock);

	switch (cmd_id) {
	case NODE_LINK_ADD:
		res = _dp_node_link_add(node, flag);
		break;
	case NODE_LINK_GET:
		res = _dp_node_link_get(node, flag);
		break;
	case NODE_LINK_EN_GET:
		res = _dp_node_link_en_get(node, flag);
		break;
	case NODE_LINK_EN_SET:
		res = _dp_node_link_en_set(node, flag);
		break;
	case NODE_UNLINK:
		res = _dp_node_unlink(node, flag);
		break;
	case LINK_ADD:
		res = _dp_link_add(node, flag);
		break;
	case LINK_GET:
		res = _dp_link_get(node, flag);
		break;
	case LINK_PRIO_SET:
		res = _dp_qos_link_prio_set(node, flag);
		break;
	case LINK_PRIO_GET:
		res = _dp_qos_link_prio_get(node, flag);
		break;
	case QUEUE_CFG_SET:
		res = _dp_queue_conf_set(node, flag);
		break;
	case QUEUE_CFG_GET:
		res = _dp_queue_conf_get(node, flag);
		break;
	case SHAPER_SET:
		res = _dp_shaper_conf_set(node, flag);
		break;
	case SHAPER_GET:
		res = _dp_shaper_conf_get(node, flag);
		break;
	case NODE_ALLOC:
		res = _dp_node_alloc(node, flag);
		break;
	case NODE_FREE:
		res = _dp_node_free(node, flag);
		break;
	case NODE_CHILDREN_FREE:
		res = dp_free_children_via_parent(node, flag);
		break;
	case DEQ_PORT_RES_GET:
		res = _dp_deq_port_res_get(node, flag);
		break;
	case COUNTER_MODE_SET:
		res = _dp_counter_mode_set(node, flag);
		break;
	case COUNTER_MODE_GET:
		res = _dp_counter_mode_get(node, flag);
		break;
	case QUEUE_MAP_GET:
		res = _dp_queue_map_get(node, flag);
		break;
	case QUEUE_MAP_SET:
		res = _dp_queue_map_set(node, flag);
		break;
	case NODE_CHILDREN_GET:
		res = _dp_children_get(node, flag);
		break;
	case QOS_LEVEL_GET:
		res = _dp_qos_max_level_get(node, flag);
		break;
	case QOS_GLOBAL_CFG_GET:
		res = _dp_qos_global_info_get(node, flag);
		break;
	case QOS_Q_LOGIC:
		res = dp_get_queue_logic(node, flag);
		break;
	case QOS_PORT_CFG_SET:
		res = _dp_qos_port_conf_set(node, flag);
		break;
	case QOS_BLOCK_FLUSH_PORT:
		res = dp_port_block_flush(node, flag);
		break;
	case QOS_BLOCK_FLUSH_QUEUE:
		res = dp_queue_block_flush(node, flag);
		break;
	case CODEL_SET:
		res = dp_codel_cfg_set(node, flag);
		break;
	case CODEL_GET:
		res = dp_codel_cfg_get(node, flag);
		break;
	default:
		pr_err("DPM: %s Unsupported cmd_id %d\n", __func__, cmd_id);
		res = DP_FAILURE;
		break;
	}

	spin_unlock_bh(&priv->qos_lock);

	return res;
}

static int pp2dp_shaper_limit(u32 pp_limit, u32 *dp_limit)
{
	int i;

	if (pp_limit > QOS_MAX_BANDWIDTH_LIMIT) {
		pr_err("DPM: %s wrong pp shaper limit: %u\n", __func__, pp_limit);
		return DP_FAILURE;
	}

	for (i = 0; i < ARRAY_SIZE(shaper_limit_maps); i++) {
		if (shaper_limit_maps[i].pp_limit == pp_limit) {
			*dp_limit = shaper_limit_maps[i].dp_limit;
			return DP_SUCCESS;
		}
	}

	/* Kbps */
	*dp_limit = pp_limit;

	return DP_SUCCESS;
}

static int dp2pp_shaper_limit(u32 dp_limit, u32 *pp_limit)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(shaper_limit_maps); i++) {
		if (shaper_limit_maps[i].dp_limit == dp_limit) {
			*pp_limit = shaper_limit_maps[i].pp_limit;
			return DP_SUCCESS;
		}
	}

	/* Kbps */
	if (dp_limit > QOS_MAX_BANDWIDTH_LIMIT) {
		pr_err("DPM: %s wrong dp shaper limit: %u\n", __func__, dp_limit);
		return DP_FAILURE;
	}
	*pp_limit = dp_limit;

	return DP_SUCCESS;
}

static int pp2dp_arbi(int pp_arbi)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(arbi_maps); i++) {
		if (arbi_maps[i].pp_arbi == pp_arbi)
			return arbi_maps[i].dp_arbi;
	}

	return DP_FAILURE;
}

static int dp2pp_arbi(int dp_arbi)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(arbi_maps); i++) {
		if (arbi_maps[i].dp_arbi == dp_arbi)
			return arbi_maps[i].pp_arbi;
	}

	return DP_FAILURE;
}

/* Search for matching queue node id.
 * Upon Success return physical queue id
 * else return DP_FAILURE
 */
static int get_qid_by_node(int inst, int node_id)
{
	struct hal_priv *priv = HAL(inst);
	int i;

	for (i = 0; i < DP_MAX_QUEUE_NUM; i++) {
		if (node_id == priv->qos_queue_stat[i].node_id)
			return i;
	}

	return DP_FAILURE;
}

/* Search for matching port node id.
 * Upon Success return physical physical port id
 * else return DP_FAILURE
 */
static int get_cqm_deq_port_by_node(int inst, int node_id)
{
	struct hal_priv *priv = HAL(inst);
	int i;

	for (i = 0; i <  ARRAY_SIZE(priv->deq_port_stat); i++) {
		if (node_id == priv->deq_port_stat[i].node_id)
			return i;
	}

	return DP_FAILURE;
}

static int dp_qos_queue_set_spl(int inst, u32 node_id, u32 qid,
				struct pp_qos_queue_conf *cfg)
{
	struct hal_priv *priv = HAL(inst);

	/* Set Q to blocking state during movement */
	cfg->blocked = 1;
	if (dp_qos_queue_set(priv->qdev, node_id, cfg)) {
		pr_err("DPM: %s dp_qos_queue_set Q[%d/%d] fail\n",
		       __func__, qid, node_id);
		return DP_FAILURE;
	}

	/* Update local Q status */
	priv->qos_queue_stat[qid].blocked = cfg->blocked;

	return DP_SUCCESS;
}

/* Check index in lookup table for qid, set to drop_q
 */
static
void dp_map_to_drop_q(int inst, int qid, struct cbm_lookup_entry *lu)
{
	struct hal_priv *priv = HAL(inst);

	lu->ppv4_drop_q = priv->ppv4_drop_q;
	lu->qid = qid;

	CBM_OPS(inst, cqm_map_to_drop_q, lu);
}

#ifdef DP_FLUSH_VIA_AUTO
static void cqm_queue_flush(int inst, int cqm_inst, int drop_port, int qid,
			    u32 node_id)
{}
#else
static void cqm_queue_flush(int inst, int cqm_inst, int drop_port, int qid,
			    u32 node_id)
{
	/* Before calling this API, Q is unmapped in lookup table.
	 * The Q is attached to drop port, blocked and resume.
	 */
	CBM_OPS(inst, cqm_qos_queue_flush, cqm_inst, drop_port, qid,
		node_id);

	DP_DEBUG(DP_DBG_FLAG_QOS, "done\n");
}
#endif

/* Note: When this API is returned, queue must be in suspend/block mode.
 * Queue may need to move to other scheduler/port after flushing.
 * node_id is logical node.
 */
static int queue_flush(int inst, int node_id, int flag)
{
	int qid, blocked, pid, cbm_inst, res = DP_FAILURE;
	struct pp_qos_queue_conf *q_cfg, *q_cfg_tmp;
	struct hal_priv *priv = HAL(inst);
	struct cbm_lookup_entry *lu;

	if (node_id >= ARRAY_SIZE(priv->qos_sch_stat)) {
		pr_err("DPM: %s error: node_id(%d) should be less than %zu\n",
		       __func__, node_id, ARRAY_SIZE(priv->qos_sch_stat));
		return DP_FAILURE;
	}
	qid = get_qid_by_node(inst, node_id);
	if (qid >= ARRAY_SIZE(priv->qos_queue_stat)) {
		pr_err("DPM: %s error: qid(%d) should be less than %zu\n",
		       __func__, qid, ARRAY_SIZE(priv->qos_queue_stat));
		return DP_FAILURE;
	}
	if (qid == DP_FAILURE) {
		pr_err("DPM: %s get_qid_by_node Q:%d fail\n", __func__, node_id);
		return DP_FAILURE;
	}

	q_cfg = dp_kzalloc(sizeof(*q_cfg), GFP_ATOMIC);
	if (!q_cfg)
		return DP_FAILURE;

	if (flag & DP_QFLUSH_FLAG_RESTORE_QOS_PORT) {
		if (dp_qos_queue_conf_get(priv->qdev, node_id, q_cfg)) {
			pr_err("DPM: %s dp_qos_queue_conf_get Q[%d/%d] fail\n",
			       __func__, qid, node_id);
			kfree(q_cfg);
			return DP_FAILURE;
		}
		blocked = q_cfg->blocked;
		pid = q_cfg->queue_child_prop.parent;
	} else {
		blocked = priv->qos_queue_stat[qid].blocked;
		pid = priv->qos_sch_stat[node_id].parent.node_id;
	}

	lu = dp_kzalloc(sizeof(*lu), GFP_ATOMIC);
	if (!lu) {
		kfree(q_cfg);
		return DP_FAILURE;
	}

	/* Map to drop queue and save lookup entries for recovery */
	dp_map_to_drop_q(inst, qid, lu);

	/* Ensure that all enqueue packets are drop on the existing q */
	if (!blocked) {
		if (pp_qos_queue_block(priv->qdev, node_id)) {
			pr_err("DPM: %s pp_qos_queue_block Q[%d/%d] fail\n",
			       __func__, qid, node_id);
			goto exit;
		}
	}

	cbm_inst = dp_port_prop[inst].cbm_inst;
	if (pid == priv->ppv4_drop_p) {
		DP_DEBUG(DP_DBG_FLAG_QOS,
			 "qid:%d already linked to drop port:%d\n",
			 qid, pid);

		cqm_queue_flush(inst, cbm_inst, priv->cqm_drop_p, qid, node_id);
	} else {
		DP_DEBUG(DP_DBG_FLAG_QOS, "Q movement before flush\n");

		q_cfg_tmp = dp_kzalloc(sizeof(*q_cfg), GFP_ATOMIC);
		if (!q_cfg_tmp)
			goto exit;

		/* Move to drop port, set block and resume the queue */
		dp_qos_queue_conf_set_default(q_cfg_tmp);
		dp_wred_def(q_cfg_tmp);
		q_cfg_tmp->queue_child_prop.parent = priv->ppv4_drop_p;
		if (dp_qos_queue_set_spl(inst, node_id, qid, q_cfg_tmp)) {
			pr_err("DPM: %s dp_qos_queue_set_spl qid:%d to P:%d fail\n",
			       __func__, qid, priv->ppv4_drop_p);
			kfree(q_cfg_tmp);
			goto exit;
		}

		DP_DEBUG(DP_DBG_FLAG_QOS, "move qid:%d to drop port:%d\n",
			 qid, priv->ppv4_drop_p);

		cqm_queue_flush(inst, cbm_inst, priv->cqm_drop_p, qid, node_id);

		if (flag & DP_QFLUSH_FLAG_RESTORE_QOS_PORT) {
			/* Move back the queue to original parent
			 * with original variable queue_cfg
			 */
			if (dp_qos_queue_set_spl(inst, node_id,
						 qid, q_cfg)) {
				pr_err("DPM: %s qos_queue_conf_set Q[%d/%d] fail\n",
				       __func__, qid, node_id);
				kfree(q_cfg_tmp);
				goto exit;
			}

			DP_DEBUG(DP_DBG_FLAG_QOS,
				 "move qid:%d back to P:%d\n", qid, pid);
		}
		kfree(q_cfg_tmp);
	}

	/* Restore lookup entry mapping for this qid if needed */
	if (flag & DP_QFLUSH_FLAG_RESTORE_LOOKUP) {
		lu->qid = qid;
		CBM_OPS(inst, cqm_restore_orig_q, lu);
	}

	res = DP_SUCCESS;

exit:
	kfree(lu);
	kfree(q_cfg);

	return res;
}

static int get_node_type_by_node_id(int inst, int node_id)
{
	struct hal_priv *priv = HAL(inst);

	return priv->qos_sch_stat[node_id].node.type;
}

static int get_free_child_idx(int inst, int node_id, int flag)
{
	struct hal_priv *priv = HAL(inst);
	int i;

	for (i = 0; i < DP_MAX_CHILD_PER_NODE; i++) {
		if (PP_FREE(CHILD(node_id, i).flag))
			return i;
	}

	return DP_FAILURE;
}

static int get_parent_node(int inst, int node_id, int flag)
{
	int type = get_node_type_by_node_id(inst, node_id);
	struct hal_priv *priv = HAL(inst);
	struct pp_sch_stat *sch_stat;

	sch_stat = &priv->qos_sch_stat[node_id];
	if (sch_stat->parent.flag && !P_NODE(type))
		return sch_stat->parent.node_id;

	return DP_FAILURE;
}

static int get_child_idx_node_id(int inst, int node_id, int flag)
{
	struct hal_priv *priv = HAL(inst);
	int i, pid;

	pid = priv->qos_sch_stat[node_id].parent.node_id;
	for (i = 0; i < DP_MAX_CHILD_PER_NODE; i++) {
		if (node_id == CHILD(pid, i).node_id)
			return i;
	}

	return DP_FAILURE;
}

/* Set queue flag from PP_NODE_ACTIVE to PP_NODE_ALLOC.
 */
static int node_queue_dec(int inst, int node_id, int flag)
{
	struct hal_priv *priv = HAL(inst);
	struct pp_queue_stat *q_stat;
	struct pp_sch_stat *sch_stat;
	int qid, pid, idx;

	qid = get_qid_by_node(inst, node_id);
	if (qid == DP_FAILURE) {
		pr_err("DPM: %s get_qid_by_node Q:%d fail\n", __func__, node_id);
		return DP_FAILURE;
	}

	q_stat = &priv->qos_queue_stat[qid];
	sch_stat = &priv->qos_sch_stat[node_id];

	if (!PP_ACT(q_stat->flag)) {
		pr_err("DPM: %s wrong Q[%d/%d] stat(%d):expect ACTIVE\n",
		       __func__, qid, node_id, q_stat->flag);
		return DP_FAILURE;
	}

	if (!sch_stat->parent.flag) {
		pr_err("DPM: %s wrong Q[%d/%d]'s parent stat(%d):expect ACTIVE\n",
		       __func__, qid, node_id, sch_stat->parent.flag);
		return DP_FAILURE;
	}

	pid = get_parent_node(inst, node_id, flag);
	if (pid == DP_FAILURE) {
		pr_err("DPM: %s get_parent_node Q[%d/%d] fail\n",
		       __func__, qid, node_id);
		return DP_FAILURE;
	}

	idx = get_child_idx_node_id(inst, node_id, 0);
	if (idx == DP_FAILURE) {
		pr_err("DPM: %s get_child_idx_node_id Q[%d/%d] fail\n",
		       __func__, qid, node_id);
		return DP_FAILURE;
	}

	DP_DEBUG(DP_DBG_FLAG_QOS, "parent:%d - Q:[%d/%d] child idx:%d\n",
		 pid, qid, node_id, idx);

	CHILD(pid, idx).type = 0;
	CHILD(pid, idx).node_id = 0;
	CHILD(pid, idx).flag = PP_NODE_FREE;
	sch_stat->parent.type = 0;
	sch_stat->parent.flag = 0;
	sch_stat->parent.node_id = 0;
	sch_stat->p_flag &= ~PP_NODE_ACTIVE;
	q_stat->flag &= ~PP_NODE_ACTIVE;

	return DP_SUCCESS;
}

/* Set queue flag from PP_NODE_ALLOC to PP_NODE_ACTIVE
 */
static int node_queue_inc(int inst, int node_id, int flag)
{
	struct hal_priv *priv = HAL(inst);
	struct pp_queue_stat *q_stat;
	struct pp_sch_stat *sch_stat;
	int qid, pid, idx;

	qid = get_qid_by_node(inst, node_id);
	if (qid == DP_FAILURE) {
		pr_err("DPM: %s get_qid_by_node Q:%d fail\n", __func__, node_id);
		return DP_FAILURE;
	}

	q_stat = &priv->qos_queue_stat[qid];
	sch_stat = &priv->qos_sch_stat[node_id];

	if (!PP_ALLOC(q_stat->flag)) {
		pr_err("DPM: %s wrong Q[%d/%d] stat(%d):expect ALLOC\n",
		       __func__, qid, node_id, q_stat->flag);
		return DP_FAILURE;
	}

	pid = get_parent_node(inst, node_id, flag);
	if (pid == DP_FAILURE) {
		pr_err("DPM: %s get_parent_node Q[%d/%d] fail\n",
		       __func__, qid, node_id);
		return DP_FAILURE;
	}

	idx = get_free_child_idx(inst, pid, 0);
	if (idx == DP_FAILURE) {
		pr_err("DPM: %s get_free_child_idx Q[%d/%d] fail\n",
		       __func__, qid, node_id);
		return DP_FAILURE;
	}

	DP_DEBUG(DP_DBG_FLAG_QOS, "parent:%d - Q:[%d/%d] child idx:%d\n",
		 pid, qid, node_id, idx);

	CHILD(pid, idx).node_id = node_id;
	CHILD(pid, idx).type = DP_NODE_QUEUE;
	CHILD(pid, idx).flag = PP_NODE_ACTIVE;
	q_stat->flag |= PP_NODE_ACTIVE;
	sch_stat->p_flag |= PP_NODE_ACTIVE;

	return DP_SUCCESS;
}

/* Set queue flag from PP_NODE_ALLOC to PP_NODE_FREE.
 * Reset Q and Sch node.
 */
static int node_queue_rst(int inst, int node_id, int flag)
{
	struct hal_priv *priv = HAL(inst);
	struct pp_queue_stat *q_stat;
	struct pp_sch_stat *sch_stat;
	int qid, dp_port, resv_idx;

	qid = get_qid_by_node(inst, node_id);
	if (qid == DP_FAILURE) {
		pr_err("DPM: %s get_qid_by_node Q:%d fail\n", __func__, node_id);
		return DP_FAILURE;
	}

	q_stat = &priv->qos_queue_stat[qid];

	dp_port = q_stat->dp_port;
	resv_idx = q_stat->resv_idx;

	DP_DEBUG(DP_DBG_FLAG_QOS, "dp_port=%d Q:[%d/%d] resv_idx:%d\n",
		 dp_port, qid, node_id, resv_idx);

	if (PP_RESV(q_stat->flag))
		priv->resv[dp_port].resv_q[resv_idx].flag = PP_NODE_FREE;

	dp_memset(q_stat, 0, sizeof(*q_stat));
	q_stat->resv_idx = INV_RESV_IDX;

	sch_stat = &priv->qos_sch_stat[node_id];
	dp_memset(sch_stat, 0, sizeof(*sch_stat));
	sch_stat->resv_idx = INV_RESV_IDX;

	return DP_SUCCESS;
}

/* Set Sch flag from PP_NODE_ACTIVE to PP_NODE_ALLOC.
 */
static int node_sched_dec(int inst, int node_id, int flag)
{
	struct hal_priv *priv = HAL(inst);
	struct pp_sch_stat *sch_stat;
	int pid, idx;

	sch_stat = &priv->qos_sch_stat[node_id];

	if (flag & C_FLAG) {
		DP_DEBUG(DP_DBG_FLAG_QOS,
			 "parent Sch:%d\n", node_id);

		if (!PP_ACT(sch_stat->c_flag) || !sch_stat->child_num) {
			pr_err("DPM: %s wrong Sch:%d stat(%d)/child_num(%d):%s\n",
			       __func__, node_id,
			       sch_stat->c_flag, sch_stat->child_num,
			       "expect ACTIVE or non-zero child_num");
			return DP_FAILURE;
		}
		DP_DEBUG(DP_DBG_FLAG_QOS,
			 "reduce sched[%d] child_num from %d to %d\n",
			 node_id,
			 sch_stat->child_num, sch_stat->child_num - 1);
		sch_stat->child_num--;
		if (!sch_stat->child_num)
			sch_stat->c_flag &= ~PP_NODE_ACTIVE;
	} else if (flag & P_FLAG) {
		if (!PP_ACT(sch_stat->p_flag)) {
			pr_err("DPM: %s wrong Sch:%d stat(%d):expect ACTIVE\n",
			       __func__, node_id, sch_stat->p_flag);
			return DP_FAILURE;
		}

		if (!sch_stat->parent.flag) {
			pr_err("DPM: %s wrong Sch:%d parent stat(%d):%s\n",
			       __func__, node_id, sch_stat->parent.flag,
			       "expect ACTIVE");
			return DP_FAILURE;
		}

		pid = get_parent_node(inst, node_id, flag);
		if (pid == DP_FAILURE) {
			pr_err("DPM: %s get_parent_node Sch:%d fail\n",
			       __func__, node_id);
			return DP_FAILURE;
		}

		idx = get_child_idx_node_id(inst, node_id, flag);
		if (idx == DP_FAILURE) {
			pr_err("DPM: %s get_child_idx_node_id Sch:%d fail\n",
			       __func__, node_id);
			return DP_FAILURE;
		}

		DP_DEBUG(DP_DBG_FLAG_QOS,
			 "parent:%d - Sch:%d child idx:%d\n",
			 pid, node_id, idx);

		CHILD(pid, idx).type = 0;
		CHILD(pid, idx).node_id = 0;
		CHILD(pid, idx).flag = PP_NODE_FREE;
		sch_stat->parent.type = 0;
		sch_stat->parent.flag = 0;
		sch_stat->parent.node_id = 0;
		sch_stat->p_flag &= ~PP_NODE_ACTIVE;
	} else {
		return DP_FAILURE;
	}

	return DP_SUCCESS;
}

/* Set Sch flag from PP_NODE_ALLOC to PP_NODE_ACTIVE
 */
static int node_sched_inc(int inst, int node_id, int flag)
{
	struct hal_priv *priv = HAL(inst);
	struct pp_sch_stat *sch_stat;
	int pid, idx;

	sch_stat = &priv->qos_sch_stat[node_id];

	if (flag & C_FLAG) {
		DP_DEBUG(DP_DBG_FLAG_QOS,
			 "parent Sch:%d\n", node_id);

		if (sch_stat->child_num && !PP_ACT(sch_stat->c_flag)) {
			pr_err("DPM: %s wrong Sch:%d stat(%d)/child_num(%d):%s\n",
			       __func__, node_id,
			       sch_stat->c_flag, sch_stat->child_num,
			       "expect ACTIVE and non-zero child_num");
			return DP_FAILURE;
		}

		if (!sch_stat->child_num && !PP_ALLOC(sch_stat->c_flag)) {
			pr_err("DPM: %s wrong Sch:%d stat(%d)/child_num(%d):%s\n",
			       __func__, node_id,
			       sch_stat->c_flag, sch_stat->child_num,
			       "expect ALLOC and zero child_num");
			return DP_FAILURE;
		}
		DP_DEBUG(DP_DBG_FLAG_QOS,
			 "increase sched[%d]'s child_num from %d to %d\n",
			 node_id,
			 sch_stat->child_num, sch_stat->child_num + 1);
		sch_stat->child_num++;
		sch_stat->c_flag |= PP_NODE_ACTIVE;
	} else if (flag & P_FLAG) {
		if (!PP_ALLOC(sch_stat->p_flag)) {
			pr_err("DPM: %s wrong Sch:%d stat(%d):expect ALLOC\n",
			       __func__, node_id, sch_stat->p_flag);
			return DP_FAILURE;
		}

		pid = get_parent_node(inst, node_id, flag);
		if (pid == DP_FAILURE) {
			pr_err("DPM: %s get_parent_node Sch:%d fail\n",
			       __func__, node_id);
			return DP_FAILURE;
		}

		idx = get_free_child_idx(inst, pid, 0);
		if (idx == DP_FAILURE) {
			pr_err("DPM: %s get_free_child_idx Sch:%d fail\n",
			       __func__, node_id);
			return DP_FAILURE;
		}

		DP_DEBUG(DP_DBG_FLAG_QOS,
			 "parent:%d - Sch:%d child idx:%d\n",
			 pid, node_id, idx);

		CHILD(pid, idx).node_id = node_id;
		CHILD(pid, idx).type = DP_NODE_SCH;
		CHILD(pid, idx).flag = PP_NODE_ACTIVE;
		sch_stat->p_flag |= PP_NODE_ACTIVE;
	} else {
		return DP_FAILURE;
	}

	return DP_SUCCESS;
}

/* Sanity check for child_num and both c and p_flag in alloc state
 * then reset Sch node.
 * Set Sched flag from PP_NODE_ALLOC to PP_NODE_FREE.
 */
static int node_sched_rst(int inst, int node_id, int flag)
{
	struct hal_priv *priv = HAL(inst);
	struct pp_sch_stat *sch_stat;
	int dp_port, resv_idx;

	sch_stat = &priv->qos_sch_stat[node_id];

	dp_port = sch_stat->dp_port;
	resv_idx = sch_stat->resv_idx;

	DP_DEBUG(DP_DBG_FLAG_QOS, "dp_port=%d Sch:%d resv_idx:%d\n",
		 dp_port, node_id, resv_idx);

	if (PP_RESV(sch_stat->p_flag))
		priv->resv[dp_port].resv_sched[resv_idx].flag = PP_NODE_FREE;

	dp_memset(sch_stat, 0, sizeof(*sch_stat));
	sch_stat->resv_idx = INV_RESV_IDX;

	return DP_SUCCESS;
}

/* Set Port flag from PP_NODE_ACTIVE to PP_NODE_ALLOC.
 */
static int node_port_dec(int inst, int node_id, int flag)
{
	struct hal_priv *priv = HAL(inst);
	struct cqm_deq_stat *port_stat;
	int phy_id;

	phy_id = get_cqm_deq_port_by_node(inst, node_id);
	if (phy_id == DP_FAILURE) {
		pr_err("DPM: %s get_cqm_deq_port_by_node %d fail\n",
		       __func__, node_id);
		return DP_FAILURE;
	}

	port_stat = &priv->deq_port_stat[phy_id];
	if (!port_stat->child_num || !PP_ACT(port_stat->flag)) {
		pr_err("DPM: %s wrong P[%d/%d] stat/child_num(%d):%s\n",
		       __func__, phy_id, node_id, port_stat->child_num,
		       "Expect ACTIVE or non-zero child_num");
		return DP_FAILURE;
	}
	DP_DEBUG(DP_DBG_FLAG_QOS,
		 "reduce port_node[%d]'s child_num from %d to %d\n",
		 node_id,
		 port_stat->child_num, port_stat->child_num - 1);
	port_stat->child_num--;
	if (!port_stat->child_num)
		port_stat->flag = PP_NODE_ALLOC;
	priv->qos_sch_stat[node_id].child_num = port_stat->child_num;

	return DP_SUCCESS;
}

/* Set Port flag from PP_NODE_ALLOC to PP_NODE_ACTIVE.
 */
static int node_port_inc(int inst, int node_id, int flag)
{
	struct hal_priv *priv = HAL(inst);
	struct cqm_deq_stat *port_stat;
	int phy_id;

	phy_id = get_cqm_deq_port_by_node(inst, node_id);
	if (phy_id == DP_FAILURE) {
		pr_err("DPM: %s get_cqm_deq_port_by_node %d fail\n",
		       __func__, node_id);
		return DP_FAILURE;
	}

	port_stat = &priv->deq_port_stat[phy_id];
	if (port_stat->child_num && !PP_ACT(port_stat->flag)) {
		pr_err("DPM: %s wrong P[%d/%d] stat/child_num(%d):%s\n",
		       __func__, phy_id, node_id, port_stat->child_num,
		       "Expect ACTIVE And Non-Zero child_num");
		return DP_FAILURE;
	}

	if (!port_stat->child_num && !PP_ALLOC(port_stat->flag)) {
		pr_err("DPM: %s wrong P[%d/%d] stat/child_num(%d):%s\n",
		       __func__, phy_id, node_id, port_stat->child_num,
		       "Expect ALLOC And Zero child_num");
		return DP_FAILURE;
	}
	DP_DEBUG(DP_DBG_FLAG_QOS,
		 "increase port_node[%d]'s child_num from %d to %d\n",
		 node_id,
		 port_stat->child_num, port_stat->child_num + 1);
	port_stat->child_num++;
	port_stat->flag = PP_NODE_ACTIVE;
	priv->qos_sch_stat[node_id].child_num = port_stat->child_num;

	return DP_SUCCESS;
}

/* Check for child_num and alloc flag
 * for port logical node_id, flag = DP_NODE_RST.
 * Set Port flag from PP_NODE_ALLOC to PP_NODE_FREE.
 */
static int node_port_rst(int inst, int node_id, int flag)
{
	struct hal_priv *priv = HAL(inst);
	struct cqm_deq_stat *port_stat;
	struct pp_sch_stat *sch_stat;
	int phy_id;

	phy_id = get_cqm_deq_port_by_node(inst, node_id);
	if (phy_id == DP_FAILURE) {
		pr_err("DPM: %s get_cqm_deq_port_by_node %d fail\n",
		       __func__, node_id);
		return DP_FAILURE;
	}

	port_stat = &priv->deq_port_stat[phy_id];
	if (port_stat->child_num || !PP_ALLOC(port_stat->flag)) {
		pr_err("DPM: %s wrong P[%d/%d] stat/child_num(%d):%s\n",
		       __func__, phy_id, node_id, port_stat->child_num,
		       "Expect ALLOC Or non-zero child_num");
		return DP_FAILURE;
	}

	sch_stat = &priv->qos_sch_stat[node_id];
	dp_memset(sch_stat, 0, sizeof(*sch_stat));
	dp_memset(port_stat, 0, sizeof(*port_stat));

	return DP_SUCCESS;
}

/* node_id is logical node id
 * if flag = DP_NODE_DEC
 *           update flag PP_NODE_ACTIVE to PP_NODE_ALLOC if needed
 *           update child info
 * else if flag = DP_NODE_INC
 *           update flag PP_NODE_ALLOC to PP_NODE_ACTIVE
 * else if flag = DP_NODE_RST
 *           update flag PP_NODE_ALLOC to PP_NODE_FREE
 *           reset table info
 * else return DP_FAILURE
 */
static int node_stat_update(int inst, int node_id, int flag)
{
	int node_type = get_node_type_by_node_id(inst, node_id);

	if (flag & DP_NODE_DEC) {
		if (Q_NODE(node_type))
			return node_queue_dec(inst, node_id, flag);
		else if (S_NODE(node_type))
			return node_sched_dec(inst, node_id, flag);
		else if (P_NODE(node_type))
			return node_port_dec(inst, node_id, flag);
	} else if (flag & DP_NODE_INC) {
		if (Q_NODE(node_type))
			return node_queue_inc(inst, node_id, flag);
		else if (S_NODE(node_type))
			return node_sched_inc(inst, node_id, flag);
		else if (P_NODE(node_type))
			return node_port_inc(inst, node_id, flag);
	} else if (flag & DP_NODE_RST) {
		if (Q_NODE(node_type))
			return node_queue_rst(inst, node_id, flag);
		else if (S_NODE(node_type))
			return node_sched_rst(inst, node_id, flag);
		else if (P_NODE(node_type))
			return node_port_rst(inst, node_id, flag);
	}

	return DP_FAILURE;
}

/* Checks for flag and input node
 * upon success allocate resource from reserve table
 * otherwise return failure
 */
static int dp_node_alloc_resv_pool(struct dp_node_alloc *node)
{
	struct hal_priv *priv = HAL(node->inst);
	struct pp_sch_stat *sch_stat;
	struct pp_queue_stat *q_stat;
	struct resv_sch *resv_sch;
	int i, num, id, phy_id;
	struct resv_q *resv_q;

	DP_DEBUG(DP_DBG_FLAG_QOS,
		 "inst=%d dp_port=%d num_resv_q=%d num_resv_sched=%d\n",
		 node->inst, node->dp_port,
		 priv->resv[node->dp_port].num_resv_q,
		 priv->resv[node->dp_port].num_resv_sched);

	if (Q_NODE(node->type)) {
		num = priv->resv[node->dp_port].num_resv_q;
		if (!num)
			return DP_FAILURE;

		DP_DEBUG(DP_DBG_FLAG_QOS, "Look for reserved queue\n");

		resv_q = priv->resv[node->dp_port].resv_q;
		for (i = 0; i < num; i++) {
			if (!PP_FREE(resv_q[i].flag))
				continue;

			id = resv_q[i].id;
			phy_id = resv_q[i].physical_id;

			q_stat = &priv->qos_queue_stat[phy_id];
			q_stat->resv_idx = i;
			q_stat->node_id = id;
			q_stat->dp_port = node->dp_port;
			q_stat->flag = PP_NODE_RESERVE | PP_NODE_ALLOC;

			sch_stat = &priv->qos_sch_stat[id];
			sch_stat->resv_idx = i;
			sch_stat->node.type = DP_NODE_QUEUE;
			sch_stat->dp_port = node->dp_port;
			sch_stat->p_flag = PP_NODE_RESERVE | PP_NODE_ALLOC;

			node->id.q_id = phy_id;
			resv_q[i].flag = PP_NODE_ALLOC;
			priv->qos_sch_stat[id].node.type = DP_NODE_QUEUE;
			priv->qos_sch_stat[id].node.id.q_id = phy_id;
			priv->qos_sch_stat[id].used = 1;
			DP_DEBUG(DP_DBG_FLAG_QOS, "Q[%d/%d]:resv_idx=%d\n",
				 phy_id, id, q_stat->resv_idx);
			break; /* I think need to add break */
		}
	} else if (S_NODE(node->type)) {
		num = priv->resv[node->dp_port].num_resv_sched;
		if (!num)
			return DP_FAILURE;

		DP_DEBUG(DP_DBG_FLAG_QOS, "Look for reserved scheduler\n");

		resv_sch = priv->resv[node->dp_port].resv_sched;
		for (i = 0; i < num; i++) {
			if (!PP_FREE(resv_sch[i].flag))
				continue;
			id = resv_sch[i].id;

			sch_stat = &priv->qos_sch_stat[id];
			sch_stat->resv_idx = i;
			sch_stat->child_num = 0;
			sch_stat->node.type = DP_NODE_SCH;
			sch_stat->dp_port = node->dp_port;
			sch_stat->c_flag = PP_NODE_RESERVE | PP_NODE_ALLOC;
			sch_stat->p_flag = PP_NODE_RESERVE | PP_NODE_ALLOC;

			node->id.sch_id = id;
			resv_sch[i].flag = PP_NODE_ALLOC;
			priv->qos_sch_stat[id].node.type = DP_NODE_SCH;
			priv->qos_sch_stat[id].node.id.sch_id = id;
			priv->qos_sch_stat[id].used = 1;

			DP_DEBUG(DP_DBG_FLAG_QOS, "Sch:%d: resv_idx=%d\n",
				 resv_sch[i].id, sch_stat->resv_idx);
			break; /* I think need to add break */
		}
	} else {
		pr_err("DPM: %s invalid node type\n", __func__);
		return DP_FAILURE;
	}

	return DP_SUCCESS;
}

/* Checks for flag and input node
 * upon success allocate resource from global table
 * otherwise return failure
 */
static int dp_node_alloc_global_pool(struct dp_node_alloc *node)
{
	struct hal_priv *priv = HAL(node->inst);
	struct pp_sch_stat *sch_stat;
	struct pp_queue_stat *q_stat;
	int id, phy_id;

	if (Q_NODE(node->type)) {
		if (dp_qos_queue_allocate_id_phy(priv->qdev, &id, &phy_id)) {
			pr_err("DPM: %s dp_qos_queue_allocate_id_phy fail\n",
			       __func__);
			return DP_FAILURE;
		}

		DP_DEBUG(DP_DBG_FLAG_QOS, "alloc Q[%d/%d]\n",
			 phy_id, id);
		q_stat = &priv->qos_queue_stat[phy_id];
		q_stat->blocked = 0;
		q_stat->node_id = id;
		q_stat->flag = PP_NODE_ALLOC;
		q_stat->resv_idx = INV_RESV_IDX;
		q_stat->dp_port = node->dp_port;

		sch_stat = &priv->qos_sch_stat[id];
		sch_stat->node.type = DP_NODE_QUEUE;
		sch_stat->p_flag = PP_NODE_ALLOC;
		sch_stat->resv_idx = INV_RESV_IDX;
		sch_stat->dp_port = node->dp_port;
		sch_stat->parent.node_id = priv->ppv4_drop_p;

		node->id.q_id = phy_id;
		priv->qos_sch_stat[id].node.type = DP_NODE_QUEUE;
		priv->qos_sch_stat[id].node.id.q_id = phy_id;
		priv->qos_sch_stat[id].used = 1;

	} else if (S_NODE(node->type)) {
		if (dp_qos_sched_allocate(priv->qdev, &id)) {
			pr_err("DPM: %s dp_qos_sched_allocate fail\n", __func__);
			return DP_FAILURE;
		}

		DP_DEBUG(DP_DBG_FLAG_QOS, "alloc Sch:%d\n",
			 id);
		sch_stat = &priv->qos_sch_stat[id];
		sch_stat->child_num = 0;
		sch_stat->node.type = DP_NODE_SCH;
		sch_stat->c_flag = PP_NODE_ALLOC;
		sch_stat->p_flag = PP_NODE_ALLOC;
		sch_stat->resv_idx = INV_RESV_IDX;
		sch_stat->dp_port = node->dp_port;

		node->id.sch_id = id;
		priv->qos_sch_stat[id].node.type = DP_NODE_SCH;
		priv->qos_sch_stat[id].node.id.sch_id = id;
		priv->qos_sch_stat[id].used = 1;
	} else {
		pr_err("DPM: Unknown node type %d\n", node->type);
		return DP_FAILURE;
	}

	return DP_SUCCESS;
}

/* Allocate QoS port.
 * Upon Success return QoS port logical id
 * else return DP_FAILURE
 */
static int dp_alloc_qos_port(struct dp_node_alloc *node, int flag, bool *new)
{
	struct pp_qos_port_conf port_cfg;
	struct cqm_port_info *port_info;
	struct cqm_deq_stat *port_stat;
	struct pp_sch_stat *sch_stat;
	struct hal_priv *priv = HAL(node->inst);
	int cqm_deq_port;
	u32 qos_port;

	*new = false;
	cqm_deq_port = node->id.cqm_deq_port;
	DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL,
		 "inst=%d dp_port=%d cqm_deq_port=%d\n",
		 node->inst, node->dp_port, cqm_deq_port);

	if (cqm_deq_port == DP_NODE_AUTO_ID) {
		pr_err("DPM: Invalid cqm_deq_port: %d\n", cqm_deq_port);
		return DP_FAILURE;
	}

	port_stat = &priv->deq_port_stat[cqm_deq_port];
	if (!PP_FREE(port_stat->flag)) {
		DP_DEBUG(DP_DBG_FLAG_QOS, "cqm_deq_port[%d] already init\n",
			 cqm_deq_port);
		return port_stat->node_id;
	}

	if (dp_qos_port_allocate(priv->qdev, cqm_deq_port, &qos_port, node->inst)) {
		pr_err("DPM: dp_qos_port_allocate:%d fail\n", cqm_deq_port);
		return DP_FAILURE;
	}

	/* Configure QoS port */
	port_info = get_dp_deqport_info(node->inst, cqm_deq_port);
	dp_qos_port_conf_set_default(&port_cfg);
	port_cfg.credit = port_info->tx_pkt_credit;
	port_cfg.ring_size = port_info->tx_ring_size;
	port_cfg.ring_address = (unsigned long)port_info->txpush_addr_qos;
	port_cfg.packet_credit_enable = port_cfg.credit ? 1 : 0;
	port_cfg.port_parent_prop.arbitration = PP_QOS_ARBITRATION_WSP;
	if (port_cfg.disable) /* try to enable deq port */
		port_cfg.disable = 0;

	if (dp_qos_port_set(priv->qdev, qos_port, &port_cfg)) {
		pr_err("DPM: %s dp_qos_port_set port[%d/%d] fail\n",
		       __func__, cqm_deq_port, qos_port);
		dp_qos_port_remove(priv->qdev, qos_port, node->inst);
		return DP_FAILURE;
	}

	*new = true;
	sch_stat = &priv->qos_sch_stat[qos_port];
	sch_stat->child_num = 0;
	sch_stat->node.type = DP_NODE_PORT;

	port_stat->node_id = qos_port;
	port_stat->flag = PP_NODE_ALLOC;
	port_stat->disabled = port_cfg.disable;

	DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL,
		 "ok: port=%d/%d for dp_port=%d\n",
		 cqm_deq_port, qos_port, node->dp_port);

	return qos_port;
}

/* _dp_node_alloc API
 * Allocate node from global/reserved resource based on flag
 */
int _dp_node_alloc(struct dp_node_alloc *node, int flag)
{
	DP_DEBUG(DP_DBG_FLAG_QOS, "flag %x\n", flag);

	if (flag & DP_ALLOC_RESV_ONLY)
		return dp_node_alloc_resv_pool(node);

	if (flag & DP_ALLOC_GLOBAL_ONLY)
		return dp_node_alloc_global_pool(node);

	if (flag & DP_ALLOC_GLOBAL_FIRST) {
		if (dp_node_alloc_global_pool(node))
			return dp_node_alloc_resv_pool(node);
		else
			return DP_SUCCESS;
	}

	/* Default */
	if (dp_node_alloc_resv_pool(node))
		return dp_node_alloc_global_pool(node);
	else
		return DP_SUCCESS;
}

/* Flush and unlink queue from its parent.
 * Check parent's child list if empty free parent recursively
 */
static int dp_smart_free_from_child(
	struct dp_node_alloc *node, int flag)
{
	struct hal_priv *priv = HAL(node->inst);
	struct pp_queue_stat *q_stat;
	struct pp_sch_stat *sch_stat = NULL;
	bool need_repeat = false;
	struct dp_node_alloc new_node = {0};

	DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL, "type=%d qid=%d\n",
		 node->type, node->id.q_id);

	if (Q_NODE(node->type)) {
		q_stat = &priv->qos_queue_stat[node->id.q_id];
		sch_stat = &priv->qos_sch_stat[q_stat->node_id];
	} else if (S_NODE(node->type)) {
		sch_stat = &priv->qos_sch_stat[node->id.sch_id];
	}
	/* retrieve parent scheduler information */
	if (sch_stat && S_NODE(sch_stat->parent.type)) {
		new_node.inst = node->inst;
		new_node.dp_port = node->dp_port;
		new_node.type = sch_stat->parent.type;
		new_node.id.sch_id = sch_stat->parent.node_id;
		need_repeat = true;
	}

	if (Q_NODE(node->type)) {
		if (!is_qid_valid(node->id.q_id))
			return DP_FAILURE;
		/* call normal node_free without DP_NODE_SMART_FREE flag */
		if (_dp_node_free(node, 0)) {
			pr_err("DPM: %s _dp_node_free qid:%d fail\n",
			       __func__, node->id.q_id);
			return DP_FAILURE;
		}
	} else if (S_NODE(node->type)) {
		if (!is_sch_valid(node->id.sch_id))
			return DP_FAILURE;
		/* call normal node_free without DP_NODE_SMART_FREE flag */
		if (_dp_node_free(node, 0)) {
			pr_err("DPM: %s _dp_node_free Sch:%d fail\n",
			       __func__, node->id.sch_id);
			return DP_FAILURE;
		}
	} else if (P_NODE(node->type)) {
		pr_err("DPM: Not allow free deq_port[%d], suppose done by dpm itself\n",
			node->id.cqm_deq_port);
		return DP_FAILURE;
	}

	if (need_repeat && !priv->qos_sch_stat[new_node.id.sch_id].child_num)
		return dp_smart_free_from_child(&new_node, DP_NODE_SMART_FREE);
	return DP_SUCCESS;
}

/* Reset parent to free state
 * Check parent's child list and free all resources recursively
 */
int dp_free_children_via_parent(struct dp_node_alloc *node, int flag)
{
	struct hal_priv *priv = HAL(node->inst);
	struct dp_node_alloc tmp = {0};
	struct dp_node_link info = {0};
	struct pp_sch_stat *sch_stat;
	int i, id, pid, node_id;

	if (P_NODE(node->type)) {
		if (!is_deqport_valid(node->id.cqm_deq_port))
			return DP_FAILURE;

		id = priv->deq_port_stat[node->id.cqm_deq_port].node_id;
		sch_stat = &priv->qos_sch_stat[id];

		DP_DEBUG(DP_DBG_FLAG_QOS, "Port:%d has child num:%d\n",
			 node->id.cqm_deq_port, sch_stat->child_num);

		for (i = 0; i < DP_MAX_CHILD_PER_NODE; i++) {
			if (!PP_ACT(CHILD(id, i).flag))
				continue;

			node_id = CHILD(id, i).node_id;
			tmp.type = CHILD(id, i).type;
			if (S_NODE(tmp.type))
				tmp.id.sch_id = node_id;
			else
				tmp.id.q_id =
					get_qid_by_node(node->inst, node_id);

			if (dp_free_children_via_parent(&tmp, 0)) {
				pr_err("DPM: %s Port:%d child:%d type:%d fail\n",
				       __func__, node->id.cqm_deq_port,
				       node_id, CHILD(id, i).type);
				return DP_FAILURE;
			}
		}

		if (!sch_stat->child_num) {
			if (_dp_node_free(node, 0)) {
				pr_err("DPM: %s _dp_node_free Port:%d fail\n",
				       __func__, node->id.cqm_deq_port);
				return DP_FAILURE;
			}
		}

		DP_DEBUG(DP_DBG_FLAG_QOS, "Port:%d free\n",
			 node->id.cqm_deq_port);
	} else if (S_NODE(node->type)) {
		if (!is_sch_valid(node->id.sch_id))
			return DP_FAILURE;

		id = node->id.sch_id;
		sch_stat = &priv->qos_sch_stat[id];

		DP_DEBUG(DP_DBG_FLAG_QOS, "Sch:%d has child num:%d\n",
			 node->id.sch_id, sch_stat->child_num);

		for (i = 0; i < DP_MAX_CHILD_PER_NODE; i++) {
			if (!PP_ACT(CHILD(id, i).flag))
				continue;

			node_id = CHILD(id, i).node_id;
			tmp.type = CHILD(id, i).type;
			if (S_NODE(tmp.type))
				tmp.id.q_id = node_id;
			else
				tmp.id.q_id =
					get_qid_by_node(node->inst, node_id);

			if (dp_free_children_via_parent(&tmp, 0)) {
				pr_err("DPM: %s Sch:%d child:%d type:%d fail\n",
				       __func__, node->id.sch_id,
				       node_id, CHILD(id, i).type);
				return DP_FAILURE;
			}

			DP_DEBUG(DP_DBG_FLAG_QOS,
				 "Sch:%d freed child:%d\n",
				 node->id.sch_id, node_id);
		}

		DP_DEBUG(DP_DBG_FLAG_QOS, "Sch:%d's all children:%d freed!\n",
			 node->id.sch_id, sch_stat->child_num);

		if (!sch_stat->child_num) {
			if (_dp_node_free(node, 0)) {
				pr_err("DPM: %s _dp_node_free Sch:%d fail\n",
				       __func__, node->id.sch_id);
				return DP_FAILURE;
			}
		}
	} else if (Q_NODE(node->type)) {
		if (q_node_chk(priv, node->id.q_id))
			return DP_FAILURE;

		pid = priv->qos_queue_stat[node->id.q_id].node_id;
		sch_stat = &priv->qos_sch_stat[pid];

		info.node_type = node->type;
		info.node_id.q_id = node->id.q_id;
		info.p_node_type = sch_stat->parent.type;
		info.p_node_id.q_id = sch_stat->parent.node_id;

		if (_dp_node_unlink(&info, 0)) {
			pr_err("DPM: %s _dp_node_unlink qid:%d fail\n",
			       __func__, node->id.q_id);
			return DP_FAILURE;
		}

		if (_dp_node_free(node, 0)) {
			pr_err("DPM: %s _dp_node_free qid:%d fail\n",
			       __func__, node->id.q_id);
			return DP_FAILURE;
		}

		DP_DEBUG(DP_DBG_FLAG_QOS, "Q:%d Parent:%d type:%d\n",
			 node->id.q_id,
			 info.p_node_id.q_id, info.p_node_type);
	} else {
		pr_err("DPM: %s invalid node type:%d\n", __func__, node->type);
		return DP_FAILURE;
	}

	return DP_SUCCESS;
}

/* If node is linked, unlink it.
 * If node is a reserved node, return to the reserved node table
 * and mark node as Free in this device's reserved node table.
 * If node is a reserved node, return to the system global table
 * abd mark node as Free in system global table.
 */
int _dp_node_free(struct dp_node_alloc *node, int flag)
{
	int node_id = -1, phy_id, pid, f = DP_NODE_DEC, res = DP_FAILURE;
	struct hal_priv *priv = HAL(node->inst);
	struct cqm_deq_stat *port_stat;
	struct pp_queue_stat *q_stat;
	struct pp_sch_stat *sch_stat;
	struct local {
		struct pp_qos_queue_conf q_cfg;
		struct dp_node_link info;
		struct pp_qos_sched_conf sch_cfg;
		struct dp_node_child child;
	};
	struct local *l;

	DP_DEBUG(DP_DBG_FLAG_QOS, "%s %d flag=%d\n",
		 node_type_str(node->type), node->id.q_id, flag);
	if (flag == DP_NODE_SMART_FREE) {
		/* Don't pass flag */
		if (dp_smart_free_from_child(node, 0)) {
			pr_err("DPM: %s dp_smart_free_from_child fail\n", __func__);
			return DP_FAILURE;
		}
		return DP_SUCCESS;
	}
	l = dp_kzalloc(sizeof(*l), GFP_ATOMIC);
	if (!l)
		return DP_FAILURE;

	if (Q_NODE(node->type)) {
		phy_id = node->id.q_id;
		if (is_q_node_free(priv, phy_id)) {
			DP_DEBUG(DP_DBG_FLAG_QOS, "DPM: q[%d] already freed and cannot free again\n",
				 phy_id);
			res = DP_SUCCESS;
			goto exit;
		}

		q_stat = &priv->qos_queue_stat[phy_id];
		node_id = q_stat->node_id;

		sch_stat = &priv->qos_sch_stat[node_id];
		pid = sch_stat->parent.node_id;

		l->info.inst = node->inst;
		l->info.node_id = node->id;
		l->info.node_type = node->type;
		if (_dp_node_unlink(&l->info, 0)) {
			pr_err("DPM: %s _dp_node_unlink Q:%d fail\n",
			       __func__, phy_id);
			goto exit;
		}

		if (PP_ALLOC(q_stat->flag) && !PP_RESV(q_stat->flag)) {
			if (dp_qos_queue_remove(priv->qdev, node_id)) {
				pr_err("DPM: %s dp_qos_queue_remove Q:%d fail\n",
				       __func__, node_id);
				goto exit;
			}
		}

		if (PP_ACT(sch_stat->p_flag)) {
			/* update current sch_stat */
			if (node_stat_update(node->inst, node_id, f)) {
				pr_err("DPM: %s node_stat_update rm Q:%d fail\n",
				       __func__, node_id);
				goto exit;
			}
			/* update parernt sch_stat */
			if (node_stat_update(node->inst, pid, f | C_FLAG)) {
				pr_err("DPM: %s node_stat_update parent:%d fail\n",
				       __func__, pid);
				goto exit;
			}
		}
		/* reset this node */
		if (node_stat_update(node->inst, node_id, DP_NODE_RST)) {
			pr_err("DPM: %s node_stat_update rst Q:%d fail\n",
			       __func__, node_id);
			goto exit;
		}

		DP_DEBUG(DP_DBG_FLAG_QOS, "Q[%d/%d] remove and update stat\n",
			 phy_id, node_id);
		priv->qos_sch_stat[node_id].used = 0;

		/* Reserved Q attach to drop port */
		if (PP_RESV(q_stat->flag)) {
			dp_qos_queue_conf_set_default(&l->q_cfg);
			l->q_cfg.queue_child_prop.parent = priv->ppv4_drop_p;
			if (dp_qos_queue_set(priv->qdev, node_id, &l->q_cfg)) {
				pr_err("DPM: %s %s Q:%d to parent:%d fail\n",
				       __func__, "dp_qos_queue_set",
				       node_id, priv->ppv4_drop_p);
				goto exit;
			}

			sch_stat->parent.node_id = priv->ppv4_drop_p;
			q_stat->blocked = l->q_cfg.blocked;
			DP_DEBUG(DP_DBG_FLAG_QOS,
				 "Q[%d/%d] attached to drop port:%d\n",
				 phy_id, node_id, priv->ppv4_drop_p);
		}
		res = DP_SUCCESS;
	} else if (S_NODE(node->type)) {
		node_id = node->id.sch_id;
		sch_stat = &priv->qos_sch_stat[node_id];

		if (sch_stat->child_num || PP_ACT(sch_stat->c_flag)) {
			pr_err("DPM: %s Sch_id=%d still have child_num=%d and c_flag=%d\n",
			       __func__, node_id, sch_stat->child_num,
			       sch_stat->c_flag);
			goto exit;
		}
		if (PP_FREE(sch_stat->p_flag)) {
			/* already free and nothing to do */
			res = DP_SUCCESS;
			goto exit;
		}
		/* get parernt id */
		pid = priv->qos_sch_stat[node_id].parent.node_id;
		if (!PP_RESV(sch_stat->p_flag)) {
			/* free the sched if it is not from reserved pool */
			if (dp_qos_sched_remove(priv->qdev, node_id)) {
				pr_err("DPM: %s dp_qos_sched_remove\n", __func__);
				goto exit;
			}
		}
		if (PP_ACT(sch_stat->p_flag)) {
			/* update current sched status */
			if (node_stat_update(node->inst, node_id, f | P_FLAG)) {
				pr_err("DPM: %s node_stat_update rm Sch:%d fail\n",
				       __func__, node_id);
				goto exit;
			}
			/* update parernt node's status */
			if (node_stat_update(node->inst, pid, f | C_FLAG)) {
				pr_err("DPM: %s node_stat_update rm parent:%d fail\n",
				       __func__, pid);
				goto exit;
			}
		}
		f = DP_NODE_RST;
		if (node_stat_update(node->inst, node_id, f | P_FLAG)) {
			pr_err("DPM: %s node_stat_update rst Sch:/%d fail\n",
			       __func__, node_id);
			goto exit;
		}
		priv->qos_sch_stat[node_id].used = 0;
		DP_DEBUG(DP_DBG_FLAG_QOS, "Sch:%d removed and stat updated\n",
			 node_id);

		/* Reserved Sch attach to drop port */
		if (PP_RESV(sch_stat->p_flag)) {
			dp_qos_sched_conf_set_default(&l->sch_cfg);
			l->sch_cfg.sched_child_prop.parent = priv->ppv4_drop_p;
			if (dp_qos_sched_set(priv->qdev, node_id, &l->sch_cfg)) {
				pr_err("DPM: %s %s Sch:%d to parent:%d fail\n",
				       __func__, "dp_qos_sched_set",
				       node_id, priv->ppv4_drop_p);
				goto exit;
			}

			DP_DEBUG(DP_DBG_FLAG_QOS,
				 "Sch:%d attached to drop port:%d\n",
				 node_id, priv->ppv4_drop_p);
		}
		res = DP_SUCCESS;
	} else if (P_NODE(node->type)) {
		phy_id = node->id.cqm_deq_port;
		port_stat = &priv->deq_port_stat[phy_id];
		node_id = port_stat->node_id;

		if (port_stat->child_num) {
			pr_err("DPM: %s P:%d still have child %d and canot free\n",
			       __func__, phy_id, port_stat->child_num);
			/* try to get child information for debugging purpose */
			l->child.inst = node->inst;
			l->child.type = DP_NODE_PORT;
			l->child.id.cqm_deq_port = phy_id;
			if (_dp_children_get(&l->child, 0)) {
				pr_err("DPM: _dp_children_get fail: deq_port=%d\n",
				       phy_id);
			} else {
				DP_DUMP("num of child via dp_children_get:%d\n",
					l->child.num);
				DP_DUMP("  child[0]: type=%d\n",
					l->child.child[0].type);
				DP_DUMP("  child[0]: id  =%d\n",
					l->child.child[0].id.sch_id);
			}
			goto exit;
		}

		if (PP_ACT(port_stat->flag)) {
			if (node_stat_update(node->inst, node_id, f))
				pr_err("DPM: %s node_stat_update rm P:%d fail\n",
				       __func__, phy_id);
		} else if (PP_ALLOC(port_stat->flag)) {
#if LATE_FREE_DEQ_PORT
#else
			/* No reset API call for port now in order to be
			 * back-compatible with PON/PPA QOS manager
			 * unexpected handling at present
			 * We will try to reset port at de_register_dev for
			 * workaround
			 */
			/* free the PP QOS Port */
			f = DP_NODE_RST;
			if (node_stat_update(node->inst, node_id, f | P_FLAG)) {
				pr_err("DPM: %s node_stat_update rst port:/%d fail\n",
				       __func__, node_id);
				goto exit;
			}
			if (dp_qos_port_remove(priv->qdev, node_id, node->inst)) {
				pr_err("DPM: %s dp_qos_port_remove\n", __func__);
				goto exit;
			}
			priv->qos_sch_stat[node_id].used = 0;
#endif
			res = DP_SUCCESS;
		} else {
			pr_err("DPM: %s unexpected P:%d flag %d\n",
			       __func__, phy_id, port_stat->flag);
		}
	} else {
		pr_err("DPM: Unexpect node type %d\n", node->type);
	}

exit:
	kfree(l);
	if (res)
		pr_err("DPM: %s node:%d fail\n", __func__, node_id);

	return res;
}

int dp_free_deq_port(int inst, u8 ep, struct dp_dev_data *data,
			     uint32_t flags)
{
#if LATE_FREE_DEQ_PORT
	struct hal_priv *priv = (struct hal_priv *)dp_port_prop[inst].priv_hal;
	struct pmac_port_info *port_info = get_dp_port_info(inst, ep);
	struct cqm_deq_stat *port_stat;
	int i, phy_id, f, node_id;

	for (i = 0; i < port_info->deq_port_num; i++) {
		phy_id = port_info->deq_ports[i];
		port_stat = &priv->deq_port_stat[phy_id];

		if (!PP_ALLOC(port_stat->flag))
			continue;

		/* free the PP QOS Port:
		 * after succesful DP_NODE_RST, its related table entry will be
		 * reset. So here need t save its node_id first
		 */
		node_id = port_stat->node_id;

		f = DP_NODE_RST;
		if (node_stat_update(inst, node_id, f | P_FLAG)) {
			pr_err("DPM: %s:node_stat_update rst port fail:%d/%d\n",
			       __func__, phy_id, node_id);
			continue;
		}
		if (dp_qos_port_remove(priv->qdev, node_id, inst)) {
			pr_err("DPM: %s:dp_qos_port_remove fail %d/%d\n", __func__,
			       phy_id, node_id);
			continue;
		}
		priv->qos_sch_stat[node_id].used = 0;
	}
#endif
	return 0;
}

/* Create parent node if needed.
 * upon Success return parent node id
 * else return DP_FAILURE
 */
static int dp_qos_parent_get(struct dp_node_link *info, int flag, bool *new_pid)
{
	struct dp_node_alloc *node;
	int ret;

	node = dp_kzalloc(sizeof(*node), GFP_ATOMIC);
	if (!node)
		return DP_FAILURE;

	if (S_NODE(info->p_node_type)) {
		if (info->p_node_id.sch_id == DP_NODE_AUTO_ID) {
			node->inst = info->inst;
			node->dp_port = info->dp_port;
			node->type = info->p_node_type;
			if (_dp_node_alloc(node, flag)) {
				kfree(node);
				pr_err("DPM: %s _dp_node_alloc Sch  fail\n",
				       __func__);
				return DP_FAILURE;
			}
			*new_pid = true;
			info->p_node_id = node->id;
		}
		*new_pid = false;
		ret = info->p_node_id.sch_id;
		kfree(node);
		return ret;
	} else if (P_NODE(info->p_node_type)) {
		node->inst = info->inst;
		node->dp_port = info->dp_port;
		node->id = info->cqm_deq_port;
		node->type = info->p_node_type;
		ret = dp_alloc_qos_port(node, flag, new_pid);
		kfree(node);
		return ret;
	}

	kfree(node);
	return DP_FAILURE;
}

/* Return parent's arbi of given node
 * else return DP_FAILURE
 */
static int get_parent_arbi(int inst, int node_id, int flag)
{
	struct hal_priv *priv = HAL(inst);
	struct pp_sch_stat *sch_stat;
	int pid, arbi;

	sch_stat = &priv->qos_sch_stat[node_id];
	if (PP_FREE(sch_stat->parent.flag)) {
		pr_err("DPM: %s parent not set for node id=%d\n", __func__,
		       node_id);
		return DP_FAILURE;
	}

	pid = sch_stat->parent.node_id;

	if (S_NODE(sch_stat->parent.type)) {
		struct pp_qos_sched_conf sch_cfg = {0};

		if (dp_qos_sched_conf_get(priv->qdev, pid, &sch_cfg)) {
			pr_err("DPM: %s dp_qos_sched_conf_get fail\n", __func__);
			return DP_FAILURE;
		}

		arbi = pp2dp_arbi(sch_cfg.sched_parent_prop.arbitration);
		if (arbi == DP_FAILURE)
			pr_err("DPM: %s wrong pp_arbitrate: %d for %s:%d\n",
			       __func__, sch_cfg.sched_parent_prop.arbitration,
			       node_type_str(sch_stat->node.type),
			       node_id);
	} else if (P_NODE(sch_stat->parent.type)) {
		struct pp_qos_port_conf port_cfg = {0};

		if (dp_qos_port_conf_get(priv->qdev, pid, &port_cfg)) {
			pr_err("DPM: %s dp_qos_port_conf_get fail\n", __func__);
			return DP_FAILURE;
		}

		arbi = pp2dp_arbi(port_cfg.port_parent_prop.arbitration);
		if (arbi == DP_FAILURE)
			pr_err("DPM: %s wrong pp_arbitrate: %d for %s:%d\n",
			       __func__, port_cfg.port_parent_prop.arbitration,
			       node_type_str(sch_stat->node.type),
			       node_id);
	} else {
		pr_err("DPM: %s invalid parent type:0x%x for node:%d\n",
		       __func__, sch_stat->parent.type, node_id);
		return DP_FAILURE;
	}

	return arbi;
}

/* Get link info.
 * Upon Success return DP_SUCCESS
 * else return DP_FAILURE
 */
int _dp_node_link_get(struct dp_node_link *info, int flag)
{
	struct hal_priv *priv = HAL(info->inst);
	struct pp_queue_stat *q_stat;
	struct pp_sch_stat *sch_stat;
	int node_id, pid;

	if (Q_NODE(info->node_type)) {
		struct pp_qos_queue_conf q_cfg = {0};

		q_stat = &priv->qos_queue_stat[info->node_id.q_id];
		node_id = q_stat->node_id;
		if (dp_qos_queue_conf_get(priv->qdev, node_id, &q_cfg)) {
			pr_err("DPM: %s dp_qos_queue_conf_get fail\n", __func__);
			return DP_FAILURE;
		}

		pid = q_cfg.queue_child_prop.parent;
		if (!pid || !PP_ACT(q_stat->flag)) {
			pr_err("DPM: %s parent not set q\n", __func__);
			return DP_FAILURE;
		}

		info->p_node_id.q_id = pid;
		info->arbi = get_parent_arbi(info->inst, node_id, flag);
		if ((int)info->arbi == DP_FAILURE) {
			pr_err("DPM: %s invalid arbitration value\n", __func__);
			return DP_FAILURE;
		}

		if (info->arbi == ARBITRATION_WRR)
			info->prio_wfq = q_cfg.queue_child_prop.wrr_weight;
		else if (info->arbi == ARBITRATION_WSP)
			info->prio_wfq = q_cfg.queue_child_prop.priority;
		else
			pr_warn("Invalid arbi %s %d\n", __func__, info->arbi);
	} else if (S_NODE(info->node_type)) {
		struct pp_qos_sched_conf sch_cfg = {0};

		sch_stat = &priv->qos_sch_stat[info->node_id.sch_id];

		if (dp_qos_sched_conf_get(priv->qdev, info->node_id.sch_id,
					  &sch_cfg)) {
			pr_err("DPM: %s dp_qos_sched_conf_get Sch:%d fail\n",
			       __func__, info->node_id.sch_id);
			return DP_FAILURE;
		}

		pid = sch_cfg.sched_child_prop.parent;
		if (!pid || !PP_ACT(sch_stat->p_flag)) {
			pr_err("DPM: %s parent not set for sch\n", __func__);
			return DP_FAILURE;
		}

		info->arbi = pp2dp_arbi(sch_cfg.sched_parent_prop.arbitration);
		if ((int)info->arbi == DP_FAILURE) {
			pr_err("DPM: %s invalid arbitration value\n", __func__);
			return DP_FAILURE;
		}

		info->p_node_id.sch_id = pid;
		if (info->arbi == ARBITRATION_WRR)
			info->prio_wfq = sch_cfg.sched_child_prop.wrr_weight;
		else if (info->arbi == ARBITRATION_WSP)
			info->prio_wfq = sch_cfg.sched_child_prop.priority;
		else
			pr_warn("Wrong arbi %s %d\n", __func__, info->arbi);
	} else {
		pr_err("DPM: %s invalid node type %d\n", __func__, info->node_type);
		return DP_FAILURE;
	}

	info->leaf = 0;
	info->p_node_type = get_node_type_by_node_id(info->inst, pid);

	return DP_SUCCESS;
}

static int dp_map_qid_to_cqmdeq(struct dp_node_link *info, int flag)
{
	struct local {
		struct dp_node_child node;
		struct dp_qos_link lnk;
		struct dp_node_link node_info;
	};
	struct local *l;
	int i, valid_child_cnt = 0;

	l = dp_kzalloc(sizeof(*l), GFP_ATOMIC);
	if (!l)
		return DP_FAILURE;
	if (Q_NODE(info->node_type)) {
		l->lnk.q_id = info->node_id.q_id;
		if (_dp_link_get(&l->lnk, 0)) {
			pr_err("DPM: %s _dp_link_get Q:%d fail\n",
			       __func__, l->lnk.q_id);
			kfree(l);
			return DP_FAILURE;
		}

		DP_DEBUG(DP_DBG_FLAG_QOS, "Parent P:%d\n",
			 l->lnk.cqm_deq_port);

		CBM_OPS(info->inst, cqm_qid2ep_map_set, l->lnk.q_id,
			l->lnk.cqm_deq_port);

		DP_DEBUG(DP_DBG_FLAG_QOS, "%s qid:%d, dq_port:%d\n",
			 "cqm_qid2ep_map_set", l->lnk.q_id, l->lnk.cqm_deq_port);
	} else if (S_NODE(info->node_type)) {

		l->node.type = info->node_type;
		l->node.id.sch_id = info->node_id.sch_id;
		if (_dp_children_get(&l->node, flag))
			DP_DEBUG(DP_DBG_FLAG_QOS,
				 "_dp_children_get fail\n");

		DP_DEBUG(DP_DBG_FLAG_QOS, "Node[%d] has %d Children\n",
			 l->node.id.q_id, l->node.num);

		for (i = 0; i < MAX_PP_CHILD_PER_NODE; i++) {
			if (l->node.child[i].type == DP_NODE_UNKNOWN)
				continue;
			valid_child_cnt++;
			if (valid_child_cnt > l->node.num)
				continue;
			DP_DEBUG(DP_DBG_FLAG_QOS,
				 "schedId: %d, childArrayIdx: %d, childNodeType: %d, childId: %d\n",
					l->node.id.sch_id, i,
					l->node.child[i].type,
					l->node.child[i].id.q_id);
			if (Q_NODE(l->node.child[i].type)) {
				l->lnk.q_id = l->node.child[i].id.q_id;
				if (_dp_link_get(&l->lnk, 0)) {
					pr_err("DPM: %s _dp_link_get fail\n",
					       __func__);
					kfree(l);
					return DP_FAILURE;
				}

				DP_DEBUG(DP_DBG_FLAG_QOS, "Parent P:%d\n",
					 l->lnk.cqm_deq_port);

				CBM_OPS(info->inst, cqm_qid2ep_map_set,
					l->lnk.q_id, l->lnk.cqm_deq_port);

				DP_DEBUG(DP_DBG_FLAG_QOS,
					 "%s Q:%d - child:%d to P:%d\n",
					 "cqm_qid2ep_map_set", l->lnk.q_id, i,
					 l->lnk.cqm_deq_port);
				continue;
			}
			l->node_info.node_id = l->node.child[i].id;
			l->node_info.node_type = l->node.child[i].type;
			dp_map_qid_to_cqmdeq(&l->node_info, flag);
		}
		if (valid_child_cnt != l->node.num)
			pr_err("DPM: %s, SchedId: %d, NumActiveChildInArray: %d and ChildNum: %d mismatch\n",
					__func__, l->node.id.sch_id,
					valid_child_cnt, l->node.num);
	} else {
		pr_err("DPM: %s invalid node id: %d type %d\n", __func__,
				info->node_id.q_id, info->node_type);
		kfree(l);
		return DP_FAILURE;
	}

	kfree(l);
	return DP_SUCCESS;
}

/* If conflict return first available free priority
 * else no conflict return the original priority.
 * Assuming Prio is from 0 to 7, this logic only used for WSP.
 * Upon Success return priority
 * else return DP_FAILURE
 */
static int is_prio_used(int inst, int pid, int prio_wfq)
{
	int i, node_id, child_prio_wfq, conflict = 0;
	u8 prio[MAX_PP_CHILD_PER_NODE] = {0};
	struct hal_priv *priv = HAL(inst);

	if (prio_wfq >= MAX_PP_CHILD_PER_NODE)
		return DP_FAILURE;

	/* Check for conflict */
	for (i = 0; i < MAX_PP_CHILD_PER_NODE; i++) {
		/* Mark all the used priorities */
		if (!PP_FREE(CHILD(pid, i).flag)) {
			node_id = CHILD(pid, i).node_id;
			child_prio_wfq = priv->qos_sch_stat[node_id].prio_wfq;
			prio[child_prio_wfq] = 1;

			/* If priority used, set conflict */
			if (prio_wfq == child_prio_wfq)
				conflict = 1;
		}
	}

	/* If conflict return first available free priority */
	if (conflict) {
		/* Find first free priority */
		for (i = 0; i < MAX_PP_CHILD_PER_NODE; i++) {
			if (!prio[i])
				break;
		}

		if (i == MAX_PP_CHILD_PER_NODE) {
			pr_err("DPM: %s cannot get a free priority\n", __func__);
			return DP_FAILURE;
		}

		/* Return the priority DP calculated */
		return i;
	}

	return prio_wfq;
}

static int set_parent_arbi(int inst, int node_id, int arbi, int flag)
{
	struct hal_priv *priv = HAL(inst);
	int pid;

	if (priv->qos_sch_stat[node_id].parent.flag == PP_NODE_FREE) {
		pr_err("DPM: %s parent is not set for node\n", __func__);
		return DP_FAILURE;
	}

	arbi = dp2pp_arbi(arbi);
	if (arbi == DP_FAILURE) {
		pr_err("DPM: %s incorrect arbi provided:%d!\n", __func__, arbi);
		return DP_FAILURE;
	}

	pid = priv->qos_sch_stat[node_id].parent.node_id;
	if (S_NODE(priv->qos_sch_stat[node_id].parent.type)) {
		struct pp_qos_sched_conf sched_cfg = {0};

		if (dp_qos_sched_conf_get(priv->qdev, pid, &sched_cfg)) {
			pr_err("DPM: %s dp_qos_sched_conf_get fail\n", __func__);
			return DP_FAILURE;
		}

		sched_cfg.sched_parent_prop.arbitration = arbi;
		if (dp_qos_sched_set(priv->qdev, pid, &sched_cfg)) {
			pr_err("DPM: fail to set arbi Sch:%d parent of node:%d\n",
			       pid, node_id);
			return DP_FAILURE;
		}
	} else if (P_NODE(priv->qos_sch_stat[node_id].parent.type)) {
		struct pp_qos_port_conf port_cfg = {0};

		/* QoS port arbi is always WSP mode except for drop port */
		if (arbi == PP_QOS_ARBITRATION_WRR &&
		    pid != priv->ppv4_drop_p) {
			pr_err("DPM: %s arbi WRR not supported for port %d\n",
			       __func__, pid);
			return DP_FAILURE;
		}

		if (dp_qos_port_conf_get(priv->qdev, pid, &port_cfg)) {
			pr_err("DPM: %s dp_qos_port_conf_get fail\n", __func__);
			return DP_FAILURE;
		}

		port_cfg.port_parent_prop.arbitration = arbi;
		if (dp_qos_port_set(priv->qdev, pid, &port_cfg)) {
			pr_err("DPM: fail to set arbi port:%d parent of node:%d\n",
			       pid, node_id);
			return DP_FAILURE;
		}
	} else {
		pr_err("DPM: %s incorrect parent type:0x%x for node:%d.\n",
		       __func__, priv->qos_sch_stat[node_id].parent.type,
		       node_id);
		return DP_FAILURE;
	}

	return DP_SUCCESS;
}

int dp_set_qos_cfg(struct dp_node_link *info, struct pp_qos_queue_conf *q_cfg)
{
	struct cqm_port_info *deq;

	if (!q_cfg)
		return -1;
	deq = get_dp_deqport_info(info->inst, info->cqm_deq_port.cqm_deq_port);

	if (deq->dts_qos) {
		q_cfg->codel_en = deq->dts_qos->codel_en;
		q_cfg->wred_enable = deq->dts_qos->wred_en;
		q_cfg->wred_max_allowed = deq->dts_qos->qlen;
	} else {
		pr_err("dpm: why deq->dts_qos for cqm_deq_port=%d\n",
		       info->cqm_deq_port.cqm_deq_port);
		return -1;
	}

	return 0;
}

/* Configure node and link to parent.
 * Upon success return DP_SUCCESS
 * else return DP_FAILURE
 * pid: parrent node id
 */
static int dp_node_link_parent_set(
	struct dp_node_link *info, int pid, int flag)
{

	int node_id, qid, f = DP_NODE_INC, res = DP_FAILURE;
	struct hal_priv *priv = HAL(info->inst);
	struct pp_queue_stat *q_stat;

	if (info->arbi == ARBITRATION_WSP) {
		res = is_prio_used(info->inst, pid, info->prio_wfq);
		if (res == DP_FAILURE) {
			pr_err("DPM: %s invalid/used priority value\n", __func__);
			return DP_FAILURE;
		}
		info->prio_wfq = res;
	}

	if (Q_NODE(info->node_type)) {
		struct pp_qos_queue_conf *q_cfg;

		q_cfg = dp_kzalloc(sizeof(*q_cfg), GFP_ATOMIC);
		if (!q_cfg)
			return DP_FAILURE;

		qid = info->node_id.q_id;
		q_stat = &priv->qos_queue_stat[qid];
		node_id = q_stat->node_id;

		dp_qos_queue_conf_set_default(q_cfg);
		if (PP_ALLOC(q_stat->flag)) {
			dp_set_qos_cfg(info, q_cfg);
		} else {
			/* seems never go here according to current design */
			if (dp_qos_queue_conf_get(priv->qdev, node_id,
						  q_cfg)) {
				kfree(q_cfg);
				pr_err("DPM: %s dp_qos_queue_conf_get Q:%d fail\n",
				       __func__, node_id);
				return DP_FAILURE;
			}
		}
		q_cfg->queue_child_prop.parent = pid;
		if (info->arbi == ARBITRATION_WRR)
			q_cfg->queue_child_prop.wrr_weight = info->prio_wfq;
		else if (info->arbi == ARBITRATION_WSP)
			q_cfg->queue_child_prop.priority = info->prio_wfq;
		else
			pr_err("DPM: %s invalid Arbi:%d\n", __func__, info->arbi);

		DP_DEBUG(DP_DBG_FLAG_QOS,
			 "Try to link Q[%d/%d] to parent[%d/%d] P:%d\n",
			 qid, node_id, info->p_node_id.cqm_deq_port, pid,
			 info->cqm_deq_port.cqm_deq_port);

		if (dp_qos_queue_set_spl(info->inst, node_id,
					 info->node_id.q_id, q_cfg)) {
			kfree(q_cfg);
			pr_err("DPM: %s dp_qos_queue_set_spl Q:%d parent:%d fail\n",
			       __func__, node_id, pid);
			return DP_FAILURE;
		}
		kfree(q_cfg);
	} else if (S_NODE(info->node_type)) {
		struct pp_qos_sched_conf *sch_cfg;

		sch_cfg = dp_kzalloc(sizeof(*sch_cfg), GFP_ATOMIC);
		if (!sch_cfg)
			return DP_FAILURE;
		node_id = info->node_id.sch_id;
		if (dp_qos_sched_conf_get(priv->qdev, node_id, sch_cfg))
			dp_qos_sched_conf_set_default(sch_cfg);

		sch_cfg->sched_child_prop.parent = pid;
		if (info->arbi == ARBITRATION_WRR)
			sch_cfg->sched_child_prop.wrr_weight = info->prio_wfq;
		else if (info->arbi == ARBITRATION_WSP)
			sch_cfg->sched_child_prop.priority = info->prio_wfq;
		else
			pr_err("DPM: %s invalid Arbi %d\n", __func__, info->arbi);

		DP_DEBUG(DP_DBG_FLAG_QOS,
			 "Try to link Sch:/%d to parent[%d/%d] port[%d]\n",
			 node_id, info->p_node_id.cqm_deq_port, pid,
			 info->cqm_deq_port.cqm_deq_port);

		if (dp_qos_sched_set(priv->qdev, node_id, sch_cfg)) {
			kfree(sch_cfg);
			pr_err("DPM: %s dp_qos_sched_set Sch:%d parent:%d fail\n",
			       __func__, node_id, pid);
			return DP_FAILURE;
		}

		f |= P_FLAG;
		kfree(sch_cfg);
	} else {
		pr_err("DPM: %s invalid node type\n", __func__);
		return DP_FAILURE;
	}

	/* Fill parent's info in child's global table */
	priv->qos_sch_stat[node_id].prio_wfq = info->prio_wfq;
	priv->qos_sch_stat[node_id].parent.node_id = pid;
	priv->qos_sch_stat[node_id].parent.flag = PP_NODE_ACTIVE;
	priv->qos_sch_stat[node_id].parent.type = info->p_node_type;

	/* Increase child_num in parent's global table and status */
	node_stat_update(info->inst, node_id, DP_NODE_INC | P_FLAG);
	node_stat_update(info->inst, pid, DP_NODE_INC | C_FLAG);

	dp_map_qid_to_cqmdeq(info, flag);

	/* Set parent's arbitration */
	if (set_parent_arbi(info->inst, node_id, info->arbi, flag)) {
		pr_err("DPM: %s set_parent_arbi arbi node:%d arbi=%d fail\n",
		       __func__, info->arbi, node_id);
		return DP_FAILURE;
	}

	DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL, "node:%d arbi=%d prio=%d\n",
		 node_id, info->arbi, info->prio_wfq);

	return DP_SUCCESS;
}

/* Set node priority.
 * Upon Success return DP_SUCCESS
 * else return DP_FAILURE
 */
int _dp_qos_link_prio_set(struct dp_node_prio *info, int flag)
{
	struct hal_priv *priv = HAL(info->inst);
	int node_id, phy_id;

	if (Q_NODE(info->type)) {
		struct pp_qos_queue_conf q_cfg = {0};

		phy_id = info->id.q_id;
		if (q_node_chk(priv, phy_id))
			return DP_FAILURE;

		node_id = priv->qos_queue_stat[phy_id].node_id;
		if (dp_qos_queue_conf_get(priv->qdev, node_id, &q_cfg)) {
			pr_err("DPM: %s dp_qos_queue_conf_get fail\n", __func__);
			return DP_FAILURE;
		}

		if (info->arbi == ARBITRATION_WRR) {
			q_cfg.queue_child_prop.wrr_weight = info->prio_wfq;
		} else if (info->arbi == ARBITRATION_WSP) {
			q_cfg.queue_child_prop.priority = info->prio_wfq;
		} else {
			pr_err("DPM: %s invalid arbi %d\n", __func__, info->arbi);
			return DP_FAILURE;
		}

		DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL, "Prio:%d for queue[%d]\n",
			 info->prio_wfq, info->id.q_id);

		if (dp_qos_queue_set(priv->qdev, node_id, &q_cfg)) {
			pr_err("DPM: %s dp_qos_queue_set fail\n", __func__);
			return DP_FAILURE;
		}

		priv->qos_sch_stat[node_id].prio_wfq = info->prio_wfq;

		if (set_parent_arbi(info->inst, node_id, info->arbi, flag)) {
			pr_err("DPM: set_parent_arbi:%d fail for Q:%d\n",
			       info->arbi, node_id);
			return DP_FAILURE;
		}

		DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL,
			 "Q=%d arbi=%d prio=%d\n",
			 phy_id, info->arbi, info->prio_wfq);
	} else if (S_NODE(info->type)) {
		struct pp_qos_sched_conf sch_cfg = {0};

		node_id = info->id.sch_id;
		if (is_sch_parent_free(priv, node_id) ||
		    !is_sch_valid(node_id))
			return DP_FAILURE;

		if (dp_qos_sched_conf_get(priv->qdev, node_id, &sch_cfg)) {
			pr_err("DPM: %s dp_qos_sched_conf_get fail\n", __func__);
			return DP_FAILURE;
		}

		if (info->arbi == ARBITRATION_WRR) {
			sch_cfg.sched_child_prop.wrr_weight = info->prio_wfq;
		} else if (info->arbi == ARBITRATION_WSP) {
			sch_cfg.sched_child_prop.priority = info->prio_wfq;
		} else {
			pr_err("DPM: %s invalid arbi %d\n", __func__, info->arbi);
			return DP_FAILURE;
		}

		DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL, "Prio:%d for Sch:%d\n",
			 info->prio_wfq, node_id);

		if (dp_qos_sched_set(priv->qdev, node_id, &sch_cfg)) {
			pr_err("DPM: %s dp_qos_sched_set fail\n", __func__);
			return DP_FAILURE;
		}

		priv->qos_sch_stat[node_id].prio_wfq = info->prio_wfq;

		if (set_parent_arbi(info->inst, node_id, info->arbi, 0)) {
			pr_err("DPM: set_parent_arbi:%d fail for Sch:%d\n",
			       info->arbi, node_id);
			return DP_FAILURE;
		}

		DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL,
			 "Sch=%d arbi=%d prio=%d\n",
			 node_id, info->arbi, info->prio_wfq);
	} else {
		pr_err("DPM: %s incorrect node type:0x%x\n", __func__, info->type);
		return DP_FAILURE;
	}

	return DP_SUCCESS;
}

/* Get node priority.
 * Upon Success return DP_SUCCESS
 * else return DP_FAILURE
 */
int _dp_qos_link_prio_get(struct dp_node_prio *info, int flag)
{
	struct hal_priv *priv = HAL(info->inst);
	int node_id, arbi;

	if (Q_NODE(info->type)) {
		if (q_node_chk(priv, info->id.q_id))
			return DP_FAILURE;

		node_id = priv->qos_queue_stat[info->id.q_id].node_id;

		arbi = get_parent_arbi(info->inst, node_id, flag);
		if (arbi == DP_FAILURE)
			return DP_FAILURE;

		info->arbi = arbi;
		info->prio_wfq = priv->qos_sch_stat[node_id].prio_wfq;
	} else if (S_NODE(info->type)) {
		if (is_sch_parent_free(priv, info->id.sch_id) ||
		    !is_sch_valid(info->id.sch_id))
			return DP_FAILURE;

		arbi = get_parent_arbi(info->inst, info->id.sch_id, flag);
		if (arbi == DP_FAILURE)
			return DP_FAILURE;

		info->arbi = arbi;
		info->prio_wfq = priv->qos_sch_stat[info->id.sch_id].prio_wfq;
	} else {
		pr_err("DPM: %s incorrect node type provided:0x%x\n",
		       __func__, info->type);
		return DP_FAILURE;
	}

	return DP_SUCCESS;
}

int _dp_deq_port_res_get(struct dp_dequeue_res *res, int flag)
{
	struct local {
		struct pp_qos_queue_conf q_cfg;
		struct pp_qos_queue_info q_info;
		struct pp_qos_sched_conf sch_cfg;
		struct dp_qos_cfg_info qos_info;
		u16 qids[MAX_Q_PER_PORT];
	};
	struct hal_priv *priv = HAL(res->inst);
	u16 i, j, k, n, pid, start_idx, idx = 0;
	bool f_absolute_idx  = false;
	struct pmac_port_info *p_info;
	struct pp_sch_stat *sch_stat;
	struct cqm_deq_stat *p_stat;
	u32 q_num;
	struct local *l;
	struct dp_cap *cap;
	struct cqm_port_info *cqm_info;
	u8 control_id, port_id;
	u16 channel_id;
	int reins_port_id;

	/* Get reinsertion port to double check for required
	 * special handling
	 */
	cqm_info = get_dp_deqport_info(res->inst,
				       reinsert_deq_port[res->inst]);

	l = dp_kzalloc(sizeof(*l), GFP_ATOMIC);
	if (!l)
		return DP_FAILURE;

	dp_dma_parse_id(cqm_info->dma_chan, &control_id, &port_id,
			&channel_id);
	reins_port_id = dp_get_pmac_id(res->inst, control_id);
	/* Differentiation depending on port id allone is not sufficient
	 * in case reinsertion port is equal to another port.
	 */
	if (res->dp_port == reins_port_id && flag & DP_SUBIF_REINSERT) {
		/* fill info for reinsertion port */
		l->qos_info.inst = res->inst;
		if (_dp_qos_global_info_get(&l->qos_info, 0)) {
			pr_err("DPM: %s: failed to get QoS global info\n",
			       __func__);
			kfree(l);
			return DP_FAILURE;
		}
		res->cqm_deq_port = l->qos_info.reinsert_deq_port;
		res->num_deq_ports = 1;
		res->dp_port = -1;
		f_absolute_idx = true;
		res->cqm_deq_idx = l->qos_info.reinsert_deq_port;
		start_idx = res->cqm_deq_idx;

		goto QUEUE_RES_GET;
	}

	p_info = get_dp_port_info(res->inst, res->dp_port);
	if (!p_info->deq_port_num) {
		DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL,
			 "get_dp_port_info %d fail\n",
			 res->dp_port);
		kfree(l);
		return DP_FAILURE;
	}
	DP_DEBUG(DP_DBG_FLAG_QOS,
		 "dp_port=%d cqm_deq_idx=0x%x\n",
		 res->dp_port, res->cqm_deq_idx);
	/* For res->cqm_deq_idx
	 * if dp_port is CPU, then cqm_deq_idx is relative and absolute both
	 * if dp_port is non-cpu, then cqm_deq_idx is relative port id only
	 */
	if (res->cqm_deq_idx == DEQ_PORT_OFFSET_ALL) {
		res->num_deq_ports = p_info->deq_port_num;
		start_idx = 0;
	} else {
		/* if dev valid, then it is set by dpm itself before here */
		if (!res->dev)
			res->num_deq_ports = 1;
		start_idx = res->cqm_deq_idx;
	}
	if (res->dp_port == CPU_PORT)
		f_absolute_idx = true;
QUEUE_RES_GET:
	cap = &get_dp_prop_info(res->inst)->cap;
	res->num_q = 0;
	for (n = 0; n < res->num_deq_ports; n++) {
		if (f_absolute_idx)
			k = n + start_idx;
		else /* get is absolute dequeue port id */
			k = p_info->deq_ports[n + start_idx];
		if (!is_deqport_valid(k)) {
			pr_err("DPM: not valid deq_port %d under dp_port=%d\n",
			       k, res->dp_port);
			kfree(l);
			return DP_FAILURE;
		}
		if (n == 0)
			res->cqm_deq_port = k;
		p_stat = &priv->deq_port_stat[k];
		DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL,
			 "deq_port=%d/%d dp_port=%d\n",
			 k, p_stat->node_id, res->dp_port);
		if (PP_FREE(p_stat->flag)) {
			DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL,
				 "deq port %d free yet\n", k);
			continue;
		}
		if (dp_qos_port_get_queues(priv->qdev, p_stat->node_id,
					   l->qids,
					   ARRAY_SIZE(l->qids),
					   &q_num)) {
			pr_err("DPM: %s dp_qos_port_get_queues P[%d/%d] fail\n",
			       __func__, k, p_stat->node_id);
			goto exit;
		}
		res->num_q += q_num;

		if (!res->q_res)
			continue;

		DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL, "P[%d/%d] logical Q list\n",
			 k, p_stat->node_id);
		for (i = 0; i < q_num; i++)
			DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL,
				 "  Q:%d\n", l->qids[i]);

		for (i = 0; (i < q_num) && (idx < res->q_res_size); i++) {
			if (dp_qos_queue_info_get(priv->qdev, l->qids[i],
						  &l->q_info)) {
				pr_err("DPM: %s dp_qos_queue_info_get Q:%d fail\n",
				       __func__, l->qids[i]);
				continue;
			}

			j = 0;
			res->q_res[idx].sch_lvl = j;
			res->q_res[idx].q_node = l->qids[i];
			res->q_res[idx].q_id = l->q_info.physical_id;
			if (res->dp_port == CPU_PORT) {
				res->q_res[idx].cpu_id =
					(k / cap->max_port_per_cpu);
				res->q_res[idx].cpu_gpid =
					p_info->gpid_base + k;
			}
			DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL, "Q[%d/%d]\n",
				 l->q_info.physical_id, l->qids[i]);

			if (dp_qos_queue_conf_get(priv->qdev, l->qids[i],
						  &l->q_cfg)) {
				pr_err("DPM: %s dp_qos_queue_conf_get Q %d fail\n",
				       __func__, l->qids[i]);
				continue;
			}

			pid = l->q_cfg.queue_child_prop.parent;
			do {
				sch_stat = &priv->qos_sch_stat[pid];
				if (P_NODE(sch_stat->node.type)) {
					struct cqm_port_info *p;

					p = get_dp_deqport_info(res->inst, k);

					res->q_res[idx].qos_deq_port = pid;
					res->q_res[idx].cqm_deq_port = k;
					res->q_res[idx].cqm_deq_port_type =
								   p->cpu_type;
					break;
				} else if (!S_NODE(sch_stat->node.type)) {
					pr_err("DPM: %s wrong P[/%d] type:%d\n",
					       __func__, pid,
					       priv->qos_sch_stat[pid].node.type);
					break;
				}

				/* Update sched as parent */
				res->q_res[idx].sch_id[j++] = pid;
				res->q_res[idx].sch_lvl = j;

				/* Get next parent */
				if (dp_qos_sched_conf_get(priv->qdev, pid,
							  &l->sch_cfg)) {
					pr_err("DPM: %s %s Sch:%d fail\n",
					       __func__,
					       "dp_qos_sched_conf_get", pid);
					break;
				}
				pid = l->sch_cfg.sched_child_prop.parent;
			} while (1);

			idx++;
		}
	}

exit:
	kfree(l);

	return DP_SUCCESS;
}

/* Keep queue in blocked state and flush queue
 * Upon Success return DP_SUCCESS
 * else return DP_FAILURE
 */
int _dp_node_unlink(struct dp_node_link *info, int flag)
{
	struct hal_priv *priv = HAL(info->inst);
	int i, node_id, phy_id, q_num;
	struct pp_queue_stat *q_stat;
	struct pp_sch_stat *sch_stat;
	u16 qids[MAX_Q_PER_PORT];

	if (Q_NODE(info->node_type)) {
		phy_id = info->node_id.q_id;
		q_stat = &priv->qos_queue_stat[phy_id];
		node_id = q_stat->node_id;
		if (!PP_ACT(q_stat->flag)) {
			DP_DEBUG(DP_DBG_FLAG_QOS,
				 "Q:%d inactive, no unlink\n", phy_id);
			return DP_SUCCESS;
		}

		CBM_OPS(info->inst, cqm_qid2ep_map_set, info->node_id.q_id,
			priv->ppv4_drop_p);

		DP_DEBUG(DP_DBG_FLAG_QOS, "Map Q:%d to drop port\n", phy_id);

		queue_flush(info->inst, node_id, 0);
	} else if (S_NODE(info->node_type)) {
		node_id = info->node_id.sch_id;
		sch_stat = &priv->qos_sch_stat[node_id];
		if (!PP_ACT(sch_stat->c_flag)) {
			DP_DEBUG(DP_DBG_FLAG_QOS,
				 "Sch:%d inactive, no unlink\n", node_id);
			return DP_SUCCESS;
		}

		if (dp_qos_sched_get_queues(priv->qdev, node_id, qids,
					    ARRAY_SIZE(qids), &q_num)) {
			pr_err("DPM: %s dp_qos_sched_get_queues Sch:%d fail\n",
			       __func__, node_id);
			return DP_FAILURE;
		}

		for (i = 0; i < q_num; i++) {
			if (!PP_ACT(priv->qos_queue_stat[qids[i]].flag))
				continue;
			queue_flush(info->inst, qids[i], 0);
		}
	}

	return DP_SUCCESS;
}

void dump_qos_node_link(struct dp_node_link *cfg)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (!cfg || !(dp_dbg_flag & DP_DBG_FLAG_QOS))
		return;
	DP_INFO("parent_node(type: %s)=%d -> node(type: %s)=%d\n",
		node_type_str(cfg->p_node_type), cfg->p_node_id.q_id,
		node_type_str(cfg->node_type), cfg->node_id.q_id);

#endif
}

/* Check for parent type and allocate parent node (if needed)
 * and check for child type and allocate child node (if needed).
 * Then link child to parent.
 * Upon Success return DP_SUCCESS
 * else return DP_FAILURE
 */
int _dp_node_link_add(struct dp_node_link *info, int flag)
{
	struct local {
		struct dp_node_alloc node;
		struct pp_qos_queue_conf q_cfg;
		struct pp_qos_sched_conf sch_cfg;
		u16 qids[MAX_Q_PER_PORT];
		u8 q_block[MAX_Q_PER_PORT];
		struct pp_qos_sched_conf s_cfg;
		struct pp_qos_port_conf p_cfg;
	};
	int free_child = 0, i, node_id = 0, qid, q_num = 0;
	int pid; /* parent node id */
	int res = DP_FAILURE;
	bool new_pid = false;
	struct hal_priv *priv = HAL(info->inst);
	struct local *l;
	struct pmac_port_info *ppi = NULL;
	struct cqm_port_info *cpi = NULL;
	int new_child_num;

	dump_qos_node_link(info);

	/*dp_port outbound check*/
	if ((info->dp_port < 0) || (info->dp_port >= MAX_DP_PORTS)) {
		pr_err("DPM: %s %d given dp port(%d) is out of range:[%d-%d]\n",
					__func__, __LINE__, info->dp_port, 0,
					MAX_DP_PORTS - 1);
		return DP_FAILURE;
	} else {
		/* For the cases p_node_type is not PORT node,
		 * info->cqm_deq_port may not be valid, so
		 * we get deq info from pmac_port_info and search across all deq
		 * ports and their dp_port[], invalidate given dp_port
		 */
		ppi = get_dp_port_info(info->inst, info->dp_port);
		for (i = 0; i < ppi->deq_port_num; i++) {
			/* Cpu dequeue port -1 */
			if ((ppi->deq_ports[i] == (u16) -1) &&
			    (info->dp_port == CPU_PORT))
				continue;
			if ((ppi->deq_ports[i] >= DP_MAX_PPV4_PORT) ||
					(ppi->deq_ports[i] < 0)) {
				pr_err("DPM: %s %d, why deq_port[%d] is out of range\n",
						__func__, __LINE__, i);
				return DP_FAILURE;
			}
			cpi = get_dp_deqport_info(info->inst, ppi->deq_ports[i]);

			if (!cpi->dp_port[info->dp_port]) {
				pr_err("DPM: %s-deq_ports[%d].dp_port[%d] zero: %u\n",
					__func__,  i,
					cpi->dp_port[info->dp_port],
					ppi->deq_ports[i]);
				return DP_FAILURE;
			}
		}
	}

	l = dp_kzalloc(sizeof(*l), GFP_ATOMIC);
	if (!l)
		return DP_FAILURE;

	DP_DEBUG(DP_DBG_FLAG_QOS, "inst=%d\n", info->inst);

	pid = dp_qos_parent_get(info, flag, &new_pid);
	if (pid == DP_FAILURE) {
		pr_err("DPM: %s dp_qos_parent_get fail\n", __func__);
		goto exit;
	}

	/* Used parent's arbi mode or force to use parent's arbi mode
	 * if 1) parent is a port, 2)not newly allocated 3)arbi mode not WSP
	 */
	if (info->arbi == ARBITRATION_PARENT ||
	    (P_NODE(info->p_node_type) &&
	     !new_pid && info->arbi != ARBITRATION_WSP)) {
		if (P_NODE(info->p_node_type)) {
			if (dp_qos_port_conf_get(priv->qdev, pid, &l->p_cfg)) {
				pr_err("DPM: %s port:%d get parent arbi fail\n",
				       __func__, pid);
				goto exit;
			}
			info->arbi =
				pp2dp_arbi(l->p_cfg.port_parent_prop.arbitration);
			DP_DEBUG(DP_DBG_FLAG_QOS, "Parent: port:%d arbi:%d\n",
				 pid, info->arbi);
		} else if (S_NODE(info->p_node_type)) {
			if (dp_qos_sched_conf_get(priv->qdev, pid, &l->s_cfg)) {
				pr_err("DPM: %s Sch:%d get parent arbi fail\n",
				       __func__, pid);
				goto exit;
			}
			info->arbi =
				pp2dp_arbi(l->s_cfg.sched_parent_prop.arbitration);
			DP_DEBUG(DP_DBG_FLAG_QOS, "Parent: Sch:%d arbi:%d\n",
				 pid, info->arbi);
		}
	}

	new_child_num = priv->qos_sch_stat[pid].child_num;
	if (info->node_id.q_id == DP_NODE_AUTO_ID)
		new_child_num++;
	if (new_child_num > DP_MAX_CHILD_PER_NODE) {
		pr_err("DPM: %s node:%d child num: node %d over limit\n", __func__,
		       priv->qos_sch_stat[pid].child_num, pid);
		goto err;
	}
	DP_DEBUG(DP_DBG_FLAG_QOS, "dp_qos_parent_get ok: parent node %d\n",
		 pid);
	if (Q_NODE(info->node_type)) {
		if (info->node_id.q_id == DP_NODE_AUTO_ID) {
			/* Create Q */
			l->node.inst = info->inst;
			l->node.type = info->node_type;
			l->node.dp_port = info->dp_port;
			if (_dp_node_alloc(&l->node, flag)) {
				pr_err("DPM: %s _dp_node_alloc Q fail\n", __func__);
				goto err;
			}
			info->node_id = l->node.id;
			free_child = 1;
		} else {
			if (priv->qos_queue_stat[info->node_id.q_id].flag ==
			    PP_NODE_FREE) {
				pr_err("DPM: %s qid:%d is in Free state\n",
				       __func__, info->node_id.q_id);
				goto err;
			}
		}

		node_id = priv->qos_queue_stat[info->node_id.q_id].node_id;
		priv->qos_queue_stat[info->node_id.q_id].deq_port =
			info->cqm_deq_port.cqm_deq_port;
		q_num = 1;
		l->qids[0] = node_id;
		/* Note: later DPM had better change to internal status machine
		 *       instead of depends on PP QOS status for performance
		 *       reason
		 */
		if (!dp_qos_queue_conf_get(priv->qdev, node_id, &l->q_cfg) &&
		    (priv->qos_sch_stat[node_id].p_flag & PP_NODE_ACTIVE)) {
			/* Q already linked to a parent */
			if (!l->q_cfg.blocked)
				l->q_block[0] = l->q_cfg.blocked;

			DP_DEBUG(DP_DBG_FLAG_QOS,
				 "flush Q[%d]\n", node_id);
			queue_flush(info->inst, node_id,
				    DP_QFLUSH_FLAG_RESTORE_LOOKUP |
				    DP_QFLUSH_FLAG_RESTORE_QOS_PORT);
			if (l->q_cfg.queue_child_prop.parent !=
			    priv->ppv4_drop_p) {
				/* already linked, ie, in active status.
				 * If current parent and new parent same. Do nothing.
				 */

				if (l->q_cfg.queue_child_prop.parent == pid) {
					DP_DEBUG(DP_DBG_FLAG_QOS,
						 "q[%d/%d] %s=%d\n",
						 info->node_id.q_id, node_id,
						 "same parent node",
						 pid);
					q_num = 0;
					goto NEXT;
				}
				/* Update flag for queue node */
				DP_DEBUG(DP_DBG_FLAG_QOS,
					 "q[%d/%d] %s=%d %s=%d\n",
					 info->node_id.q_id, node_id,
					 "old parent node",
					 l->q_cfg.queue_child_prop.parent,
					 "new parent",
					 pid);
				if (node_stat_update(info->inst, node_id,
						     DP_NODE_DEC)) {
					pr_err("DPM: %s node_stat_update Q:%d fail\n",
					       __func__, node_id);
					goto err;
				}

				/* Reduce child_num in parent's global table */
				DP_DEBUG(DP_DBG_FLAG_QOS,
					 "node_stat_update parent %d for Q[%d]\n",
					 PARENT(l->q_cfg), node_id);
				if (node_stat_update(info->inst,
						     PARENT(l->q_cfg),
						     DP_NODE_DEC | C_FLAG)) {
					pr_err("DPM: %s node_stat_update parent fail\n",
					       __func__);
					goto err;
				}
			}
		} else {
			DP_DEBUG(DP_DBG_FLAG_QOS,
				 "q_node_id(%d) not link to parent yet\n",
				 node_id);
			l->q_block[0] = 0;
		}
	} else if (S_NODE(info->node_type)) {
		if (info->node_id.sch_id == DP_NODE_AUTO_ID) {
			l->node.inst = info->inst;
			l->node.type = info->node_type;
			l->node.dp_port = info->dp_port;
			if (_dp_node_alloc(&l->node, flag)) {
				pr_err("DPM: %s dp_node_alloc Sch fail\n",
				       __func__);
				goto err;
			}
			info->node_id = l->node.id;
			free_child = 1;
		}
		node_id = info->node_id.sch_id;
		DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL,
			 "inst=%d dp_port=%d Sch:%d\n",
			 l->node.inst, l->node.dp_port, node_id);

		if (priv->qos_sch_stat[node_id].p_flag ==
		    PP_NODE_FREE) {
			pr_err("DPM: %s Sch:%d is in Free state\n",
			       __func__, info->node_id.sch_id);
			goto err;
		}

		if (!free_child &&
		    !dp_qos_sched_conf_get(priv->qdev, node_id,
					   &l->sch_cfg) &&
		    (priv->qos_sch_stat[node_id].p_flag & PP_NODE_ACTIVE)) {
			/* already linked, ie, in active status.
			 * If current parent and new parent same. Do nothing.
			 */

			if (l->sch_cfg.sched_child_prop.parent == pid) {
				DP_DEBUG(DP_DBG_FLAG_QOS,
					 "sch[%d] %s=%u\n",
					 node_id,
					 "same parent node",
					 pid);
				goto NEXT;
			}
			DP_DEBUG(DP_DBG_FLAG_QOS,
				 "sch[%d] %s=%u\n %s=%d",
				 node_id,
				 "old parent node",
				 l->sch_cfg.sched_child_prop.parent,
				 "new parent",
				 pid);
			if (dp_qos_sched_get_queues(priv->qdev,
						    info->node_id.sch_id,
						    l->qids, sizeof(l->qids),
						    &q_num)) {
				pr_err("DPM: %s %s Sch:%d fail\n",
				       __func__, "dp_qos_sched_get_queues",
				       info->node_id.sch_id);
				goto err;
			}

			for (i = 0; i < q_num; i++) {
				if (dp_qos_queue_conf_get(priv->qdev,
							  l->qids[i],
							  &l->q_cfg)) {
					pr_err("DPM: %s qids1 %d not valid\n",
						__func__, l->qids[i]);
					continue;
				}
				if (!l->q_cfg.blocked)
					l->q_block[i] = l->q_cfg.blocked;

				queue_flush(info->inst, l->qids[i],
					    DP_QFLUSH_FLAG_RESTORE_LOOKUP |
					    DP_QFLUSH_FLAG_RESTORE_QOS_PORT);
			}

			/* Update flag for sch node */
			if (node_stat_update(info->inst, info->node_id.sch_id,
					     DP_NODE_DEC | P_FLAG)) {
				pr_err("DPM: %s node_stat_update Sch %d fail\n",
				       __func__, info->node_id.sch_id);
				goto err;
			}

			/* Reduce child_num in parent's global table */
			if (node_stat_update(info->inst, PARENT_S(l->sch_cfg),
					     DP_NODE_DEC | C_FLAG)) {
				pr_err("DPM: %s node_stat_update parent fail\n",
				       __func__);
				goto err;
			}
		}
	}

	if (dp_node_link_parent_set(info, pid, flag)) {
		pr_err("DPM: %s dp_node_link_parent_set parent:%d fail\n",
		       __func__, pid);
		goto err;
	}
NEXT:
	res = DP_SUCCESS;

	for (i = 0; i < q_num; i++) {
		if (l->q_block[i] < 0)
			continue;

		if (dp_qos_queue_conf_get(priv->qdev, l->qids[i], &l->q_cfg)) {
			pr_err("DPM: %s qids2 %d not valid\n",
				__func__, l->qids[i]);
			continue;
		}

		l->q_cfg.blocked = l->q_block[i];
		DP_DEBUG(DP_DBG_FLAG_QOS, "Unblock Q[%d/%d]\n",
			 get_qid_by_node(info->inst, l->qids[i]), l->qids[i]);

		if (dp_qos_queue_set(priv->qdev, l->qids[i],
				     &l->q_cfg)) {
			pr_err("DPM: %s dp_qos_queue_set Q[/%d] fail\n",
			       __func__, l->qids[i]);
			res = DP_FAILURE;
		}

		qid = get_qid_by_node(info->inst, l->qids[i]);
		if (qid == DP_FAILURE)
			res = DP_FAILURE;
		else
			priv->qos_queue_stat[qid].blocked = l->q_cfg.blocked;
	}

	goto exit;

err:
	if (free_child) {
		if (Q_NODE(l->node.type)) {
			DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL,
				 "remove Q:%d\n", node_id);
			dp_qos_queue_remove(priv->qdev, node_id);
			node_stat_update(info->inst, node_id, DP_NODE_RST);
		} else if (S_NODE(l->node.type)) {
			DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL,
				 "remove Sch:%d\n", l->node.id.sch_id);
			dp_qos_sched_remove(priv->qdev, l->node.id.sch_id);
			node_stat_update(info->inst, node_id, DP_NODE_RST);
		}
	}

	if (new_pid) {
		if (P_NODE(info->p_node_type)) {
			DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL, "remove P:%d\n", pid);
			dp_qos_port_remove(priv->qdev, pid, info->inst);
		} else if (S_NODE(info->p_node_type) &&
			   info->p_node_id.sch_id == DP_NODE_AUTO_ID) {
			DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL, "remove Sch:%d\n",
				 pid);
			dp_qos_sched_remove(priv->qdev, pid);
		}
		node_stat_update(info->inst, pid, DP_NODE_RST);
	}

exit:
	kfree(l);

	return res;
}

int _dp_queue_conf_set(struct dp_queue_conf *cfg, int flag)
{
	struct hal_priv *priv = HAL(cfg->inst);
	struct pp_qos_queue_conf *q_cfg;
	int node_id, res = DP_FAILURE;

	q_cfg = dp_kzalloc(sizeof(*q_cfg), GFP_ATOMIC);
	if (!q_cfg)
		return DP_FAILURE;

	node_id = priv->qos_queue_stat[cfg->q_id].node_id;
	if (dp_qos_queue_conf_get(priv->qdev, node_id, q_cfg)) {
		pr_err("DPM: %s dp_qos_queue_conf_get Q:%d fail\n",
		       __func__, node_id);
		goto exit;
	}

	if (cfg->act & DP_NODE_DIS)
		q_cfg->blocked = 1;
	else if (cfg->act & DP_NODE_EN)
		q_cfg->blocked = 0;

	priv->qos_queue_stat[cfg->q_id].blocked = q_cfg->blocked;
	if (cfg->drop == DP_QUEUE_DROP_WRED)
		q_cfg->wred_enable = 1;

	if (cfg->codel == DP_CODEL_EN)
		q_cfg->codel_en = 1;

	q_cfg->wred_min_avg_green = cfg->min_size[0];
	q_cfg->wred_max_avg_green = cfg->max_size[0];
	q_cfg->wred_slope_green = cfg->wred_slope[0];
	q_cfg->wred_min_avg_yellow = cfg->min_size[1];
	q_cfg->wred_max_avg_yellow = cfg->max_size[1];
	q_cfg->wred_slope_yellow = cfg->wred_slope[1];
	q_cfg->wred_max_allowed = cfg->wred_max_allowed;
	q_cfg->wred_min_guaranteed = cfg->wred_min_guaranteed;

	if (dp_qos_queue_set(priv->qdev, node_id, q_cfg)) {
		pr_err("DPM: %s dp_qos_queue_set Q:%d fail\n", __func__, node_id);
		goto exit;
	}

	res = DP_SUCCESS;

exit:
	kfree(q_cfg);

	return res;
}

int _dp_queue_conf_get(struct dp_queue_conf *cfg, int flag)
{
	struct hal_priv *priv = HAL(cfg->inst);
	int node_id, res = DP_FAILURE;
	struct pp_qos_queue_conf *q_cfg;

	if (q_node_chk(priv, cfg->q_id))
		return res;

	q_cfg = dp_kzalloc(sizeof(*q_cfg), GFP_ATOMIC);
	if (!q_cfg)
		return res;

	node_id = priv->qos_queue_stat[cfg->q_id].node_id;
	if (dp_qos_queue_conf_get(priv->qdev, node_id, q_cfg)) {
		pr_err("DPM: %s dp_qos_queue_conf_get Q:%d fail\n",
		       __func__, node_id);
		goto exit;
	}

	if (q_cfg->blocked)
		cfg->act = DP_NODE_DIS;
	else
		cfg->act = DP_NODE_EN;

	if (q_cfg->codel_en)
		cfg->codel = DP_CODEL_EN;
	else
		cfg->codel = DP_CODEL_DIS;

	if (q_cfg->wred_enable) {
		cfg->drop = DP_QUEUE_DROP_WRED;
		cfg->min_size[0] = q_cfg->wred_min_avg_green;
		cfg->max_size[0] = q_cfg->wred_max_avg_green;
		cfg->wred_slope[0] = q_cfg->wred_slope_green;
		cfg->min_size[1] = q_cfg->wred_min_avg_yellow;
		cfg->max_size[1] = q_cfg->wred_max_avg_yellow;
		cfg->wred_slope[1] = q_cfg->wred_slope_yellow;
		cfg->min_size[2] = 0;
		cfg->max_size[2] = 0;
		cfg->wred_slope[2] = 0;
		cfg->wred_min_guaranteed = q_cfg->wred_min_guaranteed;
	} else {
		cfg->drop = DP_QUEUE_DROP_TAIL;
		cfg->min_size[0] = q_cfg->wred_min_avg_green;
		cfg->max_size[0] = q_cfg->wred_max_avg_green;
		cfg->min_size[1] = q_cfg->wred_min_avg_yellow;
		cfg->max_size[1] = q_cfg->wred_max_avg_yellow;
	}
	cfg->wred_max_allowed = q_cfg->wred_max_allowed;

	res = DP_SUCCESS;

exit:
	kfree(q_cfg);

	return res;
}

/* Enable current link node and return DP_SUCCESS
 * else return DP_FAILURE
 */
int _dp_node_link_en_set(struct dp_node_link_enable *en, int flag)
{
	struct hal_priv *priv = HAL(en->inst);
	int node_id;

	if (en->en & DP_NODE_EN && en->en & DP_NODE_DIS) {
		pr_err("DPM: %s invalid param, en & dis set!\n", __func__);
		return DP_FAILURE;
	}

	if (en->en & DP_NODE_SUSPEND && en->en & DP_NODE_RESUME) {
		pr_err("DPM: %s invalid param, suspend & resume set!\n", __func__);
		return DP_FAILURE;
	}

	if (Q_NODE(en->type)) {
		if (en->en >= DP_NODE_SET_CMD_MAX) {
			pr_err("DPM: %s invalid param\n", __func__);
			return DP_FAILURE;
		}

		if (q_node_chk(priv, en->id.q_id))
			return DP_FAILURE;

		node_id = priv->qos_queue_stat[en->id.q_id].node_id;

		if (en->en & DP_NODE_EN) {
			if (pp_qos_queue_unblock(priv->qdev, node_id)) {
				pr_err("DPM: %s pp_qos_queue_unblock qid:%d fail\n",
				       __func__, en->id.q_id);
				return DP_FAILURE;
			}
			priv->qos_queue_stat[en->id.q_id].blocked = 0;
		} else if (en->en & DP_NODE_DIS) {
			if (pp_qos_queue_block(priv->qdev, node_id)) {
				pr_err("DPM: %s pp_qos_queue_block qid:%d fail\n",
				       __func__, en->id.q_id);
				return DP_FAILURE;
			}
			priv->qos_queue_stat[en->id.q_id].blocked = 1;
		}
	} else if (S_NODE(en->type)) {
		if (!(en->en & (DP_NODE_SUSPEND | DP_NODE_RESUME))) {
			pr_err("DPM: %s invalid param\n", __func__);
			return DP_FAILURE;
		}

		if (!is_sch_valid(en->id.sch_id) ||
		    is_sch_parent_free(priv, en->id.sch_id))
			return DP_FAILURE;

	} else if (P_NODE(en->type)) {
		if (en->en >= DP_NODE_SET_CMD_MAX) {
			pr_err("DPM: %s invalid param\n", __func__);
			return DP_FAILURE;
		}

		if (!is_deqport_valid(en->id.cqm_deq_port) ||
		    is_port_node_free(priv, en->id.cqm_deq_port))
			return DP_FAILURE;

		node_id = priv->deq_port_stat[en->id.cqm_deq_port].node_id;
		if (en->en & DP_NODE_EN) {
			if (pp_qos_port_unblock(priv->qdev, node_id)) {
				pr_err("DPM: %s pp_qos_port_unblock P:%d fail\n",
				       __func__, en->id.cqm_deq_port);
				return DP_FAILURE;
			}
			priv->deq_port_stat[en->id.cqm_deq_port].disabled = 0;
		} else if (en->en & DP_NODE_DIS) {
			if (pp_qos_port_block(priv->qdev, node_id)) {
				pr_err("DPM: %s pp_qos_port_block P:%d fail\n",
				       __func__, en->id.cqm_deq_port);
				return DP_FAILURE;
			}
			priv->deq_port_stat[en->id.cqm_deq_port].disabled = 1;
		}

		if (en->en & DP_NODE_SUSPEND) {
			if (pp_qos_port_disable(priv->qdev, node_id)) {
				pr_err("DPM: %s pp_qos_port_disable P:%d fail\n",
				       __func__, en->id.cqm_deq_port);
				return DP_FAILURE;
			}
			/* Not allowed to dequeue */
			priv->deq_port_stat[en->id.cqm_deq_port].disabled = 1;
		} else if (en->en & DP_NODE_RESUME) {
			if (pp_qos_port_enable(priv->qdev, node_id)) {
				pr_err("DPM: %s pp_qos_port_enable P:%d fail\n",
				       __func__, en->id.cqm_deq_port);
				return DP_FAILURE;
			}
			/* Allowed to dequeue */
			priv->deq_port_stat[en->id.cqm_deq_port].disabled = 0;
		}
	}

	return DP_SUCCESS;
}

int _dp_node_link_en_get(struct dp_node_link_enable *en, int flag)
{
	struct hal_priv *priv = HAL(en->inst);

	if (Q_NODE(en->type)) {
		DP_DEBUG(DP_DBG_FLAG_QOS,
			 "qid:%d\n", en->id.q_id);

		if (priv->qos_queue_stat[en->id.q_id].blocked)
			en->en |= DP_NODE_DIS;
		else
			en->en |= DP_NODE_EN;
	} else if (S_NODE(en->type)) {
		DP_DEBUG(DP_DBG_FLAG_QOS,
			 "Sch:%d\n", en->id.sch_id);

		if (!PP_ACT(priv->qos_sch_stat[en->id.sch_id].c_flag)) {
			pr_err("DPM: %s wrong Sch:%d flag Expect ACTIVE\n",
			       __func__, en->id.sch_id);
			return DP_FAILURE;
		}

		en->en |= DP_NODE_EN;
	} else if (P_NODE(en->type)) {
		DP_DEBUG(DP_DBG_FLAG_QOS,
			 "P:%d\n", en->id.cqm_deq_port);

		if (priv->deq_port_stat[en->id.cqm_deq_port].disabled)
			en->en |= DP_NODE_DIS;
		else
			en->en |= DP_NODE_EN;
	}

	return DP_SUCCESS;
}

/* Get full link based on Q and return DP_SUCCESS
 * else return DP_FAILURE
 */
int _dp_link_get(struct dp_qos_link *cfg, int flag)
{
	struct hal_priv *priv = HAL(cfg->inst);
	struct pp_sch_stat *sch_stat;
	int i, node_id;

	if (!PP_ACT(priv->qos_queue_stat[cfg->q_id].flag)) {
		pr_err("DPM: %s incorrect qid:%d state:expect ACTIVE\n",
		       __func__, cfg->q_id);
		return DP_FAILURE;
	}

	node_id = priv->qos_queue_stat[cfg->q_id].node_id;
	sch_stat = &priv->qos_sch_stat[node_id];

	cfg->q_leaf = 0;
	cfg->n_sch_lvl = 0;
	cfg->q_prio_wfq = sch_stat->prio_wfq;
	cfg->q_arbi = get_parent_arbi(cfg->inst, node_id, 0);

	if (P_NODE(sch_stat->parent.type)) {
		node_id = sch_stat->parent.node_id;
		cfg->cqm_deq_port =
			get_cqm_deq_port_by_node(cfg->inst, node_id);
	} else if (S_NODE(sch_stat->parent.type)) {
		for (i = 0; i < DP_MAX_SCH_LVL - 1; i++) {
			node_id = sch_stat->parent.node_id;
			cfg->sch[i].id = node_id;
			cfg->sch[i].leaf = 0;
			cfg->sch[i].arbi = get_parent_arbi(cfg->inst,
							   node_id, 0);
			sch_stat = &priv->qos_sch_stat[node_id];
			cfg->sch[i].prio_wfq = sch_stat->prio_wfq;
			cfg->sch[i + 1].id = sch_stat->parent.node_id;
			cfg->n_sch_lvl = i + 1;

			if (P_NODE(sch_stat->parent.type))
				break;
		}

		node_id = sch_stat->parent.node_id;
		cfg->cqm_deq_port =
				get_cqm_deq_port_by_node(cfg->inst, node_id);
	} else {
		pr_err("DPM: %s invalid parent type\n", __func__);
		return DP_FAILURE;
	}

	return DP_SUCCESS;
}

void dump_qos_link(int res, struct dp_qos_link *old_cfg,
		struct dp_qos_link *cfg, int flag)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	int indent = 27, i;
	if (!cfg ||  !old_cfg || !(dp_dbg_flag & DP_DBG_FLAG_QOS))
		return;

	DP_DEBUG(DP_DBG_FLAG_QOS,
			"%s=%s %s=%d %s=%d %s=%d %s=%d %s=%d\n"
			"%*s=%d->%d %s=%d->%d %s=%d->%d %s=%d->%d\n",
			"res", !res ? "ok":"fail",
			"inst", cfg->inst,
			"cqm_deq_port", cfg->cqm_deq_port,
			"q_leaf", cfg->q_leaf,
			"n_sch_lvl", cfg->n_sch_lvl,
			"flag", flag,

			indent, "dp_port", old_cfg->dp_port, cfg->dp_port,
			"q_id", old_cfg->q_id, cfg->q_id,
			"q_arbi", old_cfg->q_arbi, cfg->q_arbi,
			"q_prio_wfq", old_cfg->q_prio_wfq, cfg->q_prio_wfq);
	for (i = 0; i < DP_MAX_SCH_LVL; i++) {
		_DP_DEBUG(DP_DBG_FLAG_QOS, KERN_CONT
			"%*s[%c]: id: %2d->%d, prio_wfq: %d->%d, leaf: %d, arbi: %d->%d\n",
			indent-1, "sch", cfg->n_sch_lvl > i ? i+48 : 'x',
			old_cfg->sch[i].id, cfg->sch[i].id,
			old_cfg->sch[i].prio_wfq, cfg->sch[i].prio_wfq,
			cfg->sch[i].leaf,
			old_cfg->sch[i].arbi, cfg->sch[i].arbi);
	}
	DP_DEBUG(DP_DBG_FLAG_QOS,
			"[Qid: %d] --> ", cfg->q_id);
	for (i = 0; i < cfg->n_sch_lvl; i++)
		_DP_DEBUG(DP_DBG_FLAG_QOS, KERN_CONT
				"[SchedId: %d] --> ", cfg->sch[i].id);
	_DP_DEBUG(DP_DBG_FLAG_QOS, KERN_CONT
			"[CqmDeqPort: %d] --> [DPID: %d]\n", cfg->cqm_deq_port,
			cfg->dp_port);


#endif
}

/* Configure end to end link and return DP_SUCCESS
 * else return DP_FAILURE
 */
int _dp_link_add(struct dp_qos_link *cfg, int flag)
{
	struct dp_node_alloc node = {0};
	struct dp_node_link info = {0};
	int i, id, ret;
	struct dp_qos_link old_cfg = {0};
	if (cfg->n_sch_lvl > DP_MAX_SCH_LVL || cfg->n_sch_lvl < 0) {
		pr_err("DPM: %s: n_sch_lvl out of range, expect 0 ~ %d\n",
		       __func__, DP_MAX_SCH_LVL);
		return DP_FAILURE;
	}
	old_cfg = *cfg;

	info.inst = cfg->inst;
	info.dp_port = cfg->dp_port;
	info.node_type = DP_NODE_SCH;
	info.p_node_type = DP_NODE_PORT;
	info.p_node_id.cqm_deq_port = cfg->cqm_deq_port;
	info.cqm_deq_port.cqm_deq_port = cfg->cqm_deq_port;

	for (i = cfg->n_sch_lvl - 1; i >= 0; i--) {
		info.node_id.sch_id = cfg->sch[i].id;
		info.leaf = cfg->sch[i].leaf;
		info.arbi = cfg->sch[i].arbi;
		info.prio_wfq = cfg->sch[i].prio_wfq;
		if (_dp_node_link_add(&info, flag)) {
			pr_err("DPM: %s: fail to link Sch:%d to Sch:%d\n", __func__,
			       info.node_id.sch_id, info.p_node_id.sch_id);
			goto err;
		}

		cfg->sch[i].id = info.node_id.sch_id;
		cfg->sch[i].arbi = info.arbi;
		cfg->sch[i].prio_wfq = info.prio_wfq;
		info.p_node_id = info.node_id;
		info.p_node_type = info.node_type;
	}

	info.node_id.q_id = cfg->q_id;
	info.node_type = DP_NODE_QUEUE;
	info.leaf = cfg->q_leaf;
	info.arbi = cfg->q_arbi;
	info.prio_wfq = cfg->q_prio_wfq;
	if (_dp_node_link_add(&info, flag)) {
		pr_err("DPM: %s: fail to link qid:%d to Sch:%d\n", __func__,
		       info.node_id.q_id, info.p_node_id.sch_id);
		goto err;
	}

	cfg->dp_port = info.dp_port;
	cfg->q_id = info.node_id.q_id;
	cfg->q_arbi = info.arbi;
	cfg->q_prio_wfq = info.prio_wfq;
	ret = DP_SUCCESS;
	goto ret;
err:
	while (i < cfg->n_sch_lvl) {
		id = i < 0 ? cfg->q_id : cfg->sch[i].id;
		if (id == DP_NODE_AUTO_ID) {
			node.id.sch_id = id;
			node.inst = cfg->inst;
			node.dp_port = cfg->dp_port;
			node.type = i < 0 ? DP_NODE_QUEUE : DP_NODE_SCH;
			if (_dp_node_free(&node, flag))
				pr_err("DPM: %s _dp_node_free fail\n", __func__);
		}
		node_stat_update(cfg->inst, id, DP_NODE_DEC);
		i++;
	}
	ret = DP_FAILURE;
ret:
	dump_qos_link(ret, &old_cfg, cfg, flag);
	return ret;
}

#define BYTE_TO_BURST_SZ(quanta, bytes)   ilog2((bytes) / (1024 * (quanta)))
#define BURST_SZ_TO_BYTE(quanta, burst)   (((1 << (burst)) * (quanta)) * 1024)

/* DP_NO_SHAPER_LIMIT no limit for shaper
 * DP_MAX_SHAPER_LIMIT max limit for shaper
 * Configure shaper limit for node and return DP_SUCCESS
 * else return DP_FAILURE
 */
int _dp_shaper_conf_set(struct dp_shaper_conf *cfg, int flag)
{
	struct hal_priv *priv = HAL(cfg->inst);
	u32 bw_limit, quanta = 0;
	int node_id;

	/* Quanta is normally 4 in PPv4 */
	if (pp_qos_get_quanta(priv->qdev, &quanta)) {
		pr_err("DPM: %s: pp_qos_get_quanta qid:%d fail\n",
		       __func__, cfg->id.q_id);
		return DP_FAILURE;
	}

	/* 2 unit is HW will take as 1 << 2 = 4 Quanta,
	 * 1 Quanta is 4KB, so 16KB
	 */
	if (!cfg->cbs)
		cfg->cbs = QOS_DEAFULT_MAX_BURST;

	if (Q_NODE(cfg->type)) {
		struct pp_qos_queue_conf *q_cfg;

		if (q_node_chk(priv, cfg->id.q_id))
			return DP_FAILURE;

		q_cfg = dp_kzalloc(sizeof(*q_cfg), GFP_ATOMIC);
		if (!q_cfg)
			return DP_FAILURE;

		node_id = priv->qos_queue_stat[cfg->id.q_id].node_id;
		if (dp_qos_queue_conf_get(priv->qdev, node_id, q_cfg)) {
			kfree(q_cfg);
			pr_err("DPM: qos_queue_conf_get fail:%d\n", cfg->id.q_id);
			return DP_FAILURE;
		}

		switch (cfg->cmd) {
		case DP_SHAPER_CMD_ADD:
		case DP_SHAPER_CMD_ENABLE:
			if (dp2pp_shaper_limit(cfg->cir, &bw_limit)) {
				kfree(q_cfg);
				return DP_FAILURE;
			}

			q_cfg->common_prop.bandwidth_limit = bw_limit;
			q_cfg->eir =
				cfg->pir > cfg->cir ? cfg->pir - cfg->cir : 0;

			if (cfg->pbs < cfg->cbs)
				cfg->pbs = cfg->cbs;

			/* If require 16KB of burst size, EBS should be 2 */
			q_cfg->ebs = BYTE_TO_BURST_SZ(quanta,
						     cfg->pbs - cfg->cbs);
			q_cfg->common_prop.max_burst =
					BYTE_TO_BURST_SZ(quanta, cfg->cbs);

			break;
		case DP_SHAPER_CMD_REMOVE:
		case DP_SHAPER_CMD_DISABLE:
			q_cfg->eir = 0;
			q_cfg->ebs = QOS_DEAFULT_MAX_BURST;
			q_cfg->common_prop.bandwidth_limit = 0;
			q_cfg->common_prop.max_burst = QOS_DEAFULT_MAX_BURST;
			break;
		default:
			pr_err("DPM: %s invalid cmd:%d\n", __func__, cfg->cmd);
			kfree(q_cfg);
			return DP_FAILURE;
		}

		if (dp_qos_queue_set(priv->qdev, node_id, q_cfg)) {
			kfree(q_cfg);
			pr_err("DPM: %s dp_qos_queue_set qid:%d fail\n",
			       __func__, cfg->id.q_id);
			return DP_FAILURE;
		}
		kfree(q_cfg);
	} else if (S_NODE(cfg->type)) {
		struct pp_qos_sched_conf sch_cfg;

		if (is_sch_parent_free(priv, cfg->id.sch_id) ||
		    !is_sch_valid(cfg->id.sch_id))
			return DP_FAILURE;

		if (dp_qos_sched_conf_get(priv->qdev, cfg->id.sch_id,
					  &sch_cfg)) {
			pr_err("DPM: qos_sched_conf_get fail:%d\n", cfg->id.sch_id);
			return DP_FAILURE;
		}

		switch (cfg->cmd) {
		case DP_SHAPER_CMD_ADD:
		case DP_SHAPER_CMD_ENABLE:
			if (dp2pp_shaper_limit(cfg->cir, &bw_limit))
				return DP_FAILURE;

			sch_cfg.common_prop.bandwidth_limit = bw_limit;
			sch_cfg.common_prop.max_burst =
				BYTE_TO_BURST_SZ(quanta, cfg->cbs);
			break;
		case DP_SHAPER_CMD_REMOVE:
		case DP_SHAPER_CMD_DISABLE:
			sch_cfg.common_prop.bandwidth_limit = 0;
			break;
		default:
			pr_err("DPM: Incorrect command provided:%d\n", cfg->cmd);
			return DP_FAILURE;
		}

		if (dp_qos_sched_set(priv->qdev, cfg->id.sch_id, &sch_cfg)) {
			pr_err("DPM: qos_sched_set fail:%d\n", cfg->id.sch_id);
			return DP_FAILURE;
		}
	} else if (P_NODE(cfg->type)) {
		struct pp_qos_port_conf port_cfg;

		if (is_port_node_free(priv, cfg->id.cqm_deq_port) ||
		    !is_deqport_valid(cfg->id.cqm_deq_port))
			return DP_FAILURE;

		node_id = priv->deq_port_stat[cfg->id.cqm_deq_port].node_id;

		if (dp_qos_port_conf_get(priv->qdev, node_id, &port_cfg)) {
			pr_err("DPM: qos_port_conf_get fail:%d\n",
			       cfg->id.cqm_deq_port);
			return DP_FAILURE;
		}

		switch (cfg->cmd) {
		case DP_SHAPER_CMD_ADD:
		case DP_SHAPER_CMD_ENABLE:
			if (dp2pp_shaper_limit(cfg->cir, &bw_limit))
				return DP_FAILURE;

			port_cfg.common_prop.bandwidth_limit = bw_limit;
			port_cfg.common_prop.max_burst =
				BYTE_TO_BURST_SZ(quanta, cfg->cbs);
			break;
		case DP_SHAPER_CMD_REMOVE:
		case DP_SHAPER_CMD_DISABLE:
			port_cfg.common_prop.bandwidth_limit = 0;
			break;
		default:
			pr_err("DPM: Incorrect command provided:%d\n", cfg->cmd);
			return DP_FAILURE;
		}

		if (dp_qos_port_set(priv->qdev, node_id, &port_cfg)) {
			pr_err("DPM: qos_port_set fail:%d\n", cfg->id.cqm_deq_port);
			return DP_FAILURE;
		}
	} else {
		pr_err("DPM: %s invalid type:%d\n", __func__, cfg->type);
		return DP_FAILURE;
	}

	return DP_SUCCESS;
}

/* DP_NO_SHAPER_LIMIT no limit for shaper
 * DP_MAX_SHAPER_LIMIT max limit for shaper
 * Get shaper limit for node fill struct and return DP_SUCCESS
 * else return DP_FAILURE
 */
int _dp_shaper_conf_get(struct dp_shaper_conf *cfg, int flag)
{
	struct hal_priv *priv = HAL(cfg->inst);
	u32 quanta = 0, bw_limit, pir = 0;
	int node_id;

	/* Quanta is normally 4 in PPv4 */
	if (pp_qos_get_quanta(priv->qdev, &quanta)) {
		pr_err("DPM: %s: pp_qos_get_quanta fail: %d\n",
		       __func__, cfg->id.q_id);
		return DP_FAILURE;
	}

	if (Q_NODE(cfg->type)) {
		struct pp_qos_queue_conf *q_cfg;

		q_cfg = dp_kzalloc(sizeof(*q_cfg), GFP_ATOMIC);
		if (!q_cfg)
			return DP_FAILURE;

		if (q_node_chk(priv, cfg->id.q_id)) {
			kfree(q_cfg);
			return DP_FAILURE;
		}

		node_id = priv->qos_queue_stat[cfg->id.q_id].node_id;

		if (dp_qos_queue_conf_get(priv->qdev, node_id, q_cfg)) {
			pr_err("DPM: qos_queue_conf_get fail:%d\n", cfg->id.q_id);
			kfree(q_cfg);
			return DP_FAILURE;
		}

		if (pp2dp_shaper_limit(q_cfg->common_prop.bandwidth_limit,
				       &bw_limit)) {
			kfree(q_cfg);
			return DP_FAILURE;
		}

		pir = q_cfg->common_prop.bandwidth_limit + q_cfg->eir;

		cfg->cbs = BURST_SZ_TO_BYTE(quanta,
					    q_cfg->common_prop.max_burst);
		cfg->pbs = BURST_SZ_TO_BYTE(quanta, q_cfg->ebs) + cfg->cbs;
		kfree(q_cfg);
	} else if (S_NODE(cfg->type)) {
		struct pp_qos_sched_conf sch_cfg = {0};

		if (!is_sch_valid(cfg->id.sch_id) ||
		    is_sch_parent_free(priv, cfg->id.sch_id))
			return DP_FAILURE;

		if (dp_qos_sched_conf_get(priv->qdev, cfg->id.sch_id,
					  &sch_cfg)) {
			pr_err("DPM: qos_sched_conf_get fail:%d\n", cfg->id.sch_id);
			return DP_FAILURE;
		}

		if (pp2dp_shaper_limit(sch_cfg.common_prop.bandwidth_limit,
				       &bw_limit))
			return DP_FAILURE;

		cfg->cbs = BURST_SZ_TO_BYTE(quanta,
					    sch_cfg.common_prop.max_burst);
	} else if (P_NODE(cfg->type)) {
		struct pp_qos_port_conf port_cfg = {0};

		if (is_port_node_free(priv, cfg->id.cqm_deq_port) ||
		    !is_deqport_valid(cfg->id.cqm_deq_port))
			return DP_FAILURE;

		node_id = priv->deq_port_stat[cfg->id.cqm_deq_port].node_id;
		if (dp_qos_port_conf_get(priv->qdev, node_id, &port_cfg)) {
			pr_err("DPM: qos_port_conf_get fail:%d\n",
			       cfg->id.cqm_deq_port);
			return DP_FAILURE;
		}

		if (pp2dp_shaper_limit(port_cfg.common_prop.bandwidth_limit,
				       &bw_limit))
			return DP_FAILURE;

		cfg->cbs = BURST_SZ_TO_BYTE(quanta,
					    port_cfg.common_prop.max_burst);
	} else {
		pr_err("DPM: %s invalid type:%d\n", __func__, cfg->type);
		return DP_FAILURE;
	}

	cfg->cir = bw_limit;
	cfg->pir = pir;

	return DP_SUCCESS;
}

int _dp_queue_map_get(struct dp_queue_map_get *cfg, int flag)
{
	cbm_queue_map_entry_t *qmap_entry = NULL;
	struct hal_priv *priv = HAL(cfg->inst);
	s32 num_entry;
	int i;

	if ((is_q_node_free(priv, cfg->q_id) &&
	     cfg->q_id != priv->ppv4_drop_q) ||
	    !is_qid_valid(cfg->q_id))
		return DP_FAILURE;

	if (CBM_OPS(cfg->inst, cbm_queue_map_get, cfg->inst, cfg->q_id,
		    &num_entry, &qmap_entry, cfg->egflag)) {
		pr_err("DPM: %s cbm_queue_map_get qid:%d fail\n",
		       __func__, cfg->q_id);
		return DP_FAILURE;
	}

	cfg->num_entry = num_entry;

	if (!qmap_entry) {
		DP_DEBUG(DP_DBG_FLAG_QOS,
			 "queue map entry Null\n");
		if (num_entry) {
			pr_err("DPM: %s num_entry:%d\n", __func__, num_entry);
			return DP_FAILURE;
		} else {
			return DP_SUCCESS;
		}
	}

	if (!cfg->qmap_entry)
		goto exit;

	if (num_entry > cfg->qmap_size) {
		pr_err("DPM: %s num_entry is greater than qmap_size:%d\n",
		       __func__, num_entry);
		goto exit;
	}

	for (i = 0; i < num_entry; i++) {
		cfg->qmap_entry[i].qmap.dec = qmap_entry[i].dec;
		cfg->qmap_entry[i].qmap.enc = qmap_entry[i].enc;
		cfg->qmap_entry[i].qmap.class = qmap_entry[i].tc;
		cfg->qmap_entry[i].qmap.mpe1 = qmap_entry[i].mpe1;
		cfg->qmap_entry[i].qmap.mpe2 = qmap_entry[i].mpe2;
		cfg->qmap_entry[i].qmap.dp_port = qmap_entry[i].ep;
		cfg->qmap_entry[i].qmap.flowid = qmap_entry[i].flowid;
		cfg->qmap_entry[i].qmap.subif = qmap_entry[i].sub_if_id;
	}

exit:
	CBM_OPS(cfg->inst, cbm_queue_map_buf_free, qmap_entry);
	qmap_entry = NULL;

	return DP_SUCCESS;
}

int _dp_queue_map_set(struct dp_queue_map_set *cfg, int flag)
{
	cbm_queue_map_entry_t qmap_cfg = {0};
	u32 cqm_flags = 0;

	if (!is_qid_valid(cfg->q_id))
		return DP_FAILURE;

	qmap_cfg.dec = cfg->map.dec;
	qmap_cfg.enc = cfg->map.enc;
	qmap_cfg.tc = cfg->map.class;
	qmap_cfg.mpe1 = cfg->map.mpe1;
	qmap_cfg.mpe2 = cfg->map.mpe2;
	qmap_cfg.ep = cfg->map.dp_port;
	qmap_cfg.flowid = cfg->map.flowid;
	qmap_cfg.egflag = cfg->map.egflag;
	qmap_cfg.sub_if_id = cfg->map.subif;

	if (cfg->mask.mpe1)
		cqm_flags |= CBM_QUEUE_MAP_F_MPE1_DONTCARE;
	if (cfg->mask.mpe2)
		cqm_flags |= CBM_QUEUE_MAP_F_MPE2_DONTCARE;
	if (cfg->mask.dp_port)
		cqm_flags |= CBM_QUEUE_MAP_F_EP_DONTCARE;
	if (cfg->mask.flowid)
		cqm_flags |= CBM_QUEUE_MAP_F_FLOWID_L_DONTCARE |
			     CBM_QUEUE_MAP_F_FLOWID_H_DONTCARE;
	if (cfg->mask.dec)
		cqm_flags |= CBM_QUEUE_MAP_F_DE_DONTCARE;
	if (cfg->mask.enc)
		cqm_flags |= CBM_QUEUE_MAP_F_EN_DONTCARE;
	if (cfg->mask.class)
		cqm_flags |= CBM_QUEUE_MAP_F_TC_DONTCARE;
	if (cfg->mask.dp_port)
		cqm_flags |= CBM_QUEUE_MAP_F_EP_DONTCARE;
	if (cfg->mask.subif) {
		cqm_flags |= CBM_QUEUE_MAP_F_SUBIF_DONTCARE;
		qmap_cfg.sub_if_id_mask_bits = cfg->mask.subif_id_mask;
	}
	if (cfg->mask.egflag)
		cqm_flags |= CBM_QUEUE_MAP_F_EGFLAG_DONTCARE;

	if (CBM_OPS(cfg->inst, cbm_queue_map_set, cfg->inst,
		    cfg->q_id, &qmap_cfg, cqm_flags)) {
		pr_err("DPM: %s cbm_queue_map_set fail for qid:%d\n",
		       __func__, cfg->q_id);
		return DP_FAILURE;
	}

	return DP_SUCCESS;
}

int _dp_counter_mode_set(struct dp_counter_conf *cfg, int flag)
{
	return DP_FAILURE;
}

int _dp_counter_mode_get(struct dp_counter_conf *cfg, int flag)
{
	return DP_FAILURE;
}

static int get_sch_level(int inst, int pid, int flag)
{
	struct hal_priv *priv = HAL(inst);
	int level;

	for (level = 0; level < DP_MAX_SCH_LVL; level++) {
		if (P_NODE(priv->qos_sch_stat[pid].parent.type)) {
			level++;
			break;
		}
		pid = priv->qos_sch_stat[pid].parent.node_id;
	}

	return level;
}

/* Get max scheduler level and return DP_SUCCESS
 * else return DP_FAILURE
 */
int _dp_qos_max_level_get(struct dp_qos_level *dp, int flag)
{
	struct hal_priv *priv = HAL(dp->inst);
	u16 i, id, pid, lvl_x = 0;

	dp->max_sch_lvl = 0;

	for (i = 0; i < DP_MAX_QUEUE_NUM; i++) {
		if (priv->qos_queue_stat[i].flag & PP_NODE_FREE)
			continue;

		id = priv->qos_queue_stat[i].node_id;

		if (P_NODE(priv->qos_sch_stat[id].parent.type)) {
			continue;
		} else if (S_NODE(priv->qos_sch_stat[id].parent.type)) {
			pid = priv->qos_sch_stat[id].parent.node_id;
			lvl_x = get_sch_level(dp->inst, pid, 0);
		}

		if (lvl_x > dp->max_sch_lvl)
			dp->max_sch_lvl = lvl_x;
	}

	if (dp->max_sch_lvl >= 0)
		return DP_SUCCESS;
	else
		return DP_FAILURE;
}

static int get_children_list(int inst, struct dp_node *child, int node_id)
{
	struct hal_priv *priv = HAL(inst);
	int i, num = 0;

	for (i = 0; i < DP_MAX_CHILD_PER_NODE; i++) {
		if (!PP_ACT(CHILD(node_id, i).flag))
			continue;

		child[i].type = CHILD(node_id, i).type;
		if (S_NODE(child[i].type))
			child[i].id.q_id = CHILD(node_id, i).node_id;
		else
			child[i].id.q_id =
			      get_qid_by_node(inst, CHILD(node_id, i).node_id);
		num++;
	}

	return num;
}

/* Get direct chldren and type of given node and return DP_SUCCESS
 * else return DP_FAILURE
 */
int _dp_children_get(struct dp_node_child *cfg, int flag)
{
	struct hal_priv *priv = HAL(cfg->inst);
	int node_id, res = 0;

	if (S_NODE(cfg->type)) {
		if (!is_sch_valid(cfg->id.sch_id) ||
		    is_sch_child_free(priv, cfg->id.sch_id))
			return DP_FAILURE;

		node_id = cfg->id.sch_id;
	} else if (P_NODE(cfg->type)) {
		if (!is_deqport_valid(cfg->id.cqm_deq_port) ||
		    is_port_node_free(priv, cfg->id.cqm_deq_port))
			return DP_FAILURE;

		node_id = priv->deq_port_stat[cfg->id.cqm_deq_port].node_id;
	} else {
		pr_err("DPM: %s unkonwn type provided:0x%x\n", __func__, cfg->type);
		return DP_FAILURE;
	}

	if (!priv->qos_sch_stat[node_id].child_num)
		return DP_SUCCESS;

	cfg->num = priv->qos_sch_stat[node_id].child_num;
	res = get_children_list(cfg->inst, cfg->child, node_id);

	if (cfg->num == res)
		return DP_SUCCESS;

	pr_err("DPM: child_num:[%d] not matched to res:[%d] for Node:%d\n",
	       cfg->num, res, cfg->id.sch_id);

	return DP_FAILURE;
}

static int dp_q_reserve_contiguous(int inst, int ep, struct dp_dev_data *data)
{
	struct hal_priv *priv = HAL(inst);
	struct resv_info *r = &priv->resv[ep];
	u32 *ids, *phy_ids;
	int i, res;
	size_t len;

	len = sizeof(*ids) * data->num_resv_q * 2;
	ids = dp_kzalloc(len, GFP_ATOMIC);
	if (!ids)
		return DP_FAILURE;

	phy_ids = ids + data->num_resv_q;
	res = pp_qos_contiguous_queue_allocate(priv->qdev, ids, phy_ids,
					       data->num_resv_q);
	if (res) {
		pr_err("DPM: %s: pp_qos_contiguous_queue_allocate fail %d\n",
		       __func__, res);
		res = DP_FAILURE;
	} else {
		for (i = 0; i < data->num_resv_q; i++) {
			r->resv_q[i].id = ids[i];
			r->resv_q[i].physical_id = phy_ids[i];
			DP_DEBUG(DP_DBG_FLAG_QOS, "%s ep:%d resv Q[%d/%d]\n",
				 __func__, ep, ids[i], phy_ids[i]);
		}

		r->num_resv_q = data->num_resv_q;
		r->flag_ops |= DP_F_DEV_CONTINUOUS_Q;
		data->qos_resv_q_base = phy_ids[0];
		res = DP_SUCCESS;
	}

	kfree(ids);

	return res;
}

static int dp_q_remove(int inst, int ep)
{
	struct hal_priv *priv = HAL(inst);
	struct resv_info *r = &priv->resv[ep];
	int i, res = DP_FAILURE;

	if (!r->resv_q) {
		r->num_resv_q = 0;
		return DP_SUCCESS;
	}
	if (r->num_resv_q <= 0) {
		pr_err("DPM: why num_resv_q(%d)<=0 but resv_q(0x%px) not NULL\n",
			r->num_resv_q, r->resv_q);
		goto exit;
	}

	if (r->flag_ops & DP_F_DEV_CONTINUOUS_Q) {
		res = pp_qos_contiguous_queue_remove(priv->qdev,
						     r->resv_q[0].id,
						     r->num_resv_q);
		if (res) {
			pr_err("DPM: %s pp_qos_contiguous_queue_remove fail %d\n",
			       __func__, res);
			goto exit;
		}
		r->flag_ops &= ~DP_F_DEV_CONTINUOUS_Q;
		res = DP_SUCCESS;
		goto exit;
	}

	while (r->num_resv_q) {
		i = r->num_resv_q - 1;
		res = dp_qos_queue_remove(priv->qdev, r->resv_q[i].id);
		if (res) {
			pr_err("DPM: %s: dp_qos_queue_remove fail %d for qid %d\n",
			       __func__, res, r->resv_q[i].id);
			return DP_FAILURE;
		}
		r->num_resv_q--;
	}
	res = DP_SUCCESS;

exit:
	kfree(r->resv_q);
	r->resv_q = NULL;
	r->num_resv_q = 0;

	return res;
}

static int dp_q_reserve(int inst, int ep, struct dp_dev_data *data)
{
	struct hal_priv *priv = HAL(inst);
	struct resv_info *r = &priv->resv[ep];
	size_t len;
	int i, res;
	u32 id, phy_id;

	if (!(data->flag_ops & DP_F_DEV_RESV_Q) || data->num_resv_q <= 0) {
		if (r->resv_q) {
			pr_err("DPM: Why initial resv_q not NULL:0x%px\n", r->resv_q);
			pr_err("DPM: may have memory issue\n");
			r->resv_q = NULL;
			r->num_resv_q = 0;
			return DP_SUCCESS;
		}
		return DP_SUCCESS;
	}

	len = sizeof(struct resv_q) * data->num_resv_q;
	r->resv_q = dp_kzalloc(len, GFP_ATOMIC);
	if (!r->resv_q)
		return DP_FAILURE;

	if (data->flag_ops & DP_F_DEV_CONTINUOUS_Q) {
		if (dp_q_reserve_contiguous(inst, ep, data)) {
			dp_q_remove(inst, ep);
			return DP_FAILURE;
		}
		return DP_SUCCESS;
	}
	for (i = 0; i < data->num_resv_q; i++) {
		res = dp_qos_queue_allocate_id_phy(priv->qdev, &id, &phy_id);
		if (res) {
			pr_err("DPM: %s: dp_qos_queue_allocate_id_phy fail %d\n",
			       __func__, res);
			dp_q_remove(inst, ep);
			return DP_FAILURE;
		}

		r->resv_q[i].id = id;
		r->resv_q[i].physical_id = phy_id;
		r->num_resv_q++;

		DP_DEBUG(DP_DBG_FLAG_QOS, "%s: reseve Q[%d/%d] for ep %d\n",
			 __func__, id, phy_id, ep);
	}

	return DP_SUCCESS;
}

static int dp_sched_remove(int inst, int ep)
{
	struct hal_priv *priv = HAL(inst);
	struct resv_info *r = &priv->resv[ep];
	int i, res = DP_FAILURE;

	if (!r->resv_sched) {
		r->num_resv_sched = 0;
		return DP_SUCCESS;
	}
	if (r->num_resv_sched <= 0) {
		pr_err("DPM: why num_resv_sched(%d)<=0 but resv_sched(0x%px) not NULL\n",
			r->num_resv_sched, r->resv_sched);
		goto exit;
	}

	DP_DEBUG(DP_DBG_FLAG_QOS, "num_resv_sched=%d\n", r->num_resv_sched);
	while (r->num_resv_sched) {
		i = r->num_resv_sched - 1;
		res = dp_qos_sched_remove(priv->qdev, r->resv_sched[i].id);
		if (res) {
			pr_err("DPM: %s: dp_qos_sched_remove fail %d for sch %d\n",
			       __func__, res, r->resv_sched[i].id);
			goto exit;
		}
		r->num_resv_sched--;
	}
	res = DP_SUCCESS;
exit:
	kfree(r->resv_sched);
	r->resv_sched = NULL;
	r->num_resv_sched = 0;

	return res;
}

static int dp_sched_reserve(int inst, int ep, struct dp_dev_data *data)
{
	struct hal_priv *priv = HAL(inst);
	struct resv_info *r = &priv->resv[ep];
	size_t len;
	int i, res;
	u32 id;

	if (!(data->flag_ops & DP_F_DEV_RESV_SCH) ||
	    data->num_resv_sched <= 0) {
		if (r->resv_sched) {
			pr_err("DPM: Why initial resv_sched not NULL:0x%px\n",
			       r->resv_sched);
			r->resv_sched = NULL;
			r->num_resv_sched = 0;
			return DP_FAILURE;
		}
		return DP_SUCCESS;
	}

	len = sizeof(struct resv_sch) * data->num_resv_sched;
	r->resv_sched = dp_kzalloc(len, GFP_ATOMIC);
	if (!r->resv_sched)
		return DP_FAILURE;

	for (i = 0; i < data->num_resv_sched; i++) {
		res = dp_qos_sched_allocate(priv->qdev, &id);
		if (res) {
			pr_err("DPM: %s: dp_qos_sched_allocate fail %d\n",
			       __func__, res);
			dp_sched_remove(inst, ep);
			return DP_FAILURE;
		}
		r->num_resv_sched++;
		r->resv_sched[i].id = id;

		DP_DEBUG(DP_DBG_FLAG_QOS, "%s Sch:%d for ep %d\n",
			 __func__, id, ep);
	}

	return DP_SUCCESS;
}

int dp_remove_reserved_node(int inst, int ep, int flags)
{
	dp_sched_remove(inst, ep);
	dp_q_remove(inst, ep);
	return DP_SUCCESS;
}

void dump_resv_info(int inst, int ep)
{
	struct hal_priv *priv = HAL(inst);
	__maybe_unused struct resv_info *r = &priv->resv[ep];

	DP_DUMP("dp_node_reserve info: inst=%d ep=%d\n", inst, ep);
	DP_DUMP("   resv_q=%lx\n   num_resv_q=%d\n",
		(unsigned long)r->resv_q,
		r->num_resv_q);
	DP_DUMP("   resv_sched=%lx\n   num_resv_sched=%d\n",
		(unsigned long) r->resv_sched,
		r->num_resv_sched);
}

int dp_node_reserve(int inst, int ep, struct dp_dev_data *data, int flags)
{
	if (flags & DP_F_DEREGISTER)
		return dp_remove_reserved_node(inst, ep, flags);

	/* Data can be null in the case of CPU Port platform_set */
	if (!data)
		return DP_SUCCESS;

	if(dp_q_reserve(inst, ep, data))
		return DP_FAILURE;
	if (dp_sched_reserve(inst, ep, data))
		goto error;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (unlikely(dp_dbg_flag & DP_DBG_FLAG_QOS))
		dump_resv_info(inst, ep);
#endif
	return DP_SUCCESS;
error:
	dp_q_remove(inst, ep);

	return DP_FAILURE;
}

int dp_get_q_logic(int inst, int qid)
{
	struct hal_priv *priv = HAL(inst);

	if (!priv) {
		pr_err("DPM: %s priv NULL\n", __func__);
		return PP_QOS_INVALID_ID;
	}

	return pp_qos_queue_id_get(priv->qdev, qid);
}

int dp_get_queue_logic(struct dp_qos_q_logic *cfg, int flag)
{
	cfg->q_logic_id = dp_get_q_logic(cfg->inst, cfg->q_id);
	if (cfg->q_logic_id == PP_QOS_INVALID_ID) {
		pr_err("DPM: %s dp_get_q_logic fail\n", __func__);
		return DP_FAILURE;
	}

	return DP_SUCCESS;
}

int _dp_qos_global_info_get(struct dp_qos_cfg_info *info, int flag)
{
	struct hal_priv *priv = HAL(info->inst);
	struct cqm_port_info *cqm_info;
	u32 quanta = 0;

	if (pp_qos_get_quanta(priv->qdev, &quanta)) {
		pr_err("DPM: %s pp_qos_get_quanta fail\n", __func__);
		return DP_FAILURE;
	}

	info->quanta = quanta;

	/* Check if reinsert flow is configured */
	if (reinsert_deq_port[info->inst]) {
		cqm_info = get_dp_deqport_info(info->inst,
					       reinsert_deq_port[info->inst]);
		info->reinsert_deq_port = reinsert_deq_port[info->inst];
		if (!is_qid_valid(cqm_info->qid[0]))
			info->reinsert_qid = -1;
		else
			info->reinsert_qid = cqm_info->qid[0];
	} else {
		info->reinsert_qid = -1;
		info->reinsert_deq_port = -1;
	}

	DP_DEBUG(DP_DBG_FLAG_QOS,
		 "quanta=%d, reinsert_deq_port = %d, reinsert_qid = %d\n",
		 quanta, info->reinsert_deq_port, info->reinsert_qid);

	return DP_SUCCESS;
}

int _dp_qos_port_conf_set(struct dp_port_cfg_info *info, int flag)
{
	struct hal_priv *priv = HAL(info->inst);
	struct pp_qos_port_conf port_cfg = {0};
	int node_id;

	if (!is_deqport_valid(info->pid) || is_port_node_free(priv, info->pid))
		return DP_FAILURE;

	node_id = priv->deq_port_stat[info->pid].node_id;

	DP_DEBUG(DP_DBG_FLAG_QOS, "%s cqm_deq:%d, qos_port:%d\n",
		 __func__, info->pid, node_id);

	if (dp_qos_port_conf_get(priv->qdev, node_id, &port_cfg)) {
		pr_err("DPM: %s dp_qos_port_conf_get P:%d fail\n",
		       __func__, node_id);
		return DP_FAILURE;
	}

	if (flag & DP_PORT_CFG_GREEN_THRESHOLD)
		port_cfg.green_threshold = info->green_threshold;

	if (flag & DP_PORT_CFG_YELLOW_THRESHOLD)
		port_cfg.yellow_threshold = info->yellow_threshold;

	if (flag & DP_PORT_CFG_EWSP)
		port_cfg.enhanced_wsp = info->ewsp;

	if (dp_qos_port_set(priv->qdev, node_id, &port_cfg)) {
		pr_err("DPM: %s set yellow:%d green:%d for P:%d fail\n", __func__,
		       info->yellow_threshold, info->green_threshold, node_id);
		return DP_FAILURE;
	}

	return DP_SUCCESS;
}

int dp_queue_block_flush(struct dp_qos_blk_flush_queue *cfg, int flag)
{
	struct hal_priv *priv = HAL(cfg->inst);
	struct pp_queue_stat *q_stat;
	int node_id;

	if (q_node_chk(priv, cfg->q_id))
		return DP_FAILURE;

	q_stat = &priv->qos_queue_stat[cfg->q_id];
	node_id = q_stat->node_id;

	/* Flush the queue and restore back to original deq port */
	if (queue_flush(cfg->inst, node_id,
			flag | DP_QFLUSH_FLAG_RESTORE_QOS_PORT)) {
		pr_err("DPM: %s queue_flush qid:%d fail\n",
		       __func__, cfg->q_id);
		return DP_FAILURE;
	}

	if (!(flag & DP_QFLUSH_FLAG_CONTINUE_BLOCK_QUEUE)) {
		/* Unblock the q, since q is mapped to drop port */
		if (pp_qos_queue_unblock(priv->qdev, node_id)) {
			pr_err("DPM: %s pp_qos_queue_unblock qid:%d fail\n",
			       __func__, cfg->q_id);
			return DP_FAILURE;
		}

		q_stat->blocked = 0;
	}

	return DP_SUCCESS;
}

int dp_port_block_flush(struct dp_qos_blk_flush_port *cfg, int flag)
{
	struct hal_priv *priv = HAL(cfg->inst);
	struct dp_qos_blk_flush_queue q_cfg;
	struct pmac_port_info *port_info;
	u32 q_num;
	u16 i, k, idx, num, start;
	u16 *qids;

	port_info = get_dp_port_info(cfg->inst, cfg->dp_port);
	if (!port_info->deq_port_num)
		return DP_FAILURE;

	if (cfg->deq_port_idx == DEQ_PORT_OFFSET_ALL) {
		start = 0;
		num = port_info->deq_port_num;
	} else { /*find the relative index */
		if (cfg->deq_port_idx >= port_info->deq_port_num) {
			pr_err("DPM: %s: Not found cqm deq port %d under dp_port %d\n",
			       __func__, cfg->deq_port_idx, cfg->dp_port);
			return DP_FAILURE;
		}
		start = cfg->deq_port_idx;
		num = 1;
	}
	qids = dp_kmalloc(sizeof(u16) * MAX_Q_PER_PORT, GFP_ATOMIC);
	if (!qids)
		return DP_FAILURE;
	for (k = start; k < (num + start); k++) {
		idx = port_info->deq_ports[k];
		if (!is_deqport_valid(idx)) {
			pr_err("DPM: wrong cqm_deq_port:%d\n", idx);
			goto err;
		}
		if (priv->deq_port_stat[idx].flag == PP_NODE_FREE) {
			/* Ignore free ports when we flush all */
			if (cfg->deq_port_idx == DEQ_PORT_OFFSET_ALL)
				continue;
			pr_err("DPM: port is free cqm_deq_port:%d\n", idx);
			goto err;
		}
		if (dp_qos_port_get_queues(priv->qdev,
					   priv->deq_port_stat[idx].node_id,
					   qids, MAX_Q_PER_PORT, &q_num)) {
			pr_err("DPM: %s: P[%d/%d]\n", __func__, idx,
			       priv->deq_port_stat[idx].node_id);
			goto err;
		}
		q_cfg.inst = cfg->inst;

		/* Even if fail, continue to block and flush other queues */
		for (i = 0; i < q_num; i++) {
			q_cfg.q_id = get_qid_by_node(cfg->inst, qids[i]);
			dp_queue_block_flush(&q_cfg,
				flag | DP_QFLUSH_FLAG_CONTINUE_BLOCK_QUEUE);
		}
	}
	kfree(qids);
	return DP_SUCCESS;
err:
	kfree(qids);
	return DP_FAILURE;
}

int dp_get_queue_mib(struct dp_qos_queue_info *info, int flag)
{
	struct pp_qos_queue_stat q_stat = {0};
	struct hal_priv *priv;

	if (!info) {
		pr_err("DPM: %s dp_qos_queue_info NULL\n", __func__);
		return DP_FAILURE;
	}

	priv = HAL(info->inst);
	if (!priv) {
		pr_err("DPM: %s priv NULL\n", __func__);
		return DP_FAILURE;
	}

	if (pp_qos_queue_stat_get(priv->qdev, info->nodeid, &q_stat)) {
		pr_err("DPM: pp_qos_queue_stat_get Q:%d fail\n", info->nodeid);
		return DP_FAILURE;
	}

	info->qocc = q_stat.queue_packets_occupancy;
	info->qacc = q_stat.total_packets_accepted;
	info->qdrp = q_stat.total_packets_dropped;
#ifdef DP_QOS_QUEUE_INFO_HAS_BYTE_STATS
	info->qocc_bytes = q_stat.queue_bytes_occupancy;
	info->qacc_bytes = q_stat.total_bytes_accepted;
	info->qdrp_bytes = q_stat.total_bytes_dropped;

	DP_DEBUG(DP_DBG_FLAG_QOS,
		 "%s: Q:%d, qocc=%d, qacc=%d, qdrp=%d, qocc_b=%llu, qacc_b=%llu, qdrp_b=%llu\n",
		 __func__, info->nodeid, info->qocc, info->qacc, info->qdrp,
		 info->qocc_bytes, info->qacc_bytes, info->qdrp_bytes);
#endif

	return DP_SUCCESS;
}

int dp_codel_cfg_set(struct dp_qos_codel_cfg *cfg, int flag)
{
	struct hal_priv *priv = HAL(0);
	struct pp_qos_codel_cfg pp_cfg = {0};

	pp_cfg.target_delay_msec = cfg->target_delay_msec;
	pp_cfg.interval_time_msec = cfg->interval_time_msec;

	DP_DUMP("QoS Dev 0x%px \n", priv->qdev);

	if (pp_qos_codel_cfg_set(priv->qdev, &pp_cfg)) {
		pr_err("DPM: %s dp_qos_codel_cfg_set fail.\n",
			__func__);
		return DP_FAILURE;
	}

	return DP_SUCCESS;
}

int dp_codel_cfg_get(struct dp_qos_codel_cfg *cfg, int flag)
{
	struct hal_priv *priv = HAL(0);
	struct pp_qos_codel_cfg pp_cfg = {0};

	DP_DUMP("QoS Dev 0x%px \n", priv->qdev);

	if (pp_qos_codel_cfg_get(priv->qdev, &pp_cfg)) {
		pr_err("DPM: %s _dp_qos_codel_cfg_get fail.\n",
				__func__);
		return DP_FAILURE;
	}

	cfg->target_delay_msec = pp_cfg.target_delay_msec;
	cfg->interval_time_msec = pp_cfg.interval_time_msec;

	return DP_SUCCESS;
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP
