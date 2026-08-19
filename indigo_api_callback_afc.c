
/* Copyright (c) 2020 Wi-Fi Alliance                                                */

/* Permission to use, copy, modify, and/or distribute this software for any         */
/* purpose with or without fee is hereby granted, provided that the above           */
/* copyright notice and this permission notice appear in all copies.                */

/* THE SOFTWARE IS PROVIDED 'AS IS' AND THE AUTHOR DISCLAIMS ALL                    */
/* WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                    */
/* WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL                     */
/* THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR                       */
/* CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING                        */
/* FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF                       */
/* CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT                       */
/* OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS                          */
/* SOFTWARE. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#include "indigo_api.h"
#include "vendor_specific.h"
#include "utils.h"
#include "wpa_ctrl.h"
#include "indigo_api_callback.h"


/* Save TLVs in afcd_configure and Send in afcd_operation */
char server_url[S_BUFFER_LEN];
char geo_area[8];
char ca_cert[S_BUFFER_LEN];
int prior_power_cycle = 0;
char * afc_request_command_str = "hostapd_cli -i wlan4 send_afc_request";

void register_apis() {
    register_api(API_GET_CONTROL_APP_VERSION, NULL, get_control_app_handler);
    //register_api(API_AFC_GET_MAC_ADDR, NULL, afc_get_mac_addr_handler);
    register_api(API_AFCD_CONFIGURE, NULL, afcd_configure_handler);
    register_api(API_AFCD_OPERATION, NULL, afcd_operation_handler);
    register_api(API_AFCD_GET_INFO, NULL, afcd_get_info_handler);
}

static int get_control_app_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    char buffer[S_BUFFER_LEN];
#ifdef _VERSION_
    snprintf(buffer, sizeof(buffer), "%s", _VERSION_);
#else
    snprintf(buffer, sizeof(buffer), "%s", TLV_VALUE_APP_VERSION);
#endif

    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, TLV_VALUE_STATUS_OK);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(TLV_VALUE_OK), TLV_VALUE_OK);
    fill_wrapper_tlv_bytes(resp, TLV_CONTROL_APP_VERSION, strlen(buffer), buffer);
    return 0;
}

static int afcd_get_info_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    struct tlv_hdr *tlv;
    int freq , channel, cfi = 0;
    char response[S_BUFFER_LEN];

    /* NEW GET INFO BLOCK - START */
    struct wpa_ctrl *w = NULL;
    size_t resp_len = 0;
    char cmd[32];
    char w_response[L_BUFFER_LEN];
    char connected_chan[S_BUFFER_LEN]; //CAN BE ANYTHING
    char center_freq_index[S_BUFFER_LEN];
    /* NEW GET INFO BLOCK - END */

    memset(response, 0, sizeof(response));
    
    /* NEW GET INFO BLOCK - START */
    //Open default hostapd
    w = wpa_ctrl_open(get_hapd_ctrl_path());
    if (!w) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to connect to hostapd via %s", get_hapd_ctrl_path());
    } else {    
        /* Assemble hostapd command */
        memset(cmd, 0, sizeof(cmd));
        snprintf(cmd, sizeof(cmd), "STATUS");
        /* Send command to hostapd UDS socket */
        resp_len = sizeof(w_response) - 1;
        memset(w_response, 0, sizeof(w_response));
        wpa_ctrl_request(w, cmd, strlen(cmd), w_response, &resp_len, NULL);
        /* Check w_response */
        memset(connected_chan, 0, sizeof(connected_chan));
        get_key_value(connected_chan, w_response, "channel");
        indigo_logger(LOG_LEVEL_ERROR, "Channel found to be %d", atoi(connected_chan));
        memset(center_freq_index, 0, sizeof(center_freq_index));
        get_key_value(center_freq_index, w_response, "eht_oper_centr_freq_seg0_idx");
        indigo_logger(LOG_LEVEL_ERROR, "Center Frequency found to be %d", atoi(center_freq_index));
    }
    /* NEW GET INFO BLOCK - END */

    /* Get current center channel */
    if (w) {
        channel = atoi(connected_chan);
        cfi = atoi(center_freq_index);
        wpa_ctrl_close(w);
    } else {
        channel = 39;
        cfi = 31;
    }
    freq = 5950 + 5*channel;

    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, TLV_VALUE_STATUS_OK);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(TLV_VALUE_OK), TLV_VALUE_OK);
    snprintf(response, sizeof(response), "%d", freq);
    fill_wrapper_tlv_bytes(resp, TLV_AFC_OPER_FREQ, strlen(response), response);
    snprintf(response, sizeof(response), "%d", channel);
    fill_wrapper_tlv_bytes(resp, TLV_AFC_OPER_CHANNEL, strlen(response), response);
    snprintf(response, sizeof(response), "%d", cfi);
    fill_wrapper_tlv_bytes(resp, TLV_AFC_CENTER_FREQ_INDEX, strlen(response), response);
    return 0;
}

