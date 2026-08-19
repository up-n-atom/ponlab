/*
 * Copyright (C) 2020-2025 MaxLinear, Inc.
 * Copyright (C) 2020 Intel Corporation
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
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: dpl module for LGM
 */

#define pr_fmt(fmt) "dpl_lgm: %s:%d: " fmt, __func__, __LINE__

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/pp_api.h>
#include <linux/pp_qos_api.h>
#include <linux/dpl.h>
#include <net/datapath_api.h>
#include <net/datapath_api_qos.h>
#include <linux/if_ether.h>
#include <linux/in.h>
#include <linux/netdevice.h>
#ifdef CONFIG_MXL_CBM
#include <net/mxl_cbm_api.h>
#else
#include <net/intel_cbm_api.h>
#endif
#include "dpl_lgm_internal.h"
#include "dpl_monitor.h"
#include "dpl_logic.h"

#define MOD_NAME "dpl"

#define DPL_NUM_QOS_PORTS_PER_CPU        2
#define DPL_MAX_DP_QUEUES_IN_CPU_PORT    8
#define NUM_DPL_QUEUES_IN_HIGH_PRIO_PORT 5
#define NUM_DPL_QUEUES_IN_LOW_PRIO_PORT  1
#define DPL_SCHED_HIGH_PRIO              1
#define DPL_SCHED_LOW_PRIO               2

#define SNMP_PORT          161
#define DNS_PORT           53
#define DHCP_SERVER_PORT   67
#define DHCP_CLIENT_PORT   68
#define DHCPV6_SERVER_PORT 547
#define DHCPV6_CLIENT_PORT 546


/* dynamic bw limit is 75% from current bw */
#define DPL_DYNAMIC_BW_LIMIT_RATIO(curr) (((curr) * 3) / 4)
#define DPL_MIN_BW_LIMIT         (1000)
#define DPL_CALC_DYNAMIC_BW_LIMIT(curr)                                        \
	(max((unsigned int)DPL_MIN_BW_LIMIT, DPL_DYNAMIC_BW_LIMIT_RATIO(curr)))
/* BW Limit definitions (in Kb) */
#define DPL_QUEUE_DEFAULT_BW_LIMIT            DPL_MIN_BW_LIMIT
#define DPL_QUEUE_HASH_BIT_BW_LIMIT           80000
#define DPL_BWL_PRIO_0_QUEUE                  DPL_QUEUE_DEFAULT_BW_LIMIT
#define DPL_BWL_PRIO_1_QUEUE                  DPL_QUEUE_DEFAULT_BW_LIMIT
#define DPL_BWL_PRIO_2_QUEUE                  DPL_QUEUE_DEFAULT_BW_LIMIT
#define DPL_BWL_PRIO_3_QUEUE                  DPL_QUEUE_HASH_BIT_BW_LIMIT
#define DPL_BWL_PRIO_4_QUEUE                  DPL_QUEUE_DEFAULT_BW_LIMIT
/* priorities 5-7 mapped to the same queue */
#define DPL_BWL_PRIO_7_QUEUE                  7000

/* BW Limit for default DP queues under attack */
#define DPL_DFLT_RESTRICTED_BW_LIMIT          80000

#define DPL_QUEUE_PRIO_MAX_ALLOWED            128
#define DPL_QUEUE_LOW_PRIO_MAX_ALLOWED        64

#define DPL_HASH_CTRL_PRIO   1
/* DPL_ARP_PRIO get DPL_HASH_CTRL_PRIO priority because there
 * is no hashing support for this protocol.
 */
#define DPL_ARP_PRIO         DPL_HASH_CTRL_PRIO
#define DPL_NDP_PRIO         DPL_CTRL_PRIO
#define DPL_SNMP_PRIO        DPL_CTRL_PRIO
#define DPL_DHCP_PRIO        DPL_CTRL_PRIO
#define DPL_DNS_PRIO         DPL_CTRL_PRIO
#define DPL_PING_PRIO        DPL_CTRL_PRIO
#define DPL_HASH_DATA_PRIO   3

/* dynmaic action for bw limit for queues with hash priority and above */
#define DPL_DYNAMIC_BW_PRIO (DPL_HASH_DATA_PRIO)

#define INVALID_IFIND (0)

/* for docsis need to protect 2 DS gpids, data and voice */
#define DPL_MAX_GPIDS_PER_DEV 2
#define DPL_MAX_PROTECTED_DEVS 4

#define DPL_INDX_VOICE_DOCSIS_GPID 32

#define dpl_for_each_dev(i) for ((i) = 0; (i) < DPL_MAX_PROTECTED_DEVS; (i)++)

/**
 * @enum dpl_product_type
 * @brief dpl product type
 */
enum dpl_product_type {
	/*! GW */
	DPL_PRODUCT_TYPE_GW,

	/*! Bridge */
	DPL_PRODUCT_TYPE_BRIDGE,

	DPL_PRODUCT_TYPE_MAX,
};

/**
 * @struct dpl_cpu_qos_port_info
 * @brief CPU qos port information. Used when retrieved from DP
 * @port       QoS port logical id
 * @gpid       gpid
 * @queue      queues list (logical id's)
 * @num_q      Number of queues
 */
struct dpl_cpu_qos_port_info {
	unsigned int port;
	unsigned int gpid;
	unsigned int queue[DPL_MAX_DP_QUEUES_IN_CPU_PORT];
	unsigned short num_q;
};

/**
 * @struct dpl_cpu_qos_info
 * @brief CPU qos information. Used when retrieved from DP
 * @port       QoS port information
 * @num_ports  Number of ports
 */
struct dpl_cpu_qos_info {
	struct dpl_cpu_qos_port_info port[DPL_NUM_QOS_PORTS_PER_CPU];
	unsigned short num_ports;
};

/**
 * @struct dpl_dev
 * @brief device to protect info
 * @ifindex   ifindex of device
 * @num_ports number of gpids related to the device
 * @gpid      port gpid
 */
struct dpl_dev {
	int            ifindex;
	int            num_gpids;
	unsigned short gpid[DPL_MAX_GPIDS_PER_DEV];
};

/**
 * @struct dpl_lgm_db
 * @brief dpl lgm platform DB
 * @protected_devs  protected device
 * @prod_type       product type
 * @mon_params      DPL monitor params
 * @qdev            QoS device
 * @cpu_info        cpu queues information
 * @num_cpus        Number of cpus
 * @actions_state   enabled/disabled state per action
 */
struct dpl_lgm_db {
	spinlock_t                lock;
	struct dpl_dev            protected_devs[DPL_MAX_PROTECTED_DEVS];
	unsigned int              max_sessions;
	enum dpl_product_type     prod_type;
	struct dpl_monitor_param  mon_params;
	struct pp_qos_dev         *qdev;
	struct dpl_cpu_info       cpu_info[CQM_MAX_CPU];
	unsigned int              num_cpus;
	bool                      actions_state[DPL_LGM_ACTION_MAX];
};

static struct dpl_lgm_db *db;
struct dentry *parent_dir;

static inline bool is_dev_protected(int ifindex)
{
	int i;

	if (ifindex == INVALID_IFIND)
		return false;

	dpl_for_each_dev(i) {
		if (db->protected_devs[i].ifindex == ifindex)
			return true;
	}
	return false;
}

/* For PPS attribute */
static void get_num_host_rx_pkt(unsigned long long *val)
{
	unsigned long long pkts;

	*val = 0;

	if (pp_exception_pkts_get(&pkts))
		return;

	*val = pkts;
}

