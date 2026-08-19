/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2020-2024 MaxLinear, Inc.
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

#ifndef __CQM_H__
#define __CQM_H__
#include <linux/unistd.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/netdevice.h>
#include <linux/dma-mapping.h>
#include <linux/skbuff.h>
#include <linux/if_ether.h>
#include <net/mxl_cbm_api.h>
#include <net/datapath_api.h>
#include <net/datapath_api_qos.h>
#include <linux/dmaengine.h>
#include <linux/debugfs.h>
#ifdef CONFIG_RFS_ACCEL
#include <linux/cpu_rmap.h>
#endif /* CONFIG_RFS_ACCEL */
#include "reg/cbm_ls.h"
#include "reg/cqem.h"
#include "reg/cqem_deq.h"
#include "reg/cqem_enq.h"
#include "reg/cqem_desc64b.h"
#include "reg/cqem_desc128b.h"
#include "reg/fsqm.h"
#include "reg/fsqm_desc.h"
#include "reg/cqem_pon_ip_if.h"
#include "cqm_config.h"
#include "cqm_common.h"
#include "cqm_debugfs.h"
#include "cqm_dev.h"

#define FREE_BUDGET 30
/*#define CBM_DEBUG 1*/
/*#define CBM_DEBUG_LVL_1 1*/
#define FLAG_PIB_ENABLE		BIT(0)
#define FLAG_PIB_PAUSE		BIT(1)
#define FLAG_PIB_OVFLW_INTR	BIT(2)
#define FLAG_PIB_ILLEGAL_INTR	BIT(3)
#define FLAG_PIB_WAKEUP_INTR	BIT(4)
#define FLAG_PIB_BYPASS		BIT(5)
#define FLAG_PIB_DELAY		BIT(6)
#define DMA_PORT_RE_INSERTION	28
#define FLAG_PIB_PKT_LEN_ADJ	BIT(7)
#define FLAG_PIB_DC_MODE	BIT(8)
#define LGM_CQM_DROP_Q		0x0
#define LGM_CQM_Q_MASK		0x1ff
#define LGM_CQM_Q_SHIFT		9
#define LGM_EQM_DMA_NO_BUF 0x1
#define LGM_EQM_DMA_HDR_ONLY 0x2
#define LGM_SIZE0_MASK	((~0x7f)U)
#define LGM_SIZE1_MASK	((~0xff)U)
#define LGM_SIZE2_MASK	((~0x1ff)U)
#define LGM_SIZE3_MASK	((~0x3ff)U)
#define PP_HEADROOM	CQM_FIXED_RX_OFFSET
#define MIN_HEADROOM	64
#define LGM_CQM_ACA_DESC_MAX	32
#define LGM_CQM_VER_2 2
#define SUB_IF_ID_MASK_SHF	16
#define SUB_IF_ID_MODE_0_MASK	0x3F
#define SUB_IF_ID_MODE_1_MASK	0xFF
#define SUB_IF_ID_MODE_2_MASK	0x0F
#define SUB_IF_ID_MODE_3_MASK	0x1F
#define SUB_IF_ID_MODE_4_MASK	0x3F
#define SUB_IF_ID_MODE_5_MASK	0xFF
#define SUB_IF_ID_MODE_6_MASK	0x03
#define SUB_IF_ID_MODE_7_MASK	0x7F
#define SUB_IF_ID_MODE_0_POS	8
#define SUB_IF_ID_MODE_1_POS	0
#define SUB_IF_ID_MODE_2_POS	8
#define SUB_IF_ID_MODE_3_POS	0
#define SUB_IF_ID_MODE_4_POS	0
#define SUB_IF_ID_MODE_5_POS	8
#define SUB_IF_ID_MODE_6_POS	0
#define SUB_IF_ID_MODE_7_POS	8
#define MODE0_SUB_IF_ID_LIMIT	BIT(6)
#define MODE1_SUB_IF_ID_LIMIT	BIT(8)
#define MODE2_SUB_IF_ID_LIMIT	BIT(4)
#define MODE3_SUB_IF_ID_LIMIT	BIT(5)
#define MODE4_SUB_IF_ID_LIMIT	BIT(6)
#define MODE5_SUB_IF_ID_LIMIT	BIT(8)
#define MODE6_SUB_IF_ID_LIMIT	BIT(2)
#define MODE7_SUB_IF_ID_LIMIT	BIT(7)
#define MAX_POLICY_NUM (DP_MAX_POLICY_GPID * 2)
#define MAX_QID_MAP_EP_NUM	16
#define MAX_ATU	8
#define INVALID_DQP	0xffff
#define STATS_GET(atomic) \
	atomic64_read(&(atomic))
#define INVALID_POOL_ID	0xff

/***********************
 * ENUM
 ***********************/

enum {
	EQ_PORT = 0,
	DQ_PORT = 1,
	EQ_DQ_PORT_IGNORE_ALLOC = 2,
};

