ifeq ($(SUBTARGET),lgm)

DATE_FMT = +%y-%m-%d
UIMAGE_NAME ?= $(shell date "$(DATE_FMT)")

# Fakeroot conf
FAKEROOT_PROG:=$(if $(CONFIG_PACKAGE_ugw-fakeroot), \
	ALTPATH="$(STAGING_DIR_ROOT)" CONFFILE="$(STAGING_DIR_HOST)/share/fakeroot/fakeroot.conf" \
	fakeroot -- $(STAGING_DIR_HOST)/bin/fakeroot.sh)

export STAGING_PREFIX=$(STAGING_DIR_HOST)

# Standalone dtb generation. The existing 'append-dtb' does not pass
# additional cflags argument (which is required in lgm for additional
# include flag).
define Build/dtb
	$(call Image/BuildDTB,$(DEVICE_DTS_DIR)/$(1).dts,$@.dtb,-I$(LINUX_DIR)/include)
	cat $@.dtb >> $@
endef

# generates dtb-ovarlay file.
define Build/dtbo
	$(call Image/BuildDTBO,$(DEVICE_DTS_DIR)/overlay_pon.dts,$@.dtbo,-I$(LINUX_DIR)/include)
	cat $@.dtbo >> $@
endef

# $(1) source dts file
# $(2) target dtb file
# $(3) extra CPP flags
# $(4) extra DTC flags
define Image/BuildDTB/sub
	$(TARGET_CROSS)cpp -nostdinc -x assembler-with-cpp \
		$(DTS_CPPFLAGS) \
		-I$(DTS_DIR) \
		-I$(DTS_DIR)/include \
		-I$(LINUX_DIR)/include/ \
		-undef -D__DTS__ $(3) \
		-o $(2).tmp $(1)
	$(LINUX_DIR)/scripts/dtc/dtc -O dtb \
		-i$(dir $(1)) $(4) \
	-@ \
		-o $(2) $(2).tmp
	$(RM) $(2).tmp
endef

ifeq ($(CONFIG_INTEL_X86_KERNEL_METADATA),y)
define Build/kernel-metadata
	./kernel_metadata.sh $(LINUX_KERNEL)
	mkenvimage -o $(LINUX_KERNEL).metadata.pad -s $(CONFIG_INTEL_X86_ENV_SZ) -r $(LINUX_KERNEL).metadata
	@mv $(LINUX_KERNEL).metadata.pad $(LINUX_KERNEL).metadata
	mkimage -A x86_64 \
                -O u-boot -T invalid -C none \
		-n "Metadata"\
		-d $(LINUX_KERNEL).metadata $(LINUX_KERNEL).metadata.pad
	mv $(LINUX_KERNEL).metadata.pad $(LINUX_KERNEL).metadata
endef
endif
ifeq ($(CONFIG_INTEL_X86_SECBOOT),y)
ifeq ($(CONFIG_INTEL_X86_EXTERNAL_IMAGE_SIGNING),y)
define Build/sign-image
endef
define Build/sign-rootfs
	echo "" > /dev/null
endef
define Build/fullimage
	# wrap rootfs to uImage
	mkimage -A $(LINUX_KARCH) -O linux -C lzma -T filesystem -a 0x00  \
		-e 0x00 -n 'LEDE RootFS' \
		-d $(2) $@.rootfs.wo_sign
	mkdir -p $(BIN_DIR)/non_signed_image
	mv $@.rootfs.wo_sign $(2)	
	cp -vf $(2) $(BIN_DIR)/non_signed_image
endef
else
define Build/sign-image
	$(CONFIG_INTEL_X86_SIGNTOOL) sign -type BLw -infile $@ \
		-prikey $(CONFIG_INTEL_X86_PRIVATE_KEY) \
		-wrapkey $(CONFIG_INTEL_X86_PROD_UNIQUE_KEY) \
		-cert $(CONFIG_INTEL_X86_CERTIFICATION) \
		-encattr -kdk -sm -secure -pubkeytype otp \
		-algo aes256 \
		-attribute 0x80000000=$(CONFIG_INTEL_X86_KERNEL_BASEADDR) \
		-attribute 0x80000002=$(CONFIG_INTEL_X86_KERNEL_BASEADDR) \
		-attribute 0x80000006=0x0 \
		-attribute rollback=$(CONFIG_INTEL_X86_KERNEL_ROLLBACKID) \
		-outfile $@.tmp
	mv $@.tmp $@
endef
define Build/sign-rootfs
	$(CONFIG_INTEL_X86_SIGNTOOL) sign -type BLw \
		-prikey $(CONFIG_INTEL_X86_PRIVATE_KEY) \
		-wrapkey $(CONFIG_INTEL_X86_PROD_UNIQUE_KEY) \
		-cert $(CONFIG_INTEL_X86_CERTIFICATION) \
		-encattr -kdk -sm -secure -pubkeytype otp \
		-algo aes256 \
		-attribute 0x80000000=0x40000000 \
		-attribute 0x80000002=0x40000000 \
		-attribute 0x80000006=0x0 \
		-attribute rollback=$(CONFIG_INTEL_X86_ROOTFS_ROLLBACKID) \
		-infile $@ \
		-outfile $@.tmp
	mv $@.tmp $@
endef
define Build/fullimage
	# wrap rootfs to uImage
	dd if=$(2) of=$@.rootfs bs=$(1) conv=sync;
	mkimage -A $(LINUX_KARCH) -O linux -C lzma -T filesystem -a 0x00  \
		-e 0x00 -n 'LEDE RootFS' \
		-d $@.rootfs $@.rootfs.pad

	echo "Sigining and generating the respective dtb.signed files"
	$(CONFIG_INTEL_X86_SIGNTOOL) sign -type BLw -prikey $(CONFIG_INTEL_X86_PRIVATE_KEY) -wrapkey $(CONFIG_INTEL_X86_PROD_UNIQUE_KEY) -encattr -kdk -sm -secure \
	-pubkeytype otp -algo aes256 -attribute 0x80000000=0x08000000 -attribute 0x80000002=0x08000000 -attribute 0x80000006=0x0 -attribute rollback=$(CONFIG_INTEL_X86_DTB_ROLLBACKID)\
	-cert $(CONFIG_INTEL_X86_CERTIFICATION) -infile $(3) -outfile $@.dtb.signed

	@echo "Waiting for the file to get signed.."
	@echo "Entering for retry mechanism if required dtb is not found...!"
	@tries=0; \
	while [ $$tries -lt 10 ]; do \
		sleep 2; \
		ls -ltr $@.dtb.signed; \
		if [ -f "$@.dtb.signed" ]; then \
			echo "File found...!"; \
			break; \
		fi; \
		echo "File not found..."; \
		$(CONFIG_INTEL_X86_SIGNTOOL) sign -type BLw -prikey $(CONFIG_INTEL_X86_PRIVATE_KEY) -wrapkey $(CONFIG_INTEL_X86_PROD_UNIQUE_KEY) -encattr -kdk -sm -secure \
		-pubkeytype otp -algo aes256 -attribute 0x80000000=0x08000000 -attribute 0x80000002=0x08000000 -attribute 0x80000006=0x0 -attribute rollback=$(CONFIG_INTEL_X86_DTB_ROLLBACKID)\
		-cert $(CONFIG_INTEL_X86_CERTIFICATION) -infile $(3) -outfile $@.dtb.signed; \
		tries=`expr $$tries + 1`; \
		echo "trail value is $$tries"; \
    done; \
	if [ ! -f "$@.dtb.signed" ]; then \
		echo "File not found after 10 tries. please retry the build..!"; \
	fi; \

	# wrap device tree blob to u-boot fit image
	dd if=$@.dtb.signed of=$@.dtb bs=$(1) conv=sync;

	[ -f "$@.dtb.signed" ] && dd if=$@.dtb.signed of=$@.dtb bs=$(1) conv=sync || sync && dd if=$@.dtb.signed of=$@.dtb bs=$(1) conv=sync || echo "Failed!" > /dev/null

	mkimage -A $(LINUX_KARCH) -O linux -C none -T flat_dt \
		-f auto -n 'Flattened Device Tree' \
		-d $@.dtb $@.dtb.pad

	cat $(IMAGE_KERNEL) $@.rootfs.pad $@.dtb.pad > $@.tmp

	mkimage -A $(LINUX_KARCH) -O linux -T multi -a 0x00 -C none \
		-e 0x00 \
		-n '$(if $(UIMAGE_NAME),$(UIMAGE_NAME),OpenWrt fullimage)' \
		-d $@.tmp $@

	rm -rf $@.rootfs
	rm -rf $@.rootfs.pad
	rm -rf $@.dtb
	rm -rf $@.dtb.pad
endef
endif
else
define Build/sign-image
endef

define Build/sign-rootfs
	echo "" > /dev/null
endef

define Build/fullimage
	dd if=$(2) of=$@.rootfs bs=$(1) conv=sync;
	mkimage -A $(LINUX_KARCH) -O linux -C lzma -T filesystem -a 0x00  \
		-e 0x00 -n 'LEDE RootFS' \
		-d $@.rootfs $@.rootfs.pad

	# wrap device tree blob to u-boot fit image
	dd if=$(3) of=$@.dtb bs=$(1) conv=sync;
	mkimage -A $(LINUX_KARCH) -O linux -C none -T flat_dt \
		-f auto -n 'Flattened Device Tree' \
		-d $@.dtb $@.dtb.pad

	cat $(IMAGE_KERNEL) $@.rootfs.pad $@.dtb.pad > $@.tmp

	mkimage -A $(LINUX_KARCH) -O linux -T multi -a 0x00 -C none \
		-e 0x00 \
		-n '$(if $(UIMAGE_NAME),$(UIMAGE_NAME),OpenWrt fullimage)' \
		-d $@.tmp $@

	rm -rf $@.rootfs
	rm -rf $@.rootfs.pad
	rm -rf $@.dtb
	rm -rf $@.dtb.pad
endef
endif

ifeq ($(CONFIG_INTEL_X86_IMAGE_FORMAT_FIT),y)
# generates kernel-dtb in fit format.
define Build/fitimage
  mkdir -p $(BIN_DIR)/fitimages
  gzip -f -9n -c $(KDIR)/vmlinux > $(KDIR)/vmlinux.gz
  DTB_BASE_FILE=$(basename $(notdir $(2))); \
  sed -e 's@KERNEL@$(KDIR)/vmlinux.gz@g' \
	-e 's@DTB@$(2)@g' kernel-dtb-fit.its > $(KDIR)/tmp/$$DTB_BASE_FILE.its; \
  PATH=$(LINUX_DIR)/scripts/dtc:$(PATH) mkimage -f $(KDIR)/tmp/$$DTB_BASE_FILE.its $(KDIR)/tmp/$$DTB_BASE_FILE.fit; \
  cp -vf $(KDIR)/tmp/$$DTB_BASE_FILE.fit $(BIN_DIR)/fitimages/$$DTB_BASE_FILE.fit
endef
# generates rootfs in fit format.
define Build/fit-rootfs
  mkdir -p $(BIN_DIR)/fitimages
  sed -e 's@ROOTFS@$@@g' rootfs-fit.its > $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-rootfs.its
  PATH=$(LINUX_DIR)/scripts/dtc:$(PATH) mkimage -f $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-rootfs.its $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-rootfs.fit
  cp -vf $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-rootfs.fit $(BIN_DIR)/fitimages/$(DEVICE_IMG_PREFIX)-rootfs.fit
