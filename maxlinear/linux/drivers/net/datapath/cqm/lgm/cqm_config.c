// SPDX-License-Identifier: GPL-2.0
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

#include "cqm.h"
#include <net/mxl_cbm_api.h>
#include <dt-bindings/net/mxl,lgm-cqm.h>
#include "cqm_config.h"

#define LGM_CQM_SYS_NUM 7

struct cqm_bm_pool_config bm_pool_conf[CQM_LGM_TOTAL_POOLS] = {
	{
	.segment_mask = 0xFFFFFE00,
	.offset_mask = 0x1ff,
	.pool = 0,
	.policy = 0
	},
	{
	.segment_mask = 0xFFFFFE00,
	.offset_mask = 0x1ff,
	.pool = 1,
	.policy = 1
	},
	{
	.segment_mask = 0xFFFFFE00,
	.offset_mask = 0x1ff,
	.pool = 2,
	.policy = 2
	},
	{
	.segment_mask = 0xFFFFFE00,
	.offset_mask = 0x1ff,
	.pool = 3,
	.policy = 3
	},
};

struct cqm_bm_pool_config fsqm_pool_conf[] = {
	{
	.buf_frm_size = CQM_SRAM_FRM_SIZE,
	.buf_type = BUF_TYPE_FSQM,
	.segment_mask = (CQM_SRAM_FRM_SIZE == 2048) ? 0xfffff800 : 0xffffff80,
	.offset_mask = (CQM_SRAM_FRM_SIZE == 2048) ? 0x7ff : 0x7f,
	.pool = 0xff,
	.policy = 0xff,
	.pool_start_low = (unsigned int)(CQM_SRAM_BASE),
	.pool_end_low = (unsigned int)(CQM_SRAM_BASE + CQM_SRAM_SIZE),
	},
};

