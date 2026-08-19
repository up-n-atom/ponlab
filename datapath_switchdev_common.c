// SPDX-License-Identifier: GPL-2.0
/*****************************************************************************
 * Copyright (c) 2022 - 2023 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/

#include <linux/version.h>
#include <linux/etherdevice.h>
#include <linux/if_bridge.h>
#include <linux/netdevice.h>
#include <linux/workqueue.h>
#include <linux/if_link.h>
#include <linux/if_vlan.h>
#include <net/switchdev.h>
#include <net/datapath_api.h>
#include <net/datapath_proc_api.h>
#include "datapath.h"
#include "datapath_switchdev.h"
#include "datapath_instance.h"
#include "datapath_br_vlan.h"
#include "datapath_trace.h"

/* disable optimization in debug mode: push */
DP_NO_OPTIMIZE_PUSH

#define CPU_BP 0 /* CPU default bridge port ID */
#define CPU_FID 0 /* CPU default bridge ID */
#define SET_BP_MAP(x, ix) (x[(ix) / 16] |= 1 << ((ix) % 16))
#define GET_BP_MAP(x, ix) ((x[(ix) / 16] >> ((ix) % 16)) & 1)
#define is_isolate(dp_dev, dp_dev_member) (dp_dev->br_member_port->isolate && \
	dp_dev_member->br_member_port->isolate)
#define is_dev_up(dp_dev) (!!(dp_dev->dev->flags & IFF_UP))
#define is_dev_dpm_register(dp_dev)  (!!dp_dev->count)
#define is_stp_disabling(dp_dev) (dp_dev->f_stp_disabling)
#define is_no_queue(dp_dev) (!!(dp_dev->dev->priv_flags & IFF_NO_QUEUE))
#define set_this_bp_bit(bpcfg, bp) SET_BP_MAP(bpcfg->nBridgePortMap, bp)

DP_SWDEV_DEFINE_LOCK(dp_swdev_lock);

struct swdev_event {
	enum switchdev_notifier_type type;
	char *name;
};

static const struct swdev_event swdev_event_list[] = {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0))
	{SWITCHDEV_FDB_ADD_TO_BRIDGE, "fdb_add_to_br"},
	{SWITCHDEV_FDB_DEL_TO_BRIDGE, "fdb_del_to_br"},
	{SWITCHDEV_FDB_ADD_TO_DEVICE, "fdb_add_to_dev"},
	{SWITCHDEV_FDB_DEL_TO_DEVICE, "fdb_del_to_dev"},
	{SWITCHDEV_FDB_OFFLOADED, "fdb_offload"},
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 15))
	{SWITCHDEV_FDB_FLUSH_TO_BRIDGE, "fdb_flush_to_br"},
	{SWITCHDEV_PORT_OBJ_ADD, "port_obj_add"},
	{SWITCHDEV_PORT_OBJ_DEL, "port_obj_del"},
	{SWITCHDEV_PORT_ATTR_SET, "port_attr_set"},
	{SWITCHDEV_VXLAN_FDB_ADD_TO_BRIDGE, "vlan_fdb_add_to_br"},
	{SWITCHDEV_VXLAN_FDB_DEL_TO_BRIDGE, "vlan_fdb_del_to_br"},
	{SWITCHDEV_VXLAN_FDB_ADD_TO_DEVICE, "vlan_fdb_add_to_dev"},
	{SWITCHDEV_VXLAN_FDB_DEL_TO_DEVICE, "vlan_fdb_del_to_dev"},
	{SWITCHDEV_VXLAN_FDB_OFFLOADED, "vlan_fdb_offload"}
#endif
};

struct swdev_obj_add {
	enum switchdev_obj_id id;
	char *name;
};

static const struct swdev_obj_add swdev_obj_add_name[] = {
#if IS_ENABLED(CONFIG_BRIDGE_MRP)
	{SWITCHDEV_OBJ_ID_MRP, "obj_id_undef"},
	{SWITCHDEV_OBJ_ID_RING_TEST_MRP, "obj_id_ring_test"},
	{SWITCHDEV_OBJ_ID_RING_ROLE_MRP, "obj_id_ring_role"},
	{SWITCHDEV_OBJ_ID_RING_STATE_MRP, "obj_id_ring_state"},
	{SWITCHDEV_OBJ_ID_IN_TEST_MRP, "obj_id_id_test"},
	{SWITCHDEV_OBJ_ID_IN_ROLE_MRP, "obj_id_in_role"},
	{SWITCHDEV_OBJ_ID_IN_STATE_MRP, "obj_id_in_state"},
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0))
	{SWITCHDEV_OBJ_ID_UNDEFINED, "obj_id_undef"},
	{SWITCHDEV_OBJ_ID_PORT_VLAN, "obj_id_port_vlan"},
	{SWITCHDEV_OBJ_ID_PORT_MDB, "obj_id_port_mdb"},
	{SWITCHDEV_OBJ_ID_HOST_MDB,"obj_id_host_mdb"},
#endif
};

struct swdev_attr {
	enum switchdev_attr_id id;
	char *name;
};

static const struct swdev_attr swdev_attr_name[] = {
	{SWITCHDEV_ATTR_ID_UNDEFINED, "attr_undef"},
	{SWITCHDEV_ATTR_ID_PORT_STP_STATE, "stp_state"},
	{SWITCHDEV_ATTR_ID_PORT_BRIDGE_FLAGS, "br_flags"},
	{SWITCHDEV_ATTR_ID_BRIDGE_AGEING_TIME, "aging"},
	{SWITCHDEV_ATTR_ID_PORT_LEARNING_LIMIT, "learning_limit"},
	{SWITCHDEV_ATTR_ID_BRIDGE_VLAN_FILTERING, "vlan_filter"},
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 0))
	{SWITCHDEV_ATTR_ID_PORT_PRE_BRIDGE_FLAGS, "pre_br_flags"},
	{SWITCHDEV_ATTR_ID_PORT_MROUTER, "mroute"},
	{SWITCHDEV_ATTR_ID_BRIDGE_VLAN_PROTOCOL, "vlan_proto"},
	{SWITCHDEV_ATTR_ID_BRIDGE_MC_DISABLED, "mc_disable"},
	{SWITCHDEV_ATTR_ID_BRIDGE_MROUTER, "br_mroute"},
	{SWITCHDEV_ATTR_ID_MRP_PORT_ROLE, "mrp_port_role"},
	{SWITCHDEV_ATTR_ID_BRIDGE_MCAST_FLOOD, "br_mcast_flood"},
