/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright (c) 2022 - 2023 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/
#ifndef _QOS_TC_
#define _QOS_TC_

#include <linux/version.h>

/* tc_flags that can be set by the callers of the qos-tc setup */
enum tc_flags {
	/*
	 * This flag can be set when there is a need to link newly created
	 * scheduler to a specific scheduler provided through qos_tc_params.
	 */
	QOS_TC_LINK_SCH = (1 << 0),
	/*
	 * This flag can be set to map the traffic class(TC) of the deleted queue
	 * to a specific queue provided through qos_tc_params.
	 */
	QOS_TC_Q_MAP = (1 << 1),
	/* This flag indicates that tc config is intended for Logical interface. */
	QOS_TC_IS_LIF_CONFIG = (1 << 2),
};

enum qos_tc_notify_event_type {
	QOS_TC_EVENT_Q_ADDED,
	QOS_TC_EVENT_Q_DELETED,
	QOS_TC_EVENT_SCH_ADDED,
	QOS_TC_EVENT_SCH_DELETED,
};

struct notifier_block;
struct qos_tc_notifier_data {
	struct net_device *netif;
	s32 sch_parent_id;
	s32 sch_id;
	s32 qid; /* dpm qid */
	s32 idx; /* index at which sch or queue is added/deleted */
};

/*
 * The purpose of this structure is to allow tc users to provide data to
 * qos-tc that is related to default configurations of the physical port and
 * other necessary flags.
 */
struct qos_tc_params {
	/* Dp port-id of the physical port */
	s32 port_id;
	/* Dequeuing index */
	s32 deq_idx;
	/* CQM dequeue port id */
	s32 cqm_deq_port;
	/* Default scheduler of the port, used to link new schedulers as children */
	s32 sch_id;
	/* Dp port id, used to set tc-to-queue mapping for newly created queues */
	s32 qmap_port;
	/*
	 * Default queue of the scheduler, used to update the tc-to-qmap table
	 * for deleted queues/TC's
	 */
	s32 def_q;
	/* 32-bit flag for handling new feature-related cases in qos-tc module */
	u32 flags;
	/* Dp alloc flag of the physical port */
	s32 dp_alloc_flag;
	/* Arbitration method of the new scheduler. */
	s32 arbi;
	/* node priority/weight of the new scheduler. */
	s32 prio_wfq;
};

int qos_tc_event_register(struct notifier_block *nb);
void qos_tc_event_deregister(struct notifier_block *nb);

int qos_tc_setup(struct net_device *dev,
		 enum tc_setup_type type,
		 void *type_data,
		 int port_id,
		 int deq_idx);

int qos_tc_setup_gen(struct net_device *dev,
		     enum tc_setup_type type,
		     void *type_data);

extern int (*qos_tc_setup_fn)(struct net_device *dev,
			      enum tc_setup_type type,
			      void *type_data,
			      int port_id,
			      int deq_idx);

int qos_tc_setup_ext(struct net_device *dev, enum tc_setup_type type,
		void *type_data, const struct qos_tc_params *tc_params);
#endif
