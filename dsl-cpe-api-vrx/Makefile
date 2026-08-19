#
# Copyright (C) 2017 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=dsl-cpe-api-vrx
PKG_VERSION:=4.21.6.2
PKG_MD5SUM:=99dba7a813b48746a3c3df85abc41b0a
PKG_RELEASE:=1
PKG_INSTALL:=1
PKG_BASE_NAME:=drv_dsl_cpe_api_vrx
PKG_SOURCE:=$(PKG_BASE_NAME)-$(PKG_VERSION).tar.gz
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/drv_dsl_cpe_api-$(PKG_VERSION)
PKG_SOURCE_URL:=http://localhost
PKG_BUILD_DEPENDS:=dsl-cpe-mei-vrx

DSL_BIN_DIR=$(VENDOR_PATH)/bin
DSL_INITD_DIR=$(VENDOR_PATH)/etc/init.d

ifeq ($(BUILD_VARIANT),debug)
ifeq (y,$(CONFIG_dsl-cpe-mei-vrx_DTM_STANDALONE))
	STARTSEQ = 18
endif
endif

PKG_FIXUP:=autoreconf

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/package-version-override.mk

#get firmware relase step
ifneq ($(CONFIG_UBOOT_CONFIG_FIRMWARE_IN_ROOTFS)$(CONFIG_dsl-cpe-mei-vrx_DEVICE_VR10_320),)
FIRMWARE_BIN=$(1)/lib/firmware
else
FIRMWARE_BIN=$(STAGING_DIR)/firmware/
endif


