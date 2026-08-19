/******************************************************************************
 *
 * Copyright (c) 2022 - 2025 MaxLinear, Inc.
 * Copyright (c) 2016 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include "pon_config.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <time.h>

#ifdef UBUS_ENABLE
#include <libubus.h>
#endif

#include "fapi_pon_os.h"
#include "fapi_pon.h"
#include "fapi_pon_error.h"
#include "fapi_pon_alarms.h"

#ifdef EXTRA_VERSION
#define pon_extra_ver_str "." EXTRA_VERSION
#else
#define pon_extra_ver_str ""
#endif

/** what string support, version string */
const char pon_whatversion[] =
	"@(#)MaxLinear PON library daemon, Version " PACKAGE_VERSION
		pon_extra_ver_str;

static bool listen = true;

struct gtc_xgtc_ploam_message {
	uint32_t msg_type_id;
	const char *msg_type_text;
};

struct ploam_state_reason_mapping {
	uint32_t codepoint;
	const char *text;
};

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))
#endif

#define CONFIG_FLAG_MAC_SA	(1 << 0)
#define CONFIG_FLAG_MAC_DA	(1 << 1)
#define CONFIG_FLAG_SERIAL	(1 << 2)
#define CONFIG_FLAG_REG_ID	(1 << 3)
#define CONFIG_FLAG_AON		(1 << 4)
#define CONFIG_FLAG_PON		(1 << 5)

#define FLAG_IS_SET(var, flag) (((var) & (flag)) == (flag))

/* This value is valid as of 2017-01-01 and will change in the future,
 * not before 2020-01-01.
 * Check https://www.iers.org/SharedDocs/News/EN/BulletinC.html.
 */
#define TAI_TO_UTC 37

/* Optical transmission in AON operation mode is enabled */
#define PON_AON_CONFIG_LASER_ON 1

/* stores configuration of pond */
struct pond_config {
	/** PON IP MAC address.
	Used as MAC source address in OMCC downstream
	and as MAC destination address in OMCC upstream. */
	uint8_t mac_sa[6];
	/** SoC MAC address
	Used as MAC destination address in OMCC downstream
	and as MAC source address in OMCC upstream. */
	uint8_t mac_da[6];
	/** Ethertype value, used in both directions. */
	uint16_t ethertype;
	/** Protocol value, used in both directions. */
	uint8_t protocol[5];
	/** Serial number. */
	uint8_t serial_no[8];
	/** Registration ID. */
	uint8_t reg_id[36];
	/** ONU identifier */
	uint32_t ident;
	/** Transmitter enable signal polarity */
	uint32_t aon_pol;
	/** Flags for enabled config params */
	uint32_t flags;
	/** activate more logging like PLOAM logging */
	bool verbose;
	struct pon_ctx *fapi_ctx;
};

/* See G.984.3 section 9.2.2 */
static const struct gtc_xgtc_ploam_message gtc_ploam_message_us[] = {
	{1, "Serial number ONU"},
	{2, "Password"},
	{3, "Dying gasp (DG)"},
	{4, "No message"},
	{5, "Encryption key"},
	{6, "Physical equipment error (PEE)"},
	{7, "PON section trace (PST)"},
	{8, "Remote error indication (REI)"},
	{9, "Acknowledge (ACK)"},
	{10, "Sleep request"}
};

/* See G.984.3 section 9.2.1 */
static const struct gtc_xgtc_ploam_message gtc_ploam_message_ds[] = {
	{1, "Upstream overhead"},
	{3, "Assign ONU ID"},
	{4, "Ranging time"},
	{5, "Deactivate ONU ID"},
	{6, "Disable serial number"},
	{8, "Encrypted port ID"},
	{9, "Request password"},
	{10, "Assign alloc ID"},
	{11, "No message"},
	{12, "Popup"},
	{13, "Request key"},
	{14, "Configure port ID"},
	{15, "Physical equipment error (PEE)"},
	{16, "Change power level (CPL)"},
	{17, "PON section trace (PST)"},
	{18, "BER interval"},
	{19, "Key switching time"},
	{20, "Extended burst length"},
	{21, "PON ID"},
	{22, "Swift Popup"},
	{23, "Ranging adjustment"},
	{24, "Sleep allow"}
};

/* See G.9807.1 section C.11.3.2 and G.989.3 section 11.3.4 */
static const struct gtc_xgtc_ploam_message xgtc_ploam_message_us[] = {
	{1, "Serial number ONU"},
	{2, "Registration"},
	{5, "Key report"},
	{9, "Acknowledge (ACK)"},
	{16, "Sleep request"},
	{26, "Tuning response"},
	{27, "Power consumption report"},
	{28, "Rate response"}
};

/* See G.9807.1 section C.11.3.1 and G.989.3 section 11.3.3 */
static const struct gtc_xgtc_ploam_message xgtc_ploam_message_ds[] = {
	{1, "Burst profile"},
	{3, "Assign ONU ID"},
	{4, "Ranging time"},
	{5, "Deactivate ONU ID"},
	{6, "Disable serial number"},
	{9, "Request registration"},
	{10, "Assign alloc ID"},
	{13, "Key control"},
	{18, "Sleep allow"},
	{19, "Calibration request"},
	{20, "Adjust tx wavelength"},
	{21, "Tuning control"},
	{23, "System profile"},
	{24, "Channel profile"},
	{25, "Protection control"},
	{26, "Change power level"},
	{27, "Power consumption Inquire"},
	{28, "Rate control"}
};

static const struct ploam_state_reason_mapping ploam_state_reason_to_text[] = {
	{0, "Timeout TOZ - Discovery timer"},
	{1, "Timeout TO1 - Ranging timer"},
	{2, "Timeout TO2 - Loss of downstream synchronization (LODS) timer"},
	{3, "Timeout TO3 - LODS protection timer"},
	{4, "Timeout TO4 - Downstream tuning timer"},
	{5, "Timeout TO5 - Upstream tuning timer"},
	{6, "Timeout TO6 - Forgotten ONU timer"},
	{7, "Timeout TProfileDwell timer"},
	{8, "Reserved"},
	{9, "Timeout Channel partition index timer"},
	{65535, "Other"}
};

static const char *pon_gtc_ploam_log_message_type_get(uint32_t message_type_id,
						      uint32_t direction)
{
	unsigned int i;

	if (direction == PON_US) {
		for (i = 0; i < ARRAY_SIZE(gtc_ploam_message_us); i++) {
			if (gtc_ploam_message_us[i].msg_type_id
			    == message_type_id)
				return gtc_ploam_message_us[i].msg_type_text;
		}
	} else {
		for (i = 0; i < ARRAY_SIZE(gtc_ploam_message_ds); i++) {
			if (gtc_ploam_message_ds[i].msg_type_id
			    == message_type_id)
				return gtc_ploam_message_ds[i].msg_type_text;
		}
	}

	fprintf(stderr,	"Unknown GTC PLOAM message id: %u, dir: %u\n",
		message_type_id, direction);
	return "Unknown PLOAM message";
}

static const char *pon_xgtc_ploam_log_message_type_get(uint32_t message_type_id,
						       uint32_t direction)
{
	unsigned int i;

	if (direction == PON_US) {
		for (i = 0; i < ARRAY_SIZE(xgtc_ploam_message_us); i++) {
			if (xgtc_ploam_message_us[i].msg_type_id
			    == message_type_id)
				return xgtc_ploam_message_us[i].msg_type_text;
		}
	} else {
		for (i = 0; i < ARRAY_SIZE(xgtc_ploam_message_ds); i++) {
			if (xgtc_ploam_message_ds[i].msg_type_id
			    == message_type_id)
				return xgtc_ploam_message_ds[i].msg_type_text;
		}
	}

	fprintf(stderr,	"Unknown XGTC PLOAM message id: %u, dir: %u\n",
		message_type_id, direction);
	return "Unknown PLOAM message";
}

static void pond_get_xgtc_log(void *priv,
				const struct pon_xgtc_ploam_message *xgtc_log)
{
	unsigned int i;

	if (xgtc_log->direction == PON_US) {
		printf("ploam us: onu_id - %u / %s :\nploam us| ",
		       xgtc_log->onu_id,
		       pon_xgtc_ploam_log_message_type_get(
						xgtc_log->message_type_id,
						xgtc_log->direction));
		for (i = 0; i < 36; i++)
			printf("%02x ", xgtc_log->message[i]);
		printf("| %x\n", xgtc_log->time_stamp);
	} else {
		printf("ploam ds: onu_id - %u / %s :\nploam ds| ",
		       xgtc_log->onu_id,
		       pon_xgtc_ploam_log_message_type_get(
						xgtc_log->message_type_id,
						xgtc_log->direction));
		for (i = 0; i < 36; i++)
			printf("%02x ", xgtc_log->message[i]);
		printf("| %x\n", xgtc_log->time_stamp);
	}
}

