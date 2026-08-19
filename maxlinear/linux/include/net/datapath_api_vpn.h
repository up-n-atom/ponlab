/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2019 Intel Corporation.
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

#ifndef __DATAPATH_API_VPN_H_
#define __DATAPATH_API_VPN_H_

#include <net/datapath_api.h>

enum mxl_vpn_mode {
	VPN_MODE_TUNNEL,
	VPN_MODE_TRANSPORT
};

enum mxl_vpn_ip_mode {
	VPN_IP_MODE_IPV4,
	VPN_IP_MODE_IPV6
};

enum mxl_vpn_direction {
	VPN_DIRECTION_OUT,
	VPN_DIRECTION_IN
};

/*! @brief session/tunnel action */
struct mxl_vpn_ipsec_act {
	u32 dw0_mask; /*!< mask set by PPA
		       * mask 0: reset the bit
		       * mask 1: keep the bit value
		       */
	u32 dw0_val;  /*!< new dw value set by PPA
		       * final dw0 = (receiving_dw0 & dw0_mask ) | dw0_val
		       */
	u32 dw1_mask; /*!< mask set by PPA
		       * mask 0: reset the bit
		       * mask 1: keep the bit value
		       */
	u32 dw1_val;  /*!< new dw value set by PPA
		       * final dw0 = (receiving_dw0 & dw0_mask ) | dw0_val
		       */
	u32 enq_qos; /*!< flag to indicate enqueue with or without qos
		      *   1: enqueue with qos
		      *   0: enqueue bypass qos
		      */
};

/*! @brief tunnel related info */
struct mxl_vpn_tunnel_info {
	u32 tunnel_id;
	u32 gpid;
	u32 subif;
	u32 qid;
	u32 iv_sz;
	enum mxl_vpn_mode mode;
	enum mxl_vpn_ip_mode ip_mode;
	struct net_device *vpn_if;
};

struct mxl_vpn_ops {
	struct device *dev;

	/*!
	 *@brief Look for an existing tunnel info from SPI
	 *@param[in] dev: device
	 *@param[in] spi: SPI index
	 *@param[out] info: tunnel info
	 *@return Returns 0 on succeed
	 */
	int (*get_tunnel_info)(struct device *dev, u32 spi,
			       struct mxl_vpn_tunnel_info *info);

	/*!
	 *@brief Update inbound action of tunnel
	 *@param[in] dev: device
	 *@param[in] tunnel_id: tunnel id
	 *@param[in] act: action
	 *@return Returns 0 on succeed
	 */
	int (*update_tunnel_in_act)(struct device *dev, int tunnel_id,
				    struct mxl_vpn_ipsec_act *act);

	/*!
	 *@brief Update outbound action of tunnel
	 *@param[in] dev: device
	 *@param[in] tunnel_id: tunnel id
	 *@param[in] act: action
	 *@return Returns 0 on succeed
	 */
	int (*update_tunnel_out_act)(struct device *dev, int tunnel_id,
				     struct mxl_vpn_ipsec_act *act);

	/*!
	 *@brief Update ingress netdev
	 *@param[in] dev: device
	 *@param[in] tunnel_id: tunnel id
	 *@param[in] netdev: netdev
	 *@return Returns 0 on succeed
	 */
	int (*update_tunnel_in_netdev)(struct device *dev, int tunnel_id,
				       struct net_device *netdev);

	/*!
	 *@brief Add a session into tunnel
	 *@param[in] dev: device
	 *@param[in] tunnel_id: tunnel id
	 *@param[in] act: action
	 *@return Returns session id if one already exist, error code otherwise
	 */
	int (*add_session)(struct device *dev, int tunnel_id,
			   struct mxl_vpn_ipsec_act *act);

	/*!
	 *@brief Update a session action
	 *@param[in] dev: device
	 *@param[in] tunnel_id: tunnel id
	 *@param[in] session_id: session id
	 *@param[in] act: action
	 *@return Returns 0 on succeed
	 */
	int (*update_session)(struct device *dev, int tunnel_id, int session_id,
			      struct mxl_vpn_ipsec_act *act);

	/*!
	 *@brief Delete a session from tunnel
	 *@param[in] dev: device
	 *@param[in] tunnel_id: tunnel id
	 *@param[in] session_id: session id
	 *@return Returns 0 on succeed
	 */
	int (*delete_session)(struct device *dev, int tunnel_id,
			      int session_id);

	/*!
	 *@brief Map protocol to VPN FW next header enum (4-bit)
	 *@param[in] dev: device
	 *@param[in] proto: proto
	 *@param[out] next_header: mapped next header value
	 *@return Returns 0 on succeed
	 */
	int (*proto_to_next_header)(struct device *dev, u8 proto,
				    u8 *next_header);

	/*!
	 *@brief Add xfrm state
	 *@param[in] x: xfrm state
	 *@return Returns 0 on succeed
	 */
	int (*add_xfrm_sa)(struct xfrm_state *x);

	/*!
	 *@brief Delete xfrm state
	 *@param[in] x: xfrm state
	 *@return void
	 */
	void (*delete_xfrm_sa)(struct xfrm_state *x);

	/*!
	 *@brief xfrm offload precheck
	 *@param[in] skb: skb
	 *@param[in] x: xfrm state
	 *@return true if offload is permitted, false otherwise
	 */
	bool (*xfrm_offload_ok)(struct sk_buff *skb, struct xfrm_state *x);
};

/*!
 *@brief get VPN ops registration
 *@param[in] inst: DP instance ID
 *@return VPN ops pointer if registered, or NULL if not registered
 */
static inline struct mxl_vpn_ops *dp_get_vpn_ops(int inst)
{
	return (struct mxl_vpn_ops *)dp_get_ops(inst, DP_OPS_VPN);
}

#endif /*DATAPATH_API_H */
