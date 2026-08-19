#
# Copyright © 2020-2023 MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=sse_storage_drv
PKG_SOURCE_VERSION:=5.11.1.5
PKG_VERSION:=$(PKG_SOURCE_VERSION)
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=system_service
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_INNERDIR:=sse_storage_src
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=348b8feb0706eb1643afbaf2962109f49a9bfa219c30468900c26b9f81ffee81

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
  TITLE:=sse_storage_drv lkm
  DEPENDS:= +kmod-sse_driver
  FILES:=$(PKG_BUILD_DIR)/secure_storage_driver.ko
  AUTOLOAD:=$(call AutoLoad,78,secure_storage_driver,1)
endef

define KernelPackage/$(PKG_NAME)/description
	Secure Service driver
endef

ifeq ($(CONFIG_EIP123_SUPPORT),y)
PKG_EXTRA_CFLAGS += -DEIP123_DEF
endif

ifeq ($(CONFIG_EIP123_DEBUG),y)
PKG_EXTRA_CFLAGS += -DDEBUG
endif

ifeq ($(CONFIG_X86_64),y)
PKG_EXTRA_CFLAGS += -DCLFLUSH_CACHE
endif

PKG_EXTRA_CFLAGS += -I$(LINUX_DIR)/include/uapi/linux/icc/ -I$(LINUX_DIR)/drivers/mtd/ubi/

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
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