int acs_get_info(int tx_power_offset) {
    int freq , channel;
    char response[S_BUFFER_LEN];

    /* NEW GET INFO BLOCK - START */
    struct wpa_ctrl *w = NULL;
    size_t resp_len = 0;
    char cmd[32];
    char w_response[L_BUFFER_LEN];
    char tx_power_reply[S_BUFFER_LEN]; //CAN BE ANYTHING
    char tx_power_cmd[S_BUFFER_LEN];
    int tx_power_value = 0;
    /* NEW GET INFO BLOCK - END */

    if(tx_power_offset == 0) {
        return 0;
    }

    sleep(5); //This sleep will only occur if we are proceeding with power offset
    memset(response, 0, sizeof(response));

    //Open default hostapd
    w = wpa_ctrl_open(get_hapd_ctrl_path());
    if (!w) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to connect to hostapd via %s", get_hapd_ctrl_path());
    } else {    
        /* Assemble hostapd command */
        memset(cmd, 0, sizeof(cmd));
        snprintf(cmd, sizeof(cmd), "STATUS");
        /* Send command to hostapd UDS socket */
        resp_len = sizeof(w_response) - 1;
        memset(w_response, 0, sizeof(w_response));
        wpa_ctrl_request(w, cmd, strlen(cmd), w_response, &resp_len, NULL);
        /* Check w_response */
        memset(tx_power_reply, 0, sizeof(tx_power_reply));
        get_key_value(tx_power_reply, w_response, "max_txpower");
        indigo_logger(LOG_LEVEL_ERROR, "Tx power found to be %d", atoi(tx_power_reply));
    }


    /* Get current center channel */
    if (w) {
        tx_power_value = atoi(tx_power_reply) - tx_power_offset;
        wpa_ctrl_close(w);
    } else {
        tx_power_value = 24;
    }
    memset(tx_power_cmd, 0, sizeof(tx_power_cmd));
    sprintf(tx_power_cmd, "iw wlan4 iwlwav sFixedPower 32 255 %d 1", tx_power_value);
    system(tx_power_cmd);

    return 0;
}