#endif
};

struct stp_stat {
	int stat;
	char *str;
} stp_stat_str[] = {
	{BR_STATE_DISABLED, "Disable"},
	{BR_STATE_LISTENING, "Listen"},
	{BR_STATE_LEARNING, "Learn"},
	{BR_STATE_FORWARDING, "Forward"},
	{BR_STATE_BLOCKING, "Block"}
};

struct dp_swdev_br_flag {
	int flag;
	char *str;
} bp_flag[] = {
	{BR_HAIRPIN_MODE, "Hairpin"},
	{BR_BPDU_GUARD, "BpduGuard"},
	{BR_ROOT_BLOCK, "RootBlock"},
	{BR_MULTICAST_FAST_LEAVE, "McFastLeave"},
	{BR_ADMIN_COST, "AdminCost"},
	{BR_LEARNING, "Learn"},
	{BR_FLOOD, "Flood"},
	{BR_AUTO_MASK, "AutoMask"},
	{BR_PROMISC, "Promisc"},
	{BR_PROXYARP, "ProxyArp"},
	{BR_LEARNING_SYNC, "LearnSync"},
	{BR_PROXYARP_WIFI, "ProxyArpWifi"},
	{BR_MCAST_FLOOD, "McFlood"},
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 19, 0))
	{BR_MULTICAST_TO_UNICAST, "Mc2Uc"},
	{BR_VLAN_TUNNEL, "VlanTunnel"},
	{BR_BCAST_FLOOD, "BcFlood"},
	{BR_NEIGH_SUPPRESS, "NeighSuppress"},
#endif
	{BR_ISOLATED, "Isolate"},
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 15)
	{BR_MRP_AWARE, "MrpAware"},
	{BR_MRP_LOST_CONT, "MrpLOST"},
	{BR_MRP_LOST_IN_CONT, "MrpLostIn"}
#endif
};

static void dp_swdev_insert_bridge_id_entry(struct br_info *);
struct hlist_head g_bridge_id_entry_hash_table[BR_ID_ENTRY_HASH_TABLE_SIZE];

static struct kmem_cache *cache_br_info_list;  /* struct br_info */
static struct kmem_cache *cache_br_port_list;  /* struct bridge_member_port */

void dump_bp_flag(struct switchdev_brport_flags flags)
{
	int i;
	int size = 300;
	int idx;
	char *buf = dp_kzalloc(size, GFP_ATOMIC);

	if (!buf)
		return;

	idx = scnprintf(buf, size, "%s", "  br_flags=");
	for (i = 0; i < ARRAY_SIZE(bp_flag); i++) {
		if (bp_flag[i].flag & flags.mask)
			idx += scnprintf(buf + idx, size - idx - 1,
					 "%s ", bp_flag[i].str);
	}
	idx += scnprintf(buf + idx, size - idx - 1, "\n");
	DP_DUMP("%s", buf);

	kfree(buf);
}

static inline u32 dp_swdev_cal_hash(const void *ptr)
{
	return hash_long((unsigned long)ptr, BRIDGE_ID_ENTRY_HASH_LENGTH);
}

struct hlist_head *get_dp_g_bridge_id_entry_hash_table_info(int idx)
{
	return &g_bridge_id_entry_hash_table[idx];
}

static struct bridge_member_port *get_bp_from_list(
	struct br_info *br_item,
	struct net_device *dev)
{
	struct bridge_member_port *bmp;

	list_for_each_entry(bmp, &br_item->bp_list, list) {
		if (bmp->dev != dev)
			continue;
		return bmp;
	}

	return NULL;
}

int dp_get_fid_by_dev(struct net_device *dev, int *inst)
{
	struct br_info *br_info;
	int fid = -1;

	if (unlikely(!dp_init_ok)) {
		pr_err("DPM: %s failed: datapath not initialized yet\n", __func__);
		return DP_FAILURE;
	}

	if (!dev || !inst) {
		pr_err("DPM: %s failed: dev or inst is NULL\n", __func__);
		return DP_FAILURE;
	}

	br_info = dp_swdev_bridge_entry_lookup(dev);
	if (br_info) {
		*inst = br_info->inst;
		fid = br_info->fid;
	}

	return fid;
}
EXPORT_SYMBOL(dp_get_fid_by_dev);

int dp_swdev_chk_bport_in_br(struct net_device *dev, int bport, int inst)
{
	struct net_device *br_dev;
	struct br_info *br_info;
	struct bridge_member_port *bmp;

	br_dev = netdev_master_upper_dev_get(dev);
	if (!br_dev)
		return DP_FAILURE;

	br_info = dp_swdev_bridge_entry_lookup(br_dev);
	if (!br_info)
		goto ERR;
	bmp = get_bp_from_list(br_info, dev);
	if (bmp) {
		if (bmp->bportid != bport) {
			pr_err("DPM: %s: why %s bportid(%d) != %d\n", __func__,
			       dev->name, bmp->bportid, bport);
			return DP_FAILURE;

		}
		DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s: bport(%s) in bridge\n",
			 __func__, dev->name ? dev->name : "NULL");
		return DP_SUCCESS;
	}

ERR:
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s: bport(%s) not in bridge\n",
		 __func__, dev->name ? dev->name : "NULL");

	return DP_FAILURE;
}

struct br_info *dp_swdev_bridge_entry_lookup(struct net_device *dev)
{

	struct br_info *br_item;
	struct hlist_head *head;
	u32 idx;

	idx = dp_swdev_cal_hash(dev);

	head = get_dp_g_bridge_id_entry_hash_table_info(idx);
	hlist_for_each_entry(br_item, head, br_hlist) {
		if (br_item->dev == dev){
			DP_DEBUG(DP_DBG_FLAG_SWDEV_DETAIL,
				 "%s: Hash entry found(%s)\n",
				 __func__, dev->name);
			return br_item;
		}
	}

	DP_DEBUG(DP_DBG_FLAG_SWDEV_DETAIL, "%s: Hash entry not found(%s)\n",
		 __func__, dev->name);

	return NULL;
}

struct br_info *dp_br_in_free_lookup(struct net_device *dev)
{

