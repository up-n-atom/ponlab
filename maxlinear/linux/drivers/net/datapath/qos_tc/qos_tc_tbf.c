// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2020 - 2025 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/
#include <net/pkt_cls.h>
#include <linux/version.h>
#include <net/datapath_api.h>
#include <net/datapath_api_qos.h>
#include <net/qos_tc.h>

#include "qos_tc_main.h"
#include "qos_tc_flower.h"
#include "qos_tc_qos.h"
#include "qos_tc_tbf.h"
#include "qos_tc_trace.h"

static struct qos_tc_tbf_data get_tbf_info(struct tc_tbf_qopt_offload *opt)
{
	struct tc_tbf_qopt_offload_replace_params *p = &opt->replace_params;
	struct qos_tc_tbf_data tbf;

	tbf.cir = div_u64(p->rate.rate_bytes_ps * 8, 1000);
	tbf.pir = div_u64(p->peak.rate_bytes_ps * 8, 1000);

	tbf.cbs = psched_ns_t2l(&p->rate, p->buffer);
	tbf.pbs = psched_ns_t2l(&p->peak, p->mtu);

	tbf.parent = opt->parent;
	tbf.handle = opt->handle;

	return tbf;
}

/* Function is mainly for debugging purpsoses for now */
static int del_tbf_queue_from_root(struct qos_tc_port *port)
{
	struct qos_tc_qdisc *sch = &port->root_qdisc;

	netdev_dbg(port->dev, "%s: deleting tbf root port: %d\n",
		   __func__,  sch->sch_id);

	return 0;
}

static int add_tbf_queue_to_root(struct qos_tc_port *port, u32 handle)
{
	struct qos_tc_qdisc *sch = &port->root_qdisc;
	struct net_device *dev = sch->dev;
	struct qos_tc_params dummy = {0};
	int ret;

	if (sch->use_cnt)
		return 0;

	ret = qos_tc_queue_add(sch, QOS_TC_QDISC_PRIO, 0, 0, &dummy);
	if (ret) {
		netdev_err(dev, "%s: tc-tbf queue add failed\n", __func__);
		return -EIO;
	}

	sch->num_q++;

	/* add to radix tree here */
	ret = radix_tree_insert(&port->qdiscs, TC_H_MAJ(handle), sch);
	if (ret) {
		netdev_err(dev, "qdisc insert to radix tree failed: %d\n", ret);
		return ret;
	}

	port->destroy = del_tbf_queue_from_root;
	sch->use_cnt = 1;

	return 0;
}

static struct qos_tc_qdata_params *get_qdata(struct qos_tc_qdisc *qdisc,
					     enum qos_tc_qdata_type type,
					     u32 parent)
{
	struct qos_tc_q_data *qid = NULL;
	struct qos_tc_qdata_params *p = {0};
	u32 handle = TC_H_MAJ(parent);

	qid = qos_tc_qdata_qid_get(qdisc->dev, qdisc, parent);
	if (!qid)
		return NULL;

	list_for_each_entry(p, &qid->params, list) {
		if (p->handle == handle && p->type == type)
			return p;
	}

	return NULL;
}

bool qos_tc_tbf_port_sch_on(struct qos_tc_port *port, u32 parent)
{
	struct qos_tc_qdisc *qdisc = NULL;

	qdisc = qos_tc_qdisc_find(port, parent);
	if (!qdisc)
		return false;

	if (qdisc->parent == TC_H_ROOT && port->tbf_on)
		return true;

	return false;
}

struct qos_tc_qdata_params *qos_tc_get_qp(struct qos_tc_port *port,
					  enum qos_tc_qdata_type type,
					  u32 parent)
{
	struct qos_tc_qdisc *qdisc = NULL;

	if (parent == TC_H_ROOT)
		return NULL;

	qdisc = qos_tc_qdisc_find(port, parent);
	if (!qdisc) {
		netdev_err(port->dev, "%s: qdisc not found\n", __func__);
		return ERR_PTR(-ENODEV);
	}

	if (qdisc->handle != TC_H_MAJ(parent))
		return get_qdata(qdisc, type, parent);

	return NULL;
}

