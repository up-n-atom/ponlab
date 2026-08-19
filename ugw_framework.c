/********************************************************************************

  Copyright © 2020-2024 MaxLinear, Inc.
  Copyright (C) 2019 Intel Corporation
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ********************************************************************************/

/* ****************************************************************************** 
 *         File Name    : ugw_framework.c                                       *
 *         Description  : helper Library , which contains functions,defines,	*
 *			  structs,enums used across the modules like CAL,CAPI,	*
 *			  CSD,Servd,and Management Entities			*
 * ******************************************************************************/

#define _GNU_SOURCE
#include <stdio.h>
#include <sys/socket.h>
#include <sys/msg.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <errno.h>
#include "safe_str_lib.h"
#include "safe_lib.h"
#include "safe_mem_lib.h"
#include <sys/types.h>
#include <grp.h>
#include <sys/stat.h>
#include <libubus.h>
#include <libubox/blobmsg_json.h>
#include <libubox/blobmsg.h>
#include "ugw_proto.h"
#include <ugw_structs.h>
#include <ugw_defs.h>
#include <ugw_enums.h>
#include <ugw_objmsg.h>

static int nObjParamCount=0;
static int nMsgHeaderCount=0;

#define MAX_LEN 64

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

/*  =============================================================================
 *   Function name 	: set_libugwhelper_loglevel				*
 *   Description 	: Function to set loglevel of this library		*
 *  ============================================================================*/
void set_libugwhelper_loglevel(uint16_t unLevel, uint16_t unType)
{
	LOGLEVEL = unLevel;
	LOGTYPE = unType;
}

/*  =============================================================================
 *   Function Name 	: help_getTimeMsec					*
 *   Description 	: getting time in sec 					*
 *  ============================================================================*/
int64_t help_getTimeMsec(void)
{
	struct timespec ts;
	int64_t val;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	//clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
	val = (int64_t) ts.tv_sec * 1000LL;
	val += ts.tv_nsec / 1000000LL;
	return val;
}

/*  =============================================================================
 *   Function Name 	: help_timeValDiff					*
 *   Description 	: Function to measure time diffrence in milliseconds.	*
 *  ============================================================================*/
long help_getDiffTime(IN struct timeval *starttime, IN struct timeval *finishtime)
{
	long msec;
	msec=(finishtime->tv_sec-starttime->tv_sec)*1000;
	msec+=(finishtime->tv_usec-starttime->tv_usec)/1000;
	return msec;
}

/*  =============================================================================
 *   Function Name 	: help_RetStBlobMsg					*
 *   Description 	: Function to construct common response msg to the	*
 *			  caller						*
 *  ============================================================================*/
void help_RetStBlobMsg(OUT struct blob_buf *buf,IN int nStatus)
{
	blobmsg_buf_init(buf);
	if (nStatus)
	{
		blobmsg_add_string(buf, "ReturnStatus", "Success");
	}
	else
	{
		blobmsg_add_string(buf, "ReturnStatus", "Failure");
	}
}
/*  =============================================================================
 *   Function Name 	: help_fillTableACSAttr					*
 *   Description 	: Function to construct objlist				*
 *  ============================================================================*/