enum {
	CBM1_INT_VPE0 = 174,
	CBM1_INT_VPE1 = 175,
	CBM1_INT_VPE2 = 176,
	CBM1_INT_VPE3 = 177,
	CBM2_INT_VPE0 = 178,
	CBM2_INT_VPE1 = 179,
	CBM2_INT_VPE2 = 180,
	CBM2_INT_VPE3 = 181,
};

enum {
	DQM_DMA = 1,
	EQM_DMA = 2
};

enum {
	DEQ_DMA_CHNL = 8,
	ENQ_DMA_FSQM_CHNL = 1,
	ENQ_DMA_SIZE0_CHNL = 2,
	ENQ_DMA_SIZE1_CHNL = 3,
	ENQ_DMA_SIZE2_CHNL = 4,
	ENQ_DMA_SIZE3_CHNL = 5,
	ENQ_DMA_HEADER_MODE_CHNL = 6,
	DEQ_DMA_CHNL_NOT_APPL = 0XFF,
};

enum CBM_EGP_TYPE {
	CPU_EG_PORT = 0,
	MPE_EG_PORT,
	WAVE500_PORT,
	LRO_PORT,
	DPDK_EG_PORT = 6,
	GSWIPL_LP_P1,
	GSWIPL_LP_P2,
	GSWIPL_LP_P3,
	GSWIPL_LP_P4,
	GSWIPL_LP_P5,
	GSWIPL_LP_P6,
	GSWIPL_HP_P1,
	GSWIPL_HP_P2,
	GSWIPL_HP_P3,
	GSWIPL_HP_P4,
	GSWIPL_HP_P5,
	GSWIPL_HP_P6,
	/*GSWIPR_VRX318,*/
	GSWIPR_REDIRECT,
	GSWIPR_ETHWAN,
	VRX318_PORT,
	MAX_DQM_PORT,
};

/*! PMAC port flag */
enum FREE_FLAG {
	/*! The port is free */
	P_FREE = 0,
	P_ALLOCATED,
	P_REGISTERED,
	P_SHARED,
	P_FLAG_NO_VALID
};

enum EQM_DQM_PORT_TYPE {
	DQM_CPU_TYPE = 0,
	DQM_ACA_TYPE = 0x4,
	DQM_DMA_TYPE = 0x7,
	DQM_PON_TYPE = 0x26,
	DQM_VUNI_TYPE = 0x27,
	DQM_REINSERT_TYPE = 0x28,
	DQM_PP_NF_TYPE = 0x55,
	DQM_DOCSIS_TYPE = 0x56,
	EQM_CPU_TYPE = 0x100,
	EQM_ACA_TYPE = 0x200,
	EQM_VM_TYPE  = 0x300,
	EQM_DMA_TYPE = 0x400,
	NONE_TYPE = 0xffff
};

enum DQ_PORT_SUB_TYPE {
	DEQ_VM = 0x6,
	DEQ_TOE = 0x7,
	DEQ_VOICE = 0x8,
	DEQ_ACA = 0x9,
	DEQ_FSQM_DESC = 0xa,
	DEQ_DC_DOCSIS = 0xb,
	DEQ_CPU_QOSBP = 0xc,
	DEQ_PP_NF = 0xd,
	DEQ_VUNI = 0xe,
	DEQ_FLUSH = 0xf,
	DEQ_REINSERT = 0x10,
	DEQ_VPN = 0x11,
};

enum EQ_PORT_SUB_TYPE {
	EQM_INVALID,
	EQM_CPU = 0x8,
	EQM_CPU_QBYP = 0x1,
	EQM_VOICE = 0x2,
	EQM_ACA = 0x3,
	EQM_TSO = 0x6,
	EQM_LRO = 0x7,
	EQM_DMA_PON = 0x9,
	EQM_DMA_HEADER = 0xa,
	EQM_DMA_SIZE2 = 0xb,
	EQM_RXDMA = 0xc,
	EQM_DPDK = 0xd,
};

enum DP_DEV_TYPE {
	DP_SPL_CONN_START = 8,
	DP_SPL_CONN_END = 17,
	CQM_TOTAL_DPID = 18,
};

enum CQM_QIDT_STATE {
	STATE_CLASS = 0,
	STATE_EP = 1,
	STATE_MPE1 = 2,
	STATE_MPE2 = 3,
	STATE_ENC = 4,
	STATE_DEC = 5,
	STATE_FLOWID_LOW = 6,
	STATE_FLOWID_HIGH = 7,
	STATE_SUBIF_ID = 8,
	STATE_SUBIF_LSB_DC = 9,
	STATE_COLOR = 10,
	STATE_NONE = 0xffff
};

enum CQM_BUF_SIZES {
	CQM_SIZE0_BUF_SIZE = 0,
	CQM_SIZE1_BUF_SIZE = 1,
	CQM_SIZE2_BUF_SIZE = 2,
	CQM_SIZE3_BUF_SIZE = 3,
	CQM_FSQM_BUF_SIZE = 4,
	CQM_VOICE_POOL = 5,
	CQM_DPDK_POOL1 = 6,
	CQM_DPDK_POOL2 = 7,
	CQM_DPDK_POOL3 = 8,
	CQM_DPDK_POOL4 = 9,
};