/*Used to correct for power issues if calibration at test tool or card is not possible*/
int calibrate_tx_power(int tx_power_offset) {
    int freq , channel;
    char response[S_BUFFER_LEN];

    /* NEW GET INFO BLOCK - START */
    struct wpa_ctrl *w = NULL;
    size_t resp_len = 0;
    char cmd[32];
    char w_response[L_BUFFER_LEN];
    char tx_power_reply[S_BUFFER_LEN]; //CAN BE ANYTHING
    char tx_power_cmd[S_BUFFER_LEN];
    int tx_power_value = 0;
    /* NEW GET INFO BLOCK - END */

    if(tx_power_offset == 0) {
        return 0;
    }

    sleep(5); //This sleep will only occur if we are proceeding with power offset
    memset(response, 0, sizeof(response));

    //Open default hostapd
    w = wpa_ctrl_open(get_hapd_ctrl_path());
    if (!w) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to connect to hostapd via %s", get_hapd_ctrl_path());
    } else {    
        /* Assemble hostapd command */
        memset(cmd, 0, sizeof(cmd));
        snprintf(cmd, sizeof(cmd), "STATUS"); //May also use GET_MAX_TX_POWER
        /* Send command to hostapd UDS socket */
        resp_len = sizeof(w_response) - 1;
        memset(w_response, 0, sizeof(w_response));
        wpa_ctrl_request(w, cmd, strlen(cmd), w_response, &resp_len, NULL);
        /* Check w_response */
        memset(tx_power_reply, 0, sizeof(tx_power_reply));
        get_key_value(tx_power_reply, w_response, "max_txpower");
        indigo_logger(LOG_LEVEL_ERROR, "Tx power found to be %d", atoi(tx_power_reply));
    }

    /* Get current center channel */
    if (w) {
        tx_power_value = atoi(tx_power_reply) - tx_power_offset;
        wpa_ctrl_close(w);
    } else {
        tx_power_value = 24;
    }
    memset(tx_power_cmd, 0, sizeof(tx_power_cmd));
    sprintf(tx_power_cmd, "iw wlan4 iwlwav sFixedPower 32 255 %d 1", tx_power_value);
    system(tx_power_cmd);

    return 0;
}


int insert_afc_config(unsigned short tlv_id, char *tlv_val) {
    FILE *f_ptr, *f_tmp_ptr;
    char *path = "/etc/config/afc_config.conf";
    char *tmp_path = "/etc/config/afc_config.conf.tmp";
    char buffer[S_BUFFER_LEN];
    char replacement_buffer[S_BUFFER_LEN];
    char target_str[S_BUFFER_LEN];

    /* Find string to locate and string replacing that line */
    memset(replacement_buffer, 0, sizeof(replacement_buffer));
    memset(target_str, 0, sizeof(target_str));
    if (tlv_id == TLV_AFC_SERVER_URL) {
        sprintf(target_str, "afc_url=");
        sprintf(replacement_buffer, "afc_url=%s/availableSpectrumInquiry\n", tlv_val);
    } else if (tlv_id == TLV_AFC_CA_CERT) {
        sprintf(target_str, "cacert_path=");
        sprintf(replacement_buffer, "cacert_path=/etc/certs/%s\n", tlv_val);
    } else if (tlv_id == TLV_AFC_BANDWIDTH) {
        ;
    } else {
        indigo_logger(LOG_LEVEL_ERROR, "Not a defined TLV %d to be inserted into AFCD config.", tlv_id);
        return -1;
    }

    f_ptr = fopen(path, "r");
    f_tmp_ptr = fopen(tmp_path, "w");

    if (f_ptr == NULL || f_tmp_ptr == NULL) {
        indigo_logger(LOG_LEVEL_ERROR, "Failed to open the files");
        return -1;
    }

    memset(buffer, 0, sizeof(buffer));
    while ((fgets(buffer, S_BUFFER_LEN, f_ptr)) != NULL) {
        if (strstr(buffer, target_str) != NULL) {
            indigo_logger(LOG_LEVEL_DEBUG, 
                "insert config: %s into the AFC conf.", replacement_buffer);
            fputs(replacement_buffer, f_tmp_ptr);
        }
        else
        {
            fputs(buffer, f_tmp_ptr);
        }
    }

    fclose(f_ptr);
    fclose(f_tmp_ptr);

    /* replace original file with new file */
    remove(path);
    rename(tmp_path, path);
    return 0;
}


