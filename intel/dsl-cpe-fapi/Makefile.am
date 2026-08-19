## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = foreign 1.9 nostdinc

SUBDIRS = src tools

DISTCHECK_CONFIGURE_FLAGS=@CONFIGURE_OPTIONS@

fapi_docdir = ${prefix}/doc/${PACKAGE}

fapi_doc_DATA = \
	LICENSE \
	README \
	AUTHORS \
	ChangeLog \
	NEWS \
	TODO \
	doc/doxyconfig \
	doc/footer.html \
	doc/header.html \
	doc/dsl_cpe_fapi.h \
	doc/fapi_sw_structure_overview-noref.jpg \
	doc/html/logo.gif \
	doc/html/spacer.gif \
	doc/html/stylesheet.css

EXTRA_DIST = $(fapi_doc_DATA)

clean-local:
	rm -Rf .built .built_check .version* .prepared*  ipkg/

dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

# Check for correct line ends
distcheck-hook:
	chmod a+w $(distdir)
	echo "Checking line ends ...!!!"; \
	find $(distdir) -type f -exec file {} \; | grep -e "CRLF" -e "Non-ISO" && exit 1; \
	echo "Checking line ends ... done."

generate-doc:
	(cd @srcdir@/doc; \
	doxygen doxyconfig; )

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

