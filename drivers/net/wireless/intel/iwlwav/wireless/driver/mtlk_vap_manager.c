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
#include "mtlk_vap_manager.h"
#include "mtlk_df.h"
#include "mtlk_df_priv.h"
#include "mtlkhal.h"
#include "mtlk_param_db.h"
#include "mtlk_ab_manager.h"
#include "mtlkwlanirbdefs.h"
#include "mtlk_snprintf.h"
#include "txmm.h"
#include "hw_mmb.h"
#include "ta.h"
#include "fw_recovery.h"
#include "core_config.h"
#include "mtlk_rtlog.h"

#define LOG_LOCAL_GID   GID_VAPM
#define LOG_LOCAL_FID   0

static int _mtlk_vap_init (mtlk_vap_handle_t vap_handle,
    mtlk_vap_manager_t *obj,
    uint8 vap_index,
    const char *name,
    mtlk_work_role_e role,
    BOOL is_master,
    struct net_device *ndev);

typedef enum {
  VAP_STATUS_FIRST,
  VAP_STATUS_READY,
  VAP_STATUS_STOPPED
} mtlk_vap_status_t;

struct _mtlk_vap_manager_t
{
  struct _mtlk_vap_handle_t    **guest_vap_array;
  /* This array contains guest_vap_array & also secondary vaps */
  struct _mtlk_vap_handle_t    **secondary_id_vap_array;

  mtlk_osal_spinlock_t           guest_lock;

  mtlk_hw_api_t                 *hw_api;

  void                          *radio;
  unsigned                       radio_id;

  mtlk_atomic_t                  activated_vaps_num;
  mtlk_work_mode_e               work_mode;

  uint32                         max_vaps_count;
  mtlk_atomic_t                  master_vap_index;
  mtlk_handle_t                  ta;

  /* This counter keeps track of how many cores has WDS Bridge Mode enabled */
  mtlk_atomic_t                  wds_bridgemode;

  /* Always use mtlk_vap_get_core_array() to access this array */
  mtlk_core_t                  **core_array;

  mtlk_dlist_entry_t   dfg_mcast_list_entry;

  mtlk_vap_status_t             *e_vap_status;
  BOOL                           master_ndev_taken;
  MTLK_DECLARE_INIT_STATUS;
};


MTLK_INIT_STEPS_LIST_BEGIN(vap_manager)
  MTLK_INIT_STEPS_LIST_ENTRY(vap_manager, VAP_MANAGER_PREPARE_SYNCHRONIZATION)
  MTLK_INIT_STEPS_LIST_ENTRY(vap_manager, PROCESS_TA)
MTLK_INIT_INNER_STEPS_BEGIN(vap_manager)
MTLK_INIT_STEPS_LIST_END(vap_manager);

MTLK_INIT_STEPS_LIST_BEGIN(vap_handler)
  MTLK_INIT_STEPS_LIST_ENTRY(vap_handler, VAP_HANDLER_PDB_CREATE)
  MTLK_INIT_STEPS_LIST_ENTRY(vap_handler, VAP_HANDLER_DF_CREATE)
  MTLK_INIT_STEPS_LIST_ENTRY(vap_handler, VAP_HANDLER_SECNDRY_DF_CREATE)
  MTLK_INIT_STEPS_LIST_ENTRY(vap_handler, VAP_HANDLER_ABMGR_CREATE)
  MTLK_INIT_STEPS_LIST_ENTRY(vap_handler, VAP_HANDLER_CORE_API_ALLOC)
  MTLK_INIT_STEPS_LIST_ENTRY(vap_handler, VAP_HANDLER_CORE_API_INIT)
#ifdef MTLK_WAVE_700
  MTLK_INIT_STEPS_LIST_ENTRY(vap_handler, ML_VAP_INFO)
#endif
  MTLK_INIT_STEPS_LIST_ENTRY(vap_handler, VAP_HANDLER_IRBD_ALLOC)
  MTLK_INIT_STEPS_LIST_ENTRY(vap_handler, VAP_HANDLER_PROC_REGISTER)
MTLK_INIT_INNER_STEPS_BEGIN(vap_handler)
MTLK_INIT_STEPS_LIST_END(vap_handler);

MTLK_START_STEPS_LIST_BEGIN(vap_handler)
  MTLK_START_STEPS_LIST_ENTRY(vap_handler, VAP_HANDLER_IRBD_INIT)
  MTLK_START_STEPS_LIST_ENTRY(vap_handler, VAP_HANDLER_CORE_START)
  MTLK_START_STEPS_LIST_ENTRY(vap_handler, VAP_HANDLER_DF_START)
  MTLK_START_STEPS_LIST_ENTRY(vap_handler, VAP_HANDLER_SECNDRY_DF_START)
MTLK_START_INNER_STEPS_BEGIN(vap_handler)
MTLK_START_STEPS_LIST_END(vap_handler);

static __INLINE mtlk_vap_handle_t
__mtlk_vap_manager_vap_handle_by_id (mtlk_vap_manager_t* obj, uint8 vap_id)
{
  MTLK_ASSERT(obj != NULL);
  MTLK_ASSERT(vap_id < obj->max_vaps_count);

  if (__LIKELY(obj->guest_vap_array)) {
    return obj->guest_vap_array[vap_id];
  }

  return NULL;
}

static __INLINE mtlk_vap_handle_t
__mtlk_vap_manager_vap_handle_by_secondary_id (mtlk_vap_manager_t* obj, uint8 vap_id)
{
  MTLK_ASSERT(obj != NULL);
  MTLK_ASSERT(vap_id < obj->max_vaps_count);

  if (__LIKELY(obj->secondary_id_vap_array)) {
    return obj->secondary_id_vap_array[vap_id];
  }

  return NULL;
}

static void __MTLK_IFUNC
_mtlk_vap_manager_cleanup (mtlk_vap_manager_t* obj)
{
    MTLK_ASSERT(NULL != obj);

    MTLK_CLEANUP_BEGIN(vap_manager, MTLK_OBJ_PTR(obj))
      MTLK_CLEANUP_STEP(vap_manager, PROCESS_TA, MTLK_OBJ_PTR(obj),
                        mtlk_ta_delete, (obj->ta));

      MTLK_CLEANUP_STEP(vap_manager, VAP_MANAGER_PREPARE_SYNCHRONIZATION, MTLK_OBJ_PTR(obj),
                        mtlk_osal_lock_cleanup, (&obj->guest_lock));
    MTLK_CLEANUP_END(vap_manager, MTLK_OBJ_PTR(obj));
}

static int __MTLK_IFUNC
_mtlk_vap_manager_init (mtlk_vap_manager_t* obj, void *radio, unsigned radio_id, mtlk_hw_api_t *hw_api, mtlk_work_mode_e work_mode)
{
    MTLK_ASSERT(NULL != obj);

    MTLK_INIT_TRY(vap_manager, MTLK_OBJ_PTR(obj))
      obj->hw_api = hw_api;
      obj->work_mode = work_mode;
      obj->max_vaps_count = 1;
      obj->radio = radio;
      obj->radio_id = radio_id;
      mtlk_osal_atomic_set(&obj->master_vap_index, MTLK_VAP_INVALID_IDX);
      mtlk_osal_atomic_set(&obj->activated_vaps_num, 0);
      mtlk_osal_atomic_set(&obj->wds_bridgemode, 0);

      MTLK_INIT_STEP(vap_manager, VAP_MANAGER_PREPARE_SYNCHRONIZATION, MTLK_OBJ_PTR(obj),
                     mtlk_osal_lock_init, (&obj->guest_lock));

      MTLK_INIT_STEP_EX(vap_manager, PROCESS_TA, MTLK_OBJ_PTR(obj),
                        mtlk_ta_create, (obj),
                        obj->ta,
                        obj->ta != MTLK_INVALID_HANDLE, MTLK_ERR_NO_MEM);

    MTLK_INIT_FINALLY(vap_manager, MTLK_OBJ_PTR(obj))
    MTLK_INIT_RETURN(vap_manager, MTLK_OBJ_PTR(obj), _mtlk_vap_manager_cleanup, (obj))
}

static void __MTLK_IFUNC
_mtlk_vap_manager_set_master_vap(mtlk_vap_manager_t *obj, u32 index)
{
  MTLK_ASSERT(NULL != obj);

  return mtlk_osal_atomic_set(&obj->master_vap_index, index);
}

uint32 __MTLK_IFUNC
mtlk_vap_manager_get_master_vap_index(mtlk_vap_manager_t *obj)
{
  MTLK_ASSERT(NULL != obj);

  return mtlk_osal_atomic_get(&obj->master_vap_index);
}

mtlk_vap_manager_t * __MTLK_IFUNC 
mtlk_vap_manager_create(void *radio, unsigned radio_id, mtlk_hw_api_t *hw_api, mtlk_work_mode_e work_mode)
{
    mtlk_vap_manager_t *vap_manager;

    MTLK_ASSERT(hw_api != NULL);

    vap_manager = (mtlk_vap_manager_t *)mtlk_osal_mem_alloc(sizeof(mtlk_vap_manager_t), MTLK_MEM_TAG_VAP_MANAGER);
    if(NULL == vap_manager) {
      return NULL;
    }

    memset(vap_manager, 0, sizeof(mtlk_vap_manager_t));

    if (MTLK_ERR_OK != _mtlk_vap_manager_init(vap_manager, radio, radio_id, hw_api, work_mode)) {
      mtlk_osal_mem_free(vap_manager);
      return NULL;
    }

    return vap_manager;
}

void __MTLK_IFUNC
mtlk_vap_manager_delete (mtlk_vap_manager_t *obj)
{
  _mtlk_vap_manager_cleanup(obj);
  mtlk_osal_mem_free(obj);
}

