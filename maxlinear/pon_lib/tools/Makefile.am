## Process this file with automake to produce Makefile.in
bin_PROGRAMS = pon_test

pon_test_SOURCES = pon_test.c

AM_CFLAGS = -Wall -Wextra -Wno-unused-parameter \
		-I@top_srcdir@/src/ \
		-I@top_srcdir@/include/ \
		-DLINUX

pon_test_LDADD = @builddir@/../src/.libs/libpon.so -lpthread

pon_test_CFLAGS = $(AM_CFLAGS)

check-style:
	for f in $(filter %.h %.c,$(DISTFILES)); do \
		$(CHECK_SYNTAX) $(addprefix @abs_srcdir@/,$$f); \
	done