static int sched_tbf_add(struct qos_tc_qdisc *sch, struct qos_tc_tbf_data *tbf)
{
	struct dp_shaper_conf cfg = {0};
	int ret = 0;

	cfg.inst = sch->inst;
	cfg.type = DP_NODE_SCH;
	cfg.cmd = DP_SHAPER_CMD_ADD;
	cfg.id.sch_id = (int)sch->sch_id;

	/* Only single rate shapers supported */
	cfg.cir = tbf->cir;
	cfg.cbs = tbf->cbs;

	netdev_dbg(sch->dev, "%s: sch_id: %u cir: %d pir: %d cbs: %d pbs: %d\n",
		   __func__, sch->sch_id, cfg.cir, cfg.pir, cfg.cbs, cfg.pbs);

	ret = dp_shaper_conf_set(&cfg, 0);
	if (ret == DP_FAILURE) {
		netdev_err(sch->dev, "%s: shaper config set failed %d\n",
			   __func__, ret);
		return -EINVAL;
	}

	sch->tbf_on = true;

	return ret;
}

static int move_tbf_to_sched(struct qos_tc_port *port,
			     u32 parent, u32 child)
{
	struct qos_tc_qdisc *q = NULL;
	struct qos_tc_tbf_data *tbf;
	int idx = TC_H_MIN(parent) - 1;

	/* get parent scheduler */
	q = qos_tc_qdisc_find(port, parent);
	WARN(!q, "%s: qdisc not found but one should exist", __func__);

	tbf = &q->tbfs[idx];

	/* get child scheduler (currently created) */
	q = qos_tc_qdisc_find(port, child);
	WARN(!q, "%s: child qdisc not found but one should exist", __func__);

	return sched_tbf_add(q, tbf);
}

/* parent is the scheduler of the queue being replaced.
 * handle is the new scheduler replacing the queue
 */
int qos_tc_tbf_sch(struct qos_tc_port *port, enum qos_tc_qdisc_type type,
		   u32 parent, u32 handle)
{
	struct net_device *dev = port->dev;
	int ret;

	ret = qos_tc_add_tbf_child_qdisc(port, type, parent, handle);
	if (ret) {
		netdev_err(dev, "%s: add tbf child qdisc failed\n", __func__);
		return ret;
	}

	return 0;
}

int qos_tc_q_tbf_sch(struct qos_tc_port *port, enum qos_tc_qdisc_type type,
		     u32 parent, u32 handle)
{
	struct net_device *dev = port->dev;
	int ret;

	ret = qos_tc_tbf_sch(port, type, parent, handle);
	if (ret) {
		netdev_err(dev, "%s: add tbf child qdisc failed\n", __func__);
		return ret;
	}

	return move_tbf_to_sched(port, parent, handle);
}

static int root_tbf_remove(struct net_device *dev, u32 handle, u32 parent)
{
	struct qos_tc_port *port = NULL;
	struct qos_tc_qdisc *qdisc_sch = NULL;
	struct dp_shaper_conf shaper_conf = {0};
	struct qos_tc_params tc_params = {0};
	int ret;

	port = qos_tc_port_get(dev);
	if (!port) {
		netdev_err(dev, "%s: no port found\n", __func__);
		return 0;
	}

	qdisc_sch = qos_tc_qdisc_find(port, handle);
	if (!qdisc_sch) {
		netdev_err(dev, "%s: qdisc doesn't exist\n", __func__);
		return 0;
	}

	// remove port shaper via datapath api
	shaper_conf.cmd = DP_SHAPER_CMD_REMOVE;
	shaper_conf.type = DP_NODE_PORT;
	shaper_conf.id.cqm_deq_port = port->root_qdisc.epn;
	ret = dp_shaper_conf_set(&shaper_conf, 0);
	if (ret) {
		netdev_err(dev, "%s:%d Failed to remove port Shaper: %d\n",
			   __func__, __LINE__, ret);
		return ret;
	}

	ret = qos_tc_qdisc_tree_del(port, qdisc_sch, &tc_params);
	if (ret) {
		netdev_err(dev, "%s:%d Failed to delete root tbf: %d\n",
			   __func__, __LINE__, ret);
		return ret;
	}

	return ret;
}

