/*************************************************************************
 *
 * FILE NAME	: ppa_api_qos_tc_logical.c
 * PROJECT	: LGM
 * MODULES	: PPA framework support for qos-tc.
 *
 * DESCRIPTION	: QoS-TC adaptation for Logical interfaces support.
 * COPYRIGHT	: Copyright (C) 2023-2026 MaxLinear Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder
 * of this software module.
 *
 * HISTORY
 * $Date                 $Author                 $Comment
 * 13 AUG 2023           Ramulu Komme            Initial version
 *
 *************************************************************************/

#include <net/pkt_cls.h>
#include <net/qos_tc.h>
#include <linux/pp_qos_api.h>
#include <linux/rtnetlink.h>
#include "ppa_api_qos_tc_logical.h"
#include "ppa_api_misc.h"
#include "ppa_api_netif.h"

struct wlan_qos_backup {
	/* Holds the queues properties */
	struct dp_dequeue_res dq_res;
	/* Holds the scheduler properties */
	struct pp_qos_common_node_properties common_prop;
	struct pp_qos_child_node_properties sched_child_prop;
};

enum ppa_qos_base_sched_type ppa_qos_base_sch_type = PPA_QOS_BASE_SCH_DRR;
/* Store the base scheduler information for each dp_port */
static struct base_sched base_sch_info[PPA_QOS_MAX_PHY_PORT];
/*
 * Temporarily stores user-configured QoS properties.
 * Saved on WLAN netdev deregister, freed on registration.
 */
static struct wlan_qos_backup wlan_qos_temp[PPA_QOS_MAX_WIFI_PORT];

static int32_t ppa_qos_get_base_sch_info(PPA_IFNAME *ifname)
{
	int32_t port_idx, lif_idx;

	for (port_idx = 0; port_idx < PPA_QOS_MAX_PHY_PORT; port_idx++) {
		for (lif_idx = 0; lif_idx < PPA_QOS_MAX_SUPPORTED_LIF; lif_idx++) {
			if (strcmp(ifname, base_sch_info[port_idx].lif_name[lif_idx]) == 0)
				return port_idx;
		}
	}
	return PPA_FAILURE;
}

static int32_t ppa_qos_get_base_sch_free_slot(void)
{
	int32_t port_idx;

	for (port_idx = 0; port_idx < PPA_QOS_MAX_PHY_PORT; port_idx++) {
		if (!base_sch_info[port_idx].sch_id)
			return port_idx;
	}
	return -1;
}

static struct base_sched *ppa_qos_get_l2_sched_info(PPA_IFNAME *ifname,
		int32_t port_idx)
{
	uint32_t i;
	struct base_sched *curr = NULL, *next = NULL;

	if (!list_empty(&base_sch_info[port_idx].l2_base_sched)) {
		ppa_list_for_each_entry_safe(curr, next,
				&base_sch_info[port_idx].l2_base_sched, list) {
			for (i = 0; i < PPA_QOS_MAX_VAP_PER_SCH; i++) {
				if (strcmp(ifname, curr->lif_name[i]) == 0)
					return curr;
			}
		}
	}
	return NULL;
}

/**
 * @brief: This function determines if the received TC configuration
 * is for adding a scheduler.
 */
static bool ppa_qos_is_sched_add_config(enum tc_setup_type type, void *type_data)
{
	struct tc_prio_qopt_offload *opt_prio = type_data;
	struct tc_drr_qopt_offload *opt_drr = type_data;
	struct tc_mqprio_qopt_offload *qopt_offload = type_data;
	struct tc_mqprio_qopt *opt_mqprio = NULL;

	if (type == TC_SETUP_QDISC_MQPRIO)
		opt_mqprio = &qopt_offload->qopt;

	if ((type == TC_SETUP_QDISC_PRIO && opt_prio->command == TC_PRIO_REPLACE) ||
		(type == TC_SETUP_QDISC_DRR && opt_drr->command == TC_DRR_REPLACE &&
		(!TC_H_MIN(opt_drr->handle))) ||
		(type == TC_SETUP_QDISC_MQPRIO && opt_mqprio && opt_mqprio->num_tc))
		return true;
	else
		return false;
}

/**
 * @brief: This function determines if the received TC configuration
 * is for deleting scheduler.
 */
static bool ppa_qos_is_sched_del_config(enum tc_setup_type type, void *type_data)
{
	struct tc_prio_qopt_offload *opt_prio = type_data;
	struct tc_drr_qopt_offload *opt_drr = type_data;
	struct tc_mqprio_qopt_offload *qopt_offload = type_data;
	struct tc_mqprio_qopt *opt_mqprio = NULL;

	if (type == TC_SETUP_QDISC_MQPRIO)
		opt_mqprio = &qopt_offload->qopt;

	if ((type == TC_SETUP_QDISC_PRIO && opt_prio->command == TC_PRIO_DESTROY) ||
		(type == TC_SETUP_QDISC_DRR && opt_drr->command == TC_DRR_DESTROY &&
		(!TC_H_MIN(opt_drr->handle))) ||
		(type == TC_SETUP_QDISC_MQPRIO && opt_mqprio && !opt_mqprio->num_tc))
		return true;
	else
		return false;
}

/**
 * @brief: This function handles the addition of a scheduler for a given LIF.
 * - Creates the base scheduler if it does not already exist.
 * - Updates the base scheduler information.
 */
