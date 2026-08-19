#serial 1

dnl Macro that provides the --enable-debug option
AC_DEFUN([ENABLE_DEBUG_OPT],
[
AC_ARG_ENABLE([debug],
  [AS_HELP_STRING([--enable-debug], [build for debugging])],
  [case $enableval in
    yes|no) : ;;
    *) AC_MSG_ERROR([--enable-debug: unrecognized ARG: expected yes or no]) ;;
  esac])

# If debug is enabled, override default CFLAGS chosen by AC_PROG_CC for gcc
AS_IF([test "${ac_save_CFLAGS}_${GCC}_${enable_debug}" = _yes_yes],
  [CFLAGS="-g -O0"])
AS_IF([test "${ac_save_CFLAGS}_${GCC}_${enable_debug}" = _no_yes],
  [AC_MSG_WARN([--enable-debug: don't know how to modify CFLAGS for this compiler])])
])dnl
