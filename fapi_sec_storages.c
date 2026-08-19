/******************************************************************************

  Copyright (C) 2022-2024 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : fapi_sec_storages.c					       *
 *     Project    : UGW                                                        *
 *     Description: secure service fapi, it provides set of API's to encrypt   *
 *		    /decrypt.						       *
 *                                                                             *
 ******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <limits.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "fapi_sec_service.h"
#include "fapi_sec_storage.h"
#include "help_error.h"
#include "help_logging.h"

#include "safe_str_lib.h"
#include "safe_lib.h"
#include "safe_mem_lib.h"

typedef struct sst_fd_handle {
	sshandle_t ss_handle;
	unsigned int fd;
	char objname[FILEPNAME_MAX];
	struct sst_fd_handle *next_sst_node;
} sst_fd_handle_t;

static sst_fd_handle_t *sst_fd = NULL;
static int stnOpenfdCount;
static pthread_mutex_t sst_lock = PTHREAD_MUTEX_INITIALIZER;

static inline int get_sst_fapi_error_code(int error)
{
	int retval = 0;

	switch (error) {
		case SST_OBJ_ID_GENERATION_ERR:
		case SST_OBJ_NOT_FOUND_ERR:
			retval = -SST_INVALID_OBJ_ERROR;
			break;
		case SST_OBJ_POLICY_NOT_FOUND_ERR:
		case SST_POLICY_NOT_MATCHED_ERR:
		case SST_OBJ_ATR_NOT_MATCHED_ERR:
			retval = -SST_OBJ_ACCESS_PERMS_ERROR;
			break;
		case SST_SSHANDLE_NOT_MATCHED_ERR:
			retval = -SST_INVALID_SSHANDLE_ERROR;
			break;
		case SST_OBJ_NODE_MEM_ALLOC_ERR:
		case SST_ICC_POOL_ALLOC_ERR:
			retval = -SST_OBJ_RESOURCE_ALLOC_ERROR;
			break;
		case SST_OBJ_CREATE_REQ_ERR:
			retval = -SST_OBJ_CREATE_ERROR;
			break;
		case SST_OBJ_OPEN_REQ_ERR:
			retval = -SST_OBJ_OPEN_ERROR;
			break;
		case SST_OBJ_SAVE_REQ_ERR:
			retval = -SST_OBJ_SAVE_ERROR;
			break;
		case SST_OBJ_LOAD_REQ_ERR:
			retval = -SST_OBJ_LOAD_ERROR;
			break;
		case SST_OBJ_DELETE_REQ_ERR:
			retval = -SST_OBJ_DELETE_ERROR;
			break;
        case SST_OBJ_ALREADY_EXIST_ERR:
            retval = -SST_OBJ_ALREADY_EXIST_ERROR;
            break;
		default:
			retval = -error;
			break;
	}

	return retval;
}

static sst_fd_handle_t *find_sstObject_by_name(const char *pszObjectName)
{
    int nDiff = -1;
	sst_fd_handle_t *sstFd = NULL;

	pthread_mutex_lock(&sst_lock);
	sstFd = sst_fd;
	if (sstFd == NULL) {
		pthread_mutex_unlock(&sst_lock);
		LOGF_LOG_ERROR("There is no SST FD is opened\n");
		return NULL;
	}

	for (int count = 0; count < stnOpenfdCount; count++) {
		memcmp_s(pszObjectName, strlen(pszObjectName), sstFd->objname,
			sizeof(sstFd->objname), &nDiff);
		if (!nDiff) {
			pthread_mutex_unlock(&sst_lock);
			return sstFd;
		}

		sstFd = sstFd->next_sst_node;
	}

	pthread_mutex_unlock(&sst_lock);
	return NULL;
}

static sst_fd_handle_t *find_sstObject_by_ssHandle(const sshandle_t xSsHandle)
{
    int nDiff = -1;
	sst_fd_handle_t *sstFd = NULL;

	pthread_mutex_lock(&sst_lock);
	sstFd = sst_fd;

	if (sstFd == NULL) {
		pthread_mutex_unlock(&sst_lock);
		LOGF_LOG_ERROR("There is no SST FD is opened\n");
		return NULL;
	}

	for (int count = 0; count < stnOpenfdCount; count++) {
		LOGF_LOG_DEBUG("xSsHandle[%lx], sst_fd sshandle[%lx]\n", xSsHandle,
				sstFd->ss_handle);
		memcmp_s(&xSsHandle, sizeof(sshandle_t), &(sstFd->ss_handle),
				sizeof(sshandle_t), &nDiff);
		if (!nDiff) {
			pthread_mutex_unlock(&sst_lock);
			return sstFd;
		}

		sstFd = sstFd->next_sst_node;
	}

	pthread_mutex_unlock(&sst_lock);
	return NULL;
}

static int close_sst_fd(const sshandle_t xSsHandle)
{
    int nDiff = -1;
	sst_fd_handle_t *sstFd = sst_fd;
	sst_fd_handle_t *prev_sstFd = NULL;

	for (int count = 0; count < stnOpenfdCount; count++) {
		LOGF_LOG_DEBUG("xSsHandle[%lx], sst_fd sshandle[%lx]\n", xSsHandle,
				sstFd->ss_handle);
		memcmp_s(&xSsHandle, sizeof(sshandle_t), &(sstFd->ss_handle),
				sizeof(sshandle_t), &nDiff);
		if (!nDiff) {
			/* First Node */
			if (count == 0)
				sst_fd = sst_fd->next_sst_node;
			/* Middle Node */
			else if (count && (count < stnOpenfdCount))
				prev_sstFd->next_sst_node = sstFd->next_sst_node;
			/* Last Node*/
			else
				prev_sstFd->next_sst_node = NULL;
			/* Close SST FD */
			close(sstFd->fd);
			/*Decrease total FD count by one */
			stnOpenfdCount--;
			/* Free SST FD resources */
			free(sstFd);
			break;
		}
		prev_sstFd = sstFd;
		sstFd = sstFd->next_sst_node;
	}

	return 0;
}

