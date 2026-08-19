/*****************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 * \file
 * This file contains functions that manage GEM Port Network CTP
 * and its assignment to a Priority Queue.
 *
 * Provided we have a following qdisc tree configured on the T-CONT network
 * device:
 *
 *      tc-prio handle 8000:
 *      |-- class:1
 *      |   `-- tc-red handle 1001:
 *      |       `-- tc-red handle 2001:
 *      |-- class:2
 *      |   `-- tc-red handle 1002:
 *      |       `-- tc-red handle 2002:
 *      |-- class:3
 *      |   `-- tc-red handle 1003:
 *      |       `-- tc-red handle 2003:
 *      |-- class:4
 *      |   `-- tc-red handle 1004:
 *      |       `-- tc-red handle 2004:
 *      |-- class:5
 *      |   `-- tc-red handle 1005:
 *      |       `-- tc-red handle 2005:
 *      |-- class:6
 *      |   `-- tc-red handle 1006:
 *      |       `-- tc-red handle 2006:
 *      |-- class:7
 *      |   `-- tc-red handle 1007:
 *      |       `-- tc-red handle 2007:
 *      `-- class:8
 *          `-- tc-red handle 1008:
 *              `-- tc-red handle 2008:
 *
 * When GEM Port Network CTP is created, it will be assigned to a queue using
 * a RT_NETLINK message to create tc-flower filter on the T-CONT network device.
 * The queue that GEM port is assigned to is specified by "classid" parameter.
 * It points to the leaf qdisc in the qdisc tree. Below is an example of
 * equivalent TC command:
 *
 *     tc filter add dev tcont32768 ingress handle 0x3 protocol all prio 103 \
 *         flower skip_sw classid 2001:1 indev gem2 action pass
 *
 * The minor number will redundantly specify queue number. Even if it is clear
 * from major number (the 2005: is attached to queue 5). This is required
 * for the qos_driver.
 *
 * As a result GEM port is assigned to 5th queue:
 *
 *      tc-prio handle 8000:
 *      |-- class:1
 *      |   `-- tc-red handle 1001:
 *      |       `-- tc-red handle 2001:
 *      |-- class:2
 *      |   `-- tc-red handle 1002:
 *      |       `-- tc-red handle 2002:
 *      |-- class:3
 *      |   `-- tc-red handle 1003:
 *      |       `-- tc-red handle 2003:
 *      |-- class:4
 *      |   `-- tc-red handle 1004:
 *      |       `-- tc-red handle 2004:
 *      |-- class:5
 *      |   `-- tc-red handle 1005:
 *      |       `-- tc-red handle 2005:      <----------  gem1
 *      |-- class:6
 *      |   `-- tc-red handle 1006:
 *      |       `-- tc-red handle 2006:
 *      |-- class:7
 *      |   `-- tc-red handle 1007:
 *      |       `-- tc-red handle 2007:
 *      `-- class:8
 *          `-- tc-red handle 1008:
 *              `-- tc-red handle 2008:
 *
 * If Traffic management option on ONU-G ME is set to "rate", then tc-tbf
 * qdiscs will be added as leaves and a single GEM will be connected to
 * multiple queues:
 *
 *      tc filter add dev tcont32768 ingress handle 0x1 protocol all prio 100 \
 *         flower skip_sw classid 3008:8 indev gem1 action pass \
 *         cookie 00000000000000000000000000000000
 *      tc filter add dev tcont32768 ingress handle 0x2 protocol all prio 101 \
 *         flower skip_sw classid 3006:6 indev gem1 action pass \
 *         cookie 00000000000000000000000000000002
 *      tc filter add dev tcont32768 ingress handle 0x3 protocol all prio 102 \
 *         flower skip_sw classid 3007:7 indev gem1 action pass \
 *         cookie 00000000000000000000000000000001
 *      tc filter add dev tcont32768 ingress handle 0x4 protocol all prio 103 \
 *         flower skip_sw classid 3004:4 indev gem1 action pass \
 *         cookie 00000000000000000000000000000004
 *      tc filter add dev tcont32768 ingress handle 0x5 protocol all prio 104 \
 *         flower skip_sw classid 3005:5 indev gem1 action pass \
 *         cookie 00000000000000000000000000000003
 *      tc filter add dev tcont32768 ingress handle 0x6 protocol all prio 105 \
 *         flower skip_sw classid 3003:3 indev gem1 action pass \
 *         cookie 00000000000000000000000000000005
 *      tc filter add dev tcont32768 ingress handle 0x7 protocol all prio 106 \
 *         flower skip_sw classid 3001:1 indev gem1 action pass \
 *         cookie 00000000000000000000000000000007
 *      tc filter add dev tcont32768 ingress handle 0x8 protocol all prio 107 \
 *         flower skip_sw classid 3002:2 indev gem1 action pass \
 *         cookie 00000000000000000000000000000006
 *
 * Which will result in the following configuration:
 *
 *      tc-prio handle 8000:
 *      |-- class:1
 *      |   `-- tc-red handle 1001:
 *      |       `-- tc-red handle 2001:
 *      |           `-- tc-tbf handle 3001:              <---------,
 *      |-- class:2                                                |
 *      |   `-- tc-red handle 1002:                                |
 *      |       `-- tc-red handle 2002:                            |
 *      |           `-- tc-tbf handle 3002:              <---------|
 *      |-- class:3                                                |
 *      |   `-- tc-red handle 1003:                                |
 *      |       `-- tc-red handle 2003:                            |
 *      |           `-- tc-tbf handle 3003:              <---------|
 *      |-- class:4                                                |
 *      |   `-- tc-red handle 1004:                                |
 *      |       `-- tc-red handle 2004:                            |
 *      |           `-- tc-tbf handle 3004:              <---------|--- gem1
 *      |-- class:5                                                |
 *      |   `-- tc-red handle 1005:                                |
 *      |       `-- tc-red handle 2005:                            |
 *      |           `-- tc-tbf handle 3005:              <---------|
 *      |-- class:6                                                |
 *      |   `-- tc-red handle 1006:                                |
 *      |       `-- tc-red handle 2006:                            |
 *      |           `-- tc-tbf handle 3006:              <---------|
 *      |-- class:7                                                |
 *      |   `-- tc-red handle 1007:                                |
 *      |       `-- tc-red handle 2007:                            |
 *      |           `-- tc-tbf handle 3007:              <---------|
 *      `-- class:8                                                |
 *          `-- tc-red handle 1008:                                |
 *              `-- tc-red handle 2008:                            |
 *                  `-- tc-tbf handle 3008:              <---------'
 */

#ifndef _PON_NET_GEM_PORT_NETWORK_CTP_H_
#define _PON_NET_GEM_PORT_NETWORK_CTP_H_

#include <stdint.h>
#include <stdbool.h>
#include "../pon_net_common.h"
#include "../pon_net_netlink.h"
#include "../pon_net_tc_filter.h"

struct pon_net_context;
struct pon_net_vlan_forwarding;

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/* This is the default value which is used if not configured by OMCI. */
#define PON_NET_GEM_PORT_MAX_FRAME_SIZE 4095
/* Minimum value accepted to avoid packet overhead,
 * in case the OLT sets a value which is too small.
 * Adapt this minimum value according to system requirements, if needed
 */
#define PON_NET_GEM_PORT_MIN_FRAME_SIZE 2048

/** This identifies the assignment of a GEM port to a priority queue in the
 *  driver
 */
struct queue_assignment {
	/** Whether this is an upstream or downstream assignment */
	bool us;
	/** ID of the Managed Entity representing the network device to which
	 *  the queue belongs. In case of the upstream assignment this is
	 *  the T-CONT me_id, and in case of the downstream this is the me_id
	 *  of PPTP Eth UNI Managed Entity **/
	uint16_t me_id;
	/** Queue identifier */
	uint16_t queue_id;
	/** ID of the Traffic Scheduler. If the GEM is attached directly to
	 *  the T-CONT, this is 0. If the GEM is attached to the
	 *  Traffic Scheduler, then this holds the me_id of the Traffic
	 *  Scheduler **/
	uint16_t traffic_scheduler_me_id;
	/** Value of priority_queue->index field */
	uint32_t priority_queue_index;
	/** Traffic Class */
	int traffic_class;
	/** Information about created tc filter */
	struct pon_net_tc_filter tc_filter;
};

/**
 * GEM can be mapped to multiple queues, so we have a dedicated struct that
 * represents these multiple assignments. In most cases, where GEM is mapped to
 * just one queue, 'num_assignments' is 1. When there are multiple mappings then
 * of course 'num_assignments' specifies number of valid entries in
 * 'assignments' array.
 *
 * If GEM is not assigned to a queue, then 'num_assignments' is 0.
 *
 * The entries in 'assignments' array are sorted by 'priority_queue_index',
 * (\see sort_by_pq_index()) so that we can compare them with single loop
 * (\see queue_assignment_list_eq())
 */
struct queue_assignment_list {
#define MAX_ASSIGNMENTS 8
	struct queue_assignment assignments[MAX_ASSIGNMENTS];
	unsigned int num_assignments;
};

/**
 * Structure storing the relevant attributes of
 * a GEM Port Network CTP Managed Entity
 */
struct pon_net_gem_port_net_ctp {
	/** GEM Port ID value */
	uint16_t port_id;

	/** Direction */
	uint8_t dir;

	/** Assignments to upstream queues */
	struct queue_assignment_list us;

	/** Assignments to downstream queues */
	struct queue_assignment_list ds;

	/** GEM's T-CONT. For a downstream GEM this is 0xFFFF */
	uint16_t tcont_me_id;

	/** Upstream Priority Queue ME ID */
	uint16_t us_priority_queue_me_id;

	/** Downstream Priority Queue ME ID */
	uint16_t ds_priority_queue_me_id;

