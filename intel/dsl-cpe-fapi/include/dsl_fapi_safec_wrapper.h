/* Copyright 2017, Intel Corporation */
/******************************************************************************

                          Copyright (c) 2015-2018
                     Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _FAPI_SAFEC_WRAPPER_H
#define _FAPI_SAFEC_WRAPPER_H

#if defined (SAFEC_SUPPORT) && (SAFEC_SUPPORT == 1)
#include "safe_lib.h"
#define fapi_dsl_memcpy_s(dest, destsz, src, srcsz) memcpy_s(dest, destsz, src, srcsz)
#else
#warning "Opensource Safe C library is not available!"

#include <stddef.h> /* size_t */
static inline size_t safec_wrapper_min(size_t a, size_t b)
{
  return a > b ? b : a;
}

#define fapi_dsl_memcpy_s(dest, destsz, src, srcsz) memcpy(dest, src, safec_wrapper_min(destsz,srcsz))
#endif /* defined (SAFEC_SUPPORT) && (SAFEC_SUPPORT == 1) */

#endif /* _FAPI_SAFEC_WRAPPER_H */