	struct br_info *br_item;
	struct hlist_head *head;
	u32 idx;

	idx = dp_swdev_cal_hash(dev);

	head = get_dp_g_bridge_id_entry_hash_table_info(idx);
	hlist_for_each_entry(br_item, head, br_hlist) {
		if (br_item->dev == dev){
			return br_item;
		}
	}
	DP_DEBUG(DP_DBG_FLAG_SWDEV_DETAIL, "%s: Hash entry not found(%s)\n",
		 __func__, dev->name);

	return NULL;
}

static void dp_swdev_insert_bridge_id_entry(struct br_info *br_item)
{
	struct br_info *tmp;
	struct hlist_head *head;
	u32 idx = 0;

	tmp = dp_swdev_bridge_entry_lookup(br_item->dev);
	if (tmp) { /* update */
		if (tmp->fid != br_item->fid) {
			DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s: fid not match: %d - %d\n", __func__,
				tmp->fid, br_item->fid);
			tmp->fid = br_item->fid;
		}
		if (tmp->inst != br_item->inst) {
			DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s: inst not match: %d - %d\n", __func__,
				tmp->inst, br_item->inst);
			tmp->inst = br_item->inst;
		}
		if (tmp->dp_port != br_item->dp_port) {
			DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s: dp_port not match: %d - %d\n", __func__,
				tmp->dp_port, br_item->dp_port);
			tmp->dp_port = br_item->dp_port;
		}
		tmp->flag = br_item->flag;
		kmem_cache_free(cache_br_info_list, br_item);
	} else { /* for back-compatible only */
		idx = dp_swdev_cal_hash(br_item->dev);
		head = get_dp_g_bridge_id_entry_hash_table_info(idx);
		hlist_add_head(&br_item->br_hlist, head);
	}

	DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s: Hash added idx:%d bridge(%s)\n",
		 __func__, idx, br_item->dev->name);
}

void free_br_item_bp_list(struct br_info *br_item)
{
	struct bridge_member_port *pos, *n;

	if (!br_item)
		return;
	list_for_each_entry_safe(pos, n, &br_item->bp_list, list) {
		list_del(&pos->list);
		kfree(pos);
		br_item->num_dev--;
	}
}

void dp_swdev_br_tbl_free(void)
{
	struct hlist_node *n;
	struct hlist_head *head;
	struct br_info *br_item;
	u32 idx;

	for (idx = 0; idx < ARRAY_SIZE(g_bridge_id_entry_hash_table); idx++) {
		head = &g_bridge_id_entry_hash_table[idx];
		hlist_for_each_entry_safe(br_item, n, head, br_hlist) {
			/* remove from hlist */
			hlist_del(&br_item->br_hlist);

			/* free bp_list */
			free_br_item_bp_list(br_item);

			if (br_item->num_dev)
				pr_err("DPM: %s: num_dev(%d) not zero yet %s:%s\n",
				       __func__, br_item->num_dev,
				       "and need free br_item->bp_list",
				       br_item->dev->name);
			/* free the memory */
			kmem_cache_free(cache_br_info_list, br_item);
		}
	}
	/* need further remove other list */
	kmem_cache_destroy(cache_br_port_list);
	kmem_cache_destroy(cache_br_info_list);
}

int dp_swdev_br_tbl_init(void)
{
	int i;

	for (i = 0; i < BR_ID_ENTRY_HASH_TABLE_SIZE; i++)
		INIT_HLIST_HEAD(&g_bridge_id_entry_hash_table[i]);

	cache_br_info_list = kmem_cache_create("dp_br_info_list",
					       sizeof(struct br_info),
					       0, SLAB_HWCACHE_ALIGN, NULL);
	if (!cache_br_info_list)
		return DP_FAILURE;

	cache_br_port_list =
		kmem_cache_create("dp_br_port_list",
				  sizeof(struct bridge_member_port),
				  0, SLAB_HWCACHE_ALIGN, NULL);
	if (!cache_br_port_list)
		goto ERR;

	return DP_SUCCESS;
ERR:
	if (cache_br_info_list) {
		kmem_cache_destroy(cache_br_info_list);
		cache_br_info_list = NULL;
	}
	return DP_FAILURE;
}

char *get_swdev_event_name(enum switchdev_notifier_type type)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(swdev_event_list); i++) {
		if (swdev_event_list[i].type != type)
			continue;
		return swdev_event_list[i].name;
	}
	return "unknown";
}

char *get_swdev_obj_add_name(enum switchdev_obj_id id)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(swdev_obj_add_name); i++) {
		if (swdev_obj_add_name[i].id != id)
			continue;
		return swdev_obj_add_name[i].name;
	}
	return "unknown";
}

char *get_stp_stat_str(int stat)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(stp_stat_str); i++) {
		if (stat != stp_stat_str[i].stat)
			continue;
		return stp_stat_str[i].str;
	}
	return "known";
}

char *get_bport_flags(unsigned long flags)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(bp_flag); i++) {
		if (flags != bp_flag[i].flag)
			continue;
		return bp_flag[i].str;
	}
	return "unknown";
}

char *get_swdev_attr_name(enum switchdev_attr_id id)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(swdev_attr_name); i++) {
		if (swdev_attr_name[i].id != id)
			continue;
		return swdev_attr_name[i].name;
	}
	return "unknown";
}

/* basic follow: ??
 * 1) BR_STATE_FORWARDING: similiar like original dp_swdev_stp_forward
      If VLAN dev: need dp_register_subif if not registered yet.
      Need map bridge to FID if not mapped yet
      Update this bridge port's member and its member's member
 * 2) BR_STATE_DISABLE: similiar like original dp_swdev_stp_disable
      If VLAN dev: de-register from dpm if already registered
      update BP's member and member's memger
      Remove FID if it is the last device in the bridge
 */
int dp_gswip_stp_set(struct dp_swdev_data *dev_data,
				   int port, int control,
				   int value)
{
	struct dp_dev *dp_dev;

	DP_DEBUG(DP_DBG_FLAG_SWDEV,
		 "dev=%s inst=%d BP=%d STP state=%s(%d)\n",
		 dev_data->dev->name,
		 dev_data->subif.inst,
		 port,
		 get_stp_stat_str(value), value);
	trace_dp_swdev_event(dev_data, value, dev_data->dev, NULL);
	dp_dev = dp_dev_lookup(dev_data->dev);
	if (!dp_dev) {
		pr_err("DPM: %s: why dp_dev NULL for %s\n", __func__,
		       dev_data->dev->name);
		return 0;
	}

	if (value == BR_STATE_FORWARDING)
		dp_swdev_stp_forward(dp_dev);
	else if (value == BR_STATE_DISABLED)
		dp_swdev_stp_disable(dp_dev);

	return 0;
}

