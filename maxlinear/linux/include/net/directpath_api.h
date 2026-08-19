// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2023 MaxLinear, Inc.
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

#ifndef DIRECTPATH_API_H
#define DIRECTPATH_API_H

#include <net/datapath_api.h>

#define NETDEV_TX_CONTINUE	INT_MAX

/*! @brief DPDP handle */
struct dpdp_handle {
	int	port_id;
	int	subif;
	int	gpid;
};

/*! @brief  Register/deregister a network device to/from DPDP
 *@param[in] owner  Kernel module pointer which owns the port
 *@param[in] dev  Pointer to Linux netdevice structure
 *@param[in] handle  Pointer to DPDP handle structure
 *@param[in] ops Datapath driver callback structure
 *		dpdp_rxtx_handler_dflt: default rx_fn handler
 *@param[in] max_subif Subif max
 *@param[in] flags :
 *		- DP_F_DEREGISTER to deregister the device
 *@return Returns 0 on OK, <0 on ERROR
 */
typedef int dpdp_register_dev_hook_t(struct module *owner,
				     struct net_device *dev,
				     struct dpdp_handle *handle,
				     struct dp_cb *ops, int max_subif,
				     u32 flags);
extern dpdp_register_dev_hook_t __rcu *dpdp_register_dev_hook;

/*! @brief  Register/deregister a logical network device to/from DPDP
 *@param[in] owner  Kernel module pointer which owns the port
 *@param[in] dev  Pointer to Linux netdevice structure
 *@param[in] handle  Pointer to DPDP handle structure
 *@param[in] flags :
 *		- DP_F_DEREGISTER to deregister the subif
 *@return Returns 0 on OK, <0 on ERROR
 */
typedef int dpdp_register_subif_hook_t(struct module *owner,
				       struct net_device *dev,
				       struct dpdp_handle *handle, u32 flags);
extern dpdp_register_subif_hook_t __rcu *dpdp_register_subif_hook;

/*! @brief  Transmit packet to DPDP for forward offload
 *@param[in] dev  Pointer to Linux netdevice structure
 *@param[in] handle  Pointer to DPDP handle structure
 *@param[in] skb  pointer to packet buffer like sk_buff
 *@param[in] flags :Reserved
 *@return Returns RX_HANDLER_CONSUMED on OK, RX_HANDLER_PASS on ERROR
 */
typedef rx_handler_result_t dpdp_fwd_offload_hook_t(struct net_device *dev,
						    struct dpdp_handle *handle,
						    struct sk_buff *skb,
						    u32 flags);
extern dpdp_fwd_offload_hook_t __rcu *dpdp_fwd_offload_hook;

/*! @brief  Transmit packet to DPDP for egress QoS offload
 *@param[in] dev  Pointer to Linux netdevice structure
 *@param[in] handle  Pointer to DPDP handle structure
 *@param[in] skb  pointer to packet buffer like sk_buff
 *@param[in] flags :Reserved
 *@return Returns NETDEV_TX_OK on OK, NETDEV_TX_BUSY on BUSY, <0 on ERROR
 */
typedef netdev_tx_t dpdp_qos_offload_hook_t(struct net_device *dev,
					    struct dpdp_handle *handle,
					    struct sk_buff *skb, u32 flags);
extern dpdp_qos_offload_hook_t __rcu *dpdp_qos_offload_hook;

#endif /* DIRECTPATH_API_H */
