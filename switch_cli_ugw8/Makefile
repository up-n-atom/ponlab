#
# Copyright (C) 2008 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
# 

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=switch_cli_ugw8
PKG_VERSION:=1.2.1

PKG_RELEASE:=1
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://localhost
PKG_BUILD_DIR:=$(BUILD_DIR)/gsw_cli_dev
PKG_CONFIG_DEPENDS:=CONFIG_LTQ_SWITCH_CLI_UGW8_HELP_PRINTOUT
PKG_FIXUP:=autoreconf
PKG_HASH:=c2da2a049a98872b6036025c8e28ef4b8d149e8f8b7627978f1dac53103dbcd8

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=driver
  CATEGORY:=Lantiq
  TITLE:=Ethernet Switch New Command Line Utility
  DEPENDS:=@(TARGET_intel_mips_xrx500||TARGET_intel_mips_prx300||TARGET_intel_x86_lgm)
  URL:=http://www.lantiq.com/
  MAINTAINER:=Lantiq GmbH
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
