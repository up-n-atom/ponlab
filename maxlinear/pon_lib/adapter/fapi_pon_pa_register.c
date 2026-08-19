/*****************************************************************************
 *
 * Copyright (c) 2020 - 2026 MaxLinear, Inc.
 * Copyright (c) 2017 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include <string.h>
#include <ctype.h>
#include <pthread.h>

#ifdef HAVE_CONFIG_H
#include "pon_config.h"
#endif

#include "pon_adapter.h"
#include "pon_adapter_debug.h"
#include "pon_adapter_system.h"
#include "pon_adapter_config.h"
#include "pon_adapter_optic.h"
#include "omci/pon_adapter_msg.h"
#include "omci/pon_adapter_omci.h"

#include "fapi_pon_os.h" /* for SSCANF_CHAR */
#include "fapi_pon_pa_common.h"
#include "fapi_pon_pa_register.h"
#include "fapi_pon_pa_twdm.h"
#include "fapi_pon.h"

uint8_t libpon_dbg_lvl = DBG_ERR;

struct init_option_parser {
	const char *name;
	size_t offset;
	size_t size;
	enum pon_adapter_errno (*parser)(void *val, size_t size,
					 char const *string);
};

#define INIT_OPTION(n, p, o) { \
	.name = n, .parser = p, \
	.offset = offsetof(struct fapi_pon_wrapper_cfg, o), \
	.size = sizeof_member(struct fapi_pon_wrapper_cfg, o) }

static enum pon_adapter_errno parse_mac(void *val, size_t size,
					char const *string)
{
	int ret;
	uint8_t *mac = val;
	unsigned int values[6];
	int i;
	char rest;

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
	if (ret != 6)
		return PON_ADAPTER_ERROR;

	for (i = 0; i < 6; i++)
		mac[i] = (uint8_t)values[i];

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno parse_serial(void *val, size_t size,
					   char const *string)
{
	int ret;
	uint8_t *serial = val;
	unsigned int values[4];
	int i;
	char rest;

	if (size != 8)
		return PON_ADAPTER_ERROR;

	/*
	 * Parse the serial number, if there is something remaining after it,
	 * the rest will get filled and this will return 9.
	 */
	ret = sscanf_s(string, "%c%c%c%c%02X%02X%02X%02X%c",
			 SSCANF_CHAR(&serial[0]),
			 SSCANF_CHAR(&serial[1]),
			 SSCANF_CHAR(&serial[2]),
			 SSCANF_CHAR(&serial[3]),
			 &values[0], &values[1], &values[2], &values[3],
			 SSCANF_CHAR(&rest));
	if (ret != 8)
		return PON_ADAPTER_ERROR;

	for (i = 0; i < 4; i++)
		serial[i + 4] = (uint8_t)values[i];

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno parse_hex(void *val, size_t size,
					char const *string)
{
	size_t i;
	char const *nptr = string;
	uint8_t *hex = val;
	char *endptr;

	for (i = 0; i < size; i++) {
		errno = 0;
		hex[i] = (uint8_t)strtoul(nptr, &endptr, 0);
		if (errno || nptr == endptr)
			return PON_ADAPTER_ERROR;
		nptr = endptr;
	}

	/* check if there are any remaining chars in the string */
	while (*nptr) {
		if (!isspace((int)*nptr))
			return PON_ADAPTER_ERROR;
		nptr++;
	}

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno parse_uint(void *val, size_t size,
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

static enum pon_adapter_errno parse_tx_power_scale(void *val, size_t size,
					 char const *string)
{
	char const *nptr = string;
	char *endptr;
	unsigned long int value;
	enum pon_tx_power_scale *scale = val;

	if (size != sizeof(enum pon_tx_power_scale))
		return PON_ADAPTER_ERROR;

	errno = 0;
	value = strtoul(nptr, &endptr, 0);
	if (errno || nptr == endptr)
		return PON_ADAPTER_ERROR;

	if (value == 1)
		*scale = TX_POWER_SCALE_0_2;
	else
		*scale = TX_POWER_SCALE_0_1;

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno parse_int(void *val, size_t size,
					char const *string)
{
	char const *nptr = string;
	char *endptr;
	int32_t value;

	errno = 0;
	value = (int32_t)strtol(nptr, &endptr, 0);
	if (errno || nptr == endptr)
		return PON_ADAPTER_ERROR;

	switch (size) {
	case 1:
		*(int8_t *)val = (int8_t)value;
		break;
	case 2:
		*(int16_t *)val = (int16_t)value;
		break;
	case 4:
		*(int32_t *)val = value;
		break;
	default:
		return PON_ADAPTER_ERROR;
	}

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno parse_float(void *val, size_t size,
					  char const *string)
{
	char const *nptr = string;
	char *endptr;
	double value;

	errno = 0;
	value = strtod(nptr, &endptr);
	if (errno || nptr == endptr)
		return PON_ADAPTER_ERROR;

	switch (size) {
	case sizeof(float):
		*(float *)val = (float)value;
		break;
	case sizeof(double):
		*(double *)val = value;
		break;
	default:
		return PON_ADAPTER_ERROR;
	}

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno parse_str(void *val, size_t size,
					char const *string)
{
	if (sprintf_s(val, size, "%s", string) < 0) {
		dbg_err_fn(sprintf_s);
		return PON_ADAPTER_ERROR;
	}

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno parse_link_type(void *val, size_t size,
					      char const *string)
{
	if (strcmp(string, "NONE") == 0)
		return parse_int(val, size, "0");

	if (strcmp(string, "A") == 0)
		return parse_int(val, size, "1");

	if (strcmp(string, "B") == 0)
		return parse_int(val, size, "2");

	if (strcmp(string, "AB") == 0)
		return parse_int(val, size, "3");

	return PON_ADAPTER_ERROR;
}

static enum pon_adapter_errno parse_cal_rec(void *val, size_t size,
					    char const *string)
{
	char const *nptr = string;
	char *endptr;
	uint64_t value;
	int i;

	if (size != sizeof(value))
		return PON_ADAPTER_ERROR;

	errno = 0;
	value =	(uint64_t)strtoull(nptr, &endptr, 0);
	if (errno || nptr == endptr)
		return PON_ADAPTER_ERROR;

	/* Nibble values greater than 3 are invalid */
	for (i = 0; i < sizeof(value) * 2; i++) {
		if (((value >> 4 * (15 - i)) & 0xF) > 3) {
			dbg_wrn("Value of nibble greater than 3!\n");
			return PON_ADAPTER_ERR_INVALID_VAL;
		}
	}

	*(uint64_t *)val = value;

	return PON_ADAPTER_SUCCESS;
}

static const struct {
	const char *key;
	enum pon_mode value;
} pon_modes[] = {
	{"gpon", PON_MODE_984_GPON},
	{"xgspon", PON_MODE_9807_XGSPON},
	{"xgpon", PON_MODE_987_XGPON},
	{"ngpon2_2G5", PON_MODE_989_NGPON2_2G5},
	{"ngpon2_10G", PON_MODE_989_NGPON2_10G},
	{"aon", PON_MODE_AON},
	{"unknown", PON_MODE_UNKNOWN}
};

static const char *pon_mode_to_string(enum pon_mode mode)
{
	unsigned int i = 0;

	for (i = 0; i < ARRAY_SIZE(pon_modes); ++i)
		if (mode == pon_modes[i].value)
			return pon_modes[i].key;

	return NULL;
}

static enum pon_adapter_errno parse_pon_mode(void *val, size_t size,
					     char const *string)
{
	enum pon_mode *pon_mode = val;
	unsigned int i = 0;

	UNUSED(size);

	for (i = 0; i < ARRAY_SIZE(pon_modes); ++i) {
		if (strcmp(string, pon_modes[i].key) == 0) {
			*pon_mode = pon_modes[i].value;
			return PON_ADAPTER_SUCCESS;
		}
	}

	*pon_mode = PON_MODE_UNKNOWN;
	return PON_ADAPTER_ERR_INVALID_VAL;
}

static const struct init_option_parser options[] = {
	INIT_OPTION("pon_mac",			parse_mac,	mac_sa),
	INIT_OPTION("soc_mac",			parse_mac,	mac_da),
	INIT_OPTION("omcc_protocol",		parse_hex,	protocol),
	INIT_OPTION("ethertype",		parse_uint,	ethertype),
	INIT_OPTION("ploam_emerg_stop_state",	parse_uint,
			ploam_emerg_stop_state),
};

#define __CFG_OPTION(r, n, s, o, v, p, m, sec) \
	{ .name = n, .section = s, .option = o, .value = v, .parser = p, \
	  .offset = offsetof(struct fapi_pon_wrapper_cfg, m), \
	  .size = sizeof_member(struct fapi_pon_wrapper_cfg, m), \
	  .is_mandatory = r, .secure = sec }

#define CFG_OPTION(r, n, s, o, v, p, m) \
	__CFG_OPTION(r, n, s, o, v, p, m, false)

#define CFG_OPTION_SEC(r, n, s, o, v, p, m) \
	__CFG_OPTION(r, n, s, o, v, p, m, true)

#define PON_OPT (1 << 0)
#define PON_REQ (1 << 1)
#define PON_NO_DEFAULT (1 << 2)

struct cfg_option {
	const char *name;
	const char *section;
	const char *option;
	const char *value; /* default */
	size_t offset;
	size_t size;
	enum pon_adapter_errno (*parser)(void *val, size_t size,
					 char const *string);
	uint32_t is_mandatory;
	bool secure;
};

static const struct cfg_option cfg_options[] = {
#ifdef PON_LIB_SIMULATOR
	/* Simulation doesn't provide these */
	CFG_OPTION_SEC(PON_OPT, "gpon", "ploam", "nSerial", NULL,
		   parse_serial, serial_no),
	CFG_OPTION_SEC(PON_OPT, "gpon", "ploam", "regID", NULL,
		   parse_hex, reg_id),
#else
	CFG_OPTION_SEC(PON_REQ, "gpon", "ploam", "nSerial", NULL,
		   parse_serial, serial_no),
	CFG_OPTION_SEC(PON_REQ, "gpon", "ploam", "regID", NULL,
		   parse_hex, reg_id),
#endif
	CFG_OPTION(PON_OPT, "gpon", "ploam", "ploam_timeout_0", "65535",
		   parse_uint, ploam_timeout_0),
	CFG_OPTION(PON_OPT, "gpon", "ploam", "ploam_timeout_1", "10000",
		   parse_uint, ploam_timeout_1),
	CFG_OPTION(PON_OPT, "gpon", "ploam", "ploam_timeout_2", "125",
		   parse_uint, ploam_timeout_2),
	CFG_OPTION(PON_OPT, "gpon", "ploam", "ploam_timeout_3", "25",
		   parse_uint, ploam_timeout_3),
	CFG_OPTION(PON_OPT, "gpon", "ploam", "ploam_timeout_4", "125",
		   parse_uint, ploam_timeout_4),
	CFG_OPTION(PON_OPT, "gpon", "ploam", "ploam_timeout_5", "125",
		   parse_uint, ploam_timeout_5),
	CFG_OPTION(PON_OPT, "gpon", "ploam", "ploam_timeout_6", "10000",
		   parse_uint, ploam_timeout_6),
	CFG_OPTION(PON_OPT, "gpon", "ploam", "ploam_timeout_cpl", "80",
		   parse_uint, ploam_timeout_cpl),
	CFG_OPTION(PON_OPT, "gpon", "ploam", "ploam_timeout_cpi", "300",
		   parse_uint, ploam_timeout_cpi),
	CFG_OPTION(PON_OPT, "gpon", "ploam", "ploam_timeout_tpd", "10000",
		   parse_uint, ploam_timeout_tpd),
	CFG_OPTION(PON_OPT, "gpon", "ploam", "plev_cap", "0",
		   parse_uint, plev_cap),
	CFG_OPTION(PON_OPT, "gpon", "ponip", "onu_identifier", "0",
		   parse_uint, ident),
	CFG_OPTION(PON_OPT, "gpon", "ponip", "pon_mode", "gpon",
		   parse_pon_mode, mode),
	CFG_OPTION(PON_OPT, "gpon", "ponip", "tdm_coexistence", "0",
		   parse_uint, tdm_coexistence),
	CFG_OPTION(PON_OPT, "gpon", "ponip", "iop_mask", "0",
		   parse_uint, iop_mask),
	CFG_OPTION(PON_OPT, "gpon", "authentication", "psk",
		   "0x11 0x22 0x33 0x44 0x55 0x66 0x77 0x88 0x99 0xAA 0xBB 0xCC 0xDD 0xEE 0xFF 0xEF",
		   parse_hex, psk),
	CFG_OPTION(PON_OPT, "gpon", "authentication", "key_size", "128",
		   parse_uint, psk_size),
	CFG_OPTION(PON_OPT, "gpon", "authentication", "mode", "1",
		   parse_uint, auth_mode),
	/* Debugging options, forcing to non default value will not work without
	 * corresponding config of datapath. Do not add to default uci config.
	 */
	CFG_OPTION(PON_OPT, "gpon", "ponip", "ds_fcs_en", "-1",
		   parse_int, ds_fcs_en),
	CFG_OPTION(PON_OPT, "gpon", "ponip", "ds_ts_dis", "-1",
		   parse_int, ds_ts_dis),

	CFG_OPTION(PON_OPT, "omci", "default", "enabled", "1",
		   parse_uint, enabled),

	CFG_OPTION(PON_OPT, "optic", "common", "gpio21_mode", "0",
		   parse_uint, gpio.gpio21_mode),
	CFG_OPTION(PON_OPT, "optic", "common", "gpio24_mode", "0",
		   parse_uint, gpio.gpio24_mode),
	CFG_OPTION(PON_OPT, "optic", "common", "gpio25_mode", "0",
		   parse_uint, gpio.gpio25_mode),
	CFG_OPTION(PON_OPT, "optic", "common", "dg_dis", "0",
		   parse_uint, dg_dis),
	CFG_OPTION(PON_OPT, "optic", "common", "mode", "unknown",
		   parse_pon_mode, optic.pon_mode),
	CFG_OPTION(PON_OPT, "optic", "common", "sfp_tweaks", "0",
		   parse_uint, sfp_tweaks),

	CFG_OPTION(PON_OPT, "tod", "common", "pps_scale", NULL,
		   parse_uint, tod.pps_scale),
	CFG_OPTION(PON_OPT, "tod", "common", "pps_width", NULL,
		   parse_uint, tod.pps_width),
	CFG_OPTION(PON_OPT, "tod", "common", "pps_delay", NULL,
		   parse_uint, tod.pps_delay),
	CFG_OPTION(PON_OPT, "tod", "common", "pps_polarity", NULL,
		   parse_int, tod.pps_level),
	CFG_OPTION(PON_OPT, "tod", "common", "us_time_correction", NULL,
		   parse_uint, tod.us_ts_corr),
	CFG_OPTION(PON_OPT, "tod", "common", "ds_time_correction", NULL,
		   parse_uint, tod.ds_ts_corr),
	CFG_OPTION(PON_OPT, "tod", "common", "asymm_corr_gpon", "0.500065",
		   parse_float, tod_asymm_corr_gpon),
	CFG_OPTION(PON_OPT, "tod", "common", "asymm_corr_xgspon", "0.500153",
		   parse_float, tod_asymm_corr_xgspon),
	CFG_OPTION(PON_OPT, "tod", "common", "asymm_corr_ngpon2", "0.5000727",
		   parse_float, tod_asymm_corr_ngpon2),
	CFG_OPTION(PON_OPT, "tod", "common", "offset_pico_seconds_2g5", "-18119000",
		   parse_int, tod_offset_pico_seconds_2g5),
	CFG_OPTION(PON_OPT, "tod", "common", "offset_pico_seconds_10g", "-17206000",
		   parse_int, tod_offset_pico_seconds_10g),

	CFG_OPTION(PON_OPT, "optic", "sfp_eeprom", "dmi", NULL,
		   parse_str, eeprom_dmi),
	CFG_OPTION(PON_OPT, "optic", "sfp_eeprom", "serial_id", NULL,
		   parse_str, eeprom_serial_id),
	/* Default values for thresholds: ITU-T G.989 chapter 11.1.4 */
	/* Defined in units of 0.5dBm */
	/* - 29 dBm is below the lowest usable receive power value. */
	CFG_OPTION(PON_OPT, "optic", "threshold", "lower_receive_optical_threshold",
		   "-29", parse_int, lower_receive_optical_threshold),
	/* -7 dBm is above the highest usable receive power value. */
	CFG_OPTION(PON_OPT, "optic", "threshold", "upper_receive_optical_threshold",
		   "-7", parse_int, upper_receive_optical_threshold),
	/* +1 dBm is below the lowest usable transmit power value. */
	CFG_OPTION(PON_OPT, "optic", "threshold", "lower_transmit_power_threshold",
		   "1", parse_int, lower_transmit_power_threshold),
	/* +10 dBm is above the highest usable transmit power value. */
	CFG_OPTION(PON_OPT, "optic", "threshold", "upper_transmit_power_threshold",
		   "10", parse_int, upper_transmit_power_threshold),
	CFG_OPTION(PON_OPT, "optic", "threshold", "bias_threshold", "60000",
		   parse_uint, bias_threshold)
};

#define OPTIC_CFG_OPTION(o, p, m) \
	CFG_OPTION(PON_OPT, "optic", NULL, o, NULL, p, m)

/* optic config options, also handled per mode */
static const struct cfg_option optic_cfg_options[] = {
	OPTIC_CFG_OPTION("laser_setup_time", parse_uint,
			 optic.laser_setup_time),
	OPTIC_CFG_OPTION("laser_hold_time", parse_uint, optic.laser_hold_time),
	OPTIC_CFG_OPTION("serdes_setup_time", parse_uint,
			 optic.serdes_setup_time),
	OPTIC_CFG_OPTION("serdes_hold_time", parse_uint,
			 optic.serdes_hold_time),
	OPTIC_CFG_OPTION("bias_setup_time", parse_uint, optic.bias_setup_time),
	OPTIC_CFG_OPTION("bias_hold_time", parse_uint, optic.bias_hold_time),
	OPTIC_CFG_OPTION("burst_en_mode", parse_uint, optic.burst_en_mode),
	OPTIC_CFG_OPTION("burst_idle_pattern", parse_uint,
			 optic.burst_idle_pattern),
	OPTIC_CFG_OPTION("tx_en_mode", parse_uint, optic.tx_en_mode),
	OPTIC_CFG_OPTION("tx_pup_mode", parse_uint, optic.tx_pup_mode),
	OPTIC_CFG_OPTION("sd_polarity", parse_int, optic.sd_polarity),
	OPTIC_CFG_OPTION("loop_timing_power_save", parse_uint,
			 optic.loop_timing_power_save),
	OPTIC_CFG_OPTION("rogue_lag_time", parse_uint, optic.rogue_lag_time),
	OPTIC_CFG_OPTION("rogue_auto_en", parse_uint, optic.rogue_auto_en),
	OPTIC_CFG_OPTION("rogue_lead_time", parse_uint, optic.rogue_lead_time),
	OPTIC_CFG_OPTION("opt_tx_sd_polarity", parse_uint, optic.opt_tx_sd_pol),
	OPTIC_CFG_OPTION("pse_en", parse_uint, optic.pse_en),
	OPTIC_CFG_OPTION("tx_power_scale", parse_tx_power_scale,
			 optic.tx_power_scale),
};

/* optic options for transceiver timing offsets */
static const struct cfg_option optic_time_offsets_options[] = {
	OPTIC_CFG_OPTION("laser_setup_time", parse_int,
			 optic_offsets.laser_setup_time),
	OPTIC_CFG_OPTION("laser_hold_time", parse_int,
			 optic_offsets.laser_hold_time),
	OPTIC_CFG_OPTION("serdes_setup_time", parse_int,
			 optic_offsets.serdes_setup_time),
	OPTIC_CFG_OPTION("serdes_hold_time", parse_int,
			 optic_offsets.serdes_hold_time),
	OPTIC_CFG_OPTION("bias_setup_time", parse_int,
			 optic_offsets.bias_setup_time),
	OPTIC_CFG_OPTION("bias_hold_time", parse_int,
			 optic_offsets.bias_hold_time),
};

/* TWDM options are specific to NG-PON2 */
static const struct cfg_option twdm_options[] = {
	CFG_OPTION(PON_OPT, "optic", "twdm", "config_method", NULL,
		   parse_uint, twdm_config_method),
	CFG_OPTION(PON_OPT, "optic", "twdm", "tuning", NULL,
		   parse_uint, twdm_tuning),
	CFG_OPTION(PON_OPT, "optic", "twdm", "optical_link_type", NULL,
		   parse_link_type, twdm.link_type),
	CFG_OPTION(PON_OPT, "optic", "twdm", "dwlch_id", NULL,
		   parse_uint, twdm.dwlch_id),
	CFG_OPTION(PON_OPT, "optic", "twdm", "free_spectral_range", NULL,
		   parse_uint, twdm.free_spectral_range),
	CFG_OPTION(PON_OPT, "optic", "twdm", "wl_ch_spacing", NULL,
		   parse_uint, twdm.wl_ch_spacing),
	CFG_OPTION(PON_OPT, "optic", "twdm", "spectral_excursion", NULL,
		   parse_uint, twdm.max_spectral_excursion),
	CFG_OPTION(PON_OPT, "optic", "twdm", "tuning_gran", NULL,
		   parse_uint, twdm.tuning_gran),
	CFG_OPTION(PON_OPT, "optic", "twdm", "rx_wl_switch_time", NULL,
		   parse_uint, twdm.rx_wl_switch_time),
	CFG_OPTION(PON_OPT, "optic", "twdm", "tx_wl_switch_time", NULL,
		   parse_uint, twdm.tx_wl_switch_time),
	CFG_OPTION(PON_OPT, "optic", "twdm", "ch_partition_index", NULL,
		   parse_uint, twdm.ch_partition_index),
	CFG_OPTION(PON_OPT, "optic", "twdm", "fine_tune_time", NULL,
		   parse_uint, twdm.fine_tune_time),
	CFG_OPTION(PON_OPT, "optic", "twdm", "cal_status_record", NULL,
		   parse_cal_rec, cal_status_record),
	CFG_OPTION(PON_OPT, "optic", "twdm", "channel_mask", NULL,
		   parse_uint, twdm_channel_mask),
	CFG_OPTION(PON_OPT, "optic", "twdm", "wl_switch_delay", "24000",
		   parse_uint, twdm_wlse_config.wl_switch_delay),
	CFG_OPTION(PON_OPT, "optic", "twdm", "rx_wl_switch_delay_initial",
		    "8000", parse_uint, twdm_wlse_config.wl_sw_delay_init),
	CFG_OPTION(PON_OPT, "optic", "twdm", "rx_wl_switch_rounds_initial",
		    "3", parse_uint, twdm_wlse_config.wl_sw_rounds_init),
};

static const struct cfg_option serdes_generic_options[] = {
	CFG_OPTION(PON_REQ | PON_NO_DEFAULT, "serdes", NULL, "tx_eq_pre",
		NULL, parse_uint, serdes.tx_eq_pre),
	CFG_OPTION(PON_REQ | PON_NO_DEFAULT, "serdes", NULL, "tx_eq_main",
		NULL, parse_uint, serdes.tx_eq_main),
	CFG_OPTION(PON_REQ | PON_NO_DEFAULT, "serdes", NULL, "tx_eq_post",
		NULL, parse_uint, serdes.tx_eq_post),
	CFG_OPTION(PON_REQ | PON_NO_DEFAULT, "serdes", NULL, "vboost_en",
		NULL, parse_uint, serdes.vboost_en),
	CFG_OPTION(PON_REQ | PON_NO_DEFAULT, "serdes", NULL, "vboost_lvl",
		NULL, parse_uint, serdes.vboost_lvl),
	CFG_OPTION(PON_REQ | PON_NO_DEFAULT, "serdes", NULL, "iboost_lvl",
		NULL, parse_uint, serdes.iboost_lvl),
	CFG_OPTION(PON_REQ | PON_NO_DEFAULT, "serdes", NULL,
		"rx_adapt_en", NULL, parse_uint, serdes.rx_adapt_en),
	CFG_OPTION(PON_REQ | PON_NO_DEFAULT, "serdes", NULL,
		"rx_adapt_cont", NULL, parse_uint, serdes.rx_adapt_cont),
	CFG_OPTION(PON_REQ | PON_NO_DEFAULT, "serdes", NULL,
		"rx_eq_att_lvl", NULL, parse_uint, serdes.rx_eq_att_lvl),
	CFG_OPTION(PON_OPT, "serdes", NULL,
		"rx_eq_adapt_mode", "3", parse_uint, serdes.rx_eq_adapt_mode),
	CFG_OPTION(PON_OPT, "serdes", NULL, "rx_eq_adapt_sel",
		"0", parse_uint, serdes.rx_eq_adapt_sel),
	CFG_OPTION(PON_REQ | PON_NO_DEFAULT, "serdes", NULL,
		"rx_eq_ctle_boost", NULL, parse_uint, serdes.rx_eq_ctle_boost),
	CFG_OPTION(PON_OPT, "serdes", NULL, "rx_vco_temp_comp_en",
		"1", parse_uint, serdes.rx_vco_temp_comp_en),
	CFG_OPTION(PON_OPT, "serdes", NULL,
		"rx_vco_step_ctrl", "1", parse_uint, serdes.rx_vco_step_ctrl),
	CFG_OPTION(PON_OPT, "serdes", NULL,
		"rx_vco_frqband", "1", parse_uint, serdes.rx_vco_frqband),
	CFG_OPTION(PON_OPT, "serdes", NULL, "rx_misc",
		"18", parse_uint, serdes.rx_misc),
	CFG_OPTION(PON_OPT, "serdes", NULL, "rx_delta_iq",
		"0", parse_uint, serdes.rx_delta_iq),
	CFG_OPTION(PON_OPT, "serdes", NULL, "rx_margin_iq",
		"0", parse_uint, serdes.rx_margin_iq),
	CFG_OPTION(PON_REQ | PON_NO_DEFAULT, "serdes", NULL,
		"rx_eq_ctle_pole", NULL, parse_uint, serdes.rx_eq_ctle_pole),
	CFG_OPTION(PON_REQ | PON_NO_DEFAULT, "serdes", NULL,
		"rx_eq_dfe_tap1", NULL, parse_uint, serdes.rx_eq_dfe_tap1),
	CFG_OPTION(PON_OPT, "serdes", NULL, "rx_eq_dfe_bypass",
		"0", parse_uint, serdes.rx_eq_dfe_bypass),
	CFG_OPTION(PON_REQ | PON_NO_DEFAULT, "serdes", NULL,
		"rx_eq_vga1_gain", NULL, parse_uint, serdes.rx_eq_vga1_gain),
	CFG_OPTION(PON_REQ | PON_NO_DEFAULT, "serdes", NULL,
		"rx_eq_vga2_gain", NULL, parse_uint, serdes.rx_eq_vga2_gain),
	CFG_OPTION(PON_OPT, "serdes", NULL, "rx_adapt_afe_en",
		"0", parse_uint, serdes.rx_adapt_afe_en),
	CFG_OPTION(PON_OPT, "serdes", NULL, "rx_adapt_dfe_en",
		"0", parse_uint, serdes.rx_adapt_dfe_en),
	/* rx_slos_thr currently not used but config option kept */
	CFG_OPTION(PON_OPT, "serdes", NULL, "rx_slos_thr",
		"4", parse_uint, serdes.rx_slos_thr),
};

static const struct cfg_option serdes_mode_options[] = {
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL, "tx_eq_pre",
		NULL, parse_uint, serdes.tx_eq_pre),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL, "tx_eq_main",
		NULL, parse_uint, serdes.tx_eq_main),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL, "tx_eq_post",
		NULL, parse_uint, serdes.tx_eq_post),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL, "vboost_en",
		NULL, parse_uint, serdes.vboost_en),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL, "vboost_lvl",
		NULL, parse_uint, serdes.vboost_lvl),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL, "iboost_lvl",
		NULL, parse_uint, serdes.iboost_lvl),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL,
		"rx_adapt_en", NULL, parse_uint, serdes.rx_adapt_en),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL,
		"rx_adapt_cont", NULL, parse_uint, serdes.rx_adapt_cont),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL,
		"rx_eq_att_lvl", NULL, parse_uint, serdes.rx_eq_att_lvl),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL,
		"rx_eq_adapt_mode", NULL, parse_uint, serdes.rx_eq_adapt_mode),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL, "rx_eq_adapt_sel",
		NULL, parse_uint, serdes.rx_eq_adapt_sel),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL,
		"rx_eq_ctle_boost", NULL, parse_uint, serdes.rx_eq_ctle_boost),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL,
		"rx_vco_temp_comp_en", NULL, parse_uint,
		serdes.rx_vco_temp_comp_en),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL,
		"rx_vco_step_ctrl", NULL, parse_uint, serdes.rx_vco_step_ctrl),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL,
		"rx_vco_frqband", NULL, parse_uint, serdes.rx_vco_frqband),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL, "rx_misc",
		NULL, parse_uint, serdes.rx_misc),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL, "rx_delta_iq",
		NULL, parse_uint, serdes.rx_delta_iq),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL, "rx_margin_iq",
		NULL, parse_uint, serdes.rx_margin_iq),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL,
		"rx_eq_ctle_pole", NULL, parse_uint, serdes.rx_eq_ctle_pole),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL,
		"rx_eq_dfe_tap1", NULL, parse_uint, serdes.rx_eq_dfe_tap1),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL,
		"rx_eq_dfe_bypass", NULL, parse_uint, serdes.rx_eq_dfe_bypass),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL,
		"rx_eq_vga1_gain", NULL, parse_uint, serdes.rx_eq_vga1_gain),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL,
		"rx_eq_vga2_gain", NULL, parse_uint, serdes.rx_eq_vga2_gain),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL, "rx_adapt_afe_en",
		NULL, parse_uint, serdes.rx_adapt_afe_en),
	CFG_OPTION(PON_OPT | PON_NO_DEFAULT, "serdes", NULL, "rx_adapt_dfe_en",
		NULL, parse_uint, serdes.rx_adapt_dfe_en),
};

