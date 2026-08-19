ARCH:=mips
SUBTARGET:=xrx500_4kec
BOARDNAME:=xRX500 BOOTCORE
# Use the same toolchain as the MIPS interAptiv CPU uses.
# This is MIPS 4KEc V7.4 CPU (mips32r2 + mips16, no dsp)
CPU_TYPE:=24kc
#CPU_SUBTYPE:=nomips16

KERNEL_PATCHVER:=4.9
LINUX_VERSION:=4.9.218

DEVICE_TYPE:=bootloader
DEFAULT_PACKAGES:=base-files libc libgcc busybox

define Target/Description
	Lantiq xRX500 Boot Core
endef
