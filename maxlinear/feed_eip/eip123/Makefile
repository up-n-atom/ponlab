#
# Openwrt Makefile for eip123
#

#### Includes ###################################
include $(TOPDIR)/rules.mk

#### Package Info ###############################
PKG_NAME:=eip123
PKG_VERSION:=1.0.12
PKG_RELEASE:=1
PKG_HASH:=5e392341728be367cc12a247a5b2b927c7631f6cb6f405f1d56c63fc4e26dfdf
PKG_BASE_NAME:=ltq_eip123
PKG_SOURCE:=$(PKG_BASE_NAME)-$(PKG_VERSION).tar.gz
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_BASE_NAME)-$(PKG_VERSION)
PKG_SOURCE_URL:=http://localhost
PKG_LIB_PATH:=$(PKG_BUILD_DIR)/ltq_eip123_user_library/Build/build_linux
PKG_APP_PATH:=$(PKG_BUILD_DIR)/linux_secure-boot_ddk
include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk

CONFIGURE_ARGS += \
	--enable-cm=2 \
	--enable-polling \
	--enable-target=versatile

define Package/$(PKG_NAME)
  SECTION:=net
  CATEGORY:=MaxLinear
  TITLE:=EIP 123 user space library
  DEPENDS:=@(TARGET_intel_mips_xrx500_4kec||TARGET_intel_mips_prx300_4kec) +libpthread +librt +kmod-eip123
  URL:=https://www.maxlinear.com
  MAINTAINER:=MaxLinear
endef

define Package/$(PKG_NAME)/description
	EIP-123 crypto library
endef

ifeq ($(CONFIG_BUILD_IA_ONLY),y)
EXTRA_CFLAGS += -DIA_STANDALONE
IA_STANDALONE=1
export IA_STANDALONE
else
ifeq ($(CONFIG_TEST_TSS),y)
TEST_TSS=1
export TEST_TSS
else
EXTRA_CFLAGS += -DSECURE_BUFFER
endif
endif

ifeq ($(CONFIG_DRM_NORMAL_FLOW),y)
EXTRA_CFLAGS += -DAES_OPT
endif

ifeq ($(CONFIG_DRM_FLUSH),y)
EXTRA_CFLAGS += -DTEST_FLUSH
endif

ifeq ($(CONFIG_UPDATE_OTP_ROLLBACK_ID),y)
EXTRA_CFLAGS += -DCONFIG_UPDATE_OTP_ROLLBACK_ID
endif

EXTRA_CFLAGS +=  -fPIC

CONFIGURE_PATH=ltq_eip123_user_library/Build/build_linux

#### Target Rules ###############################
define Package/$(PKG_NAME)/config
$(call Package/$(PKG_NAME)/override_version)
$(call Package/$(PKG_NAME)/override_source_path)
  menu "eip123 feature"
    depends on PACKAGE_$(PKG_NAME)
    source "$(SOURCE)/config/Config.in"
  endmenu
endef

ifeq ($(CONFIG_$(PKG_NAME)_USE_CUSTOM_SOURCE_DIR),y)
define Build/Configure
	export CONFIGURE_PATH
	$(call Build/Configure/Default)
endef

define Build/Prepare
	$(call Build/Prepare/Default)
	(cd $(PKG_LIB_PATH); \
		./bootstrap.sh;)
endef

define Build/Compile
	$(MAKE) -C $(PKG_LIB_PATH) all
	$(INSTALL_DIR) $(PKG_BUILD_DIR)/lib_inc
	$(MAKE) -C $(PKG_LIB_PATH) install DESTDIR=$(PKG_BUILD_DIR)/lib_inc
	$(MAKE_VARS) \
	$(MAKE) -C $(PKG_APP_PATH)/ \
                $(MAKE_FLAGS)
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/sbin
	# copying binaries
	$(CP) $(PKG_APP_PATH)/tss $(1)/usr/sbin/
ifeq ($(CONFIG_TEST_TSS),y)
	$(CP) $(PKG_APP_PATH)/test_tss $(1)/usr/sbin/
endif
	echo "Please do a make target/linux install after this"
endef

else

define Build/Compile
endef

define Package/$(PKG_NAME)/install
	echo "Please do a make target/linux install after this"
endef

endif

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lib_inc/usr/lib/* $(1)/usr/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lib_inc/usr/include/* $(1)/usr/include/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
