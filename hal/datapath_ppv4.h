// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2021, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#ifndef DATAPATH_PPV4_H
#define DATAPATH_PPV4_H

#include <net/datapath_api.h>

#define MAX_PP_CHILD_PER_NODE   8 /* Maximum queue per scheduler */
#define INV_RESV_IDX            0xFFFF  /* Invalid reserved resource index */
#define DEF_QRED_MAX_ALLOW      0x400  /* max qocc in queue */
#define DEF_QRED_MIN_ALLOW      0x40 /* minqocc in queue */
#define DEF_QRED_SLOP_GREEN     30 /* green slop in queue */
#define DEF_QRED_SLOP_YELLOW    70 /* yellow slop in queue */
#define DEF_WRED_RATIO          5

#define HAL(inst) ((struct hal_priv *)dp_port_prop[inst].priv_hal)
#define PARENT(x) (x.queue_child_prop.parent)
#define PARENT_S(x) (x.sched_child_prop.parent)
#define CHILD(x, idx) (priv->qos_sch_stat[x].child[idx])
#define DP_PORT(p) (dp_deq_port_tbl[p->inst][p->cqm_deq_port.cqm_deq_port])

#define Q_NODE(n)		((n) == DP_NODE_QUEUE)
#define S_NODE(n)		((n) == DP_NODE_SCH)
#define P_NODE(n)		((n) == DP_NODE_PORT)
#define PP_FREE(x)		((x) == PP_NODE_FREE)
#define PP_ALLOC(x)		((x) & PP_NODE_ALLOC)
#define PP_ACT(x)		((x) & PP_NODE_ACTIVE)
#define PP_RESV(x)		((x) & PP_NODE_RESERVE)

enum flag {
	DP_NODE_DEC = BIT(0), /* flag to reduce node counter */
	DP_NODE_INC = BIT(1), /* flag to increase node counter */
	DP_NODE_RST = BIT(2), /* flag to reset node counter */
	C_FLAG = BIT(8), /* scheduler flag linked to node */
	P_FLAG = BIT(9) /* scheduler flag linked to parent */
};

struct ppv4_queue {
	int inst;  /* dp instance */
	u16 qid;  /* -1 means dynammic, otherwise already configured */
	u16 node_id; /*output */
	u16 sch;  /* -1 means dynammic, otherwise already configured */
	u16 parent; /* -1 means no parent.
		     * it is used for shared dropping queueu purpose
		     */
};

struct ppv4_scheduler {
	u16 sch;  /* -1 means dynammic, otherwise already configured */
	u16 parent; /* input */
	u16 node_id; /* output */
};

struct ppv4_port {
	int inst;
	u16 dp_port;
	u16 qos_deq_port; /* -1 means dynammic, otherwise already specified.
			   * Remove in new datapath lib
			   */
	u16 cqm_deq_port;  /* rename in new datapath lib */
	u16 node_id; /* output */

	u32 tx_pkt_credit;  /* PP port tx bytes credit */
	void *tx_ring_addr;  /* PP port ring address */
	void *tx_ring_addr_push;  /* PP port ring address */
	u32 tx_ring_size; /* PP ring size */
};

struct ppv4_q_sch_port {
	/* input */
	int inst;
	int dp_port; /* for storing q/scheduler */
	int ctp; /* for storing q/scheduler: masked subifid. */
	u32 cqe_deq; /* CQE dequeue port */
	u32 tx_pkt_credit;  /* PP port tx bytes credit */
	void *tx_ring_addr;  /* PP port ring address. */
	void *tx_ring_addr_push;  /* PP port ring address. */
	u32 tx_ring_size; /* PP ring size */

	/* output of PP */
	u32 qid;
	u32 q_node;
	u32 schd_node;
	u32 port_node; /* qos port node id */
	u32 f_deq_port_en: 1; /* flag to trigger cbm_dp_enable */
};

struct pp_sch_list {
	u32 flag: 1; /* 0: valid 1-used 2-reserved */
	u16 node;
	u16 parent_type;  /* scheduler/port */
	u16 parent;
};

enum PP_NODE_STAT {
	PP_NODE_FREE = 0, /* Free and not allocated yet */
	PP_NODE_ALLOC = BIT(0), /* allocated */
	PP_NODE_ACTIVE = BIT(1), /* linked */
	PP_NODE_RESERVE = BIT(2) /* reserved */
};

struct pp_node {
	enum PP_NODE_STAT flag; /* 0: FREE 1-used/alloc */
	u16 type; /* node type */
	u16 node_id;  /* node id */
};

struct pp_queue_stat {
	enum PP_NODE_STAT flag; /* 0: valid 1-used 2-reserved */
	u16 deq_port; /* cqm dequeue port id */
	u16 node_id;  /* queue node id */
	u16 resv_idx; /* index of reserve table */
	u16 dp_port; /* datapath port id */
	u32 blocked; /* Q blocked state */
};

