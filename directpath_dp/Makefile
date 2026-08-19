#
# Copyright (C) 2020-2023 MaxLinear, Inc.
# Copyright (C) 2016 Intel Corporation
# Copyright (C) 2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=directpath-dp
PKG_VERSION:=1.0.1
PKG_RELEASE:=1
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=dp_dp_drv
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_VERSION:=225b0c14f23e953cae4efb0a53e758025b1853f3
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=eb40c79505b81717ab39444915530a31b846b8c804af29eb1bfb2ccf6f127b25
PKG_SOURCE_PROTO:=git
#PKG_SOURCE_MIRROR:=1
PKG_MIRROR:=0
PKG_MXL_LICENSE:=GPL/LICENSE

include $(INCLUDE_DIR)/package.mk

define KernelPackage/$(PKG_NAME)
  SUBMENU:=MaxLinear
  TITLE:=Directpath datapath driver
  DEPENDS:=@TARGET_intel_mips
  KCONFIG:= \
	CONFIG_DIRECTPATH_DP_API=y

  FILES:= \
	$(PKG_BUILD_DIR)/common/directpath_datapath.ko \
	$(if $(CONFIG_TARGET_intel_mips_xrx500),$(PKG_BUILD_DIR)/platform/directpath-dp-xrx500.ko)

  AUTOLOAD:=$(call AutoProbe, \
		directpath_datapath \
		$(if $(CONFIG_TARGET_intel_mips_xrx500),directpath-dp-xrx500))
  MENU:=1
endef

define KernelPackage/$(PKG_NAME)/description
  DirectPath datapath driver
endef

EXTRA_CFLAGS += -I$(PKG_BUILD_DIR)/include

ifeq ($(CONFIG_TARGET_intel_mips_xrx500),y)
	PKG_EXTRA_KCONFIG += CONFIG_DIRECTPATH_DP_XRX500=y
endif

define Build/Prepare
	$(call Build/Prepare/Default)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(STAGING_DIR)/include/net/
	$(CP) $(PKG_BUILD_DIR)/include/*.h $(STAGING_DIR)/include/net/
endef

KERNEL_MAKEOPTS += \
	$(PKG_EXTRA_KCONFIG) \
	EXTRA_CFLAGS="$(EXTRA_CFLAGS)"

define Build/Compile
	$(KERNEL_MAKE) SUBDIRS="$(PKG_BUILD_DIR)" modules
endef

#$(eval $(call KernelPackage,$(PKG_NAME)))
