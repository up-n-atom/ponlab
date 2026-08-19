# Copyright (C) 2022-2023 MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# Openwrt feed Makefile for MaxLinear eip197-firmware
#

include $(TOPDIR)/rules.mk

PKG_NAME:=eip197-firmware
PKG_VERSION:=1.0.0
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_PROTO).tar.gz
PKG_RELEASE:=1
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=eip197-firmware
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)
PKG_MIRROR_HASH:=f17e646b99f9a4fb2c54ffb5adf27790a3e82b50252c214b75ef117a1b07b420
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=MaxLinear
PKG_MXL_LICENSE:=MaxLinear/LICENSE

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)/Default
  SECTION:=firmware
  CATEGORY:=MaxLinear
  SUBMENU:=Firmware
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
endef

define Package/$(PKG_NAME)
  $(call Package/$(PKG_NAME)/Default)
  TITLE:=Inside Secure EIP197 Firmware
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/lib/firmware/inside-secure
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/ifpp.bin \
		$(1)/lib/firmware/inside-secure/ifpp.bin
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/ipue.bin \
		$(1)/lib/firmware/inside-secure/ipue.bin
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
