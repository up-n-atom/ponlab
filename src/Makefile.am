ACLOCAL_AMFLAGS = -I m4

if USE_ONEWIFI_UCI_TRANSITION
lib_LTLIBRARIES = libswpal.la
libswpal_la_SOURCES = uci_wrapper.c
libswpal_la_LDFLAGS = -luci -lpthread
AM_CPPFLAGS = -Wall -Werror -I${top_srcdir}/include $(CPPFLAGS) -DPACKAGE_ID=\"WAVE_HAL\" -DLOGGING_ID -DLOG_LEVEL=6 -DLOG_TYPE=1 -DYOCTO \
                  -Werror=format-security -Wl,-strip-debug -ffunction-sections -fdata-sections -Wno-maybe-uninitialized
AM_LDFLAGS =  -Wl,--gc-sections
pkgconfigdir = $(libdir)/pkgconfig
include_HEADERS = $(srcdir)/include/uci_wrapper.h
else
# dynamic libraries
lib_LTLIBRARIES = libswpal.la
libswpal_la_SOURCES = uci_wrapper.c uci_tools.c
libswpal_la_LDFLAGS = -luci -lpthread

lib_LTLIBRARIES += libswpal_utils.la
libswpal_utils_la_SOURCES = uci_tools.c

# static library
lib_LTLIBRARIES += libswpal_static.la
libswpal_static_la_SOURCES = uci_wrapper.c uci_tools.c
libswpal_static_la_CFLAGS = -DWAVEAPI_USE_EXTERN_LOG
libswpal_static_la_LDFLAGS = -luci -lpthread

AM_CPPFLAGS = -Wall -Werror -I${top_srcdir}/include $(CPPFLAGS) -DPACKAGE_ID=\"WAVE_HAL\" -DLOGGING_ID -DLOG_LEVEL=6 -DLOG_TYPE=1 -DYOCTO \
                  -Werror=format-security -Wl,-strip-debug -ffunction-sections -fdata-sections -Wno-maybe-uninitialized
AM_LDFLAGS =  -Wl,--gc-sections

pkgconfigdir = $(libdir)/pkgconfig
include_HEADERS = $(srcdir)/include/uci_wrapper.h \
                  $(srcdir)/include/spal_logging.h

bin_PROGRAMS = lite_nc
lite_nc_SOURCES = $(srcdir)/lite_nc/nc_help_lite.c \
                  $(srcdir)/lite_nc/nc_help_lite.h \
                  $(srcdir)/lite_nc/nc_lite.c
endif # USE_ONEWIFI_UCI_TRANSITION