endef
else
define Build/fitimage
endef
define Build/fit-rootfs
	echo "" > /dev/null
endef
endif

# generates dtb file with basedtb (eth dtb) and overlay PON dtbo.
define Build/overlay-image
  mkdir -p $(BIN_DIR)/overlay-images
  BASE_DTB_FILE=$(basename $(notdir $(2))); \
  sed -e 's@DTBO_FILE@$(1)@g' -e 's@DTB_FILE@$(2)@g' overlay_pon.its > $(KDIR)/tmp/$$BASE_DTB_FILE-overlay_pon.its; \
  PATH=$(LINUX_DIR)/scripts/dtc:$(PATH) mkimage -f $(KDIR)/tmp/$$BASE_DTB_FILE-overlay_pon.its $(KDIR)/tmp/$$BASE_DTB_FILE-ov_pon.itb; \
  mv -v $(KDIR)/tmp/$$BASE_DTB_FILE-ov_pon.itb $(KDIR)/tmp/$$BASE_DTB_FILE-ov_pon.dtb; \
  cp -vf $(KDIR)/tmp/$$BASE_DTB_FILE-ov_pon.dtb $(BIN_DIR)/overlay-images/$$BASE_DTB_FILE-ov_pon.dtb
endef

define Build/generate-ext4fs
	[ ! -f "$(BIN_DIR)/ext4.fs" ] && dd if=/dev/zero of="$(BIN_DIR)/ext4.fs" bs=1M count=112 && mkfs.ext4 -v -b 4096 -O ^metadata_csum,^64bit "$(BIN_DIR)/ext4.fs" || echo "" > /dev/null
endef

