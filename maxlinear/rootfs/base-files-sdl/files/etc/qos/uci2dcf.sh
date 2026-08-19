#!/bin/sh

#DELTA INFO
DELTA_IFACE=""
DELTA_OLD_IFACE=""
DELTA_ENTITY=""
DELTA_ENTITY_ID=""
DELTA_OLD_ENTITY=""
DELTA_OPERATION=""
DELTA_INSTANCE=0
DELTA_PRESENT=0
OPERATION_LIST="ADD MOD DEL NOOPER"


#Indexes
IF_IDX=0
Q_IDX=0
CL_IDX=0
SH_IDX=0
D_IDX=0
QOS_IDX=0


QOS_UCI="/etc/config/qos"
QOS_CONFIG="/tmp/qosd/qos.conf"

. /lib/functions.sh
QOS_OPTIONS="enable loglevel logfile logredirect defwandscp wmmenable mgmtenable"

IFACE_OPTIONS="conname enabled overhead ulOverhead dlOverhead upload download conntype category logicalIf vlanif phydev direction shaperEn peakrate commitrate peakburst commitburst cpuinggrp shaperName"
CONNTYPE_LIST="STATIC DHCP PPPOE BRIDGE"
IFCAT_LIST="LAN ETH PTM ATM PON LTE WLAN WLANN LO"
IFNAME=""
IF_LOGICAL=""
IF_BASE=""
IFTYPE=""


CLASS_OPTIONS="qname status owner maxsize peakrate commitrate qprio tc weight drop policy qlenG qlenY qlenR peakburst commitburst inggrp accel_eng queue_eng flags"

CLASSIFY_OPTIONS="comment enable target order clinterface qInterface mptcp srcmac srcmacmask srcmacex dstmac dstmacmask dstmacex vlanid vlanidex vlanprio vlanprioex srchost srchostmask srchostex dsthost dsthostmask dsthostex proto protoex srcports srcportsex dstports dstportsex tcpACKen tcpACKenex pktsize tcpflags l7app tos dscp dscpex actiondscp actionvlanprio actionacceldis peakrate commitrate peakburst commiturst cl_eng ethtype ethtypeex ethprio ethprioex ethpriomark innerethpriomark lowerlayer trafficclass prio flags innervlanprio innervlanprioex innervlanid innervlanidex"


parse_class() {
	local section="$1"
	local entityid="$2"
	local classoper="ADD" #ADD by default
	
	[ "$section" != "$entityid" ] && return 0;
	
	echo "[queue]" >> $QOS_CONFIG
	Q_IDX=$(($Q_IDX+1))
	
	[ "$section" == "$DELTA_ENTITY_ID" ] && [ "$DELTA_ENTITY" == "class" ] && DELTA_INSTANCE=$Q_IDX && classoper="${DELTA_OPERATION}"

	for option in $CLASS_OPTIONS
	do
		local var
		config_get var "$section" "$option"
		[ ! -z $var ] && echo "q_${Q_IDX}_${option}=$var" >> $QOS_CONFIG
	done
	echo "q_${Q_IDX}_oper=$classoper" >> $QOS_CONFIG
	echo "q_${Q_IDX}_ifname=$IFNAME" >> $QOS_CONFIG
	echo "q_${Q_IDX}_logicalifname=$IF_LOGICAL" >> $QOS_CONFIG
	echo "q_${Q_IDX}_phyifname=$IF_BASE" >> $QOS_CONFIG
	echo "q_${Q_IDX}_iftype=$IFTYPE" >> $QOS_CONFIG
}

