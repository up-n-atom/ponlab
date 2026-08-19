// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2020 - 2025 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/
#include <linux/netdevice.h>
#include <linux/version.h>
#include <net/datapath_api.h>
#include <net/datapath_api_qos.h>
#include <net/qos_tc.h>
#include "qos_tc_flower.h"
#include "qos_tc_qos.h"
#include "qos_tc_qmap.h"
#include "qos_tc_trace.h"

#if IS_ENABLED(CONFIG_QOS_NOTIFY)
#include <net/qos_notify.h>
#define QOS_TC_QMASK 0xff	/* to reserve bits per scheduler */
#define QOS_TC_MAX_OFFSET (SZ_64 - SZ_8) /* max offset for 64bit qmap */
#endif

static LIST_HEAD(port_list);

static int __qos_tc_qdata_remove(struct net_device *dev,
				 struct qos_tc_q_data *qid,
				 struct qos_tc_qdata_params *p);

struct qos_tc_port *qos_tc_port_get(struct net_device *dev)
{
	struct qos_tc_port *p, *n;

	list_for_each_entry_safe(p, n, &port_list, list) {
		if (p->dev == dev)
			return p;
	}

	return NULL;
}

struct qos_tc_port *qos_tc_port_alloc(struct net_device *dev)
{
	struct qos_tc_port *port = NULL;

	port = kzalloc(sizeof(*port), GFP_KERNEL);
	if (!port)
		return NULL;

	port->dev = dev;
	INIT_RADIX_TREE(&port->qdiscs, GFP_KERNEL);

	list_add(&port->list, &port_list);

	return port;
}

int qos_tc_port_delete(struct qos_tc_port *port)
{
	if (port->destroy)
		port->destroy(port);

	list_del(&port->list);
	kfree(port);

	return 0;
}

#define PORT_GREEN_TH (32 * 1024 * 1024)
#define PORT_YELLOW_TH (16 * 1024 * 1024)
static int qos_tc_overwrite_port_thresholds(struct qos_tc_qdisc *sch)
{
	struct dp_port_cfg_info port_cfg_info = {
		.inst = sch->inst,
		.pid = sch->epn,
		.green_threshold = PORT_GREEN_TH,
		.yellow_threshold = PORT_YELLOW_TH,
		.ewsp = 1,
	};
	int flags = DP_PORT_CFG_GREEN_THRESHOLD | DP_PORT_CFG_YELLOW_THRESHOLD |
		    DP_PORT_CFG_EWSP;

	if (dp_qos_port_conf_set(&port_cfg_info, flags) == DP_FAILURE) {
		netdev_err(sch->dev, "%s: dp_qos_port_conf_set() failed\n",
			   __func__);
		return -EINVAL;
	}

	return 0;
}

static bool qos_tc_is_netdev_reinsert_port(struct net_device *dev)
{
	dp_subif_t *subif __free(kfree) = NULL;
	int ret;

	subif = kzalloc(sizeof(*subif), GFP_KERNEL);
	if (!subif) {
		netdev_err(dev, "%s: failed to allocate memory for subif\n",
			   __func__);
		return false;
	}
	ret = dp_get_netif_subifid(dev, NULL, NULL, 0, subif, 0);
	if (ret == DP_FAILURE) {
		/* negative return value is no error for non
		 * reinsertion port
		 */
		return false;
	}
	netdev_dbg(dev, "%s: returned %d\n",
			   __func__, subif->data_flag & DP_SUBIF_REINSERT ? 1 : 0);

	if (subif->data_flag & DP_SUBIF_REINSERT)
		ret = true;
	else
		ret = false;

	return ret;
}

int qos_tc_fill_port_data(struct qos_tc_qdisc *sch,
		const struct qos_tc_params *tc_params)
{
	dp_subif_t *subif __free(kfree) = NULL;
	struct dp_dequeue_res deq = {0};
	struct dp_queue_res q_res[QOS_TC_MAX_Q] = {0};
	int ret;
	int flags = 0;

	if (tc_params && tc_params->flags & QOS_TC_IS_LIF_CONFIG) {
		sch->def_q = tc_params->def_q;
		/* For logical interfaces, the required phy port data is received
		 * through tc_params.
		 */
		return 0;
	}

	/* The CPU port is not a subif in DP but needs special handling. We
	 * cannot directly get the DP default CPU queues from DP for the CPU
	 * port, but we just request all queues for this port and assume that
	 * the first one is the DP default queue. The CPU port is the port 0
	 * on our systems, hard code that.
	 * If we get more than one CPU queue, we do not change anything here.
	 * On LGM we get 32 queues here, probably configured by PPA and we
	 * should not touch them. On PRX we get only one and it works fine.
	 * We plan to add dedicated support for the CPU port later.
	 */
	if (qos_tc_is_cpu_port(sch->port)) {
		deq.dp_port = 0;
		deq.cqm_deq_idx = DEQ_PORT_OFFSET_ALL;
		deq.q_res = q_res;
		deq.q_res_size = ARRAY_SIZE(q_res);
		ret = dp_deq_port_res_get(&deq, 0);
		if (ret != DP_SUCCESS) {
			netdev_err(sch->dev, "can not get CPU queue: %i", ret);
			return -ENODEV;
		}
		if (deq.num_q == 1)
			sch->def_q = q_res[0].q_id;
		else
			sch->def_q = -1;
		if (deq.num_q > 1)
			netdev_dbg(sch->dev, "found %i DP default CPU queues, do not change them",
				   deq.num_q);
		sch->inst = 0;
		sch->port = 0;
		sch->deq_idx = DEQ_PORT_OFFSET_ALL;
		memset(&deq, 0, sizeof(deq));
	} else {
		subif = kzalloc(sizeof(*subif), GFP_KERNEL);
		if (!subif) {
			netdev_err(sch->dev, "%s: failed to allocate memory for subif\n",
				   __func__);
			return -ENOMEM;
		}
		ret = dp_get_netif_subifid(sch->dev, NULL, NULL, 0, subif, 0);
		if (ret == DP_SUCCESS) {
			/* The deq_idx is provided in the qos_tc_setup()
			 * function by the caller like the PON Ethernet driver
			 * or the LAN Ethernet driver. Some drivers provide -1
			 * to indicate the value is unknown.
			 */
			if (sch->deq_idx < 0 ||
			    subif->data_flag & DP_SUBIF_REINSERT) {
				/* This is PON DS port so mark this */
				sch->port = subif->port_id;
				sch->deq_idx = 0;
				sch->ds = true;
			}
			/* Take flags from subif in order to distinguish
			 * reinsertion port from others. Special bit is set
			 * for reinsertion port.
			 */
			flags = subif->data_flag;
			/* save hw egress port settings */
			sch->inst = subif->inst;
			/* alloc_flag will be set for URX only; for other SoCs, it will be zero.
			This alloc_flag is utilized for getting interface information.
			Based on the interface, we are modifying queue length and drop algorithm
			on user queues which are created on the particular default interfaces. */
#if (defined(CONFIG_X86_INTEL_LGM) || defined(CONFIG_SOC_LGM))
			sch->alloc_flag = subif->alloc_flag;
#endif
			/* If we do not have a default queue from DP use -1 */
			if (subif->subif_common.num_q == 1)
				sch->def_q = subif->subif_common.def_qlist[0];
			else
				sch->def_q = -1;
			if (subif->subif_common.num_q > 1)
				netdev_warn(sch->dev, "found %i DP default queues, do not change them",
					    subif->subif_common.num_q);
		} else {
			netdev_dbg(sch->dev, "Can not find in DP: %i", ret);
			/* Some devices like T-Conts are not registered to DP
			 * and then this function returns an error. Just ignore
			 * it in that case.
			 */
			if (sch->deq_idx < 0)
				return -ENODEV;
			sch->def_q = -1;
			sch->inst = 0;
		}
	}

	deq.dp_port = sch->port;
	deq.cqm_deq_idx = sch->deq_idx;
	ret = dp_deq_port_res_get(&deq, flags);
	if (ret < 0)
		return ret;
	sch->epn = deq.cqm_deq_port;

	return 0;
}

static int fill_port_data(struct qos_tc_qdisc *sch)
{
	const struct qos_tc_params tc_params = {0};

	return qos_tc_fill_port_data(sch, &tc_params);
}

int qos_tc_get_port_info(struct qos_tc_qdisc *sch,
		const struct qos_tc_params *tc_params)
{
	dp_subif_t *subif __free(kfree) = NULL;
	int ret = 0;

	if (tc_params->flags & QOS_TC_IS_LIF_CONFIG) {
		sch->def_q = tc_params->def_q;
		/* For logical interfaces, the required phy port data is received
		 * through tc_params.
		 */
		return 0;
	}

	if (sch->deq_idx < 0 || sch->port < 0) {
		subif = kzalloc(sizeof(*subif), GFP_KERNEL);
		if (!subif) {
			netdev_err(sch->dev, "%s: failed to allocate memory for subif\n",
				   __func__);
			return -ENOMEM;
		}
		ret = dp_get_netif_subifid(sch->dev, NULL, NULL, 0, subif, 0);
		if (ret < 0) {
			return -ENODEV;
		}
		/* This is PON DS port so mark this */
		sch->port = subif->port_id;
		sch->deq_idx = 0;
		sch->ds = true;
	}

	return 0;
}

int qos_tc_alloc_qdisc(struct qos_tc_qdisc **qdisc)
{
	struct qos_tc_qdisc *qp;

	qp = kzalloc(sizeof(**qdisc), GFP_KERNEL);
	if (!qp)
		return -ENOMEM;

	*qdisc = qp;

	return 0;
}

void qos_tc_free_qdisc(struct qos_tc_qdisc *qdisc)
{
	kfree(qdisc);
}

struct qos_tc_qdisc *qos_tc_qdisc_find(struct qos_tc_port *port, u32 handle)
{
	return radix_tree_lookup(&port->qdiscs, TC_H_MAJ(handle));
}

int qos_tc_add_qdisc_to_dev(struct net_device *dev,
			    struct qos_tc_qdisc *qdisc, u32 handle)
{
	struct qos_tc_port *port = NULL;
	int ret;

	port = qos_tc_port_get(dev);
	if (!port)
		return -ENODEV;

	netdev_dbg(port->dev, "%s: add hdl:%#x to tree\n", __func__, handle);
	ret = radix_tree_insert(&port->qdiscs, handle, qdisc);
	if (ret < 0) {
		netdev_err(dev, "%s: qdisc radix tree insert failed: %d\n",
			   __func__, ret);
		return ret;
	}

	return 0;
}

int qos_tc_get_sch_by_handle(struct net_device *dev,
			     u32 handle,
			     struct qos_tc_qdisc **sch)
{
	struct qos_tc_port *port = NULL;

	port = qos_tc_port_get(dev);
	if (!port)
		return -ENODEV;

	*sch = qos_tc_qdisc_find(port, handle);
	if (!*sch)
		return -ENODEV;

	return 0;
}

/**
 * This function gets the the queue number from grandparent. For example if
 * we have such hierarchy:
 *
 * tc class add dev tcont32768 parent 500c: handle 500c:3 drr quantum 8k
 * tc qdisc add dev tcont32768 parent 500c:3 handle 1008: red limit 13650 \
 *	min 6820 max 13650 avpkt 1k burst 9 probability 0.004
 * tc qdisc add dev tcont32768 parent 1008:1 handle 2008: red limit 6820 \
 *	min 3410 max 6820 avpkt 1k burst 4 probability 0.004
 * tc filter add dev tcont32768 ingress handle 0x5 protocol all flower \
 *	skip_sw classid 2008:1 indev gem5 action pass
 *
 * Then, we should take the queue from red grandparent (so for 2008:1 we should
 * find red with handle 2008:, then take its parent 1008:
 * and then see that its parent is 500c:3 and the queue should be :3)
 */
static int qos_tc_get_grandparent_queue_by_handle(struct net_device *dev,
						  u32 handle,
						  struct qos_tc_q_data **qid)
{
	struct qos_tc_qdisc *qdisc_parent = NULL;
	struct qos_tc_qdata_params *p;
	struct qos_tc_q_data *qid_iter;
	struct qos_tc_qdisc *qdisc;
	int ret;
	int i;

