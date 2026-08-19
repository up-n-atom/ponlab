## Process this file with automake to produce Makefile.in

## library
lib_LIBRARIES = libtop.a

AM_CFLAGS= \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
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

if ENABLE_LINUX
AM_CFLAGS+=-DLINUX
endif ENABLE_LINUX

if ENABLE_ECOS
AM_CFLAGS+=-DECOS
endif ENABLE_ECOS

libtop_a_SOURCES = \
	top.c \
	top_common.c \
	top_ecos.c \
	top_linux.c

pkginclude_HEADERS = \
	top.h \
	top_common.h \
	top_ecos.h \
	top_linux.h \
	top_std_defs.h

nodist_pkginclude_HEADERS = \
	top_config.h

check-style:
	for f in $(filter %.h %.c,$(DISTFILES)); do \
		$(CHECK_SYNTAX) $$f; \
	done
