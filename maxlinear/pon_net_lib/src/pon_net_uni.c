/*****************************************************************************
 *
 * Copyright (c) 2020 - 2024 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <omci/me/pon_adapter_pptp_ethernet_uni.h>
#include <netlink/route/act/mirred.h>

#include "pon_net_common.h"
#include "pon_net_debug.h"
#include "pon_net_netlink.h"
#include "pon_net_ethtool.h"
#include "pon_net_qdisc.h"
#include "pon_net_qdisc_build.h"
#include "pon_net_uni.h"
#include "pon_net_dev_db.h"
#include "pon_net_dev_defs.h"
#include "me/pon_net_priority_queue.h"

#include <omci/me/pon_adapter_mac_bridge_port_config_data.h>
#include <omci/me/pon_adapter_onu_g.h>

/* This value should be supported by MxL vUNI driver on PRX and URX. */
#define JUMBO_FRAME_MTU 9216

bool pon_net_is_ext_uni(struct pon_net_context *ctx,
			uint16_t lport)
{
	dbg_in_args("%p, %u", ctx, lport);

	if (strnlen_s(ctx->cfg.uni_ext_master_name[lport], IF_NAMESIZE) > 0) {
		dbg_out_ret("%d", true);
		return true;
	}

	dbg_out_ret("%d", false);
	return false;
}

char *pon_net_ext_uni_master_get(struct pon_net_context *ctx,
				 uint16_t lport)
{
	return ctx->cfg.uni_ext_master_name[lport];
}

enum pon_adapter_errno pon_net_ext_uni_ifname_get(struct pon_net_context *ctx,
						  uint16_t class_id,
						  uint16_t me_id,
						  char *ifname, size_t size)
{
	uint16_t lport;
	int expected_size;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %u, %p, %zd", ctx, class_id, me_id, ifname, size);

	if (class_id != PON_CLASS_ID_PPTP_ETHERNET_UNI)
		goto ifname_get_default;

	ret = pon_net_uni_lport_get(me_id, &lport);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_uni_lport_get, ret);
		return ret;
	}

	if (!pon_net_is_ext_uni(ctx, lport))
		goto ifname_get_default;

	expected_size = snprintf(ifname, size, "%s",
				 pon_net_ext_uni_master_get(ctx, lport));
	if (expected_size < 0 || (unsigned int)expected_size + 1 > size) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_SIZE);
		return PON_ADAPTER_ERR_SIZE;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

ifname_get_default:
	ret = pon_net_ifname_get(ctx, class_id, me_id, ifname, size);
	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
uni_ext_init(struct uni *uni,
	     struct pon_net_context *ctx,
	     const char *ifname,
	     uint16_t lport)
{
	enum pon_adapter_errno ret, ret2;

	dbg_in_args("%p, %p, \"%s\"", uni, ctx, ifname);

	if (!pon_net_is_ext_uni(ctx, lport)) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = pon_net_dev_db_gen(ctx->db,
				 pon_net_ext_uni_master_get(ctx, lport),
				 PON_NET_HANDLE_FILTER_INGRESS,
				 uni->ext_handle_ingress, PON_EXT_INGRESS_CNT);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_dev_db_gen, ret);
		return ret;
	}

	ret = pon_net_dev_db_gen(ctx->db, ifname, PON_NET_HANDLE_FILTER_EGRESS,
				 uni->ext_handle_egress, PON_EXT_EGRESS_CNT);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_dev_db_gen, ret);
		goto cleanup_ingress_handle;
	}

	ret = pon_net_dev_db_gen(ctx->db,
				 pon_net_ext_uni_master_get(ctx, lport),
				 PON_NET_PRIO_VLAN_FILTERING_INGRESS,
				 uni->ext_prio_ingress, PON_EXT_INGRESS_CNT);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_dev_db_gen, ret);
		goto cleanup_egress_handle;
	}

	ret = pon_net_dev_db_gen(ctx->db, ifname,
				 PON_NET_PRIO_VLAN_FILTERING_EGRESS,
				 uni->ext_prio_egress, PON_EXT_EGRESS_CNT);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_dev_db_gen, ret);
		goto cleanup_prio_ingress;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

cleanup_prio_ingress:
	ret2 = pon_net_dev_db_put(ctx->db, ifname,
				  PON_NET_PRIO_VLAN_FILTERING_INGRESS,
				  uni->ext_prio_ingress, PON_EXT_INGRESS_CNT);
	if (ret2 != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret2);
cleanup_egress_handle:
	ret2 = pon_net_dev_db_put(ctx->db, ifname,
				  PON_NET_HANDLE_FILTER_EGRESS,
				  uni->ext_handle_egress, PON_EXT_EGRESS_CNT);
	if (ret2 != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret2);
cleanup_ingress_handle:
	ret2 = pon_net_dev_db_put(ctx->db, ifname,
				  PON_NET_HANDLE_FILTER_INGRESS,
				  uni->ext_handle_ingress, PON_EXT_INGRESS_CNT);
	if (ret2 != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret2);

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno uni_init(struct uni *uni,
				       struct pon_net_context *ctx,
				       const char *ifname,
				       uint16_t lport)
{
	const char *dev = ifname;
	enum pon_adapter_errno ret, ret2;

	dbg_in_args("%p, %p, \"%s\"", uni, ctx, ifname);

	uni->qdiscs = pon_net_qdiscs_create();
	if (!uni->qdiscs) {
		FN_ERR_RET(0, pon_net_qdiscs_create, PON_ADAPTER_ERR_NO_MEMORY);
		return PON_ADAPTER_ERR_NO_MEMORY;
	}

	if (pon_net_is_ext_uni(ctx, lport))
		dev = pon_net_ext_uni_master_get(ctx, lport);

	ret = pon_net_dev_db_gen(ctx->db, dev,
				 PON_NET_PRIO_TC_TO_QUEUE_ASSIGNMENT,
				 uni->prios, ARRAY_SIZE(uni->prios));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_gen, ret);
		return ret;
	}

	ret =
	    pon_net_dev_db_gen(ctx->db, dev, PON_NET_HANDLE_FILTER_INGRESS,
			       uni->handles, ARRAY_SIZE(uni->handles));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_gen, ret);
		goto cleanup_prios;
	}

	ret = pon_net_dev_db_gen(ctx->db, ifname,
				 PON_NET_PRIO_VLAN_FILTERING_EGRESS,
				 uni->prios_reassign,
				 ARRAY_SIZE(uni->prios_reassign));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_gen, ret);
		goto cleanup_handles;
	}

	ret = pon_net_dev_db_gen(ctx->db, ifname,
				 PON_NET_HANDLE_FILTER_EGRESS,
				 uni->handles_reassign,
				 ARRAY_SIZE(uni->handles_reassign));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_gen, ret);
		goto cleanup_prios_reassign;
	}

	ret = uni_ext_init(uni, ctx, ifname, lport);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(uni_ext_init, ret);
		goto cleanup_handles_reassign;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

cleanup_handles_reassign:
	ret2 = pon_net_dev_db_put(ctx->db, ifname,
				  PON_NET_HANDLE_FILTER_EGRESS,
				  uni->handles_reassign,
				  ARRAY_SIZE(uni->handles_reassign));
	if (ret2 != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret2);

cleanup_prios_reassign:
	ret2 = pon_net_dev_db_put(ctx->db, ifname,
				  PON_NET_PRIO_VLAN_FILTERING_EGRESS,
				  uni->prios_reassign,
				  ARRAY_SIZE(uni->prios_reassign));
	if (ret2 != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret2);

cleanup_handles:
	ret2 = pon_net_dev_db_put(ctx->db, dev,
				  PON_NET_HANDLE_FILTER_INGRESS,
				  uni->handles, ARRAY_SIZE(uni->handles));
	if (ret2 != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret2);
cleanup_prios:
	ret2 = pon_net_dev_db_put(ctx->db, dev,
				  PON_NET_PRIO_TC_TO_QUEUE_ASSIGNMENT,
				  uni->prios, ARRAY_SIZE(uni->prios));
	if (ret2 != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret2);

	dbg_out_ret("%d", ret);
	return ret;
}

static void uni_ext_exit(struct uni *uni,
			 struct pon_net_context *ctx,
			 const char *ifname,
			 uint16_t lport)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, \"%s\"", uni, ctx, ifname);

	if (!pon_net_is_ext_uni(ctx, lport)) {
		dbg_out();
		return;
	}

	ret = pon_net_dev_db_put(ctx->db,
				 pon_net_ext_uni_master_get(ctx, lport),
				 PON_NET_HANDLE_FILTER_INGRESS,
				 uni->ext_handle_ingress,
				 PON_EXT_INGRESS_CNT);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret);

	ret = pon_net_dev_db_put(ctx->db, ifname,
				 PON_NET_HANDLE_FILTER_EGRESS,
				 uni->ext_handle_egress, PON_EXT_EGRESS_CNT);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret);

	ret = pon_net_dev_db_put(ctx->db,
				 pon_net_ext_uni_master_get(ctx, lport),
				 PON_NET_PRIO_VLAN_FILTERING_INGRESS,
				 uni->ext_prio_ingress, PON_EXT_INGRESS_CNT);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret);

	ret = pon_net_dev_db_put(ctx->db, ifname,
				 PON_NET_PRIO_VLAN_FILTERING_EGRESS,
				 uni->ext_prio_egress, PON_EXT_EGRESS_CNT);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret);

	dbg_out();
}

static void remove_tc_to_queue_mappings(struct pon_net_context *ctx,
					struct uni *uni,
					uint16_t lport)
{
	char *ext_switch;
	uint16_t idx;

	if (!pon_net_is_ext_uni(ctx, lport))
		return;

	ext_switch = pon_net_ext_uni_master_get(ctx, lport);

	for (idx = 0; idx < PON_TRAFFIC_CLASS_CNT_MAX; ++idx) {
		netlink_filter_clear_one(ctx->netlink, ext_switch,
					 ETH_P_8021AD,
					 (uint16_t)uni->prios[idx],
					 uni->handles[idx],
					 NETLINK_FILTER_DIR_INGRESS);
	}
}

static void uni_exit(struct uni *uni, struct pon_net_context *ctx,
		     const char *ifname, uint16_t lport)
{
	const char *dev = ifname;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, \"%s\"", uni, ctx, ifname);

	if (pon_net_is_ext_uni(ctx, lport))
		dev = pon_net_ext_uni_master_get(ctx, lport);

	if (uni->qdiscs)
		pon_net_qdiscs_destroy(uni->qdiscs);

	uni->qdiscs = NULL;

	ret = pon_net_dev_db_put(ctx->db, dev,
				 PON_NET_PRIO_TC_TO_QUEUE_ASSIGNMENT,
				 uni->prios, ARRAY_SIZE(uni->prios));
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret);

	ret = pon_net_dev_db_put(ctx->db, dev, PON_NET_HANDLE_FILTER_INGRESS,
				 uni->handles, ARRAY_SIZE(uni->handles));
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret);

	ret = pon_net_dev_db_put(ctx->db, ifname, PON_NET_HANDLE_FILTER_EGRESS,
				 uni->handles_reassign,
				 ARRAY_SIZE(uni->handles_reassign));
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret);

	ret = pon_net_dev_db_put(ctx->db, ifname,
				 PON_NET_PRIO_VLAN_FILTERING_EGRESS,
				 uni->prios_reassign,
				 ARRAY_SIZE(uni->prios_reassign));
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_dev_db_put, ret);

	uni_ext_exit(uni, ctx, ifname, lport);

	dbg_out();
}

static enum pon_adapter_errno
setup_tc_to_queue_mappings(struct uni *uni, struct pon_net_context *ctx,
			   char *ifname, char *ifname_mc, char *ifname_bc,
			   uint16_t me_id, uint16_t lport)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char *dev = ifname;
	char tmp[IF_NAMESIZE] = "";
	char *indev = tmp, *indev_mc = tmp, *indev_bc = tmp;
	uint16_t tc = 0;
	uint16_t queue = 0;
	uint32_t class = PON_ROOT_QDISC;
	int idx;

	dbg_in_args("%p, %p, \"%s\", %u, %u", uni, ctx, ifname, me_id, lport);

	if (pon_net_is_ext_uni(ctx, lport)) {
		dev = pon_net_ext_uni_master_get(ctx, lport);
		indev = ifname;
		indev_mc = ifname_mc;
		indev_bc = ifname_bc;
		class = PON_QDISC_HANDLE(PON_QDISC_TS, me_id);
	}

	for (tc = 0; tc < PON_TRAFFIC_CLASS_COUNT; ++tc) {
		queue = pon_net_map_tc_to_queue(tc);

		idx = tc;
		ret = netlink_queue_assign(ctx->netlink, dev,
					   class, queue,
					   indev, uni->handles[idx],
					   (int)tc, NETLINK_FILTER_UNUSED,
					   (uint16_t)uni->prios[idx]);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_queue_assign, ret);
			return ret;
		}

		/* Setup Multicast and Broadcast queue mappings only for
		 * external switch configuration. For standard configuration
		 * replication is currently done by PON QOS driver.
		 */
		if (!pon_net_is_ext_uni(ctx, lport))
			continue;

		idx += PON_TRAFFIC_CLASS_COUNT;
		ret = netlink_queue_assign(ctx->netlink, dev,
					   class, queue,
					   indev_mc, uni->handles[idx],
					   (int)tc, NETLINK_FILTER_UNUSED,
					   (uint16_t)uni->prios[idx]);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_queue_assign, ret);
			return ret;
		}

		idx += PON_TRAFFIC_CLASS_COUNT;
		ret = netlink_queue_assign(ctx->netlink, dev,
					   class, queue,
					   indev_bc, uni->handles[idx],
					   (int)tc, NETLINK_FILTER_UNUSED,
					   (uint16_t)uni->prios[idx]);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_queue_assign, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static struct mapper *mapper_for_uni(struct pon_net_context *ctx,
				     uint16_t class_id)
{
	switch (class_id) {
	case PON_CLASS_ID_PPTP_ETHERNET_UNI:
		return ctx->mapper[MAPPER_PPTPETHERNETUNI_MEID_TO_IDX];
	case PON_CLASS_ID_VEIP:
		return ctx->mapper[MAPPER_VEIP_MEID_TO_IDX];
	default:
		return NULL;
	}
}

static uint8_t mc_type_for_uni(uint16_t class_id)
{
	switch (class_id) {
	case PON_CLASS_ID_PPTP_ETHERNET_UNI:
		return UNI_PPTP_MC;
	case PON_CLASS_ID_VEIP:
		return UNI_VEIP_MC;
	default:
		return 0;
	}
}

static uint8_t bc_type_for_uni(uint16_t class_id)
{
	switch (class_id) {
	case PON_CLASS_ID_PPTP_ETHERNET_UNI:
		return UNI_PPTP_BC;
	case PON_CLASS_ID_VEIP:
		return UNI_VEIP_BC;
	default:
		return 0;
	}
}

static int is_upstream(uint16_t me_id)
{
	return !!(me_id & 0x8000);
}

static enum pon_adapter_errno init_color_marking(struct pon_net_context *ctx,
						 uint16_t class_id,
						 uint16_t me_id)
{
	enum pon_adapter_errno ret;
	struct pon_net_me_list_item *item;
	uint8_t color_marking = 0;

	dbg_in_args("%p, %u, %u", ctx, class_id, me_id);

	pon_net_me_list_foreach_class(&ctx->me_list,
				      PON_CLASS_ID_PRIORITY_QUEUE,
				      item) {
		struct pon_net_priority_queue *priority_queue = item->data;

		if (!is_upstream(item->me_id))
			continue;

		color_marking = priority_queue->drop_precedence_color_marking;

		break;
	}

	ret = pon_net_color_marking_set(ctx, class_id, me_id, color_marking);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_color_marking_set, ret);
		return PON_ADAPTER_ERROR;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno create_dev_db_entries(struct pon_net_dev_db *db,
						    const char *ifname,
						    const char *ifname_mc,
						    const char *ifname_bc)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %p, %p", db, ifname, ifname_mc, ifname_bc);

	ret = pon_net_dev_db_add(db, ifname, &pon_net_uni_def);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_add, ret);
		return ret;
	}

	ret = pon_net_dev_db_add(db, ifname_mc, &pon_net_uni_mc_def);
	if (ret != PON_ADAPTER_SUCCESS) {
		pon_net_dev_db_del(db, ifname);
		FN_ERR_RET(ret, pon_net_dev_db_add, ret);
		return ret;
	}

	ret = pon_net_dev_db_add(db, ifname_bc, &pon_net_uni_bc_def);
	if (ret != PON_ADAPTER_SUCCESS) {
		pon_net_dev_db_del(db, ifname);
		pon_net_dev_db_del(db, ifname_mc);
		FN_ERR_RET(ret, pon_net_dev_db_add, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static void destroy_dev_db_entries(struct pon_net_dev_db *db,
				   const char *ifname, const char *ifname_mc,
				   const char *ifname_bc)
{
	dbg_in_args("%p, %p, %p, %p", db, ifname, ifname_mc, ifname_bc);

	pon_net_dev_db_del(db, ifname);
	pon_net_dev_db_del(db, ifname_mc);
	pon_net_dev_db_del(db, ifname_bc);

	dbg_out();
}

enum pon_adapter_errno pon_net_uni_create(struct pon_net_context *ctx,
					  uint16_t class_id, uint16_t me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE];
	char ifname_mc[IF_NAMESIZE];
	char ifname_bc[IF_NAMESIZE];
	uint16_t lport;
	struct uni uni;
	struct mapper *mapper = NULL;
	uint8_t mc_type = 0;
	uint8_t bc_type = 0;
	const struct pon_net_tree_builder *builder;
	uint8_t if_mac[ETH_ALEN] = {0,};

	dbg_in_args("%p, %u, %u", ctx, class_id, me_id);

	mapper = mapper_for_uni(ctx, class_id);
	if (!mapper) {
		dbg_err_fn_ret(mapper_for_uni, 0);
		goto err;
	}

	mc_type = mc_type_for_uni(class_id);
	if (!mc_type) {
		dbg_err_fn_ret(mc_type_for_uni, ret);
		goto err;
	}

	bc_type = bc_type_for_uni(class_id);
	if (!bc_type) {
		dbg_err_fn_ret(bc_type_for_uni, ret);
		goto err;
	}

	ret = pon_net_uni_lport_get(me_id, &lport);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_uni_lport_get, ret);
		goto err;
	}

	ret = mapper_explicit_map(mapper, me_id, lport);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(mapper_explicit_map, ret);
		goto err;
	}

	ret = pon_net_ifname_get(ctx, class_id, me_id, ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_ifname_get, ret);
		goto err_cleanup_mapper;
	}

	ret = pon_net_uni_ifname_get(ctx, mc_type, lport, ifname_mc,
				     sizeof(ifname_mc));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_uni_ifname_get, ret);
		goto err_cleanup_mapper;
	}

	ret = pon_net_uni_ifname_get(ctx, bc_type, lport, ifname_bc,
				     sizeof(ifname_bc));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_uni_ifname_get, ret);
		goto err_cleanup_mapper;
	}

	ret = create_dev_db_entries(ctx->db, ifname, ifname_mc, ifname_bc);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(create_dev_db_entries, ret);
		goto err_cleanup_mapper;
	}

	/* Changing the MAC address is only allowed when the interface is down.
	 * The OMCI protocol anyway expects that the UNI interface is down.
	 * ifname is put into up state by the unlock function the others when
	 * they are added to the multicast bridges.
	 */
	ret = netlink_netdevice_state_set(ctx->netlink, ifname, 0);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_netdevice_state_set, ret);
		goto err_cleanup_dev_db;
	}
	ret = netlink_netdevice_state_set(ctx->netlink, ifname_mc, 0);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_netdevice_state_set, ret);
		goto err_cleanup_dev_db;
	}
	ret = netlink_netdevice_state_set(ctx->netlink, ifname_bc, 0);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_netdevice_state_set, ret);
		goto err_cleanup_dev_db;
	}

	/* Set MAC address if it was specified in the configuration. If nothing
	 * was specified ignore it and use the (random) default one.
	 */
	ret = pon_net_macaddr_get(ctx, class_id, lport, if_mac);
	if (ret != PON_ADAPTER_SUCCESS && ret != PON_ADAPTER_ERR_NO_DATA) {
		dbg_err_fn_ret(pon_net_macaddr_get, ret);
		goto err_cleanup_dev_db;
	} else if (ret == PON_ADAPTER_SUCCESS) {
		ret = netlink_netdevice_mac_addr_set(ctx->netlink, ifname,
						     if_mac);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(netlink_netdevice_mac_addr_set, ret);
			goto err_cleanup_dev_db;
		}

		ret = netlink_netdevice_mac_addr_set(ctx->netlink, ifname_mc,
						     if_mac);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(netlink_netdevice_mac_addr_set, ret);
			goto err_cleanup_dev_db;
		}

		ret = netlink_netdevice_mac_addr_set(ctx->netlink, ifname_bc,
						     if_mac);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(netlink_netdevice_mac_addr_set, ret);
			goto err_cleanup_dev_db;
		}
	}

	/* The OLT cannot configure a specific MTU on a VEIP, this only works
	 * on PPTP UNI. Set the maximum value we support here.
	 */
	if (class_id == PON_CLASS_ID_VEIP) {
		ret = netlink_netdevice_mtu_set(ctx->netlink, ifname,
						JUMBO_FRAME_MTU);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(netlink_netdevice_mtu_set, ret);
			goto err_cleanup_dev_db;
		}
	}

	ret = netlink_qdisc_destroy(ctx->netlink, ifname,
						PON_ROOT_QDISC, 0, 0);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_qdisc_destroy, ret);
		goto err_cleanup_dev_db;
	}

	ret = netlink_qdisc_destroy(ctx->netlink, ifname_mc,
						PON_ROOT_QDISC, 0, 0);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_qdisc_destroy, ret);
		goto err_cleanup_dev_db;
	}

	ret = netlink_qdisc_destroy(ctx->netlink, ifname_bc,
						PON_ROOT_QDISC, 0, 0);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_qdisc_destroy, ret);
		goto err_cleanup_dev_db;
	}

	ret = netlink_qdisc_clsact_destroy(ctx->netlink, ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_qdisc_clsact_destroy, ret);
		goto err_cleanup_dev_db;
	}

	ret = netlink_qdisc_clsact_destroy(ctx->netlink, ifname_mc);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_qdisc_clsact_destroy, ret);
		goto err_cleanup_dev_db;
	}

	ret = netlink_qdisc_clsact_destroy(ctx->netlink, ifname_bc);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_qdisc_clsact_destroy, ret);
		goto err_cleanup_dev_db;
	}

	ret = netlink_qdisc_clsact_create(ctx->netlink, ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_qdisc_clsact_create, ret);
		goto err_cleanup_dev_db;
	}

	ret = netlink_qdisc_clsact_create(ctx->netlink, ifname_mc);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_qdisc_clsact_create, ret);
		goto err_cleanup_clsact;
	}

	ret = netlink_qdisc_clsact_create(ctx->netlink, ifname_bc);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_qdisc_clsact_create, ret);
		goto err_cleanup_clsact_mc;
	}

	ret = uni_init(&uni, ctx, ifname, lport);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(uni_init, ret);
		goto err_cleanup_clsact_bc;
	}

	if (ctx->cfg.no_ds_prio_queues)
		builder = NULL;
	else if (pon_net_is_ext_uni(ctx, lport))
		builder = pon_net_get_ext_uni_ds_qdisc_tree_builder(ctx);
	else
		builder = pon_net_get_default_qdisc_tree_builder(ctx);

	ret = pon_net_qdiscs_build_and_diff_apply(uni.qdiscs, class_id, me_id,
						  builder, true, ctx);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_qdiscs_build_and_diff_apply, ret);
		goto err_cleanup_uni;
	}

	ret = pon_net_me_list_write(&ctx->me_list, class_id, me_id, &uni,
				    sizeof(uni));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_me_list_write, ret);
		goto err_cleanup_qdiscs;
	}

	if (!ctx->cfg.no_ds_prio_queues) {
		ret = setup_tc_to_queue_mappings(&uni, ctx, ifname, ifname_mc,
						 ifname_bc, me_id, lport);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(setup_tc_to_queue_mappings, ret);
			goto err_cleanup_me_list;
		}
	}

	if (class_id == PON_CLASS_ID_PPTP_ETHERNET_UNI) {
		ret = pon_net_ethtool_pause_frames_tx_disable(ctx, ifname);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(pon_net_ethtool_pause_frames_tx_disable,
				       ret);
			goto err_cleanup_me_list;
		}
	}

	/* Select class mode for SPCP and DSCP classification */
	ret = pon_net_ethtool_priv_flag_set(ctx, ifname,
					    "qos_class_select_spcp_dscp",
					    true);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_ethtool_priv_flag_set,
			       ret);
		goto err_cleanup_me_list;
	}

	/* Remove CPU port from eth0_x map */
	ret = pon_net_ethtool_priv_flag_set(ctx, ifname, "bp_to_cpu_enable",
					    false);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_ethtool_priv_flag_set, ret);
		goto err_cleanup_me_list;
	}

	if (!ctx->cfg.no_ds_prio_queues) {
		ret = init_color_marking(ctx, class_id, me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(init_color_marking, ret);
			goto err_cleanup_me_list;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;

err_cleanup_me_list:
	pon_net_me_list_remove(&ctx->me_list, class_id, me_id);
err_cleanup_qdiscs:
	pon_net_qdiscs_build_and_diff_apply(uni.qdiscs, class_id, me_id, NULL,
					    true, ctx);
err_cleanup_uni:
	uni_exit(&uni, ctx, ifname, lport);
err_cleanup_clsact_bc:
	netlink_qdisc_clsact_destroy(ctx->netlink, ifname_bc);
err_cleanup_clsact_mc:
	netlink_qdisc_clsact_destroy(ctx->netlink, ifname_mc);
err_cleanup_clsact:
	netlink_qdisc_clsact_destroy(ctx->netlink, ifname);
err_cleanup_dev_db:
	destroy_dev_db_entries(ctx->db, ifname, ifname_mc, ifname_bc);
err_cleanup_mapper:
	mapper_id_remove(mapper, me_id);
err:
	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno pon_net_uni_destroy(struct pon_net_context *ctx,
					   uint16_t class_id, uint16_t me_id)
{
	uint16_t lport;
	uint32_t unused;
	char ifname[IF_NAMESIZE];
	char ifname_mc[IF_NAMESIZE];
	char ifname_bc[IF_NAMESIZE];
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct uni uni;
	struct mapper *mapper = NULL;
	uint8_t mc_type = 0;
	uint8_t bc_type = 0;

	dbg_in_args("%p, %u, %u", ctx, class_id, me_id);

	mapper = mapper_for_uni(ctx, class_id);
	if (!mapper) {
		FN_ERR_RET(ret, mapper_for_uni, ret);
		return ret;
	}

	mc_type = mc_type_for_uni(class_id);
	if (!mc_type) {
		FN_ERR_RET(mc_type, mc_type_for_uni, ret);
		return ret;
	}

	bc_type = bc_type_for_uni(class_id);
	if (!bc_type) {
		FN_ERR_RET(bc_type, bc_type_for_uni, ret);
		return ret;
	}

	ret = pon_net_uni_lport_get(me_id, &lport);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_uni_lport_get, ret);
		return ret;
	}

	ret = mapper_index_get(mapper, me_id, &unused);
	if (ret == PON_ADAPTER_SUCCESS) {
		ret = pon_net_ifname_get(ctx, class_id, me_id, ifname,
					 sizeof(ifname));
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_ifname_get, ret);
			return ret;
		}

		ret = pon_net_uni_ifname_get(ctx, mc_type, lport, ifname_mc,
					 sizeof(ifname_mc));
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_uni_ifname_get, ret);
			return ret;
		}

		ret = pon_net_uni_ifname_get(ctx, bc_type, lport, ifname_bc,
					 sizeof(ifname_bc));
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_uni_ifname_get, ret);
			return ret;
		}

		ret = pon_net_ethtool_priv_flag_set(ctx, ifname,
						    "bp_to_cpu_enable", true);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_ethtool_priv_flag_set, ret);
			return ret;
		}

		/* Remove all ingress VLAN flows before eth0_0 is deactivated */
		pon_net_vlan_flow_list_exit(ctx, &ctx->vlan_flow_list);

		ret = netlink_qdisc_clsact_destroy(ctx->netlink, ifname_bc);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_out_ret("%d", ret);
			return ret;
		}

		ret = netlink_qdisc_clsact_destroy(ctx->netlink, ifname_mc);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_out_ret("%d", ret);
			return ret;
		}

		ret = netlink_qdisc_clsact_destroy(ctx->netlink, ifname);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_out_ret("%d", ret);
			return ret;
		}

		ret = pon_net_me_list_read(&ctx->me_list, class_id, me_id, &uni,
					   sizeof(uni));
		if (ret == PON_ADAPTER_SUCCESS) {
			remove_tc_to_queue_mappings(ctx, &uni, lport);
			pon_net_qdiscs_build_and_diff_apply(uni.qdiscs,
							    class_id, me_id,
							    NULL, 1, ctx);
			uni_exit(&uni, ctx, ifname, lport);
			pon_net_me_list_remove(&ctx->me_list, class_id, me_id);
		}

		ret = mapper_id_remove(mapper, me_id);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_out_ret("%d", ret);
			return ret;
		}

		destroy_dev_db_entries(ctx->db, ifname, ifname_mc, ifname_bc);
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static const struct pon_net_tree_builder *
pon_net_uni_builder_get(struct pon_net_context *ctx,
			uint16_t class_id,
			uint16_t me_id)
{
	const struct pon_net_tree_builder *builder = NULL;
	uint16_t lport;
	enum pon_adapter_errno ret;

	builder = pon_net_get_default_qdisc_tree_builder(ctx);
	/* select special builder in case of ext. UNI */
	if (class_id == PON_CLASS_ID_PPTP_ETHERNET_UNI) {
		ret = pon_net_uni_lport_get(me_id, &lport);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(pon_net_uni_lport_get, ret);
			return NULL;
		}

		if (pon_net_is_ext_uni(ctx, lport) &&
		    ctx->cfg.no_ds_prio_queues == 0)
			builder =
				pon_net_get_ext_uni_ds_qdisc_tree_builder(ctx);
	}

	return builder;
}

enum pon_adapter_errno pon_net_uni_qdiscs_update(struct pon_net_context *ctx,
						 uint16_t class_id,
						 const uint16_t *me_ids,
						 unsigned int len)
{
	enum pon_adapter_errno ret;
	struct uni *uni;
	struct pon_net_me_list_item *item;
	const struct pon_net_tree_builder *builder = NULL;

	dbg_in_args("%p, %p, %u", ctx, me_ids, len);

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		uni = item->data;

		if (me_ids && !pon_net_me_id_in_array(item->me_id, me_ids, len))
			continue;

		builder = pon_net_uni_builder_get(ctx, class_id, item->me_id);
		if (!builder)
			return PON_ADAPTER_ERR_PTR_INVALID;

		ret = pon_net_qdiscs_build_and_diff_apply(uni->qdiscs,
							  class_id,
							  item->me_id,
							  builder, 1, ctx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_qdiscs_build_and_diff_apply,
				   ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno pon_net_uni_qdiscs_cleanup(struct pon_net_context *ctx,
						  uint16_t class_id,
						  const uint16_t *me_ids,
						  unsigned int len)
{
	enum pon_adapter_errno ret;
	struct uni *uni;
	struct pon_net_me_list_item *item;

	dbg_in_args("%p, %p, %u", ctx, me_ids, len);

	pon_net_me_list_foreach_class(&ctx->me_list, class_id, item) {
		uni = item->data;

		if (me_ids && !pon_net_me_id_in_array(item->me_id, me_ids, len))
			continue;

		ret = pon_net_qdiscs_build_and_diff_apply(uni->qdiscs,
							  class_id,
							  item->me_id,
							  NULL,
							  1, ctx);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, pon_net_qdiscs_build_and_diff_apply,
				   ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_net_uni_me_id_by_lport_get(struct pon_net_context *ctx,
			       const uint16_t class_id, uint8_t lport,
			       uint16_t *me_id)
{
	enum pon_adapter_errno ret;
	struct mapper *mapper = NULL;
	uint32_t tmp;

	dbg_in_args("%p, %u, %u, %p", ctx, class_id, lport, me_id);

	mapper = mapper_for_uni(ctx, class_id);
	if (!mapper) {
		FN_ERR_RET(0, mapper_for_uni, PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	ret = mapper_id_get(mapper, &tmp, lport);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	if (me_id)
		*me_id = (uint16_t)tmp;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Create egress TC filter for external switch */
static enum pon_adapter_errno
uni_reassign_eg_filter_create(struct pon_net_context *ctx,
			      uint32_t handle, uint32_t prio,
			      const char *ifname, int tc,
			      int proto, int vlan_proto)
{
	enum pon_adapter_errno ret;
	struct netlink_filter filter;
	struct netlink_skbedit_data skbedit;

	dbg_in_args("%p, %u, %u, \"%s\", %d, %d, %d", ctx, handle, prio, ifname,
		    tc, proto, vlan_proto);

	netlink_filter_defaults(&filter);

	filter.handle = handle;
	filter.prio = (int)prio;
	filter.proto = proto;
	if (proto == ETH_P_8021Q ||
	    proto == ETH_P_8021AD) {
		filter.vlan_proto = vlan_proto;
		filter.vlan_prio = tc;
	}
	filter.dir = NETLINK_FILTER_DIR_EGRESS;
	filter.act = NETLINK_FILTER_ACT_SKBEDIT;
	snprintf(filter.device, sizeof(filter.device), "%s", ifname);

	netlink_skbedit_defaults(&skbedit);
	skbedit.prio = tc;
	skbedit.action = TC_ACT_PIPE;
	snprintf(skbedit.cookie, sizeof(skbedit.cookie), "%s", ifname);
	filter.act_data = &skbedit;

	ret = netlink_filter_add(ctx->netlink, &filter);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_add, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static const uint16_t uni_reassign_protos[] = {
	ETH_P_8021Q,	/* single tagged */
	ETH_P_8021AD	/* double tagged */
};

static const int uni_reassign_vlan_protos[] = {
	NETLINK_FILTER_UNUSED,
	ETH_P_8021Q
};

/* Create reassignment egress TC filter for UNI interface */
static enum pon_adapter_errno
uni_reassign_egress_filters_create(struct pon_net_context *ctx,
				   struct uni *uni,
				   const char *ifname)
{
	uint8_t i, pref_cnt = 0;
	int tc;
	enum pon_adapter_errno ret;
	uint32_t *phandle = uni->handles_reassign;
	uint32_t *pref = uni->prios_reassign;
	const uint16_t *proto = uni_reassign_protos;
	const int *proto2 = uni_reassign_vlan_protos;

	/* Reassigment for every traffic class of
	 * single or double tagged packets.
	 */
	for (i = 0; i < ARRAY_SIZE(uni_reassign_protos);
	     i++, proto++, proto2++) {
		for (tc = 0; tc < PON_TRAFFIC_CLASS_COUNT &&
		     pref_cnt < PON_UNI_REASSIGN_CNT; tc++, pref_cnt++) {
			ret = uni_reassign_eg_filter_create(ctx, *phandle++,
							    *pref++, ifname,
							    tc, *proto,
							    *proto2);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, uni_reassign_eg_filter_create,
					   ret);
				return ret;
			}
		}
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static uint8_t uni_ext_id_get(struct pon_net_context *ctx,
			      uint16_t lport)
{
	return ctx->cfg.uni_ext_id[lport];
}

/* Create ingress TC filter for external switch */
static enum pon_adapter_errno
uni_ext_ingress_filter_create(struct pon_net_context *ctx,
			      uint32_t handle,
			      uint32_t prio,
			      const char *ifname,
			      const char *ifname2,
			      int vlan_id,
			      int proto,
			      int vlan_proto)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct netlink_filter filter = {0};
	struct netlink_mirred_data act_data = {0};
	struct netlink_vlan_data vlan_data;

	dbg_in_args("%p, %u, %u, \"%s\", \"%s\" %d, %d, %d", ctx, handle, prio,
		    ifname, ifname2, vlan_id, proto, vlan_proto);

	netlink_filter_defaults(&filter);

	filter.proto = proto;
	filter.handle = handle;
	filter.prio = (int)prio;
	filter.vlan_proto = vlan_proto;
	filter.vlan_id = vlan_id;

	snprintf(filter.device, sizeof(filter.device), "%s", ifname);
	snprintf(filter.indev, sizeof(filter.indev), "%s", ifname);
	snprintf(act_data.dev, sizeof(act_data.dev), "%s", ifname2);
	act_data.action = TCA_INGRESS_REDIR;
	act_data.policy = TC_ACT_PIPE;

	filter.dir = NETLINK_FILTER_DIR_INGRESS;
	filter.act = NETLINK_FILTER_ACT_MIRRED;
	filter.act_data = &act_data;
	filter.flags = NETLINK_HW_SW;

	netlink_vlan_defaults(&vlan_data);
	vlan_data.act_vlan = NETLINK_FILTER_ACT_VLAN_POP;
	act_data.vlan_act = &vlan_data;

	ret = netlink_filter_add(ctx->netlink, &filter);
	if (ret) {
		FN_ERR_RET(ret, netlink_filter_add, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static const uint16_t ext_ing_protos[] = {
	ETH_P_8021Q, ETH_P_8021AD,
	ETH_P_8021Q, ETH_P_8021AD, ETH_P_8021AD, ETH_P_8021Q
};
static const int ext_ing_vlan_protos[] = {
	NETLINK_FILTER_UNUSED, NETLINK_FILTER_UNUSED,
	ETH_P_8021Q, ETH_P_8021AD, ETH_P_8021Q, ETH_P_8021AD
};

static enum pon_adapter_errno
uni_ext_ingress_filters_create(struct pon_net_context *ctx,
			       const struct uni *uni,
			       const char *ifname,
			       const char *ifname2,
			       int vlan_id)
{
	int i;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, \"%s\", \"%s\" %d", ctx, uni, ifname, ifname2,
		    vlan_id);

	for (i = 0; i < PON_EXT_INGRESS_CNT; i++) {
		ret = uni_ext_ingress_filter_create(ctx,
						    uni->ext_handle_ingress[i],
						    uni->ext_prio_ingress[i],
						    ifname, ifname2, vlan_id,
						    ext_ing_protos[i],
						    ext_ing_vlan_protos[i]);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, uni_ext_ingress_filter_create, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/* Create egress TC filter for external switch */
static enum pon_adapter_errno
uni_ext_egress_filter_create(struct pon_net_context *ctx,
			     uint32_t handle,
			     uint32_t prio,
			     const char *ifname,
			     const char *ifname_cpy,
			     int vlan_id,
			     int vlan_prio,
			     int proto,
			     int vlan_proto)
{
	enum pon_adapter_errno ret;
	struct netlink_filter filter;
	struct netlink_vlan_data vlan_data;
	struct netlink_skbedit_data skbedit;

	dbg_in_args("%p, %u, %u, \"%s\", %d, %d, %d", ctx, handle, prio, ifname,
		    vlan_id, proto, vlan_proto);

	netlink_filter_defaults(&filter);

	filter.handle = handle;
	filter.prio = (int)prio;
	filter.proto = proto;
	filter.dir = NETLINK_FILTER_DIR_EGRESS;
	filter.act = NETLINK_FILTER_ACT_SKBEDIT;
	if (proto == ETH_P_8021Q ||
	    proto == ETH_P_8021AD) {
		filter.vlan_proto = vlan_proto;
		filter.vlan_prio = vlan_prio;
	}
	snprintf(filter.device, sizeof(filter.device), "%s", ifname);

	netlink_vlan_defaults(&vlan_data);
	vlan_data.vlan_id = vlan_id;
	vlan_data.vlan_prio = vlan_prio;
	vlan_data.act_vlan = NETLINK_FILTER_ACT_VLAN_PUSH;

	netlink_skbedit_defaults(&skbedit);
	skbedit.vlan_act = &vlan_data;
	skbedit.prio = vlan_prio;
	skbedit.action = TC_ACT_PIPE;
	if (ifname_cpy)
		snprintf(skbedit.cookie, sizeof(skbedit.cookie), "%s",
			 ifname_cpy);
	else
		snprintf(skbedit.cookie, sizeof(skbedit.cookie), "%s", ifname);
	filter.act_data = &skbedit;

	ret = netlink_filter_add(ctx->netlink, &filter);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_add, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static const uint16_t ext_eg_protos[] = {
	ETH_P_8021Q,	/* single tagged */
	ETH_P_8021AD};	/* double tagged */
static const int ext_eg_vlan_protos[] = {
	NETLINK_FILTER_UNUSED,
	ETH_P_8021Q};

static enum pon_adapter_errno
uni_ext_egress_filters_create(struct pon_net_context *ctx,
			      struct uni *uni,
			      const char *ifname,
			      const char *ifname_mc,
			      const char *ifname_bc,
			      int vlan_id)
{
	uint8_t i, pref_cnt = 0;
	int vlan_prio;
	enum pon_adapter_errno ret;
	uint32_t *phandle = uni->ext_handle_egress;
	uint32_t *pref = uni->ext_prio_egress;
	const uint16_t *proto = ext_eg_protos;
	const int *proto2 = ext_eg_vlan_protos;

	/* single/double tagged settings for every traffic class */
	for (i = 0; i < ARRAY_SIZE(ext_eg_protos); i++, proto++, proto2++) {
		for (vlan_prio = 0; vlan_prio < PON_TRAFFIC_CLASS_COUNT &&
		     pref_cnt < PON_EXT_EGRESS_CNT - 1; vlan_prio++,
		     pref_cnt++) {
			ret = uni_ext_egress_filter_create(ctx,
							   *phandle++,
							   *pref++,
							   ifname, NULL,
							   vlan_id, vlan_prio,
							   *proto,
							   *proto2);
			if (ret != PON_ADAPTER_SUCCESS) {
				FN_ERR_RET(ret, uni_ext_egress_filter_create,
					   ret);
				return ret;
			}
		}
	}

	/* untagged settings */
	ret = uni_ext_egress_filter_create(ctx, *phandle, *pref, ifname, NULL,
					   vlan_id, 0,
					   ETH_P_ALL,
					   NETLINK_FILTER_UNUSED);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, uni_ext_egress_filter_create, ret);
		return ret;
	}

	/* Reuse above rules on MC and BC interfaces by assigning the same CTP
	 * block to those interfaces. This is done to keep the number of rules
	 * as small as possible. A special handling is applied inside the
	 * QoS TC driver based on identical cookies.
	 */
	ret = uni_ext_egress_filter_create(ctx, *++phandle, *++pref, ifname_mc,
					   ifname, vlan_id, 0,
					   ETH_P_ALL,
					   NETLINK_FILTER_UNUSED);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, uni_ext_egress_filter_create, ret);
		return ret;
	}

	ret = uni_ext_egress_filter_create(ctx, *++phandle, *++pref, ifname_bc,
					   ifname, vlan_id, 0,
					   ETH_P_ALL,
					   NETLINK_FILTER_UNUSED);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, uni_ext_egress_filter_create, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno pon_net_uni_eg_tc_reassign(struct pon_net_context *ctx,
						  uint16_t class_id,
						  uint16_t me_id)
{
	char ifname[IF_NAMESIZE];
	uint16_t lport;
	struct uni uni;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %u", ctx, class_id, me_id);

	ret = pon_net_uni_lport_get(me_id, &lport);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_uni_lport_get, ret);
		return ret;
	}

	if (pon_net_is_ext_uni(ctx, lport)) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = pon_net_ifname_get(ctx, class_id, me_id,
				 ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_ifname_get, ret);
		return ret;
	}

	ret = pon_net_me_list_read(&ctx->me_list, class_id, me_id, &uni,
				   sizeof(uni));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	ret = uni_reassign_egress_filters_create(ctx, &uni, ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, uni_reassign_egress_filters_create, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_net_uni_eg_tc_reassign_clr(struct pon_net_context *ctx,
			       uint16_t class_id,
			       uint16_t me_id)
{
	char ifname[IF_NAMESIZE];
	struct uni uni;
	uint16_t lport;
	uint8_t i, j, pref = 0;
	enum pon_adapter_errno ret;
	uint32_t *prio, *phandle;

	dbg_in_args("%p, %u, %u", ctx, class_id, me_id);

	ret = pon_net_uni_lport_get(me_id, &lport);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_uni_lport_get, ret);
		return ret;
	}

	if (pon_net_is_ext_uni(ctx, lport)) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = pon_net_ifname_get(ctx, class_id, me_id,
				 ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_ifname_get, ret);
		return ret;
	}

	ret = pon_net_me_list_read(&ctx->me_list, class_id, me_id, &uni,
				   sizeof(uni));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	prio = uni.prios_reassign;
	phandle = uni.handles_reassign;

	for (i = 0; i < ARRAY_SIZE(uni_reassign_protos); i++) {
		for (j = 0; j < PON_TRAFFIC_CLASS_COUNT &&
		     pref < PON_UNI_REASSIGN_CNT; j++, pref++, prio++,
		     phandle++) {
			netlink_filter_clear_one(ctx->netlink, ifname,
						 ext_eg_protos[i],
						 (uint16_t)(*prio),
						 *phandle,
						 NETLINK_FILTER_DIR_EGRESS);
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_net_extern_uni_cfg(struct pon_net_context *ctx,
		       uint16_t class_id,
		       uint16_t me_id)
{
	char ifname[IF_NAMESIZE];
	char ifname_mc[IF_NAMESIZE];
	char ifname_bc[IF_NAMESIZE];
	char *ext_switch;
	uint8_t vlan_id;
	uint16_t lport;
	struct uni uni;
	enum pon_adapter_errno ret;
	uint8_t mc_type = 0;
	uint8_t bc_type = 0;

	dbg_in_args("%p, %u, %u", ctx, class_id, me_id);

	ret = pon_net_uni_lport_get(me_id, &lport);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_uni_lport_get, ret);
		return ret;
	}

	if (!pon_net_is_ext_uni(ctx, lport)) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = pon_net_ifname_get(ctx, class_id, me_id,
				 ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_ifname_get, ret);
		return ret;
	}

	mc_type = mc_type_for_uni(class_id);
	if (!mc_type) {
		dbg_err_fn_ret(mc_type_for_uni, ret);
		return ret;
	}

	bc_type = bc_type_for_uni(class_id);
	if (!bc_type) {
		dbg_err_fn_ret(bc_type_for_uni, ret);
		return ret;
	}

	ret = pon_net_uni_ifname_get(ctx, mc_type, lport, ifname_mc,
				     sizeof(ifname_mc));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_uni_ifname_get, ret);
		return ret;
	}

	ret = pon_net_uni_ifname_get(ctx, bc_type, lport, ifname_bc,
				     sizeof(ifname_bc));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_uni_ifname_get, ret);
		return ret;
	}

	vlan_id = uni_ext_id_get(ctx, lport);

	ret = pon_net_me_list_read(&ctx->me_list, class_id, me_id, &uni,
				   sizeof(uni));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	ret = uni_ext_egress_filters_create(ctx, &uni, ifname, ifname_mc,
					    ifname_bc, vlan_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, uni_ext_egress_filters_create, ret);
		return ret;
	}

	ext_switch = pon_net_ext_uni_master_get(ctx, lport);

	ret = uni_ext_ingress_filters_create(ctx, &uni, ext_switch, ifname,
					     vlan_id);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, uni_ext_ingress_filters_create, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_net_extern_uni_clr(struct pon_net_context *ctx,
		       uint16_t class_id,
		       uint16_t me_id)
{
	char ifname[IF_NAMESIZE];
	char ifname_mc[IF_NAMESIZE];
	char ifname_bc[IF_NAMESIZE];
	char *ext_switch;
	struct uni uni;
	uint16_t lport;
	uint8_t i, j, pref = 0;
	enum pon_adapter_errno ret;
	uint32_t *prio, *phandle;
	uint8_t mc_type = 0;
	uint8_t bc_type = 0;

	dbg_in_args("%p, %u, %u", ctx, class_id, me_id);

	ret = pon_net_uni_lport_get(me_id, &lport);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_uni_lport_get, ret);
		return ret;
	}

	if (!pon_net_is_ext_uni(ctx, lport)) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = pon_net_ifname_get(ctx, class_id, me_id,
				 ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_ifname_get, ret);
		return ret;
	}

	mc_type = mc_type_for_uni(class_id);
	if (!mc_type) {
		dbg_err_fn_ret(mc_type_for_uni, ret);
		return ret;
	}

	bc_type = bc_type_for_uni(class_id);
	if (!bc_type) {
		dbg_err_fn_ret(bc_type_for_uni, ret);
		return ret;
	}

	ret = pon_net_uni_ifname_get(ctx, mc_type, lport, ifname_mc,
				     sizeof(ifname_mc));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_uni_ifname_get, ret);
		return ret;
	}

	ret = pon_net_uni_ifname_get(ctx, bc_type, lport, ifname_bc,
				     sizeof(ifname_bc));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_uni_ifname_get, ret);
		return ret;
	}

	ret = pon_net_me_list_read(&ctx->me_list, class_id, me_id, &uni,
				   sizeof(uni));
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	/* replicated filters for MC and BC device have to be removed first */
	prio = &uni.ext_prio_egress[PON_EXT_EGRESS_CNT - 1];
	phandle = &uni.ext_handle_egress[PON_EXT_EGRESS_CNT - 1];

	netlink_filter_clear_one(ctx->netlink, ifname_bc,
				 ETH_P_ALL,
				 (uint16_t)(*prio),
				 *phandle,
				 NETLINK_FILTER_DIR_EGRESS);

	netlink_filter_clear_one(ctx->netlink, ifname_mc,
				 ETH_P_ALL,
				 (uint16_t)(*--prio),
				 *--phandle,
				 NETLINK_FILTER_DIR_EGRESS);

	prio = uni.ext_prio_egress;
	phandle = uni.ext_handle_egress;

	for (i = 0; i < ARRAY_SIZE(ext_eg_protos); i++) {
		for (j = 0; j < PON_TRAFFIC_CLASS_COUNT &&
		     pref < PON_EXT_EGRESS_CNT - 3; j++, pref++, prio++,
		     phandle++) {
			netlink_filter_clear_one(ctx->netlink, ifname,
						 ext_eg_protos[i],
						 (uint16_t)(*prio),
						 *phandle,
						 NETLINK_FILTER_DIR_EGRESS);
		}
	}

	netlink_filter_clear_one(ctx->netlink, ifname,
				 ETH_P_ALL,
				 (uint16_t)uni.ext_prio_egress[pref],
				 uni.ext_handle_egress[pref],
				 NETLINK_FILTER_DIR_EGRESS);

	ext_switch = pon_net_ext_uni_master_get(ctx, lport);

	for (i = 0; i < PON_EXT_INGRESS_CNT; i++) {
		netlink_filter_clear_one(ctx->netlink, ext_switch,
					 ext_ing_protos[i],
					 (uint16_t)uni.ext_prio_ingress[i],
					 uni.ext_handle_ingress[i],
					 NETLINK_FILTER_DIR_INGRESS);
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}
