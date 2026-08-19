/******************************************************************************
  Copyright (C) 2018 Intel Corporation
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : fapi_sec_service.h                                         *
 *     Project    : UGW                                                        *
 *     Description: secure fapi header file inclusions.			       *
 *                                                                             *
 ******************************************************************************/

/*! \file fapi_sec_service.h
 \brief File contains secure service APIs prototype
*/


#ifndef _SECURE_SER_FAPI__
#define _SECURE_SER_FAPI__


#include "libsafec/safe_str_lib.h"
#include "libsafec/safe_lib.h"
#include "libsafec/safe_mem_lib.h"

#include <secure_services.h>
#include "sec_upgrade.h"
#include <sys/mman.h>

#define MAX_SS_BUF_SIZE  16
#define MAX_SS_FILE_PATH_SIZE  64
#define MAX_WRITE 2
#define TEMP_BUF                4096
#define UPG_STATE_FILE VENDOR_PATH "/etc/upgrade/.upg_state_file"
#define FLASH_CNT_CHK VENDOR_PATH "/etc/upgrade/.flash_cnt_chk"

#define YR_MAGIC 1900
#define MM_MAGIC 1

#define F_MAX_FLASH_CNT "max_flash_cnt="
#define F_FLASH_CNT "cur_flash_cnt="
#define F_LAST_UPDATE_TIME_STAMP "l_updated_ts="
#define F_DAY_CNT	"day_flash_cnt="

#define U_RDWR_G_RD S_IRUSR|S_IWUSR|S_IRGRP  /* recommeded user permission for file */

/*calculate from flash driver instead hardcode */
#define MAX_FLASH_CNT_SUPPORT MAX_FLASH_CNT
#define MAX_DAY_FLASH_CNT 5

typedef enum State_t {
        UPG_NONE, 
        UPG_REQ, 
	UPG_INPROG, 
	UPG_SUCC,
	UPG_FAIL,
	UPG_RES, /* reserved for future use */
	MAX_STATE_CNT,
} State_t;

typedef struct sse_param {
	unsigned char *pcBuf;    /* buffer to store/retrive decrypted/encrypted content */
	size_t len;
	char sPath[MAX_SS_FILE_PATH_SIZE];	/* path to store encrypted content */
}sse_param_t;



/** \addtogroup SECUREFAPI */
/* @{ */

/*!
        \brief API to allocate memory for the buffer.
        \param[in] pxSSEParamList structure to secure storage parameters
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int sse_buf_alloc(sse_param_t *pxSSEParamList);

/*!
        \brief API to free memory for the buffer.
        \param[in] pxSSEParamList structure to secure storage parameters
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
void sse_buf_free(sse_param_t *pxSSEParamList);

/*! 
        \brief API to decrypt file path
        \param[in] pxSSEParamList->pcBuf will have decrypted content
        \param[in] pxSSEParamList->len decrypted buffer length
        \param[in] pxSSEParamList->pcPath secure storage file path
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int fapi_ssFRead(sse_param_t *pxSSEParamList);

/*! 
        \brief API to encrypt file 
        \param[in] pxSSEParamList->pcBuf content to be encrypted
        \param[in] pxSSEParamList->len encrypted buffer length
        \param[in] pxSSEParamList->pcPath encrypted file path
        \param[in] pcPath file path
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int fapi_ssFWrite(sse_param_t *pxSSEParamList);

/*! 
        \brief API to authenticate image
        \param[in] image_auth image_auth_t structure
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int fapi_ssImgAuth(img_param_t image_auth);

/*! 
        \brief API to get upgrade state 
        \param[in] pcState
        \return  current state on successful / UGW_FAILURE on failure
*/
int fapi_ssGetUpgState(unsigned char **pcState);

/*! 
        \brief API to set upgrade state
        \param[in] pcState 
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int fapi_ssSetUpgState(unsigned char *pcState);

/*! 
        \brief API to open file
        \param[in] pcFileName file name to open 
        \param[in] flags file open in R/W/RW 
        \param[in] mode file permissions
e       \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int fapi_Fileopen(const char *pcFileName, int flags, mode_t mode);

#endif 

/* @} */
