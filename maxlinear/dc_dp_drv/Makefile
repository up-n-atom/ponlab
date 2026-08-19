ifeq ($(CONFIG_DIRECTCONNECT_DP_XRX750),y)
ifneq ($(CONFIG_MTH_MACDB),y)
override KBUILD_EXTRA_SYMBOLS += \
    $(PKG_CONFIG_SYSROOT_DIR)$(includedir)/datapath-mod/Module.symvers \
    $(PKG_CONFIG_SYSROOT_DIR)$(includedir)/ppdrv-mod/Module.symvers \
    $(PKG_CONFIG_SYSROOT_DIR)$(includedir)/cppp-mod/Module.symvers \
    $(PKG_CONFIG_SYSROOT_DIR)$(includedir)/mth-mod/Module.symvers
endif
endif
ifeq ($(CONFIG_DIRECTCONNECT_DP),y)
obj-m += common/ dc_mode/
endif
ifeq ($(CONFIG_DIRECTCONNECT_DP_TEST),y)
obj-m += test/
endif
ifeq ($(CONFIG_MTH_MACDB),y)
obj-m += mth/
override KBUILD_EXTRA_SYMBOLS += $(PKG_CONFIG_SYSROOT_DIR)$(includedir)/ppdrv-mod/Module.symvers
endif
export KBUILD_EXTRA_SYMBOLS

ifneq ($(KERNELRELEASE),)

else

SRC := $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC)

modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules_install

clean:
	rm -f common/{*.ko,*.o,.*.o.cmd}
	rm -f dc_mode/dc_mode/{*.ko,*.o,.*.o.cmd}
	rm -f test/{*.ko,*.o,.*.o.cmd}
endif
