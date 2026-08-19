#!/bin/sh

[ -x /usr/sbin/xl2tpd ] || exit 0

[ -n "$INCLUDE_ONLY" ] || {
	. /lib/functions.sh
	. ../netifd-proto.sh
	init_proto "$@"
}

proto_l2tp_init_config() {
	proto_config_add_string "username"
	proto_config_add_string "password"
	proto_config_add_string "keepalive"
	proto_config_add_string "pppd_options"
	proto_config_add_boolean "ipv6"
	proto_config_add_int "mtu"
	proto_config_add_int "checkup_interval"
	proto_config_add_string "server"
	proto_config_add_string "defaultroute"
	proto_config_add_int "mru"
	proto_config_add_int "lcp_echo_interval"
	proto_config_add_int "lcp_echo_failure"
	proto_config_add_int "connect_delay"
	proto_config_add_int "idle"
	proto_config_add_string "debug"
	proto_config_add_string "refuse_authentication"
	proto_config_add_string "PPP_debug"
	proto_config_add_string "server_name"
	proto_config_add_string "redial"
	proto_config_add_string "redial_timeout"
	available=1
	no_device=1
	no_proto_task=1
	teardown_on_l3_link_down=1
}

proto_l2tp_setup() {
	local interface="$1"
	local optfile="/tmp/l2tp/options.${interface}"
	local conffile="/tmp/l2tp/xl2tpd.conf"
	local ip serv_addr server host
	local refuse_authentication PPP_debug server_name

	json_get_var server server
	json_get_vars refuse_authentication PPP_debug server_name
	host="${server%:*}"
	for ip in $(resolveip -t 5 "$host"); do
		( proto_add_host_dependency "$interface" "$ip" )
		serv_addr=1
	done
	[ -n "$serv_addr" ] || {
		echo "Could not resolve server address" >&2
		sleep 5
		proto_setup_failed "$interface"
		exit 1
	}

	mkdir -p /tmp/l2tp
	cat <<EOF >"$conffile"
[global]
[lac $server_name]
lns = $server
refuse authentication = $refuse_authentication
ppp debug = $PPP_debug
pppoptfile = $optfile
EOF
	# Start and wait for xl2tpd
	if [ ! -p /var/run/xl2tpd/l2tp-control -o -z "$(pidof xl2tpd)" ]; then
		/usr/sbin/xl2tpd -l -c $conffile -p /var/run/xl2tpd.pid

		local wait_timeout=0
		while [ ! -p /var/run/xl2tpd/l2tp-control ]; do
			wait_timeout=$(($wait_timeout + 1))
			[ "$wait_timeout" -gt 5 ] && {
				echo "Cannot find xl2tpd control file." >&2
				proto_setup_failed "$interface"
				exit 1
			}
			sleep 1
		done
	fi

	local ipv6 keepalive username password pppd_options mtu
	local defaultroute mru lcp_echo_interval
	local lcp_echo_failure connect_delay idle
	local debug
	local redial redial_timeout
	json_get_vars ipv6 keepalive username password pppd_options mtu
	json_get_vars defaultroute mru lcp_echo_interval
	json_get_vars lcp_echo_failure connect_delay idle
	json_get_vars debug
	json_get_vars redial redial_timeout
	[ "$ipv6" = 1 ] || ipv6=""

	local interval="${keepalive##*[, ]}"
	[ "$interval" != "$keepalive" ] || interval=5

	keepalive="${keepalive:+lcp-echo-interval $interval lcp-echo-failure ${keepalive%%[, ]*}}"
	username="${username:+user \"$username\" password \"$password\"}"

	mkdir -p /tmp/l2tp
	cat <<EOF >"$optfile"
$pppd_options
idle $idle
mtu $mtu
mru $mru
nodefaultroute
$debug
lcp-echo-interval $lcp_echo_interval
lcp-echo-failure $lcp_echo_failure
connect-delay $connect_delay
EOF

	if [ "$redial" = "yes" ]; then
		xl2tpd-control add-lac l2tp-${interface} pppoptfile=${optfile} lns=${server} redial=$redial redial timeout=$redial_timeout || {
			echo "xl2tpd-control: Add l2tp-$interface failed" >&2
			proto_setup_failed "$interface"
			exit 1
		}
	else
		xl2tpd-control add-lac l2tp-${interface} pppoptfile=${optfile} lns=${server} || {
			echo "xl2tpd-control: Add l2tp-$interface failed" >&2
			proto_setup_failed "$interface"
			exit 1
		}
	fi
	xl2tpd-control connect-lac l2tp-${interface} || {
		echo "xl2tpd-control: Connect l2tp-$interface failed" >&2
		proto_setup_failed "$interface"
		exit 1
	}
}

proto_l2tp_teardown() {
	local interface="$1"
	local optfile="/tmp/l2tp/options.${interface}"
	local conffile="/tmp/l2tp/xl2tpd.conf"

	rm -f ${optfile}
	if [ -p /var/run/xl2tpd/l2tp-control ]; then
		xl2tpd-control remove-lac l2tp-${interface} || {
			echo "xl2tpd-control: Remove l2tp-$interface failed" >&2
		}
	fi
	# Wait for interface to go down
        while [ -d /sys/class/net/l2tp-${interface} ]; do
		sleep 1
	done
	killall xl2tpd
	rm -rf $conffile
	
}

[ -n "$INCLUDE_ONLY" ] || {
	add_protocol l2tp
}
