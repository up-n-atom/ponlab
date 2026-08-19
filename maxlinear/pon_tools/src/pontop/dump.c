/******************************************************************************
 *
 * Copyright (c) 2020 - 2024 MaxLinear, Inc.
 * Copyright (c) 2017 - 2020 Intel Corporation
 * Copyright (c) 2012 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

#include <top.h>
#include "pon_top.h"
#include "dump.h"

#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <inttypes.h>

#include <fapi_pon.h>
#include <fapi_pon_alarms.h>

#ifdef HAVE_CONFIG_H
#include "pon_tools.h"
#endif

#define glue_u32(a, b, c, d) \
	((uint32_t)a << 24 | b << 16 | c << 8 | d)

/* FW message PONFW_GET_STATIC_ALARM_CMD_ID can report up
 * to 32 concurrent alarms.
 */
#define MAX_ACT_ALARMS 32

/* Max number of TWDM wavelenghts actually supported */
#define TWDM_MAX_NO_CHANNELS 4

struct active_alarms {
	int size;
	const struct alarm_type *type[MAX_ACT_ALARMS];
};

static struct table_counter_state {
	struct pon_version pon_version;
	struct pon_cap pon_cap;
	struct pon_gpon_status gpon_status;
	struct pon_serial_number pon_serial_number;
	struct pon_registration_id pon_registration_id;
	struct pon_gtc_counters pon_gtc_counters;
	struct pon_xgtc_counters pon_xgtc_counters;
	struct pon_fec_counters pon_fec_counters;
	struct pon_ploam_ds_counters pon_ploam_ds_counters;
	struct pon_ploam_us_counters pon_ploam_us_counters;
	struct pon_optic_status pon_optic_status;
	struct pon_optic_properties pon_optic_properties;
	struct pon_gem_port pon_gem_port;
	struct pon_ploam_state pon_ploam_state;
	struct pon_alarm_status pon_alarm_status;
	struct pon_gpon_cfg pon_gpon_cfg;
	struct pon_range_limits pon_range_limits;
	struct pon_gem_port_counters pon_gem_port_counters;
	struct pon_psm_cfg pon_psm_cfg;
	struct pon_psm_counters pon_psm_counters;
	struct pon_psm_state pon_psm_state;
	struct pon_psm_fsm_time pon_psm_fsm_time;
	struct pon_debug_burst_profile pon_debug_burst_profiles[8];
	struct pon_eth_counters pon_eth_counters;
	struct pon_optic_cfg pon_optic_cfg;
	struct active_alarms active_level_alarms;
	struct active_alarms active_edge_alarms;
	uint32_t alarm_entry;
	struct pon_twdm_optic_pl_counters
		twdm_optic_pl_counters[TWDM_MAX_NO_CHANNELS];
	struct pon_twdm_ploam_ds_counters
		twdm_ploam_ds_counters[TWDM_MAX_NO_CHANNELS];
	struct pon_twdm_ploam_us_counters
		twdm_ploam_us_counters[TWDM_MAX_NO_CHANNELS];
	struct pon_twdm_tuning_counters
		twdm_tuning_counters[TWDM_MAX_NO_CHANNELS];
	struct pon_twdm_xgtc_counters
		twdm_xgtc_counters[TWDM_MAX_NO_CHANNELS];
	struct pon_twdm_channel_profile
		twdm_channel_profile[TWDM_MAX_NO_CHANNELS];
	struct pon_gem_port_counters
		twdm_gem_port_counters[TWDM_MAX_NO_CHANNELS];
} *state;

static bool cached_optic_info, optic_status_available;

void table_group_init(int init)
{
	if (init) {
		state = calloc(1, sizeof(*state));

		if (!state) {
			fprintf(stderr, "No free memory\n");
			abort();
		}

	} else {
		free(state);
	}
}

/* TODO: to simplify testing, we could move table_get_ functions to
 * separate file (and create additional one for emulation)
 */
int table_get_status(struct top_context *ctx,
		     const char *input_file_name)
{
	UNUSED(input_file_name);

	fapi_pon_version_get(ctx->priv, &state->pon_version);
	fapi_pon_cap_get(ctx->priv, &state->pon_cap);
	fapi_pon_gpon_status_get(ctx->priv, &state->gpon_status);

	/* This is number of lines in ptop page */
	return 34;
}

static const char *get_ploam_state_2nd_form(uint32_t value)
{
	switch (value) {
	case 00:
		return "O0, Power-up state";
	case 10:
		return "O1, Initial state";
	case 11:
		return "O1.1, Off-sync state";
	case 12:
		return "O1.2, Profile learning state";
	case 20:
		return "O2, Stand-by state";
	case 23:
		return "O2.3, Serial number state";
	case 30:
		return "O3, Serial number state";
	case 40:
		return "O4, Ranging state";
	case 50:
		return "O5, Operation state";
	case 51:
		return "O5.1, Associated state";
	case 52:
		return "O5.2, Pending state";
	case 60:
		return "O6, Intermittent LOS state";
	case 70:
		return "O7, Emergency stop state";
	case 71:
		return "O7.1, Emergency stop off-sync state";
	case 72:
		return "O7.2, Emergency stop in-sync state";
	case 81:
		return "O8.1, Downstream tuning off-sync state";
	case 82:
		return "O8.2, Downstream tuning profile learning state";
	case 90:
		return "O9, Upstream tuning state";
	default:
		return "(error)";
	}
}

static const char *get_odn_class(uint32_t value)
{
	switch (value) {
	case 0:
		return "N1";
	case 1:
		return "N2";
	case 2:
		return "Reserved";
	case 3:
		return "E1";
	case 4:
		return "E2";
	case 5:
		return "Reserved";
	case 6:
		return "B+";
	case 7:
		return "C+";
	default:
		return "(unknown ODN class)";
	}
}

static const char *get_link_type(uint32_t value)
{
	switch (value) {
	case 0:
		return "Unspecified";
	case 1:
		return "Type B only";
	case 2:
		return "Type A only";
	case 3:
		return "Type A and B";
	default:
		return "(unknown link type)";
	}
}

char *table_entry_get_status(struct top_context *ctx,
			     const int entry,
			     char *text)
{
	uint64_t oc_pon_id =
		(uint64_t)state->gpon_status.pon_id[0] << (6 * 8) |
		(uint64_t)state->gpon_status.pon_id[1] << (5 * 8) |
		(uint64_t)state->gpon_status.pon_id[2] << (4 * 8) |
		(uint64_t)state->gpon_status.pon_id[3] << (3 * 8) |
		(uint64_t)state->gpon_status.pon_id[4] << (2 * 8) |
		(uint64_t)state->gpon_status.pon_id[5] << (1 * 8) |
		(uint64_t)state->gpon_status.pon_id[6] << (0 * 8);

	int G_PON = state->pon_cap.features & PON_FEATURE_G984;

	UNUSED(ctx);

	switch (entry) {
	case -1:
		sprintf(text, "%-50s  %s", "OPTION", "VALUE");
		break;
	case 0:
		sprintf(text, "%-50s : %d", "PON IP HW version",
			state->pon_version.hw_version);
		break;
	case 1:
		sprintf(text, "%-50s : %d.%d.%d.%d.%d-%d",
			 "PON IP FW version",
			state->pon_version.fw_version_major,
			state->pon_version.fw_version_minor,
			state->pon_version.fw_version_standard,
			state->pon_version.fw_version_platform,
			state->pon_version.fw_version_patch,
			state->pon_version.fw_timestamp);
		break;
	case 2:
		sprintf(text, "%-50s : %d.%d.%d", "PON IP SW version",
			(state->pon_version.sw_version & 0x00ff0000) >> 16,
			(state->pon_version.sw_version & 0x0000ff00) >> 8,
			(state->pon_version.sw_version & 0x000000ff) >> 0);
		break;
	case 3:
		sprintf(text, "%-50s : %s", "PON IP pontop version",
			PACKAGE_VERSION);
		break;
	case 4:
		sprintf(text, " ");
		break;
	case 5:
		sprintf(text, "%-50s : %s", "SW uptime",
			"t.b.d");
		break;
	case 6:
		sprintf(text, " ");
		break;
	case 7:
		sprintf(text, "%-50s : %s", "PON type active",
			"t.b.d");
		break;
	case 8:
		sprintf(text, " ");
		break;
	case 9:
		sprintf(text, "%-50s : %08x", "PON capability",
			state->pon_cap.features);
		break;
	case 10:
		sprintf(text, "%-2s %-47s : %u", " ", "GEM Ports",
			state->pon_cap.gem_ports);
		break;
	case 11:
		sprintf(text, "%-4s %-45s : %s", " ", "Allocations",
			"t.b.d xGPON mode currently not supported");
		break;
	case 12:
		sprintf(text, " ");
		break;
	case 13:
		sprintf(text, "%-50s : %s", "PON PLOAM Status",
			get_ploam_state_2nd_form(
				state->gpon_status.ploam_state));
		break;
	case 14:
		sprintf(text, " ");
		break;
	case 15:
		sprintf(text, " ");
		break;
	case 16:
		sprintf(text, "%-50s : %s", "FEC upstream",
			state->gpon_status.fec_status_us ?
			"ON" : "OFF");
		break;
	case 17:
		sprintf(text, "%-50s : %s", "FEC downstream",
			state->gpon_status.fec_status_ds ?
			"ON" : "OFF");
		break;
	case 18:
		sprintf(text, " ");
		break;
	case 19:
		sprintf(text, "%-50s : %u", "ONU Authentication Status",
			state->gpon_status.auth_status);
		break;
	case 20:
		sprintf(text, " ");
		break;
	case 21:
		sprintf(text, "OC Structure contents");
		break;
	case 22:
		sprintf(text, "---------------------");
		break;
	case 23:
		if (G_PON)
			sprintf(text, "%-50s : 0x%016"PRIx64, "PON ID",
				oc_pon_id);
		else
			sprintf(text, "%-50s : 0x%08"PRIx64, "PON ID",
				oc_pon_id >> 24);
		break;
	case 24:
		if ((state->gpon_status.oc_tol & 0x1FF) == 0x1FF) {
			sprintf(text, "%-50s : %s", "TOL", "Not supported");
		} else {
			uint32_t tol = state->gpon_status.oc_tol & 0x1FF;

			sprintf(text, "%-50s : %.1f dBm", "TOL",
				-30.0f + tol / 10.0f);
		}
		break;
	case 25:
		if ((state->gpon_status.oc_pit >> 7) & 0x1) {
			sprintf(text, "%-50s : %s", "RE",
				"Read extender present");
		} else {
			sprintf(text, "%-50s : %s", "RE",
				"No reach extender present");
		}
		break;
	case 26:
		sprintf(text, "%-50s : %s", "ODN Class",
			get_odn_class(state->gpon_status.oc_pit >> 4 & 0x7));
		break;
	case 27:
		if ((state->gpon_status.oc_pit >> 3) & 0x1) {
			sprintf(text, "%-50s : %s", "DS FEC",
				"Downstream FEC enabled");
		} else {
			sprintf(text, "%-50s : %s", "DS FEC",
				"Downstream FEC disabled");
		}
		break;
	case 28:
		if ((state->gpon_status.oc_pit >> 2) & 0x1) {
			sprintf(text, "%-50s : %s", "P",
				"TC mode is G.989.3");
		} else {
			sprintf(text, "%-50s : %s", "P",
				"TC mode is G.987.3");
		}
		break;
	case 29:
		sprintf(text, "%-50s : %s", "Link Type",
			get_link_type(state->gpon_status.oc_pit >> 0 & 0x3));
		break;
	case 30:
		if (!G_PON)
			sprintf(text, "%-50s : 0x%08" PRIx64, "Administrative Label",
				oc_pon_id >> (24 + 4) & 0x0FFFFFFF);
		break;
	case 31:
		if (!G_PON)
			sprintf(text, "%-50s : 0x%" PRIx64, "DWLCH ID",
				oc_pon_id >> 24 & 0xF);
		break;
	case 32:
		sprintf(text, "%-50s : %s", "R", "value not available");
		break;
	case 33:
		sprintf(text, "%-50s : %s", "C", "value not available");
		break;
	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

int table_get_cap(struct top_context *ctx,
		const char *input_file_name)
{
	UNUSED(input_file_name);

	fapi_pon_serial_number_get(ctx->priv, &state->pon_serial_number);
	fapi_pon_registration_id_get(ctx->priv, &state->pon_registration_id);
	fapi_pon_cap_get(ctx->priv, &state->pon_cap);

	/* This is number of lines in ptop page */
	return 15;
}

char *table_entry_get_cap(struct top_context *ctx,
			     const int entry,
			     char *text)
{
	UNUSED(ctx);

	switch (entry) {
	case -1:
		sprintf(text, "%-50s  %s", "OPTION", "VALUE");
		break;
	case 0:
		sprintf(text, "%-50s : %c%c%c%c%02X%02X%02X%02X",
			 "Serial number",
			state->pon_serial_number.serial_no[0],
			state->pon_serial_number.serial_no[1],
			state->pon_serial_number.serial_no[2],
			state->pon_serial_number.serial_no[3],
			state->pon_serial_number.serial_no[4],
			state->pon_serial_number.serial_no[5],
			state->pon_serial_number.serial_no[6],
			state->pon_serial_number.serial_no[7]);
		break;
	case 1:
		sprintf(text, "%-50s : %08x-%08x-%08x-%08x-%08x-%08x-"
			"%08x-%08x-%08x", "Registration ID",
			glue_u32(state->pon_registration_id.reg_id[0],
			state->pon_registration_id.reg_id[1],
			state->pon_registration_id.reg_id[2],
			state->pon_registration_id.reg_id[3]),
			glue_u32(state->pon_registration_id.reg_id[4],
			state->pon_registration_id.reg_id[5],
			state->pon_registration_id.reg_id[6],
			state->pon_registration_id.reg_id[7]),
			glue_u32(state->pon_registration_id.reg_id[8],
			state->pon_registration_id.reg_id[9],
			state->pon_registration_id.reg_id[10],
			state->pon_registration_id.reg_id[11]),
			glue_u32(state->pon_registration_id.reg_id[12],
			state->pon_registration_id.reg_id[13],
			state->pon_registration_id.reg_id[14],
			state->pon_registration_id.reg_id[15]),
			glue_u32(state->pon_registration_id.reg_id[16],
			state->pon_registration_id.reg_id[17],
			state->pon_registration_id.reg_id[18],
			state->pon_registration_id.reg_id[19]),
			glue_u32(state->pon_registration_id.reg_id[20],
			state->pon_registration_id.reg_id[21],
			state->pon_registration_id.reg_id[22],
			state->pon_registration_id.reg_id[23]),
			glue_u32(state->pon_registration_id.reg_id[24],
			state->pon_registration_id.reg_id[25],
			state->pon_registration_id.reg_id[26],
			state->pon_registration_id.reg_id[27]),
			glue_u32(state->pon_registration_id.reg_id[28],
			state->pon_registration_id.reg_id[29],
			state->pon_registration_id.reg_id[30],
			state->pon_registration_id.reg_id[31]),
			glue_u32(state->pon_registration_id.reg_id[32],
			state->pon_registration_id.reg_id[33],
			state->pon_registration_id.reg_id[34],
			state->pon_registration_id.reg_id[35]));
		break;
	case 2:
		sprintf(text, " ");
		break;
	case 3:
		sprintf(text, "Capability details");
		break;
	case 4:
		sprintf(text, "------------------");
		break;
	case 5:
		sprintf(text, "%-50s : %s%s%s%s", "Basic mode(s)",
			state->pon_cap.features & PON_FEATURE_G984 ?
			"G.984|" : "",
			state->pon_cap.features & PON_FEATURE_G987 ?
			"G.987|" : "",
			state->pon_cap.features & PON_FEATURE_G989 ?
			"G.989|" : "",
			state->pon_cap.features & PON_FEATURE_G9807 ?
			"G.9807|" : "");
		break;
	case 6:
		sprintf(text, "%-50s : %s %s%s%s%s", "OMCI support",
			"OMCI Annex",
			state->pon_cap.features & PON_FEATURE_ANXC ?
			"C|" : "",
			state->pon_cap.features & PON_FEATURE_ANXD ?
			"D|" : "",
			state->pon_cap.features & PON_FEATURE_ANXE ?
			"E|" : "",
			state->pon_cap.features & PON_FEATURE_ANXF ?
			"F" : "");
		break;
	case 7:
		sprintf(text, "%-50s : %s%s%s", "Power saving mode(s)",
			state->pon_cap.features & PON_FEATURE_DOZE ?
			"doze|" : "",
			state->pon_cap.features & PON_FEATURE_CSLP ?
			"cyclic sleep|" : "",
			state->pon_cap.features & PON_FEATURE_WSLP ?
			"watchful sleep|" : "");
		break;
	case 8:
		sprintf(text, "%-50s : %s%s", "DBA mode(s)",
			state->pon_cap.features & PON_FEATURE_DBAM0 ?
			"mode0|" : "",
			state->pon_cap.features & PON_FEATURE_DBAM1 ?
			"mode1" : "");
		break;
	case 9:
		sprintf(text, "%-50s : %s%s%s", "Crypto mode(s)",
			state->pon_cap.features & PON_FEATURE_CRY1 ?
			"AES-CMAC-128|" : "",
			state->pon_cap.features & PON_FEATURE_CRY2 ?
			"HMAC-SHA-256|" : "",
			state->pon_cap.features & PON_FEATURE_CRY3 ?
			"HMAC-SHA-512" : "");
		break;
	case 10:
		sprintf(text, " ");
		break;
	case 11:
		sprintf(text, "%-50s : %u", "GEM Ports",
			state->pon_cap.gem_ports);
		break;
	case 12:
		sprintf(text, "%-50s : %u", "Allocations",
			state->pon_cap.alloc_ids);
		break;
	case 13:
		sprintf(text, " ");
		break;
	case 14:
		sprintf(text, "%-50s : %s", "Debug mode",
			state->pon_cap.features & PON_FEATURE_DBG ?
			"enabled" : "disabled");
		break;

	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

int table_get_lan(struct top_context *ctx,
	const char *input_file_name)
{
	UNUSED(ctx);
	UNUSED(input_file_name);

	/* This is number of lines in ptop page */
	return 1;
}

char *table_entry_get_lan(struct top_context *ctx,
				int entry,
				char *text)
{
	UNUSED(ctx);

	if (entry == -1) {
		sprintf(text, "%-50s %s",
		"OPTION", "VALUE");
		return NULL;
	}
	sprintf(text, "t.b.d missing data");

	return NULL;
}

int table_get_gem_port_status(struct top_context *ctx,
		const char *input_file_name)
{
	UNUSED(input_file_name);

	fapi_pon_limits_get(ctx->priv, &state->pon_range_limits);

	/* This is number of lines in ptop page */
	return state->pon_range_limits.gem_port_idx_max + 1;
}

static const char *get_payload_type_text(uint32_t value)
{
	static const char * const description[] = {"Ethernet", "OMCI",
						  "Reserved", "Other"};

	if (value >= (sizeof(description)/sizeof(const char *)))
		return "(error)";

	return description[value];
}

static const char *get_enc_key_ring_text(uint32_t value)
{
	static const char * const description[] = {"None", "Unicast",
						  "Broadcast", "Uni. DS"};

	if (value >= (sizeof(description)/sizeof(const char *)))
		return "(error)";

	return description[value];
}

static const char *get_direction_text(uint32_t ds, uint32_t us)
{
	uint32_t value = ds + (us << 1);
	static const char * const description[] = {"Disabled", "DS", "US",
						   "DS + US"};

	if (value >= (sizeof(description)/sizeof(const char *)))
		return "(error)";

	return description[value];
}

static const char *get_isvalid_text(uint32_t value)
{
	static const char * const description[] = {"Invalid", "Valid"};

	if (value >= (sizeof(description)/sizeof(const char *)))
		return "(error)";

	return description[value];
}

static void get_valid_alloc_id(uint32_t value, char *buf)
{
	if (value > state->pon_range_limits.alloc_id_max)
		sprintf(buf, "%s", "n.a.");
	else
		sprintf(buf, "%" PRId64 "", (int64_t)value);
}

char *table_entry_get_gem_port_status(struct top_context *ctx,
					int entry,
					char *text)
{
	static char valid_alloc_id_buf[12];

	if (entry <= -1) {
		sprintf(text, "%-10s %-10s %-10s %-13s %-13s %-15s %-15s %-15s",
		"GEM Index", "GEM ID", "Alloc Id", "Alloc Id st.", "Data/OMCI",
		"Max. Size", "Encryption k.r.", "Direction");
		return NULL;
	}

	if (fapi_pon_gem_port_index_get(ctx->priv, (uint32_t) entry,
				&state->pon_gem_port) != PON_STATUS_OK)
		return NULL;

	get_valid_alloc_id(state->pon_gem_port.alloc_id, valid_alloc_id_buf);

	sprintf(text, "%-10u %-10u %-10s %-13s %-13s %-15u %-15s %-15s",
		state->pon_gem_port.gem_port_index,
		state->pon_gem_port.gem_port_id,
		valid_alloc_id_buf,
		get_isvalid_text(state->pon_gem_port.alloc_valid),
		get_payload_type_text(state->pon_gem_port.payload_type),
		state->pon_gem_port.gem_max_size,
		get_enc_key_ring_text(state->pon_gem_port.encryption_key_ring),
		get_direction_text(state->pon_gem_port.is_downstream,
				   state->pon_gem_port.is_upstream));

	return NULL;
}

int table_get_gem_port_counters(struct top_context *ctx,
			const char *input_file_name)
{
	UNUSED(input_file_name);

	fapi_pon_limits_get(ctx->priv, &state->pon_range_limits);

	/* This is number of lines in ptop page */
	return state->pon_range_limits.gem_port_idx_max + 1;
}

char *table_entry_get_gem_port_counters(struct top_context *ctx,
					int entry,
					char *text)
{
	if (entry == -1) {
		sprintf(text, "%-15s %-15s %-15s %-15s %-15s %-15s %-15s",
		"GEM Index", "GEM ID",
		"u/s packets", "u/s bytes",
		"d/s packets", "d/s bytes",
		"Key Errors");
		return NULL;
	}

	if (fapi_pon_gem_port_index_get(ctx->priv, (uint32_t) entry,
				&state->pon_gem_port) != PON_STATUS_OK)
		return NULL;

	if (fapi_pon_gem_port_counters_get(ctx->priv,
			state->pon_gem_port.gem_port_id,
			&state->pon_gem_port_counters) != PON_STATUS_OK)
		return NULL;

	sprintf(text, "%-15u %-15u %-15"PRId64" %-15"PRId64" %-15"PRId64
		" %-15"PRId64" %-15"PRId64,
		state->pon_gem_port.gem_port_index,
		state->pon_gem_port_counters.gem_port_id,
		state->pon_gem_port_counters.tx_frames,
		state->pon_gem_port_counters.tx_bytes,
		state->pon_gem_port_counters.rx_frames,
		state->pon_gem_port_counters.rx_bytes,
		state->pon_gem_port_counters.key_errors);

	return NULL;
}

int table_get_gem_port_ds_counters(struct top_context *ctx,
				const char *input_file_name)
{
	UNUSED(ctx);
	UNUSED(input_file_name);

	/* This is number of lines in ptop page */
	return 1;
}

char *table_entry_get_gem_port_ds_counters(struct top_context *ctx,
					int entry,
					char *text)
{
	UNUSED(ctx);

	if (entry == -1) {
		sprintf(text, "%-15s %-15s %-15s %-15s %-15s %-15s %-15s"
		"%-15s %-15s",
		"GEM Index", "all",
		"<64", "64-127",
		"128-255", "256-511",
		"512-1023", "1024-1518",
		">1518");
		return NULL;
	}

	sprintf(text, "t.b.d xGPON mode currently not supported");

	return NULL;
}

int table_get_gem_port_us_counters(struct top_context *ctx,
				const char *input_file_name)
{
	UNUSED(ctx);
	UNUSED(input_file_name);

	/* This is number of lines in ptop page */
	return 1;
}

char *table_entry_get_gem_port_us_counters(struct top_context *ctx,
					int entry,
					char *text)
{
	UNUSED(ctx);

	if (entry == -1) {
		sprintf(text, "%-15s %-15s %-15s %-15s %-15s %-15s %-15s"
		"%-15s %-15s",
		"GEM Index", "all",
		"<64", "64-127",
		"128-255", "256-511",
		"512-1023", "1024-1518",
		">1518");
		return NULL;
	}

	sprintf(text, "t.b.d xGPON mode currently not supported");

	return NULL;
}

int table_get_gem_port_ethernet_ds_counters(struct top_context *ctx,
					    const char *input_file_name)
{
	UNUSED(input_file_name);

	fapi_pon_limits_get(ctx->priv, &state->pon_range_limits);

	/* This is number of lines in pontop page */
	return state->pon_range_limits.gem_port_idx_max + 1;
}

char *table_entry_get_gem_port_ethernet_ds_counters(struct top_context *ctx,
						    int entry,
						    char *text)
{
	if (entry == -1) {
		sprintf(text, "%-10s %-10s %-15s %-15s %-15s %-15s %-15s",
		"GEM Index", "GEM ID",
		"bytes <64", "<128", "<512", "<=1518", ">1518");
		return NULL;
	}

	if (fapi_pon_gem_port_index_get(ctx->priv, (uint32_t) entry,
				&state->pon_gem_port) != PON_STATUS_OK)
		return NULL;

	if (fapi_pon_eth_rx_counters_get(ctx->priv,
			state->pon_gem_port.gem_port_id,
			&state->pon_eth_counters) != PON_STATUS_OK)
		return NULL;

	sprintf(text, "%-10u %-10u %-15"PRId64" %-15"PRId64" %-15"PRId64
		" %-15"PRId64" %-15"PRId64,
		state->pon_gem_port.gem_port_index,
		state->pon_gem_port.gem_port_id,
		state->pon_eth_counters.frames_lt_64,
		state->pon_eth_counters.frames_64 +
			state->pon_eth_counters.frames_65_127,
		state->pon_eth_counters.frames_128_255 +
			state->pon_eth_counters.frames_256_511,
		state->pon_eth_counters.frames_512_1023 +
			state->pon_eth_counters.frames_1024_1518,
		state->pon_eth_counters.frames_gt_1518);

	return NULL;
}

int table_get_gem_port_ethernet_us_counters(struct top_context *ctx,
					    const char *input_file_name)
{
	UNUSED(input_file_name);

	fapi_pon_limits_get(ctx->priv, &state->pon_range_limits);

	/* This is number of lines in pontop page */
	return state->pon_range_limits.gem_port_idx_max + 1;
}

char *table_entry_get_gem_port_ethernet_us_counters(struct top_context *ctx,
						    int entry,
						    char *text)
{
	if (entry == -1) {
		sprintf(text, "%-10s %-10s %-15s %-15s %-15s %-15s %-15s",
		"GEM Index", "GEM ID",
		"bytes <64", "<128", "<512", "<=1518", ">1518");
		return NULL;
	}

	if (fapi_pon_gem_port_index_get(ctx->priv, (uint32_t) entry,
				&state->pon_gem_port) != PON_STATUS_OK)
		return NULL;

	if (fapi_pon_eth_tx_counters_get(ctx->priv,
			state->pon_gem_port.gem_port_id,
			&state->pon_eth_counters) != PON_STATUS_OK)
		return NULL;

	sprintf(text, "%-10u %-10u %-15"PRId64" %-15"PRId64" %-15"PRId64
		" %-15"PRId64" %-15"PRId64,
		state->pon_gem_port.gem_port_index,
		state->pon_gem_port.gem_port_id,
		state->pon_eth_counters.frames_lt_64,
		state->pon_eth_counters.frames_64 +
			state->pon_eth_counters.frames_65_127,
		state->pon_eth_counters.frames_128_255 +
			state->pon_eth_counters.frames_256_511,
		state->pon_eth_counters.frames_512_1023 +
			state->pon_eth_counters.frames_1024_1518,
		state->pon_eth_counters.frames_gt_1518);

	return NULL;
}

int table_get_fec_status(struct top_context *ctx,
		     const char *input_file_name)
{
	UNUSED(input_file_name);

	fapi_pon_gpon_status_get(ctx->priv, &state->gpon_status);
	fapi_pon_gtc_counters_get(ctx->priv, &state->pon_gtc_counters);
	fapi_pon_fec_counters_get(ctx->priv, &state->pon_fec_counters);

	/* This is number of lines in ptop page */
	return 9;
}

char *table_entry_get_fec_status(struct top_context *ctx,
			     const int entry,
			     char *text)
{
	UNUSED(ctx);

	switch (entry) {
	case -1:
		sprintf(text, "%-50s  %s", "OPTION", "VALUE");
		break;
	case 0:
		sprintf(text, "%-50s : %s", "FEC upstream",
			state->gpon_status.fec_status_us ?
			"ON" : "OFF");
		break;
	case 1:
		sprintf(text, "%-50s : %s", "FEC downstream",
			state->gpon_status.fec_status_ds ?
			"ON" : "OFF");
		break;
	case 2:
		sprintf(text, " ");
		break;
	case 3:
		sprintf(text, "%-50s : %"PRId64, "BIP errors",
			state->pon_gtc_counters.bip_errors);
		break;
	case 4:
		if (state->gpon_status.fec_status_ds) {
			sprintf(text, "%-50s : %"PRId64,
				 "Total FEC codewords",
				state->pon_fec_counters.words);
		}
		break;
	case 5:
		if (state->gpon_status.fec_status_ds) {
			sprintf(text, "%-50s : %"PRId64,
				 "Corrected FEC codewords",
				state->pon_fec_counters.words_corr);
		}
		break;
	case 6:
		if (state->gpon_status.fec_status_ds) {
			sprintf(text, "%-50s : %"PRId64,
				 "Uncorrected FEC codewords",
				state->pon_fec_counters.words_uncorr);
		}
		break;
	case 7:
		if (state->gpon_status.fec_status_ds) {
			sprintf(text, "%-50s : %"PRId64,
				 "Corrected FEC bytes",
				state->pon_fec_counters.bytes_corr);
		}
		break;
	case 8:
		if (state->gpon_status.fec_status_ds) {
			sprintf(text, "%-50s : %"PRId64,
				 "FEC errored seconds",
				state->pon_fec_counters.seconds);
		}
		break;

	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

int table_get_gtc_status(struct top_context *ctx,
		     const char *input_file_name)
{
	UNUSED(input_file_name);

	fapi_pon_gtc_counters_get(ctx->priv, &state->pon_gtc_counters);
	fapi_pon_xgtc_counters_get(ctx->priv, &state->pon_xgtc_counters);

	/* This is number of lines in ptop page */
	return 6;
}

static char *table_entry_get_gtc_status_gpon(struct top_context *ctx,
			     const int entry,
			     char *text)
{
	UNUSED(ctx);

	switch (entry) {
	case -1:
		sprintf(text, "%-50s  %s", "OPTION", "VALUE");
		break;
	case 0:
		sprintf(text, "%-50s : %"PRId64,
			"BIP errors",
			state->pon_gtc_counters.bip_errors);
		break;
	case 1:
		sprintf(text, "%-50s : %"PRId64,
			"Discarded GEM frames",
			state->pon_gtc_counters.disc_gem_frames);
		break;
	case 2:
		sprintf(text, "%-50s : %"PRId64,
			"GEM HEC errors corrected",
			state->pon_gtc_counters.gem_hec_errors_corr);
		break;
	case 3:
		sprintf(text, "%-50s : %"PRId64,
			"GEM HEC errors uncorrected",
			state->pon_gtc_counters.gem_hec_errors_uncorr);
		break;
	case 4:
		sprintf(text, "%-50s : %"PRId64,
			"Bandwidth map errors corrected",
			state->pon_gtc_counters.bwmap_hec_errors_corr);
		break;

	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

static char *table_entry_get_gtc_status_pon(struct top_context *ctx,
			     const int entry,
			     char *text)
{
	UNUSED(ctx);

	switch (entry) {
	case -1:
		sprintf(text, "%-50s  %s", "OPTION", "VALUE");
		break;
	case 0:
		sprintf(text, "%-50s : %"PRId64,
			"PSBd HEC errors corrected",
			state->pon_xgtc_counters.psbd_hec_err_corr);
		break;
	case 1:
		sprintf(text, "%-50s : %"PRId64,
			"PSBd HEC errors uncorrected",
			state->pon_xgtc_counters.psbd_hec_err_uncorr);
		break;
	case 2:
		sprintf(text, "%-50s : %"PRId64,
			"FS HEC errors corrected",
			state->pon_xgtc_counters.fs_hec_err_corr);
		break;
	case 3:
		sprintf(text, "%-50s : %"PRId64,
			"FS HEC errors uncorrected",
			state->pon_xgtc_counters.fs_hec_err_uncorr);
		break;
	case 4:
		sprintf(text, "%-50s : %"PRId64,
			"Lost words due to HEC errors",
			state->pon_xgtc_counters.lost_words);
		break;
	case 5:
		sprintf(text, "%-50s : %"PRId64,
			"PLOAM MIC errors",
			state->pon_xgtc_counters.ploam_mic_err);
		break;

	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

char *table_entry_get_gtc_status(struct top_context *ctx,
			     const int entry,
			     char *text)
{
	fapi_pon_version_get(ctx->priv, &state->pon_version);

	if (state->pon_version.fw_version_standard == 1) {
		return table_entry_get_gtc_status_gpon(ctx,
			entry, text);
	} else {
		return table_entry_get_gtc_status_pon(ctx,
			entry, text);
	}
}

int table_get_psm_cfg(struct top_context *ctx,
		      const char *input_file_name)
{
	UNUSED(input_file_name);

	fapi_pon_psm_cfg_get(ctx->priv, &state->pon_psm_cfg);
	fapi_pon_psm_time_get(ctx->priv, &state->pon_psm_fsm_time);

	/* This is number of lines in ptop page */
	return 18;
}

char *table_entry_get_psm_cfg(struct top_context *ctx,
			      const int entry,
			      char *text)
{
	UNUSED(ctx);

	switch (entry) {
	case -1:
		sprintf(text, "%-50s  %s", "OPTION", "VALUE");
		break;
	case 0:
		sprintf(text, "%-50s : %s", "PSM",
			state->pon_psm_cfg.enable ? "enabled" : "disabled");
		break;
	case 1:
		sprintf(text, " ");
		break;
	case 2:
		sprintf(text, "%-50s : %u", "Maximum sleep interval",
			state->pon_psm_cfg.max_sleep_interval);
		break;
	case 3:
		sprintf(text, "%-50s : %u", "Minimum aware interval",
			state->pon_psm_cfg.min_aware_interval);
		break;
	case 4:
		sprintf(text, "%-50s : %u", "Minimum active held interval",
			state->pon_psm_cfg.min_active_held_interval);
		break;
	case 5:
		sprintf(text, "%-50s : %u", "Maximum Rx off interval",
			state->pon_psm_cfg.max_rx_off_interval);
		break;
	case 6:
		sprintf(text, " ");
		break;
	case 7:
		sprintf(text, "%-50s : %u", "State idle",
			state->pon_psm_fsm_time.state_idle);
		break;
	case 8:
		sprintf(text, "%-50s : %u", "State active",
			state->pon_psm_fsm_time.state_active);
		break;
	case 9:
		sprintf(text, "%-50s : %u", "State active held",
			state->pon_psm_fsm_time.state_active_held);
		break;
	case 10:
		sprintf(text, "%-50s : %u", "State active free",
			state->pon_psm_fsm_time.state_active_free);
		break;
	case 11:
		sprintf(text, "%-50s : %u", "State asleep",
			state->pon_psm_fsm_time.state_asleep);
		break;
	case 12:
		sprintf(text, "%-50s : %u", "State listen",
			state->pon_psm_fsm_time.state_listen);
		break;
	case 13:
		sprintf(text, "%-50s : %u", "State watch",
			state->pon_psm_fsm_time.state_watch);
		break;
	case 14:
		sprintf(text, "%-50s : %u", "State doze aware",
			state->pon_psm_fsm_time.state_doze_aware);
		break;
	case 15:
		sprintf(text, "%-50s : %u", "State watch aware",
			state->pon_psm_fsm_time.state_watch_aware);
		break;
	case 16:
		sprintf(text, "%-50s : %u", "State sleep aware",
			state->pon_psm_fsm_time.state_sleep_aware);
		break;
	case 17:
		sprintf(text, "%-50s : %u", "State wait",
			state->pon_psm_fsm_time.state_wait);
		break;
	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

/* TODO: t.b.d - debug counters t1,t2,t3 */

int table_get_power_status(struct top_context *ctx,
		     const char *input_file_name)
{
	UNUSED(input_file_name);

	fapi_pon_psm_counters_get(ctx->priv, &state->pon_psm_counters);
	fapi_pon_psm_state_get(ctx->priv, &state->pon_psm_state);

	/* This is number of lines in ptop page */
	return 7;
}

char *table_entry_get_power_status(struct top_context *ctx,
			     const int entry,
			     char *text)
{
	UNUSED(ctx);

	static const char * const power_state[] = {
			"Idle", "Active", "Active held", "Active free",
			"Sleep aware", "Asleep", "Doze aware", "Listen",
			"Watch aware", "Watch", "Wait"};

	switch (entry) {
	case -1:
		sprintf(text, "%-50s  %s", "OPTION", "VALUE");
		break;
	case 0:
		sprintf(text, "%-50s : %s", "Power Save State",
			power_state[state->pon_psm_state.current]);
		break;
	case 1:
		sprintf(text, " ");
		break;
	case 2:
		sprintf(text, "%-50s : %" PRIu64 " us", "Total time",
			state->pon_psm_counters.doze_time +
			state->pon_psm_counters.cyclic_sleep_time +
			state->pon_psm_counters.watchful_sleep_time);
		break;
	case 3:
		sprintf(text, " ");
		break;
	case 4:
		sprintf(text, "%-50s : %" PRIu64 " us", "Doze time",
			state->pon_psm_counters.doze_time);
		break;
	case 5:
		sprintf(text, "%-50s : %" PRIu64 " us", "Cyclic sleep time",
			state->pon_psm_counters.cyclic_sleep_time);
		break;
	case 6:
		sprintf(text, "%-50s : %" PRIu64 " us", "Watchful sleep time",
			state->pon_psm_counters.watchful_sleep_time);
		break;

	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

int table_get_alloc_counters(struct top_context *ctx,
		     const char *input_file_name)
{
	UNUSED(input_file_name);

	fapi_pon_limits_get(ctx->priv, &state->pon_range_limits);
	/* This is number of lines in ptop page */
	return state->pon_range_limits.alloc_idx_max + 1;
}

char *table_entry_get_alloc_counters(struct top_context *ctx,
			     const int entry,
			     char *text)
{
	enum fapi_pon_errorcode ret;
	struct pon_allocation_id pai = {0};
	struct pon_alloc_counters pac = {0};
	static const char *status[] = {"0 UNUSED",
				       "1 REQUESTED",
				       "2 ASSIGNED",
				       "3 LINKED",
				       "4 BLOCKED"};

	if (entry <= -1) {
		sprintf(text, "%-20s %-20s %-20s %-20s %-20s %-20s",
			"Alloc index", "Alloc id",
			"Allocations", "Alloc idle frames",
			"Upstream Bandwidth", "Status");
		return NULL;
	}

	ret = fapi_pon_alloc_index_get(ctx->priv, entry, &pai);

	if (ret != PON_STATUS_OK)
		return NULL;

	fapi_pon_alloc_counters_get(ctx->priv, entry, &pac);

	/* do not show unused allocations */
	if (pai.status == 0)
		text[0] = 0;
	else
		sprintf(text, "%-20u %-20u %-20" PRIu64 " %-20" PRIu64 " %-20" PRIu64 " %-20s",
			entry, pai.alloc_id, pac.allocations, pac.idle,
			pac.us_bw,
			(pai.status < ARRAY_SIZE(status)) ?
			status[pai.status] : "unknown");

	return NULL;
}

int table_get_ploam_ds_counters(struct top_context *ctx,
		     const char *input_file_name)
{
	UNUSED(input_file_name);

	fapi_pon_cap_get(ctx->priv, &state->pon_cap);
	fapi_pon_ploam_ds_counters_get(ctx->priv,
				       &state->pon_ploam_ds_counters);
	/* This is number of lines in ptop page */
	return 23;
}

static char *table_entry_get_ploam_ds_counters_gpon(struct top_context *ctx,
						    const int entry,
						    char *text)
{
	UNUSED(ctx);

	switch (entry) {
	case -1:
		sprintf(text, "%-50s  %s", "OPTION", "VALUE");
		break;
	case 0:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Upstream overhead",
			state->pon_ploam_ds_counters.us_overhead);
		break;
	case 1:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Assign ONU ID",
			state->pon_ploam_ds_counters.assign_onu_id);
		break;
	case 2:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Ranging time",
			state->pon_ploam_ds_counters.ranging_time);
		break;
	case 3:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Deactivate ONU ID",
			state->pon_ploam_ds_counters.deact_onu);
		break;
	case 4:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Disable serial number",
			state->pon_ploam_ds_counters.disable_ser_no);
		break;
	case 5:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Encrypted Port ID",
			state->pon_ploam_ds_counters.enc_port_id);
		break;
	case 6:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Request password",
			state->pon_ploam_ds_counters.req_passwd);
		break;
	case 7:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Assign allocation ID",
			state->pon_ploam_ds_counters.assign_alloc_id);
		break;
	case 8:
		sprintf(text, "%-50s : %"PRId64" messages",
			"No message",
			state->pon_ploam_ds_counters.no_message);
		break;
	case 9:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Popup",
			state->pon_ploam_ds_counters.popup);
		break;
	case 10:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Request key",
			state->pon_ploam_ds_counters.req_key);
		break;
	case 11:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Configure Port ID",
			state->pon_ploam_ds_counters.config_port_id);
		break;
	case 12:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Physical Equipment error",
			state->pon_ploam_ds_counters.pee);
		break;
	case 13:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Change power level",
			state->pon_ploam_ds_counters.cpl);
		break;
	case 14:
		sprintf(text, "%-50s : %"PRId64" messages",
			"PST message",
			state->pon_ploam_ds_counters.pst);
		break;
	case 15:
		sprintf(text, "%-50s : %"PRId64" messages",
			"BER interval",
			state->pon_ploam_ds_counters.ber_interval);
		break;
	case 16:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Key switching time",
			state->pon_ploam_ds_counters.key_switching);
		break;
	case 17:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Extended burst length",
			state->pon_ploam_ds_counters.ext_burst);
		break;
	case 18:
		sprintf(text, "%-50s : %"PRId64" messages",
			"PON ID",
			state->pon_ploam_ds_counters.pon_id);
		break;
	case 19:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Swift popup",
			state->pon_ploam_ds_counters.swift_popup);
		break;
	case 20:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Ranging adjust",
			state->pon_ploam_ds_counters.ranging_adj);
		break;
	case 21:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Sleep allow",
			state->pon_ploam_ds_counters.sleep_allow);
		break;
	case 22:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Unknown message",
			state->pon_ploam_ds_counters.unknown);
		break;

	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