enum BM_BUF_SIZES {
	SIZE_256 = 256,
	SIZE_512 = 512,
	SIZE_1024 = 1024,
	SIZE_2048 = 2048,
	/* SIZE_10K represents the maximum pool buffer size.
	 * For some models, e.g. docsis, it is equivalent to the max CPU pool
	 * size, which may be 64K.
	 */
	SIZE_10K = 10240,
};

/**************************************************
 *
 * Macros
 *
 ***************************************************/
/**************************
 *Address Calculation
 **************************/
#define FSQM_LLT_RAM(fsqm_base, idx) \
	(((fsqm_base) + RAM + ((idx) << 2)))

#define FSQM_RCNT(fsqm_base, idx) \
	(((fsqm_base) + RCNT + ((idx) << 2)))

#define EQ_CPU_PORT(idx, reg) \
(CFG_CPU_IGP_0 + ((idx) * 0x1000) + offsetof(struct cqm_eqm_cpu_igp_reg, reg))

#define EQ_DC_PORT(idx, reg) (CFG_DC_IGP_16 + (((idx) - 16) * 0x400) + \
offsetof(struct cqm_eqm_dc_igp_reg, reg))

#define EQ_DMA_PORT(idx, reg) (CFG_DMA_IGP_24 + (((idx) - 24) * 0x400) + \
offsetof(struct cqm_eqm_dma_igp_reg, reg))

#define DQ_CPU_PORT(idx, reg) \
(CFG_CPU_EGP_0 + (((idx) * 0x1000) + offsetof(struct cqm_dqm_cpu_egp_reg, reg)))

#define DQ_VOICE_PORT(idx, reg) \
(CFG_CPU_EGP_0 + ((idx) * 0x1000) + offsetof(struct cqm_dqm_voice_egp_reg, reg))

#define DQ_DC_PORT(idx, reg) (CFG_ACA_EGP_19 + (((idx) - 19) * 0x1000) + \
offsetof(struct cqm_dqm_dc_egp_reg, reg))

#define DQ_DMA_PORT(idx, reg) (CFG_DMA_EGP_27 + (((idx) - 27) * 0x100) + \
offsetof(struct cqm_dqm_dma_egp_reg, reg))

#define DQ_PON_PORT(idx, reg) (CFG_PON_EGP_75 + (((idx) - 75) * 0x400) + \
offsetof(struct cqm_dqm_pon_egp_reg, reg))

#define CQM_EQM_DC_DESC(pid, desc_idx) \
(DESC0_DC_0_IGP_16 + (((pid) - 16) * 0x1000) + (desc_idx) * 16)

#define CQM_EQM_DC_BUF_ALLOC(pid, desc_idx) \
((DESC0_DC_0_IGP_16 + (((pid) - 16) * 0x1000) + (desc_idx) * 8) + 0x400)

#define CQM_DQM_DC_DESC(pid, desc_idx) \
(DESC0_DC_0_EGP_19 + (((pid) - 19) * 0x1000) + (desc_idx) * 16)

#define CQM_DQM_DC_BUF_RTN(pid, desc_idx) \
((DESC0_DC_0_EGP_19 + (((pid) - 19) * 0x1000) + (desc_idx) * 8) + 0x400)

#define CQM_INT_LINE(idx, reg) \
((idx) * 0x40 + offsetof(struct cqm_int_reg, reg))

#define CQM_ENQ_DMA_DESC(pid, des_idx) \
(DESC_DMA_0_IGP_24 + (((pid) - 24) * 0x400) + (des_idx) * 16)

#define CQM_DEQ_DMA_DESC(pid, des_idx) \
(DESC_DMA_0_EGP_27  + (((pid) - 27) * 0x400) + (des_idx) * 16)

#define CQM_DEQ_PON_DMA_DESC(pid, des_idx) \
(DESC_DMA_EGP_PON_0)

/* Port 0 is reserved for CPU. Port 1 and 11 are reserved for GSWIP */
#define DPID_RESERVED (BIT(0) | BIT(1) | BIT(11))

#define DEQ_DLY_CNT_DCP(pid) (DEQ_DLY_CNT_DCP_0 + (pid) * 0x4)
#define DEQ_DLY_CNT_DCP_POMA(pid, field, op) DEQ_DLY_CNT_DCP##_##pid##_##field##_##op

/***********************
 * Default  Value Definition
 **********************/
#define DEFAULT_CPU_HWM         256
#define DEFAULT_DMA_HWM         128
#define CPU_EQM_PORT_NUM        16
#define SCPU_EQM_PORT_NUM       1
#define DMA_EQM_PORT_NUM        10
#define CPU_DQM_PORT_NUM        4
#define SCPU_DQM_PORT_NUM       1
#define DMA_DQM_PORT_NUM        17

#define CBM_QEQCNTR_SIZE        0x400
#define CBM_QDQCNTR_SIZE        0x400

