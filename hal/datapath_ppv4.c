// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2021, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <net/datapath_api.h>
#include "../datapath.h"
#include "datapath_misc.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

void dp_print_sched_conf(const struct pp_qos_sched_conf *cfg, char *s,
				   int id)
{
	if (id >= 0)
		DP_DUMP("-----> Common Sched Node properties: %s id=%d <----\n",
			s, id);
	else
		DP_DUMP("-----> Common Sched Node properties: node %s <----\n", s);
	DP_DUMP("       bw_limit        %d\n",
		cfg->common_prop.bandwidth_limit);
	DP_DUMP("       shared_bw_grp   %d\n",
		cfg->common_prop.shared_bw_group);
	DP_DUMP("       max_burst       %d\n", cfg->common_prop.max_burst);

	DP_DUMP("       -----> Parent Node properties:\n");
	DP_DUMP("              best_effort_enable %d\n",
		cfg->sched_parent_prop.best_effort_enable);

	if (cfg->sched_parent_prop.arbitration == PP_QOS_ARBITRATION_WSP)
		DP_DUMP("              arbi               %s\n", "WSP");
	else if (cfg->sched_parent_prop.arbitration == PP_QOS_ARBITRATION_WRR)
		DP_DUMP("              arbi               %s\n", "WRR");
	else
		DP_DUMP("              arbi               %s\n", "WFQ");

	DP_DUMP("       -----> Child Node properties:\n");
	DP_DUMP("              parent             %d\n",
		cfg->sched_child_prop.parent);
	DP_DUMP("              priority           %d\n",
		cfg->sched_child_prop.priority);
	DP_DUMP("              bandwidth_share    %d\n",
		cfg->sched_child_prop.wrr_weight);

	DP_DUMP("\n");
}

void dp_print_port_conf(const struct pp_qos_port_conf *cfg, char *s,
				 int id)
{
	if (id >= 0)
		DP_DUMP("-----> Common Port Node properties: %s id=%d <----\n",
			s, id);
	else
		DP_DUMP("-----> Common Port Node properties: node %s <----\n", s);

	DP_DUMP("       bw_limit        %d\n",
		cfg->common_prop.bandwidth_limit);
	DP_DUMP("       shared_bw_grp   %d\n",
		cfg->common_prop.shared_bw_group);
	DP_DUMP("       max_burst       %d\n", cfg->common_prop.max_burst);

	DP_DUMP("       -----> Parent Node properties:\n");
	DP_DUMP("              best_effort_enable    %d\n",
		cfg->port_parent_prop.best_effort_enable);

	if (cfg->port_parent_prop.arbitration == PP_QOS_ARBITRATION_WSP)
		DP_DUMP("              arbi                  %s\n", "WSP");
	else if (cfg->port_parent_prop.arbitration == PP_QOS_ARBITRATION_WRR)
		DP_DUMP("              arbi                  %s\n", "WRR");
	else
		DP_DUMP("              arbi                  %s\n", "WFQ");

	DP_DUMP("              ring_address          %lx\n", cfg->ring_address);
	DP_DUMP("              ring_size             %d\n", cfg->ring_size);
	DP_DUMP("              packet_credit_enable  %d\n",
		cfg->packet_credit_enable);
	DP_DUMP("              credit                %d\n", cfg->credit);
	DP_DUMP("              disable               %d\n", cfg->disable);
	DP_DUMP("              green_threshold       %d\n",
		cfg->green_threshold);
	DP_DUMP("              yellow_threshold      %d\n",
		cfg->yellow_threshold);
	DP_DUMP("\n");
}

