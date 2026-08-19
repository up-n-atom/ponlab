#!/bin/sh

#set -x

if pidof -o $$ "wifi_nvram_monitor.sh" > /dev/null; then
	exit 0
fi

HIGH_THR_REACHED=0
LOW_THR_REACHED=1

if [ -z "$1" ]
then
	THR=95
else
	THR=$1
fi

while true
do
	df_out=$(/bin/df -h | grep "/nvram$")
	percent=$(echo $df_out | sed -re 's/.*\s+([0-9]+)%.*/\1/')
	if [ $HIGH_THR_REACHED -eq 0 -a $percent -ge $THR ]
	then
		echo "####NVRAM CRITICAL above ${THR}%, used space: ${percent}% ####" > /dev/console
		echo "####NVRAM Files in /nvram greater than 500K: ####" > /dev/console
		find /nvram/ -size +500k -exec du -sh {} \; > /dev/console
		#du -a -h -d 2 /nvram > /dev/console
		HIGH_THR_REACHED=1
		LOW_THR_REACHED=0
	elif [ $LOW_THR_REACHED -eq 0 -a $percent -lt $THR ]
	then
		echo "####NVRAM back below ${THR}%, used space: ${percent}% ####" > /dev/console
		LOW_THR_REACHED=1
		HIGH_THR_REACHED=0
	fi
	usleep 100000
done