	ret = qos_tc_get_sch_by_handle(dev, handle, &qdisc);
	if (ret < 0)
		return ret;

	for (i = 0; i < QOS_TC_MAX_Q; i++) {
		qid_iter = &qdisc->qids[i];

		if (!(qid_iter->qid))
			continue;
		list_for_each_entry(p, &qid_iter->params, list) {
			if (TC_H_MAJ(p->handle) != TC_H_MAJ(handle))
				continue;

			ret = qos_tc_get_sch_by_handle(dev, p->parent,
						       &qdisc_parent);
			if (ret < 0)
				return ret;
			*qid = qos_tc_qdata_qid_get(dev, qdisc_parent,
						    p->parent);
			if (*qid)
				return 0;
		}
	}
	return -ENOENT;
}

int qos_tc_check_qid(struct qos_tc_qdisc *qdisc, int idx)
{
	if (idx < 0 || idx >= QOS_TC_MAX_Q)
		return -EINVAL;

	return 0;
}

int qos_tc_get_queue_by_handle(struct net_device *dev,
			       u32 handle,
			       struct qos_tc_q_data **qid)
{
	struct qos_tc_qdisc *qdisc = NULL;
	int idx = TC_H_MIN(handle) - 1;
	int ret;

	ret = qos_tc_get_sch_by_handle(dev, handle, &qdisc);
	if (ret < 0)
		return ret;

	if (idx < 0 || idx >= QOS_TC_MAX_Q)
		return -EINVAL;

	if (!qdisc->qids[idx].qid)
		return -EINVAL;

	*qid = &qdisc->qids[idx];

	return 0;
}

static bool qos_tc_is_dev_type(struct net_device *dev, u32 flag)
{
	dp_subif_t *subif __free(kfree) = NULL;
	int ret;

	if (!dev)
		return false;

	subif = kzalloc(sizeof(*subif), GFP_KERNEL);
	if (!subif) {
		netdev_err(dev, "%s: failed to allocate memory for subif\n",
			   __func__);
		return false;
	}
	ret = dp_get_netif_subifid(dev, NULL, NULL, NULL, subif, 0);
	if (ret < 0) {
		netdev_dbg(dev, "%s: subif idx get failed\n", __func__);
		ret = false;
	} else {
		if (subif->alloc_flag & flag)
			ret = true;
		else
			ret = false;
	}

	return ret;
}

inline bool qos_tc_is_lan_dev(struct net_device *dev)
{
	return qos_tc_is_dev_type(dev, DP_F_FAST_ETH_LAN);
}

inline bool qos_tc_is_vuni_dev(struct net_device *dev)
{
	return qos_tc_is_dev_type(dev, DP_F_VUNI);
}

inline bool qos_tc_is_iphost_dev(struct net_device *dev)
{
	return qos_tc_is_dev_type(dev, DP_F_DIRECT);
}

inline bool qos_tc_is_gpon_dev(struct net_device *dev)
{
	return qos_tc_is_dev_type(dev, DP_F_GPON);
}

bool qos_tc_is_first_subif(struct net_device *dev)
{
	dp_subif_t *subif __free(kfree) = NULL;
	int ret;

	subif = kzalloc(sizeof(*subif), GFP_KERNEL);
	if (!subif) {
		netdev_err(dev, "%s: failed to allocate memory for subif\n",
			   __func__);
		return false;
	}
	ret = dp_get_netif_subifid(dev, NULL, NULL, NULL, subif, 0);
	if (ret != DP_SUCCESS) {
		netdev_err(dev, "can not get subif\n");
		ret = false;
	} else {
		if (!subif->subif_groupid)
			ret = true;
		else
			ret = false;
	}

	return ret;
}

static int qos_tc_qdata_child_remove(struct net_device *dev,
				     struct qos_tc_q_data *qid,
				     u32 handle);

static int hw_queue_del(struct qos_tc_qdisc *sch, int idx)
{
	struct dp_node_link node = {0};
	struct dp_node_alloc anode = {0};
	int ret;

	if (!sch)
		return -EINVAL;

	if (idx < 0 || idx > QOS_TC_MAX_Q - 1)
		return -EINVAL;

	trace_qos_tc_queue_del_enter(sch, &sch->qids[idx], idx);

	ret = qos_tc_qdata_child_remove(sch->dev, &sch->qids[idx], sch->handle);
	if (ret)
		return ret;

	if (!sch->qids[idx].qid)
		return -EINVAL;

	node.node_type = DP_NODE_QUEUE;
	node.node_id.q_id = sch->qids[idx].qid;
	ret = dp_node_unlink(&node, 0);
	if (ret == DP_FAILURE) {
		netdev_err(sch->dev, "qid %d unlink failed\n",
			   node.node_id.q_id);
		return -ENODEV;
	}

	anode.type = DP_NODE_QUEUE;
	anode.id.q_id = sch->qids[idx].qid;

	ret = dp_node_free(&anode, DP_NODE_AUTO_FREE_RES);
	if (ret == DP_FAILURE)
		netdev_err(sch->dev, "qid %d free failed\n",
			   anode.id.q_id);

	trace_qos_tc_queue_del_exit(sch, &sch->qids[idx], idx);

	memset(&sch->qids[idx], 0, sizeof(struct qos_tc_q_data));
	sch->num_q--;

	netdev_dbg(sch->dev, "qid: %i deleted\n", anode.id.q_id);
	return 0;
}

#if IS_ENABLED(CONFIG_QOS_NOTIFY)
/*!
 * Fill up the notifier structure with required data and return structure
 */
static void fill_qos_notify_data(struct qos_notifier_data *data,
			struct qos_tc_qdisc *sch, int parent, int event, int idx, int prio_w)
{
	data->netif = sch->dev;
	data->sch_id = sch->sch_id;
	data->sch_parent_id = (event == QOS_EVENT_SCH_ADD) ? parent : QOS_TC_UNUSED;
	data->qid = ((event == QOS_EVENT_Q_ADD || event == QOS_EVENT_Q_DELETE) &&
				(idx >= 0)) ? sch->qids[idx].qid : QOS_TC_UNUSED;
	data->idx = (event == QOS_EVENT_SCH_DELETE) ? QOS_TC_UNUSED : (prio_w + 1);
}

/*!
 * validate the data and send notification
 */
static void qos_tc_check_and_notify(struct qos_tc_qdisc *sch, int event,
				    int parent, int idx, int prio_w,
				    const struct qos_tc_params *tc_params)
{
	dp_subif_t *dp_subif __free(kfree) = NULL;
	int ret = 0;
	int dp_alloc_flag = 0;
	struct qos_notifier_data *data __free(kfree) = NULL;

	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (!data) {
		netdev_dbg(sch->dev, "failed to allocate data\n");
		return;
	}

	dp_subif = kzalloc(sizeof(*dp_subif), GFP_KERNEL);
	if (!dp_subif) {
		netdev_err(sch->dev, "%s: failed to allocate memory for subif\n",
			   __func__);
		return;
	}

	/* TODO: This call may not be supported for DSL interface.
	 * It has to be handled in case if it is required. */
	ret = dp_get_netif_subifid(sch->dev, NULL, NULL, NULL, dp_subif, 0);
	if (ret < 0) {
		if (tc_params && tc_params->flags & QOS_TC_IS_LIF_CONFIG)
			dp_alloc_flag = tc_params->dp_alloc_flag;
		else {
			/* NOTE: Do not treat this as an errors as T-CONTs are not
			 * registered in DP. Actions have anyway be performed even if
			 * no notification can or has to be sent.
			 */
			netdev_dbg(sch->dev, "%s: subif idx get failed\n", __func__);
			return;
		}
	} else {
		dp_alloc_flag = dp_subif->alloc_flag;
	}

	/* NOTE: Any interfaces which are not required these notifications
	 * can be included in the below validation. */
	if (dp_alloc_flag & DP_F_GPON) {
		return;
	}

	fill_qos_notify_data(data, sch, parent, event, idx, prio_w);
	/* Send notification */
	qos_qmap_notify(data, event);
}
#else
static void qos_tc_check_and_notify(struct qos_tc_qdisc *sch, int event,
				    int parent, int idx, int prio_w,
				    const struct qos_tc_params *tc_params)
{
	return;
}
#endif /* CONFIG_QOS_NOTIFY */

static int qos_tc_link_sched(struct qos_tc_qdisc *sch, int prio,
		const struct qos_tc_params *tc_params)
{
	struct dp_node_link node = {0};

	if (tc_params && tc_params->flags & QOS_TC_LINK_SCH) {
		node.p_node_id.sch_id = tc_params->sch_id;
		node.p_node_type = DP_NODE_SCH;
		node.arbi = tc_params->arbi;
		node.cqm_deq_port.cqm_deq_port = tc_params->cqm_deq_port;
		node.prio_wfq = tc_params->prio_wfq;
	} else {
		node.p_node_type = DP_NODE_PORT;
		node.arbi = (sch->type == QOS_TC_QDISC_DRR) ?
				ARBITRATION_WRR : ARBITRATION_WSP;
		node.prio_wfq = prio;
		node.cqm_deq_port.cqm_deq_port = sch->epn;
		node.p_node_id.cqm_deq_port = sch->epn;
	}
	node.inst = sch->inst;
	node.dp_port = sch->port;
	node.node_type = DP_NODE_SCH;
	node.node_id.sch_id = sch->sch_id;
	if (dp_node_link_add(&node, 0) == DP_FAILURE) {
		netdev_err(sch->dev, "failed to link sched %d to port\n", sch->sch_id);
		return -ENODEV;
	}

	netdev_dbg(sch->dev, "%s: sch: %u - port: %u\n",
		   __func__, sch->sch_id, sch->port);
	return 0;
}

int qos_tc_add_sched(struct qos_tc_qdisc *sch, int prio,
		const struct qos_tc_params *tc_params)
{
	struct dp_node_alloc anode = {0};
	struct dp_node_prio prio_info = {0};
#if IS_ENABLED(CONFIG_QOS_NOTIFY)
	struct qos_tc_port *port = NULL;
#endif
	int ret;

	if (sch->parent != TC_H_ROOT)
		return -EINVAL;

	trace_qos_tc_add_sched_exit(sch, 0);

	/* Configure the lowest priority for the default queue which is
	 * automatically added by DP manager. Some devices like a T-Cont device
	 * do not have a default queue do nothing for those devices. The def_q
	 * is set to -1 on device without a default queue.
	 */
	if (sch->def_q >= 0) {
		/* change def queue prio */
		prio_info.inst = sch->inst;
		prio_info.id.q_id = sch->def_q;
		prio_info.type = DP_NODE_QUEUE;
		ret = dp_qos_link_prio_get(&prio_info, 0);
		if (ret == DP_FAILURE) {
			netdev_err(sch->dev, " failed to get prio node\n");
			return -EINVAL;
		}

		/* Re-set input and policy */
		prio_info.arbi = ARBITRATION_WSP;
		prio_info.prio_wfq = 7;
		ret = dp_qos_link_prio_set(&prio_info, 0);
		if (ret == DP_FAILURE) {
			netdev_err(sch->dev, "set input of link failed\n");
			return -EINVAL;
		}
	}

	/* Allocate sched */
	anode.inst = sch->inst;
	anode.dp_port = sch->port;
	anode.type = DP_NODE_SCH;
	anode.id.sch_id = DP_NODE_AUTO_ID;
	ret = dp_node_alloc(&anode, 0);
	if (ret == DP_FAILURE) {
		netdev_err(sch->dev, "sch_id alloc fialed\n");
		return -ENOMEM;
	}
	sch->sch_id = anode.id.sch_id;
	netdev_dbg(sch->dev, "%s: adding sched id %u\n", __func__, sch->sch_id);

	/* Link SP/WRR sched to port/sched */
	ret = qos_tc_link_sched(sch, prio, tc_params);
	if (ret < 0) {
		dp_node_free(&anode, DP_NODE_AUTO_FREE_RES);
		return ret;
	}

	if (sch->deq_idx >= 0) {
		if (qos_tc_overwrite_port_thresholds(sch)) {
			netdev_err(sch->dev, "%s: qos_tc_overwrite_port_thresholds() failed\n",
				   __func__);
			return -EINVAL;
		}
	}

#if IS_ENABLED(CONFIG_QOS_NOTIFY)
	port = qos_tc_port_get(sch->dev);
	if (!port) {
		netdev_err(sch->dev, "failed to get port\n");
		return -ENODEV;
	}

	port->q_map = QOS_TC_QMASK; /* reserve 8 bits for root scheduler*/
	sch->offset = 0;
	/*! Send scheduler added notification */
	qos_tc_check_and_notify(sch, QOS_EVENT_SCH_ADD, QOS_TC_UNUSED, prio,
		sch->qids[prio].p_w, tc_params);
#endif

	trace_qos_tc_add_sched_exit(sch, 0);

	return 0;
}