static int help_fillTableACSAttr(struct blob_attr *head, unsigned int len,MsgHeader *pxMsgObject,const char *pcObjName)
{

	char sObjName[MAX_LEN_OBJNAME]={0};
	char sParamName[MAX_LEN_PARAM_NAME]={0},sParamValue[MAX_LEN_PARAM_VALUE]={0};
	uint32_t unParamFlag=0;
	uint32_t unObjOper=0,unObjFlag=0;	
	struct blob_attr *attr = NULL;
	struct blobmsg_hdr *hdr;
	int nType;
	ObjACSList *pxObj=NULL;
	char *pcTmp=NULL;

	__blob_for_each_attr(attr, head, len) 
	{
		hdr = blob_data(attr);
		nType = blobmsg_type(attr);
		switch(nType) 
		{
			case BLOBMSG_TYPE_TABLE :
				if (strcmp((char *)hdr->name,"Mgh") == 0)
				{
					/* Reset required to parse new msg */
					nMsgHeaderCount=1;
					nObjParamCount=0;
					help_fillTableACSAttr(blobmsg_data(attr), blobmsg_data_len(attr), pxMsgObject, NULL);
				}
				else
				{
					/* Reset ObjList */
					nObjParamCount=0;
					help_fillTableACSAttr(blobmsg_data(attr), blobmsg_data_len(attr), pxMsgObject,(char *)hdr->name);
				}
				break;	
			case BLOBMSG_TYPE_STRING:
				pcTmp = blobmsg_get_string(attr);
				if (pcTmp != NULL)
				{	
					if (nObjParamCount == 2)
					{
						memset(&sParamName[0], 0, sizeof(sParamName));
						if (sprintf_s(sParamName, MAX_LEN_PARAM_NAME, "%s", pcTmp)<=0) {
							LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
							return UGW_FAILURE;
						}
						nObjParamCount++;
					}
					else if (nObjParamCount == 3)
					{
						memset(&sParamValue[0], 0, sizeof(sParamValue));
						if (sprintf_s(sParamValue, MAX_LEN_PARAM_VALUE, "%s", pcTmp)<=0) {
							// value can be empty in such case, sprintf_s will fail, ignore case 
							LOGF_LOG_DEBUG("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
						}
						nObjParamCount++;
					}
				}
				break;
			case BLOBMSG_TYPE_INT8:
				printf("%d\n", blobmsg_get_u8(attr));
				break;
			case BLOBMSG_TYPE_INT16:
				break;

			case BLOBMSG_TYPE_INT32:
				if (nObjParamCount == 0)
				{
					if (nMsgHeaderCount == 1)
					{
						pxMsgObject->unMainOper = blobmsg_get_u32(attr);
						nMsgHeaderCount++;
					}
					else if (nMsgHeaderCount == 2)
					{
						pxMsgObject->unSubOper = blobmsg_get_u32(attr);
						nMsgHeaderCount++;
					}
					else if (nMsgHeaderCount == 3)
					{
						pxMsgObject->unOwner = blobmsg_get_u32(attr);
						nMsgHeaderCount++;
					}
					else if (nMsgHeaderCount == 4)
					{
						/*reserved flag not used as of now */
						pxMsgObject->nFlag = blobmsg_get_u32(attr);
						nMsgHeaderCount++;
					}
				}
				if (nObjParamCount == 0 && nMsgHeaderCount == 5)
				{
					unObjOper = blobmsg_get_u32(attr);
					nObjParamCount++;
				}
				else if (nObjParamCount == 1)
				{
					if(pcObjName != NULL)
					{
						unObjFlag = blobmsg_get_u32(attr);
						if (sprintf_s(sObjName, MAX_LEN_OBJNAME, "%s", pcObjName)<=0) {
							LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
							return UGW_FAILURE;
						}
						pxObj=help_addAcsObjList(pxMsgObject->pObjType, sObjName, unObjOper, unObjFlag);
						nObjParamCount++;
					}
				}
				else if (nObjParamCount == 4)
				{
					unParamFlag = blobmsg_get_u32(attr);
					help_addAcsParamList(pxObj, sParamName, sParamValue, unParamFlag);
					/* Reset ParamList */
					nObjParamCount = 2; 
				}
				else if (strcmp("MsgSize", (char *)hdr->name) == 0)
				{
					pxMsgObject->unMsgSize += blobmsg_get_u32(attr);
					/* Msgsize is last msg in blob hence reseting msg counters. */ 
					nMsgHeaderCount=0;
					nObjParamCount=0;
				}
				break;
			case BLOBMSG_TYPE_INT64:
				printf("%"PRIu64"\n", blobmsg_get_u64(attr));
				break;
		}
	}
	return UGW_SUCCESS;
}

/*  =============================================================================
 *   Function Name 	: help_fillTableAttr					*
 *   Description 	: Function to construct objlist				*
 *  ============================================================================*/
static int help_fillTableAttr(struct blob_attr *head, unsigned int len,MsgHeader *pxMsgObject,const char *pcObjName)
{
	char sObjName[MAX_LEN_OBJNAME],sWebName[MAX_LEN_WEBNAME];
	uint32_t unObjAttr;

	char sParamName[MAX_LEN_PARAM_NAME],sParamProfile[MAX_LEN_PROFILENAME],sParamValue[MAX_LEN_PARAM_VALUE],
	     sParamWebName[MAX_LEN_WEBNAME],sParamValidVal[MAX_LEN_VALID_VALUE];
	uint32_t unMinVal=0,unMaxVal=0,unMinLen=0,unMaxLen=0,unParamFlag=0;

	struct blob_attr *attr = NULL;
	struct blobmsg_hdr *hdr;
	int nType;
	char *pcTmp=NULL;

	ObjAttrList *pxObj=NULL;

	__blob_for_each_attr(attr, head, len) 
	{
		hdr = blob_data(attr);
		nType = blobmsg_type(attr);
		switch(nType) 
		{
			case BLOBMSG_TYPE_TABLE :
				if (strcmp((char *)hdr->name,"Mgh") == 0)
				{
					/* Reset required to parse new msg */
					nMsgHeaderCount=1;
					nObjParamCount=0;
					help_fillTableAttr(blobmsg_data(attr), blobmsg_data_len(attr),pxMsgObject,NULL);
				}
				else
				{
					/* Reset ObjList */
					nObjParamCount=0;
					help_fillTableAttr(blobmsg_data(attr), blobmsg_data_len(attr),pxMsgObject,(char *)hdr->name);
				}
				break;	
			case BLOBMSG_TYPE_STRING:
				pcTmp = blobmsg_get_string(attr);
				if (pcTmp != NULL)
				{
					if ( nObjParamCount == 0)
					{
						memset(&sWebName[0], 0x0, sizeof(sWebName));
						if (sprintf_s(sWebName, MAX_LEN_WEBNAME, "%s", pcTmp)<=0) {
							LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
							return UGW_FAILURE;
						}
						nObjParamCount++;
					}
					else if (nObjParamCount == 2)
					{
						memset(&sParamName[0], 0x0, sizeof(sParamName));
						if (sprintf_s(sParamName, MAX_LEN_PARAM_NAME,  "%s", pcTmp)<=0) {
							LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
							return UGW_FAILURE;
						}
						nObjParamCount++;
					}
					else if (nObjParamCount == 3)
					{
						memset(&sParamProfile[0], 0, sizeof(sParamProfile));
						if (sprintf_s(sParamProfile, MAX_LEN_PROFILENAME, "%s", pcTmp)<=0) {
							LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
							return UGW_FAILURE;
						}
						nObjParamCount++;
					}
					else if (nObjParamCount == 4)
					{
						memset(&sParamWebName[0], 0, sizeof(sParamWebName));
						if (sprintf_s(sParamWebName, MAX_LEN_WEBNAME, "%s", pcTmp)<=0) {
							LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
							return UGW_FAILURE;
						}
						nObjParamCount++;
					}
					else if (nObjParamCount == 5)
					{
						memset(&sParamValidVal[0], 0, sizeof(sParamValidVal));
						if (sprintf_s(sParamValidVal, MAX_LEN_VALID_VALUE, "%s", pcTmp)<=0) {
							LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
							return UGW_FAILURE;
						}
						nObjParamCount++;
					}
					else if (nObjParamCount == 6)
					{
						memset(&sParamValue[0], 0, sizeof(sParamValue));
						if (sprintf_s(sParamValue, MAX_LEN_PARAM_VALUE, "%s", pcTmp)<=0) {
							LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
							// value can be empty in such case , sprintf_s will fail, ignore case 
						}
						nObjParamCount++;
					}
				}
				break;
			case BLOBMSG_TYPE_INT8:
				printf("%d\n", blobmsg_get_u8(attr));
				break;
			case BLOBMSG_TYPE_INT16:
				printf("%d\n", blobmsg_get_u16(attr));
				break;
			case BLOBMSG_TYPE_INT32:
				if (nObjParamCount == 0)
				{
					if (nMsgHeaderCount == 1)
					{
						pxMsgObject->unMainOper = blobmsg_get_u32(attr);
						nMsgHeaderCount++;
					}
					else if (nMsgHeaderCount == 2)
					{
						pxMsgObject->unSubOper = blobmsg_get_u32(attr);
						nMsgHeaderCount++;
					}
					else if (nMsgHeaderCount == 3)
					{
						pxMsgObject->unOwner = blobmsg_get_u32(attr);
						nMsgHeaderCount++;
					}
					else if (nMsgHeaderCount == 4)
					{
						/*reserved flag not used as of now */
						pxMsgObject->nFlag = blobmsg_get_u32(attr);
						nMsgHeaderCount++;
					}
				}
				if (nObjParamCount == 1)
				{
					unObjAttr = blobmsg_get_u32(attr);
					if (sprintf_s(sObjName, MAX_LEN_OBJNAME, "%s", pcObjName)<=0) {
						LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
						return UGW_FAILURE;
					}
					pxObj=help_addObjAttrList(pxMsgObject->pObjType,sObjName,sWebName,unObjAttr);
					nObjParamCount++;
				}
				if (nObjParamCount == 7)
				{
					unMinVal = blobmsg_get_u32(attr);
					nObjParamCount++;
				}
				else if (nObjParamCount == 8)
				{
					unMaxVal = blobmsg_get_u32(attr);
					nObjParamCount++;
				}
				else if (nObjParamCount == 9)
				{
					unMinLen = blobmsg_get_u32(attr);
					nObjParamCount++;
				}
				else if (nObjParamCount == 10)
				{
					unMaxLen = blobmsg_get_u32(attr);
					nObjParamCount++;
				}
				else if (nObjParamCount == 11)
				{
					unParamFlag = blobmsg_get_u32(attr);
					help_addParamAttrList(pxObj,sParamName,sParamProfile,sParamWebName,sParamValidVal,
							sParamValue,unMinVal,unMaxVal,unMinLen,unMaxLen,unParamFlag);
					/* Reset ParamList */
					nObjParamCount = 2; 
				}
				else if (strcmp("MsgSize", (char *)hdr->name) == 0)
				{
					pxMsgObject->unMsgSize += blobmsg_get_u32(attr);
					/* Msgsize is last msg in blob hence reseting msg counters. */ 
					nMsgHeaderCount=0;
					nObjParamCount=0;
				}
				break;
			case BLOBMSG_TYPE_INT64:
				printf("%"PRIu64"\n", blobmsg_get_u64(attr));
				break;
		}
	}
	return UGW_SUCCESS;
}
/*  =============================================================================
 *   Function Name 	: help_fillTableValue					*
 *   Description 	: Function to construct objlist				*
 *  ============================================================================*/
static int help_fillTableValue(struct blob_attr *head, unsigned int len,MsgHeader *pxObjList,const char *pcObjName)
{
	char sObjName[MAX_LEN_OBJNAME];
	uint16_t unOid=0,unSid=0,unParamId=0;
	char sParamName[MAX_LEN_PARAM_NAME]={0},sParamValue[MAX_LEN_PARAM_VALUE]={0};
	uint32_t unParamFlag=0;
	uint32_t unObjOper=0,unObjFlag=0;	
	struct blob_attr *attr=NULL;
	struct blobmsg_hdr *hdr=NULL;
	int nType=0;
	ObjList *pxObj=NULL;
	char *pcTmp=NULL;


	__blob_for_each_attr(attr, head, len) 
	{
		hdr = blob_data(attr);
		nType = blobmsg_type(attr);
		switch(nType) 
		{
			case BLOBMSG_TYPE_TABLE :
				if (strcmp((char *)hdr->name,"Mgh") == 0)
				{
					/* Reset required to parse new msg */
					nMsgHeaderCount=1;
					nObjParamCount=0;
					help_fillTableValue(blobmsg_data(attr), blobmsg_data_len(attr),pxObjList,NULL);
				}
				else
				{
					/* Reset ObjList */
					nObjParamCount=0;
					help_fillTableValue(blobmsg_data(attr), blobmsg_data_len(attr),pxObjList,(char *)hdr->name);
				}
				break;	
			case BLOBMSG_TYPE_STRING:	
				pcTmp = blobmsg_get_string(attr);
				if (pcTmp != NULL)
				{
					if (nObjParamCount == 4)
					{
						sParamName[0]='\0';
						if (sprintf_s(sParamName, MAX_LEN_PARAM_NAME, "%s", pcTmp)<=0) {
							LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
							return UGW_FAILURE;
						}
						nObjParamCount++;
					}
					if (nObjParamCount == 6)
					{
						sParamValue[0]='\0';
						if (sprintf_s(sParamValue, MAX_LEN_PARAM_VALUE, "%s", pcTmp)<=0) {
							LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
							// value can be empty in such case,  sprintf_s will fail, ignore case 
						}
						nObjParamCount++;
					}
				}
				break;
			case BLOBMSG_TYPE_INT8:
				printf("%d\n", blobmsg_get_u8(attr));
				break;
			case BLOBMSG_TYPE_INT16:
				if ( nObjParamCount == 0)
				{
					unSid = blobmsg_get_u16(attr);
					nObjParamCount++;
				}
				else if (nObjParamCount == 1)
				{
					unOid = blobmsg_get_u16(attr);
					nObjParamCount++;
				}
				else if (nObjParamCount == 5)
				{
					unParamId = blobmsg_get_u16(attr);
					nObjParamCount++;
				}
				break;

			case BLOBMSG_TYPE_INT32:
				if (nObjParamCount == 0)
				{
					if (nMsgHeaderCount == 1)
					{
						pxObjList->unMainOper = blobmsg_get_u32(attr);
						nMsgHeaderCount++;
					}
					else if (nMsgHeaderCount == 2)
					{
						pxObjList->unSubOper = blobmsg_get_u32(attr);
						nMsgHeaderCount++;
					}
					else if (nMsgHeaderCount == 3)
					{
						pxObjList->unOwner = blobmsg_get_u32(attr);
						nMsgHeaderCount++;
					}
					else if (nMsgHeaderCount == 4)
					{
						/*reserved flag not used as of now */
						pxObjList->nFlag = blobmsg_get_u32(attr);
						nMsgHeaderCount++;
					}
				}
				if (nObjParamCount == 2)
				{
					unObjOper = blobmsg_get_u32(attr);
					nObjParamCount++;
				}
				else if (nObjParamCount == 3)
				{
					unObjFlag = blobmsg_get_u32(attr);
					if (sprintf_s(sObjName, MAX_LEN_OBJNAME, "%s", pcObjName)<=0) {
						LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
						return UGW_FAILURE;
					}
					pxObj=help_addObjList(pxObjList->pObjType,sObjName,unSid,unOid,unObjOper,unObjFlag);
					nObjParamCount++;
				}
				else if (nObjParamCount == 7)
				{
					unParamFlag = blobmsg_get_u32(attr);
					help_addParamList(pxObj,sParamName,unParamId,sParamValue,unParamFlag);
					/* Reset ParamList */
					nObjParamCount = 4; 
				}
				else if (strcmp("MsgSize", (char *)hdr->name) == 0)
				{
					pxObjList->unMsgSize += blobmsg_get_u32(attr);
					/* Msgsize is last msg in blob hence reseting msg counters. */ 
					nMsgHeaderCount=0;
					nObjParamCount=0;
				}
				break;
			case BLOBMSG_TYPE_INT64:
				printf("%"PRIu64"\n", blobmsg_get_u64(attr));
				break;
		}
	}
	return UGW_SUCCESS;
}

/*  =============================================================================
 *   Function Name 	: help_blobToDataModStat				*
 *   Description 	: Function to convert to blob to struct 		*
 *  ============================================================================*/
int help_blobToDataModStat(struct blob_attr *head, DataModelStat *pxDataModStat)
{
	struct blob_attr *attr = NULL;
	struct blob_attr *data = NULL;
	int nType;
	unsigned int len;
	struct blobmsg_hdr *hdr;

	len = blobmsg_data_len(head); 
	data = blobmsg_data(head);

	__blob_for_each_attr(attr, data, len) 
	{
		hdr = blob_data(attr);
		nType = blobmsg_type(attr);
		switch(nType) 
		{
			case BLOBMSG_TYPE_INT32:
				if (strcmp("ObjCnt", (char *)hdr->name) == 0)
					pxDataModStat->nObjCnt=blobmsg_get_u32(attr);
				else if (strcmp("PramCnt", (char *)hdr->name) == 0) 
					pxDataModStat->nPramCnt=blobmsg_get_u32(attr);
		}
	}
	/* when msg is empty */
	return UGW_SUCCESS;
}


/*  =============================================================================
 *   Function Name 	: help_getMsgType					*
 *   Description 	: Function to type of object list 			*
 *  ============================================================================*/
int help_getMsgType(struct blob_attr *head,unsigned int len)
{
	struct blob_attr *attr = NULL;
	int nType;

	__blob_for_each_attr(attr, head, len) 
	{
		nType = blobmsg_type(attr);
		switch(nType) 
		{
			case BLOBMSG_TYPE_INT32:
				return blobmsg_get_u32(attr);

		}
	}
	/* when msg is empty */
	return UGW_SUCCESS;
}
/*  =============================================================================
 *   Function Name 	: help_blobToObjList					*
 *   Description 	: Function to extract the blob msg and fill the caller 	*
 *   			  struct.						*
 *  ============================================================================*/
int help_blobToObjList(IN struct blob_attr *msg,OUT MsgHeader *pxMsg)
{
	int nRet = UGW_SUCCESS;

	if ( IS_OBJLIST(pxMsg->unSubOper))
	{
		nRet = help_fillTableValue(blobmsg_data(msg), blobmsg_data_len(msg),pxMsg,NULL);

	}
	else if (IS_SOPT_OBJATTR(pxMsg->unSubOper))
	{
		nRet = help_fillTableAttr(blobmsg_data(msg), blobmsg_data_len(msg),pxMsg,NULL);
	}

	else if (IS_SOPT_OBJACSATTR(pxMsg->unSubOper))
	{
		nRet = help_fillTableACSAttr(blobmsg_data(msg), blobmsg_data_len(msg),pxMsg,NULL);
	}

	nObjParamCount=0;
	nMsgHeaderCount=0;
	return nRet;

}
/*  =============================================================================
 *   Function Name 	: help_blobToNotifyList					*
 *   Description 	: Function to blob msg to notify list	 		*
 *  ============================================================================*/
int help_blobToNotifyList(struct blob_attr *msg,NotifyObjData *pxNotify)
{
	struct blob_attr *attr = NULL;
	struct blob_attr *head;
	int nType;
	unsigned int len=0;
	uint32_t unParamCnt=0;
	char *pxTmp = NULL;
	char sParamName[MAX_LEN_PARAM_NAME]={0},sParamValue[MAX_LEN_PARAM_VALUE]={0};

	void *pParam;

	head = blobmsg_data(msg);
	len = blobmsg_data_len(msg);

	pParam = HELP_CREATE_PARAM(SOPT_OBJVALUE);	

	__blob_for_each_attr(attr, head, len) 
	{
		nType = blobmsg_type(attr);
		switch(nType) 
		{
			case BLOBMSG_TYPE_INT32:
				pxNotify->unNotifyId = blobmsg_get_u32(attr);
				break;
			case BLOBMSG_TYPE_INT8:
				pxNotify->ucType = blobmsg_get_u8(attr);
				break;
			case BLOBMSG_TYPE_STRING:
				pxTmp = blobmsg_get_string(attr);
				if (pxTmp != NULL)
				{
					if (unParamCnt == 0)
					{
						sParamName[0]='\0';
						if (strnlen_s(pxTmp, MAX_LEN_PARAM_NAME) > 0) {
							if (sprintf_s(sParamName, MAX_LEN_PARAM_NAME, "%s", pxTmp)<=0) {
								LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
								HELP_DELETE_PARAM(pParam, SOPT_OBJVALUE);
								return UGW_FAILURE;
							}
						}
						unParamCnt++;
					}
					else
					{
						sParamValue[0]='\0';
						if (strnlen_s(pxTmp, MAX_LEN_PARAM_VALUE) > 0) {
							if (sprintf_s(sParamValue, MAX_LEN_PARAM_VALUE, "%s", pxTmp )<=0) {
								LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
								HELP_DELETE_PARAM(pParam, SOPT_OBJVALUE);
								return UGW_FAILURE;
							}
						}
						if (strnlen_s(sParamName,MAX_LEN_PARAM_NAME) > 0)
							HELP_ONLY_PARAM_SET(pParam,sParamName,sParamValue,SOPT_OBJVALUE);
						unParamCnt=0;
					}
				}
				break;
		}
	}

	pxNotify->pxParamList = pParam;
	return UGW_SUCCESS;
}

/*  =============================================================================
 *   Function Name 	: help_notifyListToBlob					*
 *   Description 	: Function to construct the blob msg 			*
 *  ============================================================================*/
int help_notifyListToBlob(OUT struct blob_buf *buf,IN NotifyObjData *pxNotify)
{
	ParamList *pxParam = NULL;
	blobmsg_buf_init(buf);

	blobmsg_add_u32(buf, "nid",pxNotify->unNotifyId);
	blobmsg_add_u8(buf, "type",pxNotify->ucType);
	FOR_EACH_PARAM_ONLY(pxNotify->pxParamList,pxParam) 
	{
		blobmsg_add_string(buf, "pn",pxParam->sParamName);
		blobmsg_add_string(buf, "pv", pxParam->sParamValue);
	}
	return UGW_SUCCESS;
}
/*  =============================================================================
 *   Function Name 	: help_sendMsgToServer					*
 *   Description 	: Function to send get/set request msg to ubus server   *
 * 			  get appropriate response msg				* 
 *  ============================================================================*/
int help_sendMsgToServer(IN MsgHeader *pxReqMsg, IN ubus_data_handler_t fnCallBak, IN const char *pcServerName, 
		IN const char *pcOper, IN int nTimeOut, OUT MsgHeader *pxResMsgHeader)
{

	struct blob_buf buf;
	uint32_t unMsgSize=0;
	unsigned int unId=0;
	struct ubus_context *pxCtx=NULL;
	void *ptbl=NULL;
	int8_t  nRetryCnt = 0;

	int32_t nRet = UGW_SUCCESS,unInvCnt=0;


	memset(&buf, 0x0 , sizeof(struct blob_buf));

	blobmsg_buf_init(&buf);

Retry:
	pxCtx = help_ubusConnect();
	if ( pxCtx == NULL ) {
		LOGF_LOG_CRITICAL("ubus connect failed (attempt %d) \n", (int) nRetryCnt);
		nRet = UGW_FAILURE;
		//TODO
		sleep(1);
		if (nRetryCnt < 3)
		{
			nRetryCnt++;
			goto Retry;
		}
		else
		{
			LOGF_LOG_CRITICAL("ubus connect attempts exceeded retry limit \n");
			goto finish;
		}
	}
	LOGF_LOG_DEBUG("ubus_connect SUCCESS (attempts left=%d)\n", nRetryCnt);
	nRet = UGW_SUCCESS;

	if (IS_THRED_ENV_SETL(pxReqMsg->nFlag)) {
		pxCtx->sock.registered = true;	
	}
	else {
		pxCtx->sock.registered = false;	
	}

	if (ubus_lookup_id(pxCtx, pcServerName, &unId)) {
		LOGF_LOG_CRITICAL("ubus look-up failed");
		nRet = ERR_NO_FRAMEWORK;
		goto finish;
	}

	blobmsg_add_u32(&buf,"objflag",pxReqMsg->unSubOper);
	ptbl = blobmsg_open_table(&buf,"Mgh");
	blobmsg_add_u32(&buf,"mo",pxReqMsg->unMainOper);
	blobmsg_add_u32(&buf,"so",pxReqMsg->unSubOper);
	blobmsg_add_u32(&buf,"ow",pxReqMsg->unOwner);
	blobmsg_add_u32(&buf,"rf",pxReqMsg->nFlag);
	blobmsg_close_table(&buf, ptbl);

	if (IS_OBJLIST(pxReqMsg->unSubOper))
	{
		ObjList *pxTmpObj = NULL;
		ParamList *pxParam = NULL;


		FOR_EACH_OBJ(pxReqMsg->pObjType,pxTmpObj)
		{
			if (buf.buflen >= UBUS_MSG_LENGTH) 
			{
				/* retransmission msg */
				unInvCnt++;
				LOGF_LOG_INFO("ubus_invoke Request count [%d]: unId=%d (%s), pcOper=%s, pxReqMsg=[unMainOper=%d, unSubOper=%d, unOwner=%d, nFlag=%d],requestMsgSize=%d\n",
						unInvCnt,unId, pcServerName, pcOper, pxReqMsg->unMainOper, pxReqMsg->unSubOper, pxReqMsg->unOwner, pxReqMsg->nFlag, unMsgSize);
				nRet = ubus_invoke(pxCtx, unId, pcOper , buf.head, fnCallBak, pxResMsgHeader, nTimeOut);
				if ( nRet == UBUS_STATUS_TIMEOUT )
				{
					LOGF_LOG_CRITICAL(" ubus_invoke(OBJLIST) failed with UBUS_STATUS_TIMEOUT \n");
					nRet = ERR_UBUS_TIME_OUT;
					goto finish;
				}
				else if (nRet != UBUS_STATUS_OK) {
					LOGF_LOG_ERROR(" ubus_invoke(OBJLIST) failed with %d \n", nRet);
				}
				LOGF_LOG_DEBUG("COMPLETED ubus_invoke: unId=%d (%s), pcOper=%s, pxReqMsg=[unMainOper=%d, unSubOper=%d, unOwner=%d, nFlag=%d]\n",
						unId, pcServerName, pcOper, pxReqMsg->unMainOper, pxReqMsg->unSubOper, pxReqMsg->unOwner, pxReqMsg->nFlag);

				unMsgSize += buf.buflen;
				blob_buf_free(&buf);
				memset(&buf,0x0,sizeof(struct blob_buf));
				blobmsg_buf_init(&buf);
				blobmsg_add_u32(&buf,"objflag",pxReqMsg->unSubOper);
				ptbl = blobmsg_open_table(&buf,"Mgh");
				blobmsg_add_u32(&buf,"mo",pxReqMsg->unMainOper);
				blobmsg_add_u32(&buf,"so",pxReqMsg->unSubOper);
				blobmsg_add_u32(&buf,"ow",pxReqMsg->unOwner);
				blobmsg_add_u32(&buf,"rf",pxReqMsg->nFlag);
				blobmsg_close_table(&buf, ptbl);
			}
			ptbl = blobmsg_open_table(&buf,pxTmpObj->sObjName);
			blobmsg_add_u16(&buf,"sid",pxTmpObj->unSid); 
			blobmsg_add_u16(&buf,"oid",pxTmpObj->unOid);
			blobmsg_add_u32(&buf,"oo",pxTmpObj->unObjOper);
			blobmsg_add_u32(&buf,"of",pxTmpObj->unObjFlag);
			FOR_EACH_PARAM(pxTmpObj, pxParam)
			{
				blobmsg_add_string(&buf, "pn",pxParam->sParamName);
				blobmsg_add_u16(&buf, "pi", pxParam->unParamId);
				blobmsg_add_string(&buf, "pv", pxParam->sParamValue);
				blobmsg_add_u32(&buf, "pf", pxParam->unParamFlag);
			}
			blobmsg_close_table(&buf, ptbl);
		}
	}
	else if (IS_SOPT_OBJATTR(pxReqMsg->unSubOper))
	{
		ObjAttrList *pxAttrObj = NULL;
		ParamAttrList *pxAttrParam = NULL;

		FOR_EACH_OBJATTR(pxReqMsg->pObjType,pxAttrObj)
		{
			if (buf.buflen >= UBUS_MSG_LENGTH) 
			{
				blobmsg_close_table(&buf, ptbl);
				LOGF_LOG_DEBUG("ubus_invoke: unId=%d (%s), pcOper=%s, pxReqMsg=[unMainOper=%d, unSubOper=%d, unOwner=%d, nFlag=%d]\n",
						unId, pcServerName, pcOper, pxReqMsg->unMainOper, pxReqMsg->unSubOper, pxReqMsg->unOwner, pxReqMsg->nFlag);
				nRet = ubus_invoke(pxCtx, unId, pcOper , buf.head, fnCallBak, pxResMsgHeader, nTimeOut);
				if ( nRet == UBUS_STATUS_TIMEOUT )
				{
					LOGF_LOG_CRITICAL(" ubus_invoke(OBJATTR) failed with UBUS_STATUS_TIMEOUT \n");
					nRet = ERR_UBUS_TIME_OUT;
					goto finish;
				}
				else if (nRet != UBUS_STATUS_OK) {
					LOGF_LOG_ERROR(" ubus_invoke(OBJATTR) failed with %d \n", nRet);
				}
				LOGF_LOG_DEBUG("COMPLETED ubus_invoke: unId=%d (%s), pcOper=%s, pxReqMsg=[unMainOper=%d, unSubOper=%d, unOwner=%d, nFlag=%d]\n",
						unId, pcServerName, pcOper, pxReqMsg->unMainOper, pxReqMsg->unSubOper, pxReqMsg->unOwner, pxReqMsg->nFlag);

				unMsgSize += buf.buflen;
				blob_buf_free(&buf);

				memset(&buf,0x0,sizeof(struct blob_buf));
				blobmsg_buf_init(&buf);
				blobmsg_add_u32(&buf,"objflag",pxReqMsg->unSubOper);
				ptbl = blobmsg_open_table(&buf,"Mgh");
				blobmsg_add_u32(&buf,"mo",pxReqMsg->unMainOper);
				blobmsg_add_u32(&buf,"so",pxReqMsg->unSubOper);
				blobmsg_add_u32(&buf,"ow",pxReqMsg->unOwner);
				blobmsg_add_u32(&buf,"rf",pxReqMsg->nFlag);
				blobmsg_close_table(&buf, ptbl);
			}	
			ptbl = blobmsg_open_table(&buf,GET_ATTR_OBJNAME(pxAttrObj));
			blobmsg_add_string(&buf,"wn",GET_ATTR_WEBNAME(pxAttrObj)); 
			blobmsg_add_u32(&buf,"fg",GET_ATTR_FLAG(pxAttrObj));
			FOR_EACH_PARAM_ATTR(pxAttrObj,pxAttrParam)
			{
				blobmsg_add_string(&buf, "pn",GET_ATTR_PARAMNAME(pxAttrParam));
				blobmsg_add_string(&buf, "pr", GET_ATTR_PARAMPROFILE(pxAttrParam));
				blobmsg_add_string(&buf, "pw", GET_ATTR_PARAMWEBNAME(pxAttrParam));
				blobmsg_add_string(&buf, "pvv", pxAttrParam->sParamValidVal);
				blobmsg_add_string(&buf, "pv", GET_ATTR_PARAMVALUE(pxAttrParam));
				blobmsg_add_u32(&buf, "pmiv", GET_ATTR_MINVAL(pxAttrParam));
				blobmsg_add_u32(&buf, "pmav", GET_ATTR_MAXVAL(pxAttrParam));
				blobmsg_add_u32(&buf, "pmil", GET_ATTR_MINLEN(pxAttrParam));
				blobmsg_add_u32(&buf, "pmal", GET_ATTR_MAXLEN(pxAttrParam));
				blobmsg_add_u32(&buf, "pf", GET_ATTR_PARAMFLAG(pxAttrParam));
			}
			blobmsg_close_table(&buf, ptbl);
		}
	}
	else if (IS_SOPT_OBJACSATTR(pxReqMsg->unSubOper))
	{
		ObjACSList *pxTmpObjAcs = NULL;
		ParamACSList *pxParamAcs = NULL;

		FOR_EACH_OBJ_ACS_ATTR(pxReqMsg->pObjType,pxTmpObjAcs)
		{
			if (buf.buflen >= UBUS_MSG_LENGTH) 
			{
				/* retransmission msg */
				LOGF_LOG_DEBUG("ubus_invoke: unId=%d (%s), pcOper=%s, pxReqMsg=[unMainOper=%d, unSubOper=%d, unOwner=%d, nFlag=%d]\n",
						unId, pcServerName, pcOper, pxReqMsg->unMainOper, pxReqMsg->unSubOper, pxReqMsg->unOwner, pxReqMsg->nFlag);
				nRet = ubus_invoke(pxCtx, unId, pcOper , buf.head, fnCallBak, pxResMsgHeader, nTimeOut);
				if ( nRet == UBUS_STATUS_TIMEOUT )
				{
					LOGF_LOG_CRITICAL(" ubus_invoke(OBJACSATTR) failed with UBUS_STATUS_TIMEOUT \n");
					nRet = ERR_UBUS_TIME_OUT;
					goto finish;
				}				
				else if (nRet != UBUS_STATUS_OK) {
					LOGF_LOG_ERROR(" ubus_invoke(OBJACSATTR) failed with %d \n", nRet);
				}
				LOGF_LOG_DEBUG("COMPLETED ubus_invoke: unId=%d (%s), pcOper=%s, pxReqMsg=[unMainOper=%d, unSubOper=%d, unOwner=%d, nFlag=%d]\n",
						unId, pcServerName, pcOper, pxReqMsg->unMainOper, pxReqMsg->unSubOper, pxReqMsg->unOwner, pxReqMsg->nFlag);

				unMsgSize += buf.buflen;
				blob_buf_free(&buf);
				memset(&buf,0x0,sizeof(struct blob_buf));
				blobmsg_buf_init(&buf);
				blobmsg_add_u32(&buf,"objflag",pxReqMsg->unSubOper);
				ptbl = blobmsg_open_table(&buf,"Mgh");
				blobmsg_add_u32(&buf,"mo",pxReqMsg->unMainOper);
				blobmsg_add_u32(&buf,"so",pxReqMsg->unSubOper);
				blobmsg_add_u32(&buf,"ow",pxReqMsg->unOwner);
				blobmsg_add_u32(&buf,"rf",pxReqMsg->nFlag);
				blobmsg_close_table(&buf, ptbl);
			}
			ptbl = blobmsg_open_table(&buf,pxTmpObjAcs->sObjName);
			blobmsg_add_u32(&buf,"oo",pxTmpObjAcs->unObjOper);
			blobmsg_add_u32(&buf,"of",pxTmpObjAcs->unObjFlag);
			FOR_EACH_PARAM_ACS_ATTR(pxTmpObjAcs, pxParamAcs)
			{
				blobmsg_add_string(&buf, "pn",pxParamAcs->sParamName);
				blobmsg_add_string(&buf, "pv", pxParamAcs->sParamValue);
				blobmsg_add_u32(&buf, "pf", pxParamAcs->unParamFlag);
			}
			blobmsg_close_table(&buf, ptbl);
		}

	}

	unMsgSize += buf.buflen;
	blobmsg_add_u32(&buf,"MsgSize",unMsgSize);
	LOGF_LOG_DEBUG("ubus_invoke: unMsgSize=%d, unId=%d (%s), pcOper=%s, pxReqMsg=[unMainOper=%d, unSubOper=%d, unOwner=%d, nFlag=%d]\n",
			unMsgSize, unId, pcServerName, pcOper, pxReqMsg->unMainOper, pxReqMsg->unSubOper, pxReqMsg->unOwner, pxReqMsg->nFlag);
	nRet = ubus_invoke(pxCtx, unId, pcOper , buf.head, fnCallBak, pxResMsgHeader, nTimeOut);
	if ( nRet == UBUS_STATUS_TIMEOUT )
	{
		LOGF_LOG_CRITICAL(" ubus_invoke(final) failed with UBUS_STATUS_TIMEOUT \n");
		nRet = ERR_UBUS_TIME_OUT;
		goto finish;
	}
	else if (nRet != UBUS_STATUS_OK) {
		LOGF_LOG_ERROR(" ubus_invoke(final) failed with %d \n", nRet);
	}
	LOGF_LOG_DEBUG("COMPLETED ubus_invoke: unMsgSize=%d, unId=%d (%s), pcOper=%s, pxReqMsg=[unMainOper=%d, unSubOper=%d, unOwner=%d, nFlag=%d]\n",
			unMsgSize, unId, pcServerName, pcOper, pxReqMsg->unMainOper, pxReqMsg->unSubOper, pxReqMsg->unOwner, pxReqMsg->nFlag);

finish:
	if (buf.buf != NULL)
		blob_buf_free(&buf);

	help_ubusDisconnect(pxCtx);

	return nRet;
}

/*  =============================================================================
 *   Function Name 	: help_copyIdentifierData 				*
 *   Description 	: Function to parse value				*
 *  ============================================================================*/
static void help_copyIdentifierData(const char *pcString, const char *pcToken, const char *pcNxtTok, char sBuf[],int nSize,size_t nBufSize)
{
	int nLen=0;
	const char *pcEndPtr,*pcTmp;
	


	pcEndPtr = strstr(pcString, pcToken);
	if (pcEndPtr) {
		pcEndPtr=pcEndPtr+nSize;
	}else{
		LOGF_LOG_CRITICAL("No token found,, parsing abort\n");
		return;
	}

	if (pcNxtTok != NULL) {
		pcTmp=strstr(pcEndPtr,pcNxtTok);
		if (pcTmp == NULL)
			return;

		nLen=(size_t)(pcTmp-pcEndPtr);
		if ((nLen+1)<(int)nBufSize) {
			if (strncpy_s(sBuf, nBufSize, pcEndPtr, nLen) != EOK) {
				LOGF_LOG_CRITICAL("strncpy failed\n");
				return;
			}
		}
	}else{
		nLen=strnlen_s(pcEndPtr,nBufSize);
		if ((nLen+1)<(int)nBufSize) {
			if (strncpy_s(sBuf, nBufSize, pcEndPtr, nLen) != EOK) {
				LOGF_LOG_CRITICAL("strncpy failed\n");
				return;
			}
		}
	}
}

/*  =============================================================================
 *   Function Name 	: help_retrieveRespData 				*
 *   Description 	: Function to store objlist to buffer(file,shm)		*
 *  ============================================================================*/
int help_retrieveRespData(IN const char *pcPath, OUT MsgHeader *pxMsg)
{
	FILE *fp=NULL;
	int unObjAdd=0,nRet=UGW_SUCCESS;
	char sBuf[MAX_LEN]={0};


	char* line = NULL;
	size_t len = 0;

	if (fopen_s(&fp, pcPath, "r") != EOK) {
		nRet = ERR_FILE_NOT_FOUND;
		goto finish;
	}

	if (fp == NULL) {
		nRet = ERR_FILE_NOT_FOUND;
		goto finish;
        }

	if (IS_OBJLIST(pxMsg->unSubOper)) {
		ObjList *pxTmp=NULL;
		while ( getline(&line, &len, fp) != -1 ) {
			if (strstr(line,"obj=") != NULL) {
				unObjAdd=0;
				char sObjName[MAX_LEN_OBJNAME]={0};
				uint32_t unObjFlag=0,unObjOper=0,unSid=0,unOid=0;

				help_copyIdentifierData(line, "obj=", "~sid=", sObjName,4,MAX_LEN_OBJNAME);
				help_copyIdentifierData(line, "sid=", "~oid=", sBuf,4,MAX_LEN);
				unSid=atoi(sBuf);
				help_copyIdentifierData(line, "oid=", "~oo=",sBuf,4,MAX_LEN);
				unOid=atoi(sBuf);
				help_copyIdentifierData(line, "oo=", "~of=",sBuf,3,MAX_LEN);
				unObjOper = strtol(sBuf, NULL, 16);
				help_copyIdentifierData(line, "of=", NULL, sBuf,3,MAX_LEN);
				unObjFlag = strtol(sBuf, NULL, 16);

				pxTmp=help_addObjList(pxMsg->pObjType, sObjName, unSid, unOid, unObjOper, unObjFlag);
				unObjAdd=1;

			} else if (strstr(line, "param=") != NULL) {
				char sParamName[MAX_LEN_PARAM_NAME]={0}, sParamValue[MAX_LEN_PARAM_VALUE]={0};
				uint32_t unParamFlag=0,unPid=0;

				help_copyIdentifierData(line, "param=", "~pi=", sParamName,6,MAX_LEN_PARAM_NAME);
				help_copyIdentifierData(line, "pi=", "~pv=", sBuf,3,MAX_LEN); 
				unPid=atoi(sBuf);
				help_copyIdentifierData(line, "pv=", "~pf=", sParamValue,3,MAX_LEN_PARAM_VALUE);
				help_copyIdentifierData(line, "pf=", NULL, sBuf,3,MAX_LEN);
				unParamFlag = strtol(sBuf,NULL,16);

				if (unObjAdd)
					help_addParamList(pxTmp, sParamName, unPid,sParamValue, unParamFlag);

			}else{
				LOGF_LOG_CRITICAL("Invalid option [%s] file may be corrupted\n",line);
			}
		}
	}else if (IS_SOPT_OBJATTR(pxMsg->unSubOper)) {
		ObjAttrList *pxTmpAttr=NULL;
		while ( getline(&line, &len, fp) != -1 ) {
			if (strstr(line,"obj=") != NULL) {
				unObjAdd=0;
				char sAttrObjName[MAX_LEN_OBJNAME]={0},sAttrWebName[MAX_LEN_WEBNAME]={0};
				uint32_t unAttrObjFlag=0;

				help_copyIdentifierData(line, "obj=", "~wn=", sAttrObjName,4,MAX_LEN_OBJNAME);
				help_copyIdentifierData(line, "wn=", "~of=", sAttrWebName ,3,MAX_LEN_WEBNAME);
				help_copyIdentifierData(line, "of=", NULL, sBuf ,3,MAX_LEN);
				unAttrObjFlag = strtol(sBuf, NULL, 16);

				pxTmpAttr=help_addObjAttrList(pxMsg->pObjType,sAttrObjName,sAttrWebName,unAttrObjFlag);
				unObjAdd=1;
			}else if (strstr(line, "param=") != NULL) {
				char sAttrParamName[MAX_LEN_PARAM_NAME]={0},sAttrParamPr[MAX_LEN_PROFILENAME]={0};
				char sAttrParamPw[MAX_LEN_WEBNAME]={0}, sAttrParamPvv[MAX_LEN_VALID_VALUE]={0};
				char sAttrParamPv[MAX_LEN_PARAM_VALUE]={0};
				uint32_t unParamMinv=0, unParamMaxv=0, unParamMinl=0, unParamMaxl=0, unAttrParamFlag=0;

				help_copyIdentifierData(line, "param=", "~pr=", sAttrParamName,6,MAX_LEN_PARAM_NAME);
				help_copyIdentifierData(line, "pr=", "~pw=", sAttrParamPr,3,MAX_LEN_PROFILENAME);
				help_copyIdentifierData(line, "pw=", "~pvv=", sAttrParamPw,3,MAX_LEN_WEBNAME);
				help_copyIdentifierData(line, "pvv=", "~pv=", sAttrParamPvv,4,MAX_LEN_VALID_VALUE);
				help_copyIdentifierData(line, "pv=", "~pmiv=", sAttrParamPv,3,MAX_LEN_PARAM_VALUE);
				help_copyIdentifierData(line, "pmiv=", "~pmav=", sBuf,5,MAX_LEN);
				unParamMinv=atoi(sBuf);
				help_copyIdentifierData(line, "pmav=", "~pmil=", sBuf,5,MAX_LEN);
				unParamMaxv=atoi(sBuf);
				help_copyIdentifierData(line, "pmil=", "~pmal=", sBuf,5,MAX_LEN);
				unParamMinl=atoi(sBuf);
				help_copyIdentifierData(line, "pmal=", "~pf=", sBuf,5,MAX_LEN);
				unParamMaxl=atoi(sBuf);
				help_copyIdentifierData(line, "pf=", NULL, sBuf,3,MAX_LEN);
				unAttrParamFlag = strtol(sBuf,NULL,16);

				if (unObjAdd)
					help_addParamAttrList(pxTmpAttr,sAttrParamName,sAttrParamPr,sAttrParamPw, sAttrParamPvv,sAttrParamPv,unParamMinv,unParamMaxv,unParamMinl,unParamMaxl,unAttrParamFlag);
			}else{
				LOGF_LOG_CRITICAL("Invalid option [%s] file may be corrupted\n",line);
			}
		}
	}else if (IS_SOPT_OBJACSATTR(pxMsg->unSubOper)) {
		ObjACSList *pxTmpAcs=NULL;
		while ( getline(&line, &len, fp) != -1 ) {
			if (strstr(line,"obj=") != NULL) {
				unObjAdd=0;
				char sAcsObjName[MAX_LEN_OBJNAME]={0};
				uint32_t unAcsObjFlag=0,unAcsObjOper=0;

				help_copyIdentifierData(line, "obj=", "~oo=", sAcsObjName,4,MAX_LEN_OBJNAME);
				help_copyIdentifierData(line, "oo=", "~of=", sBuf,3,MAX_LEN);
				unAcsObjOper = strtol(sBuf, NULL, 16);
				help_copyIdentifierData(line, "of=", NULL, sBuf,3,MAX_LEN);
				unAcsObjFlag = strtol(sBuf, NULL, 16);

				pxTmpAcs=help_addAcsObjList(pxMsg->pObjType, sAcsObjName,unAcsObjOper, unAcsObjFlag);
				unObjAdd=1;

			}else if (strstr(line, "param=") != NULL) {
				char sAcsParamName[MAX_LEN_PARAM_NAME]={0}, sAcsParamValue[MAX_LEN_PARAM_VALUE]={0};
				uint32_t unAcsParamFlag=0;

				help_copyIdentifierData(line, "param=", "~pv=", sAcsParamName,6,MAX_LEN_PARAM_NAME);
				help_copyIdentifierData(line, "pv=", "~pf=", sAcsParamValue,3,MAX_LEN_PARAM_VALUE);
				help_copyIdentifierData(line, "pf=", NULL, sBuf,3,MAX_LEN);
				unAcsParamFlag = strtol(sBuf,NULL,16);

				if (unObjAdd)
					help_addAcsParamList(pxTmpAcs, sAcsParamName, sAcsParamValue, unAcsParamFlag);
			}else{
				LOGF_LOG_CRITICAL("Invalid option [%s] file may be corrupted\n",line);
			}
		}
	}

finish:
	if (fp)
		fclose(fp);
	if (line)
		free(line);

	return nRet;
}


/*  ============================================================================
 *   Function Name 	: help_changeOwn					*
 *   Description 	: Function to change ownership				*
 *  ============================================================================*/
static int help_changeOwn(const char *pcPath)
{
	struct group  *pxGrp;
	uid_t uid;
	int nRet=UGW_SUCCESS;

	uid=getuid();

	if (uid == 0) // root user
		return UGW_SUCCESS;

	if ((pxGrp = getgrnam("dbgrp")) == NULL) {
		perror("dbgrp");
		return UGW_FAILURE;
	}

	LOGF_LOG_INFO("UID=%d : GID=%d\n",uid,pxGrp->gr_gid);

	if (chown(pcPath, uid, pxGrp->gr_gid) == -1) {
		perror(pcPath);
		nRet = ERR_PATH_NOT_FOUND;
		goto finish;
	}

	if (chmod(pcPath,S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP) == -1) {
		perror(pcPath);
		nRet = ERR_PATH_NOT_FOUND;
		goto finish;
	}

finish:
	return nRet;

}

/*  ============================================================================
 *   Function Name 	: help_storeRespData					*
 *   Description 	: Function to store objlist to buffer(file,shm)		*
 *  ============================================================================*/
int help_storeRespData(IN const char *pcPath, IN MsgHeader *pxMsg)
{
	FILE *fp=NULL;
	int nRet=UGW_SUCCESS;

	if (fopen_s(&fp, pcPath, "w") != EOK) {
		LOGF_LOG_ERROR("Failed to open file for writing : %s\n",pcPath);
		nRet = ERR_FILE_NOT_FOUND;
		goto finish;
	}

	if (fp == NULL) {
                LOGF_LOG_ERROR("\nFile cannot be opened");
		nRet = ERR_FILE_NOT_FOUND;
		goto finish;
        }

	if (help_changeOwn(pcPath) == UGW_FAILURE) {
		LOGF_LOG_ERROR("changing ownership failed : %s\n",pcPath);
		nRet = ERR_FILE_WRITE_FAILED;
		goto finish;
	}

	if (IS_OBJLIST(pxMsg->unSubOper)) {
		ObjList *pxTmpObj = NULL;
		ParamList *pxParam = NULL;

		FOR_EACH_OBJ(pxMsg->pObjType, pxTmpObj) {
			fprintf(fp, "obj=%s~sid=%d~oid=%d~oo=%x~of=%x\n",
					pxTmpObj->sObjName, pxTmpObj->unSid, pxTmpObj->unOid,
					pxTmpObj->unObjOper, pxTmpObj->unObjFlag);
			FOR_EACH_PARAM(pxTmpObj, pxParam) {
				fprintf(fp, "param=%s~pi=%d~pv=%s~pf=%x\n",
						pxParam->sParamName, pxParam->unParamId, pxParam->sParamValue,
						pxParam->unParamFlag);
			}
		}
	}else if (IS_SOPT_OBJATTR(pxMsg->unSubOper)) {
		ObjAttrList *pxAttrObj = NULL;
		ParamAttrList *pxAttrParam = NULL;

		FOR_EACH_OBJATTR(pxMsg->pObjType,pxAttrObj) {
			fprintf(fp, "obj=%s~wn=%s~of=%x\n" ,
					GET_ATTR_OBJNAME(pxAttrObj), GET_ATTR_WEBNAME(pxAttrObj),
					GET_ATTR_FLAG(pxAttrObj));
			FOR_EACH_PARAM_ATTR(pxAttrObj,pxAttrParam) {
				fprintf(fp, "param=%s~pr=%s~pw=%s~pvv=%s~pv=%s~pmiv=%d~pmav=%d~pmil=%d~pmal=%d~pf=%x\n",
						GET_ATTR_PARAMNAME(pxAttrParam), GET_ATTR_PARAMPROFILE(pxAttrParam),
						GET_ATTR_PARAMWEBNAME(pxAttrParam), pxAttrParam->sParamValidVal,
						GET_ATTR_PARAMVALUE(pxAttrParam), GET_ATTR_MINVAL(pxAttrParam),
						GET_ATTR_MAXVAL(pxAttrParam), GET_ATTR_MINLEN(pxAttrParam),
						GET_ATTR_MAXLEN(pxAttrParam),GET_ATTR_PARAMFLAG(pxAttrParam));
			}
		}
	}else if (IS_SOPT_OBJACSATTR(pxMsg->unSubOper)) {
		ObjACSList *pxTmpObjAcs = NULL;
		ParamACSList *pxParamAcs = NULL;

		FOR_EACH_OBJ_ACS_ATTR(pxMsg->pObjType,pxTmpObjAcs) {
			fprintf(fp,"obj=%s~oo=%x~of=%x\n",pxTmpObjAcs->sObjName, pxTmpObjAcs->unObjOper, pxTmpObjAcs->unObjFlag);
			FOR_EACH_PARAM_ACS_ATTR(pxTmpObjAcs, pxParamAcs) {
				fprintf(fp,"param=%s~pv=%s~pf=%x\n",pxParamAcs->sParamName, pxParamAcs->sParamValue,
						pxParamAcs->unParamFlag);
			}
		}
	}

finish:
	if (fp)
		fclose(fp);

	return nRet;
}

/*  =============================================================================
 *   Function Name 	: help_getMsgSize					*
 *   Description 	: function get blob size from the msg			*
 *  ============================================================================*/
static int help_getMsgSize(IN MsgHeader *pxMsg)
{
	uint32_t unMsgSize=0;
	struct blob_buf xBuf;
	void *ptbl=NULL;

	memset(&xBuf, 0x0 , sizeof(struct blob_buf));

	blobmsg_buf_init(&xBuf);

	blobmsg_add_u32(&xBuf,"objflag",pxMsg->unSubOper);
	ptbl = blobmsg_open_table(&xBuf,"Mgh");
	blobmsg_add_u32(&xBuf,"mo",pxMsg->unMainOper);
	blobmsg_add_u32(&xBuf,"so",pxMsg->unSubOper);
	blobmsg_add_u32(&xBuf,"ow",pxMsg->unOwner);
	blobmsg_add_u32(&xBuf,"rf",pxMsg->nFlag);
	blobmsg_close_table(&xBuf, ptbl);

	if (IS_OBJLIST(pxMsg->unSubOper))
	{
		ObjList *pxTmpObj = NULL;
		ParamList *pxParam = NULL;

		FOR_EACH_OBJ(pxMsg->pObjType, pxTmpObj)
		{
			ptbl = blobmsg_open_table(&xBuf,pxTmpObj->sObjName);
			blobmsg_add_u16(&xBuf,"sid",pxTmpObj->unSid);
			blobmsg_add_u16(&xBuf,"oid",pxTmpObj->unOid);
			blobmsg_add_u32(&xBuf,"oo",pxTmpObj->unObjOper);
			blobmsg_add_u32(&xBuf,"of",pxTmpObj->unObjFlag);
			FOR_EACH_PARAM(pxTmpObj, pxParam)
			{
				blobmsg_add_string(&xBuf, "pn",pxParam->sParamName);
				blobmsg_add_u16(&xBuf, "pi", pxParam->unParamId);
				blobmsg_add_string(&xBuf, "pv", pxParam->sParamValue);
				blobmsg_add_u32(&xBuf, "pf", pxParam->unParamFlag);
			}
			blobmsg_close_table(&xBuf, ptbl);
		}
	}
	else if (IS_SOPT_OBJATTR(pxMsg->unSubOper))
	{
		ObjAttrList *pxAttrObj = NULL;
		ParamAttrList *pxAttrParam = NULL;

		FOR_EACH_OBJATTR(pxMsg->pObjType,pxAttrObj)
		{
			ptbl = blobmsg_open_table(&xBuf,GET_ATTR_OBJNAME(pxAttrObj));
			blobmsg_add_string(&xBuf,"wn",GET_ATTR_WEBNAME(pxAttrObj));
			blobmsg_add_u32(&xBuf,"fg",GET_ATTR_FLAG(pxAttrObj));
			FOR_EACH_PARAM_ATTR(pxAttrObj,pxAttrParam)
			{
				blobmsg_add_string(&xBuf, "pn",GET_ATTR_PARAMNAME(pxAttrParam));
				blobmsg_add_string(&xBuf, "pr", GET_ATTR_PARAMPROFILE(pxAttrParam));
				blobmsg_add_string(&xBuf, "pw", GET_ATTR_PARAMWEBNAME(pxAttrParam));
				blobmsg_add_string(&xBuf, "pvv", pxAttrParam->sParamValidVal);
				blobmsg_add_string(&xBuf, "pv", GET_ATTR_PARAMVALUE(pxAttrParam));
				blobmsg_add_u32(&xBuf, "pmiv", GET_ATTR_MINVAL(pxAttrParam));
				blobmsg_add_u32(&xBuf, "pmav", GET_ATTR_MAXVAL(pxAttrParam));
				blobmsg_add_u32(&xBuf, "pmil", GET_ATTR_MINLEN(pxAttrParam));
				blobmsg_add_u32(&xBuf, "pmal", GET_ATTR_MAXLEN(pxAttrParam));
				blobmsg_add_u32(&xBuf, "pf", GET_ATTR_PARAMFLAG(pxAttrParam));
			}
			blobmsg_close_table(&xBuf, ptbl);
		}
	}
	else if (IS_SOPT_OBJACSATTR(pxMsg->unSubOper))
	{
		ObjACSList *pxTmpObjAcs = NULL;
		ParamACSList *pxParamAcs = NULL;

		FOR_EACH_OBJ_ACS_ATTR(pxMsg->pObjType,pxTmpObjAcs)
		{
			ptbl = blobmsg_open_table(&xBuf,pxTmpObjAcs->sObjName);
			blobmsg_add_u32(&xBuf,"oo",pxTmpObjAcs->unObjOper);
			blobmsg_add_u32(&xBuf,"of",pxTmpObjAcs->unObjFlag);
			FOR_EACH_PARAM_ACS_ATTR(pxTmpObjAcs, pxParamAcs)
			{
				blobmsg_add_string(&xBuf, "pn",pxParamAcs->sParamName);
				blobmsg_add_string(&xBuf, "pv", pxParamAcs->sParamValue);
				blobmsg_add_u32(&xBuf, "pf", pxParamAcs->unParamFlag);
			}
			blobmsg_close_table(&xBuf, ptbl);
		}
	}

	unMsgSize += xBuf.buflen;

	if (xBuf.buf != NULL)
		blob_buf_free(&xBuf);

	return unMsgSize;
}


/*  =============================================================================
 *   Function Name 	: help_objListToBlob					*
 *   Description 	: Function to extract the blob msg and fill the caller 	*
 *   			  struct.						*
 *  ============================================================================*/
int help_objListToBlob(OUT struct blob_buf *buf,IN MsgHeader *pxMsg,struct ubus_context *ctx,struct ubus_request_data *req)
{
	void *ptbl;
	uint32_t unMsgSize=0;
	blobmsg_buf_init(buf);

	blobmsg_add_u32(buf,"objflag",pxMsg->unSubOper);

	ptbl = blobmsg_open_table(buf,"Mgh");
	blobmsg_add_u32(buf,"mo",pxMsg->unMainOper);
	blobmsg_add_u32(buf,"so",pxMsg->unSubOper);
	blobmsg_add_u32(buf,"ow",pxMsg->unOwner);
	blobmsg_add_u32(buf,"rf",pxMsg->nFlag);
	blobmsg_close_table(buf, ptbl);

	if (IS_OBJLIST(pxMsg->unSubOper))
	{
		ObjList *pxTmpObj = NULL;
		ParamList *pxParam = NULL;

		FOR_EACH_OBJ(pxMsg->pObjType, pxTmpObj)
		{
			if (buf->buflen >= UBUS_MSG_LENGTH) 
			{
				if ( (ctx != NO_ARG_VALUE) && (req != NO_ARG_VALUE))
				{
					ubus_send_reply(ctx, req, buf->head);
					unMsgSize += buf->buflen;
					blob_buf_free(buf);

					memset(buf,0x0,sizeof(struct blob_buf));
					blobmsg_buf_init(buf);
					blobmsg_add_u32(buf,"objflag",pxMsg->unSubOper);
					ptbl = blobmsg_open_table(buf,"Mgh");
					blobmsg_add_u32(buf,"mo",pxMsg->unMainOper);
					blobmsg_add_u32(buf,"so",pxMsg->unSubOper);
					blobmsg_add_u32(buf,"ow",pxMsg->unOwner);
					blobmsg_add_u32(buf,"rf",pxMsg->nFlag);
					blobmsg_close_table(buf, ptbl);
				}
				else
				{
					printf(" Your Msg May Not Be Sent Due To Overload \n");
				}
			}	
			ptbl = blobmsg_open_table(buf,pxTmpObj->sObjName);
			blobmsg_add_u16(buf,"sid",pxTmpObj->unSid); 
			blobmsg_add_u16(buf,"oid",pxTmpObj->unOid);
			blobmsg_add_u32(buf,"oo",pxTmpObj->unObjOper);
			blobmsg_add_u32(buf,"of",pxTmpObj->unObjFlag);
			FOR_EACH_PARAM(pxTmpObj, pxParam)
			{
				blobmsg_add_string(buf, "pn",pxParam->sParamName);
				blobmsg_add_u16(buf, "pi", pxParam->unParamId);
				blobmsg_add_string(buf, "pv", pxParam->sParamValue);
				blobmsg_add_u32(buf, "pf", pxParam->unParamFlag);
			}
			blobmsg_close_table(buf, ptbl);
		}
	}
	else if (IS_SOPT_OBJATTR(pxMsg->unSubOper))
	{
		ObjAttrList *pxAttrObj = NULL;
		ParamAttrList *pxAttrParam = NULL;

		FOR_EACH_OBJATTR(pxMsg->pObjType,pxAttrObj)
		{
			if (buf->buflen >= UBUS_MSG_LENGTH) 
			{
				if ( (ctx != NO_ARG_VALUE) && (req != NO_ARG_VALUE))
				{
					ubus_send_reply(ctx, req, buf->head);
					unMsgSize += buf->buflen;
					blob_buf_free(buf);

					memset(buf,0x0,sizeof(struct blob_buf));
					blobmsg_buf_init(buf);
					blobmsg_add_u32(buf,"objflag",pxMsg->unSubOper);
					ptbl = blobmsg_open_table(buf,"Mgh");
					blobmsg_add_u32(buf,"mo",pxMsg->unMainOper);
					blobmsg_add_u32(buf,"so",pxMsg->unSubOper);
					blobmsg_add_u32(buf,"ow",pxMsg->unOwner);
					blobmsg_add_u32(buf,"rf",pxMsg->nFlag);
					blobmsg_close_table(buf, ptbl);
				}
				else
				{
					printf(" Your Msg May Not Be Sent Due To Overload \n");
				}
			}	
			ptbl = blobmsg_open_table(buf,GET_ATTR_OBJNAME(pxAttrObj));
			blobmsg_add_string(buf,"wn",GET_ATTR_WEBNAME(pxAttrObj)); 
			blobmsg_add_u32(buf,"fg",GET_ATTR_FLAG(pxAttrObj));
			FOR_EACH_PARAM_ATTR(pxAttrObj,pxAttrParam)
			{
				blobmsg_add_string(buf, "pn",GET_ATTR_PARAMNAME(pxAttrParam));
				blobmsg_add_string(buf, "pr", GET_ATTR_PARAMPROFILE(pxAttrParam));
				blobmsg_add_string(buf, "pw", GET_ATTR_PARAMWEBNAME(pxAttrParam));
				blobmsg_add_string(buf, "pvv", pxAttrParam->sParamValidVal);
				blobmsg_add_string(buf, "pv", GET_ATTR_PARAMVALUE(pxAttrParam));
				blobmsg_add_u32(buf, "pmiv", GET_ATTR_MINVAL(pxAttrParam));
				blobmsg_add_u32(buf, "pmav", GET_ATTR_MAXVAL(pxAttrParam));
				blobmsg_add_u32(buf, "pmil", GET_ATTR_MINLEN(pxAttrParam));
				blobmsg_add_u32(buf, "pmal", GET_ATTR_MAXLEN(pxAttrParam));
				blobmsg_add_u32(buf, "pf", GET_ATTR_PARAMFLAG(pxAttrParam));
			}
			blobmsg_close_table(buf, ptbl);
		}
	}
	else if (IS_SOPT_OBJACSATTR(pxMsg->unSubOper))
	{
		ObjACSList *pxTmpObjAcs = NULL;
		ParamACSList *pxParamAcs = NULL;

		FOR_EACH_OBJ_ACS_ATTR(pxMsg->pObjType,pxTmpObjAcs)
		{
			if (buf->buflen >= UBUS_MSG_LENGTH) 
			{
				if ( (ctx != NO_ARG_VALUE) && (req != NO_ARG_VALUE))
				{
					ubus_send_reply(ctx, req, buf->head);

					unMsgSize += buf->buflen;
					blob_buf_free(buf);
					memset(buf,0x0,sizeof(struct blob_buf));
					blobmsg_buf_init(buf);
					blobmsg_add_u32(buf,"objflag",pxMsg->unSubOper);
					ptbl = blobmsg_open_table(buf,"Mgh");
					blobmsg_add_u32(buf,"mo",pxMsg->unMainOper);
					blobmsg_add_u32(buf,"so",pxMsg->unSubOper);
					blobmsg_add_u32(buf,"ow",pxMsg->unOwner);
					blobmsg_add_u32(buf,"rf",pxMsg->nFlag);
					blobmsg_close_table(buf, ptbl);
				}
				else
				{
					printf(" Your Msg May Not Be Sent Due To Overload \n");
				}
			}
			ptbl = blobmsg_open_table(buf,pxTmpObjAcs->sObjName);
			blobmsg_add_u32(buf,"oo",pxTmpObjAcs->unObjOper);
			blobmsg_add_u32(buf,"of",pxTmpObjAcs->unObjFlag);
			FOR_EACH_PARAM_ACS_ATTR(pxTmpObjAcs, pxParamAcs)
			{
				blobmsg_add_string(buf, "pn",pxParamAcs->sParamName);
				blobmsg_add_string(buf, "pv", pxParamAcs->sParamValue);
				blobmsg_add_u32(buf, "pf", pxParamAcs->unParamFlag);
			}
			blobmsg_close_table(buf, ptbl);
		}
	}

	unMsgSize += buf->buflen;
	blobmsg_add_u32(buf,"MsgSize",unMsgSize);
	return UGW_SUCCESS;
}

/*  =============================================================================
 *   Function Name 	: help_printJsonFormat					*
 *   Description 	: Function to traverse list and dump object name,id	*
 *  ============================================================================*/
int help_getJsonFormat(IN MsgHeader *pxMsg,char **ppcBuf)
{

	ObjList *pxTmpObj = NULL;
	ObjList *pxObj = NULL;
	ParamList *pxParam = NULL;
	char *pcBuf;
	int nParamFlag=0;
	uint32_t unSize = 0;
	int nTlSize=0, nRet=UGW_SUCCESS,nLen=0;
	static int nTopObj=0,nDevFlag=0,nCloseTag=0;

	pxObj = pxMsg->pObjType;

	unSize = help_getMsgSize(pxMsg);
	/* additional space allocation required for json msg fromat */	

	nTlSize=(unSize*3)+65536;

	pcBuf = HELP_MALLOC((unSize*3)+65536); 
	if (pcBuf == NULL)
		return ERR_MEMORY_ALLOC_FAILED;


	if ((nLen=sprintf_s(pcBuf,MAX_LEN_OBJNAME, "{")) <= 0) {
		LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
		nRet=ERR_JSON_BUFFER_CONSTRUCT_FAILED;
		goto finish;
	}

	list_for_each_entry(pxTmpObj,&(pxObj->xOlist),xOlist) {

		if (nLen > nTlSize) {
			nRet = ERR_JSON_BUFFER_CONSTRUCT_FAILED;
			goto finish;
		}

		if (strcmp("Device",pxTmpObj->sObjName) == 0) {
			if ((nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "\"%s\" : {", pxTmpObj->sObjName))<=0) {
				LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
				nRet=ERR_JSON_BUFFER_CONSTRUCT_FAILED;
				goto finish;
			}
			nCloseTag=1;
			continue;
		} else if (nDevFlag==0) {
			if ((nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "\"Objects\": ["))<=0) {
				LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
				nRet=ERR_JSON_BUFFER_CONSTRUCT_FAILED;
				goto finish;
			}
			nDevFlag=1;
		}
		if (nTopObj == 1) {
			if ((nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "},"))<=0) {
				LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
				nRet=ERR_JSON_BUFFER_CONSTRUCT_FAILED;
				goto finish;
			}
			nTopObj = 0;
		}
		if ((nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "{"))<=0) {
			LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
			nRet=ERR_JSON_BUFFER_CONSTRUCT_FAILED;
			goto finish;
		}
		if ((nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "\"ObjName\": \"%s\",",pxTmpObj->sObjName))<=0) {
			LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
			nRet=ERR_JSON_BUFFER_CONSTRUCT_FAILED;
			goto finish;
		}

		nParamFlag = 0;
		if ((nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "\"Param\":[" ))<=0) {
			LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
			nRet=ERR_JSON_BUFFER_CONSTRUCT_FAILED;
			goto finish;
		}
		list_for_each_entry(pxParam,&(pxTmpObj->xParamList.xPlist),xPlist) {

			if (nLen > nTlSize) {
				nRet = ERR_JSON_BUFFER_CONSTRUCT_FAILED;
				goto finish;
			}

			if (nParamFlag == 1) {
				if ((nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "},"))<=0) {
					LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
					nRet=ERR_JSON_BUFFER_CONSTRUCT_FAILED;
					goto finish;
				}
			}
			if ((nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "{"))<=0) {
				LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
				nRet=ERR_JSON_BUFFER_CONSTRUCT_FAILED;
				goto finish;
			}
			if ((nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "\"ParamName\":\"%s\",",pxParam->sParamName))<=0) {
				LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
				nRet=ERR_JSON_BUFFER_CONSTRUCT_FAILED;
				goto finish;
			}
			if ((nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "\"ParamId\": \"%d\",",GET_PARAM_ID(pxParam)))<=0) {
					LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
					nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "\"ParamId\": \"\",");
			}
			if (strnlen_s(pxParam->sParamValue, MAX_LEN_OBJNAME)>0) {
				if ((nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "\"ParamValue\": \"%s\"," ,pxParam->sParamValue))<=0) {
					LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
					nRet=ERR_JSON_BUFFER_CONSTRUCT_FAILED;
					goto finish;
				}
			} else {
				if ((nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "\"ParamValue\": \"\","))<=0) {
					LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
				}
			}
			if ((nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "\"ParamFlag\": \"0x%x\"", pxParam->unParamFlag))<=0) {
				LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
				nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "\"ParamFlag\": \"\"");
			}
			nParamFlag = 1;
		}
		nTopObj=1;
		if (nParamFlag == 1) {
			if ((nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "}"))<=0) {
				LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
				nRet=ERR_JSON_BUFFER_CONSTRUCT_FAILED;
				goto finish;
			}
		}

		if ((nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "]"))<=0) {
			LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
			nRet=ERR_JSON_BUFFER_CONSTRUCT_FAILED;
			goto finish;
		}
	}

	if (nTopObj == 1) {	
		if ((nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "}"))<=0) {
			LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
			nRet=ERR_JSON_BUFFER_CONSTRUCT_FAILED;
			goto finish;
		}
		nTopObj=0;
	}
	//Device.
	if ((nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "]"))<=0) {
		LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
		nRet=ERR_JSON_BUFFER_CONSTRUCT_FAILED;
		goto finish;
	}
	if (nCloseTag == 1) {
		if ((nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "}"))<=0) {
			LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
			nRet=ERR_JSON_BUFFER_CONSTRUCT_FAILED;
			goto finish;
		}
		nCloseTag=0;
	}
	nDevFlag=0;
	//Top Level
	if ((nLen += sprintf_s(pcBuf+nLen, MAX_LEN_OBJNAME, "}"))<=0) {
		LOGF_LOG_ERROR("sprintf_s buffer construction failed reason(%s)\n",strerror(errno));
		nRet=ERR_JSON_BUFFER_CONSTRUCT_FAILED;
		goto finish;
	}
	//pcBuf[strlen(pcBuf)]='\0';
	*ppcBuf = pcBuf;
