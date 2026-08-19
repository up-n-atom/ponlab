#!/bin/sh
#
# load.sh
#
# This script loads the UMDevXS Driver and creates the communication
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

dmesg -c > /dev/null 2>&1
if [ $? -ne 0 ];
then
    echo "Not enough rights. Try sudo."
    exit
fi

# check if already loaded (0) or not yet loaded (1)
is_loaded=$(lsmod | grep umdevxs > /dev/null; echo $?)
if [ $is_loaded -eq 0 ];
then
    echo "Already loaded; use unload.sh first";
    exit
fi

# move to the script base directory
MY_DIR=$(dirname $0);
cd $MY_DIR

if [ "x$1" = "x" ];
then
    echo "Missing argument: build configuration (like pci, mem, sim)"
    exit
fi

# check that the driver was built successfully
build=umdevxs_k-$1.ko

if [ ! -f "$build" ]; then
    echo "Cannot find driver ($build)";
    exit
fi

# load the driver
insmod $build
RES=$?
if [ $RES -ne 0 ];
then
    echo "insmod failed ($RES)";
    exit
fi

# grab the major number for the character device
major_nr=`awk "\\$2==\"umdevxs_c\" {print \\$1}" /proc/devices`
#echo "Major nr: $major_nr";

# make the communication pointer
mknod /dev/umdevxs_c c ${major_nr} 0

# make accessible to application
chmod 666 /dev/umdevxs_c

# print and clear start-up messages
dmesg -c

# end of file load.sh
