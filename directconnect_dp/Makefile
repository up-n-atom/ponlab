#
# Copyright (C) 2020-2024 MaxLinear, Inc.
# Copyright (C) 2016 Intel Corporation
# Copyright (C) 2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=directconnect-dp
PKG_RELEASE:=1
PKG_SOURCE_VERSION:=1.5.17
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=dc_dp_drv
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
#PKG_SOURCE_MIRROR:=1
PKG_MIRROR:=0
PKG_MIRROR_HASH:=93cecaccd7be5c86dd903a53d72a14ae84564388aa4ad8f93582247e5e6a31a6
PKG_MXL_LICENSE:=GPL/LICENSE
PKG_LICENSE:=GPL-2.0

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk

define KernelPackage/$(PKG_NAME)
  SUBMENU:=MaxLinear
  TITLE:=DirectConnect datapath driver
  DEPENDS:=@TARGET_intel_mips||TARGET_intel_x86
  DEPENDS+=+!TARGET_intel_mips_xrx500:kmod-gwdpa-dpm +kmod-mth-macdb
  KCONFIG:=CONFIG_DIRECTCONNECT_DP_API=y
  FILES:= \
	$(PKG_BUILD_DIR)/common/directconnect_datapath.ko \
	$(PKG_BUILD_DIR)/dc_mode/dc_mode-hw.ko
  AUTOLOAD:=$(call AutoLoad,10,directconnect_datapath dc_mode-hw)
endef

define KernelPackage/$(PKG_NAME)/description
  DirectConnect datapath driver
endef

define KernelPackage/mth-macdb
  SUBMENU:=MaxLinear
  TITLE:=MAC Transition Handling (MTH) MACDB driver
  DEPENDS:=@TARGET_intel_mips||TARGET_intel_x86
  FILES:=$(PKG_BUILD_DIR)/mth/mth_macdb.ko
  AUTOLOAD:=$(call AutoLoad,10,mth_macdb)
endef

define KernelPackage/mth-macdb/description
  MAC Transition Handling (MTH) MACDB driver
endef

define KernelPackage/$(PKG_NAME)-test
  SUBMENU:=MaxLinear
  TITLE:=DirectConnect datapath test driver
  DEPENDS:=@kmod-directconnect-dp +kmod-directconnect-dp
  FILES:=$(PKG_BUILD_DIR)/test/test-wave600_drv.ko
endef

define KernelPackage/$(PKG_NAME)-test/description
  DirectConnect datapath test driver
endef

ifeq ($(CONFIG_PACKAGE_kmod-directconnect-dp),y)
	EXTRA_CFLAGS += -DCONFIG_DIRECTCONNECT_DP_DBG
	EXTRA_CFLAGS += -I$(PKG_BUILD_DIR)/include
	PKG_EXTRA_KCONFIG += CONFIG_DIRECTCONNECT_DP=y

ifeq ($(CONFIG_TARGET_intel_mips_xrx500),y)
	EXTRA_CFLAGS += -DHAVE_DATAPATH_API
	PKG_EXTRA_KCONFIG += CONFIG_DIRECTCONNECT_DP_XRX500=y
endif

ifeq ($(CONFIG_TARGET_intel_x86_lgm),y)
	EXTRA_CFLAGS += -DHAVE_DATAPATH_EXT_API
	EXTRA_CFLAGS += -DHAVE_DATAPATH_MULTI_PORT
	PKG_EXTRA_KCONFIG += CONFIG_DIRECTCONNECT_DP_LGM=y
endif

ifeq ($(CONFIG_TARGET_intel_mips_prx300),y)
	EXTRA_CFLAGS += -DHAVE_DATAPATH_EXT_API
	PKG_EXTRA_KCONFIG += CONFIG_DIRECTCONNECT_DP_PRX300=y
endif
endif

ifeq ($(CONFIG_PACKAGE_kmod-mth-macdb),y)
	PKG_EXTRA_KCONFIG += CONFIG_MTH_MACDB=y
endif

ifeq ($(CONFIG_PACKAGE_kmod-directconnect-dp-test),y)
	EXTRA_CFLAGS += -DCONFIG_TEST_NO_PCI
	PKG_EXTRA_KCONFIG += CONFIG_DIRECTCONNECT_DP_TEST=y
endif

define Build/InstallDev
	$(INSTALL_DIR) $(STAGING_DIR)/include/net/
ifeq ($(CONFIG_PACKAGE_kmod-directconnect-dp),y)
	$(CP) $(PKG_BUILD_DIR)/include/*.h $(STAGING_DIR)/include/net/
endif
ifeq ($(CONFIG_PACKAGE_kmod-mth-macdb),y)
	$(CP) $(PKG_BUILD_DIR)/mth/mth_macdb.h $(STAGING_DIR)/include/net/
endif
endef

KERNEL_MAKEOPTS += \
	$(PKG_EXTRA_KCONFIG) \
	EXTRA_CFLAGS="$(EXTRA_CFLAGS)"

define Build/Compile
	$(KERNEL_MAKE) M="$(PKG_BUILD_DIR)" modules
endef

$(eval $(call KernelPackage,$(PKG_NAME)))
$(eval $(call KernelPackage,mth-macdb))
$(eval $(call KernelPackage,$(PKG_NAME)-test))
