## Process this file with automake to produce Makefile.in

AM_CFLAGS = -DLINUX -D__LINUX__ \
	-I@top_srcdir@/include/ \
	-I@top_srcdir@/src/ \
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
	-Waggregate-return \
	-Wcast-align \
	-Wswitch-default

bin_PROGRAMS = pond

pond_SOURCES = \
	pond.c

pond_CFLAGS= -I. $(AM_CFLAGS)

pond_LDADD = @builddir@/../src/.libs/libpon.so -lpthread

if UBUS_ENABLE
pond_LDADD += -lubus
endif

check-style:
	for f in $(filter %.h %.c,$(DISTFILES)); do \
		$(CHECK_SYNTAX) $(addprefix @abs_srcdir@/,$$f); \
	done