static int32_t ppa_qos_add_base_sched(int32_t port_db_idx, struct base_sched *l2_sch)
{
	struct dp_node_link node = {0};
	struct dp_node_alloc anode = {0};
	struct dp_dequeue_res deq = {0};
	dp_subif_t *subif = NULL;
	int32_t ret, flags = 0;

	subif = &base_sch_info[port_db_idx].subif;
	flags = subif->data_flag;
	deq.dp_port = subif->port_id;
	deq.cqm_deq_idx = -1;
	ret = dp_deq_port_res_get(&deq, flags);
	if (ret < 0) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "%s: dp_deq_port_res_get failed\n",
				__func__);
		return ret;
	}

	/* Allocate scheduler */
	anode.inst = subif->inst;
	anode.dp_port = subif->port_id;
	anode.type = DP_NODE_SCH;
	anode.id.sch_id = DP_NODE_AUTO_ID;
	ret = dp_node_alloc(&anode, 0);
	if (ret == DP_FAILURE) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "%s: sch_id alloc fialed\n", __func__);
		return PPA_FAILURE;
	}

	if (ppa_qos_base_sch_type == PPA_QOS_BASE_SCH_WSP) {
		node.arbi = ARBITRATION_WSP;
		base_sch_info[port_db_idx].arbi = ARBITRATION_WSP;
	} else {
		node.arbi = ARBITRATION_WRR;
		base_sch_info[port_db_idx].arbi = ARBITRATION_WRR;
	}
	node.node_id.sch_id = anode.id.sch_id;
	node.inst = subif->inst;
	node.dp_port = subif->port_id;
	if (l2_sch) {
		node.p_node_id.sch_id = base_sch_info[port_db_idx].sch_id;
		node.p_node_type = DP_NODE_SCH;
		node.prio_wfq = QOS_MAX_CHILD_WRR_WEIGHT;
	} else {
		node.p_node_id.cqm_deq_port = deq.cqm_deq_port;
		node.p_node_type = DP_NODE_PORT;
		node.prio_wfq = 1;
	}
	node.cqm_deq_port.cqm_deq_port = deq.cqm_deq_port;
	node.node_type = DP_NODE_SCH;
	if (dp_node_link_add(&node, 0) == DP_FAILURE) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "%s: failed to link sched: %d to port\n",
				__func__, node.node_id.sch_id);
		dp_node_free(&anode, DP_NODE_AUTO_FREE_RES);
		return PPA_FAILURE;
	}

	/* Update base_sch_info with allocated scheduler details */
	if (l2_sch) {
		l2_sch->sch_id = anode.id.sch_id;
		l2_sch->arbi = base_sch_info[port_db_idx].arbi;
		l2_sch->prio_wfq = QOS_MAX_CHILD_WRR_WEIGHT;
		/* Add l2_sch to l2_base_sched list */
		list_add(&l2_sch->list, &base_sch_info[port_db_idx].l2_base_sched);
	} else {
		base_sch_info[port_db_idx].cqm_deq_port = deq.cqm_deq_port;
		base_sch_info[port_db_idx].sch_id = anode.id.sch_id;
	}
	return PPA_SUCCESS;
}

static int32_t ppa_qos_link_internal_sched(int32_t port_idx, struct base_sched *l2_sch,
		bool link_to_port)
{
	struct dp_node_link node = {0};

	if (link_to_port) {
		node.prio_wfq = 1;
		node.p_node_type = DP_NODE_PORT;
		node.p_node_id.cqm_deq_port = base_sch_info[port_idx].cqm_deq_port;
	} else {
		node.prio_wfq = QOS_MAX_CHILD_WRR_WEIGHT;
		node.p_node_type = DP_NODE_SCH;
		node.p_node_id.sch_id = base_sch_info[port_idx].sch_id;
	}

	node.cqm_deq_port.cqm_deq_port = base_sch_info[port_idx].cqm_deq_port;
	node.arbi = base_sch_info[port_idx].arbi;
	node.inst = base_sch_info[port_idx].subif.inst;
	node.dp_port = base_sch_info[port_idx].subif.port_id;
	node.node_type = DP_NODE_SCH;
	node.node_id.sch_id = l2_sch->sch_id;
	if (dp_node_link_add(&node, 0) == DP_FAILURE) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "%s: failed to link L1 scheduler to L2 "
				"base scheduler\n", __func__);
		return PPA_FAILURE;
	}
	return PPA_SUCCESS;
}

/**
 * @brief: This function allocates the base scheduler and updates
 * the base sched db accordingly.
 */
