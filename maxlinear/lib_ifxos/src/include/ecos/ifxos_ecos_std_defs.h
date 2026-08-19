/****************************************************************************

         Copyright (c) 2021 MaxLinear, Inc.
         Copyright (c) 2009 Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*****************************************************************************/
#ifndef _IFXOS_ECOS_STD_DEFS_H
#define _IFXOS_ECOS_STD_DEFS_H

/** \file
   Function proto-types for generic OS processing of the IFXOS based SW.
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

/* #define strtok_r(a,b,c) strtok(a,b) */

#ifdef __cplusplus
/* extern "C" */
}
#endif

#endif      /* #define _IFXOS_ECOS_STD_DEFS_H */

