#############################################################################
#
# Copyright (c) 2009-2013 INSIDE Secure B.V. All Rights Reserved.
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

#
# Makefile for UMDevXS 'PeekPoke' test application
#

BIN_DIR:=$(SAFEZONE_CM_PATH)/bin

PEEKPOKE_SOURCES = \
    ../../Integration/UMDevXS/PeekPoke/src/umdevxs_peekpoke.c \
    ../../Integration/UMDevXS/UserPart/src/umdevxsproxy.c

default: $(PEEKPOKE_SOURCES)
	$(CROSS_COMPILE)gcc \
    -o umdevxs_peekpoke \
    -I. \
    -I../../Integration/UMDevXS/UserPart/incl \
    -I../../Integration/UMDevXS/KernelPart/incl \
    $(PEEKPOKE_SOURCES) \
    -pthread

clean:
	rm -f umdevxs_peekpoke

install:
	@if [ "x$(SAFEZONE_CM_PATH)" == "x" ]; then \
        echo "FATAL: Please provide SAFEZONE_CM_PATH"; \
    else \
        echo "Installing to $(BIN_DIR)"; \
        mkdir -p $(BIN_DIR); \
        cp umdevxs_peekpoke $(BIN_DIR); \
        chmod +x $(BIN_DIR)/umdevxs_peekpoke; \
        ln -s -f $(BIN_DIR)/umdevxs_peekpoke $(BIN_DIR)/pp; \
        ls -l $(BIN_DIR); \
    fi

# end of file