#define port_cfg_urx851_common \
	{\
		.type = DQM_CPU_TYPE,\
		.data.dqm_cpu = {\
			.port = 0,\
			.port_range = 7,\
			.cpu_port_type = DP_F_DEQ_CPU,\
			.num_desc = 2,\
			.txpush_desc = 2,\
		}\
	},\
	{\
		.type = DQM_CPU_TYPE,\
		.data.dqm_cpu = {\
			.port = 8,\
			.port_range = 7,\
			.cpu_port_type = DEQ_VM,\
			.num_desc = 2,\
			.txpush_desc = 2,\
		}\
	},\
	{\
		.type = DQM_CPU_TYPE,\
		.data.dqm_cpu = {\
			.port = 16,\
			.port_range = 0,\
			.cpu_port_type = DEQ_TOE,\
			.num_desc = 2,\
			.txpush_desc = 2,\
		}\
	},\
	{\
		.type = DQM_CPU_TYPE,\
		.data.dqm_cpu = {\
			.port = 17,\
			.port_range = 1,\
			.cpu_port_type = DEQ_VOICE,\
			.num_desc = 4,\
			.txpush_desc = 2,\
		}\
	},\
	{\
		.type = DQM_DMA_TYPE,\
		.data.dqm_dma = {\
			.port = 27,\
			.port_type = DEQ_VUNI,\
			.port_range = 0,\
			.dma_ctrl = 2,\
			.dma_chan = 0,\
			.port_enable = 1,\
			.num_desc = 8,\
			.txpush_desc = 8,\
		}\
	},\
	{\
		.type = DQM_DMA_TYPE,\
		.data.dqm_dma = {\
			.port = 28,\
			.port_type = DEQ_REINSERT,\
			.port_range = 0,\
			.dma_ctrl = 2,\
			.dma_chan = 1,\
			.port_enable = 1,\
			.num_desc = 8,\
			.txpush_desc = 8,\
		}\
	},\
	{\
		.type = DQM_DMA_TYPE,\
		.data.dqm_dma = {\
			.port = 29,\
			.port_type = DEQ_FSQM_DESC,\
			.port_range = 3,\
			.dma_ctrl = 2,\
			.dma_chan = 2,\
			.port_enable = 1,\
			.num_desc = 8,\
			.txpush_desc = 8,\
		}\
	},\
	{\
		.type = DQM_DMA_TYPE,\
		.data.dqm_dma = {\
			.port = 33,\
			.port_type = DEQ_FLUSH,\
			.port_range = 0,\
			.dma_ctrl = 2,\
			.dma_chan = 6,\
			.port_enable = 1,\
			.num_desc = 8,\
			.txpush_desc = 8,\
		}\
	},\
	{\
		.type = DQM_DMA_TYPE,\
		.data.dqm_dma = {\
			.port = 35,\
			.port_range = 7,\
			.dma_ctrl = 2,\
			.dma_chan = 8,\
			.port_enable = 1,\
			.num_desc = 8,\
			.txpush_desc = 8,\
		}\
	},\
	{\
		.type = DQM_DMA_TYPE,\
		.data.dqm_dma = {\
			.port = 45,\
			.port_type = DEQ_FSQM_DESC,\
			.port_range = 3,\
			.dma_ctrl = 1,\
			.dma_chan = 2,\
			.port_enable = 1,\
			.num_desc = 8,\
			.txpush_desc = 8,\
		}\
	},\
	{\
		.type = DQM_DMA_TYPE,\
		.data.dqm_dma = {\
			.port = 51,\
			.port_range = 6,\
			.dma_ctrl = 1,\
			.dma_chan = 8,\
			.port_enable = 1,\
			.num_desc = 8,\
			.txpush_desc = 8,\
		}\
	},\
	{\
		.type = DQM_DMA_TYPE,\
		.data.dqm_dma = {\
			.port = 59,\
			.port_type = DEQ_DC_DOCSIS,\
			.port_range = 7,\
			.dma_ctrl = 0,\
			.dma_chan = 0,\
			.port_enable = 1,\
			.num_desc = 8,\
			.txpush_desc = 8,\
		}\
	},\
	{\
		.type = DQM_DMA_TYPE,\
		.data.dqm_dma = {\
			.port = 67,\
			.port_type = DEQ_CPU_QOSBP,\
			.port_range = 4,\
			.dma_ctrl = 0,\
			.dma_chan = 8,\
			.port_enable = 1,\
			.num_desc = 8,\
			.txpush_desc = 8,\
		}\
	},\
	{\
		.type = DQM_DMA_TYPE,\
		.data.dqm_dma = {\
			.port = 72,\
			.port_type = DEQ_TOE,\
			.port_range = 0,\
			.dma_ctrl = 0,\
			.dma_chan = 13,\
			.port_enable = 1,\
			.num_desc = 8,\
			.txpush_desc = 8,\
		}\
	},\
	{\
		.type = DQM_DMA_TYPE,\
		.data.dqm_dma = {\
			.port = 73,\
			.port_type = DEQ_PP_NF,\
			.port_range = 0,\
			.dma_ctrl = 0,\
			.dma_chan = 14,\
			.port_enable = 1,\
			.num_desc = 8,\
			.txpush_desc = 8,\
		}\
	},\
	{\
		.type = DQM_DMA_TYPE,\
		.data.dqm_dma = {\
			.port = 74,\
			.port_type = DEQ_VPN,\
			.port_range = 0,\
			.dma_ctrl = 0,\
			.dma_chan = 15,\
			.port_enable = 1,\
			.num_desc = 8,\
			.txpush_desc = 8,\
		}\
	},\
	{\
		.type = DQM_DMA_TYPE,\
		.data.dqm_dma = {\
			.port = DMA_PORT_RE_INSERTION,\
			.port_type = DEQ_REINSERT,\
			.port_range = 0,\
			.dma_ctrl = 2,\
			.dma_chan = 1,\
			.port_enable = 1,\
			.num_desc = 8,\
			.txpush_desc = 8,\
		}\
	},\
	{\
		.type = DQM_PP_NF_TYPE,\
		.data.dqm_dma = {\
			.port = 139,\
			.port_type = DEQ_PP_NF,\
			.port_range = 255 - 139,\
			.port_enable = 1,\
			.num_desc = 8,\
		}\
	},\
	{\
		.type = EQM_CPU_TYPE,\
		.data.eqm_cpu = {\
			.port = 0,\
			.port_range = 3,\
			.port_type = EQM_CPU,\
			.num_desc = 2,\
		}\
	},\
	{\
		.type = EQM_CPU_TYPE,\
		.data.eqm_cpu = {\
			.port = 4,\
			.port_range = 5,\
			.port_type = EQM_DPDK,\
			.num_desc = 2,\
		}\
	},\
	{\
		.type = EQM_CPU_TYPE,\
		.data.eqm_cpu = {\
			.port = 11,\
			.port_range = 0,\
			.port_type = EQM_VOICE,\
			.num_desc = 2,\
		}\
	},\
	{\
		.type = EQM_CPU_TYPE,\
		.data.eqm_cpu = {\
			.port = 12,\
			.port_range = 3,\
			.port_type = EQM_CPU_QBYP,\
			.num_desc = 2,\
		}\
	},\
	{\
		.type = EQM_CPU_TYPE,\
		.data.eqm_cpu = {\
			.port = 16,\
			.port_range = 7,\
			.port_type = EQM_ACA,\
			.num_desc = 32,\
			.num_alloc_burst = 32,\
		}\
	},\
	{\
		.type = EQM_DMA_TYPE,\
		.data.eqm_dma = {\
			.port = 24,\
			.port_range = 4,\
			.port_type = EQM_DMA_PON,\
			.dma_ctrl = 1,\
			.dma_chnl = 0,\
			.num_desc = 8,\
		}\
	},\
	{\
		.type = EQM_DMA_TYPE,\
		.data.eqm_dma = {\
			.port = 29,\
			.port_range = 0,\
			.port_type = EQM_DMA_HEADER,\
			.dma_ctrl = 1,\
			.dma_chnl = 5,\
			.num_desc = 8,\
		}\
	},\
	{\
		.type = EQM_DMA_TYPE,\
		.data.eqm_dma = {\
			.port = 30,\
			.port_range = 0,\
			.port_type = EQM_DMA_SIZE2,\
			.dma_ctrl = 1,\
			.dma_chnl = 6,\
			.num_desc = 8,\
		}\
	},\
	{\
		.type = EQM_DMA_TYPE,\
		.data.eqm_dma = {\
			.port = 31,\
			.port_range = 0,\
			.port_type = EQM_TSO,\
			.dma_ctrl = 0,\
			.dma_chnl = 0,\
			.num_desc = 8,\
		}\
	},\
	{\
		.type = EQM_DMA_TYPE,\
		.data.eqm_dma = {\
			.port = 32,\
			.port_range = 0,\
			.port_type = EQM_LRO,\
			.dma_ctrl = 0,\
			.dma_chnl = 0,\
			.num_desc = 8,\
		}\
	},\
	{\
		.type = EQM_DMA_TYPE,\
		.data.eqm_dma = {\
			.port = 33,\
			.port_range = 0,\
			.port_type = EQM_RXDMA,\
			.dma_ctrl = 0,\
			.dma_chnl = 0,\
			.num_desc = 1,\
		}\
	}

