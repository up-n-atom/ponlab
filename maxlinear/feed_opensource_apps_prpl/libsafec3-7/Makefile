# 
# Copyright © 2023 MaxLinear, Inc.
#
# This Makefile for Safe c library
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libsafec3-7
PKG_VERSION:=v3.7.1
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/rurban/safeclib.git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_MIRROR_HASH:=9760a4d99dd50bfb87c1bc29f2ab1c662254a9f6917590ae34c3126debb785f3
PKG_MAINTAINER:=MaxLinear
PKG_BUILD_PARALLEL:=0
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=README.md

HOST_BUILD_DEPENDS:=automake
PKG_FIXUP:=autoreconf

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/target.mk

define Package/$(PKG_NAME)
  TITLE:=Safec library v3.7
  SECTION:=libs
  CATEGORY:=Libraries
  PROVIDES:=safeclib
  URL:=https://github.com/rurban/safeclib
  DEPENDS:=@!PACKAGE_libsafec
endef

CONFIGURE_ARGS += \
	--disable-nullslack \
	--disable-doc \
	--enable-strmax=32768

define Package/$(PKG_NAME)/description
 This package contains the safec shared library.
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/safeclib
	$(CP) $(PKG_INSTALL_DIR)/usr/include/safeclib/* $(1)/usr/include/safeclib/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libsafec*.{la,a,so*} $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/libsafec.pc $(1)/usr/lib/pkgconfig/
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libsafec.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
