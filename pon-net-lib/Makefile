#
# Copyright (C) 2020 - 2023 MaxLinear, Inc.
# Copyright (c) 2017 - 2020 Intel Corporation
# Copyright (C) 2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=pon-net-lib
PKG_VERSION:=1.22.2
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=pon_net_lib
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=f493f0e8113c1e14666b4ef885d8b0076ae3cda209eb2e4c55452af6b111f770
PKG_RELEASE:=1
PKG_FIXUP:=autoreconf
PKG_BUILD_PARALLEL:=1
PKG_INSTALL:=1
PKG_LICENSE:=BSD-2-Clause
PKG_LICENSE_FILES:=LICENSE

-include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# avoid build directly in the source directories
PKG_AUTOMAKE_PATHS:=.
CONFIGURE_PATH=build_$(PKGARCH)
CONFIGURE_CMD=../configure
MAKE_PATH=$(CONFIGURE_PATH)
PKG_INSTALL_DIR=$(PKG_BUILD_DIR)/$(CONFIGURE_PATH)/ipkg-install
# update stamps with CONFIGURE_PATH
STAMP_CONFIGURED=$(PKG_BUILD_DIR)/$(CONFIGURE_PATH)/.configured$(if $(DUMP),,_$(call confvar,$(PKG_CONFIG_DEPENDS)))
STAMP_CONFIGURED_WILDCARD=$(PKG_BUILD_DIR)/$(CONFIGURE_PATH)/.configured_*
STAMP_BUILT=$(PKG_BUILD_DIR)/$(CONFIGURE_PATH)/.built

define Package/$(PKG_NAME)
  SECTION:=application
  CATEGORY:=MaxLinear
  TITLE:= PON NET LIBRARY
  URL:=https://www.maxlinear.com
  MAINTAINER:=MaxLinear
  MENU:=1
  SUBMENU:=PON
  DEPENDS:=+pon-libnl-route \
	+pon-adapter \
	+kmod-sched-prio +kmod-sched-flower +kmod-sched-drr \
	+kmod-sched-red \
	+kmod-sched-act-vlan +kmod-sched-act-colmark \
	+kmod-sched-act-police \
	+kmod-qos-tc +MCC_DRV_ENABLE:pon-mcc-lib \
	+libuci +kmod-pon-arp-ndp-reinsert \
	+libsafec3-7
endef

define Package/$(PKG_NAME)/description
	PON NET library
endef

define Package/$(PKG_NAME)/config
	menu "configuration"
	depends on PACKAGE_$(PKG_NAME)
	config PON_NET_LIB_NO_TC_OFFLOAD
		bool "disable TC offload"
		help
			This will select NO_TC_OFFLOAD flag

	config MCC_DRV_ENABLE
		bool "enable MCC driver"
		help
			This will enable MCC drvier usage

	endmenu
	$(call Package/$(PKG_NAME)/override_source_path)
endef

define KernelPackage/pon-arp-ndp-reinsert
  TITLE:=PON multicast packet bridge reinsert
  SUBMENU:=MaxLinear
  DEPENDS:=+kmod-nf-reject
  KCONFIG:=CONFIG_DP_ARP_NDP_REINSERT
  FILES:=$(LINUX_DIR)/drivers/net/datapath/bridge_filter/arp_ndp_reinsert.ko
  # We do not want to load this in all models and do this manually in the pon_net_lib when the LCT was created.
  # AUTOLOAD:=$(call AutoProbe,arp_ndp_reinsert)
endef

PKG_CONFIG_DEPENDS:= \
	CONFIG_PON_NET_LIB_NO_TC_OFFLOAD \
	CONFIG_MCC_DRV_ENABLE

# make the submake silent:
ifeq ($(QUIET),1)
MAKE_FLAGS+=-s
endif
ifneq (,$(findstring 99,$(V)))
MAKE_FLAGS+=-s
endif

CONFIGURE_ARGS += \
	--enable-libnl-include="-I$(STAGING_DIR)/usr/include/pon-libnl3" \
	--enable-libnl-library="-L$(STAGING_DIR)/opt/pon/lib" \
	--enable-pon-adapter-include="-I$(STAGING_DIR)/usr/include/pon_adapter" \
	--enable-pon-adapter-library="-L$(STAGING_DIR)/usr/lib" \
	--enable-pon-mcc-drv-include="-I$(STAGING_DIR)/usr/include/pon" \
	--enable-pon-mcc-drv-library="-L$(STAGING_DIR)/usr/lib" \
	$(call autoconf_bool,CONFIG_MCC_DRV_ENABLE,mcc-drv) \
	$(if $(CONFIG_PON_NET_LIB_NO_TC_OFFLOAD), --without-tc-offload, --with-tc-offload)

TARGET_CFLAGS += -Werror

# include path for safec 3.7
TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include/safeclib

ifeq ($(CONFIG_TARGET_intel_x86_lgm),y)
	TARGET_CFLAGS += -DPON_NET_LIB_IP_HOST_ON_URX
endif
define Build/Configure
	mkdir -p $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)
	$(call Build/Configure/Default)
endef

define Build/Clean
	@- [ -e $(PKG_BUILD_DIR)/$(MAKE_PATH)/Makefile ] && $(MAKE) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) clean
	@- rm -Rf $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)
	$(call Build/Clean/Default)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/pon_net
	$(CP) $(PKG_BUILD_DIR)/include/*.h $(1)/usr/include/pon_net
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libponnet.so* $(1)/usr/lib
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libponnet.so* $(1)/usr/lib
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_BIN) ./files/etc/uci-defaults/ds_queues $(1)/etc/uci-defaults/
endef

$(eval $(call KernelPackage,pon-arp-ndp-reinsert))
$(eval $(call BuildPackage,$(PKG_NAME)))
