#!/bin/bash
# Script to extract kernel binary vmlinux .text section 
# address, and storing it as a key-pair value in a file which 
# will be used for creating a kernel metadata image.
# Usage: ./kernel_metadata.sh <path to readelf binary> <keypair file>
text_address=$(readelf -S -W $1.elf | grep -w ".text" |awk -F ' ' '{print $5}')
if [ -z $text_address ] 
then
	exit -1;
fi
text_add_32=${text_address:7,9}
echo "md_ro_start=${text_add_32}" > $1.metadata
text_size=$(readelf -S -W $1.elf | grep -w ".text" |awk -F ' ' '{print $7}')
if [ -z $text_size ] 
then
	exit -1;
fi
echo $text_add_32  $text_size
text_end_add_32=$(( 0x$text_add_32 + 0x$text_size ))
text_end_add_32=$( printf %X $text_end_add_32)
echo $text_end_add_32
echo "md_ro_end=${text_end_add_32}" >> $1.metadata