static void pond_get_gtc_log(void *priv,
				const struct pon_gtc_ploam_message *gtc_log)
{
	unsigned int i;

	if (gtc_log->direction == PON_US) {
		printf("ploam us: onu_id - %u / %s :\nploam us| ",
		       gtc_log->onu_id,
		       pon_gtc_ploam_log_message_type_get(
						gtc_log->message_type_id,
						gtc_log->direction));
		for (i = 0; i < 10; i++)
			printf("%02x ", gtc_log->message[i]);
		printf("| %x\n", gtc_log->time_stamp);
	} else {
		printf("ploam ds: onu_id - %u / %s :\nploam ds| ",
		       gtc_log->onu_id,
		       pon_gtc_ploam_log_message_type_get(
						gtc_log->message_type_id,
						gtc_log->direction));
		for (i = 0; i < 10; i++)
			printf("%02x ", gtc_log->message[i]);
		printf("| %x\n", gtc_log->time_stamp);
	}
}

static const char *ploam_state_reason_get(uint32_t reason)
{
	size_t i;

	for (i = 0; i < ARRAY_SIZE(ploam_state_reason_to_text); i++) {
		if (ploam_state_reason_to_text[i].codepoint == reason)
			return ploam_state_reason_to_text[i].text;
	}

	return "Unknown PLOAM_STATE Reason";
}

static void pond_get_ploam_state(void *priv,
				 const struct pon_ploam_state_evt *ploam_state)
{
	printf("ploam state: previous - %u, current - %u, change reason - %u [%s]\n",
		ploam_state->previous, ploam_state->current,
		ploam_state->change_reason,
		ploam_state_reason_get(ploam_state->change_reason));
}

static int act_alarm_level_print(void *ctx, const struct alarm_type *alarm,
				 void *data)
{
	const struct pon_alarm_status *status = data;

	if (status->alarm_id == alarm->code) {
		printf("alarm %s set\n", alarm->desc);
		/* tell iterator to stop looping */
		return 1;
	}
	return 0;
}

static int act_alarm_edge_print(void *ctx, const struct alarm_type *alarm,
				void *data)
{
	const struct pon_alarm_status *status = data;

	if (status->alarm_id == alarm->code) {
		printf("alarm %s triggered\n", alarm->desc);
		/* tell iterator to stop looping */
		return 1;
	}
	return 0;
}

static int clr_alarm_level_print(void *ctx, const struct alarm_type *alarm,
				 void *data)
{
	const struct pon_alarm_status *status = data;

	if (status->alarm_id == alarm->code) {
		printf("alarm %s cleared\n", alarm->desc);
		/* tell iterator to stop looping */
		return 1;
	}
	return 0;
}

static void
pond_print_active_alarms(void *priv, const struct pon_alarm_status *alarms)
{
	fapi_pon_visit_alarms_level(priv, act_alarm_level_print,
				    (void *)alarms);

	fapi_pon_visit_alarms_edge(priv, act_alarm_edge_print,
				   (void *)alarms);
}

static void
pond_print_inactive_alarms(void *priv, const struct pon_alarm_status *alarms)
{
	fapi_pon_visit_alarms_level(priv, clr_alarm_level_print,
				    (void *)alarms);
}

static enum fapi_pon_errorcode pond_get_xgtc_power_level(void *priv,
							 uint32_t oper,
							 uint32_t *attenuation)
{
	printf("xgtc power level: operation type - %u, attenuation lvl - %u\n",
	       oper, *attenuation);

	return PON_STATUS_OK_NO_RESPONSE;
}

static enum fapi_pon_errorcode
pond_get_onu_tod_sync(void *priv, struct pon_gpon_tod_sync *onu_tod_sync)
{
	printf("onu tod sync: multiframe count - %u, tod seconds - %u, tod extended seconds - %u, tod nano seconds - %u, tod offset pico seconds - %d, tod quality - %d\n",
	       onu_tod_sync->multiframe_count,
	       onu_tod_sync->tod_seconds,
	       onu_tod_sync->tod_extended_seconds,
	       onu_tod_sync->tod_nano_seconds,
	       onu_tod_sync->tod_offset_pico_seconds,
	       onu_tod_sync->tod_quality);

	return PON_STATUS_OK_NO_RESPONSE;
}

static enum fapi_pon_errorcode
pond_get_onu_tod_sync_output(void *priv,
	struct pon_gpon_tod_sync *onu_tod_sync)
{
	struct tm tm_time;
	time_t time = (time_t)onu_tod_sync->tod_seconds - TAI_TO_UTC;
	uint8_t checksum = 0;
	char buffer[87];
	int i;

	gmtime_r(&time, &tm_time);

	snprintf(buffer, sizeof(buffer),
		 "$GPZDA,%02d%02d%02d.00,%02d,%02d,%04d,00,00*",
		 tm_time.tm_hour,
		 tm_time.tm_min,
		 tm_time.tm_sec,
		 tm_time.tm_mday,
		 tm_time.tm_mon + 1,
		 tm_time.tm_year + 1900);

	for (i = 1; i < strnlen_s(buffer, sizeof(buffer)) - 1; i++)
		checksum ^= buffer[i];

	printf("%s%02X\n", buffer, checksum);

	return PON_STATUS_OK_NO_RESPONSE;
}

static enum fapi_pon_errorcode
pond_twdm_wl_check(void *priv, const enum pon_twdm_oper_type oper_type,
		   uint8_t ch_id, bool execute)
{
	printf("twdm_wl_check: oper_type - %d, ch_id - %u, execute - %d\n",
	       oper_type, ch_id, execute);

	return PON_STATUS_OK_NO_RESPONSE;
}

static enum fapi_pon_errorcode
pond_twdm_wl_tuning(void *priv)
{
	printf("twdm_wl_tuning:\n");

	return PON_STATUS_OK_NO_RESPONSE;
}

static enum fapi_pon_errorcode
pond_twdm_config(void *priv, uint8_t cpi, uint8_t dwlch_id)
{
	printf("twdm config: cpi - %u, dwlch_id - %u\n",
	       cpi, dwlch_id);

	return PON_STATUS_OK_NO_RESPONSE;
}

static void pond_print_synce_status(void *priv,
				const struct pon_synce_status *synce_status)
{
	printf("synce status: stat - %u\n", synce_status->stat);
}

static void
pond_print_onu_rnd_chl_tbl(void *priv,
			const struct pon_generic_auth_table *onu_rnd_chl_tbl)
{
	int i;

	printf("onu_rnd_chl_tbl received\n");
	for (i = 0; i < onu_rnd_chl_tbl->size; i++)
		printf("0x%x ", onu_rnd_chl_tbl->table[i]);
	printf("\n");
}

static void
pond_print_onu_auth_res_tbl(void *priv,
			const struct pon_generic_auth_table *onu_auth_res_tbl)
{
	int i;

	printf("onu_auth_res_tbl received\n");
	for (i = 0; i < onu_auth_res_tbl->size; i++)
		printf("0x%x ", onu_auth_res_tbl->table[i]);
	printf("\n");
}

static enum fapi_pon_errorcode pond_print_unlink_all(void *priv)
{
	printf("unlink all\n");

	return PON_STATUS_OK_NO_RESPONSE;
}

/* Currently we are doing the testing with the OpenWrt ubus method
 * internally, the non OpenWrt ubus version is not fully supported.
 */
#ifdef UBUS_ENABLE
static int pond_activate_aon_bridge(struct pond_config *cfg, bool up)
{
	uint32_t id;
	int err;
	const char *method;
	struct ubus_context *ubus_ctx;

	ubus_ctx = ubus_connect(NULL);
	if (!ubus_ctx) {
		fprintf(stderr, "%s: ubus_connect failed\n", __func__);
		return -EIO;
	}

	err = ubus_lookup_id(ubus_ctx, "network.interface.bridge", &id);
	if (err) {
		fprintf(stderr, "%s: ubus_lookup_id failed\n", __func__);
		goto out_free;
	}

	if (up)
		method = "up";
	else
		method = "down";

	err = ubus_invoke(ubus_ctx, id, method, NULL, NULL, NULL, 1000);
	if (err) {
		fprintf(stderr, "%s: ubus_invoke failed\n", __func__);
		goto out_free;
	}

out_free:
	ubus_free(ubus_ctx);
	return err;
}
#else /* UBUS_ENABLE */
static int pond_activate_aon_bridge(struct pond_config *cfg, bool up)
{
	return 0;
}
#endif /* UBUS_ENABLE */

/*
 * This function is called when the firmware was successfully loaded,
 * also after a reset done by some other program.
 */
