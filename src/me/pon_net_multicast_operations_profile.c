/*****************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2018 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <netlink/route/act/mirred.h>

#include <pon_adapter.h>
#include <omci/me/pon_adapter_multicast_operations_profile.h>

#ifdef LINUX
#include <netinet/in.h>
#endif

#include "../pon_net_common.h"
#include "../pon_net_debug.h"
#include "../pon_net_netlink.h"
#include "../pon_net_rtnl.h"
#include "../pon_net_uni.h"
#include "../pon_net_dev_db.h"
#include "../pon_net_dev_defs.h"


/* purpose of the ME 309 MOP create function is to trigger
 * multicast configuration passing igmp version and interface index of
 * multicast GEM bridge port
 */
static enum pon_adapter_errno create(void *ll_handle, const uint16_t me_id,
				     const uint8_t igmp_version)
{
#ifdef MCC_DRV_ENABLE
	struct pon_net_context *ctx = ll_handle;
#endif

	dbg_in_args("%p, %u, %u", ll_handle, me_id, igmp_version);

#ifdef MCC_DRV_ENABLE
	++ctx->me_mop_cnt;
	if (ctx->me_mop_cnt == 1)
		ctx->me_mop_igmp_version = igmp_version;

	/* If Multicast GEM ifindex is present then try to enable it */
	if (ctx->mc_gem_ifindex > 0)
		pon_net_mcc_multicast_enable(ctx);
#endif

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
destroy(void *ll_handle, const uint16_t me_id)
{
#ifdef MCC_DRV_ENABLE
	struct pon_net_context *ctx = ll_handle;
#endif

	dbg_in_args("%p, %u", ll_handle, me_id);

#ifdef MCC_DRV_ENABLE
	--ctx->me_mop_cnt;
	if (ctx->me_mop_cnt == 0)
		pon_net_mcc_multicast_disable(ctx);
#endif

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static const uint16_t protos[] = { ETH_P_8021Q, ETH_P_8021AD };

struct pon_net_mc_subscriber_cfg {
	/* Handles for TC filters created in upstream */
	uint32_t ds_handles[7];
	/* Priorities for TC filters created in upstream */
	uint32_t ds_prios[7];
	/* Handles for TC filters created in upstream */
	uint32_t us_handles[36];
	/* Priorities for TC filters created in upstream */
	uint32_t us_prios[36];
	/* Multicast flow forward handles */
	uint32_t ff_handles[4];
	/* Multicast flow forward prios */
	uint32_t ff_prios[4];
	/* UNI ifname */
	char ifname[IF_NAMESIZE];
	/* Multicast UNI ifname */
	char ifname_mc[IF_NAMESIZE];
};

/**
 * List of fields in struct pon_net_mc_subscriber, which hold allocated handles
 * and priorities from pon_net_dev_db.
 */
#define ALLOCATIONS \
	F(ff_handles, PON_NET_HANDLE_FILTER_INGRESS, PON_MASTER_DEVICE) \
	F(ff_prios, PON_NET_PRIO_MC_FLOW_FORWARD_INGRESS, PON_MASTER_DEVICE) \
	F(us_handles, PON_NET_HANDLE_FILTER_INGRESS, ifname) \
	F(us_prios, PON_NET_PRIO_MC_INGRESS, ifname) \
	F(ds_handles, PON_NET_HANDLE_FILTER_EGRESS, ifname) \
	F(ds_prios, PON_NET_PRIO_MC_EGRESS, ifname)

/* Allocate TC filter handle and priorities from pon_net_dev_db */
static enum pon_adapter_errno
allocate_handles(struct pon_net_context *ctx,
		 struct pon_net_mc_subscriber_cfg *cfg)
{
	enum pon_adapter_errno ret;
	const char *ifname = cfg->ifname;

	const struct pon_net_dev_db_alloc allocations[] = {
#define F(x, y, z) { cfg->x, ARRAY_SIZE(cfg->x), z, y },
		ALLOCATIONS
#undef F
	};

	dbg_in_args("%p, %p, \"%s\"", ctx, cfg, ifname);

	ret = pon_net_dev_db_gen_many(ctx->db, allocations,
				      ARRAY_SIZE(allocations));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_gen_many, ret);
		return ret;
	}


	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Deallocate TC filter handle and priorities from pon_net_dev_db */
static void cleanup_handles(struct pon_net_context *ctx,
			    struct pon_net_mc_subscriber_cfg *cfg)
{
	const char *ifname = cfg->ifname;
	const struct pon_net_dev_db_alloc allocations[] = {
#define F(x, y, z) { cfg->x, ARRAY_SIZE(cfg->x), z, y },
		ALLOCATIONS
#undef F
	};

	dbg_in_args("%p, %p, \"%s\"", ctx, cfg, ifname);

	pon_net_dev_db_put_many(ctx->db, allocations, ARRAY_SIZE(allocations));

	dbg_out();
}

/**
 * Retrieves struct pon_net_mc_subscriber_cfg from me_list or creates a new one
 */
static struct pon_net_mc_subscriber_cfg *
get_or_create(struct pon_net_context *ctx, uint16_t me_id, const char *ifname,
	      const char *ifname_mc)
{
	const uint16_t class_id = PON_CLASS_ID_MC_SUBSCRIBER_CFG;
	enum pon_adapter_errno ret;
	struct pon_net_mc_subscriber_cfg *cfg;
	int count;

	dbg_in_args("%p, %u, \"%s\", \"%s\"", ctx, me_id, ifname, ifname_mc);

	cfg = pon_net_me_list_get_data(&ctx->me_list, class_id, me_id);
	if (!cfg) {
		struct pon_net_mc_subscriber_cfg data = {0};

		ret = pon_net_me_list_write(&ctx->me_list, class_id, me_id,
					    &data, sizeof(data));
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_me_list_write, 0);
			return NULL;
		}
	} else {
		dbg_out_ret("%p", cfg);
		return cfg;
	}

	/* Create new data - only once */
	cfg = pon_net_me_list_get_data(&ctx->me_list, class_id, me_id);
	if (!cfg) {
		FN_ERR_RET(ret, pon_net_me_list_get_data, 0);
		return NULL;
	}

	count = snprintf(cfg->ifname, sizeof(cfg->ifname), "%s", ifname);
	if (count >= (int)sizeof(cfg->ifname)) {
		pon_net_me_list_remove(&ctx->me_list, class_id, me_id);
		FN_ERR_RET(count, snprintf, 0);
		return NULL;
	}
	count = snprintf(cfg->ifname_mc, sizeof(cfg->ifname_mc), "%s",
			 ifname_mc);
	if (count >= (int)sizeof(cfg->ifname)) {
		pon_net_me_list_remove(&ctx->me_list, class_id, me_id);
		FN_ERR_RET(count, snprintf, 0);
		return NULL;
	}

	ret = allocate_handles(ctx, cfg);
	if (ret != PON_ADAPTER_SUCCESS) {
		pon_net_me_list_remove(&ctx->me_list, class_id, me_id);
		FN_ERR_RET(ret, allocate_handles, 0);
		return NULL;
	}

	dbg_out_ret("%p", cfg);
	return cfg;
}

