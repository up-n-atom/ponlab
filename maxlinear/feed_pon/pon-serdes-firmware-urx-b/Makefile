#
# Copyright (C) 2021 - 2023 MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=pon-serdes-firmware-urx-b
PKG_VERSION:=1.1.2
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=pon_serdes_firmware_binary
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=abea300dc5643b7a773f72413264a0999ec0cef647bf9dee32a11169e0bda596
PKG_LICENSE:=MaxLinear Commercial Use License
PKG_LICENSE_FILES:=LICENSE

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=firmware
  CATEGORY:=MaxLinear
  TITLE:=PON SerDes FW for URX type B
  MAINTAINER:=MaxLinear
  SUBMENU:=PON
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/lib/firmware/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/fw.bin $(1)/lib/firmware/intel_pon_serdes_fw_urx_b.bin
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
