#
# Copyright (C) 2022 - 2023 MaxLinear, Inc.
# Copyright (c) 2017 - 2020 Intel Corporation
#
# This is free software, licensed under the GNU General Public License v2.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ltq_fw_PHY_IP
PKG_VERSION:=6.0
PKG_SOURCE_VERSION:=$(PKG_VERSION)
PKG_SOURCE_PROTO:=git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_PROJECT:=maxlinear
PKG_SOURCE_NAME:=ltq_fw_phy_ip
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_PROTO).tar.gz
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=1cb0ace6e936bc37022056814ccd78a7828732d65998c82cd454e320dc70a7ef
PKG_RELEASE:=1
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=MaxLinear

include $(INCLUDE_DIR)/package.mk


define Package/ltq_fw_PHY_IP/Default
  SECTION:=firmware
  CATEGORY:=MaxLinear
  URL:=http://www.maxlinear.com/
  MAINTAINER:=MaxLinear
endef

define Package/ltq-gphy-fw-xrx2xx-a1x-f
  $(call Package/ltq_fw_PHY_IP/Default)
  TITLE:=GPHY Firmware XRX2xx A1X Fast Ethernet
endef

define Package/ltq-gphy-fw-xrx2xx-a1x-g
  $(call Package/ltq_fw_PHY_IP/Default)
  TITLE:=GPHY Firmware XRX2xx A1X Gigabit Ethernet
endef

define Package/ltq-gphy-fw-xrx2xx-a2x-f
  $(call Package/ltq_fw_PHY_IP/Default)
  TITLE:=GPHY Firmware XRX2xx A2X Fast Ethernet
endef

define Package/ltq-gphy-fw-xrx2xx-a2x-g
  $(call Package/ltq_fw_PHY_IP/Default)
  TITLE:=GPHY Firmware XRX2xx A2X Gigabit Ethernet
endef

define Package/ltq-gphy-fw-xrx3xx-f
  $(call Package/ltq_fw_PHY_IP/Default)
  TITLE:=GPHY Firmware XRX3xx Fast Ethernet
endef

define Package/ltq-gphy-fw-xrx3xx-g
  $(call Package/ltq_fw_PHY_IP/Default)
  TITLE:=GPHY Firmware XRX3xx Gigabit Ethernet
endef

define Package/ltq-gphy-fw-xrx5xx
  $(call Package/ltq_fw_PHY_IP/Default)
  TITLE:=GPHY Firmware XRX5xx Gigabit Ethernet
endef

define Build/Compile
endef

define Package/ltq-gphy-fw-xrx2xx-a1x-f/install
	$(INSTALL_DIR) $(1)/lib/firmware
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/xRx2xx/ltq_fw_PHY22F_IP_1v4_xRx2xx_A14_T0109.bin $(1)/lib/firmware/mxl_fw_PHY22F_IP_xRx2xx_A14.bin
	$(LN) mxl_fw_PHY22F_IP_xRx2xx_A14.bin $(1)/lib/firmware/ltq_fw_PHY22F_IP_xRx2xx_A14.bin
endef

define Package/ltq-gphy-fw-xrx2xx-a1x-g/install
	$(INSTALL_DIR) $(1)/lib/firmware
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/xRx2xx/ltq_fw_PHY11G_IP_1v4_xRx2xx_A14_R8304.bin $(1)/lib/firmware/mxl_fw_PHY11G_IP_xRx2xx_A14.bin
	$(LN) mxl_fw_PHY11G_IP_xRx2xx_A14.bin $(1)/lib/firmware/ltq_fw_PHY11G_IP_xRx2xx_A14.bin
endef

define Package/ltq-gphy-fw-xrx2xx-a2x-f/install
	$(INSTALL_DIR) $(1)/lib/firmware
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/xRx2xx/ltq_fw_PHY22F_IP_1v5_xRx2xx_A22_RC434.bin $(1)/lib/firmware/mxl_fw_PHY22F_IP_xRx2xx_A22.bin
	$(LN) mxl_fw_PHY22F_IP_xRx2xx_A22.bin $(1)/lib/firmware/ltq_fw_PHY22F_IP_xRx2xx_A22.bin
endef

define Package/ltq-gphy-fw-xrx2xx-a2x-g/install
	$(INSTALL_DIR) $(1)/lib/firmware
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/xRx2xx/ltq_fw_PHY11G_IP_1v6_xRx2xx_A22_R8435.bin $(1)/lib/firmware/mxl_fw_PHY11G_IP_xRx2xx_A22.bin
	$(LN) mxl_fw_PHY11G_IP_xRx2xx_A22.bin $(1)/lib/firmware/ltq_fw_PHY11G_IP_xRx2xx_A22.bin
endef

define Package/ltq-gphy-fw-xrx3xx-f/install
	$(INSTALL_DIR) $(1)/lib/firmware
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/xRx3xx/ltq_fw_PHY22F_IP_1v5_xRx3xx_A21_RC434.bin $(1)/lib/firmware/mxl_fw_PHY22F_IP_xRx3xx_A21.bin
	$(LN) mxl_fw_PHY22F_IP_xRx3xx_A21.bin $(1)/lib/firmware/ltq_fw_PHY22F_IP_xRx3xx_A21.bin
endef

define Package/ltq-gphy-fw-xrx3xx-g/install
	$(INSTALL_DIR) $(1)/lib/firmware
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/xRx3xx/ltq_fw_PHY11G_IP_1v6_xRx3xx_A21_R8435.bin $(1)/lib/firmware/mxl_fw_PHY11G_IP_xRx3xx_A21.bin
	$(LN) mxl_fw_PHY11G_IP_xRx3xx_A21.bin $(1)/lib/firmware/ltq_fw_PHY11G_IP_xRx3xx_A21.bin
endef

define Package/ltq-gphy-fw-xrx5xx/install
	$(INSTALL_DIR) $(1)/lib/firmware
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/xRx5xx/ltq_fw_PHY11G_IP_1v1_xRx5xx_A21_R8549.bin $(1)/lib/firmware/mxl_fw_PHY11G_IP_xRx5xx_A21.bin
	$(LN) mxl_fw_PHY11G_IP_xRx5xx_A21.bin $(1)/lib/firmware/ltq_fw_PHY11G_IP_xRx5xx_A21.bin
endef


$(eval $(call BuildPackage,ltq-gphy-fw-xrx2xx-a1x-f))
$(eval $(call BuildPackage,ltq-gphy-fw-xrx2xx-a1x-g))
$(eval $(call BuildPackage,ltq-gphy-fw-xrx2xx-a2x-f))
$(eval $(call BuildPackage,ltq-gphy-fw-xrx2xx-a2x-g))
$(eval $(call BuildPackage,ltq-gphy-fw-xrx3xx-f))
$(eval $(call BuildPackage,ltq-gphy-fw-xrx3xx-g))
$(eval $(call BuildPackage,ltq-gphy-fw-xrx5xx))