static char *table_entry_get_ploam_ds_counters_xpon(struct top_context *ctx,
						    const int entry,
						    char *text)
{
	int NG_PON_2 = state->pon_cap.features & PON_FEATURE_G987;
	int XGS_PON = state->pon_cap.features & PON_FEATURE_G9807;
	int PTP_WDM = 0; /* to be replace with proper pon type active */

	UNUSED(ctx);

	switch (entry) {
	case -1:
		sprintf(text, "%-50s  %s", "OPTION", "VALUE");
		break;
	case 0:
		if (NG_PON_2 || XGS_PON) {
			sprintf(text, "%-50s : %"PRId64" messages",
				"Burst profile",
				state->pon_ploam_ds_counters.burst_profile);
		}
		break;
	case 1:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Assign ONU ID",
			state->pon_ploam_ds_counters.assign_onu_id);
		break;
	case 2:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Ranging time",
			state->pon_ploam_ds_counters.ranging_time);
		break;
	case 3:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Deactivate ONU ID",
			state->pon_ploam_ds_counters.deact_onu);
		break;
	case 4:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Disable serial number",
			state->pon_ploam_ds_counters.disable_ser_no);
		break;
	case 5:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Request Registration",
			state->pon_ploam_ds_counters.req_reg);
		break;
	case 6:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Assign allocation ID",
			state->pon_ploam_ds_counters.assign_alloc_id);
		break;
	case 7:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Key control",
			state->pon_ploam_ds_counters.key_control);
		break;
	case 8:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Sleep allow",
			state->pon_ploam_ds_counters.sleep_allow);
		break;
	case 9:
		if (NG_PON_2) {
			sprintf(text, "%-50s : %"PRId64" messages",
				"Calibration request",
				state->pon_ploam_ds_counters.cal_req);
		} else if (PTP_WDM) {
			sprintf(text, "%-50s : %"PRId64" messages",
				"Rate control",
				state->pon_ploam_ds_counters.rate);
		} else {
			sprintf(text, "%-50s : %"PRId64" messages",
				"Unknown message",
				state->pon_ploam_ds_counters.unknown);
		}
		break;
	case 10:
		if (NG_PON_2) {
			sprintf(text, "%-50s : %"PRId64" messages",
				"Adjust TX wavelength",
				state->pon_ploam_ds_counters.tx_wavelength);
		} else if (PTP_WDM) {
			sprintf(text, "%-50s : %"PRId64" messages",
				"Unknown message",
				state->pon_ploam_ds_counters.unknown);
		}
		break;
	case 11:
		if (NG_PON_2) {
			sprintf(text, "%-50s : %"PRId64" messages",
				"Tuning control with \"request\"",
				state->pon_ploam_ds_counters.tuning_request);
		}
		break;
	case 12:
		if (NG_PON_2) {
			sprintf(text, "%-50s : %"PRId64" messages",
				"Tuning control with \"complete\"",
				state->pon_ploam_ds_counters.tuning_complete);
		}
		break;
	case 13:
		if (NG_PON_2) {
			sprintf(text, "%-50s : %"PRId64" messages",
				"System Profile",
				state->pon_ploam_ds_counters.system_profile);
		}
		break;
	case 14:
		if (NG_PON_2) {
			sprintf(text, "%-50s : %"PRId64" messages",
				"Channel profile",
				state->pon_ploam_ds_counters.channel_profile);
		}
		break;
	case 15:
		if (NG_PON_2) {
			sprintf(text, "%-50s : %"PRId64" messages",
				"Protection profile",
				state->pon_ploam_ds_counters.protection);
		}
		break;
	case 16:
		if (NG_PON_2) {
			sprintf(text, "%-50s : %"PRId64" messages",
				"Change power level",
				state->pon_ploam_ds_counters.cpl);
		}
		break;
	case 17:
		if (NG_PON_2) {
			sprintf(text, "%-50s : %"PRId64" messages",
				"Power consumption inquire",
				state->pon_ploam_ds_counters.power);
		}
		break;
	case 18:
		if (NG_PON_2) {
			sprintf(text, "%-50s : %"PRId64" messages",
				"Unknown message",
				state->pon_ploam_ds_counters.unknown);
		}
		break;

	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