static enum fapi_pon_errorcode pond_fw_init_complete(void *priv)
{
	struct pond_config *cfg = priv;
	struct pon_omci_cfg omci_cfg = {0};
	struct pon_gpon_cfg onu_cfg = {0};
	struct pon_optic_cfg optic_cfg = {0};
	struct pon_aon_cfg aon_cfg = {0};
	enum fapi_pon_errorcode ret = PON_STATUS_OK;
	int err;

	if (memcpy_s(omci_cfg.mac_sa, sizeof(omci_cfg.mac_sa),
		     cfg->mac_sa, sizeof(cfg->mac_sa))) {
		fprintf(stderr, "%s: memcpy_s failed\n", __func__);
		return PON_STATUS_MEMCPY_ERR;
	}
	if (memcpy_s(omci_cfg.mac_da, sizeof(omci_cfg.mac_da),
		     cfg->mac_da, sizeof(cfg->mac_da))) {
		fprintf(stderr, "%s: memcpy_s failed\n", __func__);
		return PON_STATUS_MEMCPY_ERR;
	}
	omci_cfg.ethertype = cfg->ethertype;
	if (memcpy_s(omci_cfg.protocol, sizeof(omci_cfg.protocol),
		     cfg->protocol, sizeof(cfg->protocol))) {
		fprintf(stderr, "%s: memcpy_s failed\n", __func__);
		return PON_STATUS_MEMCPY_ERR;
	}
	if (memcpy_s(onu_cfg.serial_no, PON_SERIAL_NO_SIZE,
		     cfg->serial_no, sizeof(cfg->serial_no))) {
		fprintf(stderr, "%s: memcpy_s failed\n", __func__);
		return PON_STATUS_MEMCPY_ERR;
	}
	if (memcpy_s(onu_cfg.reg_id, PON_REG_ID_SIZE,
		     cfg->reg_id, sizeof(cfg->reg_id))) {
		fprintf(stderr, "%s: memcpy_s failed\n", __func__);
		return PON_STATUS_MEMCPY_ERR;
	}
	onu_cfg.ident = cfg->ident;
	/* TODO: make this configurable if needed */
	onu_cfg.stop = 0;
	onu_cfg.plev_cap = 0;
	onu_cfg.ploam_timeout_0 =     0;
	onu_cfg.ploam_timeout_1 =  10000;	/*   10   seconds */
	onu_cfg.ploam_timeout_2 =    100;	/*    0.1 seconds */
	/* TODO: TO3 needs to be adapted for NG-PON2 operation (TWDM) */
	onu_cfg.ploam_timeout_3 = 0xffff;	/*   65.5 seconds */

	/* FIXME:
	 * This is the default configuration for a given hardware configuration
	 * (for early lab testing). Shall be updated by the software from
	 * a system configuration data base.
	 *
	 * host write 0x2 0x000A0806 (C1)
	 * host write 0x2 0x00000004 (I1)
	 * host write 0x2 0x00000000 (I2)
	 * host write 0x2 0x00000000 (I3)
	 * host write 0x2 0x00000040 (I4)
	 * host write 0x2 0x0000FFFF (I5)
	 * host write 0x2 0x00000000 (I6)
	 */

	/*
	 * We need to use GPON_CLOCK_CYCLE
	 * as multiplier here.
	 */
	optic_cfg.laser_setup_time = 0x0 * GPON_CLOCK_CYCLE;
	optic_cfg.laser_hold_time = 0x4 * GPON_CLOCK_CYCLE;
	optic_cfg.serdes_setup_time = 0x0 * GPON_CLOCK_CYCLE;
	optic_cfg.serdes_hold_time = 0x0 * GPON_CLOCK_CYCLE;
	optic_cfg.bias_setup_time = 0x0 * GPON_CLOCK_CYCLE;
	optic_cfg.bias_hold_time = 0x40 * GPON_CLOCK_CYCLE;
	optic_cfg.burst_idle_pattern = 0x0000FFFF;
	optic_cfg.burst_en_mode = 0x0;

	if (cfg->verbose)
		ret = fapi_pon_ploam_log_enable(cfg->fapi_ctx);
	else
		ret = fapi_pon_ploam_log_disable(cfg->fapi_ctx);
	/* Ignore error when mode does not support PLOAM */
	if (ret != PON_STATUS_OK && ret != PON_STATUS_OPERATION_MODE_ERR) {
		fprintf(stderr, "setting PLOAM logging failed: %i\n",
			ret);
		return ret;
	}