static int32_t ppa_qos_sched_add(PPA_IFNAME *ifname)
{
	int32_t ret, port_idx, i;
	struct base_sched l1_sch;
	struct base_sched *l2_sch = NULL;
	struct base_sched *curr = NULL, *next = NULL;

	port_idx = ppa_qos_get_base_sch_info(ifname);
	if (port_idx == -1)
		return PPA_FAILURE;

	if (!base_sch_info[port_idx].sch_id) {
		/* Add L1 base scheduler */
		ret = ppa_qos_add_base_sched(port_idx, NULL);
		if (ret != PPA_SUCCESS) {
			ppa_debug(DBG_ENABLE_MASK_ERR, "%s: Failed to add base scheduler"
					" for %s\n", __func__, ifname);
			return ret;
		}
	} else if ((base_sch_info[port_idx].lif_count + 1) >
				PPA_QOS_MAX_VAP_PER_SCH) {
		/* If TC-QoS is configured on more than 8 VAPs:
		 * Create new L1 scheduler and link it to the port, and move previously
		 * existing L1 sched to L2 level and then link it to the new L1 sched.
		 * Add L2 schedulers for new VAP's and link them to new L1 scheduler.
		 */
		if (list_empty(&base_sch_info[port_idx].l2_base_sched)) {
			memcpy(&l1_sch, &base_sch_info[port_idx],
					sizeof(struct base_sched));

			ret = ppa_qos_add_base_sched(port_idx, NULL);
			if (ret != PPA_SUCCESS) {
				ppa_debug(DBG_ENABLE_MASK_ERR, "%s: Failed to add L1 base"
						" scheduler\n", __func__);
				return ret;
			}

			/* Move current L1 scheduler to L2 level */
			l2_sch = (struct base_sched *)ppa_malloc(sizeof(struct base_sched));
			if (l2_sch == NULL) {
				ppa_debug(DBG_ENABLE_MASK_ERR, "%s: Failed to alloc memory for "
						"l2_sch\n", __func__);
				return PPA_FAILURE;
			}
			memset(l2_sch, 0, sizeof(struct base_sched));

			for (i = 0; i < PPA_QOS_MAX_VAP_PER_SCH; i++) {
				strncpy(l2_sch->lif_name[i],
						base_sch_info[port_idx].lif_name[i], PPA_IF_NAME_SIZE);
			}
			l2_sch->sch_id = l1_sch.sch_id;
			l2_sch->arbi = l1_sch.arbi;
			l2_sch->prio_wfq = l1_sch.prio_wfq;
			l2_sch->lif_count = l1_sch.lif_count;
			base_sch_info[port_idx].l2_base_sch_cnt++;
			/* Add L2 scheduler to l2_base_sched list */
			list_add(&l2_sch->list, &base_sch_info[port_idx].l2_base_sched);

			/* Link L2 base scheduler to new L1 base scheduler */
			ret = ppa_qos_link_internal_sched(port_idx, l2_sch, false);
			if (ret != PPA_SUCCESS) {
				ppa_debug(DBG_ENABLE_MASK_ERR, "%s: Failed to link old sch: %d "
						"to new L1 sch: %d\n", __func__, l2_sch->sch_id,
						base_sch_info[port_idx].sch_id);
				ppa_list_del(&l2_sch->list);
				ppa_free(l2_sch);
				return ret;
			}
		}

		/* Check for available slots in current L2 schedulers for new LIF
		 * scheduler creation.
		 */
		l2_sch = NULL;
		ppa_list_for_each_entry_safe(curr, next,
				&base_sch_info[port_idx].l2_base_sched, list) {
			if (curr->lif_count < PPA_QOS_MAX_VAP_PER_SCH) {
				l2_sch = curr;
				break;
			}
		}

		/* Add new L2 base scheduler if no slot is available on existing
		 * L2 base schedulers.
		 */
		if (l2_sch == NULL) {
			l2_sch = (struct base_sched *)ppa_malloc(sizeof(struct base_sched));
			if (l2_sch == NULL) {
				ppa_debug(DBG_ENABLE_MASK_ERR, "%s: Failed to alloc memory for "
						"l2_sch\n", __func__);
				return PPA_FAILURE;
			}
			memset(l2_sch, 0, sizeof(struct base_sched));

			ret = ppa_qos_add_base_sched(port_idx, l2_sch);
			if (ret != PPA_SUCCESS) {
				ppa_debug(DBG_ENABLE_MASK_ERR, "%s: Failed to add base "
						"scheduler for %s\n", __func__, ifname);
				ppa_free(l2_sch);
				return ret;
			}
			base_sch_info[port_idx].l2_base_sch_cnt++;
		}
		/* Add LIF to L2 base scheduler */
		for (i = 0; i < PPA_QOS_MAX_VAP_PER_SCH; i++) {
			if (l2_sch->lif_name[i][0] == '\0') {
				strncpy(l2_sch->lif_name[i], ifname, PPA_IF_NAME_SIZE);
				l2_sch->lif_count++;
				break;
			}
		}
	}
	return PPA_SUCCESS;
}

/**
 * @brief: This function deletes the base scheduler if no logical
 * interfaces are present.
 */
static int32_t ppa_qos_del_base_sched(int32_t sch_id)
{
	struct dp_node_link node = {0};
	struct dp_node_alloc anode = {0};
	int32_t ret;

	node.node_type = DP_NODE_SCH;
	node.node_id.sch_id = sch_id;
	ret = dp_node_unlink(&node, 0);
	if (ret == DP_FAILURE) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "%s: sched id %d unlink failed\n",
				__func__, sch_id);
		return PPA_FAILURE;
	}

	anode.type = DP_NODE_SCH;
	anode.id.sch_id = sch_id;
	ret = dp_node_free(&anode, DP_NODE_AUTO_FREE_RES);
	if (ret == DP_FAILURE) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "%s: sched id %d free failed\n",
				__func__, sch_id);
		return PPA_FAILURE;
	}
	return PPA_SUCCESS;
}

/**
 * @brief: This function handles the deletion of a scheduler for a given LIF.
 * - Deletes the base scheduler if no logical interfaces are present.
 * - Updates the base scheduler information.
 */
static int32_t ppa_qos_sched_del(PPA_IFNAME *ifname)
{
	int32_t port_idx, lif_idx, i, ret = 0;
	struct base_sched *l2_sch = NULL;

	port_idx = ppa_qos_get_base_sch_info(ifname);
	if (port_idx == -1)
		return PPA_FAILURE;

	if (base_sch_info[port_idx].lif_count == 1) {
		/* No QoS configuration on LIF's, delete L1 base scheduler */
		ret = ppa_qos_del_base_sched(base_sch_info[port_idx].sch_id);
		if (ret != PPA_SUCCESS)
			return ret;
		memset(&base_sch_info[port_idx], 0, sizeof(struct base_sched));
		PPA_LIST_HEAD_INIT(&base_sch_info[port_idx].l2_base_sched);
	} else if (!list_empty(&base_sch_info[port_idx].l2_base_sched)) {
		/* Remove LIF from associated L2 scheduler.
		 * If no LIF schedulers are linked, delete the L2 internal scheduler.
		 * If there is only one L2 scheduler, link it to the port-level
		 * scheduler and delete the L1 scheduler.
		 */
		l2_sch = ppa_qos_get_l2_sched_info(ifname, port_idx);
		if (l2_sch == NULL)
			return PPA_FAILURE;
		for (i = 0; i < PPA_QOS_MAX_VAP_PER_SCH; i++) {
			if (strcmp(ifname, l2_sch->lif_name[i]) == 0) {
				memset(l2_sch->lif_name[i], '\0', PPA_IF_NAME_SIZE);
				l2_sch->lif_count--;
				break;
			}
		}
		if (l2_sch->lif_count == 0) {
			if (ppa_qos_del_base_sched(l2_sch->sch_id) == PPA_SUCCESS) {
				ppa_list_del(&l2_sch->list);
				ppa_free(l2_sch);
				base_sch_info[port_idx].l2_base_sch_cnt--;
			}
			if (base_sch_info[port_idx].l2_base_sch_cnt == 1) {
				l2_sch = list_first_entry(&base_sch_info[port_idx].l2_base_sched,
						struct base_sched, list);
				if (l2_sch == NULL)
					return ret;
				ppa_qos_link_internal_sched(port_idx, l2_sch, true);
				ret = ppa_qos_del_base_sched(base_sch_info[port_idx].sch_id);
				if (ret != PPA_SUCCESS)
					return PPA_FAILURE;
				base_sch_info[port_idx].sch_id = l2_sch->sch_id;
				base_sch_info[port_idx].l2_base_sch_cnt = 0;
				ppa_list_del(&l2_sch->list);
				ppa_free(l2_sch);
			}
		}
	}
	for (lif_idx = 0; lif_idx < PPA_QOS_MAX_SUPPORTED_LIF; lif_idx++) {
		if (strncmp(ifname, base_sch_info[port_idx].lif_name[lif_idx], IFNAMSIZ) == 0) {
			memset(base_sch_info[port_idx].lif_name[lif_idx], '\0', IFNAMSIZ);
			base_sch_info[port_idx].lif_count--;
		}
	}

	return ret;
}

