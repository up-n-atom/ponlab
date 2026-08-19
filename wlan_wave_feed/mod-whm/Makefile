##################################################################################
#                                                                                #
#       Copyright © 2023 MaxLinear, Inc.                                         #
#                                                                                #
#  For licensing information, see the file 'LICENSE' in the root folder of       #
#  this software module.                                                         #
##################################################################################
include $(TOPDIR)/rules.mk
-include $(STAGING_DIR)/components.config

PKG_NAME:=mod-whm
PKG_REV:=3de58086c3d129073729b356bce2497f76741169
PKG_VERSION:=v3.0
PKG_RELEASE:=$(PKG_REV)

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_REV).tar.bz2
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=mod_whm.git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME)
PKG_SOURCE_VERSION:=r597+a+41+a+17
PKG_SOURCE_PROTO:=git
PKG_MIRROR_HASH:=d07649d7eb799848e9ecfe667161fbfcaad3be2c8429c7f7b3d228b76867c6a2
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(BUILD_VARIANT)/$(PKG_NAME)-$(PKG_VERSION)
SHORT_DESCRIPTION:=Mxl vendor module for wireless hardware manager

PKG_MAINTAINER:=Maxlinear
PKG_LICENSE:=BSD-2-Clause
PKG_LICENSE_FILES:=README

PKG_BUILD_DEPENDS:=iwlwav-driver-uci dwpal_6x-uci
DRV_BUILD_DIR:=$(BUILD_DIR)/iwlwav-driver-uci-$(BUILD_VARIANT)/iwlwav-driver-uci-6.1.0

include $(INCLUDE_DIR)/package.mk

# This is required to see compilation warnings in case caused by incorrect pWHM (or other package header) macro usage
# Otherwise all headers are included as system headers, which silences all the warnings
unexport STAGING_DIR

# Define which custom vendor odl variant to install
ifeq ($(BUILD_VARIANT),debug)
	CUSTOM_VENDOR_ODL:=13_wld-custom-vendor-debug.odl.uc
	DELETE_VARIANT_ODL:=*release.odl.uc
else
	CUSTOM_VENDOR_ODL:=13_wld-custom-vendor-release.odl.uc
	DELETE_VARIANT_ODL:=*debug.odl.uc
endif

ifeq ($(CONFIG_DEFAULT_WIRELESS_CONFIGURATION_TYPE),"mesh")
	CUSTOM_VAPS_ODL:=32_wld-custom-vaps-mesh.odl.uc
	CUSTOM_RADS_ODL:=22_wld-custom-rads-mesh.odl.uc
	DELETE_ODLS:=*gw.odl.uc
else # default is CONFIG_DEFAULT_WIRELESS_CONFIGURATION_TYPE="gw"
	CUSTOM_VAPS_ODL:=32_wld-custom-vaps-gw.odl.uc
	CUSTOM_RADS_ODL:=22_wld-custom-rads-gw.odl.uc
	DELETE_ODLS:=*mesh.odl.uc
endif

define Package/$(PKG_NAME)
  CATEGORY:=amx
  SUBMENU:=Modules
  TITLE:=$(SHORT_DESCRIPTION)
  URL:=git@ilbitbucket.phoenix.local:29418/sw_wave/mod_whm.git
  DEPENDS += +libsahtrace
  DEPENDS += +libswlc
  DEPENDS += +libswla
  DEPENDS += +pwhm
  DEPENDS += +libamxc
  DEPENDS += +libamxm
  DEPENDS += +libamxp
  DEPENDS += +libamxd
  DEPENDS += +libamxb
  DEPENDS += +libamxo
  MENU:=1
  VARIANT:=release
endef

define Package/$(PKG_NAME)/description
	Maxlinear vendor module for wireless hardware manager
endef

define Package/$(PKG_NAME)/config
source "$(SOURCE)/Config.in"
endef

define Package/$(PKG_NAME)-debug
$(call Package/$(PKG_NAME))
VARIANT:=debug
TITLE += with debug information
endef

define Package/$(PKG_NAME)-debug/description
$(call Package/$(PKG_NAME)/description) with debug information
endef