static int
_wave_vap_manager_txmm_txdm_set(mtlk_vap_handle_t vap_handle)
{
  mtlk_vap_info_internal_t *vap_info;
  mtlk_txmm_base_t         *txmm_base;
  mtlk_txmm_base_t         *txdm_base;
  mtlk_hw_api_t            *hw_api;
  int                       res = MTLK_ERR_OK;

  MTLK_ASSERT(NULL != vap_handle);
  vap_info = (mtlk_vap_info_internal_t *)vap_handle;

  hw_api = mtlk_vap_get_hwapi(vap_handle);

  res = mtlk_hw_get_prop(hw_api, MTLK_HW_PROP_TXMM_BASE, &txmm_base, sizeof(typeof(txmm_base)));
  if (res != MTLK_ERR_OK) {
    return res;
  }

  res = mtlk_hw_get_prop(hw_api, MTLK_HW_PROP_TXDM_BASE, &txdm_base, sizeof(typeof(txdm_base)));
  if (res != MTLK_ERR_OK) {
    return res;
  }

  vap_info->txmm.base = txmm_base;
  vap_info->txmm.vap_handle = vap_handle;
  vap_info->txmm.hw_api = hw_api;
  vap_info->txdm.base = txdm_base;
  vap_info->txdm.vap_handle = vap_handle;
  vap_info->txdm.hw_api = hw_api;
  ILOG2_V("txmm-txdm init done");
  return res;
}

int __MTLK_IFUNC
mtlk_vap_manager_create_vap (mtlk_vap_manager_t *obj,
                             uint8               vap_index,
                             const char         *name,
                             mtlk_work_role_e   role,
                             BOOL               is_master,
                             struct net_device *ndev)
{
  mtlk_vap_handle_t vap_handle = MTLK_INVALID_VAP_HANDLE;
  int               res_value;
  BOOL              prepared_vaps = FALSE;

  MTLK_ASSERT(NULL != obj);

  if (!obj->guest_vap_array) {
    res_value = mtlk_vap_manager_prepare_vaps(obj);
    if (MTLK_ERR_OK != res_value) {
      ELOG_V("Failed to prepare vaps");
      goto end;
    }
    prepared_vaps = TRUE;
  }

  if (vap_index >= obj->max_vaps_count) {
    ELOG_D("Invalid VapID %u", vap_index);
    res_value = MTLK_ERR_PARAMS;
    goto free_prepare;
  }

  vap_handle = __mtlk_vap_manager_vap_handle_by_id(obj, vap_index);
  if (NULL != vap_handle) {
    ELOG_DD("CID-%04x: VapID %u already exists", mtlk_vap_get_oid(vap_handle), vap_index);
    res_value = MTLK_ERR_PARAMS;
    goto free_prepare;
  }

  vap_handle = (mtlk_vap_handle_t)mtlk_osal_mem_alloc(sizeof(struct _mtlk_vap_handle_t), MTLK_MEM_TAG_VAP_NODE);
  if (NULL == vap_handle) {
    ELOG_D("VapID %u can not to be created", vap_index);
    res_value = MTLK_ERR_NO_MEM;
    goto free_prepare;
  }
  memset(vap_handle, 0, sizeof(struct _mtlk_vap_handle_t));
  res_value = _mtlk_vap_init(vap_handle, obj, vap_index, name, role, is_master, ndev);

  if (MTLK_ERR_OK != res_value) {
    ELOG_D("VapID %u can not to be initialized", vap_index);
    goto free_all;
  }

  res_value = _wave_vap_manager_txmm_txdm_set(vap_handle);
  if (MTLK_ERR_OK != res_value) {
    ELOG_V("Failed to set txmm-txdm for VAP");
    goto free_all;
  }

  MTLK_ASSERT(NULL != obj->guest_vap_array);
  MTLK_ASSERT(NULL != obj->secondary_id_vap_array);
  obj->guest_vap_array[vap_index] = vap_handle;
  obj->secondary_id_vap_array[vap_index] = vap_handle;

  if (!mtlk_vap_is_master(vap_handle) && !mtlk_vap_is_dut(vap_handle))
  {
    res_value = mtlk_vap_start(vap_handle);
    if (MTLK_ERR_OK != res_value) {
      ELOG_DDD("CID-%04x: Can't start VapID %u res=%d", mtlk_vap_get_oid(vap_handle), vap_index, res_value);
      mtlk_vap_delete(vap_handle);
      goto free_prepare;
    }
  }
  goto end;

free_all:
  mtlk_osal_mem_free(vap_handle);
free_prepare:
  if (prepared_vaps) {
    obj->max_vaps_count = 0;
    mtlk_osal_mem_free(obj->e_vap_status);
    obj->e_vap_status = NULL;
    mtlk_osal_mem_free(obj->core_array);
    obj->core_array = NULL;
    mtlk_osal_mem_free(obj->guest_vap_array);
    obj->guest_vap_array = NULL;
  }
end:
  return res_value;
}

mtlk_error_t __MTLK_IFUNC
mtlk_vap_manager_get_free_vap_index (mtlk_vap_manager_t *obj, uint32 *vap_index, mtlk_work_role_e role)
{
  uint32 i;
  mtlk_error_t res = MTLK_ERR_NO_RESOURCES;

  MTLK_ASSERT(NULL != obj);

  if (NULL == obj->guest_vap_array) {
    res = mtlk_vap_manager_prepare_vaps(obj);
    if (MTLK_ERR_OK != res) {
      return res;
    }
  }

  if (role == MTLK_ROLE_STA && !obj->guest_vap_array[obj->max_vaps_count - 2] && !obj->secondary_id_vap_array[obj->max_vaps_count - 2]) {
      *vap_index = obj->max_vaps_count - 2;  //Master vap takes the last vap_id of band.So assign prev vap_id to last vap_id for sta vap.
      res = MTLK_ERR_OK;
      return res;
  }

  for (i = 0; i < obj->max_vaps_count; i++)
  {
    if (!obj->guest_vap_array[i] && !obj->secondary_id_vap_array[i]) {
      *vap_index = i;
      res = MTLK_ERR_OK;
      break;
    }
  }

  return res;
}

mtlk_error_t __MTLK_IFUNC
mtlk_vap_manager_check_free_vap_index (mtlk_vap_manager_t *obj, uint32 vap_index)
{
  mtlk_error_t res = MTLK_ERR_NO_RESOURCES;

  MTLK_ASSERT(NULL != obj);

  if (NULL == obj->guest_vap_array) {
    res = mtlk_vap_manager_prepare_vaps(obj);
    if (MTLK_ERR_OK != res) {
      return res;
    }
  }

  if ((vap_index < obj->max_vaps_count) && (!obj->guest_vap_array[vap_index])) {
    res = MTLK_ERR_OK;
  }

  return res;
}

void __MTLK_IFUNC
mtlk_vap_manager_set_master_ndev_taken (mtlk_vap_manager_t *obj, BOOL master_ndev_taken){
  obj->master_ndev_taken = master_ndev_taken;
}

BOOL __MTLK_IFUNC
mtlk_vap_manager_get_master_ndev_taken (mtlk_vap_manager_t *obj){
  return obj->master_ndev_taken;
}

BOOL __MTLK_IFUNC
mtlk_vap_manager_did_ap_started (mtlk_vap_manager_t *obj){
  uint32 i;
  for (i = 0; i < obj->max_vaps_count; i++)
  {
    if (obj->guest_vap_array[i]) {
      mtlk_core_t * nic = mtlk_vap_get_core(obj->guest_vap_array[i]);
      if(nic->slow_ctx->ap_started){
        return TRUE;
      }
    }
  }
  return FALSE;
}

int __MTLK_IFUNC
mtlk_vap_manager_prepare_vaps (mtlk_vap_manager_t *obj)
{
  struct _mtlk_vap_handle_t **guest_vap_array;
  struct _mtlk_vap_handle_t **secondary_id_vap_array;
  mtlk_core_t               **core_array;
  uint32                      max_vaps_count;
  mtlk_vap_status_t          *e_vap_status;

  MTLK_ASSERT(NULL != obj);

  max_vaps_count = wave_radio_max_vaps_get(obj->radio);

  guest_vap_array = (struct _mtlk_vap_handle_t **)mtlk_osal_mem_alloc((sizeof(struct _mtlk_vap_handle_t *) * max_vaps_count), MTLK_MEM_TAG_VAP_NODE);
  if (NULL == guest_vap_array) {
    return MTLK_ERR_NO_MEM;
  }
  memset(guest_vap_array, 0, (sizeof(struct _mtlk_vap_handle_t *) * max_vaps_count));

  secondary_id_vap_array = (struct _mtlk_vap_handle_t **)mtlk_osal_mem_alloc((sizeof(struct _mtlk_vap_handle_t *) * max_vaps_count), MTLK_MEM_TAG_VAP_NODE);
  if (NULL == secondary_id_vap_array) {
    mtlk_osal_mem_free(guest_vap_array);
    return MTLK_ERR_NO_MEM;
  }
  memset(secondary_id_vap_array, 0, (sizeof(struct _mtlk_vap_handle_t *) * max_vaps_count));

  core_array = (mtlk_core_t **)mtlk_osal_mem_alloc((sizeof(mtlk_core_t *) * max_vaps_count), MTLK_MEM_TAG_VAP_NODE);
  if (NULL == core_array) {
    mtlk_osal_mem_free(guest_vap_array);
    mtlk_osal_mem_free(secondary_id_vap_array);
    return MTLK_ERR_NO_MEM;
  }
  memset(core_array, 0, (sizeof(mtlk_core_t *) * max_vaps_count));

  e_vap_status = (mtlk_vap_status_t *)mtlk_osal_mem_alloc((sizeof(mtlk_vap_status_t) * max_vaps_count), MTLK_MEM_TAG_VAP_NODE);
  if (NULL == e_vap_status) {
    mtlk_osal_mem_free(guest_vap_array);
    mtlk_osal_mem_free(secondary_id_vap_array);
    mtlk_osal_mem_free(core_array);
    return MTLK_ERR_NO_MEM;
  }
  memset(e_vap_status, VAP_STATUS_FIRST, (sizeof(mtlk_vap_status_t) * max_vaps_count));

  obj->max_vaps_count = max_vaps_count;
  obj->guest_vap_array = guest_vap_array;
  obj->secondary_id_vap_array = secondary_id_vap_array;
  obj->core_array = core_array;
  obj->e_vap_status = e_vap_status;
  mtlk_vap_manager_set_master_ndev_taken(obj, FALSE);
  return MTLK_ERR_OK;
}

