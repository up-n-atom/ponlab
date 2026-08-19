// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2021 - 2024 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/
#include <linux/module.h>
#include <linux/version.h>
#include <net/pkt_cls.h>
#include <net/qos_tc.h>
#include <linux/jiffies.h>
#include "qos_tc_flower.h"
#include "qos_tc_main.h"
#include "qos_tc_qos.h"
#include "qos_tc_debugfs.h"
#include "qos_tc_trace.h"

#define DRV_NAME	"mod_qos_tc"

int qos_tc_setup(struct net_device *dev, enum tc_setup_type type,
		void *type_data, int port_id, int deq_idx)
{
	struct qos_tc_params tc_params = {0};

	tc_params.port_id = port_id;
	tc_params.deq_idx = deq_idx;

	return qos_tc_setup_ext(dev, type, type_data, &tc_params);
}
EXPORT_SYMBOL(qos_tc_setup);

int qos_tc_setup_ext(struct net_device *dev,
		 enum tc_setup_type type,
		 void *type_data,
		 const struct qos_tc_params *tc_params)
{
	int ret = 0;

	ASSERT_RTNL();
	netdev_dbg(dev, "%s: start type %d\n", __func__, type);

	trace_qos_tc_setup_enter(dev, type);

	if (!tc_params) {
		netdev_err(dev, "Invalid tc_params\n");
		return -EOPNOTSUPP;
	}

	switch (type) {
	case TC_SETUP_QDISC_MQPRIO:
		ret = qos_tc_mqprio_offload(dev,
		      ((struct tc_mqprio_qopt_offload *)type_data)->handle,
		      type_data, tc_params);
		break;
	case TC_SETUP_QDISC_PRIO:
		ret = qos_tc_prio_offload(dev, type_data, tc_params);
		break;
	case TC_SETUP_QDISC_DRR:
		ret = qos_tc_drr_offload(dev, type_data, tc_params);
		break;
	case TC_SETUP_QDISC_RED:
		ret = qos_tc_red_offload(dev, type_data);
		break;
	case TC_SETUP_QDISC_TBF:
		ret = qos_tc_tbf_offload(dev, type_data);
		break;
	case TC_SETUP_QDISC_CODEL:
		ret = qos_tc_codel_offload(dev, type_data);
		break;
	case TC_SETUP_BLOCK:
		ret = qos_tc_block_offload(dev, type_data);
		break;
	case TC_SETUP_QDISC_MQ:
	case TC_SETUP_ROOT_QDISC:
	case TC_SETUP_QDISC_FIFO:
		/* TODO: Support to be added */
		netdev_dbg(dev, "offload type: %d not supported\n", type);
		ret = -EOPNOTSUPP;
		break;
	default:
		netdev_err(dev, "offload type: %d not supported\n", type);
		ret = -EOPNOTSUPP;
		break;
	}

	trace_qos_tc_setup_exit(dev, type);

	return ret;
}
EXPORT_SYMBOL(qos_tc_setup_ext);

int qos_tc_setup_gen(struct net_device *dev,
		     enum tc_setup_type type,
		     void *type_data)
{
	return  qos_tc_setup(dev, type, type_data, -1, -1);
}

static int __init qos_tc_init(void)
{
	int ret = 0;

	ret = qos_tc_debugfs_init();
	if (ret)
		return ret;

	rtnl_lock();
	qos_tc_setup_fn = qos_tc_setup;
	ret = qos_tc_mappings_init();
	rtnl_unlock();

	if (ret) {
		qos_tc_debugfs_exit();
		return ret;
	}

	return ret;
}

static void __exit qos_tc_destroy(void)
{
	qos_tc_debugfs_exit();
	rtnl_lock();
	qos_tc_ports_cleanup();
	qos_tc_setup_fn = NULL;
	rtnl_unlock();
}

module_init(qos_tc_init);
module_exit(qos_tc_destroy);
MODULE_LICENSE("GPL");
MODULE_ALIAS_RTNL_LINK(DRV_NAME);
