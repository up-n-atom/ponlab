# eip-123 kernel style makefile

obj-$(CONFIG_CRYPTO_DEV_LANTIQ_EIP123) := umdevxs_k.o

umdevxs_k-y := \
    Integration/UMDevXS/KernelPart/src/umdevxs_lkm.o \
    Integration/UMDevXS/KernelPart/src/umdevxs_smbuf.o \
    Integration/UMDevXS/KernelPart/src/umdevxs_ofdev.o \
    Integration/UMDevXS/KernelPart/src/umdevxs_pcidev.o \
    Integration/UMDevXS/KernelPart/src/umdevxs_chrdev.o \
    Integration/UMDevXS/KernelPart/src/umdevxs_device.o \
    Integration/UMDevXS/KernelPart/src/umdevxs_simdev.o \
    Integration/UMDevXS/KernelPart/src/umdevxs_bufadmin.o \
    Integration/UMDevXS/KernelPart/src/umdevxs_interrupt.o

# add tool for some helper function
umdevxs_k-y += tool_eip123.o

# choose one of below for different bus or just simulation
#ccflags-y := -DUMDEVXS_PCI
ccflags-y := -DUMDEVXS_MEM
#ccflags-y := -DUMDEVXS_SIM

# do we seriously need the x86 flags for mips as well 
ccflags-y += -DARCH_X86

ccflags-y += \
    -I$(src) \
    -I$(src)/Kit/DriverFramework/v4/Basic_Defs/incl \
    -I$(src)/Kit/DriverFramework/v4/CLib_Abstraction/incl \
    -I$(src)/Kit/Log/incl \
    -I$(src)/Kit/Log/src/linux_kernel \
    -I$(src)/Integration/UMDevXS/KernelPart/incl \
    -I$(src)/Integration/UMDevXS/KernelPart/src
