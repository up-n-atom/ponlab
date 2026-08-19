// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2020 - 2024, MaxLinear, Inc.
 * Copyright 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <linux/types.h>
#include <net/datapath_api.h>
#include <net/datapath_proc_api.h>
#include <linux/kallsyms.h>
#if IS_ENABLED(CONFIG_SOC_LGM)
	#include <net/toe_np_lro.h>
#elif IS_ENABLED(CONFIG_LGM_TOE)
	#include <net/intel_np_lro.h>
#endif
#if IS_ENABLED(CONFIG_QOS_TC)
#include <net/qos_tc.h>
#endif
#include "datapath.h"
#include "datapath_instance.h"
#include "datapath_swdev.h"
#include "datapath_ioctl.h"

#if IS_ENABLED(CONFIG_INTEL_VPN) || IS_ENABLED(CONFIG_MXL_VPN)
#include <net/datapath_api_vpn.h>
#include <net/xfrm.h>
#endif
#if IS_ENABLED(CONFIG_QOS_TC) || IS_ENABLED(CONFIG_QOS_MGR)
#define DP_ENABLE_TC_OFFLOADS
#endif

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

int dp_cap_num;
struct dp_hw_cap hw_cap_list[DP_MAX_HW_CAP];

/* dev hash list */
struct hlist_head dp_dev_list[DP_DEV_HASH_SIZE];

/*Module hash list */
struct hlist_head dp_mod_list[DP_MOD_HASH_SIZE];

struct kmem_cache *cache_dev_list;
static struct kmem_cache *cache_mod_list;

#if !IS_ENABLED(CONFIG_DPM_DATAPATH_PARTIAL_FEEDS)
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
int (*qos_tc_setup_fn)(struct net_device *dev,
		       u32 handle,
		       __be16 protocol,
		       struct tc_to_netdev *tc,
		       int port_id,
		       int deq_idx);
#else
int (*qos_tc_setup_fn)(struct net_device *dev,
		       enum tc_setup_type type,
		       void *type_data,
		       int port_id,
		       int deq_idx);
#endif /* LINUX_VERSION_CODE */
EXPORT_SYMBOL(qos_tc_setup_fn);
#endif /* CONFIG_DPM_DATAPATH_PARTIAL_FEEDS */

char *dp_get_sym_name_by_addr(void *symaddr, char *symname,
		const char *fail_str)
{
	int ret;
	ret = sprint_symbol_no_offset(symname, (unsigned long)symaddr);
	if (ret == 0 || symname[0] == '\0' ||
		(symname[0] == '0' && symname[1] == 'x'))
		if (fail_str)
			dp_strlcpy(symname, fail_str, strlen(fail_str)+1);

	return symname;
}

struct hlist_head *get_dp_dev_list(void)
{
	return dp_dev_list;
}
EXPORT_SYMBOL(get_dp_dev_list);

int register_dp_hw_cap(struct dp_hw_cap *info, u32 flag)
{
	int i;

	if (!info) {
		pr_err("DPM: %s: NULL info\n", __func__);
		return -1;
	}
	for (i = 0; i < DP_MAX_HW_CAP; i++) {
		if (hw_cap_list[i].valid)
			continue;
		hw_cap_list[i].valid = 1;
		hw_cap_list[i].info = info->info;
		dp_cap_num++;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_EXTRA_DEBUG)
		pr_err("DPM: Succeed to %s HAL[%d]: type=%d ver=%d dp_cap_num=%d\n",
		       "Register",
		       i,
		       info->info.type,
		       info->info.ver,
		       dp_cap_num);
#endif
		return 0;
	}
	pr_err("DPM: Failed to %s HAL: type=%d ver=%d\n",
	       "Register",
	       info->info.type,
	       info->info.ver);
	return -1;
}

/*return value:
 *succeed: return 0 with info->inst updated
 *fail: -1
 */
int dp_request_inst(struct dp_inst_info *info, u32 flag)
{
	int i, k, j;
	struct inst_property *dp_prop;

	if (!info)
		return -1;

	if (flag & DP_F_DEREGISTER) {
		/*do de-register */

		return 0;
	}
	/*register a dp instance */

	/*to check whether any such matched HW cap */
	for (k = 0; k < DP_MAX_HW_CAP; k++) {
		if (!hw_cap_list[k].valid)
			continue;
		if (hw_cap_list[k].info.type == info->type &&
		    hw_cap_list[k].info.ver == info->ver) {
			break;
		}
	}
	if (k == DP_MAX_HW_CAP) {
		pr_err("DPM: %s fail to match cap type=%d/ver=%d\n",
		       __func__, info->type, info->ver);
		return -1;
	}

	/* to find a free instance */
	for (i = 0; i < DP_MAX_INST; i++) {
		if (!dp_port_prop[i].valid)
			break;
	}
	if (i == DP_MAX_INST) {
		pr_err("DPM: %s fail for dp inst full arealdy\n", __func__);
		return -1;
	}
	if (alloc_dma_chan_tbl(i)) {
		pr_err("DPM: FAIL to alloc dma chan tbl\n");
		return -1;
	}

	dp_prop = get_dp_port_prop(i);

	dp_prop->ops[0] = info->ops[0];
	dp_prop->ops[1] = info->ops[1];

	for (j = 0; j < DP_MAX_MAC_HANDLE; j++) {
		if (info->mac_ops[j])
			dp_prop->mac_ops[j] = info->mac_ops[j];
	}

	dp_prop->info = hw_cap_list[k].info;
	dp_prop->cbm_inst = info->cbm_inst;
	dp_prop->qos_inst = info->qos_inst;
	dp_prop->valid = 1;
#ifdef CONFIG_LTQ_DATAPATH_CPUFREQ
	dp_cpufreq_notify_init(i);
	DP_DEBUG(DP_DBG_FLAG_COC, "DP registered CPUFREQ notifier\n");
#endif
	if (alloc_dp_port_subif_info(i)) {
		pr_err("DPM: alloc_dp_port_subif_info fail..\n");
		return DP_FAILURE;
	}
	if (dp_prop->info.dp_platform_set(i, 0) < 0) {
		dp_prop->valid = 0;
		pr_err("DPM: %s failed for inst=%d\n", __func__, i);
		return -1;
	}
	info->inst = i;
	dp_inst_num++;
	DP_DEBUG(DP_DBG_FLAG_INST,
		 "%s ok: inst=%d, dp_inst_num=%d\n", __func__, i, dp_inst_num);
	return 0;
}
EXPORT_SYMBOL(dp_request_inst);

struct dp_hw_cap *match_hw_cap(struct dp_inst_info *info, u32 flag)
{
	int k;

