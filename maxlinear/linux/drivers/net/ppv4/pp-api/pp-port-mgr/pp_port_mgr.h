/*
 * Copyright (C) 2020-2023 MaxLinear, Inc.
 * Copyright (C) 2018-2020 Intel Corporation
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
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: PP Port Manager definitions
 */

#ifndef __PP_PORT_MGR_H__
#define __PP_PORT_MGR_H__

#include <linux/types.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/pp_api.h>
#include "uc_host_common.h"

/**
 * @brief initial configuration parameters
 * @dbgfs debugfs parent folder
 */
struct pp_pmgr_init_param {
	struct dentry  *dbgfs;
	struct kobject *sysfs;
};

#ifdef CONFIG_SOC_LGM

/**
 * @brief Number of supported GPIDs groups
 */
#define PMGR_GPID_GRP_INVALID UC_GPID_GRP_CNT

/**
 * @struct module statistics (for debug)
 */
struct pmgr_stats {
	u32 null_err;
	u32 hal_err;
	u32 port_count;
	u32 port_invalid_err;
	u32 port_invalid_headroom;
	u32 port_act_err;
	u32 port_inact_err;
	u32 port_update_no_changes_err;
	u32 port_cls_param_err;
	u32 port_policy_param_err;
	u32 port_tx_cfg_err;
	u32 port_rx_cfg_err;
	u32 hif_dps_count;
	u32 hif_dp_param_err;
	u32 hif_cls_param_err;
	u32 hif_dp_port_err;
};

/**
 * @brief GPIDs group data structure
 */
struct pmgr_db_grp {
	char name[16];
	ulong gpid_bmap[BITS_TO_LONGS(PP_MAX_PORT)];
};

/**
 * @brief Get id of the default host interface
 * @return s32 default hif id if exist if configured, error code otherwise
 */
s32 pmgr_dflt_hif_id_get(void);

/**
 * @brief Get GPID group info
 * @param grp_id group id
 * @param pmgr_grp group info buffer to copy into
 * @param uc_grp group uc info buffer to copy into
 * @param st group packets and bytes statistics
 * @return s32 0 on succuss, error code otherwise
 */
s32 pmgr_gpid_group_info_get(u32 grp_id, struct pmgr_db_grp *pmgr_grp,
			     struct uc_gpid_group *uc_grp, struct pp_stats *st);

/**
 * @brief Move gpid from one gpid group to another
 * @note if src_grp is invalid, it is the same as pp_gpid_group_add_port
 * @note if dst_grp is invalid, it is the same as pp_gpid_group_del_port
 * @param port_id the port id (GPID)
 * @param dst_grp dest group to move to
 * @param src_grp source group to move from
 * @return s32 0 on succuss, error code otherwise
 */
s32 pmgr_gpid_group_move_port(u16 port_id, u32 dst_grp, u32 src_grp);

/**
 * @brief Set host cpu configuration for ports that will be under
 *        protection
 * @param cpu cpu info
 * @param num_cpus number of cpus
 * @param group_id get the protected_grp id
 * @return s32 0 on succuss, error code otherwise
 */
s32 pmgr_protected_grp_host_cpu_info_set(struct pp_cpu_info *cpu,
					 unsigned int num_cpus, s32 *group_id);

/**
 * @brief Enable/Disable bithash feature for a group
 * @param grp_id group id
 * @param en enable/disable
 * @return s32 0 on succuss, error code otherwise
 */
s32 pmgr_gpid_group_hash_bit_set(u32 grp_id, bool en);

/**
 * @brief adding new pp port
 * @param pid port number
 * @param cfg port configuration
 * @return s32 return 0 for success
 */
s32 pmgr_port_add(u16 pid, struct pp_port_cfg *cfg);

/**
 * @brief deleting pp port
 * @param pid port number
 * @return s32 return 0 for success
 */
s32 pmgr_port_del(u16 pid);

/**
 * @brief adding new pp host interface
 * @param hif host interface configuration
 * @return s32 return 0 for success
 */
s32 pmgr_hostif_add(struct pp_hostif_cfg *hif);

/**
 * @brief update pp host interface
 * @param hif current host interface configuration
 * @param new_dp new datapath to set
 * @return s32 return 0 for success
 */
s32 pmgr_hostif_update(struct pp_hostif_cfg *hif,
		       struct pp_hif_datapath *new_dp);

/**
 * @brief deleting pp host interface
 * @param hif host interface configuration
 * @return s32 return 0 for success
 */
s32 pmgr_hostif_del(struct pp_hostif_cfg *hif);

/**
 * @brief set default pp host interface
 * @param dp host interface datapath
 * @return s32 return 0 for success
 */
s32 pmgr_hostif_dflt_set(struct pp_hif_datapath *dp);

/**
 * @brief Check if port is active
 * @param pid pp port index (GPID)
 * @return bool true if port active, false otherwise
 */
bool pmgr_port_is_active(u16 pid);

/**
 * @brief get the module statistics from database
 * @param stats port manager module statistics
 * @return s32 0 for success, non-zero otherwise
 */
s32 pmgr_stats_get(struct pmgr_stats *stats);

/**
 * @brief Reset port manager stats
 * @return s32 0 for success, non-zero otherwise
 */
s32 pmgr_stats_reset(void);

/**
 * @brief initialized PP port manager driver
 * @param init_param initial parameters
 * @return s32 0 on success
 */
s32 pmgr_init(struct pp_pmgr_init_param *init_param);

/**
 * @brief exit PP port manager driver
 * @param void
 */
void pmgr_exit(void);
#else
static inline s32 pmgr_init(struct pp_pmgr_init_param *init_param)
{
	return 0;
}

static inline void pmgr_exit(void)
{
}
#endif /* CONFIG_SOC_LGM */
#endif /* __PP_PORT_MGR_H__ */
