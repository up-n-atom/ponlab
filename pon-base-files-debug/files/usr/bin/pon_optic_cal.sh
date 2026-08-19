#!/bin/sh
# shellcheck shell=dash

# Copyright (c) 2022 Maxlinear Inc.

cal_mode=0
warned=0
regval=0

info() {
	echo $@
}

manual() {
	cat <<- EOF
		Calibration Utility Script
		-----
		This utility is a comfort script for upstream transmitter timing calibraion.
		Please read the "PRX300_PON_Optical_Timing_Calibration" Application Note before proceeding.

	EOF
	read -p "Press any key to continue" -rn1
}

help() {
	cat <<- EOF

		Calibration Utility Keys:
		-----
		x - Enable 10G US Calibration Burst Mode
		g - Enable 1.25G US Calibration Burst Mode

		1 - Extend LASER ON time
		2 - Reduce LASER ON time
		3 - Reduce LASER OFF time
		4 - Extend LASER OFF time
		7 - Extend SerDes ON time (NOT RECOMMENDED)
		8 - Reduce SerDes ON time (NOT RECOMMENDED)
		9 - Reduce SerDes OFF time (NOT RECOMMENDED)
		0 - Extend SerDes OFF time (NOT RECOMMENDED)
		e - Enable Inter-Burst Pattern (NOT RECOMMENDED)
		d - Disable Inter-Burst Pattern (NOT RECOMMENDED)

		p - Print current values
		s - Save current values (using uci)
		q - Exit

	EOF
}

warning() {
	if [ $warned -eq 0 ]; then
		cat <<- EOF
			The tuning of SerDes ON/OFF times should be done in very rare special cases only.
			High risk of misconfiguration!

		EOF
		read -p "Do you still want to proceed? [y/n]" agree
		info
		if [ "$agree" = "y" ]; then
			warned=1
		fi
	fi
}

print_values() {
	local serdes_h_t=$serdes_hold_time
	if [ $serdes_en_mode -eq 0 ]; then
		serdes_h_t=0x3FF
	fi
	cat <<- EOF
		Current parameter values:
		----
		optic.common.burst_en_mode=$((burst_en_mode))
		optic.common.laser_setup_time=$((laser_setup_time * 3215))
		optic.common.serdes_setup_time=$((serdes_setup_time * 3215))
		optic.common.serdes_hold_time=$((serdes_h_t * 3215))
		optic.common.bias_setup_time=$((bedel_on * 3215))
		optic.common.bias_hold_time=$((bedel_off * 3215))
		optic.common.burst_idle_pattern=$(printf 0x%08x $ibp)
	EOF
}

save_values() {
	local serdes_h_t=$serdes_hold_time
	if [ $serdes_en_mode -eq 0 ]; then
		serdes_h_t=0x3FF
	fi
	uci batch <<- EOF
		set optic.common.burst_en_mode=$((burst_en_mode))
		set optic.common.laser_setup_time=$((laser_setup_time * 3215))
		set optic.common.serdes_setup_time=$((serdes_setup_time * 3215))
		set optic.common.serdes_hold_time=$((serdes_h_t * 3215))
		set optic.common.bias_setup_time=$((bedel_on * 3215))
		set optic.common.bias_hold_time=$((bedel_off * 3215))
		set optic.common.burst_idle_pattern=$(printf 0x%08x $ibp)
		commit optic
	EOF
	info "Saved."
}

debug() {
	#echo $@
	:
}

regval_read() {
	local addr=$1
	local out=$(pon rg $addr)
	debug "$out"
	regval=$(echo "$out" | cut -d " " -f 3 | cut -d "=" -f 2)
}

ctrl_env1() {
	# Enable/Disable ENVCTRL.EN1 (SerDes ON/OFF)
	regval_read 0x20012
	if [ "$1" -eq 1 ]; then
		regval=$((regval | 0x2))
	else
		regval=$((regval & 0xFFFFFFFD))
		serdes_hold_time=0x3FF
	fi
	pon rs 0x20012 $regval > /dev/null
	serdes_en_mode=$1
}

get_env1() {
	regval_read 0x20012
	if [ $((regval & 0x02)) -gt 0 ]; then
		serdes_en_mode=1
	else
		serdes_en_mode=0
	fi
}

ctrl_ibp() {
	local pattern
	if [ $1 -eq 1 ]; then
		if [ $(($ibp)) -eq 0 ]; then
			pattern=0x00FFFF00 #def.value
		else
			pattern=$ibp #prev.value
		fi
	else
		pattern=0
	fi
	for addr in $(seq 0x3F90 0x3F97); do
		pon rs $addr $pattern > /dev/null
	done
}

read_ibp() {
	regval_read 0x3F90
	ibp=$regval
}

ctrl_env3() {
	# Enable/Disable ENVCTRL.EN3 (BIAS ON/OFF)
	regval_read 0x20012
	if [ "$1" -eq 1 ]; then
		regval=$((regval | 0x8))
	else
		regval=$((regval & 0xFFFFFFF7))
	fi
	pon rs 0x20012 $regval > /dev/null

	# Enable/Disable OTBEE
	regval_read 0x3F81
	if [ "$1" -eq 1 ]; then
		regval=$((regval | 0x10000000))
	else
		regval=$((regval & 0xEFFFFFFF))
	fi
	pon rs 0x3F81 $regval > /dev/null
}

