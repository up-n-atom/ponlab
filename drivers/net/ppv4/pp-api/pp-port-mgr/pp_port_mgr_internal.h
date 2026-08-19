/*
 * Copyright (C) 2020-2024 MaxLinear, Inc.
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
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: Packet Processor Port Manager Internal Definitions
 */
#ifndef __PP_PORT_MGR_INTERNAL_H__
#define __PP_PORT_MGR_INTERNAL_H__
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/pp_api.h>
#include "pp_dev.h"
#include "pp_common.h"
#include "checker.h"
#include "pp_port_mgr.h"

#ifdef pr_fmt
#undef pr_fmt
#define pr_fmt(fmt) "[PP_PORT_MGR]:%s:%d: " fmt, __func__, __LINE__
#endif

/**
 * @define number of supported datapaths for host interface
 * @note HAL exception session 63 is assign to drop errors
 */
#define PMGR_HIF_DB_DP_MAX             (CHK_NUM_EXCEPTION_SESSIONS - 1)

/**
 * @define used to mark dp number as invalid
 */
#define PMGR_HIF_DP_INVALID            PMGR_HIF_DB_DP_MAX

/**
 * @define check if dp number is valid
 */
#define PMGR_HIF_IS_DP_VALID(dp)       \
	(0 <= (dp) && (dp) < PMGR_HIF_DB_DP_MAX)

/**
 * @define for host interface datapaths database, only queue
 *         index 0 is in used (for using the same data structure
 *         in database
 */
#define PMGR_HIF_DB_EGRESS_IDX         (0)

/**
 * @define shortcut for iterating over ports TCs
 */
#define PMGR_HIF_FOR_EACH_DP_TC(i) for (i = 0; i < PP_MAX_TC; i++)

/**
 * @define shortcut for iterating over ports hash of hashes
 */
#define PMGR_HIF_FOR_EACH_DP_HOH(i) for (i = 0; i < CHK_HOH_MAX; i++)

/**
 * @define going trough all the host interface queues
 */
#define PMGR_HIF_FOR_EACH_DP_EGRESS(i) for (i = 0; i < PP_HOSTIF_EG_MAX; i++)

/**
 * @define going trough all the host interface sgcs
 */
#define PMGR_HIF_FOR_EACH_DP_SGC(i)    for (i = 0; i < PP_SI_SGC_MAX; i++)

/**
 * @define going trough all the host interface tbms
 */
#define PMGR_HIF_FOR_EACH_DP_TBM(i)    for (i = 0; i < PP_SI_TBM_MAX; i++)

/**
 * @define find the datapath index in datapath array by the
 *         number of datapaths and the hash of hash value
 */
#define PMGR_HOH_TO_DP_IDX(hoh, dp_cnt)    \
	(((hoh) * (dp_cnt)) / CHK_HOH_MAX)

enum pmgr_port_flags {
	PORT_FL_ACTIVE,
	PORT_FL_PROTECTED,
	PORT_FL_COUNT,
};

/**
 * @struct module ports database
 * @cfg port configuration
 * @rpb_map the RPB port mapping
 * @grp_id current group id
 * @flags port specific flags
 * @tc_mapped specify whether a tc was mapped or not
 * @prev_map_id exception session mappings.
 *              here we keep an old mapping configuration to be able to restore
 *              it once port is unmapped from a group or stop being protected.
 * @dflt_priority lowest priority for the port in case the port map to
 *                gpid_group.
 * @is_rbp_mapped true if the port mapped to RPB, false otherwise.
 */
struct pmgr_db_port {
	struct pp_port_cfg cfg;
	u32 rpb_map;
	u32 grp_id;
	ulong flags[BITS_TO_LONGS(PORT_FL_COUNT)];
	ulong tc_mapped[BITS_TO_LONGS(PP_MAX_TC)];
	u8 prev_map_id[PP_MAX_TC][CHK_HOH_MAX];
	u8 dflt_priority;
	bool is_rbp_mapped;
};

struct pmgr_db_hif {
	u32 refc;
	u8 color;
	u16 sgc[PP_SI_SGC_MAX];
	u16 tbm[PP_SI_TBM_MAX];
	struct pp_egress eg;
};

/**
 * @struct module database
 */
struct pmgr_db {
	struct pmgr_db_hif  hif_dp[PMGR_HIF_DB_DP_MAX];
	struct pmgr_db_port port[PP_MAX_PORT];
	struct pmgr_db_grp  grp[UC_GPID_GRP_CNT];
	struct pmgr_stats   stats;
	/*! rules reference count */
	u8 wl_rule_ref[ING_WL_RULE_TYPE_COUNT][ING_WL_RULE_MAX_RULES];
	/*! hifs busy bitmap */
	ulong hif_bmap[BITS_TO_LONGS(PMGR_HIF_DB_DP_MAX)];
	/*! groups busy bitmap */
	ulong grp_bmap[BITS_TO_LONGS(UC_GPID_GRP_CNT)];
	spinlock_t lock;
	/*! default dp id */
	u8 dflt_dp;
	/*! pp protected group id */
	u32 protected_grp_id;
};