static int tc_filter_set_ip_proto(struct netlink_filter *f)
{
	if (f->dir == NETLINK_FILTER_DIR_INGRESS)
		return f->icmpv6_type == 0 ? ETH_P_IP : ETH_P_IPV6;

	/* default ETH_P_ALL set by netlink_filter_defaults */
	if (f->proto == ETH_P_ALL)
		return ETH_P_ALL;

	return NETLINK_FILTER_UNUSED;
}

/* Create TC filters for multicast vlan operation */
static enum pon_adapter_errno
create_tc_filters(struct pon_net_context *ctx,
		  uint32_t me_id,
		  struct pon_net_mc_subscriber_cfg *cfg,
		  const char *ifname, enum netlink_filter_dir dir,
		  enum netlink_filter_act_vlan act, int vlan_id, int vlan_prio,
		  uint8_t ip_proto, uint8_t icmpv6_type, uint32_t *index)
{
	enum pon_adapter_errno ret;
	struct netlink_filter filter;
	struct netlink_vlan_data vlan_data;
	unsigned int i;
	unsigned int j;
	uint32_t *handles;
	uint32_t *prios;

	dbg_in_args("%p, %u, %p, \"%s\", %d, %d, %d, %d, %u, %u, %u", ctx,
		    me_id, cfg, ifname, dir, act, vlan_id, vlan_prio, ip_proto,
		    icmpv6_type, *index);

	if (dir == NETLINK_FILTER_DIR_INGRESS) {
		handles = cfg->us_handles;
		prios = cfg->us_prios;
	} else {
		handles = cfg->ds_handles;
		prios = cfg->ds_prios;
	}

	netlink_filter_defaults(&filter);
	filter.act = NETLINK_FILTER_ACT_VLAN;
	filter.dir = dir;
	filter.ip_proto = ip_proto;
	filter.icmpv6_type = icmpv6_type;
	snprintf(filter.device, sizeof(filter.device), "%s", ifname);

	netlink_vlan_defaults(&vlan_data);
	vlan_data.vlan_id = vlan_id;
	vlan_data.vlan_prio = vlan_prio;
	vlan_data.act_vlan = act;

	filter.act_data = &vlan_data;

	/* Untagged - pop is valid only for rules for single or double-tagged */
	if (vlan_data.act_vlan != NETLINK_FILTER_ACT_VLAN_POP) {
		/* Change the action "modify" for untagged packets as
		 * this action is trying to delete an existing tag.
		 */
		if (vlan_data.act_vlan == NETLINK_FILTER_ACT_VLAN_MODIFY)
			vlan_data.act_vlan = NETLINK_FILTER_ACT_VLAN_PUSH;
		filter.handle = handles[*index];
		filter.prio = (int)prios[*index];
		filter.proto = tc_filter_set_ip_proto(&filter);
		ret = netlink_filter_add(ctx->netlink, &filter);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_filter_add, ret);
			return ret;
		}

		/* Restore the action which might have been changed above */
		vlan_data.act_vlan = act;
	}

	/* Single tagged */
	for (i = 0; i < ARRAY_SIZE(protos); ++i) {
		(*index)++;
		filter.proto = protos[i];
		filter.handle = handles[*index];
		filter.prio = (int)prios[*index];
		filter.vlan_proto = tc_filter_set_ip_proto(&filter);
		ret = netlink_filter_add(ctx->netlink, &filter);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_filter_add, ret);
			return ret;
		}
	}

	/* Double tagged, loop over inner and outer tag */
	for (i = 0; i < ARRAY_SIZE(protos); ++i) {
		filter.proto = protos[i];
		for (j = 0; j < ARRAY_SIZE(protos); ++j) {
			(*index)++;

			filter.handle = handles[*index];
			filter.prio = (int)prios[*index];
			filter.vlan_proto = protos[j];
			filter.cvlan_proto = tc_filter_set_ip_proto(&filter);
			ret = netlink_filter_add(ctx->netlink, &filter);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, netlink_filter_add, ret);
				return ret;
			}
		}
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/* Destroy all multicast vlan operations related TC filters */
static void destroy_tc_filters(struct pon_net_context *ctx, uint16_t me_id,
			       struct pon_net_mc_subscriber_cfg *cfg,
			       const char *ifname, enum netlink_filter_dir dir,
			       uint32_t *index, uint8_t icmpv6_type)
{
	uint16_t priority;
	uint32_t handle, *handles, *prios;
	unsigned int i, j;
	uint16_t proto = ETH_P_ALL;

	dbg_in_args("%p, %u, %p, \"%s\", %d, %u", ctx, me_id, cfg, ifname, dir,
		    *index);

	if (dir == NETLINK_FILTER_DIR_INGRESS) {
		handles = cfg->us_handles;
		prios = cfg->us_prios;
		proto = icmpv6_type == 0 ? ETH_P_IP : ETH_P_IPV6;
	} else {
		handles = cfg->ds_handles;
		prios = cfg->ds_prios;
	}

	/* Untagged - pop is valid only for rules for single or double-tagged */
	handle = handles[*index];
	priority = (uint16_t)prios[*index];
	netlink_filter_clear_one(ctx->netlink, ifname, proto, priority,
				 handle, dir);

	/* Single tagged */
	for (i = 0; i < ARRAY_SIZE(protos); ++i) {
		(*index)++;
		handle = handles[*index];
		priority = (uint16_t)prios[*index];
		netlink_filter_clear_one(ctx->netlink, ifname, protos[i],
					 priority, handle, dir);
	}

	/* Double tagged, loop over inner and outer tag */
	for (i = 0; i < ARRAY_SIZE(protos); ++i) {
		for (j = 0; j < ARRAY_SIZE(protos); ++j) {
			(*index)++;
			handle = handles[*index];
			priority = (uint16_t)prios[*index];
			netlink_filter_clear_one(ctx->netlink, ifname,
						 protos[i], priority, handle,
						 dir);
		}
	}

	dbg_out();
}

