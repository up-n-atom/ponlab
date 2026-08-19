/******************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/rtnetlink.h>
#include <linux/version.h>
#include <linux/list.h>

#include <pon/pon_mcc.h>
#include <pon/pon_mcc_ikm.h>
#include "drv_pon_mcc_conv.h"

#include <net/switch_api/lantiq_gsw.h>
#include <net/switch_api/gsw_flow_ops.h>
#include <net/switch_api/gsw_dev.h>
#include <net/datapath_api.h>
#include <net/netlink.h>
#include <net/genetlink.h>

#if IS_ENABLED(CONFIG_DP_MCAST_BRIDGE_FILTER)
#include <net/mcast_bridge_filter.h>
#endif

#define PRX300_DEV_ID 0
#define BR_PORT_FWD_MAP_TO_CPU	0x1
#define BR_PORT_FWD_MAP_EMPTY	0x0

#ifndef NLA_POLICY_EXACT_LEN
#define NLA_POLICY_EXACT_LEN(_len) { .type = NLA_BINARY }
#endif

#ifdef EXTRA_VERSION
#define pon_mcc_drv_extra_ver_str "." EXTRA_VERSION
#else
#define pon_mcc_drv_extra_ver_str ""
#endif

/** what string support, version string */
const char pon_mcc_drv_whatversion[] = "@(#)MaxLinear PON Multicast driver, version "
				       __stringify(PACKAGE_VERSION)
				       pon_mcc_drv_extra_ver_str;

static LIST_HEAD(mcc_key_storage);

struct pon_mcc_key_storage_node {
	struct mcc_drv_l3tbl_key key;
	struct list_head list;
};

/* Bridge configuration data */

static const struct nla_policy pon_mcc_genl_policy[PON_MCC_A_MAX + 1] = {
	[PON_MCC_A_IFINDEX] = { .type = NLA_U32 },
	[PON_MCC_A_PROTO] = { .type = NLA_U8 },
	[PON_MCC_A_GRP_ADDR] = NLA_POLICY_EXACT_LEN(sizeof(union mcc_l3addr)),
	[PON_MCC_A_SRC_ADDR] = NLA_POLICY_EXACT_LEN(sizeof(union mcc_l3addr)),
	[PON_MCC_A_FLT_MODE] = { .type = NLA_U8 },
	[PON_MCC_A_VALID] = { .type = NLA_U8 },
	[PON_MCC_A_INITIAL] = { .type = NLA_U8 },
	[PON_MCC_A_IGMP_VERSION] = { .type = NLA_U8 },
	[PON_MCC_A_MC_GEM_IFINDEX] = { .type = NLA_U32 },
	[PON_MCC_A_BRIDGE_PORT_ID] = { .type = NLA_U8 },
};

static int nl_multicast_enable(struct sk_buff *skb, struct genl_info *info);
static int nl_multicast_disable(struct sk_buff *skb, struct genl_info *info);
static int nl_tbl_entry_add(struct sk_buff *skb, struct genl_info *info);
static int nl_tbl_entry_delete(struct sk_buff *skb, struct genl_info *info);
static int nl_tbl_entry_read(struct sk_buff *skb, struct netlink_callback *cb);
static int nl_cpu_fwd_disable(struct sk_buff *skb, struct genl_info *info);
static int nl_cpu_fwd_enable(struct sk_buff *skb, struct genl_info *info);

static int is_netlink_capable(const struct genl_ops *ops,
			      struct sk_buff *skb,
			      struct genl_info *info)
{
	return netlink_capable(skb, CAP_NET_ADMIN) ? 0 : -EPERM;
}

