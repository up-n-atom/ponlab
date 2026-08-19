/******************************************************************************

         Copyright (c) 2024, MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/*
 * $Id$
 *
 *
 *
 * Core Upper Mac Interface
 *
 */
#include "mtlkinc.h"

#include "core_priv.h"
#include "mtlk_df.h"
#include "mtlk_coreui.h"
#include "core.h"
#include "core_config.h"
#include "core_stats.h"
#include "hw_mmb.h"
#include "mtlkhal.h"
#include "bitrate.h"
#include "mtlk_gpl_helper.h"
#include "mtlkaux.h"
#include "mtlk_param_db.h"
#include "mtlk_df_nbuf.h"
#include "wave_radio.h"
#include "mhi_umi.h"
#include "core_pdb_def.h"
#include "mtlk_rtlog.h"

#define LOG_LOCAL_GID   GID_CORE
#define LOG_LOCAL_FID   7

#define MTLK_CORE_WIDAN_UNCONNECTED_STATION_RATE     140
#define EHT_MCS_MAP_BW_LE_80MHZ_IDX   4

mtlk_error_t __MTLK_IFUNC
wave_check_mac_watchdog (mtlk_handle_t core_object, const void *payload, uint32 size)
{
  struct nic *nic = HANDLE_T_PTR(struct nic, core_object);
  mtlk_txmm_msg_t man_msg;
  mtlk_txmm_data_t *man_entry;
  UMI_MAC_WATCHDOG *mac_watchdog;
  mtlk_error_t res = MTLK_ERR_OK;
  wave_radio_t *radio = wave_vap_radio_get(nic->vap_handle);

  MTLK_ASSERT(0 == size);
  MTLK_UNREFERENCED_PARAM(payload);
  MTLK_UNREFERENCED_PARAM(size);
  MTLK_ASSERT(FALSE == mtlk_vap_is_slave_ap(nic->vap_handle));

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txdm(nic->vap_handle), NULL);
  if (!man_entry) {
    res = MTLK_ERR_NO_RESOURCES;
    goto END;
  }

  man_entry->id = UM_DBG_MAC_WATCHDOG_REQ;
  man_entry->payload_size = sizeof(UMI_MAC_WATCHDOG);

  mac_watchdog = (UMI_MAC_WATCHDOG *)man_entry->payload;
  mac_watchdog->u16Timeout =
      HOST_TO_MAC16(WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_MAC_WATCHDOG_TIMER_TIMEOUT_MS));

  res = mtlk_txmm_msg_send_blocked(&man_msg,
          WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_MAC_WATCHDOG_TIMER_TIMEOUT_MS));
  if (res == MTLK_ERR_OK) {
    switch (mac_watchdog->u8Status) {
    case UMI_OK:
      break;
    case UMI_MC_BUSY:
      break;
    case UMI_TIMEOUT:
      res = MTLK_ERR_UMI;
      break;
    default:
      res = MTLK_ERR_UNKNOWN;
      break;
    }
  }
  mtlk_txmm_msg_cleanup(&man_msg);

END:
  if (res != MTLK_ERR_OK) {
    ELOG_DD("CID-%04x: MAC watchdog error %d", mtlk_vap_get_oid(nic->vap_handle), res);
    (void)mtlk_hw_set_prop(mtlk_vap_get_hwapi(nic->vap_handle), MTLK_HW_RESET, NULL, 0);
  } else {
    if (MTLK_ERR_OK !=
        mtlk_osal_timer_set(&nic->slow_ctx->mac_watchdog_timer,
                            WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_MAC_WATCHDOG_TIMER_PERIOD_MS))) {
      ELOG_D("CID-%04x: Cannot schedule MAC watchdog timer", mtlk_vap_get_oid(nic->vap_handle));
      (void)mtlk_hw_set_prop(mtlk_vap_get_hwapi(nic->vap_handle), MTLK_HW_RESET, NULL, 0);
    }
  }

  return MTLK_ERR_OK;
}

#ifdef CONFIG_WAVE_DEBUG

mtlk_error_t __MTLK_IFUNC
mtlk_core_bcl_mac_data_get (mtlk_handle_t hcore, const void* data, uint32 data_size)
{
  mtlk_error_t       res = MTLK_ERR_OK;
  mtlk_txmm_msg_t    man_msg;
  mtlk_txmm_data_t  *man_entry;
  UMI_BCL_REQUEST   *preq;
  mtlk_core_t       *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t       *clpb = *(mtlk_clpb_t **) data;
  int                exception;
  BOOL               f_bswap_data = TRUE;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);

  /* Get BCL request from CLPB */
  preq = mtlk_clpb_enum_get_next(clpb, NULL);
  if (NULL == preq) {
    ELOG_SD("Failed to get data from clipboard in function %s, line %d", __FUNCTION__, __LINE__);
    return MTLK_ERR_UNKNOWN;
  }

  /* Check MAC state */
  exception = (mtlk_core_is_hw_halted(core) &&
               !core->slow_ctx->mac_stuck_detected_by_sw);

  /* if Core got here preq->Unit field wiath value greater or equal to BCL_UNIT_MAX -
   * the Core should not convert result data words in host format. */
  if (preq->Unit >= BCL_UNIT_MAX) {
    preq->Unit -= BCL_UNIT_MAX; /*Restore original field value*/
    f_bswap_data = FALSE;
  }

  ILOG4_SDDDD("Getting BCL over %s unit(%d) address(0x%x) size(%u) (%x)",
       exception ? "io" : "txmm",
       (int)preq->Unit,
       (unsigned int)preq->Address,
       (unsigned int)preq->Size,
       (unsigned int)preq->Data[0]);

  if (exception) {
    /* MAC is halted - send BCL request through IO */
    mtlk_core_bswap_bcl_request(preq, TRUE);

    res = mtlk_hw_get_prop(mtlk_vap_get_hwapi(core->vap_handle), MTLK_HW_BCL_ON_EXCEPTION, preq, sizeof(*preq));
    if (MTLK_ERR_OK != res) {
      ELOG_D("CID-%04x: Can't get BCL", mtlk_vap_get_oid(core->vap_handle));
      goto err_push;
    }
  } else {
    /* MAC is in normal state - send BCL request through TXMM */
    man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(core->vap_handle), NULL);
    if (!man_entry) {
      ELOG_D("CID-%04x: Can't send Get BCL request to MAC due to the lack of MAN_MSG", mtlk_vap_get_oid(core->vap_handle));
      res = MTLK_ERR_NO_RESOURCES;
      goto err_push;
    }

    mtlk_core_bswap_bcl_request(preq, TRUE);

    *((UMI_BCL_REQUEST*)man_entry->payload) = *preq;
    man_entry->id           = UM_MAN_QUERY_BCL_VALUE;
    man_entry->payload_size = sizeof(*preq);

    res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
    if (MTLK_ERR_OK != res) {
      ELOG_D("CID-%04x: Can't send Get BCL request to MAC, timed-out", mtlk_vap_get_oid(core->vap_handle));
      mtlk_txmm_msg_cleanup(&man_msg);
      goto err_push;
    }

    /* Copy back results */
    *preq = *((UMI_BCL_REQUEST*)man_entry->payload);
    mtlk_txmm_msg_cleanup(&man_msg);
  }

  /* Send back results */
  mtlk_core_bswap_bcl_request(preq, !f_bswap_data);
  mtlk_dump(3, preq, sizeof(*preq), "dump of the UM_MAN_QUERY_BCL_VALUE");

  res = mtlk_clpb_push(clpb, preq, sizeof(*preq));
  if (MTLK_ERR_OK == res) {
    return res;
  }

err_push:
  mtlk_clpb_purge(clpb);
  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_bcl_mac_data_set (mtlk_handle_t hcore, const void* data, uint32 data_size)
{
  mtlk_error_t       res = MTLK_ERR_OK;
  mtlk_txmm_msg_t    man_msg;
  mtlk_txmm_data_t  *man_entry = NULL;
  mtlk_core_t       *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t       *clpb = *(mtlk_clpb_t **) data;
  UMI_BCL_REQUEST   *preq = NULL;
  int                exception;
  uint32             req_size;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);

  /* Read Set BCL request from CLPB */
  preq = mtlk_clpb_enum_get_next(clpb, &req_size);
  MTLK_CLPB_TRY(preq, req_size)
    /* Check MAC state */
    exception = (mtlk_core_is_hw_halted(core) &&
                 !core->slow_ctx->mac_stuck_detected_by_sw);

    ILOG2_SDDDD("Setting BCL over %s unit(%d) address(0x%x) size(%u) (%x)",
         exception ? "io" : "txmm",
         (int)preq->Unit,
         (unsigned int)preq->Address,
         (unsigned int)preq->Size,
         (unsigned int)preq->Data[0]);

    mtlk_dump(3, preq, sizeof(*preq), "dump of the UM_MAN_SET_BCL_VALUE");

    if (exception) {
      /* MAC is halted - send BCL request through IO */
      mtlk_core_bswap_bcl_request(preq, FALSE);

      res = mtlk_hw_set_prop(mtlk_vap_get_hwapi(core->vap_handle), MTLK_HW_BCL_ON_EXCEPTION, preq, sizeof(*preq));
      if (MTLK_ERR_OK != res) {
        ELOG_D("CID-%04x: Can't set BCL", mtlk_vap_get_oid(core->vap_handle));
        MTLK_CLPB_EXIT(res);
      }
    } else {
      /* MAC is in normal state - send BCL request through TXMM */
       man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(core->vap_handle), NULL);
       if (!man_entry) {
         ELOG_D("CID-%04x: Can't send Set BCL request to MAC due to the lack of MAN_MSG", mtlk_vap_get_oid(core->vap_handle));
         MTLK_CLPB_EXIT(MTLK_ERR_NO_RESOURCES);
       }

       mtlk_core_bswap_bcl_request(preq, FALSE);

       *((UMI_BCL_REQUEST*)man_entry->payload) = *preq;
       man_entry->id           = UM_MAN_SET_BCL_VALUE;
       man_entry->payload_size = sizeof(*preq);

       res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
       if (MTLK_ERR_OK != res) {
         ELOG_D("CID-%04x: Can't send Set BCL request to MAC, timed-out", mtlk_vap_get_oid(core->vap_handle));
       }

       mtlk_txmm_msg_cleanup(&man_msg);
    }
  MTLK_CLPB_FINALLY(res)
    return res;
  MTLK_CLPB_END
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_simple_cli (mtlk_handle_t hcore, const void* data, uint32 data_size)
{
  mtlk_error_t         res = MTLK_ERR_OK;
  mtlk_txmm_msg_t      man_msg;
  mtlk_txmm_data_t    *man_entry = NULL;
  UMI_DBG_CLI_REQ     *mac_msg;
  mtlk_dbg_cli_cfg_t  *UmiDbgCliReq;

  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  uint32 clpb_data_size;
  mtlk_dbg_cli_cfg_t* clpb_data;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);

  clpb_data = mtlk_clpb_enum_get_next(clpb, &clpb_data_size);
  MTLK_CLPB_TRY(clpb_data, clpb_data_size)
    UmiDbgCliReq = clpb_data;

    ILOG2_DDDDD("Simple CLI: Action %d, data1 %d, data2 %d, data3 %d, numOfArgs %d",
      UmiDbgCliReq->DbgCliReq.action, UmiDbgCliReq->DbgCliReq.data1, UmiDbgCliReq->DbgCliReq.data2,
      UmiDbgCliReq->DbgCliReq.data3, UmiDbgCliReq->DbgCliReq.numOfArgumets);

    man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txdm(core->vap_handle), NULL);
    if (!man_entry) {
      ELOG_D("CID-%04x: Can not get TXMM slot", mtlk_vap_get_oid(core->vap_handle));
      MTLK_CLPB_EXIT(MTLK_ERR_NO_RESOURCES);
    }

    man_entry->id = UM_DBG_CLI_REQ;
    man_entry->payload_size = sizeof(UMI_DBG_CLI_REQ);
    mac_msg = (UMI_DBG_CLI_REQ *)man_entry->payload;
    mac_msg->action         = HOST_TO_MAC32(UmiDbgCliReq->DbgCliReq.action);
    mac_msg->numOfArgumets = HOST_TO_MAC32(UmiDbgCliReq->DbgCliReq.numOfArgumets);
    mac_msg->data1          = HOST_TO_MAC32(UmiDbgCliReq->DbgCliReq.data1);
    mac_msg->data2          = HOST_TO_MAC32(UmiDbgCliReq->DbgCliReq.data2);
    mac_msg->data3          = HOST_TO_MAC32(UmiDbgCliReq->DbgCliReq.data3);

    res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
    mtlk_txmm_msg_cleanup(&man_msg);

    if (MTLK_ERR_OK != res || UMI_OK != mac_msg->Status) {
      ELOG_DDD("CID-%04x: DBG_CLI failed, res=%d status=%hhu",
              mtlk_vap_get_oid(core->vap_handle), res, mac_msg->Status);
      if (UMI_OK != mac_msg->Status)
        res = MTLK_ERR_MAC;
    }
  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res(clpb, res);
  MTLK_CLPB_END
}

#endif /* CONFIG_WAVE_DEBUG */

mtlk_error_t __MTLK_IFUNC
mtlk_core_fw_assert (mtlk_core_t *core, UMI_FW_DBG_REQ *req_msg)
{
  mtlk_error_t       res = MTLK_ERR_OK;
  mtlk_txmm_msg_t    man_msg;
  mtlk_txmm_data_t  *man_entry = NULL;
  UMI_FW_DBG_REQ    *mac_msg;

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txdm(core->vap_handle), NULL);
  if (!man_entry) {
    ELOG_D("CID-%04x: Can not get TXMM slot", mtlk_vap_get_oid(core->vap_handle));
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_DBG_FW_DBG_REQ;
  man_entry->payload_size = sizeof(UMI_FW_DBG_REQ);
  mac_msg = (UMI_FW_DBG_REQ *)man_entry->payload;

  MTLK_STATIC_ASSERT(sizeof(mac_msg->debugType) == sizeof(uint8));
  mac_msg->debugType = req_msg->debugType;

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  mtlk_txmm_msg_cleanup(&man_msg);

  if (MTLK_ERR_OK != res || UMI_OK != mac_msg->Status) {
    ELOG_DDD("CID-%04x: FW Debug message failed, res=%d status=%hhu",
            mtlk_vap_get_oid(core->vap_handle),
            res, mac_msg->Status);
    if (UMI_OK != mac_msg->Status)
      res = MTLK_ERR_MAC;
  }

  return res;
}

/* ===============================================================================
 * This function processes UM_BSS_SET_BSS_REQ FW request
 * ===============================================================================
 * If we are in master serializer context we need to prevent set chan for the master core
 * (otherwize there will be a dead-lock) but still the actual FW SET_BSS message should be
 * sent for the relevant core.
 */
mtlk_error_t __MTLK_IFUNC
mtlk_core_set_bss (mtlk_core_t *core, mtlk_core_t *context_core, UMI_SET_BSS *fw_params)
{
#define BCAST_TWT_RESPONDER_SUPPORTED 1
    mtlk_error_t       res = MTLK_ERR_OK;
    mtlk_txmm_msg_t    man_msg;
    mtlk_txmm_data_t  *man_entry;
    UMI_SET_BSS       *req;
    uint8 vap_id = mtlk_vap_get_id(core->vap_handle);
    mtlk_pdb_t *param_db_core = mtlk_vap_get_param_db(core->vap_handle);
    int twt_operation_mode = wave_pdb_get_int(param_db_core, PARAM_DB_CORE_TWT_OPERATION_MODE);
    int bcast_twt_operation_mode = !BCAST_TWT_RESPONDER_SUPPORTED;
    mtlk_hw_band_e band = wave_radio_band_get(wave_vap_radio_get(core->vap_handle));
    mtlk_hw_t *hw = mtlk_vap_get_hw(core->vap_handle);
    UMI_SET_BSS old_set_bss;
    mtlk_pdb_size_t set_bss_size = sizeof(UMI_SET_BSS);
    uint8 old_bss_color, req_bss_color;
    int ret;
    MTLK_ASSERT(fw_params);
    MTLK_ASSERT(TRUE >= twt_operation_mode);

    res = MTLK_CORE_PDB_GET_BINARY(core, PARAM_DB_CORE_SET_BSS, &old_set_bss, &set_bss_size);
    if (res != MTLK_ERR_OK) {
      ELOG_D("CID-%04x: Can't read SET_BSS data", mtlk_vap_get_oid(core->vap_handle));
      return res;
    }

    if (mtlk_hw_type_is_gen6_d2_or_gen7(hw)) {
      if (_wave_beacon_get_bcast_twt_support(core))
        bcast_twt_operation_mode = BCAST_TWT_RESPONDER_SUPPORTED;
      else
        ILOG1_V("Broadcast TWT support not set for WAV600D2/WAV700!");
    }

    /* prepare msg for the FW */
    if (!(man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(core->vap_handle), &res))) {
      ELOG_DD("CID-%04x: UM_BSS_SET_BSS_REQ init failed, err=%i",
             mtlk_vap_get_oid(core->vap_handle), res);
      return MTLK_ERR_NO_RESOURCES;
    }
    man_entry->id = UM_BSS_SET_BSS_REQ;
    man_entry->payload_size = sizeof(*req);
    req = (UMI_SET_BSS *) man_entry->payload;
    /* copy input structure and setup vapId */
    *req = *fw_params;
    req->vapId = vap_id;
    req->twtOperationMode = twt_operation_mode;
    req->twtBroadcastOperationMode = bcast_twt_operation_mode;
    req->fixedMcsVapManagement = (uint8)wave_core_management_frames_rate_select(core);
    req->beaconPhyMode = (uint8)mtlk_core_get_6ghz_beacon_format(band, param_db_core);
    req->beacon6GBandwidth = (uint8)mtlk_core_get_6ghz_duplicate_beacon_bw(band, param_db_core);

    /* Don't send SET_BSS request if nothing has changed */
    /* Ignore BSS Color changing because it is already processed */
    req_bss_color = req->u8HE_Bss_Color_info;
    old_bss_color = old_set_bss.u8HE_Bss_Color_info;
    req->u8HE_Bss_Color_info = 0; /* clear before comparison */
    old_set_bss.u8HE_Bss_Color_info = 0;
    ret = memcmp(&old_set_bss, req, sizeof(UMI_SET_BSS));
    req->u8HE_Bss_Color_info = req_bss_color; /* restore */
    old_set_bss.u8HE_Bss_Color_info = old_bss_color;
    if (!ret) {
      ILOG1_D("CID-%04x: Don't send SET_BSS request because nothing significant has changed", mtlk_vap_get_oid(core->vap_handle));
      if (old_bss_color != req_bss_color) { /* Update param DB if BSS Color was changed */
        if (MTLK_ERR_OK != MTLK_CORE_PDB_SET_BINARY(core, PARAM_DB_CORE_SET_BSS, req, set_bss_size))
          ELOG_D("CID-%04x: Can't write SET_BSS data", mtlk_vap_get_oid(core->vap_handle));
      }
      goto cleanup; /* MTLK_ERR_OK */
    }
    /* Only the 3 parameters below can be changed on the fly, i.e. when vap_in_fw_is_active is set */
    if (core->vap_in_fw_is_active &&
        (old_set_bss.protectionMode   == req->protectionMode) &&
        (old_set_bss.slotTime         == req->slotTime) &&
        (old_set_bss.useShortPreamble == req->useShortPreamble))
    {
      ELOG_D("CID-%04x: Illegal changes on the fly -> ignored", mtlk_vap_get_oid(core->vap_handle));
      mtlk_dump(0, &old_set_bss, sizeof(UMI_SET_BSS), "old SET_BSS:");
      mtlk_dump(0, req, sizeof(UMI_SET_BSS), "new SET_BSS:");
      res = MTLK_ERR_PARAMS;
      goto cleanup;
    }

    if (MTLK_ERR_OK != MTLK_CORE_PDB_SET_BINARY(core, PARAM_DB_CORE_SET_BSS, req, set_bss_size))
      ELOG_D("CID-%04x: Can't write SET_BSS data", mtlk_vap_get_oid(core->vap_handle));

    ILOG1_DDD("CID-%04x: Band = %d, UMI_SET_BSS->beaconPhyMode = %u", mtlk_vap_get_oid(core->vap_handle), band, req->beaconPhyMode);
    ILOG1_DDD("CID-%04x: Band = %d, UMI_SET_BSS->beacon6GBandwidth = %u", mtlk_vap_get_oid(core->vap_handle), band, req->beacon6GBandwidth);
    ILOG2_DD("CID-%04x: UMI_SET_BSS->u8HE_Bss_Color_info:0x%x", mtlk_vap_get_oid(core->vap_handle), req->u8HE_Bss_Color_info);
    ILOG1_DD("CID-%04x: UMI_SET_BSS->twtBroadcastOperationMode:%d", mtlk_vap_get_oid(core->vap_handle), req->twtBroadcastOperationMode);
    finish_and_prevent_fw_set_chan(context_core, TRUE); /* set_bss is a "process", so need this; core is the core on which we're running */
    mtlk_dump(2, req, sizeof(*req), "dump of UMI_SET_BSS:");

    /* send message to FW */
    res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
    if (res != MTLK_ERR_OK || UMI_OK != req->Status) {
      ELOG_DDD("CID-%04x: UM_BSS_SET_BSS_REQ send failed, res=%i status=%hhu",
              mtlk_vap_get_oid(core->vap_handle), res, req->Status);
      if (UMI_OK != req->Status)
        res = MTLK_ERR_MAC;
    } else if (mtlk_core_get_net_state(core) != NET_STATE_CONNECTED
             && MTLK_ERR_OK != (res = mtlk_core_set_net_state(core, NET_STATE_CONNECTED))) {
      ELOG_D("CID-%04x: Failed to switch core to state CONNECTED", mtlk_vap_get_oid(core->vap_handle));
    }

    finish_and_prevent_fw_set_chan(context_core, FALSE);

