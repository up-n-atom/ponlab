/******************************************************************************
 **
 ** FILE NAME   : qos_mgr_hook.c
 ** PROJECT   : UGW
 ** MODULES   : QoS Manager Hooks
 **
 ** DATE   : 2 Jan 2020
 ** AUTHOR   : Mohammed Aarif
 ** DESCRIPTION : QoS Manager Hook Pointers
 ** COPYRIGHT  : Copyright (c) 2020 - 2023 MaxLinear, Inc.
 **		 Copyright (c) 2019 - 2020 Intel Corporation
 **
 **   For licensing information, see the file 'LICENSE' in the root folder of
 **   this software module.
 **
 ** HISTORY
 ** $Date $Author $Comment
 ******************************************************************************/

/*
 * Common Head File
 */
#include <linux/version.h>
#include <generated/autoconf.h>
#include <asm/atomic.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/netdevice.h>
#include <linux/atmdev.h>
#include <net/sock.h>
#include <net/ip.h>
#include <net/ipv6.h>
#include <net/protocol.h>
#include <net/xfrm.h>
#include <linux/once.h>
#include <linux/jhash.h>
#include <linux/skbuff.h>
#include <net/netns/hash.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv6.h>
#include <linux/netfilter_bridge.h>
#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nfnetlink_conntrack.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/netfilter/nf_conntrack_ecache.h>
#include <net/netfilter/nf_conntrack_tuple.h>
#include <net/netfilter/nf_conntrack_acct.h>
/*
 * Chip Specific Head File
 */
#include <net/qos_mgr/qos_mgr_common.h>
#include <net/datapath_api.h>

int32_t (*qos_mgr_hook_setup_tc)(struct net_device *dev, enum tc_setup_type type, void *type_data) = NULL;
int32_t (*qos_mgr_hook_setup_tc_ext)(struct net_device *dev, enum tc_setup_type type, void *type_data, int32_t deq_idx, int32_t port_id, int32_t flags) = NULL;

EXPORT_SYMBOL(qos_mgr_hook_setup_tc);
EXPORT_SYMBOL(qos_mgr_hook_setup_tc_ext);

int32_t (*qos_mgr_br2684_get_vcc_fn)(struct net_device *netdev,
		struct atm_vcc **pvcc) = NULL;
EXPORT_SYMBOL(qos_mgr_br2684_get_vcc_fn);

struct qos_mgr_class2prio_notifier_info {
       s32 port_id;
       struct net_device *dev;
       u8 class2prio[MAX_TC_NUM];
};

static RAW_NOTIFIER_HEAD(qos_mgr_event_chain);

int qos_mgr_register_event_notifier(struct notifier_block *nb)
{
	return raw_notifier_chain_register(&qos_mgr_event_chain, nb);
}
EXPORT_SYMBOL(qos_mgr_register_event_notifier);

int qos_mgr_unregister_event_notifier(struct notifier_block *nb)
{
	return raw_notifier_chain_unregister(&qos_mgr_event_chain, nb);
}
EXPORT_SYMBOL(qos_mgr_unregister_event_notifier);

int qos_mgr_call_class2prio_notifiers(unsigned long val,
				  s32 port_id, struct net_device *dev,
				  u8 class2prio[MAX_TC_NUM])
{
	struct qos_mgr_class2prio_notifier_info info;

	info.port_id = port_id;
	info.dev = dev;
	memcpy(info.class2prio, class2prio, (sizeof(u8) * MAX_TC_NUM));

	return raw_notifier_call_chain(&qos_mgr_event_chain, val, &info);
}
EXPORT_SYMBOL(qos_mgr_call_class2prio_notifiers);
