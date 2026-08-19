#!/bin/sh /etc/rc.common
# Copyright (C) 2016 OpenWrt.org
# Copyright (C) 2016 lantiq.com
# Copyright (c) 2019 - 2020 Intel Corporation

. $IPKG_INSTROOT/lib/pon.sh

TOD2PHC="phc2sys -l"

get_prg() {
	echo `ps | grep "$1" | grep -v grep`
}

get_params() {
	local cfg="ptp_sync"
	config_load ieee_1588
	config_get timestamping		${cfg} timestamping
	config_get network_transport	${cfg} network_transport
	config_get delay_mechanism	${cfg} delay_mechanism
	config_get cfg_file		${cfg} cfg_file
	config_get interface		${cfg} interface
	config_get log_level		${cfg} log_level
	config_get stdout		${cfg} stdout
	config_get syslog		${cfg} syslog
}

wait_ptp_lock() {

	# first check if tod2phc started
	local prg=`get_prg "$TOD2PHC"`
	if [ -z "$prg" ]; then
		echo "tod2phc failed to start. Leaving..."
		return 0
	fi

	# wait until tod2phc synchronizes ptp clocks
	sleep 5
	return 1
}

start() {
	local prg
	local interfaces

	# check if already running
	prg=`get_prg "ptp4l"`
	if [ -n "$prg" ]; then
		prg=`echo $prg | grep -o ptp4l.*`
		echo "ptp4l already running: $prg"
		echo "Leaving..."
		return 0
	fi

	tod2phc.sh start && return 0
	# workaround to avoid timing issue which disturb initial ptp_sync start; should be solved later by procd approach with aligned priorities
	sleep 1
	wait_ptp_lock && return 0

	# read ptp4l configuration options
	get_params
	for ifc in $interface; do
		interfaces="$interfaces -i $ifc"
	done
	params="$delay_mechanism $network_transport $timestamping -f $cfg_file $interfaces -l $log_level $stdout $syslog"
	echo "starting ptp4l $params"
	ptp4l $params &
}

stop() {
	local prg

	tod2phc.sh stop

	prg=`get_prg "ptp4l"`
	if [ -n "$prg" ]; then
		# read ptp4l configuration options
		get_params
		echo "stopping ptp_sync..."
		killall -q ptp4l
		# temporarily removed with respect to PTP drv bug DRVLIB_SW-2334
		#for ifc in $interface
		#do
		#hwstamp_ctl -i $ifc -r 0 -t 0 > /dev/null
		#done
	else
		echo "ptp4l is not running."
	fi
}

restart() {
	stop
	start
}
