#
# Copyright © 2023-2024 MaxLinear, Inc.
#
# Secure crypto service client application uses libteec.so library APIs
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=seccrypto-client
PKG_VERSION:=1.0.0.0
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=MaxLinear
PKG_MXL_LICENSE:=MaxLinear/LICENSE

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=MaxLinear
  SUBMENU:=UGW Framework
  TITLE:=Secure crypto service client application
  DEPENDS:=+liboptee-client +sse_fapi_lgm +libopenssl
  URL:=http://www.maxlinear.com
  MAINTAINER:=MaxLinear Inc
endef

define Package/$(PKG_NAME)/description
	MaxLinear secure crypto service client application. This appliation
	establishes session with secure crypto service agent running on TEP to
	perfom secure crypto operations.
endef

TARGET_CFLAGS += -I$(LINUX_DIR)/include/uapi

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) -o $(PKG_BUILD_DIR)/seccrypto_client.o -c $(PKG_BUILD_DIR)/seccrypto_client.c
	$(TARGET_CC) -o $(PKG_BUILD_DIR)/$1 $(PKG_BUILD_DIR)/seccrypto_client.o -lteec -lsecurefapi -lcrypto -lssl
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/bin
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
