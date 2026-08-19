#
# Copyright (C) 2010-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

# boards missing since devicetree update
#EASY50712 ARV3527P

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk
-include ./packages.mk
include secboot.mk

KERNEL_LOADADDR := 0x80002000
KERNEL_ENTRY := 0x80002000

IMAGE_BOOTCORE := $(if $(CONFIG_PACKAGE_ltq-xrx500-bootcore),$(wildcard $(BIN_DIR)/ltq-xrx500-bootcore/*$(call qstrip $(CONFIG_XRX500_BOOTCORE_IMAGE))*.bin))

ifeq ($(SUBTARGET),xway)
  UBIFS_OPTS := -m 512 -e 15872 -c 1959
else
  UBIFS_OPTS := -m 2048 -e 126KiB -c 4096
endif

define Image/Prepare/add-servicelayer-schema
	$(eval vendordir=$(1)/$(call qstrip,$(VENDOR_PATH)))
	$(STAGING_DIR_HOST)/bin/db_merge_hash.sh $(vendordir)
endef

define Image/Prepare
	$(if $(TARGET_PER_DEVICE_ROOTFS),,$(if $(CONFIG_PACKAGE_csd),$(call Image/Prepare/add-servicelayer-schema,$(TARGET_DIR))))
endef

define Build/append-bootcore
	$(if $(IMAGE_BOOTCORE),dd if=$(IMAGE_BOOTCORE) >> $@)
endef

# $(1) padding to be used.
define Build/append-rootfs-uImage
	# Add padding to make the next image start at a specified byte boundary.
	dd if=$(IMAGE_ROOTFS) of=$@.rootfs.pad bs=$(1) conv=sync
	mkimage -A mips -O linux -C lzma -T filesystem -a 0x00  \
		-e 0x00 -n 'UGW RootFS' \
		-d $@.rootfs.pad $@.rootfs

	dd if=$@.rootfs >> $@

	rm $@.rootfs.pad
	rm $@.rootfs
endef

define Build/fullimage
	mkimage -A $(LINUX_KARCH) -O linux -T multi -a 0x00 -C none \
		-e 0x00 \
		-n '$(if $(UIMAGE_NAME),$(UIMAGE_NAME),OpenWrt fullimage)' \
		-d $@ $@.new

	mv $@.new $@
endef
define Flashimage/system_sw
	dd if=$(IMAGE_ROOTFS) of=$(1).rootfs bs=$(2) conv=sync;
	echo -en "[kernelA]\nmode=ubi\nvol_id=0\nvol_type=static\nvol_name=kernelA\n" > $(1).ubinize.cfg
	echo "image=$(IMAGE_KERNEL)" >> $(1).ubinize.cfg
	echo -en "\n[rootfsA]\nmode=ubi\nvol_id=1\nvol_type=dynamic\nvol_name=rootfsA\n" >> $(1).ubinize.cfg
	echo "image=$(1).rootfs" >> $(1).ubinize.cfg
	echo -en "\n[bootcoreA]\nmode=ubi\nvol_id=2\nvol_type=dynamic\nvol_name=bootcoreA\n" >> $(1).ubinize.cfg
	echo "image=$(IMAGE_BOOTCORE)" >> $(1).ubinize.cfg
	$(if $(CONFIG_INTEL_MIPS_ROOTFS_DATA_SIZE),\
	  echo -en "\n[rootfs_data]\nmode=ubi\nvol_id=3\nvol_type=dynamic\nvol_name=rootfs_data\nvol_size=$(CONFIG_INTEL_MIPS_ROOTFS_DATA_SIZE)MiB\n" >> $(1).ubinize.cfg)
	$(STAGING_DIR_HOST)/bin/ubinize -p $(patsubst %k,%KiB,$(BLOCKSIZE)) -m $(PAGESIZE) -s $(SUBPAGESIZE) -o $(1) $(1).ubinize.cfg
	rm $(1).rootfs
	rm $(1).ubinize.cfg
endef

define Build/flashimage
	$(eval flashpdir=$(shell dirname $@))
	$(eval flashbase=$(shell basename $(flashpdir)))

	# Assign Image path to partitions.
	$(eval partition_uboot=$(BIN_DIR)/uboot-$(IMAGE_UBOOT)/u-boot-nand.bin)
	$(eval partition_ubootconfigA=$(BIN_DIR)/uboot-$(IMAGE_UBOOT)/ubootenv.img)
	$(eval partition_ubootconfigB=$(BIN_DIR)/uboot-$(IMAGE_UBOOT)/ubootenv.img)
	$(eval partition_system_sw=$(flashpdir)/system_sw.bin)

	# Last partition name to create image and then break. Last image will not be padded to its partition size.
	$(eval last_partition=system_sw)

	# Identify partition map from u-boot configuration.
	$(eval mtdparts=)
	$(if $(IMAGE_UBOOT),$(if $(wildcard $(PLATFORM_SUBDIR)/uboot_configs/$(IMAGE_UBOOT)_defconfig),
	  $(eval mtdparts=$(shell sed -n '/^CONFIG_MTDPARTS/{s/"//g;s/.*://;s/,/ /g;s/(/:/g;s/)//g;p}' $(PLATFORM_SUBDIR)/uboot_configs/$(IMAGE_UBOOT)_defconfig))
	))

	# alias to convert Kb/Mb/Gb to bytes
	$(eval conv_bytes=$$(subst g, * 1024 * 1024 * 1024,$$(subst m, * 1024 * 1024,$$(subst k, * 1024,$$(call tolower,$$(1))))))

	$(if $(mtdparts),
	  mkdir -p $(flashpdir)
	  rm -f $@
	  $(eval b_lastpart=)
	  $(foreach mtdb,$(mtdparts),
	    $(if $(b_lastpart),,
		$(eval mtdb_name=$(word 2,$(subst :, ,$(mtdb))))
		$(eval mtdb_size=$$$$(($(call conv_bytes,$(word 1,$(subst :, ,$(mtdb)))))))
		$(if $(subst $(last_partition),,$(mtdb_name)),,$(eval b_lastpart=1))
		$(if $(partition_$(mtdb_name)),
		  $(eval inp_image=$(partition_$(mtdb_name)))
		  $(eval out_image=$(flashpdir)/$(subst $(dir $(inp_image)),,$(inp_image)).padded)
		  $(if $(Flashimage/$(mtdb_name)),
		    $(call Flashimage/$(mtdb_name),$(inp_image),$(1))
		    mkdir -p $(BIN_DIR)/$(flashbase)
		    cp -f $(inp_image) $(BIN_DIR)/$(flashbase)/
		  )
		,
		  $(eval inp_image=$(flashpdir)/$(mtdb_name).bin)
		  $(eval out_image=$(flashpdir)/$(mtdb_name).bin.padded)
		  echo -en '\0' > $(inp_image);
		)
		$(if $(b_lastpart),
		  cat $(inp_image) >> $@
		,
		  dd if=$(inp_image) of=$(out_image) bs=$(mtdb_size) count=1 conv=sync;
		  cat $(out_image) >> $@
		  rm -f $(out_image);
		)
		$(if $(partition_$(mtdb_name)),,rm -f $(inp_image);)
	    )
	  )
	)
endef

# Shared device definition: applies to every defined device
define Device/Default
  PROFILES = Default
  KERNEL_DEPENDS = $$(wildcard ../dts/$$(DEVICE_DTS).dts)
  KERNEL_INITRAMFS_NAME = $$(KERNEL_NAME)-initramfs
  KERNEL := kernel-bin | append-dtb | lzma | uImage lzma
  KERNEL_INITRAMFS := kernel-bin | append-dtb | lzma | uImage lzma
  FILESYSTEMS := squashfs
  DEVICE_DTS := $(1)
  DEVICE_DTS_DIR := ../dts
  IMAGE_SIZE :=
  SUPPORTED_DEVICES = $$(DEVICE_DTS)
  IMAGES :=
endef
DEVICE_VARS += IMAGE_SIZE

define Device/NAND/xrx500
  BLOCKSIZE := 128k
  PAGESIZE := 2048
  SUBPAGESIZE := 2048
  FILESYSTEMS += ubifs
endef

define Device/NAND/prx300
  FILESYSTEMS := squashfs
  PAGESIZE := 2048
  BLOCKSIZE := 128k
  SUBPAGESIZE := 2048
endef

define Device/NAND
  $(Device/NAND/$(SUBTARGET))
endef

define Device/lantiqFullImage
  KERNEL := kernel-bin | append-dtb | lzma | pad-offset $(if $(SECUREBOOT), 4k 64, 16 0) | uImage lzma
  IMAGES := $(if $(SECUREBOOT), fullimage-secure.img, fullimage.img)
  IMAGES += $(if $(CONFIG_INTEL_MIPS_FLASH_PROGRAMMER_IMAGE),flashprogram_bins/flashimage.bin)
  IMAGE/fullimage.img := append-kernel | append-bootcore | append-rootfs-uImage 16 | fullimage | check-size $$$$(IMAGE_SIZE)
  IMAGE/fullimage-secure.img := append-kernel | append-rootfs | sign-image | sign-add-header | pad-offset 16 0 | append-bootcore | fullimage | check-size $$$$(IMAGE_SIZE)
  IMAGE/flashprogram_bins/flashimage.bin := flashimage 16
endef

define Device/lantiqBootImage
  KERNEL_INITRAMFS := kernel-bin | append-dtb $(if $(SECUREBOOT), | sign-image) | pad-offset 16 0 | uImage none
endef

include falcon.mk
include xrx500.mk
include prx300.mk
include axp.mk
-include rax40.mk
include svip_be.mk
include svip_le.mk

$(eval $(call BuildImage))