void dp_print_queue_conf(const struct pp_qos_queue_conf *cfg, char *s,
				  int id)
{
	if (id >= 0)
		DP_DUMP("-----> Common Queue Node properties: %s id=%d <----\n",
			s, id);
	else
	DP_DUMP("-----> Common Queue Node properties: node %s <----\n", s);
	DP_DUMP("       bw_limit        %d\n",
		cfg->common_prop.bandwidth_limit);
	DP_DUMP("       shared_bw_grp   %d\n",
		cfg->common_prop.shared_bw_group);
	DP_DUMP("       max_burst       %d\n", cfg->common_prop.max_burst);

	DP_DUMP("       -----> Child Node properties:\n");
	DP_DUMP("              parent                      %d\n",
		cfg->queue_child_prop.parent);
	DP_DUMP("              priority                    %d\n",
		cfg->queue_child_prop.priority);
	DP_DUMP("              bandwidth_share             %d\n",
		cfg->queue_child_prop.wrr_weight);

	DP_DUMP("              blocked                     %d\n", cfg->blocked);
	DP_DUMP("              wred_enable                 %d\n",
		cfg->wred_enable);
	DP_DUMP("              wred_fixed_drop_prob_enable %d\n",
		cfg->wred_fixed_drop_prob_enable);
	DP_DUMP("              wred_min_avg_green          %d\n",
		cfg->wred_min_avg_green);
	DP_DUMP("              wred_max_avg_green          %d\n",
		cfg->wred_max_avg_green);
	DP_DUMP("              wred_slope_green            %d\n",
		cfg->wred_slope_green);
	DP_DUMP("              wred_fixed_drop_prob_green  %d\n",
		cfg->wred_fixed_drop_prob_green);
	DP_DUMP("              wred_min_avg_yellow         %d\n",
		cfg->wred_min_avg_yellow);
	DP_DUMP("              wred_slope_yellow           %d\n",
		cfg->wred_slope_yellow);
	DP_DUMP("              wred_fixed_drop_prob_yellow %d\n",
		cfg->wred_fixed_drop_prob_yellow);
	DP_DUMP("              wred_min_guaranteed         %d\n",
		cfg->wred_min_guaranteed);
	DP_DUMP("              wred_max_allowed            %d\n",
		cfg->wred_max_allowed);
	DP_DUMP("              codel_en                    %d\n",
		cfg->codel_en);
	DP_DUMP("              eir                         %d\n", cfg->eir);
	DP_DUMP("\n");
}

int dp_qos_queue_remove(struct pp_qos_dev *qdev, u32 id)
{
	DP_DEBUG(DP_DBG_FLAG_QOS, "QoS Dev 0x%px ID %d\n",  qdev, id);

	return pp_qos_queue_remove(qdev, id);
}

#ifdef DPM_QUEUE_ALLOC
int dp_qos_queue_allocate(struct pp_qos_dev *qdev, u32 *id)
{
	int ret;

	DP_DEBUG(DP_DBG_FLAG_QOS, "QoS Dev 0x%px\n", qdev);

	ret = pp_qos_queue_allocate(qdev, id);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_QOS_DETAIL)
		DP_INFO("%s QoS Dev 0x%px ID %d\n",
			ret ? "Fail" : "OK", qdev, *id);
#endif

	return ret;
}
#endif

int dp_qos_queue_info_get(struct pp_qos_dev *qdev, u32 id,
			  struct pp_qos_queue_info *info)
{
	int ret;

	DP_DEBUG(DP_DBG_FLAG_QOS, "QoS Dev 0x%px ID %d\n", qdev, id);

	ret = pp_qos_queue_info_get(qdev, id, info);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_QOS_DETAIL) {
		DP_INFO("%s QoS Dev 0x%px ID %d\n",
			ret ? "Fail" : "OK", qdev, id);
		DP_INFO("port ID         %x\n", info->port_id);
		DP_INFO("physical ID     %x\n", info->physical_id);
	}
#endif
	return ret;
}

int dp_qos_port_remove(struct pp_qos_dev *qdev, u32 id, int inst)
{
	struct hal_priv *priv = HAL(inst);

	priv->qos_sch_stat[id].used = 0;
	priv->qos_sch_stat[id].node.id.cqm_deq_port = -1;

	DP_DEBUG(DP_DBG_FLAG_QOS, "QoS Dev 0x%px ID %d\n", qdev, id);

	return pp_qos_port_remove(qdev, id);
}

