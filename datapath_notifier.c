// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2021, MaxLinear, Inc.
 * Copyright 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <linux/module.h>
#include <linux/types.h>	/* size_t */
#include <linux/inetdevice.h>
#include <net/datapath_api.h>
#include <net/datapath_proc_api.h>
#include "datapath.h"
#include "datapath_instance.h"
#include "datapath_swdev.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

int dp_notifier_invoke(int inst, struct net_device *dev,
		       u32 port_id, u32 subif_id, void *evt_data,
		       enum DP_EVENT_TYPE type)
{
	struct pmac_port_info *port_info;
	struct dp_evt_notif_data notif_data = {0};

	if (dp_mod_exiting)
		return DP_FAILURE;
	port_info = get_dp_port_info(inst, port_id);

	notif_data.inst = inst;
	notif_data.dev = dev;
	notif_data.dev_port = port_info->dev_port;
	notif_data.mod = port_info->owner;
	notif_data.dpid = port_id;
	notif_data.alloc_flag = port_info->alloc_flags;

	DP_DEBUG(DP_DBG_FLAG_NOTIFY, "portid: %d, subifid: %d, dev: 0x%px, dev->name: %s"
			", evt_type: %d\n", port_id, subif_id,
			dev, dev ? dev->name : "nil", type);
	switch (type) {
	case DP_EVENT_ALLOC_PORT:
		break;
	case DP_EVENT_DE_ALLOC_PORT:
		break;
	case DP_EVENT_REGISTER_DEV:
		notif_data.data.dev_data = evt_data;
		break;
	case DP_EVENT_DE_REGISTER_DEV:
		break;
	case DP_EVENT_REGISTER_SUBIF:
	case DP_EVENT_DE_REGISTER_SUBIF:
		notif_data.subif = subif_id;
		break;
	default:
		pr_err("DPM: %s Unsupported Event %d\n", __func__, type);
		return DP_FAILURE;
	}

#ifdef DP_SPIN_LOCK
#warning blocking_notifier will not work with DP_SPIN_LOCK, use atomic_notifier
#endif
	/* blocking_notifier_call_chain uses a semaphore to lock the execution,
	 * the atomic_notifier uses a rcu_read_lock. Using the semaphore is
	 * preferred, but only possible when it is not called inside a spinlock.
	 * When DP_SPIN_LOCK is defined this is called from a spinlock.
	 */
	blocking_notifier_call_chain(&dp_evt_notif_list, type, &notif_data);

	return DP_SUCCESS;
}

BLOCKING_NOTIFIER_HEAD(dp_evt_notif_list);
static int dp_notifier_cb(struct notifier_block *self, unsigned long action,
		       void *data)
{
	struct dp_evt_notif_data *notif_data = data;
	struct dp_evt_notif_info *evt_notif = NULL;
	struct dp_event_info info = {0};

	if (!data) {
		pr_err("DPM: invalid notifier data\n");
		return -EINVAL;
	}

	evt_notif = container_of(self, struct dp_evt_notif_info, nb);
	if (!evt_notif->evt_info.dp_event_cb) {
		pr_err("DPM: skipping event 0x%lx, callback not registerd: owner[%d] type[0x%x]\n",
		       action, evt_notif->evt_info.owner,
		       evt_notif->evt_info.type);
		goto end;
	}

	if (evt_notif->evt_info.inst != notif_data->inst) {
		pr_info("DPM: skipping event 0x%lx, inst(%d vs %d) not match: owner[%d] type[0x%x]\n",
			action, evt_notif->evt_info.inst, notif_data->inst,
			evt_notif->evt_info.owner,
			evt_notif->evt_info.type);
		goto end;
	}

	switch (evt_notif->evt_info.type & action) {
	case DP_EVENT_INIT:
		info.init_info.dev = notif_data->dev;
		info.init_info.owner = notif_data->mod;
		info.init_info.dev_port = notif_data->dev_port;
		break;
	case DP_EVENT_ALLOC_PORT:
		info.alloc_info.dev = notif_data->dev;
		info.alloc_info.owner = notif_data->mod;
		info.alloc_info.dev_port = notif_data->dev_port;
		break;
	case DP_EVENT_DE_ALLOC_PORT:
		info.de_alloc_info.dev = notif_data->dev;
		info.de_alloc_info.owner = notif_data->mod;
		info.de_alloc_info.dev_port = notif_data->dev_port;
		break;
	case DP_EVENT_REGISTER_DEV:
		info.reg_dev_info.dev = notif_data->dev;
		info.reg_dev_info.dpid = notif_data->dpid;
		info.reg_dev_info.owner = notif_data->mod;
		info.reg_dev_info.dev_data = notif_data->data.dev_data;
		break;
	case DP_EVENT_DE_REGISTER_DEV:
		info.dereg_dev_info.dev = notif_data->dev;
		info.dereg_dev_info.dpid = notif_data->dpid;
		info.dereg_dev_info.owner = notif_data->mod;
		break;
	case DP_EVENT_REGISTER_SUBIF:
		info.reg_subif_info.dev = notif_data->dev;
		info.reg_subif_info.dpid = notif_data->dpid;
		info.reg_subif_info.subif = notif_data->subif;
		break;
	case DP_EVENT_DE_REGISTER_SUBIF:
		info.de_reg_subif_info.dev = notif_data->dev;
		info.de_reg_subif_info.dpid = notif_data->dpid;
		info.de_reg_subif_info.subif = notif_data->subif;
		break;
	case DP_EVENT_OWNER_SWITCH:
		info.owner_info.dpid = notif_data->dpid;
		info.owner_info.new_owner = notif_data->type;
		break;
	default:
		DP_DEBUG(DP_DBG_FLAG_NOTIFY, "skipping event 0x%lx, not registered type: owner[%d] type[0x%x]\n",
		       action, evt_notif->evt_info.owner,
		       evt_notif->evt_info.type);
		goto end;
	}

	info.inst = notif_data->inst;
	info.alloc_flags = notif_data->alloc_flag;
	info.type = action;
	info.data = evt_notif->evt_info.data;
	evt_notif->evt_info.dp_event_cb(&info);
end:
	return NOTIFY_DONE;
}

