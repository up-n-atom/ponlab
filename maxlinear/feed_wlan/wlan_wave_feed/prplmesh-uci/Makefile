include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/version.mk

PKG_NAME:=prplmesh-uci
PKG_RELEASE:=master
PKG_SOURCE_URL:=https://gitlab.com/prpl-foundation/prplmesh/prplMesh.git
PKG_SOURCE_PROTO:=git
PKG_MIRROR_HASH:=d3240c6636be67a43f2b1d1692c236a7b737939050f4a31e684ae8e5af76ccf9
PKG_SOURCE_VERSION:=996857380999b3f0966017cecbd25120dd958acc
PKG_VERSION:=$(PKG_SOURCE_VERSION)

UGW_VERSION:=$(if $(wildcard $(TOPDIR)/ugw_version),$(shell cat $(TOPDIR)/ugw_version))
ifeq ($(UGW_VERSION),)
 #PRPL
 PKG_BUILD_DEPENDS:=
else
 #UGW
 PKG_BUILD_DEPENDS:=python-yaml/host
endif

NETIFD_VERSION_DIR:=$(shell awk 'f && f-- { print $$1 } /PKG_SOURCE_URL=/ { f = 1 }' $(TOPDIR)/package/network/config/netifd/Makefile | cut -b18-27)
PKG_LICENSE:=BSD-2-Clause-Patent
PKG_LICENSE_FILES:=LICENSES/BSD-2-Clause-Patent.txt

MAJOR_VERSION:=$(shell echo $(VERSION_NUMBER) | cut -f1 -d.)
MINOR_VERSION:=$(shell echo $(VERSION_NUMBER) | cut -f2 -d.)
PATCH_VERSION:=$(shell echo $(VERSION_NUMBER) | cut -f3 -d.)
CURR_OWRT_VERSION:=$(shell echo "$$(( $(MAJOR_VERSION) << 16 | $(MINOR_VERSION) << 8 | $(PATCH_VERSION) ))" )
OWRT_22_03_3:=$(shell echo "$$(( 22 << 16 | 03 << 8 | 3 ))" )
GE_OWRT_22_03_3:=$(shell [ $(CURR_OWRT_VERSION) -ge $(OWRT_22_03_3) ] && echo true)

#If OpenWRT version is >= 22.03.3 use OpenWRT based version directory, else use legacy netifd date based versioning.
ifeq ($(GE_OWRT_22_03_3),true)
	VERSION_DIR:=owrt_v22_03_3
else
	VERSION_DIR:=$(NETIFD_VERSION_DIR)
endif

# Disable Ninja CMake generator since it causes the CMake build command on UGW9.x to fail by
# running incorrectly the Ninja generator during the command.
PKG_USE_NINJA:=0

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/prplmesh-uci/Default
	SECTION:=net
  	CATEGORY:=Network
	TITLE:=prplmesh-uci
	URL:=https://gitlab.com/prpl-foundation/prplmesh/prplMesh/README.md
	MAINTAINER:=prplfoundation
	DEPENDS:=+libstdcpp +libpthread +librt +libjson-c +libopenssl +libnl +uci +ubus +ubox +bridge
endef

define Package/prplmesh-dwpal
$(call Package/prplmesh-uci/Default)
	TITLE+= - DWPAL
	VARIANT:=dwpal
	PROVIDES:=prplmesh-uci
	DEPENDS+= @TARGET_intel_mips||TARGET_intel_x86 +libdwpal +iwlwav-hostap +libsafec3
endef

define Package/prplmesh-nl80211
$(call Package/prplmesh-uci/Default)
	TITLE+= - NL80211
	VARIANT:=nl80211
	PROVIDES:=prplmesh-uci
	DEPENDS+= +hostapd-openssl
endef

CMAKE_SOURCE_DIR:=
CMAKE_OPTIONS+= \
	-DTARGET_PLATFORM=openwrt \
	-DCMAKE_INSTALL_PREFIX=/opt/prplmesh \
	-DPLATFORM_BUILD_DIR=$(BUILD_DIR) \
	-DPLATFORM_STAGING_DIR=$(STAGING_DIR) \
	-DPLATFORM_INCLUDE_DIR=$(STAGING_DIR)/usr/include \
	-DMSGLIB=None \
	-DREVISION=$(PKG_SOURCE_VERSION) \
	-DPLATFORM_LIB_DIRECTORY=$(STAGING_DIR_HOSTPKG)/lib \
	-H. -B./build

ifeq ($(BUILD_VARIANT),dwpal)
	CMAKE_OPTIONS+= -DBWL_TYPE=DWPAL\
		-DTARGET_PLATFORM_TYPE=ugw
endif

ifeq ($(BUILD_VARIANT),nl80211)
	CMAKE_OPTIONS+= -DBWL_TYPE=NL80211 \
		-DTARGET_PLATFORM_TYPE=turris-omnia
endif

Build/Compile:=cmake --build $(PKG_BUILD_DIR)/build -- $(MAKE_INSTALL_FLAGS) install -j
Build/Install:=
Build/Clean:=cd $(PKG_BUILD_DIR) && rm -rf .built .configured_* .prepared_* build/ ipkg-* || true

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/opt/prplmesh/lib/libbml* $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/opt/prplmesh/lib/libbtlvf* $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/opt/prplmesh/lib/libbcl* $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/opt/prplmesh/lib/libelpp* $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/opt/prplmesh/lib/libtlvf* $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/opt/prplmesh/include/beerocks/bml $(1)/usr/include/
endef

define Package/prplmesh-uci/install
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/usr/share/capabilities
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_DIR) $(1)/opt/prplmesh
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) ./files/etc/init.d/* $(1)/etc/init.d/
	$(CP) ./files/share/capabilities/$(VERSION_DIR)/* $(1)/usr/share/capabilities/
	$(INSTALL_BIN) ./files/99-prplmesh-update-config.sh $(1)/etc/uci-defaults/
	$(CP) ./files/etc/config/* $(1)/etc/config/
	$(CP) $(PKG_INSTALL_DIR)/opt/prplmesh/bin $(1)/opt/prplmesh/
	$(CP) $(PKG_INSTALL_DIR)/opt/prplmesh/scripts $(1)/opt/prplmesh/
	$(CP) $(PKG_INSTALL_DIR)/opt/prplmesh/lib/*.so* $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/opt/prplmesh/share $(1)/opt/prplmesh/
	$(CP) $(PKG_INSTALL_DIR)/opt/prplmesh/config $(1)/opt/prplmesh/
endef

define Package/prplmesh-dwpal/install
$(call Package/prplmesh-uci/install,$(1))
endef

define Package/prplmesh-nl80211/install
$(call Package/prplmesh-uci/install,$(1))
endef

$(eval $(call BuildPackage,prplmesh-dwpal))
$(eval $(call BuildPackage,prplmesh-nl80211))
