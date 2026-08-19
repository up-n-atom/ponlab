dnl Process this file with autoconf to produce a configure script.

AC_REVISION($Revision: 0.1 $)

AC_INIT([PON_NET_LIB],[2.3.0],[],[pon_net_lib])
AC_CONFIG_SRCDIR(src/Makefile.am)
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS(src/pon_net_config.h)
AM_INIT_AUTOMAKE([-Wno-portability tar-pax subdir-objects])

AC_PROG_CXX
ifdef([AC_PROG_CC_STDC], [AC_PROG_CC_STDC])
AC_LANG([C])
AM_PROG_CC_C_O
AC_PROG_LIBTOOL

#
# save the configure arguments
#
CONFIGURE_OPTIONS="$ac_configure_args"
AC_SUBST(CONFIGURE_OPTIONS)

AC_CHECK_HEADERS([getopt.h])
#AC_TYPE_LONG_LONG_INT

AH_TOP(
   [
#ifndef _pon_net_config_h
#define _pon_net_config_h
   ]
)

AH_BOTTOM(
   [
#endif
   ]
)

# check for safec lib 3.7 or 3.3
AC_SEARCH_LIBS(_memcpy_s_chk, safec safec-3.3,
   AC_DEFINE([HAVE_LIBSAFEC_3], [1], [safec lib V3.3 or 3.7 detected]))
AC_CHECK_FUNCS(sprintf_s sscanf_s)

dnl set libnl include path
DEFAULT_LIBNL_INCLUDE_PATH=''
AC_ARG_ENABLE(libnl-include,
   AS_HELP_STRING([--enable-libnl-include=-I/path/to/your/libnl/include],[Set the libnl include path]),
   [
    if test -n "$enableval"; then
       echo Set the libnl include path to $enableval
       AC_SUBST([LIBNL_INCLUDE_PATH],[$enableval])
    else
       echo Set the libnl include path to $DEFAULT_LIBNL_INCLUDE_PATH
       AC_SUBST([LIBNL_INCLUDE_PATH],[$DEFAULT_LIBNL_INCLUDE_PATH])
    fi
   ],
   [
      echo Set the libnl include path to $DEFAULT_LIBNL_INCLUDE_PATH
      AC_SUBST([LIBNL_INCLUDE_PATH],[$DEFAULT_LIBNL_INCLUDE_PATH])
   ]
)

dnl set libnl library path
DEFAULT_LIBNL_LIBRARY_PATH=''
AC_ARG_ENABLE(libnl-library,
   AS_HELP_STRING([--enable-libnl-library=-L/path/to/your/libnl],[set the libnl library path]),
   [
    if test -n "$enableval"; then
       echo Set the libnl library path to $enableval
       AC_SUBST([LIBNL_LIBRARY_PATH],[$enableval])
    else
       echo Set the libnl library path to $DEFAULT_LIBNL_LIBRARY_PATH
       AC_SUBST([LIBNL_LIBRARY_PATH],[$DEFAULT_LIBNL_LIBRARY_PATH])
    fi
   ],
   [
      echo Set the libnl library path to $DEFAULT_LIBNL_LIBRARY_PATH
      AC_SUBST([LIBNL_LIBRARY_PATH],[$DEFAULT_LIBNL_LIBRARY_PATH])
   ]
)

dnl set pon_adapter include path
DEFAULT_PON_ADAPTER_INCLUDE_PATH=''
AC_ARG_ENABLE(pon-adapter-include,
   AS_HELP_STRING([--enable-pon-adapter-include=-I/path/to/your/pon_adapter/include],[Set the pon_adapter include path]),
   [
    if test -n "$enableval"; then
       echo Set the pon_adapter include path to $enableval
       AC_SUBST([PON_ADAPTER_INCLUDE_PATH],[$enableval])
    else
       echo Set the pon_adapter include path to $DEFAULT_PON_ADAPTER_INCLUDE_PATH
       AC_SUBST([PON_ADAPTER_INCLUDE_PATH],[$DEFAULT_PON_ADAPTER_INCLUDE_PATH])
    fi
   ],
   [
      echo Set the pon_adapter include path to $DEFAULT_PON_ADAPTER_INCLUDE_PATH
      AC_SUBST([PON_ADAPTER_INCLUDE_PATH],[$DEFAULT_PON_ADAPTER_INCLUDE_PATH])
   ]
)

