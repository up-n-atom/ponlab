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
#include "datapath_rx.h"
#include "datapath_wa.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

static struct dp_rx_context *dp_rx_ctx;

#undef DP_F_ENUM_OR_STRING
#define DP_F_ENUM_OR_STRING(_enum, _str) _str
const char *dp_rx_prio_type[] = DP_RX_PRIORITY_LIST;

#ifdef CONFIG_DPM_DATAPATH_DBG
void rx_hooks_validate(const struct rx_hook_list *hooks)
{
	char ksym_name_str[KSYM_SYMBOL_LEN] = {0};
	struct rx_entry *entries;
	int prio = INT_MIN;
	int i = 0;

	entries = hooks->entry_list;
	if (hooks->cnt > DP_RX_HOOK_MAX_ENTRIES)
		pr_err("DPM: %s, Found more hooks than permitted, "
				"current hook_cnt: %d, max_permitted: %d\n",
				__func__, hooks->cnt, DP_RX_HOOK_MAX_ENTRIES);

	for (i = 0; i < hooks->cnt; i++) {
		if (entries[i].cb == NULL) {
			DP_DEBUG(DP_DBG_FLAG_HOOK, "Warn: idx: %d, prio: %d, cb: NULL, something wrong\n",
					i, entries[i].prio);
			continue;
		}

		if (entries[i].prio < prio) {
			DP_DEBUG(DP_DBG_FLAG_HOOK, "idx: %d, prio: %d, cb: %s priority is wrongly stored\n",
					i, entries[i].prio,
					dp_get_sym_name_by_addr(entries[i].cb, ksym_name_str, NULL));
		}

		if (entries[i].prio > prio)
			prio = entries[i].prio;
	}
}
#else
#define rx_hooks_validate(x)
#endif

int dp_rx_update_list(struct rx_entry *entry, u8 delete_entry_op)
{
	struct dp_rx_context *ctx = dp_rx_ctx;
	struct rx_hook_list *new_list, *old_list;
	u8 cnt = 0, i, inserted = false, delete_list = 0;
	int new_list_entries_cnt, old_list_entries_cnt;
	size_t list_size;
	
	if (!ctx)
		return -1;

	old_list = ctx->hook_list;
	old_list_entries_cnt = old_list ? old_list->cnt : 0;
	if (delete_entry_op) {
		if (old_list_entries_cnt == 0)
			/*Nothing to do, delete called on empty list*/
			return 0;
		new_list_entries_cnt = old_list_entries_cnt - 1;
		if (new_list_entries_cnt == 0)
			delete_list = 1;
	} else {
		new_list_entries_cnt = old_list_entries_cnt + 1;
	}
	if (new_list_entries_cnt > DP_RX_HOOK_MAX_ENTRIES) {
		pr_err("DPM: %s, Inserting more hooks than permitted, "
				"current hook_cnt: %d, max_permitted: %d\n",
				__func__, old_list_entries_cnt, DP_RX_HOOK_MAX_ENTRIES);
		return -1;
	}

	if (delete_list) {
		dp_rcu_rx_update(ctx, NULL);
		return 0;
	}

	//,---------------------------------------------------------,
	//|  sizeof rx_hook_list |  entry1 | entry2 | ... | entry N |
	//'---------------------------------------------------------'
	list_size = sizeof(*new_list) +
		    sizeof(struct rx_entry)*new_list_entries_cnt;
	
	new_list = dp_kzalloc(list_size, GFP_ATOMIC);
	if (!new_list)
		return -ENOMEM;
	new_list->cnt = new_list_entries_cnt;
	new_list->entry_list = (struct rx_entry *)(new_list + 1);
	for (i = 0; i < old_list_entries_cnt; i++) {
		if (old_list->entry_list[i].cb == NULL) { //we should never see NULL cb's
			pr_err("DPM: %s %d, Error, idx: %d, prio: %d, cb: NULL",
					__func__, __LINE__, i, old_list->entry_list[i].prio);
			continue;
		}
		if (delete_entry_op) {
			if ((old_list->entry_list[i].prio != entry->prio) ||
					(old_list->entry_list[i].cb != entry->cb)) {
				new_list->entry_list[cnt++] = old_list->entry_list[i];
			}
		} else {
			if (!inserted) {
				if (old_list->entry_list[i].prio > entry->prio) {
					new_list->entry_list[cnt++] = *entry;
					inserted = true;
				} else if (old_list->entry_list[i].prio == entry->prio) {
					if (old_list->entry_list[i].cb != entry->cb) {
						/* Same priority different
						 * callback register, instert
						 * before existing entry with same
						 * priority
						 */
						new_list->entry_list[cnt++] = *entry;
						inserted = true;
					} else {
						pr_err("DPM: %s, prio: %d, cb: %px already registered\n",
								__func__, entry->prio, entry->cb);
						kfree(new_list);
						return -EINVAL;
					}
				}
			}
			new_list->entry_list[cnt++] = old_list->entry_list[i];
		}
	}
	
	if (!delete_entry_op && !inserted)
		new_list->entry_list[cnt++] = *entry;

	rx_hooks_validate(new_list);
	dp_rcu_rx_update(ctx, new_list);

	return 0;
}

/* Return value:
 * if final return value is 
 *    DP_RX_HOOK_FN_CONTINUE, then no one has picked/taken
 *       the skb, stray skb. Hence DPM frees the skb and increments the rx pkt drop counter
 *    DP_RX_HOOK_FN_CONSUMED/DROPPED, then skb is handed over to user and user
 *       frees the skb. DPM increments the rx pkt counter
 */
