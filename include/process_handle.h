/******************************************************************************

  Copyright (C) 2022 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

 ******************************************************************************/
/***************************************************************************** *
 *     File Name  : process_handle.h                                           *
 *     Project    : UGW                                                        *
 *     Description: process init and process control api's                     *
 *                                                                             *
 ******************************************************************************/
#ifndef _PROCESS_H
#define _PROCESS_H

extern char *sProcessName;
extern char *sProcessWsdChldName;
extern int nCPUs;

void exit_sysupgrade(uint16_t unStatus);
void sysupgrade_childProcessInit(const char *pcPsName);
void sysupgrade_processControl(char *sName, int nFlg);

 #endif /* _PROCESS_H */
