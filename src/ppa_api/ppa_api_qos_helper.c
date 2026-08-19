/*************************************************************************
 *
 * FILE NAME	: ppa_api_qos_helper.c
 * PROJECT	: LGM
 * MODULES	: PPA framework support for QoS events and helper functions
 *
 * DESCRIPTION	: PPA API support for QoS events and helper functions.
 * COPYRIGHT	: Copyright (C) 2022-2026 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder
 * of this software module.
 *
 * HISTORY
 * $Date                 $Author                 $Comment
 * 06 SEPT 2022          Gaurav Sharma           Initial PPA APIs support
 *                                               for TC-QoS.
 *
 * 11 APR 2023           Sarvesh Darak           Qos_tc adaptations to fapi
 *                                               for qos notification mechanism
 *
 *****************************************************************************/

#include <linux/types.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>

#include <linux/pp_api.h>
#include <net/qos_tc.h>
#if IS_ENABLED(CONFIG_QOS_NOTIFY)
#include <net/qos_notify.h>
#endif
#include <net/ppa/ppa_api.h>
#include <net/ppa/ppa_hal_api.h>
#include <net/ppa/ppa_drv_wrapper.h>
#include "ppa_api_session.h"
#include "ppa_api_sess_helper.h"
#include "ppa_api_hal_selector.h"
#include "ppa_api_qos_helper.h"
#include "ppa_api_misc.h"
#include "ppa_api_netif.h"
#if IS_ENABLED(CONFIG_QOS_MGR)
#include <net/qos_mgr/qos_hal_api.h>
#endif

/* Macros */
/* Max TC per port */
/* Max queues per port */
#define PORT_MAX_Q  (SZ_8 * SZ_8)
#define DEBUGFS_LEN 40
#define PRIO_MAX_Q 8
#define TC_MINOR_MASK 0xFFFF

/*! module name */
#define PPA_QOS_EVENT_MODULE "qos_helper"

/*! module error */
#define MODULE_ERR(fmt, ...) pr_err(PPA_QOS_EVENT_MODULE \
	" %s:%d " fmt, __func__, __LINE__, ##__VA_ARGS__)

/*! module dbg */
#define MODULE_DBG(fmt, ...) do {\
	pr_debug(PPA_QOS_EVENT_MODULE \
			" %s:%d " fmt, __func__, __LINE__, ##__VA_ARGS__);\
} while (0)

#if IS_ENABLED(CONFIG_QOS_NOTIFY)
/* macro support for assign callback  */
#define FLUSH_OPS(dev, func) (dev)->ops.flush_sessions = (func)
#define QUEUE_ADD_OPS(dev, func) (dev)->ops.queue_add = (func)
#define QUEUE_DEL_OPS(dev, func) (dev)->ops.queue_del = (func)

/* typedefs */
typedef struct qos_notifier_data qos_notifier_data;
#endif

/*! forward declaration. */
typedef struct netif_qos_info netdev_qos_priv;

/*! workqueue attributes. */
typedef struct wq_data {
	struct list_head list;
	ulong event_type;
#if IS_ENABLED(CONFIG_QOS_NOTIFY)
	qos_notifier_data *data;
#endif
	int32_t pp_q;
} qos_wq_data;

/*! QoS helper ops. */
typedef struct netif_qos_ops {
	int32_t (*flush_sessions)(int32_t dpid, int32_t qid);
	int32_t (*queue_add)(netdev_qos_priv *devpriv, int32_t qid,
	    int32_t q_prio, int32_t flags);
	int32_t (*queue_del)(netdev_qos_priv *devpriv, int32_t qid,
	    int32_t q_prio, int32_t flags);
} netdev_qos_event_ops;

/*! QoS helper attributes. */
struct netif_qos_info {
	/* netdev priv data */
	PPA_NETIF *netif;
	PPA_IFNAME dev_name[PPA_IF_NAME_SIZE];
	int32_t	ifindex;
	int32_t dpid; /* dp port for flush sessions */
	int32_t num_q; /* max queues */
	int32_t def_q; /* default queue */
	int32_t usr_def_q; /* user default queue */
	int32_t usr_def_q_prio; /* user default queue priority */
	int32_t flags; /* stores specific flags set by QoS managers */
	int32_t alloc_flag; /* dp_subif alloc flags */
	int32_t qid_map[PORT_MAX_Q]; /* mark to queue */
	uint16_t dfl_eg_sess[DP_DFL_SESS_NUM]; /* Used for WMM slowpath */
	spinlock_t dev_ev_lock;
	struct list_head pending_ev_list;
	struct workqueue_struct *dev_wq;
	struct work_struct dev_event_worker;
	netdev_qos_event_ops ops;

	struct list_head node;
	struct rcu_head rcu;
};

/*! errors associated with network device e.g. lookup */
struct netdev_err {
	atomic_t invalid;
	atomic_t phy_invalid;
	atomic_t db_lookup;
};

/*! errors associated with QoS helper. */
struct mod_err_stats {
	atomic_t notify_err;
	atomic_t q_err;
	atomic_t ops_err;
	atomic_t dp_err ;
	struct netdev_err dev_err;
};

/*! errors associated with QoS helper. */
struct qos_helper_db {
	struct list_head netif_list;
	struct mutex db_lock;
	PPA_NOTIFIER_BLOCK qos_ev_notifier;
	/*! dbg stats */
	struct mod_err_stats stats;
};

/*! QoS helper db */
static struct qos_helper_db *db;

/*! All local function definitions starts below */

/**
 * @brief Supports qos data lookup for netif.
 * @param PPA_NETIF
 * @return valid netdev_qos_priv or null
 */
static netdev_qos_priv *ppa_netdev_qos_data_lookup(PPA_NETIF *dev, char *name)
{
	netdev_qos_priv *pos;

	list_for_each_entry_rcu(pos, &db->netif_list, node) {
		if (dev && ppa_is_netif_equal(pos->netif, dev)) {
			MODULE_DBG("Entry found for [%s]\n", dev->name);
			return pos;
		} else if (name && !strcmp(pos->dev_name, name)) {
			MODULE_DBG("Dev null, entry found for [%s] pos->dev_name %s\n",
					name, pos->dev_name);
			return pos;
		}
	}
	return NULL;
}

