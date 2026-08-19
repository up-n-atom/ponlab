/******************************************************************************

		 Copyright (c) 2023, MaxLinear, Inc.

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

*******************************************************************************/
#include <stdio.h>
#include <curl/curl.h>
#include "config_file.h"

#define MAX_RETRY_COUNT 5

int afc_curl_message_to_server(char *json_data, struct afc_config *config);