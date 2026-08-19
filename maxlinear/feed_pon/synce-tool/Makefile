#
# Copyright (c) 2022-2023 MaxLinear, Inc.
# Copyright (c) 2020 Intel Corporation
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=synce-tool
PKG_VERSION:=0.1.5
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=synce_tool
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=93a88a7053ad09bf7e3de43a15123862ec4c7cb8ad5eff653e64f3e0e6aeb2b7
PKG_RELEASE:=1
PKG_BIN_DIR=usr/bin
PKG_INSTALL:=1
PKG_FIXUP:=autoreconf
PKG_BUILD_PARALLEL:=1
PKG_LICENSE:=GPL-2.0-only or BSD-2-Clause
PKG_LICENSE_FILES:=LICENSE

-include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# avoid build directly in the source directories
PKG_AUTOMAKE_PATHS = .
CONFIGURE_PATH = build_$(PKGARCH)/synce_tool
CONFIGURE_CMD = ../../configure
MAKE_PATH = $(CONFIGURE_PATH)
PKG_INSTALL_DIR = $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)/ipkg-install

define Package/$(PKG_NAME)
  SECTION:=application
  CATEGORY:=MaxLinear
  TITLE:= Synchronous Ethernet evaluation tool
  URL:=https://www.maxlinear.com
  MAINTAINER:=MaxLinear
  DEPENDS:= +libsafec3-7
  MENU:=1
  SUBMENU:=PON
endef

define Package/$(PKG_NAME)/description
  Synchronous Ethernet evaluation tool.
endef

# make the submake silent:
ifeq ($(QUIET),1)
MAKE_FLAGS+=-s
endif
ifneq (,$(findstring 99,$(V)))
MAKE_FLAGS+=-s
endif

ifeq ($(CONFIG_TARGET_intel_x86_lgm),y)
CONFIGURE_ARGS += --enable-urx-chipset
endif

TARGET_CFLAGS += -Werror

#include path for safec 3.7
TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include/safeclib

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
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/synce_tool $(1)/$(PKG_BIN_DIR)/synce_tool
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