void __MTLK_IFUNC
mtlk_vap_manager_deallocate_vaps (mtlk_vap_manager_t *obj)
{
  int vap_index = 0;
  int max_vaps_count = 0;
  mtlk_vap_handle_t vap_handle;
  mtlk_df_user_t *df_user;
  MTLK_ASSERT(NULL != obj);

  if (mtlk_vap_manager_get_master_vap(obj, &vap_handle) == MTLK_ERR_OK) {
    df_user     = mtlk_df_get_user(mtlk_vap_get_df(vap_handle));

    if (!mtlk_vap_manager_get_master_ndev_taken(obj)) {
      free_netdev(mtlk_df_user_get_ndev(df_user));
      ILOG0_V("Removing master vap netdev from driver");
    }
    else {
      unregister_netdevice(mtlk_df_user_get_ndev(df_user));
    }
  }

  max_vaps_count = mtlk_vap_manager_get_max_vaps_count(obj);
  /* deallocate memory for all vaps including master vap */
  for (vap_index = 0; vap_index < max_vaps_count; vap_index++) {
    mtlk_vap_handle_t vap_handle = __mtlk_vap_manager_vap_handle_by_id(obj, vap_index);
    if (NULL != vap_handle) {
      mtlk_vap_delete(vap_handle);
    }
  }

  if (obj->guest_vap_array) {
    mtlk_osal_mem_free(obj->guest_vap_array);
    obj->guest_vap_array = NULL;
  }

  if (obj->secondary_id_vap_array) {
    mtlk_osal_mem_free(obj->secondary_id_vap_array);
    obj->secondary_id_vap_array = NULL;
  }

  if (obj->core_array) {
    mtlk_osal_mem_free(obj->core_array);
    obj->core_array = NULL;
  }

  if (obj->e_vap_status) {
    mtlk_osal_mem_free(obj->e_vap_status);
    obj->e_vap_status = NULL;
  }
}

void __MTLK_IFUNC
mtlk_vap_manager_prepare_stop(mtlk_vap_manager_t *obj)
{
  int vap_index = 0;
  int max_vaps_count = 0;
  MTLK_ASSERT(NULL != obj);

  max_vaps_count = mtlk_vap_manager_get_max_vaps_count(obj);
  for (vap_index = 0; vap_index < max_vaps_count; vap_index++) {
    mtlk_vap_handle_t vap_handle = __mtlk_vap_manager_vap_handle_by_id(obj, vap_index);
    if (NULL != vap_handle) {
      mtlk_vap_get_core_vft(vap_handle)->prepare_stop(vap_handle);
    }
  }
}

#ifdef MTLK_WAVE_700
mtlk_error_t __MTLK_IFUNC
wave_vap_manager_get_vap_handle_by_fw_id (mtlk_vap_manager_t *obj,
                                         uint8              vap_id,
                                         mtlk_vap_handle_t  *vap_handle)
{
  int vap_index;
  int max_vaps_count = 0;
  MTLK_ASSERT(NULL != obj);

  max_vaps_count = mtlk_vap_manager_get_max_vaps_count(obj);
  for (vap_index = 0; vap_index < max_vaps_count; vap_index++) {
    mtlk_vap_handle_t vap = __mtlk_vap_manager_vap_handle_by_id(obj, vap_index);
    if (vap && vap_id == mtlk_vap_get_id_fw(vap)) {
      *vap_handle = vap;
      return MTLK_ERR_OK;
    }
  }
  return MTLK_ERR_PARAMS;
}

void __MTLK_IFUNC
wave_vap_manager_update_ml_vap_info (mtlk_vap_handle_t vap_handle,
                                    mtlk_ml_vap_info_t ml_vap_info)
{
  mtlk_vap_info_internal_t *_info = (mtlk_vap_info_internal_t *)vap_handle;
  mtlk_vap_info_internal_t *_sibling_info = (mtlk_vap_info_internal_t *)ml_vap_info.sibling;

  MTLK_ASSERT(NULL != _info);
  MTLK_ASSERT(NULL != _sibling_info);

#ifdef BEST_EFFORT_TID_SPREADING
  /* update ml vap str tid spreading info */
  _info->ml_vap_info.tid_spread_info = ml_vap_info.tid_spread_info;
  _sibling_info->ml_vap_info.tid_spread_info = ml_vap_info.tid_spread_info;
#endif

  /* update sibling vap info */
  _info->ml_vap_info.sibling = ml_vap_info.sibling;
  _info->ml_vap_info.ml_configured = TRUE;
  _sibling_info->ml_vap_info.sibling = vap_handle;
  _sibling_info->ml_vap_info.ml_configured = TRUE;
  _info->ml_vap_info.ml_vap_rem_sync_lock = ml_vap_info.ml_vap_rem_sync_lock;
  _sibling_info->ml_vap_info.ml_vap_rem_sync_lock = ml_vap_info.ml_vap_rem_sync_lock;
}

void __MTLK_IFUNC
mtlk_vap_wait_ml_teardown(mtlk_vap_handle_t vap_handle)
{
  mtlk_vap_info_internal_t *_info = (mtlk_vap_info_internal_t *)vap_handle;
  int res;

  res = mtlk_osal_event_wait(&_info->ml_vap_info.ml_teardown_completed, 2000);
  if (MTLK_ERR_OK != res) {
    ELOG_D("ML vap teardown completed event wait error, teardown_count= %d",
           mtlk_osal_atomic_get(&_info->ml_vap_info.ml_teardown_initiated));
  }
}

int __MTLK_IFUNC
mtlk_vap_initiate_ml_teardown(mtlk_vap_handle_t vap_handle)
{
  mtlk_vap_info_internal_t *_info = (mtlk_vap_info_internal_t *)vap_handle;
  uint32 ref_cnt;

  ref_cnt = mtlk_osal_atomic_inc(&_info->ml_vap_info.ml_teardown_initiated);
  return ref_cnt;
}

int __MTLK_IFUNC
mtlk_vap_ml_teardown_inprogress(mtlk_vap_handle_t vap_handle)
{
  mtlk_vap_info_internal_t *_info = (mtlk_vap_info_internal_t *)vap_handle;
  uint32 ref_cnt;

  ref_cnt = mtlk_osal_atomic_get(&_info->ml_vap_info.ml_teardown_initiated);
  return ref_cnt;
}

void __MTLK_IFUNC
mtlk_vap_finish_ml_teardown(mtlk_vap_handle_t vap_handle)
{
  mtlk_vap_info_internal_t *_info = (mtlk_vap_info_internal_t *)vap_handle;
  uint32 ref_cnt;

  ref_cnt = mtlk_osal_atomic_dec(&_info->ml_vap_info.ml_teardown_initiated);
  if (ref_cnt == 0) {
    mtlk_osal_event_set(&_info->ml_vap_info.ml_teardown_completed);
  }
}

void __MTLK_IFUNC
mtlk_vap_ml_lock_acquire(mtlk_vap_handle_t vap_handle)
{
  mtlk_vap_info_internal_t *_info = (mtlk_vap_info_internal_t *)vap_handle;

  if (_info->ml_vap_info.ml_vap_rem_sync_lock) {
    mtlk_osal_lock_acquire(_info->ml_vap_info.ml_vap_rem_sync_lock);
  }
}

void __MTLK_IFUNC
mtlk_vap_ml_lock_release(mtlk_vap_handle_t vap_handle)
{
  mtlk_vap_info_internal_t *_info = (mtlk_vap_info_internal_t *)vap_handle;

  if (_info->ml_vap_info.ml_vap_rem_sync_lock) {
    mtlk_osal_lock_release(_info->ml_vap_info.ml_vap_rem_sync_lock);
  }
}

static void
mtlk_ml_vap_event_cleanup(mtlk_vap_handle_t vap_handle)
{
  mtlk_vap_info_internal_t *_info = (mtlk_vap_info_internal_t *)vap_handle;

  mtlk_osal_event_cleanup(&_info->ml_vap_info.ml_teardown_completed);
}

static int
mtlk_ml_vap_event_init(mtlk_vap_handle_t vap_handle)
{
  mtlk_vap_info_internal_t *_info = (mtlk_vap_info_internal_t *)vap_handle;

  return mtlk_osal_event_init(&_info->ml_vap_info.ml_teardown_completed);
}
#endif

void __MTLK_IFUNC
mtlk_vap_manager_stop_all_vaps(mtlk_vap_manager_t *obj)
{
  int vap_index = 0;
  int max_vaps_count = 0;
  MTLK_ASSERT(NULL != obj);

  max_vaps_count = mtlk_vap_manager_get_max_vaps_count(obj);
  for (vap_index = 0; vap_index < max_vaps_count; vap_index++) {
    mtlk_vap_handle_t vap_handle = __mtlk_vap_manager_vap_handle_by_id(obj, vap_index);
    if (NULL != vap_handle) {
      mtlk_vap_stop(vap_handle);
    }
  }
}

