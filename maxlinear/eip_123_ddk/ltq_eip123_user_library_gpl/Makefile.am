##
## File: Makefile.am
##

##############################################################################
## Copyright (c) 2009-2013 INSIDE Secure B.V. All Rights Reserved.
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 2 of the License, or
## any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program. If not, see <http://www.gnu.org/licenses/>.
##############################################################################

SUBDIRS=@SAFEZONE_SUBDIRS@

# Following line shall be replaced with rules from meta/*.mk
@SAFEZONE_RULES@

# Support run_check
run_check:
	RET=0; \
	for DIR in $(SUBDIRS);do \
		(cd $$DIR;make -n run_check >/dev/null 2>/dev/null); \
		if [ "X$$?" == "X0" ]; then \
			(cd $$DIR;make run_check);\
			NEWRET=$$?; \
			if [ "X$$RET" == "X0" ]; then \
				RET=$$NEWRET; \
			fi; \
		fi; \
	done

distclean-local:
	rm -f install-sh missing depcomp configure aclocal.m4 Makefile.in config.guess config.sub
	rm -rf autom4te.cache

# end of file Makefile.am
