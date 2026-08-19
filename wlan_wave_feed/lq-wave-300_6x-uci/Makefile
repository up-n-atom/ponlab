#
# Openwrt Makefile for ltq-wlan-wave_6x
####################################

# Includes
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ltq-wlan-wave_6x-uci
PKG_VERSION:=06.01.00
PKG_REV:=06555aeff884a1e82cdc7ab90d76b3971d91cbcc
PKG_BUILD_DIR:=$(BUILD_DIR)/ltq-wlan-wave_6x-uci-$(BUILD_VARIANT)/ltq-wlan-wave_6x-uci-$(PKG_VERSION)
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=wave_bin.git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME)
PKG_SOURCE_VERSION:=r597+a+41+a+17
PKG_SOURCE_PROTO:=git
PKG_MIRROR_HASH:=1147d954eef58cc457e1b902dedd96235f7943ee823fd2525fa24f00cd5411e8
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=MaxLinear
PKG_LICENSE_FILES:=LICENSE

FW_BIN_FOLDER:=$(PKG_BUILD_DIR)/fw/bin
PROGMODEL_BIN_FOLDER:=$(PKG_BUILD_DIR)/progmodel/bin
PSD_BIN_FOLDER:=$(PKG_BUILD_DIR)/psd/bin
REGULATORY_BIN_FOLDER:=$(PKG_BUILD_DIR)/regulatory/bin

BIN_FOLDER:=opt/intel/bin
SBIN_FOLDER:=opt/intel/sbin
include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk


define Package/ltq-wlan-wave_6x-uci/default
	SECTION:=utils
	CATEGORY:=MaxLinear
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
		@+PACKAGE_udev \
		+!(PACKAGE_mod-whm||PACKAGE_mod-whm-debug):prplmesh-dwpal
	URL:=http://www.maxlinear.com
	MAINTAINER:=MaxLinear Inc
	MENU:=1
endef

define Package/ltq-wlan-wave_6x-uci/description/default
 This package contains wave binaries components
endef

define Package/ltq-wlan-wave_6x-uci
	$(Package/ltq-wlan-wave_6x-uci/default)
	DEPENDS += \
		@+PACKAGE_dwpal_6x-uci \
		+libspal \
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

define Package/ltq-wlan-wave_6x-uci-osp
	$(Package/ltq-wlan-wave_6x-uci/default)
	TITLE += with opensource packages
	DEPENDS += \
		@+PACKAGE_dwpal_6x-uci-osp \
		@+PACKAGE_swpal_6x-uci-prpl \
		@+PACKAGE_iwlwav-hostap-ng-uci \
		@+PACKAGE_iwlwav-iw \
		@+PACKAGE_iwlwav-tools-osp \
		@+PACKAGE_iwlwav-driver-uci \
		@+PACKAGE_iwlwav-firmware
	VARIANT:=osp
endef

define Package/ltq-wlan-wave_6x-uci-osp/description
 $(call Package/ltq-wlan-wave_6x-uci/description/default) with opensource packages
endef

define Package/ltq-wlan-wave_6x-uci-osp-debug
	$(Package/ltq-wlan-wave_6x-uci/default)
	TITLE += with opensource packages and debug information
	DEPENDS += \
		@+PACKAGE_dwpal_6x-uci-osp-debug \
		@+PACKAGE_swpal_6x-uci-prpl-debug \
		@+PACKAGE_iwlwav-hostap-ng-uci-debug \
		@+PACKAGE_iwlwav-iw-debug \
		@+PACKAGE_iwlwav-tools-osp-debug \
		@+PACKAGE_iwlwav-driver-uci-debug \
		@+PACKAGE_iwlwav-firmware-debug
	VARIANT:=osp-debug
endef

define Package/ltq-wlan-wave_6x-uci-osp-debug/description
 $(call Package/ltq-wlan-wave_6x-uci/description/default) with opensource packages and Debug Information
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
Package/ltq-wlan-wave_6x-uci-osp/install = $(Package/ltq-wlan-wave_6x-uci/install)
Package/ltq-wlan-wave_6x-uci-osp-debug/install = $(Package/ltq-wlan-wave_6x-uci/install)

$(eval $(call BuildPackage,ltq-wlan-wave_6x-uci))
$(eval $(call BuildPackage,ltq-wlan-wave_6x-uci-debug))
$(eval $(call BuildPackage,ltq-wlan-wave_6x-uci-osp))
$(eval $(call BuildPackage,ltq-wlan-wave_6x-uci-osp-debug))