	for (k = 0; k < DP_MAX_HW_CAP; k++) {
		if (!hw_cap_list[k].valid)
			continue;
		if (hw_cap_list[k].info.type == info->type &&
		    hw_cap_list[k].info.ver == info->ver) {
			return &hw_cap_list[k];
		}
	}
	return NULL;
}

/*Note: like pon one device can have multiple ctp,
 *ie, it may register multiple times
 */
u32 dp_dev_hash(void *dev)
{
	return hash_ptr(dev, DP_DEV_HASH_BIT_LENGTH);
}

struct dp_dev *dp_dev_lookup(struct net_device *dev)
{
	struct hlist_head *head;
	struct dp_dev *item;
	u32 idx;

	idx = dp_dev_hash(dev);
	head = &dp_dev_list[idx];

	hlist_for_each_entry(item, head, hlist) {
		if (item->dev == dev)
			return item;
	}
	return NULL;
}

#if defined(DP_ENABLE_TC_OFFLOADS)
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 14, 0)
int dp_ndo_setup_tc2(struct net_device *dev, u32 handle,
		    __be16 protocol, struct tc_to_netdev *tc)
{
#if IS_ENABLED(CONFIG_QOS_TC)
	if (qos_tc_setup_fn)
		return qos_tc_setup_fn(dev, handle, protocol, tc, -1, -1);
#endif

#if IS_ENABLED(CONFIG_QOS_MGR)
	if (qos_mgr_hook_setup_tc)
		return qos_mgr_hook_setup_tc(dev, handle, protocol, tc);
#endif
	if (dev->netdev_ops->ndo_setup_tc) {
		pr_err("DPM: Cannot support ndo_setup_tc\n");
		return -1;
	}
	return -1;
}
#else /* LINUX_VERSION_CODE */
int dp_ndo_setup_tc2(struct net_device *dev,
		    enum tc_setup_type type,
		    void *type_data)
{
#if IS_ENABLED(CONFIG_QOS_TC)
	if (qos_tc_setup_fn)
		return qos_tc_setup_fn(dev, type, type_data, -1, -1);
#endif

	return -1;
}
#endif /* LINUX_VERSION_CODE */
#else
int dp_ndo_setup_tc2(struct net_device *dev,
		    int tc_setup_type,
		    void *type_data)
{
	return -1;
}

#endif /* DP_ENABLE_TC_OFFLOADS */
EXPORT_SYMBOL(dp_ndo_setup_tc2);

static int dp_inst_register_xfrm_ops(struct net_device *dev, int reset)
{
#if IS_ENABLED(CONFIG_INTEL_VPN) || IS_ENABLED(CONFIG_MXL_VPN)
#if IS_ENABLED(CONFIG_MXL_VPN)
	struct mxl_vpn_ops *vpn;
#else
	struct intel_vpn_ops *vpn;
#endif
	struct dp_dev *dp_dev;
	int i;
	u32 flag = DP_OPS_XFRMDEV;
	static const int offset[] = {
		offsetof(struct xfrmdev_ops, xdo_dev_state_add),
		offsetof(struct xfrmdev_ops, xdo_dev_state_delete),
		offsetof(struct xfrmdev_ops, xdo_dev_offload_ok),
		offsetof(struct xfrmdev_ops, xdo_dev_state_advance_esn)
	};
	static const char *memo[] = {
		"dpm-xdo_dev_state_add",
		"dpm-xdo_dev_state_delete",
		"dpm-xdo_dev_offload_ok",
		"dpm-xdo_dev_state_advance_esn"
	};
	void *cb[ARRAY_SIZE(offset)] = {NULL};
	u32 pflag = (DP_F_FAST_ETH_WAN | DP_F_FAST_DSL | DP_F_VUNI |
		     DP_F_DOCSIS | DP_F_GPON | DP_F_EPON);
	struct pmac_port_info *port_info;

	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev)
		return DP_FAILURE;

	port_info = get_dp_port_info(dp_dev->inst, dp_dev->ep);
	if (!(port_info->alloc_flags & pflag))
		return DP_SUCCESS;

	DP_DEBUG(DP_DBG_FLAG_OPS, "xfrm_ops %s for %s\n",
		 reset ? "reset" : "update", dev->name);
	vpn = dp_get_vpn_ops(0);
	if (!vpn) {
		netdev_err(dev, "Invalid vpn ops\n");
		return DP_FAILURE;
	}

	if (reset) {
		flag |= DP_OPS_RESET;
		dev->features &= ~NETIF_F_HW_ESP;
		dev->hw_enc_features &= ~NETIF_F_HW_ESP;
		dev->vlan_features &= ~NETIF_F_HW_ESP;
	} else {
		dev->features |= NETIF_F_HW_ESP;
		dev->hw_enc_features |= NETIF_F_HW_ESP;
		dev->vlan_features |= NETIF_F_HW_ESP;
		cb[0] =	vpn->add_xfrm_sa;
		cb[1] = vpn->delete_xfrm_sa;
		cb[2] = vpn->xfrm_offload_ok;
		cb[3] = vpn->state_advance_esn;
	}

	for (i = 0; i < ARRAY_SIZE(offset); i++) {
		if (dp_set_net_dev_ops_priv(dp_dev->dev, cb[i], offset[i],
					    flag, memo[i])) {
			pr_err("DPM: %s failed to set ops %d\n", __func__, i);
			return DP_FAILURE;
		}
	}
#endif
	return DP_SUCCESS;
}

int dp_dev_update_xfrm2(struct net_device *dev)
{
#if IS_ENABLED(CONFIG_MXL_VPN)
	dev->features |= NETIF_F_HW_ESP;
	dev->hw_enc_features |= NETIF_F_HW_ESP;
	dev->vlan_features |= NETIF_F_HW_ESP;
#endif
	return 0;
}
EXPORT_SYMBOL(dp_dev_update_xfrm2);

int dp_xdo_dev_state_add2(struct xfrm_state *x)
{
#if IS_ENABLED(CONFIG_MXL_VPN)
	struct mxl_vpn_ops *vpn;

	vpn = dp_get_vpn_ops(0);
	if (!vpn) {
		pr_err("Invalid vpn ops\n");
		return -1;
	}
	return vpn->add_xfrm_sa(x);
#endif
	return -1;
}
EXPORT_SYMBOL(dp_xdo_dev_state_add2);

