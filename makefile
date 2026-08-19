##################################################################################
#                                                                                #
#	Copyright (c) 2023 - 2024, MaxLinear, Inc.                               #
#                                                                                #
#  This software may be distributed under the terms of the BSD license.          #
#  See README for more details.                                                  #
##################################################################################
include makefile.inc

NOW = $(shell date +"%Y-%m-%d(%H:%M:%S %z)")

ifeq ($(CONFIG_MXL_WLAN_OSS_BUILD),y)
BUILD_TYPE:=oss_
else
BUILD_TYPE:=
endif
# Extra destination directories
PKGDIR = ./output/$(MACHINE)/pkg/

define create_changelog
	@$(ECHO) "Update changelog"
	mv CHANGELOG.md CHANGELOG.md.bak
	head -n 9 CHANGELOG.md.bak > CHANGELOG.md
	$(ECHO) "" >> CHANGELOG.md
	$(ECHO) "## Release $(VERSION) - $(NOW)" >> CHANGELOG.md
	$(ECHO) "" >> CHANGELOG.md
	$(GIT) log --pretty=format:"- %s" $$($(GIT) describe --tags | grep -v "merge" | cut -d'-' -f1)..HEAD  >> CHANGELOG.md
	$(ECHO) "" >> CHANGELOG.md
	tail -n +10 CHANGELOG.md.bak >> CHANGELOG.md
	rm CHANGELOG.md.bak
endef

# targets
all:
	$(MAKE) -C src all

clean:
	$(MAKE) -C src clean

install: all
	$(INSTALL) -d -m 0755 $(DEST)/etc/amx/wld/wld_defaults
	$(INSTALL) -D -p -m 0644 $(BUILD_TYPE)odl/wld_defaults/* $(DEST)/etc/amx/wld/wld_defaults/
	$(INSTALL) -d -m 0755 $(DEST)/etc/amx/wld/modules/$(COMPONENT)/$(COMPONENT)_defaults
	$(INSTALL) -D -p -m 0644 $(BUILD_TYPE)odl/$(COMPONENT)_definition.odl $(DEST)/etc/amx/wld/modules/$(COMPONENT)/$(COMPONENT)_definition.odl
	$(INSTALL) -D -p -m 0644 $(BUILD_TYPE)odl/$(COMPONENT).odl $(DEST)/etc/amx/wld/modules/$(COMPONENT)/$(COMPONENT).odl
	$(INSTALL) -D -p -m 0755 src/$(COMPONENT).so $(DEST)/usr/lib/amx/wld/modules/$(COMPONENT).so
	$(INSTALL) -D -p -m 0755 scripts/modPreInit.sh $(DEST)/usr/lib/amx/wld/modules/mod_init/modPreInit.sh

package: all
	$(INSTALL) -d -m 0755 $(PKGDIR)/etc/amx/wld/wld_defaults
	$(INSTALL) -D -p -m 0644 $(BUILD_TYPE)odl/wld_defaults/* $(PKGDIR)/etc/amx/wld/wld_defaults/
	$(INSTALL) -d -m 0755 $(PKGDIR)/etc/amx/wld/modules/$(COMPONENT)/$(COMPONENT)_defaults
	$(INSTALL) -D -p -m 0644 $(BUILD_TYPE)odl/$(COMPONENT)_definition.odl $(PKGDIR)/etc/amx/wld/modules/$(COMPONENT)/$(COMPONENT)_definition.odl
	$(INSTALL) -D -p -m 0644 $(BUILD_TYPE)odl/$(COMPONENT).odl $(PKGDIR)/etc/amx/wld/modules/$(COMPONENT)/$(COMPONENT).odl
	$(INSTALL) -D -p -m 0755 src/$(COMPONENT).so $(PKGDIR)/usr/lib/amx/wld/modules/$(COMPONENT).so
	$(INSTALL) -D -p -m 0755 scripts/modPreInit.sh $(PKGDIR)/usr/lib/amx/wld/modules/mod_init/modPreInit.sh
	cd $(PKGDIR) && $(TAR) -czvf ../$(COMPONENT)-$(VERSION).tar.gz .
	cp $(PKGDIR)../$(COMPONENT)-$(VERSION).tar.gz .
	make -C packages

changelog:
	$(call create_changelog)

doc:
	$(eval ODLFILES += odl/$(COMPONENT)_definition.odl)
	$(eval ODLFILES += odl/$(COMPONENT).odl)

	mkdir -p output/xml
	mkdir -p output/html
	mkdir -p output/confluence
	amxo-cg -Gxml,output/xml/$(COMPONENT).xml $(or $(ODLFILES), "")
	amxo-xml-to -x html -o output-dir=output/html -o title="$(COMPONENT)" -o version=$(VERSION) -o sub-title="Datamodel reference" output/xml/*.xml
	amxo-xml-to -x confluence -o output-dir=output/confluence -o title="$(COMPONENT)" -o version=$(VERSION) -o sub-title="Datamodel reference" output/xml/*.xml

.PHONY: all clean changelog install package doc
