#
# Copyright (C) 2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

I2C_LANTIQ_MODULES:= \
  CONFIG_I2C_LANTIQ:drivers/i2c/busses/i2c-lantiq

define KernelPackage/i2c-intel_mips
  TITLE:=Lantiq I2C controller
  $(call i2c_defaults,$(I2C_LANTIQ_MODULES),52)
  DEPENDS:=kmod-i2c-core @TARGET_intel_mips
endef

define KernelPackage/i2c-intel_mips/description
  Kernel support for the Lantiq/Falcon I2C controller
endef

$(eval $(call KernelPackage,i2c-intel_mips))

define KernelPackage/intel_mips-vpe
  TITLE:=Lantiq VPE extensions
  SUBMENU:=Lantiq
  DEPENDS:=@TARGET_intel_mips +kmod-vpe
  KCONFIG:=CONFIG_IFX_VPE_CACHE_SPLIT=y \
	  CONFIG_IFX_VPE_EXT=y \
	  CONFIG_VPE_SOFTDOG=y \
	  CONFIG_MTSCHED=y \
	  CONFIG_PERFCTRS=n
endef

define KernelPackage/intel_mips-vpe/description
  Kernel extensions for the Lantiq SoC
endef

$(eval $(call KernelPackage,intel_mips-vpe))

define KernelPackage/intel_mips-nf
  TITLE:=Lantiq NF extensions
  SUBMENU:=Lantiq
  DEPENDS:=@TARGET_intel_mips
  KCONFIG:=CONFIG_NF_CONNTRACK_EVENTS=y
endef

define KernelPackage/intel_mips-nf/description
  Netfilter extensions for the Lantiq SoC
endef

$(eval $(call KernelPackage,intel_mips-nf))

define KernelPackage/spi-intel_mips-ssc
  SUBMENU:=$(SPI_MENU)
  TITLE:=Lantiq SPI controller
  DEPENDS:=@TARGET_intel_mips +kmod-spi-bitbang @!LINUX_4_9
  KCONFIG:=CONFIG_SPI_XWAY \
	  CONFIG_SPI_XWAY_BV=y
  FILES:=$(LINUX_DIR)/drivers/spi/spi-xway.ko
  AUTOLOAD:=$(call AutoProbe,spi-xway)
endef

define KernelPackage/spi-intel_mips-ssc/description
  Lantiq SPI controller
endef

$(eval $(call KernelPackage,spi-intel_mips-ssc))

define KernelPackage/spi-intel_mips-ssc-csi
  SUBMENU:=$(SPI_MENU)
  TITLE:=Lantiq SPI controller for CSI
  DEPENDS:=@TARGET_intel_mips +kmod-spi-bitbang
  KCONFIG:=CONFIG_SPI_XWAY_CSI
  FILES:=$(LINUX_DIR)/drivers/spi/spi-xway-csi.ko
  AUTOLOAD:=$(call AutoProbe,spi-xway-csi)
endef

define KernelPackage/spi-intel_mips-ssc-csi/description
  Lantiq SPI controller for CSI
endef

$(eval $(call KernelPackage,spi-intel_mips-ssc-csi))

define KernelPackage/spi-intel_mips
  SUBMENU:=$(SPI_MENU)
  TITLE:=Lantiq SPI controller (new)
  DEPENDS:=@TARGET_intel_mips @LINUX_4_9
  KCONFIG:=CONFIG_SPI_LANTIQ_SSC \
          CONFIG_SPI=y \
          CONFIG_SPI_MASTER=y
  FILES:=$(LINUX_DIR)/drivers/spi/spi-lantiq-ssc.ko
  AUTOLOAD:=$(call AutoProbe,spi-lantiq-ssc)
endef

define KernelPackage/spi-intel_mips/description
  New Lantiq SPI controller
endef

$(eval $(call KernelPackage,spi-intel_mips))


