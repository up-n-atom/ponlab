#
# Openwrt Makefile for wave related SW element
#
# Simplify mode 1.0
#
#

#### Includes ###################################
include $(TOPDIR)/rules.mk

PKG_NAME:=iwlwav-driver-uci
PKG_REV:=25afb4fa6af0c73e30a81181c101c19e3acd2d45
PKG_VERSION:=6.1.0
PKG_RELEASE:=$(PKG_REV)

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_REV).tar.bz2
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=iwlwav-dev.git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME)
PKG_SOURCE_VERSION:=r597+a+41+a+17
PKG_SOURCE_PROTO:=git
PKG_MIRROR_HASH:=b156fd80f826437826f8276ec20b49c9b570d917363610a86521f1502c532776

PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=BSD-2-Clause,GPLv2
# LICENSE File stored at drivers/net/wireless/intel/iwlwav/LICENSE
PKG_LICENSE_FILES:=LICENSE

PKG_BUILD_DIR:=$(BUILD_DIR)/iwlwav-driver-uci-$(BUILD_VARIANT)/iwlwav-driver-uci-$(PKG_VERSION)
CONFIG_FOLDER:=../lq-wave-300_6x-uci/files/config

CONFIG_FILE:=COMMON_WAVE600_CONFIG
ifeq ($(CONFIG_TARGET_intel_mips_prx300),y)
 CONFIG_FILE:=PRX300_WAVE600_CONFIG
endif

ifeq ($(CONFIG_TARGET_intel_x86_lgm),y)
  INSMOD_PRMS:="interface_index=0,2,4,6"
endif

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk
-include $(INCLUDE_DIR)/package-version-override.mk

define KernelPackage/$(PKG_NAME)
  SUBMENU:=MaxLinear
  TITLE:=Wave WLAN Driver
  DEPENDS:=+PACKAGE_kmod-directconnect-dp:kmod-directconnect-dp
  KCONFIG:= \
    CONFIG_CFG80211 \
    CONFIG_MAC80211 \
    CONFIG_CRYPTO_SHA256=y \
    CONFIG_FW_LOADER=y \
    CONFIG_HOTPLUG=y \
    CONFIG_WIRELESS=y \
    CONFIG_CRYPTO_CCM=y \
    CONFIG_CRYPTO_CMAC=y \
    CONFIG_ATH5K_PCI=n \
    CONFIG_CFG80211_CRDA_SUPPORT=y \
    CONFIG_CFG80211_DEBUGFS=n \
    CONFIG_CFG80211_DEFAULT_PS=y \
    CONFIG_CFG80211_DEVELOPER_WARNINGS=n \
    CONFIG_CFG80211_INTERNAL_REGDB=n \
    CONFIG_CFG80211_WEXT=y \
    CONFIG_CFG80211_WEXT_EXPORT=y \
    CONFIG_IWL4965=n \
    CONFIG_DPM_DATAPATH_CPUFREQ=n \
    CONFIG_DPM_DATAPATH_PTP1588=n \
    CONFIG_MAC80211_DEBUGFS=n \
    CONFIG_MAC80211_DEBUG_MENU=n \
    CONFIG_MAC80211_HAS_RC=y \
    CONFIG_MAC80211_HWSIM=n \
    CONFIG_MAC80211_LEDS=n \
    CONFIG_MAC80211_MESH=n \
    CONFIG_MAC80211_RC_DEFAULT="minstrel_ht" \
    CONFIG_MAC80211_RC_DEFAULT_MINSTREL=y \
    CONFIG_MAC80211_RC_MINSTREL=y \
    CONFIG_MAC80211_RC_MINSTREL_HT=y \
    CONFIG_MAC80211_RC_MINSTREL_VHT=n \
    CONFIG_MCAST_HELPER_PACKAGE=n \
    CONFIG_MCAST_HELPER_REG=n \
    CONFIG_MDIO_BOARDINFO=y \
    CONFIG_MT7601U=n \
    CONFIG_MWIFIEX=n \
    CONFIG_NL80211_TESTMODE=n \
    CONFIG_RSI_91X=n \
    CONFIG_RTL8XXXU=n \
    CONFIG_WCN36XX=n \
    CONFIG_WIFI_PROXY_DEBUG=n \
    CONFIG_WIL6210=n \
    CONFIG_WILC1000_SPI=n \
    CONFIG_WLAN_VENDOR_ADMTEK=y \
    CONFIG_WLAN_VENDOR_ATH=y \
    CONFIG_WLAN_VENDOR_ATMEL=y \
    CONFIG_WLAN_VENDOR_BROADCOM=y \
    CONFIG_WLAN_VENDOR_CISCO=y \
    CONFIG_WLAN_VENDOR_INTEL=y \
    CONFIG_WLAN_VENDOR_INTERSIL=y \
    CONFIG_WLAN_VENDOR_MARVELL=y \
    CONFIG_WLAN_VENDOR_MEDIATEK=y \
    CONFIG_WLAN_VENDOR_RALINK=y \
    CONFIG_WLAN_VENDOR_REALTEK=y \
    CONFIG_WLAN_VENDOR_RSI=y \
    CONFIG_WLAN_VENDOR_ST=y \
    CONFIG_WLAN_VENDOR_TI=y \
    CONFIG_WLAN_VENDOR_ZYDAS=y \
    CONFIG_CRYPTO_ARC4=y

