/*****************************************************************************
 *
 * Copyright (c) 2020 - 2023 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <net/if.h>
#include <stdio.h>

#ifdef LINUX
#include <sys/ioctl.h>
#include <sys/socket.h>
#endif

#include <omci/me/pon_adapter_mac_bridge_port_config_data.h>
#include <omci/me/pon_adapter_gem_port_network_ctp.h>
#include <omci/me/pon_adapter_ext_vlan.h>
#include <omci/me/pon_adapter_traffic_descriptor.h>
#include <omci/me/pon_adapter_onu_g.h>
#include "pon_net_common.h"
#include "pon_net_netlink.h"
#include "pon_net_debug.h"
#include "pon_net_qdisc_build.h"
#include "pon_net_dev_db.h"
#include "pon_net_dev_defs.h"

#include "pon_net_config.h"
#include "pon_net_extern.h"
#include "me/pon_net_gem_interworking_tp.h"
#include "me/pon_net_gem_port_network_ctp.h"
#include "me/pon_net_mac_bridge_port_config_data.h"
#include "me/pon_net_multicast_gem_interworking_tp.h"
#include "me/pon_net_onu_g.h"

#ifdef EXTRA_VERSION
#define pon_net_lib_extra_ver_str "." EXTRA_VERSION
#else
#define pon_net_lib_extra_ver_str ""
#endif

#ifdef MCC_DRV_ENABLE
#include "pon_mcc.h"
#endif

/** what string support, version string */
const char pon_net_lib_whatversion[] = "@(#)MaxLinear PON Net Lib, version "
					PACKAGE_VERSION
					pon_net_lib_extra_ver_str;

static enum pon_adapter_errno pptp_eth_uni_format(struct pon_net_context *ctx,
						  uint16_t tp_ptr,
						  char *ifname,
						  size_t size)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint32_t id;
	int expected_size;

	dbg_in_args("%p, %u, %p, %zd", ctx, tp_ptr, ifname, size);
	ret = mapper_index_get(ctx->mapper[MAPPER_PPTPETHERNETUNI_MEID_TO_IDX],
			       tp_ptr, &id);

	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	if (!(id < LAN_PORT_MAX)) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	expected_size = snprintf(ifname, size, "%s", ctx->cfg.uni_name[id]);
	if (expected_size < 0 || (unsigned int)expected_size + 1 > size)
		return PON_ADAPTER_ERR_SIZE;

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno veip_format(struct pon_net_context *ctx,
					  uint16_t tp_ptr, char *ifname,
					  size_t size)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint32_t id;
	int expected_size;

	dbg_in_args("%p, %u, %p, %zd", ctx, tp_ptr, ifname, size);
	ret = mapper_index_get(ctx->mapper[MAPPER_VEIP_MEID_TO_IDX],
			       tp_ptr, &id);

	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	if (id >= VEIP_PORT_MAX) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	expected_size = snprintf(ifname, size, "%s", ctx->cfg.veip_name[id]);
	if (expected_size < 0 || (unsigned int)expected_size + 1 > size)
		return PON_ADAPTER_ERR_SIZE;

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno gem_format(struct pon_net_context *ctx,
					 uint16_t tp_ptr,
					 char *ifname,
					 size_t size)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint16_t id;
	int expected_size;

	dbg_in_args("%p, %u, %p, %zd", ctx, tp_ptr, ifname, size);

	ret = pon_net_gem_interworking_tp_ctp_get(ctx, tp_ptr, &id);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	expected_size = snprintf(ifname, size, PON_IFNAME_GEM, id);
	if (expected_size < 0 || (unsigned int)expected_size + 1 > size)
		return PON_ADAPTER_ERR_SIZE;

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno mc_gem_format(struct pon_net_context *ctx,
					    uint16_t tp_ptr,
					    char *ifname,
					    size_t size)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint16_t id;
	int expected_size;

	dbg_in_args("%p, %u, %p, %zd", ctx, tp_ptr, ifname, size);

	ret = pon_net_multicast_gem_interworking_tp_ctp_get(ctx, tp_ptr,
							    &id);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	expected_size = snprintf(ifname, size, PON_IFNAME_GEM, id);
	if (expected_size < 0 || (unsigned int)expected_size + 1 > size)
		return PON_ADAPTER_ERR_SIZE;

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno pmapper_format(struct pon_net_context *ctx,
					     uint16_t tp_ptr,
					     char *ifname,
					     size_t size)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint32_t id;
	int expected_size;

	dbg_in_args("%p, %u, %p, %zd", ctx, tp_ptr, ifname, size);
	ret = mapper_index_get(ctx->mapper[MAPPER_PMAPPER_MEID_TO_IDX],
			       tp_ptr, &id);

	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	expected_size = snprintf(ifname, size, PON_IFNAME_PMAPPER, id);
	if (expected_size < 0 || (unsigned int)expected_size + 1 > size)
		return PON_ADAPTER_ERR_SIZE;

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno iphost_format(struct pon_net_context *ctx,
					    uint16_t tp_ptr,
					    char *ifname,
					    size_t size)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	struct pon_net_ip_host ip_host = { 0 };
	int expected_size, error;

	dbg_in_args("%p, %u, %p, %zd", ctx, tp_ptr, ifname, size);

	error = pon_net_me_list_read(&ctx->me_list, PON_CLASS_ID_IP_HOST,
				     tp_ptr, &ip_host, sizeof(ip_host));
	if (error != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(error, pon_net_me_list_read, error);
		return error;
	}

	expected_size = snprintf(ifname, size, "%s", ip_host.bp_ifname);
	if (expected_size < 0 || (unsigned int)expected_size + 1 > size)
		return PON_ADAPTER_ERR_SIZE;

	dbg_out_ret("%d", ret);
	return ret;
}

