#
# Copyright (C) 2020 - 2023 MaxLinear, Inc.
# Copyright (c) 2017 - 2020 Intel Corporation
# Copyright (C) 2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=pon-mbox-drv
PKG_VERSION:=1.24.1
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=pon_mbox_drv
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=b7183c1741449f2533c1e304c9c7d0ecacf09c00810208a75e2f8473d1c4ab17
PKG_BUILD_PARALLEL:=1
PKG_LICENSE:=GPL-2.0-only or BSD-2-Clause
PKG_LICENSE_FILES:=LICENSE
PKG_IS_KERNEL_MODULE:=y

PKG_CONFIG_DEPENDS:= \
	CONFIG_PACKAGE_kmod-pon-mbox-drv_DEBUG \
	CONFIG_PACKAGE_kmod-pon-mbox-drv_SKIP_TEP \
	CONFIG_PACKAGE_kmod-pon-mbox-drv_KCOV_INSTRUMENT \
	CONFIG_PACKAGE_kmod-qos-tc \
	CONFIG_PACKAGE_kmod-qos-mgr-drv \
	CONFIG_PACKAGE_kmod-ppa-drv-lgm

-include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk

define KernelPackage/$(PKG_NAME)
  SUBMENU:=MaxLinear
  TITLE:=PON mailbox driver
  URL:=https://www.maxlinear.com
  MAINTAINER:=MaxLinear
  DEPENDS:= \
  	+kmod-i2c-core \
	+!(PACKAGE_kmod-pon-mbox-drv_SKIP_TEP||TARGET_intel_x86_lgm):kmod-sse_driver \
	+!(PACKAGE_kmod-pon-mbox-drv_SKIP_TEP||TARGET_intel_x86_lgm):kmod-sse_storage_drv
  FILES:=$(PKG_BUILD_DIR)/src/mod_pon_mbox.ko
  AUTOLOAD:=$(call AutoProbe,mod_pon_mbox)
endef

define KernelPackage/$(PKG_NAME)-eth
  SUBMENU:=MaxLinear
  TITLE:=PON Ethernet driver
  URL:=https://www.maxlinear.com
  MAINTAINER:=MaxLinear
  DEPENDS:= +kmod-$(PKG_NAME) +PACKAGE_kmod-qos-tc:kmod-qos-tc +kmod-sched-prio +kmod-gwdpa-dpm +TARGET_intel_x86_lgm:kmod-mxl_pon_hgu_vuni_lgm
  FILES:=$(PKG_BUILD_DIR)/eth/mod_pon_eth.ko
  AUTOLOAD:=$(call AutoProbe,mod_pon_eth)
endef

define KernelPackage/$(PKG_NAME)-ptp
  SUBMENU:=MaxLinear
  TITLE:=PON PTP driver
  URL:=https://www.maxlinear.com
  MAINTAINER:=MaxLinear
  DEPENDS:= +kmod-$(PKG_NAME)
  FILES:=$(PKG_BUILD_DIR)/ptp/mod_pon_ptp.ko
  AUTOLOAD:=$(call AutoProbe,mod_pon_ptp)
endef

define KernelPackage/$(PKG_NAME)/config
$(call Package/$(PKG_NAME_PREF)$(PKG_NAME)/override_source_path)
	menu "configuration"
	depends on PACKAGE_kmod-pon-mbox-drv

	config PACKAGE_kmod-pon-mbox-drv_DEBUG
		bool "enable debug logging"
		help
		  This will enable debug logging in the mailbox driver.

	config PACKAGE_kmod-pon-mbox-drv_SKIP_TEP
		bool "disable TEP support"
		default !INTEL_MIPS_KEYS_PROVISION && !INTEL_X86_SECBOOT
		help
		  This option skips the operations which need the
		  TEP / bootcore.
		  Set this to "N" for systems using a TEP and having
		  a provisioned OTP.
		  Set this to "Y" for systems not using a TEP.

		  PRX300:
		  When this option is set the pon_mbox_drv will *not* try to
		  unlock the PON IP before downloading the PON IP FW.
		  When this option is set the random number will be fetched
		  from the Linux entropy pool instead of the TRNG of the EIP123.

		  URX800:
		  This option skips the locking of the PON FW download on URX.
		  Without this option, PON FW can only be downloaded one-time after
		  board startup. Any further PON FW download afterwards is blocked.
		  PON debugging is not possible as well.

	config PACKAGE_kmod-pon-mbox-drv_KCOV_INSTRUMENT
		bool "Enable KCOV instrumentation" if !KERNEL_KCOV_INSTRUMENT_ALL
		depends on KERNEL_KCOV
		help
		  This will enable KCOV instrumentation for the driver.

	endmenu
endef

define KernelPackage/$(PKG_NAME)/description
   The driver for PON IP mailbox
endef

PON_MAKEDEFS += CONFIG_PON_MBOX_DRV=m

ifneq ($(CONFIG_PACKAGE_kmod-pon-mbox-drv-eth),)
	PON_MAKEDEFS += CONFIG_PON_ETH_DRV=m
endif

ifneq ($(CONFIG_PACKAGE_kmod-pon-mbox-drv-ptp),)
	PON_MAKEDEFS += CONFIG_PON_PTP_DRV=m
endif

ifeq ($(CONFIG_PACKAGE_kmod-pon-mbox-drv_KCOV_INSTRUMENT),y)
	PON_MAKEDEFS += KCOV_INSTRUMENT=y
endif

ifeq ($(CONFIG_PACKAGE_kmod-pon-mbox-drv_DEBUG),y)
	NOSTDINC_FLAGS += -DDEBUG
endif

ifeq ($(CONFIG_PACKAGE_kmod-pon-mbox-drv_SKIP_TEP),y)
	NOSTDINC_FLAGS += -DSKIP_TEP
endif

define Build/Compile
	+$(KERNEL_MAKE) $(PON_MAKEDEFS) $(PKG_JOBS) \
		M="$(PKG_BUILD_DIR)" \
		NOSTDINC_FLAGS="$(NOSTDINC_FLAGS)" \
		modules
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/pon
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/include/pon/* $(1)/usr/include/pon/
endef

$(eval $(call KernelPackage,$(PKG_NAME)))
$(eval $(call KernelPackage,$(PKG_NAME)-eth))
$(eval $(call KernelPackage,$(PKG_NAME)-ptp))
