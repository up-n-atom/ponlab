#
#
#                           Copyright (c) 2014-2015
#                       Lantiq Beteiligungs-GmbH & Co.KG
#                            http://www.lantiq.com
#
#  For licensing information, see the file 'LICENSE' in the root folder of
#  this software module.
#
#

if WITH_KERNEL_MODULE
if KERNEL_2_6
bin_PROGRAMS = drv_sdd_mbx.ko
# dummy file required for distcheck target
drv_sdd_mbx_ko_SOURCES = @top_srcdir@/LICENSE
else
bin_PROGRAMS = drv_sdd_mbx
endif
endif

## Common SDD mailbox driver sources
drv_sdd_mbx_SOURCES = \
   drv_sdd_mbx_handler.c \
   drv_sdd_mbx_linux.c \
   drv_sdd_mbx_firmware.c \
   drv_sdd_mbx_interrupt.c \
   drv_sdd_mbx_spi.c \
   drv_sdd_mbx_registers.c \
   drv_sdd_mbx_trace.c

EXTRA_DIST = \
   ../include/drv_sdd_mbx.h \
   ../include/drv_sdd_mbx_handler.h \
   ../include/drv_sdd_mbx_linux.h \
   ../include/drv_sdd_mbx_spi.h \
   ../include/drv_sdd_mbx_firmware.h \
   ../include/drv_sdd_mbx_interrupt.h \
   ../include/drv_sdd_mbx_version.h \
   ../include/drv_sdd_mbx_registers.h \
   ../include/drv_sdd_mbx_trace.h

nodist_drv_sdd_mbx_SOURCES =

nodist_EXTRA_drv_sdd_mbx_SOURCES =

INCLUDES_NO_KERNEL = \
   -I@abs_top_builddir@ \
   -I@abs_top_srcdir@ \
   -I@abs_srcdir@ \
   -I@IFXOS_INCL_PATH@ \
   -I@abs_top_srcdir@/src \
   -I@abs_top_srcdir@/include \
   -I@abs_top_srcdir@/src/lib/lib_bbd/src

AM_CPPFLAGS = \
   $(INCLUDES_NO_KERNEL)


AM_CPPFLAGS += \
   -I@KERNEL_INCL_PATH@ \
   -I@KERNEL_INCL_PATH@/asm/mach-generic \
   -I@KERNEL_BUILD_PATH@/include \
   -I@KERNEL_BUILD_PATH@/include2

if EVENT_LOGGER_DEBUG
AM_CPPFLAGS += -I@EVENT_LOGGER_INCL_PATH@
endif

AM_CFLAGS = -Wall -Wimplicit -Wreturn-type -Wunused -Wswitch -Wcomment -Wparentheses
drv_sdd_mbx_CFLAGS =-D__LINUX__ -DLINUX -D__KERNEL__ -DMODULE -DEXPORT_SYMTAB $(AM_CFLAGS) -fno-common
drv_sdd_mbx_LDFLAGS = -r -nostdlib


# -H
ghs:
	ccppc $(addprefix @srcdir@/,$(drv_sdd_mbx_SOURCES)) $(INCLUDES_NO_KERNEL) \
	-c -gcc -Wall -bigendian -D__BYTE_ORDER=__BIG_ENDIAN \
	-DHAVE_CONFIG_H -DGENERIC_OS -DGREENHILLS_CHECK

lint:
	@flint @top_srcdir@/std_lx.lnt $(AM_CPPFLAGS) \
	-I$(shell dirname `$(CC) -print-file-name=include`)/include \
	$(drv_sdd_mbx_SOURCES) $(nodist_drv_sdd_mbx_SOURCES)

install-exec-hook: $(bin_PROGRAMS)
	@for p in $(bin_PROGRAMS); do \
		$(STRIP) --strip-debug $(DESTDIR)$(bindir)/$$p; \
		echo "  $(STRIP) --strip-debug $(DESTDIR)$(bindir)/$$p"; \
	done

if WITH_KERNEL_MODULE
if KERNEL_2_6
drv_sdd_mbx_OBJS  = "$(subst .c,.o, $(drv_sdd_mbx_SOURCES))"

drv_sdd_mbx.ko: $(drv_sdd_mbx_SOURCES) $(nodist_drv_sdd_mbx_SOURCES) $(nodist_EXTRA_drv_sdd_mbx_SOURCES) $(EXTRA_DIST)
	@echo -e "Making Linux 2.6.x kernel object"
	@for f in $(drv_sdd_mbx_SOURCES) ; do \
		if test ! -e $(PWD)/$$f; then \
			echo "  LN      $$f" ; \
			mkdir -p $(PWD)/`dirname $$f`/; \
			ln -s @abs_srcdir@/$$f $(PWD)/$$f; \
		fi; \
	done;

	@echo -e "# Generated to build Linux 2.6.x kernel object" > $(PWD)/Kbuild
	@echo -e "obj-m := $(subst .ko,.o,$@)"			>> $(PWD)/Kbuild
	@echo -e "$(subst .ko,,$@)-y := $(drv_sdd_mbx_OBJS)"	>> $(PWD)/Kbuild
	@echo -e "EXTRA_CFLAGS := -DHAVE_CONFIG_H  $(drv_sdd_mbx_CFLAGS) $(AM_CPPFLAGS)" >> $(PWD)/Kbuild
	$(MAKE) ARCH=@KERNEL_ARCH@ -C @KERNEL_BUILD_PATH@ O=@KERNEL_BUILD_PATH@ M=$(PWD) modules

clean-generic:
	@echo "Cleanup Linux 2.6.x kernel object build"
	@- find . -name ".*.cmd" | xargs rm -f
	@- find . -name "*.o" | xargs rm -f
	@- rm -f Module.symvers Kbuild modules.order
	@- rm -rf .tmp_versions drv_sdd_mbx.mod.c
endif
endif
