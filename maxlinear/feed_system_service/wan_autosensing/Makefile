#
# Copyright © 2020-2024 MaxLinear, Inc.
# Copyright (C) 2019 Intel Corporation
#
# Makefile for wanautosensing
#
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ############################### 
PKG_NAME:=wanautosensing
PKG_SOURCE_VERSION:=5.13.1.7
PKG_VERSION:=$(PKG_SOURCE_VERSION)
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=system_service
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_INNERDIR:=wan_autosensing_src
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=30e15d87a3496be15e6b1e46e01c8d87763049b4c061f8b32a6a104e88ac20bc

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)

PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=MaxLinear
PKG_MXL_LICENSE:=MaxLinear/LICENSE

include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk
-include $(STAGING_DIR)/mk/ugw-loglevel.mk

define Package/$(PKG_NAME)
    SECTION:=MaxLinearBaseComponents
    CATEGORY:=MaxLinear
    SUBMENU:=UGW Service Daemons
    TITLE:=WAN autosensing daemon
    URL:=http://www.maxlinear.com
    MAINTAINER:=MaxLinear Inc
    DEPENDS += +libugwhelper +system_fapi +libsysservices +libsafec3-7  +libhelper
endef

define Package/$(PKG_NAME)/description
	Autosensing daemon to change WAN mode among G.Fast, GPON and 2.5G GPHY mode
endef

CONFIGURE_ARGS += \
	--enable-kernelincl="$(LINUX_DIR)/include/net/switch_api/" \

TARGET_CFLAGS += -I$(LINUX_DIR)/user_headers/include/net/ \
				 -I$(LINUX_DIR)/user_headers/include/ \
				 -I$(LINUX_DIR)/include/ \
				 -I$(STAGING_DIR)/usr/include/safeclib \
				 -DPLATFORM_XRX500

ifeq ($(CONFIG_USE_MUSL),y)
  TARGET_CFLAGS += -DCONFIG_USE_MUSL
endif


### Menuconfig ##################################
define Package/$(PKG_NAME)/config
$(call Package/$(PKG_NAME)/override_version)
$(call Package/$(PKG_NAME)/override_source_path)
$(call Package/$(PKG_NAME)/config_debug_level)
        menu "Auto-sensing Platform Select"
                depends on PACKAGE_$(PKG_NAME)
                source "$(SOURCE)/files/Config.in.autosensing"
        endmenu
endef

ifeq ($(CONFIG_TARGET_lantiq_xrx500),y)
  TARGET_CFLAGS +=-DPLATFORM_XRX500
endif

ifeq ($(CONFIG_TARGET_intel_x86_lgm),y)
  TARGET_CFLAGS +=-DPLATFORM_LGM
  export PLATFORM_LGM=1
endif

ifeq ($(CONFIG_PACKAGE_switch_cli),y)
  TARGET_CFLAGS +=-DCONFIG_LANTIQ_SWITCH
endif

ifeq ($(CONFIG_PACKAGE_switch_cli_ugw8),y)
  TARGET_CFLAGS +=-DCONFIG_LANTIQ_SWITCH
endif

ifdef CONFIG_WAN_AUTOSENSING_DSL
  TARGET_CFLAGS += -DPLATFORM_AUTOSENSE_DSL_SUPPORT
endif

ifdef CONFIG_WAN_AUTOSENSING_SFP
  TARGET_CFLAGS += -DPLATFORM_AUTOSENSE_SFP_SUPPORT
endif

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/include/
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/wanautosensing.init $(1)/etc/init.d/wanautosensing
	$(INSTALL_DIR) $(1)/usr/share/capabilities/
	$(INSTALL_BIN) ./files/wanautosensing_capability $(1)/usr/share/capabilities/wasd_cap.json
	$(INSTALL_DIR) $(1)/etc/wanautosensing/
	$(INSTALL_DATA) ./files/def_wan_config  $(1)/etc/wanautosensing/def_wan_config
	$(INSTALL_DIR) $(1)/usr/share/acl.d
	$(INSTALL_BIN) ./files/wanautosensing_acl $(1)/usr/share/acl.d/wasd_acl.json
	$(INSTALL_DIR) $(1)/usr/share/pseudofs_acl
	$(INSTALL_BIN) ./files/wasd_pseudofs_acl $(1)/usr/share/pseudofs_acl/wasd_pseudofs_acl.json
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wanautosensing $(1)/usr/sbin/
endef

define Build/Clean
       if [ -d $(PKG_BUILD_DIR) ]; then $(MAKE) -C $(PKG_BUILD_DIR) clean; \
        $(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); fi
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