/* Remove tc filters */
static void remove_ds_operation(struct pon_net_context *ctx,
				struct pon_net_mc_subscriber_cfg *cfg,
				uint16_t me_id,
				const char *ifname)
{
	uint32_t index = 0;

	dbg_in_args("%p, %p, %u, \"%s\"", ctx, cfg, me_id, ifname);

	destroy_tc_filters(ctx, me_id, cfg, ifname,
			   NETLINK_FILTER_DIR_EGRESS, &index, 0);

	dbg_out();
}

/*
 * Interpret tag_ctrl vlan operation and create corresponding filters in
 * downstream
 */
static enum pon_adapter_errno
apply_ds_operation(struct pon_net_context *ctx,
		   struct pon_net_mc_subscriber_cfg *cfg,
		   uint16_t me_id,
		   const char *ifname,
		   uint8_t tag_ctrl,
		   uint16_t tci,
		   uint16_t msp_vid)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int vlan_id = NETLINK_FILTER_UNUSED;
	int vlan_prio = NETLINK_FILTER_UNUSED;
	int act = NETLINK_FILTER_UNUSED;
	uint16_t pcp = TCI_PCP(tci);
	uint32_t index = 0;

	dbg_in_args("%p, %p, %u, \"%s\", %u, %u, %u", ctx, cfg, me_id, ifname,
		    tag_ctrl, tci, msp_vid);

	remove_ds_operation(ctx, cfg, me_id, ifname);

	if (msp_vid == PA_MOP_UNSPECIFIED_VID)
		msp_vid = TCI_VID(tci);

	switch (tag_ctrl) {
	case PA_MOP_DS_STRIP:
		act = NETLINK_FILTER_ACT_VLAN_POP;
		break;
	case PA_MOP_DS_ADD:
		act = NETLINK_FILTER_ACT_VLAN_PUSH;
		vlan_id = TCI_VID(tci);
		vlan_prio = pcp;
		break;
	case PA_MOP_DS_REPLACE:
		act = NETLINK_FILTER_ACT_VLAN_MODIFY;
		vlan_id = TCI_VID(tci);
		vlan_prio = pcp;
		break;
	case PA_MOP_DS_REPLACE_ONLY_VLAN:
		act = NETLINK_FILTER_ACT_VLAN_MODIFY;
		vlan_id = TCI_VID(tci);
		break;
	case PA_MOP_DS_ADD_MSC:
		act = NETLINK_FILTER_ACT_VLAN_PUSH;
		vlan_id = msp_vid;
		vlan_prio = pcp;
		break;
	case PA_MOP_DS_REPLACE_MSC:
		act = NETLINK_FILTER_ACT_VLAN_MODIFY;
		vlan_id = msp_vid;
		vlan_prio = pcp;
		break;
	case PA_MOP_DS_REPLACE_ONLY_VLAN_MSC:
		act = NETLINK_FILTER_ACT_VLAN_MODIFY;
		vlan_id = msp_vid;
		break;
	case PA_MOP_DS_PASS:
	default:
		dbg_out_ret("%d", ret);
		return ret;
	}

	index = 0;
	ret = create_tc_filters(ctx, me_id, cfg, ifname,
				NETLINK_FILTER_DIR_EGRESS, act, vlan_id,
				vlan_prio, 0, 0, &index);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, create_tc_filters, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/*
 * Create flow forwarding rule for multicast
 */
