/******************************************************************************

		 Copyright (c) 2023, MaxLinear, Inc.

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

*******************************************************************************/
#include <curl/curl.h>
#include <stdio.h>
#include "json.h"
#include "afc_debug.h"

int afc_curl_message_to_server(char *json_data, struct afc_config *config)
{
	int retry_count = 0;
	struct curl_slist *headers = NULL;
	CURL *curl;
	CURLcode ret;

	if (!config || !json_data)
		return AFC_STATUS_FAILURE;

	ret = curl_global_init(CURL_GLOBAL_DEFAULT);
	if (ret != CURLE_OK) {
		afc_printf(MSG_ERROR, "curl global init failed(%d), %s", ret, curl_easy_strerror(ret));
		return AFC_STATUS_FAILURE;
	}

	curl = curl_easy_init();
	if (!curl) {
		afc_printf(MSG_ERROR, "curl easy init failed");
		curl_global_cleanup();
		return AFC_STATUS_FAILURE;
	}

	curl_easy_setopt(curl, CURLOPT_URL, config->afc_server_url);
	curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
	curl_easy_setopt(curl, CURLOPT_SSLVERSION, (long)CURL_SSLVERSION_MAX_TLSv1_2);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curl, CURLOPT_CAINFO, config->cacert_path);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, config->verify_cert);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, config->verify_cert);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYSTATUS, config->verify_cert);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

	headers = curl_slist_append(headers, "Content-Type: application/json");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, afc_populate_afc_spectrum_inquiry_resp_cb);

	do {
		ret = curl_easy_perform(curl);
		if (ret != CURLE_OK) {
			afc_printf(MSG_ERROR, "libcurl : error: %s", curl_easy_strerror(ret));
			retry_count++;
		} else {
			break;
		}
	} while (retry_count < MAX_RETRY_COUNT);

	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);

	if (retry_count >= MAX_RETRY_COUNT) {
		afc_printf(MSG_ERROR, "libcurl : retry count exceeded");
		return AFC_STATUS_FAILURE;
	}

	return AFC_STATUS_SUCCESS;
}