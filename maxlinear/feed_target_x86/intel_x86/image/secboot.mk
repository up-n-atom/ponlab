SECUREBOOT:=
ifeq ($(CONFIG_INTEL_X86_SECBOOT),y)
ifeq ($(CONFIG_INTEL_X86_KEYS_PROVISION),y)
SECUREBOOT:=y
endif
endif

ifeq ($(SECUREBOOT),y)
ifeq ($(CONFIG_INTEL_X86_CERTIFICATION_SUPPORT),y)
	CERT :=	-cert $(CONFIG_INTEL_X86_CERTIFICATION)
else
	CERT:=
endif

define Build/sign-image
	$(CONFIG_INTEL_X86_SIGNTOOL) sign -type BLw -infile $@ \
		-prikey $(CONFIG_INTEL_X86_PRIVATE_KEY) \
		-wrapkey $(CONFIG_INTEL_X86_PROD_UNIQUE_KEY) \
		-cert $(CONFIG_INTEL_X86_CERTIFICATION) \
		-encattr -kdk -sm -secure -pubkeytype otp \
		-algo aes256 \
		-attribute 0x80000000=$(CONFIG_INTEL_X86_AIC_BASEADDR) \
		-attribute 0x80000002=$(CONFIG_INTEL_X86_TEP_BASEADDR) \
		-attribute 0x80000006=0x0 \
		-outfile $@.tmp
	mv $@.tmp $@
endef

define Build/sign-add-header
	mkimage -A mips -O linux -T kernel -a 0x00 -C none \
		-e 0x00 -n 'OpenWrt fullimage' \
		-d $@ $@.tmp
	cp $@.tmp $@
endef

else

define Build/sign-image
endef

define Build/sign-add-header
endef

endif
