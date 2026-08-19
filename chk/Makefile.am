# Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = -Wall -Wno-portability

mod_pon_mcc_chk_sources = \
   pon_chk.c

mod_pon_mcc_test_interface = \
   ../include/pon/pon_mcc_ikm.h

EXTRA_DIST = \
   $(mod_pon_mcc_chk_sources)

AM_CFLAGS = \
	-DLINUX \
	-I@abs_srcdir@ \
	-I@abs_srcdir@/../include \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-Wno-sign-compare \
	-Wno-type-limits \
	-Wformat-security \
	-Wmissing-noreturn \
	-Wmissing-declarations \
	-Wmissing-prototypes \
	-Winit-self \
	-Wundef

includedir = $(prefix)/include/pon
include_HEADERS = $(mod_pon_mcc_test_interface)

if INCLUDE_LINUX_KERNEL

bin_PROGRAMS = mod_pon_mcc_chk.ko

clean-local:
	@echo "Cleanup Linux kernel object build"
	@- find . -name ".*.cmd" | xargs rm -f
	@- find . -name "*.o" | xargs rm -f
	@- rm -f Module.symvers Kbuild
	@- rm -rf .tmp_versions *.mod.c *.order

mod_pon_mcc_chk_ko_SOURCES = ../ChangeLog

mod_pon_mcc_chk_ko_OBJS = "$(subst .c,.o,$(filter %.c,$(mod_pon_mcc_chk_sources)))"
mod_pon_mcc_chk.ko$(EXEEXT): $(mod_pon_mcc_chk_sources) $(nodist_mod_pon_mcc_chk_sources)
	@echo "mod_pon_mcc_chk: Making Linux kernel object"
	@if test ! -e pon_ptp.c ; then \
		echo "copy source files (as links only!)"; \
		for f in $(filter %.c,$(mod_pon_mcc_chk_sources)); do \
			mkdir -p $(PWD)/`dirname $$f`/ ; \
			cp -sf $(addprefix @abs_srcdir@/,$$f) $(PWD)/`dirname $$f`/ ; \
		done \
	fi
	@echo "# mod_pon_mcc_chk: Generated to build Linux kernel object" > $(PWD)/Kbuild
	@echo "obj-m := $(subst .ko,.o,$@)"			>> $(PWD)/Kbuild
	@echo "$(subst .ko,,$@)-y := $(mod_pon_mcc_chk_ko_OBJS)"	>> $(PWD)/Kbuild
	@echo "EXTRA_CFLAGS := $(DEFS) $(AM_CFLAGS)" >> $(PWD)/Kbuild

#	Kernel Makefile exports LDFLAGS, which might pull-in wrong settings
	LDFLAGS= \
	$(MAKE) KBUILD_EXTRA_SYMBOLS="@abs_top_builddir@/src/Module.symvers" ARCH=@KERNEL_ARCH@ -C @KERNEL_BUILD_PATH@ O=@KERNEL_BUILD_PATH@ M=$(PWD) modules

endif

check-style:
	for f in $(filter %.h %.c,$(DISTFILES)); do \
		$(CHECK_SYNTAX) $(addprefix @abs_srcdir@/,$$f); \
	done
