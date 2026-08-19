/******************************************************************************

               Copyright (c) 2018
            Lantiq Beteiligungs-GmbH & Co. KG

   For licensing information, see the file 'LICENSE' in the root folder of
   this software module.

******************************************************************************/

#ifndef _DRV_MEI_CPE_SAFEC_WRAPPER_H
#define _DRV_MEI_CPE_SAFEC_WRAPPER_H

#if defined SAFEC_SUPPORT && (SAFEC_SUPPORT == 1)
#include "safe_lib.h"
#define MEI_strncpy_s          strncpy_s
#else

#warning "Opensource Safe C library is not available!"

#include <stddef.h>   /* size_t */
static __inline__ size_t MEI_min(const size_t a, const size_t b)
{
   return a > b ? b : a;
}

#define MEI_strncpy_s(dest, destsz, src, srcsz)        strncpy(dest, src, MEI_min(destsz, srcsz))
#endif /* defined (SAFEC_SUPPORT) && (SAFEC_SUPPORT == 1) */

#endif /* _DRV_MEI_CPE_SAFEC_WRAPPER_H */