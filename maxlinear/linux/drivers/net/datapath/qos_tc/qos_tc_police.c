// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2020 - 2022 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/

#include <linux/netdevice.h>
#include <linux/types.h>
#include <linux/version.h>
#include <net/pkt_cls.h>
#include <net/tc_act/tc_colmark.h>
#include <net/tc_act/tc_police.h>
#include <linux/list.h>
#include <net/datapath_api.h>
#include <net/datapath_api_qos.h>
#include "qos_tc_flower.h"
#include "qos_tc_vlan_prepare.h"
#include "qos_tc_police.h"

#define QOS_TC_COLMARK_ACTION 0x1
#define QOS_TC_POLICE_ACTION 0x2

struct policer_data {
	struct dp_meter_cfg meter_cfg;
	struct net_device *dev;
	int id;
	int dp_inst;
	int flags;
	int acts;
	struct {
		bool flow_based;
	};
	struct list_head list;
};

static LIST_HEAD(policer_list);

static bool qos_tc_police_match(struct policer_data *p1,
				struct policer_data *p2)
{
	if (p1->dev != p2->dev)
		return false;

	if (p1->meter_cfg.dir != p2->meter_cfg.dir)
		return false;

	/* 1. Meters cannot be attached on same CTP/BP
	 * 2. Color marking and policer can be set independently
	 */

	/* Nor policer nor colmark can be set on device with policer */
	if ((p1->acts & QOS_TC_POLICE_ACTION) &&
	    (p2->acts & QOS_TC_POLICE_ACTION))
		return true;

	if ((p1->acts & QOS_TC_COLMARK_ACTION) &&
	    (p2->acts & QOS_TC_COLMARK_ACTION))
		return true;

	return false;
}

static bool qos_tc_police_parse_actions(struct flow_cls_offload *f,
					struct policer_data *pd,
					dp_subif_t *subif, bool ingress)
{
	struct dp_meter_cfg *meter_cfg = &pd->meter_cfg;
	struct policer_data *p = NULL;
	struct net_device *indev;
	const struct flow_action_entry *a;
	int i;

	pd->acts = 0;
	if (f->rule->action.num_entries == 0)
		return false;

	flow_action_for_each(i, a, &f->rule->action) {
		if (!(pd->acts & QOS_TC_COLMARK_ACTION) &&
		    a->id == FLOW_ACTION_COLMARK) {
			/* The enumerations in the TC colmark are matching
			 * enumerations in the dp manager.
			 */
			meter_cfg->col_mode = a->colmark.blind;
			meter_cfg->mode = a->colmark.precedence;
			meter_cfg->type = a->colmark.type;

			netdev_dbg(pd->dev,
				   "%s: col_mode %d mode %d type %d\n",
				   __func__,
				   meter_cfg->col_mode,
				   meter_cfg->mode,
				   meter_cfg->type);

			/* Only mode is used for color marking */
			if (a->colmark.flags & COLMARK_F_DROP_PRECEDENCE)
				pd->acts |= QOS_TC_COLMARK_ACTION;
		}

		if (!(pd->acts & QOS_TC_POLICE_ACTION) &&
		    a->id == FLOW_ACTION_POLICE) {
			/* Convert rates from bytes/s to kbit/s */
			meter_cfg->cir = a->police.rate_bytes_ps * 8;
			meter_cfg->pir = a->police.peak_bytes_ps * 8;
			meter_cfg->cbs = a->police.burst;
			meter_cfg->pbs = a->police.mtu;

			netdev_dbg(pd->dev,
				   "%s: cir %llu pir %llu cbs %u pbs %u\n",
				   __func__,
				   meter_cfg->cir,
				   meter_cfg->pir,
				   meter_cfg->cbs,
				   meter_cfg->pbs);

			pd->acts |= QOS_TC_POLICE_ACTION;
		}
	}
	netdev_dbg(pd->dev, "%s: acts: %d\n", __func__, pd->acts);

	meter_cfg->dir = ingress ? DP_DIR_INGRESS : DP_DIR_EGRESS;

	indev = qos_tc_get_indev(pd->dev, f);

	/* On the pmapper and when the indev is the same as the normal dev
	 * we should use the bridge port.
	 */
	if (subif->flag_pmapper || (indev && indev == pd->dev)) {
		pd->flags |= DP_METER_ATTACH_BRPORT;
		pd->flow_based = ingress ? false : true;
	} else {
		pd->flags |= DP_METER_ATTACH_CTP;
	}

	netdev_dbg(pd->dev, "%s: flags: %d\n", __func__, pd->flags);

	/* BP config get failed which should never happen!*/
	if (meter_cfg->dp_pce.flow < 0)
		return false;

	list_for_each_entry(p, &policer_list, list) {
		if (qos_tc_police_match(p, pd)) {
			netdev_err(pd->dev,
				   "%s: policer already on this device\n",
				   __func__);
			return false;
		}
	}

	if (pd->acts && pd->acts <= 3)
		return true;

	return false;
}

