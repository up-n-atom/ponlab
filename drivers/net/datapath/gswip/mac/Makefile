#
# Makefile for MAC
#

ccflags-y += -I$(srctree)/$(src)/include -Wimplicit-fallthrough
obj-$(CONFIG_MXL_ETHSW_API) += mac_driver.o

mac_driver-objs += src/macsec_drv.o
mac_driver-objs += src/mac_drv.o
mac_driver-objs += src/xgmac_main.o
mac_driver-objs += src/xgmac_debug.o
mac_driver-objs += src/xgmac_mtl_api.o
mac_driver-objs += src/xgmac_ptp.o
mac_driver-objs += src/xgmac_mac_api.o
mac_driver-objs += src/mac_cli.o
mac_driver-objs += src/xgmac_mdio.o
mac_driver-objs += src/mac_cfg.o
mac_driver-objs += src/gswss_api.o
mac_driver-objs += src/gswss_mac_api.o
mac_driver-objs += src/lmac_api.o
mac_driver-objs += src/mac_tx_fifo.o

