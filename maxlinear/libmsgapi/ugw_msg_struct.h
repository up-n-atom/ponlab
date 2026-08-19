/********************************************************************************
 
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.
 
********************************************************************************/

/*  ***************************************************************************** 
 *         File Name    : ugw_msg_struct.h                                         *
 *         Description  : UGW Msg API layer which exposes APIs that can be used *
 *			  to communicate with UGW framework from non UGW 	*
 *			  component 						*
 *  *****************************************************************************/

/*! \file ugw_msg_struct.h
    \brief File contains structure to communicate external entity with UGW framework.
*/
/** \addtogroup UGWMSG */
/* @{ */

#ifndef _UGW_MSG_STRUCT_H
#define _UGW_MSG_STRUCT_H

#define MAX_SERVER_NAME 32
#define MAX_SL_NAME 128

typedef struct
{
	char sSerName[MAX_SERVER_NAME];  /*!< msg owner */
        char sSLId[MAX_SL_NAME] ; /*!< Identifier of ugw msg receiver at SL like 
				    	sl_App, sl_beerock, sl_featureApp */
        uint32_t nOperType;/*!< Operation type ADD, DEL, MOD, NOTIFY, GET */
        uint32_t nMsgType;/*!< Identifier of the service like WLAN, BRIDGE, ROUTE, QOS */
        uint32_t nMsgSize;/*!< MsgSize */
        uint32_t nRMsgSize;/*!< Reply MsgSize */
        void *pMsg ; /*!< msg - can be structure or string or anything*/
        void *pRMsg ; /*!< msg - response msg from server */
	unsigned int id; /*!< msg id */
}Msg_t;

#endif /*_UGW_MSG_STRUCT_H*/
/* @} */
