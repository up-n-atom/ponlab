/* SPDX-License-Identifier: GPL-2.0 */
/* 
 * Copyright (C) 2020-2022 MaxLinear, Inc. 
 * Copyright (C) 2018-2020 Intel Corporation 
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

#ifndef _DATAPATH_UMT_H_
#define _DATAPATH_UMT_H_

/* UMT feature flag */
#define UMT_SND_DIS			BIT(0)
#define UMT_CLEAR_CNT			BIT(1)
#define UMT_NOT_SND_ZERO_CNT		BIT(2)
#define UMT_SND_RX_CNT_ONLY		BIT(3)
#define UMT_F_MAX			UMT_SND_RX_CNT_ONLY

#define UMT_DST_ADDR_MAX	11

enum umt_rx_src {
	UMT_RX_SRC_CQEM, /* RX count from CQM */
	UMT_RX_SRC_DMA,  /* RX count from DMA */
};

enum umt_msg_mode {
	UMT_MSG_SELFCNT,   /* HW count mode */
	UMT_MSG_USER_MODE, /* User count mode */
};

enum umt_sw_msg {
	UMT_NO_MSG,
	UMT_MSG0_ONLY,
	UMT_MSG1_ONLY,
	UMT_MSG0_MSG1,
	UMT_MSG4 = 0xF,
};

enum umt_cnt_mode {
	UMT_CNT_INC, /* Incremental count */
	UMT_CNT_ACC, /* Accumulate count */
};

enum umt_rx_msg_mode {
	UMT_RXOUT_MSG_SUB, /* RX OUT SUB mode */
	UMT_RXIN_MSG_ADD,   /* RX IN Add mode */
	UMT_4MSG_MODE,
};

struct umt_port_ctl {
	int                  id;	  /* UMT Port ID set by dpm or top
					   * level drivers, like dcdp driver
					   */
	u32                  dst_addr_cnt; /* MSG ADDR count set by top level driver */
	dma_addr_t           daddr;	  /* Legacy MSG ADDR set by top level driver.
					   * It may be deprecated and should no longer
					   * be used.
					   */
	dma_addr_t           dst[UMT_DST_ADDR_MAX]; /* MSG ADDR set by top level driver */
	u32                  msg_interval;/* MSG interval set by top level driver */
	enum umt_msg_mode    msg_mode;    /* set by top level drivers */
	enum umt_cnt_mode    cnt_mode;    /* set by top level drivers */
	enum umt_sw_msg      sw_msg;	  /* SW mode cfg set by top level drivers */
	enum umt_rx_msg_mode rx_msg_mode; /* set by top level drivers */
	unsigned int         enable;      /* set by top level drivers */
	unsigned long        fflag;	  /* UMT feature flag set by top level drivers */
	/**
	 * none applicable src: RXIN/TXIN/RXOUT/TXOUT
	 *                      -Bit0-Bit1--Bit2--Bit3-
	 * set to 1 means using none applicable src to overwrite UMT port mux setting
	 * set to 0 means no overwritten
	 * set by top level network drivers
	 */
	u32                  na_src;  
	u32                  msg_interval_div;/* MSG interval divisor set by top level drivers
					       * and used as right-shift value for ACA txin
					       * interval (higher PPS)
					       */
};

struct umt_port_res {
	u32               dma_id;      /* set by dpm */
	unsigned int      dma_ch_num;  /* set by dpm */
	unsigned int      cqm_enq_pid; /* set by dpm */
	unsigned int      cqm_dq_pid;  /* set by dpm */
	enum umt_rx_src   rx_src;      /* set by top level network drivers */
};

struct dp_umt_port {
	struct umt_port_ctl ctl; /* User input info, most set by top level drivers */
	struct umt_port_res res; /* some set by dpm, some by top level drivers */
};

struct umt_ops {
	struct device *umt_dev;
	int (*umt_alloc)(struct device *umt_dev, struct dp_umt_port *port);
	int (*umt_request)(struct device *umt_dev, struct dp_umt_port *port);
	int (*umt_enable)(struct device *umt_dev, unsigned int id, bool en);
	int (*umt_set_ctrl)(struct device *umt_dev, unsigned int id,
			    unsigned long flag_mask, unsigned long vflag);
	int (*umt_release)(struct device *umt_dev, unsigned int id);
};

#endif /* _DATAPATH_UMT_H_ */
