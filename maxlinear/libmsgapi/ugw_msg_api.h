/********************************************************************************

  Copyright (c) 2024 MaxLinear, Inc.
  Copyright (C) 2017-2018 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

********************************************************************************/

/*  ***************************************************************************** 
 *         File Name    : ugw_msg_api.h                                         *
 *         Description  : UGW Msg API layer which exposes APIs that can be used *
 *			  to communicate with UGW framework from non UGW 	*
 *			  component 						*
 *  *****************************************************************************/

/*! \file ugw_msg_api.h
    \brief File contains structure and function definitions to communicate external entity with UGW framework.
*/
/** \addtogroup UGWMSG */
/* @{ */

#ifndef _UGW_MSG_H
#define _UGW_MSG_H


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <libubus.h>
#include "safe_str_lib.h"
#include <libubox/blobmsg_json.h>
#include <libubox/blobmsg.h>
#include "ugw_msg_struct.h"
#include "safe_mem_lib.h"

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

#ifndef UGW_SUCCESS
#define UGW_SUCCESS 0
#endif

#ifndef UGW_FAILURE
#define UGW_FAILURE -1
#endif

#define REPLY_MSG_FALIED -2

/*! \enum OperType
    \brief Enum containing operation type.
*/
typedef enum 
{
   OPT_ADD = 0x1, /*!< Add operation */
   OPT_DEL = 0x2, /*!< Delete operation */
   OPT_MODIFY = 0x4, /*!< Modify operation */
   OPT_NOTIFY = 0x8, /*!< Notify operation */
   OPT_GET = 0x10, /*!< Get operation */
   OPT_THREAD_ENV = 0x20 /*!< consider thread handling in ubus communication */
}OperType;

#define IS_THRED_ENV_SET(bit) (bit & OPT_THREAD_ENV) 	/*!< thread handling in ubus communication is set or not */


/* call back function */
typedef int32_t (*cb_func) (Msg_t *);

/*! 
    \brief API to receive msg from external entity over ubus 
    \param[in] pxBlobMsg ubus msg fromat which is received from the external entity 
    \param[in] pxMsg ugw msg, it will be filled after parsing blob msg.  
    \return UGW_SUCCESS on success / UGW_FAILURE on failure
*/
int msg_recv(IN struct blob_attr *pxBlobMsg, OUT Msg_t *pxMsg);

/*! 
    \brief API to send msg to ugw framework over ubus 
    \param[in] pxMsg ugw msg,it will be filled by the caller of this function 
    \return UGW_SUCCESS on success / UGW_FAILURE on failure
*/
int msg_send(IN Msg_t *pxMsg);

#endif /*_UGW_MSG_H*/
/* @} */
