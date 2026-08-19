/*
 * Copyright (C) 2020-2024 MaxLinear, Inc.
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
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Description: PP descriptor definitions
 */

#ifndef __PP_DESC_H__
#define __PP_DESC_H__

#include <linux/types.h>
#include <linux/bitops.h>
#include <linux/pp_api.h>

#ifndef CONFIG_SOC_LGM
#define PP_DESC_PTR_SHIFT (0)
#else /* CONFIG_SOC_LGM */
#define PP_DESC_PTR_SHIFT (7)

/**
 * @define PP_MAX_DESC_UD_SZ
 * @brief descriptor size (16) including the UD tempate (32) and SI UD (16)
 */
#define PP_MAX_DESC_UD_SZ (64)

/**
 * @struct pp_hw_ud
 * @brief ud decriptor hw structure
 */
struct pp_hw_ud {
	u32 int_proto_info:4,
	    ext_proto_info:4,
	    rx_port:8,
	    ttl_exp:1,
	    ip_opt:1,
	    ext_df:1,
	    int_df:1,
	    ext_frag_type:2,
	    int_frag_type:2,
	    tcp_fin:1,
	    tcp_syn:1,
	    tcp_rst:1,
	    tcp_ack:1,
	    tcp_data_off:4;
	u32 ext_l3_off:8,
	    int_l3_off:8,
	    ext_l4_off:8,
	    int_l4_off:8;
	u32 ext_frag_off:8,
	    int_frag_off:8,
	    tdox_flow:10,
	    rsrv0:2,
	    lro:1,
	    rsrv1:1,
	    l2_off:2;
	u32 sess_id:24,
	    is_exc_sess:1,
	    rsrv2:3,
	    error:1,
	    drop:3;
	u32 hash_sig;
	u32 hash_h1:20,
	    hash_h2_low:12;
	u32 hash_h2_high:8,
	    l3_off_5:8,
	    l3_off_4:8,
	    l3_off_3:8;
	u32 l3_off_2:8,
	    l3_off_1:8,
	    l3_off_0:8,
	    tunn_off_id:3,
	    payld_off_id:3,
	    rsrv3:2;
} __attribute__((packed));

/**
 * @struct pp_hw_desc
 * @brief pp decriptor hw structure
 */
struct pp_hw_desc {
	u32 psb;             /*! protocol specific B                     */
	u32 psa:16,          /*! protocol specific A                     */
	    data_off:9,      /*! data_pointer = buff_ptr + data_off      */
	    pmac:1,          /*! 16B of PMAC header Part of Pre L2       */
	    color:2,         /*! color at egress                         */
	    src_pool:4;      /*! from which pool the buffer was taken    */
	u32 buff_ptr:29,     /*! buffer pointer (*128)                   */
	    pre_l2:2,        /*! preL2 size incl PMAC hdr                */
	    ts:1;            /*! indicate timestemp existence            */
	u32 tx_port:8,       /*! Egress Port @ RXDMA output              */
	    bm_policy:8,     /*! from which policy the buffer was taken  */
	    pkt_len:14,      /*! packet length. Defaults incl preL2 & ts */
	    lsp_pkt:1,       /*! last slow path packet, used for syncq   */
	    own:1;           /*! descriptor ownership                    */
	struct pp_hw_ud ud;
} __attribute__((packed));

/**
 * @brief Decode descriptor top hw buffer to host descriptor form
 * @param desc host descriptor to store the host form
 * @param hw_desc hw descriptor to decode
 * @return s32 0 on success, error code otherwise
 */
s32 pp_desc_top_decode(struct pp_desc *desc, const struct pp_hw_desc *hw_desc);

/**
 * @brief Decode descriptor hw buffer to host descriptor form
 * @param desc host descriptor to store the host form
 * @param hw_desc hw descriptor to decode
 * @return s32 0 on success, error code otherwise
 */
s32 pp_desc_decode(struct pp_desc *desc, const struct pp_hw_desc *hw_desc);

/**
 * @brief Dump dbg descriptor in host format
 * @param desc host descriptor
 * @return s32 0 on success, error code otherwise
 */
s32 pp_desc_top_dump(struct pp_desc *desc);

/**
 * @brief Dump descriptor in host format
 * @param desc host descriptor
 * @return s32 0 on success, error code otherwise
 */
s32 pp_desc_dump(struct pp_desc *desc);

#endif /* CONFIG_SOC_LGM */
#endif /* __PP_DESC_H__ */