# get config values from general image config or other packages in dependency list
# Definition of these values will be overtaken from package 'dsl-cpe-mei-vrx'
DSL_NETLINK_ID=$(strip $(subst ",, $(CONFIG_dsl-cpe-mei-vrx_DSL_NETLINK_ID)))

ifeq ($(DSL_NETLINK_ID),)
	DSL_NETLINK_ID=28
endif

ifeq ($(DUMP),)
	STAMP_CONFIGURED:=$(strip $(STAMP_CONFIGURED))_$(shell $(SH_FUNC) ( \
		grep '^CONFIG_$(PKG_NAME)' $(TOPDIR)/.config; \
		grep '^CONFIG_$(PKG_NAME)_DTM_' $(TOPDIR)/.config; \
		grep '^CONFIG_VRX_' $(TOPDIR)/.config) | md5s)
endif

define Package/dsl-cpe-api
	SECTION:=driver
	CATEGORY:=Lantiq
	SUBMENU:=DSL Subsystem
	TITLE:=DSL CPE API driver for VRX Family
	URL:=http://www.lantiq.com/
	MAINTAINER:=Lantiq Beteiligungs-GmbH & Co. KG
	DEPENDS:=+dsl-cpe-mei-vrx \
		+kmod-dsl-cpe-api-vrx
	MENU:=1
endef

define Package/$(PKG_NAME)
	$(call Package/dsl-cpe-api)
	VARIANT:=release
endef

define Package/$(PKG_NAME)-dbg
	$(call Package/dsl-cpe-api)
	TITLE+= (Debug)
	VARIANT:=debug
endef

define Package/$(PKG_NAME)/description
	This package contains the Lantiq DSL CPE API driver.

	Supported Devices:
		- VRX200 Family
		- VRX300 Family
		- VRX500 Family
endef

define Package/$(PKG_NAME)-dbg/description
	This package contains the Lantiq DSL CPE API driver (with Debug tools).

	Supported Devices:
		- VRX200 Family
		- VRX300 Family
		- VRX500 Family
endef

define Package/$(PKG_NAME)/config
$(call Package/$(PKG_NAME)/override_version)
$(call Package/$(PKG_NAME)/override_source_path)
source "$(SOURCE)/Config.in"
endef

define KernelPackage/$(PKG_NAME)
	SUBMENU:=Lantiq
	TITLE:=DSL CPE API driver for VRX Family
	URL:=http://www.lantiq.com/
	MAINTAINER:=Lantiq Beteiligungs-GmbH & Co. KG
	DEPENDS:=+kmod-dsl-cpe-mei-vrx
	KCONFIG:=CONFIG_LTQ_DSL_CPE_API_VRX=y
	FILES:=$(PKG_BUILD_DIR)/$(MAKE_PATH)/src/drv_dsl_cpe_api$(PKG_MODULE_EXTENSION)
	AUTOLOAD:=$(call AutoLoad,45,drv_dsl_cpe_api)
endef

define KernelPackage/$(PKG_NAME)/description
	Lantiq DSL CPE API Kernel Module Driver
endef

export KBUILD_EXTRA_SYMBOLS:=$(STAGING_DIR)/modulesym/dsl-cpe-mei-vrx.symvers

# avoid build directly in the source directories
PKG_AUTOMAKE_PATHS = .
CONFIGURE_PATH = build_$(PKGARCH)/$(BUILD_VARIANT)
CONFIGURE_CMD = ../../configure
MAKE_PATH = $(CONFIGURE_PATH)
PKG_INSTALL_DIR = $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)/ipkg_install

CONFIGURE_ARGS += --enable-kernel-include="$(LINUX_DIR)/include"

ifeq ($(BUILD_VARIANT),debug)
ifdef CONFIG_dsl-cpe-mei-vrx_DSL_NETLINK_ID
CONFIGURE_ARGS += \
	--enable-debug-logger-support \
	--with-dbg-netlink-id="$(DSL_NETLINK_ID)"
endif
endif

CONFIGURE_ARGS += --enable-vrx \
	--enable-ifxos \
	--enable-ifxos-include="-I$(STAGING_DIR)/usr/include/ifxos" \
	--enable-driver-include="-I$(STAGING_DIR)/usr/include" \
	--enable-add-drv-cflags="" \
	--enable-adsl-led=no \
	--enable-adsl-mib=no \
	--enable-dsl-ceoc=no

ifeq ($(CONFIG_dsl-cpe-mei-vrx_DEVICE_VR9),y)
CONFIGURE_ARGS += --enable-vrx-device=vr9
CONFIGURE_ARGS += --enable-firmware-r9
else ifeq ($(CONFIG_dsl-cpe-mei-vrx_DEVICE_VR10),y)
CONFIGURE_ARGS += --enable-vrx-device=vr10
CONFIGURE_ARGS += --enable-firmware-r9
else ifeq ($(CONFIG_dsl-cpe-mei-vrx_DEVICE_VR10_320),y)
CONFIGURE_ARGS += --enable-vrx-device=vr10_320
CONFIGURE_ARGS += --enable-firmware-r9
else ifeq ($(CONFIG_dsl-cpe-mei-vrx_DEVICE_VR11),y)
CONFIGURE_ARGS += --enable-vrx-device=vr11
else
CONFIGURE_ARGS += --enable-vrx-device=vr9
CONFIGURE_ARGS += --enable-firmware-r9
endif

ifeq ($(BUILD_VARIANT),release)
ifeq ($(CONFIG_$(PKG_NAME)_MODEL_LITE),y)
	CONFIGURE_ARGS += --enable-model=lite
	DRV_BILD_MODEL="lite"
else ifeq ($(CONFIG_$(PKG_NAME)_MODEL_FOOTPRINT),y)
	CONFIGURE_ARGS += --enable-model=footprint
	DRV_BILD_MODEL="footprint"
else ifeq ($(CONFIG_$(PKG_NAME)_MODEL_TYPICAL),y)
	CONFIGURE_ARGS += --enable-model=typical
	DRV_BILD_MODEL="typical"
else ifeq ($(CONFIG_$(PKG_NAME)_MODEL_FULL),y)
	CONFIGURE_ARGS += --enable-model=full
	DRV_BILD_MODEL="full"
else
	CONFIGURE_ARGS += --enable-model=footprint
	DRV_BILD_MODEL="footprint"
endif
else
CONFIGURE_ARGS += --enable-model=debug
DRV_BILD_MODEL="debug"
endif

ifeq ($(CONFIG_$(PKG_NAME)_USE_ADDITIONAL_BUILD_OPTIONS),y)
	CONFIGURE_ARGS += $(CONFIG_$(PKG_NAME)_ADDITIONAL_BUILD_OPTIONS)
endif

ifeq ($(CONFIG_$(PKG_NAME)_USE_DSL_ADD_EXT_DRV_CFLAGS),y)
	API_DRV_CFLAGS += $(CONFIG_$(PKG_NAME)_DSL_ADD_EXT_DRV_CFLAGS)
endif

ifeq ($(CONFIG_dsl-cpe-mei-vrx_PPA_NO),y)
	API_DRV_CFLAGS += -DDSL_DRV_ATM_PTM_INTERFACE_ENABLE=0
else
	API_DRV_CFLAGS += -DDSL_DRV_ATM_PTM_INTERFACE_ENABLE=1
endif

API_DRV_CFLAGS += -I$(STAGING_DIR)/include

CONFIGURE_ARGS += --enable-add_ext_drv_cflags="${API_DRV_CFLAGS}"

#ifeq ($(CONFIG_LINUX_2_6),y)
	CONFIGURE_ARGS += --enable-linux-26 --enable-kernelbuild="$(LINUX_DIR)" ARCH=$(LINUX_KARCH)
	PKG_MODULE_EXTENSION=.ko
#endif

ifeq ($(CONFIG_mips),y)
	EXTRA_CFLAGS = -fno-pic -mno-abicalls -mlong-calls -G 0
endif

define Build/Configure
	# configure $(BUILD_VARIANT)
	mkdir -p $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)
	# remove "prepared" and "configured" stamps of other configs
	@- rm $(PKG_BUILD_DIR)/.prepared*
	@- rm $(PKG_BUILD_DIR)/.configured*
	$(call Build/Configure/Default)
endef

define Build/Clean
	# clean $(BUILD_VARIANT)
	@- rm -Rf $(PKG_BUILD_DIR)/ipkg*
	@- [ -e $(PKG_BUILD_DIR)/$(MAKE_PATH)/Makefile ] && $(MAKE) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) clean
	@- rm -Rf $(PKG_BUILD_DIR)/$(CONFIGURE_PATH)
	$(call Build/Clean/Default)