static int qos_tc_police_config(struct policer_data *pd, bool en)
{
	int (*cfg)(struct net_device *dev, struct dp_meter_cfg *m, int flag);
	int max = pd->flow_based ? DP_TRAFFIC_TYPE_MAX : 1;
	int ret = 0, type;

	if (en)
		cfg = &dp_meter_add;
	else
		cfg = &dp_meter_del;

	for (type = 0; type < max; type++) {
		pd->meter_cfg.dp_pce.flow = type;
		ret = (cfg)(pd->dev, &pd->meter_cfg, pd->flags);
		if (ret != DP_SUCCESS) {
			netdev_err(pd->dev, "meter config fail\n");
			return ret;
		}
	}

	return 0;
}

int qos_tc_police_offload(struct net_device *dev,
			  struct flow_cls_offload *f,
			  bool ingress)
{
	struct policer_data *pd = NULL;
	struct policer_data tmp = { 0 };
	dp_subif_t *subif;
	int ret;

	subif = kzalloc(sizeof(*subif), GFP_KERNEL);
	if (!subif) {
		netdev_err(dev, "%s: failed to allocate memory for subif\n",
			   __func__);
		return -ENOMEM;
	}
	ret = dp_get_netif_subifid(dev, NULL, NULL, NULL, subif, 0);
	if (ret != DP_SUCCESS) {
		netdev_err(dev, "%s: can not get subif\n", __func__);
		kfree(subif);
		return ret;
	}

	tmp.dev = dev;
	if (!qos_tc_police_parse_actions(f, &tmp, subif, ingress)) {
		netdev_err(dev, "policer or marker actions parse error.\n");
		kfree(subif);
		return -EOPNOTSUPP;
	}

	pd = kzalloc(sizeof(*pd), GFP_KERNEL);
	if (!pd)
		return -ENOMEM;

	*pd = tmp;
	pd->dp_inst = subif->inst;

	if (pd->acts & QOS_TC_POLICE_ACTION) {
		int id;

		ret = dp_meter_alloc(subif->inst, &id, pd->flags);
		if (ret != DP_SUCCESS) {
			netdev_err(dev, "meter alloc fail\n");
			goto out_free;
		}
		pd->meter_cfg.meter_id = id;
		ret = qos_tc_police_config(pd, true);
		if (ret != DP_SUCCESS) {
			netdev_err(dev, "meter config fail\n");
			goto out_dp_remove;
		}
	}
	if (pd->acts & QOS_TC_COLMARK_ACTION) {
		/* No need to allocate a meter_id, it is not used
		 * for color marking.
		 */
		ret = dp_meter_add(dev, &pd->meter_cfg,
				   pd->flags | DP_COL_MARKING);
		if (ret != DP_SUCCESS) {
			netdev_err(dev, "meter config failed: %i\n", ret);
			goto out_free;
		}
	}

	ret = qos_tc_flower_storage_add(dev, f->cookie, TC_TYPE_COLMARK,
					&pd->meter_cfg, pd);
	if (ret < 0) {
		netdev_err(dev, "%s can not add to storage\n", __func__);
		goto out_dp_remove;
	}

	list_add(&pd->list, &policer_list);
	kfree(subif);

	return 0;

out_dp_remove:
	dp_meter_del(dev, &pd->meter_cfg, pd->flags);
out_free:
	kfree(pd);
	kfree(subif);
	return ret;
}

