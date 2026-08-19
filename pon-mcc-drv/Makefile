#
# Copyright (C) 2021 - 2023 MaxLinear, Inc.
# Copyright (C) 2018 - 2019 Intel Corporation
# Copyright (C) 2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=pon-mcc-drv
PKG_NAME_CLI:=pon-mcc-cli
PKG_NAME_LIB:=pon-mcc-lib
PKG_VERSION:=1.12.1
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=pon_mcc_drv
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=ad0cd77b2918d343ccd9c82efded4c4acef33e9db4b88e9b2d5e4ec3849c7c74
PKG_BUILD_PARALLEL:=1
PKG_FIXUP:=autoreconf
PKG_LICENSE:=GPL-2.0-only or BSD-2-Clause
PKG_LICENSE_FILES:=LICENSE
PKG_INSTALL:=1

PKG_IS_KERNEL_MODULE:=y

-include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# avoid build directly in the source directories
PKG_AUTOMAKE_PATHS = .
CONFIGURE_PATH = build_$(PKGARCH)
CONFIGURE_CMD = ../configure
MAKE_PATH = $(CONFIGURE_PATH)
PKG_INSTALL_DIR = $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)/ipkg-install
PKG_EXTMOD_SUBDIRS = $(MAKE_PATH)/src $(MAKE_PATH)/chk

define KernelPackage/$(PKG_NAME)
  SUBMENU:=MaxLinear
  TITLE:=PON multicast driver
  URL:=https://www.maxlinear.com
  MAINTAINER:=MaxLinear
  DEPENDS:= +kmod-pon-mcc-drv-filter +kmod-gwdpa-dpm
  FILES:=$(PKG_INSTALL_DIR)/usr/bin/mod_pon_mcc.ko
  AUTOLOAD:=$(call AutoProbe,mod_pon_mcc)
endef

define KernelPackage/$(PKG_NAME)-chk
  SUBMENU:=MaxLinear
  TITLE:=PON multicast driver checker
  URL:=https://www.maxlinear.com
  MAINTAINER:=MaxLinear
  DEPENDS:= +kmod-$(PKG_NAME)
  FILES:=$(PKG_INSTALL_DIR)/usr/bin/mod_pon_mcc_chk.ko
endef

define KernelPackage/$(PKG_NAME)/description
   The PON Multicast driver for the PON device
endef

define KernelPackage/pon-mcc-drv-filter
  TITLE:=PON multicast filter driver
  SUBMENU:=MaxLinear
  DEPENDS:=+kmod-nf-reject
  KCONFIG:=CONFIG_DP_MCAST_BRIDGE_FILTER
  FILES:=$(LINUX_DIR)/drivers/net/datapath/bridge_filter/mcast_bridge_filter.ko
  AUTOLOAD:=$(call AutoProbe,mcast_bridge_filter)
endef


define KernelPackage/$(PKG_NAME)/config
$(call Package/$(PKG_NAME_PREF)$(PKG_NAME)/override_source_path)
	config PACKAGE_kmod-pon-mcc-drv_KCOV_INSTRUMENT
		bool "Enable KCOV instrumentation" if !KERNEL_KCOV_INSTRUMENT_ALL
		depends on PACKAGE_kmod-pon-mcc-drv
		depends on KERNEL_KCOV
		help
		  This will enable KCOV instrumentation for the driver.
endef

ifeq ($(CONFIG_PACKAGE_kmod-pon-mcc-drv_KCOV_INSTRUMENT),y)
MAKE_FLAGS += CONFIG_KCOV_INSTRUMENT_ALL=y
endif

CONFIGURE_ARGS += \
	--enable-linux-kernel \
	--enable-kernelbuild="$(LINUX_DIR)" \
	ARCH=$(LINUX_KARCH) \
	KBUILD_EXTRA_SYMBOLS="\"$(wildcard $(PKG_SYMVERS_DIR)/*.symvers)\"" \
	$(call autoconf_bool,CONFIG_PACKAGE_kmod-pon-mcc-drv-chk,chk)

define Package/$(PKG_NAME_LIB)
  SUBMENU:=PON
  CATEGORY:=MaxLinear
  TITLE:=PON mcc drv library
  DEPENDS:=+libnl-genl
endef

define Package/$(PKG_NAME_CLI)
  SUBMENU:=PON
  CATEGORY:=MaxLinear
  TITLE:=PON mcc drv cli tool
  DEPENDS:=+$(PKG_NAME_LIB)
endef

TARGET_CPPFLAGS:= \
	-Werror \
	-I$(STAGING_DIR)/usr/include/libnl3

define Build/Configure
	mkdir -p $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)
	@- rm $(PKG_BUILD_DIR)/.configured*
	$(call Build/Configure/Default)
endef

define Build/Clean
	@- [ -e $(PKG_BUILD_DIR)/$(MAKE_PATH)/Makefile ] && $(MAKE) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) clean
	@- rm -Rf $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)
	$(call Build/Clean/Default)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/pon
	$(CP) $(PKG_INSTALL_DIR)/usr/include/pon/* $(1)/usr/include/pon
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libponmcc.so* $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libponmcc.a $(1)/usr/lib
endef

define Package/$(PKG_NAME_LIB)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libponmcc.so* $(1)/usr/lib
endef

define Package/$(PKG_NAME_CLI)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/ponmcc $(1)/usr/bin
endef

$(eval $(call KernelPackage,$(PKG_NAME)))
$(eval $(call KernelPackage,$(PKG_NAME)-chk))
$(eval $(call KernelPackage,pon-mcc-drv-filter))
$(eval $(call BuildPackage,$(PKG_NAME_LIB)))
$(eval $(call BuildPackage,$(PKG_NAME_CLI)))