static int securestore_delete_close(sst_param_t xDelObject)
{
	int nRet = -SST_INVALID_DEVNODE_ERROR;
	sst_fd_handle_t *sstFd = NULL;

	if (!xDelObject.ss_handle) {
		LOGF_LOG_ERROR("<secure store> Invalid sshandle received\n");
		nRet = -SST_INVALID_SSHANDLE_ERROR;
		goto out;
	}

	sstFd = find_sstObject_by_ssHandle(xDelObject.ss_handle);

	if (sstFd == NULL) {
		LOGF_LOG_ERROR
		("<Secure store> ssHandle for Object not found\n");
		nRet = -SST_INVALID_SSHANDLE_ERROR;
		goto out;
	}

	if ((nRet = ioctl(sstFd->fd, SS_STG_DELETE_CLOSE, &xDelObject)) < 0) {
		LOGF_LOG_ERROR("<Secure store> Failed to delete SST Obj with error : -%d\n", errno);
		nRet = get_sst_fapi_error_code(errno);
		goto out;
    }

	/* Close SST FD entry */
	pthread_mutex_lock(&sst_lock);
	close_sst_fd(xDelObject.ss_handle);
	pthread_mutex_unlock(&sst_lock);
out:
	return nRet;
}

/**===============================================================================================
 * @brief Create and open a secure store object
 * 
 * @param pszObjectName
 * Secure store object name as a string
 * 
 * @param pcxObjPolicy
 * Pointer to secure store object configuration
 * 
 * @param unFlag
 * Flags for object access mode
 *
 * @param pxSsHandle
 * Secure store handle to access the object
 *
 * @return 
 *  negative value (error code) on failure
 *  0 on success
 ==================================================================================================
 */
