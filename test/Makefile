ifneq ($(KERNELRELEASE),)

ccflags-y += \
	-I$(src)/../include

obj-$(CONFIG_PON_MBOX_DRV)  += mod_pon_mbox_test.o
mod_pon_mbox_test-objs += \
	mbox_test_main.o

else
check-style:
	@for f in *.h *.c Makefile; do \
		if [ "$$f" = *.mod.c ]; then \
			continue; \
		fi; \
		$(CHECK_SYNTAX) $$f; \
	done
endif
