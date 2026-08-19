#!/bin/sh
#
# version.sh prints versions of subsystem modules defined in this script.
# To add a new subsystem, simply define ver_<subsystem>() function and echo the version content.
#

# Function to extract what string from a binary or kernel object
# Arguments for this function
#   $1: Fully qualified name of binary or object to extract the what string
# Return value(s):
#   Version string
what_string_get() {
	local _obj_name=$1
	local _what_string
	
	for i in ${_obj_name}; do
		_what_string=`strings $i | grep "@(#)" | sed 's/\(.*\)@(#)\(.*\)/\2/g'`
	done
	
	echo "$_what_string"
}

ver_system()
{
	local _bootloader _system_type _machine _model_name _cpus _clock;
	_bootloader="`sed 's/.*ubootver=\(.*\)/BOOTLoader: \1/g' /proc/cmdline`"
	if [ -n "$_bootloader" ]; then
		[ "${_bootloader//BOOTLoader}" != "$_bootloader" ] && {
			if [ "$_bootloader" != "${_bootloader//=*}" ]; then
				_bootloader="${_bootloader//=*}"
				_bootloader="${_bootloader% *}"
			fi
			echo $_bootloader;
		}
	fi
	_machine="`sed -e '/machine.*:/!d' -e 's/machine.*: //' /proc/cpuinfo`"
	if [ -n "$_machine" ] && [ "$_machine" != "Unknown" ]; then
		_cpus="$_machine";
	else
		_model_name="`sed -e '/model.*name.*:/!d' -e 's/model.*name.*: //' /proc/cpuinfo|sort -u 2>/dev/null`"
		[ -n "$_model_name" ] && {
			_cpus="$_model_name";
		} || {
			_system_type="`sed -e '/system.*type.*:/!d' -e 's/system.*type.*: //' /proc/cpuinfo`"
			[ -n "$_system_type" ] && _cpus="$_system_type";
		}
	fi
	_clock="`sed -e '/cpu.*MHz.*:/!d' -e 's/cpu.*MHz.*: //' /proc/cpuinfo|sort -u 2>/dev/null`"
	echo "CPU: $_cpus"
	[ -n "$_clock" ] && {
		echo "CPU Clock: $_clock"
	}
	echo "Kernel: `uname -r`"
	echo "Software: `cat /etc/version`-`cat /etc/timestamp`"
	[ -f "/etc/ugw_tag" ] && echo "UGW Build Tag: $(cat /etc/ugw_tag)"
}

ver_toolchain()
{
	local _toolchain_ver;
	if [ -f /etc/toolchain_ver ]; then
		echo "Toolchain: `cat /etc/toolchain_ver`";
	else
		_toolchain_ver=`/usr/sbin/upgrade 2>/dev/null|sed -e '/ToolChain/!d' -e 's/ToolChain://'`
		if [ -n "$_toolchain_ver" ]; then
			echo "Toolchain: $_toolchain_ver";
		else
			[ -f "/tmp/toolchain-ver" ] && {
				echo "Toolchain: `cat /tmp/toolchain-ver`";
			}
		fi
	fi
}

ver_ppa()
{
	# Finding PPE/PPA version info
	local _ppa_ver
	_ppa_ver=`ppacmd getversion 2>/dev/null|grep -E 'info|version'|sed -e 's/:/: /' -e 's/^[ \t]*//'`
	[ -n "$_ppa_ver" ] && echo "$_ppa_ver"
}

ver_bios()
{                                                                                                  
        local _bios_ver   
        _bios_ver=`dmesg | grep -w DMI | awk '{print $(NF-1)}' 2>/dev/null`
        [ -n "$_bios_ver" ] && echo "Bios Version : $_bios_ver" 
}   

