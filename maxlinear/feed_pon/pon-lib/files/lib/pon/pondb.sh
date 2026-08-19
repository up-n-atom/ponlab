#!/bin/sh
# shellcheck shell=dash

OPTIC_DB_LOCATION="/etc/optic-db"
SERDES_DB_LOCATION="/etc/serdes-db"

source /lib/pon.sh

log_console() {
    echo "$@" > /dev/console
}

to_lower() {
    #tr on busybox doesn't support classes like [[:upper:]],
    #so this is done by awk
    awk '{print tolower($0)}'
}

normalize() {
    #The sed will throw out characters not suitable for filenames
    #(including whitespace)
    to_lower | sed -r 's/[^A-Za-z0-9_-]//g'
}

normalize_revision() {
    #same as normalize but will keep the '.' (dot)
    sed -r 's/[^A-Za-z0-9._-]//g'
}

prepend() {
    local prefix="$1"
    shift || return
    sed "s,^,$prefix,"
}

append() {
    local suffix="$1"
    shift || return
    sed "s,$,$suffix,"
}

image_version_get() {
    local ugw_ver

    # prefer the PON version, but if it is not there use the URDK version
    if [ -f /etc/pon.ver ]; then
        image_version=$(cat /etc/pon.ver)
    elif [ -f /etc/version ]; then
        image_version=$(cat /etc/version)
    else
        image_version=""
    fi
}

read_eeprom_real_count() {
    local skip="$1"
    shift || return
    local count="$1"
    shift || return

    dd iflag=skip_bytes,count_bytes if="$EEPROM_PATH" skip="$skip" count="$count" 2> /dev/null
}

read_eeprom_bs1() {
    local skip="$1"
    shift || return
    local count="$1"
    shift || return

    dd bs=1 if="$EEPROM_PATH" skip="$skip" count="$count" 2> /dev/null
}

transceiver_names_get() {
    local vendor_name="$($READ_EEPROM 20 16 | normalize)"
    local vendor_oui="$($READ_EEPROM 37 3 | hexdump -ve '1/1 "%.2x"')"
    local part_number="$($READ_EEPROM 40 16 | normalize)"
    local revision="$($READ_EEPROM 56 4 | normalize_revision)"

    #Examples:
    #wtd-001cad-rtsm166-501-1.0
    #wtd-rtxm166-501

    if [ -n "$vendor_name" ] && [ -n "$part_number" ]; then
        if [ "$vendor_oui" != "000000" ] && [ -n "$revision" ]; then
            echo "$vendor_name-$vendor_oui-$part_number-$revision"
        fi
        if [ -n "$revision" ]; then
            echo "$vendor_name-$part_number-$revision"
        fi
        if [ "$vendor_oui" != "000000"  ]; then
            echo "$vendor_name-$vendor_oui-$part_number"
        fi
        echo "$vendor_name-$part_number"
    fi
}

optic_files_get() {
    local board="$(pon_board_name)"

    if [ -n "$board" ]; then
        # prepend directoy name and append board name
        #Example: /etc/optic-db/superxonltd-sogx2699-psga-urx851-eva
        transceiver_names_get | prepend "$OPTIC_DB_LOCATION/" | append "-$board"
    fi

    # Add fallback variants without -b48 (BM buffer size), -512, -256, and PON instead of Eth.
    local board_truncated="$(echo $board | sed 's/-b\d\+//')"
    board_truncated="$(echo $board_truncated | sed 's/-w5500//')"
    board_truncated="$(echo $board_truncated | sed 's/-512//')"
    board_truncated="$(echo $board_truncated | sed 's/-256//')"
    board_truncated="$(echo $board_truncated | sed 's/-10g-lan//')"
    board_truncated="$(echo $board_truncated | sed 's/-eth/-pon/')"
    if [ -n "$board" -a "$board" != "$board_truncated" ]; then
        transceiver_names_get | prepend "$OPTIC_DB_LOCATION/" | append "-$board_truncated"
    fi

    #We prepend directory name
    #Example: /etc/optic-db/wtd-001cad-rtsm166-501-1.0
    transceiver_names_get | prepend "$OPTIC_DB_LOCATION/"

    echo "$OPTIC_DB_LOCATION/default"
}

serdes_files_get() {
    local board="$(pon_board_name)"

    if [ -n "$board" ]; then
        #We prepend directory name
        #Example: /etc/serdes-db/prx126-sfp-eva-pon-001cad-rtsm166-501-1.0.conf
        transceiver_names_get | prepend "$SERDES_DB_LOCATION/$board-" | append ".conf"
        echo "$SERDES_DB_LOCATION/$board.conf"
    fi

    # Add fallback variants without -b48 (BM buffer size), -512, -256, and PON instead of Eth.
    local board_truncated="$(echo $board | sed 's/-b\d\+//')"
    board_truncated="$(echo $board_truncated | sed 's/-w5500//')"
    board_truncated="$(echo $board_truncated | sed 's/-512//')"
    board_truncated="$(echo $board_truncated | sed 's/-256//')"
    board_truncated="$(echo $board_truncated | sed 's/-10g-lan//')"
    board_truncated="$(echo $board_truncated | sed 's/-eth/-pon/')"
    if [ -n "$board" -a "$board" != "$board_truncated" ]; then
        transceiver_names_get | prepend "$SERDES_DB_LOCATION/$board_truncated-" | append ".conf"
        echo "$SERDES_DB_LOCATION/$board_truncated.conf"
    fi
    echo "$SERDES_DB_LOCATION/default.conf"
}

config_apply() {
    local config_file=""
    local config="$1"
    shift || return

    for filename in "$@"; do
        log_console "[${config}-db] Looking for '$filename' configuration"
        if [ -f "$filename" ]; then
            config_file="$filename"
            break
        fi
    done

    if [ -n "$config_file" ]; then
        uci -m import "$config" < "$config_file" && uci commit "$config" &&
            log_console "[${config}-db] Applied '$config_file' configuration"
    else
        log_console "[${config}-db] Using default configuration"
    fi
}

EEPROM_PATH="$(uci get optic.sfp_eeprom.serial_id)"
[ -z "$EEPROM_PATH" ] && exit 1

image_version_get

# check if dd supports needed flags for optimized read
if dd iflag=skip_bytes,count_bytes if=/dev/zero of=/dev/null count=1 2> /dev/null; then
    READ_EEPROM=read_eeprom_real_count
else
    READ_EEPROM=read_eeprom_bs1
fi

# get first name (most detailed) as reference
transceiver_name=$(transceiver_names_get | head -n1)

optic_version=$(uci -q get optic.common.version)
optic_transceiver=$(uci -q get optic.common.transceiver_name)
[ "$optic_version" != "$image_version" ] && optic_change=1
[ "$optic_transceiver" != "$transceiver_name" ] && optic_change=1

if [ $optic_change ]; then
    config_apply optic $(optic_files_get)
    uci set optic.common.version="$image_version"
    uci set optic.common.transceiver_name="$transceiver_name"
    uci commit optic
fi

# we don't have a serdes config for specific transveivers yet.
# In case we will get this, the handling here needs to be extended like above.
serdes_version=$(uci -q get serdes.generic.version)
[ "$serdes_version" != "$image_version" ] && serdes_change=1

if [ $serdes_change ]; then
    config_apply serdes $(serdes_files_get)
    uci set serdes.generic.version="$image_version"
    uci commit serdes
fi
