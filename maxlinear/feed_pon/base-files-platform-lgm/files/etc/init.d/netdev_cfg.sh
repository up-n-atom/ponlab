#!/bin/sh /etc/rc.common
# Copyright (C) 2019 Intel Corporation

START=25

apply_settings() {
	local name
	local macaddr
	local mtu
	local speed
	local duplex

	config_get name "$1" name none
	[ -d "/sys/class/net/$name" ] || return

	config_get macaddr "$1" macaddr none
	config_get mtu "$1" mtu none
	config_get speed "$1" speed none
	config_get duplex "$1" duplex full

	[ "$macaddr" != "none" ] && ip link set dev $name address $macaddr
	[ "$mtu" != "none" ] && ip link set dev $name mtu $mtu
	[ "$speed" != "none" ] && ethtool -s $name speed $speed duplex $duplex
}

start() {
	config_load network

	config_foreach apply_settings device
}
