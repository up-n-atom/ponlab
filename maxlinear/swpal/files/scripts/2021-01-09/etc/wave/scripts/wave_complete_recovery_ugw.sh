#!/bin/sh

# wave_complete_recovery.sh is a soft link to this file in case of UGW platform

wifi down
sleep 1
rmmod mtlk.ko
rmmod mtlkroot.ko
rmmod mac80211.ko
rmmod cfg80211.ko
rmmod compat.ko
if grep -m1 -q -s "^/etc/modules.d/" "/usr/lib/opkg/info/kmod-iwlwav-driver-uci-debug.list" || \
   grep -m1 -q -s "^/etc/modules.d/" "/usr/lib/opkg/info/kmod-iwlwav-driver-uci.list"; then
    kmodloader
fi
sleep 1
#Kill dump_handler, whm_handler and rcvry_monitor to prevent if any socket related issue,
#the handlers will be respawn, as configured in /etc/rc.d/S10wireless
killall dump_handler
killall whm_handler
killall rcvry_monitor
sleep 1
wifi up
