#!/bin/sh /etc/rc.common

USE_PROCD=1
START=98
STOP=05

BIN_DIR=/opt/intel/bin
APP=dsl_cpe_control
DSL_TRANSLATION_SCRIPT=/opt/intel/etc/dsl_uci2json.sh
DSL_DEVICE_INFO=/proc/driver/mei_cpe/devinfo
XDSLRC_SCRIPT=/opt/intel/etc/init.d/xdslrc.sh
DSL_DEFAULT_CONFIG_JSON=/opt/intel/etc/dsl_cpe_control.conf
DSL_CONFIG_JSON=/tmp/dsl/dsl.conf
DSL_STATUS_JSON=/tmp/status/dsl

DSL_FW_DIR=/firmware
DSL_FW_LIB_DIR=/lib/firmware
DSL_FW_ROOTFS_DIR=/opt/intel/firmware
DSL_FW_FILENAME=xcpe_hw.bin
DSL_FW_FILENAME_2P=xcpe_hw_2p.bin

DSL_FIRMWARE=
DSL_FIRMWARE_2P=
DSL_DEBUG_CFG=
DSL_DEVICE_LAYOUT=
NOTIFICATION_SCRIPT=

DSL_STOP_WAIT_MAX_SEC=30

prepare_config_file() {
	if [ ! -f ${DSL_CONFIG_JSON} ]; then
		echo "Preparing tmpfs location \"${DSL_CONFIG_JSON}\"..."

		mkdir $(dirname ${DSL_CONFIG_JSON})
		chmod u=rwx,g=rx,o= $(dirname ${DSL_CONFIG_JSON})

		cp ${DSL_DEFAULT_CONFIG_JSON} ${DSL_CONFIG_JSON}
		chmod u=rw,g=r,o= ${DSL_CONFIG_JSON}

		chown -R :dslgrp $(dirname ${DSL_CONFIG_JSON})
	fi
}

prepare_status_file() {
	if [ ! -f ${DSL_STATUS_JSON} ]; then
		echo "Preparing tmpfs location \"${DSL_STATUS_JSON}\"..."

		mkdir $(dirname ${DSL_STATUS_JSON})
		chmod u=rwx,g=rwx,o=x $(dirname ${DSL_STATUS_JSON})

		chown -R :dslgrp $(dirname ${DSL_STATUS_JSON})
	fi
}

dsl_prepare_daemon_dbg_args() {
	# gets values from DSL config json file
	DebugLevel=$(dsl_get_dbg_value "debug_level")
	DebugLevelsApp=$(dsl_get_dbg_value "debug_levels_app")
	DebugLevelsDrv=$(dsl_get_dbg_value "debug_levels_drv")

	if ${BIN_DIR}/${APP} -h | grep -q '(-D)'; then
		if [ "$DebugLevel" != "" ]; then
			DSL_DEBUG_CFG="-D${DebugLevel}"
			if [ "$DebugLevelsDrv" != "" ]; then
				DSL_DEBUG_CFG="${DSL_DEBUG_CFG} -g${DebugLevelsDrv}"
			fi
			if [ "$DebugLevelsApp" != "" ]; then
				DSL_DEBUG_CFG="${DSL_DEBUG_CFG} -G${DebugLevelsApp}"
			fi
		fi
	fi
}

dsl_prepare_daemon_fw_args() {
	# exports e.g. LinesPerDevice to env
	if [ -e "${DSL_DEVICE_INFO}" ]; then
		eval $( cat ${DSL_DEVICE_INFO} )

		if [ -e ${DSL_FW_DIR}/${DSL_FW_FILENAME} ]; then
			DSL_FIRMWARE="-f ${DSL_FW_DIR}/${DSL_FW_FILENAME}"
		elif [ -e ${DSL_FW_LIB_DIR}/${DSL_FW_FILENAME} ]; then
			DSL_FIRMWARE="-f ${DSL_FW_LIB_DIR}/${DSL_FW_FILENAME}"
		elif [ -e ${DSL_FW_ROOTFS_DIR}/${DSL_FW_FILENAME} ]; then
			DSL_FIRMWARE="-f ${DSL_FW_ROOTFS_DIR}/${DSL_FW_FILENAME}"
		fi

		if [ ${LinesPerDevice} -ge 2 ]; then
			if [ -e ${DSL_FW_DIR}/${DSL_FW_FILENAME_2P} ]; then
				DSL_FIRMWARE_2P="-F ${DSL_FW_DIR}/${DSL_FW_FILENAME_2P}"
			elif [ -e ${DSL_FW_LIB_DIR}/${DSL_FW_FILENAME_2P} ]; then
				DSL_FIRMWARE_2P="-F ${DSL_FW_LIB_DIR}/${DSL_FW_FILENAME_2P}"
			elif [ -e ${DSL_FW_ROOTFS_DIR}/${DSL_FW_FILENAME_2P} ]; then
				DSL_FIRMWARE_2P="-F ${DSL_FW_ROOTFS_DIR}/${DSL_FW_FILENAME_2P}"
			fi
		fi
	else
		echo "ERROR: Missing MEI driver!"
	fi
}

