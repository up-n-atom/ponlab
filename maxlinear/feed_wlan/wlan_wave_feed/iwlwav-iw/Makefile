#
# Openwrt Makefile for iw
#
# Simplify mode 1.0
#
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=iwlwav-iw
PKG_REV:=e7e06f973cd3c1659e87fb17f9c04130e52245fb
PKG_VERSION:=4.14
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=iwlwav-iw.git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME)
PKG_SOURCE_VERSION:=r597+a+41+a+17
PKG_SOURCE_PROTO:=git
PKG_MIRROR_HASH:=fc4fb9a59c8ea32fef9e1528e0b492fd7599707f7e91f48fcc6ef96a0087d337
PKG_BUILD_DIR:=$(BUILD_DIR)/iwlwav-iw-$(BUILD_VARIANT)/iwlwav-iw-$(PKG_VERSION)
PKG_BUILD_DEPENDS:=libnl safec3 PACKAGE_iw:iw iwlwav-driver-uci

PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=ISC
PKG_LICENSE_FILES:=COPYING

include $(INCLUDE_DIR)/package.mk

define Package/iwlwav-iw
  SECTION:=net
  CATEGORY:=Network
  TITLE:=iw for Wave
  DEPENDS:=+libnl +libsafec3
  MENU:=1
  VARIANT:=release
endef

define Package/iwlwav-iw/description
  iw for Wave
endef

define Package/iwlwav-iw-debug
$(call Package/iwlwav-iw)
VARIANT:=debug
TITLE += with debug information
endef

define Package/iwlwav-iw-debug/description
$(call Package/iwlwav-iw/description) with debug information
endef

USR_INC_DIR:=$(wildcard $(STAGING_DIR)/usr/include)

IFX_CFLAGS += -I$(USR_INC_DIR) -DHAVE_C99=1
IFX_LDFLAGS += -L$(STAGING_DIR)/usr/lib -lnl-3 -lsafec-3.3
IFX_CFLAGS += -fstack-protector-strong -O2 -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security -Wall -fPIC -Wl,-strip-debug
IFX_LDFLAGS += -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro
ifeq ($(BUILD_VARIANT),debug)
IFX_CFLAGS += -DCONFIG_WAVE_DEBUG
endif

RELATIVE_NLV_PKG_HEADER_PATH:=vendor_cmds_copy.h
RELATIVE_NL_PKG_HEADER_PATH:=nl80211.h

define Build/Compile
	../iwlwav-driver-uci/files/check_shared_headers.sh $(PKG_NAME) $(PKG_VERSION) $(BUILD_DIR) $(BUILD_VARIANT) $(RELATIVE_NLV_PKG_HEADER_PATH) $(RELATIVE_NL_PKG_HEADER_PATH)
	$(MAKE) -C $(PKG_BUILD_DIR) clean
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(PKG_BUILD_DIR) \
		IFX_CFLAGS="$(IFX_CFLAGS)" IFX_LDFLAGS="$(IFX_LDFLAGS)"
endef

define Package/iwlwav-iw/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) -p $(1)/etc
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/iw $(1)/usr/sbin/
	echo -e "iwlwav_iw_hash=\"$(PKG_REV)\"\niwlwav_iw_tag=\"$(PKG_REV)\"" > $(1)/etc/iwlwav_iw.ver
endef
Package/iwlwav-iw-debug/install = $(Package/iwlwav-iw/install)

$(eval $(call BuildPackage,iwlwav-iw))
$(eval $(call BuildPackage,iwlwav-iw-debug))
