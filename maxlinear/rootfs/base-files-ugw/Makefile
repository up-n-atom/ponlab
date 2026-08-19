#
# Copyright (C) 2009 OpenWrt.org
# Copyright (C) 2017 Intel Corporation
# Copyright (c) 2020, MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: $

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=base-files-ugw
PKG_RELEASE:=1
CPE_NAME:=$(shell echo $(SUBTARGET) | awk '{ print toupper($$1) }')
UGW_VERSION:=$(if $(wildcard $(TOPDIR)/ugw_version),$(shell cat $(TOPDIR)/ugw_version))
UGW_BANNER:=MaxLinear UGW Software $(UGW_VERSION) on $(CPE_NAME) CPE
PKG_BUILD_DEPENDS:=lxc

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SUBMENU:=Base Files
  SECTION:=base
  CATEGORY:=MaxLinear
  TITLE:=Common files for MaxLinear CPE Systems
  URL:=https://www.maxlinear.com/
  MAINTAINER:=MaxLinear
  DEPENDS:=+busybox +logd +base-files +fstools
endef

define Package/$(PKG_NAME)/description
 This package contains files and system scripts for OpenWrt,
 common for all MaxLinear CPE Targets.
endef

define Package/$(PKG_NAME)-sdl-lite
$(call Package/$(PKG_NAME))
  TITLE:=SDL rootfs files addons
endef

define Package/$(PKG_NAME)-sdl-lite/description
  This package contains few SDL components adapted for models using base-files-ugw
endef

define Build/Prepare
endef

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	$(CP) ./files/* $(1)/
	$(if $(CONFIG_TARGET_intel_x86_lgm),mv -f $(1)/etc/passwd_lgm_x86_64 $(1)/etc/passwd,rm -f $(1)/etc/passwd_lgm_x86_64)
	$(if $(CONFIG_TARGET_intel_x86_lgm),mv -f $(1)/etc/sysctl.conf_lgm $(1)/etc/sysctl.conf,rm -f $(1)/etc/sysctl.conf_lgm)
	$(if $(CONFIG_TARGET_intel_x86_lgm),mv -f $(1)/etc/config/ppa_lgm $(1)/etc/config/ppa,rm -f $(1)/etc/config/ppa_lgm)
	$(if $(CONFIG_TARGET_intel_x86_lgm),mv -f $(1)/lib/netifd/dhcp.script_lgm $(1)/lib/netifd/dhcp.script,rm -f $(1)/lib/netifd/dhcp.script_lgm)
	$(if $(CONFIG_TARGET_intel_x86_lgm),mv -f $(1)/lib/netifd/proto/gre.sh_lgm $(1)/lib/netifd/proto/gre.sh,rm -f $(1)/lib/netifd/proto/gre.sh_lgm)
	rm -f $(1)/etc/overlay_whitelist.conf
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/mnt
	$(LN) /tmp/mounts $(1)/mnt/usb
	- (echo -ne UGW- && cat $(TOPDIR)/ugw_version) > $(1)/etc/version
	if [ -r "$(TOPDIR)/../ugw_tag" ]; then \
		$(INSTALL_DATA) $(TOPDIR)/../ugw_tag $(1)/etc/; \
	fi
	date +%H%M-%d%b%y > $(1)/etc/timestamp
	- @$$(call put_ltq_banner,$(1)/etc/banner)

	mkdir -p $(1)/$(VENDOR_PATH)/etc
	echo "#<< switch_ports" > $(1)/$(VENDOR_PATH)/etc/switchports.conf
	echo "switch_mii1_port=\""$(CONFIG_SWITCH_MII1_PORT)"\"" >> $(1)/$(VENDOR_PATH)/etc/switchports.conf
	echo "switch_lan_ports=\""$(CONFIG_SWITCH_LAN_PORTS)"\"" >> $(1)/$(VENDOR_PATH)/etc/switchports.conf
	echo "#>> switch_ports" >> $(1)/$(VENDOR_PATH)/etc/switchports.conf
endef

define Package/$(PKG_NAME)-sdl-lite/install
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
$(eval $(call BuildPackage,$(PKG_NAME)-sdl-lite))
