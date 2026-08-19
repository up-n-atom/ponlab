#
# Copyright © 2020-2024 MaxLinear, Inc.
# Copyright (C) 2017 Intel Corporation
#
# Makefile for secure storage service server
#
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ############################### 
PKG_NAME:=sse_server
PKG_SOURCE_VERSION:=5.10.1.7
PKG_VERSION:=$(PKG_SOURCE_VERSION)
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=system_service
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_INNERDIR:=sse_server_src
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=97f097a88ce98f968d7c386701ffb5b7f79d53c3dd251fe9251a903fb51dbfe2

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=MaxLinear
PKG_MXL_LICENSE:=MaxLinear/LICENSE
PKG_CONFIG_DEPENDS:=CONFIG_SSE_SERVER_DEBUG

include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=MaxLinear
  SUBMENU:=UGW Service Daemons
  TITLE:=Secure Storage Service Server
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
  DEPENDS:=@(TARGET_intel_mips_xrx500_4kec||TARGET_intel_mips_prx300_4kec) +libsafec3-7 +eip123
endef

define Package/$(PKG_NAME)/description
	Secure Storage Service Server
endef


define Package/$(PKG_NAME)/config
$(call Package/$(PKG_NAME)/override_version)
$(call Package/$(PKG_NAME)/override_source_path)
        menu "$(PKG_NAME) features"
        depends on PACKAGE_$(PKG_NAME)
        source "$(SOURCE)/config/Config.in"
        endmenu
endef

#### Target Rules ###############################

ifeq ($(CONFIG_SSE_OTP_ROLLBACK_ID),y)
TARGET_CFLAGS += -DCONFIG_OTP_ROLLBACK_ID -DCONFIG_UPDATE_OTP_ROLLBACK_ID
ifeq ($(CONFIG_TARGET_intel_mips_prx300_4kec),y)
TARGET_CFLAGS += -DROLLID_BIT_SIZE=64 -DEXTRA_IMAGE_CHECK
else
TARGET_CFLAGS += -DROLLID_BIT_SIZE=32
endif
endif

TARGET_CFLAGS += -I $(STAGING_DIR)/usr/include/ \
				 -I$(LINUX_DIR)/include/ \
				 -I$(LINUX_DIR)/user_headers/include/linux/icc/ \
				 -I$(STAGING_DIR)/usr/include/safeclib
TARGET_CFLAGS += -g -O2 -Wall -DICC_IMAGE -DECDSA_SW -DCONFIG_OBFUSCATION -DCONFIG_ENCRYPT_ATTRIBUTE

ifeq ($(CONFIG_TARGET_intel_mips_prx300_4kec),y)
TARGET_CFLAGS += -DCONFIG_PON_FW_DOWNLOAD
endif

ifeq ($(CONFIG_SSE_SERVER_DEBUG),y)
TARGET_CFLAGS += -DDEBUG
endif

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin	
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) ./files/load_ss.sh $(1)/etc/init.d/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ss_server $(1)/usr/bin/
endef

define Build/Clean
	if [ -d $(PKG_BUILD_DIR) ]; then $(MAKE) -C $(PKG_BUILD_DIR) clean; \
	$(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); fi
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
