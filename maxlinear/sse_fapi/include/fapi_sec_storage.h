/******************************************************************************

  Copyright (C) 2022 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : fapi_sec_storage.h                                         *
 *     Project    : UGW                                                        *
 *     Description: secure storage fapi header file inclusions.			       *
 *                                                                             *
 ******************************************************************************/

/*! \file fapi_sec_storage.h
 \brief File contains secure storage APIs prototype
*/

#ifndef _SEC_STORAGE_FAPI__
#define _SEC_STORAGE_FAPI__

#include <secure_services.h>

#define MIN_OTP_ASSET_ID     11
#define MAX_OTP_ASSET_ID     30

#define OTP_CUST_ASSET_12     12

/* SST reply */
typedef enum sst_error_code {
    SST_SUCCESS = 0,				/* Successful */
	SST_INVALID_OBJ_ERROR,			/* SST object is not valid */
	SST_OBJ_ACCESS_PERMS_ERROR,		/* SST object access permission is rejected */
	SST_OBJ_RESOURCE_ALLOC_ERROR,	/* SST memory allocation error */
	SST_INVALID_SSHANDLE_ERROR,		/* SST handle is not valid */
	SST_INVALID_DEVNODE_ERROR,		/* SST device node /dev/sec_store_{admin/normal} open error */
	SST_INVALID_DATABUF_ERROR,		/* SST data buffer is not valid */
	SST_INVALID_CRYPTO_MODE_ERROR,	/* SST crypto mode flag is not vaid */
    SST_OBJ_ALREADY_EXIST_ERROR,    /* SST object already exist */
    SST_OBJ_CREATE_ERROR,			/* SST object create error */
    SST_OBJ_OPEN_ERROR,				/* SST object open error */
    SST_OBJ_SAVE_ERROR,				/* SST object save error */
    SST_OBJ_LOAD_ERROR,				/* SST object load error */
    SST_OBJ_DELETE_ERROR,			/* SST object delete error */
    SST_OBJ_CLOSE_ERROR,			/* SST object close error */
    SST_INVALID_ASSET_ID_ERROR,		/* SST object close error */
} sst_error_code_t;

/*!
	\brief Create and open a secure store object.
	\param[in] pcObjectName Secure store object name as a string
	\param[in] pcxObjPolicy Pointer to secure store object configuration
	\param[in] unFlag Flags for object access mode
	\param[out] pxSsHandle Secure store handle to access the object
	\return 0 on success and negative value on failure
*/
int securestore_create_open(const char *pszObjectName,
							const sst_obj_config_t *pcxObjPolicy, const unsigned int
							unFlag, sshandle_t *pxSsHandle);
/*!
	\brief Stores the plain data in to secure store object in cryptographically protected form.
	\param[in] xSsHandle Secure store handle
	\param[in] pxWrapConfig Pointer to wrap asset
	\param[in] pucDataBuf Pointer to the data buffer to be protected
	\param[in] unDataLen Length of data to be protected
	/return 0 on success and negative value on failure
*/
int securestore_save(const sshandle_t xSsHandle,
					const secure_wrap_asset_t *pxWrapConfig,
					const unsigned char *pucDataBuf,
					const unsigned int unDatalen);
/*!
	\brief Retrieves the plain data object from secure store.
	\param[in] xSsHandle Secure store handle
	\param[in] pxWrapConfig Pointer to wrap asset
	\param[out] pucDataBuf Pointer to hold the plain data buffer
	\param[in] unDataLen Length of data to be retrieved
	\param[out] punActDataLen Actual length of plain data
	\return 0 on success and negative value on failure
*/
int securestore_retrieve(const sshandle_t xSsHandle,
						const secure_wrap_asset_t *pxWrapConfig, unsigned char *pucDataBuf,
						const unsigned int unDataLen, unsigned int *punActDataLen);
/*!
	\brief Deletes the data object from secure store.
	\param[in] xSsHandle Secure store handle
	\return 0 on success and negative value on failure
*/
int securestore_delete(const sshandle_t xSsHandle);
/*!
	\brief Closes the data object from secure store.
	\param[in] xSsHandle Secure store handle
	\return 0 on success and negative value on failure
*/
int securestore_close(const sshandle_t xSsHandle);

#endif

/* @} */
