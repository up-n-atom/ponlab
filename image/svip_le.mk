ifeq ($(SUBTARGET),svip_le)
include $(INCLUDE_DIR)/version.mk

define Device/NAND/svip_le/512
  BLOCKSIZE = 16k
  PAGESIZE = 512
  SUBPAGESIZE = 256
  FILESYSTEMS += ubifs
  UBIFS_OPTS = -m $$(PAGESIZE) -e 15872 -c 647
  IMAGES := sysupgrade.bin factory.bin
  IMAGE/factory.bin := append-kernel | pad-to $$$$(BLOCKSIZE) | append-ubi | pad-to $$$$(BLOCKSIZE)
  IMAGE/sysupgrade.bin := sysupgrade-tar | append-metadata
  UIMAGE_NAME:=$(if $(VERSION_IMAGE_SED),$(VERSION_IMAGE_SED))
  KERNEL_INITRAMFS := kernel-bin | append-dtb | lzma | uImage lzma
endef

define Device/EASY336
  $(Device/NAND/svip_le/512)
  DEVICE_TITLE := EASY336 - Intel EASY336 evalkit
endef
TARGET_DEVICES += EASY336

define Device/EASY33616
  $(Device/NAND/svip_le/512)
  DEVICE_TITLE := EASY33616 - Intel EASY33616 evalkit
endef
TARGET_DEVICES += EASY33616

define Device/EASY33016
  $(Device/NAND/svip_le/512)
  DEVICE_TITLE := EASY33016 - Intel EASY33016 evalkit
endef
TARGET_DEVICES += EASY33016

define Device/EASY34016
  $(Device/NAND/svip_le/512)
  DEVICE_TITLE := EASY34016 - Intel EASY34016 evalkit
endef
TARGET_DEVICES += EASY34016

define Device/EASY32002
  $(Device/NAND/svip_le/512)
  DEVICE_TITLE := EASY32002 - Intel EASY32002 evalkit
endef
TARGET_DEVICES += EASY32002

define Device/EASY336R
  $(Device/NAND/svip_le/512)
  DEVICE_TITLE := EASY336R - Intel EASY336R evalkit
endef
TARGET_DEVICES += EASY336R

endif