ifeq ($(CONFIG_LINUX_5_15),y)
 FILES:= \
        $(PKG_BUILD_DIR)/compat/compat.ko \
	$(PKG_BUILD_DIR)/net/mac80211/mac80211.ko \
	$(PKG_BUILD_DIR)/net/wireless/cfg80211.ko \
	$(PKG_BUILD_DIR)/drivers/net/wireless/intel/iwlwav/mtlk.ko \
	$(PKG_BUILD_DIR)/drivers/net/wireless/intel/iwlwav/tools/mtlkroot/mtlkroot.ko \
	$(LINUX_DIR)/lib/crypto/libarc4.ko
else
 FILES:= \
        $(PKG_BUILD_DIR)/compat/compat.ko \
	$(PKG_BUILD_DIR)/net/mac80211/mac80211.ko \
	$(PKG_BUILD_DIR)/net/wireless/cfg80211.ko \
	$(PKG_BUILD_DIR)/drivers/net/wireless/intel/iwlwav/mtlk.ko \
	$(PKG_BUILD_DIR)/drivers/net/wireless/intel/iwlwav/tools/mtlkroot/mtlkroot.ko
endif

 AUTOLOAD:=$(call AutoProbe,compat mac80211 cfg80211 mtlkroot mtlk)
 MODPARAMS.mtlk:= \
    fastpath=1,1,1 \
    loggersid=255,255,255 \
    dual_pci=1,1 $(INSMOD_PRMS)
 VARIANT:=release
endef

define KernelPackage/iwlwav-driver-uci/description
  iwlwav WLAN Driver
endef

IFX_CFLAGS += -O2 -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security -Wall -fPIE -Wl,-strip-debug

ifeq ($(CONFIG_USE_WAVE600_2_EMULATOR),y)
IFX_CFLAGS += -DCONFIG_WAVE_NO_TX_BSS_MONITOR
endif

define KernelPackage/iwlwav-driver-uci-debug
$(call KernelPackage/iwlwav-driver-uci)
TITLE += with debug information
VARIANT:=debug
endef

define KernelPackage/iwlwav-driver-uci-debug/description
$(call KernelPackage/iwlwav-driver-uci/description) With debug information
endef

CROSS_COMPILE=$(if $(CONFIG_EXTERNAL_TOOLCHAIN),$(CONFIG_TOOLCHAIN_ROOT)/bin/$(CONFIG_TOOLCHAIN_PREFIX),$(TOOLCHAIN_DIR)/bin/$(TARGET_CROSS)) # fix for non external toolchain!
JOBS=-j$(shell nproc) -l$(shell nproc)

CONFIG_SUFFIX:=
ifeq ($(BUILD_VARIANT),debug)
IFX_CFLAGS += -DCONFIG_WAVE_DEBUG
CONFIG_SUFFIX:=-debug
endif

define Build/Compile
	@echo "Config files use: $(CONFIG_FILE)"
	$(SED) '1s/.*/BACKPORTS_VERSION="v$(LINUX_VERSION)"/' $(PKG_BUILD_DIR)/versions
	$(CP) $(CONFIG_FOLDER)/$(CONFIG_FILE)$(CONFIG_SUFFIX) $(PKG_BUILD_DIR)/defconfigs/iwlwav
	$(KERNEL_MAKE) -C $(PKG_BUILD_DIR) CFLAGS="$(IFX_CFLAGS)" defconfig-iwlwav KLIB_BUILD=$(LINUX_DIR) KLIB=$(TARGET_DIR)/$(TARGET_MODULES_DIR)
	$(KERNEL_MAKE) $(JOBS) -C $(PKG_BUILD_DIR) CFLAGS="$(IFX_CFLAGS)" CROSS_COMPILE=$(CROSS_COMPILE) ARCH=$(ARCH) KLIB_BUILD=$(LINUX_DIR) KLIB=$(TARGET_DIR)/$(TARGET_MODULES_DIR) UGW_DRIVER_VERSION="$(PKG_VERSION).$(PKG_REV)"
endef

define KernelPackage/iwlwav-driver-uci/install
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/lib/firmware
	$(INSTALL_DIR) $(1)/lib/preinit
	echo -e "iwlwav_driver_hash=\"$(PKG_REV)\"\niwlwav_driver_tag=\"$(PKG_REV)\"" > $(1)/etc/iwlwav_driver.ver
	$(INSTALL_DATA) ./files/91_wireless_calib $(1)/lib/preinit
	cd $(1)/lib/firmware && ln -sf ../../tmp/wlanconfig/cal_wlan0.bin
	cd $(1)/lib/firmware && ln -sf ../../tmp/wlanconfig/cal_wlan2.bin
	cd $(1)/lib/firmware && ln -sf ../../tmp/wlanconfig/cal_wlan4.bin
endef

KernelPackage/iwlwav-driver-uci-debug/install = $(KernelPackage/iwlwav-driver-uci/install)

$(eval $(call KernelPackage,iwlwav-driver-uci))
$(eval $(call KernelPackage,iwlwav-driver-uci-debug))
