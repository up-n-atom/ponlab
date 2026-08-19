#!/bin/sh
>/tmp/usb/usb_connected_info.txt
for i in /sys/bus/usb/drivers/usb/[0-9]* 
do
	version=`cat $i/version | tr -d " "`
	Class=`cat $i/bDeviceClass | tr -d " "`
	SubClass=`cat $i/bDeviceSubClass | tr -d " "`
	Protocol=`cat $i/bDeviceProtocol | tr -d " "`
	ProductId=`cat $i/idProduct | tr -d " "`
	VendorId=`cat $i/idVendor | tr -d " "`
	Manufacturer=`cat $i/manufacturer | tr -d " "`
	ProductClass=`cat $i/product | tr -d " "`
	Serial=`cat $i/serial | tr -d " "`
	eval echo "$version" "$Class" "$SubClass" "$Protocol" "$ProductId" "$VendorId" "$Manufacturer" "$ProductClass" "$Serial" >> /tmp/usb/usb_connected_info.txt
done
