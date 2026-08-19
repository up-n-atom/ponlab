#
# Openwrt Makefile for hostapd
#
# Simplify mode 1.0
#
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=iwlwav-hostap-uci
PKG_REV:=aff2fc6ddc9646621f53b4097d75f5e7455ab4c0
PKG_VERSION:=2.8
PKG_RELEASE:=1
PKG_SOURCE:=$(PKG_NAME)-$(PKG_REV).tar.gz
PKG_SOURCE_URL:=https://gitlab.com/prpl-foundation/intel/iwlwav-hostap/-/archive/$(PKG_REV)
PKG_HASH:=f9e0b77bce83aeff16a10e61cde3ce563b1ba9f01f7f0e6b85d8fb400a366ebc

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(BUILD_VARIANT)/iwlwav-hostap-$(PKG_REV)

PKG_CONFIG_DEPENDS:=CONFIG_WIRELESS_STA
PKG_BUILD_DEPENDS:=openssl libnl safec3

include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk

define Package/iwlwav-hostap-uci
  SECTION:=net
  CATEGORY:=Network
  TITLE:=hostapd for Wave
  DEPENDS:=+libopenssl +librt +libnl +safeclibs3
  MENU:=1
  PROVIDES:=iwlwav-hostap
  VARIANT:=release
endef

define Package/iwlwav-hostap-uci/description
  IEEE 802.11 AP, IEEE 802.1X/WPA/WPA2/EAP/RADIUS Authenticator
endef
define Package/iwlwav-hostap-uci/config
$(call Package/iwlwav-hostap-uci/override_version)
$(call Package/iwlwav-hostap-uci/override_source_path)
    config WIRELESS_STA
      depends on PACKAGE_iwlwav-hostap-uci
      bool 'Linux WPA/WPA2/IEEE 802.1X Supplicant support'
      help
          Linux WPA/WPA2/IEEE 802.1X Supplicant
endef

define Package/iwlwav-hostap-uci-debug
$(call Package/iwlwav-hostap-uci)
VARIANT:=debug
TITLE += with debug information
endef

define Package/iwlwav-hostap-uci-debug/description
$(call Package/iwlwav-hostap-uci/description) with debug information
endef

define Package/iwlwav-hostap-uci-debug/config
$(call Package/iwlwav-hostap-uci-debug/override_version)
$(call Package/iwlwav-hostap-uci-debug/override_source_path)
    config WIRELESS_STA-debug
      depends on PACKAGE_iwlwav-hostap-uci-debug
      bool 'Linux WPA/WPA2/IEEE 802.1X Supplicant support with debug'
      help
          Linux WPA/WPA2/IEEE 802.1X Supplicant with Debug information
endef
OPENSSL_DIR:=$(wildcard $(BUILD_DIR)/openssl-*)
LIBNL_DIR:=$(wildcard $(BUILD_DIR)/libnl-3*)
LIBRT_DIR:=$(TOOLCHAIN_DIR)

USR_INC_DIR:=$(wildcard $(STAGING_DIR)/usr/include)

IFX_CFLAGS += -I$(USR_INC_DIR) -I$(OPENSSL_DIR)/include -I$(LIBNL_DIR)/include -DHAVE_C99=1
IFX_LDFLAGS += -L$(STAGING_DIR)/usr/lib -L$(OPENSSL_DIR)/ -L$(LIBNL_DIR)/lib/.libs -lnl-3 -L$(LIBRT_DIR)/lib -lrt -lsafec-3.3
IFX_CFLAGS += -fstack-protector-strong -O2 -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security -Wall -Wtype-limits -fPIE -Wl,-strip-debug
IFX_LDFLAGS += -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro -pie
ifeq ($(BUILD_VARIANT),debug)
IFX_CFLAGS += -DCONFIG_WAVE_DEBUG #-DCONFIG_NO_HOSTAPD_LOGGER
CONFIG_SUFFIX =-debug
endif

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/src clean
	$(CP) $(PKG_BUILD_DIR)/hostapd/defconfig-$(BUILD_VARIANT) $(PKG_BUILD_DIR)/hostapd/.config
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(PKG_BUILD_DIR)/hostapd \
		EXTRA_CFLAGS="$(IFX_CFLAGS)" LDFLAGS="$(IFX_LDFLAGS)"
	$(if $(CONFIG_WIRELESS_STA), \
		$(MAKE) -C $(PKG_BUILD_DIR)/src clean; \
		$(CP) $(PKG_BUILD_DIR)/wpa_supplicant/defconfig-$(BUILD_VARIANT) $(PKG_BUILD_DIR)/wpa_supplicant/.config; \
		$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(PKG_BUILD_DIR)/wpa_supplicant \
		    EXTRA_CFLAGS="$(IFX_CFLAGS)" LDFLAGS="$(IFX_LDFLAGS)" \
	)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/src/common/wpa_ctrl.h $(1)/usr/include
endef

define Package/iwlwav-hostap-uci/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) -p $(1)/etc
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/hostapd/hostapd $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/hostapd/hostapd_cli $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/hostapd/hostapd.eap_user $(1)/usr/sbin
	$(if $(CONFIG_WIRELESS_STA), \
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/wpa_supplicant/wpa_supplicant $(1)/usr/sbin; \
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/wpa_supplicant/wpa_cli $(1)/usr/sbin; \
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/wpa_supplicant/wpa_passphrase $(1)/usr/sbin \
	)
	echo -e "iwlwav_hostap_hash=\"$(PKG_REV)\"\niwlwav_hostap_tag=\"$(PKG_REV)\"" > $(1)/etc/iwlwav_hostap.ver
endef
Package/iwlwav-hostap-uci-debug/install = $(Package/iwlwav-hostap-uci/install)

$(eval $(call BuildPackage,iwlwav-hostap-uci))
$(eval $(call BuildPackage,iwlwav-hostap-uci-debug))
