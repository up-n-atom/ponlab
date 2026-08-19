/******************************************************************************

         Copyright (c) 2023 - 2025 MaxLinear, Inc.

  This software may be distributed under the terms of the BSD license.
  See README for more details.

*******************************************************************************/
#ifndef __WHM_MXL_CSI_H__
#define __WHM_MXL_CSI_H__

#include "wld/wld.h"
#include "whm_mxl_utils.h"

#define MXL_CSI_MIN_SRATE 10
#define MXL_CSI_MAX_SRATE 30

typedef struct {
    uint16_t saFamily;
    uint8_t staMac[ETHER_ADDR_LEN];
    uint8_t assocApMac[ETHER_ADDR_LEN];
    uint8_t enable;
    uint8_t rate;
} whm_mxl_csiAutoRate_t;

typedef struct {
    uint64_t csiSendQosNullCount;
    uint64_t csiRecvFrameCount;
    uint64_t csiSendNlCsiData;
    uint64_t csiReqInfoCount;
} whm_mxl_csiCounters_t;

typedef struct {
    int serverfd;
    int clientfd;
} whm_mxl_connection_t;

swl_rc_ne whm_mxl_rad_sensingCmd(T_Radio* pRad);
swl_rc_ne whm_mxl_rad_sensingAddClient(T_Radio* pRad, wld_csiClient_t* client);
swl_rc_ne whm_mxl_rad_sensingDelClient(T_Radio* pRad, swl_macChar_t macAddr);
swl_rc_ne whm_mxl_rad_sensingCsiStats(T_Radio* pRad, wld_csiState_t* csimonState);
swl_rc_ne whm_mxl_rad_sensingResetStats(T_Radio* pRad);
void mxl_rad_sendCsiStatsOverUnixSocket(wifi_csi_driver_nl_event_data_t* stats);

#endif /* __WHM_MXL_CSI_H__ */
