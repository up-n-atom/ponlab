/*****************************************************************************
 *
 * Copyright (c) 2020 - 2026 MaxLinear, Inc.
 * Copyright (c) 2017 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifdef LINUX
#include <unistd.h>

/* for Linux a socket is not different to a normal file descriptor */
#define closesocket close
#else
#include <winsock2.h>
#endif

#include <linux/pkt_cls.h>

#include <netlink/route/link.h>

#include <pon_adapter.h>
#include <pon_adapter_system.h>
#include <pon_adapter_config.h>
#include <omci/pon_adapter_omci.h>

#include "pon_net_register.h"
#include "pon_net_common.h"
#include "pon_net_debug.h"
#include "pon_net_mcc_core.h"
#include "pon_net_event.h"
#include "pon_net_netlink.h"
#include "pon_net_extern.h"
#include "pon_net_dev_db.h"
#include "pon_net_dev_defs.h"
#include "pon_net_init.h"
#include "pon_net_vlan_aware_bridging.h"
#include "pon_net_ethtool.h"

#define sizeof_member(type, member) sizeof(((type *)0)->member)
#define sizeof_array_member(type, member) sizeof(((type *)0)->member[0])

#define OPT_REQUIRED 1
#define OPT_OPTIONAL 0

#define INIT_OPTION(n, p, o, r) { \
	.name = n, .parser = p, .required = r, \
	.offset = offsetof(struct pon_net_config, o), \
	.size = sizeof_member(struct pon_net_config, o) }

struct init_option_parser {
	const char *name;
	uint8_t required;
	uint16_t offset;
	uint16_t size;
	enum pon_adapter_errno (*parser)(void *val,
					 const uint8_t size,
					 char const *string);
};

static enum pon_adapter_errno parse_mac(void *val,
					const uint8_t size,
					char const *string)
{
	int ret;
	uint8_t *mac = val;
	unsigned int values[6];
	int i;
	char rest;

	dbg_in_args("%p, %d, %s", val, size, string);

	if (size != 6)
		return PON_ADAPTER_ERROR;

	/*
	 * Parse the mac address, if there is something remaining after the mac
	 * address rest will get filled and this will return 7.
	 */
	ret = sscanf_s(string, "%x:%x:%x:%x:%x:%x%c",
		       &values[0], &values[1], &values[2],
		       &values[3], &values[4], &values[5],
		       SSCANF_CHAR(&rest));
	if (ret != 6) {
		dbg_out_ret("%d", PON_ADAPTER_ERROR);
		return PON_ADAPTER_ERROR;
	}

	for (i = 0; i < 6; i++)
		mac[i] = (uint8_t)values[i];

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno parse_str(void *val,
					uint8_t size,
					char const *string)
{
	dbg_in_args("%p, %d, %s", val, size, string);

	if (strncpy_s(val, size, string, size)) {
		dbg_err_fn(strncpy_s);
		dbg_out_ret("%d", PON_ADAPTER_ERR_NO_DATA);
		return PON_ADAPTER_ERR_NO_DATA;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno parse_uint(void *val, uint8_t size,
					 char const *string)
{
	char const *nptr = string;
	char *endptr;
	uint32_t value;

	errno = 0;
	value = (uint32_t)strtoul(nptr, &endptr, 0);
	if (errno || nptr == endptr)
		return PON_ADAPTER_ERROR;

	switch (size) {
	case 1:
		*(uint8_t *)val = (uint8_t)value;
		break;
	case 2:
		*(uint16_t *)val = (uint16_t)value;
		break;
	case 4:
		*(uint32_t *)val = value;
		break;
	default:
		return PON_ADAPTER_ERROR;
	}

	return PON_ADAPTER_SUCCESS;
}

static const struct {
	const char *key;
	enum pa_pon_op_mode value;
} pon_modes[] = {
	{"gpon", PA_PON_MODE_G984},
	{"xgspon", PA_PON_MODE_G9807},
	{"xgpon", PA_PON_MODE_G987},
	{"ngpon2_2G5", PA_PON_MODE_G989},
	{"ngpon2_10G", PA_PON_MODE_G989}
};

static enum pon_adapter_errno parse_pon_mode(void *val, uint8_t size,
					     char const *string)
{
	enum pa_pon_op_mode *pon_mode = val;
	unsigned int i = 0;

	UNUSED(size);

	for (i = 0; i < ARRAY_SIZE(pon_modes); ++i) {
		if (strcmp(string, pon_modes[i].key) == 0) {
			*pon_mode = pon_modes[i].value;
			return PON_ADAPTER_SUCCESS;
		}
	}

	*pon_mode = PA_PON_MODE_UNKNOWN;
	return PON_ADAPTER_ERR_INVALID_VAL;
}

static enum pon_adapter_errno parse_macaddr(void *val, uint8_t size,
					    char const *string)
{
	uint8_t *mac = val;
	unsigned int values[ETH_ALEN];
	int ret, i;
	char rest;

	if (!string) {
		memset(mac, 0x0, ETH_ALEN);
		return PON_ADAPTER_SUCCESS;
	}

	if (size < ETH_ALEN)
		return PON_ADAPTER_ERROR;

	/*
	 * Parse the Ethernet MAC address, if there is something remaining after
	 * the MAC address rest will get filled and this will return 7.
	 */
	ret = sscanf_s(string, "%x:%x:%x:%x:%x:%x%c",
		     &values[0], &values[1], &values[2],
		     &values[3], &values[4], &values[5], SSCANF_CHAR(&rest));
	if (ret != ETH_ALEN)
		return PON_ADAPTER_ERROR;

	for (i = 0; i < ETH_ALEN; i++)
		mac[i] = (uint8_t)values[i];

	return PON_ADAPTER_SUCCESS;
}


static struct pon_net_context g_pon_net_context;

static const struct init_option_parser init_options[] = {
	INIT_OPTION("pon_mac",	parse_mac,	omcc_dev_mac,	OPT_REQUIRED),
	INIT_OPTION("soc_mac",	parse_mac,	omcc_if_mac,	OPT_REQUIRED),
};

#define CFG_OPTION(r, n, s, o, v, p, m) \
	{ .name = n, .section = s, .option = o, .value = v, .parser = p, \
	  .offset = offsetof(struct pon_net_config, m), \
	  .size = sizeof_member(struct pon_net_config, m), \
	  .is_mandatory = r }

#define CFG_OPTION_ARRAY(r, n, s, o, v, p, m, i) \
	{ .name = n, .section = s, .option = o, .value = v, .parser = p, \
	  .offset = offsetof(struct pon_net_config, m) +\
		    i * sizeof_array_member(struct pon_net_config, m),\
	  .size = sizeof_array_member(struct pon_net_config, m), \
	  .is_mandatory = r }

#define PON_OPT 0
#define PON_REQ 1

struct cfg_option {
	const char *name;
	const char *section;
	const char *option;
	const char *value; /* default */
	size_t offset;
	size_t size;
	enum pon_adapter_errno (*parser)(void *val, uint8_t size,
					 char const *string);
	int is_mandatory;
};

static const struct cfg_option cfg_options[] = {
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni1", "name", "eth0_0",
		   parse_str, uni_name, 0),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni2", "name", "eth0_1",
		   parse_str, uni_name, 1),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni3", "name", "",
		   parse_str, uni_name, 2),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni4", "name", "",
		   parse_str, uni_name, 3),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni5", "name", "",
		   parse_str, uni_name, 4),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni1", "multicast_name",
		   "eth0_0_2", parse_str, uni_mc_name, 0),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni2", "multicast_name",
		   "eth0_1_2", parse_str, uni_mc_name, 1),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni3", "multicast_name",
		   "", parse_str, uni_mc_name, 2),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni4", "multicast_name",
		   "", parse_str, uni_mc_name, 3),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni5", "multicast_name",
		   "", parse_str, uni_mc_name, 4),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni1", "broadcast_name",
		   "eth0_0_3", parse_str, uni_bc_name, 0),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni2", "broadcast_name",
		   "eth0_1_3", parse_str, uni_bc_name, 1),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni3", "broadcast_name",
		   "", parse_str, uni_bc_name, 2),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni4", "broadcast_name",
		   "", parse_str, uni_bc_name, 3),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni5", "broadcast_name",
		   "", parse_str, uni_bc_name, 4),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni1", "macaddr",
		   NULL, parse_macaddr, uni_macaddr, 0),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni2", "macaddr",
		   NULL, parse_macaddr, uni_macaddr, 1),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni3", "macaddr",
		   NULL, parse_macaddr, uni_macaddr, 2),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni4", "macaddr",
		   NULL, parse_macaddr, uni_macaddr, 3),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni5", "macaddr",
		   NULL, parse_macaddr, uni_macaddr, 4),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "veip1", "name", "vuni1",
		   parse_str, veip_name, 0),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "veip2", "name", "vuni2",
		   parse_str, veip_name, 1),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "veip1", "multicast_name",
		   "veip1_mc", parse_str, veip_mc_name, 0),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "veip2", "multicast_name",
		   "veip2_mc", parse_str, veip_mc_name, 1),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "veip1", "broadcast_name",
		   "veip1_bc", parse_str, veip_bc_name, 0),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "veip2", "broadcast_name",
		   "veip2_bc", parse_str, veip_bc_name, 1),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "veip1", "macaddr",
		   NULL, parse_macaddr, veip_macaddr, 0),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "veip2", "macaddr",
		   NULL, parse_macaddr, veip_macaddr, 1),
	CFG_OPTION(PON_OPT, "network", "lct", "ifname", "none",
		   parse_str, lct_name),
	CFG_OPTION(PON_OPT, "gpon", "ponip", "pon_mode", "gpon",
		   parse_pon_mode, mode),
	CFG_OPTION(PON_OPT, "omci", "net", "vlan_forwarding", "1",
		   parse_uint, vlan_forwarding),
	CFG_OPTION(PON_OPT, "omci", "net", "no_ds_prio_queues", "0",
		   parse_uint, no_ds_prio_queues),
	CFG_OPTION(PON_OPT, "omci", "net", "enable_cpu_queues", "0",
		   parse_uint, enable_cpu_queues),
	CFG_OPTION(PON_OPT, "omci", "ext_switch", "name", "",
		   parse_str, uni_ext_switch_name),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni1", "ext_master", "",
			 parse_str, uni_ext_master_name, 0),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni2", "ext_master", "",
			 parse_str, uni_ext_master_name, 1),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni3", "ext_master", "",
			 parse_str, uni_ext_master_name, 2),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni4", "ext_master", "",
			 parse_str, uni_ext_master_name, 3),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni5", "ext_master", "",
			 parse_str, uni_ext_master_name, 4),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni1", "id", "0",
			 parse_uint, uni_ext_id, 0),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni2", "id", "0",
			 parse_uint, uni_ext_id, 1),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni3", "id", "0",
			 parse_uint, uni_ext_id, 2),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni4", "id", "0",
			 parse_uint, uni_ext_id, 3),
	CFG_OPTION_ARRAY(PON_OPT, "omci", "uni5", "id", "0",
			 parse_uint, uni_ext_id, 4),
};

