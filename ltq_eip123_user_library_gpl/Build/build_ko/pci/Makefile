#############################################################################
#
# Copyright (c) 2009-2013 INSIDE Secure B.V. All Rights Reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
##############################################################################

##############################################################################
##  Makefile for the Linux Kernel Driver UMDevXS
##  How to use :
##  i)    $ make
##  ii)   For cleaning : $ make clean
##  iii)  For installing: $ make install
##############################################################################

#### User Edit Section : Variables to be set by the User
##   User may not need to modify anything other than this section

  ##-->: { Here follow the variables to be set by the User

    # Set KERNEL_DIR to the path to the Kernel Source directory for which you
    # wish to compile.
    #KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
    KERNEL_DIR?=/usr/src/linux-headers-$(shell uname -r)

    #SBIN_DIR:=/opt/safezone-tm-nw/sbin
    SBIN_DIR:=$(SAFEZONE_CM_PATH)/sbin

  ##<--:  } End of User Edit Section

#### User should not need to modify anything below this line
##############################################################################

HOST_COMPILER:=gcc
COMPILER:=gcc
# cs_*.h files respond to ARCH_X86
EXTRA_CFLAGS += -DARCH_X86

EXTRA_CFLAGS += -DUMDEVXS_PCI
#EXTRA_CFLAGS += -DUMDEVXS_MEM
#EXTRA_CFLAGS += -DUMDEVXS_SIM

ifeq ($(KERNELRELEASE),)
    SFZ_ROOTDIR:=$(shell cd ../../.. ; pwd)
    export SFZ_ROOTDIR
    SFZ_BUILDDIR:=$(shell pwd)
    export SFZ_BUILDDIR
endif

# Include Paths
INCLUDE_ALL:= \
    -I$(SFZ_BUILDDIR) \
    -I$(SFZ_ROOTDIR)/Kit/DriverFramework/v4/Basic_Defs/incl \
    -I$(SFZ_ROOTDIR)/Kit/DriverFramework/v4/CLib_Abstraction/incl \
    -I$(SFZ_ROOTDIR)/Kit/Log/incl \
    -I$(SFZ_ROOTDIR)/Kit/Log/src/linux_kernel \
    -I$(SFZ_ROOTDIR)/Integration/UMDevXS/KernelPart/incl \
    -I$(SFZ_ROOTDIR)/Integration/UMDevXS/KernelPart/src

SOURCES:= \
    ../../../Integration/UMDevXS/KernelPart/src/umdevxs_lkm.o \
    ../../../Integration/UMDevXS/KernelPart/src/umdevxs_smbuf.o \
    ../../../Integration/UMDevXS/KernelPart/src/umdevxs_ofdev.o \
    ../../../Integration/UMDevXS/KernelPart/src/umdevxs_pcidev.o \
    ../../../Integration/UMDevXS/KernelPart/src/umdevxs_chrdev.o \
    ../../../Integration/UMDevXS/KernelPart/src/umdevxs_device.o \
    ../../../Integration/UMDevXS/KernelPart/src/umdevxs_simdev.o \
    ../../../Integration/UMDevXS/KernelPart/src/umdevxs_bufadmin.o \
    ../../../Integration/UMDevXS/KernelPart/src/umdevxs_interrupt.o

# Compiler Flags
WARNING_FLAGS=-Wall
#DEBUG_FLAGS=-g

EXTRA_CFLAGS += \
    $(INCLUDE_ALL) \
    $(WARNING_FLAGS) \
    $(DEBUG_FLAGS)

.PHONY = clean

default: umdevxs_k

obj-m:=umdevxs_k.o

umdevxs_k-objs:= $(SOURCES)

umdevxs_k:
#	echo "EXTRA_CFLAGS=$(EXTRA_CFLAGS)"
	make -C $(KERNEL_DIR) SUBDIRS=$(shell pwd) modules

clean: clean_module

clean_module:
	-rm -rf .tmp_versions *.o .*.cmd .*.flags *.mod.c *.ko *.symvers *.order *.markers \
       ../../../Integration/UMDevXS/KernelPart/src/*.o \
       ../../../Integration/UMDevXS/KernelPart/src/.*.cmd

install:
	@if [ "x$(SAFEZONE_CM_PATH)" == "x" ]; then \
        echo "FATAL: Please provide SAFEZONE_CM_PATH"; \
    else \
        echo "Installing to $(SBIN_DIR)"; \
        mkdir -p $(SBIN_DIR); \
        cp umdevxs_k.ko $(SBIN_DIR); \
        cp load.sh $(SBIN_DIR); \
        chmod +x $(SBIN_DIR)/load.sh; \
        cp unload.sh $(SBIN_DIR); \
        chmod +x $(SBIN_DIR)/unload.sh; \
        ls -l $(SBIN_DIR); \
    fi

# end of file
