#
# Copyright (C) 2009 OpenWrt.org
# Copyright (C) 2020-2024, MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: $

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=base-files-prpl
PKG_RELEASE:=1
PKG_LICENSE:=GPL-2.0

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SUBMENU:=Base Files
  SECTION:=base
  CATEGORY:=MaxLinear
  TITLE:=Common files for MaxLinear URX CPE Systems running prplOs
  URL:=http://www.maxlinear.com/
  MAINTAINER:=MaxLinear Inc
endef

define Package/$(PKG_NAME)/description
 This package contains files and system scripts for prplOs,
 common for all MaxLinear CPE Targets.
endef

define Build/Prepare
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(CP) ./files/* $(1)/
ifeq ($(CONFIG_TARGET_DEVICE_intel_x86_lgm_DEVICE_LGM_PRPL),y)
	rm $(1)/etc/networklayout.json
	mv $(1)/etc/networklayout-lgm.json $(1)/etc/networklayout.json
else
	rm $(1)/etc/networklayout-lgm.json
endif
	echo -ne UPDK-9.1.80 > $(1)/etc/version
	date +%H%M-%d%b%y > $(1)/etc/timestamp
endef

$(eval $(call BuildPackage,$(PKG_NAME)))

