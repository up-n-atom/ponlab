#
# Openwrt Makefile for swpal_6x-uci
#
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ############################### 
PKG_NAME:=swpal_6x-uci
PKG_REV:=068f92ebec2a0953e05ad09fea01b619cd9396a5
PKG_VERSION:=1.0.0.1
PKG_RELEASE:=$(PKG_REV)
PKG_BASE_NAME:=$(PKG_NAME)
PKG_SOURCE:=$(PKG_BASE_NAME)-$(PKG_VERSION).tar.bz2
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=swpal.git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME)
PKG_SOURCE_VERSION:=r597+a+41+a+17
PKG_SOURCE_SUBDIR:=$(PKG_BASE_NAME)-$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
PKG_MIRROR_HASH:=249b44bc04dd7e029fffec346c2235d18b279a4f00b6d3764263fe86bb925a1f
#PKG_MD5SUM:=40b89c61036add0c2dd1fc10767d3b5f

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(BUILD_VARIANT)/$(PKG_BASE_NAME)-$(PKG_VERSION)

NETIFD_VERSION_DIR:=2022-02-20
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=GPLv2, LGPL-2.1
PKG_LICENSE_FILES:=LICENSE

ifeq ($(CONFIG_BUILD_SDK),"prplos")
PATCH_DIR:=./patches_updk
else
PATCH_DIR:=./patches
endif

include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk
## don't remove '-' in the next line - this is part of the Makefile syntax ##
-include $(STAGING_DIR)/mk/ugw-loglevel.mk

UGW_VERSION:=$(if $(wildcard $(TOPDIR)/ugw_version),$(shell cat $(TOPDIR)/ugw_version))
ifeq ($(UGW_VERSION),)
 #PRPL
 PKG_DEPENDS:=
 libugwhelper_BUILD_DEPENDS:=
else
 #UGW
 PKG_DEPENDS:= +libugwhelper +libcal +csd +servd
 libugwhelper_BUILD_DEPENDS:=+libugwhelper
endif

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=MaxLinear
  SUBMENU:=Static WLAN Platform abstraction layer - SWPAL
  TITLE:=MaxLinear SWPAL
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
  DEPENDS:= $(PKG_DEPENDS) +uci +libsafec3 +libubus
  MENU:=1
  VARIANT:=release
  PROVIDES:=libspal
endef

define Package/$(PKG_NAME)/description
	MaxLinear SWPAL for WLAN Support
endef

define Package/$(PKG_NAME)-debug
$(call Package/$(PKG_NAME))
  DEPENDS+= +$(PKG_NAME)
  TITLE:=MaxLinear SWPAL DEBUG
  VARIANT:=debug
endef

define Package/$(PKG_NAME)-debug/description
	MaxLinear SWPAL for WLAN DEBUG Support
endef

define Package/$(PKG_NAME)-base
$(call Package/$(PKG_NAME))
  DEPENDS:= +libuci +safeclibs3 +libubus
  VARIANT:=base
endef

define Package/$(PKG_NAME)-base/description
	SWPAL for WLAN Support without UGW Framework
endef

define Package/$(PKG_NAME)-prpl
$(call Package/$(PKG_NAME))
  DEPENDS:= +libuci +safeclibs3 +libubus
  VARIANT:=prpl
endef

define Package/$(PKG_NAME)-prpl/description
	SWPAL for PrplOs WLAN Support
endef

define Package/$(PKG_NAME)-prpl-debug
$(call Package/$(PKG_NAME))
  DEPENDS:= +libuci +safeclibs3 +libubus
  VARIANT:=prpl-debug
endef

define Package/$(PKG_NAME)-prpl-debug/description
	SWPAL for PrplOs WLAN Support with debug information
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

define Package/$(PKG_NAME)-prpl-debug/config
$(call Package/$(PKG_NAME)-prpl-debug/override_version)
$(call Package/$(PKG_NAME)-prpl-debug/override_source_path)
$(call Package/$(PKG_NAME)-prpl-debug/config_debug_level)
endef

ifeq ($(CONFIG_USE_MUSL),y)
  TARGET_CFLAGS += -DCONFIG_USE_MUSL
endif

TARGET_CFLAGS += -DCONFIG_RPCD -DHAVE_C99=1

ifneq ($(BUILD_VARIANT), $(filter prpl prpl-debug, $(BUILD_VARIANT)))
PKG_BUILD_DEPENDS:= $(libugwhelper_BUILD_DEPENDS) uci safec3
IFX_LDFLAGS += -lsafec-3.3
IFX_CFLAGS += -fstack-protector-strong -O2 -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security -Wall -fPIC -Wl,-strip-debug
IFX_LDFLAGS += -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro
endif 

