#
# Copyright © 2020-2024 MaxLinear, Inc.
# Copyright (C) 2017 Intel Corporation
#
# Makefile for system_fapi
#
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ###############################
PKG_NAME:=system_fapi
PKG_SOURCE_VERSION:=5.1.2.13
PKG_VERSION:=$(PKG_SOURCE_VERSION)
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=system_service
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_INNERDIR:=fapi_system_src
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=5cec1ba5decdb47c6cf9e5c9311407e19f2376b3db6ceb1508414a65d1d1f98a

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
  SUBMENU:=UGW Functional APIs
  TITLE:=UGW System Configuration FAPI
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
  DEPENDS:= +libugwhelper +servd +libscapi +libsafec3-7 +!TARGET_intel_mips_prx300:kmod-bonding
  MENU:=1
endef

define Package/$(PKG_NAME)/description
	MaxLinear UGW FAPI for System Configuration
endef

#### Exports ####################################
export LINUX_KARCH TARGET_CONFIGURE_OPTS LINUX_DIR PKG_BUILD_DIR

### Menuconfig ##################################
define Package/$(PKG_NAME)/config
$(call Package/$(PKG_NAME)/override_version)
$(call Package/$(PKG_NAME)/override_source_path)
$(call Package/$(PKG_NAME)/config_debug_level)
endef

TARGET_CFLAGS += -I$(LINUX_DIR)/user_headers/include/net/ \
        -I$(LINUX_DIR)/user_headers/include/ \
		-I$(LINUX_DIR)/include/ \
		-I$(STAGING_DIR)/usr/include/safeclib \
        -I$(STAGING_DIR)/usr/include/ \
		-DCONFIG_LTQ_PPA_IPv6_ENABLE \
		-DCONFIG_LTQ_PPA_QOS \
		-DCONFIG_LTQ_PPA_QOS_WFQ \
		-DCONFIG_LTQ_PPA_QOS_RATE_SHAPING \
		-DCONFIG_LTQ_PPA_HAL_SELECTOR \
		-DCONFIG_LTQ_PPA_API_SW_FASTPATH

ifeq ($(CONFIG_PACKAGE_kmod-lantiq-swaccel),y)
  TARGET_CFLAGS += -DCONFIG_LTQ_PPA_API_SW_FASTPATH
endif

ifeq ($(CONFIG_PACKAGE_kmod-toe_drv),y)
  TARGET_CFLAGS += -DCONFIG_INTEL_TRAFFIC_OFFLOAD_ENGINE
endif

ifeq ($(CONFIG_PACKAGE_kmod-directconnect_dp),y)
  TARGET_CFLAGS += -DCONFIG_DC_DATAPATH_FRAMEWORK
endif

ifeq ($(CONFIG_PACKAGE_switch_cli),y)
  TARGET_CFLAGS +=-DCONFIG_LANTIQ_SWITCH
endif

ifeq ($(CONFIG_PACKAGE_switch_cli_ugw8),y)
  TARGET_CFLAGS +=-DCONFIG_LANTIQ_SWITCH
endif

ifeq ($(CONFIG_TARGET_lantiq_xrx200),y)
   TARGET_CFLAGS +=-DPLATFORM_XRX200
   export PLATFORM_XRX200=1 
endif

ifeq ($(strip $(CONFIG_TARGET_lantiq_xrx200_EASY220W2)),y)
        TARGET_CFLAGS +=-DPLATFORM_XRX200_EASY220W2
endif

ifeq ($(CONFIG_TARGET_lantiq_xrx330),y)
   TARGET_CFLAGS +=-DPLATFORM_XRX330
   export PLATFORM_XRX330=1 

ifeq ($(strip $(CONFIG_TARGET_lantiq_xrx330_EASY300_AC1200)),y)
       TARGET_CFLAGS +=-DPLATFORM_XRX330_EASY300_AC1200
endif