int dp_gswip_stp_get(struct dp_swdev_data *dev_data, int port, int control)
{
#ifdef need_check_later
	const struct ale_control_info *info;
	int offset, shift;
	u32 tmp;

	if (control < 0 || control >= ARRAY_SIZE(ale_controls))
		return -EINVAL;

	info = &ale_controls[control];
	if (info->port_offset == 0 && info->port_shift == 0)
		port = 0; /* global, port is a dont care */

	if (port < 0 || port >= ale->params.ale_ports)
		return -EINVAL;

	offset = info->offset + (port * info->port_offset);
	shift  = info->shift  + (port * info->port_shift);

	tmp = readl_relaxed(ale->params.ale_regs + offset) >> shift;
	return tmp & BITMASK(info->bits);
#endif
	return 0;
}

int dp_gswip_add_ucast(struct dp_swdev_data *dev_data,
				const u8 *addr, int port,
				int flags, u16 vid)
{
#ifdef need_check_later
	u32 ale_entry[ALE_ENTRY_WORDS] = {0, 0, 0};
	int idx;

	dp_gswip_set_vlan_entry_type(ale_entry, flags, vid);

	dp_gswip_set_addr(ale_entry, addr);
	dp_gswip_set_ucast_type(ale_entry, ALE_UCAST_PERSISTANT);
	dp_gswip_set_secure(ale_entry, (flags & ALE_SECURE) ? 1 : 0);
	dp_gswip_set_blocked(ale_entry, (flags & ALE_BLOCKED) ? 1 : 0);
	dp_gswip_set_port_num(ale_entry, port, ale->port_num_bits);

	idx = dp_gswip_match_addr(ale, addr, (flags & ALE_VLAN) ? vid : 0);
	if (idx < 0)
		idx = dp_gswip_match_free(ale);
	if (idx < 0)
		idx = dp_gswip_find_ageable(ale);
	if (idx < 0)
		return -ENOMEM;

	dp_gswip_write(ale, idx, ale_entry);
#endif
	return 0;
}

int dp_gswip_del_ucast(struct dp_swdev_data *dev_data,
				const u8 *addr, int port,
				int flags, u16 vid)
{
#ifdef need_check_later
	u32 ale_entry[ALE_ENTRY_WORDS] = {0, 0, 0};
	int idx;

	idx = dp_gswip_match_addr(ale, addr, (flags & ALE_VLAN) ? vid : 0);
	if (idx < 0)
		return -ENOENT;

	dp_gswip_set_entry_type(ale_entry, ALE_TYPE_FREE);
	dp_gswip_write(ale, idx, ale_entry);
#endif
	return 0;
}

int dp_gswip_add_mcast(struct dp_swdev_data *dev_data,
				const u8 *addr, int port_mask,
				int flags, u16 vid, int mcast_state)
{
#ifdef need_check_later
	u32 ale_entry[ALE_ENTRY_WORDS] = {0, 0, 0};
	int idx, mask;

	idx = dp_gswip_match_addr(ale, addr, (flags & ALE_VLAN) ? vid : 0);
	if (idx >= 0)
		dp_gswip_read(ale, idx, ale_entry);

	dp_gswip_set_vlan_entry_type(ale_entry, flags, vid);

	dp_gswip_set_addr(ale_entry, addr);
	dp_gswip_set_super(ale_entry, (flags & ALE_SUPER) ? 1 : 0);
	dp_gswip_set_mcast_state(ale_entry, mcast_state);

	mask = dp_gswip_get_port_mask(ale_entry,
				      ale->port_mask_bits);
	port_mask |= mask;
	dp_gswip_set_port_mask(ale_entry, port_mask,
			       ale->port_mask_bits);

	if (idx < 0)
		idx = dp_gswip_match_free(ale);
	if (idx < 0)
		idx = dp_gswip_find_ageable(ale);
	if (idx < 0)
		return -ENOMEM;

	dp_gswip_write(ale, idx, ale_entry);
#endif
	return 0;
}

int dp_gswip_del_mcast(struct dp_swdev_data *dev_data,
				const u8 *addr, int port_mask,
				int flags, u16 vid)
{
#ifdef need_check_later
	u32 ale_entry[ALE_ENTRY_WORDS] = {0, 0, 0};
	int mcast_members = 0;
	int idx;

	idx = dp_gswip_match_addr(ale, addr, (flags & ALE_VLAN) ? vid : 0);
	if (idx < 0)
		return -ENOENT;

	dp_gswip_read(ale, idx, ale_entry);

	if (port_mask) {
		mcast_members = dp_gswip_get_port_mask(ale_entry,
						       ale->port_mask_bits);
		mcast_members &= ~port_mask;
	}

	if (mcast_members)
		dp_gswip_set_port_mask(ale_entry, mcast_members,
				       ale->port_mask_bits);
	else
		dp_gswip_set_entry_type(ale_entry, ALE_TYPE_FREE);

	dp_gswip_write(ale, idx, ale_entry);
#endif
	return 0;
}

int dp_gswip_port_attr_bridge_flags_set(struct net_device *dev,
					unsigned long flags)
{
	struct inst_info *i_info;
	dp_subif_t *subif;
	struct net_device *br_dev;
	struct br_info *br_info;
	int ret = -EOPNOTSUPP;

	br_dev = netdev_master_upper_dev_get(dev);
	if (!br_dev)
		return -1;

	subif = dp_kzalloc(sizeof(*subif), GFP_ATOMIC);
	if (!subif)
		return DP_FAILURE;
	if (dp_get_netif_subifid(dev, NULL, NULL, NULL, subif, 0)) {
		DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s: get subif failed for %s\n",
			 __func__, dev->name);
		kfree(subif);
		return -EINVAL;
	}

	i_info = &dp_port_prop[subif->inst].info;
	if (!i_info->swdev_bridge_port_flags_set) {
		kfree(subif);
		return ret;
	}

	br_info = dp_swdev_bridge_entry_lookup(br_dev);
	if (!br_info)
		goto EXIT;

	ret = i_info->swdev_bridge_port_flags_set(br_info, subif->inst,
			subif->bport, flags);
