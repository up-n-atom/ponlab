#
# Copyright (c) 2020, MaxLinear, Inc.
# Copyright (C) 2017 Intel Corporation
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: $

include $(TOPDIR)/rules.mk

PKG_NAME:=owrt-sys-scripts
PKG_VERSION:=1.0.2.1
PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=MaxLinear
PKG_MXL_LICENSE:=MaxLinear/LICENSE
include $(INCLUDE_DIR)/package.mk

define Package/${PKG_NAME}
        SECTION:=utils
        CATEGORY:=MaxLinear
        SUBMENU:=UGW Service Scripts
        TITLE:=MaxLinear system scripts
        URL:=http://www.maxlinear.com
        MAINTAINER:=MaxLinear Inc
        MENU:=1
endef

define Package/$(PKG_NAME)/description
 OWRT sys scripts
endef

define Build/Prepare
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/etc/init.d; \
	$(INSTALL_BIN) ./files/swconfig.init $(1)/etc/init.d/swconfig.sh; \

	$(INSTALL_DIR) $(1)/etc/uci-defaults; \
	$(INSTALL_BIN) ./files/uhttpd.defaults $(1)/etc/uci-defaults/uhttpd; \
	$(INSTALL_BIN) ./files/users.defaults $(1)/etc/uci-defaults/users; \
	$(INSTALL_BIN) ./files/ethernet.defaults $(1)/etc/uci-defaults/ethernet; \
	$(INSTALL_BIN) ./files/firewall.defaults $(1)/etc/uci-defaults/firewall; \

	$(INSTALL_DIR) $(1)/etc/hotplug.d/iface; \
	$(INSTALL_BIN) ./files/10-internet-led.hotplug $(1)/etc/hotplug.d/iface/10-internet-led; \
	$(INSTALL_BIN) ./files/30-tunnels.hotplug $(1)/etc/hotplug.d/iface/30-tunnels; \

	$(INSTALL_DIR) $(1)/sbin; \
	$(INSTALL_BIN) ./files/sysupgrade $(1)/sbin/sysupgrade; \

	$(INSTALL_DIR) $(1)/usr/sbin; \
	$(INSTALL_BIN) ./files/factorycfg.sh $(1)/usr/sbin/factorycfg.sh; \
	$(INSTALL_BIN) ./files/version.sh $(1)/usr/sbin/version.sh; \
	$(INSTALL_BIN) ./files/gre.sh $(1)/usr/sbin/gre.sh; \
	$(INSTALL_BIN) ./files/l2tp.sh $(1)/usr/sbin/l2tp.sh; \

	$(INSTALL_DIR) $(1)/$(VENDOR_PATH)/etc; \
	$(CP) files/uci_to_ipsec_config_owrt.sh $(1)/$(VENDOR_PATH)/etc/; \
	$(CP) files/ipsec_sec_config.sh $(1)/$(VENDOR_PATH)/etc/; \

	$(INSTALL_DIR) $(1)/etc/config; \
	$(CP) files/ipsec.config $(1)/etc/config/ipsec

endef

$(eval $(call BuildPackage,$(PKG_NAME)))
