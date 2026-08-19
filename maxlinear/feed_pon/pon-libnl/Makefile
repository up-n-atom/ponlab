#
# Copyright (C) 2021 - 2022 MaxLinear, Inc.
# Copyright (C) 2006-2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=pon-libnl
PKG_BASE_NAME:=libnl
PKG_VERSION:=3.5.0
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_BASE_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://github.com/thom311/libnl/releases/download/libnl$(subst .,_,$(PKG_VERSION))
PKG_HASH:=352133ec9545da76f77e70ccb48c9d7e5324d67f6474744647a7ed382b5e05fa
PKG_LICENSE:=LGPL-2.1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)/$(PKG_BASE_NAME)-$(PKG_VERSION)

PKG_INSTALL:=1
PKG_FIXUP:=autoreconf

include $(INCLUDE_DIR)/package.mk

define Package/pon-libnl/default
  SECTION:=libs
  CATEGORY:=Libraries
  URL:=http://www.infradead.org/~tgr/libnl/
  ABI_VERSION:=200
endef

define Package/pon-libnl-core
$(call Package/pon-libnl/default)
  TITLE:=Core Netlink Library (with PON extentions)
  DEPENDS:=+libpthread
endef

define Package/pon-libnl-genl
$(call Package/pon-libnl/default)
  TITLE:=Generic Netlink Library (with PON extentions)
  DEPENDS:=+pon-libnl-core
endef

define Package/pon-libnl-route
$(call Package/pon-libnl/default)
  TITLE:=Routing Netlink Library (with PON extentions)
  DEPENDS:=+pon-libnl-core
endef

define Package/pon-libnl-nf
$(call Package/pon-libnl/default)
  TITLE:=Netfilter Netlink Library (with PON extentions)
  DEPENDS:=+pon-libnl-route
endef

define Package/pon-libnl
$(call Package/pon-libnl/default)
  TITLE:=Full Netlink Library (with PON extentions)
  DEPENDS:=+pon-libnl-genl +pon-libnl-route +pon-libnl-nf
endef

define Package/pon-libnl-core/description
 Common code for all netlink libraries
endef

define Package/pon-libnl-genl/description
 Generic Netlink Library Functions
endef

define Package/pon-libnl-route/description
 Routing Netlink Library Functions
endef

define Package/pon-libnl-nf/description
 Netfilter Netlink Library Functions
endef

define Package/pon-libnl/description
 Socket handling, connection management, sending and receiving of data,
 message construction and parsing, object caching system, etc.
endef

TARGET_CFLAGS += -ffunction-sections -fdata-sections $(FPIC)

CONFIGURE_ARGS += \
	--disable-debug

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/pon-libnl3 $(1)/opt/pon/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/include/libnl3/* $(1)/usr/include/pon-libnl3/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/opt/pon/lib

	# Copy symlinks
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libnl-3.so $(1)/opt/pon/lib/libnl.so
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libnl-genl-3.so $(1)/opt/pon/lib/libnl-genl.so
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libnl-nf-3.so $(1)/opt/pon/lib/libnl-nf.so
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libnl-route-3.so $(1)/opt/pon/lib/libnl-route.so
endef

define Package/pon-libnl-core/install
	$(INSTALL_DIR) $(1)/opt/pon/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libnl-3.so.* $(1)/opt/pon/lib/
endef

define Package/pon-libnl-genl/install
	$(INSTALL_DIR) $(1)/opt/pon/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libnl-genl-3.so.* $(1)/opt/pon/lib/
endef

define Package/pon-libnl-route/install
	$(INSTALL_DIR) $(1)/opt/pon/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libnl-route-3.so.* $(1)/opt/pon/lib/
endef

define Package/pon-libnl-nf/install
	$(INSTALL_DIR) $(1)/opt/pon/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libnl-nf-3.so.* $(1)/opt/pon/lib/
endef

define Package/pon-libnl/install
	:
endef

$(eval $(call BuildPackage,pon-libnl-core))
$(eval $(call BuildPackage,pon-libnl-genl))
$(eval $(call BuildPackage,pon-libnl-route))
$(eval $(call BuildPackage,pon-libnl-nf))
$(eval $(call BuildPackage,pon-libnl))