/**
 * @brief: This function populates the `tc_params` structure required
 * for adding or deleting schedulers in qos-tc.
 */
static void ppa_qos_fill_tc_params(PPA_IFNAME *ifname, struct qos_tc_params *tc_params)
{
	int32_t port_idx;
	struct base_sched *l2_sch = NULL;
	dp_subif_t *subif = NULL;

	port_idx = ppa_qos_get_base_sch_info(ifname);
	if (port_idx == -1)
		return;

	subif = &base_sch_info[port_idx].subif;
	l2_sch = ppa_qos_get_l2_sched_info(ifname, port_idx);
	tc_params->sch_id = l2_sch ? l2_sch->sch_id :
			base_sch_info[port_idx].sch_id;

	if (ppa_qos_base_sch_type == PPA_QOS_BASE_SCH_DRR)
		tc_params->prio_wfq = QOS_MAX_CHILD_WRR_WEIGHT;
	else
		tc_params->prio_wfq = (l2_sch) ? l2_sch->lif_count :
				base_sch_info[port_idx].lif_count;
	tc_params->port_id = subif->port_id;
	tc_params->deq_idx = -1;
	tc_params->cqm_deq_port = base_sch_info[port_idx].cqm_deq_port;
	tc_params->arbi = base_sch_info[port_idx].arbi;
	tc_params->flags = QOS_TC_LINK_SCH | QOS_TC_IS_LIF_CONFIG;
	tc_params->dp_alloc_flag = subif->alloc_flag;
	if (subif->subif_common.num_q == 1)
		tc_params->def_q = subif->subif_common.def_qlist[0];
	else
		tc_params->def_q = -1;
}

int32_t ppa_qos_get_base_sch_id(void)
{
	return base_sch_info[0].sch_id;
}

/**
 * @brief: This function handles the following tasks.
 * - Validates the max supported logical interfaces.
 * - Updates base scheduler info with subif details.
 * - Retrieves subif info from the base scheduler db if it already exists,
 *  otherwise, it fetches from DP and updates base scheduler info.
 */
static int32_t ppa_qos_update_base_sch_info(enum tc_setup_type type, void *type_data,
		PPA_IFNAME *ifname)
{
	struct netif_info *ifinfo = NULL;
	dp_subif_t *subif = NULL;
	struct net *net = NULL;
	PPA_NETIF *base_dev = NULL;
	int32_t port_idx, lif_idx, ret;
	int8_t max_qdisc;

	/* Check if subif info is already exist in the base_sch_info */
	port_idx = ppa_qos_get_base_sch_info(ifname);
	if (port_idx != -1)
		return PPA_SUCCESS;

	if (ppa_get_netif_info(ifname, &ifinfo) != PPA_SUCCESS) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "Failed to get netif info"
				" for %s\n", ifname);
		return PPA_FAILURE;
	}

	net = ppa_get_current_net_ns();
	if ((net != NULL) && (ifinfo != NULL)) {
		base_dev = ppa_get_netif_by_net(net, ifinfo->phys_netif_name);
	} else {
		ppa_debug(DBG_ENABLE_MASK_ERR, "Failed to get base_dev for %s\n",
				ifname);
		ret = PPA_FAILURE;
		goto TC_QOS_DONE;
	}

	subif = ppa_malloc(sizeof(dp_subif_t));
	if (!subif) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "ppa_malloc failed for %s\n", ifname);
		ret = PPA_FAILURE;
		goto TC_QOS_DONE;
	}
	memset(subif, 0, sizeof(dp_subif_t));
	ret = dp_get_netif_subifid(base_dev, NULL, NULL, 0, subif, 0);
	if (ret != DP_SUCCESS) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "%s: dp_deq_port_res_get failed\n",
				__func__);
		goto TC_QOS_DONE;
	}

	for (port_idx = 0; port_idx < PPA_QOS_MAX_PHY_PORT; port_idx++) {
		if (subif->port_id == base_sch_info[port_idx].subif.port_id)
			break;
	}

	if (port_idx >= PPA_QOS_MAX_PHY_PORT) {
		port_idx = ppa_qos_get_base_sch_free_slot();
		if (port_idx == -1) {
			ppa_debug(DBG_ENABLE_MASK_ERR, "%s: No free slot found in"
					" base_sch_info\n", __func__);
			ret = PPA_FAILURE;
			goto TC_QOS_DONE;
		}
	}

	/* Dont't allow qdisc creation on LIF's if it exceeds max supported range */
	if (subif->alloc_flag & (DP_F_FAST_WLAN | DP_F_FAST_WLAN_EXT))
		max_qdisc = PPA_QOS_MAX_SUPPORTED_LIF;
	else
		max_qdisc = PPA_QOS_MAX_LIF_SCHED;
	if (base_sch_info[port_idx].lif_count == max_qdisc) {
		pr_info("WARNING: Max supported qdiscs on internal scheduler is %d\n",
				max_qdisc);
		ret = PPA_FAILURE;
		goto TC_QOS_DONE;
	}

	/* Update base scheduler info */
	memcpy(&base_sch_info[port_idx].subif, subif, sizeof(dp_subif_t));
	for (lif_idx = 0; lif_idx < max_qdisc; lif_idx++) {
		if (base_sch_info[port_idx].lif_name[lif_idx][0] == '\0') {
			strncpy(base_sch_info[port_idx].lif_name[lif_idx], ifname,
					PPA_IF_NAME_SIZE);
			base_sch_info[port_idx].lif_count++;
			break;
		}
	}
	ret = PPA_SUCCESS;

