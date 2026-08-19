/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2016-2020 Intel Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2, as published
 * by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CQM_DEV_H
#define _CQM_DEV_H
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/export.h>
#include <linux/err.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/clk.h>

#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/reset.h>
//#include <lantiq.h>
#include <linux/platform_device.h>
#include <linux/mfd/syscon.h>
//#include <lantiq_soc.h>
#include "cqm_common.h"

#define LGM_DEV_NAME "lgm-cqm"
#define FALCON_DEV_NAME "falcon-cqm"
#define GRX500_DEV_NAME "grx500-cbm"
#define NUM_BM_POOLS 4
#define NUM_EXTEND_BM_POOLS 12
#define NUM_BM_POLICY 256
#define MAX_NUM_POOLS (NUM_BM_POOLS + NUM_EXTEND_BM_POOLS)
#define MAX_CPU_DQ_PORT_ARGS		2
#define MAX_DPDK_DQ_PORT_ARGS		2
#define MAX_CPU_DQ_PORT_N_TYPE	8
#define MAX_CPU_NUM 8
#define MAX_CQM_FW_DOMAIN 2

struct dt_node_inst {
	char *dev_name;
	char *node_name;
	int instance_id;
};

struct lpid_epg_map_s {
	u32 lpid;
	u32 epg;
	u8 txdma_ctrl;
	u8 txdma_chan;
};

struct deq_dma_delay_s {
	u32 dcp;
	u32 deq_port;
	u32 delay;
};

struct cqm_data {
	int num_resources;
	int num_intrs;
	unsigned int *intrs;
	struct clk *cqm_clk[2];
	struct reset_control *rcu_reset;
	struct regmap *syscfg;
	int force_xpcs;
	u32 num_sys_pools;
	u32 num_bm_pools;
	unsigned long pool_ptrs[MAX_NUM_POOLS];
	unsigned long pool_size[MAX_NUM_POOLS];
	unsigned long pool_type[MAX_NUM_POOLS];
	phys_addr_t voice_buf_start;
	phys_addr_t voice_buf_size;
	u32 dq_port[MAX_CPU_DQ_PORT_N_TYPE];
	u32 dpdk_cfg[MAX_CPU_DQ_PORT_N_TYPE];
	u32 config_dpdk;
	u32 num_dq_port;
	int sys_type;
	bool txin_fallback;
	bool cpu_enq_chk;
	int cqm_clkrate;
	struct cqm_policy_res policy_res[NUM_BM_POLICY];
	struct cqm_policy_params policy_params[NUM_BM_POLICY];
	int wav700;
	u32 port_resv[2];
	int sai[MAX_CQM_FW_DOMAIN];
	u32 lpid_port_mode;
	u8 lpid_wan_mode;
	struct lpid_epg_map_s lpid_epg_map[LPID_COUNT];
	struct deq_dma_delay_s deq_dma_delay[DLY_PORT_COUNT];
	u32 bm_buf_sizes[BM_BUF_SIZE_MAX];
};
#endif