/**
 *	Read all configs defined in "options" array
 *
 *	\param[in] ctx		PON wrapper context
 *	\param[in] options	Array of options
 *	\param[in] size		Size of options array
 *	\param[in] section	Default section if not defined in array
 *				(optional)
 */
static enum pon_adapter_errno read_pa_config(struct fapi_pon_wrapper_ctx *ctx,
					     const struct cfg_option *options,
					     size_t size, const char *section)
{
	/* Return value for this function, if at the end
	 * ret != PON_ADAPTER_SUCCESS, then omci daemon will not start.
	 */
	enum pon_adapter_errno ret = PON_ADAPTER_SUCCESS;
	const struct pa_config *cfg_ops = ctx->cfg_ops;
	uint8_t *cfg_base = (uint8_t *)&ctx->cfg;
	unsigned int i = 0;

	if (!cfg_ops)
		return PON_ADAPTER_ERROR;

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

		if (cfg_section) {
			cfg_error = 1;
			if (option->secure && cfg_ops->get_secure)
				cfg_error = cfg_ops->get_secure(
					ctx->hl_ctx, option->name, cfg_section,
					option->option, sizeof(value), value);

			if (cfg_error)
				cfg_error = cfg_ops->get(
					ctx->hl_ctx, option->name, cfg_section,
					option->option, sizeof(value), value);
		} else {
			cfg_error = 0;
		}

		dbg_prn("Get config option %s.%s.%s (size %zd, flags %x) with status %d\n",
			option->name, cfg_section, option->option, option->size,
			option->is_mandatory, cfg_error);

		if (cfg_error) {
			/* The option is PON_REQ and must be in UCI */
			if (option->is_mandatory & PON_REQ) {
				dbg_err("Missing required option: %s.%s.%s\n",
					option->name,
					cfg_section ? cfg_section : "NO_SECTION_SPECIFIED",
					option->option);
				ret = PON_ADAPTER_ERROR;
				continue;
			}

			/* If option is not PON_REQ it can only
			 *  be PON_OPT */
			if (!(option->is_mandatory & PON_OPT)) {
				dbg_err("Option can be either PON_REQ or PON_OPT\n");
				ret = PON_ADAPTER_ERROR;
				continue;
			}

			/* But if it is OPTIONAL we must assign a default value.
			 */
			if (!option->value) {
				/* We do not set error code, because this is not
				 * critical and pon_lib can still be
				 * initialized.
				 */
				if (option->is_mandatory & PON_NO_DEFAULT)
					continue;

				/* We expect that there is default value
				 * was provided */
				dbg_wrn("Missing option with no default value: %s.%s.%s\n",
					option->name,
					cfg_section ? cfg_section : "NO_SECTION_SPECIFIED",
					option->option);
				continue;
			}

			if (sprintf_s(value, sizeof(value),
				       "%s", option->value) < 0) {
				dbg_err_fn(sprintf_s);
				return PON_ADAPTER_ERROR;
			}
		}

		parse_error = option->parser(cfg_base + option->offset,
					     option->size, value);
		if (parse_error) {
			dbg_err("Parsing failed for: %s.%s.%s\n",
				option->name,
				cfg_section ? cfg_section : "NO_SECTION_SPECIFIED",
				option->option);
			return parse_error;
		}
		dbg_prn("Parsed config option %s.%s.%s, value %s\n",
			option->name, cfg_section, option->option, value);
	}

	return ret;
}

