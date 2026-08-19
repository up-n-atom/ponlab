#
# Copyright (C) 2016 OpenWrt.org
# Copyright (C) 2016 Lantiq Beteiligungs-GmbH & Co. KG
# Copyright (C) 2018 Intel Corporation
# Copyright (c) 2022 MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=base-files-ieee1588
PKG_RELEASE:=1
PKG_LICENSE:=GPL-2.0-only

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SUBMENU:=Base Files
  SECTION:=base
  CATEGORY:=MaxLinear
  TITLE:=Base files for Linux PTP IEEE 1588.
  DEPENDS:=+linuxptp
  URL:=https://www.maxlinear.com
  MAINTAINER:=MaxLinear
endef

define Package/$(PKG_NAME)/description
 This package contains script files for Linux PTP IEEE 1588 utilities programs.
endef

define Build/Prepare
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	[ -d ./files ] && $(CP) ./files/* $(1)/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
