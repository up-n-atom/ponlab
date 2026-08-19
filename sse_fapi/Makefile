#
# Copyright (C) 2009 OpenWrt.org
# Copyright (C) 2018 Intel Corporation
# Copyright (C) 2017-2018 Intel Corporation
#
# Openwrt Makefile for secure fapi
#
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ###############################
PKG_NAME:=sse_fapi
PKG_SOURCE_VERSION:=2.0.1.14
PKG_SOURCE_PROTO:=git
PKG_SOURCE_NAME:=system_service
PKG_SOURCE_INNERDIR:=sse_fapi_src
PKG_SOURCE_URL:=ssh://git@gts-chd.intel.com:29418/sw_ugw/$(PKG_SOURCE_NAME).git

PKG_BUILD_DEPENDS:=mkimgvendor
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)
PKG_MAINTAINER:=Intel
PKG_LICENSE:=Intel

SS_TEST_UTILS:= sse_test_app

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/package-version-override.mk
-include $(STAGING_DIR)/mk/ugw-loglevel.mk

define Package/$(PKG_NAME)
  SECTION:=libs
  CATEGORY:=Intel
  SUBMENU:=UGW Functional APIs
  TITLE:=Secure FAPI for Encrypt and Decrypt APIs.
  URL:=http://www.intel.com
  MAINTAINER:=Intel Corporation
  DEPENDS:=+kmod-sse_driver +libhelper +safeclibs3
endef

define SSTestPkg
define Package/$(PKG_NAME)-$(1)
  SECTION:=utils
  CATEGORY:=Intel
  SUBMENU:=UGW Utilities
  TITLE:=UGW Funtional APIs secure fapi - $(1) Test Utility
  URL:=http://www.intel.com
  MAINTAINER:=Intel Corporation
  DEPENDS:=+sse_fapi +libsafec3
endef

define Package/$(PKG_NAME)-$(1)/description
	UGW Funtional APIs secure fapi - '$(1)' Test Utility
endef

define Package/$(PKG_NAME)-$(1)/install
	$(INSTALL_DIR) $$(1)/$(VENDOR_PATH)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(1) $$(1)/$(VENDOR_PATH)/usr/sbin
endef
endef

define Package/$(PKG_NAME)/description
	Secure FAPI for Encrypt and Decrypt APIs.
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

ifeq ($(CONFIG_EIP123_SUPPORT),y)
TARGET_CFLAGS += -DEIP123_DEF
endif

ifeq ($(CONFIG_EIP123_DEBUG),y)
TARGET_CFLAGS += -DDEBUG
endif

ifeq ($(CONFIG_SECURE_BOOT_AUTH_SUPPORT),y)
TARGET_CFLAGS += -DSECURE_BOOT_AUTH_SUPPORT
endif


ifeq ($(CONFIG_MTD_UBI),y)
TARGET_CFLAGS += -DMAX_FLASH_CNT=$(CONFIG_MTD_UBI_WL_THRESHOLD)
else
TARGET_CFLAGS += -DMAX_FLASH_CNT=10000 #other flash case
endif

TARGET_CFLAGS += -DHAVE_C99=1

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/*.h $(1)/usr/include/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libsecurefapi.so $(1)/usr/lib/
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/usr/lib
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/etc/upgrade/
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/etc/sse/
	$(INSTALL_DIR) $(1)/usr/share/pseudofs_acl/
	$(CP) ./files/upg_state_file $(1)/$(VENDOR_PATH)/etc/upgrade/.upg_state_file
	$(CP) ./files/sse_lock $(1)/$(VENDOR_PATH)/etc/sse/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libsecurefapi.so $(1)/$(VENDOR_PATH)/usr/lib/
	$(INSTALL_DATA) ./files/ss_pseudofs_acl.json $(1)/usr/share/pseudofs_acl/
endef

define Build/Clean
	if [ -d $(PKG_BUILD_DIR) ]; then $(MAKE) -C $(PKG_BUILD_DIR) clean; \
	$(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); fi
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
$(foreach testpkg,$(SS_TEST_UTILS),$(eval $(call SSTestPkg,$(testpkg))))
$(foreach testpkg,$(SS_TEST_UTILS),$(eval $(call BuildPackage,$(PKG_NAME)-$(testpkg))))
