#ifndef _LIB_TAPI_NLT_OSMAP_H
#define _LIB_TAPI_NLT_OSMAP_H
/******************************************************************************

                              Copyright (c) 2012
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

/**
   \file lib_tapi_nlt_osmap.h
   This file contains the includes and the defines specific to the OS.
*/

#include <ifx_types.h>     /* ifx type definitions */

#ifndef VXWORKS
#include "lib_tapi_nlt_config.h"
#endif /* !VXWORKS */

#include <string.h>

#ifndef LIB_TAPI_NLT_IFXOS_SUPPORT
   #ifdef LINUX
      /* system includes */
      #include <unistd.h>
      #include <sys/ioctl.h>
   #else /* LINUX */
      #error unsupported platform
   #endif /* LINUX */
#else /* LIB_TAPI_NLT_IFXOS_SUPPORT */
   #include "ifxos_time.h"
   #include "ifxos_device_access.h"
#endif /* LIB_TAPI_NLT_IFXOS_SUPPORT */

/* ============================= */
/* Global Defines                */
/* ============================= */

/* ============================= */
/* Name mapping tables           */
/* ============================= */


#ifndef LIB_TAPI_NLT_IFXOS_SUPPORT

#ifdef LINUX

#define LIB_NLT_DeviceControl           ioctl
#define LIB_NLT_SecSleep                sleep

#else /* LINUX */
      #error unsupported platform
#endif /* LINUX */

#else /* LIB_TAPI_NLT_IFXOS_SUPPORT */


#define LIB_NLT_DeviceControl           IFXOS_DeviceControl
#define LIB_NLT_SecSleep                IFXOS_SecSleep

#endif /* LIB_TAPI_NLT_IFXOS_SUPPORT */

#endif /* _LIB_TAPI_NLT_OSMAP_H */
