##
## File: configure.ac
##
## Source file for building configure with automake+autoconf
##

##############################################################################
## Copyright (c) 2009-2013 INSIDE Secure B.V. All Rights Reserved.
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 2 of the License, or
## any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program. If not, see <http://www.gnu.org/licenses/>.
##############################################################################

AC_INIT([SafeZone-CM-DDK], [2.3.1])

AC_CONFIG_AUX_DIR([.])
AM_INIT_AUTOMAKE([foreign no-define])

AC_PROG_CC
AC_PROG_RANLIB
AM_PROG_AS

AC_CANONICAL_HOST

AC_C_CONST
AC_C_RESTRICT
AC_C_INLINE

AC_CHECK_SIZEOF(void *)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(long long)

ENABLE_TARGET_OPT
AM_CONDITIONAL([ENABLE_VERSATILE], [test "X$enable_target" = "Xversatile"])
AM_CONDITIONAL([ENABLE_CUSTOM],    [test "X$enable_target" = "Xcustom"])

AC_ARG_ENABLE([cm],
    [AS_HELP_STRING([--enable-cm=version], [Crypto Module Version (1=EIP-122; 2=EIP-123)])],
    [case $enableval in
        1 | 2 ) : ;;
        *) AC_MSG_ERROR([--enable-cm: unknown version: "$enableval"]) ;;
    esac],
    [enable_cm=2]
)
AM_CONDITIONAL([WITH_CM_HW1], [test "X$enable_cm" = "X1"])
AM_CONDITIONAL([WITH_CM_HW2], [test "X$enable_cm" = "X2"])

ENABLE_YES_NO_OPT([stackmeasurement])
AM_CONDITIONAL([ENABLE_STACK], [test "X$enable_stackmeasurement" = "Xyes"])

ENABLE_YES_NO_OPT([heapmeasurement])
AM_CONDITIONAL([ENABLE_HEAP], [test "X$enable_heapmeasurement" = "Xyes"])

ENABLE_DEBUG_OPT
AM_CONDITIONAL([ENABLE_DEBUG], [test "X$enable_debug" = "Xyes"])

ENABLE_YES_NO_OPT([coverage])
AM_CONDITIONAL([ENABLE_COVERAGE], [test "X$enable_coverage" = "Xyes"])

ENABLE_YES_NO_OPT([polling])
AM_CONDITIONAL([ENABLE_POLLING], [test "X$enable_polling" = "Xyes"])

AM_CONDITIONAL([ENABLE_GCC_STRICT_WARNINGS],
               [test "X$GCC_STRICT_WARNINGS" = "Xyes"])
if test "X$GCC_STRICT_WARNINGS" = "Xyes"
then
    AC_MSG_NOTICE([Enabling stricter warnings.])
fi

safezone_platform=unknown
case "$host" in
i*86-*) safezone_platform=x86 ;;
esac
AM_CONDITIONAL([ENABLE_ASM_X86], [test "X$safezone_platform" = "Xx86"])

safezone_config_name="safezone-config"
safezone_config_libs="-lcal -lpthread -lrt -lgcov"
test x$enable_coverage = xyes && CFLAGS="$CFLAGS -fprofile-arcs -ftest-coverage"

AC_SUBST(safezone_config_name)
AC_SUBST(safezone_config_libs)

AC_CONFIG_FILES(Makefile)
AC_CONFIG_FILES(safezone-config-template)

AC_OUTPUT

# end of file configure.ac
