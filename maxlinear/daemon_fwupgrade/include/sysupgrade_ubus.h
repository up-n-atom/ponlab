/********************************************************************************

  Copyright (C) 2020-2022 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.
********************************************************************************/

/********************************************************************************
 *       File Name    : sysupgrade_ubus.h                        *
 *       Description  : SYSUPGRADE - Service Daemon ubus definitions and prototypes  *
 *                                                                              *
 ********************************************************************************/

/*! \file sysupgrade_ubus.h
	\brief This file contains ubus connectivity definitions and prototypes for SYSUPGRADE.
*/

#ifndef _SYSUPGRADE_UBUS_H
#define _SYSUPGRADE_UBUS_H

#include <ugw_defs.h>
#include <ugw_msg_api.h>

/*!
	\brief SYSUPGRADE_UBUS_SERVER
*/
#define SYSUPGRADE_UBUS_SERVER "fwupgrade"

/*!
	\brief SYSUPGRADE_UBUS_METHOD_ACTIVATE
*/
#define SYSUPGRADE_UBUS_METHOD_IMG_ACTIVATE "img_activate" /*!< sysupgrade_img_activate method.*/
/*!
	\brief SYSUPGRADE_UBUS_METHOD_GET_UBOOT_ENV
*/
#define SYSUPGRADE_UBUS_METHOD_GET_UBOOT_ENV "get_uboot_env" /*!< sysupgrade_get_uboot_env method.*/
/*!
	\brief SYSUPGRADE_UBUS_METHOD_GET_UBOOT_PARAM
*/
#define SYSUPGRADE_UBUS_METHOD_GET_UBOOT_PARAM "get_uboot_param" /*!< sysupgrade_get_uboot_param method.*/
/*!
	\brief SYSUPGRADE_UBUS_METHOD_SET_UBOOT_ENV
*/
#define SYSUPGRADE_UBUS_METHOD_SET_UBOOT_ENV "set_uboot_env" /*!< sysupgrade_set_uboot_env method.*/
/*!
	\brief SYSUPGRADE_UBUS_METHOD_WRITE_IMG
*/
#define SYSUPGRADE_UBUS_METHOD_WRITE_IMG "write_img" /*!< sysupgrade_write_img method.*/
/*!
	\brief SYSUPGRADE_UBUS_METHOD_REBOOT
*/
#define SYSUPGRADE_UBUS_METHOD_REBOOT "reboot" /*!< sysupgrade_reboot method.*/

#endif /* _SYSUPGRADE_UBUS_H */