cleanup:
    if (NULL != man_entry)
      mtlk_txmm_msg_cleanup(&man_msg);

    return res;
#undef BCAST_TWT_RESPONDER_SUPPORTED
}

mtlk_error_t __MTLK_IFUNC
wave_core_ap_stop_traffic (struct nic *nic, uint16 sid, IEEE_ADDR *sta_addr)
{
  mtlk_error_t       res = MTLK_ERR_OK;
  mtlk_txmm_msg_t    man_msg;
  mtlk_txmm_data_t  *man_entry = NULL;
  UMI_STOP_TRAFFIC  *psUmiStopTraffic;

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(nic->vap_handle), &res);
  if (!man_entry) {
    ELOG_D("CID-%04x: Can't send STOP_TRAFFIC request to MAC: no MAN_MSG", mtlk_vap_get_oid(nic->vap_handle));
    return MTLK_ERR_NO_MEM;
  }

  man_entry->id           = UM_MAN_STOP_TRAFFIC_REQ;
  man_entry->payload_size = sizeof(UMI_STOP_TRAFFIC);
  psUmiStopTraffic = (UMI_STOP_TRAFFIC *) man_entry->payload;
  psUmiStopTraffic->u8Status = UMI_OK;
  psUmiStopTraffic->u16SID = HOST_TO_MAC16(sid);

  mtlk_dump(2, psUmiStopTraffic, sizeof(UMI_STOP_TRAFFIC), "dump of UMI_STOP_TRAFFIC:");

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (res != MTLK_ERR_OK) {
    ELOG_DD("CID-%04x: Can't send UM_MAN_STOP_TRAFFIC_REQ request to MAC (err=%d)",
            mtlk_vap_get_oid(nic->vap_handle), res);
    goto finish;
  }
  if (psUmiStopTraffic->u8Status != UMI_OK) {
    WLOG_DYD("CID-%04x: Station %Y remove failed in FW (status=%u)",
             mtlk_vap_get_oid(nic->vap_handle), sta_addr, psUmiStopTraffic->u8Status);
    res = MTLK_ERR_MAC;
    goto finish;
  }

  ILOG1_DYD("CID-%04x: Station %Y traffic stopped (SID = %u)",
            mtlk_vap_get_oid(nic->vap_handle), sta_addr, sid);

finish:
  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

mtlk_error_t __MTLK_IFUNC
wave_core_ap_remove_sta (struct nic *nic, uint16 sid, IEEE_ADDR *sta_addr)
{
  mtlk_error_t       res = MTLK_ERR_OK;
  mtlk_txmm_msg_t    man_msg;
  mtlk_txmm_data_t  *man_entry = NULL;
  UMI_STA_REMOVE    *psUmiStaRemove;
  mtlk_vap_handle_t  vap_handle = nic->vap_handle;
#ifdef MTLK_WAVE_700  
  wave_radio_t      *radio;
  mtlk_hw_band_e     band;
  uint16             num_of_high_speed_sta;
  sta_entry         *src_sta;
  mtlk_hw_t         *hw;
  struct ieee80211_sta *mac80211_sta;
#endif

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(vap_handle), &res);
  if (man_entry == NULL) {
    ELOG_D("CID-%04x: Can't send STA_REMOVE request to MAC: no MAN_MSG", mtlk_vap_get_oid(vap_handle));
    return MTLK_ERR_NO_MEM;
  }

  man_entry->id           = UM_MAN_STA_REMOVE_REQ;
  man_entry->payload_size = sizeof(UMI_STA_REMOVE);
  psUmiStaRemove = (UMI_STA_REMOVE *)man_entry->payload;
  psUmiStaRemove->u8Status = UMI_OK;
  psUmiStaRemove->u16SID = HOST_TO_MAC16(sid);
  mtlk_dump(2, psUmiStaRemove, sizeof(UMI_STA_REMOVE), "dump of UMI_STA_REMOVE:");
  ILOG1_DY("UMI_STA_REMOVE->u16SID: %u, %Y", MAC_TO_HOST16(psUmiStaRemove->u16SID), sta_addr);
  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (res != MTLK_ERR_OK) {
    ELOG_DD("CID-%04x: Can't send UM_MAN_STA_REMOVE_REQ request to MAC (err=%d)",
            mtlk_vap_get_oid(vap_handle), res);
    goto finish;
  }
  if (psUmiStaRemove->u8Status != UMI_OK) {
    WLOG_DYD("CID-%04x: Station %Y remove failed in FW (status=%u)",
             mtlk_vap_get_oid(vap_handle), sta_addr, psUmiStaRemove->u8Status);
    res = MTLK_ERR_MAC;
    goto finish;
  }

  ILOG0_DYD("CID-%04x: Station %Y disconnected (SID = %u)",
            mtlk_vap_get_oid(vap_handle), sta_addr, sid);

#ifdef MTLK_WAVE_700
  radio = wave_vap_radio_get(nic->vap_handle);
  if (wave_radio_is_gen7(radio)) {
    src_sta = mtlk_stadb_find_sid(&nic->slow_ctx->stadb, sid);
    if (src_sta) {
      mac80211_sta = wv_sta_entry_get_mac80211_sta(src_sta);
      band = wave_radio_band_get(radio);
      if ((band == MTLK_HW_BAND_6_GHZ) && (wave_wv_mac80211_sta_is_high_speed_supported(mac80211_sta))) {
         num_of_high_speed_sta = wave_radio_dec_high_speed_counter(radio);
         if ((num_of_high_speed_sta == 0) && (wave_radio_get_rx_ant(radio) >= 4)) { /* re-enable PCI ASPM since no more "high speed STA" */
            hw = mtlk_vap_get_hw(nic->vap_handle);
            MTLK_ASSERT(hw);
            mtlk_hw_pci_enable_aspm(hw);
            ILOG0_V("Re-enable PCIE ASPM because of no more high speed STA is connected");
         }
      }
      mtlk_sta_decref (src_sta); /* De-reference of find */
    }
  }
#endif

finish:
  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_send_stop_vap_traffic (struct nic *nic)
{
  mtlk_error_t          result = MTLK_ERR_OK;
  mtlk_txmm_msg_t       man_msg;
  mtlk_txmm_data_t     *man_entry = NULL;
  UMI_STOP_VAP_TRAFFIC *req;
  int net_state = mtlk_core_get_net_state(nic);
  unsigned vap_id = mtlk_vap_get_id(nic->vap_handle);

  ILOG0_DD("STOP_VAP_TRAFFIC VapID=%u, net_state=%i", vap_id, net_state);

  if (net_state == NET_STATE_HALTED) {
    /* Do not send anything to halted MAC or if STA hasn't been connected */
    wave_clean_all_sta_on_disconnect(nic);
    return MTLK_ERR_OK;
  }

  /* we switch the states beforehand because this does some checks; we'll fix the state if we fail */
  if ((result = mtlk_core_set_net_state(nic, NET_STATE_DEACTIVATING)) != MTLK_ERR_OK) {
    goto FINISH;
  }

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(nic->vap_handle), &result);
  if (man_entry == NULL) {
    ELOG_D("CID-%04x: Can't init STOP_VAP_TRAFFIC request due to the lack of MAN_MSG",
           mtlk_vap_get_oid(nic->vap_handle));
    goto FINISH;
  }

  core_ap_disconnect_all(nic);

  man_entry->id = UM_MAN_STOP_VAP_TRAFFIC_REQ;
  man_entry->payload_size = sizeof(*req);
  req = (UMI_STOP_VAP_TRAFFIC *) man_entry->payload;

  req->u16Status = HOST_TO_MAC16(UMI_OK);
  req->vapId = vap_id;
  mtlk_dump(2, req, sizeof(*req), "dump of UMI_STOP_VAP_TRAFFIC:");

  result = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (result != MTLK_ERR_OK) {
    ELOG_DD("CID-%04x: Can't send STOP_VAP_TRAFFIC request to VAP (err=%d)",
            mtlk_vap_get_oid(nic->vap_handle), result);
    mtlk_core_set_net_state(nic, net_state); /* restore previous net_state if we can */
    goto FINISH;
  }

  if (MAC_TO_HOST16(req->u16Status) != UMI_OK) {
    WLOG_DD("CID-%04x: STOP_VAP_TRAFFIC failed in FW (status=%u)", mtlk_vap_get_oid(nic->vap_handle),
      MAC_TO_HOST16(req->u16Status));
    result = MTLK_ERR_MAC;
    mtlk_core_set_net_state(nic, net_state); /* restore previous net_state if we can */
    goto FINISH;
  }

FINISH:
  if (man_entry)
    mtlk_txmm_msg_cleanup(&man_msg);

  return result;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_send_vap_remove (struct nic *nic)
{
  mtlk_error_t       result = MTLK_ERR_OK;
  mtlk_txmm_msg_t    man_msg;
  mtlk_txmm_data_t  *man_entry = NULL;
  UMI_REMOVE_VAP    *req;
  IEEE_ADDR          mac_addr;
  int net_state = mtlk_core_get_net_state(nic);
  uint8 vap_id = mtlk_vap_get_id(nic->vap_handle);

  if (net_state == NET_STATE_HALTED) {
    /* Do not send anything to halted MAC or if STA hasn't been connected */
    wave_clean_all_sta_on_disconnect(nic);
    return MTLK_ERR_OK;
  }

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(nic->vap_handle), &result);
  if (man_entry == NULL) {
    ELOG_D("CID-%04x: Can't send REMOVE_VAP request to MAC due to the lack of MAN_MSG",
           mtlk_vap_get_oid(nic->vap_handle));
    goto FINISH;
  }

  /* we switch the states beforehand because this does some checks; we'll fix the state if we fail */
  if ((result = mtlk_core_set_net_state(nic, NET_STATE_READY)) != MTLK_ERR_OK) {
    goto FINISH;
  }

  man_entry->id           = UM_MAN_REMOVE_VAP_REQ;
  /* the structs are identical so we could have not bothered with this... */
  man_entry->payload_size = sizeof(*req);
  req = (UMI_REMOVE_VAP *) man_entry->payload;
  req->u16Status = HOST_TO_MAC16(UMI_OK);
  req->vapId = vap_id;
  mtlk_dump(2, req, sizeof(*req), "dump of UMI_REMOVE_VAP:");

  result = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (result != MTLK_ERR_OK) {
    ELOG_DD("CID-%04x: Can't send REMOVE_VAP request to VAP (err=%d)",
            mtlk_vap_get_oid(nic->vap_handle), result);
    mtlk_core_set_net_state(nic, net_state); /* restore previous net_state if we can */
    goto FINISH;
  }

  if (MAC_TO_HOST16(req->u16Status) != UMI_OK) {
    WLOG_DD("CID-%04x: REMOVE_VAP failed in FW (status=%u)", mtlk_vap_get_oid(nic->vap_handle),
      MAC_TO_HOST16(req->u16Status));
    result = MTLK_ERR_MAC;
    mtlk_core_set_net_state(nic, net_state); /* restore previous net_state if we can */
    goto FINISH;
  }

  /* update disconnections statistics */
  nic->pstats.num_disconnects++;

  MTLK_CORE_PDB_GET_MAC(nic, PARAM_DB_CORE_MAC_ADDR, &mac_addr);
  ILOG0_DDYD("CID-%04x: VapID %u at %Y disconnected (status %u)", mtlk_vap_get_oid(nic->vap_handle),
              vap_id, mac_addr.au8Addr, MAC_TO_HOST16(req->u16Status));

FINISH:
  if (man_entry)
    mtlk_txmm_msg_cleanup(&man_msg);

  return result;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_receive_tx_power_limit_offset (mtlk_core_t *core, uint8 *power_limit_offset)
{
  mtlk_error_t         res;
  mtlk_txmm_msg_t      man_msg;
  mtlk_txmm_data_t    *man_entry;
  UMI_TX_POWER_LIMIT  *mac_msg;

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(core->vap_handle), NULL);
  if (!man_entry) {
      ELOG_D("CID-%04x: Can not get TXMM slot", mtlk_vap_get_oid(core->vap_handle));
      return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_MAN_SET_POWER_LIMIT_REQ;
  man_entry->payload_size = sizeof(UMI_TX_POWER_LIMIT);
  mac_msg = (UMI_TX_POWER_LIMIT *)man_entry->payload;
  mac_msg->getSetOperation = API_GET_OPERATION;

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK == res && UMI_OK == mac_msg->Status) {
    *power_limit_offset = mac_msg->powerLimitOffset;
  } else {
    ELOG_DDD("CID-%04x: Receive UM_MAN_SET_POWER_LIMIT_REQ failed, res=%i status=%hhu",
            mtlk_vap_get_oid(core->vap_handle), res, mac_msg->Status);
    if (UMI_OK != mac_msg->Status)
      res = MTLK_ERR_MAC;
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

mtlk_error_t __MTLK_IFUNC
wave_core_receive_interfdet_threshold (mtlk_core_t *core, int8 *threshold)
{
  mtlk_error_t       res;
  mtlk_txmm_msg_t    man_msg;
  mtlk_txmm_data_t  *man_entry = NULL;
  UMI_INTERFERER_DETECTION_PARAMS *umi_interfdet = NULL;

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(core->vap_handle), NULL);
  if (!man_entry)
    return MTLK_ERR_NO_RESOURCES;

  man_entry->id = UM_MAN_SET_INTERFERER_DETECTION_PARAMS_REQ;
  man_entry->payload_size = sizeof(*umi_interfdet);
  umi_interfdet = (UMI_INTERFERER_DETECTION_PARAMS *)man_entry->payload;
  umi_interfdet->getSetOperation = API_GET_OPERATION;

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK == res && UMI_OK == umi_interfdet->Status) {
    *threshold = umi_interfdet->threshold;
    ILOG1_D("Threshold %d", *threshold);
  } else {
    ELOG_DDD("CID-%04x: Failed to receive UM_MAN_SET_INTERFERER_DETECTION_PARAMS_REQ, res=%i status=%hhu",
             mtlk_vap_get_oid(core->vap_handle), res, umi_interfdet->Status);
    if (UMI_OK != umi_interfdet->Status)
      res = MTLK_ERR_MAC;
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

/* Set peer AP beacon interval (relevant only for station role interface)
 * Should be called before UMI ADD STA for the peer AP */
mtlk_error_t __MTLK_IFUNC
mtlk_beacon_man_set_beacon_interval_by_params (mtlk_core_t *core, mtlk_beacon_interval_t *mtlk_beacon_interval)
{
  mtlk_error_t         res;
  mtlk_txmm_msg_t      man_msg;
  mtlk_txmm_data_t    *man_entry = NULL;
  UMI_BEACON_INTERVAL *pBeaconInterval = NULL;
  mtlk_vap_handle_t    vap_handle = core->vap_handle;

  MTLK_ASSERT(vap_handle);

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(vap_handle), NULL);
  if (NULL == man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(vap_handle));
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_MAN_SET_BEACON_INTERVAL_REQ;
  man_entry->payload_size = sizeof(UMI_BEACON_INTERVAL);

  pBeaconInterval = (UMI_BEACON_INTERVAL *)(man_entry->payload);
  pBeaconInterval->beaconInterval = HOST_TO_MAC32(mtlk_beacon_interval->beacon_interval);
  pBeaconInterval->vapID = HOST_TO_MAC32(mtlk_beacon_interval->vap_id);

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res || UMI_OK != pBeaconInterval->Status) {
    ELOG_DDD("CID-%04x: beacon interval set failure, res=%i status=%hhu", mtlk_vap_get_oid(vap_handle), res,
              pBeaconInterval->Status);
    if (UMI_OK != pBeaconInterval->Status)
      res = MTLK_ERR_MAC;
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_send_fixed_rate (mtlk_core_t *core, const mtlk_fixed_rate_cfg_t *fixed_rate_cfg)
{
    mtlk_error_t            res;
    mtlk_txmm_msg_t         man_msg;
    mtlk_txmm_data_t       *man_entry;
    UMI_FIXED_RATE_CONFIG  *mac_msg;
    unsigned                oid;

    MTLK_ASSERT(core != NULL);
    oid = mtlk_vap_get_oid(core->vap_handle);
    ILOG2_D("CID-%04x: Set FIXED RATE", oid);

    /* allocate a new message */
    man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(core->vap_handle), NULL);
    if (!man_entry) {
      ELOG_D("CID-%04x: Can not get TXMM slot", oid);
      return MTLK_ERR_NO_RESOURCES;
    }
   /* fill the message data */
    man_entry->id = UM_MAN_FIXED_RATE_CONFIG_REQ;
    man_entry->payload_size = sizeof(UMI_FIXED_RATE_CONFIG);
    mac_msg = (UMI_FIXED_RATE_CONFIG *)man_entry->payload;

    MTLK_STATIC_ASSERT(sizeof(mac_msg->stationIndex) == sizeof(uint16));
    mac_msg->stationIndex  = HOST_TO_MAC16(fixed_rate_cfg->params[MTLK_FIXED_RATE_CFG_SID]);
    mac_msg->isAutoRate    = fixed_rate_cfg->params[MTLK_FIXED_RATE_CFG_AUTO];
    mac_msg->bw            = fixed_rate_cfg->params[MTLK_FIXED_RATE_CFG_BW];
    mac_msg->phyMode       = fixed_rate_cfg->params[MTLK_FIXED_RATE_CFG_PHYM];
    mac_msg->nss           = fixed_rate_cfg->params[MTLK_FIXED_RATE_CFG_NSS];
    mac_msg->mcs           = fixed_rate_cfg->params[MTLK_FIXED_RATE_CFG_MCS];
    mac_msg->cpMode        = fixed_rate_cfg->params[MTLK_FIXED_RATE_CFG_SCP];
    mac_msg->dcm           = fixed_rate_cfg->params[MTLK_FIXED_RATE_CFG_DCM];
    mac_msg->heExtPartialBwData
                           = fixed_rate_cfg->params[MTLK_FIXED_RATE_CFG_HE_EXTPARTIALBWDATA];
    mac_msg->heExtPartialBwMng
                           = fixed_rate_cfg->params[MTLK_FIXED_RATE_CFG_HE_EXTPARTIALBWMNG];
    mac_msg->changeType
                           = fixed_rate_cfg->params[MTLK_FIXED_RATE_CFG_CHANGETYPE];

#ifdef MTLK_WAVE_700
   if (wave_radio_is_gen7(wave_vap_radio_get(core->vap_handle)))
     mtlk_dump(1, mac_msg, sizeof(UMI_FIXED_RATE_CONFIG), "dump of UMI_FIXED_RATE_CONFIG:");
#endif
    /* send the message to FW */
    res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
    if (res != MTLK_ERR_OK || UMI_OK != mac_msg->Status) {
      ELOG_DDD("CID-%04x: Set UM_MAN_FIXED_RATE_CONFIG_REQ request Failed, res=%d status=%hhu",
              oid, res, mac_msg->Status);
      if (UMI_OK != mac_msg->Status)
        res = MTLK_ERR_MAC;
    }

    /* cleanup the message */
    mtlk_txmm_msg_cleanup(&man_msg);

    return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_get_temperature_req (mtlk_core_t *core, uint32 *temperature, uint32 calibrate_mask)
{
  mtlk_error_t         res = MTLK_ERR_OK;
  mtlk_txmm_msg_t      man_msg;
  mtlk_txmm_data_t    *man_entry = NULL;
  UMI_HDK_USER_DEMAND *pTemperature = NULL;

  mtlk_card_type_info_t  card_type_info;

  mtlk_hw_get_prop(mtlk_vap_get_hwapi(core->vap_handle), MTLK_HW_PROP_CARD_TYPE_INFO,
                   &card_type_info, sizeof(card_type_info));

  if (!_mtlk_card_is_asic(card_type_info)) { /* non ASIC, i.e. FPGA/Emul */
    return MTLK_ERR_OK;  /* Send status as OK to Linux Kernel, preventing error print flood on console */
  }

  if (!is_channel_certain(__wave_core_chandef_get(core))) {
    return MTLK_ERR_NOT_READY;
  }

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(core->vap_handle), NULL);
  if (NULL == man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(core->vap_handle));
    res = MTLK_ERR_NO_RESOURCES;
    goto FINISH;
  }

  res = mtlk_core_radio_enable_if_needed(core);
  if (MTLK_ERR_OK != res)
    goto FINISH;

  man_entry->id = UM_MAN_HDK_USER_DEMAND_REQ;
  man_entry->payload_size = sizeof(UMI_HDK_USER_DEMAND);

  pTemperature = (UMI_HDK_USER_DEMAND *)(man_entry->payload);
  pTemperature->calibrateMask = HOST_TO_MAC32(calibrate_mask);

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res) {
    ELOG_DD("CID-%04x: Get temperature failed (%i)", mtlk_vap_get_oid(core->vap_handle), res);
    goto FINISH;
  }

  *temperature = MAC_TO_HOST32(pTemperature->temperature);
  res = mtlk_core_radio_disable_if_needed(core);

FINISH:
  if (man_entry) {
    mtlk_txmm_msg_cleanup(&man_msg);
  }

  return res;
}