dsl_prepare_daemon_device_layout_args() {
	if [ -e "${DSL_DEVICE_INFO}" ]; then
		eval $( cat ${DSL_DEVICE_INFO} )

		# only config reload can overwrite these settings
		dsl_set_startup_value max_device_number ${MaxDeviceNumber}
		dsl_set_startup_value lines_per_device ${LinesPerDevice}
		dsl_set_startup_value channels_per_line ${ChannelsPerLine}

		DSL_DEVICE_LAYOUT="-V${MaxDeviceNumber} -L${LinesPerDevice} -C${ChannelsPerLine}"
	else
		echo "ERROR: Missing MEI driver!"
	fi
}

dsl_prepare_daemon_xdslrc_script() {
	if [ -e "${XDSLRC_SCRIPT}" ]; then
		NOTIFICATION_SCRIPT="-n ${XDSLRC_SCRIPT}"
	fi
}

dsl_prepare_daemon_args() {
	dsl_prepare_daemon_dbg_args
	dsl_prepare_daemon_fw_args
	dsl_prepare_daemon_device_layout_args
	dsl_prepare_daemon_xdslrc_script
}

dsl_daemon_start() {
	prepare_config_file
	prepare_status_file

	source ${DSL_TRANSLATION_SCRIPT}
	dsl_config_translate
	dsl_prepare_daemon_args

	echo "Starting DSL CPE Control application"
	procd_open_instance
	procd_set_param command ${BIN_DIR}/${APP} \
					${DSL_DEBUG_CFG} \
					-i \
					${DSL_FIRMWARE} ${DSL_FIRMWARE_2P} \
					${DSL_DEVICE_LAYOUT} \
					${NOTIFICATION_SCRIPT}
	procd_set_param stdout 1
	procd_set_param stderr 1
	procd_set_param user dsl
	procd_set_param pseudofs_acl /usr/share/pseudofs_acl/dsl_vrx518_pseudofs_acl.json
	procd_set_param capabilities /usr/share/capabilities/dsl_vrx518_cap.json
	procd_close_instance
}

start_service() {
	dsl_daemon_start
}

restart() {
	echo "Service <${APP}> restarting..."

	AppPID=$(pidof ${APP})
	if [ ! -z "${AppPID}" ]; then
		echo "Sending 'interrupt' to <${APP}> (PID=${AppPID})"
		#just kill userspace app, do not touch driver
		kill -INT ${AppPID}
	fi

	i=0
	while [ $i -lt ${DSL_STOP_WAIT_MAX_SEC} ]
	do
		if ! pidof ${APP}; then
			echo "Service ${APP}> stopped."
			break
		fi

		i=$((i+1))
		echo "Waiting for <${APP}> to stop, ${i} sec..."
		sleep 1
	done

	if [ $i -lt ${DSL_STOP_WAIT_MAX_SEC} ]; then
		echo "Service <${APP}> starting..."
		start;
	else
		echo "Service <${APP}> stop failed in ${DSL_STOP_WAIT_MAX_SEC} seconds! Terminating..."
		#power down!
		stop;
	fi
}

reload_service() {
	source ${DSL_TRANSLATION_SCRIPT}
	dsl_config_translate

	# signal (using SIGHUP) our service (basename of the init.d script)
	procd_send_signal ltq_vrx518_startup.sh
}

service_triggers() {
	procd_add_reload_trigger dsl
}
