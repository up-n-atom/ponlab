#
# Copyright (C) 2020-2024, MaxLinear, Inc.
# Copyright (C) 2019 Intel Corporation
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/version.mk

PKG_NAME:=ppmd-bin
PKG_VERSION:=2.0.9.3
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=ppmd_bin
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=b39bec9c4352a4edfd0ae5bc3e93ccceabc366800969b439821bbe7510d1b024
PKG_RELEASE:=1
PKG_BIN_DIR=$(VENDOR_PATH)/bin
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=MaxLinear
PKG_MXL_LICENSE:=MaxLinear/LICENSE

OWRT_REPO_VERSION:=$(shell echo $(VERSION_REPO) | cut -d '/' -f5 | cut -c1-5)

# Prevent OpenWrt from stripping the binary
STRIP=/bin/true
RSTRIP=/bin/true

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=power
  CATEGORY:=MaxLinear
  SUBMENU:=UGW Utilities
  TITLE:=Power Policy Management Daemon Binary
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Corporation
  DEPENDS:= @TARGET_intel_x86_lgm @USE_MUSL +libsafec3-7 +libnl +libnl-genl +libnl-route +libuci +libjson-c \
            +pon-lib +libubox +libubus
  MENU:=1
endef

define Package/$(PKG_NAME)/description
	Power Policy Management Service
endef

define Build/Compile
	$(info "")
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/$(PKG_BIN_DIR)
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/etc/config
	$(if $(CONFIG_PACKAGE_base-files-sdl), $(INSTALL_DIR) $(1)/usr/share/capabilities)
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/Owrt_$(OWRT_REPO_VERSION)/ppmd $(1)/$(PKG_BIN_DIR)/
	$(INSTALL_BIN) ./files/ppmd $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/ppmd.config $(1)/etc/config/ppmdcfg
	$(INSTALL_DATA) ./files/pm_policy.json $(1)/etc/pm_policy.json
	$(if $(CONFIG_PACKAGE_base-files-sdl), \
		$(INSTALL_DATA) ./files/sscript_ppmd_cap.json $(1)/usr/share/capabilities/sscript_ppmd_cap.json;\
		$(INSTALL_DATA) ./files/ppmd_cap.json $(1)/usr/share/capabilities/ppmd_cap.json \
	)
	$(INSTALL_BIN) ./files/ppmd_profile_helper.sh $(1)/$(PKG_BIN_DIR)/ppmd_profile_helper.sh
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
