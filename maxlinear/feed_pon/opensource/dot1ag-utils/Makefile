#
# Copyright (C) 2016 Lantiq Beteiligungs-GmbH & Co. KG
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=dot1ag-utils
PKG_VERSION:=1.0.1
PKG_RELEASE:=1
PKG_LICENSE:=BSD-2-Clause
PKG_LICENSE_FILES:=LICENSE

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/vnrick/dot1ag-utils.git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=01a24e5de960318c4cc8a97212cc33b38d19e3e2
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz
PKG_MIRROR_MD5SUM:=37b08a036e932ffe7fa7ae7f114abae7

PKG_INSTALL=1

include $(INCLUDE_DIR)/package.mk

define Package/dot1ag-utils
	SECTION:=net
	CATEGORY:=Network
	DEPENDS:=+librt +libpcap
	TITLE:=Ethernet OAM (IEEE 802.1ag) utility
	URL:=https://github.com/vnrick/dot1ag-utils
endef

define Package/dot1ag-utils/description
	Ethernet OAM (IEEE 802.1ag) utility
endef

define Package/dot1ag-utils/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin
endef

$(eval $(call BuildPackage,dot1ag-utils))