static enum pon_adapter_errno pon_net_config(struct pon_net_config *cfg,
					     char const * const *init_data)
{
	enum pon_adapter_errno error = PON_ADAPTER_SUCCESS;
	char buffer[200]; /* reg_id will be around 180 chars (36 * 5) */
	char *name, *value, *saveptr;
	uint8_t i;
	uint32_t optmask = 0;

	dbg_in_args("%p, %p", cfg, init_data);

	/* check for the required params */
	for (i = 0; i < ARRAY_SIZE(init_options); i++)
		if (init_options[i].required)
			optmask |= 1U << i;

	while (init_data && *init_data) {
		if (strncpy_s(buffer, sizeof(buffer),
					*init_data, sizeof(buffer))) {
			dbg_err_fn(strncpy_s);
			dbg_out_ret("%d", PON_ADAPTER_ERR_NO_DATA);
			return PON_ADAPTER_ERR_NO_DATA;
		}
		name = strtok_r(buffer, "=", &saveptr);
		value = strtok_r(NULL, "=", &saveptr);

		if (!name)
			continue;

		/* check for the params we are interested in */
		for (i = 0; i < ARRAY_SIZE(init_options) && value; i++) {
			if (strcmp(name, init_options[i].name) == 0) {
				if (init_options[i].size > 0xFF) {
					error = PON_ADAPTER_ERROR;
					goto err;
				}
				error = init_options[i].parser(((uint8_t *)cfg)
					+ init_options[i].offset,
					(uint8_t)init_options[i].size, value);
				if (error) {
					dbg_err("Parsing failed on %s!\n",
						name);
					goto err;
				}
				/* clear bit for each option */
				optmask &= ~(1U << i);
				break;
			}
		}
		init_data++;
	}

	/* this should be 0 if required init_options were found */
	if (optmask) {
		dbg_naked(DBG_ERR,
			  "Required init param missing, please provide:\n");
		for (i = 0; i < ARRAY_SIZE(init_options); i++) {
			if (optmask & (1U << i))
				dbg_naked(DBG_ERR, "   - %s\n",
					  init_options[i].name);
		}
		error = PON_ADAPTER_ERROR;
	}

err:
	return error;
}