define KernelPackage/spi-intel_mips-grx500
  SUBMENU:=$(SPI_MENU)
  TITLE:=Lantiq SPI controller for GRX500
  DEPENDS:=@(TARGET_intel_mips_xrx500||TARGET_intel_mips_prx300) +kmod-spi-bitbang
  KCONFIG:=CONFIG_SPI_GRX500 \
          CONFIG_SPI=y \
          CONFIG_SPI_MASTER=y \
          CONFIG_SPI_GRX500_POLL=n
  FILES:=$(LINUX_DIR)/drivers/spi/spi-grx500.ko
  AUTOLOAD:=$(call AutoProbe,spi-grx500)
endef

define KernelPackage/spi-intel_mips-grx500/description
  Lantiq SPI controller for GRX500
endef

$(eval $(call KernelPackage,spi-intel_mips-grx500))


define KernelPackage/intel_mips-svip-ve
  TITLE:=Lantiq SVIP virtual ethernet
  SUBMENU:=Lantiq
  DEPENDS:=@(TARGET_intel_mips_svip_be||TARGET_intel_mips_svip_le)
  KCONFIG:=CONFIG_LANTIQ_SVIP_VIRTUAL_ETH=y
endef

define KernelPackage/intel_mips-ve/description
  Lantiq SVIP virtual ethernet
endef

$(eval $(call KernelPackage,intel_mips-svip-ve))

define KernelPackage/intel_mips-svip-nat
  TITLE:=Lantiq SVIP NAT
  SUBMENU:=Lantiq
  DEPENDS:=@(TARGET_intel_mips_svip_be||TARGET_intel_mips_svip_le)
  KCONFIG:=CONFIG_IPV6=y \
	  CONFIG_LTQ_SVIP_NAT=y \
	  CONFIG_LTQ_SVIP_NAT_DESTIP_CHECK=y \
	  CONFIG_LTQ_SVIP_NAT_DESTIP_LIST_SIZE=10 \
	  CONFIG_LTQ_SVIP_NAT_RULES_TOTAL=768 \
	  CONFIG_LTQ_SVIP_NAT_UDP_PORT_BASE=50000
endef

define KernelPackage/intel_mips-svip-nat/description
  Performs MAC and IP address translation of incoming and ougoing
  IP packets relative the address mapping details provided by the
  SVIP NAT rules. The packets will be intercept in the IP module and
  when an appropriate NAT rule exists the source and destination address
  details are replaced, and the packets are sent out the destined Ethernet
  interface.
endef

$(eval $(call KernelPackage,intel_mips-svip-nat))

define KernelPackage/intel_eth_drv_xrx500
 SUBMENU:=Lantiq
 TITLE:= Intel Ethernet Driver for xRX500 (Module Support)
 DEPENDS:=@(TARGET_intel_mips_xrx500||TARGET_intel_mips_prx300) +kmod-intel_eth_xrx500_fw
 KCONFIG:= \
        CONFIG_LTQ_ETH_XRX500 \
        CONFIG_SW_ROUTING_MODE=y \
        CONFIG_XRX500_ETH_DRV_THERMAL_SUPPORT=n \
        CONFIG_HAPS_CPU_LOOPBACK_TEST=n
 FILES:= \
        $(LINUX_DIR)/drivers/net/ethernet/lantiq/ltq_eth_drv_xrx500.ko
  AUTOLOAD:=$(call AutoProbe,ltq_eth_drv_xrx500)
endef

define KernelPackage/intel_eth_drv_xrx500/description
 Intel Ethernet Driver (Module Support)
endef

$(eval $(call KernelPackage,intel_eth_drv_xrx500))

define KernelPackage/intel_gint_eth_drv
 SUBMENU:=Lantiq
 TITLE:= Intel G.INT Ethernet Driver (Module Support)
 DEPENDS:=@TARGET_intel_mips_prx300
 KCONFIG:=CONFIG_INTEL_GINT_ETH
 FILES:= \
        $(LINUX_DIR)/drivers/net/ethernet/lantiq/intel_gint_eth_drv.ko
  AUTOLOAD:=$(call AutoProbe,intel_gint_eth_drv)