int dp_rx_hook_call(struct net_device *rxif, struct net_device *txif,
		struct sk_buff *skb, struct dp_rx_hook_data *hd)
{
	int ret = DP_RX_HOOK_FN_CONTINUE;
	struct rx_entry *entry;
	struct rx_hook_list *list;
	struct dp_rx_common *cmn = &hd->rx_cmn;
	u8 i;

	/* dp_rx_hook_call() could be called in either normal context or softirq */
	rcu_read_lock();
	list = rcu_dereference(dp_rx_ctx->hook_list);
#if IS_ENABLED(CONFIG_DPM_DATAPATH_EXTRA_DEBUG)
	if (unlikely(!list)) {
		rcu_read_unlock();
		return ret;
	}
#endif
	for (i = 0; i < list->cnt; i++) {
		entry = &list->entry_list[i];

		cmn->user_data = entry->priv;
		ret = entry->cb(rxif, txif, skb, skb->len, cmn);
		if (ret != DP_RX_HOOK_FN_CONTINUE)
			break;
	}
	rcu_read_unlock();

	return ret;
}

void proc_dp_active_rx_hook_dump(struct seq_file *seq)
{
	char ksym_name_str[KSYM_SYMBOL_LEN] = {0};
	struct rx_hook_list *list;
	u8 i;

	if (!capable(CAP_SYS_PACCT))
		return;

	rcu_read_lock();
	list = rcu_dereference(dp_rx_ctx->hook_list);
	if (!list) {
		rcu_read_unlock();
		return;
	}
	dp_sprintf(seq, "RX hooks(%d) registered to DPM:\n", list->cnt);
	dp_sprintf(seq, "-------------------------\n");
	for (i = 0; i < list->cnt; i++) {
		const struct rx_entry *entry = &list->entry_list[i];

		if (entry->cb) {
			dp_get_sym_name_by_addr(entry->cb, ksym_name_str, NULL);
			dp_sprintf(seq, "  idx: %d, prio: %d, hook.cb: %s\n", i,
					entry->prio,
					ksym_name_str);
		} else {
			dp_sprintf(seq, "  idx: %d, prio: %d, hook.cb: NULL\n", i,
					entry->prio);
		}
	}
	rcu_read_unlock();
}

int32_t dp_subif_rx_fn_dfl_hook_cb(struct net_device *rxif, struct net_device *txif,
		struct sk_buff *skb, int32_t len, struct dp_rx_common *cmn)
{
	int ret = DP_RX_HOOK_FN_CONTINUE;
	struct dp_rx_hook_data *hd;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_EXTRA_DEBUG)
	if (unlikely(!cmn))
		return ret;
#endif
	hd = container_of(cmn, struct dp_rx_hook_data, rx_cmn);
	if (!hd->rx_fn)
		return ret;
	hd->rx_fn(rxif, txif, skb, len);

	return DP_RX_HOOK_FN_CONSUMED;
}

int dp_rx_ctx_init(int inst)
{
	int ret;

	if (inst)
		return 0;

	dp_rx_ctx = devm_kzalloc(&g_dp_dev->dev, sizeof(*dp_rx_ctx),
				    GFP_ATOMIC);
	if (!dp_rx_ctx)
		return -ENOMEM;
	DP_RX_CTX_LOCK_INIT(&dp_rx_ctx->lock);

	/*Register subif rx_fn default hook as last hook to execute rx_fn*/
	ret = dp_register_rx_hook(DP_RX_HOOK_PRI_LAST, dp_subif_rx_fn_dfl_hook_cb, NULL);
	if (ret < 0) {
		pr_err("DPM: %s %d, failed to register default rx_fn hook, ret: %d\n",
				__func__, __LINE__, ret);
		return ret;
	}

	return 0;
}

int dp_register_rx_hook2(int priority, dp_rx_hook_fn_t fn, void *priv)
{
	int ret;
	struct rx_entry e;

	if (unlikely(!dp_rx_ctx)) {
		pr_err("DPM: %s, failed, rx hook ctx is NULL\n", __func__);
		return -ENOMEM;
	}
	if (unlikely(!fn)) {
		pr_err("DPM: %s, failed, hook callback fn is NULL\n", __func__);
		return -EINVAL;
	}
	e.prio = priority;
	e.cb = fn;
	e.priv = priv;
	/* add spin lock to protect modules update RX list concurrently */
	DP_RX_CTX_LOCK(&dp_rx_ctx->lock);
	ret = dp_rx_update_list(&e, 0);
	DP_RX_CTX_UNLOCK(&dp_rx_ctx->lock);

	return ret;
}
EXPORT_SYMBOL(dp_register_rx_hook2);

int dp_deregister_rx_hook2(int priority, dp_rx_hook_fn_t fn)
{
	int ret;
	struct rx_entry e;

	if (unlikely(!dp_rx_ctx)) {
		pr_err("DPM: %s, failed, rx hook ctx is NULL\n", __func__);
		return -ENOMEM;
	}
	if (unlikely(!fn)) {
		pr_err("DPM: %s, failed, hook callback fn is NULL\n", __func__);
		return -EINVAL;
	}
	e.prio = priority;
	e.cb = fn;
	/* add spin lock to protect modules update RX list concurrently */
	DP_RX_CTX_LOCK(&dp_rx_ctx->lock);
	ret = dp_rx_update_list(&e, 1);
	DP_RX_CTX_UNLOCK(&dp_rx_ctx->lock);

	return ret;
}
EXPORT_SYMBOL(dp_deregister_rx_hook2);

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP

