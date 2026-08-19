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

#ifndef __CQM_COMMON_H
#define __CQM_COMMON_H
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <asm/dma-mapping.h>
#include <linux/netdevice.h>
#include <net/mxl_cbm_api.h>

#ifdef CONFIG_LGM_CQM
enum {
	DMA2TX = 0,
	DMA1RX,
	DMA1TX,
	DMA0TX,
	DMA3,
	TOE_DMA30,
	TOE_DMA31,
	DMAMAX,
};

enum dma_cid {
	DP_DMA0TX = 0,
	DP_DMA1TX = 1,
	DP_DMA2TX = 2,
	DP_DMA_CTRL_MAX = 0xFF,
};
#endif /* CONFIG_LGM_CQM */

#define CBM_PMAC_DYNAMIC 1000
#define CBM_PORT_INVALID 2000
#define CBM_PMAC_NOT_APPL 3000
#define CBM_PORT_NOT_APPL 255
#define LPID_COUNT	16
#define BM_BUF_SIZE_MAX         5
#define LS_PORT_NUM             4
#define LS_PORT_NUM_LGM         8
#define LS_PORT_NUM_MAX         8
#define CBM_MAX_INT_LINES       8
#define CBM_DMA_DESC_OWN        1U/*belong to DMA*/
#define CBM_DMA_DATA_OFFSET     122
#define CBM_LGM_DMA_DATA_OFFSET 48
/* TCP lite and LRO expects it to be 128 */
#define CBM_GRX550_DMA_DATA_OFFSET     128
#define DEFAULT_WAIT_CYCLES     20
#define DEFAULT_LS_QTHH         7
#define DEFAULT_LS_OVFL_CNT     0x2000
#define DEFAULT_LS_PORT_WEIGHT  2
#define CBM_SUCCESS             0
#define CBM_FAILURE             -1
#define CBM_BUSY                -16 /* Resource busy */
#define CBM_EQM_DELAY_ENQ 0x10
#define CBM_PDU_TYPE 26
#define TXPUSH_CMD_RX_EGP_1 0x100
#define CBM_MAX_POOLS_PER_POLICY	4
#define DLY_PORT_COUNT		16
#define DLY_PORT_FSQM_BASE	8
#define DLY_PORT_MIN_DELAY_IN_CYCLES	5

#define CBM_LS_PORT(idx, reg) \
(LS_DESC_DW0_PORT0 + ((idx) * 0x100) + offsetof(struct cbm_ls_reg, reg))

#define cbm_r32(m_reg)		readl(m_reg)
#define cbm_w32(m_reg, val)	writel(val, m_reg)
#define cbm_assert(cond, fmt, arg...) \
do { \
	if (!(cond)) \
		pr_err("%d:%s:" fmt "\n", __LINE__, __func__, ##arg); \
} while (0)
#define cbm_err(fmt, arg...) \
pr_err("%d:%s: " fmt "\n", __LINE__, __func__, ##arg)

#ifdef CBM_DEBUG
#define cbm_debug(fmt, arg...) \
pr_info(fmt, ##arg)
#else
#define cbm_debug(fmt, arg...)
#endif

#ifdef CBM_DEBUG_LVL_1
#define cbm_debug_1(fmt, arg...) \
pr_info(fmt, ##arg)
#else
#define cbm_debug_1(fmt, arg...)
#endif

#define get_val(val, mask, offset) (((val) & (mask)) >> (offset))

enum {
	SPREAD_WRR = 0,
	SPREAD_FLOWID = 1,
	SPREAD_MAX,
};

/* coupled with device tree data */
struct cqm_policy_params {
	u32 policy_id;
	u32 pool_id[CBM_MAX_POOLS_PER_POLICY];
	u32 pool_max_allowed[CBM_MAX_POOLS_PER_POLICY];
	u32 pool_count;
	u32 min_guaranteed;
	u32 max_allowed;
	u32 direction;
	u32 interface_type;
};

struct cqm_policy_res {
	s32 id;
	u32 count;
	u32 i_cnt;
	u32 e_cnt;
};

struct cbm_ls_reg {
	struct cbm_desc desc;
	u32 ctrl;
	u32 status;
	u32 resv0[2];
	struct cbm_desc qdesc[7];
};

struct qidt_flag_done {
	u8 cls_done;
	u8 ep_done;
	u8 fl_done;
	u8 fh_done;
	u8 dec_done;
	u8 enc_done;
	u8 mpe1_done;
	u8 mpe2_done;
	u8 sub_if_id_done;
	u8 sub_if_dc_done;
	u8 color_done;
};

struct cbm_q_info {
u16 refcnt; /* No of Queue Map table entries pointing to this q */
u16 qmap_idx_start; /* First index of Queue Map table pointing to q */
u16 qmap_idx_end; /* last index of Queue Map table pointing to q */
};

struct cbm_qidt_shadow {
u32 qidt_shadow;
u32 qidt_drop_flag;
};

struct dp_dma_desc;

static inline void set_val(void __iomem *reg, u32 val, u32 mask, u32 offset)
{
	u32 temp_val = cbm_r32(reg);

	temp_val &= ~(mask);
	temp_val |= (((val) << (offset)) & (mask));
	cbm_w32(reg, temp_val);
}

static inline int get_is_bit_set(u32 flags)
{
	return ffs(flags) - 1;
}

void buf_addr_adjust(unsigned long buf_base_addr, unsigned long buf_size,
		     unsigned long *adjusted_buf_base,
		     unsigned long *adjusted_buf_size,
		     unsigned long align);
int cbm_linearise_buf(struct sk_buff *skb, struct cbm_tx_data *data,
		      int buf_size, void *new_buf);
void init_cbm_ls(void __iomem *cqm_ls_addr_base, int port_num, int *skip);
int cqm_dma_get_controller(char *ctrl);
int dp_dma_get_controller(char *ctrl);
void ls_intr_ctrl(u32 val, void __iomem *cqm_ls_addr_base);
void cbm_dw_memset(u32 *base, int val, u32 size);
void register_cbm(const struct cbm_ops *cbm_cb);
const char *cqm_get_dma_engine_name(u32 ctl);
#endif
