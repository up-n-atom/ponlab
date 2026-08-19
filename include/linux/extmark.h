#ifndef _LINUX_EXTMARK_H
#define _LINUX_EXTMARK_H
/*
 *	xt_extmark - Netfilter module to match NFEXTMARK value
 *
 *	(C) 1999-2001 Marc Boucher <marc@mbsi.ca>
 *	Copyright © CC Computer Consultants GmbH, 2007 - 2008
 *	Jan Engelhardt <jengelh@medozas.de>
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2 as
 *	published by the Free Software Foundation.
 */

/* Bit counting starts from Least significant bit, 0..31*/

#define CPEID_START_BIT_POS 0
#define CPEID_NO_OF_BITS 5
#define CPEID_MASK 0x1f

#define QUEPRIO_START_BIT_POS 5
#define QUEPRIO_NO_OF_BITS 5
#define QUEPRIO_MASK 0x3e0

#define SUBQUEUE_START_BIT_POS 10
#define SUBQUEUE_NO_OF_BITS 3
#define SUBQUEUE_MASK 0x1c00

#define ACCELSEL_START_BIT_POS 13
#define ACCELSEL_NO_OF_BITS 1
#define ACCELSEL_MASK 0x2000

#define VPRIO_START_BIT_POS 14
#define VPRIO_NO_OF_BITS 3
#define VPRIO_MASK 0x1c000

#define VLANID_START_BIT_POS 17
#define VLANID_NO_OF_BITS 12
#define VLANID_MASK 0x1ffe0000

#define PPE_START_BIT_POS 29
#define PPE_NO_OF_BITS 1
#define PPE_MASK 0x20000000

#define FILTERTAP_START_BIT_POS 31
#define FILTERTAP_NO_OF_BITS 1
#define FILTERTAP_MASK 0x80000000

#define GET_DATA_FROM_MARK_OPT(mark, mask, pos, value) \
	(value = ((mark & mask) >> pos))
#define SET_DATA_FROM_MARK_OPT(mark, mask, pos, value) \
	do { \
		mark &= ~mask; \
		mark |= ((value << pos) & mask); \
	} while (0)

#define MASK(pos, len) (((1<<len)-1)<<pos)
#define GET_DATA_FROM_MARK(mark, pos, len, value) \
	GET_DATA_FROM_MARK_OPT(mark, MASK(pos, len), pos, value)
#define SET_DATA_FROM_MARK(mark, pos, len, value) \
	SET_DATA_FROM_MARK_OPT(mark, MASK(pos, len), pos, value)


#ifdef CONFIG_NETWORK_EXTMARK
#ifdef CONFIG_MXL_SKB_EXT_EXTMARK
#include <soc/mxl/mxl_skb_ext_extmark.h>

static inline u32 skb_extmark_get(const struct sk_buff *skb)
{
	return skb_extmark_ext_get(skb);
}

static inline s32 skb_extmark_set(struct sk_buff *skb, u32 extmark, u32 mask)
{
	return skb_extmark_ext_set(skb, extmark, mask);
}
#else
static inline u32 skb_extmark_get(const struct sk_buff *skb)
{
	return skb->extmark;
}

static inline s32 skb_extmark_set(struct sk_buff *skb, u32 extmark, u32 mask)
{
	SET_DATA_FROM_MARK_OPT(skb->extmark, mask, 0, extmark);
	return 0;
}
#endif /* CONFIG_MXL_SKB_EXT_EXTMARK */
#else
static inline u32 skb_extmark_get(const struct sk_buff *skb)
{
	return 0;
}

static inline s32 skb_extmark_set(struct sk_buff *skb, u32 extmark, u32 mask)
{
	return -EOPNOTSUPP;
}
#endif /* CONFIG_NETWORK_EXTMARK */

#endif