int securestore_create_open(const char *pszObjectName,
						const sst_obj_config_t *pcxObjPolicy, const unsigned int unFlag,
						sshandle_t *pxSsHandle)
{
    sst_param_t xOpenObject;
    int nOpenFd = -1;
    int ret = -SST_INVALID_DEVNODE_ERROR;
    char *pcDev = NULL;
	sst_fd_handle_t *sstFd = NULL;
	int nObjectLen = -1;
	sst_fd_handle_t *new_sst_fd = sst_fd;
	bool wrap_asset_flag = false;

	nObjectLen = strnlen_s(pszObjectName, FILEPNAME_MAX + 1);

	if (!pszObjectName || !nObjectLen || (nObjectLen > FILEPNAME_MAX)) {
        LOGF_LOG_ERROR("<Secure store> Invalid data received\n");
        ret =  -SST_INVALID_OBJ_ERROR;
		goto out;
    }

	/* Check if user has passed create request */
	if (unFlag == SS_CREATE) {
		/* Check if user has passed the Object configuration */
		if (pcxObjPolicy == NULL) {
			LOGF_LOG_ERROR("<Secure store> Invalid object configuration");
			ret = -SST_OBJ_ACCESS_PERMS_ERROR;
			goto out;
		}
		/*Check if user has passed the valid crypto flags */
		if (!pcxObjPolicy->crypto_mode_flag) {
			LOGF_LOG_ERROR("<Secure store> Invalid crypto Flag error");
			ret = -SST_INVALID_CRYPTO_MODE_ERROR;
			goto out;
		}
	}

	/* Check if user has passed the valid asset number if wrap key is in OTP */
	if (pcxObjPolicy && pcxObjPolicy->wrap_asset.key_location) {
		if (pcxObjPolicy->wrap_asset.key_location & KEY_IN_OTP) { /* 0b01 - OTP */
			if ((pcxObjPolicy->wrap_asset.u.asset_number < MIN_OTP_ASSET_ID) ||
			    (pcxObjPolicy->wrap_asset.u.asset_number > MAX_OTP_ASSET_ID)) {
				LOGF_LOG_ERROR("<Secure store> Invalid wrap key configuration");
				ret = -SST_INVALID_ASSET_ID_ERROR;
				goto out;
			} else {
				wrap_asset_flag = true;
			}
		}
	}

    /* Check whether the ssHandle is already available, if yes return the same */
	sstFd = find_sstObject_by_name(pszObjectName);
	if (sstFd) {
		LOGF_LOG_ERROR
		("<Secure store> SST Object already exist\n");
        if (unFlag == SS_CREATE) {
			ret = -SST_OBJ_ALREADY_EXIST_ERROR;
		 } else {
			*pxSsHandle = sstFd->ss_handle;
			ret = SST_SUCCESS;
		}
		goto out;
	}

	memset_s(&xOpenObject, sizeof(sst_param_t), 0, sizeof(sst_param_t));
	xOpenObject.objectname = pszObjectName;
	xOpenObject.sobject_len = nObjectLen;
	if ((unFlag == SS_CREATE) || wrap_asset_flag) {
		memcpy_s(&xOpenObject.sst_access_policy,
			sizeof(sst_obj_config_t), pcxObjPolicy, sizeof(sst_obj_config_t));
		xOpenObject.secure_store_flags = pcxObjPolicy->crypto_mode_flag | unFlag;
	}
	/* SEC_STORE_ADMIN if user is with root permission */
	if ((pcxObjPolicy != NULL) && pcxObjPolicy->policy_attr.u.field.admin_store) {
		pcDev = SEC_STORE_ADMIN;
        xOpenObject.sst_access_policy.policy_attr.u.field.admin_store = 1;
	} else {                    /* Else Open SEC_STORE_NORMAL */
		pcDev = SEC_STORE_NORMAL;
	}

	if ((nOpenFd = open(pcDev, O_RDONLY)) < 0) {
        LOGF_LOG_ERROR
            ("<Secure store> Failed to open the Device\n");
        goto out;
    }

	if ((ret = ioctl(nOpenFd, SS_STG_CREATE_OPEN, &xOpenObject)) < 0) {
		LOGF_LOG_ERROR("<Secure store> Failed to open/create SST Obj with error : -%d\n", errno);
		ret = get_sst_fapi_error_code(errno);
		close(nOpenFd);
		goto out;
	}
	/* store the sshandle and fd for future use */
	/* closing the openfd and freeing the node will be taken core duting close */
	 pthread_mutex_lock(&sst_lock);
	if (!sst_fd) {
		sst_fd = calloc(1, sizeof(sst_fd_handle_t));
		if (!sst_fd) {
			LOGF_LOG_ERROR("<Secure store> Failed to allocate memory\n");
			pthread_mutex_unlock(&sst_lock);
			close(nOpenFd);
			return -ENOMEM;
		}
        sst_fd->ss_handle = xOpenObject.ss_handle;
        sst_fd->fd = nOpenFd;
		memcpy_s(sst_fd->objname, sizeof(sst_fd->objname), pszObjectName, nObjectLen);
		sst_fd->next_sst_node = NULL;
		stnOpenfdCount++;
	} else {
		while (new_sst_fd->next_sst_node != NULL)
			new_sst_fd = new_sst_fd->next_sst_node;

		new_sst_fd->next_sst_node = calloc(1, sizeof(sst_fd_handle_t));
		if (!new_sst_fd->next_sst_node) {
			LOGF_LOG_ERROR("<Secure store> Failed to allocate memory\n");
			close(nOpenFd);
			pthread_mutex_unlock(&sst_lock);
			return -ENOMEM;
		}
        new_sst_fd->next_sst_node->ss_handle = xOpenObject.ss_handle;
        new_sst_fd->next_sst_node->fd = nOpenFd;
		memcpy_s(new_sst_fd->next_sst_node->objname, sizeof(sst_fd->objname), pszObjectName, nObjectLen);
		new_sst_fd->next_sst_node->next_sst_node = NULL;
		stnOpenfdCount++;
	}
	pthread_mutex_unlock(&sst_lock);

	*pxSsHandle = (sshandle_t)xOpenObject.ss_handle;

out:
	return ret;
}

