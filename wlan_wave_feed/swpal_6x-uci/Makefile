#
# Openwrt Makefile for swpal_6x-uci
#
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ############################### 
PKG_NAME:=swpal_6x-uci
PKG_REV:=7e9abfb030b4af1817d5e2aad1e99f3d1a74532e
PKG_VERSION:=1.0.0.2
PKG_RELEASE:=1
PKG_SOURCE:=$(PKG_NAME)-$(PKG_REV).tar.gz
PKG_SOURCE_URL:=https://gitlab.com/prpl-foundation/intel/swpal/-/archive/$(PKG_REV)
PKG_HASH:=3b7f214ea11e3b2efcec25f2b190f589aa006c30900fdc3780477a9aa62a20df

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(BUILD_VARIANT)/swpal-$(PKG_REV)

include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk
## don't remove '-' in the next line - this is part of the Makefile syntax ##
-include $(STAGING_DIR)/mk/ugw-loglevel.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Intel
  SUBMENU:=Static WLAN Platform abstraction layer - SWPAL
  TITLE:=Intel SWPAL
  URL:=http://www.intel.com
  MAINTAINER:=Intel Corporation
  DEPENDS:= +libuci +safeclibs3
  MENU:=1
  VARIANT:=release
  PROVIDES:=libspal
endef

define Package/$(PKG_NAME)/description
	Intel SWPAL for WLAN Support
endef

define Package/$(PKG_NAME)-debug
$(call Package/$(PKG_NAME))
  DEPENDS+= +$(PKG_NAME)
  TITLE:=Intel SWPAL DEBUG
  VARIANT:=debug
endef

define Package/$(PKG_NAME)-debug/description
	Intel SWPAL for WLAN DEBUG Support
endef


### Menuconfig ##################################
define Package/$(PKG_NAME)/config
$(call Package/$(PKG_NAME)/override_version)
$(call Package/$(PKG_NAME)/override_source_path)
$(call Package/$(PKG_NAME)/config_debug_level)
endef

define Package/$(PKG_NAME)-debug/config
$(call Package/$(PKG_NAME)-debug/override_version)
$(call Package/$(PKG_NAME)-debug/override_source_path)
$(call Package/$(PKG_NAME)-debug/config_debug_level)
endef

ifeq ($(CONFIG_USE_MUSL),y)
  TARGET_CFLAGS += -DCONFIG_USE_MUSL
endif
  
TARGET_CFLAGS += -DCONFIG_RPCD -DHAVE_C99=1

TARGET_CFLAGS += -DPACKAGE_ID=\\\"$(PKG_NAME_PREF)$(PKG_NAME)\\\" -DLOGGING_ID=\"$(subst -,_,$(PKG_NAME_PREF)$(PKG_NAME))\" \
        $(if $(call qstrip,$(CONFIG_$(PKG_NAME_PREF)$(PKG_NAME)_DEBUG_LEVEL)),\
                -DLOG_LEVEL=$(CONFIG_$(PKG_NAME_PREF)$(PKG_NAME)_DEBUG_LEVEL),-DLOG_LEVEL=$(CONFIG_FEATURE_UGW_GLOBAL_DEBUG_LEVEL)) \
        $(if $(call qstrip,$(CONFIG_$(PKG_NAME_PREF)$(PKG_NAME)_DEBUG_TYPE)),\
                -DLOG_TYPE=$(CONFIG_$(PKG_NAME_PREF)$(PKG_NAME)_DEBUG_TYPE))
TARGET_LDFLAGS += -lsafec-3.3

ifeq ($(BUILD_VARIANT),debug)
TARGET_CFLAGS += -DCONFIG_WAVE_DEBUG
define Build/Compile
	$(TARGET_CC) ${TARGET_CFLAGS} ${TARGET_LDFLAGS} $(PKG_BUILD_DIR)/src/lite_nc/nc_lite.c $(PKG_BUILD_DIR)/src/lite_nc/nc_help_lite.c $(PKG_BUILD_DIR)/src/lite_nc/nc_help_lite.h -o $(PKG_BUILD_DIR)/src/lite_nc/lite_nc
	$(call Build/Compile/Default)
