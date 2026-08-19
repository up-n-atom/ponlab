#
# Copyright © 2020-2024 MaxLinear, Inc.
# Copyright (C) 2017 Intel Corporation
#
# Makefile for libupgfapi
#
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ###############################
PKG_NAME:=libupgfapi
PKG_SOURCE_VERSION:=5.2.1.9
PKG_VERSION:=$(PKG_SOURCE_VERSION)
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=system_service
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_INNERDIR:=fapi_upgrade_src
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=1a0a57045a5ea6e3ada918d283fbbc8ed359adbe21e52b0892cbbdf8da99dacd

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)

PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=MaxLinear
PKG_MXL_LICENSE:=MaxLinear/LICENSE

include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk
-include $(STAGING_DIR)/mk/ugw-loglevel.mk


ifeq ($(CONFIG_USE_MUSL),y)
  TARGET_CFLAGS += -DCONFIG_USE_MUSL
endif

define Package/$(PKG_NAME)
  SECTION:=libs
  CATEGORY:=MaxLinear
  SUBMENU:=UGW Functional APIs
  TITLE:=UGW System Upgrade FAPI
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
  DEPENDS:= +libscapi +libsafec3-7
  MENU:=1
endef

define Package/$(PKG_NAME)/description
	MaxLinear UGW FAPI for System Upgrade Functionality
endef

### Menuconfig ##################################
define Package/$(PKG_NAME)/config
$(call Package/$(PKG_NAME)/override_version)
$(call Package/$(PKG_NAME)/override_source_path)
$(call Package/$(PKG_NAME)/config_debug_level)
endef

ifeq ($(CONFIG_TARGET_x86_puma),y)
  TARGET_CFLAGS +=-DPLATFORM_INTEL
endif

ifeq ($(CONFIG_TARGET_lantiq_xrx200),y)
  TARGET_CFLAGS +=-DPLATFORM_XRX200
endif

ifeq ($(CONFIG_TARGET_lantiq_xrx330),y)
  TARGET_CFLAGS +=-DPLATFORM_XRX330
endif
ifeq ($(CONFIG_TARGET_lantiq_xrx300),y)
  TARGET_CFLAGS +=-DPLATFORM_XRX300
endif

ifeq ($(CONFIG_TARGET_lantiq_xrx500),y)
  TARGET_CFLAGS +=-DPLATFORM_XRX500
endif

ifeq ($(CONFIG_TARGET_intel_x86_lgm),y)
  TARGET_CFLAGS +=-DPLATFORM_XRX500 -DPLATFORM_LGM
endif
TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include/safeclib

#### Target Rules ###############################

# To Staging Dir
define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/include/fapi_upg.h $(1)/usr/include/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libupgfapi.so $(1)/usr/lib/
endef

# To ipkg / Rootfs
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libupgfapi.so $(1)/$(VENDOR_PATH)/usr/lib/
endef

define Build/Clean
	if [ -d $(PKG_BUILD_DIR) ]; then $(MAKE) -C $(PKG_BUILD_DIR) clean; \
	$(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); fi
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