static uint32_t max_ports_number_get(struct pon_net_context *ctx)
{
	uint8_t i;
	uint32_t uni_ports = 0;
	uint32_t veip_ports = 0;

	for (i = 0; i < ARRAY_SIZE(ctx->cfg.uni_name); i++) {
		/* check if port is available */
		if (pon_net_if_nametoindex(ctx->cfg.uni_name[i]) > 0)
			uni_ports++;
		else
			break;
	}

	for (i = 0; i < ARRAY_SIZE(ctx->cfg.veip_name); i++) {
		/* check if port is available */
		if (pon_net_if_nametoindex(ctx->cfg.veip_name[i]) > 0)
			veip_ports++;
		else
			break;
	}

	if (uni_ports >= veip_ports)
		return uni_ports;

	return veip_ports;
}

static enum pon_adapter_errno read_pa_config(struct pon_net_context *ctx,
					     const struct pa_config *cfg,
					     const struct cfg_option *options,
					     size_t size,
					     const char *section)
{
	/* Return value for this function, if at the end
	 * ret != PON_ADAPTER_SUCCESS, then omci daemon will not start.
	 */
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	unsigned int i = 0;

	for (i = 0; i < size; ++i) {
		char value[PA_CONFIG_PARAM_STR_MAX_SIZE];
		const struct cfg_option *option = &options[i];
		const char *cfg_section = NULL;
		/* Error code for option->parser, ret can't be used for option
		 * parser because we could overwrite the fail code with
		 * successful code.
		 */
		enum pon_adapter_errno parse_error = PON_ADAPTER_SUCCESS;
		/* Used for reading uci config */
		int cfg_error;

		if (option->section)
			cfg_section = option->section;
		else if (section)
			cfg_section = section;

		if (cfg_section)
			cfg_error = cfg->get(ctx->hl_handle,
				       option->name,
				       cfg_section,
				       option->option,
				       sizeof(value),
				       value);
		else
			cfg_error = 0;

		if (cfg_error) {
			/* The option is PON_REQ and must be in UCI */
			if (option->is_mandatory == PON_REQ) {
				dbg_err("Missing required option: %s.%s.%s\n",
					option->name,
					cfg_section ? cfg_section :
						      "NO_SECTION_SPECIFIED",
					option->option);
				ret = PON_ADAPTER_ERROR;
				continue;
			}

			/* If option is not PON_REQ it can only be PON_OPT */
			if (option->is_mandatory != PON_OPT) {
				dbg_err("Option can be either PON_REQ or PON_OPT\n");
				ret = PON_ADAPTER_ERROR;
				continue;
			}

			/* If it is OPTIONAL and we do not have default value
			 * skip.
			 */
			if (!option->value)
				/* We do not set error code, because this is not
				 * critical and pon_net_lib can still be
				 * initialized.
				 */
				continue;

			if (sprintf_s(value, sizeof(value),
				       "%s", option->value) < 0) {
				dbg_err_fn(sprintf_s);
			}
		}
		if (options[i].size > 0xFF)
			return PON_ADAPTER_ERROR;
		parse_error = option->parser(
					((uint8_t *)&ctx->cfg) + option->offset,
					(uint8_t)option->size, value);
		if (parse_error) {
			dbg_err("Parsing failed for: %s.%s.%s\n",
				option->name,
				cfg_section ? cfg_section :
					      "NO_SECTION_SPECIFIED",
				option->option);
			return parse_error;
		}

		ctx->cfg.max_ports = max_ports_number_get(ctx);
	}

	return ret;
}

static bool is_netifd_running(struct pon_net_context *ctx)
{
	int err;

	/* Check if netifd is running by trying to call a method of the
	 * "network" object.
	 */
	if (!ctx->pa_config || !ctx->pa_config->ubus_call) {
		dbg_err("Ubus call is not available, cannot check netifd status\n");
		return false;
	}
	err = ctx->pa_config->ubus_call(ctx->hl_handle, "network",
					"get_proto_handlers", NULL, NULL, NULL,
					PON_UBUS_TIMEOUT);
	if (!err) {
		dbg_msg("netifd is running\n");
		return true;
	}
	dbg_msg("netifd is not running\n");
	return false;
}

static enum pon_adapter_errno pon_net_init(char const * const *init_data,
			  const struct pa_config *pa_config,
			  const struct pa_eh_ops *event_handler,
			  void *ll_handle)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	void *caller_ctx;
	int err = 0;

	dbg_in_args("%p, %p, %p, %p", init_data, pa_config, event_handler,
		    ll_handle);

	caller_ctx = ctx->hl_handle;

	if (memset_s(ctx, sizeof(*ctx), 0, sizeof(*ctx)))
		goto err;

	ctx->hl_handle = caller_ctx;

	ctx->pa_config = pa_config;

	/* read configuration */
	ret = pon_net_config(&ctx->cfg, init_data);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn(pon_net_config);
		goto err;
	}

	/* read UCI configuration */
	ret = read_pa_config(ctx, pa_config, cfg_options,
			     ARRAY_SIZE(cfg_options), NULL);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn(read_pa_config);
		goto err;
	}

	ctx->netlink = netlink_create();
	if (!ctx->netlink) {
		dbg_err_fn(netlink_create);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	if (pon_net_pa_mapper_init(ctx) != PON_ADAPTER_SUCCESS) {
		dbg_err_fn(pon_net_pa_mapper_init);
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	ctx->db = pon_net_dev_db_create();
	if (!ctx->db) {
		ret = PON_ADAPTER_ERROR;
		dbg_out_ret("%d", err);
		goto err;
	}

	pon_net_me_list_init(&ctx->me_list);
	pon_net_vlan_flow_list_init(&ctx->vlan_flow_list);

	/* File descriptor ioctl_fd handles network socket and is used later
	 * in pon_net_ethtool.c file for ioctl calls related to ethtool
	 * operations (SIOCETHTOOL identifier).
	 */
	ctx->ioctl_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (ctx->ioctl_fd < 0) {
		ret = PON_ADAPTER_ERROR;
		goto err;
	}

	ctx->event_handlers = event_handler;

	pon_net_eth_ext_pmhd_cnt_idx_defaults(&ctx->eth_ext_pmhd_cnt_idx);

	pon_net_cnt_idx_defaults(&ctx->eth_pmhd_cnt_idx);

	dbg_naked(DBG_MSG, "initialized\n");
err:
	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno pon_net_start(void *ll_handle)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	int err = 0;

	dbg_in_args("%p", ll_handle);

	ctx->netifd_running = is_netifd_running(ctx);
	if (ctx->netifd_running)
		dbg_msg("netifd is running, using it for network configuration\n");
	else
		dbg_err("netifd is not running, not possible to execute network configuration\n");

	ret = netlink_netdevices_clear_all(ctx->netlink);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_wrn_fn_ret(netlink_netdevices_clear_all, ret);

	ret = pon_net_omci_channel_init(ctx);
	if (ret != PON_ADAPTER_SUCCESS)
		goto err;

	ret = pon_net_master_dev_init(ctx);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_master_dev_init, ret);
		goto err;
	}

	ret = pon_net_iphost_init(ctx);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, iphost_init, ret);
		goto err;
	}

	ret = pon_net_ext_switch_init(ctx);
	if (ret != PON_ADAPTER_SUCCESS) {
		FN_ERR_RET(ret, pon_net_ext_switch_init, ret);
		goto err;
	}

	/* Enable create CPU queues by PON interface */
	if (ctx->cfg.enable_cpu_queues) {
		ret = pon_net_pon_handles_alloc(ctx);
		if (ret != PON_ADAPTER_SUCCESS)
			goto err;

		ret = pon_net_tc_cpu_queues_add(ctx);
		if (ret != PON_ADAPTER_SUCCESS) {
			dbg_err_fn_ret(pon_net_tc_cpu_queues_add, ret);
			goto err;
		}
	}

	/* Select class mode for SPCP and DSCP classification */
	ret = pon_net_ethtool_priv_flag_set(ctx, PON_MASTER_DEVICE,
					    "qos_class_select_spcp_dscp",
					    true);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(pon_net_ethtool_priv_flag_set,
			       ret);
		goto err;
	}

	ret = netlink_bridge_create(ctx->netlink, PON_IFNAME_BRIDGE_MULTICAST,
				    NULL);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_bridge_create, ret);
		goto err;
	}

	ret = netlink_netdevice_state_set(ctx->netlink,
					  PON_IFNAME_BRIDGE_MULTICAST, 1);
	if (ret != PON_ADAPTER_SUCCESS) {
		dbg_err_fn_ret(netlink_netdevice_state_set, ret);
		goto err;
	}

	err = netlink_cache_refill(ctx->netlink);
	if (err) {
		ret = PON_ADAPTER_ERROR;
		dbg_err_fn_ret(netlink_cache_refill, 0);
		goto err;
	}

	ctx->vab = pon_net_vlan_aware_bridging_create();
	if (!ctx->vab) {
		ret = PON_ADAPTER_ERROR;
		dbg_err_fn_ret(pon_net_vlan_aware_bridging_create, 0);
		goto err;
	}

	ret = pon_net_if_event_init(ctx);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_wrn_fn_ret(pon_net_if_event_init, ret);

	dbg_naked(DBG_MSG, "started\n");

