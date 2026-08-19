# Process this file with automake to produce Makefile.in

lib_LTLIBRARIES = libponnet.la

libponnet_la_SOURCES = \
   ../include/pon_net_register.h \
   me/pon_net_dot1p_mapper.c \
   me/pon_net_ext_vlan.c \
   me/pon_net_ext_vlan.h \
   me/pon_net_gal_eth_profile.c \
   me/pon_net_gem_interworking_tp.c \
   me/pon_net_gem_interworking_tp.h \
   me/pon_net_gem_port_network_ctp.c \
   me/pon_net_gem_port_network_ctp.h \
   me/pon_net_ip_host.c \
   me/pon_net_mac_bridge_port_bridge_table_data.c \
   me/pon_net_mac_bridge_port_config_data.c \
   me/pon_net_mac_bridge_port_config_data.h \
   me/pon_net_mac_bridge_port_filter_preassign_table.c \
   me/pon_net_mac_bridge_port_filter_preassign_table.h \
   me/pon_net_mac_bridge_service_profile.c \
   me/pon_net_multicast_gem_interworking_tp.c \
   me/pon_net_multicast_gem_interworking_tp.h \
   me/pon_net_multicast_operations_profile.c \
   me/pon_net_onu2_g.c \
   me/pon_net_onu2_g.h \
   me/pon_net_onu_g.c \
   me/pon_net_onu_g.h \
   me/pon_net_pptp_ethernet_uni.c \
   me/pon_net_pptp_lct_uni.c \
   me/pon_net_priority_queue.c \
   me/pon_net_priority_queue.h \
   me/pon_net_tcont.c \
   me/pon_net_tcont.h \
   me/pon_net_traffic_descriptor.c \
   me/pon_net_traffic_descriptor.h \
   me/pon_net_traffic_scheduler.c \
   me/pon_net_virtual_ethernet_interface_point.c \
   me/pon_net_vlan_tagging_filter_data.c \
   me/pon_net_vlan_tagging_filter_data.h \
   me/pon_net_vlan_tagging_operation_config_data.c \
   me/pon_net_vendor_emop.c \
   me/pon_net_ethernet_pmhd.c \
   pon_net_common.c \
   pon_net_common.h \
   pon_net_debug.c \
   pon_net_debug.h \
   pon_net_dev_db.c \
   pon_net_dev_db.h \
   pon_net_dev_defs.c \
   pon_net_dev_defs.h \
   pon_net_ethtool.c \
   pon_net_ethtool.h \
   pon_net_event.c \
   pon_net_event.h \
   pon_net_ext_vlan_table.h \
   pon_net_ext_vlan_filter_def.c \
   pon_net_ext_vlan_filter_def.h \
   pon_net_ext_vlan_rules.c \
   pon_net_ext_vlan_rules.h \
   pon_net_extern.c \
   pon_net_extern.h \
   pon_net_init.c \
   pon_net_init.h \
   pon_net_mcc_core.c \
   pon_net_mcc_core.h \
   pon_net_vlan_aware_bridging.c \
   pon_net_vlan_aware_bridging.h \
   pon_net_vlan_flow.c \
   pon_net_vlan_flow.h \
   pon_net_netlink.c \
   pon_net_netlink.h \
   pon_net_netlink_debug.c \
   pon_net_netlink_debug.h \
   pon_net_rtnl.c \
   pon_net_rtnl.h \
   pon_net_mapper_table.c \
   pon_net_mapper_table.h \
   pon_net_me_list.c \
   pon_net_me_list.h \
   pon_net_omci_msg.c \
   pon_net_os.h \
   pon_net_pool.h \
   pon_net_pool.c \
   pon_net_qdisc.h \
   pon_net_qdisc.c \
   pon_net_qdisc_build.h \
   pon_net_qdisc_build.c \
   pon_net_register.c \
   pon_net_tc_filter.c \
   pon_net_tc_filter.h \
   pon_net_uni.c \
   pon_net_uni.h

AM_CFLAGS = -DLINUX -D__LINUX__ \
	-D_GNU_SOURCE \
	-I@top_srcdir@/include/ \
	@LIBNL_INCLUDE_PATH@ \
	@PON_ADAPTER_INCLUDE_PATH@ \
	-Wall -Wextra \
	-Wconversion \
	-Wformat-security \
	-Wmissing-noreturn \
	-Wmissing-declarations \
	-Wredundant-decls \
	-Wmissing-prototypes \
	-Wnested-externs \
	-Wshadow \
	-Winit-self \
	-Wundef \
	-Waggregate-return

AM_LDFLAGS = -L@builddir@ \
	     @LIBNL_LIBRARY_PATH@ \
	     @PON_ADAPTER_LIBRARY_PATH@ \
	    -Wl,--no-undefined

libponnet_la_CFLAGS = $(AM_CFLAGS) -DINCLUDE_DEBUG_SUPPORT

libponnet_la_LDFLAGS = $(AM_LDFLAGS)

if TESTS_BUILD
libponnet_la_CFLAGS += -DUNIT_TESTS
libponnet_la_LDFLAGS += -Wl,--unresolved-symbols=ignore-in-object-files \
						-Wl,--allow-shlib-undefined
endif

libponnet_la_LIBADD = -lnl-3 -lnl-route-3 -lm -ladapter -lpthread

if MCC_DRV_ENABLE
libponnet_la_CFLAGS += @PON_MCC_DRV_INCLUDE_PATH@
libponnet_la_LDFLAGS += @PON_MCC_DRV_LIBRARY_PATH@
libponnet_la_LIBADD += -lponmcc
endif

check-style:
	for f in $(filter %.h %.c,$(DISTFILES)); do \
		$(CHECK_SYNTAX) $(addprefix @abs_srcdir@/,$$f); \
	done
