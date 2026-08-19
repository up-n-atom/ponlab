/******************************************************************************
  Copyright (c) 2020, MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

The source code contained or described herein and all documents related to the
source code ("Material") are owned by Intel Corporation or its suppliers or
licensors. Title to the Material remains with Maxlinear Inc. or its suppliers
and licensors. The Material contains trade secrets and proprietary and
confidential information of Maxlinear or its suppliers and licensors. The
Material is protected by worldwide copyright and trade secret laws and treaty
provisions. No part of the Material may be used, copied, reproduced, modified,
published, uploaded, posted, transmitted, distributed, or disclosed in any way
without Maxlinear's prior express written permission.

No license under any patent, copyright, trade secret or other intellectual
property right is granted to or conferred upon you by disclosure or delivery of
the Materials, either expressly, by implication, inducement, estoppel or
otherwise. Any license under such intellectual property rights must be express
and approved by Maxlinear in writing.

******************************************************************************/

#include "mtlkinc.h"
#include "hw_mmb.h"
#include "core_config.h"
#include "mtlk_df_priv.h"
#include "mtlk_coc.h"

#ifdef CONFIG_THERMAL
#include <linux/thermal.h>
#endif
#include <linux/of.h>
#include <linux/units.h>

#define LOG_LOCAL_GID   GID_THERMAL
#define LOG_LOCAL_FID   1

#ifdef CONFIG_THERMAL
typedef enum {
  COOLING_TYPE_FIRST,
  COOLING_TYPE_TX_DUTY_CYCLE = COOLING_TYPE_FIRST,
  COOLING_TYPE_ANTENNA,
  COOLING_TYPE_MAX
} cooling_type_e;

typedef struct cooling_method {
  uint32 type;
  uint32 param;
} cooling_method_t;

#define DEFAULT_NUM_COOLING_STATE 3
#define MAX_NUM_COOLING_STATE 100
struct wave_thermal {
  struct thermal_zone_device *tzd;
  struct thermal_cooling_device *cd;
  unsigned long cooling_state;
  unsigned long max_cooling_state;
  cooling_method_t *cooling_state_params;
  uint32 cooling_state_cnt[COOLING_TYPE_MAX];
  uint8 old_auto_coc_state;
  /*This flag will be used to unregister if it is registered
  * with the Linux thermal framework */
  BOOL registration_status;
};

#define DEFAULT_TEMPERATURE_VALUE -128
int __MTLK_IFUNC
#if LINUX_VERSION_IS_LESS(6,0,0)
wave_thermal_zone_get_temp (void *data, int *temperature)
#else
wave_thermal_zone_get_temp (struct thermal_zone_device *dev, int *temperature)
#endif
{
  int temp = 0;
  mtlk_core_t* master_core;
  mtlk_error_t res = MTLK_ERR_OK;
  wave_radio_t *radio;
  mtlk_df_t *master_df;
  mtlk_handle_t lhandle = MTLK_INVALID_HANDLE;
  mtlk_temperature_sensor_t *temperature_cfg;
  uint32 data_size;
  mtlk_hw_band_e wave_band;

#if LINUX_VERSION_IS_LESS(6,0,0)
  if (!data || !temperature) {
    ELOG_PP("Thermal: Invalid input params, data %p temperature %p", data, temperature);
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_PARAMS);
  }
  radio = ((wave_radio_t*) data);
#else
  if (!dev || !temperature) {
    ELOG_PP("Thermal: Invalid input params, dev %p temperature %p", dev, temperature);
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_PARAMS);
  }
  radio = (wave_radio_t*) dev->devdata;
