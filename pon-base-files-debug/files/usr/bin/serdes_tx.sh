#!/bin/sh
# shellcheck shell=dash

#set -x

argnum=$#
script=$(basename "$0")
version="V1.1"

###############################################################################
# Helper functions
###############################################################################

showversion() {
	printf "%s: version %s\n" "${script}" "${version}"
	exit 0
}

help() {
	printf "%s: <cmd> <args>\n" "${script}"
	printf "<cmd> can be version, help, get, set\n"
	printf "<args> can be 'serdes' 'tx_eq_pre' 'tx_eq_main' 'tx_eq_post' 'vboost_en' 'vboost_lvl' 'iboost_lvl'\n"
	printf "for 'set' a '=val' must follow on each argument\n"
	printf "for argument 'serdes' a '=val' must follow\n"
	printf "\nExamples:\n"
	printf "# %s get\n" "${script}"
	printf "# %s tx_eq_pre=0 tx_eq_main=40 tx_eq_post=0 vboost_en=0 vboost_lvl=5 iboost_lvl=15\n" "${script}"
}

serdes_help() {
	printf "Help for serdes argument\n"
	printf "Example:\n"
	printf "%s get serdes=<value>\n" "${script}"
	printf "Posible values:\n"
	if [ ${machine} == "URX" ]; then
		printf "0 = LAN0, HSIO-L PCIe 10/XPCS 0/SATA 0, base address 0xD0A00000\n"
		printf "1 = LAN1, HSIO-R PCIe 30/XPCS 2/SATA 2, base address 0xD0C00000\n"
		printf "2 = WAN , base address 0xE7800000\n"
		printf "3 = PCIe 20, base address 0xC0A00000\n"
		printf "4 = PCIe 40, base address 0xC0C00000\n"
		printf "5 = LAN SERDES (to P34X), base address 0xE7600000\n"

	elif [ ${machine} == "PRX" ]; then
		printf "0 = LAN0, base address 0x1A300000\n"
		printf "1 = LAN1, base address 0x1A800000\n"
		printf "2 = WAN , base address 0x18C00000\n"
	fi
}

die() {
	printf "\nERROR: %s\n" "$*"
	exit 1
}

dbg() {
	[ -n "${DEBUG}" ] && printf "DEBUG: %s\n" "$*" >&2
}

add() {
	s=$(printf "%d \+ %d" "$1" "$2")
	val=$(eval expr "${s}")
	printf "0x%08X\n" "$val"
}

###############################################################################
# Define Register Offsets
###############################################################################

define_urx_serdes() {
	LVL_OVRD_IN=0x48
	LVL_ASIC_IN=0x70
	LANE0_DIG_ASIC_TX_OVRD_IN_1=0x4008
	LANE0_DIG_ASIC_TX_OVRD_IN_2=0x400C
	LANE0_DIG_ASIC_TX_ASIC_IN_1=0x4038
	LANE0_DIG_ASIC_TX_ASIC_IN_2=0x403C
}

define_prx_serdes() {
	LVL_OVRD_IN=0x30
	LVL_ASIC_IN=0x58
	LANE0_DIG_ASIC_TX_OVRD_IN_1=0x4008
	LANE0_DIG_ASIC_TX_OVRD_IN_2=0x400C
	LANE0_DIG_ASIC_TX_ASIC_IN_1=0x4038
	LANE0_DIG_ASIC_TX_ASIC_IN_2=0x403C
}

###############################################################################
# Machine detection
###############################################################################

machine="Unknown"
[ -e /proc/device-tree/model ] && machine=$(cat /proc/device-tree/model)

cmd=$1
shift

serdes_option="None"
[ "${argnum}" -gt 1 ] && serdes_option=$(printf "$1" | awk 'BEGIN{FS="="} {print $1}')

serdes_value=""
if [ ${serdes_option} == "serdes" ]; then
	[ "$(printf "$1" | awk 'BEGIN{FS="="} {print $1}')" != "" ] && \
	serdes_value=$(printf "$1" | awk 'BEGIN{FS="="} {print $2}')
	shift
else
	serdes_value=2
fi

