// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2021 - 2025 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/
#include <net/pkt_cls.h>
#include <net/datapath_api.h>
#include <net/datapath_api_qos.h>
#include <net/qos_tc.h>
#include <linux/version.h>
#include "qos_tc_flower.h"
#include "qos_tc_qos.h"
#include "qos_tc_tbf.h"
#include "qos_tc_trace.h"

#define MAX_QUANTUM (255 * 4096)
#define KB 1024
#define MIN_WEIGHT 1
#define MAX_WEIGHT 255

static int qos_tc_scale_quantum(u32 q_quantum, u32 sch_quanta)
{
	u32 weight;

	if (q_quantum < (KB * sch_quanta))
		weight = MIN_WEIGHT;
	else if (q_quantum > MAX_QUANTUM)
		weight = MAX_WEIGHT;
	else
		weight = q_quantum / (KB * sch_quanta);

	return weight;
}

/* Two cases of adding qdisc:
 * 1. qdisc is root
 * 2. qdisc is child
 */
static int qos_tc_drr_add_child_qdisc(struct net_device *dev,
				      struct qos_tc_port *port,
				      struct tc_drr_qopt_offload *p,
				      const struct qos_tc_params *tc_params)
{
	enum qos_tc_qdisc_type type = QOS_TC_QDISC_DRR;

	if (!dev || !port || !p)
		return -EINVAL;

	return qos_tc_add_child_qdisc(dev, port, type, p->parent, p->handle,
			tc_params);
}

static int qos_tc_drr_sched_update(struct net_device *dev,
				   struct qos_tc_port *port,
				   struct tc_drr_qopt_offload *parms,
				   const struct qos_tc_params *tc_params)
{
	enum qos_tc_qdisc_type type = QOS_TC_QDISC_DRR;
	struct qos_tc_qdata_params *qp = NULL;
	int ret;

	if (!dev || !port || !parms)
		return -EINVAL;

	if (port->root_qdisc.use_cnt && parms->parent == TC_H_ROOT)
		return 0;

	if (qos_tc_get_qp(port, QOS_TC_QDATA_GREEN, parms->parent) ||
	    qos_tc_get_qp(port, QOS_TC_QDATA_YELLOW, parms->parent) ||
	    qos_tc_get_qp(port, QOS_TC_QDATA_CODEL, parms->parent)) {
		netdev_err(dev, "%s: offload not supported\n", __func__);
		return -EOPNOTSUPP;
	}

	qp = qos_tc_get_qp(port, QOS_TC_QDATA_TBF, parms->parent);
	if (IS_ERR(qp))
		return PTR_ERR(qp);

	/* Queue has tbf shaper configured */
	if (qp)
		return qos_tc_q_tbf_sch(port, type, qp->parent, parms->handle);

	if (qos_tc_tbf_port_sch_on(port, parms->parent))
		return qos_tc_add_sch_to_tbf_port(port, type, parms->parent,
						  parms->handle);

	ret = qos_tc_drr_add_child_qdisc(dev, port, parms, tc_params);
	if (ret < 0) {
		netdev_err(dev, "drr child qdisc create failed\n");
		return ret;
	}

	port->sch_num++;

	return 0;
}

static int qos_tc_drr_queue_update(struct net_device *dev,
				   struct qos_tc_qdisc *sch,
				   struct tc_drr_qopt_offload *parms,
				   const struct qos_tc_params *tc_params)
{
	u32 handle = parms->handle;
	int idx = TC_H_MIN(handle) - 1;
	int weight, quantum;

	if (idx < 0 || idx > QOS_TC_MAX_Q - 1) {
		netdev_err(dev, "invalid child minor -> should be in [1-8]\n");
		return -EINVAL;
	}

	if (!sch->use_cnt || TC_H_MAJ(handle) != sch->handle) {
		netdev_err(dev, "invalid sched configuration\n");
		return -EINVAL;
	}

	if (!parms->set_params.quantum) {
		netdev_err(dev, "invalid quantum\n");
		return -EINVAL;
	}

	quantum = parms->set_params.quantum;
	weight = qos_tc_scale_quantum(quantum, 4);
	/* Range provided from OMCI via quantum must be adapted to
	 * hardware range.
	 * PPv4 has range 1..126
	 * OMCI has range 0..255
	 * Calculation for the adaptation:
	 * ppv4_w= round_up ((126-1)/(255-0)* omci_w + 1 )
	 * Normally even values are used, then this will produce
	 * better accuracy in lower range:
	 * ppv4_w= round_up ((126-1)/(255-0)* omci_w + 0 )
	 */
	weight = (125 * weight + 254) / 255;

	return qos_tc_queue_add(sch, QOS_TC_QDISC_DRR, weight, idx, tc_params);
}

