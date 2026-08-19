#
# Copyright © 2022-2024 MaxLinear, Inc.
# Openwrt Makefile for mcast-helper
#

include $(TOPDIR)/rules.mk

PKG_NAME:=mcast-helper
PKG_VERSION:=3.4.1.19
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=mcast_helper
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=33a152ab91d2ccde31fa3731a132412c33015f14590661faab9ab41b4879bd27
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=GPL
PKG_MXL_LICENSE:=GPL/LICENSE
PKG_IS_KERNEL_MODULE:=y
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk

define KernelPackage/$(PKG_NAME)
  SUBMENU:=MaxLinear
  TITLE:=Multicast Helper Kernel Module
  KCONFIG:=CONFIG_MCAST_HELPER_PACKAGE=y \
	CONFIG_MCAST_HELPER_REG=y \
	CONFIG_MCAST_HELPER=m
  FILES:=$(PKG_BUILD_DIR)/mcast_helper.ko
  AUTOLOAD:=$(call AutoProbe,mcast_helper)
  MENU:=1
endef

define KernelPackage/$(PKG_NAME)/description
  A kernel module to help PPA & WLAN for multicast
  acceleration and ACL support.
endef

define KernelPackage/$(PKG_NAME)/config
  $(call Package/kmod-$(PKG_NAME)/override_version)
  $(call Package/kmod-$(PKG_NAME)/override_source_path)
  config $(PKG_NAME)-acl
  bool "Multicast Helper ACL support"
  depends on PACKAGE_kmod-$(PKG_NAME)
  help
    mcast_helper ACL support
endef

# Extra cflags for compilation
PKG_EXTRA_CFLAGS += -Werror -Wno-uninitialized -Wno-switch-unreachable

ifneq ($(CONFIG_mcast-helper-acl),)
EXTRA_KCONFIG += CONFIG_MCAST_HELPER_ACL=y
endif

# Convert to compilation flags e.g. CONFIG_MCAST_HELPER=m to -DCONFIG_MCAST_HELPER=1
PKG_EXTRA_CFLAGS += \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=m,%,$(filter %=m,$(EXTRA_KCONFIG)))) \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=y,%,$(filter %=y,$(EXTRA_KCONFIG))))

KERNEL_MAKEOPTS += EXTRA_CFLAGS="$(PKG_EXTRA_CFLAGS)" $(EXTRA_KCONFIG)

define Build/Compile
	$(KERNEL_MAKE) M="$(PKG_BUILD_DIR)" modules
endef

define Build/Clean
	if [ -d $(PKG_BUILD_DIR) ]; then \
		$(MAKE) -C $(PKG_BUILD_DIR) KERNEL_DIR=$(LINUX_DIR) clean; \
	fi
endef

$(eval $(call KernelPackage,$(PKG_NAME)))
