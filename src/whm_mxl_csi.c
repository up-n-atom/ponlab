/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/

/*  *****************************************************************************
*         File Name    : whm_mxl_csi.c                                         *
*         Description  : WiFi CSI related API                                  *
*                                                                              *
*  *****************************************************************************/

#include <sys/socket.h>
#include <sys/un.h>
#include <swl/swl_common.h>
#include <swla/swla_mac.h>

#include "wld/wld.h"
#include "wld/wld_util.h"
#include "wld/wld_accesspoint.h"
#include "wld/wld_assocdev.h"
#include "wld/wld_nl80211_compat.h"
#include "wld/wld_nl80211_api.h"
#include "wld/wld_ap_nl80211.h"

#include "whm_mxl_csi.h"
#include "whm_mxl_vap.h"

#define ME "mxlCsi"

whm_mxl_connection_t conn = {.serverfd = -1, .clientfd = -1};

/**
 * After adding a CSI client for a specific MAC address and monitor interval,
 * the Maxlinear WiFi driver sends back periodically the CSI raw data over a specific Netlink vendor stats event.
 * This function is called from the event parser to forward the received CSI stats to the opened unix socket stream,
 * which is used by third-party Apps to manage WiFi sensing data raw.
 */
void mxl_rad_sendCsiStatsOverUnixSocket(wifi_csi_driver_nl_event_data_t* stats) {
    ASSERT_FALSE(conn.serverfd < 0, , ME, "No server socket created");

    struct sockaddr_un clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    if(conn.clientfd == -1) {
        conn.clientfd = accept(conn.serverfd, (struct sockaddr*) &clientAddr, &addrLen);
        if(conn.clientfd == -1) {
            if((errno == EWOULDBLOCK) || (errno == EAGAIN)) {
                SAH_TRACEZ_INFO(ME, "No remote peer connected");
            } else {
                SAH_TRACEZ_ERROR(ME, "Accept failed: error:%d:%s", errno, strerror(errno));
            }
            return;
        }
        SAH_TRACEZ_INFO(ME, "New remote peer connected, fd %d", conn.clientfd);
    }

    fd_set read_fds;
    struct timeval timeout;
    FD_ZERO(&read_fds);
    FD_SET(conn.clientfd, &read_fds);

    timeout.tv_sec = 0;
    timeout.tv_usec = 5000;

    int clientReady = select(conn.clientfd + 1, &read_fds, NULL, NULL, &timeout);
    if(clientReady < 0) {
        SAH_TRACEZ_ERROR(ME, "Select failed: error:%d:%s", errno, strerror(errno));
        return;
    } else if(clientReady == 0) {
        // send data to remote peer
        SAH_TRACEZ_INFO(ME, "Send stats to connected remote peer, fd %d", conn.clientfd);
        send(conn.clientfd, stats, sizeof(wifi_csi_driver_nl_event_data_t), 0);
    } else {
        // check if remote peer already connected
        char buffer[1024];
        int bytes_received = recv(conn.clientfd, buffer, sizeof(buffer), 0);
        if(bytes_received <= 0) {
            if((bytes_received == 0) || (errno == ECONNRESET) || (errno == ECONNABORTED)) {
                SAH_TRACEZ_INFO(ME, "Remote peer disconnected, fd %d", conn.clientfd);
            } else {
                SAH_TRACEZ_ERROR(ME, "Recv data from peer failed: error:%d:%s", errno, strerror(errno));
            }
            close(conn.clientfd);
            conn.clientfd = -1;
        }
    }
}

static swl_rc_ne s_getCsiCountersCb(swl_rc_ne rc, struct nlmsghdr* nlh, void* priv) {
    ASSERT_FALSE((rc <= SWL_RC_ERROR), rc, ME, "Request error");
    ASSERT_NOT_NULL(nlh, SWL_RC_ERROR, ME, "NULL");

    struct genlmsghdr* gnlh = (struct genlmsghdr*) nlmsg_data(nlh);
    ASSERTI_EQUALS(gnlh->cmd, NL80211_CMD_VENDOR, SWL_RC_OK, ME, "unexpected cmd %d", gnlh->cmd);

    struct nlattr* tb[NL80211_ATTR_MAX + 1] = {};
    if(nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0), genlmsg_attrlen(gnlh, 0), NULL)) {
        SAH_TRACEZ_ERROR(ME, "Failed to parse netlink message");
        return SWL_RC_ERROR;
    }

    // parse data table
    void* data = NULL;
    ASSERT_NOT_NULL(tb[NL80211_ATTR_VENDOR_DATA], SWL_RC_ERROR, ME, "NULL");
    data = nla_data(tb[NL80211_ATTR_VENDOR_DATA]);
    whm_mxl_csiCounters_t* csiCounters = (whm_mxl_csiCounters_t*) data;
    SAH_TRACEZ_INFO(ME, "SendQosNullCnt %"PRIu64" | RecvFrameCnt %"PRIu64" | SendNlCsiData %"PRIu64" | ReqInfoCnt %"PRIu64"",
                    csiCounters->csiSendQosNullCount, csiCounters->csiRecvFrameCount,
                    csiCounters->csiSendNlCsiData, csiCounters->csiReqInfoCount);

    whm_mxl_csiCounters_t* globCsiCounters = (whm_mxl_csiCounters_t*) priv;
    ASSERT_NOT_NULL(globCsiCounters, SWL_RC_ERROR, ME, "NULL");

    globCsiCounters->csiSendQosNullCount += csiCounters->csiSendQosNullCount;
    globCsiCounters->csiRecvFrameCount += csiCounters->csiRecvFrameCount;
    globCsiCounters->csiSendNlCsiData += csiCounters->csiSendNlCsiData;
    globCsiCounters->csiReqInfoCount += csiCounters->csiReqInfoCount;

    return rc;
}

swl_rc_ne whm_mxl_rad_sensingCsiStats(T_Radio* pRad, wld_csiState_t* csimonState) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");

    whm_mxl_csiCounters_t csiCounters = {0};

    amxc_llist_for_each(it, &pRad->csiClientList) {
        wld_csiClient_t* client = amxc_llist_it_get_data(it, wld_csiClient_t, it);
        swl_macBin_t clientMacBin = SWL_MAC_BIN_NEW();
        SWL_MAC_CHAR_TO_BIN(&clientMacBin, &client->macAddr);

        T_AccessPoint* pAP = wld_rad_get_associated_ap(pRad, clientMacBin.bMac);
        ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "NULL");
        ASSERTI_TRUE(mxl_isApReadyToProcessVendorCmd(pAP), SWL_RC_INVALID_STATE, ME, "AP not ready to process Vendor cmd");

        SAH_TRACEZ_INFO(ME, "%s: Get stats for client : [" SWL_MAC_FMT "]", pRad->Name, SWL_MAC_ARG(clientMacBin.bMac));

        uint32_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_GET_CSI_COUNTERS;
        wld_ap_nl80211_sendVendorSubCmd(pAP, OUI_MXL, subcmd, clientMacBin.bMac, ETHER_ADDR_LEN,
                                        VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, s_getCsiCountersCb, &csiCounters);
    }

    ASSERT_NOT_NULL(pRad, SWL_RC_ERROR, ME, "NULL");
    amxc_var_set_type(csimonState->vendorCounters, AMXC_VAR_ID_HTABLE);
    amxc_var_add_key(uint32_t, csimonState->vendorCounters, "SendQosNullCnt", csiCounters.csiSendQosNullCount);
    amxc_var_add_key(uint32_t, csimonState->vendorCounters, "RecvFrameCnt", csiCounters.csiRecvFrameCount);
    amxc_var_add_key(uint32_t, csimonState->vendorCounters, "SendNlCsiData", csiCounters.csiSendNlCsiData);
    amxc_var_add_key(uint32_t, csimonState->vendorCounters, "ReqInfoCnt", csiCounters.csiReqInfoCount);

    return SWL_RC_OK;
}

swl_rc_ne whm_mxl_rad_sensingResetStats(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");

    amxc_llist_for_each(it, &pRad->csiClientList) {
        wld_csiClient_t* client = amxc_llist_it_get_data(it, wld_csiClient_t, it);
        swl_macBin_t clientMacBin = SWL_MAC_BIN_NEW();
        SWL_MAC_CHAR_TO_BIN(&clientMacBin, &client->macAddr);

        T_AccessPoint* pAP = wld_rad_get_associated_ap(pRad, clientMacBin.bMac);
        ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "NULL");
        ASSERTI_TRUE(mxl_isApReadyToProcessVendorCmd(pAP), SWL_RC_INVALID_STATE, ME, "AP not ready to process Vendor cmd");

        SAH_TRACEZ_INFO(ME, "%s: Reset stats for client : [" SWL_MAC_FMT "]", pRad->Name, SWL_MAC_ARG(clientMacBin.bMac));

        struct intel_vendor_reset_statistics data;
        data.category = MXL_RESET_STATS_CATEGORY_STA;
        memcpy(data.addr, clientMacBin.bMac, ETHER_ADDR_LEN);

        uint32_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_RESET_STATISTICS;
        wld_ap_nl80211_sendVendorSubCmd(pAP, OUI_MXL, subcmd, &data, sizeof(struct intel_vendor_reset_statistics),
                                        VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, NULL, NULL);
    }
    return SWL_RC_OK;
}

uint8_t s_msInterval2SamplingRate(uint32_t msInterval) {
    if(msInterval <= 0) {
        return MXL_CSI_MIN_SRATE;
    }
    float samplingRate = 1000.0 / msInterval;
    // Due to Mxl specification, the sampling rate must be between 10-30Hz
    samplingRate = samplingRate < MXL_CSI_MIN_SRATE ? MXL_CSI_MIN_SRATE : samplingRate;
    samplingRate = samplingRate > MXL_CSI_MAX_SRATE ? MXL_CSI_MAX_SRATE : samplingRate;

    return (uint8_t) samplingRate;
}

static swl_rc_ne s_setCsiAutoRate(T_Radio* pRad, wld_csiClient_t* client, bool enable) {
    swl_macBin_t clientMacBin = SWL_MAC_BIN_NEW();
    SWL_MAC_CHAR_TO_BIN(&clientMacBin, &client->macAddr);

    T_AccessPoint* pAP = wld_rad_get_associated_ap(pRad, clientMacBin.bMac);
    ASSERT_NOT_NULL(pAP, SWL_RC_INVALID_PARAM, ME, "NULL");
    ASSERTI_TRUE(mxl_isApReadyToProcessVendorCmd(pAP), SWL_RC_INVALID_STATE, ME, "AP not ready to process Vendor cmd");
    T_SSID* pSSID = pAP->pSSID;
    ASSERT_NOT_NULL(pSSID, SWL_RC_INVALID_PARAM, ME, "NULL");

    whm_mxl_csiAutoRate_t data;
    memset(&data, 0, sizeof(data));
    data.saFamily = 1;
    memcpy(data.staMac, clientMacBin.bMac, ETHER_ADDR_LEN);
    memcpy(data.assocApMac, pSSID->BSSID, ETHER_ADDR_LEN);
    data.rate = s_msInterval2SamplingRate(client->monitorInterval);
    data.enable = enable;

    SAH_TRACEZ_INFO(ME, "%s: %s CSI for client : [" SWL_MAC_FMT "]", pRad->Name,
                    (data.enable) ? "Enable" : "Disable",
                    SWL_MAC_ARG(data.staMac));

    uint32_t subcmd = LTQ_NL80211_VENDOR_SUBCMD_SET_CSI_AUTO_RATE;
    return wld_ap_nl80211_sendVendorSubCmd(pAP, OUI_MXL, subcmd, &data, sizeof(whm_mxl_csiAutoRate_t),
                                           VENDOR_SUBCMD_IS_SYNC, VENDOR_SUBCMD_IS_WITHOUT_ACK, 0, NULL, NULL);
}

static wld_csiClient_t* s_findCsiClient(T_Radio* pRad, swl_macChar_t clientMacAddr) {
    amxc_llist_for_each(it, &pRad->csiClientList) {
        wld_csiClient_t* client = amxc_llist_it_get_data(it, wld_csiClient_t, it);
        if(swl_str_matches(client->macAddr.cMac, clientMacAddr.cMac)) {
            return client;
        } else {
            continue;
        }
    }
    return NULL;
}

swl_rc_ne whm_mxl_rad_sensingDelClient(T_Radio* pRad, swl_macChar_t clientMacAddr) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");

    swl_rc_ne rc;
    wld_csiClient_t* client = s_findCsiClient(pRad, clientMacAddr);
    ASSERT_NOT_NULL(client, SWL_RC_INVALID_PARAM, ME, "No client %s to be deleted", clientMacAddr.cMac);

    rc = s_setCsiAutoRate(pRad, client, false);

    return rc;
}

swl_rc_ne whm_mxl_rad_sensingAddClient(T_Radio* pRad, wld_csiClient_t* client) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");

    return s_setCsiAutoRate(pRad, client, true);
}