ifeq ($(strip $(CONFIG_TARGET_lantiq_xrx330_EASY330_AC1200_35B)),y)
       TARGET_CFLAGS +=-DPLATFORM_XRX330_EASY300_AC1200
endif

ifeq ($(strip $(CONFIG_TARGET_lantiq_xrx330_EASY330_RT)),y)
       TARGET_CFLAGS +=-DPLATFORM_XRX330_EASY330_RT
endif
endif

ifeq ($(CONFIG_TARGET_lantiq_xrx300),y)
        TARGET_CFLAGS +=-DPLATFORM_XRX300
endif

ifeq ($(CONFIG_TARGET_lantiq_xrx500),y)
  TARGET_CFLAGS +=-DCONFIG_LTQ_PPA_HAL_SELECTOR -DCONFIG_LTQ_PPA_GRX500 -DCONFIG_LTQ_TARGET_GRX500 -DPLATFORM_XRX500
   export PLATFORM_XRX500=1 
endif
		
ifeq ($(CONFIG_TARGET_intel_mips_xrx500),y)
  TARGET_CFLAGS +=-DCONFIG_LTQ_PPA_HAL_SELECTOR -DCONFIG_LTQ_PPA_GRX500 -DCONFIG_LTQ_TARGET_GRX500 -DPLATFORM_XRX500
   export PLATFORM_XRX500=1
endif

ifeq ($(CONFIG_TARGET_intel_xway_xrx500),y)
  TARGET_CFLAGS +=-DCONFIG_LTQ_PPA_HAL_SELECTOR -DCONFIG_LTQ_PPA_GRX500 -DCONFIG_LTQ_TARGET_GRX500 -DPLATFORM_XRX500
   export PLATFORM_XRX500=1 
endif

ifeq ($(CONFIG_TARGET_intel_xway_xrx500_4kec),y)
  TARGET_CFLAGS +=-DCONFIG_LTQ_PPA_HAL_SELECTOR -DCONFIG_LTQ_PPA_GRX500 -DCONFIG_LTQ_TARGET_GRX500 -DPLATFORM_XRX500
   export PLATFORM_XRX500=1 
endif

ifeq ($(CONFIG_TARGET_intel_x86_grx750),y)
  TARGET_CFLAGS += -DPLATFORM_XRX750
  export PLATFORM_XRX750=1
endif

ifeq ($(CONFIG_TARGET_intel_x86_lgm),y)
  TARGET_CFLAGS +=-DCONFIG_LTQ_PPA_HAL_SELECTOR -DCONFIG_LTQ_PPA_GRX500 -DCONFIG_LTQ_TARGET_GRX500 -DPLATFORM_XRX500 -DPLATFORM_LGM
   export PLATFORM_XRX500=1
   export PLATFORM_LGM=1
endif

ifeq ($(CONFIG_TARGET_intel_mips_prx300),y)
   TARGET_CFLAGS +=-DPLATFORM_XRX500 -DPLATFORM_PON
   export PLATFORM_XRX500=1
endif

ifeq ($(CONFIG_PACKAGE_kmod-lan_port_separation),y)
TARGET_CFLAGS +=-DENABLE_LAN_PORT_SEPARATION
endif

ifeq ($(CONFIG_PACKAGE_kmod-gphy_event),y)
TARGET_CFLAGS +=-DENABLE_LAN_PORT_LINK_EVENT
endif

ifeq ($(CONFIG_PACKAGE_kmod-vrx518-dp-mod),y)
  TARGET_CFLAGS += -DCONFIG_VRX518_SUPPORT
endif

ifeq ($(CONFIG_PACKAGE_ltq_vrx320_datapath_drv),y)
  TARGET_CFLAGS +=-DCONFIG_VRX320_SUPPORT
endif
	
ifeq ($(CONFIG_VRX320_PTM_VECTORING),y)
  TARGET_CFLAGS +=-DCONFIG_VRX320_PTM_VECTORING_SUPPORT
endif
	