TC_QOS_DONE:
	ppa_netif_put(ifinfo);
	if (subif)
		ppa_free(subif);
	return ret;
}

static u32 ppa_qos_get_qdisc_handle(enum tc_setup_type type, void *type_data)
{
	u32 handle = 0;

	switch (type) {
	case TC_SETUP_QDISC_PRIO:
		handle = ((struct tc_prio_qopt_offload *)type_data)->handle;
		break;
	case TC_SETUP_QDISC_DRR:
		handle = ((struct tc_drr_qopt_offload *)type_data)->handle;
		break;
	case TC_SETUP_QDISC_TBF:
		handle = ((struct tc_tbf_qopt_offload *)type_data)->handle;
		break;
	default:
		/* For other qdisc types, handle might not be available */
		break;
	}

	return handle;
}

int32_t ppa_qos_tc_setup(struct net_device *dev, enum tc_setup_type type,
		void *type_data)
{
	struct qos_tc_params tc_params = {0};
	struct netif_info *p_ifinfo = NULL;
	int32_t i, ret = PPA_FAILURE;
	u32 handle = 0;
	bool handle_found = false;

	if (ppa_get_netif_info(dev->name, &p_ifinfo) != PPA_SUCCESS || !p_ifinfo) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "%s: Failed to get p_ifinfo for %s\n",
				__func__, dev->name);
		return PPA_FAILURE;
	}

	if (!p_ifinfo->enable && !p_ifinfo->is_tc_configured) {
		ret = PPA_FAILURE;
		goto TC_QOS_DONE;
	}

	if (ppa_qos_is_sched_add_config(type, type_data)) {
		/* Extract handle from type_data based on setup type */
		handle = ppa_qos_get_qdisc_handle(type, type_data);

		/* Check if this handle was already counted */
		if (handle != 0) {
			for (i = 0; i < PPA_QOS_MAX_CHILD_QDISC; i++) {
				if (p_ifinfo->tc_qdisc_handles[i] == handle) {
					handle_found = true;
					break;
				}
			}
		}

		ret = ppa_qos_update_base_sch_info(type, type_data, dev->name);
		if (ret != PPA_SUCCESS)
			goto TC_QOS_DONE;

		ret = ppa_qos_sched_add(dev->name);
		if (ret != PPA_SUCCESS) {
			ppa_debug(DBG_ENABLE_MASK_ERR, "Failed to add base scheduler for"
					" %s\n", dev->name);
			goto TC_QOS_DONE;
		}

		/* Only increment counter if handle is new */
		if (!handle_found) {
			/* Find an empty slot and store the handle */
			if (handle != 0 && p_ifinfo->tc_qdisc_cnt <
					PPA_QOS_MAX_CHILD_QDISC) {
				for (i = 0; i < PPA_QOS_MAX_CHILD_QDISC; i++) {
					if (p_ifinfo->tc_qdisc_handles[i] == 0) {
						p_ifinfo->tc_qdisc_handles[i] = handle;
						break;
					}
				}
			}
			p_ifinfo->tc_qdisc_cnt++;
		}
		p_ifinfo->is_tc_configured = true;
	}

	/* Fill tc_params that needs to be sent to qos-tc. */
	ppa_qos_fill_tc_params(dev->name, &tc_params);
	/* CoDel is not supported on PON vUNI due to FSQM buffer issues */
	if (tc_params.dp_alloc_flag & DP_F_VUNI && type == TC_SETUP_QDISC_CODEL) {
		netdev_err(dev, "CoDel is not supported on PON vUNI interface\n");
		goto TC_QOS_DONE;
	}
	ret = qos_tc_setup_ext(dev, type, type_data, &tc_params);
	if (ret != 0)
		goto TC_QOS_DONE;


	if (ppa_qos_is_sched_del_config(type, type_data)) {
		/* Extract handle for deletion */
		handle = ppa_qos_get_qdisc_handle(type, type_data);

		/* Remove handle from tracking array */
		if (handle != 0) {
			for (i = 0; i < PPA_QOS_MAX_CHILD_QDISC; i++) {
				if (p_ifinfo->tc_qdisc_handles[i] == handle) {
					/* Mark slot as empty */
					p_ifinfo->tc_qdisc_handles[i] = 0;
					break;
				}
			}
		}

		if (p_ifinfo->tc_qdisc_cnt == 1) {
			ret = ppa_qos_sched_del(dev->name);
			if (ret != PPA_SUCCESS) {
				ppa_debug(DBG_ENABLE_MASK_ERR, "Failed to delete base scheduler"
					" for %s\n", dev->name);
				goto TC_QOS_DONE;
			}
		}
		p_ifinfo->tc_qdisc_cnt--;
		if (!p_ifinfo->tc_qdisc_cnt)
			p_ifinfo->is_tc_configured = false;
	}