	/* if no config option was set, exit early */
	if (cfg->flags == 0)
		return PON_STATUS_OK;

	if (FLAG_IS_SET(cfg->flags, CONFIG_FLAG_AON)) {
		aon_cfg.tx_en_pol = cfg->aon_pol;
		ret = fapi_pon_aon_cfg_set(cfg->fapi_ctx, &aon_cfg);
		if (ret != PON_STATUS_OK) {
			fprintf(stderr,
				"setting AON Configuration failed: %i\n", ret);
			return ret;
		}

		ret = fapi_pon_aon_tx_enable(cfg->fapi_ctx);
		if (ret != PON_STATUS_OK) {
			fprintf(stderr, "enabling optical transmission in AON operation mode failed: %i\n",
				ret);
			return ret;
		}
		err = pond_activate_aon_bridge(cfg, true);
		if (err)
			return PON_STATUS_ERR;
	}

	if (FLAG_IS_SET(cfg->flags, CONFIG_FLAG_PON)) {
		ret = fapi_pon_optic_cfg_set(cfg->fapi_ctx, &optic_cfg);
		if (ret != PON_STATUS_OK) {
			fprintf(stderr,
				"setting optic Configuration failed: %i\n",
				ret);
			return ret;
		}

		if (FLAG_IS_SET(cfg->flags,
				CONFIG_FLAG_MAC_DA | CONFIG_FLAG_MAC_SA)) {
			ret = fapi_pon_omci_cfg_set(cfg->fapi_ctx, &omci_cfg);
			if (ret != PON_STATUS_OK) {
				fprintf(stderr,
					"setting OMCI Encapsulation Configuration failed: %i\n",
					ret);
				return ret;
			}
		}

		if (FLAG_IS_SET(cfg->flags,
				CONFIG_FLAG_SERIAL | CONFIG_FLAG_REG_ID)) {
			ret = fapi_pon_gpon_cfg_set(cfg->fapi_ctx, &onu_cfg);
			if (ret != PON_STATUS_OK)
				fprintf(stderr,
					"setting XGTC ONU Configuration failed: %i\n",
					ret);
		}
	}

	return ret;
}

static void pond_sighandler(int sig)
{
	listen = false;
}

static const struct option long_options[] = {
	{"aon_mode",	required_argument,	0, 'a'},
	{"reset",	required_argument,	0, 'r'},
	{"help" ,	no_argument,		0, 'h'},
	{"mac_sa" ,	required_argument,	0, 's'},
	{"mac_da" ,	required_argument,	0, 'd'},
	{"serial" ,	required_argument,	0, 'n'},
	{"regid" ,	required_argument,	0, 'i'},
	{"identifier",	required_argument,	0, 'o'},
	{"tod only",	no_argument,		0, 't'},
	{"verbose",	no_argument,		0, 'v'},
	{"mode",	required_argument,	0, 'm'},
	{NULL,		0,			0,  0 },
};

static void print_help(char *prog)
{
	int i;

	printf("%s: Pon daemon\n", prog);
	for (i = 0; long_options[i].name != NULL; i++) {
		printf("--%-10s  -%c  %s\n",
			long_options[i].name,
			long_options[i].val,
			(long_options[i].has_arg == required_argument)
			? "<value>" : "");
	}
}

static enum fapi_pon_errorcode pond_parse_mac(uint8_t mac[6])
{
	int ret;
	unsigned int values[6];
	int i;
	char rest;

	/*
	 * Parse the mac address, if there is something remaining after the mac
	 * address rest will get filled and this will return 7.
	 */
	ret = sscanf_s(optarg, "%x:%x:%x:%x:%x:%x%c",
		     &values[0], &values[1], &values[2],
		     &values[3], &values[4], &values[5], SSCANF_CHAR(&rest));
	if (ret != 6)
		return PON_STATUS_ERR;

	for (i = 0; i < 6; i++)
		mac[i] = (uint8_t)values[i];

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode pond_parse_hex(uint8_t *hex, size_t hex_size,
					      char *string)
{
	size_t i;
	char *nptr = string;
	char *endptr;

	for (i = 0; i < hex_size; i++) {
		errno = 0;
		hex[i] = (uint8_t)strtoul(nptr, &endptr, 0);
		if (errno || nptr == endptr) {
			return PON_STATUS_ERR;
		}
		nptr = endptr;
	}

	/* check if there are any remaining chars in the string */
	while (*nptr) {
		if (!isspace((int)*nptr))
			return PON_STATUS_ERR;
		nptr++;
	}

	return PON_STATUS_OK;
}

static enum fapi_pon_errorcode pond_parse_serial(uint8_t *serial, char *string)
{
	int ret;
	unsigned int values[4];
	int i;
	char rest;

