#
# Copyright (C) 2009 OpenWrt.org
# Copyright (C) 2017 Intel Corporation
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: $

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=base-files-bsp
PKG_RELEASE:=1


include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SUBMENU:=Base Files
  SECTION:=base
  CATEGORY:=MaxLinear
  TITLE:=Common files for BSP models
  URL:=http://www.maxlinear.com/
  MAINTAINER:=MaxLinear
endef

#define Package/$(PKG_NAME)/conffiles
#endef

BIN_FOLDER:=opt/intel/bin

define Package/$(PKG_NAME)/description
 This package contains config files and scripts that are,
 common for all BSP models.
endef

define Build/Prepare
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(CP) ./files/* $(1)/
	- (echo -ne UGW- && cat $(TOPDIR)/ugw_version) > $(1)/etc/version
	date +%H%M-%d%b%y > $(1)/etc/timestamp
	if [ -r "$(TOPDIR)/../ugw_tag" ]; then \
		$(INSTALL_DATA) $(TOPDIR)/../ugw_tag $(1)/etc/; \
	fi
endef

$(eval $(call BuildPackage,$(PKG_NAME)))

