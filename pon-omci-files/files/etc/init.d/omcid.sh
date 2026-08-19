#!/bin/sh /etc/rc.common
# shellcheck shell=dash

# Copyright (C) 2020 - 2023 MaxLinear, Inc.
# Copyright (C) 2019 - 2020 Intel Corporation
# Copyright (C) 2011 lantiq.com
# Copyright (C) 2011 OpenWrt.org

START=85
STOP=10
USE_PROCD=1
OMCID_BIN=/usr/bin/omcid

source $IPKG_INSTROOT/lib/pon.sh

# This function definitions are needed in case we are running this script on
# non-falcon board
pon_ploam_emergency_stop_state_get() {
	echo "0"
}

pon_lct_num_get() {
	# in case this is not overwritten, we don't have a LCT
	return
}

if [ -f $IPKG_INSTROOT/lib/pon.sh ]; then
	. $IPKG_INSTROOT/lib/pon.sh
fi

wait_for_overlay() {
	while ! grep overlayfs:/overlay /proc/self/mounts > /dev/null; do
		sleep 1
	done
}

is_flash_boot() {
	grep overlayfs /proc/self/mounts > /dev/null
}

start_service() {
	local aon_mode
	local stdout
	local stderr

	if [ "$(pon_ploam_emergency_stop_state_get)" = "1" ]; then
		ploam_emerg_stop_state=-e
	fi

	# by default the output of omcid is redirected to /dev/console (option 2)
	# read values from config to allow changes to this
	stdout="$(uci -q get omci.default.stdout)"
	[ -z "$stdout" ] && stdout=2
	stderr="$(uci -q get omci.default.stderr)"
	[ -z "$stderr" ] && stderr=2

	aon_mode="$(uci -q get optic.common.aon_mode)"
	[ -z "$aon_mode" ] && aon_mode=0

	if [ "$aon_mode" -ne 1 ]; then
		procd_open_instance
		procd_set_param env LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/opt/pon/lib/:/opt/intel/usr/lib/"
		procd_set_param command ${OMCID_BIN} ${ploam_emerg_stop_state}
		procd_set_param respawn
		procd_set_param stdout $stdout
		procd_set_param stderr $stderr
		procd_close_instance
	fi
}

service_stopped() {
	#echo "Wait for omcid to exit"
	while pgrep -x ${OMCID_BIN} > /dev/null; do sleep 1; done
	#echo "omcid is gone"
}

start() {
	(
		# Wait until overlay is mounted.
		# This can take some time on first boot.
		is_flash_boot && wait_for_overlay

		rc_procd start_service "$@"
	) &
}

# If mib_file or omci_iop_mask are defined in U-Boot, copy them into uci config.
# The LCT port is also detected from network config during boot.
# Finally we copy the mib_file to cur_mib_file in case of warmstart and it does
# not exist yet (TODO: This should be done by the omci daemon itself).
boot() {
	local tmp
	local lct_num
	local mib_file
	local cur_mib_file
	local warmstart

	tmp=$(fw_printenv -n mib_file 2>&-)
	if [ -f "$tmp" ]; then
		uci set omci.default.mib_file="$tmp"
	elif [ -f "/etc/mibs/$tmp" ]; then
		uci set omci.default.mib_file="/etc/mibs/$tmp"
	fi

	tmp=$(fw_printenv -n omci_iop_mask 2>&-)
	if [ -n "$tmp" ]; then
		uci set omci.default.iop_mask=$tmp
	fi

	lct_num=$(pon_lct_num_get)
	[ -n "${lct_num}" ] && uci set omci.default.lct_port="$((1 + lct_num))"

	uci commit omci

	# do not start daemon in case ponmbox node is not enabled
	[ "$(pon_mbox_node_status)" != "okay" ] && exit 1

	start "$@"
}

service_triggers()
{
	procd_add_reload_trigger "omci" "gpon" "optic"
}