EXIT:
	kfree(subif);
	return ret;
}

void dp_gswip_set_unreg_mcast(struct dp_swdev_data *dev_data, int unreg_mcast_mask,
			      bool add)
{
}

int dp_gswip_vlan_add_modify(struct dp_swdev_data *dev_data, u16 vid, int port_mask,
			     int untag_mask, int reg_mask, int unreg_mask)
{
	return 0;
}

int get_swdev_port_id(struct dp_swdev_data *dev_data)
{
	return dev_data->subif.bport;
}

int dp_gswip_del_vlan(struct dp_swdev_data *dev_data, u16 vid, int port_mask)
{

	return 0;
}
int dp_del_br_if(struct net_device *dev, struct net_device *br_dev,
		int inst, int bport)
{
	return 0;
}

/* This API is caller when bridge device register to network stack via
 * network event, like NETDEV_REGISTER
 */
int dp_add_br(struct net_device *br_dev)
{
	struct br_info *br_info;

	DP_DEBUG(DP_DBG_FLAG_SWDEV, "to add brige %s\n", br_dev->name);
	br_info = dp_swdev_bridge_entry_lookup(br_dev);
	if (!br_info) {
		/* when bridge is created. we don't know its instance yet */
		br_info = kmem_cache_zalloc(cache_br_info_list, GFP_ATOMIC);
		if (!br_info)
			return -1;
		br_info->fid = -1;
		br_info->last_fid = -1;
		br_info->inst = -1;
		br_info->dev = br_dev;
		br_info->max_vlan_limit = DP_DEF_MAX_VLAN_LIMIT;
		br_info->max_brid_limit = DP_DEF_MAX_VLAN_LIMIT;
		/* Note, with current design via NETDEV_REGISTER, definately
		 * bridge device vlan aware not enabled yet at this stage
		 */
		if (dp_br_vlan_enabled(br_info->dev))
			br_info->br_vlan_en = true;

		INIT_LIST_HEAD(&br_info->bp_list);
		INIT_LIST_HEAD(&br_info->br_vlan_list);
		dp_swdev_insert_bridge_id_entry(br_info);
		dp_register_br_vlan_ops(br_info, 0);
	}

	return br_info->fid;
}

/* free bridge information when this bridge is de-registered from network via
 * network event, like NETDEV_UNREGISTER
 */
int dp_del_br(struct net_device *br_dev)
{
	struct br_info *br_info;

	br_info = dp_swdev_bridge_entry_lookup(br_dev);
	if (!br_info)
		return DP_SUCCESS;
	dp_register_br_vlan_ops(br_info, DP_OPS_RESET);
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "free bridge:%s\n", br_dev->name);

	hlist_del(&br_info->br_hlist);
	kmem_cache_free(cache_br_info_list, br_info);

	return DP_SUCCESS;
}

/* check if loop enabled between the configuring bport devices
 * dp_dev_member - Bridge port device to be configured to cur_bport's port map
 * dp_dev - Bport device which is getting configured
 */

bool is_loop(struct dp_dev *dp_dev, struct dp_dev *dp_dev_member)
{
	struct pmac_port_info *p_info;

	p_info = get_dp_port_info(dp_dev->inst, dp_dev->ep);

	if (p_info->loop_dis && dp_dev->ep == dp_dev_member->ep)
		return true;

	return false;
}

/* bp_member - Bridge port to be configured to cur_bport's port map
 * cur_bp - Bport which is getting configured
 * Note: domain validity check not require for CPU BP,
 * hairpin mode configuration
 */
bool is_in_domain(struct dp_dev *dp_dev,
			    struct dp_dev *dp_dev_member,
			    int *member_bp)
{
	struct dp_dev_subif *dev_sif, *dev_member_sif;

	dev_sif = list_first_entry(&dp_dev->subif_list,
				   struct dp_dev_subif, list);
	dev_member_sif = list_first_entry(&dp_dev_member->subif_list,
					  struct dp_dev_subif, list);

	/* check BP member's domain id present in current BP domain member */
	/* if it is vlan device, we should compare and get bp from logic_dev */
	if (is_vlan_dev(dp_dev->dev)) {
		/*TODO: Need to improve later */
		return false;

	} else {
		if (dev_sif->sif->domain_members &
		    (1 << dev_member_sif->sif->domain_id)) {
			*member_bp = dev_member_sif->sif->bp;
			return true;
		}
	}

	return false;
}

bool is_hairpin(struct dp_dev *dp_dev, int *member_bp)
{
	struct dp_dev_subif *dev_sif;

	dev_sif = list_first_entry(&dp_dev->subif_list,
				   struct dp_dev_subif, list);

	/* check BP member's domain id present in current BP domain member */
	/* if it is vlan device, we should compare and get bp from logic_dev */
	if (is_vlan_dev(dp_dev->dev)) {
		/*TODO: Need to improve later */
		return false;

	} else {
		if (dp_dev->br_member_port->hairpin) {
			*member_bp = dev_sif->sif->bp;
			return true;
		}
	}
	return false;
}

bool is_cpu_enable(struct dp_dev *dp_dev, int *member_bp)
{

	struct dp_dev_subif *dev_sif;

	dev_sif = list_first_entry(&dp_dev->subif_list,
				   struct dp_dev_subif, list);

	if (is_vlan_dev(dp_dev->dev)) {
		/*TODO: Need to improve later */
		return false;

	} else {
		if (dev_sif->sif->cpu_port_en) {
			*member_bp = CPU_BP;
			return true;
		}
	}
	return false;
}

