#
# Copyright (C) 2008 OpenWrt.org
# Copyright (C) 2017 Intel Corporation
# Copyright (C) 2020-2024 MaxLinear Inc
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.


include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=pm-util
PKG_SOURCE_VERSION:=4.4.8
PKG_SOURCE_PROTO:=git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=pm_util
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=515c21b9ecf246a8be0005de8e03c7d3373a1c69f82e31108d738e799709a5c0
PKG_RELEASE:=1
PKG_BIN_DIR=$(VENDOR_PATH)/bin
PKG_BUILD_DEPENDS:=libjson-c
PKG_INSTALL:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)
PKG_BUILD_PARALLEL:=1
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=GPLv2
PKG_MXL_LICENSE:=GPL/LICENSE
PKG_FIXUP:=autoreconf

-include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=power
  CATEGORY:=MaxLinear
  SUBMENU:=UGW Utilities
  TITLE:=Power Management Utility
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
  DEPENDS:= +libsafec3-7
  MENU:=1
endef

define Package/$(PKG_NAME)/description
	Power Management Utility supporting UGW8
	This application offers access to system power info's.
	Features:
	- Display current power state of all registered modules
	- Get module specific power state
	- Change/Set power state of a specific module
	Example: pm_util -h    "print help menu"
endef

CONFIGURE_ARGS += \
	--enable-kernelbuild="$(LINUX_DIR)" ARCH=$(LINUX_KARCH)

TARGET_CFLAGS +=  -DHAVE_C99=1 -I$(STAGING_DIR)/usr/include/safeclib/ \
		  $(if $(CONFIG_TARGET_intel_x86_lgm),-DTARGET_X86)

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/{lib,include}
	$(CP) $(PKG_BUILD_DIR)/src/include/ltq_pmutil_interface.h $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/{lib,include}
	$(CP) $(PKG_BUILD_DIR)/src/libpmutil.a $(1)/usr/lib/
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/$(PKG_BIN_DIR)
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/pm_util $(1)/$(PKG_BIN_DIR)/
	$(INSTALL_BIN) ./files/pwr_control.sh $(1)/$(PKG_BIN_DIR)/
endef

define Build/Configure
	# remove "configured" stamps of other configs
	@- rm $(PKG_BUILD_DIR)/.configured*
	$(call Build/Configure/Default)
endef

define Build/Clean
	@- rm -Rf $(PKG_BUILD_DIR)/ipkg*
	@- [ -e $(PKG_BUILD_DIR)/Makefile ] && $(MAKE) -C $(PKG_BUILD_DIR) clean
	$(call Build/Clean/Default)
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