#define CBM_NOTFOUND             2
#define CBM_RESCHEDULE          4
#define CQM_ENQ_PORT_MAX 34
#define CQM_DEQ_PORT_MAX 139
#define NUM_PP_NF_PORTS 116
#define CQM_LGM_TOTAL_DEQ_PORTS (NUM_PP_NF_PORTS + CQM_DEQ_PORT_MAX)
#define CQM_DEQ_BUF_SIZE 64
#define CQM_POOL_METADATA 12
#define BM_MARK_OFFSET 64
#define BM_MARK_SIZE 4
#define PMAC_HDR_SIZE 16
#define BM_METADATA_OFFSET (BM_MARK_OFFSET + BM_MARK_SIZE)
#define CQM_FIXED_RX_OFFSET\
	(BM_METADATA_OFFSET + CBM_LGM_DMA_DATA_OFFSET + \
	 NET_IP_ALIGN + NET_SKB_PAD + CQM_POOL_METADATA)
#define BSL_THRES\
	(CQM_FIXED_RX_OFFSET + SKB_DATA_ALIGN(sizeof(struct skb_shared_info)))
#define QID_MODE_SEL_REG 4
#define QID_MODE_ENTRY_PER_REG 16
#define DMA_CH_STR_LEN 16

#define CQM_MAX_POOL_NUM (CQM_LGM_TOTAL_POOLS + 1)
#define CQM_MAX_POLICY_NUM (CQM_FSQM_POLICY + 1)
#define TOT_DMA_HNDL (CQM_LGM_TOTAL_POOLS + 1)
/*************************
 * Structure definition
 *************************/
#ifdef CONFIG_CPU_BIG_ENDIAN
struct flowid_desc {
	uint8_t row:5;
	uint8_t col:3;
};
#else
struct flowid_desc {
	uint8_t col:3;
	uint8_t row:5;
};
#endif
struct cbm_scpu_ptr_rtn {
	unsigned long ptr_rtn;
};

struct dma_desc {
	/* DWORD 0 */
	u32 dest_sub_if_id:16;
	u32 tunnel_id:8;
	u32 resv0:8;

	/* DWORD 1 */
	u32 classid:4;
	u32 ep:8;
	u32 color:2;
	u32 lro_type:2;
	u32 enc:1;
	u32 dec:1;
	u32 src_pool:4;
	u32 pkt_type:2;
	u32 fcs:1;
	u32 class_en:1;
	u32 pre_l2:2;
	u32 ts:1;
	u32 pmac:1;
	u32 h:1;
	u32 eg_flag:1;

	/*DWORD 2 */
	u32 data_ptr;

	/*DWORD 3*/

	u32 data_len:14;
	u32 policy:8;
	u32 sp:1;
	u32 data_ptr1:4;
	u32 dic:1;
	u32 eop:1;
	u32 sop:1;
	u32 c:1;
	u32 own:1;
};

struct cbm_desc_list {
	struct cbm_desc desc;
	struct cbm_desc_list *next;
};

struct cqm_qidt_mask {
	u32 classid_mask;
	u32 ep_mask;
	u32 mpe1_mask;
	u32 mpe2_mask;
	u32 enc_mask;
	u32 dec_mask;
	u32 flowid_lmask;
	u32 flowid_hmask;
	u32 sub_if_id_mask;
	u32 color_mask;
	u32 egflag_mask;
};

struct cqm_qidt_elm {
	u32 clsid;
	u32 ep;
	u32 mpe1;
	u32 mpe2;
	u32 enc;
	u32 dec;
	u32 flowidl;
	u32 flowidh;
	u32 sub_if_id;
	u32 color;
	u32 egflag;
};

/*Inherits CPU_EQ, VM_EQ*/
struct cqm_eqm_cpu_igp_reg {
	u32 cfg;
	u32 resv0[2];
	u32 eqpc;
	struct cbm_desc disc;
	u32 irncr;  /*Interrupt Capture :  Status + Clear */
	u32 irnicr;
	u32 irnen;  /*Interrupt Enable Mask */
	u32 resv1[3];
	u32 dicc;
	u32 drop;
	u32 npbpc;
	u32 ns0pc;
	u32 ns1pc;
	u32 ns2pc;
	u32 ns3pc;
	u32 resv2[3];
	u32 dcntr;
	u32 resv3[7];
	u32 new_pbptr;
	u32 new_pbptr_1;
	u32 resv4[2];
	u32 new_s0ptr;
	u32 new_s0ptr_1;
	u32 resv5[2];
	u32 new_s1ptr;
	u32 new_s1ptr_1;
	u32 resv6[2];
	u32 new_s2ptr;
	u32 new_s2ptr_1;
	u32 resv7[2];
	u32 new_s3ptr;
	u32 new_s3ptr_1;
	u32 resv8[14];
	struct cbm_desc desc0;
	struct cbm_desc desc1;
};

