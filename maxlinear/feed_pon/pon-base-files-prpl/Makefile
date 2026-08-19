#
# Copyright (c) 2022 MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=pon-base-files-prpl
PKG_RELEASE:=1
PKG_LICENSE:=GPL-2.0-only

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SUBMENU:=Base Files
  SECTION:=base
  CATEGORY:=MaxLinear
  TITLE:=Base files for PON in prplOS.
  URL:=https://www.maxlinear.com
  MAINTAINER:=MaxLinear
endef

define Package/$(PKG_NAME)/description
 This package contains files and scripts for supporting PON in prplOS.
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

endef

$(eval $(call BuildPackage,$(PKG_NAME)))