finish:
	if (nRet == ERR_JSON_BUFFER_CONSTRUCT_FAILED)
		HELP_FREE(pcBuf);

	return nRet;
}

/*  =============================================================================
 *   Function Name 	: help_printMsg						*
 *   Description 	: Function to traverse list and dump object name,id	*
 *  ============================================================================*/
void help_printMsg(IN MsgHeader *pxMsg)
{
	fprintf(stderr,"{\n");
	fprintf(stderr," \t \"MsgHeader\":{\n");
	fprintf(stderr,"\t\t \"MainOper\":%ju,\n",(uintmax_t)pxMsg->unMainOper);
	fprintf(stderr,"\t\t \"SubOper\":%ju,\n",(uintmax_t)pxMsg->unSubOper);
	fprintf(stderr,"\t\t \"Owner\":%ju,\n",(uintmax_t)pxMsg->unOwner);
	fprintf(stderr,"\t\t \"ReservdFlag\":%ju\n",(uintmax_t)pxMsg->nFlag);
	fprintf(stderr,"\t\t \"MsgSize\":%ju\n",(uintmax_t)pxMsg->unMsgSize);
	fprintf(stderr,"\t },\n");
	help_printObj(pxMsg->pObjType, pxMsg->unSubOper);
	fprintf(stderr," }\n");
}

