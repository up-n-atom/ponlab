#
# Copyright (C) 2020 - 2023 MaxLinear, Inc.
# Copyright (C) 2011 lantiq.com
# Copyright (C) 2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=gpon-sfp-eeprom
PKG_VERSION:=3.6.0
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=gpon_sfp_eeprom
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=71056eb0c0b6fc4db9c31968ef078657c3bcef238080f1481263f86c8572cda9
PKG_RELEASE:=1
PKG_BIN_DIR=usr/bin
PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1
PKG_ASLR_PIE_REGULAR:=1
PKG_LICENSE:=GPL-2.0-only or BSD-2-Clause
PKG_LICENSE_FILES:=LICENSE
PKG_FIXUP:=autoreconf

-include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

# avoid build directly in the source directories
PKG_AUTOMAKE_PATHS = .
CONFIGURE_PATH = build_$(PKGARCH)
CONFIGURE_CMD = ../configure
MAKE_PATH = $(CONFIGURE_PATH)
PKG_INSTALL_DIR = $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)/ipkg-install

ifeq ($(DUMP),)
	STAMP_CONFIGURED:=$(strip $(STAMP_CONFIGURED))_$(shell $(SH_FUNC) grep '^CONFIG_GPON_' $(TOPDIR)/.config | md5s)
endif

define Package/$(PKG_NAME)
  SECTION:=driver
  CATEGORY:=MaxLinear
  TITLE:= PON SFP EEPROM Application
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear
  DEPENDS:=+ifxos +kmod-pon-sfp-i2c \
	+libsafec3-7
  MENU:=1
  SUBMENU:=PON
endef

define Package/$(PKG_NAME)/description
   The sfp_i2c control application for the PON device
endef

# make the submake silent:
ifeq ($(QUIET),1)
MAKE_FLAGS+=-s
endif
ifneq (,$(findstring 99,$(V)))
MAKE_FLAGS+=-s
endif

DEFAULT_VENDOR_NAME="MaxLinear"
DEFAULT_VENDOR_PARTNO="0"
DEFAULT_VENDOR_REV="0001"
DEFAULT_VENDOR_OUI="00:50:F1"
DEFAULT_SERIAL_NO="no serial number"
DEFAULT_DATECODE="2021xxxx"
DEFAULT_DEVADDR0=0x50
DEFAULT_DEVADDR1=0x51

define Package/$(PKG_NAME)/config
	menu "configuration"
	depends on PACKAGE_$(PKG_NAME)
		config GPON_SFP_EEPROM_VENDOR_NAME
			string "Vendor Name (16 chars)"
			default $(DEFAULT_VENDOR_NAME)

		config GPON_SFP_EEPROM_VENDOR_PARTNO
			string "Vendor Part Number (16 chars)"
			default $(DEFAULT_VENDOR_PARTNO)

		config GPON_SFP_EEPROM_VENDOR_REV
			string "Vendor Revision Number (4 chars)"
			default $(DEFAULT_VENDOR_REV)

		config GPON_SFP_EEPROM_VENDOR_OUI
			string "Vendor OUI (3 hex bytes)"
			default $(DEFAULT_VENDOR_OUI)
			help
			  If this is left to the default $(DEFAULT_VENDOR_OUI)
			  then the OUI is left from the config and derived
			  from the MAC address of the system.

		config GPON_SFP_EEPROM_SERIAL_NO
			string "Serial Number (16 chars)"
			default $(DEFAULT_SERIAL_NO)

		config GPON_SFP_EEPROM_DATECODE
			string "Datecode (8 chars)"
			default $(DEFAULT_DATECODE)

		config GPON_SFP_EEPROM_DEVADDR0
			hex "Device address of first EEPROM"
			default $(DEFAULT_DEVADDR0)

		config GPON_SFP_EEPROM_DEVADDR1
			hex "Device address of second EEPROM"
			default $(DEFAULT_DEVADDR1)

		menuconfig GPON_SFP_EEPROM_BRIDGE_MODE
			bool "I2C Bridge Mode"
			default y

		if GPON_SFP_EEPROM_BRIDGE_MODE

		config GPON_SFP_EEPROM_DEVADDR0_BRIDGE
			hex "Device address of first EEPROM"
			default 0x4E

		config GPON_SFP_EEPROM_DEVADDR1_BRIDGE
			hex "Device address of second EEPROM"
			default 0x4F

		config GPON_SFP_EEPROM_PMD0_PATH
			string "Path to first PMD EEPROM"
			default "/sys/bus/i2c/devices/0-004e/eeprom"

		config GPON_SFP_EEPROM_PMD1_PATH
			string "Path to second PMD EEPROM"
			default "/sys/bus/i2c/devices/0-004f/eeprom"

		endif   # if GPON_SFP_EEPROM_BRIDGE_MODE
	endmenu
	$(call Package/$(PKG_NAME)/override_source_path)
