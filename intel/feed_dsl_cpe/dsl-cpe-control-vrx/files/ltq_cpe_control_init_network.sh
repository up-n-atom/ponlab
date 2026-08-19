#!/bin/sh

LINE_GET=""

eval $( cat /proc/driver/mei_cpe/devinfo )
xDSL_AutoCfg_Bonding=$(( $MaxDeviceNumber * $LinesPerDevice > 1 ))
if [ ${xDSL_AutoCfg_Bonding} = 1 ]; then
   LINE_GET="0"
fi

if [ -d /opt/intel ]
then
	bindir=/opt/intel/bin
else
	echo "ERROR! No vendor path found!" > /dev/console
fi

retries=0
while [ ! -e /tmp/pipe/dsl_cpe0_cmd ] && [ $retries -lt 15 ]; do
	sleep 1;
	retries=$((retries+1));
done

if [ $retries == 15 ]; then
	echo "API start - timeout. Continuing..." > /dev/console
fi

retries=0
while [ "`$bindir/dsl_cpe_pipe lsg $LINE_GET | grep -m1 -E '([2-8][0-9][0-9])' | cut -d'x' -f2 | head -n1`" != "801" ] && [ $retries -lt 45 ]; do
	sleep 1;
	retries=$((retries+1));
done

if [ $retries == 45 ]; then
	echo "SHOWTIME check - timeout. Continuing..." > /dev/console
fi


echo "VRX518 network interface configured." > /dev/console