static int add_sched(struct qos_tc_qdisc *sch, int prio)
{
	const struct qos_tc_params tc_params = {0};

	return qos_tc_add_sched(sch, prio, &tc_params);
}

int qos_tc_add_staged_sched(struct qos_tc_qdisc *psch,
			    struct qos_tc_qdisc *csch, int prio,
			    const struct qos_tc_params *tc_params)
{
	struct dp_node_link node = {0};
	struct dp_node_alloc anode = {0};
#if IS_ENABLED(CONFIG_QOS_NOTIFY)
	struct qos_tc_port *port = NULL;
#endif
	int ret;

	trace_qos_tc_add_sched_enter(csch, psch->sch_id);

	/* Allocate sched */
	anode.inst = csch->inst;
	anode.dp_port = csch->port;
	anode.type = DP_NODE_SCH;
	anode.id.sch_id = DP_NODE_AUTO_ID;
	ret = dp_node_alloc(&anode, 0);
	if (ret == DP_FAILURE) {
		netdev_err(csch->dev, "sch_id alloc fialed\n");
		return -ENOMEM;
	}
	csch->sch_id = anode.id.sch_id;

	/* Link SP/WRR sched to port/sched */
	/*node.inst = csch->inst;*/
	node.dp_port = csch->port;
	node.p_node_type = DP_NODE_SCH;
	node.p_node_id.sch_id = psch->sch_id;
	node.arbi = (psch->type == QOS_TC_QDISC_DRR) ?
		    ARBITRATION_WRR : ARBITRATION_WSP;
	node.prio_wfq = prio;
	node.node_type = DP_NODE_SCH;
	node.node_id.sch_id = csch->sch_id;
	node.cqm_deq_port.cqm_deq_port = csch->epn;
	netdev_dbg(csch->dev, "%s: adding sched id %u type: %u\n",
		   __func__, node.node_id.sch_id, node.arbi);
	if (dp_node_link_add(&node, 0) == DP_FAILURE) {
		netdev_err(csch->dev,
			   "failed to link sched %d to sched %d in: %d\n",
			   csch->sch_id, psch->sch_id, prio);
		dp_node_free(&anode, DP_NODE_AUTO_FREE_RES);
		return -ENODEV;
	}

#if IS_ENABLED(CONFIG_QOS_NOTIFY)
	port = qos_tc_port_get(csch->dev);
	if (!port) {
		netdev_err(csch->dev, "failed to get port\n");
		return -ENODEV;
	}

	/*!
	 * if valid offset found, reserve 8 bits for the scheduler
	 */
	csch->offset = find_first_zero_bit((const unsigned long *)&port->q_map, BITS_PER_LONG);
	if (csch->offset <= QOS_TC_MAX_OFFSET) {
		port->q_map |= (QOS_TC_QMASK << csch->offset);
		/* Send scheduler added notification */
		qos_tc_check_and_notify(csch, QOS_EVENT_SCH_ADD,
					csch->sch_id, prio, prio, tc_params);
	} else{
		netdev_dbg(csch->dev, "q_map is full\n");
	}
#endif

	trace_qos_tc_add_sched_exit(csch, psch->sch_id);

	return 0;
}

static int get_p_node_type(struct qos_tc_qdisc *sch, enum dp_node_type child)
{
	struct qos_tc_port *port = NULL;

	port = qos_tc_port_get(sch->dev);
	netdev_dbg(sch->dev, "%s: port tbf: %s parent: %#x child type: %u\n",
		   __func__, port->tbf_on ? "on" : "off", sch->parent, child);

	if (port && port->tbf_on && sch->parent == TC_H_ROOT)
		return DP_NODE_PORT;

	return DP_NODE_SCH;
}

static int add_child_sched(struct qos_tc_qdisc *psch,
			   struct qos_tc_qdisc *csch, int prio)
{
	struct dp_node_link node = {0};
	struct dp_node_alloc anode = {0};
	int ret;

	trace_qos_tc_add_sched_enter(csch, psch->sch_id);

	/* Allocate sched */
	anode.inst = csch->inst;
	anode.dp_port = csch->port;
	anode.type = DP_NODE_SCH;
	anode.id.sch_id = DP_NODE_AUTO_ID;
	ret = dp_node_alloc(&anode, 0);
	if (ret == DP_FAILURE) {
		netdev_err(csch->dev, "sch_id alloc fialed\n");
		return -ENOMEM;
	}
	csch->sch_id = anode.id.sch_id;

	/* Link SP/WRR sched to port/sched */
	node.dp_port = csch->port;
	node.p_node_type = get_p_node_type(psch, DP_NODE_SCH);
	node.p_node_id.sch_id = psch->sch_id;
	node.arbi = (psch->type == QOS_TC_QDISC_DRR) ?
		    ARBITRATION_WRR : ARBITRATION_WSP;
	node.prio_wfq = prio;
	node.node_type = DP_NODE_SCH;
	node.node_id.sch_id = csch->sch_id;
	node.cqm_deq_port.cqm_deq_port = csch->epn;
	netdev_dbg(csch->dev, "%s: link parent %d sch id %u -> sch id %u pw: %u\n",
		   __func__, node.p_node_type,
		   node.p_node_id.sch_id, node.node_id.sch_id, node.prio_wfq);
	if (dp_node_link_add(&node, 0) == DP_FAILURE) {
		netdev_err(csch->dev,
			   "failed to link sched %d to sched %d in: %d\n",
			   csch->sch_id, psch->sch_id, prio);
		dp_node_free(&anode, DP_NODE_AUTO_FREE_RES);
		return -ENODEV;
	}

	trace_qos_tc_add_sched_exit(csch, psch->sch_id);

	return 0;
}

static bool is_virt_tbf_port_sch(struct qos_tc_qdisc *sch)
{
	struct qos_tc_port *port = NULL;

	if (sch->parent != TC_H_ROOT)
		return false;

	port = qos_tc_port_get(sch->dev);
	if (port && port->tbf_on)
		return true;

	return false;
}

int qos_tc_sched_del(struct qos_tc_qdisc *sch,
		const struct qos_tc_params *tc_params)
{
	struct dp_node_link node = {0};
	struct dp_node_alloc anode = {0};
#if IS_ENABLED(CONFIG_QOS_NOTIFY)
	struct qos_tc_port *port = NULL;
#endif
	int ret;

	trace_qos_tc_sched_del_enter(sch, 0);

	if (is_virt_tbf_port_sch(sch)) {
		sch->use_cnt = 0;
		return 0;
	}

	node.node_type = DP_NODE_SCH;
	node.node_id.sch_id = sch->sch_id;
	ret = dp_node_unlink(&node, 0);
	if (ret == DP_FAILURE) {
		netdev_err(sch->dev, "sched id %d unlink failed\n",
			   node.node_id.sch_id);
		return -ENODEV;
	}

	anode.type = DP_NODE_SCH;
	anode.id.sch_id = sch->sch_id;
	ret = dp_node_free(&anode, DP_NODE_AUTO_FREE_RES);
	if (ret == DP_FAILURE) {
		netdev_err(sch->dev, "sched id %d free failed\n",
			   node.node_id.sch_id);
		return -ENODEV;
	}

	sch->use_cnt = 0;

#if IS_ENABLED(CONFIG_QOS_NOTIFY)
	port = qos_tc_port_get(sch->dev);
	if (!port) {
		netdev_err(sch->dev, "failed to get port\n");
		return -ENODEV;
	}
	/*!
	 * clear the q_map bits for the given scheduler offset
	 */
	if (sch->offset <= QOS_TC_MAX_OFFSET) {
		port->q_map &= ~(QOS_TC_QMASK << sch->offset);
		/* Send scheduler deleted notification */
		qos_tc_check_and_notify(sch, QOS_EVENT_SCH_DELETE, QOS_TC_UNUSED,
				QOS_TC_UNUSED, QOS_TC_UNUSED, tc_params);
	} else{
		netdev_dbg(sch->dev, "q_map is full\n");
	}
#endif

	trace_qos_tc_sched_del_exit(sch, 0);

	return 0;
}

static int sched_del(struct qos_tc_qdisc *sch)
{
	struct dp_node_link node = {0};
	struct dp_node_alloc anode = {0};
	int ret;

	trace_qos_tc_sched_del_enter(sch, 0);

	node.node_type = DP_NODE_SCH;
	node.node_id.sch_id = sch->sch_id;
	ret = dp_node_unlink(&node, 0);
	if (ret == DP_FAILURE) {
		netdev_err(sch->dev, "sched id %d unlink failed\n",
			   node.node_id.sch_id);
		return -ENODEV;
	}

	anode.type = DP_NODE_SCH;
	anode.id.sch_id = sch->sch_id;
	ret = dp_node_free(&anode, DP_NODE_AUTO_FREE_RES);
	if (ret == DP_FAILURE) {
		netdev_err(sch->dev, "sched id %d free failed\n",
			   node.node_id.sch_id);
		return -ENODEV;
	}

	sch->use_cnt = 0;

	trace_qos_tc_sched_del_exit(sch, 0);

	return 0;
}