static int s_setSocketNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if(flags == -1) {
        SAH_TRACEZ_ERROR(ME, "Get sockfd flags failed: error:%d:%s", errno, strerror(errno));
        return -1;
    }
    if(fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
        SAH_TRACEZ_ERROR(ME, "Set non-blocking flag failed: error:%d:%s", errno, strerror(errno));
        return -1;
    }
    return 0;
}

static int s_createStatsSocket() {
    int32_t sockfd;
    struct sockaddr_un socket_addr;
    if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        SAH_TRACEZ_ERROR(ME, "Create socket failed: error:%d:%s", errno, strerror(errno));
        return -1;
    }

    memset(&socket_addr, 0, sizeof(struct sockaddr_un));
    socket_addr.sun_family = AF_UNIX;
    strncpy(socket_addr.sun_path, CONFIG_MOD_WHM_CSI_SOCKET_PATH, sizeof(socket_addr.sun_path) - 1);
    if(bind(sockfd, (struct sockaddr*) &socket_addr, sizeof(struct sockaddr_un)) == -1) {
        SAH_TRACEZ_ERROR(ME, "Socket bind failed: error:%d:%s", errno, strerror(errno));
        goto error;
    }

    if(listen(sockfd, 1) < 0) {
        SAH_TRACEZ_ERROR(ME, "Listen connection failed: error:%d:%s", errno, strerror(errno));
        goto error;
    }
    // Set server socket to non-bocking mode
    if(s_setSocketNonBlocking(sockfd) == -1) {
        goto error;
    }

    SAH_TRACEZ_INFO(ME, "Unix Stream Socket created and bound to %s", CONFIG_MOD_WHM_CSI_SOCKET_PATH);
    return sockfd;

error:
    close(sockfd);
    unlink(CONFIG_MOD_WHM_CSI_SOCKET_PATH);
    return -1;
}

bool s_sensingEnabledOnOtherRadios(T_Radio* pRad) {
    T_Radio* pOtherRad;
    wld_for_eachRad(pOtherRad) {
        if((swl_str_matches(pRad->Name, pOtherRad->Name)) ||
           (pOtherRad->operatingFrequencyBand == pRad->operatingFrequencyBand)) {
            continue;
        }
        if(pOtherRad->csiEnable) {
            return true;
        } else {
            continue;
        }
    }
    return false;
}

static void s_enableExistingCsiClients(T_Radio* pRad, bool enable) {
    amxc_llist_for_each(it, &pRad->csiClientList) {
        wld_csiClient_t* client = amxc_llist_it_get_data(it, wld_csiClient_t, it);
        s_setCsiAutoRate(pRad, client, enable);
    }
}

swl_rc_ne whm_mxl_rad_sensingCmd(T_Radio* pRad) {
    ASSERT_NOT_NULL(pRad, SWL_RC_INVALID_PARAM, ME, "NULL");

    if(pRad->csiEnable) {
        // Create Unix socket to send csi stats
        if(conn.serverfd < 0) {
            conn.serverfd = s_createStatsSocket();
            if(conn.serverfd < 0) {
                SAH_TRACEZ_ERROR(ME, "Create Unix socket failed");
            }
        }
        // Restart all existing csi clients on this radio
        s_enableExistingCsiClients(pRad, true);
    } else {
        // Stop all existing csi clients on this radio
        s_enableExistingCsiClients(pRad, false);
        // Close and clean Unix socket when csi is disabled for all radios
        if(conn.serverfd != -1) {
            if(!s_sensingEnabledOnOtherRadios(pRad)) {
                if(conn.clientfd != -1) {
                    close(conn.clientfd);
                    conn.clientfd = -1;
                }
                close(conn.serverfd);
                conn.serverfd = -1;
                unlink(CONFIG_MOD_WHM_CSI_SOCKET_PATH);
            }
        }
    }

    return SWL_RC_OK;
}

amxd_status_t _whm_mxl_csi_getCsiSocketStatus(amxd_object_t* object,
                                              amxd_function_t* func _UNUSED,
                                              amxc_var_t* args _UNUSED,
                                              amxc_var_t* retval) {
    amxd_object_t* pRad = amxd_object_get_parent(object);
    ASSERT_NOT_NULL(pRad, amxd_status_unknown_error, ME, "NULL");

    amxc_var_set_type(retval, AMXC_VAR_ID_HTABLE);
    amxc_var_add_key(bool, retval, "Active", (conn.serverfd < 0) ? false : true);
    amxc_var_add_key(cstring_t, retval, "SocketPath", CONFIG_MOD_WHM_CSI_SOCKET_PATH);
    return amxd_status_ok;
}
