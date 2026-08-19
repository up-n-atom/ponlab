#
# Openwrt Makefile for afcd
#
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ###############################
PKG_NAME:=afcd
PKG_REV:=2871574f4f2fe1604a695a64b637b20f4e50aa9c
PKG_VERSION:=1.0
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=afcd.git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME)
PKG_SOURCE_VERSION:=r597+a+41+a+17
PKG_SOURCE_PROTO:=git
PKG_MIRROR_HASH:=31cb96ead7714ff53a450df98b16973aed1ef579a4276c623a48c86cd5b47068
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_BUILD_DEPENDS:=iwlwav-driver-uci iwlwav-driver-uci-debug
PKG_CONFIG_DEPENDS:=CONFIG_LIBCURL_OPENSSL CONFIG_LIBCURL_SSH2 CONFIG_LIBCURL_VERBOSE CONFIG_PACKAGE_cJSON

PKG_MAINTAINER:=Maxlinear
PKG_LICENSE:=BSD-2-Clause
PKG_LICENSE_FILES:=LICENSE
PKG_MXL_LICENSE:=MaxLinear/LICENSE

include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk

define Package/$(PKG_NAME)
	SECTION:=net
	CATEGORY:=Network
	TITLE:=AFC Daemon
	DEPENDS:=+libcurl +libopenssl +libnl +cJSON
	MAINTAINER:=MaxLinear Inc
	MENU:=1
	PROVIDES:=$(PKG_NAME)
endef

define Package/$(PKG_NAME)/description
	AFC daemon
endef

PKG_VARIANT_SUFFIX=
ifeq ($(BUILD_VARIANT),debug)
  PKG_VARIANT_SUFFIX=-debug
endif

define Package/$(PKG_NAME)-debug
$(call Package/$(PKG_NAME))
VARIANT:=debug
TITLE += with debug information
endef

define Package/$(PKG_NAME)-debug/description
$(call Package/$(PKG_NAME)/description) with debug information
endef

LIBCURL_DIR:=$(wildcard $(BUILD_DIR)/libcurl-*)
CJSON_DIR:=$(wildcard $(BUILD_DIR)/cJSON-*)
USR_INC_DIR:=$(wildcard $(STAGING_DIR)/usr/include)
IFX_CFLAGS += -I$(USR_INC_DIR) -I$(LIBCURL_DIR)/include -DHAVE_C99=1
IFX_LDFLAGS += -L$(STAGING_DIR)/usr/lib -L$(LIBCURL_DIR)/lib -lcurl -lnl-3 -L$(LIBRT_DIR)/lib -lrt
IFX_CFLAGS += -fstack-protector-strong -O2 -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security -Wall -Wtype-limits -fPIE -Wl,-strip-debug
IFX_LDFLAGS += -L$(STAGING_DIR)/usr/lib -L$(CJSON_DIR) -lcjson
IFX_LDFLAGS += -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro -fpie

RELATIVE_NLV_PKG_HEADER_PATH:=drivers/vendor_cmds_copy.h
RELATIVE_NL_PKG_HEADER_PATH:=drivers/nl80211.h

define Build/Compile
	../iwlwav-driver-uci/files/check_shared_headers.sh $(PKG_NAME) $(PKG_VERSION) $(BUILD_DIR) $(BUILD_VARIANT) $(RELATIVE_NLV_PKG_HEADER_PATH) $(RELATIVE_NL_PKG_HEADER_PATH)
	$(MAKE) -C $(PKG_BUILD_DIR)/ clean
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(PKG_BUILD_DIR)/ \
		EXTRA_CFLAGS="$(IFX_CFLAGS)" LDFLAGS="$(IFX_LDFLAGS)"
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) -p $(1)/etc
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/afcd $(1)/usr/sbin/afcd_legacy
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/afcd_cli $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/afc_config.conf $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/certs
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/afc_ca.pem $(1)/etc/certs
endef

Package/$(PKG_NAME)-debug/install = $(Package/$(PKG_NAME)/install)

$(eval $(call BuildPackage,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)-debug))
