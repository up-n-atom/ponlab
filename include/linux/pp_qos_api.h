/*
 * Copyright (C) 2020-2025 MaxLinear, Inc.
 * Copyright (C) 2018-2020 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2, as published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR APARTICULARPURPOSE.See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public
 * License along with this program; if not,see
 * <http://www.gnu.org/licenses/>.
 * SPDX-License-Identifier: GPL-2.0-only or BSD-3-Clause
 *
 * Description: PP QoS APIs and definitions
 */

#ifndef __PP_QOS_API_H__
#define __PP_QOS_API_H__

#ifdef __KERNEL__
#include <linux/types.h>
#else
#include <stdint.h>
#endif

/**
 * @file
 * @brief QoS API
 */

/******************************************************************************/
/*                                 GENERAL                                    */
/******************************************************************************/

/**
 * @define Instance id for ppv4 qos
 */
#define PP_QOS_INSTANCE_ID              (0)

/**
 * @define Invalid logical queue id
 */
#define PP_QOS_INVALID_ID               (U16_MAX)

/**
 * @define Maximum ports support by HW
 */
#define PP_QOS_MAX_PORTS                (256)

/**
 * @page Calling context
 *        The API exported by this driver, must be called only
 *        from a thread context. Calling them from either
 *        interrupt/tasklet/buttom half is not allowed and will
 *        result in unexpected behaviour.
 */

/**
 * @page Scheduling model
 *
 * Ports, schedulers and queues are modeled as a scheduling tree's nodes.
 * Each such node properties are classified to the following categories:
 * - Common properties - relevant to all nodes
 * - Parent properties - relevant only to parent nodes e.g. arbitration scheme
 *                       between children
 * - Children properties - relevant only to child nodes e.g. node's parent
 * some nodes can be both parents and children e.g. schedulers
 */

/**
 * @page Arbitration
 *
 * The way a parent arbitrates between its children can be either strict
 * priority or round robin.
 *
 * A WRR parent round robin its children - each child gets its turn - as if all
 * children have the same priority. The number of children for this parent type
 * is limited only by the number of nodes (> 1024)
 *
 * A WSP parent employs strict priority arbitration on its children - a child
 * will get scheduled only if there is no other sibling with higher priority
 * that can be scheduled. A WSP parent can have 8 direct children at most, each
 * child must have a different priority. To configure more that 8 children for
 * a WSP parent, or to have several children with the same priority,
 * intermediate schedulers should be use.
 *
 * Example of 5 queues q0, q1, q2, q3, q4 with priorities 0, 1, 2, 3, 3
 *
 *                     Port arbitrating WSP
 *                     /   |    |     \
 *                    q0   q1  q2  Sched arbitrating WRR
 *                                         |	      |
 *                                        q3          q4
 *
 * Example of 16 queues q0 to q15, each with a different priority
 *
 *		      Port arbitrating WSP
 *		         |	       |
 *	    Sched0 arbitrating WSP   Sched1 arbitrating WSP
 *	       /               \       /                   \
 *	   q0 q1 q2 q3 q4 q5 q6 q7  q8 q9 q10 q11 q12 q13 q4 q15
 */

/**
 * @enum pp_qos_arbitration
 * @brief parent method for arbitrating its children on tree
 */
enum pp_qos_arbitration {
	/*!< Strict priority      */
	PP_QOS_ARBITRATION_WSP,

	/*!< Weighted round robin */
	PP_QOS_ARBITRATION_WRR,

	/*!< Weighted fair queue  */
	PP_QOS_ARBITRATION_WFQ
};

/**
 * @struct pp_qos_common_node_properties
 * @brief Properties common to all nodes
 */
struct pp_qos_common_node_properties {
#define QOS_NO_BANDWIDTH_LIMIT          0
#define QOS_MAX_BANDWIDTH_LIMIT         0xA00000 /* 10Gbps */
	/*! bandwidth limit in Kbps */
	u32 bandwidth_limit;

	/*! shared bandwidth group. Don't use, for internal purpose only */
	u32 shared_bw_group;

#define QOS_DEFAULT_MAX_BURST           2 /* 4 Quantas */
#define QOS_MAX_MAX_BURST               7
	/*! Defines the max quantas that can be accumulated
	 * num quantas = 1 << (max_burst)
	 */
	u32 max_burst;
};

/**
 * @struct pp_qos_parent_node_properties
 * @brief Properties relevant for parent node
 */
