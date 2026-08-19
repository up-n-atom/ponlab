#
# Copyright (C) 2022-2023 MaxLinear, Inc.
# Makefile for speed test tools.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=speedtest-modules
PKG_VERSION:=1.0
PKG_MAINTAINER:=MaxLinear

define Package/$(PKG_NAME)
  SECTION:=net
  CATEGORY:=MaxLinear
  SUBMENU:=Bandwidth measuring tools
  TITLE:= speedtest tools
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
endef

define Package/$(PKG_NAME)/description
 The purpose of this config is to consolidate all bandwidth measuring tools required for speedtest.
 If a tool under this config is enabled then it gets enabled at its orignal config path as well.
 If a tool under this config is disabled then it does not get disabled at its orginal config path
 but can be controlled at its orignal config path.
endef

define Package/$(PKG_NAME)/config
	source "$(SOURCE)/config/Config.in"
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