static int setup_port_shaper(struct qos_tc_port *port, u32 handle)
{
	struct qos_tc_qdisc *sch = &port->root_qdisc;
	struct qos_tc_tbf_data *tbf = &port->tbf;
	struct net_device *dev = port->dev;
	struct dp_shaper_conf shaper_conf = {0};
	int ret;

	sch->parent = TC_H_ROOT;
	sch->handle = handle;
	sch->dev = port->dev;

	ret = qos_tc_fill_port_data(sch, NULL);
	if (ret < 0) {
		netdev_err(dev, "%s: fill port data err=%d\n",  __func__, ret);
		return ret;
	}
	/* re-usr port sched */
	sch->sch_id = sch->epn;

	shaper_conf.cir = tbf->cir;
	shaper_conf.cbs  = tbf->cbs;

	shaper_conf.cmd = DP_SHAPER_CMD_ADD;
	shaper_conf.type = DP_NODE_PORT;
	shaper_conf.id.cqm_deq_port = port->root_qdisc.epn;

	ret = dp_shaper_conf_set(&shaper_conf, 0);
	if (ret) {
		netdev_err(dev, "%s:%d Failed to set the Shaper: %d\n",
			   __func__, __LINE__, ret);
		return ret;
	}

	port->tbf_on = true;

	return ret;
}

static struct qos_tc_port *replace_port(struct qos_tc_port *port,
					struct net_device *dev)
{
	int ret;
	const struct qos_tc_params tc_params = {0};
	struct qos_tc_port *new_port;

	ret = qos_tc_qdisc_tree_del(port, &port->root_qdisc, &tc_params);
	if (ret) {
		netdev_err(dev, "%s: root qdisc reset failed\n", __func__);
		return ERR_PTR(ret);
	}

	new_port = qos_tc_port_alloc(dev);
	if (!new_port) {
		netdev_err(dev, "%s: root port alloc failed\n", __func__);
		return ERR_PTR(-ENOMEM);
	}

	// Needed for new allocated port
	new_port->root_qdisc.deq_idx = -1;
	new_port->root_qdisc.dev = dev;
	return new_port;
}

static int setup_port_tbf(struct qos_tc_port *port, u32 handle)
{
	const struct qos_tc_params tc_params = {0};
	struct net_device *dev = port->dev;
	int ret;

	ret = qos_tc_get_port_info(&port->root_qdisc, &tc_params);
	if (ret) {
		netdev_err(dev, "%s: get port info failed\n", __func__);
		return ret;
	}

	ret = setup_port_shaper(port, handle);
	if (ret) {
		netdev_err(dev, "%s: tc-tbf port shaping failed\n", __func__);
		goto err_free_qdisc;
	}

	ret = add_tbf_queue_to_root(port, handle);
	if (ret) {
		netdev_err(dev, "%s: tbf queue add failed\n", __func__);
		goto err_free_qdisc;
	}

	return 0;

err_free_qdisc:
	ret = qos_tc_sched_del(&port->root_qdisc, &tc_params);
	if (ret < 0)
		netdev_err(dev, "%s: sched del failed\n", __func__);

	return ret;
}

static int tbf_setup_new_port(struct net_device *dev,
			      struct qos_tc_tbf_data *tbf, u32 handle)
{
	struct qos_tc_port *port;
	int ret = 0;

	port = qos_tc_port_alloc(dev);
	if (!port) {
		netdev_err(dev, "%s: port alloc failed\n", __func__);
		return -ENOMEM;
	}
	// Needed for new allocated port
	port->root_qdisc.deq_idx = -1;
	port->root_qdisc.dev = dev;

	port->tbf = *tbf;

	ret = setup_port_tbf(port, handle);
	if (ret) {
		netdev_err(dev, "%s: setup_port_tbf failed\n", __func__);
		qos_tc_port_delete(port);
		return ret;
	}

	return 0;
}