# Customized build script for kernel/dtb.
# This is taken from default Device/Build/image, with the removal
# of rootfs since it should not be linked with kernel/dtb.
ifeq ($(CONFIG_INTEL_X86_EXTERNAL_IMAGE_SIGNING),y)
define Device/Build/image-non-rootfs
  $$(_TARGET): $(BIN_DIR)/$(DEVICE_IMG_PREFIX)-$(1)-image-non-rootfs
  $(eval $(call Device/Export,$(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(1)-image-non-rootfs,$(1)))

  $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(1)-image-non-rootfs: $$(KDIR_KERNEL_IMAGE)
	@rm -rf $$@
	[ -f $$(word 1,$$^) ]
	$$(call concat_cmd,$(IMAGE/$(1)))

  .IGNORE: $(BIN_DIR)/$(DEVICE_IMG_PREFIX)-$(1)-image-non-rootfs

  $(BIN_DIR)/$(DEVICE_IMG_PREFIX)-$(1)-image-non-rootfs: $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(1)-image-non-rootfs
	cp -vf $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(1)-image-non-rootfs $(BIN_DIR)/$(DEVICE_IMG_PREFIX)-$(1)
	cp -vf $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(1)-image-non-rootfs $(BIN_DIR)/non_signed_image/$(DEVICE_IMG_PREFIX)-$(1)
	rm -rf $(BIN_DIR)/non_signed_image/$(DEVICE_IMG_PREFIX)-kernel.bin
endef
else
define Device/Build/image-non-rootfs
  $$(_TARGET): $(BIN_DIR)/$(DEVICE_IMG_PREFIX)-$(1)-image-non-rootfs
  $(eval $(call Device/Export,$(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(1)-image-non-rootfs,$(1)))

  $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(1)-image-non-rootfs: $$(KDIR_KERNEL_IMAGE)
	@rm -rf $$@
	[ -f $$(word 1,$$^) ]
	$$(call concat_cmd,$(IMAGE/$(1)))

  .IGNORE: $(BIN_DIR)/$(DEVICE_IMG_PREFIX)-$(1)-image-non-rootfs

  $(BIN_DIR)/$(DEVICE_IMG_PREFIX)-$(1)-image-non-rootfs: $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(1)-image-non-rootfs
	cp -vf $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(1)-image-non-rootfs $(BIN_DIR)/$(DEVICE_IMG_PREFIX)-$(1)
	[  "$(suffix $(1))" = ".dtbo" ] && mv -v $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(1)-image-non-rootfs $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(1)

endef
endif

define Device/Build/fullimage
  $$(_TARGET): $(BIN_DIR)/$(DEVICE_IMG_PREFIX)-$(1)-fullimage
  $$(_TARGET): $(if $(findstring pon,$(basename $(1))),,$(BIN_DIR)/$(DEVICE_IMG_PREFIX)-$(basename $(1))_ov.img-fullimage)
  $(eval $(call Device/Export,$(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(1)-fullimage,$(1)))
  $(if $(findstring pon,$(basename $(1))),,$(eval $(call Device/Export,$(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(basename $(1))_ov.img-fullimage,$(basename $(1))_ov.img)))

  $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(word 4,$(IMAGE/$(1))):
	@rm -rf $$@
	[ -f $$(word 1,$$^) ]
	$$(call concat_cmd,$(IMAGE/$(word 4,$(IMAGE/$(1)))))

  $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(basename $(word 4,$(IMAGE/$(1))))-ov_pon.dtb : $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-ov_pon.dtbo-image-non-rootfs $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(word 4,$(IMAGE/$(1)))
	$(if $(findstring pon,$(basename $(1))),,$$(call Build/overlay-image,$(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-ov_pon.dtbo,$(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(word 4,$(IMAGE/$(1)))))

  $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(basename $(1))_ov.img-fullimage: $$(KDIR_KERNEL_IMAGE) $(BIN_DIR)/$(DEVICE_IMG_PREFIX)-squashfs-$$(ROOTFS) $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(basename $(word 4,$(IMAGE/$(1))))-ov_pon.dtb
	@rm -rf $$@
	[ -f $$(word 1,$$^) ]
	$(if $(CONFIG_INTEL_X86_IMAGE_FORMAT_MKIMAGE),$(if $(findstring pon,$(basename $(1))),,$$(call Build/fullimage,$(word 2,$(IMAGE/$(1))),$(BIN_DIR)/$(DEVICE_IMG_PREFIX)-squashfs-$$(ROOTFS),$(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(basename $(word 4,$(IMAGE/$(1))))-ov_pon.dtb)))

  $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(1)-fullimage: $$(KDIR_KERNEL_IMAGE) $(BIN_DIR)/$(DEVICE_IMG_PREFIX)-squashfs-$$(ROOTFS) $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(word 4,$(IMAGE/$(1)))
	@rm -rf $$@
	[ -f $$(word 1,$$^) ]
	$(if $(CONFIG_INTEL_X86_IMAGE_FORMAT_MKIMAGE),$$(call Build/fullimage,$(word 2,$(IMAGE/$(1))),$(BIN_DIR)/$(DEVICE_IMG_PREFIX)-squashfs-$$(ROOTFS),$(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(word 4,$(IMAGE/$(1)))))
	$(if $(CONFIG_INTEL_X86_IMAGE_FORMAT_FIT),$$(call Build/fitimage,$(BIN_DIR)/$(DEVICE_IMG_PREFIX)-squashfs-$$(ROOTFS),$(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(word 4,$(IMAGE/$(1)))))

  .IGNORE: $(BIN_DIR)/$(DEVICE_IMG_PREFIX)-$(1)-fullimage

  .IGNORE: $(BIN_DIR)/$(DEVICE_IMG_PREFIX)-$(basename $(1))_ov.img-fullimage

  $(BIN_DIR)/$(DEVICE_IMG_PREFIX)-$(basename $(1))_ov.img-fullimage: $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(basename $(1))_ov.img-fullimage
	cp -vf $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(basename $(1))_ov.img-fullimage $(BIN_DIR)/overlay-images/$(DEVICE_IMG_PREFIX)-$(basename $(1))_ov.img

  $(BIN_DIR)/$(DEVICE_IMG_PREFIX)-$(1)-fullimage: $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(1)-fullimage
	cp -vf $(KDIR)/tmp/$(DEVICE_IMG_PREFIX)-$(1)-fullimage $(BIN_DIR)/$(DEVICE_IMG_PREFIX)-$(1)

endef

# Default openwrt image script builds different images (kernel/dtb) per
# rootfs. This is not ideal for our usecase, which should only need
# different image for the rootfs itself (kernel/dtb should be common).
# We use customized build script for this reason.
define Device/Build
  $(if $(CONFIG_TARGET_ROOTFS_INITRAMFS),$(call Device/Build/initramfs,$(1)))
  $(call Device/Build/kernel,$(1))

  $$(eval $$(foreach compile,$$(COMPILE), \
    $$(call Device/Build/compile,$$(compile),$(1))))

  $$(eval $$(foreach fs,$$(filter $(TARGET_FILESYSTEMS),$$(FILESYSTEMS)), \
    $$(call Device/Build/image,$$(fs),$$(ROOTFS),$(1))))

  $$(eval $$(foreach image,$$(IMAGES), \
    $$(call Device/Build/image-non-rootfs,$$(image),$(1))))

  $$(eval $$(foreach image,$$(FULLIMAGES), \
    $$(call Device/Build/fullimage,$$(image),$(1))))

endef

define Device/LGM_GENERIC
  KERNEL_LOADADDR := 0x2000000
  KERNEL_ENTRY := 0x2000000
  KERNEL := kernel-bin | gzip | uImage-x86_64 gzip | pad-offset 16 0
  KERNEL_INITRAMFS := kernel-bin | gzip | uImage-x86_64 gzip
  DEVICE_DTS_DIR := ../dts
  IMAGE/kernel.bin := append-kernel
  IMAGE/fs.rootfs := append-rootfs  | sign-rootfs | fit-rootfs | generate-ext4fs
  UIMAGE_NAME:=$(if $(UIMAGE_NAME),LGM-$(UIMAGE_NAME))
endef

define Device/CBSP_B0
  $(Device/LGM_GENERIC)
  DEVICE_TITLE := LGM CBSP B-Step Model
  IMAGE/lgp_b0.dtb := dtb lgp_b0
  IMAGE/simics_datapath.dtb := dtb lgm_simics_datapath
  IMAGE/octopus_851.dtb := dtb octopus_851
  IMAGE/octopus_641.dtb := dtb octopus_641
  IMAGE/octopus_851_wan_phy.dtb := dtb octopus_851_wan_phy
  IMAGE/octopus_851_fixedlink.dtb := dtb octopus_851_fixedlink
  IMAGE/lgp_b0_fullimage.img := fullimage 16 squashfs lgp_b0.dtb
  IMAGE/simics_b0_fullimage.img := fullimage 16 squashfs simics_datapath.dtb
  IMAGE/octopus_851_fullimage.img := fullimage 16 squashfs octopus_851.dtb
  IMAGE/octopus_641_fullimage.img := fullimage 16 squashfs octopus_641.dtb
  IMAGE/octopus_851_wan_phy_fullimage.img := fullimage 16 squashfs octopus_851_wan_phy.dtb
  IMAGES += kernel.bin \
	simics_datapath.dtb \
	lgp_b0.dtb octopus_851.dtb octopus_641.dtb octopus_851_wan_phy.dtb octopus_851_fixedlink.dtb
  FULLIMAGES := \
	simics_b0_fullimage.img \
	lgp_b0_fullimage.img octopus_851_fullimage.img octopus_641_fullimage.img \
	octopus_851_wan_phy_fullimage.img
  ROOTFS := fs.rootfs
endef
TARGET_DEVICES += CBSP_B0

define Device/EVM_CBSP_B0
  $(Device/LGM_GENERIC)
  DEVICE_TITLE := EVM CBSP B 0 device
  IMAGE/lgm_evm_b0.dtb := dtb lgm_evm_b0
  IMAGES += kernel.bin lgm_evm_b0.dtb
  ROOTFS := fs.rootfs
  DEVICE_PACKAGES :=
endef
TARGET_DEVICES += EVM_CBSP_B0

define Device/CBSP_MINIFS_B0
  $(Device/CBSP_B0)
  DEVICE_TITLE := LGM CBSP minifs B-step model
  DEVICE_PACKAGES :=
endef
TARGET_DEVICES += CBSP_MINIFS_B0

define Device/CBSP_C0
  $(Device/LGM_GENERIC)
  DEVICE_TITLE := LGM CBSP C-Step Model
  IMAGE/lgp_b0.dtb := dtb lgp_b0
  IMAGE/lgm_haps.dtb := dtb lgm_haps
  IMAGE/lgm_haps_datapath.dtb := dtb lgm_haps_datapath
  IMAGE/lgm_haps_pcie.dtb := dtb lgm_haps_pcie
  IMAGE/lgm_haps_emmc.dtb := dtb lgm_haps_emmc
  IMAGE/lgmc_haps_fullimage.img := fullimage 16 squashfs lgm_haps_datapath.dtb
  IMAGES += kernel.bin \
	simics_datapath.dtb \
	lgp_b0.dtb \
	lgm_haps.dtb \
	lgm_haps_datapath.dtb \
	lgm_haps_pcie.dtb \
	lgm_haps_emmc.dtb
  FULLIMAGES := lgmc_haps_fullimage.img
  ROOTFS := fs.rootfs
endef
TARGET_DEVICES += CBSP_C0

define Device/CBSP_HAPS_C0
  $(Device/CBSP_C0)
  DEVICE_TITLE := LGM CBSP HAPS C-step model
  DEVICE_PACKAGES := $(CBSP_WAV700_UGW_PACKAGES_UCI) $(CBSP_WAV700_PACKAGES_UCI) $(UGW_PACKAGE_DCDP) $(PCIUTILS_PACKAGE)
endef
TARGET_DEVICES += CBSP_HAPS_C0

define Device/CBSP_MINIFS_WAVE_B0
  $(Device/CBSP_B0)
  DEVICE_TITLE := LGM CBSP minifs B-step model with WAVE
  DEVICE_PACKAGES := $(CBSP_WAV600_UGW_PACKAGES_UCI) $(WAV600_PACKAGES_UCI) $(UGW_PACKAGE_DCDP)
endef
TARGET_DEVICES += CBSP_MINIFS_WAVE_B0

define Device/LGM_UGW
  $(Device/LGM_GENERIC)
  DEVICE_TITLE := LGM UGW Model
  IMAGE/lgp_b0.dtb := dtb lgp_b0
  IMAGE/lgp_b0_fixedlink.dtb := dtb lgp_b0_fixedlink
  IMAGE/lgm_evm_b0.dtb := dtb lgm_evm_b0
  IMAGE/octopus_851.dtb := dtb octopus_851
  IMAGE/octopus_851_nand.dtb := dtb octopus_851_nand
  IMAGE/octopus_641.dtb := dtb octopus_641
  IMAGE/octopus_641_pon.dtb := dtb octopus_641_pon
  IMAGE/octopus_641_10g_lan_pon.dtb := dtb octopus_641_10g_lan_pon
  IMAGE/octopus_641_10g_lan_eth.dtb := dtb octopus_641_10g_lan_eth
  IMAGE/octopus_641_10g_lan_eth_nand.dtb := dtb octopus_641_10g_lan_eth_nand
  IMAGE/octopus_641_wav700_eth.dtb := dtb octopus_641_wav700_eth
  IMAGE/octopus_641_wav700_pon.dtb := dtb octopus_641_wav700_pon
  IMAGE/octopus_641_pm.dtb := dtb octopus_641_pm
  IMAGE/octopus_641_aic_10g_eth.dtb := dtb octopus_641_aic_10g_eth
  IMAGE/octopus_641_aic_gsw140.dtb := dtb octopus_641_aic_gsw140
  IMAGE/octopus_641_aic_moca.dtb := dtb octopus_641_aic_moca
  IMAGE/octopus_641_docsis.dtb := dtb octopus_641_docsis
  IMAGE/octopus_851_wan_phy.dtb := dtb octopus_851_wan_phy
  IMAGE/octopus_851_pon.dtb := dtb octopus_851_pon
  IMAGE/octopus_851_docsis.dtb := dtb octopus_851_docsis
  IMAGE/octopus_851_fixedlink.dtb := dtb octopus_851_fixedlink
  IMAGE/octopus_851_wav700_eth.dtb := dtb octopus_851_wav700_eth
  IMAGE/octopus_851_wav700_pon.dtb := dtb octopus_851_wav700_pon
  IMAGE/octopus_851_wav700_eth_pm.dtb := dtb octopus_851_wav700_eth_pm
  IMAGE/octopus_851_wav700_pon_pm.dtb := dtb octopus_851_wav700_pon_pm
  IMAGE/octopus_851_wav700_docsis.dtb := dtb octopus_851_wav700_docsis
  IMAGE/octopus_851_pm.dtb := dtb octopus_851_pm
  IMAGE/lgp_b0_pon.dtb := dtb lgp_b0_pon
  IMAGE/lgp_b0_docsis.dtb := dtb lgp_b0_docsis
  IMAGE/lgp_b0_wav700_docsis.dtb := dtb lgp_b0_wav700_docsis
  IMAGE/lgp_b0_fullimage.img := fullimage 16 squashfs lgp_b0.dtb
  IMAGE/lgm_c0_1GB_DDR_mxl86249.dtb := dtb octopus_640_1GB_DDR_mxl86249
  IMAGE/lgm_c0_1GB_DDR_10g_lan.dtb := dtb octopus_640_1GB_DDR_10g_lan
  IMAGE/octopus_851_fullimage.img := fullimage 16 squashfs octopus_851.dtb
  IMAGE/octopus_641_fullimage.img := fullimage 16 squashfs octopus_641.dtb
  IMAGE/octopus_641_pon_fullimage.img := fullimage 16 squashfs octopus_641_pon.dtb
  IMAGE/octopus_641_10g_lan_pon_fullimage.img := fullimage 16 squashfs octopus_641_10g_lan_pon.dtb
  IMAGE/octopus_641_wav700_eth_fullimage.img := fullimage 16 squashfs octopus_641_wav700_eth.dtb
  IMAGE/octopus_641_wav700_pon_fullimage.img := fullimage 16 squashfs octopus_641_wav700_pon.dtb
  IMAGE/octopus_641_pm_fullimage.img := fullimage 16 squashfs octopus_641_pm.dtb
  IMAGE/octopus_851_wan_phy_fullimage.img := fullimage 16 squashfs octopus_851_wan_phy.dtb
  IMAGE/octopus_851_pon_fullimage.img := fullimage 16 squashfs octopus_851_pon.dtb
  IMAGE/octopus_851_docsis_fullimage.img := fullimage 16 squashfs octopus_851_docsis.dtb
  IMAGE/octopus_851_wav700_eth_fullimage.img := fullimage 16 squashfs octopus_851_wav700_eth.dtb
  IMAGE/octopus_851_wav700_pon_fullimage.img := fullimage 16 squashfs octopus_851_wav700_pon.dtb
  IMAGE/octopus_851_wav700_eth_pm_fullimage.img := fullimage 16 squashfs octopus_851_wav700_eth_pm.dtb
  IMAGE/octopus_851_wav700_pon_pm_fullimage.img := fullimage 16 squashfs octopus_851_wav700_pon_pm.dtb
  IMAGE/octopus_851_wav700_docsis_fullimage.img := fullimage 16 squashfs octopus_851_wav700_docsis.dtb
  IMAGE/octopus_851_pm_fullimage.img := fullimage 16 squashfs octopus_851_pm.dtb
  IMAGE/lgp_b0_pon_fullimage.img := fullimage 16 squashfs lgp_b0_pon.dtb
  IMAGE/octopus_641_aic_10g_eth_fullimage.img := fullimage 16 squashfs octopus_641_aic_10g_eth.dtb
  IMAGE/octopus_641_aic_gsw140_fullimage.img := fullimage 16 squashfs octopus_641_aic_gsw140.dtb
  IMAGE/octopus_641_aic_moca_fullimage.img := fullimage 16 squashfs octopus_641_aic_moca.dtb
  IMAGE/lgm_c0_1GB_DDR_mxl86249_fullimage.img := fullimage 16 squashfs lgm_c0_1GB_DDR_mxl86249.dtb
  IMAGE/lgm_c0_1GB_DDR_10g_lan_fullimage.img := fullimage 16 squashfs lgm_c0_1GB_DDR_10g_lan.dtb
  IMAGES += kernel.bin lgp_b0.dtb lgp_b0_pon.dtb lgm_evm_b0.dtb octopus_851.dtb octopus_641.dtb octopus_641_pon.dtb octopus_641_10g_lan_pon.dtb octopus_641_10g_lan_eth.dtb octopus_641_wav700_eth.dtb octopus_641_wav700_pon.dtb octopus_851_wan_phy.dtb octopus_851_fixedlink.dtb octopus_851_wav700_eth.dtb octopus_851_wav700_pon.dtb lgp_b0_fixedlink.dtb \
		lgp_b0_docsis.dtb \
		lgp_b0_wav700_docsis.dtb \
		octopus_851_docsis.dtb \
		octopus_851_wav700_docsis.dtb \
		octopus_851_pm.dtb \
		octopus_641_aic_10g_eth.dtb \
		octopus_641_aic_gsw140.dtb \
		octopus_641_aic_moca.dtb \
		octopus_641_pm.dtb \
		octopus_641_docsis.dtb \
		octopus_851_wav700_eth_pm.dtb \
		octopus_851_wav700_pon_pm.dtb \
		octopus_851_pon.dtb \
		octopus_641_10g_lan_eth_nand.dtb \
		octopus_851_nand.dtb \
		lgm_c0_1GB_DDR_mxl86249.dtb \
		lgm_c0_1GB_DDR_10g_lan.dtb

  FULLIMAGES := lgp_b0_fullimage.img lgp_b0_pon_fullimage.img octopus_851_fullimage.img octopus_641_fullimage.img octopus_641_pon_fullimage.img \
		octopus_851_wan_phy_fullimage.img octopus_851_pon_fullimage.img octopus_851_wav700_eth_fullimage.img octopus_851_wav700_pon_fullimage.img octopus_641_10g_lan_pon_fullimage.img octopus_641_wav700_eth_fullimage.img octopus_641_wav700_pon_fullimage.img \
		octopus_851_docsis_fullimage.img \
		octopus_851_wav700_docsis_fullimage.img \
		octopus_851_pm_fullimage.img \
		octopus_641_aic_10g_eth_fullimage.img \
		octopus_641_aic_gsw140_fullimage.img \
		octopus_641_pm_fullimage.img \
		octopus_641_aic_moca_fullimage.img \
		octopus_851_wav700_eth_pm_fullimage.img \
		octopus_851_wav700_pon_pm_fullimage.img \
		lgm_c0_1GB_DDR_mxl86249_fullimage.img \
		lgm_c0_1GB_DDR_10g_lan_fullimage.img
  ROOTFS := fs.rootfs
  ROOTFS_PREPARE := add-servicelayer-schema
  DEVICE_PACKAGES := $(UGW_PACKAGES) \
		     $(DSL_CPE_GFAST_PACKAGES_PRX) $(DSL_CPE_GFAST_PACKAGES_RELEASE) \
		     $(PM_PACKAGES)\
		     $(WAV600_UGW_PACKAGES_UCI) $(WAV600_PACKAGES_UCI) \
		     $(UGW_DIAG_DSL_PACKAGES)
endef
TARGET_DEVICES += LGM_UGW

define Device/LGM_UGW_WLANOSP
  $(Device/LGM_GENERIC)
  DEVICE_TITLE := LGM UGW Model
  IMAGE/lgp_b0.dtb := dtb lgp_b0
  IMAGE/lgp_b0_fixedlink.dtb := dtb lgp_b0_fixedlink
  IMAGE/lgm_evm_b0.dtb := dtb lgm_evm_b0
  IMAGE/octopus_851.dtb := dtb octopus_851
  IMAGE/octopus_851_nand.dtb := dtb octopus_851_nand
  IMAGE/octopus_641.dtb := dtb octopus_641
  IMAGE/octopus_641_pon.dtb := dtb octopus_641_pon
  IMAGE/octopus_641_10g_lan_pon.dtb := dtb octopus_641_10g_lan_pon
  IMAGE/octopus_641_10g_lan_eth.dtb := dtb octopus_641_10g_lan_eth
  IMAGE/octopus_641_10g_lan_eth_nand.dtb := dtb octopus_641_10g_lan_eth_nand
  IMAGE/octopus_641_wav700_eth.dtb := dtb octopus_641_wav700_eth
  IMAGE/octopus_641_wav700_pon.dtb := dtb octopus_641_wav700_pon
  IMAGE/octopus_641_pm.dtb := dtb octopus_641_pm
  IMAGE/octopus_641_aic_10g_eth.dtb := dtb octopus_641_aic_10g_eth
  IMAGE/octopus_641_aic_gsw140.dtb := dtb octopus_641_aic_gsw140
  IMAGE/octopus_641_aic_moca.dtb := dtb octopus_641_aic_moca
  IMAGE/octopus_641_docsis.dtb := dtb octopus_641_docsis
  IMAGE/octopus_851_wan_phy.dtb := dtb octopus_851_wan_phy
  IMAGE/octopus_851_pon.dtb := dtb octopus_851_pon
  IMAGE/octopus_851_docsis.dtb := dtb octopus_851_docsis
  IMAGE/octopus_851_fixedlink.dtb := dtb octopus_851_fixedlink
  IMAGE/octopus_851_wav700_eth.dtb := dtb octopus_851_wav700_eth
  IMAGE/octopus_851_wav700_pon.dtb := dtb octopus_851_wav700_pon
  IMAGE/octopus_851_wav700_eth_pm.dtb := dtb octopus_851_wav700_eth_pm
  IMAGE/octopus_851_wav700_pon_pm.dtb := dtb octopus_851_wav700_pon_pm
  IMAGE/octopus_851_wav700_docsis.dtb := dtb octopus_851_wav700_docsis
  IMAGE/octopus_851_pm.dtb := dtb octopus_851_pm
  IMAGE/lgp_b0_pon.dtb := dtb lgp_b0_pon
  IMAGE/lgp_b0_docsis.dtb := dtb lgp_b0_docsis
  IMAGE/lgp_b0_wav700_docsis.dtb := dtb lgp_b0_wav700_docsis
  IMAGE/lgp_b0_fullimage.img := fullimage 16 squashfs lgp_b0.dtb
  IMAGE/octopus_851_fullimage.img := fullimage 16 squashfs octopus_851.dtb
  IMAGE/octopus_641_fullimage.img := fullimage 16 squashfs octopus_641.dtb
  IMAGE/octopus_641_pon_fullimage.img := fullimage 16 squashfs octopus_641_pon.dtb
  IMAGE/octopus_641_10g_lan_pon_fullimage.img := fullimage 16 squashfs octopus_641_10g_lan_pon.dtb
  IMAGE/octopus_641_wav700_eth_fullimage.img := fullimage 16 squashfs octopus_641_wav700_eth.dtb
  IMAGE/octopus_641_wav700_pon_fullimage.img := fullimage 16 squashfs octopus_641_wav700_pon.dtb
  IMAGE/octopus_641_pm_fullimage.img := fullimage 16 squashfs octopus_641_pm.dtb
  IMAGE/octopus_851_wan_phy_fullimage.img := fullimage 16 squashfs octopus_851_wan_phy.dtb
  IMAGE/octopus_851_pon_fullimage.img := fullimage 16 squashfs octopus_851_pon.dtb
  IMAGE/octopus_851_docsis_fullimage.img := fullimage 16 squashfs octopus_851_docsis.dtb
  IMAGE/octopus_851_wav700_eth_fullimage.img := fullimage 16 squashfs octopus_851_wav700_eth.dtb
  IMAGE/octopus_851_wav700_pon_fullimage.img := fullimage 16 squashfs octopus_851_wav700_pon.dtb
  IMAGE/octopus_851_wav700_eth_pm_fullimage.img := fullimage 16 squashfs octopus_851_wav700_eth_pm.dtb
  IMAGE/octopus_851_wav700_pon_pm_fullimage.img := fullimage 16 squashfs octopus_851_wav700_pon_pm.dtb
  IMAGE/octopus_851_wav700_docsis_fullimage.img := fullimage 16 squashfs octopus_851_wav700_docsis.dtb
  IMAGE/octopus_851_pm_fullimage.img := fullimage 16 squashfs octopus_851_pm.dtb
  IMAGE/lgp_b0_pon_fullimage.img := fullimage 16 squashfs lgp_b0_pon.dtb
  IMAGE/octopus_641_aic_10g_eth_fullimage.img := fullimage 16 squashfs octopus_641_aic_10g_eth.dtb
  IMAGE/octopus_641_aic_gsw140_fullimage.img := fullimage 16 squashfs octopus_641_aic_gsw140.dtb
  IMAGE/octopus_641_aic_moca_fullimage.img := fullimage 16 squashfs octopus_641_aic_moca.dtb
  IMAGES += kernel.bin lgp_b0.dtb lgp_b0_pon.dtb lgm_evm_b0.dtb octopus_851.dtb octopus_641.dtb octopus_641_pon.dtb octopus_641_10g_lan_pon.dtb octopus_641_10g_lan_eth.dtb octopus_641_wav700_eth.dtb octopus_641_wav700_pon.dtb octopus_851_wan_phy.dtb octopus_851_fixedlink.dtb octopus_851_wav700_eth.dtb octopus_851_wav700_pon.dtb lgp_b0_fixedlink.dtb \
		lgp_b0_docsis.dtb \
		lgp_b0_wav700_docsis.dtb \
		octopus_851_docsis.dtb \
		octopus_851_wav700_docsis.dtb \
		octopus_851_pm.dtb \
		octopus_641_aic_10g_eth.dtb \
		octopus_641_aic_gsw140.dtb \
		octopus_641_aic_moca.dtb \
		octopus_641_pm.dtb \
		octopus_641_docsis.dtb \
		octopus_851_wav700_eth_pm.dtb \
		octopus_851_wav700_pon_pm.dtb \
		octopus_851_pon.dtb \
		octopus_641_10g_lan_eth_nand.dtb \
		octopus_851_nand.dtb

  FULLIMAGES := lgp_b0_fullimage.img lgp_b0_pon_fullimage.img octopus_851_fullimage.img octopus_641_fullimage.img octopus_641_pon_fullimage.img \
		octopus_851_wan_phy_fullimage.img octopus_851_pon_fullimage.img octopus_851_wav700_eth_fullimage.img octopus_851_wav700_pon_fullimage.img octopus_641_10g_lan_pon_fullimage.img octopus_641_wav700_eth_fullimage.img octopus_641_wav700_pon_fullimage.img \
		octopus_851_docsis_fullimage.img \
		octopus_851_wav700_docsis_fullimage.img \
		octopus_851_pm_fullimage.img \
		octopus_641_aic_10g_eth_fullimage.img \
		octopus_641_aic_gsw140_fullimage.img \
		octopus_641_pm_fullimage.img \
		octopus_641_aic_moca_fullimage.img \
		octopus_851_wav700_eth_pm_fullimage.img \
		octopus_851_wav700_pon_pm_fullimage.img
  ROOTFS := fs.rootfs
  ROOTFS_PREPARE := add-servicelayer-schema
  DEVICE_PACKAGES := $(UGW_PACKAGES) \
		     $(DSL_CPE_GFAST_PACKAGES_PRX) $(DSL_CPE_GFAST_PACKAGES_RELEASE) \
		     $(PM_PACKAGES)\
		     $(WAV700_UGW_PACKAGES_UCI_OSP) $(WAV700_PACKAGES_UCI_OSP) \
		     $(UGW_DIAG_DSL_PACKAGES)
endef
TARGET_DEVICES += LGM_UGW_WLANOSP

define Device/LGM_PRPL
  $(Device/LGM_GENERIC)
  UIMAGE_NAME:=$(if $(UIMAGE_NAME),PRPL-$(UIMAGE_NAME))
  DEVICE_TITLE := LGM Model for prplOS
  IMAGE/lgp_b0.dtb := dtb lgp_b0
  IMAGE/lgp_b0_pon.dtb := dtb lgp_b0_pon
  IMAGE/lgp_b0_fullimage.img := fullimage 16 squashfs lgp_b0.dtb
  IMAGE/lgp_b0_pon_fullimage.img := fullimage 16 squashfs lgp_b0_pon.dtb
  IMAGES += kernel.bin \
        lgp_b0.dtb \
        lgp_b0_pon.dtb
  FULLIMAGES := lgp_b0_fullimage.img lgp_b0_pon_fullimage.img
  ROOTFS := fs.rootfs
  ROOTFS_PREPARE := add-servicelayer-schema
  DEVICE_PACKAGES := $(PM_PACKAGES)\
		     $(UGW_DIAG_PACKAGES)
endef
TARGET_DEVICES += LGM_PRPL

define Device/LGM_C0_HAPS_PRPL
  $(Device/LGM_GENERIC)
  DEVICE_TITLE := LGM C0 HAPS Model for prplOS
  IMAGE/lgp_b0.dtb := dtb lgp_b0
  IMAGE/lgp_b0_fullimage.img := fullimage 16 squashfs lgp_b0.dtb
  IMAGES += kernel.bin \
        lgp_b0.dtb
  FULLIMAGES := lgp_b0_fullimage.img
  ROOTFS := fs.rootfs
  ROOTFS_PREPARE := add-servicelayer-schema
  DEVICE_PACKAGES := $(PM_PACKAGES)\
             $(UGW_DIAG_PACKAGES)
endef
TARGET_DEVICES += LGM_C0_HAPS_PRPL

define Device/PRPL_C0
  $(Device/LGM_GENERIC)
  DEVICE_TITLE := LGM C Model for prplOS
  IMAGE/1GB_DDR_mxl86249.dtb := dtb octopus_640_1GB_DDR_mxl86249
  IMAGE/1GB_DDR_10g_lan.dtb := dtb octopus_640_1GB_DDR_10g_lan
  IMAGE/1GB_DDR_mxl86249_fullimage.img := fullimage 16 squashfs 1GB_DDR_mxl86249.dtb
  IMAGE/1GB_DDR_10g_lan_fullimage.img := fullimage 16 squashfs 1GB_DDR_10g_lan.dtb
  IMAGES += kernel.bin \
	    1GB_DDR_mxl86249.dtb \
	    1GB_DDR_10g_lan.dtb
  FULLIMAGES := 1GB_DDR_mxl86249_fullimage.img 1GB_DDR_10g_lan_fullimage.img
  ROOTFS := fs.rootfs
  ROOTFS_PREPARE := add-servicelayer-schema
  DEVICE_PACKAGES := $(PM_PACKAGES)\
             $(UGW_DIAG_PACKAGES)
endef
TARGET_DEVICES += PRPL_C0

define Device/PRPL_OSP_TB341
  $(Device/LGM_GENERIC)
  DEVICE_TITLE := LGM Model for prplOS osp tb341
  IMAGE/osp_tb341.dtb := dtb osp_tb341
  IMAGE/osp_tb341_pon.dtb := dtb osp_tb341_pon
  IMAGE/osp_tb341_fullimage.img := fullimage 16 squashfs osp_tb341.dtb
  IMAGE/osp_tb341_pon_fullimage.img := fullimage 16 squashfs osp_tb341_pon.dtb
  IMAGES += kernel.bin \
        osp_tb341.dtb \
        osp_tb341_pon.dtb
  FULLIMAGES := osp_tb341_fullimage.img osp_tb341_pon_fullimage.img
  ROOTFS := fs.rootfs
  ROOTFS_PREPARE := add-servicelayer-schema
  DEVICE_PACKAGES := $(PM_PACKAGES)\
		     $(UGW_DIAG_PACKAGES)
endef
TARGET_DEVICES += PRPL_OSP_TB341

define Device/PRPL_OSP_TB341_v2
  $(Device/LGM_GENERIC)
  DEVICE_TITLE := LGM Model for prplOS osp tb341 v2
  IMAGE/ov_pon.dtbo := dtbo overlay_pon
  IMAGE/osp_tb341_v2_wav700_eth.dtb := dtb osp_tb341_v2_wav700_eth
  IMAGE/osp_tb341_v2_wav700_pon.dtb := dtb osp_tb341_v2_wav700_pon
  IMAGE/osp_tb341_v2_wav700_eth_fullimage.img := fullimage 16 squashfs osp_tb341_v2_wav700_eth.dtb
  IMAGE/osp_tb341_v2_wav700_pon_fullimage.img := fullimage 16 squashfs osp_tb341_v2_wav700_pon.dtb
  IMAGES += kernel.bin \
		ov_pon.dtbo \
		osp_tb341_v2_wav700_eth.dtb \
		osp_tb341_v2_wav700_pon.dtb
  FULLIMAGES := osp_tb341_v2_wav700_eth_fullimage.img \
		osp_tb341_v2_wav700_pon_fullimage.img
  ROOTFS := fs.rootfs
  ROOTFS_PREPARE := add-servicelayer-schema
  DEVICE_PACKAGES := $(PM_PACKAGES)\
                     $(UGW_DIAG_PACKAGES)
endef
TARGET_DEVICES += PRPL_OSP_TB341_v2

define Device/PRPL_OSPv2_WGRTD159BE_B
  $(Device/LGM_GENERIC)
  DEVICE_TITLE := LGM Model for prplOS osp tb341 v2
  IMAGE/wav700_eth.dtb := dtb osp_tb341_v2_wav700_eth
  IMAGE/wav700_pon.dtb := dtb osp_tb341_v2_wav700_pon
  IMAGE/wav700_eth_fullimage.img := fullimage 16 squashfs wav700_eth.dtb
  IMAGE/wav700_pon_fullimage.img := fullimage 16 squashfs wav700_pon.dtb
  IMAGES += kernel.bin \
		wav700_eth.dtb \
		wav700_pon.dtb
  FULLIMAGES := wav700_eth_fullimage.img \
		wav700_pon_fullimage.img
  ROOTFS := fs.rootfs
  ROOTFS_PREPARE := add-servicelayer-schema
  DEVICE_PACKAGES := $(PM_PACKAGES)\
                     $(UGW_DIAG_PACKAGES)
endef
TARGET_DEVICES += PRPL_OSPv2_WGRTD159BE_B

define Device/PRPL_MB_URX
  $(Device/LGM_GENERIC)
  DEVICE_TITLE := LGM CBSP B-Step Model for prplos
  IMAGE/ov_pon.dtbo := dtbo overlay_pon
  IMAGE/851.dtb := dtb octopus_851
  IMAGE/851_pon.dtb := dtb octopus_851_pon
  IMAGE/641.dtb := dtb octopus_641
  IMAGE/641_pon.dtb := dtb octopus_641_pon
  IMAGE/641_10g_lan_pon.dtb := dtb octopus_641_10g_lan_pon
  IMAGE/641_10g_lan_eth.dtb := dtb octopus_641_10g_lan_eth
  IMAGE/641_wav700_eth.dtb := dtb octopus_641_wav700_eth
  IMAGE/641_wav700_pon.dtb := dtb octopus_641_wav700_pon
  IMAGE/octopus_641_aic_10g_eth.dtb := dtb octopus_641_aic_10g_eth
  IMAGE/octopus_641_aic_gsw140.dtb := dtb octopus_641_aic_gsw140
  IMAGE/851_wav700_eth.dtb := dtb octopus_851_wav700_eth
  IMAGE/851_wav700_eth_pm.dtb := dtb octopus_851_wav700_eth_pm
  IMAGE/851_wav700_pon.dtb := dtb octopus_851_wav700_pon
  IMAGE/851_wav700_pon_pm.dtb := dtb octopus_851_wav700_pon_pm
  IMAGE/851_fullimage.img := fullimage 16 squashfs 851.dtb
  IMAGE/851_pon_fullimage.img := fullimage 16 squashfs 851_pon.dtb
  IMAGE/641_fullimage.img := fullimage 16 squashfs 641.dtb
  IMAGE/641_pon_fullimage.img := fullimage 16 squashfs 641_pon.dtb
  IMAGE/641_10g_lan_pon_fullimage.img := fullimage 16 squashfs 641_10g_lan_pon.dtb
  IMAGE/641_10g_lan_eth_fullimage.img := fullimage 16 squashfs 641_10g_lan_eth.dtb
  IMAGE/641_wav700_eth_fullimage.img := fullimage 16 squashfs 641_wav700_eth.dtb
  IMAGE/641_wav700_pon_fullimage.img := fullimage 16 squashfs 641_wav700_pon.dtb
  IMAGE/octopus_641_aic_10g_eth_fullimage.img := fullimage 16 squashfs octopus_641_aic_10g_eth.dtb
  IMAGE/octopus_641_aic_gsw140_fullimage.img := fullimage 16 squashfs octopus_641_aic_gsw140.dtb
  IMAGE/851_wav700_eth_fullimage.img := fullimage 16 squashfs 851_wav700_eth.dtb
  IMAGE/851_wav700_eth_fullimage_pm.img := fullimage 16 squashfs 851_wav700_eth_pm.dtb
  IMAGE/851_wav700_pon_fullimage.img := fullimage 16 squashfs 851_wav700_pon.dtb
  IMAGE/851_wav700_pon_fullimage_pm.img := fullimage 16 squashfs 851_wav700_pon_pm.dtb
  IMAGES += kernel.bin \
		ov_pon.dtbo \
		851.dtb 851_pon.dtb 641.dtb 641_pon.dtb octopus_641_aic_gsw140.dtb octopus_641_aic_10g_eth.dtb \
		851_wav700_eth_pm.dtb \
		851_wav700_pon_pm.dtb \
		641_wav700_eth.dtb \
		641_wav700_pon.dtb
  FULLIMAGES := 851_fullimage.img 641_fullimage.img \
		851_pon_fullimage.img 641_pon_fullimage.img 641_10g_lan_pon_fullimage.img 641_10g_lan_eth_fullimage.img \
		641_wav700_eth_fullimage.img 641_wav700_pon_fullimage.img \
		851_wav700_eth_fullimage.img 851_wav700_pon_fullimage.img \
		851_wav700_eth_fullimage_pm.img 851_wav700_pon_fullimage_pm.img \
		octopus_641_aic_gsw140_fullimage.img \
		octopus_641_aic_10g_eth_fullimage.img
  ROOTFS := fs.rootfs
  ROOTFS_PREPARE := add-servicelayer-schema
  DEVICE_PACKAGES := $(PM_PACKAGES)\
		     $(UGW_DIAG_PACKAGES)
endef
TARGET_DEVICES += PRPL_MB_URX

define Device/LGM_UGW_SDL
  $(Device/LGM_GENERIC)
  DEVICE_TITLE := LGM UGW SDL Model
  FAKED_ENV := $(FAKEROOT_PROG)
  IMAGE/lgp_b0.dtb := dtb lgp_b0
  IMAGE/lgp_b0_pon.dtb := dtb lgp_b0_pon
  IMAGE/octopus_851.dtb := dtb octopus_851
  IMAGE/octopus_851_pon.dtb := dtb octopus_851_pon
  IMAGE/octopus_641.dtb := dtb octopus_641
  IMAGE/octopus_641_pon.dtb := dtb octopus_641_pon
  IMAGE/octopus_641_10g_lan_pon.dtb := dtb octopus_641_10g_lan_pon
  IMAGE/lgp_b0_emmc_fullimage.img := fullimage 16 squashfs lgp_b0.dtb
  IMAGE/lgp_b0_pon_fullimage.img := fullimage 16 squashfs lgp_b0_pon.dtb
  IMAGE/octopus_851_fullimage.img := fullimage 16 squashfs octopus_851.dtb
  IMAGE/octopus_851_pon_fullimage.img := fullimage 16 squashfs octopus_851_pon.dtb
  IMAGE/octopus_641_aic_10g_eth.dtb := dtb octopus_641_aic_10g_eth
  IMAGE/octopus_641_aic_gsw140.dtb := dtb octopus_641_aic_gsw140
  IMAGE/octopus_641_aic_moca.dtb := dtb octopus_641_aic_moca
  IMAGE/octopus_641_fullimage.img := fullimage 16 squashfs octopus_641.dtb
  IMAGE/octopus_641_pon_fullimage.img := fullimage 16 squashfs octopus_641_pon.dtb
  IMAGE/octopus_641_10g_lan_pon_fullimage.img := fullimage 16 squashfs octopus_641_10g_lan_pon.dtb
  IMAGES += kernel.bin \
        lgp_b0.dtb lgp_b0_pon.dtb octopus_851.dtb octopus_641.dtb octopus_851_pon.dtb octopus_641_pon.dtb \
	octopus_641_aic_10g_eth.dtb \
	octopus_641_aic_gsw140.dtb \
	octopus_641_aic_moca.dtb
  FULLIMAGES := lgp_b0_emmc_fullimage.img lgp_b0_pon_fullimage.img octopus_851_fullimage.img octopus_641_fullimage.img \
		octopus_851_pon_fullimage.img octopus_641_pon_fullimage.img octopus_641_10g_lan_pon_fullimage.img

  ROOTFS := fs.rootfs
  ROOTFS_PREPARE := add-servicelayer-schema
  DEVICE_PACKAGES := $(UGW_PACKAGES_SEC) \
		     $(DSL_CPE_GFAST_PACKAGES_PRX) $(DSL_CPE_GFAST_PACKAGES_RELEASE) \
		     $(UGW_DIAG_DSL_PACKAGES) $(PM_PACKAGES)\
		     $(WAV600_UGW_PACKAGES_UCI) $(WAV600_PACKAGES_UCI)
endef
TARGET_DEVICES += LGM_UGW_SDL

define Device/LGM_UGW_SDL_DEBUG
  $(Device/LGM_GENERIC)
  DEVICE_TITLE := LGM UGW SDL DEBUG Model
  FAKED_ENV := $(FAKEROOT_PROG)
  IMAGE/lgp_b0.dtb := dtb lgp_b0
  IMAGE/lgp_b0_pon.dtb := dtb lgp_b0_pon
  IMAGE/octopus_851.dtb := dtb octopus_851
  IMAGE/octopus_851_pon.dtb := dtb octopus_851_pon
  IMAGE/octopus_641.dtb := dtb octopus_641
  IMAGE/octopus_641_pon.dtb := dtb octopus_641_pon
  IMAGE/octopus_641_10g_lan_pon.dtb := dtb octopus_641_10g_lan_pon
  IMAGE/lgp_b0_emmc_fullimage.img := fullimage 16 squashfs lgp_b0.dtb
  IMAGE/lgp_b0_pon_fullimage.img := fullimage 16 squashfs lgp_b0_pon.dtb
  IMAGE/octopus_851_fullimage.img := fullimage 16 squashfs octopus_851.dtb
  IMAGE/octopus_641_fullimage.img := fullimage 16 squashfs octopus_641.dtb
  IMAGE/octopus_851_pon_fullimage.img := fullimage 16 squashfs octopus_851_pon.dtb
  IMAGE/octopus_641_aic_10g_eth.dtb := dtb octopus_641_aic_10g_eth
  IMAGE/octopus_641_aic_gsw140.dtb := dtb octopus_641_aic_gsw140
  IMAGE/octopus_641_aic_moca.dtb := dtb octopus_641_aic_moca
  IMAGE/octopus_641_pon_fullimage.img := fullimage 16 squashfs octopus_641_pon.dtb
  IMAGE/octopus_641_10g_lan_pon_fullimage.img := fullimage 16 squashfs octopus_641_10g_lan_pon.dtb
  IMAGES += kernel.bin \
        lgp_b0.dtb lgp_b0_pon.dtb octopus_851.dtb octopus_851_pon.dtb octopus_641.dtb octopus_641_pon.dtb \
	octopus_641_aic_10g_eth.dtb \
	octopus_641_aic_gsw140.dtb \
	octopus_641_aic_moca.dtb
  FULLIMAGES := lgp_b0_emmc_fullimage.img lgp_b0_pon_fullimage.img octopus_851_fullimage.img octopus_641_fullimage.img \
		octopus_851_pon_fullimage.img octopus_641_pon_fullimage.img octopus_641_10g_lan_pon_fullimage.img
  ROOTFS := fs.rootfs
  ROOTFS_PREPARE := add-servicelayer-schema
  DEVICE_PACKAGES := $(UGW_PACKAGES_SEC)\
		     $(DSL_CPE_GFAST_PACKAGES_PRX) $(DSL_CPE_GFAST_PACKAGES_DEBUG)\
		     $(PM_DEBUG_PACKAGES) $(PM_PACKAGES)\
		     $(WAV600_UGW_PACKAGES_UCI_DEBUG) $(WAV600_PACKAGES_UCI_DEBUG) \
		     $(UGW_DIAG_DSL_PACKAGES)
endef
TARGET_DEVICES += LGM_UGW_SDL_DEBUG


define Device/URX851_UGW_DEBUG
  $(Device/LGM_GENERIC)
  DEVICE_TITLE := URX851 UGW DEBUG Model
  IMAGE/lgp_b0.dtb := dtb lgp_b0
  IMAGE/lgp_b0_fixedlink.dtb := dtb lgp_b0_fixedlink
  IMAGE/octopus_851.dtb := dtb octopus_851
  IMAGE/octopus_641.dtb := dtb octopus_641
  IMAGE/octopus_641_pon.dtb := dtb octopus_641_pon
  IMAGE/octopus_641_10g_lan_pon.dtb := dtb octopus_641_10g_lan_pon
  IMAGE/octopus_641_wav700_eth.dtb := dtb octopus_641_wav700_eth
  IMAGE/octopus_641_wav700_pon.dtb := dtb octopus_641_wav700_pon
  IMAGE/octopus_641_docsis.dtb := dtb octopus_641_docsis
  IMAGE/octopus_851_wan_phy.dtb := dtb octopus_851_wan_phy
  IMAGE/octopus_851_pon.dtb := dtb octopus_851_pon
  IMAGE/octopus_851_docsis.dtb := dtb octopus_851_docsis
  IMAGE/octopus_851_pm.dtb := dtb octopus_851_pm
  IMAGE/octopus_851_fixedlink.dtb := dtb octopus_851_fixedlink
  IMAGE/octopus_851_wav700_eth.dtb := dtb octopus_851_wav700_eth
  IMAGE/octopus_851_wav700_eth_pm.dtb := dtb octopus_851_wav700_eth_pm
  IMAGE/octopus_851_wav700_pon.dtb := dtb octopus_851_wav700_pon
  IMAGE/octopus_851_wav700_pon_pm.dtb := dtb octopus_851_wav700_pon_pm
  IMAGE/octopus_851_wav700_docsis.dtb := dtb octopus_851_wav700_docsis
  IMAGE/octopus_641_aic_10g_eth.dtb := dtb octopus_641_aic_10g_eth
  IMAGE/octopus_641_aic_gsw140.dtb := dtb octopus_641_aic_gsw140
  IMAGE/octopus_641_aic_moca.dtb := dtb octopus_641_aic_moca
  IMAGE/octopus_641_pm.dtb := dtb octopus_641_pm
  IMAGE/lgp_b0_pon.dtb := dtb lgp_b0_pon
  IMAGE/lgp_b0_docsis.dtb := dtb lgp_b0_docsis
  IMAGE/lgp_b0_wav700_docsis.dtb := dtb lgp_b0_wav700_docsis
  IMAGE/lgp_b0_fullimage.img := fullimage 16 squashfs lgp_b0.dtb
  IMAGE/lgm_c0_1GB_DDR_mxl86249.dtb := dtb octopus_640_1GB_DDR_mxl86249
  IMAGE/lgm_c0_1GB_DDR_10g_lan.dtb := dtb octopus_640_1GB_DDR_10g_lan
  IMAGE/lgm_c0_1GB_DDR_10g_lan_pon.dtb := dtb octopus_640_1GB_DDR_10g_lan_pon
  IMAGE/octopus_851_fullimage.img := fullimage 16 squashfs octopus_851.dtb
  IMAGE/octopus_641_fullimage.img := fullimage 16 squashfs octopus_641.dtb
  IMAGE/octopus_851_wan_phy_fullimage.img := fullimage 16 squashfs octopus_851_wan_phy.dtb
  IMAGE/octopus_851_pon_fullimage.img := fullimage 16 squashfs octopus_851_pon.dtb
  IMAGE/octopus_851_docsis_fullimage.img := fullimage 16 squashfs octopus_851_docsis.dtb
  IMAGE/octopus_641_pon_fullimage.img := fullimage 16 squashfs octopus_641_pon.dtb
  IMAGE/octopus_641_10g_lan_pon_fullimage.img := fullimage 16 squashfs octopus_641_10g_lan_pon.dtb
  IMAGE/octopus_641_wav700_eth_fullimage.img := fullimage 16 squashfs octopus_641_wav700_eth.dtb
  IMAGE/octopus_641_wav700_pon_fullimage.img := fullimage 16 squashfs octopus_641_wav700_pon.dtb
  IMAGE/octopus_641_pm_fullimage.img := fullimage 16 squashfs octopus_641_pm.dtb
  IMAGE/octopus_851_wav700_eth_fullimage.img := fullimage 16 squashfs octopus_851_wav700_eth.dtb
  IMAGE/octopus_851_wav700_eth_pm_fullimage.img := fullimage 16 squashfs octopus_851_wav700_eth_pm.dtb
  IMAGE/octopus_851_wav700_pon_fullimage.img := fullimage 16 squashfs octopus_851_wav700_pon.dtb
  IMAGE/octopus_851_wav700_pon_pm_fullimage.img := fullimage 16 squashfs octopus_851_wav700_pon_pm.dtb
  IMAGE/octopus_851_wav700_docsis_fullimage.img := fullimage 16 squashfs octopus_851_wav700_docsis.dtb
  IMAGE/octopus_851_pm_fullimage.img := fullimage 16 squashfs octopus_851_pm.dtb
  IMAGE/lgp_b0_pon_fullimage.img := fullimage 16 squashfs lgp_b0_pon.dtb
  IMAGE/lgm_c0_1GB_DDR_mxl86249_fullimage.img := fullimage 16 squashfs lgm_c0_1GB_DDR_mxl86249.dtb
  IMAGE/lgm_c0_1GB_DDR_10g_lan_fullimage.img := fullimage 16 squashfs lgm_c0_1GB_DDR_10g_lan.dtb
  IMAGE/lgm_c0_1GB_DDR_10g_lan_pon_fullimage.img := fullimage 16 squashfs lgm_c0_1GB_DDR_10g_lan_pon.dtb
  IMAGES += kernel.bin lgp_b0.dtb lgp_b0_pon.dtb octopus_851.dtb octopus_641.dtb octopus_641_pon.dtb octopus_641_wav700_eth.dtb octopus_641_wav700_pon.dtb octopus_851_wan_phy.dtb octopus_851_fixedlink.dtb octopus_851_wav700_eth.dtb octopus_851_wav700_pon.dtb lgp_b0_fixedlink.dtb \
		lgp_b0_docsis.dtb \
		lgp_b0_wav700_docsis.dtb \
		octopus_851_docsis.dtb \
		octopus_851_wav700_docsis.dtb \
		octopus_851_pm.dtb \
		octopus_641_aic_10g_eth.dtb \
		octopus_641_aic_gsw140.dtb \
		octopus_641_aic_moca.dtb \
		octopus_641_pm.dtb \
		octopus_641_docsis.dtb \
		octopus_851_wav700_eth_pm.dtb \
		octopus_851_wav700_pon_pm.dtb \
		octopus_851_pon.dtb \
		lgm_c0_1GB_DDR_mxl86249.dtb \
		lgm_c0_1GB_DDR_10g_lan.dtb \
		lgm_c0_1GB_DDR_10g_lan_pon.dtb
  FULLIMAGES := lgp_b0_fullimage.img lgp_b0_pon_fullimage.img octopus_851_fullimage.img octopus_641_fullimage.img \
		octopus_851_wan_phy_fullimage.img octopus_851_pon_fullimage.img octopus_641_pon_fullimage.img octopus_851_wav700_eth_fullimage.img octopus_851_wav700_pon_fullimage.img octopus_641_10g_lan_pon_fullimage.img octopus_641_wav700_eth_fullimage.img octopus_641_wav700_pon_fullimage.img \
		octopus_851_docsis_fullimage.img \
		octopus_641_pm_fullimage.img \
		octopus_851_pm_fullimage.img \
		octopus_851_wav700_eth_pm_fullimage.img \
		octopus_851_wav700_pon_pm_fullimage.img \
		octopus_851_wav700_docsis_fullimage.img \
		lgm_c0_1GB_DDR_mxl86249_fullimage.img \
		lgm_c0_1GB_DDR_10g_lan_fullimage.img \
		lgm_c0_1GB_DDR_10g_lan_pon_fullimage.img

  ROOTFS := fs.rootfs
  ROOTFS_PREPARE := add-servicelayer-schema
  DEVICE_PACKAGES := $(UGW_PACKAGES) \
		      $(DSL_CPE_GFAST_PACKAGES_PRX) $(DSL_CPE_GFAST_PACKAGES_DEBUG) \
		      $(PM_DEBUG_PACKAGES) $(PM_PACKAGES)\
		      $(WAV600_UGW_PACKAGES_UCI_DEBUG) $(WAV600_PACKAGES_UCI_DEBUG) \
		      $(UGW_DIAG_DSL_PACKAGES)
endef
TARGET_DEVICES += URX851_UGW_DEBUG

define Device/LGM_UGW_WLANOSP_DEBUG
  $(Device/LGM_GENERIC)
  DEVICE_TITLE := URX851 UGW DEBUG Model
  IMAGE/lgp_b0.dtb := dtb lgp_b0
  IMAGE/lgp_b0_fixedlink.dtb := dtb lgp_b0_fixedlink
  IMAGE/octopus_851.dtb := dtb octopus_851
  IMAGE/octopus_641.dtb := dtb octopus_641
  IMAGE/octopus_641_pon.dtb := dtb octopus_641_pon
  IMAGE/octopus_641_10g_lan_pon.dtb := dtb octopus_641_10g_lan_pon
  IMAGE/octopus_641_wav700_eth.dtb := dtb octopus_641_wav700_eth
  IMAGE/octopus_641_wav700_pon.dtb := dtb octopus_641_wav700_pon
  IMAGE/octopus_641_docsis.dtb := dtb octopus_641_docsis
  IMAGE/octopus_851_wan_phy.dtb := dtb octopus_851_wan_phy
  IMAGE/octopus_851_pon.dtb := dtb octopus_851_pon
  IMAGE/octopus_851_docsis.dtb := dtb octopus_851_docsis
  IMAGE/octopus_851_pm.dtb := dtb octopus_851_pm
  IMAGE/octopus_851_fixedlink.dtb := dtb octopus_851_fixedlink
  IMAGE/octopus_851_wav700_eth.dtb := dtb octopus_851_wav700_eth
  IMAGE/octopus_851_wav700_eth_pm.dtb := dtb octopus_851_wav700_eth_pm
  IMAGE/octopus_851_wav700_pon.dtb := dtb octopus_851_wav700_pon
  IMAGE/octopus_851_wav700_pon_pm.dtb := dtb octopus_851_wav700_pon_pm
  IMAGE/octopus_851_wav700_docsis.dtb := dtb octopus_851_wav700_docsis
  IMAGE/octopus_641_aic_10g_eth.dtb := dtb octopus_641_aic_10g_eth
  IMAGE/octopus_641_aic_gsw140.dtb := dtb octopus_641_aic_gsw140
  IMAGE/octopus_641_aic_moca.dtb := dtb octopus_641_aic_moca
  IMAGE/octopus_641_pm.dtb := dtb octopus_641_pm
  IMAGE/lgp_b0_pon.dtb := dtb lgp_b0_pon
  IMAGE/lgp_b0_docsis.dtb := dtb lgp_b0_docsis
  IMAGE/lgp_b0_wav700_docsis.dtb := dtb lgp_b0_wav700_docsis
  IMAGE/lgp_b0_fullimage.img := fullimage 16 squashfs lgp_b0.dtb
  IMAGE/lgm_c0_1GB_DDR_mxl86249.dtb := dtb octopus_640_1GB_DDR_mxl86249
  IMAGE/lgm_c0_1GB_DDR_10g_lan.dtb := dtb octopus_640_1GB_DDR_10g_lan
  IMAGE/octopus_851_fullimage.img := fullimage 16 squashfs octopus_851.dtb
  IMAGE/octopus_641_fullimage.img := fullimage 16 squashfs octopus_641.dtb
  IMAGE/octopus_851_wan_phy_fullimage.img := fullimage 16 squashfs octopus_851_wan_phy.dtb
  IMAGE/octopus_851_pon_fullimage.img := fullimage 16 squashfs octopus_851_pon.dtb
  IMAGE/octopus_851_docsis_fullimage.img := fullimage 16 squashfs octopus_851_docsis.dtb
  IMAGE/octopus_641_pon_fullimage.img := fullimage 16 squashfs octopus_641_pon.dtb
  IMAGE/octopus_641_10g_lan_pon_fullimage.img := fullimage 16 squashfs octopus_641_10g_lan_pon.dtb
  IMAGE/octopus_641_wav700_eth_fullimage.img := fullimage 16 squashfs octopus_641_wav700_eth.dtb
  IMAGE/octopus_641_wav700_pon_fullimage.img := fullimage 16 squashfs octopus_641_wav700_pon.dtb
  IMAGE/octopus_641_pm_fullimage.img := fullimage 16 squashfs octopus_641_pm.dtb
  IMAGE/octopus_851_wav700_eth_fullimage.img := fullimage 16 squashfs octopus_851_wav700_eth.dtb
  IMAGE/octopus_851_wav700_eth_pm_fullimage.img := fullimage 16 squashfs octopus_851_wav700_eth_pm.dtb
  IMAGE/octopus_851_wav700_pon_fullimage.img := fullimage 16 squashfs octopus_851_wav700_pon.dtb
  IMAGE/octopus_851_wav700_pon_pm_fullimage.img := fullimage 16 squashfs octopus_851_wav700_pon_pm.dtb
  IMAGE/octopus_851_wav700_docsis_fullimage.img := fullimage 16 squashfs octopus_851_wav700_docsis.dtb
  IMAGE/octopus_851_pm_fullimage.img := fullimage 16 squashfs octopus_851_pm.dtb
  IMAGE/lgp_b0_pon_fullimage.img := fullimage 16 squashfs lgp_b0_pon.dtb
  IMAGE/lgm_c0_1GB_DDR_mxl86249_fullimage.img := fullimage 16 squashfs lgm_c0_1GB_DDR_mxl86249.dtb
  IMAGE/lgm_c0_1GB_DDR_10g_lan_fullimage.img := fullimage 16 squashfs lgm_c0_1GB_DDR_10g_lan.dtb
  IMAGES += kernel.bin lgp_b0.dtb lgp_b0_pon.dtb octopus_851.dtb octopus_641.dtb octopus_641_pon.dtb octopus_641_wav700_eth.dtb octopus_641_wav700_pon.dtb octopus_851_wan_phy.dtb octopus_851_fixedlink.dtb octopus_851_wav700_eth.dtb octopus_851_wav700_pon.dtb lgp_b0_fixedlink.dtb \
		lgp_b0_docsis.dtb \
		lgp_b0_wav700_docsis.dtb \
		octopus_851_docsis.dtb \
		octopus_851_wav700_docsis.dtb \
		octopus_851_pm.dtb \
		octopus_641_aic_10g_eth.dtb \
		octopus_641_aic_gsw140.dtb \
		octopus_641_aic_moca.dtb \
		octopus_641_pm.dtb \
		octopus_641_docsis.dtb \
		octopus_851_wav700_eth_pm.dtb \
		octopus_851_wav700_pon_pm.dtb \
		octopus_851_pon.dtb \
		lgm_c0_1GB_DDR_mxl86249.dtb \
		lgm_c0_1GB_DDR_10g_lan.dtb
  FULLIMAGES := lgp_b0_fullimage.img lgp_b0_pon_fullimage.img octopus_851_fullimage.img octopus_641_fullimage.img \
		octopus_851_wan_phy_fullimage.img octopus_851_pon_fullimage.img octopus_641_pon_fullimage.img octopus_851_wav700_eth_fullimage.img octopus_851_wav700_pon_fullimage.img octopus_641_10g_lan_pon_fullimage.img octopus_641_wav700_eth_fullimage.img octopus_641_wav700_pon_fullimage.img \
		octopus_851_docsis_fullimage.img \
		octopus_641_pm_fullimage.img \
		octopus_851_pm_fullimage.img \
		octopus_851_wav700_eth_pm_fullimage.img \
		octopus_851_wav700_pon_pm_fullimage.img \
		octopus_851_wav700_docsis_fullimage.img \
		lgm_c0_1GB_DDR_mxl86249_fullimage.img \
		lgm_c0_1GB_DDR_10g_lan_fullimage.img

  ROOTFS := fs.rootfs
  ROOTFS_PREPARE := add-servicelayer-schema
  DEVICE_PACKAGES := $(UGW_PACKAGES) \
		      $(DSL_CPE_GFAST_PACKAGES_PRX) $(DSL_CPE_GFAST_PACKAGES_DEBUG) \
		      $(PM_DEBUG_PACKAGES) $(PM_PACKAGES)\
		      $(WAV700_UGW_PACKAGES_UCI_OSP_DEBUG) $(WAV700_PACKAGES_UCI_OSP_DEBUG) \
		      $(UGW_DIAG_DSL_PACKAGES)
endef
TARGET_DEVICES += LGM_UGW_WLANOSP_DEBUG

define Device/URX851_UGW_DEBUG_PD
  $(Device/LGM_GENERIC)
  DEVICE_TITLE := URX851 UGW DEBUG PD Model
  IMAGE/lgp_b0.dtb := dtb lgp_b0
  IMAGE/lgp_b0_fixedlink.dtb := dtb lgp_b0_fixedlink
  IMAGE/lgp_b0_no_fan.dtb := dtb lgp_b0_no_fan
  IMAGE/lgp_b0_pon.dtb := dtb lgp_b0_pon
  IMAGE/lgp_wav700_eth.dtb := dtb lgp_wav700_eth
  IMAGE/lgp_wav700_pon.dtb := dtb lgp_wav700_pon
  IMAGE/lgm_evm_b0_ebu_nand.dtb := dtb lgm_evm_b0_ebu_nand
  IMAGE/lgm_evm_b0_3band_ebu_nand.dtb := dtb lgm_evm_b0_3band_ebu_nand
  IMAGE/lgm_evm_b0_qspi_nand.dtb := dtb lgm_evm_b0_qspi_nand
  IMAGE/lgm_evm_b0_slic200_cibb_ebu_nand.dtb := dtb lgm_evm_b0_slic200_cibb_ebu_nand
  IMAGE/lgm_evm_b0_slic200_cibb_qspi_nand.dtb := dtb lgm_evm_b0_slic200_cibb_qspi_nand
  IMAGE/lgm_evm_b0_slic200_cifb_ebu_nand.dtb := dtb lgm_evm_b0_slic200_cifb_ebu_nand
  IMAGE/lgm_evm_b0_slic200_cifb_qspi_nand.dtb := dtb lgm_evm_b0_slic200_cifb_qspi_nand
  IMAGE/lgm_evm_b0_slic200_ifb_ebu_nand.dtb := dtb lgm_evm_b0_slic200_ifb_ebu_nand
  IMAGE/lgm_evm_b0_slic200_ifb_qspi_nand.dtb := dtb lgm_evm_b0_slic200_ifb_qspi_nand
  IMAGE/lgm_evm_b0_slic200_ib_ebu_nand.dtb := dtb lgm_evm_b0_slic200_ib_ebu_nand
  IMAGE/lgm_evm_b0_slic200_ib_qspi_nand.dtb := dtb lgm_evm_b0_slic200_ib_qspi_nand
  IMAGE/lgm_evm_b0_slic200_cib_ebu_nand.dtb := dtb lgm_evm_b0_slic200_cib_ebu_nand
  IMAGE/lgm_evm_b0_slic200_cib_qspi_nand.dtb := dtb lgm_evm_b0_slic200_cib_qspi_nand
  IMAGE/lgp_b0_fullimage.img := fullimage 16 squashfs lgp_b0.dtb
  IMAGE/lgp_b0_pon_fullimage.img := fullimage 16 squashfs lgp_b0_pon.dtb
  IMAGE/lgp_wav700_eth_fullimage.img := fullimage 16 squashfs lgp_wav700_eth.dtb
  IMAGE/lgp_wav700_pon_fullimage.img := fullimage 16 squashfs lgp_wav700_pon.dtb
  IMAGE/lgm_evm_b0_ebu_nand_fullimage.img := fullimage 16 squashfs lgm_evm_b0_ebu_nand.dtb
  IMAGES += kernel.bin \
        lgp_b0.dtb \
        lgp_b0_fixedlink.dtb \
        lgp_b0_no_fan.dtb \
        lgp_b0_pon.dtb \
        lgp_wav700_eth.dtb \
        lgp_wav700_pon.dtb \
        lgm_evm_b0_ebu_nand.dtb \
        lgm_evm_b0_3band_ebu_nand.dtb \
        lgm_evm_b0_qspi_nand.dtb \
        lgm_evm_b0_slic200_cibb_ebu_nand.dtb \
        lgm_evm_b0_slic200_cibb_qspi_nand.dtb \
        lgm_evm_b0_slic200_cifb_ebu_nand.dtb \
        lgm_evm_b0_slic200_cifb_qspi_nand.dtb \
        lgm_evm_b0_slic200_ifb_ebu_nand.dtb \
        lgm_evm_b0_slic200_ifb_qspi_nand.dtb \
        lgm_evm_b0_slic200_ib_ebu_nand.dtb \
        lgm_evm_b0_slic200_ib_qspi_nand.dtb \
        lgm_evm_b0_slic200_cib_ebu_nand.dtb \
        lgm_evm_b0_slic200_cib_qspi_nand.dtb
  FULLIMAGES := lgp_b0_fullimage.img lgp_b0_pon_fullimage.img lgm_evm_b0_ebu_nand_fullimage.img \
                lgp_wav700_eth_fullimage.img lgp_wav700_pon_fullimage.img
  ROOTFS := fs.rootfs
  ROOTFS_PREPARE := add-servicelayer-schema
  DEVICE_PACKAGES := $(UGW_PACKAGES)
endef
TARGET_DEVICES += URX851_UGW_DEBUG_PD

define Device/URX851_UGW_DEBUG_FPGA
  $(Device/LGM_GENERIC)
  DEVICE_TITLE := URX851 UGW DEBUG FPGA Model
  IMAGE/lgp_b0.dtb := dtb lgp_b0
  IMAGE/lgp_b0_fixedlink.dtb := dtb lgp_b0_fixedlink
  IMAGE/lgp_wav700_eth.dtb := dtb lgp_wav700_eth
  IMAGE/lgp_wav700_pon.dtb := dtb lgp_wav700_pon
  IMAGE/octopus_851.dtb := dtb octopus_851
  IMAGE/octopus_641.dtb := dtb octopus_641
  IMAGE/octopus_851_wan_phy.dtb := dtb octopus_851_wan_phy
  IMAGE/octopus_851_pon.dtb := dtb octopus_851_pon
  IMAGE/octopus_851_fixedlink.dtb := dtb octopus_851_fixedlink
  IMAGE/octopus_851_wav700_eth.dtb := dtb octopus_851_wav700_eth
  IMAGE/lgp_b0_pon.dtb := dtb lgp_b0_pon
  IMAGE/lgp_b0_fullimage.img := fullimage 16 squashfs lgp_b0.dtb
  IMAGE/octopus_851_fullimage.img := fullimage 16 squashfs octopus_851.dtb
  IMAGE/octopus_641_fullimage.img := fullimage 16 squashfs octopus_641.dtb
  IMAGE/octopus_851_wan_phy_fullimage.img := fullimage 16 squashfs octopus_851_wan_phy.dtb
  IMAGE/octopus_851_pon_fullimage.img := fullimage 16 squashfs octopus_851_pon.dtb
  IMAGE/octopus_851_wav700_eth_fullimage.img := fullimage 16 squashfs octopus_851_wav700_eth.dtb
  IMAGE/lgp_b0_pon_fullimage.img := fullimage 16 squashfs lgp_b0_pon.dtb
  IMAGE/lgp_wav700_eth_fullimage.img := fullimage 16 squashfs lgp_wav700_eth.dtb
  IMAGE/lgp_wav700_pon_fullimage.img := fullimage 16 squashfs lgp_wav700_pon.dtb
  IMAGES += kernel.bin lgp_b0.dtb lgp_b0_pon.dtb octopus_851.dtb octopus_641.dtb octopus_851_wan_phy.dtb octopus_851_fixedlink.dtb lgp_b0_fixedlink.dtb \
                lgp_wav700_eth.dtb \
                lgp_wav700_pon.dtb \
                octopus_851_wav700_eth.dtb \
                octopus_851_pon.dtb
  FULLIMAGES := lgp_b0_fullimage.img lgp_b0_pon_fullimage.img octopus_851_fullimage.img octopus_641_fullimage.img \
                octopus_851_wan_phy_fullimage.img octopus_851_pon_fullimage.img lgp_wav700_pon_fullimage.img \
		 lgp_wav700_eth_fullimage.img octopus_851_wav700_eth_fullimage.img
  ROOTFS := fs.rootfs
  ROOTFS_PREPARE := add-servicelayer-schema
  DEVICE_PACKAGES := $(UGW_PACKAGES) $(DSL_CPE_GFAST_PACKAGES_PRX) \
                     $(WAV700_PACKAGES_UCI_DEBUG) $(WAV700_UGW_PACKAGES_UCI_DEBUG) $(DSL_CPE_GFAST_PACKAGES_DEBUG) \
                     $(PM_DEBUG_PACKAGES) $(WAV700-FPGA_PACKAGES) $(EXTRA_OPENWRT_PACKAGES) \
                     $(UGW_DIAG_DSL_PACKAGES)
endef
TARGET_DEVICES += URX851_UGW_DEBUG_FPGA

define Device/URX851_UGW_DEBUG_DXS
  $(Device/LGM_GENERIC)
  DEVICE_TITLE := URX851 UGW DEBUG Model with Voice DXS TID
  IMAGE/lgm_evm_b0_ebu_nand.dtb := dtb lgm_evm_b0_ebu_nand
  IMAGE/lgm_evm_b0_dxs_ebu_nand.dtb := dtb lgm_evm_b0_dxs_ebu_nand
  IMAGE/lgm_evm_b0_dxs_qspi_nand.dtb := dtb lgm_evm_b0_dxs_qspi_nand
  IMAGE/lgm_evm_b0_slic200_cibb_ebu_nand.dtb := dtb lgm_evm_b0_slic200_cibb_ebu_nand
  IMAGE/lgm_evm_b0_slic200_cibb_qspi_nand.dtb := dtb lgm_evm_b0_slic200_cibb_qspi_nand
  IMAGE/lgm_evm_b0_slic200_cifb_ebu_nand.dtb := dtb lgm_evm_b0_slic200_cifb_ebu_nand
  IMAGE/lgm_evm_b0_slic200_cifb_qspi_nand.dtb := dtb lgm_evm_b0_slic200_cifb_qspi_nand
  IMAGE/lgm_evm_b0_slic200_ifb_ebu_nand.dtb := dtb lgm_evm_b0_slic200_ifb_ebu_nand
  IMAGE/lgm_evm_b0_slic200_ifb_qspi_nand.dtb := dtb lgm_evm_b0_slic200_ifb_qspi_nand
  IMAGE/lgm_evm_b0_slic200_ib_ebu_nand.dtb := dtb lgm_evm_b0_slic200_ib_ebu_nand
  IMAGE/lgm_evm_b0_slic200_ib_qspi_nand.dtb := dtb lgm_evm_b0_slic200_ib_qspi_nand
  IMAGE/lgm_evm_b0_slic200_cib_ebu_nand.dtb := dtb lgm_evm_b0_slic200_cib_ebu_nand
  IMAGE/lgm_evm_b0_slic200_cib_qspi_nand.dtb := dtb lgm_evm_b0_slic200_cib_qspi_nand
  IMAGE/lgm_evm_b0_dxs_ebu_nand_fullimage.img := fullimage 16 squashfs lgm_evm_b0_dxs_ebu_nand.dtb
  IMAGE/lgm_evm_b0_dxs_qspi_nand_fullimage.img := fullimage 16 squashfs lgm_evm_b0_dxs_qspi_nand.dtb
  IMAGE/lgm_evm_b0_ebu_nand_fullimage.img := fullimage 16 squashfs lgm_evm_b0_ebu_nand.dtb
  IMAGES += kernel.bin \
        lgm_evm_b0_dxs_ebu_nand.dtb \
        lgm_evm_b0_dxs_qspi_nand.dtb \
        lgm_evm_b0_slic200_cibb_ebu_nand.dtb \
        lgm_evm_b0_slic200_cibb_qspi_nand.dtb \
        lgm_evm_b0_slic200_cifb_ebu_nand.dtb \
        lgm_evm_b0_slic200_cifb_qspi_nand.dtb \
        lgm_evm_b0_slic200_ifb_ebu_nand.dtb \
        lgm_evm_b0_slic200_ifb_qspi_nand.dtb \
        lgm_evm_b0_slic200_ib_ebu_nand.dtb \
        lgm_evm_b0_slic200_ib_qspi_nand.dtb \
        lgm_evm_b0_slic200_cib_ebu_nand.dtb \
        lgm_evm_b0_slic200_cib_qspi_nand.dtb
  ROOTFS := fs.rootfs
  ROOTFS_PREPARE := add-servicelayer-schema
  FULLIMAGES :=  lgm_evm_b0_dxs_ebu_nand_fullimage.img lgm_evm_b0_dxs_qspi_nand_fullimage.img lgm_evm_b0_ebu_nand_fullimage.img
  DEVICE_PACKAGES := $(UGW_PACKAGES) $(VOIP_PACKAGES_DXS_DEBUG)\
					 $(PM_DEBUG_PACKAGES)
endef
TARGET_DEVICES += URX851_UGW_DEBUG_DXS

endif