int dp_qos_sched_allocate(struct pp_qos_dev *qdev, u32 *id)
{
	int ret;

	DP_DEBUG(DP_DBG_FLAG_QOS, "QoS Dev 0x%px\n", qdev);

	ret = pp_qos_sched_allocate(qdev, id);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_QOS_DETAIL)
		DP_INFO("%s QoS Dev 0x%px ID %d\n",
			ret ? "Fail" : "OK", qdev, *id);
#endif
	if (!ret) {
		if (*id >= DP_MAX_NODES) {
			pr_err("DPM: sched_id(%d) should less than %d\n", *id,
			       DP_MAX_NODES);
			return DP_FAILURE;
		}
	}
	return ret;
}

int dp_qos_sched_remove(struct pp_qos_dev *qdev, u32 id)
{
	DP_DEBUG(DP_DBG_FLAG_QOS, "QoS Dev 0x%px ID %u\n", qdev, id);

	return pp_qos_sched_remove(qdev, id);
}

int dp_qos_port_allocate(struct pp_qos_dev *qdev,
			 u32 physical_id, u32 *id, int inst)
{
	int ret;
	struct hal_priv *priv = HAL(inst);

	DP_DEBUG(DP_DBG_FLAG_QOS, "QoS Dev 0x%px\n", qdev);

	ret =  pp_qos_port_allocate(qdev, physical_id, id);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_QOS_DETAIL)
		DP_DUMP("DPM: %s: %s QoS Dev 0x%px ID %d\n", __func__,
			ret ? "Fail" : "OK", qdev, *id);
#endif
	if (!ret) {
		if (*id >= DP_MAX_NODES) {
			pr_err("DPM: port_id(%d) should less than %d\n", *id,
			       DP_MAX_NODES);
			return DP_FAILURE;
		}
		priv->qos_sch_stat[*id].used = 1;
		priv->qos_sch_stat[*id].node.type = DP_NODE_PORT;
		priv->qos_sch_stat[*id].node.id.cqm_deq_port = physical_id;
	}
	return ret;
}

int dp_qos_port_set(struct pp_qos_dev *qdev, u32 id,
		    const struct pp_qos_port_conf *conf)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_QOS_DETAIL) {
		DP_INFO("QoS Dev 0x%px ID %d\n", qdev, id);
		dp_print_port_conf(conf, "set", id);
	}
#endif

	return pp_qos_port_set(qdev, id, conf);
}

void dp_qos_port_conf_set_default(struct pp_qos_port_conf *conf)
{
	pp_qos_port_conf_set_default(conf);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_QOS_DETAIL) {
		dp_print_port_conf(conf, "set_default", -1);
	}
#endif
}

void dp_qos_queue_conf_set_default(struct pp_qos_queue_conf *conf)
{
	pp_qos_queue_conf_set_default(conf);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_QOS_DETAIL) {
		dp_print_queue_conf(conf, "set_default", -1);
	}
#endif
}

int dp_qos_queue_set(struct pp_qos_dev *qdev, u32 id,
		     const struct pp_qos_queue_conf *conf)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_QOS_DETAIL) {
		DP_INFO("Attach q_node=%d to parent_node=%d\n",
			id, conf->queue_child_prop.parent);
		DP_DUMP("QoS Dev 0x%px ID %d\n", qdev, id);
		dp_print_queue_conf(conf, "set", id);
	}
#endif
	return pp_qos_queue_set(qdev, id, conf);
}

void dp_qos_sched_conf_set_default(struct pp_qos_sched_conf *conf)
{
	pp_qos_sched_conf_set_default(conf);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_QOS_DETAIL) {
		dp_print_sched_conf(conf, "set_default", -1);
	}
#endif
}

int dp_qos_sched_set(struct pp_qos_dev *qdev, u32 id,
		     const struct pp_qos_sched_conf *conf)
{
#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_QOS_DETAIL) {
		DP_INFO("QoS Dev 0x%px ID %d\n", qdev, id);
		dp_print_sched_conf(conf, "sched_set", id);
	}
