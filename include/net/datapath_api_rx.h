// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2024 MaxLinear, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _DATAPATH_API_RX_H
#define _DATAPATH_API_RX_H
#include <linux/bits.h>
#include <linux/types.h>
#include <linux/netdevice.h>
#include <net/datapath_api.h>

struct sk_buff;

/*! @{ */
/*!
 * @file datapath_api_rx.h
 *
 * @brief Datapath RX path API
 */
/*! @} */

/**
 * enum DP_RX_HOOK_RET - define RX hook return values flag
 */
enum DP_RX_HOOK_RET {
	DP_RX_HOOK_FN_CONSUMED = -1,	/*!< callback return value:
				  	  hook callback consumed the skb and freed => stop process the further hooks
				  	  */
	DP_RX_HOOK_FN_DROPPED = -2,	/*!< callback return value:
					  hook callback dropped the skb and freed => stop process the further hooks
					  */
	DP_RX_HOOK_FN_CONTINUE = 0, 	/*!< callback return value:
					  to continue processing next hooks
					  */
};

/**
 * enum DP_RX_HOOK_PRIORITY - define RX chain priority from high to low
 */
/*! @brief
 * define RX chain priority from DP_RX_HOOK_PRI_FIRST is highest
 * priority, first execution. and DP_RX_HOOK_PRI_LAST is lowest prio,
 * executes at the last
 */
enum DP_RX_HOOK_PRIORITY {
	DP_RX_HOOK_PRI_FIRST = INT_MIN,
	DP_RX_HOOK_PRI_NORMAL = 0,
	DP_RX_HOOK_PRI_LAST = INT_MAX,
};

#define DP_RX_PRIORITY_LIST { \
	DP_F_ENUM_OR_STRING(DP_RX_HOOK_PRI_FIRST, "DP_RX_HOOK_PRI_FIRST"), \
	DP_F_ENUM_OR_STRING(DP_RX_HOOK_PRI_NORMAL, "DP_RX_HOOK_PRI_NORMAL"), \
	DP_F_ENUM_OR_STRING(DP_RX_HOOK_PRI_LAST, "DP_RX_HOOK_PRI_LAST"), \
}

/**
 * struct dp_rx_common - datapath RX callback parameters
 */
/*! @brief datapath RX callback parameters */
struct dp_rx_common {
	u8 inst;                      /*!< inst */
	u8 portid;                    /*!< datapath portid */
	int alloc_flags;              /*!< bitmap of enum DP_F_FLAG */
	u32 subif;                    /*!< subif id */
	void *user_data;              /*!< store any user given pointer for data */
};

/**
 * dp_rx_hook_fn_t() - callback prototype for RX path
 * @param[in,out] skb: skb to process
 * @param[in,out] cmn: common data which is persist across the call chain
 *
 * @Return DP_RX_HOOK_FN_CONSUMED - skb consumed, skb freed by user, stop further process;
 *         DP_RX_HOOK_FN_DROPPED  - skb dropped,  skb freed by user, stop further process;
 *         DP_RX_HOOK_FN_CONTINUE - skb not consumed, not freed by user/cb yet, continue for further process;
 *         others - skb not consumed with error code, stop further process
 */
typedef int32_t(*dp_rx_hook_fn_t)(struct net_device *rxif, struct net_device *txif,
	struct sk_buff *skb, int32_t len, struct dp_rx_common *cmn);

/**
 * dp_register_rx_hook() - register datapath rx path hook function
 * @priority: RX priority
 * @dp_rx_hook_fn_t: callback function, which must be given at registration
 * @p: parameters
 */
int dp_register_rx_hook(int priority, dp_rx_hook_fn_t fn, void *p);

/**
 * dp_deregister_rx_hook() - deregister datapath rx path hook function
 * @priority: RX priority
 * @dp_rx_hook_fn_t: callback function
 */
int dp_deregister_rx_hook(int priority, dp_rx_hook_fn_t fn);

#endif /* end of include guard: _DATAPATH_API_RX_H */
