/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright (c) 2021 - 2022 MaxLinear, Inc.
 *
 *****************************************************************************/

#ifndef _QOS_TC_PARSER_
#define _QOS_TC_PARSER_

int qos_tc_parse_ip(struct net_device *dev,
		    struct flow_cls_offload *f,
		    GSW_PCE_rule_t *pce_rule);

void qos_tc_tc2pce_vlan_prio_parse(struct net_device *dev,
				   struct flow_cls_offload *f,
				   GSW_PCE_rule_t *pce_rule);

void qos_tc_tc2pce_ndp_parse(struct net_device *dev,
			     struct flow_cls_offload *f,
			     GSW_PCE_rule_t *pce_rule);

bool qos_tc_parse_is_mcc(struct net_device *dev,
			 struct flow_cls_offload *f);

int qos_tc_to_pce(struct net_device *dev,
		  struct flow_cls_offload *f,
		  GSW_PCE_rule_t *pce_rule);

void qos_tc_set_drop(struct net_device *dev,
		     struct flow_cls_offload *f,
		     GSW_PCE_rule_t *pce_rule);

#endif