case "${machine}" in
*lightning*)
	machine="URX"
	define_urx_serdes
	if [ "${serdes_value}" == "" ]; then
		serdes_help
		die "serdes value is missing"
	fi
	if [ ${serdes_value} -eq 0 ]; then
		base_offset=0xD0A00000
	elif [ ${serdes_value} -eq 1 ]; then
		base_offset=0xD0C00000
	elif [ ${serdes_value} -eq 2 ]; then
		base_offset=0xE7800000
	elif [ ${serdes_value} -eq 3 ]; then
		base_offset=0xC0A00000
	elif [ ${serdes_value} -eq 4 ]; then
		base_offset=0xC0C00000
	elif [ ${serdes_value} -eq 5 ]; then
		base_offset=0xE7600000
	else
		serdes_help
		die "serdes value is wrong"
	fi
	;;
*PRX*)
	machine="PRX"
	define_prx_serdes
	if [ "${serdes_value}" == "" ]; then
		serdes_help
		die "serdes value is missing"
	fi
	if [ ${serdes_value} -eq 0 ]; then
		base_offset=0x1A300000
	elif [ ${serdes_value} -eq 1 ]; then
		base_offset=0x1A800000
	elif [ ${serdes_value} -eq 2 ]; then
		base_offset=0x18C00000
	else
		serdes_help
		die "serdes value is wrong"
	fi
	;;
*)
	define_prx_serdes # for testing on host
	base_offset=0
	;;
esac
dbg "Detected machine type: ${machine}"

###############################################################################
# Register access functions (low-level, based on tool)
###############################################################################

if [ -n "$(which devmem 2>/dev/null)" ]; then
	dbg "Use <devmem> for HW access"
	read_32() {
		addr=$(add "${base_offset}" "$1")
		dbg "devmem read from ${addr}"
		devmem "${addr}" 32
	}
	write_32() {
		addr=$(add "${base_offset}" "$1")
		dbg "devmem write of $2 to ${addr}"
		devmem "${addr}" 32 "$2"
	}
elif [ -n "$(which io 2>/dev/null)" ]; then
	dbg "Use <io> for HW access"
	read_32() {
		addr=$(add "${base_offset}" "$1")
		dbg "io read from ${addr}"
		io -4 "${addr}" | awk 'BEGIN{FS=" "} {printf "0x%s", $2}'
	}
	write_32() {
		addr=$(add "${base_offset}" "$1")
		dbg "io write of $2 to ${addr}"
		io -4 "${addr}" "$2"
	}
else
	dbg "No tool for HW access found, only dummy values"
	read_32() {
		addr=$(add "${base_offset}" "$1")
		dbg "dummy read from ${addr}"
		echo "${DEFAULT_VAL:=0x1234cafe}" | awk '{printf "%s", $1}'
	}
	write_32() {
		addr=$(add "${base_offset}" "$1")
		hexval=$(printf "%08X" $2)
		dbg "dummy write of ${hexval} to ${addr}"
	}
fi

###############################################################################
# Bit operations
###############################################################################

# $1: val
# $2: lowest bit num (shift right)
# $3: mask (after shifting)
get_bits() {
	printf "%d %d %d" "$1" "$2" "$3" | awk '{printf "%d\n", and(rshift($1,$2),$3)}'
}

