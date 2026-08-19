// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2024, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kallsyms.h>
#include <net/xfrm.h>
#include <linux/log2.h>
#include <net/datapath_api.h>
#include "datapath.h"
#include "datapath_tx.h"
#include "datapath_wa.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

static struct dp_tx_context *dp_tx_ctx;

#undef DP_F_ENUM_OR_STRING
#define DP_F_ENUM_OR_STRING(_enum, _str) _str
const char *dp_tx_prio_type[] = DP_TX_PRIORITY_LIST;

#define POS(VAL) ilog2(DP_TX_##VAL)
const char *dp_tx_flag_str[] = {
	[0] 				= "NONE",
	[POS(CAL_CHKSUM)] 	= "DP_TX_CAL_CHKSUM",
	[POS(DSL_FCS)] 		= "DP_TX_DSL_FCS",
	[POS(BYPASS_QOS)]	= "DP_TX_BYPASS_QOS",
	[POS(BYPASS_FLOW)]  = "DP_TX_BYPASS_FLOW",
	[POS(OAM)] 			= "DP_TX_OAM",
	[POS(TO_DL_MPEFW)] 	= "DP_TX_TO_DL_MPEFW",
	[POS(INSERT)] 		= "DP_TX_INSERT",
	[POS(INSERT_POINT)] = "DP_TX_INSERT_POINT",
	[POS(WITH_PMAC)] 	= "DP_TX_WITH_PMAC",
};

#define dp_tx_flags_str_len \
	(sizeof (dp_tx_flag_str) / sizeof (dp_tx_flag_str[0]))

void dp_tx_dbg(char *title, struct sk_buff *skb, struct dp_tx_common_ex *ex)
{
	struct inst_info *dp_info = get_dp_prop_info(0);

#if defined(DP_SKB_HACK)
	DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
		 "%s: dp_xmit:skb->data/len=0x%px/%d data_ptr=%x from port=%d and subitf=%d\n",
		 title,
		 skb->data, skb->len,
		 ((struct dma_tx_desc_2 *)&skb->DW2)->field.data_ptr,
		 ex->port->port_id, ex->cmn.subif);
#endif
	if (dp_dbg_flag & DP_DBG_FLAG_DUMP_TX_DATA) {
		if (dp_tx_get_pmac_len(ex)) {
			dp_dump_raw_data(dp_tx_get_pmac(ex),
					 dp_tx_get_pmac_len(ex),
					 "Tx Data");
		}
		dp_dump_raw_data(skb->data, skb->len, "Tx Data");
	}

	DP_DEBUG(DP_DBG_FLAG_DUMP_TX_SUM,
		 "ip_summed=%s(%d) encapsulation=%s\n",
		 dp_skb_csum_str(skb), skb->ip_summed,
		 skb->encapsulation ? "Yes" : "No");

	if (skb->encapsulation)
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX_SUM,
			 "inner ip start=0x%lx(%d), transport=0x%lx(%d)\n",
			 (unsigned long)skb_inner_network_header(skb),
			 (int)(skb_inner_network_header(skb) -
			       skb->data),
			 (unsigned long)
			 skb_inner_transport_header(skb),
			 (int)(skb_inner_transport_header(skb) -
			       skb_inner_network_header(skb)));
	else
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX_SUM,
			 "ip start=0x%lx(%d), transport=0x%lx(%d)\n",
			 (unsigned long)skb_network_header(skb),
			 (int)(skb_network_header(skb) - skb->data),
			 (unsigned long)skb_transport_header(skb),
			 (int)(skb_transport_header(skb) -
			       skb_network_header(skb)));

	if (dp_dbg_flag & DP_DBG_FLAG_DUMP_TX_DESCRIPTOR)
#if defined(DP_SKB_HACK)
		dp_info->dump_tx_dma_desc((struct dma_tx_desc_0 *)&skb->DW0,
					  (struct dma_tx_desc_1 *)&skb->DW1,
					  (struct dma_tx_desc_2 *)&skb->DW2,
					  (struct dma_tx_desc_3 *)&skb->DW3);
#else
		;
#endif

	DP_DEBUG(DP_DBG_FLAG_DUMP_TX, "skb->len=%d skb->priority=%d\n",
		 skb->len, skb->priority);

	if (dp_dbg_flag & DP_DBG_FLAG_DUMP_TX) {
		int i;
		DP_DUMP("flags =");
		if (ex->cmn.flags) {
			/* valid flags start from index 1 */
			for (i = 1; i < dp_tx_flags_str_len; i++) {
				if (ex->cmn.flags & (1 << i))
					DP_DUMP(" %s |", dp_tx_flag_str[i]);
			}
		} else {
			DP_DUMP(" %s", dp_tx_flag_str[0]);
		}
		DP_DUMP("\n");
	}

	DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
		 "skb->data=0x%px with pmac hdr size=%zu\n", skb->data,
		 sizeof(struct pmac_tx_hdr));

	if (dp_tx_get_pmac_len(ex)) { /*insert one pmac header */
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX, "need pmac\n");

		if (dp_dbg_flag & DP_DBG_FLAG_DUMP_TX_DESCRIPTOR)
			dp_info->dump_tx_pmac(dp_tx_get_pmac(ex));
	} else {
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX, "no pmac\n");
	}

	if (skb_is_gso(skb))
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX, "GSO pkt\n");
	else
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX, "Non-GSO pkt\n");

	if (ex->cmn.flags & DP_TX_CAL_CHKSUM)
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX, "Need checksum offload\n");
	else
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX, "No need checksum offload pkt\n");

	DP_DEBUG(DP_DBG_FLAG_DUMP_TX, "\n\n");
}

