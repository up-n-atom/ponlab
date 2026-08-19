#!/bin/sh

bindir=/usr/bin
image_version=/etc/image_version
sw_versions=/tmp/version

if [ -e $image_version ]; then
	cat $image_version
fi
[ -e /etc/pon.ver ] && echo "PON subsystem version:" `cat /etc/pon.ver`

if [ ! -e $sw_versions ]; then
	$bindir/what.sh /usr/bin/* /usr/lib/* /lib/modules/*/* | sort -u > $sw_versions
fi
cat $sw_versions
