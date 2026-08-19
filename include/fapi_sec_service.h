/******************************************************************************

  Copyright (C) 2020-2022 MaxLinear, Inc.
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

#ifndef _SEC_SERVICE_FAPI__
#define _SEC_SERVICE_FAPI__

#include <stdio.h>
#include <sys/stat.h>

#define MAX_SS_FILE_PATH_SIZE  64
#ifndef LGM_SECURE
/* original definition available in sse_driver_lgm */
#define MAX_SIZE  (4000 * 1024)
#endif

/* recommeded user permission for file */
#define U_RDWR_G_RD (S_IRUSR|S_IWUSR|S_IRGRP)

typedef struct sse_param {
    unsigned char *pcBuf;       /* buffer to store/retrive decrypted/encrypted
                                   content */
    size_t len;
    char sPath[MAX_SS_FILE_PATH_SIZE];  /* path to store encrypted content */
} sse_param_t;

/** \addtogroup SECUREFAPI */
/* @{ */

/*!
        \brief API to allocate memory for the buffer.
        \param[in] pxSSEParamList structure to secure storage parameters
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int sse_buf_alloc(sse_param_t * pxSSEParamList);

/*!
        \brief API to free memory for the buffer.
        \param[in] pxSSEParamList structure to secure storage parameters
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
void sse_buf_free(sse_param_t * pxSSEParamList);

/*! 
        \brief API to open file
        \param[in] pcFileName file name to open 
        \param[in] flags file open in R/W/RW 
        \param[in] mode file permissions
e       \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int fapi_Fileopen(const char *pcFileName, int flags, mode_t mode);

/*! 
        \brief API to decrypt file path
        \param[in] pxSSEParamList->pcBuf will have decrypted content
        \param[in] pxSSEParamList->len decrypted buffer length
        \param[in] pxSSEParamList->pcPath secure storage file path
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int fapi_ssFRead(sse_param_t * pxSSEParamList);

/*! 
        \brief API to encrypt file 
        \param[in] pxSSEParamList->pcBuf content to be encrypted
        \param[in] pxSSEParamList->len encrypted buffer length
        \param[in] pxSSEParamList->pcPath encrypted file path
        \param[in] pcPath file path
        \return  UGW_SUCCESS on successful / UGW_FAILURE on failure
*/
int fapi_ssFWrite(sse_param_t * pxSSEParamList);

/*! 
        \brief API to dump hex infomration
        \param[in] pcBuf content to be displayed
        \param[in] unLen length of the content
        \return void
*/
void fapi_hexDump(unsigned char *pcBuf, unsigned int unLen);
#endif

/* @} */