endef

define KernelPackage/intel_gint_eth_drv/description
 Intel G.INT Ethernet Driver (Module Support)
endef

$(eval $(call KernelPackage,intel_gint_eth_drv))

define KernelPackage/intel_pon_hgu_vuni
 SUBMENU:=Lantiq
 TITLE:= Intel PON HGU vUNI Driver (Module Support)
 DEPENDS:=@TARGET_intel_mips_prx300
 KCONFIG:=CONFIG_INTEL_PON_HGU_VUNI
 FILES:= \
        $(LINUX_DIR)/drivers/net/ethernet/lantiq/intel_pon_hgu_vuni.ko
  AUTOLOAD:=$(call AutoProbe,intel_pon_hgu_vuni)
endef

define KernelPackage/intel_pon_hgu_vuni/description
 Intel PON HGU vUNI Driver (Module Support)
endef

$(eval $(call KernelPackage,intel_pon_hgu_vuni))

define KernelPackage/intel_eth_toe_drv_xrx500
 SUBMENU:=Lantiq
 TITLE:= Intel Ethernet TOE Driver for xRX500
 DEPENDS:=kmod-intel_eth_drv_xrx500
 KCONFIG:= \
	CONFIG_LTQ_TOE_DRIVER=y
endef

define KernelPackage/intel_eth_toe_drv_xrx500/description
 Intel Ethernet TOE Driver
endef

$(eval $(call KernelPackage,intel_eth_toe_drv_xrx500))

define KernelPackage/intel_eth_xrx500_fw
 SUBMENU:=Lantiq
 TITLE:= Intel Ethernet Driver FW loading for xRX500 (Module Support)
 DEPENDS:=@(TARGET_intel_mips_xrx500||TARGET_intel_mips_prx300)
 KCONFIG:= \
        CONFIG_XRX500_PHY_FW
 FILES:= \
        $(LINUX_DIR)/drivers/net/ethernet/lantiq/xrx500_phy_fw.ko
  AUTOLOAD:=$(call AutoProbe,xrx500_phy_fw)
endef

define KernelPackage/intel_eth_xrx500_fw/description
 Intel Ethernet Driver FW loading (Module Support)
endef

$(eval $(call KernelPackage,intel_eth_xrx500_fw))


define KernelPackage/usb-dwc3-grx500
  TITLE:=Intel DWC3 USB GRX500 driver
  DEPENDS:=+kmod-usb-dwc3
  KCONFIG:= \
	CONFIG_PHY_GRX500_USB \
	CONFIG_USB_DWC3_GRX500

  FILES:= \
	$(LINUX_DIR)/drivers/phy/phy-grx500-usb.ko \
	$(LINUX_DIR)/drivers/usb/dwc3/dwc3-grx500.ko

  AUTOLOAD:=$(call AutoProbe,phy-grx500-usb dwc3-grx500)
  $(call AddDepends/usb)
endef

define KernelPackage/usb-dwc3-grx500/description
 This driver provides generic platform glue for the integrated DesignWare
 USB3 IP Core in Intel GRX500 Platforms
endef

$(eval $(call KernelPackage,usb-dwc3-grx500))


define KernelPackage/intel_ppv4_qos_drv
 SUBMENU:=Lantiq
 TITLE:=Intel PPv4 QoS Driver
 DEPENDS:=@TARGET_intel_mips_prx300 +ppv4-qos-firmware
 KCONFIG:= \
	CONFIG_LTQ_PPV4_QOS=y \
	CONFIG_LTQ_PPV4_QOS_TEST=n
endef

define KernelPackage/intel_ppv4_qos_drv/description
 Intel PPv4 QoS Driver
endef

$(eval $(call KernelPackage,intel_ppv4_qos_drv))


