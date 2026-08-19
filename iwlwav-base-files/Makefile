include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=iwlwav-base-files
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/iwlwav-base-files
  SUBMENU:=Base Files
  SECTION:=base
  CATEGORY:=Intel
  TITLE:=Common files for Intel OWRT CPE Systems
  URL:=http://www.intel.com/
  MAINTAINER:=Intel
  DEPENDS:=+iwlwav-hostap-uci
endef

define Package/iwlwav-base-files/description
 This package contains files and system scripts for OpenWrt,
 common for all Intel CPE Targets.
endef

define Build/Prepare
endef

define Build/Compile
endef

define Package/iwlwav-base-files/install
	$(INSTALL_DIR) $(1)/mnt
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(LN) /tmp/mounts $(1)/mnt/usb
	$(CP) ./files/* $(1)/
endef

$(eval $(call BuildPackage,iwlwav-base-files))

