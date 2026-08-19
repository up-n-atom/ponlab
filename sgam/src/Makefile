##################################################################################
#
#       Copyright (C) 2022-2023 MaxLinear, Inc.
#                                                                                #
##################################################################################
#
# Makefile for sgam_cli
#

CFLAGS += -Werror $(CFG_FLAG)

LDFLAGS += -lsafec

all: sgam_cli

sgam_cli.o: sgam_cli.c

sgam_cli: sgam_cli.o

clean:
	rm -f *.o sgam_cli
