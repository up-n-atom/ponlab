#
# Copyright (C) 2010-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk
include $(INCLUDE_DIR)/kernel-defaults.mk
-include ./packages.mk

define Image/Prepare/add-servicelayer-schema
	$(eval vendordir=$(1)/$(call qstrip,$(VENDOR_PATH)))
	$(eval xmlsmerge=LD_LIBRARY_PATH="$(STAGING_DIR)/host/lib/:${LD_LIBRARY_PATH}" $(STAGING_DIR_HOST)/bin/python \
		$(STAGING_DIR_HOST)/bin/merge.py -v $(STAGING_DIR_HOST)/bin/target_validate.xsd)
	$(eval sslbase64=$(STAGING_DIR_HOST)/bin/openssl aes-256-cbc -base64)
	- cd $(vendordir)/xml/ && { \
		ls ./*_control.xml ./*_data.xml >/dev/null 2>&1 && { \
			$(xmlsmerge) *_control*.xml -o=control.xml || exit 1; \
			$(CP) $(vendordir)/xml/control.xml  $(vendordir)/csd/etc/ctrlcfg; \
			$(xmlsmerge) *_data*.xml -o=data.xml || exit 1; \
			$(SED) "s/\(<HwModelName.*>\).*\(<.*\)/\1$(CONFIG_HW_MODEL_NAME)\2/" data.xml; \
			$(SED) "s/\(<HwVersion.*>\).*\(<.*\)/\1$(CONFIG_HW_MODEL_VERSION)\2/" data.xml; \
			$(SED) "s/\(<SwUpdtTimeStamp.*>\).*\(<.*\)/\1$(TIME_STAMP)\2/" data.xml; \
			$(CP) $(vendordir)/xml/data.xml  $(vendordir)/csd/etc/datacfg; \
			rm -f $(vendordir)/xml/*; \
		} \
	} 2> $(vendordir)/exec_python_stat
	[ -f $(vendordir)/exec_python_stat ] && { \
		if  grep -i "ImportError\|ModuleNotFoundError" $(vendordir)/exec_python_stat ; then \
			echo "Error: Failed DB merge due to either all python3 packages are not installed or forgot to set PYTHONPATH"; \
			echo "Please set PYTHONPATH or install required modules before full make"; \
			rm -f $(vendordir)/exec_python_stat; \
			exit 1; \
		else \
			rm -f $(vendordir)/exec_python_stat; \
			echo "DB merged is done successfully!"; \
		fi \
	}
	$(if $(CONFIG_TARGET_ROOTFS_INITRAMFS),$(call Kernel/CompileImage/Initramfs))
endef

define Image/Prepare
        $(if $(CONFIG_PACKAGE_csd),$(call Image/Prepare/add-servicelayer-schema,$(TARGET_DIR)))
endef

# Default uImage uses LINUX_KARCH (instead of ARCH), which for x86_64
# will be forced to x86. For this reason, we define our own uImage
# command which uses ARCH.

ifeq ($(CONFIG_INTEL_X86_KERNEL_METADATA),y)
ifeq ($(CONFIG_INTEL_X86_SECBOOT),y)
ifeq ($(CONFIG_INTEL_X86_EXTERNAL_IMAGE_SIGNING),y)
define Build/uImage-x86_64
	$(call Build/kernel-metadata)
	mkimage -A x86_64 \
		-O linux -T kernel \
		-C $(1) -a $(KERNEL_LOADADDR) -e $(if $(KERNEL_ENTRY),$(KERNEL_ENTRY),$(KERNEL_LOADADDR)) \
		-n '$(if $(UIMAGE_NAME),$(UIMAGE_NAME),$(call toupper,x86_64) LEDE Linux-$(LINUX_VERSION))' -d  $@ $@.new
	mkimage -A x86_64 \
		-O linux -T multi \
		-C none \
		-n 'URX Linux Kernel Container' \
		-d $(LINUX_KERNEL).metadata:$@.new $@.pad
	@mv $@.pad $@

        mkimage -A x86_64 \
                -O linux -T kernel \
                -C $(1) -a $(KERNEL_LOADADDR) -e $(if $(KERNEL_ENTRY),$(KERNEL_ENTRY),$(KERNEL_LOADADDR)) \
                -n '$(if $(UIMAGE_NAME),$(UIMAGE_NAME),$(call toupper,x86_64) LEDE Linux-$(LINUX_VERSION))' -d  $@ $@.new
        @mv $@.new $@
	mkdir -p $(BIN_DIR)/non_signed_image;
	cp $@ $(BIN_DIR)/non_signed_image;
endef

else
define Build/uImage-x86_64
	$(call Build/kernel-metadata)
	mkimage -A x86_64 \
		-O linux -T kernel \
		-C $(1) -a $(KERNEL_LOADADDR) -e $(if $(KERNEL_ENTRY),$(KERNEL_ENTRY),$(KERNEL_LOADADDR)) \
		-n '$(if $(UIMAGE_NAME),$(UIMAGE_NAME),$(call toupper,x86_64) LEDE Linux-$(LINUX_VERSION))' -d  $@ $@.new
	mkimage -A x86_64 \
		-O linux -T multi \
		-C none \
		-n 'URX Linux Kernel Container' \
		-d $(LINUX_KERNEL).metadata:$@.new $@.pad
	@mv $@.pad $@

	$(call Build/sign-image,$@)
	mkimage -A x86_64 \
		-O linux -T kernel \
		-C $(1) -a $(KERNEL_LOADADDR) -e $(if $(KERNEL_ENTRY),$(KERNEL_ENTRY),$(KERNEL_LOADADDR)) \
		-n '$(if $(UIMAGE_NAME),$(UIMAGE_NAME),$(call toupper,x86_64) LEDE Linux-$(LINUX_VERSION))' -d  $@ $@.new
	@mv $@.new $@
endef
endif
else
define Build/uImage-x86_64
	$(call Build/kernel-metadata)
	mkimage -A x86_64 \
		-O linux -T kernel \
		-C $(1) -a $(KERNEL_LOADADDR) -e $(if $(KERNEL_ENTRY),$(KERNEL_ENTRY),$(KERNEL_LOADADDR)) \
		-n '$(if $(UIMAGE_NAME),$(UIMAGE_NAME),$(call toupper,x86_64) LEDE Linux-$(LINUX_VERSION))' -d  $@ $@.new
	mkimage -A x86_64 \
		-O linux -T multi \
		-C none \
		-n 'URX Linux Kernel Container' \
		-d $(LINUX_KERNEL).metadata:$@.new $@.pad
	@mv $@.pad $@
endef
endif
else
define Build/uImage-x86_64
	mkimage -A x86_64 \
		-O linux -T kernel \
		-C $(1) -a $(KERNEL_LOADADDR) -e $(if $(KERNEL_ENTRY),$(KERNEL_ENTRY),$(KERNEL_LOADADDR)) \
		-n '$(if $(UIMAGE_NAME),$(UIMAGE_NAME),$(call toupper,x86_64) LEDE Linux-$(LINUX_VERSION))' -d $@ $@.new
	@mv $@.new $@
endef

endif

define Image/SignImage
	$(call BuildImage)
ifeq ($(CONFIG_INTEL_X86_EXTERNAL_IMAGE_SIGNING),y)
	cp -f fullimg_dtb.json $(BIN_DIR)/non_signed_image
	$(STAGING_DIR_HOST)/bin/signingutil sign $(BIN_DIR)/non_signed_image $(CONFIG_INTEL_X86_SIGNING_SER_USER) \
		$(CONFIG_INTEL_X86_SIGNING_SER_PASS) $(BIN_DIR)/non_signed_image/fullimg_dtb.json
	cp -f $(BIN_DIR)/non_signed_image/*.dtb $(BIN_DIR)/
	cp -f $(BIN_DIR)/non_signed_image/*.rootfs $(BIN_DIR)/
	cp -f $(BIN_DIR)/non_signed_image/*kernel.bin $(BIN_DIR)/$(IMG_PREFIX)-*-kernel.bin
	cp -f $(BIN_DIR)/non_signed_image/*fullimage.img $(BIN_DIR)/
	if [ -a $(BIN_DIR)/non_signed_image/tep_fw.bin.new.signed ]; then \
		cp -vf $(BIN_DIR)/non_signed_image/tep_fw.bin.new.signed $(BIN_DIR)/tep_fw.bin; \
	fi;
	rm -rf $(BIN_DIR)/uboot-lgp-*/*
	for f in $(BIN_DIR)/non_signed_image/*/; do \
		if [ -d $$$$f ]; then \
			cd $$$$f; \
			cp -vf u-boot-without-signing.lzimg.new.signed $(BIN_DIR)/$$$$(basename $$$$f)/u-boot-emmc.bin; \
			cp -vf u-boot-spl-without-signing.img.new.signed $(BIN_DIR)/$$$$(basename $$$$f)/u-boot-spl-emmc.bin; \
			cp -vf u-boot-mem-without-signing.bin.new.signed.asc $(BIN_DIR)/$$$$(basename $$$$f)/u-boot-recovery.asc; \
			cp -vf u-boot-mem-without-signing.bin.new.signed $(BIN_DIR)/$$$$(basename $$$$f)/u-boot-recovery.bin; \
			rm -f u-boot-without-signing.lzimg.new.signed u-boot-spl-without-signing.img.new.signed; \
			rm -f u-boot-mem-without-signing.bin.new.signed.asc u-boot-mem-without-signing.bin.new.signed; \
		fi; \
	done
	rm -f $(BIN_DIR)/non_signed_image/*fullimage.img
	rm -f $(BIN_DIR)/non_signed_image/*.dtb
	rm -f $(BIN_DIR)/non_signed_image/*.rootfs
	rm -f $(BIN_DIR)/non_signed_image/*kernel.bin
	rm -f $(BIN_DIR)/non_signed_image/*tep_fw.bin.new.signed
endif
endef

include lgm.mk

$(eval $(call Image/SignImage))
