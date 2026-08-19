KCOV_INSTRUMENT:=$(CONFIG_QOS_TC_KCOV)

obj-$(CONFIG_QOS_TC) = qos_tc.o

qos_tc-objs := \
	qos_tc_main.o \
	qos_tc_qos.o  \
	qos_tc_qmap.o \
	qos_tc_drr.o \
	qos_tc_debugfs.o \
	qos_tc_red.o \
	qos_tc_prio.o \
	qos_tc_mqprio.o \
	qos_tc_flower.o \
	qos_tc_vlan_prepare.o \
	qos_tc_vlan_storage.o \
	qos_tc_ext_vlan.o \
	qos_tc_vlan_filter.o \
	qos_tc_pce.o \
	qos_tc_parser.o \
	qos_tc_mirred.o \
	qos_tc_ds_tc_remap.o \
	qos_tc_police.o \
	qos_tc_tbf.o \
	qos_tc_trap.o \
	qos_tc_ip_drop.o \
	qos_tc_skbedit.o \
	qos_tc_codel.o \
	qos_tc_extract_cfm.o

qos_tc-$(CONFIG_QOS_TC_TRACING) += qos_tc_trace.o

# otherwise trace header is not found
CFLAGS_qos_tc_trace.o += -I$(srctree)/$(src)
