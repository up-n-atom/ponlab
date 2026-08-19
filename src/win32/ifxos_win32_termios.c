/****************************************************************************

         Copyright (c) 2021 MaxLinear, Inc.
         Copyright (c) 2009 Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*****************************************************************************/

#if defined(WIN32) && !defined(NUCLEUS_PLUS)

/** \file
   This file contains the IFXOS Layer implementation for Win32 -
   Terminal IO System.
*/

/* ============================================================================
   IFX Win32 adaptation - Global Includes
   ========================================================================= */
#include "ifx_types.h"
#include "ifxos_termios.h"

/* ============================================================================
   IFX Win32 adaptation - Terminal IO System
   ========================================================================= */
/** \addtogroup IFXOS_TERMIOS_WIN32_APPL
@{ */

#if ( defined(IFXOS_HAVE_TERMIOS) && (IFXOS_HAVE_TERMIOS == 1) )
/**
   Win32 - Disable the local echo of the console.
*/
IFX_void_t IFXOS_EchoOff (void)
{
}

/**
   Win32 - Enable the local echo of the console.
*/
IFX_void_t IFXOS_EchoOn (void)
{
}

/**
   Win32 - Enable the console line mode.
   In this mode the input from the device is available only after receiving NEWLINE .
   This allows to modify the command line until the Enter key is pressed.
*/
IFX_void_t IFXOS_KeypressSet (void)
{
}

/**
   Win32 - Disable the console line mode.
   Please refer to \ref IFXOS_KeypressSet .
*/
IFX_void_t IFXOS_KeypressReset (void)
{
}
#endif      /* #if ( defined(IFXOS_HAVE_TERMIOS) && (IFXOS_HAVE_TERMIOS == 1) ) */

/** @} */

#endif      /* #ifdef WIN32 */