parse_classify() {
	local section="$1"
	local ifname=""
	local oper="ADD" #ADD by default

	echo "[classifier]" >> $QOS_CONFIG
	CL_IDX=$(($CL_IDX+1))

	[ "$section" == "$DELTA_ENTITY_ID" ] && [ "$DELTA_ENTITY" == "classify" ] && DELTA_INSTANCE=$CL_IDX && oper="${DELTA_OPERATION}"

	for option in $CLASSIFY_OPTIONS
	do
		local var
		config_get var "${section}" "${option}"
		if [ ! -z "$var" ]
		then
		case "${option}" in
			comment)
				echo "cl_${CL_IDX}_name=${var}" >> $QOS_CONFIG
				;;
			clinterface)
				ifname="${var}"
				echo "cl_${CL_IDX}_${option}=${var}" >> $QOS_CONFIG
				;;
			*)
				echo "cl_${CL_IDX}_${option}=${var}" >> $QOS_CONFIG
				;;
		esac
		fi
	done
	echo "cl_${CL_IDX}_oper=$oper" >> $QOS_CONFIG

	config_foreach parse_interface interface "${ifname}" "1"
	echo "cl_${CL_IDX}_ifname=$IFNAME" >> $QOS_CONFIG
	echo "cl_${CL_IDX}_logicalifname=$IF_LOGICAL" >> $QOS_CONFIG
	echo "cl_${CL_IDX}_phyifname=$IF_BASE" >> $QOS_CONFIG
	echo "cl_${CL_IDX}_iftype=$IFTYPE" >> $QOS_CONFIG
}

parse_deltacfg() {
	local section="$1"
	DELTA_PRESENT=1

	config_get DELTA_ENTITY "$section" "entity"
	config_get DELTA_ENTITY_ID "$section" "entityId"
	config_get DELTA_OPERATION "$section" "operation"
	DELTA_OPERATION=`echo $DELTA_OPERATION | awk '{print toupper($0)}'`
	
	config_get DELTA_OLD_ENTITY "$section" "oldentityId"
	config_get DELTA_IFACE "$section" "interface"
	config_get DELTA_OLD_IFACE "$section" "oldinterface"
}

parse_qoscfg() {
	local section="$1"
	local var
	
	echo "[qoscfg]" >> $QOS_CONFIG
	QOS_IDX=$(($QOS_IDX+1))

	[ "${DELTA_ENTITY}" == "qoscfg" ] && DELTA_INSTANCE=$QOS_IDX
	
	for option in $QOS_OPTIONS
	do 
		config_get var "${section}" "${option}"
		if [ ! -z $var ]
		then
				echo "qos_${QOS_IDX}_${option}=$var" >> $QOS_CONFIG
		fi
	done
}

parse_classgrp() {
	local section="$1"
	local grpname="$2"
	local classes
	[ "$section" != "$grpname" ] && return 0;
	
	config_get classes "$section" "classes"
	classes=`echo $classes | tr ',' ' '`
	for class in $classes
	do
		config_foreach parse_class class "${class}"
	done
}

