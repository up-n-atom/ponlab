/******************************************************************************
 *
 * Copyright (c) 2022 - 2023 MaxLinear, Inc.
 * Copyright (c) 2017 - 2019 Intel Corporation
 * Copyright (c) 2012 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <top.h>
#include "pon_top.h"
#include "dump.h"
#include "uci_config.h"

#include <getopt.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdio.h>

#include <unistd.h>
#include <libgen.h>

#include <fapi_pon.h>

#ifdef HAVE_CONFIG_H
#include "pon_tools.h"
#endif

#ifdef EXTRA_VERSION
#define pon_extra_ver_str "." EXTRA_VERSION
#else
#define pon_extra_ver_str ""
#endif

/* what string support, version string */
const char pon_top_whatversion[] = "@(#)MaxLinear PON top, version " PACKAGE_VERSION
				   pon_extra_ver_str;

top_page_init_t pontop_page_init[] = {
	table_group_init,
};
unsigned int pontop_page_init_num = ARRAY_SIZE(pontop_page_init);

static int debugfs_get(struct top_context *ctx, const char *name);

#define DEBUGFS(key1, key2, name, debugfs_entry) \
{ key1, key2, name, top_proc_line_get, debugfs_get, NULL, NULL, debugfs_entry },

const struct top_page_desc pontop_page[] = {
	CNT(0, '?', "Help", top_proc_line_get,
		help_get, NULL, NULL)
	CNT(0, 's', "Status", table_entry_get_status,
		table_get_status, NULL, NULL)
	CNT(0, 'c', "Capability and Configuration",
		table_entry_get_cap, table_get_cap, NULL, NULL)
	CNT(0, 'l', "LAN Interface Status & Counters",
		table_entry_get_lan, table_get_lan, NULL, NULL)
	CNT(0, 'w', "Active alarms",
		table_entry_get_alarms, table_get_alarms, NULL, NULL)
	CNT('g', 's', "GEM/XGEM Port Status",
		table_entry_get_gem_port_status,
		table_get_gem_port_status, NULL, NULL)
	CNT('g', 'c', "GEM/XGEM Port Counters",
		table_entry_get_gem_port_counters,
		table_get_gem_port_counters, NULL, NULL)
	CNT('g', 'd', "GEM/XGEM port DS Counters",
		table_entry_get_gem_port_ds_counters,
		table_get_gem_port_ds_counters, NULL, NULL)
	CNT('g', 'u', "GEM/XGEM port US Counters",
		table_entry_get_gem_port_us_counters,
		table_get_gem_port_us_counters, NULL, NULL)
	CNT('e', 'd', "GEM/XGEM port Eth DS Cnts",
		table_entry_get_gem_port_ethernet_ds_counters,
		table_get_gem_port_ethernet_ds_counters, NULL, NULL)
	CNT('e', 'u', "GEM/XGEM port Eth US Cnts",
		table_entry_get_gem_port_ethernet_us_counters,
		table_get_gem_port_ethernet_us_counters, NULL, NULL)
	CNT(0, 'f', "FEC Status & Counters",
		table_entry_get_fec_status,
		table_get_fec_status, NULL, NULL)
	CNT(0, 't', "GTC/XGTC Status & Counters",
		table_entry_get_gtc_status,
		table_get_gtc_status, NULL, NULL)
	CNT('p', 's', "Power Save Status",
		table_entry_get_power_status,
		table_get_power_status, NULL, NULL)
	CNT('p', 'c', "PSM Configuration",
		table_entry_get_psm_cfg,
		table_get_psm_cfg, NULL, NULL)
	CNT('a', 'c', "Allocation Counters",
		table_entry_get_alloc_counters,
		table_get_alloc_counters, NULL, NULL)
	CNT('p', 'd', "PLOAM Downstream Counters",
		table_entry_get_ploam_ds_counters,
		table_get_ploam_ds_counters, NULL, NULL)
	CNT('p', 'u', "PLOAM Upstream Counters",
		table_entry_get_ploam_us_counters,
		table_get_ploam_us_counters, NULL, NULL)
	CNT('o', 's', "Optical Interface Status",
		table_entry_get_optic_status,
		table_get_optic_status, NULL, NULL)
	CNT('o', 'i', "Optical Interface Info",
		table_entry_get_optic_info,
		table_get_optic_info, NULL, NULL)
	CNT('d', 'b', "Debug Burst Profile",
		table_entry_get_debug_burst_profile,
		table_get_debug_burst_profile, NULL, NULL)
	/* Group of TWDM pages "T"
	 * each showing a table of params for all supported wavelengths
	 */
	CNT('T', 's', "TWDM Channel Profile Status",
		table_entry_get_twdm_channel_profile_status,
		table_get_twdm_channel_profile_status, NULL, NULL)
	CNT('T', 'd', "TWDM PLOAM DS Message Counter",
		table_entry_get_twdm_ploam_ds_counters,
		table_get_twdm_ploam_ds_counters, NULL, NULL)
	CNT('T', 'u', "TWDM PLOAM US Message Counter",
		table_entry_get_twdm_ploam_us_counters,
		table_get_twdm_ploam_us_counters, NULL, NULL)
	CNT('T', 'x', "TWDM XGTC Counters",
		table_entry_get_twdm_xgtc_counters,
		table_get_twdm_xgtc_counters, NULL, NULL)
	CNT('T', 'l', "TWDM LODS Counters",
		table_entry_get_twdm_lods_counters,
		table_get_twdm_lods_counters, NULL, NULL)
	CNT('T', 'p', "TWDM Power Level Counters",
		table_entry_get_twdm_pl_counters,
		table_get_twdm_pl_counters, NULL, NULL)
	CNT('T', 't', "TWDM Tuning Control Counters",
		table_entry_get_twdm_tc_counters,
		table_get_twdm_tc_counters, NULL, NULL)
	CNT('T', 'g', "TWDM xGEM-Port Counters",
		table_entry_get_twdm_xgem_port_counters,
		table_get_twdm_xgem_port_counters, NULL, NULL)
	DEBUGFS('C', 'o', "CQM ofsc", "cqm/ofsc")
	DEBUGFS('C', 'q', "CQM Queue Map", "cqm/qid_queue_map")
	DEBUGFS('d', 'p', "Datapath Ports", "dp/port")
	DEBUGFS('d', 'q', "Datapath QOS", "dp/qos")
	DEBUGFS('P', 'b', "PPv4 Buffer MGR HW Stats", "pp/buff_mgr/hw_stats")
	DEBUGFS('P', 'p', "PPv4 QoS Queue PPS", "pp/qos_queues_pps")
	DEBUGFS('P', 's', "PPv4 QoS Queues Stats", "pp/qos_queues_stats")
	DEBUGFS('P', 't', "PPv4 QoS Tree", "pp/qos_tree")
	DEBUGFS('P', 'q', "PPv4 QoS QStats", "pp/qos0/qstat")
};
const unsigned int pontop_page_num = ARRAY_SIZE(pontop_page);