static enum pon_adapter_errno start(void *ll_handle)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum pon_adapter_errno ret;

	pthread_mutex_lock(&ctx->lock);
	ret = pon_pa_event_start(ctx);
	pthread_mutex_unlock(&ctx->lock);
	return ret;
}

enum pon_adapter_errno pon_fapi_to_pa_error(enum fapi_pon_errorcode err)
{
	switch (err) {
	case PON_STATUS_OK:
	case PON_STATUS_OK_NO_RESPONSE:
		return PON_ADAPTER_SUCCESS;
	case PON_STATUS_VALUE_RANGE_ERR:
		return PON_ADAPTER_ERR_IF_NOT_FOUND;
	case PON_STATUS_RESOURCE_ERR:
	case PON_STATUS_ALLOC_ID_EXISTS:
	case PON_STATUS_ALLOC_ID_MISSING:
	case PON_STATUS_QOS_PORT_INDEX_ERR:
	case PON_STATUS_DUPLICATE_GEM_ERR:
	case PON_STATUS_INPUT_ERR:
	case PON_STATUS_GEM_PORT_ID_NOT_EXISTS_ERR:
	case PON_STATUS_ALLOC_GEM_MAP_ERR:
		return PON_ADAPTER_ERR_INVALID_VAL;
	case PON_STATUS_MEM_NOT_ENOUGH:
	case PON_STATUS_MEM_ERR:
		return PON_ADAPTER_ERR_MEM_ACCESS;
	case PON_STATUS_SUPPORT:
	case PON_STATUS_OPERATION_MODE_ERR:
		return PON_ADAPTER_ERR_NOT_SUPPORTED;
	default:
		/* Map the rest of the error to generic code */
		return PON_ADAPTER_ERR_DRV;
	}
}

enum pon_adapter_errno
pon_fapi_thread_stop(const pthread_t *th_id, const char *th_name, int time)
{
	char errbuf[256] = {0};
	char *errstr;
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
		errstr = pon_strerr(err, errbuf, sizeof(errbuf));
		dbg_err("pthread_cancel <%s> returned: %d - %s\n",
			th_name, err, errstr);
		return PON_ADAPTER_ERROR;
	}

	/* wait a given time for joining the thread */
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += time;
	err = pthread_timedjoin_np(*th_id, &res, &ts);
	if (err) {
		errstr = pon_strerr(err, errbuf, sizeof(errbuf));
		dbg_err("%s <%s> returned: %d %s\n",
			"pthread_timedjoin_np", th_name, err, errstr);
		return PON_ADAPTER_ERROR;
	}

	if (res && res != PTHREAD_CANCELED) {
		dbg_err("<%s> returned with %ld\n", th_name, (long)res);
		return PON_ADAPTER_ERROR;
	}

	dbg_out_ret("%d", PON_ADAPTER_SUCCESS);
	return PON_ADAPTER_SUCCESS;
}

static void set_sd_polarity(struct fapi_pon_wrapper_cfg *cfg,
			    struct pon_ctx *pon_ctx)
{
	struct pon_optic_properties optic_properties;
	enum fapi_pon_errorcode ret;

	/* only check in EEPROM if no user value is set, -1 is default */
	if (cfg->optic.sd_polarity != -1)
		return;

	ret = fapi_pon_optic_properties_get(pon_ctx, &optic_properties);
	if (ret != PON_STATUS_OK) {
		/* if we cannot access the EEPROM, try to guess */
		if (cfg->mode == PON_MODE_984_GPON)
			cfg->optic.sd_polarity = 1;
		else
			cfg->optic.sd_polarity = 0;
		return;
	}

	if (optic_properties.signal_detect && optic_properties.rx_los)
		dbg_err("LOS signal configuration is invalid\n");
	else if (optic_properties.signal_detect)
		cfg->optic.sd_polarity = 1;
	else if (optic_properties.rx_los)
		cfg->optic.sd_polarity = 0;
}

static enum pon_adapter_errno init(char const * const *init_data,
				   const struct pa_config *pa_config,
				   const struct pa_eh_ops *event_handler,
				   void *ll_handle)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct fapi_pon_wrapper_cfg *cfg = &ctx->cfg;
	struct pon_ctx *pon_ctx = NULL;
	enum fapi_pon_errorcode fapi_ret;
	enum pon_adapter_errno error;
	char buffer[200]; /* reg_id will be around 180 chars (36 * 5) */
	char *name, *value, *saveptr;
	/* Each bit in this mask defines a required option from the list above.
	 * If not all required options are found, an error is returned.
	 */
	uint32_t optmask = 0x0003;
	unsigned int i;
	static const uint8_t protocol_default[5] = {0x0, 0x19, 0xA7, 0x0, 0x2};
	const char *pon_mode = NULL;
	struct pon_dp_config dp_config = { 0 };

	pthread_mutex_init(&ctx->lock, NULL);

	error = pon_pa_mapper_init(ctx);
	if (error != PON_ADAPTER_SUCCESS)
		return error;

	ctx->cfg_ops = pa_config;

	memset(cfg, 0, sizeof(*cfg));
	/* set defaults for some config values */
	if (memcpy_s(cfg->protocol, sizeof(cfg->protocol),
		     protocol_default, sizeof(protocol_default))) {
		dbg_err_fn(memcpy_s);
		return PON_ADAPTER_ERROR;
	}
	cfg->ethertype = 0x88B7;
	cfg->optic.sd_polarity = -1;
	cfg->optic.rogue_auto_en = 2;

	while (init_data && *init_data) {
		if (sprintf_s(buffer, sizeof(buffer), "%s", *init_data) < 0) {
			dbg_err_fn(sprintf_s);
			return PON_ADAPTER_ERROR;
		}
		name = strtok_r(buffer, "=", &saveptr);
		value = strtok_r(NULL, "=", &saveptr);

		if (!name)
			continue;

		/* check for the params we are interested in */
		for (i = 0; i < ARRAY_SIZE(options) && value; i++) {
			if (strcmp(name, options[i].name) == 0) {
				error = options[i].parser(((uint8_t *)cfg) +
					options[i].offset,
					options[i].size, value);
				if (error) {
					dbg_err("Parsing failed on %s!\n",
						      name);
					return error;
				}
				/* clear bit for each option */
				optmask &= ~(1 << i);
				break;
			}
		}
		init_data++;
	}

	/* this should be 0 if required options were found */
	if (optmask) {
		dbg_err("Required param missing!\n");
		dbg_err("Please provide value for:");
		for (i = 0; i < ARRAY_SIZE(options); i++) {
			if (optmask & (1 << i))
				dbg_err(" %s", options[i].name);
		}
		dbg_err("\n");
		return PON_ADAPTER_ERROR;
	}

	error = read_pa_config(ctx, cfg_options, ARRAY_SIZE(cfg_options), NULL);
	if (error)
		return error;

	pon_mode = pon_mode_to_string(ctx->cfg.mode);
	if (ctx->cfg.mode != ctx->cfg.optic.pon_mode)
		dbg_wrn("optic (transceiver) mode (%s) is different from pon_mode (%s)\n",
			pon_mode_to_string(ctx->cfg.optic.pon_mode), pon_mode);

	error = read_pa_config(ctx, optic_cfg_options,
			       ARRAY_SIZE(optic_cfg_options), "common");
	if (error)
		return error;

	error = read_pa_config(ctx, optic_time_offsets_options,
			       ARRAY_SIZE(optic_time_offsets_options),
			       "offsets");
	if (error)
		return error;

	if (pon_mode) {
		/* Overwrite defaults with values specific for
		 * selected PON mode */
		error = read_pa_config(ctx, optic_cfg_options,
				       ARRAY_SIZE(optic_cfg_options), pon_mode);
		if (error)
			return error;
	}

	if (ctx->cfg.mode == PON_MODE_989_NGPON2_10G ||
	    ctx->cfg.mode == PON_MODE_989_NGPON2_2G5) {
		error = read_pa_config(ctx, twdm_options,
				       ARRAY_SIZE(twdm_options), "twdm");
		if (error)
			return error;
	}
	ctx->twdm_ops = pon_twdm_select_ops(ctx->cfg.twdm_config_method);

#ifndef PON_LIB_SIMULATOR
	/* Read default serdes configuration */
	error = read_pa_config(ctx, serdes_generic_options,
			       ARRAY_SIZE(serdes_generic_options), "generic");
	if (error)
		return error;
#endif

	if (pon_mode) {
		/* Overwrite defaults with values specific for
		 * selected PON mode */
		error = read_pa_config(ctx, serdes_mode_options,
				       ARRAY_SIZE(serdes_mode_options),
				       pon_mode);
		if (error)
			return error;
	}

	fapi_ret = fapi_pon_open(&pon_ctx);
	if (fapi_ret != PON_STATUS_OK)
		return PON_ADAPTER_ERROR;

	if (strnlen_s(cfg->eeprom_serial_id,
	    sizeof(cfg->eeprom_serial_id)) > 0) {
		fapi_ret = fapi_pon_eeprom_open(pon_ctx, PON_DDMI_A0,
						cfg->eeprom_serial_id);
		if (fapi_ret != PON_STATUS_OK) {
			fapi_pon_close(pon_ctx);
			return PON_ADAPTER_ERROR;
		}
	}

	if (strnlen_s(cfg->eeprom_dmi,
	    sizeof(cfg->eeprom_dmi)) > 0) {
		fapi_ret = fapi_pon_eeprom_open(pon_ctx, PON_DDMI_A2,
						cfg->eeprom_dmi);
		if (fapi_ret != PON_STATUS_OK) {
			fapi_pon_close(pon_ctx);
			return PON_ADAPTER_ERROR;
		}
	}

	fapi_ret = fapi_pon_dp_config_get(pon_ctx, &dp_config);
	if (fapi_ret == PON_STATUS_OK) {
		if (cfg->ds_fcs_en == -1)
			cfg->ds_fcs_en = dp_config.with_rx_fcs;
		if (cfg->ds_ts_dis == -1)
			cfg->ds_ts_dis = dp_config.without_timestamp;
	}

	set_sd_polarity(cfg, pon_ctx);

	/* Initialize ani_g_data lock */
	pthread_mutex_init(&ctx->ani_g_data.lock, NULL);

	ctx->pon_ctx = pon_ctx;
	ctx->event_handlers = *event_handler;

	return pon_pa_event_handling_init(ctx);
}

static enum pon_adapter_errno shutdown(void *ll_handle)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum pon_adapter_errno pa_error;
	enum fapi_pon_errorcode err;

	if (ctx->pon_ctx) {
		err = fapi_pon_1pps_event_disable(ctx->pon_ctx);
		if (err != PON_STATUS_OK)
			dbg_err_fn_ret(fapi_pon_1pps_event_disable, err);
	}

	pa_error = pon_pa_mapper_shutdown(ctx);
	if (pa_error != PON_ADAPTER_SUCCESS)
		dbg_err("Error in mapper_shutdown\n");

	pa_error = pon_pa_event_handling_stop(ctx);

	if (ctx->pon_ctx)
		(void)fapi_pon_close(ctx->pon_ctx);

	pthread_mutex_destroy(&ctx->lock);
	free(ctx);

	return pa_error;
}

