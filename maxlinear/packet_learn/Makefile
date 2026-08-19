# Makefile
#
# Description:
# Makefile to build learning module.
#
# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2020 Intel Corporation
#
################################################################################

TARGET := learning
ccflags-y += -Wall -Werror -Wimplicit-fallthrough=3

# Target objects
$(TARGET)-y += pktprs.o
$(TARGET)-$(CONFIG_DEBUG_FS) += pktprs_debugfs.o

ifneq ($(KERNELRELEASE),)

obj-m += $(TARGET).o

else

SRC := $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC)

modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules_install

clean:
	rm -f *.o *~ core .depend .*.cmd *.ko *.mod.c
	rm -f Module.markers Module.symvers modules.order
	rm -rf .tmp_versions Modules.symvers

endif
