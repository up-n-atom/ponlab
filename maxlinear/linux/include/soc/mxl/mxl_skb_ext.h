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
 * Description: MXL SKB Extension
 */

#ifndef __MXL_SKB_EXT_H__
#define __MXL_SKB_EXT_H__

#include <linux/bitops.h>

typedef unsigned long mxl_skb_ext_bmap;
#define MXL_SKB_EXTS_MAX BITS_PER_TYPE(mxl_skb_ext_bmap)
#define MXL_SKB_EXT_INVALID MXL_SKB_EXTS_MAX

/* Forward declaration for sk_buff structure */
struct sk_buff;

#if IS_ENABLED(CONFIG_MXL_SKB_EXT)
/**
 * @brief register to mxl_skb_ext services
 * @param name requester name
 * @param size extension size
 * @return ext_id, or MXL_SKB_EXT_INVALID in case of error
 * @note the registration is permanent till system reboot
 */
unsigned int mxl_skb_ext_register(const char *name, unsigned short size);

/**
 * @brief get ext_id by requester name
 * @param name requester name
 * @return ext_id, or MXL_SKB_EXT_INVALID in case of error
 */
unsigned int mxl_skb_ext_get_ext_id(const char *name);

/**
 * @brief returns pointer to store the ext_id's extension, the block size is the
 * 	  size which the ext_id's gave in the register process
 * @param skb skbuff pointer
 * @param ext_id ext_id
 * @return pointer to extension, or NULL in case of error
 * @note the pointer should hold only static data and not pointers
 * @note in case the skb already has extension from the same ext_id, the pointer
 * 	 still point to the same location
 */
void *mxl_skb_ext_add(struct sk_buff *skb, unsigned int ext_id);

/**
 * @brief get ext_id's extension from the skb
 * @param skb skbuff pointer
 * @param ext_id ext_id
 * @return pointer to extension, or NULL in case of error
 */
void *mxl_skb_ext_find(const struct sk_buff *skb, unsigned int ext_id);

/**
 * @brief delete ext_id's extension from the skb
 * @param skb skbuff pointer
 * @param ext_id ext_id
 */
void mxl_skb_ext_del(const struct sk_buff *skb, unsigned int ext_id);

#else
static inline unsigned int mxl_skb_ext_register(const char *name,
						unsigned short size)
{
	return 0;
}

static inline unsigned int mxl_skb_ext_get_ext_id(const char *name)
{
	return 0;
}

static inline void *mxl_skb_ext_add(struct sk_buff *skb, unsigned int ext_id)
{
	return NULL;
}

static inline void *mxl_skb_ext_find(const struct sk_buff *skb,
				     unsigned int ext_id)
{
	return NULL;
}

static inline void mxl_skb_ext_del(const struct sk_buff *skb,
				   unsigned int ext_id)
{
}
#endif /* IS_ENABLED(CONFIG_MXL_SKB_EXT) */
#endif /* __MXL_SKB_EXT_H__ */