ver_wlan()
{
	[ -e /etc/wave_components.ver ] && . /etc/wave_components.ver
	[ -e /etc/iwlwav_driver.ver ] && . /etc/iwlwav_driver.ver
	[ -e /etc/iwlwav_hostap.ver ] && . /etc/iwlwav_hostap.ver
	[ -e /etc/iwlwav_iw.ver ] && . /etc/iwlwav_iw.ver
	[ -e /etc/iwlwav_tools.ver ] && . /etc/iwlwav_tools.ver
	[ -e /opt/prplmesh/config/version ] && . /opt/prplmesh/config/version

	echo "Wave wlan version: $wave_release_minor"
	[ -n "$prplmesh_version" ] && echo "Wave wlan APM version: prplmesh_${prplmesh_version}_${prplmesh_revision}_${prplmesh_build_date}"
	[ -n "$wave_driver_ver" ] && echo "Wave wlan driver version: $wave_driver_ver"
	[ -n "$iwlwav_driver_hash" ] && echo "Wave wlan driver hash: $iwlwav_driver_hash"
	[ -n "$iwlwav_hostap_hash" ] && echo "Wave wlan hostap hash: $iwlwav_hostap_hash"
	[ -n "$iwlwav_tools_hash" ] && echo "Wave wlan tools hash: $iwlwav_tools_hash"
	[ -n "$iwlwav_iw_hash" ] && echo "Wave wlan iw hash: $iwlwav_iw_hash"
	[ -n "$wave_mac_ver" ] && echo "Wave wlan MAC FW version: $wave_mac_ver"
	[ -n "$wave_tx_sender_ver" ] && echo "Wave wlan tx_sender version: $wave_tx_sender_ver"
	[ -n "$wave_rx_handler_ver" ] && echo "Wave wlan rx_handler version: $wave_rx_handler_ver"
	[ -n "$wave_host_interface_ver" ] && echo "Wave wlan host_interface version: $wave_host_interface_ver"
	[ -n "$wave_tx_sender_gen5b_ver" ] && echo "Wave wlan tx_sender_gen5b version: $wave_tx_sender_gen5b_ver"
	[ -n "$wave_rx_handler_gen5b_ver" ] && echo "Wave wlan rx_handler_gen5b version: $wave_rx_handler_gen5b_ver"
	[ -n "$wave_host_interface_gen5b_ver" ] && echo "Wave wlan host_interface_gen5b version: $wave_host_interface_gen5b_ver"
	[ -n "$wave_tx_sender_gen6_ver" ] && echo "Wave wlan tx_sender_gen6 version: $wave_tx_sender_gen6_ver"
	[ -n "$wave_rx_handler_gen6_ver" ] && echo "Wave wlan rx_handler_gen6 version: $wave_rx_handler_gen6_ver"
	[ -n "$wave_host_interface_gen6_ver" ] && echo "Wave wlan host_interface_gen6 version: $wave_host_interface_gen6_ver"
	[ -n "$wave_tx_sender_gen6b_ver" ] && echo "Wave wlan tx_sender_gen6b version: $wave_tx_sender_gen6b_ver"
	[ -n "$wave_rx_handler_gen6b_ver" ] && echo "Wave wlan rx_handler_gen6b version: $wave_rx_handler_gen6b_ver"
	[ -n "$wave_host_interface_gen6b_ver" ] && echo "Wave wlan host_interface_gen6b version: $wave_host_interface_gen6b_ver"
	[ -n "$wave_tx_sender_gen6d2_ver" ] && echo "Wave wlan tx_sender_gen6d2 version: $wave_tx_sender_gen6d2_ver"
	[ -n "$wave_rx_handler_gen6d2_ver" ] && echo "Wave wlan rx_handler_gen6d2 version: $wave_rx_handler_gen6d2_ver"
	[ -n "$wave_host_interface_gen6d2_ver" ] && echo "Wave wlan host_interface_gen6d2 version: $wave_host_interface_gen6d2_ver"
	[ -n "$wave_ar10_progmodel_ver" ] && echo "Wave wlan AR10 PROGMODEL version: $wave_ar10_progmodel_ver"
	[ -n "$wave500_progmodel_ver" ] && echo "Wave wlan Wave500 PROGMODEL version: $wave500_progmodel_ver"
	[ -n "$wave500B_progmodel_ver" ] && echo "Wave wlan Wave500B PROGMODEL version: $wave500B_progmodel_ver"
	[ -n "$wave600_progmodel_ver" ] && echo "Wave wlan Wave600 PROGMODEL version: $wave600_progmodel_ver"
	[ -n "$wave600B_progmodel_ver" ] && echo "Wave wlan Wave600B PROGMODEL version: $wave600B_progmodel_ver"
	[ -n "$wave600D2_progmodel_ver" ] && echo "Wave wlan Wave600D2 PROGMODEL version: $wave600D2_progmodel_ver"
	[ -n "$wave700_progmodel_ver" ] && echo "Wave wlan Wave700 PROGMODEL version: $wave700_progmodel_ver"
	[ -n "$wave_sw_atom_tag" ] && echo "Wave SW Atom(TM) tag: $wave_sw_atom_tag"
	[ -n "$wave_fpga_sim_ver" ] && echo "FPGA SIM version: $wave_fpga_sim_ver"
	[ -n "$wave_psd_ver" ] && echo "Wave wlan PSD version: $wave_psd_ver"
	[ -n "$wave_scripts_ver" ] && echo "Wave wlan scripts version: $wave_scripts_ver"
	[ -n "$wave_regulatory_ver" ] && echo "Wave wlan regulatory version: $wave_regulatory_ver"	
}

ver_voip()
{
	[ -f /usr/sbin/VoIP_VERSION ] && {
		sed 's/.*em/\0: \1/' /usr/sbin/VoIP_VERSION
	}
}

ver_pm()
{
	local VER_STR DRV_STR RAILNAME
	if [ -e /opt/intel/bin/pm_util ]; then
		VER_STR=`/opt/intel/bin/pm_util -v`
		echo "PM pm_util version: $VER_STR"
	fi

	if [ -e /opt/intel/bin/ppmd ]; then
		VER_STR=`/opt/intel/bin/ppmd -v`
		echo "PM ppmd version: $VER_STR"
	fi  

	if [ -e /sys/devices/system/cpu/cpu0/cpufreq/scaling_driver ];then
		DRVNAME_STR=`cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_driver`
		echo "PM cpu scaling drv: $DRVNAME_STR"
	fi

	for i in /sys/class/hwmon/hwmon*; do
		if [ -e $i/name ]; then
			RAILNAME=""
			DRV_STR=`cat $i/name`
			if [ $DRV_STR = 'ina219' ] || [ $DRV_STR = 'ina226' ]; then
				RAILNAME=`grep [A-Z,a-z] $i/of_node/rail-name`
			fi
			echo "PM hwmon: $DRV_STR $RAILNAME"
		fi
	done
}