struct pp_qos_parent_node_properties {
	/*! how parent node arbitrates between its children */
	enum pp_qos_arbitration arbitration;

	/*! whether best effort scheduling is enabled */
	/*! for this node's children */
	s32  best_effort_enable;
};

/**
 * @struct pp_qos_child_node_properties
 * @brief Properties relevant for child node
 */
struct pp_qos_child_node_properties {
	/*! parent's id */
	u32 parent;

#define QOS_MAX_CHILD_PRIORITY 7
	/*! strict priority, relevant only if parent uses wsp arbitration */
	u32 priority;

#define QOS_MIN_CHILD_WRR_WEIGHT 1
#define QOS_MAX_CHILD_WRR_WEIGHT 126
	/*! For WRR, child's weight.
	* For example, in case parent has 3 children
	* setting weights of 2-1-1 for the children means that the first child
	* will have double credits to transmit comparing to other two children.
	* range is 1 - 126
	*/
	u32 wrr_weight;
};

struct pp_qos_dev;

/******************************************************************************/
/*                                      PORTS                                 */
/******************************************************************************/

/**
 * @struct pp_qos_port_stat
 * @brief Statistics per port
 */
struct pp_qos_port_stat {
	/*! Clear statistics after read */
	s32 reset;

	/*! Total green bytes currently in all port queues */
	u32 total_green_bytes;

	/*! Total yellow bytes currently in all port queues */
	u32 total_yellow_bytes;

	/*! Port back pressure status */
	u32 debug_back_pressure_status;

	/*! Actual packet credit */
	u32 debug_actual_packet_credit;

	/*! Actual byte credit */
	u32 debug_actual_byte_credit;
};

/**
 * @struct pp_qos_port_conf
 * @brief Configuration structure for port
 */
struct pp_qos_port_conf {
	/*! common properties */
	struct pp_qos_common_node_properties common_prop;

	/*! properties as a parent */
	struct pp_qos_parent_node_properties port_parent_prop;

	/*! Ring address */
	unsigned long ring_address;

	/*! Ring size */
	u32 ring_size;

	/*! packet credit mode (byte credit disabled) */
	s32 packet_credit_enable;

	/*! amount of credit to add to the port. when packet_credit is */
	/*! enabled this designates packet credit, otherwise byte credit */
	u32 credit;

	/*! disable port transmition */
	s32 disable;

	/*! Egress green bytes threshold */
	u32 green_threshold;

	/*! Egress yellow bytes threshold */
	u32 yellow_threshold;

	/*! Enhanced WSP operation, only for PRX
	    for LGM, the driver is managing it internally and it's applicable
	    for all SP ports */
	u32 enhanced_wsp;
};

/**
 * @struct pp_qos_port_info
 * @brief Port's information
 */
struct pp_qos_port_info {
	u32 physical_id;   /*! node id on scheduling tree */
	u32 num_of_queues; /*! number of queues feeds this port */
};

#define PP_QOS_MAX_INSTANCES (1)

/**
 * @brief Init port configuration with default values
 * @param conf pointer to client allocated struct. This struct
 *        will be filled with default values.
 *
 * Defaults values:
 *    bandwidth_limit - QOS_NO_BANDWIDTH_LIMIT
 *    shared_bandwidth_limit - NO_SHARED_BW_GRP
 *    arbitration - WSP
 *    best_effort_enable - no
 *    packet_credit_enable - yes
 *    byte_credit_enable - no
 *    packet_credit - 0
 *    byte_credit - 0
 *    ring_size - 0
 *    ring_address - 0
 *    green_threshold - 0xFFFFFFFF
 *    yellow_threshold - 0xFFFFFFFF
 */
void pp_qos_port_conf_set_default(struct pp_qos_port_conf *conf);

#define ALLOC_PORT_ID 0xFFFF
/**
 * @brief Allocate a resource for a new port
 * @param qos_dev handle to qos device instance obtained
 *        previously from pp_qos_dev_open
 * @param physical_id if equal ALLOC_PORT_ID then library
 *        allocates a free port id for the new port, otherwise
 *        must be one of the reserved ports ids that were
 *        configured at qos_init
 * @param upon success holds new port's id
 *
 * @return 0 on success.
 */
s32 pp_qos_port_allocate(struct pp_qos_dev *qos_dev, u32 physical_id, u32 *id);