#endif
  wave_band = wave_radio_band_get(radio);

  master_core = wave_radio_master_core_get(radio);
  if (mtlk_core_get_net_state(master_core) != NET_STATE_CONNECTED) {
  /* send the default temperature value to upper layer before interface UP */
  /* convert to millicelsius */
    *temperature = DEFAULT_TEMPERATURE_VALUE * MILLIDEGREE_PER_DEGREE;
    ILOG3_DD("Thermal: Band-%d Default temperature %d", wave_band, *temperature);
    return res;
  }
  master_df = mtlk_vap_manager_get_master_df(mtlk_vap_get_manager(master_core->vap_handle));
  if (master_df == NULL) {
    ELOG_D("Thermal: Band-%d Error getting master_df", wave_band);
    res = MTLK_ERR_UNKNOWN;
    goto end;
  }
  /* Query the temperature sensor value to FW when core state is connected */
  res = _mtlk_df_user_pull_core_data(master_df, WAVE_HW_REQ_GET_TEMPERATURE_SENSOR,
                                     FALSE, (void**) &temperature_cfg, &data_size, &lhandle);
  if (res != MTLK_ERR_OK) {
    if (res == MTLK_ERR_NOT_READY) {
      ILOG1_D("Thermal: Band-%d Unable to read temperature data for tzd during CAC, or channel not certain", wave_band);
    } else {
      ELOG_D("Thermal: Band-%d Failed to read temperature data for tzd", wave_band);
    }
    goto end;
  }

  MTLK_ASSERT(sizeof(*temperature_cfg) == data_size);
  MTLK_CFG_GET_ITEM(temperature_cfg, temperature, temp);

  /* convert to millicelsius */
  *temperature = temp * MILLIDEGREE_PER_DEGREE;
  ILOG3_DD("Thermal: Band-%d temperature %d", wave_band, *temperature);

end:
  if (MTLK_INVALID_HANDLE != lhandle) {
    _mtlk_df_user_free_core_data(master_df, lhandle);
  }

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

int __MTLK_IFUNC
wave_cooling_get_max_state (struct thermal_cooling_device *cd,
            unsigned long *state)
{
  wave_radio_t *radio;
  struct wave_thermal *thermal;
  mtlk_hw_band_e wave_band;

  if (!cd || !state) {
    ELOG_PP("Thermal: Invalid input params, cd %p state %p", cd, state);
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_PARAMS);
  }
  radio = (wave_radio_t*) cd->devdata;
  if (!radio) {
    ELOG_S("Thermal: Invalid devdata from %s", cd->type);
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_PARAMS);
  }
  wave_band = wave_radio_band_get(radio);
  thermal = wave_radio_get_thermal_mgmt_data(radio);
  if (!thermal) {
    ELOG_D("Thermal: Band-%d Invalid thermal mgmt data",
            wave_band);
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_VALUE);
  }

  *state = thermal->max_cooling_state;
  ILOG3_DD("Thermal: Band-%d Max state %lu", wave_band, *state);
  return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_OK);
}

int __MTLK_IFUNC
wave_cooling_get_cur_state (struct thermal_cooling_device *cd,
            unsigned long *state)
{
  wave_radio_t *radio;
  struct wave_thermal *thermal;
  mtlk_hw_band_e wave_band;

  if (!cd || !state) {
    ELOG_PP("Thermal: Invalid input params, cd %p state %p", cd, state);
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_PARAMS);
  }
  radio = (wave_radio_t*) cd->devdata;
  if (!radio) {
    ELOG_S("Thermal: Invalid devdata from %s", cd->type);
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_PARAMS);
  }
  wave_band = wave_radio_band_get(radio);
  thermal = wave_radio_get_thermal_mgmt_data(radio);
  if (!thermal) {
    ELOG_D("Thermal: Band-%d Invalid thermal mgmt data",
            wave_band);
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_VALUE);
  }
  *state = thermal->cooling_state;

  ILOG3_DD("Thermal: Band-%d Current state %lu", wave_band, *state);
  return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_OK);
}

