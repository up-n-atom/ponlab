#!/bin/sh /etc/rc.common
# Copyright (C) 2022 MaxLinear, Inc.

# Enables BBF mode 1 on URX for BBF testing purpose.
# Parameter "mode1" is mandatory to enable it.
# If other parameters are not set, they
# will be read from network config or use
# default values.
# gpon example configuration:
# config gpon 'bbf'
#	option mode1 '1'
#	option vani 'VANI0'
#	option vuni 'VUNI0_0'
#	option lan 'eth0_1'
#
# In addition the checksum verification inside GSWIP
# can be disabled via following option (required as MT2
# is sending packets without valid checksum)
#	option skip_checksum '1'

START=84

BBF_DEBUGFS_FILE="/sys/kernel/debug/ppa/hal/pp/bbf247_hgu_model"
BBF_CFG_FILE="gpon"
BBF_CFG_SECTION="bbf"

. $IPKG_INSTROOT/lib/pon.sh

board_name=$(pon_board_name)

bbf_vani_get() {
	local vani

	# check for VANI interface name in gpon config
	config_load "$BBF_CFG_FILE"
	config_get vani "$BBF_CFG_SECTION" "vani" ""
	if [ -z "$vani" ]; then
		# read ani interface name from network config
		config_load network
		config_get vani "iface_VANI0" "ifname" "VANI0"
	fi
	echo "$vani"
}

bbf_vuni_get() {
	local vuni

	# check for VUNI interface name in gpon config
	config_load "$BBF_CFG_FILE"
	config_get vuni "$BBF_CFG_SECTION" "vuni" ""
	if [ -z "$vuni" ]; then
		# read vuni interface name from network config
		config_load network
		config_get vuni "veip1" "name" "VUNI0_0"
	fi
	echo "$vuni"
}

bbf_lan_get() {
	local lan

	config_load "$BBF_CFG_FILE"
	config_get lan "$BBF_CFG_SECTION" "lan" "eth0_1"
	echo "$lan"
}

bbf_mode1_get() {
	local bbf_mode1

	config_load "$BBF_CFG_FILE"
	config_get_bool bbf_mode1 "$BBF_CFG_SECTION" "mode1" 0
	echo $bbf_mode1
}

bbf_checksum_option_get() {
	local skip_checksum

	config_load "$BBF_CFG_FILE"
	config_get_bool skip_checksum "$BBF_CFG_SECTION" "skip_checksum" 0
	echo $skip_checksum
}

# Disable/Enable checksum verification in GSWIP
bbf_checksum_verification() {
	if [ "$(bbf_checksum_option_get)" -eq 1 ]; then
		if [ -n "$(which switch_cli 2>/dev/null)" ]; then
			if [ "$1" = "no" ]; then
				switch_cli gsw_register_set nregaddr=0xf03 ndata=0x20
			else
				switch_cli gsw_register_set nregaddr=0xf03 ndata=0
			fi
		else
			printf "BBF checksum switching failed due to missing switch_cli\n"
		fi
	fi
}

# Enable/Disable BBF mode 1
bbf_mode1_enable() {
	local vani=$(bbf_vani_get)
	local vuni=$(bbf_vuni_get)
	local lan=$(bbf_lan_get)

	if [ "$(bbf_mode1_get)" -eq 1 ]; then
		if [ -f "$BBF_DEBUGFS_FILE" ]; then
			if [ "$1" = "yes" ]; then
				brctl addif br-lan "$vani"
				echo "$vani" "$vuni" "$lan" "enable" > "$BBF_DEBUGFS_FILE"
			else
				brctl delif br-lan "$vani"
				echo "disable" > "$BBF_DEBUGFS_FILE"
			fi
		else
			printf "BBF mode1 could not be enabled due to missing file %s\n" "$BBF_DEBUGFS_FILE"
		fi
	fi
}

start() {
	case $board_name in
	lgm*|urx851*)
		bbf_mode1_enable "yes"
		bbf_checksum_verification "no"
		;;
	esac
}

stop() {
	case $board_name in
	lgm*|urx851*)
		bbf_mode1_enable "no"
		bbf_checksum_verification "yes"
		;;
	esac
}
