/******************************************************************************

	Copyright (c) 2023, MaxLinear, Inc.

	For licensing information, see the file 'LICENSE' in the root folder of
	this software module.

 *******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <signal.h>
#include "driver_nl80211.h"

#define BAND_ID_2_4G	0
#define BAND_ID_5G	2
#define BAND_ID_6G	4
struct nl80211_state state;
u8 g_band_id;
char g_ifname[10];
char fileName[100];
FILE *file = NULL;

static void mxl_vendor_event_handler(struct nlattr **tb)
{
	char ifname[100] = {0};
	u32 subcmd;
	u8 *data = NULL;
	size_t len;

	subcmd = nla_get_u32(tb[NL80211_ATTR_VENDOR_SUBCMD]);
	if (tb[NL80211_ATTR_VENDOR_DATA]) {
		data = nla_data(tb[NL80211_ATTR_VENDOR_DATA]);
		len = nla_len(tb[NL80211_ATTR_VENDOR_DATA]);
	}

	if (tb[NL80211_ATTR_IFINDEX]) {
		if_indextoname(nla_get_u32(tb[NL80211_ATTR_IFINDEX]), ifname);
	}

	switch (subcmd) {
		case LTQ_NL80211_VENDOR_EVENT_RX_MEASURE:
			{
				if (strcmp(ifname, g_ifname) == 0) {
					struct mxl_rx_measure_report *report = (struct mxl_rx_measure_report *)data;

					if(report == NULL) {
						printf("rx_measure_handler: report has no data\n");
						return;
					}

					if (file == NULL) {
						file = fopen(fileName, "w");
						if (file == NULL) {
							printf("rx_measure_handler: Error opening file: %d %s\n", errno, strerror(errno));
							return;
						}

						// Write the headers if the file is empty
						fprintf(file, "ANT0_RSSI_(dBm),ANT0_RCPI_(dBm),ANT0_Noise_Level_(dBm),ANT0_EVM_(dBm),");
						fprintf(file, "ANT1_RSSI_(dBm),ANT1_RCPI_(dBm),ANT1_Noise_Level_(dBm),ANT1_EVM_(dBm),");
						fprintf(file, "ANT2_RSSI_(dBm),ANT2_RCPI_(dBm),ANT2_Noise_Level_(dBm),ANT2_EVM_(dBm),");
						fprintf(file, "ANT3_RSSI_(dBm),ANT3_RCPI_(dBm),ANT3_Noise_Level_(dBm),ANT3_EVM_(dBm)\n");
					}

					// Append the new data to the CSV file
					fprintf(file, "%d,%d,%d,%f,", report->rssi[0], report->rcpi[0], report->noise[0], ((float)report->evm[0])/2);
					fprintf(file, "%d,%d,%d,%f,", report->rssi[1], report->rcpi[1], report->noise[1], ((float)report->evm[1])/2);
					fprintf(file, "%d,%d,%d,%f,", report->rssi[2], report->rcpi[2], report->noise[2], ((float)report->evm[2])/2);
					fprintf(file, "%d,%d,%d,%f\n", report->rssi[3], report->rcpi[3], report->noise[3], ((float)report->evm[3])/2);

					if (report->captures_rem == 0) {
						if (file) {
							fclose(file);
							file = NULL;
						}

						nl80211_cleanup(&state);
						printf("rx_measure_handler: Completed receiving events.\n");
						exit(0);
					}
				}
			}
			break;
	}
}

void sigint_handler(void)
{
	if (file) {
		fclose(file);
		file = NULL;
	}

	nl80211_cleanup(&state);
	printf("rx_measure_handler: Received SIGINT. Stopped while receiving events.\n");
	exit(1);
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s <band_id>\n", argv[0]);
		return 1;
	}

	g_band_id = atoi(argv[1]);
	if ((g_band_id != BAND_ID_2_4G) && (g_band_id != BAND_ID_5G) && (g_band_id != BAND_ID_6G)) {
		printf("rx_measure_handler: Invalid Band id: %d\n", g_band_id);
		printf("Valid bands: 2.4G:0  5G:2  6G:4\n");
		return 1;
	}

	snprintf(g_ifname, sizeof(g_ifname), "wlan%d", g_band_id);
	snprintf(fileName, sizeof(fileName), "/tmp/rx_measure%d.csv", g_band_id);

	printf("rx_measure_handler: Listening on iface %s\n",g_ifname);

	state.event_handler = mxl_vendor_event_handler;
	if (nl80211_init(&state)) {
		printf("rx_measure_handler: netlink init failed: %d %s\n", errno, strerror(errno));
		return 1;
	}

	signal(SIGINT, sigint_handler);

	//Start listening to events
	listen_events(&state);

	if (file) {
		fclose(file);
		file = NULL;
	}

	nl80211_cleanup(&state);

	return 0;
}
