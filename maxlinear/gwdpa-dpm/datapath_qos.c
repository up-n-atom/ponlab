// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2021, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <net/datapath_api.h>
#include <net/datapath_api_qos.h>
#include "datapath.h"
#include "datapath_trace.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

/*Datapath arbi enum to string*/
char *dp_arbi_to_str(enum dp_arbitate ar)
{
	switch (ar) {
		case ARBITRATION_NULL:
			return "ARBI_NULL";
		case ARBITRATION_WRR:
			return "ARBI_WRR";
		case ARBITRATION_SP:
			return "ARBI_SP";
		case ARBITRATION_WSP:
			return "ARBI_WSP";
		case ARBITRATION_WSP_WRR:
			return "ARBI_WSP_WRR";
		case ARBITRATION_WFQ:
			return "ARBI_WFQ";
		case ARBITRATION_PARENT:
			return "ARBI_PARENT";
		default:
			return "ARBI_UNKNOWN";
	}
	return NULL;
}

/*Datapath qos flag to string */
char *dp_qos_flag_to_str(enum QOS_FLAG qf)
{
	switch (qf) {
		case LINK_ADD:
			return "LINK_ADD";
		case LINK_GET:
			return "LINK_ADD";
		case LINK_PRIO_SET:
			return "LINK_PRIO_SET";
		case LINK_PRIO_GET:
			return "NODE_PRIO_GET";
		case NODE_LINK_ADD:
			return "NODE_LINK_ADD";
		case NODE_LINK_GET:
			return "NODE_LINK_GET";
		case NODE_LINK_EN_GET:
			return "NODE_LINK_EN_GET";
		case NODE_LINK_EN_SET:
			return "NODE_LINK_EN_SET";
		case NODE_UNLINK:
			return "NODE_UNLINK";
		case NODE_ALLOC:
			return "NODE_ALLOC";
		case NODE_FREE:
			return "NODE_FREE";
		case NODE_CHILDREN_GET:
			return "NODE_CHILDREN_GET";
		case NODE_CHILDREN_FREE:
			return "NODE_CHILDREN_FREE";
		case SHAPER_SET:
			return "SHAPER_SET";
		case SHAPER_GET:
			return "SHAPER_GET";
		case DEQ_PORT_RES_GET:
			return "DEQ_PORT_RES_GET";
		case COUNTER_MODE_SET:
			return "COUNTER_MODE_SET";
		case COUNTER_MODE_GET:
			return "COUNTER_MODE_GET";
		case QUEUE_CFG_SET:
			return "QUEUE_CFG_SET";
		case QUEUE_CFG_GET:
			return "QUEUE_CFG_GET";
		case QUEUE_MAP_GET:
			return "QUEUE_MAP_GET";
		case QUEUE_MAP_SET:
			return "QUEUE_MAP_SET";
		case QOS_LEVEL_GET:
			return "QOS_LEVEL_GET";
		case QOS_Q_LOGIC:
			return "QOS_Q_LOGIC";
		case QOS_GLOBAL_CFG_GET:
			return "QOS_GLOBAL_CFG_GET";
		case QOS_PORT_CFG_SET:
			return "QOS_PORT_CFG_SET";
		case QOS_BLOCK_FLUSH_PORT:
			return "QOS_BLOCK_FLUSH_PORT";
		case QOS_BLOCK_FLUSH_QUEUE:
			return "QOS_BLOCK_FLUSH_QUEUE";
		case CODEL_GET:
			return "CODEL_GET";
		case CODEL_SET:
			return "CODEL_SET";
		default:
			return "UNKOWN_QOS_FLAG";
	}
	return NULL;
}