int dp_gswip_vlan_mac_learning_en(GSW_BRIDGE_portConfig_t *bpcfg, bool status)
{
	bpcfg->eMask |= GSW_BRIDGE_PORT_CONFIG_MASK_VLAN_BASED_MAC_LEARNING;
	if (status) {
		bpcfg->bVlanDstMacVidEnable = 1;
		bpcfg->bVlanSrcMacVidEnable = 1;
		 /* Intermediate outer VLAN
		  * tag is used for MAC address learning
		  */
		bpcfg->bVlanTagSelection = 0;
		/* Kernel's default bridge port settings including BR_LEARNING
		 * are not set inside GSWIP at the point in time of bridge port
		 * creation. Subsequent commands to enable learning are blocked
		 * in newer kernel versions because the kernel does not
		 * recognize any difference between old and new settings.
		 * To overcome this issue, enable the learning here together
		 * with above flags as VLAN aware mode is only working
		 * if learning is enabled as well. No further check required
		 * as non PON applications are not using learning without VLAN.
		 */
		bpcfg->eMask |= GSW_BRIDGE_PORT_CONFIG_MASK_MC_SRC_MAC_LEARNING;
		bpcfg->bSrcMacLearningDisable = 0;
	} else {
		bpcfg->bVlanDstMacVidEnable = 0;
		bpcfg->bVlanSrcMacVidEnable = 0;
		bpcfg->eMask |= GSW_BRIDGE_PORT_CONFIG_MASK_MC_SRC_MAC_LEARNING;
		bpcfg->bSrcMacLearningDisable = 1;
	}
	return 0;
}

void dump_bp_cfg(struct net_device *dev, GSW_BRIDGE_portConfig_t *bpcfg)
{
	#define buf_len 1024
	int i, idx = 0;
	char *buf = dp_kmalloc(buf_len, GFP_ATOMIC);

	if (!buf)
		return;
	buf[0] = 0;
	for (i = 0; i < sizeof(bpcfg->nBridgePortMap) * 8; i++) { /* 8 bits */
		if (GET_BP_MAP(bpcfg->nBridgePortMap, i))
			idx += scnprintf(buf + idx, buf_len - 1, "%d ", i);
	}
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "dev=%s bp=%d fid=%d member_list=%s\n",
		 dev ? dev->name : " ", bpcfg->nBridgePortId,
		 bpcfg->eMask & GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_ID ?
		 bpcfg->nBridgeId: -1,
		 bpcfg->eMask & GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP ?
		 buf : " ");
	kfree(buf);
}

#define GSWIP_LEARN_LIMIT_PORT_MAX 254
int dp_set_one_bp_member(struct dp_dev *dp_dev, struct list_head *head)
{
	GSW_BRIDGE_portConfig_t *bpcfg = dp_kzalloc(sizeof(*bpcfg), GFP_ATOMIC);
	struct bridge_member_port *member_pos;
	struct dp_dev_subif *dev_sif;
	int member_bp;
	struct core_ops *gsw_ops;
	int ret = DP_FAILURE;

	if (!bpcfg)
		return DP_FAILURE;
	dev_sif = list_first_entry(&dp_dev->subif_list,
				   struct dp_dev_subif, list);
	if (!is_dev_dpm_register(dp_dev)) /* do nothing */
		goto EXIT;

#ifdef need_check_later
	if (is_vlan_dev(dp_dev->dev)) {
		/*TODO: Need to improve later */
		goto EXIT;
	} else {
		bpcfg->nBridgePortId = dev_sif->sif->bp;
		bpcfg->nBridgeId = dev_sif->sif->fid;
	}
#else
	if (!dp_dev->br_info) {
		pr_err("DPM: br_info NULL: %s\n", dp_dev->dev->name);
		goto EXIT;
	}
	bpcfg->nBridgePortId = dp_dev->bp;
	bpcfg->nBridgeId = dp_dev->br_info->fid;
#endif
	if (bpcfg->nBridgePortId <= 0) {
		pr_err("DPM: wrong nBridgePortId=%d for %s\n",
		       bpcfg->nBridgePortId, dp_dev->dev->name);
		goto EXIT;
	}
	if (bpcfg->nBridgeId <= 0) {
		pr_err("DPM: wrong nBridgeId=%d for %s\n",
		       bpcfg->nBridgePortId, dp_dev->dev->name);
		goto EXIT;
	}
	if (!dp_dev->swdev_en) {
		/* non swdev supported device no need to config member */
		goto EXIT;
	}

	bpcfg->eMask |= GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_ID |
			GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP;

	if(is_stp_disabling(dp_dev)) {
		/* overwritten fid/nBridgeId */

		bpcfg->nBridgeId = CPU_FID;
		/* Disable VLAN based Mac learning on bridge port when
		 * this interface is removed from bridge
		 */
		if (is_soc_lgm(dp_dev->inst)) {
			if ((dev_sif->sif->vlan_aware_en) &&
				(dp_br_vlan_enabled(dp_dev->br_info->dev))) {
				dev_sif->sif->vlan_aware_en = false;
				dp_gswip_vlan_mac_learning_en(bpcfg, false);
				DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
					 "vlan aware disable on bp(%d)\n",
					 dp_dev->bp);
			}
		}
		/* bypass cross member check */
		goto SKIP_CROSS_MEMEBER_CHK;
	}

	list_for_each_entry(member_pos, head, list) {
		if (dp_dev == member_pos->dp_dev)
			continue;
		if (!is_dev_up(member_pos->dp_dev))
			continue;
		if (!member_pos->dp_dev->swdev_en)
			continue;
		if (is_stp_disabling(member_pos->dp_dev))
			continue;
		if (!is_dev_dpm_register(member_pos->dp_dev))
			continue;
		if (!is_in_domain(dp_dev, member_pos->dp_dev, &member_bp))
			continue;
		if (is_isolate(dp_dev, member_pos->dp_dev))
			continue;
		if (is_no_queue(member_pos->dp_dev))
			continue;
		if (is_loop(dp_dev, member_pos->dp_dev))
			continue;

		//note: all necessary checking finished: add to member list
		set_this_bp_bit(bpcfg, member_bp);
	}
	/* Enable VLAN based Mac learning on bridge port */
	if (is_soc_lgm(dp_dev->inst)) {
		if ((!dev_sif->sif->vlan_aware_en) &&
			(dp_br_vlan_enabled(dp_dev->br_info->dev))) {
			dev_sif->sif->vlan_aware_en = true;
			dp_gswip_vlan_mac_learning_en(bpcfg, true);
			DP_DEBUG(DP_DBG_FLAG_BR_VLAN,
				 "vlan aware enable on bp(%d)\n",
				 dp_dev->bp);
		}
	}

