#
# Copyright (c) 2022-2023 MaxLinear, Inc.
# Copyright (c) 2020 Intel Corporation
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=pon-kernel-fuzzer
PKG_VERSION:=1.1.1

PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=pon_kernel_fuzzer
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_SOURCE_VERSION:=2ff4ae6f4368fcb7af32ac0a05c8b575a34c6acc
PKG_SOURCE:=$(PKG_NAME)-$(PKG_SOURCE_VERSION).tar.gz
PKG_MIRROR_HASH:=da375a09a05a4b3f827cbbe06f56cc55d0931a006d966206a99dada70206ab06

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

PKG_LICENSE:=BSD-CloudFlare
PKG_LICENSE_FILES:=LICENSE-BSD-CloudFlare

include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk

define Package/$(PKG_NAME)
  SUBMENU:=PON
  CATEGORY:=MaxLinear
  TITLE:=PON Netlink fuzzer
  DEPENDS:=+libnl-genl
endef

define Package/$(PKG_NAME)/description
  This package contains multiple programs to test kernel interfaces
  in combination with "afl"
endef

TARGET_LDFLAGS := \
	-I$(STAGING_DIR)/usr/include/libnl3

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/fuzznetlink $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/fuzzioctl $(1)/usr/bin/
	$(CP) ./files/* $(1)/
endef
$(eval $(call BuildPackage,$(PKG_NAME)))
