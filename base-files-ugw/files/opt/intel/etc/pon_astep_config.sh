#!/bin/sh
# This script is placeholder for the PON config on LGM A-step to address the lower packet size throughput issues.
uc_config()
{
	# Disable IPv4 fallback to uC
	echo idx=1 fallback=0 > /sys/kernel/debug/pp/hal/parser/tables/imem
	# Disable uC for GPIDs
	for i in `seq 16 49`; do echo idx=$i fallback=0 > /sys/kernel/debug/pp/hal/parser/tables/ports; done
	# Enable 1st loop FSQM for US PON
	echo id=48 seg_en=1 hr=0 tr=0 > /sys/kernel/debug/pp/port_mgr/ports/config
}

uc_config
