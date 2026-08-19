/****************************************************************************

         Copyright (c) 2021 MaxLinear, Inc.
         Copyright (c) 2009 Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*****************************************************************************/
#ifndef _IFXOS_NUCLEUS_DEVICE_ACCESS_H
#define _IFXOS_NUCLEUS_DEVICE_ACCESS_H

#ifdef NUCLEUS_PLUS

/** \file
   This file contains Nucleus definitions for Device Access.
*/

/** \defgroup IFXOS_DEVICE_ACCESS_NUCLEUS_APPL Device Access (Nucleus)

   This Group contains the Nucleus Device Access definitions and function.

   The standard system calls (open, close, etc) are mapped to device specific
   functions.

   The poll/select mechanism is supported. This requires OS support and a
   corresponding implementation on application and driver side.

\ingroup IFXOS_LAYER_NUCLEUS
*/

#ifdef __cplusplus
   extern "C" {
#endif

/* ============================================================================
   IFX Nucleus adaptation - Includes
   ========================================================================= */
#include <nucleus.h>
#include <ioctl.h>
#include <selectLib.h>

/* ============================================================================
   IFX Nucleus adaptation - supported features
   ========================================================================= */

/** IFX Nucleus adaptation - User support "device access" */
#ifndef IFXOS_HAVE_DEVICE_ACCESS
#  define IFXOS_HAVE_DEVICE_ACCESS                 1
#endif

/** IFX Nucleus adaptation - User support "device access - select" */
#ifndef IFXOS_HAVE_DEVICE_ACCESS_SELECT
#  define IFXOS_HAVE_DEVICE_ACCESS_SELECT          1
#endif

/* ============================================================================
   IFX Nucleus adaptation - types
   ========================================================================= */

#ifndef _IO
   /** required form OS headers */
#  error "missing _IO definition"
#endif


/** Nucleus User - select, type fd_set for device select. */
typedef fd_set       IFXOS_devFd_set_t;

#ifdef __cplusplus
}
#endif
#endif      /* #ifdef NUCLEUS_PLUS */
#endif      /* #ifndef _IFXOS_NUCLEUS_DEVICE_ACCESS_H */


