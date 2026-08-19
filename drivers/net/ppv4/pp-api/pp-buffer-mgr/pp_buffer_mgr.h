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
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: Packet Processor Buffer Manager Driver
 */

#ifndef _PP_BUFFER_MGR_H_
#define _PP_BUFFER_MGR_H_

#include <linux/types.h>
#include <linux/pp_api.h>
#include <linux/pp_buffer_mgr_api.h>
#include <linux/debugfs.h> /* struct dentry */

/**
 * @brief PP_BM_MAX_POLICIES
 *       Max supported policies. Real max defined in the DTS
 */
#define PP_BM_MAX_POLICIES                   (128)
#define PP_BM_POLICY_INVALID                 PP_BM_MAX_POLICIES
#define PP_BM_IS_POLICY_VALID(p)             \
	(0 <= (p) && (p) < PP_BM_POLICY_INVALID)

#define PP_BM_MAX_BURST_IN_POP               (32)

/**
 * @struct pp_bmgr_cfg
 * @brief Buffer manager initial configuration
 */
struct pp_bmgr_init_param {
	bool valid;          /*! params valid */
	u32  max_pools;      /*! Max number of pools */
	u32  max_groups;     /*! Max number of groups */
	u32  max_policies;   /*! Max number of policies */
	u32  max_pools_in_policy; /*! Max number of pools in policy */
	bool pool_pop_hw_en;      /*! true if feature is enabled in hw */
	struct dentry *dbgfs;
};

/**
 * @brief This structure is used for buffer manager database
 * @pools Pools in policy
 * @policies_bmap bitmap specifing which policies are member of the group
 */
struct bmgr_group_db_entry {
	ulong  policies_bmap[BITS_TO_LONGS(PP_BM_MAX_POLICIES)];
	ulong  pools_bmap[BITS_TO_LONGS(PP_BM_MAX_POOLS)];
	u32 reserved;
	u32 available;
};

/**
 * @brief Buffer Manager stats
 */
struct bmgr_stats {
	u32 active_pools;    /*! number of active pools */
	u32 active_policies; /*! number of active policies */
};

/**
 * @brief Module init function
 * @param cfg Buffer manager configuration
 * @return s32 0 on success, non-zero value otherwise
 */
s32 pp_bmgr_init(const struct pp_bmgr_init_param *cfg);

/**
 * @brief Module exit function, clean all resources
 * @return void
 */
void pp_bmgr_exit(void);

/**
 * @brief Get buffer manager statistics
 * @param stats
 * @return s32 0 on success, error code otherwise
 */
s32 pp_bmgr_stats_get(struct bmgr_stats *stats);

/**
 * @brief Set maximum pools per policy
 * @note this is for debug/testing purposes, do not use otherwise
 * @param pools_per_policy
 * @return s32
 */
void bmgr_pools_per_policy_set(u32 pools_per_policy);

/**
 * @brief Get active policies bitmap
 * @param bmap policies bitmap buffer
 * @param size num of bits in the bitmap
 * @return s32 0 on success, error code otherwise
 */
s32 bmgr_get_active_policies(ulong *bmap, size_t size);

/**
 * @brief Get active pools bitmap
 * @param bmap pools bitmap buffer
 * @param size num of bits in the bitmap
 * @return s32 0 on success, error code otherwise
 */
s32 bmgr_get_active_pools(ulong *bmap, size_t size);

/**
 * @brief Set bmgr configuration
 * @param cfg Buffer manager configuration
 * @return 0 on success, other error code on failure
 */
s32 bmgr_config_set(const struct pp_bmgr_init_param * const cfg);

/**
 * @brief Get bmgr configuration
 * @param cfg Buffer manager configuration
 * @return 0 on success, other error code on failure
 */
s32 pp_bmgr_config_get(struct pp_bmgr_init_param * const cfg);

/**
 * @brief Get the SSB policy for LRO uc
 * @param policy_id
 * @return 0 on success, other error code on failure
 * @note This function is used by the LRO UC to get the SSB policy
 */
s32 pp_bmgr_ssb_policy_get(u32 *policy_id);

/**
 * @brief Get group database info
 * @param grp_id group id
 * @param info
 * @return 0 on success, other error code on failure
 */
s32 pp_bmgr_group_db_info_get(u8 grp_id, struct bmgr_group_db_entry *info);

/**
 * @brief Get buffer pool buffers memory info
 * @param pool_id pool id
 * @param virt pointer for virtual address
 * @param phys pointer to phys address
 * @param sz pointer to total pool size
 * @return s32 0 on succuss, error code otherwise
 */
s32 pp_bmgr_pool_base_get(u8 pool_id, void **virt, dma_addr_t *phys,
			  size_t *sz);

#endif /* _PP_BUFFER_MGR_H_ */