typedef enum pon_adapter_errno (format_get_fn)(struct pon_net_context *ctx,
					       uint16_t tp_ptr,
					       char *ifname,
					       size_t size);

struct template {
	/* type */
	uint16_t type;
	/* Function converting TP Ptr to the name of the device */
	format_get_fn *format_get;
};

/* For given TP Type and TP Ptr it will return the network device name
   This is used to obtain network device name when creating filters */
static enum pon_adapter_errno ifname_get(const struct template *templates,
					 size_t templates_size,
					 struct pon_net_context *ctx,
					 uint16_t tp_type,
					 uint16_t tp_ptr,
					 char *ifname,
					 size_t size)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	unsigned int i;

	dbg_in_args("%p, %zd, %p, %u, %u, %p, %zd", templates, templates_size,
		    ctx, tp_type, tp_ptr, ifname, size);

	for (i = 0; i < templates_size; ++i) {
		if (templates[i].type == tp_type) {
			ret = templates[i].format_get(ctx, tp_ptr, ifname,
						      size);
			if (ret != PON_ADAPTER_SUCCESS) {
				dbg_out_ret("%d", ret);
				return ret;
			}
			dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
			return PON_ADAPTER_SUCCESS;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
	return PON_ADAPTER_ERR_INVALID_VAL;
}

static const struct template tp_templates[] = {
	{ PA_BP_TP_TYPE_GEM, gem_format},
	{ PA_BP_TP_TYPE_MC_GEM, mc_gem_format},
	{ PA_BP_TP_TYPE_PPTP_UNI, pptp_eth_uni_format},
	{ PA_BP_TP_TYPE_VEIP, veip_format},
	{ PA_BP_TP_TYPE_PMAP, pmapper_format},
	{ PA_BP_TP_TYPE_IP_HOST, iphost_format}
};

/* For given TP Type and TP Ptr it will return the network device name
   This is used to obtain network device name when creating filters */
enum pon_adapter_errno pon_net_tp_ifname_get(struct pon_net_context *ctx,
					     uint8_t tp_type,
					     uint16_t tp_ptr,
					     char *ifname,
					     size_t size)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %zd, %u, %u", ifname, size, tp_type, tp_ptr);

	ret = ifname_get(tp_templates,
			 ARRAY_SIZE(tp_templates),
			 ctx,
			 tp_type,
			 tp_ptr,
			 ifname,
			 size);

	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno pon_net_tp_state_set(struct pon_net_context *ctx,
					    uint8_t tp_type, uint16_t tp_ptr,
					    int state)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	char ifname[IF_NAMESIZE];

	dbg_in_args("%p, %u, %u, %d", ctx, tp_type, tp_ptr, state);

	ret = pon_net_tp_ifname_get(ctx, tp_type, tp_ptr,
				    ifname, sizeof(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_tp_ifname_get, ret);
		return ret;
	}

	ret = netlink_netdevice_state_set(ctx->netlink, ifname, state);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_netdevice_state_set, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno mac_bp_format(struct pon_net_context *ctx,
					    uint16_t ptr,
					    char *ifname,
					    size_t size)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint8_t tp_type;
	uint16_t tp_ptr;

	dbg_in_args("%p, %u, %p, %zd", ctx, ptr, ifname, size);

	ret = pon_net_tp_get(ctx, ptr, &tp_type, &tp_ptr);

	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	ret = pon_net_tp_ifname_get(ctx, tp_type, tp_ptr, ifname, size);
	dbg_out_ret("%d", ret);
	return ret;
}

static const struct template association_templates[] = {
	{ PA_EXT_VLAN_MAC_BP_CFG_DATA, mac_bp_format },
	{ PA_EXT_VLAN_IEEE_P_MAPPER, pmapper_format },
	{ PA_EXT_VLAN_PPTP_ETH_UNI, pptp_eth_uni_format },
	{ PA_EXT_VLAN_VEIP, veip_format },
	{ PA_EXT_VLAN_GEM_ITP, gem_format },
	{ PA_EXT_VLAN_IP_HOST_CONFIG_DATA, iphost_format },
	{ PA_EXT_VLAN_MC_GEM_ITP, mc_gem_format },
};

/* For given Association Type and Associated Ptr it will return the network
   device name. This is used to obtain network device name when creating
   filters */
enum pon_adapter_errno
pon_net_associated_ifname_get(struct pon_net_context *ctx,
			      uint16_t association_type,
			      uint16_t associated_ptr,
			      char *ifname,
			      size_t size)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %zd, %u, %u", ifname, size, association_type,
				       associated_ptr);

	ret = ifname_get(association_templates,
			 ARRAY_SIZE(association_templates),
			 ctx,
			 association_type,
			 associated_ptr,
			 ifname,
			 size);

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno tcont_format(struct pon_net_context *ctx,
					   uint16_t me_id, char *ifname,
					   size_t size)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int expected_size;

	dbg_in_args("%p, %u, %p, %zd", ctx, me_id, ifname, size);

	expected_size = snprintf(ifname, size, PON_IFNAME_TCONT, me_id);
	if (expected_size < 0 || (unsigned int)expected_size + 1 > size)
		return PON_ADAPTER_ERR_SIZE;

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno gem_ctp_format(struct pon_net_context *ctx,
					     uint16_t tp_ptr, char *ifname,
					     size_t size)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint32_t tmp;
	int expected_size;

	dbg_in_args("%p, %u, %p, %zd", ctx, tp_ptr, ifname, size);

	ret = mapper_index_get(ctx->mapper[MAPPER_GEMPORTCTP_MEID_TO_ID],
			       tp_ptr, &tmp);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	expected_size = snprintf(ifname, size, PON_IFNAME_GEM, tp_ptr);
	if (expected_size < 0 || (unsigned int)expected_size + 1 > size)
		return PON_ADAPTER_ERR_SIZE;

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno ani_g_format(struct pon_net_context *ctx,
					   uint16_t me_id, char *ifname,
					   size_t size)
{
	int expected_size;

	dbg_in_args("%p, %u, %p, %zd", ctx, me_id, ifname, size);

	expected_size = snprintf(ifname, size, "%s", PON_MASTER_DEVICE);
	if (expected_size < 0 || (unsigned int)expected_size + 1 > size) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_SIZE);
		return PON_ADAPTER_ERR_SIZE;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static const struct template templates[] = {
	{ PON_CLASS_ID_ANI_G, ani_g_format },
	{ PON_CLASS_ID_BRIDGE_PORT_CONFIG, mac_bp_format },
	{ PON_CLASS_ID_PMAPPER, pmapper_format },
	{ PON_CLASS_ID_PPTP_ETHERNET_UNI, pptp_eth_uni_format },
	{ PON_CLASS_ID_VEIP, veip_format },
	{ PON_CLASS_ID_TCONT, tcont_format },
	{ PON_CLASS_ID_GEM_PORT_NET_CTP_DATA, gem_ctp_format }
};

