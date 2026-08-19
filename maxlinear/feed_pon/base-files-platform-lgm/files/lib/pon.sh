#!/bin/sh
# Copyright (C) 2011 OpenWrt.org
# Copyright (C) 2011 lantiq.com
# Copyright (C) 2018 - 2020 Intel Corporation
# Copyright (c) 2022 Maxlinear Inc.

pon_dt_name() {
	local machine
	local name

	# check directly the device tree
	[ -e /proc/device-tree/model ] && machine=$(cat /proc/device-tree/model)

	# use second word in lower case
	name=$(echo $machine | awk 'BEGIN{FS=","} {print tolower($2);}')

	case "$name" in
		lgm*)
			name="urx851-eva"
			;;
		lgp*|osp-tb341)
			name="urx851-ref"
			;;
		octopus*|osp-tb341-v2)
			name="urx851-octopus"
			;;
		*)
			echo "pon_dt_name: Unknown machine name: $name" > /dev/console
			;;
	esac

	echo "$name"
}

pon_board_name() {
	local name

	name=$(pon_dt_name)

	echo "$name"
}

# return ponmbox node status
pon_mbox_node_status() {
	local node
	local node_paths="/proc/device-tree/ /proc/device-tree/soc/"
	local status=""

	for path in $node_paths; do
		node=$(ls -d "$path"ponmbox*) 2> /dev/null
		[ -n "$node" ] && [ -e "$node/status" ] && {
			status=$(cat "$node/status")
			break
		}
	done

	echo "$status"
}

# return the (board specific) number of lan ports
pon_get_number_of_lan_ports() {
	local num="1"

	echo $num
}

# return the (board specific) default interface used for "lct"
pon_default_lct_get() {
	case $(pon_board_name) in
	*)
		echo "eth0_0"
		;;
	esac
}

pon_lct_num_get() {
	local ifname=$(uci -q get network.lct.ifname)
	local uni_num=$(uci -q get network.lct.uni_num)
	if [ -n "$uni_num" ]; then
		echo "`expr $uni_num \\- 1`"
		return
	fi
	case ${ifname} in
	eth0_*)
		ifname=${ifname#eth0_}
		ifname=${ifname%_1_lct}
		echo "${ifname}"
		return
		;;
	esac
}

pon_sgmii_mode() {
	local retval="6" # 6 = 10G
	# TODO: Extend to define other speed modes
	echo $retval
}

pon_base_mac_get() {
	local mac_addr=$(awk 'BEGIN{RS=" ";FS="="} $1 == "ethaddr" {print $2}' /proc/cmdline)
	[ -z "$mac_addr" ] && mac_addr='ac:9a:96:00:00:00'
	echo $mac_addr
}

_hex2mac() {
	# adding ":" after each 2 chars and remove the last one again
	echo $1 | sed -e 's/../&:/g' -e 's/:$//'
}

_mac_add_offset() {
	local mac=$1
	local offs=$(printf "%d\n" $2) # make sure this is decimal

	mac=$(echo "$mac" | tr -d ':') # to remove colons
	mac=$(printf "%d\n" 0x$mac)    # to convert to decimal
	mac=$(expr $mac + $offs)       # add offset
	mac=$(printf "%012x\n" $mac)   # to convert to hex again

	echo $(_hex2mac $mac)
}

pon_mac_get() {
	local mac_offset
	local mac_limit=5

	case "$1" in
	eth0_0_1_lct)
		mac_offset=0
		;;
	host | wan)
		mac_offset=1
		;;
	eth0_0)
		mac_offset=2
		;;
	iphost)
		mac_offset=$((3+$2))
		;;
	*)
		mac_offset=-1
		;;
	esac

	if [ $mac_offset -ge 0 ]; then
		echo $(_mac_add_offset $(pon_base_mac_get) $mac_offset)
	fi

	if [ "$mac_offset" -ge "$mac_limit" ]; then
		echo "pon.sh[warning]: used mac_offset($mac_offset) over the limit($mac_limit)" >&2
	fi
}

pon_oui_get() {
	echo $(pon_base_mac_get) | awk 'BEGIN{FS=":"} {printf "%s:%s:%s\n", $1,$2,$3}'
}

pon_ploam_emergency_stop_state_get() {
	local retval="0"

	echo $retval
}

pon_is_10g_platform() {
	# true
	return 0
}

pon_transceiver_eeprom_path() {
	local dmi_alias="/sys/firmware/devicetree/base/aliases/wan_eeprom_51"
	local serial_alias="/sys/firmware/devicetree/base/aliases/wan_eeprom_50"
	local path
	local adr
	local bus

	# check for new path via pon_mbox
	case "$1" in
	dmi)
		path="/sys/class/pon_mbox/pon_mbox0/device/eeprom51"
		;;
	serial_id)
		path="/sys/class/pon_mbox/pon_mbox0/device/eeprom50"
		;;
	esac
	[ -e "$path" ] && {
		echo "$path"
		return
	}

	# backward compatible path
	case "$1" in
	dmi)
		adr=$(awk 'BEGIN{RS="/";FS="@"} {if (NR == 2) {print $2}}' "$dmi_alias")
		if [ -n $"adr" ]; then
			bus=$(find /sys/devices/soc0/$adr".i2c" -maxdepth 1 -name "i2c*" |
				sed -e 's/.*i2c-\([0-9]\)/\1/')
			[ -n $"bus" ] && path="/sys/bus/i2c/devices/"$bus"-0051/eeprom"
		else
			path="/sys/bus/i2c/devices/1-0051/eeprom"
		fi
		;;
	serial_id)
		adr=$(awk 'BEGIN{RS="/";FS="@"} {if (NR == 2) {print $2}}' "$serial_alias")
		if [ -n $"adr" ]; then
			bus=$(find /sys/devices/soc0/$adr".i2c" -maxdepth 1 -name "i2c*" |
				sed -e 's/.*i2c-\([0-9]\)/\1/')
			[ -n $"bus" ] && path="/sys/bus/i2c/devices/"$bus"-0050/eeprom"
		else
			path="/sys/bus/i2c/devices/1-0050/eeprom"
		fi
		;;
	esac
	[ -e "$path" ] && {
		echo "$path"
		return
	}
}
