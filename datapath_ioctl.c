// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2022, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include "datapath_ioctl.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

int dp_get_ts_info2(struct net_device *dev,
		    struct ethtool_ts_info *ts_info)
{
	struct mac_ops *ops;
	dp_subif_t *subif;
	int inst = 0;
	int err = 0;
	struct pmac_port_info *port_info;

	subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif)
		return DP_FAILURE;
	if (dp_get_netif_subifid(dev, NULL, NULL, NULL, subif, 0)) {
		pr_err("DPM: %s dp_get_netif_subifid failed for %s\n",
		       __func__, dev->name);
		kfree(subif);
		return -EFAULT;
	}

	port_info = get_dp_port_info(inst, subif->port_id);

	/* PTP is applicable to only physical & non-physical
	 * active Ethernet ports,
	 * For Prx300 portid should be 2,3,4
	 * For LGM portid should be 2 ... 10
	 */
	if (port_info->alloc_flags & (DP_F_FAST_ETH_LAN |
				      DP_F_FAST_ETH_WAN)) {
		ops = dp_port_prop[inst].mac_ops[subif->port_id];
		if (!ops) {
			kfree(subif);
			return -EFAULT;
		}
		err = ops->mac_get_ts_info(ops, ts_info);
		if (err < 0) {
			kfree(subif);
			return -EFAULT;
		}
		DP_DEBUG(DP_DBG_FLAG_INST, "done:%s\n", dev->name);
	}

	/* NOTE: Timestamp should not be reported for all other ports and
	 * subif, report back 0, otherwise tcpdump for non-physical ports will
	 * fail
	 */
	kfree(subif);
	return 0;
}
EXPORT_SYMBOL(dp_get_ts_info2);

/* check whether ndo is set already or not */
int is_dp_ndo_cb_set(void **dev_ops, void *dp_new_ops, int ndo_offset,
			     void **dp_saved_ops, int *ops_cnt)
{
	unsigned long addr1, addr2;
	/* if one of ndo_xx is already hacked, ops_cnt should be non-zero */
	if (!*ops_cnt)
		return false;
	/* if one of ndo_ already hacked, then *dev_ops == dp_new_ops */
	if (*dev_ops != dp_new_ops) {
		pr_err("DPM: %s: dev_ops(0x%px) != dp_new_ops(0x%px)\n",
		       __func__, *dev_ops, dp_new_ops);
		/* note: here return true to avoid further ops pdate */
		return true;
	}
	/* if one of ndo_xx is already hacked, dev_ops should be non-NULL */
	if (!dp_new_ops)
		return false;

	addr1 = (unsigned long)dp_new_ops + ndo_offset;
	if (*(unsigned long *)addr1 == 0)
		return false;
	if(*dp_saved_ops) {
		addr2 = (unsigned long)*dp_saved_ops + ndo_offset;
		if (*(unsigned long *)addr1 == *(unsigned long *)addr2)
			return false;
	}
	return true;
}

/* offset: unit in bytes */
int dp_ndo_ops_set(void *ops_base, const int offset, const void *new_ops)
{
	unsigned long addr;

	if (!ops_base) {
		pr_err("DPM: %s: ops_base NULL\n", __func__);
		return DP_FAILURE;
	}
	if (offset % sizeof(void *)) {
		pr_err("DPM: %s: wrong offset=%d\n", __func__, offset);
		return DP_FAILURE;
	}
	addr = (unsigned long)ops_base + offset;
	*(unsigned long *)addr = (unsigned long )new_ops;

	return DP_SUCCESS;
}

void *dp_ndo_ops_get(void *ops_base, const int offset)
{
	void **addr = NULL;

	if (!ops_base) {
		pr_err("DPM: %s: ops_base NULL\n", __func__);
		return NULL;
	}
	if (offset % sizeof(void *)) {
		pr_err("DPM: %s: wrong offset=%d\n", __func__, offset);
		return NULL;
	}
	addr = ops_base + offset;
	return *addr;
}