int qos_tc_add_child_qdisc(struct net_device *dev,
			   struct qos_tc_port *port,
			   enum qos_tc_qdisc_type type,
			   u32 parent,
			   u32 handle,
			   const struct qos_tc_params *tc_params)
{
	struct qos_tc_qdisc *qdisc = NULL;
	struct qos_tc_qdisc *csch = NULL;
	struct qos_tc_qdisc *psch = NULL;
	int idx = TC_H_MIN(parent) - 1;
	int prio_w = idx;
	int ret;

	if (!dev || !port)
		return -EINVAL;

	if (parent == TC_H_ROOT)
		idx = 0;

	if (idx < 0 || idx >= QOS_TC_MAX_Q)
		return -EINVAL;

	netdev_dbg(dev, "searching qdisc %#x idx: %d\n", parent, idx);
	if (parent != TC_H_ROOT) {
		psch = qos_tc_qdisc_find(port, parent);
		if (!psch) {
			netdev_err(dev, "%s: parent not found\n", __func__);
			return -ENODEV;
		}
	} else {
		psch = &port->root_qdisc;
	}

	/* if there is a queue on this input then delete it */
	if (psch->qids[idx].qid) {
		netdev_dbg(dev, "parent: %#x handle: %#x prio: %d\n",
			   psch->parent, psch->handle, psch->qids[idx].p_w);
		prio_w = psch->qids[idx].p_w;
		ret = qos_tc_queue_del(psch, idx, tc_params);
		if (ret < 0) {
			netdev_err(dev,
				   "%s: queue delete failed\n", __func__);
			return ret;
		}
	}
	/* if there is a sched on this input return with error for now */
	csch = psch->children[idx];
	if (csch && csch->sch_id) {
		netdev_err(dev, "%s input allocated by sched\n", __func__);
		return -EINVAL;
	}

	if (parent != TC_H_ROOT) {
		/* ready to add the sched */
		ret = qos_tc_alloc_qdisc(&qdisc);
		if (ret < 0)
			return -ENOMEM;

		psch->children[idx] = qdisc;
		psch->num_children++;
		qdisc->p_w = prio_w;
		/* get port data from parent qdisc */
		qdisc->dev = dev;
		qdisc->port = psch->port;
		qdisc->deq_idx = psch->deq_idx;
		qdisc->inst = psch->inst;
		qdisc->def_q = psch->def_q;
		qdisc->epn = psch->epn;
		qdisc->alloc_flag = psch->alloc_flag;
	} else {
		qdisc = psch;
		qdisc->dev = dev;
		qdisc->p_w = QOS_TC_UNUSED;
		ret = qos_tc_fill_port_data(qdisc, tc_params);
		if (ret < 0) {
			netdev_err(dev,
				   "%s: failed getting port hw config %d\n",
				   __func__, ret);
			goto err_free_qdisc;
		}
	}

	qdisc->type = type;
	qdisc->handle = handle;
	qdisc->parent = parent;
	qdisc->use_cnt = 1;
	if (parent == TC_H_ROOT) {
		ret = qos_tc_add_sched(psch, idx, tc_params);
		if (ret < 0) {
			netdev_err(dev, "%s: add sched failed\n", __func__);
			goto err_free_qdisc;
		}
		netdev_dbg(dev, "%s: add root sch id %d\n",
			   __func__, psch->sch_id);
	} else {
		netdev_dbg(dev, "root id: %d parent id: %d idx: %d <=> %d\n",
			   port->root_qdisc.sch_id, psch->sch_id, idx, prio_w);
		ret = qos_tc_add_staged_sched(psch, qdisc, prio_w, tc_params);
		if (ret < 0) {
			netdev_err(dev, "%s: add sched failed\n", __func__);
			goto err_free_qdisc;
		}
	}

	/* add to radix tree here */
	ret = radix_tree_insert(&port->qdiscs, TC_H_MAJ(handle), qdisc);
	if (ret) {
		netdev_err(dev, "qdisc insertion to radix tree failed: %d\n",
			   ret);
		goto err_free_sched;
	}

	return 0;

err_free_sched:
	qos_tc_sched_del(qdisc, tc_params);
err_free_qdisc:
	if (parent != TC_H_ROOT) {
		netdev_err(dev, "%s: freeing child qdisc\n", __func__);
		psch->children[idx] = NULL;
		psch->num_children--;
		qos_tc_free_qdisc(qdisc);
	}
	return ret;
}

static struct qos_tc_qdisc *get_parent_sch(struct qos_tc_port *port, u32 parent)
{
	struct net_device *dev = port->dev;
	struct qos_tc_qdisc *psch = NULL;
	struct qos_tc_qdisc *csch = NULL;
	int idx = parent != TC_H_ROOT ? TC_H_MIN(parent) - 1 : 0;

	if (!port)
		return ERR_PTR(-EINVAL);

	if (idx < 0 || idx >= QOS_TC_MAX_Q)
		return ERR_PTR(-EINVAL);

	if (parent != TC_H_ROOT) {
		psch = qos_tc_qdisc_find(port, parent);
		if (!psch) {
			netdev_err(dev, "%s: parent not found\n", __func__);
			return ERR_PTR(-ENODEV);
		}
	} else {
		psch = &port->root_qdisc;
	}

	/* if there is a sched on this input return with error for now */
	csch = psch->children[idx];
	if (csch && csch->sch_id) {
		netdev_err(dev, "%s input allocated by sched\n", __func__);
		return ERR_PTR(-EINVAL);
	}

	return psch;
}

static int add_qdisc(struct qos_tc_qdisc *psch, struct qos_tc_qdisc *qp)
{
	struct net_device *dev = psch->dev;
	struct qos_tc_port *port = qos_tc_port_get(dev);
	struct qos_tc_qdisc *qdisc = NULL;
	int idx = TC_H_MIN(qp->parent) - 1;
	int ret;

	/* ready to add the sched */
	ret = qos_tc_alloc_qdisc(&qdisc);
	if (ret < 0)
		return -ENOMEM;

	psch->children[idx] = qdisc;
	psch->num_children++;
	qdisc->p_w = qp->p_w;
	/* get port data from parent qdisc */
	qdisc->dev = psch->dev;
	qdisc->port = psch->port;
	qdisc->deq_idx = psch->deq_idx;
	qdisc->inst = psch->inst;
	qdisc->def_q = psch->def_q;
	qdisc->epn = psch->epn;

	qdisc->type = qp->type;
	qdisc->handle = qp->handle;
	qdisc->parent = qp->parent;
	qdisc->use_cnt = 1;
	qdisc->alloc_flag = psch->alloc_flag;

	netdev_dbg(dev, "%s: parent sch id: %d idx: %d <=> %d\n",
		   __func__, psch->sch_id, idx, qp->p_w);

	ret = add_child_sched(psch, qdisc, qp->p_w);
	if (ret < 0) {
		netdev_err(dev, "%s: add sched failed\n", __func__);
		goto err_free_qdisc;
	}

	/* add to radix tree here */
	ret = radix_tree_insert(&port->qdiscs, TC_H_MAJ(qp->handle), qdisc);
	if (ret) {
		netdev_err(dev, "qdisc insertion to radix tree failed: %d\n",
			   ret);
		goto err_free_sched;
	}

	return 0;

err_free_sched:
	sched_del(qdisc);

err_free_qdisc:
	netdev_err(dev, "%s: freeing child qdisc\n", __func__);
	psch->children[idx] = NULL;
	psch->num_children--;
	qos_tc_free_qdisc(qdisc);

	return ret;
}

static int add_qdisc_root(struct qos_tc_qdisc *psch, struct qos_tc_qdisc *qp)
{
	struct net_device *dev = psch->dev;
	struct qos_tc_port *port = qos_tc_port_get(dev);
	struct qos_tc_qdisc *qdisc = NULL;
	int idx = TC_H_MIN(qp->parent) - 1;
	int ret = 0;

	qdisc = psch;
	qdisc->dev = dev;
	qdisc->p_w = QOS_TC_UNUSED;
	ret = fill_port_data(qdisc);
	if (ret < 0) {
		netdev_err(dev,
			   "%s: failed getting port hw config %d\n",
			   __func__, ret);
		return -EINVAL;
	}

	qdisc->type = qp->type;
	qdisc->handle = qp->handle;
	qdisc->parent = qp->parent;
	qdisc->use_cnt = 1;

	ret = add_sched(psch, idx);
	if (ret < 0) {
		netdev_err(dev, "%s: add sched failed\n", __func__);
		return -EINVAL;
	}
	netdev_dbg(dev, "%s: add root sch id %d\n",
		   __func__, psch->sch_id);

	/* add to radix tree here */
	ret = radix_tree_insert(&port->qdiscs, TC_H_MAJ(qp->handle), qdisc);
	if (ret) {
		netdev_err(dev, "qdisc insertion to radix tree failed: %d\n",
			   ret);
		goto err_free_sched;
	}

	return 0;

err_free_sched:
	sched_del(qdisc);

	return ret;
}

static int tbf_queue_delete(struct qos_tc_qdisc *q, struct qos_tc_qdisc *new_q,
			    int idx)
{
	struct qos_tc_q_data *qid = &q->qids[idx];
	struct qos_tc_tbf_data *tbf = &q->tbfs[idx];

	int ret;

	if (!q->qids[idx].qid)
		return 0;

	netdev_dbg(q->dev, "%s: parent: %#x handle: %#x p_w: %d idx: %d qid: %u\n",
		   __func__, q->parent, q->handle, q->qids[idx].p_w,
		   idx, qid->qid);

	new_q->p_w = qid->p_w;

	/* remove shaper data from queue param list */
	ret = qos_tc_qdata_remove(q->dev, qid, tbf->handle, tbf->parent);
	if (ret < 0) {
		netdev_err(q->dev, "%s: tbf qdata remove failed\n", __func__);
		return ret;
	}

	ret = hw_queue_del(q, idx);
	if (ret < 0) {
		netdev_err(q->dev, "%s: queue delete failed\n", __func__);
		return ret;
	}

	return ret;
}

int qos_tc_add_tbf_child_qdisc(struct qos_tc_port *port,
			       enum qos_tc_qdisc_type type,
			       u32 parent,
			       u32 handle)
{
	struct net_device *dev = port->dev;
	struct qos_tc_qdisc *psch = NULL;
	int idx = TC_H_MIN(parent) - 1;
	int ret;

	struct qos_tc_qdisc qdisc = {
		.type = type,
		.handle = handle,
		.parent = parent,
	};

	netdev_dbg(dev, "%s adding sched p: %#x h:%#x\n",
		   __func__, parent, handle);
	psch = get_parent_sch(port, parent);
	if (IS_ERR_OR_NULL(psch)) {
		netdev_err(dev, "%s: get_parent_sch failed %ld\n",
			   __func__, PTR_ERR(psch));
		return PTR_ERR(psch);
	}
	netdev_dbg(dev, "%s: parent sch p:%#x h:%x -> input: %d\n",
		   __func__, psch->parent, psch->handle, idx);
	/* if there is a queue on this input then delete it */
	ret = tbf_queue_delete(psch, &qdisc, idx);
	if (ret) {
		netdev_err(dev, "%s: error removing tbf_queue\n", __func__);
		return -EIO;
	}
	netdev_dbg(dev, "%s: using p_w: %i\n",
		   __func__, qdisc.p_w);

	if (parent == TC_H_ROOT)
		return add_qdisc_root(psch, &qdisc);

	return add_qdisc(psch, &qdisc);
}

int qos_tc_add_sch_to_tbf_port(struct qos_tc_port *port,
			       enum qos_tc_qdisc_type type,
			       u32 parent, u32 handle)
{
	struct net_device *dev = port->dev;
	struct qos_tc_qdisc *psch = &port->root_qdisc;
	struct qos_tc_q_data *qid = &psch->qids[0];
	struct qos_tc_qdata_params *p;
	int ret;

	struct qos_tc_qdisc qdisc = {
		.type = type,
		.handle = handle,
		.parent = parent,
		.p_w = 0,
	};

	WARN_ON(!qid);

	list_for_each_entry(p, &qid->params, list)
		__qos_tc_qdata_remove(dev, qid, p);

	ret = hw_queue_del(psch, 0);
	if (ret < 0) {
		netdev_err(dev, "%s: queue delete failed\n", __func__);
		return ret;
	}

	return add_qdisc(psch, &qdisc);
}

static int qos_tc_sched_policy_update(struct qos_tc_qdisc *sch,
				      int arbi, int prio_w, int idx)
{	struct dp_node_prio prio_info = {0};
	int ret;

	/* update parent sched policy and set prio_w correctly */
	prio_info.id.sch_id = sch->qids[idx].qid;
	prio_info.type = DP_NODE_QUEUE;
	ret = dp_qos_link_prio_get(&prio_info, 0);
	if (ret == DP_FAILURE) {
		netdev_err(sch->dev, "%s :get prio node failed\n", __func__);
		return -EINVAL;
	}

	/* Re-set policy */
	prio_info.arbi = (sch->type == QOS_TC_QDISC_DRR) ?
		    ARBITRATION_WRR : ARBITRATION_WSP;
	ret = dp_qos_link_prio_set(&prio_info, 0);
	if (ret == DP_FAILURE) {
		netdev_err(sch->dev, "%s: set sch arbi failed\n", __func__);
		return -EINVAL;
	}

	prio_info.id.sch_id = sch->qids[idx].qid;
	prio_info.type = DP_NODE_QUEUE;
	ret = dp_qos_link_prio_get(&prio_info, 0);
	if (ret == DP_FAILURE) {
		netdev_err(sch->dev, "%s :get prio node failed\n", __func__);
		return -EINVAL;
	}

	prio_info.prio_wfq = prio_w;
	ret = dp_qos_link_prio_set(&prio_info, 0);
	if (ret == DP_FAILURE) {
		netdev_err(sch->dev, "%s: set sch arbi failed\n", __func__);
		return -EINVAL;
	}

	return 0;
}