define Build/Configure
	if [ -e files/$(PKG_NAME).cfg ]; then \
	  $(STAGING_DIR)/generate_sah_build_files.sh $(PKG_NAME); \
	fi
endef

define Build/Compile
	$(call Build/Compile/Default, STAGINGDIR=$(STAGING_DIR) CONFIGDIR=$(STAGING_DIR) PKG_CONFIG_PATH=$(STAGING_DIR)/usr/lib/pkgconfig RAW_VERSION=$(PKG_VERSION) HARDCO_HAL_DIR=$(STAGING_DIR)/usr/include DRV_BUILD_DIR=$(DRV_BUILD_DIR) DWPAL_6X_DIR=$(DWPAL_6X_DIR) CONFIG_MOD_WHM_SENSING_SOCKET_PATH=$(CONFIG_MOD_WHM_SENSING_SOCKET_PATH) CONFIG_MXL_WLAN_OSS_BUILD=$(CONFIG_MXL_WLAN_OSS_BUILD))
endef

define Build/Install
	$(call Build/Install/Default, install INSTALL=install D=$(PKG_INSTALL_DIR) DEST=$(PKG_INSTALL_DIR) CONFIGDIR=$(STAGING_DIR) PV=$(PKG_VERSION) PKG_CONFIG_LIBDIR=$(STAGING_DIR)/usr/lib/pkgconfig LIBDIR=/usr/lib INSTALL_LIB_DIR=/lib INSTALL_BIN_DIR=/bin RAW_VERSION=$(PKG_VERSION) HARDCO_HAL_DIR=$(STAGING_DIR)/usr/include DRV_BUILD_DIR=$(DRV_BUILD_DIR) CONFIG_MXL_WLAN_OSS_BUILD=$(CONFIG_MXL_WLAN_OSS_BUILD))
	find $(PKG_INSTALL_DIR) -name *.a -exec rm {} +;
	find $(PKG_INSTALL_DIR) -name *.h -exec rm {} +;
	find $(PKG_INSTALL_DIR) -name *.pc -exec rm {} +;
endef

define Build/InstallDev
	$(call Build/Install/Default, install INSTALL=install D=$(STAGING_DIR) DEST=$(STAGING_DIR) CONFIGDIR=$(STAGING_DIR) PV=$(PKG_VERSION) PKG_CONFIG_LIBDIR=$(STAGING_DIR)/usr/lib/pkgconfig LIBDIR=/usr/lib INSTALL_LIB_DIR=/lib INSTALL_BIN_DIR=/bin RAW_VERSION=$(PKG_VERSION) HARDCO_HAL_DIR=$(STAGING_DIR)/usr/include DRV_BUILD_DIR=$(DRV_BUILD_DIR) CONFIG_MXL_WLAN_OSS_BUILD=$(CONFIG_MXL_WLAN_OSS_BUILD))
endef

define Package/$(PKG_NAME)/install
	$(CP) $(PKG_INSTALL_DIR)/* $(1)/
	mv $(1)/etc/amx/wld/wld_defaults/$(CUSTOM_VAPS_ODL)  $(1)/etc/amx/wld/wld_defaults/32_wld-custom-vaps.odl.uc
	mv $(1)/etc/amx/wld/wld_defaults/$(CUSTOM_RADS_ODL)  $(1)/etc/amx/wld/wld_defaults/22_wld-custom-rads.odl.uc
	mv $(1)/etc/amx/wld/wld_defaults/$(CUSTOM_VENDOR_ODL)  $(1)/etc/amx/wld/wld_defaults/13_wld-custom-vendor.odl.uc
	rm $(1)/etc/amx/wld/wld_defaults/$(DELETE_ODLS)
	rm $(1)/etc/amx/wld/wld_defaults/$(DELETE_VARIANT_ODL)
	if [ -d ./files ]; then \
		$(CP) ./files/* $(1)/; \
	fi
endef

Package/$(PKG_NAME)-debug/install = $(Package/$(PKG_NAME)/install)

$(eval $(call BuildPackage,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)-debug))
