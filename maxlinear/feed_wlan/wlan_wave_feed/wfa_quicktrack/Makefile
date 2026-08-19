#
# Openwrt Makefile for wfa_quicktrack
#

#### Includes ###################################
include $(TOPDIR)/rules.mk

#### Package Info ###############################
PKG_NAME:=wfa_quicktrack
PKG_REV:=8e801e46a2f4d0ff705612be321b6eb914a0a093
PKG_VERSION:=1.0.6
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=wfa_quicktrack.git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME)
PKG_SOURCE_VERSION:=r597+a+41+a+17
PKG_SOURCE_PROTO:=git
PKG_MIRROR_HASH:=1e604facaa31be39e5f0e9980cbc4b06b248d7c4c019da0258e9938f3e456834
PKG_BUILD_DIR:=$(BUILD_DIR)/${PKG_NAME}-$(BUILD_VARIANT)/wfa_quicktrack-$(PKG_VERSION)

PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=WFA
PKG_LICENSE_FILES:=README.md

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=ControlAppC from Wi-Fi Alliance QuickTrack for WAV
  MAINTAINER:=MaxLinear Inc
  MENU:=1
  VARIANT:=release
endef

define Package/$(PKG_NAME)/description
	ControlAppC: DUT side agent used with WFA Quicktrack platform for certification testing
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

IFX_CFLAGS += -I$(STAGING_DIR)/usr/include/
IFX_CFLAGS += -DHAVE_C99=1
IFX_LDFLAGS += -L$(STAGING_DIR)/usr/lib
# skip error on warnings till wfa_quicktrack will be updated
IFX_CFLAGS += -fstack-protector-strong -O2 -Wl,-strip-debug
IFX_LDFLAGS += -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro

ifeq ($(BUILD_VARIANT),debug)
IFX_CFLAGS += -DCONFIG_WAVE_DEBUG
endif

ifeq ($(CONFIG_BUILD_SDK),"prplos")
IFX_CFLAGS += -DUPDK
endif

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) clean
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(PKG_BUILD_DIR) \
		EXTRA_CFLAGS="$(IFX_CFLAGS)" IFX_LDFLAGS="$(IFX_LDFLAGS)"
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/app $(1)/usr/sbin/wfa_quicktrack
	chmod 555 $(1)/usr/sbin/wfa_quicktrack

	$(INSTALL_DIR) $(1)/opt/intel/wave/scripts/QuickTrack/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mxl_files/* $(1)/opt/intel/wave/scripts/QuickTrack/
endef

Package/$(PKG_NAME)-debug/install = $(Package/$(PKG_NAME)/install)

$(eval $(call BuildPackage,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)-debug))
