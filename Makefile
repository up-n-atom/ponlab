#
# Makefile for the mcast_helper.
#

obj-$(CONFIG_MCAST_HELPER) += mcast_helper.o

ifneq ($(KERNELRELEASE),)
else
SRC := $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC)

modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules_install

clean:
	rm -f *.o *~ core .depend .*.cmd *.ko *.mod.c .*.o.*
	rm -f Module.markers Module.symvers modules.order
	rm -rf .tmp_versions Modules.symver Module.symvers.tmp
	rm -rf .built .built_* .configured_* .dep_files ipkg-* .pkgdir .prepared*
	rm -rf .quilt_* .source_*
endif
