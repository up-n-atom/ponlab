/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright (c) 2021 - 2023 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/

#ifndef _QOS_TC_EXT_VLAN_
#define _QOS_TC_EXT_VLAN_

#include "qos_tc_flower.h"

struct net_device;
struct seq_file;

int qos_tc_ext_vlan_add(struct net_device *dev, struct flow_cls_offload *f,
			bool ingress, struct net_device *bp_dev);

int qos_tc_ext_vlan_del(struct net_device *dev, void *vlan_storage,
			void *rule);

struct list_head *qos_tc_get_ext_vlan_storage(void);

void qos_tc_ext_vlan_debugfs(struct seq_file *file, void *ctx);

#endif
