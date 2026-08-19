// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2025 MaxLinear, Inc.
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

#include <linux/bitfield.h>
#include <linux/bitmap.h>
#include <linux/types.h>
#include <linux/dmaengine.h>
#include <linux/genalloc.h>
#include <linux/percpu.h>
#include <linux/ethtool.h>
#include <linux/log2.h>
#include <soc/mxl/mxl_sec.h>
#include <soc/mxl/mxl_gen_pool.h>
#include <dt-bindings/net/mxl,lgm-cqm.h>
#if IS_ENABLED(CONFIG_LGM_CQM_CPUBUF_RET_DC)
#include <linux/dma/intel_mcpy.h>
#include <linux/circ_buf.h>
#endif

/*header file to be included for 32 bit backward compatible
 * for 32 bit processor access order lo -> high
 */
#include <linux/io-64-nonatomic-lo-hi.h>
#include "cqm.h"
#include "cqm_debugfs.h"
#include "cqm_config.h"
#include <net/ip6_checksum.h>

#define CQM_KB(x) ((x) * 1024UL)
#define OWN_BIT  BIT(31)
#define COMPLETE_BIT  BIT(30)
#define CQM_PON_IP_PKT_LEN_ADJ_BYTES 17
#define CQEM_PON_IP_IF_PIB_OVERSHOOT_BYTES 9260
#define LGM_CQM_DROP_INIT ((LGM_CQM_DROP_Q << 16) | \
			   LGM_CQM_DROP_Q)
#define IS_CPU_PORT_TYPE(TYPE)((TYPE == DP_F_DEQ_CPU) ||\
			   (TYPE == DP_F_DEQ_CPU1) ||\
			   (TYPE == DP_F_DEQ_MPE) ||\
			   (TYPE == DP_F_DEQ_DL))
#define CQM_PON_IP_BASE_ADDR_L 0xE75003FC
#define CQM_PON_IP_BASE_ADDR_H 0
/* CQM_TX_HEADROOM can be 64 ~ PP_HEADROOM */
#define CQM_TX_HEADROOM PP_HEADROOM
#define CQM_JUMBO_PKT_SIZE 2048
#define RTN_PTRS_IN_CACHELINE (L1_CACHE_BYTES / LGM_SKB_PTR_SIZE)
#define BYTES_IN_SEL0_3	4
#define BYTES_IN_SEL4	1
#define LAN_10G_DP_PORT 4
#define IOC_2K_POLICY_BUF_SIZE 2048
DEFINE_PER_CPU(struct cbm_desc, cqm_enq_desc);
DEFINE_PER_CPU(struct cbm_desc, cqm_deq_desc);

static struct cqm_cpubuff *cpubuff;
static unsigned long CPUBUFF_SZ[CQM_LGM_TOTAL_POOLS];
static u8 CPUBUFF_SHF[CQM_LGM_TOTAL_POOLS];
static struct cqm_buf_dbg_cnt (*cqm_dbg_ptr)[CQM_MAX_POLICY_NUM][CQM_MAX_POOL_NUM];
struct cqm_buf_dbg_cnt cqm_dbg_cntrs[CQM_MAX_POLICY_NUM][CQM_MAX_POOL_NUM] = {0};
/*HW design accepts only ALLOW_CNT - 1 packets to be enqueued*/
static struct fe_policy cqm_fe_policy = {
	.enq_cnt = {0},
	.allow_cnt = CQM_CPU_POOL_BUF_ALW_NUM - 1,
};

#define CQM_CPU_VM_PORT_OFFSET (cqm_ctrl->dqm_pid_cfg.vm_start - cqm_ctrl->dqm_pid_cfg.cpu_start)
#define CPU_PORT_DESC3_OFFSET 0xC
#define CPU_PORT_DESC_OFFSET 0x10

static const char cqm_name[] = "cqm";
static int bufreq_bm_pool_conf_lut[BYTES_IN_SEL0_3 + BYTES_IN_SEL4];
static void __iomem *bufreq[CQM_LGM_TOTAL_SYS_POOLS];
static void __iomem *bufreq_1[CQM_LGM_TOTAL_SYS_POOLS];
static void __iomem *eqmdesc[CPU_EQM_PORT_NUM];
static struct cbm_cntr_mode cbm_cntr_func[2];
static const char * const pmac_list_flag_str[] = {"Free",
						  "Allocated",
						  "Registered",
						  "Shared",
						  "Flag Not Valid"};

/*Maintain a shadow queue lookup table for fast
 *processing of queue map get API
 */
static struct cbm_qidt_shadow
		 g_cbm_qidt_mirror[CQM_QIDT_TABLE_NUM][CQM_QIDT_DW_NUM];
static u32 g_qidt_help[CQM_QIDT_TABLE_NUM][0x2000];
static spinlock_t cqm_qidt_lock[CQM_QIDT_TABLE_NUM];
static spinlock_t cqm_port_map;
static spinlock_t cpu_pool_enq;

#if IS_ENABLED(CONFIG_LGM_CQM_CPUBUF_RET_DC)
#define CPUBUF_RING_SIZE  (sizeof(u64) * 64)
struct dc_return_ring {
	u64 dc_desc[CPUBUF_RING_SIZE];
	phys_addr_t dc_paddr;
	u16 head;
	u16 tail;
};

DEFINE_PER_CPU(struct dc_return_ring, dc_ret_ring);
#endif

#define CQM_PON_IP_PORT_START (cqm_ctrl->sys_type == LGM_SYS_DOCSIS ? \
			       cqm_ctrl->dqm_pid_cfg.docsis_start : \
			       cqm_ctrl->dqm_pid_cfg.pon_start)
#define CQM_PON_IP_PORT_END (cqm_ctrl->sys_type == LGM_SYS_DOCSIS ? \
			     cqm_ctrl->dqm_pid_cfg.docsis_end : \
			     cqm_ctrl->dqm_pid_cfg.pon_end)
/* Below 2 macros defines the last ENQ DC port and DEQ DC port. They are reserved
 * for DOCSIS on docsis system. The two macros are used to allocate first DC port
 */
#define CQM_ENQ_DC_PORT_DOCSIS (cqm_ctrl->sys_type == LGM_SYS_DOCSIS ? \
				cqm_ctrl->eqm_pid_cfg.aca_end : -1)
#define CQM_DEQ_DC_PORT_DOCSIS (cqm_ctrl->sys_type == LGM_SYS_DOCSIS ? \
				cqm_ctrl->dqm_pid_cfg.aca_end : -1)

LIST_HEAD(pmac_mapping_list);
static struct cqm_ctrl *cqm_ctrl;
static s32 cqm_dma_port_enable(s32 cqm_port_id, u32 flags, unsigned long size,
			       enum CQM_BUF_TYPE buf_type);
static s32 cqm_dq_dma_chan_init(s32 cqm_port_id, u32 flags);
static void init_cqm_deq_cpu_port(int idx, u32);
static s32 cqm_dequeue_dma_port_uninit(s32 cqm_port_id, u32 flags);
static s32 cqm_pp_policy_setup(u32, u32, u32);
static void init_cqm_enq_cpu_port(u32, u32);
static void init_qos_bypass_dma_deq(const u32);
static s32 get_cqmport(u32 *, u32, u32, u32);
static s32 handle_dma_chnl_init(s32, u32);
static void cqm_vm_pool_check_enable(const u32, const u16, const u16);
static u8 get_pool_from_policy(const s16 id);
static s16 get_policy_from_type(enum CQM_SUP_DEVICE type, enum DIRECTION dir);
static u32 get_policy_base_and_num(enum CQM_SUP_DEVICE type, enum DIRECTION dir,
				   s32 dp_port, int alloc_flags,
				   int *tx_policy_base, int *tx_policy_num);
static u32 get_min_guar_from_policy(const s16);
static u8 get_matching_pool(enum CQM_SUP_DEVICE type, enum DIRECTION dir);
static s32 cqm_enqueue_dma_port_init(s32 cqm_port_id, s32 dma_hw_num,
				     u32 chan_num, u32 flags);
static void fill_dp_alloc_data(struct cbm_dp_alloc_data *data, int dp,
			       int port, u32 dp_flags);
static int cqm_poll(struct napi_struct *napi, int budget);
static int cqm_poll_dp(struct napi_struct *napi, int budget);
static s32 get_policy_pos(enum CQM_SUP_DEVICE type, const u32 dir,
			  u16 *start, u16 *end, const u32 res_id, u32 flag);
static u32 get_bm_policy_from_cfg_reg(u32 enqport);
static void cqm_deq_port_flush(int port);
static void cqm_deq_pon_port_disable(u32 port, u32 alloc_flags);
static void dc_buff_free(u32 dc_dq_pid, u64 *dc_buf_ptr, u32 idx);
static unsigned long
cqm_gen_pool_alloc(u32 ioc_f, size_t size, dma_addr_t *phy,
		   enum E_GEN_POOL_TYPE ptype, int sai);
static void cqm_dc_enqport_disable(u32 enq);
static void cqm_dc_deqport_disable(u32 deq);
static int cpu_pool_free(void);
static int cqm_buff_free_real(void *va, u8 pool, u8 policy);
__attribute__((unused)) static s32 pon_deq_cntr_get(int port, u32 *count);
static inline int cqm_get_policy(void *va, u8 pool, u8 *policy);
#define FLAG_WAN (DP_F_FAST_ETH_WAN | DP_F_GPON | DP_F_EPON)
#define FLAG_LAN (DP_F_FAST_ETH_LAN | DP_F_GINT)
#define FLAG_WLAN (DP_F_FAST_WLAN | DP_F_FAST_WLAN_EXT)
#define FLAG_ACA (FLAG_WLAN | DP_F_FAST_DSL)
#define FLAG_DOCSIS (DP_F_ACA | DP_F_DOCSIS)
#define FLAG_DIRECTPATH (DP_F_DIRECT)
#define FLAG_LSB_DONT_CARE 0x80000000
#define PORT_LAN 35
#define PORT_LAN1 51
#define PORT_WAN 75
/*TODO: change as per DP lib later*/
#define DP_F_SPL_PATH 100
#define DP_F_PP_NF 101
#define Q_FLUSH_NO_PACKET_CNT 5
#define BSL_THRESHOLD_0 192
#define BSL_THRESHOLD_1 1728
#define CQM_LPID_BITMAP_LEN (sizeof(u32) * BITS_PER_BYTE)

#define CQM_DEFAULT_DC_ENQ_PORT_CFG	(CFG_DC_IGP_16_EQREQ_MASK | \
					 CFG_DC_IGP_16_BUFREQ0_MASK | \
					 CFG_DC_IGP_16_EQPCEN_MASK)

#define CQM_DEFAULT_DC_DEQ_PORT_CFG	(CFG_ACA_EGP_19_DQREQ_MASK | \
					 CFG_ACA_EGP_19_BUFRTN_MASK | \
					 CFG_ACA_EGP_19_BPRTNDETEN_MASK | \
					 CFG_ACA_EGP_19_DQPCEN_MASK)
#define CQM_DC_DESC_DW_POOL_MASK	GENMASK(27, 24)
#define CQM_DC_DESC_DW_POLICY_MASK	GENMASK(23, 16)
#define CQM_DC_DESC_DW_BUFH_MASK	GENMASK(3, 0)
#define CQM_DC_DESC_DW_BUFL_MASK	GENMASK(31, 0)
#define CQM_DC_DESC_QW_POOL_MASK	GENMASK_ULL(59, 56)
#define CQM_DC_DESC_QW_POLICY_MASK	GENMASK_ULL(55, 48)
#define CQM_DC_DESC_QW_BUFH_MASK	GENMASK_ULL(35, 32)
#define CQM_DC_DESC_QW_BUFL_MASK	GENMASK_ULL(31, 0)

/* Port 12-15 must always be dynamic port */
#define LPID_LPID_DYN_MINIMUM 0xF000

/* Entries with valid field as false are dynamic ports.
 * This table will be updated during CQM probe based on DT property
 *
 * egp, egp_rage, igp and igp_range for dynamic ports will be updated
 * based on cqm port config during CQM driver probe.
 *
 * If lpid_wan_mode is set to DOCSIS, CQM ENQ DC port 7 is reserved
 * for DOCSIS by design.
 */
static struct cqm_egp_map epg_lookup_table[] = {
	{0,		0, 0, 0, 0, 0},
	/* !< lan and wan deq, pmac port range */
	{PORT_WAN,	0, 2, 0, 2, FLAG_WAN | FLAG_LAN, true},
	{PORT_LAN,	0, 3, 0, 3, FLAG_WAN | FLAG_LAN, true},
	{PORT_LAN1,	0, 4, 0, 4, FLAG_WAN | FLAG_LAN, true},
	{PORT_LAN + 1,	0, 5, 0, 5, FLAG_WAN | FLAG_LAN, true},
	{PORT_LAN + 2,	0, 6, 0, 6, FLAG_WAN | FLAG_LAN, true},
	{PORT_LAN1 + 1,	0, 7, 0, 7, FLAG_WAN | FLAG_LAN, true},
	{PORT_LAN1 + 2,	0, 8, 0, 8, FLAG_WAN | FLAG_LAN, true},
	{PORT_LAN + 3,	0, 9, 0, 9, FLAG_WAN | FLAG_LAN, true},
	{PORT_LAN1 + 3,	0, 10, 0, 10, FLAG_WAN | FLAG_LAN, true},
	/* !< wav and docsis deq, enq, pmac port range */
	{0, 7, 0, 7, CBM_PMAC_DYNAMIC, FLAG_ACA, true},
	/* vUNI deq, pmac range */
	{0, 0, 0, 0, CBM_PMAC_DYNAMIC, DP_F_VUNI, true},
	/* !< DOCSIS */
	{0, 7, 0, 7, CBM_PMAC_DYNAMIC, FLAG_DOCSIS, true},
	/* !< directpath */
	{0, 7, 0, 7, CBM_PMAC_DYNAMIC, FLAG_DIRECTPATH, true},
};

static const u32 buf_size_table[] = {
	128, 256, 512, 1024, 2048, 4096, 8192, 10240, 65536
};

struct hr_tr_tbl hr_tr_spl[DP_SPL_MAX] = {
	[DP_NON_SPL] = {0, 0, "NON_SPL"},
	[DP_SPL_TOE] = {192, 320, "SPL_TOE"},
	[DP_SPL_VOICE] = {64, 0, "SPL_VOICE"},
	[DP_SPL_VPNA] =	{128, 64, "SPL_VPN"},
	[DP_SPL_APP_LITEPATH] =	{192, 320, "SPL_APP_LITEPATH"},
	[DP_SPL_PP_NF] = {192, 320, "SPL_PP_NF"},
	[DP_SPL_PP_DUT] = {192, 320, "SPL_PP_DUT"},
};

struct hr_tr_tbl hr_tr_dp[MAX_SUP_DEVICE] = {
	[SYS] =	{64, 4, "Streaming Port"},
	[WAV] =	{128, 4, "WAV"},
	/* Voice HR/TR is taken from hr_tr_spl.
	 * This entry is just to maintain the index
	 */
	[VOICE] = {0, 0, "VOICE"},
	[DSL] =	{128, 4, "DSL"},
	[DOCSIS] = {128, 4, "DOCSIS"},
	[CPU_DEV] = {192, 320, "CPU_DEV"},
	/* TOE HR/TR is taken from hr_tr_spl.
	 * This entry is just to maintain the index
	 */
	[TOE] =	{0, 0, "TOE"},
	[DPDK] = {192, 320, "DPDK"},
};

#if IS_ENABLED(CONFIG_KGDB)
/* Optimization level need to be 1 as 0 will cause asm_inline macro expansion
 * error for pr_debug, WARN_ON, etc, resulting in compilation error.
 */
#define NO_OPTIMIZE __attribute__((optimize("O1")))
#else
#define NO_OPTIMIZE
#endif

static inline int cqm_desc_data_len(u32 dw)
{
	return dw & 0x00003FFF;
}

static inline int cqm_desc_data_len_e(u32 dw)
{
	return dw >> 16;
}

static inline int cqm_desc_data_off(u32 dw)
{
	return (dw & 0x7);
}

static inline int cqm_desc_data_pool(u32 dw)
{
	return (dw & 0x3c0000) >> 18;
}

static inline int cqm_desc_data_policy(u32 dw)
{
	return (dw & 0x3fc000) >> 14;
}

static inline int cqm_desc_data_port(u32 dw)
{
	return (dw & 0xff0) >> 4;
}

static inline int cqm_desc_data_egflag(u32 dw)
{
	return (dw & 0x80000000) >> 31;
}

enum CQM_SUP_DEVICE get_type_from_alloc_flags(u32 flags)
{
	if (flags & FLAG_WLAN)
		return WAV;
	else if (flags & DP_F_FAST_DSL)
		return DSL;
	else if (flags & DP_F_DOCSIS)
		return DOCSIS;
	else if ((flags & DP_F_CPU) || (flags & DP_F_DIRECT))
		return CPU_DEV;
	else if (is_stream_port(flags))
		return SYS;
	else
		return UNKNOWN;
}

static inline void clear_bits(int offset, int len, void *p)
{
	for (; len > 0; len--)
		clear_bit(offset + (len - 1), p);
}

static inline bool is_version2(void)
{
	/* Always assume version 2
	 * CQM will not provide backward
	 * compatiblility for A-step
	 */
	return true;
}

static int is_lgmc_soc(void)
{
	if (cqm_ctrl->soc == URX851C)
		return 1;
	else
		return 0;
}

static int is_txin_64(void)
{
	if (cqm_ctrl->soc == URX851C && !cqm_ctrl->txin_fallback)
		return 1;
	else
		return 0;
}

static inline bool is_cpu_iso_pool(int pool)
{
	return ((pool >= cqm_ctrl->cpu_base_pool) &&
		(pool <= (cqm_ctrl->cpu_base_pool + 3)));
}

int cqm_get_version(void)
{
	u32 ver = cqm_ctrl->version;
	if ((ver & CBM_VERSION_REG_MAJOR_MASK) == 0x3) {
		if (((ver & CBM_VERSION_REG_MINOR_MASK) >> CBM_VERSION_REG_MINOR_POS) == 0x2)
			return CQM_LGM_B;
		if (((ver & CBM_VERSION_REG_MINOR_MASK) >> CBM_VERSION_REG_MINOR_POS) == 0x3)
			return CQM_LGM_C;
	}
	return CQM_UNKNOWN;
}

static int get_policy_stats(u16 policy_id, int type, u32 *alloc_buff)
{
	struct pp_bmgr_policy_stats pp_stats;

	if (pp_bmgr_policy_stats_get(&pp_stats,
				     policy_id)) {
		dev_err(cqm_ctrl->dev,
			"Err in getting policy stats for %d\n",
			policy_id);
		return CBM_FAILURE;
	}

	dev_info(cqm_ctrl->dev, "%s policy %d, pop %d null cntrs %d\n",
		 (type ? "Tx" : "Rx"),
		 policy_id,
		 pp_stats.policy_alloc_buff,
		 pp_stats.policy_null_ctr);
	*alloc_buff = pp_stats.policy_alloc_buff;
	return CBM_SUCCESS;
}

/* Based on intended BM buffer size, get the corresponding bit value in policy map.
 * The bit value is based on two sources:
 * 1. Bit value of buffer size in array buffer_pool_size, which is defined in
 *    device tree and shared between CQM and BM.
 * 2. Whether the intended buffer size is defined in system/cpu pool. If
 *    system/cpu pool does not include the buffer size, 0 wll be returned.
 *
 * For example, if buffer_pool_size = {256, 512, 1024, 2048, 10240}, buffer
 * size 1024 will return BIT(2) and buffer size 2048 will return BIT(3), if
 * system/cpu pool has pools with these sizes.
 * If system/cpu pool does not have pools with these buffer sizes, 0 will be
 * returned.
 */
static u32 _policymap_bit(enum BM_BUF_SIZES bm_buf_size, enum E_GEN_POOL_TYPE type)
{
	u32 policy_map = 0;
	int idx;

	for (idx = 0; idx < BM_BUF_SIZE_MAX; idx++) {
		if (bm_buf_size == cqm_ctrl->bm_buf_sizes[idx]) {
			int pidx;
			if (type == GEN_SYS_POOL) {
				for (pidx = 0; pidx < cqm_ctrl->num_sys_pools; pidx++) {
					if (cqm_ctrl->lgm_pool_size[pidx] == bm_buf_size) {
						policy_map |= BIT(idx);
						break;
					}
				}
			} else if (type == GEN_CPU_POOL) {
				for (pidx = cqm_ctrl->cpu_base_pool;
				     pidx < cqm_ctrl->cpu_base_pool + 4; pidx++) {
					if (cqm_ctrl->lgm_pool_size[pidx] == bm_buf_size ||
					    (bm_buf_size == SIZE_10K &&
					     cqm_ctrl->lgm_pool_size[pidx] >= SIZE_10K)) {
						policy_map |= BIT(idx);
						break;
					}
				}
			}
			break;
		}
	}

	return policy_map;
}

static s32
NO_OPTIMIZE get_policymap_from_syspool(int inst, int policy_base,
				       int policy_range,
				       u32 alloc_flags, int flags)
{
	u32 policy_map = 0;
	u32 pool = 0;
	u32 i, j;

	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s: policy_base %d, policy_range %d, alloc_flags 0x%x, flags 0x%x\n",
		  __func__, policy_base, policy_range, alloc_flags, flags);

	/* Special GPID and Memory Port policy is 0x1E */
	if (flags & SP_GPID_POLICYMAP &&
	    (!is_stream_port(alloc_flags) || !(alloc_flags & DP_F_VUNI))) {
		policy_map = _policymap_bit(SIZE_512, GEN_SYS_POOL) |
			     _policymap_bit(SIZE_1024, GEN_SYS_POOL) |
			     _policymap_bit(SIZE_2048, GEN_SYS_POOL)|
			     _policymap_bit(SIZE_10K, GEN_SYS_POOL);
		CQM_DEBUG(CQM_DBG_FLAG_API,
			  "%s: SP GPID policy_map 0x%x\n", __func__, policy_map);
		return policy_map;
	}

	if ((alloc_flags & DP_F_CPU) || (alloc_flags & DP_F_DIRECT)) {
		policy_map = _policymap_bit(SIZE_512, GEN_CPU_POOL) |
			     _policymap_bit(SIZE_1024, GEN_CPU_POOL) |
			     _policymap_bit(SIZE_2048, GEN_CPU_POOL )|
			     _policymap_bit(SIZE_10K, GEN_CPU_POOL);
		CQM_DEBUG(CQM_DBG_FLAG_API,
			  "%s: CPU policy_map 0x%x\n", __func__, policy_map);
		return policy_map;
	}

	if (policy_range > 0) {
		/* For all other cases policy_map is from system pool */
		for (i = policy_base; i < policy_base + policy_range; i++) {
			pool = get_pool_from_policy(i);
			if (pool == INVALID_POOL_ID) {
				dev_warn(cqm_ctrl->dev, "%s: invalid poolid for policy %d\n",
					 __func__, i);
				continue;
			}
			for (j = 0; j < BM_BUF_SIZE_MAX; j++) {
				if (cqm_ctrl->bm_buf_sizes[j] ==
				    bm_pool_conf[pool].buf_frm_size) {
					policy_map |= BIT(j);
					break;
				}
			}
		}
	} else {
		/* RX policy Map for streaming port and vUNI is 0
		 * TX policy Map for streaming port and vUNI is 0x1E
		 */
		if (is_stream_port(alloc_flags) || (alloc_flags & DP_F_VUNI)) {
			if (flags & RX_POLICYMAP) {
				policy_map = 0;
				CQM_DEBUG(CQM_DBG_FLAG_API,
					  "%s: RX policy_map 0x%x\n", __func__, policy_map);
				return policy_map;
			} else {
				policy_map = _policymap_bit(SIZE_512, GEN_SYS_POOL) |
					     _policymap_bit(SIZE_1024, GEN_SYS_POOL) |
					     _policymap_bit(SIZE_2048, GEN_SYS_POOL )|
					     _policymap_bit(SIZE_10K, GEN_SYS_POOL);
				CQM_DEBUG(CQM_DBG_FLAG_API,
					  "%s: TX policy_map 0x%x\n", __func__, policy_map);
				return policy_map;
			}
		}
	}

	/* Tx Policy Map cannot be 0 */
	if ((flags & TX_POLICYMAP) && !policy_map)
		pr_err("wrong tx policy_map %d for alloc_flag=%x\n",
		       policy_map, alloc_flags);

	/* if alloc_flags with ACA, then policy_map cannot be 0 */
	if ((alloc_flags & DP_F_ACA) && !policy_map)
		pr_err("wrong policy_map %d for ACA alloc_flag=%x\n",
		       policy_map, alloc_flags);

	CQM_DEBUG(CQM_DBG_FLAG_API, 
		  "%s: policy_map 0x%x\n", __func__, policy_map);
	return policy_map;
}

static void enable_buf_alloc_soffet(const u32 val)
{
	void *base = cqm_get_dmadesc_64();
	u8 i, id;
	struct eqm_pid_s *enq_pid_p = &cqm_ctrl->eqm_pid_cfg;

	for (id = enq_pid_p->cpu_start; id <= enq_pid_p->cpu_end; id++) {
		for (i = 0; i < CQM_LGM_NUM_CPU_POLICY; i++)
			cbm_w32(bufreq[i] + (id * 0x1000), val);
	}
	for (id = enq_pid_p->vm_start; id <= enq_pid_p->qosbp_end; id++)
		cbm_w32(bufreq[CQM_SIZE2_BUF_SIZE] + (id * 0x1000), val);
	for (id = enq_pid_p->aca_start; id <= enq_pid_p->aca_end; id++) {
		for (i = 0; i < LGM_CQM_ACA_DESC_MAX; i++)
			cbm_w32(base + CQM_EQM_DC_BUF_ALLOC(id, i), val);
	}
}

int find_eqm_port_type(int port)
{
	struct eqm_pid_s *p = &cqm_ctrl->eqm_pid_cfg;

	if (port >= p->cpu_start && port <= p->cpu_end)
		return EQM_CPU_TYPE;
	else if (port >= p->vm_start && port <= p->voice_end)
		return EQM_VM_TYPE;
	else if (port >= p->aca_start && port <= p->aca_end)
		return EQM_ACA_TYPE;
	else if (port >= p->dma0_start && port <= p->rxdma)
		return EQM_DMA_TYPE;
	else
		return NONE_TYPE;
}

int find_dqm_port_type(int port)
{
	struct dqm_pid_s *p = &cqm_ctrl->dqm_pid_cfg;

	if (port >= p->cpu_start && port <= p->lro_end)
		return DQM_CPU_TYPE;
	if (port >= p->aca_start && port <= p->aca_end)
		return DQM_ACA_TYPE;
	else if (port >= p->dma2_start && port <= p->dma0_end)
		return DQM_DMA_TYPE;
	else if (port >= p->pon_start && port <= p->pon_end)
		return DQM_PON_TYPE;
	else if (port >= p->docsis_start && port <= p->docsis_end)
		return DQM_DOCSIS_TYPE;
	else if (port >= p->ppnf_start && port <= p->ppnf_end)
		return DQM_PP_NF_TYPE;
	else if (port >= p->vuni_start && port <= p->vuni_end)
		return DQM_VUNI_TYPE;
	else if (port >= p->reinsert_start && port <= p->reinsert_end)
		return DQM_REINSERT_TYPE;
	else
		return NONE_TYPE;
}

static inline bool cqm_is_port_valid(const u32 id)
{
	return (id != CBM_PORT_INVALID) ? 1 : 0;
}

static bool is_system_pool(const u8 id)
{
	if (id < CQM_LGM_TOTAL_BM_POOLS)
		return (cqm_ctrl->lgm_pool_type[id] == CQM_NIOC_SHARED) ||
		       (cqm_ctrl->lgm_pool_type[id] == SSB_NIOC_SHARED);
	else
		return false;
}

static bool is_lrouc_pool(const u8 id)
{
	if (id < CQM_LGM_TOTAL_BM_POOLS)
		return (cqm_ctrl->lgm_pool_type[id] == SSB_LROUC_NIOC_SHARED) ||
		       (cqm_ctrl->lgm_pool_type[id] == SSB_LROUC_NIOC_ISOLATED);

	else
		return false;
}

void deq_umt_trigger(const int *cbm_port, struct umt_trig *ctrl)
{
	u32 aca_port = cbm_port[0] - cqm_ctrl->dqm_pid_cfg.aca_start;
	u32 config = 0, pattern = BIT(0);

	if (ctrl->mark_en) {
		pattern |= BIT(1);
		cbm_w32(cqm_ctrl->cqm + CBM_BUFFER_METADATA_CTRL,
			BM_MARK_OFFSET);
	}
	config = cbm_r32(cqm_ctrl->cqm + CBM_ACA_CTRL);

	if (ctrl->port_en)
		config |= pattern << (4 * aca_port);
	else
		config &= (~(BIT(0) | BIT(1))) << (4 * aca_port);
	cbm_w32(cqm_ctrl->cqm + CBM_ACA_CTRL, config);
}

void *cqm_cpubuff_get_buffer_head(void *p, u8 pool)
{
	unsigned long addr = (unsigned long)p;
	unsigned long offset;

	/* The buffer head should be (pool_start_low + buf_frm_size * N).
	 * It should not be assumed that pool_start_low is aligned to
	 * buf_frm_size.
	 *
	 * The differentiation of whether buf_frm_size is power of 2 is
	 * necessary to avoid the division operation for pools with buf_frm_size
	 * being power of 2.
	 */
	if (is_power_of_2(bm_pool_conf[pool].buf_frm_size)) {
		offset = (addr - bm_pool_conf[pool].pool_start_low) &
			(bm_pool_conf[pool].buf_frm_size - 1);
	} else {
		offset = (addr - bm_pool_conf[pool].pool_start_low) %
			(bm_pool_conf[pool].buf_frm_size);
	}
	p = (void *)(addr - offset);

	return p;
}
EXPORT_SYMBOL(cqm_cpubuff_get_buffer_head);

static inline void cqm_populate_entry(struct cqm_pmac_port_map *local_entry,
				      int *phys_port, u32 cbm_port,
				      u32 flags)
{
	int index = cbm_port / BITS_PER_LONG;
	*phys_port = cbm_port;
	cqm_ctrl->dqm_port_info[cbm_port].allocated = P_ALLOCATED;
	local_entry->egp_port_map[index] |= BIT(cbm_port % BITS_PER_LONG);
	local_entry->egp_type = flags;
}

static inline unsigned long *cqm_port_mode(void)
{
	return &cqm_ctrl->lpid_config.cqm_lpid_port_mode;
}

static inline unsigned long *cqm_wav_port_mode(void)
{
	return &cqm_ctrl->wav.lpid_port;
}

static inline struct cqm_lpid_epg_map_s *cqm_lpid_epg_map(void)
{
	return &cqm_ctrl->lpid_config.lpid_epg_map[0];
}

static int is_excluded(u32 flags, u32 exclude_flags)
{
	if (flags & exclude_flags)
		return 1;
	else
		return 0;
}

/* !< function	 : is_cbm_allocated()
 *    description: check the availability from global table
 *    param[in]	 : port no, flags(eq/dq type)
 *    param[out] : bool
 */
static bool is_cbm_allocated(s32 port, u32 flags)
{
	if (flags == EQ_PORT &&
	    cqm_ctrl->eqm_port_info[port].allocated == P_ALLOCATED)
		return true;
	else if (flags == DQ_PORT &&
		 cqm_ctrl->dqm_port_info[port].allocated == P_ALLOCATED)
		return true;
	else
		return false;
}

static struct cqm_pmac_port_map *is_dp_allocated(s32 pmac, u32 flags)
{
	struct cqm_pmac_port_map *ptr = NULL;
	unsigned long lock_flags;
	u32 exclude_flags = DP_F_DONTCARE;

	if (!is_excluded(flags, exclude_flags))
		flags = DP_F_DONTCARE;
	spin_lock_irqsave(&cqm_port_map, lock_flags);
	if (flags & DP_F_DONTCARE) {
		list_for_each_entry(ptr, &pmac_mapping_list, list) {
			dev_dbg(cqm_ctrl->dev,
				"DC: pmac %d type %d input %d flags %d\r\n",
				ptr->port_id, ptr->egp_type, pmac, flags);
			if (ptr->port_id == pmac &&
			    (!is_excluded(ptr->egp_type, exclude_flags))) {
				spin_unlock_irqrestore(&cqm_port_map,
						       lock_flags);
				return ptr;
			}
		}
	} else {
		list_for_each_entry(ptr, &pmac_mapping_list, list) {
			dev_dbg(cqm_ctrl->dev, "pmac %d type %d \r\n",
				ptr->port_id, ptr->egp_type);
			if (ptr->egp_type & flags) {
				spin_unlock_irqrestore(&cqm_port_map,
						       lock_flags);
				return ptr;
			}
		}
	}
	spin_unlock_irqrestore(&cqm_port_map, lock_flags);
	return NULL;
}

static void cqm_dump_pmac_mapping_list(void)
{
	struct cqm_pmac_port_map *local_entry = NULL;
	unsigned char str[128];
	int idx;

	CQM_DEBUG_RETURN_ON_UNSET(CQM_DBG_FLAG_API);

	CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "pmac_mapping_list:\n");
	list_for_each_entry(local_entry, &pmac_mapping_list, list) {
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "entry:\n");
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "\t flags: %s\n",
			  local_entry->flags < ARRAY_SIZE(pmac_list_flag_str) ?
			  pmac_list_flag_str[local_entry->flags] : "Invalid");
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "\t port_id: %u\n", local_entry->port_id);
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "\t dev_port: %u\n", local_entry->dev_port);
		memset(str, '\0', sizeof(str));
		for (idx = 0; idx < LGM_MAX_PORT_MAP; idx++) {
			snprintf(str + strlen(str), sizeof(str) - strlen(str), "0x%llx%s",
				local_entry->egp_port_map[idx],
				idx == LGM_MAX_PORT_MAP - 1 ? "" : ", ");
		}
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "\t egp_port_map: [%s]\n", str);
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "\t qid_num: %u\n", local_entry->qid_num);
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "\t egp_type: 0x%x\n", local_entry->egp_type);
		memset(str, '\0', sizeof(str));
		for (idx = 0; idx < DP_RX_RING_NUM; idx++) {
			snprintf(str + strlen(str), sizeof(str) - strlen(str), "%u%s",
				local_entry->out_cqm_deq_port_id[idx],
				idx == LGM_MAX_PORT_MAP - 1 ? "" : ", ");
		}
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "\t out_cqm_deq_port_id: [%s]\n", str);
		memset(str, '\0', sizeof(str));
		for (idx = 0; idx < DP_RX_RING_NUM; idx++) {
			snprintf(str + strlen(str), sizeof(str) - strlen(str), "%u%s",
				local_entry->out_enq_port_id[idx],
				idx == LGM_MAX_PORT_MAP - 1 ? "" : ", ");
		}
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "\t out_enq_port_id: [%s]\n", str);
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "\t num_rx_ring: %u\n", local_entry->num_rx_ring);
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "\t num_tx_ring: %u\n", local_entry->num_tx_ring);
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "\t bm_policy_res_id: %u\n",
			  local_entry->bm_policy_res_id);
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "\t dma_chnl_init: %u\n",
			  local_entry->dma_chnl_init);
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "\t eqp_en_cnt: %u\n", local_entry->eqp_en_cnt);
		memset(str, '\0', sizeof(str));
		for (idx = 0; idx < DP_TX_RING_NUM; idx++) {
			snprintf(str + strlen(str), sizeof(str) - strlen(str), "%u%s",
				local_entry->tx_deq_port_id[idx],
				idx == LGM_MAX_PORT_MAP - 1 ? "" : ", ");
		}
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "\t tx_deq_port_id: [%s]\n", str);
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "\t link_speed_cap: 0x%x\n",
			  local_entry->link_speed_cap);
	}
}

static void cqm_dump_dp_rx_ring(const char *fn, int idx, struct dp_rx_ring *dp)
{
	CQM_DEBUG_RETURN_ON_UNSET(CQM_DBG_FLAG_API);

	if (!dp)
		return;

	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t rx_ring[%d]: out_enq_ring_size: %d\n",
		  fn, idx, dp->out_enq_ring_size);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t rx_ring[%d]: out_enq_port_id: %u\n",
		  fn, idx, dp->out_enq_port_id);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t rx_ring[%d]: out_cqm_deq_port_id: %u\n",
		  fn, idx, dp->out_cqm_deq_port_id);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t rx_ring[%d]: num_out_cqm_deq_port: %u\n",
		  fn, idx, dp->num_out_cqm_deq_port);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t rx_ring[%d]: in_alloc_ring_size: %d\n",
		  fn, idx, dp->in_alloc_ring_size);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t rx_ring[%d]: num_pkt: %u\n",
		  fn, idx, dp->num_pkt);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t rx_ring[%d]: rx_pkt_size: %d\n",
		  fn, idx, dp->rx_pkt_size);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t rx_ring[%d]: rx_policy_base: %u\n",
		  fn, idx, dp->rx_policy_base);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t rx_ring[%d]: policy_num: %u\n",
		  fn, idx, dp->policy_num);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t rx_ring[%d]: rx_poolid: %u\n",
		  fn, idx, dp->rx_poolid);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t rx_ring[%d]: prefill_pkt_num: %d\n",
		  fn, idx, dp->prefill_pkt_num);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t rx_ring[%d]: out_msg_mode: %d\n",
		  fn, idx, dp->out_msg_mode);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t rx_ring[%d]: out_qos_mode: %d\n",
		  fn, idx, dp->out_qos_mode);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t rx_ring[%d]: num_egp: %d\n",
		  fn, idx, dp->num_egp);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t rx_ring[%d]: bpress_out: %d\n",
		  fn, idx, dp->bpress_out);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t rx_ring[%d]: bpress_alloc: %d\n",
		  fn, idx, dp->bpress_alloc);
}

static void cqm_dump_dp_tx_ring(const char *fn, int idx, struct dp_tx_ring *dp)
{
	CQM_DEBUG_RETURN_ON_UNSET(CQM_DBG_FLAG_API);

	if (!dp)
		return;

	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t tx_ring[%d]: in_deq_ring_size: %d\n",
		  fn, idx, dp->in_deq_ring_size);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t tx_ring[%d]: out_free_ring_size: %d\n",
		  fn, idx, dp->out_free_ring_size);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t tx_ring[%d]: num_tx_pkt: %u\n",
		  fn, idx, dp->num_tx_pkt);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t tx_ring[%d]: tx_pkt_size: %d\n",
		  fn, idx, dp->tx_pkt_size);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t tx_ring[%d]: txout_policy_base: %d\n",
		  fn, idx, dp->txout_policy_base);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t tx_ring[%d]: policy_num: %d\n",
		  fn, idx, dp->policy_num);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t tx_ring[%d]: tx_poolid: %d\n",
		  fn, idx, dp->tx_poolid);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t tx_ring[%d]: f_out_auto_free: %u\n",
		  fn, idx, dp->f_out_auto_free);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t tx_ring[%d]: tx_deq_port: %u\n",
		  fn, idx, dp->tx_deq_port);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t tx_ring[%d]: not_valid: %d\n",
		  fn, idx, dp->not_valid);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t tx_ring[%d]: bpress_in: %d\n",
		  fn, idx, dp->bpress_in);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t tx_ring[%d]: bpress_free: %d\n",
		  fn, idx, dp->bpress_free);
}

static void cqm_dump_dp_umt_port(const char *fn, int idx, struct dp_umt_port *dp)
{
	int id_ta;

	CQM_DEBUG_RETURN_ON_UNSET(CQM_DBG_FLAG_API);

	if (!dp)
		return;

	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t umt[%d]: ctl.id: %d\n",
		  fn, idx, dp->ctl.id);
	for (id_ta = 0; id_ta < dp->ctl.dst_addr_cnt; id_ta++)
		CQM_DEBUG(CQM_DBG_FLAG_API,
			  "%s:\t umt[%d]: ctl.dst[%d]: 0x%llx\n",
			  fn, idx, id_ta, dp->ctl.dst[id_ta]);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t umt[%d]: ctl.msg_interval: %u\n",
		  fn, idx, dp->ctl.msg_interval);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t umt[%d]: ctl.msg_mode: %d\n",
		  fn, idx, dp->ctl.msg_mode);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t umt[%d]: ctl.cnt_mode: %d\n",
		  fn, idx, dp->ctl.cnt_mode);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t umt[%d]: ctl.sw_msg: %d\n",
		  fn, idx, dp->ctl.sw_msg);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t umt[%d]: ctl.rx_msg_mode: %d\n",
		  fn, idx, dp->ctl.rx_msg_mode);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t umt[%d]: ctl.enable: %u\n",
		  fn, idx, dp->ctl.enable);
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s:\t umt[%d]: ctl.fflag: 0x%lx\n",
		  fn, idx, dp->ctl.fflag);
}

static void cqm_dump_dp_dev_data(const char *fn, struct dp_dev_data *dp)
{
	int idx;

	CQM_DEBUG_RETURN_ON_UNSET(CQM_DBG_FLAG_API);

	if (!dp)
		return;

	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: dp_dev_data:\n", fn);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t flag_ops: %d\n",
		  fn, dp->flag_ops);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t num_rx_ring: %d\n",
		  fn, dp->num_rx_ring);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t num_tx_ring: %d\n",
		  fn, dp->num_tx_ring);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t num_umt_port: %d\n",
		  fn, dp->num_umt_port);
	for (idx = 0; idx < DP_RX_RING_NUM; idx++)
		cqm_dump_dp_rx_ring(fn, idx, &dp->rx_ring[idx]);
	for (idx = 0; idx < DP_RX_RING_NUM; idx++)
		cqm_dump_dp_tx_ring(fn, idx, &dp->tx_ring[idx]);
	for (idx = 0; idx < DP_RX_RING_NUM; idx++)
		cqm_dump_dp_umt_port(fn, idx, &dp->umt[idx]);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t enable_cqm_meta: %u\n",
		  fn, dp->enable_cqm_meta);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t max_ctp: %u\n",
		  fn, dp->max_ctp);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t max_gpid: %u\n",
		  fn, dp->max_gpid);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t num_resv_q: %d\n",
		  fn, dp->num_resv_q);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t num_resv_sched: %d\n",
		  fn, dp->num_resv_sched);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t qos_resv_q_base: %u\n",
		  fn, dp->qos_resv_q_base);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t wib_tx_phy_addr: 0x%llx\n",
		  fn, dp->wib_tx_phy_addr);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t bm_policy_res_id: %u\n",
		  fn, dp->bm_policy_res_id);
}

static void cqm_dump_dp_alloc_complete_data(const char *fn, struct cbm_dp_alloc_complete_data *dp)
{
	CQM_DEBUG_RETURN_ON_UNSET(CQM_DBG_FLAG_API);

	if (!dp)
		return;

	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: cbm_dp_alloc_complete_data:\n", fn);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: num_rx_ring: %u\n",
		  fn, dp->num_rx_ring);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: num_tx_ring: %u\n",
		  fn, dp->num_tx_ring);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: num_umt_port: %u\n",
		  fn, dp->num_umt_port);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: enable_cqm_meta: %u\n",
		  fn, dp->enable_cqm_meta);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: qid_base: %d\n", fn, dp->qid_base);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: num_qid: %d\n", fn, dp->num_qid);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: bm_policy_res_id: %u\n",
		  fn, dp->bm_policy_res_id);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: tx_policy_base: %d\n",
		  fn, dp->tx_policy_base);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: tx_policy_num: %d\n",
		  fn, dp->tx_policy_num);
	cqm_dump_dp_dev_data(fn, dp->data);
}

static void cqm_dump_dp_port_data(const char *fn, struct dp_port_data *dp)
{
	CQM_DEBUG_RETURN_ON_UNSET(CQM_DBG_FLAG_API);

	if (!dp)
		return;

	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: dp_port_data:\n", fn);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t flag_ops: %d\n", fn, dp->flag_ops);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t resv_num_port: %u\n",
		  fn, dp->resv_num_port);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t start_port_no: %u\n",
		  fn, dp->start_port_no);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t num_resv_q: %d\n",
		  fn, dp->num_resv_q);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t num_resv_sched: %d\n",
		  fn, dp->num_resv_sched);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t deq_port_base: %d\n",
		  fn, dp->deq_port_base);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t deq_num: %d\n", fn, dp->deq_num);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t link_speed_cap: %u\n",
		  fn, dp->link_speed_cap);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: \t n_wib_credit_pkt: %u\n",
		  fn, dp->n_wib_credit_pkt);
}

static void cqm_dump_dp_alloc_data(const char *fn, struct cbm_dp_alloc_data *dp)
{
	CQM_DEBUG_RETURN_ON_UNSET(CQM_DBG_FLAG_API);

	if (!dp)
		return;

	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: cbm_dp_alloc_data:\n", fn);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: dp_inst: %d\n", fn, dp->dp_inst);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: cbm_inst: %d\n", fn, dp->cbm_inst);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: flags: 0x%x\n", fn, dp->flags);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: dp_port: %u\n", fn, dp->dp_port);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: deq_port_num: %u\n",
		  fn, dp->deq_port_num);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: deq_port: %u\n", fn, dp->deq_port);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: dma_chan: %u\n", fn, dp->dma_chan);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: tx_pkt_credit: %u\n",
		  fn, dp->tx_pkt_credit);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: tx_b_credit: %u\n",
		  fn, dp->tx_b_credit);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: txpush_addr_qos: %pS\n",
		  fn, dp->txpush_addr_qos);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: txpush_addr: %pS\n",
		  fn, dp->txpush_addr);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: tx_ring_size: %u\n",
		  fn, dp->tx_ring_size);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: tx_ring_offset: %u\n",
		  fn, dp->tx_ring_offset);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: tx_ring_addr_txpush: %pS\n",
		  fn, dp->tx_ring_addr_txpush);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: num_dma_chan: %u\n",
		  fn, dp->num_dma_chan);
	cqm_dump_dp_port_data(fn, dp->data);
}

static void cqm_dump_dp_spl_cfg(const char *fn, const struct dp_spl_cfg *dp)
{
	CQM_DEBUG_RETURN_ON_UNSET(CQM_DBG_FLAG_API);

	if (!dp)
		return;

	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: dp_spl_cfg:\n", fn);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: flag: %u\n", fn, dp->flag);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: type: %d\n", fn, dp->type);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: spl_id: %u\n", fn, dp->spl_id);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: f_subif: %u\n", fn, dp->f_subif);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: f_gpid: %u\n", fn, dp->f_gpid);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: f_policy: %u\n", fn, dp->f_policy);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: f_hostif: %u\n", fn, dp->f_hostif);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: prel2_len: %u\n", fn, dp->prel2_len);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: subif: %d\n", fn, dp->subif);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: gpid: %d\n", fn, dp->gpid);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: spl_gpid: %d\n", fn, dp->spl_gpid);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: dp_port: %d\n", fn, dp->dp_port);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: num_igp: %u\n", fn, dp->num_igp);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: num_egp: %u\n", fn, dp->num_egp);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: num_umt_port: %u\n",
		  fn, dp->num_umt_port);
}

static void cqm_dump_dp_subif_data(const char *fn, struct dp_subif_data *dp)
{
	CQM_DEBUG_RETURN_ON_UNSET(CQM_DBG_FLAG_API);

	if (!dp)
		return;

	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: dp_subif_data:\n", fn);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: deq_port_idx: %d\n",
		  fn, dp->deq_port_idx);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: flag_ops: %d\n", fn, dp->flag_ops);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: q_id: %d\n", fn, dp->q_id);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: f_policy: %u\n", fn, dp->f_policy);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: tx_pkt_size: %u\n",
		  fn, dp->tx_pkt_size);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: tx_policy_base: %u\n",
		  fn, dp->tx_policy_base);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: tx_policy_num: %u\n",
		  fn, dp->tx_policy_num);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: rx_policy_base: %u\n",
		  fn, dp->rx_policy_base);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: rx_policy_num: %u\n",
		  fn, dp->rx_policy_num);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: txin_ring_size: %u\n",
		  fn, dp->txin_ring_size);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: txin_ring_phy_addr: %pS\n",
		  fn, dp->txin_ring_phy_addr);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: credit_add_phy_addr: %pS\n",
		  fn, dp->credit_add_phy_addr);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: credit_left_phy_addr: %pS\n",
		  fn, dp->credit_left_phy_addr);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: mac_learn_disable: %u\n",
		  fn, dp->mac_learn_disable);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: num_deq_port: %u\n",
		  fn, dp->num_deq_port);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: rx_en_flag: %u\n",
		  fn, dp->rx_en_flag);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: bm_policy_res_id: %u\n",
		  fn, dp->bm_policy_res_id);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: swdev_en_flag: %u\n",
		  fn, dp->swdev_en_flag);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: domain_id: %u\n",
		  fn, dp->domain_id);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: domain_members: %u\n",
		  fn, dp->domain_members);
}

static void cqm_dump_dp_en_data(const char *fn, struct cbm_dp_en_data *dp)
{
	CQM_DEBUG_RETURN_ON_UNSET(CQM_DBG_FLAG_API);

	if (!dp)
		return;

	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: cbm_dp_en_data:\n", fn);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: dp_inst: %d\n", fn, dp->dp_inst);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: cbm_inst: %d\n", fn, dp->cbm_inst);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: deq_port: %u\n", fn, dp->deq_port);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: num_deq_port: %d\n",
		  fn, dp->num_deq_port);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: dma_chnl_init: %u\n",
		  fn, dp->dma_chnl_init);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: f_policy: %u\n", fn, dp->f_policy);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: tx_policy_num: %d\n",
		  fn, dp->tx_policy_num);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: tx_policy_base: %u\n",
		  fn, dp->tx_policy_base);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: tx_ring_size: %d\n",
		  fn, dp->tx_ring_size);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: rx_policy_num: %d\n",
		  fn, dp->rx_policy_num);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: rx_policy_base: %u\n",
		  fn, dp->rx_policy_base);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: num_dma_chan: %u\n",
		  fn, dp->num_dma_chan);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: bm_policy_res_id: %u\n",
		  fn, dp->bm_policy_res_id);
	cqm_dump_dp_subif_data(fn, dp->data);
}

static void cqm_dump_cbm_dc_res(const char *fn, struct cbm_dc_res *dp)
{
	CQM_DEBUG_RETURN_ON_UNSET(CQM_DBG_FLAG_API);

	if (!dp)
		return;

	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: cbm_dc_res:\n", fn);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: cqm_inst: %d\n", fn, dp->cqm_inst);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: dp_port: %d\n", fn, dp->dp_port);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: res_id: %d\n", fn, dp->res_id);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: alloc_flags: 0x%x\n",
		  fn, dp->alloc_flags);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: cqm_inst: %d\n", fn, dp->cqm_inst);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: rx_res.in_alloc_ring_size: %d\n",
		  fn, dp->rx_res.in_alloc_ring_size);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: rx_res.out_enq_ring_size: %d\n",
		  fn, dp->rx_res.out_enq_ring_size);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: rx_res.rx_bufs: %d\n",
		  fn, dp->rx_res.rx_bufs);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: tx_res.in_deq_ring_size: %d\n",
		  fn, dp->tx_res.in_deq_ring_size);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: tx_res.out_free_ring_size: %d\n",
		  fn, dp->tx_res.out_free_ring_size);
	CQM_DEBUG(CQM_DBG_FLAG_API, "%s: tx_res.tx_bufs: %d\n",
		  fn, dp->tx_res.tx_bufs);
}

static struct cqm_pmac_port_map *cqm_get_pmac_mapping(s32 dp_port)
{
	struct cqm_pmac_port_map *local_entry = NULL;

	list_for_each_entry(local_entry, &pmac_mapping_list, list) {
		if (local_entry->port_id == dp_port)
			return local_entry;
	}

	/* Unable to find matching pmac. Dump pmac list */
	cqm_dump_pmac_mapping_list();

	return NULL;
}

static u32 cqm_pmac_to_deqp_from_pmac_mapping_list(struct cqm_pmac_port_map *local_entry)
{
	u32 deqport = INVALID_DQP;
	int idx;

	if (!local_entry)
		return deqport;

	/* Find the deqport from egp_port_map with smallest index.
	 * There will be more than 1 bit set for dp_port if number
	 * of tx_ring is 2. In this case, we only return the shared
	 * deqport which has the smallest index.
	 */
	for (idx = 0; idx < LGM_MAX_PORT_MAP; idx++) {
		if (local_entry->egp_port_map[idx]) {
			deqport = __ffs64(local_entry->egp_port_map[idx]) +
				(BITS_PER_LONG * idx);
			break;
		}
	}
	return deqport;
}

static u32 cqm_dp_to_deqp_from_pmac_mapping_list(s32 dp_port)
{
	struct cqm_pmac_port_map *local_entry = NULL;

	local_entry = cqm_get_pmac_mapping(dp_port);
	if (!local_entry)
		return INVALID_DQP;

	return cqm_pmac_to_deqp_from_pmac_mapping_list(local_entry);
}

/* Helper function to get the link speed cap for the dp_port.
 * If the dp_port has not been allocated, return 0.
 * If the dp_port has been allocated, return the link_speed_cap.
 * For example, if the link support 10G, it should return 10000.
 */
static u32 dp_port_link_speed_cap(s32 dp_port)
{
	struct cqm_pmac_port_map *ptr = NULL;

	ptr = cqm_get_pmac_mapping(dp_port);
	if (!ptr) {
		dev_err(cqm_ctrl->dev, "%s invalid dp_port\n", __func__);
		return 0;
	}

	return ptr->link_speed_cap;
}

static inline bool check_table(int lpid_in, int lpid, int i, int range)
{
	bool matched = false;
	int port = epg_lookup_table[i].epg + range;

	if (lpid_in &&
	    epg_lookup_table[i].lpid == lpid) {
		matched = true;
	} else if (!lpid_in) {
		matched = true;
	}

	if (matched) {
		if (is_cbm_allocated(port, DQ_PORT)) {
			dev_dbg(cqm_ctrl->dev,
				"CQM port %d allocated already\n",
				port);
			return false;
		} else if (port == cqm_ctrl->flush_port) {
			dev_err(cqm_ctrl->dev,
				"DMA port %d in use lpid %d\n",
				port, lpid);
			return false;
		}
		return true;
	}
	return false;
}

static u32 get_matching_pmac(u32 *ep, u32 flags, u32 *cbm_port, int lpid,
			     int lpid_in)
{
	u32 i;
	u32 j;
	u32 result = CBM_NOTFOUND;

	for (i = 0; i < LGM_MAX_PORT_PER_EP; i++) {
		ep[i] = CBM_PORT_INVALID;
		cbm_port[i] = CBM_PORT_INVALID;
	}
	for (i = 0; i < ARRAY_SIZE(epg_lookup_table); i++) {
		if (epg_lookup_table[i].port_type & flags) {
			for (j = 0; j <= epg_lookup_table[i].epg_range; j++) {
				if (check_table(lpid_in, lpid, i, j) == true) {
					ep[0] = epg_lookup_table[i].lpid;
					cbm_port[0] =
						epg_lookup_table[i].epg + j;
					return CBM_SUCCESS;
				}
			}
		}
	}
	return result;
}

static inline int get_intr_to_line(int intr, int *line)
{
	int i;

	for (i = 0; i < CQM_MAX_INTR_LINE; i++) {
		if (cqm_ctrl->cbm_irq[i] == intr) {
			*line = cqm_ctrl->cbm_line[i];
			return CBM_SUCCESS;
		}
	}
	return CBM_FAILURE;
}

static int cqm_delete_from_list(s32 egp_port, u32 flags)
{
	struct cqm_pmac_port_map *ptr = NULL;
	struct cqm_pmac_port_map *next = NULL;
	int found = 0;
	unsigned long lock_flags;
	u32 exclude_flags = DP_F_DONTCARE;

	if (!is_excluded(flags, exclude_flags))
		flags = DP_F_DONTCARE;
	spin_lock_irqsave(&cqm_port_map, lock_flags);
	list_for_each_entry_safe(ptr, next, &pmac_mapping_list, list) {
		if (ptr->port_id == egp_port) {
			if (flags == DP_F_DONTCARE) {
				found = 1;
				break;
			} else if (ptr->egp_type & flags) {
				found = 1;
				break;
			}
		}
	}
	if (found) {
		list_del(&ptr->list);
		devm_kfree(cqm_ctrl->dev, ptr);
		spin_unlock_irqrestore(&cqm_port_map, lock_flags);
		return 1;
	}
	spin_unlock_irqrestore(&cqm_port_map, lock_flags);
	return 0;
}

static struct
cqm_pmac_port_map *cqm_add_to_list(const struct cqm_pmac_port_map *egpinfo)
{
	struct cqm_pmac_port_map *l_egpinfo = NULL;
	int port = 0;
	unsigned long flags;

	l_egpinfo = devm_kzalloc(cqm_ctrl->dev,
				 sizeof(struct cqm_pmac_port_map), GFP_ATOMIC);
	if (l_egpinfo == 0)
		return NULL;

	l_egpinfo->port_id = egpinfo->port_id;
	l_egpinfo->alloc_flags = egpinfo->alloc_flags;
	for (port = 0; port < LGM_MAX_PORT_MAP; port++)
		l_egpinfo->egp_port_map[port] = egpinfo->egp_port_map[port];
	l_egpinfo->egp_type = egpinfo->egp_type;
	l_egpinfo->owner = 0;
	l_egpinfo->dev = 0;
	l_egpinfo->dev_port = egpinfo->dev_port;
	l_egpinfo->flags = P_ALLOCATED;
	l_egpinfo->link_speed_cap = egpinfo->link_speed_cap;
	spin_lock_irqsave(&cqm_port_map, flags);
	/* Init the list within the struct. */
	INIT_LIST_HEAD(&l_egpinfo->list);
	/* Add this struct to the tail of the list. */
	list_add_tail(&l_egpinfo->list, &pmac_mapping_list);
	spin_unlock_irqrestore(&cqm_port_map, flags);
	return l_egpinfo;
}

static u32 get_matching_flag(u32 *flags, u32 cqm_port)
{
	int i, result = CBM_NOTFOUND;

	for (i = 0; i < ARRAY_SIZE(epg_lookup_table); i++) {
		if (epg_lookup_table[i].epg == cqm_port) {
			*flags = epg_lookup_table[i].port_type;
			result = CBM_SUCCESS;
			break;
		}
	}
	return result;
}

#define IS_CPU_POOL(pool, policy) ((!(pool)) && ((policy) == CQM_CPU_POLICY))

static void
NO_OPTIMIZE get_bm_info(struct cqm_bm_pool_config *tmp_bm, unsigned long size,
			enum CQM_BUF_TYPE buf_type)
{
	u32 pool;

	for (pool = 0; pool < CQM_LGM_TOTAL_POOLS; pool++) {
		if (bm_pool_conf[pool].buf_frm_size != size ||
		    bm_pool_conf[pool].buf_type != buf_type)
			continue;
		tmp_bm->offset_mask = bm_pool_conf[pool].offset_mask;
		tmp_bm->segment_mask = bm_pool_conf[pool].segment_mask;
		tmp_bm->pool = bm_pool_conf[pool].pool;
		tmp_bm->policy = bm_pool_conf[pool].policy;
		tmp_bm->buf_frm_size = bm_pool_conf[pool].buf_frm_size;
		tmp_bm->buf_type = buf_type;
		return;
	}
}

static void store_bufreq_baseaddr(void)
{
	void *enq = cqm_ctrl->enq;
	u32 enqport;
	struct eqm_pid_s *enq_p = cqm_get_eqm_pid_cfg();

	bufreq[CQM_SIZE0_BUF_SIZE] = enq + EQ_CPU_PORT(0, new_s0ptr);
	bufreq[CQM_SIZE1_BUF_SIZE] = enq + EQ_CPU_PORT(0, new_s1ptr);
	bufreq[CQM_SIZE2_BUF_SIZE] = enq + EQ_CPU_PORT(0, new_s2ptr);
	bufreq[CQM_SIZE3_BUF_SIZE] = enq + EQ_CPU_PORT(0, new_s3ptr);
	bufreq[CQM_FSQM_BUF_SIZE] = enq + EQ_CPU_PORT(0, new_pbptr);
	bufreq_1[CQM_SIZE0_BUF_SIZE] = enq + EQ_CPU_PORT(0, new_s0ptr_1);
	bufreq_1[CQM_SIZE1_BUF_SIZE] = enq + EQ_CPU_PORT(0, new_s1ptr_1);
	bufreq_1[CQM_SIZE2_BUF_SIZE] = enq + EQ_CPU_PORT(0, new_s2ptr_1);
	bufreq_1[CQM_SIZE3_BUF_SIZE] = enq + EQ_CPU_PORT(0, new_s3ptr_1);
	bufreq_1[CQM_FSQM_BUF_SIZE] = enq + EQ_CPU_PORT(0, new_pbptr_1);
	cbm_cntr_func[0].reg = enq + CBM_EQM_CTRL;
	cbm_cntr_func[1].reg = enq + CBM_DQM_CTRL;
	/*!< get CPU Ingress Port Descriptor base */

	for (enqport = enq_p->cpu_start; enqport <= enq_p->qosbp_end;
	     enqport++) {
		eqmdesc[enqport] = enq +
			EQ_CPU_PORT(enqport, desc0.desc0);
		dev_dbg(cqm_ctrl->dev, "%s: enqport:%d\n",
			__func__, enqport);
	}
}

static s32 pib_status_get(struct pib_stat *ctrl)
{
	u32 reg = cbm_r32(cqm_ctrl->pib + PIB_STATUS);

	ctrl->en_stat = (reg & PIB_STATUS_PIB_EN_STS_MASK) >>
			 PIB_STATUS_PIB_EN_STS_POS;
	ctrl->pause_stat = (reg & PIB_STATUS_PIB_PAUSE_STS_MASK) >>
			    PIB_STATUS_PIB_PAUSE_STS_POS;
	ctrl->fifo_size = (reg & PIB_STATUS_FIFO_SIZE_MASK) >>
			   PIB_STATUS_FIFO_SIZE_POS;
	ctrl->fifo_full = (reg & PIB_STATUS_FIFO_FULL_MASK) >>
			   PIB_STATUS_FIFO_FULL_POS;
	ctrl->fifo_empty = (reg & PIB_STATUS_FIFO_EMPTY_MASK) >>
			    PIB_STATUS_FIFO_EMPTY_POS;
	ctrl->cmd_ovflw = (reg & PIB_STATUS_FIFO_OVFL_MASK) >>
			   PIB_STATUS_FIFO_OVFL_POS;
	ctrl->cmd_illegal_port = (reg & PIB_STATUS_ILLEGAL_PORT_MASK) >>
				  PIB_STATUS_ILLEGAL_PORT_POS;
	ctrl->wakeup_intr = (reg & PIB_STATUS_WAKEUP_INT_MASK) >>
			     PIB_STATUS_WAKEUP_INT_POS;
	return CBM_SUCCESS;
}

static s32 pib_ovflw_cmd_get(u32 *cmd)
{
	*cmd = cbm_r32(cqm_ctrl->pib + PIB_FIFO_OVFL_CMD_REG);
	return CBM_SUCCESS;
}

static s32 pib_illegal_cmd_get(u32 *cmd)
{
	*cmd = cbm_r32(cqm_ctrl->pib + PIB_ILLEGAL_CMD_REG);
	return CBM_SUCCESS;
}

static s32 config_pib_ctrl(struct pib_ctrl *ctrl, u32 flags)
{
	void *base = (cqm_ctrl->pib + PIB_CTRL);

	if (flags & FLAG_PIB_ENABLE)
		set_val(base, ctrl->pib_en, PIB_CTRL_PIB_EN_MASK,
			PIB_CTRL_PIB_EN_POS);
	if (flags & FLAG_PIB_PAUSE)
		set_val(base, ctrl->pib_pause, PIB_CTRL_PIB_PAUSE_MASK,
			PIB_CTRL_PIB_PAUSE_POS);
	if (flags & FLAG_PIB_OVFLW_INTR)
		set_val(base, ctrl->cmd_ovflw_intr_en,
			PIB_CTRL_OVRFLW_INT_EN_MASK,
			PIB_CTRL_OVRFLW_INT_EN_POS);
	if (flags & FLAG_PIB_ILLEGAL_INTR)
		set_val(base, ctrl->cmd_illegal_port_intr_en,
			PIB_CTRL_ILLEGAL_PORT_INT_EN_MASK,
			PIB_CTRL_ILLEGAL_PORT_INT_EN_POS);
	if (flags & FLAG_PIB_WAKEUP_INTR)
		set_val(base, ctrl->wakeup_intr_en,
			PIB_CTRL_WAKEUP_INT_EN_MASK,
			PIB_CTRL_WAKEUP_INT_EN_POS);
	if (flags & FLAG_PIB_BYPASS)
		set_val(base, ctrl->pib_bypass, PIB_CTRL_PIB_BYPASS_MASK,
			PIB_CTRL_PIB_BYPASS_POS);
	if (flags & FLAG_PIB_DELAY)
		set_val(base, ctrl->deq_delay, PIB_CTRL_DQ_DLY_MASK,
			PIB_CTRL_DQ_DLY_POS);
	if (flags & FLAG_PIB_PKT_LEN_ADJ)
		set_val(base, ctrl->pkt_len_adj, PIB_CTRL_PKT_LEN_ADJ_EN_MASK,
			PIB_CTRL_PKT_LEN_ADJ_EN_POS);
	if (flags & FLAG_PIB_DC_MODE)
		set_val(base, ctrl->dc_mode, PIB_CTRL_PIB_DC_MODE_MASK,
			PIB_CTRL_PIB_DC_MODE_POS);

	return CBM_SUCCESS;
}

static s32 pib_port_enable(u32 deq_port, u32 enable)
{
	u32 port = deq_port % CQM_PON_IP_PORT_START;
	u32 offset = 0;

	offset = port < 32 ? PIB_PORT_EN_31_0 : PIB_PORT_EN_64_32;

	set_val((cqm_ctrl->pib + offset), !!enable,
		(PIB_PORT_EN_31_0_EN_PORT0_MASK << port), port);

	return CBM_SUCCESS;
}

static s32 pon_deq_cntr_get(int port, u32 *count)
{
	if (port < LGM_MAX_PON_PORTS) {
		cbm_r32((cqm_ctrl->deq + DQPC_PON_EGP_75 + (port * 4)));
		return CBM_SUCCESS;
	} else {
		return CBM_FAILURE;
	}
}

static s32 pib_program_overshoot(u32 overshoot_bytes)
{
	cbm_w32((cqm_ctrl->pib + PIB_OVERSHOOT_BYTES),
		(overshoot_bytes & PIB_OVERSHOOT_BYTES_OVERSHOOT_MASK));
	return CBM_SUCCESS;
}

static s32 enable_backpressure(s32 port_id, bool flag)
{
	void *deq = cqm_ctrl->deq;
	int retval = CBM_SUCCESS;
	u32 reg_off, config;

	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s port_id: %d flag: %d\n",
		  __func__, port_id, flag);

	switch (find_dqm_port_type(port_id)) {
	case DQM_PON_TYPE:
	case DQM_DOCSIS_TYPE:
		reg_off = (((port_id -
			   CQM_PON_IP_PORT_START) >=
			   BITS_PER_LONG)
			   ? EPON_PORTBP_REG_1 : EPON_PORTBP_REG_0);
		cbm_w32(cqm_ctrl->enq + reg_off, flag);
		break;
	case DQM_ACA_TYPE:
		config = cbm_r32(deq + DQ_CPU_PORT(port_id, cfg));
		if (flag)
			config |= CFG_ACA_EGP_19_DQBPEN_MASK;
		else
			config &= ~(CFG_ACA_EGP_19_DQBPEN_MASK);
		cbm_w32((deq + DQ_CPU_PORT(port_id, cfg)), config);

		break;
	default:
		dev_err(cqm_ctrl->dev, "Invalid port_id: %u", port_id);
		retval = CBM_FAILURE;
		break;
	}

	dev_dbg(cqm_ctrl->dev, "%s exit retval %d\n", __func__, retval);
	return retval;
}

static void cqm_qid_program(u8 qid_val, u32 qidt, u32 egflag)
{
	u32 qidt_idx;
	u32 qidt_offset;
	u32 offset_factor, value_mask;
	u32 value, shadow;
	void *qidt_base = egflag > 0 ? cqm_ctrl->qidt1 : cqm_ctrl->qidt0;

	qidt_idx = qidt >> 1;
	qidt_idx <<= 1;
	qidt_offset = qidt % 2;
	offset_factor = qidt_offset > 0 ? 9 : 0;
	value_mask = LGM_CQM_Q_MASK << offset_factor;

	shadow = g_cbm_qidt_mirror[egflag][qidt_idx >> 1].qidt_shadow;
	value = (shadow & ~value_mask) |
		((qid_val & LGM_CQM_Q_MASK) << offset_factor);
	g_cbm_qidt_mirror[egflag][qidt_idx >> 1].qidt_shadow = value;
	cbm_w32((qidt_base + qidt_idx * 2), value);
}

static void set_lookup_qid_via_index_lgm(struct cbm_lookup *info)
{
	unsigned long sys_flag;
	void *qidt = cqm_ctrl->qidt0;

	if (info->egflag)
		qidt = cqm_ctrl->qidt1;

	spin_lock_irqsave(&cqm_qidt_lock[info->egflag], sys_flag);
	cqm_qid_program(info->qid, info->index, info->egflag);
	spin_unlock_irqrestore(&cqm_qidt_lock[info->egflag], sys_flag);
}

static u8 get_lookup_qid_via_idx_lgm(struct cbm_lookup *info)
{
	u32 offset = (info->index / 2) * 4;
	u32 shift = (info->index % 2) * 9;
	unsigned long sys_flag;
	u8 value = 0;
	void *qidt = cqm_ctrl->qidt0;

	if (info->egflag)
		qidt = cqm_ctrl->qidt1;

	spin_lock_irqsave(&cqm_qidt_lock[info->egflag], sys_flag);
	value = ((cbm_r32(qidt + offset)) >> shift) & LGM_CQM_Q_MASK;
	spin_unlock_irqrestore(&cqm_qidt_lock[info->egflag], sys_flag);
	return value;
}

static void map_to_drop_q(struct cbm_lookup_entry *lu_entry)
{
	unsigned long sys_flag;
	u8 value = 0;
	int i = 0, j = 0;
	u32 offset, shift;
	void *qidt = cqm_ctrl->qidt0;
	u32 lookup_tbl_size =
		MAX_LOOKUP_TBL_SIZE / MAX_LOOKUP_TBL;

	for (j = 0; j < MAX_LOOKUP_TBL; j++) {
		if (j)
			qidt = cqm_ctrl->qidt1;
		spin_lock_irqsave(&cqm_qidt_lock[j], sys_flag);
		/* In LGM there are 2k lines per table
		 * and each line 2 entries
		 */
		for (i = 0; i < lookup_tbl_size / 2; i++) {
			offset = (i / 2) * 4;
			shift = (i % 2) * 9;
			value = ((cbm_r32(qidt + offset)) >> shift) &
				LGM_CQM_Q_MASK;
			/* if match, set the index to drop q
			 * Return the entry and egflag
			 */
			if (lu_entry->qid == value) {
				lu_entry->entry[j * lookup_tbl_size +
						lu_entry->num[j]++] = i;
				lu_entry->egflag[j] = j;
				cqm_qid_program(lu_entry->ppv4_drop_q, i, j);
			}
		}
		spin_unlock_irqrestore(&cqm_qidt_lock[j], sys_flag);
	}
}

static void restore_orig_q(const struct cbm_lookup_entry *lu)
{
	unsigned long sys_flag;
	int i = 0, j = 0;
	u32 lookup_tbl_size =  MAX_LOOKUP_TBL_SIZE / MAX_LOOKUP_TBL;

	for (j = 0; j < MAX_LOOKUP_TBL; j++) {
		spin_lock_irqsave(&cqm_qidt_lock[j], sys_flag);

		for (i = 0; i < lu->num[j]; i++)
			cqm_qid_program(lu->qid,
					lu->entry[j * lookup_tbl_size + i],
					lu->egflag[j]);

		spin_unlock_irqrestore(&cqm_qidt_lock[j], sys_flag);
	}
}

static inline void handle_state(int *state, int nxt_state, int done_state,
				u32 limit, u8 *done, u32 value, u32 mask,
				u32 *qidt_value, u32 *idx)
{
	*state = nxt_state;
	if (*done) {
		*state = done_state;
		*done = 0;
		*idx = 0;
		return;
	}
	if ((mask) && (!(*done))) {
		*qidt_value = *idx;
		*idx += 1;
		if (*idx >= limit)
			*done = 1;
	} else {
		*qidt_value = value;
		*done = 1;
	}
}

static s32 qid2ep_map_set(int qid, int port)
{
	cbm_w32(cqm_ctrl->qid2ep + (qid * 4), port);
	return CBM_SUCCESS;
}

static s32 qid2ep_map_get(int qid, int *port)
{
	*port = cbm_r32(cqm_ctrl->qid2ep + (qid * 4));
	return CBM_SUCCESS;
}

static s32 gpid_lpid_map(struct cbm_gpid_lpid  *map)
{
	u32 regoffset, offset, mask;
	void *c_base = cqm_ctrl->cqm + CBM_GPID_LPID_MAP_0;

	regoffset = (map->gpid & (~0x7)) >> 1;
	offset = (map->gpid % 8) * 4;
	mask = CBM_GPID_LPID_MAP_0_PORTID0_MASK << offset;
	set_val(c_base + regoffset, map->lpid, mask, offset);
	return CBM_SUCCESS;
}

u32 epon_mode_reg_get(void)
{
	return cbm_r32(cqm_ctrl->enq + EPON_EPON_MODE_REG);
}

static s32 mode_table_get(int cbm_inst, int *mode,
			  cbm_queue_map_entry_t *entry, u32 flags)
{
	u32 ep, reg_value, offset, regoffset;
	void *c_base = cqm_ctrl->cqm;

	ep = entry->ep;
	regoffset = ((ep <= 7) ?
		     CBM_QID_MODE_SEL_REG_0 :
		     CBM_QID_MODE_SEL_REG_1);

	ep = entry->ep;
	offset = (CBM_QID_MODE_SEL_REG_0_MODE1_POS * ep);
	reg_value = cbm_r32(c_base + regoffset);
	*mode = (reg_value >> offset) & CBM_QID_MODE_SEL_REG_0_MODE0_MASK;
	return CBM_SUCCESS;
}

static s32 mode_table_set(int cbm_inst, cbm_queue_map_entry_t *entry,
			  u32 mode, u32 flags)
{
	u32 ep, mask, offset, regoffset;
	void *c_base = cqm_ctrl->cqm;

	ep = entry->ep;
	regoffset = ((ep <= 7) ?
		     CBM_QID_MODE_SEL_REG_0 :
		     CBM_QID_MODE_SEL_REG_1);

	offset = (CBM_QID_MODE_SEL_REG_0_MODE1_POS * ep);
	mask = CBM_QID_MODE_SEL_REG_0_MODE0_MASK << offset;
	set_val(c_base + regoffset, mode, mask, offset);
	return CBM_SUCCESS;
}

static void cqm_qid_reg_set(struct cqm_qidt_elm *qidt_elm, u16 qid_val,
			    u8 sel_field)
{
	u32 qidt;

	qidt = (((qidt_elm->ep & 0xf) << LGM_EP_POS) |
		(sel_field & 0xff));
	cqm_qid_program(qid_val, qidt, qidt_elm->egflag);
}

static inline void find_next_sub_if_id(u32 *idx_p, u32 mask, u32 mask_all,
				       u32 limit_v, u32 start_v)
{
	if (*idx_p < start_v)
		*idx_p = start_v;
	if (mask && (*idx_p) && mask_all) {
		while ((start_v != ((*idx_p) & (~mask))) &&
		       ((*idx_p) < limit_v)) {
			*idx_p =  *idx_p + 1;
		}
	}
}

static inline void sub_if_id_masking(u32 sub_if_id, u32 *st, u32 *lim,
				     u32 in_mask, u32 *bit_mask)
{
	if (in_mask & 1) {
		*bit_mask = in_mask >> SUB_IF_ID_MASK_SHF;
		if (*bit_mask) {
			*st = sub_if_id & (~(*bit_mask));
			*lim = ((*st) | (*bit_mask)) + 1;
		}
	}
}

static void
NO_OPTIMIZE cqm_qidt_set_mode0(const struct cqm_qidt_elm *qid_set,
			       const struct cqm_qidt_mask *qid_mask,
			       u16 qid_val)
{
	struct cqm_qidt_elm qidt_idx = {0};
	struct cqm_qidt_elm qidt_elm = {0};
	int state;
	struct qidt_flag_done flag_done = {0};
	u32 limit = MODE0_SUB_IF_ID_LIMIT, masking_bits = 0, start = 0;
	u32 sub_if_id_bit_mask =
		((qid_mask->sub_if_id_mask >> SUB_IF_ID_MODE_0_POS) &
		(SUB_IF_ID_MODE_0_MASK << SUB_IF_ID_MASK_SHF)) |
		(qid_mask->sub_if_id_mask & 0x01);
	u8 sel_field = 0;

	CQM_DEBUG(CQM_DBG_FLAG_QID_MAP,
		  "%s qid_val:%u, set:%u %u %u, mask:%x %x %x\n",
		  __func__,
		  qid_val, qid_set->clsid, qid_set->ep,
		  qid_set->sub_if_id, qid_mask->classid_mask,
		  qid_mask->ep_mask, qid_mask->sub_if_id_mask);

	qidt_elm.egflag = qid_set->egflag;
	state = STATE_CLASS;

	sub_if_id_masking(((qid_set->sub_if_id >> SUB_IF_ID_MODE_0_POS) &
			  SUB_IF_ID_MODE_0_MASK), &start, &limit,
			  sub_if_id_bit_mask, &masking_bits);
	qidt_elm.sub_if_id = start;
	qidt_idx.sub_if_id = start;

	while (state != STATE_NONE) {
		switch (state) {
		case STATE_CLASS:
			handle_state(&state, STATE_SUBIF_ID, STATE_NONE, 4,
				     &flag_done.cls_done, qid_set->clsid & 0x3,
				     qid_mask->classid_mask, &qidt_elm.clsid,
				     &qidt_idx.clsid);
		break;
		case STATE_SUBIF_ID:
			handle_state(&state, STATE_EP, STATE_CLASS, limit,
				     &flag_done.sub_if_id_done,
				     (qid_set->sub_if_id >> 8) & 0x3f,
				     qid_mask->sub_if_id_mask,
				     &qidt_elm.sub_if_id,
				     &qidt_idx.sub_if_id);
			sel_field = ((qidt_elm.sub_if_id << 2) |
				    (qidt_elm.clsid & 0x3));

			find_next_sub_if_id(&qidt_idx.sub_if_id, masking_bits,
					    (qid_mask->sub_if_id_mask & 1),
					    limit, start);
		break;
		case STATE_EP:
			handle_state(&state, STATE_EP, STATE_SUBIF_ID, 16,
				     &flag_done.ep_done, qid_set->ep & 0xf,
				     qid_mask->ep_mask, &qidt_elm.ep,
				     &qidt_idx.ep);
			if (!flag_done.ep_done) {
				cqm_qid_reg_set(&qidt_elm, qid_val, sel_field);
				if (qid_mask->egflag_mask) {
					qidt_elm.egflag = !qidt_elm.egflag;
					cqm_qid_reg_set(&qidt_elm, qid_val,
							sel_field);
				}
			}
		break;
		case STATE_NONE:
		break;
		};
	}
}

static void
NO_OPTIMIZE cqm_qidt_set_mode1(const struct cqm_qidt_elm *qid_set,
			       const struct cqm_qidt_mask *qid_mask,
			       u16 qid_val)
{
	struct cqm_qidt_elm qidt_idx = {0};
	struct cqm_qidt_elm qidt_elm = {0};
	int state;
	struct qidt_flag_done flag_done = {0};
	u32 limit = MODE1_SUB_IF_ID_LIMIT, masking_bits = 0, start = 0;
	u32 sub_if_id_bit_mask =
		((qid_mask->sub_if_id_mask >> SUB_IF_ID_MODE_1_POS) &
		(SUB_IF_ID_MODE_1_MASK << SUB_IF_ID_MASK_SHF)) |
		(qid_mask->sub_if_id_mask & 0x01);
	u8 sel_field = 0;

	CQM_DEBUG(CQM_DBG_FLAG_QID_MAP,
		  "%s qid_val:%u, set: %u %u, mask: %x %x\n",
		  __func__, qid_val,
		  qid_set->ep, qid_set->sub_if_id,
		  qid_mask->ep_mask, qid_mask->sub_if_id_mask);

	state = STATE_SUBIF_ID;
	qidt_elm.egflag = qid_set->egflag;

	sub_if_id_masking(((qid_set->sub_if_id >> SUB_IF_ID_MODE_1_POS) &
			  SUB_IF_ID_MODE_1_MASK), &start, &limit,
			  sub_if_id_bit_mask, &masking_bits);

	qidt_elm.sub_if_id = start;
	qidt_idx.sub_if_id = start;

	while (state != STATE_NONE) {
		switch (state) {
		case STATE_SUBIF_ID:
			handle_state(&state, STATE_EP, STATE_NONE, limit,
				     &flag_done.sub_if_id_done,
				     (qid_set->sub_if_id & 0xff),
				     qid_mask->sub_if_id_mask,
				     &qidt_elm.sub_if_id, &qidt_idx.sub_if_id);
			sel_field = qidt_elm.sub_if_id;

			find_next_sub_if_id(&qidt_idx.sub_if_id, masking_bits,
					    (qid_mask->sub_if_id_mask & 1),
					    limit, start);
		break;
		case STATE_EP:
			handle_state(&state, STATE_EP, STATE_SUBIF_ID, 16,
				     &flag_done.ep_done, qid_set->ep & 0xf,
				     qid_mask->ep_mask, &qidt_elm.ep,
				     &qidt_idx.ep);
			if (!flag_done.ep_done) {
				cqm_qid_reg_set(&qidt_elm, qid_val, sel_field);
				if (qid_mask->egflag_mask) {
					qidt_elm.egflag = !qidt_elm.egflag;
					cqm_qid_reg_set(&qidt_elm,
							qid_val, sel_field);
				}
			}
		break;
		case STATE_NONE:
		break;
		};
	}
}

static void
NO_OPTIMIZE cqm_qidt_set_mode2(const struct cqm_qidt_elm *qid_set,
			       const struct cqm_qidt_mask *qid_mask,
			       u16 qid_val)
{
	struct cqm_qidt_elm qidt_idx = {0};
	struct cqm_qidt_elm qidt_elm = {0};
	int state;
	struct qidt_flag_done flag_done = {0};
	u32 limit = MODE2_SUB_IF_ID_LIMIT, masking_bits = 0, start = 0;
	u32 sub_if_id_bit_mask =
		((qid_mask->sub_if_id_mask >> SUB_IF_ID_MODE_2_POS) &
		(SUB_IF_ID_MODE_2_MASK << SUB_IF_ID_MASK_SHF)) |
		(qid_mask->sub_if_id_mask & 0x01);
	u8 sel_field = 0;

	CQM_DEBUG(CQM_DBG_FLAG_QID_MAP,
		  "%s qid_val:%u, set:%u %u %u, mask:0x%x 0x%x 0x%x\n",
		  __func__,
		  qid_val, qid_set->clsid, qid_set->ep,
		  qid_set->sub_if_id, qid_mask->classid_mask,
		  qid_mask->ep_mask, qid_mask->sub_if_id_mask);

	state = STATE_CLASS;
	qidt_elm.egflag = qid_set->egflag;

	sub_if_id_masking(((qid_set->sub_if_id >> SUB_IF_ID_MODE_2_POS) &
			  SUB_IF_ID_MODE_2_MASK), &start, &limit,
			  sub_if_id_bit_mask, &masking_bits);

	qidt_elm.sub_if_id = start;
	qidt_idx.sub_if_id = start;

	while (state != STATE_NONE) {
		switch (state) {
		case STATE_CLASS:
			handle_state(&state, STATE_SUBIF_ID, STATE_NONE, 16,
				     &flag_done.cls_done, qid_set->clsid & 0xf,
				     qid_mask->classid_mask, &qidt_elm.clsid,
				     &qidt_idx.clsid);
		break;
		case STATE_SUBIF_ID:
			handle_state(&state, STATE_EP, STATE_CLASS, limit,
				     &flag_done.sub_if_id_done,
				     (qid_set->sub_if_id >> 8) & 0xf,
				     qid_mask->sub_if_id_mask & 1,
				     &qidt_elm.sub_if_id,
				     &qidt_idx.sub_if_id);

			sel_field = ((qidt_elm.sub_if_id << 4) |
				    (qidt_elm.clsid & 0xf));

			find_next_sub_if_id(&qidt_idx.sub_if_id, masking_bits,
					    (qid_mask->sub_if_id_mask & 1),
					    limit, start);
		break;
		case STATE_EP:
			handle_state(&state, STATE_EP, STATE_SUBIF_ID, 16,
				     &flag_done.ep_done, qid_set->ep & 0xf,
				     qid_mask->ep_mask, &qidt_elm.ep,
				     &qidt_idx.ep);
			if (!flag_done.ep_done) {
				cqm_qid_reg_set(&qidt_elm, qid_val, sel_field);
				if (qid_mask->egflag_mask) {
					qidt_elm.egflag = !qidt_elm.egflag;
					cqm_qid_reg_set(&qidt_elm,
							qid_val, sel_field);
				}
			}
		break;
		case STATE_NONE:
		break;
		};
	}
}

static void
NO_OPTIMIZE cqm_qidt_set_mode3(const struct cqm_qidt_elm *qid_set,
			       const struct cqm_qidt_mask *qid_mask,
			       u16 qid_val)
{
	struct cqm_qidt_elm qidt_idx = {0};
	struct cqm_qidt_elm qidt_elm = {0};
	int state;
	struct qidt_flag_done flag_done = {0};
	u8 sel_field = 0;
	u32 limit = MODE3_SUB_IF_ID_LIMIT, masking_bits = 0, start = 0;
	u32 sub_if_id_bit_mask =
		((qid_mask->sub_if_id_mask >> SUB_IF_ID_MODE_3_POS) &
		(SUB_IF_ID_MODE_3_MASK << SUB_IF_ID_MASK_SHF)) |
		(qid_mask->sub_if_id_mask & 0x01);

	CQM_DEBUG(CQM_DBG_FLAG_QID_MAP,
		  "%s qid_val:%u, set:%u %u %u, mask:%x %x %x\n",
		  __func__,
		  qid_val, qid_set->clsid, qid_set->ep,
		  qid_set->sub_if_id, qid_mask->classid_mask,
		  qid_mask->ep_mask, qid_mask->sub_if_id_mask);
	state = STATE_CLASS;
	qidt_elm.egflag = qid_set->egflag;

	sub_if_id_masking(((qid_set->sub_if_id >> SUB_IF_ID_MODE_3_POS) &
			  SUB_IF_ID_MODE_3_MASK), &start, &limit,
			  sub_if_id_bit_mask, &masking_bits);

	qidt_elm.sub_if_id = start;
	qidt_idx.sub_if_id = start;

	while (state != STATE_NONE) {
		switch (state) {
		case STATE_CLASS:
			handle_state(&state, STATE_SUBIF_ID, STATE_NONE, 8,
				     &flag_done.cls_done, qid_set->clsid,
				     qid_mask->classid_mask, &qidt_elm.clsid,
				     &qidt_idx.clsid);
		break;
		case STATE_SUBIF_ID:
			handle_state(&state, STATE_EP, STATE_CLASS, limit,
				     &flag_done.sub_if_id_done,
				     qid_set->sub_if_id & 0x1f,
				     qid_mask->sub_if_id_mask,
				     &qidt_elm.sub_if_id,
				     &qidt_idx.sub_if_id);
			sel_field = ((qidt_elm.sub_if_id << 3) |
				    (qidt_elm.clsid & 0x7));

			find_next_sub_if_id(&qidt_idx.sub_if_id, masking_bits,
					    (qid_mask->sub_if_id_mask & 1),
					    limit, start);
		break;
		case STATE_EP:
			handle_state(&state, STATE_EP, STATE_SUBIF_ID, 16,
				     &flag_done.ep_done, qid_set->ep,
				     qid_mask->ep_mask, &qidt_elm.ep,
				     &qidt_idx.ep);
			if (!flag_done.ep_done) {
				cqm_qid_reg_set(&qidt_elm, qid_val, sel_field);
				if (qid_mask->egflag_mask) {
					qidt_elm.egflag = !qidt_elm.egflag;
					cqm_qid_reg_set(&qidt_elm,
							qid_val, sel_field);
				}
			}
		break;
		case STATE_NONE:
		break;
		};
	}
}

static void
NO_OPTIMIZE cqm_qidt_set_mode4(const struct cqm_qidt_elm *qid_set,
			       const struct cqm_qidt_mask *qid_mask,
			       u16 qid_val)
{
	struct cqm_qidt_elm qidt_idx = {0};
	struct cqm_qidt_elm qidt_elm = {0};
	int state;
	struct qidt_flag_done flag_done = {0};
	u8 sel_field = 0;
	u32 limit = MODE4_SUB_IF_ID_LIMIT, masking_bits = 0, start = 0;
	u32 sub_if_id_bit_mask =
		((qid_mask->sub_if_id_mask >> SUB_IF_ID_MODE_4_POS) &
		(SUB_IF_ID_MODE_4_MASK << SUB_IF_ID_MASK_SHF)) |
		(qid_mask->sub_if_id_mask & 0x01);

	CQM_DEBUG(CQM_DBG_FLAG_QID_MAP,
		  "%s qid_val:%u, set:%u %u %u, mask:%x %x %x\n",
		  __func__,
		  qid_val, qid_set->clsid, qid_set->ep,
		  qid_set->sub_if_id, qid_mask->classid_mask,
		  qid_mask->ep_mask, qid_mask->sub_if_id_mask);
	state = STATE_SUBIF_ID;
	qidt_elm.egflag = qid_set->egflag;

	sub_if_id_masking(((qid_set->sub_if_id >> SUB_IF_ID_MODE_4_POS) &
			  SUB_IF_ID_MODE_4_MASK), &start, &limit,
			  sub_if_id_bit_mask, &masking_bits);

	qidt_elm.sub_if_id = start;
	qidt_idx.sub_if_id = start;

	while (state != STATE_NONE) {
		switch (state) {
		case STATE_SUBIF_ID:
			handle_state(&state, STATE_CLASS, STATE_NONE, limit,
				     &flag_done.sub_if_id_done,
				     qid_set->sub_if_id & 0x3f,
				     qid_mask->sub_if_id_mask,
				     &qidt_elm.sub_if_id,
				     &qidt_idx.sub_if_id);

		find_next_sub_if_id(&qidt_idx.sub_if_id, masking_bits,
				    (qid_mask->sub_if_id_mask & 1),
				    limit, start);
		break;
		case STATE_CLASS:
			handle_state(&state, STATE_EP, STATE_SUBIF_ID, 4,
				     &flag_done.cls_done, qid_set->clsid & 0x3,
				     qid_mask->classid_mask, &qidt_elm.clsid,
				     &qidt_idx.clsid);
			sel_field = ((qidt_elm.clsid << 6) |
				    (qidt_elm.sub_if_id & 0x3f));
		break;
		case STATE_EP:
			handle_state(&state, STATE_EP, STATE_CLASS, 16,
				     &flag_done.ep_done, qid_set->ep & 0xf,
				     qid_mask->ep_mask, &qidt_elm.ep,
				     &qidt_idx.ep);
			if (!flag_done.ep_done) {
				cqm_qid_reg_set(&qidt_elm, qid_val, sel_field);
				if (qid_mask->egflag_mask) {
					qidt_elm.egflag = !qidt_elm.egflag;
					cqm_qid_reg_set(&qidt_elm,
							qid_val, sel_field);
				}
			}
		break;
		case STATE_NONE:
		break;
		};
	}
}

static void
NO_OPTIMIZE cqm_qidt_set_mode5(const struct cqm_qidt_elm *qid_set,
			       const struct cqm_qidt_mask *qid_mask,
			       u16 qid_val)
{
	struct cqm_qidt_elm qidt_idx = {0};
	struct cqm_qidt_elm qidt_elm = {0};
	int state;
	struct qidt_flag_done flag_done = {0};
	u32 limit = MODE5_SUB_IF_ID_LIMIT, masking_bits = 0, start = 0;
	u8 sel_field = 0;
	u32 sub_if_id_bit_mask =
		((qid_mask->sub_if_id_mask >> SUB_IF_ID_MODE_5_POS) &
		(SUB_IF_ID_MODE_5_MASK << SUB_IF_ID_MASK_SHF)) |
		(qid_mask->sub_if_id_mask & 0x01);

	CQM_DEBUG(CQM_DBG_FLAG_QID_MAP,
		  "%s qid_val:%u, set:%u %u, mask:%x %x\n",
		  __func__, qid_val, qid_set->ep, qid_set->sub_if_id,
		  qid_mask->ep_mask, qid_mask->sub_if_id_mask);
	state = STATE_SUBIF_ID;
	qidt_elm.egflag = qid_set->egflag;

	sub_if_id_masking(((qid_set->sub_if_id >> SUB_IF_ID_MODE_5_POS) &
			  SUB_IF_ID_MODE_5_MASK), &start, &limit,
			  sub_if_id_bit_mask, &masking_bits);

	qidt_elm.sub_if_id = start;
	qidt_idx.sub_if_id = start;
	while (state != STATE_NONE) {
		switch (state) {
		case STATE_SUBIF_ID:
			handle_state(&state, STATE_EP, STATE_NONE, limit,
				     &flag_done.sub_if_id_done,
				     (qid_set->sub_if_id >> 8) & 0xff,
				     qid_mask->sub_if_id_mask,
				     &qidt_elm.sub_if_id, &qidt_idx.sub_if_id);
			sel_field = qidt_elm.sub_if_id;

			find_next_sub_if_id(&qidt_idx.sub_if_id, masking_bits,
					    (qid_mask->sub_if_id_mask & 1),
					    limit, start);
		break;
		case STATE_EP:
			handle_state(&state, STATE_EP, STATE_SUBIF_ID, 16,
				     &flag_done.ep_done, qid_set->ep & 0xf,
				     qid_mask->ep_mask, &qidt_elm.ep,
				     &qidt_idx.ep);
			if (!flag_done.ep_done) {
				cqm_qid_reg_set(&qidt_elm, qid_val, sel_field);
				if (qid_mask->egflag_mask) {
					qidt_elm.egflag = !qidt_elm.egflag;
					cqm_qid_reg_set(&qidt_elm,
							qid_val, sel_field);
				}
			}
		break;
		case STATE_NONE:
		break;
		};
	}
}

static void
NO_OPTIMIZE cqm_qidt_set_mode6(const struct cqm_qidt_elm *qid_set,
			       const struct cqm_qidt_mask *qid_mask,
			       u16 qid_val)
{
	struct cqm_qidt_elm qidt_idx = {0};
	struct cqm_qidt_elm qidt_elm = {0};
	int state;
	struct qidt_flag_done flag_done = {0};
	u32 limit = MODE6_SUB_IF_ID_LIMIT, masking_bits = 0, start = 0;
	u32 sub_if_id_bit_mask =
		((qid_mask->sub_if_id_mask >> SUB_IF_ID_MODE_6_POS) &
		(SUB_IF_ID_MODE_6_MASK << SUB_IF_ID_MASK_SHF)) |
		(qid_mask->sub_if_id_mask & 0x01);
	u8 sel_field = 0;

	CQM_DEBUG(CQM_DBG_FLAG_QID_MAP,
		  "%s qid_val:%u, set:%u %u %u %u, mask:%x %x %x %x\n",
		  __func__, qid_val, qid_set->clsid, qid_set->ep,
		  qid_set->sub_if_id, qid_set->color,
		  qid_mask->classid_mask,
		  qid_mask->ep_mask, qid_mask->sub_if_id_mask,
		  qid_mask->color_mask);
	state = STATE_COLOR;
	qidt_elm.egflag = qid_set->egflag;

	sub_if_id_masking(((qid_set->sub_if_id >> SUB_IF_ID_MODE_6_POS) &
			  SUB_IF_ID_MODE_6_MASK), &start, &limit,
			  sub_if_id_bit_mask, &masking_bits);

	qidt_elm.sub_if_id = start;
	qidt_idx.sub_if_id = start;
	while (state != STATE_NONE) {
		switch (state) {
		case STATE_COLOR:
			handle_state(&state, STATE_CLASS, STATE_NONE, 4,
				     &flag_done.color_done,
				     qid_set->color & 0x3,
				     qid_mask->color_mask,
				     &qidt_elm.color,
				     &qidt_idx.color);
		break;
		case STATE_CLASS:
			handle_state(&state, STATE_SUBIF_ID, STATE_COLOR, 16,
				     &flag_done.cls_done, qid_set->clsid & 0xf,
				     qid_mask->classid_mask, &qidt_elm.clsid,
				     &qidt_idx.clsid);
		break;
		case STATE_SUBIF_ID:
			handle_state(&state, STATE_EP, STATE_CLASS, 4,
				     &flag_done.sub_if_id_done,
				     qid_set->sub_if_id & 0x3,
				     qid_mask->sub_if_id_mask,
				     &qidt_elm.sub_if_id,
				     &qidt_idx.sub_if_id);
			sel_field = ((qidt_elm.clsid << 2) |
				     (qidt_elm.color & 0x3) |
				     ((qidt_elm.sub_if_id & 0x3) << 6));

			find_next_sub_if_id(&qidt_idx.sub_if_id, masking_bits,
					    (qid_mask->sub_if_id_mask & 1),
					    limit, start);
		break;
		case STATE_EP:
			handle_state(&state, STATE_EP, STATE_SUBIF_ID, 16,
				     &flag_done.ep_done, qid_set->ep & 0xf,
				     qid_mask->ep_mask, &qidt_elm.ep,
				     &qidt_idx.ep);
			if (!flag_done.ep_done) {
				cqm_qid_reg_set(&qidt_elm, qid_val, sel_field);
				if (qid_mask->egflag_mask) {
					qidt_elm.egflag = !qidt_elm.egflag;
					cqm_qid_reg_set(&qidt_elm,
							qid_val, sel_field);
				}
			}
		break;
		case STATE_NONE:
		break;
		};
	}
}

static void
NO_OPTIMIZE cqm_qidt_set_mode7(const struct cqm_qidt_elm *qid_set,
			       const struct cqm_qidt_mask *qid_mask,
			       u16 qid_val)
{
	struct cqm_qidt_elm qidt_idx = {0};
	struct cqm_qidt_elm qidt_elm = {0};
	int state;
	struct qidt_flag_done flag_done = {0};
	u32 limit = MODE7_SUB_IF_ID_LIMIT, masking_bits = 0, start = 0;
	u32 sub_if_id_bit_mask =
		((qid_mask->sub_if_id_mask >> SUB_IF_ID_MODE_7_POS) &
		(SUB_IF_ID_MODE_7_MASK << SUB_IF_ID_MASK_SHF)) |
		(qid_mask->sub_if_id_mask & 0x01);
	u8 sel_field = 0;

	CQM_DEBUG(CQM_DBG_FLAG_QID_MAP,
		  "%s qid_val:%u, set:%u %u %u, mask:%x %x %x\n",
		  __func__,
		  qid_val, qid_set->clsid, qid_set->ep,
		  qid_set->sub_if_id, qid_mask->classid_mask,
		  qid_mask->ep_mask, qid_mask->sub_if_id_mask);
	state = STATE_CLASS;
	qidt_elm.egflag = qid_set->egflag;

	sub_if_id_masking(((qid_set->sub_if_id >> SUB_IF_ID_MODE_7_POS) &
			  SUB_IF_ID_MODE_7_MASK), &start, &limit,
			  sub_if_id_bit_mask, &masking_bits);

	qidt_elm.sub_if_id = start;
	qidt_idx.sub_if_id = start;

	while (state != STATE_NONE) {
		switch (state) {
		case STATE_CLASS:
			handle_state(&state, STATE_SUBIF_ID, STATE_NONE, 2,
				     &flag_done.cls_done, qid_set->clsid & 0x1,
				     qid_mask->classid_mask, &qidt_elm.clsid,
				     &qidt_idx.clsid);
		break;
		case STATE_SUBIF_ID:
			handle_state(&state, STATE_EP, STATE_CLASS, limit,
				     &flag_done.sub_if_id_done,
				     (qid_set->sub_if_id >> 8) & 0x7f,
				     qid_mask->sub_if_id_mask,
				     &qidt_elm.sub_if_id,
				     &qidt_idx.sub_if_id);
			sel_field = ((qidt_elm.sub_if_id << 1) |
				    (qidt_elm.clsid & 0x1));

			find_next_sub_if_id(&qidt_idx.sub_if_id, masking_bits,
					    (qid_mask->sub_if_id_mask & 1),
					    limit, start);
		break;
		case STATE_EP:
			handle_state(&state, STATE_EP, STATE_SUBIF_ID, 16,
				     &flag_done.ep_done, qid_set->ep & 0xf,
				     qid_mask->ep_mask, &qidt_elm.ep,
				     &qidt_idx.ep);
			if (!flag_done.ep_done) {
				cqm_qid_reg_set(&qidt_elm, qid_val, sel_field);
				if (qid_mask->egflag_mask) {
					qidt_elm.egflag = !qidt_elm.egflag;
					cqm_qid_reg_set(&qidt_elm,
							qid_val, sel_field);
				}
			}
		break;
		case STATE_NONE:
		break;
		};
	}
}

static void
NO_OPTIMIZE cqm_qidt_set(const struct cqm_qidt_elm *qid_set,
			 const struct cqm_qidt_mask *qid_mask,
			 u16 qid_val)
{
	int mode;
	cbm_queue_map_entry_t entry = {0};

	entry.ep = qid_set->ep;
	mode_table_get(0, &mode, &entry, 0);
	switch (mode) {
	case 0:
		cqm_qidt_set_mode0(qid_set, qid_mask, qid_val);
	break;
	case 1:
		cqm_qidt_set_mode1(qid_set, qid_mask, qid_val);
	break;
	case 2:
		cqm_qidt_set_mode2(qid_set, qid_mask, qid_val);
	break;
	case 3:
		cqm_qidt_set_mode3(qid_set, qid_mask, qid_val);
	break;
	case 4:
		cqm_qidt_set_mode4(qid_set, qid_mask, qid_val);
	break;
	case 5:
		cqm_qidt_set_mode5(qid_set, qid_mask, qid_val);
	break;
	case 6:
		cqm_qidt_set_mode6(qid_set, qid_mask, qid_val);
	break;
	case 7:
		cqm_qidt_set_mode7(qid_set, qid_mask, qid_val);
	break;
	default:
		dev_err(cqm_ctrl->dev, "unknown mode\n");
	break;
	}
}

s32 NO_OPTIMIZE queue_map_get(int cbm_inst, s32 queue_id, s32 *num_entries,
			      cbm_queue_map_entry_t **entries, u32 flags)
{
	int i = 0, j = 0, k = 0;
	u32 index, mode, sel_field, egflag;
	unsigned long sys_flag;
	cbm_queue_map_entry_t *temp_entry;
	u32 *ptr;

	egflag = flags;
	spin_lock_irqsave(&cqm_qidt_lock[egflag], sys_flag);
	if (queue_id >= 0 && queue_id <= 511) {
		for (i = 0, k = i; i < CQM_QIDT_DW_NUM; i += 1, k += 2) {
			/*unrolling the loop*/
			ptr = &g_cbm_qidt_mirror[egflag][i].qidt_shadow;
			if ((*ptr & LGM_CQM_Q_MASK) == queue_id) {
				g_qidt_help[egflag][j] = k;
				j++;
			}
			if (((*ptr >> LGM_CQM_Q_SHIFT) &
			    LGM_CQM_Q_MASK) == queue_id) {
				g_qidt_help[egflag][j] = k + 1;
				j++;
			}
		}
	} else {
		spin_unlock_irqrestore(&cqm_qidt_lock[egflag], sys_flag);
		return CBM_FAILURE;
	}
	temp_entry = kzalloc(sizeof(cbm_queue_map_entry_t) * (j),
			     GFP_ATOMIC);
	if (!temp_entry) {
		spin_unlock_irqrestore(&cqm_qidt_lock[egflag], sys_flag);
		return CBM_FAILURE;
	}
	*entries = temp_entry;
	*num_entries = j;
	for (i = 0; i < j; i++) {
		index = g_qidt_help[egflag][i];
		temp_entry[i].ep = (index & 0xF00) >> LGM_EP_POS;
		mode_table_get(0, &mode, &temp_entry[i], 0);
		sel_field = index & 0xff;
		switch (mode) {
		case 0:
			temp_entry[i].tc = sel_field & 0x3;
			temp_entry[i].sub_if_id = sel_field & 0xfc;
		break;
		case 1:
		case 5:
			temp_entry[i].sub_if_id = sel_field;
		break;
		case 2:
			temp_entry[i].tc = (sel_field & 0x0F);
			temp_entry[i].sub_if_id = ((sel_field & 0xf0) >> 4);
		break;
		case 3:
			temp_entry[i].tc = (sel_field & 0x07);
			temp_entry[i].sub_if_id = ((sel_field & 0xf8) >> 3);
		break;
		case 4:
			temp_entry[i].sub_if_id = (sel_field & 0x3f);
			temp_entry[i].tc = (sel_field & 0xc0 >> 6);
		break;
		case 6:
			temp_entry[i].color = (sel_field & 0x3);
			temp_entry[i].tc = (sel_field & 0x3c) >> 2;
			temp_entry[i].sub_if_id = ((sel_field & 0xc0) >> 6);
		break;
		case 7:
			temp_entry[i].tc = (sel_field & 0x1);
			temp_entry[i].sub_if_id = ((sel_field & 0xfe) >> 7);
		break;
		default:
			dev_err(cqm_ctrl->dev, "unknown mode\n");
		break;
		}
	}
	spin_unlock_irqrestore(&cqm_qidt_lock[egflag], sys_flag);
	return CBM_SUCCESS;
}

s32 NO_OPTIMIZE queue_map_set(int cbm_inst, s32 queue_id,
			      cbm_queue_map_entry_t *entry, u32 flags)
{
	unsigned long sys_flag;
	struct cqm_qidt_elm qidt_elm = {0};
	struct cqm_qidt_mask qidt_mask = {0};
	u32 i;

	if (!entry)
		return CBM_FAILURE;
	qidt_elm.clsid = entry->tc;
	qidt_elm.ep = entry->ep;
	qidt_elm.mpe1 = entry->mpe1;
	qidt_elm.mpe2 = entry->mpe2;
	qidt_elm.enc = entry->enc;
	qidt_elm.dec = entry->dec;
	qidt_elm.flowidl = entry->flowid & 0x1;
	qidt_elm.flowidh = entry->flowid >> 1;
	qidt_elm.sub_if_id = entry->sub_if_id;
	qidt_elm.color = entry->color;

	if (flags & CBM_QUEUE_MAP_F_FLOWID_L_DONTCARE)
		qidt_mask.flowid_lmask = 1;
	if (flags & CBM_QUEUE_MAP_F_FLOWID_H_DONTCARE)
		qidt_mask.flowid_hmask = 1;
	if (flags & CBM_QUEUE_MAP_F_EN_DONTCARE)
		qidt_mask.enc_mask = 1;
	if (flags & CBM_QUEUE_MAP_F_DE_DONTCARE)
		qidt_mask.dec_mask = 1;
	if (flags & CBM_QUEUE_MAP_F_MPE1_DONTCARE)
		qidt_mask.mpe1_mask = 1;
	if (flags & CBM_QUEUE_MAP_F_MPE2_DONTCARE)
		qidt_mask.mpe2_mask = 1;
	if (flags & CBM_QUEUE_MAP_F_TC_DONTCARE)
		qidt_mask.classid_mask = 1;
	if (flags & CBM_QUEUE_MAP_F_SUBIF_DONTCARE) {
		qidt_mask.sub_if_id_mask = 1;
		qidt_mask.sub_if_id_mask |= (entry->sub_if_id_mask_bits
			<< SUB_IF_ID_MASK_SHF);
	}

	if (flags & CBM_QUEUE_MAP_F_COLOR_DONTCARE)
		qidt_mask.color_mask = 1;
	if (flags & CBM_QUEUE_MAP_F_EGFLAG_DONTCARE)
		qidt_mask.egflag_mask = 1;
	else
		qidt_elm.egflag = entry->egflag;

	spin_lock_irqsave(&cqm_qidt_lock[0], sys_flag);
	if (flags & CBM_QUEUE_MAP_F_EP_DONTCARE) {
		for (i = 0; i < MAX_QID_MAP_EP_NUM; i++) {
			qidt_elm.ep = i;
			cqm_qidt_set(&qidt_elm, &qidt_mask, queue_id);
		}
	} else {
		cqm_qidt_set(&qidt_elm, &qidt_mask, queue_id);
	}
	spin_unlock_irqrestore(&cqm_qidt_lock[0], sys_flag);
	return CBM_SUCCESS;
}

void queue_map_buf_free(cbm_queue_map_entry_t *entries)
{
	kfree(entries);
}

void *cqm_get_enq_phy_base(void)
{
	return cqm_ctrl->enq_phys;
}

void *cqm_get_deq_phy_base(void)
{
	return cqm_ctrl->deq_phys;
}

void *cqm_get_ls_phy_base(void)
{
	return cqm_ctrl->ls_phys;
}

void *cqm_get_ctrl_phy_base(void)
{
	return cqm_ctrl->cqm_phys;
}

void *cqm_get_enq_base(void)
{
	return cqm_ctrl->enq;
}

void *cqm_get_deq_base(void)
{
	return cqm_ctrl->deq;
}

void *cqm_get_dma_desc_base(void)
{
	return cqm_ctrl->dmadesc_128;
}

void *cqm_get_ctrl_base(void)
{
	return cqm_ctrl->cqm;
}

void *cqm_get_ls_base(void)
{
	return cqm_ctrl->ls;
}

void *cqm_get_dmadesc_64(void)
{
	return cqm_ctrl->dmadesc_64;
}

void *cqm_get_dmadesc_64_phys(void)
{
	return cqm_ctrl->dmadesc_64_phys;
}

struct eqm_pid_s *cqm_get_eqm_pid_cfg(void)
{
	return &cqm_ctrl->eqm_pid_cfg;
}

struct dqm_pid_s *cqm_get_dqm_pid_cfg(void)
{
	return &cqm_ctrl->dqm_pid_cfg;
}

struct cqm_lpid_config_s *cqm_get_lpid_config(void)
{
	return &cqm_ctrl->lpid_config;
}

struct deq_dma_delay_s *cqm_get_deq_dma_delay(void)
{
	return cqm_ctrl->deq_dma_delay;
}

void cqm_desc_get(struct cbm_desc *desc_p, u32 pid, u32 enq_flag)
{
	unsigned long flags;

	local_irq_save(flags);
	preempt_disable();
	if (enq_flag) {
		desc_p->desc0 = per_cpu(cqm_enq_desc.desc0, pid);
		desc_p->desc1 = per_cpu(cqm_enq_desc.desc1, pid);
		desc_p->desc2 = per_cpu(cqm_enq_desc.desc2, pid);
		desc_p->desc3 = per_cpu(cqm_enq_desc.desc3, pid);
	} else {
		desc_p->desc0 = per_cpu(cqm_deq_desc.desc0, pid);
		desc_p->desc1 = per_cpu(cqm_deq_desc.desc1, pid);
		desc_p->desc2 = per_cpu(cqm_deq_desc.desc2, pid);
		desc_p->desc3 = per_cpu(cqm_deq_desc.desc3, pid);
	}
	preempt_enable();
	local_irq_restore(flags);
}

struct cqm_eqm_port_info *cqm_get_eqp_info_p(u32 port_id)
{
	struct cqm_eqm_port_info *eqp_info = NULL;

	if (port_id < CQM_ENQ_PORT_MAX)
		eqp_info = &cqm_ctrl->eqm_port_info[port_id];
	return eqp_info;
}

struct cqm_dqm_port_info *cqm_get_dqp_info_p(u32 port_id)
{
	struct cqm_dqm_port_info *deqp_info = NULL;

	if (port_id < CQM_DEQ_PORT_MAX)
		deqp_info = &cqm_ctrl->dqm_port_info[port_id];
	return deqp_info;
}

static bool buf_in_pool(void *va, const struct cqm_bm_pool_config *cfg)
{
	return (unsigned long)va >= cfg->pool_start_low &&
		(unsigned long)va < cfg->pool_end_low;
}

static int get_buf_pool(void *va, const struct cqm_bm_pool_config *cfgs, u8 *pool_p)
{
	int pool, base_pool;

	if (!cqm_ctrl || !cfgs)
		return CBM_FAILURE;

	*pool_p = 0;
	base_pool = cqm_ctrl->cpu_base_pool;
	/* higher chance to find the buffer in the below pool first */
	for (pool = base_pool + 3; pool >= base_pool; pool--) {
		if (buf_in_pool(va, cfgs + pool)) {
			*pool_p = (u8)pool;
			return CBM_SUCCESS;
		}
	}

	for (pool = 0; pool < base_pool; pool++) {
		if (buf_in_pool(va, cfgs + pool)) {
			*pool_p = (u8)pool;
			return CBM_SUCCESS;
		}
	}

	for (pool = base_pool + 4; pool < CQM_LGM_TOTAL_BM_POOLS; pool++) {
		if (buf_in_pool(va, cfgs + pool)) {
			*pool_p = (u8)pool;
			return CBM_SUCCESS;
		}
	}

	return CBM_FAILURE;
}

void cqm_read_dma_desc_prep(int port, void **base_addr, int *num_desc)
{
	struct dma_chan *chan;

	/*Switch off the corresponding DMA TX channel (DMA register)*/
	chan = cqm_ctrl->dqm_port_info[port].pch;
	/* Disable DMA channels associated with CBM DQ Port*/
	if (chan && cqm_ctrl->dqm_port_info[port].dma_ch_in_use)
		dmaengine_pause(chan);
	usleep_range(20, 25);
	/*Disable read pointer auto-increment mode
	 *for that DMA channel(CQEM register)
	 */
	if (port < CQM_PON_IP_PORT_START) {
		set_val((cqm_ctrl->deq + DQ_DMA_PORT((port), cfg)), 1, 0x2, 1);
		*base_addr = cqm_ctrl->dmadesc_128 + CQM_DEQ_DMA_DESC(port, 0);
	} else {
		set_val(cqm_ctrl->pib + PIB_CTRL, 1, 0x4, 2);
		*base_addr = cqm_ctrl->dmadesc_128 +
			      CQM_DEQ_PON_DMA_DESC(0, 0);
	}
	*num_desc =  cqm_ctrl->dqm_port_info[port].deq_info.num_desc;
}

void cqm_read_dma_desc_end(int port)
{
	struct dma_chan *chan;

	/*Disable read pointer auto-increment mode
	 *for that DMA channel(CQEM register)
	 */
	if (port < CQM_PON_IP_PORT_START)
		set_val((cqm_ctrl->deq + DQ_DMA_PORT((port), cfg)), 0, 0x2, 1);
	else
		set_val(cqm_ctrl->pib + PIB_CTRL, 0, 0x4, 2);
	chan = cqm_ctrl->dqm_port_info[port].pch;
	/* Disable DMA channels associated with CBM DQ Port*/
	if (chan && cqm_ctrl->dqm_port_info[port].dma_ch_in_use)
		dma_async_issue_pending(chan);
}

struct hr_tr_tbl *get_hr_tr_tbl(u32 spl_flag)
{
	if (spl_flag)
		return hr_tr_spl;
	else
		return hr_tr_dp;
}

static inline void add_metadata(u8 *v_buf_addr, u8 pool,
				u8 policy)
{
	(v_buf_addr)[0] = 0xde;
	(v_buf_addr)[1] = pool;
	(v_buf_addr)[2] = policy;
	(v_buf_addr)[3] = 0xad;
}

static int get_metadata(u8 *v_buf_addr, u8 *pool, u8 *policy)
{
	if (((v_buf_addr)[0] == 0xde) &&
	    ((v_buf_addr)[3] == 0xad)) {
		*pool = (v_buf_addr)[1];
		*policy = (v_buf_addr)[2];
		return CBM_SUCCESS;
	}
	dev_warn(cqm_ctrl->dev, "Invalid METADATA: 0: %x 3: %x\n",
		v_buf_addr[0], v_buf_addr[3]);
	return CBM_FAILURE;
}

static int cqm_cpu_pool_enqueue_status(void)
{
	int i = 0;
	u32 reg;

	do {
		if (unlikely(cqm_ctrl->soc == URX851C && cqm_ctrl->cpu_enq_chk)) {
			reg = cbm_r32(cqm_ctrl->cqm + CBM_CPU_POOL_ENQ_CNT);
			reg = (reg & CBM_CPU_POOL_ENQ_CNT_STS_MASK) >>
				 CBM_CPU_POOL_ENQ_CNT_STS_POS;
		} else {
			reg = (atomic_read(&cqm_fe_policy.enq_cnt) < cqm_fe_policy.allow_cnt) ? 1 : 0;
		}
		i++;
		if (i == 10) {
			dev_err_once(cqm_ctrl->dev, "Error Enq CPU pool buff pkt reg 0x%x %d\n",
				     reg, atomic_read(&cqm_fe_policy.enq_cnt));
			return CBM_FAILURE;
		}
	} while (!reg);
	return CBM_SUCCESS;
}

static inline void cpu_enqueue_hw(u32 pid, struct cbm_desc *desc, u64 desc_qw0, u64 desc_qw1)
{
	unsigned long flags;

	local_irq_save(flags);
	preempt_disable();

	writeq(desc_qw0, eqmdesc[pid]);
	writeq(desc_qw1, eqmdesc[pid] + 8);

	preempt_enable();
	local_irq_restore(flags);
}

static int lgm_ioc_enable(void)
{
	if (!is_version2()) {
	#if IS_ENABLED(CONFIG_IOC_POOL) && IS_ENABLED(CONFIG_LGM_A0_IOC)
		return (1);
	#endif
	} else {
	#if IS_ENABLED(CONFIG_IOC_POOL)
		return (1);
	#endif
	}
	return 0;
}

unsigned long
NO_OPTIMIZE cqm_cpu_enqueue(u32 pid, struct cbm_desc *desc,
			    unsigned long data_ptr)
{
	int pool, policy, cpu_pool_ioc = cqm_ctrl->is_cpu_pool_ioc;
	unsigned long ptr_base, p_data = 0;
	unsigned long size_to_wb;
	u64 desc_qw0, desc_qw1;
	struct cbm_desc *enq_p;
	u8 high_addr;
	u32 rtn_cnt;

	if (!desc || pid >= CPU_EQM_PORT_NUM)
		return CBM_FAILURE;
	pool = cqm_desc_data_pool(desc->desc1);
	policy = cqm_desc_data_policy(desc->desc3);

	if (IS_CPU_POOL(pool, policy)) {
		p_data = __pa(data_ptr);
	} else {
		ptr_base = (unsigned long)
			cqm_cpubuff_get_buffer_head((void *)data_ptr, pool);
		if (!cpu_pool_ioc) {
			size_to_wb = data_ptr - ptr_base +
				((struct dma_desc *)desc)->data_len;
			p_data = dma_map_single_attrs(cqm_ctrl->dev, (void *)ptr_base,
						size_to_wb, DMA_TO_DEVICE,
						DMA_ATTR_NON_CONSISTENT);
			if (dma_mapping_error(cqm_ctrl->dev, p_data)) {
				dev_err(cqm_ctrl->dev, "%s DMA map failed\n", __func__);
				return CBM_FAILURE;
			}
		} else {
			p_data = __pa(ptr_base);
		}
		p_data += data_ptr - ptr_base;
	}

	high_addr =  p_data >> 32;
	desc->desc3 |= (high_addr & 0xf) << PTR_RTN_CPU_DW3_EGP_0_PTR_MSB_POS;
	desc_qw0 = (unsigned long)*(u64 *)&desc->desc0;
	desc_qw1 = (((u64)(desc->desc3)) << 32) + (p_data & 0xffffffff);
	CQM_DEBUG(CQM_DBG_FLAG_TX_DESC, "%s, desc_qw0:%lx, desc_qw1:%lx\n",
				 __func__, (unsigned long)desc_qw0,
				 (unsigned long)desc_qw1);

	if (IS_CPU_POOL(pool, policy)) {
		spin_lock_bh(&cpu_pool_enq);
		rtn_cnt = cbm_r32(cqm_ctrl->cqm + CBM_CPU_POOL_BUF_RTRN_CNT);
		if (rtn_cnt >= RTN_PTRS_IN_CACHELINE)
			cpu_pool_free();
		if (cqm_cpu_pool_enqueue_status() == CBM_FAILURE) {
			spin_unlock_bh(&cpu_pool_enq);
			return CBM_FAILURE;
		}
		cpu_enqueue_hw(pid, desc, desc_qw0, desc_qw1);
		atomic_inc(&cqm_fe_policy.enq_cnt);
		spin_unlock_bh(&cpu_pool_enq);
	} else {
		cpu_enqueue_hw(pid, desc, desc_qw0, desc_qw1);
	}
	enq_p = &get_cpu_var(cqm_enq_desc);
	enq_p->desc0 = desc->desc0;
	enq_p->desc1 = desc->desc1;
	enq_p->desc2 = desc->desc2;
	enq_p->desc3 = desc->desc3;
	put_cpu_var(cqm_enq_desc);
	return CBM_SUCCESS;
}

/*return 1: pointer is valid
 *0: pointer is invalid
 */
static inline int check_ptr_validation_lgm(void *v_buf)
{
	unsigned long buf = (unsigned long)v_buf;
	u8 id = cqm_ctrl->cpu_base_pool;

	if (!buf)
		return 0;

	return (buf >= bm_pool_conf[id].pool_start_low &&
		buf < bm_pool_conf[id + 3].pool_end_low);
}

#ifdef MODULE_TEST
void NO_OPTIMIZE *cqm_fsqm_buff_alloc(u32 pid, u32 policy)
#else
static void NO_OPTIMIZE *cqm_fsqm_buff_alloc(u32 pid, u32 policy)
#endif
{
	unsigned long buf_addr1 = 0;
	unsigned long buf_addr = 0;
	unsigned long sys_flag;
	u8 wait = 0;
	/*get the fsqm segment mask from pool config table */
	u32 segment_mask = bm_pool_conf[CQM_LGM_TOTAL_POOLS - 1].segment_mask;

	if (pid >= CPU_EQM_PORT_NUM || policy != CQM_FSQM_POLICY) {
		dev_err(cqm_ctrl->dev, "fsqm alloc err! pid: %d, policy: %d\n",
			pid, policy);
		return NULL;
	}
	local_irq_save(sys_flag);
	preempt_disable();
	do {
		buf_addr = cbm_r32(bufreq[CQM_FSQM_BUF_SIZE] + (pid * 0x1000));
	} while (((buf_addr & segment_mask) == segment_mask) &&
		 ((wait++) < DEFAULT_WAIT_CYCLES));
	if ((buf_addr & segment_mask) == segment_mask) {
		dev_err(cqm_ctrl->dev, "alloc fsqm buff fail for pid: %d 0x%x",
			pid, (int)buf_addr);
		preempt_enable();
		local_irq_restore(sys_flag);
		return NULL;
	}
	buf_addr1 = cbm_r32(bufreq_1[CQM_FSQM_BUF_SIZE] + (pid * 0x1000));
	buf_addr1 = ((buf_addr1 & NEW_PBPTR1_CPU_IGP_0_SA_MASK) >>
		NEW_PBPTR1_CPU_IGP_0_SA_POS);
	buf_addr |= buf_addr1 << 32;
	preempt_enable();
	local_irq_restore(sys_flag);
	CQM_UP_STATS(cqm_dbg_cntrs[CQM_FSQM_POLICY]
			 [0].alloc_cnt);
	return (void *)buf_addr;
}

static void
NO_OPTIMIZE *cqm_buffer_alloc(u32 pid, u32 flag, u32 size,
			      u32 *buf_size, u32 headroom)
{
	u32 new_size = size + headroom;
	unsigned long buf_addr1 = 0;
	unsigned long buf_addr = 0;
	unsigned long sys_flag;
	u32 segment_mask = 0;
	u8 policy, pool;
	u32 i = 0, req_idx = 0;
	unsigned long v_buf;
	int v2_flag = cqm_ctrl->v2_flag;
	int bm_idx, result = 0;

	if (flag & BIT(1))
		new_size = size;

	if (pid >= CPU_EQM_PORT_NUM) {
		dev_err(cqm_ctrl->dev, "illegal pid: %d\n", pid);
		return NULL;
	}
	if (new_size >
	    bm_pool_conf[cqm_ctrl->num_sys_pools - 1].buf_frm_size) {
		dev_err(cqm_ctrl->dev, "Invalid size requested: %d\n", size);
		return NULL;
	}
	/*A0:	For any packet size <= 2048, allocate IOC 2k policy buffer
	 *	For any packet size > 2048, print failure and return
	 * B0:	For any packet size <= 2048, allocate IOC 2k policy buffer
	 *	For jumbo packets, use NIOC policy of 10K buffers are used,
	 *	do cache flush operations(No clflush issue is expected)
	 */
	if (new_size <= IOC_2K_POLICY_BUF_SIZE) {
		new_size = IOC_2K_POLICY_BUF_SIZE;
	} else {
		if (!v2_flag) {
			dev_err(cqm_ctrl->dev,
				"Invalid size requested for A0 %d\n", size);
			return NULL;
		}
	}

	while (req_idx < BYTES_IN_SEL0_3 + BYTES_IN_SEL4) {
		bm_idx = bufreq_bm_pool_conf_lut[req_idx];
		if (new_size <= bm_pool_conf[bm_idx].buf_frm_size &&
		    bm_pool_conf[bm_idx].buf_type ==
		    (new_size == IOC_2K_POLICY_BUF_SIZE ?
		    BUF_TYPE_CPU_ISOLATED :
		    BUF_TYPE_NIOC_SHARED)) {
			*buf_size = bm_pool_conf[bm_idx].buf_frm_size;
			segment_mask = bm_pool_conf[bm_idx].segment_mask;
			break;
		}
		req_idx++;
	}
	if (req_idx >= BYTES_IN_SEL0_3 + BYTES_IN_SEL4) {
		dev_err(cqm_ctrl->dev,
			"Req Buff size exceeds the max available\n");
		return NULL;
	}

	local_irq_save(sys_flag);
	preempt_disable();
	while (req_idx < BYTES_IN_SEL0_3) {
		i = 0;
		do {
			buf_addr = cbm_r32(bufreq[req_idx] + (pid * 0x1000));
			buf_addr1 = cbm_r32(bufreq_1[req_idx] + (pid * 0x1000));
		} while (((buf_addr & segment_mask) == segment_mask) &&
			 ((i++) < DEFAULT_WAIT_CYCLES));
		if ((buf_addr & segment_mask) == segment_mask) {
			CQM_DEBUG(CQM_DBG_FLAG_CPU_BUFF,
				  "buf_ptr: 0x%x wait: %d\n",
				  (int)buf_addr, i);
			req_idx++;
			*buf_size = bm_pool_conf[req_idx].buf_frm_size;
			segment_mask =
				bm_pool_conf[req_idx].segment_mask;
			CQM_DEBUG(CQM_DBG_FLAG_CPU_BUFF,
				  "trying next policy %d\n", req_idx);
		} else {
			result = 1;
			break;
		}
	}
	if (!result) {
		dev_err(cqm_ctrl->dev, "alloc fail in %d typ: %d policy: %d\n",
			pid, flag, req_idx);
		preempt_enable();
		local_irq_restore(sys_flag);
		return NULL;
	}
	pool = (u8)buf_addr1 & NEW_S0PTR1_CPU_IGP_0_SRC_POOL_MASK;
	policy = (u8)bm_pool_conf[pool].policy;
	buf_addr |= ((buf_addr1 & NEW_S0PTR1_CPU_IGP_0_SA_MASK) >>
		 NEW_S0PTR1_CPU_IGP_0_SA_POS) << 32;
	if (pool < cqm_ctrl->cpu_base_pool) {
		v_buf = (unsigned long)
			__va(nioc_addr_to_ioc_addr(buf_addr));
	} else {
		v_buf = (unsigned long)__va(buf_addr);
	}
	add_metadata((u8 *)v_buf - PP_HEADROOM + BM_METADATA_OFFSET,
		     pool, policy);
	if (pool < CQM_LGM_TOTAL_BM_POOLS)
		CQM_UP_STATS(cqm_dbg_cntrs[policy][pool].alloc_cnt);
	v_buf = v_buf - PP_HEADROOM + headroom;
	preempt_enable();
	local_irq_restore(sys_flag);
	CQM_DEBUG(CQM_DBG_FLAG_CPU_BUFF,
		  "%s:v_buff:0x%lx, p_buff:0x%lx, PP_HR:%d, in_hr:%d, pool:%u policy:%u\n",
		  __func__, (unsigned long)v_buf, (unsigned long)buf_addr,
		  PP_HEADROOM, headroom, pool, policy);

	return (void *)v_buf;
}

static int cqm_buffer_free(u32 pid, void *v_buf_free, u32 flag)
{
	return cqm_buff_free(v_buf_free);
}

static void cqm_dc_buffer_return(u32 port, phys_addr_t pa,
				 u8 policy, u8 pool, u32 id)
{
	u64 dc_buf_ptr;
	u32 hi = FIELD_PREP(PTR_RTN_CPU_DW3_EGP_0_POOL_MASK, pool) |
		 FIELD_PREP(PTR_RTN_CPU_DW3_EGP_0_POLICY_MASK, policy) |
		 FIELD_PREP(PTR_RTN_CPU_DW3_EGP_0_PTR_MSB_MASK,
			    upper_32_bits(pa));
	u64 val = lower_32_bits(pa) | ((u64)hi << 32);

	dc_buf_ptr = val;
	dc_buff_free(port, &dc_buf_ptr, id);
}

static int skb_buff_free(u32 pid, struct sk_buff *skb, u32 flag)
{
	dev_warn(cqm_ctrl->dev,
		 "kfree_skb() is preferred instead of calling %s directly",
		 __func__);
	return CBM_FAILURE;
}

static int
NO_OPTIMIZE setup_desc(struct cbm_desc *desc, struct cbm_tx_data *data,
		       struct cbm_tx_desc_params *desc_param)
{
	struct dma_desc temp_desc;
	struct cbm_desc *ptr;
	unsigned long data_ptr = desc_param->data_ptr;
	int pool = desc_param->pool;
	int policy = desc_param->policy;

	if (!desc)
		return CBM_FAILURE;
	memset(&temp_desc, 0, sizeof(struct dma_desc));
	temp_desc.h			= data->f_byqos ? 1 : 0;
	temp_desc.data_ptr		= data_ptr;
	temp_desc.data_ptr1		= (data_ptr >> 32) & 0xf;
	temp_desc.data_len		= desc_param->data_len;
	temp_desc.sop			= 1;
	temp_desc.eop			= 1;
	temp_desc.own			= 0;
	/*keep dic original value set by caller */
	if (desc_param->DW3 & BIT(27))
		temp_desc.dic = 1;
	ptr = (struct cbm_desc *)&temp_desc;
	ptr->desc0 = desc_param->DW0;
	ptr->desc1 = desc_param->DW1;
	ptr->desc3 = (ptr->desc3 & (~PTR_RTN_CPU_DW3_EGP_0_POLICY_MASK)) |
		     (policy << PTR_RTN_CPU_DW3_EGP_0_POLICY_POS);

	ptr->desc1 = (ptr->desc1 & (~0x3c0000)) |
		     (pool << 18);

	desc->desc0 = ptr->desc0;
	desc->desc1 = ptr->desc1;
	desc->desc2 = ptr->desc2;
	desc->desc3 = ptr->desc3;
	CQM_DEBUG(CQM_DBG_FLAG_TX_DESC,
		  "%s 0x%x 0x%x 0x%x 0x%x, pool:%d, policy:%d\n",
		  __func__, desc->desc0,
		  desc->desc1, desc->desc2, desc->desc3, pool, policy);
	return CBM_SUCCESS;
}

static inline int cqm_cpu_pkt_tx_path_check(struct cpu_pkt_tx_info *pkt_info_p,
					    enum E_CPU_TX_PATH *path)
{
	int v2_flag = cqm_ctrl->v2_flag;

	if (pkt_info_p->cpu_buf && pkt_info_p->f_qosby && !pkt_info_p->no_hdr_room_f)
		pkt_info_p->policy_fe_allowed = 1;

	if (pkt_info_p->jumbo_pkt && !pkt_info_p->policy_fe_allowed &&
	    !v2_flag) {
		dev_err(cqm_ctrl->dev, "packet exceeds limit for A0\n");
		return CBM_FAILURE;
	}

	if (pkt_info_p->no_hdr_room_f || pkt_info_p->clone_f ||
	    (pkt_info_p->cpu_buf && !pkt_info_p->policy_fe_allowed))
		*path = LINEARISE_PATH;
	else if (pkt_info_p->policy_fe_allowed)
		*path = CPU_POLICY_FE_PATH;
	else
		*path = NON_LINEARISE_PATH;

	CQM_DEBUG(CQM_DBG_FLAG_TX_PATH,
		  "%s no_tailroom:%d no_headroom:%d\n",
		  __func__, pkt_info_p->no_tail_room_f,
		  pkt_info_p->no_hdr_room_f);
	CQM_DEBUG(CQM_DBG_FLAG_TX_PATH,
		  "%s path %d cpu_buff:%d clone:%d fe_allowed:%d\n",
		  __func__, *path, pkt_info_p->cpu_buf,
		  pkt_info_p->clone_f,
		  pkt_info_p->policy_fe_allowed);
	return CBM_SUCCESS;
}

static s32 cqm_tx_skb_data(struct sk_buff *skb, struct cbm_tx_data *data)
{
	if (data->pmac) {
		skb_push(skb, data->pmac_len);
		memcpy(skb->data, data->pmac, data->pmac_len);
	}

	return CBM_SUCCESS;
}

static inline u16 cqm_skb_padded(struct sk_buff *skb,
				 struct pmac_tx_hdr *pmac,
				 struct iphdr *iph)
{
	int maclen = pmac->ip_offset << 1;

	return (skb->len - (ntohs(iph->tot_len) + maclen));
}

/* Updte UDP checksum in skbdptr_in_buf.
 * skb data is not updated as it may be shared in LINEARISE_PATH.
 */
static s32 cqm_cpu_pkt_tx_chksum_wa(struct sk_buff *skb, struct cbm_tx_data *data,
				    unsigned char *skbdptr_in_buf)
{
	struct pmac_tx_hdr *pmac = (struct pmac_tx_hdr *)data->pmac;
	struct udphdr *udph, *buf_udph;
	int offset = skb_transport_offset(skb);
	__wsum csum = 0;
	__sum16 csum_final;
	u16 pad_len = 0;

	/* If tcp_chksum in PMAC header is 0 and SKB has CHECKSUM_PARTIAL,
	 * CQM need to calculate the checksum and update the buffer.
	 * This is a software workaround for a GSWIP hardware bug that
	 * UDP checksum calculation for small packet is incorrect. DPM identifies
	 * those packets and clears tcp_chksum field in PMAC.
	 */
	if (skb->ip_summed == CHECKSUM_PARTIAL && !pmac->tcp_chksum) {
		udph = skb->encapsulation ? inner_udp_hdr(skb) : udp_hdr(skb);

		csum = csum_sub(skb_checksum(skb, offset, skb->len - offset, 0),
				(__wsum)udph->check);
		buf_udph = (struct udphdr *)(skbdptr_in_buf + ((unsigned char *)udph - skb->data));
		if (pmac->tcp_type == UDP_OVER_IPV6) {
			struct ipv6hdr *ip6h = skb->encapsulation ? inner_ipv6_hdr(skb) :
					       ipv6_hdr(skb);

			csum_final = csum_ipv6_magic(&ip6h->saddr, &ip6h->daddr,
						     skb->len - offset, IPPROTO_UDP,
						     csum);
		} else if (pmac->tcp_type == UDP_OVER_IPV4) {
			struct iphdr *iph = skb->encapsulation ? inner_ip_hdr(skb) :
					    ip_hdr(skb);
			struct iphdr *buf_iph;

			/* Packet might be padded by upper drivers e.g. PON
			 * adjust actual len for cksum.
			 */
			pad_len = cqm_skb_padded(skb, pmac, iph);
			csum_final = csum_tcpudp_magic(iph->saddr, iph->daddr,
					(skb->len - pad_len) - offset, IPPROTO_UDP,
					csum);

			if ((unsigned char *)iph < skb->data)
				return CBM_FAILURE;
			buf_iph = (struct iphdr *)(skbdptr_in_buf +
						   ((unsigned char *)iph - skb->data));
			buf_iph->check = 0;
			buf_iph->check = ip_fast_csum(buf_iph, buf_iph->ihl);
		} else {
			csum_final = buf_udph->check;
		}

		if ((unsigned char *)udph < skb->data)
			return CBM_FAILURE;
		buf_udph->check = csum_final;
	}

	return CBM_SUCCESS;
}

static s32
NO_OPTIMIZE cqm_cpu_pkt_tx(struct sk_buff *skb, struct cbm_tx_data *data,
			   u32 flags)
{
	struct cbm_desc desc;
	unsigned long tmp_data_ptr;
	unsigned long new_buf = 0;
	struct cpu_pkt_tx_info pkt_info = {0};
	u8 pool = 0, policy = 0;
	struct skb_shared_info *shinfo;
	int copied_to_cbm = 0;
	unsigned long metadata;
	enum E_CPU_TX_PATH path_sel = INVALID_PATH;
	int tot_len, buf_size, cpu, enq_cpu;
	struct cbm_tx_desc_params tx_desc_param;
	unsigned char *skbdptr_in_buf = skb->data;
	unsigned short needed_headroom = LGM_SKB_PTR_SIZE;

	tot_len = skb->len;
	pkt_info.clone_f = (skb_cloned(skb) || skb_shared(skb));
	pkt_info.cpu_buf = !check_ptr_validation_lgm((skb->head));
	pkt_info.f_qosby = data->f_byqos;

	cpu = smp_processor_id();
	if (data->pmac) {
		tot_len += data->pmac_len;
		needed_headroom += data->pmac_len;
	}

	pkt_info.no_hdr_room_f = skb_headroom(skb) < needed_headroom;
	CQM_DEBUG(CQM_DBG_FLAG_TX_PATH, "%s: skb_headroom:%d, pmac_len:%d\n",
		  __func__, skb_headroom(skb), data->pmac_len);
	/* Fragments may be a combination of CPU/non-CPU buffer.
	 * Force it to clone.
	 */
	shinfo = skb_shinfo(skb);
	if (shinfo->nr_frags || shinfo->frag_list)
		pkt_info.clone_f = 1;

	if (tot_len > CQM_JUMBO_PKT_SIZE)
		pkt_info.jumbo_pkt = 1;

	if (cqm_cpu_pkt_tx_path_check(&pkt_info, &path_sel) == CBM_FAILURE) {
		/* return CBM_FAILURE, skb will be freed by dp
		 * if return CBM_SUCCESS, skb needs to be freed by CQM
		 * using dev_kfree_skb_any(skb);
		 */
		dev_info(cqm_ctrl->dev, "Invalid cpu tx path\n");
		return CBM_FAILURE;
	}

	switch (path_sel) {
	case LINEARISE_PATH:
		dev_dbg(cqm_ctrl->dev, "use BM buffer\n");
		new_buf = (unsigned long)cqm_buffer_alloc(cpu,
							  0, tot_len,
							  &buf_size,
							  CQM_TX_HEADROOM);
		if (!new_buf) {
			dev_dbg(cqm_ctrl->dev,
				"buf alloc failed! req_size: %d\n",
				buf_size);
				/*DPM seems to free skb, do nothing here*/
			return CBM_FAILURE;
		}
		new_buf -= CQM_TX_HEADROOM;
		if (get_metadata((u8 *)new_buf + BM_METADATA_OFFSET,
				 &pool, &policy))
			goto ERR_CASE_1;

		if (cbm_linearise_buf(skb, data, buf_size,
				      (void *)new_buf + CQM_TX_HEADROOM) < 0) {
			pr_err("linearize failed\n");
			goto ERR_CASE_1;
		}
		tmp_data_ptr = new_buf + CQM_TX_HEADROOM;
		copied_to_cbm = 1;
		skbdptr_in_buf = data->pmac ? (unsigned char *)tmp_data_ptr + data->pmac_len :
					      (unsigned char *)tmp_data_ptr;
		break;
	case CPU_POLICY_FE_PATH:
		cqm_tx_skb_data(skb, data);

		metadata = (unsigned long)skb->data - LGM_SKB_PTR_SIZE;
		*(unsigned long *)metadata = (unsigned long)skb;
		pool = CQM_CPU_POOL;
		policy = CQM_CPU_POLICY;
		tmp_data_ptr = (unsigned long)skb->data;
		CQM_DEBUG(CQM_DBG_FLAG_TX_PATH,
			  "%s fe: tmp_data_ptr 0x%lx, metadata: 0x%lx\n",
			  __func__, (unsigned long)skb->data, metadata);
		break;
	case NON_LINEARISE_PATH:

		if (pkt_info.cpu_buf)
			WARN_ON(1);
		cqm_tx_skb_data(skb, data);
		tmp_data_ptr = (unsigned long)skb->data;
		if (get_metadata(skb->head - CQM_POOL_METADATA, &pool, &policy) &&
		    (get_buf_pool(skb->head, bm_pool_conf, &pool) ||
		     cqm_get_policy(skb->head, pool, &policy))) {
			dev_err(cqm_ctrl->dev,
				"skb head 0x%lx pool %u policy %u check failed..\n",
				(unsigned long)skb->head, pool, policy);
			dev_kfree_skb_any(skb);
			goto ERR_CASE_2;
		}
		CQM_DEBUG(CQM_DBG_FLAG_TX_PATH,
			  "%s NON_LINEARISE_PATH tmp_data_ptr 0x%lx pool %u policy %u\n",
			  __func__, tmp_data_ptr, pool, policy);
		/* Detach the skb */
		skb->head = NULL;
		copied_to_cbm = 1;
		break;
	default:
		return CBM_FAILURE;
	}

	tx_desc_param.DW0 = skb->DW0;
	tx_desc_param.DW1 = skb->DW1;
	tx_desc_param.DW3 = skb->DW3;
	tx_desc_param.data_len = tot_len;
	tx_desc_param.data_ptr = tmp_data_ptr;
	tx_desc_param.pool = (int)pool;
	tx_desc_param.policy = (int)policy;
	if (setup_desc((struct cbm_desc *)&desc, data, &tx_desc_param)) {
		dev_err(cqm_ctrl->dev, "cbm setup desc failed..\n");
		cqm_buff_free((void *)tmp_data_ptr);
		goto ERR_CASE_2;
	}

	/* PP hardware will mis-behave if packet with length <17 Bytes is sent
	 * to PP. However, padding should not be done in CQM driver as some
	 * drivers, e.g. DSL, do not expect padding. Padding, if required, should
	 * be done by different drivers at RX.
	 *
	 * Here, if packet length is <17 Bytes, print a warning message.
	 */
	if (skb->len < 17) {
		struct dma_desc *dma_desc = (struct dma_desc *)&desc;
		dev_warn(cqm_ctrl->dev,
			 "Warning: %s: short pkt (%u B) to port %u with policy: %u\n",
			 skb->dev ? netdev_name(skb->dev) : "Unknown device",
			 skb->len, dma_desc->ep, dma_desc->policy);
	}

	if (data->pmac)
		cqm_cpu_pkt_tx_chksum_wa(skb, data, skbdptr_in_buf);

	if (!(pkt_info.cpu_buf && !copied_to_cbm)) {
		dev_dbg(cqm_ctrl->dev, "%s, dev_free_skb_any\n", __func__);
		dev_kfree_skb_any(skb);
	}

	enq_cpu = data->f_byqos ? cpu +
		cqm_ctrl->eqm_pid_cfg.qosbp_start : cpu;
	CQM_DEBUG(CQM_DBG_FLAG_TX_PATH,
		  "%s: f_byqos:%d, qosbp_start:%d pid:%d\n",
		  __func__, data->f_byqos,
		  cqm_ctrl->eqm_pid_cfg.qosbp_start, enq_cpu);

	if (cqm_cpu_enqueue(enq_cpu, &desc, tmp_data_ptr)) {
		if (policy != CQM_CPU_POLICY) {
			dev_err(cqm_ctrl->dev, "cpu hw enqueue failed..\n");
			cqm_buff_free((void *)tmp_data_ptr);
		} else {
			dev_kfree_skb_any(skb);
			dev_err(cqm_ctrl->dev, "cpu pool enqueue failed..\n");
		}
		/*if return FAILURE, upper layer will also try to free the skb*/
		return CBM_SUCCESS;
	}
	if (pool < CQM_LGM_TOTAL_BM_POOLS)
		CQM_UP_STATS(cqm_dbg_cntrs[policy][pool].tx_cnt);
	return CBM_SUCCESS;
ERR_CASE_1:
	cqm_buff_free((void *)new_buf);
ERR_CASE_2:
/*dpm seems to free skb, do nothing here*/
	return CBM_FAILURE;
}

static s32 NO_OPTIMIZE update_flush_port_info(struct cbm_tx_push *ptr_flush)
{
	void *tx_addr = cqm_ctrl->txpush_phys;
	u32 port;

	if (get_cqmport(&port, DQM_DMA_TYPE,
			DEQ_FLUSH, DQ_PORT) < 0) {
		dev_err(cqm_ctrl->dev, "Flush port get failed..\n");
		return -1;
	}

	cqm_ctrl->flush_port = port;
	ptr_flush->deq_port = port;
	ptr_flush->tx_ring_size = 1;
	ptr_flush->tx_b_credit = 0;
	ptr_flush->txpush_addr = (void *)((unsigned long)(tx_addr +
				  (TXPUSH_CMD_RX_EGP_1 * port)));
	ptr_flush->txpush_addr_qos = (void *)((unsigned long)(tx_addr +
				  (TXPUSH_CMD_RX_EGP_1 * port)) |
				  BIT(28));
	ptr_flush->tx_ring_offset = TXPUSH_CMD_RX_EGP_1;
	ptr_flush->tx_pkt_credit = cqm_ctrl->dqm_port_info[port].dq_txpush_num;
	cqm_dma_port_enable(cqm_ctrl->flush_port,
			    CBM_PORT_F_DEQUEUE_PORT, 0, BUF_TYPE_INVALID);
	return 0;
}

static bool flush_port_enabled(void)
{
	int dqp_idx = cqm_ctrl->flush_port;
	void *deq = cqm_ctrl->deq;
	u32 offset = 0;
	u32 port_type;
	u32 val = 0;
	bool ret = false;

	port_type = find_dqm_port_type(dqp_idx);

	switch (port_type) {
	case DQM_DMA_TYPE:
		offset = DQ_DMA_PORT(dqp_idx, cfg);
		val = cbm_r32(deq + offset);
		break;
	default:
		dev_dbg(cqm_ctrl->dev,  "%s default port_type %d\n",
			__func__, port_type);
		return false;
	};

	if (val == 0)
		ret = false;
	else
		ret = true;

	return ret;
}

static int get_cpu_policy_num(void)
{
	struct cqm_bm_policy_params *info = NULL;
	int idx;
	u32 count = 0;

	for (idx = 0; idx < CQM_LGM_NUM_BM_POLICY; idx++) {
		info = &cqm_ctrl->cqm_bm_policy_dts[idx];
		if (info->policy_type == CPU_DEV)
			count++;
	}
	return count;
}

static s32
NO_OPTIMIZE update_cqm_core_linux_port(const u8 core,
			   struct cbm_cpu_port_data *data,
			   u32 base_policy)
{
	void *tx_addr = cqm_ctrl->txpush_phys;
	struct cbm_tx_push *info = NULL;
	u32 offset;
	u32 type;
	u32 idx;
	u32 j;
	u32 num_policy_cpu;

	for (j = 0; j < 2; j++) {
		data->type[core][j] = CBM_CORE_LINUX;
		idx = (core * 2) + j;
		type = cqm_ctrl->dqm_port_info[idx].cpu_port_type;
		if (!(cqm_ctrl->dqm_port_info[idx].valid &&
		      IS_CPU_PORT_TYPE(type) &&
		      cpu_online(core))) {
			data->dq_tx_push_info[core][j].deq_port = -1;
			continue;
		}
		info = &data->dq_tx_push_info[core][j];
		info->deq_port = idx;
		info->tx_ring_size = 1;
		info->tx_b_credit = 0;
		offset = TXPUSH_CMD_RX_EGP_1 * idx;
		info->txpush_addr = (void *)((unsigned long)(tx_addr + offset));
		info->txpush_addr_qos =
			(void *)((unsigned long)(tx_addr + offset) | BIT(28));
		info->tx_ring_offset = TXPUSH_CMD_RX_EGP_1;
		info->tx_pkt_credit =
			cqm_ctrl->dqm_port_info[core].dq_txpush_num;

		num_policy_cpu = get_cpu_policy_num();
		data->policy_num[core][j] = num_policy_cpu;
		data->policy_base[core][j] = base_policy;
	}
	return 0;
}

static s32
NO_OPTIMIZE update_cqm_core_dpdk_port(const u8 core,
				      struct cbm_cpu_port_data *data)
{
	struct cqm_dpdk_setting *dpdk = cqm_ctrl->cqm_dpdk;
	void *tx_addr = cqm_ctrl->txpush_phys;
	struct cbm_tx_push *info;
	static u8 temp;
	dma_addr_t mem_dma;
	u32 dqp_qbp = 0;
	u32 eqp_qbp = 0;
	u32 dqmdmaq = 0;
	u32 config = 0;
	u32 j;
	int v2_flag = cqm_ctrl->v2_flag;

	config = CFG_CPU_IGP_0_EQREQ_MASK |
		 CFG_CPU_IGP_0_EQPCEN_MASK |
		 CFG_CPU_IGP_0_BP_EN_MASK;
	if (v2_flag)
		config |= CFG_CPU_IGP_0_PSBKEEP_MASK;

	for (j = 0; j < 1; j++) {
		data->type[core][j] = CBM_CORE_DPDK;
		info = &data->dq_tx_push_info[core][j];
		info->deq_port = dpdk[temp].deq_port;

		if (dpdk[temp].deq_mode == DEQ_PPV4) {
			info->tx_ring_size = dpdk[temp].ring_size;
			info->tx_pkt_credit = dpdk[temp].ring_size;
			if (!cqm_gen_pool_alloc(1, (size_t)info->tx_pkt_credit,
						&mem_dma,
						GEN_RW_POOL, -1))
				return -1;
			info->txpush_addr = (void *)mem_dma;
		} else {
			info->tx_ring_size = 1;
			info->tx_pkt_credit =
			cqm_ctrl->dqm_port_info[info->deq_port].dq_txpush_num;
			info->txpush_addr = (void *)(tx_addr +
				(TXPUSH_CMD_RX_EGP_1 * info->deq_port));
		}

		/* get QOSBP deq port */
		if (get_cqmport(&dqp_qbp, DQM_DMA_TYPE,
				DEQ_CPU_QOSBP, DQ_PORT) < 0)
			return -1;

		handle_dma_chnl_init(dqp_qbp, CBM_PORT_F_DEQUEUE_PORT);
		cqm_ctrl->dqm_port_info[dqp_qbp].allocated = P_ALLOCATED;
		cqm_ctrl->dqm_port_info[info->deq_port].allocated = P_ALLOCATED;

		if (get_cqmport(&info->igp_id, EQM_CPU_TYPE,
				EQM_DPDK, EQ_PORT) < 0)
			return -1;

		init_cqm_enq_cpu_port(info->igp_id, config);
		cqm_ctrl->eqm_port_info[info->igp_id].allocated = P_ALLOCATED;

		/* get and handle QOSBP enq port */
		if (get_cqmport(&eqp_qbp, EQM_CPU_TYPE, EQM_DPDK, EQ_PORT) < 0)
			return -1;
		dqmdmaq = ((dqp_qbp - cqm_ctrl->dqm_pid_cfg.qosbp_start) <<
			   CFG_CPU_IGP_0_DQMDMAQ_POS) &
			   CFG_CPU_IGP_0_DQMDMAQ_MASK;
		config = CFG_CPU_IGP_0_EQREQ_MASK |
			 CFG_CPU_IGP_0_EQPCEN_MASK |
			 CFG_CPU_IGP_0_BP_EN_MASK |
			 CFG_CPU_IGP_0_QOSBYPSEN_MASK |
			 CFG_CPU_IGP_0_BUFREQ3_MASK |
			 dqmdmaq;
		if (v2_flag)
			config |= CFG_CPU_IGP_0_PSBKEEP_MASK;
		init_cqm_enq_cpu_port(eqp_qbp, config);
		cqm_ctrl->eqm_port_info[eqp_qbp].allocated = P_ALLOCATED;
#ifdef DPDK_POLICY
		data->policy_num[core][j] = 1;
		data->policy_base[core][j] = cqm_pp_policy_setup(INGRESS,
								 DPDK,
								 0);
		if (data->policy_base[core][j] < 0)
			return -1;
		/* add vm related config for qosbp enq port*/
		poolid = get_pool_from_policy(data->policy_base[core][j]);
		if (poolid == INVALID_POOL_ID) {
			dev_warn(cqm_ctrl->dev, "%s: invalid poolid for policy %d\n",
				 __func__, data->policy_base[core][j]);
			continue;
		}
		cqm_vm_pool_check_enable(eqp_qbp, poolid,
					 data->policy_base[core][j]);
		info->txpush_addr_qos = (void *)((unsigned long)
		(info->txpush_addr) | BIT(28));
#else
		data->policy_num[core][j] = 4;
		data->policy_base[core][j] = 0;
#endif
		info->txpush_addr_qos = (void *)((unsigned long)
					(info->txpush_addr) | BIT(28));
	}
	temp++;
	return 0;
}

/* Construct bufreq index to bm_pool_conf LUT. This is to avoid iterating the
 * cqm_bm_policy_dts during buf allocation API.
 * bufreq_bm_pool_conf_lut will have bm_pool_conf index for policies in
 * register sel0_3 and sel4. For example, bufreq_bm_pool_conf_lut[2] is for
 * buffer policy for size 2, i.e. policy 20. Its value is the bm_pool_conf
 * index for policy 20, i.e. 8. If policy is not set in bm_pool_conf, then
 * the index of bm_pool_conf that has same pool_id for the policy is returned.
 */
static s32 NO_OPTIMIZE construct_bufreq_bm_pool_conf_lut(void)
{
	int idx, bm_pool_conf_idx, policy;
	u32 sel0_3, sel4;
	struct cqm_bm_policy_params *info = NULL;

	/* Get req idx */
	sel0_3 = cbm_r32(cqm_ctrl->cqm + CBM_POLICY_SEL0_3);
	sel4 = cbm_r32(cqm_ctrl->cqm + CBM_POLICY_SEL4);
	pr_debug("sel0_3: 0x%x sel4: 0x%x\n", sel0_3, sel4);

	for (idx = 0; idx < BYTES_IN_SEL0_3 + BYTES_IN_SEL4; idx++) {
		if (idx < BYTES_IN_SEL0_3)
			policy = sel0_3 >> (idx * 8) & 0xFF;
		else
			policy = sel4 >> ((idx - BYTES_IN_SEL0_3) * 8) & 0xFF;

		/* Find BM pool */
		for (bm_pool_conf_idx = 0;
		     bm_pool_conf_idx < CQM_LGM_TOTAL_POOLS;
		     bm_pool_conf_idx++) {
			if (bm_pool_conf[bm_pool_conf_idx].policy == policy) {
				bufreq_bm_pool_conf_lut[idx] = bm_pool_conf_idx;
				break;
			}
		}
		if (bm_pool_conf_idx >= CQM_LGM_TOTAL_POOLS) {
			info = &cqm_ctrl->cqm_bm_policy_dts[policy];
			for (bm_pool_conf_idx = 0;
			     bm_pool_conf_idx < CQM_LGM_TOTAL_POOLS;
			     bm_pool_conf_idx++) {
				if (bm_pool_conf[bm_pool_conf_idx].pool ==
				    info->pp_policy_cfg.pools_in_policy[0].pool_id) {
					bufreq_bm_pool_conf_lut[idx] =
						bm_pool_conf_idx;
					break;
				}
			}
			if (bm_pool_conf_idx >= CQM_LGM_TOTAL_POOLS) {
				pr_err("%s: Unable to find bm based on pool\n",
				       __func__);
				return CBM_FAILURE;
			}
		}
	}

	for (idx = 0; idx < BYTES_IN_SEL0_3 + BYTES_IN_SEL4; idx++) {
		pr_debug("bufreq_bm_pool_conf_lut[%d]=%d\n", idx,
			 bufreq_bm_pool_conf_lut[idx]);
	}

	return CBM_SUCCESS;
}

static u32 _get_policy_from_type_dir_size(enum CQM_SUP_DEVICE type,
					  enum DIRECTION dir,
					  u32 buf_size)
{
	struct cqm_bm_policy_params *info = NULL;
	int idx;
	u32 policy = 0;

	for (idx = 0; idx < CQM_LGM_NUM_BM_POLICY; idx++) {
		info = &cqm_ctrl->cqm_bm_policy_dts[idx];
		if (info->policy_type == type &&
		    info->direction == dir) {
			u32 pool = info->pp_policy_cfg.pools_in_policy[0].pool_id;
			if (pool < CQM_LGM_TOTAL_POOLS &&
			    bm_pool_conf[pool].buf_frm_size == buf_size) {
				policy = idx;
				break;
			}
		}
	}
	return policy;
}

static u32 _config_policy_sel(void)
{
	u32 policy;
	u32 val = 0;

	policy = _get_policy_from_type_dir_size(SYS, INGRESS | EGRESS, SIZE_512);
	val = policy;
	policy = _get_policy_from_type_dir_size(SYS, INGRESS | EGRESS, SIZE_2048);
	val |= policy << CBM_POLICY_SEL0_3_POLICY1_POS;
	policy = _get_policy_from_type_dir_size(CPU_DEV, INGRESS | EGRESS, SIZE_2048);
	val |= policy << CBM_POLICY_SEL0_3_POLICY2_POS;
	policy = _get_policy_from_type_dir_size(SYS, INGRESS | EGRESS, SIZE_10K);
	val |= policy << CBM_POLICY_SEL0_3_POLICY3_POS;
	pr_debug("CBM_POLICY_SEL0_3: 0x%x\n", val);
	cbm_w32(cqm_ctrl->cqm + CBM_POLICY_SEL0_3, val);

	policy = _get_policy_from_type_dir_size(SYS, EGRESS, SIZE_2048);
	pr_debug("CBM_POLICY_SEL4: 0x%x\n", policy);
	cbm_w32(cqm_ctrl->cqm + CBM_POLICY_SEL4, policy);

	return 0;
}

/* DRVLIB_SW-4335: Add feature to differentiate first-time and subsequent
 * call to cqm_cpu_port_get. Initialization will only be performed in the
 * first-time call to cqm_cpu_port_get. Subsequent calls will just retrieve
 * the data that is stored in cqm_ctlrl that was stored in first-time call.
 *
 * Whether it's first-time call will be determined based on the flush_port
 * register. Zero value indicates first-time call. Non-zero value indicate
 * subsequent call.
 *
 * The use of flags will be deprecated, i.e. value of flags parameter will
 * not affect the operation of the API. But it will be kept to keep the same
 * API syntax.
 */

static s32
NO_OPTIMIZE cqm_cpu_port_get(struct cbm_cpu_port_data *data, u32 flags)
{
	struct cqm_eqm_port_info *p_info;
	u32 base_policy = 0;
	int i;
	u8 cpu;
	int cpu_pool_ioc = cqm_ctrl->is_cpu_pool_ioc;
	struct dqm_pid_s *p = &cqm_ctrl->dqm_pid_cfg;
	s32 ret = CBM_FAILURE;

	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s flags: 0x%x\n", __func__, flags);

	//todo: remove all the debug prints
	if (!data) {
		dev_err(cqm_ctrl->dev, "port data empty\n");
		return CBM_FAILURE;
	}

	/* Retrieve port info from cqm_ctrl for non first-time call */
	if (flush_port_enabled()) {
		dev_dbg(cqm_ctrl->dev, "%s: retrieving port_data from cqm_ctrl\n",
			__func__);
		memcpy(data, &cqm_ctrl->cpu_port_data,
		       sizeof(struct cbm_cpu_port_data));
		return CBM_SUCCESS;
	}

	update_flush_port_info(&data->dq_tx_flush_info);

	if (cpu_pool_ioc)
		base_policy = cqm_pp_policy_setup(EGRESS, CPU_DEV, 0);
	dev_dbg(cqm_ctrl->dev,  "%s base_policy = %d, flags:%x\n",
		__func__, base_policy, flags);

	_config_policy_sel();

	/* Construct */
	construct_bufreq_bm_pool_conf_lut();

	/* todo: get the number of cpu in real time for portability */
	for (cpu = 0; cpu < CQM_MAX_CPU; cpu++) {
		if (cqm_ctrl->cpu_owner[cpu] == CBM_CORE_LINUX)
			update_cqm_core_linux_port(cpu, data, base_policy);
		else if (cqm_ctrl->cpu_owner[cpu] == CBM_CORE_DPDK)
			update_cqm_core_dpdk_port(cpu, data);
		else
			dev_warn(cqm_ctrl->dev,  "[%s] unknown cpu type!\n",
				 __func__);
	}

	/*BM device is initialized only at this stage, to request BM buffers
	 *so moving all the ENQ DMA port initialization here
	 */
	/*setup the DMA channels*/
	for (i = 0; i < CQM_ENQ_PORT_MAX; i++) {
		p_info = &cqm_ctrl->eqm_port_info[i];
		if (!p_info->valid)
			continue;
		switch (p_info->port_type) {
		/* prepare this port for size2 dma enq */
		case EQM_DMA_SIZE2:
			p_info->dma_dt_init_type = ENQ_DMA_SIZE2_CHNL;
			cqm_dma_port_enable(i, 0, IOC_2K_POLICY_BUF_SIZE,
					    BUF_TYPE_NIOC_SHARED);
			break;
		/* prepare the rxdma port with no buf */
		case EQM_RXDMA:
			cqm_dma_port_enable(i, LGM_EQM_DMA_NO_BUF,
					    p_info->buf_size,
					    p_info->buf_type);
			break;
		/* prepare this ports sequentially for diff sys buffs size */
		case EQM_DMA_PON:
			/* port 27 not in use, skipped configuration*/
			if (i == 27)
				continue;
			/* port 24 uses FSQM buffer, skip configuration for non-PON
			 * model or FSQM buffer size is 0
			 */
			if (i == 24 &&
			    (!(cqm_ctrl->lpid_config.cqm_lpid_wan_mode & LPID_WAN_PON) ||
			     !cqm_ctrl->fsqm_sz))
				continue;
			cqm_dma_port_enable(i, 0, p_info->buf_size,
					    p_info->buf_type);
			break;
		default:
			break;
		};
		switch (p_info->dma_dt_init_type) {
		case ENQ_DMA_FSQM_CHNL:
		case ENQ_DMA_SIZE0_CHNL:
		case ENQ_DMA_SIZE1_CHNL:
		case ENQ_DMA_SIZE2_CHNL:
		case ENQ_DMA_SIZE3_CHNL:
			ret = cqm_enqueue_dma_port_init(i,
							p_info->dma_dt_ctrl,
							p_info->dma_dt_ch,
							p_info->dma_dt_init_type);
			if (ret) {
				dev_err(cqm_ctrl->dev,
					"[%s] failed to init dma port %d\n",
					__func__, i);
				return ret;
			}
		break;
		default:
		break;
		}
	}

	fill_dp_alloc_data(&data->re_insertion, 0,
			   p->reinsert_start, 0);
#ifdef CONFIG_RFS_ACCEL
	data->rmap = cqm_ctrl->rx_cpu_rmap;
#endif /* CONFIG_RFS_ACCEL */

	/* Copy port_data to cqm_ctrl for future retrieval */
	memcpy(&cqm_ctrl->cpu_port_data, data,
	       sizeof(struct cbm_cpu_port_data));

	return CBM_SUCCESS;
}

void print_reg(char *name, void *addr)
{
	dev_dbg(cqm_ctrl->dev, "%s addr: 0x%8p, val: 0x%8x\n",
		name, addr, cbm_r32(addr));
}

static void
NO_OPTIMIZE fill_dp_alloc_data(struct cbm_dp_alloc_data *data, int dp,
			       int port, u32 dp_flags)
{
	struct cqm_dqm_port_info *p_info;
	int dma_controller, chan;
	unsigned char dma_ctrl[DMA_CH_STR_LEN];
	int port_type = find_dqm_port_type(port);

	/*Write only valid, else skip*/
	data->flags |= CBM_PORT_DP_SET | CBM_PORT_DQ_SET;
	data->dp_port = dp;
	/* For PON and DOCSIS, deq_port (pon_start) and deq_port_num (64 or 36)
	 * should be filled up according to cqm_config.
	 * For DC ports, e.g. WLAN, DSL, etc, DPM requires deq_port_num to be 0
	 * and deq_port not set.
	 * For non-DC ports, DPM requires deq_port_num and deq_port to
	 * be set accordingly.
	 */
	if (dp_flags & DP_F_EPON ||
	    dp_flags & DP_F_GPON ||
	    dp_flags & DP_F_DOCSIS) {
		data->deq_port = CQM_PON_IP_PORT_START;
		data->deq_port_num = CQM_PON_IP_PORT_END - CQM_PON_IP_PORT_START + 1;
		/* port parameter is the DC DEQ port. In this case, it should
		 * be the DEQ port, instead of DC DEQ port.
		 */
		port = data->deq_port;
	} else if (port_type == DQM_ACA_TYPE) {
		data->deq_port_num = 0;
	} else {
		data->deq_port = port;
		data->deq_port_num = 1;
	}
	data->num_dma_chan = 0;
	p_info = &cqm_ctrl->dqm_port_info[port];
	if (p_info->dma_dt_init_type == DEQ_DMA_CHNL) {
		data->flags |= CBM_PORT_DMA_CHAN_SET | CBM_PORT_PKT_CRDT_SET;
		CQM_DEBUG(CQM_DBG_FLAG_DMA_CTRL,
			  "%s deq_port:%d dma_dt_ctrl:%d dma_dt_ch:%d\n",
			  __func__, port,
			  p_info->dma_dt_ctrl, p_info->dma_dt_ch);
		if (p_info->dma_dt_ctrl == 3)
			snprintf(dma_ctrl, sizeof(dma_ctrl), "DMA%d"
			, p_info->dma_dt_ctrl);
		else
			snprintf(dma_ctrl, sizeof(dma_ctrl), "DMA%dTX"
			, p_info->dma_dt_ctrl);
		dma_controller = dp_dma_get_controller(dma_ctrl);
		chan = dp_dma_set_id(dma_controller, 0, p_info->dma_dt_ch);
		CQM_DEBUG(CQM_DBG_FLAG_DMA_CTRL,
			  "%s dma_ctrl:%d dma_ch:0x%x\r\n",
			  __func__, dma_controller, chan);

		p_info->dma_ch = chan;
		snprintf(p_info->dma_chan_str, DMA_CH_STR_LEN, "port%d", port);
		data->dma_chan = p_info->dma_ch;
		data->tx_pkt_credit = p_info->dq_txpush_num;
		data->num_dma_chan = 1; /* !< supports 1 ring for now */
	} else {
		data->flags |= CBM_PORT_PKT_CRDT_SET;
		data->tx_pkt_credit = p_info->dq_txpush_num;
	}

	if ((dp_flags & DP_F_DOCSIS) && data->data &&
	    data->data->n_wib_credit_pkt) {
		data->tx_pkt_credit = data->data->n_wib_credit_pkt;
		p_info->wib_credit_pkt = data->data->n_wib_credit_pkt;
	}

	data->flags |= CBM_PORT_RING_ADDR_SET |
		       CBM_PORT_RING_SIZE_SET |
		       CBM_PORT_RING_OFFSET_SET;
	data->tx_ring_size = 1;
	data->tx_b_credit = 0;
	/*Lower 22 bits*/
	data->txpush_addr = (cqm_ctrl->txpush_phys +
				(TXPUSH_CMD_RX_EGP_1 * port));
	data->txpush_addr_qos = (void *)((unsigned long)
					     (cqm_ctrl->txpush_phys +
					     TXPUSH_CMD_RX_EGP_1 * port) |
					     BIT(28));
	data->tx_ring_offset = TXPUSH_CMD_RX_EGP_1;
}

static s32 NO_OPTIMIZE handle_dma_chnl_init(int port, u32 flags)
{
	struct cqm_dqm_port_info *p_info;
	struct dma_chan *chan;
	int port_type = find_dqm_port_type(port);
	u32 pon_start_id = CQM_PON_IP_PORT_START;

	if (port_type == DQM_PON_TYPE || port_type == DQM_DOCSIS_TYPE)
		p_info = &cqm_ctrl->dqm_port_info[pon_start_id];
	else
		p_info = &cqm_ctrl->dqm_port_info[port];

	chan = p_info->pch;
	if (flags & CBM_PORT_F_DISABLE) {
		if (port_type != DQM_PON_TYPE && port_type != DQM_DOCSIS_TYPE) {
			if (chan)
				dmaengine_pause(chan);

			usleep_range(20, 25);
			set_val((cqm_ctrl->deq
				+ DQ_DMA_PORT((port), cfg)),
				1, 0x2, 1);
			if (flags & CBM_PORT_F_FLUSH)
				cqm_deq_port_flush(port);
			cqm_dma_port_enable(port, CBM_PORT_F_DEQUEUE_PORT |
					    CBM_PORT_F_DISABLE,
					    0, BUF_TYPE_INVALID);
		}
	} else {
		if (chan && p_info->dma_ch_in_use) {
			cqm_dma_port_enable(port, CBM_PORT_F_DEQUEUE_PORT,
					    0, BUF_TYPE_INVALID);
			dma_async_issue_pending(chan);
		} else {
			cqm_dma_port_enable(port, CBM_PORT_F_DEQUEUE_PORT,
					    0, BUF_TYPE_INVALID);
			cqm_dq_dma_chan_init(port, p_info->dma_dt_init_type);
			p_info->dma_ch_in_use = 1;
		}
	}
	return CBM_SUCCESS;
}

static void NO_OPTIMIZE cqm_free_aca_port(s32 enqport, s32 deqport)
{
	struct cqm_dqm_port_info *p_info;
	int id = 0, cnt;
	u64 buf, desc_qw1, desc_qw0;
	u8 pool, policy;
	u32 total_cnt = 0;
	u32 dptr, brptr, brptr_num, dptr_mask;

	if (is_txin_64())
		dptr_mask = DPTR_CPU_EGP_19_DPTR_LGMC_MASK;
	else
		dptr_mask = DPTR_CPU_EGP_19_DPTR_MASK;

	if (cqm_is_port_valid(deqport)) {
		p_info = &cqm_ctrl->dqm_port_info[deqport];
		dptr = cbm_r32((cqm_ctrl->deq + DQ_DC_PORT(deqport, dptr)));
		id = get_val(dptr, dptr_mask, DPTR_CPU_EGP_19_DPTR_POS);
		brptr = cbm_r32((cqm_ctrl->deq + DQ_DC_PORT(deqport, brptr)));
		/* num_desc could be reduced to 16 if the interface requests
		 * for it. Here we use num_free_burst for the number of brptr
		 * registers for each port.
		 */
		brptr_num = cqm_ctrl->dqm_port_info[deqport].deq_info.num_free_burst;
		cnt = p_info->deq_info.num_desc * 3;
		enable_backpressure(deqport, 0);
		while (cnt--) {
			id = id % p_info->deq_info.num_desc;
			desc_qw0 = readq(cqm_ctrl->dmadesc_64 +
				CQM_DQM_DC_DESC(deqport, id));
			desc_qw1 = readq(cqm_ctrl->dmadesc_64 +
				CQM_DQM_DC_DESC(deqport, id) + 0x8);
			CQM_DEBUG(CQM_DBG_FLAG_DC_PORT,
				  "%s id:%d qw0~1: %llx %llx, addr:0x%x, own:%llx\n",
				  __func__, id, desc_qw0, desc_qw1,
				  CQM_DQM_DC_DESC(deqport, id),
				  desc_qw1 & BIT(63));
			if (desc_qw1 & BIT(63)) {
				pool = (u8)FIELD_GET(CQM_DC_DESC_QW_POOL_MASK, desc_qw1);
				policy = (u8)FIELD_GET(CQM_DC_DESC_QW_POLICY_MASK, desc_qw1);
				buf = FIELD_GET(CQM_DC_DESC_QW_BUFL_MASK, desc_qw1) |
				      (FIELD_GET(CQM_DC_DESC_QW_BUFH_MASK, desc_qw0) << 32);
				CQM_DEBUG(CQM_DBG_FLAG_DC_PORT,
					  "%s id:%d buf:%llx, pool:%u, policy:%u brptr: %u\n",
					  __func__, id, buf,
					  pool, policy, brptr);
				cqm_dc_buffer_return(deqport, (phys_addr_t)buf, policy, pool, brptr);
				brptr++;
				/* CQM IP requires brptr registers being
				 * written in sequence. Hence, brptr should be
				 * wrapped around for the number of brptr
				 * registers for each port.If some register is
				 * skipped during port flush, the buffer
				 * it tries to free will not be freed,
				 * resulting in buffer leak.
				 */
				brptr = brptr % brptr_num;
				total_cnt++;
			}
			id++;
		}
		CQM_DEBUG(CQM_DBG_FLAG_DC_PORT,
			  "%s flushing deq:%d loop:%d, total_rtn:%d\n",
			  __func__, deqport,
			  p_info->deq_info.num_desc * 3, total_cnt);
		/*reset dptr dqpc will be handled after port is disabled*/
	}
}

static s32 NO_OPTIMIZE do_port_setting(u32 *pmac, u32 flags, u32 *cbm_port,
				       struct cqm_pmac_port_map *local_entry)
{
	u32 l_flags;
	int i = 0;
	int phys_port = 0;
	struct pib_ctrl ctrl = {0};
	int populate = 0;
	struct umt_trig umt_ctrl;

	l_flags = (flags & FLAG_WLAN) ? DP_F_FAST_WLAN : flags;
	l_flags = (flags & DP_F_FAST_DSL) ? DP_F_FAST_DSL : l_flags;
	l_flags = (flags & DP_F_DOCSIS) ? DP_F_DOCSIS : l_flags;
	if (cbm_port[i] != CBM_PORT_NOT_APPL) {
		dev_dbg(cqm_ctrl->dev, "0x%x 0x%x\n", cbm_port[i], flags);
		populate = 1;
	}
	switch (l_flags) {
	case DP_F_FAST_ETH_WAN:
		/*set PIB bypass*/
		ctrl.pib_bypass = 1;
		ctrl.pib_en = 1;
		config_pib_ctrl(&ctrl, FLAG_PIB_BYPASS | FLAG_PIB_ENABLE);
	break;
	case DP_F_GPON:
		pib_program_overshoot(CQEM_PON_IP_IF_PIB_OVERSHOOT_BYTES);
		/*clear PIB bypass*/
		ctrl.pib_bypass = 0;
		ctrl.pib_en = 1;
		ctrl.wakeup_intr_en = 1;
		/* Below dequeue delay value of 8 is for URX only.
		 * On PRX a dequeue delay of 7 should be used.
		 */
		ctrl.deq_delay = 8;
		config_pib_ctrl(&ctrl,
				FLAG_PIB_BYPASS |
				FLAG_PIB_ENABLE |
				FLAG_PIB_DELAY |
				FLAG_PIB_WAKEUP_INTR);
	break;
	case DP_F_DOCSIS:
		/*clear PIB bypass*/
		ctrl.pib_bypass = 0;
		ctrl.pib_en = 1;
		ctrl.wakeup_intr_en = 1;
		ctrl.dc_mode = 1;
		ctrl.pkt_len_adj = 0;
		config_pib_ctrl(&ctrl,
				FLAG_PIB_BYPASS |
				FLAG_PIB_ENABLE |
				FLAG_PIB_WAKEUP_INTR |
				FLAG_PIB_DC_MODE |
				FLAG_PIB_PKT_LEN_ADJ);

		umt_ctrl.mark_en = 0;
		umt_ctrl.port_en = 1;
		deq_umt_trigger(cbm_port, &umt_ctrl);
	break;
	case DP_F_EPON:
		cbm_w32(cqm_ctrl->cqm + CBM_EPONL_BASE,
			CQM_PON_IP_BASE_ADDR_L);
		cbm_w32(cqm_ctrl->cqm + CBM_EPONH_BASE,
			CQM_PON_IP_BASE_ADDR_H);
		pib_program_overshoot(CQM_PON_IP_PKT_LEN_ADJ_BYTES);
		/*clear PIB bypass*/
		ctrl.pib_bypass = 0;
		ctrl.pib_en = 1;
		ctrl.wakeup_intr_en = 1;
		ctrl.pkt_len_adj = 1;
		ctrl.deq_delay = 0xf;
		config_pib_ctrl(&ctrl,
				FLAG_PIB_BYPASS |
				FLAG_PIB_ENABLE |
				FLAG_PIB_DELAY |
				FLAG_PIB_WAKEUP_INTR |
				FLAG_PIB_PKT_LEN_ADJ);
	break;
	case DP_F_FAST_DSL:
	case DP_F_FAST_WLAN:
		populate = 1;
		umt_ctrl.port_en = 1;
		if (cqm_ctrl->v2_flag)
			umt_ctrl.mark_en = 1;
		else
			umt_ctrl.mark_en = 0;
		deq_umt_trigger(cbm_port, &umt_ctrl);
	break;
	case DP_F_DIRECT:
		cqm_ctrl->dqm_port_info[cbm_port[0]].tx_ring_size = cqm_ctrl->dqm_port_info[cbm_port[0]].deq_info.num_desc;
		break;
	default:
	break;
	}
	if (populate)
		cqm_populate_entry(local_entry, &phys_port, cbm_port[i], flags);
	return CBM_SUCCESS;
}

static s32
NO_OPTIMIZE cqm_dp_port_dealloc(struct module *owner, u32 dev_port,
				s32 port_id, struct cbm_dp_alloc_data *dp,
				u32 flags)
{
	struct cqm_pmac_port_map *local_entry = NULL;
	struct cqm_dqm_port_info *dqp_info;
	u32 deqport = INVALID_DQP;

	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s owner: %s dev_port: %u port_id: %d flags: 0x%x\n",
		  __func__, owner->name, dev_port, port_id, flags);

	cqm_dump_dp_alloc_data(__func__, dp);

	if (port_id >= LPID_COUNT || port_id < 0)
		return DP_FAILURE;

	/* DPM deprecates deq_port from struct cbm_dp_alloc_data for DC
	 * ports. Look for CQM port from port_id.
	 */
	local_entry = cqm_get_pmac_mapping(port_id);
	if (!local_entry) {
		dev_err(cqm_ctrl->dev, "Unable to find deq port for dp_port %d\n",
			port_id);
		return DP_FAILURE;
	}
	deqport = cqm_pmac_to_deqp_from_pmac_mapping_list(local_entry);
	if (deqport == INVALID_DQP) {
		dev_err(cqm_ctrl->dev, "Unable to find deq port for dp_port %d\n",
			port_id);
		return DP_FAILURE;
	}

	CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "%s: port_id:%d, deq_port:%d\n",
		  __func__, port_id, deqport);

	dqp_info = &cqm_ctrl->dqm_port_info[deqport];
	dqp_info->allocated = P_FREE;
	if (find_dqm_port_type(deqport) == DQM_DMA_TYPE) {
		cqm_dequeue_dma_port_uninit(deqport, 0);
		cqm_dma_port_enable(deqport,
				    CBM_PORT_F_DEQUEUE_PORT |
				    CBM_PORT_F_DISABLE, 0, BUF_TYPE_INVALID);
		dqp_info->dma_ch_in_use = 0;
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
			  "%s: DMA port disabled dpid:%d, deq_port:%d\n",
			  __func__, port_id, deqport);
	} else if (find_dqm_port_type(deqport) == DQM_PON_TYPE ||
		   find_dqm_port_type(deqport) == DQM_DOCSIS_TYPE) {
		if (!(cbm_r32(cqm_ctrl->pib + PIB_CTRL) &
		    PIB_CTRL_PIB_DC_MODE_MASK))
			cqm_dequeue_dma_port_uninit(deqport, 0);

		cqm_dma_port_enable(deqport,
				    CBM_PORT_F_DEQUEUE_PORT |
				    CBM_PORT_F_DISABLE, 0, BUF_TYPE_INVALID);
		dqp_info->dma_ch_in_use = 0;
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
			  "%s: DMA or PON port disabled dpid:%d, deq_port:%d\n",
			  __func__, port_id, deqport);
	} else if (find_dqm_port_type(deqport) != DQM_ACA_TYPE) {
		cbm_w32((cqm_ctrl->deq + DQ_CPU_PORT(deqport, cfg)), 0);
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
			  "%s: CPU port disabled dpid:%d, deq_port:%d\n",
			  __func__, port_id, deqport);
	} else if (find_dqm_port_type(deqport) == DQM_ACA_TYPE) {
		cqm_dc_deqport_disable(deqport);
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
			  "%s: ACA port disabled dpid:%d, deq_port:%d\n",
			  __func__, port_id, deqport);
	}

	CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "%s: delete entry dpid:%d\n",
		  __func__, port_id);
	cqm_delete_from_list(port_id, local_entry->alloc_flags);
	cqm_dump_pmac_mapping_list();
	return DP_SUCCESS;
}

s32 NO_OPTIMIZE alloc_dynamic_dp_id(u32 flags, u32 dev_port)
{
	s32 alloc_id;
	struct cqm_pmac_port_map *dp_local_entry = NULL;
	int idx;

	if (cqm_ctrl->wav.num && (flags & DP_F_SHARE_RES)) {
		if (dev_port > 2)
			return CBM_FAILURE;
		alloc_id = find_first_bit(cqm_wav_port_mode(),
					  CQM_LPID_BITMAP_LEN) +
			   dev_port;
		dp_local_entry = is_dp_allocated(alloc_id, flags);
		if (!dp_local_entry)
			return alloc_id;
		else
			return CBM_FAILURE;
	}

	for_each_set_bit(idx, cqm_port_mode(), CQM_LPID_BITMAP_LEN) {
		if (BIT(idx) & DPID_RESERVED)
			continue;

		if (cqm_ctrl->wav.num && test_bit(idx, cqm_wav_port_mode()))
			continue;

		dp_local_entry = is_dp_allocated(idx, flags);
		if (!dp_local_entry)
			return idx;
	}
	return CBM_FAILURE;
}

/* Validate whether a port_id for static port is valid */
static s32 cqm_validate_port_id(s32 port_id)
{
	int idx;

	/* Boundary check and static port check */
	if (port_id >= CQM_TOTAL_DPID || port_id < 0 ||
	    test_bit(port_id, cqm_port_mode())) {
		dev_err(cqm_ctrl->dev,
			"pord_id %i is invalid\n", port_id);
		return CBM_FAILURE;
	}

	/* Port_type check */
	for (idx = 0; idx < ARRAY_SIZE(epg_lookup_table); idx++) {
		if (epg_lookup_table[idx].valid &&
		    epg_lookup_table[idx].lpid == (u32)port_id)
			return CBM_SUCCESS;
	}

	dev_err(cqm_ctrl->dev,
		"pord_id %i is invalid\n", port_id);
	return CBM_FAILURE;
}

static s32
NO_OPTIMIZE dp_port_alloc(struct module *owner, struct net_device *dev,
			  u32 dev_port, s32 port_id,
			  struct cbm_dp_alloc_data *data, u32 flags)
{
	s32  result = CBM_SUCCESS;
	int param_pmac = 0, pmac_present = 0;
	u32 ch_dp_port;
	u32 cbm_port[LGM_MAX_PORT_PER_EP] = {0},
	pmac[LGM_MAX_PORT_PER_EP] = {0};
	struct cqm_pmac_port_map local_entry = {0};
	u32 new_deq_port = 0;

	if (flags & DP_F_DEREGISTER)
		return cqm_dp_port_dealloc(owner, dev_port, port_id, data, flags);

	data->flags = 0;
	if (!owner)
		return CBM_FAILURE;

	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s owner: %s dev: %s dev_port: %u port_id: %d flags: 0x%x\n",
		  __func__, owner->name, dev->name, dev_port, port_id, flags);
	cqm_dump_dp_alloc_data(__func__, data);

	/*allocate with specified port id */
	if ((port_id) && (!(flags & DP_F_SHARE_RES))) {
		if (cqm_validate_port_id(port_id)) {
			dev_err(cqm_ctrl->dev,
				"port_id %i is invalid\n", port_id);
			return DP_FAILURE;
		}

		if (is_dp_allocated(port_id, flags)) {
			dev_err(cqm_ctrl->dev,
				"already allocated pmac port %d\n", port_id);
			return DP_FAILURE; /*not free*/
		}
		param_pmac = port_id;
		pmac_present = 1;
		new_deq_port = 1;
		result = get_matching_pmac(pmac, flags, cbm_port, param_pmac,
					   pmac_present);
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "%s get_matching_pmac = %d\n",
			  __func__, cbm_port[0]);
	} else { /* dynamic alloc a free port */
		port_id = alloc_dynamic_dp_id(flags, dev_port);
		if (port_id == CBM_FAILURE) {
			dev_err(cqm_ctrl->dev,
				"Failed to get a free port for module %p\n",
				owner);
			return DP_FAILURE;
		}
		param_pmac = port_id;
		if (cqm_ctrl->wav.dqpd && (flags & DP_F_SHARE_RES)) {
			pmac[0] = CBM_PMAC_DYNAMIC;
			result = CBM_SUCCESS;
			CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
				  "%s pmac[0] = CBM_PMAC_DYNAMIC\n",
				  __func__);
		} else {
			new_deq_port = 1;
			/* For DOCSIS, the port allocated in cbm_dp_port_alloc
			 * API is always the last CQM DEQ DC port. This is to
			 * be in pair with CQM ENQ DC port requirement for
			 * DOCSIS.
			 */
			if (flags & DP_F_DOCSIS) {
				if (!is_cbm_allocated(CQM_DEQ_DC_PORT_DOCSIS, DQ_PORT)) {
					pmac[0] = CBM_PMAC_DYNAMIC;
					cbm_port[0] = CQM_DEQ_DC_PORT_DOCSIS;
				} else {
					result = CBM_FAILURE;
				}
			} else {
				result = get_matching_pmac(pmac, flags,
							   cbm_port, param_pmac,
							   pmac_present);
				if (result == CBM_SUCCESS &&
				    (flags & DP_F_SHARE_RES)) {
					cqm_ctrl->wav.dqpd = cbm_port[0];
					CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
						  "%s assigned cqm_ctrl->wav.dqpd =  %d\n",
						  __func__, cbm_port[0]);
				}
			}
		}
	}

	if ((pmac[0] != CBM_PMAC_DYNAMIC &&
	     pmac[0] != port_id) || result != CBM_SUCCESS) {
		dev_err(cqm_ctrl->dev,
			"%s Error pmac[0]:%d, port_id:%d, result:%d\n",
			__func__,
			pmac[0], port_id, result);
		return DP_FAILURE;
	}
	CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
		  "%s pmac[0]:%d, cbm_port[0]: %d port_id:%d\n",
		  __func__, pmac[0], cbm_port[0], port_id);
	local_entry.port_id = port_id;
	local_entry.alloc_flags = flags;
	local_entry.owner = owner;
	local_entry.dev = dev;
	local_entry.dev_port = dev_port;
	local_entry.flags = P_ALLOCATED;
	if (data->data && data->data->link_speed_cap == SPEED_10000) {
		local_entry.link_speed_cap = SPEED_10000;
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
			  "%s dev_port %d, port_id %d, link_speed_cap: %d\n",
			  __func__, dev_port, port_id,
			  local_entry.link_speed_cap);
	}

	if (new_deq_port) {
		do_port_setting(pmac, flags, cbm_port, &local_entry);
	} else {
		cqm_populate_entry(&local_entry,
				   &ch_dp_port, cqm_ctrl->wav.dqpd,
				   flags);
		cbm_port[0] = cqm_ctrl->wav.dqpd;
	}
	cqm_add_to_list(&local_entry);
	cqm_dump_pmac_mapping_list();
	fill_dp_alloc_data(data, port_id, cbm_port[0], flags);
	cqm_dump_dp_alloc_data(__func__, data);
	CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "%s flags:0x%x cbm_deqp:%d, dp_id:%d\n",
		  __func__, flags, cbm_port[0], port_id);
	return CBM_SUCCESS;
}

static void
NO_OPTIMIZE get_enq_port_position(u32 *start, u32 *range, const u32 flags)
{
	u8 type;

	for (type = 0; type < ARRAY_SIZE(epg_lookup_table); type++) {
		if (epg_lookup_table[type].port_type & flags) {
			start[0] = epg_lookup_table[type].ipg;
			range[0] = epg_lookup_table[type].ipg_range;
			break;
		}
	}
}

static s32 NO_OPTIMIZE get_free_enqport(u32 *enqport, const u32 flags)
{
	u32 port_start = 0;
	u32 port_range = 0;

	get_enq_port_position(&port_start, &port_range, flags);
	port_range = port_start + port_range;

	/* For DOCSIS system, ENQ DC port 7 is reserved for DOCSIS. It is
	 * allocated first for flags with DOCSIS set.
	 */
	if (cqm_ctrl->lpid_config.cqm_lpid_wan_mode & LPID_WAN_DOCSIS) {
		port_range -= 1;

		if (flags & DP_F_DOCSIS) {
			if (cqm_ctrl->eqm_port_info[CQM_ENQ_DC_PORT_DOCSIS].allocated == 0) {
				*enqport = CQM_ENQ_DC_PORT_DOCSIS;
				dev_dbg(cqm_ctrl->dev, "got eqpid: %d\n", CQM_ENQ_DC_PORT_DOCSIS);
				return CBM_SUCCESS;
			}
		}
	}

	/* For non-DOCSIS or subsequent DOCSIS enq port allocation */
	while (port_start <= port_range) {
		/* !< get port availablility from global table */
		if (cqm_ctrl->eqm_port_info[port_start].allocated == 0) {
			*enqport = port_start;
			dev_dbg(cqm_ctrl->dev, "got eqpid: %d\n", port_start);
			return CBM_SUCCESS;
		}
		port_start++;
	}

	return CBM_NOTFOUND;
}

/* !< function	 : get_port_position()
 *    description: it will get rack position as per the config table
 *    param[in]	 : port_type
 *    param[out] : start, range
 */
static int
NO_OPTIMIZE get_port_position(const u32 i, u32 *start, u32 *range,
			      const u32 port_type)
{
	const struct cqm_port_config *cfg = &cqm_ctrl->cqm_cfg[i];
	int ret = CBM_FAILURE;

	switch (cfg->type) {
	case DQM_CPU_TYPE:
		if (cfg->data.dqm_cpu.cpu_port_type == port_type) {
			*start = cfg->data.dqm_cpu.port;
			*range = cfg->data.dqm_cpu.port_range;
			ret = 0;
		}
		break;
	case DQM_ACA_TYPE:
		if (cfg->data.dqm_aca.cpu_port_type == port_type) {
			*start = cfg->data.dqm_aca.port;
			*range = cfg->data.dqm_aca.port_range;
			ret = 0;
		}
		break;
	case DQM_DMA_TYPE:
		if (cfg->data.dqm_dma.port_type == port_type) {
			*start = cfg->data.dqm_dma.port;
			*range = cfg->data.dqm_dma.port_range;
			ret = 0;
		}
		break;
	case DQM_PON_TYPE:
	case DQM_DOCSIS_TYPE:
		break;
	case DQM_PP_NF_TYPE:
		if (cfg->data.dqm_dma.port_type == port_type) {
			*start = cfg->data.dqm_dma.port;
			*range = cfg->data.dqm_dma.port_range;
			ret = 0;
		}
		break;
	case EQM_CPU_TYPE:
		if (cfg->data.eqm_cpu.port_type == port_type) {
			*start = cfg->data.eqm_cpu.port;
			*range = cfg->data.eqm_cpu.port_range;
			ret = 0;
		}
		break;
	case EQM_DMA_TYPE:
		if (cfg->data.eqm_dma.port_type == port_type) {
			*start = cfg->data.eqm_dma.port;
			*range = cfg->data.eqm_dma.port_range;
			ret = 0;
		}
		break;
	default:
		ret = CBM_FAILURE;
		break;
	};
	return ret;
}

/* !< function	 : get_cqmport()
 *    description: it will get cqm port info from the cqm_cfg
		 : and cross check the availability
 *    param[in]	 : port_group, port_type, flags(eq/dq type)
 *    param[out] : port
 */
static s32
NO_OPTIMIZE get_cqmport(u32 *port, enum EQM_DQM_PORT_TYPE group,
			enum DQ_PORT_SUB_TYPE type, const u32 flags)
{
	u32 range = 0;
	u32 end = 0;
	u32 pid = 0;
	u32 i;

	for (i = 0; cqm_ctrl->cqm_cfg[i].type != NONE_TYPE; i++) {
		if (cqm_ctrl->cqm_cfg[i].type != group)
			continue;

		/* must reset to fetch fresh copy */
		range = 0; pid = 0; end = 0;
		if (get_port_position(i, &pid, &range, type) != CBM_SUCCESS)
			continue;

		end = pid + range;
		while (pid <= end) {
			if (!is_cbm_allocated(pid, flags)) {
				*port = pid;
				return CBM_SUCCESS;
			}
			pid++;
		}
	}
	dev_err(cqm_ctrl->dev, "typ:0x%x,pid:%d,rng:%d\n", type, pid, range);
	return CBM_FAILURE;
}

static s32
NO_OPTIMIZE get_pool_policy(enum DIRECTION dir, struct cqm_policy_table *p,
			    enum CQM_SUP_DEVICE type, u32 resource)
{
	s32 policy_id;

	CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "dir %d type %d resource %d\n",
		  dir, type, resource);

	policy_id = cqm_pp_policy_setup(dir, type, resource);

	if (policy_id < 0) {
		pr_info("%s :policy_id not found: dir %d type %d resource %d\n",
			__func__, dir, type, resource);
		return CBM_FAILURE;
	}

	if (dir == INGRESS) {
		p->ingress_policy = policy_id;
		p->ingress_cnt =
			cqm_ctrl->cqm_bm_policy_dts[policy_id].res_i_cnt;
	} else if (dir == EGRESS) {
		p->egress_policy = policy_id;
		p->egress_cnt =
			cqm_ctrl->cqm_bm_policy_dts[policy_id].res_e_cnt;
	}

	p->pool = get_pool_from_policy(policy_id);
	if (p->pool == INVALID_POOL_ID) {
		dev_err(cqm_ctrl->dev, "%s: invalid pool id for policy %d\n",
			__func__, policy_id);
		return CBM_FAILURE;
	}
	p->min_guaranteed = get_min_guar_from_policy(policy_id);

	return CBM_SUCCESS;
}

/* function	: do_cqmport_config()
 * description	: 1) Init call for ACA enq and deq port.
 *		: 2) Pass Invalid port no, if want to discard it.
 * param[in]	: enq & deq port no, ingress & egress policy,
 *              : default enq port config, default deq port config,
 *              : enq_bpress, deq_bpress
 *              : alloc_flags - used for special config for different port types
 * param[out]	: 0 or -1
 */
static s32
NO_OPTIMIZE do_cqmport_config(const u32 enqport, const u32 deqport,
		  const struct cqm_policy_table *info,
		  u32 def_enq_config, u32 def_deq_config,
		  enum DP_RXOUT_QOS_MODE qos_mode,
		  u32 enq_bpress, u32 deq_bpress,
		  u32 alloc_flags)
{
	struct cqm_dqm_port_info *dqp_info;
	u32 enq_config;
	u32 deq_config;
	u32 dev_qos_cfg = DFLT_DEV_QOS;

	CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
		  "%s: enqport: %d deqport: %d enq_bpress: 0x%x deq_bpress: 0x%x\n",
		  __func__, enqport, deqport, enq_bpress, deq_bpress);

	enq_config = def_enq_config;
	deq_config = def_deq_config;

	if (enq_bpress)
		enq_config |= enq_bpress;

	if (deq_bpress)
		deq_config |= deq_bpress;

	/* Special configuration for DOCSIS DC ports.
	 * ENQ DC port 7 (last): EQREQ | EQPCEN | Base policy |
	 *                       (QOS bypass & FSQM according to out_qos_mode)
	 * ENQ DC port non-data: EQREQ | EQPCEN | Base policy
	 * DEQ DC port 7 (last): 0 (Port is not enabled)
	 * DEQ DC port non-data: BUFRTN
	 */
	if (alloc_flags & DP_F_DOCSIS)
		enq_config = CFG_DC_IGP_16_EQREQ_MASK |
			     CFG_DC_IGP_16_EQPCEN_MASK;

	switch (qos_mode) {
	case DP_RXOUT_BYPASS_QOS_ONLY:
		enq_config |=	CFG_DC_IGP_16_QOSBYPSEN_MASK |
				CFG_DC_IGP_16_FSQMQEN_MASK |
				info->ingress_policy <<
				CFG_DC_IGP_16_BASEPOLICY_POS;
		if (enqport % 2)
			dev_qos_cfg = PONG_DEV_QOS;
		else
			dev_qos_cfg = PING_DEV_QOS;
		break;
	case DP_RXOUT_QOS:
		enq_config |=	info->egress_policy <<
				CFG_DC_IGP_16_BASEPOLICY_POS;
		deq_config |=	deqport << CFG_DMA_EGP_27_EPMAP_POS |
				info->pool << CFG_ACA_EGP_19_POOL_POS |
				info->egress_policy <<
				CFG_ACA_EGP_19_POLICY_POS;
		break;
	case DP_RXOUT_BYPASS_QOS_FSQM:
		enq_config |=	CFG_DC_IGP_16_QOSBYPSEN_MASK |
				info->ingress_policy <<
				CFG_DC_IGP_16_BASEPOLICY_POS;
		break;
	case DP_RXOUT_QOS_MAX:
	default:
		dev_err(cqm_ctrl->dev, "unknown qos mode: 0x%x\n", alloc_flags);
		return CBM_FAILURE;
	}

	if (alloc_flags & DP_F_DOCSIS) {
		/* Unset CFG_DC_IGP_16_QOSBYPSEN_MASK and
		 * CFG_DC_IGP_16_FSQMQEN_MASK for non-data ENQ DC port
		 */
		if (enqport != CQM_ENQ_DC_PORT_DOCSIS) {
			enq_config &= ~(CFG_DC_IGP_16_QOSBYPSEN_MASK |
					CFG_DC_IGP_16_FSQMQEN_MASK);
		} else {
			cbm_w32(cqm_ctrl->enq + EQ_DC_PORT(enqport, desc_conv), 0x2);
		}

		if (deqport == CQM_DEQ_DC_PORT_DOCSIS)
			deq_config = 0;
		else
			deq_config = CFG_ACA_EGP_19_BUFRTN_MASK;
	}

	if (cqm_is_port_valid(enqport)) {
		cbm_w32(cqm_ctrl->enq + EQ_DC_PORT(enqport, cfg), enq_config);
		cbm_w32(cqm_ctrl->enq + EQ_DC_PORT(enqport, devqmap0to7),
			dev_qos_cfg);
		cbm_w32(cqm_ctrl->enq + EQ_DC_PORT(enqport, devqmap8to15),
			dev_qos_cfg);
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
			  "%s enqport:%d :dev_qos_cfg:%x, enq_config:%x\n",
			  __func__, enqport, dev_qos_cfg, enq_config);
	}

	if (cqm_is_port_valid(deqport)) {
		dqp_info = &cqm_ctrl->dqm_port_info[deqport];
		cbm_w32((cqm_ctrl->deq + DQ_CPU_PORT(deqport, dptr)),
			dqp_info->deq_info.num_desc - 1);
		cbm_w32(cqm_ctrl->deq + DQ_DC_PORT(deqport, cfg), deq_config);
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
			  "%s deqport:%d dptr:%x, deq_config:%x, dptr:%x\n",
			  __func__, deqport,
			  dqp_info->deq_info.num_desc - 1, deq_config,
			  cbm_r32(cqm_ctrl->deq + DQ_CPU_PORT(deqport, dptr)));
	}
	return CBM_SUCCESS;
}

static u32
NO_OPTIMIZE cqmport_enq_bpress(enum DP_BPRESS bpress_out, enum DP_BPRESS bpress_alloc)
{
	u32 enq_bpress = 0;

	if (bpress_out == DP_BPRESS_EN || bpress_out == DP_BPRESS_NA)
		enq_bpress |= CFG_DC_IGP_16_BP_EN_MASK;

	if (bpress_alloc == DP_BPRESS_EN || bpress_alloc == DP_BPRESS_NA)
		enq_bpress |= CFG_DC_IGP_16_BRBPEN_MASK;

	CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
		  "%s: bpress_out: %d bpress_alloc: %d enq_bpress: 0x%x\n",
		  __func__, bpress_out, bpress_alloc, enq_bpress);

	return enq_bpress;
}

static u32
NO_OPTIMIZE cqmport_deq_bpress(enum DP_BPRESS bpress_in, enum DP_BPRESS bpress_free)
{
	u32 deq_bpress = 0;

	if (bpress_in == DP_BPRESS_EN || bpress_in == DP_BPRESS_NA)
		deq_bpress |= CFG_ACA_EGP_19_DQBPEN_MASK;

	if (bpress_free == DP_BPRESS_EN || bpress_free == DP_BPRESS_NA)
		deq_bpress |= CFG_ACA_EGP_19_BFBPEN_MASK;

	CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
		  "%s: bpress_in: %d bpress_free: %d enq_bpress: 0x%x\n",
		  __func__, bpress_in, bpress_free, deq_bpress);

	return deq_bpress;
}

/*!< select buffer size based on pool from preconfigured register */
static inline u32 get_buf_size_from_cqmtop(u16 pool, void *cqm_base)
{
	u32 buf_select = 0;
	u32 config = 0;

	if (pool < 8)
		config = cbm_r32(cqm_base + CBM_BUF_SIZE0_7);
	else
		config = cbm_r32(cqm_base + CBM_BUF_SIZE8_15);

	buf_select = (config >> (4 * (pool % 8))) & 0x7;

	return buf_size_table[buf_select];
}

static s32
NO_OPTIMIZE fill_dp_alloc_complete_tx(struct cbm_dp_alloc_complete_data *dp,
				      const u32 i, u32 dp_port, u32 alloc_flags,
				      struct cqm_pmac_port_map *dp_entry)
{
	void *p_dma = cqm_get_dmadesc_64_phys();
	void *deq_base = cqm_get_deq_base();
	void *v_dma = cqm_get_dmadesc_64();
	struct cqm_dqm_port_info *info;
	struct cqm_dqm_port_info *info_n;
	struct cqm_dqm_port_info *info_txin0;
	u32 alloc_buff = 0;
	enum CQM_SUP_DEVICE type;
	u32 dqpid;
	u32 config;
	struct cqm_policy_table *policy;
	u32 result, dq_port;
	int res, cnt = 0;
	u32 deq_bpress;

	u32 cbm_port[LGM_MAX_PORT_PER_EP] = {0},
		pmac[LGM_MAX_PORT_PER_EP] = {0};

	dqpid = cqm_dp_to_deqp_from_pmac_mapping_list(dp_port);
	if (dqpid == INVALID_DQP) {
		dev_err(cqm_ctrl->dev, "Unable to find deq port for dp_port %d\n",
			dp_port);
		return DP_FAILURE;
	}

	info = &cqm_ctrl->dqm_port_info[dqpid];
	policy = &info->policy_res;

	CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
		  "%s: i:%d  dqpid:%d\n", __func__, i, dqpid);

	if (alloc_flags & DP_F_SHARE_RES) {
		if (cqm_ctrl->wav.dqpd)
			info_txin0 =
				&cqm_ctrl->dqm_port_info[cqm_ctrl->wav.dqpd];
	}

	if (i > 0) {
		result = get_matching_pmac(pmac, alloc_flags, cbm_port, dp_port, 0);
		if (result == CBM_FAILURE) {
			dev_err(cqm_ctrl->dev, "get_matching pmac error\n");
			return DP_FAILURE;
		}
		dp->tx_ring[i]->tx_deq_port = cbm_port[0];
		info_n = &cqm_ctrl->dqm_port_info[cbm_port[0]];
		memcpy(&info_n->policy_res, &info->policy_res,
		       sizeof(struct cqm_policy_table));

		do_port_setting(pmac, alloc_flags, cbm_port, dp_entry);
	} else {
		dp->tx_ring[i]->tx_deq_port = dqpid;
	}

	type = get_type_from_alloc_flags(dp_entry->alloc_flags);
	if (!(alloc_flags & DP_F_SHARE_RES) ||
	    (cqm_ctrl->wav.dqpd && !info_txin0->policy_res.egress_policy)) {
		if (get_pool_policy(EGRESS, &info->policy_res,
				    type, dp->bm_policy_res_id) < 0)
			return CBM_FAILURE;
		/*Add a check for buffer leak in Tx policy*/
		do {
			res = get_policy_stats(info->policy_res.egress_policy,
					       CQM_TX, &alloc_buff);
			if (res || alloc_buff > 0)
				usleep_range(10000, 15000);
		} while (alloc_buff && ((cnt++) < 4));
		/*if the allocated buffer is non zero value, return failure*/
		if (res || alloc_buff) {
			dev_err(cqm_ctrl->dev,
				"Buffer Leak detected for Tx policy%d,num:%d\n",
				info->policy_res.egress_policy,
				alloc_buff);
			return CBM_FAILURE;
		}
		dq_port = dp->tx_ring[i]->tx_deq_port;
		dev_info(cqm_ctrl->dev, "DQPC 0x%x BPRC 0x%x\n",
			 cbm_r32(cqm_ctrl->deq + DQ_DC_PORT(dq_port, dqpc)),
			 cbm_r32(cqm_ctrl->deq + DQ_DC_PORT(dq_port, bprc)));
	}
	deq_bpress = cqmport_deq_bpress(dp->tx_ring[i]->bpress_in,
					dp->tx_ring[i]->bpress_free);
	do_cqmport_config(CBM_PORT_INVALID, dp->tx_ring[i]->tx_deq_port,
			  &info->policy_res, CQM_DEFAULT_DC_ENQ_PORT_CFG,
			  CQM_DEFAULT_DC_DEQ_PORT_CFG, DP_RXOUT_QOS,
			  0, deq_bpress, dp_entry->alloc_flags);
	dqpid = dp->tx_ring[i]->tx_deq_port;
	config = cbm_r32(deq_base + DQ_DC_PORT(dqpid, cfg));
	dp->tx_ring[i]->in_deq_ring_size = info->deq_info.num_desc;
	dp->tx_ring[i]->in_deq_paddr = p_dma + CQM_DQM_DC_DESC(dqpid, 0);
	dp->tx_ring[i]->in_deq_vaddr = v_dma + CQM_DQM_DC_DESC(dqpid, 0);
	dp->tx_ring[i]->out_free_ring_size = info->deq_info.num_free_burst;
	dp->tx_ring[i]->out_free_paddr = p_dma + CQM_DQM_DC_BUF_RTN(dqpid, 0);
	dp->tx_ring[i]->out_free_vaddr = v_dma + CQM_DQM_DC_BUF_RTN(dqpid, 0);
	if (dp->tx_ring[i]->f_out_auto_free)
		cbm_w32(deq_base + DQ_DC_PORT(dqpid, cfg),
			config | CFG_ACA_EGP_19_BPRTNDETEN_MASK);
	dp->tx_ring[i]->txpush_addr = (cqm_ctrl->txpush_phys +
		(TXPUSH_CMD_RX_EGP_1 * dqpid));
	dp->tx_ring[i]->txpush_addr_qos = (void *)((unsigned long)
		(cqm_ctrl->txpush_phys +
		TXPUSH_CMD_RX_EGP_1 * dqpid) |
		BIT(28));
	dp->tx_ring[i]->num_tx_pkt = info->policy_res.min_guaranteed;
	dp->tx_ring[i]->txout_policy_base = info->policy_res.egress_policy;
	dp->tx_ring[i]->tx_poolid = info->policy_res.pool;
	dp->tx_ring[i]->policy_num = info->policy_res.egress_cnt;
	CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
		  "%s TX ring %d assigned policy start:%d, cnt:%d, pool:%d, num:%d\n",
		  __func__, i,
		  info->policy_res.egress_policy,
		  info->policy_res.egress_cnt,
		  info->policy_res.pool, dp->num_tx_ring);
	dp_entry->tx_deq_port_id[i] = dp->tx_ring[i]->tx_deq_port;
	return CBM_SUCCESS;
}

static bool
NO_OPTIMIZE fill_dp_alloc_complete_rx(struct cbm_dp_alloc_complete_data *dp,
			  struct cqm_policy_table *policy, const u32 i)
{
	u32 dqpid = dp->rx_ring[i]->out_cqm_deq_port_id;
	u32 eqpid = dp->rx_ring[i]->out_enq_port_id;
	void *p_dma = cqm_get_dmadesc_64_phys();
	void *v_dma = cqm_get_dmadesc_64();
	struct cqm_dqm_port_info *dqp_info;
	struct cqm_eqm_port_info *eqp_info;
	void *cqm_base = cqm_ctrl->cqm;
	u8 dma_ctrl[DMA_CH_STR_LEN];
	void *enq = cqm_ctrl->enq;
	u32 req_rxring_size;
	u32 def_rxring_size;
	u32 dma_controller;

	CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "%s, dqpid:%d, eqpid:%d\n",
		  __func__, dqpid, eqpid);
	def_rxring_size = cqm_ctrl->eqm_port_info[eqpid].eq_info.num_desc;
	req_rxring_size = dp->rx_ring[i]->out_enq_ring_size;
	dqp_info = &cqm_ctrl->dqm_port_info[dqpid];
	eqp_info = &cqm_ctrl->eqm_port_info[eqpid];

	/* !< tune down descriptor ring size in demand */
	if (req_rxring_size && req_rxring_size < def_rxring_size) {
		cbm_w32(enq + EQ_DC_PORT(eqpid, dptr), req_rxring_size - 1);
	} else {
		cbm_w32(enq + EQ_DC_PORT(eqpid, dptr), def_rxring_size - 1);
		dp->rx_ring[i]->out_enq_ring_size = def_rxring_size;
	}

	dp->rx_ring[i]->num_pkt = policy->min_guaranteed;
	dp->rx_ring[i]->out_enq_port_id = eqpid;
	dp->rx_ring[i]->out_enq_paddr = p_dma + CQM_EQM_DC_DESC(eqpid, 0);
	dp->rx_ring[i]->out_enq_vaddr = v_dma + CQM_EQM_DC_DESC(eqpid, 0);

	eqp_info->in_alloc_paddr = p_dma + CQM_EQM_DC_BUF_ALLOC(eqpid, 0);
	eqp_info->in_alloc_vaddr = v_dma + CQM_EQM_DC_BUF_ALLOC(eqpid, 0);

	dp->rx_ring[i]->in_alloc_paddr = eqp_info->in_alloc_paddr;
	dp->rx_ring[i]->in_alloc_vaddr = eqp_info->in_alloc_vaddr;

	dp->rx_ring[i]->rx_policy_base = policy->ingress_policy;
	dp->rx_ring[i]->policy_num = policy->ingress_cnt;
	dp->rx_ring[i]->rx_poolid = policy->pool;
	CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
		  "%s RX ring %d assigned policy start:%d, cnt:%d, pool:%d\n",
		  __func__, i,
		  policy->ingress_policy,
		  policy->ingress_cnt,
		  policy->pool);
	dp->rx_ring[i]->rx_pkt_size =
		get_buf_size_from_cqmtop(policy->pool, cqm_base);
	dp->rx_ring[i]->in_alloc_ring_size = eqp_info->eq_info.num_alloc_desc;

	if (dqp_info->dma_dt_init_type == DEQ_DMA_CHNL) {
		if (dqp_info->dma_dt_ctrl == 3)
			snprintf(dma_ctrl, sizeof(dma_ctrl), "DMA%d",
				 dqp_info->dma_dt_ctrl);
		else
			snprintf(dma_ctrl, sizeof(dma_ctrl), "DMA%dTX",
				 dqp_info->dma_dt_ctrl);

		dma_controller = dp_dma_get_controller(dma_ctrl);
		dp->rx_ring[i]->out_dma_ch_to_gswip =
			dp_dma_set_id(dma_controller, 0, dqp_info->dma_dt_ch);
		CQM_DEBUG(CQM_DBG_FLAG_DMA_CTRL, "%s out_dma_ch_to_gswip:%d\n",
			  __func__, dp->rx_ring[i]->out_dma_ch_to_gswip);

		dp->rx_ring[i]->num_out_tx_dma_ch = DEQ_DMA_CHNL;
	}
	return true;
}

static void do_umt_mux_config(u32 port, enum umt_rx_msg_mode mode, enum umt_rx_src src)
{
	u32 mux_cfg = cbm_r32(cqm_ctrl->cqm + CBM_UMTRX_MUX_SEL);
	u32 eqm_dc_start_id = cqm_ctrl->eqm_pid_cfg.aca_start;

	if (mode == UMT_RXIN_MSG_ADD || mode == UMT_4MSG_MODE)
		mux_cfg |= 1UL << ((port % eqm_dc_start_id) +
			CBM_UMTRX_MUX_SEL_RXCNTSEL_POS);
	else
		mux_cfg &= ~(1UL << ((port % eqm_dc_start_id) +
			CBM_UMTRX_MUX_SEL_RXCNTSEL_POS));

	if (src)
		mux_cfg |= 1UL << (port % eqm_dc_start_id);
	else
		mux_cfg &= ~(1UL << (port % eqm_dc_start_id));

	cbm_w32((cqm_ctrl->cqm + CBM_UMTRX_MUX_SEL), mux_cfg);
}

static void dc_deq_port_bptr_autodetect(u32 dqpid, u32 enable)
{
	u32 config;
	void *deq_base = cqm_get_deq_base();

	config = cbm_r32(deq_base + DQ_DC_PORT(dqpid, cfg));
	config &= ~CFG_ACA_EGP_19_BPRTNDETEN_MASK;
	config |= enable << CFG_ACA_EGP_19_BPRTNDETEN_POS;
	cbm_w32(deq_base + DQ_DC_PORT(dqpid, cfg), config);
}

/* dc_buffer_alloc_fifo_flush reads the enq buff alloc register
 * make sure the allocated buffer is in align with desc ring size (32)
 */
static s32 dc_buffer_alloc_fifo_flush(u32 enqport)
{
	u64 dc_buf_ptr = 0;
	u32 desc_idx, enq_config;
	u32 dptr_nd = cbm_r32(cqm_ctrl->enq + EQ_DC_PORT(enqport, dptr));
	u32 npbpc = cbm_r32(cqm_ctrl->enq + EQ_DC_PORT(enqport, npbpc));
	u32 rmd_idx = 0;

	dptr_nd &= DPTR_DC_IGP_16_ND_MASK;

	rmd_idx = npbpc % (dptr_nd + 1);
	if (!rmd_idx) {
		CQM_DEBUG(CQM_DBG_FLAG_DC_PORT,
			  "%s no buf alloc fifo flush: enqport:%d dptr_nd:%d npbpc:%d\n",
			  __func__, enqport, dptr_nd, npbpc);
		return 0;
	}

	enq_config = cbm_r32(cqm_ctrl->enq + EQ_DC_PORT(enqport, cfg));
	enq_config &= ~CFG_DC_IGP_16_BRBPEN_MASK;
	cbm_w32(cqm_ctrl->enq + EQ_DC_PORT(enqport, cfg), enq_config);
	for (desc_idx = rmd_idx; desc_idx <= dptr_nd; desc_idx++) {
		dc_buf_ptr = readq(cqm_ctrl->dmadesc_64 +
				      CQM_EQM_DC_BUF_ALLOC(enqport, desc_idx));
	}
	CQM_DEBUG(CQM_DBG_FLAG_DC_PORT,
		  "%s buff:%lx flushing idx:%d to :%d, offset:%x\n",
		  __func__, (unsigned long)dc_buf_ptr, rmd_idx,
		  dptr_nd, CQM_EQM_DC_BUF_ALLOC(enqport, rmd_idx));
	return 0;
}

static u32 metadata_check(u32 in_meta, u32 *m_p, u32 cnt)
{
	u32 i;

	for (i = 0; i < cnt; i++) {
		if (in_meta == m_p[i])
			return 1;
	}
	return 0;
}

static void
NO_OPTIMIZE update_meta_info(u32 dir, u32 cnt, u32 start,
			     struct meta_pool_s *m_p, u32 port)
{
	u32 policy, i, pool;
	struct pp_bmgr_policy_stats pp_stats;
	u32 meta_marking;
	u32 m_cnt;

	if (dir)
		port = port - cqm_ctrl->dqm_pid_cfg.aca_start;
	else
		port = port - cqm_ctrl->eqm_pid_cfg.aca_start;

	for (i = 0; i < cnt; i++) {
		policy = start + i;
		pp_bmgr_policy_stats_get(&pp_stats, policy);
		if (pp_stats.policy_alloc_buff == 0)
			continue;
		pool = get_pool_from_policy(policy);
		if (pool == INVALID_POOL_ID) {
			dev_warn(cqm_ctrl->dev, "%s: invalid pool id for policy %d\n",
				 __func__, policy);
			continue;
		}
		m_cnt = m_p[pool].meta_cnt;
		meta_marking = 0x80000000 | policy << 16 |
			pool << 24 | port << 8 | dir << 15;
		m_p[pool].meta_a[m_cnt] = meta_marking;
		m_p[pool].meta_cnt++;
		m_p[pool].buff_cnt += pp_stats.policy_alloc_buff;
		CQM_DEBUG(CQM_DBG_FLAG_MARKING,
			  "meta:0x%x idx:%d t_alloc:%d pool:%d policy:%d dir:%d port:%d alloc:%d\n",
			  m_p[pool].meta_a[m_cnt], m_p[pool].meta_cnt,
			  m_p[pool].buff_cnt, pool, policy, dir,
			  port, pp_stats.policy_alloc_buff);
	}
}

static int
NO_OPTIMIZE free_rxin_buffers(u32 pool, u32 policy,
			      const u32 enq, const u32 deq_port)
{
	unsigned long buff_ptr, ret_pa;
	u32 bm_mark, desc_id, brptr_num;
	int buf_size, i, j, total_free, cnt;
	struct cqm_dqm_port_info *dqp_info;
	struct cqm_eqm_port_info *eqp_info;
	struct meta_pool_s *meta_pool_a;
	u32 alloc_buff;

	meta_pool_a =
		devm_kzalloc(cqm_ctrl->dev,
			     sizeof(struct meta_pool_s) *
			     CQM_LGM_TOTAL_BM_POOLS,
			     GFP_KERNEL);
	if (!meta_pool_a)
		return CBM_FAILURE;

	eqp_info = &cqm_ctrl->eqm_port_info[enq];
	dqp_info = &cqm_ctrl->dqm_port_info[deq_port];

	/*enq port policy*/
	update_meta_info(0, eqp_info->policy_res.ingress_cnt,
			 eqp_info->policy_res.ingress_policy,
			 meta_pool_a, enq);
	/*deq port policy*/
	update_meta_info(1, dqp_info->policy_res.egress_cnt,
			 dqp_info->policy_res.egress_policy,
			 meta_pool_a, deq_port);
	dev_info(cqm_ctrl->dev, "before cleanup\n");
	get_policy_stats(dqp_info->policy_res.egress_policy, CQM_TX, &alloc_buff);
	get_policy_stats(eqp_info->policy_res.ingress_policy, CQM_RX, &alloc_buff);
	dev_info(cqm_ctrl->dev, "DQPC 0x%x BPRC 0x%x\n",
		 cbm_r32(cqm_ctrl->deq + DQ_DC_PORT(deq_port, dqpc)),
		 cbm_r32(cqm_ctrl->deq + DQ_DC_PORT(deq_port, bprc)));
	dev_info(cqm_ctrl->dev, "EQPC 0x%x NBPC 0x%x\n",
		 cbm_r32(cqm_ctrl->enq + EQ_DC_PORT(enq, eqpc)),
		 cbm_r32(cqm_ctrl->enq + EQ_DC_PORT(enq, npbpc)));
	/* num_desc could be reduced to 16 if the interface requests for it.
	 * Here we use num_free_burst for the number of brptr registers
	 * for each port.
	 */
	brptr_num = cqm_ctrl->dqm_port_info[deq_port].deq_info.num_free_burst;
	dc_deq_port_bptr_autodetect(deq_port, 0);
	desc_id = cbm_r32(cqm_ctrl->deq +  DQ_DC_PORT(deq_port, brptr));

	for (j = 0; j < CQM_LGM_TOTAL_BM_POOLS; j++) {
		if (meta_pool_a[j].meta_cnt == 0)
			continue;

		pool = j;
		cnt = meta_pool_a[pool].meta_cnt;
		buff_ptr = bm_pool_conf[pool].pool_start_low + BM_MARK_OFFSET;
		total_free = 0;
		for (i = 0; i < bm_pool_conf[pool].buf_frm_num; i++) {
			buf_size = bm_pool_conf[pool].buf_frm_size;
			#ifndef LGM_STEP0
			ret_pa = dma_map_single_attrs(cqm_ctrl->dev,
						      (void *)buff_ptr,
						      4,
						      DMA_FROM_DEVICE,
						      DMA_ATTR_NON_CONSISTENT);
			#endif
			bm_mark = *((u32 *)buff_ptr);
			if (metadata_check(bm_mark,
					   meta_pool_a[pool].meta_a,
					   cnt)) {
				/* ((u32 *)buff_ptr) = 0;
				 * demarking handled by dc port
				 */
				/* free the buff*/
				policy = (bm_mark >> 16) & 0xff;
				CQM_DEBUG(CQM_DBG_FLAG_MARKING_BUFF,
					  "bm_mark 0x%x, 0x%lx pool:%d, ret_pa:%lx, policy:%d\n",
					  bm_mark, (unsigned long)buff_ptr,
					  pool, (unsigned long)ret_pa, policy);

				cqm_dc_buffer_return(deq_port, ret_pa,
						     policy, pool, desc_id);
				desc_id++;
				desc_id %= brptr_num;
				total_free++;
			}
			buff_ptr += bm_pool_conf[pool].buf_frm_size;
			if (total_free >= meta_pool_a[pool].buff_cnt) {
				CQM_DEBUG(CQM_DBG_FLAG_DC_PORT,
					  "%s: DC port total_free:%d, pool:%d, ptr:%lx, marking:%x\n",
					  __func__, total_free, pool,
					  (unsigned long)ret_pa,
					  bm_mark);
				break;
			}
			cpu_relax();
		}
	}
	dc_deq_port_bptr_autodetect(deq_port, 1);
	devm_kfree(cqm_ctrl->dev, meta_pool_a);
	dev_info(cqm_ctrl->dev, "After cleanup\n");
	get_policy_stats(dqp_info->policy_res.egress_policy, CQM_TX, &alloc_buff);
	get_policy_stats(eqp_info->policy_res.ingress_policy, CQM_RX, &alloc_buff);
	dev_info(cqm_ctrl->dev, "DQPC 0x%x BPRC 0x%x\n",
		 cbm_r32(cqm_ctrl->deq + DQ_DC_PORT(deq_port, dqpc)),
		 cbm_r32(cqm_ctrl->deq + DQ_DC_PORT(deq_port, bprc)));
	dev_info(cqm_ctrl->dev, "EQPC 0x%x NBPC 0x%x\n",
		 cbm_r32(cqm_ctrl->enq + EQ_DC_PORT(enq, eqpc)),
		 cbm_r32(cqm_ctrl->enq + EQ_DC_PORT(enq, npbpc)));
	return CBM_SUCCESS;
}

static bool NO_OPTIMIZE cqm_release_enqport(const u32 enq, const u32 dc_deq)
{
	struct cqm_eqm_port_info *info = &cqm_ctrl->eqm_port_info[enq];
	u32 type = find_eqm_port_type(enq);
	u32 policy, pool;

	switch (type) {
	case EQM_CPU_TYPE:
	case EQM_VM_TYPE:
		cbm_w32((cqm_ctrl->enq + EQ_CPU_PORT(enq, cfg)), 0);
		break;
	case EQM_ACA_TYPE:
		if (cqm_ctrl->v2_flag) {
			policy = get_bm_policy_from_cfg_reg(enq);
			pool = get_pool_from_policy(policy);
			if (pool == INVALID_POOL_ID) {
				dev_err(cqm_ctrl->dev, "%s: invalid pool id for policy %d\n",
					__func__, policy);
				return false;
			}
			free_rxin_buffers(pool, policy, enq, dc_deq);
		}
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
			  "%s release dc_enq port:%d, reset npbpc to 0\n",
			  __func__, enq);

		cqm_dc_enqport_disable(enq);
		break;
	case EQM_DMA_TYPE:
		cqm_dma_port_enable(enq, CBM_PORT_F_DISABLE,
				    fsqm_pool_conf[0].buf_frm_size,
				    BUF_TYPE_FSQM);
		break;
	default:
		return false;
	};
	info->allocated = P_FREE;
	return true;
}

static size_t cqm_get_ssb_pool_size(void)
{
	int idx;
	size_t size = 0;

	for (idx = 0; idx < CQM_LGM_TOTAL_BM_POOLS; idx++)
		if (cqm_ctrl->lgm_pool_type[idx] == SSB_NIOC_SHARED ||
		    cqm_ctrl->lgm_pool_type[idx] == SSB_LROUC_NIOC_SHARED ||
		    cqm_ctrl->lgm_pool_type[idx] == SSB_LROUC_NIOC_ISOLATED)
			size += (size_t)cqm_ctrl->lgm_pool_ptrs[idx] *
				cqm_ctrl->lgm_pool_size[idx];

	return size;
}

/* Helper function to get the index of first non-SSB pool.
 * For 5-pool configuration with pool 0 being SSB pool,
 * this function should return 1.
 * For 4-pool configuration without SSB pool, it should return 0.
 * Otherwise, it returns -1.
 */

static int cqm_get_first_system_pool_idx(int *index)
{
	int idx;

	for (idx = 0; idx < CQM_LGM_TOTAL_BM_POOLS; idx++) {
		if (cqm_ctrl->lgm_pool_type[idx] == CQM_NIOC_SHARED)
			break;
	}
	if (idx >= CQM_LGM_TOTAL_BM_POOLS) {
		dev_err(cqm_ctrl->dev, "Unable to find nioc shared pool\n");
		return CBM_FAILURE;
	}
	*index = idx;
	return CBM_SUCCESS;
}

static s32 program_pcidata(struct cbm_dp_alloc_complete_data *dp, int p_idx,
			   int reg, u32 alloc_flags)
{
	u32 start;
	u32 idx, idx_t;
	int i;
	void *sysdata = dp->opt_param.pcidata;
	size_t size;
	int start_idx;

#if IS_ENABLED(CONFIG_MXL_CBM_TEST) || IS_ENABLED(CONFIG_MXL_UMT_TEST)
	if (!sysdata) {
		dev_info(cqm_ctrl->dev, "%s pcidata is NULL\n",
			 __func__);
		return CBM_SUCCESS;
	}
#else
	if (!sysdata) {
		dev_err(cqm_ctrl->dev, "%s pcidata is NULL\n",
			__func__);
		return CBM_FAILURE;
	}
#endif

	idx = cqm_ctrl->atu_index[p_idx].num_idx;
	if (!reg) {
		if (alloc_flags & DP_F_SHARE_RES) {
			cqm_ctrl->wav.atu_refcnt--;
			if (cqm_ctrl->wav.atu_refcnt)
				return CBM_SUCCESS;

			p_idx = cqm_ctrl->wav.atu_portref;
			idx = cqm_ctrl->atu_index[p_idx].num_idx;
		}
		for (i = 0; i < idx; i++) {
			idx_t = cqm_ctrl->atu_index[p_idx].atu_idx[i];
			#if IS_ENABLED(CONFIG_PCIE_INTEL_GW)
			if (intel_pcie_dc_iatu_inbound_remove(sysdata,
							      idx_t))
				return CBM_FAILURE;
			#endif
			cqm_ctrl->atu_index[p_idx].num_idx--;
		}
		return CBM_SUCCESS;
	}

	/*Hardcode the range(0-3GB) until DCDP DTS solution is available*/
	if (alloc_flags & FLAG_WLAN) {
		start = 0x2000000;
		size = 0xbe000000;
	} else {
		/* buffer from SSB pool should not be included */
		if (cqm_get_first_system_pool_idx(&start_idx)) {
			dev_err(cqm_ctrl->dev,
				"failed to find system pool start idx\n");
			return CBM_FAILURE;
		}
		start = nioc_addr_to_ioc_addr(cqm_ctrl->bm_buf_phy[start_idx]);
		size = cqm_ctrl->max_mem_alloc_sys;
	}

	/* force to program single iATU inbound region for wav700 */
	if ((alloc_flags & DP_F_SHARE_RES) && cqm_ctrl->wav.atu_refcnt) {
		cqm_ctrl->wav.atu_refcnt++;
		dev_warn(cqm_ctrl->dev,
			 "%s ignored iATU programming for wav700 port:%d\n",
			 __func__, p_idx);
		return CBM_SUCCESS;
	}

	#if IS_ENABLED(CONFIG_PCIE_INTEL_GW)
	if (intel_pcie_dc_iatu_inbound_add(sysdata,
					   start,
					   size,
					   &idx_t)) {
		dev_err(cqm_ctrl->dev, "%s iATU programming failed\n",
			__func__);
		return CBM_FAILURE;
	}
	#endif

	if (idx_t >= MAX_ATU)
		dev_err(cqm_ctrl->dev, "%d ATU index exceeds max %d\n", idx_t,
			MAX_ATU);
	cqm_ctrl->atu_index[p_idx].atu_idx[idx] = idx_t;
	cqm_ctrl->atu_index[p_idx].num_idx++;
	if (alloc_flags & DP_F_SHARE_RES) {
		cqm_ctrl->wav.atu_portref = p_idx;
		cqm_ctrl->wav.atu_refcnt++;
	}
	return CBM_SUCCESS;
}

static void cqm_dc_deqport_disable(u32 deq_port)
{
	cbm_w32((cqm_ctrl->deq + DQ_DC_PORT(deq_port, cfg)), 0);
	cbm_w32((cqm_ctrl->deq + DQ_DC_PORT(deq_port, dqpc)), 0);
	cbm_w32((cqm_ctrl->deq + DQ_DC_PORT(deq_port, dptr)), 0);
	cbm_w32((cqm_ctrl->deq + DQ_DC_PORT(deq_port, brptr)), 0);
	cbm_w32((cqm_ctrl->deq + DQ_DC_PORT(deq_port, bprc)), 0);

	dev_info(cqm_ctrl->dev, "After %s: port: %u DQPC 0x%x BPRC 0x%x\n",
		 __func__, deq_port,
		 cbm_r32(cqm_ctrl->deq + DQ_DC_PORT(deq_port, dqpc)),
		 cbm_r32(cqm_ctrl->deq + DQ_DC_PORT(deq_port, bprc)));
}

static void cqm_dc_enqport_disable(u32 enq)
{
	dc_buffer_alloc_fifo_flush(enq);

	dev_info(cqm_ctrl->dev, "Port: %u EQPC 0x%x NBPC 0x%x\n",
		 enq,
		 cbm_r32(cqm_ctrl->enq + EQ_DC_PORT(enq, eqpc)),
		 cbm_r32(cqm_ctrl->enq + EQ_DC_PORT(enq, npbpc)));

	cbm_w32((cqm_ctrl->enq + EQ_DC_PORT(enq, cfg)), 0);
	cbm_w32((cqm_ctrl->enq + EQ_DC_PORT(enq, npbpc)), 0);
	cbm_w32((cqm_ctrl->enq + EQ_DC_PORT(enq, eqpc)), 0);
	/*set dptr to reset value*/
	cbm_w32((cqm_ctrl->enq + EQ_DC_PORT(enq, dptr)), 1);

	dev_info(cqm_ctrl->dev, "After %s: port: %u EQPC 0x%x NBPC 0x%x\n",
		 __func__, enq,
		 cbm_r32(cqm_ctrl->enq + EQ_DC_PORT(enq, eqpc)),
		 cbm_r32(cqm_ctrl->enq + EQ_DC_PORT(enq, npbpc)));
}

static s32
NO_OPTIMIZE dp_port_dealloc_complete(struct cbm_dp_alloc_complete_data *dp,
				     s32 port_id, u32 pci_flag,  u32 flags)
{
	struct cqm_bm_policy_params *info;
	u8 i, id, start, end;
	u32 eqp_id;
	struct cqm_dqm_port_info *dqp_info;
	struct cqm_eqm_port_info *eqp_info;
	struct cqm_pmac_port_map *entry_ptr = NULL;
	u8 sh_f;
	u32 deq_port;

	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s port_id: %d pci_flag: 0x%x flags: 0x%x\n",
		  __func__, port_id, pci_flag, flags);

	cqm_dump_dp_alloc_complete_data(__func__, dp);

	entry_ptr = cqm_get_pmac_mapping(port_id);
	if (!entry_ptr) {
		dev_err(cqm_ctrl->dev, "%s invalid port_id\n",
			__func__);
		return CBM_FAILURE;
	}
	sh_f = (entry_ptr->alloc_flags & DP_F_SHARE_RES) ? 1 : 0;

	/* Verify whether enq and deq ports in rx_ring and tx_ring match
	 * CQM internal record.
	 */
	for (id = 0; id < entry_ptr->num_rx_ring; id++) {
		if (dp->rx_ring[id]->out_enq_port_id !=
		    entry_ptr->out_enq_port_id[id]) {
			dev_err(cqm_ctrl->dev,
				"%s invalid enq port in rx_ring[%d]: %u\n",
				__func__, id, dp->rx_ring[id]->out_enq_port_id);
			return CBM_FAILURE;
		}
	}
	for (id = 0; id < entry_ptr->num_tx_ring; id++) {
		if (dp->tx_ring[id]->tx_deq_port !=
		    entry_ptr->tx_deq_port_id[id]) {
			dev_err(cqm_ctrl->dev,
				"%s invalid deq port in tx_ring[%d]: %u\n",
				__func__, id, dp->tx_ring[id]->tx_deq_port);
			return CBM_FAILURE;
		}
	}

	for (id = 0; id < entry_ptr->num_rx_ring; id++) {
		eqp_id = entry_ptr->out_enq_port_id[id];
		eqp_info = &cqm_ctrl->eqm_port_info[eqp_id];
		if (eqp_info->ref_cnt > 0)
			eqp_info->ref_cnt--;

		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
			  "%s eqp_id:%d, eqp_info->ref_cnt:%d\n",
			  __func__, eqp_id, eqp_info->ref_cnt);
		if (eqp_info->ref_cnt == 0 &&
		    ((cqm_ctrl->wav.ecnt == 1 || id) || !sh_f)) {
			cqm_free_aca_port(eqp_id, CBM_PORT_INVALID);
			start = eqp_info->policy_res.ingress_policy;
			end = start + eqp_info->policy_res.ingress_cnt - 1;

			if (is_system_pool(get_pool_from_policy(start)) &&
					   (!(sh_f && id))) {
				for (i = start; i <= end; i++) {
					info = &cqm_ctrl->cqm_bm_policy_dts[i];
					info->busy = 0;
				}
			}

			eqp_info->allocated = P_FREE;
			cqm_dc_enqport_disable(eqp_id);
		}
	}

	if (sh_f) {
		if (cqm_ctrl->wav.dcnt)
			cqm_ctrl->wav.dcnt--;
	}

	CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
		  "%s wav.dqpd: %u wav.eqpd: %u dcnt: %u ecnt: %u\n",
		  __func__, cqm_ctrl->wav.dqpd, cqm_ctrl->wav.eqpd,
		  cqm_ctrl->wav.dcnt, cqm_ctrl->wav.ecnt);

	/* First free buffers for deq_port in tx_ring[0]. This port is different
	 * from deq_ports in other tx_ring because the port is allocated in
	 * port_alloc API. It should only be deallocated in port_dealloc API.
	 */
	deq_port = entry_ptr->tx_deq_port_id[0];
	if (find_dqm_port_type(deq_port) != DQM_ACA_TYPE) {
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
			  "%s tx_ring[0] deq_port invalid: %d\n",
			  __func__, deq_port);
		return CBM_SUCCESS;
	}
	dqp_info = &cqm_ctrl->dqm_port_info[deq_port];
	if (!dqp_info) {
		dev_err(cqm_ctrl->dev, "%s Unable to find deq port for id %d\n",
			__func__, deq_port);
		return CBM_FAILURE;
	}

	/* In wav700 mac recovery, DP_F_SHARE_RES flag is set and
	 * cbm_dp_port_dealloc is not called. In this case, cqm_release_enqport
	 * need to be called to free pool buffers.
	 */
	if (dqp_info->ref_cnt == 0 &&
	    (!sh_f || (cqm_ctrl->wav.dcnt == 0 && sh_f))) {
		cqm_free_aca_port(CBM_PORT_INVALID, deq_port);
		for (id = 0; id < entry_ptr->num_rx_ring; id++) {
			eqp_id = entry_ptr->out_enq_port_id[id];
			eqp_info = &cqm_ctrl->eqm_port_info[eqp_id];
			if (eqp_info->ref_cnt == 0) {
				if (!cqm_release_enqport(eqp_id, deq_port))
					return CBM_FAILURE;
			}
		}
		start = dqp_info->policy_res.egress_policy;
		end = start + dqp_info->policy_res.egress_cnt - 1;
		dqp_info->policy_res.egress_policy = 0;
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
			  "%s egress start: %d end:%d\n",
			  __func__, start, end);
		if (is_system_pool(get_pool_from_policy(start))) {
			for (i = start; i <= end; i++) {
				info = &cqm_ctrl->cqm_bm_policy_dts[i];
				info->busy = 0;
				}
		}

		if (find_dqm_port_type(deq_port) == DQM_ACA_TYPE)
			cqm_dc_deqport_disable(deq_port);
	}

	/* Now free additional deq_ports for tx_rings with index >= 1. */
	if (sh_f) {
		for (id = 1; id < entry_ptr->num_tx_ring; id++) {
			deq_port = entry_ptr->tx_deq_port_id[id];
			dqp_info = &cqm_ctrl->dqm_port_info[deq_port];
			if (dqp_info->ref_cnt > 0)
				dqp_info->ref_cnt--;

			CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
				  "%s deq_port %d ,dqp_ref_cnt:%d sh_f:%d\n",
				  __func__, deq_port, dqp_info->ref_cnt, sh_f);
			/* Deallocate buffer for additional deq ports
			 * to avoid buffer leak.
			 */
			if (!dqp_info->ref_cnt) {
				cqm_free_aca_port(CBM_PORT_INVALID, deq_port);
				for (id = 0; id < entry_ptr->num_rx_ring; id++) {
					eqp_id = entry_ptr->out_enq_port_id[id];
					eqp_info = &cqm_ctrl->eqm_port_info[eqp_id];
					if (eqp_info->ref_cnt == 0) {
						if (!cqm_release_enqport(eqp_id, deq_port))
							return CBM_FAILURE;
					}
				}
				cqm_dc_deqport_disable(deq_port);
				dqp_info->allocated = P_FREE;
			}
		}
	}

	if (sh_f) {
		if (cqm_ctrl->wav.ecnt)
			cqm_ctrl->wav.ecnt--;
		if (!cqm_ctrl->wav.ecnt) {
			CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
				  "%s wav ecnt is zero. Release eq ports\n",
				  __func__);
			cqm_ctrl->wav.eqpd = 0;
		}
	}

	if (pci_flag) {
		if (program_pcidata(dp, port_id, 0, entry_ptr->alloc_flags))
			return CBM_FAILURE;
	}

	cqm_dump_pmac_mapping_list();
	return CBM_SUCCESS;
}

static s32
NO_OPTIMIZE dp_port_alloc_complete(struct module *owner, struct net_device *dev,
		       u32 dev_port, s32 dp_port,
		       struct cbm_dp_alloc_complete_data *dp, u32 flags)
{
	struct cqm_dqm_port_info *dqp_info;
	struct cqm_eqm_port_info *eqp_info;
	struct cqm_eqm_port_info *pr_eqp_info;
	u32 port_type;
	u32 port_group = DQM_DMA_TYPE;
	u32 dqpid = CBM_PORT_INVALID;
	u32 eqpid = CBM_PORT_INVALID;
	u32 reg, alloc_buff = 0;
	u32 ret = CBM_SUCCESS;
	enum CQM_SUP_DEVICE type;
	u32 dma_init_type = 0;
	enum umt_rx_msg_mode msg_mode;
	enum umt_rx_src umt_src;
	u8 i = 0, umt_p, sh_f;
	s16 umt_dc_idx;
	struct cqm_pmac_port_map *local_entry = NULL;
	u32 enq_bpress;
	u32 deq_port = 0;
	u32 alloc_flags;
	u32 dma_port_num;

	if (flags & DP_F_DEREGISTER)
		return dp_port_dealloc_complete(dp, dp_port, 1, flags);

	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s owner: %s dev: %s dev_port: %u dp_port: %d flags: 0x%x\n",
		  __func__, owner->name, dev->name, dev_port, dp_port, flags);
	cqm_dump_dp_alloc_complete_data(__func__, dp);

	if (dp->data->num_umt_port > ARRAY_SIZE(dp->data->umt) ||
	    dp->num_tx_ring > ARRAY_SIZE(dp->tx_ring) ||
	    dp->num_rx_ring > ARRAY_SIZE(dp->rx_ring)) {
		dev_err(cqm_ctrl->dev, "Param invalid: umt: %u tx: %u rx: %u\n",
			dp->data->num_umt_port, dp->num_tx_ring,
			dp->num_rx_ring);
		return DP_FAILURE;
	}

	/* DPM will depricate deq_port in struct cbm_dp_alloc_complete_data.
	 * CQM should get the port using dp_port.
	 */
	local_entry = cqm_get_pmac_mapping(dp_port);
	if (!local_entry) {
		dev_err(cqm_ctrl->dev, "%s invalid dp_port\n", __func__);
		return CBM_FAILURE;
	}
	/*retrieve data from CQM DB*/
	alloc_flags = local_entry->alloc_flags;
	sh_f = (alloc_flags & DP_F_SHARE_RES) ? 1 : 0;
	deq_port = cqm_dp_to_deqp_from_pmac_mapping_list(dp_port);
	if (deq_port == INVALID_DQP) {
		dev_err(cqm_ctrl->dev, "Unable to find deq port for dp_port %d\n",
			dp_port);
		return DP_FAILURE;
	}
	port_type = find_dqm_port_type(deq_port);
	type = get_type_from_alloc_flags(alloc_flags);

	CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
		  "%s deq_port %d dp_port %d, rxring:%d, txring:%d alloc_flags: 0x%x\n",
		  __func__, deq_port, dp_port,
		  dp->num_rx_ring, dp->num_tx_ring, alloc_flags);

	CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
		  "%s wav.dqpd: %u wav.eqpd: %u dcnt: %u ecnt: %u\n",
		  __func__, cqm_ctrl->wav.dqpd, cqm_ctrl->wav.eqpd,
		  cqm_ctrl->wav.dcnt, cqm_ctrl->wav.ecnt);

	if (alloc_flags & DP_F_EPON) {
		reg = (EPON_EPON_MODE_REG_EPONCHKEN_MASK |
			EPON_EPON_MODE_REG_EPONPKTSIZADJ_MASK |
			(deq_port << EPON_EPON_MODE_REG_EPONBASEPORT_POS) |
			((dp->qid_base + (dp->num_qid - 1)) <<
			EPON_EPON_MODE_REG_ENDQ_POS)
			| dp->qid_base);
		cbm_w32(cqm_ctrl->enq + EPON_EPON_MODE_REG, reg);
	}

	if (sh_f && !cqm_ctrl->wav.num) {
		dev_err(cqm_ctrl->dev, "invalid request, wav num:%d",
			cqm_ctrl->wav.num);
		return DP_FAILURE;
	}

	/* Set wib_tx_phy_addr for DOCSIS */
	if (type == DOCSIS) {
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
			  "%s DOCSIS wib_tx_phy_addr: 0x%llx\n",
			  __func__, dp->data->wib_tx_phy_addr);
		cbm_w32(cqm_ctrl->pib + DC_MODE_DESC_WR_BASE,
			dp->data->wib_tx_phy_addr);
	}

	switch (port_type) {
	case DQM_DMA_TYPE:
	case DQM_PON_TYPE:
	case DQM_VUNI_TYPE:
	case DQM_CPU_TYPE:
		ret = get_policy_base_and_num(type, EGRESS, dp_port,
					      alloc_flags,
					      &dp->tx_policy_base,
					      &dp->tx_policy_num);
		if (ret) {
			dev_info(cqm_ctrl->dev, "%s: fail to get policy for sub_dev:%d\n",
				 __func__, type);
			return CBM_FAILURE;
		}

		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
			  "%s dp_port %d alloc_flags: %d tx_policy: %d %d\n",
			  __func__, dp_port, alloc_flags,
			  dp->tx_policy_base, dp->tx_policy_num);
		return CBM_SUCCESS;
	case DQM_ACA_TYPE:
		/* port_type = DEQ_FSQM_DESC; */
		break;
	default:
		break;
	};

	if (!owner || dp->num_rx_ring == 0 || dp->num_tx_ring == 0 ||
	    dp->num_rx_ring != dp->num_tx_ring || dp->num_tx_ring >
	    DP_TX_RING_NUM || dp->num_rx_ring > DP_RX_RING_NUM) {
		dev_err(cqm_ctrl->dev, "%s error: num_tx_ring:%d num_rx_ring:%d\n",
			__func__, dp->num_tx_ring, dp->num_rx_ring);
		return CBM_FAILURE;
	}

	if (sh_f) {
		if (cqm_ctrl->wav.dqpd) {
			cqm_ctrl->wav.dcnt++;
			CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
				  "%s cqm_ctrl->wav.dqpd %d\n",
				  __func__, cqm_ctrl->wav.dqpd);
		}
	}

	/*!< for dp_port_alloc_complete rx rings */
	local_entry->num_rx_ring = dp->num_rx_ring;

	/* For docsis, dqpid is the deq port for DMA port_type DEQ_DC_DOCSIS.
	 * For wav, dqpid is the deq port for DMA port_type DEQ_FSQM_DESC.
	 */
	if (type == DOCSIS) {
		if (get_cqmport(&dqpid, port_group, DEQ_DC_DOCSIS,
				EQ_DQ_PORT_IGNORE_ALLOC) < 0) {
			dev_err(cqm_ctrl->dev, "out of deq ports\n");
			return CBM_NOTFOUND;
		}
	} else if (!sh_f || !cqm_ctrl->wav.dqpd) {
		if (get_cqmport(&dqpid, port_group, DEQ_FSQM_DESC,
				EQ_DQ_PORT_IGNORE_ALLOC) < 0) {
			dev_err(cqm_ctrl->dev, "out of deq ports\n");
			return CBM_NOTFOUND;
		}
		cqm_ctrl->wav.fsqm_port = dqpid;

	} else {
		dqpid = cqm_ctrl->wav.fsqm_port;
	}
	dqp_info = &cqm_ctrl->dqm_port_info[dqpid];

	for (i = 0; i < dp->num_rx_ring; i++) {
		if (i > 0 || /* 2nd rx_ring or later */
		    (!cqm_ctrl->wav.eqpd && (alloc_flags & FLAG_WLAN)) ||
		    !sh_f) {
			if (get_free_enqport(&eqpid, alloc_flags) == CBM_NOTFOUND) {
				dev_err(cqm_ctrl->dev, "out of enq ports\n");
				return CBM_NOTFOUND;
			}
			CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
				  "%s get new enq port %u\n",
				  __func__, eqpid);
			eqp_info = &cqm_ctrl->eqm_port_info[eqpid];
			if (!sh_f ||
			    (!cqm_ctrl->wav.eqpd && (alloc_flags & FLAG_WLAN))) {
				if (get_pool_policy(INGRESS,
						    &eqp_info->policy_res,
						    type,
						    dp->bm_policy_res_id) < 0) {
					return CBM_FAILURE;
				}
				CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
					  "%s new resource. in: %d eg: %d\n",
					  __func__,
					  eqp_info->policy_res.ingress_policy,
					  eqp_info->policy_res.egress_policy);
				if (!cqm_ctrl->wav.eqpd &&
				    (alloc_flags & FLAG_WLAN)) {
					cqm_ctrl->wav.eqpd = eqpid;
					cqm_ctrl->wav.ecnt = 1;
					CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
						  "%s cqm_ctrl->wav.eqpd = %d\n",
						  __func__, cqm_ctrl->wav.eqpd);
				}
			} else if (sh_f) {
				pr_eqp_info =
				&cqm_ctrl->eqm_port_info[cqm_ctrl->wav.eqpd];
				CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
					  "%s cqm_ctrl->wav.eqpd = %d\n",
					  __func__, cqm_ctrl->wav.eqpd);
				memcpy(&eqp_info->policy_res,
				       &pr_eqp_info->policy_res,
				       sizeof(struct cqm_policy_table));
			}
			enq_bpress = cqmport_enq_bpress(dp->rx_ring[i]->bpress_out,
							dp->rx_ring[i]->bpress_alloc);
			do_cqmport_config(eqpid, CBM_PORT_INVALID,
					  &eqp_info->policy_res,
					  CQM_DEFAULT_DC_ENQ_PORT_CFG,
					  CQM_DEFAULT_DC_DEQ_PORT_CFG,
					  dp->rx_ring[i]->out_qos_mode,
					  enq_bpress, 0, alloc_flags);
		} else if (sh_f) {
			eqpid = cqm_ctrl->wav.eqpd;
			cqm_ctrl->wav.ecnt++;
		}
		eqp_info = &cqm_ctrl->eqm_port_info[eqpid];
		local_entry->out_cqm_deq_port_id[i] = dqpid;
		local_entry->out_enq_port_id[i] = eqpid;
		dma_init_type = dqp_info[dqpid].dma_dt_init_type;
		/*Add a check for buffer leak in Rx policy*/
		get_policy_stats(eqp_info->policy_res.ingress_policy, CQM_RX,
				 &alloc_buff);
		/*buffer num is not zero due to prefetched buffers*/
		if (alloc_buff) {
			dev_info(cqm_ctrl->dev, "buffer alloc for Rx policy %d, num: %d\n",
				 eqp_info->policy_res.ingress_policy,
				 alloc_buff);
		}
		dev_info(cqm_ctrl->dev, "EQPC 0x%x NBPC 0x%x\n",
			 cbm_r32(cqm_ctrl->enq + EQ_DC_PORT(eqpid, eqpc)),
			 cbm_r32(cqm_ctrl->enq + EQ_DC_PORT(eqpid, npbpc)));

		dp->rx_ring[i]->out_cqm_deq_port_id = dqpid;
		dp->rx_ring[i]->out_enq_port_id = eqpid;
		eqp_info = &cqm_ctrl->eqm_port_info[eqpid];
		eqp_info->ref_cnt++;
		fill_dp_alloc_complete_rx(dp, &eqp_info->policy_res, i);
		if (type == DOCSIS) {
			dma_port_num = cqm_ctrl->dqm_pid_cfg.docsis_dc_dma_end -
				       cqm_ctrl->dqm_pid_cfg.docsis_dc_dma_start +
				       1;
			dp->rx_ring[i]->num_out_cqm_deq_port = dma_port_num;
		} else if (dp->rx_ring[i]->num_out_tx_dma_ch == DEQ_DMA_CHNL) {
			dp->rx_ring[i]->num_out_cqm_deq_port = 1;
		}

		cqm_ctrl->eqm_port_info[eqpid].allocated = P_ALLOCATED;
		cqm_ctrl->dqm_port_info[dqpid].allocated = P_ALLOCATED;
	}

	/*!< for dp_port_alloc_complete tx rings */
	local_entry->num_tx_ring = dp->num_tx_ring;
	for (i = 0; i < dp->num_tx_ring; i++) {
		if (fill_dp_alloc_complete_tx(dp, i, dp_port,
					      alloc_flags, local_entry)) {
			ret = CBM_FAILURE;
			break;
		}
	}
	for (umt_p = 0; umt_p < dp->data->num_umt_port; umt_p++) {
		msg_mode = dp->data->umt[umt_p].ctl.rx_msg_mode;
		umt_src = dp->data->umt[umt_p].res.rx_src;
		umt_dc_idx = dp->data->umt_dc[umt_p].dc_idx;
		if (umt_dc_idx < 0 ||
		    umt_dc_idx >= ARRAY_SIZE(dp->data->rx_ring)) {
			dev_err(cqm_ctrl->dev, "umt_dc_idx out of bound: %d\n",
				umt_dc_idx);
			continue;
		}

		do_umt_mux_config(dp->data->rx_ring[umt_dc_idx].out_enq_port_id,
				  msg_mode, umt_src);
	}
	if (!ret) {
		if (program_pcidata(dp, dp_port, 1, alloc_flags))
			return CBM_FAILURE;
	} else {
		dp_port_dealloc_complete(dp, dp_port, 0, alloc_flags);
			return CBM_FAILURE;
	}

	cqm_dump_dp_alloc_complete_data(__func__, dp);
	cqm_dump_pmac_mapping_list();
	return CBM_SUCCESS;
}

static void
cqm_vm_pool_check_enable(const u32 eqpid, const u16 pool, const u16 policy)
{
	u32 vm_start_id = cqm_ctrl->eqm_pid_cfg.vm_start;
	u32 offset = CBM_VM_POOL_CHK_REG_0 + ((eqpid - vm_start_id) * 0x4);

	u32 config = CBM_VM_POOL_CHK_REG_0_VMMODEN_MASK |
		     pool << CBM_VM_POOL_CHK_REG_0_POOLID_POS |
		     policy;

	cbm_w32(cqm_ctrl->cqm + offset, config);
}

static s32
NO_OPTIMIZE update_policy_details(enum DIRECTION dir,
				  struct cqm_policy_table *p,
				  struct dp_spl_policy *dp,
				  enum CQM_SUP_DEVICE type, u32 res_id)
{
	if (get_pool_policy(dir, p, type, res_id) < 0)
		return CBM_FAILURE;

	dp->rx_policy_num = 1;
	dp->rx_policy_base = p->ingress_policy;
	dp->tx_policy_num = 1;
	dp->tx_policy_base = p->ingress_policy;
	dp->pool_id[0] = p->pool;
	/* get buffer size from global pool table */
	dp->rx_pkt_size = bm_pool_conf[p->pool].buf_frm_size;
	dp->num_pkt = (dp->num_pkt < p->min_guaranteed) ?
				p->min_guaranteed : dp->num_pkt;
	return CBM_SUCCESS;
}

static void
NO_OPTIMIZE fill_dp_spl_egp(struct dp_spl_egp *egp, u32 id,
			    enum DP_EGP_TYPE egp_type)
{
	const struct cqm_dqm_port_info *info = &cqm_ctrl->dqm_port_info[id];

	egp->type = egp_type;
	egp->egp_id = id;
	egp->pp_ring_size = 1;
	egp->egp_paddr = info->deq_info.cbm_dq_port_base;
	egp->egp_ring_size = info->deq_info.num_desc;
	egp->tx_pkt_credit = info->dq_txpush_num;
	egp->tx_push_paddr = cqm_ctrl->txpush_phys + (id * 0x100);
	egp->tx_push_paddr_qos = (void *)
		((unsigned long)(egp->tx_push_paddr) | BIT(28));
}

static s32 NO_OPTIMIZE cqm_voice_pool_setup(void)
{
	u8 pool = get_matching_pool(VOICE, INGRESS);
	struct pp_bmgr_pool_params pp_params;
	struct cqm_bm_pool_config *info;

	if (pool == INVALID_POOL_ID) {
		dev_err(cqm_ctrl->dev, "No pool found for voice\n");
		return CBM_FAILURE;
	}
	info = &bm_pool_conf[pool];
	pp_params.base_addr_low = info->pool_start_low & GENMASK(31, 0);
	pp_params.base_addr_high = info->pool_start_low >> 32;
	pp_params.num_buffers = info->buf_frm_num;
	pp_params.size_of_buffer = info->buf_frm_size;
	pp_params.flags = POOL_ENABLE_FOR_MIN_GRNT_POLICY_CALC |
			  POOL_ISOLATED;
	return pp_bmgr_pool_configure(&pp_params, &pool);
}

static s32 NO_OPTIMIZE cqm_voice_reg_interface(struct dp_spl_cfg *conn)
{
	struct cqm_dqm_port_info *dqp_info = NULL;
	struct cqm_eqm_port_info *eqp_info = NULL;
	struct cqm_policy_table policy;
	s32 dma_controller = DMAMAX;
	u8 dma_ctrl[DMA_CH_STR_LEN];
	u32 enqport = 0;
	u32 deqport = 0;
	u32 dqmdmaq;
	u32 config;
	u8 i;

	if (cqm_voice_pool_setup())
		return CBM_FAILURE;

	//todo: add the policy config clear as part of dereg
	if (update_policy_details(INGRESS, &policy, &conn->policy[0],
				  VOICE, 0) < 0)
		return CBM_FAILURE;

	for (i = 0;  i < conn->num_egp; i++) {
		switch (i) {
		case 0:
			if (get_cqmport(&deqport, DQM_CPU_TYPE, DEQ_VOICE,
					DQ_PORT) != CBM_SUCCESS) {
				dev_err(cqm_ctrl->dev,
					"spl: voice-dqp in use!\n");
				return CBM_NOTFOUND;
			}
			dqp_info = &cqm_ctrl->dqm_port_info[deqport];
			fill_dp_spl_egp(&conn->egp[i], deqport, DP_EGP_TO_DEV);
			conn->egp[i].free_paddr = cqm_ctrl->deq_phys +
				DQ_CPU_PORT(deqport, ptr_rtn_dw2d0);
			conn->egp[i].free_vaddr = cqm_ctrl->deq +
				DQ_CPU_PORT(deqport, ptr_rtn_dw2d0);
			init_cqm_deq_cpu_port(deqport,
					      dqp_info->deq_info.num_desc);
			config = cbm_r32(cqm_ctrl->deq +
					 DQ_CPU_PORT(deqport, cfg));
			config |= (conn->policy[0].rx_policy_base <<
				   CFG_CPU_EGP_17_POLICY_POS) &
				   CFG_CPU_EGP_17_POLICY_MASK;
			cbm_w32(cqm_ctrl->deq +
				DQ_CPU_PORT(deqport, cfg), config);
			dqp_info->allocated = P_ALLOCATED;
			break;
		case 1:
			if (get_cqmport(&deqport, DQM_DMA_TYPE, DEQ_CPU_QOSBP,
					DQ_PORT) != CBM_SUCCESS) {
				dev_err(cqm_ctrl->dev,
					"spl: inter-voice dqp in use!\n");
				return CBM_NOTFOUND;
			}
			dqp_info = &cqm_ctrl->dqm_port_info[deqport];
			fill_dp_spl_egp(&conn->egp[i], deqport,
					DP_EGP_TO_GSWIP);
			dqp_info->allocated = P_ALLOCATED;
			/* avoid re-entrant if enabled already */
			if (cbm_r32(cqm_ctrl->deq + DQ_DMA_PORT(deqport, cfg)))
				break;
			init_qos_bypass_dma_deq(deqport);
			cqm_dq_dma_chan_init(deqport,
					     dqp_info->dma_dt_init_type);
			dqp_info->dma_ch_in_use = 1;
			dqp_info->allocated = P_ALLOCATED;
			break;
		default:
			dev_err(cqm_ctrl->dev, "spl: more than 2 rings!\n");
			break;
		};
	}

	for (i = 0; i < conn->num_igp; i++) {
		if (get_cqmport(&enqport, EQM_CPU_TYPE, EQM_VOICE,
				EQ_PORT) != CBM_SUCCESS) {
			dev_err(cqm_ctrl->dev, "spl: voice-eqp in use!\n");
			return CBM_NOTFOUND;
		}
		/* only when using, vm port for voice handling */
		if (find_eqm_port_type(enqport) == EQM_VM_TYPE) {
			cqm_vm_pool_check_enable
			(enqport, conn->policy[0].pool_id[0],
			 conn->policy[0].rx_policy_base);
		}
		conn->igp[i].igp_id = enqport;
		eqp_info = &cqm_ctrl->eqm_port_info[enqport];
		conn->igp[i].igp_ring_size = eqp_info->eq_info.num_desc;
		conn->igp[i].igp_paddr =
			cqm_ctrl->enq_phys + EQ_CPU_PORT(enqport, desc0);
		conn->igp[i].alloc_paddr =
			cqm_ctrl->enq_phys + EQ_CPU_PORT(enqport, new_s2ptr);
		conn->igp[i].alloc_vaddr =
			cqm_ctrl->enq + EQ_CPU_PORT(enqport, new_s2ptr);
		/* fill dma settings for intermediate deq port */
		conn->igp[i].num_out_tx_dma_ch = 1;
		conn->igp[i].num_out_cqm_deq_port = 1;
		dqp_info = &cqm_ctrl->dqm_port_info[conn->egp[i + 1].egp_id];
		snprintf(dma_ctrl, DMA_CH_STR_LEN, "DMA%dTX",
			 dqp_info->dma_dt_ctrl);
		dma_controller = dp_dma_get_controller(dma_ctrl);
		conn->igp[i].igp_dma_ch_to_gswip =
			dp_dma_set_id(dma_controller, 0, dqp_info->dma_dt_ch);
		dev_dbg(cqm_ctrl->dev, "%s , dma_ch = %d \r\n",
			__func__, conn->igp[i].igp_dma_ch_to_gswip);
		conn->igp[i].egp = &conn->egp[i + 1];
		conn->igp[i].out_qos_mode = DP_RXOUT_BYPASS_QOS_FSQM;
		/* required when cpu_enq through qos-by-pass path */
		dqmdmaq = ((conn->egp[1].egp_id -
			   cqm_ctrl->dqm_pid_cfg.qosbp_start) <<
		    CFG_CPU_IGP_0_DQMDMAQ_POS) & CFG_CPU_IGP_0_DQMDMAQ_MASK;
		config = CFG_CPU_IGP_0_EQREQ_MASK |
			 CFG_CPU_IGP_0_BRBPEN_MASK |
			 CFG_CPU_IGP_0_EQPCEN_MASK |
			 CFG_CPU_IGP_0_BP_EN_MASK |
			 CFG_CPU_IGP_0_QOSBYPSEN_MASK |
			 CFG_CPU_IGP_0_BUFREQ3_MASK |
			 dqmdmaq;
		if (cqm_ctrl->v2_flag)
			config |= CFG_CPU_IGP_0_PSBKEEP_MASK;
		init_cqm_enq_cpu_port(enqport, config);
		eqp_info->allocated = P_ALLOCATED;
	}
	return CBM_SUCCESS;
}

static s32 NO_OPTIMIZE cqm_vpna_reg_interface(struct dp_spl_cfg *conn)
{
	struct cqm_policy_table policy = {0};
	struct cqm_eqm_port_info *eqp_info;
	struct cqm_dqm_port_info *dqp_info;
	s32 dma_controller = DMAMAX;
	u8 dma_ctrl[DMA_CH_STR_LEN];
	u32 def_num_desc = 0;
	u32 req_num_desc = 0;
	u32 qos_mode = 0;
	struct umt_trig umt_ctrl;
	u32 enqport = 0;
	u32 deqport = 0;
	u32 config = 0;
	u8 i;
	u32 enq_bpress, deq_bpress;

	if (!conn->num_egp || !conn->num_igp)
		return CBM_FAILURE;

	for (i = 0; i < conn->num_egp; i++) {
		switch (i) {
		case 0:
			if (get_cqmport(&deqport, DQM_ACA_TYPE, DEQ_ACA,
					DQ_PORT) != CBM_SUCCESS) {
				dev_err(cqm_ctrl->dev, "spl:deq-vpn in use!\n");
				return CBM_NOTFOUND;
			}
			dqp_info = &cqm_ctrl->dqm_port_info[deqport];
			fill_dp_spl_egp(&conn->egp[i], deqport, DP_EGP_TO_DEV);
			conn->egp[i].free_paddr = dqp_info->cbm_buf_free_base;
			deq_bpress = cqmport_deq_bpress(DP_BPRESS_DIS,
							DP_BPRESS_NA);
			do_cqmport_config(CBM_PORT_INVALID, deqport, &policy,
					  CQM_DEFAULT_DC_ENQ_PORT_CFG,
					  CQM_DEFAULT_DC_DEQ_PORT_CFG,
					  DP_RXOUT_QOS, 0, deq_bpress, 0);
			umt_ctrl.port_en = 1;
			umt_ctrl.mark_en = 0;
			deq_umt_trigger(&deqport, &umt_ctrl);
			dqp_info->allocated = P_ALLOCATED;
		break;
		case 1:
			if (get_cqmport(&deqport, DQM_DMA_TYPE, DEQ_VPN,
					DQ_PORT) != CBM_SUCCESS) {
				dev_err(cqm_ctrl->dev, "inter-dq-vpn in use!\n");
				return CBM_NOTFOUND;
			}
			dqp_info = &cqm_ctrl->dqm_port_info[deqport];
			fill_dp_spl_egp(&conn->egp[i], deqport,
					DP_EGP_TO_GSWIP);
			snprintf(dma_ctrl, DMA_CH_STR_LEN, "DMA%dTX",
				 dqp_info->dma_dt_ctrl);
			dma_controller = dp_dma_get_controller(dma_ctrl);
			handle_dma_chnl_init(deqport, CBM_PORT_F_DEQUEUE_PORT);
			dqp_info->allocated = P_ALLOCATED;
			break;
		break;
		default:
			dev_err(cqm_ctrl->dev, "spl vpn: more than 2 rings!\n");
			return CBM_NOTFOUND;
		};
	}

	for (i = 0; i < conn->num_igp; i++) {
		qos_mode =  DP_RXOUT_QOS;

		if (get_cqmport(&enqport, EQM_CPU_TYPE, EQM_ACA,
				EQ_PORT) != CBM_SUCCESS) {
			dev_err(cqm_ctrl->dev, "spl: enq-vpn in use!\n");
			return CBM_NOTFOUND;
		}

		/* ingress deq port is not required for vpn adaptor */
		conn->igp[i].num_out_cqm_deq_port = 0;
		eqp_info = &cqm_ctrl->eqm_port_info[enqport];
		conn->igp[i].igp_id = enqport;
		conn->igp[i].out_qos_mode = qos_mode;
		req_num_desc = conn->igp[i].igp_ring_size;
		def_num_desc = eqp_info->eq_info.num_desc;

		/* tune the num of descriptor fifo */
		config = (req_num_desc && req_num_desc < def_num_desc) ?
			  req_num_desc : def_num_desc;
		cbm_w32(cqm_ctrl->enq + EQ_DC_PORT(enqport, dptr), config - 1);

		conn->igp[i].igp_paddr =
			cqm_ctrl->dmadesc_64_phys + CQM_EQM_DC_DESC(enqport, 0);
		conn->igp[i].igp_ring_size = config;
		conn->igp[i].egp = &conn->egp[1]; /* intermediate dequeue */
		conn->igp[i].igp_dma_ch_to_gswip =
			dp_dma_set_id(dma_controller, 0, dqp_info->dma_dt_ch);

		enq_bpress = cqmport_enq_bpress(DP_BPRESS_DIS, DP_BPRESS_NA);
		do_cqmport_config(enqport, CBM_PORT_INVALID, &policy,
				  CQM_DEFAULT_DC_ENQ_PORT_CFG,
				  CQM_DEFAULT_DC_DEQ_PORT_CFG,
				  qos_mode, enq_bpress, 0, 0);
		do_umt_mux_config(enqport, (qos_mode == DP_RXOUT_QOS) ? 0 : 1, UMT_RX_SRC_CQEM);
		/* remove buffer alloc facility */
		config = cbm_r32(cqm_ctrl->enq + EQ_DC_PORT(enqport, cfg))
				 & ~CFG_DC_IGP_16_BUFREQ0_MASK;
		cbm_w32(cqm_ctrl->enq + EQ_DC_PORT(enqport, cfg), config);
		eqp_info->allocated = P_ALLOCATED;
	}

	return CBM_SUCCESS;
}

static s32 NO_OPTIMIZE cqm_toe_reg_interface(struct dp_spl_cfg *conn)
{
	struct cqm_dqm_port_info *dqp_info = NULL;
	struct cqm_eqm_port_info *eqp_info = NULL;
	s32 dma_controller = DMAMAX;
	u8 dma_ctrl[DMA_CH_STR_LEN];
	u32 enqport;
	u32 deqport;
	u8 i;

	for (i = 0; i < conn->num_egp; i++) {
		switch (i) {
		case 0:
			if (get_cqmport(&deqport, DQM_CPU_TYPE, DEQ_TOE,
					DQ_PORT) != CBM_SUCCESS) {
				dev_err(cqm_ctrl->dev, "spl: dq-toe in use!\n");
				return CBM_NOTFOUND;
			}
			dqp_info = &cqm_ctrl->dqm_port_info[deqport];
			fill_dp_spl_egp(&conn->egp[i], deqport, DP_EGP_TO_DEV);
			init_cqm_deq_cpu_port(deqport,
					      dqp_info->deq_info.num_desc);
			dqp_info->allocated = P_ALLOCATED;
			break;
		case 1:
			if (get_cqmport(&deqport, DQM_DMA_TYPE, DEQ_TOE,
					DQ_PORT) != CBM_SUCCESS) {
				dev_err(cqm_ctrl->dev, "spl: inter-dq-toe in use!\n");
				return CBM_NOTFOUND;
			}
			dqp_info = &cqm_ctrl->dqm_port_info[deqport];
			fill_dp_spl_egp(&conn->egp[i], deqport,
					DP_EGP_TO_GSWIP);
			snprintf(dma_ctrl, DMA_CH_STR_LEN, "DMA%dTX",
				 dqp_info->dma_dt_ctrl);
			dma_controller = dp_dma_get_controller(dma_ctrl);
			handle_dma_chnl_init(deqport, CBM_PORT_F_DEQUEUE_PORT);
			dqp_info->allocated = P_ALLOCATED;
			break;
		default:
			dev_err(cqm_ctrl->dev, "spl toe: more than 2 rings!\n");
			break;
		};
	}

	for (i = 0; i < conn->num_igp; i++) {
		if (get_cqmport(&enqport, EQM_DMA_TYPE, (EQM_TSO + i % 2),
				EQ_PORT) != CBM_SUCCESS) {
			dev_err(cqm_ctrl->dev, "spl: toe enq port in use!\n");
			return -1;
		}
		conn->igp[i].igp_id = enqport;
		eqp_info = &cqm_ctrl->eqm_port_info[enqport];
		conn->igp[i].igp_paddr = cqm_ctrl->dmadesc_128_phys +
					 CQM_ENQ_DMA_DESC(enqport, 0);
		conn->igp[i].num_out_cqm_deq_port = 1;
		conn->igp[i].out_qos_mode = DP_RXOUT_QOS;
		if (dqp_info)
			conn->igp[i].igp_dma_ch_to_gswip =
			dp_dma_set_id(dma_controller, 0, dqp_info->dma_dt_ch);
		dev_dbg(cqm_ctrl->dev, "%s, dma_ch = %d \r\n",
			__func__,  conn->igp[i].igp_dma_ch_to_gswip);
		conn->igp[i].num_out_tx_dma_ch = 1;
		conn->igp[i].igp_ring_size = eqp_info->eq_info.num_desc;
		/*Both IGPs share the same intermediate DMA port*/
		conn->igp[i].egp = &conn->egp[1];
		cqm_dma_port_enable(enqport, 0, IOC_2K_POLICY_BUF_SIZE,
				    BUF_TYPE_NIOC_SHARED);
		eqp_info->allocated = P_ALLOCATED;
	}
	return CBM_SUCCESS;
}

static s32 NO_OPTIMIZE cqm_ppnf_reg_interface(struct dp_spl_cfg *conn)
{
	struct cqm_dqm_port_info *dqp_info = NULL;
	s32 dma_controller = DMAMAX;
	u8 dma_ctrl[DMA_CH_STR_LEN];
	u32 deqport;
	u32 size;
	u8 i;

	for (i = 0; i < conn->num_egp; i++) {
		switch (i) {
		case 0:
			if (get_cqmport(&deqport, DQM_PP_NF_TYPE,
					DEQ_PP_NF, DQ_PORT) != CBM_SUCCESS) {
				dev_err(cqm_ctrl->dev,
					"spl: virtual-deq-ppnf in use!\n");
				return CBM_NOTFOUND;
			}
			dqp_info = &cqm_ctrl->dqm_port_info[deqport];
			size = sizeof(struct cbm_desc) *
			       dqp_info->deq_info.num_desc;
			/* The QoS API driver is a generic API and is unaware of
			 * the interface type.So the NULL pointer check can't be
			 * removed from QoS driver. Workaround is to pass -1 as
			 * ring address instead of NULL
			 */
			conn->egp[i].tx_push_paddr = (void *)-1;
			conn->egp[i].tx_push_paddr_qos = (void *)-1;
			conn->egp[i].egp_paddr = (void *)-1;
			conn->egp[i].egp_id = deqport;
			conn->egp[i].pp_ring_size = 1;
			conn->egp[i].type = DP_EGP_TO_DEV;
			dqp_info->allocated = P_ALLOCATED;
			break;
		case 1:
			/*same port gets assigned for all the PPNFs*/
			if (get_cqmport(&deqport, DQM_DMA_TYPE,
					DEQ_PP_NF, DQ_PORT) != CBM_SUCCESS) {
				dev_err(cqm_ctrl->dev,
					"spl: deq-ppnf in use!\n");
				return CBM_NOTFOUND;
			}
			conn->igp[i].num_out_cqm_deq_port = 1;
			conn->igp[i].num_out_tx_dma_ch = 1;
			dqp_info = &cqm_ctrl->dqm_port_info[deqport];
			fill_dp_spl_egp(&conn->egp[i], deqport,
					DP_EGP_TO_GSWIP);
			conn->igp[i].egp = &conn->egp[i];
			conn->igp[i].out_qos_mode = DP_RXOUT_QOS;
			snprintf(dma_ctrl, DMA_CH_STR_LEN, "DMA%dTX",
				 dqp_info->dma_dt_ctrl);
			dma_controller = dp_dma_get_controller(dma_ctrl);
			conn->igp[i].igp_dma_ch_to_gswip =
				dp_dma_set_id(dma_controller,
					      0, dqp_info->dma_dt_ch);
			dev_dbg(cqm_ctrl->dev, "%s dma_ch = %d \r\n",
				__func__, conn->igp[i].igp_dma_ch_to_gswip);
			if (!dqp_info->dma_ch_in_use)
				handle_dma_chnl_init(deqport,
						     CBM_PORT_F_DEQUEUE_PORT);
			dqp_info->allocated = P_SHARED;
			break;
		default:
			dev_err(cqm_ctrl->dev, "spl: more than 2 rings!\n");
			return CBM_FAILURE;
		};
	}
	return CBM_SUCCESS;
}

static s32 NO_OPTIMIZE spl_interface_dereg(const struct dp_spl_cfg *dreg_conn)
{
	struct cqm_dqm_port_info *dqp_info = NULL;
	u32 port_type;
	u32 deqport = 0;
	u8 idx;

	cqm_dump_dp_spl_cfg(__func__, dreg_conn);

	for (idx = 0; idx < dreg_conn->num_egp; idx++) {
		deqport = dreg_conn->egp[idx].egp_id;
		dqp_info = &cqm_ctrl->dqm_port_info[deqport];
		port_type = find_dqm_port_type(deqport);

		switch (port_type) {
		case DQM_CPU_TYPE:
			cbm_w32((cqm_ctrl->deq + DQ_CPU_PORT(deqport, cfg)), 0);
			break;
		case DQM_DMA_TYPE:
			cqm_dequeue_dma_port_uninit(deqport, 0);
			cqm_dma_port_enable(deqport,
					    CBM_PORT_F_DEQUEUE_PORT |
					    CBM_PORT_F_DISABLE,
					    0, BUF_TYPE_INVALID);
			dqp_info->dma_ch_in_use = 0;
			break;
		case DQM_PP_NF_TYPE:
			break;
		default:
			return CBM_FAILURE;
		};
		dqp_info->allocated = P_FREE;
	}

	for (idx = 0; idx < dreg_conn->num_igp; idx++) {
		if (!cqm_release_enqport(dreg_conn->igp[idx].igp_id, deqport))
			return CBM_FAILURE;
	}

	if (dreg_conn->spl_id >= DP_SPL_CONN_START &&
	    dreg_conn->spl_id <= DP_SPL_CONN_END) {
		/* adjust the spl_id to reuse the dp_port_id linked list */
		cqm_delete_from_list(dreg_conn->spl_id + 8, DP_F_SPL_PATH);
	} else {
		dev_err(cqm_ctrl->dev, "spl: invalid spl_id in dreg: %d\n",
			dreg_conn->spl_id);
	}
	return CBM_SUCCESS;
}

static s32 NO_OPTIMIZE cqm_dp_spl_conn(int inst, struct dp_spl_cfg *conn)
{
	struct cqm_pmac_port_map spl_info = {0};
	struct dp_spl_policy *policy_ptr;
#ifdef CONFIG_IOC_POOL
	u8 base_pool = cqm_ctrl->cpu_base_pool;
#endif

	if (conn->flag & DP_F_DEREGISTER) {
		spl_interface_dereg(conn);
		return CBM_SUCCESS;
	}

	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s inst: %d\n", __func__, inst);
	cqm_dump_dp_spl_cfg(__func__, conn);

	/* system policy info*/
	policy_ptr = &conn->policy[0];
	policy_ptr->rx_policy_base = 1;
	policy_ptr->rx_policy_num = 4;
	policy_ptr->tx_policy_base = 1;
	policy_ptr->tx_policy_num = 4;

	conn->num_igp = 0;
	conn->num_egp = 0;
	switch (conn->type) {
	case DP_SPL_TOE:
		/*EQM_LRO port is not used.Enable only EQM_TSO port*/
		conn->num_igp = 1;
		/* same EGP is shared by both IGPs
		 * LRO ACK and TSO use the same intermediate port
		 * num_EGP = 1 TOE DQ port + 1 intermediate DMA port for the
		 * IGPS
		 */
		conn->num_egp = 2;
#ifdef CONFIG_IOC_POOL
		policy_ptr->rx_policy_base = bm_pool_conf[base_pool].policy;
		policy_ptr->rx_policy_num = 4;
		policy_ptr->tx_policy_base = bm_pool_conf[base_pool].policy;
		policy_ptr->tx_policy_num = 4;
#endif
		if (cqm_toe_reg_interface(conn) != CBM_SUCCESS)
			return CBM_FAILURE;
		break;
	case DP_SPL_VOICE:
		conn->num_igp = 1;
		conn->num_egp = 1 + conn->num_igp;
		conn->num_umt_port = 0;
		if (cqm_voice_reg_interface(conn) != CBM_SUCCESS)
			return CBM_FAILURE;
		break;
	case DP_SPL_VPNA:
		conn->num_igp = 1;
		/*1 DC EGP and 1 intermediate DMA EGP*/
		conn->num_egp = 2;
		conn->num_umt_port = 1;
		if (cqm_vpna_reg_interface(conn) != CBM_SUCCESS)
			return CBM_FAILURE;
		break;
	case DP_SPL_APP_LITEPATH:
	case DP_SPL_PP_DUT:
#ifdef CONFIG_IOC_POOL
		policy_ptr->rx_policy_base = bm_pool_conf[base_pool].policy;
		policy_ptr->rx_policy_num = 4;
		policy_ptr->tx_policy_base = bm_pool_conf[base_pool].policy;
		policy_ptr->tx_policy_num = 4;
#endif
		break;
	case DP_SPL_PP_NF:
		conn->num_egp = 2;
		conn->num_umt_port = 0;
		if (cqm_ppnf_reg_interface(conn) != CBM_SUCCESS)
			return CBM_FAILURE;
		break;
	case DP_SPL_MAX:
	case DP_NON_SPL:
	default:
		dev_err(cqm_ctrl->dev, "invalid interface: %x\n", conn->type);
		return CBM_FAILURE;
	};

	if (conn->spl_id >= DP_SPL_CONN_START &&
	    conn->spl_id <= DP_SPL_CONN_END) {
		/* adjust the spl_id to reuse the dp_port_id linked list */
		spl_info.port_id = conn->spl_id + 8;
		spl_info.egp_type = DP_F_SPL_PATH;
		spl_info.flags = P_ALLOCATED;
		cqm_add_to_list(&spl_info);
	} else {
		dev_err(cqm_ctrl->dev, "invalid spl_id in reg: %d\n",
			conn->spl_id);
	}
	return CBM_SUCCESS;
}

static s32 NO_OPTIMIZE dp_enable(struct module *owner, u32 port_id,
				 struct cbm_dp_en_data *data, u32 flags)
{
	struct cqm_policy_table enq_policy_res;
	int type, i;
	struct core_ops *ops;
	GSW_PMAC_Glbl_Cfg_t glbl_cfg;
	static bool l_set_bsl;
	void *deq = cqm_ctrl->deq;
	u32 val, dma_flag = 0;
	struct cqm_pmac_port_map *entry_ptr = NULL;
	struct cqm_dqm_port_info *dqp_info = NULL;
	int port = data->deq_port;
	u32 alloc_flags;

	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s owner: %s port_id: %d flags: 0x%x\n",
		  __func__, owner->name, port_id, flags);

	cqm_dump_dp_en_data(__func__, data);

	entry_ptr = cqm_get_pmac_mapping(port_id);
	if (!entry_ptr || port < 0) {
		dev_err(cqm_ctrl->dev, "Invalid cbm_port:%d or dpid:%d\n",
			port, port_id);
		return CBM_FAILURE;
	}
	/*override the alloc_flags from the value retrieved from CQM DB*/
	alloc_flags = entry_ptr->alloc_flags;
	CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
		  "%s, dpid:%d deq_port:%d flags: 0x%x alloc_flags 0x%x ring:%d\n",
		  __func__, port_id, port, flags, alloc_flags, entry_ptr->num_tx_ring);
	if (alloc_flags & DP_F_SHARE_RES) {
		if (!entry_ptr->num_tx_ring) {
			pr_err("%s:num_tx_ring is invalid %d\n", __func__,
			       entry_ptr->num_tx_ring);
			return CBM_FAILURE;
		}
		for (i = 0; i < entry_ptr->num_tx_ring; i++) {
			port = entry_ptr->tx_deq_port_id[i];
			dqp_info =  &cqm_ctrl->dqm_port_info[port];
			if (flags & CBM_PORT_F_DISABLE) {
				if (dqp_info->ref_cnt > 0)
					dqp_info->ref_cnt--;
			} else {
				dqp_info->ref_cnt++;
			}
		}
	} else {
		dqp_info =  &cqm_ctrl->dqm_port_info[port];
		if (flags & CBM_PORT_F_DISABLE) {
			if (dqp_info->ref_cnt)
				dqp_info->ref_cnt--;
			else
				return CBM_SUCCESS;
		} else {
			if (!dqp_info->ref_cnt)
				dqp_info->ref_cnt++;
			else
				return CBM_SUCCESS;
		}
	}

	if (!dqp_info) {
		dev_err(cqm_ctrl->dev, "%s: dqp_info is null\n", __func__);
		return CBM_FAILURE;
	}

	if (flags & CBM_PORT_F_DISABLE) {
		if (entry_ptr->eqp_en_cnt) {
			entry_ptr->eqp_en_cnt--;
			if (!entry_ptr->eqp_en_cnt)
				dma_flag = 1;
		}
	} else {
		if (!entry_ptr->eqp_en_cnt) {
			entry_ptr->dma_chnl_init = data->dma_chnl_init;
			dma_flag = 1;
		}
		dqp_info->tx_ring_size = data->tx_ring_size;
		entry_ptr->eqp_en_cnt++;
	}

	type = find_dqm_port_type(port);

	switch (type) {
	case DQM_CPU_TYPE:
		if (!(flags & CBM_PORT_F_DISABLE))
			init_cqm_deq_cpu_port(port, dqp_info->tx_ring_size);
	break;
	case DQM_ACA_TYPE:
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
			  "dqpid: %d already enabled\n", port);
	break;
	case DQM_PON_TYPE:
	case DQM_DOCSIS_TYPE:
		if ((alloc_flags & DP_F_DOCSIS) &&
		    dqp_info->wib_credit_pkt != 0)
			cbm_w32((deq + DQ_PON_PORT((port), dptr)),
				dqp_info->wib_credit_pkt);
		if ((alloc_flags & DP_F_DOCSIS) &&
		    data->f_policy == 1) {
			if (get_pool_policy(EGRESS, &dqp_info->policy_res, DOCSIS,
					    data->bm_policy_res_id) < 0)
				return CBM_FAILURE;
			data->tx_policy_num = dqp_info->policy_res.egress_cnt;
			data->tx_policy_base = dqp_info->policy_res.egress_policy;
			/* It's also required to return rx policy_num and base */
			memset(&enq_policy_res, 0, sizeof(enq_policy_res));
			if (get_pool_policy(INGRESS, &enq_policy_res, DOCSIS,
					    data->bm_policy_res_id) < 0)
				return CBM_FAILURE;
			data->rx_policy_num = enq_policy_res.ingress_cnt;
			data->rx_policy_base = enq_policy_res.ingress_policy;
		}
		if ((flags & CBM_PORT_F_DISABLE) &&
		    entry_ptr->dma_chnl_init && dma_flag)
			cqm_deq_pon_port_disable(port, alloc_flags);

		if ((flags & CBM_PORT_F_DISABLE) &&
		    (!entry_ptr->dma_chnl_init || !dma_flag) &&
		    (alloc_flags & (DP_F_EPON | DP_F_GPON))) {
			val = ((port - CQM_PON_IP_PORT_START)
				<< PIB_PON_IP_CMD_PORT_ID_POS) | 0x28000;
			cbm_w32(cqm_ctrl->pib + PIB_PON_IP_CMD, val);
			CQM_DEBUG(CQM_DBG_FLAG_PON_PORT,
				  "%s: write CQEM_PIB_PIB_PON_IP_CMD %lx , val %x\n",
				  __func__,
				  (unsigned long)(cqm_ctrl->pib +
				  PIB_PON_IP_CMD),
				  val);
		}

		/* If PIB_DC_MODE is set, then do not initialize DMA */
		if (cbm_r32(cqm_ctrl->pib + PIB_CTRL) &
		    PIB_CTRL_PIB_DC_MODE_MASK)
			dma_flag = 0;

		if (entry_ptr->dma_chnl_init && !(flags & CBM_PORT_F_DISABLE)) {
			if (dma_flag)
				handle_dma_chnl_init(port, flags);
			val = CFG_PON_EGP_75_DQREQ_MASK |
				CFG_PON_EGP_75_BUFRTN_MASK |
				CFG_PON_EGP_75_BFBPEN_MASK |
				CFG_PON_EGP_75_DQPCEN_MASK;
			val |= ((data->deq_port << CFG_PON_EGP_75_EPMAP_POS) &
				CFG_PON_EGP_75_EPMAP_MASK);
			cbm_w32((deq + DQ_PON_PORT((port), cfg)),
				val);
			pib_port_enable(port, 1);
		} else if (!(flags & CBM_PORT_F_DISABLE)) {
			cqm_dma_port_enable(port, CBM_PORT_F_DEQUEUE_PORT,
					    0, BUF_TYPE_INVALID);
			val = CFG_PON_EGP_75_DQREQ_MASK |
				CFG_PON_EGP_75_BUFRTN_MASK |
				CFG_PON_EGP_75_BFBPEN_MASK |
				CFG_PON_EGP_75_DQPCEN_MASK;
			val |= ((data->deq_port << CFG_PON_EGP_75_EPMAP_POS) &
				CFG_PON_EGP_75_EPMAP_MASK);
			cbm_w32((deq + DQ_PON_PORT((port), cfg)),
				val);
			pib_port_enable(port, 1);
		}
	break;
	case DQM_DMA_TYPE:
		if (flags & CBM_PORT_F_DISABLE)
			flags |= CBM_PORT_F_FLUSH;
		if (entry_ptr->dma_chnl_init && dma_flag)
			handle_dma_chnl_init(port, flags);
	break;
	default:
		dev_err(cqm_ctrl->dev, "Unknown port type %d\n", type);
		return CBM_FAILURE;
	break;
	}
	if (!l_set_bsl) {
		ops = gsw_get_swcore_ops(0);
		for (i = 0; i < 2; i++) {
			glbl_cfg.nPmacId = i;
			if (ops)
				ops->gsw_pmac_ops.Pmac_Gbl_CfgGet(ops,
						&glbl_cfg);
			glbl_cfg.nBslThreshold[0] = BSL_THRESHOLD_0;
			glbl_cfg.nBslThreshold[1] = BSL_THRESHOLD_1;
			glbl_cfg.nBslThreshold[2] = BSL_THRESHOLD_1;
			glbl_cfg.nMaxJumboLen = cqm_ctrl->lgm_pool_size[3];
			if (ops)
				ops->gsw_pmac_ops.Pmac_Gbl_CfgSet(ops,
								  &glbl_cfg);
		}
		l_set_bsl = true;
	}
	return CBM_SUCCESS;
}

int get_fsqm_ofsc(void)
{
	void *fsqm = cqm_ctrl->fsqm;

	return cbm_r32(fsqm + OFSC);
}

static void NO_OPTIMIZE init_fsqm(void)
{
	int i;
	int minlsa, maxlsa;
	u32 fsqm_frm_num;
	void *fsqm = cqm_ctrl->fsqm;

	dev_dbg(cqm_ctrl->dev,
		"%s, fsqm_size =  0x%zx\n", __func__, cqm_ctrl->fsqm_sz);
	if (cqm_ctrl->fsqm_sz == 0)
		return;

	fsqm_frm_num = cqm_ctrl->fsqm_sz / CQM_SRAM_FRM_SIZE;
	dev_dbg(cqm_ctrl->dev, "fsqm frm num %d", fsqm_frm_num);
	for (i = 1; i <= fsqm_frm_num; i++) {
		if (i == fsqm_frm_num - 1) {
			cbm_w32((FSQM_LLT_RAM(fsqm, i - 1)), 0x7FFF);
		} else {
			cbm_w32((FSQM_LLT_RAM(fsqm, i - 1)),
				i % fsqm_frm_num);
		}
#ifdef ENABLE_LL_DEBUG
		cbm_w32((FSQM_RCNT(fsqm, i - 1)), 0);
#else
		cbm_w32((FSQM_RCNT(fsqm, i - 1)), 1);
#endif
	}
	/* minlas is always zero, it's offset counted into the BA */
	minlsa = 0;
	/* The last item is invalid inside the FSQM */
	maxlsa = fsqm_frm_num - 2;
	cbm_w32((fsqm + LSARNG),
		((maxlsa << LSARNG_MAXLSA_POS) & LSARNG_MAXLSA_MASK) |
		((minlsa << LSARNG_MINLSA_POS) & LSARNG_MINLSA_MASK));
	cbm_w32((fsqm + OFSQ),
		((maxlsa << OFSQ_TAIL_POS) & OFSQ_TAIL_MASK) |
		((minlsa << OFSQ_HEAD_POS) & OFSQ_HEAD_MASK));
	cbm_w32((fsqm + OFSC),
		((maxlsa - minlsa + 1) << OFSC_FSC_POS) &
		OFSC_FSC_MASK);

	cbm_w32((fsqm + FSQM_IRNEN), 0x111101F);

	cbm_w32((fsqm + FSQT0), fsqm_frm_num / 5);
	cbm_w32((fsqm + FSQT1), fsqm_frm_num / 8);
	cbm_w32((fsqm + FSQT2), fsqm_frm_num / 12);
	cbm_w32((fsqm + FSQT3), fsqm_frm_num / 25);
	cbm_w32((fsqm + FSQT4), fsqm_frm_num / 25);
	cbm_w32(fsqm + IO_BUF_RD, 0);
	cbm_w32(fsqm + IO_BUF_WR, 0);
	cbm_w32(fsqm + FSQM_CTRL, 0x1);
	/*Dump FSQM registers*/
	print_reg("FSQM_CTRL",	(u32 *)(fsqm + FSQM_CTRL));
	print_reg("IO_BUF_RD",	(u32 *)(fsqm + IO_BUF_RD));
	print_reg("IO_BUF_WR",	(u32 *)(fsqm + IO_BUF_WR));
	print_reg("FSQM_IRNCR", (u32 *)(fsqm + FSQM_IRNCR));
	print_reg("FSQM_IRNEN", (u32 *)(fsqm + FSQM_IRNEN));
	print_reg("FSQM_OFSQ",	(u32 *)(fsqm + OFSQ));
	print_reg("FSQM_OFSC",	(u32 *)(fsqm + OFSC));
	print_reg("FSQM_FSQT0", (u32 *)(fsqm + FSQT0));
	print_reg("FSQM_FSQT1", (u32 *)(fsqm + FSQT1));
	print_reg("FSQM_FSQT2", (u32 *)(fsqm + FSQT2));
	print_reg("FSQM_FSQT3", (u32 *)(fsqm + FSQT3));
	print_reg("FSQM_FSQT4", (u32 *)(fsqm + FSQT4));
	print_reg("FSQM_LSARNG", (u32 *)(fsqm + LSARNG));
	dev_info(cqm_ctrl->dev, "fsqm init successfully, free buffers %d\n",
		 get_fsqm_ofsc());
}

static void NO_OPTIMIZE init_fsqm_desc(void)
{
	int i;
	int minlsa, maxlsa;
	u32 fsqm_frm_num;
	void *fsqm = cqm_ctrl->fsqm_desc;

	fsqm_frm_num = CQM_DESC_MEM_SIZE / CQM_DESC_MEM_FRM_SIZE;
	dev_dbg(cqm_ctrl->dev, "fsqm frm num %d", fsqm_frm_num);
	for (i = 1; i <= fsqm_frm_num; i++) {
		if (i == fsqm_frm_num - 1) {
			cbm_w32((FSQM_LLT_RAM(fsqm, i - 1)), 0x7FFF);
		} else {
			cbm_w32((FSQM_LLT_RAM(fsqm, i - 1)),
				i % fsqm_frm_num);
		}
#ifdef ENABLE_LL_DEBUG
		cbm_w32((FSQM_RCNT(fsqm, i - 1)), 0);
#else
		cbm_w32((FSQM_RCNT(fsqm, i - 1)), 1);
#endif
	}
	/* minlas is always zero, it's offset counted into the BA */
	minlsa = 0;
	/* The last item is invalid inside the FSQM */
	maxlsa = fsqm_frm_num - 2;
	cbm_w32((fsqm + LSARNG),
		((maxlsa << LSARNG_MAXLSA_POS) & LSARNG_MAXLSA_MASK) |
		((minlsa << LSARNG_MINLSA_POS) & LSARNG_MINLSA_MASK));
	cbm_w32((fsqm + OFSQ),
		((maxlsa << OFSQ_TAIL_POS) & OFSQ_TAIL_MASK) |
		((minlsa << OFSQ_HEAD_POS) & OFSQ_HEAD_MASK));
	cbm_w32((fsqm + OFSC),
		((maxlsa - minlsa + 1) << OFSC_FSC_POS) &
		OFSC_FSC_MASK);

	cbm_w32((fsqm + FSQM_IRNEN), 0x111101F);

	cbm_w32((fsqm + FSQT0), fsqm_frm_num / 6 * 5);
	cbm_w32((fsqm + FSQT1), fsqm_frm_num / 6 * 4);
	cbm_w32((fsqm + FSQT2), fsqm_frm_num / 6 * 3);
	cbm_w32((fsqm + FSQT3), fsqm_frm_num / 6 * 2);
	cbm_w32((fsqm + FSQT4), fsqm_frm_num / 6);
	cbm_w32(fsqm + IO_BUF_RD, 0);
	cbm_w32(fsqm + IO_BUF_WR, 0);
	cbm_w32(fsqm + FSQM_CTRL, 0x1);
	/*Dump FSQM registers*/
	print_reg("FSQM_CTRL",	(u32 *)(fsqm + FSQM_CTRL));
	print_reg("IO_BUF_RD",	(u32 *)(fsqm + IO_BUF_RD));
	print_reg("IO_BUF_WR",	(u32 *)(fsqm + IO_BUF_WR));
	print_reg("FSQM_IRNCR", (u32 *)(fsqm + FSQM_IRNCR));
	print_reg("FSQM_IRNEN", (u32 *)(fsqm + FSQM_IRNEN));
	print_reg("FSQM_OFSQ",	(u32 *)(fsqm + OFSQ));
	print_reg("FSQM_OFSC",	(u32 *)(fsqm + OFSC));
	print_reg("FSQM_FSQT0", (u32 *)(fsqm + FSQT0));
	print_reg("FSQM_FSQT1", (u32 *)(fsqm + FSQT1));
	print_reg("FSQM_FSQT2", (u32 *)(fsqm + FSQT2));
	print_reg("FSQM_FSQT3", (u32 *)(fsqm + FSQT3));
	print_reg("FSQM_FSQT4", (u32 *)(fsqm + FSQT4));
	print_reg("FSQM_LSARNG", (u32 *)(fsqm + LSARNG));

	dev_info(cqm_ctrl->dev, "fsqm init successfully, free buffers %d\n",
		 get_fsqm_ofsc());
}

static void ls_intr_handler(u32 pid)
{
	u32 ls_intr_stat;
	void *ls = cqm_ctrl->ls;

	ls_intr_stat = cbm_r32(ls + IRNICR_LS);
	ls_intr_stat &= (3 << (pid * 2 + IRNICR_LS_INT_PORT0_POS));
	cbm_w32(ls + IRNCR_LS, ls_intr_stat);
}

static struct sk_buff *__build_skb_cqm(void *data, unsigned int frag_size,
				       u8 pool, gfp_t priority)
{
	u32 buf_size;

	buf_size = bm_pool_conf[pool].buf_frm_size;
	if (frag_size > buf_size) {
		dev_err(cqm_ctrl->dev, "Insufficient Headroom %d %d\n",
			frag_size, buf_size);
		return NULL;
	}
	return __build_skb(data + CQM_POOL_METADATA + BM_METADATA_OFFSET,
			   frag_size);
}

static struct sk_buff *build_skb_cqm(void *data, unsigned int frag_size,
				     gfp_t priority)
{
	u8 pool = 0, policy = 0;

	if (get_metadata((u8 *)data + BM_METADATA_OFFSET, &pool, &policy)) {
		dev_err(cqm_ctrl->dev, "cpu buff for %s\n", __func__);
		return NULL;
	}
	return __build_skb_cqm(data, frag_size, pool, priority);
}

static unsigned int
NO_OPTIMIZE dp_dma_desc_get_buf(u32 dw2, u32 dw3, unsigned char **ptr)
{
	u64 ioc_pa = DP_MAKE_PHYS(FIELD_GET(DW3_ADDRH, dw3), dw2);

	*ptr = phys_to_virt(ioc_pa);
	return FIELD_GET(DW3_LEN, dw3);
}

static struct sk_buff *build_skb_from_dma_desc(const struct dp_dma_desc *desc)
{
	unsigned long buf_size;
	struct sk_buff *skb;
	unsigned char *data;
	unsigned char *head;
	unsigned int len;
	u8 policy, pool;

	len = dp_dma_desc_get_buf(desc->dw[2], desc->dw[3], &data);
	policy = (u8)FIELD_GET(DW3_POLICY, desc->dw[3]);
	pool = (u8)FIELD_GET(DW1_POOL, desc->dw[1]);

	head = cqm_cpubuff_get_buffer_head(data, pool);
	buf_size = bm_pool_conf[pool].buf_frm_size;

	add_metadata(head + BM_METADATA_OFFSET, pool, policy);
	skb = __build_skb(head, buf_size);
	if (likely(skb)) {
		skb->DW0 = desc->dw[0];
		skb->DW1 = desc->dw[1];
		skb->DW2 = desc->dw[2];
		skb->DW3 = desc->dw[3];
		skb->data = data;
		skb->len = len;
#ifdef NET_SKBUFF_DATA_USES_OFFSET
		skb->tail = data - head + len;
#else
		skb->tail = data + len;
#endif
		skb->buf_base = head;
		skb->truesize = buf_size;
	} else {
		cqm_buff_free(head);
	}
	return skb;
}

static struct sk_buff *cqm_alloc_skb(unsigned int size, gfp_t priority)
{
	struct sk_buff *skbuf = NULL;
	void *buf = NULL;
	u32 buf_size;
	u32 cpu = smp_processor_id();

	size = SKB_DATA_ALIGN(size + PP_HEADROOM) +
			SKB_DATA_ALIGN(sizeof(struct skb_shared_info));
	buf = cqm_buffer_alloc(cpu, BIT(1),
			       size, &buf_size, PP_HEADROOM);
	if (!buf)
		return NULL;
	buf -= PP_HEADROOM;
	skbuf = build_skb_cqm((void *)buf, size, priority);
	if (skbuf) {
		skb_reserve(skbuf, PP_HEADROOM - CQM_POOL_METADATA -
			    BM_METADATA_OFFSET);
	} else {
		pr_err("build_skb_cqm failed\n");
		cqm_buff_free(buf);
	}
	return skbuf;
}

static void NO_OPTIMIZE cqm_deq_port_flush(int port)
{
	int port_type = find_dqm_port_type(port);
	u32 dw3, no_packet = 0;
	u32 i = 0;
	u32 d_num = cqm_ctrl->dqm_port_info[port].deq_info.num_desc;
	u32 total_free_cnt = 0;
	u32 dqpc_reg;
	void *desc3_addr_base;

	if (port_type == DQM_DMA_TYPE)
		desc3_addr_base =
			cqm_ctrl->dmadesc_128 + CQM_DEQ_DMA_DESC(port, 0) + 0xc;
	else if (port_type == DQM_PON_TYPE || port_type == DQM_DOCSIS_TYPE)
		desc3_addr_base =
			cqm_ctrl->dmadesc_128 + CQM_DEQ_PON_DMA_DESC(port, 0)
			+ 0xc;
	else
		return;

	while (no_packet < 2 * d_num) {
		dw3 = cbm_r32(desc3_addr_base + i * 0x10);
		if (dw3 & OWN_BIT) {
			total_free_cnt++;
			CQM_DEBUG(CQM_DBG_FLAG_PORT_FLUSH,
				  "%s desc_%d, addr, dw3:%x, cnt:%d\n",
				  __func__, i, dw3, total_free_cnt);
			cbm_w32(desc3_addr_base + i * 0x10,
				(dw3 & ~OWN_BIT) | COMPLETE_BIT);
			no_packet = 0;
		} else {
			no_packet++;
			CQM_DEBUG(CQM_DBG_FLAG_PORT_FLUSH,
				  "no_packet %2d desc_%d:0x%x\n",
				  no_packet, i, dw3);
		}
		i = (i + 1) % d_num;
	}

	if (port_type == DQM_DMA_TYPE) {
		dqpc_reg = cbm_r32(cqm_ctrl->deq + DQ_DMA_PORT(port, dqpc));
		CQM_DEBUG(CQM_DBG_FLAG_DMA_PORT, "%s: DQPC_EGP_%d = %d\n",
			  __func__, port, dqpc_reg);
	} else if (port_type == DQM_PON_TYPE || port_type == DQM_DOCSIS_TYPE) {
		CQM_DEBUG(CQM_DBG_FLAG_PON_PORT,
			  "%s: port%d, total_free_cnt = %d\n",
			  __func__, port, total_free_cnt);
	}
}

static void NO_OPTIMIZE cqm_deq_pon_port_disable(u32 port, u32 alloc_flags)
{
	struct cqm_dqm_port_info *p_info;
	void *deq = cqm_ctrl->deq;
	struct dma_chan *chan;
	int i;
	u32 val;
	struct pib_stat stat;

	p_info = &cqm_ctrl->dqm_port_info[CQM_PON_IP_PORT_START];
	chan = p_info->pch;

	if (alloc_flags & (DP_F_EPON | DP_F_GPON)) {
		val = ((port - CQM_PON_IP_PORT_START) <<
			PIB_PON_IP_CMD_PORT_ID_POS) | 0x28000;
		cbm_w32(cqm_ctrl->pib + PIB_PON_IP_CMD, val);
		CQM_DEBUG(CQM_DBG_FLAG_PON_PORT,
			  "%s: write CQEM_PIB_PIB_PON_IP_CMD %lx , val %x\n",
			  __func__,
			  (unsigned long)(cqm_ctrl->pib + PIB_PON_IP_CMD),
			  val);
		pib_status_get(&stat);
		CQM_DEBUG(CQM_DBG_FLAG_PON_PORT,
			  "%s waiting for fifo_emtpy %d\n",
			  __func__, stat.fifo_empty);
		while (!stat.fifo_empty)
			pib_status_get(&stat);

		CQM_DEBUG(CQM_DBG_FLAG_PON_PORT,
			  "%s disable all DQ PON PORT %d - %d\n",
			  __func__,
			  CQM_PON_IP_PORT_START,
			  CQM_PON_IP_PORT_END);
		for (i = CQM_PON_IP_PORT_START;
		     i < CQM_PON_IP_PORT_END; i++)
			cbm_w32((deq + DQ_PON_PORT((i), cfg)), 0);
	} else if (alloc_flags & DP_F_FAST_ETH_WAN) {
		if (chan) {
			CQM_DEBUG(CQM_DBG_FLAG_PON_PORT,
				  "%s dma_chan off for port %d\n",
				__func__, port);
			dmaengine_pause(chan);
		}
		cqm_deq_port_flush(port);
		cbm_w32((deq + DQ_PON_PORT((port), cfg)), 0);
	}
}

s32 NO_OPTIMIZE qos_q_flush(s32 cqm_inst, s32 cqm_drop_port,
			    s32 qid, s32 nodeid)
{
	void *dma_desc = cqm_ctrl->dmadesc_128;
	int no_packet = 0;
	u32 port = cqm_ctrl->flush_port;
	u32 reg, dqpc_reg;
	u32 i = cqm_ctrl->dqm_port_info[port].flush_index;
	u32 desc_num = cqm_ctrl->dqm_port_info[port].deq_info.num_desc;
	u32 total_flush = 0;
	u32 qocc_q = 0;
	struct dp_qos_queue_info qocc_info;

	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s cqm_inst: %d cqm_drop_port: %d qid: %d nodeid: %d\n",
		  __func__, cqm_inst, cqm_drop_port, qid, nodeid);

	qocc_info.nodeid = nodeid;
	qocc_info.inst = cqm_inst;

	while ((no_packet < Q_FLUSH_NO_PACKET_CNT) || (qocc_q)) {
		reg = cbm_r32(dma_desc +
				CQM_DEQ_DMA_DESC(port, i) + 0xc);
		if (reg & OWN_BIT) {
			cbm_w32(dma_desc +
				CQM_DEQ_DMA_DESC(port, i) + 0xc,
				(reg & ~OWN_BIT) | COMPLETE_BIT);
			i = (i + 1) % desc_num;
			no_packet = 0;
			total_flush++;
		} else {
			no_packet++;
			CQM_DEBUG(CQM_DBG_FLAG_Q_FLUSH, "%s:no_packet %d\n",
				  __func__, no_packet);
			if (no_packet >= Q_FLUSH_NO_PACKET_CNT) {
				if (dp_qos_get_q_mib(&qocc_info,
						     0) == DP_SUCCESS) {
					qocc_q = qocc_info.qocc;
					if (qocc_q)
						no_packet = 0;
				} else {
					qocc_q = 0;
					dev_dbg(cqm_ctrl->dev,
						"qocc read error!\n");
				}
				CQM_DEBUG(CQM_DBG_FLAG_Q_FLUSH,
					  "%s qid:%d nodeid:%d qocc:%d\n",
					 __func__, qid, nodeid, qocc_q);
			}
		}
	}
	cqm_ctrl->dqm_port_info[port].flush_index = i;

	dqpc_reg = cbm_r32(cqm_ctrl->deq + DQ_DMA_PORT(port, dqpc));
	CQM_DEBUG(CQM_DBG_FLAG_Q_FLUSH,
		  "%s: DQPC_EGP_%d = %d, total_flush = %d\n",
		 __func__, port, dqpc_reg, total_flush);
	return CBM_SUCCESS;
}

static void rtn_ptr_cache_invalidate(int rd_ptr, int rtn_cnt, int end)
{
	if (end > 0) {
	/*this mean there is wrap around */
		dma_sync_single_for_cpu(cqm_ctrl->dev,
					cqm_ctrl->cpu_rtn_phy +
					rd_ptr * LGM_SKB_PTR_SIZE,
					(CQM_CPU_POOL_BUF_ALW_NUM - rd_ptr) *
					LGM_SKB_PTR_SIZE, DMA_FROM_DEVICE);
		dma_sync_single_for_cpu(cqm_ctrl->dev,
					cqm_ctrl->cpu_rtn_phy,
					(end) * LGM_SKB_PTR_SIZE,
					DMA_FROM_DEVICE);
	} else {
		dma_sync_single_for_cpu(cqm_ctrl->dev,
					cqm_ctrl->cpu_rtn_phy +
					rd_ptr * LGM_SKB_PTR_SIZE,
					rtn_cnt * LGM_SKB_PTR_SIZE,
					DMA_FROM_DEVICE);
	}
}

static int cpu_pool_free(void)
{
	int to_proc = 0;
	unsigned long *base_addr;
	int i, end, start;
	static int rd_ptr;
	void *base = cqm_ctrl->cqm;
	unsigned long *metadata;
	unsigned long *data_ptr;
	struct sk_buff *skb_to_free;

	/* Process only a multiple RTN_PTRS_IN_CACHELINE
	 * to prevent garbage getting flushed for the rd_ptr write
	 * to the rd_ptr index by the free tasklet. RTN_PTRS_IN_CACHELINE
	 * should be at least 8 for batch processing of the datapointers writes
	 * from the CQM ENQ Manager
	 */
	to_proc = RTN_PTRS_IN_CACHELINE;
	end = rd_ptr + to_proc - CQM_CPU_POOL_BUF_ALW_NUM;
	rtn_ptr_cache_invalidate(rd_ptr, to_proc, end);
	start = rd_ptr;
	for (i = 0; i < to_proc; i++) {
		if (test_bit(63, &cqm_ctrl->cpu_rtn_ptr[start])) {
			/*error: rtn buff ptr is not ready*/
			CQM_DEBUG(CQM_DBG_FLAG_FE_POLICY,
				  "data_ptr:%lx rd_ptr %d\n",
				  cqm_ctrl->cpu_rtn_ptr[start],
				  rd_ptr);
			return CBM_FAILURE;
		}
		start++;
		start = start % CQM_CPU_POOL_BUF_ALW_NUM;
	}
	start = rd_ptr;
	for (i = 0; i < to_proc; i++) {
		data_ptr = (unsigned long *)
			cqm_ctrl->cpu_rtn_ptr[rd_ptr];
		metadata = (unsigned long *)
			((unsigned long)data_ptr - LGM_SKB_PTR_SIZE);
		base_addr = (unsigned long *)__va
				((dma_addr_t)metadata);

		skb_to_free = (struct sk_buff *)(*base_addr);
		cqm_ctrl->cpu_rtn_ptr[rd_ptr] = BIT(63);
		rd_ptr++;
		rd_ptr = rd_ptr % CQM_CPU_POOL_BUF_ALW_NUM;
		CQM_UP_STATS(cqm_dbg_cntrs[CQM_CPU_POLICY][CQM_CPU_POOL].isr_free_cnt);

		CQM_DEBUG(CQM_DBG_FLAG_FE_POLICY,
			  "%s:cpu_rtn[%d]:%lx, freed skb:%lx\n",
			  __func__, rd_ptr,
			  (unsigned long)data_ptr,
			  (unsigned long)skb_to_free);

		dev_kfree_skb_any(skb_to_free);
	}

	rtn_ptr_cache_invalidate(start, to_proc, end);
	cbm_w32(base + CBM_CPU_POOL_ENQ_DEC, to_proc);

	atomic64_add(to_proc, &(cqm_dbg_cntrs[CQM_CPU_POLICY]
		 [CQM_CPU_POOL].free_cnt));
	atomic_sub(to_proc, &cqm_fe_policy.enq_cnt);
	/*make sure decrement cnt is written*/
	return CBM_SUCCESS;
}

int is_fsqm(u32 desc2)
{
	u32 p_st = (u32)bm_pool_conf[CQM_LGM_TOTAL_POOLS - 1].pool_start_low;
	u32 p_end = (u32)bm_pool_conf[CQM_LGM_TOTAL_POOLS - 1].pool_end_low;

	if (desc2 >= p_st && desc2 < p_end)
		return 1;
	return 0;
}

static inline bool is_bm(unsigned long buf)
{
	return (buf >= bm_pool_conf[cqm_ctrl->cpu_base_pool].pool_start_low &&
		buf < bm_pool_conf[cqm_ctrl->cpu_base_pool + 3].pool_end_low);
}

static int NO_OPTIMIZE cqm_poll_dp(struct napi_struct *napi, int budget)
{
	struct cbm_desc_list desc_list;
	struct sk_buff *skb;
	unsigned long len_to_inval;
	int data_len = 0, j = 0;
	unsigned long data_ptr, ptr;
	int real_len = 0, temp_len = 0, new_offset;
	unsigned long buf_base;
	u8 pool, policy, bm_pool;
	struct cqm_napi *cqm_napi = container_of(napi, struct cqm_napi, napi);
	int intr_line = cqm_napi->line;
	int cpu_pool_ioc = cqm_ctrl->is_cpu_pool_ioc;
	struct cbm_desc *deq_p;

	/* Dequeue the packet */
	j = 0;
	/* Read DW3 of descriptor 0 or 1 */
	while (cbm_r32(cqm_napi->desc + CPU_PORT_DESC3_OFFSET +
		       (cqm_napi->read_idx * CPU_PORT_DESC_OFFSET)) & BIT(31)) {
		if (j >= budget)
			break;
		j++;

		*((u64 *)&desc_list.desc) =
			readq(cqm_napi->desc + (cqm_napi->read_idx * CPU_PORT_DESC_OFFSET));
		*((u64 *)&desc_list.desc.desc2) =
			readq(cqm_napi->desc + CPU_PORT_DESC_OFFSET / 2 +
			      (cqm_napi->read_idx * CPU_PORT_DESC_OFFSET));

		/* To clear own bit */
		cbm_w32(cqm_napi->desc + CPU_PORT_DESC3_OFFSET +
			(cqm_napi->read_idx * CPU_PORT_DESC_OFFSET), 0);

		cqm_napi->read_idx = !cqm_napi->read_idx;

		/* Build the SKB */
		data_len = cqm_desc_data_len(desc_list.desc.desc3);
		pool = (u8)cqm_desc_data_pool(desc_list.desc.desc1);
		policy = (u8)cqm_desc_data_policy(desc_list.desc.desc3);
		data_ptr = desc_list.desc.desc2;
		CQM_DEBUG(CQM_DBG_FLAG_RX_DP,
			  "%s desc 0~3: %x, %x, %x, %x, data_len:%d pool:%u policy:%u\n",
			  __func__,
			  desc_list.desc.desc0, desc_list.desc.desc1,
			  desc_list.desc.desc2, desc_list.desc.desc3,
			  data_len, pool, policy);
		deq_p = &get_cpu_var(cqm_deq_desc);
		deq_p->desc0 = desc_list.desc.desc0;
		deq_p->desc1 = desc_list.desc.desc1;
		deq_p->desc2 = desc_list.desc.desc2;
		deq_p->desc3 = desc_list.desc.desc3;
		put_cpu_var(cqm_deq_desc);
		ptr = ((desc_list.desc.desc3 >>
			PTR_RTN_CPU_DW3_EGP_0_PTR_MSB_POS) & 0xf);
		ptr <<= 32;
		data_ptr |= ptr;
		if (is_fsqm(data_ptr)) {
			CQM_DEBUG(CQM_DBG_FLAG_RX_DP, "FSQM buffer 0x%lx %u %u\n",
				  data_ptr, pool, policy);
			cqm_buff_free((void *)data_ptr);
			goto HANDLE_INTR;
		}
		data_ptr = (unsigned long)__va(data_ptr);
		if (get_buf_pool((void *)data_ptr, bm_pool_conf, &bm_pool))
			break;

		if (bm_pool != pool || bm_pool >= CQM_LGM_TOTAL_POOLS ||
		    ((!is_cpu_iso_pool(pool)) && cpu_pool_ioc)) {
			dev_err(cqm_ctrl->dev, "Incorrect Pool/policy info\n");
			dev_err(cqm_ctrl->dev, "ptr 0x%lx pool %u policy %u\n",
				data_ptr, pool, policy);
			dev_err(cqm_ctrl->dev, "Actual pool %u\n", bm_pool);
			break;
		}
		buf_base = (unsigned long)cqm_cpubuff_get_buffer_head((void *)data_ptr, pool);

		if (!cpu_pool_ioc) {
			len_to_inval = data_ptr - buf_base + data_len;
			dma_map_single_attrs(cqm_ctrl->dev, (void *)(buf_base),
					     len_to_inval, DMA_FROM_DEVICE,
					     DMA_ATTR_NON_CONSISTENT);
			if (dma_mapping_error(cqm_ctrl->dev,
					      desc_list.desc.desc2)) {
				dev_err(cqm_ctrl->dev, "%s DMA map failed\n", __func__);
				break;
			}
		}

		/* prefetch cache line with the header and data respectively,
		 * which will be processed soon or later.
		 */
		prefetch((void *)buf_base);
		prefetch((void *)data_ptr);

		new_offset = (data_ptr - buf_base - CQM_POOL_METADATA - BM_METADATA_OFFSET);
		if (new_offset < 0) {
			dev_err(cqm_ctrl->dev, "%s headroom checked failed\n",
				__func__);
			cqm_buff_free((void *)data_ptr);
			continue;
		}
		temp_len = data_len + new_offset;
		real_len = SKB_DATA_ALIGN(temp_len) +
			   SKB_DATA_ALIGN(sizeof(struct skb_shared_info));
		add_metadata((u8 *)buf_base + BM_METADATA_OFFSET, pool, policy);
		if (pool < CQM_LGM_TOTAL_BM_POOLS)
			CQM_UP_STATS(cqm_dbg_cntrs[policy][pool].rx_cnt);
		skb = __build_skb_cqm((void *)buf_base, real_len, pool, GFP_ATOMIC);
		if (skb) {
			skb_reserve(skb, new_offset);
			skb->DW0 = desc_list.desc.desc0;
			skb->DW1 = desc_list.desc.desc1;
			skb->DW2 = data_ptr;
			skb->DW3 = desc_list.desc.desc3;
			skb_put(skb, data_len);
			CQM_DEBUG(CQM_DBG_FLAG_RX_DP,
				  "%s: skb head=0x%lx, data=0x%lx, tail=0x%x, end=0x%xn",
				  __func__, (unsigned long)skb->head,
				  (unsigned long)skb->data, skb->tail, skb->end);
			if (!is_bm(data_ptr)) {
				dev_err(cqm_ctrl->dev, "skb 0x%p, dptr %lu\n",
					skb, data_ptr);
				cqm_buff_free((void *)data_ptr);
				goto HANDLE_INTR;
			}
			skb->buf_base = (unsigned char *)buf_base;

			CQM_DEBUG(CQM_DBG_FLAG_RX_DP,
				  "call dp_rx skb: buf_base 0x%lx DW0~3:%x %x %x %x\n",
				  (unsigned long)skb->buf_base,
				  skb->DW0, skb->DW1, skb->DW2, skb->DW3);
			dp_rx(skb, 0);
		} else {
			dev_err(cqm_ctrl->dev, "%s:failure in allocating skb\n", __func__);
			cqm_buff_free((void *)data_ptr);
		}
	}
HANDLE_INTR:
	if (j < budget)
		if (likely(napi_complete_done(napi, j))) {
			cbm_w32(cqm_ctrl->deq +
				DQ_CPU_PORT(intr_line + CQM_CPU_VM_PORT_OFFSET,
					    irncr), BIT(1));
			CQM_DEBUG(CQM_DBG_FLAG_RX_DP,
				  "%s (%d): write reg: 0x%pS val: 0x%lx\n",
				  __func__, __LINE__,
				  cqm_ctrl->deq_phys +
				  DQ_CPU_PORT(intr_line + CQM_CPU_VM_PORT_OFFSET, irncr),
				  BIT(1));
			cbm_w32(cqm_ctrl->cqm + CQM_INT_LINE(intr_line, egp_irncr), BIT(intr_line) << CQM_CPU_VM_PORT_OFFSET);
			CQM_DEBUG(CQM_DBG_FLAG_RX_DP, "%s (%d): write reg: 0x%pS val: 0x%lx\n",
				  __func__, __LINE__, cqm_ctrl->cqm_phys +
				  CQM_INT_LINE(intr_line, egp_irncr), BIT(intr_line) << CQM_CPU_VM_PORT_OFFSET);
			cbm_w32((cqm_ctrl->cqm + CQM_INT_LINE(intr_line, egp_irnen)),
				BIT(intr_line) << CQM_CPU_VM_PORT_OFFSET);
			CQM_DEBUG(CQM_DBG_FLAG_RX_DP,
				  "%s (%d): write reg: 0x%pS val: 0x%lx\n",
				  __func__, __LINE__,
				  cqm_ctrl->cqm_phys +
				  CQM_INT_LINE(intr_line, egp_irnen), BIT(intr_line) << CQM_CPU_VM_PORT_OFFSET);
			cbm_w32((cqm_ctrl->cqm + CQM_INT_LINE(intr_line, cbm_irnen)),
				cqm_ctrl->cbm_line_map[intr_line]);
		}
	return j;
}

static int cqm_process_pkt(struct cqm_napi *cqm_napi, int pkts)
{
	struct cbm_desc_list desc_list;
	struct sk_buff *skb;
	unsigned long len_to_inval;
	int data_len = 0, j = 0;
	unsigned long data_ptr, ptr;
	int real_len = 0, temp_len = 0, new_offset;
	unsigned long buf_base;
	u8 pool, policy, bm_pool;
	int intr_line = cqm_napi->line;
	int cpu_pool_ioc = cqm_ctrl->is_cpu_pool_ioc;
	struct cbm_desc *deq_p;

	for (j = 0; j < pkts; j++) {
		*((u64 *)&desc_list.desc) = readq(cqm_napi->desc);
		*((u64 *)&desc_list.desc.desc2) = readq(cqm_napi->desc + 8);

		/* Build the SKB */
		data_len = cqm_desc_data_len(desc_list.desc.desc3);
		if (data_len == 0x3FFF) {
			data_len = cqm_desc_data_len_e(desc_list.desc.desc0);
			desc_list.desc.desc0 &= 0xFFFF;
		}
		pool = (u8)cqm_desc_data_pool(desc_list.desc.desc1);
		policy = (u8)cqm_desc_data_policy(desc_list.desc.desc3);
		data_ptr = desc_list.desc.desc2;
		CQM_DEBUG(CQM_DBG_FLAG_RX,
			  "%s desc 0~3: %x, %x, %x, %x, data_len:%d pool:%u policy:%u\n",
			  __func__,
			  desc_list.desc.desc0, desc_list.desc.desc1,
			  desc_list.desc.desc2, desc_list.desc.desc3,
			  data_len, pool, policy);
		deq_p = &get_cpu_var(cqm_deq_desc);
		deq_p->desc0 = desc_list.desc.desc0;
		deq_p->desc1 = desc_list.desc.desc1;
		deq_p->desc2 = desc_list.desc.desc2;
		deq_p->desc3 = desc_list.desc.desc3;
		put_cpu_var(cqm_deq_desc);
		ptr = ((desc_list.desc.desc3 >>
			PTR_RTN_CPU_DW3_EGP_0_PTR_MSB_POS) & 0xf);
		ptr <<= 32;
		data_ptr |= ptr;
		if (is_fsqm(data_ptr)) {
			CQM_DEBUG(CQM_DBG_FLAG_RX, "FSQM buffer 0x%lx %u %u\n",
				  data_ptr, pool, policy);
			cqm_buff_free((void *)data_ptr);
			continue;
		}
		data_ptr = (unsigned long)__va(nioc_addr_to_ioc_addr(data_ptr));

		if (get_buf_pool((void *)data_ptr, bm_pool_conf, &bm_pool))
			continue;

		if (bm_pool != pool || bm_pool >= CQM_LGM_TOTAL_POOLS) {
			dev_err(cqm_ctrl->dev, "Incorrect Pool/policy info\n");
			dev_err(cqm_ctrl->dev, "ptr 0x%lx pool %u policy %u\n",
				data_ptr, pool, policy);
			dev_err(cqm_ctrl->dev, "Actual pool %u\n", bm_pool);
			continue;
		}

		buf_base = (unsigned long)cqm_cpubuff_get_buffer_head((void *)data_ptr, pool);

		if (!cpu_pool_ioc || !is_cpu_iso_pool(pool)) {
			len_to_inval = data_ptr - buf_base + data_len;
			dma_map_single_attrs(cqm_ctrl->dev, (void *)(buf_base),
					     len_to_inval, DMA_FROM_DEVICE,
					     DMA_ATTR_NON_CONSISTENT);
			if (dma_mapping_error(cqm_ctrl->dev,
					      desc_list.desc.desc2)) {
				dev_err(cqm_ctrl->dev, "%s DMA map failed\n", __func__);
				continue;
			}
		}

		/* prefetch cache line with the header and data respectively,
		 * which will be processed soon or later.
		 */
		prefetch((void *)buf_base);
		prefetch((void *)data_ptr);

		new_offset = (data_ptr - buf_base - CQM_POOL_METADATA - BM_METADATA_OFFSET);
		if (new_offset < 0) {
			dev_err(cqm_ctrl->dev, "%s headroom checked failed\n",
				__func__);
			cqm_buff_free((void *)data_ptr);
			continue;
		}
		temp_len = data_len + new_offset;
		real_len = SKB_DATA_ALIGN(temp_len) +
			   SKB_DATA_ALIGN(sizeof(struct skb_shared_info));
		add_metadata((u8 *)buf_base + BM_METADATA_OFFSET, pool, policy);
		if (pool < CQM_LGM_TOTAL_BM_POOLS)
			CQM_UP_STATS(cqm_dbg_cntrs[policy][pool].rx_cnt);
		skb = __build_skb_cqm((void *)buf_base, real_len, pool, GFP_ATOMIC);
		if (skb) {
			skb_reserve(skb, new_offset);
			skb->DW0 = desc_list.desc.desc0;
			skb->DW1 = desc_list.desc.desc1;
			skb->DW2 = data_ptr;
			skb->DW3 = desc_list.desc.desc3;
			skb_put(skb, data_len);
			CQM_DEBUG(CQM_DBG_FLAG_RX,
				  "%s: skb head=0x%lx, data=0x%lx, tail=0x%x, end=0x%xn",
				  __func__, (unsigned long)skb->head,
				  (unsigned long)skb->data, skb->tail, skb->end);
			skb->buf_base = (unsigned char *)buf_base;
#ifdef CONFIG_RFS_ACCEL
			skb_record_rx_queue(skb, intr_line);
#endif /* CONFIG_RFS_ACCEL */
			CQM_DEBUG(CQM_DBG_FLAG_RX,
				  "call dp_rx skb: buf_base 0x%lx DW0~3:%x %x %x %x\n",
				  (unsigned long)skb->buf_base,
				  skb->DW0, skb->DW1, skb->DW2, skb->DW3);
			dp_rx(skb, 0);
		} else {
			dev_err(cqm_ctrl->dev, "%s:failure in allocating skb\n", __func__);
			cqm_buff_free((void *)data_ptr);
		}
	}

	return j;
}

/** Trigger when taklet schedule calls*/
static int NO_OPTIMIZE cqm_poll(struct napi_struct *napi, int budget)
{
	unsigned long pkt_recvd;
	void *c_base = cqm_ctrl->cqm;
	void *ls_base = cqm_ctrl->ls;
	struct cqm_napi *cqm_napi = container_of(napi, struct cqm_napi, napi);
	int intr_line = cqm_napi->line;
	int pkt_processed = 0, budget_left;

	budget_left = budget;

	while ((pkt_recvd = get_val(cbm_r32(ls_base +
					    CBM_LS_PORT(intr_line, status)),
					    LS_STATUS_PORT0_QUEUE_LEN_MASK,
					    LS_STATUS_PORT0_QUEUE_LEN_POS)) > 0) {
		CQM_DEBUG(CQM_DBG_FLAG_RX, "%s:pkt_recvd %lu cpu_id:%d\n",
			  __func__, pkt_recvd, smp_processor_id());

		if (pkt_recvd > budget_left)
			pkt_recvd = budget_left;

		pkt_processed += cqm_process_pkt(cqm_napi, pkt_recvd);

		budget_left -= pkt_recvd;

		if (budget_left <= 0)
			break;
	}

	if (pkt_processed < budget)
		if (likely(napi_complete_done(napi, pkt_processed))) {
			ls_intr_handler(intr_line);
			/*wmb() and dummy read has been removed*/
			cbm_w32(c_base + CQM_INT_LINE(intr_line, cbm_irncr),
				BIT(intr_line) << CBM_IRNEN_0_LSI_POS);
			CQM_DEBUG(CQM_DBG_FLAG_RX, "clear int line %d :0x%x\n",
				  intr_line, cbm_r32(c_base +
				  CQM_INT_LINE(intr_line, cbm_irncr)));
			cbm_w32((cqm_ctrl->cqm + CQM_INT_LINE(intr_line, cbm_irnen)),
				cqm_ctrl->cbm_line_map[intr_line]);
			cbm_w32((cqm_ctrl->cqm + CQM_INT_LINE(intr_line, egp_irnen)),
				BIT(intr_line) << CQM_CPU_VM_PORT_OFFSET);
		}

	return pkt_processed;
}

static irqreturn_t cqm_isr(int irq, void *dev_id)
{
	int line;
	u32 cbm_intr_status = 0;
	u32 dqm_intr_status = 0;
	void *c_base = cqm_ctrl->cqm;

	if (get_intr_to_line(irq, &line))
		return IRQ_NONE;

	dqm_intr_status = cbm_r32(c_base + CQM_INT_LINE(line, egp_irncr));
	CQM_DEBUG(CQM_DBG_FLAG_RX_DP, "%s(%d): dqm_intr_status: 0x%x reg: 0x%pS\n",
		  __func__, __LINE__, dqm_intr_status,
		  cqm_ctrl->cqm_phys + CQM_INT_LINE(line, egp_irncr));
	cbm_intr_status = cbm_r32(c_base + CQM_INT_LINE(line, cbm_irncr));
	cbm_w32((c_base + CQM_INT_LINE(line, egp_irnen)), 0);
	CQM_DEBUG(CQM_DBG_FLAG_RX_DP, "%s(%d): write: reg: 0x%pS val: 0x%x\n",
		  __func__, __LINE__, cqm_ctrl->cqm_phys + CQM_INT_LINE(line, egp_irnen), 0);
	/*Dummy Read, for write flush*/
	cbm_r32((c_base + CQM_INT_LINE(line, egp_irnen)));
	/* Disable line interrupts */
	cbm_w32((c_base + CQM_INT_LINE(line, cbm_irnen)), 0);
	/*Dummy Read, for write flush*/
	cbm_r32((c_base + CQM_INT_LINE(line, cbm_irnen)));

	if (dqm_intr_status)
		napi_schedule(&cqm_ctrl->cqm_napi_dp[line].napi);

	if (cbm_intr_status & (0x1 << (line + 4))) {
		/* Schedule the tasklet */
		napi_schedule(&cqm_ctrl->cqm_napi[line].napi);
	}
	return IRQ_HANDLED;
}

/*	CBM Interrupt init
 *	Register to handle all the 8 lines ,
 *	need to be adapted to work with MPE FW
 */
static int NO_OPTIMIZE cqm_interrupt_init(void)
{
	int ret = CBM_SUCCESS, i;
	u32 start, end;

	start = cqm_ctrl->dqm_pid_cfg.cpu_start;
	end = cqm_ctrl->dqm_pid_cfg.cpu_end;

#ifdef CONFIG_RFS_ACCEL
	/* Allocation based on actual Rx queues/DQ ports initialized */
	cqm_ctrl->rx_cpu_rmap = alloc_irq_cpu_rmap(end - start + 1);
#endif /* CONFIG_RFS_ACCEL */

	for (i = start; i <= end; i++) {
		if (!cqm_ctrl->cbm_irq[i])
			continue;
		if (cqm_ctrl->cpu_owner[i / 2] == CBM_CORE_DPDK)
			continue;

#ifdef CONFIG_RFS_ACCEL
		if (cqm_ctrl->rx_cpu_rmap) {
			ret = irq_cpu_rmap_add(cqm_ctrl->rx_cpu_rmap,
					       cqm_ctrl->cbm_irq[i]);
			if (ret) {
				dev_err(cqm_ctrl->dev,
					"Failed adding irq rmap for IRQ = %lu",
					cqm_ctrl->cbm_irq[i]);
				free_irq_cpu_rmap(cqm_ctrl->rx_cpu_rmap);
				cqm_ctrl->rx_cpu_rmap = NULL;
				/* No need to return - continue remaining IRQ
				 * init without rmap
				 */
			}
		}
#endif /* CONFIG_RFS_ACCEL */

		ret = devm_request_irq(cqm_ctrl->dev, cqm_ctrl->cbm_irq[i],
				       cqm_isr, 0, "cbm_dqm", NULL);
		if (ret) {
			pr_err("Can not get IRQ - %d", ret);
			return ret;
		}

#if defined(CONFIG_SMP)
		/* Set the Affinity dq0, dq1 -> cpu0 */
		ret = irq_set_affinity(cqm_ctrl->cbm_irq[i], cpumask_of(i / 2));
		if (ret < 0) {
#ifdef CONFIG_RFS_ACCEL
			free_irq_cpu_rmap(cqm_ctrl->rx_cpu_rmap);
			cqm_ctrl->rx_cpu_rmap = NULL;
#endif /* CONFIG_RFS_ACCEL */
			dev_err(cqm_ctrl->dev, "Can't set affinity IRQ - %lu",
				cqm_ctrl->cbm_irq[i]);
			devm_free_irq(cqm_ctrl->dev, cqm_ctrl->cbm_irq[i],
				      NULL);
		} else
#endif
			cqm_ctrl->cbm_line[i] = i;
	}

return ret;
}

/*CBM interrupt mapping initialization*/
static void cqm_intr_mapping_init(void)
{
	void *c_base = cqm_ctrl->cqm;
	unsigned long intr_map;
	int i;

	/*map LS, PIB intr to line 0 and CPU pool Interrupt*/
	for (i = 0; i < CQM_MAX_INTR_LINE; i++) {
		intr_map = (BIT(i) << CBM_IRNEN_0_LSI_POS);
		cqm_ctrl->cbm_line_map[i] = intr_map;
		cbm_w32(c_base + CQM_INT_LINE(i, cbm_irnen), intr_map);
		cbm_w32((c_base + CQM_INT_LINE(i, egp_irnen)), BIT(i) << CQM_CPU_VM_PORT_OFFSET);
	}
}

static s32 cqm_igp_delay_set(s32 port_id, s32 delay)
{
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s: port_id: %d delay: %d\n",
		  __func__, port_id, delay);
	WARN_ON(!((port_id >= 0) && (port_id <= 32)));
	cbm_w32(cqm_ctrl->enq + EQ_CPU_PORT(port_id, dcntr),
		delay & 0x3f);
	return CBM_SUCCESS;
}

static s32 cqm_igp_delay_get(s32 port_id, s32 *delay)
{
	WARN_ON(!((port_id >= 0) && (port_id <= 32)));
	*delay = cbm_r32(cqm_ctrl->enq +
			 EQ_CPU_PORT(port_id, dcntr)) & 0x3f;
	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s: port_id: %d delay: %d\n",
		  __func__, port_id, *delay);

	return CBM_SUCCESS;
}

static void eqm_intr_ctrl(u32 val)
{
	int i;
	struct eqm_pid_s *enq_p = cqm_get_eqm_pid_cfg();

	for (i = enq_p->cpu_start; i <= enq_p->cpu_end; i++)
		cbm_w32(cqm_ctrl->enq + EQ_CPU_PORT(i, irnen),
			val);

	for (i = enq_p->vm_start; i <= enq_p->vm_end; i++)
		cbm_w32(cqm_ctrl->enq + EQ_CPU_PORT(i, irnen),
			val);

	for (i = enq_p->voice_start; i <= enq_p->qosbp_end; i++)
		cbm_w32(cqm_ctrl->enq + EQ_CPU_PORT(i, irnen),
			val);
}

static void dqm_intr_ctrl(u32 val)
{
	int i;

	for (i = 0; i < CPU_DQM_PORT_NUM; i++)
		cbm_w32(cqm_ctrl->deq + DQ_CPU_PORT(i, irnen),
			val);
}

static void dump_cqem_basic_regs(u32 flag)
{
	int i;
	void *c_base = cqm_ctrl->cqm;

	dev_dbg(cqm_ctrl->dev, "dump cqem basic registers\n");
	for (i = 0; i < CQM_MAX_INTR_LINE; i++) {
		print_reg("CBM_IRNCR\n", c_base + CQM_INT_LINE(i, cbm_irncr));
		print_reg("CBM_IRNEN\n", c_base + CQM_INT_LINE(i, cbm_irnen));
		print_reg("IGP_IRNCR\n", c_base + CQM_INT_LINE(i, igp_irncr));
		print_reg("IGP_IRNEN\n", c_base + CQM_INT_LINE(i, igp_irnen));
		print_reg("EGP_IRNCR\n", c_base + CQM_INT_LINE(i, egp_irncr));
		print_reg("EGP_IRNEN\n", c_base + CQM_INT_LINE(i, egp_irnen));
	}
	print_reg("CQEM SRAM BASE\n", (u32 *)(c_base + CBM_PB_BASE));
	print_reg("CQEM STAT\n", (u32 *)(c_base + CBM_CTRL));
	print_reg("CQEM STAT\n", (u32 *)(c_base + CBM_BUF_SIZE0_7));
	print_reg("CQEM STAT\n", (u32 *)(c_base + CBM_BUF_SIZE8_15));
	print_reg("CQEM STAT\n", (u32 *)(c_base + CBM_BSL_CTRL));
	print_reg("CQEM STATn", (u32 *)(c_base + CBM_BM_BASE));
	print_reg("CQEM STAT\n", (u32 *)(c_base + CBM_WRED_BASE));
	print_reg("CQEM STAT\n", (u32 *)(c_base + CBM_QPUSH_BASE));
	print_reg("CQEM STAT\n", (u32 *)(c_base + CBM_TX_CREDIT_BASE));
	print_reg("CQEM STAT\n",
		  (u32 *)(c_base + CBM_CPU_POOL_BUF_RTRN_START_ADDR));
	print_reg("CQEM STAT\n",
		  (u32 *)(c_base + CBM_CPU_POOL_BUF_ALW_NUM));
	print_reg("CQEM STAT\n", (u32 *)(c_base + CBM_VERSION_REG));
}

static int NO_OPTIMIZE init_cqm_basic(struct platform_device *pdev)
{
	u32 version;
	int frm_size_bit = (CQM_SRAM_FRM_SIZE == 128) ? 0 : 1;
	void *c_base = cqm_ctrl->cqm;
	u32 idx;
	u32 bufsize0_7 = 0;
	u32 bufsize8_15 = 0;
	u32 cqm_sram_base;
	int tmp_v, size;
	u32 reg_val;

	/* base address of internal packet buffer managed by the FSQM */
	cqm_sram_base =
		(u32)bm_pool_conf[CQM_LGM_TOTAL_POOLS - 1].pool_start_low;
	dev_dbg(cqm_ctrl->dev,
		"%s cqm_sram_base = 0x%x\n", __func__, cqm_sram_base);
	if (cqm_sram_base != 0 &&
	    bm_pool_conf[CQM_LGM_TOTAL_POOLS - 1].pool_end_low != 0)
		cbm_w32(c_base + CBM_PB_BASE, cqm_sram_base >> 4);

	/*Program the PBS bit to Select between
	 *Internal Packet Buffer frame size of 128 Bytes or 2 KBytes
	 */
	set_val((c_base + CBM_CTRL), frm_size_bit, CBM_CTRL_PBSEL_MASK,
		CBM_CTRL_PBSEL_POS);

	cqm_intr_mapping_init();
	if (cqm_interrupt_init()) {
		dev_err(cqm_ctrl->dev,
			"Error in cqm interrupt init\n");
		return CBM_FAILURE;
	}
	/* store various pool buf frame size to cqm-top look up table */
	for (idx = 0; idx < cqm_ctrl->total_pool_entries; idx++) {
		if (idx < 8)
			bufsize0_7 |= bm_pool_conf[idx].buf_frm_size_reg <<
				      ((idx % 8) * 0x4);
		else
			bufsize8_15 |= bm_pool_conf[idx].buf_frm_size_reg <<
				       ((idx % 8) * 0x4);
	}

	cbm_w32((c_base + CBM_BUF_SIZE0_7), bufsize0_7);
	cbm_w32((c_base + CBM_BUF_SIZE8_15), bufsize8_15);

	/*BSL config*/
	cbm_w32((c_base + CBM_BSL_CTRL),
		(1 << CBM_BSL_CTRL_BSL1_EN_POS));

	cbm_w32(c_base + CBM_BM_BASE, 0x0f00c000);
	cbm_w32(c_base + CBM_WRED_BASE, 0x0f000800);
	cbm_w32(c_base + CBM_QPUSH_BASE, 0x0f003000);
	cbm_w32(c_base + CBM_TX_CREDIT_BASE, 0x0f106000);
	cbm_w32(c_base + CBM_WRED_RESP_BASE, 0x0f6d0000);
	cbm_w32(c_base + CBM_TXPUSH_DMA_QOSBYPASS_BASE, 0x0e6b03b0);
	/*cbm_w32(c_base + CBM_EPON_BASE, 0x0e6c1a00);*/
	for (idx = 0; idx < 8; idx++) {
		cbm_w32(c_base + CBM_FSQM_QUEUE_REG_0 + (idx * 4),
			cbm_r32(cqm_ctrl->fsqm_desc + DESC_OMQ));
	}
	enable_buf_alloc_soffet(PP_HEADROOM);

	tmp_v =  TOT_DMA_HNDL - 1;
	size = CQM_CPU_POOL_BUF_ALW_NUM * LGM_SKB_PTR_SIZE;
	dev_info(cqm_ctrl->dev, "%s CPU_POOL_BUF size: %u\n", __func__, size);
	/*Address returned by genpool is 4K aligned*/
	cqm_ctrl->cpu_rtn_ptr = (unsigned long *)
		cqm_gen_pool_alloc(0, (size_t)size,
				   &cqm_ctrl->cpu_rtn_phy,
				   GEN_RW_POOL, cqm_ctrl->sai[0]);
	if (!cqm_ctrl->cpu_rtn_ptr) {
		dev_err(cqm_ctrl->dev, "Error in cpu_rtn_ptr allocation\n");
		return CBM_FAILURE;
	}

	dev_info(cqm_ctrl->dev,
		 "%s cpu_rtn_phy:0x%llx, cpu_rtn_ptr = %lx\n",
		 __func__, cqm_ctrl->cpu_rtn_phy,
		 (unsigned long)cqm_ctrl->cpu_rtn_ptr);
	for (idx = 0; idx < CQM_CPU_POOL_BUF_ALW_NUM; idx++)
		cqm_ctrl->cpu_rtn_ptr[idx] = BIT(63);
	dma_map_single_attrs(cqm_ctrl->dev,
			     (void *)cqm_ctrl->cpu_rtn_ptr,
			     CQM_CPU_POOL_BUF_ALW_NUM * LGM_SKB_PTR_SIZE,
			     DMA_TO_DEVICE, DMA_ATTR_NON_CONSISTENT);
	if (dma_mapping_error(cqm_ctrl->dev, cqm_ctrl->cpu_rtn_phy)) {
		dev_err(cqm_ctrl->dev, "%s DMA map failed\n",
			__func__);
		return CBM_FAILURE;
	}
	reg_val = cqm_ctrl->cpu_rtn_phy >> 4;
	cbm_w32((c_base + CBM_CPU_POOL_BUF_RTRN_START_ADDR), reg_val);
	cbm_w32((c_base + CBM_CPU_POOL_BUF_ALW_NUM),
		CQM_CPU_POOL_BUF_ALW_NUM);

	/*Init the CQM activity control registers for HW DVFS
	 *map the clock division ratio to the DVFS level in the incoming
	 *encoding
	 */
	cbm_w32((c_base + CBM_ACTIVITY_LEVEL_REG), 0x210);
	/*trigger for Low activity detection*/
	cbm_w32((c_base + CQM_LOW_ACT_COUNTER), 0x100000);
	/*trigger for High activity detection*/
	cbm_w32((c_base + CQM_HIGH_ACT_COUNTER), 0x200000);
	/*enable high and low activity levels from CQM*/
	cbm_w32((c_base + CBM_ACTIVITY_CTRL_REG), 0x3);
	cqm_ctrl->version = cbm_r32(c_base + CBM_VERSION_REG);
	dev_dbg(cqm_ctrl->dev,
		"Basic init of CQM successful ver Major: %u Minor: %u\n",
		(version & 0xf), ((version >> 8) & 0xf));
	dump_cqem_basic_regs(0);
	return CBM_SUCCESS;
}

static int get_bufsize(int size)
{
	switch (size) {
	case 128:
		return 0;
	break;
	case 256:
		return 1;
	break;
	case 512:
		return 2;
	break;
	case 1024:
		return 3;
	break;
	case 2048:
		return 4;
	break;
	case 4096:
		return 5;
	break;
	case 8192:
	case 65536:
		return 6;
	break;
	case 10240:
		return 7;
	break;
	};
	return 0;
}

/* helper function to get min_guaranteed from policy */
static u32 NO_OPTIMIZE get_min_guar_from_policy(const s16 id)
{
	struct cqm_bm_policy_params *info;

	info = container_of(&cqm_ctrl->cqm_bm_policy_dts[id].pp_policy_cfg,
			    struct cqm_bm_policy_params, pp_policy_cfg);

	return info->pp_policy_cfg.min_guaranteed;
}

/* helper function to get pool from policy */
static u8 NO_OPTIMIZE get_pool_from_policy(const s16 id)
{
	struct cqm_bm_policy_params *info;

	/* If the policy is not set, return invalid pool id */
	if (cqm_ctrl->cqm_bm_policy_dts[id].policy_type == MAX_SUP_DEVICE)
		return INVALID_POOL_ID;

	info = container_of(&cqm_ctrl->cqm_bm_policy_dts[id].pp_policy_cfg,
			    struct cqm_bm_policy_params, pp_policy_cfg);

	return info->pp_policy_cfg.pools_in_policy[0].pool_id;
}

/* helper function to get policy from device type */
static s16
NO_OPTIMIZE get_policy_from_type(enum CQM_SUP_DEVICE type, enum DIRECTION dir)
{
	struct cqm_bm_policy_params *info = NULL;
	u16 id;

	for (id = 0; id < CQM_LGM_NUM_BM_POLICY; id++) {
		info = &cqm_ctrl->cqm_bm_policy_dts[id];
		if (info->policy_type == type &&
		    info->direction & dir)
			return id;
	}
	return -1;
}

/* helper function to get policy base and policy num */
static u32 get_policy_base_and_num(enum CQM_SUP_DEVICE type, enum DIRECTION dir,
				   s32 dp_port, int alloc_flags,
				   int *tx_policy_base, int *tx_policy_num)
{
	struct cqm_bm_policy_params *info = NULL;
	u16 id;
	int policy_skipped = 0;

	for (id = 0; id < CQM_LGM_NUM_BM_POLICY; id++) {
		info = &cqm_ctrl->cqm_bm_policy_dts[id];
		if (info->policy_type == type &&
		    info->direction & dir) {
			/* SSB can only be used for 10G LAN */
			if (!((alloc_flags & DP_F_FAST_ETH_LAN) &&
			      (dp_port_link_speed_cap(dp_port) == SPEED_10000)) &&
			       id == 0) {
				policy_skipped = 1;
				continue;
			}
			*tx_policy_base = id;
			if (policy_skipped)
				*tx_policy_num =
					cqm_ctrl->cqm_bm_policy_dts[id].res_e_cnt - 1;
			else
				*tx_policy_num =
					cqm_ctrl->cqm_bm_policy_dts[id].res_e_cnt;
			return CBM_SUCCESS;
		}
	}
	return CBM_FAILURE;
}

static u8
NO_OPTIMIZE get_matching_pool(enum CQM_SUP_DEVICE type, enum DIRECTION dir)
{
	s32 policy = get_policy_from_type(type, dir);

	return get_pool_from_policy(policy);
}

static void NO_OPTIMIZE cpubuff_init(void)
{
	int i;
#if IS_ENABLED(CONFIG_LGM_CQM_CPUBUF_RET_DC)
	struct cqm_dqm_port_info *dqp_info;
	u32 deqport = 0, enqport = 0;
	struct cqm_policy_table policy = {0};
#endif

	cpubuff->policy_base = cqm_ctrl->cpu_base_policy;
	cpubuff->pool_base = cqm_ctrl->cpu_base_pool;
	cpubuff->size = 0;
	for (i = 0; i < CQM_LGM_TOTAL_POOLS; i++) {
		struct cqm_bm_pool_config *cfg;
		struct cqm_cpubuff_pool *p;

		cfg = &bm_pool_conf[i];
		if (!cfg->buf_frm_size)
			continue;

		p = &cpubuff->pool[i];
		p->start = virt_to_phys((void *)cfg->pool_start_low);
		p->size = cfg->buf_frm_num * cfg->buf_frm_size;
		p->ref_chk = kzalloc(cfg->buf_frm_num / sizeof(p->ref_chk),
				     GFP_KERNEL);
		p->ref = kvcalloc(cfg->buf_frm_num,
				  sizeof(atomic_t), GFP_KERNEL);
		cpubuff->size += p->size;
	}
#if IS_ENABLED(CONFIG_LGM_CQM_CPUBUF_RET_DC)
	policy.ingress_policy = cqm_ctrl->cpu_base_policy;
	policy.egress_policy = cqm_ctrl->cpu_base_policy;
	policy.pool = cqm_ctrl->cpu_base_pool;
	if (get_cqmport(&deqport, DQM_ACA_TYPE, DEQ_ACA, DQ_PORT) != CBM_SUCCESS) {
		dev_err(cqm_ctrl->dev, "Cannot allocate DC port for CPU Buff!\n");
		return;
	}
	if (get_cqmport(&enqport, EQM_CPU_TYPE, EQM_ACA, EQ_PORT) != CBM_SUCCESS) {
		dev_err(cqm_ctrl->dev, "Cannot allocate DC enq port!\n");
		return ;
	}
	cqm_ctrl->eqm_port_info[enqport].allocated = P_ALLOCATED;
	dqp_info = &cqm_ctrl->dqm_port_info[deqport];
	do_cqmport_config(CBM_PORT_INVALID, deqport, &policy,
			  CQM_DEFAULT_DC_ENQ_PORT_CFG,
			  CQM_DEFAULT_DC_DEQ_PORT_CFG & ~CFG_ACA_EGP_19_BPRTNDETEN_MASK,
			  DP_RXOUT_QOS, 0, 0, 0);
	dqp_info->allocated = P_ALLOCATED;

	cpubuff->num_desc = dqp_info->deq_info.num_desc;
	cpubuff->brptr_num = dqp_info->deq_info.num_free_burst;
	cpubuff->dc_free_base = cqm_ctrl->dmadesc_64_phys + CQM_DQM_DC_BUF_RTN(deqport, 0);

	dev_dbg(cqm_ctrl->dev,
		"%s CPU Buff Return DC port: %d Base Addr: %lx brptr_num = %d\n",
		__func__, deqport, (unsigned long)cpubuff->dc_free_base,
		cpubuff->brptr_num);
#endif
}

static unsigned long
NO_OPTIMIZE cqm_gen_pool_alloc(u32 ioc_f, size_t size, dma_addr_t *phy,
		   enum E_GEN_POOL_TYPE ptype, int sai)
{
	struct mxl_pool_alloc_data data = {0};
	struct gen_pool *pool = NULL;
	unsigned long vaddr, attr = 0;

	dev_info(cqm_ctrl->dev,
		 "%s is called, genpool type:%d, size:%d, ioc:%d, sai:%d\n",
		 __func__, ptype, (u32)size, ioc_f, sai);

	if (ptype == GEN_CPU_POOL) {
		pool = of_gen_pool_get(cqm_ctrl->dev->of_node, "cpupool", 0);

		/* already added by static rule */
		data.opt = MXL_FW_OPT_SKIP_HW_FWRULE;
	} else if (ptype == GEN_SYS_POOL) {
		pool = of_gen_pool_get(cqm_ctrl->dev->of_node, "syspool", 0);

		/* already added by static rule */
		data.opt = MXL_FW_OPT_SKIP_HW_FWRULE;
	} else {
		pool = of_gen_pool_get(cqm_ctrl->dev->of_node, "rwpool", 0);
	}

	if (!pool) {
		dev_err(cqm_ctrl->dev, "%s of_gen_pool_get error\n",
			__func__);
		return 0;
	}

	data.dev = cqm_ctrl->dev;
	data.sai = sai;
	data.perm = FW_READ_WRITE;
	if (!ioc_f) {
		data.opt |= MXL_FW_OPT_USE_NONCOHERENT;
		attr = DMA_ATTR_NON_CONSISTENT;
	}
	/*pool addresses are to be 64K aligned*/
	vaddr = mxl_soc_pool_alloc(pool, size, &data);

	if (vaddr) {
		dev_dbg(cqm_ctrl->dev, "Successfully allocated buf: 0x%lx\n",
			vaddr);
	} else {
		dev_err(cqm_ctrl->dev, "%s Allocate buf fails\n", __func__);
		return 0;
	}

	*phy = dma_map_single_attrs(cqm_ctrl->dev,
				    (void *)vaddr,
				    size,
				    DMA_FROM_DEVICE,
				    attr);
	if (dma_mapping_error(cqm_ctrl->dev, *phy)) {
		dev_err(cqm_ctrl->dev, "%s DMA map failed\n",
			__func__);
		return 0;
	}
	return vaddr;
}

static bool
cqm_dma_buf_alloc(struct platform_device *pdev, const struct cqm_data *pdata)
{
	unsigned long head, cpu_base_pool, voice_pool, id, offset, head_cpu = 0;
	unsigned long lrouc_pool;
	dma_addr_t *phy = &cqm_ctrl->dma_hndl_p[0];
	dma_addr_t *phy_cpu;
	u8 total = cqm_ctrl->total_pool_entries;
	int cpu_pool_ioc = cqm_ctrl->is_cpu_pool_ioc;
	struct gen_pool *fsqm_pool;
	dma_addr_t pool_phy;
	void *pool_virt = NULL;
	int idx;
	size_t bm_pool_ssb_sz = 0;
	u32 val = 0;
	int system_pool_start_idx;

	if (cpu_pool_ioc) {
		cpu_base_pool = get_matching_pool(CPU_DEV, EGRESS);
		cqm_ctrl->cpu_base_pool = cpu_base_pool;
		voice_pool = get_matching_pool(VOICE, INGRESS);
		lrouc_pool = get_matching_pool(LROUC, INGRESS);
		phy_cpu = &cqm_ctrl->dma_hndl_p[cpu_base_pool];
		if (cpu_base_pool == INVALID_POOL_ID ||
		    voice_pool == INVALID_POOL_ID) {
			dev_err(cqm_ctrl->dev, "%s: Invalid pool id\n",
				__func__);
			return false;
		}
	} else {
		cpu_base_pool = total + 1;
		voice_pool = total + 1;
		lrouc_pool = total + 1;
	}

	for (id = 0; id < total; id++) {
		cqm_ctrl->max_mem_alloc +=
			pdata->pool_ptrs[id] * pdata->pool_size[id];
		if (id >= cpu_base_pool && (id <= cpu_base_pool + 3)) {
			cqm_ctrl->max_mem_alloc_cpu +=
				pdata->pool_ptrs[id] * pdata->pool_size[id];
		} else if (id == voice_pool) {
			continue;
		} else if (id == lrouc_pool) {
			continue;
		} else {
			if (pdata->pool_type[id] != SSB_NIOC_SHARED &&
			    pdata->pool_type[id] != SSB_LROUC_NIOC_SHARED &&
			    pdata->pool_type[id] != SSB_LROUC_NIOC_ISOLATED)
				cqm_ctrl->max_mem_alloc_sys +=
					pdata->pool_ptrs[id] *
					pdata->pool_size[id];
		}
	}

	fsqm_pool = of_gen_pool_get(pdev->dev.of_node, "mxl,sram-pool", 0);

	if (fsqm_pool)
		val = (u32)gen_pool_size(fsqm_pool);
	else
		val = 0;

	bm_pool_ssb_sz = cqm_get_ssb_pool_size();
	if (bm_pool_ssb_sz > (size_t)val) {
		pr_err("mxl,sram-size %zu is smaller than BM SSB pool size %zu\n",
		       (size_t)val, bm_pool_ssb_sz);
		return -ENODEV;
	}

	for (idx = 0; idx < CQM_LGM_TOTAL_BM_POOLS; idx++)
		if (cqm_ctrl->lgm_pool_type[idx] == SSB_NIOC_SHARED ||
		    cqm_ctrl->lgm_pool_type[idx] == SSB_LROUC_NIOC_SHARED ||
		    cqm_ctrl->lgm_pool_type[idx] == SSB_LROUC_NIOC_ISOLATED) {
			size_t pool_size = (size_t)cqm_ctrl->lgm_pool_ptrs[idx] *
				cqm_ctrl->lgm_pool_size[idx];
			if (fsqm_pool)
				pool_virt = gen_pool_dma_alloc(fsqm_pool,
							       pool_size,
							       &pool_phy);
			if (!pool_virt) {
				dev_err(&pdev->dev,
					"Failed to get the requested size from pool!\n");
				return -ENODEV;
			}
			cqm_ctrl->bm_buf_base[idx] = (unsigned long)pool_virt;
			cqm_ctrl->bm_buf_phy[idx] = pool_phy;
		}

	if (!cpu_pool_ioc) {
		head = (unsigned long)
		cqm_gen_pool_alloc(0,
				   (size_t)cqm_ctrl->max_mem_alloc,
				   phy,
				   GEN_SYS_POOL, -1);

		if (!head)
			return false;
	} else {
		/*Alloc IO coherent memory for the CPU pools*/

		/*To do:Align the start address of head to 64K
		 *Adjust the 10K pool size of cpu/sys pools accordingly
		 */
		head_cpu = (unsigned long)
			cqm_gen_pool_alloc(1,
					   (size_t)cqm_ctrl->max_mem_alloc_cpu,
					   phy_cpu, GEN_CPU_POOL, -1);
		/*Alloc IO non coherent memory for the sys pools*/
		/*In B0, by default IOC memory is allocated, later remap
		 */
		head = (unsigned long)
			cqm_gen_pool_alloc(0,
					   (size_t)cqm_ctrl->max_mem_alloc_sys,
					   phy, GEN_SYS_POOL, -1);
		if (!head || !head_cpu)
			return false;
	}

	if (!cqm_get_first_system_pool_idx(&system_pool_start_idx)) {
		cqm_ctrl->bm_buf_base[system_pool_start_idx] = head;
		cqm_ctrl->bm_buf_phy[system_pool_start_idx] = *phy;
	}
	for (id = system_pool_start_idx + 1; id < total; id++) {
		offset = pdata->pool_ptrs[id - 1] * pdata->pool_size[id - 1];
		if (id == cpu_base_pool) {
			cqm_ctrl->bm_buf_base[id] = head_cpu;
			cqm_ctrl->bm_buf_phy[id] = *phy_cpu;
		} else if (id == voice_pool) {
			continue;
		} else if (id == lrouc_pool) {
			continue;
		} else {
			cqm_ctrl->bm_buf_base[id] +=
				offset + cqm_ctrl->bm_buf_base[id - 1];
			cqm_ctrl->bm_buf_phy[id] +=
				offset + cqm_ctrl->bm_buf_phy[id - 1];
		}
	}

	if (!cpu_pool_ioc)
		cqm_ctrl->cpu_base_pool = 0;
	return true;
}

static s32
NO_OPTIMIZE cqm_pool_config(const u32 pool, const struct cqm_data *pdata)
{
	struct cqm_bm_pool_config *cfg = &bm_pool_conf[pool];
	unsigned long size;
	u8 voice_pool;

	cfg->buf_frm_size = pdata->pool_size[pool];
	cfg->buf_frm_num = pdata->pool_ptrs[pool];
	size = cfg->buf_frm_size * cfg->buf_frm_num;
	voice_pool = get_matching_pool(VOICE, INGRESS);
	if (voice_pool == INVALID_POOL_ID) {
		dev_err(cqm_ctrl->dev, "voice pool not found\n");
		return CBM_FAILURE;
	}
	if (pool == voice_pool && size != pdata->voice_buf_size)
		dev_warn(cqm_ctrl->dev, "mismatch in c55, bm dts\n");

	if (pool == voice_pool && pdata->voice_buf_start) {
		cfg->pool_start_low = (unsigned long)pdata->voice_buf_start;
		cfg->pool_end_low =
			(unsigned long)(pdata->voice_buf_start + size);
		cfg->buf_frm_size_reg = get_bufsize(cfg->buf_frm_size);
	} else {
		cfg->pool_start_low = cqm_ctrl->bm_buf_base[pool];
		cfg->pool_end_low = cqm_ctrl->bm_buf_base[pool] + size;
		cfg->buf_frm_size_reg = get_bufsize(cfg->buf_frm_size);
	}
	return CBM_SUCCESS;
}

static int NO_OPTIMIZE cqm_get_dc_config(struct cbm_dc_res *dc_res, int flag)
{
	enum CQM_SUP_DEVICE type;
	struct cqm_bm_policy_params *rx_info, *tx_info;
	struct cqm_dqm_port_info *dqp_info = cqm_ctrl->dqm_port_info;
	struct cqm_eqm_port_info *eqp_info = cqm_ctrl->eqm_port_info;
	u32 cbm_port = INVALID_DQP, i, eqpd;
	u32 tx_min_guaranteed = 0;
	u16 start = CBM_PORT_INVALID;
	u32 busy_flag = 0;
	u16 end;

	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s flag: 0x%x\n", __func__, flag);

	type = get_type_from_alloc_flags(dc_res->alloc_flags);
	if (dc_res->alloc_flags & FLAG_WLAN)
		busy_flag = 1;

	if (dc_res->alloc_flags & FLAG_ACA) {
		if (get_policy_pos(type, INGRESS, &start,
				   &end, dc_res->res_id, busy_flag) < 0)
			return CBM_FAILURE;

		rx_info = &cqm_ctrl->cqm_bm_policy_dts[start];
		dc_res->rx_res.rx_bufs  =
			rx_info->pp_policy_cfg.min_guaranteed;
	}

	if (dc_res->alloc_flags &
	    (FLAG_ACA | DP_F_CPU |
	    DP_F_FAST_ETH_LAN | DP_F_FAST_ETH_WAN |
	    DP_F_GPON | DP_F_EPON)) {
		if (get_policy_pos(type, EGRESS, &start,
				   &end, dc_res->res_id, busy_flag) < 0)
			return CBM_FAILURE;

		tx_info = &cqm_ctrl->cqm_bm_policy_dts[start];
		tx_min_guaranteed = tx_info->pp_policy_cfg.min_guaranteed;
		for (i = 1; i < tx_info->res_e_cnt; i++) {
			tx_info = &cqm_ctrl->cqm_bm_policy_dts[start + i];
			tx_min_guaranteed =
				min(tx_min_guaranteed,
				    tx_info->pp_policy_cfg.min_guaranteed);
		}
		dc_res->tx_res.tx_bufs = tx_min_guaranteed;
	}

	if (dc_res->alloc_flags & FLAG_ACA) {
		cbm_port = cqm_dp_to_deqp_from_pmac_mapping_list(dc_res->dp_port);
		if (cbm_port == INVALID_DQP) {
			dev_err(cqm_ctrl->dev, "Invalid cbm port\n");
			return CBM_NOTFOUND;
		}
		dc_res->tx_res.in_deq_ring_size =
			dqp_info[cbm_port].deq_info.num_desc;
		dc_res->tx_res.out_free_ring_size =
			dqp_info[cbm_port].deq_info.num_free_burst;

		if (get_free_enqport(&eqpd, dc_res->alloc_flags) ==
			CBM_NOTFOUND) {
			dev_err(cqm_ctrl->dev, "out of enq ports\n");
			return CBM_NOTFOUND;
		}
		dc_res->rx_res.out_enq_ring_size =
			eqp_info[eqpd].eq_info.num_desc;
		dc_res->rx_res.in_alloc_ring_size =
			eqp_info[eqpd].eq_info.num_alloc_desc;
	}

	cqm_dump_cbm_dc_res(__func__, dc_res);

	return CBM_SUCCESS;
}

static s32 NO_OPTIMIZE cqm_pp_pool_setup(const struct cqm_data *pdata)
{
	unsigned long buf[CQM_LGM_TOTAL_BM_POOLS + 1] = {0};
	u32 total_pools = cqm_ctrl->total_pool_entries;
	struct cqm_bm_pool_config *cfg;
	struct pp_bmgr_pool_params pp;
	void *base = cqm_ctrl->cqm;
	unsigned long start;
	u32 buf_size = 0;
	u8 voice_pool;
	u8 pool;
	int cpu_pool_ioc = cqm_ctrl->is_cpu_pool_ioc;
	unsigned long cbm_buf_start, cbm_buf_end;
	int v2_flag = cqm_ctrl->v2_flag;

	if (cpu_pool_ioc && v2_flag)
		cbm_buf_start = cqm_ctrl->bm_buf_phy[cqm_ctrl->cpu_base_pool];
	else
		cbm_buf_start = cqm_ctrl->bm_buf_phy[0];

	cbm_buf_end = cbm_buf_start;

	voice_pool = get_matching_pool(VOICE, INGRESS);
	if (voice_pool == INVALID_POOL_ID) {
		dev_err(cqm_ctrl->dev, "Invalid voice pool\n");
		return CBM_FAILURE;
	}
	pp_bmgr_set_total_active_pools(total_pools);
	for (pool = 0; pool < total_pools; pool++) {
		cfg = &bm_pool_conf[pool];
		pp.num_buffers = cfg->buf_frm_num;
		pp.size_of_buffer = cfg->buf_frm_size;
		start = cfg->pool_start_low;
		buf_size = (pp.num_buffers) * pp.size_of_buffer;
		if (pool == voice_pool && pdata->voice_buf_size) {
			/* boot reserved mem for BM module */
			buf[pool] = start;
		} else {
			buf[pool] = cqm_ctrl->bm_buf_phy[pool];
			if ((cpu_pool_ioc && v2_flag &&
			     is_cpu_iso_pool(pool)) ||
			     ((!cpu_pool_ioc || !v2_flag) &&
			     !is_cpu_iso_pool(pool))) {
				cbm_buf_start = min(cbm_buf_start, buf[pool]);
				cbm_buf_end  = max(cbm_buf_end,
						   buf[pool] + buf_size);
			}
		}
		cbm_w32((base + CBM_POOL_START_ADDR_0 + (4 * pool)),
			((buf[pool] & GENMASK_ULL(35, 0)) >> 4));
		cbm_w32((base + (CBM_POOL_END_ADDR_0 + (4 * pool))),
			(((buf[pool] + buf_size - 1) &
			GENMASK_ULL(35, 0)) >> 4));
		pp.base_addr_low = buf[pool] & GENMASK(31, 0);
		pp.base_addr_high = buf[pool] >> 32;
		pp.flags = POOL_ENABLE_FOR_MIN_GRNT_POLICY_CALC;
		/* typically extended pools are isolated pools */
		if (bm_pool_conf[pool].buf_type == BUF_TYPE_CPU_ISOLATED ||
		    bm_pool_conf[pool].buf_type == BUF_TYPE_NIOC_ISOLATED ||
		    bm_pool_conf[pool].buf_type == BUF_TYPE_SSB_LROUC_ISOLATED)
			pp.flags |= POOL_ISOLATED;
		if (bm_pool_conf[pool].buf_type == BUF_TYPE_SSB_LROUC ||
		    bm_pool_conf[pool].buf_type == BUF_TYPE_SSB_LROUC_ISOLATED)
			pp.flags |= POOL_SSB;
		if (pool != voice_pool)
			pp_bmgr_pool_configure(&pp, &pool);
	}
	cbm_w32(base + CBM_BM_BUFFER_START_ADDR,
		(cbm_buf_start & GENMASK_ULL(35, 0)) >> 4);
	cbm_w32(base + CBM_BM_BUFFER_END_ADDR,
		((cbm_buf_end - 1) & GENMASK_ULL(35, 0)) >> 4);
	return CBM_SUCCESS;
}

static s32
NO_OPTIMIZE get_policy_pos(enum CQM_SUP_DEVICE type, const u32 dir,
			   u16 *start, u16 *end, const u32 res_id, u32 flag)
{
	struct cqm_bm_policy_params *info;
	u16 id = CBM_PORT_INVALID;
	bool found_but_busy = false;

	for (id = 0; id < CQM_LGM_NUM_BM_POLICY; id++) {
		info = &cqm_ctrl->cqm_bm_policy_dts[id];
		/* policy eligibility checking */
		if (info->policy_type != type ||
		    !(info->direction & dir) ||
		    info->res_id != res_id)
			continue;
		if (info->busy && !flag && (info->busy & dir)) {
			found_but_busy = true;
			continue;
		}

		start[0] = id;
		break;
	}
	if (unlikely(start[0] == CBM_PORT_INVALID)) {
		if (found_but_busy) {
			return CBM_BUSY;
		} else {
			dev_err(cqm_ctrl->dev, "err in getting free policy pos\n");
			return CBM_FAILURE;
		}
	}
	if (dir == EGRESS)
		end[0] = start[0] + info->res_e_cnt - 1;
	else
		end[0] = start[0] + info->res_i_cnt - 1;
	return CBM_SUCCESS;
}

static s32 NO_OPTIMIZE cqm_pp_policy_init(const u16 start, const u16 end)
{
	struct cqm_bm_policy_params *p;
	s32 ret = 0;
	u16 id;

	for (id = start; id <= end; id++) {
		p = &cqm_ctrl->cqm_bm_policy_dts[id];
		if (p->reserved)
			ret = pp_bmgr_policy_configure(&p->pp_policy_cfg, &id);

		if (ret < 0)
			return CBM_FAILURE;
	}
	return CBM_SUCCESS;
}

/* pick and setup the policy based on the dev_type, direction and
 * policy resource_id, return the policy base on success else CBM_FAULURE
 */
static s32
NO_OPTIMIZE cqm_pp_policy_setup(enum DIRECTION dir,
				enum CQM_SUP_DEVICE dev_type, u32 res)
{
	u16 i, start = CBM_PORT_INVALID;
	struct cqm_bm_policy_params *p;
	u16 end = CBM_PORT_INVALID;
	s32 ret = CBM_FAILURE;

	/* Two CQM DC ports are required for DOCSIS. They both require
	 * same policies to be returned.
	 */
	ret = get_policy_pos(dev_type, dir, &start, &end, res, 0);
	if (ret == CBM_BUSY && dev_type == DOCSIS) {
		ret = get_policy_pos(dev_type, dir, &start, &end, res, 1);
		if (ret < 0)
			return ret;
	} else if (ret < 0) {
		return ret;
	}
	/* system policy init already finished in cbm init state */
	if (is_system_pool(get_pool_from_policy(start))) {
		for (i = start; i <= end; i++) {
			p = &cqm_ctrl->cqm_bm_policy_dts[i];
			p->busy = dir;
		}
		return start;
	}

	if (cqm_pp_policy_init(start, end) < 0) {
		dev_err(cqm_ctrl->dev, "err in getting free policy!\n");
		return ret;
	}
	for (i = start; i <= end; i++) {
		p = &cqm_ctrl->cqm_bm_policy_dts[i];
		p->busy = dir;
	}
	return start;
}

static s32
NO_OPTIMIZE cqm_prepare_pool_db(struct platform_device *pdev,
				const struct cqm_data *pdata)
{
	struct cqm_bm_policy_params *db;
	struct pp_bmgr_policy_params *pp_cfg;
	u8 pool;
	unsigned long id;
	int cpu_pool_ioc = cqm_ctrl->is_cpu_pool_ioc;

	memcpy(cqm_ctrl->lgm_pool_ptrs, pdata->pool_ptrs,
	       sizeof(cqm_ctrl->lgm_pool_ptrs));
	memcpy(cqm_ctrl->lgm_pool_size, pdata->pool_size,
	       sizeof(cqm_ctrl->lgm_pool_size));
	memcpy(cqm_ctrl->lgm_pool_type, pdata->pool_type,
	       sizeof(cqm_ctrl->lgm_pool_type));
	memcpy(CPUBUFF_SZ, pdata->pool_size,
	       sizeof(CPUBUFF_SZ));

	for (id = 0; id < CQM_LGM_TOTAL_POOLS; id++)
		CPUBUFF_SHF[id] = ffs(CPUBUFF_SZ[id]) - 1;

	cqm_ctrl->total_pool_entries =
		pdata->num_sys_pools + pdata->num_bm_pools;
	cqm_ctrl->num_sys_pools = pdata->num_sys_pools;

	if (!cqm_dma_buf_alloc(pdev, pdata))
		panic("cqm dma buf alloc failed\n");

	for (pool = 0; pool < cqm_ctrl->total_pool_entries; pool++) {
		if (cqm_pool_config(pool, pdata) < 0)
			return CBM_FAILURE;
	}
	if (cpu_pool_ioc) {
		for (id = 0; id < CQM_LGM_NUM_BM_POLICY; id++) {
		/* store policy data as per index in case of non-seq order */
			db = &cqm_ctrl->cqm_bm_policy_dts[id];
			if (db->reserved == 1) {
				pp_cfg = &db->pp_policy_cfg;
				pool = pp_cfg->pools_in_policy[0].pool_id;
				if (id < cqm_ctrl->num_sys_pools ||
				    (pool >= cqm_ctrl->cpu_base_pool &&
				    (pool <= cqm_ctrl->cpu_base_pool + 3)))
					bm_pool_conf[pool].policy = id;
				if (pool == cqm_ctrl->cpu_base_pool)
					cqm_ctrl->cpu_base_policy = id;
			}
		}
	}

	/* fill buffer type to all bm pools including idle pools */
	for (pool = 0; pool < CQM_LGM_TOTAL_BM_POOLS; pool++) {
		if (bm_pool_conf[pool].buf_frm_size == 0) {
			bm_pool_conf[pool].buf_type = BUF_TYPE_INVALID;
			continue;
		}
		bm_pool_conf[pool].segment_mask = 0xFFFFFE00;
		bm_pool_conf[pool].offset_mask = 0x1ff;
		bm_pool_conf[pool].pool = pool;

		switch (cqm_ctrl->lgm_pool_type[pool]) {
		case SSB_NIOC_SHARED:
			bm_pool_conf[pool].buf_type = BUF_TYPE_SSB;
			break;
		case SSB_LROUC_NIOC_SHARED:
			bm_pool_conf[pool].buf_type = BUF_TYPE_SSB_LROUC;
			break;
		case CQM_NIOC_SHARED:
			bm_pool_conf[pool].buf_type = BUF_TYPE_NIOC_SHARED;
			break;
		case CQM_CPU_ISOLATED:
			bm_pool_conf[pool].buf_type = BUF_TYPE_CPU_ISOLATED;
			break;
		case CQM_NIOC_ISOLATED:
			bm_pool_conf[pool].buf_type = BUF_TYPE_NIOC_ISOLATED;
			break;
		case SSB_LROUC_NIOC_ISOLATED:
			bm_pool_conf[pool].buf_type = BUF_TYPE_SSB_LROUC_ISOLATED;
			break;
		default:
			bm_pool_conf[pool].buf_type = BUF_TYPE_INVALID;
			break;
		}
	}

	for (pool = 0; pool < CQM_LGM_TOTAL_BM_POOLS; pool++) {
		pr_debug("bm_pool_conf[%d]\n", pool);
		pr_debug("\t buf_type: %d\n", bm_pool_conf[pool].buf_type);
		pr_debug("\t buf_frm_size: %lu\n", bm_pool_conf[pool].buf_frm_size);
		pr_debug("\t buf_frm_num: %lu\n", bm_pool_conf[pool].buf_frm_num);
		pr_debug("\t pool_start_low: 0x%lx\n", bm_pool_conf[pool].pool_start_low);
		pr_debug("\t pool_start_high: 0x%lx\n", bm_pool_conf[pool].pool_start_high);
		pr_debug("\t pool_end_low: 0x%lx\n", bm_pool_conf[pool].pool_end_low);
		pr_debug("\t pool_end_high: 0x%lx\n", bm_pool_conf[pool].pool_end_high);
		pr_debug("\t segment_mask: 0x%lx\n", bm_pool_conf[pool].segment_mask);
		pr_debug("\t offset_mask: 0x%lx\n", bm_pool_conf[pool].offset_mask);
		pr_debug("\t pool: %lu\n", bm_pool_conf[pool].pool);
		pr_debug("\t policy: %lu\n", bm_pool_conf[pool].policy);
	}

	return CBM_SUCCESS;
}

static void NO_OPTIMIZE cqm_reset_bm_policy_dts(void)
{
	struct cqm_bm_policy_params *db;
	int idx;

	for (idx = 0; idx < CQM_LGM_NUM_BM_POLICY; idx++) {
		db = &cqm_ctrl->cqm_bm_policy_dts[idx];
		db->policy_type = MAX_SUP_DEVICE;
		db->direction = DIRECTION_INVALID;
		db->busy = 0;
		db->res_id = -1;
		db->res_count = 0;
		db->res_i_cnt = 0;
		db->res_e_cnt = 0;
	}
}

static s32 NO_OPTIMIZE cqm_prepare_policy_db(struct cqm_data *pdata)
{
	struct cqm_bm_policy_params *db;
	struct cqm_policy_params *dts;
	struct cqm_policy_res *res;
	u16 id, l_res = 0, l_count;
	int idx;

	/* Reset cqm_bm_policy_dts to default values */
	cqm_reset_bm_policy_dts();

	/* iterate only available policy pdata and keep in the cqm_ctrl */
	for (id = 0; id < CQM_LGM_NUM_BM_POLICY; id++) {
		dts = &pdata->policy_params[id];
		res = &pdata->policy_res[id];
		if (dts->min_guaranteed < MIN_POOL_SIZE)
			continue;
		l_res = res->id;
		l_count = res->count;
		/* store policy data as per index in case of non-seq order */
		if (l_res == DP_RES_ID_WAV700 && !pdata->wav700)
			continue;
		else if ((pdata->wav700) && (l_res == DP_RES_ID_WAV614 ||
					     l_res == DP_RES_ID_WAV624))
			continue;
		else if (!(pdata->lpid_wan_mode & LPID_WAN_DOCSIS) &&
			 (l_res == DP_RES_ID_DOCSIS ||
			 l_res == DP_RES_ID_DOCSIS_MMM ||
			 l_res == DP_RES_ID_DOCSIS_VOICE ||
			 l_res == DP_RES_ID_DOCSIS_MPEG))
			/* If mxl,wan-mode does not set DP_RES_ID_DOCSIS,
			 * ignore docsis policies.
			 */
			continue;
		else
			db = &cqm_ctrl->cqm_bm_policy_dts[dts->policy_id];

		db->pp_policy_cfg.num_pools_in_policy = dts->pool_count;
		if (db->pp_policy_cfg.num_pools_in_policy >
		   PP_BM_MAX_POOLS_PER_PLCY) {
			pr_err("CBM policy(%d) pools %d > PP limit (%d)\n",
			       dts->policy_id, dts->pool_count,
			       PP_BM_MAX_POOLS_PER_PLCY);
			return CBM_FAILURE;
		}
		for (idx = 0; idx < dts->pool_count; idx++) {
			db->pp_policy_cfg.pools_in_policy[idx].pool_id =
				dts->pool_id[idx];
			/* CQM BM policy includes max_allowed as max buffers
			 * for all associated pools. While in pp, max_allowed
			 * is the max buffer for each pool.
			 * For now, assign policy max allowd as max buffer for
			 * each pool
			 */
			db->pp_policy_cfg.pools_in_policy[idx].max_allowed =
				dts->pool_max_allowed[idx];
		}

		db->pp_policy_cfg.min_guaranteed = dts->min_guaranteed;
		db->pp_policy_cfg.max_allowed = dts->max_allowed;
		db->policy_type = dts->interface_type;
		db->direction = (enum DIRECTION)dts->direction;
		db->reserved = 1;
		/* policy resource id is predefined in dts */
		db->res_id = l_res;
		/* ingress has 1 policy and egress has multiple policy*/
		db->res_count = l_count;
		db->res_i_cnt = res->i_cnt;
		db->res_e_cnt = res->e_cnt;
	}
	return CBM_SUCCESS;
}

/* boot time init for all the policies matches with system pool */
static s32 cqm_sys_bm_init(const u32 num_sys_pool)
{
	u16 i;

	for (i = 0; i < CQM_LGM_NUM_BM_POLICY; i++) {
		if (!is_system_pool(get_pool_from_policy(i)) &&
		    !is_lrouc_pool(get_pool_from_policy(i)))
			continue;

		if (unlikely(cqm_pp_policy_init(i, i))) {
			dev_err(cqm_ctrl->dev, "failed sys policy: %d\n", i);
			return CBM_FAILURE;
		}
	}
	return CBM_SUCCESS;
}

static s32
NO_OPTIMIZE cqm_bm_init(struct platform_device *pdev, struct cqm_data *pdata)
{
	s32 ret = CBM_FAILURE;

	if (cqm_prepare_policy_db(pdata) < 0)
		return ret;

	if (unlikely(cqm_prepare_pool_db(pdev, pdata)))
		return ret;

	cpubuff_init();

	if (unlikely(cqm_pp_pool_setup(pdata)))
		return ret;

	if (unlikely(cqm_sys_bm_init(pdata->num_sys_pools)))
		return ret;

	return CBM_SUCCESS;
}

static int NO_OPTIMIZE cbm_hw_init(struct platform_device *pdev)
{
	init_fsqm();
	init_fsqm_desc();
	if (init_cqm_basic(pdev))
		return CBM_FAILURE;
	dev_dbg(cqm_ctrl->dev, "CBM HW init\n");
	return CBM_SUCCESS;
}

static int NO_OPTIMIZE get_buf(unsigned long *buf,
			       struct cqm_bm_pool_config *tmp_bm)
{
	u32 cpu = smp_processor_id();
	u64 buf_addr;
	u32 pool;

	if (tmp_bm->policy == CQM_FSQM_POLICY) {
		*buf = (unsigned long)cqm_fsqm_buff_alloc(cpu, CQM_FSQM_POLICY);
	} else {
		if (pp_bmgr_pop_buffer(tmp_bm->policy, &pool, &buf_addr)) {
			dev_err(cqm_ctrl->dev, "alloc from BM failed!!!\n");
			return CBM_FAILURE;
		}
		CQM_UP_STATS(cqm_dbg_cntrs[tmp_bm->policy]
				 [pool].dma_ring_buff_cnt);
		tmp_bm->pool = pool;
		*buf = buf_addr;
	}
	return CBM_SUCCESS;
}

static int
NO_OPTIMIZE setup_enq_dma_desc(int pid, u32 desc_num, unsigned long size,
			       enum CQM_BUF_TYPE buf_type,
			       u32 data_offset, int flags)
{
	struct dma_desc desc;
	unsigned long buf = 0;
	u32 reg1;
	u32 reg2;
	u32 reg4;
	int i;
	struct cqm_bm_pool_config tmp_bm = {0};
	void *dmadesc = cqm_ctrl->dmadesc_128;
	u64 desc_qw0, desc_qw1 = 0;

	get_bm_info(&tmp_bm, size, buf_type);
	for (i = 0; i < desc_num; i++) {
		cbm_dw_memset((u32 *)&desc, 0, sizeof(struct dma_desc) /
			      sizeof(u32));
		reg1 = 0;
		reg2 = 0;
		reg4 = 0;
		if (!(flags & LGM_EQM_DMA_NO_BUF)) {
			if (get_buf(&buf, &tmp_bm))
				return CBM_FAILURE;
			if (tmp_bm.policy == CQM_FSQM_POLICY)
				data_offset = 0;
			reg2 = tmp_bm.buf_frm_size - data_offset;
			/*DW2 buf_ptr_l*/
			reg1 = (u32)(buf & 0xffffffff) + data_offset;
			/*DW3 buf_ptr_h*/
			reg2 |= ((buf >> 32) & 0xf) << 23;
			reg2 |= OWN_BIT;
			/*DW3 Policy*/
			reg2 |= (tmp_bm.policy & 0xFF) << 14;
			/*DW1 src_pool*/
			reg4 |= (tmp_bm.pool & 0xF) << 18;
			/*DW1 egress flag*/
			reg4 |= (1 << 31);
			/*DW1 FCS*/
			reg4 |= (1 << 24);
			/*DW1 port No*/
			reg4 |= (3 << 4);
		} else {
			reg2 |= OWN_BIT;
		}
		desc_qw0 = ((u64)reg4) << 32;
		desc_qw1 = (((u64)reg2) << 32) +
			((unsigned long)reg1 & 0xffffffff);
		writeq(desc_qw0, dmadesc + CQM_ENQ_DMA_DESC(pid, i));
		writeq(desc_qw1, dmadesc + CQM_ENQ_DMA_DESC(pid, i) + 8);

		dev_dbg(cqm_ctrl->dev, "%s, desc_qw0:%lx, desc_qw1:%lx\n",
			__func__, (unsigned long)desc_qw0,
			(unsigned long)desc_qw1);
	}
	return CBM_SUCCESS;
}

static void
NO_OPTIMIZE init_cqm_enq_dma_port(int eqpid, int flags, unsigned long size,
				  enum CQM_BUF_TYPE buf_type)
{
	u32 config;
	/* no headroom for dma enq ports! */
	u32 head_room = 0;
	struct cqm_eqm_port_info *eqp_info;

	eqp_info = &cqm_ctrl->eqm_port_info[eqpid];
	cbm_w32(cqm_ctrl->enq + EQ_DMA_PORT(eqpid, dptr),
		eqp_info->eq_info.num_desc - 1);
	if (eqp_info->port_type == EQM_TSO)
		head_room = MIN_HEADROOM;
	setup_enq_dma_desc(eqpid, eqp_info->eq_info.num_desc,
			   size, buf_type, head_room, flags);
	config = CFG_DMA_IGP_24_EQREQ_MASK |
		 CFG_DMA_IGP_24_EQPCEN_MASK |
		 CFG_DMA_IGP_24_BP_EN_MASK;
	if (flags & LGM_EQM_DMA_HDR_ONLY)
		config |= CFG_DMA_IGP_24_H_MODE_MASK;
	if (eqp_info->port_type == EQM_RXDMA)
		config |= CFG_RXDMA_IGP_33_BUFREQ_MASK;
	cbm_w32(cqm_ctrl->enq + EQ_DMA_PORT(eqpid, cfg), config);

	CQM_DEBUG(CQM_DBG_FLAG_DMA_PORT,
		  "CQM_DMA_ENQ_%d_CFG: 0x%x\tADDR: %pS\n",
		  eqpid, cbm_r32(cqm_ctrl->enq + EQ_DMA_PORT(eqpid, cfg)),
		  cqm_ctrl->enq_phys + EQ_DMA_PORT(eqpid, cfg));
}

static void NO_OPTIMIZE setup_deq_dma_desc(u32 pid, u32 desc_num)
{
	int i;
	void *dma_desc = cqm_ctrl->dmadesc_128;

	cbm_assert(pid >= cqm_ctrl->dqm_pid_cfg.dma2_start,
		   "cbm dma dqm port id less than %d, pid:%d",
		   cqm_ctrl->dqm_pid_cfg.dma2_start, pid);

	for (i = 0; i < desc_num; i++) {
		writeq(0, dma_desc + CQM_DEQ_DMA_DESC(pid, i));
		writeq(0, dma_desc + CQM_DEQ_DMA_DESC(pid, i) + 8);
	}
}

static s32 NO_OPTIMIZE init_cqm_deq_dma_port(int dqp_idx)
{
	void *deq = cqm_ctrl->deq;
	u32 offset = 0;
	u32 port_type;
	u32 num_desc;
	u32 config;

	num_desc = cqm_ctrl->dqm_port_info[dqp_idx].deq_info.num_desc;
	port_type = find_dqm_port_type(dqp_idx);
	config = CFG_DMA_EGP_27_DQPCEN_MASK |
		 ((dqp_idx << CFG_DMA_EGP_27_EPMAP_POS) &
		 CFG_DMA_EGP_27_EPMAP_MASK);
	if (cqm_ctrl->dqm_port_info[dqp_idx].dma_port_type == DEQ_VPN) {
		config |= CFG_DMA_EGP_27_HFLAG_MASK | CFG_DMA_EGP_27_HMODE_MASK;
	} else if (cqm_ctrl->dqm_port_info[dqp_idx].dma_port_type == DEQ_PP_NF ||
		   cqm_ctrl->dqm_port_info[dqp_idx].dma_port_type == DEQ_TOE) {
		config |= CFG_DMA_EGP_27_HMODE_MASK;
	}

	switch (port_type) {
	case DQM_DMA_TYPE:
		config |= CFG_DMA_EGP_27_DQRDPTRDIS_MASK;
		offset = DQ_DMA_PORT(dqp_idx, cfg);
		setup_deq_dma_desc(dqp_idx, num_desc);
		cbm_w32((deq + offset), config);
		config &= ~CFG_DMA_EGP_27_DQRDPTRDIS_MASK;
		config |= CFG_DMA_EGP_27_DQREQ_MASK;
		cbm_w32((deq + offset), config);
		break;
	case DQM_PON_TYPE:
	case DQM_DOCSIS_TYPE:
		offset = DQ_PON_PORT(dqp_idx, cfg);
		config |= CFG_PON_EGP_75_BUFRTN_MASK |
			  CFG_PON_EGP_75_DQREQ_MASK |
			  CFG_PON_EGP_75_BFBPEN_MASK;
		if (dqp_idx == CQM_PON_IP_PORT_START &&
		    !(cbm_r32(cqm_ctrl->pib + PIB_CTRL) &
		      PIB_CTRL_PIB_DC_MODE_MASK))
			setup_deq_dma_desc(dqp_idx, num_desc);
		cbm_w32((deq + offset), config);
		break;
	default:
		return CBM_FAILURE;
	};

	CQM_DEBUG(CQM_DBG_FLAG_DMA_PORT, "CQM_DQ_%d_CFG: 0x%x\tADDR: %pS\n",
		  dqp_idx, cbm_r32(deq + offset), cqm_ctrl->deq_phys + offset);
	return CBM_SUCCESS;
}

static s32
NO_OPTIMIZE cqm_dma_port_enable(s32 port_id, u32 flags, unsigned long size,
				enum CQM_BUF_TYPE buf_type)
{
	int port_type = find_dqm_port_type(port_id);

	CQM_DEBUG(CQM_DBG_FLAG_DMA_PORT, "%s deq_port:%d, flags:0x%x",
		  __func__, port_id, flags);
	if (flags & CBM_PORT_F_DEQUEUE_PORT) {
		/*DMA dequeue port*/
		if (flags & CBM_PORT_F_DISABLE) {
			if (port_type != DQM_PON_TYPE && port_type != DQM_DOCSIS_TYPE) {
				set_val((cqm_ctrl->deq
					+ DQ_DMA_PORT((port_id), cfg)),
					0, 0x1, 0);
				cbm_w32((cqm_ctrl->deq
					+ DQ_DMA_PORT(port_id, dqpc)),
					0);
			} else {
				cbm_w32((cqm_ctrl->deq
					+ DQ_PON_PORT(port_id, cfg)), 0);
			}
		} else {
			init_cqm_deq_dma_port(port_id);
		}
	} else {
		/*DMA Enqueue port*/
		if (flags & CBM_PORT_F_DISABLE)
			cbm_w32((cqm_ctrl->enq +
				 EQ_DMA_PORT(port_id, cfg)), 0);
		else
			init_cqm_enq_dma_port(port_id, flags, size, buf_type);
	}
	return CBM_SUCCESS;
}

static void NO_OPTIMIZE init_cqm_deq_cpu_port(int idx, u32 tx_ring_size)
{
	u32 config = CFG_CPU_EGP_0_DQREQ_MASK |
		     CFG_CPU_EGP_0_BUFRTN_MASK |
		     CFG_CPU_EGP_0_BFBPEN_MASK |
		     CFG_CPU_EGP_0_DQPCEN_MASK;
	void *deq = cqm_ctrl->deq;

	if (tx_ring_size)
		cbm_w32((deq + DQ_CPU_PORT(idx, dptr)), tx_ring_size - 1);

	/*!< preserve complete data changes */
	config |= cbm_r32(deq + DQ_CPU_PORT(idx, cfg));
	config |= ((idx << CFG_CPU_EGP_0_EPMAP_POS) & CFG_CPU_EGP_0_EPMAP_MASK);
	dev_dbg(cqm_ctrl->dev, "%d\n", idx);
	cbm_w32((deq + DQ_CPU_PORT(idx, cfg)), config);

	/* Only required for VM CPU ports */
	if (idx >= cqm_ctrl->dqm_pid_cfg.vm_start &&
	    idx <= cqm_ctrl->dqm_pid_cfg.vm_end) {
		cbm_w32((deq + DQ_CPU_PORT(idx, irnen)), BIT(1));
		CQM_DEBUG(CQM_DBG_FLAG_RX_DP, "%s(%d): write: reg: 0x%pS val: 0x%lx\n",
			  __func__, __LINE__, cqm_ctrl->deq_phys + DQ_CPU_PORT(idx, irnen), BIT(1));
	}

	CQM_DEBUG(CQM_DBG_FLAG_CPU_PORT,
		  "CQM_CPU_DEQ_%d_CFG 0x%x, ADDR:%pS\n",
		  idx, cbm_r32(deq + DQ_CPU_PORT(idx, cfg)),
		  cqm_ctrl->deq_phys + DQ_CPU_PORT(idx, cfg));
}

static void init_cqm_enq_cpu_qbyp(int port)
{
	struct cqm_dqm_port_info *info = NULL;
	void *enq = cqm_ctrl->enq;
	u32 dqm_dmaq;
	u32 dqp_qbp;
	u32 psbkeep_cfg = 0;

	if (get_cqmport(&dqp_qbp, DQM_DMA_TYPE, DEQ_CPU_QOSBP, DQ_PORT) < 0) {
		dev_err(cqm_ctrl->dev, "config: err getting free dqp_qbp\n");
		return;
	}
	dqm_dmaq = dqp_qbp - cqm_ctrl->dqm_pid_cfg.qosbp_start;
	if (dqm_dmaq > 7) {
		dev_err(cqm_ctrl->dev, "Invalid DMAQ %d\n", dqm_dmaq);
		return;
	}
	info = &cqm_ctrl->dqm_port_info[dqp_qbp];
	info->allocated = P_ALLOCATED;

	if (cqm_ctrl->v2_flag)
		psbkeep_cfg = CFG_CPU_IGP_0_PSBKEEP_MASK;

	cbm_w32((enq + EQ_CPU_PORT(port, cfg)),
		CFG_CPU_IGP_0_EQREQ_MASK |
		CFG_CPU_IGP_0_BUFREQ0_MASK |
		CFG_CPU_IGP_0_BUFREQ1_MASK |
		CFG_CPU_IGP_0_EQPCEN_MASK |
		CFG_CPU_IGP_0_BP_EN_MASK |
		CFG_CPU_IGP_0_BUFREQ2_MASK |
		CFG_CPU_IGP_0_BUFREQ3_MASK |
		CFG_CPU_IGP_0_BUFREQ4_MASK |
		CFG_CPU_IGP_0_QOSBYPSEN_MASK |
		psbkeep_cfg |
		dqm_dmaq << CFG_CPU_IGP_0_DQMDMAQ_POS);
	CQM_DEBUG(CQM_DBG_FLAG_CPU_PORT,
		  "CQM_CPU_ENQ_%d_CFG: 0x%x ADDR:%pS\n",
		  port, cbm_r32(enq + EQ_CPU_PORT(port, cfg)),
		  cqm_ctrl->enq_phys + EQ_CPU_PORT(port, cfg));
}

static void init_cqm_enq_cpu_port(u32 idx, u32 custom_cfg)
{
	void *enq = cqm_ctrl->enq;
	u32 default_cfg = CFG_CPU_IGP_0_EQREQ_MASK |
			  CFG_CPU_IGP_0_BUFREQ0_MASK |
			  CFG_CPU_IGP_0_BUFREQ1_MASK |
			  CFG_CPU_IGP_0_EQPCEN_MASK |
			  CFG_CPU_IGP_0_BP_EN_MASK |
			  CFG_CPU_IGP_0_BUFREQ2_MASK |
			  CFG_CPU_IGP_0_BUFREQ3_MASK |
			  CFG_CPU_IGP_0_BUFREQ4_MASK;
	if (cqm_ctrl->v2_flag)
		default_cfg |= CFG_CPU_IGP_0_PSBKEEP_MASK;

	if (custom_cfg > 0)
		default_cfg = custom_cfg;

	cbm_w32((enq + EQ_CPU_PORT(idx, cfg)), default_cfg);

	CQM_DEBUG(CQM_DBG_FLAG_CPU_PORT,
		  "CQM_CPU_ENQ_%d_CFG: 0x%x\tADDR: %pS\n",
		  idx, cbm_r32(enq + EQ_CPU_PORT(idx, cfg)),
		  cqm_ctrl->enq_phys + EQ_CPU_PORT(idx, cfg));
}

static u32 get_bm_policy_from_cfg_reg(u32 enqport)
{
	u32 info = cbm_r32(cqm_ctrl->enq + EQ_DC_PORT(enqport, cfg));

	return (info & CFG_DC_IGP_16_BASEPOLICY_MASK) >>
		CFG_DC_IGP_16_BASEPOLICY_POS;
}

static void
dc_buff_free(u32 dc_dq_pid, u64 *dc_buf_ptr, u32 idx)
{
	void *base = cqm_get_dmadesc_64();

	dev_dbg(cqm_ctrl->dev,
		"%s port: %u idx: %u dc_buf_ptr: 0x%llx\n",
		__func__, dc_dq_pid, idx, dc_buf_ptr[0]);
	writeq(dc_buf_ptr[0], (base + CQM_DQM_DC_BUF_RTN(dc_dq_pid, idx)));
}

/* !< it used to dummy request flush buffer fifo for aca port */
static s32 NO_OPTIMIZE dc_buffer_dummy_request(u32 enqport, u32 deqport)
{
	u64 dc_buf_ptr;
	u32 desc_idx;
	u32 desc_ring_size =
		cbm_r32(cqm_ctrl->enq + EQ_DC_PORT(enqport, dptr));

	CQM_DEBUG(CQM_DBG_FLAG_API,
		  "%s enqport: %u deqport: %u\n",
		  __func__, enqport, deqport);

	desc_ring_size &= DPTR_DC_IGP_16_ND_MASK;
	for (desc_idx = 0; desc_idx <= desc_ring_size; desc_idx++) {
		dc_buf_ptr = readq(cqm_ctrl->dmadesc_64 +
				   CQM_EQM_DC_BUF_ALLOC(enqport, desc_idx));
		dev_dbg(cqm_ctrl->dev, "%s ptr:%lx, idx:%d, addr:%x,\n",
			__func__, (unsigned long)dc_buf_ptr,
			desc_idx, CQM_EQM_DC_BUF_ALLOC(enqport, desc_idx));
	}

	return CBM_SUCCESS;
}

static void NO_OPTIMIZE init_fsqm1_dma_deq(const u32 id)
{
	struct cqm_dqm_port_info *info = &cqm_ctrl->dqm_port_info[id];
	void *deq = cqm_ctrl->deq;
	u32 fsqmq = 0;

	setup_deq_dma_desc(id, info->deq_info.num_desc);
	fsqmq = (id < LGM_FSQMQ_DMAT1_PORT) ?
		(id - LGM_FSQMQ_DMAT2_PORT) :
		(id - LGM_FSQMQ_DMAT1_PORT + 4);
	fsqmq = (fsqmq << CFG_DMA_EGP_27_FSQMQ_POS) & CFG_DMA_EGP_27_FSQMQ_MASK;
	cbm_w32(deq + DQ_DMA_PORT(id, cfg),
		CFG_DMA_EGP_27_DQREQ_MASK |
		CFG_DMA_EGP_27_QBYPSEN_MASK |
		CFG_DMA_EGP_27_HMODE_MASK |
		CFG_DMA_EGP_27_FSQMQEN_MASK |
		CFG_DMA_EGP_27_DQPCEN_MASK |
		fsqmq);
	if (!cqm_dq_dma_chan_init(id, 0))
		info->dma_ch_in_use = true;
	dev_dbg(cqm_ctrl->dev, "FSQM1_DMADQ_%d_CFG: 0x%x\tADDR: %pS\n",
		id, cbm_r32(deq + DQ_DMA_PORT(id, cfg)),
		cqm_ctrl->deq_phys + DQ_DMA_PORT(id, cfg));
}

static void init_qos_bypass_dma_deq(const u32 index)
{
	void *deq = cqm_ctrl->deq;

	setup_deq_dma_desc(index,
			   cqm_ctrl->dqm_port_info[index].deq_info.num_desc);
	cbm_w32(deq + DQ_DMA_PORT(index, cfg),
		CFG_DMA_EGP_27_DQREQ_MASK |
		CFG_DMA_EGP_27_QBYPSEN_MASK |
		CFG_DMA_EGP_27_DQPCEN_MASK |
		CFG_DMA_EGP_27_EGMODE_MASK |
		CFG_DMA_EGP_27_EGFLAG_MASK |
		CFG_DMA_EGP_27_HMODE_MASK |
		CFG_DMA_EGP_27_HFLAG_MASK |
		CFG_DMA_EGP_27_DESCCONVDIS_MASK);

	CQM_DEBUG(CQM_DBG_FLAG_DMA_PORT,
		  "QOSBYPASS_DMADQ_%d_CFG: 0x%x\tADDR: %pS\n",
		  index, cbm_r32(deq + DQ_DMA_PORT(index, cfg)),
		  cqm_ctrl->deq_phys + DQ_DMA_PORT(index, cfg));
}

static void init_docsis_dma_deq(const u32 index)
{
	void *deq = cqm_ctrl->deq;

	setup_deq_dma_desc(index,
			   cqm_ctrl->dqm_port_info[index].deq_info.num_desc);
	cbm_w32(deq + DQ_DMA_PORT(index, cfg),
		CFG_DMA_EGP_27_DQREQ_MASK |
		CFG_DMA_EGP_27_QBYPSEN_MASK |
		CFG_DMA_EGP_27_DQPCEN_MASK /*|
		CFG_DMA_EGP_27_HMODE_MASK |
		CFG_DMA_EGP_27_HFLAG_MASK*/);

	CQM_DEBUG(CQM_DBG_FLAG_DMA_PORT,
		  "QOSBYPASS_DMADQ_%d_CFG: 0x%x\tADDR: %pS\n",
		  index, cbm_r32(deq + DQ_DMA_PORT(index, cfg)),
		  cqm_ctrl->deq_phys + DQ_DMA_PORT(index, cfg));
}

static struct dma_chan *setup_DMA_channel(int chan, dma_addr_t desc_base,
					  int desc_num)
{
	char name[DMA_CH_STR_LEN];
	struct dma_chan *pch;
	const char *engine;
	u8 ctrl;
	u16 ch;

	dp_dma_parse_id(chan, &ctrl, NULL, &ch);
	engine = cqm_get_dma_engine_name((u32)ctrl);
	if (!engine)
		return NULL;

	CQM_DEBUG(CQM_DBG_FLAG_DMA_CTRL,
		  "%s dma_id:0x%x, chid:%u, cid:%u, name:%s\n",
		  __func__, chan, ch, ctrl, engine);
	snprintf(name, DMA_CH_STR_LEN, "%sC%d", engine, ch);

	pch = dma_request_chan(cqm_ctrl->dev, name);
	if (!pch) {
		dev_err(cqm_ctrl->dev, "%s failed to open chan for %s\r\n",
			__func__, name);
		return NULL;
	}

	/* For DMA version greater than DMA_VER22, the last two parameters
	 * of dmaengine_prep_slave_single are ignored.
	 */
	if ((dmaengine_prep_slave_single(pch, desc_base, desc_num,
					 DMA_DEV_TO_MEM, 0)) < 0) {
		dev_err(cqm_ctrl->dev, "%s failed to setup chan desc for %s\r\n",
			__func__, name);
		return NULL;
	}
	/* Channel on */
	dma_async_issue_pending(pch);
	dev_dbg(cqm_ctrl->dev, "%s executed\n", __func__);
	return pch;
}

static s32 NO_OPTIMIZE cqm_enqueue_dma_port_init(s32 cqm_port_id,
						 s32 dma_hw_num,
						 u32 chan_num, u32 flags)
{
	int dma_controller, chan;
	unsigned char dma_ctrl[DMA_CH_STR_LEN];
	dma_addr_t desc_base;
	struct cqm_eqm_port_info *p_info;

	p_info = &cqm_ctrl->eqm_port_info[cqm_port_id];
	desc_base = (dma_addr_t)cqm_ctrl->dmadesc_128_phys +
		     CQM_ENQ_DMA_DESC(cqm_port_id, 0);
		/*config the dma channel*/
	snprintf(dma_ctrl, DMA_CH_STR_LEN, "DMA%dRX", dma_hw_num);
	dma_controller = cqm_dma_get_controller(dma_ctrl);
	chan = dp_dma_set_id(dma_controller, 0, chan_num);

	p_info->eq_info.dma_rx_chan_std = chan;
	CQM_DEBUG(CQM_DBG_FLAG_DMA_PORT,
		  "%s port: %d flags: 0x%x dma_hw_num: %d chan: 0x%x \r\n",
		  __func__, cqm_port_id, flags, dma_hw_num, chan);
	snprintf(p_info->dma_chan_str, DMA_CH_STR_LEN, "port%d", cqm_port_id);
	p_info->pch = setup_DMA_channel(chan, desc_base,
					p_info->eq_info.num_desc);
	if (!p_info->pch) {
		dev_err(cqm_ctrl->dev, "%s failed to setup chan for 0x%x\r\n",
			__func__, chan);
		return CBM_FAILURE;
	}
	return CBM_SUCCESS;
}

static s32 NO_OPTIMIZE cqm_dequeue_dma_port_uninit(s32 cqm_port_id, u32 flags)
{
	struct dma_chan *chan;

	chan = cqm_ctrl->dqm_port_info[cqm_port_id].pch;
	if (chan && cqm_ctrl->dqm_port_info[cqm_port_id].dma_ch_in_use) {
		dmaengine_pause(chan);
		dma_release_channel(chan);
	}
	return CBM_SUCCESS;
}

static s32 NO_OPTIMIZE cqm_dq_dma_chan_init(s32 cqm_port_id, u32 flags)
{
	u8 dma_ctrl[DMA_CH_STR_LEN];
	s32 ret = CBM_SUCCESS;
	dma_addr_t desc_base;
	struct cqm_dqm_port_info *p_info;
	u32 chan_num;
	u32 dma_controller;
	s32 dma_hw_num;
	s32 chan;

	if (cqm_port_id < CQM_PON_IP_PORT_START) {
		desc_base = (dma_addr_t)cqm_ctrl->dmadesc_128_phys +
			CQM_DEQ_DMA_DESC(cqm_port_id, 0);
	} else {
		/* We have only one DMA for 64 PON ports */
		cqm_port_id = CQM_PON_IP_PORT_START;
		desc_base = (dma_addr_t)cqm_ctrl->dmadesc_128_phys +
			CQM_DEQ_PON_DMA_DESC(cqm_port_id, 0);
	}

	/*config the dma channel*/
	p_info = &cqm_ctrl->dqm_port_info[cqm_port_id];

	dma_hw_num = p_info->dma_dt_ctrl;
	chan_num = p_info->deq_info.dma_tx_chan;
	snprintf(dma_ctrl, DMA_CH_STR_LEN, "DMA%dTX", dma_hw_num);
	dma_controller = cqm_dma_get_controller(dma_ctrl);
	chan = dp_dma_set_id(dma_controller, 0, chan_num);
	CQM_DEBUG(CQM_DBG_FLAG_DMA_CTRL, "%s chan:0x%x\n", __func__, chan);

	snprintf(p_info->dma_chan_str, DMA_CH_STR_LEN, "port%d", cqm_port_id);
	p_info->pch = setup_DMA_channel(chan, desc_base,
					p_info->deq_info.num_desc);
	if (!p_info->pch)
		return CBM_FAILURE;
	return ret;
}

static int NO_OPTIMIZE conf_deq_cpu_port(const struct dqm_cpu_port *cpu_ptr)
{
	u32 port = 0;
	u32 flags = 0;
	u32 j = 0;
	u32 range = 0;
	struct cqm_pmac_port_map local_entry = {0};
	struct cqm_dqm_port_info *p_info;

	port = cpu_ptr->port;
	range = cpu_ptr->port_range;

	while (j <= range) {
		p_info = &cqm_ctrl->dqm_port_info[port];
		memset(&local_entry, 0, sizeof(local_entry));
		p_info->deq_info.cbm_dq_port_base =
			cqm_ctrl->deq_phys + DQ_CPU_PORT(port, desc[0].desc0);
		p_info->deq_info.num_desc = cpu_ptr->num_desc;
		p_info->deq_info.port_no = port;
		p_info->deq_info.dma_tx_chan = 255;
		p_info->cbm_buf_free_base =
			cqm_ctrl->deq + DQ_CPU_PORT(port, ptr_rtn_dw2d0);
		p_info->num_free_entries =
			(port >= cqm_ctrl->dqm_pid_cfg.voice_start) ? 4 : 1;
		p_info->cpu_port_type = cpu_ptr->cpu_port_type;
		p_info->dq_txpush_num = cpu_ptr->txpush_desc;
		p_info->valid = 1;

		/*config cbm/dma port*/
		if (port >= cqm_ctrl->dqm_pid_cfg.lro_start &&
		    port <= cqm_ctrl->dqm_pid_cfg.lro_end) {
			flags = DP_F_LRO;
			goto ASSIGN_FLAGS;
		} else if ((port >= cqm_ctrl->dqm_pid_cfg.vm_start) &&
			   (port <= cqm_ctrl->dqm_pid_cfg.vm_end)) {
			flags = DP_F_DIRECT;
			goto ASSIGN_FLAGS;
		} else if ((port >= cqm_ctrl->dqm_pid_cfg.voice_start) &&
			   (port <= cqm_ctrl->dqm_pid_cfg.voice_end)) {
			flags = 0;
			goto ASSIGN_FLAGS;
		}
		cqm_ctrl->dqm_port_info[port].allocated = P_ALLOCATED;
		local_entry.egp_port_map[0] |= BIT(port);
		local_entry.owner = 0;
		local_entry.dev = 0;
		local_entry.dev_port = 0;
		local_entry.flags = P_ALLOCATED;
		cqm_add_to_list(&local_entry);
ASSIGN_FLAGS:
		p_info->egp_type = flags;
		j++;
		port++;
	}
	cpu_ptr = NULL;
	return CBM_SUCCESS;
}

static int NO_OPTIMIZE conf_deq_aca_port(const struct dqm_aca_port *aca_ptr)
{
	u32 port = 0;
	u32 port_end = 0;
	struct cqm_dqm_port_info *dqp_info;

	port = aca_ptr->port;
	port_end = port + aca_ptr->port_range;
	while (port <= port_end) {
		dqp_info = &cqm_ctrl->dqm_port_info[port];
		dqp_info->deq_info.cbm_dq_port_base =
			cqm_ctrl->dmadesc_64_phys + CQM_DQM_DC_DESC(port, 0);
		dqp_info->deq_info.num_desc = aca_ptr->num_desc;
		dqp_info->deq_info.port_no = port;
		dqp_info->deq_info.dma_tx_chan = 255;
		dqp_info->cbm_buf_free_base =
			cqm_ctrl->dmadesc_64_phys + CQM_DQM_DC_BUF_RTN(port, 0);
		dqp_info->num_free_entries = 32;
		dqp_info->deq_info.num_free_burst = aca_ptr->num_free_burst;
		dqp_info->cpu_port_type = aca_ptr->cpu_port_type;
		dqp_info->dq_txpush_num = aca_ptr->txpush_desc;
		dqp_info->valid = 1;
		dqp_info->egp_type = DP_F_ACA;
		port++;
	}
	aca_ptr = NULL;
	return CBM_SUCCESS;
}

static int NO_OPTIMIZE conf_pp_nf_port(const struct dqm_dma_port *dma_ptr)
{
	struct cqm_dqm_port_info *dqp_info;
	u16 port_num;
	u8 idx = 0;

	for (idx = 0; idx <= dma_ptr->port_range; idx++) {
		port_num = dma_ptr->port + idx;
		dqp_info = &cqm_ctrl->dqm_port_info[port_num];
		dqp_info->deq_info.port_no = port_num;
		dqp_info->deq_info.num_desc = dma_ptr->num_desc;
		dqp_info->dma_port_type = dma_ptr->port_type;
		dqp_info->egp_type = DP_F_PP_NF;
	}
	return CBM_SUCCESS;
}

static int NO_OPTIMIZE conf_deq_dma_port(const struct dqm_dma_port *dma_ptr)
{
	struct cqm_dqm_port_info *p_info;
	u32 offset = 0;
	u32 range = 0;
	u32 flags = 0;
	u32 index = 0;
	u32 port = 0;
	u32 j = 0;

	port = dma_ptr->port;
	range = dma_ptr->port_range;
	while (j <= range) {
		index = port + j;
		dev_info(cqm_ctrl->dev, "cqm_deq_port: %d\n", index);
		p_info = &cqm_ctrl->dqm_port_info[index];
		if (index < CQM_PON_IP_PORT_START) {
			offset = CQM_DEQ_DMA_DESC(index, 0);
		} else {
			offset = CQM_DEQ_PON_DMA_DESC(index, 0);
			range = 0; /*only one PON DMA*/
		}

		p_info->deq_info.cbm_dq_port_base =
			cqm_ctrl->dmadesc_128_phys + offset;
		p_info->deq_info.num_desc = dma_ptr->num_desc;
		p_info->deq_info.port_no = index;
		p_info->dma_port_type = dma_ptr->port_type;
		p_info->deq_info.dma_tx_chan = dma_ptr->dma_chan + j;
		p_info->cbm_buf_free_base = NULL;
		p_info->num_free_entries = 0;
		p_info->dq_txpush_num = dma_ptr->txpush_desc;
		if (!get_matching_flag(&flags, index))
			p_info->egp_type = flags;
		p_info->dma_dt_ch = dma_ptr->dma_chan + j;
		p_info->dma_dt_ctrl = dma_ptr->dma_ctrl;
		p_info->dma_dt_init_type = DEQ_DMA_CHNL;
		p_info->dma_ch_in_use = 0;
		p_info->valid = 1;

		if (dma_ptr->port_type == DEQ_FSQM_DESC)
			init_fsqm1_dma_deq(index);
		if (dma_ptr->port_type == DEQ_CPU_QOSBP) {
			init_qos_bypass_dma_deq(index);
			if (!cqm_dq_dma_chan_init(index,
						  p_info->dma_dt_init_type)) {
				p_info->dma_ch_in_use = true;
			} else {
				pr_err("err in DMA channel init: %d\n", index);
			}
		}
		if (cqm_ctrl->lpid_config.cqm_lpid_wan_mode & LPID_WAN_DOCSIS) {
			if (dma_ptr->port_type == DEQ_DC_DOCSIS) {
				init_docsis_dma_deq(index);
				if (!cqm_dq_dma_chan_init(index,
							  p_info->dma_dt_init_type)) {
					p_info->dma_ch_in_use = true;
				} else {
					pr_err("err in DMA channel init: %d\n", index);
				}
			}
			if (index >= CQM_PON_IP_PORT_START)
				p_info->dma_dt_init_type = DEQ_DMA_CHNL_NOT_APPL;
		}
		j++;
	}
	return CBM_SUCCESS;
}

int port_idx_to_buf_size(int idx)
{
	int buf_size = 0;

	switch (idx) {
	case 0:
		buf_size = fsqm_pool_conf[0].buf_frm_size;
		break;
	case 1:
		buf_size = 512;
		break;
	case 2:
		buf_size = 1024;
		break;
	case 3:
		buf_size = 2048;
		break;
	case 4:
		buf_size = 10240;
		break;
	default:
		pr_err("Unable to find buf_size for index %d\n", idx);
		buf_size = IOC_2K_POLICY_BUF_SIZE;
		break;
	}
	return buf_size;
}

static int NO_OPTIMIZE conf_enq_dma_port(const struct eqm_dma_port *eqp_conf)
{
	u32 idx = 0;
	u32 pid = 0;
	u32 port_start = eqp_conf->port;
	u32 port_range = eqp_conf->port_range;
	struct cqm_eqm_port_info *eqp_info;

	for (idx = 0; idx <= port_range; idx++) {
		pid = port_start + idx;
		eqp_info = &cqm_ctrl->eqm_port_info[pid];
		memset(eqp_info, 0, sizeof(struct cqm_eqm_port_info));
		eqp_info->num_eq_ports = 1;
		eqp_info->eq_info.cbm_eq_port_base =
			cqm_ctrl->dmadesc_128_phys + CQM_ENQ_DMA_DESC(pid, 0);
		eqp_info->eq_info.port_no = pid;
		eqp_info->port_type = eqp_conf->port_type;
		eqp_info->eq_info.num_desc = eqp_conf->num_desc;
		eqp_info->dma_dt_ch = eqp_conf->dma_chnl + idx;
		eqp_info->dma_dt_ctrl = eqp_conf->dma_ctrl;
		eqp_info->valid = 1;
		switch (eqp_info->port_type) {
		/*EQM_DMA_HEADER port is not used.Disable unused ports*/
		case EQM_RXDMA:
			eqp_info->buf_size = fsqm_pool_conf[0].buf_frm_size;
			eqp_info->buf_type = BUF_TYPE_FSQM;
			break;
		case EQM_DMA_PON:
			eqp_info->dma_dt_init_type = ENQ_DMA_FSQM_CHNL + idx;
			/* SSB_POOL:use buf_type & buf_size concurrently
			 * idx = 0 buf_type = REQ_FSQM_BUF, buf_size can be
			 * ignored here
			 * idx = 1 buf_type = REQ_BM_BUF, buf_size = 512
			 * idx = 2 buf_type = REQ_BM_BUF, buf_size = 2048
			 * idx = 3, skip
			 * idx  4, buf_type = REQ_BM_buf, buf_size = 10240
			 */
			eqp_info->buf_size = port_idx_to_buf_size(idx);
			if (eqp_info->buf_size ==
			    fsqm_pool_conf[0].buf_frm_size)
				eqp_info->buf_type = BUF_TYPE_FSQM;
			else
				eqp_info->buf_type = BUF_TYPE_NIOC_SHARED;
			if (pid == 27)
				continue;
			if (pid == 26)
				eqp_info->buf_size  = IOC_2K_POLICY_BUF_SIZE;
			break;
		/* prepare this port for size2 dma enq */
		case EQM_DMA_SIZE2:
			eqp_info->buf_size = IOC_2K_POLICY_BUF_SIZE;
			eqp_info->buf_type = BUF_TYPE_NIOC_SHARED;
			break;
		default:
			break;
		};
	}
	return CBM_SUCCESS;
}

static int NO_OPTIMIZE conf_enq_cpu_port(const struct eqm_cpu_port *cpu_ptr)
{
	u32 port_start = 0;
	int idx = 0;
	u32 pid = 0;
	u32 port_range = 0;
	struct cqm_eqm_port_info *p_info;

	port_start = cpu_ptr->port;
	port_range = cpu_ptr->port_range;
	for (idx = 0; idx <= port_range; idx++) {
		pid = port_start + idx;
		p_info = &cqm_ctrl->eqm_port_info[pid];
		p_info->num_eq_ports = 1;
		p_info->eq_info.cbm_eq_port_base =
			cqm_ctrl->enq_phys + EQ_CPU_PORT(pid, desc0.desc0);
		p_info->eq_info.num_alloc_desc = cpu_ptr->num_alloc_burst;
		p_info->eq_info.port_no = pid;
		p_info->port_type = cpu_ptr->port_type;
		p_info->eq_info.num_desc = cpu_ptr->num_desc;
		p_info->valid = 1;

		switch (cpu_ptr->port_type) {
		case EQM_CPU:
			init_cqm_enq_cpu_port(pid, 0);
		break;
		case EQM_CPU_QBYP:
			/* using Voice enq port for CPU QOS bypass enq */
			init_cqm_enq_cpu_qbyp(pid);
		break;
		case EQM_VOICE:
			dev_dbg(cqm_ctrl->dev, "pid: %d range: %d\n",
				pid, port_range);
		break;
		case EQM_ACA:
			dev_dbg(cqm_ctrl->dev, "pid: %d range: %d\n",
				pid, port_range);
		/*!< place holder for aca ports */
		break;
		default:
			dev_dbg(cqm_ctrl->dev, "unknown port! %d\n", pid);
		break;
		}
	}
	return CBM_SUCCESS;
}

static void set_egp_lu_tbl(void)
{
	u32 i;

	for (i = 0; i < ARRAY_SIZE(epg_lookup_table); i++) {
		if (epg_lookup_table[i].port_type == FLAG_ACA ||
		    epg_lookup_table[i].port_type == FLAG_DOCSIS) {
			epg_lookup_table[i].epg =
				cqm_ctrl->dqm_pid_cfg.aca_start;
			epg_lookup_table[i].epg_range =
				cqm_ctrl->dqm_pid_cfg.aca_end -
				cqm_ctrl->dqm_pid_cfg.aca_start;
			epg_lookup_table[i].ipg_range =
				cqm_ctrl->eqm_pid_cfg.aca_end -
				cqm_ctrl->eqm_pid_cfg.aca_start;
			epg_lookup_table[i].ipg =
				cqm_ctrl->eqm_pid_cfg.aca_start;
			dev_dbg(cqm_ctrl->dev,
				"FLAG_ACA: epg:%d range:%d, igp:%d, range:%d\n",
				epg_lookup_table[i].epg,
				epg_lookup_table[i].epg_range,
				epg_lookup_table[i].ipg,
				epg_lookup_table[i].ipg_range);
		}
		if (epg_lookup_table[i].port_type == DP_F_VUNI) {
			epg_lookup_table[i].epg =
				cqm_ctrl->dqm_pid_cfg.vuni_start;
			epg_lookup_table[i].epg_range =
				cqm_ctrl->dqm_pid_cfg.vuni_end -
				cqm_ctrl->dqm_pid_cfg.vuni_start;
			dev_dbg(cqm_ctrl->dev,
				"DP_F_VUNI: epg:%d range:%d, igp:%d, range:%d\n",
				epg_lookup_table[i].epg,
				epg_lookup_table[i].epg_range,
				epg_lookup_table[i].ipg,
				epg_lookup_table[i].ipg_range);
		}
		if (epg_lookup_table[i].port_type == FLAG_DIRECTPATH) {
			epg_lookup_table[i].epg =
				cqm_ctrl->dqm_pid_cfg.vm_start;
			epg_lookup_table[i].epg_range =
				cqm_ctrl->dqm_pid_cfg.vm_end -
				cqm_ctrl->dqm_pid_cfg.vm_start;
			epg_lookup_table[i].ipg_range = 0;
			epg_lookup_table[i].ipg = 0;
			dev_dbg(cqm_ctrl->dev,
				"FLAG_DIRECTPATH: epg:%d range:%d, igp:%d, range:%d\n",
				epg_lookup_table[i].epg,
				epg_lookup_table[i].epg_range,
				epg_lookup_table[i].ipg,
				epg_lookup_table[i].ipg_range);
		}
	}
}

static void
NO_OPTIMIZE set_dqm_cpu_port_range(const struct cqm_port_config *port_config)
{
	struct dqm_pid_s *deq_p = cqm_get_dqm_pid_cfg();

	switch (port_config->data.dqm_cpu.cpu_port_type) {
	case DP_F_DEQ_CPU:
		deq_p->cpu_start = port_config->data.dqm_cpu.port;
		deq_p->cpu_end = port_config->data.dqm_cpu.port +
			port_config->data.dqm_cpu.port_range;
		break;
	case DP_F_DEQ_INVALID:
		deq_p->vm_start = port_config->data.dqm_cpu.port;
		deq_p->vm_end = port_config->data.dqm_cpu.port +
			port_config->data.dqm_cpu.port_range;
		break;
	case DEQ_TOE:
		deq_p->lro_start = port_config->data.dqm_cpu.port;
		deq_p->lro_end = port_config->data.dqm_cpu.port +
			port_config->data.dqm_cpu.port_range;
		break;
	case DEQ_VOICE:
		deq_p->voice_start = port_config->data.dqm_cpu.port;
		deq_p->voice_end = port_config->data.dqm_cpu.port +
			port_config->data.dqm_cpu.port_range;
		break;
	case DEQ_VM:
		deq_p->vm_start = port_config->data.dqm_cpu.port;
		deq_p->vm_end = port_config->data.dqm_cpu.port +
			port_config->data.dqm_cpu.port_range;
		break;
	default:
		break;
	}
}

static void
NO_OPTIMIZE set_eqm_cpu_port_range(const struct cqm_port_config *port_config)
{
	struct eqm_pid_s *enq_p = cqm_get_eqm_pid_cfg();

	switch (port_config->data.eqm_cpu.port_type) {
	case EQM_CPU:
		enq_p->cpu_start = port_config->data.eqm_cpu.port;
		enq_p->cpu_end = port_config->data.eqm_cpu.port +
			port_config->data.eqm_cpu.port_range;
		break;
	case EQM_DPDK:
		enq_p->vm_start = port_config->data.eqm_cpu.port;
		enq_p->vm_end = port_config->data.eqm_cpu.port +
			port_config->data.eqm_cpu.port_range;
		break;
	case EQM_VOICE:
		enq_p->voice_start = port_config->data.eqm_cpu.port;
		enq_p->voice_end = port_config->data.eqm_cpu.port +
			port_config->data.eqm_cpu.port_range;
		break;
	case EQM_CPU_QBYP:
		enq_p->qosbp_start = port_config->data.eqm_cpu.port;
		enq_p->qosbp_end = port_config->data.eqm_cpu.port +
			port_config->data.eqm_cpu.port_range;
	break;
	case EQM_ACA:
		enq_p->aca_start = port_config->data.eqm_cpu.port;
		enq_p->aca_end = port_config->data.eqm_cpu.port +
			port_config->data.eqm_cpu.port_range;
		break;
	default:
		break;
	}
}

static void
NO_OPTIMIZE set_dqm_dma_port_range(const struct cqm_port_config *port_config)
{
	struct dqm_pid_s *deq_p = cqm_get_dqm_pid_cfg();

	switch (port_config->data.dqm_dma.port_type) {
	case DEQ_VUNI:
		deq_p->vuni_start = port_config->data.dqm_dma.port;
		deq_p->vuni_end = port_config->data.dqm_dma.port +
			port_config->data.dqm_dma.port_range;
		deq_p->dma2_start = port_config->data.dqm_dma.port;
		break;
	case DEQ_CPU_QOSBP:
		deq_p->qosbp_start = port_config->data.dqm_dma.port;
		deq_p->qosbp_end = port_config->data.dqm_dma.port +
			port_config->data.dqm_dma.port_range;
		break;
	case DEQ_VPN:
		deq_p->vpn_start = port_config->data.dqm_dma.port;
		deq_p->vpn_end = port_config->data.dqm_dma.port +
			port_config->data.dqm_dma.port_range;
		deq_p->dma0_end = deq_p->vpn_end;
		break;
	case DEQ_REINSERT:
		deq_p->reinsert_start = port_config->data.dqm_dma.port;
		deq_p->reinsert_end = port_config->data.dqm_dma.port +
			port_config->data.dqm_dma.port_range;
		break;
	case DEQ_DC_DOCSIS:
		deq_p->docsis_dc_dma_start = port_config->data.dqm_dma.port;
		deq_p->docsis_dc_dma_end = port_config->data.dqm_dma.port +
			port_config->data.dqm_dma.port_range;
		break;

	default:
		break;
	}
}

static void
NO_OPTIMIZE set_eqm_dma_port_range(const struct cqm_port_config *port_config)
{
	struct eqm_pid_s *enq_p = cqm_get_eqm_pid_cfg();

	switch (port_config->data.eqm_dma.port_type) {
	case EQM_DMA_PON:
		enq_p->dma0_start = port_config->data.eqm_dma.port;
		break;
	case EQM_DMA_SIZE2:
		enq_p->dma0_end = port_config->data.eqm_dma.port +
			port_config->data.eqm_dma.port_range;
		break;
	case EQM_TSO:
		enq_p->tso = port_config->data.eqm_dma.port;
		break;
	case EQM_LRO:
		enq_p->lro_start = port_config->data.eqm_dma.port;
		enq_p->lro_end = port_config->data.eqm_dma.port +
			port_config->data.eqm_dma.port_range;
		break;
	case EQM_RXDMA:
		enq_p->rxdma = port_config->data.eqm_dma.port;
		break;
	default:
		break;
	}
}

static void
NO_OPTIMIZE set_port_range(const struct cqm_port_config *port_config)
{
	struct dqm_pid_s *deq_p = cqm_get_dqm_pid_cfg();

	while (port_config->type != NONE_TYPE) {
		switch (port_config->type) {
		case DQM_CPU_TYPE:
			set_dqm_cpu_port_range(port_config);
			break;
		case DQM_ACA_TYPE:
			if (port_config->data.dqm_aca.cpu_port_type ==
			    DEQ_ACA) {
				deq_p->aca_start =
					port_config->data.dqm_aca.port;
				deq_p->aca_end =
					port_config->data.dqm_aca.port +
					port_config->data.dqm_aca.port_range;
			}
			break;
		case DQM_DMA_TYPE:
			set_dqm_dma_port_range(port_config);
			break;
		case DQM_PON_TYPE:
			deq_p->pon_start = port_config->data.dqm_dma.port;
			deq_p->pon_end = port_config->data.dqm_dma.port +
				port_config->data.dqm_dma.port_range;
			break;
		case DQM_PP_NF_TYPE:
			deq_p->ppnf_start =
				port_config->data.dqm_dma.port;
			deq_p->ppnf_end = port_config->data.dqm_dma.port +
				port_config->data.dqm_dma.port_range;
			break;
		case DQM_DOCSIS_TYPE:
			deq_p->docsis_start = port_config->data.dqm_dma.port;
			deq_p->docsis_end = port_config->data.dqm_dma.port +
				port_config->data.dqm_dma.port_range;
			break;
		case EQM_CPU_TYPE:
			set_eqm_cpu_port_range(port_config);
			break;
		case EQM_DMA_TYPE:
			set_eqm_dma_port_range(port_config);
			break;
		default:
			break;
		}
		port_config++;
	}
}

static int
NO_OPTIMIZE configure_ports(const struct cqm_port_config *port_config)
{
	int result = CBM_FAILURE;

	while (port_config->type != NONE_TYPE) {
		switch (port_config->type) {
		case DQM_DMA_TYPE:
		case DQM_PON_TYPE:
		case DQM_DOCSIS_TYPE:
			result = conf_deq_dma_port(&port_config->data.dqm_dma);
			break;
		case DQM_PP_NF_TYPE:
			result = conf_pp_nf_port(&port_config->data.dqm_dma);
			break;
		case DQM_CPU_TYPE:
			result = conf_deq_cpu_port(&port_config->data.dqm_cpu);
			break;
		case DQM_ACA_TYPE:
			result = conf_deq_aca_port(&port_config->data.dqm_aca);
			break;
		case EQM_DMA_TYPE:
			result = conf_enq_dma_port(&port_config->data.eqm_dma);
			break;
		case EQM_CPU_TYPE:
			result = conf_enq_cpu_port(&port_config->data.eqm_cpu);
			break;
		};
		if (result)
			return CBM_FAILURE;
		port_config++;
	}
	return CBM_SUCCESS;
}

static inline void cqm_rst_deassert(struct cqm_data *lpp)
{
	u32 status = reset_control_status(lpp->rcu_reset);

	dev_dbg(cqm_ctrl->dev, "0x%x\n", status);
	reset_control_deassert(lpp->rcu_reset);
	dev_dbg(cqm_ctrl->dev, "poll until its zero\n");
	while (status)
		status = reset_control_status(lpp->rcu_reset);
}

static void cqm_rst(struct cqm_data *lpp)
{
	cqm_rst_deassert(lpp);
}

static int
NO_OPTIMIZE get_dflt_head_tail_room(struct cbm_dflt_resv *resv, int flag)
{
	enum DP_SPL_TYPE spl_type = DP_SPL_MAX;
	enum CQM_SUP_DEVICE sub_dev = UNKNOWN;

	/* CPU, DirectPath or spL_conn under CPU port */
	if (!resv->dp_port || (resv->alloc_flags & DP_F_DIRECT)) {
		/* It is normal CPU or DirectPath: LGM has 8 subif/GPID */
		if (resv->f_cpu || (resv->alloc_flags & DP_F_DIRECT)) {
			resv->headroom = PP_HEADROOM;
			resv->tailroom =
				SKB_DATA_ALIGN(sizeof(struct skb_shared_info));
		} else {
			/* it is spl_conn */
			spl_type = resv->spl_conn_type;
			if (spl_type < DP_SPL_MAX) {
				resv->headroom = hr_tr_spl[spl_type].headroom;
				resv->tailroom = hr_tr_spl[spl_type].tailroom;
			} else {
				dev_err(cqm_ctrl->dev, "Inval SPL CONN type%d!\n",
					spl_type);
				return CBM_FAILURE;
			}
		}
	} else {/* non-cpu LPID/DP Port */
	/* check alloc_flag  to get its type */
		if (resv->alloc_flags & (DP_F_GPON | DP_F_EPON)
		& resv->f_segment) {
			resv->headroom = 0;
			resv->tailroom = 0;
			return CBM_SUCCESS;
		}
		sub_dev = get_type_from_alloc_flags(resv->alloc_flags);
		if (sub_dev != UNKNOWN) {
			resv->headroom = hr_tr_dp[sub_dev].headroom;
			resv->tailroom = hr_tr_dp[sub_dev].tailroom;
		}
	}
	return CBM_SUCCESS;
}

static int NO_OPTIMIZE cqm_get_mtu_size(struct cbm_mtu *mtu)
{
	struct cbm_dflt_resv resv = {0};
	u32 largest_buf_bit_pos = 0;
	u32 largest_buf_size;
	int idx;
	u32 lgm_pool_size_start, lgm_pool_size_end;

	if (!mtu->policy_map ||
	    mtu->policy_map >= 1 << BM_BUF_SIZE_MAX) {
		dev_info(cqm_ctrl->dev, "%s Invalid policy_map %d\n",
			 __func__, mtu->policy_map);
		return CBM_FAILURE;
	}
	resv.alloc_flags = mtu->alloc_flag;
	resv.f_cpu = mtu->f_cpu;
	resv.f_segment = mtu->f_segment;
	resv.spl_conn_type = mtu->spl_conn_type;
	resv.dp_port = mtu->dp_port;
	get_dflt_head_tail_room(&resv, 0);

	/* mtu should be the maximum buffer size in the policy map */
	largest_buf_bit_pos = fls(mtu->policy_map) - 1;
	largest_buf_size = cqm_ctrl->bm_buf_sizes[largest_buf_bit_pos];

	if ((mtu->alloc_flag & DP_F_CPU) || (mtu->alloc_flag & DP_F_DIRECT)) {
		lgm_pool_size_start = cqm_ctrl->cpu_base_pool;
		lgm_pool_size_end = lgm_pool_size_start + 4;
	} else {
		lgm_pool_size_start = 0;
		lgm_pool_size_end = lgm_pool_size_start + cqm_ctrl->num_sys_pools;
	}
	if (lgm_pool_size_start >= CQM_LGM_TOTAL_BM_POOLS ||
	    lgm_pool_size_end >= CQM_LGM_TOTAL_BM_POOLS) {
		dev_err(cqm_ctrl->dev, "Invalid lgm_pool_size_start %d or lgm_pool_size_end %d\n",
			lgm_pool_size_start, lgm_pool_size_end);
		return CBM_FAILURE;
	}
	mtu->mtu = 0;
	for (idx = lgm_pool_size_start; idx < lgm_pool_size_end; idx++) {
		if (cqm_ctrl->lgm_pool_size[idx] >= largest_buf_size) {
			/* It was agreed that the maximum mtu is 10K - HR - TR */
			mtu->mtu = min_t(int, cqm_ctrl->lgm_pool_size[idx], SIZE_10K) -
				   resv.headroom - resv.tailroom;
			break;
		}
	}
	if (!mtu->mtu) {
		dev_err(cqm_ctrl->dev, "Invalid mtu size\n");
		return CBM_FAILURE;
	}

	CQM_DEBUG(CQM_DBG_FLAG_MTU,
		  "%s dp_port: 0x%x\n", __func__, mtu->dp_port); 
	CQM_DEBUG(CQM_DBG_FLAG_MTU,
		  "%s policy_map: 0x%x\n", __func__, mtu->policy_map);
	CQM_DEBUG(CQM_DBG_FLAG_MTU,
		  "%s alloc_flags: 0x%x\n", __func__, mtu->alloc_flag);
	CQM_DEBUG(CQM_DBG_FLAG_MTU,
		  "%s subif_flag: 0x%x\n", __func__, mtu->subif_flag);
	CQM_DEBUG(CQM_DBG_FLAG_MTU,
		  "%s spl_conn_type: %d\n", __func__, mtu->spl_conn_type);
	CQM_DEBUG(CQM_DBG_FLAG_MTU,
		  "%s f_cpu: %u\n", __func__, mtu->f_cpu);
	CQM_DEBUG(CQM_DBG_FLAG_MTU,
		  "%s f_spl_gpid: %u\n", __func__, mtu->f_spl_gpid);
	CQM_DEBUG(CQM_DBG_FLAG_MTU,
		  "%s f_segment: 0x%x\n", __func__, mtu->f_segment);

	CQM_DEBUG(CQM_DBG_FLAG_MTU,
		  "%s mtu:%u, headroom:%d, tailroom:%d\n",
		  __func__, mtu->mtu, resv.headroom, resv.tailroom);
	return CBM_SUCCESS;
}

static const struct cbm_ops cqm_ops = {
	.cbm_igp_delay_set = cqm_igp_delay_set,
	.cbm_igp_delay_get = cqm_igp_delay_get,
	.cbm_buffer_free = cqm_buffer_free,
	.cbm_skb_buff_free = skb_buff_free,
	.cbm_cpu_pkt_tx = cqm_cpu_pkt_tx,
	.check_ptr_validation = check_ptr_validation_lgm,
	.cbm_build_skb = build_skb_cqm,
	.cbm_build_skb_rx = build_skb_from_dma_desc,
	.cbm_queue_map_get = queue_map_get,
	.cbm_queue_map_set = queue_map_set,
	.cbm_queue_map_buf_free = queue_map_buf_free,
	.cqm_qid2ep_map_get = qid2ep_map_get,
	.cqm_qid2ep_map_set = qid2ep_map_set,
	.cqm_mode_table_get = mode_table_get,
	.cqm_mode_table_set = mode_table_set,
	.cbm_gpid_lpid_map = gpid_lpid_map,
	.cbm_dp_port_alloc = dp_port_alloc,
	.cbm_dp_port_alloc_complete = dp_port_alloc_complete,
	.cbm_dp_spl_conn = cqm_dp_spl_conn,
	.cbm_dp_port_dealloc = cqm_dp_port_dealloc,
	.cbm_dp_enable = dp_enable,
	.cqm_qos_queue_flush = qos_q_flush,
	.cbm_alloc_skb = cqm_alloc_skb,
	.cbm_cpu_port_get = cqm_cpu_port_get,
	.pib_program_overshoot = pib_program_overshoot,
	.pib_status_get = pib_status_get,
	.pib_ovflw_cmd_get = pib_ovflw_cmd_get,
	.pib_illegal_cmd_get = pib_illegal_cmd_get,
	.set_lookup_qid_via_index = set_lookup_qid_via_index_lgm,
	.get_lookup_qid_via_index = get_lookup_qid_via_idx_lgm,
	.cqm_map_to_drop_q = map_to_drop_q,
	.cqm_restore_orig_q = restore_orig_q,
	.cqm_get_dflt_resv = get_dflt_head_tail_room,
	.cqm_dc_buffer_dummy_request = dc_buffer_dummy_request,
	.cbm_enable_backpressure = enable_backpressure,
	.cbm_get_mtu_size = cqm_get_mtu_size,
	.cqm_get_policy_map = get_policymap_from_syspool,
	.cbm_dp_get_dc_config = cqm_get_dc_config,
	.cqm_get_version = cqm_get_version,
};

static const struct cqm_soc_data lgm_urx851_data = {
	.sys_type = LGM_SYS_PON | LGM_SYS_DOCSIS,
	.cqm_soc = URX851B,
};

static const struct cqm_soc_data lgm_urx851c_data = {
	.sys_type = LGM_SYS_PON | LGM_SYS_DOCSIS,
	.cqm_soc = URX851C,
};

static const struct cqm_soc_data lgm_urx651_data = {
	.sys_type = LGM_SYS_PON,
	.cqm_soc = URX651,
};

static const struct of_device_id cqm_lgm_match[] = {
	{ .compatible = "mxl,lgm-cqm", .data = &lgm_urx851_data},
	{ .compatible = "mxl,lgmc-cqm", .data = &lgm_urx851c_data},
	{ .compatible = "mxl,lgm-cqm-urx651", .data = &lgm_urx651_data},
	{ /* sentinel */ }
};

static int NO_OPTIMIZE prepare_dpdk_db(struct cqm_data *pdata)
{
	struct cqm_dpdk_setting *dpdk;
	int dpdk_idx = 0;
	int i;

	dpdk = cqm_ctrl->cqm_dpdk;
	for (i = 1; i < pdata->num_dq_port; i += 2) {
		if (pdata->dq_port[i] == DPDK_EG_PORT) {
			if (dpdk_idx >= DPDK_MAX_CPU) {
				dev_dbg(cqm_ctrl->dev, "DPDK at most %d CPU!",
					DPDK_MAX_CPU);
				break;
			}
			dpdk[dpdk_idx].cpu_index = pdata->dq_port[i - 1] / 2;
			dpdk[dpdk_idx].deq_port = pdata->dq_port[i - 1];
			dpdk[dpdk_idx].deq_mode = pdata->dpdk_cfg[i - 1];
			dpdk[dpdk_idx].ring_size = pdata->dpdk_cfg[i];
			dev_dbg(cqm_ctrl->dev,
				"DPDK CPU %d deq mode %d ring size %d\n",
				dpdk[dpdk_idx].cpu_index,
				dpdk[dpdk_idx].deq_mode,
				dpdk[dpdk_idx].ring_size);
			dpdk_idx++;
		}
	}
	if (dpdk_idx > 0)
		pdata->config_dpdk = 1;

	for (i = 0; i < dpdk_idx; i++)
		cqm_ctrl->cpu_owner[dpdk[i].cpu_index] = CBM_CORE_DPDK;

	for (; dpdk_idx < DPDK_MAX_CPU; dpdk_idx++)
		dpdk[dpdk_idx].cpu_index = -EINVAL;

	for (i = 0; i < CQM_MAX_CPU; i++)
		dev_dbg(cqm_ctrl->dev, "# CPU %d  owner %d\n",
			i, cqm_ctrl->cpu_owner[i]);

	return 0;
}

static int check_base_addr(void)
{
	if (!cqm_ctrl->txpush ||
	    !cqm_ctrl->dmadesc_128 ||
	    !cqm_ctrl->dmadesc_64 ||
	    !cqm_ctrl->cqm ||
	    !cqm_ctrl->qidt0 ||
	    !cqm_ctrl->qidt1 ||
	    !cqm_ctrl->ls ||
	    !cqm_ctrl->enq ||
	    !cqm_ctrl->deq ||
	    !cqm_ctrl->qid2ep ||
	    !cqm_ctrl->fsqm ||
	    !cqm_ctrl->fsqm_desc ||
	    !cqm_ctrl->pib) {
		return -ENOMEM;
	}
	return CBM_SUCCESS;
}

static void NO_OPTIMIZE cqm_enable_re_insertion_port(s32 portid)
{
	struct cbm_dp_alloc_data data = {0};
	struct dqm_pid_s *p = &cqm_ctrl->dqm_pid_cfg;

	/* Fill the DMA channels and enable */
	fill_dp_alloc_data(&data, 0, p->reinsert_start, 0);
	handle_dma_chnl_init(p->reinsert_start, 0);
}

static int cqm_tx(struct sk_buff *skb, struct dp_tx_common *cmn, void *p)
{
	struct cbm_tx_data data = {
		.cbm_inst = cmn->inst,
		.pmac = (cmn->pmac_len ? cmn->pmac : NULL),
		.pmac_len = cmn->pmac_len,
		.f_byqos = !!(cmn->flags & DP_TX_BYPASS_QOS),
	};

	return cqm_cpu_pkt_tx(skb, &data, 0);
}

static int cqm_update_epg_lookup_table(unsigned long *port_p)
{
	int idx;

	for (idx = 0; idx < ARRAY_SIZE(epg_lookup_table); idx++)
		if (test_bit(idx, port_p))
			epg_lookup_table[idx].valid = false;
		else
			epg_lookup_table[idx].valid = true;

	CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "dynamic port bitmap: 0x%lx\n",
		  *port_p);

	for (idx = 0; idx < ARRAY_SIZE(epg_lookup_table); idx++) {
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "epg_lookup_table:\n");
		CQM_DEBUG(CQM_DBG_FLAG_DP_INTF,
			  "[%d]: lpid: %u port_type: 0x%x valid: %d\n",
			  idx,
			  epg_lookup_table[idx].lpid,
			  epg_lookup_table[idx].port_type,
			  epg_lookup_table[idx].valid);
	}

	return CBM_SUCCESS;
}

static int cqm_validate_lpid_epg_map(struct cqm_lpid_epg_map_s *map_p)
{
	int idx;
	u8 txdma_ctrl = 0, txdma_chan = 0;

	for (idx = 0; idx < LPID_COUNT; idx++) {
		if (!map_p[idx].lpid)
			continue;

		if (cqm_config_deq_port_to_txdma(cqm_ctrl, map_p[idx].epg,
						 &txdma_ctrl, &txdma_chan)) {
			dev_err(cqm_ctrl->dev, "Unable to find txdma for deq port: %d\n",
				map_p[idx].epg);
			return CBM_FAILURE;
		} else {
			if (map_p[idx].txdma_ctrl != txdma_ctrl ||
			    map_p[idx].txdma_chan != txdma_chan) {
				dev_err(cqm_ctrl->dev,
					"Invalid txdma (%d, %d) for deq port: %d\n",
					map_p[idx].txdma_ctrl,
					map_p[idx].txdma_chan,
					map_p[idx].epg);
				return CBM_FAILURE;
			}
		}
	}

	return CBM_SUCCESS;
}

static int cqm_update_lpid_epg_map(struct cqm_lpid_epg_map_s *map_p)
{
	int idx, idy;

	for (idx = 0; idx < LPID_COUNT; idx++) {
		if (!map_p[idx].lpid)
			continue;
		for (idy = 0; idy < ARRAY_SIZE(epg_lookup_table); idy++) {
			if (epg_lookup_table[idy].lpid == map_p[idx].lpid) {
				epg_lookup_table[idy].epg = map_p[idx].epg;
				break;
			}
		}
		if (idy == ARRAY_SIZE(epg_lookup_table)) {
			dev_err(cqm_ctrl->dev, "Unable to find map for lpid: %d\n",
				map_p[idx].lpid);
			return CBM_FAILURE;
		}
	}

	return CBM_SUCCESS;
}

static int cqm_parse_lpid_config(const struct cqm_data *pdata)
{
	int idx;

	cqm_ctrl->lpid_config.cqm_lpid_port_mode = (unsigned long)pdata->lpid_port_mode;
	cqm_ctrl->lpid_config.cqm_lpid_wan_mode = pdata->lpid_wan_mode;
	for (idx = 0; idx < LPID_COUNT; idx++) {
		cqm_ctrl->lpid_config.lpid_epg_map[idx].lpid = pdata->lpid_epg_map[idx].lpid;
		cqm_ctrl->lpid_config.lpid_epg_map[idx].epg = pdata->lpid_epg_map[idx].epg;
		cqm_ctrl->lpid_config.lpid_epg_map[idx].txdma_ctrl = pdata->lpid_epg_map[idx].txdma_ctrl;
		cqm_ctrl->lpid_config.lpid_epg_map[idx].txdma_chan = pdata->lpid_epg_map[idx].txdma_chan;
	}

	/* Verify whether minimum dynamic ports are configured */
	if ((LPID_LPID_DYN_MINIMUM & *cqm_port_mode())
	    != LPID_LPID_DYN_MINIMUM) {
		dev_err(cqm_ctrl->dev, "lpid config port mode 0x%lx is invalid\n",
			cqm_ctrl->lpid_config.cqm_lpid_port_mode);
		return CBM_FAILURE;
	}

	/* Update epg_lookup_table according to lpid_port_mode */
	if (cqm_update_epg_lookup_table(cqm_port_mode())) {
		dev_err(cqm_ctrl->dev, "Failed to update epg lookup table\n");
		return CBM_FAILURE;
	}

	/* Validate CQM DEQ port to txdma mapping in lpid_epg_map */
	if (cqm_validate_lpid_epg_map(cqm_lpid_epg_map())) {
		dev_err(cqm_ctrl->dev, "Invalid lpid_epg_map\n");
		return CBM_FAILURE;
	}

	/* Update epg_lookup_table according to lpid_epg_map */
	if (cqm_update_lpid_epg_map(cqm_lpid_epg_map())) {
		dev_err(cqm_ctrl->dev, "Failed to update lpid epg map\n");
		return CBM_FAILURE;
	}

	CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "%s: port_sel_mode:0x%lx, wan_sel:%d\n",
		  __func__, *cqm_port_mode(),
		  cqm_ctrl->lpid_config.cqm_lpid_wan_mode);
	return CBM_SUCCESS;
}

static int cqm_parse_bm_buf_sizes(const struct cqm_data *pdata)
{
	int idx;

	for (idx = 0; idx < BM_BUF_SIZE_MAX; idx++) {
		cqm_ctrl->bm_buf_sizes[idx] = pdata->bm_buf_sizes[idx];
		if (idx > 0 &&
		    cqm_ctrl->bm_buf_sizes[idx] <= cqm_ctrl->bm_buf_sizes[idx - 1]) {
			dev_err(cqm_ctrl->dev, "Invalid buffer size %d\n",
				cqm_ctrl->bm_buf_sizes[idx]);
			return CBM_FAILURE;
		}
	}
	return CBM_SUCCESS;
}

/* A function that factors a number into two numbers.
 * The two numbers have the range of 0-1023 and 1-256.
 * If the number can't be factored, find the two numbers such that
 * their product is closest to the number.
 * This function uses binary search to find the factors faster.
 */
static void cqm_factor(u32 number, u32 *count, u32 *step)
{
	u32 i, low, high, mid, min, product;
	u32 count_min = 0, step_min = 1;
	u32 count_max =  DEQ_DLY_CNT_DCP_POMA(0, COUNT, MASK) >>
			 DEQ_DLY_CNT_DCP_POMA(0, COUNT, POS);
	u32 step_max = 1 << (DEQ_DLY_CNT_DCP_POMA(0, STEP1, POS) -
			     DEQ_DLY_CNT_DCP_POMA(0, STEP128, POS) + 1);

	*count = 0;
	*step = 0;

	min = number;
	for (i = step_min; i <= step_max; i *= 2) {
		if ((i * count_max) < number)
			continue;

		low = count_min;
		high = count_max;
		while (low <= high) {
			mid = (low + high) / 2;
			product = i * mid;
			if (product == number) {
				*step = i;
				*count = mid;
				return;
			} else if (product > number) {
				if (product - number < min) {
					min = product - number;
					*step = i;
					*count = mid;
				}
				high = mid - 1;
			} else {
				if (number - product < min) {
					min = number - product;
					*step = i;
					*count = mid;
				}
				low = mid + 1;
			}
		}
	}
	if (*step == 0 && *count == 0) {
		*step = step_max;
		*count = count_max;
	}
}

int cqm_update_deq_dma_delay(int idx)
{
	int step_pos_start = DEQ_DLY_CNT_DCP_POMA(0, STEP128, POS);
	int step_pos_end = DEQ_DLY_CNT_DCP_POMA(0, STEP1, POS);
	u32 count, step;
	u32 reg_val;

	if (cqm_ctrl->deq_dma_delay[idx].delay < DLY_PORT_MIN_DELAY_IN_CYCLES &&
	    cqm_ctrl->deq_dma_delay[idx].dcp < DLY_PORT_FSQM_BASE)
		cqm_ctrl->deq_dma_delay[idx].delay = DLY_PORT_MIN_DELAY_IN_CYCLES;

	cqm_factor(cqm_ctrl->deq_dma_delay[idx].delay, &count, &step);
	reg_val = count;
	reg_val |= bitrev32(step & GENMASK(step_pos_end - step_pos_start, 0)) >>
		   (BITS_PER_TYPE(typeof(step)) - 1 - step_pos_end);
	reg_val &= ~DEQ_DLY_CNT_DCP_POMA(0, DEQ_DLY_EN, MASK);
	reg_val |= (cqm_ctrl->deq_dma_delay[idx].delay ? 1 : 0) <<
		   DEQ_DLY_CNT_DCP_POMA(0, DEQ_DLY_EN, POS);
	cbm_w32(cqm_ctrl->deq + DEQ_DLY_CNT_DCP(idx), reg_val);
	return CBM_SUCCESS;
}

static int cqm_parse_deq_dma_delay(const struct cqm_data *pdata)
{
	int idx;

	for (idx = 0; idx < DLY_PORT_COUNT; idx++) {
		cqm_ctrl->deq_dma_delay[idx].dcp = pdata->deq_dma_delay[idx].dcp;
		cqm_ctrl->deq_dma_delay[idx].deq_port = pdata->deq_dma_delay[idx].deq_port;
		cqm_ctrl->deq_dma_delay[idx].delay = pdata->deq_dma_delay[idx].delay;
	}

	for (idx = 0; idx < DLY_PORT_COUNT; idx++)
		cqm_update_deq_dma_delay(idx);

	return CBM_SUCCESS;
}

/* Get port bitmap allowed for wav700 */
static s32 cqm_get_wav700_port(struct cqm_data *pdata)
{
	unsigned long port;
	int idx;

	if (pdata->port_resv[1] == 0 || pdata->port_resv[0] == 0) {
		dev_err(cqm_ctrl->dev, "%s invalid wav700 setting!\n",
			__func__);
		return CBM_FAILURE;
	}

	bitmap_zero(cqm_wav_port_mode(), CQM_LPID_BITMAP_LEN);
	for_each_set_bit(idx, cqm_port_mode(), CQM_LPID_BITMAP_LEN) {
		if (!(idx % pdata->port_resv[1])) {
			port = GENMASK(idx + pdata->port_resv[0] - 1, idx);
			if (bitmap_subset(&port,
					  cqm_port_mode(),
					  CQM_LPID_BITMAP_LEN)) {
				bitmap_copy(cqm_wav_port_mode(),
					    &port,
					    CQM_LPID_BITMAP_LEN);
				CQM_DEBUG(CQM_DBG_FLAG_DP_INTF, "wav lpid_port: 0x%lx\n",
					  *cqm_wav_port_mode());
			}
		}
	}
	if (bitmap_empty(cqm_wav_port_mode(), CQM_LPID_BITMAP_LEN)) {
		dev_err(cqm_ctrl->dev, "Unable to find dynamic port range\n");
		return CBM_FAILURE;
	}

	return CBM_SUCCESS;
}

static inline int cqm_get_metadata_policy(void *va, u8 pool, u8 *policy)
{
	void *head;
	u8 policy_loc = 0, pool_loc = 0;

	head = cqm_cpubuff_get_buffer_head(va, pool);
	if (get_metadata((u8 *)head + BM_METADATA_OFFSET, &pool_loc, &policy_loc)) {
		dev_err(cqm_ctrl->dev, "Invalid metadata for va: %p\n", va);
		return CBM_FAILURE;
	}

	*policy = policy_loc;
	return CBM_SUCCESS;
}

static inline int cqm_get_policy(void *va, u8 pool, u8 *policy)
{
	if (is_cpu_iso_pool((int)pool)) {
		*policy = cqm_ctrl->cpu_base_policy + pool - cqm_ctrl->cpu_base_pool;
		return CBM_SUCCESS;
	} else {
		return cqm_get_metadata_policy(va, pool, policy);
	}
}

#if IS_ENABLED(CONFIG_LGM_CQM_CPUBUF_RET_DC)
static inline int dc_ring_has_space(struct dc_return_ring *ring)
{
	return CIRC_SPACE_TO_END(ring->head,
				 ring->tail,
				 CPUBUF_RING_SIZE);
}

static inline int dc_ring_count(struct dc_return_ring *ring)
{
	return CIRC_CNT(ring->head,
			ring->tail,
			CPUBUF_RING_SIZE);
}

static inline u64 cqm_cpubuf_to_dc_desc(void *va, u8 pool, u8 policy)
{
	dma_addr_t pa;
	u32 hi, lo;
	void *head;

	head = cqm_cpubuff_get_buffer_head(va, pool);
	if (is_cpu_iso_pool((int)pool))
		pa = (dma_addr_t)virt_to_phys(head);
	else
		pa = dma_map_single_attrs(cqm_ctrl->dev, (void *)head,
					  bm_pool_conf[pool].buf_frm_size,
					  DMA_TO_DEVICE,
					  DMA_ATTR_NON_CONSISTENT);
	hi = FIELD_PREP(PTRRET_DW3_HIADDR, (upper_32_bits(pa) & 0xf));
	lo = lower_32_bits(pa);
	hi |= pool | policy << PTR_RTN_CPU_DW3_EGP_1_POLICY_POS;

	return (lo | ((u64)hi << 32ULL));
}

void cqm_cpubuff_return_dc(void *va, u8 pool, u8 policy)
{
	struct dc_return_ring *dc_ret;
	void *dc_reg = cpubuff->dc_free_base;
	unsigned long flags;
	unsigned long paddr;

	local_irq_save(flags);
	dc_ret = &get_cpu_var(dc_ret_ring);

	dc_ret->dc_desc[dc_ret->head] = cqm_cpubuf_to_dc_desc(va, pool, policy);
	dc_ret->head = (dc_ret->head + 1) & (CPUBUF_RING_SIZE - 1);

	if ((dc_ring_count(dc_ret) < cpubuff->brptr_num) &&
	    dc_ring_has_space(dc_ret)) {
		put_cpu_var(dc_ret_ring);
		local_irq_restore(flags);
		return;
	}

	spin_lock(&cpubuff->dcfree_lock);
	/* Return 32 buffers in a burst write */
	paddr = __pa((&dc_ret->dc_desc[dc_ret->tail]));
	intel_hw_mcpy(dc_reg + sizeof(u64) * (dc_ret->tail % cpubuff->num_desc),
		      (void *)paddr, cpubuff->mcpy_pid,
		      cpubuff->brptr_num * sizeof(u64),
		      MCPY_SRC_PADDR | MCPY_DST_PADDR | MCPY_WAIT_BEF_NEXT_COPY);

	dc_ret->tail = ((dc_ret->tail + cpubuff->brptr_num) & (CPUBUF_RING_SIZE - 1));
	spin_unlock(&cpubuff->dcfree_lock);

	put_cpu_var(dc_ret_ring);
	local_irq_restore(flags);
}
EXPORT_SYMBOL(cqm_cpubuff_return_dc);
#endif

static void __maybe_unused cqm_cpubuff_return(void *va, u8 pool, u8 policy)
{
	phys_addr_t pa;
	void *head;

	head = cqm_cpubuff_get_buffer_head(va, pool);

	if (is_cpu_iso_pool((int)pool))
		pa = virt_to_phys((void *)head);
	else
		pa = dma_map_single_attrs(cqm_ctrl->dev, (void *)head,
					  bm_pool_conf[pool].buf_frm_size,
					  DMA_TO_DEVICE,
					  DMA_ATTR_NON_CONSISTENT);

	pp_bmgr_push_buffer((u32)policy, (u32)pool, pa);
}

static int cqm_buff_free_real(void *va, u8 pool, u8 policy)
{
#if IS_ENABLED(CONFIG_LGM_CQM_CPUBUF_RET_DC)
	if (!cpubuff)
		return -ENOENT;

	if (cpubuff->mcpy_pid >= 0)
		cqm_cpubuff_return_dc(va, pool, policy);
	else /* fallback to original buf return */
		cqm_cpubuff_return(va, pool, policy);
#else
	cqm_cpubuff_return(va, pool, policy);
#endif
	return 0;
}

int cqm_ioc_free(u64 ioc_pa)
{
	dma_addr_t pa = ioc_pa;
	u8 pool = 0, policy = 0;
	void *va;
	int ret;

	va = (void *)phys_to_virt(pa);

	if (get_buf_pool(va, bm_pool_conf, &pool))
		return -ENOENT;

	if (cqm_get_policy(va, pool, &policy))
		return -ENOENT;

	ret = cqm_buff_free_real(va, pool, policy);
	if (ret)
		return ret;

	CQM_UP_STATS((*cqm_dbg_ptr)[policy][pool].free_lro_err_cnt);
	return 0;
}
EXPORT_SYMBOL(cqm_ioc_free);

int cqm_buff_free(void *va)
{
	u8 pool = 0, policy = 0;
	int ret;

	if (get_buf_pool(va, bm_pool_conf, &pool))
		return -ENOENT;

	if (cqm_get_policy(va, pool, &policy))
		return -ENOENT;

	ret = cqm_buff_free_real(va, pool, policy);
	if (ret)
		return ret;

	CQM_UP_STATS((*cqm_dbg_ptr)[policy][pool].free_cnt);
	return 0;
}

static inline int get_buf_idx(dma_addr_t pa, int pool_idx)
{
	u64 offset = (u64)pa - (u64)cpubuff->pool[pool_idx].start;

	return offset / CPUBUFF_SZ[pool_idx];
}

static inline atomic_t *cqm_page_ref_cnt(int pool_idx, int idx)
{
	return &cpubuff->pool[pool_idx].ref[idx];
}

int cqm_page_ref(dma_addr_t pa)
{
	u8 pool = 0;
	void *va;
	int idx;

	va = (void *)phys_to_virt(pa);

	if (get_buf_pool(va, bm_pool_conf, &pool))
		return -ENOENT;

	idx = get_buf_idx(pa, pool);
	set_bit(idx, cpubuff->pool[pool].ref_chk);
	return atomic_inc_return(cqm_page_ref_cnt(pool, idx));
}

int cqm_page_unref(dma_addr_t pa)
{
	u8 pool = 0, policy = 0;
	void *va;
	int idx;
	int ret;

	va = (void *)phys_to_virt(pa);

	if (get_buf_pool(va, bm_pool_conf, &pool))
		return -ENOENT;

	if (cqm_get_policy(va, pool, &policy))
		return -ENOENT;

	idx = get_buf_idx(pa, pool);
	if (test_bit(idx, cpubuff->pool[pool].ref_chk)) {
		if (!atomic_dec_and_test(cqm_page_ref_cnt(pool, idx)))
			return 0;
		clear_bit(idx, cpubuff->pool[pool].ref_chk);
	}

	ret = cqm_buff_free_real(va, pool, policy);
	if (ret)
		return ret;

	CQM_UP_STATS((*cqm_dbg_ptr)[pool + cpubuff->policy_base][pool +
	cpubuff->pool_base].free_frag_cnt);
	return 0;
}

#if IS_ENABLED(CONFIG_MXL_CBM_TEST)
/* CQM unit test module requires access to cqm_ops, which can be passed using
 * platform_device driver data. However, platform_device obtained from DT is
 * different from the platform_deivce passed to CQM during probe.
 * Here we set the driver data to the platform device obtained from DT.
 */
static int cqm_test_set_drv_data(void)
{
	struct device_node *node;
	struct platform_device *pdev;
	void *drv_data = NULL;

	/* Find CQM Platform device */
	node = of_find_compatible_node(NULL, NULL, "mxl,lgm-cqm");
	if (!node) {
		dev_err(cqm_ctrl->dev, "Unable to find CQM node\n");
		return -ENODEV;
	}
	of_node_put(node);
	pdev = of_find_device_by_node(node);
	if (!pdev) {
		dev_err(cqm_ctrl->dev, "Unable to find CQM platform device\n");
		return -ENODEV;
	}
	drv_data = platform_get_drvdata(pdev);
	if (drv_data) {
		dev_err(cqm_ctrl->dev, "Driver data is not NULL\n");
		return -EINVAL;
	}
	platform_set_drvdata(pdev, cqm_ctrl);

	return 0;
}
#endif

static int NO_OPTIMIZE cqm_lgm_probe(struct platform_device *pdev)
{
	struct resource *res[LGM_MAX_RESOURCE] = {NULL};
	int i, j;
	unsigned long sys_flag;
	struct cqm_data *pdata = NULL;
	const struct of_device_id *match;
	int result, idx;
	int skip[LS_PORT_NUM_LGM] = {0};
	struct gen_pool *fsqm_pool;
	dma_addr_t pool_phy;
	void *pool_virt;
	struct cqm_soc_data *soc_data = NULL;
	u32 val = 0;
	size_t sz, bm_ssb_pool_sz;
	int sys_type;

	/* for A0: CONFIG_LGM_A0_IOC is needed to enable CONFIG_IOC_POOL */
	#ifdef CONFIG_IOC_POOL
	if (dma_set_mask_and_coherent(&pdev->dev, DMA_BIT_MASK(36))) {
		dev_err(&pdev->dev, "Set dma bitmask to 36 failed!\n");
		return -ENODEV;
	}
	#else
	if (dma_set_mask(&pdev->dev, DMA_BIT_MASK(32)) < 0) {
		dev_err(&pdev->dev, "Set dma bitmask to 32 failed!\n");
		return -ENODEV;
	}
	#endif

	pdata = platform_get_drvdata(pdev);
	match = of_match_device(cqm_lgm_match, &pdev->dev);
	if (!match) {
		dev_err(&pdev->dev, "Error: No matching device found\n");
		return -ENODEV;
	}
	soc_data = (struct cqm_soc_data *)match->data;
	dev_dbg(&pdev->dev, "CQM: %s\n", match->compatible);
	/* PON and DOCSIS will not co-exist. Select sys_type based on
	 * device tree
	 */
	if ((pdata->lpid_wan_mode & LPID_WAN_DOCSIS) &&
	    (pdata->lpid_wan_mode & LPID_WAN_PON)) {
		dev_err(&pdev->dev, "PON and DOCSIS can't co-exist\n");
		return -EINVAL;
	} else if ((pdata->lpid_wan_mode & LPID_WAN_DOCSIS) &&
		   (soc_data->sys_type & LGM_SYS_DOCSIS)) {
		sys_type = LGM_SYS_DOCSIS;
	} else {
		sys_type = LGM_SYS_PON;
	}

	dev_info(&pdev->dev, "sys_type: 0x%x\n", sys_type);
	cqm_ctrl = get_cqm_ctrl(sys_type, soc_data->cqm_soc);
	cqm_ctrl->soc = soc_data->cqm_soc;
	cqm_ctrl->txin_fallback = pdata->txin_fallback;
	cqm_ctrl->cpu_enq_chk = pdata->cpu_enq_chk;
	if (is_lgmc_soc() && cqm_ctrl->txin_fallback)
		cqm_config_lgmc_txin_fallback(cqm_ctrl);

	cqm_ctrl->cqm_ops = &cqm_ops;
	register_cbm(cqm_ctrl->cqm_ops);
	dp_register_tx(DP_TX_CQM, cqm_tx, NULL);
	dp_register_ops(0, DP_OPS_CQM, (void *)cqm_ctrl->cqm_ops);

	/** init spin lock */
	spin_lock_init(&cqm_qidt_lock[0]);
	spin_lock_init(&cqm_qidt_lock[1]);
	spin_lock_init(&cqm_port_map);
	spin_lock_init(&cpu_pool_enq);

	/* load the memory ranges */
	for (i = 0; i < pdata->num_resources; i++) {
		res[i] = platform_get_resource(pdev, IORESOURCE_MEM, i);
		if (!res[i]) {
			dev_err(&pdev->dev, "failed to get resources %d\n", i);
			return -ENOENT;
		}
	}

	cqm_ctrl->name = cqm_name;
	cqm_ctrl->id = pdev->id;
	cqm_ctrl->dev = &pdev->dev;
	cqm_ctrl->dmadesc_64 = devm_ioremap_resource(&pdev->dev, res[0]);
	cqm_ctrl->dmadesc_64_phys = (void *)res[0]->start;
	cqm_ctrl->pib = devm_ioremap_resource(&pdev->dev, res[1]);
	cqm_ctrl->pib_phys = (void *)res[1]->start;
	cqm_ctrl->qid2ep = devm_ioremap_resource(&pdev->dev, res[2]);
	cqm_ctrl->qid2ep_phys = (void *)res[2]->start;
	cqm_ctrl->cqm = devm_ioremap_resource(&pdev->dev, res[3]);
	cqm_ctrl->cqm_phys = (void *)res[3]->start;
	cqm_ctrl->qidt0 = devm_ioremap_resource(&pdev->dev, res[4]);
	cqm_ctrl->qidt0_phys = (void *)res[4]->start;
	cqm_ctrl->qidt1 = devm_ioremap_resource(&pdev->dev, res[5]);
	cqm_ctrl->qidt1_phys = (void *)res[5]->start;
	cqm_ctrl->ls = devm_ioremap_resource(&pdev->dev, res[6]);
	cqm_ctrl->ls_phys = (void *)res[6]->start;
	cqm_ctrl->enq = devm_ioremap_resource(&pdev->dev, res[7]);
	cqm_ctrl->enq_phys = (void *)res[7]->start;
	cqm_ctrl->deq = devm_ioremap_resource(&pdev->dev, res[8]);
	cqm_ctrl->deq_phys = (void *)res[8]->start;
	cqm_ctrl->fsqm = devm_ioremap_resource(&pdev->dev, res[9]);
	cqm_ctrl->fsqm_phys = (void *)res[9]->start;
	cqm_ctrl->fsqm_desc = devm_ioremap_resource(&pdev->dev, res[10]);
	cqm_ctrl->fsqm_desc_phys = (void *)res[10]->start;
	cqm_ctrl->txpush = devm_ioremap_resource(&pdev->dev, res[11]);
	cqm_ctrl->txpush_phys = (void *)res[11]->start;
	cqm_ctrl->dmadesc_128 = devm_ioremap_resource(&pdev->dev, res[12]);
	cqm_ctrl->dmadesc_128_phys = (void *)res[12]->start;
	result = check_base_addr();
	if (result) {
		dev_err(cqm_ctrl->dev, "failed to request and remap io ranges\n");
		return result;
	}
	cqm_rst(pdata);
	cqm_dbg_ptr = &cqm_dbg_cntrs;
	pr_info("CQM RCU reset enabled\n");
	if (cqm_parse_lpid_config(pdata)) {
		dev_err(cqm_ctrl->dev, "pase lpid_config failed\n");
		return CBM_FAILURE;
	}
	if (cqm_parse_bm_buf_sizes(pdata)) {
		dev_err(cqm_ctrl->dev, "pase bm_buf_sizes failed\n");
		return CBM_FAILURE;
	}

	set_port_range(cqm_ctrl->cqm_cfg);
	store_bufreq_baseaddr();

	cqm_ctrl->v2_flag = is_version2();
	cqm_ctrl->is_cpu_pool_ioc = lgm_ioc_enable();

	dev_dbg(cqm_ctrl->dev, "%s: v2_flag:%d, is_cpu_pool_ioc:%d\n",
		__func__, cqm_ctrl->v2_flag, cqm_ctrl->is_cpu_pool_ioc);
	cpubuff = devm_kzalloc(cqm_ctrl->dev, sizeof(*cpubuff), GFP_KERNEL);
	if (!cpubuff)
		return -ENOMEM;

	cpubuff->dev = cqm_ctrl->dev;
	for (i = 0; i < num_possible_cpus(); i++)
		cpubuff->regs[i].ret =
			cqm_ctrl->deq + DQ_CPU_PORT(i, ptr_rtn_dw2d0);

#if IS_ENABLED(CONFIG_LGM_CQM_CPUBUF_RET_DC)
	cpubuff->mcpy_pid = mcpy_get_pid();
	spin_lock_init(&cpubuff->dcfree_lock);
#endif

	for_each_present_cpu(i) {
		j = i * 2;
		if (cpu_online(i)) {
			cqm_ctrl->cbm_irq[j] = pdata->intrs[j];
			cqm_ctrl->cbm_irq[j + 1] = pdata->intrs[j + 1];
		} else {
			cqm_ctrl->cbm_irq[j] = 0;
			cqm_ctrl->cbm_irq[j + 1] = 0;
		}
	}
	//todo: goto keep in reg code to avoid global variables
	result = prepare_dpdk_db(pdata);
	if (result)
		dev_warn(cqm_ctrl->dev, "failed to prepare dpdk data\n");

#ifndef CONFIG_USE_EMULATOR
	cqm_ctrl->cbm_clk = pdata->cqm_clk[0];
	clk_prepare_enable(pdata->cqm_clk[1]);
#endif
	/*override the default clk freq(800MHz) by DT entry*/
	if (pdata->cqm_clkrate > 0)
		clk_set_rate(cqm_ctrl->cbm_clk, pdata->cqm_clkrate);
	cqm_ctrl->syscfg = pdata->syscfg;
	cqm_ctrl->force_xpcs = pdata->force_xpcs;

	for (i = 0; i < MAX_CQM_FW_DOMAIN; i++)
		cqm_ctrl->sai[i] = pdata->sai[i];

	spin_lock_irqsave(&cqm_qidt_lock[0], sys_flag);
	for (i = 0; i < CQM_QIDT_DW_NUM; i++) {
		g_cbm_qidt_mirror[0][i].qidt_shadow = LGM_CQM_DROP_INIT;
		cbm_w32((cqm_ctrl->qidt0 + i * 4), LGM_CQM_DROP_INIT);
	}
	for (i = 0; i < CQM_QIDT_DW_NUM; i++) {
		g_cbm_qidt_mirror[1][i].qidt_shadow = LGM_CQM_DROP_INIT;
		cbm_w32((cqm_ctrl->qidt1 + i * 4), LGM_CQM_DROP_INIT);
	}
	for (i = 0; i < CQM_QID2EP_DW_NUM; i++)
		cbm_w32((cqm_ctrl->qid2ep + i * 4), 0x0);
	spin_unlock_irqrestore(&cqm_qidt_lock[0], sys_flag);

	configure_ports(cqm_ctrl->cqm_cfg);

	if (unlikely(cqm_bm_init(pdev, pdata))) {
		dev_err(cqm_ctrl->dev, "buf init failed\n");
		return CBM_FAILURE;
	}

	/* keep fsqm details at end of pool config lookup table */
	fsqm_pool = of_gen_pool_get(pdev->dev.of_node, "mxl,sram-pool", 0);

	if (fsqm_pool)
		val = (u32)gen_pool_size(fsqm_pool);
	else
		val = 0;

	if (!fsqm_pool || !val) {
		dev_info(&pdev->dev, "Failed to get sram pool!\n");
		bm_pool_conf[CQM_LGM_TOTAL_POOLS - 1] = fsqm_pool_conf[0];
		bm_pool_conf[CQM_LGM_TOTAL_POOLS - 1].pool_start_low = 0;
		bm_pool_conf[CQM_LGM_TOTAL_POOLS - 1].pool_end_low = 0;
	} else {
		/* Split sram into 2 parts:
		 * First part is used for SSB BM pool. Second part is used for
		 * FSQM. Size for BM pool 0 is buf_frm_size * buf_frm_num.
		 * Size for fsqm is sram-size - size of BM pool 0.
		 */
		bm_ssb_pool_sz = cqm_get_ssb_pool_size();
		if (bm_ssb_pool_sz > (size_t)val) {
			dev_err(&pdev->dev,
				"mxl,sram-size %zu < BM SSB pool size %zu\n",
				(size_t)val, bm_ssb_pool_sz);
			return -ENODEV;
		}
		cqm_ctrl->fsqm_sz = (size_t)val - bm_ssb_pool_sz;

		sz = gen_pool_avail(fsqm_pool);
		if (cqm_ctrl->fsqm_sz > sz) {
			dev_err(&pdev->dev,
				"mxl,sram-size %zu is larger than SRAM :%zu\n",
				cqm_ctrl->fsqm_sz, sz);
			return -ENODEV;
		}

		dev_info(&pdev->dev,
			 "FSQM allocating SRAM Size = %zu.\n",
			 cqm_ctrl->fsqm_sz);

		pool_virt = gen_pool_dma_alloc(fsqm_pool, cqm_ctrl->fsqm_sz,
					       &pool_phy);
		if (!pool_virt && cqm_ctrl->fsqm_sz) {
			dev_err(&pdev->dev,
				"Failed to get the requested size from pool!\n");
			return -ENODEV;
		}
		bm_pool_conf[CQM_LGM_TOTAL_POOLS - 1] = fsqm_pool_conf[0];
		bm_pool_conf[CQM_LGM_TOTAL_POOLS - 1].pool_start_low =
			(unsigned long)pool_phy;
		bm_pool_conf[CQM_LGM_TOTAL_POOLS - 1].pool_end_low =
			(unsigned long)pool_phy + cqm_ctrl->fsqm_sz;
	}

	if (cbm_hw_init(pdev))
		return -1;

	set_egp_lu_tbl();

	cbm_w32((cqm_ctrl->cqm + CBM_BSL_CTRL), CBM_BSL_CTRL_BSL1_EN_MASK);
	/*Enable the EQM and DQM contollers*/
	cbm_w32((cqm_ctrl->deq + CBM_DQM_CTRL), CBM_DQM_CTRL_DQM_EN_MASK);
	for (i = 0; i < DEFAULT_WAIT_CYCLES; i++) {
		if ((cbm_r32(cqm_ctrl->deq + CBM_DQM_CTRL) &
		    CBM_DQM_CTRL_DQM_ACT_MASK) != 0)
			dev_dbg(cqm_ctrl->dev, "cbm dqm init successfully\n");
	}

	/* Enable C-step DEQ DLY and TXIN Enhancement features */
	if (is_lgmc_soc()) {
		set_val((cqm_ctrl->deq + CBM_DQM_CTRL), 1,
			CBM_DQM_CTRL_DEQ_DLY_FEN_MASK,
			CBM_DQM_CTRL_DEQ_DLY_FEN_POS);
		/* Set per port DEQ DMA delay */
		if (cqm_parse_deq_dma_delay(pdata)) {
			dev_err(cqm_ctrl->dev, "parse deq dma delay failed\n");
			return CBM_FAILURE;
		}

		if (!cqm_ctrl->txin_fallback)
			set_val((cqm_ctrl->deq + CBM_DQM_CTRL), 1,
				CBM_DQM_CTRL_TXIN_FEN_MASK,
				CBM_DQM_CTRL_TXIN_FEN_POS);
	}

	for (i = 0; i < DPDK_MAX_CPU; i++) {
		if (cqm_ctrl->cqm_dpdk[i].deq_mode == DEQ_PPV4) {
			skip[cqm_ctrl->cqm_dpdk[i].cpu_index * 2] = 1;
			skip[cqm_ctrl->cqm_dpdk[i].cpu_index * 2 + 1] = 1;
		}
	}
	init_cbm_ls(cqm_ctrl->ls, LS_PORT_NUM_LGM, &skip[0]);

	cbm_w32((cqm_ctrl->enq + CBM_EQM_CTRL), CBM_EQM_CTRL_EQM_EN_MASK);
	for (i = 0; i < DEFAULT_WAIT_CYCLES; i++) {
		if ((cbm_r32(cqm_ctrl->enq + CBM_EQM_CTRL) &
		    CBM_EQM_CTRL_EQM_ACT_MASK) != 0) {
			dev_dbg(cqm_ctrl->dev, "cbm eqm init successfully\n");
			break;
		}
	}
	cqm_ctrl->wav.num = pdata->wav700;
	if (cqm_ctrl->wav.num) {
		if (cqm_get_wav700_port(pdata)) {
			dev_err(cqm_ctrl->dev, "fail to get wav ports\n");
			return CBM_FAILURE;
		}
	}
	cqm_enable_re_insertion_port(0);

	init_dummy_netdev(&cqm_ctrl->dummy_dev);
	memset(&cqm_ctrl->cqm_napi, 0, sizeof(cqm_ctrl->cqm_napi));

	init_dummy_netdev(&cqm_ctrl->dummy_dev_dp);
	memset(&cqm_ctrl->cqm_napi_dp, 0, sizeof(cqm_ctrl->cqm_napi_dp));

	for_each_online_cpu(i) {
		if (cqm_ctrl->cpu_owner[i] == CBM_CORE_DPDK)
			continue;

		for (idx = 2 * i; idx < 2 * i + PORTS_PER_CPU; idx++) {
			cqm_ctrl->cqm_napi[idx].line = idx;
			cqm_ctrl->cqm_napi[idx].desc =
				(struct dp_dma_desc __iomem *)(cqm_ctrl->ls +
				CBM_LS_PORT(idx, desc.desc0));
			netif_napi_add(&cqm_ctrl->dummy_dev,
				       &cqm_ctrl->cqm_napi[idx].napi,
				       cqm_poll, CQM_NAPI_POLL_WEIGHT);
			napi_enable(&cqm_ctrl->cqm_napi[idx].napi);
		}
	}

	for (idx = 0; idx <= cqm_ctrl->dqm_pid_cfg.vm_end -
	     cqm_ctrl->dqm_pid_cfg.vm_start; idx++) {
		cqm_ctrl->cqm_napi_dp[idx].line = idx;
		cqm_ctrl->cqm_napi_dp[idx].read_idx = 0;
		cqm_ctrl->cqm_napi_dp[idx].desc = (struct dp_dma_desc __iomem *)(cqm_ctrl->deq +
						  DQ_CPU_PORT(idx + CQM_CPU_VM_PORT_OFFSET, desc[0]));
		netif_napi_add(&cqm_ctrl->dummy_dev_dp,
			       &cqm_ctrl->cqm_napi_dp[idx].napi,
			       cqm_poll_dp, CQM_NAPI_POLL_WEIGHT);
		napi_enable(&cqm_ctrl->cqm_napi_dp[idx].napi);
	}

	/* Disable all the EQM and DQM interrupts */
	eqm_intr_ctrl(0);
	dqm_intr_ctrl(0);
	/*Override the HW reset values*/
	cbm_w32(cqm_ctrl->enq + DMA_RDY_EN, 0x3ff);
	dev_info(cqm_ctrl->dev, "CBM Clock: %ldHz\n",
		 clk_get_rate(cqm_ctrl->cbm_clk));

	platform_set_drvdata(pdev, cqm_ctrl);
	cqm_debugfs_init(cqm_ctrl);

#if IS_ENABLED(CONFIG_MXL_CBM_TEST)
	cqm_test_set_drv_data();
#endif

	dev_info(cqm_ctrl->dev, "CBM: Init Done !!\n");
	return 0;
}

static int cqm_lgm_release(struct platform_device *pdev)
{
	return 0;
}

static struct platform_driver cqm_lgm_driver = {
	.probe = cqm_lgm_probe,
	.remove = cqm_lgm_release,
	.driver = {
		.name = LGM_DEV_NAME,
		.owner = THIS_MODULE,
	},
};

int __init cqm_lgm_init(void)
{
	return platform_driver_register(&cqm_lgm_driver);
}

device_initcall(cqm_lgm_init);