endef

define Build/InstallDev
	# InstallDev $(BUILD_VARIANT)
	$(INSTALL_DIR) $(1)/usr/include/drv_dsl_cpe_api
	$(CP) $(PKG_BUILD_DIR)/src/include/drv_dsl_cpe*.h $(1)/usr/include/drv_dsl_cpe_api/
endef

define Package/$(PKG_NAME)/install
	# Install $(BUILD_VARIANT)

	$(INSTALL_DIR) $(1)/$(DSL_INITD_DIR)

	$(INSTALL_BIN) ./files/ltq_load_dsl_cpe_api.sh $(1)/$(DSL_INITD_DIR)
	$(SED) "s:@dsl_bin_dir@:$(DSL_BIN_DIR):g" $(1)/$(DSL_INITD_DIR)/ltq_load_dsl_cpe_api.sh

	$(INSTALL_DIR) $(1)/$(DSL_BIN_DIR)

	# Copy configuration files to rootfs
	@- [ -e ./files/dsl_auto_api.cfg ] && $(INSTALL_DATA) ./files/dsl_auto_api.cfg $(1)/$(DSL_BIN_DIR)

	# Set common variables within DSL config file
	$(SED) "s:@drv_build_model@:$(DRV_BILD_MODEL):g" $(1)/$(DSL_BIN_DIR)/dsl_auto_api.cfg;

	$(INSTALL_BIN) ./files/inst_drv_dsl_cpe_api.sh $(1)/$(DSL_BIN_DIR)/
	$(SED) "s:@dsl_bin_dir@:$(DSL_BIN_DIR):g" $(1)/$(DSL_BIN_DIR)/inst_drv_dsl_cpe_api.sh
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(MAKE_PATH)/src/drv_dsl_cpe_api$(PKG_MODULE_EXTENSION) $(1)/$(DSL_BIN_DIR)
	$(INSTALL_DIR) $(1)/$(MODULES_SUBDIR)
	$(INSTALL_DIR) $(1)/$(DSL_BIN_DIR)
	ln -snf /$(MODULES_SUBDIR)/drv_dsl_cpe_api$(PKG_MODULE_EXTENSION) $(1)/$(DSL_BIN_DIR)/$(PKG_BASE_NAME)$(PKG_MODULE_EXTENSION)
endef

define Package/$(PKG_NAME)-dbg/install
	# Install $(BUILD_VARIANT)

	$(INSTALL_DIR) $(1)/$(DSL_INITD_DIR)

	$(INSTALL_BIN) ./files/ltq_load_dsl_cpe_api.sh $(1)/$(DSL_INITD_DIR)
	$(SED) "s:@dsl_bin_dir@:$(DSL_BIN_DIR):g" $(1)/$(DSL_INITD_DIR)/ltq_load_dsl_cpe_api.sh
ifneq (y,$(CONFIG_NEW_FRAMEWORK))
ifeq (y,$(CONFIG_dsl-cpe-mei-vrx_DTM_STANDALONE))
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/ltq_load_dsl_cpe_api_standalone.sh $(1)/etc/init.d/
	$(SED) "s:@dsl_init_dir@:$(DSL_INITD_DIR):g" $(1)/etc/init.d/ltq_load_dsl_cpe_api_standalone.sh
	$(SED) "s:@start_seq@:$(STARTSEQ):g" $(1)/etc/init.d/ltq_load_dsl_cpe_api_standalone.sh
endif
endif

	$(INSTALL_DIR) $(1)/$(DSL_BIN_DIR)

	# Copy configuration files to rootfs
	@- [ -e ./files/dsl_auto_api.cfg ] && $(INSTALL_DATA) ./files/dsl_auto_api.cfg $(1)/$(DSL_BIN_DIR)

	# Set common variables within DSL config file
	$(SED) "s:@drv_build_model@:$(DRV_BILD_MODEL):g" $(1)/$(DSL_BIN_DIR)/dsl_auto_api.cfg;

	$(INSTALL_BIN) ./files/inst_drv_dsl_cpe_api.sh $(1)/$(DSL_BIN_DIR)/
	$(SED) "s:@dsl_bin_dir@:$(DSL_BIN_DIR):g" $(1)/$(DSL_BIN_DIR)/inst_drv_dsl_cpe_api.sh
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(MAKE_PATH)/src/drv_dsl_cpe_api$(PKG_MODULE_EXTENSION) $(1)/$(DSL_BIN_DIR)
	$(INSTALL_DIR) $(1)/$(MODULES_SUBDIR)
	$(INSTALL_DIR) $(1)/$(DSL_BIN_DIR)
	ln -snf /$(MODULES_SUBDIR)/drv_dsl_cpe_api$(PKG_MODULE_EXTENSION) $(1)/$(DSL_BIN_DIR)/$(PKG_BASE_NAME)$(PKG_MODULE_EXTENSION)
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)-dbg))
$(eval $(call KernelPackage,$(PKG_NAME)))
