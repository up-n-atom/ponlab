#
# Copyright (C) 2024 MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=pon-omci-files
PKG_RELEASE:=1
PKG_BIN_DIR=usr/bin
PKG_LICENSE:=MaxLinear
PKG_LICENSE_FILES:=LICENSE
PKG_CONFIG_DEPENDS+= \
	CONFIG_GPON_OMCI_ONU_LCT_DISABLE

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)/conffiles
/etc/config/omci
endef

define Package/$(PKG_NAME)
  SECTION:=driver
  CATEGORY:=MaxLinear
  TITLE:= GPON OMCI config and script files for PON
  URL:=https://www.maxlinear.com
  MAINTAINER:=MaxLinear
  SUBMENU:=PON
  MENU:=1
endef

define Package/$(PKG_NAME)/description
   GPON OMCI config and script files
endef

define Package/$(PKG_NAME)/config
  menu "configuration"
    depends on PACKAGE_$(PKG_NAME)
    config GPON_OMCI_ONU_LCT_DISABLE
      bool "Keep LCT port disabled during init"
      help
        If an LCT port is configured by OMCI, it is normally enabled by default.
        This option will change the UCI config to set it to disabled.
        It can be changed in the UCI config later.
  endmenu
endef

define Build/Compile
endef

LCT_DISABLE=$(if $(CONFIG_GPON_OMCI_ONU_LCT_DISABLE),1,0)

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/$(PKG_BIN_DIR)
	$(INSTALL_BIN) ./files/usr/bin/omci_hook.sh $(1)/$(PKG_BIN_DIR)
	$(INSTALL_BIN) ./files/usr/bin/omci_simulate.sh $(1)/$(PKG_BIN_DIR)
	$(INSTALL_DIR) $(1)/etc/
	$(CP) ./files/etc/* $(1)/etc/
	$(SED) 's,$$$$LCT_DIS,$(LCT_DISABLE),g' $(1)/etc/config/omci
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