int __MTLK_IFUNC
mtlk_vap_manager_get_master_vap (mtlk_vap_manager_t *obj,
                                 mtlk_vap_handle_t  *vap_handle)
{
  mtlk_vap_handle_t _vap_handle;
  uint32 master_vap_idx;

  MTLK_ASSERT(obj != NULL);
  MTLK_ASSERT(vap_handle != NULL);

  master_vap_idx = mtlk_vap_manager_get_master_vap_index(obj);
  if (__UNLIKELY(master_vap_idx == MTLK_VAP_INVALID_IDX)){
    WLOG_V("Master VAP is not selected");
    return MTLK_ERR_NOT_IN_USE;
  }

  _vap_handle = __mtlk_vap_manager_vap_handle_by_id(obj, master_vap_idx);
  *vap_handle = _vap_handle;

  if (__UNLIKELY(NULL == _vap_handle)) {
    ELOG_V("Get Master VAP handle failed!");
    return MTLK_ERR_NOT_IN_USE;
  }

  return MTLK_ERR_OK;
}

int __MTLK_IFUNC
mtlk_vap_manager_get_vap_handle (mtlk_vap_manager_t *obj,
                                 uint8               vap_id,
                                 mtlk_vap_handle_t  *vap_handle)
{
  mtlk_vap_handle_t _vap_handle;

  MTLK_ASSERT(obj != NULL);
  MTLK_ASSERT(vap_handle != NULL);

  if (__UNLIKELY(!obj)) {
    return MTLK_ERR_NOT_IN_USE;
  }

  if (__UNLIKELY(vap_id >= obj->max_vaps_count)) {
    return MTLK_ERR_PARAMS;
  }

  _vap_handle = __mtlk_vap_manager_vap_handle_by_id(obj, vap_id);
  if (__UNLIKELY(NULL == _vap_handle)) {
    return MTLK_ERR_NOT_IN_USE;
  }

  *vap_handle = _vap_handle;

  return MTLK_ERR_OK;
}

int __MTLK_IFUNC
mtlk_vap_manager_get_vap_handle_by_secondary_id (mtlk_vap_manager_t *obj,
                                                 uint8               vap_id,
                                                 mtlk_vap_handle_t  *vap_handle)
{
  mtlk_vap_handle_t _vap_handle;

  MTLK_ASSERT(obj != NULL);
  MTLK_ASSERT(vap_handle != NULL);

  if (__UNLIKELY(!obj)) {
    return MTLK_ERR_NOT_IN_USE;
  }

  if (__UNLIKELY(vap_id >= obj->max_vaps_count)) {
    return MTLK_ERR_PARAMS;
  }

  _vap_handle = __mtlk_vap_manager_vap_handle_by_secondary_id(obj, vap_id);
  if (__UNLIKELY(NULL == _vap_handle)) {
    return MTLK_ERR_NOT_IN_USE;
  }

  *vap_handle = _vap_handle;

  return MTLK_ERR_OK;
}

BOOL __MTLK_IFUNC
mtlk_vap_manager_vap_is_not_ready (mtlk_vap_manager_t *obj, uint8 vap_id)
{
  if (__UNLIKELY(!obj)) {
    MTLK_ASSERT(FALSE);
    return TRUE; /* VAP is not ready */
  }
  if (__UNLIKELY(vap_id >= obj->max_vaps_count)) {
    MTLK_ASSERT(FALSE);
    return TRUE; /* VAP is not ready */
  }

  return (obj->e_vap_status[vap_id] != VAP_STATUS_READY);
}


mtlk_hw_api_t * __MTLK_IFUNC
mtlk_vap_manager_get_hwapi(mtlk_vap_manager_t *obj)
{
  MTLK_ASSERT(NULL != obj);

  return obj->hw_api;
}

void __MTLK_IFUNC
mtlk_vap_manager_notify_vap_activated(mtlk_vap_manager_t *obj)
{
  MTLK_ASSERT(NULL != obj);

  mtlk_osal_atomic_inc(&obj->activated_vaps_num);
}

void __MTLK_IFUNC
mtlk_vap_manager_notify_vap_deactivated(mtlk_vap_manager_t *obj)
{
  MTLK_ASSERT(NULL != obj);

  mtlk_osal_atomic_dec(&obj->activated_vaps_num);
}

uint32 __MTLK_IFUNC
mtlk_vap_manager_get_active_vaps_number(mtlk_vap_manager_t *obj)
{
  MTLK_ASSERT(NULL != obj);

  return mtlk_osal_atomic_get(&obj->activated_vaps_num);
}

void __MTLK_IFUNC
mtlk_vap_manager_set_active_vaps_number(mtlk_vap_manager_t *obj, uint32 num)
{
  MTLK_ASSERT(NULL != obj);

  return mtlk_osal_atomic_set(&obj->activated_vaps_num, num);
}

uint32 __MTLK_IFUNC
mtlk_vap_manager_get_max_vaps_count (mtlk_vap_manager_t *obj)
{
  MTLK_ASSERT(obj != NULL);

  return obj->max_vaps_count;
}
#ifdef UNUSED_CODE
/* Always call this function right before using core array.
 * It becomes invalid each time when another VAP is created or deleted.
 */
mtlk_core_t ** __MTLK_IFUNC
mtlk_vap_get_core_array (mtlk_vap_manager_t *obj, uint32 *core_cnt)
{
  unsigned vap_index = 0;
  unsigned max_vaps_count = 0;
  mtlk_vap_handle_t vap_handle = NULL;

  MTLK_ASSERT(NULL != obj);
  MTLK_ASSERT(NULL != obj->core_array);
  MTLK_ASSERT(NULL != core_cnt);

  *core_cnt = 0;
  max_vaps_count = mtlk_vap_manager_get_max_vaps_count(obj);

  /* Prevent accumulation of stale pointers between calls */
  memset(obj->core_array, 0, (sizeof(mtlk_core_t *) * max_vaps_count));

  for (vap_index = 0; vap_index < max_vaps_count; vap_index++) {
    vap_handle = __mtlk_vap_manager_vap_handle_by_id(obj, vap_index);
    if (NULL != vap_handle) {
      obj->core_array[*core_cnt] = mtlk_vap_get_core(vap_handle);
      (*core_cnt)++;
    }
  }

  return obj->core_array;
}
#endif /* UNUSED_CODE */

bool __MTLK_IFUNC
mtlk_vap_is_sta_in_cca (mtlk_vap_manager_t *obj)
{
  bool res = FALSE;
  unsigned vap_index = 0;
  unsigned max_vaps_count = 0;
  mtlk_vap_handle_t vap_handle = NULL;

  MTLK_ASSERT(NULL != obj);

  max_vaps_count = mtlk_vap_manager_get_max_vaps_count(obj);

  for (vap_index = 0; vap_index < max_vaps_count; vap_index++) {
    vap_handle = __mtlk_vap_manager_vap_handle_by_id(obj, vap_index);
    if (NULL != vap_handle) {
      mtlk_core_t *cur_nic = mtlk_vap_get_core(vap_handle);
      if(cur_nic->color_change_active && mtlk_vap_is_sta(cur_nic->vap_handle)) {
        res = TRUE;
        return res;
      }
    }
  }

  return res;  
}

mtlk_core_t * __MTLK_IFUNC
mtlk_vap_get_core (mtlk_vap_handle_t vap_handle)
{
  mtlk_vap_info_internal_t *_info = (mtlk_vap_info_internal_t *)vap_handle;

  MTLK_ASSERT(NULL != _info);
  MTLK_ASSERT(NULL != _info->core_api);

  return HANDLE_T_PTR(mtlk_core_t, _info->core_api->core_handle);
}

mtlk_core_vft_t const * __MTLK_IFUNC
mtlk_vap_get_core_vft (mtlk_vap_handle_t vap_handle)
{
  mtlk_vap_info_internal_t *_info = (mtlk_vap_info_internal_t *)vap_handle;

  MTLK_ASSERT(NULL != _info);
  MTLK_ASSERT(NULL != _info->core_api);
  MTLK_ASSERT(NULL != _info->core_api->vft);

  return _info->core_api->vft;
}

mtlk_work_mode_e __MTLK_IFUNC
mtlk_vap_manager_get_mode(mtlk_vap_manager_t *obj)
{
  MTLK_ASSERT(NULL != obj);

  return obj->work_mode;
}

mtlk_hw_t* __MTLK_IFUNC
mtlk_vap_get_hw(mtlk_vap_handle_t vap_handle)
{
  mtlk_vap_info_internal_t *_info = (mtlk_vap_info_internal_t *)vap_handle;

  MTLK_ASSERT(NULL != _info);
  MTLK_ASSERT(NULL != _info->manager);
  MTLK_ASSERT(NULL != _info->manager->hw_api);
  MTLK_ASSERT(NULL != _info->manager->hw_api->hw);

  return _info->manager->hw_api->hw;
}

mtlk_hw_t* __MTLK_IFUNC
mtlk_vap_manager_get_hw (mtlk_vap_manager_t *obj)
{
  MTLK_ASSERT(NULL != obj);
  MTLK_ASSERT(NULL != obj->hw_api);
  MTLK_ASSERT(NULL != obj->hw_api->hw);

  return obj->hw_api->hw;
}