static int dp_notify_registered_info(struct dp_evt_notif_info *evt_notif)
{
	struct dp_event_info info = {0};
	struct hlist_head *head;
	struct hlist_node *n;
	struct dp_subif_cache *pos;
	u32 idx;
	int port_id, i;
	int max_dp_ports;	/* max dp ports */
	struct inst_info *inst_info = NULL;
	struct pmac_port_info *p_info;

	if (evt_notif->evt_info.type & DP_EVENT_ALLOC_PORT) {
		/* loop through pmac port array DP_EVENT_ALLOC_PORT */
		for (i = 0; i < DP_MAX_INST; i++) {
			/* Retrieve the hw capabilities for each instance */
			inst_info = get_dp_prop_info(i);
			max_dp_ports = inst_info->cap.max_num_dp_ports;
			for (port_id = 0; port_id < max_dp_ports; port_id++) {
				p_info = get_dp_port_info(i, port_id);
				if (p_info->status >= PORT_ALLOCATED) {
					info.inst = i;
					info.alloc_info.dev = p_info->dev;
					info.alloc_info.owner = p_info->owner;
					info.alloc_info.dev_port = p_info->dev_port;
					info.type = DP_EVENT_ALLOC_PORT;
					info.data = evt_notif->evt_info.data;
					evt_notif->evt_info.dp_event_cb(&info);
				}
			}
		}
	}

	if (evt_notif->evt_info.type & DP_EVENT_REGISTER_SUBIF) {
		if (info.alloc_info.owner) /*Just reset if it is used above*/
			dp_memset(&info, 0, sizeof(info));
		/* loop through RCU subif struct for DP_EVENT_REGISTER_SUBIF */
		for (idx = 0; idx < DP_SUBIF_LIST_HASH_SIZE; idx++) {
			head = &dp_subif_list[idx];
			hlist_for_each_entry_safe(pos, n, head, hlist) {
				info.inst = pos->subif.inst;
				info.reg_subif_info.dev = pos->dev;
				info.reg_subif_info.dpid = pos->subif.port_id;
				info.reg_subif_info.subif = pos->subif.subif;
				info.type = DP_EVENT_REGISTER_SUBIF;
				info.data = evt_notif->evt_info.data;
				evt_notif->evt_info.dp_event_cb(&info);
			}
		}
	}
	return DP_SUCCESS;
}