/* List of the supported Netlink messages. */
static struct genl_ops pon_mcc_genl_ops[] = {
	{
		.cmd = PON_MCC_C_TABLE_ENTRY_ADD,
		.policy = pon_mcc_genl_policy,
		.doit = nl_tbl_entry_add,
	},
	{
		.cmd = PON_MCC_C_TABLE_ENTRY_DELETE,
		.policy = pon_mcc_genl_policy,
		.doit = nl_tbl_entry_delete,
	},
	{
		.cmd = PON_MCC_C_TABLE_ENTRY_READ,
		.policy = pon_mcc_genl_policy,
		.dumpit = nl_tbl_entry_read,
	},
	{
		.cmd = PON_MCC_C_MULTICAST_ENABLE,
		.policy = pon_mcc_genl_policy,
		.doit = nl_multicast_enable,
	},
	{
		.cmd = PON_MCC_C_MULTICAST_DISABLE,
		.policy = pon_mcc_genl_policy,
		.doit = nl_multicast_disable,
	},
	{
		.cmd = PON_MCC_C_CPU_FWD_ON,
		.policy = pon_mcc_genl_policy,
		.doit = nl_cpu_fwd_enable,
	},
	{
		.cmd = PON_MCC_C_CPU_FWD_OFF,
		.policy = pon_mcc_genl_policy,
		.doit = nl_cpu_fwd_disable,
	},
};

static struct genl_family pon_mcc_genl_family = {
#if (KERNEL_VERSION(4, 10, 0) > LINUX_VERSION_CODE)
	.id = GENL_ID_GENERATE,
#endif
	.ops = pon_mcc_genl_ops,
	.n_ops = ARRAY_SIZE(pon_mcc_genl_ops),
	.hdrsize = 0,
	.name = PON_MCC_FAMILY,
	.version = 1,
	.maxattr = PON_MCC_A_MAX,
	.parallel_ops = 1,
	.pre_doit = is_netlink_capable,
};

/** Function used to get datapath bridge port ID based on Multicast GEM port
 *  ifindex.
 */
static int bridge_port_id_get(u32 mc_gem_ifindex, u8 *br_port_id,
			      struct net_device **br_port_dev)
{
	dp_subif_t subif = {0};
	u32 flags = 0;
	int err;

	/* get net device (bridge port) name from MC GEM ifindex */
	*br_port_dev = dev_get_by_index(&init_net, mc_gem_ifindex);
	if (!(*br_port_dev)) {
		pr_err("%s: can't get net_device using ifindex=%u\n",
		       __func__, mc_gem_ifindex);
		return -ENODEV;
	}

	pr_debug("DEBUG: %s: get bridge port device, name = %s\n",
		 __func__, (*br_port_dev)->name);

	/* get subif information from net device */
	err = dp_get_netif_subifid(*br_port_dev, NULL, NULL, NULL, &subif,
				   flags);
	if (err != DP_SUCCESS) {
		pr_err("%s: function returned %d net_device %s\n",
		       __func__, err, (*br_port_dev)->name);
		dev_put(*br_port_dev);
		return -EINVAL;
	}
	*br_port_id = subif.bport;

	pr_debug("DEBUG: %s: get bridge port = %u\n",
		 __func__, *br_port_id);

	return 0;
}

/** Read entry from the forwarding table */
int mcc_drv_l3_tbl_get(struct mcc_drv_l3tbl_key *key, bool *busy, bool initial,
		       bool *last, int *flags, void *portmap)
{
	GSW_return_t ret;
	GSW_multicastTableRead_t entry_read;
	struct core_ops *ops;

	memset(&entry_read, 0, sizeof(entry_read));

	l3tbl_key_to_gsw_entry_read(key, initial, &entry_read);

	ops = gsw_get_swcore_ops(PRX300_DEV_ID);
	if (!ops) {
		pr_err("%s: Open SWAPI device FAILED!\n", __func__);
		return -EIO;
	}

	ret = ops->gsw_multicast_ops.Multicast_TableEntryRead(ops, &entry_read);
	if (ret != GSW_statusOk) {
		pr_err("%s: Multicast_TableEntryRead returned %d\n",
		       __func__, ret);
		return -EIO;
	}

	gsw_entry_read_to_l3tbl_key(&entry_read, key, last, portmap);

	return 0;
}
EXPORT_SYMBOL(mcc_drv_l3_tbl_get);

static int key_storage_add(const struct mcc_drv_l3tbl_key *key)
{
	struct pon_mcc_key_storage_node *node;

	node = kzalloc(sizeof(*node), GFP_KERNEL);
	if (!node)
		return -ENOMEM;

	memcpy(&node->key, key, sizeof(*key));

	list_add(&node->list, &mcc_key_storage);

	return 0;
}

static bool key_compare(const struct mcc_drv_l3tbl_key *a,
			const struct mcc_drv_l3tbl_key *b)
{
	if (a->ifindex != b->ifindex)
		return false;

	if (a->proto != b->proto)
		return false;

	if (a->proto == MCC_L3_PROTO_IPV4) {
		if (memcmp(&a->src.addr, &b->src.addr, sizeof(a->src.addr)))
			return false;
		if (memcmp(&a->grp.addr, &b->grp.addr, sizeof(a->grp.addr)))
			return false;
	} else {
		if (memcmp(&a->src.addr6, &b->src.addr6, sizeof(a->src.addr6)))
			return false;
		if (memcmp(&a->grp.addr6, &b->grp.addr6, sizeof(a->grp.addr6)))
			return false;
	}

	return true;
}

static bool key_storage_find(const struct mcc_drv_l3tbl_key *key)
{
	struct pon_mcc_key_storage_node *p;

	list_for_each_entry(p, &mcc_key_storage, list) {
		if (key_compare(key, &p->key))
			return true;
	}

	return false;
}

static void key_storage_remove(const struct mcc_drv_l3tbl_key *key)
{
	struct pon_mcc_key_storage_node *p;

	list_for_each_entry(p, &mcc_key_storage, list) {
		if (key_compare(key, &p->key)) {
			list_del(&p->list);
			kfree(p);
			return;
		}
	}
}

/** Add entry to the forwarding table */
int mcc_drv_l3_tbl_add(const struct mcc_drv_l3tbl_key *key, int flags)
{
	GSW_return_t ret;
	GSW_multicastTable_t entry;
	struct core_ops *ops;

	memset(&entry, 0, sizeof(entry));

	/*
	 * lock required by linux network functions
	 * used in l3tbl_key_to_gsw_entry
	 */
	rtnl_lock();

	/* Return success if entry already exists */
	if (key_storage_find(key)) {
		rtnl_unlock();
		return 0;
	}

	if (l3tbl_key_to_gsw_entry(key, &entry) != 0) {
		pr_err("%s: can't convert given key to multicast table entry\n",
		       __func__);
		rtnl_unlock();
		return -ENOENT;
	}

	/* Get pointer to general operations which contains SWITCH operations */
	ops = gsw_get_swcore_ops(PRX300_DEV_ID);
	if (!ops) {
		pr_err("%s: Open SWAPI device FAILED!\n", __func__);
		rtnl_unlock();
		return -EIO;
	}

	ret = key_storage_add(key);
	if (ret) {
		rtnl_unlock();
		return ret;
	}

	ret = ops->gsw_multicast_ops.Multicast_TableEntryAdd(ops, &entry);
	if (GSW_statusOk != ret) {
		key_storage_remove(key);
		pr_err("%s: Multicast_TableEntryAdd returned %d\n",
		       __func__, ret);
		rtnl_unlock();
		return -EIO;
	}
	pr_debug("%s: Table entry added\n", __func__);

	rtnl_unlock();

	return 0;
}
EXPORT_SYMBOL(mcc_drv_l3_tbl_add);

/** Remove entry from the forwarding table */
int mcc_drv_l3_tbl_del(const struct mcc_drv_l3tbl_key *key, int flags)
{
	GSW_return_t ret;
	GSW_multicastTable_t entry;
	struct core_ops *ops;

	memset(&entry, 0, sizeof(entry));

	/*
	 * lock required by linux network functions
	 * used in l3tbl_key_to_gsw_entry
	 */
	rtnl_lock();

	/* Return success if entry do not exists */
	if (!key_storage_find(key)) {
		rtnl_unlock();
		return 0;
	}

	if (l3tbl_key_to_gsw_entry(key, &entry) != 0) {
		pr_err("%s: can't convert given key to multicast table entry\n",
		       __func__);
		rtnl_unlock();
		return -ENOENT;
	}

	ops = gsw_get_swcore_ops(PRX300_DEV_ID);
	if (!ops) {
		pr_err("%s: Open SWAPI device FAILED!\n", __func__);
		rtnl_unlock();
		return -EIO;
	}

	ret = ops->gsw_multicast_ops.Multicast_TableEntryRemove(ops, &entry);
	if (ret != GSW_statusOk) {
		pr_debug("%s: Multicast_TableEntryRemove returned %d\n",
			 __func__, ret);
		rtnl_unlock();
		/* The case where the entry was not removed because it was not
		 * in the table should be treated differently because in fact
		 * it is not an execution error.
		 */
		return (ret == GSW_statusEntryNotFound) ? -ENOENT : -EIO;
	}
	pr_debug("%s: Table entry removed\n", __func__);

	key_storage_remove(key);
	rtnl_unlock();

	return 0;
}
EXPORT_SYMBOL(mcc_drv_l3_tbl_del);