static mtlk_df_t ** _mtlk_vap_secondary_df_array_create(mtlk_vap_handle_t vap_handle)
{
  mtlk_vap_info_internal_t *_info;
  mtlk_vap_manager_t *obj = NULL;
  mtlk_df_t **secondary_dfs;
  uint32 max_vaps_count;

  MTLK_ASSERT(NULL != vap_handle);
  _info = (mtlk_vap_info_internal_t *)vap_handle;

  obj = mtlk_vap_get_manager(vap_handle);
  MTLK_ASSERT(NULL != obj);

  max_vaps_count = obj->max_vaps_count;
  secondary_dfs = mtlk_osal_mem_alloc((sizeof(mtlk_df_t *) * max_vaps_count),
                                      MTLK_MEM_TAG_VAP_NODE);
  if (NULL == secondary_dfs)
    return NULL;

  memset(secondary_dfs, 0, sizeof(mtlk_df_t *) * max_vaps_count);
  secondary_dfs[_info->id] = _info->df;

  return secondary_dfs;
}

static int
_mtlk_vap_reg_proc_file (mtlk_vap_info_internal_t *vap_info, const proc_file_info_t *info)
{
  return mtlk_df_reg_file(mtlk_df_get_user(vap_info->df), info);
}

static void
_mtlk_vap_unreg_proc_file (mtlk_vap_info_internal_t *vap_info, const proc_file_info_t *info)
{
  mtlk_df_unreg_file(mtlk_df_get_user(vap_info->df), info);
}

static void __MTLK_IFUNC
_mtlk_vap_cleanup (mtlk_vap_handle_t vap_handle)
{
  int i;
  mtlk_df_user_t *df_user;
  mtlk_vap_info_internal_t *vap_info;

  MTLK_ASSERT(NULL != vap_handle);
  vap_info = (mtlk_vap_info_internal_t *)vap_handle;
  df_user = mtlk_df_get_user(vap_info->df);

  MTLK_CLEANUP_BEGIN(vap_handler, MTLK_OBJ_PTR(vap_info))
    if (!mtlk_df_user_is_secondary(df_user)) {
      int size = 0;
      const proc_file_info_t *pinfo = wave_get_proc_table(&size);
      for (i = 0; MTLK_CLEANUP_ITERATONS_LEFT(MTLK_OBJ_PTR(vap_info), VAP_HANDLER_PROC_REGISTER) > 0; i++)
      {
        MTLK_CLEANUP_STEP_LOOP(vap_handler, VAP_HANDLER_PROC_REGISTER, MTLK_OBJ_PTR(vap_info),
                               _mtlk_vap_unreg_proc_file, (vap_info, &pinfo[i]));
      }
    }
    MTLK_CLEANUP_STEP(vap_handler, VAP_HANDLER_IRBD_ALLOC, MTLK_OBJ_PTR(vap_info),
                      mtlk_irbd_free, (vap_info->irbd));
#ifdef MTLK_WAVE_700
    MTLK_CLEANUP_STEP(vap_handler, ML_VAP_INFO, MTLK_OBJ_PTR(vap_info),
                      mtlk_ml_vap_event_cleanup, (vap_handle));
#endif
    MTLK_CLEANUP_STEP(vap_handler, VAP_HANDLER_CORE_API_INIT, MTLK_OBJ_PTR(vap_info),
                      mtlk_core_api_cleanup, (vap_info->core_api));
    MTLK_CLEANUP_STEP(vap_handler, VAP_HANDLER_CORE_API_ALLOC, MTLK_OBJ_PTR(vap_info),
                      mtlk_osal_mem_free, (vap_info->core_api));
    MTLK_CLEANUP_STEP(vap_handler, VAP_HANDLER_ABMGR_CREATE, MTLK_OBJ_PTR(vap_info),
                      mtlk_abmgr_delete, (vap_info->abmgr));
    MTLK_CLEANUP_STEP(vap_handler, VAP_HANDLER_SECNDRY_DF_CREATE, MTLK_OBJ_PTR(vap_info),
                      mtlk_osal_mem_free, (vap_info->secondary_dfs));
    MTLK_CLEANUP_STEP(vap_handler, VAP_HANDLER_DF_CREATE, MTLK_OBJ_PTR(vap_info),
                      mtlk_df_delete, (vap_info->df));
    MTLK_CLEANUP_STEP(vap_handler, VAP_HANDLER_PDB_CREATE, MTLK_OBJ_PTR(vap_info),
                      wave_pdb_delete, (vap_info->param_db));
  MTLK_CLEANUP_END(vap_handler, MTLK_OBJ_PTR(vap_info))
}

extern const char *mtlk_drv_info[];
static void
_mtlk_print_drv_info (void) {
#if (IWLWAV_RTLOG_MAX_DLEVEL >= 1)
  int i = 0;
  ILOG1_V("*********************************************************");
  ILOG1_V("* Driver Compilation Details:");
  ILOG1_V("*********************************************************");
  while (mtlk_drv_info[i]) {
    ILOG1_S("* %s", mtlk_drv_info[i]);
    i++;
  }
  ILOG1_V("*********************************************************");
#endif
}

static int
_mtlk_vap_init (mtlk_vap_handle_t vap_handle,
    mtlk_vap_manager_t *obj,
    uint8 vap_index,
    const char *name,
    mtlk_work_role_e role,
    BOOL is_master,
    struct net_device *ndev)
{
  mtlk_vap_info_internal_t *vap_info;
  mtlk_card_type_t hw_type = MTLK_CARD_UNKNOWN;
  mtlk_card_type_info_t hw_type_info;
  int res, i;
  BOOL is_dut = (MTLK_MODE_DUT == mtlk_vap_manager_get_mode(obj));

  MTLK_UNREFERENCED_PARAM(is_dut);

  MTLK_ASSERT(vap_handle != NULL);
  vap_info = (mtlk_vap_info_internal_t *)vap_handle;

  _mtlk_print_drv_info();
  ILOG1_S("Working in %s mode", is_dut ? "DUT" : "normal");

  MTLK_INIT_TRY(vap_handler, MTLK_OBJ_PTR(vap_info))
    vap_info->manager = obj;
    vap_info->id = vap_index;
    vap_info->hw_idx = mtlk_hw_mmb_get_card_idx(obj->hw_api->hw);
    vap_info->radio = obj->radio;
    vap_info->radio_id = obj->radio_id;
    vap_info->radio_id_txmm = obj->radio_id;
    MTLK_ASSERT(NULL != vap_info->radio);
    vap_info->ep_bit = wave_hw_band_hd_ep_bit_get(obj->hw_api->hw, obj->radio_id,
                                                  vap_index, &vap_info->id_fw);

    vap_info->role = role;
    vap_info->is_master = is_master;
    if (is_master && (mtlk_vap_manager_get_master_vap_index(obj) != vap_index)){
      if (mtlk_vap_manager_get_master_vap_index(obj) != MTLK_VAP_INVALID_IDX ){
        ELOG_S("Error, trying to add Master VAP mode %s when another VAP index is already configured as master",
            mtlk_vap_is_ap(vap_handle) ? "AP" : "STA");
        return MTLK_ERR_ALREADY_EXISTS;
      }

      _mtlk_vap_manager_set_master_vap(obj, vap_index);
    }

    res = mtlk_hw_get_prop(mtlk_vap_get_hwapi(vap_handle), MTLK_HW_PROP_CARD_TYPE, &hw_type, sizeof(hw_type));
    if (res != MTLK_ERR_OK) {
      ELOG_D("CID-%04x: Can't determine HW type", mtlk_vap_get_oid(vap_handle));
      return res;
    }

    res = mtlk_hw_get_prop(mtlk_vap_get_hwapi(vap_handle), MTLK_HW_PROP_CARD_TYPE_INFO, &hw_type_info, sizeof(hw_type_info));
    if (res != MTLK_ERR_OK) {
      ELOG_D("CID-%04x: Can't determine HW type info", mtlk_vap_get_oid(vap_handle));
      return res;
    }

    MTLK_INIT_STEP_EX(vap_handler, VAP_HANDLER_PDB_CREATE, MTLK_OBJ_PTR(vap_info),
                      wave_pdb_create, (PARAM_DB_MODULE_ID_CORE, hw_type, hw_type_info),
                      vap_info->param_db,
                      vap_info->param_db != NULL,
                      MTLK_ERR_NO_MEM);
    MTLK_INIT_STEP_EX(vap_handler, VAP_HANDLER_DF_CREATE, MTLK_OBJ_PTR(vap_info),
                      mtlk_df_create, (vap_handle, name, ndev, FALSE),
                      vap_info->df,
                      vap_info->df != NULL,
                      MTLK_ERR_NO_MEM);
    MTLK_INIT_STEP_EX(vap_handler, VAP_HANDLER_SECNDRY_DF_CREATE, MTLK_OBJ_PTR(vap_info),
                      _mtlk_vap_secondary_df_array_create, (vap_handle),
                      vap_info->secondary_dfs,
                      vap_info->secondary_dfs != NULL,
                      MTLK_ERR_NO_MEM);
    MTLK_INIT_STEP_EX(vap_handler, VAP_HANDLER_ABMGR_CREATE, MTLK_OBJ_PTR(vap_info),
                      mtlk_abmgr_create, (),
                      vap_info->abmgr,
                      vap_info->abmgr != NULL,
                      MTLK_ERR_NO_MEM);
    MTLK_INIT_STEP_EX(vap_handler, VAP_HANDLER_CORE_API_ALLOC, MTLK_OBJ_PTR(vap_info),
                      mtlk_osal_mem_alloc, (sizeof(mtlk_core_api_t), MTLK_MEM_TAG_USER_VAPMGR),
                      vap_info->core_api,
                      vap_info->core_api != NULL,
                      MTLK_ERR_NO_MEM);
    MTLK_INIT_STEP(vap_handler, VAP_HANDLER_CORE_API_INIT, MTLK_OBJ_PTR(vap_info),
                   mtlk_core_api_init, (vap_handle, vap_info->core_api, vap_info->df));
#ifdef MTLK_WAVE_700
    MTLK_INIT_STEP(vap_handler, ML_VAP_INFO, MTLK_OBJ_PTR(vap_info),
                   mtlk_ml_vap_event_init, (vap_handle));
#endif

    /* validate core VFT
      NOTE: all functions should be initialized by core, no any NULL values
        accepted. In case of unsupported functionality the function with
        empty body required!
        Validation must be in sync with mtlk_core_vft_t declaration. */
    MTLK_ASSERT(NULL != vap_info->core_api->vft->start);
    MTLK_ASSERT(NULL != vap_info->core_api->vft->handle_tx_data);
    MTLK_ASSERT(NULL != vap_info->core_api->vft->release_tx_data);
    MTLK_ASSERT(NULL != vap_info->core_api->vft->handle_rx_data);
    MTLK_ASSERT(NULL != vap_info->core_api->vft->handle_rx_bss);
    MTLK_ASSERT(NULL != vap_info->core_api->vft->handle_rx_ctrl);
    MTLK_ASSERT(NULL != vap_info->core_api->vft->get_prop);
    MTLK_ASSERT(NULL != vap_info->core_api->vft->set_prop);
    MTLK_ASSERT(NULL != vap_info->core_api->vft->stop);
    MTLK_ASSERT(NULL != vap_info->core_api->vft->prepare_stop);

    MTLK_INIT_STEP_EX(vap_handler, VAP_HANDLER_IRBD_ALLOC, MTLK_OBJ_PTR(vap_info),
                      mtlk_irbd_alloc, (),
                      vap_info->irbd,
                      vap_info->irbd != NULL,
                      MTLK_ERR_NO_MEM);
    if (!mtlk_df_user_is_secondary(mtlk_df_get_user(vap_info->df))) {
      int size = 0;
      const proc_file_info_t *pinfo = wave_get_proc_table(&size);
      for (i = 0; i < size; i++) {
        MTLK_INIT_STEP_LOOP(vap_handler, VAP_HANDLER_PROC_REGISTER, MTLK_OBJ_PTR(vap_info),
                            _mtlk_vap_reg_proc_file, (vap_info, &pinfo[i]));
      }
    }
  MTLK_INIT_FINALLY(vap_handler, MTLK_OBJ_PTR(vap_info))
  MTLK_INIT_RETURN(vap_handler, MTLK_OBJ_PTR(vap_info), _mtlk_vap_cleanup, (vap_handle))
}

void __MTLK_IFUNC
mtlk_vap_delete (mtlk_vap_handle_t vap_handle)
{
  mtlk_vap_manager_t *obj = NULL;
  uint8              vap_index;

  MTLK_ASSERT(NULL != vap_handle);

  obj = mtlk_vap_get_manager(vap_handle);
  MTLK_ASSERT(NULL != obj);

  vap_index = mtlk_vap_get_id(vap_handle);
  MTLK_ASSERT(vap_index < obj->max_vaps_count);

  _mtlk_vap_cleanup(vap_handle);

  if (obj->guest_vap_array) {
    mtlk_osal_mem_free((struct _mtlk_vap_handle_t *)obj->guest_vap_array[vap_index]);
    obj->guest_vap_array[vap_index] = NULL;
    obj->secondary_id_vap_array[vap_index] = NULL;
  }
}

int __MTLK_IFUNC
mtlk_vap_start (mtlk_vap_handle_t vap_handle)
{
  mtlk_vap_info_internal_t *vap_info;
  mtlk_irbd_t              *hw_irbd;
  mtlk_hw_api_t            *hw_api;
  int                       res;

  MTLK_ASSERT(NULL != vap_handle);
  vap_info = (mtlk_vap_info_internal_t *)vap_handle;

  hw_api = mtlk_vap_get_hwapi(vap_handle);
  MTLK_ASSERT(NULL != hw_api);

  res = mtlk_hw_get_prop(hw_api, MTLK_HW_IRBD, &hw_irbd, sizeof(typeof(hw_irbd)));
  if (res != MTLK_ERR_OK) {
    return res;
  }

  MTLK_START_TRY(vap_handler, MTLK_OBJ_PTR(vap_info))
    MTLK_START_STEP(vap_handler, VAP_HANDLER_IRBD_INIT, MTLK_OBJ_PTR(vap_info),
                    mtlk_irbd_init, (vap_info->irbd,
                                     hw_irbd,
                                     mtlk_df_get_name(vap_info->df)));
    MTLK_START_STEP(vap_handler, VAP_HANDLER_CORE_START, MTLK_OBJ_PTR(vap_info),
                    mtlk_vap_get_core_vft(vap_handle)->start, (vap_handle));
    MTLK_START_STEP(vap_handler, VAP_HANDLER_DF_START, MTLK_OBJ_PTR(vap_info),
                    mtlk_df_start, (mtlk_vap_get_df(vap_handle)));
    MTLK_START_STEP_VOID(vap_handler, VAP_HANDLER_SECNDRY_DF_START, MTLK_OBJ_PTR(vap_info),
                    MTLK_NOACTION, ());
  MTLK_START_FINALLY(vap_handler, MTLK_OBJ_PTR(vap_info))
  MTLK_START_RETURN(vap_handler, MTLK_OBJ_PTR(vap_info), mtlk_vap_stop, (vap_handle))
}

void __MTLK_IFUNC
mtlk_vap_set_stopped (mtlk_vap_handle_t vap_handle)
{
  mtlk_vap_manager_t *obj = NULL;
  uint8              vap_index;

  MTLK_ASSERT(NULL != vap_handle);

  obj = mtlk_vap_get_manager(vap_handle);
  MTLK_ASSERT(NULL != obj);

  vap_index = mtlk_vap_get_id(vap_handle);
  MTLK_ASSERT(vap_index < obj->max_vaps_count);

  if (obj->e_vap_status) {
    ILOG1_D("VAP is stopped:%d", vap_index);
    obj->e_vap_status[vap_index] = VAP_STATUS_STOPPED;
  }
}

void __MTLK_IFUNC
mtlk_vap_set_ready (mtlk_vap_handle_t vap_handle)
{
  mtlk_vap_manager_t *obj = NULL;
  uint8              vap_index;

  MTLK_ASSERT(NULL != vap_handle);

  obj = mtlk_vap_get_manager(vap_handle);
  MTLK_ASSERT(NULL != obj);

  vap_index = mtlk_vap_get_id(vap_handle);
  MTLK_ASSERT(vap_index < obj->max_vaps_count);

  if (obj->e_vap_status) {
    ILOG1_D("VAP is ready:%d", vap_index);
    obj->e_vap_status[vap_index] = VAP_STATUS_READY;
  }
}

void __MTLK_IFUNC
mtlk_vap_stop (mtlk_vap_handle_t vap_handle)
{
  mtlk_vap_info_internal_t *vap_info;
  mtlk_vap_manager_t *obj = NULL;
  uint8              vap_index;

  MTLK_ASSERT(NULL != vap_handle);

  obj = mtlk_vap_get_manager(vap_handle);
  MTLK_ASSERT(NULL != obj);

  vap_index = mtlk_vap_get_id(vap_handle);
  MTLK_ASSERT(vap_index < obj->max_vaps_count);

  vap_info = (mtlk_vap_info_internal_t *)vap_handle;

  MTLK_STOP_BEGIN(vap_handler, MTLK_OBJ_PTR(vap_info))
    MTLK_STOP_STEP(vap_handler, VAP_HANDLER_SECNDRY_DF_START, MTLK_OBJ_PTR(vap_info),
                   mtlk_vap_del_all_secondary_vaps, (vap_handle));
    MTLK_STOP_STEP(vap_handler, VAP_HANDLER_DF_START, MTLK_OBJ_PTR(vap_info),
                   mtlk_df_stop, (mtlk_vap_get_df(vap_handle)));
    MTLK_STOP_STEP(vap_handler, VAP_HANDLER_CORE_START, MTLK_OBJ_PTR(vap_info),
                   mtlk_vap_get_core_vft (vap_handle)->stop, (vap_handle));
    MTLK_STOP_STEP(vap_handler, VAP_HANDLER_IRBD_INIT, MTLK_OBJ_PTR(vap_info),
                   mtlk_irbd_cleanup, (vap_info->irbd));
  MTLK_STOP_END(vap_handler, MTLK_OBJ_PTR(vap_info))
}

int __MTLK_IFUNC
mtlk_vap_create_secondary_vap (mtlk_vap_handle_t vap_handle,
                               uint8 sec_vap_index,
                               const char* sec_vap_name,
                               struct net_device *ndev)
{
  mtlk_vap_info_internal_t *vap_info;
  mtlk_vap_manager_t *obj;
  mtlk_df_t *secondary_df = NULL;
  mtlk_df_user_t *secondary_df_user = NULL;
  uint32 max_vaps_count;
  int res;

  MTLK_ASSERT(NULL != vap_handle);
  vap_info = (mtlk_vap_info_internal_t *)vap_handle;
  MTLK_ASSERT(NULL != vap_info->secondary_dfs);

  obj = mtlk_vap_get_manager(vap_handle);
  MTLK_ASSERT(NULL != obj);
  MTLK_ASSERT(obj->secondary_id_vap_array != NULL);

  max_vaps_count = obj->max_vaps_count;
  MTLK_ASSERT(sec_vap_index < max_vaps_count);

  if (obj->guest_vap_array[sec_vap_index]) {
    ELOG_D("VapID %u already exists", sec_vap_index);
    return MTLK_ERR_PARAMS;
  }

  if (obj->secondary_id_vap_array[sec_vap_index]) {
    ELOG_D("VapID %u is already reserved for secondary index", sec_vap_index);
    return MTLK_ERR_PARAMS;
  }

