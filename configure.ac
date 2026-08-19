dnl Process this file with autoconf to produce a configure script.

AC_REVISION($Revision: 0.1 $)

AC_INIT([PON library],[2.4.1],[http://www.maxlinear.com],[pon_lib])
AC_CONFIG_SRCDIR(src/Makefile.am)
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wno-portability tar-pax subdir-objects])
AC_CONFIG_HEADERS(src/pon_config.h)
AC_PROG_CC
ifdef([AC_PROG_CC_STDC], [AC_PROG_CC_STDC])
AC_LANG([C])
AM_PROG_CC_C_O
AC_PROG_LIBTOOL

AX_SPLIT_VERSION
AC_DEFINE_UNQUOTED([PON_VER_MAJOR], [$AX_MAJOR_VERSION], [Major version])
AC_DEFINE_UNQUOTED([PON_VER_MINOR], [$AX_MINOR_VERSION], [Minor version])
AC_DEFINE_UNQUOTED([PON_VER_STEP], [$AX_POINT_VERSION], [Build version])

#
# save the configure arguments
#
CONFIGURE_OPTIONS="$ac_configure_args"
AC_SUBST(CONFIGURE_OPTIONS)

AC_CHECK_HEADERS([getopt.h])
#AC_TYPE_LONG_LONG_INT

AH_TOP(
   [
#ifndef _pon_config_h
#define _pon_config_h
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

AC_CHECK_FUNCS(sprintf_s)
AC_CHECK_FUNCS(sscanf_s)
AC_CHECK_FUNCS(strerror_s)
AC_CHECK_FUNCS(_strerror_s_chk)

dnl set cli include path
DEFAULT_CLI_INCLUDE_PATH=''
AC_ARG_ENABLE(cli-include,
	AS_HELP_STRING([--enable-cli-include=-I/path/to/your/libcli/src/include],[Set the libcli.a include path.]),
	[
		if test -n "$enableval"; then
			echo Set the libcli.a include path $enableval
			AC_SUBST([CLI_INCLUDE_PATH],[$enableval])
		else
			echo -e Set the default libcli.a include path $DEFAULT_CLI_INCLUDE_PATH
			AC_SUBST([CLI_INCLUDE_PATH],[$DEFAULT_CLI_INCLUDE_PATH])
		fi
	],
	[
		echo -e Set the default libcli.a include path $DEFAULT_CLI_INCLUDE_PATH
		AC_SUBST([CLI_INCLUDE_PATH],[$DEFAULT_CLI_INCLUDE_PATH])
	]
)

dnl set cli library path
DEFAULT_CLI_LIBRARY_PATH='.'
AC_ARG_ENABLE(cli-library,
	AS_HELP_STRING([--enable-cli-library=/path/to/your/libcli/src] , [Set the libcli.a library path. In this location the libcli.a should be found]),
	[
		if test -n "$enableval"; then
			echo Set the libcli.a library path to $enableval
			AC_SUBST([CLI_LIBRARY_PATH],[$enableval])
		else
			echo Set the libcli.a library path $DEFAULT_CLI_LIBRARY_PATH
			AC_SUBST([CLI_LIBRARY_PATH],[$DEFAULT_CLI_LIBRARY_PATH])
		fi
	],
	[
		echo Set the libcli.a library path $DEFAULT_CLI_LIBRARY_PATH
		AC_SUBST([CLI_LIBRARY_PATH],[$DEFAULT_CLI_LIBRARY_PATH])
	]
)

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

dnl set pon_mbox_drv include path
DEFAULT_MBOXDRV_INCLUDE_PATH=''
AC_ARG_ENABLE(mboxdrv-include,
   AS_HELP_STRING([--enable-mboxdrv-include=-I/path/to/your/mboxdrv/include],[Set the pon_mbox_drv include path]),
   [
    if test -n "$enableval"; then
       echo Set the pon_mbox_drv include path to $enableval
       AC_SUBST([MBOXDRV_INCLUDE_PATH],[$enableval])
    else
       echo Set the pon_mbox_drv include path to $DEFAULT_MBOXDRV_INCLUDE_PATH
       AC_SUBST([MBOXDRV_INCLUDE_PATH],[$DEFAULT_MBOXDRV_INCLUDE_PATH])
    fi
   ],
   [
      echo Set the pon_mbox_drv include path to $DEFAULT_MBOXDRV_INCLUDE_PATH
      AC_SUBST([MBOXDRV_INCLUDE_PATH],[$DEFAULT_MBOXDRV_INCLUDE_PATH])
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

dnl enable\disable build with PON Adapter
AM_CONDITIONAL(INCLUDE_PON_ADAPTER, false)
AC_ARG_ENABLE(pon-adapter,
   AS_HELP_STRING([--enable-pon-adapter], [enable PON Adapter build]),
   [
      if test "$enableval" = yes; then
         echo Enable PON Adapter build
         AC_DEFINE([INCLUDE_PON_ADAPTER], [1], [enable PON Adapter build])
         AM_CONDITIONAL(INCLUDE_PON_ADAPTER, true)
      else
         echo Disable PON Adapter build
      fi
   ],
   [
      echo Disable PON Adapter build
   ]
)

dnl UBUS_ENABLE enable\disable
AC_ARG_WITH(ubus,
   AS_HELP_STRING([--without-ubus], [build without ubus support]),
   [
      case $withval in
      0|no)
            AM_CONDITIONAL(UBUS_ENABLE, false)
            ;;
      1|yes)
            AC_DEFINE([UBUS_ENABLE], [1], [Build PON_LIB with UBUS support])
            AM_CONDITIONAL(UBUS_ENABLE, true)
            ;;
      *)
            AC_DEFINE([UBUS_ENABLE], [1], [Build PON_LIB with UBUS support])
            AM_CONDITIONAL(UBUS_ENABLE, true)
            ;;
      esac
   ],
   [
      AC_DEFINE([UBUS_ENABLE], [1], [Build PON_LIB with UBUS support])
      AM_CONDITIONAL(UBUS_ENABLE, true)
   ]
)

dnl enable\disable PON Simulation
AM_CONDITIONAL(PON_LIB_SIMULATOR, false)
AC_ARG_ENABLE(simulator,
   AS_HELP_STRING([--enable-simulator], [enable Simulator]),
   [
      if test "$enableval" = yes; then
         echo Enable Simulator
         AC_DEFINE([PON_LIB_SIMULATOR], [1], [enable Simulator])
         AM_CONDITIONAL(PON_LIB_SIMULATOR, true)
      else
         echo Disable Simulator
      fi
   ],
   [
      echo Disable Simulator
   ]
)

dnl enable\disable build with PON Tests
AM_CONDITIONAL(INCLUDE_PON_TESTS, false)
AC_ARG_ENABLE(tests,
   AS_HELP_STRING([--enable-tests], [enable tests]),
   [
      if test "$enableval" = yes; then
         echo Enable Tests
         AC_DEFINE([INCLUDE_PON_TESTS], [1], [enable tests])
         AM_CONDITIONAL(INCLUDE_PON_TESTS, true)
      fi
   ],
   [
      echo Disable tests
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

AC_CONFIG_FILES([Makefile src/Makefile cli/Makefile daemon/Makefile tools/Makefile])
AC_OUTPUT
