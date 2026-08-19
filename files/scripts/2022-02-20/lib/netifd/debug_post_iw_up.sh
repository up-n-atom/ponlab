#!/bin/sh

script_name=$0

debug_infrastructure=/lib/netifd/debug_infrastructure.sh
if [ ! -f $debug_infrastructure ]; then
        exit
fi
. $debug_infrastructure

timestamp(){
	echo "[$(date '+%Y-%m-%d %T.%6N')]"
}

log_msg(){
	local tstamp=$(timestamp)
	echo -e "$tstamp [$$] $script_name: $*" # > /dev/console
}

# Wrappers to work with a local uci_data copy instead of real UCI DB
uci_show(){
	echo "$uci_output"
}

uci_get(){
	local ptrn=$1
	[ -z "$ptrn" ] && return
	local ret=$(uci_show | grep -w "$ptrn" |sed -e "s|^.*${ptrn}='\(.*\)'|\1|")
	# Restore the apostrophe "'" char from the escaped "uci show" output "'\''"
	ret=$(echo $ret |sed -e "s|['][\]['][']|'|g")
	echo $ret
}

# For certification: set IP address
# Check OS
os_name="RDKB"
default_ip="10.0.0.1"
certification_file="/etc/wave/certification_enabled"
which dmcli > /dev/null
if [ $? -eq 1 ]
then
	os_name="UGW"
	default_ip="192.168.1.1"
	certification_file="/opt/intel/wave/certification_enabled"
fi
if [ -e "$certification_file" ]
then
	if [ "$os_name" = "UGW" ]
	then
		br_ip=`uci get network.lan.ipaddr`
	else
		br_ip=`dmcli eRT getv Device.X_CISCO_COM_DeviceControl.LanManagementEntry.1.LanIPAddress`
		br_ip=${br_ip##*value: }
		br_ip=`echo $br_ip`
	fi
	if [ "$br_ip" = "$default_ip" ] || [ -z "$br_ip" ]
	then
		br_ip="192.165.100.10"
	fi
	change_ip=1
fi

init_debug_config(){
	uci_output=$(uci show wireless)
	uci_indexes=`uci_show | grep ifname | sed 's/.ifname/ /g' | awk '{print $1}'`
	vaps=`uci_show | grep ifname | cut -d"=" -f2 | grep -v "\." | tr '\r\n' ' ' | tr -d "'" | sed 's/w/-vw/g'`
	dbg_ptrn="debug_iw_post_up_"
	nof_dbg_cmds=$(uci_show |grep -c $dbg_ptrn) # number of debug command in total
}

do_debug_config(){
	for uci_index in $uci_indexes ; do
		interface=`uci_get ${uci_index}.ifname`
		#log_msg "uci_index ($uci_index) interface($interface)"
		for i in $(seq 1 $number_of_debug_configs); do
			debug_config=`uci_get ${uci_index}.${dbg_ptrn}${i}`
			if [ -n "$debug_config" ]; then
				log_msg "Execute: iw $interface iwlwav $debug_config"
				eval "iw $interface iwlwav $debug_config"
			fi
		done
	done
}

log_msg "Start"

init_debug_config

while :;
do
	log_msg "Wait for eventMsg"
	eventMsg=`dwpal_cli -ihostap $vaps -l"AP-ENABLED" -l"INTERFACE_RECONNECTED_OK"`
	log_msg "Received eventMsg"
	sleep 1
	if [ "0" != "$nof_dbg_cmds" ]; then
		do_debug_config
	fi

	# Change IP address (if needed) once only
	if [ "$change_ip" = "1" ]
	then
		if [ "$os_name" = "UGW" ]
		then
			ubus call uci set '{ "config" : "network" ,"section" : "lan", "values": {"ipaddr" : "'$br_ip'"}}'
			ubus call uci commit '{"config" : "network" }'
		else
			bridgeIPChange $br_ip
		fi
		change_ip=0
	fi

	if [ "0" = "$nof_dbg_cmds" ]; then
		break
	fi
done

log_msg "Finish"

