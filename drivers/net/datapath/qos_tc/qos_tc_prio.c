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

static int qos_tc_sched_update(struct net_device *dev,
			       struct qos_tc_port *port,
			       struct tc_prio_qopt_offload *opt,
			       const struct qos_tc_params *tc_params)
{
	enum qos_tc_qdisc_type type = QOS_TC_QDISC_PRIO;
	struct qos_tc_qdata_params *qp = NULL;
	int ret;

	if (!dev || !port || !opt)
		return -EINVAL;

	if (qos_tc_get_qp(port, QOS_TC_QDATA_GREEN, opt->parent) ||
	    qos_tc_get_qp(port, QOS_TC_QDATA_YELLOW, opt->parent) ||
	    qos_tc_get_qp(port, QOS_TC_QDATA_CODEL, opt->parent)) {
		netdev_err(dev, "%s: offload not supported\n", __func__);
		return -EOPNOTSUPP;
	}

	qp = qos_tc_get_qp(port, QOS_TC_QDATA_TBF, opt->parent);
	if (IS_ERR(qp))
		return PTR_ERR(qp);

	/* Queue has tbf shaper configured */
	if (qp)
		return qos_tc_q_tbf_sch(port, type, qp->parent, opt->handle);

	if (qos_tc_tbf_port_sch_on(port, opt->parent))
		return qos_tc_add_sch_to_tbf_port(port, type, opt->parent,
						  opt->handle);

	ret = qos_tc_add_child_qdisc(dev, port, type, opt->parent, opt->handle,
				     tc_params);
	if (ret) {
		netdev_err(port->dev, "%s: add child qdisc failed\n", __func__);
		return ret;
	}

	return ret;
}

static int qos_tc_queues_update(struct net_device *dev,
				struct qos_tc_qdisc *sch,
				struct tc_prio_qopt_offload *parms,
				const struct qos_tc_params *tc_params)
{
	u8 *priomap = parms->replace_params.priomap;
	u8 bands = parms->replace_params.bands;
	int ret, i;

	if (!sch->use_cnt)
		return -EINVAL;

	if (bands == sch->prio.bands)
		/* TODO: nothing to do? */
		return 0;

	if (bands < sch->prio.bands) {
		for (i = 0; i < sch->prio.bands; i++) {
			ret = qos_tc_queue_del(sch, i, tc_params);
			if (ret < 0) {
				netdev_err(dev, "queue del failed\n");
				return ret;
			}
		}
	}

	sch->prio.bands = bands;
	memcpy(sch->prio.priomap, priomap,
	       sizeof(sch->prio.priomap));

	for (i = 0; i < bands; i++) {
		ret = qos_tc_queue_add(sch, QOS_TC_QDISC_PRIO, priomap[i], i,
				tc_params);
		if (ret < 0)
			netdev_err(dev, "queue add failed\n");
	}

	return 0;
}

static int qos_tc_prio_update_tree(struct qos_tc_qdisc *sch,
				   struct tc_prio_qopt_offload *p)
{
	/* tree update requires grafting so skip it for now */

	if (sch->num_children)
		return -EINVAL;

	return 0;
}

static int qos_tc_prio_replace(struct net_device *dev,
			       struct tc_prio_qopt_offload *opt,
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
			netdev_err(dev, "tc-prio port alloc failed\n");
			return -ENOMEM;
		}
		newp = true;
	}

	if (opt->parent == TC_H_ROOT) {
		port->root_qdisc.port = tc_params->port_id;
		port->root_qdisc.deq_idx = tc_params->deq_idx;
		port->root_qdisc.dev = dev;
		ret = qos_tc_get_port_info(&port->root_qdisc, tc_params);
		if (ret < 0)
			goto err_free_port;
	}

	qdisc = qos_tc_qdisc_find(port, opt->handle);
	if (!qdisc) {
		ret = qos_tc_sched_update(dev, port, opt, tc_params);
		if (ret < 0) {
			netdev_err(dev, "tc-prio sched config failed\n");
			goto err_free_port;
		}
	}

	qdisc = qos_tc_qdisc_find(port, opt->handle);
	if (!qdisc) {
		netdev_err(dev, "%s: radix tree broken\n", __func__);
		ret = -ENODEV;
		goto err_free_port;
	}

	ret = qos_tc_prio_update_tree(qdisc, opt);
	if (ret < 0) {
		ret = -EOPNOTSUPP;
		goto err_free_qdisc;
	}

	ret = qos_tc_queues_update(dev, qdisc, opt, tc_params);
	if (ret < 0) {
		netdev_err(dev, "tc-prio queues config failed\n");
		goto err_free_qdisc;
	}

	return 0;