static int qos_tc_drr_replace(struct net_device *dev,
			      struct tc_drr_qopt_offload *opt,
			      const struct qos_tc_params *tc_params)
{
	struct qos_tc_port *port = NULL;
	struct qos_tc_qdisc *qdisc = NULL;
	int ret = 0;
	bool newp = false;

	port = qos_tc_port_get(dev);
	if (!port) {
		port = qos_tc_port_alloc(dev);
		if (!port) {
			netdev_err(dev, "tc-drr port alloc failed\n");
			return -ENOMEM;
		}
		newp = true;
	}

	if (!TC_H_MIN(opt->handle)) {
		netdev_dbg(dev, "%s: Config drr sched\n", __func__);
		port->root_qdisc.port = tc_params->port_id;
		port->root_qdisc.deq_idx = tc_params->deq_idx;
		port->root_qdisc.dev = dev;
		ret = qos_tc_get_port_info(&port->root_qdisc, tc_params);
		if (ret < 0) {
			netdev_err(dev, "tc-drr port info get failed\n");
			goto err_free_port;
		}
		ret = qos_tc_drr_sched_update(dev, port, opt, tc_params);
		if (ret < 0) {
			netdev_err(dev, "tc-drr sched config failed\n");
			goto err_free_port;
		}
	}

	/* This is a drr class so queue has to be configured on
	 * an existing port.
	 */
	if (TC_H_MIN(opt->handle) && !newp) {
		netdev_dbg(dev, "%s: Config queue\n", __func__);
		qdisc = qos_tc_qdisc_find(port, opt->handle);
		if (!qdisc) {
			netdev_err(dev, "%s: radix tree broken\n", __func__);
			return -ENODEV;
		}
		ret = qos_tc_drr_queue_update(dev, qdisc, opt, tc_params);
		if (ret < 0) {
			netdev_err(dev, "%s: drr queue config err\n", __func__);
			return ret;
		}
	} else if (TC_H_MIN(opt->handle) && newp) {
		/* this is expected never to happen */
		netdev_err(dev, "%s: drr class without port\n", __func__);
		goto err_free_port;
	}

	qos_tc_sched_status(port, &port->root_qdisc);

	return 0;

err_free_port:
	if (newp)
		qos_tc_port_delete(port);
	return ret;
}

static int qos_tc_drr_queue_del(struct qos_tc_qdisc *sch, u32 handle,
		const struct qos_tc_params *tc_params)
{
	int idx = TC_H_MIN(handle) - 1;
	int ret;

	/* not a queue */
	if (!TC_H_MIN(handle))
		return 0;

	if (idx < 0 && idx >= QOS_TC_MAX_Q)
		return -EINVAL;

	/* check if this is a queue */
	if (sch->qids[idx].qid) {
		netdev_dbg(sch->dev, "%s: qid:%d deleting\n",
			   __func__, sch->qids[idx].qid);
		ret = qos_tc_queue_del(sch, idx, tc_params);
		if (ret < 0) {
			netdev_err(sch->dev, "%s: queue del err\n", __func__);
			return ret;
		}
		return 1;
	}

	return 0;
}