#if IS_ENABLED(CONFIG_QOS_NOTIFY)
/**
 * @brief Update user default queue based on the queue priority.
 * @return 0
 */
static void ppa_qos_update_user_def_q(netdev_qos_priv *devpriv, int32_t qid,
		int32_t q_prio, bool is_q_add)
{
	int idx;

	if (is_q_add) {
		if (devpriv->usr_def_q == 0 || q_prio > devpriv->usr_def_q_prio) {
			devpriv->usr_def_q = qid;
			devpriv->usr_def_q_prio = q_prio;
		}
	} else {
		/*
		 * When deleting the current user default queue, find and set the next
		 * lowest priority queue as the new user default queue.
		 */
		devpriv->usr_def_q = 0;
		devpriv->usr_def_q_prio = -1;
		for (idx = q_prio - 1; idx > 0; idx--) {
			if (devpriv->qid_map[idx] != -1) {
				devpriv->usr_def_q = devpriv->qid_map[idx];
				devpriv->usr_def_q_prio = idx;
				break;
			}
		}
	}
}

#if !IS_ENABLED(CONFIG_QOS_MGR)
/**
 * @brief Update Q-Mapping for WiFi egress session.
 * @param netdev_qos_priv, qid and q_prio
 * @return PPA_SUCCESS/PPA_FAILURE
 */
static int32_t ppa_qos_session_dst_queue_update(netdev_qos_priv *devpriv,
		int32_t qid, int32_t q_prio)
{
	int32_t ret;
	struct dp_qos_q_logic q_logic = {0};

	q_logic.q_id = qid;
	if (dp_qos_get_q_logic(&q_logic, 0) == DP_FAILURE) {
		MODULE_ERR("Failed to get Logical queue for qid: %d\n", qid);
		return PPA_FAILURE;
	}

	ret = pp_session_dst_queue_update(devpriv->dfl_eg_sess[q_prio - 1],
		q_logic.q_logic_id);
	if (unlikely(ret)) {
		MODULE_ERR("Failed to set session %d dest queue to %d, ret %d\n",
		devpriv->dfl_eg_sess[q_prio - 1], q_logic.q_logic_id, ret);
		return PPA_FAILURE;
	}

	return PPA_SUCCESS;
}
#endif

/**
 * @brief netdev q add ops.
 * @param PPA_NETIF, qid
 * @return PPA_SUCCESS/PPA_FAILURE
 */
static int32_t ppa_qos_event_netif_q_add_ops(netdev_qos_priv *devpriv,
	int32_t qid, int32_t q_prio, int32_t flags)
{
	int32_t ret = PPA_SUCCESS;

	if (!devpriv) {
		atomic_inc(&db->stats.ops_err);
		MODULE_DBG("invalid args\n");
		return PPA_FAILURE;
	}

	if (q_prio >= PORT_MAX_Q) {
		atomic_inc(&db->stats.q_err);
		MODULE_ERR("Invalid q_prio(%d) for Dev(%s)\n", q_prio,
				devpriv->netif->name);
		return PPA_FAILURE;
	}

	if ((flags & USER_DEFAULT_QUEUE) == USER_DEFAULT_QUEUE) {
		devpriv->usr_def_q = qid;
		devpriv->flags |= USER_DEFAULT_QUEUE;
	}

	if (!(devpriv->flags & USER_DEFAULT_QUEUE))
		ppa_qos_update_user_def_q(devpriv, qid, q_prio, true);

	devpriv->qid_map[q_prio] = qid;
	MODULE_DBG("q(%d) add success at pos (%d)\n", qid, q_prio);

#if !IS_ENABLED(CONFIG_QOS_MGR)
	/* Update Q-Mapping for WMM egress session */
	if (devpriv->alloc_flag & DP_F_ACA)
		ret = ppa_qos_session_dst_queue_update(devpriv, qid, q_prio);
#endif

	return ret;
}

/**
 * @brief Supports netdev q del.
 * @param qid
 * @return PPA_FAILURE/PPA_SUCCESS
 */
static int32_t ppa_qos_event_netif_q_del_ops(netdev_qos_priv *devpriv,
		int32_t qid, int32_t q_prio, int32_t flags)
{
	int32_t ret = PPA_SUCCESS;

	if (!devpriv || qid < 0) {
		atomic_inc(&db->stats.ops_err);
		return PPA_FAILURE;
	}

	if (q_prio >= PORT_MAX_Q) {
		atomic_inc(&db->stats.q_err);
		MODULE_ERR("Invalid q_prio(%d) for Dev(%s)\n",
				q_prio, devpriv->netif->name);
		return PPA_FAILURE;
	}

	if (devpriv->qid_map[q_prio] < 0) {
		atomic_inc(&db->stats.ops_err);
		MODULE_DBG("Dev(%s) q (%d) not found\n",
				devpriv->netif->name, qid);
		return PPA_FAILURE;
	}

	if ((flags & USER_DEFAULT_QUEUE) == USER_DEFAULT_QUEUE) {
		devpriv->usr_def_q = 0;
		devpriv->flags &= ~USER_DEFAULT_QUEUE;
	}

	if (devpriv->usr_def_q_prio == q_prio)
		ppa_qos_update_user_def_q(devpriv, qid, q_prio, false);

	devpriv->qid_map[q_prio] = -1;

#if !IS_ENABLED(CONFIG_QOS_MGR)
	/* Set WMM egress queue to system_default_q for deleted queue */
	if (devpriv->alloc_flag & DP_F_ACA)
		ret = ppa_qos_session_dst_queue_update(devpriv, devpriv->def_q, q_prio);
#endif

	return ret;
}

/**
 * @brief support for next event in wq.
 * @param netdev
 * @return list ptr.
 */
static struct list_head *next_ev(netdev_qos_priv *devpriv)
{
	struct list_head *ret = NULL;

	spin_lock(&devpriv->dev_ev_lock);

	if (!list_empty(&devpriv->pending_ev_list)) {
		ret = devpriv->pending_ev_list.next;
		list_del(ret);
	}

	spin_unlock(&devpriv->dev_ev_lock);

	return ret;
}

/**
 * @brief support for handling events.
 * @param dp port
 * @return 0
 */
static void ppa_qos_wq_handler(netdev_qos_priv *devpriv,
		qos_wq_data *wq_data)
{
	PPA_NETIF *r_dev = NULL;
	int32_t r_q;
	int32_t r_idx;
	int32_t r_event;
	qos_notifier_data *data = NULL;

	if (!wq_data) {
		MODULE_ERR("invalid args\n");
		return;
	}

	data = wq_data->data;

	r_dev = data->netif;
	if (!r_dev || !ppa_is_netif_equal(devpriv->netif, r_dev)) {
		atomic_inc(&db->stats.dev_err.invalid);
		return;
	}

	r_q = data->qid;
	r_idx = data->idx;
	r_event = wq_data->event_type;
	MODULE_DBG("Received: dev (%s) q (%d) event (%s)\n",
			r_dev->name, r_q,
			(r_event == QOS_EVENT_Q_ADD) ? "Q ADD" :
			(r_event == QOS_EVENT_Q_DELETE) ? "Q DEL" :
			(r_event == QOS_EVENT_SCH_ADD) ? "SCH ADD" :
			(r_event == QOS_EVENT_SCH_DELETE) ? "SCH DEL" :
			"Invalid event!!");

	if (r_event == QOS_EVENT_Q_ADD) {
		devpriv->ops.queue_add(devpriv, r_q, r_idx, data->flags);
		/* flush sessions on the interface. */
		/* evaluate performance impact!! */
		/* time taken to flush sessions */
		devpriv->ops.flush_sessions(devpriv->dpid, -1);
	} else if (r_event == QOS_EVENT_Q_DELETE) {
		devpriv->ops.queue_del(devpriv, r_q, r_idx, data->flags);
		/* now flush sessions on received qid. */
		devpriv->ops.flush_sessions(devpriv->dpid, wq_data->pp_q);
	}
}

/**
 * @brief Support for Workqueue.
 * @param struct work_struct
 * @return 0
 */
static void ppa_qos_ev_wq_work(struct work_struct *work)
{
	qos_wq_data *wq_data = NULL;
	netdev_qos_priv *devpriv = NULL;
	struct list_head *lh = NULL;

	devpriv = container_of(work, netdev_qos_priv, dev_event_worker);
	if (!devpriv) {
		MODULE_ERR("Invalid devpriv\n");
		return;
	}

	while ((lh = next_ev(devpriv)) != NULL) {
		wq_data = list_entry(lh, qos_wq_data, list);
		ppa_qos_wq_handler(devpriv, wq_data);
		kfree(wq_data->data);
		kfree(wq_data);
	}
}

/**
 * @brief Support for flushing sessions based on netif.
 * @param dp port
 * @return 0
 */
static bool match_dpid(struct uc_session_node *p_item, void *data)
{
	int32_t *dpid = (int32_t *)data;

	if ((p_item->flags & SESSION_ADDED_IN_HW) &&
		p_item->dest_ifid == *dpid) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * @brief Support for flushing sessions.
 * @param dp port
 * @return 0
 */
static bool match_qid(struct uc_session_node *p_item, void *data)
{
	int32_t *dest_qid = (int32_t *)data;

	if ((p_item->flags & SESSION_ADDED_IN_HW) &&
			p_item->dest_qid == *dest_qid) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * @brief Support for flushing sessions.
 * @param dp port
 * @return 0
 */
static int32_t ppa_qos_event_netif_hw_sess_flush(int32_t dp_id, int32_t qid)
{
	/* TODO flush logic for multicast sessions */
	if (qid != -1) {
		MODULE_DBG("Deleting session with qid %d\n", qid);
		ppa_test_and_session_action(&qid, match_qid,
			ppa_hsel_del_routing_session);
	} else {
		MODULE_DBG("Deleting session with dp_id %d\n", dp_id);
		ppa_test_and_session_action(&dp_id, match_dpid,
			ppa_hsel_del_routing_session);
	}

	return PPA_SUCCESS;
}
#endif /* CONFIG_QOS_NOTIFY */

/**
 * @brief Support for allocating QoS resources.
 * @param void
 * @return 0
 */
static netdev_qos_priv *ppa_netif_qos_alloc_priv(void)
{
	netdev_qos_priv *obj = NULL;

	obj = ppa_malloc(sizeof(*obj));
	if (!obj) {
		MODULE_ERR("Critical malloc failed!\n");
		return NULL;
	}

	ppa_memset(obj, 0, sizeof(*obj));
	INIT_LIST_HEAD(&obj->pending_ev_list);
	spin_lock_init(&obj->dev_ev_lock);
#if IS_ENABLED(CONFIG_QOS_NOTIFY)
	INIT_WORK(&obj->dev_event_worker, ppa_qos_ev_wq_work);
	FLUSH_OPS(obj, ppa_qos_event_netif_hw_sess_flush);
	QUEUE_ADD_OPS(obj, ppa_qos_event_netif_q_add_ops);
	QUEUE_DEL_OPS(obj, ppa_qos_event_netif_q_del_ops);
#endif
	return obj;
}

/**
 * @brief Add support.
 * @param PPA_NETIF
 * @return 0
 */
static void ppa_add_netdev_qos_priv(netdev_qos_priv *dev)
{
	if (WARN_ON(!dev))
		return;

	list_add_rcu(&dev->node, &db->netif_list);
}

/**
 * @brief del support.
 * @param PPA_NETIF
 * @return 0
 */
static void ppa_del_netdev_qos_priv(netdev_qos_priv *dev)
{
	if (WARN_ON(!dev))
		return;

	if (!list_empty(&db->netif_list))
		list_del_rcu(&dev->node);
}

/**
 * @brief API support for fetching pp qid.
 * @param PPA_NETIF
 * @return 0
 */
static int32_t ppa_dpm_to_pp_qos(int32_t q_id)
{
	struct pp_qos_dev *qdev;

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(!qdev)) {
		atomic_inc(&db->stats.dev_err.invalid);
		return PPA_FAILURE;
	}

	return pp_qos_queue_id_get(qdev, q_id);
}

/**
 * @brief API support for getting queues from DPM.
 * @param PPA_NETIF
 * @return 0
 */
static int32_t ppa_netdev_set_queue_map(netdev_qos_priv *priv)
{
	struct dp_dequeue_res dq_res = {0};
	dp_subif_t *dp_subif = NULL;
	PPA_NETIF *phy_dev = NULL;
	PPA_IFNAME phys_netif_name[PPA_IF_NAME_SIZE];
	int32_t index = 0;
	int32_t ret = PPA_FAILURE;
	struct atm_vcc *vcc = NULL;
	PPA_NETIF *netdev = NULL;

#if IS_ENABLED(CONFIG_QOS_MGR)
	int32_t qid_qmap[PORT_MAX_Q] = {0};
#endif

	if (!priv) {
		atomic_inc(&db->stats.dev_err.invalid);
		return PPA_FAILURE;
	}

	netdev = priv->netif;
	if (ppa_get_physical_if(netdev, NULL, phys_netif_name) == PPA_SUCCESS) {
		phy_dev = ppa_get_netif(phys_netif_name);
		if (!phy_dev || !phy_dev->name[0]) {
			MODULE_DBG("Invalid dev!!\n");
			return PPA_FAILURE;
		}
	} else {
		MODULE_ERR("no phydev for (%s)\n", netdev->name);
		return PPA_FAILURE;
	}

	dp_subif = ppa_malloc(sizeof(dp_subif_t));
	if (!dp_subif) {
		MODULE_ERR("dp_subif allocation failed\n");
		return PPA_FAILURE;
	}
	ppa_memset(dp_subif, 0, sizeof(PPA_SUBIF));
	if (netdev != NULL) {
		ppa_br2684_get_vcc(phy_dev, &vcc);
		if (dp_get_netif_subifid(phy_dev, NULL, vcc, 0, dp_subif, 0)
			!= PPA_SUCCESS) {
			atomic_inc(&db->stats.dp_err);
			MODULE_DBG("Failed to get subifId for <%s>\n",
					netdev->name);
			goto done;
		}
	}

	dq_res.dev = phy_dev;
	dq_res.dp_port = dp_subif->port_id;
	dq_res.q_res = NULL;
	dq_res.q_res_size = 0;
	dq_res.cqm_deq_idx = DEQ_PORT_OFFSET_ALL;
	if (dp_deq_port_res_get(&dq_res, 0) != DP_SUCCESS) {
		atomic_inc(&db->stats.dp_err);
		MODULE_DBG("Failed to Get number of QOS queues\n");
		goto done;
	}
	/* TC QOS max queue limit is 32 per interface */
	/* Configuration currently valid for TC QOS only */
	if (dq_res.num_q <= 0 || dq_res.num_q > PORT_MAX_Q) {
		atomic_inc(&db->stats.dp_err);
		MODULE_DBG("queue count out-of-range  %d\n", dq_res.num_q);
		goto done;
	}

	dq_res.q_res = ppa_malloc(sizeof(struct dp_queue_res) * dq_res.num_q);
	if (!dq_res.q_res) {
		MODULE_ERR("Queue list allocation failed\n");
		goto done;
	}

	priv->num_q = dq_res.q_res_size = dq_res.num_q;
	priv->dpid = dp_subif->port_id;

	if (dp_deq_port_res_get(&dq_res, 0) != DP_SUCCESS) {
		atomic_inc(&db->stats.dp_err);
		MODULE_DBG("Queue list receive failed\n");
		goto done;
	}

	ppa_memcpy(priv->dfl_eg_sess, dp_subif->dfl_eg_sess,
		sizeof(dp_subif->dfl_eg_sess));
	priv->alloc_flag = dp_subif->alloc_flag;
	priv->def_q = dp_subif->def_qid;
	for (; index < PORT_MAX_Q; index++)
		priv->qid_map[index] = -1;

#if IS_ENABLED(CONFIG_QOS_MGR)
	/* fetch the init queues */
	qos_mgr_get_queues(&dq_res, netdev, qid_qmap);
	for (index = 0; index < PORT_MAX_Q; index++) {
		if (qid_qmap[index])
			priv->qid_map[index] = qid_qmap[index];
	}
#endif
	ret = PPA_SUCCESS;
done:
	if (dq_res.q_res)
		ppa_free(dq_res.q_res);
	if (dp_subif)
		ppa_free(dp_subif);

	return ret;
}

/**
 * @brief API support read dpm and update qid map
 * @param netdev_qos_priv
 * @return PPA_FAILURE or PPA_SUCCESS
 */
static int32_t ppa_netdev_init_qmap(int32_t *ifindex)
{
	netdev_qos_priv *priv = NULL;

	priv = container_of(ifindex, netdev_qos_priv, ifindex);
	/* read netdevice dpm queues and update db */
	if (ppa_netdev_set_queue_map(priv)
			== PPA_FAILURE) {
		MODULE_DBG("queue read failed for (%s)\n",
				priv->netif->name);
		return PPA_FAILURE;
	}
	MODULE_DBG("dev:%s queue count:%d\n",
			priv->netif->name, priv->num_q);
	return PPA_SUCCESS;
}

#if IS_ENABLED(CONFIG_QOS_NOTIFY)
/**
 * @brief API support for data validation.
 * @param PPA_NETIF
 * @return 0
 */
static int32_t ppa_validate_qos_data(qos_notifier_data *rdata, u64 event)
{
	PPA_NETIF *r_dev = NULL;

	if (unlikely(!rdata)) {
		MODULE_DBG("Invalid data received!\n");
		return PPA_FAILURE;
	}

	r_dev = rdata->netif;
	if (unlikely(!r_dev)) {
		MODULE_DBG("Invalid net device received!\n");
		return PPA_FAILURE;
	}

	if (unlikely(event > QOS_EVENT_SCH_DELETE)) {
		MODULE_DBG("Invalid event %llu\n", event);
		return PPA_FAILURE;
	}

	return PPA_SUCCESS;
}

/**
 * @brief API support for receiving tc updates.
 * @param void *
 * @return 0
 */
static int32_t ppa_notify_handler(struct notifier_block *unused, ulong event,
		void *data)
{
	qos_wq_data *wq_data = NULL;
	netdev_qos_priv *devpriv = NULL;
	qos_notifier_data *pdata = NULL;
	PPA_NETIF *r_dev = NULL;

	if (!data) {
		atomic_inc(&db->stats.notify_err);
		return PPA_FAILURE;
	}

	/* Validate data */
	if (ppa_validate_qos_data(data, event) == PPA_FAILURE) {
		atomic_inc(&db->stats.notify_err);
		return PPA_FAILURE;
	}

	wq_data = kzalloc(sizeof(*wq_data), GFP_ATOMIC);
	if (!wq_data) {
		atomic_inc(&db->stats.notify_err);
		MODULE_DBG("failed to allocate data.\n");
		return PPA_ENOMEM;
	}

	wq_data->data = kzalloc(sizeof(qos_notifier_data), GFP_ATOMIC);
	if (!wq_data->data) {
		atomic_inc(&db->stats.notify_err);
		kfree(wq_data);
		return PPA_ENOMEM;
	}

	ppa_memcpy(wq_data->data, data, sizeof(qos_notifier_data));
	wq_data->event_type = event;
	pdata = wq_data->data;

	if (event == QOS_EVENT_Q_ADD || event == QOS_EVENT_Q_DELETE)
		wq_data->pp_q = ppa_dpm_to_pp_qos(pdata->qid);

	r_dev = pdata->netif;
	if (!r_dev) {
		atomic_inc(&db->stats.notify_err);
		kfree(wq_data->data);
		kfree(wq_data);
		return PPA_FAILURE;
	}

	rcu_read_lock();
	devpriv = ppa_netdev_qos_data_lookup(r_dev, NULL);
	if (devpriv) {
		spin_lock(&devpriv->dev_ev_lock);
		list_add_tail(&wq_data->list, &devpriv->pending_ev_list);
		queue_work(devpriv->dev_wq, &devpriv->dev_event_worker);
		spin_unlock(&devpriv->dev_ev_lock);
	} else {
		atomic_inc(&db->stats.notify_err);
		kfree(wq_data->data);
		kfree(wq_data);
		rcu_read_unlock();
		return PPA_FAILURE;
	}
	rcu_read_unlock();
	return PPA_SUCCESS;
}
#endif /* CONFIG_QOS_NOTIFY */

static bool ppa_qos_is_configured(netdev_qos_priv *devqospriv)
{
	uint8_t i;

	for (i = 0; i < PORT_MAX_Q; i++) {
		if (devqospriv->qid_map[i] != -1)
			return true;
	}
	return false;
}

static void ppa_qos_update_high_prio_q(uint32_t prio, netdev_qos_priv *devqospriv,
		struct netdev_attr *attr)
{
	int32_t idx;

	/*
	 * The usr_def_q will be stored at lower priority in the qid_map.
	 * Traverse in the reverse direction to find the next high-priority queue.
	 * For unclassified traffic, use the prio of usr-def-q to get the dst_q_high
	 */
	idx = prio ? (prio - 1) : (devqospriv->usr_def_q_prio - 1);
	for (; idx > 0; idx--) {
		if (devqospriv->qid_map[idx] != -1) {
			MODULE_DBG("High prio queue(%d) found\n", devqospriv->qid_map[idx]);
			attr->dst_q_high = ppa_dpm_to_pp_qos(devqospriv->qid_map[idx]);
			return;
		}
	}
	attr->dst_q_high = -1;
}

/**
 * @brief Support for reading qid map of netif.
 * @param PPA_NETIF
 * @param netdev attributes.
 * @return queue or -1
 */
static int32_t ppa_qos_get_mapped_queue(PPA_NETIF __rcu *netif,
	struct netdev_attr *attr)
{
	int32_t dp_q = -1;
	int32_t pp_q = 0;
	uint32_t prio = 0U;
	PPA_NETIF *txif = NULL;
	PPA_IFNAME phys_netif_name[PPA_IF_NAME_SIZE];
	netdev_qos_priv *devqospriv = NULL;
	PPA_NETIF *netifdev = NULL;

	netifdev = rcu_dereference_raw(netif);
	if (!netifdev) {
		atomic_inc(&db->stats.dev_err.invalid);
		return PP_QOS_INVALID_ID;
	}

	if (ppa_get_physical_if(netifdev, NULL, phys_netif_name) == PPA_SUCCESS)
		txif = ppa_get_netif(phys_netif_name);

	if (!txif) {
		atomic_inc(&db->stats.dev_err.phy_invalid);
		return PP_QOS_INVALID_ID;
	}

	rcu_read_lock();
	devqospriv = ppa_netdev_qos_data_lookup(netifdev, NULL);
	if (devqospriv && !ppa_qos_is_configured(devqospriv)) {
		/* Fetch queues from phy port if QoS is not configured on logical-if */
		devqospriv = ppa_netdev_qos_data_lookup(txif, NULL);
	}
	if (!devqospriv) {
		atomic_inc(&db->stats.dev_err.db_lookup);
		rcu_read_unlock();
		return PP_QOS_INVALID_ID;
	}
	MODULE_DBG("netif %s txif %s mark %u\n",
		netifdev->name, txif->name, attr->mark);

#ifdef HAVE_QOS_EXTMARK
	GET_DATA_FROM_MARK_OPT(attr->mark, QUEPRIO_MASK, QUEPRIO_START_BIT_POS,
			prio);
#else
	GET_DATA_FROM_MARK_OPT(attr->mark, MARK_QUEPRIO_MASK,
			MARK_QUEPRIO_START_BIT_POS, prio);
#endif
	/*
	 * Priority selection logic:
	 * - If mark priority < PRIO_MAX_Q (8): Extract from skb->priority (TC minor handle)
	 * - If mark priority >= PRIO_MAX_Q: Use the extracted mark priority value
	 * Valid priority range: 0(lowest) -> 7(highest).
	 * TODO: Queue selection should be based on both major and minor handles
	 *       once hierarchical QoS support is implemented.
	 */
	prio = (prio < PRIO_MAX_Q) ? (attr->tc & TC_MINOR_MASK) : prio;

	/* Get the dst_q_high for TDOX ACK prioritization */
	ppa_qos_update_high_prio_q(prio, devqospriv, attr);

	/* Get the destination queue for data traffic */
	dp_q = devqospriv->qid_map[prio];
	dp_q = (dp_q >= 0) ? dp_q : ((devqospriv->usr_def_q > 0) ?
		devqospriv->usr_def_q : devqospriv->def_q);

	pp_q = ppa_dpm_to_pp_qos(dp_q);
	if (pp_q == PP_QOS_INVALID_ID) {
		/* get default queue */
		dp_q = devqospriv->qid_map[0];
		pp_q = ppa_dpm_to_pp_qos(dp_q);
	}
	MODULE_DBG("devqospriv dev %s prio %u dp_q %d pp_q %d\n",
			devqospriv->netif->name, prio, dp_q, pp_q);
	rcu_read_unlock();
	return pp_q;
}

/**
 * @brief reset qos helper stats.
 */
static ssize_t debugfs_reset_helper_stats(struct file *file,
	const char __user *buf,
	size_t count, loff_t *data)
{
	int32_t len;
	char str[DEBUGFS_LEN];
	char *p;

	if (!capable(CAP_NET_ADMIN)) {
		MODULE_DBG("Write Permission denied");
		return 0;
	}

	len = min_t(size_t, count, DEBUGFS_LEN-1);
	len -= ppa_copy_from_user(str, buf, len);
	while (len && str[len - 1] <= ' ')
		len--;
	str[len] = 0;
	for (p = str; *p && *p <= ' '; p++, len--)
		;
	if (!*p)
		return count;

	if ((strlen(p) == 1) && strncmp(p, "c", 1) == 0) {
		atomic_set(&db->stats.notify_err, 0);
		atomic_set(&db->stats.q_err, 0);
		atomic_set(&db->stats.ops_err, 0);
		atomic_set(&db->stats.dp_err, 0);
		atomic_set(&db->stats.dev_err.invalid, 0);
		atomic_set(&db->stats.dev_err.phy_invalid, 0);
		atomic_set(&db->stats.dev_err.db_lookup, 0);
	} else {
		MODULE_ERR("usage: echo c > /<debugfs>/ppa/qos_helper/stats\n");
	}

	return len;
}

/**
 * @brief API support for debugfs read.
 * @param debugfs fs ptr.
 * @param void.
 * @return 0
 */
static int32_t _debugfs_read_ppa_qos_qid_map(struct seq_file *seq, void *v)
{
	netdev_qos_priv *pos;
	int32_t i = 0;
	int32_t j = 0;
	int32_t index = 0;

	if (!capable(CAP_SYSLOG)) {
		MODULE_DBG("Read Permission denied");
		return 0;
	}

	rcu_read_lock();
	list_for_each_entry_rcu(pos, &db->netif_list, node) {
		if (pos->netif->name[0] == '\0')
			continue;
		seq_printf(seq, "\n%s qos data:\n", pos->netif->name);
		for (j = 0; j < 4; j++) {
			seq_puts(seq,	"Mark: ");
			for (i = 0; i < 16; i++) {
				index = j * 16 + i;
				seq_printf(seq,	"%3d %*c", index, 1, ' ');
			}
			seq_puts(seq,	"\nDP(q):");
			for (i = 0; i < 16; i++) {
				index = j * 16 + i;
				if (pos->qid_map[index] == -1) {
					seq_printf(seq,	"%3d %*c",
						pos->usr_def_q ? pos->usr_def_q : pos->def_q, 1, ' ');
				} else {
					seq_printf(seq,	"%3d %*c",
							pos->qid_map[index], 1, ' ');
				}
			}
			seq_puts(seq,	"\nPP(q):");
			for (i = 0; i < 16; i++) {
				index = j * 16 + i;
				if (pos->qid_map[index] == -1) {
					seq_printf(seq,	"%3d %*c",
						ppa_dpm_to_pp_qos(
							pos->usr_def_q ? pos->usr_def_q : pos->def_q),
						1, ' ');
				} else {
					seq_printf(seq,	"%3d %*c",
							ppa_dpm_to_pp_qos(
								pos->qid_map[index]),
							1, ' ');
				}
			}
			seq_puts(seq,	"\n");
		}
	}
	rcu_read_unlock();
	return 0;
}

/**
 * @brief API support for helper stats
 * @param debugfs fs ptr.
 * @param void.
 * @return 0
 */
static int32_t _debugfs_read_helper_stats(struct seq_file *seq, void *v)
{

	if (!capable(CAP_SYSLOG)) {
		MODULE_DBG("Read Permission denied");
		return 0;
	}
	seq_printf(seq, "QoS helper db stats:\n");
	seq_printf(seq, "===============\n");
	seq_printf(seq, "notify err: (%u)\n",
			atomic_read(&db->stats.notify_err));
	seq_printf(seq, "q err: (%u)\n", atomic_read(&db->stats.q_err));
	seq_printf(seq, "ops err: (%u)\n", atomic_read(&db->stats.ops_err));
	seq_printf(seq, "dp err: (%u)\n", atomic_read(&db->stats.dp_err));
	seq_printf(seq, "netdev invalid: (%u)\n",
			atomic_read(&db->stats.dev_err.invalid));
	seq_printf(seq, "netdev base intf err: (%u)\n",
			atomic_read(&db->stats.dev_err.phy_invalid));
	seq_printf(seq, "netdev db lookup err: (%u)\n",
			atomic_read(&db->stats.dev_err.db_lookup));

	return 0;
}

/**
 * @brief API support for PPA TC debugfs
 * @param debugfs fs inode
 * @param file ptr
 * @return fs
 */
static int32_t debugfs_read_ppa_qos_qid_map(struct inode *inode,
	struct file *file)
{
	return single_open(file, _debugfs_read_ppa_qos_qid_map, NULL);
}

/**
 * @brief API support for helper stats.
 * @param debugfs fs inode
 * @param file ptr
 * @return fs
 */
static int32_t debugfs_read_helper_stats(struct inode *inode,
	struct file *file)
{
	return single_open(file, _debugfs_read_helper_stats, NULL);
}

/**
 * @brief struct support for PPA qos events debugfs
 * @param void
 * @return void
 */
static const struct file_operations dbgfs_file_ppa_qid_map_fops = {
	.owner          = THIS_MODULE,
	.open           = debugfs_read_ppa_qos_qid_map,
	.read           = seq_read,
	.write          = NULL,
	.llseek         = seq_lseek,
	.release        = single_release,
};

/**
 * @brief struct support for helper stats
 * @param void
 * @return void
 */
static const struct file_operations dbgfs_file_qos_helper_stats_fops = {
	.owner          = THIS_MODULE,
	.open           = debugfs_read_helper_stats,
	.read           = seq_read,
	.write          = debugfs_reset_helper_stats,
	.llseek         = seq_lseek,
	.release        = single_release,
};

static struct dentry *ppa_qos_sync_debugfs_dir;

/**
 * @brief struct support for PPA TC debugfs
 * @param void
 * @return void
 */
static struct ppa_debugfs_files ppa_qos_helper_debugfs_files[] = {
	{ "mark-to-queue", 0600, &dbgfs_file_ppa_qid_map_fops },
	{ "stats", 0600, &dbgfs_file_qos_helper_stats_fops },
};

/**
 * @brief API support for PPA TC debugfs.
 * @param void
 * @return void
 */
static void qos_helper_dbgfs_create(void)
{
	ppa_debugfs_create(ppa_debugfs_dir_get(), "qos_helper",
			&ppa_qos_sync_debugfs_dir, ppa_qos_helper_debugfs_files,
			ARRAY_SIZE(ppa_qos_helper_debugfs_files));
}

/**
 * @brief API support for debug support.
 * @param void
 * @return void
 */
static void qos_events_dbgfs_remove(void)
{
	debugfs_remove_recursive(ppa_qos_sync_debugfs_dir);
}

static int32_t ppa_qos_event_get_default_queue(PPA_NETIF *netdev,
	int32_t portid, PPA_SKBUF *skb)
{
	int32_t dst_q = PP_QOS_INVALID_ID;
	dp_subif_t *dp_subif = NULL;
	struct atm_vcc *vcc = NULL;
	PPA_NETIF *phy_dev = NULL;
	PPA_IFNAME phys_netif_name[PPA_IF_NAME_SIZE];

	if (ppa_get_physical_if(netdev, NULL, phys_netif_name) == PPA_SUCCESS)
		phy_dev = ppa_get_netif(phys_netif_name);

	if (!phy_dev) {
		MODULE_ERR("Failed to get phy_dev for Logical netdev: %s\n",
				netdev->name);
		return PPA_FAILURE;
	}

	dp_subif = ppa_malloc(sizeof(dp_subif_t));
	if (!dp_subif) {
		MODULE_ERR("dp_subif allocation failed\n");
		goto done;
	}

	if (netdev != NULL) {
		ppa_br2684_get_vcc(phy_dev, &vcc);
		if (dp_get_netif_subifid(phy_dev, skb, vcc, 0, dp_subif, 0) !=
				PPA_SUCCESS) {
			atomic_inc(&db->stats.dp_err);
			ppa_free(dp_subif);
			return PPA_FAILURE;
		}
	} else {
		dp_subif->port_id = portid;
		dp_subif->subif = 0;
	}

	dst_q = ppa_dpm_to_pp_qos(dp_subif->def_qid);
done:
	if (dp_subif)
		ppa_free(dp_subif);

	return dst_q;
}

int32_t ppa_qos_helper_dev_modify(PPA_NETIF *oldif, PPA_NETIF *newif)
{
	netdev_qos_priv *devpriv = NULL;

	rcu_read_lock();
	devpriv = ppa_netdev_qos_data_lookup(oldif, NULL);
	if (devpriv) {
		MODULE_DBG("devpriv exists for %s\n", devpriv->netif->name);
		rcu_read_unlock();
		rcu_replace_pointer(devpriv->netif, newif, 1);
		return PPA_SUCCESS;
	}
	rcu_read_unlock();
	return PPA_FAILURE;
}

int32_t ppa_qos_helper_dev_add(PPA_NETIF *dev)
{
	netdev_qos_priv *devpriv = NULL;
	PPA_NETIF *phy_dev = NULL;
	int32_t ret = PPA_SUCCESS;
	PPA_IFNAME phys_netif_name[PPA_IF_NAME_SIZE];

	if (!dev) {
		atomic_inc(&db->stats.dev_err.invalid);
		MODULE_DBG("Invalid dev!!\n");
		return PPA_FAILURE;
	}

	if (ppa_get_physical_if(dev, NULL, phys_netif_name) == PPA_SUCCESS) {
		phy_dev = ppa_get_netif(phys_netif_name);
		if (!phy_dev || !phy_dev->name[0]) {
			atomic_inc(&db->stats.dev_err.invalid);
			MODULE_DBG("Invalid dev!!\n");
			return PPA_FAILURE;
		}
	} else {
		atomic_inc(&db->stats.dev_err.phy_invalid);
		return PPA_FAILURE;
	}

	rcu_read_lock();
	devpriv = ppa_netdev_qos_data_lookup(dev, NULL);
	if (devpriv) {
		atomic_inc(&db->stats.dev_err.db_lookup);
		MODULE_DBG("QoS helper data already exist : %s\n", dev->name);
		rcu_read_unlock();
		return PPA_FAILURE;
	}
	rcu_read_unlock();

	devpriv = ppa_netif_qos_alloc_priv();
	if (devpriv) {
		/* assign dev */
		devpriv->netif = dev;
		ppa_strncpy(devpriv->dev_name, dev->name, sizeof(devpriv->dev_name));
		devpriv->ifindex = dev->ifindex;
		/* Initialize the user default queue priority to invalid value */
		devpriv->usr_def_q_prio = -1;
		/* initialize workq */
		devpriv->dev_wq = create_singlethread_workqueue(
				netdev_name(devpriv->netif));
		if (!devpriv->dev_wq) {
			MODULE_DBG("wq error for (%s)", dev->name);
			ret = PPA_FAILURE;
			goto done;
		}
		/* read dpm queues and update qid map */
		if (ppa_netdev_init_qmap(&devpriv->ifindex) != PPA_SUCCESS) {
			atomic_inc(&db->stats.dev_err.invalid);
			MODULE_DBG("Error qid map for (%s)", dev->name);
			ret = PPA_FAILURE;
			goto done;
		}
	} else {
		atomic_inc(&db->stats.dev_err.invalid);
		MODULE_DBG("qospriv alloc error (%s)\n", dev->name);
		ret = PPA_FAILURE;
		return ret;
	}
done:
	if (ret != PPA_SUCCESS) {
		if (devpriv->dev_wq)
			destroy_workqueue(devpriv->dev_wq);
		ppa_free(devpriv);
	} else {
		/* add now net device data to qos helper db*/
		mutex_lock(&db->db_lock);
		ppa_add_netdev_qos_priv(devpriv);
		mutex_unlock(&db->db_lock);
		MODULE_DBG("(%s) qos events registered successfully\n", dev->name);
	}
	return ret;
}

/**
 * @brief RCU callback support to free netdev_qos_priv*
 * @param rcu pointer.
 * @return void
 */
static void ppa_qos_helper_dev_free_cb(struct rcu_head *rcu)
{
	netdev_qos_priv *devpriv;

	devpriv = container_of(rcu, netdev_qos_priv, rcu);
	ppa_free(devpriv);
}

int32_t ppa_qos_helper_dev_del(struct netif_info *p_ifinfo)
{
	PPA_NETIF *dev = NULL;
	netdev_qos_priv *devpriv = NULL;
	bool dev_reset = 0;

	if (!p_ifinfo) {
		atomic_inc(&db->stats.dev_err.invalid);
		MODULE_ERR("Invalid p_ifinfo!!\n");
		return PPA_FAILURE;
	}
	mutex_lock(&db->db_lock);
	dev = p_ifinfo->netif;
	if (!dev && !strlen(p_ifinfo->name)) {
		atomic_inc(&db->stats.dev_err.invalid);
		MODULE_DBG("Invalid dev\n");
		mutex_unlock(&db->db_lock);
		return PPA_FAILURE;
	} else if (!dev && strlen(p_ifinfo->name)) {
		dev_reset = 1;
		MODULE_DBG("interface (%s) with null dev\n", p_ifinfo->name);
	}

	if (!dev_reset)
		devpriv = ppa_netdev_qos_data_lookup(dev, NULL);
	else
		devpriv = ppa_netdev_qos_data_lookup(NULL, p_ifinfo->name);

	if (!devpriv) {
		atomic_inc(&db->stats.dev_err.db_lookup);
		MODULE_DBG("Entry not found for dev: %s\n", dev->name);
		mutex_unlock(&db->db_lock);
		return PPA_FAILURE;
	}
	ppa_del_netdev_qos_priv(devpriv);
	mutex_unlock(&db->db_lock);
#if IS_ENABLED(CONFIG_QOS_NOTIFY)
	cancel_work_sync(&devpriv->dev_event_worker);
#endif
	if (devpriv->dev_wq)
		destroy_workqueue(devpriv->dev_wq);
	call_rcu(&devpriv->rcu, ppa_qos_helper_dev_free_cb);
	MODULE_DBG("deletion success for dev %s\n", p_ifinfo->name);
	return PPA_SUCCESS;
}

/**
 * @brief Delete all interfaces from helper database.
 * @param void
 * @return void
 */
void ppa_qos_helper_dev_del_all(void)
{
	netdev_qos_priv *pos;

	mutex_lock(&db->db_lock);
	list_for_each_entry_rcu(pos, &db->netif_list, node) {
		ppa_del_netdev_qos_priv(pos);
#if IS_ENABLED(CONFIG_QOS_NOTIFY)
		cancel_work_sync(&pos->dev_event_worker);
#endif
		if (pos->dev_wq)
			destroy_workqueue(pos->dev_wq);
		call_rcu(&pos->rcu, ppa_qos_helper_dev_free_cb);
		MODULE_DBG("deletion success for dev %s\n", pos->dev_name);
	}
	mutex_unlock(&db->db_lock);
}

/**
 * @brief API support getting interface mapped queue.
 * @param net device and structure for qos attributes.
 * @return dst_q_low and dst_q_high are updated by this API.
 * @return PPA_SUCCESS or PPA_FAILURE.
 */
int32_t ppa_dev_get_mapped_queue(PPA_NETIF *netdev, struct netdev_attr *attr)
{
	int32_t rc = PPA_SUCCESS;

	if (unlikely(!netdev) || unlikely(!attr)) {
		atomic_inc(&db->stats.dev_err.invalid);
		MODULE_DBG("NULL params\n");
		return PPA_EINVAL;
	}

	attr->dst_q_low = ppa_qos_get_mapped_queue(netdev, attr);
	if (attr->dst_q_low == PP_QOS_INVALID_ID) {
		MODULE_DBG("no queue retrieved from QoS modules (%s)",
				netdev->name);
		attr->dst_q_low = ppa_qos_event_get_default_queue(netdev,
				attr->portid, attr->skb);
		if (attr->dst_q_low == PP_QOS_INVALID_ID) {
			atomic_inc(&db->stats.q_err);
			MODULE_ERR("Critical error! no queue retrieved for (%s)",
					netdev->name);
		}
	}
	MODULE_DBG("returned q(%d) for tc (%d) mark (%u)\n", attr->dst_q_low,
				attr->tc, attr->mark);
	return rc;
}

void ppa_netdev_qos_helper_init(void)
{
	db = ppa_malloc(sizeof(*db));
	if (!db) {
		MODULE_ERR("db malloc failed!! \n");
		return;
	}
#if IS_ENABLED(CONFIG_QOS_NOTIFY)
	db->qos_ev_notifier.notifier_call = ppa_notify_handler;
	qos_event_register(&db->qos_ev_notifier);
#endif
	mutex_init(&db->db_lock);
	INIT_LIST_HEAD(&db->netif_list);
	ppa_memset(&db->stats, 0, sizeof(db->stats));
	qos_helper_dbgfs_create();
	ppa_api_get_mapped_queue = ppa_dev_get_mapped_queue;
}

void ppa_netdev_qos_helper_deinit(void)
{
#if IS_ENABLED(CONFIG_QOS_NOTIFY)
	qos_event_deregister(&db->qos_ev_notifier);
#endif
	qos_events_dbgfs_remove();
	ppa_api_get_mapped_queue = NULL;
	if (db) {
		ppa_qos_helper_dev_del_all();
		ppa_free(db);
	}
}
