#
# Copyright (C) 2009 OpenWrt.org
# Copyright (C) 2017 Intel Corporation
# Copyright (c) 2020-2022, MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: $

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=base-files-sdl
PKG_RELEASE:=1
CPE_NAME:=$(shell echo $(SUBTARGET) | awk '{ print toupper($$1) }')
UGW_VERSION:=$(if $(wildcard $(TOPDIR)/ugw_version),$(shell cat $(TOPDIR)/ugw_version))
UGW_BANNER:=MaxLinear UGW Software $(UGW_VERSION) on $(CPE_NAME) CPE

PKG_BUILD_DEPENDS:=netifd

PLATFORM_FILES:=$(wildcard files-*)

include $(INCLUDE_DIR)/package.mk

ifeq ($(UGW_VERSION),)
 #PRPL
 PKG_DEPENDS:=
else
 #UGW
 PKG_DEPENDS:= +websockets +libugwnetwork
endif

define Package/$(PKG_NAME)
  SUBMENU:=Base Files
  SECTION:=base
  CATEGORY:=MaxLinear
  TITLE:=Common files for MaxLinear CPE Systems with SDL Compliance
  URL:=https://www.maxlinear.com/
  MAINTAINER:=MaxLinear
  DEPENDS:=+busybox +lighttpd +owrt-ppa-scripts +logd +base-files $(PKG_DEPENDS)
endef

define Package/$(PKG_NAME)/description
 This package contains files and system scripts for OpenWrt,
 common for all MaxLinear CPE Targets.
endef

define Package/$(PKG_NAME)-ovl-whitelist
  $(call Package/$(PKG_NAME))
  TITLE:=overlay whitelist feature
endef

define Package/$(PKG_NAME)-ovl-whitelist/description
  This package contains overlay whitelist feature mechanism to use for Debugging
endef

define platform_pkg
  define Package/$(PKG_NAME)-$(1)
    $(call Package/$(PKG_NAME))
    TITLE:=$(call toupper,$(1)) specific rootfs files
    DEPENDS += +$(PKG_NAME) $(if $(wildcard ./depends-$(1)),$(shell cat ./depends-$(1)))
  endef

  define Package/$(PKG_NAME)-$(1)/description
    This package contains $(call toupper,$(1)) specific rootfs files
  endef

  define Package/$(PKG_NAME)-$(1)/install
	$(CP) ./files-$(1)/* $$(1)/
  endef
endef

define Build/Prepare
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(CP) ./files/* $(1)/
	$(if $(CONFIG_TARGET_intel_x86_lgm),mv -f $(1)/lib/netifd/proto/gre.sh_lgm $(1)/lib/netifd/proto/gre.sh,rm -f $(1)/lib/netifd/proto/gre.sh_lgm)
	$(if $(CONFIG_TARGET_intel_x86_lgm),mv -f $(1)/etc/board.d/02_network_lgm $(1)/etc/board.d/02_network,rm -f $(1)/etc/board.d/02_network_lgm)
	rm -f $(1)/etc/overlay_whitelist.conf
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/mnt
	$(LN) ../tmp/mounts $(1)/mnt/usb
	$(LN) ../tmp/mount_upgrade $(1)/mnt/upgrade
	$(LN) ../tmp/timezone/TZ $(1)/etc/TZ
	- (echo -ne UGW- && cat $(TOPDIR)/ugw_version) > $(1)/etc/version
	if [ -r "$(TOPDIR)/../ugw_tag" ]; then \
		$(INSTALL_DATA) $(TOPDIR)/../ugw_tag $(1)/etc/; \
	fi
	date +%H%M-%d%b%y > $(1)/etc/timestamp
	- @$$(call put_ltq_banner,$(1)/etc/banner)
endef

define Package/$(PKG_NAME)-ovl-whitelist/install
	$(INSTALL_DIR) $(1)/etc
	$(CP) ./files/etc/overlay_whitelist.conf $(1)/etc/
endef

define put_ltq_banner
	b_str=" | $(UGW_BANNER) |"; \
	b_chars=`echo -n $$b_str | wc -m`; \
	(echo -n " +"; for ((i=2;i<$$b_chars;i++)); do echo -n "-"; done; echo "+"; \
	echo "$$b_str";echo -n " +";for ((i=2;i<$$b_chars;i++));do echo -n "-";done;echo -en "+\n\n") >> $(1)
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)-ovl-whitelist))
$(foreach pkg,$(subst files-,,$(PLATFORM_FILES)),$(eval $(call platform_pkg,$(pkg))))
$(foreach pkg,$(subst files-,,$(PLATFORM_FILES)),$(eval $(call BuildPackage,$(PKG_NAME)-$(pkg))))
