#
# Copyright (C) 2009 OpenWrt.org
# Copyright (C) 2017 Intel Corporation
# Copyright © 2020-2024 MaxLinear Inc
#
# Openwrt Makefile for upgrade
#
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ############################### 
PKG_NAME:=upgrade_utils
PKG_SOURCE_VERSION:=2.0.0.18
PKG_SOURCE_PROTO:=git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=upgrade_utils
PKG_SOURCE_INNERDIR:=upgrade
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=7e7c0401f99fab3785eb5a104e3b7d1a4812ef779e217159d12c154561236172

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)

PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=GPL-2.0
PKG_MXL_LICENSE:=GPL/LICENSE

include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk
-include $(STAGING_DIR)/mk/ugw-loglevel.mk

OPENWRT_VERSION:=$(shell cat $(TOPDIR)/version | grep 22.03)

define Package/$(PKG_NAME)
  SECTION:=libs
  CATEGORY:=MaxLinear
  SUBMENU:=UGW Utilities
  TITLE:=UGW utility to support cmd options.
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
  DEPENDS:=@!PACKAGE_sse_fapi +libsafec3-7
  MENU:=1
endef

define Package/$(PKG_NAME)/description
	This package contains upgrade utilities for MaxLinear modules.
endef

### Menuconfig ##################################
define Package/$(PKG_NAME)/config
$(call Package/$(PKG_NAME)/override_version)
$(call Package/$(PKG_NAME)/override_source_path)
	menu "$(PKG_NAME) features"
	depends on PACKAGE_$(PKG_NAME)
	source "$(SOURCE)/config/Config.in"
	endmenu
endef

TARGET_CFLAGS += -DHAVE_C99=1 \
	-DBUILD_FROM_IFX_UTILITIES -DBUILD_FROM_LTQ_APPS \
	-DCONFIG_BOOT_FROM_NAND -DCONFIG_TARGET_UBI_MTD_SUPPORT -DCONFIG_UBOOT_CONFIG_DUAL_IMAGE -DCONFIG_UBOOT_CONFIG_ENV_REDUND \
	-DCONFIG_UBOOT_CONFIG_ENV_SIZE=0x4000 \
	-DCONFIG_UBOOT_CONFIG_FIRMWARE_IN_ROOTFS=1 -DCONFIG_FIRMWARE_IN_ROOTFS \
	-DCONFIG_UBOOT_CONFIG_OVERLAY \
	-DCONFIG_FEATURE_LQ_NEW_UPGRADE -DCONFIG_CMD_UBI \
	-DBUILD_FOR_KERNEL -DIFX_IMAGE_EXTRA_CHECKS \
	$(if $(CONFIG_TARGET_intel_mips),-DARCH_MIPS) \
	-DCONFIG_GCC_VERSION=\\\"$(CONFIG_GCC_VERSION)\\\" \
	-DCONFIG_LIBC_VERSION=\\\"$(CONFIG_LIBC_VERSION)\\\"

TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include/safeclib

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/usr/sbin
	$(INSTALL_DIR) $(1)/usr/share/capabilities/
	$(INSTALL_DIR) $(1)/usr/share/acl.d/
	
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/read_img $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/uboot_env $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/upgrade $(1)/usr/sbin/
	$(INSTALL_DATA) ./files/upgrade_cap.json $(1)/usr/share/capabilities/
	$(if $(OPENWRT_VERSION), \
		$(INSTALL_DATA) ./files/upgrade_cap_owrt_22x.json $(1)/usr/share/capabilities/upgrade_cap.json
	)
	$(INSTALL_DATA) ./files/upgrade_acl.json $(1)/usr/share/acl.d/

	$(INSTALL_BIN) ./files/vol_mgmt $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/secure_upgrade.sh $(1)/sbin/
endef

define Build/Clean
	if [ -d $(PKG_BUILD_DIR) ]; then $(MAKE) -C $(PKG_BUILD_DIR) clean; \
	$(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); fi
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
