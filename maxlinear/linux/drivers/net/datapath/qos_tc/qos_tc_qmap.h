/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright (c) 2021 - 2023 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/
#ifndef _QOS_TC_QMAP_
#define _QOS_TC_QMAP_

#include <net/qos_tc.h>

int qos_tc_map(struct net_device *dev, struct flow_cls_offload *f,
	       bool ingress, const struct qos_tc_params *tc_params);
int qos_tc_unmap(struct net_device *dev, void *list_node,
		const struct qos_tc_params *tc_params);
int qos_tc_classid_unmap(u32 classid);

void qos_tc_class_list_debugfs(struct seq_file *file, void *ctx);

#endif