int dp_node_link_add2(struct dp_node_link *info, int flag)
{
	struct inst_info *dp_info;
	struct dp_node_link old_cfg;
	int ret;

	if (unlikely(!dp_init_ok && !dp_cpu_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!info || is_invalid_inst(info->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(info->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;

	old_cfg = *info;
	ret = dp_info->dp_qos_platform_set(NODE_LINK_ADD, info, flag);
	trace_dp_node_link_add(ret, &old_cfg, info, NODE_LINK_ADD, flag);

	return ret;
}
EXPORT_SYMBOL(dp_node_link_add2);

int dp_node_unlink2(struct dp_node_link *info, int flag)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!info || is_invalid_inst(info->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(info->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	return dp_info->dp_qos_platform_set(NODE_UNLINK, info, flag);
}
EXPORT_SYMBOL(dp_node_unlink2);

int dp_node_link_get(struct dp_node_link *info, int flag)
{
	struct inst_info *dp_info;
	int res;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!info || is_invalid_inst(info->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(info->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	res = dp_info->dp_qos_platform_set(NODE_LINK_GET, info, flag);
	trace_dp_node_link_get(res, info, NODE_LINK_GET, flag);
	return res;
}
EXPORT_SYMBOL(dp_node_link_get);

int dp_node_link_en_set(struct dp_node_link_enable *en, int flag)
{
	struct inst_info *dp_info;
	int res;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!en || is_invalid_inst(en->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(en->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	res = dp_info->dp_qos_platform_set(NODE_LINK_EN_SET, en, flag);

	trace_dp_node_link_en_set(res, en, NODE_LINK_EN_SET, flag);

	return res;
}
EXPORT_SYMBOL(dp_node_link_en_set);

int dp_node_link_en_get(struct dp_node_link_enable *en, int flag)
{
	struct inst_info *dp_info;
	int ret;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!en || is_invalid_inst(en->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(en->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	ret = dp_info->dp_qos_platform_set(NODE_LINK_EN_GET, en, flag);
	trace_dp_node_link_en_get(ret, en, NODE_LINK_EN_GET, flag);

	return ret;
}
EXPORT_SYMBOL(dp_node_link_en_get);

int dp_link_add(struct dp_qos_link *cfg, int flag)
{
	struct inst_info *dp_info;
	struct dp_qos_link old_cfg = {};
	int ret = DP_FAILURE;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		goto exit;
	}

	if (!cfg || is_invalid_inst(cfg->inst))
		goto exit;

	dp_info = get_dp_prop_info(cfg->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		goto exit;
	old_cfg = *cfg;
	ret = dp_info->dp_qos_platform_set(LINK_ADD, cfg, flag);
exit:
	trace_dp_link_add(ret, &old_cfg, cfg, LINK_ADD, flag);
	return ret;
}
EXPORT_SYMBOL(dp_link_add);

int dp_link_get(struct dp_qos_link *cfg, int flag)
{
	struct inst_info *dp_info;
	int ret = DP_FAILURE;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		goto exit;
	}

	if (!cfg || is_invalid_inst(cfg->inst))
		goto exit;

	dp_info = get_dp_prop_info(cfg->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		goto exit;
	ret = dp_info->dp_qos_platform_set(LINK_GET, cfg, flag);
exit:
	trace_dp_link_get(ret, cfg, LINK_GET, flag);
	return ret;
}
EXPORT_SYMBOL(dp_link_get);

int dp_qos_link_prio_set2(struct dp_node_prio *info, int flag)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!info || is_invalid_inst(info->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(info->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	return dp_info->dp_qos_platform_set(LINK_PRIO_SET, info, flag);
}
EXPORT_SYMBOL(dp_qos_link_prio_set2);

int dp_qos_link_prio_get2(struct dp_node_prio *info, int flag)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!info || is_invalid_inst(info->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(info->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	return dp_info->dp_qos_platform_set(LINK_PRIO_GET, info, flag);
}
EXPORT_SYMBOL(dp_qos_link_prio_get2);

int dp_queue_conf_set2(struct dp_queue_conf *cfg, int flag)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!cfg || is_invalid_inst(cfg->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(cfg->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	return dp_info->dp_qos_platform_set(QUEUE_CFG_SET, cfg, flag);
}
EXPORT_SYMBOL(dp_queue_conf_set2);

int dp_queue_conf_get2(struct dp_queue_conf *cfg, int flag)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!cfg || is_invalid_inst(cfg->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(cfg->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	return dp_info->dp_qos_platform_set(QUEUE_CFG_GET, cfg, flag);
}
EXPORT_SYMBOL(dp_queue_conf_get2);

int dp_shaper_conf_set2(struct dp_shaper_conf *cfg, int flag)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!cfg || is_invalid_inst(cfg->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(cfg->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	return dp_info->dp_qos_platform_set(SHAPER_SET, cfg, flag);
}
EXPORT_SYMBOL(dp_shaper_conf_set2);

int dp_shaper_conf_get2(struct dp_shaper_conf *cfg, int flag)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!cfg || is_invalid_inst(cfg->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(cfg->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	return dp_info->dp_qos_platform_set(SHAPER_GET, cfg, flag);
}
EXPORT_SYMBOL(dp_shaper_conf_get2);

int dp_node_alloc2(struct dp_node_alloc *node, int flag)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!node || is_invalid_inst(node->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(node->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	return dp_info->dp_qos_platform_set(NODE_ALLOC, node, flag);
}
EXPORT_SYMBOL(dp_node_alloc2);

int dp_node_free2(struct dp_node_alloc *node, int flag)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!node || is_invalid_inst(node->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(node->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	return dp_info->dp_qos_platform_set(NODE_FREE, node, flag);
}
EXPORT_SYMBOL(dp_node_free2);

int dp_node_children_free(struct dp_node_alloc *node, int flag)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!node || is_invalid_inst(node->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(node->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	return dp_info->dp_qos_platform_set(NODE_CHILDREN_FREE, node, flag);
}
EXPORT_SYMBOL(dp_node_children_free);

int dp_deq_port_res_get2(struct dp_dequeue_res *res, int flag)
{
	dp_subif_t *subif;
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!res || is_invalid_inst(res->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(res->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;

	/* if res->dev is NULL, res->dp_port should be valid */
	if (res->dev) { /*fill dp_port if dev is provided */
		struct pmac_port_info *port;
		struct dp_subif_info *sif;
		subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
		if (!subif)
			return DP_FAILURE;
		dp_get_netif_subifid(res->dev, NULL, NULL, NULL, subif, 0);
		if (!subif->subif_num) {
			pr_err("DPM: Not found dev %s\n", res->dev->name);
			kfree(subif);
			return DP_FAILURE;
		}
		port = get_dp_port_info(res->inst, subif->port_id);
		sif = get_dp_port_subif(port, subif->subif_groupid);
		res->dp_port = subif->port_id;
		res->cqm_deq_idx = sif->cqm_port_idx;
		res->num_deq_ports = subif->num_q;
		kfree(subif);
		subif = NULL;
	}
	DP_DEBUG(DP_DBG_FLAG_QOS_DETAIL, "%s: dp_port=%d tconf_idx=%d\n",
		 __func__, res->dp_port, res->cqm_deq_idx);
	return dp_info->dp_qos_platform_set(DEQ_PORT_RES_GET, res, flag);
}
EXPORT_SYMBOL(dp_deq_port_res_get2);

int dp_counter_mode_set(struct dp_counter_conf *cfg, int flag)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!cfg || is_invalid_inst(cfg->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(cfg->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	return dp_info->dp_qos_platform_set(COUNTER_MODE_SET, cfg, flag);
}
EXPORT_SYMBOL(dp_counter_mode_set);

int dp_counter_mode_get(struct dp_counter_conf *cfg, int flag)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!cfg || is_invalid_inst(cfg->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(cfg->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	return dp_info->dp_qos_platform_set(COUNTER_MODE_GET, cfg, flag);
}
EXPORT_SYMBOL(dp_counter_mode_get);

int dp_queue_map_set2(struct dp_queue_map_set *cfg, int flag)
{
	struct inst_info *dp_info;
	int ret;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!cfg || is_invalid_inst(cfg->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(cfg->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	ret = dp_info->dp_qos_platform_set(QUEUE_MAP_SET, cfg, flag);
	trace_dp_queue_map_set(ret, cfg, QUEUE_MAP_SET, flag);

	return ret;
}
EXPORT_SYMBOL(dp_queue_map_set2);

int dp_queue_map_get2(struct dp_queue_map_get *cfg, int flag)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!cfg || is_invalid_inst(cfg->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(cfg->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	return dp_info->dp_qos_platform_set(QUEUE_MAP_GET, cfg, flag);
}
EXPORT_SYMBOL(dp_queue_map_get2);

int dp_children_get(struct dp_node_child *cfg, int flag)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!cfg || is_invalid_inst(cfg->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(cfg->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	return dp_info->dp_qos_platform_set(NODE_CHILDREN_GET, cfg, flag);
}
EXPORT_SYMBOL(dp_children_get);

int dp_qos_level_get(struct dp_qos_level *cfg, int flag)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!cfg || is_invalid_inst(cfg->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(cfg->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	return dp_info->dp_qos_platform_set(QOS_LEVEL_GET, cfg, flag);
}
EXPORT_SYMBOL(dp_qos_level_get);

int dp_qos_get_q_logic2(struct dp_qos_q_logic *cfg, int flag)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!cfg || is_invalid_inst(cfg->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(cfg->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set) {
		cfg->q_logic_id = cfg->q_id; /* For GRX500 */
		return DP_SUCCESS;
	}
	return dp_info->dp_qos_platform_set(QOS_Q_LOGIC, cfg, flag);
}
EXPORT_SYMBOL(dp_qos_get_q_logic2);

int dp_qos_global_info_get(struct dp_qos_cfg_info *info, int flag)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!info || is_invalid_inst(info->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(info->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;
	return dp_info->dp_qos_platform_set(QOS_GLOBAL_CFG_GET, info, flag);
}
EXPORT_SYMBOL(dp_qos_global_info_get);

int dp_qos_port_conf_set2(struct dp_port_cfg_info *info, int flag)
{
	struct inst_info *dp_info;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!info || is_invalid_inst(info->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(info->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;

	return dp_info->dp_qos_platform_set(QOS_PORT_CFG_SET, info, flag);
}
EXPORT_SYMBOL(dp_qos_port_conf_set2);

int dp_block_flush_port(struct dp_qos_blk_flush_port *cfg, int flag)
{
	struct inst_info *dp_info;
	int ret = DP_FAILURE;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!cfg || is_invalid_inst(cfg->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(cfg->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;

	if (cfg->dp_port >= dp_info->cap.max_num_dp_ports) {
		pr_err("DPM: %s dp_port %d should be valid\n",
		       __func__, cfg->dp_port);
		return DP_FAILURE;
	}

	DP_LIB_LOCK(&dp_lock);
	ret = dp_info->dp_qos_platform_set(QOS_BLOCK_FLUSH_PORT, cfg, flag);
	DP_LIB_UNLOCK(&dp_lock);

	return ret;
}
EXPORT_SYMBOL(dp_block_flush_port);

int dp_block_flush_queue(struct dp_qos_blk_flush_queue *cfg, int flag)
{
	struct inst_info *dp_info;
	int ret = DP_FAILURE;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!cfg || is_invalid_inst(cfg->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(cfg->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;

	if (!(flag & DP_QFLUSH_FLAG_NO_LOCK))
		DP_LIB_LOCK(&dp_lock);
	ret = dp_info->dp_qos_platform_set(QOS_BLOCK_FLUSH_QUEUE, cfg, flag);
	if (!(flag & DP_QFLUSH_FLAG_NO_LOCK))
		DP_LIB_UNLOCK(&dp_lock);

	return ret;
}
EXPORT_SYMBOL(dp_block_flush_queue);

int dp_qos_get_q_mib2(struct dp_qos_queue_info *info, int flag)
{
	struct inst_info *dp_info;
	int ret = DP_FAILURE;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!(info->nodeid) || is_invalid_inst(info->inst))
		return DP_FAILURE;

	dp_info = get_dp_prop_info(info->inst);

	if (!dp_info || !dp_info->dp_get_queue_mib)
		return DP_FAILURE;

	ret = dp_info->dp_get_queue_mib(info, flag);

	return ret;
}
EXPORT_SYMBOL(dp_qos_get_q_mib2);

int dp_qos_codel_cfg_set2(struct dp_qos_codel_cfg *cfg, int flag)
{
	struct inst_info *dp_info;

	dp_info = get_dp_prop_info(cfg->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;

	return dp_info->dp_qos_platform_set(CODEL_SET, cfg, cfg->inst);
}

int dp_qos_codel_cfg_get2(struct dp_qos_codel_cfg *cfg, int flag)
{
	struct inst_info *dp_info;

	dp_info = get_dp_prop_info(cfg->inst);

	if (!dp_info || !dp_info->dp_qos_platform_set)
		return DP_FAILURE;

	return dp_info->dp_qos_platform_set(CODEL_GET, cfg, cfg->inst);
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP
