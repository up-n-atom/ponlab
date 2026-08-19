#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([PON_ADAPTER], [2.2.0], [pon_adapter])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([-Wno-portability tar-pax])
AC_CONFIG_SRCDIR([self_test/main.c])
AC_CONFIG_MACRO_DIRS([m4])

# Save the configure arguments
CONFIGURE_OPTIONS="$ac_configure_args"
AC_SUBST(CONFIGURE_OPTIONS)

# Checks for programs.
AC_PROG_CC
LT_INIT

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

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

AC_CONFIG_FILES([Makefile
                 include/Makefile
                 src/Makefile
                 self_test/Makefile])
AC_OUTPUT