static mtlk_error_t
_wave_cooling_radio_set_duty_cycle (mtlk_hw_t *hw, uint32 offtime)
{
  mtlk_core_t *master_core;
  mtlk_error_t res;
  mtlk_coc_erp_cfg_t erp_cfg;
  mtlk_erp_t *erp_obj;
  uint8 card_idx = mtlk_hw_mmb_get_card_idx(hw);

  master_core = wave_hw_get_first_radio_master_vap_core(hw);
  erp_obj = wave_radio_erp_mgmt_get(wave_vap_radio_get(master_core->vap_handle));
#define DUTY_CYCLE_ON_MAX 100
  if (offtime != MAX_UINT32) {
    erp_cfg.erp_enabled    =  TRUE;
    erp_cfg.radio_on_time  =  (DUTY_CYCLE_ON_MAX - offtime);
    erp_cfg.radio_off_time =  offtime;
  } else {
    erp_cfg.erp_enabled    =  FALSE;
    erp_cfg.radio_on_time  =  DUTY_CYCLE_ON_MAX;
    erp_cfg.radio_off_time =  0;
  }

  /* ERP config: initial_wait_time = 1 sec for immediate UMI dispatch.
   * If the erp criterion value are reached (ex. max_num_sta, tx_*_max_tp values),
   * ERP will be turned off. This will turnoff our duty cycle config too.
   * Hence keep other irrelevent params to MAX value
   */
  erp_cfg.initial_wait_time =  1;
  erp_cfg.max_num_sta       =  WAVE_MAX_STA_ID;
  erp_cfg.rx_tp_max         =  MAX_UINT32;
  erp_cfg.tx_20_max_tp      =  MAX_UINT32;
  erp_cfg.tx_40_max_tp      =  MAX_UINT32;
  erp_cfg.tx_80_max_tp      =  MAX_UINT32;
  erp_cfg.tx_160_max_tp     =  MAX_UINT32;
#ifdef MTLK_WAVE_700
  erp_cfg.tx_320_max_tp     =  MAX_UINT32;
#endif

  ILOG2_DDD("Thermal: CID-%02x Set DutyCycle request offtime %u, ontime %u",
     mtlk_hw_mmb_get_card_idx(hw), erp_cfg.radio_off_time, erp_cfg.radio_on_time);
  res = mtlk_coc_set_erp_mode(erp_obj, &erp_cfg);
  if (res != MTLK_ERR_OK) {
    ELOG_DDD("Thermal: CID-%02x Unable to set DutyCycle request offtime %u, ontime %u",
             card_idx, erp_cfg.radio_off_time, erp_cfg.radio_on_time);
  }
  return res;
}