struct cqm_eqm_dc_igp_reg {
	u32 cfg;
	u32 desc_conv;
	u32 resv1[1];
	u32 eqpc;
	struct cbm_desc disc;
	u32 irncr;
	u32 irnicr;
	u32 irnen;
	u32 resv2[1];
	u32 dptr;
	u32 resv3[2];
	u32 drop;
	u32 npbpc;
	u32 resv4[3];
	u32 devqmap0to7;
	u32 devqmap8to15;
	u32 resv5[2];
	u32 dcntr;
};

struct cqm_eqm_dma_igp_reg {
	u32 cfg;
	u32 resv0[2];
	u32 eqpc;
	struct cbm_desc disc;
	u32 irncr;
	u32 irnicr;
	u32 irnen;
	u32 resv1[1];
	u32 dptr;
	u32 resv2;
	u32 dicc;
	u32 drop;
	struct cbm_desc dropdesc;
	u32 resv3[4];
	u32 dcntr;
};

struct cbm_tx_desc_params {
	unsigned long data_ptr;
	u32 data_len;
	u32 DW0;
	u32 DW1;
	u32 DW3;
	int pool;
	int policy;
};

struct cqm_dqm_cpu_egp_reg {
	u32 cfg;
	u32 dqpc;
	u32 resv0[6];
	u32 irncr;
	u32 irnicr;
	u32 irnen;
	u32 resv1[1];
	u32 dptr;
	u32 bprc;
	u32 brptr;
	u32 resv2[49];
	u32 ptr_rtn_dw2d0;
	u32 ptr_rtn_dw3d0;
	u32 resv3[30];
	u32 ptr_rtn_dw2err;
	u32 ptr_rtn_dw3err;
	u32 resv4[30];
	struct cbm_desc desc[2];
};

struct cqm_dqm_voice_egp_reg {
	u32 cfg;
	u32 dqpc;
	u32 resv0[6];
	u32 irncr;
	u32 irnicr;
	u32 irnen;
	u32 resv1[1];
	u32 dptr;
	u32 bprc;
	u32 brptr;
	u32 resv2[49];
	u32 ptr_rtn_dw2d0;
	u32 ptr_rtn_dw3d0;
	u32 resv3[2];
	u32 ptr_rtn_dw2d1;
	u32 ptr_rtn_dw3d1;
	u32 resv4[2];
	u32 ptr_rtn_dw2d2;
	u32 ptr_rtn_dw3d2;
	u32 resv5[2];
	u32 ptr_rtn_dw2d3;
	u32 ptr_rtn_dw3d3;
	u32 resv6[50];
	struct cbm_desc desc[4];
};

struct cqm_dqm_dc_egp_reg {
	u32 cfg;
	u32 dqpc;
	u32 resv0[6];
	u32 irncr;
	u32 irnicr;
	u32 irnen;
	u32 resv1[1];
	u32 dptr;
	u32 bprc;
	u32 brptr;
	u32 resv2[81];
	u32 ptr_rtn_dw2d0;
	u32 ptr_rtn_dw3d0;
};

struct cqm_dqm_dma_egp_reg {
	u32 cfg;
	u32 dqpc;
	u32 resv0[6];
	u32 irncr;
	u32 irnicr;
	u32 irnen;
	u32 resv1;
	u32 dptr;
};

struct cqm_dqm_pon_egp_reg {
	u32 cfg;
	u32 resv0[3];
	u32 dqpc;
	u32 resv1[3];
	u32 irncr;
	u32 irnicr;
	u32 irnen;
	u32 resv2;
	u32 dptr;
	u32 bprc;
	u32 resv3[18];
	u32 ptr_rtn_dw2d0;
	u32 ptr_rtn_dw3d0;
	u32 resv4[30];
	struct cbm_desc desc[8];
	u32 resv5[32];
	struct cbm_desc segdesc[8];
};

struct cqm_int_reg {
	u32 cbm_irncr;
	u32 cbm_irnicr;
	u32 cbm_irnen;
	u32 res0;
	u32 igp_irncr;
	u32 igp_irnicr;
	u32 igp_irnen;
	u32 res1;
	u32 egp_irncr;
	u32 egp_irnicr;
	u32 egp_irnen;
};

struct cqm_egp_map {
	u32 epg;	/*!< egress port group */
	u32 epg_range;	/*!< egress port group range */
	u32 ipg;	/*!< ingress port group */
	u32 ipg_range;	/*!< ingress port range */
	u32 lpid;	/*!< lpid */
	u32 port_type;	/*!< Overall port type */
	bool valid;	/*!< whether non-dynamic port is valid. */
};

