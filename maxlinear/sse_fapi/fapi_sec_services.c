/******************************************************************************

  Copyright (C) 2020-2024 MaxLinear, Inc.
  Copyright (C) 2019 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/

/***************************************************************************** *
 *     File Name  : fapi_sec_services.c					       *
 *     Project    : UGW                                                        *
 *     Description: secure service fapi, it provides set of API's to encrypt   *
 *		    /decrypt.						       *
 *                                                                             *
 ******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "fapi_sec_service.h"
#include "help_error.h"
#include "help_logging.h"

#include "safe_str_lib.h"
#include "safe_lib.h"
#include "safe_mem_lib.h"

#ifdef LGM_SECURE
#include <secure_services.h>
#else
/* original definitions are available in sse_driver_lgm */
#define SS_PATH "/dev/secure_service_driver"
#define SS_MAGIC 'H'
#define SS_IOC_SEC_DECRYPT _IOW(SS_MAGIC, 1, file_param_t)
#define SS_IOC_SEC_ENCRYPT _IOW(SS_MAGIC, 2, file_param_t)

typedef struct file_param_ {
    unsigned char *address;
    int size;
} file_param_t;
#endif


/* lock file for sse operation */
#define LOCK_FILE "/opt/intel/etc/sse/sse_lock"

uint16_t LOGLEVEL = SYS_LOG_DEBUG + 1;
uint16_t LOGTYPE = SYS_LOG_TYPE_FILE;

/**
  ====================================================================================================
 * @brief file open
 ====================================================================================================
 */
int fapi_Fileopen(const char *pcFileName, int flags, mode_t mode)
{
    return open(pcFileName, flags, mode);
}

#ifdef DEBUG
/**
  ====================================================================================================
 * @brief Hexdump function
 ====================================================================================================
 */
void fapi_hexDump(unsigned char *pcBuf, unsigned int unLen)
{
    if(unLen > 0) {
        while(unLen--)
            printf("%02x", *pcBuf++);
    }
    printf("\n");
}
#endif

/**===============================================================================================
 * @brief  Sends the file parameters to Secure Service Driver to Decrypt.
 * 
 * The API uses ioctl system call to send the physical address of the file, size of the file,
 * and AssetId (for key) to Secure Services Driver
 *
 * @pre nFd != NULL
 *
 * @param nFd   
 * file descriptor of the file obtained after opening the image
 * 
 * @return 
 * UGW_SUCCESS on success
 * UGW_FAILURE/ERR_IOCTL_FAILED on failure
 ==================================================================================================
 */

static int fapi_doDecrypt(sse_param_t *pxSSEParamList){

	fd_set xReadFd;
	int nVal = 0, nRet = 0;
	file_param_t xFileParam;
	int nSecFd = -1;
#ifndef EIP123_DEF
	int nLoopCnt=0;
#endif

	if(pxSSEParamList->len == 0 || pxSSEParamList->pcBuf == NULL)
		return UGW_FAILURE;

	if((nSecFd = fapi_Fileopen(SS_PATH, (O_RDWR & (~O_NONBLOCK)),0)) > 0){	
		FD_ZERO(&xReadFd);
		FD_SET(nSecFd, &xReadFd);

		memset_s(&xFileParam, sizeof(file_param_t), 0, sizeof(file_param_t));

		xFileParam.size = pxSSEParamList->len;
		xFileParam.address = pxSSEParamList->pcBuf;

		if((ioctl(nSecFd, SS_IOC_SEC_DECRYPT, &xFileParam)) != UGW_SUCCESS){
			nRet=ERR_IOCTL_FAILED;
			goto finish;
		}

		nVal = select(nSecFd+1, &xReadFd, NULL, NULL, NULL);
		if (nVal < 0  || errno == EINTR ) {
			LOGF_LOG_ERROR("select error\n");
		} else {
			if(FD_ISSET(nSecFd, &xReadFd)){

				memset_s(pxSSEParamList->pcBuf, MAX_SIZE, 0,MAX_SIZE);

				nRet = read(nSecFd, pxSSEParamList->pcBuf, MAX_SIZE);
				if (nRet < 0) {
					nRet=UGW_FAILURE;
					goto finish;
				}
#ifndef EIP123_DEF
				// special charcter to identify the padding data. bad logic to have, then use eip123.
				while (pxSSEParamList->pcBuf[nRet-1] == '~' ) {
					nRet--;
					if(nLoopCnt < 16 )
						nLoopCnt++;
					else
						break;
				}
#endif
				pxSSEParamList->pcBuf[nRet] = '\0';
				pxSSEParamList->len= nRet;
				nRet = UGW_SUCCESS;
			}
		}
	}
finish:
	if(nSecFd>=0)
		close(nSecFd);
	return nRet;
}

/**===============================================================================================
 * @brief  Sends the file parameters to Secure Service Driver to Encrypt.
 * 
 * The API uses ioctl system call to send the physical address of the file, size of the file,
 * and AssetId (for key) to Secure Services Driver.
 *
 * @pre nFd != NULL
 *
 * @param nFd   
 * file descriptor of the file obtained after opening the image
 * 
 * @return 
 * file descriptor (nFd) of the file on success
 * -1 on failure
 ==================================================================================================
 */

