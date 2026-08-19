/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 * This module provides API to publish and subscribe
 * QoS events like queue addition, deletion etc.
 * Copyright (C) 2023 MaxLinear, Inc.
 ******************************************************************************/
#define pr_fmt(fmt) "[qos_notify:%s:%d] " fmt, __func__, __LINE__

#include <linux/module.h>
#include <linux/slab.h>
#include <net/qos_notify.h>

/*!
 * brief strucutre to maintain notifier chain and users of the event
 */
struct qos_event_db {
	/*!
	 * qos event notifier chain name
	 */
	struct atomic_notifier_head qos_parse_chain;
	/*!
	 * Number of users registered for qos queue/scheduler
	 */
	atomic_t netif_qmap_users;
};

static struct qos_event_db *qos_db;

/*!
 *  qos events db initialization
 */
static int qos_evt_db_init(void)
{
	qos_db = kzalloc(sizeof(*qos_db), GFP_KERNEL);
	if (!qos_db) {
		pr_err("Cannot allocate qos_db\n");
		return -ENOMEM;
	}
	atomic_set(&qos_db->netif_qmap_users, 0);
	ATOMIC_INIT_NOTIFIER_HEAD(&qos_db->qos_parse_chain);

	return 0;
}

/*!
 * notification event initialization
 */
static int __init qos_event_notify_init(void)
{
	return qos_evt_db_init();
}

static void __exit qos_event_notify_exit(void)
{
	kfree(qos_db); /* qos events db cleanup */
}

/*!
 * API for register qos_event
 */
int qos_event_register(struct notifier_block *nb)
{
	if (!nb) {
		pr_info("Invalid notifier_block\n");
		return -EINVAL;
	}

	atomic_inc(&qos_db->netif_qmap_users);
	return atomic_notifier_chain_register(&qos_db->qos_parse_chain, nb);
}
EXPORT_SYMBOL(qos_event_register);

/*!
 * API for deregister qos_event
 */
int qos_event_deregister(struct notifier_block *nb)
{
	if (!nb) {
		pr_info("Invalid notifier_block\n");
		return -EINVAL;
	}

	atomic_dec(&qos_db->netif_qmap_users);
	return atomic_notifier_chain_unregister(&qos_db->qos_parse_chain, nb);
}
EXPORT_SYMBOL(qos_event_deregister);

/*!
 * Description: This API will be used by notifiers to send notification to PPA
 *              and will returns once PPA copies this data into its db
 */
void qos_qmap_notify(void *data, int event)
{
	switch (event) {
	case QOS_EVENT_SCH_ADD:
	case QOS_EVENT_Q_ADD:
	case QOS_EVENT_SCH_DELETE:
	case QOS_EVENT_Q_DELETE:
		break;
	default:
		pr_err("%s: Invalid param\n", __func__);
	}
	atomic_notifier_call_chain(&qos_db->qos_parse_chain, event, data);
}
EXPORT_SYMBOL(qos_qmap_notify);

module_init(qos_event_notify_init);
module_exit(qos_event_notify_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("This module provides API to publish and subscribe QoS events like queue addition, deletion etc.");