static int qos_tc_drr_qdisc_del(struct qos_tc_port *port,
				struct qos_tc_qdisc *sch,
				const struct qos_tc_params *tc_params)
{
	int ret;

	if (!port || !sch)
		return -EINVAL;

	netdev_dbg(sch->dev, "%s:free sch:%d parent:%#x %#x children:%d/%d\n",
		   __func__,
		   sch->sch_id, sch->parent, sch->handle,
		   sch->num_q, sch->num_children);

	if (TC_H_MIN(sch->handle) || sch->num_q || sch->num_children) {
		netdev_dbg(sch->dev, "%s: empty input or busy sched\n",
			   __func__);
		return 0;
	}

	if (sch->use_cnt) {
		ret = qos_tc_sched_del(sch, tc_params);
		if (ret < 0) {
			netdev_err(sch->dev, "%s: sch:%d del failed\n",
				   __func__, sch->sch_id);
			return ret;
		}
		port->sch_num--;
		if (sch->parent != TC_H_ROOT && TC_H_MIN(sch->parent)) {
			ret = qos_tc_qdisc_unlink(port, sch, tc_params);
			if (ret < 0)
				netdev_err(sch->dev, "%s:sch:%d unlink fail\n",
					   __func__, sch->sch_id);
		}
	}

	WARN_ON(radix_tree_delete(&port->qdiscs, TC_H_MAJ(sch->handle)) != sch);
	if (sch->parent != TC_H_ROOT)
		qos_tc_free_qdisc(sch);
	else
		qos_tc_port_delete(port);

	return 0;
}

static int qos_tc_drr_destroy(struct net_device *dev,
			      struct tc_drr_qopt_offload *opt,
			      const struct qos_tc_params *tc_params)
{
	struct qos_tc_port *port = NULL;
	struct qos_tc_qdisc *sch = NULL;
	int ret;

	port = qos_tc_port_get(dev);
	if (!port) {
		netdev_dbg(dev, "%s:port not found or deallocated\n", __func__);
		return 0;
	}

	sch = qos_tc_qdisc_find(port, opt->handle);
	if (!sch) {
		netdev_dbg(dev, "%s:sch not found or deallocated\n", __func__);
		return 0;
	}

	ret = qos_tc_drr_queue_del(sch, opt->handle, tc_params);
	if (ret < 0)
		return ret;

	/* this is a queue no need to do anything further */
	if (ret == 1)
		return 0;

	qos_tc_sched_status(port, &port->root_qdisc);

	ret = qos_tc_drr_qdisc_del(port, sch, tc_params);
	if (ret < 0) {
		netdev_err(dev, "%s: %#x id: %d failed\n", __func__,
			   sch->handle, sch->sch_id);
		return ret;
	}

	return 0;
}

int qos_tc_drr_offload(struct net_device *dev,
		       void *type_data,
		       const struct qos_tc_params *tc_params)
{
	int err = 0;
	struct tc_drr_qopt_offload *opt = type_data;

	ASSERT_RTNL();
	netdev_dbg(dev, "DRR: offload starting\n");

	trace_qos_tc_drr_enter(dev, opt);
	switch (opt->command) {
	case TC_DRR_REPLACE:
		netdev_dbg(dev, "pid: %#x class/handle: %#x quantum: %#x\n",
			   opt->parent, opt->handle, opt->set_params.quantum);
		err = qos_tc_drr_replace(dev, opt, tc_params);
		if (err < 0) {
			netdev_err(dev, "tc-drr replace failed\n");
			return err;
		}
		break;
	case TC_DRR_DESTROY:
		netdev_dbg(dev,
			   "destroy pid: %#x class/handle: %#x quantum: %#x\n",
			   opt->parent, opt->handle, opt->set_params.quantum);
		err = qos_tc_drr_destroy(dev, opt, tc_params);
		if (err < 0) {
			netdev_err(dev, "tc-drr destroy failed\n");
			return err;
		}

		break;
	case TC_DRR_STATS:
		/* TODO */
		return -EOPNOTSUPP;
	case TC_DRR_GRAFT:
		/* TODO */
		return -EOPNOTSUPP;
	default:
		break;
	}
	trace_qos_tc_drr_exit(dev, opt);

	netdev_dbg(dev, "DRR: offload end: %d\n", err);
	return err;
}
