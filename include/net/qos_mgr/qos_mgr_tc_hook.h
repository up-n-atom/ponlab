// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) 2019~2020 Intel Corporation.
 *  */
#ifndef __QOS_MGR_TC_HOOK_H
#define __QOS_MGR_TC_HOOK_H
/*! \file qos_mgr_tc_hook.h
  \brief This file contains TC exported HOOK API to linux Kernel builtin.
 */
/** \addtogroup  QOS_MGR_TC_HOOK_API */
/*@{*/
/*
 * ####################################
 *             Declaration
 * ####################################
 */
#include <linux/netdevice.h>
#include <linux/version.h>

#define NETDEV_TYPE_TCONT 0x100000
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,13,0)
extern int32_t (*qos_mgr_hook_setup_tc)(struct net_device *dev, u32 handle, __be16 protocol, struct tc_to_netdev *tc);
extern int32_t (*qos_mgr_hook_setup_tc_ext)(struct net_device *dev, u32 handle, __be16 protocol, struct tc_to_netdev *tc, int32_t deq_idx, int32_t port_id);
#else
extern int32_t (*qos_mgr_hook_setup_tc)(struct net_device *dev, enum tc_setup_type type, void *type_data);
#endif

/* @} */
#endif  /*  __QOS_MGR_TC_HOOK_H */
