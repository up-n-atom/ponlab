#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

BOARD:=intel_x86
BOARDNAME:=Intel x86
FEATURES:=squashfs ext4 vdi vmdk pcmcia targz fpu
VENDOR_PATH="/opt/intel"
MAINTAINER:=Intel

KERNEL_PATCHVER:=5.15

KERNELNAME:=bzImage
define Target/Description
	Build firmware images for Intel x86 SoCs
endef

include $(INCLUDE_DIR)/target.mk

#
# The 'Kernel/Patch' and 'Kernel/Perpare' definitions were patterend after include/kernel-defaults.mk.
# The definitions are placed here to avoid applying unwanted generic patches but allow specific target
# patches for an external Linux repo.
#
#   Create a patches subdirectory in the LINUX_DIR.
#   Install needed Kernel patches into ${LINUX_DIR}/patches.
#
ifeq ($(strip $(CONFIG_EXTERNAL_KERNEL_TREE)),"")

KPatchesDirs = patches
define Kernel/Patch =
	$(if $(QUILT),rm -rf $(LINUX_DIR)/patches; mkdir -p $(LINUX_DIR)/patches)
	find $(LINUX_DIR)/ -name \*.rej -or -name \*.orig | $(XARGS) rm -f
	$(foreach d,$(KPatchesDirs),$(newline)$(call PatchDir,$(LINUX_DIR),$d,platform/))
endef

define Kernel/Prepare
	$(LINUX_CAT) $(DL_DIR)/$(LINUX_SOURCE) | $(TAR) -C $(KERNEL_BUILD_DIR) $(TAR_OPTIONS)
	$(Kernel/Patch)
	$(if $(QUILT),touch $(LINUX_DIR)/.quilt_used)
endef

endif

DEFAULT_PACKAGES +=
#partx-utils mkf2fs e2fsprogs

$(eval $(call BuildTarget))

#$(eval $(call $(if $(CONFIG_TARGET_ROOTFS_ISO),RequireCommand,Ignore),mkisofs, \
#   	Please install mkisofs. \
#))