void dp_xdo_dev_state_delete2(struct xfrm_state *x)
{
#if IS_ENABLED(CONFIG_MXL_VPN)
	struct mxl_vpn_ops *vpn;

	vpn = dp_get_vpn_ops(0);
	if (!vpn) {
		pr_err("Invalid vpn ops\n");
		return;
	}
	vpn->delete_xfrm_sa(x);
#endif
}
EXPORT_SYMBOL(dp_xdo_dev_state_delete2);

bool dp_xdo_dev_offload_ok2(struct sk_buff *skb, struct xfrm_state *x)
{
#if IS_ENABLED(CONFIG_MXL_VPN)
	struct mxl_vpn_ops *vpn;

	vpn = dp_get_vpn_ops(0);
	if (!vpn) {
		pr_err("Invalid vpn ops\n");
		return false;
	}
	return vpn->xfrm_offload_ok(skb, x);
#endif
	return false;
}
EXPORT_SYMBOL(dp_xdo_dev_offload_ok2);

void dp_xdo_dev_state_advance_esn2(struct xfrm_state *x)
{
#if IS_ENABLED(CONFIG_MXL_VPN)
	struct mxl_vpn_ops *vpn;

	vpn = dp_get_vpn_ops(0);
	if (!vpn) {
		pr_err("Invalid vpn ops\n");
		return;
	}
	vpn->state_advance_esn(x);
#endif
}
EXPORT_SYMBOL(dp_xdo_dev_state_advance_esn2);

static void dp_inst_register_toe(struct net_device *dev, int reset)
{
#if IS_ENABLED(CONFIG_LGM_TOE) || IS_ENABLED(CONFIG_SOC_LGM)
	struct dp_dev *dp_dev;
	struct pmac_port_info *port_info;
	struct lro_ops *ops;

	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev)
		return;
	port_info = get_dp_port_info(dp_dev->inst, dp_dev->ep);

	/* remove TOE features if TOE is not loaded at the moment */
	ops = dp_get_lro_ops();
	if (!ops)
		return;

	if (!reset)
		netif_set_gso_max_size(dp_dev->dev,
				       ops->get_gso_max_size(ops->toe));

	ops->cfg_netdev_feature(ops->toe, dev, !reset);
#endif
}

int dp_dev_update_toe2(struct net_device *dev)
{
#if IS_ENABLED(CONFIG_LGM_TOE) || IS_ENABLED(CONFIG_SOC_LGM)
	struct lro_ops *ops;

	ops = dp_get_lro_ops();
	if (!ops)
		return -1;

	netif_set_gso_max_size(dev, ops->get_gso_max_size(ops->toe));
	ops->cfg_netdev_feature(ops->toe, dev, true);
#endif
	return 0;
}
EXPORT_SYMBOL(dp_dev_update_toe2);

static void dp_inst_register_tc_ops(struct net_device *dev, int reset)
{
#if defined(DP_ENABLE_TC_OFFLOADS)
	int offset;
	struct dp_dev *dp_dev;

	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev) {
		pr_err("DPM: %s: not found %s\n", __func__, dev->name);
		return;
	}

	offset = offsetof(const struct net_device_ops, ndo_setup_tc);
	dp_set_net_dev_ops_priv(dev, &dp_ndo_setup_tc, offset,
				DP_OPS_NETDEV | (reset ? DP_OPS_RESET : 0),
				"dpm-ndo_setup_tc");
	if (!reset) {
		dp_dev->f_orig_tc_flag =
			dev->features & NETIF_F_HW_TC ? true : false;
		/* add NETIF_F_HW_TC feature flag only if originally don't
		 * have
		 */
		if (!dp_dev->f_orig_tc_flag)
			dev->features |= NETIF_F_HW_TC;
	} else {
		/* Remove NETIF_F_HW_TC feature flag only if originally don't
		 * have
		 */
		if (!dp_dev->f_orig_tc_flag)
			dev->features &= ~NETIF_F_HW_TC;
	}
#endif
}

int dp_dev_update_tc2(struct net_device *dev)
{
#if defined(DP_ENABLE_TC_OFFLOADS)
	dev->features |= NETIF_F_HW_TC;
#endif
	return 0;
}
EXPORT_SYMBOL(dp_dev_update_tc2);

int get_ndo_offset_idx(struct ndo_offset *offset_arr, int offset)
{
	int i;

	for (i =0 ; i < MAX_NDO_PER_OPS; i++) {
		if (!offset_arr[i].f_hacked)
			continue;
		if (offset_arr[i].offset == offset)
			return i;
	}
	return -1;
}

int get_free_ndo_offset_idx(struct ndo_offset *offset_arr)
{
	int i;

	for (i =0 ; i < MAX_NDO_PER_OPS; i++) {
		if (offset_arr[i].f_hacked)
			continue;
		return i;
	}
	return -1;
}

int dp_set_net_dev_ops_priv(struct net_device *dev,
	const void *ndo_cb, const int offset, u32 flag, const char *memo)
{
	struct dp_dev *dp_dev;
	void **dev_ops;
	void **org_ops;
	void *new_ops;
	int *ops_cnt;
	int ops_sz;
	int err = DP_FAILURE;
	char *s_type;
	struct ndo_offset *p_offset;
	int ndo_idx;
	char **memo_dst;
	__maybe_unused char ksym[KSYM_SYMBOL_LEN] = {0};

	if (!dev) {
		pr_err("DPM: %s invalid dev\n", __func__);
		return err;
	}

	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev) {
		pr_err("DPM: %s not found dp_dev %s\n", __func__, dev->name);
		goto EXIT;
	}

	switch (flag & ~DP_OPS_RESET) {
	case DP_OPS_NETDEV:
		dev_ops = (void **)&dev->netdev_ops;
		org_ops = (void **)&dp_dev->old_dev_ops;
		new_ops = &dp_dev->new_dev_ops;
		ops_sz = sizeof(*dev->netdev_ops);
		ops_cnt = &dp_dev->dev_ops_cnt;
		s_type = "netdev_ops";
		p_offset = dp_dev->net_dev_offset;
		memo_dst = dp_dev->net_dev_memo;
		break;

	case DP_OPS_ETHTOOL:
		dev_ops = (void **)&dev->ethtool_ops;
		org_ops = (void **)&dp_dev->old_ethtool_ops;
		new_ops = &dp_dev->new_ethtool_ops;
		ops_sz = sizeof(*dev->ethtool_ops);
		ops_cnt = &dp_dev->ethtool_ops_cnt;
		s_type = "ethtool_ops";
		p_offset = dp_dev->ethtool_offset;
		memo_dst = dp_dev->ethtool_memo;
		break;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV) && \
    LINUX_VERSION_CODE < KERNEL_VERSION(5, 15, 15)
	case DP_OPS_SWITCHDEV:
		dev_ops = (void **)&dev->switchdev_ops;
		org_ops = (void **)&dp_dev->old_swdev_ops;
		new_ops = &dp_dev->new_swdev_ops;
		ops_sz = sizeof(*dev->switchdev_ops);
		ops_cnt = &dp_dev->swdev_ops_cnt;
		s_type = "swdev_ops";
		p_offset = dp_dev->swdev_offset;
		memo_dst = dp_dev->swdev_memo;
		break;