/**
 * This function checks if we got matching bridge_port_id from user and hw.
 * If its matching converts port_id to correct ifindex and process with
 * removal otherwise returns with invalid value to skip it.
 */
static int mcc_drv_rm_group_ifindex_chk(struct mcc_drv_l3tbl_key *key,
					int ifindex,
					u8 brport_id,
					void *portmap)
{
	if (!test_bit(brport_id, portmap)) {
		pr_debug("DEBUG: %s, bridge_port id not matching, brport_id: %u\n",
			__func__, brport_id);
		return -1;
	}

	pr_debug("DEBUG: %s, bridge_port id matching, brport_id: %u, ifindex: %d\n",
		__func__, brport_id, ifindex);
	key->ifindex = ifindex;

	return 0;
}

/** Function which allows to remove all forwarding table entries related to
 *  a given network device.
 */
static int remove_all(int ifindex)
{
	struct mcc_drv_l3tbl_key key;
	int flags = 0;
	int ret;
	bool activity;
	bool initial;
	bool last;
	struct net_device *br_port_dev = NULL;
	u8 brport_id = 0;
	DECLARE_BITMAP(portmap, PORTMAP_SIZE) = { 0 };

	ret = bridge_port_id_get(ifindex, &brport_id, &br_port_dev);
	if (ret != 0) {
		pr_err("%s, bridge_port_id_get failed:%d\n", __func__, ret);
		return ret;
	}

	memset(&key, 0, sizeof(key));
	initial = true;
	do {
		ret = mcc_drv_l3_tbl_get(&key, &activity, initial,
					 &last, &flags, portmap);
		if (ret != 0) {
			pr_err("%s: mcc_drv_l3_tbl_get returned %d\n",
			       __func__, ret);
			break;
		}

		initial = false;

		ret = mcc_drv_rm_group_ifindex_chk(&key, ifindex, brport_id,
						   portmap);
		if (ret != 0)
			continue;

		if (key.valid) {
			ret = mcc_drv_l3_tbl_del(&key, flags);
			if (ret != 0)
				return ret;
			memset(&key, 0, sizeof(key));
		}
	} while (!last);

	return 0;
}

/** Function which allows to remove all forwarding table entries related to
 *  a given group address.
 */
static int remove_group(const union mcc_l3addr *grp_addr, int ifindex)
{
	struct mcc_drv_l3tbl_key key;
	int flags = 0;
	int ret;
	bool activity;
	bool initial;
	bool last;
	struct net_device *br_port_dev = NULL;
	u8 brport_id = 0;
	DECLARE_BITMAP(portmap, PORTMAP_SIZE) = { 0 };

	ret = bridge_port_id_get(ifindex, &brport_id, &br_port_dev);
	if (ret != 0) {
		pr_err("%s, bridge_port_id_get failed:%d\n", __func__, ret);
		return ret;
	}

	memset(&key, 0, sizeof(key));
	initial = true;
	do {
		ret = mcc_drv_l3_tbl_get(&key, &activity, initial,
					 &last, &flags, portmap);
		if (ret != 0) {
			pr_err("%s: mcc_drv_l3_tbl_get returned %d\n",
			       __func__, ret);
			break;
		}

		initial = false;

		ret = mcc_drv_rm_group_ifindex_chk(&key, ifindex, brport_id,
						   portmap);
		if (ret != 0)
			continue;

		if (key.valid) {
			if (key.proto == MCC_L3_PROTO_IPV4) {
				if (key.grp.addr.s_addr ==
				    grp_addr->addr.s_addr) {
					ret = mcc_drv_l3_tbl_del(&key, flags);
					if (ret != 0)
						return ret;
				}
			} else {
				if (strcmp(key.grp.addr6.s6_addr,
					   grp_addr->addr6.s6_addr) == 0) {
					ret = mcc_drv_l3_tbl_del(&key, flags);
					if (ret != 0)
						return ret;
				}
			}
			memset(&key, 0, sizeof(key));
		}
	} while (!last);

	return 0;
}

