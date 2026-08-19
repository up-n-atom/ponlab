ifeq ($(SUBTARGET),falcon)
include $(INCLUDE_DIR)/version.mk

define Device/Falcon
  IMAGES := image.bin kernel.bin
  IMAGE/image.bin := append-kernel | append-rootfs | pad-rootfs | append-metadata | check-size $$$$(IMAGE_SIZE)
  IMAGE/kernel.bin := append-kernel
  UIMAGE_NAME:=$(if $(VERSION_IMAGE_SED),$(VERSION_IMAGE_SED))
endef


define Device/EASY98000
  $(Device/Falcon)
  IMAGE_SIZE := 3904k
  DEVICE_TITLE := EASY98000NOR - Lantiq Falcon Eval Board NOR
  DEVICE_PACKAGES := kmod-dm9000-nfs kmod-i2c-intel_mips kmod-eeprom-at24
  DEVICE_DTS := EASY98000NOR
endef
TARGET_DEVICES += EASY98000

define Device/EASY98000NAND
  $(Device/Falcon)
  IMAGE_SIZE := 65536k
  DEVICE_TITLE := EASY98000NAND - Lantiq Falcon Eval Board NAND
  DEVICE_PACKAGES := kmod-dm9000-nfs kmod-i2c-intel_mips kmod-eeprom-at24
endef
TARGET_DEVICES += EASY98000NAND

define Device/EASY98000SFLASH
  $(Device/Falcon)
  IMAGE_SIZE := 7424k
  DEVICE_TITLE := EASY98000SFLASH - Lantiq Falcon Eval Board SFLASH
  DEVICE_PACKAGES := kmod-dm9000-nfs kmod-i2c-intel_mips kmod-eeprom-at24
endef
TARGET_DEVICES += EASY98000SFLASH


define Device/FALCON-MDU
  $(Device/Falcon)
  IMAGE_SIZE := 7424k
  DEVICE_TITLE := MDU - Lantiq Falcon / VINAXdp MDU Board
endef
TARGET_DEVICES += FALCON-MDU

define Device/EASY88388
  $(Device/Falcon)
  IMAGE_SIZE := 7424k
  DEVICE_TITLE := EASY88388 - Lantiq Falcon FTTDP8 Reference Board
endef
TARGET_DEVICES += EASY88388

define Device/EASY88444
  $(Device/Falcon)
  IMAGE_SIZE := 7424k
  DEVICE_TITLE := EASY88444 - Lantiq Falcon FTTdp G.FAST Reference Board
endef
TARGET_DEVICES += EASY88444


define Device/FALCON-SFP
  $(Device/Falcon)
  IMAGE_SIZE := 7424k
  DEVICE_TITLE := SFP - Lantiq Falcon SFP Stick
endef
TARGET_DEVICES += FALCON-SFP

define Device/EASY98035SYNCE
  $(Device/Falcon)
  IMAGE_SIZE := 7424k
  DEVICE_TITLE := EASY98035SYNCE - Lantiq Falcon SFP Stick with Synchronous Ethernet
endef
TARGET_DEVICES += EASY98035SYNCE

define Device/EASY98035SYNCE1588
  $(Device/Falcon)
  IMAGE_SIZE := 7424k
  DEVICE_TITLE := EASY98035SYNCE1588 - Lantiq Falcon SFP Stick with SyncE and IEEE1588
endef
TARGET_DEVICES += EASY98035SYNCE1588


define Device/EASY98020
  $(Device/Falcon)
  IMAGE_SIZE := 7424k
  DEVICE_TITLE := EASY98020 - Lantiq Falcon SFU Reference Board
endef
TARGET_DEVICES += EASY98020

define Device/EASY98020V18
  $(Device/Falcon)
  IMAGE_SIZE := 7424k
  DEVICE_TITLE := EASY98020V18 - Lantiq Falcon SFU Reference Board V1.8
endef
TARGET_DEVICES += EASY98020V18


define Device/EASY98021
  $(Device/Falcon)
  IMAGE_SIZE := 7424k
  DEVICE_TITLE := EASY98021 - Lantiq Falcon HGU Reference Board
endef
TARGET_DEVICES += EASY98021


define Device/EASY98020V18-PONIP
  $(Device/Falcon)
  IMAGE_SIZE := 7424k
  DEVICE_TITLE := EASY98020V18PONIP - Lantiq Falcon SFU Reference Board V1.8 (PON IP FPGA)
endef
TARGET_DEVICES += EASY98020V18-PONIP

define Device/EASY98020-PONIP
  $(Device/Falcon)
  IMAGE_SIZE := 7424k
  DEVICE_TITLE := EASY98020PONIP - Lantiq Falcon SFU Reference Board (PON IP FPGA)
endef
TARGET_DEVICES += EASY98020-PONIP

define Device/EASY98000-PONIP
  $(Device/Falcon)
  IMAGE_SIZE := 7424k
  DEVICE_TITLE := EASY98000NORPONIP - Lantiq Falcon Eval Board NOR (PON IP FPGA)
  DEVICE_PACKAGES := kmod-dm9000-nfs kmod-i2c-intel_mips kmod-eeprom-at24
  DEVICE_DTS := EASY98000NOR-PONIP
endef
TARGET_DEVICES += EASY98000-PONIP

endif