static int tbf_replace_alloc_port(struct qos_tc_port *port,
				  struct qos_tc_tbf_data *tbf,
				  u32 handle)
{
	struct net_device *dev = port->dev;
	int ret = 0;

	port = replace_port(port, dev);
	if (IS_ERR_OR_NULL(port)) {
		netdev_err(dev, "%s: port replace failed\n", __func__);
		return PTR_ERR(port);
	}

	port->tbf = *tbf;

	ret = setup_port_tbf(port, handle);
	if (ret) {
		netdev_err(dev, "%s: setup_port_tbf failed\n", __func__);
		qos_tc_port_delete(port);
		return ret;
	}

	return 0;
}

static int tbf_replace_current_port(struct qos_tc_port *port,
				    struct qos_tc_tbf_data *tbf,
				    u32 handle)
{
	port->tbf = *tbf;

	return setup_port_tbf(port, handle);
}

static int tbf_replace_port(struct qos_tc_port *port,
			    struct qos_tc_tbf_data *tbf,
			    u32 handle)
{
	struct qos_tc_qdisc *qdisc = NULL;

	/* If the qdisc handle isn't registered, the root qdisc should
	 * either be replaced or is complety new.
	 * In both cases, we can delete all children.
	 * This also eliminates the port struct, so we need a new one.
	 */
	qdisc = qos_tc_qdisc_find(port, handle);
	if (!qdisc)
		return tbf_replace_alloc_port(port, tbf, handle);
	else
		return tbf_replace_current_port(port, tbf, handle);
}

static int tbf_replace_root(struct net_device *dev,
			    struct tc_tbf_qopt_offload *opt)
{
	struct qos_tc_tbf_data tbf = get_tbf_info(opt);
	struct qos_tc_port *port;
	int ret = 0;

	port = qos_tc_port_get(dev);
	if (!port)
		ret = tbf_setup_new_port(dev, &tbf, opt->handle);
	else
		ret = tbf_replace_port(port, &tbf, opt->handle);

	return ret;
}

static struct qos_tc_q_data *tbf_get_q_data(struct qos_tc_qdisc *qdisc,
					    u32 parent)
{
	struct net_device *dev = qdisc->dev;
	struct qos_tc_q_data *qid = NULL;
	int ret;

	/* Check all scheduler input for the queue checking the q_data
	 * parameters list first. The check is needed as qdiscs as tc-tbf,
	 * tc-red condigured only queue parameters. The queues are created
	 * in the hardware by their parents qdiscs like tc-prio and tc-drr.
	 */
	qid = qos_tc_qdata_qid_get(dev, qdisc, parent);
	if (qid)
		return qid;

	/* Return the queue matching the qdisc child. The parent minor id
	 * defines the index in the qids array
	 */
	ret = qos_tc_get_queue_by_handle(dev, parent, &qid);
	if (ret < 0 || !qid) {
		netdev_err(dev, "%s: queue parent id %#x not found\n",
			   __func__, parent);
		return ERR_PTR(-EINVAL);
	}

	return qid;
}

static int tbf_q_shaper_exists(struct net_device *dev,
			       struct tc_tbf_qopt_offload *opt)
{
	struct qos_tc_qdata_params *qp = NULL;
	struct qos_tc_port *port = qos_tc_port_get(dev);

	if (!port)
		return -ENODEV;

	qp = qos_tc_get_qp(port, QOS_TC_QDATA_TBF, opt->parent);
	if (IS_ERR(qp))
		return -EINVAL;

	if (qp)
		return true;

	return false;
}

static int tbf_replace_qid(struct net_device *dev,
			   struct tc_tbf_qopt_offload *opt)
{
	struct qos_tc_tbf_data tbf = get_tbf_info(opt);
	struct qos_tc_qdisc *qdisc = NULL;
	struct qos_tc_q_data *qid = NULL;
	int idx = TC_H_MIN(opt->parent) - 1;
	int ret;

	ret = qos_tc_get_sch_by_handle(dev, opt->parent, &qdisc);
	if (ret) {
		netdev_err(dev, "%s: no scheduler configured\n", __func__);
		return ret;
	}