#define WRED_GREEN_MAX_TH (500 * 1024)
#define WRED_GREEN_MIN_TH ((WRED_GREEN_MAX_TH * 90) / 100)
#define WRED_GREEN_MAX_P 100
#define WRED_YELLOW_MAX_TH (250 * 1024)
#define WRED_YELLOW_MIN_TH ((WRED_YELLOW_MAX_TH * 90) / 100)
#define WRED_YELLOW_MAX_P 100

#define WRED_UNASSIGNED_TH 0
#define WRED_CPU_TH 32
#define WRED_RED_TH 8000
#define MAX_QUEUE_LENGTH_1K 0x400
#define MAX_QUEUE_LENGTH_3K 0xC00

int qos_tc_queue_wred_defaults_set(struct qos_tc_qdisc *sch, int idx)
{
	struct dp_queue_conf q_cfg = {
		.q_id = sch->qids[idx].qid,
		.inst = sch->inst
	};

	/** Read */
	if (dp_queue_conf_get(&q_cfg, 0) == DP_FAILURE) {
		netdev_err(sch->dev, "%s: dp_queue_conf_get() failed\n",
			   __func__);
		return -EINVAL;
	}

	/** Modify */
	q_cfg.act = DP_NODE_EN;

	q_cfg.min_size[0] = WRED_GREEN_MIN_TH;
	q_cfg.max_size[0] = WRED_GREEN_MAX_TH;
	q_cfg.wred_slope[0] = WRED_GREEN_MAX_P;
	q_cfg.min_size[1] = WRED_YELLOW_MIN_TH;
	q_cfg.max_size[1] = WRED_YELLOW_MAX_TH;
	q_cfg.wred_slope[1] = WRED_YELLOW_MAX_P;

	q_cfg.wred_min_guaranteed = qos_tc_is_cpu_port(sch->port) ?
					WRED_CPU_TH : WRED_UNASSIGNED_TH;

#if (defined(CONFIG_X86_INTEL_LGM) || defined(CONFIG_SOC_LGM))
	/* Set queue length and drop algorithm based on the interface */
	if (sch->alloc_flag & DP_F_GPON) {
		q_cfg.wred_max_allowed = WRED_RED_TH;
		q_cfg.drop = DP_QUEUE_DROP_WRED;
	} else if (sch->alloc_flag & DP_F_FAST_ETH_LAN) {
		q_cfg.wred_max_allowed = WRED_RED_TH;
		q_cfg.codel = DP_CODEL_EN;
	} else if (sch->alloc_flag & (DP_F_FAST_ETH_WAN | DP_F_DIRECT)) {
		q_cfg.wred_max_allowed = MAX_QUEUE_LENGTH_3K;
		q_cfg.codel = DP_CODEL_EN;
	} else if (sch->alloc_flag & DP_F_VUNI) {
		q_cfg.wred_max_allowed = MAX_QUEUE_LENGTH_3K;
	} else if (sch->alloc_flag & DP_F_FAST_WLAN) {
		q_cfg.wred_max_allowed = MAX_QUEUE_LENGTH_1K;
	} else {
		q_cfg.wred_max_allowed = WRED_RED_TH;
		q_cfg.drop = DP_QUEUE_DROP_WRED;
	}
#else
	q_cfg.wred_max_allowed = WRED_RED_TH;
	q_cfg.drop = DP_QUEUE_DROP_WRED;
#endif

	/** Write */
	if (dp_queue_conf_set(&q_cfg, 1) == DP_FAILURE) {
		netdev_err(sch->dev, "%s: dp_queue_conf_set() failed\n",
			   __func__);
		return -EINVAL;
	}

	return 0;
}

int qos_tc_queue_add(struct qos_tc_qdisc *sch, int arbi, int prio_w, int idx,
		const struct qos_tc_params *tc_params)
{
	struct dp_node_link node = {0};
	struct dp_node_alloc anode = {0};
	int ret;

	if (!sch)
		return -EINVAL;

	trace_qos_tc_queue_add_enter(sch, &sch->qids[idx], idx);

	anode.inst = sch->inst;
	anode.dp_port = sch->port;
	anode.type = DP_NODE_QUEUE;
	anode.id.q_id = DP_NODE_AUTO_ID;
	ret = dp_node_alloc(&anode, 0);
	if (ret == DP_FAILURE) {
		netdev_err(sch->dev, "failed to alloc queue\n");
		return -ENOMEM;
	}

	sch->qids[idx].qid = anode.id.q_id;
	sch->qids[idx].arbi = arbi;
	sch->qids[idx].p_w = prio_w;
	INIT_LIST_HEAD(&sch->qids[idx].params);

	node.arbi = (arbi == QOS_TC_QDISC_DRR) ?
		    ARBITRATION_WRR : ARBITRATION_WSP;
	node.prio_wfq = prio_w;
	node.node_type = DP_NODE_QUEUE;
	node.node_id.q_id = sch->qids[idx].qid;
	node.p_node_id.sch_id = sch->sch_id;

	node.p_node_type = get_p_node_type(sch, DP_NODE_QUEUE);
	if (node.p_node_type == DP_NODE_PORT) {
		node.cqm_deq_port.cqm_deq_port = sch->epn;
		node.p_node_id.cqm_deq_port = sch->epn;
	}

	netdev_dbg(sch->dev, "%s: attach queue %d to sch id %d p_w: %u\n",
		   __func__, anode.id.q_id, sch->sch_id, node.prio_wfq);
	if (dp_node_link_add(&node, 0) == DP_FAILURE) {
		netdev_err(sch->dev, "failed to link queue\n");
		anode.type = DP_NODE_QUEUE;
		anode.id.q_id = sch->qids[idx].qid;
		ret = dp_node_free(&anode, DP_NODE_AUTO_FREE_RES);
		if (ret == DP_FAILURE)
			netdev_err(sch->dev, "qid %d free failed\n",
				   anode.id.q_id);
		memset(&sch->qids[idx], 0, sizeof(struct qos_tc_q_data));
		return -ENODEV;
	}

	sch->num_q++;

	if (node.p_node_type != DP_NODE_PORT) {
		netdev_dbg(sch->dev, "%s: update sch policy to %u\n",
			   __func__, arbi);
		ret = qos_tc_sched_policy_update(sch, arbi, prio_w, idx);
		if (ret < 0)
			netdev_err(sch->dev, "%s:policy set fail\n", __func__);
	}

	ret = qos_tc_queue_wred_defaults_set(sch, idx);
	if (ret < 0)
		netdev_err(sch->dev, "%s:wred defaults set fail\n", __func__);

#if IS_ENABLED(CONFIG_QOS_NOTIFY)
	/* Send queue added notification */
	if (sch->offset <= QOS_TC_MAX_OFFSET) {
		if (arbi == QOS_TC_QDISC_DRR)
			qos_tc_check_and_notify(sch, QOS_EVENT_Q_ADD, QOS_TC_UNUSED,
				idx, (sch->offset + idx), tc_params);
		else
			qos_tc_check_and_notify(sch, QOS_EVENT_Q_ADD, QOS_TC_UNUSED,
				idx, (sch->offset + prio_w), tc_params);
	} else{
		netdev_dbg(sch->dev, "q_map is full\n");
	}
#endif

	trace_qos_tc_queue_add_exit(sch, &sch->qids[idx], idx);

	return 0;
}


int qos_tc_queue_del(struct qos_tc_qdisc *sch, int idx,
		const struct qos_tc_params *tc_params)
{
	if (!sch)
		return -EINVAL;

	if (idx < 0 || idx > QOS_TC_MAX_Q - 1)
		return -EINVAL;

#if IS_ENABLED(CONFIG_QOS_NOTIFY)
	/* Send queue deleted notification */
	if (sch->offset <= QOS_TC_MAX_OFFSET) {
		if (sch->qids[idx].arbi == QOS_TC_QDISC_DRR)
			qos_tc_check_and_notify(sch, QOS_EVENT_Q_DELETE, QOS_TC_UNUSED,
				idx, (sch->offset + idx), tc_params);
		else
			qos_tc_check_and_notify(sch, QOS_EVENT_Q_DELETE, QOS_TC_UNUSED,
				idx, (sch->offset + sch->qids[idx].p_w), tc_params);
	} else{
		netdev_dbg(sch->dev, "q_map is full\n");
	}
#endif

	return hw_queue_del(sch, idx);
}

static int add_shaper_on_sch_unlink(struct qos_tc_qdisc *sch, int idx)
{
	struct qos_tc_q_data *qid = &sch->qids[idx];
	struct qos_tc_tbf_data *tbf = &sch->tbfs[idx];
	int ret;

	ret = qos_tc_shaper_add(sch, qid, tbf);
	if (ret < 0)
		return ret;

	ret = qos_tc_qdata_add_tbf(sch->dev, qid, tbf, tbf_remove);
	if (ret < 0)
		return ret;

	return qos_tc_add_qdisc_to_dev(sch->dev, sch, tbf->handle);
}

int qos_tc_qdisc_unlink(struct qos_tc_port *p, struct qos_tc_qdisc *sch,
		const struct qos_tc_params *tc_params)
{
	int idx = TC_H_MIN(sch->parent) - 1;
	struct qos_tc_qdisc *psch = NULL;
	int ret = 0;

	if (!p || !sch)
		return -EINVAL;

	if (idx < 0 || idx >= QOS_TC_MAX_Q)
		return -EINVAL;

	psch = qos_tc_qdisc_find(p, sch->parent);
	if (!psch)
		return -ENODEV;

	psch->children[idx] = NULL;
	psch->num_children--;

	/* We have a parent e.g. prio then re-add missing queue/band. */
	ret = qos_tc_queue_add(psch, psch->type, sch->p_w, idx, tc_params);
	if (ret < 0) {
		netdev_err(psch->dev, "%s: re-add queue fail\n", __func__);
		return -ECANCELED;
	}

	if (!sch->tbf_on)
		return 0;

	ret = add_shaper_on_sch_unlink(psch, idx);
	if (ret < 0) {
		netdev_err(psch->dev, "%s: tbf move to queue fail\n", __func__);
		return -ECANCELED;
	}

	netdev_dbg(psch->dev, "%s:%i -> sch %u\n", __func__, idx, psch->sch_id);
	return 0;
}

int qos_tc_qdisc_tree_del(struct qos_tc_port *p, struct qos_tc_qdisc *root,
		const struct qos_tc_params *tc_params)
{
	int i, ret;

	for (i = 0; i < QOS_TC_MAX_Q; i++) {
		struct qos_tc_qdisc *sch = root->children[i];

		if (sch && sch->use_cnt) {
			netdev_dbg(sch->dev, "%s: delete child %u of %u\n",
				   __func__,
				   sch->sch_id,
				   root->sch_id);
			ret = qos_tc_qdisc_tree_del(p, sch, tc_params);
			if (ret < 0)
				return ret;
		}
	}

	netdev_dbg(root->dev, "%s: del sched id: %d use: %d\n",
		   __func__, root->sch_id, root->use_cnt);
	if (!root->use_cnt)
		return 0;

	/* try removing the queues */
	for (i = 0; i < QOS_TC_MAX_Q; i++) {
		if (root->qids[i].qid) {
			netdev_dbg(root->dev, "%s: deleting qid: %d\n",
				   __func__, root->qids[i].qid);
			ret = qos_tc_queue_del(root, i, tc_params);
			if (ret < 0)
				return ret;
		}
	}
	netdev_dbg(root->dev, "%s: deleting sched %#x %#x\n",
		   __func__, root->parent, root->handle);
	ret = qos_tc_sched_del(root, tc_params);
	if (ret < 0)
		return ret;

	p->sch_num--;
	if (root->parent != TC_H_ROOT && TC_H_MIN(root->parent)) {
		netdev_dbg(root->dev, "%s: unlinking qdisk %#x\n",
			   __func__, root->parent);
		ret = qos_tc_qdisc_unlink(p, root, tc_params);
		if (ret < 0)
			netdev_err(root->dev, "%s: sch[%d] unlink failed\n",
				   __func__, root->sch_id);
	}

	WARN_ON(radix_tree_delete(&p->qdiscs, TC_H_MAJ(root->handle)) != root);
	if (root->parent != TC_H_ROOT)
		qos_tc_free_qdisc(root);
	else
		qos_tc_port_delete(p);

	return 0;
}

