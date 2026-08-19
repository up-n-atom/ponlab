dnl Process this file with autoconf to produce a configure script.

AC_REVISION($Revision: 1.9 $)

AC_INIT([Command Line Interface],[2.9.0],[],[lib_cli])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([cli_aux])

AC_CONFIG_SRCDIR(src/Makefile.am)
AM_INIT_AUTOMAKE([tar-pax])

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_LIBTOOL
AM_PROG_CC_C_O
LT_INIT

#
# save the configure arguments
#
CONFIGURE_OPTIONS="$ac_configure_args"
AC_SUBST(CONFIGURE_OPTIONS)

AM_CONDITIONAL(HAVE_GCC, true)
AM_CONDITIONAL(ENABLE_LINUX, true)
AM_CONDITIONAL(ENABLE_ECOS, false)
AM_CONDITIONAL(ENABLE_GENERIC, false)
AM_CONDITIONAL(ENABLE_LINT, false)

AM_CONDITIONAL(WITH_KERNEL_MODULE, false)
AM_CONDITIONAL(HAVE_IFXOS_SYSOBJ_SUPPORT, false)

AM_CONDITIONAL(ENABLE_DEBUG, false)
AM_CONDITIONAL(ENABLE_CLI_CONSOLE, true)
AM_CONDITIONAL(ENABLE_CLI_PIPE, true)
AM_CONDITIONAL(BUILD_APPL_EXAMPLE, false)

AM_CONDITIONAL(CLI_SPECIAL_IO_FILE_TYPE, false)

dnl target options for library build
LIB_CLI_TARGET_OPTIONS=""
dnl feature configuration options for the library build
LIB_CLI_FEATURE_OPTIONS=""

dnl target options for kernel module build
DRV_CLI_TARGET_OPTIONS=""
dnl feature configuration options for the kernel module build
DRV_CLI_FEATURE_OPTIONS=""

dnl enable linux kernel 2.6.x support
AM_CONDITIONAL(KERNEL_2_6, true)
AC_ARG_ENABLE(linux-26,
   AS_HELP_STRING(--enable-linux-26,Enable support for linux kernel 2.6.x),
   [
      AM_CONDITIONAL(KERNEL_2_6, true)
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
   ]
)

dnl Set kernel include path (Linux, eCos, ...)
DEFAULT_KERNEL_INCL_PATH='.'
AC_ARG_ENABLE(kernelincl,
   AS_HELP_STRING([--enable-kernelincl=x],[Set the target kernel include path
   ]),
   [
    if test -n $enableval; then
       echo Set the kernel include path $enableval
       AC_SUBST([KERNEL_INCL_PATH],[$enableval])
    else
       echo Set the default kernel include path $DEFAULT_KERNEL_INCL_PATH
       AC_SUBST([KERNEL_INCL_PATH],[$DEFAULT_KERNEL_INCL_PATH])
    fi
   ],
   [
       echo Set the default kernel include path $DEFAULT_KERNEL_INCL_PATH
       AC_SUBST([KERNEL_INCL_PATH],[$DEFAULT_KERNEL_INCL_PATH])
   ]
)

dnl Set kernel build path
AC_ARG_ENABLE(kernelbuild,
   AS_HELP_STRING(--enable-kernelbuild=x,Set the target kernel build path (only for kernel 2.6.x)),
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

dnl Build of the Kernel modules under LINUX
AC_ARG_WITH(kernel-module,
	AS_HELP_STRING(
		[--with-kernel-module],
		[Build with or without Kernel Module]
	),
	[
		echo Build with or without Kernel Module = $withval

		case $withval in
		0|no|NO)
			AM_CONDITIONAL(WITH_KERNEL_MODULE, false)
			AC_SUBST([WITH_KERNEL_MODULE],[no])
			;;
		1|yes|YES)
			AM_CONDITIONAL(WITH_KERNEL_MODULE, true)
			AC_SUBST([WITH_KERNEL_MODULE],[yes])
			;;
		*) AC_MSG_ERROR([ With Kernel Module, use with or without ]);
			;;
		esac
	],
	[
		AM_CONDITIONAL(WITH_KERNEL_MODULE, false)
		AC_SUBST([WITH_KERNEL_MODULE],[no])
	]
)

dnl Enable the cc compiler
AC_ARG_ENABLE(have-cc,
	AS_HELP_STRING(
		[--enable-have-cc],
		[Use the cc compiler]
	),
	[
		if test "$enableval" = yes; then
			echo Use the cc compiler
			AM_CONDITIONAL(HAVE_GCC, false)
			AC_SUBST([HAVE_GCC],[no])
		else
			echo Use the gcc compiler
			AM_CONDITIONAL(HAVE_GCC, true)
			AC_SUBST([HAVE_GCC],[yes])
		fi
	],
	[
		AM_CONDITIONAL(HAVE_GCC, true)
		AC_SUBST([HAVE_GCC],[yes])
	]
)

dnl Enable the eCos operating system
AC_ARG_ENABLE(ecos,
	AS_HELP_STRING(
		[--enable-ecos],
		[Enable eCos OS]
	),
	[
		if test "$enableval" = yes; then
			echo Enable eCos operating system
			AM_CONDITIONAL(ENABLE_LINUX, false)
			AM_CONDITIONAL(ENABLE_ECOS, true)
		fi
	]
)

dnl Enable the generic operating system
AC_ARG_ENABLE(generic,
	AS_HELP_STRING(
		[--enable-generic],
		[Enable generic OS]
	),
	[
		if test "$enableval" = yes; then
			echo Enable generic operating system
			AM_CONDITIONAL(ENABLE_LINUX, false)
			AM_CONDITIONAL(ENABLE_GENERIC, true)
		fi
	]
)

dnl Enable debugging mode
AC_ARG_ENABLE(debug,
	AS_HELP_STRING(
		[--enable-debug],
		[Compilation with Debug Enable]
	),
	[
		case $enableval in
		0|no|NO)
			AM_CONDITIONAL(ENABLE_DEBUG, false)
			AC_SUBST([ENABLE_DEBUG],[no])
			;;
		1|yes|YES)
			AM_CONDITIONAL(ENABLE_DEBUG, true)
			AC_SUBST([ENABLE_DEBUG],[yes])
			LIB_CLI_TARGET_OPTIONS="$LIB_CLI_TARGET_OPTIONS -DCLI_DEBUG"
			;;
		*)
			AM_CONDITIONAL(ENABLE_DEBUG, false)
			AC_SUBST([ENABLE_DEBUG],[no])
			;;
		esac
	],
	[
		AM_CONDITIONAL(ENABLE_DEBUG, false)
		AC_SUBST([ENABLE_DEBUG],[no])
	]
)

dnl Enable debug printouts
AC_ARG_ENABLE(debug-print,
	AS_HELP_STRING(
		[--enable-debug-print],
		[Enable debug printouts]
	),
	[
		case $enableval in
		0|no|NO)
			LIB_CLI_TARGET_OPTIONS="$LIB_CLI_TARGET_OPTIONS -DENABLE_DEBUG_PRINT=0"
			AC_SUBST([ENABLE_DEBUG_PRINT],[no])
			;;
		1|yes|YES)
			LIB_CLI_TARGET_OPTIONS="$LIB_CLI_TARGET_OPTIONS -DENABLE_DEBUG_PRINT=1"
			AC_SUBST([ENABLE_DEBUG_PRINT],[yes])
			;;
		*)
			LIB_CLI_TARGET_OPTIONS="$LIB_CLI_TARGET_OPTIONS -DENABLE_DEBUG_PRINT=0"
			AC_SUBST([ENABLE_DEBUG_PRINT],[no])
			;;
		esac
	],
	[
		AC_SUBST([ENABLE_DEBUG_PRINT],[internal])
	]
)

dnl Enable error printouts
AC_ARG_ENABLE(error-print,
	AS_HELP_STRING(
		[--enable-error-print],
		[Enable error printouts]
	),
	[
		case $enableval in
		0|no|NO)
			LIB_CLI_TARGET_OPTIONS="$LIB_CLI_TARGET_OPTIONS -DENABLE_ERROR_PRINT=0"
			AC_SUBST([ENABLE_ERROR_PRINT],[no])
			;;
		1|yes|YES)
			LIB_CLI_TARGET_OPTIONS="$LIB_CLI_TARGET_OPTIONS -DENABLE_ERROR_PRINT=1"
			AC_SUBST([ENABLE_ERROR_PRINT],[yes])
			;;
		*)
			LIB_CLI_TARGET_OPTIONS="$LIB_CLI_TARGET_OPTIONS -DENABLE_ERROR_PRINT=0"
			AC_SUBST([ENABLE_ERROR_PRINT],[no])
			;;
		esac
	],
	[
		AC_SUBST([ENABLE_ERROR_PRINT],[internal])
	]
)

dnl Enable user print out via buffer
AC_ARG_ENABLE(cli-buffer-pout,
	AS_HELP_STRING(
		[--enable-cli-buffer-pout],
		[Enable user print out via buffer]
	),
	[
		case $enableval in
		0|no|NO)
			LIB_CLI_FEATURE_OPTIONS="$LIB_CLI_FEATURE_OPTIONS -DCLI_SUPPORT_BUFFER_OUT=0"
			AC_SUBST([ENABLE_CLI_BUFFER_OUT],[no])
			;;
		1|yes|YES)
			LIB_CLI_FEATURE_OPTIONS="$LIB_CLI_FEATURE_OPTIONS -DCLI_SUPPORT_BUFFER_OUT=1"
			AC_SUBST([ENABLE_CLI_BUFFER_OUT],[yes])
			;;
		*)
			LIB_CLI_FEATURE_OPTIONS="$LIB_CLI_FEATURE_OPTIONS -DCLI_SUPPORT_BUFFER_OUT=0"
			AC_SUBST([ENABLE_CLI_BUFFER_OUT],[no])
			;;
		esac
	],
	[
		AC_SUBST([ENABLE_CLI_BUFFER_OUT],[internal])
	]
)

dnl Enable user print out via file
AC_ARG_ENABLE(cli-file-pout,
	AS_HELP_STRING(
		[--enable-cli-file-pout],
		[Enable user print out via file fd]
	),
	[
		case $enableval in
		0|no|NO)
			LIB_CLI_FEATURE_OPTIONS="$LIB_CLI_FEATURE_OPTIONS -DCLI_SUPPORT_FILE_OUT=0"
			AC_SUBST([ENABLE_CLI_FILE_OUT],[no])
			;;
		1|yes|YES)
			LIB_CLI_FEATURE_OPTIONS="$LIB_CLI_FEATURE_OPTIONS -DCLI_SUPPORT_FILE_OUT=1"
			AC_SUBST([ENABLE_CLI_FILE_OUT],[yes])
			;;
		*)
			LIB_CLI_FEATURE_OPTIONS="$LIB_CLI_FEATURE_OPTIONS -DCLI_SUPPORT_FILE_OUT=0"
			AC_SUBST([ENABLE_CLI_FILE_OUT],[no])
			;;
		esac
	],
	[
		AC_SUBST([ENABLE_CLI_FILE_OUT],[internal])
	]
)

dnl Enable CLI Console support
AC_ARG_ENABLE(cli-console,
	AS_HELP_STRING(
		[--enable-cli-console],
		[Enable CLI Console support]
	),
	[
		case $enableval in
		0|no|NO)
			LIB_CLI_FEATURE_OPTIONS="$LIB_CLI_FEATURE_OPTIONS -DCLI_SUPPORT_CONSOLE=0"
			AM_CONDITIONAL(ENABLE_CLI_CONSOLE, false)
			AC_SUBST([ENABLE_CLI_CONSOLE],[no])
			;;
		1|yes|YES)
			LIB_CLI_FEATURE_OPTIONS="$LIB_CLI_FEATURE_OPTIONS -DCLI_SUPPORT_CONSOLE=1"
			AM_CONDITIONAL(ENABLE_CLI_CONSOLE, true)
			AC_SUBST([ENABLE_CLI_CONSOLE],[yes])
			;;
		*)
			LIB_CLI_FEATURE_OPTIONS="$LIB_CLI_FEATURE_OPTIONS -DCLI_SUPPORT_CONSOLE=0"
			AM_CONDITIONAL(ENABLE_CLI_CONSOLE, false)
			AC_SUBST([ENABLE_CLI_CONSOLE],[no])
			;;
		esac
	],
	[
		AC_SUBST([ENABLE_CLI_CONSOLE],[internal])
	]
)

