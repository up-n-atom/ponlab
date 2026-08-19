dnl Process this file with autoconf to produce a configure script.

AC_REVISION($Revision: 1.10 $)
AC_INIT([GPON Libraries], [1.8.0], [], [gpon_libs])
AC_CONFIG_SRCDIR([libtop/top.c])
AC_CONFIG_HEADER([gpon_libs_config.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_RANLIB
AM_INIT_AUTOMAKE([-Wno-portability tar-pax])

#
# save the configure arguments
#
CONFIGURE_OPTIONS="$ac_configure_args"
AC_SUBST(CONFIGURE_OPTIONS)

AH_TOP(
   [
#ifndef _gpon_libs_config_h
#define _gpon_libs_config_h
   ]
)

AH_BOTTOM(
   [
#endif
   ]
)

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

dnl compile for linux kernel
ENABLE_LINUX=true
AC_ARG_ENABLE(linux,
   AS_HELP_STRING(
      [--enable-linux],
      [Enable linux support]
   ),
   [
      case $enableval in
      0|no)
         ENABLE_LINUX=false
         ;;
      1|yes)
         ;;
      *) AC_MSG_ERROR([linux support]);
         ;;
      esac
   ]
)

dnl compile for eCos kernel
ENABLE_ECOS=false
AC_ARG_ENABLE(ecos,
   AS_HELP_STRING(
      [--enable-ecos],
      [Enable eCos support]
   ),
   [
      case $enableval in
      0|no)
         ;;
      1|yes)
         ENABLE_ECOS=true
         ;;
      *) AC_MSG_ERROR([eCos support]);
         ;;
      esac
   ]
)

AM_CONDITIONAL(ENABLE_LINUX, $ENABLE_LINUX)
if test "$ENABLE_LINUX" == "true" ; then
   AC_DEFINE(ENABLE_LINUX,,[compile for linux kernel])
fi

AM_CONDITIONAL(ENABLE_ECOS, $ENABLE_ECOS)
if test "$ENABLE_ECOS" == "true" ; then
   AC_DEFINE(ENABLE_ECOS,,[compile for eCos])
fi

dnl Set the target library include path
if test -z "$TARGET_INCLUDE"; then
	DEFAULT_TARGET_INCL_PATH='.'
else
	DEFAULT_TARGET_INCL_PATH=$TARGET_INCLUDE
fi
AC_ARG_ENABLE(targetincl,
	AS_HELP_STRING(
		[--enable-targetincl=x],
		[Set the target image include path]
	),
	[
	if test -n "$enableval"; then
		echo Set the target image include path $enableval
		AC_SUBST([TARGET_INCL_PATH],[$enableval])
	else
		echo Set the default target image include path $DEFAULT_TARGET_INCL_PATH
		AC_SUBST([TARGET_INCL_PATH],[$DEFAULT_TARGET_INCL_PATH])
	fi
	],
	[
		echo Set the default target image include path $DEFAULT_TARGET_INCL_PATH
		AC_SUBST([TARGET_INCL_PATH],[$DEFAULT_TARGET_INCL_PATH])
	]
)

dnl set maximum line length for TOP table/dump data
DEFAULT_LINE_LEN='1000'
AC_ARG_ENABLE(top-line-len,
	AS_HELP_STRING([--enable-top-line-len=size],[set TOP maximum line length]),
	[
	if test -n "$enableval"; then
		echo Set TOP maximum line length to configured $enableval
		AC_DEFINE_UNQUOTED([LINE_LEN],[$enableval],[TOP maximum line length])
		AC_SUBST([TOP_LINE_LEN], [$enableval])
	else
		echo Set TOP maximum line length X to $DEFAULT_LINE_LEN
		AC_DEFINE_UNQUOTED([LINE_LEN],[$DEFAULT_LINE_LEN],[TOP maximum line length])
		AC_SUBST([TOP_LINE_LEN], [$DEFAULT_LINE_LEN])
	fi
	],
	[
		echo Set TOP maximum line length Y to $DEFAULT_LINE_LEN
		AC_DEFINE_UNQUOTED([LINE_LEN],[$DEFAULT_LINE_LEN],[TOP maximum line length])
		AC_SUBST([TOP_LINE_LEN], [$DEFAULT_LINE_LEN])
	]
)

dnl set maximum amount of lines for TOP table/dump data
DEFAULT_LINE_MAX='25000'
AC_ARG_ENABLE(top-line-max,
	AS_HELP_STRING([--enable-top-line-max=size],[set TOP maximum amount of lines]),
	[
	if test -n "$enableval"; then
		echo Set TOP maximum amount of lines to configured $enableval
		AC_DEFINE_UNQUOTED([LINE_MAX],[$enableval],[TOP maximum amount of lines])
		AC_SUBST([TOP_LINE_MAX], [$DEFAULT_LINE_MAX])
	else
		echo Set TOP maximum amount of lines X to $DEFAULT_LINE_MAX
		AC_DEFINE_UNQUOTED([LINE_MAX],[$DEFAULT_LINE_MAX],[TOP maximum amount of lines])
		AC_SUBST([TOP_LINE_MAX], [$DEFAULT_LINE_MAX])
	fi
	],
	[
		echo Set TOP maximum amount of lines Y to $DEFAULT_LINE_MAX
		AC_DEFINE_UNQUOTED([LINE_MAX],[$DEFAULT_LINE_MAX],[TOP maximum amount of lines])
		AC_SUBST([TOP_LINE_MAX], [$DEFAULT_LINE_MAX])
	]
)

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

AC_CONFIG_FILES([Makefile
                 libtop/Makefile
                 libtop/top_config.h])
AC_OUTPUT
