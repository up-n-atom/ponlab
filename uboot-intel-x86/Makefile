#
# Copyright (C) 2018 Intel Corporation
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=uboot-intel-x86
PKG_BASE_NAME:=u-boot
PKG_VERSION:=2022.01
PKG_RELEASE:=1

# this is the list of subtargets scanned for uboot-configs
TARGET_LIST:=lgm

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(BUILD_VARIANT)/$(PKG_BASE_NAME)-$(PKG_VERSION)
# out-of-tree not supported yet
#PKG_VARIANT_BUILD_DIR:=$(PKG_BUILD_DIR)/build-$(BUILD_VARIANT)
PKG_VARIANT_BUILD_DIR:=$(PKG_BUILD_DIR)
PKG_SOURCE:=$(PKG_BASE_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://ftp.denx.de/pub/u-boot
PKG_HASH:=81b4543227db228c03f8a1bf5ddbc813b0bb8f6555ce46064ef721a6fc680413
PKG_TARGETS:=bin
PKG_BUILD_PARALLEL:=0
PKG_FILE_DEPENDS:=../open_uboot/patches ../open_uboot/files
PKG_BOARD_DIR:=$(PKG_BUILD_DIR)/board/intel/
PKG_ASLR_PIE:=0
PKG_LICENSE:=GPL-2.0-or-later
PKG_LICENSE_FILES:=Licenses/gpl-2.0.txt

-include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

ifeq ($(DUMP),)
  STAMP_BUILT:=$(PKG_VARIANT_BUILD_DIR)/.built
endif
ifneq ($(CONFIG_TARGET),)
  STAMP_CONFIGURED:=$(PKG_VARIANT_BUILD_DIR)/.configured
  $(shell rm $(STAMP_CONFIGURED))
endif

define Package/$(PKG_NAME)/description
U-Boot variants:
uboot-<board name>      -> U-Boot for specific board + optional variant (flash, etc.)
endef

define Package/$(PKG_NAME)
  SECTION:=boot
  CATEGORY:=Boot Loaders
  SUBMENU:=$(PKG_NAME)
  DEPENDS:=@TARGET_intel_x86
  TITLE:=U-Boot Meta package (no binaries)
  DEFAULT:=y
  URL:=http://www.denx.de/wiki/U-Boot
endef

# only override_source_path
define Package/$(PKG_NAME)/config
  $(call Package/$(PKG_NAME)/override_source_path)
endef

define Package/uboot/template
define Package/uboot-$(1)
  SECTION:=boot
  CATEGORY:=Boot Loaders
  SUBMENU:=$(PKG_NAME)
  DEPENDS:=$(PKG_NAME) $(2)
  TITLE:=$(1)
  DEFAULT:=n
  VARIANT:=$(1)
  URL:=http://www.denx.de/wiki/U-Boot
  MENU=1
endef
endef

UBOOTS:=

define BuildUbootPackage
# params:
# $(1) name (TITLE, build-variant)
# $(2) config-name (only if different from build-variant)
# $(3) "default y" dependency
	UBOOTS+=$(1)
	CONFIG_$(1):=$(2)
	$(call Package/uboot/template,$(1),$(3))
endef

################################################################################
# definitions for supported boards/configs
################################################################################

# create list of boards from defconfig files
define ScanTarget
	$(eval CFG_DIR:=$(TOPDIR)/target/linux/feeds/intel_x86/$(1)/uboot_configs)
	$(eval CFG_FILES:=$(strip $(patsubst $(CFG_DIR)/%,%,$(sort $(wildcard $(CFG_DIR)/*_defconfig)))))
	$(eval UBOOT_TARGETS:=$(patsubst %_defconfig,%,$(CFG_FILES)))
	$(foreach u,$(UBOOT_TARGETS),$(eval $(call BuildUbootPackage,$(u),,@TARGET_intel_x86_$(1))))
endef
$(foreach t,$(TARGET_LIST),$(eval $(call ScanTarget,$(t))))

################################################################################

ifdef BUILD_VARIANT
UBOOT_CONFIG:=$(if $(CONFIG_$(BUILD_VARIANT)),$(CONFIG_$(BUILD_VARIANT)),$(BUILD_VARIANT))
endif

UBOOT_CONFIG_DIR:=$(TOPDIR)/target/linux/feeds/intel_x86/$(SUBTARGET)/uboot_configs

ifneq ($(CONFIG_uboot-intel-x86_USE_CUSTOM_SOURCE_DIR),y)
define Build/Prepare
	$(Build/Prepare/Default)
	[ -d ./files ] && $(CP) ./files/* $(PKG_BUILD_DIR)/
	[ -d $(UBOOT_CONFIG_DIR) ] && $(CP) $(UBOOT_CONFIG_DIR)/* $(PKG_BUILD_DIR)/configs/
endef
endif

UBOOT_MAKE_OPTS= \
	$(SILENT_OPTS) \
	HOSTCC="$(HOSTCC)" \
	HOSTCFLAGS="$(HOST_CFLAGS) $(HOST_CPPFLAGS) -std=gnu11" \
	HOSTLDFLAGS="$(HOST_LDFLAGS) -pthread" \
	STAGING_PREFIX="$(STAGING_DIR_HOST)" \
	CROSS_COMPILE=$(TARGET_CROSS)

# for separate build-dirs, not yet supported:
#	O=$(PKG_VARIANT_BUILD_DIR)

# make the submake silent:
ifneq (,$(findstring 99,$(V)))
SILENT_OPTS:=V=1
endif
ifneq (,$(findstring s,$(V)))
SILENT_OPTS:=-s
endif

define config_save
	echo "Config save: $(UBOOT_CONFIG)"; \
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(UBOOT_MAKE_OPTS) \
		savedefconfig; \
	cp $(PKG_VARIANT_BUILD_DIR)/defconfig \
		$(PKG_BUILD_DIR)/configs/$(UBOOT_CONFIG)_defconfig; \
	cp $(PKG_VARIANT_BUILD_DIR)/defconfig \
		$(UBOOT_CONFIG_DIR)/$(UBOOT_CONFIG)_defconfig
endef

define Build/Configure
	echo "Configure: $(UBOOT_CONFIG)"
	chmod +x $(PKG_BUILD_DIR)/scripts_platform/*
	find $(PKG_BOARD_DIR) -name "*.pl" -type f -exec chmod +x {} \;
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(UBOOT_MAKE_OPTS) \
		$(UBOOT_CONFIG)_defconfig
	if [ "$(CONFIG_TARGET)" = "$(UBOOT_CONFIG)" ]; then \
		$(_SINGLE)$(MAKE) -C $(PKG_BUILD_DIR) menuconfig; \
		$(call config_save); \
	fi
endef

ifeq ($(QUILT),)
update: configure
	echo "Update: $(UBOOT_CONFIG)"
	$(call config_save)
endif

define Build/Compile
	echo "Compile: $(UBOOT_CONFIG)"
	+$(MAKE_VARS) \
	$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR) \
		$(UBOOT_MAKE_OPTS) \
		all $(CSCOPE)
endef

UBOOT_IMAGES:=u-boot-recovery.bin u-boot-recovery.asc \
		ubootenv.img \
		u-boot-mem.bin \
		spl/ext_sign/* spl/rot/*

#for emmc 
UBOOT_IMAGES+=*-emmc.bin
UBOOT_IMAGES+=spl/*-emmc.bin
#for nand
UBOOT_IMAGES+=*-nand.bin
UBOOT_IMAGES+=spl/*-nand.bin
#for snor
UBOOT_IMAGES+=*-sf.bin
UBOOT_IMAGES+=spl/*-sf.bin


ifeq ($(CONFIG_INTEL_X86_EXTERNAL_IMAGE_SIGNING),y)
define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)
	cd $(PKG_VARIANT_BUILD_DIR); \
	for f in $(UBOOT_IMAGES); do \
		if [ -e $$$$f ]; then \
			cp -v $$$$f $(1); \
		fi; \
	done
	if [ -d "$(PKG_VARIANT_BUILD_DIR)/spl/ext_sign" ]; then \
		mkdir -p $(BIN_DIR)/non_signed_image/uboot-$(BUILD_VARIANT); \
		cp -f $(PKG_VARIANT_BUILD_DIR)/spl/ext_sign/u-boot-* $(BIN_DIR)/non_signed_image/uboot-$(BUILD_VARIANT)/; \
	fi;
endef
else
define Package/$(PKG_NAME)/install
	$(eval variant_path:=$(subst $(PKG_NAME),$(BUILD_VARIANT),$(1)))
	$(INSTALL_DIR) $(variant_path)
	cd $(PKG_VARIANT_BUILD_DIR); \
	for f in $(UBOOT_IMAGES); do \
		if [ -e $$$$f ]; then \
			cp -v $$$$f $(variant_path); \
		fi; \
	done
	mkdir -p $(BIN_DIR)/uboot-$(BUILD_VARIANT)
	cp -vf $(variant_path)/u-boot-* $(BIN_DIR)/uboot-$(BUILD_VARIANT)
endef
endif

define Package/uboot-lgp-urx651-10g-lan-emmc/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-lgp-urx651-p34x-phy-emmc/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-lgp-urx851-10g-lan-emmc/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-lgp-urx851-10g-lan-emmc-firewall-on/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-lgp-urx851-p34x-phy-2K-page-ebu_nand/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-lgp-urx851-p34x-phy-4K-page-ebu_nand/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-lgp-urx851-p34x-phy-emmc/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-lgp-urx851-p34x-phy-emmc-prpl/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-lgp-urx851-4GB-ddr-p34x-phy-emmc/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-lgp-urx851-p34x-phy-emmc-firewall-on/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-lgp-urx851-sec-10g-lan-emmc/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-lgp-urx851-sec-p34x-phy-2K-page-ebu_nand/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-lgp-urx851-sec-p34x-phy-emmc/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-lgp-urx851-sec-p34x-phy-emmc-prpl/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-lgp-urx851-emmc-cust-img/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-lgp-urx851-emmc-field-img/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-lgp-urx851-emmc-manuboot-img/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx640-rtl8261-4K-ebu-nand-prpl/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx641-aqn-phy-emmc/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx641-10g-lan-emmc-prpl/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx641-p34x-phy-emmc/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx641-p34x-phy-emmc-prpl_overlay/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx641-p34x-phy-emmc-prpl/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx641-sec-p34x-phy-emmc-prpl/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx641-emmc-cust-img/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx641-emmc-field-img/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx641-emmc-manuboot-img/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx851-aqn-phy-emmc/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx851-p34x-phy-4K-page-ebu_nand/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx851-10g-lan-emmc-prpl/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx851-p34x-phy-emmc/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx851-p34x-phy-emmc-prpl/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx851-sec-p34x-phy-emmc/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx851-sec-p34x-phy-emmc-prpl/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx851-emmc-cust-img/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx851-emmc-field-img/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-octopus-urx851-emmc-manuboot-img/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-osp-tb341-p34x-phy-emmc/install
        $(call Package/$(PKG_NAME)/install $(1))
endef
define Package/uboot-osp-tb341-4GB-ddr-p34x-phy-emmc/install
        $(call Package/$(PKG_NAME)/install $(1))
endef

# dummy package for global config options
$(eval $(call BuildPackage,$(PKG_NAME)))

$(foreach u,$(UBOOTS), \
	$(eval $(call BuildPackage,uboot-$(u))) \
)