static enum pon_adapter_errno
multicast_flow_forward(struct pon_net_context *ctx,
		       uint32_t me_id,
		       const char *ifname,
		       const char *ifname2,
		       int proto,
		       int vlan_proto,
		       uint8_t ip_proto,
		       uint8_t icmpv6_type,
		       uint32_t handle,
		       uint32_t prio)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct netlink_filter filter = {0};
	struct netlink_mirred_data act_data = {0};

	dbg_in_args("%p, %u, \"%s\", \"%s\", %d, %d, %u, %u, %u, %u", ctx,
		    me_id, ifname, ifname2, proto, vlan_proto, ip_proto,
		    icmpv6_type, handle, prio);

	netlink_filter_defaults(&filter);

	snprintf(filter.device, sizeof(filter.device), "%s", ifname);
	snprintf(filter.indev, sizeof(filter.indev), "%s", ifname);
	snprintf(act_data.dev, sizeof(act_data.dev), "%s", ifname2);
	act_data.action = TCA_EGRESS_REDIR;

	filter.proto = proto;
	filter.vlan_proto = vlan_proto;
	filter.ip_proto = ip_proto;
	filter.icmpv6_type = icmpv6_type;
	filter.handle = handle;
	filter.prio = (int)prio;

	filter.dir = NETLINK_FILTER_DIR_INGRESS;
	filter.act = NETLINK_FILTER_ACT_MIRRED;
	filter.act_data = &act_data;

	ret = pon_net_rtnl_bridge_learning_set(ctx, ifname2, false);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_rtnl_bridge_learning_set, ret);
		return ret;
	}

	ret = netlink_filter_add(ctx->netlink, &filter);
	if (ret) {
		FN_ERR_RET(ret, netlink_filter_add, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static const uint8_t icmpv6_types[] = { ICMPV6_MGM_QUERY, ICMPV6_MGM_REPORT,
					ICMPV6_MGM_REDUCTION,
					ICMPV6_MLD2_REPORT};

/* Remove flow forwarding rules */
static void remove_flow_forwarding(struct pon_net_context *ctx,
				   const struct pon_net_mc_subscriber_cfg *cfg)
{
	unsigned int i;
	uint16_t protocol[] = { ETH_P_8021Q, ETH_P_8021Q, ETH_P_IPV6, ETH_P_IP};

	dbg_in_args("%p, %p", ctx, cfg);

	for (i = 0; i < ARRAY_SIZE(protocol); i++)
		netlink_filter_clear_one(ctx->netlink, PON_MASTER_DEVICE,
					 protocol[i],
					 (uint16_t)cfg->ff_prios[i],
					 cfg->ff_handles[i],
					 NETLINK_FILTER_DIR_INGRESS);

	dbg_out();
}

/*
 * Remove tc filters created based on tag_ctrl vlan operation
 */
static void remove_us_operation(struct pon_net_context *ctx,
				struct pon_net_mc_subscriber_cfg *cfg,
				uint16_t me_id,
				const char *ifname)
{
	uint32_t index, i;

	dbg_in_args("%p, %p, %u, \"%s\"", ctx, cfg, me_id, ifname);

	index = 0;
	destroy_tc_filters(ctx, me_id, cfg, ifname,
			   NETLINK_FILTER_DIR_INGRESS, &index, 0);

	for (i = 0; i < ARRAY_SIZE(icmpv6_types); ++i) {
		index++;
		destroy_tc_filters(ctx, me_id, cfg, ifname,
				   NETLINK_FILTER_DIR_INGRESS,
				   &index, icmpv6_types[i]);
	}

	remove_flow_forwarding(ctx, cfg);

	dbg_out();
}

/*
 * Interpret tag_ctrl vlan operation and create corresponding filters in
 * upstream
 */
static enum pon_adapter_errno
apply_us_operation(struct pon_net_context *ctx,
		   struct pon_net_mc_subscriber_cfg *cfg,
		   uint16_t me_id,
		   const char *ifname,
		   const char *ifname_mc,
		   uint8_t tag_ctrl, uint16_t tci)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int vlan_id = NETLINK_FILTER_UNUSED;
	int vlan_prio = NETLINK_FILTER_UNUSED;
	int act = NETLINK_FILTER_UNUSED;
	uint32_t index, i;

	dbg_in_args("%p, %p, %u, \"%s\", \"%s\", %u, %u", ctx, cfg, me_id,
		    ifname, ifname_mc, tag_ctrl, tci);

	remove_us_operation(ctx, cfg, me_id, ifname);

	switch (tag_ctrl) {
	case PA_MOP_US_PASS:
		/* We do not create filters - we pass all traffic */
		goto out;
	case PA_MOP_US_ADD:
		act = NETLINK_FILTER_ACT_VLAN_PUSH;
		vlan_id = TCI_VID(tci);
		vlan_prio = TCI_PCP(tci);
		break;
	case PA_MOP_US_REPLACE:
		act = NETLINK_FILTER_ACT_VLAN_MODIFY;
		vlan_id = TCI_VID(tci);
		vlan_prio = TCI_PCP(tci);
		break;
	case PA_MOP_US_REPLACE_ONLY_VLAN:
		act = NETLINK_FILTER_ACT_VLAN_MODIFY;
		vlan_id = TCI_VID(tci);
		break;
	default:
		goto out;
	}

	index = 0;
	ret = create_tc_filters(ctx, me_id, cfg, ifname,
				NETLINK_FILTER_DIR_INGRESS, act, vlan_id,
				vlan_prio, IPPROTO_IGMP, 0, &index);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(create_tc_filters, ret);
		goto out;
	}

	for (i = 0; i < ARRAY_SIZE(icmpv6_types); ++i) {
		index++;
		ret = create_tc_filters(ctx, me_id, cfg, ifname,
					NETLINK_FILTER_DIR_INGRESS, act,
					vlan_id, vlan_prio, IPPROTO_ICMPV6,
					icmpv6_types[i], &index);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(create_tc_filters, ret);
			goto out;
		}
	}

	ret = multicast_flow_forward(ctx, me_id, PON_MASTER_DEVICE,
				     ifname_mc, ETH_P_8021Q, ETH_P_IPV6,
				     IPPROTO_ICMPV6, ICMPV6_MGM_QUERY,
				     cfg->ff_handles[0],
				     cfg->ff_prios[0]);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(multicast_flow_forward, ret);
		goto out;
	}

	ret = multicast_flow_forward(ctx, me_id, PON_MASTER_DEVICE,
				     ifname_mc, ETH_P_8021Q, ETH_P_IP,
				     IPPROTO_IGMP, 0,
				     cfg->ff_handles[1],
				     cfg->ff_prios[1]);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(multicast_flow_forward, ret);
		goto out;
	}

	ret = multicast_flow_forward(ctx, me_id, PON_MASTER_DEVICE,
				     ifname_mc, ETH_P_IPV6,
				     NETLINK_FILTER_UNUSED, IPPROTO_ICMPV6,
				     ICMPV6_MGM_QUERY,
				     cfg->ff_handles[2],
				     cfg->ff_prios[2]);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(multicast_flow_forward, ret);
		goto out;
	}

	ret = multicast_flow_forward(ctx, me_id, PON_MASTER_DEVICE,
				     ifname_mc, ETH_P_IP, NETLINK_FILTER_UNUSED,
				     IPPROTO_IGMP, 0,
				     cfg->ff_handles[3],
				     cfg->ff_prios[3]);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(multicast_flow_forward, ret);
		goto out;
	}

out:
	dbg_out_ret("%d", ret);
	return ret;
}

/* Returns VEIP or PPTP Ethernet UNI interface name, depending on what is
 * configured. If 'mc' is true it will return interface for multicast */
static enum pon_adapter_errno uni_ifname_get(struct pon_net_context *ctx,
					     uint16_t lan_idx,
					     bool mc,
					     char *ifname,
					     unsigned int size)
{
	static const struct {
		uint16_t class_id;
		uint8_t uni_type;
		uint8_t mc_uni_type;
	} to_try[] = {
		{ PON_CLASS_ID_PPTP_ETHERNET_UNI, UNI_PPTP_ETH, UNI_PPTP_MC },
		{ PON_CLASS_ID_VEIP, UNI_VEIP, UNI_VEIP_MC },
	};
	unsigned int i = 0;
	enum pon_adapter_errno ret;
	uint16_t me_id;
	uint8_t type;

	dbg_in_args("%p, %u, %d, %p, %u", ctx, lan_idx, mc, ifname, size);

	for (i = 0; i < ARRAY_SIZE(to_try); ++i) {
		ret = pon_net_uni_me_id_by_lport_get(ctx, to_try[i].class_id,
						     (uint8_t)lan_idx, &me_id);
		if (ret == PON_ADAPTER_ERR_NOT_FOUND)
			continue;
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_uni_me_id_by_lport, ret);
			return ret;
		}

		type = mc ? to_try[i].mc_uni_type : to_try[i].uni_type;

		ret = pon_net_uni_ifname_get(ctx, type, lan_idx, ifname, size);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_uni_ifname_get, ret);
			return ret;
		}

		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_FOUND);
	return PON_ADAPTER_ERR_NOT_FOUND;
}

/* This function is called by me_update pointer from
 * Multicast Subscriber Config manage entity
 * Note please that this is intended to pass only MOP/EMOP vlan handling
 * not extVLAN, despite what is suggested by the name
 */
static enum pon_adapter_errno
mc_ext_vlan_update(void *ll_handle, const uint16_t me_id,
		   const struct pa_mc_profile_ext_vlan_update_data *update_data)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_context *ctx = ll_handle;
	char ifname[IF_NAMESIZE] = "";
	char ifname_mc[IF_NAMESIZE] = "";
	struct pon_net_mc_subscriber_cfg *cfg;

	dbg_in_args("%p, %u, %p", ll_handle, me_id, update_data);

	ret = uni_ifname_get(ctx, update_data->lan_idx, false, ifname,
			     sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, uni_ifname_get, ret);
		return ret;
	}

	ret = uni_ifname_get(ctx, update_data->lan_idx, true, ifname_mc,
			     sizeof(ifname_mc));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, uni_ifname_get, ret);
		return ret;
	}

	cfg = get_or_create(ctx, me_id, ifname, ifname_mc);
	if (!cfg) {
		FN_ERR_RET(0, get_or_create, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	ret = apply_us_operation(ctx,
				 cfg,
				 me_id,
				 ifname,
				 ifname_mc,
				 update_data->us_igmp_tag_ctrl,
				 update_data->us_igmp_tci);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, apply_us_operation, ret);
		return ret;
	}

	ret = apply_ds_operation(ctx,
				 cfg,
				 me_id,
				 ifname_mc,
				 update_data->ds_igmp_mc_tag_ctrl,
				 update_data->ds_igmp_mc_tci,
				 update_data->msp_vid);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, apply_ds_operation, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
