#
# Copyright © 2020-2024 MaxLinear, Inc.
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
# 

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=switch_cli_ugw8
PKG_VERSION:=1.3.7

PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
PKG_RELEASE:=1
PKG_SOURCE:=$(PKG_NAME)-$(PKG_SOURCE_VERSION).tar.gz
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=switch-cli
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)
PKG_CONFIG_DEPENDS:=CONFIG_LTQ_SWITCH_CLI_UGW8_HELP_PRINTOUT
PKG_FIXUP:=autoreconf
PKG_MIRROR_HASH:=9119dd1c5f5a24fea7b4ec92690b62512907069b223b9e90e415f2af15c238ee

-include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=driver
  CATEGORY:=MaxLinear
  TITLE:=Ethernet Switch New Command Line Utility
  DEPENDS:=@(TARGET_intel_mips_xrx500||TARGET_intel_mips_prx300||TARGET_intel_x86_lgm)
  URL:=http://www.maxlinear.com/
  MAINTAINER:=MaxLinear Pvt Ltd
  MENU:=1
endef

define Package/$(PKG_NAME)/description
	Advanced Utility for Ethernet Switch driver with ioctl support
endef

### Menuconfig ##################################
define Package/$(PKG_NAME)/config
        menu "$(PKG_NAME) features"
        depends on PACKAGE_$(PKG_NAME)
        source "$(SOURCE)/config/Config.in"
        endmenu
        # enable to get custom source_dir option
        #$ (call Package/$(PKG_NAME)/override_source_path)
endef

CONFIGURE_ARGS += \
	--enable-kernelincl="$(LINUX_DIR)/include/net/switch_api/" \
	--enable-gswip \
	$(call autoconf_bool,CONFIG_LTQ_SWITCH_CLI_UGW8_HELP_PRINTOUT,printhelp)

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/switch_cli $(1)/usr/bin/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
