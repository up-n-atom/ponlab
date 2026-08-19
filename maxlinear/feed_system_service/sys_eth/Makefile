#
# Copyright © 2020-2024 MaxLinear, Inc.
# Copyright (C) 2017 Intel Corporation
#
# Openwrt Makefile for sys eth 
#
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ###############################

PKG_NAME:=sys_eth
PKG_SOURCE_VERSION:=5.12.1.12
PKG_VERSION:=$(PKG_SOURCE_VERSION)
PKG_SOURCE_PROTO:=git
PKG_SOURCE_NAME:=system_service
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_PROJECT:=maxlinear
PKG_SOURCE_INNERDIR:=sys_eth_src
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=21ac12216365faa641d8795b9ad69384939bedd310e37ca89edd9ace162a28b5

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
  SUBMENU:=UGW Service Libraries
  TITLE:=UGW Ethernet and Switch Configuration utility
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
  DEPENDS:= +libugwhelper +libscapi +system_fapi +libsafec3-7
  MENU:=1
endef

define Package/$(PKG_NAME)/description
	MaxLinear UGW Service Library for Ethernet and Switch Configuration
endef

### Menuconfig ##################################
define Package/$(PKG_NAME)/config
$(call Package/$(PKG_NAME)/override_version)
$(call Package/$(PKG_NAME)/override_source_path)
$(call Package/$(PKG_NAME)/config_debug_level)
endef

TARGET_CFLAGS += -I$(LINUX_DIR)/user_headers/include/net/ \
				 -I$(LINUX_DIR)/user_headers/include/ -I$(LINUX_DIR)/include/ \
				 -I$(STAGING_DIR)/usr/include/safeclib

ifeq ($(CONFIG_PACKAGE_switch_cli),y)
  TARGET_CFLAGS +=-DCONFIG_LANTIQ_SWITCH
endif

ifeq ($(CONFIG_PACKAGE_switch_cli_ugw8),y)
  TARGET_CFLAGS +=-DCONFIG_LANTIQ_SWITCH
endif

ifneq ($(CONFIG_PACKAGE_kmod-ppa-drv),)
  TARGET_CFLAGS += -DCONFIG_PPA
endif

# To ipkg / Rootfs
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sys_eth $(1)/$(VENDOR_PATH)/bin/

	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) files/sys_eth $(1)/etc/init.d/
# sys_link configures the XGMAC for the LAN ports on the GRX350 only, not LGM
ifeq ($(CONFIG_TARGET_intel_mips_xrx500),y)
	$(INSTALL_BIN) files/sys_link $(1)/etc/init.d/
endif

	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_BIN) files/ethernet $(1)/etc/config/

	$(INSTALL_DIR) $(1)/usr/share/capabilities
	$(CP) files/syseth_cap.json $(1)/usr/share/capabilities/
	$(INSTALL_DIR) $(1)/usr/share/acl.d/
	$(INSTALL_DATA) ./files/syseth_acl.json $(1)/usr/share/acl.d/syseth_acl.json
	$(INSTALL_DIR) $(1)/usr/share/pseudofs_acl
	$(INSTALL_DATA) ./files/syseth_pseudofs_acl.json $(1)/usr/share/pseudofs_acl/syseth_pseudofs_acl.json

endef

define Build/Clean
	if [ -d $(PKG_BUILD_DIR) ]; then $(MAKE) -C $(PKG_BUILD_DIR) clean; \
	$(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); fi
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
