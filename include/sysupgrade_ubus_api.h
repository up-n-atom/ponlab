/********************************************************************************

  Copyright (C) 2020-2022 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.
********************************************************************************/

/********************************************************************************
 *       File Name    : sysupgrade_ubus_api.h                                   *
 *       Description  : sysupgrade - Service Daemon ubus function prototypes    *
 *                                                                              *
 ********************************************************************************/

/*! \file sysupgrade_ubus_api.h
    \brief This file contains ubus connectivity definitions and prototypes for SYSUPGRADE.
*/

/** \defgroup sysupgrade Service Daemon [ SYSUPGRADE ]

  \brief Description of sysupgrade
  */
/* @{ */
#ifndef _SYSUPGRADE_UBUS_API_H
#define _SYSUPGRADE_UBUS_API_H

void sysupgrade_ubusRegister(void);
uint16_t sysupgrade_ubusInit(void);
void sysupgrade_ubusDone(void);

/* @} */

#endif /* _SYSUPGRADE_UBUS_API_H */
