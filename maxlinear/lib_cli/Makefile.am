## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = foreign 1.6 nostdinc

SUBDIRS = src
LINT_SUBDIRS = src

DISTCHECK_CONFIGURE_FLAGS=@CONFIGURE_OPTIONS@

lib_cli_docdir = ${docdir}

lib_cli_doc_DATA = \
	ChangeLog \
	LICENSE \
	README

EXTRA_DIST = \
	$(lib_cli_doc_DATA)

VXWORKS_6_6_MIPS_PRJFILES = \
	@srcdir@/build_vxworks/6.6_mips/.cproject \
	@srcdir@/build_vxworks/6.6_mips/.project \
	@srcdir@/build_vxworks/6.6_mips/.wrproject \
	@srcdir@/build_vxworks/6.6_mips/.wrmakefile

EXTRA_DIST += $(VXWORKS_6_6_MIPS_PRJFILES)

#	doc/lib_cli.dxcfg\
#	doc/footer.html\
#	doc/header.html\
#	doc/logo.gif\
#	doc/stylesheet.css


# Copy all the spec files. Of cource, only one is actually used.
dist-hook:
	for specfile in *.spec; do \
		if test -f $$specfile; then \
			cp -p $$specfile $(distdir); \
		fi \
	done

lint:
	@target=lint; \
	list='$(LINT_SUBDIRS)'; \
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

