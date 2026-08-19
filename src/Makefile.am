#
#                              Copyright (c) 2012
#                            Lantiq Deutschland GmbH
#
#  For licensing information, see the file 'LICENSE' in the root folder of
#  this software module.
#
#


lib_LIBRARIES = lib_tapi_nlt_cpe.a

lib_tapi_nlt_cpe = \
	lib_tapi_lt_gr909.c

if LIB_TAPI_NLT_S220
lib_tapi_nlt_cpe += \
	lib_tapi_lt_gr909_s220.c
endif

if LIB_TAPI_NLT_VMMC
lib_tapi_nlt_cpe += \
	lib_tapi_lt_gr909_vmmc.c
endif

if LIB_TAPI_NLT_DXT
lib_tapi_nlt_cpe += \
	lib_tapi_lt_gr909_dxt.c
endif

if LIB_TAPI_NLT_DXS
lib_tapi_nlt_cpe += \
	lib_tapi_lt_gr909_dxs.c
endif

if LIB_TAPI_NLT_VINCPE
lib_tapi_nlt_cpe += \
	lib_tapi_lt_gr909_vincpe.c
endif

AM_CPPFLAGS = \
        -I@abs_top_srcdir@ \
        -I@abs_top_srcdir@/include \
        -I@KERNEL_INCL_PATH@ \
        -I@HL_TAPI_INCL_PATH@ \
        -I@IFXOS_INCL_PATH@

lib_tapi_nlt_cpe_a_SOURCES = \
	$(lib_tapi_nlt_cpe)

EXTRA_DIST = \
	$(lib_tapi_nlt_cpe) \
	@top_srcdir@/lib_tapi_nlt_version.h \
	@top_srcdir@/src/lib_tapi_nlt_strerrno.h \
	@top_srcdir@/src/lib_tapi_nlt_osmap.h \
	@top_srcdir@/vxworks_6.6/.cproject \
	@top_srcdir@/vxworks_6.6/.project \
	@top_srcdir@/vxworks_6.6/.wrmakefile \
	@top_srcdir@/vxworks_6.6/.wrproject \
	@top_srcdir@/script/prepare-vxworks-6-6.pl

AM_CFLAGS = -DLINUX -O2
