PACKAGE:=pon_mbox_drv
PACKAGE_VERSION:=1.28.1

# Set M to current directory, when not called via kernel build system
ifeq ($(M),)
M=.
endif

# Check if current dir is under git control, before extracting extra version details
ifneq ($(wildcard $(M)/.git),)
latest_tag:=$(shell git -C $(M) describe --tags --abbrev=0)
EXTRA_VERSION:=$(shell git -C $(M) rev-list HEAD "^$(latest_tag)" --count 2>/dev/null || echo 0)
ifneq ($(shell git -C $(M) diff-index --quiet HEAD || echo dirty),)
EXTRA_VERSION:=$(EXTRA_VERSION)-dirty
else
# The '-g' prefix before the git hash is a common convention (e.g., 1.28.1-5-gabcdef1).
# Ensure all downstream consumers expect this format.
EXTRA_VERSION:=$(EXTRA_VERSION)-g$(shell git -C $(M) rev-parse --short=7 HEAD)
endif
PACKAGE_VERSION:=$(PACKAGE_VERSION)-$(EXTRA_VERSION)
endif

ifneq ($(KERNELRELEASE),)

subdir-ccflags-y += -DPACKAGE_VERSION=$(PACKAGE_VERSION)
obj-y  += eth/
obj-y  += ptp/
obj-y  += vuni/
obj-y  += src/
obj-y  += test/

else

SRC := $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC)

modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules_install

clean:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) clean

CHECK_SYNTAX=checkpatch.pl -f --no-tree --terse --show-types \
	--ignore LINUX_VERSION_CODE,PREFER_PACKED,SPLIT_STRING,LONG_LINE_STRING,FILE_PATH_CHANGES

check-style:
	@for dir in ./src ./eth ./ptp ./vuni ./test ; do \
		(make -C $$dir check-style CHECK_SYNTAX="$(CHECK_SYNTAX)"); \
	done

distcheck: dist

PACKAGE_EXCLUDE=^(scripts/|unit_tests/|build_unit_tests/|build_win32/)

dist:
	git ls-tree -r HEAD --name-only | \
		grep -v -E "$(PACKAGE_EXCLUDE)" | \
		tar cfz $(PACKAGE)-$(PACKAGE_VERSION).tar.gz --transform='s/^/$(PACKAGE)-$(PACKAGE_VERSION)\//' -T -

doc:
	cd doc && doxygen doxyconfig; cd ..

doc-internals:
	cd doc && doxygen doxyconfig_internals; cd ..

.PHONY: all clean dist distcheck doc-internals modules_install check-style

endif
