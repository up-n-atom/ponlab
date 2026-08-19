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

#ifndef __CQM_CONFIG_H__
#define __CQM_CONFIG_H__

#include <linux/pp_buffer_mgr_api.h>
#include <dt-bindings/net/mxl,lgm-cqm.h>

#define CQM_QIDT_DW_NUM	0x800
#define CQM_QIDT_TABLE_NUM	0x2
#define CQM_QID2EP_DW_NUM	0x200
#define CQM_SRAM_SIZE	0x40000
#define CQM_SRAM_FRM_SIZE 128
#define CQM_DESC_MEM_SIZE	0x4000
#define CQM_DESC_MEM_FRM_SIZE 16
/*bits 35:7, on bits 31:3*/
#define CQM_SRAM_BASE	0xecd00000
#define MAX_QOS_QUEUES 512
#define LGM_MAX_PORT_PER_EP 1
#define LGM_MAX_PORT_MAP 2
#define LGM_MAX_PON_PORTS 64
#define LGM_SKB_PTR_SIZE 8
#define LGM_WAN_AON_MODE BIT(0)
#define LGM_WAN_PON_MODE 0
#define LGM_MAX_RESOURCE 16
#define CQM_MAX_INTR_LINE 8
#define LGM_EP_POS 8
#define LGM_FSQMQ_DMAT2_PORT 29
#define LGM_FSQMQ_DMAT1_PORT 45
#define LGM_QBYP_ENQ_PORT 4
#define LGM_DMA_DEQ_BYPASS_PORT 67
#define BM_ADDR_MASK 0xfffffffff
#define BM_CPU_ADDR_MASK 0xfffffff000000000
#define DFLT_DEV_QOS 0x76543210
#define PING_DEV_QOS 0x33221100
#define PONG_DEV_QOS 0x77665544
#define BM_POOL_ALIGN 0x10000
#define LGM_SYS_PON 0x1
#define LGM_SYS_DOCSIS 0x2
#define LGM_SYS_GINT 0x4

enum CQM_POOL_POLICY_SIZES {
	CQM_LGM_FSQM_POOLS = 1,
	CQM_LGM_BM_POOLS = 5,
	CQM_LGM_EXTD_BM_POOLS = 11,
	CQM_LGM_TOTAL_SYS_POOLS = (CQM_LGM_BM_POOLS + CQM_LGM_FSQM_POOLS),
	CQM_LGM_TOTAL_BM_POOLS = (CQM_LGM_BM_POOLS + CQM_LGM_EXTD_BM_POOLS),
	CQM_LGM_TOTAL_POOLS = (CQM_LGM_TOTAL_BM_POOLS + CQM_LGM_FSQM_POOLS),
	CQM_CPU_POOL = 0,
	CQM_FSQM_POOL = 0xff,
	CQM_CPU_POLICY = 0xfe,
	CQM_FSQM_POLICY = 0xff,
	CQM_LGM_NUM_CPU_POLICY = 4,
	CQM_LGM_NUM_SYS_POLICY = 5,
	CQM_LGM_NUM_BM_POLICY = 256,
	CQM_LGM_POOL_POL_HDR_SIZE = 4,
	CQM_CPU_POOL_BUF_ALW_NUM = 0x4000,
	MIN_POOL_SIZE = 64,
};

enum BUF_REQ_SIZES {
	REQ_FSQM_BUF = 0,
	REQ_SIZE0_BUF = 1,
	REQ_SIZE1_BUF = 2,
	REQ_SIZE2_BUF = 3,
	REQ_SIZE3_BUF = 4,
	REQ_EXTEND_BUF = 5,
};

enum DIRECTION {
	INGRESS = CQM_RX,
	EGRESS = CQM_TX,
	DIRECTION_INVALID = 0,
};

enum CQM_SUP_DEVICE {
	SYS = DP_RES_ID_SYS,
	WAV = DP_RES_ID_WAV,
	VOICE = DP_RES_ID_VOICE0,
	DSL = DP_RES_ID_VRX,
	DOCSIS = DP_RES_ID_DOCSIS,
	CPU_DEV = DP_RES_ID_CPU,
	TOE = DP_RES_ID_TOE,
	DPDK = DP_RES_ID_DPDK,
	LROUC = DP_RES_ID_LROUC,
	UNKNOWN,
	MAX_SUP_DEVICE = UNKNOWN,
};

