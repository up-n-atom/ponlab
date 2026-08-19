/*****************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/**
 * This file is the common support for "IP Host Configuration Data",
 * "IPv6 Host Configuration Data" and "IP host performance monitoring data"
 */

/*
 * The implementation uses UCI and UBUS interfaces,
 * which are specific to OpenWrt.
 * For other systems an own implementation of "struct pa_ip_host_ops"
 * must be provided.
 */

#include <ctype.h>
#include <stdio.h>

#if defined(WIN32)
#include <WinSock2.h>
#include <ws2tcpip.h>
#endif

#include <netlink/route/act/mirred.h>

#include <pon_adapter.h>
#include <pon_adapter_config.h>
#include <omci/me/pon_adapter_ip_host.h>

#include "pon_net_config.h"
#include "pon_net_debug.h"
#include "pon_net_common.h"
#include "pon_net_extern.h"
#include "pon_net_dev_db.h"
#include "pon_net_dev_defs.h"

#ifdef LINUX
#include <unistd.h>
/* Needed for glibc compilation */
#include <fcntl.h>

#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/icmp6.h>
#endif

/**
 * List of fields in struct pon_net_ip_host, which hold allocated handles
 * and priorities from pon_net_dev_db.
 */
#define ALLOCATIONS \
	F(handles, PON_NET_HANDLE_FILTER_INGRESS, PON_MASTER_DEVICE) \
	F(prios, PON_NET_PRIO_MC_FLOW_FORWARD_INGRESS, PON_MASTER_DEVICE) \
	A(queue_handle, PON_NET_HANDLE_FILTER_INGRESS, PON_MASTER_DEVICE) \
	A(queue_prio, PON_NET_HANDLE_FILTER_INGRESS, PON_MASTER_DEVICE) \

/* Allocate TC filter handle and priorities from pon_net_dev_db */
static enum pon_adapter_errno
allocate_handles(struct pon_net_context *ctx,
		 struct pon_net_ip_host *cfg, const char *ifname)
{
	enum pon_adapter_errno ret;

	const struct pon_net_dev_db_alloc allocations[] = {
#define F(x, y, z) { cfg->x, ARRAY_SIZE(cfg->x), z, y },
#define A(x, y, z) { &cfg->x, 1, z, y },
		ALLOCATIONS
#undef A
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
			    struct pon_net_ip_host *cfg,
			    const char *ifname)
{
	const struct pon_net_dev_db_alloc allocations[] = {
#define F(x, y, z) { cfg->x, ARRAY_SIZE(cfg->x), z, y },
#define A(x, y, z) { &cfg->x, 1, z, y },
		ALLOCATIONS
#undef A
#undef F
	};

	dbg_in_args("%p, %p, \"%s\"", ctx, cfg, ifname);

	pon_net_dev_db_put_many(ctx->db, allocations, ARRAY_SIZE(allocations));

	dbg_out();
}

static enum pon_adapter_errno filter_add(struct pon_net_context *ctx,
					 struct pon_net_ip_host *ip_host,
					 struct netlink_filter *filter,
					 unsigned int idx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %u", ctx, filter, idx);

	filter->handle = ip_host->handles[idx];
	filter->prio = (int)ip_host->prios[idx];

	netlink_filter_del(ctx->netlink, filter);
	ret = netlink_filter_add(ctx->netlink, filter);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_add, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno filter_del(struct pon_net_context *ctx,
					 struct pon_net_ip_host *ip_host,
					 struct netlink_filter *filter,
					 unsigned int idx)
{
	dbg_in_args("%p, %p, %u", ctx, filter, idx);

	filter->handle = ip_host->handles[idx];
	filter->prio = (int)ip_host->prios[idx];

	netlink_filter_del(ctx->netlink, filter);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
ip_host_mirred_foreach(struct pon_net_context *ctx,
		       struct pon_net_ip_host *ip_host,
		       char *ifname,
		       const uint8_t *eth_dst,
		       enum pon_adapter_errno (*fn)(struct pon_net_context *ctx,
						    struct pon_net_ip_host *iph,
						    struct netlink_filter *flt,
						    unsigned int idx))
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct netlink_filter filter = {0};
	struct netlink_mirred_data mirred = {0};
	unsigned int idx = 0;
	int expected;

	dbg_in_args("%p, \"%s\", %p, %p", ctx, ifname, eth_dst, fn);

	expected = snprintf(mirred.dev, sizeof(mirred.dev), "%s", ifname);
	if (expected >= (int)sizeof(mirred.dev)) {
		FN_ERR_RET(expected, snprintf, PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	mirred.action = TCA_EGRESS_REDIR;

	/* We disable most of these rules for the time being, in order to
	 * save PCE rules. This may be enabled in future when needed */
#if 0
	ret = netlink_filter_arp(&filter, PON_MASTER_DEVICE, IPHOST_ARP_HW_TC,
				 UNTAGGED, NETLINK_FILTER_ACT_MIRRED, &mirred);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_arp, ret);
		return ret;
	}

	ret = fn(ctx, ip_host, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, fn, ret);
		return ret;
	}

	ret = netlink_filter_arp(&filter, PON_MASTER_DEVICE, IPHOST_ARP_HW_TC,
				 TAGGED, NETLINK_FILTER_ACT_MIRRED, &mirred);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_arp, ret);
		return ret;
	}

	ret = fn(ctx, ip_host, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, fn, ret);
		return ret;
	}

	ret =
	    netlink_filter_icmpv6(&filter, PON_MASTER_DEVICE, IPHOST_NDP_HW_TC,
				  ND_NEIGHBOR_SOLICIT, UNTAGGED,
				  NETLINK_FILTER_ACT_MIRRED, &mirred);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_icmpv6, ret);
		return ret;
	}

	ret = fn(ctx, ip_host, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, fn, ret);
		return ret;
	}

	ret = netlink_filter_icmpv6(&filter, PON_MASTER_DEVICE,
				    IPHOST_NDP_HW_TC, ND_NEIGHBOR_SOLICIT,
				    TAGGED, NETLINK_FILTER_ACT_MIRRED, &mirred);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_icmpv6, ret);
		return ret;
	}

	ret = fn(ctx, ip_host, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, fn, ret);
		return ret;
	}

	ret =
	    netlink_filter_icmpv6(&filter, PON_MASTER_DEVICE, IPHOST_NDP_HW_TC,
				  ND_NEIGHBOR_ADVERT, UNTAGGED,
				  NETLINK_FILTER_ACT_MIRRED, &mirred);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_icmpv6, ret);
		return ret;
	}

	ret = fn(ctx, ip_host, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, fn, ret);
		return ret;
	}

	ret = netlink_filter_icmpv6(&filter, PON_MASTER_DEVICE,
				    IPHOST_NDP_HW_TC, ND_NEIGHBOR_ADVERT,
				    TAGGED, NETLINK_FILTER_ACT_MIRRED, &mirred);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_icmpv6, ret);
		return ret;
	}

	ret = fn(ctx, ip_host, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, fn, ret);
		return ret;
	}
#endif

	ret = netlink_filter_mac(&filter, PON_MASTER_DEVICE,
				 NETLINK_FILTER_DIR_INGRESS, IPHOST_MAC_HW_TC,
				 ETH_P_ALL, NETLINK_FILTER_UNUSED, eth_dst,
				 NETLINK_FILTER_ACT_MIRRED, &mirred);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_lct_mac_trap, ret);
		return ret;
	}

	ret = fn(ctx, ip_host, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, fn, ret);
		return ret;
	}


	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
config_ifname_get(struct pon_net_context *ctx,
		  const struct pon_net_ip_host *ip_host, char *value,
		  unsigned int value_cap)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %p, %d", ctx, ip_host, value, value_cap);

#if !defined(PON_NET_LIB_SIMULATOR)
	if (!ctx->pa_config->get) {
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	ret = ctx->pa_config->get(ctx->hl_handle, "network", ip_host->cfg_name,
				  "ifname", value_cap, value);
#else
	(void)ip_host;
	(void)ctx;
	snprintf(value, value_cap, "%s", "iphost0");
	ret = PON_ADAPTER_SUCCESS;
#endif
	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
ip_host_create(void *ll_handle, uint16_t me_id, bool ipv6)
{
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_ip_host ip_host = { 0 };
	struct mapper *mapper;
	char value[IF_NAMESIZE];
	enum pon_adapter_errno ret;
	int len;

	dbg_in_args("%p, %u, %d", ll_handle, me_id, ipv6);

	/* Does this already exist? We use PON_CLASS_ID_IP_HOST for both,
	 * IPv4 and IPv6, as there is no overlap in me_id allowed.
	 */
	if (pon_net_me_list_exists(&ctx->me_list, PON_CLASS_ID_IP_HOST,
				   me_id)) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_RESOURCE_EXISTS);
		return PON_ADAPTER_ERR_RESOURCE_EXISTS;
	}

	ip_host.is_ipv6 = ipv6;
	mapper = ipv6 ? ctx->mapper[MAPPER_IPV6HOST_MEID_TO_IDX] :
			ctx->mapper[MAPPER_IPHOST_MEID_TO_IDX];

	ret = mapper_id_map(mapper, me_id, &ip_host.index);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, mapper_id_map, ret);
		return ret;
	}

	len = snprintf(ip_host.cfg_name, sizeof(ip_host.cfg_name), "ip%shost%u",
		       ipv6 ? "6" : "", ip_host.index);
	if (len >= (int)sizeof(ip_host.cfg_name)) {
		FN_ERR_RET(len, snprintf, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	ret = config_ifname_get(ctx, &ip_host, value, sizeof(value));
	/* Return error if section does not exist or is not correctly setup */
	if (ret != PON_ADAPTER_SUCCESS || strnlen(value, sizeof(value)) < 1) {
		ret = mapper_id_remove(mapper, me_id);
		if (ret != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(mapper_id_remove, ret);
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	len = snprintf(ip_host.ifname, sizeof(ip_host.ifname), "%s", value);
	if (len >= (int)sizeof(ip_host.ifname)) {
		FN_ERR_RET(len, snprintf, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	len = snprintf(ip_host.bp_ifname, sizeof(ip_host.bp_ifname), "%s_bp",
		       value);
	if (len >= (int)sizeof(ip_host.bp_ifname)) {
		FN_ERR_RET(len, snprintf, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	ret = pon_net_iphost_create(ctx, ip_host.bp_ifname, IPH_MASTER_DEVICE);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_iphost_create, ret);
		return ret;
	}

	ret = pon_net_iphost_create(ctx, ip_host.ifname, ip_host.bp_ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_iphost_create, ret);
		return ret;
	}

	ip_host.is_up = false;

	ret = allocate_handles(ctx, &ip_host, PON_MASTER_DEVICE);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, allocate_handles, ret);
		return ret;
	}

	ret = pon_net_me_list_write(&ctx->me_list, PON_CLASS_ID_IP_HOST,
				    me_id, &ip_host, sizeof(ip_host));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_write, ret);
		return ret;
	}

	/* TODO: Queue handling on URX to be adapted according to
	 * PONRTSYS-11630. This define has then to be removed.
	 */
#if !defined(PON_NET_LIB_IP_HOST_ON_URX)
	/* Assign the IP host bridge port interface to the IPHOST_MAC_HW_TC
	 * queue. Without this setting it would use the default queue to the
	 * CPU. This is equivalent to this tc command:
	 * tc filter add dev pon0 ingress handle 0x13f3 flower skip_sw \
	 *    indev iphost1_bp classid 1001:1 action ok cookie 0a
	 */
	ret = netlink_queue_assign(ctx->netlink, PON_MASTER_DEVICE,
				   PON_WRED_QDISC_0(1), 1,
				   ip_host.bp_ifname, ip_host.queue_handle,
				   NETLINK_FILTER_UNUSED,
				   IPHOST_MAC_HW_TC,
				   (uint16_t)ip_host.queue_prio);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_queue_assign, ret);
		return ret;
	}
#endif

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno ip_host_destroy(void *ll_handle, uint16_t me_id)
{
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_ip_host ip_host = { 0 };
	struct mapper *mapper;
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u", ll_handle, me_id);

	ret = pon_net_me_list_read(&ctx->me_list, PON_CLASS_ID_IP_HOST,
				   me_id, &ip_host, sizeof(ip_host));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_me_list_read, ret);
		return ret;
	}

	if (ip_host.is_up) {
		ret = ip_host_mirred_foreach(ctx, &ip_host, ip_host.bp_ifname,
					       NULL, filter_del);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, ip_host_mirred_foreach, ret);
			return ret;
		}
	}

	netlink_filter_clear_one(ctx->netlink, PON_MASTER_DEVICE,
				 ETH_P_ALL,
				 (uint16_t)ip_host.queue_prio,
				 ip_host.queue_handle,
				 NETLINK_FILTER_DIR_INGRESS);

	cleanup_handles(ctx, &ip_host, PON_MASTER_DEVICE);

	mapper = ip_host.is_ipv6 ? ctx->mapper[MAPPER_IPV6HOST_MEID_TO_IDX] :
				   ctx->mapper[MAPPER_IPHOST_MEID_TO_IDX];

	(void)pon_net_iphost_destroy(ctx, ip_host.ifname);
	(void)pon_net_iphost_destroy(ctx, ip_host.bp_ifname);
	(void)mapper_id_remove(mapper, me_id);
	pon_net_me_list_remove(&ctx->me_list, PON_CLASS_ID_IP_HOST, me_id);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* (IF_NAMESIZE - 1) "(" may not be accepted in format strings */
# define IFN_CHARS 15

/* these variables are automatically initialized to 0 */
static const struct pa_ip_host_address ip_any;
static const struct pa_ipv6_host_address ipv6_any;

static bool ipv4_is_any(const struct pa_ip_host_address *ip)
{
	return memcmp(ip, &ip_any, sizeof(ip_any)) == 0;
}

static bool ipv6_is_any(const struct pa_ipv6_host_address *ip)
{
	return memcmp(ip, &ipv6_any, sizeof(ipv6_any)) == 0;
}

