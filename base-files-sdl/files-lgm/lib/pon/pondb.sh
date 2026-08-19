#!/bin/sh
# shellcheck shell=dash

# In contrast to the original file, Ubus calls
# are used here to read and write UCI config files.
# Furthermore, a synchronization file /tmp/pon_uci_cfg
# is created at the end of the script.

OPTIC_DB_LOCATION="/etc/optic-db"
SERDES_DB_LOCATION="/etc/serdes-db"

source /lib/pon.sh

log_console() {
    echo "$@" >&1
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

pon_ubus_uci_get() {
    local val

    val="$(ubus call uci get '{"config":"'$1'", "section":"'$2'", "option":"'$3'"}')"
    echo "$val" | grep value | cut -f2 -d: | cut -f2 -d\"
}

pon_ubus_uci_set() {
    ubus call uci set '{"config":"'$1'", "section":"'$2'", "values": {"'$3'":"'$4'"}}'
}

pon_ubus_uci_commit() {
    ubus call uci commit '{ "config" : "'$1'" }'
}

ubus_uci_file_merge() {
    local file=$1
    local db_file=$2
    local line
    local sec_name=""
    local new_sec_name
    local sec_type
    local option
    local val
    local sec_exists

    while read line; do
        new_sec_name=$(echo $line | awk '/config/ {print $3}' | tr -d "\'")
        if [ -n "$new_sec_name" ]; then
            sec_name="$new_sec_name"
            sec_type=$(echo $line | awk '/config/ {print $2}' | tr -d "\'")
            # checking whether section is already existing
            sec_exists=$(ubus call uci get '{ "config" : "'$file'", "section" : "'$sec_name'" }' 2> /dev/null)
            if [ -z "$sec_exists" ]; then
               ubus call uci add '{ "config": "'$file'", "type": "'$sec_type'", "name": "'$sec_name'" }' 1> /dev/null
            fi
        else
            option=$(echo $line | awk '/option/ {print $2}' | tr -d "\'")
            if [ -n "$sec_name" ] && [ -n "option" ]; then
                val=$(echo $line | awk ' {print $3}' | tr -d "\'")
                pon_ubus_uci_set "$file" "$sec_name" "$option" "$val"
            fi
        fi
    done < $db_file

    pon_ubus_uci_commit "$file"
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

read_eeprom() {
    local skip="$1"
    shift || return
    local count="$1"
    shift || return

    dd iflag=skip_bytes,count_bytes if="$EEPROM_PATH" skip="$skip" count="$count" 2> /dev/null
}

transceiver_names_get() {
    local vendor_name="$(read_eeprom 20 16 | normalize)"
    local vendor_oui="$(read_eeprom 37 3 | hexdump -ve '1/1 "%.2x"')"
    local part_number="$(read_eeprom 40 16 | normalize)"
    local revision="$(read_eeprom 56 4 | normalize_revision)"

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
        ubus_uci_file_merge "$config" "$config_file" &&
            log_console "[${config}-db] Applied '$config_file' configuration"
    else
        log_console "[${config}-db] Using default configuration"
    fi
}

EEPROM_PATH="$(pon_ubus_uci_get "optic" "sfp_eeprom" "serial_id")"
# to unblock omcid.sh even in case of missing eeprom path
if [ -z "$EEPROM_PATH" ]; then
   touch /tmp/pon_uci_cfg
   exit 1
fi

image_version_get
# get first name (most detailed) as reference
transceiver_name=$(transceiver_names_get | head -n1)

optic_version="$(pon_ubus_uci_get "optic" "common" "version")"
optic_transceiver="$(pon_ubus_uci_get "optic" "common" "transceiver_name")"
[ "$optic_version" != "$image_version" ] && optic_change=1
[ "$optic_transceiver" != "$transceiver_name" ] && optic_change=1

if [ $optic_change ]; then
    config_apply optic $(optic_files_get)
    pon_ubus_uci_set "optic" "common" "version" "$image_version"
    pon_ubus_uci_set "optic" "common" "transceiver_name" "$transceiver_name"
    pon_ubus_uci_commit "optic"
fi

# we don't have a serdes config for specific transveivers yet.
# In case we will get this, the handling here needs to be extended like above.
serdes_version="$(pon_ubus_uci_get "serdes" "generic" "version")"
[ "$serdes_version" != "$image_version" ] && serdes_change=1

if [ $serdes_change ]; then
    config_apply serdes $(serdes_files_get)
    pon_ubus_uci_set "serdes" "generic" "version" "$image_version"
    pon_ubus_uci_commit "serdes"
fi

# to sync omcid.sh execution with UCI config file update
touch /tmp/pon_uci_cfg