/*! CBM port information*/
struct cqm_dqm_port_info {
	struct dma_chan *pch;
	u32 dma_ch;
	u32 dma_dt_ch;
	u32 dma_dt_ctrl;
	u32 dma_dt_init_type;
	int dma_ch_in_use;
	u32 def_qid;
	u32 def_schd;
	u32 def_tmu_pid;
	void *cbm_buf_free_base;
	u32 num_free_entries;/*!< Number of Free Port entries */
	cbm_dq_info_t	deq_info;
	u32 dq_txpush_num;
	u32 egp_type;
	char dma_chan_str[DMA_CH_STR_LEN];
	u32 cpu_port_type;
	u32 dma_port_type;
	u32 valid;
	int allocated;
	u32 flush_index;
	s32 ref_cnt; /*!< counter of deq port being enabled by dp_enable() */
	struct cqm_policy_table policy_res;
	struct dp_dev_opt_param opt_param;
	u32 tx_ring_size;
	u32 wib_credit_pkt; /*!< credit size for CQM non-DC egress port */
};

struct cqm_eqm_port_info {
	struct dma_chan *pch;
	u32 port_type;
	enum CQM_BUF_TYPE buf_type;
	unsigned long buf_size;
	int num_eq_ports;/*!< Number of Enqueue Ports */
	cbm_eq_info_t eq_info;/*!<  EQ port info */
	u32 dma_dt_ch;
	u32 dma_dt_ctrl;
	u32 dma_dt_init_type;
	char dma_chan_str[DMA_CH_STR_LEN];
	u32 valid;
	int allocated;
	void *in_alloc_paddr;
	void *in_alloc_vaddr;
	s32 ref_cnt; /*!< counter of enq port being enabled by dp_enable() */
	struct cqm_policy_table policy_res;
};

struct cqm_pmac_port_map {
	int flags; /*!< fill port_id status*/
	struct module *owner;
	struct net_device *dev;
	u32 dev_port;
	u32 port_id;
	u32 alloc_flags;
	u64 egp_port_map[LGM_MAX_PORT_MAP];/*bit map to egp port*/
	u32 qid_num;/*queue numbers allocated to that pmac port*/
	u32 qids[16];/*qid array*/
	/* e.g. DP_F_FAST_ETH_LAN/DP_F_FAST_ETH_WAN/DP_F_DIRECT/
	 *High priority/Low priority
	 */
	u32 egp_type;
	u32 out_cqm_deq_port_id[DP_RX_RING_NUM];
	u32 out_enq_port_id[DP_RX_RING_NUM];
	u32 num_rx_ring;
	u32 num_tx_ring;
	struct list_head list;
	u32 bm_policy_res_id;
	u32 dma_chnl_init;
	u32 eqp_en_cnt;
	u32 tx_deq_port_id[DP_TX_RING_NUM];
	u32 link_speed_cap;
};

struct cqm_res_t {
	u32 cqm_deq_port;
};

struct cbm_cntr_mode {
	void __iomem *reg;
	u32 msel_mask;
	u32 msel_pos;
	u32 qen_mask;
	u32 qen_pos;
	void (*reset_counter)(void);
};

struct pib_ctrl {
	u32 pib_en:1;
	u32 pib_pause:1;
	u32 cmd_ovflw_intr_en:1;
	u32 cmd_illegal_port_intr_en:1;
	u32 wakeup_intr_en:1;
	u32 pib_bypass:1;
	u32 pkt_len_adj:1;
	u32 deq_delay:4;
	u32 dc_mode:1;
};

struct cqm_bm_policy_params {
	enum CQM_SUP_DEVICE policy_type;
	enum DIRECTION direction;
	u32 reserved;
	u32 busy;
	s32 res_id;
	u32 res_count;
	u32 res_i_cnt;
	u32 res_e_cnt;
	struct pp_bmgr_policy_params pp_policy_cfg;
};

struct cqm_dpdk_setting {
	int cpu_index;
	u32 deq_mode;
	u32 ring_size;
	u32 deq_port;
	u32 enq_port;
	void *tx_ring_cpu;
	dma_addr_t tx_ring_dma;
};

struct cpu_pkt_tx_info {
	u32 data_pad_n_prel2 :1;
	u32 jumbo_pkt :1;
	u32 clone_f :1;
	u32 cpu_buf :1;
	u32 policy_fe_test :1;
	u32 policy_fe_allowed :1;
	u32 no_tail_room_f :1;
	u32 no_hdr_room_f :1;
	u32 f_qosby :1;
};

enum E_CPU_TX_PATH {
	LINEARISE_PATH = 0,
	CPU_POLICY_FE_PATH = 1,
	NON_LINEARISE_PATH = 2,
	INVALID_PATH = 0xFF,
};

enum E_GEN_POOL_TYPE {
	GEN_RW_POOL = 0,
	GEN_CPU_POOL = 1,
	GEN_SYS_POOL = 2,
};

#define CQM_NAPI_POLL_WEIGHT NAPI_POLL_WEIGHT
struct cqm_napi {
	int line;
	struct napi_struct napi ____cacheline_aligned_in_smp;
	void __iomem *desc;
	int read_idx;
};

struct umt_trig {
	u32 mark_en;
	u32 port_en;
};