ifeq ($(BUILD_VARIANT), $(filter debug prpl-debug, $(BUILD_VARIANT)))
TARGET_CFLAGS += -DCONFIG_WAVE_DEBUG
define Build/Compile
	$(TARGET_CC) ${TARGET_CFLAGS} ${TARGET_LDFLAGS} $(PKG_BUILD_DIR)/src/lite_nc/nc_lite.c $(PKG_BUILD_DIR)/src/lite_nc/nc_help_lite.c $(PKG_BUILD_DIR)/src/lite_nc/nc_help_lite.h -o $(PKG_BUILD_DIR)/src/lite_nc/lite_nc
	$(call Build/Compile/Default)
endef
endif

ifeq ($(BUILD_VARIANT),base)
TARGET_CFLAGS += -DPACKAGE_ID=\\\"$(PKG_NAME_PREF)$(PKG_NAME)\\\" -DLOGGING_ID=\"$(subst -,_,$(PKG_NAME_PREF)$(PKG_NAME))\" \
        $(if $(call qstrip,$(CONFIG_$(PKG_NAME_PREF)$(PKG_NAME)_DEBUG_LEVEL)),\
                -DLOG_LEVEL=$(CONFIG_$(PKG_NAME_PREF)$(PKG_NAME)_DEBUG_LEVEL),-DLOG_LEVEL=$(CONFIG_FEATURE_UGW_GLOBAL_DEBUG_LEVEL)) \
        $(if $(call qstrip,$(CONFIG_$(PKG_NAME_PREF)$(PKG_NAME)_DEBUG_TYPE)),\
                -DLOG_TYPE=$(CONFIG_$(PKG_NAME_PREF)$(PKG_NAME)_DEBUG_TYPE))
TARGET_LDFLAGS += -lsafec-3.3
endif

ifeq ($(BUILD_VARIANT), $(filter prpl prpl-debug, $(BUILD_VARIANT)))
TARGET_CFLAGS += -DPACKAGE_ID=\\\"$(PKG_NAME_PREF)$(PKG_NAME)\\\" -DLOGGING_ID=\"$(subst -,_,$(PKG_NAME_PREF)$(PKG_NAME))\" \
        $(if $(call qstrip,$(CONFIG_$(PKG_NAME_PREF)$(PKG_NAME)_DEBUG_LEVEL)),\
                -DLOG_LEVEL=$(CONFIG_$(PKG_NAME_PREF)$(PKG_NAME)_DEBUG_LEVEL),-DLOG_LEVEL=$(CONFIG_FEATURE_UGW_GLOBAL_DEBUG_LEVEL)) \
        $(if $(call qstrip,$(CONFIG_$(PKG_NAME_PREF)$(PKG_NAME)_DEBUG_TYPE)),\
                -DLOG_TYPE=$(CONFIG_$(PKG_NAME_PREF)$(PKG_NAME)_DEBUG_TYPE))
