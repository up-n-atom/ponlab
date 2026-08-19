#!/bin/sh

. /etc/ugw_notify_defs.sh

# DSL Event handling script - Triggered from DSL CPE control Application
case "$DSL_NOTIFICATION_TYPE" in
	DSL_INTERFACE_STATUS)
		case "$DSL_INTERFACE_STATUS" in
			"UP")
				echo "xDSL Enter SHOWTIME!!"
				echo 1 > /tmp/dsl_status
				if [ "$DSL_TC_LAYER_STATUS" = "ATM" ]; then
					ubus call service event '{ "type": "config.change", "data": { "package": "atm_up" }}'
				fi
			;;
			"DOWN")
				echo "xDSL Leave SHOWTIME!!"
				echo 0 > /tmp/dsl_status
				if [ "$DSL_TC_LAYER_STATUS" = "ATM" ]; then
					ubus call service event '{ "type": "config.change", "data": { "package": "atm_down" }}'
				fi
			;;
		esac
	;;
esac

