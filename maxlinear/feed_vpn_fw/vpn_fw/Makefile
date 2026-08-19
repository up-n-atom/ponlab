#
# Copyright (C) 2020-2023 MaxLinear, Inc.
# Copyright (C) 2019 Intel Corporation
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# Openwrt feed Makefile for MaxLinear vpn-firmware
#

include $(TOPDIR)/rules.mk

PKG_NAME:=vpn-firmware
PKG_VERSION:=2.12.2
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_PROTO).tar.gz
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=vpn-firmware
PKG_RELEASE:=1
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)
PKG_MIRROR_HASH:=83a9d0bcc2d62c336315672b14e24633c9078096c3942e478b68a713845301cc
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

define Package/$(PKG_NAME)-lgm
  $(call Package/$(PKG_NAME)/Default)
  TITLE:=VPN Firmware for LGM
endef

define Build/Compile
endef

define Package/$(PKG_NAME)-lgm/install
	$(INSTALL_DIR) $(1)/lib/firmware
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/vpn_fw.img \
		$(1)/lib/firmware/vpn_fw.img
endef

$(eval $(call BuildPackage,$(PKG_NAME)-lgm))