#endif
#if IS_ENABLED(CONFIG_INTEL_VPN) || IS_ENABLED(CONFIG_MXL_VPN)
	case DP_OPS_XFRMDEV:
		dev_ops = (void **)&dev->xfrmdev_ops;
		org_ops = (void **)&dp_dev->old_xfrm_ops;
		new_ops = &dp_dev->new_xfrm_ops;
		ops_sz = sizeof(*dev->xfrmdev_ops);
		ops_cnt = &dp_dev->xfrm_ops_cnt;
		s_type = "xfrmdev_ops";
		p_offset = dp_dev->xfrm_offset;
		memo_dst = dp_dev->xfrm_memo;
		break;
#endif
	default:
		pr_err("DPM: %s ops not supported: flag=0x%x\n", __func__, flag);
		goto EXIT;
	}
	DP_DEBUG(DP_DBG_FLAG_OPS, "%s before %s %s[%d] to %s, ops_cnt=%d\n"
		 "                              %s=0x%px %s[cb@%d]: 0x%px %s\n"
		 "                              %s=0x%px %s[cb@%d]: 0x%px %s\n",
		 dev->name,
		 flag & DP_OPS_RESET ? "reset" : "set",
		 s_type, offset, memo, *ops_cnt,
		 "dev_ops", *dev_ops,
		 s_type, offset, dp_ndo_ops_get(*dev_ops, offset),
		 dp_get_sym_name_by_addr(dp_ndo_ops_get(*dev_ops, offset), ksym, memo),
		 "dpm_ops", new_ops,
		 s_type, offset, dp_ndo_ops_get(new_ops, offset),
		 dp_get_sym_name_by_addr(dp_ndo_ops_get(new_ops, offset), ksym, memo));
	ndo_idx = get_ndo_offset_idx(p_offset, offset);
	if (flag & DP_OPS_RESET) { /* reset ndo */
		if (ndo_idx < 0) {
			/* no need to reset since not set yet */
			err = 0;
			goto EXIT;
		}
	} else { /* set ndo */
		if (ndo_idx >= 0) {
			/* santity check only */
			unsigned long addr = (unsigned long)*dev_ops;
			addr += offset;
			if (*(unsigned long *)addr == (unsigned long)ndo_cb) {
				/* set with same ndo_cb */
				pr_err("DPM: %s: ndo_x already set with it %s at offset=%d %s\n",
				       __func__, dev->name, offset, s_type);
				err = 0;
				goto EXIT;
			}
			pr_err("DPM: %s: ndo_x not allowed to set to new one %s at offset=%d %s\n",
			       __func__, dev->name, offset, s_type);
			goto EXIT;
		}
		ndo_idx = get_free_ndo_offset_idx(p_offset);
		if (ndo_idx < 0) {
			pr_err("DPM: %s: ndo_x no free entry for %s at offset=%d %s\n",
			       __func__, dev->name, offset, s_type);
			goto EXIT;
		}
	}
	err = dp_ops_set(dev_ops, offset, ops_sz,
			 org_ops, new_ops, ndo_cb, ops_cnt,
			 &dp_dev->all_ops_cnt, flag);
	if (err) {
		pr_err("DPM: %s: fail %s ops=%s offset=%d ops_sz=%d for %s\n",
		       __func__,
		       flag & DP_OPS_RESET ? "reset" : "set",
		       s_type, offset, ops_sz, dev->name);
		goto EXIT;
	}
	if (flag & DP_OPS_RESET) {
		p_offset[ndo_idx].f_hacked = false;
		p_offset[ndo_idx].offset = -1;
		if (memo_dst)
			memo_dst[ndo_idx] = NULL;
	} else {
		p_offset[ndo_idx].f_hacked = true;
		p_offset[ndo_idx].offset = offset;
		if (memo_dst)
			memo_dst[ndo_idx] = (char *)memo;
	}
	switch (flag & ~DP_OPS_RESET) {
		case DP_OPS_NETDEV:
			dev_ops = (void **)&dev->netdev_ops;
			break;

		case DP_OPS_ETHTOOL:
			dev_ops = (void **)&dev->ethtool_ops;
			break;

#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV) && \
	    LINUX_VERSION_CODE < KERNEL_VERSION(5, 15, 15)
		case DP_OPS_SWITCHDEV:
			dev_ops = (void **)&dev->switchdev_ops;
			break;
#endif
#if IS_ENABLED(CONFIG_INTEL_VPN) || IS_ENABLED(CONFIG_MXL_VPN)
		case DP_OPS_XFRMDEV:
			dev_ops = (void **)&dev->xfrmdev_ops;
			break;
#endif
		default:
			pr_err("DPM: %s ops not supported: flag=0x%x\n", __func__, flag);
			goto EXIT;
	}
	DP_DEBUG(DP_DBG_FLAG_OPS, "%s after  %s %s[%d] to %s, ops_cnt=%d\n"
		 "                              %s=0x%px %s[cb@%d]: 0x%px %s\n"
		 "                              %s=0x%px %s[cb@%d]: 0x%px %s\n",
		 dev->name,
		 flag & DP_OPS_RESET ? "reset" : "set",
		 s_type, offset, memo, *ops_cnt,
		 "dev_ops", *dev_ops,
		 s_type, offset, dp_ndo_ops_get(*dev_ops, offset),
		 dp_get_sym_name_by_addr(dp_ndo_ops_get(*dev_ops, offset), ksym, memo),
		 "dpm_ops", new_ops,
		 s_type, offset, dp_ndo_ops_get(new_ops, offset),
		 dp_get_sym_name_by_addr(dp_ndo_ops_get(new_ops, offset), ksym, memo));
EXIT:
	return err;
}
EXPORT_SYMBOL(dp_set_net_dev_ops_priv);

u32 dp_mod_hash(struct module *owner, u16 ep)
{
	return hash_ptr(owner, DP_MOD_HASH_BIT_LENGTH);
}

struct dp_mod *dp_mod_lookup(struct hlist_head *head, struct module *owner,
			     u16 ep, u32 flag)
{
	struct dp_mod *item;

	hlist_for_each_entry(item, head, hlist) {
		if (item->mod == owner && item->ep == ep)
			return item;
	}
	return NULL;
}

