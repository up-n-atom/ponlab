#
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=mpe_fw_prx300
PKG_VERSION:=2.1.19.1
PKG_SOURCE_PROTO:=git
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_PROTO).tar.gz
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=mpe_fw_prx
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_LICENSE:=MaxLinear
PKG_LICENSE_FILES:=LICENSE
PKG_MXL_LICENSE:=MaxLinear/LICENSE
PKG_MIRROR_HASH:=f6613a4e1a69499638c556e57a554ee30a4d357396c3aa74785f0f93da657cdb

-include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=firmware
  CATEGORY:=MaxLinear
  TITLE:=MPE FW for PRX300
  DEPENDS:=@TARGET_intel_mips_prx300
  MAINTAINER:=MaxLinear
  SUBMENU:=Firmware
endef

define Package/$(PKG_NAME)/config
  $(call Package/$(PKG_NAME)/override_version)
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/lib/firmware/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/mpe_fw_be.img $(1)/lib/firmware/mpe_fw_be.img
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
