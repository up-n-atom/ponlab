/******************************************************************************
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

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/file.h> 
#include <errno.h>
#include <sys/types.h>
#include <limits.h>
#include <crc32.h>
#include "fapi_sec_service.h"
#include <addver.h>
#include "help_error.h"
#include "help_logging.h"

static unsigned char sState[][12]={"UPG_NONE", "UPG_REQ", "UPG_INPROG", "UPG_SUCC", "UPG_FAIL", "UPG_RES" };

#define DATA_SIZE 32
#define DEV_MODEL_NAME_SIZE 256

//for other flash we need to see how fetch this count
#define UBI_FLASH_CUR_CNT "cat /sys/class/ubi/ubi0/max_ec"
#define DEV_TREE_MODEL_FILE "/proc/device-tree/model"
#define LOCK_FILE "/opt/intel/etc/sse/sse_lock"

#ifndef LOG_LEVEL
uint16_t LOGLEVEL = SYS_LOG_DEBUG + 1;
#else
uint16_t LOGLEVEL = LOG_LEVEL + 1;
#endif

#ifndef LOG_TYPE
uint16_t LOGTYPE = SYS_LOG_TYPE_FILE;
#else
uint16_t LOGTYPE = LOG_TYPE;
#endif


/**
  ====================================================================================================
 * @brief file open
 ====================================================================================================
 */
int fapi_Fileopen(const char *pcFileName, int flags, mode_t mode){
	return open(pcFileName, flags, mode);
}

#ifdef DEBUG
/**
  ====================================================================================================
 * @brief Hexdump function
 ====================================================================================================
 */
