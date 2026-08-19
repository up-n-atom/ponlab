include $(TOPDIR)/rules.mk

PKG_NAME:=dwpal_6x-uci
PKG_REV:=086be2f98d42b57e17730e649e22f2fa18623acf
PKG_VERSION:=1.0.0.1
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=dwpal.git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME)
PKG_SOURCE_VERSION:=r597+a+41+a+17
PKG_SOURCE_PROTO:=git
PKG_MIRROR_HASH:=63698b3b3262fb6541e81593f76b8560d1cda6a12293bb570fe34e459cd61fad
PKG_BUILD_DIR:=$(BUILD_DIR)/dwpal_6x-uci-$(BUILD_VARIANT)/dwpal_6x-uci-$(PKG_VERSION)

PKG_BUILD_DEPENDS:=libnl ncurses safec3 libedit iwlwav-iw uci iwlwav-driver-uci

ifeq ($(BUILD_VARIANT), $(filter osp osp-debug, $(BUILD_VARIANT)))
 PKG_BUILD_DEPENDS += iwlwav-hostap-ng-uci
else
 PKG_BUILD_DEPENDS += iwlwav-hostap-uci
endif

PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=BSD 3-clause
PKG_LICENSE_FILES:=LICENSE

include $(INCLUDE_DIR)/package.mk
-include $(INCLUDE_DIR)/package-version-override.mk
define Package/dwpal_6x-uci
  SECTION:=net
  CATEGORY:=MaxLinear
  SUBMENU:=UGW Software
  TITLE:=D-WPAL Library
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
  DEPENDS:= +libnl +librt +libsafec3 +libncurses +libedit +iwlwav-hostap
  MENU:=1
  VARIANT:=release
  PROVIDES:=libdwpal
endef

define Package/dwpal_6x-uci/description
	D-WPAL: Dynamic Wireless Platform Abstraction Layer
endef

define Package/dwpal_6x-uci-debug
$(call Package/dwpal_6x-uci)
VARIANT:=debug
DEPENDS:= +libnl +librt +libsafec3 +libncurses +libedit +iwlwav-hostap
TITLE += with debug information
endef

define Package/dwpal_6x-uci-debug/description
$(call Package/dwpal_6x-uci/description) with debug information
endef

define Package/dwpal_6x-uci-osp
$(call Package/dwpal_6x-uci)
VARIANT:=osp
DEPENDS:= +libnl +librt +libsafec3 +libncurses +libedit +iwlwav-hostap
TITLE += with opensource packages
endef

define Package/dwpal_6x-uci-osp/description
$(call Package/dwpal_6x-uci/description) with opensource packages
endef

define Package/dwpal_6x-uci-osp-debug
$(call Package/dwpal_6x-uci)
VARIANT:=osp-debug
DEPENDS:= +libnl +librt +libsafec3 +libncurses +libedit +iwlwav-hostap
TITLE += with opensource packages and debug information
endef

define Package/dwpal_6x-uci-osp-debug/description
$(call Package/dwpal_6x-uci/description) with opensource packages and debug information
endef

define Package/dwpal_6x-uci/config
$(call Package/dwpal_6x-uci/override_version)
$(call Package/dwpal_6x-uci/override_source_path)
endef

define Package/dwpal_6x-uci-debug/config
$(call Package/dwpal_6x-uci-debug/override_version)
$(call Package/dwpal_6x-uci-debug/override_source_path)
endef

define Package/dwpal_6x-uci-osp/config
$(call Package/dwpal_6x-uci-osp/override_version)
$(call Package/dwpal_6x-uci-osp/override_source_path)
endef

define Package/dwpal_6x-uci-osp-debug/config
$(call Package/dwpal_6x-uci-osp-debug/override_version)
$(call Package/dwpal_6x-uci-osp-debug/override_source_path)
endef

LIBNL_DIR:=$(wildcard $(BUILD_DIR)/libnl-3*)

