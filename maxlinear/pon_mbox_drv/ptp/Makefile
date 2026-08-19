ifneq ($(KERNELRELEASE),)

ccflags-y += \
	-I$(src)/../include

obj-$(CONFIG_PON_PTP_DRV)  += mod_pon_ptp.o
mod_pon_ptp-objs += \
	pon_ptp.o

else
check-style:
	@for f in *.h *.c Makefile; do \
		if [ "$$f" = *.mod.c ]; then \
			continue; \
		fi; \
		$(CHECK_SYNTAX) $$f; \
	done
endif
