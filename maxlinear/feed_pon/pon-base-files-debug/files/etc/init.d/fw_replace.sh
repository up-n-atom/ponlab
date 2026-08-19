#!/bin/sh /etc/rc.common
# Copyright (C) 2017 - 2020 Intel Corporation

. $IPKG_INSTROOT/lib/pon.sh

XGSPON_FW=prx_xpon_fw_b.bin
GPON_FW=prx_gpon_fw_b.bin

START=80

start() {
	local fw_path
	local server_ip
	local err

	fw_path=`fw_printenv -n fw_path 2>&-`
	server_ip=`fw_printenv -n serverip 2>&-`

	if [ -n "$fw_path" -a -n "$server_ip" ]; then
		# lct is disabled by default and enabled later by OMCI
		# so need to manually enable it for the tftp to work
		ifup lct
		if err=`tftp -l /tmp/$XGSPON_FW -r "$fw_path"$XGSPON_FW -g "$server_ip" 2>&1`; then
			mv /tmp/$XGSPON_FW /lib/firmware/$XGSPON_FW
		else
			echo "[fw_replace] XGSPON firmware download failed (used default) - $err" > /dev/console
		fi
		if err=`tftp -l /tmp/$GPON_FW -r "$fw_path"$GPON_FW -g "$server_ip" 2>&1`; then
			mv /tmp/$GPON_FW /lib/firmware/$GPON_FW
		else
			echo "[fw_replace] GPON firmware download failed (used default) - $err" > /dev/console
		fi
	fi
}
