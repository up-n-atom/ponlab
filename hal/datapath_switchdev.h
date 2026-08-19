// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2021, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#ifndef DATAPATH_SWITCHDEV_H_
#define DATAPATH_SWITCHDEV_H_

int dp_swdev_alloc_bridge_id(int inst);
int dp_swdev_bridge_port_cfg_set(struct br_info *br_item,
				 int inst, int bport);
int dp_swdev_bridge_port_cfg_reset(struct br_info *br_item,
				   int inst, int bport);
int dp_swdev_bridge_cfg_set(int inst, u16 fid);
int dp_swdev_free_brcfg(int inst, u16 fid);
int dp_gswip_ext_vlan(int inst, int vap, int ep);
int dp_swdev_bridge_port_flags_set(struct br_info *br_item, int inst, int bport,
				   unsigned long flags);
int dp_swdev_bridge_port_flags_get(int inst, int bport, unsigned long *flags);
int dp_swdev_port_learning_limit_set(int inst, int bport, int learning_limit,
				     struct dp_dev *dev);
int dp_swdev_bridge_mcast_flood(int inst, int br_id, bool enable);
bool is_valid_domain(int inst, int curr_bp, int bp_member);
#endif