mtlk_error_t __MTLK_IFUNC
wave_core_receive_bf_mode (mtlk_core_t *core, uint8 *bf_mode)
{
  mtlk_error_t         res;
  mtlk_txmm_msg_t      man_msg;
  mtlk_txmm_data_t    *man_entry = NULL;
  UMI_BF_MODE_CONFIG  *bf_mode_cfg = NULL;
  mtlk_vap_handle_t    vap_handle = core->vap_handle;

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(vap_handle), NULL);
  if (NULL == man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(vap_handle));
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_MAN_BF_MODE_CONFIG_REQ;
  man_entry->payload_size = sizeof(*bf_mode_cfg);

  bf_mode_cfg = (UMI_BF_MODE_CONFIG *)(man_entry->payload);
  bf_mode_cfg->getSetOperation = API_GET_OPERATION;

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if ((MTLK_ERR_OK == res) && (UMI_OK == bf_mode_cfg->Status)) {
    *bf_mode = bf_mode_cfg->bfMode;
  } else {
    ELOG_DDD("CID-%04x: Receive UM_MAN_BF_MODE_CONFIG_REQ failed, res=%d status=%hhu",
            mtlk_vap_get_oid(vap_handle), res, bf_mode_cfg->Status);
    if (UMI_OK != bf_mode_cfg->Status)
      res = MTLK_ERR_MAC;
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_send_bf_mode (mtlk_core_t *core, BeamformingMode_e bf_mode)
{
  mtlk_error_t         res;
  mtlk_txmm_msg_t      man_msg;
  mtlk_txmm_data_t    *man_entry = NULL;
  UMI_BF_MODE_CONFIG  *bf_mode_config = NULL;
  mtlk_vap_handle_t    vap_handle = core->vap_handle;
  wave_radio_t        *radio = wave_vap_radio_get(vap_handle);
  uint8                cur_antennas = mtlk_coc_get_current_tx_antennas (__wave_core_coc_mgmt_get(core));

  if ((BF_NUMBER_OF_MODES <= bf_mode) &&
      (BF_STATE_AUTO_MODE != bf_mode)) {
    ELOG_DDDD("Wrong Beamforming mode: %u, must be from %u to %u or %u",
           bf_mode, BF_FIRST_STATE, BF_LAST_STATE, BF_STATE_AUTO_MODE);
    return MTLK_ERR_PARAMS;
  }

  if (cur_antennas == 1) {
    _mtlk_coc_set_restore_bf_mode(core, bf_mode, TRUE);
    if (bf_mode != BF_STATE_NON_BF) {
      ILOG0_D("Can't enable Beamforming when number of antennas is %d, will enable BF mode when antenna config changes",
            cur_antennas);
      return MTLK_ERR_OK;
    }
  }

  ILOG2_D("Sending UM_MAN_BF_MODE_CONFIG_REQ with mode %u", bf_mode);

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(vap_handle), NULL);
  if (NULL == man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(vap_handle));
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_MAN_BF_MODE_CONFIG_REQ;
  man_entry->payload_size = sizeof(UMI_BF_MODE_CONFIG);

  bf_mode_config = (UMI_BF_MODE_CONFIG *)(man_entry->payload);
  bf_mode_config->bfMode = bf_mode;

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res || UMI_OK != bf_mode_config->Status) {
    ELOG_DDD("CID-%04x: Set UM_MAN_BF_MODE_CONFIG_REQ failed, res=%d status=%hhu",
            mtlk_vap_get_oid(vap_handle), res, bf_mode_config->Status);
    if (UMI_OK != bf_mode_config->Status)
      res = MTLK_ERR_MAC;
  } else {
    WAVE_RADIO_PDB_SET_INT(radio, PARAM_DB_RADIO_BF_MODE, bf_mode);
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_set_radio_mode_req (mtlk_core_t *core, uint32 enable_radio)
{
  mtlk_error_t         res;
  mtlk_txmm_msg_t      man_msg;
  mtlk_txmm_data_t    *man_entry = NULL;
  UMI_ENABLE_RADIO    *pEnableRadio = NULL;
  mtlk_vap_handle_t    vap_handle = core->vap_handle;
  wave_radio_t        *radio = wave_vap_radio_get(vap_handle);

  MTLK_ASSERT(vap_handle);
  RLOG_DD("CID-%04x: EnableRadio FW request: Set %u mode", mtlk_vap_get_oid(vap_handle), enable_radio);

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(vap_handle), NULL);
  if (NULL == man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(vap_handle));
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_MAN_ENABLE_RADIO_REQ;
  man_entry->payload_size = sizeof(UMI_ENABLE_RADIO);

  pEnableRadio = (UMI_ENABLE_RADIO *)(man_entry->payload);
  pEnableRadio->u32RadioOn = HOST_TO_MAC32(enable_radio);

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res || UMI_OK != pEnableRadio->Status) {
    ELOG_DDD("CID-%04x: Set Radio Enable failure, res=%i status=%hhu",
            mtlk_vap_get_oid(vap_handle), res, pEnableRadio->Status);
    if (UMI_OK != pEnableRadio->Status)
      res = MTLK_ERR_MAC;
  } else {
    WAVE_RADIO_PDB_SET_INT(radio, PARAM_DB_RADIO_MODE_CURRENT, enable_radio);
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_set_radar_detect (mtlk_core_t *core, uint32 radar_detect)
{
  mtlk_error_t         res;
  mtlk_txmm_msg_t      man_msg;
  mtlk_txmm_data_t    *man_entry = NULL;
  mtlk_vap_handle_t    vap_handle = core->vap_handle;
  UMI_ENABLE_RADAR_INDICATION *radar_ind_cfg = NULL;

  MTLK_ASSERT(vap_handle);
  ILOG1_V("Sending UMI_ENABLE_RADAR_INDICATION");

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(vap_handle), NULL);
  if (NULL == man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(vap_handle));
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_MAN_ENABLE_RADAR_INDICATION_REQ;
  man_entry->payload_size = sizeof(UMI_ENABLE_RADAR_INDICATION);

  radar_ind_cfg = (UMI_ENABLE_RADAR_INDICATION *)(man_entry->payload);
  radar_ind_cfg->enableIndication = radar_detect;

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res || UMI_OK != radar_ind_cfg->Status) {
    ELOG_DDD("CID-%04x: Set UMI_ENABLE_RADAR_INDICATION failed, res=%i status=%hhu",
            mtlk_vap_get_oid(vap_handle), res, radar_ind_cfg->Status);
    if (UMI_OK != radar_ind_cfg->Status)
      res = MTLK_ERR_MAC;
  } else {
    WAVE_RADIO_PDB_SET_INT(wave_vap_radio_get(vap_handle), PARAM_DB_RADIO_DFS_RADAR_DETECTION, radar_detect);
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

mtlk_error_t __MTLK_IFUNC
wave_core_send_multicast_rate (mtlk_core_t *core, mtlk_multicast_rate_cfg_t *multicast_rate_cfg)
{
    mtlk_error_t       res;
    mtlk_txmm_msg_t    man_msg;
    mtlk_txmm_data_t  *man_entry = NULL;
    UMI_DMR_CONFIG    *mac_msg = NULL;

    ILOG2_DDD("CID-%04x: Set Multicast Rate. mode: %u, maxRate: %u",
            mtlk_vap_get_oid(core->vap_handle), multicast_rate_cfg->multicast_rate.mode, multicast_rate_cfg->multicast_rate.maxRate);

    /* allocate a new message */
    man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(core->vap_handle), NULL);
    if (!man_entry) {
        ELOG_D("CID-%04x: Can not get TXMM slot", mtlk_vap_get_oid(core->vap_handle));
        return MTLK_ERR_NO_RESOURCES;
    }

    /* fill the message data */
    man_entry->id = UM_MAN_DMR_CONFIG_REQ;
    man_entry->payload_size = sizeof(UMI_DMR_CONFIG);
    mac_msg = (UMI_DMR_CONFIG *)man_entry->payload;
    mac_msg->dmrMode = multicast_rate_cfg->multicast_rate.mode;
    mac_msg->maxRate = multicast_rate_cfg->multicast_rate.maxRate;

    /* send the message to FW */
    res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
    if (MTLK_ERR_OK != res || UMI_OK != mac_msg->status) {
      ELOG_DDD("CID-%04x: Set UM_MAN_DMR_CONFIG_REQ failed, res=%d status=%hhu",
             mtlk_vap_get_oid(core->vap_handle), res, mac_msg->status);
      if (UMI_OK != mac_msg->status)
        res = MTLK_ERR_MAC;
    }
    /* cleanup the message */
    mtlk_txmm_msg_cleanup(&man_msg);

    return res;
}

/************* MU OFDMA Beamforming *******************/

mtlk_error_t __MTLK_IFUNC
wave_core_receive_mu_ofdma_bf (mtlk_core_t *core, mtlk_mu_ofdma_bf_req_cfg_t *mu_ofdma_bf_cfg)
{
  mtlk_error_t            res;
  mtlk_txmm_msg_t         man_msg;
  mtlk_txmm_data_t       *man_entry;
  UMI_MU_OFDMA_BF_CONFIG *mac_msg;

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(core->vap_handle), NULL);
  if (!man_entry) {
    ELOG_D("CID-%04x: Can not get TXMM slot", mtlk_vap_get_oid(core->vap_handle));
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_MAN_MU_OFDMA_BF_CONFIG_REQ;
  man_entry->payload_size = sizeof(UMI_MU_OFDMA_BF_CONFIG);
  mac_msg = (UMI_MU_OFDMA_BF_CONFIG *)man_entry->payload;
  mac_msg->getSetOperation = API_GET_OPERATION;

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res || UMI_OK != mac_msg->Status) {
    ELOG_DDD("CID-%04x: Failed to receive UM_MAN_MU_OFDMA_BF_CONFIG_REQ, res=%d status=%hhu",
           mtlk_vap_get_oid(core->vap_handle), res, mac_msg->Status);
    if (UMI_OK != mac_msg->Status)
      res = MTLK_ERR_MAC;
  } else {
    mu_ofdma_bf_cfg->mode = mac_msg->muBfMode;
    mu_ofdma_bf_cfg->bfPeriod = ((MAC_TO_HOST32(mac_msg->bfPeriod))/1000);
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

mtlk_error_t __MTLK_IFUNC
wave_core_send_mu_ofdma_bf (mtlk_core_t *core, mtlk_mu_ofdma_bf_cfg_t *mu_ofdma_bf_cfg)
{
    mtlk_error_t            res;
    mtlk_txmm_msg_t         man_msg;
    mtlk_txmm_data_t       *man_entry;
    UMI_MU_OFDMA_BF_CONFIG *mac_msg;

    ILOG2_DDD("CID-%04x: Set MU OFDMA BF. muBfEnabledisable: %u, bfperiod: %u", mtlk_vap_get_oid(core->vap_handle),
              mu_ofdma_bf_cfg->mu_ofdma_bf.mode, mu_ofdma_bf_cfg->mu_ofdma_bf.bfPeriod);

    /* allocate a new message */
    man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(core->vap_handle), NULL);
    if (!man_entry) {
        ELOG_D("CID-%04x: Can not get TXMM slot", mtlk_vap_get_oid(core->vap_handle));
        return MTLK_ERR_NO_RESOURCES;
    }

    /* fill the message data */
    man_entry->id = UM_MAN_MU_OFDMA_BF_CONFIG_REQ;
    man_entry->payload_size = sizeof(UMI_MU_OFDMA_BF_CONFIG);
    mac_msg = (UMI_MU_OFDMA_BF_CONFIG *)man_entry->payload;
    mac_msg->muBfMode = mu_ofdma_bf_cfg->mu_ofdma_bf.mode;
    mac_msg->bfPeriod = HOST_TO_MAC32((mu_ofdma_bf_cfg->mu_ofdma_bf.bfPeriod * 1000));

    /* send the message to FW */
    res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
    if (MTLK_ERR_OK != res || UMI_OK != mac_msg->Status) {
      ELOG_DDD("CID-%04x: Set UM_MAN_MU_OFDMA_BF_CONFIG_REQ failed, res=%d status=%hhu",
             mtlk_vap_get_oid(core->vap_handle), res, mac_msg->Status);
      if (UMI_OK != mac_msg->Status)
        res = MTLK_ERR_MAC;
    }
    /* cleanup the message */
    mtlk_txmm_msg_cleanup(&man_msg);
    return res;
}

/************* AGG rate limit *******************/

mtlk_error_t __MTLK_IFUNC
wave_core_receive_agg_rate_limit (mtlk_core_t *core, mtlk_agg_rate_limit_req_cfg_t *arl_cfg)
{
  mtlk_error_t         res;
  mtlk_txmm_msg_t      man_msg;
  mtlk_txmm_data_t    *man_entry;
  UMI_AGG_RATE_LIMIT  *mac_msg;

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(core->vap_handle), NULL);
  if (!man_entry) {
    ELOG_D("CID-%04x: Can not get TXMM slot", mtlk_vap_get_oid(core->vap_handle));
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_MAN_SET_AGG_RATE_LIMIT_REQ;
  man_entry->payload_size = sizeof(UMI_AGG_RATE_LIMIT);
  mac_msg = (UMI_AGG_RATE_LIMIT *)man_entry->payload;
  mac_msg->getSetOperation = API_GET_OPERATION;

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res || UMI_OK != mac_msg->Status) {
    ELOG_DDD("CID-%04x: Failed to receive UM_MAN_SET_AGG_RATE_LIMIT_REQ, res=%d status=%hhu",
           mtlk_vap_get_oid(core->vap_handle), res, mac_msg->Status);
    if (UMI_OK != mac_msg->Status)
      res = MTLK_ERR_MAC;
  } else {
    arl_cfg->mode    = mac_msg->mode;
    arl_cfg->maxRate = MAC_TO_HOST16(mac_msg->maxRate);
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_set_agg_rate_limit (mtlk_core_t *core, mtlk_agg_rate_limit_cfg_t *agg_rate_cfg)
{
    mtlk_error_t           res;
    mtlk_txmm_msg_t        man_msg;
    mtlk_txmm_data_t      *man_entry;
    UMI_AGG_RATE_LIMIT    *mac_msg;

    MTLK_ASSERT(agg_rate_cfg);
    MTLK_ASSERT((0==agg_rate_cfg->agg_rate_limit.mode) || (1==agg_rate_cfg->agg_rate_limit.mode));

    ILOG2_DDD("CID-%04x: Set aggregation-rate limit. Mode: %u, maxRate: %u",
            mtlk_vap_get_oid(core->vap_handle), agg_rate_cfg->agg_rate_limit.mode, agg_rate_cfg->agg_rate_limit.maxRate);

    /* allocate a new message */
    man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(core->vap_handle), NULL);
    if (!man_entry) {
        ELOG_D("CID-%04x: Can not get TXMM slot", mtlk_vap_get_oid(core->vap_handle));
        return MTLK_ERR_NO_RESOURCES;
    }

    /* fill the message data */
    man_entry->id = UM_MAN_SET_AGG_RATE_LIMIT_REQ;
    man_entry->payload_size = sizeof(UMI_AGG_RATE_LIMIT);
    mac_msg = (UMI_AGG_RATE_LIMIT *)man_entry->payload;
    mac_msg->mode = agg_rate_cfg->agg_rate_limit.mode;
    mac_msg->maxRate = HOST_TO_MAC16(agg_rate_cfg->agg_rate_limit.maxRate);

    /* send the message to FW */
    res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
    if (MTLK_ERR_OK != res || UMI_OK != mac_msg->Status) {
      ELOG_DDD("CID-%04x: Set UM_MAN_SET_AGG_RATE_LIMIT_REQ failed, res=%d status=%hhu",
             mtlk_vap_get_oid(core->vap_handle), res, mac_msg->Status);
      if (UMI_OK != mac_msg->Status)
        res = MTLK_ERR_MAC;
    }
    /* cleanup the message */
    mtlk_txmm_msg_cleanup(&man_msg);

    return res;
}

/* Aggregation configuration */

