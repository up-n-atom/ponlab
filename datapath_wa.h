// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 * Copyright (c) 2024, MaxLinear, Inc.
 *
 ******************************************************************************/

#ifndef DATAPATH_WA_H
#define DATAPATH_WA_H

#include "datapath.h"
#include "datapath_tx.h"

#if IS_ENABLED(CONFIG_DPM_DATAPATH_DISABLE_OPTIMIZE)
struct dp_tx_context;
struct dp_rx_context;
void dp_rcu_tx_update(struct dp_tx_context *ctx, void *new);
void dp_rcu_rx_update(struct dp_rx_context *ctx, void *new);

#else

#define dp_rcu_tx_update(ctx, new) do { \
	struct tx_hook_list *old_list; \
	old_list = rcu_access_pointer(ctx->hook_list); \
	rcu_assign_pointer(ctx->hook_list, new); \
	if (old_list) \
		kfree_rcu(old_list, rcu_head); \
} while (0)

#define dp_rcu_rx_update(ctx, new) do { \
	struct rx_hook_list *old_list; \
	old_list = rcu_access_pointer(ctx->hook_list); \
	rcu_assign_pointer(ctx->hook_list, new); \
	if (old_list) \
		kfree_rcu(old_list, rcu_head); \
} while (0)

#endif

#endif