mc_ext_vlan_clear(void *ll_handle, const uint16_t me_id, const uint16_t lan_idx)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_context *ctx = ll_handle;
	char ifname[IF_NAMESIZE] = "";
	char ifname_mc[IF_NAMESIZE] = "";
	const uint16_t class_id = PON_CLASS_ID_MC_SUBSCRIBER_CFG;
	struct pon_net_mc_subscriber_cfg *cfg;

	dbg_in_args("%p, %u, %u", ll_handle, me_id, lan_idx);

	cfg = pon_net_me_list_get_data(&ctx->me_list, class_id, me_id);
	if (!cfg) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = uni_ifname_get(ctx, lan_idx, false, ifname, sizeof(ifname));
	if (ret == PON_ADAPTER_SUCCESS) {
		ret = uni_ifname_get(ctx, lan_idx, true, ifname_mc,
				     sizeof(ifname_mc));
		if (ret == PON_ADAPTER_SUCCESS) {
			remove_ds_operation(ctx, cfg, me_id, ifname_mc);
			remove_us_operation(ctx, cfg, me_id, ifname);
		}
	}

	remove_flow_forwarding(ctx, cfg);

	cleanup_handles(ctx, cfg);

	pon_net_me_list_remove(&ctx->me_list, class_id, me_id);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno static_acl_table_entry_add(
	void *ll_handle,
	uint16_t me_id,
	const struct pa_mc_profile_static_acl_table_entry_add_data *add_data)
{
	dbg_in_args("%p, %u, %p", ll_handle, me_id, add_data);
	/* TODO: Implement this. For now it is here just to avoid SEGFAULTs */

	/* omci api implementation
	 * return (enum pon_adapter_errno)
	 * omci_api_multicast_operations_profile_static_acl_table_entry_add(
	 *	ll_handle,
	 *	me_id,
	 *	add_data->table_idx,
	 *	add_data->gem_port_id,
	 *	add_data->vlan_id,
	 *	add_data->src_ip_addr,
	 *	add_data->dest_ip_addr_mc_range_start,
	 *	add_data->dest_ip_addr_mc_range_end,
	 *	add_data->imputed_group_bandwidth);
	 */

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno static_acl_table_entry_rm(
	void *ll_handle,
	uint16_t me_id,
	uint16_t table_idx)
{
	dbg_in_args("%p, %u, %u", ll_handle, me_id, table_idx);
	/* TODO: Implement this. For now it is here just to avoid SEGFAULTs */

	/* omci api implementation
	 * return (enum pon_adapter_errno)
	 * omci_api_multicast_operations_profile_static_acl_table_entry_remove(
	 *	ll_handle,
	 *	me_id,
	 *	table_idx);
	 */

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pa_mc_profile_ops mc_profile_ops = {
	.create = create,
	.destroy = destroy,
	.mc_ext_vlan_update = mc_ext_vlan_update,
	.mc_ext_vlan_clear = mc_ext_vlan_clear,
	.static_acl_table_entry_add = static_acl_table_entry_add,
	.static_acl_table_entry_rm = static_acl_table_entry_rm,
};
