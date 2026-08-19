#
# Copyright (C) 2020-2024 MaxLinear, Inc.
# Copyright (C) 2017-2018 Intel Corporation
#
# Makefile for secure fapi
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ###############################
PKG_NAME:=sse_fapi_lgm
PKG_SOURCE_VERSION:=5.9.2.83
PKG_VERSION:=$(PKG_SOURCE_VERSION)
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=sse_fapi
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=460e0706edc26fd254adac0c5c4621de1079cc0837d0e52009c972313d9bc283

PKG_BUILD_DEPENDS:=mkimgvendor
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=BSD
PKG_MXL_LICENSE:=BSD/LICENSE

PKG_CONFIG_DEPENDS += CONFIG_INTEL_X86_ENV_SZ
TARGET_CFLAGS += -DCONFIG_SYS_REDUNDAND_ENVIRONMENT=1
TARGET_CFLAGS += -DCONFIG_UBOOT_CONFIG_ENV_SIZE=$(CONFIG_INTEL_X86_ENV_SZ)
TARGET_CFLAGS += -DPLATFORM_LGM

include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk
-include $(STAGING_DIR)/mk/ugw-loglevel.mk

define Package/$(PKG_NAME)
  SECTION:=libs
  CATEGORY:=MaxLinear
  SUBMENU:=UGW Functional APIs
  TITLE:=Secure FAPI for Encrypt and Decrypt APIs for LGM.
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
  DEPENDS:=+INTEL_X86_SECBOOT:kmod-sse_driver_lgm +libhelper +libsafec3-7 +libfdt +libscapi +libopenssl @TARGET_intel_x86_lgm
endef

define SSTestPkg
define Package/$(PKG_NAME)-$(1)
  SECTION:=utils
  CATEGORY:=MaxLinear
  SUBMENU:=UGW Utilities
  TITLE:=UGW Funtional APIs secure fapi - $(1) Test Utility
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
  DEPENDS:=+sse_fapi_lgm +libsafec3-7 +libfdt +libscapi @INTEL_X86_SECBOOT
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
	Secure FAPI for Encrypt and Decrypt APIs for LGM.
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

ifeq ($(CONFIG_LGM_EIP123_SUPPORT),y)
TARGET_CFLAGS += -DEIP123_DEF
endif

ifeq ($(CONFIG_LGM_EIP123_DEBUG),y)
TARGET_CFLAGS += -DDEBUG
endif

ifeq ($(CONFIG_LGM_SECURE_BOOT_AUTH_SUPPORT),y)
TARGET_CFLAGS += -DSECURE_BOOT_AUTH_SUPPORT
endif

SS_TEST_UTILS:= sse_test_app sse_ctest_util secdbg_ctest

ifeq ($(CONFIG_INTEL_X86_SECBOOT),y)
TARGET_CFLAGS += -DLGM_SECURE
TARGET_CFLAGS += -DIMG_AUTH
export LGM_SECURE=1
endif

ifeq ($(CONFIG_MTD_UBI),y)
TARGET_CFLAGS += -DMAX_FLASH_CNT=$(CONFIG_MTD_UBI_WL_THRESHOLD)
else
TARGET_CFLAGS += -DMAX_FLASH_CNT=10000 #other flash case
endif

TARGET_CFLAGS += -DHAVE_C99=1
TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include/safeclib

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/include/*.h $(1)/usr/include/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libsecurefapi.so $(1)/usr/lib/
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/usr/lib
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/etc/upgrade/
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/etc/sse/
	$(INSTALL_DIR) $(1)/usr/share/pseudofs_acl/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/usr/sbin
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
