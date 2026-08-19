## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = foreign

pontop_SOURCES = \
	pon_top.h \
	dump.h \
	uci_config.h \
	pon_top.c \
	dump.c \
	uci_config.c

bin_PROGRAMS = pontop

AM_CPPFLAGS =  \
	-I@srcdir@ \
	@PONLIB_INCLUDE_PATH@ \
	@GPONLIBS_INCLUDE_PATH@

AM_CFLAGS= \
	-Wall \
	-Wextra -Wno-unused-parameter \
	-Wno-sign-compare \
	-Wformat-security \
	-Wmissing-noreturn \
	-Wmissing-declarations \
	-Wredundant-decls \
	-Wmissing-prototypes \
	-Wnested-externs \
	-Winit-self \
	-Wundef \
	-Waggregate-return

AM_CFLAGS+=-DLINUX

pontop_LDADD=-lpon -ltop -lpthread -lm -luci -L.

pontop_CFLAGS=$(AM_CFLAGS)

check-style:
	for f in $(filter %.h %.c,$(DISTFILES)); do \
		$(CHECK_SYNTAX) $(addprefix @abs_srcdir@/,$$f); \
	done
