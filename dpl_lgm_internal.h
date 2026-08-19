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
 * Description: dpl module internal h file
 */
#ifndef __PP_DPL_LGM_INTERNAL_H__
#define __PP_DPL_LGM_INTERNAL_H__

#include <linux/types.h>
#include "dpl_monitor.h"

#define DPL_VERSION "0.0.18"

/**
 * @define DPL_NON_CONTINUOUS_THR ON/OFF
 *         for non continuous attack, if the attr is high for over 1/3 from
 *         DPL_HISTORY_SIZE activating the action, and to deactivate it the attr
 *         should be low for over 2/3 from DPL_HISTORY_SIZE
 */
#define DPL_NON_CONTINUOUS_THR_ON  (DIV_ROUND_UP_ULL(DPL_HISTORY_SIZE, 3))
#define DPL_NON_CONTINUOUS_THR_OFF (DIV_ROUND_UP_ULL(DPL_HISTORY_SIZE * 2, 3))

/**
 * @define DPL white list action defines
 */
/* The PPS val not calc PER CPU, to detect attack that flows to specific CPU */
#define DPL_ACTION_WHITE_LIST_THR_ON_PPS_VAL          11000
#define DPL_ACTION_WHITE_LIST_THR_ON_PPS_SEQ_ITER     5
#define DPL_ACTION_WHITE_LIST_THR_ON_PPS_ITER         DPL_NON_CONTINUOUS_THR_ON
#define DPL_ACTION_WHITE_LIST_THR_ON_PPS_DEP          DPL_CPU
#define DPL_ACTION_WHITE_LIST_THR_OFF_PPS_VAL         7500
#define DPL_ACTION_WHITE_LIST_THR_OFF_PPS_SEQ_ITER    5
#define DPL_ACTION_WHITE_LIST_THR_OFF_PPS_ITER        DPL_NON_CONTINUOUS_THR_OFF
#define DPL_ACTION_WHITE_LIST_THR_OFF_PPS_DEP         DPL_CPU

#define DPL_ACTION_WHITE_LIST_THR_ON_CPU_VAL          90
#define DPL_ACTION_WHITE_LIST_THR_ON_CPU_SEQ_ITER     5
#define DPL_ACTION_WHITE_LIST_THR_ON_CPU_ITER         DPL_NON_CONTINUOUS_THR_ON
#define DPL_ACTION_WHITE_LIST_THR_ON_CPU_DEP          DPL_PPS
#define DPL_ACTION_WHITE_LIST_THR_OFF_CPU_VAL         50
#define DPL_ACTION_WHITE_LIST_THR_OFF_CPU_SEQ_ITER    5
#define DPL_ACTION_WHITE_LIST_THR_OFF_CPU_ITER        DPL_NON_CONTINUOUS_THR_OFF
#define DPL_ACTION_WHITE_LIST_THR_OFF_CPU_DEP         DPL_PPS

/**
 * @define DPL restrict default queues bw limit defines
 */
/* the thresholds are the same as white list action thresholds */
#define DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_ON_PPS_VAL          DPL_ACTION_WHITE_LIST_THR_ON_PPS_VAL
#define DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_ON_PPS_SEQ_ITER     DPL_ACTION_WHITE_LIST_THR_ON_PPS_SEQ_ITER
#define DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_ON_PPS_ITER         DPL_ACTION_WHITE_LIST_THR_ON_PPS_ITER
#define DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_ON_PPS_DEP          DPL_ACTION_WHITE_LIST_THR_ON_PPS_DEP
#define DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_OFF_PPS_VAL         DPL_ACTION_WHITE_LIST_THR_OFF_PPS_VAL
#define DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_OFF_PPS_SEQ_ITER    DPL_ACTION_WHITE_LIST_THR_OFF_PPS_SEQ_ITER
#define DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_OFF_PPS_ITER        DPL_ACTION_WHITE_LIST_THR_OFF_PPS_ITER
#define DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_OFF_PPS_DEP         DPL_ACTION_WHITE_LIST_THR_OFF_PPS_DEP

