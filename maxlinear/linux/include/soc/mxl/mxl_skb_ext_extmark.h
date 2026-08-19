/*
 * Copyright (C) 2023-2024 MaxLinear, Inc.
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
 * Description: SKB extmark over MXL SKB Extension
 */

#ifndef __MXL_SKB_EXT_EXTMARK_H__
#define __MXL_SKB_EXT_EXTMARK_H__

#ifdef CONFIG_MXL_SKB_EXT_EXTMARK
extern u32 skb_extmark_ext_get(const struct sk_buff *skb);
extern s32 skb_extmark_ext_set(struct sk_buff *skb, u32 extmark, u32 mask);
#else
static inline u32 skb_extmark_ext_get(const struct sk_buff *skb)
{
	return 0;
}

static inline s32 skb_extmark_ext_set(struct sk_buff *skb, u32 extmark, u32 mask)
{
	return -EOPNOTSUPP;
}
#endif /* CONFIG_MXL_SKB_EXT_EXTMARK */

#endif /* __MXL_SKB_EXT_EXTMARK_H__ */