#define port_cfg_urx851b_common \
	port_cfg_urx851_common, \
	{\
		.type = DQM_ACA_TYPE,\
		.data.dqm_aca = {\
			.port = 19,\
			.port_range = 7,\
			.cpu_port_type = DEQ_ACA,\
			.num_desc = 32,\
			.txpush_desc = 32,\
			.num_free_burst = 32,\
		}\
	}

#define port_cfg_urx851c_common \
	port_cfg_urx851_common, \
	{\
		.type = DQM_ACA_TYPE,\
		.data.dqm_aca = {\
			.port = 19,\
			.port_range = 7,\
			.cpu_port_type = DEQ_ACA,\
			.num_desc = 64,\
			.txpush_desc = 32,\
			.num_free_burst = 64,\
		}\
	}

static struct cqm_port_config port_cfg_docsis_urx851b[] = {
	port_cfg_urx851b_common,
	{
		.type = DQM_DOCSIS_TYPE,
		.data.dqm_dma = {
			.port = 75,
			.port_range = 35,
			.dma_ctrl = 0,
			.dma_chan = 0,
			.port_enable = 1,
			.num_desc = 16,
			.txpush_desc = 8,
		}
	},
	{
		.type = NONE_TYPE,
	}
};

static struct cqm_port_config port_cfg_gint_urx851b[] = {
	{
		.type = NONE_TYPE,
	}
};

