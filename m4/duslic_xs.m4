dnl Option used to define the DC/DC converter type on the board HW
dnl --------------------------------------------------------------
dnl DXS_DCDC_HW_CHECK()
dnl
dnl specify with --enable-dcdc-hw = <IBB12|CIBB12|IB12|BB48|IFB3|IFB12|CIFB3|CIFB12|IBGD12|IBVD3|EVAL>
dnl
AC_DEFUN([DXS_DCDC_HW_CHECK],
[
   AC_MSG_CHECKING(for DC/DC converter type)
   # m4 definitions required to pass value in to the help message
   m4_define('DCDC_NAMES',"IBB12|CIBB12|IB12|BB48|IFB3|IFB12|CIFB3|CIFB12|IBGD12|IBVD3|IBVD12")
   AC_SUBST([DCDC_NAMES],[m4_defn('DCDC_NAMES')])

   AC_ARG_ENABLE(dcdc-hw,
      AS_HELP_STRING(
         [--enable-dcdc-hw@<:@=ARG@:>@],
         [Define the DC/DC converter type of the target HW.
          "IBB12": Inverting Buck-Boost Converter 12V,
          "CIBB12": Combined Inverting Buck-Boost Coverter 12V,
          "IB12": Inverting Boost Converter 12V,
          "BB48": Buck-or-Boost Converter -48V,
          "IFB3": Inverting Flyback Converter 3.3V,
          "IFB12": Inverting Flyback Converter 12V,
          "CIFB3": Combined Inverting Flyback Converter 3.3V,
          "CIFB12": Combined Inverting Flyback Converter 12V,
          "IBGD12": Inverting Boost Converter with Gate Driver 12V,
          "IBVD3": Inverting Boost Converter with Voltage Doubler 3.3V,
          "IBVD12": Inverting Boost Converter with Voltage Doubler 6-12V]
      ),
      [DCDC_NAME=$enableval])

   # check if DC/DC HW was specified
   if [[ "x$DCDC_NAME" = "x" ]]; then
      AC_MSG_ERROR([Define which DC/DC converter is on your board using --enable-dcdc-hw, available values are $DCDC_NAMES])
   fi

   case $DCDC_NAME in
      IBB12 | CIBB12 | IB12 | BB48 | IFB3 | IFB12 | CIFB3 | CIFB12 | IBGD12 | IBVD3 | IBVD12 | EVAL)
         AC_DEFINE_UNQUOTED([DXS_DCDC_TYPE],["$DCDC_NAME"],[Define the DC/DC converter type of the target HW.])
         ;;
      *)
         AC_MSG_ERROR([Invalid DC/DC converter value, available values are $DCDC_NAMES])
         ;;
   esac

   AC_MSG_RESULT([$DCDC_NAME]);
])

dnl DXS_INCL_CHECK([DEFAULT-PATH], [ACTION-IF-PRESENT], [ACTION-IF-MISSED])
dnl ----------------------------------------------------------
dnl
dnl Checks for drv_duslic_xs
dnl specify --with-dxt-incl
dnl If not supplied it checks for default and returns error when
dnl the header file was not found.
dnl
AC_DEFUN([DXS_INCL_CHECK],
[
	WITH_DRV_INCL_CHECK(
			[dxs],
			[Duslic-XS driver],
			[drv_dxs drv_dxs-* drv_duslic_xs drv_duslic_xs-* voice_duslic_xs_drv],
			[drv_dxs_io.h],
			[$1],
			[
				AC_SUBST([DUSLICXS_INCL_PATH],[$cached_with_dxs_incl])
				ifelse([$2],,[:],[$2])
			],
			[ifelse([$3],,[:],[$3])]
		)
])dnl

dnl DXS_DEVICE_ENABLE([1- ACTION_IF_SET])
dnl ----------------------------------------------------------
dnl
dnl Set DXS support for LT lib
dnl by specifing --enable-dxs.
dnl
AC_DEFUN([DXS_DEVICE_ENABLE],
[
    DEVICE_ENABLE([dxs],[DXS],[ifelse([$1],,[:],[$1])])

])dnl


dnl DXS_SPI_8BIT_CHECK
dnl ----------------------------------------------------------
dnl
dnl Enable 8-bit SPI access mode
dnl
AC_DEFUN([DXS_SPI_8BIT_CHECK],
[
   AC_MSG_CHECKING(for SPI 8-bit mode)
   AC_ARG_ENABLE(8bit-spi,
      AS_HELP_STRING(
         [--enable-8bit-spi],
         [Enable 8-bit SPI access mode]
      ),
      [
       if test $enableval = 'yes'; then
          AC_MSG_RESULT([enabled])
          AC_DEFINE_UNQUOTED([DXS_SPI_8BIT_ACCESS],[1],[DUSLIC XS SPI 8-bit access])
       else
          AC_MSG_RESULT([disabled])
       fi
      ],
      [
          AC_MSG_RESULT([disabled (default)])
      ]
  )
])dnl
