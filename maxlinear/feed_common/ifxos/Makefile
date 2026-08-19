#
# Copyright (C) 2009-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ifxos
PKG_BASE_NAME:=lib_ifxos
PKG_VERSION:=1.7.2
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE:=$(PKG_BASE_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_PROTO).tar.gz
PKG_RELEASE:=1
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=lib_ifxos
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)/$(PKG_NAME)-$(PKG_VERSION)
PKG_MIRROR_HASH:=51ec7f453fdc55494eddbaca9f1e4fe45db0d1d56abfde4946972a783b5ad20e

PKG_FIXUP:=libtool
PKG_INSTALL:=1
PKG_CONFIG_DEPENDS:=CONFIG_IFXOS_SHARED_LIB
PKG_LICENSE:=GPL-2.0-only BSD-2-Clause
PKG_LICENSE_FILES:=LICENSE

-include $(INCLUDE_DIR)/package-version-override.mk

# re-define PATCH_DIR to get version-specific patches
PATCH_DIR:=./patches-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

ifeq ($(DUMP),)
	STAMP_CONFIGURED:=$(strip $(STAMP_CONFIGURED))_$(shell $(SH_FUNC) grep '^CONFIG_PACKAGE_.*ifxos' $(TOPDIR)/.config | md5s)
endif

define Package/$(PKG_NAME)
  SECTION:=libs
  CATEGORY:=MaxLinear
  SUBMENU:=IFXOS
  TITLE:=OS abstraction library (Linux)
  URL:=http://www.maxlinear.com/
  MAINTAINER:=MaxLinear, Inc.
  DEPENDS:=+libpthread +librt
  MENU:=1
endef

define Package/$(PKG_NAME)/config
$(call Package/$(PKG_NAME)/override_source_path)
	menu "configuration"
	depends on PACKAGE_$(PKG_NAME)
	source "$(SOURCE)/config/Config.in"
	endmenu
endef

define Package/$(PKG_NAME)/description
  This library will be used by some MaxLinear applications / drivers under Linux
endef

define KernelPackage/$(PKG_NAME)
  SUBMENU:=MaxLinear
  TITLE:=MaxLinear OS abstraction library
  URL:=http://www.maxlinear.com/
  MAINTAINER:=MaxLinear, Inc.
  FILES:=$(PKG_INSTALL_DIR)/usr/bin/drv_ifxos$(PKG_MODULE_EXTENSION)
  AUTOLOAD:=$(call AutoLoad,10,drv_ifxos)
endef

MAKE_FLAGS+=-s
TARGET_CFLAGS += $(FPIC)

ifeq ($(CONFIG_IFXOS_IPV6_SUPPORT),y)
IFXOS_IPV6_SUPPORT:=--enable-ipv6-drv
else
IFXOS_IPV6_SUPPORT:=--disable-ipv6-drv
endif

CONFIGURE_ARGS += \
	$(IFXOS_IPV6_SUPPORT)

ifneq ($(CONFIG_PACKAGE_kmod-ifxos),)
CONFIGURE_ARGS += --with-kernel-module
CONFIGURE_ARGS += --enable-kernelincl="$(LINUX_DIR)/include"

ifeq ($(CONFIG_LINUX_2_4),)
CONFIGURE_ARGS += --enable-linux-26 --enable-kernelbuild="$(LINUX_DIR)" ARCH=$(LINUX_KARCH)
PKG_MODULE_EXTENSION:=.ko
endif

else
CONFIGURE_ARGS += --without-kernel-module
endif

ifeq ($(CONFIG_LINUX_2_4),y)

ifneq ($(CONFIG_mips)$(CONFIG_mipsel),)
CONFIGURE_ARGS += --enable-add_drv_cflags="-fno-pic -mno-abicalls -mlong-calls -G 0"
endif

define KernelPackage/$(PKG_NAME)/install
	cd $(1)/lib/modules/$(LINUX_VERSION) && ln -snf drv_ifxos drv_ifxos.o
endef

endif

define Build/Clean
	@- rm -Rf $(PKG_BUILD_DIR)/ipkg*
	@- [ -e $(PKG_BUILD_DIR)/Makefile ] && $(MAKE) -C $(PKG_BUILD_DIR) clean
	$(call Build/Clean/Default)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/{lib,include/ifxos}
	$(CP) $(PKG_BUILD_DIR)/src/include/* $(1)/usr/include/ifxos

	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libifxos.a $(1)/usr/lib/libifxos.a
ifeq ($(CONFIG_IFXOS_SHARED_LIB),y)
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libifxos.so* $(1)/usr/lib/
endif

	$(INSTALL_DIR) $(1)/modulesym
	- $(CP) $(PKG_BUILD_DIR)/src/Module.symvers $(1)/modulesym/$(PKG_NAME).symvers
endef

define Build/UninstallDev
	- rm -Rf $(1)/usr/include/ifxos
	- rm -f $(1)/usr/lib/libifxos*
endef

define Package/$(PKG_NAME)/install
#
ifeq ($(CONFIG_IFXOS_SHARED_LIB),y)
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libifxos.so* $(1)/usr/lib/
endif
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
$(eval $(call KernelPackage,$(PKG_NAME)))
