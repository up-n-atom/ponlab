#!/bin/sh

### Usage:
###     wmdctrl.sh : Show help
###     wmdctrl.sh start : Load the wmd module
###     wmdctrl.sh stop : Unload the wmd module
###     wmdctrl.sh enable <wlan_name> : Enable wlan
###     wmdctrl.sh disable <wlan_name> : Disable wlan
###     wmdctrl.sh <wlan_name> get : Get WAN metrics for wlan_name

usage()
{
	echo "wmdctrl.sh <ACTION> [VALUE]"
	echo
	echo "ACTION:"
	echo "  help                    Show this help message."
	echo "  start                   Start WMD with default configuration file (/etc/wmd.conf)."
	echo "  start [CONF_FILE_PATH]  Start WMD with specified configuration file"
	echo "  restart                 Makes wmd reload conf file /ramdisk/etc/wmd.conf"
	echo "  stop                    Stop WMD."
	echo "  <wlan_name> get         Get WAN metrics for <wlan_name>."
	echo "  enable <wlan_name>      Enable a given wlan"
	echo "  disable <wlan_name>     Disable a given wlan"
	echo "  set duration [VALUE]    Set interval value for wmd operation."
	echo "  set wan_port [VALUE]    Set the port id for MII1 wan."
	echo "  set q_active [VALUE]    Enable/disable qos_rate_update operations."
	echo "  set wlan 	[name=<wlan_name>] 
				[threshold=<threshold_at_which_at_capacity_flag_is_set>] 
				[w_active=<1 if VAP is HS2.0 enabled>] 
				[wans=<wan_list>]  
				Set wlan related parameters."
	echo "                          wan_list is a colon separated list of wans for the VAP. Ex, <wan0:wan1>"
	echo " Example wmdctrl.sh set duration <duration> set q_active <value>"
}

create_default_conf()
{
	#Get default wan port from /etc/switchports.conf
	wan_port=0
	sed '/wan_port/d' $FILENAME > $FILENAME_TEMP
	mv $FILENAME_TEMP $FILENAME 2>>/dev/null
	echo "# Ethernet wan_port (wan_port is always MII1)" >> $FILENAME
	wan_port=$(cat /opt/lantiq/etc/switchports.conf | grep "switch_mii1_port" | cut -f2 -d "\"")
	echo "wan_port=$wan_port" >> $FILENAME

	#Create default wlan_conf for hs_20 enabled VAP
	eval hs2_ap_count='$'wlan_hs2_Count
	hs2_ap_idx=0
	eval default_wan='$'default_wan_conn_iface
	while [ $hs2_ap_idx -lt $hs2_ap_count ] ; do
		eval hs2_mode='$'wlhs2_${hs2_ap_idx}_hs20Mode
		if [ $hs2_mode -ne 0 ] ; then
			wlan_if=`find_wave_if_from_index $hs2_ap_idx`
			x=$(cat $FILENAME | grep $wlan_if | cut -f1 -d " ")
			if [ "$wlan_if" != "$x" ] ; then
				set_wlan "name=$wlan_if" "threshold=90" "w_active=1" "wans=$default_wan"
			else
				set_wlan "name=$wlan_if" "enable=1"
			fi
		fi
		hs2_ap_idx=`expr $hs2_ap_idx + 1`
	done
}




set_wlan()
{
	wlan_name=$(echo $1 | cut -f2 -d=)
	[ -z $wlan_name ] && echo "No hs2.0 enabled wlan" && return
	threshold=-1
	wan_list="none"
	w_active=-1
	wlan_enable=-1
	token="none"
	value="none"
	shift

	# get the values
	while [ $# -ne 0 ]; do
		token=$(echo $1 | cut -f1 -d=)
		value=$(echo $1 | cut -f2 -d=)
		case "$token" in
		threshold)
			threshold=$value
			if [ $threshold -gt 100 ] || [ $threshold -lt 0 ]; then 
				echo "Improper threshold set, Reverting to 100"
				threshold=100
			fi
			shift
			;;
		w_active)
			w_active=$value
			shift
			;;
		wans)
			wan_list=$value
			shift
			;;
		enable)
			wlan_enable=$value
			shift
			;;
		*)
			echo "Unrecognised parameter"
			shift
			;;
		esac
	done


	# handle enable/disable first
	if [ $wlan_enable -ne -1 ] ; then
		LINE="none"
		tmp_str="none"
		block=0
		touch $FILENAME_TEMP
		cat $FILENAME | while read LINE
		do
			tmp_str=$(echo $LINE | cut -f1 -d " ")
			[ "$tmp_str" = "$wlan_name" ] && block=1
			[ "$tmp_str" = "}" ] && [ $block -eq 1 ] && block=0
        		if [ $block -eq 1 ] ; then
					case "$LINE" in
					*w_active*)
						LINE="    w_active=$wlan_enable"
						;;
					esac
				fi
				echo $LINE >> $FILENAME_TEMP
		done
		mv $FILENAME_TEMP $FILENAME 2>>/dev/null
		if [ $threshold -ne -1 ] || [ $w_active -ne -1 ] || [ $wan_list != "none" ] ; then 
			echo "Enabling/disabling $wlan_name and setting parameters should be done separately"
		fi
	fi
	[ $wlan_enable -ne -1 ] && return


	# remove any earlier entries
	LINE="none"
	tmp_str="none"
	block=0
	touch $FILENAME_TEMP
	cat $FILENAME | while read LINE
	do
		tmp_str=$(echo $LINE | cut -f1 -d " ")
		[ "$tmp_str" = "$wlan_name" ] && block=1 && continue
		[ "$tmp_str" = "}" ] && [ $block -eq 1 ] && block=0 && continue
        [ $block -eq 0 ] && echo "$LINE" >> $FILENAME_TEMP
	done
	mv $FILENAME_TEMP $FILENAME 2>>/dev/null
	

	# insert new block
	echo "$wlan_name {" >> $FILENAME
	if [ $threshold -ne -1 ] ; then
		echo "    # Percentage of downlink load when the \"At Capacity\" flag will be set" >> $FILENAME
		echo "    threshold=$threshold" >> $FILENAME
	fi
	if [ $w_active -ne -1 ] ; then
		echo "    # Whether WMD is active for interface" >> $FILENAME
		echo "    w_active=$w_active" >> $FILENAME
	fi
	if [ $wan_list != "none" ]; then
		i=1
		wan_name=$(echo $wan_list | cut -f$i -d:)
		i=`expr index "$wan_list" :`
		if [ $i -eq 0 ] ; then 
			wan_name=$(echo $wan_list)
			echo "    # Active WAN $i index" >> $FILENAME
			echo "    wan=$wan_name" >> $FILENAME
			echo "}" >> $FILENAME
			return
		fi
		i=1
		while [ ".$wan_name" != "." ] ; do
			echo "    # Active WAN $i index" >> $FILENAME
			echo "    wan=$wan_name" >> $FILENAME
			i=`expr $i + 1`
			wan_name=$(echo $wan_list | cut -f$i -d:)
		done
	fi
	echo "}" >> $FILENAME
}

duration=0
q_active=-1
ETC_CONF=/ramdisk/etc/wmd.conf
FILENAME=$ETC_CONF
FILENAME_TEMP=/tmp/wmd.conf.temp
PID_FILE=/var/run/wmd.pid
WMD_FIFO=/tmp/wmd_fifo


if [ ! "$ENVLOADED" ]; then
	if [ -r /etc/rc.conf ]; then
		. /etc/rc.conf 2> /dev/null
		ENVLOADED="1"
	fi
fi

if [ ! "$CONFIGLOADED" ]; then
	if [ -r /etc/rc.d/config.sh ]; then
		. /etc/rc.d/config.sh 2>/dev/null
		CONFIGLOADED="1"
	fi
fi

if [ ! "$MTLK_INIT_PLATFORM" ]; then
	if [ -f /tmp/mtlk_init_platform.sh ]; then
		. /tmp/mtlk_init_platform.sh
	fi
fi


if [ -z $1 ]; then
	usage
	exit 0
fi

if [ "$2" = "get" ] && [ -n $1 ] ; then
	if [ ! -p $WMD_FIFO ]; then
		exit 1
	fi
	kill -s SIGUSR2 $(cat $PID_FILE) 2>/dev/null
	cat <$WMD_FIFO | grep "$1:"
	exit 0
fi

while [ $# -ne 0 ]; do
	case "$1" in
	help|-h)
		usage
		exit 0
		;;
	start)
		if [ -f $PID_FILE ]; then
			echo " WMD already running or pid file from previous run not deleted"
			echo ""
			exit 1
		fi
		if [ -z $2 ]; then
			create_default_conf
			/usr/sbin/qos_rate_update
		else
			/usr/sbin/qos_rate_update -c $2
			shift
		fi
		;;
	stop)
		killall qos_rate_update 2>/dev/null
		rm -f $PID_FILE 2>>/dev/null
		;;
	restart)
		kill -s SIGHUP $(cat $PID_FILE) 2>>/dev/null
		;;
	enable)
		[ -z $2 ] && echo "provide wlan name" && exit 1
		set_wlan "name=$2" "enable=1"
		kill -s SIGHUP $(cat $PID_FILE) 2>>/dev/null
		shift
		;;
	disable)
		[ -z $2 ] && echo "provide wlan name" && exit 1
		set_wlan "name=$2" "enable=0"
		kill -s SIGHUP $(cat $PID_FILE) 2>>/dev/null
		shift
		;;
	set)
		case "$2" in
		duration)
			if [ -z $3 ]; then
				echo "No duration mentioned"
				exit 1
			fi
			if [ -f $FILENAME ] ; then 
				sed '/duration/d' $FILENAME > $FILENAME_TEMP
				mv $FILENAME_TEMP $FILENAME 2>>/dev/null
				sed '/Duration/d' $FILENAME > $FILENAME_TEMP
				mv $FILENAME_TEMP $FILENAME 2>>/dev/null
			fi
			echo "# Duration in seconds" >> $FILENAME
			echo duration=$3 >> $FILENAME
			shift;shift;
			kill -s SIGHUP $(cat $PID_FILE) 2>>/dev/null
			;;
		q_active)
			if [ -z $3 ]; then
				echo "No value mentioned"
				exit 1
			fi
			if [ -f $FILENAME ] ; then 
				sed '/q_active/d' $FILENAME > $FILENAME_TEMP
				mv $FILENAME_TEMP $FILENAME 2>>/dev/null
				sed '/QoS Rate update/d' $FILENAME > $FILENAME_TEMP
				mv $FILENAME_TEMP $FILENAME 2>>/dev/null
			fi
			echo "# Whether QoS Rate update is active" >> $FILENAME
			echo q_active=$3 >> $FILENAME
			kill -s SIGHUP $(cat $PID_FILE) 2>>/dev/null
			shift;shift;
			;;
		wan_port)
			if [ -z $3 ]; then
				echo "No value mentioned"
				exit 1
			fi
			if [ -f $FILENAME ] ; then 
				sed '/wan_port/d' $FILENAME > $FILENAME_TEMP
				mv $FILENAME_TEMP $FILENAME 2>>/dev/null
			fi
			echo "# Ethernet wan_port (wan_port is always MII1)" >> $FILENAME
			echo "wan_port=$3" >> $FILENAME
			kill -s SIGHUP $(cat $PID_FILE) 2>>/dev/null
			shift;shift;
			;;
		wlan)
			if [ -z $3 ]; then
				echo "No wlan name mentioned"
				exit 1
			fi
			shift;shift
			set_wlan $@
			kill -s SIGHUP $(cat $PID_FILE) 2>>/dev/null
			exit 0
			;;
		*)
			echo "Improper argument"
			;;
		esac
		;;
	*)
		echo "Improper argument"
		exit 1
		;;
	esac
	shift
done

