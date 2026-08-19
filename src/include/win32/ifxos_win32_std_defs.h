/****************************************************************************

         Copyright (c) 2021 MaxLinear, Inc.
         Copyright (c) 2009 Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*****************************************************************************/
#ifndef _IFXOS_WIN32_STD_DEFS_H
#define _IFXOS_WIN32_STD_DEFS_H

/** \file
   Function prototypes from standard headers for the IFXOS package.
*/

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>      /* sprintf */
#include <stdlib.h>     /* strtoul */
#include <string.h>     /* strtok_r, strcmp, strncmp, strlen, memcpy, memset, NULL */
#include <ctype.h>      /* isspace */
#include <errno.h>      /* errno */

/* Windows has not strtok_r, but strtok_s uses the same arguments */
#define strtok_r(a,b,c) strtok_s(a,b,c)

/* mapping for non-standard function declarations by Microsoft */
#define snprintf _snprintf

#ifdef __cplusplus
/* extern "C" */
}
#endif

#endif      /* #define _IFXOS_WIN32_STD_DEFS_H */