char *table_entry_get_ploam_ds_counters(struct top_context *ctx,
			     const int entry,
			     char *text)
{
	/* GPON mode */
	if (state->pon_cap.features & PON_FEATURE_G984)
		return table_entry_get_ploam_ds_counters_gpon(ctx, entry, text);
	/* XG-PON/XGS-PON/NG-PON2 mode */
	else
		return table_entry_get_ploam_ds_counters_xpon(ctx, entry, text);
}

int table_get_ploam_us_counters(struct top_context *ctx,
		     const char *input_file_name)
{
	UNUSED(input_file_name);

	fapi_pon_cap_get(ctx->priv, &state->pon_cap);
	fapi_pon_ploam_us_counters_get(ctx->priv,
				       &state->pon_ploam_us_counters);

	/* This is number of lines in ptop page */
	return 10;
}

static char *table_entry_get_ploam_us_counters_gpon(struct top_context *ctx,
						    const int entry,
						    char *text)
{
	UNUSED(ctx);

	switch (entry) {
	case -1:
		sprintf(text, "%-50s  %s", "OPTION", "VALUE");
		break;
	case 0:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Serial number",
			state->pon_ploam_us_counters.ser_no);
		break;
	case 1:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Password",
			state->pon_ploam_us_counters.passwd);
		break;
	case 2:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Dying gasp",
			state->pon_ploam_us_counters.dying_gasp);
		break;
	case 3:
		sprintf(text, "%-50s : %"PRId64" messages",
			"No message",
			state->pon_ploam_us_counters.no_message);
		break;
	case 4:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Encryption key",
			state->pon_ploam_us_counters.enc_key);
		break;
	case 5:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Physical equipment error",
			state->pon_ploam_us_counters.pee);
		break;
	case 6:
		sprintf(text, "%-50s : %"PRId64" messages",
			"PON section trace",
			state->pon_ploam_us_counters.pst);
		break;
	case 7:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Remote error indication",
			state->pon_ploam_us_counters.rei);
		break;
	case 8:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Acknowledge",
			state->pon_ploam_us_counters.ack);
		break;
	case 9:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Sleep Request",
			state->pon_ploam_us_counters.sleep_req);
		break;

	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

