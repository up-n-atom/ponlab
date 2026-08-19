#!/bin/sh

ppacmd init
while read line
do
	echo "ppacmd $line"
	ppacmd $line
done <"$1"
