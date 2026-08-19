include $(TOPDIR)/rules.mk 
include $(INCLUDE_DIR)/kernel.mk 

PKG_NAME:=gphy_fw_update
PKG_VERSION:=2.7.1.1
PKG_RELEASE:=1

-include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=driver 
  CATEGORY:=MaxLinear
  TITLE:=GPY FW update tool
  MAINTAINER:=MaxLinear
  MENU:=1
endef

define Package/$(PKG_NAME)/description
	Advanced Utility for updating gpy FW 
endef 
### Menuconfig ################################## 
define Package/$(PKG_NAME)/config
	menu "$(PKG_NAME) features"
	depends on PACKAGE_$(PKG_NAME)
	endmenu
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/bin/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