/**
 * @brief Remove port
 * @param qos_dev handle to qos hardware instance.
 * @param id port's id obtained from port_allocate
 *
 * @Return: 0 on success.
 */
s32 pp_qos_port_remove(struct pp_qos_dev *qos_dev, u32 id);

/**
 * @brief Set port configuration
 * @param qos_dev handle to qos device instance obtained
 *         previously from qos_dev_init
 * @param id port's id obtained from port_allocate
 * @param conf port configuration.
 * @return 0 on success
 */
s32 pp_qos_port_set(struct pp_qos_dev *qos_dev, u32 id,
		    const struct pp_qos_port_conf *conf);

/**
 * @brief Remove port desendants from scheduling
 * @param qos_dev handle to qos hardware instance.
 * @param id port's id obtained from port_allocate
 * @return 0 on success
 */
s32 pp_qos_port_disable(struct pp_qos_dev *qos_dev, u32 id);

/**
 * @brief Resume port and its decendants scheduling
 * @param qos_dev handle to qos hardware instance.
 * @param id port's id obtained from port_allocate
 * @return 0 on success
 */
s32 pp_qos_port_enable(struct pp_qos_dev *qos_dev, u32 id);

/**
 * @brief All new packets enqueued to queues feeding this port
 *        will be dropped
 * @param qos_dev handle to qos hardware instance.
 * @param id port's id obtained from port_allocate
 * @note already enportd descriptors will be transmitted
 * @return 0 on success
 */
s32 pp_qos_port_block(struct pp_qos_dev *qos_dev, u32 id);

/**
 * @brief Unblock enqueuing of new packets to queues that feeds
 *        this port
 * @param qos_dev handle to qos hardware instance.
 * @param id port's id obtained from port_allocate
 * @return 0 on success
 */
s32 pp_qos_port_unblock(struct pp_qos_dev *qos_dev, u32 id);

/**
 * @brief Get port configuration
 * @param qos_dev handle to qos device instance obtained from
 * @param id port's id obtained from port_allocate
 * @param conf pointer to struct to be filled with port's
 *        configuration
 * @return 0 on success
 */

s32 pp_qos_port_conf_get(struct pp_qos_dev *qos_dev, u32 id,
			 struct pp_qos_port_conf *conf);

/**
 * @brief Get extra information about port
 * @param id port's id obtained from port_allocate
 * @param info pointer to struct to be filled with port's info
 * @return 0 on success
 */
s32 pp_qos_port_info_get(struct pp_qos_dev *qos_dev, u32 id,
			 struct pp_qos_port_info *info);

/**
 * @brief Get all port's queues
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param id scheduler's id obtained from sched_allocate
 * @param queue_ids client allocated array that will hold queues
 *        ids
 * @param size size of queue_ids
 * @param queues_num holds the number of queues
 * @note queues_num can be bigger than size
 * @return 0 on success
 */
s32 pp_qos_port_get_queues(struct pp_qos_dev *qos_dev, u32 id,
			   uint16_t *queue_ids, u32 size, u32 *queues_num);

/**
 * @brief Get port's statistics
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param id port's id obtained from port_allocate
 * @param stat pointer to struct to be filled with port's
 *        statistics
 * @return 0 on success
 */
s32 pp_qos_port_stat_get(struct pp_qos_dev *qos_dev, u32 id,
			 struct pp_qos_port_stat *stat);

/**
 * @brief Get logical port id
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param physical_id port's physical id allocated at
 *        pp_qos_port_set
 * @return queue logical id or PP_QOS_INVALID_ID on failure
 */
u32 pp_qos_port_id_get(struct pp_qos_dev *qos_dev, u32 physical_id);

/******************************************************************************/
/*                                    QUEUES                                  */
/******************************************************************************/

/**
 * @struct pp_qos_queue_stat
 * @brief Statistics per queue
 */
struct pp_qos_queue_stat {
	/*! Reset after reading */
	s32 reset;

	/*! Packets currently in queue */
	u32 queue_average_size_bytes;

	/*! Packets currently in queue */
	u32 queue_packets_occupancy;

	/*! Bytes currently in queue */
	u32 queue_bytes_occupancy;

	/*! WRED packets accepted */
	u32 total_packets_accepted;

	/*! WRED (any reason) Packet dropped */
	u32 total_packets_dropped;

	/*! WRED Packet dropped (red) */
	u32 total_packets_red_dropped;