/* tuple: owner + ep
 * act: inst
 */
int dp_inst_insert_mod(struct module *owner, u16 ep, u32 inst, u32 flag)
{
	struct dp_mod *dp_mod;
	u8 new_f = 0;
	u32 idx;

	if (!owner) {
		pr_err("DPM: %s owner: NULL?\n", __func__);
		return -1;
	}
	idx = dp_mod_hash(owner, ep);
	DP_DEBUG(DP_DBG_FLAG_INST, "dp_mod_list idx=%u\n", idx);
	dp_mod = dp_mod_lookup(&dp_mod_list[idx], owner, ep, flag);
	if (!dp_mod) { /*alloc new */
		dp_mod = kmem_cache_zalloc(cache_mod_list, GFP_ATOMIC);
		if (dp_mod) {
			dp_mod->mod = owner;
			dp_mod->ep = ep;
			dp_mod->inst = inst;
			new_f = 1;
		}
	}
	if (!dp_mod)
		return -1;
	if (new_f)
		hlist_add_head(&dp_mod->hlist, &dp_mod_list[idx]);
	DP_DEBUG(DP_DBG_FLAG_INST, "owner: %s\n", owner->name);
	return 0;
}

int dp_inst_del_mod(struct module *owner, u16 ep, u32 flag)
{
	struct dp_mod *dp_mod;
	u32 idx;

	if (!owner) {
		pr_err("DPM: %s owner: NULL?\n", __func__);
		return -1;
	}
	idx = dp_mod_hash(owner, ep);
	dp_mod = dp_mod_lookup(&dp_mod_list[idx], owner, ep, flag);
	if (!dp_mod) {
		pr_err("DPM: Failed to dp_mod_lookup: %s\n",
		       owner->name);
		return -1;
	}
	hlist_del(&dp_mod->hlist);
	kmem_cache_free(cache_mod_list, dp_mod);

	DP_DEBUG(DP_DBG_FLAG_INST, "ok: %s:\n", owner->name);
	return 0;
}

int dp_get_inst_via_module(struct module *owner,  u16 ep, u32 flag)
{
	struct dp_mod *dp_mod;
	u32 idx;

	if (!owner) {
		pr_err("DPM: owner NULL?\n");
		return -1;
	}
	idx = dp_mod_hash(owner, ep);
	dp_mod = dp_mod_lookup(&dp_mod_list[idx], owner, ep, flag);
	if (!dp_mod) {
		pr_err("DPM: Failed to dp_mod_lookup: %s\n",
		       owner->name);
		return -1;
	}

	return dp_mod->inst;
}

/* if dev NULL, use subif_name, otherwise use dev to search */
int dp_get_inst_via_dev(struct net_device *dev, char *subif_name,
			u32 flag)
{
	struct dp_dev *dp_dev;

	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev)
		return 0; /*workaround:otherwise caller need to check value */

	return dp_dev->inst;
}

struct dev_proc_data {
	u32 hash_idx;
	struct dp_dev *entry;
};
int proc_dev_dump(struct seq_file *s, int pos)
{
	struct dp_proc_file_entry *p;
	struct dev_proc_data *data;
	struct dp_dev_subif *dev_sif_pos, *dev_sif_next;

	if (!capable(CAP_SYS_PACCT))
		return -1;

	p = (struct dp_proc_file_entry *) s->private;
	if (!p || !p->data) {
		if (!p)
			pr_err("DPM: p NULL\n");
		else
			pr_err("DPM: p->data NULL\n");
		return -1;
	}
	data = p->data;

	while (!data->entry) {
		data->hash_idx++;
		if (data->hash_idx == DP_DEV_HASH_SIZE) {
			return -1;
		}
		data->entry =
			hlist_entry_safe((&dp_dev_list[data->hash_idx])->first,
					 struct dp_dev, hlist);
	}
	/*             hash    pos     dev      state f_ctp  inst   ep      bp      count   cb_cnt  bridge */
	dp_sprintf(s, "%s=%-4u %s=%-2d %s=%-15s %-1s  %-7s %s=%2d %s=%-2d %s=%-3d %s=%-2d %s=%-2d br=%-8s ",
		   "Hash", data->hash_idx,
		   "pos", pos,
		   "dev", data->entry->dev->name,
		   data->entry->count ? "R" : "",
		   data->entry->f_ctp_dev ? "ctp_dev" : "",
		   "inst", data->entry->inst,
		   "ep", data->entry->ep,
		   "bp", data->entry->bp,
		   "count", data->entry->count,
		   "ops_cnt", data->entry->all_ops_cnt,
		   data->entry->br_info ?
			data->entry->br_info->dev->name : "");
	if (!list_empty(&data->entry->subif_list)) {
		dp_sputs(s, "vap=");
		list_for_each_entry_safe(dev_sif_pos, dev_sif_next,
					 &data->entry->subif_list, list) {
			dp_sprintf(s, "%x/ %s=%d", dev_sif_pos->subif, "vlan_aware",
					   dev_sif_pos->sif->vlan_aware_en);
		}
	}
	dp_sputs(s, "\n");
	data->entry = hlist_entry_safe((data->entry)->hlist.next,
					     struct dp_dev, hlist);
	pos++;
	return pos;
}

int proc_dev_start(void *param)
{
	struct dp_proc_file_entry *p;
	struct dev_proc_data *data = dp_kzalloc(sizeof(*data), GFP_ATOMIC);

	if (!data)
		return 0;
	p = param;
	p->data = data;
	data->hash_idx = 0;
	data->entry = hlist_entry_safe((&dp_dev_list[data->hash_idx])->first,
				       struct dp_dev, hlist);
	return 0;
}

static char dp_proc_dev_name[IFNAMSIZ];
ssize_t proc_dev_ops_write(struct file *file, const char *buf,
				    size_t count, loff_t *ppos)
{
	int n;

	if (!capable(CAP_SYS_PACCT))
		return count;
	n = count >= IFNAMSIZ - 2 ? IFNAMSIZ - 2 : count;
	if (dp_copy_from_user(dp_proc_dev_name, buf, n))
		return -EFAULT;
	dp_proc_dev_name[n + 1] = 0;
	dp_replace_ch(dp_proc_dev_name, n, '\r', 0);
	dp_replace_ch(dp_proc_dev_name, n, '\n', 0);

	return count;
}