  MTLK_ASSERT(vap_info->secondary_dfs[sec_vap_index] == NULL);

  secondary_df = mtlk_df_create(vap_handle, sec_vap_name, ndev, TRUE);
  if (!secondary_df) {
    ELOG_S("%s: failed to create secondary df", sec_vap_name);
    return MTLK_ERR_UNKNOWN;
  }

  secondary_df_user = mtlk_df_get_user(secondary_df);
  mtlk_secondary_df_user_set_vap_idx(secondary_df_user, sec_vap_index);

  res = mtlk_df_start(secondary_df);
  if (res != MTLK_ERR_OK) {
    ELOG_S("%s: failed to start secondary df", sec_vap_name);
    mtlk_df_delete(secondary_df);
    return res;
  }

  vap_info->secondary_dfs[sec_vap_index] = secondary_df;
  obj->secondary_id_vap_array[sec_vap_index] = vap_handle;

  return res;
}

void __MTLK_IFUNC
mtlk_vap_del_secondary_vap (mtlk_vap_handle_t vap_handle, uint8 sec_vap_idx)
{
  mtlk_vap_info_internal_t *vap_info;
  mtlk_df_t *secondary_df;
  mtlk_vap_manager_t *obj;

  MTLK_ASSERT(NULL != vap_handle);
  vap_info = (mtlk_vap_info_internal_t *)vap_handle;
  MTLK_ASSERT(vap_info->secondary_dfs != NULL);

  obj = mtlk_vap_get_manager(vap_handle);
  MTLK_ASSERT(NULL != obj);
  MTLK_ASSERT(sec_vap_idx < obj->max_vaps_count);
  MTLK_ASSERT(obj->secondary_id_vap_array != NULL);

  secondary_df = vap_info->secondary_dfs[sec_vap_idx];
  if (secondary_df) {
    /* Bkh interface can be removed before backhaul STA deletion */
    sta_entry *sta = mtlk_df_user_get_backhaul_sta(mtlk_df_get_user(secondary_df));
    if (sta) {
      mtlk_vap_unbind_backhaul_sta(vap_handle, sta);
      mtlk_sta_decref(sta);
    }

    mtlk_df_stop(secondary_df);
    mtlk_df_delete(secondary_df);

    vap_info->secondary_dfs[sec_vap_idx] = NULL;
    obj->secondary_id_vap_array[sec_vap_idx] = obj->guest_vap_array[sec_vap_idx];
  }
}

void __MTLK_IFUNC
mtlk_vap_del_all_secondary_vaps (mtlk_vap_handle_t vap_handle)
{
  mtlk_vap_info_internal_t *vap_info;
  mtlk_df_t *secondary_df;
  mtlk_vap_manager_t *obj;
  uint32 vap_idx;

  MTLK_ASSERT(NULL != vap_handle);
  vap_info = (mtlk_vap_info_internal_t *)vap_handle;
  MTLK_ASSERT(vap_info->secondary_dfs != NULL);

  obj = mtlk_vap_get_manager(vap_handle);
  MTLK_ASSERT(NULL != obj);
  MTLK_ASSERT(obj->secondary_id_vap_array != NULL);

  for (vap_idx = 0; vap_idx < obj->max_vaps_count; vap_idx++) {

    if (vap_info->id == vap_idx)
      continue;

    secondary_df = vap_info->secondary_dfs[vap_idx];
    if (!secondary_df)
      continue;

    mtlk_df_stop(secondary_df);
    mtlk_df_delete(secondary_df);

    vap_info->secondary_dfs[vap_idx] = NULL;
    obj->secondary_id_vap_array[vap_idx] = NULL;
  }
}

int __MTLK_IFUNC
mtlk_vap_bind_backhaul_sta_to_secondary_vap (mtlk_vap_handle_t vap_handle, sta_entry *sta, uint8 sec_vap_idx)
{
  /* Find secondary vap by secondary vap index and bind bkh STA with this vap */

  mtlk_vap_info_internal_t *vap_info;
  mtlk_df_t *secondary_df;
  mtlk_vap_manager_t *obj;

  MTLK_ASSERT(NULL != vap_handle);
  vap_info = (mtlk_vap_info_internal_t *)vap_handle;
  MTLK_ASSERT(vap_info->secondary_dfs != NULL);
  MTLK_ASSERT(NULL != sta);

  obj = mtlk_vap_get_manager(vap_handle);
  MTLK_ASSERT(NULL != obj);
  MTLK_ASSERT(sec_vap_idx < obj->max_vaps_count);
  MTLK_ASSERT(obj->secondary_id_vap_array != NULL);

  secondary_df = vap_info->secondary_dfs[sec_vap_idx];
  if (secondary_df) {
    mtlk_df_user_t *secondary_df_user = mtlk_df_get_user(secondary_df);
    if (mtlk_df_user_backhaul_sta_is_set(secondary_df_user)) {
      ELOG_S("Backhaul STA for VLAN interface '%s' already set!", mtlk_df_get_name(secondary_df));
      return MTLK_ERR_NO_RESOURCES;
    }

    ILOG0_SY("VLAN interface '%s' exists, set backhaul STA %Y", mtlk_df_get_name(secondary_df), mtlk_sta_get_addr(sta)->au8Addr);
    sta->secondary_vap_id = sec_vap_idx;
    mtlk_df_user_set_backhaul_sta(secondary_df_user, sta);
    return MTLK_ERR_OK;
  }
  return MTLK_ERR_NO_RESOURCES;
}

int __MTLK_IFUNC
mtlk_vap_bind_backhaul_sta (mtlk_vap_handle_t vap_handle, sta_entry *sta)
{
  /* Find secondary vap by backhaul interface name and bind bkh STA with this vap.
     Backhaul interfaces uses VLAN interface notation: wlanX.Y.sta<AID>
  */

  mtlk_vap_info_internal_t *vap_info;
  mtlk_vap_manager_t *obj;
  uint8 _vap_index;

  MTLK_ASSERT(NULL != sta);
  MTLK_ASSERT(NULL != vap_handle);
  vap_info = (mtlk_vap_info_internal_t *)vap_handle;
  MTLK_ASSERT(NULL != vap_info->secondary_dfs);
  if (NULL == vap_info->secondary_dfs)
    return MTLK_ERR_UNKNOWN;

  obj = mtlk_vap_get_manager(vap_handle);
  MTLK_ASSERT(NULL != obj);

  /* Construct ifname for search in the format <wlanX.Y>.sta<AID> */
  mtlk_snprintf(sta->vlan_ifname, sizeof(sta->vlan_ifname), "%s.sta%u",
    mtlk_df_get_name(mtlk_vap_get_df(vap_handle)), mtlk_sta_get_aid(sta));
  ILOG0_S("looking for VLAN interface '%s' ...", sta->vlan_ifname);

  /* We connect only 4addr stations to backhaul vap ids, and never to the original netdev */
  for (_vap_index = 0; _vap_index < obj->max_vaps_count; _vap_index++) {
    mtlk_df_t *secondary_df;
    mtlk_df_user_t *secondary_df_user;

    if (_vap_index == vap_info->id)
      continue;

    secondary_df = vap_info->secondary_dfs[_vap_index];
    if ((NULL == secondary_df) || (0 != strcmp(mtlk_df_get_name(secondary_df), sta->vlan_ifname)))
      continue;

    secondary_df_user = mtlk_df_get_user(secondary_df);
    if (mtlk_df_user_backhaul_sta_is_set(secondary_df_user)) {
      ELOG_S("Backhaul STA for VLAN interface '%s' already set!", sta->vlan_ifname);
      return MTLK_ERR_NO_RESOURCES;
    }

    ILOG0_SY("VLAN interface '%s' found, set backhaul STA %Y", sta->vlan_ifname, mtlk_sta_get_addr(sta)->au8Addr);
    sta->secondary_vap_id = _vap_index;
    mtlk_df_user_set_backhaul_sta(secondary_df_user, sta);

    return MTLK_ERR_OK;
  }

  ILOG0_S("vlan_ifname:%s wasn't found, pending vlan/bkh interface creation...", sta->vlan_ifname);
  /* The secondary df was not found, but that's not a problem,
     the backhaul interface will be created later by userspace code (hostapd),
     so just say we're waiting for a backhaul interface */
  return MTLK_ERR_PENDING;
}

void __MTLK_IFUNC
mtlk_vap_unbind_backhaul_sta (mtlk_vap_handle_t vap_handle, sta_entry *sta)
{
  mtlk_vap_info_internal_t *vap_info;
  mtlk_df_user_t *secondary_df_user;

  MTLK_ASSERT(NULL != sta);
  MTLK_ASSERT(NULL != vap_handle);
  vap_info = (mtlk_vap_info_internal_t *)vap_handle;

  MTLK_ASSERT(NULL != vap_info->secondary_dfs);
  MTLK_ASSERT(NULL != vap_info->secondary_dfs[sta->secondary_vap_id]);
  if (NULL == vap_info->secondary_dfs[sta->secondary_vap_id])
    return;

  ILOG0_Y("unbind backhaul STA %Y", mtlk_sta_get_addr(sta)->au8Addr);
  secondary_df_user = mtlk_df_get_user(vap_info->secondary_dfs[sta->secondary_vap_id]);
  if (!mtlk_df_user_is_equal_backhaul_sta(secondary_df_user, sta)) {
    MTLK_ASSERT(sta->secondary_vap_id == vap_info->id);
    sta->secondary_vap_id = vap_info->id;
    return;
  }

  sta->secondary_vap_id = vap_info->id;
  mtlk_df_user_set_backhaul_sta(secondary_df_user, NULL);
}

