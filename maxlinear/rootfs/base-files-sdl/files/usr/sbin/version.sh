#!/bin/sh
#
# version.sh prints versions of subsystem modules defined in this script.
# To add a new subsystem, simply define ver_<subsystem>() function and echo the version content.
#

if [ -z "$VENDOR_PATH" ]; then
	VENDOR_PATH="/opt/intel"
fi

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
	[ -n "$_bootloader" -a "${_bootloader//BOOTLoader}" != "$_bootloader" ] && {
		if [ "$_bootloader" != "${_bootloader//=*}" ]; then
			_bootloader="${_bootloader//=*}"
			_bootloader="${_bootloader% *}"
		fi
		echo $_bootloader;
	}
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

ver_dsl()
{
	local _fw_vdsl_ver _fw_adsl_ver _mei_ver _drv_ver _app_ver _fapi_ver _sl_ver;
	local _bin_dir="$VENDOR_PATH/bin";
	local _dsl_fw_dir;
		
	grep -q drv_dsl_cpe_api /proc/modules && {
	
		_mei_ver=`echo $(what_string_get ${_bin_dir}/drv_mei_cpe.ko) | sed 's/.*version //'`
		_drv_ver=`echo $(what_string_get ${_bin_dir}/drv_dsl_cpe_api.ko) | cut -c14-`
		_app_ver=`echo $(what_string_get ${_bin_dir}/dsl_cpe_control) | cut -d@ -f2 | sed 's/(#)//'`
		_fapi_ver=`echo $(what_string_get /usr/lib/libdslfapi*) 2>/dev/null`
		[ -n "$_fapi_ver" ] && _fapi_ver=", FAPI $_fapi_ver"
		_sl_ver=`echo $(what_string_get /usr/lib/libdslsl*) 2>/dev/null`
		[ -n "$_sl_ver" ] && _sl_ver=", SL $_sl_ver"
		
		# Firmware directory might be /lib/firmware or /firmware, so we are checking both possibilities
		if [ -f /firmware/xcpe_hw.bin ]; then
			_dsl_fw_dir="/firmware"
		elif [ -f /lib/firmware/xcpe_hw.bin ]; then
			_dsl_fw_dir="/lib/firmware"
		fi
		
		if [ -n "${_dsl_fw_dir}" ]; then
			_fw_vdsl_ver=`echo $(what_string_get ${_dsl_fw_dir}/xcpe_hw.bin) | cut -f2 -d@ | cut -f1 -d' ' | sed 's/(#)//'`
			_fw_adsl_ver=`echo $(what_string_get ${_dsl_fw_dir}/xcpe_hw.bin) | cut -f3 -d@ | cut -f2 -d' ' |sed 's/(#)//'`
		fi

		echo "DSL: FW $_fw_vdsl_ver, $_fw_adsl_ver, MEI $_mei_ver, DRIVER $_drv_ver, APP $_app_ver${_fapi_ver}${_sl_ver}"
			
	}
}