static inline struct pmgr_db *pmgr_get_db(void)
{
	struct pp_dev_priv *pp_priv = pp_priv_get();

	if (unlikely(!pp_is_ready()))
		return NULL;

	if (unlikely(!pp_priv))
		return NULL;

	return (struct pmgr_db *)pp_priv->pmgr_db;
}

static inline bool pmgr_test_port_flag(u16 pid, enum pmgr_port_flags f)
{
	struct pmgr_db *db = pmgr_get_db();

	if (unlikely(ptr_is_null(db)))
		return false;

	return test_bit(f, db->port[pid].flags);
}

static inline void pmgr_set_port_flag(u16 pid, enum pmgr_port_flags f)
{
	struct pmgr_db *db = pmgr_get_db();

	if (unlikely(ptr_is_null(db)))
		return;

	set_bit(f, db->port[pid].flags);
}

static inline void pmgr_clear_port_flag(u16 pid, enum pmgr_port_flags f)
{
	struct pmgr_db *db = pmgr_get_db();

	if (unlikely(ptr_is_null(db)))
		return;

	clear_bit(f, db->port[pid].flags);
}

#define pmgr_port_is_protected(p) pmgr_test_port_flag(p, PORT_FL_PROTECTED)

/**
 * @brief Init pp_hif_datapath structure
 * @param dp
 */
static inline void pp_hif_init(struct pp_hif_datapath *dp)
{
	u32 i;

	dp->color = PP_COLOR_INVALID;

	for (i = 0; i < ARRAY_SIZE(dp->eg); i++) {
		dp->eg[i].qos_q = PP_QOS_INVALID_ID;
		dp->eg[i].pid = PP_PORT_INVALID;
	}
	PMGR_HIF_FOR_EACH_DP_SGC(i)
		dp->sgc[i] = PP_SGC_INVALID;
	PMGR_HIF_FOR_EACH_DP_TBM(i)
		dp->tbm[i] = PP_TBM_INVALID;
}

/**
 * @brief Init pmgr_db_hif structure
 * @param dp
 */
static inline void db_hif_init(struct pmgr_db_hif *dp)
{
	u32 i;

	dp->color = PP_COLOR_INVALID;
	dp->eg.pid = PP_PORT_INVALID;
	dp->eg.qos_q = PP_PORT_INVALID;

	PMGR_HIF_FOR_EACH_DP_SGC(i)
		dp->sgc[i] = PP_SGC_INVALID;
	PMGR_HIF_FOR_EACH_DP_TBM(i)
		dp->tbm[i] = PP_TBM_INVALID;
}

/**
 * @brief Check if port is active in Module database
 * @note called under lock
 * @param pid port index
 * @return bool true if port active, false otherwise
 */
bool __pmgr_port_is_active(u16 pid);

/**
 * @brief Get port's DB info copy
 * @param pid the port id
 * @param info info buffer to copy into
 * @return s32 0 on succuss, error code otherwise
 */
s32 pmgr_port_db_info_get(u16 pid, struct pmgr_db_port *info);

/**
 * @brief Add new host interface datapath
 * @note called under lock
 * @param dp host interface datapath parameter
 * @param dp_num host interface datapath index for database
 * @param eg_idx egress index to add from dp
 * @param exp_flags extra exception session flags to add,
 *                  SI_CHCK_FLAG_UPDT_SESS_CNT is on for all DPs,
 *                  see enum si_chck_flags for more flags
 * @return s32 0 for success, non-zero otherwise
 */
s32 pmgr_hif_dp_add(struct pp_hif_datapath *dp, u8 dp_num, u8 eg_idx,
		    ulong exp_flags);

/**
 * @brief Get ports stats
 * @param stats pointer to save the stats
 * @param num_stats number of stats entries
 * @param data user data
 * @return s32 0 on success, error code otherwise
 */
s32 pmgr_ports_stats_get(void *stats, u32 num_stats, void *data);

/**
 * @brief Calculates the difference between ports stats
 * @param pre pre stats
 * @param num_pre number of pre stats
 * @param post post stats
 * @param num_post number of post stats
 * @param delta stats to save the delta
 * @param num_delta number of delta stats
 * @note all stats array MUST be with same size
 * @return s32 0 on success, error code otherwise
 */
s32 pmgr_ports_stats_diff(void *pre, u32 num_pre, void *post, u32 num_post,
			  void *delta, u32 num_delta, void *data);

