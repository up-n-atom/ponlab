#
# Copyright © 2020-2024 MaxLinear, Inc.
# Copyright (C) 2019-2020 Intel Corporation
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ppv4-firmware-lgm
PKG_BASE_NAME:=ppv4-fw
PKG_SOURCE_PROTO:=git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=ppv4_fw_bin
PKG_VERSION:=1.5.47
PKG_RELEASE:=1
PKG_SOURCE:=$(PKG_BASE_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=f7bea47fb952330424d3e95484ea18b30ce5551b7b3890d67045c9fa0193113b
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_LICENSE:=MaxLinear
PKG_LICENSE_FILES:=LICENSE
PKG_SOURCE_MIRROR:=0

# Prevent OpenWrt from stripping this firmware binary
STRIP=/bin/true
RSTRIP=/bin/true

-include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  TITLE:=PPv4 Firmware for LGM
  SECTION:=firmware
  CATEGORY:=MaxLinear
  MAINTAINER:=MaxLinear
  SUBMENU:=Firmware
  DEPENDS:=@TARGET_intel_x86_lgm
endef

define Package/$(PKG_NAME)/config
  $(call Package/$(PKG_NAME)/override_version)
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/lib/firmware/
	$(CP) $(PKG_BUILD_DIR)/egress_uc.bin   $(1)/lib/firmware/egress_uc.bin
	$(CP) $(PKG_BUILD_DIR)/ingress_uc.bin  $(1)/lib/firmware/ingress_uc.bin
	$(CP) $(PKG_BUILD_DIR)/ppv4-qos-fw.elf $(1)/lib/firmware/ppv4-qos-fw.elf
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