static int fapi_doEncrypt(sse_param_t *pxSSEParamList){

	fd_set xReadFd;
	int nVal = 0, nRet = UGW_FAILURE;
	file_param_t xFileParam;
	int nSecFd = -1;
	int nTmpSize = 0;

#ifndef EIP123_DEF
	int nLoopCnt=0;
#endif

	if(pxSSEParamList->len == 0 || pxSSEParamList->pcBuf == NULL)
		return UGW_FAILURE;

	if((nSecFd = fapi_Fileopen(SS_PATH, (O_RDWR & (~O_NONBLOCK)),0)) > 0){

		FD_ZERO(&xReadFd);
		FD_SET(nSecFd, &xReadFd);

#ifndef EIP123_DEF
		// finding padding bytes length to pad the data.
		if((pxSSEParamList->len % block_length) != 0){
			nTmpSize = pxSSEParamList->len;
			while((nTmpSize % block_length) != 0){
				nTmpSize++;
			}
		}else{
			nTmpSize = pxSSEParamList->len;
		}
#else
		nTmpSize = pxSSEParamList->len;
#endif

		memset_s(&xFileParam, sizeof(file_param_t), 0, sizeof(file_param_t));

		xFileParam.size = nTmpSize;

#ifndef EIP123_DEF
		for(nLoopCnt = pxSSEParamList->len; nLoopCnt < nTmpSize; nLoopCnt++){
		//using special charcter to pad the data, bad logic to have, then use eip123.
			pxSSEParamList->pcBuf[nLoopCnt]='~';
		}
#endif

		xFileParam.address = pxSSEParamList->pcBuf;

		if ((ioctl(nSecFd, SS_IOC_SEC_ENCRYPT, &xFileParam)) != UGW_SUCCESS) {
			LOGF_LOG_ERROR("ioctl failed.\n");
			nRet = ERR_IOCTL_FAILED;
			goto finish;
		}
		nVal = select(nSecFd+1, &xReadFd, NULL, NULL, NULL);
		if(nVal < 0 || errno == EINTR ){
			LOGF_LOG_ERROR("select error\n");
		}else{
			if(FD_ISSET(nSecFd, &xReadFd)){

				memset_s(pxSSEParamList->pcBuf, MAX_SIZE, 0, MAX_SIZE);

				nRet = read(nSecFd, pxSSEParamList->pcBuf, MAX_SIZE);
				if(nRet < 0){
					LOGF_LOG_ERROR("Read failed\n");
					nRet=UGW_FAILURE;
					goto finish;
				}

				pxSSEParamList->len = nRet;
				nRet = UGW_SUCCESS;
			}
		}
	}
finish:
	if(nSecFd>=0)
		close(nSecFd);
	return nRet;
}

/**===============================================================================================
 * @brief  function to allocate the buffer
 *
 * @param pxSSEParamList secure storage structure
 *
 * @return
 * UGW_SUCCESS/UGW_FAILURE
 *
 ==================================================================================================
 */
int sse_buf_alloc(sse_param_t *pxSSEParamList)
{
    int nRet = UGW_SUCCESS;

	pxSSEParamList->pcBuf = calloc(1, MAX_SIZE);
	if ( pxSSEParamList->pcBuf == NULL) {
		nRet = UGW_FAILURE;
    }

    return nRet;
}

/**===============================================================================================
 * @brief  function to free the buffer
 *
 * @param pxSSEParamList secure storage structure
 *
 * @return
 * UGW_SUCCESS/UGW_FAILURE
 *
 ==================================================================================================
 */
void sse_buf_free(sse_param_t *pxSSEParamList)
{
	if ( pxSSEParamList->pcBuf != NULL)
		free(pxSSEParamList->pcBuf);
}

/**===============================================================================================
 * @brief  Securely read the file, specified in the path.
 * 
 * @param pcPath   
 * The path of the file that need to be decrypted/read
 * 
 * @return 
 * file descriptor (nFd) of the file on success  and -1 on failure
 * 
 ==================================================================================================
 */

