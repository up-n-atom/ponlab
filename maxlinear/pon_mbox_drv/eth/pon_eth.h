/******************************************************************************
 *
 * Copyright (c) 2020 - 2025 MaxLinear, Inc.
 * Copyright (C) 2019 - 2020 Intel Corporation
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 ******************************************************************************/

/**
 *  \file
 *  This is the PON Ethernet driver header file, used for internal definitions.
 */

#ifndef __PON_ETH_H
#define __PON_ETH_H

/** \addtogroup PON_ETH PON Ethernet Driver
 *  @{
 */

#include <linux/spinlock.h>

/** Reserve the required number of queues for PON port */
#define DEFAULT_NUM_OF_QUEUES 64
/** Reserve the required number of schedulers for PON port */
#define DEFAULT_NUM_OF_SCHEDULERS 64
/** Set to this value in case no queue is assigned by the datapath lib */
#define PON_ETH_QUEUE_UNASSIGNED -1

/** SoC-specific data. */
struct ltq_pon_net_soc_data {
	bool pmac_remove;
};

/** This represents the PON IP hardware block, one instance per PON IP HW. */
struct ltq_pon_net_hw {
	struct device *dev;
	struct net_device *ndev;
	/** IP Port Netdevice */
	struct net_device *ipp_ndev;
	u32 dp_mode_registered;
	int port_id;
	/** IP Port Id */
	int ipp_id;
	/** IP Port owner on URX */
	struct module *ipp_owner;
	/* true when the system is in O5 or O6 */
	atomic_t in_o5;
	const struct ltq_pon_net_soc_data *soc_data;
	struct list_head pmapper_list;
	struct list_head tcont_list;
	struct list_head gem_list;
	struct list_head iphost_list;
	/* locking for iphost_list */
	spinlock_t iphost_lock;
	/** Queue lookup mode.
	 *  7 6 5 4 3 2 1 0
	 * +-+-+-+-+-+-+-+-+
	 * |   X    |  TC  |  mode = 0
	 * +-+-+-+-+-+-+-+-+
	 * |     subif     |  mode = 1
	 * +-+-+-+-+-+-+-+-+
	 * | subif |  TC   |  mode = 2 (subif[11:8])
	 * +-+-+-+-+-+-+-+-+
	 * | subif   | TC  |  mode = 3 (subif[4:0])
	 * +-+-+-+-+-+-+-+-+
	 */
	u8 queue_lookup_mode;
	u32 ethtool_flags;
	/** Array of accumulated counters from deleted GEM ports */
	u64 *ani_archived_stats;
	/** Length of ani_archived_stats array */
	int num_ani_archived_stats;
	/** Options read from device-tree */
	/** HW instance (reg) for the driver */
	u32 dev_id;
	/** DP settings that should be shared with userspace */
	struct pon_dp_flags dp_flags;
	u32 max_ctps;
	u32 max_gpid;
};

static inline void ltq_pon_net_set_skd_dw(struct sk_buff *skb,
					  unsigned int port_id,
					  unsigned int subif)
{
	struct dma_tx_desc_0 *DW0 = (struct dma_tx_desc_0 *)&skb->DW0;
	struct dma_tx_desc_1 *DW1 = (struct dma_tx_desc_1 *)&skb->DW1;

	DW1->field.ep = port_id;
	DW0->field.dest_sub_if_id = subif;
}

extern dp_cb_t ltq_pon_net_dp_cb;

/** Registers the port in DP manager based on given mode. */
int ltq_pon_net_register_dp_port(struct ltq_pon_net_hw *hw, enum pon_mode mode);
/** @} */

#endif /* __PON_ETH_H */
