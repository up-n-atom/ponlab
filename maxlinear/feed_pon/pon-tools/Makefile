#
# Copyright (C) 2020 - 2023 MaxLinear, Inc.
# Copyright (c) 2019 Intel Corporation
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=pon-tools
PKG_VERSION:=1.11.1
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=pon_tools
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=e73ad4bc4a26b3db2a4048a6a21ec007f657461ba6cb87fe53ba751d4ee2d312
PKG_RELEASE:=1
PKG_BIN_DIR=usr/bin
PKG_INSTALL:=1
PKG_FIXUP:=autoreconf
PKG_BUILD_PARALLEL:=1
PKG_LICENSE:=BSD-2-Clause
PKG_LICENSE_FILES:=LICENSE

-include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# avoid build directly in the source directories
PKG_AUTOMAKE_PATHS = .
CONFIGURE_PATH = build_$(PKGARCH)/pontop
CONFIGURE_CMD = ../../configure
MAKE_PATH = $(CONFIGURE_PATH)
PKG_INSTALL_DIR = $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)/ipkg-install

define Package/$(PKG_NAME)
  SECTION:=application
  CATEGORY:=MaxLinear
  TITLE:= Debug application for PON
  URL:=https://www.maxlinear.com
  MAINTAINER:=MaxLinear
  DEPENDS:=+libpthread \
	   +librt \
	   +pon-lib \
	   +gpon-libs \
	   +libuci
  MENU:=1
  SUBMENU:=PON
endef

define Package/$(PKG_NAME)/description
   The Pontop, Debug & Trace application for PON device.
endef

# make the submake silent:
ifeq ($(QUIET),1)
MAKE_FLAGS+=-s
endif
ifneq (,$(findstring 99,$(V)))
MAKE_FLAGS+=-s
endif

CONFIGURE_ARGS += \
	--enable-ponlib-include="-I$(STAGING_DIR)/usr/include/pon_lib" \
	--enable-gponlibs-include="-I$(STAGING_DIR)/usr/include/gpon_libs"

define Build/Configure
	mkdir -p $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)
	# remove "configured" stamps of other configs
	@-rm $(PKG_BUILD_DIR)/$(CONFIGURE_PATH).configured*
	$(call Build/Configure/Default)
endef

define Build/Clean
	@- rm -Rf $(PKG_BUILD_DIR)/ipkg*
	@- [ -e $(PKG_BUILD_DIR)/$(MAKE_PATH)/Makefile ] && $(MAKE) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) clean
	@- rm -Rf $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)
	$(call Build/Clean/Default)
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/$(PKG_BIN_DIR)
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/pontop $(1)/$(PKG_BIN_DIR)/pontop
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
