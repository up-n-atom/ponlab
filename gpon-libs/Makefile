#
# Copyright (C) 2021 - 2023 MaxLinear, Inc.
# Copyright (C) 2013 OpenWrt.org
# Copyright (C) 2013 lantiq.com
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=gpon-libs
PKG_VERSION:=1.8.0
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=gpon_libs
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=e588d25d3e868a306421ecd7d437c57dcd7dee0e050d78e7d966bbc17bdda764
PKG_RELEASE:=1
PKG_FIXUP:=autoreconf
PKG_BUILD_PARALLEL:=1
PKG_LICENSE:=GPL-2.0-only
PKG_LICENSE_FILES:=License

PKG_CONFIG_DEPENDS:= \
	CONFIG_PKG_ASLR_PIE \
	CONFIG_GPON_TOP_LINE_LEN \
	CONFIG_GPON_TOP_LINE_MAX

PKG_INSTALL:=1

-include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# avoid build directly in the source directories
PKG_AUTOMAKE_PATHS := .
CONFIGURE_PATH = build_$(PKGARCH)
CONFIGURE_CMD = ../configure
MAKE_PATH = $(CONFIGURE_PATH)
PKG_INSTALL_DIR = $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)/ipkg-install

define Package/$(PKG_NAME)
  SECTION:=lib
  CATEGORY:=MaxLinear
  TITLE:= PON Libraries
  URL:=https://www.maxlinear.com
  MAINTAINER:=MaxLinear
  MENU:=1
  SUBMENU:=PON
endef

define Package/$(PKG_NAME)/description
   PON Libraries
endef

define Package/$(PKG_NAME)/config
  menu "configuration"
  depends on PACKAGE_$(PKG_NAME)
    source "$(SOURCE)/config/Config.in"
  endmenu
  $(call Package/$(PKG_NAME)/override_source_path)
endef

# make the submake silent:
ifeq ($(QUIET),1)
MAKE_FLAGS+=-s
endif
ifneq (,$(findstring 99,$(V)))
MAKE_FLAGS+=-s
endif

CONFIGURE_ARGS += \
	--enable-targetincl=$(STAGING_DIR)/usr/include \
	--enable-linux \
	--disable-ecos \
	--enable-top-line-len=$(CONFIG_GPON_TOP_LINE_LEN) \
	--enable-top-line-max=$(CONFIG_GPON_TOP_LINE_MAX)

define Build/Configure
	mkdir -p $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)
	$(call Build/Configure/Default)
endef

define Build/Clean
	@- rm -Rf $(PKG_BUILD_DIR)/ipkg*
	@- [ -e $(PKG_BUILD_DIR)/$(MAKE_PATH)/Makefile ] && $(MAKE) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) clean
	@- rm -Rf $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)
	$(call Build/Clean/Default)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/gpon_libs
	$(CP) $(PKG_INSTALL_DIR)/usr/include/gpon_libs/*.h $(1)/usr/include/gpon_libs/

	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libtop.a $(1)/usr/lib/libtop.a
endef


define Build/UninstallDev
	- rm -Rf $(1)/usr/include/gpon_libs
	- rm -f $(1)/usr/lib/libtop*
endef

# Dummy install to create a package at all
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
