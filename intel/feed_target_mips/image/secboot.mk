SECUREBOOT:=$(if $(and $(CONFIG_INTEL_MIPS_SECBOOT),$(CONFIG_INTEL_MIPS_KEYS_PROVISION)),y)

ifeq ($(SECUREBOOT),y)

CERT:=$(if $(CONFIG_INTEL_MIPS_CERTIFICATION_SUPPORT),-cert $(CONFIG_INTEL_MIPS_CERTIFICATION))

define Build/sign-image
	$(CONFIG_INTEL_MIPS_SIGNTOOL)  sign -type BLw  -infile $@ \
		-prikey $(CONFIG_INTEL_MIPS_PRIVATE_KEY) \
		-wrapkey $(CONFIG_INTEL_MIPS_PROD_UNIQUE_KEY) \
		-attribute rollback=$(CONFIG_INTEL_MIPS_ROLLBACK) \
		-attribute 0x80000000=$(KERNEL_ENTRY) \
		-attribute 0x80000001=0x00000001 \
		-attribute 0x80000002=$(KERNEL_LOADADDR) \
		-encattr \
		$(CERT) \
		-kdk \
		-pubkeytype otp \
		-algo aes256 \
		-outfile $@.tmp
	mv $@.tmp $@
endef

define Build/sign-add-header
	mkimage -A mips -O linux -T kernel -a 0x00 -C none \
		-e 0x00 -n 'UGW Secure Image' \
		-d $@ $@.tmp
	mv $@.tmp $@
endef

else

define Build/sign-image
endef

define Build/sign-add-header
endef

endif
