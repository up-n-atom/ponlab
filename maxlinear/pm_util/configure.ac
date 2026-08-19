dnl Process this file with autoconf to produce a configure script.

AC_REVISION($Revision: 1.10 $)

AC_INIT([PM_UTIL Intel Power Management Utility],[4.4.2],[],[pm_util])

AC_CONFIG_AUX_DIR([build-aux])

AC_PROG_CC
ifdef([AC_PROG_CC_STDC], [AC_PROG_CC_STDC])
AC_LANG([C])
AC_PROG_RANLIB

AC_CONFIG_SRCDIR(src/pm_util.c)
AM_INIT_AUTOMAKE([tar-pax])
AM_PROG_CC_C_O
AC_PROG_CXX

AC_CONFIG_HEADER(src/pm_util_config.h)
AH_TOP([
/*
 *  Copyright 2020 MaxLinear, Inc.
 *  Copyright 2016 - 2020 Intel Corporation
 *  Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 *  Copyright 2009 - 2014 Lantiq Deutschland GmbH
 *  Copyright 2007 - 2008 Infineon Technologies AG
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 */
#ifndef _LTQ_PM_UTIL_CONFIG_H
#define _LTQ_PM_UTIL_CONFIG_H])
AH_BOTTOM([
#endif /* _LTQ_PM_UTIL_CONFIG_H */])

#
# save the configure arguments
#
CONFIGURE_OPTIONS="$ac_configure_args"
AC_SUBST(CONFIGURE_OPTIONS)

if test -n "$ARCH" ; then
    echo "Set the kernel architecture to $ARCH"
    AC_SUBST([KERNEL_ARCH],[$ARCH])
else
    AC_MSG_ERROR([Kernel architecture not set!])
fi

AC_CHECK_TYPES([ulong_t])


dnl Enable debugging mode
AM_CONDITIONAL(DEBUG, false)
AC_ARG_ENABLE(debug,
    AS_HELP_STRING(
        [--enable-debug],
        [enable debug messages]
    ),
    [
        AC_MSG_RESULT(enable Debug Symbols)
        AM_CONDITIONAL(DEBUG, true)
    ]
)
 
dnl Enable puma support
AM_CONDITIONAL(PUMA_SUPPORT, false)
AC_ARG_ENABLE(puma_support,
    AS_HELP_STRING(
        [--enable-puma-support],
        [enable puma support macro]
    ),
    [
        AC_MSG_RESULT(enable puma support)
        AM_CONDITIONAL(PUMA_SUPPORT, true)
    ]
)
 
dnl Option used to set additional (device specific) CFLAGS
dnl -------------------------------------------------
dnl
AC_DEFUN([WITH_CFLAGS_CHECK],
[
	AC_MSG_CHECKING(for additional CFLAGS)
	AC_ARG_WITH(cflags,
		AS_HELP_STRING(
			[--with-cflags=val],
			[pass additional (device specific) CFLAGS, not required for Linux 2.6]
		),
		[
			CFLAGS="$CFLAGS $withval"
			AC_MSG_RESULT([$withval])
		],
		[
			AC_MSG_RESULT([not set (default), set with --with-cflags])
		]
	)
])

 
dnl Set kernel include path (...)
DEFAULT_KERNEL_INCL_PATH='.'
AC_ARG_ENABLE(kernelincl,
   AS_HELP_STRING([--enable-kernelincl=x],[Set the target kernel include path
   ]),
   [
    if test -n $enableval; then
       echo Set the kernel include path $enableval
       AC_SUBST([KERNEL_INCL_PATH],[$enableval])
    else
       echo Set the default kernel include path $DEFAULT_KERNEL_INCL_PATH
       AC_SUBST([KERNEL_INCL_PATH],[$DEFAULT_KERNEL_INCL_PATH])
    fi
   ],
   [
       echo Set the default kernel include path $DEFAULT_KERNEL_INCL_PATH
       AC_SUBST([KERNEL_INCL_PATH],[$DEFAULT_KERNEL_INCL_PATH])
   ]
)

dnl Set kernel build path
AC_ARG_ENABLE(kernelbuild,
   AS_HELP_STRING(--enable-kernelbuild=x,Set the target kernel build path (only for kernel 2.6.x)),
   [
      if test -e $enableval/include/linux/autoconf.h -o -e $enableval/include/generated/autoconf.h; then
         AC_SUBST([KERNEL_BUILD_PATH],[$enableval])
      else
         AC_MSG_ERROR([The kernel build directory is not valid or not configured!])
      fi
   ],
   [
      if test -z $KERNEL_BUILD_PATH; then
         # assume kernel was build in source dir...
         AC_SUBST([KERNEL_BUILD_PATH],[$KERNEL_INCL_PATH/..])
      fi
   ]
)

dnl set additional (device specific) CFLAGS
WITH_CFLAGS_CHECK

AC_DEFINE_UNQUOTED([WITH_CLFAGS_CONFIGURE_OPTION],[$CFLAGS],[environment and device specific CFLAGS passed to configure])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT

echo \
"------------------------------------------------------------------------
pm_util configuration:

  Source code location:          ${srcdir}
  CROSS_COMPILE:                 ${CROSS_COMPILE}
  Compiler:                      ${CC}
  Compiler flags:                ${CFLAGS}
  Host System Type:              ${host}
  Install path:                  ${prefix}

  KERNEL_INCL_PATH:              ${KERNEL_INCL_PATH}
  KERNEL_BUILD_PATH:             ${KERNEL_BUILD_PATH}
------------------------------------------------------------------------" 