enum pon_adapter_errno pon_net_ifname_get(struct pon_net_context *ctx,
					  uint16_t class_id, uint16_t me_id,
					  char *ifname, size_t size)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %u, %u, %p, %zd", ctx, class_id, me_id, ifname, size);

	ret = ifname_get(templates, ARRAY_SIZE(templates), ctx, class_id, me_id,
			 ifname, size);

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno
pptp_uni_name_get(const char (*names)[IF_NAMESIZE],
		  unsigned int num_names,
		  uint32_t lport,
		  char *ifname,
		  size_t size,
		  const char *suffix)
{
	int expected_size;

	dbg_in_args("%p, %u, %u, %p, %zd, \"%s\"", names, num_names, lport,
		    ifname, size, suffix);

	if (!(lport < num_names)) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	expected_size = snprintf(ifname, size, "%s%s", names[lport], suffix);
	/* Check if the number of characters is greater than buffer size,
	   or if error occur */
	if (expected_size < 0 || (unsigned int)expected_size + 1 > size) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_SIZE);
		return PON_ADAPTER_ERR_SIZE;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno uni_pptp_eth_format(struct pon_net_context *ctx,
						  uint16_t lport,
						  char *ifname,
						  size_t size)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u, %p, %zd", ctx, lport, ifname, size);

	ret = pptp_uni_name_get(ctx->cfg.uni_name,
				ARRAY_SIZE(ctx->cfg.uni_name),
				lport, ifname, size, "");
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pptp_uni_name_get, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno uni_pptp_lct_format(struct pon_net_context *ctx,
						  uint16_t lport,
						  char *ifname,
						  size_t size)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u, %p, %zd", ctx, lport, ifname, size);

	ret = pptp_uni_name_get(ctx->cfg.uni_name,
				ARRAY_SIZE(ctx->cfg.uni_name),
				lport, ifname, size, PON_IFNAME_LCT_SUFFIX);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pptp_uni_name_get, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno uni_pptp_mc_format(struct pon_net_context *ctx,
						 uint16_t lport, char *ifname,
						 size_t size)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u, %p, %zd", ctx, lport, ifname, size);

	ret = pptp_uni_name_get(ctx->cfg.uni_mc_name,
				ARRAY_SIZE(ctx->cfg.uni_mc_name),
				lport, ifname, size, "");
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pptp_uni_name_get, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno uni_pptp_bc_format(struct pon_net_context *ctx,
						 uint16_t lport, char *ifname,
						 size_t size)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u, %p, %zd", ctx, lport, ifname, size);

	ret = pptp_uni_name_get(ctx->cfg.uni_bc_name,
				ARRAY_SIZE(ctx->cfg.uni_bc_name),
				lport, ifname, size, "");
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pptp_uni_name_get, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno uni_veip_format(struct pon_net_context *ctx,
					      uint16_t lport, char *ifname,
					      size_t size)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u, %p, %zd", ctx, lport, ifname, size);

	ret = pptp_uni_name_get(ctx->cfg.veip_name,
				ARRAY_SIZE(ctx->cfg.veip_name),
				lport, ifname, size, "");
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pptp_uni_name_get, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno uni_veip_mc_format(struct pon_net_context *ctx,
						 uint16_t lport, char *ifname,
						 size_t size)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u, %p, %zd", ctx, lport, ifname, size);

	ret = pptp_uni_name_get(ctx->cfg.veip_mc_name,
				ARRAY_SIZE(ctx->cfg.veip_mc_name),
				lport, ifname, size, "");
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pptp_uni_name_get, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno uni_veip_bc_format(struct pon_net_context *ctx,
						 uint16_t lport, char *ifname,
						 size_t size)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p, %u, %p, %zd", ctx, lport, ifname, size);

	ret = pptp_uni_name_get(ctx->cfg.veip_bc_name,
				ARRAY_SIZE(ctx->cfg.veip_bc_name),
				lport, ifname, size, "");
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pptp_uni_name_get, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

static const struct template uni_templates[] = {
	{ UNI_VEIP, uni_veip_format },
	{ UNI_PPTP_ETH, uni_pptp_eth_format },
	{ UNI_PPTP_LCT, uni_pptp_lct_format },
	{ UNI_PPTP_MC, uni_pptp_mc_format },
	{ UNI_VEIP_MC, uni_veip_mc_format },
	{ UNI_PPTP_BC, uni_pptp_bc_format },
	{ UNI_VEIP_BC, uni_veip_bc_format },
};

enum pon_adapter_errno pon_net_uni_ifname_get(struct pon_net_context *ctx,
					      uint8_t uni_type,
					      uint16_t lport,
					      char *ifname, size_t size)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, %zd, %u, %u", ifname, size, uni_type, lport);

	ret = ifname_get(uni_templates, ARRAY_SIZE(uni_templates), ctx,
			 uni_type, lport, ifname, size);

	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno
pon_net_bridge_ifname_get_by_idx(struct pon_net_context *ctx,
				 unsigned int bridge_idx,
				 char *ifname,
				 size_t size)
{
	int expected_size;

	dbg_in_args("%p, %u, %p, %zd", ctx, bridge_idx, ifname, size);

	expected_size = snprintf(ifname, size, PON_IFNAME_BRIDGE,
				 bridge_idx);
	if (expected_size < 0 || (unsigned int)expected_size + 1 > size)
		return PON_ADAPTER_ERR_SIZE;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_net_bridge_ifname_get(struct pon_net_context *ctx,
			  uint16_t me_id,
			  char *ifname,
			  size_t size)
{
	enum pon_adapter_errno ret;
	unsigned int bridge_idx;

	dbg_in_args("%p, %u, %p, %zd", ctx, me_id, ifname, size);

	ret = mapper_index_get(ctx->mapper[MAPPER_MACBRIDGE_MEID_TO_IDX],
			       me_id, &bridge_idx);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_out_ret("%d", ret);
		return ret;
	}

	ret = pon_net_bridge_ifname_get_by_idx(ctx, bridge_idx, ifname, size);

	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno
pon_net_uni_lport_get(const uint16_t me_id, uint16_t *lport)
{
	dbg_in_args("%u, %p", me_id, lport);

	if (!lport) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	if ((me_id & 0xFF) == 0) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}

	*lport = (uint16_t)((me_id & 0xFF) - 1);

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_net_pptp_eth_uni_name_get(struct pon_net_context *ctx,
			      uint16_t lport,
			      char *ifname,
			      size_t size)
{
	return pon_net_uni_ifname_get(ctx, UNI_PPTP_ETH, lport, ifname, size);
}

enum pon_adapter_errno
pon_net_pptp_lct_uni_name_get(struct pon_net_context *ctx,
			      uint16_t lport,
			      char *ifname,
			      size_t size)
{
	return pon_net_uni_ifname_get(ctx, UNI_PPTP_LCT, lport, ifname, size);
}

enum pon_adapter_errno
pon_net_veip_name_get(struct pon_net_context *ctx,
		      uint32_t lport,
		      char *ifname,
		      size_t size)
{
	return pon_net_uni_ifname_get(ctx, UNI_VEIP, (uint16_t)lport,
				      ifname, size);
}

enum pon_adapter_errno pon_net_macaddr_get(struct pon_net_context *ctx,
					   uint16_t class_id, uint16_t lport,
					   uint8_t mac[ETH_ALEN])
{
	struct pon_net_config *cfg = &ctx->cfg;
	uint8_t empty_mac[ETH_ALEN] = {0,};
	int err;

	switch (class_id) {
	case PON_CLASS_ID_PPTP_ETHERNET_UNI:
		if (lport >= ARRAY_SIZE(cfg->uni_macaddr))
			return PON_ADAPTER_ERR_INVALID_VAL;
		if (memcmp(cfg->uni_macaddr[lport], empty_mac, ETH_ALEN) == 0)
			return PON_ADAPTER_ERR_NO_DATA;
		err = memcpy_s(mac, ETH_ALEN, cfg->uni_macaddr[lport],
			       sizeof(cfg->uni_macaddr[lport]));
		if (err)
			return PON_ADAPTER_ERR_OUT_OF_BOUNDS;
		return PON_ADAPTER_SUCCESS;
	case PON_CLASS_ID_VEIP:
		if (lport >= ARRAY_SIZE(cfg->veip_macaddr))
			return PON_ADAPTER_ERR_INVALID_VAL;
		if (memcmp(cfg->veip_macaddr[lport], empty_mac, ETH_ALEN) == 0)
			return PON_ADAPTER_ERR_NO_DATA;
		err = memcpy_s(mac, ETH_ALEN, cfg->veip_macaddr[lport],
			       sizeof(cfg->veip_macaddr[lport]));
		if (err)
			return PON_ADAPTER_ERR_OUT_OF_BOUNDS;
		return PON_ADAPTER_SUCCESS;
	default:
		return PON_ADAPTER_ERR_NO_DATA;
	}
}

uint8_t ifname_to_lan_idx(struct pon_net_context *ctx, const char *ifname)
{
	uint32_t me_id;
	uint8_t i;
	struct mapper *mapper_uni;
	struct mapper *mapper_veip;

	mapper_uni = ctx->mapper[MAPPER_PPTPETHERNETUNI_MEID_TO_IDX];
	mapper_veip = ctx->mapper[MAPPER_VEIP_MEID_TO_IDX];

	for (i = 0; i < ARRAY_SIZE(ctx->cfg.uni_name); i++) {
		/* check if port is used (mapped) */
		if (mapper_id_get(mapper_uni, &me_id, i) != PON_ADAPTER_SUCCESS)
			continue;
		if (strncmp(ifname, ctx->cfg.uni_name[i], IF_NAMESIZE) == 0)
			return i;
	}

	for (i = 0; i < ARRAY_SIZE(ctx->cfg.veip_name); i++) {
		/* check if port is used (mapped) */
		if (mapper_id_get(mapper_veip, &me_id, i)
							!= PON_ADAPTER_SUCCESS)
			continue;
		if (strncmp(ifname, ctx->cfg.veip_name[i], IF_NAMESIZE) == 0)
			return i;
	}

	return 0xFF;
}

uint8_t ifindex_to_lan_index(struct pon_net_context *ctx, int ifindex)
{
	char ifname[IF_NAMESIZE];
	uint8_t ret;

	if (ifindex < 0 || !if_indextoname((unsigned int)ifindex, ifname)) {
		dbg_err_fn(if_indextoname);
		dbg_out_ret("%d", 0xFF);
		return 0xFF;
	}
	ret = ifname_to_lan_idx(ctx, ifname);

	dbg_out_ret("%d", ret);
	return ret;
}

int pon_net_me_id_in_array(uint16_t id, const uint16_t *array,
			   size_t capacity)
{
	unsigned int i;

	dbg_in_args("%u, %p, %zu", id, array, capacity);

	for (i = 0; i < capacity; ++i) {
		if (id == array[i]) {
			dbg_out_ret("%d", 1);
			return 1;
		}
	}

	dbg_out_ret("%d", 0);
	return 0;
}

enum pon_adapter_errno pon_net_color_marking_set(struct pon_net_context *ctx,
						 uint16_t class_id,
						 uint16_t me_id,
						 uint8_t color_marking)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint32_t handle, prio;
	char ifname[IF_NAMESIZE] = {0};

	dbg_in_args("%p, %u, %u, %u", ctx, class_id, me_id, color_marking);

	ret = pon_net_ifname_get(ctx, class_id, me_id, ifname,
				 ARRAY_SIZE(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_ifname_get, ret);
		return ret;
	}

	ret = pon_net_dev_db_map(ctx->db, ifname,
				 PON_NET_HANDLE_COLOR_MARKING_INGRESS,
				 0, &handle);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_map, ret);
		return ret;
	}

	ret = pon_net_dev_db_map(ctx->db, ifname,
				 PON_NET_PRIO_COLOR_MARKING_INGRESS,
				 0, &prio);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_map, ret);
		return ret;
	}

	ret = netlink_color_marking_set(ctx->netlink, ifname,
					NETLINK_FILTER_DIR_INGRESS,
					color_marking, handle,
					(uint16_t)prio);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_color_marking_set, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno pon_net_police_set(struct pon_net_context *ctx,
					  uint16_t class_id,
					  uint16_t me_id,
					  enum netlink_filter_dir dir,
					  struct netlink_police_data *police)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint32_t handle, prio;
	char ifname[IF_NAMESIZE] = {0};

	dbg_in_args("%p, %u, %u, %u, %p", ctx, class_id, me_id, dir, police);

	ret = pon_net_ifname_get(ctx, class_id, me_id, ifname,
				 ARRAY_SIZE(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_ifname_get, ret);
		return ret;
	}

	ret = pon_net_dev_db_map(ctx->db, ifname,
				 dir == NETLINK_FILTER_DIR_INGRESS
				     ? PON_NET_HANDLE_POLICE_INGRESS
				     : PON_NET_HANDLE_POLICE_EGRESS,
				 0, &handle);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_map, ret);
		return ret;
	}

	ret = pon_net_dev_db_map(ctx->db, ifname,
				 dir == NETLINK_FILTER_DIR_INGRESS
				     ? PON_NET_PRIO_POLICE_INGRESS
				     : PON_NET_PRIO_POLICE_EGRESS,
				 0, &prio);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_map, ret);
		return ret;
	}

	ret = netlink_police_set(ctx->netlink, ifname, dir, handle,
				 (uint16_t)prio, police);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_police_set, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno pon_net_police_unset(struct pon_net_context *ctx,
					    uint16_t class_id,
					    uint16_t me_id,
					    enum netlink_filter_dir dir)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint32_t handle, prio;
	char ifname[IF_NAMESIZE] = {0};

	dbg_in_args("%p, %u, %u, %u", ctx, class_id, me_id, dir);

	ret = pon_net_ifname_get(ctx, class_id, me_id, ifname,
				 ARRAY_SIZE(ifname));
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_ifname_get, ret);
		return ret;
	}

	ret = pon_net_dev_db_map(ctx->db, ifname,
				 dir == NETLINK_FILTER_DIR_INGRESS
				     ? PON_NET_HANDLE_POLICE_INGRESS
				     : PON_NET_HANDLE_POLICE_EGRESS,
				 0, &handle);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_map, ret);
		return ret;
	}

	ret = pon_net_dev_db_map(ctx->db, ifname,
				 dir == NETLINK_FILTER_DIR_INGRESS
				     ? PON_NET_PRIO_POLICE_INGRESS
				     : PON_NET_PRIO_POLICE_EGRESS,
				 0, &prio);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_map, ret);
		return ret;
	}

	ret = netlink_filter_clear_one(ctx->netlink, ifname, ETH_P_ALL,
				       (uint16_t)prio, handle, dir);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_filter_clear_one, ret);
		return ret;
	}

	dbg_out_ret("%d", ret);
	return ret;
}

