#
# Copyright (c) 2020 Intel Corporation
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=afl
PKG_VERSION:=2.56b

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/google/AFL.git
PKG_SOURCE_VERSION:=v$(PKG_VERSION)

PKG_LICENSE:=Apache-2.0
PKG_LICENSE_FILES:=LICENSE

PKG_BUILD_PARALLEL:=1
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

MAKE_FLAGS += \
  AFL_NO_X86=1 \
  PREFIX=/usr

define Package/$(PKG_NAME)
  SECTION:=devel
  CATEGORY:=Development
  TITLE:=american fuzzy lop
  URL:=http://lcamtuf.coredump.cx/afl/
endef

AFL_BINS:=afl-analyze afl-cmin afl-fuzz afl-gcc afl-gotcpu afl-plot afl-showmap afl-tmin afl-whatsup

define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(foreach bin,$(AFL_BINS),$(PKG_INSTALL_DIR)/usr/bin/$(bin)) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/afl/afl-as $(1)/usr/bin/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
