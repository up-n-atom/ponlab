dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.63)
AC_INIT([PON Multicast Driver],[1.12.1],[],[pon_mcc_drv])
#AC_CONFIG_HEADERS([src/drv_pon_mbox_config.h])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR(src/drv_pon_mcc.c)

AM_INIT_AUTOMAKE([-Wno-portability tar-pax])

#
# save the configure arguments
#
CONFIGURE_OPTIONS="$ac_configure_args"
AC_SUBST(CONFIGURE_OPTIONS)

# Checks for programs
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
LT_INIT

#AC_CHECK_TYPES([ulong_t])

#dnl Set kernel include path (Linux, eCos, ...)
#AC_ARG_ENABLE(kernelincl,
#    AS_HELP_STRING(
#        [--enable-kernelincl=x],
#        [Set the target kernel include path]
#    ),
#    [
#        if test -n $enableval; then
#            echo Set the kernel include path $enableval
#            AC_SUBST([KERNEL_INCL_PATH],[$enableval])
#        fi
#    ]
#)

dnl enable linux kernel support
AC_ARG_ENABLE(linux-kernel,
   AS_HELP_STRING(--enable-linux-kernel,Enable support for linux kernel),
   [
      if test -z "$ARCH" ; then
         [ARCH=`$CC -dumpmachine | sed -e s'/-.*//' \
         -e 's/i[3-9]86/i386/' \
         -e 's/mipsel/mips/' \
         -e 's/powerpc/ppc/' \
         -e 's/sh[234]/sh/' \
         `]
      fi
      if test -n "$ARCH" ; then
         echo "Set the kernel architecture to $ARCH"
         AC_SUBST([KERNEL_ARCH],[$ARCH])
      else
         AC_MSG_ERROR([Kernel architecture not set!])
      fi
      if test -n "$KBUILD_EXTRA_SYMBOLS" ; then
         AC_SUBST([KBUILD_EXTRA_SYMBOLS],[$KBUILD_EXTRA_SYMBOLS])
      fi

      if test "$enableval" = yes; then
         echo Enable Linux Kernel compile
         AM_CONDITIONAL(INCLUDE_LINUX_KERNEL, true)
      else
         echo Disable Linux Kernel compile
         AM_CONDITIONAL(INCLUDE_LINUX_KERNEL, false)
      fi
   ],
   [
     echo Disable Linux Kernel compile
     AM_CONDITIONAL(INCLUDE_LINUX_KERNEL, false)
   ]
)

dnl Set kernel build path
AC_ARG_ENABLE(kernelbuild,
    AS_HELP_STRING(
        [--enable-kernelbuild=x],
        [Set the target kernel build path (optional, assumed from kernelincl)]
    ),
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

AC_ARG_ENABLE(chk,
   AS_HELP_STRING(--enable-chk,build pon mcc chk driver in addition),
   [
      AS_IF([test "$enable_chk" = "yes"], [
         AM_CONDITIONAL(CONFIG_CHK, true)
      ], [
         AM_CONDITIONAL(CONFIG_CHK, false)
      ])
   ], [
      AM_CONDITIONAL(CONFIG_CHK, false)
   ]
)


AC_CONFIG_FILES([Makefile src/Makefile lib/Makefile cli/Makefile chk/Makefile])
AC_OUTPUT