SKIP_CROSS_MEMEBER_CHK:
	/* re-use member_bp variable for hairpin/cpu */
	if (is_hairpin(dp_dev, &member_bp))
		set_this_bp_bit(bpcfg, member_bp);
	if (is_cpu_enable(dp_dev, &member_bp))
		set_this_bp_bit(bpcfg, member_bp);

	bpcfg->bMacLearningLimitEnable = dp_dev->learning_limit_en;
	bpcfg->nMacLearningLimit = dp_dev->bp_learning_limit;
	if (is_cpu_enable(dp_dev, &member_bp))
		set_this_bp_bit(bpcfg, member_bp);
	bpcfg->eMask |= GSW_BRIDGE_PORT_CONFIG_MASK_MAC_LEARNING_LIMIT;

	if (dp_dbg_flag & DP_DBG_FLAG_SWDEV)
		dump_bp_cfg(dp_dev->dev, bpcfg);

	gsw_ops = dp_port_prop[dp_dev->inst].ops[0];
	if (gsw_ops->gsw_brdgport_ops.BridgePort_ConfigSet(gsw_ops, bpcfg)) {
		pr_err("DPM: %s: Fail in BridgePort_ConfigSet:\n", __func__);
		if (dp_dbg_flag & DP_DBG_FLAG_SWDEV)
			dump_bp_cfg(dp_dev->dev, bpcfg);
		goto EXIT;
	}
	trace_dp_swdev(true, dp_dev->bp, dp_dev->br_info->fid, dp_dev->dev);
	ret = DP_SUCCESS;
EXIT:
	kfree(bpcfg);
	return ret;
}

static int dp_reset_one_bp(struct dp_dev *dp_dev)
{
	GSW_BRIDGE_portConfig_t *bpcfg;
	struct core_ops *gsw_ops;
	int ret = DP_FAILURE;

	if (!dp_dev) {
		pr_err("DPM: %s %d, dp_dev: NULL\n", __func__, __LINE__);
		return DP_FAILURE;
	}

	bpcfg = dp_kzalloc(sizeof(*bpcfg), GFP_ATOMIC);
	if (!bpcfg)
		return DP_FAILURE;

	/* if dev still in bridge, it means this API call is trigger by
	 * the de-registration from DPM, which trigger a dummy call to
	 * update GSWIP setting. In this case, no need to continue since this
	 * BP is updated in the dp_set_all_bp_member stage.
	 * If dev not in bridge, in this case, it is triggered because of
	 * brctl delif action. So this case, we need to continue configure GSWIP
	 * since it is not in bridge and dp_set_all_bp_member will not update it
	 */
	if (dp_dev->br_info) {
		/* still in bridge, so it is de-registration from DPM */
		ret = DP_SUCCESS;
		trace_dp_swdev(false, dp_dev->bp, dp_dev->br_info->fid, dp_dev->dev);
		goto EXIT;
	}
	trace_dp_swdev(false, dp_dev->bp, -1, dp_dev->dev);

	if ((dp_dev->bp >= 0) && (dp_dev->inst >= 0)) {
		bpcfg->nBridgePortId = dp_dev->bp;
		/* default CPU_FID and no member */
		bpcfg->nBridgeId = CPU_FID;
		bpcfg->eMask = GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_ID |
			   GSW_BRIDGE_PORT_CONFIG_MASK_BRIDGE_PORT_MAP;

		gsw_ops = dp_port_prop[dp_dev->inst].ops[0];
		if (gsw_ops->gsw_brdgport_ops.BridgePort_ConfigSet(gsw_ops, bpcfg)) {
			pr_err("DPM: %s: Fail in BridgePort_ConfigSet:\n", __func__);
			dump_bp_cfg(dp_dev->dev, bpcfg);
			goto EXIT;
		}
		/* workaround done */
		dp_dev->inst = -1;
		dp_dev->ep = -1;
		dp_dev->bp = -1;
	}
	ret = DP_SUCCESS;
EXIT:
	kfree(bpcfg);
	return ret;
}

static int dp_set_all_bp_member(struct dp_dev *dp_dev)
{
	struct bridge_member_port *pos;
	int ret = DP_FAILURE;

	if (!dp_dev->br_info) {
		pr_err("DPM: %s: why dp_dev->br_info NULL for %s\n",
		       __func__, dp_dev->dev->name);
		goto EXIT;
	}
	if (dp_dev->bp <= 0) {
		pr_err("DPM: %s: why bp=%d: %s?\n", __func__, dp_dev->bp,
		       dp_dev->dev->name);
		goto EXIT;
	}

	/* loop each bridge port device to set its bp member */
	list_for_each_entry(pos, &dp_dev->br_info->bp_list, list)
		dp_set_one_bp_member(pos->dp_dev, &dp_dev->br_info->bp_list);

	ret = DP_SUCCESS;
EXIT:
	return ret;
}

int dp_swdev_stp_forward(struct dp_dev *dp_dev)
{
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s\n", dp_dev->dev->name);
	trace_dp_swdev_event(NULL, BR_STATE_FORWARDING, dp_dev->dev, NULL);
	dp_set_all_bp_member(dp_dev);
	return 0;
}

int dp_swdev_stp_disable(struct dp_dev *dp_dev)
{
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s\n", dp_dev->dev->name);
	trace_dp_swdev_event(NULL, BR_STATE_DISABLED, dp_dev->dev, NULL);
	dp_dev->f_stp_disabling = true;
	dp_set_all_bp_member(dp_dev);
	dp_reset_one_bp(dp_dev);
	dp_dev->f_stp_disabling = false;
	return 0;
}

