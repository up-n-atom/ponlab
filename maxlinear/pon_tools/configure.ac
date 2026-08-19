dnl Process this file with autoconf to produce a configure script.

AC_REVISION($Revision: 1.2.3.1 $)

AC_INIT([PON TOOLS Application],[1.11.1],[],[pon_tools])

AC_CONFIG_SRCDIR(src/pontop/Makefile.am)
AC_CONFIG_HEADERS(src/pon_tools.h)
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([-Wno-portability -Wall tar-pax])

AC_PROG_CC
ifdef([AC_PROG_CC_STDC], [AC_PROG_CC_STDC])
AC_LANG([C])
AC_PROG_RANLIB
AM_PROG_CC_C_O

#
# save the configure arguments
#
CONFIGURE_OPTIONS="$ac_configure_args"
AC_SUBST(CONFIGURE_OPTIONS)

dnl set pon_lib include path
DEFAULT_PONLIB_INCLUDE_PATH=''
AC_ARG_ENABLE(ponlib-include,
   AS_HELP_STRING([--enable-ponlib-include=-I/path/to/your/ponlib/include],[Set the pon_lib include path]),
   [
    if test -n "$enableval"; then
       echo Set the pon_lib include path to $enableval
       AC_SUBST([PONLIB_INCLUDE_PATH],[$enableval])
    else
       echo Set the pon_lib include path to $DEFAULT_PONLIB_INCLUDE_PATH
       AC_SUBST([PONLIB_INCLUDE_PATH],[$DEFAULT_PONLIB_INCLUDE_PATH])
    fi
   ],
   [
      echo Set the pon_lib include path to $DEFAULT_PONLIB_INCLUDE_PATH
      AC_SUBST([PONLIB_INCLUDE_PATH],[$DEFAULT_PONLIB_INCLUDE_PATH])
   ]
)

dnl set pon_lib include path
DEFAULT_GPONLIBS_INCLUDE_PATH=''
AC_ARG_ENABLE(gponlibs-include,
   AS_HELP_STRING([--enable-gponlibs-include=-I/path/to/your/gponlibs/include],[Set the gponlibs include path]),
   [
    if test -n "$enableval"; then
       echo Set the pon_lib include path to $enableval
       AC_SUBST([GPONLIBS_INCLUDE_PATH],[$enableval])
    else
       echo Set the pon_lib include path to $DEFAULT_GPONLIBS_INCLUDE_PATH
       AC_SUBST([GPONLIBS_INCLUDE_PATH],[$DEFAULT_GPONLIBS_INCLUDE_PATH])
    fi
   ],
   [
      echo Set the pon_lib include path to $DEFAULT_GPONLIBS_INCLUDE_PATH
      AC_SUBST([GPONLIBS_INCLUDE_PATH],[$DEFAULT_GPONLIBS_INCLUDE_PATH])
   ]
)

AH_TOP(
   [
#ifndef _pon_tools_config_h
#define _pon_tools_config_h

/** \defgroup PON TOOLS config - PON debug tool Configuration
   @{
*/
   ]
)

AH_BOTTOM(
   [
/** @} */

#endif
   ]
)

echo \
"------------------------------------------------------------------------
Configuration:

	source code location:         ${srcdir}
	compiler:                     ${CC}
	compiler flags:               ${CFLAGS}
	host system type:             ${host}
	install path:                 ${prefix}

	ponlib include path	      ${PONLIB_INCLUDE_PATH}
	gponlibs include path	   ${GPONLIBS_INCLUDE_PATH}

Settings:
	configure options:            ${CONFIGURE_OPTIONS}
------------------------------------------------------------------------"

# Check for git and a git repo.
if test -e "$srcdir/.git"; then
        # Hash of lastest tag
        latest_tag=`git -C "$srcdir" describe --tags --abbrev=0`

        # Number of commits until latest tag
        if test -n "$latest_tag"; then
                EXTRA_VERSION=`git -C "$srcdir" rev-list HEAD "^$latest_tag" --count`
        fi

        # Check if repo is dirty
        if test `git -C "$srcdir" diff-index -p HEAD | wc -l` -gt 0 ; then
                EXTRA_VERSION="$EXTRA_VERSION-dirty"
        fi
fi

if test -n "$EXTRA_VERSION"; then
        AC_DEFINE_UNQUOTED([EXTRA_VERSION], ["$EXTRA_VERSION"], [extra version])
fi

AC_CONFIG_FILES([Makefile src/pontop/Makefile])
AC_OUTPUT