/* Maximum number of supported GEM ports */
#define MAX_GEMS 256

static const struct {
	uint16_t class_id;
	uint8_t dir;
} class_id_to_dir_map[] = {
	{ PON_CLASS_ID_TCONT, PA_GEMPORTNETCTP_DIR_UPSTREAM },
	{ PON_CLASS_ID_PPTP_ETHERNET_UNI, PA_GEMPORTNETCTP_DIR_DOWNSTREAM },
	{ PON_CLASS_ID_VEIP, PA_GEMPORTNETCTP_DIR_DOWNSTREAM }
};

static enum pon_adapter_errno
class_id_to_dir(uint16_t class_id, uint8_t *dir)
{
	unsigned int i = 0;

	dbg_in_args("%u, %p", class_id, dir);

	for (i = 0; i < ARRAY_SIZE(class_id_to_dir_map); ++i) {
		if (class_id_to_dir_map[i].class_id == class_id) {
			*dir = class_id_to_dir_map[i].dir;
			dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
			return PON_ADAPTER_SUCCESS;
		}
	}

	dbg_out_ret("%d", PON_ADAPTER_ERROR);
	return PON_ADAPTER_ERROR;
}

enum pon_adapter_errno pon_net_queue_unassign(struct pon_net_context *ctx,
					      uint16_t class_id, uint16_t me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint16_t gems[MAX_GEMS] = {0};
	uint8_t dir;
	size_t num_gems = 0;

	dbg_in_args("%p, %u, %u", ctx, class_id, me_id);

	ret = class_id_to_dir(class_id, &dir);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(class_id_to_dir, ret);
		goto error;
	}

	num_gems = pon_net_gem_me_ids_for_class_get(ctx, class_id, me_id, gems,
						    ARRAY_SIZE(gems));
	if (num_gems > ARRAY_SIZE(gems)) {
		dbg_err("Too many GEM ports (needed %zu, supported %zu)\n",
			num_gems, ARRAY_SIZE(gems));
		goto error;
	}

	ret = pon_net_gem_queue_unassign(ctx, dir, gems, num_gems);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_gem_queue_unassign, ret);
		goto error;
	}