int dp_tx_update_list(void)
{
	struct dp_tx_context *ctx = dp_tx_ctx;
	struct tx_hook_list *new_list;
	u8 cnt = 0;
	u8 i;

	/* update stored tx info into fast list */
	new_list = dp_kzalloc(sizeof(*new_list), GFP_ATOMIC);
	if (!new_list)
		return -ENOMEM;
	for (i = 0; i < DP_TX_CNT; i++) {
		const struct tx_stored *s = &ctx->stored[i];
		bool en = s->preprocess_always_enabled || s->process.cb;

		if (s->preprocess.cb && en) {
			dp_memcpy(&new_list->entry[cnt++], &s->preprocess,
				  sizeof(struct tx_entry));
		}
		if (s->process.cb) {
			dp_memcpy(&new_list->entry[cnt++], &s->process,
				  sizeof(struct tx_entry));
		}
	}
	new_list->cnt = cnt;
	dp_rcu_tx_update(ctx, new_list);

	return 0;
}

void dp_tx_register_process(enum DP_TX_PRIORITY priority, tx_fn fn, void *priv)
{
	struct tx_stored *s;

	if (!dp_tx_ctx)
		return;
	s = &dp_tx_ctx->stored[priority];
	s->process.cb = fn;
	s->process.priv = priv;
}

void dp_tx_register_preprocess(enum DP_TX_PRIORITY priority, tx_fn fn,
			       void *priv, bool always_enabled)
{
	struct tx_stored *s;

	if (!dp_tx_ctx)
		return;
	s = &dp_tx_ctx->stored[priority];
	s->preprocess.cb = fn;
	s->preprocess.priv = priv;
	s->preprocess_always_enabled = always_enabled;
}

int dp_tx_start(struct sk_buff *skb, struct dp_tx_common *cmn)
{
	int ret = DP_XMIT_ERR_NOT_INIT;
	struct tx_hook_list *list;
	u8 i;

	/* dp_tx_start() could be called in either normal context or softirq */
	rcu_read_lock();
	list = rcu_dereference(dp_tx_ctx->hook_list);
	if (!list) {
		rcu_read_unlock();
		return ret;
	}
	for (i = 0; i < list->cnt; i++) {
		const struct tx_entry *entry = &list->entry[i];

		ret = entry->cb(skb, cmn, entry->priv);
		if (ret != DP_TX_FN_CONTINUE)
			break;
	}
	rcu_read_unlock();
	return ret;
}

void proc_dp_active_tx_hook_dump(struct seq_file *seq)
{
	char ksym_name_str[KSYM_SYMBOL_LEN] = {0};
	struct dp_tx_context *ctx = dp_tx_ctx;
	u8 i;
	u8 print_once = 1;
	rcu_read_lock();
	for (i = 0; i < DP_TX_CNT; i++) {
		const struct tx_stored *s = &ctx->stored[i];
		if (s->process.cb) {
			if (print_once) {
				dp_sprintf(seq, "TX hooks registered to DPM:\n");
				dp_sprintf(seq, "-----------------------\n");
				print_once = 0;
			}
			dp_get_sym_name_by_addr(s->process.cb, ksym_name_str, NULL);
			dp_sprintf(seq, "  prio: %s, cb: %s\n",
					dp_tx_prio_type[i], ksym_name_str);
		}
	}
	rcu_read_unlock();
}

int dp_tx_ctx_init(int inst)
{
	if (inst)
		return 0;
	dp_tx_ctx = devm_kzalloc(&g_dp_dev->dev, sizeof(*dp_tx_ctx),
				    GFP_ATOMIC);
	if (!dp_tx_ctx)
		return -ENOMEM;
	DP_TX_CTX_LOCK_INIT(&dp_tx_ctx->lock);
	return 0;
}

int dp_register_tx2(enum DP_TX_PRIORITY priority, tx_fn fn, void *priv)
{
	int ret;

	if (!dp_tx_ctx)
		return -ENOMEM;
	/* add spin lock to protect modules update TX list concurrently */
	DP_TX_CTX_LOCK(&dp_tx_ctx->lock);
	dp_tx_register_process(priority, fn, priv);
	ret = dp_tx_update_list();
	DP_TX_CTX_UNLOCK(&dp_tx_ctx->lock);

	return ret;
}
EXPORT_SYMBOL(dp_register_tx2);

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP

