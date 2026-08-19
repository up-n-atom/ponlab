/*****************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2019 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifdef LINUX
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif /* LINUX */

#include <netinet/icmp6.h>
#include <linux/if_ether.h>
#include <netlink/route/link.h>

#include <pon_adapter.h>
#include <pon_adapter_config.h>

#include "../pon_net_common.h"
#include "../pon_net_debug.h"
#include "../pon_net_netlink.h"
#include "../pon_net_dev_db.h"
#include "../pon_net_dev_defs.h"
#include "../pon_net_ethtool.h"
#include <omci/me/pon_adapter_pptp_lct_uni.h>

#if defined(LINUX) && !defined(PON_NET_LIB_SIMULATOR)

#define ARP_NDP_MOD_NAME "arp_ndp_reinsert"
#define KERNEL_MOD_PATH "/lib/modules/%s/%s.ko"

/* This loads the arp_ndp_reinsert.ko kernel module which will filter all ARP
 * and NDP packets on all bridges. We only add this when we configured the LCT
 * as the LCT configuration will add a PCE rule which traps such packets. To
 * still forward them we reinsert them again into the GSWIP. We do not want to
 * do this on Ethernet modes where the PCE rule is not installed.
 */
static enum pon_adapter_errno kmod_arp_ndp_reinsert_load(void)
{
	long sys_err;
	int err;
	int module_fd;
	struct utsname ver;
	char path[sizeof(KERNEL_MOD_PATH) + sizeof(ARP_NDP_MOD_NAME) +
		  sizeof(ver.release)];

	dbg_in();

	err = uname(&ver);
	if (err) {
		FN_ERR_RET(errno, uname, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	snprintf(path, sizeof(path), KERNEL_MOD_PATH, ver.release,
		 ARP_NDP_MOD_NAME);

	module_fd = open(path, O_RDONLY);
	if (module_fd == -1) {
		FN_ERR_RET(errno, open, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	sys_err = syscall(__NR_finit_module, module_fd, "", 0);
	if (sys_err && errno != EEXIST) {
		FN_ERR_RET(errno, finit_module, PON_ADAPTER_ERROR);
		close(module_fd);
		return PON_ADAPTER_ERROR;
	}
	close(module_fd);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Unload the arp_ndp_reinsert kernel module again */
static enum pon_adapter_errno kmod_arp_ndp_reinsert_unload(void)
{
	long sys_err;

	dbg_in();

	sys_err = syscall(__NR_delete_module, ARP_NDP_MOD_NAME, 0);
	if (sys_err) {
		FN_ERR_RET(errno, delete_module, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}
#else
static enum pon_adapter_errno kmod_arp_ndp_reinsert_load(void)
{
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno kmod_arp_ndp_reinsert_unload(void)
{
	return PON_ADAPTER_SUCCESS;
}
#endif /* LINUX */

static enum pon_adapter_errno map_handles(struct pon_net_context *ctx,
					  struct netlink_filter *filter,
					  unsigned int idx)
{
	enum pon_adapter_errno ret;
	uint32_t handle;
	uint32_t prio;

	dbg_in_args("%p, %p, %d", ctx, filter, idx);

	ret = pon_net_dev_db_map(ctx->db, filter->device,
				 PON_NET_HANDLE_LCT_INGRESS, idx, &handle);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_map, ret);
		return ret;
	}

	ret = pon_net_dev_db_map(ctx->db, filter->device,
				 PON_NET_PRIO_LCT_INGRESS, idx, &prio);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_map, ret);
		return ret;
	}

	filter->handle = handle;
	filter->prio = (int)prio;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno filter_add(struct pon_net_context *ctx,
					 struct netlink_filter *filter,
					 unsigned int idx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %u", ctx, filter, idx);

	ret = map_handles(ctx, filter, idx);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, map_handles, ret);
		return ret;
	}

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
					 struct netlink_filter *filter,
					 unsigned int idx)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %p, %u", ctx, filter, idx);

	ret = map_handles(ctx, filter, idx);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, map_handles, ret);
		return ret;
	}

	netlink_filter_del(ctx->netlink, filter);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Iterates through every trap filter for LCT */
static enum pon_adapter_errno
lct_trap_foreach(struct pon_net_context *ctx, char *ifname,
		 const uint8_t *eth_dst,
		 enum pon_adapter_errno (*fn)(struct pon_net_context *ctx,
					      struct netlink_filter *filter,
					      unsigned int idx))
{
	enum pon_adapter_errno ret;
	struct netlink_filter filter;
	unsigned int idx = 0;

	if (eth_dst)
		dbg_in_args("%p, \"%s\", %02x:%02x:%02x:%02x:%02x:%02x %p",
			    ctx, ifname, eth_dst[0], eth_dst[1],
			    eth_dst[2], eth_dst[3], eth_dst[4], eth_dst[5], fn);
	else
		dbg_in_args("%p, \"%s\", %p, %p", ctx, ifname, eth_dst, fn);

	/* Address Resolution Packet - ARP */
	ret = netlink_filter_arp(&filter, ifname, LCT_ARP_HW_TC, UNTAGGED,
				 NETLINK_FILTER_ACT_TRAP, NULL);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_arp, ret);
		return ret;
	}

	ret = fn(ctx, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, fn, ret);
		return ret;
	}

	ret = netlink_filter_arp(&filter, ifname, LCT_ARP_HW_TC, TAGGED,
				 NETLINK_FILTER_ACT_TRAP, NULL);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_arp, ret);
		return ret;
	}

	ret = fn(ctx, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, fn, ret);
		return ret;
	}

	/* IPv6 - trap only NDP packets*/
	ret = netlink_filter_icmpv6(&filter, ifname, IGMP_HW_TC,
				    ND_NEIGHBOR_SOLICIT, UNTAGGED,
				    NETLINK_FILTER_ACT_TRAP, NULL);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_icmpv6, ret);
		return ret;
	}

	ret = fn(ctx, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, fn, ret);
		return ret;
	}

	ret = netlink_filter_icmpv6(&filter, ifname, IGMP_HW_TC,
				    ND_NEIGHBOR_SOLICIT, TAGGED,
				    NETLINK_FILTER_ACT_TRAP, NULL);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_icmpv6, ret);
		return ret;
	}

	ret = fn(ctx, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, fn, ret);
		return ret;
	}

	ret = netlink_filter_icmpv6(&filter, ifname, IGMP_HW_TC,
				    ND_NEIGHBOR_ADVERT, UNTAGGED,
				    NETLINK_FILTER_ACT_TRAP, NULL);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_icmpv6, ret);
		return ret;
	}

	ret = fn(ctx, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, fn, ret);
		return ret;
	}

	ret = netlink_filter_icmpv6(&filter, ifname, IGMP_HW_TC,
				    ND_NEIGHBOR_ADVERT, TAGGED,
				    NETLINK_FILTER_ACT_TRAP, NULL);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_icmpv6, ret);
		return ret;
	}

	ret = fn(ctx, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, fn, ret);
		return ret;
	}

	/* IEEE 1588 Time sync - PTP */
	ret = netlink_filter_lct_trap_ptp(&filter, ifname, UNTAGGED);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_lct_trap_ptp, ret);
		return ret;
	}

	ret = fn(ctx, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, fn, ret);
		return ret;
	}

	ret = netlink_filter_lct_trap_ptp(&filter, ifname, TAGGED);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_lct_trap_ptp, ret);
		return ret;
	}

	ret = fn(ctx, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, fn, ret);
		return ret;
	}

	/* MAC address*/
	ret = netlink_filter_mac(&filter, ifname, NETLINK_FILTER_DIR_INGRESS,
				 LCT_MAC_HW_TC, ETH_P_ALL,
				 NETLINK_FILTER_UNUSED, eth_dst,
				 NETLINK_FILTER_ACT_TRAP, NULL);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_mac, ret);
		return ret;
	}

	ret = fn(ctx, &filter, idx++);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, fn, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

