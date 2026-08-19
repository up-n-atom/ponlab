#
# Copyright (c) 2021-2024 MaxLinear Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=gwdpa-dpm
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=gwdpa-dpm
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_SOURCE_VERSION:=1.0.0.206
PKG_MIRROR_HASH:=6e817da5c20640ba36f8fa7ad408297d4e6269d20bf374cdb6df89d58f7a9074
PKG_VERSION:=$(PKG_SOURCE_VERSION)
PKG_MAINTAINER:=dpm team
PKG_BUILD_PARALLEL:=1
PKG_IS_KERNEL_MODULE:=y
PKG_DEFAULT_CUSTOM_SOURCE_DIR:=$(TOPDIR)/../source/gwdpa-dpm/
PKG_LICENSE:=GPL-2.0

-include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk
define KernelPackage/$(PKG_NAME)
  SUBMENU:=MaxLinear
  TITLE:=datapath core/manager/library
  DEFAULT:=y
  DEPENDS:= @(TARGET_intel_mips_prx300||TARGET_intel_x86_lgm)
  MAINTAINER:=dpm team
  FILES:= \
	$(PKG_BUILD_DIR)/$(PKG_NAME).ko
  AUTOLOAD:=$(call AutoLoad,06,gwdpa-dpm)
  MENU:=1
endef

define KernelPackage/$(PKG_NAME)/description
  dpm manager/core/library
endef

define KernelPackage/$(PKG_NAME)/config
  $(call Package/kmod-$(PKG_NAME)/override_version)
  $(call Package/kmod-$(PKG_NAME)/override_source_path)
  menu "configuration"
  depends on PACKAGE_kmod-gwdpa-dpm
  config PACKAGE_kmod-gwdpa-dpm-disable-optimization
		bool "disable gcc optimization"
		help
		  This will disable gcc optimization in dpm
  endmenu
endef

ifeq ($(CONFIG_PACKAGE_kmod-gwdpa-dpm-disable-optimization),y)
    EXTRA_CFLAGS += -DCONFIG_DPM_DATAPATH_DISABLE_OPTIMIZE=1
endif

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		BSP_KERNEL_DIR=$(LINUX_DIR) \
		M="$(PKG_BUILD_DIR)" \
		EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
		modules
endef

define Build/InstallDev
	if [ -e $(PKG_BUILD_DIR)/datapath_gdb_wrapper.h ] && [ "$(1)" != "" ]; then \
		$(INSTALL_DIR) $(1)/include/dpm; \
		$(INSTALL_DATA) $(PKG_BUILD_DIR)/datapath_gdb_wrapper.h $(1)/include/dpm/; \
	fi
endef

define Build/Clean
	if [ -d $(PKG_BUILD_DIR) ]; then \
		$(MAKE) -C $(PKG_BUILD_DIR) KERNEL_DIR=$(LINUX_DIR) clean; \
		$(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); \
	fi
endef

$(eval $(call KernelPackage,$(PKG_NAME)))

