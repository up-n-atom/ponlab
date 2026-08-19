#
# Copyright © 2022 MaxLinear, Inc.
#
# TR-143 performance diagnostic tool
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/version.mk
include $(INCLUDE_DIR)/package.mk


PKG_NAME:=tr143diagtool
PKG_VERSION:=1.0.0.1
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=MaxLinear
PKG_MXL_LICENSE:=MaxLinear/LICENSE

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=MaxLinear
  SUBMENU:=UGW Framework
  TITLE:=tr143diagtool
  DEPENDS:=+kmod-speedtest-driver +keyutils +libsafec3-7
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
endef

define Package/$(PKG_NAME)/description
	Speedtest TR-143 performance diagnostic tool
endef

TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include/safeclib
TARGET_LDFLAGS += -lsafec

ifeq ($(CONFIG_PACKAGE_kmod-dnsresolver),y)
	TARGET_CFLAGS += -DCONFIG_DNS_RESOLVER
endif

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(PKG_BUILD_DIR)/tr143diagtool.o -c $(PKG_BUILD_DIR)/tr143diagtool.c
	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/$1 $(PKG_BUILD_DIR)/tr143diagtool.o
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/bin
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
