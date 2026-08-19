#serial 1

dnl Macro that provides a yes/no option
AC_DEFUN([ENABLE_YES_NO_OPT],
[
AC_ARG_ENABLE([$1],
  [AS_HELP_STRING([--enable-$1], [enable $1 option])],
  [case $enableval in
    yes|no) : ;;
    *) AC_MSG_ERROR([--enable-$1: unrecognized ARG: "=$enableval": expected =yes or =no]) ;;
  esac])
])dnl
