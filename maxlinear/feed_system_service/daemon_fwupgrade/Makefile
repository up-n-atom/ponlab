#
# Copyright (C) 2020-2024 MaxLinear, Inc.
# Copyright (C) 2017-2018 Intel Corporation
#
# Makefile for fwupgrade daemon
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ###############################
PKG_NAME:=fwupgrade
PKG_SOURCE_VERSION:=5.9.3.8
PKG_VERSION:=$(PKG_SOURCE_VERSION)
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=daemon_fwupgrade
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=e892fa865625d06b082253cc5d79af7c49663c990d95afb14d75e65395578c95

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=BSD
PKG_MXL_LICENSE:=BSD/LICENSE

include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk
-include $(STAGING_DIR)/mk/ugw-loglevel.mk

define Package/$(PKG_NAME)
  SECTION:=libs
  CATEGORY:=MaxLinear
  SUBMENU:=UGW Functional APIs
  TITLE:=fwupgrade daemon package for LGM.
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
  DEPENDS:=+libubox +libubus +libugwhelper +libblobmsg-json +libmsgapi +libhelper +libsafec3-7 +libscapi +sse_fapi_lgm
endef

define Package/$(PKG_NAME)/description
	daemon fwupgrade for LGM.
endef

TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include/safeclib

define Build/InstallDev
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/fwupgrade $(1)/$(VENDOR_PATH)/usr/sbin/
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/fwupgrade $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/fwupgrade $(1)/$(VENDOR_PATH)/usr/sbin/fwupgrade
endef

define Build/Clean
	if [ -d $(PKG_BUILD_DIR) ]; then $(MAKE) -C $(PKG_BUILD_DIR) clean; \
	$(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); fi
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
