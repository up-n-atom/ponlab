/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright (c) 2025 MaxLinear, Inc.
 *
 *****************************************************************************/
#ifndef _QOS_TC_TBF_H_
#define _QOS_TC_TBF_H_

#include <linux/types.h>

struct qos_tc_port;
struct qos_tc_qdata_params;

struct qos_tc_qdata_params *qos_tc_get_qp(struct qos_tc_port *port,
					  enum qos_tc_qdata_type type,
					  u32 parent);
int qos_tc_q_tbf_sch(struct qos_tc_port *port, enum qos_tc_qdisc_type type,
		     u32 parent, u32 handle);
int qos_tc_tbf_sch(struct qos_tc_port *port, enum qos_tc_qdisc_type type,
		   u32 parent, u32 handle);

bool qos_tc_tbf_port_sch_on(struct qos_tc_port *port, u32 parent);

#endif
