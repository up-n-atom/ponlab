#!/bin/sh /etc/rc.common
# shellcheck shell=dash

# Copyright (C) 2020 - 2022 MaxLinear, Inc.
# Copyright (C) 2019 - 2020 Intel Corporation
# Copyright (C) 2011 lantiq.com
# Copyright (C) 2011 OpenWrt.org

# In contrast to the original file, UCI write commands
# have been removed. Furthermore, the existience of synchronization
# file /tmp/pon_uci_cfg is checked before starting the service.

START=85
STOP=10
USE_PROCD=1
OMCID_BIN=/usr/bin/omcid

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

wait_uci_config_done() {
	# wait until serdes and optic config files are
	# updated by pondb.sh
	while [ ! -e "/tmp/pon_uci_cfg" ]; do
		sleep 1
	done
}

start_service() {
	local aon_mode

	if [ "$(pon_ploam_emergency_stop_state_get)" = "1" ]; then
		ploam_emerg_stop_state=-e
	fi

	aon_mode="$(uci -q get optic.common.aon_mode)"
	[ -z "$aon_mode" ] && aon_mode=0

	if [ "$aon_mode" -ne 1 ]; then
		wait_uci_config_done
		procd_open_instance
		procd_set_param env LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/opt/pon/lib/:/opt/intel/usr/lib/"
		procd_set_param command ${OMCID_BIN} ${ploam_emerg_stop_state}
		procd_set_param respawn
		procd_set_param stdout 2
		procd_set_param stderr 2
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

	config_load omci
	config_get mib_file "default" "mib_file" "/etc/mibs/prx300_1U.ini"
	config_get_bool warmstart "default" "warmstart" 0
	if [ "$warmstart" -eq 1 ]; then
		config_get cur_mib_file "default" "cur_mib_file" "/etc/mibs/current_mib.ini"
		if [ ! -f "$cur_mib_file" ]; then
			echo "Copy $mib_file to $cur_mib_file" > /dev/console
			cp "$mib_file" "$cur_mib_file"
		fi
	fi

	start "$@"
}

service_triggers()
{
	procd_add_reload_trigger "omci" "gpon" "optic"
}
