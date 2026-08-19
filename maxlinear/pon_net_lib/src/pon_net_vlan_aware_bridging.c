/*****************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include "pon_net_vlan_aware_bridging.h"
#include "pon_net_dev_db.h"
#include "pon_net_common.h"
#include "pon_net_debug.h"
#include "pon_net_netlink.h"
#include "pon_net_rtnl.h"

/* Entry on the VLAN list */
struct vlan_entry {
	SLIST_ENTRY(vlan_entry) entries;
	/* MAC Bridge Port Configuration data ME ID */
	uint16_t me_id;
	/* Interface name */
	char ifname[IF_NAMESIZE];
	/* Bridge interface name */
	char br_ifname[IF_NAMESIZE];
	/* VLAN ID */
	uint16_t vlan_id;
	/* Flag used to mark entries for deletion */
	bool marked_for_deletion;
};

/* Return 'true' if two instances of struct vlan_flow are equal */
static bool vlan_entry_eq(const struct vlan_entry *a,
			  const struct vlan_entry *b)
{
	return strcmp(a->ifname, b->ifname) == 0 &&
	       strcmp(a->br_ifname, b->br_ifname) == 0 &&
	       a->me_id == b->me_id &&
	       a->vlan_id == b->vlan_id;
}

SLIST_HEAD(vlan_list, vlan_entry);

/* Create vlan entry, return NULL if failed */
static struct vlan_entry *vlan_entry_create(uint16_t me_id, const char *ifname,
					    const char *br_ifname,
					    uint16_t vlan_id)
{
	struct vlan_entry *entry;
	int ret;

	dbg_in_args("%u, \"%s\", \"%s\", %u", me_id, ifname, br_ifname,
		    vlan_id);

	entry = calloc(1, sizeof(*entry));
	if (!entry) {
		dbg_out_ret("%p", NULL);
		return NULL;
	}

	entry->me_id = me_id;

	ret = snprintf(entry->ifname, sizeof(entry->ifname), "%s", ifname);
	if (ret >= (int)sizeof(entry->ifname)) {
		free(entry);
		FN_ERR_RET(ret, snprintf, 0);
		return NULL;
	}

	ret = snprintf(entry->br_ifname, sizeof(entry->br_ifname), "%s",
		       br_ifname);
	if (ret >= (int)sizeof(entry->ifname)) {
		free(entry);
		FN_ERR_RET(ret, snprintf, 0);
		return NULL;
	}

	entry->vlan_id = vlan_id;

	dbg_out_ret("%p", entry);
	return entry;
}

static void bport_vlan_del(struct pon_net_netlink *nl_ctx,
			   const char *ifname,
			   uint16_t vlan,
			   bool self)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, \"%s\", %u, %d", nl_ctx, ifname, vlan, self);

	ret = netlink_bport_vlan_del(nl_ctx, ifname, vlan, self);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(netlink_bport_vlan_del, ret);

	dbg_out();
}

