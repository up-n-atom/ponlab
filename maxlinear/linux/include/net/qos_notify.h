/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 * Copyright (C) 2023 MaxLinear, Inc.
 ******************************************************************************/
#include <linux/types.h>

#define USER_DEFAULT_QUEUE 0x01

/* event types */
enum qos_notify_event_type {
	QOS_EVENT_Q_ADD,
	QOS_EVENT_Q_DELETE,
	QOS_EVENT_SCH_ADD,
	QOS_EVENT_SCH_DELETE,
};

struct notifier_block;
/* brief structure for sending the queue event notification to PPA */
struct qos_notifier_data {
	struct net_device *netif;
	s32 sch_parent_id; /* parent scheduler */
	s32 sch_id; /* scheduler on which queue is created */
	s32 qid; /* dpm qid */
	s32 idx; /* index at which sch or queue is added/deleted */
	s32 flags; /* caller can specify additional info through this flags */
};

int qos_event_register(struct notifier_block *nb);
int qos_event_deregister(struct notifier_block *nb);
void qos_qmap_notify(void *data, int event);