error:
	dbg_out_ret("%d", ret);
	return ret;
}

enum pon_adapter_errno pon_net_queue_update(struct pon_net_context *ctx,
					    uint16_t class_id, uint16_t me_id)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	uint16_t gems[MAX_GEMS] = {0};
	uint8_t dir;
	size_t num_gems = 0;

	dbg_in_args("%p, %u, %u", ctx, class_id, me_id);

	ret = class_id_to_dir(class_id, &dir);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(class_id_to_dir, ret);
		goto error;
	}

	num_gems = pon_net_gem_me_ids_for_class_get(ctx, class_id, me_id, gems,
						    ARRAY_SIZE(gems));
	if (num_gems > ARRAY_SIZE(gems)) {
		dbg_err("Too many GEM ports (needed %zu, supported %zu)\n",
			num_gems, ARRAY_SIZE(gems));
		goto error;
	}

	ret = pon_net_gem_queue_update(ctx, dir, gems, num_gems);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_gem_queue_update, ret);
		goto error;
	}

error:
	dbg_out_ret("%d", ret);
	return ret;
}

static const struct {
	uint8_t type;
	bool tagged;
} traps[] = {
	/* Trap ICMPv6 MC Listener Query/Report/Done(Leave) (MLDv1) */
	{ ICMPV6_MGM_QUERY, false },
	{ ICMPV6_MGM_QUERY, true },
	{ ICMPV6_MGM_REPORT, false },
	{ ICMPV6_MGM_REPORT, true },
	{ ICMPV6_MGM_REDUCTION, false },
	{ ICMPV6_MGM_REDUCTION, true },
	/* ICMPv6 Multicast Listener Report (MLDv2) */
	{ ICMPV6_MLD2_REPORT, false },
	{ ICMPV6_MLD2_REPORT, true },
};

static enum pon_adapter_errno
mcc_handle_and_prio(struct pon_net_dev_db *db,
		    unsigned int i,
		    uint32_t *handle,
		    uint16_t *prio)
{
	enum pon_adapter_errno ret;
	uint32_t priority;

	dbg_in_args("%p, %u, %p, %p", db, i, handle, prio);

	ret = pon_net_dev_db_map(db, PON_MASTER_DEVICE,
				 PON_NET_HANDLE_MCC_TRAP_INGRESS, i, handle);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_map, ret);
		return ret;
	}

	ret = pon_net_dev_db_map(db, PON_MASTER_DEVICE,
				 PON_NET_PRIO_MCC_TRAP_INGRESS, i, &priority);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_map, ret);
		return ret;
	}

	*prio = (uint16_t)priority;

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno
pon_net_mcc_cpu_traps_set(struct pon_net_context *ctx)
{
	enum pon_adapter_errno ret;
	unsigned int i;
	uint32_t handle;
	uint16_t priority;
	struct netlink_filter filter;

	for (i = 0; i < ARRAY_SIZE(traps); ++i) {
		ret = mcc_handle_and_prio(ctx->db, i, &handle, &priority);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, mcc_handle_and_prio, ret);
			return ret;
		}

		ret = netlink_filter_icmpv6(
		    &filter, PON_MASTER_DEVICE, IGMP_HW_TC, traps[i].type,
		    traps[i].tagged, NETLINK_FILTER_ACT_TRAP, NULL);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_filter_icmpv6, ret);
			return ret;
		}

		filter.handle = handle;
		filter.prio = priority;

		netlink_filter_del(ctx->netlink, &filter);
		ret = netlink_filter_add(ctx->netlink, &filter);
		if (ret != PON_ADAPTER_SUCCESS) {
			FN_ERR_RET(ret, netlink_filter_add, ret);
			return ret;
		}
	}

	netlink_filter_igmp_trap(&filter, false);

	ret = mcc_handle_and_prio(ctx->db, i, &handle, &priority);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, mcc_handle_and_prio, ret);
		return ret;
	}

	filter.handle = handle;
	filter.prio = priority;

	netlink_filter_del(ctx->netlink, &filter);
	ret = netlink_filter_add(ctx->netlink, &filter);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_filter_add, ret);
		return ret;
	}

	i++;

	netlink_filter_igmp_trap(&filter, true);

	ret = mcc_handle_and_prio(ctx->db, i, &handle, &priority);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, mcc_handle_and_prio, ret);
		return ret;
	}

	filter.handle = handle;
	filter.prio = priority;

	netlink_filter_del(ctx->netlink, &filter);
	ret = netlink_filter_add(ctx->netlink, &filter);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_filter_add, ret);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