ver_pon()
{
	local _bin_dir="/usr/bin"
	local _mod_dir="/lib/modules/`uname -r`"
	local _lib_dir="/usr/lib"

	[ -e /etc/pon.ver ] && echo "PON subsystem version:" `cat /etc/pon.ver`

	[ -f ${_mod_dir}/mod_pon_mbox.ko ] && echo `what_string_get ${_mod_dir}/mod_pon_mbox.ko`
	[ -f ${_mod_dir}/mod_pon_eth.ko ] && echo `what_string_get ${_mod_dir}/mod_pon_eth.ko`
	[ -f ${_mod_dir}/mod_pon_mcc.ko ] && echo `what_string_get ${_mod_dir}/mod_pon_mcc.ko`
	[ -f ${_mod_dir}/mod_sfp_i2c.ko ] && echo `what_string_get ${_mod_dir}/mod_sfp_i2c.ko`
	[ -f ${_lib_dir}/libadapter.so ] && echo `what_string_get ${_lib_dir}/libadapter.so`
	[ -f ${_lib_dir}/libpon.so.0 ] && echo `what_string_get ${_lib_dir}/libpon.so.0`
	[ -f ${_lib_dir}/libponnet.so ] && echo `what_string_get ${_lib_dir}/libponnet.so`
	[ -f ${_lib_dir}/libponimg.so ] && echo `what_string_get ${_lib_dir}/libponimg.so`
	[ -f ${_lib_dir}/libtop.so ] && echo `what_string_get ${_lib_dir}/libtop.so`
	[ -f ${_bin_dir}/pon ] && echo `what_string_get ${_bin_dir}/pon`
	[ -f ${_bin_dir}/pond ] && echo `what_string_get ${_bin_dir}/pond`
	[ -f ${_bin_dir}/omcid ] && echo `what_string_get ${_bin_dir}/omcid`
	[ -f ${_bin_dir}/pontop ] && echo `what_string_get ${_bin_dir}/pontop`
	[ -f ${_bin_dir}/todd ] && echo `what_string_get ${_bin_dir}/todd`
	[ -f ${_bin_dir}/sfp_i2c ] && echo `what_string_get ${_bin_dir}/sfp_i2c`
	[ -f ${_bin_dir}/synce_tool ] && echo `what_string_get ${_bin_dir}/synce_tool`
}

ver_updk()
{
	if [ `opkg list | grep -c pwhm` -gt 0 ]; then
		opkg list | grep -e whm -e libsw -e libamx[b-d] > /tmp/updk_ver
		sed -i 's/ - /=/' /tmp/updk_ver
		sed -i 's/mod-whm-debug/mod_whm_debug/' /tmp/updk_ver
		sed -i 's/mod-whm/mod_whm/' /tmp/updk_ver
		. /tmp/updk_ver

		[ -n "$libamxb" ] && echo "UPDK libamxb: ${libamxb}"
		[ -n "$libamxc" ] && echo "UPDK libamxc: ${libamxc}"
		[ -n "$libamxd" ] && echo "UPDK libamxd: ${libamxd}"
		[ -n "$libswla" ] && echo "UPDK libswla: ${libswla}"
		[ -n "$libswlc" ] && echo "UPDK libswlc: ${libswlc}"
		[ -n "$mod_whm_debug" ] && echo "UPDK mod-whm-debug: ${mod_whm_debug}"
		[ -n "$mod_whm" ] && echo "UPDK mod-whm: ${mod_whm}"
		[ -n "$pwhm" ] && echo "UPDK pwhm: ${pwhm}"
	fi
}

#---- Donot Edit below this line -------------#

if [ -r /etc/config.sh ]; then
	. /etc/config.sh 2>/dev/null
fi

_functions=$(sed -e '/.*sed.*ver.*/d' -e '/.*ver_\$1/d' -e 's/(.*).*//' -e '/^ver_.*/p' -n $0)

if [ -n "$1" ]; then
	case "$1" in
		-h|--help|help) echo "Usage:- $0 [modules|-h (help)|-f (force show)]";
		echo "modules:-"; echo "$_functions"|sed 's/ver_/  /';
		exit 0;;
		-f|--force|force) rm -f /tmp/version;;
		*) if [ `echo "$_functions"|grep -w ver_$1` ]; then ver_$1; else
			echo "Not found. Use below modules:- ";
			echo "$_functions"|sed 's/ver_/  /';
		fi
		exit 0;;
	esac
fi
[ -f /tmp/version ] && {
	cat /tmp/version
} || {
	for _ii in $_functions; do $_ii; done > /tmp/version
	cat /tmp/version
}

