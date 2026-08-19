#
# Copyright (C) 2021-2024 MaxLinear, Inc.
# Copyright (C) 2016 Intel Corporation
# Copyright (C) 2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=packet_learn
PKG_VERSION:=1.0.35
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=packet_learn
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=3838c1f5fd4753fb4d096b13fe5e884d573c052b116e6fcfeb8c80feded9161d
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=GPL-2.0

PKG_BUILD_PARALLEL:=1
PKG_IS_KERNEL_MODULE=y
PKG_DEFAULT_CUSTOM_SOURCE_DIR:=$(TOPDIR)/packet_learn/

-include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define KernelPackage/$(PKG_NAME)
  SUBMENU:=MaxLinear
  TITLE:=Packet Learning Driver
  DEPENDS:=+kmod-l2tp @TARGET_intel_mips||TARGET_intel_x86_lgm

  MAINTAINER:=MaxLinear
  KCONFIG:= \
	CONFIG_LTQ_PON_ETH=n
  FILES:= \
	$(PKG_BUILD_DIR)/learning.ko

  AUTOLOAD:=$(call AutoProbe,learning)
  MENU:=1
endef

define KernelPackage/$(PKG_NAME)/description
  Packet Learning Driver
endef

define KernelPackage/$(PKG_NAME)/config
  $(call Package/kmod-$(PKG_NAME)/override_version)
  $(call Package/kmod-$(PKG_NAME)/override_source_path)
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		M="$(PKG_BUILD_DIR)" \
		EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
		modules
endef

define Build/Clean
	if [ -d $(PKG_BUILD_DIR) ]; then \
		$(MAKE) -C $(PKG_BUILD_DIR) KERNEL_DIR=$(LINUX_DIR) clean; \
		$(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); \
	fi
endef

$(eval $(call KernelPackage,$(PKG_NAME)))
