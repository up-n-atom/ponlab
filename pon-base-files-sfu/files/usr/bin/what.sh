#!/bin/sh

for i in $*; do
	#echo "** $i **"
	version=""
	[ ! -L $i ] && version=`strings $i 2>/dev/null | grep -m1 "@(#)" | grep -v "/g" | sed 's/\(.*\)@(#)\(.*\)/\2/g'`
	[ -n "$version" ] && echo "$version"
done
