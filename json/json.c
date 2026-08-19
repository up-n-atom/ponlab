/******************************************************************************

		 Copyright (c) 2023, MaxLinear, Inc.

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

*******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "afc.h"
#include "json.h"
#include "utils.h"

extern struct afc_spectrum_inquiry_resp afc_response;

static cJSON *afc_create_inquired_channels(const struct afc_req_chan_list *channel)
{
	int num_chan;
	cJSON *channel_info, *channel_cfi_arr;

	channel_info = cJSON_CreateObject();
	if (!channel_info)
		return NULL;

	cJSON_AddNumberToObject(channel_info, "globalOperatingClass", channel->global_op_class);
	if (channel->num_chan_cfi > 0) {
		channel_cfi_arr = cJSON_CreateArray();
		if (!channel_cfi_arr) {
			cJSON_Delete(channel_info);
			return NULL;
		}

		for (num_chan = 0; num_chan < channel->num_chan_cfi; num_chan++) {
			cJSON_AddItemToArray(channel_cfi_arr, cJSON_CreateNumber(channel->channel_cfi[num_chan]));
		}
		cJSON_AddItemToObject(channel_info, "channelCfi", channel_cfi_arr);
	}
	return channel_info;
}

static cJSON *afc_create_inquired_freq_range(const struct afc_req_freq_list *list_freq_range)
{
	int num_freq;
	cJSON *inquired_freq_range, *freq_range;

	inquired_freq_range = cJSON_CreateArray();
	if (!inquired_freq_range)
		return NULL;

	for (num_freq = 0; num_freq < list_freq_range->num_range; num_freq++) {
		freq_range = cJSON_CreateObject();
		if (!freq_range) {
			cJSON_Delete(inquired_freq_range);
			return NULL;
		}

		cJSON_AddNumberToObject(freq_range, "lowFrequency",
								list_freq_range->range[num_freq].lower_freq);
		cJSON_AddNumberToObject(freq_range, "highFrequency",
								list_freq_range->range[num_freq].higher_freq);
		cJSON_AddItemToArray(inquired_freq_range, freq_range);
	}
	return inquired_freq_range;
}

cJSON *afc_spectrum_inquiry_req_params_to_json(struct afc_spectrum_inquiry_req_params *req_params)
{
	int num_global_op_cls;
	cJSON *root, *request_arr, *request, *device_descriptor;
	cJSON *certification_id_arr, *certification_id, *location;
	cJSON *ellipse, *center, *elevation;
	cJSON *inquired_freq_range, *inquired_channels;

	if (!req_params)
		return NULL;

	root = cJSON_CreateObject();
	request_arr = cJSON_CreateArray();
	request = cJSON_CreateObject();
	if (!root || !request_arr || !request) {
		cJSON_Delete(root);
		cJSON_Delete(request_arr);
		cJSON_Delete(request);
		return NULL;
	}

	cJSON_AddItemToObject(root, "version", cJSON_CreateString(req_params->version));
	cJSON_AddItemToObject(root, "availableSpectrumInquiryRequests", request_arr);
	cJSON_AddItemToArray(request_arr, request);
	cJSON_AddItemToObject(request, "requestId", cJSON_CreateString(req_params->request_id));
	cJSON_AddItemToObject(request, "deviceDescriptor", cJSON_CreateObject());
	cJSON_AddItemToObject(request, "location", cJSON_CreateObject());

	inquired_freq_range = afc_create_inquired_freq_range(&req_params->list_freq_range);
	if (!inquired_freq_range)
		goto fail;

	cJSON_AddItemToObject(request, "inquiredFrequencyRange", inquired_freq_range);
	cJSON_AddItemToObject(request, "inquiredChannels", cJSON_CreateArray());

	device_descriptor = cJSON_GetObjectItemCaseSensitive(request, "deviceDescriptor");
	if (!device_descriptor)
		goto fail;

	cJSON_AddItemToObject(device_descriptor,
			"serialNumber", cJSON_CreateString(req_params->device_descriptor.serial_number));
	cJSON_AddItemToObject(device_descriptor, "certificationId", cJSON_CreateArray());
	certification_id_arr = cJSON_GetObjectItemCaseSensitive(device_descriptor, "certificationId");
	if (!certification_id_arr)
		goto fail;

	/*TODO: In the array of certificationId there is no nra field in WFA AFC test server,
	once that issue is ressolved we need to add nra field in our request JSON. */
	certification_id = cJSON_CreateObject();
	if (!certification_id)
		goto fail;

	cJSON_AddItemToObject(certification_id,
			"rulesetId", cJSON_CreateString(req_params->device_descriptor.rule_set_ids));
	cJSON_AddItemToObject(certification_id,
			"nra", cJSON_CreateString(req_params->device_descriptor.certification_id.nra));
	cJSON_AddItemToObject(certification_id,
			"id", cJSON_CreateString(req_params->device_descriptor.certification_id.id));
	cJSON_AddItemToArray(certification_id_arr, certification_id);
	location = cJSON_GetObjectItemCaseSensitive(request, "location");
	if (!location)
		goto fail;

	cJSON_AddItemToObject(location, "ellipse", cJSON_CreateObject());
	cJSON_AddItemToObject(location, "elevation", cJSON_CreateObject());

	ellipse = cJSON_GetObjectItemCaseSensitive(location, "ellipse");
	if (!ellipse)
		goto fail;

	cJSON_AddItemToObject(ellipse, "center", cJSON_CreateObject());
	cJSON_AddNumberToObject(ellipse, "majorAxis", req_params->location.ellipse.major_axis);
	cJSON_AddNumberToObject(ellipse, "minorAxis", req_params->location.ellipse.minor_axis);
	cJSON_AddNumberToObject(ellipse, "orientation", req_params->location.ellipse.orientation);

	center = cJSON_GetObjectItemCaseSensitive(ellipse, "center");
	if (!center)
		goto fail;

	cJSON_AddNumberToObject(center, "longitude", req_params->location.ellipse.center.longitude);
	cJSON_AddNumberToObject(center, "latitude", req_params->location.ellipse.center.latitude);

	elevation = cJSON_GetObjectItemCaseSensitive(location, "elevation");
	if (!elevation)
		goto fail;

	cJSON_AddNumberToObject(elevation, "height", req_params->location.elevation.height);
	cJSON_AddItemToObject(elevation,
			"heightType", cJSON_CreateString(req_params->location.elevation.height_type));
	cJSON_AddNumberToObject(elevation,
			"verticalUncertainty", req_params->location.elevation.vertical_uncertainty);
	cJSON_AddNumberToObject(location, "indoorDeployment", req_params->location.indoor_deployment);
	for (num_global_op_cls = 0; num_global_op_cls < MAX_NUM_OF_6GHZ_GLOBAL_OP_CLASS; num_global_op_cls++) {
		if (req_params->list_chan[num_global_op_cls].global_op_class) {
			inquired_channels = afc_create_inquired_channels(&req_params->list_chan[num_global_op_cls]);
			if (!inquired_channels)
				goto fail;

			cJSON_AddItemToArray(cJSON_GetObjectItemCaseSensitive(request, "inquiredChannels"),
								 inquired_channels);
		}
	}
	return root;

