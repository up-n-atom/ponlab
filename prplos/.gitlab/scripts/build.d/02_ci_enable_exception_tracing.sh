#!/bin/bash

[ -n "$CI_COMMIT_TAG" ] && exit
[ -z "$CI_BUILD_ENABLE_EXCEPTION_TRACE" ] && exit

name=$(basename "$0")
mkdir -p files/etc/uci-defaults

cat > "files/etc/uci-defaults/$name" <<EOF
trace_file=/proc/sys/debug/exception-trace
[ -f "\$trace_file" ] || exit 0
logger -t "uci-defaults" -p daemon.warn "prplOS: enabling exception tracing"
echo 1 > "\$trace_file"
exit 1 # keep for every boot
EOF
