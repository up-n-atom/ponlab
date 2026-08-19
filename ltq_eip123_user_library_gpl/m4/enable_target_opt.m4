#serial 1

dnl Macro that provides the --enable-target option
AC_DEFUN([ENABLE_TARGET_OPT],
[
AC_ARG_ENABLE([target],
  [AS_HELP_STRING([--enable-target=@<:@custom|versatile@:>@], [select target])],
  [case $enableval in
     custom|versatile) : ;;
     *) AC_MSG_ERROR([--enable-target: unknown target: "$enableval"]) ;;
  esac],
  [enable_target=custom]
)
])dnl

