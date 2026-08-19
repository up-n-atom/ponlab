# Target objects
TARGET := dpmpa-dpm-hal

lib-y += datapath_misc.o datapath_gswip.o
lib-y += datapath_ppv4.o
lib-y += datapath_ppv4_api.o
lib-y += datapath_rx.o datapath_tx.o

ifneq ($(CONFIG_DPM_DATAPATH_HAL_GSWIP32),)
lib-y += datapath_ppv4_session.o datapath_spl_conn.o
endif

ifneq ($(CONFIG_DPM_DATAPATH_SWITCHDEV),)
lib-y += datapath_switchdev.o datapath_ext_vlan.o datapath_tc_asym_vlan.o
endif

lib-y += datapath_proc.o
ifneq ($(KERNELRELEASE),)
lib-y := dpmpa-dpm-hal.o
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
