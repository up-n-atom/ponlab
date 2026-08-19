## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4

AUTOMAKE_OPTIONS = foreign 1.9 nostdinc

SUBDIRS = src lib cli chk

DISTCHECK_CONFIGURE_FLAGS=@CONFIGURE_OPTIONS@

mod_pon_mcc_docdir = ${docdir}

mod_pon_mcc_INCLUDES = \
	include/pon/pon_mcc.h

mod_pon_mcc_doc_DATA = \
	ChangeLog \
	LICENSE \
	README.md

EXTRA_DIST = $(mod_pon_mcc_doc_DATA) \
	$(mod_pon_mcc_INCLUDES) \
	doc/doxyconfig

clean-local:
	rm -Rf .built .built_check .version* .prepared*  ipkg*/

# Copy all the spec files. Of course, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

CHECK_SYNTAX=checkpatch.pl -f --no-tree --terse --show-types \
	--ignore LINUX_VERSION_CODE \
	--ignore LONG_LINE_STRING \
	--ignore CONST_STRUCT

check-style:
	@for dir in $(SUBDIRS); do \
		(make -C $$dir check-style CHECK_SYNTAX="$(CHECK_SYNTAX)"); \
	done

distcheck-hook:
	chmod a+w $(distdir)
	echo "Checking line ends ..."; \
	find $(distdir) -type f -exec file {} \; | grep -e "CRLF" -e "Non-ISO" && exit 1; \
	echo "Done!"

doc:
	( cd @top_srcdir@/doc; \
	doxygen doxyconfig; )

.PHONY: doc
