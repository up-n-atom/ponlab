#
# Copyright © 2020 MaxLinear, Inc.
# Makefile for cpu cycle profiling
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=cpu-cycle-profile
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/cpu-cycle-profile
  SUBMENU:=Hardware Monitoring Support
  TITLE:=Profiling Tool
  AUTOLOAD:=$(call AutoProbe,cpu_cycle_profile)
  FILES:=$(PKG_BUILD_DIR)/cpu_cycle_profile.ko
endef

define KernelPackage/cpu-cycle-profile/description
  Kernel module for doing profling.
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		$(KERNEL_MAKE_FLAGS) \
		SUBDIRS="$(PKG_BUILD_DIR)" \
		EXTRA_CFLAGS="$(BUILDFLAGS)" \
		modules
endef

$(eval $(call KernelPackage,cpu-cycle-profile))