static bool mcc_l3addr_zero(const union mcc_l3addr *addr,
			    enum mcc_l3proto proto)
{
	if (proto == MCC_L3_PROTO_IPV4)
		return *(uint32_t *)&addr->addr == 0;
	return *(uint32_t *)&addr->addr6 == 0;
}

/** This function is used to remove entries from the forwarding table.
 *  If the source address is given, only one entry that matches both the source
 *  and the group address is removed.
 *  If the source address is empty it means that it is an IGMPv2 or MLDv1
 *  packet and all entries that match the group address are removed.
 *  If the source address is empty and the group address is empty,
 *  then all entries for the given interface are removed.
 */
static int mcc_drv_tbl_del(const struct mcc_drv_l3tbl_key *key, int flags)
{
	if (!mcc_l3addr_zero(&key->src, key->proto))
		return mcc_drv_l3_tbl_del(key, flags);

	if (!mcc_l3addr_zero(&key->grp, key->proto))
		return remove_group(&key->grp, key->ifindex);

	return remove_all(key->ifindex);
}

/** Function used to set bridge port forwarding map based on bridge port ID */
int mcc_drv_br_port_fwd_map_set_br_port_id(u8 br_port_id,
					   u16 fwd_mask_to_clear,
					   u16 fwd_mask_to_set)
{
	GSW_return_t ret;
	GSW_BRIDGE_portConfig_t br_port_cfg;
	struct core_ops *ops;

	pr_debug("DEBUG: %s: begin\n", __func__);

	memset(&br_port_cfg, 0, sizeof(br_port_cfg));

	ops = gsw_get_swcore_ops(PRX300_DEV_ID);
	if (!ops) {
		pr_err("%s: Open SWAPI device FAILED!\n", __func__);
		return -EIO;
	}

	pr_debug("DEBUG: %s: br_port_cfg input br_port_id = %u\n",
		 __func__, br_port_id);

	br_port_cfg.nBridgePortId = br_port_id;
	br_port_cfg.eMask = GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP;

	ret = ops->gsw_brdgport_ops.BridgePort_ConfigGet(ops, &br_port_cfg);
	if (ret != GSW_statusOk) {
		pr_err("%s: BridgePort_ConfigGet returned %d\n",
		       __func__, ret);
		return -EIO;
	}

	br_port_cfg.nBridgePortMap[0] &= ~fwd_mask_to_clear;
	br_port_cfg.nBridgePortMap[0] |= fwd_mask_to_set;
	br_port_cfg.eMask = GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP;

	ret = ops->gsw_brdgport_ops.BridgePort_ConfigSet(ops, &br_port_cfg);
	if (ret != GSW_statusOk) {
		pr_err("%s: BridgePort_ConfigSet returned %d\n",
		       __func__, ret);
		return -EIO;
	}

	pr_debug("DEBUG: %s: end\n", __func__);

	return 0;
}
EXPORT_SYMBOL(mcc_drv_br_port_fwd_map_set_br_port_id);

/** Function used to set bridge port forwarding map based on bridge port ID */
static int mcc_drv_br_port_fwd_map_set_ifindex(u8 ifindex,
					       u16 fwd_mask_to_clear,
					       u16 fwd_mask_to_set)
{
	struct net_device *br_port_dev = NULL;
	u8 br_port_id = 0;
	int err;

	err = bridge_port_id_get(ifindex, &br_port_id, &br_port_dev);
	if (err != 0) {
		pr_err("%s: function returned %d\n", __func__, err);
		return err;
	}

	dev_put(br_port_dev);

	err = mcc_drv_br_port_fwd_map_set_br_port_id(br_port_id,
						     fwd_mask_to_clear,
						     fwd_mask_to_set);
	if (err) {
		pr_err("%s: function returned %d\n", __func__, err);
		return err;
	}

	return 0;
}

/* Netlink functions */

static int nl_multicast_enable(struct sk_buff *skb, struct genl_info *info)
{
#if IS_ENABLED(CONFIG_DP_MCAST_BRIDGE_FILTER)
	dp_mcast_filter_active(true);
#endif

	return 0;
}