TC_QOS_DONE:
	ppa_netif_put(p_ifinfo);
	return ret;
}

static void ppa_shutdown_scheduler_queue(struct net_device *dev,
		struct netdev_queue *dev_queue,
		void *_qdisc_default)
{
	struct Qdisc *qdisc = dev_queue->qdisc_sleeping;
	struct Qdisc *qdisc_default = _qdisc_default;

	if (qdisc) {
		rcu_assign_pointer(dev_queue->qdisc, qdisc_default);
		dev_queue->qdisc_sleeping = qdisc_default;

		qdisc_put(qdisc);
	}
}

static inline void ppa_netdev_for_each_tx_queue(struct net_device *dev,
		void (*f)(struct net_device *, struct netdev_queue *, void *),
		void *arg)
{
	unsigned int i;

	for (i = 0; i < dev->num_tx_queues; i++)
		f(dev, &dev->_tx[i], arg);
}

static void ppa_tc_qos_cleanup(struct net_device *dev)
{
	ppa_netdev_for_each_tx_queue(dev, ppa_shutdown_scheduler_queue,
			&noop_qdisc);
	if (dev_ingress_queue(dev))
		ppa_shutdown_scheduler_queue(dev, dev_ingress_queue(dev),
				&noop_qdisc);
	qdisc_put(rtnl_dereference(dev->qdisc));
	rcu_assign_pointer(dev->qdisc, &noop_qdisc);

	WARN_ON(timer_pending(&dev->watchdog_timer));
}

static bool ppa_qos_is_netif_wlan(int32_t inst, int32_t dpid)
{
	struct dp_port_prop port_prop = {0};

	if (dp_get_port_prop(inst, dpid, &port_prop)) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "dp_get_port_prop fail\n");
		return PPA_FAILURE;
	}

	if (port_prop.alloc_flags & (DP_F_FAST_WLAN | DP_F_FAST_WLAN_EXT))
		return true;

	return false;
}

static int32_t ppa_qos_wlan_set_tc_ops(struct dp_event_info *info)
{

	if (!ppa_qos_is_netif_wlan(info->inst, info->reg_subif_info.dpid))
		return PPA_SUCCESS;

	dp_set_net_dev_ops_priv(info->reg_subif_info.dev, ppa_qos_tc_setup,
		offsetof(const struct net_device_ops, ndo_setup_tc),
		DP_OPS_NETDEV, "non-dpm");
	info->reg_subif_info.dev->features |= NETIF_F_HW_TC;

	return PPA_SUCCESS;
}

static int32_t ppa_qos_wlan_reset_tc_ops(struct dp_event_info *info)
{
	int32_t ret = PPA_SUCCESS;
	struct netif_info *p_ifinfo = NULL;

	if (!ppa_qos_is_netif_wlan(info->inst, info->de_reg_subif_info.dpid))
		return PPA_SUCCESS;

	if (ppa_get_netif_info(info->de_reg_subif_info.dev->name, &p_ifinfo)
			!= PPA_SUCCESS) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "Failed to get netif info for %s\n",
			info->de_reg_subif_info.dev->name);
		return PPA_FAILURE;
	}
	/* Skip TC-QoS cleanup if the interface is not shutting down */
	if (!(p_ifinfo->flags & NETIF_GOING_DOWN)) {
		ppa_netif_put(p_ifinfo);
		return PPA_SUCCESS;
	}

	/* Teardown tc qdisc and reset tc-ops */
	ppa_tc_qos_cleanup(info->de_reg_subif_info.dev);
	dp_set_net_dev_ops_priv(info->de_reg_subif_info.dev, NULL,
		offsetof(const struct net_device_ops, ndo_setup_tc),
		DP_OPS_NETDEV | DP_OPS_RESET, "non-dpm");
	info->dereg_dev_info.dev->features &= ~NETIF_F_HW_TC;

	ppa_netif_put(p_ifinfo);
	return ret;
}

/**
 * @brief: Retrieves the index of a free slot or the backup QoS index
 *   from wlan_qos_temp:
 * - Pass zero as dp_port to get the index of a free available slot.
 * - Pass a valid dp_port to retrieve the backup QoS index for the WLAN.
 */
static int32_t ppa_api_get_wlan_qos_backup_idx(int32_t dp_port)
{
	int idx;

	for (idx = 0; idx < PPA_QOS_MAX_WIFI_PORT; idx++) {
		if (wlan_qos_temp[idx].dq_res.dp_port == dp_port)
			return idx;
	}

	return idx;
}

static int32_t ppa_api_backup_wlan_qos_config(int32_t sch_info_idx,
		int32_t wlan_qos_idx, struct pp_qos_dev *qdev)
{
	struct pp_qos_sched_conf sch_conf = {0};

	/* Get the queue properties and store them in wlan_qos_temp */
	memset(&wlan_qos_temp[wlan_qos_idx].dq_res, 0,
		sizeof(struct dp_dequeue_res));
	wlan_qos_temp[wlan_qos_idx].dq_res.dp_port =
		base_sch_info[sch_info_idx].subif.port_id;
	wlan_qos_temp[wlan_qos_idx].dq_res.cqm_deq_idx = DEQ_PORT_OFFSET_ALL;
	if (dp_deq_port_res_get(&wlan_qos_temp[wlan_qos_idx].dq_res, 0) != 0) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "ERROR Failed to Get QOS Resources\n");
		return PPA_FAILURE;
	}

	wlan_qos_temp[wlan_qos_idx].dq_res.q_res =
		kzalloc(sizeof(struct dp_queue_res) *
		wlan_qos_temp[wlan_qos_idx].dq_res.num_q, GFP_KERNEL);
	if (!wlan_qos_temp[wlan_qos_idx].dq_res.q_res) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "ERROR Malloc failed for q res\n");
		return PPA_FAILURE;
	}

	wlan_qos_temp[wlan_qos_idx].dq_res.q_res_size =
		wlan_qos_temp[wlan_qos_idx].dq_res.num_q;
	if (dp_deq_port_res_get(&wlan_qos_temp[wlan_qos_idx].dq_res, 0) != 0) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "ERROR Failed to Get QOS Resources\n");
		goto free_q_res;
	}

	/* Get the root scheduler prperties and store them in wlan_qos_temp */
	if (pp_qos_sched_conf_get(qdev, base_sch_info[sch_info_idx].sch_id,
			&sch_conf) != 0) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "ERROR: pp_qos_sched_conf_get failed\n");
		goto free_q_res;
	}
	memcpy(&wlan_qos_temp[wlan_qos_idx].common_prop, &sch_conf.common_prop,
		sizeof(struct pp_qos_common_node_properties));
	memcpy(&wlan_qos_temp[wlan_qos_idx].sched_child_prop,
		&sch_conf.sched_child_prop,
		sizeof(struct pp_qos_child_node_properties));

	return PPA_SUCCESS;

