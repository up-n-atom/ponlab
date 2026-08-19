include makefile.include

all: links pon_lib lib_ifxos lib_cli

clean: \
	lib_ifxos-clean \
	lib_cli-clean \
	pon_lib-clean \
	links-clean

distclean: \
	lib_ifxos-distclean \
	lib_cli-distclean \
	pon_lib-distclean \
	links-distclean

distcheck: \
	pon_lib-distcheck \
	links-distcheck

dirclean: \
	lib_ifxos-dirclean \
	lib_cli-dirclean \
	pon_lib-dirclean \
	links-dirclean

include makefile.pon_lib
include makefile.lib_ifxos
include makefile.lib_cli
include makefile.links
