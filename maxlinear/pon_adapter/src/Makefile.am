lib_LTLIBRARIES = libadapter.la

libadapter_la_SOURCES = pon_adapter_mapper.c \
			pon_adapter_crc.c \
			pon_adapter.c

AM_CFLAGS = -I@top_srcdir@/include/ \
			-Wall \
			-Wmissing-prototypes

libadapter_la_CFLAGS = $(AM_CFLAGS)

libadapter_la_LDFLAGS = -Wl\
			--no-undefined

check-style:
	for f in $(filter %.h %.c,$(DISTFILES)); do \
		$(CHECK_SYNTAX) $(addprefix @abs_srcdir@/,$$f); \
	done