static mtlk_error_t
_wave_cooling_radio_set_antenna_mask (wave_radio_t *radio, uint32 ant_cnt,
            BOOL trend_raising)
{
  mtlk_core_t *master_core;
  mtlk_error_t res;
  mtlk_coc_t *coc_mgmt;
  mtlk_coc_antenna_cfg_t *current_params;
  mtlk_clpb_t *clpb = NULL;
  mtlk_df_t *df;
  mtlk_coc_mode_cfg_t coc_cfg;

  struct wave_thermal *thermal = wave_radio_get_thermal_mgmt_data(radio);
  mtlk_hw_band_e wave_band = wave_radio_band_get(radio);
  if (wave_radio_get_is_zwdfs_radio(radio))
    return MTLK_ERR_OK;

  df = wave_radio_df_get(radio);
  MTLK_CHECK_DF(df);

  master_core = wave_radio_master_core_get(radio);

  coc_mgmt = __wave_core_coc_mgmt_get(master_core);
  MTLK_ASSERT(coc_mgmt != NULL);
  current_params = mtlk_coc_get_current_params(coc_mgmt);


  memset(&coc_cfg, 0, sizeof(coc_cfg));
  /*
   * By default AutoCOC is enabled so we should stop this when
   * Thermal takes control of antenna switch
   * */
  coc_cfg.power_params.is_auto_mode = FALSE;

  if (thermal->old_auto_coc_state == MAX_UINT8) {
    thermal->old_auto_coc_state = mtlk_coc_get_auto_mode_cfg(coc_mgmt);
  }
  if (ant_cnt != MAX_UINT32) {
    coc_cfg.power_params.antenna_params.num_rx_antennas = ant_cnt;
    coc_cfg.power_params.antenna_params.num_tx_antennas = ant_cnt;
  } else {
  /*
   * Here Thermal gives the antenna control back to AutoCOC
   * Make sure we start from default antenna configuration*/
    coc_cfg.power_params.is_auto_mode = thermal->old_auto_coc_state;
    coc_cfg.power_params.antenna_params.num_rx_antennas  =
    mtlk_core_get_max_rx_antennas(master_core);
    coc_cfg.power_params.antenna_params.num_tx_antennas  =
    ant_cnt = mtlk_core_get_max_tx_antennas(master_core);
    thermal->old_auto_coc_state = MAX_UINT8;
  }

  if (trend_raising) {
    if (ant_cnt >= current_params->num_tx_antennas) {
      ELOG_DDD("Thermal: Band-%d COC antenna count %u is less than Thermal suggested tx %u ",
             wave_band, current_params->num_tx_antennas, ant_cnt);
    return MTLK_ERR_OK;
  }
  }

  if (coc_cfg.power_params.antenna_params.num_rx_antennas > mtlk_core_get_max_rx_antennas(master_core) ||
      coc_cfg.power_params.antenna_params.num_tx_antennas > mtlk_core_get_max_tx_antennas(master_core)) {
    ELOG_DDD("Thermal: Band-%d Invalid Antenna count value tx %u rx %u",
             wave_band, coc_cfg.power_params.antenna_params.num_tx_antennas,
             coc_cfg.power_params.antenna_params.num_rx_antennas);
    return MTLK_ERR_PARAMS;
  }
  ILOG2_DDD("Thermal: Band-%d Set Antenna request, tx %u rx %u",
            wave_band, coc_cfg.power_params.antenna_params.num_tx_antennas,
            coc_cfg.power_params.antenna_params.num_rx_antennas);

  coc_cfg.power_params_filled = 1;
  /* The very first call the below function will stop the autoCOC*/
  res = _mtlk_df_user_invoke_core(df, WAVE_RADIO_REQ_SET_COC_CFG, &clpb, &coc_cfg, sizeof(coc_cfg));
  res = _mtlk_df_user_process_core_retval(res, clpb, WAVE_RADIO_REQ_SET_COC_CFG, TRUE);

  if (res != MTLK_ERR_OK) {
    ELOG_DDD("Thermal: Band-%d Unable to set Antenna param tx %u rx %u", wave_band,
             coc_cfg.power_params.antenna_params.num_tx_antennas,
             coc_cfg.power_params.antenna_params.num_rx_antennas);
    return res;
  }

  return MTLK_ERR_OK;
}

static mtlk_error_t
_wave_cooling_cur_state_cooling_method(wave_radio_t *radio, struct wave_thermal *thermal,
            cooling_method_t *cmethod, uint32 *updated_cooling_state_cnt, BOOL trend_raising)
{
  uint32 i, param;
  mtlk_error_t res = MTLK_ERR_UNKNOWN;
  for (i = COOLING_TYPE_FIRST; i < COOLING_TYPE_MAX; i++) {
    ILOG2_DDDDD("Thermal: Band-%d Type %u param %u cooling_state_cnt (old/new): %u/%u",
                wave_radio_band_get(radio), i,  cmethod[i].param,
                thermal->cooling_state_cnt[i], updated_cooling_state_cnt[i]);
    if (thermal->cooling_state_cnt[i] == updated_cooling_state_cnt[i]) {
      res = MTLK_ERR_OK;
      continue;
    }

    param = cmethod[i].param;
    /* The updated count is 0. i.e. Any previous cooling state of the cooling method
     * should be turned off. Set param to MAX_UINT32 to identify such case.
     */
    if (thermal->cooling_state_cnt[i] && !updated_cooling_state_cnt[i])
      param = MAX_UINT32;

    switch (i) {
      case COOLING_TYPE_TX_DUTY_CYCLE:
        /*TODO Change duty_cycle method to work radio basis*/
        res = _wave_cooling_radio_set_duty_cycle(wave_radio_get_hw(radio), param);
        break;
      case COOLING_TYPE_ANTENNA:
        res = _wave_cooling_radio_set_antenna_mask(radio, param, trend_raising);
        break;
      default:
        res = MTLK_ERR_UNKNOWN;
        break;
    }
    if (res == MTLK_ERR_OK)
      /* Store the cooling state count for the cooling method */
      thermal->cooling_state_cnt[i] = updated_cooling_state_cnt[i];
  }
  return res;
}

