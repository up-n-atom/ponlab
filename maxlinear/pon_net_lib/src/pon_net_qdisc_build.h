/*****************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 * \file
 * This contains functions that construct a qdisc tree from managed entities
 * stored on ME list
 */

#ifndef _PON_NET_QDISC_BUILD_H_
#define _PON_NET_QDISC_BUILD_H_

/** \addtogroup PON_NET_LIB
 *
 *  @{
 */

/**
 * \file
 * The OMCI defines Priority Queue ME and Traffic Scheduler ME. These two
 * managed entities are used together to implement hierarchical scheduling.
 * The scheduler hierarchy is implemented using TC qdiscs. The hierarchy
 * defined by OMCI will be represented by the equivalent TC qdisc hierarchy.
 * We use tc-drr and tc-prio to represent respectively WRR and SP scheduling.
 *
 * To configure WRED queue parameters defined on Priority Queue ME,
 * two tc-red qdiscs are attached as a leaf of every WRR and SP qdisc in the
 * tree.
 *
 * Also, if ONU-G ME has the traffic management option configured to RATE,
 * additional tc-tbf qdisc will be added to each leaf tc-red.
 *
 * Example:
 *
 * Let's assume that we have the following Priority Queue MEs configured:
 *
 * +--------+--------------+------------------------------------------------+
 * | ME ID  | Related port | TS ptr | Weight | Packet drop queue thresholds |
 * |        |              |        |        | green       yellow           |
 * |        |              |        |        | min   max   min   max        |
 * +--------+--------------+--------+--------+------------------------------+
 * | 0x8000 | 0x80000000   | 0      | 0      | 46080 51200 23040 25600      |
 * | 0x8001 | 0x80000001   | 0      | 0      | 46080 51200 23040 25600      |
 * | 0x8002 | 0x80000002   | 0      | 0      | 46080 51200 23040 25600      |
 * | 0x8003 | 0x80000003   | 0      | 0      | 46080 51200 23040 25600      |
 * | 0x8004 | 0x80000004   | 0xff00 | 1      | 46080 51200 23040 25600      |
 * | 0x8005 | 0x80000005   | 0xff00 | 2      | 46080 51200 23040 25600      |
 * | 0x8006 | 0x80000006   | 0xff00 | 4      | 46080 51200 23040 25600      |
 * | 0x8007 | 0x80000007   | 0xff00 | 8      | 46080 51200 23040 25600      |
 * +--------+--------------+--------+--------+------------------------------+
 *
 * And the following traffic scheduler ME:
 *
 * +--------+--------+--------+--------+--------+
 * | ME ID  | T-CONT | TS ptr | Policy | Weight |
 * +--------+--------+--------+--------+--------+
 * | 0xff00 | 0x8000 | 0      | 0      | 0      |
 * +--------+--------+--------+--------+--------+
 *
 * This results in following scheduling hierarchy:
 *
 *     T-CONT (0x8000)
 *     |-- Priority Queue (0x8000)
 *     |-- Priority Queue (0x8001)
 *     |-- Priority Queue (0x8002)
 *     |-- Priority Queue (0x8003)
 *     `-- Traffic Scheduler (0xff00)
 *         |-- Priority Queue (0x8004)
 *         |-- Priority Queue (0x8005)
 *         |-- Priority Queue (0x8006)
 *         `-- Priority Queue (0x8007)
 *
 * Which will be represented by the following qdisc hierarchy:
 *
 *     tc-prio
 *     |-- class:1
 *     |   `-- tc-red (min 46080, max 51200)
 *     |       `-- tc-red (min 23040, max 25600)
 *     |-- class:2
 *     |   `-- tc-red (min 46080, max 51200)
 *     |       `-- tc-red (min 23040, max 25600)
 *     |-- class:3
 *     |   `-- tc-red (min 46080, max 51200)
 *     |       `-- tc-red (min 23040, max 25600)
 *     |-- class:4
 *     |   `-- tc-red (min 46080, max 51200)
 *     |       `-- tc-red (min 23040, max 25600)
 *     |-- class:5
 *     |   `-- tc-drr
 *     |       |-- class:1 (weight 4096)
 *     |       |   `-- tc-red (min 46080, max 51200)
 *     |       |       `-- tc-red (min 23040, max 25600)
 *     |       |-- class:2 (weight 8192)
 *     |       |   `-- tc-red (min 46080, max 51200)
 *     |       |       `-- tc-red (min 23040, max 25600)
 *     |       |-- class:3 (weight 16384)
 *     |       |   `-- tc-red (min 46080, max 51200)
 *     |       |       `-- tc-red (min 23040, max 25600)
 *     |       `-- class:4 (weight 32768)
 *     |           `-- tc-red (min 46080, max 51200)
 *     |               `-- tc-red (min 23040, max 25600)
 *     |-- class:6
 *     |-- class:7
 *     `-- class:8
 *
 * To build this hierarchy, struct \ref pon_net_tree_builder is used and
 * together with \ref pon_net_qdiscs_build_and_diff_apply()
 */

