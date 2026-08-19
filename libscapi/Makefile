#
# Copyright (C) 2020-2023 MaxLinear, Inc.
# Copyright (C) 2017-2018 Intel Corporation
#
# Makefile for libscapi
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ############################### 
PKG_NAME:=libscapi
PKG_SOURCE_VERSION:=5.4.1.37
PKG_VERSION:=$(PKG_SOURCE_VERSION)
PKG_SOURCE_PROTO:=git
PKG_SOURCE_NAME:=libscapi
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_PROJECT:=maxlinear
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=2a1828e83796f26b37e3b07c68bb7918f098f52bd55e6e37c3f0668952e33ebb

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=LGPL2.1
PKG_MXL_LICENSE:=LGPL2.1/LICENSE

include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk
-include $(STAGING_DIR)/mk/ugw-loglevel.mk

define Package/$(PKG_NAME)
  SECTION:=lib
  CATEGORY:=MaxLinear
  SUBMENU:=UGW Framework
  TITLE:=UGW Framework System Configuration APIs
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
  DEPENDS:= +libhelper +safeclibs3
  MENU:=1
endef

define Package/$(PKG_NAME)/description
	UGW Framework System Configuration APIs
endef


### Menuconfig ##################################
define Package/$(PKG_NAME)/config
$(call Package/$(PKG_NAME)/override_version)
$(call Package/$(PKG_NAME)/override_source_path)
$(call Package/$(PKG_NAME)/config_debug_level)
	menu "$(PKG_NAME) features"
	depends on PACKAGE_$(PKG_NAME)
        source "$(SOURCE)/config/Config.in"
        endmenu
endef
#export $(PKG_CONFIG_DEPENDS)

ifeq ($(CONFIG_USE_MUSL),y)
  TARGET_CFLAGS += -DCONFIG_USE_MUSL -DHAVE_C99=1
endif

#### Target Rules ###############################
define Build/Prepare
	$(call Build/Prepare/Default)
	$(if $(CONFIG_$(PKG_NAME)_USE_CUSTOM_SOURCE_DIR),,./files/interface_config.sh $(TOPDIR)/.config $(PKG_BUILD_DIR)/interfaces.cfg)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/*.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/include/*.h $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/docs
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/include/scapi_proto.h $(1)/usr/docs/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/include/ugw_error.h $(1)/usr/docs/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/include/ulogging.h $(1)/usr/docs/
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/utils/scapiutil $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/utils/try_reboot $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/etc/
	$(if $(CONFIG_$(PKG_NAME)_USE_CUSTOM_SOURCE_DIR),,\
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/interfaces.cfg $(1)/$(VENDOR_PATH)/etc/; \
	)
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/interfaces.cfg $(1)/$(VENDOR_PATH)/etc/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/*.so $(1)/usr/lib
	$(if $(CONFIG_TARGET_x86), \
		$(INSTALL_DIR) $(1)/lib/preinit; \
		$(INSTALL_BIN) ./files/scapi_utils.sh $(1)/lib/preinit/; \
	,\
		$(INSTALL_BIN) ./files/scapi_utils.sh $(1)/usr/sbin/; \
		$(SED) 's/.*boot_hook_add.*preinit.*/scapi_utils_init/g' $(1)/usr/sbin/scapi_utils.sh; \
	)
endef

define Build/Clean
	if [ -d $(PKG_BUILD_DIR) ]; then $(MAKE) -C $(PKG_BUILD_DIR) clean; \
	$(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); fi
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
