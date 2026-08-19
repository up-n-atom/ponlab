// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2024 MaxLinear, Inc.
 *
 *****************************************************************************/
#include <net/pkt_cls.h>
#include <net/datapath_api.h>
#include "qos_tc_trace.h"

#define CODEL_DFLT_TARGET_DELAY_MSEC	(5)
#define CODEL_DFLT_INTERVAL_TIME_MSEC	(100)
/* codel limit is set to 1000 by default in sch_codel.c */
#define DEFAULT_CODEL_LIMIT	(1000)
#define QOS_HAL_QWRED_MIN_GUARANTEED	(64)
#define QOS_HAL_QWRED_MAX_ALLOWED	(3072)

static int qos_tc_codel_remove(struct net_device *dev, u32 handle, u32 parent)
{
	struct qos_tc_qdisc *qdisc = NULL;
	struct qos_tc_q_data *qdata = NULL;
	struct qos_tc_port *port = NULL;
	struct dp_queue_conf q_cfg = {0};
	int ret, idx;

	port = qos_tc_port_get(dev);
	if (!port) {
		netdev_err(dev, "%s: Port get failed\n", __func__);
		return -ENODEV;
	}

	qdisc = qos_tc_qdisc_find(port, parent);
	if (!qdisc) {
		netdev_err(dev, "%s: qdisc get failed\n", __func__);
		return -ENODEV;
	}

	qdata = qos_tc_qdata_qid_get(dev, qdisc, parent);
	if (!qdata) {
		ret = qos_tc_get_queue_by_handle(dev, parent, &qdata);
		if (ret < 0 || !qdata) {
			netdev_err(dev, "%s: handle not found err %d\n", __func__, ret);
			return -EINVAL;
		}
	}

	/* Disable codel for the given queue */
	q_cfg.inst = 0;
	q_cfg.q_id = qdata->qid;
	q_cfg.codel = DP_CODEL_DIS;
	if (dp_queue_conf_set(&q_cfg, 0) != DP_SUCCESS) {
		netdev_err(dev, "%s: failed to disable codel\n", __func__);
		return -EINVAL;
	}

	/* Restore the default wred configuration for the specified queue */
	idx = TC_H_MIN(parent) - 1;
	ret = qos_tc_queue_wred_defaults_set(qdisc, idx);
	if (ret < 0) {
		netdev_err(dev, "%s:wred defaults set fail\n", __func__);
		return -EINVAL;
	}
	radix_tree_delete(&port->qdiscs, TC_H_MAJ(handle));

	return 0;
}

int qos_tc_codel_add(struct qos_tc_qdisc *sch, struct qos_tc_q_data *qdata,
		struct tc_codel_qopt_offload *opt)
{
	struct dp_queue_conf q_cfg = {0};

	/* Enable codel for the specified queue */
	q_cfg.inst = sch->inst;
	q_cfg.q_id = qdata->qid;
	q_cfg.codel = DP_CODEL_EN;
	q_cfg.act = BIT(1);
	q_cfg.drop = DP_QUEUE_DROP_CODEL;
	q_cfg.min_size[0] = 0;
	q_cfg.min_size[1] = 0;
	q_cfg.wred_min_guaranteed = QOS_HAL_QWRED_MIN_GUARANTEED;
	q_cfg.wred_max_allowed = (opt->cparams.limit != DEFAULT_CODEL_LIMIT) ? opt->cparams.limit :
		QOS_HAL_QWRED_MAX_ALLOWED;
	if (dp_queue_conf_set(&q_cfg, 1) != DP_SUCCESS) {
		netdev_err(sch->dev, "%s: dp_queue_conf_set failed\n", __func__);
		return -EINVAL;
	}

	return 0;
}

static int qos_tc_codel_replace(struct net_device *dev,
		struct tc_codel_qopt_offload *opt)
{
	struct qos_tc_qdisc *qdisc = NULL;
	struct qos_tc_q_data *qid = NULL;
	struct qos_tc_port *port = NULL;
	int ret;

	port = qos_tc_port_get(dev);
	if (!port) {
		netdev_err(dev, "%s: tc-codel port get failed\n", __func__);
		return -ENODEV;
	}

	qdisc = qos_tc_qdisc_find(port, opt->parent);
	if (!qdisc) {
		netdev_err(dev, "%s: tc-codel qdisc get failed\n", __func__);
		return -ENODEV;
	}

	qid = qos_tc_qdata_qid_get(dev, qdisc, opt->parent);
	if (!qid) {
		ret = qos_tc_get_queue_by_handle(dev, opt->parent, &qid);
		if (ret < 0 || !qid) {
			netdev_err(dev, "%s: failed to get qid\n", __func__);
			return -EINVAL;
		}
	}

	ret = qos_tc_codel_add(qdisc, qid, opt);
	if (ret < 0)
		return ret;

	ret = qos_tc_qdata_add(dev, qid, opt->handle, opt->parent,
			       QOS_TC_QDATA_CODEL, qos_tc_codel_remove);
	if (ret < 0)
		return ret;

	ret = radix_tree_insert(&port->qdiscs, TC_H_MAJ(opt->handle), qdisc);
	if (ret < 0) {
		netdev_err(dev, "%s: qdisc insertion to radix tree failed\n", __func__);
		return ret;
	}

	return 0;
}

static int qos_tc_codel_destroy(struct net_device *dev,
		struct tc_codel_qopt_offload *opt)
{
	struct qos_tc_qdisc *qdisc = NULL;
	struct qos_tc_q_data *qid = NULL;
	struct qos_tc_port *port = NULL;
	int ret;

	port = qos_tc_port_get(dev);
	if (!port) {
		netdev_dbg(dev, "%s: port get failed\n", __func__);
		return -ENODEV;
	}

	qdisc = qos_tc_qdisc_find(port, opt->parent);
	if (!qdisc) {
		netdev_err(dev, "%s: qdisc get failed\n", __func__);
		return -ENODEV;
	}

	qid = qos_tc_qdata_qid_get(dev, qdisc, opt->parent);
	if (!qid) {
		ret = qos_tc_get_queue_by_handle(dev, opt->parent, &qid);
		if (ret < 0 || !qid) {
			netdev_err(dev, "%s: handle not found\n", __func__);
			return -EINVAL;
		}
	}
	return qos_tc_qdata_remove(dev, qid, opt->handle, opt->parent);
}

int qos_tc_codel_offload(struct net_device *dev,
		void *type_data)
{
	int ret = 0;
	struct tc_codel_qopt_offload *opt = type_data;

	/* CoDel is not supported on PON vUNI due to FSQM buffer issues */
	if (qos_tc_is_vuni_dev(dev)) {
		netdev_err(dev, "CoDel is not supported on PON vUNI interface\n");
		return -EOPNOTSUPP;
	}

	switch (opt->command) {
	case TC_CODEL_REPLACE:
		ret = qos_tc_codel_replace(dev, opt);
		if (ret < 0) {
			netdev_err(dev, "tc-codel replace failed\n");
			return ret;
		}
		break;
	case TC_CODEL_DESTROY:
		ret = qos_tc_codel_destroy(dev, opt);
		if (ret < 0) {
			netdev_dbg(dev, "tc-codel destroy failed\n");
			return ret;
		}
		break;
	case TC_CODEL_STATS:
		return -EOPNOTSUPP;
	default:
		break;
	}

	return ret;
}
