#
# Copyright (C) 2009 OpenWrt.org
# Copyright (C) 2017 Intel Corporation
# Copyright (C) 2020-2022, MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: $

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=base-files-lgm
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SUBMENU:=Base Files
  SECTION:=base
  CATEGORY:=MaxLinear
  TITLE:=Common files for Intel LGM CPE Systems
  URL:=http://www.maxlinear.com/
  MAINTAINER:=MaxLinear Inc
  DEPENDS:=+busybox
endef

#define Package/$(PKG_NAME)/conffiles
#endef

BIN_FOLDER:=opt/intel/bin

define Package/$(PKG_NAME)/description
 This package contains files and system scripts for OpenWrt,
 common for all MaxLinear CPE Targets.
endef

define Build/Prepare
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/mnt
	$(INSTALL_DIR) $(1)/usr/sbin
	$(LN) /tmp/mounts $(1)/mnt/usb
	$(CP) ./files/* $(1)/
	$(if $(CONFIG_PACKAGE_hostapd_wave),$(LN) /$(BIN_FOLDER)/hostapd $(1)/usr/sbin/hostapd,)
	$(if $(CONFIG_TARGET_intel_x86_lgm),mv -f $(1)/etc/board.d/02_network_lgm $(1)/etc/board.d/02_network,rm -f $(1)/etc/board.d/02_network_lgm)
	$(if $(CONFIG_TARGET_DEVICE_intel_x86_lgm_DEVICE_CBSP_HAPS_C0),mv -f $(1)/etc/config/system-haps $(1)/etc/config/system,rm -f $(1)/etc/config/system-haps)
	$(if $(CONFIG_TARGET_DEVICE_intel_x86_lgm_DEVICE_CBSP_HAPS_C0)$(CONFIG_TARGET_DEVICE_intel_x86_lgm_DEVICE_CBSP_MINIFS_WAVE_B0),,rm -f $(1)/etc/config/wireless)
endef

$(eval $(call BuildPackage,$(PKG_NAME)))