#endif
	return pp_qos_sched_set(qdev, id, conf);
}

int dp_qos_queue_conf_get(struct pp_qos_dev *qdev, u32 id,
			  struct pp_qos_queue_conf *conf)
{
	int ret;

	ret = pp_qos_queue_conf_get(qdev, id, conf);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_QOS_DETAIL) {
		DP_INFO("QoS Dev 0x%px ID %d\n", qdev, id);
		dp_print_queue_conf(conf, "conf_get", id);
	}
#endif
	return ret;
}

int dp_qos_sched_conf_get(struct pp_qos_dev *qdev, u32 id,
			  struct pp_qos_sched_conf *conf)
{
	int ret;

	DP_DEBUG(DP_DBG_FLAG_QOS, "QoS Dev 0x%px ID %d\n", qdev, id);

	ret = pp_qos_sched_conf_get(qdev, id, conf);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_QOS_DETAIL) {
		dp_print_sched_conf(conf, "conf_get", id);
	}
#endif
	return ret;
}

int dp_qos_sched_get_queues(struct pp_qos_dev *qdev, u32 id,
			    u16 *queue_ids, u32 size,
			    u32 *queues_num)
{
	int ret;

	DP_DEBUG(DP_DBG_FLAG_QOS, "QoS Dev 0x%px ID %d\n", qdev, id);

	ret = pp_qos_sched_get_queues(qdev, id, queue_ids, size,
				      queues_num);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_QOS_DETAIL)
		DP_INFO("ret = %d queue_ids %d size %d queue_num %d\n",
			ret, *queue_ids, size, *queues_num);
#endif
	return ret;
}

int dp_qos_port_get_queues(struct pp_qos_dev *qdev, u32 id,
			   u16 *queue_ids, u32 size,
			   u32 *queues_num)
{
	int ret;

	DP_DEBUG(DP_DBG_FLAG_QOS, "QoS Dev 0x%px ID %d\n", qdev, id);

	ret = pp_qos_port_get_queues(qdev, id, queue_ids, size, queues_num);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_QOS_DETAIL)
		DP_INFO("ret = %d queue_ids %d size %d queue_num %d\n",
			ret, *queue_ids, size, *queues_num);
#endif
	return ret;
}

int dp_qos_port_conf_get(struct pp_qos_dev *qdev, u32 id,
			 struct pp_qos_port_conf *conf)
{
	int ret;

	DP_DEBUG(DP_DBG_FLAG_QOS, "QoS Dev 0x%px ID %d\n", qdev, id);

	ret = pp_qos_port_conf_get(qdev, id, conf);

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DBG)
	if (dp_dbg_flag & DP_DBG_FLAG_QOS_DETAIL)
		dp_print_port_conf(conf, "conf_get", id);
#endif
	return ret;
}

int dp_qos_queue_allocate_id_phy(struct pp_qos_dev *qdev, u32 *id,
				 u32 *phy)
{
	int ret;

	ret = pp_qos_queue_allocate_id_phy(qdev, id, phy);
	DP_DEBUG(DP_DBG_FLAG_QOS, "QoS Dev 0x%px ID %d\n", qdev, *id);
	if (!ret) {
		if (*id >= DP_MAX_NODES) {
			pr_err("DPM: id(%d) should less than %d\n", *id, DP_MAX_NODES);
			return DP_FAILURE;
		}
		if (*phy >= DP_MAX_QUEUE_NUM) {
			pr_err("DPM: phy(%d) should less than %d\n", *phy,
				DP_MAX_QUEUE_NUM);
			return DP_FAILURE;
		}
	}

	return ret;
}

struct pp_qos_dev *dp_qos_dev_open(u32 id)
{
	DP_DEBUG(DP_DBG_FLAG_QOS, "ID %d\n", id);

	return pp_qos_dev_open(id);
}

