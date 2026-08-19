#
# Copyright (C) 2022-2023 MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# Makefile for Inside Secure DDK EIP197 for linux 4.9
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ###############################
PKG_NAME:=eip197
PKG_SOURCE_VERSION:=1.0.0.4
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=eip_197_ddk
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=5cf8c3a15e850c13a176ca5234b8b45816ad44e90f4a54e03f8aa15d5baf3d59
PKG_IS_KERNEL_MODULE:=y
PKG_DEFAULT_CUSTOM_SOURCE_DIR:=$(TOPDIR)/../source/eip197/

include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk

define KernelPackage/$(PKG_NAME)
  SUBMENU:=MaxLinear
  TITLE:=EIP197 crypto driver
  DEPENDS:=@TARGET_intel_x86_lgm @LINUX_4_19 +eip197-firmware
  MAINTAINER:=Maxlinear Inc
  KCONFIG:= \
	CONFIG_CRYPTO_HW=y

  FILES:= \
	$(PKG_BUILD_DIR)/intel_eip197.ko

  AUTOLOAD:=$(call AutoLoad,80,intel_eip197)
endef

define KernelPackage/$(PKG_NAME)/description
  Kernel module to EIP-197 crypto library
endef

define Build/Compile
	$(KERNEL_MAKE) SUBDIRS="$(PKG_BUILD_DIR)" modules
endef

define Build/Clean
	if [ -d $(PKG_BUILD_DIR) ]; then \
	  $(KERNEL_MAKE) SUBDIRS="$(PKG_BUILD_DIR)" clean; \
	  $(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); \
	fi
endef

$(eval $(call KernelPackage,$(PKG_NAME)))
