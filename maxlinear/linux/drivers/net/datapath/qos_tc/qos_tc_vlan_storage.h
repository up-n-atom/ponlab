/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright (c) 2021 - 2023 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/

#ifndef _QOS_TC_VLAN_STORAGE_
#define _QOS_TC_VLAN_STORAGE_

#include "qos_tc_vlan_prepare.h"

/* Received frame EVLAN tagging operation layout (encoded in the cookie)
 *    3                   2                   1
 *  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  FOP  |         FIV             | FOT |  FOP* | FOT*|    r    |\merge
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ key
 * |  FIP  |         FIV             | FIT |  FIP* | FIT*|r| ETYPE |/.hi
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |x|      PAD          |  TOP  |           TOV             | TOT |\merge
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ key
 * |  TIP* | TIT*| PAD |y|  TIP  |           TIV             | TIT |/.lo
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * * - Downstram
 * r - Reserved
 * x - Treatment, tags to remove
 * y - Enable downstream, this will cause the driver to use * fields
 *     when merging.
 *
 * The * fields and y are ignored when sorting
 * (sort_key is merge_key.hi & FILTER_SORT_MASK)
 *
 * sort_key, merge_key.hi and merge_key.lo are 64 bit values
 */

#define FILTER_SORT_MASK 0xFFFFF000FFFFF00F

/* Macros valid for sort_key or merge_key.hi */
#define FILTER_OUTER_PRIO(x) (((x) >> 60) & 0xF)
#define FILTER_OUTER_VID(x) (((x) >> 47) & 0x1FFF)
#define FILTER_OUTER_TPID(x) (((x) >> 44) & 0x7)

#define FILTER_DS_OUTER_PRIO(x) (((x) >> 40) & 0xF)
#define FILTER_DS_OUTER_TPID(x) (((x) >> 37) & 0x7)

#define FILTER_INNER_PRIO(x) (((x) >> 28) & 0xF)
#define FILTER_INNER_VID(x) (((x) >> 15) & 0x1FFF)
#define FILTER_INNER_TPID(x) (((x) >> 12) & 0x7)

#define FILTER_DS_INNER_PRIO(x) (((x) >> 8) & 0xF)
#define FILTER_DS_INNER_TPID(x) (((x) >> 5) & 0x7)

#define FILTER_ETHERTYPE(x) (((x) >> 0) & 0xF)

/* Macros valid for merge_key.lo */
#define TREATMENT_DS_INNER_PRIO(x) (((x) >> 28) & 0xF)
#define TREATMENT_DS_INNER_TPID(x) (((x) >> 25) & 0x7)
#define TREATMENT_DS_EN(x) (((x) >> 20) & 0x1)

#define FILTER_PRIO_IGNORE 15
#define FILTER_PRIO_DEFAULT 14
#define FILTER_ETHERTYPE_ANY 0
#define FILTER_VID_ANY 4096
#define FILTER_TPID_ANY 0

struct qos_tc_vlan_storage_node {
	struct net_device *dev;
	bool ingress;
	enum tc_flower_vlan_tag tag;
	struct list_head rules;
	int rule_cnt;
	bool mcc;
	struct list_head list;
};

struct qos_tc_vlan_storage_node
*qos_tc_vlan_storage_get(struct net_device *dev,
			 bool ingress,
			 struct net_device *bp_dev,
			 enum tc_flower_vlan_tag tag,
			 bool mcc,
			 struct list_head *head);

int qos_tc_vlan_storage_crt(struct net_device *dev,
			    bool ingress,
			    enum tc_flower_vlan_tag tag,
			    bool mcc,
			    struct qos_tc_vlan_storage_node **node);

void qos_tc_vlan_storage_del(struct net_device *dev,
			     struct qos_tc_vlan_storage_node *node);

int qos_tc_cookie_cmp0(void *priv, const struct list_head *lh_a,
		       const struct list_head *lh_b);

int qos_tc_cookie_cmp1(void *priv, const struct list_head *lh_a,
		       const struct list_head *lh_b);

int qos_tc_cookie_cmp2(void *priv, const struct list_head *lh_a,
		       const struct list_head *lh_b);

#endif