int dp_pp_alloc_port(struct ppv4_port *info)
{
	struct hal_priv *priv = HAL(info->inst);
	struct pp_qos_dev *qdev = priv->qdev;
	struct pp_qos_port_conf cfg;
	int qos_pid;

	if (dp_qos_port_allocate(qdev, info->cqm_deq_port, &qos_pid, info->inst)) {
		pr_err("DPM: %s dp_qos_port_allocate P:=%d fail\n",
		       __func__, info->cqm_deq_port);
		return DP_FAILURE;
	}
	if (qos_pid >= ARRAY_SIZE(priv->qos_sch_stat)) {
		pr_err("DPM: qos_pid(%d) should less than %zu\n", qos_pid,
		       ARRAY_SIZE(priv->qos_sch_stat));
		return DP_FAILURE;
	}

	dp_qos_port_conf_set_default(&cfg);
	cfg.packet_credit_enable = 1;
	cfg.credit = info->tx_pkt_credit;
	cfg.ring_size = info->tx_ring_size;
	cfg.ring_address = (unsigned long)info->tx_ring_addr_push;
	cfg.port_parent_prop.arbitration = PP_QOS_ARBITRATION_WSP;
	if (dp_qos_port_set(qdev, qos_pid, &cfg)) {
		pr_err("DPM: %s dp_qos_port_set P[%d/%d] fail\n",
		       __func__, info->cqm_deq_port, qos_pid);
		dp_qos_port_remove(qdev, qos_pid, info->inst);
		return DP_FAILURE;
	}

	info->node_id = qos_pid;
	priv->deq_port_stat[info->cqm_deq_port].node_id = qos_pid;
	priv->deq_port_stat[info->cqm_deq_port].flag = PP_NODE_ALLOC;

	return DP_SUCCESS;
}

int dp_pp_alloc_queue(struct ppv4_queue *info)
{
	struct hal_priv *priv = HAL(info->inst);
	struct pp_qos_dev *qdev = priv->qdev;
	u32 q_node_id, q_phy_id;

	if (dp_qos_queue_allocate_id_phy(qdev, &q_node_id, &q_phy_id)) {
		pr_err("DPM: %s dp_qos_queue_allocate_id_phy fail\n", __func__);
		return DP_FAILURE;
	}
	if (q_node_id >= ARRAY_SIZE(priv->qos_sch_stat)) {
		pr_err("DPM: q_node_id(%d) should less than %zu\n", q_node_id,
			ARRAY_SIZE(priv->qos_sch_stat));
		return DP_FAILURE;
	}
	if (q_phy_id >= ARRAY_SIZE(priv->qos_queue_stat)) {
		pr_err("DPM: q_phy_id(%d) should less than %zu\n", q_phy_id,
			ARRAY_SIZE(priv->qos_queue_stat));
		return DP_FAILURE;
	}

	info->qid = q_phy_id;
	info->node_id = q_node_id;

	DP_DEBUG(DP_DBG_FLAG_QOS, "Attached Q[%d/%d] to parent_node=%d\n",
		 q_phy_id, q_node_id, info->parent);

	return DP_SUCCESS;
}