static char *table_entry_get_ploam_us_counters_xpon(struct top_context *ctx,
						    const int entry,
						    char *text)
{
	int NG_PON_2 = state->pon_cap.features & PON_FEATURE_G987;
	int PTP_WDM = 0; /* to be replace with proper pon type active */

	UNUSED(ctx);

	switch (entry) {
	case -1:
		sprintf(text, "%-50s  %s", "OPTION", "VALUE");
		break;
	case 0:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Serial number",
			state->pon_ploam_us_counters.ser_no);
		break;
	case 1:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Registration",
			state->pon_ploam_us_counters.reg);
		break;
	case 2:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Key report",
			state->pon_ploam_us_counters.key_rep);
		break;
	case 3:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Acknowledge",
			state->pon_ploam_us_counters.ack);
		break;
	case 4:
		sprintf(text, "%-50s : %"PRId64" messages",
			"Sleep request",
			state->pon_ploam_us_counters.sleep_req);
		break;
	case 5:
		if (NG_PON_2) {
			sprintf(text, "%-50s : %"PRId64" messages",
				"Tuning response",
				state->pon_ploam_us_counters.key_rep);
		} else if (PTP_WDM) {
			sprintf(text, "%-50s : %"PRId64" messages",
				"Rate response",
				state->pon_ploam_us_counters.rate_resp);
		}
		break;
	case 6:
		if (NG_PON_2) {
			sprintf(text, "%-50s : %"PRId64" messages",
			"Power consumption report",
			state->pon_ploam_us_counters.power_rep);
		}
		break;

	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

char *table_entry_get_ploam_us_counters(struct top_context *ctx,
			     const int entry,
			     char *text)
{
	/* GPON mode */
	if (state->pon_cap.features & PON_FEATURE_G984)
		return table_entry_get_ploam_us_counters_gpon(ctx, entry, text);
	/* XG-PON/XGS-PON/NG-PON2 mode */
	else
		return table_entry_get_ploam_us_counters_xpon(ctx, entry, text);
}

int table_get_optic_status(struct top_context *ctx,
		     const char *input_file_name)
{

	enum fapi_pon_errorcode ret;

	UNUSED(input_file_name);

	if (!cached_optic_info) {
		ret = fapi_pon_optic_properties_get(ctx->priv,
				&state->pon_optic_properties);
		if (ret != PON_STATUS_OK) {
			cached_optic_info = false;
			return 0;
		}

		cached_optic_info = true;
	}

	optic_status_available = false;

	if (!state->pon_optic_properties.digital_monitoring)
		return 0;

	ret = fapi_pon_optic_status_get(ctx->priv, &state->pon_optic_status,
					pontop_tx_power_scale);
	if (ret != PON_STATUS_OK)
		return 0;

	ret = fapi_pon_optic_cfg_get(ctx->priv, &state->pon_optic_cfg);
	if (ret != PON_STATUS_OK)
		return 0;

	optic_status_available = true;

	/* This is number of lines in ptop page */
	return 8;
}

static const char *get_los_status(uint32_t rx_los,
				  uint32_t sd_polarity)
{
	/* The sd_polarity indicates if rx_los or !rx_los is LOS. */
	if (rx_los ^ sd_polarity)
		return "LOS";

	return "OK";
}