#define DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_ON_CPU_VAL          DPL_ACTION_WHITE_LIST_THR_ON_CPU_VAL
#define DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_ON_CPU_SEQ_ITER     DPL_ACTION_WHITE_LIST_THR_ON_CPU_SEQ_ITER
#define DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_ON_CPU_ITER         DPL_ACTION_WHITE_LIST_THR_ON_CPU_ITER
#define DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_ON_CPU_DEP          DPL_ACTION_WHITE_LIST_THR_ON_CPU_DEP
#define DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_OFF_CPU_VAL         DPL_ACTION_WHITE_LIST_THR_OFF_CPU_VAL
#define DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_OFF_CPU_SEQ_ITER    DPL_ACTION_WHITE_LIST_THR_OFF_CPU_SEQ_ITER
#define DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_OFF_CPU_ITER        DPL_ACTION_WHITE_LIST_THR_OFF_CPU_ITER
#define DPL_ACTION_RESTRICT_BW_DPLT_QS_THR_OFF_CPU_DEP         DPL_ACTION_WHITE_LIST_THR_OFF_CPU_DEP

/**
 * @define DPL l2 classification action defines
 */
#define DPL_ACTION_L2_CLASS_THR_ON_PPS_VAL       100
#define DPL_ACTION_L2_CLASS_THR_ON_PPS_SEQ_ITER  5
#define DPL_ACTION_L2_CLASS_THR_ON_PPS_ITER      DPL_NON_CONTINUOUS_THR_ON
#define DPL_ACTION_L2_CLASS_THR_ON_PPS_DEP       DPL_ATTR_MAX
#define DPL_ACTION_L2_CLASS_THR_OFF_PPS_VAL      30
#define DPL_ACTION_L2_CLASS_THR_OFF_PPS_SEQ_ITER 5
#define DPL_ACTION_L2_CLASS_THR_OFF_PPS_ITER     DPL_NON_CONTINUOUS_THR_OFF
#define DPL_ACTION_L2_CLASS_THR_OFF_PPS_DEP      DPL_ATTR_MAX

/**
 * @define DPL min packets for session creation action defines
 */
#define DPL_ACTION_SESS_CREATE_THR_ON_PPS_VAL       100
#define DPL_ACTION_SESS_CREATE_THR_ON_PPS_SEQ_ITER  5
#define DPL_ACTION_SESS_CREATE_THR_ON_PPS_ITER      DPL_NON_CONTINUOUS_THR_ON
#define DPL_ACTION_SESS_CREATE_THR_ON_PPS_DEP       DPL_ATTR_MAX
#define DPL_ACTION_SESS_CREATE_THR_OFF_PPS_VAL      30
#define DPL_ACTION_SESS_CREATE_THR_OFF_PPS_SEQ_ITER 5
#define DPL_ACTION_SESS_CREATE_THR_OFF_PPS_ITER     DPL_NON_CONTINUOUS_THR_OFF
#define DPL_ACTION_SESS_CREATE_THR_OFF_PPS_DEP      DPL_ATTR_MAX

/**
 * @enum dpl_lgm_action
 * @brief dpl lgm action
 */
enum dpl_lgm_action {
	/*! White list */
	DPL_LGM_ACTION_WHITE_LIST,

	/*! Switch acceleration mode */
	DPL_LGM_ACTION_L2_CLASS,

	/*! Increase minimum packets for session creation */
	DPL_LGM_ACTION_INC_MIN_PKT_FOR_SESS_CREATE,

	/*! Restrict BW limit to DP/default queues */
	DPL_LGM_ACTION_RESTRICT_DFLT_QUEUES_BW_LIMIT,

	DPL_LGM_ACTION_MAX,
};

/**
 * @brief DPL set action
 * @param action       action to set
 * @param type         threshold type
 * @param attr         attribute to set
 * @param val          threshold value
 * @param num_seq_iter attribute to set
 * @param num_iter     attribute to set
 * @param dependency   dependency to other attribute or DPL_ATTR_MAX if none
 * @param do_register  register action to monitor.
 *                     User can register after each set (usually for debug) or
 *                     register manually once after setting all actions
 * @return 0 on success, error code otherwise
 */
