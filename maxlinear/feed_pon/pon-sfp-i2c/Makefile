#
# Copyright (C) 2020 - 2023 MaxLinear, Inc.
# Copyright (C) 2018 Intel Corporation
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=pon-sfp-i2c
PKG_VERSION:=1.2.1
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=pon_sfp_i2c
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=317418509e3b7137374506177c1f5d0f7900de89ce08222a589cf2e0b6400350
PKG_BUILD_PARALLEL:=1
PKG_LICENSE:=GPL-2.0-only or BSD-2-Clause
PKG_LICENSE_FILES:=LICENSE
PKG_IS_KERNEL_MODULE:=y

-include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk


# firmware details:
PKG_FW_VERSION:=v1.0.2.1.24
# dummy url
PKG_FW_URL:=http://localhost
PKG_FW_PATH:=prx_i2c_aca_fw-$(PKG_FW_VERSION)
PKG_FW_FILENAME:=$(PKG_FW_PATH).tar.gz

FW_BIN_NAME:=prx_i2c_aca_fw.bin

define Download/fw
  FILE:=$(PKG_FW_FILENAME)
  URL:=$(PKG_FW_URL)
  HASH:=1505dacce12cb053c71ac3de29c0981e347d15d9e2a92bc536aee14510635567
endef
$(eval $(call Download,fw))

define KernelPackage/$(PKG_NAME)
  SUBMENU:=MaxLinear
  TITLE:=PON SFP I2C driver
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear
  DEPENDS:=
  FILES:=$(PKG_BUILD_DIR)/src/mod_sfp_i2c.ko
  AUTOLOAD:=$(call AutoLoad,20,mod_sfp_i2c)
  MODPARAMS.mod_sfp_i2c:= \
	fw_name=$(FW_BIN_NAME) \
	eeprom0_init=sfp_eeprom0.bin
endef

define KernelPackage/$(PKG_NAME)/config
$(call Package/$(PKG_NAME_PREF)$(PKG_NAME)/override_source_path)
	config PACKAGE_kmod-pon-sfp-i2c_KCOV_INSTRUMENT
		bool "Enable KCOV instrumentation" if !KERNEL_KCOV_INSTRUMENT_ALL
		depends on PACKAGE_kmod-pon-sfp-i2c
		depends on KERNEL_KCOV
		help
		  This will enable KCOV instrumentation for the driver.
endef

PON_MAKEDEFS += CONFIG_PON_SFP_I2C=m

ifeq ($(CONFIG_PACKAGE_kmod-pon-sfp-i2c_KCOV_INSTRUMENT),y)
PON_MAKEDEFS += CONFIG_KCOV_INSTRUMENT_ALL=y
endif


define KernelPackage/$(PKG_NAME)/description
   The driver and firmware for PON SFP I2C emulation
endef

define Build/Compile
	+$(KERNEL_MAKE) $(PON_MAKEDEFS) $(PKG_JOBS) \
		M="$(PKG_BUILD_DIR)" \
		NOSTDINC_FLAGS="$(NOSTDINC_FLAGS)" \
		modules
endef


define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/sfp_i2c_drv
	$(CP) $(PKG_BUILD_DIR)/src/include/* $(1)/usr/include/sfp_i2c_drv
endef

define KernelPackage/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/lib/firmware
	tar xfz $(DL_DIR)/$(PKG_FW_FILENAME) -C $(PKG_BUILD_DIR)
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/$(PKG_FW_PATH)/aca_fw.bin \
		$(1)/lib/firmware/$(FW_BIN_NAME)
	# if a files folder exist, also copy it:
	[ "`ls ./files/*.bin`" ] && $(CP) files/*.bin $(1)/lib/firmware/ ||true
endef

$(eval $(call KernelPackage,$(PKG_NAME)))