static void fapi_hexDump(unsigned char *pcBuf, unsigned int unLen){
	if(unLen > 0){
		while (unLen--)
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
 * file descriptor (nFd) of the file on success
 * -1 on failure
 ==================================================================================================
 */

static int fapi_doDecrypt(sse_param_t *pxSSEParamList){

	fd_set xReadFd;
	int nVal = 0, nRet = UGW_FAILURE;
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
				if (nRet <= 0) {
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
				if(nRet <= 0){
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
	pxSSEParamList->pcBuf = calloc(1,MAX_SIZE);
	if ( pxSSEParamList->pcBuf == NULL)
		return UGW_FAILURE;
	else
		return UGW_SUCCESS;
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

/**===============================================================================================
 * @brief  Securely validate the image
 * 
 * @param nFd
 * The fd of the file that need to be validated
 * 
 * @return
 *  UGW_SUCCESS on success
 *  UGW_FAILURE on failure

 ==================================================================================================
 */
#ifdef SECURE_BOOT_AUTH_SUPPORT
static int fapi_ssSecImgAuth(img_param_t xImgParam){

	fd_set xReadFd;
//	int nVal = 0;
	int nSecFd = -1;
	int nRet=UGW_SUCCESS;

	/* Reset File fd */
	lseek(xImgParam.src_img_fd, 0, SEEK_SET);

	if((nSecFd = fapi_Fileopen(SS_PATH, (O_RDWR & (~O_NONBLOCK)), U_RDWR_G_RD)) > 0){

		FD_ZERO(&xReadFd);
		FD_SET(nSecFd, &xReadFd);

		if((nRet = ioctl(nSecFd, SS_IOC_SEC_IMGAUTH, &xImgParam)) != UGW_SUCCESS){
			LOGF_LOG_ERROR("ioctl failed.\n");
			nRet = UGW_FAILURE;
			goto finish;
		}

#if 0 /*not receiving decrypted image for now. */
		/* handle decrypted image content */
		nVal = select(nSecFd+1, &xReadFd, NULL, NULL, NULL);
		if(nVal < 0 || errno == EINTR ){
			LOGF_LOG_ERROR("select error\n");
		}else{
			if(FD_ISSET(nSecFd, &xReadFd)){
				nRet = read(nSecFd, xImgParam.src_img_addr, MAX_SIZE);
				if(nRet < 0){
					LOGF_LOG_ERROR("Read failed\n");
					nRet=UGW_FAILURE;
					goto finish;
				}
				nRet = UGW_SUCCESS;
			}
		}
#endif
	}
finish:
	if(nSecFd>=0) 
		close(nSecFd);

	return nRet;
}

#endif

/**===============================================================================================
 * @brief  fapi_ssParaseTimeStamp
 *
 * @param
 *
 * @return
 *  UGW_SUCCESS on success
 *  UGW_FAILURE on failure

 ==================================================================================================
 */
static int fapi_ssParaseTimeStamp(char **pcEndPtr){

	int nCnt;
	char *pcTmp;
	char sBuf[MAX_SS_BUF_SIZE]={0};

	pcTmp=strstr(*pcEndPtr,":");
	nCnt=(size_t)(pcTmp-*pcEndPtr);
	sprintf_s(sBuf,nCnt+1,"%s",*pcEndPtr);
	*pcEndPtr = *pcEndPtr+nCnt+1;
	return atoi(sBuf);
}

/**===============================================================================================
 * @brief  fapi_ssUpdateFlashDefaultCnt
 *
 * @param
 *
 * @return
 *  UGW_SUCCESS on success
 *  UGW_FAILURE on failure

 ==================================================================================================
 */
static void fapi_ssUpdateFlashDefaultCnt(FILE *fp){

	time_t t = time(NULL);
	struct tm *tm ;

	if(t != -1) {
		tm = localtime(&t);

		if(tm != NULL){
			fprintf(fp, "%s%d\n",F_MAX_FLASH_CNT, MAX_FLASH_CNT_SUPPORT);
			fprintf(fp, "%s0\n",F_FLASH_CNT);
			fprintf(fp, "%s0\n",F_DAY_CNT);
			fprintf(fp, "%s%d:%d:%d:%d:%d:%d\n",   F_LAST_UPDATE_TIME_STAMP,
					tm->tm_year + YR_MAGIC,
					tm->tm_mon + MM_MAGIC,
					tm->tm_mday,
					tm->tm_hour,
					tm->tm_min,
					tm->tm_sec);
		}
	}
}

/**===============================================================================================
 * @brief  fapi_ssFetchCurrentCnt
 *
 * @param
 *
 * @return
 *  UGW_SUCCESS on success
 *  UGW_FAILURE on failure

 ==================================================================================================
 */
static int fapi_ssFetchCurrentCnt(size_t *nTtCnt){

	FILE *pf=NULL;
	char cData[DATA_SIZE];
	size_t nTmp;
	int nRet = UGW_SUCCESS;

	pf = popen(UBI_FLASH_CUR_CNT,"r");
	if(!pf){
		fprintf(stderr, "Could not open pipe for output. %s\n",UBI_FLASH_CUR_CNT);
		return UGW_FAILURE;
	}

	fgets(cData, DATA_SIZE , pf);

	nTmp = atoi(cData);

	//20% buffer 
	if(nTmp < ((MAX_FLASH_CNT_SUPPORT*20)/100))
		*nTtCnt = nTmp;
	else
		nRet=UGW_FAILURE;

	if (pclose(pf) != 0)
		fprintf(stderr," Error: Failed to close command stream \n");

	return nRet;
}

/**===============================================================================================
 * @brief  flash-wearout check
 * 
 * @return
 *  UGW_SUCCESS on success
 *  UGW_FAILURE on failure

 ==================================================================================================
 */
static int fapi_ssFlashWearOutChk(void){

	int nRet=UGW_SUCCESS;
	struct tm start_date={0}, end_date={0};
	time_t t={0};
	struct tm *tm=NULL;
	size_t nMaxFlashCnt=0, nFlashCnt=0, nDayFlashCnt=0;

	time_t start_time={0}, end_time={0};
	double seconds=0;

	FILE *fp=NULL;
	size_t len = 0, nTtCnt=0;
	char *pcEndPtr=NULL, *line=NULL;
	struct flock fl = {};


	if(fopen_s(&fp, FLASH_CNT_CHK, "r") == EOK){

		if(fp == NULL ){
			LOGF_LOG_ERROR("file open failed \n");
			return UGW_FAILURE;
		}

		fl.l_type = F_RDLCK;
		fl.l_whence = SEEK_SET;
		fl.l_start = 0;
		fl.l_len = 0;

		if (fcntl(fileno(fp), F_SETLKW, &fl) == -1) {
			LOGF_LOG_ERROR("file lock failed\n");
			nRet = UGW_FAILURE;
			goto finish;
		}

		while ( getline(&line, &len, fp) != -1 ) {
			if((pcEndPtr=strstr(line,F_LAST_UPDATE_TIME_STAMP)) != NULL) {
				pcEndPtr=pcEndPtr+(sizeof(F_LAST_UPDATE_TIME_STAMP)-1);
				start_date.tm_year = fapi_ssParaseTimeStamp(&pcEndPtr);
				start_date.tm_mon  = fapi_ssParaseTimeStamp(&pcEndPtr);
				start_date.tm_mday = fapi_ssParaseTimeStamp(&pcEndPtr);
				start_date.tm_hour = fapi_ssParaseTimeStamp(&pcEndPtr);
				start_date.tm_min  = fapi_ssParaseTimeStamp(&pcEndPtr);
				start_date.tm_sec  = atoi(pcEndPtr);
			}else if((pcEndPtr=strstr(line,F_DAY_CNT)) != NULL) {
				pcEndPtr=pcEndPtr+(sizeof(F_DAY_CNT)-1);
				nDayFlashCnt=atoi(pcEndPtr);
			}else if((pcEndPtr=strstr(line,F_FLASH_CNT)) != NULL) {
				pcEndPtr=pcEndPtr+(sizeof(F_FLASH_CNT)-1);
				nFlashCnt=atoi(pcEndPtr);
			}else if((pcEndPtr=strstr(line,F_MAX_FLASH_CNT)) != NULL) {
				pcEndPtr=pcEndPtr+(sizeof(F_MAX_FLASH_CNT)-1);
				nMaxFlashCnt=atoi(pcEndPtr);
			}
		}


		if (line != NULL)
			free(line);

		t = time(NULL);
		if(t != -1){
			tm = localtime(&t);
			if(tm != NULL){
				end_date.tm_hour = tm->tm_hour;
				end_date.tm_min  = tm->tm_min;
				end_date.tm_sec  = tm->tm_sec;
				end_date.tm_mon  = tm->tm_mon + MM_MAGIC;
				end_date.tm_mday = tm->tm_mday;
				end_date.tm_year = tm->tm_year + YR_MAGIC;

				start_time = mktime(&start_date);
				end_time = mktime(&end_date);

				seconds = difftime(end_time, start_time);
			}
		}

		if((seconds / (60 * 60 * 24)) < 1){
			/* update day flash cnt */
			if(nDayFlashCnt < MAX_DAY_FLASH_CNT)
				nDayFlashCnt++;
			else{
				LOGF_LOG_ERROR("Max Limit reached for Day ...");
				nRet = UGW_FAILURE;
				goto next;
			}
		}else{
			/*reset day cnt */
			nDayFlashCnt=0;
		}

		if((fapi_ssFetchCurrentCnt(&nTtCnt)) == UGW_FAILURE){
			LOGF_LOG_ERROR("Max Limit reached flash write, No more updates possible\n");
			nRet=UGW_FAILURE;
			goto next;
		}

		/* update flash cnt */
		if(nFlashCnt < nTtCnt)
			nFlashCnt++;
		else{
			LOGF_LOG_ERROR("Max Limit reached flash write, No more updates possible\n");
			nRet = UGW_FAILURE;
			goto next;
		}

		fl.l_type = F_UNLCK;
		if (fcntl(fileno(fp), F_SETLK, &fl) == -1) {
			LOGF_LOG_ERROR("file unlock failed\n");
			nRet = UGW_FAILURE;
		}
		if(fp != NULL)
			fclose(fp);

		if(fopen_s(&fp, FLASH_CNT_CHK, "w") != EOK){
			LOGF_LOG_ERROR("file open failed \n");
			nRet = UGW_FAILURE;
			goto finish;
		}

		if(fp == NULL ){
			LOGF_LOG_ERROR("file open failed \n");
			nRet = UGW_FAILURE;
			goto finish;
		}

		fl.l_type = F_WRLCK;
		fl.l_whence = SEEK_SET;
		fl.l_start = 0;
		fl.l_len = 0;

		if (fcntl(fileno(fp), F_SETLKW, &fl) == -1) {
			LOGF_LOG_ERROR("file lock failed\n");
			nRet = UGW_FAILURE;
			goto finish;
		}


		fprintf(fp, "%s%zu\n",F_MAX_FLASH_CNT, nMaxFlashCnt);
		fprintf(fp, "%s%zu\n",F_FLASH_CNT,nFlashCnt);
		fprintf(fp, "%s%zu\n",F_DAY_CNT,nDayFlashCnt);
		fprintf(fp, "%s%d:%d:%d:%d:%d:%d\n",   F_LAST_UPDATE_TIME_STAMP,
				end_date.tm_year,
				end_date.tm_mon,
				end_date.tm_mday,
				end_date.tm_hour,
				end_date.tm_min,
				end_date.tm_sec);
next:
		fl.l_type = F_UNLCK;
		if (fcntl(fileno(fp), F_SETLK, &fl) == -1) {
			LOGF_LOG_ERROR("file unlock failed\n");
			nRet = UGW_FAILURE;
		}
	}else{
		/* first boot update flash wear-out variables to the file */
		if(fopen_s(&fp,FLASH_CNT_CHK, "w") != EOK){
			LOGF_LOG_ERROR("file open failed \n");
			nRet=UGW_FAILURE;
			goto finish;
		}
		if(fp == NULL ){
			LOGF_LOG_ERROR("file open failed \n");
			nRet=UGW_FAILURE;
			goto finish;
		}
		fl.l_type = F_WRLCK;
		fl.l_whence = SEEK_SET;
		fl.l_start = 0;
		fl.l_len = 0;

		if (fcntl(fileno(fp), F_SETLKW, &fl) == -1) {
			LOGF_LOG_ERROR("file lock failed\n");
			nRet = UGW_FAILURE;
			goto finish;
		}

		// adding default values for first time
		fapi_ssUpdateFlashDefaultCnt(fp);

		fl.l_type = F_UNLCK;
		if (fcntl(fileno(fp), F_SETLK, &fl) == -1) {
			LOGF_LOG_ERROR("file unlock failed\n");
			nRet = UGW_FAILURE;
		}
	}
finish:
	if(fp != NULL)
		fclose(fp);

	return nRet;
}

/**===============================================================================================
 * @brief  fetch the model name from the device tree.
 *
 * @return
 *  UGW_SUCCESS on model matches with device tree and image header
 *  UGW_FAILURE on failure in case not matches.

 ==================================================================================================
 */
static int fapi_verifyModel(const char *pcModelName)
{

	FILE *pf=NULL;
	int nRet=UGW_FAILURE;

	char cData[DEV_MODEL_NAME_SIZE];

	if (pcModelName == NULL) {
		fprintf(stderr, "Model Name Can't be empty.\n");
		return  UGW_FAILURE;
	}

	pf = popen(DEV_TREE_MODEL_FILE,"r");
	if(!pf){
		fprintf(stderr, "Could not open pipe for output. %s\n",DEV_TREE_MODEL_FILE);
		return UGW_FAILURE;
	}

	if (fgets(cData, DEV_MODEL_NAME_SIZE , pf) != NULL) {
		if (strstr(cData, pcModelName) != NULL)
			nRet=UGW_SUCCESS;
		else{
			fprintf(stderr, "Image Hdr[%s] and Device Hdr[%s] not matching, image upgrade not possible",pcModelName, cData);
			nRet=UGW_FAILURE;
		}
	}

	if(pclose(pf) == -1)
		 fprintf(stderr, "pclose failed for %s and reason %s\n",DEV_TREE_MODEL_FILE,strerror(errno));

	return nRet;
}

/**===============================================================================================
 * @brief  Securely validate the image 
 * 
 * @param nFd   
 * The fd of the file that need to be validated
 * 
 * @return 
 *  UGW_SUCCESS on success
 *  UGW_FAILURE on failure

 ==================================================================================================
 */

static int fapi_ssImgValidate(img_param_t img_auth){

	int nRet=UGW_SUCCESS;
	char s_buf[TEMP_BUF] = {0};
	int n_len = 0;
	int32_t n_size = 0;
	uint32_t un_crc = 0;
	image_header_t img_header;
	unsigned char *header = NULL;
	uint32_t file_read_size =0, total_file_read_size = 0;
	uint32_t cur_par_size=0, pad;
#ifdef SECURE_BOOT_AUTH_SUPPORT
	image_header_t xImg_h;
	void *tmp_ptr;
#endif

	lseek(img_auth.src_img_fd, 0, SEEK_SET);

	memset_s(&img_header, sizeof(image_header_t), 0x00,sizeof(image_header_t));

	n_size = read(img_auth.src_img_fd, &img_header, sizeof(image_header_t));
	if(n_size < 0){
		printf("Firmware image verification : failed %s\n",
				"Firmware Upgrade failed as the image does not contain valid header.\n\n");
		nRet = UGW_FAILURE;
		goto abort;
	}

	if (img_header.ih_magic != IH_MAGIC) {
		printf( "Firmware image verification : failed %s\n",
				"Firmware Upgrade failed as the image does not contain valid header.\n\n");
		nRet = UGW_FAILURE;
		goto abort;
	}

	un_crc = 0x00000000 ^ 0xffffffff;
	n_len = 0;
	while ((n_len = read(img_auth.src_img_fd, s_buf, sizeof(s_buf))) > 0) {
		un_crc = crc32(un_crc, s_buf, n_len);
		n_len = 0;
		memset_s(s_buf, sizeof(s_buf), 0,sizeof(s_buf));
	}
	un_crc ^= 0xffffffff;

	if (un_crc != img_header.ih_dcrc) {
		printf("Firmware image verification : failed %s [%u:%u]\n\n",
				"Firmware Upgrade failed as the image does not contain valid checksum.", un_crc, img_header.ih_dcrc);
		nRet = UGW_FAILURE;
		goto abort;
	}

	/* parse mkimage to fetch vendor related options */
	header =  img_auth.src_img_addr+sizeof(image_header_t);
	do {
		img_header = *((image_header_t *)header);

		if(img_header.ih_type == IH_TYPE_MULTI){
                        cur_par_size = sizeof(image_header_t) + 8;
                        total_file_read_size += cur_par_size;
                        header =  img_auth.src_img_addr + total_file_read_size;
                        continue;
                }

		cur_par_size = sizeof(image_header_t) + img_header.ih_size;
		pad = (16 - (cur_par_size % 16)) % 16;
		header =  img_auth.src_img_addr + total_file_read_size;
		file_read_size = cur_par_size + pad;

		switch(img_header.ih_type) {
			/* vendor realted options are added part of script image type. */
			case IH_TYPE_SCRIPT:
				cur_par_size = sizeof(image_header_t) + 8;
				header = header + cur_par_size;
				xvendor_t data;
				data = *((xvendor_t *)header);
				/* verify the model name */
				if((nRet = fapi_verifyModel(data.model)) == UGW_FAILURE)
					goto abort;
				/* add more vendor option part of device tree , samething can be adapted here */
				break;
		}
		total_file_read_size += file_read_size;
		header += file_read_size;
	} while(img_auth.src_img_len > total_file_read_size);

#ifdef SECURE_BOOT_AUTH_SUPPORT

	memcpy_s(&xImg_h, sizeof(image_header_t), img_auth.src_img_addr, sizeof(image_header_t));

	tmp_ptr = img_auth.src_img_addr;
	//skip mkimage header
	img_auth.src_img_addr = img_auth.src_img_addr+sizeof(image_header_t);
	img_auth.src_img_len = xImg_h.ih_size;

	if(xImg_h.ih_type == IH_TYPE_KERNEL) {
		nRet = fapi_ssSecImgAuth(img_auth);
	} else {
		LOGF_LOG_ALERT("Not supposed to update this image type ; only kernel type is allowed\n");
		nRet = UGW_FAILURE;
		img_auth.src_img_addr = tmp_ptr;
		goto abort;
	}
	img_auth.src_img_addr = tmp_ptr;
#endif

	nRet = fapi_ssFlashWearOutChk();
	if(nRet == UGW_FAILURE)
		goto abort;

abort:
	return nRet;
}

/**===============================================================================================
 * @brief  validate the state input
 * 
 * @param pcState
 * upgrade state variable
 * 
 * @return 
 *  current state enum value on success
 *  UGW_FAILURE on failure

 ==================================================================================================
 */
static State_t fapi_ssValidateState(unsigned char *pcState)
{
	int nCnt=0;
	State_t eRes=UPG_NONE;

	for (nCnt=0; nCnt < MAX_STATE_CNT; ++nCnt,++eRes){
		if (0==strcmp((char *)pcState, (char *)sState[nCnt]))
			return eRes;
	}
	return MAX_STATE_CNT;
}

/**===============================================================================================
 * @brief  get upgrade status 
 * 
 * @param pcState
 * upgrade state variable
 * 
 * @return 
 *  current state on success
 *  UGW_FAILURE on failure

 ==================================================================================================
 */
int fapi_ssGetUpgState(unsigned char **pcState){
	int nFd = -1,nRet=UGW_SUCCESS,nEn=-1;
	char sBuf[MAX_WRITE]={0};
	struct flock fl = {};

	if((nFd = fapi_Fileopen(UPG_STATE_FILE, O_RDONLY,0))<0) {
		LOGF_LOG_ERROR("open failed for %s\n", UPG_STATE_FILE);
		return UGW_FAILURE;	
	}

	fl.l_type = F_RDLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;

	if (fcntl(nFd, F_SETLKW, &fl) == -1) {
		LOGF_LOG_ERROR("file lock failed\n");
		nRet = UGW_FAILURE;
		goto finish;
	}

	if(read(nFd,sBuf,MAX_WRITE-1)<0){
		LOGF_LOG_ERROR("read state variable failed\n");
		nRet=UGW_FAILURE;
		goto next;
	}


	nEn = atoi(sBuf);

	if(nEn == UPG_NONE)
		*pcState = sState[UPG_NONE];
	else if(nEn == UPG_REQ)
		*pcState = sState[UPG_REQ];
	else if(nEn == UPG_INPROG)
		*pcState = sState[UPG_INPROG];
	else if(nEn == UPG_SUCC)
		*pcState = sState[UPG_SUCC];
	else if(nEn == UPG_FAIL)
		*pcState = sState[UPG_FAIL];
	else 
		*pcState = 0;

next:
	fl.l_type = F_UNLCK;

	if (fcntl(nFd, F_SETLK, &fl) == -1) {
		LOGF_LOG_ERROR("file unlock failed\n");
	}

finish:
	if(nFd>=0)
		close(nFd);

	return nRet;
}



/**===============================================================================================
 * @brief  set upgrade status 
 * 
 * @param pcState
 * upgrade state variable
 * 
 * @return 
 *  current state on success
 *  UGW_FAILURE on failure

 ==================================================================================================
 */
int fapi_ssSetUpgState(unsigned char *pcState){
	int nFd = -1,nRet=UGW_SUCCESS;
	State_t eRes;
	char sBuf[MAX_WRITE]={0};
	struct flock fl = {};

	if((eRes=fapi_ssValidateState(pcState)) == MAX_STATE_CNT){
		LOGF_LOG_ERROR("invalid state value(%s) set request\n", pcState);
		return UGW_FAILURE;
	}

	if((nFd = fapi_Fileopen(UPG_STATE_FILE, O_RDWR, U_RDWR_G_RD))<0) {
		LOGF_LOG_ERROR("open failed for %s\n", UPG_STATE_FILE);
		return UGW_FAILURE;
	}

	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;

	if (fcntl(nFd, F_SETLKW, &fl) == -1) {
		LOGF_LOG_ERROR("file lock failed\n");
		nRet = UGW_FAILURE;
		goto finish;
	}

	sprintf_s(sBuf, MAX_WRITE, "%d", eRes);
	if(write(nFd,sBuf,MAX_WRITE)<0){
		LOGF_LOG_ERROR("updating state variable failed\n");
		nRet = UGW_FAILURE;
	}

	fl.l_type = F_UNLCK;

	if (fcntl(nFd, F_SETLK, &fl) == -1) {
		LOGF_LOG_ERROR("file unlock failed\n");
		nRet = UGW_FAILURE;
	}

finish:
	if(nFd>=0)
		close(nFd);

	return nRet;
}

/**===============================================================================================
 * @brief  Securely validate the image.
 * 
 * @param image_auth
 * structure to validate the image
 * 
 * @return 
 *  UGW_SUCCESS on success
 *  UGW_FAILURE on failure

 ==================================================================================================
 */

int fapi_ssImgAuth(img_param_t image_auth){

	int nRet=UGW_SUCCESS;
	unsigned char *pcState=NULL;

	int nLockFd=-1;

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

	if(image_auth.src_img_fd < 0){
		nRet=ERR_BAD_FD;
		goto finish;
	}

	if( (nRet = fapi_ssGetUpgState(&pcState)) == UGW_FAILURE){
		LOGF_LOG_ERROR("State variable retrieval failed\n");
		nRet=UGW_FAILURE;
		goto finish;
	}

	if(pcState == NULL){
		nRet=UGW_FAILURE;
		goto finish;
	} else if ( (strcmp((char *)pcState, (char *)sState[UPG_REQ]) == 0) || (strcmp((char *)pcState, (char *)sState[UPG_INPROG]) == 0) ){
		LOGF_LOG_ERROR("Upgrade in progress, wait until previous upgrade completes\n");
		nRet=UGW_FAILURE;
		goto finish;
	}else{
		nRet = fapi_ssSetUpgState(sState[UPG_REQ]);
		if(nRet == UGW_FAILURE)
			goto finish;

		/* perform validation 
		   1. image header valiation
		   2. secure image valiation on 4kec side
		   3. flash wear-out validation  */

		nRet = fapi_ssImgValidate(image_auth);

		if(nRet == UGW_SUCCESS){
			if(fapi_ssSetUpgState(sState[UPG_INPROG]) == UGW_FAILURE){
				LOGF_LOG_ERROR("state variable success case update failed\n");
				nRet = UGW_FAILURE;
			}
		}else{
			if(fapi_ssSetUpgState(sState[UPG_FAIL]) == UGW_FAILURE){
				LOGF_LOG_ERROR("state variable failure case update failed\n");
				nRet = UGW_FAILURE;
			}
		}
	}

finish:
	if(flock(nLockFd, LOCK_UN) < 0){
		LOGF_LOG_DEBUG("unlock failed with reason [%s]\n", strerror(errno));
	}

	if(close(nLockFd) < 0){
		LOGF_LOG_DEBUG("close failed with reason [%s]\n", strerror(errno));
	}
	return nRet;
}