int proc_dev_ops_dump(struct seq_file *s, int pos)
{
	struct dp_proc_file_entry *p;
	struct dev_proc_data *data;
	int i, cnt;
	char ksym_name_str[KSYM_SYMBOL_LEN] = {0};

	if (!capable(CAP_SYS_PACCT))
		return -1;

	p = (struct dp_proc_file_entry *) s->private;
	if (!p || !p->data) {
		if (!p)
			pr_err("DPM: p NULL\n");
		else
			pr_err("DPM: p->data NULL\n");
		return -1;
	}
	data = p->data;

	if (pos == 0 && strlen(dp_proc_dev_name) )
		dp_sprintf(s, "%s\n", dp_proc_dev_name);

	while (!data->entry) {
		data->hash_idx++;
		if (data->hash_idx == DP_DEV_HASH_SIZE) {
			return -1;
		}
		data->entry =
			hlist_entry_safe((&dp_dev_list[data->hash_idx])->first,
					 struct dp_dev, hlist);
	}
	if (strlen(dp_proc_dev_name) &&
	    strcmp(dp_proc_dev_name, data->entry->dev->name))
		goto NEXT;

	cnt = data->entry->dev_ops_cnt + data->entry->ethtool_ops_cnt;
#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV) && \
    LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
	cnt += data->entry->swdev_ops_cnt;
#endif
#if IS_ENABLED(CONFIG_INTEL_VPN) || IS_ENABLED(CONFIG_MXL_VPN)
	cnt += data->entry->xfrm_ops_cnt;
#endif
	if (data->entry->all_ops_cnt != cnt)
		pr_err("DPM: all_ops_cnt not match for %s: %d != %d\n",
		       data->entry->dev->name, data->entry->all_ops_cnt, cnt);

	dp_sprintf(s, "%s: all_ops_cnt=%d\n", data->entry->dev->name,
		   data->entry->all_ops_cnt);

	if (data->entry->dev_ops_cnt) {
		dp_sprintf(s, "    .dev_ops_cnt=%d old/new/curr netdev_ops=0x%px/0x%px/0x%px\n",
			   data->entry->dev_ops_cnt,
			   data->entry->old_dev_ops,
			   &data->entry->new_dev_ops,
			   data->entry->dev->netdev_ops);
		for (i = 0; i < MAX_NDO_PER_OPS; i++) {
			if (!data->entry->net_dev_offset[i].f_hacked)
				continue;
			dp_get_sym_name_by_addr(dp_ndo_ops_get(
						(void *)data->entry->dev->netdev_ops,
						data->entry->net_dev_offset[i].offset),
					ksym_name_str, data->entry->net_dev_memo[i]);
			dp_sprintf(s, "        offset=%-3d : fun=%s\n",
				   data->entry->net_dev_offset[i].offset,
				   ksym_name_str);
		}
	}

	if (data->entry->ethtool_ops_cnt) {
		dp_sprintf(s, "    .ethtool_ops_cnt=%d old/new/curr ethtool_ops=0x%px/0x%px/0x%px\n",
			   data->entry->ethtool_ops_cnt,
			   data->entry->old_ethtool_ops,
			   &data->entry->new_ethtool_ops,
			   data->entry->dev->ethtool_ops);
		for (i = 0; i < MAX_NDO_PER_OPS; i++) {
			if (!data->entry->ethtool_offset[i].f_hacked)
				continue;
			dp_get_sym_name_by_addr(dp_ndo_ops_get(
						(void *)data->entry->dev->ethtool_ops,
						data->entry->ethtool_offset[i].offset),
					ksym_name_str, data->entry->ethtool_memo[i]);
			dp_sprintf(s, "        offset=%-3d : fun=%s\n",
				   data->entry->ethtool_offset[i].offset,
				   ksym_name_str);
		}
	}

#if IS_ENABLED(CONFIG_DPM_DATAPATH_SWITCHDEV) && \
	    LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
	if (data->entry->swdev_ops_cnt) {
		dp_sprintf(s, "    .swdev_ops_cnt=%d old/new/curr swdev_ops=0x%px/0x%px/0x%px\n",
			   data->entry->swdev_ops_cnt,
			   data->entry->old_swdev_ops,
			   &data->entry->new_swdev_ops,
			   data->entry->dev->switchdev_ops);

		for (i = 0; i < MAX_NDO_PER_OPS; i++) {
			if (!data->entry->swdev_offset[i].f_hacked)
				continue;
			dp_get_sym_name_by_addr(dp_ndo_ops_get(
						(void *)data->entry->dev->switchdev_ops,
						data->entry->swdev_offset[i].offset),
					ksym_name_str, data->entry->swdev_memo[i]);
			dp_sprintf(s, "        offset=%-3d : fun=%s\n",
				   data->entry->swdev_offset[i].offset,
				   ksym_name_str);
		}
	}
#endif

#if IS_ENABLED(CONFIG_INTEL_VPN) || IS_ENABLED(CONFIG_MXL_VPN)
	if (data->entry->xfrm_ops_cnt) {
		dp_sprintf(s, "    .xfrm_ops_cnt=%d old/new/curr xfrm_ops=0x%px/0x%px/0x%px\n",
			   data->entry->xfrm_ops_cnt,
			   data->entry->old_xfrm_ops,
			   &data->entry->new_xfrm_ops,
			   data->entry->dev->xfrmdev_ops);
		for (i = 0; i < MAX_NDO_PER_OPS; i++) {
			if (!data->entry->xfrm_offset[i].f_hacked)
				continue;
			dp_get_sym_name_by_addr(dp_ndo_ops_get(
						(void *)data->entry->dev->xfrmdev_ops,
						data->entry->xfrm_offset[i].offset),
					ksym_name_str, data->entry->xfrm_memo[i]);
			dp_sprintf(s, "        offset=%-3d : fun=%s\n",
				   data->entry->xfrm_offset[i].offset,
				   ksym_name_str);
		}
	}
#endif
NEXT:
	data->entry = hlist_entry_safe((data->entry)->hlist.next,
					     struct dp_dev, hlist);
	pos++;
	return pos;
}

int proc_dev_ops_start(void *param)
{
	struct dp_proc_file_entry *p;
	struct dev_proc_data *data = dp_kzalloc(sizeof(*data), GFP_ATOMIC);

	if (!data)
		return 0;
	p = param;
	p->data = data;
	data->hash_idx = 0;
	data->entry = hlist_entry_safe((&dp_dev_list[data->hash_idx])->first,
				       struct dp_dev, hlist);
	return 0;
}

