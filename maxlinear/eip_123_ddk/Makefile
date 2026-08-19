#
# wrapper compile eip 123 user space lib 
# Author  : William Widjaja
# contact :  W.Widjaja.EE@lantiq.com
# DAte : 19-9-2014

################# configurable options #########################

# your build machine 
AUTOTOOLS_BUILD_MACHINE  ?= $(shell $(CURDIR)/scripts/config.guess)
# target arch as in system type triplet 
# <YOUR-CROSS>-gcc -dumpmachine if you dont know
AUTOTOOLS_TARGET_MACHINE ?= mips-openwrt-linux-uclibc

DDK_FOLDER := ltq_eip123_user_library_gpl

################## dont touch unless needed #####################
TOPDIR      :=$(CURDIR)
INSTALL_DIR :=$(CURDIR)/lib_inc

CFGOPTS  := --target=$(AUTOTOOLS_TARGET_MACHINE) \
            --host=$(AUTOTOOLS_TARGET_MACHINE) \
            --build=$(AUTOTOOLS_BUILD_MACHINE) \
            --enable-cm=2 \
            --enable-polling \
            --enable-target=versatile \
            --prefix=/usr

# for debug
# CFGOPTS+= --enable-debug

.PHONY:all
all: clean prepare configure compile install

.PHONY:clean
clean:
	@echo cleaning...
	rm -rvf $(INSTALL_DIR)/*
        ifneq (,$(wildcard $(TOPDIR)/$(DDK_FOLDER)/Build/build_linux/Makefile))
	make -C $(TOPDIR)/$(DDK_FOLDER)/Build/build_linux clean
        endif

.PHONY:prepare
prepare:
	@echo preparing...
	cd $(TOPDIR)/$(DDK_FOLDER)/Build/build_linux && \
	./bootstrap.sh

.PHONY:configure
configure:
	@echo configuring...
	cd $(TOPDIR)/$(DDK_FOLDER)/Build/build_linux && \
        ./configure $(CFGOPTS)

.PHONY:compile
compile:
	@echo compiling....
	make -C $(TOPDIR)/$(DDK_FOLDER)/Build/build_linux all

.PHONY:install
install:
	@echo installing...
	make -C $(TOPDIR)/$(DDK_FOLDER)/Build/build_linux install DESTDIR=$(INSTALL_DIR)

