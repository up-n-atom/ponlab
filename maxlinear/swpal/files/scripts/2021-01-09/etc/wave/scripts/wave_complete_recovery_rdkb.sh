#!/bin/sh

#
# This script performs WiFi Complete Recovery.
# For usage info, run it with help | -h | --help .
#

function log_msg() {
	#echo "[CompleteRecovery] $1" # DEBUG
	print_logs "[CompleteRecovery] $1"
}

function usage() {
	echo -e "\nUsage: `basename $0` <params ...>\n"\
			"  Perform Complete Recovery of the WiFi subsystem,\n"\
			"  by stopping and restarting of all services and daemons.\n"\
			"Params:\n"\
			"  help | -h | --help : print usage info\n"\
			"  -dwpald : restart the dwpal daemon\n"\
			"Examples:\n"\
			"  `basename $0`\n"\
			"  `basename $0` -dwpald\n" > /dev/console
}

function stop_onewifi() {
	log_msg "stop_onewifi"
	systemctl stop onewifi
}

function start_onewifi() {
	log_msg "start_onewifi"
	systemctl start onewifi
}

function stop_wifi_agent() {
	log_msg "stop_wifi_agent"
	systemctl stop ccspwifiagent
}

function start_wifi_agent() {
	log_msg "start_wifi_agent"
	systemctl start ccspwifiagent
}

function stop_dwpald() {
	log_msg "stop_dwpald"
	systemctl stop systemd-dwpal_daemon.service
}

function start_dwpald() {
	log_msg "start_dwpald"
	systemctl start systemd-dwpal_daemon.service
}

function stop_netifd() {
	log_msg "stop_netifd"
	# stop hostapd
	systemctl stop systemd-netifd.service
}

function start_netifd() {
	log_msg "start_netifd"
	# start hostapd
	systemctl start systemd-netifd.service
}

function stop_wifi_driver() {
	log_msg "stop_wifi_driver"
	systemctl stop systemd-wave_init.service
}

function start_wifi_driver() {
	log_msg "start_wifi_driver"
	systemctl start systemd-wave_init.service
}

function main() {
	# DO NOT change the order of the following lines!
	# There is no symmetry between start and stop orders.
	# When Starting, the dwpald must be up before wifi driver and netifd.

	#stop_wifi_agent
	[ $restart_onewifi = 1 ] && stop_onewifi
	[ $restart_dwpald = 1 ] && stop_dwpald
	[ $restart_onewifi = 0 ] && stop_netifd
	stop_wifi_driver

	[ $restart_dwpald = 1 ] && start_dwpald
	start_wifi_driver
	[ $restart_onewifi = 0 ] && start_netifd
	[ $restart_onewifi = 1 ] && start_onewifi
	#start_wifi_agent
}

if [ -f /lib/wifi/platform_dependent.sh ]; then
	. /lib/wifi/platform_dependent.sh
	. "$SCRIPTS_PATH/common_utils.sh"
else
	. "$1"
	local tmp=$(echo $0 | sed 's/\(.*\)\/.*/\1/')
	# assuming it is in the same path as called script
	. "$tmp/common_utils.sh"
	shift
fi

log_msg "$0 started"

restart_dwpald=0
[ -f "/usr/bin/OneWifi" ] && restart_onewifi=1 || restart_onewifi=0

case $1 in
	help | -h | --help)
		usage
		;;
	-dwpald)
		restart_dwpald=1
		main "$@"
		;;
	*)
		main "$@"
		;;
esac

log_msg "$0 ended"
