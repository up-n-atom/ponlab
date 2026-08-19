#
# Copyright (C) 2009 OpenWrt.org
# Copyright (C) 2011 Lantiq
# Copyright (C) 2018 - 2020 Intel Corporation
# Copyright (c) 2021 - 2022 MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=pon-base-files-debug
PKG_RELEASE:=1
PKG_LICENSE:=GPL-2.0-only

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SUBMENU:=Base Files
  SECTION:=base
  CATEGORY:=MaxLinear
  TITLE:=Base files for PON debugging.
  URL:=https://www.maxlinear.com
  MAINTAINER:=MaxLinear
  DEPENDS:=+uboot-envtools
endef

define Package/$(PKG_NAME)/description
 This package contains files and scripts for debugging PON systems.
endef

define Build/Prepare
endef

define Build/Compile
endef

define Build/Configure
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)
	$(CP) ./files/* $(1)/

ifeq ($(CONFIG_TARGET_intel_x86_lgm),y)
	$(CP) ./files-urx/* $(1)/
endif

ifeq ($(CONFIG_TARGET_intel_mips_prx300),y)
	$(CP) ./files-prx/* $(1)/
endif

endef

$(eval $(call BuildPackage,$(PKG_NAME)))
