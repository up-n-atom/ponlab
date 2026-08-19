#!/bin/sh /etc/rc.common
# Copyright (C) 2018 - 2020 Intel Corporation
# This is only for supporting testing and should not be used in production images!

. $IPKG_INSTROOT/lib/pon.sh

GPON_CFG_FILE_PATH=/etc/config/gpon

START=81

start() {
	local tmp
	local vendor_id
	local mib_file
	local curr_vendor_id

	tmp=`fw_printenv -n mib_file 2>&-`
	if [ -n "$tmp" ]; then
		if [ -f "$tmp" ]; then
			mib_file="$tmp"
		elif [ -f "/etc/mibs/$tmp" ]; then
			mib_file="/etc/mibs/$tmp"
		else
			mib_file="/etc/mibs/ponip.ini"
		fi
	else
		config_load omci
		config_get mib_file "default" "mib_file" "/etc/mibs/ponip.ini"
	fi

	vendor_id=`fw_printenv -n vendor_id 2>&-`
	if [ -n "$vendor_id" ]; then
		curr_vendor_id=`uci get omci.default.curr_vendor_id`
		if [ "$vendor_id" != "$curr_vendor_id" ]; then
			sed -r "s/(LQDE|INTL|INTC|MAXL|ALCL|ADTN)/$vendor_id/g" -i $GPON_CFG_FILE_PATH
			sed -r "s/(LQDE|INTL|INTC|MAXL|ALCL|ADTN)/$vendor_id/g" -i $mib_file
			uci set omci.default.curr_vendor_id=$vendor_id
			uci commit
			echo "[sw_image_type_change] Vendor ID has been changed to $vendor_id" > /dev/console
		fi
	fi
}
