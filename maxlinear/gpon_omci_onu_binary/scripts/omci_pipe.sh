#!/bin/sh

pipe_no=0

# check for lock command:
lock=true
which lock > /dev/null && lock=lock

# use specified pipe no
case "$1" in
    0 | 1 | 2)
        pipe_no=$1
        shift
    ;;
esac

$lock /tmp/.omci_pipe_${pipe_no}
echo "$*" > /tmp/pipe/omci_${pipe_no}_cmd
result=$(cat /tmp/pipe/omci_${pipe_no}_ack)
$lock -u /tmp/.omci_pipe_${pipe_no}

echo "$result"
