/****************************************************************************

         Copyright (c) 2021 MaxLinear, Inc.
         Copyright (c) 2016 - 2019 Intel Corporation
         Copyright (c) 2011 - 2016 Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*****************************************************************************/

/* ============================================================================
   Description : IFX OS adaptation, Linux Kernel copy form and to user
   ========================================================================= */

#ifdef LINUX
#ifdef __KERNEL__

/** \file
   This file contains the IFXOS Layer implementation for LINUX Kernel
   Data exchange between driver and application.
*/

/* ============================================================================
   IFX Linux adaptation - Global Includes - Kernel
   ========================================================================= */

#include <linux/kernel.h>
#ifdef MODULE
   #include <linux/module.h>
#endif
#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 12, 0)
#include <asm/uaccess.h>
#else
#include <linux/uaccess.h>
#endif

#include "ifx_types.h"
#include "ifxos_rt_if_check.h"
#include "ifxos_copy_user_space.h"

/* ============================================================================
   IFX Linux adaptation - copy between user and kernel space
   ========================================================================= */

/** \addtogroup IFXOS_DRV_CPY_USER_SPACE_LINUX
@{ */

/**
   LINUX Kernel - Copy a block FORM USER space (application) to driver space (kernel).

\par Implementation
   Copy data from user to kernel space by use of the kernel function "copy_from_user"

\param
   pTo         Points to the source (in user space).
\param
   pFrom       Points to the destination (in driver space).
\param
   size_byte   Block size to copy [byte].

\return
   IFX_NULL if an error occurred, else pTo
*/
IFX_void_t *IFXOS_CpyFromUser(
               IFX_void_t        *pTo,
               const IFX_void_t  *pFrom,
               IFX_uint32_t      size_byte)
{
   IFX_uint32_t remainBytes;

   IFXOS_RETURN_IF_POINTER_NULL(pTo, IFX_NULL);
   IFXOS_RETURN_IF_POINTER_NULL(pFrom, IFX_NULL);
   IFXOS_RETURN_IF_ARG_LE_ZERO(size_byte, IFX_NULL);

   remainBytes = (IFX_uint32_t)copy_from_user( pTo, pFrom,
      (unsigned long)size_byte);

   return (remainBytes) ? IFX_NULL : pTo;
}

/**
   LINUX Kernel - Copy a block form driver space (kernel) TO USER space (application).

\par Implementation
   Copy data from kernel to user space by use of the kernel function "copy_to_user"

\param
   pTo         Points to the source (in driver space)
\param
   pFrom       Points to the destination (in user space)
\param
   size_byte   Block size to copy [byte]

\return
   IFX_NULL if an error occurred, else pTo
*/
IFX_void_t *IFXOS_CpyToUser(
               IFX_void_t *pTo,
               const IFX_void_t *pFrom,
               IFX_uint32_t size_byte)
{
   IFX_uint32_t remainBytes;

   IFXOS_RETURN_IF_POINTER_NULL(pTo, IFX_NULL);
   IFXOS_RETURN_IF_POINTER_NULL(pFrom, IFX_NULL);
   IFXOS_RETURN_IF_ARG_LE_ZERO(size_byte, IFX_NULL);

   remainBytes = (IFX_uint32_t)copy_to_user( pTo, pFrom,
      (unsigned long)size_byte);

   return (remainBytes) ? IFX_NULL : pTo;
}

/** @} */

#ifdef MODULE
EXPORT_SYMBOL(IFXOS_CpyFromUser);
EXPORT_SYMBOL(IFXOS_CpyToUser);
#endif

#endif      /* #ifdef __KERNEL__ */
#endif      /* #ifdef LINUX */