static struct cqm_port_config port_cfg_pon_urx851b[] = {
	port_cfg_urx851b_common,
	{
		.type = DQM_PON_TYPE,
		.data.dqm_dma = {
			.port = 75,
			.port_range = 63,
			.dma_ctrl = 0,
			.dma_chan = 0,
			.port_enable = 1,
			.num_desc = 16,
			.txpush_desc = 8,
		}
	},
	{
		.type = NONE_TYPE,
	}
};

static struct cqm_port_config port_cfg_docsis_urx851c[] = {
	port_cfg_urx851c_common,
	{
		.type = DQM_DOCSIS_TYPE,
		.data.dqm_dma = {
			.port = 75,
			.port_range = 35,
			.dma_ctrl = 0,
			.dma_chan = 0,
			.port_enable = 1,
			.num_desc = 16,
			.txpush_desc = 8,
		}
	},
	{
		.type = NONE_TYPE,
	}
};

static struct cqm_port_config port_cfg_gint_urx851c[] = {
	{
		.type = NONE_TYPE,
	}
};

static struct cqm_port_config port_cfg_pon_urx851c[] = {
	port_cfg_urx851c_common,
	{
		.type = DQM_PON_TYPE,
		.data.dqm_dma = {
			.port = 75,
			.port_range = 63,
			.dma_ctrl = 0,
			.dma_chan = 0,
			.port_enable = 1,
			.num_desc = 16,
			.txpush_desc = 8,
		}
	},
	{
		.type = NONE_TYPE,
	}
};