int register_dp_event_notifier(struct dp_event *info)
{
	struct dp_evt_notif_info *evt_notif;
	struct notifier_block *nb = NULL;
	int ret = 0;

	if (!info) {
		pr_err("DPM: %s: invalid info\n", __func__);
		return DP_FAILURE;
	}

	if (!info->id) {
		evt_notif = kzalloc(sizeof(*evt_notif), GFP_ATOMIC);
		if (!evt_notif)
			return DP_FAILURE;

		evt_notif->nb.notifier_call = dp_notifier_cb;
		info->id = &evt_notif->nb;
		nb = &evt_notif->nb;
	} else {
		/* to avoid race between dp_notifier_cb and update_dp_evt_notif_info,
		 * remove nb from list, modify evt_info and re-insert the nb
		 */
		nb = info->id;
		ret = blocking_notifier_chain_unregister(&dp_evt_notif_list,
							 nb);
		if (ret) {
			pr_err("DPM: %s: invalid handle\n", __func__);
			return DP_FAILURE;
		}

		evt_notif = container_of(nb, struct dp_evt_notif_info, nb);
	}

	dp_memcpy(&evt_notif->evt_info, info, sizeof(*info));
	blocking_notifier_chain_register(&dp_evt_notif_list, nb);
	/* Trigger callback for current module to receive the already
	 * allocated DPM ports, registered subifs
	 */
	 DP_LIB_LOCK(&dp_lock);
	 dp_notify_registered_info(evt_notif);
	 DP_LIB_UNLOCK(&dp_lock);

	return 0;
}

int unregister_dp_event_notifier(struct dp_event *info)
{
	struct dp_evt_notif_info *evt_notif;
	int ret = 0;

	if (!info || !info->id) {
		pr_err("DPM: %s:%d: invalid handle\n", __func__, __LINE__);
		return DP_FAILURE;
	}

	ret = blocking_notifier_chain_unregister(&dp_evt_notif_list, info->id);
	if (ret) {
		pr_err("DPM: %s:%d: invalid handle\n", __func__, __LINE__);
		return DP_FAILURE;
	}

	evt_notif = container_of(info->id, struct dp_evt_notif_info, nb);
	kfree(evt_notif);
	info->id = NULL;
	return 0;
}

/* NOTE: keep in sync with enum DP_EVENT_OWNER */
const char *dp_event_owner_str[] = {
	[DP_EVENT_OWNER_OTHERS] = "OTHERS",
	[DP_EVENT_OWNER_DPDK] = "DPDK",
	[DP_EVENT_OWNER_PPA] = "PPA",
	[DP_EVENT_OWNER_MIB] = "MIB",
};

/* NOTE: keep in sync with enum DP_EVENT_TYPE_BITS */
const char *dp_event_type_str[] = {
	[DP_EVENT_INIT_BIT] = "INIT",
	[DP_EVENT_ALLOC_PORT_BIT] = "ALLOC_PORT",
	[DP_EVENT_DE_ALLOC_PORT_BIT] = "DE_ALLOC_PORT",
	[DP_EVENT_REGISTER_DEV_BIT] = "REGISTER_DEV",
	[DP_EVENT_DE_REGISTER_DEV_BIT] = "DE_REGISTER_DEV",
	[DP_EVENT_REGISTER_SUBIF_BIT] = "REGISTER_SUBIF",
	[DP_EVENT_DE_REGISTER_SUBIF_BIT] = "DEREGISTER_SUBIF",
	[DP_EVENT_OWNER_SWITCH_BIT] = "OWNER_SWITCH"
};

int proc_dp_event_list_dump(struct seq_file *s, int pos)
{
	struct dp_evt_notif_info *evt_notif;
	struct notifier_block *nb, *next_nb;
	int i;

	if (!capable(CAP_SYS_PACCT))
		return -1;
	if (!dp_port_prop[pos].valid)
		goto NEXT;

	dp_sprintf(s, "Inst[%d]\n", pos);
	DP_LIB_LOCK(&dp_lock);
	nb = rcu_dereference_raw(dp_evt_notif_list.head);
	while (nb) {
		next_nb = rcu_dereference_raw(nb->next);
		evt_notif = container_of(nb, struct dp_evt_notif_info, nb);
		if (evt_notif->evt_info.inst != pos) {
			nb = next_nb;
			continue;
		}
		if (evt_notif->evt_info.owner < DP_EVENT_OWNER_MAX)
			dp_sprintf(s, " owner: %s\n",
					dp_event_owner_str[evt_notif->evt_info.owner]);

		dp_sputs(s, " type:\n");
		for (i = 0; i < DP_EVENT_MAX_BIT; i++)
			if (evt_notif->evt_info.type & BIT(i))
				dp_sprintf(s, "  %s\n", dp_event_type_str[i]);

		dp_sprintf(s, " id: 0x%px\n", evt_notif->evt_info.id);
		nb = next_nb;
	}
	DP_LIB_UNLOCK(&dp_lock);
NEXT:
	pos++;
	if (pos == DP_MAX_INST)
		return -1;
	return pos;
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP
