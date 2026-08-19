################################################################################
#
# Makefile
#  Description:
#  Makefile for the Packet Processor Network Adapter driver
#
#  SPDX-License-Identifier: GPL-2.0-only
#  Copyright (C) 2018 Intel Corporation
#  Copyright (C) 2020-2022 MaxLinear, Inc.
#
################################################################################

TARGET := pp_net_adapter

# Enable warning as errors
ccflags-y += -Wall -Werror

# Add other modules symbols
export KBUILD_EXTRA_SYMBOLS=$(PKG_CONFIG_SYSROOT_DIR)$(datadir)/modules/fpgapci.symvers  \
			    $(PKG_CONFIG_SYSROOT_DIR)$(datadir)/modules/learning.symvers \
                            $(PKG_CONFIG_SYSROOT_DIR)$(datadir)/modules/pp_drv.symvers

$(TARGET)-y += pp_network_adapter.o

ifeq ($(CONFIG_SOC_LGM),y)
$(TARGET)-$(CONFIG_DEBUG_FS) += pp_network_adapter_debugfs.o
$(TARGET)-$(CONFIG_DEBUG_FS) += pp_network_adapter_dut.o
else ifeq ($(CONFIG_PPV4_LGM),y)
$(TARGET)-$(CONFIG_DEBUG_FS) += pp_network_adapter_debugfs.o
$(TARGET)-$(CONFIG_DEBUG_FS) += pp_network_adapter_dut.o
endif

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

