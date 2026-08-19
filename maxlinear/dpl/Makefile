# Makefile
#
# Description:
# Makefile to build DoS Protection Layer module.
#
# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2020 Intel Corporation
# Copyright (C) 2020-2023 MaxLinear, Inc.
#
################################################################################

TARGET := dpl

# Enable warning as errors
ccflags-y += -Wall -Werror

# Target objects
$(TARGET)-y += dpl_lgm.o
$(TARGET)-y += dpl_monitor.o
$(TARGET)-y += dpl_logic.o
$(TARGET)-$(CONFIG_DEBUG_FS) += dpl_monitor_debugfs.o
$(TARGET)-$(CONFIG_DEBUG_FS) += dpl_lgm_debugfs.o
$(TARGET)-$(CONFIG_DEBUG_FS) += dpl_logic_debugfs.o

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