int de_init_ppv4(struct hal_priv *priv)
{
	int i, j;
	struct pmac_port_info *p_info;
	struct dp_subif_info *subif_info;
	struct dp_node_alloc node;
	u32 node_id;

	/* free CPU Port */
	for (i = 0; i < DP_MAX_INST; i++) {
		p_info = get_dp_port_info(i, 0);
		if (!p_info || !p_info->status)
			continue;
		for (j = 0; j < DP_MAX_CPU * 2; j++) {
			subif_info = get_dp_port_subif(p_info, j);
			if (!subif_info->flags)
				continue;
			node.dp_port = CPU_PORT;
			node.inst = i;

			node.id.q_id = subif_info->qid;
			node.type = DP_NODE_QUEUE;
			dp_node_free(&node, 0);

			node.id.cqm_deq_port = subif_info->cqm_deq_port[0];
			node.type = DP_NODE_PORT;
			dp_node_free(&node, 0);
		}
		/* free reinsert_deq_port and queue */
		if (reinsert_deq_port[priv->inst]) {
			struct cqm_port_info *cqm_info;

			node.dp_port = CPU_PORT;
			node.inst = i;
			cqm_info = get_dp_deqport_info(i,
				   reinsert_deq_port[priv->inst]);
			if (is_qid_valid(cqm_info->qid[0])) {
				node.id.q_id = cqm_info->qid[0];
				node.type = DP_NODE_QUEUE;
				dp_node_free(&node, 0);
			}
			node.id.cqm_deq_port = reinsert_deq_port[priv->inst];
			node.type = DP_NODE_PORT;
			dp_node_free(&node, 0);
		}
		/* Note: Don't use dp_node_free to free drop queue/port since
		 *       they are not created via dp_node_xxx APIs.
		 */
		node_id = pp_qos_queue_id_get(priv->qdev, priv->ppv4_drop_q);
		dp_qos_queue_remove(priv->qdev, node_id);

		dp_qos_port_remove(priv->qdev, priv->ppv4_drop_p, i);
		priv->qdev = NULL;
	}
	return 0;
}