static enum pon_adapter_errno
uci_set_ipv4(struct pon_net_context *ctx, const char *sec, const char *opt,
	     const struct pa_ip_host_address *ip, bool append)
{
	enum pon_adapter_errno ret;
	char ip_str[INET_ADDRSTRLEN] = "";

	dbg_in_args("%p %p %p %p %d", ctx, sec, opt, ip, append);

	if (!ctx->pa_config->set) {
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	if (!ipv4_is_any(ip)) {
		if (inet_ntop(AF_INET, ip,
			      ip_str, INET_ADDRSTRLEN) == NULL) {
			/* format failed*/
			dbg_out_ret("%d", PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}
	} else if (append) {
		/* for empty value no need to add to list */
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = ctx->pa_config->set(ctx->hl_handle, "network", sec, opt, ip_str,
				  append);
	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
uci_set_ipv6(struct pon_net_context *ctx, const char *sec, const char *opt,
	     const struct pa_ipv6_host_address *ip, bool append)
{
	enum pon_adapter_errno ret;
	char ip_str[INET6_ADDRSTRLEN] = "";

	dbg_in_args("%p %p %p %p %d", ctx, sec, opt, ip, append);

	if (!ctx->pa_config->set) {
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	if (!ipv6_is_any(ip)) {
		if (inet_ntop(AF_INET6, ip,
			      ip_str, INET6_ADDRSTRLEN) == NULL) {
			/* format failed*/
			dbg_out_ret("%d", PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}
	} else if (append) {
		/* for empty value no need to add to list */
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	ret = ctx->pa_config->set(ctx->hl_handle, "network", sec, opt, ip_str,
				  append);
	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
ip_host_mac_address_parse(const char *macaddr_buf, uint8_t mac[ETH_ALEN])
{
	unsigned int values[ETH_ALEN];
	int ret, i;
	char rest;

	/*
	 * Parse the Ethernet MAC address, if there is something remaining after
	 * the MAC address rest will get filled and this will return 7.
	 */
	ret = sscanf_s(macaddr_buf, "%x:%x:%x:%x:%x:%x%c",
		     &values[0], &values[1], &values[2],
		     &values[3], &values[4], &values[5], SSCANF_CHAR(&rest));
	if (ret != ETH_ALEN)
		return PON_ADAPTER_ERROR;

	for (i = 0; i < ETH_ALEN; i++)
		mac[i] = (uint8_t)values[i];

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
ip_host_mac_address_get_uci(struct pon_net_context *ctx,
			    struct pon_net_ip_host *ip_host,
			    uint8_t mac_address[ETH_ALEN])
{
	enum pon_adapter_errno err;
	char macaddr_buf[30];

	dbg_in_args("%p, %p", ip_host, mac_address);

	if (!ctx->pa_config->get) {
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	err = ctx->pa_config->get(ctx->hl_handle, "network", ip_host->cfg_name,
				  "macaddr", sizeof(macaddr_buf), macaddr_buf);
	if (err != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", err);
		return err;
	}

	err = ip_host_mac_address_parse(macaddr_buf, mac_address);
	dbg_out_ret("%d", err);
	return err;
}

static enum pon_adapter_errno
ip_host_mac_address_get(void *ll_handle, uint16_t me_id,
			uint8_t mac_address[ETH_ALEN])
{
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_ip_host ip_host = { 0 };
	enum pon_adapter_errno error;

	dbg_in_args("%p, %d, %p", ll_handle, me_id, mac_address);

	error = pon_net_me_list_read(&ctx->me_list, PON_CLASS_ID_IP_HOST,
				     me_id, &ip_host, sizeof(ip_host));
	if (error != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(error, pon_net_me_list_read, error);
		return error;
	}

	/* Read MAC address from UCI configuration and only fall back to the
	 * MAC address configured on the network interface in case of an error.
	 * Netifd only updates the MAC address on the interface after the
	 * IP host interface was brought up.
	 */
	error = ip_host_mac_address_get_uci(ctx, &ip_host, mac_address);
	if (error == PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return error;
	}

	error = pon_net_get_mac_by_ifname(ctx, ip_host.ifname, mac_address);
	if (error != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(error, pon_net_get_mac_by_ifname, error);
		return error;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
ip_host_platform_update(void *ll_handle, const struct pon_net_ip_host *ip_host,
			const struct pa_ip_host_update_data *update_data)
{
	struct pon_net_context *ctx = ll_handle;
	const char *cfg_name = ip_host->cfg_name;
	const char *proto, *method;
	char *path;
	enum pon_adapter_errno err;
	int error;

	dbg_in_args("%p, %p, %p", ll_handle, ip_host, update_data);

	if (!ctx->pa_config->ubus_call || !ctx->pa_config->set) {
		dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
		return PON_ADAPTER_SUCCESS;
	}

	if (update_data->ip_options & 1)
		proto = "dhcp";
	else
		proto = "static";

	err = ctx->pa_config->set(ctx->hl_handle, "network", cfg_name,
				  "clientid", update_data->ont_id, false);
	if (err != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(err, set, err);
		return err;
	}

	err = uci_set_ipv4(ctx, cfg_name, "ipaddr", &update_data->ip_address,
			   false);
	if (err != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(err, uci_set_ipv4, err);
		return err;
	}

	err = uci_set_ipv4(ctx, cfg_name, "netmask", &update_data->mask, false);
	if (err != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(err, uci_set_ipv4, err);
		return err;
	}

	err = uci_set_ipv4(ctx, cfg_name, "gateway", &update_data->gateway,
			   false);
	if (err != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(err, uci_set_ipv4, err);
		return err;
	}

	err = uci_set_ipv4(ctx, cfg_name, "dns", &update_data->primary_dns,
			   false);
	if (err != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(err, uci_set_ipv4, err);
		return err;
	}

	err = uci_set_ipv4(ctx, cfg_name, "dns", &update_data->secondary_dns,
			   true);
	if (err != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(err, uci_set_ipv4, err);
		return err;
	}

	err = ctx->pa_config->set(ctx->hl_handle, "network", cfg_name,
				  "proto", proto, false);
	if (err != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(err, set, err);
		return err;
	}

	error = ctx->pa_config->ubus_call(ctx->hl_handle, "network", "reload",
					  NULL, NULL, NULL, PON_UBUS_TIMEOUT);
	if (error) {
		FN_ERR_RET(error, ubus_call, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	if (update_data->ip_options > 1)
		method = "up";
	else
		method = "down";

	error = asprintf(&path, "%s.%s", "network.interface", cfg_name);
	if (error < 0) {
		FN_ERR_RET(error, asprintf, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
	error = ctx->pa_config->ubus_call(ctx->hl_handle, path, method, NULL,
					  NULL, NULL, PON_UBUS_TIMEOUT);
	free(path);
	if (error) {
		FN_ERR_RET(error, ubus_call, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
ip_host_update(void *ll_handle, uint16_t me_id,
	       const struct pa_ip_host_update_data *update_data)
{
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_ip_host ip_host = { 0 };
	enum pon_adapter_errno error;
	uint8_t mac_address[ETH_ALEN];

	dbg_in_args("%p, %u, %p", ll_handle, me_id, update_data);

	error = pon_net_me_list_read(&ctx->me_list, PON_CLASS_ID_IP_HOST,
				     me_id, &ip_host, sizeof(ip_host));
	if (error != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(error, pon_net_me_list_read, error);
		return error;
	}

	if (ctx->netifd_running) {
		error = ip_host_platform_update(ctx, &ip_host, update_data);
		if (error != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(error, ip_host_platform_update, error);
			return error;
		}
	}

	if (update_data->ip_options > 1)
		ip_host.is_up = true;
	else
		ip_host.is_up = false;

	error =
	    pon_net_get_mac_by_ifname(ctx, ip_host.ifname, mac_address);
	if (error != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(error, pon_net_get_mac_by_ifname, error);
		return error;
	}

	error = ip_host_mirred_foreach(ctx, &ip_host, ip_host.bp_ifname,
				       mac_address,
				       ip_host.is_up ? filter_add : filter_del);
	if (error != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(error, ip_host_mirred_foreach, error);
		return error;
	}

	error = pon_net_me_list_write(&ctx->me_list, PON_CLASS_ID_IP_HOST,
				    me_id, &ip_host, sizeof(ip_host));
	if (error != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(error, pon_net_me_list_write, error);
		return error;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
ip_host_update_v6(void *ll_handle, uint16_t me_id,
		  const struct pa_ipv6_host_update_data *update_data)
{
	struct pon_net_context *ctx = ll_handle;

	dbg_in_args("%p %u %p", ll_handle, me_id, update_data);

	if (0)
		if (uci_set_ipv6(ctx, "ip_host.cfg_name", "ip6addr",
				 &update_data->ipv6_addr, false)) {
			dbg_out_ret("%d", PON_ADAPTER_ERROR);
			return PON_ADAPTER_ERROR;
		}

	/** TODO: Add missing parts and return SUCCESS */
	dbg_out_ret("%d", PON_ADAPTER_ERR_NOT_SUPPORTED);
	return PON_ADAPTER_ERR_NOT_SUPPORTED;
}

#if !defined(PON_NET_LIB_SIMULATOR)
static enum pon_adapter_errno
ip_host_current_address_get(void *ll_handle, uint16_t me_id,
			    struct pa_ip_host_address *address)
{
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_ip_host ip_host = { 0 };
	enum pon_adapter_errno error;
	int fd;
	struct ifreq ifr;
	struct sockaddr_in *sa;
	int res;

	dbg_in_args("%p, %d, %p", ll_handle, me_id, address);

	error = pon_net_me_list_read(&ctx->me_list, PON_CLASS_ID_IP_HOST,
				     me_id, &ip_host, sizeof(ip_host));
	if (error != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(error, pon_net_me_list_read, error);
		return error;
	}

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		FN_ERR_RET(fd, socket, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
	ifr.ifr_addr.sa_family = AF_INET;
	snprintf(ifr.ifr_name, IF_NAMESIZE, "%s", ip_host.ifname);
	if (ioctl(fd, SIOCGIFADDR, &ifr) < 0) {
		if (errno != EADDRNOTAVAIL)
			FN_ERR_RET(errno, "SIOCGIFADDR", PON_ADAPTER_ERROR);
		close(fd);
		memset(address, 0, sizeof(*address));
		return PON_ADAPTER_ERROR;
	}
	sa = (struct sockaddr_in *)&ifr.ifr_addr;
	close(fd);
	res = memcpy_s(address, sizeof(*address), &sa->sin_addr.s_addr,
		       sizeof(sa->sin_addr.s_addr));
	if (res) {
		dbg_err_fn_ret(memcpy_s, res);
		return PON_ADAPTER_ERROR;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
ip_host_current_mask_get(void *ll_handle, uint16_t me_id,
			 struct pa_ip_host_address *mask)
{
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_ip_host ip_host = { 0 };
	enum pon_adapter_errno error;
	int fd;
	struct ifreq ifr;
	struct sockaddr_in *sa;
	int res;

	dbg_in_args("%p, %d, %p", ll_handle, me_id, mask);

	error = pon_net_me_list_read(&ctx->me_list, PON_CLASS_ID_IP_HOST,
				     me_id, &ip_host, sizeof(ip_host));
	if (error != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(error, pon_net_me_list_read, error);
		return error;
	}

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		FN_ERR_RET(fd, socket, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
	ifr.ifr_addr.sa_family = AF_INET;
	snprintf(ifr.ifr_name, IF_NAMESIZE, "%s", ip_host.ifname);
	if (ioctl(fd, SIOCGIFNETMASK, &ifr) < 0) {
		if (errno != EADDRNOTAVAIL)
			FN_ERR_RET(errno, "SIOCGIFNETMASK", PON_ADAPTER_ERROR);
		close(fd);
		memset(mask, 0, sizeof(*mask));
		return PON_ADAPTER_ERROR;
	}
	sa = (struct sockaddr_in *)&ifr.ifr_addr;
	close(fd);
	res = memcpy_s(mask, sizeof(*mask), &sa->sin_addr.s_addr,
		       sizeof(sa->sin_addr.s_addr));
	if (res) {
		dbg_err_fn_ret(memcpy_s, res);
		return PON_ADAPTER_ERROR;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
ip_host_current_gateway_get(void *ll_handle, uint16_t me_id,
			    struct pa_ip_host_address *gateway)
{
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_ip_host ip_host = { 0 };
	enum pon_adapter_errno error;
# define ADR_LEN 8
	FILE *f;
	char line[192], ifn[IF_NAMESIZE];
	uint32_t dest, gw;
	int res;
	char *line_ptr;

	dbg_in_args("%p, %d, %p", ll_handle, me_id, gateway);

	error = pon_net_me_list_read(&ctx->me_list, PON_CLASS_ID_IP_HOST,
				     me_id, &ip_host, sizeof(ip_host));
	if (error != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(error, pon_net_me_list_read, error);
		return error;
	}

	f = fopen("/proc/net/route", "r");
	if (f) {
		/* read header line */
		line_ptr = fgets(line, sizeof(line), f);
		/* If no route is configured this file will be empty and we
		 * return here. OMCI ignored the return code anyway
		 */
		if (!line_ptr) {
			fclose(f);
			dbg_wrn_fn(fgets);
			return PON_ADAPTER_ERROR;
		}
		while (fgets(line, sizeof(line), f)) {
			int ret = sscanf_s(line, "%" _MKSTR(IFN_CHARS) "s\t%"
					       _MKSTR(ADR_LEN) "x\t%"
					       _MKSTR(ADR_LEN) "x",
						SSCANF_STR(ifn, sizeof(ifn)),
						&dest, &gw);
			if (ret == 3 && strcmp(ip_host.ifname, ifn) == 0) {
				res = memcpy_s(gateway, sizeof(*gateway), &gw,
					       sizeof(*gateway));
				fclose(f);
				if (res) {
					dbg_err_fn_ret(memcpy_s, res);
					return PON_ADAPTER_ERROR;
				}
				dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
				return PON_ADAPTER_SUCCESS;
			}
		}
		fclose(f);
	}
	memset(gateway, 0, sizeof(*gateway));

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
ip_host_current_dns_get(void *ll_handle, uint16_t me_id,
			bool secondary, struct pa_ip_host_address *dns_addr)
{
	struct pon_net_context *ctx = ll_handle;
	struct pon_net_ip_host ip_host = { 0 };
	enum pon_adapter_errno error;
	FILE *f = NULL;
	char line[128], ifn[IF_NAMESIZE];
	uint8_t dns[4] = { 0 };
	int ret, dns_cnt;

	dbg_in_args("%p, %d, %p", ll_handle, me_id, dns_addr);

	error = pon_net_me_list_read(&ctx->me_list, PON_CLASS_ID_IP_HOST,
				     me_id, &ip_host, sizeof(ip_host));
	if (error != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(error, pon_net_me_list_read, error);
		return error;
	}

	f = fopen("/etc/resolv.conf", "r");
	if (f) {
		while (fgets(line, sizeof(line), f)) {
			ret = sscanf_s(line,
				     "# Interface %" _MKSTR(IFN_CHARS) "s",
				     SSCANF_STR(ifn, sizeof(ifn)));
			if (ret == 1 && strcmp(ip_host.ifname, ifn) == 0) {
				dns_cnt = 0;
				while (fgets(line, sizeof(line), f)) {
					ret = sscanf_s(line,
						     "nameserver %hhu.%hhu.%hhu.%hhu",
						     &dns[0], &dns[1],
						     &dns[2], &dns[3]);
					if (ret != 4)
						continue;

					dns_cnt++;

					if (!(dns_cnt == 1 && !secondary) &&
					    !(dns_cnt == 2 &&  secondary))
						continue;

					ret = memcpy_s(dns_addr,
							sizeof(*dns_addr), dns,
							sizeof(dns));
					fclose(f);
					if (ret) {
						dbg_err_fn_ret(memcpy_s, ret);
						return PON_ADAPTER_ERROR;
					}

					dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
					return PON_ADAPTER_SUCCESS;
				}
			}
		}
		fclose(f);
	}
	memset(dns_addr, 0, sizeof(*dns_addr));

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno system_name_get(const char *name,
					      uint8_t size, char *value)
{
	int fd, i;
	ssize_t len;

	fd = open(name, O_RDONLY);
	if (fd < 0)
		return PON_ADAPTER_ERROR;

	for (i = 0; i < size; i++) {
		len = read(fd, value + i, 1);
		if (len == 0 || !isprint(*(value + i))) {
			*(value + i) = '\0';
			break;
		}
	}
	close(fd);

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
ip_host_domain_name_get(void *ll_handle, uint16_t me_id,
			uint8_t size, char *domain)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p %u %u %p", ll_handle, me_id, size, domain);

	ret = system_name_get("/proc/sys/kernel/domainname", size, domain);
	if (ret) {
		FN_ERR_RET(ret, system_name_get, ret);
		return ret;
	}

	if (!strncmp("(none)", domain, 6))
		memset(domain, 0, size);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
ip_host_host_name_get(void *ll_handle, uint16_t me_id, uint8_t size, char *host)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p %u %u %p", ll_handle, me_id, size, host);

	ret = system_name_get("/proc/sys/kernel/hostname", size, host);
	if (ret) {
		FN_ERR_RET(ret, system_name_get, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pa_ip_host_ops ip_host_ops = {
	.create = ip_host_create,
	.update = ip_host_update,
	.update_v6 = ip_host_update_v6,
	.destroy = ip_host_destroy,
	.mac_address_get = ip_host_mac_address_get,
	.current_address_get = ip_host_current_address_get,
	.current_mask_get = ip_host_current_mask_get,
	.current_gateway_get = ip_host_current_gateway_get,
	.current_dns_get = ip_host_current_dns_get,
	.domain_name_get = ip_host_domain_name_get,
	.host_name_get = ip_host_host_name_get,
};

#else /* !defined(PON_NET_LIB_SIMULATOR) */

const struct pa_ip_host_ops ip_host_ops = {
	.create = ip_host_create,
	.update = ip_host_update,
	.update_v6 = ip_host_update_v6,
	.destroy = ip_host_destroy,
	.mac_address_get = ip_host_mac_address_get,
};
#endif
