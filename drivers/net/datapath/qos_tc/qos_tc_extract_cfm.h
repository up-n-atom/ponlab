/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright (c) 2024 MaxLinear, Inc.
 *
 *****************************************************************************/

#ifndef _QOS_TC_EXTRACT_CFM_

#define _QOS_TC_EXTRACT_CFM_
#include <linux/netdevice.h>
#include <linux/types.h>
#include <net/pkt_cls.h>
#include "../dp_oam/dp_oam.h"

struct cfm_rule;

int qos_tc_cfm_offload(struct net_device *dev, struct flow_cls_offload *f,
		       bool ingress);

int qos_tc_cfm_unoffload(struct net_device *dev, struct flow_cls_offload *f,
			 unsigned long cookie, struct cfm_rule *rule,
			 unsigned long *p_cookie);

void qos_tc_cfm_debugfs(struct seq_file *file, void *ctx);

#endif
