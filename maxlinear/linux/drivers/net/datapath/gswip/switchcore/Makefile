#
# Makefile for the Switch API
#

obj-$(CONFIG_MXL_ETHSW_API) += drv_switch_api.o

ccflags-y += -I$(srctree)/$(src)/include -I$(srctree)/$(src)/../mac/include/ -I$(srctree)/$(src)/../macsec/ -Wimplicit-fallthrough

drv_switch_api-objs := src/gsw_ioctl_wrapper.o
drv_switch_api-objs += src/gsw_init.o
drv_switch_api-objs += src/gsw_flow_core.o
drv_switch_api-objs += src/gsw_ll_table.o
drv_switch_api-objs += src/gsw_flow_pce.o
drv_switch_api-objs += src/gsw_pae.o
drv_switch_api-objs += src/gsw_swmcast.o
drv_switch_api-objs += src/gsw_debug.o
drv_switch_api-objs += src/gsw_defconf.o
drv_switch_api-objs += src/gsw_irq.o
drv_switch_api-objs += src/gsw_tbl_rw.o
drv_switch_api-objs += src/gsw_mdio.o

