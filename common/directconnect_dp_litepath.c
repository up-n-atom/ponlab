/*
 * DirectConnect provides a common interface for the network devices to achieve the full or partial 
   acceleration services from the underlying packet acceleration engine
 * Copyright (c) 2017, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */

#include <linux/module.h>
#include <directconnect_dp_dcmode_api.h>
#include <net/directpath_api.h>
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
#include <linux/pp_api.h>
#endif

#include "directconnect_dp_device.h"
#include "directconnect_dp_litepath.h"

#define LITEPATH_HASH_BITS 8

struct litepath_entry {
    struct hlist_node hlist;
    struct rcu_head rcu;
    struct net_device *key;    /* device as key */
    struct dpdp_handle handle; /* lower info */
    struct dp_subif dp_subif;  /* upper info */
};

static DEFINE_READ_MOSTLY_HASHTABLE(lp_table, LITEPATH_HASH_BITS);
static DEFINE_MUTEX(lp_tlock);

static void
litepath_add_entry(uint32_t hash, struct litepath_entry *entry)
{
    mutex_lock(&lp_tlock);
    hlist_add_tail_rcu(&entry->hlist, &lp_table[hash]);
    mutex_unlock(&lp_tlock);
}

static void
litepath_del_entry(struct litepath_entry *entry)
{
    mutex_lock(&lp_tlock);
    hlist_del_rcu(&entry->hlist);
    mutex_unlock(&lp_tlock);
}

static void *
litepath_get_entry(uint32_t hash, struct net_device *key)
{
    struct litepath_entry *obj;

    hlist_for_each_entry_rcu(obj, &lp_table[hash], hlist) {
        if (obj->key == key)
            return obj;
    }

    return NULL;
}

static uint32_t
litepath_get_hash_by_handle(struct dpdp_handle *handle)
{
    uint32_t hash = 0;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
    hash = handle->gpid;
#elif IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP30)
    hash = handle->port_id << 4;
    hash |= (handle->subif >> 8) & 0xF;
#endif

    return hash % ARRAY_SIZE(lp_table);
}

static uint32_t
litepath_get_hash_by_skb(struct net_device *rxif, struct net_device *txif,
                         struct sk_buff *skb)
{
    uint32_t hash = 0;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP32)
    struct dma_tx_desc_1 *desc_1;
    struct pp_desc *pp_desc;

    if (txif) {
        desc_1 = (struct dma_tx_desc_1 *)&skb->DW1;
        hash = desc_1->field.ep;
    } else {
        pp_desc = pp_pkt_desc_get(skb);
        if (pp_desc && pp_desc->ud.is_exc_sess)
            hash = pp_desc->ud.rx_port;
    }
#elif IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP30)
    struct dma_tx_desc_0 *desc_0;
    struct dma_tx_desc_1 *desc_1;
    struct pmac_rx_hdr *pmac;

    if (txif) {
        desc_0 = (struct dma_tx_desc_0 *)&skb->DW0;
        desc_1 = (struct dma_tx_desc_1 *)&skb->DW1;

        hash = desc_1->field.ep << 4;
        hash |= (desc_0->field.dest_sub_if_id >> 8) & 0xF;
    } else {
        pmac = (struct pmac_rx_hdr *)(skb->data - sizeof(*pmac));

        hash = pmac->sppid << 4;
        hash |= pmac->src_sub_inf_id & 0xF;
    }
#endif /* CONFIG_DPM_DATAPATH_HAL_GSWIP32 */

    return hash % ARRAY_SIZE(lp_table);
}

static int32_t
dc_dp_litepath_rx(struct net_device *rxif, struct net_device *txif,
                  struct sk_buff *skb, int32_t len)
{
    int32_t ret;
    uint32_t hash = litepath_get_hash_by_skb(rxif, txif, skb);
    struct net_device *key = (txif ? txif : rxif);
    struct litepath_entry *entry;
    struct dp_subif *dp_subif = NULL;

    rcu_read_lock();
    entry = litepath_get_entry(hash, key);
    if (entry)
        dp_subif = &entry->dp_subif;

    ret = dc_dp_rx(rxif, txif, dp_subif, skb, len, DC_DP_F_RX_LITEPATH);
    rcu_read_unlock();

    return ret;
}

int32_t
dc_dp_litepath_register_dev(struct dc_dp_priv_dev_info *dev_ctx,
                            struct module *owner, uint32_t port_id,
                            struct net_device *dev, struct dc_dp_cb *datapathcb,
                            struct dc_dp_dev *devspec, uint32_t flags)
{
    int32_t ret = DC_DP_FAILURE;
    struct dpdp_handle handle = {
        .port_id = -1,
    };
    struct dp_cb dp_ops = {
        .rx_fn = dc_dp_litepath_rx,
        .stop_fn = datapathcb->stop_fn,
        .restart_fn = datapathcb->restart_fn,
    };

    if (dpdp_register_dev_hook)
        ret = dpdp_register_dev_hook(owner, dev, &handle, &dp_ops, 32, 0);

    if (!ret) {
        dev_ctx->litepath_port = handle.port_id;
        dev_ctx->litepath_used = 1;

        devspec->dc_accel_used = DC_DP_ACCEL_PARTIAL_OFFLOAD;
    }

