/****************************************************************************

         Copyright (c) 2021 - 2022 MaxLinear, Inc.
         Copyright (c) 2009 Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*****************************************************************************/
#ifndef _IFXOS_LINUX_COMMON_H
#define _IFXOS_LINUX_COMMON_H

#ifdef LINUX
/** \file
   This file contains common definitions used for the Linux OS adaptation
*/

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _IFXOS_COMMON_H
#  error "missing IFXOS endian defines, include 'ifx_common.h' at first"
#endif

#ifdef __KERNEL__
/* ============================================================================
   IFX Linux adaptation - Linux Kernel Space
   ========================================================================= */

/** \defgroup IFXOS_IF_LINUX_DRV Defines for Linux Kernel Adaptation

   This Group contains the Linux specific definitions and function.

\par Linux Kernel Endianess
   Under Linux in Kernel space the Endianess is defined within the corresponding
   architecture header files.
   The platform endianess is mapped to the internal used IFXOS endianess definitions.

\attention
   Under Linux only the _LITTLE_ENDIAN or the __BIG_ENDIAN is set, so avoid
   to use this settings directly !!!

\ingroup IFXOS_INTERFACE
*/

/* ============================================================================
   IFX Linux adaptation - Includes (Linux Kernel)
   ========================================================================= */
#include <linux/kernel.h>
#include <linux/version.h>
#include <asm/byteorder.h>

/* ============================================================================
   IFX Linux adaptation - Macro definitions (Linux Kernel)
   ========================================================================= */

/** \addtogroup IFXOS_IF_LINUX_DRV
@{ */

#if defined ( __LITTLE_ENDIAN )
   /** set the internal endianess macro for LITTLE endian */
#  define __BYTE_ORDER                       __LITTLE_ENDIAN
   /** set the common IFXOS byte order for LITTLE endian */
#  define IFXOS_BYTE_ORDER                   IFXOS_LITTLE_ENDIAN
#elif defined ( __BIG_ENDIAN )
   /** set the internal endianess macro for BIG endian */
#  define __BYTE_ORDER                       __BIG_ENDIAN
   /** set the common IFXOS byte order for BIG endian */
#  define IFXOS_BYTE_ORDER                   IFXOS_BIG_ENDIAN
#else
#  error "missing endian definition"
#endif

/** @} */

#else      /* #ifdef __KERNEL__ */

/** \defgroup IFXOS_IF_LINUX_APPL Defines for Linux Application Adaptation

   This Group contains the Linux specific definitions and function.

\par Linux Application Endianess
   Under Linux in user space the endianess is defined within the corresponding
   header file <endian.h>.
   The platform endianess is mapped to the internal used IFXOS endianess definitions.

\ingroup IFXOS_INTERFACE
*/

/* ============================================================================
   IFX Linux adaptation - Includes (Linux Kernel)
   ========================================================================= */
#include <endian.h>

/* ============================================================================
   IFX Linux adaptation - Macro definitions (Linux User Space)
   ========================================================================= */

/** \addtogroup IFXOS_IF_LINUX_APPL
@{ */

#ifndef __BYTE_ORDER
#  error "Unknown System Byteorder!"
#endif

#ifndef _IFXOS_COMMON_H
#  error "missing IFXOS endian defines, include 'ifx_common.h' at first"
#endif

#if (__BYTE_ORDER == __LITTLE_ENDIAN)
#  define IFXOS_BYTE_ORDER                IFXOS_LITTLE_ENDIAN
#elif (__BYTE_ORDER == __BIG_ENDIAN )
#  define IFXOS_BYTE_ORDER                IFXOS_BIG_ENDIAN
#else
#  error "no matching __BYTE_ORDER found"
#endif

/** @} */

#endif      /* #ifdef __KERNEL__ */

#ifdef __cplusplus
}
#endif
#endif      /* #ifdef Linux */
#endif      /* #ifndef _IFXOS_LINUX_COMMON_H */

