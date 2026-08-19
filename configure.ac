dnl Process this file with autoconf to produce a configure script.

AC_REVISION($Revision: 0.1 $)

AC_INIT([PON_PA_TEMPLATE],[0.0.2],[],[pon_pa_template])
AC_CONFIG_SRCDIR(src/Makefile.am)
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wno-portability tar-pax subdir-objects])
AC_PROG_CC

AC_LANG([C])
AM_PROG_CC_C_O
LT_INIT

# save the configure arguments
CONFIGURE_OPTIONS="$ac_configure_args"
AC_SUBST(CONFIGURE_OPTIONS)

dnl set pon_adapter include path
DEFAULT_PON_ADAPTER_INCLUDE_PATH=''
AC_ARG_ENABLE(pon-adapter-include,
   AS_HELP_STRING([--enable-pon-adapter-include=-I/path/to/your/pon_adapter/include],[Set the pon_adapter include path]),
   [
    if test -n "$enableval"; then
       AC_MSG_NOTICE([Set the pon_adapter include path to $enableval])
       AC_SUBST([PON_ADAPTER_INCLUDE_PATH],[$enableval])
    else
       AC_MSG_NOTICE([Set the pon_adapter include path to $DEFAULT_PON_ADAPTER_INCLUDE_PATH])
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
       AC_MSG_NOTICE([Set the pon_adapter library path to $enableval])
       AC_SUBST([PON_ADAPTER_LIBRARY_PATH],[$enableval])
    else
       AC_MSG_NOTICE([Set the pon_adapter library path to $DEFAULT_PON_ADAPTER_LIBRARY_PATH])
       AC_SUBST([PON_ADAPTER_LIBRARY_PATH],[$DEFAULT_PON_ADAPTER_LIBRARY_PATH])
    fi
   ],
   [
      echo Set the pon_adapter library path to $DEFAULT_PON_ADAPTER_LIBRARY_PATH
      AC_SUBST([PON_ADAPTER_LIBRARY_PATH],[$DEFAULT_PON_ADAPTER_LIBRARY_PATH])
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