static struct cqm_port_config port_cfg_urx651[] = {
	{
		.type = DQM_CPU_TYPE,
		.data.dqm_cpu = {
			.port = 0,
			.port_range = 3,
			.cpu_port_type = DP_F_DEQ_CPU,
			.num_desc = 2,
			.txpush_desc = 2,
		}
	},
	{
		.type = DQM_CPU_TYPE,
		.data.dqm_cpu = {
			.port = 8,
			.port_range = 2,
			.cpu_port_type = DP_F_DEQ_INVALID,
			.num_desc = 2,
			.txpush_desc = 2,
		}
	},
	{
		.type = DQM_CPU_TYPE,
		.data.dqm_cpu = {
			.port = 16,
			.port_range = 0,
			.cpu_port_type = DEQ_TOE,
			.num_desc = 2,
			.txpush_desc = 2,
		}
	},
	{
		.type = DQM_CPU_TYPE,
		.data.dqm_cpu = {
			.port = 17,
			.port_range = 0,
			.cpu_port_type = DEQ_VOICE,
			.num_desc = 4,
			.txpush_desc = 2,
		}
	},
	{
		.type = DQM_ACA_TYPE,
		.data.dqm_aca = {
			.port = 19,
			.port_range = 3,
			.cpu_port_type = DEQ_ACA,
			.num_desc = 32,
			.txpush_desc = 32,
			.num_free_burst = 32,
		}
	},
	{
		.type = DQM_DMA_TYPE,
		.data.dqm_dma = {
			.port = 27,
			.port_type = DEQ_VUNI,
			.port_range = 0,
			.dma_ctrl = 2,
			.dma_chan = 0,
			.port_enable = 1,
			.num_desc = 8,
			.txpush_desc = 8,
		}
	},
	{
		.type = DQM_DMA_TYPE,
		.data.dqm_dma = {
			.port = 28,
			.port_type = DEQ_REINSERT,
			.port_range = 0,
			.dma_ctrl = 2,
			.dma_chan = 1,
			.port_enable = 1,
			.num_desc = 8,
			.txpush_desc = 8,
		}
	},
	{
		.type = DQM_DMA_TYPE,
		.data.dqm_dma = {
			.port = 29,
			.port_type = DEQ_FSQM_DESC,
			.port_range = 3,
			.dma_ctrl = 2,
			.dma_chan = 2,
			.port_enable = 1,
			.num_desc = 8,
			.txpush_desc = 8,
		}
	},
	{
		.type = DQM_DMA_TYPE,
		.data.dqm_dma = {
			.port = 33,
			.port_type = DEQ_FLUSH,
			.port_range = 0,
			.dma_ctrl = 2,
			.dma_chan = 6,
			.port_enable = 1,
			.num_desc = 8,
			.txpush_desc = 8,
		}
	},
	{
		.type = DQM_DMA_TYPE,
		.data.dqm_dma = {
			.port = 35,
			.port_range = 7,
			.dma_ctrl = 2,
			.dma_chan = 8,
			.port_enable = 1,
			.num_desc = 8,
			.txpush_desc = 8,
		}
	},
	{
		.type = DQM_DMA_TYPE,
		.data.dqm_dma = {
			.port = 45,
			.port_type = DEQ_FSQM_DESC,
			.port_range = 3,
			.dma_ctrl = 1,
			.dma_chan = 2,
			.port_enable = 1,
			.num_desc = 8,
			.txpush_desc = 8,
		}
	},
	{
		.type = DQM_DMA_TYPE,
		.data.dqm_dma = {
			.port = 51,
			.port_range = 6,
			.dma_ctrl = 1,
			.dma_chan = 8,
			.port_enable = 1,
			.num_desc = 8,
			.txpush_desc = 8,
		}
	},
	{
		.type = DQM_DMA_TYPE,
		.data.dqm_dma = {
			.port = 59,
			.port_type = DEQ_DC_DOCSIS,
			.port_range = 7,
			.dma_ctrl = 0,
			.dma_chan = 0,
			.port_enable = 1,
			.num_desc = 8,
			.txpush_desc = 8,
		}
	},
	{
		.type = DQM_DMA_TYPE,
		.data.dqm_dma = {
			.port = 67,
			.port_type = DEQ_CPU_QOSBP,
			.port_range = 4,
			.dma_ctrl = 0,
			.dma_chan = 8,
			.port_enable = 1,
			.num_desc = 8,
			.txpush_desc = 8,
		}
	},
	{
		.type = DQM_DMA_TYPE,
		.data.dqm_dma = {
			.port = 72,
			.port_type = DEQ_TOE,
			.port_range = 0,
			.dma_ctrl = 0,
			.dma_chan = 13,
			.port_enable = 1,
			.num_desc = 8,
			.txpush_desc = 8,
		}
	},
	{
		.type = DQM_DMA_TYPE,
		.data.dqm_dma = {
			.port = 73,
			.port_type = DEQ_PP_NF,
			.port_range = 0,
			.dma_ctrl = 0,
			.dma_chan = 14,
			.port_enable = 1,
			.num_desc = 8,
			.txpush_desc = 8,
		}
	},
	{
		.type = DQM_DMA_TYPE,
		.data.dqm_dma = {
			.port = 74,
			.port_type = DEQ_VPN,
			.port_range = 0,
			.dma_ctrl = 0,
			.dma_chan = 15,
			.port_enable = 1,
			.num_desc = 8,
			.txpush_desc = 8,
		}
	},
	{
		.type = DQM_DMA_TYPE,
		.data.dqm_dma = {
			.port = DMA_PORT_RE_INSERTION,
			.port_type = DEQ_REINSERT,
			.port_range = 0,
			.dma_ctrl = 2,
			.dma_chan = 1,
			.port_enable = 1,
			.num_desc = 8,
			.txpush_desc = 8,
		}
	},
	{
		.type = DQM_PON_TYPE,
		.data.dqm_dma = {
			.port = 75,
			.port_range = 63,
			.dma_ctrl = 0,
			.dma_chan = 0,
			.port_enable = 1,
			.num_desc = 16,
			.txpush_desc = 8,
		}
	},
	{
		.type = DQM_PP_NF_TYPE,
		.data.dqm_dma = {
			.port = 139,
			.port_type = DEQ_PP_NF,
			.port_range = 255 - 139,
			.port_enable = 1,
			.num_desc = 8,
		}
	},
	{
		.type = EQM_CPU_TYPE,
		.data.eqm_cpu = {
			.port = 0,
			.port_range = 3,
			.port_type = EQM_CPU,
			.num_desc = 2,
		}
	},
	{
		.type = EQM_CPU_TYPE,
		.data.eqm_cpu = {
			.port = 4,
			.port_range = 2,
			.port_type = EQM_DPDK,
			.num_desc = 2,
		}
	},
	{
		.type = EQM_CPU_TYPE,
		.data.eqm_cpu = {
			.port = 11,
			.port_range = 0,
			.port_type = EQM_VOICE,
			.num_desc = 2,
		}
	},
	{
		.type = EQM_CPU_TYPE,
		.data.eqm_cpu = {
			.port = 12,
			.port_range = 1,
			.port_type = EQM_CPU_QBYP,
			.num_desc = 2,
		}
	},
	{
		.type = EQM_CPU_TYPE,
		.data.eqm_cpu = {
			.port = 16,
			.port_range = 3,
			.port_type = EQM_ACA,
			.num_desc = 32,
			.num_alloc_burst = 32,
		}
	},
	{
		.type = EQM_DMA_TYPE,
		.data.eqm_dma = {
			.port = 24,
			.port_range = 4,
			.port_type = EQM_DMA_PON,
			.dma_ctrl = 1,
			.dma_chnl = 0,
			.num_desc = 8,
		}
	},
	{
		.type = EQM_DMA_TYPE,
		.data.eqm_dma = {
			.port = 29,
			.port_range = 0,
			.port_type = EQM_DMA_HEADER,
			.dma_ctrl = 0,
			.dma_chnl = 5,
			.num_desc = 8,
		}
	},
	{
		.type = EQM_DMA_TYPE,
		.data.eqm_dma = {
			.port = 30,
			.port_range = 0,
			.port_type = EQM_DMA_SIZE2,
			.dma_ctrl = 0,
			.dma_chnl = 6,
			.num_desc = 8,
		}
	},
	{
		.type = EQM_DMA_TYPE,
		.data.eqm_dma = {
			.port = 31,
			.port_range = 0,
			.port_type = EQM_TSO,
			.dma_ctrl = 0,
			.dma_chnl = 0,
			.num_desc = 8,
		}
	},
	{
		.type = EQM_DMA_TYPE,
		.data.eqm_dma = {
			.port = 32,
			.port_range = 0,
			.port_type = EQM_LRO,
			.dma_ctrl = 0,
			.dma_chnl = 0,
			.num_desc = 8,
		}
	},
	{
		.type = EQM_DMA_TYPE,
		.data.eqm_dma = {
			.port = 33,
			.port_range = 0,
			.port_type = EQM_RXDMA,
			.dma_ctrl = 0,
			.dma_chnl = 0,
			/*PPV4 uses only 1 descriptor*/
			.num_desc = 1,
		}
	},
	{
		.type = NONE_TYPE,
	}
};

