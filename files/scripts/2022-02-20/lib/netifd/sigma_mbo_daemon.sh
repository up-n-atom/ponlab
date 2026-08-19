#!/bin/sh

. /lib/netifd/sigma-ap.sh

kill_dwpal_cli()
{
        event1="AP-STA-WNM-NOTIF"
        event2="RRM-BEACON-REP-RECEIVED"
		if [ "$OS_NAME" = "UPDK" ]; then
			all_event=`ps -ef | grep -e $event1 -e $event2 | awk '{print $2}'`
		else
			all_event=`ps -w | grep -e $event1 -e $event2 | awk '{print $1}'`
		fi
        for pid in $(echo "$all_event" | head -n -1)
        do
                kill -9 $pid
        done
}

if [ "$OS_NAME" = "UPDK" ]; then
	# UPDK
	is_running=`ps -ef | grep sigma_mbo_d | wc -l`
else
	# UGW
	is_running=`ps | grep sigma_mbo_d | wc -l`
fi
if [ $is_running -ge 4 ]; then
	debug_print "mbo daemon is already running"
	exit 0
fi

debug_print "--------------------------------------------------- MBO DAEMON STARTED ---------------------------------------------------"

while :;
do
	if [ "$OS_NAME" = "UPDK" ]; then
		# UPDK
		ap_tmp=`kill_dwpal_cli`
		ap_tmp1=`killall logread`
		ap_tmp2=`killall tail`
		event=`logread -f | grep -e "AP-STA-WNM-NOTIF" -e "RRM-BEACON-REP-RECEIVED" -e "AP-STA-CONNECTED" -e "AP-STA-SCS-ADD" -e "AP-STA-SCS-REM" > /tmp/event.log &`
		sleep 5
		event=`cat /tmp/event.log`
		tmp=`rm -rf /tmp/event.log`
		if [ "$event" != "" ]; then
			debug_print "sigma_mbo_handler event received = $event"
			mbo_handler $event
		fi
	else
		# UGW
		vaps=`uci show wireless | grep ifname | cut -d"=" -f2 | grep "\." | tr '\r\n' ' ' | tr -d "'" | sed 's/w/-vw/g'`
		ap_tmp=`kill_dwpal_cli`
		event=`run_dwpal_cli_cmd -ihostap -mMain $vaps -dd -l"AP-STA-CONNECTED" -l"RRM-BEACON-REP-RECEIVED" -l"AP-STA-WNM-NOTIF" -l"AP-STA-SCS-ADD" -l"AP-STA-SCS-REM"`
		debug_print "sigma_mbo_handler event received = $event"
		mbo_handler $event
	fi
done
