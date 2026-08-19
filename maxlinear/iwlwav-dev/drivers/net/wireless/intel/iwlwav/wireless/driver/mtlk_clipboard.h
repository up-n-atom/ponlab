/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation
         Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
         Copyright 2009 - 2014 Lantiq Deutschland GmbH
         Copyright 2007 - 2008 Infineon Technologies AG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/*
 * $Id$
 *
 * 
 *
 * Clipboard between Core and DF
 *
 * Originally written by Andrii Tseglytskyi
 *
 */

#ifndef _MTLK_CLIPBOARD_H_
#define _MTLK_CLIPBOARD_H_

#define LOG_LOCAL_GID   GID_CLIPBOARD
#define LOG_LOCAL_FID   2

/* "clipboard" between DF UI and Core */
typedef struct _mtlk_clpb_t mtlk_clpb_t;

#ifdef CPTCFG_IWLWAV_ENABLE_CLPB_TRACKING

void __MTLK_IFUNC mtlk_clpb_tracking_init (void);
void __MTLK_IFUNC mtlk_clpb_tracking_cleanup (void);

mtlk_clpb_t* __MTLK_IFUNC
mtlk_clpb_create_ex (mtlk_slid_t creator_slid,
                     unsigned    invoke_req_id);

#define mtlk_clpb_create()          mtlk_clpb_create_ex(MTLK_SLID, 0)
#define mtlk_clpb_create_req(__id)  mtlk_clpb_create_ex(MTLK_SLID, __id)

#define mtlk_clpb_push(clpb, data, size) \
  mtlk_clpb_push_ex(MTLK_SLID, clpb, data, NULL, size)

#define mtlk_clpb_push_nocopy(clpb, data_preallocated, size) \
  mtlk_clpb_push_ex(MTLK_SLID, clpb, NULL, data_preallocated, size)

#else /* CPTCFG_IWLWAV_ENABLE_CLPB_TRACKING */

mtlk_clpb_t* __MTLK_IFUNC
mtlk_clpb_create_ex (void);

#define mtlk_clpb_create()          mtlk_clpb_create_ex()
#define mtlk_clpb_create_req(__id)  mtlk_clpb_create_ex()

#define mtlk_clpb_push(clpb, data, size) \
  mtlk_clpb_push_ex(clpb, data, NULL, size)

#define mtlk_clpb_push_nocopy(clpb, data_preallocated, size) \
  mtlk_clpb_push_ex(clpb, NULL, data_preallocated, size)

#endif /* CPTCFG_IWLWAV_ENABLE_CLPB_TRACKING */

int __MTLK_IFUNC
mtlk_clpb_push_ex (
#ifdef CPTCFG_IWLWAV_ENABLE_CLPB_TRACKING
                   mtlk_slid_t pusher_slid,
#endif
                   mtlk_clpb_t *clpb,
                   const void *data_src,
                   void *data_preallocated,
                   uint32 size);

void __MTLK_IFUNC
mtlk_clpb_delete(mtlk_clpb_t *clpb);

void __MTLK_IFUNC
mtlk_clpb_purge(mtlk_clpb_t *clpb);



static __INLINE int
mtlk_clpb_push_res (mtlk_clpb_t *clpb, int res)
{
  if (MTLK_ERR_CLPB_DATA == res)
    return MTLK_ERR_UNKNOWN;
  else
    return mtlk_clpb_push(clpb, &res, sizeof(res));
}

static __INLINE int
mtlk_clpb_push_res_data (mtlk_clpb_t *clpb, int res, void *data, uint32 size)
{
  /* Push result code */
  int err_code = mtlk_clpb_push_res(clpb, res);
  if (MTLK_ERR_OK == err_code) {
    /* Push data */
    if (MTLK_ERR_OK == res)
      err_code = mtlk_clpb_push(clpb, data, size);
  }
  return err_code;
}

static __INLINE int
mtlk_clpb_push_res_data_no_copy (mtlk_clpb_t *clpb, int res, void *data, uint32 size)
{
  /* Push result code */
  int err_code = mtlk_clpb_push_res(clpb, res);
  if (MTLK_ERR_OK == err_code) {
    /* Push data */
    if (MTLK_ERR_OK == res)
      err_code = mtlk_clpb_push_nocopy(clpb, data, size);
    else
      err_code = res;
  }
  return err_code;
}

/***************************************
 * helper macros for result check. Usage:

 int func(...) {
  mtlk_clpb_t *clpb = <get clipboard>;
  unsigned params_size;
  <data_type> *clpb_data;
  int res;

  clpb_data = mtlk_clpb_enum_get_next(clpb, &params_size);
  MTLK_CLPB_TRY(clpb_data, params_size)
    do_something();
    if (something_happened()) {
      MTLK_CLPB_EXIT(MTLK_ERR_UNKNOWN);
    }
    res = do_another();

  MTLK_CLPB_FINALLY(res)
    return mtlk_clpb_push_res(clpb, res);

  MTLK_CLPB_END;
 }
*/

/* Start of protected section */
#define MTLK_CLPB_CHECK_DATA(clpb_data, data_size) \
  (mtlk_clpb_check_data(clpb_data, data_size, sizeof(*(clpb_data)), __FUNCTION__, __LINE__ ))

#define MTLK_CLPB_TRY_EX(clpb_data, clpb_data_size, expected_size) {  \
  int __res;                        \
  MTLK_ASSERT(NULL != (clpb_data)); \
  if (!mtlk_clpb_check_data(clpb_data, clpb_data_size, expected_size, __FUNCTION__, __LINE__ )) {\
    __res = MTLK_ERR_CLPB_DATA; \
    goto __mtlk_clbp_finally;  \
  }{ \
    __res = MTLK_ERR_OK; /* start of protected section */

#define MTLK_CLPB_TRY(clpb_data, data_size) \
    MTLK_CLPB_TRY_EX(clpb_data, data_size, sizeof(typeof(*(clpb_data))))

#define MTLK_CLPB_TRY_NODATA() { \
  int __res = MTLK_ERR_OK; \
  { \
  /* start of protected section */

#define MTLK_CLPB_EXIT(res) \
  do { __res = (res); goto __mtlk_clbp_finally;} while(0)

/* Finalization */
#define MTLK_CLPB_FINALLY(res) } \
  __res = (res);     \
__mtlk_clbp_finally: \
   res = __res;

/* End of protected section*/
#define MTLK_CLPB_END }

void __MTLK_IFUNC
mtlk_clpb_enum_rewind(mtlk_clpb_t *clpb);

void* __MTLK_IFUNC
mtlk_clpb_enum_get_next(mtlk_clpb_t *clpb, uint32* size);

uint32 __MTLK_IFUNC
mtlk_clpb_get_num_of_elements(mtlk_clpb_t *clpb);

BOOL __MTLK_IFUNC
mtlk_clpb_check_data (void *clpb_data, size_t data_size, size_t expected_size, const char* fn_name, int n_line);

#undef LOG_LOCAL_GID
#undef LOG_LOCAL_FID

#endif /* _MTLK_CLIPBOARD_H_ */
