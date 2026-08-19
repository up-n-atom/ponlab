// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 *****************************************************************************/

#include <linux/list.h>
#include <net/switch_api/gsw_types.h>
#include <net/switch_api/lantiq_gsw.h>
#include <net/switch_api/gsw_dev.h>
#include <net/switch_api/lantiq_gsw_flow.h>
#include <net/datapath_api.h>
#include <net/flow_dissector.h>
#include <net/pkt_cls.h>
#include <net/tc_act/tc_mirred.h>
#include <linux/version.h>
#include <net/tc_act/tc_gact.h>
#include <net/tc_act/tc_vlan.h>
#include "qos_tc_flower.h"
#include "qos_tc_pce.h"
#include "qos_tc_parser.h"
#include "qos_tc_mirred.h"
#include "qos_tc_ext_vlan.h"
#include "qos_tc_qos.h"

struct qos_tc_mirr_filter {
	int proto;
	struct flow_dissector_key_vlan mask;
	bool drop;
	enum pce_type pce_type;
};

static int qos_tc_parse_flower(struct net_device *dev,
			       struct flow_cls_offload *f,
			       struct qos_tc_mirr_filter *flt)
{
	struct flow_dissector *d = qos_tc_get_dissector(f);

	memset(flt, 0, sizeof(*flt));

	if (d->used_keys &
			~(BIT(FLOW_DISSECTOR_KEY_CONTROL) |
			  BIT(FLOW_DISSECTOR_KEY_BASIC) |
			  BIT(FLOW_DISSECTOR_KEY_VLAN) |
			  BIT(FLOW_DISSECTOR_KEY_CVLAN) |
			  BIT(FLOW_DISSECTOR_KEY_ICMP) |
			  BIT(FLOW_DISSECTOR_KEY_META) |
			  BIT(FLOW_DISSECTOR_KEY_ETH_ADDRS))) {
		pr_debug("%s: Unsupported key used: 0x%x\n", __func__,
			 d->used_keys);
		return -EINVAL;
	}
	pr_debug("%s: Supported key used: 0x%x\n", __func__, d->used_keys);

	flt->proto = f->common.protocol;

	/* Classification/Matching arguments parsing */
	if (dissector_uses_key(d, FLOW_DISSECTOR_KEY_VLAN)) {
		struct flow_dissector_key_vlan *mask =
			skb_flow_dissector_target(d, FLOW_DISSECTOR_KEY_VLAN,
						  qos_tc_get_mask(f));
		netdev_dbg(dev, "%s: pcp: %#x\n", __func__, mask->vlan_id);
		flt->mask = *mask;
	}

	return 0;
}

static int qos_tc_flow_get_bp_id(struct net_device *dev)
{
	dp_subif_t *subif;
	int err;

	subif = kzalloc(sizeof(*subif), GFP_KERNEL);
	if (!subif) {
		netdev_err(dev, "%s: failed to allocate memory for subif\n",
			   __func__);
		return -ENOMEM;
	}
	err = dp_get_netif_subifid(dev, NULL, NULL, NULL, subif, 0);
	if (err != DP_SUCCESS)
		err = -ENODEV;
	else
		err = subif->bport;
	kfree(subif);

	return err;
}

static int qos_tc_set_forward_port(struct net_device *dev,
				   struct net_device *mirr_dev,
				   u16 nForwardPortMap[16])
{
	int bp = 0, pidx;

	bp = qos_tc_flow_get_bp_id(mirr_dev);
	if (bp < 0) {
		netdev_err(dev, "%s: cannot get BP id from DPM for %s\n",
			   __func__, netdev_name(mirr_dev));
		return bp;
	}
	if (bp >= 16 * 16) {
		netdev_err(dev, "%s: bridge port (%i) is too big\n",
			   __func__, bp);
		return -EINVAL;
	}

	/* Set destination bridge port id in 256 bit nForwardPortMap
	 * which is split into 16 u16 blocks.
	 */
	pidx = ((bp / 16)) % 16;
	nForwardPortMap[pidx] |= 1 << (bp - (pidx << 4));

	return 0;
}

static int qos_tc_parse_act_mirred(struct net_device *dev,
				   struct flow_cls_offload *f,
				   u16 nForwardPortMap[16],
				   bool *drop_act,
				   struct net_device **mirr_dev)
{
	const struct flow_action_entry *a;
	int i;
	int ret = 0;

	*drop_act = false;
	*mirr_dev = NULL;

	if (f->rule->action.num_entries == 0)
		return -EINVAL;

	flow_action_for_each(i, a, &f->rule->action) {
		/* If one of the actions is a drop action just configure a drop
		 * rule and no forwarding. Normal drop rules are configured in
		 * the extended VLAN configuration, this only gets called if
		 * the first action is a mirred action.
		 */
		if (a->id == FLOW_ACTION_DROP) {
			*drop_act =  true;
			return 0;
		}
	}

	/* We support one drop action (is_tcf_gact_shot()) or multiple mirred
	 * redirect actions. If multiple mirred redirect actions are piped
	 * together the traffic will be duplicated and forwarded to all
	 * these devices.
	 */
	flow_action_for_each(i, a, &f->rule->action) {
		if (a->id == FLOW_ACTION_REDIRECT ||
		    a->id == FLOW_ACTION_REDIRECT_INGRESS) {
			*mirr_dev = a->dev;
			if (!*mirr_dev)
				return -ENODEV;

			if (a->id == FLOW_ACTION_REDIRECT) {
				ret = qos_tc_set_forward_port(dev, *mirr_dev,
							      nForwardPortMap);
				if (ret)
					return ret;
			}
		} else {
			/* Only accept pipe actions here which are used to pipe
			 * together multiple mirred redirect rules.
			 */
			netdev_err(dev, "%s: unsupported action: %i\n",
				   __func__, a->id);
			return -EINVAL;
		}
	}

