#/******************************************************************************
#
#         Copyright (c) 2020, MaxLinear, Inc.
#         Copyright 2016 - 2020 Intel Corporation
#
#  For licensing information, see the file 'LICENSE' in the root folder of
#  this software module.
#
#*******************************************************************************/

builddir = .

#override CC := $(abs_top)/support/mtlkfront.sh $(CC)
override AM_CFLAGS = -Wall -Wno-multichar -fno-strict-aliasing -Werror -Wno-error=undef
override CFLAGS += -O2 -Wno-multichar
override CFLAGS += -I$(builddir)/.
override CFLAGS += -I$(abs_top)
override CFLAGS += -I$(builddir)/drivers/

all: rx_measure_handler

LINK = $(LDFLAGS) $(LD_LIBS) $(LD_LIBS) $(AM_CFLAGS) $(CFLAGS) -o $@ -I$(builddir)/drivers/

objs = \
	rx_measure_handler.o	\
	drivers/driver_nl80211.o	\


rx_measure_handler: $(objs)
	$(CC) $(LINK) $(objs)

clean:
	rm -f $(objs) rx_measure_handler

export abs_builddir = $(builddir)
