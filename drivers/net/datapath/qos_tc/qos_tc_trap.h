/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/

/**
 *  Offload trap actions to HW using PCE rules.
 *
 *  \param[in]	dev		Network device
 *  \param[in]	f		Flower offload
 *  \param[in]	cookie		TC command identifier
 */
int qos_tc_trap_offload(struct net_device *dev,
			struct flow_cls_offload *f,
			unsigned long cookie);

/**
 *  Remove trap actions from HW by removing PCE rule.
 *
 *  \param[in]	dev		Network device
 *  \param[in]	f		Flower offload
 *  \param[in]	cookie		TC command identifier
 */
int qos_tc_trap_unoffload(struct net_device *dev,
			  struct flow_cls_offload *f,
			  unsigned long cookie);

void qos_tc_trap_storage_debugfs(struct seq_file *file, void *ctx);