mtlk_error_t __MTLK_IFUNC
mtlk_vap_manager_find_primary_vap (mtlk_vap_manager_t *vap_manager, const char* sec_vap_name, mtlk_vap_handle_t *vap_handle)
{
  /* "primary vap" in term of "secondary vap": the vap which "owns" secondary (backhaul) vaps
     Example: secondary vap: wlan0.0.sta123, primary vap: wlan0.0
  */
  char primary_vap_name[IFNAMSIZ + 1];
  const char* sta_suffix;
  unsigned vap_id;

  MTLK_ASSERT(NULL != vap_manager);
  MTLK_ASSERT(NULL != sec_vap_name);
  MTLK_ASSERT(NULL != vap_handle);

  sta_suffix = strstr(sec_vap_name, ".sta");
  if (!sta_suffix) {
    wave_strcopy(primary_vap_name, sec_vap_name, sizeof(primary_vap_name));
  } else {
    wave_strncopy(primary_vap_name, sec_vap_name, sizeof(primary_vap_name), sta_suffix-sec_vap_name);
  }

  ILOG2_SS("Search for primary_vap_name '%s' for sec_vap_name '%s'", primary_vap_name, sec_vap_name);
  for (vap_id = 0; vap_id < vap_manager->max_vaps_count; ++vap_id) {
    mtlk_vap_handle_t handle = __mtlk_vap_manager_vap_handle_by_id(vap_manager, vap_id);
    if (handle) {
      if (0 == strcmp(mtlk_df_get_name(mtlk_vap_get_df(handle)), primary_vap_name)) {
        ILOG2_S("Primary VAP found: %s", mtlk_df_get_name(mtlk_vap_get_df(handle)));
        *vap_handle = handle;
        return MTLK_ERR_OK;
      }
    }
  }

  *vap_handle = NULL;
  return MTLK_ERR_NO_ENTRY;
}

BOOL __MTLK_IFUNC
mtlk_vap_is_dut (mtlk_vap_handle_t vap_handle)
{
  BOOL is_dut = FALSE;
  mtlk_vap_handle_t master_vap_handle = NULL;
  mtlk_vap_manager_t *obj = mtlk_vap_get_manager(vap_handle);
  mtlk_work_mode_e work_mode = mtlk_vap_manager_get_mode(obj);

  if (MTLK_MODE_DUT == work_mode) {
    return TRUE;
  }

  if (mtlk_vap_manager_get_master_vap(obj, &master_vap_handle) == MTLK_ERR_OK) {
    if (mtlk_vap_get_core_vft(master_vap_handle)->get_prop(master_vap_handle,
                              MTLK_CORE_PROP_IS_DUT, &is_dut, sizeof(is_dut)) != MTLK_ERR_OK) {
      WLOG_V("Cannot get DUT mode status");
    }
  }

  return is_dut;
}

mtlk_handle_t __MTLK_IFUNC
mtlk_vap_get_ta (mtlk_vap_handle_t vap_handle)
{
  MTLK_ASSERT(vap_handle != NULL);
  return mtlk_vap_get_manager(vap_handle)->ta;
}
#ifdef UNUSED_CODE
BOOL __MTLK_IFUNC mtlk_vap_manager_has_connections(mtlk_vap_manager_t  *vap_mng)
{
  mtlk_core_t         *cur_core;
  mtlk_vap_handle_t   vap_handle;
  unsigned            i, max_vaps;

  max_vaps = mtlk_vap_manager_get_max_vaps_count(vap_mng);

  for (i = 0; i < max_vaps; i++) {
    if (MTLK_ERR_OK !=
          mtlk_vap_manager_get_vap_handle(vap_mng, i, &vap_handle)) {
      continue;   /* VAP does not exist */
    }
    cur_core = mtlk_vap_get_core(vap_handle);
    if (NET_STATE_CONNECTED != mtlk_core_get_net_state(cur_core)) {
      /* Core is not ready */
      continue;
    }
    if(mtlk_core_has_connections(cur_core)) {
      return TRUE;
    }
  }

  return FALSE;
}
#endif /* UNUSED_CODE */
static void mtlk_vap_manager_has_peer_connections_hlpr(void *data, struct ieee80211_sta *sta){
  BOOL * res = (BOOL *)data;
  *res = TRUE;
}

BOOL __MTLK_IFUNC mtlk_vap_manager_has_peer_connections(mtlk_vap_manager_t  *vap_mng)
{
  BOOL res = FALSE;

  ieee80211_iterate_stations_atomic(wave_radio_ieee80211_hw_get((wave_radio_t *)vap_mng->radio),
                          &mtlk_vap_manager_has_peer_connections_hlpr, &res);
  return res;
}

int __MTLK_IFUNC
mtlk_vap_manager_rcvry_start_all_subifs(mtlk_vap_manager_t  *obj)
{
  int res = MTLK_ERR_OK;
  unsigned vap_index, max_vaps_count;
  mtlk_vap_handle_t vap_handle;
  MTLK_ASSERT(obj != NULL);

  /* Register only in case of FastPath */
  max_vaps_count = mtlk_vap_manager_get_max_vaps_count(obj);
  for (vap_index = 0; vap_index < max_vaps_count; vap_index++) {
    vap_handle = __mtlk_vap_manager_vap_handle_by_secondary_id(obj, vap_index);
    if (NULL != vap_handle) {
      res = mtlk_df_on_rcvry_subxface_start(mtlk_vap_get_secondary_df(vap_handle, vap_index));
      if (MTLK_ERR_OK != res)
        break;
    }
  }

  return res;
}

void __MTLK_IFUNC
mtlk_vap_manager_rcvry_stop_all_subifs(mtlk_vap_manager_t  *obj)
{
  unsigned vap_index, max_vaps_count;
  mtlk_vap_handle_t vap_handle;
  MTLK_ASSERT(obj != NULL);

  /* Unregister only in case of FastPath */
  max_vaps_count = mtlk_vap_manager_get_max_vaps_count(obj);
  for (vap_index = 0; vap_index < max_vaps_count; vap_index++) {
    vap_handle = __mtlk_vap_manager_vap_handle_by_secondary_id(obj, vap_index);
    if (NULL != vap_handle) {
      mtlk_df_on_rcvry_subxface_stop(mtlk_vap_get_secondary_df(vap_handle, vap_index));
    }
  }
}

bool __MTLK_IFUNC
mtlk_vap_manager_is_sta_connected (mtlk_core_t *core, mtlk_core_t **sta_core, const unsigned char *mac)
{
  mtlk_core_t        *cur_core;
  sta_entry          *sta = NULL;
  mtlk_vap_manager_t *vap_mng;
  mtlk_vap_handle_t   vap_handle;
  unsigned            i, max_vaps;

  vap_mng  = mtlk_vap_get_manager(core->vap_handle);
  max_vaps = wave_radio_max_vaps_get(wave_vap_radio_get(core->vap_handle));

  for (i = 0; i < max_vaps; i++) {
    if (MTLK_ERR_OK != mtlk_vap_manager_get_vap_handle(vap_mng, i, &vap_handle)) {
      continue;   /* VAP does not exist */
    }
    cur_core = mtlk_vap_get_core(vap_handle);

    if (NET_STATE_CONNECTED != mtlk_core_get_net_state(cur_core)) {
      continue; /* Core is not ready */
    }

    sta = mtlk_stadb_find_sta(mtlk_core_get_stadb(cur_core), mac);
    if (sta) {
      mtlk_sta_decref(sta);

      if (sta_core)
        *sta_core = cur_core;

      return TRUE;
    }
  }

  return FALSE;
}

void __MTLK_IFUNC
mtlk_vap_manager_inc_wds_bridgemode (mtlk_vap_manager_t *vap_manager)
{
  MTLK_ASSERT(NULL != vap_manager);

  mtlk_osal_atomic_inc(&vap_manager->wds_bridgemode);
}

void __MTLK_IFUNC
mtlk_vap_manager_dec_wds_bridgemode (mtlk_vap_manager_t *vap_manager)
{
  MTLK_ASSERT(NULL != vap_manager);

  mtlk_osal_atomic_dec(&vap_manager->wds_bridgemode);
}

uint32 __MTLK_IFUNC
mtlk_vap_manager_get_wds_bridgemode (mtlk_vap_manager_t *vap_manager)
{
  MTLK_ASSERT(NULL != vap_manager);

  return mtlk_osal_atomic_get(&vap_manager->wds_bridgemode);
}

uint32 __MTLK_IFUNC mtlk_vap_manager_get_wlan_index (mtlk_vap_manager_t *vap_manager)
{
  mtlk_df_t *master_df = mtlk_vap_manager_get_master_df(vap_manager);
  char ifname[IFNAMSIZ] = {0};

  MTLK_ASSERT(master_df);

  if (!master_df) {
    ELOG_V("Failed to get master DF");
    return IWLWAV_RTLOG_WLAN_IF_INVALID;
  }

  wave_strcopy(ifname, mtlk_df_get_name(master_df), IFNAMSIZ);
  return mtlk_rtlog_get_wlanif_from_string(ifname);
}

void __MTLK_IFUNC
wave_vap_radio_id_txmm_set(mtlk_vap_handle_t vap_handle, unsigned new_radio_id_txmm)
{
  mtlk_vap_info_internal_t *_info = (mtlk_vap_info_internal_t *)vap_handle;

  MTLK_ASSERT(NULL != _info);
  MTLK_ASSERT(NULL != _info->manager);
  MTLK_ASSERT(NULL != _info->radio);

  ILOG1_DD("radio_id_txmm %d changed to: %d", _info->radio_id_txmm, new_radio_id_txmm);

  _info->radio_id_txmm = new_radio_id_txmm;
}
