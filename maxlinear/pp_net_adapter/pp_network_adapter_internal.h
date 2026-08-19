/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2020 Intel Corporation
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
 * Description: PPv4 Netowrk adapter Internal Definitions
 */

#ifndef __PP_NETWORK_ADAPTER_INTERNAL_H__
#define __PP_NETWORK_ADAPTER_INTERNAL_H__

#include <linux/types.h>
#include <linux/pp_api.h>

#define DUT_HW_DESC_SIZE_WORDS (14)

/**
 * @define the two frag currently supported by test
 */
enum dut_frags {
	FRAG_0 = 0,
	FRAG_1 = 1,
	FRAG_NUM = 2
};

/**
 * @define Check if a number is an odd number
 * @param num the number
 */
#define num_is_odd(num)                ((num) & (typeof(num))BIT(0))

/**
 * @define Check if a number is an even number
 * @param num the number
 */
#define num_is_even(num)               !num_is_odd(num)

/**
 * @brief Shortcut for checking if pointer is null
 */
#define ptr_is_null(ptr)               __ptr_is_null(ptr, #ptr, __func__)

/**
 * @brief Shortcut for checking if pointer is null and print error
 *        message in case he is which includes the parameter name and
 *        the caller function name
 * @param ptr pointer to check
 * @param ptr_name pointer variable name for error print
 * @return bool true if the pointer is null, false otherwise
 */
static inline bool __ptr_is_null(const void *ptr, const char *name,
				 const char *caller)
{
	if (likely(ptr))
		return false;

	pr_err("%s: '%s' is null\n", caller, name);
	return true;
}

/**
 * @define Test whether PP event is valid
 * @param ev the event to check
 */
#define PP_IS_EVENT_VALID(ev) \
	(ev >= 0 && ev < PP_EVENTS_NUM)

/**
 * @brief Test whether PP event is valid, print an error message in
 *        case it is not valid
 * @param ev the event
 * @return bool true in case the event is valid, false otherwise
 */
static inline bool __pp_is_event_valid(enum pp_event ev)
{
	if (likely(PP_IS_EVENT_VALID(ev)))
		return true;

	pr_err("Invalid PP event id %u\n", ev);
	return false;
}

#if defined(CONFIG_DEBUG_FS) && (defined(CONFIG_SOC_LGM) || defined(CONFIG_PPV4_LGM))

/**
 * @brief Net adapter debug init
 * @return s32 0 for success, non-zero otherwise
 */
s32 net_adapter_dbg_init(void);

/**
 * @brief Net adapter debug cleanup
 * @return s32 0 for success, non-zero otherwise
 */
s32 net_adapter_dbg_clean(void);

/**
 * @brief dut device port initialize
 * @return s32 0 for success, non-zero otherwise
 */
s32 dut_port_init(void);

/**
 * @brief set dut ingress packet header
 * @return None
 */
void dut_dbg_pkt_set(u8 index, unsigned char *pkt, size_t pkt_size, bool is_tdox, u16 sessions);

/**
 * @brief get descriptor from first packet
 * @return None
 */
void dut_pkt_desc_get(unsigned char *desc);

#else

static inline s32 net_adapter_dbg_init(void)
{
	return 0;
}

static inline s32 net_adapter_dbg_clean(void)
{
	return 0;
}
#endif
#endif /* __PP_NETWORK_ADAPTER_INTERNAL_H__ */
