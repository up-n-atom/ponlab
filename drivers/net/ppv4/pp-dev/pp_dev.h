/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
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
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: PP Device Driver definitions
 */

#ifndef __PP_DEV_H__
#define __PP_DEV_H__
#include <linux/platform_device.h>
#include <linux/clk.h>
#include "infra.h"
#include "bm.h"
#include "port_dist.h"
#include "pp_session_mgr.h"
#include "pp_port_mgr.h"
#include "pp_buffer_mgr.h"
#include "pp_qos_utils.h"
#include "pp_misc.h"
#include "parser.h"
#include "rpb.h"
#include "classifier.h"
#include "checker.h"
#include "modifier.h"
#include "rx_dma.h"
#include "uc.h"

#define PP_MAX_CLKS     4

struct pp_ext_clk {
	bool       valid;
	struct clk *ppv4_freq_clk;
	struct clk *ppv4_gate_clk;
};

/**
 * @brief dts config structure
 */
struct pp_dts_cfg {
	u32 dma_ioc_sz;
	u32 dma_nioc_sz;
	const char *pp_clocks[PP_MAX_CLKS];
	u32 hw_clk; /*! hw_clk HW Clock in MHz */
	struct pp_pmgr_init_param pmgr_params;
	struct pp_smgr_init_param    smgr_params;
	struct pp_bmgr_init_param    bmgr_params;
	struct pp_qos_init_param     qos_params;
	struct infra_init_param      infra_params;
	struct prsr_init_param       parser_params;
	struct rpb_init_param        rpb_params;
	struct port_dist_init_param  port_dist_params;
	struct pp_cls_init_param     cls_param;
	struct pp_chk_init_param     chk_param;
	struct pp_mod_init_param     mod_param;
	struct pp_rx_dma_init_params rx_dma_params;
	struct pp_bm_init_param      bm_params;
	struct uc_init_params        uc_params;
	struct pp_misc_init_param    misc_params;
};

/**
 * @brief device's private data
 */
struct pp_dev_priv {
	/* PP Debug FS directory */
	struct dentry    *pp_dbgfs_dir;
	struct dentry    *pp_hal_dbgfs_dir;
	struct kobject   *pp_sysfs_dir; /* PP sysfs directory */
	struct pp_dts_cfg dts_cfg;
	bool   ready; /* specify if PP driver is ready */
	bool   init_done; /* specify if PP dev init is done */
	bool   is_clk_enable;
	void   *misc_db;
	void   *smgr_db;
	void   *pmgr_db;
};

s32 pp_dts_cfg_get(struct platform_device *pdev, struct pp_dts_cfg *cfg);
void pp_dts_cfg_dump(struct pp_dts_cfg *cfg);
void pp_fpga_cfg_get(struct pp_dts_cfg *cfg);

#endif /* __PP_DEV_H__ */