dnl Enable CLI Pipe support
AC_ARG_ENABLE(cli-pipe,
	AS_HELP_STRING(
		[--enable-cli-pipe],
		[Enable CLI Pipe support]
	),
	[
		case $enableval in
		0|no|NO)
			LIB_CLI_FEATURE_OPTIONS="$LIB_CLI_FEATURE_OPTIONS -DCLI_SUPPORT_PIPE=0"
			AM_CONDITIONAL(ENABLE_CLI_PIPE, false)
			AC_SUBST([ENABLE_CLI_PIPE],[no])
			;;
		1|yes|YES)
			LIB_CLI_FEATURE_OPTIONS="$LIB_CLI_FEATURE_OPTIONS -DCLI_SUPPORT_PIPE=1"
			AM_CONDITIONAL(ENABLE_CLI_PIPE, true)
			AC_SUBST([ENABLE_CLI_PIPE],[yes])
			;;
		*)
			LIB_CLI_FEATURE_OPTIONS="$LIB_CLI_FEATURE_OPTIONS -DCLI_SUPPORT_PIPE=0"
			AM_CONDITIONAL(ENABLE_CLI_PIPE, false)
			AC_SUBST([ENABLE_CLI_PIPE],[no])
			;;
		esac
	],
	[
		AC_SUBST([ENABLE_CLI_PIPE],[internal])
	]
)

dnl set the number of max supported pipes (default 3)
AC_ARG_WITH(max-pipes,
	AS_HELP_STRING(
		[--with-max-pipes=value],
		[max cards.]
	),
	[
		if test "$withval" = yes; then
			AC_MSG_ERROR([ Please provide a value for the max pipes]);
		fi
		LIB_CLI_FEATURE_OPTIONS="$LIB_CLI_FEATURE_OPTIONS -DCLI_MAX_PIPES=$withval"
		AC_SUBST([CLI_MAX_PIPES],[$withval])
	],
	[
		AC_SUBST([CLI_MAX_PIPES],[internal])
	]
)

dnl Build the CLI Example
AC_ARG_ENABLE(build-example,
	AS_HELP_STRING(
		[--enable-build-example],
		[Build the CLI Example]
	),
	[
		case $enableval in
		0|no|NO)
			AM_CONDITIONAL(BUILD_APPL_EXAMPLE, false)
			AC_SUBST([BUILD_APPL_EXAMPLE],[no])
			;;
		1|yes|YES)
			AM_CONDITIONAL(BUILD_APPL_EXAMPLE, true)
			AC_SUBST([BUILD_APPL_EXAMPLE],[yes])
			;;
		*)
			AM_CONDITIONAL(BUILD_APPL_EXAMPLE, false)
			AC_SUBST([BUILD_APPL_EXAMPLE],[no])
			;;
		esac
	],
	[
		AM_CONDITIONAL(BUILD_APPL_EXAMPLE, true)
		AC_SUBST([BUILD_APPL_EXAMPLE],[internal])
	]
)

dnl set libifxos.a library path
DEFAULT_IFXOS_LIBRARY_PATH='-L.'
AC_ARG_ENABLE(ifxos-library,
	AS_HELP_STRING([--enable-ifxos-library=-L/path/to/your/build/lib_ifxos/src] ,
				[Set the lib_ifxos library path. In this location the libifxos.* should be found]),
	[
	 if test -n "$enableval"; then
		 echo Set the lib_ifxos library path to $enableval
		 AC_SUBST([IFXOS_LIBRARY_PATH],[$enableval])
	 else
		 echo Set the lib_ifxos library path to $DEFAULT_IFXOS_LIBRARY_PATH
		 AC_SUBST([IFXOS_LIBRARY_PATH],[$DEFAULT_IFXOS_LIBRARY_PATH])
	 fi
	],
	[
		echo Set the lib_ifxos library path to $DEFAULT_IFXOS_LIBRARY_PATH
		AC_SUBST([IFXOS_LIBRARY_PATH],[$DEFAULT_IFXOS_LIBRARY_PATH])
	]
)

dnl set lib_ifxos include path
DEFAULT_IFXOS_INCLUDE_PATH='-I.'
AC_ARG_ENABLE(ifxos-include,
	AS_HELP_STRING([--enable-ifxos-include=-I/path/to/your/lib_ifxos/src/include],[Set the lib_ifxos include path.]),
	[
	 if test -n "$enableval"; then
		 echo Set the lib_ifxos include path to $enableval
		 AC_SUBST([IFXOS_INCLUDE_PATH],[$enableval])
	 else
		 echo Set the lib_ifxos include path to $DEFAULT_IFXOS_INCLUDE_PATH
		 AC_SUBST([IFXOS_INCLUDE_PATH],[$DEFAULT_IFXOS_INCLUDE_PATH])
	 fi
	],
	[
		echo Set the lib_ifxos include path to $DEFAULT_IFXOS_INCLUDE_PATH
		AC_SUBST([IFXOS_INCLUDE_PATH],[$DEFAULT_IFXOS_INCLUDE_PATH])
	]
)