/* For DPS attribute */
static void get_num_host_rx_drop_pkt(unsigned long long *val)
{
	unsigned short i, j;
	int ret;
	struct pp_qos_queue_stat stat;

	/* Reset stat so statistics will not be zero-ed */
	memset(&stat, 0, sizeof(stat));

	*val = 0;

	for (i = 0; i < db->num_cpus; i++) {
		for (j = 0; j < db->cpu_info[i].num_q; j++) {
			ret = pp_qos_queue_stat_get(db->qdev,
				db->cpu_info[i].queue[j].queue, &stat);
			if (unlikely(ret)) {
				pr_err("Failed getting queue %u stat (ret %u)\n",
				       db->cpu_info[i].queue[j].queue, ret);
				return;
			}

			*val += stat.total_packets_dropped;
		}
	}
}

/* For SPS attribute */
static void get_num_created_sessions(unsigned long long *val)
{
	unsigned int created_sessions;
	int ret;

	*val = 0;

	ret = pp_created_sessions_get(&created_sessions);
	if (unlikely(ret)) {
		pr_err("Failed getting created sessions, ret %u\n", ret);
		return;
	}

	*val += created_sessions;
}

/* For SUT attribute */
static void get_session_utilization(unsigned long long *val)
{
	unsigned int open_sessions;
	int ret;

	ret = pp_open_sessions_get(&open_sessions);
	if (unlikely(ret)) {
		pr_err("Failed getting max sessions, ret %u\n", ret);
		return;
	}

	*val = (open_sessions * 100) / db->max_sessions;
}

/* Actions:
 *         White list - Prioritization, bit hash
 *         Switch acceleration mode (enable/disable, disable for gw)
 *         Increase minimum packets for session creation
 */

static void white_list_enable(void)
{
	unsigned short i, j;

	pr_debug("%s\n", __func__);

	spin_lock_bh(&db->lock);
	db->actions_state[DPL_LGM_ACTION_WHITE_LIST] = true;

	/* Protect all protected ports */
	dpl_for_each_dev(i) {
		if (db->protected_devs[i].ifindex == INVALID_IFIND)
			continue;
		for (j = 0; j < db->protected_devs[i].num_gpids; j++)
			if (db->protected_devs[i].gpid[j] != PP_PORT_INVALID)
				pp_port_protect(db->protected_devs[i].gpid[j]);
	}
	spin_unlock_bh(&db->lock);

	dpl_logic_hash_bit_activate();
}

static void white_list_disable(void)
{
	unsigned short i, j;

	pr_debug("%s\n", __func__);

	spin_lock_bh(&db->lock);
	db->actions_state[DPL_LGM_ACTION_WHITE_LIST] = false;

	/* Unprotect all ports */
	dpl_for_each_dev(i) {
		if (db->protected_devs[i].ifindex == INVALID_IFIND)
			continue;
		for (j = 0; j < db->protected_devs[i].num_gpids; j++)
			if (db->protected_devs[i].gpid[j] != PP_PORT_INVALID)
				pp_port_unprotect(db->protected_devs[i].gpid[j]);
	}
	spin_unlock_bh(&db->lock);

	dpl_logic_hash_bit_deactivate();
}

/**
 * @brief update the bw of q
 * @param q queue
 * @param is_restrict if true restrict the bw otherwise restore the original bw
 *        of the q
 */
static void update_q_bw_limit(struct dpl_cpu_queue_info *q, bool is_restrict)
{
	unsigned int old_bw, new_bw;
	struct pp_qos_queue_conf conf;

	/* if not restrict and the bw for the q is not changed, nothing to do */
	if (!is_restrict && !q->is_bw_changed)
		return;

	if (pp_qos_queue_conf_get(db->qdev, q->queue, &conf))
		return;

	if (!is_restrict) {
		new_bw = q->dflt_bw;
		q->is_bw_changed = false;
	} else {
		old_bw = conf.common_prop.bandwidth_limit;
		if (old_bw == 0)
			new_bw = DPL_DFLT_RESTRICTED_BW_LIMIT;
		else
			new_bw = DPL_CALC_DYNAMIC_BW_LIMIT(old_bw);

		if (!q->is_bw_changed) {
			q->dflt_bw = old_bw;
			q->is_bw_changed = true;
		}
	}

	conf.common_prop.bandwidth_limit = new_bw;
	pp_qos_queue_set(db->qdev, q->queue, &conf);
}

static void restrict_dflt_queues_bw_limit_enable(void)
{
	unsigned short i, j;
	struct dpl_cpu_queue_info *q;

	pr_debug("%s\n", __func__);

	db->actions_state[DPL_LGM_ACTION_RESTRICT_DFLT_QUEUES_BW_LIMIT] = true;

	for (i = 0; i < db->num_cpus; i++) {
		for (j = 0; j < db->cpu_info[i].num_q; j++) {
			q = &db->cpu_info[i].queue[j];
			/* changing only DP queues */
			if (!q->dpl_owner) {
				spin_lock_bh(&db->lock);
				update_q_bw_limit(q, true);
				spin_unlock_bh(&db->lock);
			}
		}
	}
}

static void restrict_dflt_queues_bw_limit_dynamic(void)
{
	unsigned short i, j;
	struct dpl_cpu_queue_info *q;

	pr_debug("%s\n", __func__);
	for (i = 0; i < db->num_cpus; i++) {
		/* changing DP queues or prio 7 queue */
		for (j = DPL_DYNAMIC_BW_PRIO; j < db->cpu_info[i].num_q; j++) {
			q = &db->cpu_info[i].queue[j];
			spin_lock_bh(&db->lock);
			update_q_bw_limit(q, true);
			spin_unlock_bh(&db->lock);
		}
	}
}

static void restrict_dflt_queues_bw_limit_disable(void)
{
	unsigned short i, j;
	struct dpl_cpu_queue_info *q;

	pr_debug("%s\n", __func__);

	db->actions_state[DPL_LGM_ACTION_RESTRICT_DFLT_QUEUES_BW_LIMIT] = false;

	for (i = 0; i < db->num_cpus; i++) {
		for (j = 0; j < db->cpu_info[i].num_q; j++) {
			q = &db->cpu_info[i].queue[j];
			spin_lock_bh(&db->lock);
			update_q_bw_limit(q, false);
			spin_unlock_bh(&db->lock);
		}
	}
}

int _dpl_to_pp_fields(struct dpl_whitelist_field *dpl_fields,
				struct pp_whitelist_field *pp_fields, u32 cnt)
{
	u32 i;
	struct dpl_whitelist_field *dpl_field;
	struct pp_whitelist_field *pp_field;

	if (dpl_fields == NULL)
		return -EINVAL;

	for (i = 0 ; i < cnt; i++) {
		dpl_field = &dpl_fields[i];
		pp_field = &pp_fields[i];
		switch (dpl_field->type) {
		case DPL_FLD_SRC_MAC:
			pp_field->type = PP_FLD_SRC_MAC;
			memcpy(pp_field->src_mac,
				dpl_field->src_mac,
				sizeof(pp_field->src_mac));
			break;
		case DPL_FLD_DST_MAC:
			pp_field->type = PP_FLD_DST_MAC;
			memcpy(pp_field->dst_mac,
				dpl_field->dst_mac,
				sizeof(pp_field->dst_mac));
			break;
		case DPL_FLD_ETH_TYPE:
			pp_field->type = PP_FLD_ETH_TYPE;
			pp_field->eth_type = dpl_field->eth_type;
			break;
		case DPL_FLD_IPV4_SRC_IP:
			pp_field->type = PP_FLD_IPV4_SRC_IP;
			pp_field->src_ip.v4 = dpl_field->src_ip.v4;
			break;
		case DPL_FLD_IPV4_DST_IP:
			pp_field->type = PP_FLD_IPV4_DST_IP;
			pp_field->dst_ip.v4 = dpl_field->dst_ip.v4;
			break;
		case DPL_FLD_IPV6_SRC_IP:
			pp_field->type = PP_FLD_IPV6_SRC_IP;
			memcpy(&pp_field->src_ip.v6,
				&dpl_field->src_ip.v6,
				sizeof(pp_field->src_ip.v6));
			break;
		case DPL_FLD_IPV6_DST_IP:
			pp_field->type = PP_FLD_IPV6_DST_IP;
			memcpy(&pp_field->dst_ip.v6,
				&dpl_field->dst_ip.v6,
				sizeof(pp_field->dst_ip.v6));
			break;
		case DPL_FLD_IP_PROTO:
			pp_field->type = PP_FLD_IP_PROTO;
			pp_field->ip_proto = dpl_field->ip_proto;
			break;
		case DPL_FLD_L4_SRC_PORT:
			pp_field->type = PP_FLD_L4_SRC_PORT;
			pp_field->src_port = dpl_field->src_port;
			break;
		case DPL_FLD_L4_DST_PORT:
			pp_field->type = PP_FLD_L4_DST_PORT;
			pp_field->dst_port = dpl_field->dst_port;
			break;
		case DPL_FLD_ICMP_TYPE_CODE:
			pp_field->type = PP_FLD_ICMP_TYPE_CODE;
			pp_field->icmp_type_code = dpl_field->icmp_type_code;
			break;
		default:
			pr_err("unknown dpl field type %u\n", dpl_field->type);
			return -EINVAL;
		}
	}

	return 0;
}

int dpl_whitelist_rule_add(unsigned int prio,
			struct dpl_whitelist_field *fields, unsigned int cnt)
{
	int ret;
	struct pp_whitelist_field *pp_fields;

	pp_fields = kzalloc(cnt * sizeof(struct pp_whitelist_field), GFP_KERNEL);
	if (!pp_fields) {
		pr_err("failed to alloc pp_whitelist_field\n");
		return -EINVAL;
	}
	ret = _dpl_to_pp_fields(fields, pp_fields, cnt);
	if (ret) {
		pr_err("dpl fields translation to pp fields failed\n");
		goto done;
	}

	ret = pp_whitelist_rule_add(prio, pp_fields, cnt);
done:
	kfree(pp_fields);

	return ret;
}
EXPORT_SYMBOL(dpl_whitelist_rule_add);

int dpl_whitelist_rule_del(unsigned int prio, struct dpl_whitelist_field *fields,
			   unsigned int cnt)
{
	int ret;
	struct pp_whitelist_field *pp_fields;

	pp_fields = kzalloc(cnt * sizeof(struct pp_whitelist_field), GFP_KERNEL);
	if (!pp_fields) {
		pr_err("failed to alloc pp_whitelist_field\n");
		return -EINVAL;
	}
	ret = _dpl_to_pp_fields(fields, pp_fields, cnt);
	if (ret) {
		pr_err("dpl fields translation to pp fields failed\n");
		goto done;
	}

	ret =  pp_whitelist_rule_del(prio, pp_fields, cnt);

done:
	kfree(pp_fields);

	return ret;
}
EXPORT_SYMBOL(dpl_whitelist_rule_del);

static void l2_class_enable(void)
{
	pr_debug("%s\n", __func__);

	db->actions_state[DPL_LGM_ACTION_L2_CLASS] = true;
}

static void l2_class_disable(void)
{
	pr_debug("%s\n", __func__);

	db->actions_state[DPL_LGM_ACTION_L2_CLASS] = false;
}

static void inc_min_pkt_for_sess_create_enable(void)
{
	pr_debug("%s\n", __func__);

	db->actions_state[DPL_LGM_ACTION_INC_MIN_PKT_FOR_SESS_CREATE] = true;
}

static void inc_min_pkt_for_sess_create_disable(void)
{
	pr_debug("%s\n", __func__);

	db->actions_state[DPL_LGM_ACTION_INC_MIN_PKT_FOR_SESS_CREATE] = false;
}

int dpl_set_action(enum dpl_lgm_action action, enum dpl_thr_type type,
		   enum dpl_attr_type attr, unsigned int val,
		   unsigned int num_seq_iter, unsigned int num_iter,
		   enum dpl_attr_type dependency, bool do_register)
{
	unsigned int i;
	unsigned int j;
	bool is_enabled = false;

	spin_lock_bh(&db->lock);

	db->mon_params.act[action].thr[type][attr].val = val;
	db->mon_params.act[action].thr[type][attr].num_seq_iter = num_seq_iter;
	db->mon_params.act[action].thr[type][attr].num_iter = num_iter;
	db->mon_params.act[action].thr[type][attr].dependency = dependency;
	db->mon_params.act[action].thr[type][attr].is_active = true;
	db->mon_params.act[action].enable = NULL;
	db->mon_params.act[action].disable = NULL;
	db->mon_params.act[action].dynamic = NULL;

	for (i = 0; i < DPL_ATTR_MAX && !is_enabled; i++)
		for (j = 0; j < DPL_THR_TYPE_MAX; j++)
			if (db->mon_params.act[action].thr[j][i].val) {
				is_enabled = true;
				break;
			}

	if (is_enabled) {
		switch (action) {
		case DPL_LGM_ACTION_WHITE_LIST:
			db->mon_params.act[action].enable = white_list_enable;
			db->mon_params.act[action].disable =
				white_list_disable;
			break;
		case DPL_LGM_ACTION_L2_CLASS:
			db->mon_params.act[action].enable =
				l2_class_enable;
			db->mon_params.act[action].disable =
				l2_class_disable;
			break;
		case DPL_LGM_ACTION_INC_MIN_PKT_FOR_SESS_CREATE:
			db->mon_params.act[action].enable =
				inc_min_pkt_for_sess_create_enable;
			db->mon_params.act[action].disable =
				inc_min_pkt_for_sess_create_disable;
			break;
		case DPL_LGM_ACTION_RESTRICT_DFLT_QUEUES_BW_LIMIT:
			db->mon_params.act[action].enable =
				restrict_dflt_queues_bw_limit_enable;
			db->mon_params.act[action].dynamic =
				restrict_dflt_queues_bw_limit_dynamic;
			db->mon_params.act[action].disable =
				restrict_dflt_queues_bw_limit_disable;
			break;
		default:
			break;
		}
	}

	if (do_register) {
		dpl_mon_unregister();
		dpl_mon_register(&db->mon_params);
	}

	spin_unlock_bh(&db->lock);

	return 0;
}

static void dpl_configure_actions(void)
{
	/* Configure white list thresholds */
	/* To enable/disable white list action there 2 attributes: PPS && CPU */
	dpl_set_action(DPL_LGM_ACTION_WHITE_LIST, THR_ON, DPL_PPS,
		       DPL_ACTION_WHITE_LIST_THR_ON_PPS_VAL,
		       DPL_ACTION_WHITE_LIST_THR_ON_PPS_SEQ_ITER,
		       DPL_ACTION_WHITE_LIST_THR_ON_PPS_ITER,
		       DPL_ACTION_WHITE_LIST_THR_ON_PPS_DEP, false);
	dpl_set_action(DPL_LGM_ACTION_WHITE_LIST, THR_OFF, DPL_PPS,
		       DPL_ACTION_WHITE_LIST_THR_OFF_PPS_VAL,
		       DPL_ACTION_WHITE_LIST_THR_OFF_PPS_SEQ_ITER,
		       DPL_ACTION_WHITE_LIST_THR_OFF_PPS_ITER,
		       DPL_ACTION_WHITE_LIST_THR_OFF_PPS_DEP, false);

	dpl_set_action(DPL_LGM_ACTION_WHITE_LIST, THR_ON, DPL_CPU,
		       DPL_ACTION_WHITE_LIST_THR_ON_CPU_VAL,
		       DPL_ACTION_WHITE_LIST_THR_ON_CPU_SEQ_ITER,
		       DPL_ACTION_WHITE_LIST_THR_ON_CPU_ITER,
		       DPL_ACTION_WHITE_LIST_THR_ON_CPU_DEP, false);
	dpl_set_action(DPL_LGM_ACTION_WHITE_LIST, THR_OFF, DPL_CPU,
		       DPL_ACTION_WHITE_LIST_THR_OFF_CPU_VAL,
		       DPL_ACTION_WHITE_LIST_THR_OFF_CPU_SEQ_ITER,
		       DPL_ACTION_WHITE_LIST_THR_OFF_CPU_ITER,
		       DPL_ACTION_WHITE_LIST_THR_OFF_CPU_DEP, false);

	/* Configure restrict default queues bw thresholds */
	/* To enable/disable bw-limit action there 2 attributes: PPS && CPU */
	dpl_set_action(DPL_LGM_ACTION_RESTRICT_DFLT_QUEUES_BW_LIMIT, THR_ON, DPL_PPS,
		       DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_ON_PPS_VAL,
		       DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_ON_PPS_SEQ_ITER,
		       DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_ON_PPS_ITER,
		       DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_ON_PPS_DEP, false);
	dpl_set_action(DPL_LGM_ACTION_RESTRICT_DFLT_QUEUES_BW_LIMIT, THR_OFF, DPL_PPS,
		       DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_OFF_PPS_VAL,
		       DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_OFF_PPS_SEQ_ITER,
		       DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_OFF_PPS_ITER,
		       DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_OFF_PPS_DEP, false);

	dpl_set_action(DPL_LGM_ACTION_RESTRICT_DFLT_QUEUES_BW_LIMIT, THR_ON, DPL_CPU,
		       DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_ON_CPU_VAL,
		       DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_ON_CPU_SEQ_ITER,
		       DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_ON_CPU_ITER,
		       DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_ON_CPU_DEP, false);
	dpl_set_action(DPL_LGM_ACTION_RESTRICT_DFLT_QUEUES_BW_LIMIT, THR_OFF, DPL_CPU,
		       DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_OFF_CPU_VAL,
		       DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_OFF_CPU_SEQ_ITER,
		       DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_OFF_CPU_ITER,
		       DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_OFF_CPU_DEP, false);

	dpl_mon_register(&db->mon_params);
}

int dpl_protect(void)
{
	return 0;
}

int dpl_unprotect(void)
{
	return 0;
}

/**
 * @brief create a scheduler under port
 * @param port          port id
 * @param prio          priority
 * @param id            [OUT] new created id
 */
static int create_sched_under_port(unsigned int port, unsigned int prio,
				   int *id)
{
	struct dp_node_link node_sched = {0};

	node_sched.inst = 0;
	node_sched.dp_port = 0;
	node_sched.node_type = DP_NODE_SCH;
	node_sched.node_id.sch_id = DP_NODE_AUTO_ID;
	node_sched.arbi = ARBITRATION_WSP;
	node_sched.prio_wfq = prio;
	node_sched.leaf = 0;
	node_sched.p_node_type = DP_NODE_PORT;
	node_sched.p_node_id.cqm_deq_port = port;
	node_sched.cqm_deq_port.cqm_deq_port = port;

	if (dp_node_link_add(&node_sched, 0) == DP_FAILURE) {
		pr_err("Failed to add sched node\n");
		return DP_FAILURE;
	}

	*id = node_sched.node_id.sch_id;

	return 0;
}

/**
 * @brief Move queues to be under a scheduler
 * @param port          port id
 * @param queue         queues list
 * @param num_queues    number of queues in list
 * @param dst_sched     new parent (scheduler)
 */
static int move_existing_queues(unsigned int port, unsigned int *queue,
			unsigned short num_queues, unsigned int dst_sched)
{
	struct dp_node_link node_info = {0};
	struct pp_qos_queue_info qinfo;
	int i;

	node_info.inst = 0;
	node_info.dp_port = 0;
	node_info.node_type = DP_NODE_QUEUE;
	node_info.arbi = ARBITRATION_WSP;
	node_info.leaf = 0;
	node_info.p_node_type = DP_NODE_SCH;
	node_info.cqm_deq_port.cqm_deq_port = port;

	for (i = 0; i < num_queues; i++) {
		pr_debug("Moving q %u\n", queue[i]);

		node_info.p_node_id.sch_id = dst_sched;

		if (pp_qos_queue_info_get(db->qdev, queue[i], &qinfo)) {
			pr_err("failed getting queue %u info\n", queue[i]);
			return -EIO;
		}
		node_info.node_id.q_id = qinfo.physical_id;

		if (dp_node_link_add(&node_info, 0) == DP_FAILURE) {
			pr_err("Failed to move queue %u\n", queue[i]);
			return -EIO;
		}
	}

	return 0;
}

static void db_q_add(unsigned int cpu_id, unsigned int port_id,
		     unsigned int gpid, unsigned int q_id, bool dpl_owner)
{
	unsigned short num_q;

	num_q = db->cpu_info[cpu_id].num_q;
	db->cpu_info[cpu_id].queue[num_q].port = port_id;
	db->cpu_info[cpu_id].queue[num_q].gpid = gpid;
	db->cpu_info[cpu_id].queue[num_q].queue = q_id;
	db->cpu_info[cpu_id].queue[num_q].dpl_owner = dpl_owner;
	db->cpu_info[cpu_id].num_q++;
}

/**
 * @brief Sets bw limit to a queue
 * @param qid             queue id
 * @param high_prio_port  is high priority port (CPU has 2 ports)
 * @param prio            priority
 */
static void set_queue_bwl(int qid, bool high_prio_port, int prio)
{
	struct dp_shaper_conf shaper;

	/* set shaper */
	memset(&shaper, 0, sizeof(shaper));

	shaper.type = DP_NODE_QUEUE;
	shaper.id.q_id = qid;
	dp_shaper_conf_get(&shaper, 0);
	shaper.cmd = DP_SHAPER_CMD_ADD;

	shaper.cir = DPL_QUEUE_DEFAULT_BW_LIMIT;
	if (high_prio_port) {
		switch (prio) {
		case 0:
			shaper.cir = DPL_BWL_PRIO_0_QUEUE;
			break;
		case 1:
			shaper.cir = DPL_BWL_PRIO_1_QUEUE;
			break;
		case 2:
			shaper.cir = DPL_BWL_PRIO_2_QUEUE;
			break;
		case 3:
			shaper.cir = DPL_BWL_PRIO_3_QUEUE;
			break;
		case 4:
			shaper.cir = DPL_BWL_PRIO_4_QUEUE;
			break;
		}
	} else {
		switch (prio) {
		case 0:
			shaper.cir = DPL_BWL_PRIO_7_QUEUE;
			break;
		}
	}

	dp_shaper_conf_set(&shaper, 0);
}


/**
 * @brief Sets max_allowed to a queue
 * @param qid             queue id
 * @param high_prio_port  is high priority port (CPU has 2 ports)
 * @param prio            priority
 */
static void set_queue_max_allowed(int qid, bool high_prio_port, int prio)
{
	struct pp_qos_queue_conf conf;

	if (pp_qos_queue_conf_get(db->qdev, qid, &conf))
		return;

	if (high_prio_port) {
		switch (prio) {
		case 0:
			conf.wred_max_allowed = DPL_QUEUE_PRIO_MAX_ALLOWED;
			break;
		case 1:
			conf.wred_max_allowed = DPL_QUEUE_PRIO_MAX_ALLOWED;
			break;
		case 2:
			conf.wred_max_allowed = DPL_QUEUE_PRIO_MAX_ALLOWED;
			break;
		case 4:
			conf.wred_max_allowed = DPL_QUEUE_LOW_PRIO_MAX_ALLOWED;
			break;
		}
	} else {
		switch (prio) {
		case 0:
			conf.wred_max_allowed = DPL_QUEUE_LOW_PRIO_MAX_ALLOWED;
			break;
		}
	}

	pp_qos_queue_set(db->qdev, qid, &conf);
}

/**
 * @brief Creates host qos queues
 * @param cpu_id    queue id
 * @param port_id   port logical id
 * @param gpid      gpid
 * @param port      port physical id
 * @param sched     scheduler id (parent)
 * @param high_prio is high priority port (CPU has 2 ports)
 */
static int create_host_qos_queues(unsigned int cpu_id, unsigned int port_id,
	unsigned int gpid, unsigned int port, unsigned int sched,
	bool high_prio)
{
	struct dp_node_link node_info = {0};
	unsigned short num_queues_to_create;
	unsigned int qid;
	int i;

	node_info.inst = 0;
	node_info.dp_port = 0;
	node_info.node_type = DP_NODE_QUEUE;
	node_info.arbi = ARBITRATION_WSP;
	node_info.leaf = 0;
	node_info.p_node_type = DP_NODE_SCH;
	node_info.cqm_deq_port.cqm_deq_port = port;
	node_info.p_node_id.sch_id = sched;

	num_queues_to_create = high_prio ? NUM_DPL_QUEUES_IN_HIGH_PRIO_PORT :
					   NUM_DPL_QUEUES_IN_LOW_PRIO_PORT;

	for (i = 0 ; i < num_queues_to_create ; i++) {
		node_info.node_id.q_id = DP_NODE_AUTO_ID;
		node_info.prio_wfq = i;
		if (dp_node_link_add(&node_info, 0) == DP_FAILURE) {
			pr_err("Failed to add queue node\n");
			return -EIO;
		}
		qid = pp_qos_queue_id_get(db->qdev, node_info.node_id.q_id);
		db_q_add(cpu_id, port_id, gpid, qid, true);

		/* set queue bw limit and max_allowed */
		set_queue_bwl(node_info.node_id.q_id, high_prio, i);
		set_queue_max_allowed(qid, high_prio, i);

		pr_debug("Queue was added. id %d physical qid %d parent id %d\n",
		       qid, node_info.node_id.q_id, node_info.p_node_id.sch_id);
	}

	return 0;
}

/**
 * @brief Builds host QoS on specific port
 * @param port_id    port logical id
 * @param high_prio  is high priority port (CPU has 2 ports)
 * @param queue      queues currently under port
 * @param num_queues number of queues
 * @param cpu_id     cpu id
 * @param gpid       gpid
 */
static int build_host_qos(unsigned int port_id, bool high_prio,
	unsigned int *queue, unsigned short num_queues, unsigned int cpu_id,
	unsigned int gpid)
{
	int high_prio_sched, low_prio_sched;
	int i;
	unsigned int sched;
	struct pp_qos_port_info pinfo;

	pr_debug("Port %u high %u num q %u cpu %u gpid %u\n",
		 port_id, high_prio, num_queues, cpu_id, gpid);

	if (pp_qos_port_info_get(db->qdev, port_id, &pinfo)) {
		pr_err("failed getting port %u info\n", port_id);
		return -EIO;
	}

	if (create_sched_under_port(pinfo.physical_id, DPL_SCHED_HIGH_PRIO,
				    &high_prio_sched)) {
		pr_err("Failed to add sched node (prio %u)\n",
		       DPL_SCHED_HIGH_PRIO);
		return -EIO;
	}

	if (create_sched_under_port(pinfo.physical_id, DPL_SCHED_LOW_PRIO,
				    &low_prio_sched)) {
		pr_err("Failed to add sched node (prio %u)\n",
		       DPL_SCHED_LOW_PRIO);
		return -EIO;
	}

	pr_debug("high prio sched %u low prio sched %u\n",
		 high_prio_sched, low_prio_sched);

	sched = high_prio ? low_prio_sched : high_prio_sched;
	if (move_existing_queues(pinfo.physical_id, queue,
				 num_queues, sched)) {
		pr_err("Failed moving queues under schedulers\n");
		return -EIO;
	}

	/* Update dp queues in DB. In low prio port,
	 * their priority is higher than the Host QoS queues
	 */
	if (!high_prio)
		for (i = 0; i < num_queues; i++)
			db_q_add(cpu_id, port_id, gpid, queue[i], false);

	sched = high_prio ? high_prio_sched : low_prio_sched;
	if (create_host_qos_queues(cpu_id, port_id, gpid, pinfo.physical_id,
				   sched, high_prio)) {
		pr_err("Failed creating new host QoS queues (port %u)\n",
		       port_id);
		return -EIO;
	}

	/* Update dp queues in DB. In high prio port,
	 * their priority is lower than the Host QoS queues
	 */
	if (high_prio)
		for (i = 0; i < num_queues; i++)
			db_q_add(cpu_id, port_id, gpid, queue[i], false);

	return 0;
}

/**
 * @brief Init Host QoS
 */
