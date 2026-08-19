# Get the zone name if interface present in firewall config
# 1: interface
firewall_get_zone() {
	local __tmp="$(ubus call firewalld notify_netifd_update '{ "network" : "'$1'"}')"
	local zone="$(echo $__tmp | jsonfilter -e '@.zone')"
	[ -n "$zone" ] && echo "$zone"
}

# reload fw3 for any configuration change
firewall_reload() {
	"$(ubus call firewalld notify_netifd_update '{ "reload" : "fw3 -q reload"}')"
}    