err:
	dbg_out_ret("%d", ret);
	return ret;
}

static enum pon_adapter_errno pon_net_shutdown(void *ll_handle)
{
	struct pon_net_context *ctx = ll_handle;
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;

	dbg_in_args("%p", ll_handle);

	ret = pon_net_if_event_stop(ctx);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_wrn_fn_ret(pon_net_if_event_stop, ret);

	if (ctx->ioctl_fd > 0)
		closesocket(ctx->ioctl_fd);

	if (ctx->vab)
		pon_net_vlan_aware_bridging_destroy(ctx->vab, ctx);

	pon_net_me_list_exit(&ctx->me_list);
	pon_net_vlan_flow_list_exit(ctx, &ctx->vlan_flow_list);

	netlink_netdevice_destroy(ctx->netlink, PON_IFNAME_BRIDGE_MULTICAST);

	/* If 'pon0' was created, unconfigure it */
	ret = pon_net_master_dev_exit(ctx);
	if (ret != PON_ADAPTER_SUCCESS)
		dbg_wrn_fn_ret(pon_net_master_dev_exit, ret);
	if (ctx->cfg.enable_cpu_queues) {
		ret = pon_net_tc_queues_exit(ctx);
		if (ret != PON_ADAPTER_SUCCESS)
			dbg_wrn_fn_ret(pon_net_tc_queues_exit, ret);
		pon_net_pon_handles_free(ctx);
	}
	pon_net_iphost_exit(ctx);

	if (pon_net_pa_mapper_shutdown(ctx))
		dbg_err_fn(pon_net_pa_mapper_shutdown);

	if (ctx->netlink) {
		pon_net_ext_switch_exit(ctx);
		pon_net_omci_channel_exit(ctx);
	}

	netlink_destroy(ctx->netlink);