int qos_tc_police_unoffload(struct net_device *dev,
			    void *meter_cfg_ptr, void *flags_ptr)
{
	struct policer_data *pd = flags_ptr;
	struct dp_meter_cfg *meter_cfg = meter_cfg_ptr;
	int ret;

	/* It is not possible to delete the color marking, dp_meter_del() will
	 * only delete the metering in DP manager and not the color marking.
	 * Instead of deleting the setting, just overwrite it with a new one
	 * using the default settings, DP manager does not store any list of
	 * the settings, so this should be save.
	 * Please adapt this to delete the color marking in case
	 * dp_meter_add() or dp_meter_del() change.
	 */
	netdev_dbg(dev, "police action mask: %#x\n", pd->acts);
	if (pd->acts & QOS_TC_COLMARK_ACTION) {
		meter_cfg->col_mode = 0;
		meter_cfg->mode = DP_INTERNAL;
		meter_cfg->type = srTCM;
		ret = dp_meter_add(dev, meter_cfg, pd->flags | DP_COL_MARKING);
		if (ret != DP_SUCCESS) {
			/* We can not do anything when this fails and will
			 * probably leak some memory because we do not know
			 * how much was deleted.
			 */
			netdev_err(dev, "%s: del failed: %i\n", __func__, ret);
		}
	}

	if (pd->acts & QOS_TC_POLICE_ACTION) {
		netdev_dbg(dev, "del meter id: %u\n", meter_cfg->meter_id);
		ret = qos_tc_police_config(pd, false);
		if (ret != DP_SUCCESS)
			netdev_err(dev, "meter del failed: %i\n", ret);
		ret = dp_meter_alloc(pd->dp_inst, &meter_cfg->meter_id, DP_F_DEREGISTER);
		if (ret != DP_SUCCESS)
			netdev_err(dev, "meter dealloc failed: %i\n", ret);
	}

	list_del(&pd->list);
	kfree(pd);
	return 0;
}

static void print_dp_meter_cfg(struct seq_file *file,
			       struct dp_meter_cfg p)
{
	seq_puts(file, "dp_meter_cfg:\n");

	seq_printf(file, "\tmeter_id: %d\n", p.meter_id);

	switch (p.type) {
	case srTCM:
		seq_puts(file, "\ttype: srTCM\n");
		break;
	case trTCM:
		seq_puts(file, "\ttype: trTCM\n");
		break;
	default:
		seq_puts(file, "\ttype: unknown\n");
		break;
	}

	seq_printf(file, "\tcir: %llu\n", p.cir);
	seq_printf(file, "\tpir: %llu\n", p.pir);
	seq_printf(file, "\tcbs: %d\n", p.cbs);
	seq_printf(file, "\tpbs: %d\n", p.pbs);
	seq_printf(file, "\tcol_mode: %s\n", p.col_mode ? "true" : "false");
	seq_printf(file, "\tdir: %d\n", p.dir);

	switch (p.mode) {
	case DP_NO_MARKING:
		seq_puts(file, "\tmode: DP_NO_MARKING\n");
		break;
	case DP_INTERNAL:
		seq_puts(file, "\tmode: DP_INTERNAL\n");
		break;
	case DP_DEI:
		seq_puts(file, "\tmode: DP_DEI\n");
		break;
	case DP_PCP_8P0D:
		seq_puts(file, "\tmode: DP_PCP_8P0D\n");
		break;
	case DP_PCP_7P1D:
		seq_puts(file, "\tmode: DP_PCP_7P1D\n");
		break;
	case DP_PCP_6P2D:
		seq_puts(file, "\tmode: DP_PCP_6P2D\n");
		break;
	case DP_PCP_5P3D:
		seq_puts(file, "\tmode: DP_PCP_5P3D\n");
		break;
	case DP_DSCP_AF:
		seq_puts(file, "\tmode: DP_DSCP_AF\n");
		break;
	default:
		seq_puts(file, "\tmode: unknown\n");
		break;
	}

	switch (p.dp_pce.flow) {
	case DP_OTHERS:
		seq_puts(file, "\tflow: DP_OTHERS\n");
		break;
	case DP_UKNOWN_UNICAST:
		seq_puts(file, "\tflow: DP_UKNOWN_UNICAST\n");
		break;
	case DP_MULTICAST:
		seq_puts(file, "\tflow: DP_MULTICAST\n");
		break;
	case DP_BROADCAST:
		seq_puts(file, "\tflow: DP_BROADCAST\n");
		break;
	case DP_TRAFFIC_TYPE_MAX:
		seq_puts(file, "\tflow: DP_TRAFFIC_TYPE_MAX\n");
		break;
	default:
		seq_puts(file, "\tflow: unknown\n");
		break;
	}
}

static void print_policer_data(struct seq_file *file,
			       struct policer_data *p)
{
	seq_printf(file, "dev: %s\n", p->dev->name);
	seq_printf(file, "id: %d\n", p->id);
	seq_printf(file, "dp_inst: %d\n", p->dp_inst);
	seq_printf(file, "flags: %d\n", p->flags);
	seq_printf(file, "acts: %d\n", p->acts);
	seq_printf(file, "flow_based: %s\n", p->flow_based ? "true" : "false");

	print_dp_meter_cfg(file, p->meter_cfg);

	seq_puts(file, "\n");
}

void qos_tc_policer_list_debugfs(struct seq_file *file, void *ctx)
{
	struct policer_data *p = NULL;

	rtnl_lock();
	list_for_each_entry(p, &policer_list, list) {
		print_policer_data(file, p);
	}
	rtnl_unlock();
}