static int dpl_init_host_qos(void)
{
	int ret;
	struct dp_dequeue_res dp_res = {0};
	struct dp_queue_res *q_res;
	unsigned int i, j;
	struct dpl_cpu_qos_info cpu[CQM_MAX_CPU] = {0};
	unsigned short cpu_id;
	unsigned short port_idx;
	unsigned short queue_idx;
	bool new_port;

	dp_res.cqm_deq_idx = DEQ_PORT_OFFSET_ALL;
	dp_res.cqm_deq_port = DEQ_PORT_OFFSET_ALL;
	dp_res.q_res_size = 0;
	dp_res.q_res = NULL;

	/* Retrieve the number of queues */
	ret = dp_deq_port_res_get(&dp_res, 0);
	if (ret) {
		pr_err("failed to get dp resources (err %d)\n", ret);
		return -EIO;
	}

	dp_res.q_res_size = dp_res.num_q;
	q_res = kcalloc(dp_res.num_q, sizeof(struct dp_queue_res), GFP_KERNEL);
	if (!q_res)
		return -ENOMEM;

	dp_res.q_res = q_res;

	/* Retrieve host ports/queues information from dp */
	ret = dp_deq_port_res_get(&dp_res, 0);
	if (ret) {
		pr_err("failed to get dp resources (err %d)\n", ret);
		return -EIO;
	}

	/* Retrieve cpu QoS ports/queues information */
	pr_debug("number of queues: %u\n", dp_res.num_q);
	for (i = 0; i < dp_res.num_q; i++) {
		pr_debug("queue: %u (phy %u port %u) cpu %u, gpid %u\n",
			 q_res[i].q_node, q_res[i].q_id,
			 q_res[i].qos_deq_port,
			 q_res[i].cpu_id, q_res[i].cpu_gpid);

		cpu_id = q_res[i].cpu_id;
		if (cpu_id >= db->num_cpus) {
			pr_err("cpu_id %u exceeds num of active cpu (%u)",
			       cpu_id, db->num_cpus);
			kfree(q_res);
			return -EIO;
		}
		port_idx = cpu[cpu_id].num_ports;
		new_port = true;

		/* if there is already such port, use its entry */
		for (j = 0; j < port_idx; j++) {
			if (cpu[cpu_id].port[j].port ==
			    q_res[i].qos_deq_port) {
				port_idx = j;
				new_port = false;
			}
		}

		if (new_port && (port_idx >= DPL_NUM_QOS_PORTS_PER_CPU)) {
			pr_err("Too many ports %u in cpu %u\n",
			       port_idx, cpu_id);
			return -EIO;
		}

		queue_idx = cpu[cpu_id].port[port_idx].num_q;

		cpu[cpu_id].port[port_idx].port = q_res[i].qos_deq_port;
		cpu[cpu_id].port[port_idx].gpid = q_res[i].cpu_gpid;
		cpu[cpu_id].port[port_idx].queue[queue_idx] = q_res[i].q_node;

		cpu[cpu_id].port[port_idx].num_q++;
		if (new_port)
			cpu[cpu_id].num_ports++;
	}

	kfree(q_res);

	/* Build host QoS for each cpu port */
	for (i = 0; i < db->num_cpus; i++) {
		if (cpu[i].num_ports != DPL_NUM_QOS_PORTS_PER_CPU) {
			pr_err("CPU %u has %u ports\n", i, cpu[i].num_ports);
			return -EIO;
		}

		for (j = 0; j < cpu[i].num_ports; j++) {
			ret = build_host_qos(cpu[i].port[j].port,
					     (j == 0) ? true : false,
					     cpu[i].port[j].queue,
					     cpu[i].port[j].num_q,
					     i,
					     cpu[i].port[j].gpid);
			if (ret) {
				pr_err("build host qos port %u failed\n",
				       cpu[i].port[j].port);
				return ret;
			}
		}
	}

	return 0;
}

static int dpl_init_pp_cpu_info(void)
{
	struct pp_cpu_info pp_cpu_info[PP_MAX_HOST_CPUS] = { 0 };
	struct dpl_cpu_info *dpl_cpu;
	struct pp_cpu_info *pp_cpu;
	unsigned int i, j;

	pp_cpu = &pp_cpu_info[0];
	dpl_cpu = &db->cpu_info[0];
	/* convert dpl db dpu info to PP structures */
	for (j = 0, i = 0; i < db->num_cpus; i++, pp_cpu++, dpl_cpu++) {
		for (j = 0; j < dpl_cpu->num_q; j++) {
			if (!dpl_cpu->queue[j].dpl_owner)
				continue;
			pp_cpu->queue[pp_cpu->num_q].id =
				dpl_cpu->queue[j].queue;
			pp_cpu->queue[pp_cpu->num_q].gpid =
				dpl_cpu->queue[j].gpid;
			pp_cpu->num_q++;
		}
	}

	return pp_host_cpu_info_init(pp_cpu_info, db->num_cpus);
}

static void dump_dpl_actions_state(void)
{
	unsigned int act;

	for (act = 0; act < DPL_LGM_ACTION_MAX; act++)
		pr_info("ACTION %u state ==> %d\n",
			act, db->actions_state[act]);
}

void dpl_retrieve_cpu_info(struct dpl_cpu_info *cpu_info,
			   unsigned int *num_cpus)
{
	*num_cpus = db->num_cpus;

	memcpy(cpu_info, db->cpu_info, sizeof(db->cpu_info));
}

void dpl_test(enum test_operation op, unsigned int port, unsigned int queue)
{
	struct dp_dequeue_res dp_res = {0};
	struct dp_queue_res *q_res;
	unsigned short i;

	switch (op) {
	case test_operation_dump_dp_info:
		dp_res.cqm_deq_idx = DEQ_PORT_OFFSET_ALL;
		dp_res.cqm_deq_port = DEQ_PORT_OFFSET_ALL;
		dp_res.q_res_size = 0;
		dp_res.q_res = NULL;

		/* Retrieve the number of queues */
		if (dp_deq_port_res_get(&dp_res, 0)) {
			pr_err("failed to get dp resources\n");
			return;
		}
		dp_res.q_res_size = dp_res.num_q;
		q_res = kcalloc(dp_res.num_q, sizeof(struct dp_queue_res),
				GFP_KERNEL);
		if (!q_res)
			return;

		dp_res.q_res = q_res;

		/* Retrieve host ports/queues information from dp */
		if (dp_deq_port_res_get(&dp_res, 0)) {
			pr_err("failed to get dp resources\n");
			return;
		}

		/* dump dp info */
		pr_info("number of queues: %u\n", dp_res.num_q);
		for (i = 0; i < dp_res.num_q; i++)
			pr_info("queue: %u (phy %u port %u) cpu %u, gpid %u\n",
				q_res[i].q_node, q_res[i].q_id,
				q_res[i].qos_deq_port,
				q_res[i].cpu_id, q_res[i].cpu_gpid);
		kfree(q_res);

		break;
	case test_operation_dump_protected:
		/* dump protected ports */
		dpl_protected_devs_dump();
		break;
	case test_operation_white_list_en:
		/* white list enable */
		white_list_enable();
		break;
	case test_operation_white_list_dis:
		/* white list disable */
		white_list_disable();
		break;
	case test_operation_dpl_enable:
		/* dpl enable */
		dpl_configure_actions();
		break;
	case test_operation_dump_mon_params:
		/* dump mon params */
		dpl_mon_param_dump(&db->mon_params);
		break;
	case test_operation_dump_actions_state:
		/* dump actions state */
		dump_dpl_actions_state();
		break;
	default:
		pr_err("op %d not supported\n", op);
		break;
	}
}

void dpl_protected_devs_dump(void)
{
	int i, j;

	if (!db)
		return;

	pr_info("protected devs:\n");
	dpl_for_each_dev(i) {
		if (db->protected_devs[i].ifindex == INVALID_IFIND)
			continue;
		pr_info("ifindex=%d, gpids[%d]: {",
			db->protected_devs[i].ifindex,
			db->protected_devs[i].num_gpids);
		for (j = 0; j < db->protected_devs[i].num_gpids; j++)
			pr_cont("%u, ", db->protected_devs[i].gpid[j]);
		pr_cont("}\n");
	}
}

struct activity_data {
	unsigned long long last_used[NR_CPUS];
	unsigned long long last_boot;
};

static struct activity_data activity_data;

/**
 * @brief calc cpu usage for each cpu, and return the max usage
 * @param usage ptr to return the max usage
 */
static void sysidle_check_cpu(unsigned long long *usage)
{
	int i;
	unsigned long long curr_cpu_used;
	unsigned long long curr_boot;
	int diff_used;
	int diff_boot;
	int cpu_usage;
	int max_usage = 0;

	curr_boot = ktime_get_boottime_ns();
	diff_boot = (curr_boot - activity_data.last_boot) >> 16;
	activity_data.last_boot = curr_boot;

	for_each_online_cpu(i) {
		curr_cpu_used = kcpustat_cpu(i).cpustat[CPUTIME_USER]
			      + kcpustat_cpu(i).cpustat[CPUTIME_NICE]
			      + kcpustat_cpu(i).cpustat[CPUTIME_SYSTEM]
			      + kcpustat_cpu(i).cpustat[CPUTIME_SOFTIRQ]
			      + kcpustat_cpu(i).cpustat[CPUTIME_IRQ];

		diff_used = (curr_cpu_used - activity_data.last_used[i]) >> 16;
		activity_data.last_used[i] = curr_cpu_used;

		if (diff_boot <= 0 || diff_used < 0)
			cpu_usage = 0;
		else
			cpu_usage = min(100 ,(100 * diff_used / diff_boot));

		if (cpu_usage > max_usage)
			max_usage = cpu_usage;
	}

	*usage = (unsigned long long)max_usage;
}

