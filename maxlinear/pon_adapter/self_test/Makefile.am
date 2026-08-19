bin_PROGRAMS=pon_adapter

pon_adapter_SOURCES = main.c

AM_CFLAGS = -I@top_srcdir@/include/ \
			-Wall

check-style:
	for f in $(filter %.h %.c,$(DISTFILES)); do \
		$(CHECK_SYNTAX) $(addprefix @abs_srcdir@/,$$f); \
	done