fail:
	afc_printf(MSG_ERROR, "unable to create CJSON object\n");
	cJSON_Delete(root);
	return NULL;
}

static struct afc_resp_code parse_afc_resp_code(cJSON *resp_info_obj)
{
	struct afc_resp_code resp_code = {0};
	cJSON *resp_status, *short_description;

	if (!resp_info_obj) {
		afc_printf(MSG_ERROR, "invalid CJSON response object");
		return resp_code;
	}

	resp_status = cJSON_GetObjectItemCaseSensitive(resp_info_obj, "responseCode");
	if (resp_status)
		resp_code.resp_status = (uint8_t)cJSON_GetNumberValue(resp_status);

	short_description = cJSON_GetObjectItemCaseSensitive(resp_info_obj, "shortDescription");
	if (short_description) {
		strncpy(resp_code.short_description, cJSON_GetStringValue(short_description),
				sizeof(short_description) - 1);
	}

	return resp_code;
}

static struct afc_resp_chan_info parse_afc_resp_chan_info(cJSON *chan_info_array)
{
	int num_chan;
	int num_max_eirp;
	int chan_count, eirp_count;
	struct afc_resp_chan_info chan_info = {0};
	cJSON *global_op_class, *channel_cfi_array, *cfi_item;
	cJSON *max_eirp_array, *max_eirp_item;

	if (!chan_info_array) {
		afc_printf(MSG_ERROR, "invalid channel information");
		return chan_info;
	}

	global_op_class = cJSON_GetObjectItemCaseSensitive(chan_info_array, "globalOperatingClass");
	if (global_op_class) {
		chan_info.global_op_class = (uint16_t)cJSON_GetNumberValue(global_op_class);
		channel_cfi_array = cJSON_GetObjectItemCaseSensitive(chan_info_array, "channelCfi");
		if (channel_cfi_array) {
			num_chan = cJSON_GetArraySize(channel_cfi_array);
			chan_info.num_chan_cfi = (uint8_t)num_chan;
			if (num_chan > 0) {
				chan_info.channel_cfi = (uint8_t*)zalloc(num_chan * sizeof(uint8_t));
				if (chan_info.channel_cfi) {
					for (chan_count = 0; chan_count < num_chan; chan_count++) {
						cfi_item = cJSON_GetArrayItem(channel_cfi_array, chan_count);
						if (cfi_item)
							chan_info.channel_cfi[chan_count] = (uint8_t)cJSON_GetNumberValue(cfi_item);
					}
				}
			}
		}

		max_eirp_array = cJSON_GetObjectItemCaseSensitive(chan_info_array, "maxEirp");
		if (max_eirp_array && cJSON_IsArray(max_eirp_array)) {
			num_max_eirp = cJSON_GetArraySize(max_eirp_array);
			chan_info.max_eirp = (double*)zalloc(num_max_eirp * sizeof(double));
			if (chan_info.max_eirp) {
				for (eirp_count = 0; eirp_count < num_max_eirp; eirp_count++) {
					max_eirp_item = cJSON_GetArrayItem(max_eirp_array, eirp_count);
					if (max_eirp_item && cJSON_IsNumber(max_eirp_item))
						chan_info.max_eirp[eirp_count] = max_eirp_item->valuedouble;
				}
			}
		}
	}

