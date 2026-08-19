#!/bin/sh

# A wrapper to execute only one instance of the SCRIPT using a file lock
SCRIPT="$(dirname $0)/wave_factory_reset_ex.sh"
OWNPID=$$

LOCKFILE="/tmp/$(basename $SCRIPT |sed -e 's|[.]|-|g').lock"

# ash specific: flock does not work. Use lock file polling instead.

if [ -f $LOCKFILE ]; then
	echo "$0: The script is already running. Waiting for its completion..."
	let cntr=0
	while [ $cntr -lt 30 ]; do
		sleep 1
		[ ! -f $LOCKFILE ] && break
		let cntr++
	done
	echo "$0: The wait is over."
	if [ -f $LOCKFILE ]; then
		echo "$0: The script is still running, force terminate it."
		pids=$(ps |grep $(basename $0) |grep -v $OWNPID |awk '{ print $1 }')
		kill $pids &>/dev/null
		kill -KILL $pids &>/dev/null
		sleep 1
	fi
fi

touch $LOCKFILE
#flock -x $LOCKFILE $SCRIPT $@
$SCRIPT "$@"
rm -f $LOCKFILE