free_q_res:
	kfree(wlan_qos_temp[wlan_qos_idx].dq_res.q_res);
	memset(&wlan_qos_temp[wlan_qos_idx], 0, sizeof(struct wlan_qos_backup));
	return PPA_FAILURE;
}

static int32_t ppa_api_flush_and_block_queues(int32_t wlan_qos_idx)
{
	int32_t i;
	struct dp_qos_blk_flush_queue flush_q_cfg = {0};

	flush_q_cfg.inst = 0;
	for (i = 0; i < wlan_qos_temp[wlan_qos_idx].dq_res.num_q; i++) {
		flush_q_cfg.q_id = wlan_qos_temp[wlan_qos_idx].dq_res.q_res[i].q_id;
		dp_block_flush_queue(&flush_q_cfg, DP_QFLUSH_FLAG_CONTINUE_BLOCK_QUEUE |
			DP_QFLUSH_FLAG_NO_LOCK);
	}

	return PPA_SUCCESS;
}

static int32_t ppa_api_link_root_scheduler_to_drop_port(int32_t sch_info_idx,
		int32_t wlan_qos_idx, struct pp_qos_dev *qdev)
{
	struct dp_qos_q_logic q_logic = {0};
	struct pp_qos_queue_conf q_conf = {0};
	struct pp_qos_sched_conf sch_conf = {0};

	q_logic.q_id = 0; /* zero is the drop queue */
	if (dp_qos_get_q_logic(&q_logic, 0) == DP_FAILURE) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "ERROR failed to get logical queue\n");
		return PPA_FAILURE;
	}

	if (pp_qos_queue_conf_get(qdev, q_logic.q_logic_id, &q_conf) != 0) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "ERROR: pp_qos_queue_conf_get failed\n");
		return PPA_FAILURE;
	}
	memcpy(&sch_conf.common_prop, &q_conf.common_prop,
		sizeof(struct pp_qos_common_node_properties));
	memcpy(&sch_conf.sched_child_prop, &q_conf.queue_child_prop,
		sizeof(struct pp_qos_child_node_properties));
	if (pp_qos_sched_set(qdev, base_sch_info[sch_info_idx].sch_id,
			&sch_conf) != 0) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "ERROR: pp_qos_sched_set failed\n");
		return PPA_FAILURE;
	}

	return PPA_SUCCESS;
}

static int32_t ppa_api_get_base_sched_idx_by_dp_port(int32_t dp_port)
{
	int32_t idx;

	for (idx = 0; idx < PPA_QOS_MAX_PHY_PORT; idx++) {
		if (base_sch_info[idx].subif.port_id == dp_port)
			break;
	}
	return idx;
}

/**
 * @brief: Handles WiFi recovery by:
 * - Backing up WLAN interface QoS configuration.
 * - Getting drop port details based on drop queue (0).
 * - Flushing and blocking user-configured queues.
 * - Linking the root scheduler of the WLAN interface to the drop port.
 */
static int32_t ppa_api_move_wmm_qos_to_drop_port(struct dp_event_info *info)
{
	int32_t ret;
	int32_t sch_info_idx, wlan_qos_idx;
	struct pp_qos_dev *qdev = NULL;

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(!qdev)) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "pp_qos_dev_open Failed\n");
		return PPA_FAILURE;
	}

	if (!ppa_qos_is_netif_wlan(info->inst, info->dereg_dev_info.dpid))
		return PPA_SUCCESS;

	/* Find the base_sch_info index for the given dp port */
	sch_info_idx = ppa_api_get_base_sched_idx_by_dp_port(
		info->dereg_dev_info.dpid);
	if (sch_info_idx == PPA_QOS_MAX_PHY_PORT) {
		/* No user QoS is configured on WLAN interface, return */
		return PPA_SUCCESS;
	}

	/* Get the available slot in wlan_qos_temp for temporary WLAN QoS storage */
	wlan_qos_idx = ppa_api_get_wlan_qos_backup_idx(0);
	if (wlan_qos_idx == PPA_QOS_MAX_WIFI_PORT) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "No free slot found in wlan_qos_temp\n");
		return PPA_FAILURE;
	}

	ret = ppa_api_backup_wlan_qos_config(sch_info_idx, wlan_qos_idx, qdev);
	if (ret != PPA_SUCCESS)
		return ret;

	/* Flush and block all user-queues before linking them to the drop port */
	ret = ppa_api_flush_and_block_queues(wlan_qos_idx);
	if (ret != PPA_SUCCESS)
		goto free_q_res;

	/* Get drop port details and link root scheduler of the WLAN interface */
	ret = ppa_api_link_root_scheduler_to_drop_port(sch_info_idx, wlan_qos_idx,
		qdev);
	if (ret != PPA_SUCCESS)
		goto free_q_res;

	return PPA_SUCCESS;

free_q_res:
	kfree(wlan_qos_temp[wlan_qos_idx].dq_res.q_res);
	memset(&wlan_qos_temp[wlan_qos_idx], 0, sizeof(struct wlan_qos_backup));
	return ret;
}

