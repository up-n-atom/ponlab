// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2021, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#ifndef DATAPATH_TX_H
#define DATAPATH_TX_H

#include <net/datapath_api_tx.h>

struct pmac_port_info;
struct dp_subif_info;
struct pmac_tx_hdr;
struct dev_mib;

#define DP_TX_CTX_LOCK_T	spinlock_t
#define DP_TX_CTX_LOCK		spin_lock_bh
#define DP_TX_CTX_UNLOCK	spin_unlock_bh
#define DP_TX_CTX_LOCK_INIT	spin_lock_init

/**
 * struct tx_entry - hook setup
 * @cb: callback function
 * @priv: data
 */
struct tx_entry {
	tx_fn cb;
	void *priv;
};

/**
 * struct tx_stored - entries been set
 * @process: external process
 * @preprocess: internal preprocess
 * @preprocess_always_enabled: process even when hook is not registered
 */
struct tx_stored {
	struct tx_entry process;
	struct tx_entry preprocess;
	bool preprocess_always_enabled;
};

/**
 * struct tx_hook_list - datapath TX call chain
 * @rcu_head: used by RCU
 * @entry: TX entry for preprocess and process sorted by priority
 * @cnt: number of entry in the list
 */
struct tx_hook_list {
	struct rcu_head rcu_head;
	struct tx_entry entry[DP_TX_CNT * 2];
	u8 cnt;
};

/**
 * struct dp_tx_context - datapath TX runtime
 * @lock: Lock to pretect concurrent update
 * @hook_list: TX entry list for reader
 * @stored: TX information from updater
 */
struct dp_tx_context {
	DP_TX_CTX_LOCK_T lock;
	struct tx_hook_list __rcu *hook_list;
	struct tx_stored stored[DP_TX_CNT];
};

/**
 * struct dp_checksum_info - datapath checksum info to pass to GSWIP HW
 */
/*! @brief datapath checksum parameters */
struct dp_checksum_info {
	u32 tcp_h_offset;
	u32 ip_offset;
	u32 tcp_type;
};

struct dp_tx_common_ex {
	struct dp_tx_common cmn;
	struct net_device *dev;	  	  /*!< net device */
	struct dev_mib *mib;          /*!< mib info */
	struct pmac_port_info *port;  /*!< port info */
	struct dp_subif_info *sif;    /*!< subif info */
	dp_subif_t *rx_subif;		  /*!< rx_subif */
	enum DP_TEMP_DMA_PMAC tmpl;   /*!< DMA Pmac Template */
	struct dp_checksum_info csum_info;      /*!< DP checksum parameters */
};

int dp_tx_ctx_init(int inst);

void dp_tx_register_process(enum DP_TX_PRIORITY priority, tx_fn fn,
			    void *priv);

void dp_tx_register_preprocess(enum DP_TX_PRIORITY priority, tx_fn fn,
			       void *priv, bool always_enabled);

int dp_tx_update_list(void);

int dp_tx_start(struct sk_buff *skb, struct dp_tx_common *cmn);

void dp_tx_dbg(char *title, struct sk_buff *skb, struct dp_tx_common_ex *ex);

static inline struct pmac_tx_hdr *
dp_tx_get_pmac(const struct dp_tx_common_ex *ex)
{
	return (struct pmac_tx_hdr *)ex->cmn.pmac;
}

static inline u8 dp_tx_get_pmac_len(const struct dp_tx_common_ex *ex)
{
	return ex->cmn.pmac_len;
}

#endif /* end of include guard: DATAPATH_TX_H */
