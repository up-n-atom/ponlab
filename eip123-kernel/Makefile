#
# Openwrt Makefile for eip123
#

#### Includes ###################################
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

#### Package Info ###############################
PKG_NAME:=eip123-kernel

PKG_RELEASE:=1
PKG_SOURCE_VERSION:=1.0.13
PKG_VERSION:=$(PKG_SOURCE_VERSION)
PKG_SOURCE_PROTO:=git
PKG_PROJECT:=maxlinear
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/eip_123_ddk.git
PKG_MIRROR_HASH:=b140ebae90b2f048697896be04ba94cea10703e0c66dfeed85d55532fa3cd4bf
PKG_MOD_PATH:=$(PKG_BUILD_DIR)/ltq_eip123_kernel_driver_gpl

include $(INCLUDE_DIR)/package.mk

define KernelPackage/eip123
  SUBMENU:=Cryptographic API modules
  TITLE:=EIP123 driver (umdevxs_k.ko)
  DEPENDS:=@(TARGET_intel_mips_xrx500_4kec||TARGET_intel_mips_prx300_4kec)
  FILES:=$(PKG_MOD_PATH)/umdevxs_k.ko
  AUTOLOAD:=$(call AutoProbe,umdevxs_k)
endef


KERNEL_MAKE_FLAGS += CONFIG_CRYPTO_DEV_LANTIQ_EIP123=m

define Build/Compile
	$(MAKE) $(PKG_JOBS) -C "$(LINUX_DIR)" \
		$(KERNEL_MAKE_FLAGS) \
		M="$(PKG_MOD_PATH)" \
		NOSTDINC_FLAGS="$(NOSTDINC_FLAGS)" \
		modules
endef

define Build/Install
	:
endef

$(eval $(call KernelPackage,eip123))
