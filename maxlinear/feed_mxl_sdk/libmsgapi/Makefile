#
# Copyright © 2020-2024 MaxLinear, Inc.
# Copyright (C) 2017-2018 Intel Corporation
#
# Makefile for libmsgapi
#
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ############################### 
PKG_NAME:=libmsgapi
PKG_SOURCE_VERSION:=5.2.0.8
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=libmsgapi
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=721aee1aba5291f61577f8cf7991f3d11a687f899b5b9f16503489d5f8a85f85

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=LGPL2.1
PKG_MXL_LICENSE:=LGPL2.1/LICENSE

include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk
-include $(STAGING_DIR)/mk/ugw-loglevel.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=MaxLinear
  SUBMENU:=UGW Framework
  TITLE:=UGW SDK Message API
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
  DEPENDS:= +libubox +libubus +libpthread +libscapi +libsafec3-7
  MENU:=1
endef

TARGET_CFLAGS += -DHAVE_C99=1
TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include/safeclib

define Package/$(PKG_NAME)/description
	UGW SDK Message API Library to define function,prototypes,structs,enums which are
	used for non framework components to interact with framework and vice-versa.
endef

### Menuconfig ##################################
define Package/$(PKG_NAME)/config
$(call Package/$(PKG_NAME)/override_version)
$(call Package/$(PKG_NAME)/override_source_path)
$(call Package/$(PKG_NAME)/config_debug_level)
endef


define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/docs
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/*.h $(1)/usr/include/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/*.h $(1)/usr/docs/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/*.so $(1)/usr/lib/
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/usr/lib
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/*.so $(1)/$(VENDOR_PATH)/usr/lib/
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/test/MsgAPI* $(1)/$(VENDOR_PATH)/usr/sbin/
endef

define Build/Clean
	if [ -d $(PKG_BUILD_DIR) ]; then $(MAKE) -C $(PKG_BUILD_DIR) clean; \
	$(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); fi
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
