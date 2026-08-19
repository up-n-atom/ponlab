#
# Copyright (C) 2022 - 2023 MaxLinear, Inc.
#
# Speed test kernel module
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk

PKG_NAME:=speedtest-driver
PKG_VERSION:=1.0.0.2
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=GPL-2.0
PKG_MXL_LICENSE:=GPL/LICENSE
PKG_MIRROR_HASH:=skip

ifeq ($(CONFIG_PACKAGE_kmod-dnsresolver),y)
	EXTRA_CFLAGS:=-DCONFIG_DNS_RESOLVER
endif

define KernelPackage/$(PKG_NAME)
	SUBMENU:=MaxLinear
	TITLE:=Speed test kernel driver
	KCONFIG:=CONFIG_SPEEDTEST_DRIVER=m
	FILES:=$(PKG_BUILD_DIR)/speedtest.ko
	URL:=http://www.maxlinear.com
	MAINTAINER:=MaxLinear Inc
	DEPENDS:=@TARGET_intel_x86_lgm||TARGET_intel_mips +libkeyutils +keyutils +kmod-dnsresolver
endef

define KernelPackage/$(PKG_NAME)/description
	Speed test driver for TR-143 diagnostic tool
	For in-kernel DNS resolver support, enable kmod-dnsresolver
endef

EXTRA_KCONFIG:=CONFIG_SPEEDTEST_DRIVER=m

KERNEL_MAKEOPTS += EXTRA_CFLAGS="$(EXTRA_CFLAGS)" $(EXTRA_KCONFIG)

define Build/Compile
	$(KERNEL_MAKE) M="$(PKG_BUILD_DIR)" modules
endef

define Build/InstallDev
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/tr143diagtool.h $(STAGING_DIR)/usr/include/
endef

$(eval $(call KernelPackage,$(PKG_NAME)))
