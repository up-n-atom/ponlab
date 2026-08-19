#
# Makefile for Intel EIP97 for linux 4.9
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ###############################
PKG_NAME:=eip97
PKG_VERSION:=1.4.10
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://gitlab.com/prpl-foundation/intel/eip97.git
PKG_MIRROR_HASH:=db174ec1c6491cc82a5b156779d8d27e001b6e740462dd87c3d14560b8c6b8c6
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=ce4edaca76d0e253c98eb1611a7a6b6f45fadb17
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_IS_KERNEL_MODULE:=y

include $(INCLUDE_DIR)/package.mk

define KernelPackage/$(PKG_NAME)
  SUBMENU:=Intel
  TITLE:=EIP97 crypto driver
  DEPENDS:=@TARGET_intel_mips_xrx500 +kmod-crypto-cmac \
		+kmod-crypto-aead +kmod-crypto-hash \
		+kmod-crypto-authenc +kmod-crypto-hmac \
		+kmod-crypto-md5 +kmod-crypto-sha1 \
		+kmod-crypto-sha256 +kmod-crypto-sha512 +kmod-ipsec4
  
  MAINTAINER:=Intel
  KCONFIG:= \
	CONFIG_CRYPTO_HW=y

  FILES:= \
	$(PKG_BUILD_DIR)/ltq_crypto.ko

  AUTOLOAD:=$(call AutoLoad,90,ltq_crypto,1)
endef

define KernelPackage/$(PKG_NAME)/description
  Kernel module to EIP-97 crypto library
endef

define KernelPackage/$(PKG_NAME)-TEST/description
  Kernel module to EIP-97 crypto library
endef

PKG_EXTRA_KCONFIG += \
	CONFIG_CRYPTO_DEV_LANTIQ_EIP97=m \
	CONFIG_LTQ_CRYPTO=y \
	CONFIG_LTQ_MPE_IPSEC_SUPPORT=y

PKG_EXTRA_CFLAGS += \
	-DCONFIG_LTQ_CRYPTO \
	-DCONFIG_CRYPTO_DEV_LANTIQ_EIP97 \
	-DCONFIG_LTQ_MPE_IPSEC_SUPPORT \
	-DCONFIG_LTQ_CRYPTO_MAX_RING_USED=2 \
	-DCONFIG_PPA_MPE_IP97

define Build/Compile
        $(MAKE) -C "$(LINUX_DIR)" \
                ARCH="$(LINUX_KARCH)" \
                CROSS_COMPILE="$(TARGET_CROSS)" \
                SUBDIRS="$(PKG_BUILD_DIR)/" \
		$(PKG_EXTRA_KCONFIG) \
		EXTRA_CFLAGS="$(PKG_EXTRA_CFLAGS)" \
                V="$(if $(V),1)" \
                modules
endef

define Build/Clean
	if [ -d $(PKG_BUILD_DIR) ]; then \
          $(MAKE) -C "$(LINUX_DIR)" \
                ARCH="$(LINUX_KARCH)" \
                CROSS_COMPILE="$(TARGET_CROSS)" \
                SUBDIRS="$(PKG_BUILD_DIR)/" \
		$(PKG_EXTRA_KCONFIG) \
		EXTRA_CFLAGS="$(PKG_EXTRA_CFLAGS)" \
                V="$(if $(V),1)" \
                clean; \
          $(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); \
	fi
endef

$(eval $(call KernelPackage,$(PKG_NAME)))