	/*! WRED bytes accepted */
	u64 total_bytes_accepted;

	/*! WRED bytes dropped */
	u64 total_bytes_dropped;
};

/**
 * @struct pp_qos_queue_conf
 * @brief Configuration structure for queue
 */
struct pp_qos_queue_conf {
	/*! common properties */
	struct pp_qos_common_node_properties common_prop;

	/*! properties as a child node */
	struct pp_qos_child_node_properties  queue_child_prop;

	/*! queue will drop new enqueued packets */
	s32 blocked;

	/*! WRED is applied on this queue, */
	/*! otherwise only min and max values are used */
	s32 wred_enable;

	/*! use fixed drop probability for WRED instead of slope */
	s32 wred_fixed_drop_prob_enable;

	/*! Start of the slope area (Below that no drops) */
	u32 wred_min_avg_green;

	/*! End of the slope area (Above that 100% drops) */
	u32 wred_max_avg_green;

	/*! 0-100 scale */
	u32 wred_slope_green;

	/*! Fixed drop rate in between min & max */
	u32 wred_fixed_drop_prob_green;

	/*! Start of the slope area (Below that no drops) */
	u32 wred_min_avg_yellow;

	/*! End of the slope area (Above that 100% drops) */
	u32 wred_max_avg_yellow;

	/*! 0-100 scale */
	u32 wred_slope_yellow;

	/*! Fixed drop rate in between min & max */
	u32 wred_fixed_drop_prob_yellow;

	/*! Minimum descriptors guaranteed */
	u32 wred_min_guaranteed;

	/*! Maximum descriptors allowed in this queue */
	u32 wred_max_allowed;

	/*! codel enable */
	s32 codel_en;

	/*! Excess inforation rate
	 * User can set an extra bw per queue, which will be applied only
	 * after serving all "regular" queues in tree
	 */
	u32 eir;

	/*! Excess burst size
	 * Defines the max quantas that can be accumulated
	 * for the excess node (eir)
	 * max num quantas = 1 << (max_excess_burst)
	 */
	u32 ebs;

	/*! Is Fast Queue
	 * Defines whethear this queue is "fast" or not
	 * For fast queues hw maintains double pages to cache.
	 * There could be maximum 128 fast queues in the system.
	 * User should call to pp_qos_get_num_fast_queues,
	 * to verify this queue can be defined as fast
	 */
	u32 is_fast_q;
};

/**
 * @struct pp_qos_queue_info
 * @brief Queue information struct
 */
struct pp_qos_queue_info {
	/*! port fed by this queue */
	u32 port_id;

	/*! queue (ready list) number */
	u32 physical_id;
};

#define PP_QOS_CONTEXT_MAX_QUEUES     (8)
#define PP_QOS_MAX_LLD_SERVICE_FLOWS  (8)
#define PP_QOS_MAX_SERVICE_FLOWS      (16)
#define PP_MAX_HISTOGRAM_BINS         (16)

/**
 * @struct pp_qos_aqm_sf_config
 * @brief AQM service flow configuration
 */
struct pp_qos_aqm_sf_config {
	/*! AQM Latency Target for this Service Flow (milliseconds) */
	u32 latency_target_ms;

	/*! Service Flow configured Peak Rate, expressed in Bytes/sec */
	u32 peak_rate;

	/*! Service Flow configured MaxSustained Rate expressed in Bytes/sec */
	u32 msr;
};

/**
 * @struct pp_qos_lld_sf_config
 * @brief LLD service flow configuration
 */
struct pp_qos_lld_sf_config {
	/* IAQM enable */
	u8     iaqm_en;

	/* Queue Protection enable */
	u8     qp_en;

	/* Max threshold */
	u16    maxth_us;

	/* LG Aging */
	u8     lg_aging;

	/* LG range */
	u8     lg_range;

	/* Critical QL */
	u16    critical_ql_us;

	/* Critical QL Score */
	u16    critical_ql_score_us;

	/* VQ interval */
	u32    vq_interval;

	/* EWMA Alpha */
	u32    vq_ewma_alpha;
};

enum pp_qos_aqm_mode {
	/*! Normal AQM mode */
	PP_QOS_AQM_MODE_NORMAL = 1,

	/*! AQM is defined, but algorithm will not drop.
	 *  Used when defining a queue which may use AQM later on.
	 */
	PP_QOS_AQM_MODE_NO_DROP = 2,

