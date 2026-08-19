#
# Openwrt Makefile for ltq-wlan-wave_6x
####################################

# Includes
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ltq-wlan-wave_6x-uci
PKG_VERSION:=06.01.00
PKG_SOURCE_PROTO:=git
PKG_REV:=16310f3978c5eb98b1d2a933ee6a01f9867edc7c

































































PKG_BUILD_DIR:=$(BUILD_DIR)/ltq-wlan-wave_6x-uci-$(BUILD_VARIANT)/ltq-wlan-wave_6x-uci-$(PKG_VERSION)
PKG_SOURCE_VERSION:=ugw-8.5.2-cleanup
PKG_SOURCE_URL:=https://gitlab.com/prpl-foundation/intel/wave_bin.git
PKG_MIRROR_HASH:=277972bed4880098fba090661f8ff275b219ac4c6ebe8fde4e97fc98fa241854
PKG_SOURCE_PROTO:=git
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

FW_BIN_FOLDER:=$(PKG_BUILD_DIR)/fw/bin
GENRISC_BIN_FOLDER:=$(PKG_BUILD_DIR)/genrisc/bin
PROGMODEL_BIN_FOLDER:=$(PKG_BUILD_DIR)/progmodel/bin
PSD_BIN_FOLDER:=$(PKG_BUILD_DIR)/psd/bin
REGULATORY_BIN_FOLDER:=$(PKG_BUILD_DIR)/regulatory/bin

BIN_FOLDER:=opt/intel/bin
SBIN_FOLDER:=opt/intel/sbin
include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk


define Package/ltq-wlan-wave_6x-uci/default
	SECTION:=utils
	CATEGORY:=Intel
	TITLE:=Wave components
	DEPENDS:= \
		@+PACKAGE_ltq-devm:FEATURE_DEVM_WIFILAN \
		@+BUSYBOX_CONFIG_WC \
		@+BUSYBOX_CONFIG_WHICH \
		@+BUSYBOX_CONFIG_AWK \
		@+BUSYBOX_CONFIG_FEATURE_AWK_MATH \
		@+BUSYBOX_CONFIG_FIND \
		@+BUSYBOX_CONFIG_FEATURE_FIND_MTIME \
		@+BUSYBOX_CONFIG_FEATURE_FIND_PERM \
		@+BUSYBOX_CONFIG_FEATURE_FIND_TYPE \
		@+BUSYBOX_CONFIG_FEATURE_FIND_XDEV \
		@+BUSYBOX_CONFIG_FEATURE_FIND_NEWER \
		@+BUSYBOX_CONFIG_FEATURE_FIND_INUM \
		@+BUSYBOX_CONFIG_XARGS \
		@+BUSYBOX_CONFIG_LSMOD \
		@+PACKAGE_libuuid \
		@+PACKAGE_uuidgen \
		@+PACKAGE_udev
	URL:=http://www.intel.com
	MAINTAINER:=Intel Corporation
	MENU:=1
endef

define Package/ltq-wlan-wave_6x-uci/description/default
 This package contains wave binaries components
endef

define Package/ltq-wlan-wave_6x-uci
	$(Package/ltq-wlan-wave_6x-uci/default)
	DEPENDS += \
		@+PACKAGE_dwpal_6x-uci \
		@+PACKAGE_swpal_6x-uci \
		@+PACKAGE_wav_api_6x-uci \
		@+PACKAGE_iwlwav-hostap-uci \
		@+PACKAGE_iwlwav-iw \
		@+PACKAGE_iwlwav-tools \
		@+PACKAGE_iwlwav-driver-uci \
		@+PACKAGE_iwlwav-firmware
	VARIANT:=release
endef

define Package/ltq-wlan-wave_6x-uci/description
 $(call Package/ltq-wlan-wave_6x-uci/description/default) without Debug Information
endef

define Package/ltq-wlan-wave_6x-uci-debug
	$(Package/ltq-wlan-wave_6x-uci/default)
	TITLE += with debug information
	DEPENDS += \
		@+PACKAGE_dwpal_6x-uci-debug \
		@+PACKAGE_swpal_6x-uci-debug \
		@+PACKAGE_wav_api_6x-uci-debug \
		@+PACKAGE_iwlwav-hostap-uci-debug \
		@+PACKAGE_iwlwav-iw-debug \
		@+PACKAGE_iwlwav-tools-debug \
		@+PACKAGE_iwlwav-driver-uci-debug \
		@+PACKAGE_iwlwav-firmware-debug
	VARIANT:=debug
endef

define Package/ltq-wlan-wave_6x-uci-debug/description
 $(call Package/ltq-wlan-wave_6x-uci/description/default) with Debug Information
endef
#### Target Rules ##########################
define Build/Prepare
	$(call Build/Prepare/Default)
endef

define Build/Compile
endef

define Download/ltq-wlan-wave_6x-uci
endef

define Package/ltq-wlan-wave_6x-uci/install
	$(INSTALL_DIR) -p $(1)/$(BIN_FOLDER)/
	$(INSTALL_DIR) -p $(1)/$(SBIN_FOLDER)/
	$(INSTALL_DIR) -p $(1)/etc/hotplug/
	$(INSTALL_DIR) -p $(1)/etc/hotplug.d/
	$(INSTALL_DIR) -p $(1)/etc/udev/rules.d/
	$(INSTALL_DIR) -p $(1)/lib/firmware/
	$(INSTALL_DIR) -p $(1)/opt/intel/wave/conf/
	$(LN) /dev/null $(1)/etc/udev/rules.d/75-persistent-net-generator.rules
	$(CP) $(FW_BIN_FOLDER)/* $(1)/lib/firmware/
	$(CP) $(GENRISC_BIN_FOLDER)/* $(1)/lib/firmware/
	$(CP) $(PROGMODEL_BIN_FOLDER)/* $(1)/lib/firmware/
	$(CP) $(PSD_BIN_FOLDER)/* $(1)/lib/firmware/
	$(CP) $(REGULATORY_BIN_FOLDER)/* $(1)/lib/firmware/
	$(CP) ./files/binaries/sbin/* $(1)/$(SBIN_FOLDER)/
	$(CP) ./files/etc/* $(1)/etc/
	$(CP) $(PKG_BUILD_DIR)/wave_components.ver $(1)/etc/
	$(if $(WLAN_BUILD_VERSION), \
	echo -e "wave_release_minor=\"$(PKG_VERSION)$(if $(WLAN_BUILD_VERSION),.$(WLAN_BUILD_VERSION))\"" >> $(1)/etc/wave_components.ver;\
	)
endef

Package/ltq-wlan-wave_6x-uci-debug/install = $(Package/ltq-wlan-wave_6x-uci/install)

$(eval $(call BuildPackage,ltq-wlan-wave_6x-uci))
$(eval $(call BuildPackage,ltq-wlan-wave_6x-uci-debug))