static struct cqm_sys_config sys_config[LGM_CQM_SYS_NUM] = {
	{ LGM_SYS_PON, URX851B, port_cfg_pon_urx851b},
	{ LGM_SYS_DOCSIS, URX851B, port_cfg_docsis_urx851b},
	{ LGM_SYS_GINT, URX851B, port_cfg_gint_urx851b},
	{ LGM_SYS_PON, URX851C, port_cfg_pon_urx851c},
	{ LGM_SYS_DOCSIS, URX851C, port_cfg_docsis_urx851c},
	{ LGM_SYS_GINT, URX851C, port_cfg_gint_urx851c},
	{ LGM_SYS_PON, URX651, port_cfg_urx651},
};

static struct cqm_ctrl g_lgm_ctrl = {
	.num_dqm_ports = CQM_LGM_TOTAL_DEQ_PORTS,
	.num_eqm_ports = CQM_ENQ_PORT_MAX,
	.max_queues = 512,
	.total_pool_entries = CQM_LGM_TOTAL_BM_POOLS,
	.num_intrs = 8,
	.sys_type = LGM_SYS_PON,
	.cqm_cfg = port_cfg_pon_urx851b,
	.max_mem_alloc_sys = 0,
	.max_mem_alloc_cpu = 0,
	.max_mem_alloc = 0,
	.bm_buf_base[0] = 0,
	.bm_buf_base[1] = 0,
	.bm_buf_base[2] = 0,
	.bm_buf_base[3] = 0,
	.cqm_bm_policy_dts = { {0} },
	.dqm_port_info = { {0} },
	.eqm_port_info = { {0} },
};