	/*
	 * Parse the mac address, if there is something remaining after the mac
	 * address rest will get filled and this will return 7.
	 */
	ret = sscanf_s(optarg, "%c%c%c%c%02X%02X%02X%02X%c",
		     &serial[0], &serial[1], &serial[2], &serial[3],
		     &values[0], &values[1], &values[2], &values[3],
		     SSCANF_CHAR(&rest));
	if (ret != 8)
		return PON_STATUS_ERR;

	for (i = 0; i < 4; i++)
		serial[i + 4] = (uint8_t)values[i];

	return PON_STATUS_OK;
}

int main(int argc, char *argv[])
{
	int opt;
	int option_index;
	struct sigaction sig_action = {0,};
	enum fapi_pon_errorcode ret;
	enum pon_mode pon_mode = PON_MODE_UNKNOWN;
	bool reset = false,  tod_only = false;
	struct pond_config cfg = {
		.aon_pol = 0,
		.mac_sa = {0,},
		.mac_da = {0,},
		.ethertype = 0x88b7,
		.protocol = {0x0, 0x19, 0xA7, 0x0, 0x2},
		.ident = 0,
		.verbose = false,
	};

	/* attempt to set both stdout and stderr to unbuffered mode! */
	if (setvbuf(stdout, NULL, _IONBF, 0))
		perror("Attempt to set stdout to unbuffered mode has failed");
	if (setvbuf(stderr, NULL, _IONBF, 0))
		perror("Attempt to set stderr to unbuffered mode has failed");

	while ((opt = getopt_long(argc, argv, "a:r:hs:d:n:i:o:tvm:",
				  long_options, &option_index)) != -1) {
		switch (opt) {
		case 'a':
			cfg.aon_pol = strtol(optarg, NULL, 0);
			if (errno) {
				fprintf(stderr,	"invalid aon_pol: %s\n",
					optarg);
				return EXIT_FAILURE;
			}
			cfg.flags |= CONFIG_FLAG_AON;
			break;
		case 'm':
			if (strcmp(optarg, "gpon") == 0)
				pon_mode = PON_MODE_984_GPON;
			if (strcmp(optarg, "xgspon") == 0)
				pon_mode = PON_MODE_9807_XGSPON;
			if (strcmp(optarg, "xgpon") == 0)
				pon_mode = PON_MODE_987_XGPON;
			if (strcmp(optarg, "ngpon2_2G5") == 0)
				pon_mode = PON_MODE_989_NGPON2_2G5;
			if (strcmp(optarg, "ngpon2_10G") == 0)
				pon_mode = PON_MODE_989_NGPON2_10G;
			if (strcmp(optarg, "aon") == 0)
				pon_mode = PON_MODE_AON;
			break;
		case 'r':
			reset = true;
			pon_mode = strtol(optarg, NULL, 0);
			break;
		case 's':
			ret = pond_parse_mac(cfg.mac_sa);
			if (ret != PON_STATUS_OK) {
				fprintf(stderr,
					"invalid mac address for mac_sa: %s\n",
					optarg);
				return EXIT_FAILURE;
			}
			cfg.flags |= CONFIG_FLAG_MAC_SA;
			cfg.flags |= CONFIG_FLAG_PON;
			break;
		case 'd':
			ret = pond_parse_mac(cfg.mac_da);
			if (ret != PON_STATUS_OK) {
				fprintf(stderr,
					"invalid mac address for mac_da: %s\n",
					optarg);
				return EXIT_FAILURE;
			}
			cfg.flags |= CONFIG_FLAG_MAC_DA;
			cfg.flags |= CONFIG_FLAG_PON;
			break;
		case 'n':
			ret = pond_parse_serial(cfg.serial_no, optarg);
			if (ret != PON_STATUS_OK) {
				fprintf(stderr,	"invalid serial number: %s\n",
					optarg);
				return EXIT_FAILURE;
			}
			cfg.flags |= CONFIG_FLAG_SERIAL;
			cfg.flags |= CONFIG_FLAG_PON;
			break;
		case 'i':
			ret = pond_parse_hex(cfg.reg_id, 36, optarg);
			if (ret != PON_STATUS_OK) {
				fprintf(stderr,	"invalid registration ID: %s\n",
					optarg);
				return EXIT_FAILURE;
			}
			cfg.flags |= CONFIG_FLAG_REG_ID;
			cfg.flags |= CONFIG_FLAG_PON;
			break;
		case 'o':
			cfg.ident = strtol(optarg, NULL, 0);
			if (errno) {
				fprintf(stderr,	"invalid ident: %s\n", optarg);
				return EXIT_FAILURE;
			}
			cfg.flags |= CONFIG_FLAG_PON;
			break;
		case 't':
			tod_only = true;
			break;
		case 'v':
			cfg.verbose = true;
			break;
		case 'h':
			print_help(argv[0]);
			return EXIT_SUCCESS;
		default:
			fprintf(stderr, "invalid parameters\n");
			print_help(argv[0]);
			return EXIT_FAILURE;
		}
	}
	if (cfg.flags & CONFIG_FLAG_PON &&
	    cfg.flags & CONFIG_FLAG_AON) {
		fprintf(stderr, "Only PON *or* AON mode possible\n");
		return EXIT_FAILURE;
	}

	sig_action.sa_handler = &pond_sighandler;
	sigaction(SIGINT, &sig_action, NULL);

	ret = fapi_pon_open(&cfg.fapi_ctx);
	if (ret != PON_STATUS_OK) {
		fprintf(stderr, "creating pon context failed\n");
		return EXIT_FAILURE;
	}

	ret = fapi_pon_listener_connect(cfg.fapi_ctx, &cfg);
	if (ret != PON_STATUS_OK) {
		fprintf(stderr, "connecting event listener failed\n");
		return EXIT_FAILURE;
	}

	if (tod_only == false) {
		fapi_pon_register_xgtc_log(cfg.fapi_ctx, pond_get_xgtc_log);
		fapi_pon_register_gtc_log(cfg.fapi_ctx, pond_get_gtc_log);
		fapi_pon_register_ploam_state(cfg.fapi_ctx,
					      pond_get_ploam_state);
		fapi_pon_register_alarm_report(cfg.fapi_ctx,
					       pond_print_active_alarms);
		fapi_pon_register_alarm_clear(cfg.fapi_ctx,
					      pond_print_inactive_alarms);
		fapi_pon_register_xgtc_power_level(cfg.fapi_ctx,
						   pond_get_xgtc_power_level);
		fapi_pon_register_fw_init_complete(cfg.fapi_ctx,
						   pond_fw_init_complete);
		fapi_pon_register_onu_tod_sync(cfg.fapi_ctx,
					       pond_get_onu_tod_sync);
		fapi_pon_register_twdm_wl_check(cfg.fapi_ctx,
						pond_twdm_wl_check);
		fapi_pon_register_twdm_us_wl_tuning(cfg.fapi_ctx,
						    pond_twdm_wl_tuning);
		fapi_pon_register_twdm_config(cfg.fapi_ctx,
					      pond_twdm_config);
		fapi_pon_register_synce_status(cfg.fapi_ctx,
					       pond_print_synce_status);
		fapi_pon_register_onu_rnd_chl_tbl(cfg.fapi_ctx,
						  pond_print_onu_rnd_chl_tbl);
		fapi_pon_register_onu_auth_res_tbl(cfg.fapi_ctx,
						   pond_print_onu_auth_res_tbl);
		fapi_pon_register_unlink_all(cfg.fapi_ctx,
					     pond_print_unlink_all);
	} else {
		fapi_pon_register_onu_tod_sync(cfg.fapi_ctx,
					       pond_get_onu_tod_sync_output);
	}

	/*
	 * Activate ploam logging, but ignore return value here, if
	 * this is done before the FW is loaded, we try it again after
	 * the FW was loaded.
	 */
	if (cfg.verbose)
		fapi_pon_ploam_log_enable(cfg.fapi_ctx);
	else
		fapi_pon_ploam_log_disable(cfg.fapi_ctx);

	if (reset)
		fapi_pon_reset(cfg.fapi_ctx, pon_mode);

	while (listen) {
		ret = fapi_pon_listener_run(cfg.fapi_ctx);
		if (ret != PON_STATUS_OK)
			break;
	}

	fapi_pon_close(cfg.fapi_ctx);

	return EXIT_SUCCESS;
}