int dpl_dev_protect(struct net_device *dev)
{
	dp_subif_t *subif;
	int ret, ifindex, num_gpids, i, j;

	if (!db || !dev || dev->ifindex == INVALID_IFIND)
		return -EINVAL;

	ifindex = dev->ifindex;
	subif = kzalloc(sizeof(*subif), GFP_KERNEL);
	if (!subif) {
		pr_err("failed to allocate memory for subif\n");
		return -ENOMEM;
	}

	ret = dp_get_netif_subifid(dev, NULL, NULL, NULL, subif, 0);
	if (ret) {
		pr_err("failed to get subif from device: %s\n", dev->name);
		goto free;
	}

	spin_lock_bh(&db->lock);

	if (is_dev_protected(ifindex)) {
		pr_debug("dev %d is already protected\n", ifindex);
		goto unlock;
	}

	/* find free index for new dev */
	dpl_for_each_dev(i)
		if (db->protected_devs[i].ifindex == INVALID_IFIND)
			break;

	if (i == ARRAY_SIZE(db->protected_devs)) {
		pr_err("protected_devs is full\n");
		ret = -ENOSPC;
		goto unlock;
	}

	ret = pktprs_dev_add(dev);
	if (ret) {
		pr_err("falied to add dev %s to pktprs\n", dev->name);
		goto unlock;
	}

	db->protected_devs[i].ifindex = ifindex;
	if (subif->subif_num == 0) {
		pr_err("no gpids for dev %s\n", dev->name);
		ret = -EINVAL;
		goto unlock;
	}
	num_gpids = 1;
	db->protected_devs[i].gpid[0] = subif->gpid_list[0];
	/* workaround to add docsis voice gpid */
	if (subif->subif_num > DPL_INDX_VOICE_DOCSIS_GPID) {
		db->protected_devs[i].gpid[1] =
				   subif->gpid_list[DPL_INDX_VOICE_DOCSIS_GPID];
		num_gpids++;
	}
	db->protected_devs[i].num_gpids = num_gpids;
	for (j = 0; j < num_gpids; j++) {
		/* if this port is already under attack */
		if (db->actions_state[DPL_LGM_ACTION_WHITE_LIST])
			pp_port_protect(db->protected_devs[i].gpid[j]);
	}
	pr_debug("dev %s ifindex %d is added to DPL\n", dev->name, ifindex);

unlock:
	spin_unlock_bh(&db->lock);
free:
	kfree(subif);
	return ret;
}
EXPORT_SYMBOL(dpl_dev_protect);

int dpl_dev_unprotect(struct net_device *dev)
{
	int ret = 0, ifindex, i, j;

	if (!db || !dev || dev->ifindex == INVALID_IFIND)
		return -EINVAL;

	ifindex = dev->ifindex;

	spin_lock_bh(&db->lock);

	/* find the i index of the dev in the db */
	dpl_for_each_dev(i)
		if (db->protected_devs[i].ifindex == ifindex)
			break;

	if (i == ARRAY_SIZE(db->protected_devs)) {
		pr_err("dev %s does not exist\n", dev->name);
		ret = -EINVAL;
		goto unlock;
	}

	for (j = 0; j < db->protected_devs[i].num_gpids; j++) {
		/* if this port is under attack */
		if (db->actions_state[DPL_LGM_ACTION_WHITE_LIST])
			pp_port_unprotect(db->protected_devs[i].gpid[j]);
		db->protected_devs[i].gpid[j] = PP_PORT_INVALID;
	}
	db->protected_devs[i].num_gpids = 0;
	db->protected_devs[i].ifindex = INVALID_IFIND;

	pr_debug("dev %s ifindex %d is removed from DPL\n", dev->name, ifindex);
	/* here we are not removing the dev from pktprs because maybe another
	   user using this dev */
unlock:
	spin_unlock_bh(&db->lock);
	return ret;
}
EXPORT_SYMBOL(dpl_dev_unprotect);

static int set_default_prio_arp(void)
{
	struct dpl_whitelist_field fields = { 0 };

	fields.type = DPL_FLD_ETH_TYPE;
	fields.eth_type = htons(ETH_P_ARP);

	return dpl_whitelist_rule_add(DPL_ARP_PRIO, &fields, 1);
}

static int set_default_prio_neighbor_advertisement(void)
{
	struct dpl_whitelist_field fields = { 0 };

	fields.type = DPL_FLD_ICMP_TYPE_CODE;
	/* ICMPv6: type: 134, code: 0 */
	fields.icmp_type_code = htons(0x8600);

	return dpl_whitelist_rule_add(DPL_NDP_PRIO, &fields, 1);
}

static int set_default_prio_neighbor_solicitation(void)
{
	struct dpl_whitelist_field fields = { 0 };

	fields.type = DPL_FLD_ICMP_TYPE_CODE;
	/* ICMPv6: type: 135, code: 0 */
	fields.icmp_type_code = htons(0x8700);

	return dpl_whitelist_rule_add(DPL_NDP_PRIO, &fields, 1);
}

static int set_default_prio_router_advertisement(void)
{
	struct dpl_whitelist_field fields = { 0 };

	fields.type = DPL_FLD_ICMP_TYPE_CODE;
	/* ICMPv6: type: 136, code: 0 */
	fields.icmp_type_code = htons(0x8800);

	return dpl_whitelist_rule_add(DPL_NDP_PRIO, &fields, 1);
}

static int set_default_prio_snmp(void)
{
	struct dpl_whitelist_field fields[2] = { 0 };

	fields[0].type = DPL_FLD_IP_PROTO;
	fields[0].ip_proto = IPPROTO_UDP;

	fields[1].type = DPL_FLD_L4_DST_PORT;
	fields[1].dst_port = htons(SNMP_PORT);

	return dpl_whitelist_rule_add(DPL_SNMP_PRIO, fields, ARRAY_SIZE(fields));
}

static int set_default_prio_dhcp(void)
{
	struct dpl_whitelist_field fields[3] = { 0 };

	fields[0].type = DPL_FLD_IP_PROTO;
	fields[0].ip_proto = IPPROTO_UDP;

	/* src and dst ports for DHCP reply */
	fields[1].type = DPL_FLD_L4_SRC_PORT;
	fields[1].src_port = htons(DHCP_SERVER_PORT);

	fields[2].type = DPL_FLD_L4_DST_PORT;
	fields[2].dst_port = htons(DHCP_CLIENT_PORT);

	return dpl_whitelist_rule_add(DPL_DHCP_PRIO, fields, ARRAY_SIZE(fields));
}

static int set_default_prio_dhcp_v6(void)
{
	struct dpl_whitelist_field fields[3] = { 0 };

	fields[0].type = DPL_FLD_IP_PROTO;
	fields[0].ip_proto = IPPROTO_UDP;

	/* src and dst ports for DHCPv6 reply */
	fields[1].type = DPL_FLD_L4_SRC_PORT;
	fields[1].src_port = htons(DHCPV6_SERVER_PORT);

	fields[2].type = DPL_FLD_L4_DST_PORT;
	fields[2].dst_port = htons(DHCPV6_CLIENT_PORT);

	return dpl_whitelist_rule_add(DPL_DHCP_PRIO, fields, ARRAY_SIZE(fields));
}