# $1: value to modify
# $2: bit position (shift left)
# $3: mask (before shifting)
# $4: new_val (before shifting)
set_bits() {
	printf "%d %d %d %d" "$1" "$2" "$3" "$4" | awk '{mask=xor(0xFFFFFFFF,lshift($3,$2)); a=and($1,mask); printf "%d\n", or(a,lshift($4,$2))}'
}
###############################################################################
# Get values
###############################################################################
get_tx_eq_pre() {
	dbg "Dump tx_eq_pre"
	val_ovrd=$(read_32 "${LANE0_DIG_ASIC_TX_OVRD_IN_2}")
	val_asic=$(read_32 "${LANE0_DIG_ASIC_TX_ASIC_IN_2}")

	ovrd=$(get_bits "${val_ovrd}" 6 1)
	dbg "Use OVRD ${ovrd}"
	if [ "${ovrd}" -eq 1 ]; then
		val=$(get_bits "${val_ovrd}" 0 0x3f)
	else
		val=$(get_bits "${val_asic}" 0 0x3f)
	fi
	printf "%s" "${val}"
}
get_tx_eq_main() {
	dbg "Dump tx_eq_main"
	val_ovrd=$(read_32 "${LANE0_DIG_ASIC_TX_OVRD_IN_1}")
	val_asic=$(read_32 "${LANE0_DIG_ASIC_TX_ASIC_IN_1}")

	ovrd=$(get_bits "${val_ovrd}" 15 1)
	dbg "Use OVRD ${ovrd}"
	if [ "${ovrd}" -eq 1 ]; then
		val=$(get_bits "${val_ovrd}" 9 0x3f)
	else
		val=$(get_bits "${val_asic}" 6 0x3f)
	fi
	printf "%s" "${val}"
}
get_tx_eq_post() {
	dbg "Dump tx_eq_post"
	val_ovrd=$(read_32 "${LANE0_DIG_ASIC_TX_OVRD_IN_2}")
	val_asic=$(read_32 "${LANE0_DIG_ASIC_TX_ASIC_IN_2}")

	ovrd=$(get_bits "${val_ovrd}" 13 1)
	dbg "Use OVRD ${ovrd}"
	if [ "${ovrd}" -eq 1 ]; then
		val=$(get_bits "${val_ovrd}" 7 0x3f)
	else
		val=$(get_bits "${val_asic}" 6 0x3f)
	fi
	printf "%s" "${val}"
}
get_vboost_en() {
	dbg "Dump vboost_en"
	val_ovrd=$(read_32 "${LANE0_DIG_ASIC_TX_OVRD_IN_1}")
	val_asic=$(read_32 "${LANE0_DIG_ASIC_TX_ASIC_IN_1}")

	ovrd=$(get_bits "${val_ovrd}" 8 1)
	dbg "Use OVRD ${ovrd}"
	if [ "${ovrd}" -eq 1 ]; then
		val=$(get_bits "${val_ovrd}" 7 0x1)
	else
		val=$(get_bits "${val_asic}" 5 0x1)
	fi
	printf "%s" "${val}"
}
get_vboost_lvl() {
	dbg "Dump vboost_lvl"
	val_ovrd=$(read_32 "${LVL_OVRD_IN}")
	val_asic=$(read_32 "${LVL_ASIC_IN}")

	ovrd=$(get_bits "${val_ovrd}" 9 1)
	dbg "Use OVRD ${ovrd}"
	if [ "${ovrd}" -eq 1 ]; then
		val=$(get_bits "${val_ovrd}" 6 0x7)
	else
		val=$(get_bits "${val_asic}" 5 0x7)
	fi
	printf "%s" "${val}"
}
get_iboost_lvl() {
	dbg "Dump iboost_lvl"
	val_ovrd=$(read_32 "${LANE0_DIG_ASIC_TX_OVRD_IN_1}")
	val_asic=$(read_32 "${LANE0_DIG_ASIC_TX_ASIC_IN_1}")

	ovrd=$(get_bits "${val_ovrd}" 8 1)
	dbg "Use OVRD ${ovrd}"
	if [ "${ovrd}" -eq 1 ]; then
		val=$(get_bits "${val_ovrd}" 3 0xf)
	else
		val=$(get_bits "${val_asic}" 1 0xf)
	fi
	printf "%s" "${val}"
}

###############################################################################
# Set values (argument $1 is the value)
###############################################################################
set_tx_eq_pre() {
	dbg "Set tx_eq_pre to value ${1}"
	val_ovrd=$(read_32 "${LANE0_DIG_ASIC_TX_OVRD_IN_2}")

	# set OVRD bit
	val_ovrd=$(set_bits "${val_ovrd}" 6 1 1)
	val_ovrd=$(set_bits "${val_ovrd}" 0 0x3f "${1}")

	write_32 "${LANE0_DIG_ASIC_TX_OVRD_IN_2}" "${val_ovrd}"
}
set_tx_eq_main() {
	dbg "Set tx_eq_main to value ${1}"
	val_ovrd=$(read_32 "${LANE0_DIG_ASIC_TX_OVRD_IN_1}")

	# set OVRD bit
	val_ovrd=$(set_bits "${val_ovrd}" 15 1 1)
	val_ovrd=$(set_bits "${val_ovrd}" 9 0x3f "${1}")

	write_32 "${LANE0_DIG_ASIC_TX_OVRD_IN_1}" "${val_ovrd}"
}
set_tx_eq_post() {
	dbg "Set tx_eq_post to value ${1}"
	val_ovrd=$(read_32 "${LANE0_DIG_ASIC_TX_OVRD_IN_2}")

	# set OVRD bit
	val_ovrd=$(set_bits "${val_ovrd}" 13 1 1)
	val_ovrd=$(set_bits "${val_ovrd}" 7 0x3f "${1}")

	write_32 "${LANE0_DIG_ASIC_TX_OVRD_IN_2}" "${val_ovrd}"
}
set_vboost_en() {
	dbg "Set vboost_en to value ${1}"
	val_ovrd=$(read_32 "${LANE0_DIG_ASIC_TX_OVRD_IN_1}")

	# set OVRD bit
	val_ovrd=$(set_bits "${val_ovrd}" 8 1 1)
	val_ovrd=$(set_bits "${val_ovrd}" 7 1 "${1}")

	write_32 "${LANE0_DIG_ASIC_TX_OVRD_IN_1}" "${val_ovrd}"
}
set_vboost_lvl() {
	dbg "Set vboost_lvl to value ${1}"
	val_ovrd=$(read_32 "${LVL_OVRD_IN}")

	# set OVRD bit
	val_ovrd=$(set_bits "${val_ovrd}" 9 1 1)
	val_ovrd=$(set_bits "${val_ovrd}" 6 0x7 "${1}")

	write_32 "${LVL_OVRD_IN}" "${val_ovrd}"
}
set_iboost_lvl() {
	dbg "Set iboost_lvl to value ${1}"
	val_ovrd=$(read_32 "${LANE0_DIG_ASIC_TX_OVRD_IN_1}")

	# set OVRD bit
	val_ovrd=$(set_bits "${val_ovrd}" 8 1 1)
	val_ovrd=$(set_bits "${val_ovrd}" 3 0xf "${1}")

	write_32 "${LANE0_DIG_ASIC_TX_OVRD_IN_1}" "${val_ovrd}"
}
###############################################################################
# Toplevel functions
###############################################################################

serdes_tx_get() {
	list=${*:-tx_eq_pre tx_eq_main tx_eq_post vboost_en vboost_lvl iboost_lvl}
	for option in ${list}; do
		if eval "type get_${option}" 2> /dev/null > /dev/null; then
			printf "%s=%s\n" ${option} $(get_"${option}")
		else
			printf "Unknown option <%s>\n" "${option}"
		fi
	done
}

# ranges for set function
max_tx_eq_pre=63
max_tx_eq_main=63
max_tx_eq_post=63
max_vboost_en=1
max_vboost_lvl=7
max_iboost_lvl=15

serdes_tx_set() {
	changed=""
	for setting in $*; do
		option=$(printf "${setting}" | awk 'BEGIN{FS="="} {print $1}')
		value=$(printf "${setting}" | awk 'BEGIN{FS="="} {print $2}')
		[ -n "${value}" ] || die "Value for ${option} is missing"

		if eval "type set_${option}" 2> /dev/null > /dev/null; then
			eval $(printf "maxval=\$max_$option")
			[ "${value}" -le "${maxval}" ] || die "Value ${value} is to large for ${option}"
			dbg "Change ${option} to ${value}"
			set_"${option}" "$value"
		else
			die "Cannot set unknown option <${option}>"
		fi
		changed="${changed} ${option}"
	done

	[ -n "${changed}" ] && {
		printf "New settings:\n"
		# remove the comment before $changed to dump only the changed value
		serdes_tx_get # "${changed}"
	}
}

###############################################################################
# "main" function
###############################################################################

[ "${argnum}" -lt 1 ] && {
	help
	die "Not enough arguments"
}

case "${cmd}" in
"version" | "--version" | "-v")
	showversion
	;;
"help" | "-help" | "-h")
	help
	;;
"get")
	serdes_tx_get "$@"
	;;
"set")
	serdes_tx_set "$@"
	;;
*)
	die "Unknown command <${cmd}>"
	;;
esac
