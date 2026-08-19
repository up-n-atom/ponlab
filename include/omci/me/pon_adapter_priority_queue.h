/******************************************************************************
 *
 * Copyright (c) 2020 MaxLinear, Inc.
 * Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 *
 * This is a PON Adapter OMCI Managed Entity header file for the
 * Priority Queue.
 */

#ifndef _pon_adapter_me_priority_queue
#define _pon_adapter_me_priority_queue

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER_ME
 *
 *  @{
 */

/** \defgroup PON_ADAPTER_ME_PRIORITY_QUEUE Priority Queue
 *
 *  This Managed Entity specifies the priority queue used by a GEM port network
 *  CTP. In the upstream direction, a Priority Queue-G Managed Entity is also
 *  related to a T-CONT Managed Entity. By default, this relationship is fixed
 *  by the ONU hardware architecture, but some ONUs may also permit the
 *  relationship to be configured through OMCI, as indicated by the QoS
 *  configuration flexibility attribute of the ONU2-G Managed Entity.
 *
 *  In the downstream direction, priority queues are associated with UNIs.
 *  Again, the association is fixed by default, but some ONUs may permit the
 *  association to be configured through OMCI. The OLT can find all the queues
 *  by reading the priority queue-G Managed Entity instances. If the OLT tries
 *  to retrieve a non-existent priority queue, the ONU denies the get action
 *  with an error indication.
 *
 *  Upstream priority queues can be added to the ONU. Moreover, priority queues
 *  can exist in the ONU core and circuit packs serving both UNI and ANI
 *  functions.
 *
 *  In the upstream direction, the weight attribute permits configuring an
 *  optional traffic scheduler. Several attributes support back pressure
 *  operation, whereby a back pressure signal is sent backward and causes the
 *  attached terminal to temporarily suspend sending data.
 *
 *  The yellow packet drop thresholds are used to specify the packet drop
 *  probability for a packet that has been marked yellow (drop eligible) by a
 *  traffic descriptor or by external equipment such as a residential gateway.
 *  If the current queue occupancy is less than the min threshold, the yellow
 *  packet drop probability is zero. If the current queue occupancy is greater
 *  than or equal to the max threshold, the yellow packet drop probability is
 *  one. Otherwise, the yellow drop probability increases linearly between 0 and
 *  max_p as the current queue occupancy increases from the min to the max
 *  threshold.
 *
 *  Drop precedence color marking indicates the method by which a packet is
 *  marked as drop eligible (yellow). For DEI and PCP marking, a drop eligible
 *  indicator is equivalent to yellow color, otherwise the color is green.
 *  For DSCP AF marking, the lowest drop precedence is equivalent to green,
 *  otherwise the color is yellow.
 *
 *  Note: A Priority Queue will be identified in as "prio_q" function names.
 *
 *  @{
 */

/** Maximum Number of priority queues */
#define PON_ADAPTER_PRIORITY_QUEUE_MAX	255

/** GEM Port Direction
 *
 * \todo Move this enumeration to some common file.
 */
enum pa_gem_port_direction {
	/** Upstream only */
	PA_GEM_PORT_DIR_US = 1,
	/** Downstream only */
	PA_GEM_PORT_DIR_DS = 2,
	/** Both */
	PA_GEM_PORT_DIR_BOTH = 3
};

/** Priority Queue Direction */
enum pa_queue_direction {
	/** Upstream only */
	PA_QUEUE_DIR_US = 1,
	/** Downstream only */
	PA_QUEUE_DIR_DS = 2
};

#define PA_PQ_RELATED_MEID(related_port) (((related_port) >> 16) & 0xFFFF)
#define PA_PQ_RELATED_PRIO(related_port) ((related_port) & 0xFFFF)

/** Arguments to Priority Queue update and create */
struct pa_priority_queue_update_data {
	/** Discard-block counter reset interval */
	uint16_t discard_block_cnt_reset_interval;
	/** Threshold value for discarded blocks due to buffer overflow */
	uint16_t thr_value_for_discarded_blocks;
	/** Weight */
	uint8_t weight;
	/** Back pressure operation */
	uint16_t back_pressure_operation;
	/** Back pressure time */
	uint32_t back_pressure_time;
	/** Back pressure occur queue threshold */
	uint16_t back_pressure_occur_thr;
	/** Back pressure clear queue threshold */
	uint16_t back_pressure_clear_thr;
	/** Packet drop queue thresholds first value */
	uint16_t pkt_drop_q_thr_green_min;
	/** Packet drop queue thresholds second value */
	uint16_t pkt_drop_q_thr_green_max;
	/** Packet drop queue thresholds third value */
	uint16_t pkt_drop_q_thr_yellow_min;
	/** Packet drop queue thresholds fourth value */
	uint16_t pkt_drop_q_thr_yellow_max;
	/** The probability of dropping a green packet */
	uint8_t pkt_drop_probability_green;
	/** The probability of dropping a yellow packet */
	uint8_t pkt_drop_probability_yellow;
	/** Queue drop averaging coefficient */
	uint8_t q_drop_averaging_coefficient;
	/** Drop precedence color marking */
	uint8_t drop_precedence_color_marking;
	/** Traffic scheduler ptr */
	uint16_t traffic_scheduler_ptr;
	/** Related port */
	uint32_t related_port;
};

/** Priority_queue operations structure */
struct pa_priority_queue_ops {
	/** Get priority queue scale factor.

	   \param ll_handle               Lower layer context pointer
	   \param scale                   Scale factor

	*/
	enum pon_adapter_errno
	(*scale_get)(void *ll_handle, uint32_t *scale);

	/** Enable Managed Entity identifier mapping to driver index and
	   initialize corresponding driver structures

	   \param ll_handle                Lower layer context pointer
	   \param me_id                    Managed Entity identifier
	   \param update_data              Update data
	*/
	enum pon_adapter_errno
	(*create)(void *ll_handle,
		  const uint16_t me_id,
		  struct pa_priority_queue_update_data *update_data);

	/** Disable Managed Entity identifier mapping to driver index and clear
	   corresponding driver structures

	   \param ll_handle                Lower layer context pointer
	   \param me_id                    Managed Entity identifier
	*/
	enum pon_adapter_errno
	(*destroy)(void *ll_handle,
		   uint16_t me_id);

	/** Update Priority Queue resources

	   \param ll_handle                Lower layer context pointer
	   \param me_id                    Managed Entity identifier
	   \param update_data              Update data
	*/
	enum pon_adapter_errno
	(*update)(void *ll_handle,
		  const uint16_t me_id,
		  struct pa_priority_queue_update_data *update_data);

	/** Retrieve maximum queue size.
	    This value take the scaling factor of ONU2G into account.

	   \param ll_handle    Lower layer context pointer
	   \param me_id        Managed Entity identifier
	   \param data         Maximum queue size
	*/
	enum pon_adapter_errno
	(*maximum_queue_size_get)(void *ll_handle,
				  const uint16_t me_id,
				  uint16_t *data);

	/** Read the allocated queue size.

	   \param ll_handle    Lower layer context pointer
	   \param me_id        Managed Entity identifier
	   \param data         Alloc queue size
	*/
	enum pon_adapter_errno
	(*alloc_queue_size_get)(void *ll_handle,
				const uint16_t me_id,
				uint16_t *data);

	/** Read the queue drop threshold

	   \param ll_handle    Lower layer context pointer
	   \param me_id        Managed Entity identifier
	   \param data         Queue drop threshold
	*/
	enum pon_adapter_errno
	(*packet_drop_queue_threshold_get)(void *ll_handle,
					   const uint16_t me_id,
					   uint8_t *data);


	/** Get Drop Precedence Color Marking for a given LAN port and direction

	   \param ll_handle       Lower layer context pointer
	   \param direction       Direction, upstream or downstream
	   \param lan_port        LAN port index
	   \param color_marking   Related port

	*/
	enum pon_adapter_errno
	(*drop_precedence_color_marking_get)(void *ll_handle,
	       const enum pa_queue_direction direction,
	       const uint8_t lan_port,
	       uint8_t *color_marking);
};

/** @} */

/** @} */

#ifdef __cplusplus
}
#endif

#endif
