# 
# Copyright(C) 2006-2015 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=owrt-ppa-scripts
PKG_VERSION:=1.0.0
PKG_RELEASE:=1
PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=MaxLinear
PKG_MXL_LICENSE:=GPL_BSD/LICENSE

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=MaxLinear
  SUBMENU:=UGW Service Scripts
  TITLE:=OWRT PPA Scripts
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc.
  DEPENDS:=+kmod-ppa-drv
  MENU:=1
endef

define Package/${PKG_NAME}/description
 Contains PPA init and hotplug script
endef

define Build/Prepare
endef

define Build/Compile
endef

define Package/${PKG_NAME}/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/ppa.init $(1)/etc/init.d/ppa
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_DATA) ./files/ppa.defaults $(1)/etc/uci-defaults/ppa
endef

$(eval $(call BuildPackage,${PKG_NAME}))