static void qos_tc_dump_qdisc(struct qos_tc_qdisc *sch)
{
	netdev_dbg(sch->dev, "hdl:%#x pid:%#x sta:%d numq:%d num sched:%d\n",
		   sch->handle, sch->parent, sch->use_cnt,
		   sch->num_q, sch->num_children);
}

int qos_tc_sched_status(struct qos_tc_port *p, struct qos_tc_qdisc *root)
{
	int i;

	qos_tc_dump_qdisc(root);

	for (i = 0; i < QOS_TC_MAX_Q; i++) {
		struct qos_tc_qdisc *sch = root->children[i];

		if (!sch)
			continue;

		qos_tc_sched_status(p, sch);
	}

	return 0;
}

static int qos_tc_set_qmap(int qid, struct qos_tc_qdisc *sch, int subif,
		int tc_cookie, bool en, const struct qos_tc_params *tc_params)
{
	struct dp_queue_map_set qmap_set = {0};
	int ret;

	if (WARN(qid < 0, "change invalid queue qid (%i) for subif: %i, en: %i",
		 qid, subif, en))
		return -EINVAL;

	qmap_set.inst = 0;
	qmap_set.mask.flowid = 1;

	/* If tc cookie is not set, assume class is same as subif */
	if (tc_cookie == QOS_TC_COOKIE_EMPTY)
		qmap_set.map.class = subif;
	else
		qmap_set.map.class = tc_cookie;

	if (tc_params && (tc_params->flags & QOS_TC_Q_MAP)) {
		qmap_set.q_id = en ? qid : tc_params->def_q;
		qmap_set.map.dp_port = tc_params->qmap_port;
		qmap_set.map.mpe1 = 1;
		qmap_set.map.mpe2 = 0;
		qmap_set.mask.enc = 1;
		qmap_set.mask.dec = 1;
		qmap_set.mask.subif = 1;
	} else {
		qmap_set.q_id = en ? qid : 0;
		qmap_set.map.dp_port = sch->port;
		qmap_set.map.subif = subif;

		if (!qos_tc_is_cpu_port(sch->port)) {
			/* Do not set the flags below for the CPU port otherwise the
			 * lookup table entry for the re-insert queue is overwritten.
			 * The re-inserted packets carry mpe1 = 0 and
			 * mpe = 0 in the DMA descriptor and they have to be kept.
			 * Not used on URX, only relevant on PRX. URX uses dedicated
			 * logical port.
			 */
			qmap_set.mask.mpe1 = 1;
			qmap_set.mask.mpe2 = 1;
		}

#if (defined(CONFIG_X86_INTEL_LGM) || defined(CONFIG_SOC_LGM))
		/*
		 * URX specific setting: egflag should be set only for egress ports,
		 * and skipped for non-egress ports like CPU and VUNI.
		 */
		if (!qos_tc_is_cpu_port(sch->port) && !qos_tc_is_vuni_dev(sch->dev)) {
			qmap_set.map.egflag = 1;
			qmap_set.mask.egflag = 0;
		}
#endif
		if (qos_tc_is_netdev_reinsert_port(sch->dev)) {
			/* subif to be ignored for reinsertion port */
			qmap_set.mask.subif = 1;
			/* enc is relevant for PRX only */
			qmap_set.map.enc = 1;
		}
	}

	ret = dp_queue_map_set(&qmap_set, 0);
	if (ret == DP_FAILURE) {
		pr_err("%s: queue map set failed\n", __func__);
		return -EINVAL;
	}

	return 0;
}

static int qos_tc_config_cpu_port(struct qos_tc_q_data *qid,
				  struct qos_tc_qdisc *sch,
				  int subif, char tc_cookie, bool en,
				  const struct qos_tc_params *tc_params)
{
	int ret;

	ret = qos_tc_set_qmap(qid->qid, sch, subif, tc_cookie, en, tc_params);
	if (ret != 0)
		return ret;

	if (!en)
		atomic_dec(&qid->ref_cnt);
	else
		atomic_inc(&qid->ref_cnt);

	return ret;
}

static int qos_tc_set_qmap_extraction(struct qos_tc_q_data *qid, int port,
				      bool en)
{
	int ret;
	struct dp_queue_map_set qmap_set = {
		.inst = 0,
		.q_id = en ? qid->qid : 0,
		.map = {
			.dp_port = port,
			.mpe1 = 0,
			.mpe2 = 1,
		},
		.mask = {
			.subif = 1,
			.class = 1,
			.flowid = 1,
		}
	};

	ret = dp_queue_map_set(&qmap_set, 0);
	if (ret == DP_FAILURE) {
		pr_err("%s: queue map set failed\n", __func__);
		return -EINVAL;
	}

	if (!en)
		atomic_dec(&qid->ref_cnt);
	else
		atomic_inc(&qid->ref_cnt);

	return 0;
}

static int qos_tc_set_qmap_cpu_from_indev(struct qos_tc_q_data *qid, int port,
					  int tc_cookie,
					  bool en)
{
	int ret;
	struct dp_queue_map_set qmap_set = {
		.inst = 0,
		.q_id = en ? qid->qid : 0,
		.map = {
			.dp_port = port,
			.mpe1 = 0,
			.class = tc_cookie,
		},
		.mask = {
			.mpe2 = 1,
			.subif = 1,
			.flowid = 1,
		}
	};

	ret = dp_queue_map_set(&qmap_set, 0);
	if (ret == DP_FAILURE) {
		pr_err("%s: queue map set failed\n", __func__);
		return -EINVAL;
	}

	if (!en)
		atomic_dec(&qid->ref_cnt);
	else
		atomic_inc(&qid->ref_cnt);

	return 0;
}

static int qos_tc_force_tc(struct net_device *dev, int pqn, bool en)
{
	struct dp_tc_cfg tc_n = {};
	int ret;

	if (en) {
		tc_n.dev = dev;
		/* Force the TC based on the PQ number i.e.,
		 * PQN = 1 e [1..8] goes to TC = 7 e [0..7]
		 */
		tc_n.tc = (QOS_TC_8021P_HIGHEST_PRIO - (pqn - 1));
		tc_n.force = 1;
	} else {
		tc_n.dev = dev;
		tc_n.tc = 0;
		tc_n.force = 0;
	}

	ret = dp_ingress_ctp_tc_map_set(&tc_n, 0);
	if (ret != 0)
		netdev_err(dev, "%s: force tc fail\n", __func__);

	return ret;
}

static int qos_tc_map_qid(struct qos_tc_q_data *qid,
			  struct qos_tc_qdisc *sch,
			  int subif,
			  char tc_cookie,
			  bool en)
{
	int ret;

	ret = qos_tc_set_qmap(qid->qid, sch, subif, tc_cookie, en, NULL);
	if (ret != 0)
		return -EFAULT;

	qos_tc_check_and_notify(sch, en ? QOS_EVENT_Q_ADD : QOS_EVENT_Q_DELETE,
		QOS_TC_UNUSED, qid->p_w, (sch->offset + tc_cookie), NULL);

	if (!en)
		atomic_dec(&qid->ref_cnt);
	else
		atomic_inc(&qid->ref_cnt);

	return ret;
}

static int qos_tc_is_ds_virt_port(struct qos_tc_qdisc *sch,
				  struct qos_tc_qmap_tc *q_tc)
{
	return (sch->ds && q_tc->indev && qos_tc_is_lan_dev(q_tc->indev));
}

/* We need this function so that the command:
 *
 *    tc filter add dev eth0_0 ingress pref 1 protocol 802.1q flower \
 *        skip_sw vlan_prio 0 classid 8002:1 action ok
 *
 * would trigger mappings for each subif, which for example effectively means
 * that it will "replicate" the mappings for eth0_0 subinterfaces (like:
 * eth0_0_1, eth0_0_2, ...)
 */
static int qos_tc_replicate_mappings_for_subifs(struct net_device *dev,
						struct qos_tc_q_data *qid,
						struct qos_tc_qdisc *sch,
						int tc, bool en)
{
	struct dp_port_prop prop;
	int i = 0;
	int ret = 0;
	dp_subif_t *subif __free(kfree) = NULL;

	struct net_device *netdev;

	if (dp_get_port_prop(sch->inst, sch->port, &prop))
		return -EINVAL;

	subif = kzalloc(sizeof(*subif), GFP_KERNEL);
	if (!subif) {
		netdev_err(dev, "%s: failed to allocate memory for subif\n",
			   __func__);
		return -ENOMEM;
	}

	read_lock(&dev_base_lock);

	/* Perform (subif, tc) -> (queue) mappings for each sub interface */
	for_each_netdev(&init_net, netdev) {
		if (dp_get_netif_subifid(netdev, NULL, NULL, NULL, subif, 0))
			continue;
		if (subif->port_id != sch->port)
			continue;

		/*
		 * If en == false, this should restore the default queue
		 * mappings. If en is false in qos_tc_set_qmap() we will map
		 * to qid=0 which is the drop queue.
		 * So when we stop the omcid the normal traffic is dropped, but
		 * the default setting done by the eth driver is to make the
		 * traffic pass by default.
		 * Therefore we should map to default queue, instead of drop
		 * queue, if en == false.
		 */
		if (en)
			ret = qos_tc_set_qmap(qid->qid, sch, subif->subif, tc, en, NULL);
		else
			ret = qos_tc_set_qmap(sch->def_q, sch, subif->subif, tc, true, NULL);
		if (ret)
			goto cleanup;
		i++;
	}

cleanup:
	if (ret) {
		en = !en;
		for_each_netdev(&init_net, netdev) {
			if (dp_get_netif_subifid(netdev, NULL, NULL, NULL, subif, 0))
				continue;
			if (subif->port_id != sch->port)
				continue;
			if (i <= 0)
				break;

			if (en)
				ret = qos_tc_set_qmap(qid->qid, sch,
						      subif->subif, tc, en, NULL);
			else
				ret = qos_tc_set_qmap(sch->def_q, sch,
						      subif->subif, tc, true, NULL);
			i--;
		}
	}

	read_unlock(&dev_base_lock);

	return ret;
}

/* classid option identifies the queue id.
 *
 * Queue mapping scenarios PON upstream:
 *  - GEM port is mapped to specific queue: The GEM port netdev is passed using
 *    the indev option and defines the subif/CTP which is mapped to the qid.
 *    "... flower indev gem1023 classid 100:1 action ok"
 *
 *  - GEM port + Traffic Class to specific queue: The GEM port netdev is passed
 *    using the indev option and defines the subif/CTP and the tc action cookie
 *    defines the traffic class [0..15] which are mapped to the queue id.
 *    "... flower indev gem1029 classid 100:1 action cookie 07 ok"
 *
 * Queue mapping scenarios PON downstream:
 *  - GEM port is mapped to specific queue: The GEM port netdev is passed using
 *    the indev option and defines the subif/CTP which force traffic class
 *    configuration is set according to the classid number i.e. queue priority.
 *    "... flower indev gem1023 classid 100:1 action ok"
 *
 *  - Traffic Class is mapped to specific queue: The traffic class is passed
 *    either using the vlan_prio option [0..7] where each pbit=0 -> TC=0 and so
 *    on or using the tc action cookie.
 *    "... flower classid 100:1 action cookie 07 ok"
 *    "... flower vlan_prio 7 classid 100:1 action ok"
 *
 *  In downstream additionally extVLAN rules to reassign the TC after the vlan
 *  operations are configured. The reassignment maps pbit=0 -> TC=0 and so on.
 *
 *  The above queue mappings can be applied independently for any netdev and
 *  direction if required.
 */

