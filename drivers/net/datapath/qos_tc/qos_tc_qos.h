/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright (c) 2020 - 2025 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/
#ifndef _QOS_TC_QOS_
#define _QOS_TC_QOS_

#include <net/pkt_cls.h>
#include <linux/list.h>
#include <linux/radix-tree.h>
#include <linux/types.h>

#include "qos_tc_flower.h"

#define QOS_TC_MAX_Q 8
#define QOS_TC_DOT1P_SZ 8
#define QOS_TC_SCH_MAX_LEVEL 3
#define QOS_TC_8021P_HIGHEST_PRIO 7
#define QOS_TC_UNUSED -1
#define QOS_TC_CPU_PORT 0

#define QOS_TC_COOKIE_EMPTY -1

enum qos_tc_qdisc_type {
	QOS_TC_QDISC_NONE,
	QOS_TC_QDISC_PRIO,
	QOS_TC_QDISC_DRR,
	QOS_TC_QDISC_TBF,
	QOS_TC_QDISC_RED,
};

/* Forward declaration of tc_params */
struct qos_tc_params;

struct qos_tc_qdisc_ops {
	int (*offload)(struct net_device *dev, void *params);

	int (*unoffload)(struct net_device *dev, void *params);
};

enum qos_tc_qdata_type {
	QOS_TC_QDATA_GREEN,
	QOS_TC_QDATA_YELLOW,
	QOS_TC_QDATA_TBF,
	QOS_TC_QDATA_CODEL,
};

struct qos_tc_qdata_params {
	u32 handle;
	u32 parent;
	enum qos_tc_qdata_type type;
	struct qos_tc_q_data *qid;
	int (*destroy)(struct net_device *dev, u32 handle, u32 parent);
	struct list_head list;
};

struct qos_tc_q_data {
	u32 qid;
	int tc;
	atomic_t ref_cnt;
	int arbi;
	int p_w;
	struct list_head params;
};

struct qos_tc_dot1p {
	bool en;
	unsigned int tcid;
};

struct qos_tc_tbf_data {
	u32 parent;
	u32 handle;
	u32 cir;
	u32 pir;
	u32 cbs;
	u32 pbs;
};

struct qos_tc_qdisc {
	struct net_device *dev;
	enum qos_tc_qdisc_type type;
	u32 parent;
	u32 handle;
	int use_cnt;
#if IS_ENABLED(CONFIG_QOS_NOTIFY)
	int offset; /* scheduler offset to maintain q_map */
#endif

	/* HW specific settings */
	u32 sch_id;
	int inst;
	s32 port;
	s32 deq_idx;
	int epn;
	int def_q;
	bool ds;
	struct qos_tc_dot1p dot1p[QOS_TC_DOT1P_SZ];

	unsigned int num_q;
	struct qos_tc_q_data qids[QOS_TC_MAX_Q];

	unsigned int num_children;
	struct qos_tc_qdisc *children[QOS_TC_MAX_Q];
	/* Used for sibling qdiscs */
	int p_w;

	bool tbf_on;
	unsigned int num_tbf;
	struct qos_tc_tbf_data tbfs[QOS_TC_MAX_Q];

	union {
		struct {
			u8 bands;
			u8 priomap[TC_PRIO_MAX + 1];
		} prio;

		struct {
			u8 num_tc;
			u8 priomap[TC_PRIO_MAX + 1];
		} mqprio;

		struct {
			int classes;
		} drr;
	};

	int alloc_flag;
	/* TODO: abstrac the hw specific part */
	struct qos_tc_qdisc_ops *ops;
};

struct qos_tc_port {
	struct net_device *dev;
	struct qos_tc_qdisc root_qdisc;
	unsigned int sch_num;
	bool tbf_on;
	struct qos_tc_tbf_data tbf;
	struct qos_tc_q_data tbf_queue;
#if IS_ENABLED(CONFIG_QOS_NOTIFY)
	u64 q_map; /* q_map for maintaining queues */
#endif
	int (*destroy)(struct qos_tc_port *);
	struct radix_tree_root qdiscs;
	struct list_head list;
};

struct qos_tc_qmap_tc {
#define TC_VAL_INV -1
	int flags;
	u32 handle;
	struct net_device *indev;
	int tc;
	int subif;
	char tc_cookie;
	bool ingress;
};

int qos_tc_get_queue_by_handle(struct net_device *dev,
			       u32 handle,
			       struct qos_tc_q_data **qid);
int qos_tc_get_sch_by_handle(struct net_device *dev,
			     u32 handle,
			     struct qos_tc_qdisc **sch);


struct qos_tc_port *qos_tc_port_get(struct net_device *dev);
struct qos_tc_port *qos_tc_port_alloc(struct net_device *dev);
int qos_tc_port_delete(struct qos_tc_port *port);
int qos_tc_alloc_qdisc(struct qos_tc_qdisc **qdisc);
void qos_tc_free_qdisc(struct qos_tc_qdisc *qdisc);
struct qos_tc_qdisc *qos_tc_qdisc_find(struct qos_tc_port *port, u32 handle);

