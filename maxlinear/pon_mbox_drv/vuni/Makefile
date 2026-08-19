ifneq ($(KERNELRELEASE),)

ccflags-y += \
	-I$(src)/../include

obj-$(CONFIG_PON_HGU_VUNI_DRV)  += mod_pon_hgu_vuni.o
mod_pon_hgu_vuni-objs += \
	pon_hgu_vuni.o

else
check-style:
	@for f in *.h *.c Makefile; do \
		if [ "$$f" = *.mod.c ]; then \
			continue; \
		fi; \
		$(CHECK_SYNTAX) $$f; \
	done
endif
