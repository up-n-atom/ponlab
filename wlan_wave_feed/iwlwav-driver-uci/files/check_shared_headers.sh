#!/bin/sh

PKG_NAME=$1
PKG_VERSION=$2
BUILD_DIR=$3
BUILD_VARIANT=$4
RELATIVE_NLV_PKG_HEADER_PATH=$5
RELATIVE_NL_PKG_HEADER_PATH=$6

DRV_NAME=`grep "PKG_NAME:=" ../iwlwav-driver-uci/Makefile | cut -d '=' -f 2-`
DRV_VERSION=`grep "PKG_VERSION:=" ../iwlwav-driver-uci/Makefile | cut -d '=' -f 2-`

NLV_DRV_HEADER_PATH=$BUILD_DIR/$DRV_NAME-release/$DRV_NAME-$DRV_VERSION/drivers/net/wireless/intel/iwlwav/wireless/driver/shared/vendor_cmds.h
NL_DRV_HEADER_PATH=$BUILD_DIR/$DRV_NAME-release/$DRV_NAME-$DRV_VERSION/include/uapi/linux/nl80211.h

if [[ ! -f "$NLV_DRV_HEADER_PATH" ]]; then
    NLV_DRV_HEADER_PATH=$BUILD_DIR/$DRV_NAME-debug/$DRV_NAME-$DRV_VERSION/drivers/net/wireless/intel/iwlwav/wireless/driver/shared/vendor_cmds.h
    NL_DRV_HEADER_PATH=$BUILD_DIR/$DRV_NAME-debug/$DRV_NAME-$DRV_VERSION/include/uapi/linux/nl80211.h
fi

if [[ "$PKG_NAME" == "afcd" ]]; then
    NLV_PKG_HEADER_PATH=$BUILD_DIR/$PKG_NAME-$PKG_VERSION/$RELATIVE_NLV_PKG_HEADER_PATH
    NL_PKG_HEADER_PATH=$BUILD_DIR/$PKG_NAME-$PKG_VERSION/$RELATIVE_NL_PKG_HEADER_PATH
else
    NLV_PKG_HEADER_PATH=$BUILD_DIR/$PKG_NAME-$BUILD_VARIANT/$PKG_NAME-$PKG_VERSION/$RELATIVE_NLV_PKG_HEADER_PATH
    NL_PKG_HEADER_PATH=$BUILD_DIR/$PKG_NAME-$BUILD_VARIANT/$PKG_NAME-$PKG_VERSION/$RELATIVE_NL_PKG_HEADER_PATH
fi

check_if_files_aligned () {
    ERROR_MSG="    ERROR: Shared headers check failed:"
    [[ ! -f "$1" ]] && { echo "$ERROR_MSG File not found: $1"; exit 1; }
    [[ ! -f "$2" ]] && { echo "$ERROR_MSG File not found: $2"; exit 1; }
    
    CMP_RES=`cmp $1 $2` 
    if [[ $? != 0 ]]; then
        DIFFERS_AT=$(echo "$CMP_RES" | awk -F 'differ: ' '{print $2}')
        echo "$ERROR_MSG $1 differs from $2 at $DIFFERS_AT"
        exit 1
    fi
}

if [[ ! -z "$RELATIVE_NLV_PKG_HEADER_PATH" ]]; then
    check_if_files_aligned $NLV_PKG_HEADER_PATH $NLV_DRV_HEADER_PATH
fi

if [[ ! -z "$RELATIVE_NL_PKG_HEADER_PATH" ]]; then
    check_if_files_aligned $NL_PKG_HEADER_PATH $NL_DRV_HEADER_PATH
fi