int qos_tc_queue_add(struct qos_tc_qdisc *sch, int arbi, int prio_w, int idx,
		const struct qos_tc_params *tc_params);
int qos_tc_queue_del(struct qos_tc_qdisc *sch, int idx,
		const struct qos_tc_params *tc_params);

int qos_tc_qos_init(void);
int qos_tc_qos_destroy(void);

int qos_tc_add_sched(struct qos_tc_qdisc *sch, int prio,
		const struct qos_tc_params *tc_params);
int qos_tc_add_staged_sched(struct qos_tc_qdisc *psch,
		struct qos_tc_qdisc *csch, int prio,
		const struct qos_tc_params *tc_params);
int qos_tc_sched_del(struct qos_tc_qdisc *sch,
		const struct qos_tc_params *tc_params);
int qos_tc_add_child_qdisc(struct net_device *dev,
			   struct qos_tc_port *port,
			   enum qos_tc_qdisc_type type,
			   u32 parent,
			   u32 hanle,
			   const struct qos_tc_params *tc_params);

int qos_tc_fill_port_data(struct qos_tc_qdisc *sch,
		const struct qos_tc_params *tc_params);
int qos_tc_get_port_info(struct qos_tc_qdisc *sch,
		const struct qos_tc_params *tc_params);

int qos_tc_qdisc_unlink(struct qos_tc_port *p, struct qos_tc_qdisc *sch,
		const struct qos_tc_params *tc_params);
int qos_tc_qdisc_tree_del(struct qos_tc_port *p, struct qos_tc_qdisc *root,
		const struct qos_tc_params *tc_params);
int qos_tc_sched_status(struct qos_tc_port *p, struct qos_tc_qdisc *root);

int qos_tc_update_qmap(struct net_device *dev,
		       struct qos_tc_qmap_tc *q_tc,
		       bool en,
		       u32 *qid,
		       const struct qos_tc_params *tc_params);
int qos_tc_ev_tc_assign(struct qos_tc_qdisc *sch, u8 tc);
int qos_tc_ev_tc_unassign(struct qos_tc_qdisc *sch, u8 tc);
int qos_tc_mappings_init(void);

int qos_tc_ports_cleanup(void);

int qos_tc_qdata_get_by_type(struct net_device *dev,
			     struct qos_tc_q_data *qid,
			     enum qos_tc_qdata_type type,
			     u32 *handle, u32 *parent);
struct qos_tc_q_data *qos_tc_qdata_qid_get(struct net_device *dev,
					   struct qos_tc_qdisc *qdisc,
					   u32 parent);
int qos_tc_qdata_add(struct net_device *dev, struct qos_tc_q_data *qid,
		     u32 handle, u32 parent, enum qos_tc_qdata_type type,
		     int (*destroy)(struct net_device *dev, u32 handle,
				    u32 parent));
int qos_tc_qdata_add_tbf(struct net_device *dev, struct qos_tc_q_data *qid,
			 struct qos_tc_tbf_data *tbf,
			 int (*destroy)(struct net_device *dev, u32 handle,
					u32 parent));

int qos_tc_qdata_remove(struct net_device *dev, struct qos_tc_q_data *qid,
			u32 handle, u32 parent);

int qos_tc_shaper_add(struct qos_tc_qdisc *sch,
		      struct qos_tc_q_data *qdata,
		      struct qos_tc_tbf_data *tbf);

int qos_tc_shaper_remove(struct qos_tc_qdisc *sch,
			 struct qos_tc_q_data *qid);

int tbf_remove(struct net_device *dev, u32 handle, u32 parent);
int qos_tc_del_tbf_from_qdisc(struct qos_tc_qdisc *qdisc,
			      struct qos_tc_q_data *qid);

bool qos_tc_is_cpu_port(int port);

bool qos_tc_is_netdev_cpu_port(struct net_device *dev);

void qos_tc_port_list_debugfs(struct seq_file *file, void *ctx);

bool qos_tc_is_lan_dev(struct net_device *dev);
bool qos_tc_is_iphost_dev(struct net_device *dev);
bool qos_tc_is_gpon_dev(struct net_device *dev);
bool qos_tc_is_first_subif(struct net_device *dev);
bool qos_tc_is_vuni_dev(struct net_device *dev);
int qos_tc_queue_wred_defaults_set(struct qos_tc_qdisc *sch, int idx);

u64 psched_ns_t2l(const struct psched_ratecfg *r, u64 time_in_ns);
int qos_tc_check_qid(struct qos_tc_qdisc *qdisc, int idx);

int qos_tc_add_qdisc_to_dev(struct net_device *dev,
			    struct qos_tc_qdisc *qdisc, u32 handle);

int qos_tc_add_tbf_child_qdisc(struct qos_tc_port *port,
			       enum qos_tc_qdisc_type type,
			       u32 parent,
			       u32 handle);
int qos_tc_add_sch_to_tbf_port(struct qos_tc_port *port,
			       enum qos_tc_qdisc_type type,
			       u32 parent, u32 handle);

#endif
