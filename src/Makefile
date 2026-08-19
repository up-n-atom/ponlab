ifneq ($(KERNELRELEASE),)

ccflags-y += \
	-I$(src)/ \
	-I$(src)/../include

obj-$(CONFIG_PON_MBOX_DRV)  += mod_pon_mbox.o
mod_pon_mbox-objs += \
	drv_pon_mbox.o \
	drv_pon_mbox_sfp.o \
	drv_pon_mbox_serdes.o \
	drv_pon_mbox_counters.o \
	drv_pon_mbox_counters_conv.o \
	drv_pon_mbox_counters_update.o \
	drv_pon_mbox_counters_autoupdate.o

else
check-style:
	@for f in *.h *.c Makefile; do \
		if [ "$$f" = *.mod.c ]; then \
			continue; \
		fi; \
		$(CHECK_SYNTAX) $$f; \
	done
endif
