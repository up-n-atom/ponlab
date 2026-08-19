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
#include <linux/netdevice.h>
#include <directconnect_dp_dcmode_api.h>
#include "directconnect_dp_device.h"

static uint32_t sw_port_map = 0x0;

struct dc_dp_sw_port_lock {
    spinlock_t        lock;  /*!< DC SW lock */
};

struct dc_dp_sw_port_lock g_sw_port_lck;

static inline int32_t dc_dp_sw_port_lock_init(struct dc_dp_sw_port_lock *p_lock)
{
    spin_lock_init(&p_lock->lock);
    return 0;
}

static inline void dc_dp_sw_port_lock_free(struct dc_dp_sw_port_lock *p_lock)
{
    return;
}

int32_t
dp_alloc_sw_port(struct module *owner, struct net_device *dev, int32_t dev_port, int32_t port_id, uint32_t flags)
{
    int32_t port_idx = 1;
    int32_t sw_port_id = 0;

    spin_lock_bh(&g_sw_port_lck.lock);
    while ((sw_port_map & port_idx)) {
        port_idx <<= 1;
        sw_port_id++;
    }
    if (sw_port_id >= DC_DP_MAX_SW_DEVICE) {
        spin_unlock_bh(&g_sw_port_lck.lock);
        return -1;
    }
    sw_port_map |= port_idx;
    spin_unlock_bh(&g_sw_port_lck.lock);

    return (sw_port_id + DC_DP_SW_PORT_RANGE_START);
}

int32_t
dp_dealloc_sw_port(struct module *owner, struct net_device *dev, int32_t dev_port, int32_t port_id, uint32_t flags)
{
    int32_t port_idx = 1;
    int32_t sw_port_id;

    if (!is_sw_port(port_id))
        return -1;

    spin_lock_bh(&g_sw_port_lck.lock);
    sw_port_id = port_id - DC_DP_SW_PORT_RANGE_START;
    port_idx <<= sw_port_id;
    if (!(sw_port_map & port_idx)) {
        spin_unlock_bh(&g_sw_port_lck.lock);
        return -1;
    }
    sw_port_map &= ~port_idx;
    spin_unlock_bh(&g_sw_port_lck.lock);

    return 0;
}

void dc_dp_sw_port_init(void)
{
    dc_dp_sw_port_lock_init(&g_sw_port_lck);
}

void dc_dp_sw_port_exit(void)
{
    dc_dp_sw_port_lock_free(&g_sw_port_lck);
}

/* TODO : Miscleneous API to be used by any SW DC mode */
