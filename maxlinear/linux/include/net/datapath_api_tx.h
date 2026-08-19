// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2020 Intel Corporation.
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

#ifndef _DATAPATH_API_TX_H
#define _DATAPATH_API_TX_H
#include <linux/bits.h>
#include <linux/types.h>
#include <linux/netdevice.h>

struct sk_buff;
#define PMAC_TX_HDR_SIZE (sizeof(struct pmac_tx_hdr))

/*! @{ */
/*!
 * @file datapath_api_tx.h
 *
 * @brief Datapath TX path API
 */
/*! @} */

/**
 * enum DP_TX_FN_RET - define TX return value for dp_xmit() with DP_TX_NEWRET
 * flag
 */
/*! @brief define TX return value for dp_xmit() with DP_TX_NEWRET flag */
enum DP_TX_FN_RET {
	DP_TX_FN_CONSUMED = NETDEV_TX_OK,
	DP_TX_FN_DROPPED = NET_XMIT_DROP,
	DP_TX_FN_BUSY = NETDEV_TX_BUSY,
	/* DP_TX_FN_CONTINUE is an internal state, will not be returned to
	 * dp_xmit() */
	DP_TX_FN_CONTINUE = INT_MAX,
};

/**
 * enum DP_TX_PRIORITY - define TX chain priority from high to low
 */
/*! @brief define TX chain priority from high to low */
enum DP_TX_PRIORITY {
	DP_TX_PP, /*!< packet processor */
	DP_TX_UDP_CKSUM_WA, /*!< UDP cksum wa for payload < 8*/
	DP_TX_VPNA, /*!< VPN adapter */
	DP_TX_TSO, /*!< traffic offload engine */
	DP_TX_CQM,  /*!< CQM CPU port */
	DP_TX_CNT,
};

#define DP_TX_PRIORITY_LIST { \
	DP_F_ENUM_OR_STRING(DP_TX_PP, "DP_TX_PP"), \
	DP_F_ENUM_OR_STRING(DP_TX_UDP_CKSUM_WA, "DP_TX_UDP_CKSUM_WA"), \
	DP_F_ENUM_OR_STRING(DP_TX_VPNA, "DP_TX_VPNA"), \
	DP_F_ENUM_OR_STRING(DP_TX_TSO, "DP_TX_TSO"), \
	DP_F_ENUM_OR_STRING(DP_TX_CQM, "DP_TX_CQM") \
}

/**
 * enum DP_TX_FLAGS - define TX path common flags
 */
/*! @brief define TX path common flags */
enum DP_TX_FLAGS {
	DP_TX_FLAG_NONE       = BIT(0), /*!< @brief enable HW checksum offload */
	DP_TX_CAL_CHKSUM      = BIT(1), /*!< @brief enable HW checksum offload */
	DP_TX_DSL_FCS         = BIT(2), /*!< @brief enable DSL FCS checksum offload */
	DP_TX_BYPASS_QOS      = BIT(3), /*!< @brief bypass QoS */
	DP_TX_BYPASS_FLOW     = BIT(4), /*!< @brief bypass DMA descriptor filling flow */
	DP_TX_OAM             = BIT(5), /*!< @brief OAM packet */
	DP_TX_TO_DL_MPEFW     = BIT(6), /*!< @brief Send pkt directly to DL FW */
	DP_TX_INSERT          = BIT(7), /*!< @brief GSWIP insertion Support */
	DP_TX_INSERT_POINT    = BIT(8), /*!< @brief For GSWIP insertion Point 0 or 1 */
	DP_TX_WITH_PMAC       = BIT(9), /*!< @brief caller already put pmac in
					*           the skb->data-16 to skb->data-1
					*/
	DP_TX_NEWRET          = BIT(10), /*!< @brief dp_xmit() returns enum DP_TX_FN_RET */
};

/**
 * struct dp_tx_common - datapath TX callback parameters
 * @flags: bitmap of DP_TX_FLAGS
 * @private: private value from dp_register_txpath()
 */
/*! @brief datapath TX callback parameters */
struct dp_tx_common {
	u32 subif;                    /*!< subif id */
	u32 flags;                    /*!< bitmap of enum DP_TX_FLAGS */
	int alloc_flags;              /*!< bitmap of enum DP_F_FLAG */
	u8 pmac[PMAC_TX_HDR_SIZE];    /*!< PMAC */
	u8 pmac_len;                  /*!< length of PMAC */
	u8 tx_portid;                 /*!< tx portid */
	u8 inst;                      /*!< inst */
	u8 toe_tc;                    /*!< TOE class max class id */
};

/**
 * tx_fn() - callback prototype for TX path
 * @param[in,out] skb: skb to process
 * @param[in,out] cmn: common data which is persist across the call chain
 * @param[in,out] p: private parameter passed in from dp_register_tx()
 *
 * @Return DP_TX_FN_CONSUMED - skb consumed, stop further process;
 *         DP_TX_FN_CONTINUE skb - not consumed, continue for further process;
 *         others - skb not consumed with error code, stop further process
 */
typedef int (*tx_fn)(struct sk_buff *skb, struct dp_tx_common *cmn, void *p);

/**
 * dp_register_tx() - register datapath tx path function
 * @priority: TX priority
 * @tx_fn: callback function
 * @p: parameters
 */
int dp_register_tx(enum DP_TX_PRIORITY priority, tx_fn fn, void *p);

#endif /* end of include guard: _DATAPATH_API_TX_H */
