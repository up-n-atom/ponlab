/******************************************************************************
 *
 * Copyright (c) 2021 MaxLinear, Inc.
 * Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#ifndef _FAPI_PON_OS_H_
#define _FAPI_PON_OS_H_

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef HAVE_CONFIG_H
#  include "pon_config.h"
#endif

#ifdef WIN32
#  include <io.h>
#  define PON_RDONLY _O_RDONLY
#  define PON_RDWR _O_RDWR
#  define pon_open _open
static inline
int pon_pread(int fd, void *buf, size_t count, off_t offset)
{
	off_t err;

	err = _lseek(fd, offset, SEEK_SET);
	if (err < 0)
		return err;

	return _read(fd, buf, count);
}

static inline
int pon_pwrite(int fd, const void *buf, size_t count, off_t offset)
{
	off_t err;

	err = _lseek(fd, offset, SEEK_SET);
	if (err < 0)
		return err;

	return _write(fd, buf, count);
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

#ifdef WIN64
#define _AMD64_
#else
#define _X86_
#endif
#include <synchapi.h>

static inline
void sleep(unsigned int seconds)
{
	/* WIN API Sleep takes milliseconds */
	Sleep(seconds * 1000);
}

static inline
void usleep(unsigned long microseconds)
{
	/*
	 * WIN API Sleep takes milliseconds
	 * so this function will round the microseconds
	 * to wait at least the time provided as parameter.
	 */
	Sleep((microseconds / 1000) + 1);
}
#  define pon_close _close
/* Microsoft has no secure function for memset */
#  define memset_s(p, s1, v, s2) memset(p, v, s2)
/* Windows has not strtok_r, but strtok_s uses the same arguments */
#ifndef strtok_r
#  define strtok_r(a, b, c) strtok_s(a, b, c)
#endif
/* Microsoft has no secure function for memcmp.
 * Returning the result is different from memcmp.
 */
static inline int
memcmp_s(const void *p, size_t s1, const void *v, size_t s2, int *pret)
{
	if (!p || !v)
		return 400; /*ESNULLP;*/
	if (!s1 || !s2)
		return 401; /*ESZEROL*/
	if (s1 < s2)
		return 406; /*ESNOSPC*/

	*pret = memcmp(p, v, s2);
	return 0;
}

#  define HAVE_MEMCPY_S
#  define HAVE_STRNCAT_S
#  define HAVE_STRNLEN_S
#  define HAVE_SOPEN_S
#  define HAVE_SSCANF_S
#  define HAVE_SPRINTF_S
#  define HAVE_STRERROR_S

/* dummy version definition for WIN simulation */
#  define PON_VER_MAJOR 2
#  define PON_VER_MINOR 3
#  define PON_VER_STEP 4
#  define PACKAGE_VERSION "2.4.1"

#endif
#ifdef LINUX
#  include <unistd.h>
#  define PON_RDONLY O_RDONLY
#  define PON_RDWR O_RDWR
#  define pon_open open
#  define pon_pread pread
#  define pon_pwrite pwrite
#  define pon_close close
#  if defined(HAVE_LIBSAFEC_3)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wredundant-decls"
#    include <safe_lib.h>
#    include <safe_str_lib.h>
#    include <safe_mem_lib.h>
#    pragma GCC diagnostic pop
#    define HAVE_MEMCPY_S
#    define HAVE_STRNCAT_S
#    define HAVE_STRNLEN_S
#  else
/*
 * The safe_lib is mandatory for Linux.
 * Macros are used in simulation code only as long the
 * safe_lib is not available for it.
 */
#error safec_lib not available but safe functions are mandatory
#  endif /* HAVE_LIBSAFEC_3 */
#endif /* LINUX */

#ifndef EOK
#define EOK (0)
#endif

#ifndef RSIZE_MAX_STR
#define RSIZE_MAX_STR (32UL << 10)
#endif

/*
 * The libsafec3.3 does not support the second argument for these format
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
#endif

#if !defined(HAVE_SPRINTF_S) && !defined(sprintf_s)
#define sprintf_s(buf, size, fmt, ...)\
	snprintf(buf, size, fmt, __VA_ARGS__)
#endif /* HAVE_SPRINTF_S */

static inline char *pon_strerr(int err, char *buf, size_t buflen)
{
	char *errstr = buf;

#if defined(HAVE_STRERROR_S) || defined(HAVE__STRERROR_S_CHK)
	if (strerror_s(buf, buflen, err))
		errstr = NULL;
#elif defined(__GLIBC__)
	errstr = strerror_r(err, buf, buflen);
#else
	if (strerror_r(err, buf, buflen))
		errstr = NULL;
#endif
	if (errstr)
		return errstr;

	sprintf_s(buf, buflen, "%s", "unknown error");
	return buf;
}

#endif /* _FAPI_PON_OS_H_ */
