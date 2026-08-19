/******************************************************************************

		 Copyright (c) 2023-2024, MaxLinear, Inc.

For licensing information, see the file 'LICENSE' in the root folder of
this software module.

*******************************************************************************/
#include <stdint.h>

#define STARTING_FREQ_6GHZ 5925
#define ENDING_FREQ_6GHZ 7125
#define GLOBAL_OP_CLASS_20_MHZ 131
#define GLOBAL_OP_CLASS_320_MHZ 137
#define ENABLE_CERT_VERIFICATION 1
#define DISABLE_CERT_VERIFICATION 0
#define MAX_NUM_OF_ENTRIES 2000
#define AFCD_CONFIG_FILE "/etc/config/afc_config.conf"

struct afc_req_device_descriptor {
	char serial_number[20];
	struct {
		char nra[4];
		char id[8];
	} certification_id;
	char rule_set_ids[40];
};

struct afc_req_ellipse {
	struct {
		double longitude;
		double latitude;
	} center;
	uint16_t major_axis;
	uint16_t minor_axis;
	uint16_t orientation;
};

struct afc_req_elevation {
	double height;
	char height_type[5];
	uint8_t vertical_uncertainty;
};

struct afc_req_location {
	struct afc_req_ellipse ellipse;
	struct afc_req_elevation elevation;
	uint8_t indoor_deployment;
};

struct afc_req_freq_list {
	struct afc_freq_range {
		uint16_t lower_freq;
		uint16_t higher_freq;
	} *range;
	uint8_t num_range;
};

struct afc_req_chan_list {
	uint16_t global_op_class;
	uint8_t num_chan_cfi;
	uint8_t *channel_cfi;
};

/* Refer from AFC System to AFC Device Interface Specification V1.4 - Section : 4.2.1*/
struct afc_spectrum_inquiry_req_params {
	struct afc_req_device_descriptor device_descriptor;
	struct afc_req_location location;
	struct afc_req_freq_list list_freq_range;
	struct afc_req_chan_list list_chan[5];
	char version[4];
	char request_id[2];
};

struct afc_config {
	struct afc_spectrum_inquiry_req_params req_params;
	char cacert_path[256];
	uint8_t verify_cert;
	char afc_server_url[256];
	char country[3];
};

int afc_read_req_configs (struct afc_config *config);