ifeq ($(CONFIG_FEATURE_SESSION_LIMIT),y) 
  TARGET_CFLAGS +=-DCONFIG_LTQ_PPA_HANDLE_CONNTRACK_SESSIONS 
endif

ifeq ($(CONFIG_PACKAGE_kmod-pp_crypto_drv),y)
	EXTRA_CFLAGS += -DCONFIG_IPSEC_SUPPORT
endif

ifeq ($(CONFIG_FEATURE_WWAN_LTE_SUPPORT),y)
	EXTRA_CFLAGS += -DCONFIG_WWAN_LTE_SUPPORT
endif

ifeq ($(CONFIG_PACKAGE_kmod-l2nat),y)
	EXTRA_CFLAGS += -DCONFIG_L2NAT_SUPPORT
endif

ifeq ($(CONFIG_USE_MUSL),y)
  TARGET_CFLAGS += -DCONFIG_USE_MUSL
endif

ifneq ($(CONFIG_PACKAGE_kmod-ppa-drv),)
  TARGET_CFLAGS += -DCONFIG_PPA
endif

TARGET_LDFLAGS += -L$(STAGING_DIR)/usr/lib/ppa_api/ \
		-L$(LINUX_DIR)/user_headers/include/net/

#### Target Rules ###############################
# To Staging Dir
define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/docs/
	$(CP) $(PKG_BUILD_DIR)/include/fapi_processorstat.h $(1)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/include/fapi_sys_common.h $(1)/usr/include/
ifeq ($(CONFIG_TARGET_lantiq_xrx200),y)
	$(CP) $(PKG_BUILD_DIR)/xRX220/xRX220_callback.h $(1)/usr/include/
endif
ifeq ($(CONFIG_TARGET_lantiq_xrx500),y)
	$(CP) $(PKG_BUILD_DIR)/xRX350/xRX350_callback.h $(1)/usr/include/
endif
ifeq ($(CONFIG_TARGET_intel_xway_xrx500),y)
	$(CP) $(PKG_BUILD_DIR)/xRX350/xRX350_callback.h $(1)/usr/include/
endif
ifeq ($(CONFIG_TARGET_intel_xway_xrx500_4kec),y)
	$(CP) $(PKG_BUILD_DIR)/xRX350/xRX350_callback.h $(1)/usr/include/
endif
ifeq ($(CONFIG_TARGET_lantiq_xrx330),y)
	$(CP) $(PKG_BUILD_DIR)/xRX330/xRX330_callback.h $(1)/usr/include/
endif
	$(CP) $(PKG_BUILD_DIR)/include/fapi_led.h $(1)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/include/fapi_sys_common.h $(1)/usr/docs/
	$(CP) $(PKG_BUILD_DIR)/include/fapi_led.h $(1)/usr/docs/
	$(CP) $(PKG_BUILD_DIR)/*.so $(1)/usr/lib/
endef

# To ipkg / Rootfs
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/usr/lib
ifeq ($(CONFIG_TARGET_lantiq_xrx200),y)
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/etc/
endif
ifeq ($(CONFIG_TARGET_lantiq_xrx330),y)
	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/etc/
endif
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libsysfapi.so $(1)/$(VENDOR_PATH)/usr/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libethfapi.so $(1)/$(VENDOR_PATH)/usr/lib/
ifneq ($(CONFIG_LANTIQ_OPENWRT_FIRMWARE),y)
ifeq ($(CONFIG_TARGET_lantiq_xrx200),y)
	$(CP) ./files/etc/* $(1)/$(VENDOR_PATH)/etc/
endif
ifeq ($(CONFIG_TARGET_lantiq_xrx330),y)
	$(CP) ./files/etc/* $(1)/$(VENDOR_PATH)/etc/
endif
endif
endef


define Build/Clean
	if [ -d $(PKG_BUILD_DIR) ]; then $(MAKE) -C $(PKG_BUILD_DIR) clean; \
	$(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); fi
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
