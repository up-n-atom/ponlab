#serial 1

dnl Macro that provides capability to specify or probe endianness.
 --with-endian=big|little capability
AC_DEFUN([ENDIAN_FLAGS],
[
  endian_flag=
  AC_ARG_WITH([endian],
    [AS_HELP_STRING([--with-endian=@<:@big|little@:>@], [use big/little endian])],
    [endian=$withval],
    [endian=])
  AS_IF([test "x$endian" == xbig],
    [endian_flag=-DSB_CF_BIGENDIAN])
  AS_IF([test "x$endian" == xlittle],
    [endian_flag=-DSB_CF_LITTLEENDIAN])
  AS_IF([test "x$endian_flag" == x],
    [AC_C_BIGENDIAN([endian_flag=-DSB_CF_BIGENDIAN],
                    [endian_flag=-DSB_CF_LITTLEENDIAN],
                    [AC_MSG_FAILURE([Unknown byte-order. Please use --with-endian=big or --with-endian=little to specify byte order.])])])
  CPPFLAGS="$CPPFLAGS $endian_flag"
])dnl