static int nl_multicast_disable(struct sk_buff *skb, struct genl_info *info)
{
#if IS_ENABLED(CONFIG_DP_MCAST_BRIDGE_FILTER)
	dp_mcast_filter_active(false);
#endif

	return 0;
}

static int nl_tbl_entry_add_del_chk(struct nlattr **attrs,
				    struct mcc_drv_l3tbl_key *key,
				    bool add)
{
	union mcc_l3addr *grp_addr = NULL;
	union mcc_l3addr *src_addr = NULL;

	memset(key, 0, sizeof(*key));

	if (attrs[PON_MCC_A_IFINDEX])
		key->ifindex = nla_get_u32(attrs[PON_MCC_A_IFINDEX]);
	else
		return -EINVAL;

	if (attrs[PON_MCC_A_PROTO])
		key->proto = nla_get_u8(attrs[PON_MCC_A_PROTO]);
	else
		return -EINVAL;

	if (attrs[PON_MCC_A_FLT_MODE])
		key->flt_mode = nla_get_u8(attrs[PON_MCC_A_FLT_MODE]);
	else
		return -EINVAL;

	if (attrs[PON_MCC_A_GRP_ADDR])
		grp_addr = nla_data(attrs[PON_MCC_A_GRP_ADDR]);

	if (attrs[PON_MCC_A_SRC_ADDR])
		src_addr = nla_data(attrs[PON_MCC_A_SRC_ADDR]);

	/* To add an entry, the group address must be available */
	if (add && !grp_addr)
		return -EINVAL;

	/* Source address requires group address */
	if (src_addr && !grp_addr)
		return -EINVAL;

	if (src_addr)
		key->src = *src_addr;
	if (grp_addr)
		key->grp = *grp_addr;

	return 0;
}

static int nl_tbl_entry_add(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **attrs = info->attrs;
	struct mcc_drv_l3tbl_key key;
	int ret = 0;

	ret = nl_tbl_entry_add_del_chk(attrs, &key, true);
	if (ret) {
		pr_err("%s: cannot get attributes values\n", __func__);
		return ret;
	}

	ret = mcc_drv_l3_tbl_add(&key, 0);
	if (ret) {
		pr_err("%s: cannot add table entry\n", __func__);
		return ret;
	}

	return ret;
}

static int nl_tbl_entry_delete(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **attrs = info->attrs;
	struct mcc_drv_l3tbl_key key;
	int ret = 0;

	ret = nl_tbl_entry_add_del_chk(attrs, &key, false);
	if (ret) {
		pr_err("%s: cannot get attributes values\n", __func__);
		return ret;
	}

	ret = mcc_drv_tbl_del(&key, 0);
	if (ret) {
		pr_err("%s: cannot delete table entry\n", __func__);
		return ret;
	}

	return ret;
}

static u8 get_initial(const struct nlmsghdr *n)
{
	struct nlattr *attrs[PON_MCC_A_MAX + 1];
	size_t hdrlen = GENL_HDRLEN + pon_mcc_genl_family.hdrsize;

#if (KERNEL_VERSION(4, 12, 0) > LINUX_VERSION_CODE)
	if (nlmsg_parse(n, hdrlen, attrs, PON_MCC_A_MAX, NULL) < 0)
#else
	if (nlmsg_parse(n, hdrlen, attrs, PON_MCC_A_MAX, NULL, NULL) < 0)
#endif
		return 1;

	if (attrs[PON_MCC_A_INITIAL])
		return nla_get_u8(attrs[PON_MCC_A_INITIAL]);

	pr_err("cannot read initial value, show first table entry\n");
	return 1;
}