int qos_tc_update_ds_qmap(struct net_device *dev,
			  struct qos_tc_qdisc *sch,
			  struct qos_tc_q_data *qid,
			  struct qos_tc_qmap_tc *q_tc,
			  bool en)
{
	int pqn;
	int ret;
	int class;

	/* traffic class can be stored in either of below two parameters */
	if (q_tc->tc_cookie != -1)
		class = q_tc->tc_cookie;
	else
		class = q_tc->tc;

	if (q_tc->indev && qos_tc_is_gpon_dev(q_tc->indev)) {
		/* Get the queue id from the handle. In PON pq = 1 is
		 * the one with the highest priority e.g.
		 * handle 100:1 has higher prio than 100:7
		 */
		pqn = TC_H_MIN(q_tc->handle);
		return qos_tc_force_tc(q_tc->indev, pqn, en);
	}

	/* traffic class can be stored in either of below two parameters */
	if (q_tc->tc_cookie != -1)
		class = q_tc->tc_cookie;
	else
		class = q_tc->tc;

	/* Perform the mapping also for all subinterfaces */
	ret = qos_tc_replicate_mappings_for_subifs(dev, qid, sch,
						   class, en);
	if (ret)
		return ret;

	return 0;
}

int qos_tc_update_us_qmap(struct net_device *dev, struct qos_tc_qdisc *sch,
			  struct qos_tc_q_data *qid,
			  struct qos_tc_qmap_tc *q_tc, bool en)
{
	int ret;

	ret = qos_tc_map_qid(qid, sch, q_tc->subif, q_tc->tc_cookie, en);
	if (ret) {
		netdev_err(dev, "%s: qid map err\n", __func__);
		return ret;
	}

	return 0;
}

/*
 * If mapping is egress and it has indev specified as cpu port,
 * then it is an extraction mapping
 */
static bool qos_tc_is_extraction(struct qos_tc_qmap_tc *q_tc)
{
	if (q_tc->ingress)
		return false;
	if (!q_tc->indev)
		return false;

	return qos_tc_is_netdev_cpu_port(q_tc->indev);
}

static int qos_tc_get_qid_data(struct net_device *dev,
			       struct qos_tc_qmap_tc *q_tc,
			       struct qos_tc_q_data **qid)
{
	int ret;

	ret = qos_tc_get_grandparent_queue_by_handle(dev, q_tc->handle, qid);
	if (ret < 0 || !*qid) {
		ret = qos_tc_get_queue_by_handle(dev, q_tc->handle, qid);
		if (ret < 0 || !*qid) {
			netdev_err(dev, "%s: handle %#x not found err %d\n",
				   __func__, q_tc->handle, ret);
			return -EINVAL;
		}
	}

	return ret;
}

static void dump_qid_map_config(struct qos_tc_qdisc *sch,
				struct qos_tc_q_data *qid,
				struct qos_tc_qmap_tc *q_tc,
				bool en)
{
	struct net_device *dev = sch->dev;

	netdev_dbg(dev,
		   "in:%s port:%d dqi:%d q:%d cls:%#x tc:%d/%u sif: %d %s %s\n",
		   q_tc->indev ? q_tc->indev->name : "none",
		   sch->port, sch->deq_idx, qid->qid, q_tc->handle,
		   q_tc->tc, q_tc->tc_cookie, q_tc->subif,
		   sch->ds ? "ds" : "us", en ? "on" : "off");
}
/*
 * Does the qmap update for extraction mapping (mapping for which
 * qos_tc_is_extraction() returns true)
 */
static int qos_tc_update_qmap_extraction(struct qos_tc_qdisc *sch,
					 struct qos_tc_qmap_tc *q_tc,
					 bool en,
					 u32 *queue_id)
{
	struct qos_tc_q_data *qid = NULL;
	struct net_device *dev = sch->dev;
	int ret;

	ret = qos_tc_get_qid_data(q_tc->indev, q_tc, &qid);
	if (ret)
		return ret;

	dump_qid_map_config(sch, qid, q_tc, en);

	trace_qos_tc_update_qmap_enter(dev, sch, qid, q_tc, en);

	ret = qos_tc_set_qmap_extraction(qid, sch->port, en);
	if (ret != 0) {
		netdev_err(dev, "%s: qid map err\n", __func__);
		return -EFAULT;
	}

	trace_qos_tc_update_qmap_exit(dev, sch, qid, q_tc, en);

	*queue_id = qid->qid;

	return ret;
}

/*
 * If mapping is ingress and it has indev specified as any other
 * device but cpu port, then it means we want to map traffic
 * from specific device to specific cpu queue
 */
static bool qos_tc_is_cpu_from_indev(struct net_device *dev,
				     struct qos_tc_qmap_tc *q_tc)
{
	if (!q_tc->ingress)
		return false;
	if (!q_tc->indev)
		return false;
	if (!qos_tc_is_netdev_cpu_port(dev))
		return false;
	if (qos_tc_is_netdev_cpu_port(q_tc->indev))
		return false;

	return true;
}

/*
 * Does the qmap update for the traffic ingressing to CPU to specific
 * CPU queues from specific netdev (mapping for which
 * qos_tc_is_cpu_from_indev() returns true)
 */
static int qos_tc_update_qmap_cpu_from_indev(struct qos_tc_qdisc *sch,
					     struct qos_tc_qmap_tc *q_tc,
					     bool en,
					     u32 *queue_id)
{
	struct qos_tc_q_data *qid = NULL;
	struct net_device *dev = sch->dev;
	int ret;
	int port;
	dp_subif_t *subif __free(kfree) = NULL;

	subif = kzalloc(sizeof(*subif), GFP_KERNEL);
	if (!subif) {
		netdev_err(sch->dev, "%s: failed to allocate memory for subif\n",
			   __func__);
		return -ENOMEM;
	}

	ret = dp_get_netif_subifid(q_tc->indev, NULL, NULL, 0, subif, 0);
	if (ret < 0) {
		if (!en) {
			/* If port is not there, there are no mappings.
			 * So if we try to unmap a queue from non-existent port,
			 * we can consider this successful */
			return 0;
		}
		return -ENODEV;
	}
	port = subif->port_id;

	ret = qos_tc_get_qid_data(dev, q_tc, &qid);
	if (ret)
		return ret;

	dump_qid_map_config(sch, qid, q_tc, en);

	ret =
	    qos_tc_set_qmap_cpu_from_indev(qid, port, q_tc->tc_cookie, en);
	if (ret != 0) {
		netdev_err(dev, "%s: qid map err (%d)\n", __func__, ret);
		return -EFAULT;
	}

	*queue_id = qid->qid;

	return ret;
}

/* Updates the qmap for most of the queue mapping cases (except extraction) */
static int qos_tc_update_qmap_default(struct qos_tc_qdisc *sch,
				      struct qos_tc_qmap_tc *q_tc,
				      bool en,
				      u32 *queue_id,
				      const struct qos_tc_params *tc_params)
{
	struct net_device *dev = sch->dev;
	struct qos_tc_q_data *qid = NULL;
	int ret;


	ret = qos_tc_get_qid_data(dev, q_tc, &qid);
	if (ret)
		return ret;

	dump_qid_map_config(sch, qid, q_tc, en);

	trace_qos_tc_update_qmap_enter(dev, sch, qid, q_tc, en);

	*queue_id = qid->qid;

	/* TODO: DPM uses port zero currently as the CPU port */
	if (qos_tc_is_cpu_port(sch->port)) {
		ret = qos_tc_config_cpu_port(qid, sch, 0, q_tc->tc_cookie, en,
				tc_params);
		trace_qos_tc_update_qmap_exit(dev, sch, qid, q_tc, en);
		return ret;
	}

	if (sch->ds) {
		ret = qos_tc_update_ds_qmap(dev, sch, qid, q_tc, en);
		trace_qos_tc_update_qmap_exit(dev, sch, qid, q_tc, en);
		return ret;
	}

	ret = qos_tc_update_us_qmap(dev, sch, qid, q_tc, en);
	trace_qos_tc_update_qmap_exit(dev, sch, qid, q_tc, en);

	return ret;
}

static int qos_tc_get_tc(struct qos_tc_qmap_tc *q_tc)
{
	/* traffic class can be stored in either of below two parameters */
	return (q_tc->tc_cookie != -1) ? q_tc->tc_cookie : q_tc->tc;
}

static int qos_tc_map_subif_tc_qid(struct qos_tc_qdisc *sch,
				   struct qos_tc_qmap_tc *q_tc,
				   u32 qid, bool en)
{
	u32 sif = q_tc->subif;
	int cls = qos_tc_get_tc(q_tc);

	if (en)
		return qos_tc_set_qmap(qid, sch, sif, cls, en, NULL);
	else
		return qos_tc_set_qmap(sch->def_q, sch, sif, cls, true, NULL);
}

static int qos_tc_update_ds_qmap_virt_port(struct qos_tc_qdisc *sch,
					   struct qos_tc_qmap_tc *q_tc,
					   bool en, u32 *queue_id)
{
	struct qos_tc_q_data *qid = NULL;
	struct net_device *dev = sch->dev;
	int ret;

	ret = qos_tc_get_qid_data(dev, q_tc, &qid);
	if (ret)
		return ret;

	dump_qid_map_config(sch, qid, q_tc, en);

	trace_qos_tc_update_qmap_enter(dev, sch, qid, q_tc, en);

	*queue_id = qid->qid;

	ret = qos_tc_map_subif_tc_qid(sch, q_tc, qid->qid, en);

	trace_qos_tc_update_qmap_exit(dev, sch, qid, q_tc, en);

	return ret;
}

int qos_tc_update_qmap(struct net_device *dev,
		       struct qos_tc_qmap_tc *q_tc,
		       bool en,
		       u32 *qid,
		       const struct qos_tc_params *tc_params)
{
	struct qos_tc_port *port = NULL;
	struct qos_tc_qdisc *sch = NULL;

	port = qos_tc_port_get(dev);
	if (!port) {
		if (!en) {
			/* If port is not there, there are no mappings.
			 * So if we try to unmap a queue from non-existent port,
			 * we can consider this successful.
			 */
			return 0;
		}
		netdev_err(dev, "%s: qos port does not exist\n", __func__);
		return -ENODEV;
	}
	sch = &port->root_qdisc;

	if (qos_tc_is_extraction(q_tc))
		return qos_tc_update_qmap_extraction(sch, q_tc, en, qid);

	if (qos_tc_is_cpu_from_indev(dev, q_tc))
		return qos_tc_update_qmap_cpu_from_indev(sch, q_tc, en, qid);

	if (qos_tc_is_ds_virt_port(sch, q_tc))
		return qos_tc_update_ds_qmap_virt_port(sch, q_tc, en, qid);

	return qos_tc_update_qmap_default(sch, q_tc, en, qid, tc_params);
}

int qos_tc_ports_cleanup(void)
{
	struct qos_tc_port *p, *n;
	int ret;

	list_for_each_entry_safe(p, n, &port_list, list) {
		ret = qos_tc_qdisc_tree_del(p, &p->root_qdisc, NULL);
		if (ret < 0) {
			pr_err("%s: error freeing port\n", __func__);
			return ret;
		}
		qos_tc_port_delete(p);
	}

	return 0;
}

int qos_tc_qdata_get_by_type(struct net_device *dev,
			     struct qos_tc_q_data *qid,
			     enum qos_tc_qdata_type type,
			     u32 *handle, u32 *parent)
{
	struct qos_tc_qdata_params *p;

	list_for_each_entry(p, &qid->params, list) {
		if (p->type == type) {
			netdev_dbg(dev, "%s: pid:%#x class/handle:%#x\n",
				   __func__, p->parent, p->handle);
			*handle = p->handle;
			*parent = p->parent;
			return 0;
		}
	}

	netdev_dbg(dev, "%s: type not found\n", __func__);
	return -EINVAL;
}

int qos_tc_qdata_add(struct net_device *dev, struct qos_tc_q_data *qid,
		     u32 handle, u32 parent, enum qos_tc_qdata_type type,
		     int (*destroy)(struct net_device *dev, u32 handle,
				    u32 parent))
{
	struct qos_tc_qdata_params *params;
	u32 tmp_h, tmp_p;

	/* Only one queue setting type allowed */
	if (!qos_tc_qdata_get_by_type(dev, qid, type, &tmp_h, &tmp_p)) {
		netdev_err(dev, "%s: Duplicated setting pid:%#x class/handle:%#x\n",
			   __func__, parent, handle);
		return -ENODEV;
	}

	params = kzalloc(sizeof(*params), GFP_KERNEL);
	if (!params)
		return -ENOMEM;

	params->handle = handle;
	params->parent = parent;
	params->type = type;
	params->destroy = destroy;
	params->qid = qid;

	list_add(&params->list, &qid->params);

	return 0;
}

