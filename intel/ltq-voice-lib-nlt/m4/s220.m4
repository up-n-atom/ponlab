dnl S220_DEVICE_ENABLE([1- ACTION_IF_SET])
dnl ----------------------------------------------------------
dnl
dnl Set S220 support for LT lib
dnl by specifing --enable-s220.
dnl
AC_DEFUN([S220_DEVICE_ENABLE],
[
    DEVICE_ENABLE([s220],[S220],[ifelse([$1],,[:],[$1])])

])dnl

