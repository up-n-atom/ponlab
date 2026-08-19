#
# Copyright (C) 2020 - 2022 MaxLinear, Inc.
# Copyright (C) 2018 - 2020 Intel Corporation
# Copyright (C) 2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/version.mk

PKG_NAME:=pon-base-files
PKG_RELEASE:=1
PKG_LICENSE:=GPL-2.0-only
PON_VERSION_NUMBER:=1.23.0

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SUBMENU:=Base Files
  SECTION:=base
  CATEGORY:=MaxLinear
  TITLE:=Base files for PON systems.
  URL:=https://www.maxlinear.com
  MAINTAINER:=MaxLinear
  DEPENDS:=\
	+@IMAGEOPT +@VERSIONOPT
endef

define Package/$(PKG_NAME)/description
  This package contains the subsystem version information and
  optionally creates an image version file and banner for PON systems.
endef

define Package/$(PKG_NAME)/config
	menu "configuration"
	depends on PACKAGE_$(PKG_NAME)
		source "$(SOURCE)/config/Config.in"
	endmenu
endef

define Build/Prepare
endef

define Build/Compile
endef

define Build/Configure
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/etc
	echo "$(PON_VERSION_NUMBER)" > $(1)/etc/pon.ver
ifneq ($(CONFIG_PON_BASE_FILES_COPY),)
	[ -d ./files ] && $(CP) ./files/* $(1)/
	$(VERSION_SED_SCRIPT) $(1)/etc/banner \
		$(1)/etc/image_version
endif
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