TARGET_LDFLAGS += -lsafec-3.3
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
	$(INSTALL_DIR) $(1)/opt/intel/wave/db/11n
	$(INSTALL_DIR) $(1)/opt/intel/wave/db/he
	$(INSTALL_DIR) $(1)/opt/intel/wave/db/he_testbed
	$(INSTALL_DIR) $(1)/opt/intel/wave/db/eht
	$(INSTALL_DIR) $(1)/opt/intel/wave/db/eht_testbed
	$(INSTALL_DIR) $(1)/opt/intel/wave/db/mbo
	$(INSTALL_DIR) $(1)/opt/intel/wave/db/vht
	$(INSTALL_DIR) $(1)/opt/intel/wave/db/wpa2
	$(INSTALL_DIR) $(1)/opt/intel/wave/db/wpa3
	$(INSTALL_DIR) $(1)/opt/intel/wave/db/wps2
	$(INSTALL_DIR) $(1)/opt/intel/wave/db/wps2/case_4_1_2
	$(INSTALL_DIR) $(1)/opt/intel/wave/db/wps2/case_4_3_2
	$(INSTALL_DIR) $(1)/opt/intel/wave/db/d2_pdxp
	$(INSTALL_DIR) $(1)/opt/intel/wave/db/d2_fpga
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_DIR) $(1)/opt/intel/wave/scripts/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libspal.so $(1)/$(VENDOR_PATH)/usr/lib
ifneq ($(BUILD_VARIANT), $(filter prpl prpl-debug, $(BUILD_VARIANT)))
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libspal_utils.so $(1)/$(VENDOR_PATH)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/netifd/utils.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/netifd/6ghz_rnr.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/config/uci.sh $(1)/lib/config/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/sbin/wifi $(1)/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/db/etc/wave/db/meta-factory $(1)/opt/intel/wave/db/
	echo "		option prog 'ugw'" >> $(1)/opt/intel/wave/db/meta-factory
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/db/etc/wave/db/ugw/* $(1)/opt/intel/wave/db/ugw/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/netifd/netifd-wireless.sh $(1)/lib/netifd/
endif

ifeq ($(BUILD_VARIANT), $(filter prpl prpl-debug, $(BUILD_VARIANT)))
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/netifd/netifd-wireless.sh $(1)/lib/netifd/swpal-netifd-wireless.sh
endif
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/netifd/hostapd.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/netifd/wireless/* $(1)/lib/netifd/wireless/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/netifd/sigma-ap.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/netifd/sigma-start.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/netifd/sigmaManagerDaemon.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/netifd/sigma_mbo_daemon.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/netifd/update_rnr.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/netifd/update_rnr_uci.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/netifd/sigmaManagerDaemon_static.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/netifd/terminate_radio_after.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/wifi/mac80211.sh $(1)/lib/wifi/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/wifi/platform_dependent_ugw.sh $(1)/lib/wifi/platform_dependent.sh
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/wifi/platform_dependent_common.sh $(1)/lib/wifi/

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/etc/wave/scripts/load_dut.sh $(1)/opt/intel/wave/scripts/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/etc/wave/scripts/flash_file_saver.sh $(1)/opt/intel/wave/scripts/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/etc/wave/scripts/wave_factory_reset.sh $(1)/opt/intel/wave/scripts/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/etc/wave/scripts/wave_factory_reset_ex.sh $(1)/opt/intel/wave/scripts/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/etc/wave/scripts/wave_complete_recovery_ugw.sh $(1)/opt/intel/wave/scripts/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/etc/wave/scripts/wave_complete_recovery_updk.sh $(1)/opt/intel/wave/scripts/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/etc/wave/scripts/common_utils.sh $(1)/opt/intel/wave/scripts/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/db/etc/wave/db/11n/* $(1)/opt/intel/wave/db/11n
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/db/etc/wave/db/he/* $(1)/opt/intel/wave/db/he
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/db/etc/wave/db/he_testbed/* $(1)/opt/intel/wave/db/he_testbed
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/db/etc/wave/db/eht/* $(1)/opt/intel/wave/db/eht
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/db/etc/wave/db/eht_testbed/* $(1)/opt/intel/wave/db/eht_testbed
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/db/etc/wave/db/mbo/* $(1)/opt/intel/wave/db/mbo
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/db/etc/wave/db/vht/* $(1)/opt/intel/wave/db/vht
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/db/etc/wave/db/wpa2/* $(1)/opt/intel/wave/db/wpa2
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/db/etc/wave/db/wpa3/* $(1)/opt/intel/wave/db/wpa3
	$(CP) -r $(PKG_BUILD_DIR)/files/db/etc/wave/db/wps2/* $(1)/opt/intel/wave/db/wps2
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/db/etc/wave/db/d2_pdxp/* $(1)/opt/intel/wave/db/d2_pdxp
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/db/etc/wave/db/d2_fpga/* $(1)/opt/intel/wave/db/d2_fpga
	echo "		option checksum '`cat $(1)/opt/intel/wave/db/ugw/* | md5sum | cut -f1 -d ' '`'" >> $(1)/opt/intel/wave/db/meta-factory
	chmod +x $(1)/lib/wifi/mac80211.sh
	ln -s /opt/intel/wave/scripts/wave_factory_reset.sh $(1)/sbin/wavFactoryReset
	ln -s /opt/intel/wave/scripts/debug_toolbox.sh $(1)/sbin/wavToolBox

ifeq ($(CONFIG_BUILD_SDK),"prplos")
	ln -s /opt/intel/wave/scripts/wave_complete_recovery_updk.sh $(1)/opt/intel/wave/scripts/wave_complete_recovery.sh
else
	ln -s /opt/intel/wave/scripts/wave_complete_recovery_ugw.sh $(1)/opt/intel/wave/scripts/wave_complete_recovery.sh
endif

ifeq ($(BUILD_VARIANT), $(filter debug prpl-debug, $(BUILD_VARIANT)))
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
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/netifd/debug_infrastructure.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/lib/netifd/debug_post_iw_up.sh $(1)/lib/netifd/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/etc/wave/scripts/debug_toolbox.sh $(1)/opt/intel/wave/scripts/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/files/scripts/$(NETIFD_VERSION_DIR)/etc/wave/scripts/wave_collect_debug.sh $(1)/opt/intel/wave/scripts/
endif
endef

Package/swpal_6x-uci-debug/install = $(Package/swpal_6x-uci/install)
Package/swpal_6x-uci-prpl/install = $(Package/swpal_6x-uci/install)
Package/swpal_6x-uci-prpl-debug/install = $(Package/swpal_6x-uci/install)
Package/swpal_6x-uci-base/install = $(Package/swpal_6x-uci/install)

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libspal.so $(1)/usr/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libspal_utils.so $(1)/usr/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/include/uci_wrapper.h $(1)/usr/include/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/include/spal_logging.h $(1)/usr/include/
endef

define Build/Clean
	if [ -d $(PKG_BUILD_DIR) ]; then $(MAKE) -C $(PKG_BUILD_DIR) clean; \
	$(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); fi
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)-debug))
$(eval $(call BuildPackage,$(PKG_NAME)-prpl))
$(eval $(call BuildPackage,$(PKG_NAME)-prpl-debug))
$(eval $(call BuildPackage,$(PKG_NAME)-base))
