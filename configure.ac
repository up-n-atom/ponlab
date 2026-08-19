#
#                      Copyright (c) 2009-2013, 2015, 2016
#                        Lantiq Beteiligungs-GmbH & Co.KG
#                             http://www.lantiq.com
#
#  For licensing information, see the file 'LICENSE' in the root folder of
#  this software module.
#
#


AC_PREREQ(2.59)
AC_COPYRIGHT([Copyright (C) 2009-2013, 2015, 2016, Lantiq Beteiligungs-GmbH & Co.KG
For licensing information please see the file 'LICENSE' in the root folder of
this software module.])

AC_INIT([TAPI Line Testing library],[0.6.0.5],[Lantiq Beteiligungs-GmbH & Co.KG],[lib_tapi_nlt_cpe])
AC_CONFIG_SRCDIR([src/lib_tapi_lt_gr909.c])
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
		\'--prefix=*)
			#echo "skipping configure option for distcheck: $c_arg"
			;;
		*)
			CONFIGURE_OPTIONS="$CONFIGURE_OPTIONS $c_arg"
			;;
	esac
done
AC_SUBST(CONFIGURE_OPTIONS)

AC_CONFIG_HEADER(lib_tapi_nlt_config.h)

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_RANLIB

AH_TOP(
[/******************************************************************************

                      Copyright (c) 2009-2013, 2015, 2016
                        Lantiq Beteiligungs-GmbH & Co.KG
                             http://www.lantiq.com

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

  ****************************************************************************/
#ifndef _LIB_TAPI_NLT_CONFIG_H_
#define _LIB_TAPI_NLT_CONFIG_H_]
)

AH_BOTTOM([
#endif /* _LIB_TAPI_NLT_CONFIG_H_ */
])

DISTRIBUTION_TYPE_CHECK

#AC_DEFINE_UNQUOTED([LIB_TAPI_NLT_CONFIGURE_STR], ["$ac_configure_args"], [configure options string])

KERNEL_2_6_CHECK

KERNEL_INCL_CHECK

# altough the name, it can be used here to check for driver TAPI includes
DRV_TAPI_INCL_CHECK_FOR_LL_DRV

WARNINGS_CHECK([CFLAGS="$CFLAGS -Wunused -Wpointer-arith"])

# enable warnings as error
WERROR_CHECK

# set additional (device specific) CFLAGS
WITH_CFLAGS_CHECK

# ifxos includes for ifx_types.h if no IFXOS support
LIBIFXOS_CHECK_CONFIG_INCL
IFXOS_ENABLE(disable,
  [AC_DEFINE([LIB_TAPI_NLT_IFXOS_SUPPORT],[1],[enable support of ifxos library])])


AM_CONDITIONAL(LIB_TAPI_NLT_DXT, false)
AM_CONDITIONAL(LIB_TAPI_NLT_DXS, false)
AM_CONDITIONAL(LIB_TAPI_NLT_VMMC, false)
AM_CONDITIONAL(LIB_TAPI_NLT_VINCPE, false)
AM_CONDITIONAL(LIB_TAPI_NLT_S220, false)

# Enable Line testing depending on device
DXT_DEVICE_ENABLE(
    [AM_CONDITIONAL(LIB_TAPI_NLT_DXT, true)
     AC_DEFINE([LIB_TAPI_NLT_DXT_SUPPORT],[1], [enable Line Testing for DXT device])])
DXS_DEVICE_ENABLE(
    [AM_CONDITIONAL(LIB_TAPI_NLT_DXS, true)
     AC_DEFINE([LIB_TAPI_NLT_DXS_SUPPORT],[1], [enable Line Testing for DXS device])])
VMMC_DEVICE_ENABLE(
    [AM_CONDITIONAL(LIB_TAPI_NLT_VMMC, true)
     AC_DEFINE([LIB_TAPI_NLT_VMMC_SUPPORT],[1], [enable Line Testing for VMMC device])])
VINCPE_DEVICE_ENABLE(
    [AM_CONDITIONAL(LIB_TAPI_NLT_VINCPE, true)
     AC_DEFINE([LIB_TAPI_NLT_VINCPE_SUPPORT],[1], [enable Line Testing for vinetic-cpe device])])
S220_DEVICE_ENABLE(
    [AM_CONDITIONAL(LIB_TAPI_NLT_S220, true)
     AC_DEFINE([LIB_TAPI_NLT_S220_SUPPORT],[1], [enable Line Testing for S220 device])])

dnl enable GR909
AC_MSG_CHECKING(for GR909 support)
AC_ARG_ENABLE(gr909,
   AS_HELP_STRING(
      [--enable-gr909],
      [enable/disable support for Line Testing GR909]
      ),
   [
      if test $enableval = 'no'; then
         AC_MSG_RESULT([disabled])
      else
         AC_MSG_RESULT([enabled])
         AC_DEFINE([LIB_TAPI_NLT_GR909],[1],[enable support for Line Testing GR909])
      fi
   ],
   [
      AC_MSG_RESULT([enabled])
      AC_DEFINE([LIB_TAPI_NLT_GR909],[1],[enable support for Line Testing GR909])
   ]
)

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])

AC_OUTPUT

echo \
"------------------------------------------------------------------------
Configuration:

  System path:          ${KERNEL_INCL_PATH}
  Compiler:             ${CC}
  Compiler flags:       ${CFLAGS}
  Host System Type:     ${host}
  Install path:         ${prefix}
------------------------------------------------------------------------"