endef
endif

# To ipkg / Rootfs
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/usr/lib
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/lib/wifi
	$(INSTALL_DIR) $(1)/lib/netifd
	$(INSTALL_DIR) $(1)/lib/netifd/wireless
	$(INSTALL_DIR) $(1)/lib/config
	$(INSTALL_DIR) $(1)/opt/intel/wave/db
	$(INSTALL_DIR) $(1)/opt/intel/wave/db/ugw
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_DIR) $(1)/opt/intel/wave/scripts/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libspal.so $(1)/$(VENDOR_PATH)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/lib/netifd/netifd-wireless.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/lib/netifd/hostapd.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/lib/netifd/wireless/* $(1)/lib/netifd/wireless/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/lib/netifd/sigma-ap.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/lib/netifd/sigma-start.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/lib/netifd/sigmaManagerDaemon.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/lib/netifd/sigma_mbo_daemon.sh $(1)/lib/netifd/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/lib/netifd/utils.sh $(1)/lib/netifd/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/lib/netifd/6ghz_rnr.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/lib/netifd/update_rnr.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/lib/netifd/update_rnr_uci.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/lib/netifd/lgm_radio_map $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/lib/netifd/sigmaManagerDaemon_static.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/lib/netifd/terminate_radio_after.sh $(1)/lib/netifd/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/lib/config/uci.sh $(1)/lib/config/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/lib/wifi/mac80211.sh $(1)/lib/wifi/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/lib/wifi/platform_dependent_ugw.sh $(1)/lib/wifi/platform_dependent.sh
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/sbin/wifi $(1)/sbin/

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/etc/wave/scripts/load_dut.sh $(1)/opt/intel/wave/scripts/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/etc/wave/scripts/flash_file_saver.sh $(1)/opt/intel/wave/scripts/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/etc/wave/scripts/wave_factory_reset.sh $(1)/opt/intel/wave/scripts/
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/etc/wave/scripts/common_utils.sh $(1)/opt/intel/wave/scripts/

#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/db/etc/wave/db/meta-factory $(1)/opt/intel/wave/db/
#	echo "		option prog 'ugw'" >> $(1)/opt/intel/wave/db/meta-factory
#	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/db/etc/wave/db/ugw/* $(1)/opt/intel/wave/db/ugw/
	echo "		option checksum '`cat $(1)/opt/intel/wave/db/ugw/* | md5sum | cut -f1 -d ' '`'" >> $(1)/opt/intel/wave/db/meta-factory
	chmod +x $(1)/lib/wifi/mac80211.sh
	ln -s /opt/intel/wave/scripts/wave_factory_reset.sh $(1)/sbin/wavFactoryReset
	ln -s /opt/intel/wave/scripts/debug_toolbox.sh $(1)/sbin/wavToolBox
endef

define Package/$(PKG_NAME)-debug/install
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/usr/lib
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/lib/wifi
	$(INSTALL_DIR) $(1)/lib/netifd
	$(INSTALL_DIR) $(1)/lib/netifd/wireless
	$(INSTALL_DIR) $(1)/lib/config
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_DIR) $(1)/opt/intel/wave/scripts/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/lite_nc/lite_nc $(1)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/lib/netifd/debug_infrastructure.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/lib/netifd/debug_post_iw_up.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/etc/wave/scripts/debug_toolbox.sh $(1)/opt/intel/wave/scripts/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/etc/wave/scripts/wave_collect_debug.sh $(1)/opt/intel/wave/scripts/
	ln -s /opt/intel/wave/scripts/debug_toolbox.sh $(1)/sbin/wavToolBox
	ln -s /opt/intel/wave/scripts/wave_factory_reset.sh $(1)/sbin/wavFactoryReset
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libspal.so $(1)/usr/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/include/uci_wrapper.h $(1)/usr/include/
endef

define Build/Clean
	if [ -d $(PKG_BUILD_DIR) ]; then $(MAKE) -C $(PKG_BUILD_DIR) clean; \
	$(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); fi
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)-debug))
