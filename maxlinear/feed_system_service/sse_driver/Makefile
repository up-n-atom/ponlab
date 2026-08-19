#
# Copyright © 2020-2023 MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=sse_driver
PKG_SOURCE_VERSION:=5.6.1.6
PKG_VERSION:=$(PKG_SOURCE_VERSION)
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=system_service
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_INNERDIR:=sse_driver_src
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=43ec3a41fdc378c9e68b4efacd184a98970561db4dfb835facd8bd0c6d8bf085

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)
PKG_IS_KERNEL_MODULE:=y
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=GPL-2.0
PKG_MXL_LICENSE:=GPL/LICENSE
PKG_CONFIG_DEPENDS:= \
	CONFIG_EIP123_SUPPORT \
	CONFIG_EIP123_DEBUG

include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk

define KernelPackage/$(PKG_NAME)
  SUBMENU:=Other modules
  TITLE:=sse_driver lkm
  FILES:=$(PKG_BUILD_DIR)/secure_service_driver.ko
  AUTOLOAD:=$(call AutoLoad,80,secure_service_driver,1)
endef

define KernelPackage/$(PKG_NAME)/description
	Secure Service driver
endef

### Menuconfig ##################################
define Package/kmod-$(PKG_NAME)/config
$(call Package/kmod-$(PKG_NAME)/override_version)
$(call Package/kmod-$(PKG_NAME)/override_source_path)
	menu "kmod-$(PKG_NAME) features"
	depends on PACKAGE_kmod-$(PKG_NAME)
	source "$(SOURCE)/config/Config.in"
	endmenu
endef

ifeq ($(CONFIG_EIP123_SUPPORT),y)
PKG_EXTRA_CFLAGS += -DEIP123_DEF
endif

ifeq ($(CONFIG_EIP123_DEBUG),y)
PKG_EXTRA_CFLAGS += -DDEBUG -DSSE_TEST_STUB
endif

ifeq ($(CONFIG_X86_64),y)
PKG_EXTRA_CFLAGS += -DCLFLUSH_CACHE
endif

ifeq ($(CONFIG_TARGET_intel_mips_prx300),y)
PKG_EXTRA_CFLAGS += -DCONFIG_PON_FW_DOWNLOAD
endif

PKG_EXTRA_CFLAGS += -I$(LINUX_DIR)/include/uapi/linux/icc/

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/secure_services.h $(1)/usr/include/
endef


define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		EXTRA_CFLAGS+="$(PKG_EXTRA_CFLAGS)" \
		modules
endef

$(eval $(call KernelPackage,$(PKG_NAME)))