static int32_t ppa_api_link_scheduler_and_unblock_queues(int32_t sch_info_idx,
		int32_t wlan_qos_idx)
{
	int32_t i, ret;
	struct pp_qos_dev *qdev = NULL;
	struct dp_qos_q_logic q_logic = {0};
	struct pp_qos_sched_conf sch_conf = {0};

	qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (unlikely(!qdev)) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "pp_qos_dev_open Failed\n");
		return PPA_FAILURE;
	}

	/* Link the user-configured root scheduler to WLAN port */
	memcpy(&sch_conf.common_prop, &wlan_qos_temp[wlan_qos_idx].common_prop,
		sizeof(struct pp_qos_common_node_properties));
	memcpy(&sch_conf.sched_child_prop,
		&wlan_qos_temp[wlan_qos_idx].sched_child_prop,
		sizeof(struct pp_qos_child_node_properties));
	ret = pp_qos_sched_set(qdev, base_sch_info[sch_info_idx].sch_id, &sch_conf);
	if (ret != PPA_SUCCESS) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "ERROR pp_qos_sched_set failed\n");
		return PPA_FAILURE;
	}

	/* Unblock all user-configured queues */
	for (i = 0; i < wlan_qos_temp[wlan_qos_idx].dq_res.num_q; i++) {
		q_logic.q_id = wlan_qos_temp[wlan_qos_idx].dq_res.q_res[i].q_id;
		if (dp_qos_get_q_logic(&q_logic, 0) == DP_FAILURE) {
			ppa_debug(DBG_ENABLE_MASK_ERR, "ERROR dp_qos_get_q_logic failed\n");
			return PPA_FAILURE;
		}
		if (pp_qos_queue_unblock(qdev, q_logic.q_logic_id)) {
			ppa_debug(DBG_ENABLE_MASK_ERR, "pp_qos_queue_unblock qid:%d fail\n",
				q_logic.q_logic_id);
			return PPA_FAILURE;
		}
	}

	return PPA_SUCCESS;
}

/**
 * @brief: Restores WLAN interface QoS configuration by:
 * - Retrieving backed-up QoS configuration.
 * - Linking the root scheduler of the WLAN interface to the original port.
 * - Unblocking user-configured queues.
 */
static int32_t ppa_api_restore_wmm_qos(struct dp_event_info *info)
{
	int32_t ret;
	int32_t sch_info_idx, wlan_qos_idx;

	if (!ppa_qos_is_netif_wlan(info->inst, info->reg_dev_info.dpid))
		return PPA_SUCCESS;

	/* Find the base_sch_info index for the given dp port */
	sch_info_idx = ppa_api_get_base_sched_idx_by_dp_port(
		info->reg_dev_info.dpid);
	if (sch_info_idx == PPA_QOS_MAX_PHY_PORT) {
		/* No user QoS is configured on WLAN interface, return */
		return PPA_SUCCESS;
	}

	/* Get the wlan_qos_temp index for the specified WLAN dp port */
	wlan_qos_idx = ppa_api_get_wlan_qos_backup_idx(info->reg_dev_info.dpid);
	if (wlan_qos_idx == PPA_QOS_MAX_WIFI_PORT) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "No backup QoS found for dpid: %d\n",
			info->reg_dev_info.dpid);
		return PPA_FAILURE;
	}

	ret = ppa_api_link_scheduler_and_unblock_queues(sch_info_idx, wlan_qos_idx);
	if (ret != PPA_SUCCESS)
		return ret;

	kfree(wlan_qos_temp[wlan_qos_idx].dq_res.q_res);
	memset(&wlan_qos_temp[wlan_qos_idx], 0, sizeof(struct wlan_qos_backup));

	return PPA_SUCCESS;
}

static int32_t ppa_api_dp_event_handler(struct dp_event_info *info)
{
	int32_t ret = PPA_SUCCESS;

	switch (info->type) {
	case DP_EVENT_REGISTER_SUBIF:
		ret = ppa_qos_wlan_set_tc_ops(info);
		break;

	case DP_EVENT_DE_REGISTER_SUBIF:
		ret = ppa_qos_wlan_reset_tc_ops(info);
		break;

	case DP_EVENT_REGISTER_DEV:
		ret = ppa_api_restore_wmm_qos(info);
		break;

	case DP_EVENT_DE_REGISTER_DEV:
		ret = ppa_api_move_wmm_qos_to_drop_port(info);
		break;

	default:
		ppa_debug(DBG_ENABLE_MASK_DEBUG_PRINT, "Unknown Event Type: %d",
			info->type);
		ret = PPA_FAILURE;
		break;
	}

	return ret;
}

/* Fill the DPM events details */
static struct dp_event dp_event = {
	.owner = DP_EVENT_OWNER_PPA,
	.type = DP_EVENT_REGISTER_SUBIF | DP_EVENT_DE_REGISTER_SUBIF |
			DP_EVENT_REGISTER_DEV | DP_EVENT_DE_REGISTER_DEV,
	.dp_event_cb = ppa_api_dp_event_handler,
};

void ppa_qos_tc_lif_info_init(void)
{
	int8_t idx;

	/* Initialize l2_base_sched list */
	for (idx = 0; idx < PPA_QOS_MAX_PHY_PORT; idx++)
		PPA_LIST_HEAD_INIT(&base_sch_info[idx].l2_base_sched);

	/* Register for DPM events */
	if (dp_register_event_cb(&dp_event, 0) != DP_SUCCESS) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "Can't register DP_EVENT callback");
		return;
	}
}

void ppa_qos_tc_lif_info_exit(void)
{
	/* De-register DPM events */
	if (dp_register_event_cb(&dp_event, DP_F_DEREGISTER) != DP_SUCCESS) {
		ppa_debug(DBG_ENABLE_MASK_ERR, "Can't de-register DP_EVENT callback");
		return;
	}
}
