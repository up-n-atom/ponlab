#
# Copyright © 2020-2024 MaxLinear, Inc.
# Copyright (C) 2017-2018 Intel Corporation
#
# Makefile for libugwhelper
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ############################### 
PKG_NAME:=libugwhelper
PKG_SOURCE_VERSION:=5.3.0.18
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=libugwhelper
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=2d886f828868336258bfc6f49884ac417a04968988c1c62df1fca8358ef6f1c4

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
  DEPENDS:=+libc +librt +libpthread +libjson-c +libubox +libubus +libblobmsg-json +libscapi +libhelper +libmsgapi +libsafec3-7
  MENU:=1
endef


define Package/$(PKG_NAME)/description
	UGW SDK Helper Library to define function,prototypes,structs,enums which are
	used across the system. 
endef

TARGET_CFLAGS += -DHAVE_C99=1

### Menuconfig ##################################
define Package/$(PKG_NAME)/config
$(call Package/$(PKG_NAME)/override_version)
$(call Package/$(PKG_NAME)/override_source_path)
$(call Package/$(PKG_NAME)/config_debug_level)
$(call config_add_global_debug_level)
	menu "$(PKG_NAME) features"
	depends on PACKAGE_$(PKG_NAME)
        source "$(SOURCE)/config/Config.in"
        endmenu
endef

TARGET_CFLAGS += $(if $(CONFIG_libugwhelper_MEM_DEBUG),-DMEM_DEBUG)
TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include/safeclib

#### Target Rules ###############################
define Build/Prepare
	$(call Build/Prepare/Default)
	python $(PKG_BUILD_DIR)/CreateNIDString.py $(PKG_BUILD_DIR)/include/ugw_notify_defs.h $(PKG_BUILD_DIR)/ugw_notify.c
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libugwhelper.so $(1)/usr/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/include/*.h $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/docs
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/include/ugw_common.h $(1)/usr/docs/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/include/ugw_debug.h $(1)/usr/docs/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/include/ugw_defs.h $(1)/usr/docs/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/include/ugw_enums.h $(1)/usr/docs/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/include/ugw_proto.h $(1)/usr/docs/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/include/ugw_structs.h $(1)/usr/docs/
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/usr/lib
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/usr/sbin
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/etc/
	$(INSTALL_DIR) $(1)/etc/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libugwhelper.so $(1)/$(VENDOR_PATH)/usr/lib/
	$(INSTALL_BIN) ./files/syslog-ng.conf $(1)/$(VENDOR_PATH)/etc/
	$(INSTALL_BIN) ./files/profile_logs.sh $(1)/$(VENDOR_PATH)/usr/sbin
	./files/gen_notify_defs.sh $(PKG_BUILD_DIR)/include/ugw_notify_defs.h $(1)/$(VENDOR_PATH)/etc/ugw_notify_defs.sh
	./files/gen_notify_defs.sh $(PKG_BUILD_DIR)/include/ugw_notify_defs.h $(1)/etc/ugw_notify_defs.sh
endef

define Build/Clean
	if [ -d $(PKG_BUILD_DIR) ]; then $(MAKE) -C $(PKG_BUILD_DIR) clean; \
	$(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); fi
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