int dpl_set_action(enum dpl_lgm_action action, enum dpl_thr_type type,
		   enum dpl_attr_type attr, unsigned int val,
		   unsigned int num_seq_iter, unsigned int num_iter,
		   enum dpl_attr_type dependency, bool do_register);

enum test_operation {
	test_operation_dump_dp_info,
	test_operation_dump_protected,
	test_operation_white_list_en,
	test_operation_white_list_dis,
	test_operation_dpl_enable,
	test_operation_dump_mon_params,
	test_operation_dump_actions_state,
	test_operation_max
};

/**
 * @struct dpl_cpu_queue_info
 * @brief CPU queues information
 * @queue      queue (logical id)
 * @port       port
 * @gpid       gpid
 * @dflt_bw    bw limit before DPL action (relevant for queues that change their
 *             BW after attack)
 * @is_bw_changed true if BW changed by DPL action
 * @dpl_owner  true if created by dpl
 */
struct dpl_cpu_queue_info {
	unsigned short queue;
	unsigned short port;
	unsigned short gpid;
	unsigned int dflt_bw;
	bool is_bw_changed;
	bool dpl_owner;
};

#define DPL_MAX_CPU_QUEUES 16

/**
 * @struct dpl_cpu_info
 * @brief CPU information.
 * @queue    Queue information
 * @num_q    Number of queues under cpu
 */
struct dpl_cpu_info {
	struct dpl_cpu_queue_info queue[DPL_MAX_CPU_QUEUES];
	unsigned short num_q;
};

/**
 * @brief debugfs operations to execute
 * @param op          operation
 * @param port        port
 * @param queue       queue
 */
void dpl_test(enum test_operation op, unsigned int port, unsigned int queue);

/**
 * @brief Retrieves the cpu information
 * @param cpu_info          cpu information to fill
 * @param num_cpus          num cpus
 */
void dpl_retrieve_cpu_info(struct dpl_cpu_info *cpu_info,
			   unsigned int *num_cpus);

/**
 * @brief dump all protected devs and their gpids
 */
void dpl_protected_devs_dump(void);

/**
 * @brief Add new white list rule to prioritize important packets
 *        over regular packets on the host path
 * @param prio priority, used for choosing destination queue for packets
 *             who matched the rule.
 *             valid values are from 0 (highest) to 7 (lowest)
 * @param fields fields array to compare to the the packets
 *               e.g. ip address, udp port, mac address etc...
 * @param cnt number of fields
 *
 * White rules are used to protect the host when a DoS attack occurs
 * by prioritizing packets that was matched to one of the rules
 * to queues with high priority
 * @return int 0 on success, error code otherwise
 */
int dpl_whitelist_rule_add(unsigned int prio, struct dpl_whitelist_field *fields,
			   unsigned int cnt);

/**
 * @brief Delete an existing white rule, imput parameters here
 *        should be the exact ones that were used in dpl_whitelist_rule_add
 * @param prio priority
 * @param fields fields array
 * @param cnt number of fields
 * @return int 0 on success, error code otherwise
 */
int dpl_whitelist_rule_del(unsigned int prio, struct dpl_whitelist_field *fields,
			   unsigned int cnt);

#ifdef CONFIG_DEBUG_FS
int dpl_lgm_dbg_init(struct dentry **parent_dir);
int dpl_lgm_dbg_clean(struct dentry *parent_dir);
#else /* !CONFIG_DEBUG_FS */
static inline int dpl_lgm_dbg_init(struct dentry **parent_dir)
{
	return 0;
}
static inline int dpl_lgm_dbg_clean(struct dentry *parent_dir)
{
	return 0;
}
#endif /* CONFIG_DEBUG_FS */
#endif /* __PP_DPL_LGM_INTERNAL_H__ */
