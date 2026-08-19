#!/bin/sh
# This script is used to switch the main UART between the InterAptiv
# Linux and the PON IP ARC console on the PRX300.

echo "This will switch to the PON console. Press <CR> to continue or <^C> to stop."
read

# parse the output like this: 16180120:  00d8000c
old=$(io -4 0x16180120 |sed -e 's/[0-9]*: *\([0-9a-f]*\)/\1/')

echo "- To switch back, enter this on the FW console: 'iowrite -p 0x9860048 0x$old'"

sleep 1
io -4 -o 0x16180120 0x2000

while true; do
	sleep 60
done
