/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright (c) 2021 - 2022 MaxLinear, Inc.
 *
 *****************************************************************************/

/**
 *  Drop packets based on IP address using PCE rules.
 *
 *  \param[in]	dev		Network device
 *  \param[in]	f		Flower offload
 *  \param[in]	cookie		TC command identifier
 */
int qos_tc_ip_drop_offload(struct net_device *dev,
			   struct flow_cls_offload *f,
			   unsigned long cookie);

/**
 *  Remove packets drop by removing PCE rule.
 *
 *  \param[in]	dev		Network device
 *  \param[in]	f		Flower offload
 *  \param[in]	cookie		TC command identifier
 */
int qos_tc_ip_drop_unoffload(struct net_device *dev,
			     struct flow_cls_offload *f,
			     unsigned long cookie);
