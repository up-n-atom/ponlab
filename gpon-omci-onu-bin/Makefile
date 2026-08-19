#
# Copyright (C) 2024 MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=gpon-omci-onu-binary
PKG_VERSION:=8.19.5
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=gpon_omci_onu_binary
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=aea17abffd0b6cf0ebb71acadb59bdbce4cd93ecfcda3bb10edc246d5bd90b26
PKG_RELEASE:=1
PKG_BIN_DIR=usr/bin
PKG_LICENSE:=MaxLinear
PKG_LICENSE_FILES:=LICENSE

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=driver
  CATEGORY:=MaxLinear
  TITLE:= OMCI stack for PON (binary version)
  URL:=https://www.maxlinear.com
  MAINTAINER:=MaxLinear
  DEPENDS:= \
	@TARGET_intel_x86_lgm \
	@USE_MUSL \
	+pon-omci-files \
	+librt +ifxos \
	+argp-standalone-shared \
	+uboot-envtools \
	+pon-adapter \
	+libuci \
	+lib-cli \
	+libsafec3-7 \
	+libopenssl \
	+libubus
  SUBMENU:=PON
  VARIANT:=release
endef

define Package/$(PKG_NAME)-debug
  $(call Package/$(PKG_NAME))
  VARIANT:=debug
  TITLE += with debug information
  DEPENDS += @!PACKAGE_$(PKG_NAME)
endef

define Package/$(PKG_NAME)/description
   The Binary of OMCI stack for PON Optical Network Unit device
endef

define Package/$(PKG_NAME)-debug/description
   $(call Package/$(PKG_NAME)/description) with debug information
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/$(PKG_BIN_DIR)
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(VARIANT)/omcid $(1)/$(PKG_BIN_DIR)
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(VARIANT)/omci_simulate $(1)/$(PKG_BIN_DIR)/omci_simulate
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/scripts/omci_pipe.sh $(1)/$(PKG_BIN_DIR)/
endef

define Package/$(PKG_NAME)-debug/install
   $(call Package/$(PKG_NAME)/install,$(1))
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)-debug))
