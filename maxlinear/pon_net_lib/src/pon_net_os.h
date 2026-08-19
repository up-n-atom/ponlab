/******************************************************************************
 *
 * Copyright (c) 2021 MaxLinear, Inc.
 * Copyright (c) 2018 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file
 * This file contains operation system specific definitions.
 */

#ifndef _PON_NET_OS_H
#define _PON_NET_OS_H

#include <stdlib.h>
#include <string.h>
#include "pon_net_config.h"

#ifdef LINUX
#  if defined(HAVE_LIBSAFEC_3)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wredundant-decls"
#    include <safe_lib.h>
#    include <safe_str_lib.h>
#    include <safe_mem_lib.h>
#    pragma GCC diagnostic pop
#  else
/* The safe_lib is mandatory for Linux. */
#    error Not using safe functions! The usage of safe_lib is mandatory
#  endif /* HAVE_LIBSAFEC_3 */
#endif /* LINUX */

/*
 * The libsafec3.3 and 3.7 do not support the second argument for these format
 * modifiers "%c, %s, and %["  and violates the C11 extension. Use the
 * legacy format with the safelibc.
 * https://github.com/rurban/safeclib/issues/68
 */
#if defined(HAVE_SSCANF_S) && defined(HAVE_LIBSAFEC_3)
#  define SSCANF_CHAR(var)	var
#  define SSCANF_STR(var, size)	var
#elif defined(HAVE_SSCANF_S)
/*
 * sscanf_s's %c conversion specifier requires an additional parameter
 * to check for the buffer size. In cases where %c is not used this macro
 * can be directly used.
 */
#  define SSCANF_CHAR(var)	var, 1
#  define SSCANF_STR(var, size)	var, size
#else
#  define sscanf_s sscanf
#  define SSCANF_CHAR(var)	var
#  define SSCANF_STR(var, size)	var
#endif /* HAVE_SSCANF_S */

#if !defined(HAVE_SPRINTF_S) && !defined(sprintf_s)
#define sprintf_s(buf, size, fmt, ...)\
	snprintf(buf, size, fmt, __VA_ARGS__)
#endif /* HAVE_SPRINTF_S */

#ifdef WIN32
/* Microsoft has no secure function for memset */
static inline int memset_s(void *p, size_t s1, int val, size_t s2)
{
	if (s1 < s2)
		return -EINVAL;
	memset(p, val, s2);
	return 0;
}

#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>

#define CLOCK_REALTIME 0

static inline
int clock_gettime(int mode, struct timespec *tv)
{
	struct _timeb timebuffer;

	_ftime64_s(&timebuffer);
	tv->tv_sec = timebuffer.time;
	tv->tv_nsec = 1000000L * timebuffer.millitm;
	return 0;
}

/* Windows has not strtok_r, but strtok_s uses the same arguments */
#ifndef strtok_r
#  define strtok_r(a, b, c) strtok_s(a, b, c)
#endif

/* Map strerror_r to strerror_s with adapting the order of arguments */
#define strerror_r(errno, buf, len) \
	strerror_s(buf, len, errno)

#include <stdarg.h>
#include <stdio.h>

/* Define asprintf as it is not available for WIN32 */
static inline int asprintf(char **strp, const char *fmt, ...)
{
	va_list args;
	int size, result;

	va_start(args, fmt);
	size = vsnprintf(NULL, 0, fmt, args);
	va_end(args);

	if (size < 0)
		return -1;

	*strp = malloc(size + 1);
	if (!*strp)
		return -1;

	va_start(args, fmt);
	result = vsnprintf(*strp, size + 1, fmt, args);
	va_end(args);

	return result;
}
#endif /* WIN32 */

#ifndef EOK
#define EOK (0)
#endif

#endif /* _PON_NET_OS_H */