static int nl_tbl_entry_read(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct mcc_drv_l3tbl_key key = {0,};
	void *msg_head = NULL;
	bool activity = 0;
	bool last = 0;
	u8 initial = get_initial(cb->nlh);
	int grp_addr_len = sizeof(union mcc_l3addr);
	int src_addr_len = sizeof(union mcc_l3addr);
	int flags = 0;
	int ret = 0;
	u32 portid, seq;
	DECLARE_BITMAP(portmap, PORTMAP_SIZE) = { 0 };

	portid = NETLINK_CB(cb->skb).portid;
	seq = cb->nlh->nlmsg_seq;

	msg_head = genlmsg_put(skb, portid, seq,
			       &pon_mcc_genl_family, 0,
			       PON_MCC_C_TABLE_ENTRY_READ);
	if (!msg_head) {
		pr_err("cannot create netlink msg\n");
		return -ENOMEM;
	}

	ret = mcc_drv_l3_tbl_get(&key, &activity, initial, &last, &flags,
				 portmap);
	if (ret) {
		pr_err("%s: cannot read table entry\n", __func__);
		genlmsg_cancel(skb, msg_head);
		return ret;
	}

	ret = nla_put_u8(skb, PON_MCC_A_VALID, key.valid);
	if (ret) {
		pr_err("cannot add data to netlink msg: %i\n", ret);
		genlmsg_cancel(skb, msg_head);
		return ret;
	}

	if (key.valid) {
		ret = nla_put_u32(skb, PON_MCC_A_IFINDEX, key.ifindex);
		if (ret) {
			pr_err("cannot add data to netlink msg: %i\n", ret);
			genlmsg_cancel(skb, msg_head);
			return ret;
		}

		ret = nla_put_u8(skb, PON_MCC_A_PROTO, key.proto);
		if (ret) {
			pr_err("cannot add data to netlink msg: %i\n", ret);
			genlmsg_cancel(skb, msg_head);
			return ret;
		}

		ret = nla_put(skb, PON_MCC_A_GRP_ADDR, grp_addr_len,
			      &key.grp);
		if (ret) {
			pr_err("cannot add data to netlink msg: %i\n",
			       ret);
			genlmsg_cancel(skb, msg_head);
			return ret;
		}

		ret = nla_put(skb, PON_MCC_A_SRC_ADDR, src_addr_len,
			      &key.src);
		if (ret) {
			pr_err("cannot add data to netlink msg: %i\n",
			       ret);
			genlmsg_cancel(skb, msg_head);
			return ret;
		}

		ret = nla_put_u8(skb, PON_MCC_A_FLT_MODE, key.flt_mode);
		if (ret) {
			pr_err("cannot add data to netlink msg: %i\n", ret);
			genlmsg_cancel(skb, msg_head);
			return ret;
		}
	}
	genlmsg_end(skb, msg_head);

	return skb->len;
}

static int nl_cpu_fwd_disable(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **attrs = info->attrs;
	u8 bridge_port_id = 0;
	int ret = 0;

	if (attrs[PON_MCC_A_BRIDGE_PORT_ID])
		bridge_port_id = nla_get_u8(attrs[PON_MCC_A_BRIDGE_PORT_ID]);

	ret = mcc_drv_br_port_fwd_map_set_ifindex(bridge_port_id,
						  BR_PORT_FWD_MAP_EMPTY,
						  BR_PORT_FWD_MAP_TO_CPU);
	if (ret) {
		pr_err("%s: cannot set bridge port fwd map: %i\n",
		       __func__, ret);
		return ret;
	}

	return ret;
}

static int nl_cpu_fwd_enable(struct sk_buff *skb, struct genl_info *info)
{
	struct nlattr **attrs = info->attrs;
	u8 bridge_port_id = 0;
	int ret = 0;

	if (attrs[PON_MCC_A_BRIDGE_PORT_ID])
		bridge_port_id = nla_get_u8(attrs[PON_MCC_A_BRIDGE_PORT_ID]);

	ret = mcc_drv_br_port_fwd_map_set_ifindex(bridge_port_id,
						  BR_PORT_FWD_MAP_TO_CPU,
						  BR_PORT_FWD_MAP_EMPTY);
	if (ret) {
		pr_err("%s: cannot set bridge port fwd map: %i\n",
		       __func__, ret);
		return ret;
	}

	return ret;
}

/* End of Netlink functions */

static int __init drv_pon_mcc_init(void)
{
	int ret;

	ret = genl_register_family(&pon_mcc_genl_family);
	if (ret)
		pr_err("can't register generic netlink ops");

	return ret;
}

static void __exit drv_pon_mcc_exit(void)
{
	genl_unregister_family(&pon_mcc_genl_family);
}

module_init(drv_pon_mcc_init);
module_exit(drv_pon_mcc_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("PON Multicast Driver");
MODULE_AUTHOR("MaxLinear Inc");
