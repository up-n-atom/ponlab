ARCH:=mips
SUBTARGET:=xrx500
BOARDNAME:=XRX500
FEATURES:=squashfs atm nand ubifs ramdisk usb
# interaptiv is not supported by GCC 5.4.0 and -mips32r3 optimizes for MIPS m4k CPU
CPU_TYPE:=24kc
CPU_SUBTYPE:=nomips16

KERNEL_PATCHVER:=4.9
LINUX_VERSION:=4.9.218

DEFAULT_PACKAGES+=kmod-intel_eth_drv_xrx500 ltq-gphy-fw-xrx5xx

define Target/Description
	Lantiq XRX500
endef