static int g_batch_mode;
static struct top_context pontop_ctx;

enum pon_tx_power_scale pontop_tx_power_scale;

static int debugfs_get(struct top_context *ctx, const char *name)
{
	char tmp[64];

	snprintf(tmp, sizeof(tmp), "/sys/kernel/debug/%s", name);
	return linux_file_read(ctx, tmp);
}

/** Print help */
static void help_print(char const *name)
{
	if (name == NULL)
		name = "pontop";

	printf("%s\n", pon_top_whatversion);

	printf("Usage: %s [options]\n\n"
	       "Options:\n"
	       "\t-b, --batch       Start in `Batch mode`\n"
	       "\t-d, --delay <ms>  Counters update delay\n"
	       "\t-g, --group <grp> Show specified counters upon startup\n",
	       name);

	printf("\t                  Possible group values:\n");

	top_print_groups(&pontop_ctx);

	printf("\n"
	       "\t-h, --help        Print help (this message)\n"
	       "\t-v, --version     Print version information\n");
}

/** Parse command line arguments

   \param[in] argc Arguments count
   \param[in] argv Array of arguments
*/
static int arguments_parse(int argc, char *argv[])
{
	int c;
	int option;
	unsigned int upd_delay;

	static struct option opt_str[] = {
		{ "help", no_argument, 0, 'h' },
		{ "version", no_argument, 0, 'v' },
		{ "batch", no_argument, 0, 'b' },
		{ "delay", required_argument, 0, 'd' },
		{ "group", required_argument, 0, 'g' },
		{ "remote-ip", required_argument, 0, 'r' },
		{ NULL, no_argument, 0, 'd' }
	};

	static const char long_opts[] = "hvbd:g:r:";

	do {
		c = getopt_long(argc, argv, long_opts, opt_str, &option);

		if (c == -1)
			return 0;

		switch (c) {
		case 'h':
			help_print((char *)basename(argv[0]));

			return 1;

		case 'v':
			printf("%s %s\n", (char *)basename(argv[0]),
			       pon_top_whatversion);

			return 1;

		case 'b':
			g_batch_mode = 1;

			break;

		case 'd':
			upd_delay = (unsigned int)atoi(optarg);

			if (upd_delay == 0) {
				fprintf(stderr,
					"Invalid value for option 'd'\n");
				return 1;
			}
			top_upd_delay_set(&pontop_ctx, upd_delay);

			break;

		case 'g':
			if (!top_select_group(&pontop_ctx, optarg)) {
				fprintf(stderr,
					"Invalid value for option 'g'\n");
				return 1;
			}

			break;

		default:
			return 1;
		}
	} while (1);

	return 0;
}

