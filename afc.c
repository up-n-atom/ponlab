/******************************************************************************

		 Copyright (c) 2023, MaxLinear, Inc.

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

*******************************************************************************/
#include <string.h>
#include <sys/time.h>
#include <cjson/cJSON.h>
#include "afc.h"
#include "eloop.h"
#include "json.h"

struct afc_config config;
struct afc_spectrum_inquiry_resp afc_response;

enum afc_status afc_spectrum_resp_expiry(void)
{
	int remaining_time;
	struct tm expire_tm;
	time_t expire_timestamp, current_time;

	memset(&expire_tm, 0, sizeof(expire_tm));
	if (!strptime(afc_response.expire_time, "%Y-%m-%dT%H:%M:%SZ", &expire_tm))
		return AFC_STATUS_FAILURE;

	expire_timestamp = mktime(&expire_tm);
	if (expire_timestamp < 0)
		return AFC_STATUS_FAILURE;

	current_time = time(NULL);

	afc_printf(MSG_INFO, "expiry time in resp: %s\n expire_timestamp: %ld\n current_time: %ld\n",
			   afc_response.expire_time, (long)expire_timestamp, (long)current_time);

	if (expire_timestamp > current_time) {
		remaining_time = (int)(expire_timestamp - current_time);
		afc_printf(MSG_INFO, "remaining time = %d hrs", remaining_time/ONE_HOUR_IN_SECONDS);
			if (eloop_is_timeout_registered(afc_query_server, NULL, NULL))
				eloop_cancel_timeout(afc_query_server, NULL, NULL);
			eloop_register_timeout(remaining_time, 0, afc_query_server, NULL, NULL);
	} else {
		afc_printf(MSG_ERROR, "expiration time has already passed.");
		return AFC_STATUS_FAILURE;
	}

	return AFC_STATUS_SUCCESS;
}

enum afc_status afc_construct_afc_reg_db(void)
{
	int num_freq;
	int num_eirp;
	int num_chan, num_chan_cfi;

	FILE *fp = fopen(AFCD_RESP_DUMP_FILE, "w");
	if (!fp) {
		afc_printf(MSG_ERROR, "error opening file for writing: %s", AFCD_RESP_DUMP_FILE);
		return AFC_STATUS_FAILURE;
	}

	fprintf(fp, "version: %s\n", afc_response.version);
	fprintf(fp, "request_id: %s\n", afc_response.request_id);
	fprintf(fp, "ruleset_ids: %s\n", afc_response.rule_set_ids);
	fprintf(fp, "expire_time: %s\n", afc_response.expire_time);
	fprintf(fp, "response_status: %d\n", afc_response.resp_info.resp_status);
	fprintf(fp, "short_description: %s\n", afc_response.resp_info.short_description);

	for (num_freq = 0; num_freq < afc_response.num_freq_info; num_freq++) {
		fprintf(fp, "frequency_range %d: %u - %u, max_psd: %.2f\n",
				num_freq + 1, afc_response.freq_info[num_freq].freq_range.low_frequency,
				afc_response.freq_info[num_freq].freq_range.high_frequency,
				afc_response.freq_info[num_freq].max_psd);
	}

	for (num_chan = 0; num_chan < afc_response.num_chan_info; num_chan++) {
		fprintf(fp, "channel_info %d: global_operating_class: %u\n",
				num_chan + 1, afc_response.chan_info[num_chan].global_op_class);

		if (afc_response.chan_info[num_chan].num_chan_cfi > 0) {
			fprintf(fp, "channel_cfi:");
			for (num_chan_cfi = 0;
				 num_chan_cfi < afc_response.chan_info[num_chan].num_chan_cfi;
				 num_chan_cfi++) {
				fprintf(fp, " %u", afc_response.chan_info[num_chan].channel_cfi[num_chan_cfi]);
			}
			fprintf(fp, "\n");
		}

		if (afc_response.chan_info[num_chan].max_eirp != NULL) {
			fprintf(fp, "max_eirp:");
			for (num_eirp = 0;
				 num_eirp < afc_response.chan_info[num_chan].num_chan_cfi;
				 num_eirp++) {
				fprintf(fp, " %.2f", afc_response.chan_info[num_chan].max_eirp[num_eirp]);
			}
			fprintf(fp, "\n");
		}
	}

	fclose(fp);
	memcpy(afc_response.country, config.country, 2);
	if (afc_construct_regrule_from_afc_response(&afc_response))
		return AFC_STATUS_FAILURE;

	return AFC_STATUS_SUCCESS;
}

enum afc_status afc_validate_spectrum_resp(void)
{
	if (strcmp(afc_response.resp_info.short_description, "SUCCESS") != 0) {
		afc_printf(MSG_ERROR, "AFC response failed : %d", afc_response.resp_info.resp_status);
		afc_construct_regrule_from_afc_response(&afc_response);
		return AFC_STATUS_FAILURE;
	}

	afc_printf(MSG_INFO, "AFC response status : %d\n short description : %s",
			   afc_response.resp_info.resp_status, afc_response.resp_info.short_description);

	return AFC_STATUS_SUCCESS;
}

enum afc_status afc_send_spectrum_request(void)
{
	char *json_data;

	cJSON *json = afc_spectrum_inquiry_req_params_to_json(&config.req_params);
	if (!json)
		return AFC_STATUS_FAILURE;

	json_data = cJSON_Print(json);
	if (!json_data)
		goto fail;

	afc_printf(MSG_INFO, "afc_server_url: %s\n JSON request message:\n  %s",
			   config.afc_server_url, json_data);

	if (afc_curl_message_to_server(json_data, &config)) {
		cJSON_free(json_data);
		goto fail;
	}

	cJSON_free(json_data);
	cJSON_Delete(json);
	return AFC_STATUS_SUCCESS;

fail:
	cJSON_Delete(json);
	return AFC_STATUS_FAILURE;
}

enum afc_status afc_query_server(void)
{
	memset(&afc_response, 0, sizeof(afc_response));

	if (afc_read_req_configs(&config)) {
		afc_printf(MSG_ERROR, "failed to read AFC config");
		goto fail;
	}

	if (afc_send_spectrum_request()) {
		afc_printf(MSG_ERROR, "failed to send AFC request");
		goto fail;
	}

	if (afc_validate_spectrum_resp()) {
		afc_printf(MSG_ERROR, "validation of AFC response failed");
		goto fail;
	}

	if (afc_construct_afc_reg_db()) {
		afc_printf(MSG_ERROR, "failed to construct regdb");
		goto fail;
	}

	if (afc_spectrum_resp_expiry()) {
		afc_printf(MSG_ERROR, "failed to schedule timeout based on the afc response");
		goto fail;
	}

	if (afc_response.freq_info)
		free(afc_response.freq_info);
	if (afc_response.chan_info)
		free(afc_response.chan_info);

	return AFC_STATUS_SUCCESS;

fail:
	if (afc_response.freq_info)
		free(afc_response.freq_info);
	if (afc_response.chan_info)
		free(afc_response.chan_info);

	if (eloop_is_timeout_registered(afc_query_server, NULL, NULL))
		eloop_cancel_timeout(afc_query_server, NULL, NULL);
	eloop_register_timeout(TIMEOUT_INTERVAL_IN_SEC, 0, afc_query_server, NULL, NULL);
	afc_printf(MSG_ERROR, "scheduled timeout of 24 hours");

	return AFC_STATUS_FAILURE;
}
