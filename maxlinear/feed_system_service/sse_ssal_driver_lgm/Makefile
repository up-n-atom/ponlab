#
# Copyright © 2020-2023 MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=sse_ssal_driver_lgm
PKG_SOURCE_VERSION:=5.14.1.17
PKG_VERSION:=$(PKG_SOURCE_VERSION)
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=sse_ssal_driver
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=dfcdf69a93c3b870b5d172cd66c54c2179b9862e1c56d47c7381cb34ca377faf

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)
PKG_CHECK_FORMAT_SECURITY:=0
PKG_IS_KERNEL_MODULE:=y
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=GPL-2.0
PKG_MXL_LICENSE:=GPL/LICENSE

include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk

define KernelPackage/$(PKG_NAME)
  SUBMENU:=Other modules
  TITLE:=sse ssal driver for lgm
  FILES:=$(PKG_BUILD_DIR)/ssal.ko
  AUTOLOAD:=$(call AutoLoad,80,ssal,1)
endef

define KernelPackage/$(PKG_NAME)/description
	Secure Service ssal driver
endef

ifeq ($(CONFIG_LGM_EIP123_SUPPORT),y)
PKG_EXTRA_CFLAGS += -DEIP123_DEF
endif

ifeq ($(CONFIG_LGM_EIP123_DEBUG),y)
PKG_EXTRA_CFLAGS += -DDEBUG
endif

ifeq ($(CONFIG_X86_64),y)
PKG_EXTRA_CFLAGS += -DCLFLUSH_CACHE
endif

PKG_EXTRA_CFLAGS += -I$(LINUX_DIR)/include/uapi/linux/icc/

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		ARCH="$(LINUX_KARCH)" \
		M="$(PKG_BUILD_DIR)" \
		EXTRA_CFLAGS+="$(PKG_EXTRA_CFLAGS)" \
		modules
endef

$(eval $(call KernelPackage,$(PKG_NAME)))
