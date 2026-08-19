#
# Copyright (C) 2024 MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=dp-oam

PKG_LICENSE:=GPL-2.0

include $(INCLUDE_DIR)/package.mk

define KernelPackage/dp-oam
  SUBMENU:=MaxLinear
  TITLE:=DP OAM Driver
  DEPENDS:=+PACKAGE_kmod-qos-tc:kmod-qos-tc
  KCONFIG:=CONFIG_DP_OAM
  FILES:=$(LINUX_DIR)/drivers/net/datapath/dp_oam/dp_oam.ko
  AUTOLOAD:=$(call AutoProbe,dp_oam)
endef

define KernelPackage/dp-oam/description
  Kernel module for DP OAM support.
endef

define Build/Configure
endef

define Build/Compile
endef


$(eval $(call KernelPackage,dp-oam))
