#
# Copyright (C) 2009 OpenWrt.org
# Copyright (C) 2018 Intel Corporation
# Copyright (C) 2020 MaxLinear Inc
#
# Openwrt Makefile for cap_provide
#

include $(TOPDIR)/rules.mk

PKG_NAME:=cap_provide
PKG_VERSION:=1.2
PKG_RELEASE:=1

PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=MaxLinear
PKG_MXL_LICENSE:=MaxLinear/LICENSE

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=MaxLinear
  SUBMENU:=UGW Utilities
  TITLE:=UGW utility to provide ambient caps and user mode
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
  MENU:=1
endef

define Package/$(PKG_NAME)/description
  UGW utility to help a process running under user mode with supplementary groups
  and provide ambient caps.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cap_provide $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
