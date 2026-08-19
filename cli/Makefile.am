## Process this file with automake to produce Makefile.in

if !PON_LIB_SIMULATOR
bin_PROGRAMS = pon ponmbox
endif

lib_LIBRARIES = libpon_cli.a

libpon_cli_a_SOURCES = \
	fapi_pon_cli.c \
	fapi_pon_cli_ext.c \
	pon_cli.h

pon_SOURCES = \
	pon.c

poncli_CFLAGS=$(AM_CFLAGS) \
	-DLINUX -D__LINUX__ \
	-DINCLUDE_CLI_SUPPORT \
	@CLI_INCLUDE_PATH@ \
	-I@srcdir@/../include/ -I@srcdir@/../src/ -I.

pon_LDFLAGS=$(AM_LDFLAGS) @CLI_LIBRARY_PATH@

pon_LDADD = libpon_cli.a -L@builddir@/../src/.libs/ -lpon -lcli -lpthread

if PON_LIB_SIMULATOR
pon_LDADD += -lrt
endif

AM_CFLAGS = -DLINUX -D__LINUX__ \
	-DINCLUDE_CLI_SUPPORT \
	-I@top_srcdir@/include/ \
	-I@top_srcdir@/src/ \
	@CLI_INCLUDE_PATH@ \
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
	-Waggregate-return \
	-Wcast-align \
	-Wswitch-default

libpon_cli_a_CFLAGS = $(AM_CFLAGS)

ponmbox_LDADD = -lnl-3 -lnl-genl-3
ponmbox_CFLAGS=$(AM_CFLAGS) \
	@LIBNL_INCLUDE_PATH@ \
        -Wno-unused-parameter \
        -Wno-sign-compare

ponmbox_SOURCES = \
        ponmbox.c

EXTRA_DIST =

check-style:
	for f in $(filter %.h %.c,$(DISTFILES)); do \
		$(CHECK_SYNTAX) $(addprefix @abs_srcdir@/,$$f); \
	done