/** Entry point

   \param[in] argc Arguments count
   \param[in] argv Array of arguments
*/
int main(int argc, char *argv[])
{
	struct pon_ctx *fapi_ctx;
	enum fapi_pon_errorcode err;
	int uci_ret;
	char uci_opt[UCI_PARAM_STR_MAX_SIZE];

	err = fapi_pon_open(&fapi_ctx);
	if (err != PON_STATUS_OK)
		return EXIT_FAILURE;

	top_init(&pontop_ctx, &console_top_ops,
		 0,
		 pontop_page, pontop_page_num,
		 pontop_page_init, pontop_page_init_num,
		 1000,
		 NULL,
		 NULL,
		 fapi_ctx);

	uci_ret = uci_config_get("optic", "sfp_eeprom", "dmi", uci_opt);
	if (uci_ret == 0)
		fapi_pon_eeprom_open(fapi_ctx, PON_DDMI_A2, uci_opt);

	uci_ret = uci_config_get("optic", "sfp_eeprom", "serial_id", uci_opt);
	if (uci_ret == 0)
		fapi_pon_eeprom_open(fapi_ctx, PON_DDMI_A0, uci_opt);

	uci_ret = uci_config_get("optic", "common", "tx_power_scale", uci_opt);
	if (uci_ret == 0) {
		if (strcmp(uci_opt, "1") == 0)
			pontop_tx_power_scale = TX_POWER_SCALE_0_2;
		else
			pontop_tx_power_scale = TX_POWER_SCALE_0_1;
	}

	if (arguments_parse(argc, argv))
		goto exit;

	if (g_batch_mode == 1) {
		top_batch(&pontop_ctx, "/tmp/pontop.txt");
	} else {
		top_ui_prepare(&pontop_ctx);
		top_ui_main_loop(&pontop_ctx);
		top_ui_shutdown(&pontop_ctx);
	}

exit:
	top_shutdown(&pontop_ctx);
	fapi_pon_close(fapi_ctx);

	return 0;
}
