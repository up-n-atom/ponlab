#ifndef __PPA_API_QOS_TC_LOGICAL_H__
#define __PPA_API_QOS_TC_LOGICAL_H__
/*************************************************************************
 *
 * FILE NAME	: ppa_api_qos_tc_logical.h
 * PROJECT	: LGM
 * MODULES	: PPA framework support for qos-tc.
 *
 * DESCRIPTION	: QoS-TC adaptation for Logical interfaces support.
 * COPYRIGHT	: Copyright (C) 2023-2024 MaxLinear Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder
 * of this software module.
 *
 * HISTORY
 * $Date                 $Author                 $Comment
 * 13 AUG 2023           Ramulu Komme            Initial version
 *
 **************************************************************************/

#if IS_ENABLED(CONFIG_QOS_TC)
#include <net/ppa/ppa_api.h>
#include <net/datapath_api.h>

#define PPA_QOS_MAX_SUPPORTED_LIF	32
#define PPA_QOS_MAX_PHY_PORT	4
#define PPA_QOS_MAX_LIF_SCHED	4
#define PPA_QOS_MAX_VAP_PER_SCH	8
#define PPA_QOS_MAX_WIFI_PORT 3

/* Base scheduler types */
enum ppa_qos_base_sched_type {
	PPA_QOS_BASE_SCH_DRR,
	PPA_QOS_BASE_SCH_WSP,
};

/* Forward declaration of tc_params */
struct qos_tc_params;

struct base_sched {
	/* CQM dequeue port-id */
	s32 cqm_deq_port;
	/* Base scheduler-id of the port, used to link new schedulers as children */
	s32 sch_id;
	/* Arbitration method of the new scheduler. */
	s32 arbi;
	/* node priority/weight of the new scheduler. */
	s32 prio_wfq;
	/* Indicates the total count of LIF's qdiscs linked to the phy port */
	s8 lif_count;
	/* Total count of level-2 base schedulers created on the phy port */
	s8 l2_base_sch_cnt;
	dp_subif_t subif;
	/* Maintains list of L2 schedulers, specifically used for WiFi interfaces */
	PPA_LIST_NODE list;
	PPA_LIST_HEAD l2_base_sched;
	/* Maintains the list of interfaces where QoS is configured */
	PPA_IFNAME lif_name[PPA_QOS_MAX_SUPPORTED_LIF][IFNAMSIZ];
};

/* ####################################
 *				Declaration
 * ####################################
 */

/*	variable
 */
extern enum ppa_qos_base_sched_type ppa_qos_base_sch_type;

/* Function declarations
 */
int32_t ppa_qos_tc_setup(struct net_device *dev, enum tc_setup_type type,
		void *type_data);
int32_t ppa_qos_get_base_sch_id(void);
void ppa_qos_tc_lif_info_init(void);
void ppa_qos_tc_lif_info_exit(void);
#endif /* CONFIG_QOS_TC */
#endif /* __PPA_API_QOS_TC_LOGICAL_H__ */
