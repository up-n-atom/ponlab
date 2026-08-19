##################################################################################
#                                                                                #
#       Copyright © 2020-2021 MaxLinear, Inc.                                    #
#                                                                                #
#  For licensing information, see the file 'LICENSE' in the root folder of       #
#  this software module.                                                         #
##################################################################################

TARGET := lgm_pp_hal_drv

# Target objects
$(TARGET)-y += lgm_pp_hal.o

ifneq ($(CONFIG_LGM_TOE),)
$(TARGET)-objs     += lgm_lro_hal.o
endif
$(TARGET)-objs     += lgm_hw_litepath.o

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
ccflags-y += -Werror
