/* SPDX-License-Identifier: GPL-2.0-or-later */
#if LINUX_VERSION_IS_GEQ(6,0,0)
#include_next <net/dropreason.h>
#else 
#ifndef _LINUX_DROPREASON_H
#define _LINUX_DROPREASON_H
#include "dropreason-core.h"

#define SKB_CONSUMED 0
#define SKB_NOT_DROPPED_YET 1
/**
 * enum skb_drop_reason_subsys - subsystem tag for (extended) drop reasons
 */
enum skb_drop_reason_subsys {
	/** @SKB_DROP_REASON_SUBSYS_CORE: core drop reasons defined above */
	SKB_DROP_REASON_SUBSYS_CORE,

	/**
	 * @SKB_DROP_REASON_SUBSYS_MAC80211_UNUSABLE: mac80211 drop reasons
	 * for unusable frames, see net/mac80211/drop.h
	 */
	SKB_DROP_REASON_SUBSYS_MAC80211_UNUSABLE,

	/**
	 * @SKB_DROP_REASON_SUBSYS_MAC80211_MONITOR: mac80211 drop reasons
	 * for frames still going to monitor, see net/mac80211/drop.h
	 */
	SKB_DROP_REASON_SUBSYS_MAC80211_MONITOR,

	/**
	 * @SKB_DROP_REASON_SUBSYS_OPENVSWITCH: openvswitch drop reasons,
	 * see net/openvswitch/drop.h
	 */
	SKB_DROP_REASON_SUBSYS_OPENVSWITCH,

	/** @SKB_DROP_REASON_SUBSYS_NUM: number of subsystems defined */
	SKB_DROP_REASON_SUBSYS_NUM
};
#endif
#endif
