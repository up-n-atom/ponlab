/* SPDX-License-Identifier: GPL-2.0 */
/****************************************************
 *
 * Copyright (C) 2020 - 2023 MaxLinear, Inc.
 * Copyright (C) 2017 - 2020 Intel Corporation
 *
 ***************************************************/
#ifndef __QOS_HAL_API_H
#define __QOS_HAL_API_H
/*! \file qos_hal_api.h
  \brief This file contains: QOS HAL API.
 */
#include <net/qos_mgr/qos_mgr_common.h>

/* forward declarations */
struct dp_dequeue_res;

void qos_mgr_get_queues(struct dp_dequeue_res *qos_res, struct net_device *netdev,
		int *qid_qmap);

int32_t qos_mgr_set_mapped_queue(
		struct net_device *netdev,
		int32_t portid,
		int32_t deq_idx,
		uint32_t queue_id, /* Queue Id */
		uint32_t dir, /* 0 - Egress 1 - Ingress */
		uint32_t class, /* Traffic class */
		int32_t flag);

#endif  /*  __QOS_HAL_API_H*/
