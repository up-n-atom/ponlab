// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/

#include <linux/netdevice.h>
#include <linux/types.h>
#include <net/pkt_cls.h>

int qos_tc_police_offload(struct net_device *dev,
			  struct flow_cls_offload *f,
			  bool ingress);

int qos_tc_police_unoffload(struct net_device *dev,
			    void *meter_cfg_ptr, void *flags_ptr);

void qos_tc_policer_list_debugfs(struct seq_file *file, void *ctx);
