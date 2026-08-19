/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright (c) 2021 - 2022 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/

#include <net/switch_api/gsw_types.h>
#include <net/switch_api/lantiq_gsw.h>
#include <net/switch_api/lantiq_gsw_flow.h>

enum pce_type {
	/* PCE rule will be placed in common block */
	PCE_COMMON = 0,
	/* PCE rule will be placed in CTP block */
	PCE_UNCOMMON = 1,
	PCE_MIRR_UNTAG_DROP = 2,
	PCE_MIRR_UNTAG_FWD = 3,
	PCE_MIRR_VLAN_WILD = 4,
	PCE_MIRR_VLAN_DROP = 5,
	PCE_MIRR_VLAN_FWD = 6,
};

/**
 *  Create PCE rule in hardware.
 *
 *  \param[in]	cookie		TC command identifier
 *  \param[in]	pref		TC command preference
 *  \param[in]	pce_rule	PCE rule derived from TC command
 */
int qos_tc_pce_rule_create(struct net_device *dev,
			   unsigned long cookie, int pref,
			   enum pce_type type,
			   const GSW_PCE_rule_t *pce_rule);

/**
 *  Delete PCE rule from hardware.
 *
 *  \param[in]	cookie		TC command identifier
 *  \param[in]	pref		TC command preference
 */
int qos_tc_pce_rule_delete(unsigned long cookie, int pref);

void qos_tc_pce_list_debugfs(struct seq_file *file, void *ctx);
