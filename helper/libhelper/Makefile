#
# Copyright © 2020-2023 MaxLinear, Inc.
# Copyright (C) 2017-2018 Intel Corporation
#
# Makefile for libhelper
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ############################### 
PKG_NAME:=libhelper
PKG_SOURCE_VERSION:=5.1.1.11
PKG_SOURCE_PROTO:=git
PKG_SOURCE_NAME:=libhelper
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_PROJECT:=maxlinear
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=7121be345e694ee3c6e27cab6c917762b0a7beb13868e688e675f81bbceb1b61

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
  TITLE:=UGW SDK Helper library
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
  DEPENDS:=+safeclibs3
  MENU:=1
endef

define Package/$(PKG_NAME)/description
	UGW SDK Helper Library to define function,prototypes,structs,enums which are
	used for list manipulation and debug purposes.
endef

### Menuconfig ##################################
define Package/$(PKG_NAME)/config
$(call Package/$(PKG_NAME)/override_version)
$(call Package/$(PKG_NAME)/override_source_path)
$(call Package/$(PKG_NAME)/config_debug_level)
$(call config_add_global_debug_level)
	config $(PKG_NAME)_MEM_DEBUG
	depends on PACKAGE_$(PKG_NAME)
		bool "Memory Debug"
		help
		  Memory Debug
endef

TARGET_CFLAGS += $(if $(CONFIG_libugwhelper_MEM_DEBUG),-DMEM_DEBUG)
TARGET_CFLAGS += -DHAVE_C99=1

#### Target Rules ###############################
define Build/Prepare
	$(call Build/Prepare/Default)
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include
	touch $(STAGING_DIR)/usr/include/vendor.h
	echo "#define VENDOR_PATH \"$(VENDOR_PATH)\"" > $(STAGING_DIR)/usr/include/vendor.h
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/docs
	$(INSTALL_DIR) $(STAGING_DIR)/mk/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/include/*.h $(1)/usr/include/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/include/*.h $(1)/usr/docs/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libhelper.so $(1)/usr/lib/
	$(INSTALL_DATA) ./files/ugw-loglevel.mk $(STAGING_DIR)/mk/
	$(RM) $(1)/usr/docs/list.h
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libhelper.so $(1)/$(VENDOR_PATH)/usr/lib/
endef

define Build/Clean
	if [ -d $(PKG_BUILD_DIR) ]; then $(MAKE) -C $(PKG_BUILD_DIR) clean; \
	$(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); fi
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