struct dp_wav_s {
	u32 dqpd;
	u32 eqpd;
	unsigned long lpid_port; /*!< Bitmap shows lpid port allowed for wav */
	u32 num;
	u32 fsqm_port;
	u32 dcnt; /*!< counter of dqpd being used by dp_port_alloc_complete() */
	u32 ecnt; /*!< counter of eqpd being used by dp_port_alloc_complete() */
	u32 atu_portref;
	u32 atu_refcnt;
};

struct atu_index_s {
	u32 atu_idx[MAX_ATU];
	u32 num_idx;
};

struct dqm_pid_s {
	u32 cpu_start;
	u32 cpu_end;
	u32 vm_start;
	u32 vm_end;
	u32 lro_start;
	u32 lro_end;
	u32 voice_start;
	u32 voice_end;
	u32 aca_start;
	u32 aca_end;
	u32 dma2_start;
	u32 dma0_end;
	u32 qosbp_start;
	u32 qosbp_end;
	u32 pon_start;
	u32 pon_end;
	u32 docsis_dc_dma_start;
	u32 docsis_dc_dma_end;
	u32 docsis_start;
	u32 docsis_end;
	u32 ppnf_start;
	u32 ppnf_end;
	u32 vuni_start;
	u32 vuni_end;
	u32 vpn_start;
	u32 vpn_end;
	u32 reinsert_start;
	u32 reinsert_end;
};

struct eqm_pid_s {
	u32 cpu_start;
	u32 cpu_end;
	u32 vm_start;
	u32 vm_end;
	u32 voice_start;
	u32 voice_end;
	u32 qosbp_start;
	u32 qosbp_end;
	u32 aca_start;
	u32 aca_end;
	u32 dma0_start;
	u32 dma0_end;
	u32 tso;
	u32 lro_start;
	u32 lro_end;
	u32 rxdma;
};

struct cqm_soc_data {
	int sys_type;
	enum CQM_SOC_T cqm_soc;
};

struct cqm_lpid_epg_map_s {
	u32 lpid;
	u32 epg;
	u8 txdma_ctrl;
	u8 txdma_chan;
};

struct cqm_lpid_config_s {
	unsigned long cqm_lpid_port_mode;
	u8 cqm_lpid_wan_mode;
	struct cqm_lpid_epg_map_s lpid_epg_map[LPID_COUNT];
};

struct cqm_ctrl {
	int id;
	struct device *dev;
	struct net_device dummy_dev;
	struct net_device dummy_dev_dp;
	struct cqm_napi cqm_napi[CQM_MAX_INTR_LINE];
	struct cqm_napi cqm_napi_dp[CQM_MAX_INTR_LINE];
	int num_dqm_ports;
	int num_eqm_ports;
	int max_queues;
	int total_pool_entries;
	int num_sys_pools;
	int num_dt_entries;
	int num_intrs;
	int sys_type;
	enum CQM_SOC_T soc;
	bool txin_fallback;
	bool cpu_enq_chk;
	struct cqm_port_config *cqm_cfg;
	struct clk *cbm_clk;
	unsigned long cbm_irq[CQM_MAX_INTR_LINE];
	unsigned long cbm_line[CQM_MAX_INTR_LINE];
	unsigned long cbm_line_map[CQM_MAX_INTR_LINE];
	unsigned long cpu_port_alloc;
	struct tasklet_struct cqm_tasklet[CQM_MAX_INTR_LINE];
	unsigned long *cpu_rtn_ptr;
	dma_addr_t cpu_rtn_phy;
#ifdef ENABLE_LL_DEBUG
	struct tasklet_struct cqm_debug_tasklet;
#endif
	const struct cbm_ops *cqm_ops;
	void *cqm_qmgr_buf_base;
	unsigned long bm_buf_base[CQM_LGM_TOTAL_BM_POOLS];
	unsigned long bm_buf_phy[CQM_LGM_TOTAL_BM_POOLS];
	struct regmap *syscfg;
	int force_xpcs;
	void __iomem *txpush;
	void __iomem *txpush_phys;
	void __iomem *dmadesc_128;
	void __iomem *dmadesc_128_phys;
	void __iomem *dmadesc_64;
	void __iomem *dmadesc_64_phys;
	void __iomem *cqm;
	void __iomem *cqm_phys;
	void __iomem *qidt0;
	void __iomem *qidt0_phys;
	void __iomem *qidt1;
	void __iomem *qidt1_phys;
	void __iomem *ls;
	void __iomem *ls_phys;
	void __iomem *enq;
	void __iomem *enq_phys;
	void __iomem *deq;
	void __iomem *deq_phys;
	void __iomem *qid2ep;
	void __iomem *qid2ep_phys;
	void __iomem *fsqm;
	void __iomem *fsqm_phys;
	void __iomem *fsqm_desc;
	void __iomem *fsqm_desc_phys;
	void __iomem *pib;
	void __iomem *pib_phys;
	u32 max_mem_alloc_sys;
	u32 max_mem_alloc_cpu;
	u32 max_mem_alloc;
	u8 cpu_base_pool;
	u8 cpu_base_policy;
	dma_addr_t dma_hndl_qos;
	dma_addr_t dma_hndl_p[TOT_DMA_HNDL];
	struct dentry *debugfs;
	const char *name;
	unsigned long lgm_pool_ptrs[CQM_LGM_TOTAL_BM_POOLS];
	unsigned long lgm_pool_size[CQM_LGM_TOTAL_BM_POOLS];
	unsigned long lgm_pool_type[CQM_LGM_TOTAL_BM_POOLS];
	struct cqm_bm_policy_params cqm_bm_policy_dts[CQM_LGM_NUM_BM_POLICY];
	struct cqm_dpdk_setting cqm_dpdk[DPDK_MAX_CPU];
	u32 cpu_owner[CQM_MAX_CPU];
	s16 dpdk_enq_policy_start;
	s16 dpdk_enq_policy_num;
	s16 dpdk_deq_policy_start;
	s16 dpdk_deq_policy_num;
	struct cqm_dqm_port_info dqm_port_info[CQM_LGM_TOTAL_DEQ_PORTS];
	struct cqm_eqm_port_info eqm_port_info[CQM_ENQ_PORT_MAX];
	struct atu_index_s atu_index[CQM_TOTAL_DPID];
	unsigned long flush_port;
	int re_insertion;
	int v2_flag;
	int is_cpu_pool_ioc;
	struct dp_wav_s wav;
	struct dqm_pid_s dqm_pid_cfg;
	struct eqm_pid_s eqm_pid_cfg;
	int sai[MAX_CQM_FW_DOMAIN];
	struct cbm_cpu_port_data cpu_port_data;
#ifdef CONFIG_RFS_ACCEL
	struct cpu_rmap     *rx_cpu_rmap;
#endif /* CONFIG_RFS_ACCEL */
	struct cqm_lpid_config_s lpid_config;
	struct deq_dma_delay_s deq_dma_delay[DLY_PORT_COUNT];
	u32 bm_buf_sizes[BM_BUF_SIZE_MAX];
	size_t fsqm_sz;
	u32 version;
};

