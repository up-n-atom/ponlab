// SPDX-License-Identifier: GPL-2.0
/******************************************************************************
 *
 * Copyright (c) 2021 - 2023 MaxLinear, Inc.
 *
 *****************************************************************************/

#include <net/switch_api/gsw_types.h>
#include <net/switch_api/lantiq_gsw.h>
#include <net/switch_api/gsw_dev.h>
#include <net/switch_api/lantiq_gsw_flow.h>
#include <net/datapath_api.h>
#include <net/flow_dissector.h>
#include <net/pkt_cls.h>
#include "qos_tc_flower.h"
#include "qos_tc_parser.h"
#include "qos_tc_pce.h"
#include "qos_tc_ip_drop.h"
#include "qos_tc_qos.h"

static void qos_tc_ip_drop_set_default(struct net_device *dev,
				       GSW_PCE_rule_t *pce_rule)
{
	/* Set default values for ip drop action */
	pce_rule->pattern.bEnable = 1;
	pce_rule->pattern.bPortIdEnable = 1;
	pce_rule->pattern.bSubIfIdEnable = 1;
	pce_rule->pattern.bSLAN_Vid = 1;
	pce_rule->pattern.nSLAN_Vid = 0;
	pce_rule->pattern.nOuterVidRange = 4095;
	pce_rule->pattern.bSVidRange_Select = 1;
	pce_rule->pattern.bInsertionFlag_Enable = 1;
	/* INS=2 means match on no special tag (set for LAN ports). */
	if (IS_ENABLED(CONFIG_MXL_ETHSW_NOSPTAG) && qos_tc_is_lan_dev(dev))
		pce_rule->pattern.nInsertionFlag = GSW_PCE_INSERT_MATCH_NO_STAG;
	else
		pce_rule->pattern.nInsertionFlag =
			GSW_PCE_INSERT_MATCH_NO_INSERT_FLAG;
	pce_rule->action.nForwardPortMap[0] = 0x0000;

	return;
}

int qos_tc_ip_drop_offload(struct net_device *dev,
			   struct flow_cls_offload *f,
			   unsigned long cookie)
{
	GSW_PCE_rule_t *pce_rule = NULL;
	int pref = f->common.prio >> 16;
	int ret = 0;

	pce_rule = kzalloc(sizeof(*pce_rule), GFP_KERNEL);
	if (!pce_rule) {
		ret = -ENOMEM;
		goto err;
	}

	qos_tc_ip_drop_set_default(dev, pce_rule);

	ret = qos_tc_to_pce(dev, f, pce_rule);
	if (ret != 0) {
		kfree(pce_rule);
		goto err;
	}

	qos_tc_set_drop(dev, f, pce_rule);

	netdev_dbg(dev, "%s: PCE rule prepared\n", __func__);

	ret = qos_tc_pce_rule_create(dev, cookie, pref, PCE_UNCOMMON, pce_rule);
	if (ret != 0) {
		kfree(pce_rule);
		goto err;
	}

	kfree(pce_rule);

	ret = qos_tc_flower_storage_add(dev, f->cookie, TC_TYPE_IP_DROP,
					NULL, NULL);
	if (ret < 0) {
		(void)qos_tc_pce_rule_delete(cookie, pref);
		goto err;
	}

err:
	return ret;
}

int qos_tc_ip_drop_unoffload(struct net_device *dev,
			     struct flow_cls_offload *f,
			     unsigned long cookie)
{
	int pref = f->common.prio >> 16;
	int ret = 0;

	ret = qos_tc_pce_rule_delete(cookie, pref);
	return ret;
}