endef

CONFIGURE_ARGS += \
	--enable-monitor-sync \
	--enable-kernelincl="$(LINUX_DIR)/include" \
	--enable-sfp-i2c-include="-I$(STAGING_DIR)/usr/include/sfp_i2c_drv" \
	--enable-ifxos-include="-I$(STAGING_DIR)/usr/include/ifxos" \
	--enable-ifxos-library="-L$(STAGING_DIR)/usr/lib"

TARGET_CFLAGS += -Werror

#include path for safec 3.7
TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include/safeclib

define Build/Configure
	mkdir -p $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)
	# remove "configured" stamps of other configs
	@-rm $(PKG_BUILD_DIR)/.configured*
	$(call Build/Configure/Default)
endef

define Build/Clean
	@- rm -Rf $(PKG_BUILD_DIR)/ipkg*
	@- [ -e $(PKG_BUILD_DIR)/$(MAKE_PATH)/Makefile ] && $(MAKE) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) clean
	@- rm -Rf $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)
	$(call Build/Clean/Default)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/sfp_eeprom
	$(CP) $(PKG_INSTALL_DIR)/usr/include/* $(1)/usr/include/sfp_eeprom
endef

# Add an option if it is different from the default value
define AddOption
# this line is to avoid empty macro
ifneq ($(CONFIG_GPON_SFP_EEPROM_$(3)),$(DEFAULT_$(3)))
	echo -e "\toption '$(2)' '$(CONFIG_GPON_SFP_EEPROM_$(3))'" >> $(1)/sfp_eeprom
endif
endef

define DefaultConfigCreate
	echo "config 'sfp_eeprom' 'default'" > $(1)/sfp_eeprom
	$(call AddOption,$(1),vendor_name,VENDOR_NAME)
	$(call AddOption,$(1),vendor_partno,VENDOR_PARTNO)
	$(call AddOption,$(1),vendor_rev,VENDOR_REV)
	$(call AddOption,$(1),vendor_oui,VENDOR_OUI)
	$(call AddOption,$(1),serial_no,SERIAL_NO)
	$(call AddOption,$(1),datecode,DATECODE)
	$(call AddOption,$(1),addr_eeprom_0,DEVADDR0)
	$(call AddOption,$(1),addr_eeprom_1,DEVADDR1)
	# add empty line
	echo "" >> $(1)/sfp_eeprom
ifneq (CONFIG_GPON_SFP_EEPROM_BRIDGE_MODE,)
	echo "config 'sfp_eeprom' 'factory_bridge'" >> $(1)/sfp_eeprom
	echo -e "\toption 'enable' '0'" >> $(1)/sfp_eeprom
	# the defaults for these options are not defined,
	# so they will always added to the config file
	$(call AddOption,$(1),addr_eeprom_0,DEVADDR0_BRIDGE)
	$(call AddOption,$(1),addr_eeprom_1,DEVADDR1_BRIDGE)
	$(call AddOption,$(1),pmd_0,PMD0_PATH)
	$(call AddOption,$(1),pmd_1,PMD1_PATH)
	# add empty line
	echo "" >> $(1)/sfp_eeprom
endif

endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/$(PKG_BIN_DIR)
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/sfp_i2c $(1)/$(PKG_BIN_DIR)
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) files/sfp_eeprom.sh $(1)/etc/init.d/
	$(INSTALL_DIR) $(1)/etc/config
	$(call DefaultConfigCreate,$(1)/etc/config)
	$(INSTALL_DIR) $(1)/lib/firmware/
	# convert from "hexdump -Cv" format to binary
	dump=$$$$(sed 's/^\(.\)\{10\}\(.*\) \(|.*|\)$$$$/\2/g' files/eeprom0.txt);\
		for i in $$$$dump; \
			do printf "\x$$$$i"; \
		done > $(1)/lib/firmware/sfp_eeprom0.bin
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