/* offset: unit in bytes */
int dp_ndo_ops_reset(void *ops_base, const int offset, void *saved_ops_base)
{
	unsigned long orig_addr;
	unsigned long new_addr = 0;

	if (saved_ops_base) {
		orig_addr = (unsigned long)saved_ops_base + offset;
		new_addr = *(unsigned long *)orig_addr;
	}
	return dp_ndo_ops_set(ops_base, offset, (void *)new_addr);
}

/* dp_ops_set: to set/reset one ndo_xxx in one specific group of ops,
 * for exaple: set/reset ndo_bridge_setlink in struct net_device_ops of dev
 * dev_ops: dev's ops pointer, for example: dev->dp_new_ops
 *          Note: this dev's ops will be changed accordingly
 * ndo_offset: offset in bytes to get ndo_xxx in the spcified ops struct
 * ops_size: whole ops struct size, for example: sizeof(struct net_device_ops)
 * dp_saved_ops: to save dev's original ops pointer.
 *               Note, this pointer will be changed
 * dp_new_ops: new ops address for device
 * ops_cnt: point to the ops reference cnt
 * ndo_cb: ndo_xx in spcified group of ops, for example: new_ndo_bridge_setlink
 *         in struct net_device_ops
 *         Note: It is required to be set for DP_OPS_RESET. Otherwise it can be
 *               NULL.
 * ops_cnt: the point of ops reference counter, to tell how many ndo_xxx is
 *          changed in the specified grop of ops
 *         struct net_device_ops
 *         It is used for DP_OPS_RESET only
 * flag: if DP_OPS_RESET not set, it is to change its ndo_cb, otherwise restore
 *       to its original ndo_cb
 */
int dp_ops_set(void **dev_ops, const int ndo_offset, size_t ops_size,
		    void **dp_saved_ops, void *dp_new_ops,
		    const void *ndo_cb, int *ops_cnt, int *all_ops_cnt, u32 flag)
{
	int err = DP_FAILURE;

	if (!ops_cnt) {
		pr_err("DPM: %s ops_cnt NULL\n", __func__);
		return err;
	}
	if (!all_ops_cnt) {
		pr_err("DPM: %s all_ops_cnt NULL\n", __func__);
		return err;
	}
	if (!dp_new_ops) {
		pr_err("DPM: %s dp_new_ops NULL\n", __func__);
		return err;
	}
	if (!dp_saved_ops) {
		pr_err("DPM: %s old_dev_ops NULL\n", __func__);
		return err;
	}

	if (flag & DP_OPS_RESET) {
		/* Set ops with original callback */
		if (!dp_ndo_ops_reset(dp_new_ops, ndo_offset, *dp_saved_ops)) {
			(*ops_cnt)--;
			(*all_ops_cnt)--;
			if (!*ops_cnt) {
				/* restore to original ops */
				*dev_ops = *dp_saved_ops;
				*dp_saved_ops = NULL;
				dp_memset(dp_new_ops, 0, ops_size);
			}
			err = DP_SUCCESS;
		} else {
			pr_err("DPM: %s: fail to reset ndo_ at offset:%d\n", __func__,
			       ndo_offset);
		}
	} else {
		/* check current ndo__ is set already or not */
		if(is_dp_ndo_cb_set(dev_ops, dp_new_ops, ndo_offset,
				    dp_saved_ops, ops_cnt)) {
			pr_err("DPM: %s: ndo_ already set at offset:%d\n", __func__,
			       ndo_offset);
			return err;
		}
		/* Set ops with new callback */
		if (!dp_ndo_ops_set(dp_new_ops, ndo_offset, ndo_cb)) {
			if (!*ops_cnt) {
				/* save whole netdev_ops*/
				*dp_saved_ops = *dev_ops;
				if (*dev_ops)
					dp_memcpy(dp_new_ops, *dev_ops, ops_size);
				/* point to new dp_new_ops */
				*dev_ops = dp_new_ops;
			}
			(*ops_cnt)++;
			(*all_ops_cnt)++;
			dp_ndo_ops_set(dp_new_ops, ndo_offset, ndo_cb);
			err = DP_SUCCESS;
		} else {
			pr_err("DPM: %s: fail to set ndo_ at offset:%d\n", __func__,
			       ndo_offset);
		}
	}
	return err;
}

