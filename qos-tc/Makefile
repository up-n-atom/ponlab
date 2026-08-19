#
# Copyright (C) 2023 MaxLinear, Inc.
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=qos-tc

PKG_LICENSE:=GPL-2.0

include $(INCLUDE_DIR)/package.mk

define KernelPackage/qos-tc
  SUBMENU:=MaxLinear
  TITLE:= MaxLinear QoS TC Driver
  DEPENDS:=@(TARGET_intel_x86_lgm||TARGET_intel_mips_prx300) +kmod-sched-core \
           +kmod-sched-act-colmark +kmod-sched-act-police +kmod-sched-act-vlan \
           +kmod-sched-drr +kmod-sched-flower +kmod-sched-prio +kmod-sched-red
  KCONFIG:=CONFIG_QOS_TC
  FILES:=$(LINUX_DIR)/drivers/net/datapath/qos_tc/qos_tc.ko
  AUTOLOAD:=$(call AutoProbe,qos_tc)
endef

define KernelPackage/qos-tc/description
 MaxLinear Kernel module for QoS TC support.
endef

define KernelPackage/qos-tc/config
	menu "configuration"
	depends on PACKAGE_kmod-qos-tc

	config KERNEL_QOS_TC_KCOV
		bool "Enable KCOV instrumentation" if !KERNEL_KCOV_INSTRUMENT_ALL
		depends on KERNEL_KCOV
		help
		  This will enable KCOV instrumentation for the driver.

	config KERNEL_QOS_TC_TRACING
		bool "QoS TC tracing support"
		select KERNEL_FTRACE
		select KERNEL_ENABLE_DEFAULT_TRACERS
		help
		  Select this to enable qos_tc tracing infrastructure

	endmenu
endef

define Build/Configure
endef

define Build/Compile
endef

$(eval $(call KernelPackage,qos-tc))