    return ret;
}

int32_t
dc_dp_litepath_deregister_dev(struct dc_dp_priv_dev_info *dev_ctx,
                              struct module *owner, uint32_t port_id,
                              struct net_device *dev, struct dc_dp_cb *datapathcb,
                              struct dc_dp_dev *devspec, uint32_t flags)
{
    int32_t ret = DC_DP_FAILURE;
    struct dpdp_handle handle = {
        .port_id = dev_ctx->litepath_port,
        .subif = -1,
    };

    if (dpdp_register_dev_hook)
        ret = dpdp_register_dev_hook(owner, dev, &handle, NULL, 0, DP_F_DEREGISTER);

    if (!ret) {
        dev_ctx->litepath_port = 0;
        dev_ctx->litepath_used = 0;
    }

    return ret;
}

int32_t
dc_dp_litepath_register_subif(struct dc_dp_priv_dev_info *dev_ctx,
                              struct module *owner, struct net_device *dev,
                              const uint8_t *subif_name, struct dp_subif *subif_id, uint32_t flags)
{
    int32_t ret = DC_DP_FAILURE;
    int32_t subif_idx;
    struct litepath_entry *entry;
    uint32_t hash;

    if (is_sw_port(subif_id->port_id) && dev_ctx->litepath_used) {
        entry = kzalloc(sizeof(*entry), GFP_KERNEL);
        if (!entry)
            goto out;

        entry->handle.port_id = dev_ctx->litepath_port;
        entry->handle.subif = -1;

        if (dpdp_register_subif_hook)
            ret = dpdp_register_subif_hook(owner, dev, &entry->handle, 0);

        if (ret) {
            kfree(entry);
            goto out;
        }

        INIT_HLIST_NODE(&entry->hlist);
        entry->key = dev;
        entry->dp_subif.port_id = subif_id->port_id;
        entry->dp_subif.subif = subif_id->subif;
        hash = litepath_get_hash_by_handle(&entry->handle);
        litepath_add_entry(hash, entry);

        subif_idx = ((subif_id->subif >> DC_DP_SUBIFID_OFFSET) & DC_DP_SUBIFID_MASK);
        dev_ctx->subif_info[subif_idx].litepath_hash = hash;
        dev_ctx->subif_info[subif_idx].litepath_used = 1;
    }

out:
    return ret;
}

int32_t
dc_dp_litepath_deregister_subif(struct dc_dp_priv_dev_info *dev_ctx,
                                struct module *owner, struct net_device *dev,
                                const uint8_t *subif_name,
                                struct dp_subif *subif_id, uint32_t flags)
{
    int32_t ret = DC_DP_FAILURE;
    int32_t subif_idx;
    uint32_t hash;
    struct litepath_entry *entry;

    subif_idx = ((subif_id->subif >> DC_DP_SUBIFID_OFFSET) & DC_DP_SUBIFID_MASK);
    hash = dev_ctx->subif_info[subif_idx].litepath_hash;

    rcu_read_lock();
    entry = litepath_get_entry(hash, dev);
    if (!entry)
        goto out;

    if (dpdp_register_subif_hook)
        ret = dpdp_register_subif_hook(owner, dev, &entry->handle, DP_F_DEREGISTER);

    if (!ret) {
        litepath_del_entry(entry);
        kfree(entry);

        dev_ctx->subif_info[subif_idx].litepath_hash = 0;
        dev_ctx->subif_info[subif_idx].litepath_used = 0;
    }

out:
    rcu_read_unlock();
    return ret;
}

int32_t
dc_dp_litepath_xmit(struct dc_dp_priv_dev_info *dev_ctx, struct net_device *rx_if,
                    struct dp_subif *rx_subif, struct dp_subif *tx_subif,
                    struct sk_buff *skb, int32_t len, uint32_t flags)
{
    int32_t ret = DC_DP_FAILURE;
    int32_t tx_subif_idx;
    uint32_t hash;
    struct net_device *key;
    struct litepath_entry *entry;

    tx_subif_idx = ((tx_subif->subif >> DC_DP_SUBIFID_OFFSET) & DC_DP_SUBIFID_MASK);
    hash = dev_ctx->subif_info[tx_subif_idx].litepath_hash;
    key = (rx_if ? rx_if : skb->dev);

    rcu_read_lock();
    entry = litepath_get_entry(hash, key);
    if (!entry) {
        dev_kfree_skb_any(skb);
        goto out;
    }

    if (rx_if) {
        if (dpdp_fwd_offload_hook) {
            ret = dpdp_fwd_offload_hook(rx_if, &entry->handle, skb, 0);
            if (ret != RX_HANDLER_CONSUMED) {
                dev_kfree_skb_any(skb);
                ret = DC_DP_FAILURE;
            }
        }
    } else {
        if (dpdp_qos_offload_hook) {
            ret = dpdp_qos_offload_hook(skb->dev, &entry->handle, skb, 0);
            if (ret == NETDEV_TX_CONTINUE)
                ret = -2;
            else if (ret != NETDEV_TX_OK)
                ret = DC_DP_FAILURE;
        }
    }

out:
    rcu_read_unlock();
    return ret;
}