int _dp_port_attr_br_flags_get(
	struct net_device *dev,
	struct switchdev_brport_flags *flags)
{
	struct net_device *br_dev;
	struct dp_dev *dp_dev;
	struct inst_info *i_info;
	int ret = -EINVAL; // 4.X ?? -EOPNOTSUPP;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
	unsigned long support_flags = BR_LEARNING | BR_MCAST_DEST_LOOKUP |
				      BR_MCAST_SRC_LOOKUP |
				      BR_HAIRPIN_MODE | BR_ISOLATED;
#endif
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s flags=0x%lx ...\n",
		 dev->name, flags->mask);
	if (dp_dbg_flag & DP_DBG_FLAG_SWDEV)
		dump_bp_flag(*flags);
	br_dev = netdev_master_upper_dev_get(dev);
	if (!br_dev)
		return ret;
	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev) {
		pr_err("DPM: %s: why dp_dev NULL for %s\n", __func__, dev->name);
		return 0;
	}
	if(!dp_dev->count)
		return ret;
	if (!dp_dev->br_info)
		return ret;
	if (!dp_dev->br_member_port)
		return ret;
	if (dp_dev->inst < 0)
		return ret;

	/* Note:
	 * 1) in Linux 4.x it is unsigned long brport_flags
	 * 2) in Linux 5.x it is a struct switchdev_brport_flags with value/mask
	 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
	if (flags->mask & ~support_flags) {
		DP_DEBUG(DP_DBG_FLAG_SWDEV,
			 "not supported flags=0x%lx\n",
			 flags->mask & ~support_flags);
		return ret;
	}
#endif
	flags->val |= dp_dev->br_member_port->hairpin ? BR_HAIRPIN_MODE : 0;
	flags->val |= dp_dev->br_member_port->isolate ? BR_ISOLATED : 0;

	i_info = get_dp_prop_info(dp_dev->inst);
	if (i_info->swdev_bridge_port_flags_get)
		ret = i_info->swdev_bridge_port_flags_get(dp_dev->inst,
							  dp_dev->bp,
							  &flags->val);
	return 0;
}

int _dp_swdev_port_attr_learning_limit_set(
	struct net_device *dev,
	int learning_limit)
{
	struct inst_info *i_info;
	int ret = -EOPNOTSUPP;
	struct dp_dev *dp_dev;

	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev) {
		pr_err("DPM: %s: why dp_dev NULL for %s\n", __func__, dev->name);
		return ret;
	}
	if (!dp_dev->count) /* not registerd to dpm yet */
		return ret;
	if (dp_dev->inst < 0)
		return ret;
	i_info = get_dp_prop_info(dp_dev->inst);
	if (!i_info->swdev_port_learning_limit_set)
		return ret;

	/* The maximum limited supported by the GSWIP 3.1 is 254 entries */
	if (learning_limit > i_info->cap.max_num_learn_limit_port) {
		pr_err("DPM: mac learning limit (%i) too high, max: %i\n",
		       learning_limit,
		       i_info->cap.max_num_learn_limit_port);
		return -EINVAL;
	}
	ret = i_info->swdev_port_learning_limit_set(dp_dev->inst,
						    dp_dev->bp,
						    learning_limit, dp_dev);
	return ret;
}

int _dp_swdev_bridge_attr_mcast_flood(
	struct net_device *bridge,
	enum br_mcast_flood_mode mcast_flood)
{
	struct inst_info *i_info;
	int inst;
	int br_id;

	DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s\n", bridge->name);
	/* get FID (bridge ID) */
	br_id = dp_get_fid_by_dev(bridge, &inst);
	if (br_id < 0) {
		pr_err("DPM: %s: No valid fid(%d) assigned: %s\n", __func__, br_id,
		       bridge->name);
		return -ENOENT;
	}
	if (inst < 0) {
		pr_err("DPM: %s: not valid inst=%d: %s\n", __func__, inst,
		       bridge->name);
		return -ENOENT;
	}

	i_info = get_dp_prop_info(inst);
	if (!i_info->swdev_bridge_mcast_flood)
		return -EOPNOTSUPP;

	return i_info->swdev_bridge_mcast_flood(inst, br_id, mcast_flood);
}

/* below API is for bridge port device only
 * For bridge, there is another ndo_xxx since their handling is different
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 15, 15)
int dp_bp_ndo_br_setlink2(struct net_device *dev, struct nlmsghdr *nlh,
				   u16 flags)
#else
int dp_bp_ndo_br_setlink2(struct net_device *dev, struct nlmsghdr *nlh,
			 u16 flags, struct netlink_ext_ack *extack)
#endif
{
	struct nlattr *attr, *protinfo;
	u8 mode;
	int rem;
	struct dp_dev *dp_dev;
	int ret = 0;

	DP_SWDEV_LOCK(&dp_swdev_lock);
	dp_dev = dp_dev_lookup(dev);
	if (!dp_dev) {
		pr_err("DPM: %s: why dp_dev NULL for %s\n", __func__, dev->name);
		goto exit;
	}
	DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s\n", dp_dev->dev->name);
	if(!dp_dev->count)
		goto exit;
	if (!dp_dev->br_info)
		goto exit;
	if (!dp_dev->br_member_port)
		goto exit;
	if (dp_dev->inst < 0)
		goto exit;
	protinfo = nlmsg_find_attr(nlh, sizeof(struct ifinfomsg),IFLA_PROTINFO);
	if (!protinfo)
		goto exit;
	nla_for_each_nested(attr, protinfo, rem) {
		if (nla_type(attr) == IFLA_BRPORT_MODE) {
			mode = nla_get_u8(attr);
			DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s hairpin=%d\n",
				 dp_dev->dev->name, mode);
			if (mode)
				dp_dev->br_member_port->hairpin = true;
			else
				dp_dev->br_member_port->hairpin = false;
			/* only need to update this dev */
			dp_set_one_bp_member(dp_dev, &dp_dev->br_info->bp_list);
		} else if (nla_type(attr) == IFLA_BRPORT_ISOLATED) {
			mode = nla_get_u8(attr);
			DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s isolated=%d\n",
				 dp_dev->dev->name, mode);
			if (mode)
				dp_dev->br_member_port->isolate = true;
			else
				dp_dev->br_member_port->isolate = false;
			/* need update this dev and its member both */
			dp_set_all_bp_member(dp_dev);
		} else
			DP_DEBUG(DP_DBG_FLAG_SWDEV, "%s type=%d\n",
				 dp_dev->dev->name, nla_type(attr));
	}
exit:
	DP_SWDEV_UNLOCK(&dp_swdev_lock);
	return ret;
}
EXPORT_SYMBOL(dp_bp_ndo_br_setlink2);

int dp_bp_ndo_br_dellink2(struct net_device *dev, struct nlmsghdr *nlh,
			  u16 flags)
{
	return 0;
}
EXPORT_SYMBOL(dp_bp_ndo_br_dellink2);

void dp_switchdev_exit(void)
{
	dp_swdev_br_tbl_free();
}

int dp_switchdev_init(void)
{
	return dp_swdev_br_tbl_init();
}

/* disable optimization in debug mode: pop */
DP_NO_OPTIMIZE_POP