static void dump_cap(struct seq_file *s, struct dp_cap *cap)
{
	if (!s)
		return;
	dp_sprintf(s, "	HW TX checksum offloading: %s\n",
		   cap->tx_hw_chksum ? "Yes" : "No");
	dp_sprintf(s, "	HW RX checksum verification: %s\n",
		   cap->rx_hw_chksum ? "Yes" : "No");
	dp_sprintf(s, "	HW TSO: %s\n",
		   cap->hw_tso ? "Yes" : "No");
	dp_sprintf(s, "	HW GSO: %s\n",
		   cap->hw_gso ? "Yes" : "No");
	dp_sprintf(s, "	QOS Engine: %s\n",
		   cap->qos_eng_name);
	dp_sprintf(s, "	Pkt Engine: %s\n",
		   cap->pkt_eng_name);
	dp_sprintf(s, "	max_num_queues: %d\n",
		   cap->max_num_queues);
	dp_sprintf(s, "	max_num_scheds: %d\n",
		   cap->max_num_scheds);
	dp_sprintf(s, "	max_num_deq_ports: %d\n",
		   cap->max_num_deq_ports);
	dp_sprintf(s, "	max_num_qos_ports: %d\n",
		   cap->max_num_qos_ports);
	dp_sprintf(s, "	max_num_dp_ports: %d\n",
		   cap->max_num_dp_ports);
	dp_sprintf(s, "	max_num_subif_per_port: %d\n",
		   cap->max_num_subif_per_port);
	dp_sprintf(s, "	max_num_subif: %d\n",
		   cap->max_num_subif);
	dp_sprintf(s, "	max_num_bridge_port: %d\n",
		   cap->max_num_bridge_port);
}

static u32 mod_hash_index;
static struct dp_mod *dp_mod_proc;
int proc_inst_mod_dump(struct seq_file *s, int pos)
{
	if (!capable(CAP_SYS_PACCT))
		return -1;
	while (!dp_mod_proc) {
		mod_hash_index++;
		if (mod_hash_index == DP_MOD_HASH_SIZE)
			return -1;

		dp_mod_proc =
			hlist_entry_safe((&dp_mod_list[mod_hash_index])->first,
					 struct dp_mod, hlist);
	}
	dp_sprintf(s, "Hash=%u pos=%d owner=%s(@0x%px) ep=%d inst=%d\n",
		   mod_hash_index,
		   pos,
		   dp_mod_proc->mod->name,
		   dp_mod_proc->mod,
		   dp_mod_proc->ep,
		   dp_mod_proc->inst);

	dp_mod_proc = hlist_entry_safe((dp_mod_proc)->hlist.next,
				       struct dp_mod, hlist);
	pos++;
	return pos;
}

int proc_inst_dump(struct seq_file *s, int pos)
{
	struct dp_cap *cap;

	if (!capable(CAP_SYS_PACCT))
		return -1;
	if (!dp_port_prop[pos].valid)
		goto NEXT;
	dp_sprintf(s, "Inst[%d] Type=%u ver=%d\n",
		   pos,
		   dp_port_prop[pos].info.type,
		   dp_port_prop[pos].info.ver);
	/*dump_cap(s, &dp_port_prop[pos].info.cap);*/
	cap = dp_kzalloc(sizeof(*cap), GFP_ATOMIC);
	if (!cap)
		return DP_FAILURE;
	cap->inst = pos;
	dp_get_cap(cap, 0);
	dump_cap(s, cap);
	kfree(cap);
NEXT:
	pos++;
	if (pos == DP_MAX_INST)
		return -1;
	return pos;
}

int proc_inst_hal_dump(struct seq_file *s, int pos)
{
	if (!capable(CAP_SYS_PACCT))
		return -1;
	if (!hw_cap_list[pos].valid) {
		if (pos == 0) {
		/* For spl_conn automation test case purpose
		 * before DPM instance is ready
		 */
#if IS_ENABLED(CONFIG_DPM_DATAPATH_HAL_GSWIP31)
			dp_sputs(s, "Inst Type ver=1 Not Ready");
#else
			dp_sputs(s, "Inst Type ver=2 Not Ready");
#endif
		}
		goto NEXT;
	}

	dp_sprintf(s, "HAL[%d] Type=%u ver=%d dp_cap_num=%d\n",
		   pos,
		   hw_cap_list[pos].info.type,
		   hw_cap_list[pos].info.ver,
		   dp_cap_num);
	dump_cap(s, &hw_cap_list[pos].info.cap);

NEXT:
	pos++;
	if (pos == DP_MAX_HW_CAP)
		return -1;
	return pos;
}

int proc_inst_mod_start(void  *param)
{
	mod_hash_index = 0;
	dp_mod_proc = hlist_entry_safe((&dp_mod_list[mod_hash_index])->first,
				       struct dp_mod, hlist);
	return 0;
}

int dp_inst_init(u32 flag)
{
	int i;

	dp_cap_num = 0;
	dp_memset(hw_cap_list, 0, sizeof(hw_cap_list));
	for (i = 0; i < DP_DEV_HASH_SIZE; i++)
		INIT_HLIST_HEAD(&dp_dev_list[i]);

	cache_dev_list = kmem_cache_create("dp_dev_list", sizeof(struct dp_dev),
					   0, SLAB_HWCACHE_ALIGN, NULL);
	if (!cache_dev_list)
		return -ENOMEM;
	cache_mod_list = kmem_cache_create("dp_mod_list", sizeof(struct dp_mod),
					   0, SLAB_HWCACHE_ALIGN, NULL);
	if (!cache_mod_list) {
		kmem_cache_destroy(cache_dev_list);
		return -ENOMEM;
	}
	return 0;
}

void dp_inst_free(void)
{
	kmem_cache_destroy(cache_mod_list);
	kmem_cache_destroy(cache_dev_list);
}

/* flag: pas DP_F_SUBIF_LOGICAL during set new ops
*  f_reset_ops:
 *   0 -- set new ops
 *   1 -- reset to old/original ops
 */
int change_dev_ops_all(struct dp_dev *dp_dev, struct net_device *dev,
				int flag, int f_reset_ops)
{
	struct dp_subif_info *subif_info;
	struct dp_dev_subif *dev_sif;

	dev_sif = list_first_entry(&dp_dev->subif_list,
				struct dp_dev_subif, list);
	if (unlikely(!dev_sif)) {
		pr_err("DPM: %s %d, Error, dev_sif: NULL\n",
				__func__, __LINE__);
		return -1;
	}

	subif_info = dev_sif->sif;
	if (unlikely(!subif_info)) {
		pr_err("DPM: %s %d, Error, subif_info: NULL\n",
				__func__, __LINE__);
		return -1;
	}
	/* don't hack ndo_xxx and capability if caller request */
	if (subif_info->data_flag_ops & DP_SUBIF_NO_NDO_HACK)
		return 0;

	dp_inst_register_tc_ops(dev, f_reset_ops);
	if (!(flag & DP_F_SUBIF_LOGICAL))
		dp_register_switchdev_ops(dev, f_reset_ops);
	dp_register_ptp_ioctl(dev, f_reset_ops);
	if (subif_info->data_flag_ops & DP_SUBIF_TOE) {
		if (!subif_info->data_toe_disable)
			dp_inst_register_toe(dev, f_reset_ops);
	} else if (!g_toe_disable){
		dp_inst_register_toe(dev, f_reset_ops);
	}
	dp_inst_register_xfrm_ops(dev, f_reset_ops);

	return 0;
}

