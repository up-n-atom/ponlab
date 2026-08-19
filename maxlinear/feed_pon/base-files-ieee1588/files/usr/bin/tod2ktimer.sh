#!/bin/sh /etc/rc.common
# Copyright (C) 2016 OpenWrt.org
# Copyright (C) 2016 lantiq.com
# Copyright (c) 2019 - 2020 Intel Corporation

. $IPKG_INSTROOT/lib/pon.sh

TOD2PHC="phc2sys -l"
TOD2KTIMER="phc2sys"

get_prg() {
	echo `ps | grep "$1" | grep -v grep`
}

get_pid() {
	local tmp=`get_prg $1`
	echo `echo $tmp  | awk '{print $1}'`
}

get_params() {
	local cfg="tod2ktimer"
	config_load ieee_1588
	config_get xpectd_phc_name	${cfg} xpectd_phc_name
	config_get auto_cfg		${cfg} auto_cfg
	config_get synch_sys_clk	${cfg} synch_sys_clk
	config_get phc_clk		${cfg} phc_clk
	config_get domain_num		${cfg} domain_num
	config_get offset		${cfg} offset
	config_get log_level		${cfg} log_level
	config_get stdout		${cfg} stdout
	config_get syslog		${cfg} syslog
}

stop_ntpd() {
	local pid=`get_prg "ntpd" | awk '{print $1}'`
	if [ -n "$pid" ]; then
		echo "stopping ntpd..."
		/etc/init.d/sysntpd stop
	fi
}

compare() {
	if [ "$1" != "$2" ]; then
		echo "Abort: expected '$2' different from loaded '$1'"
		return 0
	fi
	return 1
}

is_1588_capable() {
	echo `find /sys/class/ptp -name "ptp1" | grep "ptp1"`
}

start() {
	local params

	# read configuration's parameters
	get_params

	# see if an instance of phc2sys is already running (excluding TOD2PHC)
	prg=`get_prg $TOD2KTIMER | grep -v "$TOD2PHC"`
	if [ -n "$prg" ]; then
		prg=`echo $prg | grep -o phc2sys.*`
		echo "$TOD2KTIMER already running: $prg"
		return 1
	fi

	params="$auto_cfg $synch_sys_clk -l $log_level $stdout $syslog"
	dev=`is_1588_capable`
	if [ -n "$dev" ] ; then
		# Verify if loaded ptp device matches the expected in
		# configuration by comparing clock names
		mc_name=`cat /sys/class/ptp/$phc_clk/clock_name`
		compare "$mc_name" "$xpectd_phc_name" && return 0

		echo "Support for 1588 available"
		params="$params -n $domain_num -p /dev/$phc_clk"
	else
		# see if an instance of ptp4l is already running
		prg=`get_prg "ptp4l"`
		if [ -n "$prg" ]; then
			prg=`echo $prg | grep -o ptp4l.*`
			echo "ptp4l already running: $prg"
			return 1
		fi
		echo "starting ptp4l -0"
		ptp4l -0 &
	fi

	stop_ntpd

	echo "starting $TOD2KTIMER $params"
	$TOD2KTIMER $params &
	return 1
}

stop() {
	# get_pid
	local pid=`get_pid "$TOD2KTIMER"`
	if [ -n "$pid" ]; then
		echo "stopping $TOD2KTIMER..."
		kill -s SIGTERM $pid
	else
		echo "$TOD2KTIMER is not running."
	fi

	dev=`is_1588_capable`
	if [ -z "$dev" ] ; then
		prg=`get_prg "ptp4l"`
		if [ -n "$prg" ]; then
			echo "stopping ptp4l..."
			killall -q ptp4l
		else
			echo "ptp4l is not running."
		fi
	fi
}

restart() {
	stop
	start
}
