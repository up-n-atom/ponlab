#
# Copyright (C) 2023 MaxLinear, Inc.
#
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk

PKG_NAME:=sck-fw-update
PKG_VERSION:=0.2.1
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=GPL-2.0-only or BSD-2-Clause
PKG_MIRROR_HASH:=skip

define Package/$(PKG_NAME)
  SECTION:=application
  CATEGORY:=MaxLinear
  SUBMENU:=PON
  TITLE:=MxL862xx Firmware Update
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear
endef

define Package/$(PKG_NAME)/description
	SwanCreek Firmware Update
endef

TARGET_CFLAGS += -I$(LINUX_DIR)/include

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -D VERSION=\"$(PKG_VERSION)\" -o $(PKG_BUILD_DIR)/main.o -c $(PKG_BUILD_DIR)/main.c
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(PKG_BUILD_DIR)/lif_api.o -c $(PKG_BUILD_DIR)/lif_api.c
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(PKG_BUILD_DIR)/sck_fw_dwnld.o -c $(PKG_BUILD_DIR)/sck_fw_dwnld.c
	$(TARGET_CC) $(TARGET_LDFLAGS) -o $(PKG_BUILD_DIR)/$1 $(PKG_BUILD_DIR)/main.o $(PKG_BUILD_DIR)/lif_api.o $(PKG_BUILD_DIR)/sck_fw_dwnld.o
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/bin
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
