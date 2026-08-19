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

#include "cqm_common.h"
#include "lgm/reg/cbm_ls.h"

static void __iomem *cqm_ls_addr_base;

#define LS_BASE cqm_ls_addr_base
#define LS_TH_SET \
((DEFAULT_LS_QTHH << LS_CTRL_PORT0_QUEUE_THRSHLD_POS) \
& (LS_CTRL_PORT0_QUEUE_THRSHLD_MASK))
#define LS_TH_SET_A0 \
((7 << LS_CTRL_PORT0_QUEUE_THRSHLD_POS) \
& (LS_CTRL_PORT0_QUEUE_THRSHLD_MASK))
#define LS_OVFL_SET \
((DEFAULT_LS_OVFL_CNT << LS_CTRL_PORT0_CNT_THRSHLD_POS) \
& (LS_CTRL_PORT0_CNT_THRSHLD_MASK))

void buf_addr_adjust(unsigned long buf_base_addr,
		     unsigned long buf_size,
		     unsigned long *adjusted_buf_base,
		     unsigned long *adjusted_buf_size,
		     unsigned long align)
{
	unsigned long base;
	unsigned long size;

	base = ALIGN(buf_base_addr, align);
	size = buf_base_addr + buf_size - base;

	*adjusted_buf_base = base;
	*adjusted_buf_size = size;
}

int cbm_linearise_buf(struct sk_buff *skb, struct cbm_tx_data *data,
		      int buf_size, void *new_buf)
{
	void *tmp_buf;
	void *frag_addr;
	const skb_frag_t *frag;
	int i, len = 0, copy_len = 0;

	if (skb_has_frag_list(skb)) {
		pr_err("%s: skb has frag list\n", __func__);
		return CBM_FAILURE;
	}

	if (new_buf) {
		tmp_buf = new_buf;
		if (skb_is_nonlinear(skb))
			copy_len = skb_headlen(skb);
		else
			copy_len = skb->len;
		if (data->pmac) {
			memcpy((u8 *)tmp_buf, data->pmac,
			       data->pmac_len);
			tmp_buf += data->pmac_len;
		}

		memcpy((u8 *)tmp_buf, skb->data, copy_len);
		tmp_buf += copy_len;
		/* If the packet has fragments, copy that also */
		for (i = 0; i < (skb_shinfo(skb)->nr_frags); i++) {
			frag = &skb_shinfo(skb)->frags[i];
			len = skb_frag_size(frag);
			frag_addr = skb_frag_address(frag);
			if (len <= (buf_size - copy_len)) {
				memcpy((u8 *)tmp_buf, (u8 *)frag_addr, len);
				tmp_buf += len;
				copy_len += len;
			} else {
				pr_err("%s:22:copied = %d\n",
				       __func__, copy_len);
				pr_err("remaining = %d and skb->head is %p\n",
				       len, skb->head);
				return CBM_FAILURE;
			}
		}
	} else {
		pr_err("%s:33:Cannot alloc CBM Buffer !\n", __func__);
		return CBM_FAILURE;
	}
	return CBM_SUCCESS;
}

