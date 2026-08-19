/****************************************************************************

         Copyright (c) 2021 MaxLinear, Inc.
         Copyright (c) 2009 Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*****************************************************************************/
#ifndef _IFXOS_NUCLEUS_TERMIOS_H
#define _IFXOS_NUCLEUS_TERMIOS_H

#ifdef NUCLEUS_PLUS

/** \file
   This file contains Nucleus definitions for Terminal IO System.
*/

/** \defgroup IFXOS_TERMIOS_NUCLEUS_APPL Terminal IO System (Nucleus)

   This Group contains the Nucleus "Terminal IO System" definitions and function.


\ingroup IFXOS_LAYER_NUCLEUS
*/

#ifdef __cplusplus
   extern "C" {
#endif

/* ============================================================================
   IFX Nucleus adaptation - Includes
   ========================================================================= */

/* ============================================================================
   IFX Nucleus adaptation - supported features
   ========================================================================= */

/** IFX Nucleus adaptation - User support "Terminal IO System" */
#ifndef IFXOS_HAVE_TERMIOS
#  define IFXOS_HAVE_TERMIOS                       1
#endif

/* ============================================================================
   IFX Nucleus adaptation - types
   ========================================================================= */

#ifdef __cplusplus
}
#endif
#endif      /* #ifdef NUCLEUS_PLUS */
#endif      /* #ifndef _IFXOS_NUCLEUS_TERMIOS_H */