	if (tbf_q_shaper_exists(dev, opt)) {
		netdev_err(dev, "%s: err or queue shaper exists\n", __func__);
		return -EOPNOTSUPP;
	}

	qid = tbf_get_q_data(qdisc, opt->parent);
	if (IS_ERR(qid))
		return PTR_ERR(qid);

	ret = qos_tc_shaper_add(qdisc, qid, &tbf);
	if (ret < 0)
		return ret;

	ret = qos_tc_qdata_add_tbf(dev, qid, &tbf, tbf_remove);
	if (ret < 0)
		return ret;

	qdisc->tbfs[idx] = tbf;

	return qos_tc_add_qdisc_to_dev(dev, qdisc, opt->handle);
}

static int tbf_replace(struct net_device *dev,
		       struct tc_tbf_qopt_offload *opt)
{
	int ret;

	if (!dev) {
		netdev_err(dev, "%s: no valid device.\n", __func__);
		return -EINVAL;
	}

	// Check if qdisc is a root qdisc
	if (opt->parent == TC_H_ROOT)
		return tbf_replace_root(dev, opt);

	ret = tbf_replace_qid(dev, opt);
	if (ret) {
		netdev_err(dev, "%s: no queue find\n", __func__);
		return -EINVAL;
	}

	return 0;
}

static int tbf_queue_remove(struct qos_tc_qdisc *qdisc, u32 parent, u32 handle)
{
	struct net_device *dev = qdisc->dev;
	struct qos_tc_q_data *qid = NULL;
	int ret = 0;

	qid = tbf_get_q_data(qdisc, parent);
	if (IS_ERR_OR_NULL(qid)) {
		netdev_err(dev, "%s: no queue data found\n", __func__);
		return -ENOKEY;
	}

	ret = qos_tc_qdata_remove(dev, qid, handle, parent);
	if (ret) {
		netdev_err(dev, "%s: qdata remove failed\n", __func__);
		return -EIO;
	}

	ret = qos_tc_del_tbf_from_qdisc(qdisc, qid);
	if (ret < 0)
		return ret;

	return 0;
}

static int tbf_queue_destroy(struct net_device *dev,
			     struct tc_tbf_qopt_offload *opt)
{
	struct qos_tc_qdisc *qdisc = NULL;
	struct qos_tc_port *port = NULL;

	port = qos_tc_port_get(dev);
	if (!port) {
		/* Linux deletes some qdiscs from root to the leaf node
		 * which may cause that the tbf offload is already removed
		 * by its parent i.e. deleted by qos_tc_qdisc_tree_del.
		 */
		netdev_dbg(dev, "%s: port get failed\n", __func__);
		return -ENODEV;
	}

	qdisc = qos_tc_qdisc_find(port, opt->parent);
	if (!qdisc) {
		netdev_err(dev, "%s: qdisc get failed\n", __func__);
		return -ENODEV;
	}

	return tbf_queue_remove(qdisc, opt->parent, opt->handle);
}

static int tbf_destroy(struct net_device *dev,
		       struct tc_tbf_qopt_offload *opt)
{
	if (opt->parent == TC_H_ROOT)
		return root_tbf_remove(dev, opt->handle, opt->parent);

	return tbf_queue_destroy(dev, opt);
}

int qos_tc_tbf_offload(struct net_device *dev,
		       void *type_data)
{
	int err = 0;
	struct tc_tbf_qopt_offload *opt = type_data;

	trace_qos_tc_tbf_enter(dev, opt);

	switch (opt->command) {
	case TC_TBF_REPLACE:
		err = tbf_replace(dev, opt);
		if (err < 0) {
			netdev_err(dev, "%s: replace failed\n", __func__);
			return err;
		}
		break;
	case TC_TBF_DESTROY:
		err = tbf_destroy(dev, opt);
		if (err < 0) {
			/* Expected to fail if root was previously removed */
			netdev_dbg(dev, "%s: destroy failed\n", __func__);
			return err;
		}
		break;
	case TC_TBF_STATS:
		return -EOPNOTSUPP;
	default:
		break;
	}

	trace_qos_tc_tbf_exit(dev, opt);

	return 0;
}
