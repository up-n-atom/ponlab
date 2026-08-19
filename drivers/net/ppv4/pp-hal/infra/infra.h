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
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: PP infrastructure h file
 */

#ifndef __INFRA_H__
#define __INFRA_H__
#include <linux/types.h>
#include <linux/init.h>
#include <linux/pp_api.h>
#include <linux/debugfs.h> /* struct dentry */
/**
 * \brief Infra Init Config
 * @valid params valid
 * @bootcfg_base bootconfig base address
 * @clk_ctrl_base clock ctrl base address
 * @dbgfs debugfs parent directory
 */
struct infra_init_param {
	bool valid;
	u64 bootcfg_base;
	u64 clk_ctrl_base;
	struct dentry *dbgfs;
#ifdef CONFIG_SOC_LGM
	bool is_srvs_log_valid;
	u64 service_log_inga_base;
	u64 service_log_ingb_base;
	u64 service_log_qos_base;
#endif
};

#ifdef CONFIG_SOC_LGM
/**
 * @brief services log names
 */
enum service_log {
	SERVICE_LOG_INVALID = -1,
	SERVICE_INGA,
	SERVICE_INGB,
	SERVICE_QOS,
	SERVICE_LOG_NUM,
};

/**
 * @brief service log codes for target flow
 */
enum srvs_log_target_flow {
	BMGR_CFG,
	BMGR_POP_PUSH,
	BOOT_CFG,
	CLK_CTRL,
	CLS_CFG,
	CQM_CFG_NSP,
	DISTRIBUTOR_CFG,
	EGRESS_UC_CFG,
	EGRESS_UC_CL,
	INGRESS_ILA_CFG,
	INGRESS_UC_CL,
	IRC_CFG,
	MOD_CFG,
	PARSER_CFG,
	PPV4_DDR_NSP,
	QM_PUSH,
	QOS_HOST_IF,
	QOS_RX_DMA_QUARY_REQ,
	RPB2_CFG,
	RPB_CFG,
	RX_DMA_CFG,
	RX_DMA_WRED_RESP,
	TARGET_FLOW_NONE
};

/**
 * @brief service log codes for init flow
 */
enum srvs_log_init_flow {
	BMGR_MAIN,
	CHK,
	CLS_HT,
	CLS_SI,
	CQM_MAIN_NSP,
	EGRESS_UC_MAIN,
	INGRESS_UC_MAIN,
	PPV4_HOST_NSP,
	QM_MAIN,
	QOS_RX_DMA_QUARY_RESP,
	QOS_UC_INST,
	QOS_UC_MAIN,
	RPB_MST,
	RX_DMA_MAIN,
	RX_DMA_WRED_REQ,
	TXMNG_MAIN,
	INIT_FLOW_NONE
};

/**
 * @brief service log err values
 * @is_err indicate for err
 * @seq_id which subrange of the target’s memory range was used
 * @subrange which subrange of the target’s memory range was used
 * @init_flow which initiator is the transaction source
 * @target_flow which target is the transaction destination
 */
struct srvs_log_err {
	bool is_err;
	u16 seq_id;
	u16 subrange;
	enum srvs_log_init_flow init_flow;
	enum srvs_log_target_flow target_flow;
};

/**
 * @brief Get infra HW version
 * @return u32 infra hw version
 */
u32 infra_version_get(void);

/**
 * @brief Get PP HW silicon step
 * @return enum pp_silicon_step the silicon step
 */
enum pp_silicon_step pp_silicon_step_get(void);

/**
 * @brief Module init function
 * @param init_param initial parameters
 * @return s32 0 on success
 */
s32 infra_init(struct infra_init_param *init_param);

/**
 * @brief Init dynamic hw clock gating feature
 *        This is done in a separate API cause it MUST
 *        be done after the entire PP was initialized
 */
void infra_dynamic_clk_init(void);

/**
 * @brief Enable/Disable (timeout) fault_en register for specific service log
 * @param srvs_log service log
 * @param en enable/disable
 * @return s32 0 on success, error code otherwise
 */
s32 infra_service_log_fault_en_set(enum service_log srvs_log, bool en);

/**
 * @brief Get fault_en register value for specific service log
 * @param srvs_log service log
 * @param en pointer to get en value
 * @return s32 0 on success, error code otherwise
 */
s32 infra_service_log_fault_en_get(enum service_log srvs_log, bool *en);

/**
 * @brief Clear is_error register for specific service log
 * @param srvs_log service log
 * @return s32 0 on success, error code otherwise
 */
s32 infra_service_log_err_clear(enum service_log srvs_log);

/**
 * @brief Get err value from register for specific service log
 * @param srvs_log service log
 * @param err pointer to get err value
 * @return s32 0 on success, error code otherwise
 */
s32 infra_service_log_get_err(enum service_log srvs_log,
			      struct srvs_log_err *err);

/**
 * @brief Module exit function
 */
void infra_exit(void);
#else
#include "pp_common.h"

static inline s32 infra_init(struct infra_init_param *cfg)
{
	if (cfg->valid)
		return -EINVAL;
	else
		return 0;
}

static inline void infra_exit(void)
{
}

static inline void infra_dynamic_clk_init(void)
{
}

static inline enum pp_silicon_step pp_silicon_step_get(void)
{
	return PP_SSTEP_B;
}

#endif /* CONFIG_SOC_LGM */
#endif /* __INFRA_H__ */