	if (ctx->db)
		pon_net_dev_db_destroy(ctx->db);

	dbg_naked(DBG_MSG, "shutdown\n");
	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno get_lanports(void *ll_handle,
					   uint32_t *lanports_num)
{
	struct pon_net_context *ctx = ll_handle;

	dbg_in_args("%p", ll_handle);
	dbg_out_ret("%u", ctx->cfg.max_ports);

	if (!lanports_num)
		return PON_ADAPTER_ERR_PTR_INVALID;

	*lanports_num = ctx->cfg.max_ports;

	return PON_ADAPTER_SUCCESS;
}

static const struct pa_system_cap_ops pon_net_sys_cap_ops = {
	.get_lanports = get_lanports,
};

static const struct pa_system_ops system_ops = {
	.init = pon_net_init,
	.start = pon_net_start,
	.reboot = NULL,
	.shutdown = pon_net_shutdown,
};

static const struct pa_omci_me_ops omci_me_ops = {
	.gem_itp = &gem_itp_ops,
	.gem_port_net_ctp = &gem_port_net_ctp_ops,
	.mac_bp_config_data = &mac_bp_config_data_ops,
	.mac_bridge_service_profile = &mac_bridge_service_profile_ops,
	.mac_bp_table_data = &mac_bp_bridge_table_data_ops,
	.mac_bp_filter_preassign_table = &mac_bp_filter_preassign_table_ops,
	.mc_gem_itp = &mc_gem_itp_ops,
	.mc_profile = &mc_profile_ops,
	.pptp_eth_uni = &pptp_eth_uni_ops,
	.pptp_lct_uni = &pptp_lct_uni_ops,
	.tcont = &tcont_ops,
	.traffic_descriptor = &traffic_descriptor_ops,
	.virtual_ethernet_interface_point =
		&virtual_ethernet_interface_point_ops,
	.vlan_tag_filter_data = &vlan_tagging_filter_data_ops,
	.vlan_tag_oper_cfg_data = &vlan_tagging_operation_config_data_ops,
	.dot1p_mapper = &pon_net_dot1p_mapper_ops,
	.ext_vlan = &ext_vlan_ops,
	.vendor_emop = &vendor_emop_ops,
	.priority_queue = &priority_queue_ops,
	.traffic_scheduler = &traffic_scheduler_ops,
	.gal_eth_profile = &gal_eth_profile_ops,
	.onu2_g = &pon_net_onu2_g_ops,
	.onu_g = &pon_net_onu_g_ops,
	.ip_host = &ip_host_ops,
	.eth_pmhd = &eth_pmhd_ops,
};

static const struct pa_ops pon_net_pa_ops = {
	.system_ops = &system_ops,
	.msg_ops = &msg_ops,
	.omci_me_ops = &omci_me_ops,
	.omci_mcc_ops = &pon_net_omci_mcc_ops,
	.vlan_flow_ops = &pon_net_vlan_flow_ops,
	.dbg_lvl_ops = &libponnet_dbg_lvl_ops,
	.sys_cap_ops = &pon_net_sys_cap_ops,
};

enum pon_adapter_errno libponnet_ll_register_ops(void *hl_handle_legacy,
						 const struct pa_ops **pa_ops,
						 void **ll_handle,
						 void *hl_handle,
						 uint32_t if_version)
{
	enum pon_adapter_errno ret = PON_ADAPTER_ERROR;

	dbg_in_args("%p, %p, %p, %p, %d",
		    hl_handle_legacy, pa_ops, ll_handle, hl_handle, if_version);

	/* In legacy mode, set the new arguments to compatible values. */
	if (hl_handle_legacy) {
		hl_handle = hl_handle_legacy;
		if_version = PA_IF_1ST_VER_NUMBER;
	}

	if (PA_IF_VERSION_CHECK_COMPATIBLE(if_version)) {
		g_pon_net_context.hl_handle = hl_handle;
		*pa_ops = &pon_net_pa_ops;
		*ll_handle = &g_pon_net_context;
		ret = PON_ADAPTER_SUCCESS;
	}

	dbg_out_ret("%d", ret);
	return ret;
}
