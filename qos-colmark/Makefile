#
# Copyright (C) 2023 MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=qos-colmark

PKG_LICENSE:=GPL-2.0
PKG_LICENSE_FILES:=

include $(INCLUDE_DIR)/package.mk

define KernelPackage/sched-act-colmark
  SUBMENU:=$(NETWORK_SUPPORT_MENU)
  TITLE:=Color marking
  DEPENDS:=+kmod-sched-core
  KCONFIG:=CONFIG_NET_ACT_COLMARK
  FILES:=$(LINUX_DIR)/net/sched/act_colmark.ko
  AUTOLOAD:=$(call AutoProbe, act_colmark)
endef

define KernelPackage/sched-act-colmark/description
 Allows to configure rules to color mark packets.
endef

define Build/Configure
endef

define Build/Compile
endef

$(eval $(call KernelPackage,sched-act-colmark))