#define ELLIPSE 0
#define LINEARPOLYGON 1
#define RADIALPOLYGON 2
static int afcd_configure_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    int status = TLV_VALUE_STATUS_OK;
    char *message = TLV_VALUE_OK;
    struct tlv_hdr *tlv;
    int i = 0;
    int bw_set = 0;
    char security[8];
    char bw[8];

    for (i = 0; i < req->tlv_num; i++) {
        struct indigo_tlv *i_tlv;
        char tlv_value[64];
        i_tlv = get_tlv_by_id(req->tlv[i]->id);
        if (i_tlv) {
                memset(tlv_value, 0, sizeof(tlv_value));
                memcpy(tlv_value, req->tlv[i]->value, req->tlv[i]->len);
                indigo_logger(LOG_LEVEL_DEBUG, "TLV: %s - %s", i_tlv->name, tlv_value);
        }
    }

    tlv = find_wrapper_tlv_by_id(req, TLV_AFC_SERVER_URL);
    if (tlv) {
        memset(server_url, 0, sizeof(server_url));
        memcpy(server_url, tlv->value, tlv->len);
        insert_afc_config((unsigned short)TLV_AFC_SERVER_URL, server_url);
    } else {
        indigo_logger(LOG_LEVEL_ERROR, "Missed TLV: TLV_AFC_SERVER_URL");
        status = TLV_VALUE_STATUS_NOT_OK;
        message = TLV_VALUE_NOT_OK;
        goto done;
    }

    tlv = find_wrapper_tlv_by_id(req, TLV_AFC_CA_CERT);
    if (tlv) {        
        memset(ca_cert, 0, sizeof(ca_cert));
        memcpy(ca_cert, tlv->value, tlv->len);
        if (strlen(ca_cert) > 0) {
            indigo_logger(LOG_LEVEL_DEBUG, "Configure root certificate: %s", ca_cert);
            insert_afc_config((unsigned short)TLV_AFC_CA_CERT, ca_cert);
        } else
            indigo_logger(LOG_LEVEL_DEBUG, "Configure NULL root certificate!");
            insert_afc_config((unsigned short)TLV_AFC_CA_CERT, ca_cert); //Must config NULL for NoRootCA case
    } else {
        indigo_logger(LOG_LEVEL_ERROR, "Missed TLV: TLV_AFC_CA_CERT");
        status = TLV_VALUE_STATUS_NOT_OK;
        message = TLV_VALUE_NOT_OK;
        goto done;
    }

    /* BSS Configurations: SSID, Security, Passphrase */
    tlv = find_wrapper_tlv_by_id(req, TLV_AFC_SECURITY_TYPE);
    if (tlv) {
        memset(security, 0, sizeof(security));
        memcpy(security, tlv->value, tlv->len);
        if (atoi(security) == 0) {
            indigo_logger(LOG_LEVEL_DEBUG, "Configure SAE");
        }
    }
    tlv = find_wrapper_tlv_by_id(req, TLV_AFC_BANDWIDTH);
    if (tlv) {
        memset(bw, 0, sizeof(bw));
        memcpy(bw, tlv->value, tlv->len);
        if (atoi(bw) == 0) {
            indigo_logger(LOG_LEVEL_DEBUG, "Configure DUT to 20MHz bandwidth");
#ifdef UPDK
            system("sleep 10");
            system("ubus-cli WiFi.Radio.3.OperatingChannelBandwidth='20MHz'");
            system("sleep 20");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsFallbackPrimaryChan=73");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsFallbackSecChan=0");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsFallbackBW=20");
            system("sleep 7");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsStrictChList='1'");
            system("sleep 7");
            system("ubus-cli WiFi.Radio.3.Vendor.DuplicateBeaconEnabled=0");
            system("sleep 7");
#else
            system("uci set wireless.radio4.acs_fallback_chan='73 0 20'");
            system("uci set wireless.radio4.acs_ch_list='1 5 9 13 17 21 25 29 33 37 41 45 49 53 57 61 65 69 73 77 81 85 89 93'");
            system("uci set wireless.radio4.acs_strict_chanlist='1'");
            system("uci set wireless.radio4.htmode='VHT20'; uci commit wireless; wifi");
#endif
        } else if (atoi(bw) == 1) {
            indigo_logger(LOG_LEVEL_DEBUG, "Configure DUT to 40MHz bandwidth");
#ifdef UPDK
            system("sleep 10");
            system("ubus-cli WiFi.Radio.3.OperatingChannelBandwidth='40MHz'");
            system("sleep 20");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsFallbackPrimaryChan=33");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsFallbackSecChan=37");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsFallbackBW=40");
            system("sleep 7");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsStrictChList='1'");
            system("sleep 7");
            system("ubus-cli WiFi.Radio.3.Vendor.DuplicateBeaconEnabled=1");
            system("sleep 7");
            system("ubus-cli WiFi.Radio.3.Vendor.DuplicateBeaconBw=1");
            system("sleep 20");