enum CQM_SOC_T {
	URX851B,
	URX851C,
	URX651,
};

enum CQM_LPID_WAN_MODE_BIT {
	CQM_LPID_WAN_BIT_ETH = 0,
	CQM_LPID_WAN_BIT_PON = 1,
	CQM_LPID_WAN_BIT_DOCSIS = 2,
	CQM_LPID_WAN_BIT_DSL = 3,
	CQM_LPID_WAN_BIT_MAX = 4,
};

struct dqm_dma_port {
	u32 port_range;
	u32 port;
	u32 port_type;
	u32 queue;
	u32 queue_nos;
	u32 dma_ctrl;
	u32 dma_chan;
	u32 port_enable;
	u32 num_desc;
	u32 txpush_desc;
};

struct dqm_pon_port {
	u32 port_range;
	u32 port;
	u32 queue;
	u32 queue_nos;
	u32 dma_ctrl;
	u32 dma_chan;
	u32 port_enable;
	u32 num_desc;
	u32 txpush_desc;
};

struct dqm_cpu_port {
	u32 port;
	u32 port_range;
	u32 queue;
	u32 cpu_port_type;
	u32 num_desc;
	u32 txpush_desc;
};

struct dqm_aca_port {
	u32 port;
	u32 port_range;
	u32 queue;
	u32 cpu_port_type;
	u32 num_desc;
	u32 txpush_desc;
	u32 num_free_burst;
};

struct eqm_dma_port {
	u32 port;
	u32 port_range;
	u32 dma_ctrl;
	u32 dma_chnl;
	u32 dma_chnl_type;
	u32 port_type;
	u32 num_desc;
};

struct eqm_cpu_port {
	u32 port;
	u32 port_range;
	u32 port_type;
	u32 num_desc;
	u32 num_alloc_burst;
};

struct cqm_port_config {
	u32 type;
	union {
		struct dqm_dma_port dqm_dma;
		struct dqm_cpu_port dqm_cpu;
		struct dqm_aca_port dqm_aca;
		struct eqm_dma_port eqm_dma;
		struct eqm_cpu_port eqm_cpu;
	} data;
};

struct cqm_sys_config {
	int type;
	enum CQM_SOC_T cqm_soc;
	struct cqm_port_config *cfg;
};

enum CQM_BUF_TYPE {
	BUF_TYPE_CPU_ISOLATED = CQM_CPU_ISOLATED,
	BUF_TYPE_NIOC_ISOLATED = CQM_NIOC_ISOLATED,
	BUF_TYPE_NIOC_SHARED = CQM_NIOC_SHARED,
	BUF_TYPE_SSB = SSB_NIOC_SHARED,
	BUF_TYPE_SSB_LROUC = SSB_LROUC_NIOC_SHARED,
	BUF_TYPE_SSB_LROUC_ISOLATED = SSB_LROUC_NIOC_ISOLATED,
	BUF_TYPE_FSQM,
	BUF_TYPE_INVALID = 0xFF,
};

struct cqm_bm_pool_config {
	unsigned long buf_frm_size;
	unsigned long buf_frm_size_reg;
	unsigned long buf_frm_num;
	unsigned long pool_start_low;
	unsigned long pool_start_high;
	unsigned long pool_end_low;
	unsigned long pool_end_high;
	unsigned long segment_mask;
	unsigned long offset_mask;
	unsigned long pool;
	unsigned long policy;
	enum CQM_BUF_TYPE buf_type;
};

struct cqm_policy_table {
	int type;
	int ingress_policy;
	int egress_policy;
	u32 min_guaranteed;
	u16 pool;
	int egress_cnt; /*egress policy num*/
	int ingress_cnt; /*ingress policy num*/
};

extern struct cqm_bm_pool_config bm_pool_conf[CQM_LGM_TOTAL_POOLS];
extern struct cqm_bm_pool_config fsqm_pool_conf[CQM_LGM_FSQM_POOLS];

struct cqm_ctrl;
int cqm_config_lgmc_txin_fallback(struct cqm_ctrl *cqm_ctrl);
int cqm_config_deq_port_to_txdma(struct cqm_ctrl *cqm_ctrl, int port,
				 u8 *txdma_ctrl, u8 *txdma_chan);
struct cqm_ctrl *get_cqm_ctrl(int sys_type, enum CQM_SOC_T soc);
#endif //__CQM_CONFIG_H__
