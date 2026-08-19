#
# Copyright (C) 2022-2023 MaxLinear, Inc.
#
# Feed makefile for SGAM cli
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=sgam_cli
PKG_VERSION:=1.0
PKG_MAINTAINER:=MaxLinear
TARGET_CFLAGS += -I$(LINUX_DIR)/user_headers/include

define Package/sgam-cli
  SECTION:=utils
  CATEGORY:=MaxLinear
  TITLE:=Session Group Accounting and Metering Commandline tool
  DEPENDS:=+@TARGET_intel_x86_lgm:kmod-sgam-drv +libsafec3-7
  MAINTAINER:=$(PKG_MAINTAINER)
  URL:=http://www.maxlinear.com
endef

define Package/sgam-cli/description
  Command line tool for configuring the Session based Group Accounting and
  Metering module.
endef

define Package/sgam-cli/install
	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/sgam_cli $(1)/usr/sbin/
endef

define Build/Clean
	@- rm -Rf $(PKG_BUILD_DIR)/ipk*
	@- [ -e $(PKG_BUILD_DIR)/Makefile ] && $(MAKE) -C $(PKG_BUILD_DIR) clean
	$(call Build/Clean/Default)
endef

$(eval $(call BuildPackage,sgam-cli))