mtlk_error_t __MTLK_IFUNC
mtlk_core_receive_agg_rate_cfg (mtlk_core_t *core,  mtlk_core_aggr_cfg_t *aggr_cfg)
{
  mtlk_error_t           res;
  mtlk_txmm_msg_t        man_msg;
  mtlk_txmm_data_t      *man_entry = NULL;
  UMI_TS_VAP_CONFIGURE  *pAggrConfig = NULL;
  mtlk_vap_handle_t      vap_handle = core->vap_handle;

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(vap_handle), NULL);
  if (NULL == man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(vap_handle));
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_MAN_TS_VAP_CONFIGURE_REQ;
  man_entry->payload_size = sizeof(UMI_TS_VAP_CONFIGURE);

  pAggrConfig = (UMI_TS_VAP_CONFIGURE *)(man_entry->payload);
  pAggrConfig->vapId = (uint8)mtlk_vap_get_id(vap_handle);
  pAggrConfig->getSetOperation = API_GET_OPERATION;

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK == res && UMI_OK == pAggrConfig->Status) {
    aggr_cfg->ba_mode    = pAggrConfig->enableBa;
    aggr_cfg->amsdu_mode = pAggrConfig->amsduSupport;
    aggr_cfg->windowSize = MAC_TO_HOST32(pAggrConfig->windowSize);
  } else {
    ELOG_DDD("CID-%04x: Failed to receive UM_MAN_TS_VAP_CONFIGURE_REQ, res=%d status=%hhu",
              mtlk_vap_get_oid(vap_handle), res, pAggrConfig->Status);
    if (UMI_OK != pAggrConfig->Status)
      res = MTLK_ERR_MAC;
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_set_aggr_cfg_req (mtlk_core_t *core, uint8 enable_amsdu, uint8 enable_ba, uint32 windowSize)
{
  mtlk_error_t           res;
  mtlk_txmm_msg_t        man_msg;
  mtlk_txmm_data_t      *man_entry = NULL;
  UMI_TS_VAP_CONFIGURE  *pAggrConfig = NULL;
  mtlk_vap_handle_t      vap_handle = core->vap_handle;

  MTLK_ASSERT(vap_handle);

  if ((enable_amsdu != 0 && enable_amsdu != 1) ||
      (enable_ba != 0 && enable_ba != 1)) {
    ELOG_V("Wrong parameter value given, must be 0 or 1");
    return MTLK_ERR_PARAMS;
  }

  if ((windowSize != WINDOW_SIZE_NO_CHANGE) &&
      ((windowSize < WINDOW_SIZE_MIN) || (windowSize > WINDOW_SIZE_MAX))) {
    ELOG_DDDD("Wrong windows size provided %d, must be %d (leave unchanged), or %u..%u",
              windowSize, WINDOW_SIZE_NO_CHANGE, WINDOW_SIZE_MIN, WINDOW_SIZE_MAX);
    return MTLK_ERR_PARAMS;
  }

  ILOG1_DDDD("CID-%04x: Send Aggregation config to FW: enable AMSDU %u, enable BA %u, Window size %u",
            mtlk_vap_get_oid(vap_handle), enable_amsdu, enable_ba, windowSize);

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(vap_handle), NULL);
  if (NULL == man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(vap_handle));
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_MAN_TS_VAP_CONFIGURE_REQ;
  man_entry->payload_size = sizeof(UMI_TS_VAP_CONFIGURE);

  pAggrConfig = (UMI_TS_VAP_CONFIGURE *)(man_entry->payload);
  pAggrConfig->vapId = (uint8) mtlk_vap_get_id(vap_handle);
  pAggrConfig->enableBa = (uint8)enable_ba;
  pAggrConfig->amsduSupport = (uint8)enable_amsdu;
  pAggrConfig->windowSize = HOST_TO_MAC32(windowSize);

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res || UMI_OK != pAggrConfig->Status) {
    ELOG_DDD("CID-%04x: Set Aggregation config failure, res=%i status=%hhu",
              mtlk_vap_get_oid(vap_handle), res, pAggrConfig->Status);
    if (UMI_OK != pAggrConfig->Status)
      res = MTLK_ERR_MAC;
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

/************* TX Power Limit configuration ******************/

mtlk_error_t __MTLK_IFUNC
mtlk_core_send_tx_power_limit_offset (mtlk_core_t *core, const uint32 power_limit)
{
    mtlk_error_t         res;
    mtlk_txmm_msg_t      man_msg;
    mtlk_txmm_data_t    *man_entry;
    UMI_TX_POWER_LIMIT  *mac_msg;
    wave_radio_t        *radio = wave_vap_radio_get(core->vap_handle);

    MTLK_ASSERT(((power_limit == 0) ||
                 (power_limit == 1) ||
                 (power_limit == 3) ||
                 (power_limit == 6) ||
                 (power_limit == 9) ||
                 (power_limit == 12)));

    if ((wave_radio_sta_cnt_get(radio) > 0) && !wave_in_fw_recovery(core)) {
      ILOG1_V("Setting TX power limit is disabled while connected STA or peer AP exists");
      return MTLK_ERR_NOT_SUPPORTED;
    }

    ILOG2_DD("CID-%04x: Set TX Power Limit. powerLimitOffset: %u",
            mtlk_vap_get_oid(core->vap_handle), power_limit);

    /* allocate a new message */
    man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(core->vap_handle), NULL);
    if (!man_entry) {
        ELOG_D("CID-%04x: Can not get TXMM slot", mtlk_vap_get_oid(core->vap_handle));
        return MTLK_ERR_NO_RESOURCES;
    }

    /* fill the message data */
    man_entry->id = UM_MAN_SET_POWER_LIMIT_REQ;
    man_entry->payload_size = sizeof(UMI_TX_POWER_LIMIT);
    mac_msg = (UMI_TX_POWER_LIMIT *)man_entry->payload;
    mac_msg->powerLimitOffset = (uint8)power_limit;

    /* send the message to FW */
    res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
    if (MTLK_ERR_OK != res || UMI_OK != mac_msg->Status) {
      ELOG_DDD("CID-%04x: Set UM_MAN_SET_POWER_LIMIT_REQ failed, res=%i status=%hhu",
              mtlk_vap_get_oid(core->vap_handle), res, mac_msg->Status);
      if (UMI_OK != mac_msg->Status)
        res = MTLK_ERR_MAC;
    }

    /* cleanup the message */
    mtlk_txmm_msg_cleanup(&man_msg);

    return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_set_fw_interfdet_req (mtlk_core_t *core, int8 notification_threshold)
{
  mtlk_error_t         res = MTLK_ERR_UNKNOWN;
  mtlk_txmm_msg_t      man_msg;
  mtlk_txmm_data_t    *man_entry = NULL;
  UMI_INTERFERER_DETECTION_PARAMS *umi_interfdet = NULL;

  MTLK_ASSERT(core != NULL);
  MTLK_ASSERT(mtlk_vap_is_master_ap(core->vap_handle));

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(core->vap_handle), NULL);
  if (!man_entry) {
    res = MTLK_ERR_NO_RESOURCES;
    goto END;
  }

  man_entry->id = UM_MAN_SET_INTERFERER_DETECTION_PARAMS_REQ;
  man_entry->payload_size = sizeof(*umi_interfdet);

  umi_interfdet = (UMI_INTERFERER_DETECTION_PARAMS *)man_entry->payload;
  umi_interfdet->threshold = notification_threshold;

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (res != MTLK_ERR_OK || UMI_OK != umi_interfdet->Status) {
    ELOG_DDD("CID-%04x: Cannot send UM_MAN_SET_INTERFERER_DETECTION_PARAMS_REQ to the FW, res=%d status=%hhu",
            mtlk_vap_get_oid(core->vap_handle), res, umi_interfdet->Status);
    if (UMI_OK != umi_interfdet->Status)
      res = MTLK_ERR_MAC;
    goto END;
  }

  ILOG1_DD("CID-%04x: UMI_INTERFERER_DETECTION_PARAMS: Threshold: %d",
            mtlk_vap_get_oid(core->vap_handle), umi_interfdet->threshold);

END:
  if (man_entry) {
    mtlk_txmm_msg_cleanup(&man_msg);
  }

  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_send_11b_antsel (mtlk_core_t *core, const mtlk_11b_antsel_t *antsel)
{
#define _11B_ANTSEL_ROUNDROBIN    4 /* TODO: move this define to the mac shared files */

  /* TODO-ZWDFS: probably needs to be re-writen to support 4x4 -> 3x3 + 1x1 */
  mtlk_error_t           res = MTLK_ERR_UNKNOWN;
  mtlk_txmm_msg_t        man_msg;
  mtlk_txmm_data_t      *man_entry = NULL;
  UMI_ANT_SELECTION_11B *umi_11b_antsel = NULL;
  BOOL valid_roundrobin;
  BOOL valid_ant_sel;
  uint32 tx_hw_supported_antenna_mask;
  uint32 rx_hw_supported_antenna_mask;
  mtlk_hw_t *hw;

  MTLK_ASSERT(core != NULL);
  MTLK_ASSERT(antsel != NULL);
  MTLK_STATIC_ASSERT(MAX_NUM_TX_ANTENNAS <= _11B_ANTSEL_ROUNDROBIN);
  MTLK_STATIC_ASSERT(MAX_NUM_RX_ANTENNAS <= _11B_ANTSEL_ROUNDROBIN);

  hw = mtlk_vap_get_hw(core->vap_handle);
  tx_hw_supported_antenna_mask = hw_get_tx_antenna_mask(hw);
  rx_hw_supported_antenna_mask = hw_get_rx_antenna_mask(hw);

  /* rate field is set only when txAnt and rxAnt set to Round Robin mode,
     otherwise the rate value should be 0 */

  valid_roundrobin = (antsel->txAnt == _11B_ANTSEL_ROUNDROBIN) &&
                     (antsel->rxAnt == _11B_ANTSEL_ROUNDROBIN) &&
                     (antsel->rate != 0);

  valid_ant_sel = (antsel->rate == 0) &&
                  (antsel->txAnt < MAX_NUM_TX_ANTENNAS) &&
                  (antsel->rxAnt < MAX_NUM_RX_ANTENNAS);

  if (!valid_roundrobin && !valid_ant_sel) {
    ELOG_DDDD("CID-%04x: Incorrect configuration: txAnt=%d, rxAnt=%d, rate=%d", mtlk_vap_get_oid(core->vap_handle), antsel->txAnt, antsel->rxAnt, antsel->rate);
    res = MTLK_ERR_PARAMS;
    goto END;
  }

  if (valid_ant_sel &&
    !((tx_hw_supported_antenna_mask & (1 << antsel->txAnt)) &&
      (rx_hw_supported_antenna_mask & (1 << antsel->rxAnt)))) {
    ELOG_DDDDD("CID-%04x: Antenna selection is not supported by HW: txAnt=%d, rxAnt=%d, TX/RX mask: 0x%x/0x%x",
    mtlk_vap_get_oid(core->vap_handle), antsel->txAnt, antsel->rxAnt, tx_hw_supported_antenna_mask, rx_hw_supported_antenna_mask);
    res = MTLK_ERR_PARAMS;
    goto END;
  }

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(core->vap_handle), NULL);
  if (!man_entry) {
    res = MTLK_ERR_NO_RESOURCES;
    goto END;
  }

  man_entry->id = UM_MAN_SEND_11B_SET_ANT_REQ;
  man_entry->payload_size = sizeof(*umi_11b_antsel);

  umi_11b_antsel = (UMI_ANT_SELECTION_11B *)man_entry->payload;
  umi_11b_antsel->txAnt = antsel->txAnt;
  umi_11b_antsel->rxAnt = antsel->rxAnt;
  umi_11b_antsel->rate  = antsel->rate;

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (res != MTLK_ERR_OK || UMI_OK != umi_11b_antsel->Status) {
    ELOG_DDD("CID-%04x: Set UM_MAN_SEND_11B_SET_ANT_REQ failed, err=%d status=%hhu",
            mtlk_vap_get_oid(core->vap_handle), res, umi_11b_antsel->Status);
    if (UMI_OK != umi_11b_antsel->Status)
      res = MTLK_ERR_MAC;
    goto END;
  }

END:
  if (man_entry) {
    mtlk_txmm_msg_cleanup(&man_msg);
  }

  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_receive_11b_antsel (mtlk_core_t *core, mtlk_11b_antsel_t *antsel)
{
  mtlk_error_t           res;
  mtlk_txmm_msg_t        man_msg;
  mtlk_txmm_data_t      *man_entry = NULL;
  UMI_ANT_SELECTION_11B *umi_11b_antsel = NULL;

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(core->vap_handle), NULL);
  if (!man_entry)
    return MTLK_ERR_NO_RESOURCES;

  man_entry->id = UM_MAN_SEND_11B_SET_ANT_REQ;
  man_entry->payload_size = sizeof(*umi_11b_antsel);
  umi_11b_antsel = (UMI_ANT_SELECTION_11B *)man_entry->payload;
  umi_11b_antsel->getSetOperation = API_GET_OPERATION;

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK == res && UMI_OK == umi_11b_antsel->Status) {
    antsel->txAnt = umi_11b_antsel->txAnt;
    antsel->rxAnt = umi_11b_antsel->rxAnt;
    antsel->rate  = umi_11b_antsel->rate;
  } else {
    ELOG_DDD("CID-%04x: Failed to receive UM_MAN_SEND_11B_SET_ANT_REQ, res=%i status=%hhu",
            mtlk_vap_get_oid(core->vap_handle), res, umi_11b_antsel->Status);
    if (UMI_OK != umi_11b_antsel->Status)
      res = MTLK_ERR_MAC;
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_set_wep_key_blocked (struct nic  *nic,
                                uint16     *sid_p,
                                uint16      key_idx)
{
  mtlk_error_t           res = MTLK_ERR_UNKNOWN;
  mtlk_txmm_msg_t        man_msg;
  mtlk_txmm_data_t      *man_entry = NULL;
  UMI_SET_KEY           *umi_key;
  UMI_DEFAULT_KEY_INDEX *umi_default_key;
  uint16                 key_len = 0;
  int                    i;
  uint16                 sid;
  uint16                 key_type;

  if(key_idx >= WAVE_WEP_NUM_DEFAULT_KEYS) {
    ELOG_DD("CID-%04x: Wrong default key index %d", mtlk_vap_get_oid(nic->vap_handle), key_idx);
    goto FINISH;
  }

  if(0 == nic->slow_ctx->wds_keys[key_idx].key_len) {
    ELOG_DD("CID-%04x: There is no key with default index %d", mtlk_vap_get_oid(nic->vap_handle), key_idx);
    goto FINISH;
  }

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(nic->vap_handle), &res);
  if (!man_entry) {
    ELOG_DD("CID-%04x: No man entry available (res = %d)", mtlk_vap_get_oid(nic->vap_handle), res);
    goto FINISH;
  }

  if (sid_p) {
    key_type = HOST_TO_MAC16(UMI_RSN_PAIRWISE_KEY);
    sid      = HOST_TO_MAC16(*sid_p);
  } else {
    key_type = HOST_TO_MAC16(UMI_RSN_GROUP_KEY);
    sid      = HOST_TO_MAC16(wave_core_get_all_sta_sid(nic));
  }

  man_entry->id           = UM_MAN_SET_KEY_REQ;
  man_entry->payload_size = sizeof(*umi_key);
  umi_key = (UMI_SET_KEY*)man_entry->payload;

  for (i = 0; i < WAVE_WEP_NUM_DEFAULT_KEYS; i++) {
    key_len = nic->slow_ctx->wds_keys[i].key_len;
    if(0 == key_len) { /* key is not set */
      break;
    }
    if ((nic->slow_ctx->wds_keys[i].cipher != SECURITY_TYPE_SECD_WEP40) && (nic->slow_ctx->wds_keys[i].cipher != SECURITY_TYPE_SECD_WEP104)) {
      ELOG_DD("CID-%04x: CipherSuite is not WEP %d", mtlk_vap_get_oid(nic->vap_handle), nic->slow_ctx->wds_keys[i].cipher);
      goto FINISH;
    }
    /* Prepeare the key */
    memset(umi_key, 0, sizeof(*umi_key));
    umi_key->u16Sid         = sid;
    umi_key->u16KeyType     = key_type;
    umi_key->u16KeyIndex    = HOST_TO_MAC16(i);
    umi_key->u16CipherSuite = HOST_TO_MAC16(nic->slow_ctx->wds_keys[i].cipher);
    wave_memcpy(umi_key->au8Tk, sizeof(umi_key->au8Tk), nic->slow_ctx->wds_keys[i].key, key_len);

    ILOG1_DDDD("UMI_SET_KEY SID:0x%x, Type:0x%x, Suite:0x%x, Index:%d",
               MAC_TO_HOST16(umi_key->u16Sid), MAC_TO_HOST16(umi_key->u16KeyType),
               MAC_TO_HOST16(umi_key->u16CipherSuite), MAC_TO_HOST16(umi_key->u16KeyIndex));
    mtlk_dump(2, umi_key->au8RxSeqNum, sizeof(umi_key->au8RxSeqNum), "RxSeqNum");
    mtlk_dump(2, umi_key->au8TxSeqNum, sizeof(umi_key->au8TxSeqNum), "TxSeqNum");
    mtlk_dump(2, umi_key->au8Tk, key_len, "KEY:");

    res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
    if (res != MTLK_ERR_OK) {
      ELOG_DD("CID-%04x: mtlk_mm_send failed: %d", mtlk_vap_get_oid(nic->vap_handle), res);
      goto FINISH;
    }
    if (MAC_TO_HOST16(umi_key->u16Status) != UMI_OK) {
      ELOG_DD("CID-%04x: MAC returned error (status=%hu) for UM_MAN_SET_KEY_REQ",
          mtlk_vap_get_oid(nic->vap_handle), MAC_TO_HOST16(umi_key->u16Status));
      res = MTLK_ERR_MAC;
      goto FINISH;
    }
  }

  /* Send default key index */
  man_entry->id           = UM_MAN_SET_DEFAULT_KEY_INDEX_REQ;
  man_entry->payload_size = sizeof(*umi_key);
  umi_default_key = (UMI_DEFAULT_KEY_INDEX *)man_entry->payload;
  umi_default_key->u16SID     = sid;
  umi_default_key->u16KeyType = key_type;
  umi_default_key->u16KeyIndex = MAC_TO_HOST16(key_idx);

  ILOG1_D("UMI_SET_DEFAULT_KEY             SID:0x%x", MAC_TO_HOST16(umi_default_key->u16SID));
  ILOG1_D("UMI_SET_DEFAULT_KEY         KeyType:0x%x", MAC_TO_HOST16(umi_default_key->u16KeyType));
  ILOG1_D("UMI_SET_DEFAULT_KEY        KeyIndex:%d", MAC_TO_HOST16(umi_default_key->u16KeyIndex));

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (res != MTLK_ERR_OK || UMI_OK != MAC_TO_HOST16(umi_default_key->u16Status)) {
    ELOG_DDD("CID-%04x: failed to send default key, res=%d status=%hu",
            mtlk_vap_get_oid(nic->vap_handle), res, MAC_TO_HOST16(umi_default_key->u16Status));
    if (UMI_OK != MAC_TO_HOST16(umi_default_key->u16Status))
      res = MTLK_ERR_MAC;
    goto FINISH;
  }

FINISH:
  if (man_entry) {
    mtlk_txmm_msg_cleanup(&man_msg);
  }

  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_set_default_key_cfg (mtlk_handle_t hcore, const void* data, uint32 data_size)
{
  mtlk_error_t              res = MTLK_ERR_OK;
  mtlk_core_t              *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t              *clpb = *(mtlk_clpb_t **) data;
  uint32                    size;
  mtlk_txmm_msg_t           man_msg;
  mtlk_txmm_data_t         *man_entry = NULL;
  mtlk_txmm_t              *txmm;
  mtlk_hw_t                *hw;
  UMI_DEFAULT_KEY_INDEX    *umi_default_key;
  sta_entry                *sta = NULL;
  uint16                   key_indx;
  mtlk_core_ui_default_key_cfg_t *default_key_cfg;

  MTLK_ASSERT(core != NULL);
  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);

  txmm = mtlk_vap_get_txmm(core->vap_handle);
  hw = mtlk_vap_get_hw(core->vap_handle);

  if (wave_in_fw_recovery(core)) {
    ELOG_V("Recovery is running, can't set default key");
    res = MTLK_ERR_NOT_READY;
    goto FINISH;
  }

  if (mtlk_vap_is_ap(core->vap_handle) && !wave_core_sync_hostapd_done_get(core)) {
    ILOG1_D("CID-%04x: HOSTAPD STA database is not synced", mtlk_vap_get_oid(core->vap_handle));
    res = MTLK_ERR_NOT_READY;
    goto FINISH;
  }

  default_key_cfg = mtlk_clpb_enum_get_next(clpb, &size);
  if ( (NULL == default_key_cfg) || (sizeof(*default_key_cfg) != size) ) {
    ELOG_D("CID-%04x: Failed to get ENC_EXT configuration parameters from CLPB", mtlk_vap_get_oid(core->vap_handle));
    res = MTLK_ERR_UNKNOWN;
    goto FINISH;
  }

  /* Prepare UMI message */
  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, txmm, NULL);
  if (!man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(core->vap_handle));
    res = MTLK_ERR_NO_RESOURCES;
    goto FINISH;
  }

  man_entry->id           = UM_MAN_SET_DEFAULT_KEY_INDEX_REQ;
  man_entry->payload_size = sizeof(UMI_DEFAULT_KEY_INDEX);
  umi_default_key = (UMI_DEFAULT_KEY_INDEX *)man_entry->payload;

  if (mtlk_osal_eth_is_broadcast(default_key_cfg->sta_addr.au8Addr)) {
    umi_default_key->u16SID     = HOST_TO_MAC16(wave_core_get_all_sta_sid(core));
    if (default_key_cfg->is_mgmt_key) {
       umi_default_key->u16KeyType = HOST_TO_MAC16(UMI_RSN_MGMT_GROUP_KEY);
    } else {
       umi_default_key->u16KeyType = IS_BCN_KEY_IDX(default_key_cfg->key_idx) ? HOST_TO_MAC16(UMI_RSN_MGMT_BEACON_GROUP_KEY) : HOST_TO_MAC16(UMI_RSN_GROUP_KEY);
    }
  } else {
    /* Check STA availability */
    sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, default_key_cfg->sta_addr.au8Addr);
    if (NULL == sta) {
      ILOG1_Y("There is no connection with %Y", default_key_cfg->sta_addr.au8Addr);
      res = MTLK_ERR_PARAMS;
      goto FINISH;
    }

    umi_default_key->u16SID     = HOST_TO_MAC16(mtlk_sta_get_sid(sta));
    umi_default_key->u16KeyType = HOST_TO_MAC16(UMI_RSN_PAIRWISE_KEY);
  }

  key_indx = default_key_cfg->key_idx;
  umi_default_key->u16KeyIndex = HOST_TO_MAC16(key_indx);

  ILOG2_D("UMI_SET_DEFAULT_KEY             SID:0x%x", MAC_TO_HOST16(umi_default_key->u16SID));
  ILOG2_D("UMI_SET_DEFAULT_KEY         KeyType:0x%x", MAC_TO_HOST16(umi_default_key->u16KeyType));
  ILOG2_D("UMI_SET_DEFAULT_KEY        KeyIndex:%d", MAC_TO_HOST16(umi_default_key->u16KeyIndex));

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res || UMI_OK != MAC_TO_HOST16(umi_default_key->u16Status)) {
    ELOG_DDD("CID-%04x: Set UM_MAN_SET_DEFAULT_KEY_INDEX_REQ failed, res=%i status=%hu",
              mtlk_vap_get_oid(core->vap_handle), res, MAC_TO_HOST16(umi_default_key->u16Status));
    if (UMI_OK != MAC_TO_HOST16(umi_default_key->u16Status))
      res = MTLK_ERR_MAC;
    goto FINISH;
  }

  /* Store default key */
  if (default_key_cfg->is_mgmt_key) {
    core->slow_ctx->default_mgmt_key = key_indx;
  } else {
    core->slow_ctx->default_key = key_indx;
  }

FINISH:
  if (NULL != sta) {
    mtlk_sta_decref(sta); /* De-reference of find */
  }
  if (NULL != man_entry) {
    mtlk_txmm_msg_cleanup(&man_msg);
  }

  return res;
}

mtlk_error_t __MTLK_IFUNC
core_recovery_default_key (mtlk_core_t *core, BOOL is_mgmt_key)
{
  mtlk_error_t           res;
  mtlk_txmm_msg_t        man_msg;
  mtlk_txmm_data_t      *man_entry = NULL;
  mtlk_txmm_t           *txmm = mtlk_vap_get_txmm(core->vap_handle);
  UMI_DEFAULT_KEY_INDEX *umi_default_key;
  uint16                 alg_type;

  alg_type = core->slow_ctx->group_cipher;
  if (alg_type == IW_ENCODE_ALG_NONE) {
    ILOG2_D("CID-%04x: ENCODE ALG is NONE, ignore set default key index.", mtlk_vap_get_oid(core->vap_handle));
    res = MTLK_ERR_OK;
    goto FINISH;
  }

  /* Prepare UMI message */
  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, txmm, NULL);
  if (!man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(core->vap_handle));
    res = MTLK_ERR_NO_RESOURCES;
    goto FINISH;
  }

  man_entry->id           = UM_MAN_SET_DEFAULT_KEY_INDEX_REQ;
  man_entry->payload_size = sizeof(UMI_DEFAULT_KEY_INDEX);
  umi_default_key = (UMI_DEFAULT_KEY_INDEX *)man_entry->payload;
  umi_default_key->u16SID     = HOST_TO_MAC16(wave_core_get_all_sta_sid(core));
  if (is_mgmt_key) {
    umi_default_key->u16KeyType = HOST_TO_MAC16(UMI_RSN_MGMT_GROUP_KEY);
    umi_default_key->u16KeyIndex = HOST_TO_MAC16(core->slow_ctx->default_mgmt_key);
  } else {
    umi_default_key->u16KeyType = HOST_TO_MAC16(UMI_RSN_GROUP_KEY);
    umi_default_key->u16KeyIndex = HOST_TO_MAC16(core->slow_ctx->default_key);
  }

  ILOG2_D("UMI_SET_DEFAULT_KEY             SID:0x%x", MAC_TO_HOST16(umi_default_key->u16SID));
  ILOG2_D("UMI_SET_DEFAULT_KEY         KeyType:0x%x", MAC_TO_HOST16(umi_default_key->u16KeyType));
  ILOG2_D("UMI_SET_DEFAULT_KEY        KeyIndex:%d", MAC_TO_HOST16(umi_default_key->u16KeyIndex));

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res || UMI_OK != MAC_TO_HOST16(umi_default_key->u16Status)) {
    ELOG_DDD("CID-%04x: Set UM_MAN_SET_DEFAULT_KEY_INDEX_REQ failed, res=%i status=%hu",
            mtlk_vap_get_oid(core->vap_handle), res, MAC_TO_HOST16(umi_default_key->u16Status));
    if (UMI_OK != MAC_TO_HOST16(umi_default_key->u16Status))
      res = MTLK_ERR_MAC;
    goto FINISH;
  }

