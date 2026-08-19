/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright (c) 2021 - 2023 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/

#ifndef _QOS_TC_VLAN_PREPARE_
#define _QOS_TC_VLAN_PREPARE_

enum tc_flower_vlan_tag {
	TC_VLAN_UNKNOWN = 0,
	TC_VLAN_UNTAGGED = 1,
	TC_VLAN_SINGLE_TAGGED = 2,
	TC_VLAN_DOUBLE_TAGGED = 3,
};

void qos_tc_dp_vlan_prepare(struct net_device *dev,
			    struct dp_tc_vlan *dp_vlan,
			    bool ingress);

enum tc_flower_vlan_tag
qos_tc_vlan_tag_get(struct net_device *dev,
		    struct flow_cls_offload *f);

int qos_tc_vlan_untagged_prepare(struct net_device *dev,
				 struct flow_cls_offload *f,
				 struct net_device *bp_dev,
				 struct dp_vlan0 *rule);
int qos_tc_vlan_untagged_flt_prepare(struct net_device *dev,
				     struct flow_cls_offload *f,
				     struct dp_tc_vlan *dp_vlan,
				     struct dp_vlan0 *rule);

int qos_tc_vlan_single_tagged_prepare(struct net_device *dev,
				      struct flow_cls_offload *f,
				      struct net_device *bp_dev,
				      struct dp_vlan1 *rule);
int qos_tc_vlan_single_tagged_flt_prepare(struct net_device *dev,
					  struct flow_cls_offload *f,
					  struct dp_vlan1 *rule);

int qos_tc_vlan_double_tagged_prepare(struct net_device *dev,
				      struct flow_cls_offload *f,
				      struct net_device *bp_dev,
				      struct dp_vlan2 *rule);
int qos_tc_vlan_double_tagged_flt_prepare(struct net_device *dev,
					  struct flow_cls_offload *f,
					  struct dp_vlan2 *rule);

int vlan_action_parse(struct net_device *dev,
		      struct flow_cls_offload *f,
		      struct dp_act_vlan *act,
		      enum tc_flower_vlan_tag tag);
#endif
