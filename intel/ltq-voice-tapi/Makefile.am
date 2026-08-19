#
#  Copyright (c) 2006-2009 Infineon Technologies AG
#  Copyright (c) 2009-2015 Lantiq Deutschland GmbH
#  Copyright (c) 2015-2016 Lantiq Beteiligungs-GmbH & Co.KG
#  Copyright (c) 2016-2017, Intel Corporation.
#
#  For licensing information, see the file 'LICENSE' in the root folder of
#  this software module.
#
#


AUTOMAKE_OPTIONS = foreign 1.6 nostdinc

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src xml

# set distcheck options - see configure.in for list of modifications
DISTCHECK_CONFIGURE_FLAGS=@CONFIGURE_OPTIONS@
CONFIG_CLEAN_FILES=src/modules.order
drv_tapi_docdir = ${prefix}/doc/drv_tapi

EXTRA_DIST = $(drv_tapi_doc_DATA)\
	ChangeLog \
	CHANGELOG_TAPI_V3 \
	doc/doxyconfig_v3.dxy \
	doc/footer.html\
	doc/header.html\
	doc/html/logo.jpg\
	doc/stylesheet.css \
	script/updateIoctl.pl\
	script/addLibtool.sh\
	drv_tapi_version.h \
	script/prepare-vxworks-6-4.pl \
	script/prepare-vxworks-6-6.pl

VXWORKS_6_4_PRJFILES = \
	vxworks_6.4/.project \
	vxworks_6.4/.wrproject \
	vxworks_6.4/.wrmakefile
VXWORKS_6_6_PRJFILES = \
	vxworks_6.6/.cproject \
	vxworks_6.6/.project \
	vxworks_6.6/.wrproject \
	vxworks_6.6/.wrmakefile

EXTRA_DIST += $(VXWORKS_6_4_PRJFILES) $(VXWORKS_6_6_PRJFILES)

if VXWORKS_6_4_DIST
VXWORKS_PRJFILES = $(VXWORKS_6_4_PRJFILES)
else
VXWORKS_PRJFILES = $(VXWORKS_6_6_PRJFILES)
endif

if LINUX
if KERNEL_2_6
EXT = .ko
endif
endif

# Make subdirectories inside tar.gz writable for everyone
# (required for 2.6 kbuild)
dist-hook: $(VXWORKS_PRJFILES)
	@echo "Make subdirectories writable... "; \
	find $(distdir) -type d -exec chmod a+w {} \; -print
	@for f in $(VXWORKS_PRJFILES) ; do \
		echo "Copying file $$f to folder $(distdir)" ; \
		cp -p @srcdir@/$$f $(distdir) ; \
	done


# Create self extracting linux distribution
distcheck-hook:
	@echo "Make subdirectories writable... "; \
	find $(distdir) -type d -exec chmod a+w {} \; -print
	@echo -n "Checking for the line ending style... "; \
	failure_list="`find $(distdir) \( -path '$(distdir)/doc' -o -path '$(distdir)/vxworks_6.6' -o -path '$(distdir)/vxworks_6.4' \) -prune -o -type f -exec grep -UIqsc $$'\r$$' {} \; -printf '\tDOS line ending style: %p\n'`"; \
	if test -z "$$failure_list"; then \
		echo "Ok"; \
	else \
		echo "FAIL"; \
		echo "$$failure_list"; \
		echo; \
		exit -1; \
	fi


.PHONY: ghs
ghs:
	cd src && make ghs

.PHONY: lint
lint:
	@target=lint; \
	list='$(SUBDIRS)'; \
	for subdir in $$list; do \
		echo "Making $$target in $$subdir"; \
		if test "$$subdir" = "."; then \
			dot_seen=yes; \
			local_target="$$target-am"; \
		else \
			local_target="$$target"; \
		fi; \
		(cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
		|| eval $$failcom; \
	done;

.PHONY: errno
errno:
	@top_srcdir@/script/updateErrno.pl @top_srcdir@/include/drv_tapi_errno.h @top_srcdir@/include/drv_tapi_strerrno.h @top_srcdir@/src

.PHONY: style
style:
	find @top_srcdir@/src -type f -name '*.[hc]' -exec @top_srcdir@/script/checkStyle.pl {} \;

.PHONY: checkstack
checkstack:
	( \
		if test -z "$(host_alias)" ; then \
			objdump -d src/drv_tapi$(EXT) | @top_srcdir@/script/checkstack.pl ; \
		else \
		    $(host_alias)-objdump -d src/drv_tapi$(EXT) | @top_srcdir@/script/checkstack.pl $(host_alias) ; \
		fi ;\
	)

.PHONY: ioctl
ioctl:
	@@top_srcdir@/script/updateIoctl.pl @top_srcdir@

.PHONY: examples
examples:
	@@top_srcdir@/script/checkExamples.pl @top_srcdir@ @IFXOS_INCL_PATH@
