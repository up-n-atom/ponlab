#
# Copyright (C) 2009 OpenWrt.org
# Copyright (C) 2019 Intel Corporation
# Copyright (c) 2022 MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=base-files-platform-lgm
PKG_RELEASE:=1
PKG_LICENSE:=GPL-2.0-only

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SUBMENU:=Base Files
  SECTION:=base
  CATEGORY:=MaxLinear
  TITLE:=Base files for LGM platform.
  URL:=https://www.maxlinear.com
  MAINTAINER:=MaxLinear
  DEPENDS:=@TARGET_intel_x86_lgm
endef

define Package/$(PKG_NAME)/conffiles
/etc/config/gpon
endef

define Package/$(PKG_NAME)/description
 This package contains files and system scripts for PON on LGM platform.
endef

define Build/Prepare
endef

define Build/Compile
endef

define Build/Configure
endef

define Package/$(PKG_NAME)/install
	[ -d ./files ] && $(CP) ./files/* $(1)/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