char *table_entry_get_optic_status(struct top_context *ctx,
			     const int entry,
			     char *text)
{
	UNUSED(ctx);
	double power;

	switch (entry) {
	case -1:
		if (!optic_status_available)
			sprintf(text, "Not available!");
		else
			sprintf(text, "%-50s  %s", "OPTION", "VALUE");
		break;
	case 0:
		sprintf(text, "%-50s : %d deg C / %d K",
			"Optical transceiver temperature",
			state->pon_optic_status.temperature / 256,
			state->pon_optic_status.temperature / 256 + 273);
		break;
	case 1:
		sprintf(text, "%-50s : %0.2f V",
			"Transceiver supply voltage",
			state->pon_optic_status.voltage / 10000.0);
		break;
	case 2:
		sprintf(text, "%-50s : %0.2f mA",
			"Transmit bias current",
			state->pon_optic_status.bias / 500.0);
		break;
	case 3:
		power = state->pon_optic_status.tx_power / 500.0;
		if (power <= -40) {
			sprintf(text, "%-50s : %s",
				"Transmit power", "No signal");
		} else {
			sprintf(text, "%-50s : %0.2f dBm",
				"Transmit power", power);
		}
		break;
	case 4:
		power = state->pon_optic_status.rx_power / 500.0;
		if (power <= -40) {
			sprintf(text, "%-50s : %s",
				"Receive power", "No signal");
		} else {
			sprintf(text, "%-50s : %0.2f dBm",
				"Receive power", power);
		}
		break;
	case 5:
		sprintf(text, " ");
		break;
	case 6:
		sprintf(text, "%-50s : %s",
			"Receiver status",
			get_los_status(state->pon_optic_status.rx_los,
				       state->pon_optic_cfg.sd_polarity));
		break;
	case 7:
		sprintf(text, "%-50s : %s %s",
			"Transmitter status",
			state->pon_optic_status.tx_disable ?
			"DISABLED" : "ENABLED",
			state->pon_optic_status.tx_fault ?
			"FAULT" : "");
		break;

	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

int table_get_optic_info(struct top_context *ctx,
			 const char *input_file_name)
{
	enum fapi_pon_errorcode ret;

	UNUSED(input_file_name);

	if (!cached_optic_info) {
		ret = fapi_pon_optic_properties_get(ctx->priv,
				&state->pon_optic_properties);
		if (ret != PON_STATUS_OK) {
			cached_optic_info = false;
			return 0;
		}

		cached_optic_info = true;
	}

	ret = fapi_pon_optic_cfg_get(ctx->priv, &state->pon_optic_cfg);
	if (ret != PON_STATUS_OK)
		return 0;

	/* This is number of lines in ptop page */
	return 42;
}

static const char *get_power_level_declaration(uint32_t high_power_lvl,
					       uint32_t power_lvl)
{
	if (high_power_lvl)
		return "Power Level 3";

	if (power_lvl)
		return "Power Level 2";

	return "Power Level 1";
}

static const char *get_sff_compliance(uint32_t compliance)
{
	switch (compliance) {
	case 0:
		return "Undefined";
	case 1:
		return "SFF-8472 Rev 9.3";
	case 2:
		return "SFF-8472 Rev 9.5";
	case 3:
		return "SFF-8472 Rev 10.2";
	case 4:
		return "SFF-8472 Rev 10.4";
	case 5:
		return "SFF-8472 Rev 11.0";
	case 6:
		return "SFF-8472 Rev 11.3";
	case 7:
		return "SFF-8472 Rev 11.4";
	case 8:
		return "SFF-8472 Rev 12.0";
	default:
		return "Reserved";
	}
}

char *table_entry_get_optic_info(struct top_context *ctx,
				 const int entry,
				 char *text)
{
	char buff[32];

	UNUSED(ctx);

	switch (entry) {
	case -1:
		if (!cached_optic_info)
			sprintf(text, "Not available!");
		else
			sprintf(text, "%-50s  %s", "SFP+ information",
				"Status");
		break;
	case 0:
		memset(buff, 0, sizeof(buff));
		memcpy(buff, state->pon_optic_properties.vendor_name,
		       sizeof(state->pon_optic_properties.vendor_name));
		sprintf(text, "%-50s : %s",
			"Vendor name",
			buff);
		break;
	case 1:
		memset(buff, 0, sizeof(buff));
		memcpy(buff, state->pon_optic_properties.vendor_oui,
		       sizeof(state->pon_optic_properties.vendor_oui));
		sprintf(text, "%-50s : %s",
			"Vendor oui",
			buff);
		break;
	case 2:
		memset(buff, 0, sizeof(buff));
		memcpy(buff, state->pon_optic_properties.part_number,
		       sizeof(state->pon_optic_properties.part_number));
		sprintf(text, "%-50s : %s",
			"Part number",
			buff);
		break;
	case 3:
		memset(buff, 0, sizeof(buff));
		memcpy(buff, state->pon_optic_properties.revision,
		       sizeof(state->pon_optic_properties.revision));
		sprintf(text, "%-50s : %s",
			"Revision",
			buff);
		break;
	case 4:
		memset(buff, 0, sizeof(buff));
		memcpy(buff, state->pon_optic_properties.serial_number,
		       sizeof(state->pon_optic_properties.serial_number));
		sprintf(text, "%-50s : %s",
			"Serial number",
			buff);
		break;
	case 5:
		memset(buff, 0, sizeof(buff));
		memcpy(buff, state->pon_optic_properties.date_code,
		       sizeof(state->pon_optic_properties.date_code));
		sprintf(text, "%-50s : %s",
			"Date code",
			buff);
		break;
	case 6:
		sprintf(text, "%-50s : %u nm",
			"Wavelength",
			state->pon_optic_properties.tx_wavelength);
		break;
	case 7:
		sprintf(text, " ");
		break;
	case 8:
		sprintf(text, "%s", "Options");
		break;
	case 9:
		sprintf(text, "%s", "------------");
		break;
	case 10:
		sprintf(text, "%-50s : %s",
			"Power level declaration",
			get_power_level_declaration(
			    state->pon_optic_properties.high_power_lvl_decl,
			    state->pon_optic_properties.power_lvl_decl));
		break;
	case 11:
		sprintf(text, "%-50s : %s",
			"Paging implemented indicator",
			state->pon_optic_properties.paging_implemented_ind ?
			"Yes" : "No");
		break;
	case 12:
		sprintf(text, "%-50s : %s",
			"Retimer / CDR indicator",
			state->pon_optic_properties.retimer_ind ?
			"Yes" : "No");
		break;
	case 13:
		sprintf(text, "%-50s : %s",
			"Cooled Transceiver declaration",
			state->pon_optic_properties.cooled_transceiver_decl ?
			"Yes" : "No");
		break;
	case 14:
		sprintf(text, "%-50s : %s",
			"Linear Receiver Output implemented",
			state->pon_optic_properties.linear_rx_output_impl ?
			"Yes" : "No");
		break;
	case 15:
		sprintf(text, "%-50s : %s",
			"Receiver decision threshold implemented",
			state->pon_optic_properties.rx_decision_thr_impl ?
			"Yes" : "No");
		break;
	case 16:
		sprintf(text, "%-50s : %s",
			"Tunable transmitter technology",
			state->pon_optic_properties.tunable_transmitter ?
			"Yes" : "No");
		break;
	case 17:
		sprintf(text, "%-50s : %s",
			"RATE_SELECT functionality implemented",
			state->pon_optic_properties.rate_select ?
			"Yes" : "No");
		break;
	case 18:
		sprintf(text, "%-50s : %s",
			"TX_DISABLE implemented",
			state->pon_optic_properties.tx_disable ?
			"Yes" : "No");
		break;
	case 19:
		sprintf(text, "%-50s : %s",
			"TX_FAULT implemented",
			state->pon_optic_properties.tx_fault ?
			"Yes" : "No");
		break;
	case 20:
		sprintf(text, "%-50s : %s",
			"Inverted loss of signal implemented",
			state->pon_optic_properties.signal_detect ?
			"Yes" : "No");
		break;
	case 21:
		sprintf(text, "%-50s : %s",
			"Loss of signal implemented",
			state->pon_optic_properties.rx_los ?
			"Yes" : "No");
		break;
	case 22:
		sprintf(text, "%-50s : %s",
			"SD Polarity",
			state->pon_optic_cfg.sd_polarity ?
			"Inverted" : "Normal");
		break;
	case 23:
		sprintf(text, " ");
		break;
	case 24:
		sprintf(text, "%s", "DMI type");
		break;
	case 25:
		sprintf(text, "%s", "-------------");
		break;
	case 26:
		sprintf(text, "%-50s : %s",
			"Digital monitoring implemented",
			state->pon_optic_properties.digital_monitoring ?
			"Yes" : "No");
		break;
	case 27:
		sprintf(text, "%-50s : %s",
			"Calibration",
			state->pon_optic_properties.ext_calibrated ?
			"External" : "Internal");
		break;
	case 28:
		sprintf(text, "%-50s : %s",
			"Received power measurement type",
			state->pon_optic_properties.rx_power_measurement_type ?
			"Average" : "OMA");
		break;
	case 29:
		sprintf(text, "%-50s : %s",
			"Address change required",
			state->pon_optic_properties.address_change_req ?
			"Yes" : "No");
		break;
	case 30:
		sprintf(text, " ");
		break;
	case 31:
		sprintf(text, "%s", "Enhanced options");
		break;
	case 32:
		sprintf(text, "%s", "---------------------");
		break;
	case 33:
		sprintf(text, "%-50s : %s",
			"Optional Alarm/Warnings flags implemented",
			state->pon_optic_properties.optional_flags_impl ?
			"Yes" : "No");
		break;
	case 34:
		sprintf(text, "%-50s : %s",
			"Soft TX_DISABLE control and monitoring implemented",
			state->pon_optic_properties.soft_tx_disable_monitor ?
			"Yes" : "No");
		break;
	case 35:
		sprintf(text, "%-50s : %s",
			"Soft TX_FAULT monitoring implemented",
			state->pon_optic_properties.soft_tx_fault_monitor ?
			"Yes" : "No");
		break;
	case 36:
		sprintf(text, "%-50s : %s",
			"Soft RX_LOS monitoring implemented",
			state->pon_optic_properties.soft_rx_los_monitor ?
			"Yes" : "No");
		break;
	case 37:
		sprintf(text, "%-50s : %s",
			"Soft RATE_SELECT ctrl and monitoring implemented",
			state->pon_optic_properties.soft_rate_select_monitor ?
			"Yes" : "No");
		break;
	case 38:
		sprintf(text, "%-50s : %s",
			"Application select control implemented",
			state->pon_optic_properties.app_select_impl ?
			"Yes" : "No");
		break;
	case 39:
		sprintf(text, "%-50s : %s",
			"Soft RATE_SELECT control implemented",
			state->pon_optic_properties.soft_rate_select_ctrl_impl ?
			"Yes" : "No");
		break;
	case 40:
		sprintf(text, " ");
		break;
	case 41:
		sprintf(text, "%-50s : %s",
			"Compliance",
			get_sff_compliance(
				state->pon_optic_properties.compliance));
		break;

	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

enum alarm_types {
	ALARM_TYPE_LEVEL,
	ALARM_TYPE_EDGE
};

struct alarm_top_entry {
	enum alarm_types type;
	const uint32_t bit;
	const char *name;
	const char *description;
};

static int
collect_act_alarm(void *ctx, const struct alarm_type *alarm, void *data)
{
	struct active_alarms *a_alarms = data;
	struct pon_alarm_status param = { 0 };

	if (fapi_pon_alarm_status_get(ctx, alarm->code, &param) !=
	    PON_STATUS_OK) {
		/* tell external loop controller to break the loop
		 * because of the error
		 */
		return -1;
	}

	if (param.alarm_status) {
		/* use pointer to level or edge table entry */
		a_alarms->type[a_alarms->size] = alarm;
		a_alarms->size++;
		if (a_alarms->size == MAX_ACT_ALARMS) {
			/* active alarms table is full
			 * tell external loop controller to break the loop
			 */
			return 1;
		}
	}

	/* allow subsequent iteration of external loop */
	return 0;
}

int table_get_alarms(struct top_context *top_ctx,
		     const char *input_file_name)
{
	int entries = 0;

	UNUSED(input_file_name);

	/* clear the temporary collection of active alarms */
	state->active_level_alarms.size = 0;
	state->active_edge_alarms.size = 0;

	/* collect active level alarms */
	fapi_pon_visit_alarms_level(top_ctx->priv, collect_act_alarm,
				    &state->active_level_alarms);

	/* collect active edge alarms */
	fapi_pon_visit_alarms_edge(top_ctx->priv, collect_act_alarm,
				   &state->active_edge_alarms);
	/* count active edge and level alarms */
	entries = state->active_level_alarms.size +
		  state->active_edge_alarms.size;

	state->alarm_entry = 0;
	return entries;
}

static void print_alarm(char *out, const struct alarm_type *alarm, int type)
{
	sprintf(out, "%-15s  %-25s %-30s",
		type == ALARM_TYPE_LEVEL ? "LEVEL" : "EDGE",
		alarm->name, alarm->desc);
}

char *table_entry_get_alarms(struct top_context *ctx, int entry, char *text)
{
	int i;
	int type;
	UNUSED(ctx);

	if (entry <= -1) {
		sprintf(text, "%-15s  %-25s %-30s", "Alarm type", "Alarm",
			      "Description");
		return NULL;
	}

	if (entry < state->active_level_alarms.size) {
		i = entry;
		type = ALARM_TYPE_LEVEL;
		print_alarm(text, state->active_level_alarms.type[i], type);
	} else if (entry < (state->active_level_alarms.size +
			    state->active_edge_alarms.size)) {
		i = entry - state->active_level_alarms.size;
		type = ALARM_TYPE_EDGE;
		print_alarm(text, state->active_edge_alarms.type[i], type);
	}

	return NULL;
}

#define BURST_PROFILE_ENTRY_FIELDS (8 + 3)
#define BURST_PROFILE_INDEXES 4
#define BURST_PROFILE_US_RATES 2

/* Report profiles only for current wavelength channel */
#define DWLCH_ID 0

int table_get_debug_burst_profile(struct top_context *ctx,
				  const char *input_file_name)
{
	uint32_t index;
	uint32_t us_rate;

	UNUSED(input_file_name);

	for (us_rate = 0; us_rate < BURST_PROFILE_US_RATES; ++us_rate) {
		for (index = 0; index < BURST_PROFILE_INDEXES; ++index) {
			int i = index * BURST_PROFILE_US_RATES + us_rate;

			fapi_pon_debug_burst_profile_get(ctx->priv,
				index, us_rate, DWLCH_ID,
				&state->pon_debug_burst_profiles[i]);
		}
	}

	/* This is number of lines in ptop page */
	return ARRAY_SIZE(state->pon_debug_burst_profiles) *
		BURST_PROFILE_ENTRY_FIELDS;
}

char *table_entry_get_debug_burst_profile(struct top_context *ctx,
					  int entry,
					  char *text)
{
	struct pon_debug_burst_profile *profile = NULL;
	int local_entry = 0;
	int index = 0;
	int us_rate = 0;
	int i = 0;

	UNUSED(ctx);

	if (entry <= -1) {
		sprintf(text, "%-15s", "Burst Profile Index (Upstream Rate)");
		return NULL;
	}

	if (entry >= (int)ARRAY_SIZE(state->pon_debug_burst_profiles) *
	    BURST_PROFILE_ENTRY_FIELDS) {
		sprintf(text, " ");
		return NULL;
	}

	i = entry / BURST_PROFILE_ENTRY_FIELDS;
	index = i / BURST_PROFILE_US_RATES;
	us_rate = i % BURST_PROFILE_US_RATES;
	/* Local entry is entry for ith burst profile */
	local_entry = entry % BURST_PROFILE_ENTRY_FIELDS;

	profile = &state->pon_debug_burst_profiles[i];

	switch (local_entry) {
	case 0:
		sprintf(text, "Index %d (%s)",
			index,
			us_rate ? "High" : "Low");
		break;
	case 1:
		sprintf(text, "---------------------");
		break;
	case 2:
		sprintf(text, "%-50s : %u",
			"Version",
			profile->version);
		break;
	case 3:
		sprintf(text, "%-50s : %s",
			"Upstream FEC",
			profile->us_fec ? "EN" : "DIS");
		break;
	case 4:
		sprintf(text, "%-50s : %u",
			"Delimiter Length",
			profile->delimiter_length);
		break;
	case 5:
		sprintf(text,
			"%-50s : 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
			"Delimiter Pattern",
			profile->delimiter_pattern[0],
			profile->delimiter_pattern[1],
			profile->delimiter_pattern[2],
			profile->delimiter_pattern[3],
			profile->delimiter_pattern[4],
			profile->delimiter_pattern[5],
			profile->delimiter_pattern[6],
			profile->delimiter_pattern[7]);
		break;
	case 6:
		sprintf(text, "%-50s : %u",
			"Preamble Length",
			profile->preamble_length);
		break;
	case 7:
		sprintf(text, "%-50s : %u",
			"Preamble Repeat Count",
			profile->preamble_repeat_count);
		break;
	case 8:
		sprintf(text,
			"%-50s : 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
			"Preamble Pattern",
			profile->preamble_pattern[0],
			profile->preamble_pattern[1],
			profile->preamble_pattern[2],
			profile->preamble_pattern[3],
			profile->preamble_pattern[4],
			profile->preamble_pattern[5],
			profile->preamble_pattern[6],
			profile->preamble_pattern[7]);
		break;
	case 9:
		sprintf(text,
			"%-50s : 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
			"PON Tag",
			profile->pon_tag[0],
			profile->pon_tag[1],
			profile->pon_tag[2],
			profile->pon_tag[3],
			profile->pon_tag[4],
			profile->pon_tag[5],
			profile->pon_tag[6],
			profile->pon_tag[7]);
		break;
	case 10:
		sprintf(text, " ");
		break;
	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

#define TWDM_TABLE_HEADER "OPTION \\ CHANNEL"
#define TWDM_TABLE_HEADER_LINES 1
/* column limiting the parameter names on the left, may cut names */
#define TWDM_TABLE_INDENT 30
#define TWDM_TABLE_FORMAT_STR(indent) "%-" #indent "s |"
#define TWDM_TABLE_FORMAT(indent) TWDM_TABLE_FORMAT_STR(indent)
#define TWDM_TABLE_PRINT_HEADER(buffer, no_channels) do { \
		unsigned int channel; \
		buffer += sprintf(buffer, \
				  TWDM_TABLE_FORMAT(TWDM_TABLE_INDENT), \
				  TWDM_TABLE_HEADER); \
		for (channel = 0; channel < (no_channels); channel++) \
			buffer += sprintf(buffer, "\t%u", channel); \
	} while (0)

#define TWDM_TABLE_PRINT_ENTRY(buffer, no_channels, struct, param) do { \
		unsigned int channel; \
		char param_buf[TWDM_TABLE_INDENT]; \
		snprintf(param_buf, TWDM_TABLE_INDENT, "%s", #param); \
		buffer += sprintf(buffer, \
				  TWDM_TABLE_FORMAT(TWDM_TABLE_INDENT), \
				  param_buf); \
		for (channel = 0; channel < (no_channels); channel++) \
			buffer += sprintf(buffer, "\t%"PRIu64, \
					(uint64_t)state->struct[channel].param); \
	} while (0)

#define TWDM_TABLE_PRINT_ENTRY_ARR(buffer, no_channels, struct, param, idx) \
	do { \
		unsigned int channel; \
		char param_buf[TWDM_TABLE_INDENT]; \
		snprintf(param_buf, TWDM_TABLE_INDENT, "%s[%02d]", \
			 #param, idx); \
		buffer += sprintf(buffer, \
				  TWDM_TABLE_FORMAT(TWDM_TABLE_INDENT), \
				  param_buf); \
		for (channel = 0; channel < (no_channels); channel++) \
			buffer += sprintf(buffer, "\t%"PRIu64, \
					(uint64_t)state->struct[channel].param[idx]); \
	} while (0)

int table_get_twdm_tc_counters(struct top_context *ctx,
			       const char *input_file_name)
{
	unsigned int channel;

	UNUSED(input_file_name);

	for (channel = 0;
	     channel < ARRAY_SIZE(state->twdm_tuning_counters);
	     channel++) {
		fapi_pon_twdm_tuning_counters_get(ctx->priv,
			channel,
			&state->twdm_tuning_counters[channel]);
	}

	/* This is number of lines in ptop page */
	return TWDM_TABLE_HEADER_LINES +
		ARRAY_SIZE(state->twdm_tuning_counters[0].counters);
}

char *table_entry_get_twdm_tc_counters(struct top_context *ctx,
				       int entry,
				       char *text)
{
	unsigned int max_channels = ARRAY_SIZE(state->twdm_tuning_counters);
	unsigned int max_tcs = ARRAY_SIZE(state->twdm_tuning_counters[0].counters);

	UNUSED(ctx);

	if (entry <= -1) {
		sprintf(text, "%-15s", "TWDM Tuning Counters");
		return NULL;
	}

	if (entry >= (int)max_tcs + TWDM_TABLE_HEADER_LINES) {
		sprintf(text, " ");
		return NULL;
	}

	if (entry == 0) {
		TWDM_TABLE_PRINT_HEADER(text, max_channels);
	} else {
		TWDM_TABLE_PRINT_ENTRY_ARR(text, max_channels,
					   twdm_tuning_counters, counters,
					   (entry-(TWDM_TABLE_HEADER_LINES)));
	}

	return NULL;
}

int table_get_twdm_lods_counters(struct top_context *ctx,
				 const char *input_file_name)
{
	unsigned int channel;

	UNUSED(input_file_name);

	for (channel = 0;
	     channel < ARRAY_SIZE(state->twdm_xgtc_counters);
	     channel++) {
		fapi_pon_twdm_xgtc_counters_get(ctx->priv,
			channel,
			&state->twdm_xgtc_counters[channel]);
	}

	/* This is number of lines in ptop page */
	return 7 + TWDM_TABLE_HEADER_LINES;
}

char *table_entry_get_twdm_lods_counters(struct top_context *ctx,
					 int entry,
					 char *text)
{
	unsigned int max_channels = ARRAY_SIZE(state->twdm_xgtc_counters);

	UNUSED(ctx);

	if (entry <= -1) {
		sprintf(text, "%-15s", "TWDM LODS Counters");
		return NULL;
	}

	switch (entry) {
	case 0:
		TWDM_TABLE_PRINT_HEADER(text, max_channels);
		break;
	case 1:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels, twdm_xgtc_counters,
				       lods_events_all);
		break;
	case 2:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels, twdm_xgtc_counters,
				       lods_restored_oper);
		break;
	case 3:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels, twdm_xgtc_counters,
				       lods_restored_prot);
		break;
	case 4:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels, twdm_xgtc_counters,
				       lods_restored_disc);
		break;
	case 5:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels, twdm_xgtc_counters,
				       lods_reactivation);
		break;
	case 6:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels, twdm_xgtc_counters,
				       lods_reactivation_prot);
		break;
	case 7:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels, twdm_xgtc_counters,
				       lods_reactivation_disc);
		break;
	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

int table_get_twdm_ploam_ds_counters(struct top_context *ctx,
				     const char *input_file_name)
{
	unsigned int channel;

	UNUSED(input_file_name);

	for (channel = 0;
	     channel < ARRAY_SIZE(state->twdm_ploam_ds_counters);
	     channel++) {
		fapi_pon_twdm_ploam_ds_counters_get(ctx->priv,
			channel,
			&state->twdm_ploam_ds_counters[channel]);
	}

	/* This is number of lines in ptop page */
	return 25 + TWDM_TABLE_HEADER_LINES;
}

char *table_entry_get_twdm_ploam_ds_counters(struct top_context *ctx,
					     int entry,
					     char *text)
{
	unsigned int max_channels = ARRAY_SIZE(state->twdm_ploam_ds_counters);

	UNUSED(ctx);

	if (entry <= -1) {
		sprintf(text, "%-15s", "TWDM PLOAM DS Message Counter");
		return NULL;
	}

	switch (entry) {
	case 0:
		TWDM_TABLE_PRINT_HEADER(text, max_channels);
		break;
	case 1:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       ranging_time);
		break;
	case 2:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       deact_onu);
		break;
	case 3:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       disable_ser_no);
		break;
	case 4:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       assign_alloc_id);
		break;
	case 5:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       cpl);
		break;
	case 6:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       sleep_allow);
		break;
	case 7:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       req_reg);
		break;
	case 8:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       key_control);
		break;
	case 9:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       burst_profile);
		break;
	case 10:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       cal_req);
		break;
	case 11:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       tx_wavelength);
		break;
	case 12:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       tuning_request);
		break;
	case 13:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       tuning_complete);
		break;
	case 14:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       system_profile);
		break;
	case 15:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       channel_profile);
		break;
	case 16:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       protection);
		break;
	case 17:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       power);
		break;
	case 18:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       rate);
		break;
	case 19:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       reset);
		break;
	case 20:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       unknown);
		break;
	case 21:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       tx_wavelength_err);
		break;
	case 22:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       mic_err);
		break;
	case 23:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       tune_ctrl);
		break;
	case 25:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       all);
		break;
	case 24:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_ds_counters,
				       assign_onu_id);
		break;
	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

