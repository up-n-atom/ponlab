/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright (c) 2021 - 2022 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/

#ifndef _QOS_TC_VLAN_FILTER_
#define _QOS_TC_VLAN_FILTER_

int qos_tc_vlan_filter_add(struct net_device *dev,
			   struct flow_cls_offload *f,
			   bool ingress);

int qos_tc_vlan_filter_del(struct net_device *dev,
			   void *vlan_storage,
			   void *rule);

void qos_tc_vlan_filter_storage_debugfs(struct seq_file *file, void *ctx);

#endif
