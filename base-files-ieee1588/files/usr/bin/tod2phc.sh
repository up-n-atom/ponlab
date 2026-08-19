#!/bin/sh /etc/rc.common
# Copyright (c) 2021 Maxlinear Inc.
# Copyright (c) 2019 - 2020 Intel Corporation
# Copyright (C) 2016 lantiq.com
# Copyright (C) 2016 OpenWrt.org

. $IPKG_INSTROOT/lib/pon.sh

# "-l" is defined here to distinguish from other instances of phc2sys
TOD2PHC="phc2sys -l"

get_prg() {
	echo `ps | grep "$TOD2PHC" | grep -v grep`
}

get_pid() {
	local tmp=`get_prg`
	echo `echo $tmp  | awk '{print $1}'`
}

get_params() {
	local cfg="tod2phc"
	config_load ieee_1588
	config_get xpectd_mc_name	${cfg} xpectd_mc_name
	config_get xpectd_sc_name	${cfg} xpectd_sc_name
	config_get master_clock		${cfg} master_clock
	config_get slave_clock		${cfg} slave_clock
	config_get offset		${cfg} offset
	config_get time_thr		${cfg} time_thr
	config_get step_thr		${cfg} step_thr
	config_get sync_offset		${cfg} sync_offset
	config_get log_level		${cfg} log_level
	config_get stdout		${cfg} stdout
	config_get syslog		${cfg} syslog
}

if pon_is_10g_platform
then
pps_pin_active() {
	# PPS check for PRX not available
	return 1
}

wait_sync() {
	# SYNCE check for PRX not available
	return 1
}
else
pps_pin_active() {
	# check if 1PPS pin output is active
	echo `onu gpe_tod_pin_cfg_get` | grep -q "pps_pin_mode=2" && return 1
	echo "Abort: Missing 1PPS signal"
	return 0
}

wait_sync() {
	local cnt=0
	local sts=""

	while [ $cnt -le 30 ]
	do
		sts=`onu lan_synce_status_get 2>&- | cut -f3 -d=`
		if [ $sts = "2" ]; then
			return 1
		fi

		sleep 1
		cnt=$((cnt+1))
	done

	echo "SyncE not synchronized after 30 seconds. Leaving..."
	return 0
}
fi

compare () {
	if [ "$1" != "$2" ]; then
		echo "Abort: expected '$2' different from loaded '$1'"
		return 0
	fi
	return 1
}

start() {
	# read configuration's parameters
	get_params

	# Verify if loaded ptp device matches the expected in configuration
	# Compares clock names
	mc_name=`cat /sys/class/ptp/$master_clock/clock_name`
	compare "$mc_name" "$xpectd_mc_name" && return 0
	sc_name=`cat /sys/class/ptp/$slave_clock/clock_name`
	compare "$sc_name" "$xpectd_sc_name" && return 0

	# see if already running
	prg=`get_prg`
	if [ -n "$prg" ]; then
		prg=`echo $prg | grep -o phc2sys.*`
		echo "tod2phc already running: $prg"
		return 1
	fi

	pps_pin_active && return 0
	wait_sync && return 0

	if pon_is_10g_platform; then
		### FIXME: temporary calls for PRX platform
		# Start XGMAC 1 ptp clock
		hwstamp_ctl -i eth0_0 -r 1 -t 1
		# Select Aux Trigger for EXTTS0 timestamps to PON PTP 1PPS
		switch_cli gswss aux_trig 0 0 0
	fi

	# "-l" for log_level is part of $TOD2PHC
	params="$log_level -s /dev/$master_clock -e 0 -c /dev/$slave_clock -e 0 -O $offset -T $time_thr -o $sync_offset -S $step_thr $stdout $syslog"
	echo "starting $TOD2PHC $params"
	$TOD2PHC $params &
	return 1
}

stop() {
	# get_pid
	local pid=`get_pid`
	if [ -n "$pid" ]; then
		echo "stopping tod2phc..."
		kill -s SIGTERM $pid
	else
		echo "tod2phc is not running."
	fi
}

restart() {
	stop
	start
}
