#!/usr/bin/env bash
export LANG=C
export LC_ALL=C

PRPLOS_DEVELOPMENT_VERSION="${PRPLOS_DEVELOPMENT_VERSION:-3.0.0}"

tagged_version() {
  local tag

  tag=$(git describe --exact-match --tags HEAD 2>/dev/null | sed 's/^prplos-v//')
  [ -n "$tag" ] && echo "$tag" && exit
}

development_version() {
  local git_hash
  
  git_hash=$(git log --pretty=format:%h -n 1 --abbrev=8 2>/dev/null)
  [ -z "$git_hash" ] && git_hash="unknown"
  echo "${PRPLOS_DEVELOPMENT_VERSION}-${git_hash}"
}

[ -n "$TOPDIR" ] && cd "$TOPDIR" || exit
tagged_version
development_version
