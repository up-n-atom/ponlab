#!/bin/sh

# A wrapper to execute only one instance of the SCRIPT using a file lock
SCRIPT="$(dirname $0)/wave_factory_reset_ex.sh"

LOCKFILE="/tmp/$(basename $SCRIPT |sed -e 's|[.]|-|g').lock"

touch $LOCKFILE
flock -x $LOCKFILE $SCRIPT "$@"
