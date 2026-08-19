#!/bin/sh
#
# unload.sh
#
# This script unloads the UMDevXS Driver and deletes the communication
# point for the application (using the proxy).
#

##############################################################################
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
##############################################################################

# print and clear messages
dmesg -c 2>/dev/null

# check that we run with root rights
# (this assumes dmesg -c is root-only)
if [ $? -ne 0 ]; then
    echo "Not enough rights. Try sudo."
    exit;
fi

# remove the communication point
rm -f /dev/umdevxs_c

# unload the driver
rmmod umdevxs_k

# print and clear the shutdown-messages
dmesg -c

# end of script unload.sh
