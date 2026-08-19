#
#
#  Copyright 2014-2015 Lantiq Deutschland GmbH
#  Copyright 2015-2016 Lantiq Beteiligungs-GmbH & Co.KG
#  Copyright 2016-2017 Intel Corporation.
#
#  For licensing information, see the file 'LICENSE' in the root folder of
#  this software module.
#
#

AC_PREREQ(2.59)
AC_COPYRIGHT([Copyright (c) 2014-2017, Intel Corporation
For licensing information please see the file 'LICENSE' in the root folder of
this software module.])

AC_INIT([SDD mailbox driver],[1.4.0.0],[Intel Corporation],[drv_sdd_mbx])
AC_CONFIG_SRCDIR(src/drv_sdd_mbx_linux.c)
AM_INIT_AUTOMAKE([-Wno-portability tar-pax])

m4_pattern_allow([^AM_ACLOCAL_INCLUDE$])
AM_ACLOCAL_INCLUDE(m4)

# save the configure arguments
# remove the install dir from the distcheck configure line to avoid that the
# installed binaries are deleted on distcheck. The regex will remove
# --prefix and all subsequent "non-space" chars from the configure options
# as well as the internal features.
# Note: The escaped \' are important.
# Note: Don't lin* - it would match also --enable-linux-26 (!)
CONFIGURE_OPTIONS=''
for c_arg in $ac_configure_args; do
         case $c_arg in
            \'--prefix=* | \'--enable-el_debug\' | \'--enable-eval\')
            #echo "skipping configure option for distcheck: $c_arg"
            ;;
         *)
            CONFIGURE_OPTIONS="$CONFIGURE_OPTIONS $c_arg"
            ;;
         esac
done
AC_SUBST(CONFIGURE_OPTIONS)

AC_CONFIG_HEADER(drv_sdd_mbx_config.h)

dnl enable debugging mode
DEBUG_CHECK

if [[ "$CFLAGS" == "" ]] ; then
   CFLAGS="-O2"
fi

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_RANLIB

AH_TOP(
[/******************************************************************************

  Copyright 2014-2015 Lantiq Deutschland GmbH
  Copyright 2015-2016 Lantiq Beteiligungs-GmbH & Co.KG
  Copyright 2016-2017 Intel Corporation.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/
#ifndef _DRV_SDD_MBX_CONFIG_H
#define _DRV_SDD_MBX_CONFIG_H]
)

AH_BOTTOM([
#endif /* _DRV_SDD_MBX_CONFIG_H */
])

AC_DEFINE_UNQUOTED([SDD_MBX_CONFIGURE_STR], ["$ac_configure_args"], [configure options string])

dnl set kernel include and build directories
dnl enable linux kernel 2.6.x support (Make sure AC_PROG_CC is defined before to set $CC!)
KERNEL_CHECK_CONFIG

dnl enable compiler warnings
WARNINGS_CHECK

dnl enable warnings as error
WERROR_CHECK

dnl set additional (device specific) CFLAGS
WITH_CFLAGS_CHECK

dnl Set lib_ifxos include path
LIBIFXOS_CHECK_CONFIG_INCL

dnl enable kernel module
WITH_KERNEL_MODULE_CHECK(
   [yes], # default
   [INTERRUPTS_CHECK(
      [enable], # default
      [
         AC_DEFINE([SDD_MBX_INTERRUPTS],[1],[enable interrupts])
      ]
   )]
)

EVENT_LOGGER_CHECK_CONFIG

AC_DEFINE_UNQUOTED([WITH_CLFAGS_CONFIGURE_OPTION],[$CFLAGS],[environment and device specific CFLAGS passed to configure])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])

AC_OUTPUT

echo \
"------------------------------------------------------------------------
SDD mailbox driver configuration:

  Source code location: ${srcdir}
  Compiler:             ${CC}
  Compiler flags:       ${CFLAGS}
  Host System Type:     ${host}
  Install path:         ${prefix}

  IFXOS include path:   ${IFXOS_INCL_PATH}
  KERNEL_INCL_PATH:     ${KERNEL_INCL_PATH}
------------------------------------------------------------------------"
