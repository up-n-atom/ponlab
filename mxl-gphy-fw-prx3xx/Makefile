#
# Copyright (C) 2023 MaxLinear, Inc.
# Copyright (c) 2017 - 2020 Intel Corporation
#

include $(TOPDIR)/rules.mk

PKG_NAME:=mxl-gphy-fw-prx3xx
PKG_VERSION:=6.0
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=fw_phy_ip_prx3xx
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)
PKG_MIRROR_HASH:=ef87944804f14135df7c46b3a8a197d491f3aab4f9063e92f1a29779699aff09
PKG_RELEASE:=1
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=MaxLinear

include $(INCLUDE_DIR)/package.mk

define Package/mxl-gphy-fw-prx3xx
  SECTION:=firmware
  CATEGORY:=MaxLinear
  URL:=http://www.maxlinear.com/
  MAINTAINER:=MaxLinear
  TITLE:=GPHY Firmware PRX3xx Gigabit Ethernet
endef

define Build/Compile
endef

define Package/mxl-gphy-fw-prx3xx/install
	$(INSTALL_DIR) $(1)/lib/firmware
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/ltq_fw_PHY31G_IP_prx3xx_Bxx_R879d.bin $(1)/lib/firmware/mxl_fw_PHY31G_IP_prx3xx_Bxx.bin
	$(LN) mxl_fw_PHY31G_IP_prx3xx_Bxx.bin $(1)/lib/firmware/ltq_fw_PHY31G_IP_prx3xx_Bxx.bin
endef

$(eval $(call BuildPackage,mxl-gphy-fw-prx3xx))