	return chan_info;
}

size_t afc_populate_afc_spectrum_inquiry_resp_cb(void *data, size_t size, size_t nmemb, void *userdata)
{
	int iter = 0;
	char *json_data;
	int num_responses;
	cJSON *json, *responses_array, *first_response, *freq_info_array, *freq_info_item;
	cJSON *freq_range_obj, *chan_info_array, *chan_info_item, *resp_info_obj, *expire_time;

	json = cJSON_Parse(data);
	if (!json)
		return NULL;

	json_data = cJSON_Print(json);
	if (!json_data) {
		cJSON_Delete(json);
	}

	afc_printf(MSG_INFO, "afc server response = %s", json_data);

	strncpy(afc_response.version,
			cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(json, "version")),
			sizeof(afc_response.version) - 1);
	responses_array = cJSON_GetObjectItemCaseSensitive(json,
													   "availableSpectrumInquiryResponses");
	if (!cJSON_IsArray(responses_array))
		goto fail;

	num_responses = cJSON_GetArraySize(responses_array);
	if (num_responses > 0) {
		first_response = cJSON_GetArrayItem(responses_array, 0);
		if (!first_response)
			goto fail;

		strncpy(afc_response.request_id,
				cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(first_response, "requestId")),
				sizeof(afc_response.request_id) - 1);
		strncpy(afc_response.rule_set_ids,
				cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(first_response, "rulesetId")),
				sizeof(afc_response.rule_set_ids) - 1);

		freq_info_array = cJSON_GetObjectItemCaseSensitive(first_response,
				"availableFrequencyInfo");
		if (freq_info_array) {
			freq_info_item = NULL;
			afc_response.num_freq_info = cJSON_GetArraySize(freq_info_array);
			if (afc_response.num_freq_info) {
				afc_response.freq_info = (struct afc_resp_freq_info *)zalloc(
						afc_response.num_freq_info * sizeof(struct afc_resp_freq_info));
				if (!afc_response.freq_info)
					goto fail;

				cJSON_ArrayForEach(freq_info_item, freq_info_array) {
					freq_range_obj = cJSON_GetObjectItemCaseSensitive(freq_info_item,
							"frequencyRange");
					if (freq_range_obj) {
						afc_response.freq_info[iter].freq_range.low_frequency =
								(uint16_t)cJSON_GetNumberValue(
								cJSON_GetObjectItemCaseSensitive(freq_range_obj, "lowFrequency"));
						afc_response.freq_info[iter].freq_range.high_frequency =
								(uint16_t)cJSON_GetNumberValue(
								cJSON_GetObjectItemCaseSensitive(freq_range_obj, "highFrequency"));
					} else {
						goto fail;
					}
					afc_response.freq_info[iter].max_psd = cJSON_GetNumberValue(
							cJSON_GetObjectItemCaseSensitive(freq_info_item, "maxPsd"));
					iter++;
				}
			}
		}

		chan_info_array = cJSON_GetObjectItemCaseSensitive(first_response,
				"availableChannelInfo");
		if (chan_info_array) {
			chan_info_item = NULL;
			afc_response.num_chan_info = cJSON_GetArraySize(chan_info_array);
			if (afc_response.num_chan_info) {
				afc_response.chan_info = (struct afc_resp_chan_info *)zalloc(
						afc_response.num_chan_info * sizeof(struct afc_resp_chan_info));
				if (!afc_response.chan_info)
					goto fail;

				iter = 0;
				cJSON_ArrayForEach(chan_info_item, chan_info_array) {
					afc_response.chan_info[iter] = parse_afc_resp_chan_info(chan_info_item);
					iter++;
				}
			}
		}

		expire_time = cJSON_GetStringValue(cJSON_GetObjectItemCaseSensitive(first_response, "availabilityExpireTime"));
		if (expire_time)
			strncpy(afc_response.expire_time, expire_time, sizeof(afc_response.expire_time) - 1);

		resp_info_obj = cJSON_GetObjectItemCaseSensitive(first_response, "response");
		if (resp_info_obj)
			afc_response.resp_info = parse_afc_resp_code(resp_info_obj);
	}

	cJSON_free(json_data);
	cJSON_Delete(json);
	return size * nmemb;

fail:
	cJSON_free(json_data);
	cJSON_Delete(json);
	return 0;
}
