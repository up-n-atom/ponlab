#
# Copyright (C) 2021 MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=obudpst

PKG_SOURCE_PROTO:=git
PKG_VERSION:=v8.0.0
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL=https://github.com/BroadbandForum/obudpst.git
PKG_SOURCE_VERSION:=v8.0.0
PKG_MIRROR_HASH:=c0d471ab8f2caae88cef3106f420021bf003ca9235ca4a9fbcb0b846fd906e01

PKG_LICENSE:=BSD-3c
PKG_LICENSE_FILES:=LICENSE
CMAKE_INSTALL:=1
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/$(PKG_NAME)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Open Broadband-UDP Speed Test
  URL:=https://github.com/BroadbandForum/obudpst.git
  DEPENDS:=+libopenssl
endef

define Package/$(PKG_NAME)/description
  Open Broadband-UDP Speed Test (OB-UDPST) is a client/server software utility
  to demonstrate one approach of doing IP capacity measurements.
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/udpst $(1)/usr/bin
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