#else
            system("uci set wireless.radio4.acs_fallback_chan='33 37 40'");
            system("uci set wireless.radio4.acs_ch_list='3 11 19 27 35 43 51 59 67 75 83 91'");
            system("uci set wireless.radio4.acs_strict_chanlist='1'");
            system("uci set wireless.radio4.htmode='VHT40'; uci commit wireless; wifi");
#endif
        } else if (atoi(bw) == 2) {
            indigo_logger(LOG_LEVEL_DEBUG, "Configure DUT to 80MHz bandwidth");
#ifdef UPDK
            system("sleep 10");
            system("ubus-cli WiFi.Radio.3.OperatingChannelBandwidth='80MHz'");
            system("sleep 20");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsFallbackPrimaryChan=33");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsFallbackSecChan=49");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsFallbackBW=80");
            system("sleep 7");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsStrictChList='1'");
            system("sleep 7");
            system("ubus-cli WiFi.Radio.3.Vendor.DuplicateBeaconEnabled=1");
            system("sleep 7");
            system("ubus-cli WiFi.Radio.3.Vendor.DuplicateBeaconBw=2");
            system("sleep 20");

#else
            system("uci set wireless.radio4.acs_fallback_chan='33 49 80'");
            system("uci set wireless.radio4.acs_ch_list='7 23 39 55 71 87'");
            system("uci set wireless.radio4.acs_strict_chanlist='1'");
            system("uci set wireless.radio4.htmode='VHT80'; uci commit wireless; wifi");
#endif
        } else if (atoi(bw) == 3) {
            indigo_logger(LOG_LEVEL_DEBUG, "Configure DUT to 160MHz bandwidth");
#ifdef UPDK
            system("sleep 10");
            system("ubus-cli WiFi.Radio.3.OperatingChannelBandwidth='160MHz'");
            system("sleep 20");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsFallbackPrimaryChan=73");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsFallbackSecChan=0");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsFallbackBW=20");
            system("sleep 7");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsStrictChList='1'");
            system("sleep 7");
            system("ubus-cli WiFi.Radio.3.Vendor.DuplicateBeaconEnabled=1");
            system("sleep 7");
            system("ubus-cli WiFi.Radio.3.Vendor.DuplicateBeaconBw=3");
            system("sleep 20");
#else
            system("uci set wireless.radio4.acs_fallback_chan='73 0 20'");
            system("uci set wireless.radio4.acs_ch_list='15 47 79'");
            system("uci set wireless.radio4.acs_strict_chanlist='1'");
            system("uci set wireless.radio4.htmode='VHT160'; uci commit wireless; wifi");
#endif
        } else if (atoi(bw) == 4) {
            indigo_logger(LOG_LEVEL_DEBUG, "Configure DUT to 320MHz bandwidth");
#ifdef UPDK
            system("sleep 10");
            system("ubus-cli WiFi.Radio.3.OperatingChannelBandwidth='320MHz-1'");
            system("sleep 20");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsFallbackPrimaryChan=31");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsFallbackSecChan=63");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsFallbackBW=95");
            system("sleep 7");
            system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsStrictChList='1'");
            system("sleep 7");
            system("ubus-cli WiFi.Radio.3.Vendor.DuplicateBeaconEnabled=1");
            system("sleep 7");
            system("ubus-cli WiFi.Radio.3.Vendor.DuplicateBeaconBw=4");
            system("sleep 20");

#else
            system("uci set wireless.radio4.acs_fallback_chan='31 63 95'");
            system("uci set wireless.radio4.acs_ch_list='31 63 95 127 159 191'");
            system("uci set wireless.radio4.acs_strict_chanlist='1'");
            system("uci set wireless.radio4.htmode='EHT320_0'; uci commit wireless; wifi");
