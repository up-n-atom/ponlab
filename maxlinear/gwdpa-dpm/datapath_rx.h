// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2024, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#ifndef DATAPATH_RX_H
#define DATAPATH_RX_H

#include <net/datapath_api_rx.h>

#define DP_RX_CTX_LOCK_T	spinlock_t
#define DP_RX_CTX_LOCK		spin_lock_bh
#define DP_RX_CTX_UNLOCK	spin_unlock_bh
#define DP_RX_CTX_LOCK_INIT	spin_lock_init

#define DP_RX_HOOK_MAX_ENTRIES     64

/**
 * struct rx_entry - hook setup
 * @cb: callback function
 * @priv: data
 * @delete: flag for delete operation
 */
struct rx_entry {
	int prio;
	dp_rx_hook_fn_t cb;
	void *priv;
};

/**
 * struct rx_hook_list - datapath RX call chain
 * @rcu_head: used by RCU
 * @entry: RX entry for process sorted by priority
 * @cnt: number of entry in the list
 */
struct rx_hook_list {
	struct rcu_head rcu_head;
	u8 cnt;
	struct rx_entry *entry_list;
};

/**
 * struct dp_rx_context - datapath RX runtime
 * @lock: Lock to protect concurrent update
 * @hook_list: RX entry list for reader
 * @stored: RX information from updater
 */
struct dp_rx_context {
	DP_RX_CTX_LOCK_T lock;
	struct rx_hook_list __rcu *hook_list;
};

int dp_rx_ctx_init(int inst);

int dp_rx_update_list(struct rx_entry *entry, u8 del_flag);

struct dp_rx_hook_data { /*dpm rx_hook internal structure*/
        struct dp_rx_common rx_cmn;
        dp_rx_fn_t rx_fn;             /*!< subif rx_fn callback given at subif registration */
};

int dp_rx_hook_call(struct net_device *rxif, struct net_device *txif,
		struct sk_buff *skb, struct dp_rx_hook_data *hd);

#endif /* end of include guard: DATAPATH_RX_H */
