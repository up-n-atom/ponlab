## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4

AUTOMAKE_OPTIONS = foreign 1.7 nostdinc

SUBDIRS = src/pontop

DISTCHECK_CONFIGURE_FLAGS=@CONFIGURE_OPTIONS@

pon_tools_docdir = ${docdir}

pon_tools_doc_DATA = \
	LICENSE \
	ChangeLog

EXTRA_DIST = \
	$(pon_tools_doc_DATA) \
	doc/doxyconfig

clean-local:
	rm -Rf .built .built_check .version* .prepared*  ipkg/

# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

CHECK_SYNTAX=checkpatch.pl -f --no-tree --terse
check-style:
	@for dir in $(SUBDIRS) ; do \
		(make -C $$dir check-style CHECK_SYNTAX="$(CHECK_SYNTAX)"); \
	done

distcheck-hook:
	chmod a+w $(distdir)
	echo "Checking line ends ..."; \
	find $(distdir) -type f -exec file {} \; | grep -e "CRLF" -e "Non-ISO" && exit 1; \
	echo "Done!"

doc: @srcdir@/doc/doxyconfig
	( cd @srcdir@/doc; \
	doxygen doxyconfig; )