	/*! AQM mode num */
	PP_QOS_AQM_MODE_NUM
};

enum pp_qos_sf_queue_type {
	/*! Queue is a low queue */
	PP_QOS_SF_QUEUE_TYPE_LOW,

	/*! Queue is a high queue */
	PP_QOS_SF_QUEUE_TYPE_HIGH,

	/*! Queue is a management queue */
	PP_QOS_SF_QUEUE_TYPE_MGMT,

	/*! Queue type num */
	PP_QOS_SF_QUEUE_TYPE_NUM,
};

/**
 * @struct pp_qos_sf_queue_config
 * @brief service flow queue configuration
 */
struct pp_qos_sf_queue_config {
	/*! logical queue id */
	u32                       id;

	/*! queue type */
	enum pp_qos_sf_queue_type type;
};

/**
 * @struct pp_qos_aqm_lld_sf_config
 * @brief AQM & LLD service flow configuration
 */
struct pp_qos_aqm_lld_sf_config {
	/*! number of queues attached to this context */
	u32 num_queues;

	/*! array of queues attached to this context */
	struct pp_qos_sf_queue_config queue[PP_QOS_CONTEXT_MAX_QUEUES];

	/*! The size (in bytes) of the buffer for this Service Flow */
	u32 buffer_size;

	/*! AQM Mode
	* Mode PP_QOS_AQM_MODE_NO_DROP is valid only if llsf is 0
	* If a queue is in NO_DROP mode, it can later on be configured to LL SF
	*/
	enum pp_qos_aqm_mode aqm_mode;

	/*! LLD service flow
	* Set to 1 for LLD service flow
	*/
	u8 llsf;

	/*! Coupled SF index. Relevant for ASF only
	* PP_QOS_MAX_SERVICE_FLOWS means that there is no coupled SF
	*/
	u8 coupled_sf;

	/*! For ASF, aggregated SF MSR (b/s) */
	u32 amsr;

	/* For ASF, MSR of the LL SF (b/s) */
	u32 msr_l;

	/*! For ASF, coupling factor */
	u32 coupling_factor;

	/*! For ASF, (Scheduling Weight / 256) */
	u32 weight;

	/*! Histogram - Num bins. Set 0 to disable Histogram */
	u8  num_hist_bins;

	/*! Histogram - Bin edges */
	u32 bin_edges[PP_MAX_HISTOGRAM_BINS - 1];

	/*! AQM Or LLD specific configuration */
	union {
		struct pp_qos_aqm_sf_config aqm_cfg;
		struct pp_qos_lld_sf_config lld_cfg;
	} cfg;
};

/**
 * @struct pp_qos_codel_cfg
 * @brief CoDel configuration
 */
struct pp_qos_codel_cfg {
	/*! Target delay in mSec */
	u32 target_delay_msec;

	/*! Interval time mSec */
	u32 interval_time_msec;
};

/**
 * @struct pp_qos_codel_stat
 * @brief Statistics per CoDel queue
 */
struct pp_qos_codel_stat {
	/*! Reset after reading */
	s32 reset;

	/*! Packets dropped by CoDel */
	u32 packets_dropped;

	/*! Bytes dropped by CoDel */
	u32 bytes_dropped;

	/*! Minimum sojourn time */
	u32 min_sojourn_time;

	/*! Maximum sojourn time */
	u32 max_sojourn_time;

	/*! Total sojourn time */
	u32 total_sojourn_time;

	/*! Maximum packet size */
	u32 max_packet_size;

	/*! Total number of packets */
	u32 total_packets;
};

/**
 * PP MAX Histogram bins
 */
#define PP_QOS_MAX_HIST_BINS      (16)

/**
 * @struct pp_sf_hist_stat
 * @brief Statistics per Histogram
 */
struct pp_sf_hist_stat {
	/*! Num histogram bins */
	u8 num_hist_bins;

	/*! Histogram counter per bin */
	u32 hist_counter[PP_QOS_MAX_HIST_BINS];

	/*! Number of total updates */
	u32 hist_updates;

	/*! maximum latency */
	u32 max_latency;
};

/**
 * @struct pp_qos_pci_addr
 * @brief struct holding the addresses of registers
 * 		  used over pci device in pp
 */
struct pp_qos_pci_addr {
	u32 msrtoken_addr;
};