/* Applies trap filters */
static
enum pon_adapter_errno lct_trap_set(struct pon_net_context *ctx, char *ifname,
				    const uint8_t *eth_dst)
{
	enum pon_adapter_errno ret;

	if (eth_dst)
		dbg_in_args("%p, \"%s\", %02x:%02x:%02x:%02x:%02x:%02x",
			    ctx, ifname, eth_dst[0], eth_dst[1],
			    eth_dst[2], eth_dst[3], eth_dst[4], eth_dst[5]);
	else
		dbg_in_args("%p, \"%s\", %p", ctx, ifname, eth_dst);

	ret = lct_trap_foreach(ctx, ifname, eth_dst, filter_add);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, lct_trap_foreach, ret);
		return ret;
	}

	/* Remove CPU port from eth0_x_x_lct map */
	ret = pon_net_ethtool_priv_flag_set(ctx, ifname, "bp_to_cpu_enable",
					    false);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_ethtool_priv_flag_set, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static
void lct_trap_unset(struct pon_net_context *ctx, char *ifname)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, \"%s\"", ctx, ifname);

	/* Add CPU port to eth0_x_x_lct map */
	ret = pon_net_ethtool_priv_flag_set(ctx, ifname, "bp_to_cpu_enable",
					    true);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(pon_net_ethtool_priv_flag_set, ret);

	ret = lct_trap_foreach(ctx, ifname, NULL, filter_del);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_err_fn_ret(lct_trap_foreach, ret);

	dbg_out();
}

/* Currently we are doing the testing with the OpenWrt ubus method
 * internally, the non OpenWrt ubus version is not fully supported.
 */
static enum pon_adapter_errno
pptp_lct_uni_update_ubus(struct pon_net_context *ctx, uint16_t me_id,
			 const uint8_t state_admin)
{
	const char *method;
	char ifname[IF_NAMESIZE] = "";
	uint8_t mac_address[ETH_ALEN];
	enum pon_adapter_errno ret;
	uint16_t lport;
	int err;

	dbg_in_args("%p, %u, %u", ctx, me_id, state_admin);

	if (state_admin)
		method = "down";
	else
		method = "up";

	err = ctx->pa_config->ubus_call(ctx->hl_handle,
					"network.interface.lct",
					method, NULL, NULL, NULL,
					PON_UBUS_TIMEOUT);
	if (err) {
		FN_ERR_RET(err, ubus_call, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	ret = pon_net_uni_lport_get(me_id, &lport);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_uni_lport_get, ret);
		return ret;
	}

	ret = pon_net_pptp_lct_uni_name_get(ctx, lport, ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_pptp_lct_uni_name_get, ret);
		return ret;
	}

	ret = pon_net_get_mac_by_ifname(ctx, ifname, mac_address);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_get_mac_by_ifname, ret);
		return ret;
	}

	if (state_admin) {
		lct_trap_unset(ctx, ifname);
	} else {
		ret = lct_trap_set(ctx, ifname, mac_address);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, lct_trap_set, ret);
			return ret;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno
pptp_lct_uni_update_ifname(struct pon_net_context *ctx, uint16_t me_id,
			   const uint8_t state_admin)
{
	enum pon_adapter_errno ret;
	char ifname[IF_NAMESIZE] = "";
	uint16_t lport;
	uint8_t mac_address[ETH_ALEN];

	dbg_in_args("%p, %u, %u", ctx, me_id, state_admin);

	ret = pon_net_uni_lport_get(me_id, &lport);
	if (ret != PON_ADAPTER_SUCCESS)
		goto out;

	ret = pon_net_pptp_lct_uni_name_get(ctx, lport, ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS)
		goto out;

	/* Using !state_admin, as 0 means the interface should be enabled */
	ret = netlink_netdevice_state_set(ctx->netlink, ifname, !state_admin);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_netdevice_state_set, ret);
		return ret;
	}

	ret = pon_net_get_mac_by_ifname(ctx, ifname, mac_address);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_get_mac_by_ifname, ret);
		return ret;
	}

	if (state_admin) {
		lct_trap_unset(ctx, ifname);
	} else {
		ret = lct_trap_set(ctx, ifname, mac_address);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, lct_trap_set, ret);
			return ret;
		}
	}

out:
	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno pptp_lct_uni_update(void *ll_handle,
						  uint16_t me_id,
						  const uint8_t state_admin)
{
	struct pon_net_context *ctx = ll_handle;

	if (ctx->pa_config->ubus_call && ctx->netifd_running)
		return pptp_lct_uni_update_ubus(ctx, me_id, state_admin);
	return pptp_lct_uni_update_ifname(ctx, me_id, state_admin);
}

static enum pon_adapter_errno pptp_lct_uni_create(void *ll_handle,
						  uint16_t me_id)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret;
	char ifname[IF_NAMESIZE] = "";
	uint16_t lport;

	dbg_in_args("%p, %u", ll_handle, me_id);

	ret = pon_net_uni_lport_get(me_id, &lport);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_uni_lport_get, ret);
		return ret;
	}

	ret = pon_net_pptp_lct_uni_name_get(ctx, lport, ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_pptp_lct_uni_name_get, ret);
		return ret;
	}

	ret = netlink_qdisc_clsact_create(ctx->netlink, ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_qdisc_clsact_create, ret);
		return ret;
	}

	/* Load the arp_ndp_reinsert module when the first LCT is created */
	if (ctx->num_lct == 0) {
		ret = kmod_arp_ndp_reinsert_load();
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, kmod_arp_ndp_reinsert_unload, ret);
			return ret;
		}
	}
	ctx->num_lct++;

	ret = pon_net_dev_db_add(ctx->db, ifname, &pon_net_uni_def);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_add, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno pptp_lct_uni_destroy(void *ll_handle,
						   uint16_t me_id)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE] = "";
	uint16_t lport;

	dbg_in_args("%p, %u", ll_handle, me_id);

	/* Ignore return value as ubusd might be removed before shutdown. */
	if (ctx->pa_config->ubus_call && ctx->netifd_running)
		ctx->pa_config->ubus_call(ctx->hl_handle,
					  "network.interface.lct", "down",
					  NULL, NULL, NULL, PON_UBUS_TIMEOUT);

	ret = pon_net_uni_lport_get(me_id, &lport);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_uni_lport_get, ret);
		return ret;
	}

	ret = pon_net_pptp_lct_uni_name_get(ctx, lport, ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_pptp_lct_uni_name_get, ret);
		return ret;
	}

	if (ctx->num_lct > 0) {
		ctx->num_lct--;

		/* Unload the arp_ndp_reinsert module when the last LCT is
		 * destroyed */
		if (ctx->num_lct == 0) {
			ret = kmod_arp_ndp_reinsert_unload();
			if (ret != PON_ADAPTER_SUCCESS)
				dbg_err_fn_ret(kmod_arp_ndp_reinsert_unload,
					       ret);
		}
	}

	netlink_qdisc_clsact_destroy(ctx->netlink, ifname);
	if (pon_net_dev_db_get(ctx->db, ifname))
		lct_trap_unset(ctx, ifname);

	pon_net_dev_db_del(ctx->db, ifname);

	dbg_out_ret("%d", ret);
	return ret;
}

const struct pa_pptp_lct_uni_ops pptp_lct_uni_ops = {
	.update = pptp_lct_uni_update,
	.create = pptp_lct_uni_create,
	.destroy = pptp_lct_uni_destroy,
};
