ARCH:=x86_64
SUBTARGET:=lgm
BOARDNAME:=LGM
FEATURES:=squashfs ext4 nand pci usb gpio

LINUX_VERSION:=5.15
KERNEL_PATCHVER:=5.15

define Target/Description
	Intel LGM
endef