	/** Traffic descriptor profile pointer for upstream */
	uint16_t us_td_me_id;

	/** Traffic descriptor profile pointer for downstream */
	uint16_t ds_td_me_id;

	/** GEM is connected to pmapper */
	unsigned int has_pmapper;

	/** Color marking configured on the GEM network interface */
	uint8_t color_marking;

	/** Current upstream police configuration applied to HW. */
	struct netlink_police_data us_police;

	/** Current downstream police configuration applied to HW. */
	struct netlink_police_data ds_police;
};

/**
 * Set GEM port max payload size
 *
 * \param[in] ctx		PON NET context pointer
 * \param[in] gem_port_me_id	GEM Port ME Id
 * \param[in] max_payload	Max payload size
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_gem_max_payload_size_set(struct pon_net_context *ctx,
				 uint16_t gem_port_me_id, uint16_t max_payload);

/** Get Ids of GEM Port Network CTP Managed Entities that are assigned
 *  to a given Managed Entity
 *
 * \param[in] ctx PON NET context pointer
 * \param[in] class_id Managed Entity Class ID
 * \param[in] me_id Managed Entity ID
 * \param[out] gem_ctp_me_ids Array of size 'capacity' containing
 *                           GEM CTP Managed Entity IDs
 * \param[in] capacity Array capacity
 *
 * \return number of GEM Port Network CTP Managed Entity IDs
 */
size_t pon_net_gem_me_ids_for_class_get(struct pon_net_context *ctx,
					uint16_t class_id,
					uint16_t me_id,
					uint16_t *gem_ctp_me_ids,
					unsigned int capacity);

/**
 * Inform GEM port network CTP ME when GEM is assigned or unassigned to pmapper.
 *
 * \param[in] ctx		PON NET context pointer
 * \param[in] gem_ctp_me_ids	List of pmapper gems
 * \param[in] status		GEM assigned or unassigned
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_gem_port_net_ctp_pmapper_assign(struct pon_net_context *ctx,
					uint16_t *gem_ctp_me_ids,
					bool status);

/** Unassign GEM ports from the queues
 *
 * To assign GEM ports again, according to the state stored in ctx->me_list,
 * you need to call \see pon_net_gem_queue_update()
 *
 * The 'dir' parameter describes, whether to unassign the upstream, downstream
 * or both kinds of queue mappings.
 *
 * \param[in] ctx PON NET context pointer
 * \param[in] dir One of following constants:
 *                - PA_GEMPORTNETCTP_DIR_DOWNSTREAM
 *                - PA_GEMPORTNETCTP_DIR_UPSTREAM
 *                - PA_GEMPORTNETCTP_DIR_BOTH
 * \param[in] gem_ctp_me_ids Array of size 'capacity' containing
 *                           GEM CTP Managed Entity IDs
 * \param[in] capacity Array capacity
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_gem_queue_unassign(struct pon_net_context *ctx,
			   enum netlink_filter_dir dir,
			   const uint16_t *gem_ctp_me_ids,
			   size_t capacity);

/**
 * Update GEM port queue assignments
 *
 * This function updates GEM port assignment to reflect the state stored
 * in ctx->me_list
 *
 * The 'dir' parameter describes, whether to update the upstream, downstream
 * or both kinds of queue mappings
 *
 * \param[in] ctx PON NET context pointer
 * \param[in] dir One of following constants:
 *                - PA_GEMPORTNETCTP_DIR_DOWNSTREAM
 *                - PA_GEMPORTNETCTP_DIR_UPSTREAM
 *                - PA_GEMPORTNETCTP_DIR_BOTH
 * \param[in] gem_ctp_me_ids Array of size 'capacity' containing
 *                           GEM CTP Managed Entity IDs
 * \param[in] capacity Array capacity
 *
 * \return returns value as follows:
 * - PON_ADAPTER_SUCCESS: If successful
 * - Other: An error code in case of error.
 */
enum pon_adapter_errno pon_net_gem_queue_update(struct pon_net_context *ctx,
						enum netlink_filter_dir dir,
						const uint16_t *gem_ctp_me_ids,
						size_t capacity);

/** Get T-CONTs for GEM Port Network CTP
 *
 * \param[in] ctx PON NET context pointer
 * \param[in] gem_ctp_me_ids Array of Managed Entity Ids of GEM Port Network CTP
 * \param[in] gem_ctp_me_ids_len Length of the array
 * \param[out] tcont_me_ids Managed Entity Id of T-CONT
 * \param[in] tcont_me_ids_capacity Capacity of output array
 *
 * \return number of T-CONT Managed Entity Ids in the tcont_me_ids array
 */
unsigned int pon_net_gem_tcont_me_ids_get(struct pon_net_context *ctx,
					  const uint16_t *gem_ctp_me_ids,
					  unsigned int gem_ctp_me_ids_len,
					  uint16_t *tcont_me_ids,
					  unsigned int tcont_me_ids_capacity);

/** @} */ /* PON_NET_LIB */
#endif
