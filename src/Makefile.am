# Process this file with automake to produce Makefile.in

lib_LTLIBRARIES = libpon.la

pon_adapter_sources = \
   ../adapter/fapi_pon_mapper_table.c \
   ../adapter/fapi_pon_mapper_table.h \
   ../adapter/fapi_pon_pa_common.h \
   ../adapter/fapi_pon_pa_event.c \
   ../adapter/fapi_pon_pa_register.c \
   ../adapter/fapi_pon_pa_register.h \
   ../adapter/fapi_pon_pa_twdm.c \
   ../adapter/fapi_pon_pa_twdm.h \
   ../adapter/me/fapi_pon_me_ani_g_alarm.c \
   ../adapter/me/fapi_pon_me_ani_g.c \
   ../adapter/me/fapi_pon_me_counters.c \
   ../adapter/me/fapi_pon_me_enh_sec_ctrl.c \
   ../adapter/me/fapi_pon_me_olt_g.c \
   ../adapter/me/fapi_pon_me_onu_dyn_pwr_mngmt_ctrl.c \
   ../adapter/me/fapi_pon_me_onu_g.c \
   ../adapter/me/fapi_pon_me_twdm_system_profile.c \
   ../adapter/me/fapi_pon_me_twdm_channel.c

pkginclude_HEADERS = \
   ../include/fapi_pon_alarms.h \
   ../include/fapi_pon_aon.h \
   ../include/fapi_pon_develop.h \
   ../include/fapi_pon_error.h \
   ../include/fapi_pon_events.h \
   ../include/fapi_pon_gpon.h \
   ../include/fapi_pon_twdm.h \
   ../include/fapi_pon.h

libpon_la_extra = \
   fapi_pon_core.h \
   fapi_pon_debug.h \
   fapi_pon_os.h

libpon_la_SOURCES = \
   fapi_pon_alarms.c \
   fapi_pon_api.c \
   fapi_pon_core.c \
   fapi_pon_event.c

if INCLUDE_PON_ADAPTER
libpon_la_SOURCES += $(pon_adapter_sources)
endif

libnl_library = -lnl-3 -lnl-genl-3

EXTRA_DIST = \
   $(libpon_la_extra) \
   $(pon_adapter_sources)

AM_CFLAGS = -DLINUX -D__LINUX__ \
	-D_GNU_SOURCE \
	-I@top_srcdir@/include/ \
	@LIBNL_INCLUDE_PATH@ \
	@MBOXDRV_INCLUDE_PATH@ \
	@PON_ADAPTER_INCLUDE_PATH@ \
	-Wall \
	-Wextra \
	-Wno-sign-compare \
	-Wformat-security \
	-Wmissing-noreturn \
	-Wmissing-declarations \
	-Wredundant-decls \
	-Wmissing-prototypes \
	-Wnested-externs \
	-Winit-self \
	-Wundef \
	-Waggregate-return \
	-Wcast-align \
	-Wswitch-default

AM_LDFLAGS = -L@builddir@ \
	     @LIBNL_LIBRARY_PATH@ \
	     @PON_ADAPTER_LIBRARY_PATH@ \
	     -lpthread \
	     -Wl,--no-undefined

libpon_la_CFLAGS = $(AM_CFLAGS) -DINCLUDE_DEBUG_SUPPORT

libpon_la_LDFLAGS = $(AM_LDFLAGS)

libpon_la_LIBADD = $(libnl_library) -lm
if INCLUDE_PON_ADAPTER
libpon_la_LIBADD += -ladapter
endif

check-style:
	for f in $(filter %.h %.c,$(DISTFILES)); do \
		$(CHECK_SYNTAX) $(addprefix @abs_srcdir@/,$$f); \
	done