int set_dev_ops(struct dp_dev *dp_dev, struct net_device *dev, int flag)
{
	return change_dev_ops_all(dp_dev, dev, flag, 0);
}

int reset_dev_ops(struct dp_dev *dp_dev, struct net_device *dev)
{
	return change_dev_ops_all(dp_dev, dev, 0, 1);
}

struct br_dev_proc_data {
	u32 hash_idx;
	struct br_info *entry;
};

void dump_bp_member(int inst, int bp, struct seq_file *s)
{
	int i;
	GSW_BRIDGE_portConfig_t *bpcfg;
	struct core_ops *gsw_ops;

	if ((inst < 0) || (bp < 0))
		return;

	bpcfg = dp_kzalloc(sizeof(*bpcfg), GFP_ATOMIC);
	if (!bpcfg)
		return;

	bpcfg->nBridgePortId = bp;
	bpcfg->eMask = GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP;

	gsw_ops = dp_port_prop[inst].ops[0];
	if (gsw_ops->gsw_brdgport_ops.BridgePort_ConfigGet(gsw_ops, bpcfg)) {
		pr_err("DPM: %s: Fail in BridgePort_ConfigGet for bp=%d\n",
		       __func__, bp);
		kfree(bpcfg);
		return;
	}

	for (i = 0; i < sizeof(bpcfg->nBridgePortMap) * 8; i++) { /* 8 bits */
		if (GET_BP_MAP(bpcfg->nBridgePortMap, i))
			dp_sprintf(s, " %d", i);
	}
	kfree(bpcfg);
}

int proc_br_dump(struct seq_file *s, int pos)
{
	struct bridge_member_port *mem_dev;
	struct dp_proc_file_entry *p;
	struct br_dev_proc_data *data;
	bool flag = 0;

	if (!capable(CAP_SYS_PACCT))
		return -1;

	p = (struct dp_proc_file_entry *) s->private;
	if (!p || !p->data) {
		if (!p)
			pr_err("DPM: p NULL\n");
		else
			pr_err("DPM: p->data NULL\n");
		return -1;
	}
	data = p->data;

	while (!data->entry) {
		data->hash_idx++;
		if (data->hash_idx == BR_ID_ENTRY_HASH_TABLE_SIZE) {
			return -1;
		}
		pos = 0;
		data->entry =
			hlist_entry_safe((&g_bridge_id_entry_hash_table[data->hash_idx])->first,
					 struct br_info, br_hlist);
	}
	/*            hash     pos     br_dev   fid     last_f  inst   ep      vlan  cnt    */
	dp_sprintf(s, "%s=%-4u %s=%-3d %s=%-15s %s=%-3d %s=%-3d %s=%1d %s=%-2d %s=%d %s=%d\n",
		   "Hash", data->hash_idx,
		   "pos", pos,
		   "br_dev", data->entry->dev->name,
		   "fid", data->entry->fid,
		   "last_fid", data->entry->last_fid,
		   "inst", data->entry->inst,
		   "ep", data->entry->dp_port,
		   "br_vlan_en", data->entry->br_vlan_en,
		   "num_dev", data->entry->num_dev);
	seq_printf(s, "    f_mac_addr=%d: %02x:%02x:%02x:%02x:%02x:%02x\n",
		   data->entry->f_mac_add,
		   data->entry->br_mac[0], data->entry->br_mac[1],
		   data->entry->br_mac[2], data->entry->br_mac[3],
		   data->entry->br_mac[4], data->entry->br_mac[5]);

	list_for_each_entry(mem_dev, &data->entry->bp_list, list) {
		if (!flag) {
			flag = true;
			/*               dev   ep   bp   iso  hair priv reg */
			dp_sprintf(s, "  %-15s %-8s %-3s %-8s %-8s %-8s %-8s %s\n",
				   "dev",
				   "dp_port",
				   "bp",
				   "isolate",
				   "hairpin",
				   "priv_f",
				   "reg_flag",
				   "member_bps");
		}
		dp_sprintf(s, "  %-15s %-8d %-3d %-8d %-8d %-8x %-8x",
			   mem_dev->dev->name,
			   mem_dev->dp_port,
			   mem_dev->bportid,
			   mem_dev->isolate,
			   mem_dev->hairpin,
			   mem_dev->dev_priv_flag,
			   mem_dev->dev_reg_flag);
		if (mem_dev->dp_dev) /* dp_dev v1 not support mem_dev->dp_dev */
			dump_bp_member(mem_dev->dp_dev->inst, mem_dev->bportid, s);
		dp_sputs(s, "\n");
	}
	data->entry = hlist_entry_safe((data->entry)->br_hlist.next,
				       struct br_info, br_hlist);
	pos++;
	return pos;
}

int proc_br_start(void *param)
{
	struct dp_proc_file_entry *p;
	struct br_dev_proc_data *data = dp_kzalloc(sizeof(*data), GFP_ATOMIC);

	if (!data)
		return 0;
	p = param;
	p->data = data;
	data->hash_idx = 0;
	data->entry = hlist_entry_safe(
		(&g_bridge_id_entry_hash_table[data->hash_idx])->first,
		struct br_info, br_hlist);
	return 0;
}

int proc_bp_pmapper_dump(struct seq_file *s, int pos)
{
	if (!capable(CAP_SYS_PACCT))
		return -1;

	if (pos == 0)
		dp_sprintf(s, "alloc_bp    pmapper_flag pmapper_ref_cnt\n");
	if (dp_bp_tbl[0][pos].f_alloc)
		dp_sprintf(s, "%-4d        %-4d          %-4d\n", pos,
			   dp_bp_tbl[0][pos].flag,
			   dp_bp_tbl[0][pos].ref_cnt);
		
	pos ++;
	if (pos >= DP_MAX_BP_NUM) {
		pos = -1;
		dp_sprintf(s, "n_dp_bp=%d\n", n_dp_bp);
	}
	return pos;
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP
