/*****************************************************************************
 *
 * Copyright (c) 2022 - 2023 MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <netlink/route/act/mirred.h>

#include <pon_adapter.h>
#include <omci/me/pon_adapter_mac_bridge_port_filter_preassign_table.h>
#include <omci/me/pon_adapter_mac_bridge_port_config_data.h>

#include "../pon_net_common.h"
#include "../pon_net_netlink.h"
#include "../pon_net_debug.h"
#include "../pon_net_dev_db.h"
#include "../pon_net_dev_defs.h"
#include "../pon_net_tc_filter.h"

#include "pon_net_mac_bridge_port_filter_preassign_table.h"
#include "pon_net_vlan_tagging_filter_data.h"
#include "pon_net_mac_bridge_port_config_data.h"

#define FILTER2(__name, __proto, __proto2, __dst_mac, __dst_mask) \
	{ .offset =  \
	   offsetof(struct pa_mac_bp_filter_preassign_table_upd_data, __name), \
	  .proto = __proto, \
	  .proto2 = __proto2, \
	  .dst_mac = __dst_mac, \
	  .dst_mask = __dst_mask, \
	}

#define FILTER(name, proto, dst_mac, dst_mask)                             \
	FILTER2(name, proto, ETH_P_ALL, dst_mac, dst_mask)

static const struct filter_cfg {
	/**
	 * Offset from the beginning of
	 * struct pa_mac_bp_filter_preassign_table_upd_data to the desired
	 * field
	 */
	unsigned int offset;
	/** Protocol for the TC filter */
	uint16_t proto;
	/** Protocol for the second TC filter - zero if not used*/
	uint16_t proto2;
	/** Destination MAC address to match - zero if not used*/
	uint64_t dst_mac;
	/**
	 * Mask, specifying which parts of MAC address will be matched.
	 * The value must be zero, if mask is not to be used.
	 */
	uint64_t dst_mask;
} filters[] = {
	FILTER(ipv4_multicast_filtering,
	       ETH_P_IP, 0x01005E000000, 0xFFFFFF000000),
	FILTER(ipv6_multicast_filtering,
	       ETH_P_IPV6, 0x333300000000, 0xFFFF00000000),
	FILTER(ipv4_broadcast_filtering, ETH_P_IP, 0xFFFFFFFFFFFF, 0),
	FILTER(rarp_filtering, ETH_P_RARP, 0xFFFFFFFFFFFF, 0),
	FILTER(ipx_filtering, ETH_P_IPX, 0xFFFFFFFFFFFF, 0),
	FILTER(net_beui_filtering, ETH_P_IP, 0x030000000001, 0),
	FILTER2(apple_talk_filtering,
		ETH_P_ATALK, ETH_P_AARP, 0xFFFFFFFFFFFF, 0),
	FILTER(bridge_management_inf_filtering,
	       ETH_P_IP, 0x0180C2000000, 0xFFFFFFFFFF00),
	FILTER(arp_filtering, ETH_P_ARP, 0xFFFFFFFFFFFF, 0),
	FILTER2(pppoe_filtering,
		ETH_P_PPP_DISC, ETH_P_PPP_SES, 0xFFFFFFFFFFFF, 0),
};

/*
 * This table determines which kinds of filters are supported on
 * each interface type
 */
struct supported_options {
	/*
	 * The type of the termination point on which
	 * the MAC Bridge Port Filter Preassign Table resides.
	 * The value of 0 means this should apply to any tp_type.
	 */
	uint8_t tp_type;
	/*
	 * The type of the termination point on which
	 * TC filters with mirred action are created.
	 * The value of 0 means this should apply to any tp_type.
	 */
	uint8_t other_tp_type;

	/* A mask indicating which settings are enabled */
	unsigned int enabled[ARRAY_SIZE(filters)];
} supported_options[] = {
	{ .tp_type = PA_BP_TP_TYPE_PMAP, .other_tp_type = 0,
	  .enabled = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
	{ .tp_type = 0, .other_tp_type = PA_BP_TP_TYPE_PMAP,
	  .enabled = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },
	{ .tp_type = 0, .other_tp_type = 0,
	  .enabled = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 } },
};

static const struct supported_options *
find_supported_options(uint8_t tp_type, uint8_t other_tp_type)
{
	const struct supported_options *s;
	unsigned int i;

	dbg_in_args("%u, %u", tp_type, other_tp_type);

	for (i = 0; i < ARRAY_SIZE(supported_options); ++i) {
		s = &supported_options[i];

		if (s->tp_type && s->tp_type != tp_type)
			continue;
		if (s->other_tp_type && s->other_tp_type != other_tp_type)
			continue;

		dbg_out_ret("%p", s);
		return s;
	}

	dbg_out_ret("%p", NULL);
	return NULL;
}

/* Convert MAC address from uint64_t to array of bytes */
static void mac_to_array(uint64_t mac, uint8_t *dst)
{
	unsigned int i;

	for (i = 0; i < ETH_ALEN; ++i)
		dst[i] = (mac >> 8 * (ETH_ALEN - i - 1)) & 0xFF;
}

/*
 * Read setting from 'cfg', using 'offset'. Offset is provided by
 * struct filter_cfg. If 'true' is returned it means that the packets
 * are blocked.
 */
static bool
read_setting(const struct pa_mac_bp_filter_preassign_table_upd_data *cfg,
	     unsigned int index)
{
	const struct filter_cfg *f;
	uint8_t *data;

	dbg_in_args("%p, %u", cfg, index);

	if (index >= ARRAY_SIZE(filters)) {
		dbg_err("There is no such filter in the table - out of bounds\n");
		dbg_out_ret("%d", 1);
		return true;
	}

	f = &filters[index];
	data = (uint8_t *)((unsigned char *)(cfg) + f->offset);

	dbg_out_ret("%d", !!(*data));
	return !!(*data);
}

static bool
does_filtering(const struct pa_mac_bp_filter_preassign_table_upd_data *cfg)
{
	unsigned int i;

	dbg_in_args("%p", cfg);

	for (i = 0; i < ARRAY_SIZE(filters); ++i) {
		if (read_setting(cfg, i)) {
			dbg_out_ret("%d", true);
			return true;
		}
	}

	dbg_out_ret("%d", false);
	return false;
}

/*
 * Read setting from 'cfg', using 'offset'. Offset is provided by
 * struct filter_cfg. Also consult with support table, to check
 * if the setting is supported. If setting is not supported, then
 * by default it is set to block packets (1).
 */
static bool read_supported_setting(
	const struct pa_mac_bp_filter_preassign_table_upd_data *cfg,
	unsigned int index, uint8_t tp_type, uint8_t other_tp_type)
{
	const struct supported_options *support;
	bool ret;

	dbg_in_args("%p, %u, %u, %u", cfg, index, tp_type, other_tp_type);

	support = find_supported_options(tp_type, other_tp_type);
	if (index >= ARRAY_SIZE(support->enabled)) {
		dbg_err("There is no such filter in the support table\n");
		dbg_out_ret("%d", 1);
		return true;
	}
	if (support && !support->enabled[index]) {
		dbg_out_ret("%d", true);
		return true;
	}

	ret = read_setting(cfg, index);

	dbg_out_ret("%d", ret);
	return ret;
}

/* Reference to the interface */
struct network_interface {
	/* Interface name */
	char ifname[IF_NAMESIZE];
	/* Termination point type */
	uint8_t tp_type;
};

/* Compare network interface names */
static int network_interface_cmp(const struct network_interface *a,
				 const struct network_interface *b)
{
	return strcmp(a->ifname, b->ifname);
}

/* Wrapper for qsort() */
static int network_interface_qsort_cmp(const void *a, const void *b)
{
	return network_interface_cmp(a, b);
}

/* Wrapper for pon_net_pair_walk() */
static int network_interface_pair_walk_cmp(const void *a, const void *b,
					   void *arg)
{
	(void)arg;

	return network_interface_cmp(a, b);
}

/** The configuration state of the MAC Bridge Port Filter Preassign Table */
struct configuration {
	/** Configured values */
	struct pa_mac_bp_filter_preassign_table_upd_data cfg;
	/** VLAN forwarding data */
	struct pon_net_vlan_forwarding fwd;
	/**
	 * Interfaces on which this configuration is configured.
	 * The interfaces are sorted by name, so that we can walk over them
	 * using pon_net_pair_walk()
	 */
	struct network_interface *interfaces;
	/** Number of interface in "interfaces" array */
	unsigned int interfaces_len;
};

static const struct configuration initial_configuration = {
	.cfg = {
		.ipv4_multicast_filtering = 1,
		.ipv6_multicast_filtering = 1,
		.ipv4_broadcast_filtering = 1,
		.rarp_filtering = 1,
		.ipx_filtering = 1,
		.net_beui_filtering = 1,
		.apple_talk_filtering = 1,
		.bridge_management_inf_filtering = 1,
		.arp_filtering = 1,
		.pppoe_filtering = 1,
	},
};

/* Initialize configuration */
static void configuration_init(struct configuration *cfg)
{
	dbg_in_args("%p", cfg);

	*cfg = initial_configuration;

	dbg_out();
}

/* Exit configuration */
static void configuration_exit(struct configuration *cfg)
{
	dbg_in_args("%p", cfg);

	if (cfg)
		free(cfg->interfaces);

	dbg_out();
}

/* Swap configuration */
static void configuration_swap(struct configuration *a, struct configuration *b)
{
	struct configuration tmp = *b;

	dbg_in_args("%p, %p", a, b);

	*b = *a;
	*a = tmp;

	dbg_out();
}

/*
 * Copy configuration. Both arguments must be initialized with
 * configuration_init().
 */
static enum pon_adapter_errno
configuration_copy(struct configuration *src, const struct configuration *dst)
{
	enum pon_adapter_errno ret;
	struct network_interface *interfaces = NULL;
	int err;

	dbg_in_args("%p, %p", src, dst);

	if (dst->interfaces_len) {
		interfaces = calloc(dst->interfaces_len, sizeof(*interfaces));
		if (!interfaces) {
			dbg_err_fn_ret(calloc, 0);
			ret = PON_ADAPTER_ERR_NO_MEMORY;
			goto err_out;
		}

		err = memcpy_s(interfaces,
			       dst->interfaces_len * sizeof(*interfaces),
			       dst->interfaces,
			       dst->interfaces_len * sizeof(*dst->interfaces));
		if (err) {
			dbg_err_fn_ret(memcpy_s, err);
			ret = PON_ADAPTER_ERR_MEM_ACCESS;
			goto err_free_interfaces;
		}
	}

	free(src->interfaces);
	src->cfg = dst->cfg;
	src->fwd = dst->fwd;
	src->interfaces = interfaces;
	src->interfaces_len = dst->interfaces_len;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

err_free_interfaces:
	free(interfaces);
err_out:
	dbg_out_ret("%d", ret);
	return ret;
}

struct pon_net_mac_bp_filter_preassign_table {
	/** The configuration, which is currently applied */
	struct configuration applied;

	/** The configuration, which is pending - to be applied */
	struct configuration pending;

	/** TC filter arrays for each filter type */
	struct pon_net_tc_filter_array filter_arrays[ARRAY_SIZE(filters)];
};

/*
 * Delete TC filters from 'array', starting from 'start' element
 * in the 'array', but only delete filters from interface called
 * 'ifname'. If 'ifname' is NULL, then delete all filters from
 * selected range, regardless of the interface name.
 */
static void delete_tc_filters(struct pon_net_netlink *netlink,
			      struct pon_net_dev_db *db,
			      struct pon_net_tc_filter_array *array,
			      const char *ifname,
			      unsigned int start)
{
	enum pon_adapter_errno ret;
	unsigned int j = 0;
	unsigned int count = array->count;

	dbg_in_args("%p, %p, %p, %p, %u", netlink, db, array, ifname, start);

	/* Check, if we can subtract without overflowing */
	if (start > count) {
		dbg_out();
		return;
	}

	for (j = 0; j < count - start; ++j) {
		/* Start deleting from last filter */
		unsigned int i = count - 1 - j;
		const struct pon_net_tc_filter *filter =
		    &array->filters[i];
		uint32_t handle = filter->handle;
		uint32_t prio = filter->prio;

		/* Match interface */
		if (ifname && strcmp(filter->ifname, ifname) != 0)
			continue;

		/* Return IDs to the pools */
		ret = pon_net_dev_db_put(db, filter->ifname,
					 filter->handle_pool_id, &handle, 1);
		if (ret != PON_ADAPTER_SUCCESS &&
		    ret != PON_ADAPTER_ERR_NOT_FOUND)
			dbg_err_fn_ret(pon_net_dev_db_put, ret);
		ret = pon_net_dev_db_put(db, filter->ifname,
					 filter->prio_pool_id, &prio, 1);
		if (ret != PON_ADAPTER_SUCCESS &&
		    ret != PON_ADAPTER_ERR_NOT_FOUND)
			dbg_err_fn_ret(pon_net_dev_db_put, ret);

		/* Remove the TC filter */
		ret = netlink_filter_clear_one(netlink, filter->ifname,
					       filter->proto, filter->prio,
					       filter->handle, filter->dir);
		if (ret != PON_ADAPTER_SUCCESS &&
		    ret != PON_ADAPTER_ERR_NOT_FOUND) {
			dbg_err_fn_ifname_ret(netlink_filter_clear_one,
					      filter->ifname, ret);
		}

		/*
		 * Swap deleted filter with last filter.
		 * Since we started deleting from the end, this filter
		 * has already been processed.
		 */
		array->filters[i] = array->filters[array->count - 1];
		array->count--;
	}

	dbg_out();
}

/*
 * Convert struct netlink_filter into struct pon_net_tc_filter.
 * Needed to store information about created filter for later deletion.
 */
static void into_tcf(const struct netlink_filter *filter,
		     struct pon_net_tc_filter *tcf,
		     unsigned int handle_pool,
		     unsigned int prio_pool)
{

	tcf->dir = filter->dir,
	tcf->handle = filter->handle,
	tcf->prio = (uint16_t)filter->prio,
	tcf->vlan_id = filter->vlan_id,
	tcf->proto = filter->proto != NETLINK_FILTER_UNUSED ?
			(uint16_t)filter->proto : ETH_P_ALL;
	tcf->handle_pool_id = handle_pool;
	tcf->prio_pool_id = prio_pool;
	snprintf(tcf->ifname, sizeof(tcf->ifname), "%s", filter->device);
}

/*
 * Generate TC filter priority and TC filter handle using "db",
 * and then add "filter" to the system using netlink_filter_add().
 * The added filter will be appended to the "array"
 */
static enum pon_adapter_errno add_filter(struct pon_net_netlink *netlink,
					 struct pon_net_dev_db *db,
					 struct netlink_filter *filter,
					 unsigned int prio_pool_id,
					 struct pon_net_tc_filter_array *array)
{
	uint32_t handle, prio;
	struct pon_net_tc_filter tcf;
	enum pon_adapter_errno ret, ret2;

	dbg_in_args("%p, %p, %p, %u, %p", netlink, db, filter, prio_pool_id,
		    array);

	ret = pon_net_dev_db_gen(db, filter->device,
				 PON_NET_HANDLE_FILTER_INGRESS, &handle, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_dev_db_gen, ret);
		goto err_out;
	}

	ret = pon_net_dev_db_gen(db, filter->device, prio_pool_id, &prio, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_dev_db_gen, ret);
		goto err_put_handle;
	}

	filter->handle = handle;
	filter->prio = (int)prio;

	into_tcf(filter, &tcf, PON_NET_HANDLE_FILTER_INGRESS, prio_pool_id);

	ret = pon_net_tc_filter_array_add(array, &tcf);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_tc_filter_array_add, ret);
		goto err_put_prio;
	}

	ret = netlink_filter_add(netlink, filter);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_filter_add, ret);
		goto err_pop_filter;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

err_pop_filter:
	pon_net_tc_filter_array_pop(array);
err_put_prio:
	ret2 = pon_net_dev_db_put(db, filter->device, prio_pool_id, &prio, 1);
	if (ret2 != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret2);
err_put_handle:
	ret2 = pon_net_dev_db_put(db, filter->device,
				  PON_NET_HANDLE_FILTER_INGRESS, &handle, 1);
	if (ret2 != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret2);
err_out:
	dbg_out_ret("%d", ret);
	return ret;
}

/* Create TC filters for filter configuration 'cfg'. */
static enum pon_adapter_errno
create_tc_filters(struct pon_net_netlink *netlink,
		  struct pon_net_dev_db *db,
		  const struct filter_cfg *cfg,
		  const struct pon_net_vlan_forwarding *fwd,
		  const char *ifname,
		  const char *egdev,
		  struct pon_net_tc_filter_array *array)
{
	enum pon_adapter_errno ret;
	struct netlink_filter filter = {0};
	int len;
	unsigned int i;
	struct netlink_mirred_data mirred = {0};
	unsigned int start = array->count;

	dbg_in_args("%p, %p, %p, %p, %p, %p, %p", netlink, db, cfg, fwd, ifname,
		    egdev, array);
	netlink_filter_defaults(&filter);

	len = snprintf(mirred.dev, sizeof(mirred.dev), "%s", egdev);
	if (len >= (int)sizeof(mirred.dev)) {
		FN_ERR_RET(len, snprintf, PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}
	mirred.action = TCA_EGRESS_REDIR;

	filter.dir = NETLINK_FILTER_DIR_INGRESS;
	filter.act = NETLINK_FILTER_ACT_MIRRED;
	filter.act_data = &mirred;

	len = snprintf(filter.device, sizeof(filter.device), "%s", ifname);
	if (len >= (int)sizeof(filter.device)) {
		FN_ERR_RET(len, snprintf, PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	ret = netlink_qdisc_clsact_create(netlink, ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_qdisc_clsact_create, ret);
		return ret;
	}

	if (cfg->dst_mac) {
		filter.eth_dst = true;
		mac_to_array(cfg->dst_mac, filter.eth_dst_addr);
	}
	if (cfg->dst_mask)
		mac_to_array(cfg->dst_mask, filter.eth_dst_mask);

	/* Create untagged filters */
	if (!fwd->tci_list_len && cfg->proto != ETH_P_ALL) {
		filter.proto = cfg->proto;

		ret = add_filter(netlink, db, &filter,
				 PON_NET_PRIO_PREASSIGN_INGRESS, array);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(add_filter, ret);
			goto err_delete_filters;
		}
	}
	if (!fwd->tci_list_len && cfg->proto2 != ETH_P_ALL) {
		filter.proto = cfg->proto2;

		ret = add_filter(netlink, db, &filter,
				 PON_NET_PRIO_PREASSIGN_INGRESS, array);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(add_filter, ret);
			goto err_delete_filters;
		}

	}

	/* Create tagged filters */
	filter.proto = ETH_P_8021Q;
	for (i = 0; i < fwd->tci_list_len; ++i) {
		filter.vlan_id = TCI_VID(fwd->tci_list[i]);

		if (cfg->proto != ETH_P_ALL) {
			filter.vlan_proto = cfg->proto;

			ret = add_filter(netlink, db, &filter,
					 PON_NET_PRIO_PREASSIGN_VLAN_INGRESS,
					 array);
			if (ret != PON_ADAPTER_SUCCESS) {
				dbg_err_fn_ret(add_filter, ret);
				goto err_delete_filters;
			}
		}

		if (cfg->proto2 != ETH_P_ALL) {
			filter.vlan_proto = cfg->proto2;

			ret = add_filter(netlink, db, &filter,
					 PON_NET_PRIO_PREASSIGN_VLAN_INGRESS,
					 array);
			if (ret != PON_ADAPTER_SUCCESS) {
				dbg_err_fn_ret(add_filter, ret);
				goto err_delete_filters;
			}
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

err_delete_filters:
	delete_tc_filters(netlink, db, array, ifname, start);

	dbg_out_ret("%d", ret);
	return ret;
}

/* Arguments to apply_on_if() */
struct apply_on_if_args {
	/** Pending configuration */
	const struct configuration *pending;
	/** Applied configuration */
	const struct configuration *applied;
	/** Netlink context */
	struct pon_net_netlink *netlink;
	/** Device database for handle generation */
	struct pon_net_dev_db *db;
	/** Array of TC filter arrays */
	struct pon_net_tc_filter_array *tc_filter_arrays;
	/* Interface */
	const struct network_interface *network_interface;
};

/*
 * This is a callback function passed to pon_net_pair_walk(). It will be
 * executed for pairs of matching interfaces.
 */
static enum pon_adapter_errno apply_on_if(void *__old_interface,
					  void *__new_interface, void *arg)
{
	struct apply_on_if_args *args = arg;
	const struct network_interface *old_interface = __old_interface;
	const struct network_interface *new_interface = __new_interface;
	enum pon_adapter_errno ret;
	unsigned int i;

	dbg_in_args("%p, %p, %p", __old_interface, __new_interface, arg);

	if (!old_interface && !new_interface) {
		dbg_err("Both interfaces can't be NULL\n");
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	for (i = 0; i < ARRAY_SIZE(filters); ++i) {
		const struct filter_cfg *f = &filters[i];
		struct pon_net_tc_filter_array *array =
		    &args->tc_filter_arrays[i];
		bool old = false;
		bool new = false;

		if (old_interface)
			old = read_supported_setting(
				  &args->applied->cfg, i,
				  args->network_interface->tp_type,
				  old_interface->tp_type) == 0;
		if (new_interface)
			new = read_supported_setting(
				  &args->pending->cfg, i,
				  args->network_interface->tp_type,
				  new_interface->tp_type) == 0;

		bool vlan_changed =
		    !pon_net_array_eq_u16(args->applied->fwd.tci_list,
					  args->applied->fwd.tci_list_len,
					  args->pending->fwd.tci_list,
					  args->pending->fwd.tci_list_len);

		/* Continue only if interfaces are the same and VLAN
		 * configuration did not change. Otherwise - nothing
		 * changed and we can skip deletion/creation.
		 */
		if (!vlan_changed && old == new)
			continue;

		/* Delete old configuration for this setting */
		if (old)
			delete_tc_filters(args->netlink, args->db, array,
					  old_interface->ifname, 0);

		/* Create new configuration for this setting */
		if (new) {
			ret = create_tc_filters(args->netlink, args->db, f,
						&args->pending->fwd,
						new_interface->ifname,
						args->network_interface->ifname,
						array);
			if (ret != PON_ADAPTER_SUCCESS) {
				dbg_out_ret("%d", ret);
				return ret;
			}
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Apply 'pending' configuration, taking into account already 'applied'
 * configuration, in order to reduce TC filter creations/deletions.
 * The 'ifname' is the interface name of the device on which the
 * MAC Bridge Port Preassign Table has been created
 */
static enum pon_adapter_errno
apply_configuration(struct pon_net_netlink *netlink,
		    struct pon_net_dev_db *db,
		    struct configuration *applied,
		    struct configuration *pending,
		    const struct network_interface *network_interface,
		    struct pon_net_tc_filter_array *tc_filter_arrays)
{
	struct apply_on_if_args apply_args = {
		.netlink = netlink,
		.db = db,
		.applied = applied,
		.pending = pending,
		.tc_filter_arrays = tc_filter_arrays,
		.network_interface = network_interface,
	};
	struct pon_net_pair_walk_args args = {
		.first_array = applied->interfaces,
		.first_array_len = applied->interfaces_len,
		.second_array = pending->interfaces,
		.second_array_len = pending->interfaces_len,
		.elem_size = sizeof(pending->interfaces[0]),
		.cmp = network_interface_pair_walk_cmp,
		.arg = &apply_args,
		.fn = apply_on_if,
	};
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %p, %p, %p, %p", netlink, db, applied, pending,
		    network_interface, tc_filter_arrays);

	ret = pon_net_pair_walk(&args);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_pair_walk, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Supported TP types on the ani side of the bridge */
static const uint16_t ani_tp_types[] = {
	PA_BP_TP_TYPE_PMAP,
	PA_BP_TP_TYPE_GEM,
	PA_BP_TP_TYPE_MC_GEM
};

/* Supported TP types on the UNI side of the bridge */
static const uint16_t uni_tp_types[] = {
	PA_BP_TP_TYPE_PPTP_UNI,
	PA_BP_TP_TYPE_VEIP,
};

/* Return 'true' if 'id' is in 'array' of 'capacity' length. */
static bool in_array(uint16_t id, const uint16_t *array, size_t capacity)
{
	unsigned int i;

	dbg_in_args("%u, %p, %zu", id, array, capacity);

	for (i = 0; i < capacity; ++i) {
		if (id == array[i]) {
			dbg_out_ret("%d", true);
			return true;
		}
	}

	dbg_out_ret("%d", false);
	return false;
}

/* If 'tp_type' on the opposite side of the bridge as the 'other_tp_type' */
static bool opposite_supported_tp_type(uint16_t tp_type, uint16_t other_tp_type)
{
	dbg_in_args("%u, %u", tp_type, other_tp_type);

	if (in_array(tp_type, uni_tp_types, ARRAY_SIZE(uni_tp_types)) &&
	    in_array(other_tp_type, ani_tp_types, ARRAY_SIZE(ani_tp_types))) {
		dbg_out_ret("%d", true);
		return true;
	}
	if (in_array(tp_type, ani_tp_types, ARRAY_SIZE(ani_tp_types)) &&
	    in_array(other_tp_type, uni_tp_types, ARRAY_SIZE(uni_tp_types))) {
		dbg_out_ret("%d", true);
		return true;
	}

	dbg_out_ret("%d", false);
	return false;
}

static enum pon_adapter_errno
actually_connected_to_bridge(struct pon_net_context *ctx, uint8_t tp_type,
			     uint16_t tp_ptr, int *result)
{
	enum pon_adapter_errno ret;
	char ifname[IF_NAMESIZE];

	dbg_in_args("%p, %u, %u, %p", ctx, tp_type, tp_ptr, result);

	ret = pon_net_tp_ifname_get(ctx, tp_type, tp_ptr,
				    ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_tp_ifname_get, ret);
		return ret;
	}

	ret = netlink_is_slave(ctx->netlink, ifname, result);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_is_slave, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/*
 * Return sorted array of interfaces on the opposite side of the
 * bridge to bridge port identified by "me_id"
 */
static unsigned int get_interfaces(struct pon_net_context *ctx, uint16_t me_id,
				   struct network_interface *interfaces,
				   unsigned int interfaces_capacity)
{
	struct pon_net_bridge_port_config *bp;
	struct pon_net_bridge_port_config *other_bp;
	struct pon_net_me_list_item *item;
	const uint16_t class_id = PON_CLASS_ID_BRIDGE_PORT_CONFIG;
	unsigned int count = 0;
	enum pon_adapter_errno ret;
	int actually_connected;

	dbg_in_args("%p, %u, %p, %u", ctx, me_id, interfaces,
		    interfaces_capacity);

	/* Get bridge port data */
	bp = pon_net_me_list_get_data(&ctx->me_list, class_id, me_id);
	if (!bp) {
		/* No interface names if bridge port does not exist */
		dbg_out_ret("%d", 0);
		return 0;
	}

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		other_bp = item->data;

		/* Skip bridge port connected to a different bridge */
		if (bp->bridge_me_id != other_bp->bridge_me_id)
			continue;

		if (!opposite_supported_tp_type(bp->tp_type, other_bp->tp_type))
			continue;

		ret = actually_connected_to_bridge(ctx, other_bp->tp_type,
						   other_bp->tp_ptr,
						   &actually_connected);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(actually_connected_to_bridge, ret);
			continue;
		}
		if (!actually_connected)
			continue;

		if (interfaces && count <= interfaces_capacity) {
			ret = pon_net_tp_ifname_get(ctx,
						    other_bp->tp_type,
						    other_bp->tp_ptr,
						    interfaces[count].ifname,
						    sizeof(interfaces[count]));
			if (ret != PON_ADAPTER_SUCCESS)
				dbg_err_fn_ret(pon_net_tp_ifname_get, ret);
		}

		count++;
	}

	if (interfaces)
		qsort(interfaces,
		      count < interfaces_capacity ? count : interfaces_capacity,
		      sizeof(*interfaces), network_interface_qsort_cmp);

	dbg_out_ret("%d", count);
	return count;
}

/* Read interfaces into the configuration */
static enum pon_adapter_errno
configuration_get_interfaces(struct configuration *cfg,
			  struct pon_net_context *ctx,
			  uint16_t me_id)
{
	unsigned int interfaces_len;
	struct network_interface *interfaces = NULL;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %u", cfg, ctx, me_id);

	interfaces_len = get_interfaces(ctx, me_id, NULL, 0);

	if (interfaces_len) {
		interfaces = calloc(interfaces_len, sizeof(*interfaces));
		if (!interfaces) {
			dbg_err_fn_ret(calloc, 0);
			ret = PON_ADAPTER_ERR_NO_MEMORY;
			goto err_out;
		}

		if (get_interfaces(ctx, me_id, interfaces, interfaces_len) !=
		    interfaces_len) {
			dbg_err_fn(get_interfaces);
			dbg_err("Unexpected number interfaces connected to the other side of the bridge\n");
			ret = PON_ADAPTER_ERR_OUT_OF_BOUNDS;
			goto err_free_interfaces;
		}
	}

	free(cfg->interfaces);
	cfg->interfaces = interfaces;
	cfg->interfaces_len = interfaces_len;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

err_free_interfaces:
	free(interfaces);
err_out:
	dbg_out_ret("%d", ret);
	return ret;
}

/* Update the MAC Bridge Port Filter Preassign Table configuration */
static enum pon_adapter_errno
table_update(struct pon_net_context *ctx, uint16_t me_id,
	     struct pon_net_mac_bp_filter_preassign_table *tbl)
{
	enum pon_adapter_errno ret;
	struct pon_net_vlan_forwarding fwd = {0};
	struct configuration pending;
	struct pon_net_bridge_port_config *bp;
	const uint16_t class_id = PON_CLASS_ID_BRIDGE_PORT_CONFIG;
	int should_apply;
	struct network_interface network_interface = {0};
	int len;

	dbg_in_args("%p, %u, %p", ctx, me_id, tbl);

	/* Get bridge port data */
	bp = pon_net_me_list_get_data(&ctx->me_list, class_id, me_id);
	if (!bp) {
		/*
		 * There is no bridge port data.
		 * This might happen if MAC Bridge Port Preassign table is
		 * created before MAC Bridge Port Config Data.
		 */
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	network_interface.tp_type = bp->tp_type;
	len = snprintf(network_interface.ifname,
		       sizeof(network_interface.ifname), "%s", bp->ifname);
	if (len >= (int)sizeof(network_interface.ifname)) {
		FN_ERR_RET(len, snprintf, PON_ADAPTER_ERR_OUT_OF_BOUNDS);
		return PON_ADAPTER_ERR_OUT_OF_BOUNDS;
	}

	/* Read VLAN filtering */
	ret = pon_net_vlan_tagging_filter_data_vlan_forwarding_get(ctx, me_id,
								   &fwd);
	if (ret != PON_ADAPTER_SUCCESS && ret != PON_ADAPTER_ERR_NOT_FOUND) {
		FN_ERR_RET(ret,
			   pon_net_vlan_tagging_filter_data_vlan_forwarding_get,
			   ret);
		return ret;
	}

	/*
	 * Only apply the settings if "bp->ifname" is really connected
	 * to the bridge
	 */
	ret = netlink_is_slave(ctx->netlink, bp->ifname, &should_apply);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_is_slave, ret);
		return ret;
	}

	configuration_init(&pending);

	pending.cfg = tbl->pending.cfg;

	if (should_apply) {
		pending.fwd = fwd;

		/*
		 * Read interface names on which the preassign filters will be
		 * configured
		 */
		ret = configuration_get_interfaces(&pending, ctx, me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(configuration_get_interfaces, ret);
			goto err_exit_configuration;
		}
	}

	ret = configuration_copy(&tbl->pending, &pending);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(configuration_copy, ret);
		goto err_exit_configuration;
	}

	/* Apply configuration to the system */
	ret =
	    apply_configuration(ctx->netlink, ctx->db, &tbl->applied, &pending,
				&network_interface, tbl->filter_arrays);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(apply_configuration, ret);
		goto err_exit_configuration;
	}

	configuration_swap(&tbl->applied, &pending);

err_exit_configuration:
	configuration_exit(&pending);

	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno
pon_net_mac_bridge_port_filter_preassign_table_update(
		struct pon_net_context *ctx,
		uint16_t *me_ids,
		unsigned int me_ids_len)
{
	enum pon_adapter_errno ret;
	struct pon_net_mac_bp_filter_preassign_table *tbl;
	unsigned int i = 0;

	dbg_in_args("%p, %p, %u", ctx, me_ids, me_ids_len);

	for (i = 0; i < me_ids_len; ++i) {
		uint16_t me_id = me_ids[i];

		tbl = pon_net_me_list_get_data(&ctx->me_list,
			       PON_CLASS_ID_BRIDGE_PORT_FILTER_PREASSIGN_TABLE,
			       me_id);
		if (!tbl)
			continue;

		ret = table_update(ctx, me_id, tbl);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, table_update, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

bool pon_net_mac_bridge_port_filter_preassign_table_configured(
	struct pon_net_context *ctx, uint16_t me_id)
{
	const uint16_t class_id =
	    PON_CLASS_ID_BRIDGE_PORT_FILTER_PREASSIGN_TABLE;
	struct pon_net_mac_bp_filter_preassign_table *tbl;

	dbg_in_args("%p, %u", ctx, me_id);

	tbl = pon_net_me_list_get_data(&ctx->me_list, class_id, me_id);
	if (!tbl) {
		dbg_out_ret("%d", false);
		return false;
	}

	if (does_filtering(&tbl->pending.cfg)) {
		dbg_out_ret("%d", true);
		return true;
	}

	dbg_out_ret("%d", false);
	return false;
}

static enum pon_adapter_errno
update(void *ll_handle, uint16_t me_id,
	const struct pa_mac_bp_filter_preassign_table_upd_data *upd_data)
{
	const uint16_t class_id =
	    PON_CLASS_ID_BRIDGE_PORT_FILTER_PREASSIGN_TABLE;
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret;
	struct pon_net_mac_bp_filter_preassign_table *tbl;
	struct pon_net_mac_bp_filter_preassign_table new = {0};
	bool create = false;
	unsigned int i;

	dbg_in_args("%p, %u, %p", ll_handle, me_id, upd_data);

	tbl = pon_net_me_list_get_data(&ctx->me_list, class_id, me_id);
	if (!tbl) {
		create = true;

		for (i = 0; i < ARRAY_SIZE(new.filter_arrays); ++i)
			pon_net_tc_filter_array_init(&new.filter_arrays[i]);

		configuration_init(&new.pending);
		configuration_init(&new.applied);

		ret = pon_net_me_list_write(&ctx->me_list, class_id, me_id,
					    &new, sizeof(new));
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(pon_net_me_list_write, ret);
			goto err_exit_configuration;
		}

		tbl = pon_net_me_list_get_data(&ctx->me_list, class_id, me_id);
		if (!tbl) {
			dbg_err_fn_ret(pon_net_me_list_get_data, ret);
			ret = PON_ADAPTER_ERROR;
			goto err_remove_me_list_entry;
		}
	}

	tbl->pending.cfg = *upd_data;

	ret = table_update(ctx, me_id, tbl);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(table_update, ret);
		goto err_remove_me_list_entry;
	}

	ret = pon_net_bp_vlan_forwarding_update(ctx, &me_id, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_bp_vlan_forwarding_update, ret);
		return PON_ADAPTER_ERROR;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

err_remove_me_list_entry:
	if (create)
		pon_net_me_list_remove(&ctx->me_list, class_id, me_id);
err_exit_configuration:
	if (create) {
		configuration_exit(&new.pending);
		configuration_exit(&new.applied);
		for (i = 0; i < ARRAY_SIZE(new.filter_arrays); ++i)
			pon_net_tc_filter_array_exit(&new.filter_arrays[i]);
	}
	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno destroy(void *ll_handle, uint16_t me_id)
{
	enum pon_adapter_errno ret;
	struct configuration pending;
	struct pon_net_context *ctx = ll_handle;
	const uint16_t class_id =
	    PON_CLASS_ID_BRIDGE_PORT_FILTER_PREASSIGN_TABLE;
	struct pon_net_mac_bp_filter_preassign_table *tbl;
	unsigned int i;

	dbg_in_args("%p, %u", ll_handle, me_id);

	tbl = pon_net_me_list_get_data(&ctx->me_list, class_id, me_id);
	if (!tbl) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	configuration_init(&pending);

	configuration_swap(&tbl->pending, &pending);

	ret = table_update(ctx, me_id, tbl);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(table_update, ret);

	configuration_exit(&tbl->pending);
	configuration_exit(&tbl->applied);

	for (i = 0; i < ARRAY_SIZE(tbl->filter_arrays); ++i)
		pon_net_tc_filter_array_exit(&tbl->filter_arrays[i]);

	pon_net_me_list_remove(&ctx->me_list, class_id, me_id);

	configuration_exit(&pending);

	ret = pon_net_bp_vlan_forwarding_update(ctx, &me_id, 1);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_bp_vlan_forwarding_update, ret);

	dbg_out_ret("%d", ret);
	return ret;
}

const struct pa_mac_bp_filter_preassign_table_ops
mac_bp_filter_preassign_table_ops = {
	.update = update,
	.destroy = destroy,
};