int table_get_twdm_ploam_us_counters(struct top_context *ctx,
				     const char *input_file_name)
{
	unsigned int channel;

	UNUSED(input_file_name);

	for (channel = 0;
	     channel < ARRAY_SIZE(state->twdm_ploam_us_counters);
	     channel++) {
		fapi_pon_twdm_ploam_us_counters_get(ctx->priv,
			channel,
			&state->twdm_ploam_us_counters[channel]);
	}

	/* This is number of lines in ptop page */
	return 10 + TWDM_TABLE_HEADER_LINES;
}

char *table_entry_get_twdm_ploam_us_counters(struct top_context *ctx,
					     int entry,
					     char *text)
{
	unsigned int max_channels = ARRAY_SIZE(state->twdm_ploam_us_counters);

	UNUSED(ctx);

	if (entry <= -1) {
		sprintf(text, "%-15s", "TWDM PLOAM US Message Counter");
		return NULL;
	}

	switch (entry) {
	case 0:
		TWDM_TABLE_PRINT_HEADER(text, max_channels);
		break;
	case 1:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_us_counters,
				       reg);
		break;
	case 2:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_us_counters,
				       key_rep);
		break;
	case 3:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_us_counters,
				       ack);
		break;
	case 4:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_us_counters,
				       sleep_req);
		break;
	case 5:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_us_counters,
				       tuning_resp_ack_nack);
		break;
	case 6:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_us_counters,
				       tuning_resp_complete_rollback);
		break;
	case 7:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_us_counters,
				       power_rep);
		break;
	case 8:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_us_counters,
				       cpl_err);
		break;
	case 9:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_us_counters,
				       ser_no);
		break;
	case 10:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_ploam_us_counters,
				       all);
		break;
	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