FINISH:
  if (NULL != man_entry) {
    mtlk_txmm_msg_cleanup(&man_msg);
  }

  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_send_filter_req (mtlk_core_t *core, uint16 sid, BOOL authorizing)
{
  mtlk_error_t         res = MTLK_ERR_PARAMS;
  mtlk_txmm_msg_t      man_msg;
  mtlk_txmm_data_t    *man_entry = NULL;
  mtlk_txmm_t         *txmm = mtlk_vap_get_txmm(core->vap_handle);
  UMI_802_1X_FILTER   *umi_filter;

  /* Prepare UMI message */
  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, txmm, NULL);
  if (!man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(core->vap_handle));
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id           = UM_MAN_SET_802_1X_FILTER_REQ;
  man_entry->payload_size = sizeof(UMI_802_1X_FILTER);
  umi_filter = (UMI_802_1X_FILTER *)man_entry->payload;
  umi_filter->u16SID         = HOST_TO_MAC16(sid);
  umi_filter->u8IsFilterOpen = authorizing;

  ILOG1_DD("CID-%04x: UM_MAN_SET_802_1X_FILTER_REQ            SID:0x%x", mtlk_vap_get_oid(core->vap_handle),
           MAC_TO_HOST16(umi_filter->u16SID));
  ILOG1_DD("CID-%04x: UM_MAN_SET_802_1X_FILTER_REQ u8IsFilterOpen:%d", mtlk_vap_get_oid(core->vap_handle),
           umi_filter->u8IsFilterOpen);

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res || UMI_OK != MAC_TO_HOST16(umi_filter->u16Status)) {
    ELOG_DDD("CID-%04x: mtlk_mm_send_blocked failed, res=%i status=%hu",
            mtlk_vap_get_oid(core->vap_handle), res, MAC_TO_HOST16(umi_filter->u16Status));
    if (UMI_OK != MAC_TO_HOST16(umi_filter->u16Status))
      res = MTLK_ERR_MAC;
  }

  if (NULL != man_entry) {
    mtlk_txmm_msg_cleanup(&man_msg);
  }

  return res;
}

static __INLINE unsigned
_mtlk_core_get_rrsi_offs (mtlk_core_t *nic)
{
    return mtlk_hw_get_rrsi_offs(mtlk_vap_get_hw(nic->vap_handle));
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_ap_add_sta_req (struct nic *nic, struct ieee80211_sta *mac80211_sta)
{
  int               res = MTLK_ERR_OK;
  mtlk_txmm_msg_t   man_msg;
  mtlk_txmm_data_t *man_entry = NULL;
  UMI_STA_ADD      *psUmiStaAdd;
  uint8             u8Flags;
  uint8             u8FlagsExt;
  unsigned          rssi_offs;
  int               rssi;
  sta_entry        *sta = wv_ieee80211_get_sta(mac80211_sta);
  sta_info         *info = &sta->info;
  uint8 zero_rx_mask[WV_HT_MCS_MASK_LEN] = { 0 };
  uint16 zero_vht_info[WV_VHT_MCS_INFO_NUM_FIELDS] = { 0 };
  uint32 ampdu_density;
  mtlk_hw_band_e band = MTLK_HW_BAND_NONE;
  wave_radio_t *radio = wave_vap_radio_get(nic->vap_handle);
#ifdef MTLK_WAVE_700
  uint16         num_of_high_speed_sta;
  mtlk_hw_t     *hw;
#endif

  MTLK_ASSERT(NULL != nic);
  MTLK_ASSERT(NULL != info);

  if (info->supported_rates_len == 0) {
    ELOG_Y("Rates length is zero, STA %Y", mac80211_sta->addr);
    res = MTLK_ERR_PARAMS;
    goto FINISH;
  } else if (info->supported_rates_len > MAX_NUM_SUPPORTED_RATES) {
    ELOG_DDY("Rates length (%u) is longer than rates array size (%d), STA %Y",
      info->supported_rates_len, MAX_NUM_SUPPORTED_RATES, mac80211_sta->addr);
    res = MTLK_ERR_PARAMS;
    goto FINISH;
  }

  mtlk_core_find_max_sta_rate(nic, sta, mac80211_sta);

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(nic->vap_handle), &res);
  if (!man_entry) {
    ELOG_D("CID-%04x: Can't send STA_ADD request to MAC due to the lack of MAN_MSG",
           mtlk_vap_get_oid(nic->vap_handle));
    res = MTLK_ERR_NO_RESOURCES;
    goto FINISH;
  }

  man_entry->id           = UM_MAN_STA_ADD_REQ;
  man_entry->payload_size = sizeof(UMI_STA_ADD);
  psUmiStaAdd = (UMI_STA_ADD *)man_entry->payload;

  rssi_offs = _mtlk_core_get_rrsi_offs(nic);
  rssi      = info->rssi_dbm + rssi_offs;
  ILOG2_DDD("rssi_dbm %d, offs %d, rssi %d", info->rssi_dbm, rssi_offs, rssi);
  MTLK_ASSERT((MIN_RSSI <= rssi) && (rssi <= MAX_INT8));

  psUmiStaAdd->u8Status         = UMI_OK;
  psUmiStaAdd->u16SID           = HOST_TO_MAC16(info->sid);
  psUmiStaAdd->u8VapIndex       = mtlk_vap_get_id(nic->vap_handle);
  psUmiStaAdd->u8SecondaryVapIndex = sta->secondary_vap_id;
  psUmiStaAdd->u8BSS_Coex_20_40 = info->bss_coex_20_40;
  psUmiStaAdd->u8UAPSD_Queues   = mac80211_sta->uapsd_queues;
  psUmiStaAdd->u8Max_SP         = mac80211_sta->max_sp;
  psUmiStaAdd->u16AID           = HOST_TO_MAC16(mac80211_sta->aid);
  if (mac80211_sta->deflink.ht_cap.ht_supported && memcmp(zero_rx_mask, mac80211_sta->deflink.ht_cap.mcs.rx_mask,
      sizeof(zero_rx_mask)) != 0) {
    psUmiStaAdd->u16HT_Cap_Info   = MAC_TO_HOST16(mac80211_sta->deflink.ht_cap.cap);  /* Do not convert to MAC format */
  }

  ampdu_density = mtlk_core_get_max_tx_ampdu_density(nic, (uint32)MTLK_BFIELD_GET(info->ampdu_param, MTLK_STA_AMPDU_PARAMS_MIN_START_SPACING));
  psUmiStaAdd->u8AMPDU_Param    = ((info->ampdu_param & (~IEEE80211_HT_AMPDU_PARM_DENSITY)) | (ampdu_density << IEEE80211_HT_AMPDU_PARM_DENSITY_SHIFT));

  ieee_addr_set(&psUmiStaAdd->sAddr, mac80211_sta->addr);
  psUmiStaAdd->u8Rates_Length   = info->supported_rates_len;
  psUmiStaAdd->u8ListenInterval = info->listen_interval;

 if(mac80211_sta->deflink.vht_cap.vht_supported && memcmp(&zero_vht_info, &mac80211_sta->deflink.vht_cap.vht_mcs,
     sizeof(zero_vht_info)) != 0) {
    psUmiStaAdd->u32VHT_Cap_Info  = MAC_TO_HOST32(mac80211_sta->deflink.vht_cap.cap);
  }

  if(mac80211_sta->deflink.he_cap.has_he) {
    psUmiStaAdd->u8HE_Mac_Phy_Cap_Info[0] = WLAN_EID_EXT_HE_CAPABILITY;
    wave_memcpy(psUmiStaAdd->u8HE_Mac_Phy_Cap_Info + 1, sizeof(psUmiStaAdd->u8HE_Mac_Phy_Cap_Info) - 1,
          &mac80211_sta->deflink.he_cap.he_cap_elem, sizeof(mac80211_sta->deflink.he_cap.he_cap_elem));
  }

  psUmiStaAdd->rssi = rssi;
  psUmiStaAdd->transmitBfCapabilities = info->tx_bf_cap_info; /* Do not convert to MAC format */
  psUmiStaAdd->u8VHT_OperatingModeNotification = info->opmode_notif;
  psUmiStaAdd->u8WDS_client_type = info->WDS_client_type;

  u8Flags = 0;
  MTLK_BFIELD_SET(u8Flags, STA_ADD_FLAGS_WDS,
    MTLK_BFIELD_GET(info->flags, STA_FLAGS_WDS) || mtlk_sta_info_is_4addr(info));
  MTLK_BFIELD_SET(u8Flags, STA_ADD_FLAGS_MFP, MTLK_BFIELD_GET(info->flags, STA_FLAGS_MFP));
  MTLK_BFIELD_SET(u8Flags, STA_ADD_FLAGS_IS_HT, MTLK_BFIELD_GET(info->flags, STA_FLAGS_11n));
  MTLK_BFIELD_SET(u8Flags, STA_ADD_FLAGS_IS_VHT, MTLK_BFIELD_GET(info->flags, STA_FLAGS_11ac));
  MTLK_BFIELD_SET(u8Flags, STA_ADD_FLAGS_OMN, MTLK_BFIELD_GET(info->flags, STA_FLAGS_OMN_SUPPORTED));
  MTLK_BFIELD_SET(u8Flags, STA_ADD_FLAGS_OPER_MODE_NOTIF_VALID, MTLK_BFIELD_GET(info->flags, STA_FLAGS_OPMODE_NOTIF));
  MTLK_BFIELD_SET(u8Flags, STA_ADD_FLAGS_WME, MTLK_BFIELD_GET(info->flags, STA_FLAGS_WMM) ||
                                              MTLK_BFIELD_GET(u8Flags, STA_ADD_FLAGS_IS_HT) ||
                                              MTLK_BFIELD_GET(u8Flags, STA_ADD_FLAGS_IS_VHT));
  psUmiStaAdd->u8Flags = u8Flags;

  u8FlagsExt = 0;
  MTLK_BFIELD_SET(u8FlagsExt, STA_ADD_FLAGS_EXT_IS_HE, MTLK_BFIELD_GET(info->flags, STA_FLAGS_11ax));
  MTLK_BFIELD_SET(u8FlagsExt, STA_ADD_FLAGS_EXT_PBAC, MTLK_BFIELD_GET(info->flags, STA_FLAGS_PBAC));

  if (mac80211_sta->ml_sta_info.is_ml) {
     MTLK_BFIELD_SET(u8FlagsExt, STA_ADD_FLAGS_EXT_ML_STA, 1);
     if (mtlk_osal_is_zero_address(mac80211_sta->ml_sta_info.linked_sta_mac))
        MTLK_BFIELD_SET(u8FlagsExt, STA_ADD_FLAGS_EXT_ML_SINGLE_LINK, 1);
  }

  psUmiStaAdd->u8FlagsExt = u8FlagsExt;

  wave_memcpy(psUmiStaAdd->u8Rates, sizeof(psUmiStaAdd->u8Rates),
      info->rates, psUmiStaAdd->u8Rates_Length);
  wave_memcpy(psUmiStaAdd->u8HT_MCS_Set, sizeof(psUmiStaAdd->u8HT_MCS_Set),
      &mac80211_sta->deflink.ht_cap.mcs, sizeof(mac80211_sta->deflink.ht_cap.mcs));
  wave_memcpy(psUmiStaAdd->u32VHT_Mcs_Nss, sizeof(psUmiStaAdd->u32VHT_Mcs_Nss),
      &mac80211_sta->deflink.vht_cap.vht_mcs, sizeof(mac80211_sta->deflink.vht_cap.vht_mcs));
  wave_memcpy(psUmiStaAdd->u8HE_Mcs_Nss, sizeof(psUmiStaAdd->u8HE_Mcs_Nss),
      &mac80211_sta->deflink.he_cap.he_mcs_nss_supp,
      MIN(sizeof(psUmiStaAdd->u8HE_Mcs_Nss), sizeof(mac80211_sta->deflink.he_cap.he_mcs_nss_supp))); /* 80P80 not supported by FW */

  wave_memcpy(psUmiStaAdd->u8HE_Ppe_Th, sizeof(psUmiStaAdd->u8HE_Ppe_Th),
      &mac80211_sta->deflink.he_cap.ppe_thres,
      MIN(sizeof(psUmiStaAdd->u8HE_Ppe_Th), sizeof(mac80211_sta->deflink.he_cap.ppe_thres)));  /* 8 NSS not supported by FW */

  band = wave_radio_band_get(radio);
  if ((mac80211_sta->deflink.he_cap.has_he) && (MTLK_HW_BAND_6_GHZ == band)) {
    ILOG1_DD("STA_ADD for band %d: he_6ghz_cap = %x", band, mac80211_sta->deflink.he_6ghz_capa.capa);
    psUmiStaAdd->u16HE_6ghz_Cap_Info = MAC_TO_HOST16(mac80211_sta->deflink.he_6ghz_capa.capa);
    MTLK_BFIELD_SET(u8FlagsExt, STA_ADD_FLAGS_EXT_IS_HE_6GHZ, MTLK_BFIELD_GET(info->flags, STA_FLAGS_11ax_6ghz));
    psUmiStaAdd->u8FlagsExt = u8FlagsExt;
  }
#ifdef MTLK_WAVE_700
  if(wave_radio_is_gen7(radio)) {
    if (mac80211_sta->deflink.eht_cap.has_eht) {
      psUmiStaAdd->u8EHT_Mac_Phy_Cap_Info[0] = WLAN_EID_EXT_EHT_CAPABILITY;
      wave_memcpy(psUmiStaAdd->u8EHT_Mac_Phy_Cap_Info + 1, sizeof(psUmiStaAdd->u8EHT_Mac_Phy_Cap_Info) - 1,
                  &mac80211_sta->deflink.eht_cap.eht_cap_elem, sizeof(mac80211_sta->deflink.eht_cap.eht_cap_elem));
      /*Handle MCS/NSS support for 20 MHz-only STA */
      if (!(mac80211_sta->deflink.he_cap.he_cap_elem.phy_cap_info[0] & IEEE80211_HE_PHY_CAP0_CHANNEL_WIDTH_SET_MASK_ALL)) {
        wave_memcpy(psUmiStaAdd->u8EHT_Mcs_Nss, sizeof(mac80211_sta->deflink.eht_cap.eht_mcs_nss_supp.only_20mhz),
                    &mac80211_sta->deflink.eht_cap.eht_mcs_nss_supp,
                    MIN(sizeof(psUmiStaAdd->u8EHT_Mcs_Nss), sizeof(mac80211_sta->deflink.eht_cap.eht_mcs_nss_supp.only_20mhz)));
      } else {
        wave_memcpy(&psUmiStaAdd->u8EHT_Mcs_Nss[EHT_MCS_MAP_BW_LE_80MHZ_IDX], sizeof(mac80211_sta->deflink.eht_cap.eht_mcs_nss_supp),
                    &mac80211_sta->deflink.eht_cap.eht_mcs_nss_supp,
                    MIN(sizeof(psUmiStaAdd->u8EHT_Mcs_Nss), sizeof(mac80211_sta->deflink.eht_cap.eht_mcs_nss_supp)));
      }
      wave_memcpy(psUmiStaAdd->u8EHT_Ppe_Th, sizeof(psUmiStaAdd->u8EHT_Ppe_Th),
                  &mac80211_sta->deflink.eht_cap.eht_ppe_thres,
                  MIN(sizeof(psUmiStaAdd->u8EHT_Ppe_Th), sizeof(mac80211_sta->deflink.eht_cap.eht_ppe_thres)));  /* 8 NSS not supported by FW */

      MTLK_BFIELD_SET(u8FlagsExt, STA_ADD_FLAGS_EXT_IS_EHT, MTLK_BFIELD_GET(info->flags, STA_FLAGS_11be));
      psUmiStaAdd->u8FlagsExt = u8FlagsExt;
    }
  }
#endif

  ILOG1_D("CID-%04x: UMI_STA_ADD", mtlk_vap_get_oid(nic->vap_handle));
  mtlk_dump(1, psUmiStaAdd, sizeof(UMI_STA_ADD), "dump of UMI_STA_ADD:");

  ILOG1_D("UMI_STA_ADD->u16SID:                %u", MAC_TO_HOST16(psUmiStaAdd->u16SID));
  ILOG1_D("UMI_STA_ADD->u8VapIndex:            %u", psUmiStaAdd->u8VapIndex);
  ILOG1_D("UMI_STA_ADD->u8SecondaryVapIndex:   %u", psUmiStaAdd->u8SecondaryVapIndex);
  ILOG1_D("UMI_STA_ADD->u8Status:              %u", psUmiStaAdd->u8Status);
  ILOG1_D("UMI_STA_ADD->u8ListenInterval:      %u", psUmiStaAdd->u8ListenInterval);
  ILOG1_D("UMI_STA_ADD->u8BSS_Coex_20_40:      %u", psUmiStaAdd->u8BSS_Coex_20_40);
  ILOG1_D("UMI_STA_ADD->u8UAPSD_Queues:        %u", psUmiStaAdd->u8UAPSD_Queues);
  ILOG1_D("UMI_STA_ADD->u8Max_SP:              %u", psUmiStaAdd->u8Max_SP);
  ILOG1_D("UMI_STA_ADD->u16AID:                %u", MAC_TO_HOST16(psUmiStaAdd->u16AID));
  ILOG1_Y("UMI_STA_ADD->sAddr:                 %Y", &psUmiStaAdd->sAddr);
  mtlk_dump(1, psUmiStaAdd->u8Rates, sizeof(psUmiStaAdd->u8Rates), "dump of UMI_STA_ADD->u8Rates:");
  ILOG1_D("UMI_STA_ADD->u16HT_Cap_Info:        %04X", MAC_TO_HOST16(psUmiStaAdd->u16HT_Cap_Info));
  ILOG1_D("UMI_STA_ADD->u8AMPDU_Param:         %u", psUmiStaAdd->u8AMPDU_Param);
  mtlk_dump(1, psUmiStaAdd->u8HT_MCS_Set, sizeof(psUmiStaAdd->u8HT_MCS_Set), "dump of UMI_STA_ADD->u8HT_MCS_Set:");
  ILOG1_D("UMI_STA_ADD->u8Rates_Length:        %u", psUmiStaAdd->u8Rates_Length);
  ILOG1_DD("UMI_STA_ADD->rssi:                 %02X(%d)", psUmiStaAdd->rssi, (int8)psUmiStaAdd->rssi);
  ILOG1_D("UMI_STA_ADD->u8Flags:               %02X", psUmiStaAdd->u8Flags);
  ILOG1_D("UMI_STA_ADD->u8FlagsExt:            %02X", psUmiStaAdd->u8FlagsExt);
  ILOG1_D("UMI_STA_ADD->u32VHT_Cap_Info:       %08X", MAC_TO_HOST32(psUmiStaAdd->u32VHT_Cap_Info));
  ILOG1_D("UMI_STA_ADD->u16HE_6ghz_Cap_Info:   %04X", MAC_TO_HOST16(psUmiStaAdd->u16HE_6ghz_Cap_Info));
  ILOG1_D("UMI_STA_ADD->transmitBfCapabilities:%08X", MAC_TO_HOST32(psUmiStaAdd->transmitBfCapabilities));
  ILOG1_D("UMI_STA_ADD->u8VHT_OperatingModeNotification:%02X", psUmiStaAdd->u8VHT_OperatingModeNotification);
  ILOG1_D("UMI_STA_ADD->u8HE_OperatingModeNotification:%02X", psUmiStaAdd->u8HE_OperatingModeNotification);
  ILOG1_D("UMI_STA_ADD->u8WDS_client_type:     %u", psUmiStaAdd->u8WDS_client_type);
  mtlk_dump(1, psUmiStaAdd->u32VHT_Mcs_Nss, sizeof(psUmiStaAdd->u32VHT_Mcs_Nss), "dump of UMI_STA_ADD->u8VHT_Mcs_Nss:");

  ILOG1_D("UMI_STA_ADD->b8WDS:                 %u", MTLK_BFIELD_GET(u8Flags, STA_ADD_FLAGS_WDS));
  ILOG1_D("UMI_STA_ADD->b8WME:                 %u", MTLK_BFIELD_GET(u8Flags, STA_ADD_FLAGS_WME));
  ILOG1_D("UMI_STA_ADD->b8MFP:                 %u", MTLK_BFIELD_GET(u8Flags, STA_ADD_FLAGS_MFP));
  ILOG1_D("UMI_STA_ADD->b8HT:                  %u", MTLK_BFIELD_GET(u8Flags, STA_ADD_FLAGS_IS_HT));
  ILOG1_D("UMI_STA_ADD->b8VHT:                 %u", MTLK_BFIELD_GET(u8Flags, STA_ADD_FLAGS_IS_VHT));
  ILOG1_D("UMI_STA_ADD->b8OMN_supported:       %u", MTLK_BFIELD_GET(u8Flags, STA_ADD_FLAGS_OMN));
  ILOG1_D("UMI_STA_ADD->b8OPER_MODE_NOTIF:     %u", MTLK_BFIELD_GET(u8Flags, STA_ADD_FLAGS_OPER_MODE_NOTIF_VALID));
  ILOG1_D("UMI_STA_ADD->b8PBAC:                %u", MTLK_BFIELD_GET(u8FlagsExt, STA_ADD_FLAGS_EXT_PBAC));
  ILOG1_D("UMI_STA_ADD->b8HE:                  %u", MTLK_BFIELD_GET(u8FlagsExt, STA_ADD_FLAGS_EXT_IS_HE));
  ILOG1_D("UMI_STA_ADD->b8HE_6GHZ:             %u", MTLK_BFIELD_GET(u8FlagsExt, STA_ADD_FLAGS_EXT_IS_HE_6GHZ));
  ILOG1_D("UMI_STA_ADD->b8EHT:                 %u", MTLK_BFIELD_GET(u8FlagsExt, STA_ADD_FLAGS_EXT_IS_EHT));
  ILOG1_D("UMI_STA_ADD->heExtSingleUserDisable: %u", psUmiStaAdd->heExtSingleUserDisable);

  mtlk_dump(1, psUmiStaAdd->u8HE_Mac_Phy_Cap_Info, sizeof(psUmiStaAdd->u8HE_Mac_Phy_Cap_Info), "dump of UMI_STA_ADD->u8HE_Mac_Phy_Cap_Info:");
  mtlk_dump(1, psUmiStaAdd->u8HE_Mcs_Nss, sizeof(psUmiStaAdd->u8HE_Mcs_Nss), "dump of UMI_STA_ADD->u8HE_Mcs_Nss:");
  mtlk_dump(1, psUmiStaAdd->u8HE_Ppe_Th, sizeof(psUmiStaAdd->u8HE_Ppe_Th), "dump of UMI_STA_ADD->u8HE_Ppe_Th:");

#ifdef MTLK_WAVE_700
  if (MTLK_BFIELD_GET(u8FlagsExt, STA_ADD_FLAGS_EXT_IS_EHT)) {
    mtlk_dump(1, psUmiStaAdd->u8EHT_Mac_Phy_Cap_Info, sizeof(psUmiStaAdd->u8EHT_Mac_Phy_Cap_Info), "dump of UMI_STA_ADD->u8EHT_Mac_Phy_Cap_Info:");
    mtlk_dump(1, psUmiStaAdd->u8EHT_Mcs_Nss, sizeof(psUmiStaAdd->u8EHT_Mcs_Nss), "dump of UMI_STA_ADD->u8EHT_Mcs_Nss:");
    mtlk_dump(1, psUmiStaAdd->u8EHT_Ppe_Th, sizeof(psUmiStaAdd->u8EHT_Ppe_Th), "dump of UMI_STA_ADD->u8EHT_Ppe_Th:");
    mtlk_dump(1, &mac80211_sta->deflink.eht_oper, sizeof(mac80211_sta->deflink.eht_oper), "dump of info->eht_oper_params:");
  }
#endif

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (res != MTLK_ERR_OK) {
    ELOG_DD("CID-%04x: Can't send UM_MAN_STA_ADD_REQ request to MAC (err=%d)",
            mtlk_vap_get_oid(nic->vap_handle), res);
    goto FINISH;
  }

  if (psUmiStaAdd->u8Status != UMI_OK) {
    WLOG_DYD("CID-%04x: Station %Y add failed in FW (status=%u)",
             mtlk_vap_get_oid(nic->vap_handle),
             mac80211_sta->addr,
             psUmiStaAdd->u8Status);
    res = MTLK_ERR_MAC;
    goto FINISH;
  }

  info->sid = MAC_TO_HOST16(psUmiStaAdd->u16SID);
  ILOG1_DYD("CID-%04x: Station %Y connected (SID = %u)",
            mtlk_vap_get_oid(nic->vap_handle), mac80211_sta->addr, info->sid);

#ifdef MTLK_WAVE_700
  if (wave_radio_is_gen7(radio)) {
    if ((band == MTLK_HW_BAND_6_GHZ) && (wave_wv_mac80211_sta_is_high_speed_supported(mac80211_sta))) {
       num_of_high_speed_sta = wave_radio_inc_high_speed_counter(radio);
       if ((wave_radio_get_rx_ant(radio) >= 4) && (num_of_high_speed_sta == 1)) {/* first high speed STA to be connected causes disabling of ASPM */
          hw = mtlk_vap_get_hw(nic->vap_handle);
          MTLK_ASSERT(hw);
          mtlk_hw_pci_disable_aspm(hw);
          ILOG0_V("Disable PCIE ASPM because of high speed add STA");
       }
    }
  }
#endif

FINISH:
  if (sta)
    mtlk_sta_decref(sta);

  if (man_entry) {
    mtlk_txmm_msg_cleanup(&man_msg);
  }

  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_get_unconnected_station (mtlk_handle_t hcore, const void* data, uint32 data_size)
{
#define SYNC_REQ_SCAN_TIME_MS 20
  mtlk_core_t *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  uint8 *addr;
  IEEE_ADDR *ieee_addr;
  struct vendor_unconnected_sta_req_data_internal *sta_req_data;
  struct intel_vendor_unconnected_sta sta_res_data;
  unsigned sta_req_data_size;
  mtlk_error_t res = MTLK_ERR_OK, res2;
  unsigned i;
  uint16 sid;
  struct mtlk_chan_def origianl_ccd;
  struct mtlk_chan_def cd;
  struct set_chan_param_data cpd;
  struct ieee80211_channel *c;
  BOOL paused_scan = FALSE;
  BOOL async_response = TRUE;
  wave_radio_t       *radio = wave_vap_radio_get(core->vap_handle);
  wv_mac80211_t      *mac80211 = wave_radio_mac80211_get(radio);
  mtlk_nbuf_t *nbuf_ndp = NULL;
  frame_head_t *wifi_header;
  mtlk_df_user_t *df_user;
  mtlk_vap_handle_t vap_handle;
  mtlk_core_t *caller_core, *sta_core;
  uint32 scan_flags;
  int sid_new;
  mtlk_hw_t *hw = NULL;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);

  sta_req_data = mtlk_clpb_enum_get_next(clpb, &sta_req_data_size);
  MTLK_CLPB_TRY(sta_req_data, sta_req_data_size)
    df_user = mtlk_df_user_from_ndev(sta_req_data->wdev->netdev);
    if (NULL == df_user) {
      res = MTLK_ERR_UNKNOWN;
      goto end;
    }

    vap_handle = mtlk_df_get_vap_handle(mtlk_df_user_get_df(df_user));
    caller_core = mtlk_vap_get_core(vap_handle);
    hw = mtlk_vap_get_hw(caller_core->vap_handle);

    memset(&cd, 0, sizeof(cd));
    memset(&sta_res_data, 0, sizeof(sta_res_data));
    async_response = (sta_req_data->req_type == NASTA_STATS_REQ_ASYNC) ? TRUE : FALSE;
    sta_res_data.addr = sta_req_data->addr;
    for (i = 0; i < MTLK_ARRAY_SIZE(sta_res_data.rssi); i++)
      sta_res_data.rssi[i] = MIN_RSSI;

    c = ieee80211_get_channel(sta_req_data->wdev->wiphy, sta_req_data->center_freq);
    if (!c) {
      ELOG_DD("CID-%04x: Getting channel structure for frequency %d MHz failed",
              mtlk_vap_get_oid(core->vap_handle), sta_req_data->center_freq);
      res = MTLK_ERR_PARAMS;
      goto end;
    }
    cd.chan.band = nlband2mtlkband(c->band);
    if (mtlk_core_is_band_supported(core, cd.chan.band) != MTLK_ERR_OK) {
      ELOG_DD("CID-%04x: HW does not support band %i",
              mtlk_vap_get_oid(core->vap_handle), cd.chan.band);
      res = MTLK_ERR_NOT_SUPPORTED;
      goto end;
    }

    scan_flags = wave_radio_scan_flags_get(radio);
    if (wave_radio_scan_is_running(radio)) {
      if (!(scan_flags & SDF_BACKGROUND)) {
        ELOG_V("Cannot change channels in the middle of a non-BG scan");
        res = MTLK_ERR_BUSY;
        goto end;
      } else if (!(scan_flags & SDF_BG_BREAK)) { /* background scan and not on a break at the moment */
        ILOG0_V("Background scan encountered, so pausing it first");
        if ((res = pause_or_prevent_scan(core)) != MTLK_ERR_OK) {
          ELOG_V("Scan is already paused/prevented, canceling the channel switch");
          goto end;
        }
        paused_scan = TRUE;
      } else {
        ILOG0_V("Background scan during its break encountered, so changing the channel");
      }
    }

    /* find station in stadb of any VAP */
    addr = sta_req_data->addr.au8Addr;
    ieee_addr = (IEEE_ADDR *)addr;
    if (mtlk_vap_manager_is_sta_connected(core, &sta_core, addr)) {
      WLOG_DYD("CID-%04x: station %Y already connected to CID-%04x",
        mtlk_vap_get_oid(caller_core->vap_handle), ieee_addr->au8Addr,
        mtlk_vap_get_oid(sta_core->vap_handle));
        res = MTLK_ERR_ALREADY_EXISTS;
        goto end;
    }

    if (wv_mac80211_has_sta_connections(mac80211)) {
      nbuf_ndp = mtlk_df_nbuf_alloc(sizeof(frame_head_t));
      if (!nbuf_ndp) {
        ELOG_D("CID-%04x: Unable to allocate buffer for Null Data Packet",
               mtlk_vap_get_oid(caller_core->vap_handle));
        res = MTLK_ERR_UNKNOWN;
        goto end;
      }

      wifi_header = (frame_head_t *) nbuf_ndp->data;
      memset(wifi_header, 0, sizeof(*wifi_header));
      wifi_header->frame_control = HOST_TO_WLAN16(IEEE80211_STYPE_NULLFUNC | IEEE80211_FTYPE_DATA);

      res = wv_mac80211_NDP_send_to_all_APs(mac80211, nbuf_ndp, TRUE, TRUE);
      if (res != MTLK_ERR_OK) {
        ELOG_DDS("CID-%04x: sending NDP failed with error %d (%s)",
                 mtlk_vap_get_oid(caller_core->vap_handle), res,
                 mtlk_get_error_text(res));
        if (res == MTLK_ERR_TIMEOUT)
          wv_mac80211_NDP_send_to_all_APs(mac80211, nbuf_ndp, FALSE, FALSE);
        goto end;
      }
    }

    /* get SID */
    res = core_cfg_internal_request_sid(caller_core, ieee_addr, &sid);
    if (res != MTLK_ERR_OK) {
         ELOG_D("CID-%04x: Can't send SID request to MAC due to the lack of MAN_MSG",
              mtlk_vap_get_oid(caller_core->vap_handle));
      goto end;
    }

    /* add station */
    {
      UMI_STA_ADD      *psUmiStaAdd;
      mtlk_txmm_msg_t   man_msg;
      mtlk_txmm_data_t *man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(caller_core->vap_handle), &res);
      if (!man_entry) {
        ELOG_D("CID-%04x: Can't send STA_ADD request to MAC due to the lack of MAN_MSG",
              mtlk_vap_get_oid(caller_core->vap_handle));
        res = MTLK_ERR_NO_RESOURCES;
        goto remove_sid;
      }
      man_entry->id           = UM_MAN_STA_ADD_REQ;
      man_entry->payload_size = sizeof(UMI_STA_ADD);
      psUmiStaAdd = (UMI_STA_ADD *)man_entry->payload;
      psUmiStaAdd->u16SID = HOST_TO_MAC16(sid);
      psUmiStaAdd->u8VapIndex       = mtlk_vap_get_id(caller_core->vap_handle);
      psUmiStaAdd->sAddr = *ieee_addr; /* Struct copy */
      psUmiStaAdd->u8Rates[0] = MTLK_CORE_WIDAN_UNCONNECTED_STATION_RATE;
      psUmiStaAdd->u8Rates_Length = sizeof(psUmiStaAdd->u8Rates[0]);
      MTLK_BFIELD_SET(psUmiStaAdd->u8Flags, STA_ADD_FLAGS_WME, 1);
      /* RSSI in ADD STA should be set to MIN value */
      psUmiStaAdd->rssi = MIN_RSSI - _mtlk_core_get_rrsi_offs(caller_core);
      res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
      if (res != MTLK_ERR_OK || psUmiStaAdd->u8Status != UMI_OK) {
        res = MTLK_ERR_MAC;
        goto remove_sid;
      }
      sid_new = MAC_TO_HOST16(psUmiStaAdd->u16SID);
      mtlk_txmm_msg_cleanup(&man_msg);
    }

    /* may not do set_chan unless there has been a VAP activated */
    if (core->is_stopped &&
      (res = mtlk_mbss_send_vap_activate(core, cd.chan.band)) != MTLK_ERR_OK)
    {
      ELOG_D("Could not activate the master VAP, err=%i", res);
      goto remove_sta;
    }

    /* save original channel definition for restoring it afterwards */
    origianl_ccd = *__wave_core_chandef_get(core);
    cd.center_freq1 = sta_req_data->cf1;
    cd.center_freq2 = sta_req_data->cf2;
    cd.width = nlcw2mtlkcw(sta_req_data->chan_width);
    cd.chan.center_freq = sta_req_data->center_freq;
    memset(&cpd, 0, sizeof(cpd));
    cpd.switch_type = ST_RSSI;
    cpd.bg_scan = core_cfg_channels_overlap(&origianl_ccd, &cd) ? 0 : SDF_RUNNING;
    cpd.sid = sid;
    res = core_cfg_send_set_chan(core, &cd, &cpd);
    if (res != MTLK_ERR_OK)
      goto remove_sta;

    if (async_response)
      mtlk_osal_msleep(WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_UNCONNECTED_STA_SCAN_TIME));
    else
      mtlk_osal_msleep(SYNC_REQ_SCAN_TIME_MS);

    memset(&cpd, 0, sizeof(cpd));
    cpd.switch_type = ST_NORMAL_AFTER_RSSI;
    cpd.sid = sid;
    res = core_cfg_send_set_chan(core, &origianl_ccd, &cpd);
    if (res != MTLK_ERR_OK)
      goto remove_sta;

    if (nbuf_ndp)
      wv_mac80211_NDP_send_to_all_APs(mac80211, nbuf_ndp, FALSE, FALSE);

    for(i = 0; i < MTLK_ARRAY_SIZE(cpd.rssi); i++) {
      cpd.rssi[i] = mtlk_stadb_apply_rssi_offset(cpd.rssi[i], _mtlk_core_get_rrsi_offs(core));
    }

    ILOG0_DDDD("get_unconnected_station RSSI -%04x -%04x -%04x -%04x",
              cpd.rssi[0],cpd.rssi[1],cpd.rssi[2],cpd.rssi[3]);

    /* Get MHI Statistics */
    mtlk_core_get_statistics(caller_core, mtlk_vap_get_hw(caller_core->vap_handle));
    mtlk_hw_get_rx_packets_and_bytes(hw, sid, &sta_res_data);

    wave_memcpy(sta_res_data.rssi, sizeof(sta_res_data.rssi), cpd.rssi, sizeof(cpd.rssi));

    sta_res_data.rate = MTLK_BITRATE_TO_MBPS(cpd.rate);

    MTLK_STATIC_ASSERT(ARRAY_SIZE(cpd.noise) == ARRAY_SIZE(cpd.rf_gain));
    MTLK_STATIC_ASSERT(ARRAY_SIZE(cpd.noise) == ARRAY_SIZE(sta_res_data.noise));

    for (i = 0; i < MTLK_ARRAY_SIZE(cpd.noise); i++) {
      sta_res_data.noise[i] = mtlk_hw_noise_phy_to_noise_dbm(mtlk_vap_get_hw(core->vap_handle),
                                                             cpd.noise[i]);
    }

