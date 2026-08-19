# Process this file with automake to produce Makefile.in
lib_LTLIBRARIES = libdslfapi.la

libdslfapi_sources = \
   dsl_fapi.c

libdslfapi_extra_sources = \
   @top_srcdir@/src/dsl_fapi_common.h \
   @top_srcdir@/src/dsl_fapi_debug.h \
   @top_srcdir@/src/ulogging.h \
   @top_srcdir@/include/dsl_fapi.h \
   @top_srcdir@/include/dsl_fapi_safec_wrapper.h

libdslfapi_la_SOURCES = \
   $(libdslfapi_sources)

EXTRA_DIST = \
   $(libdslfapi_extra_sources)

AM_CFLAGS = -DLINUX -D__LINUX__ \
            -I@top_srcdir@/include/ \
            @IFXOS_INCLUDE_PATH@ \
            @DRIVER_INCLUDE_PATH@

if SAFEC_SUPPORT
AM_CFLAGS += @SAFEC_INCL_PATH@
endif

AM_LDFLAGS = -L@builddir@ \
             @IFXOS_LIBRARY_PATH@

libdslfapi_la_CFLAGS = $(AM_CFLAGS)
libdslfapi_la_LDFLAGS = -version-info 0:0:0

if SAFEC_SUPPORT
libdslfapi_la_LDFLAGS += -l@SAFEC_LIB_NAME@
endif