void
pon_net_mcc_cpu_traps_unset(struct pon_net_context *ctx)
{
	unsigned int i;
	enum pon_adapter_errno ret;
	uint32_t handle;
	uint16_t priority;
	struct netlink_filter filter;

	for (i = 0; i < ARRAY_SIZE(traps); ++i) {
		ret = netlink_filter_icmpv6(
		    &filter, PON_MASTER_DEVICE, IGMP_HW_TC, traps[i].type,
		    traps[i].tagged, NETLINK_FILTER_ACT_TRAP, NULL);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(netlink_filter_icmpv6, ret);
			return;
		}

		ret = mcc_handle_and_prio(ctx->db, i, &handle, &priority);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(mcc_handle_and_prio, ret);
			return;
		}

		filter.handle = handle;
		filter.prio = priority;

		netlink_filter_del(ctx->netlink, &filter);
	}

	netlink_filter_igmp_trap(&filter, true);

	ret = mcc_handle_and_prio(ctx->db, i, &handle, &priority);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(mcc_handle_and_prio, ret);
		return;
	}

	filter.handle = handle;
	filter.prio = priority;

	netlink_filter_del(ctx->netlink, &filter);

	netlink_filter_igmp_trap(&filter, false);

	i++;

	ret = mcc_handle_and_prio(ctx->db, i, &handle, &priority);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(mcc_handle_and_prio, ret);
		return;
	}

	filter.handle = handle;
	filter.prio = priority;

	netlink_filter_del(ctx->netlink, &filter);
}

#ifdef MCC_DRV_ENABLE

void pon_net_mcc_multicast_enable(struct pon_net_context *ctx)
{
	int ret;

	dbg_in_args("%p", ctx);

	if (ctx->is_multicast_enabled) {
		dbg_out();
		return;
	}

	/*
	 * TODO:
	 * Once sources verification for multicast data packets
	 * will be implemented, then replace the argument value
	 * value '2' by the value 'ctx->me_mop_igmp_version'
	 * in the arguments list below
	 */
	ret = mcl_multicast_enable(2, (uint32_t)ctx->mc_gem_ifindex);
	if (ret) {
		dbg_wrn_fn_ret(mcl_multicast_enable, ret);
	} else {
		ctx->is_multicast_enabled = 1;
		if (sem_post(&ctx->multicast_enabled_event))
			dbg_err_fn(sem_post);
	}

	if (ctx->mc_gem_ifindex) {
		ret = netlink_bridge_port_cfg(ctx->netlink,
					      ctx->mc_gem_ifindex,
					      true, false, false);
		if (ret != PON_ADAPTER_SUCCESS)
			dbg_wrn_fn_ret(netlink_bridge_port_cfg, ret);
	}

	if (ctx->mc_br_ifindex) {
		ret = netlink_bridge_mcast_flood(ctx->netlink,
						 ctx->mc_br_ifindex, true);
		if (ret != PON_ADAPTER_SUCCESS)
			dbg_wrn_fn_ret(netlink_bridge_mcast_flood, ret);
	}

	(void)pon_net_mcc_cpu_traps_set(ctx);
	dbg_out();
}

void pon_net_mcc_multicast_disable(struct pon_net_context *ctx)
{
	int ret;

	dbg_in_args("%p", ctx);

	if (!ctx->is_multicast_enabled) {
		return;
		dbg_out();
	}

	if (ctx->mc_gem_ifindex) {
		ret = netlink_bridge_port_cfg(ctx->netlink,
					      ctx->mc_gem_ifindex,
					      false, false, false);
		if (ret != PON_ADAPTER_SUCCESS)
			dbg_err_fn_ret(netlink_bridge_port_cfg, ret);
	}

	if (ctx->mc_br_ifindex) {
		ret = netlink_bridge_mcast_flood(ctx->netlink,
						 ctx->mc_br_ifindex,
						 false);
		if (ret != PON_ADAPTER_SUCCESS)
			dbg_wrn_fn_ret(netlink_bridge_mcast_flood, ret);
	}

	ret = mcl_multicast_disable();
	if (ret)
		dbg_wrn_fn_ret(mcl_multicast_disable, ret);
	/* Assume multicast is disabled successfully */
	ctx->is_multicast_enabled = 0;
	pon_net_mcc_cpu_traps_unset(ctx);

	dbg_out();
}
#endif

