#
# Copyright © 2020-2024 MaxLinear, Inc.
# Copyright (C) 2017 Intel Corporation
#
# Makefile for sys_cli
#
#
#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ###############################
PKG_NAME:=system_service_cli
PKG_SOURCE_VERSION:=5.0.1.15
PKG_VERSION:=$(PKG_SOURCE_VERSION)
PKG_SOURCE_PROTO:=git
PKG_SOURCE_NAME:=system_service
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_PROJECT:=maxlinear
PKG_SOURCE_INNERDIR:=eth_sys_cli_src
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=c73a8a82f1046a7a1a3fb609a23c792f5247d2f8e827861fa507222ba350f5bc

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)

PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=MaxLinear
PKG_MXL_LICENSE:=MaxLinear/LICENSE

include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk
-include $(STAGING_DIR)/mk/ugw-loglevel.mk


define Package/$(PKG_NAME)
  SECTION:=libs
  CATEGORY:=MaxLinear
  SUBMENU:=UGW Software
  TITLE:=System Configuration FAPI
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
  DEPENDS:= +system_fapi +libscapi +libsafec3-7
  MENU:=1
endef

define Package/$(PKG_NAME)/description
	CLI for system/eth fapi
endef

ifeq ($(CONFIG_USE_MUSL),y)
  TARGET_CFLAGS += -DCONFIG_USE_MUSL
endif

ifneq ($(CONFIG_PACKAGE_kmod-ppa-drv),)
  TARGET_CFLAGS += -DCONFIG_PPA
endif

### Menuconfig ##################################
define Package/$(PKG_NAME)/config
$(call Package/$(PKG_NAME)/override_version)
$(call Package/$(PKG_NAME)/override_source_path)
$(call Package/$(PKG_NAME)/config_debug_level)
endef

TARGET_CFLAGS += -I$(LINUX_DIR)/user_headers/include/net/ \
				 -I$(LINUX_DIR)/user_headers/include/ -I$(LINUX_DIR)/include/ \
				 -I$(STAGING_DIR)/usr/include/safeclib
		
ifeq ($(CONFIG_TARGET_lantiq_xrx200),y)
   TARGET_CFLAGS +=-DPLATFORM_XRX200
   export PLATFORM_XRX200=1 
endif
ifeq ($(CONFIG_TARGET_lantiq_xrx330),y)
   TARGET_CFLAGS +=-DPLATFORM_XRX330
   export PLATFORM_XRX330=1 
endif

ifeq ($(CONFIG_TARGET_lantiq_xrx500),y)
  TARGET_CFLAGS +=-DPLATFORM_XRX500
endif

ifeq ($(CONFIG_TARGET_intel_x86_lgm),y)
  TARGET_CFLAGS +=-DPLATFORM_LGM
  export PLATFORM_LGM=1
endif

ifeq ($(CONFIG_TARGET_x86_puma_puma7),y)
   TARGET_CFLAGS +=-DPLATFORM_XRX750
   export PLATFORM_XRX750=1 
endif

ifneq ($(CONFIG_PACKAGE_kmod-ipv4-ipv6-counter-support), )
   TARGET_CFLAGS +=-DIPV4_IPV6_COUNTER_SUPPORT
   export IPV4_IPV6_COUNTER_SUPPORT=1
endif

#### Target Rules ###############################
define Build/Prepare
	$(call Build/Prepare/Default)
endef

# To ipkg / Rootfs
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sys_cli $(1)/$(VENDOR_PATH)/usr/sbin/
endef

define Build/Clean
	if [ -d $(PKG_BUILD_DIR) ]; then $(MAKE) -C $(PKG_BUILD_DIR) clean; \
	$(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); fi
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