/* Apply vlan entry to the system, by issuing ctx call */
static enum pon_adapter_errno vlan_entry_apply(const struct vlan_entry *entry,
					       struct pon_net_context *ctx,
					       bool apply_to_br)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p", entry, ctx);

	ret = netlink_bport_vlan_add(ctx->netlink, entry->ifname,
				     entry->vlan_id, false);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_dport_vlan_add, ret);
		return ret;
	}

	if (apply_to_br) {
		ret = netlink_bport_vlan_add(ctx->netlink, entry->br_ifname,
					     entry->vlan_id, true);
		if (ret != PON_ADAPTER_SUCCESS) {
			bport_vlan_del(ctx->netlink, entry->ifname,
				       entry->vlan_id, false);
			FN_ERR_RET(ret, netlink_dport_vlan_add, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Remove vlan entry from the system, by issuing ctx call */
static enum pon_adapter_errno vlan_entry_clear(const struct vlan_entry *entry,
					       struct pon_net_context *ctx,
					       bool apply_to_br)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	enum pon_adapter_errno ret2;

	dbg_in_args("%p, %p", entry, ctx);

	/* Only if the device was not removed before */
	if (netlink_netdevice_exists(ctx->netlink, entry->ifname)) {
		int is_slave = 0;

		/* Check if device is still in bridge */
		ret2 = netlink_is_slave(ctx->netlink, entry->ifname,
					&is_slave);
		if (ret2 != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(netlink_is_slave, ret2);
		if (is_slave) {
			/*
			 * Only if it is in the bridge, then attempt to remove
			 */
			ret2 = netlink_bport_vlan_del(ctx->netlink,
						      entry->ifname,
						      entry->vlan_id, false);
			if (ret2 != PON_ADAPTER_SUCCESS) {
				dbg_err_fn_ret(netlink_bport_vlan_del, ret2);
				ret = ret2;
			}
		}
	}

	if (apply_to_br) {
		/* Only if the device was not removed before */
		if (netlink_netdevice_exists(ctx->netlink, entry->br_ifname)) {
			ret2 = netlink_bport_vlan_del(ctx->netlink,
						      entry->br_ifname,
						      entry->vlan_id, true);
			if (ret2 != PON_ADAPTER_SUCCESS) {
				dbg_err_fn_ret(netlink_bport_vlan_del, ret2);
				ret = ret2;
			}
		}
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/* Destroy vlan entry */
static void vlan_entry_destroy(struct vlan_entry *entry)
{
	free(entry);
}

struct pon_net_vlan_aware_bridging {
	struct vlan_list vlans;
};

struct pon_net_vlan_aware_bridging *pon_net_vlan_aware_bridging_create(void)
{
	struct pon_net_vlan_aware_bridging *vap;

	dbg_in();

	vap = calloc(1, sizeof(*vap));
	if (!vap) {
		dbg_out_ret("%p", NULL);
		return NULL;
	}

	SLIST_INIT(&vap->vlans);

	dbg_out_ret("%p", NULL);
	return vap;
}

/* Mark all entries identified by 'me_id' in vlan list for deletion */
static void
mark_for_deletion(struct pon_net_vlan_aware_bridging *vab, uint16_t me_id)
{
	struct vlan_entry *i;

	dbg_in_args("%p, %u", vab, me_id);

	SLIST_FOREACH(i, &vab->vlans, entries)
		if (i->me_id == me_id)
			i->marked_for_deletion = true;

	dbg_out();
}

/*
 * Unmark entry marked for deletion if it is the same as 'entry' and return
 * 'true' if the entry was unmarked
 */
static bool unmark_one(struct pon_net_vlan_aware_bridging *vab, uint16_t me_id,
		       const struct vlan_entry *entry)
{
	struct vlan_entry *i;

	dbg_in_args("%p, %u, %p", vab, me_id, entry);

	SLIST_FOREACH(i, &vab->vlans, entries) {
		if (i->me_id != me_id)
			continue;

		if (!vlan_entry_eq(i, entry))
			continue;

		i->marked_for_deletion = false;
		dbg_out_ret("%d", true);
		return true;
	}

	dbg_out_ret("%d", false);
	return false;
}

/* Count number of occurrences of pair (vlan_id, br_ifname) in the vab */
static uint32_t
count_bridge_vid_entries(struct pon_net_vlan_aware_bridging *vab,
			 uint16_t vlan_id,
			 const char *br_ifname)
{
	struct vlan_entry *i;
	uint32_t count = 0;

	dbg_in_args("%p, %u, %s", vab, vlan_id, br_ifname);

	SLIST_FOREACH(i, &vab->vlans, entries) {
		if (i->vlan_id != vlan_id)
			continue;

		if (strcmp(i->br_ifname, br_ifname))
			continue;

		count++;
	}
	dbg_out_ret("%d", count);
	return count;
}

/* Creates multiple VLAN ID entries, and applies them to the system */
static enum pon_adapter_errno
create_vlan_entries(struct pon_net_vlan_aware_bridging *vab,
		    uint16_t me_id,
		    const char *ifname,
		    const char *br_ifname, const uint16_t *tci_list,
		    uint8_t tci_list_len, struct pon_net_context *ctx)
{
	unsigned int i = 0;
	struct vlan_entry *entry;
	enum pon_adapter_errno ret;
	bool exists;
	bool apply_to_br;

	dbg_in_args("%p, %u, \"%s\", \"%s\", %p, %u, %p", vab, me_id, ifname,
		    br_ifname, tci_list, tci_list_len, ctx);

	for (i = 0; i < tci_list_len; ++i) {
		/* Create entry */
		entry =
		    vlan_entry_create(me_id, ifname, br_ifname, tci_list[i]);
		if (!entry) {
			dbg_err_fn_ret(vlan_entry_create, 0);
			continue;
		}

		/*
		 * If 'entry' already exists, there is no need to create it
		 * again As all flows were previously marked for deletion, we
		 * unmark it so that it is not deleted afterwards
		 */
		exists = unmark_one(vab, me_id, entry);
		if (exists) {
			vlan_entry_destroy(entry);
			continue;
		}

		/* Add the VLAN to the bridge only if it will be the first
		 * occurrence of the VLAN related to the bridge, so first check
		 * if there is no one such entry int the vab list
		 */
		apply_to_br = !count_bridge_vid_entries(vab, entry->vlan_id,
							entry->br_ifname);

		/* Apply entry to the system */
		ret = vlan_entry_apply(entry, ctx, apply_to_br);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(vlan_entry_apply, ret);
			vlan_entry_destroy(entry);
			continue;
		}

		/* Place it on the list for cleanup later */
		SLIST_INSERT_HEAD(&vab->vlans, entry, entries);
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Removes entries that were marked for deletion */
static void cleanup_marked_vlan_entries(struct pon_net_vlan_aware_bridging *vab,
					uint16_t me_id,
					struct pon_net_context *ctx)
{
	struct vlan_entry *i, *next;
	enum pon_adapter_errno ret;
	uint32_t br_vid_count;
	bool apply_to_br;

	dbg_in_args("%p, %u, %p", vab, me_id, ctx);

	FOREACH_SAFE(i, next, &vab->vlans, entries) {
		if (i->me_id != me_id)
			continue;

		if (!i->marked_for_deletion)
			continue;

		br_vid_count = count_bridge_vid_entries(vab, i->vlan_id,
							i->br_ifname);
		/* Remove this VLAN from the bridge, only if it is the last
		 * occurrence of the pair (vlan_id, br_ifname) in the vab list
		 */
		apply_to_br = (br_vid_count == 1);

		if (br_vid_count == 0)
			dbg_wrn("vab: VLAN id: %u on bridge %s not found\n",
				i->vlan_id, i->br_ifname);

		SLIST_REMOVE(&vab->vlans, i, vlan_entry, entries);

		ret = vlan_entry_clear(i, ctx, apply_to_br);
		if (ret != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(vlan_entry_clear, ret);

		vlan_entry_destroy(i);
	}

	dbg_out();
}

/* Removes already configured VLAN entries */
static void cleanup_vlan_entries(struct pon_net_vlan_aware_bridging *vab,
				 uint16_t me_id, struct pon_net_context *ctx)
{
	dbg_in_args("%p, %u, %p", vab, me_id, ctx);

	mark_for_deletion(vab, me_id);

	cleanup_marked_vlan_entries(vab, me_id, ctx);

	dbg_out();
}

static void cleanup_all_vlan_entries(struct pon_net_vlan_aware_bridging *vab,
				     struct pon_net_context *ctx)
{
	struct vlan_entry *i, *next;
	enum pon_adapter_errno ret;
	uint32_t br_vid_count;
	bool apply_to_br;

	dbg_in_args("%p, %p", vab, ctx);

	FOREACH_SAFE(i, next, &vab->vlans, entries) {

		br_vid_count = count_bridge_vid_entries(vab, i->vlan_id,
							i->br_ifname);
		/* Remove this VLAN from the bridge, only if it is the last
		 * occurrence of this pair (vlan_id, br_ifname) in the vab list
		 */
		apply_to_br = (br_vid_count == 1);

		if (br_vid_count == 0)
			dbg_wrn("vab: VLAN id: %u on bridge %s not found\n",
				i->vlan_id, i->br_ifname);

		SLIST_REMOVE(&vab->vlans, i, vlan_entry, entries);

		ret = vlan_entry_clear(i, ctx, apply_to_br);
		if (ret != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(vlan_entry_clear, ret);

		vlan_entry_destroy(i);
	}

	dbg_out();
}

enum pon_adapter_errno
pon_net_vlan_aware_bridging_update(struct pon_net_vlan_aware_bridging *vab,
				   struct pon_net_me_list *me_list,
				   uint16_t me_id,
				   const uint16_t *tci_list,
				   uint8_t tci_list_len,
				   struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;
	const struct pon_net_bridge_port_config *bp;

	dbg_in_args("%p, %p, %u, %p, %u, %p", vab, me_list, me_id, tci_list,
		    tci_list_len, ctx);


	/* Get bridge port data */
	bp = pon_net_me_list_get_data(me_list, PON_CLASS_ID_BRIDGE_PORT_CONFIG,
				      me_id);
	if (!bp) {
		FN_ERR_RET(0, pon_net_me_list_get_data,
			   PON_ADAPTER_ERR_NOT_FOUND);
		return PON_ADAPTER_ERR_NOT_FOUND;
	}

	ret = pon_net_rtnl_bridge_learning_set(ctx, bp->ifname, true);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_rtnl_bridge_learning_set, ret);
		return ret;
	}

	/* Mark all entries for deletion for this 'me_id' */
	mark_for_deletion(vab, me_id);

	/* Create new entries or unmark existing ones */
	ret = create_vlan_entries(vab, me_id, bp->ifname, bp->br_ifname,
				  tci_list, tci_list_len, ctx);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, create_vlan_entries, ret);
		return ret;
	}

	/* Delete entries which are still marked for deletion */
	cleanup_marked_vlan_entries(vab, me_id, ctx);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_net_vlan_aware_bridging_clear(struct pon_net_vlan_aware_bridging *vab,
				  uint16_t me_id,
				  struct pon_net_context *ctx)
{
	dbg_in_args("%p, %u, %p", vab, me_id, ctx);

	cleanup_vlan_entries(vab, me_id, ctx);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

void
pon_net_vlan_aware_bridging_destroy(struct pon_net_vlan_aware_bridging *vab,
				    struct pon_net_context *ctx)
{
	dbg_in_args("%p, %p", vab, ctx);

	cleanup_all_vlan_entries(vab, ctx);

	free(vab);

	dbg_out();
}