remove_sta:
    wave_core_ap_stop_traffic(caller_core, sid_new, ieee_addr);
    wave_core_ap_remove_sta(caller_core, sid_new, ieee_addr);
remove_sid:
    core_remove_sid(caller_core, sid);
end:
    if (paused_scan) {
      ILOG0_V("Resuming the paused scan");
      resume_or_allow_scan(core);
    }
    if (nbuf_ndp)
      mtlk_df_nbuf_free(nbuf_ndp);
    
    if (async_response)
      res2 = wv_cfg80211_handle_get_unconnected_sta(sta_req_data->wdev,
        &sta_res_data, sizeof(sta_res_data));
  MTLK_CLPB_FINALLY(res)
    return async_response ? (res != MTLK_ERR_OK ? res : res2) :
      mtlk_clpb_push_res_data(clpb, res, &sta_res_data, sizeof(sta_res_data));
  MTLK_CLPB_END
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_set_amsdu_num_req (mtlk_core_t *core, uint32 htMsduInAmsdu,
                             uint32 vhtMsduInAmsdu, uint32 heMsduInAmsdu, uint32 ehtMsduInAmsdu)
{
  mtlk_error_t               res;
  mtlk_txmm_msg_t            man_msg;
  mtlk_txmm_data_t          *man_entry = NULL;
  UMI_MSDU_IN_AMSDU_CONFIG  *pAMSDUNum = NULL;
  mtlk_vap_handle_t          vap_handle = core->vap_handle;

  MTLK_ASSERT(vap_handle);
  ILOG1_DDD("CID-%04x:AMSDUNum FW request: Set %d %d", mtlk_vap_get_oid(vap_handle), htMsduInAmsdu, vhtMsduInAmsdu);

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(vap_handle), NULL);
  if (NULL == man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(vap_handle));
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_MAN_MSDU_IN_AMSDU_CONFIG_REQ;
  man_entry->payload_size = sizeof(UMI_MSDU_IN_AMSDU_CONFIG);

  pAMSDUNum = (UMI_MSDU_IN_AMSDU_CONFIG *)(man_entry->payload);
  pAMSDUNum->htMsduInAmsdu = (uint8)htMsduInAmsdu;
  pAMSDUNum->vhtMsduInAmsdu = (uint8)vhtMsduInAmsdu;
  pAMSDUNum->heMsduInAmsdu = (uint8)heMsduInAmsdu;
#ifdef MTLK_WAVE_700
  pAMSDUNum->ehtMsduInAmsdu = (uint8)ehtMsduInAmsdu;
#else
  pAMSDUNum->ehtMsduInAmsdu = 0;
#endif
  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res || UMI_OK != pAMSDUNum->Status) {
    ELOG_DDD("CID-%04x: Set AMSDU Enable failure, res=%i status=%hhu",
            mtlk_vap_get_oid(vap_handle), res, pAMSDUNum->Status);
    if (UMI_OK != pAMSDUNum->Status)
      res = MTLK_ERR_MAC;
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

mtlk_error_t __MTLK_IFUNC
core_remove_sid (mtlk_core_t *core, uint16 sid)
{
  mtlk_error_t      res = MTLK_ERR_OK;
  mtlk_txmm_msg_t   man_msg;
  mtlk_txmm_data_t *man_entry;
  mtlk_vap_handle_t vap_handle;
  wave_vap_id_t     vap_id_fw;
  unsigned          oid;
  mtlk_hw_t        *hw;
  UMI_REMOVE_SID   *umi_rem_sid;

  MTLK_ASSERT(core);
  if (!core)
    return MTLK_ERR_PARAMS;

  vap_handle = core->vap_handle;
  vap_id_fw = mtlk_vap_get_id_fw(vap_handle);
  oid = mtlk_vap_get_oid(vap_handle);
  hw  = mtlk_vap_get_hw(vap_handle);

  if (!wave_hw_is_sid_in_use(hw, sid, vap_id_fw)) {
    WLOG_DD("CID-%04x: SID %u is not in use", oid, sid);
    return MTLK_ERR_OK;
  }

  /* prepare the msg to the FW */
  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(vap_handle), &res);
  if (!man_entry) {
    ELOG_DD("CID-%04x: UM_MAN_REMOVE_SID_REQ init failed, err=%i", oid, res);
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_MAN_REMOVE_SID_REQ;
  man_entry->payload_size = sizeof(UMI_REMOVE_SID);
  umi_rem_sid = (UMI_REMOVE_SID *) man_entry->payload;
  umi_rem_sid->u16SID = HOST_TO_MAC16(sid);

  mtlk_dump(2, umi_rem_sid, sizeof(UMI_REMOVE_SID), "dump of UMI_REMOVE_SID before submitting to FW:");
  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  mtlk_dump(2, umi_rem_sid, sizeof(UMI_REMOVE_SID), "dump of UMI_REMOVE_SID after submitting to FW:");

  if (res != MTLK_ERR_OK) {
    ELOG_DD("CID-%04x: UM_MAN_REMOVE_SID_REQ send failed, err=%i", oid, res);
  } else if (umi_rem_sid->u8Status) {
    ELOG_DD("CID-%04x: UM_MAN_REMOVE_SID_REQ execution failed, status=%hhu",
            oid, umi_rem_sid->u8Status);
    res = MTLK_ERR_MAC;
  }

  if (res == MTLK_ERR_OK) {
    wave_hw_clear_sid_in_use(hw, sid, vap_id_fw, TRUE /* do warn */);
    ILOG1_DD("CID-%04x: SID %u is removed", oid, sid);
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

/*----------------------------------------------------------------------------*/

#ifdef WAVE_ENABLE_PIE
mtlk_error_t __MTLK_IFUNC
wave_core_pie_cfg_receive (mtlk_core_t *core, wave_pie_cfg_t *pie_cfg_params)
{
  mtlk_error_t       res;
  mtlk_txmm_msg_t    man_msg;
  mtlk_txmm_data_t  *man_entry = NULL;
  UMI_PIE_CONTROL   *pie_cfg = NULL;
  mtlk_vap_handle_t  vap_handle = core->vap_handle;
  int                i;

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(vap_handle), NULL);
  if (NULL == man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(vap_handle));
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_MAN_PIE_REQ;
  man_entry->payload_size = sizeof(UMI_PIE_CONTROL);

  pie_cfg = (UMI_PIE_CONTROL *)(man_entry->payload);
  pie_cfg->getSetOperation = API_GET_OPERATION;

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK == res) {
    pie_cfg_params->drop_thresh         = MAC_TO_HOST32(pie_cfg->dropThresh);
    pie_cfg_params->max_burst           = MAC_TO_HOST32(pie_cfg->maxBurst);
    pie_cfg_params->minQueueSizeDefault = MAC_TO_HOST32(pie_cfg->minQueueSizeDefault);
    pie_cfg_params->minQueueSizePs      = MAC_TO_HOST32(pie_cfg->minQueueSizePs);
    pie_cfg_params->weigth              = MAC_TO_HOST32(pie_cfg->weight);
    pie_cfg_params->decay_val           = MAC_TO_HOST32(pie_cfg->decayVal);
    pie_cfg_params->min_drop_prob       = MAC_TO_HOST32(pie_cfg->minDropProb);
    pie_cfg_params->max_drop_prob       = MAC_TO_HOST32(pie_cfg->maxDropProb);
    pie_cfg_params->t_update            = MAC_TO_HOST16(pie_cfg->tUpdate);
    pie_cfg_params->user_mode           = pie_cfg->userMode;
    for (i = 0; i < PIE_NUM_OF_CONVERSIONS; i++) {
      pie_cfg_params->pie_convert_p[i].drop_thresh = MAC_TO_HOST32(pie_cfg->pieConvertP[i].dropProbTresh);
      pie_cfg_params->pie_convert_p[i].denominator = MAC_TO_HOST32(pie_cfg->pieConvertP[i].denominator);
    }
    for (i = 0; i < ACCESS_CATEGORY_NUM; i++) {
      pie_cfg_params->dq_thresh[i]      = MAC_TO_HOST16(pie_cfg->dqThreshold[i]);
      pie_cfg_params->target_latency[i] = MAC_TO_HOST16(pie_cfg->targetLatency[i]);
      pie_cfg_params->alpha[i]          = MAC_TO_HOST16(pie_cfg->alpha[i]);
      pie_cfg_params->beta[i]           = MAC_TO_HOST16(pie_cfg->beta[i]);
    }
  } else {
    ELOG_DD("CID-%04x: Receiving UM_MAN_PIE_REQ failed, res %d", mtlk_vap_get_oid(vap_handle), res);
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

mtlk_error_t __MTLK_IFUNC
wave_core_pie_cfg_send (mtlk_core_t *core, wave_pie_cfg_t *pie_cfg_params)
{
  mtlk_error_t       res;
  mtlk_txmm_msg_t    man_msg;
  mtlk_txmm_data_t  *man_entry = NULL;
  UMI_PIE_CONTROL   *pie_cfg = NULL;
  mtlk_vap_handle_t  vap_handle = core->vap_handle;
  int                i;

  ILOG0_V("Sending UM_MAN_PIE_REQ");

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(vap_handle), NULL);
  if (NULL == man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(vap_handle));
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_MAN_PIE_REQ;
  man_entry->payload_size = sizeof(UMI_PIE_CONTROL);
  pie_cfg = (UMI_PIE_CONTROL *)man_entry->payload;
  pie_cfg->dropThresh              = HOST_TO_MAC32(pie_cfg_params->drop_thresh);
  pie_cfg->maxBurst                = HOST_TO_MAC32(pie_cfg_params->max_burst);
  pie_cfg->minQueueSizeDefault     = HOST_TO_MAC32(pie_cfg_params->minQueueSizeDefault);
  pie_cfg->minQueueSizePs          = HOST_TO_MAC32(pie_cfg_params->minQueueSizePs);
  pie_cfg->weight                  = HOST_TO_MAC32(pie_cfg_params->weigth);
  pie_cfg->decayVal                = HOST_TO_MAC32(pie_cfg_params->decay_val);
  pie_cfg->minDropProb             = HOST_TO_MAC32(pie_cfg_params->min_drop_prob);
  pie_cfg->maxDropProb             = HOST_TO_MAC32(pie_cfg_params->max_drop_prob);
  pie_cfg->tUpdate                 = HOST_TO_MAC16(pie_cfg_params->t_update);
  pie_cfg->userMode                = pie_cfg_params->user_mode;

  for (i = 0; i < PIE_NUM_OF_CONVERSIONS; i++) {
    pie_cfg->pieConvertP[i].dropProbTresh = HOST_TO_MAC32(pie_cfg_params->pie_convert_p[i].drop_thresh);
    pie_cfg->pieConvertP[i].denominator   = HOST_TO_MAC32(pie_cfg_params->pie_convert_p[i].denominator);
  }

  for (i = 0; i < ACCESS_CATEGORY_NUM; i++) {
    pie_cfg->dqThreshold[i]   = HOST_TO_MAC16(pie_cfg_params->dq_thresh[i]);
    pie_cfg->targetLatency[i] = HOST_TO_MAC16(pie_cfg_params->target_latency[i]);
    pie_cfg->alpha[i]         = HOST_TO_MAC16(pie_cfg_params->alpha[i]);
    pie_cfg->beta[i]          = HOST_TO_MAC16(pie_cfg_params->beta[i]);
  }

  mtlk_dump(0, pie_cfg, sizeof(*pie_cfg), "dump of UMI_PIE_CONTROL:");

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res) {
    ELOG_DD("CID-%04x: Sending UM_MAN_PIE_REQ failed (res = %d)", mtlk_vap_get_oid(vap_handle), res);
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

mtlk_error_t __MTLK_IFUNC
wave_core_get_aqm_sta_en (mtlk_core_t *core, wave_aqm_sta_en_t *wave_aqm_sta_en)
{
  mtlk_error_t       res;
  mtlk_txmm_msg_t    man_msg;
  mtlk_txmm_data_t  *man_entry = NULL;
  UMI_AQM_STA_EN    *umi_aqm_en = NULL;
  mtlk_vap_handle_t  vap_handle = core->vap_handle;

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(vap_handle), NULL);
  if (NULL == man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(vap_handle));
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_MAN_AQM_STA_EN_REQ;
  man_entry->payload_size = sizeof(UMI_AQM_STA_EN);

  umi_aqm_en = (UMI_AQM_STA_EN *)(man_entry->payload);
  umi_aqm_en->getSetOperation = API_GET_OPERATION;

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK == res) {
    wave_memcpy(wave_aqm_sta_en->aqmStaEnBitmap, sizeof(wave_aqm_sta_en->aqmStaEnBitmap), umi_aqm_en->aqmStaEnBitmap, sizeof(umi_aqm_en->aqmStaEnBitmap));
  } else {
    ELOG_DD("CID-%04x: Receiving UM_MAN_AQM_STA_EN_REQ failed, res %d", mtlk_vap_get_oid(vap_handle), res);
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

mtlk_error_t __MTLK_IFUNC
wave_core_set_aqm_sta_en (mtlk_core_t *core, wave_aqm_sta_en_t *wave_aqm_sta_en)
{
  mtlk_error_t       res;
  mtlk_txmm_msg_t    man_msg;
  mtlk_txmm_data_t  *man_entry = NULL;
  UMI_AQM_STA_EN    *umi_aqm_en = NULL;
  mtlk_vap_handle_t  vap_handle = core->vap_handle;

  ILOG1_DD("Sending UM_MAN_AQM_STA_EN_REQ: staId=%d enable=%d\n", wave_aqm_sta_en->staId, wave_aqm_sta_en->enable);

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(vap_handle), NULL);
  if (NULL == man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(vap_handle));
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_MAN_AQM_STA_EN_REQ;
  man_entry->payload_size = sizeof(UMI_AQM_STA_EN);
  umi_aqm_en = (UMI_AQM_STA_EN *)man_entry->payload;
  umi_aqm_en->staId = wave_aqm_sta_en->staId;
  umi_aqm_en->enable = wave_aqm_sta_en->enable;

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res) {
    ELOG_DD("CID-%04x: Sending UMI_AQM_STA_EN failed (res = %d)", mtlk_vap_get_oid(vap_handle), res);
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}
#endif /* WAVE_ENABLE_PIE */

/*----------------------------------------------------------------------------*/

/* This function should be called from master serializer context */
mtlk_error_t __MTLK_IFUNC
mtlk_core_set_ap_beacon_info_by_params (mtlk_core_t *core, struct mtlk_beacon_info_parameters *params)
{
  mtlk_error_t       res = MTLK_ERR_OK;
  mtlk_txmm_msg_t    man_msg;
  mtlk_txmm_data_t  *man_entry;
  mtlk_pdb_t        *param_db_core = mtlk_vap_get_param_db(core->vap_handle);
  UMI_SET_AP_BEACON_INFO *req;

  /* prepare msg for the FW */
  if (!(man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(core->vap_handle), &res))) {
    ELOG_DD("CID-%04x: UM_MAN_SET_AP_BEACON_INFO_REQ init failed, err=%i",
            mtlk_vap_get_oid(core->vap_handle), res);
    res = MTLK_ERR_NO_RESOURCES;
    goto end;
  }

  man_entry->id = UM_MAN_SET_AP_BEACON_INFO_REQ;
  man_entry->payload_size = sizeof(*req);
  req = (UMI_SET_AP_BEACON_INFO *) man_entry->payload;
  req->vapId = params->vap_index;
  req->beaconInterval = HOST_TO_MAC16(params->beacon_int);
  req->dtimInterval = HOST_TO_MAC16(params->dtim_period);
  ILOG1_DD("dtim_period: %d beacon_int: %d", params->dtim_period, params->beacon_int);
  req->mbssIdNumOfVapsInGroup = (uint8)wave_pdb_get_int(param_db_core, PARAM_DB_CORE_MBSSID_NUM_VAPS_IN_GROUP);
  ILOG2_D("UMI_SET_AP_BEACON_INFO->mbssIdNumOfVapsInGroup:%d", req->mbssIdNumOfVapsInGroup);

  mtlk_dump(2, req, sizeof(*req), "dump of UMI_SET_AP_BEACON_INFO:");

  /* send message to FW */
  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (res != MTLK_ERR_OK || UMI_OK != req->Status) {
    ELOG_DDD("CID-%04x: UM_MAN_SET_AP_BEACON_INFO_REQ send failed, res=%i status=%hhu",
             mtlk_vap_get_oid(core->vap_handle), res, req->Status);
    if (UMI_OK != req->Status)
      res = MTLK_ERR_MAC;
  }

  if (NULL != man_entry)
    mtlk_txmm_msg_cleanup(&man_msg);

end:
  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_get_enc_ext_cfg (mtlk_handle_t hcore, const void* data, uint32 data_size)
{
  mtlk_error_t       res = MTLK_ERR_OK;
  mtlk_txmm_msg_t    man_msg;
  mtlk_txmm_data_t  *man_entry = NULL;
  mtlk_clpb_t *clpb = *(mtlk_clpb_t **) data;
  unsigned clpb_size;
  u8* key_idx = NULL;
  mtlk_core_t       *nic = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_txmm_t       *txmm = mtlk_vap_get_txmm(nic->vap_handle);
  UMI_GROUP_PN      *umi_gpn = NULL;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);

  if (mtlk_vap_is_ap(nic->vap_handle) && !wave_core_sync_hostapd_done_get(nic)) {
    ILOG1_D("CID-%04x: HOSTAPD STA database is not synced", mtlk_vap_get_oid(nic->vap_handle));
    res = MTLK_ERR_NOT_READY;
    goto FINISH;
  }

  if (0 != (mtlk_core_get_net_state(nic) & (NET_STATE_HALTED | NET_STATE_IDLE))) {
    ILOG1_D("CID-%04x: Invalid card state - request rejected", mtlk_vap_get_oid(nic->vap_handle));
    res = MTLK_ERR_NOT_READY;
    goto FINISH;
  }

  if (mtlk_core_scan_is_running(nic)) {
    ILOG1_D("CID-%04x: Can't get WEP configuration - scan in progress", mtlk_vap_get_oid(nic->vap_handle));
    res = MTLK_ERR_NOT_READY;
    goto FINISH;
  }

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, txmm, NULL);
  if (NULL == man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(nic->vap_handle));
    res = MTLK_ERR_NO_RESOURCES;
    goto FINISH;
  }

  /* In case we do not have a cipher it may be junk since it was not set.
  *, we cannot use UM_MAN_GET_GROUP_PN_REQ when there is none */
  if((nic->slow_ctx->wep_enabled == 0) && (nic->slow_ctx->rsn_enabled == 0)) {
    goto FINISH;
  }

  man_entry->id           = UM_MAN_GET_GROUP_PN_REQ;
  man_entry->payload_size = sizeof(UMI_GROUP_PN);
  umi_gpn = (UMI_GROUP_PN*)man_entry->payload;

  key_idx = mtlk_clpb_enum_get_next(clpb, &clpb_size);

  if (*key_idx == 0 || clpb_size != sizeof(*key_idx)) {
    ELOG_D("CID-%04x: Invalid key index in CLPB", mtlk_vap_get_oid(nic->vap_handle));
    res = MTLK_ERR_PARAMS;
    goto FINISH;
  }

  if (*key_idx == WAVE_KEY_IDX6_BIGTK){
    umi_gpn->keyType = UMI_RSN_MGMT_BEACON_GROUP_KEY;
  }
  else {
    umi_gpn->keyType = UMI_RSN_GROUP_KEY;
  }

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res) {
    ELOG_D("CID-%04x: Timeout expired while waiting for CFM from MAC", mtlk_vap_get_oid(nic->vap_handle));
    goto FINISH;
  }

  if (UMI_OK != MAC_TO_HOST16(umi_gpn->u16Status)) {
    ELOG_DD("CID-%04x: MAC returned error (status=%hu) for UM_MAN_GET_GROUP_PN_REQ", mtlk_vap_get_oid(nic->vap_handle),
            MAC_TO_HOST16(umi_gpn->u16Status));
    res = MTLK_ERR_MAC;
    goto FINISH;
  }

  mtlk_dump(3, umi_gpn->au8TxSeqNum, UMI_RSN_SEQ_NUM_LEN, "GROUP RSC");

  MTLK_STATIC_ASSERT(sizeof(nic->slow_ctx->seq) >= UMI_RSN_SEQ_NUM_LEN);
  wave_memcpy(nic->slow_ctx->seq, sizeof(nic->slow_ctx->seq), umi_gpn->au8TxSeqNum, UMI_RSN_SEQ_NUM_LEN);

FINISH:
  if (man_entry) {
    mtlk_txmm_msg_cleanup(&man_msg);
  }
  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_set_enc_ext_cfg (mtlk_handle_t hcore, const void* data, uint32 data_size)
{
  mtlk_error_t       res = MTLK_ERR_OK;
  mtlk_txmm_msg_t    man_msg;
  mtlk_txmm_data_t  *man_entry = NULL;
  uint16             key_indx;
  mtlk_core_t       *core = HANDLE_T_PTR(mtlk_core_t, hcore);
  mtlk_clpb_t       *clpb = *(mtlk_clpb_t **) data;
  uint32             size;
  mtlk_txmm_t       *txmm = mtlk_vap_get_txmm(core->vap_handle);
  UMI_SET_KEY       *umi_key;
  uint16             alg_type = IW_ENCODE_ALG_NONE;
  uint16             key_len = 0;
  sta_entry         *sta = NULL;
  mtlk_core_ui_encext_cfg_t *encext_cfg;

  MTLK_ASSERT(sizeof(mtlk_clpb_t*) == data_size);

  if ((mtlk_core_get_net_state(core) & (NET_STATE_ACTIVATING | NET_STATE_CONNECTED)) == 0) {
    ILOG1_D("CID-%04x: Invalid card state - request rejected", mtlk_vap_get_oid(core->vap_handle));
    res = MTLK_ERR_NOT_READY;
    goto FINISH;
  }

  if (mtlk_core_is_stopping(core)) {
    ILOG1_D("CID-%04x: Can't set ENC_EXT configuration - core is stopping", mtlk_vap_get_oid(core->vap_handle));
    res = MTLK_ERR_NOT_READY;
    goto FINISH;
  }

  if (core->waiting_for_eapol_ack) {
    int wait_res = mtlk_osal_event_wait(&core->eapol_acked, 50);
    if (wait_res != MTLK_ERR_OK) /* try setting key anyway */
      WLOG_DD("CID-%04x: Wait for eapol ack failed (wait_res=%d)", mtlk_vap_get_oid(core->vap_handle), wait_res);
    core->waiting_for_eapol_ack = FALSE;
  }

  encext_cfg = mtlk_clpb_enum_get_next(clpb, &size);
  if ( (NULL == encext_cfg) || (sizeof(*encext_cfg) != size) ) {
    ELOG_D("CID-%04x: Failed to get ENC_EXT configuration parameters from CLPB", mtlk_vap_get_oid(core->vap_handle));
    res = MTLK_ERR_UNKNOWN;
    goto FINISH;
  }

  /* Prepare UMI message */
  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, txmm, NULL);
  if (!man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(core->vap_handle));
    res = MTLK_ERR_NO_RESOURCES;
    goto FINISH;
  }

  man_entry->id           = UM_MAN_SET_KEY_REQ;
  man_entry->payload_size = sizeof(*umi_key);
  umi_key = (UMI_SET_KEY*)man_entry->payload;

  key_len = encext_cfg->key_len;
  if(0 == key_len) {
    ELOG_D("CID-%04x: No key is set", mtlk_vap_get_oid(core->vap_handle));
    res = MTLK_ERR_PARAMS;
    goto FINISH;
  }

  key_indx = encext_cfg->key_idx;

  /* Set Ciper Suite and security params */
  alg_type = encext_cfg->alg_type;
  if (alg_type == IW_ENCODE_ALG_WEP) {
    core->slow_ctx->wep_enabled = 1;
    core->slow_ctx->rsn_enabled = 0;
  } else {
    core->slow_ctx->wep_enabled = 0;
    core->slow_ctx->rsn_enabled = 1;
  }
  switch (alg_type) {
    case IW_ENCODE_ALG_WEP:
      if(WAVE_WEP_KEY_WEP2_LENGTH == key_len) { /* 104 bit */
        umi_key->u16CipherSuite = HOST_TO_MAC16(SECURITY_TYPE_SECD_WEP104);
      } else if (WAVE_WEP_KEY_WEP1_LENGTH == key_len) { /* 40 bit */
        umi_key->u16CipherSuite = HOST_TO_MAC16(SECURITY_TYPE_SECD_WEP40);
      } else {
        ELOG_DD("CID-%04x: Wrong WEP key lenght %d", mtlk_vap_get_oid(core->vap_handle), key_len);
        res = MTLK_ERR_PARAMS;
        goto FINISH;
      }
      break;
    case IW_ENCODE_ALG_TKIP:
      umi_key->u16CipherSuite = HOST_TO_MAC16(SECURITY_TYPE_SECD_TKIP);
      break;
    case IW_ENCODE_ALG_CCMP:
      umi_key->u16CipherSuite = HOST_TO_MAC16(SECURITY_TYPE_SECD_CCMP);
      break;
    case IW_ENCODE_ALG_AES_CMAC:
      umi_key->u16CipherSuite = HOST_TO_MAC16(SECURITY_TYPE_SECD_BIP_CMAC128);
      break;
    case IW_ENCODE_ALG_GCMP:
      umi_key->u16CipherSuite = HOST_TO_MAC16(SECURITY_TYPE_SECD_GCMP128);
      break;
    case IW_ENCODE_ALG_GCMP_256:
      umi_key->u16CipherSuite = HOST_TO_MAC16(SECURITY_TYPE_SECD_GCMP256);
      break;
    case IW_ENCODE_ALG_BIP_GMAC_128:
      umi_key->u16CipherSuite = HOST_TO_MAC16(SECURITY_TYPE_SECD_BIP_GMAC128);
      break;
    case IW_ENCODE_ALG_BIP_GMAC_256:
      umi_key->u16CipherSuite = HOST_TO_MAC16(SECURITY_TYPE_SECD_BIP_GMAC256);
      break;
    default:
      ELOG_D("CID-%04x: Unknown CiperSuite", mtlk_vap_get_oid(core->vap_handle));
      res = MTLK_ERR_PARAMS;
      goto FINISH;
  }

  /* Set key type */
  if (mtlk_osal_eth_is_broadcast(encext_cfg->sta_addr.au8Addr)) {
    if (IS_BCN_KEY_IDX(encext_cfg->key_idx)) {
      umi_key->u16KeyType = HOST_TO_MAC16(UMI_RSN_MGMT_BEACON_GROUP_KEY);
      core->slow_ctx->bcn_prot_cipher = alg_type;
      core->slow_ctx->default_bcn_key = encext_cfg->key_idx;
    } else {
      umi_key->u16KeyType = HOST_TO_MAC16(UMI_RSN_GROUP_KEY);
      core->slow_ctx->group_cipher = alg_type;
    }
    umi_key->u16Sid = HOST_TO_MAC16(wave_core_get_all_sta_sid(core));
  } else {
    /* Check STA availability */
    if (NULL == encext_cfg->sta) { /*if we have peer ap address we need to search for it */
      sta = mtlk_stadb_find_sta(&core->slow_ctx->stadb, encext_cfg->sta_addr.au8Addr);
    } else {
      sta = wv_ieee80211_get_sta(encext_cfg->sta);
    }
    if (NULL == sta) {
      ILOG1_Y("There is no connection with %Y", encext_cfg->sta_addr.au8Addr);
      res = MTLK_ERR_PARAMS;
      goto FINISH;
    }

    umi_key->u16Sid = HOST_TO_MAC16(mtlk_sta_get_sid(sta));
    umi_key->u16KeyType = HOST_TO_MAC16(UMI_RSN_PAIRWISE_KEY);
  }

  /* The key has been copied into au8Tk1 array with UMI_RSN_TK1_LEN size.
   * But key can have UMI_RSN_TK1_LEN+UMI_RSN_TK2_LEN size - so
   * actually second part of key is copied into au8Tk2 array */
  wave_memcpy(umi_key->au8Tk, sizeof(umi_key->au8Tk), encext_cfg->key, key_len);

  if(sta) {
    mtlk_sta_set_cipher(sta, alg_type);
  }

  mtlk_core_set_umi_key_au8TxSeqNum(umi_key, core);
  umi_key->u16KeyIndex = HOST_TO_MAC16(key_indx);

  wave_memcpy(umi_key->au8RxSeqNum, sizeof(umi_key->au8RxSeqNum), encext_cfg->rx_seq, sizeof(encext_cfg->rx_seq));

  ILOG1_DDDD("UMI_SET_KEY SID:0x%x, Type:0x%x, Suite:0x%x, Index:%d",
             MAC_TO_HOST16(umi_key->u16Sid), MAC_TO_HOST16(umi_key->u16KeyType),
             MAC_TO_HOST16(umi_key->u16CipherSuite), MAC_TO_HOST16(umi_key->u16KeyIndex));
  mtlk_dump(2, umi_key->au8RxSeqNum, sizeof(umi_key->au8RxSeqNum), "RxSeqNum");
  mtlk_dump(2, umi_key->au8TxSeqNum, sizeof(umi_key->au8TxSeqNum), "TxSeqNum");
  mtlk_dump(2, umi_key->au8Tk, key_len, "KEY:");

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res || UMI_OK != MAC_TO_HOST16(umi_key->u16Status)) {
    ELOG_DDD("CID-%04x: Set UM_MAN_SET_KEY_REQ failed, res=%i status=%hu",
            mtlk_vap_get_oid(core->vap_handle), res, MAC_TO_HOST16(umi_key->u16Status));
    if (UMI_OK != MAC_TO_HOST16(umi_key->u16Status))
      res = MTLK_ERR_MAC;
    goto FINISH;
  }

  /* Store the key */
  wave_memcpy(core->slow_ctx->keys[key_indx].key, sizeof(core->slow_ctx->keys[key_indx].key),
      encext_cfg->key, key_len);
  core->slow_ctx->keys[key_indx].key_len = key_len;
  memset(core->slow_ctx->keys[key_indx].seq, 0, CORE_KEY_SEQ_LEN);
  wave_memcpy(core->slow_ctx->keys[key_indx].seq, sizeof(core->slow_ctx->keys[key_indx].seq),
      encext_cfg->rx_seq, sizeof(encext_cfg->rx_seq));
  core->slow_ctx->keys[key_indx].seq_len =  CORE_KEY_SEQ_LEN;
  core->slow_ctx->keys[key_indx].cipher =  MAC_TO_HOST16(umi_key->u16CipherSuite);

FINISH:
  if (NULL != sta) {
    mtlk_sta_decref(sta); /* De-reference of find */
  }
  if (NULL != man_entry) {
    mtlk_txmm_msg_cleanup(&man_msg);
  }
  return res;
}

mtlk_error_t __MTLK_IFUNC
wave_core_set_umi_key (mtlk_core_t *core, int key_index)
{
  mtlk_error_t       res;
  mtlk_txmm_msg_t    man_msg;
  mtlk_txmm_data_t  *man_entry = NULL;
  mtlk_txmm_t       *txmm = mtlk_vap_get_txmm(core->vap_handle);
  UMI_SET_KEY       *umi_key;
  uint16             alg_type;
  uint16             key_len;

  if ((mtlk_core_get_net_state(core) & (NET_STATE_READY | NET_STATE_ACTIVATING | NET_STATE_CONNECTED)) == 0) {
    ILOG1_D("CID-%04x: Invalid card state - request rejected", mtlk_vap_get_oid(core->vap_handle));
    res = MTLK_ERR_NOT_READY;
    goto FINISH;
  }

  if (mtlk_core_is_stopping(core)) {
    ILOG1_D("CID-%04x: Can't set ENC_EXT configuration - core is stopping", mtlk_vap_get_oid(core->vap_handle));
    res = MTLK_ERR_NOT_READY;
    goto FINISH;
  }

  alg_type = IS_BCN_KEY_IDX(key_index) ? core->slow_ctx->bcn_prot_cipher : core->slow_ctx->group_cipher;
  if ((key_index == 0) && (alg_type != IW_ENCODE_ALG_WEP)) {
    ILOG2_DDD("CID-%04x: Ignore Key %d, if alg_type=%d is not WEP",
        mtlk_vap_get_oid(core->vap_handle), key_index, alg_type);
    res = MTLK_ERR_OK;
    goto FINISH;
  } else if (IS_BCN_KEY_IDX(key_index) && (key_index != core->slow_ctx->default_bcn_key)) {
    ILOG2_DDD("CID-%04x: Ignore Key %d for non default bcn index with alg %d",
        mtlk_vap_get_oid(core->vap_handle), key_index, alg_type);
    res = MTLK_ERR_OK;
    goto FINISH;
  }

  /* Prepare UMI message */
  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, txmm, NULL);
  if (!man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(core->vap_handle));
    res = MTLK_ERR_NO_RESOURCES;
    goto FINISH;
  }

  man_entry->id           = UM_MAN_SET_KEY_REQ;
  man_entry->payload_size = sizeof(*umi_key);
  umi_key = (UMI_SET_KEY*)man_entry->payload;
  memset(umi_key, 0, sizeof(*umi_key));

  key_len = core->slow_ctx->keys[key_index].key_len;
  if( 0 == key_len) {
    ILOG2_DD("CID-%04x: Key %d is not set", mtlk_vap_get_oid(core->vap_handle), key_index);
    res = MTLK_ERR_OK;
    goto FINISH;
  }

  /* Set Ciper Suite */
  switch (alg_type) {
    case IW_ENCODE_ALG_WEP:
      if(WAVE_WEP_KEY_WEP2_LENGTH == key_len) { /* 104 bit */
        umi_key->u16CipherSuite = HOST_TO_MAC16(SECURITY_TYPE_SECD_WEP104);
      } else if (WAVE_WEP_KEY_WEP1_LENGTH == key_len) { /* 40 bit */
        umi_key->u16CipherSuite = HOST_TO_MAC16(SECURITY_TYPE_SECD_WEP40);
      } else {
        ELOG_DD("CID-%04x: Wrong WEP key length %d", mtlk_vap_get_oid(core->vap_handle), key_len);
        res = MTLK_ERR_PARAMS;
        goto FINISH;
      }
      break;
    case IW_ENCODE_ALG_TKIP:
      umi_key->u16CipherSuite = HOST_TO_MAC16(SECURITY_TYPE_SECD_TKIP);
      break;
    case IW_ENCODE_ALG_CCMP:
      umi_key->u16CipherSuite = HOST_TO_MAC16(SECURITY_TYPE_SECD_CCMP);
      break;
    case IW_ENCODE_ALG_AES_CMAC:
      umi_key->u16CipherSuite = HOST_TO_MAC16(SECURITY_TYPE_SECD_BIP_CMAC128);
      break;
    case IW_ENCODE_ALG_GCMP:
      umi_key->u16CipherSuite = HOST_TO_MAC16(SECURITY_TYPE_SECD_GCMP128);
      break;
    case IW_ENCODE_ALG_GCMP_256:
      umi_key->u16CipherSuite = HOST_TO_MAC16(SECURITY_TYPE_SECD_GCMP256);
      break;
    case IW_ENCODE_ALG_BIP_GMAC_128:
      umi_key->u16CipherSuite = HOST_TO_MAC16(SECURITY_TYPE_SECD_BIP_GMAC128);
      break;
    case IW_ENCODE_ALG_BIP_GMAC_256:
      umi_key->u16CipherSuite = HOST_TO_MAC16(SECURITY_TYPE_SECD_BIP_GMAC256);
      break;
    default:
      ELOG_D("CID-%04x: Unknown CiperSuite", mtlk_vap_get_oid(core->vap_handle));
      res = MTLK_ERR_PARAMS;
      goto FINISH;
  }
  umi_key->u16KeyType = IS_BCN_KEY_IDX(key_index) ? HOST_TO_MAC16(UMI_RSN_MGMT_BEACON_GROUP_KEY) : HOST_TO_MAC16(UMI_RSN_GROUP_KEY);
  umi_key->u16Sid = HOST_TO_MAC16(wave_core_get_all_sta_sid(core));

  /* The key has been copied into au8Tk1 array with UMI_RSN_TK1_LEN size.
   * But key can have UMI_RSN_TK1_LEN+UMI_RSN_TK2_LEN size - so
   * actually second part of key is copied into au8Tk2 array */
  wave_memcpy(umi_key->au8Tk, sizeof(umi_key->au8Tk),
         core->slow_ctx->keys[key_index].key,
         core->slow_ctx->keys[key_index].key_len);

  mtlk_core_set_umi_key_au8TxSeqNum(umi_key, core);
  umi_key->u16KeyIndex = HOST_TO_MAC16(key_index);

  ILOG1_DDDD("UMI_SET_KEY SID:0x%x, Type:0x%x, Suite:0x%x, Index:%d",
             MAC_TO_HOST16(umi_key->u16Sid), MAC_TO_HOST16(umi_key->u16KeyType),
             MAC_TO_HOST16(umi_key->u16CipherSuite), MAC_TO_HOST16(umi_key->u16KeyIndex));
  mtlk_dump(2, umi_key->au8RxSeqNum, sizeof(umi_key->au8RxSeqNum), "RxSeqNum");
  mtlk_dump(2, umi_key->au8TxSeqNum, sizeof(umi_key->au8TxSeqNum), "TxSeqNum");
  mtlk_dump(2, umi_key->au8Tk, key_len, "KEY:");

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res || UMI_OK  != MAC_TO_HOST16(umi_key->u16Status)) {
    ELOG_DDD("CID-%04x: Set UM_MAN_SET_KEY_REQ failed, res=%i status=%hu", mtlk_vap_get_oid(core->vap_handle), res,
            MAC_TO_HOST16(umi_key->u16Status));
    if (UMI_OK  != MAC_TO_HOST16(umi_key->u16Status))
      res = MTLK_ERR_MAC;
    goto FINISH;
  }

FINISH:
  if (NULL != man_entry) {
    mtlk_txmm_msg_cleanup(&man_msg);
  }
  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_mcast_notify_fw (mtlk_vap_handle_t vap_handle, int action, int sta_id, int grp_id)
{
  mtlk_error_t           res = MTLK_ERR_OK;
  mtlk_txmm_msg_t        man_msg;
  mtlk_txmm_data_t      *man_entry = NULL;
  UMI_MULTICAST_ACTION  *pMulticastAction = NULL;

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(vap_handle), &res);
  if (NULL == man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(vap_handle));
    res = MTLK_ERR_NO_RESOURCES;
    goto FINISH;
  }

  man_entry->id = UM_MAN_MULTICAST_ACTION_REQ;
  man_entry->payload_size = sizeof(UMI_MULTICAST_ACTION);

  pMulticastAction = (UMI_MULTICAST_ACTION *) man_entry->payload;
  pMulticastAction->u8Action = (action == MTLK_MC_STA_JOIN_GROUP) ? ADD_STA_TO_MULTICAST_GROUP : REMOVE_STA_FROM_MULTICAST_GROUP;
  pMulticastAction->u16StaID = HOST_TO_MAC16((uint16)sta_id);
  pMulticastAction->u8GroupID = grp_id;

  ILOG1_DSD("Multicast FW action request: STA (SID=%d) %s group %d", sta_id, (action == MTLK_MC_STA_JOIN_GROUP) ? "joined" : "leaving", grp_id);
  mtlk_dump(1, pMulticastAction, sizeof(UMI_MULTICAST_ACTION), "dump of UMI_MULTICAST_ACTION:");

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res || UMI_OK != pMulticastAction->Status) {
    ELOG_DDD("CID-%04x: Multicast Action sending failure, res=%i status=%hhu",
            mtlk_vap_get_oid(vap_handle), res, pMulticastAction->Status);
    if (UMI_OK != pMulticastAction->Status)
      res = MTLK_ERR_MAC;
    goto FINISH;
  }

FINISH:
  if (man_entry) {
    mtlk_txmm_msg_cleanup(&man_msg);
  }
  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_receive_qamplus_mode (mtlk_core_t *master_core, uint8 *qamplus_mode)
{
  mtlk_error_t           res = MTLK_ERR_OK;
  mtlk_txmm_msg_t        man_msg;
  mtlk_txmm_data_t      *man_entry;
  UMI_QAMPLUS_ACTIVATE  *mac_msg;

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(master_core->vap_handle), NULL);
  if (!man_entry) {
    ELOG_D("CID-%04x: Can not get TXMM slot", mtlk_vap_get_oid(master_core->vap_handle));
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_MAN_QAMPLUS_ACTIVATE_REQ;
  man_entry->payload_size = sizeof(UMI_QAMPLUS_ACTIVATE);
  mac_msg = (UMI_QAMPLUS_ACTIVATE *)man_entry->payload;
  mac_msg->getSetOperation = API_GET_OPERATION;

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK == res && UMI_OK == mac_msg->Status) {
      *qamplus_mode = mac_msg->enableQAMplus;
  } else {
    ELOG_DDD("CID-%04x: Receive UM_MAN_QAMPLUS_ACTIVATE_REQ failed, res=%d status=%hhu",
            mtlk_vap_get_oid(master_core->vap_handle), res, mac_msg->Status);
    if (UMI_OK != mac_msg->Status)
      res = MTLK_ERR_MAC;
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_qamplus_mode_req (mtlk_core_t *master_core, const uint32 qamplus_mode)
{
  mtlk_error_t           res = MTLK_ERR_OK;
  mtlk_txmm_msg_t        man_msg;
  mtlk_txmm_data_t      *man_entry;
  UMI_QAMPLUS_ACTIVATE  *mac_msg;

  ILOG2_DD("CID-%04x: QAMplus mode: %u",
    mtlk_vap_get_oid(master_core->vap_handle), qamplus_mode);

  /* allocate a new message */
  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(master_core->vap_handle), NULL);
  if (!man_entry) {
    ELOG_D("CID-%04x: Can not get TXMM slot", mtlk_vap_get_oid(master_core->vap_handle));
    return MTLK_ERR_NO_RESOURCES;
  }

  /* fill the message data */
  man_entry->id = UM_MAN_QAMPLUS_ACTIVATE_REQ;
  man_entry->payload_size = sizeof(UMI_QAMPLUS_ACTIVATE);
  mac_msg = (UMI_QAMPLUS_ACTIVATE *)man_entry->payload;
  mac_msg->enableQAMplus = (uint8)qamplus_mode;

  /* send the message to FW */
  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res || UMI_OK != mac_msg->Status) {
    ELOG_DDD("CID-%04x: Set UM_MAN_QAMPLUS_ACTIVATE_REQ failed, res=%d status=%hhu",
            mtlk_vap_get_oid(master_core->vap_handle), res, mac_msg->Status);
    if (UMI_OK != mac_msg->Status)
      res = MTLK_ERR_MAC;
  }
  /* cleanup the message */
  mtlk_txmm_msg_cleanup(&man_msg);

  return res;
}

mtlk_error_t __MTLK_IFUNC
wave_core_recovery_cfg_change_bss (mtlk_core_t *core)
{
  mtlk_error_t       res;
  mtlk_txmm_msg_t    man_msg;
  mtlk_txmm_data_t  *man_entry;
  UMI_SET_BSS *req;
  uint8 vap_id = mtlk_vap_get_id(core->vap_handle);
  mtlk_pdb_t *param_db_core = mtlk_vap_get_param_db(core->vap_handle);
  struct nic *master_core = mtlk_core_get_master(core->vap_handle);
  struct mtlk_chan_def *current_chandef = __wave_core_chandef_get(master_core);
  wave_radio_t *radio = wave_vap_radio_get(core->vap_handle);
  mtlk_hw_band_e band = wave_radio_band_get(radio);

  /* prepare msg for the FW */
  if (!(man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(core->vap_handle), &res))) {
    ELOG_DD("CID-%04x: UM_BSS_SET_BSS_REQ init failed, err=%i", mtlk_vap_get_oid(core->vap_handle), res);
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_BSS_SET_BSS_REQ;
  man_entry->payload_size = sizeof(*req);
  req = (UMI_SET_BSS *) man_entry->payload;
  req->vapId = vap_id;

  ILOG4_V("Dealing w/ protection");
  req->protectionMode = MTLK_CORE_PDB_GET_INT(core, PARAM_DB_CORE_HT_PROTECTION);

  ILOG4_V("Dealing w/ short slot");
  req->slotTime = WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_SHORT_SLOT);
  req->useShortPreamble = WAVE_RADIO_PDB_GET_INT(radio, PARAM_DB_RADIO_SHORT_PREAMBLE);

  ILOG4_V("Dealing w/ basic rates");
  req->u8Rates_Length = (uint8)wave_core_param_db_basic_rates_get(core, current_chandef->chan.band,
                                                                  req->u8Rates, sizeof(req->u8Rates));

  ILOG4_V("Dealing w/ Fixed MCS VAP management");
  req->fixedMcsVapManagement = (uint8)wave_core_management_frames_rate_select(core);

  ILOG4_V("Dealing w/ HT/VHT MCS-s and flags");
  {
    mtlk_pdb_size_t mcs_len = sizeof(req->u8HT_MCS_Set);
    wave_pdb_get_binary(param_db_core, PARAM_DB_CORE_RX_MCS_BITMASK, &req->u8HT_MCS_Set, &mcs_len);
    mcs_len = sizeof(req->u8VHT_Mcs_Nss);
    wave_pdb_get_binary(param_db_core, PARAM_DB_CORE_VHT_MCS_NSS, &req->u8VHT_Mcs_Nss, &mcs_len);
    req->flags = wave_pdb_get_int(param_db_core, PARAM_DB_CORE_SET_BSS_FLAGS);

    if (mtlk_df_user_get_hs20_status(mtlk_vap_get_df(core->vap_handle))) {
      req->flags |= MTLK_BFIELD_VALUE(VAP_ADD_FLAGS_HS20_ENABLE, 1, uint8);
    }
  }
  req->beaconPhyMode = (uint8)mtlk_core_get_6ghz_beacon_format(band, param_db_core);
  ILOG1_DD("Band = %d, UMI_SET_BSS->beaconPhyMode = %u", band, req->beaconPhyMode);
  req->beacon6GBandwidth = (uint8)mtlk_core_get_6ghz_duplicate_beacon_bw(band, param_db_core);
  mtlk_dump(2, req, sizeof(*req), "dump of UMI_SET_BSS:");
  req->u8HE_Bss_Color_info = MTLK_CORE_PDB_GET_INT(core, PARAM_DB_CORE_HE_BSS_COLOR);
  ILOG1_DD("CID-%04x: UMI_SET_BSS->u8HE_Bss_Color_info: %u", mtlk_vap_get_oid(core->vap_handle), req->u8HE_Bss_Color_info);

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (res != MTLK_ERR_OK || UMI_OK != req->Status) {
      ELOG_DDD("CID-%04x: UM_BSS_SET_BSS_REQ send failed, res=%i status=%hhu",
              mtlk_vap_get_oid(core->vap_handle), res, req->Status);
    if (UMI_OK != req->Status)
      res = MTLK_ERR_MAC;
  } else {
    if (mtlk_core_get_net_state(core) != NET_STATE_CONNECTED) {
      res = mtlk_core_set_net_state(core, NET_STATE_CONNECTED);
    }
    /* Mark vif as initialized after successful recovery set bss */
    wave_radio_recovery_init_vif(radio, vap_id);
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_core_send_fixed_pwr_cfg (mtlk_core_t *core, FIXED_POWER *fixed_pwr_params)
{
  mtlk_error_t       res;
  mtlk_txmm_msg_t    man_msg;
  mtlk_txmm_data_t  *man_entry = NULL;
  FIXED_POWER       *fixed_pwr_cfg = NULL;
  mtlk_vap_handle_t  vap_handle = core->vap_handle;

  ILOG2_V("Sending UM_MAN_FIXED_POWER_REQ");

  man_entry = mtlk_txmm_msg_init_with_empty_data(&man_msg, mtlk_vap_get_txmm(vap_handle), NULL);
  if (NULL == man_entry) {
    ELOG_D("CID-%04x: No man entry available", mtlk_vap_get_oid(vap_handle));
    return MTLK_ERR_NO_RESOURCES;
  }

  man_entry->id = UM_MAN_FIXED_POWER_REQ;
  man_entry->payload_size = sizeof(FIXED_POWER);

  fixed_pwr_cfg = (FIXED_POWER *)(man_entry->payload);
  *fixed_pwr_cfg = *fixed_pwr_params;

  res = mtlk_txmm_msg_send_blocked(&man_msg, MTLK_MM_BLOCKED_SEND_TIMEOUT);
  if (MTLK_ERR_OK != res || UMI_OK != fixed_pwr_cfg->Status) {
    ELOG_DDD("CID-%04x: Set UM_MAN_FIXED_POWER_REQ failed, res=%d status=%hhu",
            mtlk_vap_get_oid(vap_handle), res, fixed_pwr_cfg->Status);
    if (UMI_OK != fixed_pwr_cfg->Status)
      res = MTLK_ERR_MAC;
  }

  mtlk_txmm_msg_cleanup(&man_msg);
  return res;
}