	netdev_dbg(dev, "%s: ret: %d\n", __func__, ret);
	return ret;
}

static void qos_tc_set_flt_pce_type(struct net_device *dev,
				    struct qos_tc_mirr_filter *flt)
{
	if (eth_type_vlan(flt->proto)) {
		if (!flt->mask.vlan_id) {
			/* wildcard */
			netdev_dbg(dev, "%s: Mirring VLAN wildcard\n",
				   __func__);
			flt->pce_type = PCE_MIRR_VLAN_WILD;
		}

		if (flt->mask.vlan_id == 0xfff) {
			/* single */
			if (flt->drop) {
				pr_debug("%s: Mirring VLAN drop\n",
					 __func__);
				flt->pce_type = PCE_MIRR_VLAN_DROP;
			} else {
				pr_debug("%s: Mirring VLAN forward\n",
					 __func__);
				flt->pce_type = PCE_MIRR_VLAN_FWD;
			}
		}
	} else {
		/* untagged */
		if (flt->drop) {
			netdev_dbg(dev, "%s: Mirring VLAN untag drop\n",
				   __func__);
			flt->pce_type = PCE_MIRR_UNTAG_DROP;
		} else {
			netdev_dbg(dev, "%s: Mirring VLAN untag forward\n",
				   __func__);
			flt->pce_type = PCE_MIRR_UNTAG_FWD;
		}
	}
}

static bool is_type_mirred_ingress_redirect(struct flow_cls_offload *f)
{
	return has_action_id(f, FLOW_ACTION_REDIRECT_INGRESS);
}

static bool force_port_forwarding(struct net_device *dev,
				  struct net_device *mirr_dev,
				  struct flow_cls_offload *f)
{
	if (qos_tc_parse_is_mcc(dev, f))
		return true;

	if (qos_tc_is_iphost_dev(mirr_dev))
		return true;

	return false;
}

int qos_tc_mirred_offload(struct net_device *dev,
			  struct flow_cls_offload *f,
			  unsigned long cookie)
{
	struct qos_tc_mirr_filter flt = {0};
	struct net_device *mirr_dev;
	GSW_PCE_rule_t *pce_rule = NULL;
	int pref = f->common.prio >> 16;
	u16 nForwardPortMap[16] = {0,};
	int ret = 0;

	/* Only mirred egress and
	 * redirect egress and ingress are supported
	 */
	if (has_action_id(f, FLOW_ACTION_MIRRED_INGRESS))
		return -EOPNOTSUPP;

	ret = qos_tc_parse_flower(dev, f, &flt);
	if (ret != 0)
		return ret;

	ret = qos_tc_parse_act_mirred(dev, f, nForwardPortMap, &flt.drop,
				      &mirr_dev);
	if (ret != 0)
		return ret;

	/* call extended vlan handling for mirred ingress redirect action */
	if (is_type_mirred_ingress_redirect(f)) {
		netdev_dbg(dev, "%s: MIRRED ingress redirect handling\n",
			   __func__);
		ret = qos_tc_ext_vlan_add(dev, f, true, mirr_dev);
		return ret;
	}

	pce_rule = kzalloc(sizeof(*pce_rule), GFP_KERNEL);
	if (!pce_rule)
		return -ENOMEM;

	pce_rule->pattern.bEnable = 1;
	flt.pce_type = PCE_COMMON;

	ret = qos_tc_to_pce(dev, f, pce_rule);
	if (ret != 0) {
		kfree(pce_rule);
		return ret;
	}

	if (qos_tc_parse_is_mcc(dev, f)) {
		/* use only on reinserted packets */
		pce_rule->pattern.bInsertionFlag_Enable = 1;
		pce_rule->pattern.nInsertionFlag =
			GSW_PCE_INSERT_MATCH_INSERT_FLAG;
	}

	qos_tc_set_flt_pce_type(dev, &flt);

	if (flt.drop) {
		qos_tc_set_drop(dev, f, pce_rule);
	} else {
		if (force_port_forwarding(dev, mirr_dev, f)) {
			pce_rule->action.ePortMapAction =
				GSW_PCE_ACTION_PORTMAP_ALTERNATIVE;
		} else {
			pce_rule->action.ePortFilterType_Action =
				GSW_PCE_PORT_FILTER_ACTION_1;
		}

		memcpy(pce_rule->action.nForwardPortMap, nForwardPortMap,
		       sizeof(pce_rule->action.nForwardPortMap));
	}

	netdev_dbg(dev, "%s: PCE rule prepared\n", __func__);

	ret = qos_tc_pce_rule_create(dev, cookie, pref, flt.pce_type, pce_rule);
	if (ret != 0) {
		kfree(pce_rule);
		return ret;
	}

	kfree(pce_rule);

	ret = qos_tc_flower_storage_add(dev, f->cookie, TC_TYPE_MIRRED,
					NULL, NULL);
	if (ret < 0) {
		(void)qos_tc_pce_rule_delete(cookie, pref);
		return ret;
	}

	return 0;
}

int qos_tc_mirred_unoffload(struct net_device *dev,
			    struct flow_cls_offload *f,
			    unsigned long cookie)
{
	int pref = f->common.prio >> 16;
	int ret = 0;

	ret = qos_tc_pce_rule_delete(cookie, pref);
	return ret;
}