ver_gfast()
{
    local _fw_ver _drv_dp_ver _drv_pmi_ver _drv_ep_ver _pmi_control_ver _dti_agent_ver _dti_device_ver _fapi_ver _daemon_ver _sl_ver
    local _bin_dir="$VENDOR_PATH/bin"
    local _mod_dir="/lib/modules/`uname -r`"
    local _lib_dir="/usr/lib"
    local _fw_path _fw_dir="/lib/firmware/09AA"

    grep -q dsl_gfast_drv_pmi /proc/modules && {
	if [ -f ${_fw_dir}/xcpe_fw.bin ]; then
	    _fw_path="${_fw_dir}/xcpe_fw.bin"
	elif [ -f ${_fw_dir}/xco_fw.bin ]; then
	    _fw_path="${_fw_dir}/xco_fw.bin"
	fi
	if [ -n "$_fw_path" ]; then
	    _fw_ver=`strings $_fw_path | grep "@(#)"|sed 's/\(.*\)@(#)\(.*\)/\2/g'`
	fi
	[ -f ${_bin_dir}/dsl_daemon ] && _daemon_ver=`what_string_get ${_bin_dir}/dsl_daemon | cut -d ' ' -f 3 | cut -c 2-`
	[ -f ${_lib_dir}/libdslfapi.so.0 ] && _fapi_ver=`what_string_get ${_lib_dir}/libdslfapi.so.0`
	[ -f ${_lib_dir}/libdslsl.so.0 ] && _sl_ver=`what_string_get ${_lib_dir}/libdslsl.so.0`

	[ -f ${_mod_dir}/dsl_gfast_drv_dp.ko ] && _drv_dp_ver=`what_string_get ${_mod_dir}/dsl_gfast_drv_dp.ko | cut -d ' ' -f 5`
	[ -f ${_mod_dir}/dsl_gfast_drv_vrx618_ep.ko ] && _drv_ep_ver=`what_string_get ${_mod_dir}/dsl_gfast_drv_vrx618_ep.ko | cut -d ' ' -f 6`
	[ -f ${_mod_dir}/dsl_gfast_drv_pmi.ko ] && _drv_pmi_ver=`what_string_get ${_mod_dir}/dsl_gfast_drv_pmi.ko | cut -d ' ' -f 5`

	[ -f ${_lib_dir}/libcontrol_pmi.so.0 ] && _pmi_control_ver=`what_string_get ${_lib_dir}/libcontrol_pmi.so.0 | cut -d ' ' -f 5`
	[ -f ${_lib_dir}/libdti_agent_pmi.so.0 ] && _dti_agent_ver=`what_string_get ${_lib_dir}/libdti_agent_pmi.so.0 | cut -d ' ' -f 7`
	[ -f ${_lib_dir}/libdti_device_pmi.so.0 ] && _dti_device_ver=`what_string_get ${_lib_dir}/libdti_device_pmi.so.0 | cut -d ' ' -f 6`

	[ -n "$_fw_ver" ] && echo "DSL FW:$_fw_ver"
	[ -n "$_daemon_ver" ] && echo "DSL DAEMON: $_daemon_ver"

	[ -n "$_fapi_ver" ] && echo "DSL FAPI: $_fapi_ver"
	[ -n "$_sl_ver" ] && echo "DSL SL: $_sl_ver"

	[ -n "$_drv_dp_ver" ] && echo "DSL DataPath: $_drv_dp_ver"
	[ -n "$_drv_ep_ver" ] && echo "DSL EP: $_drv_ep_ver"
	[ -n "$_drv_pmi_ver" ] && echo "DSL PMI: $_drv_pmi_ver"

	[ -n "$_pmi_control_ver" ] && echo "DSL PMI Control Access: $_pmi_control_ver"
	[ -n "$_dti_agent_ver" ] && echo "DTI Agent: $_dti_agent_ver"
	[ -n "$_dti_device_ver" ] && echo "DTI Device: $_dti_device_ver"
    }
}

ver_xtc()
{
	# Finding PTM/ATM TC driver version info
	local _xtc_ver;
	_xtc_ver=`cat /proc/driver/vrx518/ver 2>/dev/null|grep -E 'ver'|sed -e 's/:/: /' -e 's/^[ \t]*//'`
	[ -n "$_xtc_ver" ] && echo "$_xtc_ver"
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

ver_switch()
{
	# Switch API information
	local _switch_ver;
	_switch_ver="`switch_cli GSW_VERSION_GET|sed -e '/Version String:.*\..*/!d' -e 's/.*Version.*: //' 2>/dev/null`"
	if [ -z "$_switch_ver" ]; then
		_switch_ver="`switch_cli IFX_ETHSW_VERSION_GET|sed -e '/Version String:.*\..*/!d' -e 's/.*Version.*: //' 2>/dev/null`"
	fi
	echo "Switch API Version: $_switch_ver"
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

ver_voice()
{
	# only ARX168 and GRX168 don't support voice functionaility
	local tapi_str
	if [ -r /proc/driver/tapi/version ] && [ -r /proc/driver/vmmc/version ] && [ -r /proc/driver/ltq_mps_voice/version ]; then
		tapi_str="TAPI `grep TAPI /proc/driver/tapi/version | cut -d' ' -f3`, VMMC `grep VMMC /proc/driver/vmmc/version | cut -d ' ' -f6`, MPS `grep MPS /proc/driver/ltq_mps_voice/version | cut -d' ' -f6`"
		echo Voice: $tapi_str
	fi
}

ver_voip()
{
	[ -f /usr/sbin/VoIP_VERSION ] && {
		sed 's/.*em/\0: \1/' /usr/sbin/VoIP_VERSION
	}
}

ver_dect()
{
	if [ -f /proc/driver/dect/version ]; then
		sed 's/.*em/\0: \1/' /usr/sbin/DECT_VERSION
		sed -e '/^$/d' -e '/Modules.*/d' -e 's/.*[a-z]/\0: \1/' /proc/driver/dect/version
	fi
}

ver_devm()
{
	if [ -f /usr/sbin/DEVMD_VERSION ];  then
		sed 's/ :/: /' /usr/sbin/DEVMD_VERSION
	fi
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

