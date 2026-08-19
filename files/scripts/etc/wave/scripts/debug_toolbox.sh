#!/bin/sh

. /lib/wifi/platform_dependent.sh

version="20190708T1059"
script_name="$0"
command=$1
pc_ip=$2
param1=$3

burn_cal_file()
{
	local no_restart tftp_path interfaces_list interface_name burn_both cal_status

	no_restart=$1
	[ -z "$pc_ip" ] && echo "The PC IP parameter is missing." && exit

	tftp_path=${param1%\/*}
	interfaces_list=${param1##*\/}
	if [ "$tftp_path" = "$interfaces_list" ]
	then
		tftp_path=""
	else
		tftp_path="$tftp_path/"
	fi

	[ ! -d ${CAL_FILES_PATH} ] && mkdir -p ${CAL_FILES_PATH}
	cd ${CAL_FILES_PATH}
	cal_status=0
	interface_name=${interfaces_list%%,*}
	while [ -n "$interface_name" ]
	do
		if [ "$interface_name" = "all" ]
		then
			tftp -gr "${tftp_path}cal_wlan0.bin" -l cal_wlan0.bin $pc_ip
			cal_status=$(( $cal_status + `echo $?` ))
			tftp -gr "${tftp_path}cal_wlan2.bin" -l cal_wlan2.bin $pc_ip
			cal_status=$(( $cal_status + `echo $?` ))
			tftp -gr "${tftp_path}cal_wlan4.bin" -l cal_wlan4.bin $pc_ip
			cal_status=$(( $cal_status + `echo $?` ))
		else
			tftp -gr "${tftp_path}cal_${interface_name}.bin" -l cal_${interface_name}.bin $pc_ip
			cal_status=$(( $cal_status + `echo $?` ))
		fi
		interfaces_list=${interfaces_list#$interface_name}
		interfaces_list=${interfaces_list#,}
		interface_name=${interfaces_list%%,*}
	done
	cd - > /dev/null
	
	${SCRIPTS_PATH}/flash_file_saver.sh
	ret=$?
	if [ $ret = 0 ]
	then
		echo "$script_name: calibration files saved to flash, rebooting..."
		reboot
	else
		echo "$script_name: ERROR - failed to save calibration files to flash." >&2
		exit -1
	fi
}

remove_flash_cal_files()
{
	if [ -d "/nvram/etc/wave_calibration" ]
	then
		chattr -i /nvram/etc/wave_calibration #remove write protection
		rm -rf /nvram/etc/wave_calibration
		sync
		reboot
	fi
}

wlan_status_info()
{
                local hstpd_wlan_cnt
                hstpd_wlan_cnt=$(ps | grep -c hostapd-phy)

                if [ $hstpd_wlan_cnt -gt 1 ]; then
                   echo -e "\e[4mwlan0\e[0m" > /dev/console
                   hostapd_cli -i wlan0 stat | grep -w -i 'ssid\[1\]\|bssid\[1\]\|channel\|freq\|num_sta\[1\]'
                fi

                if [ $hstpd_wlan_cnt -gt 2 ]; then
                   echo -e "\n\e[4mwlan2\e[0m" > /dev/console
                   hostapd_cli -i wlan2 stat | grep -w -i 'ssid\[1\]\|bssid\[1\]\|channel\|freq\|num_sta\[1\]'
                fi
}

setting_overlay()
{

	local user_select trace

	echo -e "####### overlay setting ############" > /dev/console
	echo -e "## 0. overlay Disabled            ##" > /dev/console
	echo -e "## 1. overlay Enabled             ##" > /dev/console
	echo -e "####################################" > /dev/console
	echo -e "Enter selection:" > /dev/console
	echo -ne ">>" > /dev/console;read user_select

	case $user_select in
	"0")
		overlay.sh remove
		trace="Change overlay configuation Disabled..rebooting"
	;;
	"1")
		overlay.sh create
		trace="Change overlay configuation Enabled...rebooting"
	;;
	esac

	echo -e "$trace" > /dev/console
	/etc/wave/scripts/wave_factory_reset.sh
	sleep 2
	reboot
}

setting_work_mode()
{
	systemctl stop CcspPandMSsp
	systemctl stop CcspEthAgent
}

wlan_version()
{
	iw dev wlan0 iwlwav gEEPROM
	iw dev wlan2 iwlwav gEEPROM
	cat /proc/net/mtlk/version
	cat ${WAVE_COMPONENTS_PATH}/wave_components.ver
	uname -a
}

wlan_factory()
{
	echo -e "####### #######################################################" > /dev/console
	echo -e "####### factroy ( on RDKB overlay is not deleted ) ############" > /dev/console
	echo -e "####### reboot...please wait                       ############" > /dev/console
	echo -e "####### #######################################################" > /dev/console
	sleep 5
	if [ $OS_NAME = "RDKB" ] ;then
		systemctl start CcspEthAgent
		systemctl start CcspPandMSsp
		fapi_wlan_cli factoryReboot
	else
		ubus call csd factoryreset '{ "object":"Device.WiFi."}'
	fi
}

dut_get_cv()
{
	cat ${WAVE_COMPONENTS_PATH}/wave_components.ver
}
case $command in
	dut_get_cv)
		dut_get_cv
	;;
	burn_cal)
		burn_cal_file
	;;
	remove_cal)
		remove_flash_cal_files
	;;
	"wlan_status"|"ws")
		wlan_status_info
	;;
	"overlay"|"ov")
		setting_overlay
	;;
	"work_mode"|"wm")
		setting_work_mode
	;;
	"wlan_version"|"wv")
		wlan_version
	;;
	"wlan_factroy"|"wf")
		wlan_factory
	;;
	"wlan_collect_debug"|"wcd")
		${SCRIPTS_PATH}/wave_collect_debug.sh
	;;
	"wlan_collect_debug_assert"|"wcda")
		${SCRIPTS_PATH}/wave_collect_debug.sh -a $pc_ip
	;;
	"wlan_collect_debug_config"|"wcdc")
		${SCRIPTS_PATH}/wave_collect_debug.sh -c
	;;
	*)

		[ "$command" = "" ] && command="help"
		echo -e "$script_name Version:$version: Unknown command $command\n \
		Usage: $script_name COMMAND [Argument 1] [Argument 2]\n" \
		"example: wavToolBox wcda 192.168.0.100 \n" \
		 "\n" \
		 "Commnads:\n" \
		 "burn_cal       Burn the calibration files\n" \
		 "  Arguments:\n" \
		 "  Argument 1:  Your PC IP\n" \
		 "  Argument 2:  The interface name or names to which calibration is burned: wlan0/wlan2/wlan4/all\n" \
		 "               Names can be specified in a comma-separated list: wlan0,wlan2\n" \
		 "               This argument can contain also the path in the tftp server before the interface name: /path/wlan\n" \
		 "               Example: $script_name burn_cal <PC IC> /private_folder/wlan0,wlan2,wlan4\n" \
		 "remove_cal                  Removes /nvram/etc/wave_calibration directory if exists\n" \
		 "wlan_status                (ws) gives wlan interface main vaps (wlan0.0 and wlan2.0) status\n" \
		 "overlay                    (ov) setting overlay /bin /etc /lib access\n" \
		 "work_mode                  (wm) setting debug work mode - disable un-needed terminal traces \n" \
		 "wlan_version               (wv) getting wlan version info includes:eeprom,kernel_version,cv \n" \
		 "wlan_factory               (wf) complete clean-up ( overlay will not be deleted ) \n" \
		 "dut_get_cv                 returns wave_components.ver content \n" \
		 "wlan_collect_debug         (wcd) wlan collect debug info\n" \
		 "wlan_collect_debug_assert  (wcda) <tftp ip> wlan collect debug info after triggring FW assert and optional upload to tftp\n" \
		 "wlan_collect_debug_config  (wcdc) Only for RDKB:reconfig the syslog to save all future logs to a single file\n"
	;;
esac