static int get_qid_idx(struct qos_tc_qdisc *qdisc, struct qos_tc_q_data *qid)
{
	int idx;

	for (idx = 0; idx < QOS_TC_MAX_Q; idx++)
		if (!memcmp(&qid->qid, &qdisc->qids[idx], sizeof(qid->qid)))
			return idx;

	return -EINVAL;
}

int qos_tc_del_tbf_from_qdisc(struct qos_tc_qdisc *qdisc,
			      struct qos_tc_q_data *qid)
{
	int idx;

	idx = get_qid_idx(qdisc, qid);
	if (idx < 0) {
		netdev_err(qdisc->dev, "%s: cannot get qid idx\n", __func__);
		return -EIO;
	}

	memset(&qdisc->tbfs[idx], 0, sizeof(qdisc->tbfs[idx]));

	return 0;
}

int tbf_remove(struct net_device *dev, u32 handle, u32 parent)
{
	struct qos_tc_qdisc *qdisc = NULL;
	struct qos_tc_q_data *qid = NULL;
	struct qos_tc_port *port = NULL;
	int ret;

	port = qos_tc_port_get(dev);
	if (!port) {
		netdev_err(dev, "%s: no port found\n", __func__);
		return 0;
	}

	qdisc = qos_tc_qdisc_find(port, parent);
	if (!qdisc) {
		netdev_err(dev, "%s: qdisc doesn't exist\n", __func__);
		return 0;
	}

	qid = qos_tc_qdata_qid_get(dev, qdisc, parent);
	if (!qid) {
		ret = qos_tc_get_queue_by_handle(dev, parent, &qid);
		if (ret < 0 || !qid) {
			netdev_err(dev, "%s: handle not found err %d\n",
				   __func__, ret);
			return -EINVAL;
		}
	}

	ret = qos_tc_shaper_remove(qdisc, qid);
	if (ret < 0)
		return ret;

	radix_tree_delete(&port->qdiscs, TC_H_MAJ(handle));

	return 0;
}

int qos_tc_qdata_add_tbf(struct net_device *dev, struct qos_tc_q_data *qid,
			 struct qos_tc_tbf_data *tbf,
			 int (*destroy)(struct net_device *dev, u32 handle,
					u32 parent))
{
	enum qos_tc_qdata_type type = QOS_TC_QDATA_TBF;
	struct qos_tc_qdata_params *params;
	u32 tmp_h, tmp_p;

	/* Only one queue setting type allowed */
	if (!qos_tc_qdata_get_by_type(dev, qid, type, &tmp_h, &tmp_p)) {
		netdev_err(dev, "%s: Duplicated setting pid:%#x class/handle:%#x\n",
			   __func__, tbf->parent, tbf->handle);
		return -ENODEV;
	}

	params = kzalloc(sizeof(*params), GFP_KERNEL);
	if (!params)
		return -ENOMEM;

	params->handle = tbf->handle;
	params->parent = tbf->parent;
	params->type = QOS_TC_QDATA_TBF;
	params->destroy = destroy;
	params->qid = qid;

	/* Convert from bytes to kilobits */

	list_add(&params->list, &qid->params);

	return 0;
}

struct qos_tc_q_data *qos_tc_qdata_qid_get(struct net_device *dev,
					   struct qos_tc_qdisc *qdisc,
					   u32 parent)
{
	struct qos_tc_q_data *qid;
	struct qos_tc_qdata_params *p;
	int idx;

	for (idx = 0; idx < QOS_TC_MAX_Q; idx++) {
		qid = &qdisc->qids[idx];
		if (!(qid->qid))
			continue;
		list_for_each_entry(p, &qid->params, list) {
			if (TC_H_MAJ(p->handle) == TC_H_MAJ(parent))
				return qid;
		}
	}

	return NULL;
}

static int qos_tc_qdata_child_remove(struct net_device *dev,
				     struct qos_tc_q_data *qid,
				     u32 handle)
{
	struct qos_tc_qdata_params *p, *n;
	int ret;

	list_for_each_entry_safe(p, n, &qid->params, list) {
		if (TC_H_MAJ(p->parent) == TC_H_MAJ(handle)) {
			netdev_dbg(dev, "%s: pid:%#x class/handle:%#x\n",
				   __func__, p->parent, p->handle);
			ret = __qos_tc_qdata_remove(dev, qid, p);
			if (ret)
				return ret;
		}
	}

	return 0;
}

static int __qos_tc_qdata_remove(struct net_device *dev,
				 struct qos_tc_q_data *qid,
				 struct qos_tc_qdata_params *p)
{
	int ret;

	netdev_dbg(dev, "%s: pid:%#x class/handle:%#x\n",
		   __func__, p->parent, p->handle);

	ret = qos_tc_classid_unmap(p->handle);
	if (ret)
		return ret;

	/* Remove all successors before removing ancestor */
	ret = qos_tc_qdata_child_remove(dev, qid, p->handle);
	if (ret)
		return ret;

	ret = p->destroy(dev, p->handle, p->parent);
	if (ret) {
		netdev_err(dev, "%s: failed to destroy qdata parameter\n",
			   __func__);
		return ret;
	}

	list_del(&p->list);
	kfree(p);

	return 0;
}

int qos_tc_qdata_remove(struct net_device *dev, struct qos_tc_q_data *qid,
			u32 handle, u32 parent)
{
	struct qos_tc_qdata_params *p;

	list_for_each_entry(p, &qid->params, list) {
		if (p->handle == handle && p->parent == parent)
			return __qos_tc_qdata_remove(dev, qid, p);
	}

	netdev_dbg(dev, "%s: failed to find qdata parameter\n",
		   __func__);
	return 0;
}

static void qos_tc_shaper_config_init(struct qos_tc_qdisc *sch,
				      struct qos_tc_q_data *qdata,
				      enum dp_shaper_cmd cmd,
				      struct dp_shaper_conf *cfg)
{
	cfg->inst = sch->inst;
	cfg->type = DP_NODE_QUEUE;
	cfg->cmd = cmd;
	cfg->id.q_id = (int)qdata->qid;
}

/* Time to Length, convert time in ns to length in bytes
 * to determinate how many bytes can be sent in given time.
 * Copied from net/sched/sch_tbf.c
 */
u64 psched_ns_t2l(const struct psched_ratecfg *r,
		  u64 time_in_ns)
{
	/* The formula is :
	 * len = (time_in_ns * r->rate_bytes_ps) / NSEC_PER_SEC
	 */
	u64 len = time_in_ns * r->rate_bytes_ps;

	do_div(len, NSEC_PER_SEC);
	/* Round up in case there is a remainder. Sometimes one gets lost in
	 * the div operations and then the eps in the PPv4 is one less.
	 */
	if (len * NSEC_PER_SEC < time_in_ns * r->rate_bytes_ps)
		len++;

	if (unlikely(r->linklayer == TC_LINKLAYER_ATM)) {
		do_div(len, 53);
		len = len * 48;
	}

	if (len > r->overhead)
		len -= r->overhead;
	else
		len = 0;

	return len;
}

int qos_tc_shaper_add(struct qos_tc_qdisc *sch,
		      struct qos_tc_q_data *qdata,
		      struct qos_tc_tbf_data *tbf)
{
	struct dp_shaper_conf cfg = {0};
	int ret;

	qos_tc_shaper_config_init(sch, qdata, DP_SHAPER_CMD_ADD, &cfg);

	netdev_dbg(sch->dev, "%s: inst: %d type: %d cmd: %d q_id: %d sch_id: %d cqm_deq_port: %d\n",
		   __func__, cfg.inst, cfg.type, cfg.cmd, cfg.id.q_id,
		   cfg.id.sch_id, cfg.id.cqm_deq_port);

	/* Convert from bytes to kilobits */
	cfg.cir = tbf->cir;
	cfg.pir = tbf->pir;

	cfg.cbs = tbf->cbs;
	cfg.pbs = tbf->pbs;

	ret = dp_shaper_conf_set(&cfg, 0);
	if (ret == DP_FAILURE) {
		netdev_err(sch->dev, "%s: shaper config set failed %d\n",
			   __func__, ret);
		return -EINVAL;
	}

	return 0;
}

int qos_tc_shaper_remove(struct qos_tc_qdisc *sch,
			 struct qos_tc_q_data *qdata)
{
	struct dp_shaper_conf cfg = {0};
	int ret;

	qos_tc_shaper_config_init(sch, qdata, DP_SHAPER_CMD_REMOVE, &cfg);

	netdev_dbg(sch->dev, "%s: inst: %d type: %d cmd: %d q_id: %d sch_id: %d cqm_deq_port: %d\n",
		   __func__, cfg.inst, cfg.type, cfg.cmd, cfg.id.q_id,
		   cfg.id.sch_id, cfg.id.cqm_deq_port);

	ret = dp_shaper_conf_set(&cfg, 0);
	if (ret == DP_FAILURE) {
		netdev_err(sch->dev, "%s: shaper config set failed %d\n",
			   __func__, ret);
		return -EINVAL;
	}

	return 0;
}

bool qos_tc_is_cpu_port(int port)
{
	return port == QOS_TC_CPU_PORT;
}

bool qos_tc_is_netdev_cpu_port(struct net_device *dev)
{
	struct qos_tc_port *port = NULL;

	port = qos_tc_port_get(dev);
	if (!port)
		return false;

	return qos_tc_is_cpu_port(port->root_qdisc.port);
}

static void print_qos_tc_qdisc(struct seq_file *file,
			       struct qos_tc_qdisc p)
{
	seq_puts(file, "root_qdisc:\n");

	switch (p.type) {
	case QOS_TC_QDISC_NONE:
		seq_puts(file, "\ttype: QOS_TC_QDISC_NONE\n");
		break;
	case QOS_TC_QDISC_PRIO:
		seq_puts(file, "\ttype: QOS_TC_QDISC_PRIO\n");
		break;
	case QOS_TC_QDISC_DRR:
		seq_puts(file, "\ttype: QOS_TC_QDISC_DRR\n");
		break;
	case QOS_TC_QDISC_TBF:
		seq_puts(file, "\ttype: QOS_TC_QDISC_TBF\n");
		break;
	case QOS_TC_QDISC_RED:
		seq_puts(file, "\ttype: QOS_TC_QDISC_RED\n");
		break;
	default:
		seq_puts(file, "\ttype: unknown\n");
		break;
	}

	seq_printf(file, "\tparent: %#x\n", p.parent);
	seq_printf(file, "\thandle: %#x\n", p.handle);
	seq_printf(file, "\tuse_cnt: %d\n", p.use_cnt);

	seq_printf(file, "\tsch_id: %u\n", p.sch_id);
	seq_printf(file, "\tinst: %d\n", p.inst);
	seq_printf(file, "\tport: %d\n", p.port);
	seq_printf(file, "\tdeq_idx: %d\n", p.deq_idx);
	seq_printf(file, "\tepn: %d\n", p.epn);
	seq_printf(file, "\tdef_q: %u\n", p.def_q);
	seq_printf(file, "\tds: %s\n", p.ds ? "true" : "false");
}

static void print_qos_tc_port(struct seq_file *file,
			      struct qos_tc_port *p)
{
	seq_printf(file, "dev: %s\n", p->dev->name);

	seq_printf(file, "sch_num: %d\n", p->sch_num);

	print_qos_tc_qdisc(file, p->root_qdisc);

	seq_puts(file, "\n");
}

void qos_tc_port_list_debugfs(struct seq_file *file, void *ctx)
{
	struct qos_tc_port *p, *n;

	rtnl_lock();
	list_for_each_entry_safe(p, n, &port_list, list) {
		print_qos_tc_port(file, p);
	}
	rtnl_unlock();
}