#endif
        }
    } else { 
        //If no BW set, default to 20MHz
        indigo_logger(LOG_LEVEL_DEBUG, "NO BW set so, configure DUT to 20MHz bandwidth");
#ifdef UPDK
        system("sleep 10");
        system("ubus-cli WiFi.Radio.3.OperatingChannelBandwidth='20MHz'");
        system("sleep 20");
        system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsFallbackPrimaryChan=73");
        system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsFallbackSecChan=0");
        system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsFallbackBW=20");
        system("sleep 7");
        system("ubus-cli WiFi.Radio.3.Vendor.ACS.AcsStrictChList='1'");
        system("sleep 7");
        system("ubus-cli WiFi.Radio.3.Vendor.DuplicateBeaconEnabled=0");
        system("sleep 7");
#else
        system("uci set wireless.radio4.acs_fallback_chan='73 0 20'");
        system("uci set wireless.radio4.acs_ch_list='1 5 9 13 17 21 25 29 33 37 41 45 49 53 57 61 65 69 73 77 81 85 89 93'");
        system("uci set wireless.radio4.acs_strict_chanlist='1'");
        system("uci set wireless.radio4.htmode='VHT20'; uci commit wireless; wifi");
#endif
        if(prior_power_cycle) {
            //Wifi must stay down after power cycle command
            sleep(10);
            indigo_logger(LOG_LEVEL_DEBUG, "Remove standard power");
#ifdef UPDK
            system("/etc/init.d/prplmesh_whm stop");
#else
            system("wifi down");
#endif
            prior_power_cycle = 0;
        }
    }

    /* Mandatory Registration Configurations */
    tlv = find_wrapper_tlv_by_id(req, TLV_AFC_LOCATION_GEO_AREA);
    if (tlv) {
        memset(geo_area, 0, sizeof(geo_area));
        memcpy(geo_area, tlv->value, tlv->len);
        if (atoi(geo_area) == ELLIPSE) {
            tlv = find_wrapper_tlv_by_id(req, TLV_AFC_ELLIPSE_CENTER);
            tlv = find_wrapper_tlv_by_id(req, TLV_AFC_ELLIPSE_MAJOR_AXIS);
            tlv = find_wrapper_tlv_by_id(req, TLV_AFC_ELLIPSE_MINOR_AXIS);
            tlv = find_wrapper_tlv_by_id(req, TLV_AFC_ELLIPSE_ORIENTATION);
        } else if (atoi(geo_area) == LINEARPOLYGON) {
            tlv = find_wrapper_tlv_by_id(req, TLV_AFC_LINEARPOLY_BOUNDARY);
        } else if (atoi(geo_area) == RADIALPOLYGON){
            tlv = find_wrapper_tlv_by_id(req, TLV_AFC_RADIALPOLY_CENTER);
            tlv = find_wrapper_tlv_by_id(req, TLV_AFC_RADIALPOLY_BOUNDARY);
        }
    } else {
        //indigo_logger(LOG_LEVEL_DEBUG, "Missed TLV: TLV_AFC_LOCATION_GEO_AREA");
    }

done:
    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, status);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(message), message);
    return 0;
}