int __MTLK_IFUNC
wave_cooling_set_cur_state(struct thermal_cooling_device *cd,
            unsigned long state)
{
  wave_radio_t *radio;
  struct wave_thermal *thermal;
  mtlk_error_t res;
  unsigned long cur_state, req_state = state;
  uint32 type;
  BOOL trend_raising;
  cooling_method_t *cooling_state_params, cur_cooling_state_params[COOLING_TYPE_MAX];
  uint32 updated_cooling_state_cnt[COOLING_TYPE_MAX];
  mtlk_hw_band_e wave_band;

  if (!cd) {
    ELOG_P("Thermal: Invalid input params, cd %p", cd);
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_PARAMS);
  }
  radio = (wave_radio_t*) cd->devdata;
  if (!radio) {
    ELOG_S("Thermal: Invalid devdata from %s", cd->type);
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_PARAMS);
  }
  wave_band = wave_radio_band_get(radio);
  thermal = wave_radio_get_thermal_mgmt_data(radio);
  if (!thermal) {
    ELOG_D("Thermal: Band-%d Invalid thermal mgmt data",
            wave_band);
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_VALUE);
  }
  if (req_state > thermal->max_cooling_state) {
    ELOG_DDD("Thermal: Band-%d Invalid state request, requested %lu max %lu",
            wave_band, req_state, thermal->max_cooling_state);
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_PARAMS);
  }
  cur_state = thermal->cooling_state;
  if (req_state == cur_state)
    return _mtlk_df_mtlk_to_linux_error_code(MTLK_ERR_ALREADY_EXISTS);

  cooling_state_params = thermal->cooling_state_params;
  memset(cur_cooling_state_params, 0xFF, sizeof(cur_cooling_state_params));
  wave_memcpy(updated_cooling_state_cnt, sizeof(updated_cooling_state_cnt),
       thermal->cooling_state_cnt, sizeof(thermal->cooling_state_cnt));
  trend_raising = (cur_state < req_state);
  /* When requested is not same as cur_state
   * If trend_raising is TRUE i.e.  Requested cooling state is Higher than current
   *  1. Go through cooling state params for each state from current
   *     to Requested state. This will be in ascending order thus providing the
   *     Max cooling state param for each type for the requested cooling state.
   *  2. Increment cooling state count for the cooling method/type. Helps in
   *     turning off the cooling method and skip when no change in cooling state
   *     of a type.
   * If trend_raising is FALSE i.e.  Requested cooling state is Lower than current
   *  1. Go through cooling state params for each state from current
   *     to Requested state. This will be in descending order thus providing the
   *     Max cooling state param for each type for the requested cooling state.
   *  2. Decrement cooling state count for the cooling method/type.
   */
  while (cur_state != req_state) {
    if (trend_raising) {
      type = cooling_state_params[cur_state++].type;
      updated_cooling_state_cnt[type]++;
    } else {
      type = cooling_state_params[--cur_state].type;
      updated_cooling_state_cnt[type]--;
    }
    if (cur_state > 0) {
      type = cooling_state_params[cur_state-1].type;
      cur_cooling_state_params[type] = cooling_state_params[cur_state-1];
    }
    ILOG2_DD("Thermal: Band-%d state -> %lu", wave_band, cur_state);
  }
  res = _wave_cooling_cur_state_cooling_method(radio, thermal,
            &cur_cooling_state_params[0], updated_cooling_state_cnt, trend_raising);
  if (res == MTLK_ERR_OK)
    thermal->cooling_state = state;

  return _mtlk_df_mtlk_to_linux_error_code(res);
}