int dp_ndo_do_ioctl2(struct net_device *dev,
		     struct ifreq *ifr, int cmd)
{
	int err = 0;
	struct mac_ops *ops;
	int inst = 0;
	struct pmac_port_info *port;
	struct dp_dev *dp_dev = NULL;
	dp_subif_t *subif;

	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev) {
		pr_err("DPM: %s: dp_dev NULL\n", __func__);
		return -EFAULT;
	}

	/* DP handles only SIOCSHWTSTAMP and SIOCGHWTSTAMP */
	if (cmd != SIOCSHWTSTAMP && cmd != SIOCGHWTSTAMP) {
		if (dp_dev->old_dev_ops->ndo_do_ioctl)
			err = dp_dev->old_dev_ops->ndo_do_ioctl(dev, ifr, cmd);
		else
			return -EFAULT;
		return err;
	}

	subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif)
		return DP_FAILURE;
	if (dp_get_netif_subifid(dev, NULL, NULL, NULL, subif, 0)) {
		kfree(subif);
		pr_err("DPM: %s DP get subifid fail\n", __func__);
		return -EFAULT;
	}

	port = get_dp_port_info(inst, subif->port_id);

	ops = dp_port_prop[inst].mac_ops[subif->port_id];
	if (!ops) {
		kfree(subif);
		return -EFAULT;
	}

	switch (cmd) {
	case SIOCSHWTSTAMP:
		err = ops->set_hwts(ops, ifr);
		if (err < 0) {
			port->f_ptp = 0;
			break;
		}
		port->f_ptp = 1;
		DP_DEBUG(DP_DBG_FLAG_DBG,
			 "PTP in SIOCGHWTSTAMP done\n");
		break;
	case SIOCGHWTSTAMP:
		ops->get_hwts(ops, ifr);
		DP_DEBUG(DP_DBG_FLAG_DBG,
			 "PTP in SIOCGHWTSTAMP done\n");
		break;
	default:
		break;
	}

	kfree(subif);
	return err;
}
EXPORT_SYMBOL(dp_ndo_do_ioctl2);

int dp_register_ptp_ioctl(struct net_device *dev, int reset)
{
#if IS_ENABLED(CONFIG_PTP_1588_CLOCK)
	struct dp_dev *dp_dev;
	struct dp_cap *cap;
	static const void *cb[] = {&dp_ndo_do_ioctl, &dp_get_ts_info};
	static const int offset[] = {
		offsetof(const struct net_device_ops, ndo_do_ioctl),
		offsetof(const struct ethtool_ops, get_ts_info)
	};
	static const char *memo[] = {"dpm-ndo_do_ioctl", "dpm-get_ts_info"};
	u32 flag[] = {DP_OPS_NETDEV, DP_OPS_ETHTOOL};
	int i;

	DP_DEBUG(DP_DBG_FLAG_OPS, "ptp_ops %s for %s\n",
		 reset ? "reset" : "update", dev->name);
	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev)
		return DP_FAILURE;
	if (dp_dev->inst < 0) {
		pr_err("DPM: %s: wrong inst=%d for %s\n",
			__func__, dp_dev->inst, dev->name);
		return DP_FAILURE;
	}
	cap = dp_kzalloc(sizeof(*cap), GFP_ATOMIC);
	if (!cap)
		return DP_FAILURE;
	cap->inst = dp_dev->inst;
	if (dp_get_cap(cap, 0)) {
		kfree(cap);
		pr_err("DPM: %s: cap failed for %s\n", __func__, dev->name);
		return DP_FAILURE;
	}
	if (!cap->hw_ptp) {
		kfree(cap);
		return DP_FAILURE;
	}

	for (i = 0; i < ARRAY_SIZE(offset); i++) {
		if (reset)
			flag[i] |= DP_OPS_RESET;
		if (dp_set_net_dev_ops_priv(dp_dev->dev, cb[i], offset[i],
					    flag[i], memo[i])) {
			pr_err("DPM: %s failed to register ops %d\n", __func__, i);
			kfree(cap);
			return DP_FAILURE;
		}
	}

	kfree(cap);
#endif
	return DP_SUCCESS;
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP
