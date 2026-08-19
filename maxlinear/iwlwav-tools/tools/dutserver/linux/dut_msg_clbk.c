/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation
         Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
         Copyright 2009 - 2014 Lantiq Deutschland GmbH
         Copyright 2007 - 2008 Infineon Technologies AG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#include "mtlkinc.h"
#include "driver_api.h"
#include "dut_msg_clbk.h"
#include "dataex.h"
#include "mhi_dut.h"

#define LOG_LOCAL_GID   GID_DUT_MSG_CLB
#define LOG_LOCAL_FID   1

static dut_msg_clbk_t _dut_hostif_funcs_array[DUT_SERVER_MSG_CNT];

#define   MTLK_PACK_ON
#include "mtlkpack.h"

/* backward compatibility, doReset parameter is absent in V0 */
typedef struct _dutResetParamsCompat
{
  uint32 nvMemoryType;
  uint32 eepromSize;
} __MTLK_PACKED dutResetParamsV0_t;

#define   MTLK_PACK_OFF
#include "mtlkpack.h"

#define DUT_MAX_PROGMODEL_NAME_LEN 256

int __MTLK_IFUNC
dut_api_send_dut_core_command(dutDriverMessagesId_e in_msg_id, 
      const char *data, int length, int hw_idx);

#define _dut_progmodel_free(msg) mtlk_osal_mem_free(msg)

static dut_progmodel_t *
_dut_progmodel_alloc (const char *name)
{
  dut_progmodel_t *msg;
  uint32 len = strnlen_s(name, DUT_MAX_PROGMODEL_NAME_LEN);

  msg = (dut_progmodel_t *)mtlk_osal_mem_alloc(sizeof(*msg) + len + 1, MTLK_MEM_TAG_DUT_CORE);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(*msg) + len + 1);

  msg->status = MTLK_ERR_UNKNOWN;
  msg->len = len + 1;
  sprintf_s(msg->name, msg->len, "%s", name);

  return msg;
}

static BOOL
_dut_msg_clbk_upload_progmodel(int dutIndex, uint8_t* data, size_t length, size_t *out_length)
{
  BOOL ok = FALSE;
  int res;
  dut_progmodel_t *msg0 = NULL;
  dut_progmodel_t *msg1 = NULL;

  char progNames[2][DUT_MAX_PROGMODEL_NAME_LEN];
  unsigned int namesLen[2];
  char* str_data = (char*) data;
  char* newLinePtr = strchr(str_data, '\n');

  if (!dut_api_is_connected_to_hw(dutIndex))
  {
    ELOG_D("No connection with HW #%d", dutIndex);
    return FALSE;
  }

  if (newLinePtr == NULL) 
  {
    ELOG_V("Invalid data in upload progmodel message.");
    return FALSE;
  }

  namesLen[0] = newLinePtr - str_data;
  namesLen[1] = length - namesLen[0] - 1;
  if (namesLen[0] >= sizeof(progNames[0]) || namesLen[1] >= sizeof(progNames[1]))
  {
    ELOG_DD("Filenames of progmodels are too long (%d, %d)", namesLen[0], namesLen[1]);
    return FALSE;
  }

  wave_strncopy(progNames[0], str_data, sizeof(progNames[0]), namesLen[0]);
  wave_strncopy(progNames[1], newLinePtr + 1, sizeof(progNames[1]), namesLen[1]);

  msg0 = _dut_progmodel_alloc(progNames[0]);
  msg1 = _dut_progmodel_alloc(progNames[1]);
  if ((!msg0) || (!msg1)) {
    ELOG_V("Memory alloc failed");
    goto FINISH;
  }

  res = dut_api_upload_progmodel(msg0, (sizeof(*msg0) + msg0->len), dutIndex);
  if (MTLK_ERR_OK != res)
  {
    ELOG_SD("DUT: Failed to load progmodel file(%s), error %d", progNames[0], res);
    goto FINISH;
  }

  res = dut_api_upload_progmodel(msg1, (sizeof(*msg1) + msg1->len), dutIndex);
  if (MTLK_ERR_OK != res)
  {
    ELOG_SD("DUT: Failed to load progmodel file(%s), error %d", progNames[1], res);
    goto FINISH;
  }

  *out_length = 0;
  ok = TRUE;

FINISH:
  /* in case the upload failed - do not send response */
  if (msg0) {
    _dut_progmodel_free(msg0);
  }
  if (msg1) {
    _dut_progmodel_free(msg1);
  }

  return ok;
}

static BOOL
_dut_msg_clbk_forward_to_dut_core (
      dutDriverMessagesId_e in_msg_id, 
      int dutIndex, uint8_t* data, size_t length, size_t *out_length)
{
  int res;
  
  if (!dut_api_is_connected_to_hw(dutIndex))
  {
    ELOG_D("No connection with HW #%d", dutIndex);
    return FALSE;
  }

  res = dut_api_send_dut_core_command(in_msg_id, (const char*)data, length, dutIndex);
  if (MTLK_ERR_OK != res)
  {
    ELOG_D("Failed to send dut core message, error %d", res);
    memset((void*)data, 0, length);
  }

  *out_length = length;
  return TRUE;
}

static BOOL
_dut_msg_clbk_add_vap(int dutIndex, uint8_t* data, size_t length, size_t *out_length) {

  return _dut_msg_clbk_forward_to_dut_core(
                DUT_SERVER_MSG_ADD_VAP, 
                dutIndex, data, length, out_length);
}

static BOOL
_dut_msg_clbk_remove_vap(int dutIndex, uint8_t* data, size_t length, size_t *out_length) {

  return _dut_msg_clbk_forward_to_dut_core(
                DUT_SERVER_MSG_REMOVE_VAP, 
                dutIndex, data, length, out_length);
}

static BOOL
_dut_msg_clbk_set_bss(int dutIndex, uint8_t* data, size_t length, size_t *out_length) {

  return _dut_msg_clbk_forward_to_dut_core(
                DUT_SERVER_MSG_SET_BSS, 
                dutIndex, data, length, out_length);
}

static BOOL
_dut_msg_clbk_set_wmm_parameters(int dutIndex, uint8_t* data, size_t length, size_t *out_length) {

  return _dut_msg_clbk_forward_to_dut_core(
                DUT_SERVER_MSG_SET_WMM_PARAMETERS, 
                dutIndex, data, length, out_length);
}

static BOOL
_dut_msg_clbk_stop_vap_traffic(int dutIndex, uint8_t* data, size_t length, size_t *out_length) {

  return _dut_msg_clbk_forward_to_dut_core(
                DUT_SERVER_MSG_STOP_VAP_TRAFFIC, 
                dutIndex, data, length, out_length);
}

static BOOL
_dut_msg_clbk_driver_fw_general(int dutIndex, uint8_t* data, size_t length, size_t *out_length) {

  return _dut_msg_clbk_forward_to_dut_core(
                DUT_SERVER_MSG_DRIVER_FW_GENERAL, 
                dutIndex, data, length, out_length);
}

static BOOL
_dut_msg_clbk_c100 (int dutIndex, uint8_t* data, size_t length, size_t *out_length)
{
  int res;

  ILOG2_D("DLL msg len = %d", length);
  ILOG2_DDDDDDDD("DLL msg[0~7](%02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x)",
      data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7] );
  ILOG2_DDDDDDDD("DLL msg [8~15](%02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x)",
      data[8], data[9], data[0xa], data[0xb], data[0xc], data[0xd], data[0xe], data[0xf] );

  if (!dut_api_is_connected_to_hw(dutIndex))
  {
    ELOG_D("No connection with HW #%d", dutIndex);
    return FALSE;
  }

  res = dut_api_send_fw_command(data, length, dutIndex);
  if (MTLK_ERR_OK != res)
  {
    ELOG_D("Failed to send C100 message, error %d", res);
    memset((void*)data, 0, length);
  }

  *out_length = length;
  return TRUE;
}

static BOOL
_dut_msg_clbk_drv(int dutIndex, uint8_t* data, size_t length, size_t *out_length)
{
  int res;

  if (!dut_api_is_connected_to_hw(dutIndex))
  {
    ELOG_D("No connection with HW #%d", dutIndex);
    return FALSE;
  }

  res = dut_api_send_drv_command(data, length, dutIndex);
  if (MTLK_ERR_OK != res)
  {
    ELOG_D("Failed to send DRV message, error %d", res);
    memset((void*)data, 0, length);
  }

  *out_length = length;
  return TRUE;
}