#if LINUX_VERSION_IS_LESS(6,0,0)
static const struct thermal_zone_of_device_ops tzd_ops = {
  .get_temp = wave_thermal_zone_get_temp,
};
#else
static struct thermal_zone_device_ops tzd_ops = {
  .get_temp = wave_thermal_zone_get_temp,
};
#endif

static const struct thermal_cooling_device_ops wave_cooling_ops = {
  .get_max_state = wave_cooling_get_max_state,
  .get_cur_state = wave_cooling_get_cur_state,
  .set_cur_state = wave_cooling_set_cur_state,
};

static void
_wave_thermal_cooling_device_unregister(mtlk_hw_t *hw, unsigned radio_idx)
{
  wave_radio_t *radio = wave_hw_radio_get(hw, radio_idx);
  struct wave_thermal *thermal = wave_radio_get_thermal_mgmt_data(radio);
  if (!thermal || !thermal->registration_status)
    return;

  ILOG1_D("Thermal: Band-%d Unregister cooling device",
          wave_radio_band_get(radio));
  thermal_cooling_device_unregister(thermal->cd);
}

static int
_wave_thermal_cooling_device_register (mtlk_hw_t *hw, unsigned radio_idx)
{
  char dev_name[THERMAL_NAME_LENGTH];
  struct pci_dev *pdev = wave_hw_mmb_get_pci_dev(hw);
  wave_radio_t *radio = wave_hw_radio_get(hw, radio_idx);
  struct wave_thermal *thermal = wave_radio_get_thermal_mgmt_data(radio);

  ILOG1_D("Thermal: Band-%d Register cooling device", wave_radio_band_get(radio));
  mtlk_snprintf(dev_name, sizeof(dev_name), "cdev-%s", thermal->tzd->type);
  thermal->cd = thermal_of_cooling_device_register(pdev->dev.of_node,
      dev_name, radio, &wave_cooling_ops);
  if (IS_ERR(thermal->cd)) {
    pr_err("Failed to register cooling device (error %ld)",
      PTR_ERR(thermal->cd));
    return MTLK_ERR_UNKNOWN;
  }
  thermal->registration_status = TRUE;
  return MTLK_ERR_OK;
}

static mtlk_error_t
_wave_thermal_set_cooling_props(struct wave_thermal *thermal, uint32 max_cooling_state, cooling_method_t *cooling_params)
{
  cooling_method_t default_cooling_params[DEFAULT_NUM_COOLING_STATE] =
    {
/* TODO: reenable when dutycycle can be used */
#if 0
     {COOLING_TYPE_TX_DUTY_CYCLE, 70},
     {COOLING_TYPE_TX_DUTY_CYCLE, 50},
     {COOLING_TYPE_TX_DUTY_CYCLE, 30},
#endif
     {COOLING_TYPE_ANTENNA, 3},
     {COOLING_TYPE_ANTENNA, 2},
     {COOLING_TYPE_ANTENNA, 1}};

  thermal->cooling_state_params = mtlk_osal_mem_alloc(sizeof(cooling_method_t) * max_cooling_state, WAVE_MEM_TAG_THERMAL);
  if (!thermal->cooling_state_params) {
    ELOG_S("Thermal: Unable to allocate cooling state param for tzd %s", thermal->tzd->type);
    return MTLK_ERR_NO_MEM;
  }
  memset(thermal->cooling_state_params, 0, sizeof(cooling_method_t) * max_cooling_state);
  thermal->max_cooling_state = max_cooling_state;

  if (cooling_params) {
    wave_memcpy(thermal->cooling_state_params, sizeof(cooling_method_t) * max_cooling_state,
        cooling_params, sizeof(cooling_method_t) * max_cooling_state);
  } else {
    wave_memcpy(thermal->cooling_state_params, sizeof(cooling_method_t) * max_cooling_state,
        default_cooling_params, sizeof(default_cooling_params));
  }
  mtlk_dump(2, thermal->cooling_state_params, (sizeof(cooling_method_t) * thermal->max_cooling_state), "cooling-states");
  return MTLK_ERR_OK;
}

