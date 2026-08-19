obj-m += directconnect_datapath.o
directconnect_datapath-y += directconnect_dp_api.o
directconnect_datapath-y += directconnect_dp_device.o
directconnect_datapath-y += directconnect_dp_dcmode_wrapper.o
directconnect_datapath-y += directconnect_dp_dcmode_api.o
directconnect_datapath-y += directconnect_dp_swpath.o
ifneq ($(CONFIG_DP_DIRECTPATH),)
directconnect_datapath-y += directconnect_dp_litepath.o
endif
directconnect_datapath-y += directconnect_dp_parser.o
directconnect_datapath-y += directconnect_dp_debug.o
directconnect_datapath-y += directconnect_dp_proc.o
ifeq ($(CONFIG_SW_DCMODE1_BUILTIN),y)
	directconnect_datapath-y += ../dc_mode/dc_mode1/dc_mode1_sw.o
endif
