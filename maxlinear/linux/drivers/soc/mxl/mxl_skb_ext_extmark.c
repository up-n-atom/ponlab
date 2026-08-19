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

#define pr_fmt(fmt) "[MXL_SKB_EXT_EXTMARK]:%s:%d: " fmt, __func__, __LINE__

#include <linux/skbuff.h>
#include <soc/mxl/mxl_skb_ext.h>

struct extmark_ext {
	u32 extmark;
};

static int32_t extmark_ext_id = MXL_SKB_EXT_INVALID;

u32 skb_extmark_ext_get(const struct sk_buff *skb)
{
	struct extmark_ext *ext = mxl_skb_ext_find(skb, extmark_ext_id);

	return ext ? ext->extmark : 0;
}
EXPORT_SYMBOL(skb_extmark_ext_get);

s32 skb_extmark_ext_set(struct sk_buff *skb, u32 extmark, u32 mask)
{
	struct extmark_ext *ext = mxl_skb_ext_find(skb, extmark_ext_id);

	if (!ext) {
		/* no ext, allocate a new one */
		ext = mxl_skb_ext_add(skb, extmark_ext_id);
		if (!ext) {
			pr_debug("%s: extmark_ext add error!\n", __func__);
			return -1;
		}
		ext->extmark = 0;
	}

	ext->extmark &= ~mask;
	ext->extmark |= (extmark & mask);
	return 0;
}
EXPORT_SYMBOL(skb_extmark_ext_set);

static int __init mxl_skb_ext_extmark_init(void)
{
	pr_debug("init mxl_skb_extmark\n");

	extmark_ext_id =
		mxl_skb_ext_register("extmark_ext", sizeof(struct extmark_ext));
	if (extmark_ext_id == MXL_SKB_EXT_INVALID) {
		pr_info("mxl_skb_ext_register failed %s %d\n", __func__, __LINE__);
		return -EINVAL;
	}

	return 0;
}
arch_initcall(mxl_skb_ext_extmark_init);
