ponmcc_interface = \
   ../include/pon/pon_mcc.h

bin_PROGRAMS = ponmcc

includedir = $(prefix)/include/pon
include_HEADERS = $(ponmcc_interface)

AM_CFLAGS = \
	-I@top_srcdir@/include \
	-Wall \
	-Wextra \
	-Wformat-security \
	-Wmissing-noreturn \
	-Wmissing-declarations \
	-Wredundant-decls \
	-Wmissing-prototypes \
	-Wnested-externs \
	-Winit-self \
	-Wundef \
	-Waggregate-return

ponmcc_DEPENDENCIES = @builddir@/../lib/.libs/libponmcc.la
ponmcc_LDADD = -L@builddir@/../lib/.libs/ -lponmcc
ponmcc_SOURCES = ponmcc.c

check-style:
	for f in $(filter %.h %.c,$(DISTFILES)); do \
		$(CHECK_SYNTAX) $(addprefix @abs_srcdir@/,$$f); \
	done