parse_interface() {
	local section="$1"
	local ifname="$2"
	local var
	local numVar
	local shEnable
	local sh_oper="ADD" #ADD by default

	if ( [ -z $3 ] && [ "$3" == "" ]) && ([ -z $ifname ] || [ "${ifname}" == "" ] ); then
		IF_IDX=$((IF_IDX+1))

		config_get var "$section" "conname"
		IFNAME="${var}"

		config_get var "$section" "logicalIf"
		[ ! -z $var ] && IF_LOGICAL="$var" || IF_LOGICAL="${IFNAME}"

		config_get var "$section" "phydev"
		[ ! -z $var ] && IF_BASE="$var" || IF_BASE="${IFNAME}"

		config_get var "$section" "category"
		var=`echo "$var" | awk '{print toupper($0)}'`
		numVar=0
		for str in $IFCAT_LIST
		do
			[ "$str" == "$var" ] && break;
			numVar=$(($numVar+1))
		done
		IFTYPE=$numVar

		config_get var "$section" "classgroup"
		config_foreach parse_classgrp classgroup "${var}"

		# Check the shaper properties.
		config_get shEnable "${section}" "shaperEn"
		config_get var "${section}" "peakrate"
		config_get shName "${section}" "shaperName"
		if ([ ! -z $shEnable ] && [ ! -z $var ])
		then
			SH_IDX=$(($SH_IDX+1))
			[ "$shName" == "$DELTA_ENTITY_ID" ] && [ "$DELTA_ENTITY" == "interface" ] && DELTA_INSTANCE=$SH_IDX && sh_oper="${DELTA_OPERATION}"

			echo "[shaper]" >> $QOS_CONFIG
			echo "sh_${SH_IDX}_shaperName=$shName" >> $QOS_CONFIG
			echo "sh_${SH_IDX}_ifname=$IFNAME" >> $QOS_CONFIG
			echo "sh_${SH_IDX}_logicalifname=$IF_LOGICAL" >> $QOS_CONFIG
			echo "sh_${SH_IDX}_phyifname=$IF_BASE" >> $QOS_CONFIG
			echo "sh_${SH_IDX}_iftype=$IFTYPE" >> $QOS_CONFIG

			echo "sh_${SH_IDX}_shaperoper=${sh_oper}" >> $QOS_CONFIG
			echo "sh_${SH_IDX}_shaperen=${shEnable}" >> $QOS_CONFIG
			echo "sh_${SH_IDX}_shapermode=1" >> $QOS_CONFIG	#Setting blind color mode by default

			config_get var "${section}" "peakrate"
			echo "sh_${SH_IDX}_shapercir=${var}" >> $QOS_CONFIG
			echo "sh_${SH_IDX}_shaperpir=${var}" >> $QOS_CONFIG

			config_get var "${section}" "peakburst"
			echo "sh_${SH_IDX}_shapercbs=${var}" >> $QOS_CONFIG
			echo "sh_${SH_IDX}_shaperpibs=${var}" >> $QOS_CONFIG

			config_get var "${section}" "cpuinggrp"
			[ ! -z $var ] && echo "sh_${SH_IDX}_shaperinggrp=${var}" >> $QOS_CONFIG

			config_get var "${section}" "flags"
			[ ! -z $var ] && echo "sh_${SH_IDX}_shaperflags=${var}" >> $QOS_CONFIG
		fi
	else
		# Get interface details only
		config_get var "$section" "conname"

		[ "$3" == "2" ] && [ "$section" != "$ifname" ] && return 0;
		[ "$3" == "1" ] && [ "$var" != "$ifname" ] && return 0;

		IFNAME="${var}"

		config_get var "$section" "logicalIf"
		[ ! -z $var ] && IF_LOGICAL="$var" || IF_LOGICAL="${IFNAME}"

		config_get var "$section" "phydev"
		[ ! -z $var ] && IF_BASE="$var" || IF_BASE="${IFNAME}"

		config_get var "$section" "category"
		var=`echo "$var" | awk '{print toupper($0)}'`
		numVar=0
		for str in $IFCAT_LIST
		do
			[ "$str" == "$var" ] && break;
			numVar=$(($numVar+1))
		done
		IFTYPE=$numVar
	fi
}

set_deltacfg() {
	echo "[deltacfg]" >> $QOS_CONFIG
	D_IDX=$(($D_IDX+1))
	echo "d_${D_IDX}_entity=$DELTA_ENTITY" >> $QOS_CONFIG
	echo "d_${D_IDX}_operation=$DELTA_OPERATION" >> $QOS_CONFIG
	echo "d_${D_IDX}_entityid=$DELTA_INSTANCE" >> $QOS_CONFIG
	echo "d_${D_IDX}_oldentityid=$DELTA_OLD_ENTITY" >> $QOS_CONFIG
	if [ "$DELTA_ENTITY" != "qoscfg" ]; then
		config_foreach parse_interface interface "${DELTA_IFACE}" "2"
		echo "d_${D_IDX}_interface=$IFNAME" >> $QOS_CONFIG
		IFNAME=""
		config_foreach parse_interface interface "${DELTA_OLD_IFACE}" "2"
		if [ "${IFNAME}" == "" ]; then
			IFNAME="${DELTA_OLD_IFACE}"
		fi
		echo "d_${D_IDX}_oldinterface=$IFNAME" >> $QOS_CONFIG
	fi
}
echo -n > $QOS_CONFIG

config_load qos
config_foreach parse_deltacfg deltacfg
config_foreach parse_qoscfg qoscfg
config_foreach parse_interface interface
config_foreach parse_classify classify

[ $DELTA_PRESENT != 0 ] && set_deltacfg

chmod 640 $QOS_CONFIG
chown :qosd $QOS_CONFIG