static mtlk_error_t
_wave_thermal_parse_cooling_props(struct device_node *np, struct wave_thermal *thermal, mtlk_hw_band_e wave_band)
{
  uint32 max_cooling_state = 0xFF;
  int error;
  mtlk_error_t res = MTLK_ERR_UNKNOWN;

  if (((error = of_property_read_u32(np, "max-cooling-state", &max_cooling_state)) != 0) ||
      (max_cooling_state == 0 || max_cooling_state > MAX_NUM_COOLING_STATE)) {
    WLOG_DDD("Thermal: Band-%d Didn't find/Invalid max-cooling-state param, error %d"
            " max_cooling_state %u", wave_band, error, max_cooling_state);
    /* set default cooling states and params */
    res = _wave_thermal_set_cooling_props(thermal, DEFAULT_NUM_COOLING_STATE, NULL);
    return res;
  }

  ILOG1_DD("Thermal: Band-%d Found max-cooling-state param %u", wave_band, max_cooling_state);
  thermal->max_cooling_state = max_cooling_state;
  {
    uint32 num_u32_elements = thermal->max_cooling_state * (sizeof(cooling_method_t)/sizeof(uint32));
    cooling_method_t  *cooling_params;
    cooling_params = mtlk_osal_mem_alloc(sizeof(*cooling_params) * thermal->max_cooling_state, WAVE_MEM_TAG_THERMAL);
    if (!cooling_params) {
      ELOG_D("Thermal: Band-%d Failed to allocate cooling params", wave_band);
      return MTLK_ERR_NO_MEM;
    }
    if ((error = of_property_read_u32_array(np, "cooling-states", (uint32*) cooling_params, num_u32_elements)) != 0) {
      /* set default cooling states and params */
      WLOG_DD("Thermal: Band-%d Didn't find cooling state params, error %d", wave_band, error);
      res = _wave_thermal_set_cooling_props(thermal, DEFAULT_NUM_COOLING_STATE, NULL);
      mtlk_osal_mem_free(cooling_params);
      return res;
    } else {
      ILOG1_D("Thermal: Band-%d Found cooling state params", wave_band);
      res = _wave_thermal_set_cooling_props(thermal, max_cooling_state, cooling_params);
    }
    mtlk_osal_mem_free(cooling_params);
  }
  return res;
}

static mtlk_error_t
_wave_thermal_thermal_zone_register (mtlk_hw_t *hw, unsigned radio_idx)
{
  struct device_node *np;
  struct wave_thermal *thermal = NULL;
  struct pci_dev *pdev = wave_hw_mmb_get_pci_dev(hw);
  mtlk_error_t res;

  wave_radio_descr_t *radio_descr = wave_card_radio_descr_get(hw);

  /* Temperature zone are per radio.*/
  wave_radio_t *radio = wave_radio_descr_wave_radio_get(radio_descr, radio_idx);
  mtlk_hw_band_e wave_band = wave_radio_band_get(radio);

  ILOG1_D("Thermal: Band:%d Register thermal zone device", wave_band);

  thermal = mtlk_osal_mem_alloc(sizeof(*thermal), WAVE_MEM_TAG_THERMAL);
  if (!thermal) {
    ELOG_D("Thermal: Band-%d Failed to allocate thermal data", wave_band);
    return MTLK_ERR_NO_MEM;
  }
  memset(thermal, 0, sizeof(*thermal));

  thermal->old_auto_coc_state = MAX_UINT8;
  /* PCI  device has no node in the DT, search for compatible devices */
  for_each_compatible_node(np, NULL, "mxl,wave") {
    u32 band;

    if (of_property_read_u32(np, "wave-band", &band) == 0) {
      if (band == wave_band) {
        ILOG1_D("Thermal: band-%d found DT node for PCI dev", wave_band);
        res = _wave_thermal_parse_cooling_props(np, thermal, wave_band);
        if (res != MTLK_ERR_OK)
           goto error;
        pdev->dev.of_node = of_node_get(np);
        goto register_sensor;
      }
    }
  }

  /* TODO: No PCI device node, no compatible node, try to register a thermal
   * zone and don't use DT */

  ILOG0_D("Thermal: Band-%d No DT node for PCI dev", wave_band);
  mtlk_osal_mem_free(thermal);
  /* DT node not found. Lets not fail, Skip tzd registeration */
  return MTLK_ERR_NO_ENTRY;

register_sensor:
#if LINUX_VERSION_IS_LESS(6,0,0)
  thermal->tzd = devm_thermal_zone_of_sensor_register(&pdev->dev,
                   0, radio, &tzd_ops);
#else
  thermal->tzd = devm_thermal_of_zone_register(&pdev->dev,
                   0, radio, &tzd_ops);
#endif
  if (IS_ERR(thermal->tzd)) {
    pr_err(" Thermal: Band-%d Failed to register thermal zone device"
           "( err %ld)", wave_band, PTR_ERR(thermal->tzd));
    res = MTLK_ERR_UNKNOWN;
    goto error;
  }
  ILOG1_D("Thermal: Band-%d Register thermal zone device success!!!", wave_band);
  /* per radio thermal data */
  wave_radio_set_thermal_mgmt_data(radio, thermal);

  return MTLK_ERR_OK;

error:
  if (thermal) {
    if (thermal->cooling_state_params)
      mtlk_osal_mem_free(thermal->cooling_state_params);
    mtlk_osal_mem_free(thermal);
  }
  wave_radio_set_thermal_mgmt_data(radio, NULL);

  return res;
}

