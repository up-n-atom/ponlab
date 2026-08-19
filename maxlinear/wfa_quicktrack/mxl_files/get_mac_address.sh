#!/bin/sh
# This is platform depended implementation to obtain mac address
# for wireless interface. This script need to be modified for new platform support 

. /lib/wifi/platform_dependent.sh
. "$SCRIPTS_PATH/common_utils.sh"

if [ -z "$1" ]; then
	exit
fi

echo -n `update_mac_address $1`