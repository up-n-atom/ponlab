# Enable warning as errors
ccflags-y += -Wall -Werror -Wno-unused-function -fstack-usage $(extra_ccflags)
ifneq ($(CONFIG_KGB),)
ccflags-y += -Wframe-larger-than=180
endif

ifneq ($(CONFIG_DPM_DATAPATH),)
gwdpa-dpm-y = datapath_api.o datapath_misc.o datapath_notifier.o datapath_logical_dev.o datapath_instance.o datapath_soc.o datapath_qos.o datapath_tx.o datapath_rx.o datapath_cqm.o datapath_pce.o
gwdpa-dpm-y += datapath_swdev_v2.o datapath_br_vlan_v2.o
gwdpa-dpm-y += datapath_ioctl.o datapath_dflt_q_map.o datapath_api_fn.o
gwdpa-dpm-y += datapath_dev_list.o datapath_switchdev_common.o datapath_switchdev.o datapath_netdev_event.o
gwdpa-dpm-y += hal/datapath_misc.o hal/datapath_gswip.o
gwdpa-dpm-y += hal/datapath_ppv4.o hal/datapath_ppv4_api.o hal/datapath_rx.o hal/datapath_tx.o
gwdpa-dpm-y += hal/datapath_ppv4_session.o hal/datapath_spl_conn.o
gwdpa-dpm-y += hal/datapath_switchdev.o hal/datapath_ext_vlan.o hal/datapath_tc_asym_vlan.o
gwdpa-dpm-y += hal/datapath_proc.o hal/datapath_mib_proc.o
gwdpa-dpm-y += hal/datapath_mib.o
gwdpa-dpm-$(CONFIG_DEBUG_FS) += datapath_proc_api.o
gwdpa-dpm-$(CONFIG_DEBUG_FS) += datapath_gdb_wa.o
gwdpa-dpm-y += datapath_dts.o datapath_netdev.o

gwdpa-dpm-y += datapath_proc.o datapath_proc_qos.o
gwdpa-dpm-$(CONFIG_EVENT_TRACING) += datapath_trace.o
else
#workaround for bootcore in prx
gwdpa-dpm-y = datapath_dummy.o
endif

# otherwise trace header is not found
CFLAGS_datapath_trace.o +=  -I$(src)

ifneq ($(KERNELRELEASE),)
obj-m += gwdpa-dpm.o
else
SRC := $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC)

modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules_install

clean:
	rm -f *.o *~ core .depend .*.cmd *.ko *.mod.c .*.o.* hal/*.o hal/.*.o.cmd
	rm -f Module.markers Module.symvers modules.order
	rm -rf .tmp_versions Modules.symver Module.symvers.tmp
	rm -rf .built .built_* .configured_* .dep_files ipkg-* .pkgdir .prepared*
	rm -rf .*.su *.mod.su hal/.*.su *.su hal/*.su gwdpa-dpm.mod
endif