dnl enable IFXOS System Object support
AC_ARG_ENABLE(ifxos-sys-obj-support,
	AS_HELP_STRING(
		[--enable-ifxos-sys-obj-support],[Support IFXOS System Objects.]
	),

	[
		case $enableval in
		0|no|NO)
			AM_CONDITIONAL(HAVE_IFXOS_SYSOBJ_SUPPORT, false)
			AC_SUBST([HAVE_IFXOS_SYSOBJ_SUPPORT],[no])
			;;
		1|yes|YES)
			AM_CONDITIONAL(HAVE_IFXOS_SYSOBJ_SUPPORT, true)
			AC_SUBST([HAVE_IFXOS_SYSOBJ_SUPPORT],[yes])
			;;
		*)
			AM_CONDITIONAL(HAVE_IFXOS_SYSOBJ_SUPPORT, false)
			AC_SUBST([HAVE_IFXOS_SYSOBJ_SUPPORT],[no])
			;;
		esac
	],
	[
		AM_CONDITIONAL(HAVE_IFXOS_SYSOBJ_SUPPORT, false)
		AC_SUBST([HAVE_IFXOS_SYSOBJ_SUPPORT],[no])
	]
)

dnl enable special IO file type
AC_ARG_ENABLE(special-io-file-type,
   AS_HELP_STRING(
      [--enable-special-io-file-type],[Enable special IO file type.]
   ),
   [
      if test "$enableval" = yes; then
         AC_DEFINE(CLI_SPECIAL_IO_FILE_TYPE, 1,[Enable special IO file type])
         AM_CONDITIONAL(CLI_SPECIAL_IO_FILE_TYPE, true)
         AC_SUBST([CLI_SPECIAL_IO_FILE_TYPE],[yes])
      else
         AC_DEFINE(CLI_SPECIAL_IO_FILE_TYPE, 0,[Disable special IO file type])
         AC_SUBST([CLI_SPECIAL_IO_FILE_TYPE],[no])
      fi
   ],
   [
      AC_DEFINE(CLI_SPECIAL_IO_FILE_TYPE, 0,[Disable special IO file type])
      AC_SUBST([CLI_SPECIAL_IO_FILE_TYPE],[no])
   ]
)

# LT_INIT

AC_SUBST([LIB_CLI_TARGET_OPTIONS],[$LIB_CLI_TARGET_OPTIONS])
AC_SUBST([LIB_CLI_FEATURE_OPTIONS],[$LIB_CLI_FEATURE_OPTIONS])
AC_SUBST([DISTCHECK_CONFIGURE_PARAMS],[$CONFIGURE_OPTIONS])

echo \
"------------------------------------------------------------------------
Configuration:

	source code location:         ${srcdir}
	compiler:                     ${CC}
	compiler flags:               ${CFLAGS}
	host system type:             ${host}
	install path:                 ${prefix}

	ifxos library path:           ${IFXOS_LIBRARY_PATH}
	ifxos include path:           ${IFXOS_INCLUDE_PATH}
	IFXOS Object Support:         ${HAVE_IFXOS_SYSOBJ_SUPPORT}

	Build Kernel Module:          ${WITH_KERNEL_MODULE}

Feature Config:
	Support printout via buffer:  ${ENABLE_CLI_BUFFER_OUT}
	Support printout via file fd: ${ENABLE_CLI_FILE_OUT}
	Support CLI Console IF:       ${ENABLE_CLI_CONSOLE}
	Support CLI Pipe IF:          ${ENABLE_CLI_PIPE}
	Max pipes:                    ${CLI_MAX_PIPES}
	feature options:              ${LIB_CLI_FEATURE_OPTIONS}

Misc Target Options:
	Debug printouts:              ${ENABLE_DEBUG_PRINT}
	Error printouts:              ${ENABLE_ERROR_PRINT}
	target options:               ${LIB_CLI_TARGET_OPTIONS}

Application Target Options:
	Build Example (user space)    ${BUILD_APPL_EXAMPLE}

Settings:
	configure options:            ${CONFIGURE_OPTIONS}
------------------------------------------------------------------------"

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