int fapi_ssFRead(sse_param_t *pxSSEParamList)
{
	int nFd = -1, nLockFd=-1, nRet=UGW_FAILURE;

	if ((nLockFd = fapi_Fileopen(LOCK_FILE, O_RDONLY,0)) < 0) {
		LOGF_LOG_ERROR("LOCK FILE open failed [%s]\n",strerror(errno));
		return UGW_FAILURE;
	}

	if (flock(nLockFd, LOCK_EX) < 0) {
		LOGF_LOG_ERROR("flock failed with reason [%s]\n", strerror(errno));
		if (close(nLockFd) < 0) {
			LOGF_LOG_DEBUG("close failed with reason [%s]\n", strerror(errno));
		}
		return UGW_FAILURE;
	}

	/* if caller sends empty allocated buffer, it means read the content from the path*/
	if (!pxSSEParamList->len) {
		if(pxSSEParamList->sPath){
			if((nFd = fapi_Fileopen(pxSSEParamList->sPath, O_RDONLY,0))>0) {
				if ((nRet=read(nFd, pxSSEParamList->pcBuf, MAX_SIZE))!= -1)
					pxSSEParamList->len=nRet;
				else {
					LOGF_LOG_ERROR("input file read failed (%s)\n",strerror(errno));
					nRet = UGW_FAILURE;
					goto finish;
				}
			} else {
				LOGF_LOG_ERROR("input file open failed (%s)\n",strerror(errno));
				nRet = UGW_FAILURE;
				goto finish;
			}
		}
	}

	/* buffer will have decrypted content */
	nRet = fapi_doDecrypt(pxSSEParamList);
	/* Below nRet=UGW_SUCCESS has to be removed once vault ip 130 is enabled on LGM boards */
	nRet = UGW_SUCCESS;

finish:
	if(flock(nLockFd, LOCK_UN) < 0){
		LOGF_LOG_DEBUG("unlock failed with reason [%s]\n", strerror(errno));
	}

	if(close(nLockFd) < 0){
		LOGF_LOG_DEBUG("close failed with reason [%s]\n", strerror(errno));
	}

	if (nFd >= 0)
		close(nFd);

	return nRet;
}

/**===============================================================================================
 * @brief  Securely write the file, specified in the path.
 * 
 * @param pcPath   
 * The path of the file that need to be encrypted/written. 
 * 
 * @return 
 * file descriptor (fd) of the file on success
 * -1 on failure

 ==================================================================================================
 */

int fapi_ssFWrite(sse_param_t *pxSSEParamList)
{
	int nFd = -1, nLockFd=-1, nRet=UGW_FAILURE;

	if ((nLockFd = fapi_Fileopen(LOCK_FILE, O_RDONLY,0)) < 0) {
		LOGF_LOG_ERROR("LOCK FILE open failed [%s]\n",strerror(errno));
		return UGW_FAILURE;
	}

	if (flock(nLockFd, LOCK_EX) < 0) {
		LOGF_LOG_ERROR("flock failed with reason [%s]\n", strerror(errno));
		if (close(nLockFd) < 0) {
			LOGF_LOG_DEBUG("close failed with reason [%s]\n", strerror(errno));
		}
		return UGW_FAILURE;
	}

	/* if caller sends empty allocated buffer, it means read the content from the path*/
	if (!pxSSEParamList->len) {
		if(pxSSEParamList->sPath){
			if((nFd = fapi_Fileopen(pxSSEParamList->sPath, O_RDONLY,0))>0) {
				if ((nRet=read(nFd, pxSSEParamList->pcBuf, MAX_SIZE))!= -1){
					pxSSEParamList->len=nRet;
				} else {
					LOGF_LOG_ERROR("file read error %s\n", strerror(errno));
					nRet = UGW_FAILURE;
					goto finish;
				}
				if (nFd >= 0)
					close(nFd);
			} else {
				LOGF_LOG_ERROR("file open error %s\n", strerror(errno));
				nRet = UGW_FAILURE;
				goto finish;
			}
		}
	}

	nRet = fapi_doEncrypt(pxSSEParamList);
	/* Below nRet=UGW_SUCCESS has to be removed once vault ip 130 is enabled on LGM boards */
	nRet = UGW_SUCCESS;

	/* write encrypted buffer in passed path by caller, once DAC implmentation in place
	   caller will pass objectid, based on objectid secure fapi will decide to store the encrypted content,
	   in this case API expects buffer should be filled by the caller */
#if 0
	if (nRet == UGW_SUCCESS) {
		if((nFd = fapi_Fileopen(pxSSEParamList->sPath, O_RDWR|O_CREAT, U_RDWR_G_RD))>0) {
			if ( write(nFd, pxSSEParamList->pcBuf, pxSSEParamList->len)  == -1) {
				LOGF_LOG_ERROR("write back failed [%s]\n", strerror(errno));
				nRet = UGW_FAILURE;
				goto finish;
			}
			if (ftruncate(nFd, pxSSEParamList->len)<0) {
				perror("Truncate failed");
				nRet = ERR_FILE_TRUNCATE_FAILED;
				goto finish;
			}
		} else {
			LOGF_LOG_ERROR("file open error %s\n", strerror(errno));
			nRet = UGW_FAILURE;
			goto finish;
		}
		sync();
	} else { 
		fprintf(stderr, "@@@@@ %s:%d Encryption failed ret(%d)\n",__func__,__LINE__, nRet);
	}
#endif

finish:
	if(flock(nLockFd, LOCK_UN) < 0){
		LOGF_LOG_DEBUG("unlock failed with reason [%s]\n", strerror(errno));
	}

	if(close(nLockFd) < 0){
		LOGF_LOG_DEBUG("close failed with reason [%s]\n", strerror(errno));
	}

	if (nFd >= 0)
		close(nFd);

	return nRet;
}
