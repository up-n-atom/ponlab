#
# Copyright © 2020-2023 MaxLinear, Inc.
# Copyright (C) 2017 Intel Corporation
# Makefile for gphy_event
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ###############################
PKG_NAME:=gphy_event
PKG_SOURCE_VERSION:=5.3.1.3
PKG_VERSION:=$(PKG_SOURCE_VERSION)
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=system_service
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_INNERDIR:=gphy_event_src
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=8ae84f9a1805dc26c0005928d50d4de3209e02246a3f3b27a14e601161e9c5d6

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)

PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=GPL-2.0
PKG_MXL_LICENSE:=GPL/LICENSE

include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk

define KernelPackage/$(PKG_NAME)
  SUBMENU:=MaxLinear
  TITLE:=Switch LAN port phy netlink event notifier
  DEPENDS:=+switch_cli_ugw8
  FILES:=$(PKG_BUILD_DIR)/eth_phy_status.ko
  DEFAULT:=y if CONFIG_TARGET_intel_mips_xrx500
  AUTOLOAD:=$(call AutoLoad,20,drv_switch_api eth_phy_status)
endef

define KernelPackage/$(PKG_NAME)/Description
	This will create a kernel thread & keep checking the switch ethernet port physical connectivity status
endef


PKG_EXTRA_CFLAGS += -I$(LINUX_DIR)/include/net/switch_api


define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		EXTRA_CFLAGS+="$(PKG_EXTRA_CFLAGS)" \
		modules
endef

$(eval $(call KernelPackage,$(PKG_NAME)))
