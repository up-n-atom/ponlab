#
# Copyright (C) 2009 OpenWrt.org
# Copyright (c) 2018 Intel Corporation
# Copyright (c) 2022-2023 MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=pon-img-lib
PKG_APP_NAME:=sw-upgrade-app
PKG_VERSION:=2.4.0
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=pon_img_lib
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=f73c591c419021163e1877e05eb2acf537b7c9677dd4c0665a68c8e12c436f2d
PKG_RELEASE:=1
PKG_FIXUP:=autoreconf
PKG_BUILD_PARALLEL:=1
PKG_BIN_DIR=usr/bin
PKG_INSTALL:=1
PKG_LICENSE:=BSD-2-Clause
PKG_LICENSE_FILES:=LICENSE

-include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# avoid build directly in the source directories
PKG_AUTOMAKE_PATHS:=.
CONFIGURE_PATH=build_$(PKGARCH)
CONFIGURE_CMD=../configure
MAKE_PATH=$(CONFIGURE_PATH)
PKG_INSTALL_DIR=$(PKG_BUILD_DIR)/$(CONFIGURE_PATH)/ipkg-install
# update stamps with CONFIGURE_PATH
STAMP_CONFIGURED=$(PKG_BUILD_DIR)/$(CONFIGURE_PATH)/.configured$(if $(DUMP),,_$(call confvar,$(PKG_CONFIG_DEPENDS)))
STAMP_CONFIGURED_WILDCARD=$(PKG_BUILD_DIR)/$(CONFIGURE_PATH)/.configured_*
STAMP_BUILT=$(PKG_BUILD_DIR)/$(CONFIGURE_PATH)/.built

define Package/$(PKG_NAME)
  SECTION:=application
  CATEGORY:=MaxLinear
  TITLE:= PON Image Library
  URL:=https://www.maxlinear.com
  MAINTAINER:=MaxLinear
  MENU:=1
  SUBMENU:=PON
  DEPENDS:=+pon-adapter +ifxos +ubi-utils +libubox \
	+libsafec3-7
endef

define Package/$(PKG_NAME)-$(PKG_APP_NAME)
  SECTION:=application
  CATEGORY:=MaxLinear
  TITLE:= PON Software Upgrade Application
  URL:=https://www.maxlinear.com
  MAINTAINER:=MaxLinear
  MENU:=1
  SUBMENU:=PON
  DEPENDS:=$(PKG_NAME) +libubus
endef

define Package/$(PKG_NAME)/description
 PON Image Library
endef

define Package/$(PKG_NAME)-$(PKG_APP_NAME)/description
 PON Software Upgrade Application
 This tool allows executing the Software Upgrade without OMCI
endef

# make the submake silent:
ifeq ($(QUIET),1)
MAKE_FLAGS+=-s
endif
ifneq (,$(findstring 99,$(V)))
MAKE_FLAGS+=-s
endif

CONFIGURE_ARGS += \
	--enable-ifxos-include="-I$(STAGING_DIR)/usr/include/ifxos" \
	--enable-ifxos-library="-L$(STAGING_DIR)/usr/lib" \
	--enable-pon-adapter-include="-I$(STAGING_DIR)/usr/include/pon_adapter" \
	--enable-pon-adapter-library="-L$(STAGING_DIR)/usr/lib"

TARGET_CFLAGS += -Werror

#include path for safec 3.7
TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include/safeclib

define Build/Configure
	mkdir -p $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)
	$(call Build/Configure/Default)
endef

define Build/Clean
	@- [ -e $(PKG_BUILD_DIR)/$(MAKE_PATH)/Makefile ] && $(MAKE) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) clean
	@- rm -Rf $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)
	$(call Build/Clean/Default)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/pon_img
	$(CP) $(PKG_BUILD_DIR)/include/*.h $(1)/usr/include/pon_img
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libponimg.so* $(1)/usr/lib
	# install own scripts if no UGW upgrade utils package is included
ifeq ($(CONFIG_PACKAGE_secure_upgrade_utils)$(CONFIG_PACKAGE_upgrade_utils),)
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/pon_img_split $(1)/sbin/
	$(INSTALL_BIN) ./files/sbin/uboot_img_vars.sh $(1)/sbin/
	$(INSTALL_BIN) ./files/sbin/secure_upgrade.sh $(1)/sbin/
endif
endef

define Package/$(PKG_NAME)-$(PKG_APP_NAME)/install
	$(INSTALL_DIR) $(1)/$(PKG_BIN_DIR)
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/pon_sw_upgrade $(1)/$(PKG_BIN_DIR)
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)-$(PKG_APP_NAME)))