/**===============================================================================================
 * @brief Stores the plain data in to secure store object in cryptographically protected form
 * 
 * @param xSsHandle
 * Secure store handle
 * 
 * @param pucDataBuf
 * Pointer to the data buffer to be protected
 * 
 * @param unDataLen
 * Length of data to be protected
 * 
 * @return 
 *  negative value (error code) on failure
 *  0 on success
 ==================================================================================================
 */
int securestore_save(const sshandle_t xSsHandle, const secure_wrap_asset_t *pxWrapConfig,
						const unsigned char *pucDataBuf, const unsigned int unDataLen)
{
    sst_data_param_t xSaveObject;
	sst_fd_handle_t *sstFd = NULL;
    int nRet = -SST_INVALID_DEVNODE_ERROR;

	if (!xSsHandle) {
		LOGF_LOG_ERROR("<secure store> Invalid sshandle received\n");
		nRet = -SST_INVALID_SSHANDLE_ERROR;
		goto out;
	}

	if (!pucDataBuf || !unDataLen) {
        LOGF_LOG_ERROR("<Secure store> Invalid data received\n");
		nRet = -SST_INVALID_DATABUF_ERROR;
		goto out;
    }

	/* Check if user has passed the valid asset number if wrap key is in OTP */
	if (pxWrapConfig && pxWrapConfig->key_location) {
		if (pxWrapConfig->key_location & KEY_IN_OTP) { /* 0b01 - OTP */
			if ((pxWrapConfig->u.asset_number < MIN_OTP_ASSET_ID) ||
			    (pxWrapConfig->u.asset_number > MAX_OTP_ASSET_ID)) {
				LOGF_LOG_ERROR("<Secure store> Invalid wrap key configuration");
				nRet = -SST_INVALID_ASSET_ID_ERROR;
				goto out;
			}
		}
	}

	memset_s(&xSaveObject, sizeof(sst_data_param_t), 0,
			sizeof(sst_data_param_t));

	xSaveObject.ss_handle = xSsHandle;
	xSaveObject.payload_len = unDataLen;
	xSaveObject.payload = pucDataBuf;

	if (pxWrapConfig) {
		memcpy_s(&xSaveObject.wrap_asset,
			sizeof(secure_wrap_asset_t), pxWrapConfig, sizeof(secure_wrap_asset_t));
	}

	sstFd = find_sstObject_by_ssHandle(xSsHandle);
	if (sstFd == NULL) {
		LOGF_LOG_ERROR
		("<Secure store> ssHandle for Object not found\n");
		nRet = -SST_INVALID_SSHANDLE_ERROR;
		goto out;
	}

	if ((nRet = ioctl(sstFd->fd, SS_STG_SAVE, &xSaveObject)) < 0) {
		LOGF_LOG_ERROR("<Secure store> Failed to Save SST Obj data with error : -%d\n", errno);
		nRet = get_sst_fapi_error_code(errno);
        goto out;;
    }

out:
	return nRet;
}

