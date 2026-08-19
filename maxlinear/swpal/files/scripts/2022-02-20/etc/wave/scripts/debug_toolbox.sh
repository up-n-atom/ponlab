#!/bin/sh

. /lib/wifi/platform_dependent.sh

version="20221122T1800"
script_name="$0"
command=$1
pc_ip=$2
param1=$3
param2=$4
param3=$5
param4=$6
param5=$7
numargs=$#
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
	[ "${OS_NAME}" = "RDKB" ] && chattr -i ${CAL_FILES_PATH}
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
	[ "${OS_NAME}" = "RDKB" ] && chattr +i ${CAL_FILES_PATH}
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
	local save_mode image status rm_cal_files_dir interfaces_list interface_name

	# Detect which flash saving method to use
	save_mode=""
	# Check for upgrade tool (UGW mode)
	image=`which upgrade`
	status=$?
	if [ $status -eq 0 ]
	then
		save_mode="UGW"
	else
		save_mode="Puma"
	fi

	# Prepare relevant files to copy
	rm_cal_files_dir="/tmp/remove_cal_files"
	rm -rf ${rm_cal_files_dir}
	mkdir ${rm_cal_files_dir}
	cd ${rm_cal_files_dir}
	cp ${CAL_FILES_PATH}/cal_*.bin .

	# Remove calibration files according to input interfaces list
	interfaces_list=$1
	interface_name=${interfaces_list%%,*}
	while [ -n "$interface_name" ]
	do
		if [ "$interface_name" = "all" ]
		then
			rm ./*
		else
			rm ./cal_${interface_name}.bin
		fi
		interfaces_list=${interfaces_list#$interface_name}
		interfaces_list=${interfaces_list#,}
		interface_name=${interfaces_list%%,*}
	done

	# Save remaining cal files back to flash directory
	if [ "$save_mode" = "Puma" ]
	then
		#remove write protection
		chattr -i ${CAL_FILES_PATH}
		rm ${CAL_FILES_PATH}/*
		cp ./* ${CAL_FILES_PATH}
		#restore write protection
		chattr +i ${CAL_FILES_PATH}
		sync
	else
		tar czf wlanconfig_cal_files.tar.gz ./cal_*.bin &>/dev/null
		upgrade wlanconfig_cal_files.tar.gz wlanconfig 0 0
	fi

	echo "$script_name: required calibration files removed from flash, rebooting..."
	reboot
}

wlan_status_info()
{
	iw dev
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
	echo -e "####### factory ( on RDKB overlay is not deleted ) ############" > /dev/console
	echo -e "####### reboot... please wait                      ############" > /dev/console
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

wav_certification()
{
	local br_ip="$pc_ip"

	if [ -z "$br_ip" ]
	then
		if [ "$OS_NAME" = "UGW" ]
		then
				br_ip=`uci get network.lan.ipaddr`
		elif [ "$OS_NAME" = "UPDK" ]
		then
				br_ip=`ubus-cli IP.Interface.$lan.IPv4Address.$lan.IPAddress?`
				br_ip=`echo $br_ip | cut -d"=" -f2`
		else
				br_ip=`dmcli eRT setv Device.X_CISCO_COM_DeviceControl.LanManagementEntry.1.LanIPAddress`
				br_ip=${br_ip##*value: }
				br_ip=`echo $br_ip`
		fi
		[ "$br_ip" = "$DEFAULT_IP_ADDRESS" ] && br_ip="192.165.100.10"
	fi
	echo "Setting bridge IP to $br_ip" > /dev/console
	if [ "$OS_NAME" = "UGW" ]
	then
			ubus call uci set '{ "config" : "network" ,"section" : "lan", "values": {"ipaddr" : "'$br_ip'", "netmask" : "255.255.0.0"}}'
			ubus call uci commit '{"config" : "network" }'
            echo "object:Device.IP.Interface.IPv4Address.: :MODIFY" > /tmp/cert_ip_change.txt
			echo "param:IPAddress: :$br_ip" >> /tmp/cert_ip_change.txt
			echo "param:SubnetMask: :255.255.0.0" >> /tmp/cert_ip_change.txt
			caltest -s /tmp/cert_ip_change.txt -c SERVD
	elif [ "$OS_NAME" = "UPDK" ]
	then
			ubus-cli IP.Interface.$lan.IPv4Address.$lan.IPAddress=$br_ip
			ubus-cli IP.Interface.$lan.IPv4Address.$lan.SubnetMask=255.255.0.0
	else
			ifconfig brlan0 $br_ip netmask 255.255.0.0
	fi

	# Set certification flag to enabled
	[ "$OS_NAME" = "RDKB" ] && mkdir $WAVE_PATH
	touch "$CERTIFICATION_FILE_PATH"
	[ "$param1" = "wdbg" ] || [ "$param1" = "wlan_debug"] && touch "/tmp/wlan_debug"
	# Execute sigma-start.sh
	echo "Running sigma-start" > /dev/console
	/lib/netifd/sigma-start.sh
	#Sleep to let the sigma-ap run successfully
	sleep 10

	#Restart the network in order to have the number_of_debug_configs updated for the cert mode
	if [ "$OS_NAME" != "UPDK"]; then
		echo "Restarting network service ..."
		if [ "$OS_NAME" = "UGW" ]; then
			ap_tmp=`/etc/init.d/network restart`
		elif [ "$OS_NAME" = "RDKB" ]; then
			ap_tmp=`systemctl restart systemd-netifd.service`
		fi

		echo "Network Restart DONE"
	fi
}

driver_debug()
{
	local enable=$param1
	local interface="radio0"

	if [ $enable = "1" ] ;then
		#echo 8 cdebug=1 > /proc/net/mtlk_log/debug
		uci set wireless.$interface.debug_iw_post_up_20="sMtlkLogLevel 8 1 1"
	elif  [ $enable = "0" ]; then
		#echo 8 cdebug=0 > /proc/net/mtlk_log/debug
		uci set wireless.$interface.debug_iw_post_up_20="sMtlkLogLevel 8 0 1"
	fi

	if [ $OS_NAME = "RDKB" ] ;then
		uci commit wireless
		[ $enable = "1" ] && echo "" > /var/log/daemon
	else
		ubus call uci commit '{ "config" : "wireless" }'
		[ $enable = "1" ] && echo "" > /var/log/messages
	fi
	ubus call network restart
	cat /proc/net/mtlk_log/debug | grep -B1 'all\|Default log levels'
}

hostapd_debug()
{
	local interface=$pc_ip
	local enable=$param1
	local status=`uci get wireless.$interface.hostapd_log_level 2>/dev/null`

	[ "$status" = "dd" ] && status=1 || status=0

	if [ $status != $enable ]; then

		if [ $enable = "1" ]; then
			uci set wireless.$interface.hostapd_log_level=dd
		else
			uci delete wireless.$interface.hostapd_log_level
		fi

		if [ $OS_NAME = "RDKB" ] ;then
			uci commit wireless
			[ $enable = "1" ] && echo "" > /var/log/daemon
		else
			ubus call uci commit '{ "config" : "wireless" }'
			[ $enable = "1" ] && echo "" > /var/log/messages
		fi
		echo -e "####### restart hostapd...please wait #######" > /dev/console
		sleep 5
		ubus call network restart
	else
		echo -e "####### hostapd debug level is already set #######" > /dev/console
	fi

	cat $ETC_CONFIG_WIRELESS_PATH | grep -w -m8 'dd\|radio0\|radio2\|radio4' | grep -v mac
}

wlan_fw_logger_600_B_HW_FIFO_table()
{
	echo -e "                    0           1         2           3                4             5"
	echo -e "1-7-HWCommonFIFO1   HostTXIn    QMRxLists TxLib       RxPP             Band0Activity QMTXMPDUBand1"
	echo -e "2-8-HWCommonFIFO2   HostTXOut   QMRxdata  TxLib       RxPP             Band1Activity QMTXMPDUBand0"
	echo -e "3-9-HWCommonFIFO3   HostRXIn    QMTxLists RXLib       TxAger           QMDMA         QMRXMPDUBand1"
	echo -e "4-10-HWCommonFIFO4  HostRxOut   QMTXData  RXLib       TxAger           Reserved      QMRXMPDUBand0"
	echo -e "5-11-HWBand0FIFO1   Reserved    Security  PreAgg      MUbitMapSelector BSRC          Reserved "
	echo -e "6-12-HWBand0FIFO2   Classifier  Reserved  OTFA        MULocker         Reserved      Reserved "
	echo -e "7-13-HWBand0FIFO3   RXHCDeagg   BFParser  TXHC        Locker           BAA           Reserved"
	echo -e "8-14-HWBand0FIFO4   Coordinator PSSetting TXPDAcc     TxSelBitmap      Delia         Reserved "
	echo -e "9-15-HWBand0FIFO5   AutoResp    Reserved  TFGen       Sequencer        PlanLocker    Reserved"
	echo -e "10-16-HWBand0FIFO6  BSRC        Alpha     FilterDelia TxH              Beacon        RXC"
	echo -e "11-17-HWBand1FIFO1  Reserved    Security  PreAgg      MUbitMapSelector BSRC          Reserved"
	echo -e "12-18-HWBand1FIFO2  Classifier  Reserved  OTFA        MULocker         Reserved      Reserved "
	echo -e "13-19-HWBand1FIFO3  RXHCDeagg   BFParser  TXHC        Locker           BAA           Reserved"
	echo -e "14-20-HWBand1FIFO4  Coordinator PSSetting TXPDAcc     TxSelBitmap      Delia         Reserved"
	echo -e "15-21-HWBand1FIFO5  AutoResp    Reserved  TFGen       Sequencer        PlanLocker    Reserved "
	echo -e "16-22-HWBand1FIFO6  BSRC        Alpha     FilterDelia TxH              Beacon        RXC"
	echo -e ">> 00-CONTINUE"
}

wlan_fw_logger_600_B_BIT_SET_table()
{
	echo -e "BIT  Module"
	echo -e "0  - UMACBand0"
	echo -e "1  - LMACBand0"
	echo -e "2  - HostIfRISC"
	echo -e "3  - RxHandlerBand0"
	echo -e "4  - TxSenderBand0"
	echo -e "5  - PHYGenRiscBand0"
	echo -e "6  - PHYHWBand0"
	echo -e "23 - LMACBand1"
	echo -e "24 - RxHandlerBand1"
	echo -e "25 - TxSenderBand1"
	echo -e "26 - PHYGenRiscBand1"
	echo -e "27 - PHYHWBand1"
	echo -e ">> 00 - CONTINUE"
}

wlan_fw_logger_600_D2_HW_FIFO_table()
{
	echo -e "                    0             1              2             3                4             5"
	echo -e "1-7-HWCommonFIFO1   HostTXIn      QMRxLists      TxLib         RxPP             Band0Activity QMTXMPDUBand1"
	echo -e "2-8-HWCommonFIFO2   HostTXOut     QMRxdata       TxLib         RxPP             Band1Activity QMTXMPDUBand0"
	echo -e "3-9-HWCommonFIFO3   HostRXIn      QMTxLists      RXLib         TxAger           QMDMA         QMRXMPDUBand1"
	echo -e "4-10-HWCommonFIFO4  HostRxOut     QMTXData       RXLib         TxAger           Reserved      QMRXMPDUBand0"
	echo -e "5-11-HWBand1FIFO1   BAAStatCNTR   Security       PreAgg        MUbitMapSelector BSRC          BestRU"
	echo -e "6-12-HWBand1FIFO2   Classifier    Reserved       OTFA          MULocker         Reserved      Reserved "
	echo -e "7-13-HWBand1FIFO3   RXHCDeagg     BFParser       TXHC          Locker           BAA           Reserved"
	echo -e "8-14-HWBand1FIFO4   Coordinator   PSSetting      TXPDAcc       TxSelBitmap      DeliaLegacy   Reserved"
	echo -e "9-15-HWBand1FIFO5   AutoResp      BAAStatCNTR    TFGen         Sequencer        PlanLocker    AlphaFiltersPHY"
	echo -e "10-16-HWBand1FIFO6  BSRC          AlphaFilterMAC DeliaAutofill TxH              Beacon        RXC"
	echo -e "11-17-HWBand0FIFO1  BAAStatCNTR   Security       PreAgg        MUbitMapSelector BSRC          BestRU "
	echo -e "12-18-HWBand0FIFO2  Classifier    Reserved       OTFA          MULocker         Reserved      Reserved "
	echo -e "13-19-HWBand0FIFO3  RXHCDeagg     BFParser       TXHC          Locker           BAA           Reserved"
	echo -e "14-20-HWBand0FIFO4  Coordinator   PSSetting      TXPDAcc       TxSelBitmap      DeliaLegacy   Reserved "
	echo -e "15-21-HWBand0FIFO5  AutoResp      BAAStatCNTR    TFGen         Sequencer        PlanLocker    AlphaFiltersPHY"
	echo -e "16-22-HWBand0FIFO6  BSRC          AlphaFilterMAC DeliaAutofill TxH              Beacon        BestRU"
	echo -e ">> 00-CONTINUE"
}

wlan_fw_logger_600_D2_BIT_SET_table()
{
	echo -e "BIT  Module"
	echo -e "0  - UMACBand0"
	echo -e "1  - LMACBand0"
	echo -e "2  - HostIfRISC"
	echo -e "3  - RxHandlerBand0"
	echo -e "4  - TxSenderBand0"
	echo -e "5  - PHYGenRiscBand0"
	echo -e "6  - PHYHWBand0"
	echo -e "23 - LMACBand1"
	echo -e "24 - RxHandlerBand1"
	echo -e "25 - TxSenderBand1"
	echo -e "26 - PHYGenRiscBand1"
	echo -e "27 - PHYHWBand1"
	echo -e ">> 00 - CONTINUE"
}

wlan_fw_logger_700_HW_FIFO_table()
{
	echo -e "                      0            1            2             3                4             5"
	echo -e "1-7-HWCommonFIFO1     HostTXIn     QMRxLists    TxLib         RxPP             Band0Activity QMTXMPDUBand1"
	echo -e "2-8-HWCommonFIFO2     HostTXOut    QMRxdata     TxLib         RxPP             Band1Activity QMTXMPDUBand0"
	echo -e "3-9-HWCommonFIFO3     HostRXIn     QMTxLists    RXLib         TxAger           QMDMA         QMRXMPDUBand1"
	echo -e "4-10-HWCommonFIFO4    HostRxOut    QMTXData     RXLib         TxAger           HostIfTxINAQM QMRXMPDUBand0"
	echo -e "5-11-HWBand0/2FIFO1   BAAStatsCntr Security     PreAgg        MUbitMapSelector BSRC          BestRU "
	echo -e "6-12-HWBand0/2FIFO2   Classifier   Reserved     OTFA          MULocker         Reserved      Reserved "
	echo -e "7-13-HWBand0/2FIFO3   RXHCDeagg    BFParser     TXHC          Locker           BAA           Reserved"
	echo -e "8-14-HWBand0/2FIFO4   Coordinator  PSSetting    TXPDAcc       TxSelBitmap      DeliaLegacy   Reserved "
	echo -e "9-15-HWBand0/2FIFO5   AutoResp     BAAStatsCntr TFGen         Sequencer        PlanLocker    AlphaPHY"
	echo -e "10-16-HWBand0/2FIFO6  BSRC         Alpha        DeliaAutoFill TxH              Beacon        RXC"
	echo -e "11-17-HWBand1/2FIFO1  BAAStatsCntr Security     PreAgg        MUbitMapSelector BSRC          BestRU"
	echo -e "12-18-HWBand1/2FIFO2  Classifier   Reserved     OTFA          MULocker         Reserved      Reserved "
	echo -e "13-19-HWBand1/2FIFO3  RXHCDeagg    BFParser     TXHC          Locker           BAA           Reserved"
	echo -e "14-20-HWBand1/2FIFO4  Coordinator  PSSetting    TXPDAcc       TxSelBitmap      DeliaLegacy   Reserved"
	echo -e "15-21-HWBand1/2FIFO5  AutoResp     BAAStatsCntr TFGen         Sequencer        PlanLocker    AlphaPHY "
	echo -e "16-22-HWBand1/2FIFO6  BSRC         Alpha        DeliaAutoFill TxH              Beacon        RXC"
	echo -e ">> 00-CONTINUE"
}

wlan_fw_logger_700_BIT_SET_table()
{
	echo -e "BIT  Module"
	echo -e "0  - UMACBand0"
	echo -e "1  - LMACBand0"
	echo -e "2  - HostIfRISC"
	echo -e "3  - RxHandlerBand0"
	echo -e "4  - TxSenderBand0"
	echo -e "5  - PHYGenRiscBand0"
	echo -e "6  - PHYHWBand0"
	echo -e "23 - LMACBand1"
	echo -e "24 - RxHandlerBand1"
	echo -e "25 - TxSenderBand1"
	echo -e "26 - PHYGenRiscBand1"
	echo -e "27 - PHYHWBand1"
	echo -e "28 - LMACBand2"
	echo -e "29 - RxHandlerBand2"
	echo -e "30 - TxSenderBand2"
	echo -e "31 - PHYGenRiscBand2"
	echo -e ">> 00 - CONTINUE"
}

wlan_fw_logger()
{
	local host_pc_serverip="$pc_ip"
	local host_pc_ethaddr="$param1"
	local interface="$param2"
	local um_lm="$param3"
	local hw_modules="$param4"
	local bits_set="$param5"
	local cards_set="0x00"
	local temp="0x01"

	if [ "$pc_ip" = "" ]; then
	clear
	echo -ne "Enter platfrom type:700/D2/B>>" > /dev/console;read plat_type
	while [ 1 ]
	do
			rm -f /tmp/fw_logger > /dev/null 2>&1
			if [ "$plat_type" = "B" ]; then
				echo -e "PLAT TYPE 600-$plat_type"
				wlan_fw_logger_600_B_HW_FIFO_table >>/tmp/fw_logger
				wlan_fw_logger_600_B_HW_FIFO_table
			elif [ "$plat_type" = "D2" ]; then
				echo -e "PLAT TYPE 600-$plat_type"
				wlan_fw_logger_600_D2_HW_FIFO_table >>/tmp/fw_logger
				wlan_fw_logger_600_D2_HW_FIFO_table
			elif [ "$plat_type" = "700" ]; then
				echo -e "PLAT TYPE $plat_type"
				wlan_fw_logger_700_HW_FIFO_table >>/tmp/fw_logger
				wlan_fw_logger_700_HW_FIFO_table
			else
				echo -e "Invalid input\n" > /dev/console
				exit 0
			fi
			echo -e "Enter selection valid raw:1-16 column:0-5 e.g. 6=2:HWBand0FIFO2 & OTFA:" > /dev/console
			echo -ne ">>" > /dev/console;read hw_modules_temp
			[ "$hw_modules_temp" = "00" ] && break
			hw_modules="$hw_modules,$hw_modules_temp"
			line_num=${hw_modules_temp%%=*}
			line_num=$((line_num+1))
			choosed_line=`cat /tmp/fw_logger  | sed -n "${line_num}p"`

			column_select=${hw_modules_temp##*=}
			column_select=$((column_select+2))
			select_summary=`echo $choosed_line | awk -v param="$column_select" '{print $1 " set "  $param}'`
			echo -e "Selected:${select_summary##*-}" >> /tmp/fw_logger_select_sum

			bit_select=${choosed_line%%-H*}
			bit_select=${bit_select##*-}
			while [ "$bit_select" != "0" ]
			do 
				temp=$((temp << 1))
				bit_select=$((bit_select-1))
			done
			cards_set=`printf '0x%X\n' $(( cards_set | temp ))`
			temp="0x01"
			clear
		done
		hw_modules=${hw_modules/,/}

		while [ 1 ]
		do
				if [ "$plat_type" = "B" ]; then
					echo -e "PLAT TYPE 600-$plat_type"
					wlan_fw_logger_600_B_BIT_SET_table
					echo -ne "Module BIT select valid:0-6 & 23-27 >>" > /dev/console;read bit_select
				elif [ "$plat_type" = "D2" ]; then
					echo -e "PLAT TYPE 600-$plat_type"
					wlan_fw_logger_600_D2_BIT_SET_table
					echo -ne "Module BIT select valid:0-6 & 23-27 >>" > /dev/console;read bit_select
				elif [ "$plat_type" = "700" ]; then
					echo -e "PLAT TYPE $plat_type"
					wlan_fw_logger_700_BIT_SET_table
					echo -ne "Module BIT select valid:0-6 & 23-31 >>" > /dev/console;read bit_select
				else
					echo -e "Invalid plat type &plat_type" > /dev/console
					exit 0
				fi
				[ "$bit_select" = "00" ] && break
				bit_select_sum="$bit_select_sum:$bit_select"
				temp="0x01"
				while [ "$bit_select" != "0" ]
				do
					temp=$((temp << 1))
					bit_select=$((bit_select-1))
				done
				cards_set=`printf '0x%X\n' $(( cards_set | temp ))`
				clear
		done

		echo -e "\nUser selection summary:\n"
		echo -e "hw_modules=$hw_modules"
		[ -e /tmp/fw_logger_select_sum ] && cat /tmp/fw_logger_select_sum
		rm -rf /tmp/fw_logger*
		echo -e "\nSelected bit modules:$bit_select_sum\n\nbit_set_summary=$cards_set"
		echo -ne "\npress any key to continue...";read p
		clear

		echo -e "Enter host_pc_serverip:" > /dev/console
		echo -ne ">>" > /dev/console;read host_pc_serverip
		echo -e "Enter  host_pc_ethaddr:" > /dev/console
		echo -ne ">>" > /dev/console;read host_pc_ethaddr
		echo -e "Enter interface 0-wlan0 2-wlan2 4-wlan4:" > /dev/console
		if [ "$plat_type" = "700" ]; then
			echo -e "Note: for WAV700 logger interaface is always wlan0" > /dev/console
		fi
		echo -ne "Enter interface number>>" > /dev/console;read interface
		interface="wlan$interface"

	else

	[ -d /proc/net/mtlk/card0 ] && card0=1 || card0=0
	[ -d /proc/net/mtlk/card1 ] && card1=1 || card1=0
	[ -d /proc/net/mtlk/card2 ] && card2=1 || card2=0 
	[ "$card1" = "1" ] || [ "$card2" = "1" ] && cards_set=0x0000 || cards_set=0x800000
	cards_set=`printf '%X\n'  $(( cards_set | um_lm | bits_set))`
	cards_set="0x$cards_set"

	fi



	local ipaddr=`ifconfig $brlan | grep "inet addr"`
	ipaddr=${ipaddr##*addr:}
	ipaddr=${ipaddr%%' '*}

	local ethaddr="00:00:01:02:03:04"

	local hw_m=""
	if [ "$hw_modules" != "" ]; then
	[ "${hw_modules/16//}" = "$hw_modules" ] && hw_modules="$hw_modules,16=0"
	hw_modules=`echo $hw_modules | tr "," "\n" | sort -n | tr "\n" ","`
	local index
	local oifs="$IFS"
	IFS=,
		local index_arry=1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
		for module in $hw_modules; do
			for index in $index_arry; do
			  index_arry=`echo $index_arry | sed "s/^$index //"`
			  index_arry=${index_arry// /,}
			  if [ "$index" = ${module%%=*} ] ; then
				 hw_m="$hw_m ${module##*=}" && break
			  else
				 hw_m="$hw_m 0"
			  fi
			done
		done
	IFS="$oifs"
	else
		hw_m="0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0"
	fi

	echo -e "ifconfig rtlog0 up\n " >/dev/console
	sleep 1
	ifconfig rtlog0 up
	echo -e "ifconfig rtlog0 hw ether $ethaddr\n " >/dev/console
	sleep 1
	ifconfig rtlog0 hw ether $ethaddr
	echo -e "brctl addif $brlan rtlog0\n " >/dev/console
	sleep 1
	[ "$(brctl show br-lan | grep rtlog0)" = "" ] && brctl addif $brlan rtlog0
	echo -e "LogHwModuleFifo $interface $hw_m\n " >/dev/console
	sleep 1
	echo LogHwModuleFifo $interface $hw_m > /proc/net/mtlk_log/rtlog
	echo -e "LogRemStream 0 $interface 0\n " >/dev/console
	sleep 1
	echo LogRemStream 0 $interface 0 > /proc/net/mtlk_log/rtlog
	echo -e "LogRemStream 1 $interface 0\n " >/dev/console
	sleep 1
	echo LogRemStream 1 $interface 0 > /proc/net/mtlk_log/rtlog

	echo -e "LogAddStream 0 $interface 0 $ipaddr 00:00:00:00:00:10 2008 $host_pc_serverip $host_pc_ethaddr 2009 0 1024 $cards_set \n " >/dev/console
	sleep 1
	echo LogAddStream 0 $interface 0 $ipaddr 00:00:00:00:00:10 2008 $host_pc_serverip $host_pc_ethaddr 2009 0 1024 $cards_set >/proc/net/mtlk_log/rtlog

	echo -e "\n#########################################################" >/dev/console
	echo -e "  if no Errors:Open WireShark IP:$host_pc_serverip" >/dev/console
	echo -e "#########################################################" >/dev/console

}

get_number_of_stats_connected_to()
{	
	interface=$1 #Interface to look for connected stations
	stations=$(dwpal_cli $interface PeerList) 	#Returns a string where the first word indicates the number of stations connected 
	num_stations=$(echo $stations | cut -f 1 -d " ") #Get the first word

	if [ -z "$num_stations" ]	#If no stations are connected the $connected_stations variable is empty, so the script finishes
	then
		echo 0
	else
		echo $num_stations
	fi
}

index_init_value()
{
	interface=$1 #Interface to look for connected stations
	line_num=$(grep -n -m1 MAC /proc/net/mtlk/${interface}/sta_list)
	line_num=${line_num%%:*}

	## STA list starts 2 raws below
	echo $((line_num+2))	#Returns the row of the table where the index should point to
}

station_mac()
{
	index=$1		#Index that points to the stations table
	interface=$2	#Interface to look for connected stations
	sta=`sed -n ${index}p  /proc/net/mtlk/${interface}/sta_list`	
	sta_mac=${sta%% *}
	echo $sta_mac	#MAC of the station pointed by index
}

next(){
	index=$(($1+1))	#Next index of the table
	interface=$2	#Interface to look for connected stations

	init_value=$(index_init_value $interface)
	conected_sta=$(get_number_of_stats_connected_to $interface)
	if [ $index -ge $((init_value+conected_sta)) ]
	then
		echo $init_value
	else
		echo $index
	fi
}

wlan_statistics()
{
	local vap_interface="$pc_ip"
	local display_interval="$param1"
	local sta_mac index sta_count

	[ "$display_interval" = "" ] && display_interval=0

	echo -e "\n########################### INTERFACE STS START ######################\n" >/dev/console
	dwpal_cli $vap_interface PeerList	
	dwpal_cli $vap_interface RecoveryStats
	dwpal_cli $vap_interface HWFlowStatus
	dwpal_cli $vap_interface TR181WLANStat
	dwpal_cli $vap_interface TR181HWStat
	dwpal_cli $vap_interface PTAStats
	dwpal_cli $vap_interface WLANHostIfQos
	dwpal_cli $vap_interface WLANHostIf
	dwpal_cli $vap_interface WLANRxStats
	dwpal_cli $vap_interface WLANBaaStats
	dwpal_cli $vap_interface RadioRxStats
	dwpal_cli $vap_interface RadioBaaStats
	dwpal_cli $vap_interface TsManInitTidGlStats
	dwpal_cli $vap_interface TsManInitStaGlStats
	dwpal_cli $vap_interface TsManRcptTidGlStats
	dwpal_cli $vap_interface TsManRcptStaGlStats
	dwpal_cli $vap_interface RadioLinkAdaptionStats
	dwpal_cli $vap_interface MulticastStats
	dwpal_cli $vap_interface TrainingManStats
	dwpal_cli $vap_interface GroupManStats
	dwpal_cli $vap_interface GeneralStats
	dwpal_cli $vap_interface CurrentChannelStats
	dwpal_cli $vap_interface RadioPhyRxStats
	dwpal_cli $vap_interface DynamicBwStats
	dwpal_cli $vap_interface GetCcaStats
	dwpal_cli $vap_interface LaMuOfdmaCounters
	echo -e "\n########################### INTERFACE STS END ######################\n" >/dev/console

	if [ $display_interval -gt 0 ]
	then
		sleep $((display_interval))
	fi

	index=$(index_init_value $vap_interface)
	sta_count=0

	while [ $(get_number_of_stats_connected_to $vap_interface) -ne 0 ]
	do
		clear

		sta_mac=$(station_mac $index $vap_interface)
		
		if [ $sta_count -ge $(get_number_of_stats_connected_to $vap_interface) ]
		then
			sta_count=0
		fi

		sta_count=$((sta_count+1))

		echo -e "\n########################### STA STS[$sta_count]START :[ $sta_mac ]######################" >/dev/console
		dwpal_cli $vap_interface PeerFlowStatus $sta_mac
		dwpal_cli $vap_interface PeerCapabilities $sta_mac
		dwpal_cli $vap_interface PeerRatesInfo $sta_mac
		dwpal_cli $vap_interface TR181PeerStat $sta_mac
		dwpal_cli $vap_interface PacketErrorRate $sta_mac
		dwpal_cli $vap_interface PeerHostIfQos $sta_mac
		dwpal_cli $vap_interface PeerHostIf $sta_mac
		dwpal_cli $vap_interface PeerRxStats $sta_mac		
		dwpal_cli $vap_interface PeerUlBsrcTidStats $sta_mac
		dwpal_cli $vap_interface PeerBaaStats $sta_mac
		dwpal_cli $vap_interface LinkAdaptionStats $sta_mac
		dwpal_cli $vap_interface PlanManagerStats $sta_mac
		dwpal_cli $vap_interface TwtStats $sta_mac
		dwpal_cli $vap_interface PerClientStats $sta_mac
		dwpal_cli $vap_interface PeerPhyRxStatus $sta_mac
		dwpal_cli $vap_interface PeerInfo $sta_mac
		dwpal_cli $vap_interface PeerDiagResult3 $sta_mac
		dwpal_cli $vap_interface getApAssociatedDeviceDiagnosticResult3 $sta_mac
		dwpal_cli $vap_interface GetPeerMixedStats $sta_mac
		dwpal_cli $vap_interface CSIStats $sta_mac
		dwpal_cli $vap_interface CSICounters $sta_mac
		echo -e "\n########################### STA STS[$sta_count]END :[$sta_mac]######################" >/dev/console

		index=$(next $index $vap_interface)	
		if [ $index -eq $(index_init_value $vap_interface) ] && [ $display_interval -eq 0 ]		
		then
			exit 0				
		fi
		
		sleep $((display_interval))

	done

	echo -e "\n########################### NO STAs CONNECTED ######################\n" >/dev/console
	exit 0
}

wlan_statistics_reset()
{
	local interface="$pc_ip"
	dwpal_cli $interface reset_statistics 0
}


alt_bin()
{
	BINS_FOLDER=/lib/firmware/wave

	# Binaries can be expected in different folders depending on plaform:
	#   - RDKB uses symbolic links from /lib/firmware to where the binaries are located, /lib/firmware/wave
	#   - UGW products (AXP, LGM and Octopus) use directly the binaries located at /lib/firmware
	# Because of this, we look for $BINS_FOLDER and redefine it if it doesn't exist.
	if [ ! -d "$BINS_FOLDER" ]; then
		BINS_FOLDER=/lib/firmware
	fi

	MODE=$1
	KEYWORD=$2

	cd $BINS_FOLDER

	if [ "$MODE" == "enable" ]; then
		
		for bin_file in $(ls *.$KEYWORD.bin 2>/dev/null); do
			
			ORIGINAL=${bin_file/.$KEYWORD/}
			
			echo "Backup $ORIGINAL --> $ORIGINAL.original"
			cp $ORIGINAL $ORIGINAL.original
			
			echo "Update $bin_file --> $ORIGINAL"
			cp $bin_file $ORIGINAL
			
		done
		
		
	else
		for bin_file in $(ls *.original 2>/dev/null); do
			
			echo "Restoring $bin_file --> ${bin_file/.original/}"
			cp $bin_file ${bin_file/.original/}
			
		done
	fi
}


staid_to_mac(){
	if [ $numargs -le 1 ]	#if the command is executed this way: wavToolBox staid2mac, an error is generated
	then
		echo -e "\n########################### EXPECTING VAP INTERFACE ######################\n" >/dev/console
		exit 0
	else
		local vap_interface=$pc_ip
		connected_stations=$(get_number_of_stats_connected_to $vap_interface)
		if [ $connected_stations -eq 0 ]	#If no stations are connected the $connected_stations variable is empty, so the script finishes
		then
			echo -e "\n########################### NO STAs CONNECTED ######################\n" >/dev/console
			exit 0
		fi

		index=$(index_init_value $vap_interface)
		sta_count=0

		if [ $numargs -le 2 ]	#if the command is executed this way: wavToolBox staid2mac wlan0.1, all stations are printed
		then
			printf 'STA ID	MAC ADDRESS\n' 
			printf '---------------------------\n'
			while [ $sta_count -lt $connected_stations ]
			do
				current_aid=$(sed -n ${index}p  /proc/net/mtlk/${vap_interface}/sta_list | awk '{ print $3 }') 	#Extract ID (3rd substring) of each line				
				mac=$(sed -n ${index}p  /proc/net/mtlk/${vap_interface}/sta_list | cut -c1-17)
				let staid=$current_aid-1

				printf '%s	%s\n' "${staid}" "${mac}"

				sta_count=$((sta_count+1))
				index=$(next $index $vap_interface)	
			done
			printf '---------------------------\n'			
		else	#if a station id is specified: wavToolBox staid2mac wlan0.1 0, only that mac is printed
			local sta_id=$param1
			let AID=$sta_id+1

			while [ $sta_count -lt $connected_stations ]
			do
				current_aid=$(sed -n ${index}p  /proc/net/mtlk/${vap_interface}/sta_list | awk '{ print $3 }') 	#Extract ID (3rd substring) of each line		
				if [ $current_aid -eq $AID ]																#Look for the AID that matches
				then
					mac=$(sed -n ${index}p  /proc/net/mtlk/${vap_interface}/sta_list | cut -c1-17) 				#Extract MAC
					echo $mac
					exit 0
				fi
				sta_count=$((sta_count+1))
				index=$(next $index $vap_interface)	
			done

			echo -e "\n########################### NO STA FOUND WITH THAT STAID ######################\n" >/dev/console
		fi

	fi
}


wds_init(){
	local ap="$1"
	local apUpper=$(echo "$ap" | tr '[a-z]' '[A-Z]')
	local apNumber

	if [ $apUpper = "AP1" ]; then
		apNumber=1
	elif  [ $apUpper = "AP2" ]; then
		apNumber=2
	else
		echo -e "\nWRONG AP\n"
		exit 0
    fi

	/etc/init.d/servd stop
	/etc/init.d/servd disable
	uci set wireless.default_radio1.enable_hairpin=''
	uci set wireless.default_radio2.enable_hairpin=''
	uci set wireless.default_radio3.enable_hairpin=''

	ifconfig br-lan 192.168.1.${apNumber} hw ether 02:E0:92:00:0${apNumber}:A1
	ubus call uci set "{ \"config\" : \"wireless\" , \"section\" : \"default_radio1\", \"values\": { \"ssid\" : \"Octopus-700_2G_AP${apNumber}\"}}"
	ubus call uci set "{ \"config\" : \"wireless\" , \"section\" : \"default_radio2\", \"values\": { \"ssid\" : \"Octopus-700_5G_AP${apNumber}\"}}"
	ubus call uci set "{ \"config\" : \"wireless\" , \"section\" : \"default_radio3\", \"values\": { \"ssid\" : \"Octopus-700_6G_AP${apNumber}\"}}"
	ubus call uci set "{ \"config\" : \"wireless\" , \"section\" : \"default_radio1\", \"values\": { \"macaddr\" : \"00:E0:92:00:0${apNumber}:41\"}}"
	ubus call uci set "{ \"config\" : \"wireless\" , \"section\" : \"default_radio2\", \"values\": { \"macaddr\" : \"00:E0:92:00:0${apNumber}:24\"}}"
	ubus call uci set "{ \"config\" : \"wireless\" , \"section\" : \"default_radio3\", \"values\": { \"macaddr\" : \"00:E0:92:00:0${apNumber}:00\"}}"

	ubus call uci set '{ "config" : "wireless" , "section" : "radio0", "values": { "hwmode" : "11bgnaxbe"}}'
	ubus call uci set '{ "config" : "wireless" , "section" : "radio2", "values": { "hwmode" : "11anacaxbe"}}'
	ubus call uci set '{ "config" : "wireless" , "section" : "radio4", "values": { "hwmode" : "11axbe"}}'
	ubus call uci set '{ "config" : "wireless" , "section" : "radio4", "values": { "channel" : "37"}}'
	ubus call uci set '{ "config" : "wireless" , "section" : "radio4", "values": { "htmode" : "EHT320_1"}}'
	ubus call uci set '{ "config" : "wireless" , "section" : "radio0", "values": { "channel" : "1"}}'
	ubus call uci set '{ "config" : "wireless" , "section" : "radio0", "values": { "htmode" : "VHT40"}}'
	ubus call uci set '{ "config" : "wireless" , "section" : "radio2", "values": { "channel" : "36"}}'
	ubus call uci set '{ "config" : "wireless" , "section" : "radio2", "values": { "htmode" : "VHT160"}}'
	ubus call uci commit '{ "config" : "wireless" }'
}

disable_recovery(){
	iw dev wlan0 iwlwav sFWRecovery 0 5 3 1 3600
	iw dev wlan2 iwlwav sFWRecovery 0 5 3 1 3600
	iw dev wlan4 iwlwav sFWRecovery 0 5 3 1 3600
}

disable_atom_wdt(){
	if [ $1 = "192.168.1.1" ]; then
		ubus call system watchdog '{"magicclose": true, "stop": true}'
	fi
}

configure_wds(){
	local apAddress=$1
	local band="$2"
	local crossedApNumber

	if [ $apAddress = "192.168.1.1" ]; then
		crossedApNumber=2
	elif  [ $apAddress = "192.168.1.2" ]; then
		crossedApNumber=1
	else
		echo -e "\nWRONG AP\n"
		exit 0
    fi

	if [ $band = "b0" ]; then
		iw dev wlan0 iwlwav sBfMode 4
		iw dev wlan0.1 iwlwav sBridgeMode 1
		iw dev wlan0.1 iwlwav sAddPeerAP  00:E0:92:00:0${crossedApNumber}:41
		sleep 2
		cat /proc/net/mtlk/wlan0.1/Debug/wds_dbg
    elif  [ $band = "b1" ]; then
		iw dev wlan2 iwlwav sBfMode 4
		iw dev wlan2.2 iwlwav sBridgeMode 1
		iw dev wlan2.2 iwlwav sAddPeerAP  00:E0:92:00:0${crossedApNumber}:24
		sleep 2
		cat /proc/net/mtlk/wlan2.2/Debug/wds_dbg
	elif  [ $band = "b2" ]; then
		iw dev wlan4 iwlwav sBfMode 4
		iw dev wlan4.3 iwlwav sBridgeMode 1
		iw dev wlan4.3 iwlwav sAddPeerAP  00:E0:92:00:0${crossedApNumber}:00
		sleep 2
		cat /proc/net/mtlk/wlan4.3/Debug/wds_dbg
    fi
}

do_ping (){
	if [ $1 = "192.168.1.1" ]; then
		ping 192.168.1.2
	fi
}

wds_bX(){
	local ap_address=$(ip -o -4 addr show scope global | tail -n 1 | awk '{print $4}' | cut -d '/' -f1)
	local band="$1"

	disable_recovery
	disable_atom_wdt $ap_address
	configure_wds $ap_address $band
	do_ping $ap_address
}

to_hex()
{
	# Converts the specified value into an hexadecimal string

	_value=$1

	printf "0x%x" "$_value"
}

sum()
{
	# Sums the specified two values

	_a=$1
	_b=$2

	echo "$((_a + _b))"
}

multiply()
{
	# Multiplies the specified two values

	_a=$1
	_b=$2

	echo "$((_a * _b))"
}

get_pci_base_address()
{
	# Gets the base address of the first PCI board (we asume that in Wave700 there's one board only)
	# To read data from the host, this PCI base address must be added to the target IRAM address.

	echo "0x$(lspci -v 2>/etc/null | grep "Memory at" | head -n 1 | cut -d ' ' -f 3)"
}

translate_pci_address()
{
	# Translates an IRAM address accessible through the PCI to the host point of view (by adding 
	# the PCI base address to the target IRAM address).

	_address=$1

	sum "$(get_pci_base_address)" "$_address"
}

read_iram()
{
	# Reads the word contained in the IRAM position specified by the given address and offset values. 

	_address=$1
	_offset=${2:-0}

	devmem "$(translate_pci_address "$(sum "$_address" "$_offset")")"
}

dump_iram()
{
	# Dumps the contents of the IRAM block identified by the given starting address and length to 
	# the specified file. 

	_filename=$1
	_address=$2
	_length=$3

	io -4 -r -f "$_filename" -l "$_length" "$(translate_pci_address "$_address")"
}

get_cpu_index()
{
	# Gets the index of the specified CPU in [LMAC0, LMAC1, LMAC2, UMAC]. 

	_cpu=$1

	case $_cpu in

	LMAC0) 
		echo 0
	;;

	LMAC1)
		echo 1
	;;

	LMAC2)
		echo 2
	;;

	UMAC)
		echo 3
	;;

	*)
		echo "ERROR: Unknown CPU value '$_cpu'. Valid values are: LMAC0, LMAC1, LMAC2 and UMAC"
		exit 1
	;;

	esac
}

get_cpu_offset()
{
	# Gets the offset required to read from an array of uint32_t with one item per CPU, given such 
	# CPU. That is, returns index_of(cpu) * sizeof(uint32_t)

	_cpu=$1

	_index=$(get_cpu_index "$_cpu")
	_size_of_uint32=4

	multiply "$_index" $_size_of_uint32
}

get_profile_db_base_address()
{
	# Gets the starting address of the .mcount section (i.e. the profile database) for the 
	# specified CPU.

	_cpu=$1

	read_iram 0x60 "$(get_cpu_offset "$_cpu")"
}

get_profile_db_size()
{
	# Gets the size in bytes of the .mcount section (i.e. the profile database) for the specified 
	# CPU.

	_cpu=$1

	read_iram 0x70 "$(get_cpu_offset "$_cpu")"
}

dump_profile_db()
{
	# Dumps the contents of the .mcount section (i.e. the profile database) for the specified CPU
	# into a file.

	_cpu=$1
	_filename=$2

	_result=$(get_cpu_index "$_cpu"); _exit_code=$?
	if [ $_exit_code -ne 0 ]; then
		echo "$_result"
		exit 2
	fi

	_size=$(get_profile_db_size "$_cpu")

	if [ "$_size" = "0x00000000" ] || [ "$_size" = "0xDEADBEEF" ]; then
		echo "ERROR: No profile data available for $_cpu"
		exit 1
	fi

	_address=$(get_profile_db_base_address "$_cpu")

	dump_iram "$_filename" "$_address" "$_size"
}

reset_profile_db()
{
	# Reset the contents of the .mcount section (i.e. the profile database) for the specified CPU
	# (all counters are set to 0).

	_cpu=$1

	_result=$(get_cpu_index "$_cpu"); _exit_code=$?
	if [ $_exit_code -ne 0 ]; then
		echo "$_result"
		exit 2
	fi

	# Not implemented yet
}

case $command in
	"dut_get_cv"|"cv")
		dut_get_cv
	;;
	burn_cal)
		burn_cal_file
	;;
	remove_cal)
		remove_flash_cal_files $2
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
	"wlan_collect_debug_default"|"wcdd")
		${SCRIPTS_PATH}/wave_collect_debug.sh -d $pc_ip
	;;
	"wlan_collect_debug_extension"|"wcde")
		${SCRIPTS_PATH}/wave_collect_debug.sh -e $pc_ip
	;;
	"wlan_collect_debug_assert"|"wcda")
		${SCRIPTS_PATH}/wave_collect_debug.sh -a $pc_ip
	;;
	"wlan_collect_debug_config"|"wcdc")
		${SCRIPTS_PATH}/wave_collect_debug.sh -c
	;;
	"wlan_certification"|"cert")
		wav_certification
	;;
	"wlan_hostapd_debug"|"whd")
		hostapd_debug
	;;
	"wlan_driver_debug"|"wdd")
		driver_debug
	;;
	"wlan_hostapd_driver_debug"|"whdd")
		param1=1
		hostapd_debug
		pc_ip=all
		driver_debug
	;;
	"wlan_fw_logger"|"wfl")
		wlan_fw_logger
	;;
	"wlan_statistics"|"wst")
		wlan_statistics
	;;
	"wlan_statistics_reset"|"wst_rst")
		wlan_statistics_reset
	;;	
	"alt_bin"|"ab")
		alt_bin $2 $3
	;;
	"staid2mac")
		staid_to_mac
	;;
	"wds_init")
		wds_init $2
	;;
	"wdsb0")
		wds_bX "b0"
	;;
	"wdsb1")
		wds_bX "b1"
	;;
	"wdsb2")
		wds_bX "b2"
	;;
	"disableRecovery")
		disable_recovery
	;;
	"dump_profile_db" | "dpdb")
		dump_profile_db "$2" "$3"
	;;
	"reset_profile_db" | "rpdb")
		reset_profile_db "$2"
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
		 "remove_cal                  Removes calibration files according to intrerfaces list\n" \
		 "  Arguments:\n" \
		 "  Argument 1:  The interface name or names to which calibration is removed: wlan0/wlan2/wlan4/all\n" \
		 "               Names can be specified in a comma-separated list, for example: wlan0,wlan2\n" \
		 "wlan_status                  (ws) gives wlan interface main vaps (wlan0.0 and wlan2.0) status\n" \
		 "overlay                      (ov) setting overlay /bin /etc /lib access\n" \
		 "work_mode                    (wm) setting debug work mode - disable un-needed terminal traces \n" \
		 "wlan_version                 (wv) getting wlan version info includes:eeprom,kernel_version,cv \n" \
		 "wlan_factory                 (wf) complete clean-up ( overlay will not be deleted ) \n" \
		 "dut_get_cv                   (cv) returns wave_components.ver content \n" \
		 "wlan_collect_debug           (wcd) wlan collect debug info\n" \
		 "wlan_collect_debug_default   (wcdd) <tftp ip> wlan collect default debug info and optional upload to tftp\n" \
		 "wlan_collect_debug_extension (wcde) <tftp ip> wlan collect extended debug info and optional upload to tftp\n" \
		 "wlan_collect_debug_assert    (wcda) <tftp ip> wlan collect debug info after triggring FW assert and optional upload to tftp\n" \
		 "wlan_collect_debug_config    (wcdc) Only for RDKB:reconfig the syslog to save all future logs to a single file\n" \
		 "wav_certification            (cert) switch to certification mode (change bridge IP and run sigma-start)\n" \
		 "  Arguments:\n" \
		 "      Argument 1: IP address to set (if no IP specified, setting IP to 192.165.100.10)\n" \
		 "		Argument 2: wlan_debug (wdbg) enable the log level 1 in driver and Debug log level in hostapd in sigma-ap.sh" \
		 "                                                      \n" \
		 "wlan_hostapd_debug           (whd) set hostapd debug \n" \
		 "      Argument1: radio0 or radio2 or radio4\n" \
		 "      Argument2: 0 - disable 1 - enable\n" \
		  "      e.g. wavToolBox whd radio0 1 \n" \
		 "wlan_driver_debug            (wdd) set driver debug \n" \
		 "       Argument1: all\n" \
		 "		 Argument2: 0 - disable 1 - enable\n" \
		 "      e.g. wavToolBox wdd all 1 \n" \
		 "wlan_hostapd_driver_debug    (whdd) set hostapd and driver debug\n"\
		 "      Argument1: radio0 or radio2 or radio4\n" \
		 "	    Argument2:  1 - enable\n" \
		 "      e.g. wavToolBox whdd radio0 1 \n" \
		 "wlan_fw_logger               (wfl) enable FW logger (No Argumnet:interactive mode)\n" \
		 "      Arguments: <host PC serverip> <host PC ethaddr> <interface> <upper and lower MAC e.g: 0x3> \n" \
		 "      Optional:[hw_modules e.g HWBand0FIFO2=OTFA: 6=2 ] optional:[hw_modules bits e.g: 0x1000]\n" \
		 "wlan_statistics              (wst) display sta statistics per VAP interface\n" \
		 "      Arguments: <VAP interface> optional:[display interval in sec]\n" \
		 "      		If no interval is sent, the statistics of the connected stations are shown, and the process stops.\n" \
		 "      		If some interval X is sent, the statistics of the connected stations are shown in loop, indefinitely,\n" \
		 "				with X seconds delay between them, until no stations are connected, or the execution is stopped.\n" \
		 "				Example of use: wavToolBox wst wlan0.1\n" \
		 "					   			wavToolBox wst wlan0.1 4\n" \
		 "wlan_statistics_reset        (wst_rst) reset sta statistics per interface\n" \
		 "      Arguments: <interface>\n" \
		 "      		Example of use: wavToolBox wst_rst wlan0\n" \
		 "alt_bin                      (ab) Switch to a special version of the FW binaries\n" \
		 "                                  You will need to reset the AP after running this command in order for the switch to take effect\n" \
		 "      Argument1: enable - switch to the special version of FW binaries specified by Argument2\n" \
		 "                 disable - switch back to the original (non-special) FW binaries.\n" \
		 "      Argument2: Flavor of the special FW binaries to switch to (ex: 'grouping')\n" \
		 "                 It only makes sense when Argument1 is set to 'enable', otherwise it is not needed and can be left blank\n" \
		 "      Examples:\n" \
		 "          - wavToolBox alt_bin enable grouping\n" \
		 "          - wavToolBox disable\n"	\
		 "staid2mac       Obtain the MAC from a StaId connected to certain VAP interface.\n" \
		 "                Arguments: <VAP interface> optional:[StaId]\n" \
		 "       		  If the command is executed without StaId, all the stations of the specified VAP interface are printed.\n" \
		 "					Example of use: wavToolBox staid2mac wlan0.1\n" \
		 "					   				wavToolBox staid2mac wlan0.1 1\n" \
		 "wds_init       Configuration of APs before stablish a WDS connection.\n" \
		 "				 Send always before the wdsbX commands.\n" \
		 "               Arguments: AP1 | AP2 (accepted also in lowercase) \n" \
		 "wdsb0       	WDS Connection between APs in Band 0.\n" \
		 "              Arguments: No arguments needed \n" \
		 "wdsb1       	WDS Connection between APs in Band 1.\n" \
		 "              Arguments: No arguments needed \n" \
		 "wdsb2       	WDS Connection between APs in Band 2.\n" \
		 "              Arguments: No arguments needed \n" \
		 "disableRecovery       	Disable Recovery.\n" \
		 "              			Arguments: No arguments needed \n" \
		 "dump_profile_db       	(dpdb) Dump profile database (.mcount section in IRAM) for the specified CPU to a file.\n" \
		 "              Arguments: <CPU> <fileName> \n" \
		 "              Valid values for CPU are LMAC0, LMAC1, LMAC2 and UMAC.\n" \
		 "reset_profile_db       	(rpdb) Reset profile database (.mcount section in IRAM) for the specified CPU (all counters are set to 0).\n" \
		 "              Arguments: <CPU> \n" \
		 "              Valid values for CPU are LMAC0, LMAC1, LMAC2 and UMAC.\n" 
	;;
esac
