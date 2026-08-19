#
# Copyright (C) 2006-2010 OpenWrt.org
# Copyright © 2020-2023 MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=lib-cli
PKG_RELEASE:=1
PKG_BASE_NAME:=lib_cli
PKG_VERSION:=2.9.0
PKG_SOURCE_PROTO:=git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=lib_cli
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)
PKG_MIRROR_HASH:=a675ce066f1960f6c7e562fd143b6204d042057a9dd90eaadd9d957dcd778af0

PKG_FIXUP:=libtool
PKG_INSTALL:=1
PKG_LICENSE:=GPL-2.0-only BSD-2-Clause
PKG_LICENSE_FILES:=LICENSE

#PKG_IS_KERNEL_MODULE:=y
-include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

ifeq ($(DUMP),)
	STAMP_CONFIGURED:=$(strip $(STAMP_CONFIGURED))_$(shell $(SH_FUNC) grep '^CONFIG_COMMON_CLI_' $(TOPDIR)/.config | md5s)
endif

define Package/$(PKG_NAME)
  SECTION:=driver
  CATEGORY:=MaxLinear
  TITLE:= CLI Library
  URL:=http://www.maxlinear.com/
  MAINTAINER:=MaxLinear, Inc.
  DEPENDS:=+ifxos +libpthread
  MENU:=1
  SUBMENU:=CLI
endef

define Package/$(PKG_NAME)/description
 The common command line interface (CLI) library.
endef

define Package/$(PKG_NAME)/config
	menu "configuration"
	depends on PACKAGE_$(PKG_NAME)
		source "$(SOURCE)/config/Config.in"
	endmenu
   $(call Package/$(PKG_NAME)/override_source_path)
endef

# make the submake silent:
MAKE_FLAGS += -s

CONFIGURE_ARGS += \
	--enable-kernelincl="$(LINUX_DIR)/include" \
	--enable-ifxos-include="-I$(STAGING_DIR)/usr/include/ifxos" \
	--enable-ifxos-library="-L$(STAGING_DIR)/usr/lib" \
	--enable-cli-file-pout \
	$(call autoconf_bool,CONFIG_COMMON_CLI_ERROR_PRINTS,error-print) \
	$(call autoconf_bool,CONFIG_COMMON_CLI_DEBUG_PRINTS,debug-print) \
	$(call autoconf_bool,CONFIG_COMMON_CLI_SUPPORT_BUFFER_OUT,cli-buffer-pout) \
	$(call autoconf_bool,CONFIG_COMMON_CLI_SUPPORT_CONSOLE,cli-console) \
	$(call autoconf_bool,CONFIG_COMMON_CLI_SUPPORT_PIPE,cli-pipe) \
	$(call autoconf_bool,CONFIG_COMMON_CLI_EXAMPLE,build-example)

ifeq ($(CONFIG_COMMON_CLI_SUPPORT_AUTO_COMPLETION),y)
TARGET_CFLAGS += -DCLI_SUPPORT_AUTO_COMPLETION=1
else
TARGET_CFLAGS += -DCLI_SUPPORT_AUTO_COMPLETION=0
endif

define KernelPackage/$(PKG_NAME)
  SUBMENU:=MaxLinear
  DEPENDS:=+kmod-ifxos
  TITLE:=CLI Module (kernel space)
  URL:=http://www.maxlinear.com/
  MAINTAINER:=MaxLinear, Inc.
  FILES:=$(PKG_BUILD_DIR)/src/mod_cli$(PKG_MODULE_EXTENSION)
  AUTOLOAD:=$(call AutoLoad,15,drv_ifxos mod_cli)
endef

define KernelPackage/$(PKG_NAME)/description
  The common command line interface (CLI) kernel module.
endef

define Build/Clean
	@- rm -Rf $(PKG_BUILD_DIR)/ipkg*
	@- [ -e $(PKG_BUILD_DIR)/Makefile ] && $(MAKE) -C $(PKG_BUILD_DIR) clean
	$(call Build/Clean/Default)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/cli/
	$(CP) $(PKG_INSTALL_DIR)/usr/include/cli/* $(1)/usr/include/cli/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libcli.* $(1)/usr/lib/
endef

define Build/UninstallDev
	- rm -Rf $(1)/usr/include/cli
	- rm -f $(1)/usr/lib/libcli.*
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libcli.so* $(1)/usr/lib/
ifeq ($(CONFIG_COMMON_CLI_EXAMPLE),y)
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/cli_example $(1)/usr/bin/
endif
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
$(eval $(call KernelPackage,$(PKG_NAME)))