static int afcd_operation_handler(struct packet_wrapper *req, struct packet_wrapper *resp) {
    struct tlv_hdr *tlv;
    char req_type[8];
    char frame_bw[8];

    /* NEW GET CMD BLOCK - START */
    struct wpa_ctrl *w = NULL;
    size_t resp_len = 0;
    char cmd[32];
    char w_response[L_BUFFER_LEN];
    char connected_chan[S_BUFFER_LEN]; //CAN BE ANYTHING
    /* NEW GET CMD BLOCK - END */


    tlv = find_wrapper_tlv_by_id(req, TLV_AFC_DEVICE_RESET);
    if (tlv) {
        indigo_logger(LOG_LEVEL_DEBUG, "Device reset");
        prior_power_cycle = 0;
        //Change back to LPI mode
        /* Vendor specific: add in vendor_specific_afc.c */

        //Open default hostapd
        w = wpa_ctrl_open(get_hapd_ctrl_path());
        if (!w) {
            indigo_logger(LOG_LEVEL_ERROR, "Failed to connect to hostapd via %s", get_hapd_ctrl_path());
        } else {
            /* Assemble hostapd command */
            memset(cmd, 0, sizeof(cmd));
            snprintf(cmd, sizeof(cmd), "reset_acs_state");
            /* Send command to hostapd UDS socket */
            resp_len = sizeof(w_response) - 1;
            memset(w_response, 0, sizeof(w_response));
            wpa_ctrl_request(w, cmd, strlen(cmd), w_response, &resp_len, NULL);

            /* Assemble hostapd command */
            memset(cmd, 0, sizeof(cmd));
            snprintf(cmd, sizeof(cmd), "SET_POWER_MODE 0");
            /* Send command to hostapd UDS socket */
            resp_len = sizeof(w_response) - 1;
            memset(w_response, 0, sizeof(w_response));
            wpa_ctrl_request(w, cmd, strlen(cmd), w_response, &resp_len, NULL);
            wpa_ctrl_close(w);
        }
        /* Inteface must stay alive but in low power to await settings */
        system("iw wlan4 iwlwav sFixedPower 32 255 12 1");

    }
    tlv = find_wrapper_tlv_by_id(req, TLV_AFC_SEND_SPECTRUM_REQ);
    if (tlv) {

        //Workaround for afc driver improperly adjusting from allowed to disallowed state on the same channel
#ifdef UPDK
        system("/etc/init.d/prplmesh_whm restart");
        sleep(40);
#else
        system("wifi");
#endif
        sleep(20);

        //Send command to Hostap to open 60 second timeout window to recieve standard power commands
        //Contact AFC server to recieve AFC data
        memset(req_type, 0, sizeof(req_type));
        memcpy(req_type, tlv->value, tlv->len);

        //Open default hostapd
        w = wpa_ctrl_open(get_hapd_ctrl_path());
        if (!w) {
            indigo_logger(LOG_LEVEL_ERROR, "Failed to connect to hostapd via %s", get_hapd_ctrl_path());
        } else {
            /* Assemble hostapd command */
            memset(cmd, 0, sizeof(cmd));
            snprintf(cmd, sizeof(cmd), "reset_acs_state");
            /* Send command to hostapd UDS socket */
            resp_len = sizeof(w_response) - 1;
            memset(w_response, 0, sizeof(w_response));
            wpa_ctrl_request(w, cmd, strlen(cmd), w_response, &resp_len, NULL);

            /* Assemble hostapd command */
            memset(cmd, 0, sizeof(cmd));
            snprintf(cmd, sizeof(cmd), "SET_POWER_MODE 1");
            /* Send command to hostapd UDS socket */
            resp_len = sizeof(w_response) - 1;
            memset(w_response, 0, sizeof(w_response));
            wpa_ctrl_request(w, cmd, strlen(cmd), w_response, &resp_len, NULL);

            wpa_ctrl_close(w);
        }

        if (atoi(req_type) == 0) {
            indigo_logger(LOG_LEVEL_DEBUG, "Send Spectrum request with Channel and Frequency based");
            //Load premade config into afcd config file
            sleep(17);
            system(afc_request_command_str);
            calibrate_tx_power(0);
        } else if (atoi(req_type) == 1) {
            indigo_logger(LOG_LEVEL_DEBUG, "Send Spectrum request with Channel based");
            //Load premade config into afcd config file
            sleep(17);
            system(afc_request_command_str);
            calibrate_tx_power(0);
        } else if (atoi(req_type) == 2) {
            indigo_logger(LOG_LEVEL_DEBUG, "Send Spectrum request with Frequency based");
            //Load premade config into afcd config file
            sleep(17);
            system(afc_request_command_str);
            calibrate_tx_power(0);
        }
    }
    tlv = find_wrapper_tlv_by_id(req, TLV_AFC_POWER_CYCLE);
    if (tlv) {
        indigo_logger(LOG_LEVEL_DEBUG, "Trigger power cycle");
        prior_power_cycle = 1;
        /* Vendor specific: add in vendor_specific_afc.c */

        //Open default hostapd
        w = wpa_ctrl_open(get_hapd_ctrl_path());
        if (!w) {
            indigo_logger(LOG_LEVEL_ERROR, "Failed to connect to hostapd via %s", get_hapd_ctrl_path());
        } else {
            /* Assemble hostapd command */
            memset(cmd, 0, sizeof(cmd));
            snprintf(cmd, sizeof(cmd), "SET_POWER_MODE 0");
            /* Send command to hostapd UDS socket */
            resp_len = sizeof(w_response) - 1;
            memset(w_response, 0, sizeof(w_response));
            wpa_ctrl_request(w, cmd, strlen(cmd), w_response, &resp_len, NULL);
            wpa_ctrl_close(w);
        }
        /* Inteface must stay alive but in low power to await settings */
        system("iw wlan4 iwlwav sFixedPower 32 255 12 1");

    }
    tlv = find_wrapper_tlv_by_id(req, TLV_AFC_SEND_TEST_FRAME);
    if (tlv) {
        memset(frame_bw, 0, sizeof(frame_bw));
        memcpy(frame_bw, tlv->value, tlv->len);
        if (atoi(frame_bw) == 0) {
            indigo_logger(LOG_LEVEL_DEBUG, "Trigger DUT to send test frames for 20MHz bandwidth");
        } else if (atoi(frame_bw) == 1) {
            indigo_logger(LOG_LEVEL_DEBUG, "Trigger DUT to send test frames for 40MHz bandwidth");
        } else if (atoi(frame_bw) == 2) {
            indigo_logger(LOG_LEVEL_DEBUG, "Trigger DUT to send test frames for 80MHz bandwidth");
        } else if (atoi(frame_bw) == 3) {
            indigo_logger(LOG_LEVEL_DEBUG, "Trigger DUT to send test frames for 160MHz bandwidth");
        } else if (atoi(frame_bw) == 4) {
            indigo_logger(LOG_LEVEL_DEBUG, "Trigger DUT to send test frames for 320MHz bandwidth");
        }
    }
    tlv = find_wrapper_tlv_by_id(req, TLV_AFC_CONNECT_SP_AP);
    if (tlv) {
        indigo_logger(LOG_LEVEL_DEBUG, "Trigger AFC DUT to initiate connection procedure between AFC DUT and SP Access Point");
        /* Vendor specific */

        //Open default hostapd
        w = wpa_ctrl_open(get_hapd_ctrl_path());
        if (!w) {
            indigo_logger(LOG_LEVEL_ERROR, "Failed to connect to hostapd via %s", get_hapd_ctrl_path());
        }
        else {    
            // Assemble hostapd command 
            memset(cmd, 0, sizeof(cmd));
            snprintf(cmd, sizeof(cmd), "SET_POWER_MODE 0");
            // Send command to hostapd UDS socket
            resp_len = sizeof(w_response) - 1;
            memset(w_response, 0, sizeof(w_response));
            wpa_ctrl_request(w, cmd, strlen(cmd), w_response, &resp_len, NULL);
            wpa_ctrl_close(w);
        }


        system(afc_request_command_str);
        sleep(1);
        /* Inteface must stay alive but in low power to await settings */
        system("iw wlan4 iwlwav sFixedPower 32 255 12 1");
    }

    fill_wrapper_message_hdr(resp, API_CMD_RESPONSE, req->hdr.seq);
    fill_wrapper_tlv_byte(resp, TLV_STATUS, TLV_VALUE_STATUS_OK);
    fill_wrapper_tlv_bytes(resp, TLV_MESSAGE, strlen(TLV_VALUE_OK), TLV_VALUE_OK);
    return 0;
}
