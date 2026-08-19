## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4

AUTOMAKE_OPTIONS = foreign 1.9 nostdinc

SUBDIRS = src
DIST_SUBDIRS = src

DISTCHECK_CONFIGURE_FLAGS=@CONFIGURE_OPTIONS@ --disable-lcov-build --disable-tests-build

pon_net_lib_docdir = ${docdir}

pon_net_lib_doc_DATA = \
	LICENSE \
	README.md \
	ChangeLog

EXTRA_DIST = \
	$(pon_net_lib_doc_DATA) \
	doc/doxyconfig

clean-local:
	rm -Rf .built .built_check .version* .prepared*  ipkg/

dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

CHECK_SYNTAX=checkpatch.pl -f --no-tree --terse \
	--show-types \
	--ignore BLOCK_COMMENT_STYLE,FILE_PATH_CHANGES,LONG_LINE_STRING \
	--ignore PREFER_PRINTF \
	--ignore MACRO_WITH_FLOW_CONTROL \
	--ignore PREFER_PACKED \
	--ignore COMPLEX_MACRO

check-style:
	@for dir in src ; do \
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

.PHONY: lint doc
