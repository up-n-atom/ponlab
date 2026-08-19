/******************************************************************************

		 Copyright (c) 2023-2024, MaxLinear, Inc.

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "utils.h"
#include "afc_debug.h"

int afc_read_req_configs(struct afc_config *config)
{
	int freq_range_idx = 0;
	int list_chan_idx = 0;
	int chan_cfi_idx = 0;
	int len, parsed_value;
	char *token, *value, *pos;
	char *freq_range_token, *cfi_token;
	char line[MAX_NUM_OF_ENTRIES];
	FILE *fp;

	fp = fopen(AFCD_CONFIG_FILE, "r");
	if (!fp) {
		afc_printf(MSG_ERROR, "Error opening %s file", AFCD_CONFIG_FILE);
		return AFC_STATUS_FAILURE;
	}

	memset(config, 0, sizeof(struct afc_config));

	while (fgets(line, sizeof(line), fp)) {
		token = strtok(line, "=");
		value = strtok(NULL, "\n");

		if (strcmp(token, "version") == 0) {
			/* TODO: validation of version param - TBD */
			strncpy(config->req_params.version, value, sizeof(config->req_params.version) - 1);
		} else if (strcmp(token, "request_id") == 0) {
			/* TODO: Unique request ID, currently we are testing with WFA AFC test server,
			Here by default the request ID is zero. */
			strncpy(config->req_params.request_id, value, sizeof(config->req_params.request_id) - 1);
		} else if (strcmp(token, "serial_number") == 0) {
			strncpy(config->req_params.device_descriptor.serial_number, value,
					sizeof(config->req_params.device_descriptor.serial_number) - 1);
		} else if (strcmp(token, "nra") == 0) {
			strncpy(config->req_params.device_descriptor.certification_id.nra, value,
					sizeof(config->req_params.device_descriptor.certification_id.nra) - 1);
		} else if (strcmp(token, "id") == 0) {
			strncpy(config->req_params.device_descriptor.certification_id.id, value,
					sizeof(config->req_params.device_descriptor.certification_id.id) - 1);
		} else if (strcmp(token, "ruleset_ids") == 0) {
			if (strcmp(value, "US_47_CFR_PART_15_SUBPART_E") != 0 &&
					strcmp(value, "CA_RES_DBS-06") != 0) {
				afc_printf(MSG_ERROR, "invalid ruleset_ids allowed configs are"
						   "US_47_CFR_PART_15_SUBPART_E or CA_RES_DBS-06");
				goto fail;
			}
			strncpy(config->req_params.device_descriptor.rule_set_ids, value,
					sizeof(config->req_params.device_descriptor.rule_set_ids) - 1);
		} else if (strcmp(token, "longitude") == 0) {
			config->req_params.location.ellipse.center.longitude = atof(value);
		} else if (strcmp(token, "latitude") == 0) {
			config->req_params.location.ellipse.center.latitude = atof(value);
		} else if (strcmp(token, "major_axis") == 0) {
			config->req_params.location.ellipse.major_axis = atoi(value);
		} else if (strcmp(token, "minor_axis") == 0) {
			config->req_params.location.ellipse.minor_axis = atoi(value);
		} else if (strcmp(token, "orientation") == 0) {
			config->req_params.location.ellipse.orientation = atoi(value);
		} else if (strcmp(token, "height") == 0) {
			config->req_params.location.elevation.height = atof(value);
		} else if (strcmp(token, "height_type") == 0) {
			if (strcmp(value, "AGL") != 0 && strcmp(value, "AMSL") != 0) {
				afc_printf(MSG_ERROR, "invalid height_type, allowed configs are AGL or AMSL");
				goto fail;
			}
			strncpy(config->req_params.location.elevation.height_type, value,
					sizeof(config->req_params.location.elevation.height_type) - 1);
		} else if (strcmp(token, "vertical_uncertainty") == 0) {
			config->req_params.location.elevation.vertical_uncertainty = atoi(value);
		} else if (strcmp(token, "indoor_deployment") == 0) {
			if (atoi(value) < 0 || atoi(value) > 2) {
				afc_printf(MSG_ERROR, "invalid indoor_deployment, allowed"
						   "configs are 0, 1, 2");
				goto fail;
			}
			config->req_params.location.indoor_deployment = atoi(value);
		} else if (strcmp(token, "freq_range") == 0) {
			len = 0;
			pos = value;
			while (*pos++) {
				if (*pos == '\'') {
					len++;
					break;
				}

				if (isspace(*pos))
					len++;
			}

			config->req_params.list_freq_range.num_range = len;
			config->req_params.list_freq_range.range =
					zalloc(config->req_params.list_freq_range.num_range * sizeof(struct afc_freq_range));
			if (!config->req_params.list_freq_range.range)
				goto fail;

			freq_range_token = strtok(value, " '");
			while (freq_range_token != NULL) {
				sscanf(freq_range_token, "%d-%d", &config->req_params.list_freq_range.range[freq_range_idx].lower_freq,
					&config->req_params.list_freq_range.range[freq_range_idx].higher_freq);
				freq_range_idx++;
				freq_range_token = strtok(NULL, " ");
			}
		} else if (strcmp(token, "global_op_class") == 0) {
			if (atoi(value) < GLOBAL_OP_CLASS_20_MHZ || atoi(value) > GLOBAL_OP_CLASS_320_MHZ) {
				afc_printf(MSG_ERROR, "invalid global_op_class configuration,"
						   "the allowed values start from 131 to 137");
				goto fail;
			}
			config->req_params.list_chan[list_chan_idx].global_op_class = atoi(value);
		} else if (strcmp(token, "channel_cfi") == 0) {
			len = 0;
			pos = value;
			while (*pos++) {
				if (*pos == '\'') {
					len++;
					break;
				}

				if (isspace(*pos))
					len++;
			}

			config->req_params.list_chan[list_chan_idx].num_chan_cfi = len;
			config->req_params.list_chan[list_chan_idx].channel_cfi =
					(uint8_t*)zalloc(config->req_params.list_chan[list_chan_idx].
					num_chan_cfi * sizeof(uint8_t));
			if (!(config->req_params.list_chan[list_chan_idx].channel_cfi)) {
				afc_printf(MSG_ERROR, "channel_cfi memory allocation failure");
				goto fail;
			}

			cfi_token = strtok(value, " '");
			while (cfi_token != NULL) {
				parsed_value = atoi(cfi_token);
				if (parsed_value) {
					config->req_params.list_chan[list_chan_idx]
							.channel_cfi[chan_cfi_idx++] = parsed_value;
				}
				cfi_token = strtok(NULL, " '");
				if (chan_cfi_idx == config->req_params.list_chan[list_chan_idx].num_chan_cfi) {
					list_chan_idx++;
					chan_cfi_idx = 0;
					break;
				}
			}
		} else if (strcmp(token, "cacert_path") == 0) {
			strncpy(config->cacert_path, value, sizeof(config->cacert_path) - 1);
		} else if (strcmp(token, "verify_cert") == 0) {
			if (atoi(value) < DISABLE_CERT_VERIFICATION || atoi(value) > ENABLE_CERT_VERIFICATION) {
				afc_printf(MSG_ERROR, "invalid verify_cert configuration,"
						   "the allowed values are 0 or 1");
				goto fail;
			}
			config->verify_cert = (uint8_t)atoi(value);
		} else if (strcmp(token, "afc_url") == 0) {
			strncpy(config->afc_server_url, value, sizeof(config->afc_server_url) - 1);
		} else if (strcmp(token, "country_code") == 0) {
			if (value[0] < 'A' || value[0] > 'Z' || value[1] < 'A' || value[1] > 'Z') {
				afc_printf(MSG_ERROR, "invalid country_code %s", value);
				goto fail;
			}
			memcpy(config->country, value, 2);
		}
	}

	fclose(fp);
	return AFC_STATUS_SUCCESS;

fail:
	if (config->req_params.list_freq_range.range)
		free(config->req_params.list_freq_range.range);

	while(!list_chan_idx) {
		if (config->req_params.list_chan[list_chan_idx].channel_cfi)
			free(config->req_params.list_chan[list_chan_idx].channel_cfi);
		list_chan_idx--;
	}

	fclose(fp);
	return AFC_STATUS_FAILURE;
}