/**===============================================================================================
 * @brief Retrieves the plain data object from secure store
 * 
 * @param xSsHandle
 * Secure store handle
 * 
 * @param pucDataBuf
 * Pointer to hold the plain data buffer
 * 
 * @param unDataLen
 * Length of data to be retrieved
 * 
 * @param punActDataLen
 * Actual length of plain data
 * 
 * @return 
 *  negative value (error code) on failure
 *  0 on success
 ==================================================================================================
 */
int securestore_retrieve(const sshandle_t xSsHandle, const secure_wrap_asset_t *pxWrapConfig,
					unsigned char *pucDataBuf, const unsigned int unDataLen, unsigned int *punActDataLen)
{
    sst_data_param_t xLoadObject;
	sst_fd_handle_t *sstFd = NULL;
    int nRet = -SST_INVALID_DEVNODE_ERROR;

	if (!xSsHandle) {
		LOGF_LOG_ERROR("<secure store> Invalid sshandle received\n");
		nRet = -SST_INVALID_SSHANDLE_ERROR;
		goto out;
	}

	if (!pucDataBuf || !unDataLen) {
        LOGF_LOG_ERROR("<Secure store> Invalid data buffer or length \n");
		nRet = -SST_INVALID_DATABUF_ERROR;
		goto out;
    }

	/* Check if user has passed the valid asset number if wrap key is in OTP */
	if (pxWrapConfig && pxWrapConfig->key_location) {
		if (pxWrapConfig->key_location & KEY_IN_OTP) { /* 0b01 - OTP */
			if ((pxWrapConfig->u.asset_number < MIN_OTP_ASSET_ID) ||
			    (pxWrapConfig->u.asset_number > MAX_OTP_ASSET_ID)) {
				LOGF_LOG_ERROR("<Secure store> Invalid wrap key configuration");
				nRet = -SST_INVALID_ASSET_ID_ERROR;
				goto out;
			}
		}
	}

	memset_s(&xLoadObject, sizeof(sst_data_param_t), 0,
			sizeof(sst_data_param_t));

	xLoadObject.ss_handle = xSsHandle;
	xLoadObject.payload_len = unDataLen;
	xLoadObject.payload = pucDataBuf;

	if (pxWrapConfig) {
		memcpy_s(&xLoadObject.wrap_asset,
			sizeof(secure_wrap_asset_t), pxWrapConfig, sizeof(secure_wrap_asset_t));
	}

	sstFd = find_sstObject_by_ssHandle(xSsHandle);
	if (sstFd == NULL) {
		LOGF_LOG_ERROR
		("<Secure store> ssHandle for Object not found\n");
		nRet = -SST_INVALID_SSHANDLE_ERROR;
		goto out;
	}

	if ((nRet = ioctl(sstFd->fd, SS_STG_RESTORE, &xLoadObject)) < 0) {
		LOGF_LOG_ERROR("<Secure store> Failed to Load SST Obj data with error : -%d\n", errno);
		nRet = get_sst_fapi_error_code(errno);
        goto out;
    }

	*punActDataLen = nRet;

out:
	return nRet;
}

/**===============================================================================================
 * @brief Deletes the data object from secure store
 *
 * @param xSsHandle
 * Secure store handle
 *
 * @return
 *  negative value (error code) on failure
 *  0 on success
 ==================================================================================================
 */
int securestore_delete(const sshandle_t xSsHandle)
{
    sst_param_t xDelObject;

	memset_s(&xDelObject, sizeof(sst_param_t), 0,
			sizeof(sst_param_t));
	xDelObject.ss_handle = xSsHandle;
	xDelObject.secure_store_flags = SS_DELETE;

	return securestore_delete_close(xDelObject);
}

/**===============================================================================================
 * @brief Closes the data object from secure store
 * 
 * @param xSsHandle
 * secure store handle
 * 
 * @return 
 *  negative value (error code) on failure
 *  0 on success
 ==================================================================================================
 */
int securestore_close(const sshandle_t xSsHandle)
{
    sst_param_t xDelObject;

	memset_s(&xDelObject, sizeof(sst_param_t), 0,
			sizeof(sst_param_t));
	xDelObject.ss_handle = xSsHandle;

	return securestore_delete_close(xDelObject);
}
