#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=memutil
PKG_VERSION:=1.0

include $(INCLUDE_DIR)/package.mk


define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Memory utility
  DEPENDS += @KERNEL_DEVMEM
endef

define Package/memutil/description
	Display the contents of the memory.
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_LDFLAGS) -Wall \
		$(PKG_BUILD_DIR)/mem.c -o $(PKG_BUILD_DIR)/mem
endef

define Package/memutil/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mem $(1)/usr/bin/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))
