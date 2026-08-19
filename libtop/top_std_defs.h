/******************************************************************************

			   Copyright (c) 2013 - 2015
			Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

/**
   \file top_std_defs.h
   This is a common header file, providing the globally used type definitions.
*/
#ifndef _top_std_defs_h
#define _top_std_defs_h

#if defined(__LINUX__) && !defined(__KERNEL__)
/* include __WORDSIZE */
#  include <bits/wordsize.h>
#endif

#if defined(__KERNEL__)
/* use linux kernel types */
#  include <linux/kernel.h>
#endif				/* __KERNEL__ */

#if !defined(__KERNEL__)

#if defined(__GNUC__) || (defined(__STDC_VERSION__) && \
			    (__STDC_VERSION__ >= 199901L))
/* use standard C99 types */
#  include <stdint.h>
#  ifndef ECOS
#    include <stdbool.h>
#  endif
#endif				/* __STDC_VERSION__ */

#if (!defined(__STDC_VERSION__) || \
			    (__STDC_VERSION__ < 199901L)) && (!defined(__GNUC__))
/* standard C99 types not available */

/** C99 type */
typedef signed char int8_t;
/** C99 type */
typedef unsigned char uint8_t;
/** C99 type */
typedef signed short int16_t;
/** C99 type */
typedef unsigned short uint16_t;
/** C99 type */
typedef signed int int32_t;
/** C99 type */
typedef unsigned int uint32_t;

#  if defined(__WORDSIZE) && (__WORDSIZE == 64)
/** C99 type */
typedef unsigned long int uint64_t;
/** C99 type */
typedef long int intptr_t;
/** C99 type */
typedef unsigned long int uintptr_t;
#  else
/** C99 type */
typedef unsigned long long int uint64_t;
/** C99 type */
typedef int intptr_t;
/** C99 type */
typedef unsigned int uintptr_t;
#  endif
#  if !defined(__cplusplus) && !defined(bool)
#     ifndef SWIG
/** C99 type */
typedef char bool;
#     endif
#  define false 0
#  define true 1
#  endif
#endif

#endif /* !defined(__KERNEL__) */

#ifndef UINT64_MAX
#   define UINT64_MAX   (((uint64_t)(-1)))
#endif

#ifndef UINT32_MAX
#   define UINT32_MAX   (((uint32_t)(-1)))
#endif

#ifndef UINT16_MAX
#   define UINT16_MAX   (((uint16_t)(-1)))
#endif

#if defined(LINUX)
#   ifdef __KERNEL__
#      include <linux/kernel.h>
#      include <linux/string.h>	/* memset */
#      include <asm/div64.h>	/* do_div */
#   else
#      include <stdio.h>
#      include <stdlib.h>
#      include <string.h>
#      include <ctype.h>
#      include <errno.h>
#   endif
#elif defined(ECOS)
#    include <stdio.h>
#    include <stdlib.h>
#    include <string.h>
#    include <ctype.h>
#    include <errno.h>
#else
#   include <stddefs.h>
#   include <stdint.h>
#   include <stdlib.h>
#   include <string.h>
#   include <stdio.h>
#   include <stdarg.h>
#   include <ioctl.h>
#endif
#if defined(ECOS)
#   include <pkgconf/system.h>
#   include <cyg/kernel/kapi.h>  /* All the kernel specific stuff like cyg_flag_t, ... */
#endif

/** C99 type */
#ifndef HAVE_ULONG_T
#define HAVE_ULONG_T
typedef unsigned long int ulong_t;
#endif

#ifndef ULONG_MAX
#   define ULONG_MAX   (~((unsigned long)0))
#endif

#ifdef WIN32
#  define inline __inline
#endif

#ifndef INLINE
#  define INLINE inline
#endif

#ifdef __cplusplus
#  ifndef EXTERN_C_BEGIN
/** Extern C block begin macro */
#     define EXTERN_C_BEGIN extern "C" {
#  endif
#  ifndef EXTERN_C_END
/** Extern C block end macro */
#     define EXTERN_C_END }
#  endif
#else
#  ifndef EXTERN_C_BEGIN
/** Extern C block begin macro */
#     define EXTERN_C_BEGIN
#  endif
#  ifndef EXTERN_C_END
/** Extern C block end macro */
#     define EXTERN_C_END
#  endif
#endif

#endif
