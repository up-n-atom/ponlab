#!/bin/sh /etc/rc.common
# Copyright (c) 2021 MaxLinear, Inc.
# Copyright (C) 2013 OpenWrt.org
# Copyright (C) 2013 lantiq.com

START=15

ACTION_QUIET=0
ACTION_SIGNAL=1
ACTION_SHOW=2

ACTION=$ACTION_SHOW

print_action() {
	case "$1" in
	0) echo QUIET ;;
	1) echo SIGNAL ;;
	2) echo SHOW ;;
	esac
}

boot() {
	[ ! -e /sys/kernel/debug/mips/unaligned_action ] && return
	echo "MIPS: set unaligned_action to '$(print_action $ACTION)'" > /dev/console
	echo $ACTION > /sys/kernel/debug/mips/unaligned_action
}