static void
_wave_thermal_thermal_zone_unregister (mtlk_hw_t *hw, unsigned radio_idx)
{
  wave_radio_t *radio = wave_hw_radio_get(hw, radio_idx);
  struct wave_thermal *thermal = wave_radio_get_thermal_mgmt_data(radio);
  struct pci_dev *pdev = wave_hw_mmb_get_pci_dev(hw);

  ILOG1_D("Thermal: Band-%d Unregister thermal zone device", wave_radio_band_get(radio));

  if (!thermal || !thermal->registration_status)
    return;

  if (thermal->tzd) {
#if LINUX_VERSION_IS_LESS(6,0,0)
    devm_thermal_zone_of_sensor_unregister(&pdev->dev, thermal->tzd);
#else
    devm_thermal_of_zone_unregister(&pdev->dev, thermal->tzd);
#endif
    thermal->tzd = NULL;
  }

  if (thermal->cooling_state_params)
    mtlk_osal_mem_free(thermal->cooling_state_params);
  if (thermal)
    mtlk_osal_mem_free(thermal);
  wave_radio_set_thermal_mgmt_data(radio, NULL);
}
#endif /*CONFIG_THERMAL */

mtlk_error_t __MTLK_IFUNC
wave_thermal_init (mtlk_hw_t *hw)
{
  mtlk_error_t res = MTLK_ERR_OK;
#ifdef CONFIG_THERMAL
  wave_radio_descr_t *radio_descr = wave_card_radio_descr_get(hw);
  unsigned radio_idx;

  for (radio_idx = 0; radio_idx < radio_descr->num_radios; radio_idx++) {
    res = _wave_thermal_thermal_zone_register(hw, radio_idx);
    if (res != MTLK_ERR_OK) {
      if (res == MTLK_ERR_NO_ENTRY) {
        /* Don't fail the init, when TZD DT entry not found */
        res = MTLK_ERR_OK;
        continue;
      }
      goto finish;
    }
    res = _wave_thermal_cooling_device_register(hw, radio_idx);
    if (res != MTLK_ERR_OK)
      goto finish;
  }

finish:
#endif
  return res;
}

void __MTLK_IFUNC
wave_thermal_exit (mtlk_hw_t *hw)
{
#ifdef CONFIG_THERMAL
  wave_radio_descr_t *radio_descr = wave_card_radio_descr_get(hw);
  unsigned radio_idx;

  for (radio_idx = 0; radio_idx < radio_descr->num_radios; radio_idx++) {
    _wave_thermal_cooling_device_unregister(hw, radio_idx);
    _wave_thermal_thermal_zone_unregister(hw, radio_idx);
  }
#endif
}