get_env3() {
	regval_read 0x20012
	local val1=$regval
	regval_read 0x3F81
	local val2=$regval
	if [ $((val1 & 0x08)) -gt 0 ] && [ $((val2 & 0x10000000)) -gt 0 ]; then
		burst_en_mode=1
	else
		burst_en_mode=0
	fi
}

read_laser_setup_time() {
	regval_read 0x2241F
	laser_setup_time=$((regval & 0xFFF))
}

write_laser_setup_time() {
	pon rs 0x2241F $laser_setup_time > /dev/null
}

read_serdes_setup_time() {
	regval_read 0x2241D
	serdes_setup_time=$((regval & 0xFFF))
}

write_serdes_setup_time() {
	pon rs 0x2241D $serdes_setup_time > /dev/null
}

read_serdes_hold_time() {
	regval_read 0x20015
	serdes_hold_time=$((regval & 0xFFF))
}

write_serdes_hold_time() {
	pon rs 0x20015 $serdes_hold_time > /dev/null
}

read_bedel() {
	regval_read 0x20011
	bedel_on=$((regval & 0xFFF))
	bedel_off=$(((regval >> 16) & 0xFFF))
}

write_bedel() {
	regval=$(((bedel_off << 16) + bedel_on))
	pon rs 0x20011 $regval > /dev/null
}

clean_stdin() {
	# Flush input thus allowing onoly 1 key press event per second.
	while read -s -t 1 discard; do :; done
}

manual
help
while true; do

	read_bedel
	read_laser_setup_time
	read_serdes_setup_time
	read_serdes_hold_time
	read_ibp
	get_env1
	get_env3

	if [ $burst_en_mode -eq 1 ]; then
		bedel_on=0
	fi

	clean_stdin

	info
	read -p "Enter command: " -rn1 input
	info

	case $input in
	"h")
		help
		;;

	"p")
		print_values
		continue
		;;

	"s")
		print_values
		read -p "Save the current settings? [y/n]" conf
		if [ "$conf" = "y" ]; then
			save_values
		fi
		continue
		;;

	"x")
		if [ $cal_mode -eq 1 ]; then
			info "Calibration mode already enabled"
			continue
		fi
		info "Enable 10G US Calibration Burst Mode"
		pon debug_ploam_cfg_set 1 23 0 > /dev/null
		ponmbox -c 0xe0 -w 0x00040480 > /dev/null
		cal_mode=1
		;;
	"g")
		if [ $cal_mode -eq 1 ]; then
			info "Calibration mode already enabled"
			continue
		fi
		info "Enable 1.25G Calibration Burst Mode"
		pon debug_ploam_cfg_set 1 30 0 > /dev/null
		ponmbox -c 0xe0 -w 0x00040480 > /dev/null
		cal_mode=1
		;;

	"q")
		exit
		;;

	*)
		if [ $cal_mode -ne 1 ]; then
			info "Enable calibration mode first! ('x' or 'g')"
			continue
		fi
		case $input in
		"1")
			info "Extend LASER ON"
			if [ $bedel_on -gt 0 ]; then
				bedel_on=$(((bedel_on - 1) & 0xFFF))
				write_bedel
				burst_en_mode=0
				ctrl_env3 $burst_en_mode
			else
				laser_setup_time=$((laser_setup_time + 1))
				write_laser_setup_time
				burst_en_mode=1
				ctrl_env3 $burst_en_mode
			fi
			;;

		"2")
			info "Reduce LASER ON"
			if [ $laser_setup_time -gt 0 ]; then
				laser_setup_time=$((laser_setup_time - 1))
				write_laser_setup_time
				burst_en_mode=1
				ctrl_env3 $burst_en_mode
			else
				bedel_on=$(((bedel_on + 1) & 0xFFF))
				write_bedel
				burst_en_mode=0
				ctrl_env3 $burst_en_mode
			fi
			;;

		"3")
			info "Reduce LASER OFF"
			if [ $bedel_off -gt 0 ]; then
				bedel_off=$(((bedel_off - 1) & 0xFFF))
				write_bedel
			else
				info "LASER OFF reached the lowest limit"
			fi
			;;

		"4")
			info "Extend LASER OFF"
			bedel_off=$(((bedel_off + 1) & 0xFFF))
			write_bedel
			;;

		"7")
			warning
			info "Extend SerDes ON"
			serdes_setup_time=$(((serdes_setup_time + 1) & 0xFFF))
			write_serdes_setup_time
			;;

		"8")
			warning
			info "Reduce SerDes ON"
			if [ $serdes_setup_time -gt 0 ]; then
				serdes_setup_time=$(((serdes_setup_time - 1) & 0xFFF))
				write_serdes_setup_time
			else
				info "SerDes ON reached the lowest limit"
			fi
			;;

		"9")
			warning
			info "Reduce SerDes OFF"
			if [ $serdes_hold_time -gt 0 ]; then
				serdes_hold_time=$(((serdes_hold_time - 1) & 0xFFF))
				write_serdes_hold_time
			fi
			;;

		"0")
			warning
			info "Extend SerDes OFF"
			serdes_hold_time=$(((serdes_hold_time + 1) & 0xFFF))
			write_serdes_hold_time
			;;

		"e")
			warning
			info "Enable Inter-Burst Pattern"
			ctrl_env1 0
			ctrl_ibp 1
			;;

		"d")
			warning
			info "Disable Inter-Burst Pattern"
			ctrl_env1 1
			ctrl_ibp 0
			;;

		*)
			info "Unknown key. For valid keys press h."
			;;
		esac
		;;
	esac

done
