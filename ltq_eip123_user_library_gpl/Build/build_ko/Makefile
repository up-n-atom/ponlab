#############################################################################
#
# Copyright (c) 2011-2013 INSIDE Secure B.V. All Rights Reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
##############################################################################

default: subbuild

subbuild:
	./_subbuild.sh

clean:
	rm -f umdevxs_k-*.ko
	make -f pci/Makefile clean
	make -f mem/Makefile clean
	make -f sim/Makefile clean

# note: not using install in sub makefile
install:
	@if [ "x$(SAFEZONE_CM_PATH)" == "x" ]; then \
        echo "FATAL: Please provide SAFEZONE_CM_PATH"; \
    else \
        echo "Installing to $(SAFEZONE_CM_PATH)/sbin"; \
        mkdir -p            $(SAFEZONE_CM_PATH)/sbin; \
        cp umdevxs_k-*.ko   $(SAFEZONE_CM_PATH)/sbin; \
        cp load.sh          $(SAFEZONE_CM_PATH)/sbin; \
        cp unload.sh        $(SAFEZONE_CM_PATH)/sbin; \
        chmod +x            $(SAFEZONE_CM_PATH)/sbin/load.sh; \
        chmod +x            $(SAFEZONE_CM_PATH)/sbin/unload.sh; \
    fi

# end of file