dnl set pon_adapter library path
DEFAULT_PON_ADAPTER_LIBRARY_PATH=''
AC_ARG_ENABLE(pon-adapter-library,
   AS_HELP_STRING([--enable-pon-adapter-library=-L/path/to/your/libadapter],[set the pon_adapter library path]),
   [
    if test -n "$enableval"; then
       echo Set the pon_adapter library path to $enableval
       AC_SUBST([PON_ADAPTER_LIBRARY_PATH],[$enableval])
    else
       echo Set the pon_adapter library path to $DEFAULT_PON_ADAPTER_LIBRARY_PATH
       AC_SUBST([PON_ADAPTER_LIBRARY_PATH],[$DEFAULT_PON_ADAPTER_LIBRARY_PATH])
    fi
   ],
   [
      echo Set the pon_adapter library path to $DEFAULT_PON_ADAPTER_LIBRARY_PATH
      AC_SUBST([PON_ADAPTER_LIBRARY_PATH],[$DEFAULT_PON_ADAPTER_LIBRARY_PATH])
   ]
)

dnl enable MCC driver
AM_CONDITIONAL(MCC_DRV_ENABLE, false)
AC_ARG_ENABLE(mcc-drv,
   AS_HELP_STRING([--enable-mcc-drv],[Enable MCC driver]),
   [
        if test "$enableval" = yes; then
            echo Enable MCC driver support
            AC_DEFINE([MCC_DRV_ENABLE], [1], [Enabled MCC driver support])
            AM_CONDITIONAL(MCC_DRV_ENABLE, true)
        fi
   ]
)

dnl set pon_mcc_drv include path
DEFAULT_PON_MCC_DRV_INCLUDE_PATH=''
AC_ARG_ENABLE(pon-mcc-drv-include,
   AS_HELP_STRING([--enable-pon-mcc-drv-include=-I/path/to/your/pon_mcc_drv/include],[Set the pon_mcc_drv include path]),
   [
    if test -n "$enableval"; then
       echo Set the pon_mcc_drv include path to $enableval
       AC_SUBST([PON_MCC_DRV_INCLUDE_PATH],[$enableval])
    else
       echo Set the pon_mcc_drv include path to $DEFAULT_PON_MCC_DRV_INCLUDE_PATH
       AC_SUBST([PON_MCC_DRV_INCLUDE_PATH],[$DEFAULT_PON_MCC_DRV_INCLUDE_PATH])
    fi
   ],
   [
      echo Set the pon_mcc_drv include path to $DEFAULT_PON_MCC_DRV_INCLUDE_PATH
      AC_SUBST([PON_MCC_DRV_INCLUDE_PATH],[$DEFAULT_PON_MCC_DRV_INCLUDE_PATH])
   ]
)

dnl set pon_mcc_drv library path
DEFAULT_PON_MCC_DRV_LIBRARY_PATH=''
AC_ARG_ENABLE(pon-mcc-drv-library,
   AS_HELP_STRING([--enable-pon-mcc-drv-library=-L/path/to/your/libponmcc],[set the pon_mcc_drv library path]),
   [
    if test -n "$enableval"; then
       echo Set the pon_mcc_drv library path to $enableval
       AC_SUBST([PON_MCC_DRV_LIBRARY_PATH],[$enableval])
    else
       echo Set the pon_mcc_drv library path to $DEFAULT_PON_MCC_DRV_LIBRARY_PATH
       AC_SUBST([PON_MCC_DRV_LIBRARY_PATH],[$DEFAULT_PON_MCC_DRV_LIBRARY_PATH])
    fi
   ],
   [
      echo Set the pon_mcc_drv library path to $DEFAULT_PON_MCC_DRV_LIBRARY_PATH
      AC_SUBST([PON_MCC_DRV_LIBRARY_PATH],[$DEFAULT_PON_MCC_DRV_LIBRARY_PATH])
   ]
)

dnl enable\disable tests build
AM_CONDITIONAL(TESTS_BUILD, false)
AC_ARG_ENABLE(tests-build,
   AS_HELP_STRING([--enable-tests-build], [enable unit tests build]),
   [
      if test "$enableval" = yes; then
         echo Enable unit tests build.
         AC_DEFINE([TESTS_BUILD], [1], [enable unit tests build])
         AM_CONDITIONAL(TESTS_BUILD, true)
      else
         echo Disable unit tests build.
      fi
   ],
   [
      echo Disable unit tests build.
   ]
)

dnl enable\disable Coverage testing
AM_CONDITIONAL(LCOV_ENABLE, false)
AC_ARG_ENABLE(lcov-build,
   AS_HELP_STRING([--enable-lcov-build], [enable coverage testing]),
   [
      if test "$enableval" = yes; then
         echo Enable coverage testing build.
         AC_DEFINE([LCOV_ENABLE], [1], [enable coverage testing])
         AM_CONDITIONAL(LCOV_ENABLE, true)
      else
         echo Disable coverage testing.
      fi
   ],
   [
      echo Disable coverage testing.
   ]
)

dnl TC_OFFLOAD enable\disable
AC_ARG_WITH(tc-offload,
   AS_HELP_STRING([--without-tc-offload], [build without TC-OFFLOAD]),
   [
      case $withval in
      0|no)
            AC_DEFINE([NO_TC_OFFLOAD], [1], [Build PON_NET_LIB without TC-OFFLOAD])
            AM_CONDITIONAL(NO_TC_OFFLOAD, true)
            ;;
      1|yes)
            AM_CONDITIONAL(NO_TC_OFFLOAD, false)
            ;;
      *)
            AM_CONDITIONAL(NO_TC_OFFLOAD, false)
            ;;
      esac
   ],
   [
      AM_CONDITIONAL(NO_TC_OFFLOAD, false)
   ]
)

dnl enable\disable PON Simulation
AM_CONDITIONAL(PON_NET_LIB_SIMULATOR, false)
AC_ARG_ENABLE(simulator,
   AS_HELP_STRING([--enable-simulator], [enable Simulator]),
   [
      if test "$enableval" = yes; then
         echo Enable Simulator
         AC_DEFINE([PON_NET_LIB_SIMULATOR], [1], [enable Simulator])
         AM_CONDITIONAL(PON_NET_LIB_SIMULATOR, true)
      else
         echo Disable Simulator
      fi
   ],
   [
      echo Disable Simulator
   ]
)

# Check for git and a git repo.
if test -e "$srcdir/.git"; then
        # Hash of latest tag
        latest_tag=`git -C "$srcdir" describe --tags --abbrev=0`

        # Number of commits until latest tag
        if test -n "$latest_tag"; then
                EXTRA_VERSION=`git -C "$srcdir" rev-list HEAD "^$latest_tag" --count`
        fi

        # Add the git commit hash or dirty suffix
        if test `git -C "$srcdir" diff-index -p HEAD | wc -l` -gt 0 ; then
                EXTRA_SUFFIX="dirty"
        else
                EXTRA_SUFFIX="g`git -C "$srcdir" rev-parse --short=7 HEAD`"
        fi
        EXTRA_VERSION="$EXTRA_VERSION-$EXTRA_SUFFIX"
fi

if test -n "$EXTRA_VERSION"; then
        AC_DEFINE_UNQUOTED([EXTRA_VERSION], ["$EXTRA_VERSION"], [extra version])
fi

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
