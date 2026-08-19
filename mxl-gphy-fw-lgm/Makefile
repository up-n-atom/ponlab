#
# Copyright (C) 2023 MaxLinear, Inc.
# Copyright (c) 2017 - 2020 Intel Corporation
#

include $(TOPDIR)/rules.mk

PKG_NAME:=mxl-gphy-fw-lgm
PKG_VERSION:=6.4
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=fw_phy_ip_urx8xx
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)
PKG_MIRROR_HASH:=53aa69ace4d8e10cd41cae7194391395a276335b6aa9cff18ebd69fc088ddede
PKG_RELEASE:=1
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=MaxLinear

include $(INCLUDE_DIR)/package.mk

define Package/mxl-gphy-fw-lgm
  SECTION:=firmware
  CATEGORY:=MaxLinear
  URL:=http://www.maxlinear.com/
  MAINTAINER:=MaxLinear
  TITLE:=GPHY Firmware URX8xx (LGM) Gigabit Ethernet
endef

define Build/Compile
endef

define Package/mxl-gphy-fw-lgm/install
	$(INSTALL_DIR) $(1)/lib/firmware
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/p34x_fw_v8977.bin $(1)/lib/firmware/mxl_fw_PHY34X_URx8xx_Axx.bin
	$(LN) mxl_fw_PHY34X_URx8xx_Axx.bin $(1)/lib/firmware/ltq_fw_PHY34X_URx8xx_Axx.bin
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/intel_fw_PHY34X_OTP_URx8xx_Axx_R8bab.bin $(1)/lib/firmware/mxl_fw_PHY34X_URx8xx_Bxx.bin
	$(LN) mxl_fw_PHY34X_URx8xx_Bxx.bin $(1)/lib/firmware/ltq_fw_PHY34X_URx8xx_Bxx.bin
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/GPY24X_FLASH_IMAGE_0x8BAB_MSIGNED_BSTEP.bin $(1)/lib/firmware/mxl_fw_PHY34X_URx6xx_Bxx.bin
endef

$(eval $(call BuildPackage,mxl-gphy-fw-lgm))
