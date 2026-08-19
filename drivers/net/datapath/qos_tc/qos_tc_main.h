/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright (c) 2020 - 2024 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/
#ifndef _QOS_TC_MAIN_
#define _QOS_TC_MAIN_

int qos_tc_drr_offload(struct net_device *dev,
		       void *type_data,
		       const struct qos_tc_params *tc_params);

int qos_tc_prio_offload(struct net_device *dev,
			void *type_data,
			const struct qos_tc_params *tc_params);

int qos_tc_mqprio_offload(struct net_device *dev,
			  u32 handle,
			  void *type_data,
			  const struct qos_tc_params *tc_params);

int qos_tc_flower_offload(struct net_device *dev, void *type_data,
		const struct qos_tc_params *tc_params);

int qos_tc_red_offload(struct net_device *dev,
		       void *type_data);

int qos_tc_tbf_offload(struct net_device *dev,
		       void *type_data);

int qos_tc_block_offload(struct net_device *dev,
			 void *type_data);

int qos_tc_codel_offload(struct net_device *dev, void *type_data);
#endif
