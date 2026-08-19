#
# Openwrt Makefile
#
#### Includes ###################################
include $(TOPDIR)/rules.mk

#### Package Info ###############################
PKG_NAME:=ltq-xrx500-bootcore
PKG_RELEASE:=1
PKG_DEFAULT_CUSTOM_SOURCE_DIR:=$(BUILD_DIR_BASE)/xRX500_BootCore
PKG_TARGETS:=bin
PKG_CONFIG_DEPENDS:= \
	CONFIG_XRX500_BOOTCORE_MODEL \
	CONFIG_XRX500_BOOTCORE_IMAGE \
	CONFIG_XRX500_BOOTCORE_SECBOOT \
	CONFIG_XRX500_BOOTCORE_SIGNTOOL \
	CONFIG_XRX500_BOOTCORE_PROD_UNIQUE_KEY \
	CONFIG_XRX500_BOOTCORE_PRIVATE_KEY \
	CONFIG_XRX500_BOOTCORE_CERTIFICATION_SUPPORT \
	CONFIG_XRX500_BOOTCORE_CERTIFICATION \
	CONFIG_XRX500_BOOTCORE_ROLLBACK

include $(INCLUDE_DIR)/package.mk

PKG_BUILD_PARALLEL:=1

define Package/$(PKG_NAME)
	SECTION:=utils
	CATEGORY:=Lantiq
	SUBMENU:=Enable xRX500 BootCore build
	TITLE:=Lantiq xRX500 BootCore environment build
	URL:=http://www.lantiq.com
	MAINTAINER:=Lantiq
	MENU:=1
endef

define Package/$(PKG_NAME)/description
	This package is for building xRX500 BootCore model.
endef

BOOTCORE_SUFFIX:=$(shell echo $(CONFIG_XRX500_BOOTCORE_MODEL) |tr '[:upper:]' '[:lower:]')

### Menuconfig ##################################
define Package/$(PKG_NAME)/config
	menu "xRX500 Boot Core features"
		depends on PACKAGE_$(PKG_NAME)

		config XRX500_BOOTCORE_MODEL
		string "xrx500 Bootcore Model name"
		default ""
		help
		  Specify xrx500 Boot core model name for build

		config XRX500_BOOTCORE_IMAGE
		string "xrx500 Bootcore image name"
		default ""
		help
		  Specify xrx500 Boot core image name for build

		menuconfig XRX500_BOOTCORE_SECBOOT
			bool "enable Secure boot config"
			default INTEL_MIPS_SECBOOT

		if XRX500_BOOTCORE_SECBOOT
			config XRX500_BOOTCORE_CERTIFICATION_SUPPORT
				bool "Certification Support"
				default INTEL_MIPS_CERTIFICATION_SUPPORT

			config XRX500_BOOTCORE_CERTIFICATION
				string "certification (path)"
				default INTEL_MIPS_CERTIFICATION
				depends on XRX500_BOOTCORE_CERTIFICATION_SUPPORT

			config XRX500_BOOTCORE_SIGNTOOL
				string "signtool (path)" if XRX500_BOOTCORE_CERTIFICATION_SUPPORT
				default INTEL_MIPS_SIGNTOOL

			config XRX500_BOOTCORE_PROD_UNIQUE_KEY
				string "product unique key (path)" if XRX500_BOOTCORE_CERTIFICATION_SUPPORT
				default INTEL_MIPS_PROD_UNIQUE_KEY

			config XRX500_BOOTCORE_PRIVATE_KEY
				string "private key (path)" if XRX500_BOOTCORE_CERTIFICATION_SUPPORT
				default INTEL_MIPS_PRIVATE_KEY

			config XRX500_BOOTCORE_ROLLBACK
				int "rollback id (0-63)" if BROKEN
				range 0 63
				default INTEL_MIPS_ROLLBACK
				help
				  This is the rollback id for the bootcore image.
				  Setting this to a different value than the rollback id
				  of the main image can result in a broken system.
		endif

	endmenu
endef

#### Target Rules ###############################

define Build/Clean
	@rm -rf $(PKG_DEFAULT_CUSTOM_SOURCE_DIR)
	@rm -rf $(PKG_BUILD_DIR)
	@rm -rf $(BUILD_DIR_BASE)/*$(BOOTCORE_SUFFIX)
	@rm -rf $(TOPDIR)/staging_dir/*$(BOOTCORE_SUFFIX)
endef

define Build/Prepare
	echo Build/Prepare: ltq_bootcore_env_prepare.sh $(TOPDIR) $(PKG_BUILD_DIR)
	$(SHELL) ltq_bootcore_env_prepare.sh $(TOPDIR) $(PKG_BUILD_DIR)
endef

# no need to configure (./configure not existing anyway)
define Build/Configure
endef

# avoid exporting setting for cross-compilation, will break tools (and toolchain?) builds
define Build/Exports
endef

# override some variables and undefine others
MAKE_VARS:=OPENWRT_BUILD=0
MAKE_FLAGS:=-s V=$(OPENWRT_VERBOSE)

override undefine CFLAGS
override undefine GCC_HONOUR_COPTS

define Build/Compile
	$(call Build/Compile/Default)
endef

XRX500_BOOTCORE_IMAGE_FILE:=$(shell echo $(CONFIG_XRX500_BOOTCORE_IMAGE) | sed 's/_debug//g')

define Package/$(PKG_NAME)/install
	mkdir -p $(1)
	# This removes the _debug postfix from the name, this is used in the PRX300 debug images.
	cp -f $(PKG_DEFAULT_CUSTOM_SOURCE_DIR)/bin/targets/$(BOARD)/$(SUBTARGET)_4kec/openwrt-$(BOARD)-$(SUBTARGET)_4kec-$(CONFIG_XRX500_BOOTCORE_IMAGE)-initramfs-kernel.bin \
		$(1)/openwrt-$(BOARD)-$(SUBTARGET)_4kec-$(XRX500_BOOTCORE_IMAGE_FILE)-initramfs-kernel.bin
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
