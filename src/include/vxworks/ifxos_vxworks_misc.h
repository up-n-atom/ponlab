/****************************************************************************

         Copyright (c) 2021 MaxLinear, Inc.
         Copyright (c) 2009 Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*****************************************************************************/
#ifndef _IFXOS_VXWORKS_MISC_H
#define _IFXOS_VXWORKS_MISC_H

#ifdef VXWORKS

/** \file
   This file contains VxWorks definitions of Miscellaneous functions.
*/

/** \defgroup IFXOS_MISC_VXWORKS_APPL Miscellaneous functions (VxWorks)

   This Group contains the VxWorks "Miscellaneous" definitions and function.


\ingroup IFXOS_LAYER_VXWORKS
*/

#ifdef __cplusplus
   extern "C" {
#endif

/* ============================================================================
   IFX VxWorks adaptation - Includes
   ========================================================================= */

/* ============================================================================
   IFX VxWorks adaptation - supported features
   ========================================================================= */

/** IFX VxWorks adaptation - User support "Miscellaneous functions" */
#ifndef IFXOS_HAVE_MISC
#  define IFXOS_HAVE_MISC                       1
#endif

/* ============================================================================
   IFX VxWorks adaptation - types
   ========================================================================= */

#ifdef __cplusplus
}
#endif
#endif      /* #ifdef VXWORKS */
#endif      /* #ifndef _IFXOS_VXWORKS_MISC_H */