/*Load Spreader Initialization*/
static void init_cbm_ls_port(int idx, void __iomem *cqm_ls_addr_base)
{
	int lsport = CBM_LS_PORT(idx, ctrl);
#ifdef CONFIG_CBM_LS_ENABLE
	/*if ((1 << idx) & g_cpu_port_alloc) {*/
	if (cpu_online(idx)) {
	/*Enable spreading only for port 0*/
#else
	if (!idx) {
#endif
		cbm_w32((LS_BASE + lsport), 0xF
			| LS_TH_SET
			| LS_OVFL_SET);
	} else {
		/* A0:Bug in counter_interrupt for ports 4-7
		 * so set the LS queue threshold to 1
		 */
		if (idx >= 4)
			cbm_w32((LS_BASE + lsport), 0xD
				| LS_TH_SET_A0
				| LS_OVFL_SET);
		else
			cbm_w32((LS_BASE + lsport), 0xD
				| LS_TH_SET
				| LS_OVFL_SET);
	}
}

void cbm_add_ls_port(int idx, int flag, void __iomem *cqm_ls_addr_base)
{
	int lsport = CBM_LS_PORT(idx, ctrl);

	if (flag) {
		cbm_w32((LS_BASE + lsport), 0xF
			| LS_TH_SET
			| LS_OVFL_SET);
	} else {
		cbm_w32((LS_BASE + lsport), 0xD
			| LS_TH_SET
			| LS_OVFL_SET);
	}
}

void cbm_ls_spread_alg_set(u32 alg, void __iomem *cqm_ls_addr_base)
{
	cbm_assert(alg < SPREAD_MAX, "illegal cbm load spread alg");
	set_val((LS_BASE + LS_SPR_CTRL), alg, LS_SPR_CTRL_SPR_SEL_MASK,
		LS_SPR_CTRL_SPR_SEL_POS);
}

void cbm_ls_port_weight_set(u32 port_id, u32 weight,
			    void __iomem *cqm_ls_addr_base)
{
	u32 pos, mask;

	cbm_assert(port_id < LS_PORT_NUM_MAX, "illegal cbm ls port id");

	pos  = LS_SPR_CTRL_WP0_POS + 2 * port_id;
	mask = (LS_SPR_CTRL_WP0_MASK >> LS_SPR_CTRL_WP0_POS) << pos;
	set_val((LS_BASE + LS_SPR_CTRL), weight, mask, pos);
}

void cbm_ls_flowid_map_set(u32 col, u32 val,
			   void __iomem *cqm_ls_addr_base)
{
	cbm_w32((LS_BASE + LS_FLOWID_MAP_COL0 + (col * 4)), val);
}

u32 cbm_ls_flowid_map_get(u32 col, void __iomem *ls_base)
{
	return cbm_r32(LS_BASE + LS_FLOWID_MAP_COL0 + (col * 4));
}

u32 cbm_ls_port_weight_get(u32 port_id,
			   void __iomem *cqm_ls_addr_base)
{
	u32 pos, mask;

	cbm_assert(port_id < LS_PORT_NUM, "illegal cbm ls port id");

	pos  = LS_SPR_CTRL_WP0_POS + 2 * port_id;
	mask = (LS_SPR_CTRL_WP0_MASK >> LS_SPR_CTRL_WP0_POS) << pos;
	return get_val(cbm_r32(LS_BASE + LS_SPR_CTRL), mask, pos);
}

void ls_intr_ctrl(u32 val, void __iomem *cqm_ls_addr_base)
{
	cbm_w32(LS_BASE + IRNEN_LS, val);
}

void init_cbm_ls(void __iomem *cqm_ls_addr_base, int port_num, int *skip)
{
	int i;
	unsigned long intr = 0;

	for (i = 0; i < port_num; i++) {
		if (skip[i] == 0)
			init_cbm_ls_port(i, LS_BASE);
	}

	cbm_ls_spread_alg_set(SPREAD_WRR, LS_BASE);
	for (i = 0; i < port_num; i++)
		cbm_ls_port_weight_set(i, DEFAULT_LS_PORT_WEIGHT, LS_BASE);

	for (i = 0; i < port_num; i++) {
		if (skip[i] == 0)
			intr |= (unsigned long)(0x3 << (IRNEN_LS_INT_PORT0_POS +
				(i * 2)));
	}
	cbm_w32((LS_BASE + IRNEN_LS), (intr & 0xffffffff));
	cbm_w32((LS_BASE + LS_GLBL_CTRL), (0x01 << LS_GLBL_CTRL_EN_POS));
}

void cbm_dw_memset(u32 *base, int val, u32 size)
{
	int i;

	for (i = 0; i < size; i++)
		base[i] = val;
}

int dp_dma_get_controller(char *ctrl)
{
	if (!strcmp(ctrl, "DMA0TX"))
		return DP_DMA0TX;
	else if (!strcmp(ctrl, "DMA1TX"))
		return DP_DMA1TX;
	else if (!strcmp(ctrl, "DMA2TX"))
		return DP_DMA2TX;
	else
		return DP_DMA_CTRL_MAX;
}

int cqm_dma_get_controller(char *ctrl)
{
	if (!strcmp(ctrl, "DMA1TX"))
		return DMA1TX;
	else if (!strcmp(ctrl, "DMA1RX"))
		return DMA1RX;
	else if (!strcmp(ctrl, "DMA2TX"))
		return DMA2TX;
	else if (!strcmp(ctrl, "DMA0TX"))
		return DMA0TX;
	else if (!strcmp(ctrl, "DMA2RX"))
		return 0;
	else if (!strcmp(ctrl, "DMA3"))
		return DMA3;
	else
		return DMAMAX;
}

const char *cqm_get_dma_engine_name(u32 ctl)
{
	switch (ctl) {
	case DMA2TX:
		return "DMA2TX";
	case DMA1RX:
		return "DMA1RX";
	case DMA1TX:
		return "DMA1TX";
	case DMA0TX:
		return "DMA0TX";
	case DMA3:
		return "DMA3";
	default:
		return NULL;
	}
}