/**
 * @brief Init configuration with default values
 * @param conf pointer to client allocated struct. This struct
 *        will be filled with default values.
 *
 * Defaults values:
 *    bandwidth_limit - 0
 *    shared_bandwidth_limit - 0
 *    parent - 0
 *    priority - 0
 *    bandwidth_share - 0
 *    wred_enable - no
 *    ALL wred params - 0
 *    codel_en - 0
 */
void pp_qos_queue_conf_set_default(struct pp_qos_queue_conf *conf);

/**
 * @brief Allocate resources for a new queue
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param id upon success hold's new queue id
 * @return 0 on success
 */
s32 pp_qos_queue_allocate(struct pp_qos_dev *qos_dev, u32 *id);

/**
 * @brief Allocate resources for a new queue
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param id upon success hold's new queue id
 * @param phy upon success hold's new physical queue id
 * @return 0 on success
 */
s32 pp_qos_queue_allocate_id_phy(struct pp_qos_dev *qdev, u32 *id, u32 *phy);

/**
 * @brief Remove a queue
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param id queue's id obtained from queue_allocate
 *
 * @note client should make sure that queue is empty and that
 *        new packets are not enqueued, by calling
 *        pp_qos_queue_disable
 * @return 0 on success
 */
s32 pp_qos_queue_remove(struct pp_qos_dev *qos_dev, u32 id);

/**
 * @brief Allocate resources for new contiguous queues
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param ids upon success hold's new queue id's list
 * @param phy_ids upon success hold's new physical queue id's list
 * @param count Amount of contiguous queues to allocate
 * @note Amount of queues must be as defined in dts
 * @return 0 on success
 */
s32 pp_qos_contiguous_queue_allocate(struct pp_qos_dev *qos_dev, u32 *ids,
				     u32 *phy_ids, u32 count);

/**
 * @brief Remove all contiguous queues
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param id holds the first queue id obtained by alloc function
 * @param count Amount of contiguous queues to remove
 * @note Amount of queues must be as defined in dts
 * @return 0 on success
 */
s32 pp_qos_contiguous_queue_remove(struct pp_qos_dev *qos_dev, u32 id,
				   u32 count);

/**
 * @brief Set queue configuration
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param id queue's id obtained from queue_allocate
 * @param conf new configuration for the queue
 * @return 0 on success
 */
s32 pp_qos_queue_set(struct pp_qos_dev *qos_dev, u32 id,
		     const struct pp_qos_queue_conf *conf);

/**
 * @brief All new packets enqueue to this queue will be dropped
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param id queue's id obtained from queue_allocate
 * @note already enqueued descriptors will be transmitted
 * @return 0 on success
 */
s32 pp_qos_queue_block(struct pp_qos_dev *qos_dev, u32 id);

/**
 * @brief Unblock enqueuing of new packets
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param id queue's id obtained from queue_allocate
 * @return 0 on success
 */
s32 pp_qos_queue_unblock(struct pp_qos_dev *qos_dev, u32 id);

/**
 * @brief Get queue current configuration
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open id: port's id obtained from
 *        queue_allocate
 * @param conf pointer to struct to be filled with queue's
 *        configuration
 * @return 0 on success
 */
s32 pp_qos_queue_conf_get(struct pp_qos_dev *qos_dev, u32 id,
			  struct pp_qos_queue_conf *conf);

/**
 * @brief Get information about queue
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param id queue's id obtained from queue_allocate
 * @param info pointer to struct to be filled with queue's info
 * @return 0 on success
 */
s32 pp_qos_queue_info_get(struct pp_qos_dev *qos_dev, u32 id,
			  struct pp_qos_queue_info *info);

/**
 * @brief Get logical queue id
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param physical_id queue's physical id allocated at
 *        pp_qos_queue_set
 * @return queue logical id or PP_QOS_INVALID_ID on failure
 */
u32 pp_qos_queue_id_get(struct pp_qos_dev *qos_dev, u32 physical_id);

/**
 * @brief Get queue's statistics
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param id queue's id obtained from queue_allocate
 * @param stat pointer to struct to be filled with queue's
 *        statistics
 * @return 0 on success
 */
s32 pp_qos_queue_stat_get(struct pp_qos_dev *qos_dev, u32 id,
	struct pp_qos_queue_stat *stat);

/**
 * @brief Set Aqm/LLD service flow configuration
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param sf_id   service flow id
 * @param sf_cfg  service flow configuration
 * @return 0 on success
 */
