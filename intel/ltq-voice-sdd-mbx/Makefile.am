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


AUTOMAKE_OPTIONS = foreign 1.7 nostdinc
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src

# set distcheck options - see configure.in for list of modifications
DISTCHECK_CONFIGURE_FLAGS=@CONFIGURE_OPTIONS@
CONFIG_CLEAN_FILES=src/modules.order

drv_dxt_docdir = ${prefix}/doc/drv_dxt

drv_dxt_doc_DATA = \
	README\
	LICENSE

EXTRA_DIST = $(drv_dxt_doc_DATA) \
	CHANGELOG \
	doc/lantiq,grx500-vcodec.txt \
	doc/doxyconfig \
	doc/footer.html \
	doc/header.html \
	doc/logo.jpg \
	doc/stylesheet.css

# Make subdirectories inside tar.gz writable for everyone
# (required for 2.6 kbuild)
dist-hook:
	find $(distdir) -type d -exec chmod 777 {} \; -print

# Create self extracting linux distribution
distcheck-hook:
	@echo "Make subdirectories writable... "; \
	find $(distdir) -type d -exec chmod a+w {} \; -print
	@echo -n "Checking for the line ending style... "; \
	failure_list="`find $(distdir) \( -path '$(distdir)/doc' \) -prune -o -type f -exec grep -UIqsc $$'\r$$' {} \; -printf '\tDOS line ending style: %p\n'`"; \
	if test -z "$$failure_list"; then \
		echo "Ok"; \
	else \
		echo "FAIL"; \
		echo "$$failure_list"; \
		echo; \
		exit -1; \
	fi