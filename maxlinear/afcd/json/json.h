/******************************************************************************

		 Copyright (c) 2023, MaxLinear, Inc.

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

*******************************************************************************/
#include <stdio.h>
#include <cjson/cJSON.h>
#include <time.h>
#include "lib_curl.h"

#define MAX_NUM_OF_6GHZ_GLOBAL_OP_CLASS 5

size_t afc_populate_afc_spectrum_inquiry_resp_cb(void *data, size_t size, size_t nmemb, void *userdata);
cJSON* afc_spectrum_inquiry_req_params_to_json(struct afc_spectrum_inquiry_req_params *req_params);