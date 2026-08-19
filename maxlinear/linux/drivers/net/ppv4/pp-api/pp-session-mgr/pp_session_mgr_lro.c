/*
 * Copyright (C) 2024-2025 MaxLinear, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2, as published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public
 * License along with this program; if not,see
 * <http://www.gnu.org/licenses/>.
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: PP session manager soft LRO module
 */

#define pr_fmt(fmt) "[PP_SESS_MGR_LRO]:%s:%d: " fmt, __func__, __LINE__

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/errno.h>

#include "uc.h"
#include "pp_session_mgr.h"
#include "pp_session_mgr_internal.h"

#define LRO_MAX_AGG_SEGMENTS (44)

/**
 * @brief lro database definitions
 */
struct smgr_lro_db {
	spinlock_t lock;
	struct smgr_lro_conf conf;
};

/**
 * @brief Get SSB buffer size 
 */
static s32 __smgr_lro_ssb_buf_sz_get(u16 policy, u16 *ssb_sz)
{
	struct pp_bmgr_policy_params policy_params;
	struct pp_bmgr_pool_params pool_params;
	s32 ret = 0;
	u16 pool;

	/* Get policy */
	ret = pp_bmgr_policy_conf_get(policy, &policy_params);
	if (ret)
		return ret;

	/* Get pool */
	pool = policy_params.pools_in_policy[0].pool_id;
	ret = pp_bmgr_pool_conf_get(pool, &pool_params);
	if (ret)
		return ret;

	*ssb_sz = pool_params.size_of_buffer;
	return 0;
}

/**
 * @brief return lro_db from smgr_db
 */
static inline struct smgr_lro_db *lro_db_get(void)
{
	struct smgr_database *smgr_db = smgr_get_db();

	if (ptr_is_null(smgr_db))
		return NULL;
	return (struct smgr_lro_db *)smgr_db->lro_db;
}

s32 smgr_lro_conf_set(struct smgr_lro_conf *conf)
{
	struct smgr_lro_db *db = lro_db_get();
	struct uc_ing_cmd msg = {0};
	u16 ssb_buf_sz = 0;
	u32 ssb_policy = 0;
	s32 ret = 0;

	if (ptr_is_null(db))
		return -EINVAL;

	/* Update local db */
	spin_lock_bh(&db->lock);
	memcpy(&db->conf, conf, sizeof(*conf));
	spin_unlock_bh(&db->lock);

	/* Update egress db */
	ret = uc_egr_mbox_cmd_send(UC_CMD_LRO_CONF_SET, 0, (const void *)conf,
				   sizeof(*conf), NULL, 0);

	if (ret)
		goto error;

	if (pp_bmgr_ssb_policy_get(&ssb_policy))
		goto error;

	/* Update ingress db */
	msg.msg_type = ING_MBOX_LRO_PORT_SET;
	msg.ing_cmd.lro_cfg.port   = conf->pid;
	msg.ing_cmd.lro_cfg.policy = ssb_policy;
	if (__smgr_lro_ssb_buf_sz_get(ssb_policy, &ssb_buf_sz))
		goto error;
	msg.ing_cmd.lro_cfg.ssb_pkt_sz =
			ssb_buf_sz - conf->headroom - conf->tailroom;
	ret = uc_ing_host_mbox_cmd_send(&msg, false);
	if (ret)
		goto error;

	return 0;

error:
	pr_err("failed to configure lro\n");
	return ret;
}

s32 smgr_lro_conf_get(struct smgr_lro_conf *conf)
{
	struct smgr_lro_db *db = lro_db_get();

	if (ptr_is_null(conf))
		return -EINVAL;

	if (ptr_is_null(db))
		return -EINVAL;

	spin_lock_bh(&db->lock);
	memcpy(conf, &db->conf, sizeof(*conf));
	spin_unlock_bh(&db->lock);

	return 0;
}

s32 smgr_lro_nf_set(u16 gpid, u16 subif, u16 phyq)
{
	s32 ret = 0;
	u16 policy;
	u8 pool;
	struct smgr_lro_conf conf;
	struct pp_port_cfg port_cfg;
	struct pp_bmgr_policy_params policy_params;

	/* if litepath is disabled, don't configure lro nf */
	if (!gpid || !subif || !phyq)
		return 0;

	/* Get policy */
	ret = pp_port_get(gpid, &port_cfg);
	if (ret)
		return ret;
	policy = port_cfg.tx.base_policy +
			fls(port_cfg.tx.policies_map) - ffs(port_cfg.tx.policies_map);
	
	/* Get pool */
	ret = pp_bmgr_policy_conf_get(policy, &policy_params);
	if (ret)
		return ret;
	pool = policy_params.pools_in_policy[0].pool_id;

	conf.pid           = gpid;
	conf.tx_q          = phyq;
	conf.max_agg_bytes = U16_MAX;
	conf.max_agg_pkts  = LRO_MAX_AGG_SEGMENTS;
	conf.headroom      = port_cfg.tx.headroom_size;
	conf.tailroom      = port_cfg.tx.tailroom_size;
	conf.subif         = subif;
	conf.policy        = policy;
	conf.pool          = pool;

	return smgr_lro_conf_set(&conf);
}

s32 smgr_lro_init(struct device *dev, struct smgr_database *smgr_db)
{
	struct smgr_lro_db *db;

	if (ptr_is_null(dev) || ptr_is_null(smgr_db))
		return -EINVAL;

	/* allocate lro db */
	db = devm_kzalloc(dev, sizeof(*db), GFP_KERNEL);
	if (!db) {
		pr_err("Failed to allocate database memory\n");
		return -ENOMEM;
	}

	/* updating smgr_db to hold db */
	smgr_db->lro_db = db;

	spin_lock_init(&db->lock);

	return 0;
}

void smgr_lro_exit(void)
{
	struct device *dev = pp_dev_get();
	struct smgr_database *smgr_db = smgr_get_db();
	struct smgr_lro_db *db = lro_db_get();

	if (!dev || !smgr_db || !db)
		return;

	devm_kfree(dev, db);
	smgr_db->lro_db = NULL;
}