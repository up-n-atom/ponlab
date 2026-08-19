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
 * Description: PP descriptor module
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/types.h>
#include "pp_common.h"
#include "pp_fv.h"
#include "pp_desc.h"

#ifdef pr_fmt
#undef pr_fmt
#define pr_fmt(fmt) "[PP_DESC]: " fmt
#endif

/**
 * @brief Decode UD hw buffer to host UD form
 * @param ud host UD descriptor to store the host form
 * @param desc hw descriptor to decode
 * @return s32 0 on success, error code otherwise
 */
static s32 __desc_ud_decode(struct pp_pkt_ud *ud, const struct pp_hw_desc *desc)
{
	if (ptr_is_null(ud) || ptr_is_null(desc))
		return -EINVAL;

	ud->rx_port = desc->ud.rx_port;
	ud->sess_id = desc->ud.sess_id;
	ud->is_exc_sess = desc->ud.is_exc_sess;
	ud->hash_sig = desc->ud.hash_sig;
	ud->hash_h1 = desc->ud.hash_h1;
	ud->hash_h2 = (desc->ud.hash_h2_high << 12) | desc->ud.hash_h2_low;

	return 0;
}

s32 pp_desc_top_decode(struct pp_desc *desc, const struct pp_hw_desc *hw_desc)
{
	if (ptr_is_null(desc) || ptr_is_null(hw_desc))
		return -EINVAL;

	desc->ps        = hw_desc->psb;
	desc->data_off  = hw_desc->data_off;
	desc->color     = hw_desc->color;
	desc->src_pool  = hw_desc->src_pool;
	desc->buff_ptr  = hw_desc->buff_ptr << PP_DESC_PTR_SHIFT;
	desc->tx_port   = hw_desc->tx_port;
	desc->bm_policy = hw_desc->bm_policy;
	desc->pkt_len   = hw_desc->pkt_len;
	desc->lsp_pkt   = hw_desc->lsp_pkt;

	return 0;
}

s32 pp_desc_decode(struct pp_desc *desc, const struct pp_hw_desc *hw_desc)
{
	if (ptr_is_null(desc) || ptr_is_null(hw_desc))
		return -EINVAL;

	pp_desc_top_decode(desc, hw_desc);
	return __desc_ud_decode(&desc->ud, hw_desc);
}

s32 pp_desc_top_dump(struct pp_desc *desc)
{
	if (ptr_is_null(desc))
		return -EINVAL;

	pr_info("\n");
	pr_info("DESCRIPTOR:\n");
	pr_info(" tx port           : %hhu\n", (u8)desc->tx_port);
	pr_info(" packet len        : %hu\n", (u16)desc->pkt_len);
	pr_info(" buff addr         : %#llx\n", (u64)desc->buff_ptr);
	pr_info(" data offset       : %hu\n", (u16)desc->data_off);
	pr_info(" PS                : %#llx\n", (u64)desc->ps);
	pr_info(" src pool          : %llu\n", (u64)desc->src_pool);
	pr_info(" bm policy         : %hhu\n", (u8)desc->bm_policy);
	pr_info(" last slow path pkt: %s\n", BOOL2STR(desc->lsp_pkt));
	pr_info(" color             : %s\n", PP_COLOR_TO_STR(desc->color));
	pr_info("\n");

	return 0;
}

s32 pp_desc_dump(struct pp_desc *desc)
{
	struct pp_pkt_ud *ud;

	if (ptr_is_null(desc))
		return -EINVAL;

	ud = &desc->ud;
	pp_desc_top_dump(desc);

	pr_info("PACKET UD (Depend on SI UD enablement):\n");
	pr_info(" rx port           : %hhu\n", (u8)ud->rx_port);
	pr_info(" session id        : %u\n", (u32)ud->sess_id);
	pr_info(" exception session : %s\n", BOOL2STR(ud->is_exc_sess));
	pr_info(" SIG hash          : %#x\n", (u32)ud->hash_sig);
	pr_info(" H1 hash           : %#x\n", (u32)ud->hash_h1);
	pr_info(" H2 hash           : %#x\n", (u32)ud->hash_h2);
	pr_info(" data offset       : %llu\n", (u64)desc->data_off);
	pr_info("\n");

	return 0;
}