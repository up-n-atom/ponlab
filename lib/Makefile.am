lib_LTLIBRARIES = libponmcc.la

EXTRA_DIST = \
	../include/pon/pon_mcc.h \
	../include/pon/pon_mcc_ikm.h

libponmcc_la_SOURCES = \
	pon_mcc_lib.c

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

libponmcc_la_LDFLAGS = -Wl,--no-undefined
libponmcc_la_LIBADD = -lnl-3 -lnl-genl-3

check-style:
	for f in $(filter %.h %.c,$(DISTFILES)); do \
		$(CHECK_SYNTAX) $(addprefix @abs_srcdir@/,$$f); \
	done