enum pon_adapter_errno
pon_net_get_mac_by_ifname(struct pon_net_context *ctx, char *ifname,
			  uint8_t mac_address[ETH_ALEN])
{
#ifdef LINUX
	int fd;
	struct ifreq ifr;
	int res;
#endif

	dbg_in_args("%p, \"%s\", %p", ctx, ifname, mac_address);

	memset(mac_address, 0x00, ETH_ALEN);
#ifdef LINUX
	fd = pon_net_socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		FN_ERR_RET(fd, pon_net_socket, PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}
	ifr.ifr_addr.sa_family = AF_INET;
	snprintf(ifr.ifr_name, IF_NAMESIZE, "%s", ifname);
	if (pon_net_ioctl(fd, SIOCGIFHWADDR, &ifr) < 0) {
		FN_ERR_RET(errno, "SIOCGIFHWADDR", PON_ADAPTER_ERROR);
		pon_net_close(fd);
		return PON_ADAPTER_ERROR;
	}
	pon_net_close(fd);
	res = memcpy_s(mac_address, ETH_ALEN, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
	if (res) {
		dbg_err_fn_ret(memcpy_s, res);
		return PON_ADAPTER_ERROR;
	}
#endif

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

const struct pon_net_tree_builder *
pon_net_get_default_qdisc_tree_builder(struct pon_net_context *ctx)
{
	const struct pon_net_tree_builder *builder_to_setup;

	dbg_in_args("%p", ctx);

	if (pon_net_traffic_management_option_get(ctx) == PA_TMO_RATE)
		builder_to_setup = &pon_net_sp_tbf_builder;
	else
		builder_to_setup = &pon_net_sp_builder;

	dbg_out_ret("%p", builder_to_setup);
	return builder_to_setup;
}

const struct pon_net_tree_builder *
pon_net_get_ext_uni_ds_qdisc_tree_builder(struct pon_net_context *ctx)
{
	dbg_in_args("%p", ctx);

	dbg_out_ret("%p", &pon_net_ext_uni_ds_builder);
	return &pon_net_ext_uni_ds_builder;
}

enum pon_adapter_errno
pon_net_thread_stop(const pthread_t *th_id, const char *th_name, int time)
{
	char errbuf[256] = {0};
	struct timespec ts;
	void *res;
	int err;

	dbg_in_args("%p, %s, %d", th_id, th_name, time);

#ifndef WIN32
	if (!*th_id) {
		dbg_wrn("pthread %s does not exist, can not stop\n", th_name);
		return PON_ADAPTER_SUCCESS;
	}
#endif

	err = pthread_cancel(*th_id);
	if (err && err != ESRCH) {
		if (strerror_r(err, errbuf, sizeof(errbuf)))
			snprintf(errbuf, sizeof(errbuf) - 1, "%s",
				 "unknown error");
		dbg_err("pthread_cancel <%s> returned: %d - %s\n",
			th_name, err, errbuf);
		return PON_ADAPTER_ERROR;
	}

	/* wait a given time for joining the thread */
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += time;
	err = pthread_timedjoin_np(*th_id, &res, &ts);
	if (err) {
		if (strerror_r(err, errbuf, sizeof(errbuf)))
			snprintf(errbuf, sizeof(errbuf) - 1, "%s",
				 "unknown error");
		dbg_err("%s <%s> returned: %d %s\n",
			"pthread_timedjoin_np", th_name, err, errbuf);
		return PON_ADAPTER_ERROR;
	}

	if (res && res != PTHREAD_CANCELED) {
		dbg_err("<%s> returned with %ld\n",
			th_name, (long int)res);
		return PON_ADAPTER_ERROR;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

enum pon_adapter_errno pon_net_iphost_create(struct pon_net_context *ctx,
					     const char *ifname,
					     const char *master)
{
	enum pon_adapter_errno ret;

	dbg_in_args("%p, \"%s\", %s", ctx, ifname, master);

	ret = pon_net_dev_db_add(ctx->db, ifname, &pon_net_iphost_def);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_dev_db_add, ret);
		return ret;
	}

	ret = netlink_iphost_create(ctx->netlink, ifname, master);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_iphost_create, ret);
		pon_net_dev_db_del(ctx->db, ifname);
		return ret;
	}

	ret = netlink_qdisc_clsact_create(ctx->netlink, ifname);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, netlink_qdisc_clsact_create, ret);
		netlink_iphost_destroy(ctx->netlink, ifname);
		pon_net_dev_db_del(ctx->db, ifname);
		return ret;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

void pon_net_iphost_destroy(struct pon_net_context *ctx, const char *ifname)
{
	dbg_in_args("%p, %p", ctx, ifname);

	pon_net_dev_db_del(ctx->db, ifname);
	netlink_qdisc_clsact_destroy(ctx->netlink, ifname);
	netlink_iphost_destroy(ctx->netlink, ifname);

	dbg_out();
}

bool pon_net_array_eq_u16(const uint16_t *array_a, unsigned int array_a_len,
			  const uint16_t *array_b, unsigned int array_b_len)
{
	unsigned int i;

	dbg_in_args("%p, %u, %p, %u", array_a, array_a_len, array_b,
		    array_b_len);

	if (array_a_len != array_b_len) {
		dbg_out_ret("%d", false);
		return false;
	}

	for (i = 0; i < array_a_len; ++i) {
		if (array_a[i] != array_b[i]) {
			dbg_out_ret("%d", false);
			return false;
		}
	}

	dbg_out_ret("%d", true);
	return true;
}

static void *array_at(void *base, unsigned int size, unsigned int i)
{
	return (unsigned char *)base + size * i;
}

#define LESS -1
#define GREATER 1
#define EQUAL 0

enum pon_adapter_errno
pon_net_pair_walk(const struct pon_net_pair_walk_args *args)
{
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	unsigned int old_idx = 0;
	unsigned int new_idx = 0;
	void *old = NULL;
	void *new = NULL;

	dbg_in_args("%p", args);

	while (old_idx < args->first_array_len &&
	       new_idx < args->second_array_len) {

		old = array_at(args->first_array, args->elem_size, old_idx);
		new = array_at(args->second_array, args->elem_size, new_idx);

		switch (args->cmp(old, new, args->arg)) {
		case LESS:
			/* present in old not in new */
			ret = args->fn(old, NULL, args->arg);
			old_idx++;
			break;
		case GREATER:
			/* present in new not in old */
			ret = args->fn(NULL, new, args->arg);
			new_idx++;
			break;
		case EQUAL:
			/* present in new and old */
			ret = args->fn(old, new, args->arg);
			old_idx++;
			new_idx++;
			break;
		default:
			ret = PON_ADAPTER_ERROR;
			break;
		}

		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(fn, ret);
			goto out;
		}
	}

	for (; old_idx < args->first_array_len; ++old_idx) {
		old = array_at(args->first_array, args->elem_size, old_idx);
		ret = args->fn(old, NULL, args->arg);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(fn, ret);
			goto out;
		}
	}

	for (; new_idx < args->second_array_len; ++new_idx) {
		new = array_at(args->second_array, args->elem_size, new_idx);
		ret = args->fn(NULL, new, args->arg);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(fn, ret);
			goto out;
		}
	}

out:
	dbg_out_ret("%d", ret);
	return ret;
}