struct pon_net_context;
struct pon_net_qdiscs;
struct pon_net_me_list;

/**
 * Tree builders are used to build a qdisc tree
 * (represented by struct pon_net_qdiscs) from struct pon_net_me_list
 * Initially the builders with _sp are used, and later if the OLT changes
 * the configuration builders with _mixed are used.
 */
struct pon_net_tree_builder {
	/* build() will fill the qdiscs tree with qdisc data structures, which
	 * will later be used to setup qdisc hierarchy. */
	enum pon_adapter_errno (*build)(struct pon_net_me_list *me,
					uint16_t related_me_id,
					struct pon_net_qdiscs *tree);
};

/**
 * Builds strict priority tree (only tc-prio qdiscs), even if Priority Queue
 * configuration suggests otherwise.
 * Note: On the uni side we configure always need strict priority, even if OLT
 * suggests otherwise (PONRTSYS-7959)
 */
extern const struct pon_net_tree_builder pon_net_sp_builder;

/**
 * Builds a qdisc tree containing both - tc-drr and tc-prio - this is when OLT
 * changes Priority Queue weight, after T-CONT is updated.
 * Note: This is done after T-CONT is updated to avoid PONRTSYS-5432 OLT bug,
 * for which the proper fix has still to be implemented
 */
extern const struct pon_net_tree_builder pon_net_mixed_builder;

/**
 * Same as pon_net_sp_builder, but configures tc-tbf as leaves to support
 * traffic shaping specified by Traffic Descriptor ME
 */
extern const struct pon_net_tree_builder pon_net_sp_tbf_builder;

/**
 * Same as pon_net_mixed_builder, but configures tc-tbf as leaves to support
 * traffic shaping specified by Traffic Descriptor ME
 */
extern const struct pon_net_tree_builder pon_net_mixed_tbf_builder;

/** Builds a qdisc tree containing root tc-drr and tc-prio */
extern const struct pon_net_tree_builder pon_net_ext_uni_ds_builder;

/** Builds a collection of qdisc using specified builder, then
 *  compares the resulting collection with the provided one and
 *  applies the difference to TC.
 *
 *  \param[in]  qdiscs		Collection of qdiscs
 *  \param[in]  class_id	Managed Entity Class ID
 *  \param[in]  me_id		Managed Entity ID
 *  \param[in]  builder		Tree builder
 *  \param[in]  reassign_gems   Whether this function should reassign
 *				GEMs to queues
 *  \param[in]  ctx		PON Net Lib context
 *
 *  \return returns value as follows:
 *  - PON_ADAPTER_SUCCESS: If successful
 *  - Other: An error code in case of error.
 */
enum pon_adapter_errno
pon_net_qdiscs_build_and_diff_apply(struct pon_net_qdiscs *qdiscs,
				    uint16_t class_id,
				    uint16_t me_id,
				    const struct pon_net_tree_builder *builder,
				    bool reassign_gems,
				    struct pon_net_context *ctx);

/** @} */ /* PON_NET_LIB */

#endif