struct meta_pool_s {
	u32 meta_a[MAX_POLICY_NUM];
	u32 meta_cnt;
	u32 buff_cnt;
};

struct hr_tr_tbl {
	u32 headroom;
	u32 tailroom;
	char *name;
};

struct fe_policy {
	atomic_t enq_cnt;
	u32 allow_cnt;
};

struct cqm_cpubuff_pool {
	dma_addr_t start;
	int size;
	atomic_t *ref;
	unsigned long *ref_chk;
};

struct cqm_cpubuff_regs {
	void __iomem *ret;
};

struct cqm_cpubuff {
	struct cqm_cpubuff_pool pool[CQM_LGM_TOTAL_POOLS];
	struct cqm_cpubuff_regs regs[NR_CPUS];
	struct device *dev;
	int policy_base;
	int pool_base;
	int size;
#if IS_ENABLED(CONFIG_LGM_CQM_CPUBUF_RET_DC)
	u32 num_desc;
	u32 brptr_num;
	void *dc_free_base;
	int mcpy_pid;
	spinlock_t dcfree_lock;
#endif
};

struct cqm_buf_dbg_cnt {
	atomic64_t rx_cnt;
	atomic64_t tx_cnt;
	atomic64_t free_cnt;
	atomic64_t free_lro_err_cnt;
	atomic64_t free_frag_cnt;
	atomic64_t alloc_cnt;
	atomic64_t isr_free_cnt;
	atomic64_t dma_ring_buff_cnt;
};

extern struct
cqm_buf_dbg_cnt cqm_dbg_cntrs[CQM_MAX_POLICY_NUM][CQM_MAX_POOL_NUM];
/************************
 * Function prototypes
 ************************/
void cqm_read_dma_desc_prep(int port, void **base_addr, int *num_desc);
void cqm_read_dma_desc_end(int port);
int get_fsqm_ofsc(void);
void *cqm_get_enq_base(void);
void *cqm_get_deq_base(void);
void *cqm_get_dma_desc_base(void);
void *cqm_get_ctrl_base(void);
void *cqm_get_ls_base(void);
void *cqm_get_enq_phy_base(void);
void *cqm_get_deq_phy_base(void);
void *cqm_get_ctrl_phy_base(void);
void *cqm_get_ls_phy_base(void);
void *cqm_get_dmadesc_64(void);
int cqm_debugfs_init(struct cqm_ctrl *pctrl);
struct cqm_dqm_port_info *cqm_get_dqp_info_p(u32 port_id);
struct cqm_eqm_port_info *cqm_get_eqp_info_p(u32 port_id);
struct hr_tr_tbl *get_hr_tr_tbl(u32 spl_flag);
struct eqm_pid_s *cqm_get_eqm_pid_cfg(void);
struct dqm_pid_s *cqm_get_dqm_pid_cfg(void);
struct cqm_lpid_config_s *cqm_get_lpid_config(void);
struct deq_dma_delay_s *cqm_get_deq_dma_delay(void);
int cqm_update_deq_dma_delay(int idx);
#endif