s32 pp_qos_aqm_lld_sf_set(struct pp_qos_dev *qos_dev, u8 sf_id,
	struct pp_qos_aqm_lld_sf_config *sf_cfg);

/**
 * @brief Set PCI addresses in qos fw
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param pci_addr structure holding register addresses
 * @return 0 on success
 */
s32 pp_qos_pci_addr_set(struct pp_qos_dev *qdev, struct pp_qos_pci_addr *pci_addr);

/**
 * @brief Set AQM engine type in QoS FW
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param aqm_engine sw or hw engine type
 * @return 0 on success
 */
s32 pp_qos_set_aqm_engine(struct pp_qos_dev *qdev, u8 aqm_engine);

/**
 * @brief Remove Aqm/LLD service flow
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param sf_id   service flow id
 * @param sf_cfg SF config
 * @return 0 on success
 */
s32 pp_qos_aqm_lld_sf_remove(struct pp_qos_dev *qos_dev, u8 sf_id,
	struct pp_qos_aqm_lld_sf_config *sf_cfg);

/**
 * @brief Get histogram statistics
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param sf_id service flow id
 * @param hist pointer to struct to be filled with histogram
 *        statistics
 * @param reset Clear statistics after read
 * @return 0 on success
 */
s32 pp_qos_sf_hist_get(struct pp_qos_dev *qos_dev, u8 sf_id,
	struct pp_sf_hist_stat *hist, bool reset);

/**
 * @brief Set CoDel configuration
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param cfg CoDel configuration
 * @return 0 on success
 */
s32 pp_qos_codel_cfg_set(struct pp_qos_dev *qdev, struct pp_qos_codel_cfg *cfg);

/**
 * @brief Get CoDel configuration
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param cfg CoDel configuration
 * @return 0 on success
 */
s32 pp_qos_codel_cfg_get(struct pp_qos_dev *qdev, struct pp_qos_codel_cfg *cfg);

/**
 * @brief Get CoDel queue statistics
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param id queue's id obtained from queue_allocate
 * @param stat pointer to struct to be filled with queue's
 *        CoDel statistics
 * @return 0 on success
 */
s32 pp_qos_codel_queue_stat_get(struct pp_qos_dev *qdev, u32 id,
				struct pp_qos_codel_stat *stat);

/******************************************************************************/
/*                                 SCHEDULER                                  */
/******************************************************************************/

/**
 * @struct pp_qos_sched_conf
 * @brief Configuration structure for scheduler
 */
struct pp_qos_sched_conf {
	/*! common properties as a node */
	struct pp_qos_common_node_properties common_prop;

	/*! properties as a parent node */
	struct pp_qos_parent_node_properties sched_parent_prop;

	/*! properties as a child node */
	struct pp_qos_child_node_properties  sched_child_prop;
};

/**
 * @struct pp_qos_sched_info
 * @brief Scheduler information struct
 */
struct pp_qos_sched_info {
	u32 port_id;       /*! port fed by this scheduler */
	u32 num_of_queues; /*! number of queues feed this scheduler */
};

/**
 * @brief Init scheduler config with defaults
 * @param conf pointer to client allocated struct which will be
 *        filled with default values
 *
 * Defaults values:
 *    bandwidth_limit - 0
 *    shared_bandwidth_limit - 0
 *    arbitration - WSP
 *    best_effort_enable - no
 *    parent - 0
 *    priority - 0
 *    bandwidth_share - 0
 */
void pp_qos_sched_conf_set_default(struct pp_qos_sched_conf *conf);

/**
 * @brief Allocate a resource for a new scheduler
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param id upon success - holds new scheduler's id
 * @return  0 on success
 */
s32 pp_qos_sched_allocate(struct pp_qos_dev *qos_dev, u32 *id);

/**
 * @brief Remove scheduler
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param id scheduler's id obtained from sched_allocate
 * @return 0 on success
 */
s32 pp_qos_sched_remove(struct pp_qos_dev *qos_dev, u32 id);

/**
 * @brief Set scheduler configuration
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param id scheduler's id obtained from sched_allocate
 * @param conf new configuration for the scheduler
 * @note must init conf by pp_qos_sched_conf_init before calling
 *        this function.
 * @return 0 on success
 */
s32 pp_qos_sched_set(struct pp_qos_dev *qos_dev, u32 id,
		     const struct pp_qos_sched_conf *conf);

