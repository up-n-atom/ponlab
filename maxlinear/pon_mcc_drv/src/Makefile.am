##Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = -Wall -Wno-portability

mod_pon_mcc_sources = \
   drv_pon_mcc.c \
   drv_pon_mcc_conv.c

mod_pon_mcc_includes = \
   drv_pon_mcc_conv.h

EXTRA_DIST = \
   $(mod_pon_mcc_sources) \
   $(mod_pon_mcc_includes)

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

if INCLUDE_LINUX_KERNEL

bin_PROGRAMS = mod_pon_mcc.ko

clean-local:
	@echo "Cleanup Linux kernel object build"
	@- find . -name ".*.cmd" | xargs rm -f
	@- find . -name "*.o" | xargs rm -f
	@- find . -name "*.o.d" | xargs rm -f
	@- rm -f Module.symvers Kbuild
	@- rm -rf .tmp_versions *.mod.c *.order

mod_pon_mcc_ko_SOURCES = ../ChangeLog

mod_pon_mcc_ko_OBJS = "$(subst .c,.o,$(filter %.c,$(mod_pon_mcc_sources)))"
mod_pon_mcc.ko$(EXEEXT): $(mod_pon_mcc_sources)
	@echo "mod_pon_mcc: Making Linux kernel object"
	@if test ! -e drv_pon_mcc.c ; then \
		echo "copy source files (as links only!)"; \
		for f in $(filter %.c,$(mod_pon_mcc_sources)); do \
			mkdir -p $(PWD)/`dirname $$f`/ ; \
			cp -sf $(addprefix @abs_srcdir@/,$$f) $(PWD)/`dirname $$f`/ ; \
		done \
	fi
	@echo "# mod_pon_mcc: Generated to build Linux kernel object" > $(PWD)/Kbuild
	@echo "obj-m := $(subst .ko,.o,$@)"			>> $(PWD)/Kbuild
	@echo "$(subst .ko,,$@)-y := $(mod_pon_mcc_ko_OBJS)"	>> $(PWD)/Kbuild
	@echo "EXTRA_CFLAGS := $(DEFS) $(AM_CFLAGS)" >> $(PWD)/Kbuild

#	Kernel Makefile exports LDFLAGS, which might pull-in wrong settings
	LDFLAGS= \
	$(MAKE) CONFIG_DEBUG_SECTION_MISMATCH=y ARCH=@KERNEL_ARCH@ -C @KERNEL_BUILD_PATH@ O=@KERNEL_BUILD_PATH@ M=$(PWD) KBUILD_EXTRA_SYMBOLS=@KBUILD_EXTRA_SYMBOLS@ modules

endif

check-style:
	for f in $(filter %.h %.c,$(DISTFILES)); do \
		$(CHECK_SYNTAX) $(addprefix @abs_srcdir@/,$$f); \
	done
