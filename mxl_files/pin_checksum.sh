#!/bin/sh
# WPS PIN validation script using hostapd cli interface

pin=$1
pid="$(pidof hostapd)"
if [ -n "$pid" ]; then
	result="$(hostapd_cli WPS_CHECK_PIN any $pin)"
	if !(echo $result | grep -q -i "FAIL"); then
		echo "1"
	fi
fi