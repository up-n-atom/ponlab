#!/bin/sh

#secure upgrade script to perform upgrade operation from procd.
. /etc/profile.d/intel.sh
. /etc/ugw_notify_defs.sh
. /etc/profile.d/ugw_framework_env.sh


UPG_UTILTIY="/opt/intel/usr/sbin/secupg"
IMG_PATH="/tmp/upgrade/firmware.img"

#default upgrade operation later more operations will be added
UPG=1

c_clr="\033[00;00m"
c_gre="\033[32;01m"
c_red="\033[31;01m"

_info()
{
        echo -en "\n${c_gre}$@${c_clr}\n";
}

_hlptxt()
{
        local _opt="$1"
        shift
        echo -en "      $c_red${_opt}${c_clr}  $@\n"
}


_help()
{
        _hlptxt "Secure system upgarde ."
        _hlptxt "Usage: $0 [ options ]";
        _hlptxt "     $0 -u <img path> "
        exit $1;
}

[ -n "$1" ] && {
        case "$1" in
                -h) _help 0;
                ;;
                -u) UPG=1;
                ;;
                *) _help 1;
        esac
}

[ -n "$UPG" ] && {
        logger -t "secupg secupg" "SYSTEM UPGRADE Start"
	$UPG_UTILTIY -u $IMG_PATH
	_ret=$?
	if [ $_ret -eq 0 ] ; then
		logger -t "secupg secupg" "SYSTEM UPGRADE Completed... Going for Reboot "
		sync
		ubus call system reboot &
		exit
	else
		if [ $_ret -eq 1 ] ; then
			logger -t "secupg secupg" "SYSTEM UPGRADE Validation Failed... Abort UPGRADE... "
		else
			logger -t "secupg secupg" "SYSTEM UPGRADE Flash Write Failed... Going for Reboot... "
			ubus call system reboot &
			exit
		fi
	fi
}
