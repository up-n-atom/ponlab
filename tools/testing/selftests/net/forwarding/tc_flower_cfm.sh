#!/bin/bash
# SPDX-License-Identifier: GPL-2.0

ALL_TESTS="match_cfm_opcode match_cfm_level match_cfm_level_and_opcode"

NUM_NETIFS=2
source tc_common.sh
source lib.sh

tcflags="skip_hw"

h1_create()
{
	simple_if_init $h1 192.0.2.1/24 198.51.100.1/24
}

h1_destroy()
{
	simple_if_fini $h1 192.0.2.1/24 198.51.100.1/24
}

h2_create()
{
	simple_if_init $h2 192.0.2.2/24 198.51.100.2/24
	tc qdisc add dev $h2 clsact
}

h2_destroy()
{
	tc qdisc del dev $h2 clsact
	simple_if_fini $h2 192.0.2.2/24 198.51.100.2/24
}

cfm_mdl_opcode()
{
	local mdl=$1
	local op=$2
	local flags=$3
	local tlv_offset=$4

	printf "%02x %02x %02x %02x"    \
		   $((mdl << 5))             \
		   $((op & 0xff))             \
		   $((flags & 0xff)) \
		   $tlv_offset
}

match_cfm_opcode()
{
	local ethtype="89 02"; readonly ethtype
	RET=0

	tc filter add dev $h2 ingress protocol cfm pref 1 handle 101 \
	   flower cfm op 47 action drop
	tc filter add dev $h2 ingress protocol cfm pref 2 handle 102 \
	   flower cfm op 43 action drop

	pkt="$ethtype $(cfm_mdl_opcode 7 47 0 4)"
	$MZ $h1 -c 1 -p 64 -a $h1mac -b $h2mac "$pkt" -q
	pkt="$ethtype $(cfm_mdl_opcode 6 5 0 4)"
	$MZ $h1 -c 1 -p 64 -a $h1mac -b $h2mac "$pkt" -q

	tc_check_packets "dev $h2 ingress" 101 1
	check_err $? "Did not match on correct opcode"

	tc_check_packets "dev $h2 ingress" 102 0
	check_err $? "Matched on the wrong opcode"

	tc filter del dev $h2 ingress protocol cfm pref 1 handle 101 flower
	tc filter del dev $h2 ingress protocol cfm pref 2 handle 102 flower

	log_test "CFM opcode match test"
}

match_cfm_level()
{
	local ethtype="89 02"; readonly ethtype
	RET=0

	tc filter add dev $h2 ingress protocol cfm pref 1 handle 101 \
	   flower cfm mdl 5 action drop
	tc filter add dev $h2 ingress protocol cfm pref 2 handle 102 \
	   flower cfm mdl 3 action drop

	pkt="$ethtype $(cfm_mdl_opcode 5 42 0 4)"
	$MZ $h1 -c 1 -p 64 -a $h1mac -b $h2mac "$pkt" -q
	pkt="$ethtype $(cfm_mdl_opcode 6 1 0 4)"
	$MZ $h1 -c 1 -p 64 -a $h1mac -b $h2mac "$pkt" -q

	tc_check_packets "dev $h2 ingress" 101 1
	check_err $? "Did not match on correct level"

	tc_check_packets "dev $h2 ingress" 102 0
	check_err $? "Matched on the wrong level"

	tc filter del dev $h2 ingress protocol cfm pref 1 handle 101 flower
	tc filter del dev $h2 ingress protocol cfm pref 2 handle 102 flower

	log_test "CFM level match test"
}

match_cfm_level_and_opcode()
{
	local ethtype="89 02"; readonly ethtype
	RET=0

	tc filter add dev $h2 ingress protocol cfm pref 1 handle 101 \
	   flower cfm mdl 5 op 41 action drop
	tc filter add dev $h2 ingress protocol cfm pref 2 handle 102 \
	   flower cfm mdl 7 op 42 action drop

	pkt="$ethtype $(cfm_mdl_opcode 5 41 0 4)"
	$MZ $h1 -c 1 -p 64 -a $h1mac -b $h2mac "$pkt" -q
	pkt="$ethtype $(cfm_mdl_opcode 7 3 0 4)"
	$MZ $h1 -c 1 -p 64 -a $h1mac -b $h2mac "$pkt" -q
	pkt="$ethtype $(cfm_mdl_opcode 3 42 0 4)"
	$MZ $h1 -c 1 -p 64 -a $h1mac -b $h2mac "$pkt" -q

	tc_check_packets "dev $h2 ingress" 101 1
	check_err $? "Did not match on correct level and opcode"
	tc_check_packets "dev $h2 ingress" 102 0
	check_err $? "Matched on the wrong level and opcode"

	tc filter del dev $h2 ingress protocol cfm pref 1 handle 101 flower
	tc filter del dev $h2 ingress protocol cfm pref 2 handle 102 flower

	log_test "CFM opcode and level match test"
}

setup_prepare()
{
	h1=${NETIFS[p1]}
	h2=${NETIFS[p2]}
	h1mac=$(mac_get $h1)
	h2mac=$(mac_get $h2)

	vrf_prepare

	h1_create
	h2_create
}

cleanup()
{
	pre_cleanup

	h2_destroy
	h1_destroy

	vrf_cleanup
}

trap cleanup EXIT

setup_prepare
setup_wait

tests_run

tc_offload_check
if [[ $? -ne 0 ]]; then
	log_info "Could not test offloaded functionality"
else
	tcflags="skip_sw"
	tests_run
fi

exit $EXIT_STATUS
