#!/bin/sh
# shellcheck shell=dash

# Secure upgrade script to perform upgrade operation from procd.
################################################################

IMG_PATH="/tmp/upgrade"
IMG_NAME="firmware.img"

#default upgrade operation
UPG=0
REBOOT=1

_info() {
	logger -s -t "upgrade" "$@"
}

_help() {
	printf "Secure system upgrade\n"
	printf "Usage: %s [ options ]\n" "$@"
	printf "       %s -u\n" "$@"
	printf "       %s -r (no reboot)\n" "$@"
	exit "$1"
}

get_inactive_bank() {
	case "$(fw_printenv -n active_bank 2> /dev/null)" in
	A)
		echo B
		;;
	B)
		echo A
		;;
	*)
		_info "not able to detect inactive bank"
		exit 1
		;;
	esac
}

# remove_vol: Removes a volume
# remove_vol: arguments: Volume name
remove_vol() {
	ubirmvol /dev/ubi0 -N "$1"
}

# create_vol: Creates a volume
# create_vol: arguments: Volume name, size(in KiB/MiB)
create_vol() {
	ubimkvol /dev/ubi0 -s "$2" -N "$1"
}

# write_vol: Creates or re-use the provided volume and writes the file.
# write_vol: It also increases the volume size if not sufficient to fit the file.
# write_vol: arguments: Volume name, file
write_vol() {
	[ -n "$1" -a -n "$2" ] && {
		local file_size vol_node
		[ -f "$2" ] && {
			file_size=$(wc -c "$2" | awk '{ print $1 }')
			vol_node=$(grep -w "$1" /sys/class/ubi/ubi0*/name | cut -d'/' -f5)
			[ -n "$vol_node" ] && {
				ubiupdatevol /dev/"$vol_node" "$2" 2>&-
				[ $? -ne 0 ] && {
					remove_vol "$1" 2>&-
					create_vol "$1" "$file_size"
					vol_node=$(grep -w "$1" /sys/class/ubi/ubi0*/name | cut -d'/' -f5)
					ubiupdatevol /dev/"$vol_node" "$2"
					return $?
				}
			} || {
				remove_vol "$1" 2>&-
				create_vol "$1" "$file_size"
				vol_node=$(grep -w "$1" /sys/class/ubi/ubi0*/name | cut -d'/' -f5)
				ubiupdatevol /dev/"$vol_node" "$2"
			}
		}
	}
}

[ -n "$1" ] && {
	case "$1" in
	-h)
		_help 0
		;;
	-u) # upgrade
		UPG=1
		;;
	-r) # upgrade, no reboot
		REBOOT=0
		UPG=1
		;;
	*)
		_help 1
		;;
	esac
}

[ "$UPG" != "0" ] && {
	_info "Install new image"
	cd $IMG_PATH || exit
	if ! pon_img_split -f $IMG_NAME; then
		_info "Splitting Upgrade-File failed"
		exit 1
	fi
	bank=$(get_inactive_bank)
	fw_setenv "img_valid$bank" false
	for f in bootcore kernel rootfs; do
		if [ -f "img-$f" ]; then
			if ! write_vol "$f$bank" "img-$f"; then
				_info "writing $f$bank failed"
				exit 1
			fi
		else
			_info "No file img-$f extracted, skip writing"
		fi
	done
	if [ -f img-version ]; then
		version=$(cat img-version)
		fw_setenv "img_version$bank" "$version"
		_info "Version for bank $bank set to $version"
	fi
	fw_setenv "img_valid$bank" true
	if [ "$REBOOT" != "0" ]; then
		_info "Rebooting system..."
		ubus call system reboot
	fi
}
