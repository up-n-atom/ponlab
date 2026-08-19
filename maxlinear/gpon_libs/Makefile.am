ACLOCAL_AMFLAGS = -I m4

AUTOMAKE_OPTIONS = foreign 1.9 nostdinc

SUBDIRS = libtop

DISTCHECK_CONFIGURE_FLAGS=@CONFIGURE_OPTIONS@

gpon_libs_docdir = ${docdir}

gpon_libs_doc_DATA = \
	LICENSE \
	ChangeLog

EXTRA_DIST = \
	$(gpon_libs_doc_DATA)

clean-local:
	rm -Rf .built .built_check .version* .prepared*  ipkg/

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
