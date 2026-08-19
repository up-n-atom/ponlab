/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright (c) 2023 MaxLinear, Inc.
 *
 *****************************************************************************/

/**
 *  Offload skbedit actions to HW.
 *
 *  \param[in]	dev		Network device
 *  \param[in]	f		Flower offload
 *  \param[in]	ingress		Ingress/Egress
 */
int qos_tc_skbedit_offload(struct net_device *dev,
			   struct flow_cls_offload *f,
			   bool ingress);

/**
 *  Remove skbedit actions from HW.
 *
 *  \param[in]	dev		Network device
 *  \param[in]	list_node	skbedit storage
 *  \param[in]  rule		Rule to be removed
 */
int qos_tc_skbedit_unoffload(struct net_device *dev,
			     void *skbedit,
			     void *rule);

/**
 *  Check for skbedit actions
 *
 *  \param[in]	dev		Network device
 *  \param[out]	tc		Pointer to traffic class
 */

int qos_tc_skbedit_action_check(struct flow_cls_offload *f,
				int *tc);

/**
 *  Check for skbedit actions
 *
 *  \param[in]	file		File
 *  \param[in]	ctx		Context
 */
void qos_tc_skbedit_debugfs(struct seq_file *file, void *ctx);
