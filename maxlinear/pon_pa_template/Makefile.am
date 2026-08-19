## Process this file with automake to produce Makefile.in
ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = foreign 1.9 nostdinc

SUBDIRS = src

DISTCHECK_CONFIGURE_FLAGS=@CONFIGURE_OPTIONS@

pon_pa_template_docdir = ${docdir}

pon_pa_template_doc_DATA = \
	LICENSE \
	ChangeLog

EXTRA_DIST = \
	$(pon_pa_template_doc_DATA) \
	doc/doxyconfig

clean-local:
	rm -Rf .built .built_check .version* .prepared*  ipkg/

dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
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
