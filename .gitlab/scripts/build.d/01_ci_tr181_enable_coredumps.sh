#!/bin/bash

[ -n "$CI_COMMIT_TAG" ] && exit
[ -z "$CI_BUILD_ENABLE_COREDUMPS" ] && exit

name="$(basename $0)"
mkdir -p files/etc/env.d

cat > "files/etc/env.d/$name" <<EOF
logger -t "environment" -p daemon.warn "prplOS: enabling coredumps"
printf 'export ULIMIT_CONFIGURATION=unlimited\n' >> /etc/environment
EOF
