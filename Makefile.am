#
#                              Copyright (c) 2012
#                            Lantiq Deutschland GmbH
#
#  For licensing information, see the file 'LICENSE' in the root folder of
#  this software module.
#
#


AUTOMAKE_OPTIONS = foreign 1.6 nostdinc
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src

DISTCHECK_CONFIGURE_FLAGS=@CONFIGURE_OPTIONS@

lib_tapi_nlt_docdir = ${prefix}/doc/

lib_tapi_nlt_doc_DATA = \
    CHANGELOG \
    README \
    AUTHORS \
    LICENSE

EXTRA_DIST = \
    @top_srcdir@/include/lib_tapi_nlt_errno.h \
    @top_srcdir@/include/lib_tapi_lt_gr909.h \
    $(lib_tapi_nlt_doc_DATA) \
    doc/doxyconfig \
    doc/footer.html \
    doc/header.html \
    doc/logo.jpg \
    doc/stylesheet.css

# Make subdirectories inside tar.gz writable for everyone
# (required for 2.6 kbuild)
dist-hook:
	@echo "Make subdirectories writable... "; \
	find $(distdir) -type d -exec chmod 777 {} \; -print

# Create self extracting linux distribution
distcheck-hook:
	@echo -n "Checking for the line ending style... "; \
	failure_list="`find $(distdir) \( -path '$(distdir)/doc' -o -path '$(distdir)/vxworks_6.6' \) -prune -o -type f -exec grep -UIqsc $$'\r$$' {} \; -printf '\tDOS line ending style: %p\n'`"; \
	if test -z "$$failure_list"; then \
		echo "Ok"; \
	else \
		echo "FAIL"; \
		echo "$$failure_list"; \
		echo; \
		exit -1; \
	fi
	makeself.sh --gzip --notemp $(distdir) lib_tapi_nlt_cpe-${PACKAGE_VERSION}.sh \
	"${PACKAGE_NAME}"