static int set_default_prio_dns(void)
{
	struct dpl_whitelist_field fields[2] = { 0 };

	fields[0].type = DPL_FLD_IP_PROTO;
	fields[0].ip_proto = IPPROTO_UDP;

	/* src port for DNS reply */
	fields[1].type = DPL_FLD_L4_SRC_PORT;
	fields[1].src_port = htons(DNS_PORT);

	return dpl_whitelist_rule_add(DPL_DNS_PRIO, fields, ARRAY_SIZE(fields));
}

static int set_default_prio_dns_over_tcp(void)
{
	struct dpl_whitelist_field fields[2] = { 0 };

	fields[0].type = DPL_FLD_IP_PROTO;
	fields[0].ip_proto = IPPROTO_TCP;

	/* src port for DNS reply */
	fields[1].type = DPL_FLD_L4_SRC_PORT;
	fields[1].src_port = htons(DNS_PORT);

	return dpl_whitelist_rule_add(DPL_DNS_PRIO, fields, ARRAY_SIZE(fields));
}

static int set_default_prio_echo_req(void)
{
	struct dpl_whitelist_field fields = { 0 };

	fields.type = DPL_FLD_ICMP_TYPE_CODE;
	/* ICMP: echo request: type: 8, code: 0 */
	fields.icmp_type_code = htons(0x0800);

	return dpl_whitelist_rule_add(DPL_PING_PRIO, &fields, 1);
}

static int set_default_prio_echo_rply(void)
{
	struct dpl_whitelist_field fields = { 0 };

	fields.type = DPL_FLD_ICMP_TYPE_CODE;
	/* ICMP: echo reply: type: 0, code: 0 */
	fields.icmp_type_code = htons(0x0000);

	return dpl_whitelist_rule_add(DPL_PING_PRIO, &fields, 1);
}

static int set_default_prio_echo_req_v6(void)
{
	struct dpl_whitelist_field fields = { 0 };

	fields.type = DPL_FLD_ICMP_TYPE_CODE;
	/* ICMPv6: echo request: type: 128, code: 0 */
	fields.icmp_type_code = htons(0x8000);

	return dpl_whitelist_rule_add(DPL_PING_PRIO, &fields, 1);
}

static int set_default_prio_echo_rply_v6(void)
{
	struct dpl_whitelist_field fields = { 0 };

	fields.type = DPL_FLD_ICMP_TYPE_CODE;
	/* ICMPv6: echo reply: type: 129, code: 0 */
	fields.icmp_type_code = htons(0x8100);

	return dpl_whitelist_rule_add(DPL_PING_PRIO, &fields, 1);
}

static void set_default_prio(void)
{
	if (set_default_prio_arp())
		pr_err("error to add default prio to ARP\n");
	if (set_default_prio_neighbor_advertisement())
		pr_err("error to add default prio to Neighbor Advertisement\n");
	if (set_default_prio_neighbor_solicitation())
		pr_err("error to add default prio to Neighbor Solicitation\n");
	if (set_default_prio_router_advertisement())
		pr_err("error to add default prio to Router Advertisement\n");
	if (set_default_prio_snmp())
		pr_err("error to add default prio to SNMP\n");
	if (set_default_prio_dhcp())
		pr_err("error to add default prio to DHCP\n");
	if (set_default_prio_dhcp_v6())
		pr_err("error to add default prio to DHCPv6\n");
	if (set_default_prio_dns())
		pr_err("error to add default prio to DNS\n");
	if (set_default_prio_dns_over_tcp())
		pr_err("error to add default prio to DNS over TCP\n");
	if (set_default_prio_echo_req())
		pr_err("error to add default prio to echo request ping\n");
	if (set_default_prio_echo_rply())
		pr_err("error to add default prio to echo reply ping\n");
	if (set_default_prio_echo_req_v6())
		pr_err("error to add default prio to IPv6 echo request ping\n");
	if (set_default_prio_echo_rply_v6())
		pr_err("error to add default prio to IPv6 echo reply ping\n");
}

static void logic_param_set(struct dpl_logic_param *param)
{
	param->cb.is_dev_protected = is_dev_protected;
	param->cb.enable_ind = pp_dpl_hash_bit_enable;
	param->cb.disable_ind = pp_dpl_hash_bit_disable;
	param->cb.reset = pp_dpl_hash_bit_reset;

	param->hash_bit_num_entries = PP_DPL_HASH_BIT_MAX_ENTRIES;
}

static int __init dpl_lgm_init(void)
{
	unsigned short i, j;
	int ret;
	struct dpl_logic_param logic_param;

	db = kzalloc(sizeof(*db), GFP_KERNEL);
	if (!db) {
		pr_err("alloc db failed\n");
		return -ENOMEM;
	}

	db->qdev = pp_qos_dev_open(PP_QOS_INSTANCE_ID);
	if (!db->qdev) {
		kfree(db);
		return -EINVAL;
	}

	spin_lock_init(&db->lock);

	db->mon_params.attr_get[DPL_PPS].get_cb = get_num_host_rx_pkt;
	db->mon_params.attr_get[DPL_DPS].get_cb = get_num_host_rx_drop_pkt;
	db->mon_params.attr_get[DPL_SPS].get_cb = get_num_created_sessions;
	db->mon_params.attr_get[DPL_SUT].get_cb = get_session_utilization;
	db->mon_params.attr_get[DPL_CPU].get_cb = sysidle_check_cpu;

	db->prod_type = DPL_PRODUCT_TYPE_GW;

	dpl_for_each_dev(i)
		for (j = 0; j < ARRAY_SIZE(db->protected_devs[i].gpid); j++)
			db->protected_devs[i].gpid[j] = PP_PORT_INVALID;

	ret = pp_max_sessions_get(&db->max_sessions);
	if (unlikely(ret)) {
		kfree(db);
		pr_err("Failed getting max sessions, ret %u\n", ret);
		return ret;
	}

	db->num_cpus = num_active_cpus();
	if (db->num_cpus > CQM_MAX_CPU) {
		pr_err("num cpus %u > %u\n", db->num_cpus, CQM_MAX_CPU);
		db->num_cpus = CQM_MAX_CPU;
	}

	/* Build & configure host QoS */
	ret = dpl_init_host_qos();
	if (ret) {
		kfree(db);
		pr_err("Host QoS initialization failed (%d)\n", ret);
		return ret;
	}

	/* Build & configure host QoS */
	ret = dpl_init_pp_cpu_info();
	if (ret) {
		kfree(db);
		pr_err("Failed to init pp with cpu info (%d)\n", ret);
		return ret;
	}

	set_default_prio();

	logic_param_set(&logic_param);

	dpl_lgm_dbg_init(&parent_dir);
	dpl_logic_init(&logic_param, parent_dir);
	dpl_mon_init(parent_dir);
	dpl_configure_actions();

	return 0;
}

static void __exit dpl_lgm_exit(void)
{
	dpl_mon_unregister();
	dpl_logic_exit();
	dpl_mon_exit();

	dpl_lgm_dbg_clean(parent_dir);

	kfree(db);
	db = NULL;
}

MODULE_AUTHOR("Intel Corporation");
MODULE_DESCRIPTION(MOD_NAME);
MODULE_LICENSE("GPL v2");

module_init(dpl_lgm_init);
module_exit(dpl_lgm_exit);