IWLWAV_HOSTAP_DIR:=$(wildcard $(BUILD_DIR)/iwlwav-hostap-uci-$(BUILD_VARIANT)/iwlwav-hostap-uci-*)
IWLWAV_IW_DIR:=$(wildcard $(BUILD_DIR)/iwlwav-iw-$(BUILD_VARIANT)/iwlwav-iw-*)
ifeq ($(BUILD_VARIANT),osp)
IWLWAV_IW_DIR:=$(wildcard $(BUILD_DIR)/iwlwav-iw-release/iwlwav-iw-*)
IWLWAV_HOSTAP_DIR:=$(wildcard $(BUILD_DIR)/iwlwav-hostap-ng-uci-release/iwlwav-hostap-ng-uci-*)
endif
ifeq ($(BUILD_VARIANT),osp-debug)
IWLWAV_IW_DIR:=$(wildcard $(BUILD_DIR)/iwlwav-iw-debug/iwlwav-iw-*)
IWLWAV_HOSTAP_DIR:=$(wildcard $(BUILD_DIR)/iwlwav-hostap-ng-uci-debug/iwlwav-hostap-ng-uci-*)
endif

IFX_CFLAGS += -I$(IWLWAV_IW_DIR) -I$(IWLWAV_HOSTAP_DIR)/src/common/ -I$(IWLWAV_HOSTAP_DIR)/src/utils/ -I$(LIBNL_DIR)/include -DHAVE_C99=1
IFX_LDFLAGS += -L$(STAGING_DIR)/usr/lib -L$(LIBNL_DIR)/lib/.libs -lnl-3 -lsafec-3.3
IFX_CFLAGS += -fstack-protector-strong -O2 -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security -Werror=format-security -Wall -fPIC -Wl,-strip-debug
IFX_LDFLAGS += -Wl,-z,noexecstack -Wl,-z,now -Wl,-z,relro
ifeq ($(BUILD_VARIANT), $(filter debug osp-debug, $(BUILD_VARIANT)))
IFX_CFLAGS += -DCONFIG_WAVE_DEBUG
endif

RELATIVE_NLV_PKG_HEADER_PATH:=include/vendor_cmds_copy.h
RELATIVE_NL_PKG_HEADER_PATH:=include/nl80211_copy.h

define Build/Compile
	../iwlwav-driver-uci/files/check_shared_headers.sh $(PKG_NAME) $(PKG_VERSION) $(BUILD_DIR) $(BUILD_VARIANT) $(RELATIVE_NLV_PKG_HEADER_PATH) $(RELATIVE_NL_PKG_HEADER_PATH)
	$(MAKE) $(TARGET_CONFIGURE_OPTS) -C $(PKG_BUILD_DIR) \
		CFLAGS="$(IFX_CFLAGS)" LDFLAGS="$(IFX_LDFLAGS)" IWLWAV_HOSTAP_DIR="$(IWLWAV_HOSTAP_DIR)" IWLWAV_IW_DIR="$(IWLWAV_IW_DIR)"
endef

define Package/dwpal_6x-uci/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/opt/intel/wave/scripts/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libdwpal.so $(1)/usr/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dwpal_cli $(1)/usr/sbin
	chmod 555 $(1)/usr/sbin/dwpal_cli
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libwv_core.so $(1)/usr/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libdwpald_client.so $(1)/usr/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dwpal_daemon $(1)/usr/sbin
	chmod 555 $(1)/usr/sbin/dwpal_daemon

ifeq ($(BUILD_VARIANT), $(filter debug osp-debug, $(BUILD_VARIANT)))
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dwpald_client_ref $(1)/usr/sbin
	chmod 555 $(1)/usr/sbin/dwpald_client_ref

	$(INSTALL_BIN) $(PKG_BUILD_DIR)/test_lib_wv_ipc $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/test_dwpal $(1)/usr/sbin
	chmod 555 $(1)/usr/sbin/test_lib_wv_ipc
	chmod 555 $(1)/usr/sbin/test_dwpal
endif

endef

Package/dwpal_6x-uci-debug/install = $(Package/dwpal_6x-uci/install)
Package/dwpal_6x-uci-osp/install = $(Package/dwpal_6x-uci/install)
Package/dwpal_6x-uci-osp-debug/install = $(Package/dwpal_6x-uci/install)

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libdwpal.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libwv_core.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libdwpald_client.so $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/include/*.h $(1)/usr/include/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/daemon/dwpald_client.h $(1)/usr/include/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/daemon/dwpald_hostap_parse.h $(1)/usr/include/
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/*.h $(1)/usr/include/
endef

$(eval $(call BuildPackage,dwpal_6x-uci))
$(eval $(call BuildPackage,dwpal_6x-uci-debug))
$(eval $(call BuildPackage,dwpal_6x-uci-osp))
$(eval $(call BuildPackage,dwpal_6x-uci-osp-debug))
