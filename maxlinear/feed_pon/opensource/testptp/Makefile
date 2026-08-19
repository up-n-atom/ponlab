#
# Copyright (C) 2016 Lantiq Beteiligungs-GmbH & Co. KG
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=testptp
PKG_RELEASE:=$(LINUX_VERSION)
PKG_LICENSE:=GPL-2.0-only

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Utilities
  DEPENDS:=+librt
  TITLE:=PTP testing utility
  VERSION:=$(LINUX_VERSION)-$(PKG_RELEASE)
  URL:=http://www.kernel.org
endef

ifeq ($(strip $(call CompareKernelPatchVer,$(KERNEL_PATCHVER),lt,4.9.0)),1)
  TESTPTP_FILE:=$(LINUX_DIR)/Documentation/ptp/testptp.c
else
  TESTPTP_FILE:=$(LINUX_DIR)/tools/testing/selftests/ptp/testptp.c
endif

define Package/$(PKG_NAME)/description
  PTP testing utility.
endef

define Build/Prepare
endef

TARGET_CFLAGS += -I$(LINUX_DIR)/user_headers/include
define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_LDFLAGS) -lrt \
		-o $(PKG_BUILD_DIR)/testptp $(TESTPTP_FILE)
endef

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/testptp $(1)/sbin/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