err_free_qdisc:
	ret = qos_tc_sched_del(qdisc, tc_params);
	if (ret < 0)
		netdev_err(dev, "%s: sched del failed\n", __func__);
	if (qdisc != &port->root_qdisc)
		kfree(qdisc);
err_free_port:
	if (newp)
		qos_tc_port_delete(port);
	return ret;
}

static int qos_tc_prio_destroy(struct net_device *dev,
			       struct tc_prio_qopt_offload *opt,
			       const struct qos_tc_params *tc_params)
{
	struct qos_tc_port *port = NULL;
	struct qos_tc_qdisc *sch = NULL;

	port = qos_tc_port_get(dev);
	if (!port) {
		netdev_dbg(dev, "%s:port not found or deallocated\n", __func__);
		return 0;
	}

	sch = qos_tc_qdisc_find(port, opt->handle);
	if (!sch) {
		netdev_dbg(dev, "%s: sch not found or deallocated\n", __func__);
		return 0;
	}

	/*qos_tc_sched_status(port, &port->root_qdisc);*/
	qos_tc_qdisc_tree_del(port, sch, tc_params);

	return 0;
}

int qos_tc_prio_offload(struct net_device *dev,
			void *type_data,
			const struct qos_tc_params *tc_params)
{
	int err = 0;
	struct tc_prio_qopt_offload *opt = type_data;

	ASSERT_RTNL();
	netdev_dbg(dev, "PRIO: offload starting\n");

	trace_qos_tc_prio_enter(dev, opt);

	switch (opt->command) {
	case TC_PRIO_REPLACE:
		if (opt->replace_params.bands > QOS_TC_MAX_Q) {
			netdev_err(dev, "tc-prio - out of range bands %d\n",
				   opt->replace_params.bands);
			return -EINVAL;
		}
		netdev_dbg(dev, "replace pid:%#x class/handle:%#x bands:%#x\n",
			   opt->parent, opt->handle, opt->replace_params.bands);
		err = qos_tc_prio_replace(dev, opt, tc_params);
		if (err < 0) {
			netdev_err(dev, "tc-prio replace failed\n");
			return err;
		}
		break;
	case TC_PRIO_DESTROY:
		netdev_dbg(dev, "destroy pid:%#x class/handle:%#x\n",
			   opt->parent, opt->handle);
		err = qos_tc_prio_destroy(dev, opt, tc_params);
		if (err < 0) {
			netdev_err(dev, "tc-prio destroy failed\n");
			return err;
		}
		break;
	case TC_PRIO_STATS:
		/* TODO */
		netdev_dbg(dev, "stats pid:%#x class/handle:%#x\n",
			   opt->parent, opt->handle);
		return -EOPNOTSUPP;
	case TC_PRIO_GRAFT:
		/* TODO */
		netdev_dbg(dev, "graft pid:%#x class/handle:%#x band:%#x\n",
			   opt->parent, opt->handle, opt->graft_params.band);
		return -EOPNOTSUPP;
	default:
		break;
	}

	trace_qos_tc_prio_exit(dev, opt);

	netdev_dbg(dev, "PRIO: offload end: %d\n", err);
	return 0;
}