int init_ppv4_qos(int inst, int flag)
{
	struct local {
		struct pp_qos_port_conf p_cfg;
		struct pp_qos_queue_conf q_cfg;
		struct cbm_cpu_port_data cpu_data;
	};
	struct cbm_tx_push *drop_port_info;
	struct hal_priv *priv = HAL(inst);
	struct cqm_port_info *cqm_port;
	u32 id, qid, drop_port;
	struct local *l;
	int ret = DP_FAILURE;

	if (!priv)
		return DP_FAILURE;
	if (flag & DP_PLATFORM_DE_INIT) {
		de_init_ppv4(priv);
		return DP_SUCCESS;
	}

	priv->qdev = dp_qos_dev_open(dp_port_prop[inst].qos_inst);
	if (!priv->qdev) {
		pr_err("DPM: %s dp qos_dev_open %d fail\n",
		       __func__, dp_port_prop[inst].qos_inst);
		return DP_FAILURE;
	}
	l = dp_kzalloc(sizeof(*l), GFP_ATOMIC);
	if (!l) {
		return DP_FAILURE;
	}
	if (CBM_OPS(inst, cbm_cpu_port_get, &l->cpu_data, 0)) {
		pr_err("DPM: %s cbm_cpu_port_get fail\n", __func__);
		goto exit;
	}
	dump_cpu_data(inst, &l->cpu_data);

	drop_port_info = &l->cpu_data.dq_tx_flush_info;
	drop_port = drop_port_info->deq_port;
	if (!drop_port || drop_port >= ARRAY_SIZE(dp_deq_port_tbl[inst])) {
		pr_err("DPM: %s invalid drop port:%d\n", __func__, drop_port);
		goto exit;
	}
	priv->cqm_drop_p = drop_port;
	cqm_port = get_dp_deqport_info(inst, drop_port);
	cqm_port->dp_port[0] = 1;
	cqm_port->tx_ring_size = drop_port_info->tx_ring_size;
	cqm_port->tx_pkt_credit = drop_port_info->tx_pkt_credit;
	cqm_port->txpush_addr = (void *)drop_port_info->txpush_addr;
	cqm_port->txpush_addr_qos = (void *)drop_port_info->txpush_addr_qos;

	DP_DEBUG(DP_DBG_FLAG_QOS,
		 "%s %d: ring addr/push=0x%px/0x%px size=%d pkt_credit=%d\n",
		 "DP drop port", priv->cqm_drop_p, cqm_port->txpush_addr_qos,
		 cqm_port->txpush_addr, cqm_port->tx_ring_size,
		 cqm_port->tx_pkt_credit);

	DP_DEBUG(DP_DBG_FLAG_DBG, "priv=0x%px deq_port_stat=0x%px q_dev=0x%px\n",
		 priv, priv->deq_port_stat, priv->qdev);

	if (dp_qos_port_allocate(priv->qdev, priv->cqm_drop_p,
				 &priv->ppv4_drop_p, inst)) {
		pr_err("DPM: %s dp_qos_port_allocate %d fail\n",
		       __func__, priv->cqm_drop_p);
		goto exit;
	}
	if (priv->ppv4_drop_p >= ARRAY_SIZE(priv->qos_sch_stat)) {
		pr_err("DPM: ppv4_drop_p(%d) should less than %zu\n",
		       priv->ppv4_drop_p,
		       ARRAY_SIZE(priv->qos_sch_stat));
		goto exit;
	}

	dp_qos_port_conf_set_default(&l->p_cfg);
	l->p_cfg.packet_credit_enable = 1;
	l->p_cfg.credit = cqm_port->tx_pkt_credit;
	l->p_cfg.ring_size = cqm_port->tx_ring_size;
	l->p_cfg.port_parent_prop.arbitration = PP_QOS_ARBITRATION_WRR;
	l->p_cfg.ring_address = (unsigned long)cqm_port->txpush_addr_qos;
	if (dp_qos_port_set(priv->qdev, priv->ppv4_drop_p, &l->p_cfg)) {
		pr_err("DPM: %s dp_qos_port_set P[%d/%d] fail\n",
		       __func__, priv->cqm_drop_p, priv->ppv4_drop_p);
		dp_qos_port_remove(priv->qdev, priv->ppv4_drop_p, inst);
		goto exit;
	}

	if (dp_qos_queue_allocate_id_phy(priv->qdev, &id, &qid)) {
		pr_err("DPM: %s dp_qos_queue_allocate_id_phy fail\n", __func__);
		dp_qos_port_remove(priv->qdev, priv->ppv4_drop_p, inst);
		goto exit;
	}

	DP_DEBUG(DP_DBG_FLAG_QOS, "ppv4_drop Q[%d/%d] alloc done\n", qid, id);

	/* Connects queue to drop port */
	dp_qos_queue_conf_set_default(&l->q_cfg);
	l->q_cfg.blocked = 1; /* drop mode */
	l->q_cfg.wred_enable = 0;
	l->q_cfg.wred_max_allowed = 0; /* max qocc in pkt */
	l->q_cfg.queue_child_prop.parent = priv->ppv4_drop_p;
	if (dp_qos_queue_set(priv->qdev, id, &l->q_cfg)) {
		pr_err("DPM: %s dp_qos_queue_set node_id=%d to parent=%d fail\n",
		       __func__, id, priv->ppv4_drop_p);
		dp_qos_port_remove(priv->qdev, priv->ppv4_drop_p, inst);
		dp_qos_queue_remove(priv->qdev, id);
		goto exit;
	}

	priv->ppv4_drop_q = qid;
	DP_DEBUG(DP_DBG_FLAG_QOS, "Drop Q[%d/%d] to Port[%d/%d]\n",
		 qid, id, priv->cqm_drop_p, priv->ppv4_drop_p);

	/* Update drop queue/port state in global table */
	if (id >= ARRAY_SIZE(priv->qos_sch_stat)) {
		pr_err("DPM: %s error: id(%d) should be less than %zu\n",
			__func__, id, ARRAY_SIZE(priv->qos_sch_stat));
		goto exit;
	}
	priv->qos_sch_stat[id].parent.type = DP_NODE_PORT;
	priv->qos_sch_stat[id].parent.flag = PP_NODE_ACTIVE;
	priv->qos_sch_stat[id].parent.node_id = priv->ppv4_drop_p;
	priv->qos_sch_stat[priv->ppv4_drop_p].child[0].node_id = id;
	priv->qos_sch_stat[priv->ppv4_drop_p].child[0].type = DP_NODE_QUEUE;
	priv->qos_sch_stat[priv->ppv4_drop_p].child[0].flag = PP_NODE_ACTIVE;
	spin_lock_init(&priv->qos_lock);
	ret = DP_SUCCESS;
exit:
	kfree(l);
	return ret;
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP
