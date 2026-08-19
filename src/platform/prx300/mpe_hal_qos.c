/*******************************************************************************
 **
 ** FILE NAME	: mpe_hal_qos.c
 ** PROJECT	: MPE HAL
 ** MODULES	: MPE (Routing/Bridging Acceleration )
 **
 ** DATE	: 5 July 2023
 ** AUTHOR	: Ramulu Komme
 ** DESCRIPTION	: MPE HAL Layer
 ** COPYRIGHT	: Copyright © 2023-2024 MaxLinear, Inc.
 **
 **	 For licensing information, see the file 'LICENSE' in the root folder of
 **	 this software module.
 **
 *******************************************************************************/

/*
 *	Chip Specific Header File
 */
#include <net/ppa/ppa_api.h>
#include "ltq_mpe_hal.h"
#include "mpe_hal_qos.h"

#if IS_ENABLED(CONFIG_QOS_TC)
#include <net/qos_tc.h>
#endif

struct mpe_qos_port_info {
	int mpe_sch_id; /* mpe default scheduler ID */
	int cqm_deq_port; /* CQM dequeue port ID */
	int mpe_qmap_port; /* Dp port id, used to set tc-to-queue mapping */
	int mpe_def_q; /* Default queue of the scheduler, used to update the tc-to-qmap */
};

/* Store mpe port QoS details */
static struct mpe_qos_port_info mpe_qos_info;

/* mpe net device for mpe0 */
static struct net_device *mpe_ndev;

#if IS_ENABLED(CONFIG_QOS_TC)
/* The hardware interface open callback.
 * This interface does not support traffic and is only used for management.
 */
static int mpe_hal_netdev_open(struct net_device *mpe_ndev)
{
	return -EBUSY;
}

/* The hardware interface close callback.
 * This interface does not support traffic and is only used for management.
 */
static int mpe_hal_netdev_close(struct net_device *mpe_ndev)
{
	return -EBUSY;
}

/* The hardware interface transmit callback.
 * This interface does not support traffic and is only used for management.
 */
static netdev_tx_t mpe_hal_netdev_start_xmit(struct sk_buff *skb,
		struct net_device *mpe_ndev)
{
	kfree_skb(skb);
	return NETDEV_TX_OK;
}

/*
 * The purpose of this function is to invoke the tc offload function to
 * configure QoS on mpe interfaces.
 */
#if (KERNEL_VERSION(4, 14, 0) > LINUX_VERSION_CODE)
static int mpe_hal_qos_tc_setup(struct net_device *mpe_ndev, u32 handle,
		__be16 protocol, struct tc_to_netdev *tc)
#else
static int mpe_hal_qos_tc_setup(struct net_device *dev, enum tc_setup_type type,
				void *type_data)
#endif
{
	struct qos_tc_params tc_params = {0};

	tc_params.port_id = 0;
	tc_params.deq_idx = 0;
	tc_params.sch_id = mpe_qos_info.mpe_sch_id;
	tc_params.cqm_deq_port = mpe_qos_info.cqm_deq_port;
	tc_params.qmap_port = mpe_qos_info.mpe_qmap_port;
	tc_params.def_q = mpe_qos_info.mpe_def_q;
	tc_params.flags |= QOS_TC_LINK_SCH | QOS_TC_Q_MAP;
	tc_params.arbi = ARBITRATION_WRR;
	/*
	 * The maximum weight range supported by ppv4 is from 1 to 126.
	 * TODO: Set weight to the maximum value 126.
	 */
	tc_params.prio_wfq = 1;

#if (KERNEL_VERSION(4, 14, 0) > LINUX_VERSION_CODE)
	return qos_tc_setup_ext(mpe_ndev, handle, protocol, tc, &tc_params);
#else
	return qos_tc_setup_ext(mpe_ndev, type, type_data, &tc_params);
#endif
}

/* Net dev ops for the mpe0 interface. */
static const struct net_device_ops mpe_hal_netdev_ops = {
	.ndo_open = mpe_hal_netdev_open,
	.ndo_stop = mpe_hal_netdev_close,
	.ndo_start_xmit = mpe_hal_netdev_start_xmit,
	.ndo_setup_tc = mpe_hal_qos_tc_setup,
};
#endif /* CONFIG_QOS_TC */

/*
 * The purpose of this function is to create the mpe0 dummy interface during
 * system init.
 */
int mpe_hal_create_mpe_netdev(void)
{
	int ret = 0;

	mpe_ndev = alloc_netdev_mq(0, "mpe%d", 0, ether_setup, 1);
	if (!mpe_ndev) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "Failed to allocate mpe0 netdev\n");
		return PPA_FAILURE;
	}

#if IS_ENABLED(CONFIG_QOS_TC)
	mpe_ndev->netdev_ops = &mpe_hal_netdev_ops;
	mpe_ndev->features |= NETIF_F_HW_TC;
#endif /* CONFIG_QOS_TC */
	ret = register_netdev(mpe_ndev);
	if (ret) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "Cannot register mpe0 net device\n");
		free_netdev(mpe_ndev);
		return ret;
	}
	return ret;
}

void mpe_hal_remove_mpe_netdev(void)
{
	if (mpe_ndev) {
		unregister_netdev(mpe_ndev);
		free_netdev(mpe_ndev);
	}
}

/*
 * The purpose of this function is to link the existing queue of the mpe port
 * to DRR scheduler.
 */
static int mpe_hal_link_queue_to_sch(int sch_id, int q_id)
{
	struct dp_node_link node = {0};

	node.arbi = ARBITRATION_WRR;
	/* The PPv4 max supported weight range is 1 to 126 */
	node.prio_wfq = 1; /* TODO: Use max supported weight(126) */
	node.node_type = DP_NODE_QUEUE;
	node.node_id.q_id = q_id;
	node.p_node_type = DP_NODE_SCH;
	node.p_node_id.sch_id = sch_id;
	if (dp_node_link_add(&node, 0) == DP_FAILURE) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "%s: failed to link queue: %d to sched: %d\n",
				__func__, q_id, sch_id);
		return PPA_FAILURE;
	}
	return PPA_SUCCESS;
}

/*
 * The purpose of this function is to add additional queue on mpe port for DS
 * traffic and link it to the DRR scheduler.
 */
int mpe_hal_add_ds_queue(int sch_id)
{
	struct dp_node_alloc anode = {0};
	int ret;

	/* Allocate queue */
	anode.inst = 0;
	anode.dp_port = 0;
	anode.type = DP_NODE_QUEUE;
	anode.id.q_id = DP_NODE_AUTO_ID;
	ret = dp_node_alloc(&anode, 0);
	if (ret == DP_FAILURE) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "%s: failed to alloc queue\n", __func__);
		return PPA_FAILURE;
	}

	/* Link allocated queue to WRR scheduler */
	if (mpe_hal_link_queue_to_sch(sch_id, anode.id.q_id) < 0) {
		dp_node_free(&anode, DP_NODE_AUTO_FREE_RES);
		return PPA_FAILURE;
	}
	return anode.id.q_id;
}

/*
 * The purpose of this function is to perform the following tasks.
 * Allocate a Weighted Round Robin (WRR) scheduler on the MPE port.
 * Connects the existing MPE queue to the newly allocated scheduler.
 */
int mpe_hal_add_sched(struct dp_dequeue_res *dq_res)
{
	struct dp_node_link node = {0};
	struct dp_node_alloc anode = {0};
	int i, ret;

	/* Allocate scheduler */
	anode.inst = 0;
	anode.dp_port = 0;
	anode.type = DP_NODE_SCH;
	anode.id.sch_id = DP_NODE_AUTO_ID;
	ret = dp_node_alloc(&anode, 0);
	if (ret == DP_FAILURE) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "%s: sch_id alloc fialed\n", __func__);
		return PPA_FAILURE;
	}

	/* Fetch the mpe queue from the list of cpu queues */
	for (i = 0; i < dq_res->num_q; i++) {
		if (dq_res->q_res[i].cqm_deq_port_type == DP_DATA_PORT_MPE) {
			node.cqm_deq_port.cqm_deq_port = dq_res->q_res[i].cqm_deq_port;
			break;
		}
	}

	node.node_id.sch_id = anode.id.sch_id;
	node.inst = 0;
	node.dp_port = 0;
	node.p_node_type = DP_NODE_PORT;
	node.p_node_id.cqm_deq_port = dq_res->cqm_deq_port;
	node.arbi = ARBITRATION_WRR;
	node.prio_wfq = 0;
	node.node_type = DP_NODE_SCH;
	if (dp_node_link_add(&node, 0) == DP_FAILURE) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "%s: failed to link sched: %d to port\n",
				__func__, node.node_id.sch_id);
		dp_node_free(&anode, DP_NODE_AUTO_FREE_RES);
		return PPA_FAILURE;
	}

	/* Link the existing MPE queue to the newly allocated scheduler. */
	if (mpe_hal_link_queue_to_sch(anode.id.sch_id, dq_res->q_res[i].q_id) < 0) {
		dp_node_free(&anode, DP_NODE_AUTO_FREE_RES);
		return PPA_FAILURE;
	}

	/* Update mpe port QoS details */
	mpe_qos_info.mpe_sch_id = anode.id.sch_id;
	mpe_qos_info.cqm_deq_port = node.cqm_deq_port.cqm_deq_port;

	return anode.id.sch_id;
}

void mpe_hal_update_mpe_qos_info(u32 dp_port, int q_id)
{
	mpe_qos_info.mpe_qmap_port = dp_port;
	mpe_qos_info.mpe_def_q = q_id;
}

MODULE_LICENSE("GPL");