/**
 * @brief Get sched current configuration
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open id: port's id obtained from
 *        port_allocate
 * @param conf pointer to struct to be filled with sched's
 *        configuration
 * @return 0 on success
 */
s32 pp_qos_sched_conf_get(struct pp_qos_dev *qos_dev, u32 id,
			  struct pp_qos_sched_conf *conf);

/**
 * @brief Get information about scheduler
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param id scheduler's id obtained from sched_allocate
 * @param info pointer to struct to be filled with scheduler's
 *        info
 * @return 0 on success
 */
s32 pp_qos_sched_info_get(struct pp_qos_dev *qos_dev, u32 id,
			  struct pp_qos_sched_info *info);

/**
 * @brief Get all scheduler's queues
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param id scheduler's id obtained from sched_allocate
 * @param queue_ids client allocated array that will hold
 *        queues ids
 * @param size size of queue_ids
 * @param queues_num holds the number of queues
 * @note queues_num can be bigger than size
 * @return 0 on success
 */
s32 pp_qos_sched_get_queues(struct pp_qos_dev *qos_dev, u32 id,
			    uint16_t *queue_ids, u32 size, u32 *queues_num);

/******************************************************************************/
/*                                  INIT	                              */
/******************************************************************************/

/**
 * @brief Initialize of a qos device
 * @param id Identify the requested qos instance (same id that
 *        designates the instance in the device tree)
 * @return on success handle to qos device
 *        NULL otherwise
 */
struct pp_qos_dev *pp_qos_dev_open(u32 id);

void pp_qos_mgr_init(void);
void pp_qos_mgr_exit(void);

/**
 * @brief get fw version
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param major storage for major part of version
 * @param minor storage for minor part of version
 * @param build storage for build part of version
 * @return 0 on success
 */
s32 pp_qos_get_fw_version(struct pp_qos_dev *qos_dev, u32 *major, u32 *minor,
			  u32 *build);

enum pp_qos_node_type {
	PPV4_QOS_NODE_TYPE_PORT,
	PPV4_QOS_NODE_TYPE_SCHED,
	PPV4_QOS_NODE_TYPE_QUEUE,
};

#define PPV4_QOS_INVALID (-1U)

/**
 * @struct pp_qos_node_num
 */
struct pp_qos_node_num {
	/*! Physical node on scheduler's node table */
	u32 phy;

	/*! Logical id seen by client */
	u32 id;
};

struct pp_qos_stats {
	u64 pkts_rcvd;
	u64 pkts_dropped;
	u64 wred_pkts_dropped;
	u64 codel_pkts_dropped;
	u64 pkts_transmit;
	u64 bytes_rcvd;
	u64 bytes_dropped;
	u64 bytes_transmit;
};

struct pp_qos_aqm_sf_stats {
	u32 occupancy;
	u32 burst_state;
	u32 drop_prob;
	u32 hist_updates;
	u32 max_latency;
	u32 last_total_accepts;
};

/**
 * @brief Get the packet statistics
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param stats
 * @return s32 0 on success, error code otherwise
 */
s32 pp_qos_stats_get(struct pp_qos_dev *qos_dev, struct pp_qos_stats *stats);

/**
 * @brief Reset all Qos queues statistics
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @return s32 0 on success, error code otherwise
 */
s32 pp_qos_stats_reset(struct pp_qos_dev *qos_dev);

/**
 * @brief Gets QoS quanta (In MB). QoS credits calculations are
 *        done in Quanta scale. QoS will transmit a
 *        multiplication of a quanta in one iteration
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param quanta pointer which the quanta value will be written
 *        to
 * @return s32 0 on success, error code otherwise
 */
s32 pp_qos_get_quanta(struct pp_qos_dev *qdev, unsigned int *quanta);

#define PP_QOS_MAX_FAST_QUEUES    128
/**
 * @brief Gets number of fast queues.
 *        Maximum fast queues which can be defined is PP_QOS_MAX_FAST_QUEUES
 *        User should call this API prior to set a fast queue, to verify
 *        enough resources
 * @param qos_dev handle to qos device instance obtained from
 *        pp_qos_dev_open
 * @param num_fast_queues pointer which the num queues will be written to
 *
 * @return s32 0 on success, error code otherwise
 */
s32 pp_qos_get_num_fast_queues(struct pp_qos_dev *qdev,
			       unsigned int *num_fast_queues);

#endif /* __PP_QOS_API_H__ */
