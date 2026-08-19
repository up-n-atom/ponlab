#
# Openwrt Makefile for ltq-wlan-wave_6x
####################################

# Includes
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=iwlwav-firmware
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=wave_fw_bin.git
PKG_SOURCE_PROTO:=git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME)
PKG_VERSION:=6f5afc
PKG_SOURCE_VERSION:=wlan_7.2.60_updk@r597+a+41+a+17-6f5afc
PKG_MIRROR_HASH:=33a30fb0917d288c05ebf0c0c561a93f4ee201a16a81a75bf1c4ca1fca6d5748
#package_source
# fw source url needed for gci automation
FW_SOURCE_NAME:=fw_src.git
FW_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(FW_SOURCE_NAME)
PKG_BUILD_DIR:=$(BUILD_DIR)/iwlwav-firmware-$(BUILD_VARIANT)/iwlwav-firmware-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=iwlwav-firmware-$(PKG_VERSION)

PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=MaxLinear
PKG_LICENSE_FILES:=LICENSE

FW_BIN_FOLDER:=$(PKG_BUILD_DIR)/fw/bin

BIN_FOLDER:=opt/intel/bin
SBIN_FOLDER:=opt/intel/sbin
include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk


define Package/iwlwav-firmware/default
	SECTION:=utils
	CATEGORY:=MaxLinear
	TITLE:=Wave firmware binaries
	URL:=http://www.maxlinear.com
	MAINTAINER:=MaxLinear Inc
	MENU:=1
endef

define Package/iwlwav-firmware/description/default
	This package contains wave fw binaries components
endef

define Package/iwlwav-firmware
	$(Package/iwlwav-firmware/default)
	VARIANT:=release
endef

define Package/iwlwav-firmware/description
 $(call Package/iwlwav-firmware/description/default) without Debug Information
endef

define Package/iwlwav-firmware-debug
	$(Package/iwlwav-firmware/default)
	VARIANT:=debug
endef

define Package/iwlwav-firmware-debug/description
 $(call Package/iwlwav-firmware/description/default) with Debug Information
endef


#### Target Rules ##########################
define Build/Prepare
	$(call Build/Prepare/Default)
	# Set PKG_SOURCE_PROTO to git when working with git and unset
	# when working with NFS / Artifactory
	$(if $(PKG_SOURCE_PROTO),,mv $(PKG_BUILD_DIR)/../fw/ $(PKG_BUILD_DIR)/)	
endef

define Build/Compile
endef

define Download/iwlwav-firmware
endef

define Package/iwlwav-firmware/install
	$(INSTALL_DIR) -p $(1)/$(BIN_FOLDER)/
	$(INSTALL_DIR) -p $(1)/$(SBIN_FOLDER)/
	$(INSTALL_DIR) -p $(1)/etc/
	$(INSTALL_DIR) -p $(1)/lib/firmware/
	$(INSTALL_DIR) -p $(1)/opt/intel/wave/conf/
	$(CP) $(FW_BIN_FOLDER)/* $(1)/lib/firmware/
	$(CP) $(FW_BIN_FOLDER)/fw_scd_file.scd $(1)/lib/firmware/
	echo -e "iwlwav_firmware_tag=\"$(PKG_VERSION)\"" > $(1)/etc/iwlwav_firmware.ver
endef

Package/iwlwav-firmware-debug/install = $(Package/iwlwav-firmware/install)

$(eval $(call BuildPackage,iwlwav-firmware))
$(eval $(call BuildPackage,iwlwav-firmware-debug))
