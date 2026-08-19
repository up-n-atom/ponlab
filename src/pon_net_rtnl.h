/******************************************************************************
 *
 * Copyright (c) 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/** Enable/Disable MAC learning on chosen interface.
 *
 *  \param[in]  ctx		PON Network Library Context
 *  \param[in]  ifname		Interface name
 *  \param[in]  learning	Enable/Disable learning
 */
enum pon_adapter_errno
pon_net_rtnl_bridge_learning_set(struct pon_net_context *ctx,
				 const char *ifname,
				 uint8_t learning);