/*  =============================================================================
 *   Function Name 	: help_delMsg						*
 *   Description 	: Function to traverse list and free objects 		*
 *  ============================================================================*/
void help_delMsg(IN MsgHeader *pxMsg,bool bFlag)
{

	if (IS_OBJLIST(pxMsg->unSubOper))
	{
		ObjList *pxTempObj; 
		ObjList *pxObjList=NULL;
		ParamList *pxParamList; 

		pxObjList = pxMsg->pObjType;

		while( !list_empty(&pxObjList->xOlist) ) 
		{
			pxTempObj = list_entry(pxObjList->xOlist.next,ObjList,xOlist);
			while( !list_empty(&pxTempObj->xParamList.xPlist) ) 
			{
				pxParamList = list_entry(pxTempObj->xParamList.xPlist.next,ParamList,xPlist);
				list_del(&pxParamList->xPlist);
				free(pxParamList);
				pxParamList = NULL;
			}
			list_del(&pxTempObj->xOlist);
			free(pxTempObj);
			pxTempObj = NULL;
		}
	}
	else if (IS_SOPT_OBJATTR(pxMsg->unSubOper))
	{
		ObjAttrList *pxTempObj; 
		ObjAttrList *pxObjList=NULL;
		ParamAttrList *pxParamList; 

		pxObjList = pxMsg->pObjType;

		while( !list_empty(&pxObjList->xOlist) ) 
		{
			pxTempObj = list_entry(pxObjList->xOlist.next,ObjAttrList,xOlist);
			while( !list_empty(&pxTempObj->xParamAttrList.xPlist) ) 
			{
				pxParamList = list_entry(pxTempObj->xParamAttrList.xPlist.next,ParamAttrList,xPlist);
				list_del(&pxParamList->xPlist);
				free(pxParamList);
				pxParamList = NULL;
			}
			list_del(&pxTempObj->xOlist);
			free(pxTempObj);
			pxTempObj = NULL;
		}
	}
	if (IS_SOPT_OBJACSATTR(pxMsg->unSubOper))	
	{
		ObjACSList *pxTempAcsObj; 
		ParamACSList *pxParamAcsList; 
		ObjACSList *pxObjAcsList;

		pxObjAcsList = pxMsg->pObjType;
		while( !list_empty(&pxObjAcsList->xOlist) ) 
		{
			pxTempAcsObj = list_entry(pxObjAcsList->xOlist.next,ObjACSList,xOlist);
			while( !list_empty(&pxTempAcsObj->xParamAcsList.xPlist) ) 
			{
				pxParamAcsList = list_entry(pxTempAcsObj->xParamAcsList.xPlist.next,ParamACSList,xPlist);
				list_del(&pxParamAcsList->xPlist);
				free(pxParamAcsList);
				pxParamAcsList = NULL;
			}
			list_del(&pxTempAcsObj->xOlist);
			free(pxTempAcsObj);
			pxTempAcsObj = NULL;
		}
	}
	if (bFlag)
	{
		if (pxMsg->pObjType != NULL)
			free(pxMsg->pObjType);
		pxMsg->pObjType = NULL;
	}
}

/*  =============================================================================
 *   Function Name 	: help_ubusConnect					*
 *   Description 	: Function opens ubus socket and creates context 	*
 *   			  to connect with csd daemon				*
 *  ============================================================================*/
struct ubus_context * help_ubusConnect(void) {
	const char *ubus_socket = NULL;
	struct ubus_context *ctx;
	ctx =  ubus_connect(ubus_socket);
	if (NULL == ctx) {
		LOGF_LOG_ERROR("help_ubusConnect - ubus_connect failed.\n");
	}
	return ctx;
}
/*  =============================================================================
 *   Function Name 	: help_ubusDisconnect					*
 *   Description 	: Function to terminate ubus connection			*
 *  ============================================================================*/
void help_ubusDisconnect(struct ubus_context * ctx)
{
	if (ctx != NULL)
	{
		ubus_free(ctx);
		ctx = NULL;
	}
}