static BOOL
_dut_msg_clbk_reset_fw(int dutIndex, uint8_t* data, size_t length, size_t *out_length)
{
  dutResetParams_t *params = (dutResetParams_t *)data;
  int res;
  BOOL doReset = FALSE;

  /* backward compatibility, doReset parameter is absent in V0 */
  if ((NULL != params) && (length == sizeof(dutResetParamsV0_t)))
  {
    doReset = TRUE;
  }

  if ((NULL != params) && (length == sizeof(dutResetParams_t)))
  {
    doReset = (BOOL)params->doReset;
  }

  /* call stop procedure anyway */
  (void)dut_api_driver_stop(doReset);

  (void)dut_api_driver_start(doReset);

  /* backward compatibility, no parameters */
  if ((NULL != params) && (length >= sizeof(dutResetParamsV0_t)))
  {
    if (!dut_api_is_connected_to_hw(dutIndex))
    {
      ELOG_D("No connection with HW #%d", dutIndex);
      return FALSE;
    }

    if (DUT_NV_MEMORY_FLASH == DUT_TO_HOST32(params->nvMemoryType))
    {
      res = dut_api_eeprom_data_on_flash_prepare(DUT_TO_HOST32(params->eepromSize), dutIndex);
      if (MTLK_ERR_OK != res)
      {
        ELOG_D("Failed to create EEPROM data on FLASH, error %d", res);
        memset((void*)data, 0, length);
      }
    }
  }

  *out_length = 0;
  return TRUE;
}

static BOOL
_dut_msg_clbk_platform_data_fields(int dutIndex, uint8_t* data, size_t length, size_t *out_length) {
   return _dut_msg_clbk_forward_to_dut_core(
                DUT_SERVER_MSG_PLATFORM_DATA_FIELDS,
                dutIndex, data, length, out_length);
}

static BOOL
_dut_msg_clbk_platform_general(int dutIndex, uint8_t* data, size_t length, size_t *out_length)
{
  int res;
  res = dut_api_send_platform_command(data, length, dutIndex);

  if(MTLK_ERR_OK != res)
  {
    ELOG_D(" Failed to send platform command , error : %d",res);
    *out_length = 0;
  }
  else
  {
    *out_length = length;
  }

  return TRUE;
}

void
dut_msg_clbk_init()
{
  _dut_hostif_funcs_array[DUT_SERVER_MSG_RESET_MAC] = _dut_msg_clbk_reset_fw;
  _dut_hostif_funcs_array[DUT_SERVER_MSG_UPLOAD_PROGMODEL] = _dut_msg_clbk_upload_progmodel;
  _dut_hostif_funcs_array[DUT_SERVER_MSG_MAC_C100] = _dut_msg_clbk_c100;
  _dut_hostif_funcs_array[DUT_SERVER_MSG_DRIVER_GENERAL] = _dut_msg_clbk_drv;
  _dut_hostif_funcs_array[DUT_SERVER_MSG_ADD_VAP] = _dut_msg_clbk_add_vap;
  _dut_hostif_funcs_array[DUT_SERVER_MSG_REMOVE_VAP] = _dut_msg_clbk_remove_vap;
  _dut_hostif_funcs_array[DUT_SERVER_MSG_SET_BSS] = _dut_msg_clbk_set_bss;
  _dut_hostif_funcs_array[DUT_SERVER_MSG_SET_WMM_PARAMETERS] = _dut_msg_clbk_set_wmm_parameters;
  _dut_hostif_funcs_array[DUT_SERVER_MSG_STOP_VAP_TRAFFIC] = _dut_msg_clbk_stop_vap_traffic;
  _dut_hostif_funcs_array[DUT_SERVER_MSG_DRIVER_FW_GENERAL] = _dut_msg_clbk_driver_fw_general;
  _dut_hostif_funcs_array[DUT_SERVER_MSG_PLATFORM_DATA_FIELDS] = _dut_msg_clbk_platform_data_fields;
  _dut_hostif_funcs_array[DUT_SERVER_MSG_PLATFORM_GENERAL] = _dut_msg_clbk_platform_general;
}

dut_msg_clbk_t
dut_msg_clbk_get_handler(int msgID)
{
  if(msgID >= DUT_SERVER_MSG_CNT || msgID <= 0)
  {
    ELOG_DDD("Incorrect message ID 0x%x, range(%d, %d)", msgID, 0, DUT_SERVER_MSG_CNT);
    return NULL;
  }

  return _dut_hostif_funcs_array[msgID];
}