define KernelPackage/intel_ppv4_qos_drv_mod
 SUBMENU:=Lantiq
 TITLE:=Intel PPv4 QoS Driver
 DEPENDS:=@TARGET_intel_mips_prx300 @!PACKAGE_kmod-intel_ppv4_qos_drv +ppv4-qos-firmware
 KCONFIG:= \
	CONFIG_LTQ_PPV4_QOS \
	CONFIG_LTQ_PPV4_QOS_TEST=y
 FILES:= \
	$(LINUX_DIR)/drivers/net/ethernet/lantiq/ppv4/qos/pp_qos_drv.ko
  AUTOLOAD:=$(call AutoProbe,pp_qos_drv)
endef

define KernelPackage/intel_ppv4_qos_drv_mod/description
 Intel PPv4 QoS Driver (Module Support)
endef

$(eval $(call KernelPackage,intel_ppv4_qos_drv_mod))

define KernelPackage/intel-extmark
  TITLE:=Intel Extension Mark Support
  SUBMENU:=Intel
  KCONFIG:= \
	CONFIG_NETWORK_EXTMARK=y
endef

define KernelPackage/intel-extmark/description
 Add extension mark(extmark) param in sk_buff
endef

$(eval $(call KernelPackage,intel-extmark))

define KernelPackage/intel-cpufreq
  TITLE:=Intel CPUFREQ support
  SUBMENU:=Intel
  KCONFIG:= \
        CONFIG_CPU_FREQ_DEFAULT_GOV_CONSERVATIVE=y \
        CONFIG_INTEL_DATAPATH_CPUFREQ=y
endef

define KernelPackage/intel-cpufreq/description
  CPU frequency scaling support (CPUFreq driver) for Intel SoC's based on MIPS architecture. 
  If enabled the CONSERVATIVE_GOVERNOR will periodically check the current Linux CPU load and
  trigger UP or DOWN scaling of the CPU frequency based on the current CPU utilization.
  Datapath and voice driver can deny a DOWN scaling in case of activity.
endef

$(eval $(call KernelPackage,intel-cpufreq))


define KernelPackage/serial-lantiq
  TITLE:=Intel Serial (compile in)
  SUBMENU:=Intel
  DEPENDS:=@TARGET_intel_mips_prx300
  KCONFIG:=CONFIG_SERIAL_LANTIQ=y
endef

define KernelPackage/serial-lantiq/description
  Compiles the lantiq serial driver into the kernel.
endef

$(eval $(call KernelPackage,serial-lantiq))


define KernelPackage/serial-bootcore
  TITLE:=Intel bootcore Serial (compile in)
  SUBMENU:=Intel
  DEPENDS:=@TARGET_intel_mips_prx300_4kec
  KCONFIG:=CONFIG_SERIAL_GRX500_BOOTCORE_CONSOLE=y
endef

define KernelPackage/serial-bootcore/description
  Compiles the grx500 bootcore serial driver into the kernel.
endef

$(eval $(call KernelPackage,serial-bootcore))


define KernelPackage/intel-icc-regmap
  TITLE:=Intel ICC regmap driver (compiled in)
  SUBMENU:=Intel
  KCONFIG:= \
        CONFIG_REGMAP_ICC=y
endef

define KernelPackage/intel-icc-regmap/description
  Regmap over ICC driver for register access over the TEP.
endef

$(eval $(call KernelPackage,intel-icc-regmap))


define KernelPackage/pon-qos
  SUBMENU:=Lantiq
  TITLE:=PON QoS Driver
  DEPENDS:=@TARGET_intel_mips_prx300
  KCONFIG:=CONFIG_PON_QOS
  FILES:=$(LINUX_DIR)/drivers/net/datapath/pon_qos/pon_qos.ko
  AUTOLOAD:=$(call AutoProbe,pon_qos)
endef

define KernelPackage/pon_qos/description
 Kernel module for PON QoS support.
endef

$(eval $(call KernelPackage,pon-qos))
