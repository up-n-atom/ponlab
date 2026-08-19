/******************************************************************************

		 Copyright (c) 2023, MaxLinear, Inc.

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

*******************************************************************************/

#include <stdint.h>
#include <time.h>
#include "afc_debug.h"

#define TIMEOUT_INTERVAL_IN_SEC 86400 /* 24 hours */
#define AFCD_RESP_DUMP_FILE  "/tmp/afc_resp_dump.db"
#define AFCD_SOCKET_PATH "/tmp/afc_ctrl_socket"
#define ONE_HOUR_IN_SECONDS 3600

#define UNUSED_PARAM(param) ((void)(param))

struct afc_resp_freq_range {
	uint16_t low_frequency;
	uint16_t high_frequency;
};

struct afc_resp_freq_info {
	struct afc_resp_freq_range freq_range;
	double max_psd;
};

struct afc_resp_chan_info {
	double *max_eirp;
	uint16_t global_op_class;
	uint8_t *channel_cfi;
	uint8_t num_chan_cfi;
};

struct afc_resp_code {
	uint8_t resp_status;
	char short_description[30];
};

/* Refer from AFC System to AFC Device Interface Specification V1.4 - Section : 4.2.2*/
struct afc_spectrum_inquiry_resp {
	struct afc_resp_freq_info *freq_info;
	struct afc_resp_chan_info *chan_info;
	struct afc_resp_code resp_info;
	int num_freq_info;
	int num_chan_info;
	char version[4];
	char request_id[2];
	char rule_set_ids[40];
	char expire_time[30];
	char country[3];
};

enum afc_status afc_query_server(void);