static enum pon_adapter_errno ik_key_get(void *ll_handle, uint8_t *ik_key,
					 uint32_t key_len)
{
	enum fapi_pon_errorcode ret;
	struct pon_omci_ik omci_ik;
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;

	pthread_mutex_lock(&ctx->lock);
	ret = fapi_pon_omci_ik_get(pon_ctx, &omci_ik);
	pthread_mutex_unlock(&ctx->lock);
	if (ret != PON_STATUS_OK)
		return pon_fapi_to_pa_error(ret);

	if (memcpy_s(ik_key, key_len,
		     omci_ik.key, sizeof(omci_ik.key))) {
		dbg_err_fn(memcpy_s);
		return PON_ADAPTER_ERROR;
	}

	return PON_ADAPTER_SUCCESS;
}

static void dbg_lvl_set(const uint8_t level)
{
	libpon_dbg_lvl = level;
	fapi_pon_dbg_level_set(level);
}

static uint8_t dbg_lvl_get(void)
{
	return fapi_pon_dbg_level_get();
}

static enum pa_pon_op_mode get_pa_mode(enum pon_mode mode)
{
	switch (mode) {
	case PON_MODE_987_XGPON:
		return PA_PON_MODE_G987;
	case PON_MODE_9807_XGSPON:
		return PA_PON_MODE_G9807;
	case PON_MODE_989_NGPON2_2G5:
	case PON_MODE_989_NGPON2_10G:
		return PA_PON_MODE_G989;
	case PON_MODE_984_GPON:
		return PA_PON_MODE_G984;
	case PON_MODE_UNKNOWN:
	default:
		return PA_PON_MODE_UNKNOWN;
	}
}

static enum pon_adapter_errno get_pon_op_mode(void *ll_handle,
						enum pa_pon_op_mode *mode)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;

	if (!mode) {
		dbg_out_ret("%d", PON_ADAPTER_ERR_INVALID_VAL);
		return PON_ADAPTER_ERR_INVALID_VAL;
	}
	*mode =	get_pa_mode(ctx->cfg.mode);

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno get_alarm_status(void *ll_handle,
					       uint16_t alarm_id,
					       uint8_t *status)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_alarm_status param = {0,};
	enum fapi_pon_errorcode err;

	if (!status)
		return PON_ADAPTER_ERR_INVALID_VAL;

	pthread_mutex_lock(&ctx->lock);
	err = fapi_pon_alarm_status_get(ctx->pon_ctx, alarm_id, &param);
	pthread_mutex_unlock(&ctx->lock);
	if (err != PON_STATUS_OK) {
		dbg_err("getting alarm status failed\n");
		return pon_fapi_to_pa_error(err);
	}

	*status = param.alarm_status;

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno get_pon_status(void *ll_handle, bool *enabled)
{
	struct pon_gpon_rerange_cfg rerange_cfg = {0};
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode err;

	pthread_mutex_lock(&ctx->lock);
	err = fapi_pon_gpon_rerange_status_get(ctx->pon_ctx, &rerange_cfg);
	pthread_mutex_unlock(&ctx->lock);
	if (err != PON_STATUS_OK) {
		dbg_err("setting rerange config failed\n");
		return pon_fapi_to_pa_error(err);
	}

	*enabled = !(rerange_cfg.psmdis || rerange_cfg.txdis);

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno set_pon_status(void *ll_handle, bool enable)
{
	struct pon_gpon_rerange_cfg rerange_cfg = {0};
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode err;

	if (enable) {
		rerange_cfg.psmdis = 0;
		rerange_cfg.txdis = 0;
	} else {
		rerange_cfg.psmdis = 1;
		rerange_cfg.txdis = 1;
	}
	pthread_mutex_lock(&ctx->lock);
	err = fapi_pon_gpon_rerange_cfg_set(ctx->pon_ctx, &rerange_cfg);
	pthread_mutex_unlock(&ctx->lock);
	if (err != PON_STATUS_OK) {
		dbg_err("setting rerange config failed\n");
		return pon_fapi_to_pa_error(err);
	}
	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno get_serial_number(void *ll_handle,
						uint8_t *serial_number)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct fapi_pon_wrapper_cfg *cfg = &ctx->cfg;

	if (memcpy_s(serial_number, PON_SERIAL_NO_SIZE,
		     cfg->serial_no, sizeof(cfg->serial_no))) {
		dbg_err_fn(memcpy_s);
		return PON_ADAPTER_ERROR;
	}

	return PON_ADAPTER_SUCCESS;
}

static enum pon_adapter_errno get_gemports(void *ll_handle,
					   uint16_t *gemport_num)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	struct pon_ctx *pon_ctx = ctx->pon_ctx;
	enum fapi_pon_errorcode ret;
	struct pon_cap caps;

	if (gemport_num == NULL)
		return PON_ADAPTER_ERR_PTR_INVALID;

	pthread_mutex_lock(&ctx->lock);
	ret = fapi_pon_cap_get(pon_ctx, &caps);
	pthread_mutex_unlock(&ctx->lock);
	if (ret != PON_STATUS_OK) {
		dbg_err("The maximum number of GEM ports can not be read from the capabilities!\n");
		return pon_fapi_to_pa_error(ret);
	}

	*gemport_num = (uint16_t)caps.gem_ports;

	return PON_ADAPTER_SUCCESS;
}

#define COPY_ARRAY_OR_RETURN(dst, src) do { \
	PON_STATIC_ASSERT(sizeof(dst) == sizeof(src), arrays_do_not_match); \
	int err = memcpy_s(dst, sizeof(dst), src, sizeof(src)); \
	if (err) \
		return PON_ADAPTER_ERR_INVALID_VAL; \
} while (0)

/** Get optical properties */
static enum pon_adapter_errno
optic_properties_get(void *ll_handle, struct pa_optic_properties *props)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode ret;
	struct pon_optic_properties tmp;

	if (!ctx)
		return PON_ADAPTER_ERR_INVALID_VAL;

	pthread_mutex_lock(&ctx->lock);
	ret = fapi_pon_optic_properties_get(ctx->pon_ctx, &tmp);
	pthread_mutex_unlock(&ctx->lock);
	if (ret != PON_STATUS_OK)
		return pon_fapi_to_pa_error(ret);

	COPY_ARRAY_OR_RETURN(props->vendor_name, tmp.vendor_name);
	COPY_ARRAY_OR_RETURN(props->vendor_oui, tmp.vendor_oui);
	COPY_ARRAY_OR_RETURN(props->part_number, tmp.part_number);
	COPY_ARRAY_OR_RETURN(props->revision, tmp.revision);
	COPY_ARRAY_OR_RETURN(props->serial_number, tmp.serial_number);
	COPY_ARRAY_OR_RETURN(props->date_code, tmp.date_code);

	props->identifier = tmp.identifier;
	props->connector = tmp.connector;
	props->signaling_rate = tmp.signaling_rate;
	props->tx_wavelength = tmp.tx_wavelength;
	props->high_power_lvl_decl = tmp.high_power_lvl_decl;
	props->paging_implemented_ind = tmp.paging_implemented_ind;
	props->retimer_ind = tmp.retimer_ind;
	props->cooled_transceiver_decl = tmp.cooled_transceiver_decl;
	props->power_lvl_decl = tmp.power_lvl_decl;
	props->linear_rx_output_impl = tmp.linear_rx_output_impl;
	props->rx_decision_thr_impl = tmp.rx_decision_thr_impl;
	props->tunable_transmitter = tmp.tunable_transmitter;
	props->rate_select = tmp.rate_select;
	props->tx_disable = tmp.tx_disable;
	props->tx_fault = tmp.tx_fault;
	props->signal_detect = tmp.signal_detect;
	props->rx_los = tmp.rx_los;
	props->digital_monitoring = tmp.digital_monitoring;
	props->int_calibrated = tmp.int_calibrated;
	props->ext_calibrated = tmp.ext_calibrated;
	props->rx_power_measurement_type = tmp.rx_power_measurement_type;
	props->address_change_req = tmp.address_change_req;
	props->optional_flags_impl = tmp.optional_flags_impl;
	props->soft_tx_disable_monitor = tmp.soft_tx_disable_monitor;
	props->soft_tx_fault_monitor = tmp.soft_tx_fault_monitor;
	props->soft_rx_los_monitor = tmp.soft_rx_los_monitor;
	props->soft_rate_select_monitor = tmp.soft_rate_select_monitor;
	props->app_select_impl = tmp.app_select_impl;
	props->soft_rate_select_ctrl_impl = tmp.soft_rate_select_ctrl_impl;
	props->compliance = tmp.compliance;
	props->pon_mode = get_pa_mode(ctx->cfg.optic.pon_mode);

	return PON_ADAPTER_SUCCESS;
}