/**
 * @brief Prints ports stats into a buffer
 * @param buf the buffer to print into
 * @param sz buffer size
 * @param n pointer to update number of written bytes
 * @param stats the stats
 * @param num_stats number of stats
 * @return s32 0 on success, error code otherwise
 */
s32 pmgr_ports_stats_show(char *buf, size_t sz, size_t *n, void *stats,
			  u32 num_stats, void *data);

/**
 * @brief get the port mapping in RPB
 * @param pid port number
 * @param rpb_port RPB port map
 * @return s32 0 for success, non-zero otherwise
 */
s32 pmgr_port_rpb_map_get(u16 pid, u32 *rpb_port);

/**
 * @brief get the host interface datapath from databse
 * @param dp_idx datapath index in database
 * @param dp datapath configuration
 * @return S32 0 for success, non-zero otherwise
 */
s32 pmgr_hif_dp_get(u8 dp_idx, struct pmgr_db_hif *dp);

/**
 * @brief get the host interface datapath refc from databse
 * @param dp_idx datapath index in database
 * @param refc reference count
 * @return S32 0 for success, non-zero otherwise
 */
s32 pmgr_hif_dp_refc_get(u8 dp_idx, u32 *refc);

/**
 * @brief get the host interface mapping per ingress
 *        port/tc/hoh
 * @param pid port number [0..255]
 * @param tc traffic class value [0..3]
 * @param hoh FV hash of hash result [0..3]
 * @return u8 host interface datapath ID, PMGR_HIF_DB_DP_MAX for
 *         error
 */
u8 pmgr_port_hif_map_get(u16 pid, u8 tc, u8 hoh);

/**
 * @brief port manager sysfs init
 * @return s32 0 for success, non-zero otherwise
 */
s32 pmgr_sysfs_init(struct kobject *parent);

/**
 * @brief port manager sysfs cleanup
 */
void pmgr_sysfs_clean(void);

#ifdef CONFIG_DEBUG_FS
/**
 * @brief port manager debug init
 * @return s32 0 for success, non-zero otherwise
 */
s32 pmgr_dbg_init(struct dentry *parent);

/**
 * @brief port manager debug cleanup
 * @return s32 0 for success, non-zero otherwise
 */
s32 pmgr_dbg_clean(void);

/**
 * @brief port debug init
 * @param pid port number
 * @return s32 0 for success, non-zero otherwise
 */
s32 pmgr_port_dbg_init(u16 pid);

/**
 * @brief port debug cleanup
 * @param pid port number
 * @return s32 0 for success, non-zero otherwise
 */
s32 pmgr_port_dbg_clean(u16 pid);

/**
 * @brief hostif dp debug init
 * @param dp dp number
 * @return s32 0 for success, non-zero otherwise
 */
s32 pmgr_dp_dbg_create(u8 dp);


/**
 * @brief hostif dp debug cleanup
 * @param dp dp number
 * @return s32 0 for success, non-zero otherwise
 */
s32 pmgr_dp_dbg_clean(u8 dp);

/**
 * @brief Create gpid group debugfs dir
 * @param grp group id
 * @param name name (optional)
 * @return s32 0 on succuss, error code otherwise
 */
s32 pmgr_gpid_grp_dbg_create(u32 grp, const char *name);

/**
 * @brief Clean gpid group debugfs dir
 * @param grp group id
 * @param name name (optional)
 * @return s32 0 on succuss, error code otherwise
 */
s32 pmgr_gpid_grp_dbg_clean(u32 grp, const char *name);

/**
 * @brief Get active GPID groups bitmap
 *        Bit i specify if group i is active
 * @param grp_bmap buffer to copy the bitmap to
 * @return s32 0 on succuss, error code otherwise
 */
s32 pmgr_gpid_groups_bmap_get(ulong *grp_bmap);

#else /* !CONFIG_DEBUG_FS */
static inline s32 pmgr_dbg_init(struct dentry *parent)
{
	return 0;
}

static inline s32 pmgr_dbg_clean(void)
{
	return 0;
}

static inline s32 pmgr_port_dbg_init(u16 pid)
{
	return 0;
}

static inline s32 pmgr_port_dbg_clean(u16 pid)
{
	return 0;
}

static inline s32 pmgr_dp_dbg_create(u8 dp)
{
	return 0;
}

static inline s32 pmgr_dp_dbg_clean(u8 dp)
{
	return 0;
}

s32 pmgr_gpid_grp_dbg_create(u32 grp, const char *name)
{
	return 0;
}

s32 pmgr_gpid_grp_dbg_clean(u32 grp, const char *name)
{
	return 0;
}

#endif /* CONFIG_DEBUG_FS */
#endif /* __PP_PORT_MGR_INTERNAL_H__ */