struct pp_sch_stat {
	u8 used;
	struct dp_node node;
	enum PP_NODE_STAT c_flag; /* sch flag linked to child */
	enum PP_NODE_STAT p_flag; /* sch flag linked to parent */
	u16 resv_idx; /* index of reserve table */
	struct pp_node child[MAX_PP_CHILD_PER_NODE];
	u16 child_num; /* Number of child */
	struct pp_node parent; /* valid for node type queue/sch */
	u16 dp_port; /* datapath port id */
	int prio_wfq; /* Q priority */
};

struct cqm_deq_stat {
	enum PP_NODE_STAT flag; /* 0: valid 1-used 2-reserved */
	u16 deq_id; /* qos dequeue port physical id. Maybe no need */
	u16 node_id; /* qos dequeue port's node id */
	u16 child_num; /* Number of child */
	u8 disabled; /* Port disabled state */
};

struct limit_map {
	int pp_limit; /* pp shaper limit */
	int dp_limit; /* dp shaper limit */
};

struct arbi_map {
	int pp_arbi; /* pp arbitrate */
	int dp_arbi; /* dp arbitrate */
};

void init_qos_fn_32(void);

int dp_qos_queue_remove(struct pp_qos_dev *qdev, u32 id);

int dp_qos_queue_allocate(struct pp_qos_dev *qdev, u32 *id);
int dp_qos_queue_allocate_id_phy(struct pp_qos_dev *qdev, u32 *id,
				 u32 *phy);
int dp_qos_queue_info_get(struct pp_qos_dev *qdev, u32 id,
			  struct pp_qos_queue_info *info);

int dp_qos_port_remove(struct pp_qos_dev *qdev, u32 id, int inst);

int dp_qos_sched_allocate(struct pp_qos_dev *qdev, u32 *id);

int dp_qos_sched_remove(struct pp_qos_dev *qdev, u32 id);

int dp_qos_port_allocate(struct pp_qos_dev *qdev,
			 u32 physical_id, u32 *id, int inst);

int dp_qos_port_set(struct pp_qos_dev *qdev, u32 id,
		    const struct pp_qos_port_conf *conf);

void dp_qos_port_conf_set_default(struct pp_qos_port_conf *conf);

void dp_qos_queue_conf_set_default(struct pp_qos_queue_conf *conf);

int dp_qos_queue_set(struct pp_qos_dev *qdev, u32 id,
		     const struct pp_qos_queue_conf *conf);

void dp_qos_sched_conf_set_default(struct pp_qos_sched_conf *conf);

int dp_qos_sched_set(struct pp_qos_dev *qdev, u32 id,
		     const struct pp_qos_sched_conf *conf);

int dp_qos_queue_conf_get(struct pp_qos_dev *qdev, u32 id,
			  struct pp_qos_queue_conf *conf);

int dp_qos_sched_conf_get(struct pp_qos_dev *qdev, u32 id,
			  struct pp_qos_sched_conf *conf);

int dp_qos_sched_get_queues(struct pp_qos_dev *qdev, u32 id,
			    u16 *queue_ids, u32 size,
			    u32 *queues_num);

int dp_qos_port_get_queues(struct pp_qos_dev *qdev, u32 id,
			   u16 *queue_ids, u32 size,
			   u32 *queues_num);

int dp_qos_port_conf_get(struct pp_qos_dev *qdev, u32 id,
			 struct pp_qos_port_conf *conf);

struct pp_qos_dev *dp_qos_dev_open(u32 id);

int dp_pp_alloc_port(struct ppv4_port *info);
int dp_pp_alloc_sched(struct ppv4_scheduler *info);
int dp_pp_alloc_queue(struct ppv4_queue *info);
int alloc_q_to_port(struct ppv4_q_sch_port *info, u32 flag);
extern struct cqm_deq_stat deq_port_stat[DP_MAX_PPV4_PORT];
extern struct pp_queue_stat qos_queue_stat[DP_MAX_QUEUE_NUM];
int init_ppv4_qos(int inst, int flag);

static inline char *node_type_str(enum dp_node_type type)
{
	if (Q_NODE(type))
		return "queue";
	if (S_NODE(type))
		return "sched";
	if (P_NODE(type))
		return "port";
	if (type == 0)
		return "0";

	pr_err("DPM: unknown node type: %d\n", type);
	return "??";
}

static inline char *node_stat_str(enum PP_NODE_STAT stat)
{
	/* Note: it can be multiple bit set.
	 *       we need to print from high bit to low bit
	 */
	if (PP_RESV(stat)) {
		if (PP_ACT(stat))
			return "resv+act";
		if (PP_ALLOC(stat))
			return "resv+alloc";
		if (PP_FREE(stat))
			return "resv+free";
	} else {
		if (PP_ACT(stat))
			return "active";
		if (PP_ALLOC(stat))
			return "alloc";
		if (PP_FREE(stat))
			return "free";
	}
	pr_err("DPM: unknown node stat: %d\n", stat);
	return "??";
}

#endif /* DATAPATH_PPV4_H */