/** Get optical properties */
static enum pon_adapter_errno
optic_status_get(void *ll_handle, struct pa_optic_status *props)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode ret;
	struct pon_optic_status tmp;
	enum pon_tx_power_scale scale;

	if (!ctx)
		return PON_ADAPTER_ERR_INVALID_VAL;

	pthread_mutex_lock(&ctx->lock);
	scale = ctx->cfg.optic.tx_power_scale;
	ret = fapi_pon_optic_status_get(ctx->pon_ctx, &tmp, scale);
	pthread_mutex_unlock(&ctx->lock);
	if (ret != PON_STATUS_OK)
		return pon_fapi_to_pa_error(ret);

	props->temperature = tmp.temperature;
	props->voltage = tmp.voltage;
	props->bias = tmp.bias;
	props->tx_power = tmp.tx_power;
	props->rx_power = tmp.rx_power;
	props->rx_los = tmp.rx_los;
	props->tx_disable = tmp.tx_disable;
	props->tx_fault = tmp.tx_fault;

	return PON_ADAPTER_SUCCESS;
}

/* Translate FAPI enum to PON Adapter enum */
static enum pon_ddmi_page to_pon_ddmi_page(enum pa_ddmi_page ddmi_page)
{
	switch (ddmi_page) {
	case PA_DDMI_A0:
		return PON_DDMI_A0;
	case PA_DDMI_A2:
		return PON_DDMI_A2;
	default:
		return PON_DDMI_MAX;
	}

	return PON_DDMI_MAX;
}

/* Get data from optical interface eeprom */
static enum pon_adapter_errno eeprom_data_get(void *ll_handle,
					      enum pa_ddmi_page ddmi_page,
					      unsigned char *data, long offset,
					      size_t data_size)
{
	struct fapi_pon_wrapper_ctx *ctx = ll_handle;
	enum fapi_pon_errorcode ret;
	enum pon_ddmi_page ddmi = to_pon_ddmi_page(ddmi_page);

	if (!ctx || ddmi == PON_DDMI_MAX)
		return PON_ADAPTER_ERR_INVALID_VAL;

	pthread_mutex_lock(&ctx->lock);
	ret = fapi_pon_eeprom_data_get(ctx->pon_ctx, ddmi, data, offset,
				       data_size);
	pthread_mutex_unlock(&ctx->lock);
	if (ret != PON_STATUS_OK)
		return pon_fapi_to_pa_error(ret);

	return PON_ADAPTER_SUCCESS;
}

static const struct pa_system_ops system_ops = {
	.init = init,
	.start = start,
	.reboot = NULL,
	.shutdown = shutdown
};

static const struct pa_system_status_ops sys_sts_ops = {
	.get_pon_op_mode = get_pon_op_mode,
	.get_alarm_status = get_alarm_status,
	.get_pon_status = get_pon_status,
	.set_pon_status = set_pon_status,
};

static const struct pa_system_cap_ops sys_cap_ops = {
	.get_serial_number = get_serial_number,
	.get_gemports = get_gemports,
};

static const struct pa_integrity_ops integrity_ops = {
	.key_get = ik_key_get
};

static const struct pa_omci_me_ops omci_me_ops = {
	.ani_g = &pon_pa_ani_g_ops,
	.enh_sec_ctrl = &pon_pa_enh_sec_ctrl_ops,
	.fec_pmhd = &pon_pa_fec_pmhd_ops,
	.gem_port_net_ctp = &pon_pa_gem_port_net_ctp_ops,
	.gem_port_net_ctp_pmhd = &pon_pa_gem_port_net_ctp_pmhd_ops,
	.enhanced_tc_pmhd = &pon_pa_enhanced_tc_pmhd_ops,
	.mngmt_cnt = &pon_pa_mngmt_cnt_ops,
	.olt_g = &pon_pa_olt_g_ops,
	.onu_g = &pon_pa_onu_g_ops,
	.onu_dyn_pwr_mngmt_ctrl = &pon_pa_onu_dyn_pwr_mngmt_ctrl_ops,
	.twdm_system_profile = &pon_pa_twdm_system_profile_ops,
	.twdm_channel = &pon_pa_twdm_channel_ops,
	.twdm_channel_xgem_pmhd = &pon_pa_twdm_channel_xgem_pmhd_ops,
	.twdm_channel_phy_lods_pmhd = &pon_pa_twdm_channel_phy_lods_pmhd_ops,
	.twdm_channel_ploam_pmhd = &pon_pa_twdm_channel_ploam_pmhd_ops,
	.twdm_channel_tuning_pmhd = &pon_pa_twdm_channel_tuning_pmhd_ops,
};

static const struct pa_ll_dbg_lvl_ops dbg_ops = {
	.set = &dbg_lvl_set,
	.get = &dbg_lvl_get,
};

static const struct pa_optic_ops optic_ops = {
	.eeprom_data_get = eeprom_data_get,
	.optic_properties_get = optic_properties_get,
	.optic_status_get = optic_status_get,
};

static const struct pa_ops pon_pa_ops = {
	.system_ops = &system_ops,
	.sys_sts_ops = &sys_sts_ops,
	.sys_cap_ops = &sys_cap_ops,
	.integrity_ops = &integrity_ops,
	.omci_me_ops = &omci_me_ops,
	.dbg_lvl_ops = &dbg_ops,
	.optic_ops = &optic_ops,
};

enum pon_adapter_errno libpon_ll_register_ops(void *hl_handle_legacy,
					      const struct pa_ops **pa_ops,
					      void **ll_handle,
					      void *hl_handle,
					      uint32_t if_version)
{
	struct fapi_pon_wrapper_ctx *ctx;

	/* In legacy mode, set the new arguments to compatible values. */
	if (hl_handle_legacy) {
		hl_handle = hl_handle_legacy;
		if_version = PA_IF_1ST_VER_NUMBER;
	}

	if (!PA_IF_VERSION_CHECK_COMPATIBLE(if_version))
		return PON_ADAPTER_ERROR;

	ctx = calloc(1, sizeof(*ctx));
	if (!ctx)
		return PON_ADAPTER_ERR_NO_MEMORY;

	ctx->hl_ctx = hl_handle;

	*pa_ops = &pon_pa_ops;
	*ll_handle = ctx;

	return PON_ADAPTER_SUCCESS;
}
