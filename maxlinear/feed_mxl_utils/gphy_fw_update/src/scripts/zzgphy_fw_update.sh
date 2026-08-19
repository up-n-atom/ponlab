#!/bin/sh /etc/rc.common

USE_PROCD=1
START=99
P34X_LPID=5
PHYAD=0
FW_FILE=mxl_fw_PHY34X_URx6xx_Bxx.bin

start_service() {
	echo "starting gphy_fw_update"
	local INTERFACES=$(ls -d /proc/device-tree/soc/eth*/interface*)
	local GPHYS=$(ls -d /proc/device-tree/soc/gswitch*/gsw_mac*/mdio/ethernet*)
	local int
	local phy_handle
	local handle
	local phy
	local phyad
	local cur_fw_ver
	local file_fw_ver
	local chip
	local count=0

	## Check whether FW file exist in file system
	if [ ! -e /lib/firmware/$FW_FILE ]; then
		echo "GPHY FW does not exist in file system"
		exit 1
	fi

	## If CHIP is not URX641, exit
	chip=$(printf "0x%X" $(($(io -4 0xecc80004 | awk '{printf("0x%s",$2)}') >> 12 & 0xFFFF)))
	echo "chip: $chip"
	if [ $chip != "0xA0" ]; then
		echo "CHIP is not URX641"
		exit 1
	fi
	
	## Get FW version from FW file
	file_fw_ver=$(printf "0x%s" $(hexdump -s 0x11FE -n2 -e '2/1 "%x"' /lib/firmware/$FW_FILE))
	echo "File FW version: $file_fw_ver"

	## Get FW version from gphy. This requires phy address 	
	for int in $INTERFACES; do
		PORT_ID=$(printf "0x%s" $(hexdump -e '4/1 "%x"' -n 4 $int/mxl,dp-port-id))
		if [ $(($PORT_ID)) -eq $(($P34X_LPID)) ] ; then
			phy_handle=$(printf "0x%s" $(hexdump -e '4/1 "%x"' -n 4 $int/phy))
			for phy in $GPHYS; do
				handle=$(printf "0x%s" $(hexdump -e '4/1 "%x"' -n 4 $phy/phandle))
				if [ $(($phy_handle)) -eq $(($handle)) ]; then
					phyad=$(printf "0x%s" $(hexdump -e '4/1 "%x"' -n 4 $phy/reg))
					break
				fi
			done
		fi
	done

	echo "phyad: $phyad"
	cur_fw_ver=$(switch_cli xgmac 2 mdio_rd 0 $phyad 30 | grep "^RD" | awk '{printf("0x%s",$NF)}')
	echo "Current FW version: $cur_fw_ver"

	## Verify whether upgrade is necessary
	if [ $(($cur_fw_ver)) -ge $(($file_fw_ver)) ] ; then
		echo "GPHY firmware version is the same as or newer than FW file. No update reuqired"
		exit 1
	else
		echo "Updating GPHY firmware"
		gphy_fw_update fw /lib/firmware/$FW_FILE 2 $phyad 1 0 $file_fw_ver 0
	fi

	## Verify updated FW version
	count=0
	while [ $(($count)) -le 60 ] ; do
		cur_fw_ver=$(switch_cli xgmac 2 mdio_rd 0 $phyad 30 | grep "^RD" | awk '{printf("0x%s",$NF)}')
		if [ $(($cur_fw_ver)) -eq $(($file_fw_ver)) ] ; then
			echo "GPHY firmware version updated to $cur_fw_ver"
			break;
		else
			count=$(($count + 1))
			sleep 1;
		fi
	done
}
