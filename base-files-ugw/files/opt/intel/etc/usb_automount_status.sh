#!/bin/sh
# mountd Addon Script to display Disc statistics in WebPage
sync_disc_info ()
{
	ls -dalR /mnt/usb/* >/dev/null 2>/dev/null
	mountd_state=`ls /sys/block/ | grep -i sd`;
	df_state=`df`;
	disk_list=`ls /sys/block/ | grep -i sd`
	count_disk_list=`echo $disk_list | wc -w`
}

print_disk_part_info ()
{
	j=0;
	while read infoline; do
	  info_dev=`echo $infoline|awk '{ print $1 }'|cut -d'/' -f3`
	  mount_path="USB-"`ls -lart /mnt/usb/ | grep -i ${info_dev:2} |awk -F'USB-' '{print $2}' |cut -d' ' -f1`
	  info_dev_no=`echo $info_dev|sed 's/[a-z]//g'`
	  info_mounted_on="/$2/`echo "$mountd_state"|grep -w "$2\.name$info_dev_no"|cut -d= -f2`"
	  info_fs="`mount | grep -i USB-${info_dev:2}| awk '{print $5}'`"
	  info_disk_stats="`echo $infoline|awk '{ print $2" "$3" "$4" "$5 }'`"
	  info_str="USB_$1_DISK_INFO_$j $mount_path $info_fs $info_disk_stats"
	  echo $info_str
	  j=`expr $j + 1`
        done
}

print_disk_info ()
{
  echo USB_DISK_TOTAL $count_disk_list
  if [ $count_disk_list -gt 0 ]; then
    i=0
    for dsk in $disk_list; do
        disk_vendor=`cat /sys/block/$dsk/device/vendor|cut -d= -f2`
        disk_model=`cat /sys/block/$dsk/device/model|cut -d= -f2`
        disk_rev=`cat /sys/block/$dsk/device/rev|cut -d= -f2`
        eval echo "USB_$i'_DISK_NAME' $dsk $disk_model\(rev:$disk_rev\) $disk_vendor"
        disk_nod=`ls /sys/block/$dsk/ | grep sd`
        disk_nod=`echo $disk_nod`
        count_disk_part=0
        for part in $disk_nod; do
            count_disk_part=`expr $count_disk_part + 1`
        done
        eval echo USB_$i'_DISK_PART' $count_disk_part
        for part in $disk_nod; do
        echo "$df_state" | grep $part | print_disk_part_info $i $dsk
        done
        i=`expr $i + 1`
    done
  fi
}

disc_umount ()
{
	mountd_state=`cat /var/state/mountd`;
	disk_vendor=`echo "$mountd_state"|grep "$1\.vendor"|cut -d= -f2`
	disk_model=`echo "$mountd_state"|grep "$1\.model"|cut -d= -f2`
	disk_rev=`echo "$mountd_state"|grep "$1\.rev"|cut -d= -f2`
	info_mounted_dirs="`echo "$mountd_state"|grep "$1\.name"|cut -d= -f2`"
	#count_mounted_dirs=`echo $info_mounted_dirs | wc -w`
	for mnt_dir in $info_mounted_dirs; do
		info_update=1
		umount "/mnt/usb/$1/$mnt_dir" >/dev/null 2>/dev/null
		if [ $? -ne 0 ]; then
			ls "/mnt/usb/$1/$mnt_dir/" >/dev/null 2>/dev/null
			if [ $? -ne 0 ]; then
				umnt_success="$umnt_success, $mnt_dir"
			else umnt_busy="$umnt_busy, $mnt_dir"; fi
		else
			rm -f "/mnt/usb/$1/$mnt_dir" 2>/dev/null;
			umnt_success="$umnt_success, $mnt_dir";
		fi
	done
	[ -z "$info_update" ] && exit 0
	if [ -z "$umnt_success" ]; then
		umnt_success="none";
	fi
	if [ -z "$umnt_busy" ]; then
		umnt_busy="none";
		echo "You can now safely remove the disk '"$disk_model"(rev:"$disk_rev")"$disk_vendor" - "$1"'"
	else
		echo "One or more mounted partitions are busy for the disk '"$disk_model"(rev:"$disk_rev")"$disk_vendor" - "$1"'!!"
		echo "Please stop accessing these partitions and try again."
	fi
	umnt_success=`echo $umnt_success | cut -d',' -f2-`
	umnt_busy=`echo $umnt_busy | cut -d',' -f2-`
	echo "Safely un-mounted partitions: $umnt_success"
	echo "Busy partitions: $umnt_busy"
}

[ -z "$1" ] && echo "Usage: $0 <status/umount> [give disc serial id for umount]" && exit 0

if [ "$1" = "status" ]; then
	sync_disc_info;
	print_disk_info > /tmp/usb/usb_info.txt

	exit 0;
elif [ "$1" = "umount" ]; then
	[ -z "$2" ] && exit 0
	disc_umount "$2"
	exit 0
fi

