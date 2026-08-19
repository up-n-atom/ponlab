## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = foreign 1.7 nostdinc

SUBDIRS = src

DISTCHECK_CONFIGURE_FLAGS=@CONFIGURE_OPTIONS@

pm_util_docdir = ${prefix}/doc/

pm_util_doc_DATA = \
	LICENSE \
	README \
	AUTHORS \
	ChangeLog \
	NEWS \
	TODO \
	doc/doxyconfig \
	doc/footer.html \
	doc/header.html \
	doc/logo.gif \
	doc/stylesheet.css

EXTRA_DIST = $(pm_util_doc_DATA)

clean-local:
	rm -Rf .built .built_check .version* .prepared*  ipkg/

# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

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

# Create self extracting linux distribution
distcheck-hook:
	chmod a+w $(distdir)
	echo "Checking line ends ...!!!"; \
	find $(distdir) -type f -exec file {} \; | grep -e "CRLF" -e "Non-ISO" && exit 1; \
	echo "Create installation package ...";

doc:
	( cd @top_srcdir@/doc; \
	doxygen doxyconfig; )

.PHONY: lint doc