int table_get_twdm_pl_counters(struct top_context *ctx,
			       const char *input_file_name)
{
	unsigned int channel;

	UNUSED(input_file_name);

	for (channel = 0;
	     channel < ARRAY_SIZE(state->twdm_optic_pl_counters);
	     channel++) {
		fapi_pon_twdm_optic_pl_counters_get(ctx->priv,
			channel,
			&state->twdm_optic_pl_counters[channel]);
	}

	/* This is number of lines in ptop page */
	return 3 + TWDM_TABLE_HEADER_LINES;
}

char *table_entry_get_twdm_pl_counters(struct top_context *ctx,
				       int entry,
				       char *text)
{
	unsigned int max_channels = ARRAY_SIZE(state->twdm_optic_pl_counters);

	UNUSED(ctx);

	if (entry <= -1) {
		sprintf(text, "%-15s", "TWDM Optic Power Level Counter");
		return NULL;
	}

	switch (entry) {
	case 0:
		TWDM_TABLE_PRINT_HEADER(text, max_channels);
		break;
	case 1:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_optic_pl_counters,
				       incomplete);
		break;
	case 2:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_optic_pl_counters,
				       complete);
		break;
	case 3:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_optic_pl_counters,
				       rejected);
		break;
	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

int table_get_twdm_xgtc_counters(struct top_context *ctx,
				 const char *input_file_name)
{
	unsigned int channel;

	UNUSED(input_file_name);

	for (channel = 0;
	     channel < ARRAY_SIZE(state->twdm_xgtc_counters);
	     channel++) {
		fapi_pon_twdm_xgtc_counters_get(ctx->priv,
			channel,
			&state->twdm_xgtc_counters[channel]);
	}

	/* This is number of lines in ptop page */
	return 11 + TWDM_TABLE_HEADER_LINES;
}

char *table_entry_get_twdm_xgtc_counters(struct top_context *ctx,
					 int entry,
					 char *text)
{
	unsigned int max_channels = ARRAY_SIZE(state->twdm_xgtc_counters);

	UNUSED(ctx);

	if (entry <= -1) {
		sprintf(text, "%-15s", "TWDM XGTC Counters");
		return NULL;
	}

	switch (entry) {
	case 0:
		TWDM_TABLE_PRINT_HEADER(text, max_channels);
		break;
	case 1:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_xgtc_counters,
				       psbd_hec_err_uncorr);
		break;
	case 2:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_xgtc_counters,
				       psbd_hec_err_corr);
		break;
	case 3:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_xgtc_counters,
				       fs_hec_err_uncorr);
		break;
	case 4:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_xgtc_counters,
				       fs_hec_err_corr);
		break;
	case 5:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_xgtc_counters,
				       ploam_mic_err);
		break;
	case 6:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_xgtc_counters,
				       xgem_hec_err_corr);
		break;
	case 7:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_xgtc_counters,
				       xgem_hec_err_uncorr);
		break;
	case 8:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_xgtc_counters,
				       burst_profile_err);
		break;
	case 9:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_xgtc_counters,
				       words);
		break;
	case 10:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_xgtc_counters,
				       bip_errors);
		break;
	case 11:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_xgtc_counters,
				       lost_words);
		break;
	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

int table_get_twdm_channel_profile_status(struct top_context *ctx,
					  const char *input_file_name)
{
	unsigned int channel;

	UNUSED(input_file_name);

	for (channel = 0;
	     channel < ARRAY_SIZE(state->twdm_channel_profile);
	     channel++) {
		fapi_pon_twdm_channel_profile_status_get(ctx->priv,
			channel,
			&state->twdm_channel_profile[channel]);
	}

	/* This is number of lines in ptop page */
	return 18 + TWDM_TABLE_HEADER_LINES;
}

char *table_entry_get_twdm_channel_profile_status(struct top_context *ctx,
						  int entry,
						  char *text)
{
	unsigned int max_channels = ARRAY_SIZE(state->twdm_channel_profile);

	UNUSED(ctx);

	if (entry <= -1) {
		sprintf(text, "%-15s", "TWDM Channel Profile Status");
		return NULL;
	}

	switch (entry) {
	case 0:
		TWDM_TABLE_PRINT_HEADER(text, max_channels);
		break;
	case 1:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_channel_profile,
				       pon_id);
		break;
	case 2:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_channel_profile,
				       us_frq);
		break;
	case 3:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_channel_profile,
				       resp_thr);
		break;
	case 4:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_channel_profile,
				       ds_offset);
		break;
	case 5:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_channel_profile,
				       cloned);
		break;
	case 6:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_channel_profile,
				       version);
		break;
	case 7:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_channel_profile,
				       ds_valid);
		break;
	case 8:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_channel_profile,
				       ds_fec);
		break;
	case 9:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_channel_profile,
				       ds_rate);
		break;
	case 10:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_channel_profile,
				       us_valid);
		break;
	case 11:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_channel_profile,
				       us_type);
		break;
	case 12:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_channel_profile,
				       us_rate);
		break;
	case 13:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_channel_profile,
				       dswlch_id);
		break;
	case 14:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_channel_profile,
				       uswlch_id);
		break;
	case 15:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_channel_profile,
				       def_att);
		break;
	case 16:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_channel_profile,
				       cpi);
		break;
	case 17:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_channel_profile,
				       this_wl_channel);
		break;
	case 18:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_channel_profile,
				       def_resp_ch);
		break;
	default:
		text[0] = 0;
		break;
	}

	return NULL;
}

#define TWDM_XGEM_PORT_LINES (8+3)
int table_get_twdm_xgem_port_counters(struct top_context *ctx,
				      const char *input_file_name)
{
	unsigned int gemports_used;

	UNUSED(input_file_name);

	fapi_pon_limits_get(ctx->priv, &state->pon_range_limits);

	gemports_used = state->pon_range_limits.gem_port_idx_max + 1;

	/* This is number of lines in ptop page */
	return TWDM_XGEM_PORT_LINES * gemports_used;
}

char *table_entry_get_twdm_xgem_port_counters(struct top_context *ctx,
					      int entry,
					      char *text)
{
	unsigned int max_channels = ARRAY_SIZE(state->twdm_gem_port_counters);
	unsigned int channel, gempidx = entry / TWDM_XGEM_PORT_LINES;

	UNUSED(ctx);

	if (entry <= -1) {
		sprintf(text, "%-15s", "TWDM XGEM-Port Counter");
		return NULL;
	}

	/* get the GEM port ID from the index */
	if (fapi_pon_gem_port_index_get(ctx->priv, (uint32_t) gempidx,
				&state->pon_gem_port) != PON_STATUS_OK)
		return NULL;

	/* get all TWDM channel's data of this GEM port */
	for (channel = 0;
	     channel < ARRAY_SIZE(state->twdm_gem_port_counters);
	     channel++) {
		fapi_pon_twdm_xgem_port_counters_get(ctx->priv,
			channel,
			state->pon_gem_port.gem_port_id,
			&state->twdm_gem_port_counters[channel]);
	}

	switch (entry % TWDM_XGEM_PORT_LINES) {
	case 0:
		sprintf(text, "GEM Index %d GEM ID %d", gempidx,
			state->pon_gem_port.gem_port_id);
		break;
	case 1:
		TWDM_TABLE_PRINT_HEADER(text, max_channels);
		break;
	case 2:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_gem_port_counters,
				       gem_port_id);
		break;
	case 3:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_gem_port_counters,
				       tx_frames);
		break;
	case 4:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_gem_port_counters,
				       tx_fragments);
		break;
	case 5:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_gem_port_counters,
				       tx_bytes);
		break;
	case 6:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_gem_port_counters,
				       rx_frames);
		break;
	case 7:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_gem_port_counters,
				       rx_fragments);
		break;
	case 8:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_gem_port_counters,
				       rx_bytes);
		break;
	case 9:
		TWDM_TABLE_PRINT_ENTRY(text, max_channels,
				       twdm_gem_port_counters,
				       key_errors);
		break;
	case 10:
		sprintf(text, "-------------------------");
		break;
	default:
		text[0] = 0;
		break;
	}

	return NULL;
}
