# Process this file with automake to produce Makefile.in

lib_LTLIBRARIES = libponimg.la
bin_PROGRAMS = pon_sw_upgrade pon_img_split

libponimg_la_extra = \
	../include/pon_img_register.h\
	../include/pon_img.h\
	../include/pon_uboot.h\
	pon_img_common.h\
	pon_img_debug.h

libponimg_la_SOURCES = \
	pon_img.c\
	pon_uboot.c\
	pon_img_register.c\
	pon_img_debug.c\
	me/pon_sw_image.c

pon_sw_upgrade_SOURCES = pon_sw_upgrade.c

pon_img_split_SOURCES = pon_img_split.c

EXTRA_DIST = \
   $(libponimg_la_extra)

AM_CFLAGS = -DLINUX -D__LINUX__ \
	-I@top_srcdir@/include/ \
	@IFXOS_INCLUDE_PATH@ \
	@PON_ADAPTER_INCLUDE_PATH@ \
	-Wall \
	-Wextra -Wno-sign-compare \
	-Wformat-security \
	-Wmissing-noreturn \
	-Wmissing-declarations \
	-Wredundant-decls \
	-Wmissing-prototypes \
	-Wnested-externs \
	-Winit-self \
	-Wundef \
	-Waggregate-return

AM_LDFLAGS = -L@builddir@ \
	     @IFXOS_LIBRARY_PATH@ \
	     @PON_ADAPTER_LIBRARY_PATH@ \
	    -Wl,--no-undefined

libponimg_la_CFLAGS = $(AM_CFLAGS) -DINCLUDE_DEBUG_SUPPORT

libponimg_la_LDFLAGS = $(AM_LDFLAGS)

libponimg_la_LIBADD = -ladapter -lubox -lifxos

pon_sw_upgrade_DEPENDENCIES = libponimg.la
pon_sw_upgrade_LDADD = -lponimg -lubus

check-style:
	for f in $(filter %.h %.c,$(DISTFILES)); do \
		$(CHECK_SYNTAX) $(addprefix @abs_srcdir@/,$$f); \
	done