int cqm_config_lgmc_txin_fallback(struct cqm_ctrl *cqm_ctrl)
{
	struct cqm_port_config *port_config = cqm_ctrl->cqm_cfg;

	while (port_config->type != NONE_TYPE) {
		if (port_config->type == DQM_ACA_TYPE) {
			port_config->data.dqm_aca.num_desc = 32;
			port_config->data.dqm_aca.num_free_burst = 32;
			return CBM_SUCCESS;
		}
		port_config++;
	}

	return CBM_FAILURE;
}

int cqm_config_deq_port_to_txdma(struct cqm_ctrl *cqm_ctrl, int port,
				 u8 *txdma_ctrl, u8 *txdma_chan)
{
	struct cqm_port_config *port_config = cqm_ctrl->cqm_cfg;

	while (port_config->type != NONE_TYPE) {
		if (port_config->type == DQM_DMA_TYPE ||
		    port_config->type == DQM_PON_TYPE ||
		    port_config->type == DQM_DOCSIS_TYPE) {
			if (port >= port_config->data.dqm_dma.port &&
			    port <= port_config->data.dqm_dma.port +
			    port_config->data.dqm_dma.port_range) {
				*txdma_ctrl = (u8)port_config->data.dqm_dma.dma_ctrl;
				*txdma_chan = (u8)(port_config->data.dqm_dma.dma_chan +
						   (port - port_config->data.dqm_dma.port));
				return CBM_SUCCESS;
			}
		}
		port_config++;
	}

	return CBM_FAILURE;
}

struct cqm_ctrl *get_cqm_ctrl(int sys_type, enum CQM_SOC_T soc)
{
	int i;

	for (i = 0; i < LGM_CQM_SYS_NUM; i++) {
		if (sys_config[i].type == sys_type &&
		    sys_config[i].cqm_soc == soc) {
			g_lgm_ctrl.cqm_cfg = sys_config[i].cfg;
			g_lgm_ctrl.sys_type = sys_type;
			break;
		}
	}
	return &g_lgm_ctrl;
}
