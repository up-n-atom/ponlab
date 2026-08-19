#!/bin/sh
# shellcheck shell=dash

# dump all needed vars as json object
_get_uboot_vars() {
	vars="active_bank img_validA img_validB img_versionA img_versionB commit_bank img_activate"

	printf "{"
	for v in $vars; do
		val=$(fw_printenv -n "$v" 2> /dev/null)
		printf '"%s":"%s"' "$v" "$val"
		[ "$v" != "img_activate" ] && printf ","
	done
	printf "}\n"
}

# wrapper to ensure that result is dumped to stdout in one call
get_uboot_vars() {
	uboot_vars=$(_get_uboot_vars)
	echo "$uboot_vars"
}

# Usage: set_img_valid A|B (1|0)
set_img_valid() {
	case "$1" in
	A | B)
		case "$2" in
		false | 0)
			value=false
			;;
		*)
			value=true
			;;
		esac
		fw_setenv "img_valid$1" $value || return 1
		return 0
		;;
	esac
	return 1
}

# Usage: set_img_version A|B "version"
set_img_version() {
	case "$1" in
	A | B)
		fw_setenv "img_version$1" "$2" || return 1
		return 0
		;;
	esac
	return 1
}

# Usage: activate_img A|B
activate_img() {
	case "$1" in
	A | B)
		fw_setenv img_activate "$1" || return 1
		return 0
		;;
	esac
	return 1

}

# Usage: set_commit_bank A|B
set_commit_bank() {
	case "$1" in
	A | B)
		fw_setenv commit_bank "$1" || return 1
		return 0
		;;
	esac
	return 1
}

[ -n "$1" ] && {
	func="$1"
	shift
	$func $@ || >&-
}
