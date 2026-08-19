#
# Copyright (C) 2017 Cezary Jackiewicz <cezary@eko.one.pll>
#
# This is free software, licensed under the GNU General Public License v2.
#
define KernelPackage/mrpc
 SUBMENU:=MaxLinear
 TITLE:=RPC Over HW Mailbox Support
 DEPENDS:=@TARGET_intel_x86
 KCONFIG:=CONFIG_MRPC=y
endef

define KernelPackage/mrpc/description
 MaxLinear RPC Over HW Mailbox (mrpc) support
endef

$(eval $(call KernelPackage,mrpc))

define KernelPackage/mrpc_examples
 SUBMENU:=MaxLinear
 TITLE:=RPC Over HW Mailbox (mrpc) examples
 DEPENDS:=@TARGET_intel_x86 +kmod-mrpc
 KCONFIG:= \
       CONFIG_MRPC_EXAMPLES=y \
       CONFIG_MRPC_CLIENT_EXAMPLE=m \
       CONFIG_MRPC_SERVER_EXAMPLE=m
 FILES:= \
       $(LINUX_DIR)/drivers/mrpc/examples/client_example.ko \
       $(LINUX_DIR)/drivers/mrpc/examples/server_example.ko
endef

define KernelPackage/mrpc_examples/description
 MaxLinear RPC Over HW Mailbox (mrpc) client and server examples
endef

$(eval $(call KernelPackage,mrpc_examples))

define KernelPackage/mrpc_modphy
 SUBMENU:=MaxLinear
 TITLE:=mrpc modphy client
 DEPENDS:=@TARGET_intel_x86 +kmod-mrpc
 KCONFIG:=CONFIG_MRPC_MODPHY_CLIENT=y
endef

define KernelPackage/mrpc_modphy/description
 mrpc modphy client
endef

$(eval $(call KernelPackage,mrpc_modphy))

define KernelPackage/mrpc_pp
 SUBMENU:=MaxLinear
 TITLE:=mrpc pp client
 DEPENDS:=@TARGET_intel_x86 +kmod-mrpc
 KCONFIG:=CONFIG_MRPC_PP_CLIENT=y
endef

define KernelPackage/mrpc_pp/description
 mrpc pp client
endef

$(eval $(call KernelPackage,mrpc_pp))

define KernelPackage/mrpc_cppi
 SUBMENU:=MaxLinear
 TITLE:=mrpc cppi client
 DEPENDS:=@TARGET_intel_x86 +kmod-mrpc
 KCONFIG:= \
	CONFIG_MRPC_CPPI_CLIENT=y \
	CONFIG_MRPC_CPPI_SERVER=y
endef

define KernelPackage/mrpc_cppi/description
 mrpc cppi client
endef

$(eval $(call KernelPackage,mrpc_cppi))

define KernelPackage/cuckoo_hash_test
SUBMENU:= MaxLinear
TITLE:= Cuckoo Hash Test kernel module
DEPENDS:= @TARGET_intel_x86_lgm
KCONFIG:= CONFIG_CUCKOO_HASH_TEST=m
FILES:= $(LINUX_DIR)/lib/cuckoo_hash_test.ko
endef

define KernelPackage/cuckoo_hash_test/description
 Cuckoo hash test kernel module
endef

$(eval $(call KernelPackage,cuckoo_hash_test))


MXL_ETH_DRV_AUTOLOAD:= sfp mxl_eth_drv
define KernelPackage/mxl_eth_drv
 SUBMENU:= MaxLinear
 TITLE:= MaxLinear Eth driver kernel module
 DEPENDS:= @TARGET_intel_x86_lgm +kmod-phylink
 KCONFIG:= \
	CONFIG_MXL_ETH \
	CONFIG_MXL_ETH_THERMAL=y
 FILES:= $(LINUX_DIR)/drivers/net/phy/sfp.ko \
	$(LINUX_DIR)/drivers/net/mdio/mdio-i2c.ko \
	$(LINUX_DIR)/drivers/net/ethernet/mxl/mxl_eth_drv.ko
 AUTOLOAD:=$(call AutoLoad,98,$(MXL_ETH_DRV_AUTOLOAD))
endef


define KernelPackage/mxl_eth_driver/description
  mxl eth driver
endef

$(eval $(call KernelPackage,mxl_eth_drv))


define KernelPackage/mxl_pon_hgu_vuni_lgm
 SUBMENU:= MaxLinear
 TITLE:= MaxLinear PON HGU vUNI Driver (Module Support)
 DEPENDS:= @TARGET_intel_x86_lgm
 KCONFIG:= CONFIG_MXL_PON_HGU_VUNI
 FILES:= $(LINUX_DIR)/drivers/net/ethernet/mxl/mxl_pon_hgu_vuni.ko
 AUTOLOAD:=$(call AutoProbe,mxl_pon_hgu_vuni)
endef


define KernelPackage/mxl_pon_hgu_vuni_lgm/description
  Mxl PON HGU vUNI Driver module
endef

$(eval $(call KernelPackage,mxl_pon_hgu_vuni_lgm))

define KernelPackage/p34x_phy_fwdl
 SUBMENU:= MaxLinear
 TITLE:= MaxLinear P34X FW download Driver (Module Support)
 DEPENDS:= @TARGET_intel_x86_lgm
 KCONFIG:= CONFIG_MXL_P34X_FWDL
 FILES:= $(LINUX_DIR)/drivers/net/datapath/p34x/p34x_phy_fwdl.ko
 AUTOLOAD:=$(call AutoLoad,94,p34x_phy_fwdl)
endef

define KernelPackage/p34x_phy_fwdl/description
  MaxLinear P34X FW download Driver
endef

$(eval $(call KernelPackage,p34x_phy_fwdl))

define KernelPackage/mxl_gpy
 SUBMENU:= MaxLinear
 TITLE:= MaxLinear P34X driver (Module Support)
 DEPENDS:= @TARGET_intel_x86_lgm
 KCONFIG:= CONFIG_MAXLINEAR_GPHY
 FILES:= $(LINUX_DIR)/drivers/net/phy/mxl-gpy.ko
 AUTOLOAD:=$(call AutoLoad,95,mxl_gpy)
endef

define KernelPackage/mxl_gpy/description
  MaxLinear P34X phy driver
endef

$(eval $(call KernelPackage,mxl_gpy))

define KernelPackage/mxl_lgm_toe
 SUBMENU:= MaxLinear
 TITLE:= MaxLinear LGM TOE Driver (Module Support)
 DEPENDS:= @TARGET_intel_x86_lgm +kmod-p34x_phy_fwdl
 KCONFIG:= CONFIG_LGM_TOE
 FILES:= $(LINUX_DIR)/drivers/net/datapath/toe/toe.ko
 AUTOLOAD:=$(call AutoLoad,90,toe)
endef

define KernelPackage/mxl_lgm_toe/description
  MaxLinear LGM TOE Driver module
endef

$(eval $(call KernelPackage,mxl_lgm_toe))

define KernelPackage/ipsec4-offload
 SUBMENU:=$(NETWORK_SUPPORT_MENU)
 TITLE:=IPsec hardware offload (IPv4)
 DEPENDS:=kmod-ipsec4
 KCONFIG:= CONFIG_INET_ESP_OFFLOAD
 FILES:=$(LINUX_DIR)/net/ipv4/esp4_offload.ko
 AUTOLOAD:=$(call AutoLoad,32,esp4_offload)
endef

define KernelPackage/ipsec4-offload/description
 Kernel modules for IPsec hardware offload support in IPv4.
endef

$(eval $(call KernelPackage,ipsec4-offload))

define KernelPackage/ipsec6-offload
 SUBMENU:=$(NETWORK_SUPPORT_MENU)
 TITLE:=IPsec hardware offload (IPv6)
 DEPENDS:=kmod-ipsec6
 KCONFIG:= CONFIG_INET6_ESP_OFFLOAD
 FILES:=$(LINUX_DIR)/net/ipv6/esp6_offload.ko
 AUTOLOAD:=$(call AutoLoad,32,esp6_offload)
endef

define KernelPackage/ipsec6-offload/description
 Kernel modules for IPsec hardware offload support in IPv6.
endef

$(eval $(call KernelPackage,ipsec6-offload))

define KernelPackage/safexcel
 SUBMENU:= MaxLinear
 TITLE:= Inside secure eip197 kernel module
 DEPENDS:= @TARGET_intel_x86_lgm +eip197-firmware
 KCONFIG:= CONFIG_CRYPTO_DEV_SAFEXCEL
 FILES:= $(LINUX_DIR)/drivers/crypto/inside-secure/crypto_safexcel.ko
 AUTOLOAD:=$(call AutoLoad,80,crypto_safexcel)
endef

define KernelPackage/safexcel/description
  Inside Secure's SafeXcel cryptographic engine driver
endef

$(eval $(call KernelPackage,safexcel))

define KernelPackage/mxl_vpn
 SUBMENU:= MaxLinear
 TITLE:= MaxLinear vpn driver kernel module
 DEPENDS:= @TARGET_intel_x86_lgm +kmod-safexcel +vpn-firmware-lgm \
	+kmod-ipsec4-offload +kmod-ipsec6-offload
 KCONFIG:= \
	CONFIG_MXL_VPN=m \
	CONFIG_MXL_VPN_TEST=n
 FILES:= $(LINUX_DIR)/drivers/net/datapath/vpn/mxl_vpn.ko
 AUTOLOAD:=$(call AutoLoad,90,mxl_vpn)
endef

define KernelPackage/mxl_vpn/description
  MaxLinear vpn module
endef

$(eval $(call KernelPackage,mxl_vpn))

define KernelPackage/ppv4
 SUBMENU:= MaxLinear
 TITLE:= PPv4 Driver
 DEPENDS:= @TARGET_intel_x86_lgm
 KCONFIG:= CONFIG_PPV4=y
 #In order to compile PPv4 as a module:
 #1. Change CONFIG_PPV4=m
 #2. Uncomment the next line
 #FILES:= $(LINUX_DIR)/drivers/net/ppv4/pp_drv.ko
endef

define KernelPackage/ppv4/description
  PPv4 Driver
endef

$(eval $(call KernelPackage,ppv4))

define KernelPackage/intel_ppv4_fw_lgm
 SUBMENU:= MaxLinear
 TITLE:=MaxLinear PPv4 FW for LGM
 DEPENDS:=@TARGET_intel_x86_lgm +ppv4-firmware-lgm
endef

define KernelPackage/intel_ppv4_fw_lgm/description
 MaxLinear PPv4 FW
endef

$(eval $(call KernelPackage,intel_ppv4_fw_lgm))

define KernelPackage/sgam-drv
 SUBMENU:=MaxLinear
 TITLE:=Session based group accounting and metering module
 DEPENDS:=@TARGET_intel_x86_lgm +kmod-ppv4
 MAINTAINER:=MaxLinear
 KCONFIG:= CONFIG_SGAM
 FILES:= $(LINUX_DIR)/drivers/net/sgam/sgam_drv.ko
 AUTOLOAD:=$(call AutoProbe, sgam_drv)
endef

define KernelPackage/sgam-drv/description
 This is the core module in SGAM and it maintains the database of the
 configured meters and group account. It provides IOCTLs to sgam_cli to
 create accounts/meters and get their stats. It also provides APIs/hooks to the
 xt_GROUP target to convert acccount/meter name to its ID.
endef

$(eval $(call KernelPackage,sgam-drv))

define KernelPackage/intel_lgm_haps_clk
 SUBMENU:=MaxLinear
 TITLE:=LGM CGU driver run on HAPS
 DEPENDS:=@TARGET_intel_x86
 KCONFIG:=CONFIG_INTEL_LGM_HAPS_CLK=y
endef

define KernelPackage/intel_lgm_haps_clk/description
 Enable this if you are running on HAPS platform
endef

$(eval $(call KernelPackage,intel_lgm_haps_clk))

define KernelPackage/kvm_guest
 SUBMENU:=MaxLinear
 TITLE:=KVM guest kernel configuration
 DEPENDS:= @TARGET_intel_x86 +kmod-e1000
 KCONFIG:= \
	CONFIG_HYPERVISOR_GUEST=y	\
	CONFIG_PARAVIRT=y		\
	CONFIG_PARAVIRT_DEBUG=y		\
	CONFIG_PARAVIRT_SPINLOCKS=y	\
	CONFIG_KVM_GUEST=y		\
	CONFIG_KVM_DEBUG_FS=y		\
	CONFIG_PARAVIRT_CLOCK=y		\
	CONFIG_VIRTIO_NET=y		\
	CONFIG_VIRTIO_PCI=y		\
	CONFIG_VIRTIO_CONSOLE=y		\
	CONFIG_EARLY_PRINTK=y		\
	CONFIG_SERIAL_8250=y		\
	CONFIG_SERIAL_8250_CONSOLE=y	\
	CONFIG_SERIAL_EARLYCON=y	\
	CONFIG_ACPI=y			\
	CONFIG_EFI=y			\
	CONFIG_EFI_STUB=y		\
	CONFIG_VSOCKETS=y		\
	CONFIG_HPET_TIMER=y		\
	CONFIG_X86_16BIT=y		\
	CONFIG_VHOST_NET=y		\
	CONFIG_VIRTIO_NET=y
endef

define KernelPackage/kvm_guest/description
  Configure the kernel as a KVM guest image
endef

$(eval $(call KernelPackage,kvm_guest))

define KernelPackage/phy-lgm-usb
  TITLE:=MaxLinear LGM USB PHY
  DEPENDS:= @TARGET_intel_x86_lgm
  KCONFIG:= CONFIG_USB_LGM_PHY
  FILES:= $(LINUX_DIR)/drivers/phy/phy-lgm-usb.ko
  AUTOLOAD:=$(call AutoLoad,52,phy-lgm-usb,1)
endef

define KernelPackage/phy-lgm-usb/description
 This driver provides USB phy support on LGM
endef

$(eval $(call KernelPackage,phy-lgm-usb))

define KernelPackage/usb-dwc3-intel
  TITLE:=DWC3 OF driver
  DEPENDS:= @TARGET_intel_x86_lgm +kmod-usb-dwc3 +kmod-phy-lgm-usb
  KCONFIG:= CONFIG_USB_DWC3_OF_SIMPLE
  FILES:= $(LINUX_DIR)/drivers/usb/dwc3/dwc3-of-simple.ko
  AUTOLOAD:=$(call AutoProbe,dwc3-of-simple,1)
  $(call AddDepends/usb)
endef

define KernelPackage/usb-dwc3-intel/description
 This driver provides generic platform glue for the integrated DesignWare
 USB3 IP Core in MaxLinear LGM Platforms
endef

$(eval $(call KernelPackage,usb-dwc3-intel))

define KernelPackage/extcon-ptn5150
 SUBMENU:=EXTCON
 TITLE:=NXP PTN5150 CC LOGIC USB EXTCON support
 DEPENDS:= @TARGET_intel_x86_lgm
 KCONFIG:= CONFIG_EXTCON_PTN5150
 FILES:= $(LINUX_DIR)/drivers/extcon/extcon-ptn5150.ko
 AUTOLOAD:=$(call AutoLoad,51,extcon-ptn5150,1)
endef

define KernelPackage/extcon-ptn5150/description
  NXP PTN5150 CC LOGIC USB EXTCON support
endef

$(eval $(call KernelPackage,extcon-ptn5150))

define KernelPackage/mxl_mps2_driver
 SUBMENU:= MaxLinear
 TITLE:= MaxLinear LGM MPS2 driver
 DEPENDS:= @TARGET_intel_x86_lgm
 KCONFIG:= \
	CONFIG_MXL_MPS2=y
endef

define KernelPackage/mxl_mps2_driver/description
  MXL LGM MPS2 driver support
endef

$(eval $(call KernelPackage,mxl_mps2_driver))

define KernelPackage/intel-extmark-x86
  TITLE:=MaxLinear Extension Mark Support
  SUBMENU:=MaxLinear
  DEPENDS:= @TARGET_intel_x86_lgm
  KCONFIG:= \
	CONFIG_NETWORK_EXTMARK=y
endef

define KernelPackage/intel-extmark-x86/description
 Add extension mark(extmark) param in sk_buff
endef

$(eval $(call KernelPackage,intel-extmark-x86))

define KernelPackage/use_wave700_palladium
  TITLE:=Palladium Support for WAVE700
  SUBMENU:=MaxLinear
  DEPENDS:= @TARGET_intel_x86_lgm
  KCONFIG:= \
        CONFIG_MXL_UMT_INTERVAL_DEFAULT=12800 \
        CONFIG_USE_WAVE700_PD_FPGA=y
endef

define KernelPackage/USE_WAVE700_Palladium/description
 Add Palladium support for WAVE700
endef

$(eval $(call KernelPackage,use_wave700_palladium))

define KernelPackage/use_wave700_fpga
  TITLE:=FPGA Support for WAVE700
  SUBMENU:=MaxLinear
  DEPENDS:= @TARGET_intel_x86_lgm
  KCONFIG:= \
        CONFIG_MXL_UMT_INTERVAL_DEFAULT=400 \
        CONFIG_USE_WAVE700_PD_FPGA=y
endef

define KernelPackage/USE_WAVE700_FPGA/description
 Add FPGA support for WAVE700
endef

$(eval $(call KernelPackage,use_wave700_fpga))


define KernelPackage/intel-icc-regmap-x86
  TITLE:=MaxLinear ICC regmap driver (compiled in)
  SUBMENU:=MaxLinear
  DEPENDS:=@TARGET_intel_x86_lgm
  KCONFIG:= \
        CONFIG_REGMAP_ICC=y
endef

define KernelPackage/intel-icc-regmap-x86/description
  Regmap over ICC driver for register access over the TEP.
endef

$(eval $(call KernelPackage,intel-icc-regmap-x86))

define KernelPackage/reset_on_hot
 SUBMENU:= MaxLinear
 TITLE:= Support reset on hot temperature
 DEPENDS:= @TARGET_intel_x86_lgm
 KCONFIG:= \
	CONFIG_RESET_ON_HOT=y
endef

define KernelPackage/reset_on_hot/description
 Support reset of the platform when the hot temperature is reached
endef

$(eval $(call KernelPackage,reset_on_hot))

define KernelPackage/battery_bbu
 SUBMENU:= MaxLinear
 TITLE:= Backup Battery support
 DEPENDS:= @TARGET_intel_x86_lgm
 KCONFIG:= \
	CONFIG_POWER_SUPPLY=y \
	CONFIG_BATTERY_BBU_MXL=y
endef

define KernelPackage/battery_bbu/description
 Backup Battey support
endef

$(eval $(call KernelPackage,battery_bbu))

define KernelPackage/mxl_icc_driver
 SUBMENU:= MaxLinear
 TITLE:= MaxLinear LGM ICC driver
 DEPENDS:= @TARGET_intel_x86_lgm +kmod-mxl_mps2_driver
 KCONFIG:= \
	CONFIG_MXL_ICC=y
endef

define KernelPackage/mxl_icc_driver/description
  LGM ICC driver support
endef

$(eval $(call KernelPackage,mxl_icc_driver))

define KernelPackage/intel_noc_firewall
 SUBMENU:= MaxLinear
 TITLE:=  LGM Noc firewall driver enabling
 DEPENDS:= @TARGET_intel_x86_lgm +kmod-mxl_icc_driver
 KCONFIG:= \
	CONFIG_MXL_NOC_FIREWALL=y \
	CONFIG_P2P_FIREWALL=y \
	CONFIG_MXL_NOC_FW_DEFAULT=y
endef

define KernelPackage/intel_noc_firewall/description
  LGM Noc firewall enabling configuration
endef

$(eval $(call KernelPackage,intel_noc_firewall))

define KernelPackage/ugw_kdump-enabler
 SUBMENU:= MaxLinear
 TITLE:= MXL Kdump support
 DEPENDS:= @TARGET_intel_x86_lgm +kexec +kexec-tools
 KCONFIG:= \
	CONFIG_KEXEC=y
	CONFIG_KEXEC_FILE=y
endef

define KernelPackage/ugw_kdump-enabler/description
  Enable kdump for UGW
endef
$(eval $(call KernelPackage,ugw_kdump-enabler))

define KernelPackage/directpath-dp
  SUBMENU:=MaxLinear
  TITLE:=Directpath datapath (DPDP) driver
  DEPENDS:=@TARGET_intel_x86_lgm
  MAINTAINER:=MaxLinear
  KCONFIG:=CONFIG_DP_DIRECTPATH=m
  FILES:=$(LINUX_DIR)/drivers/net/datapath/directpath/directpath_dp.ko
  AUTOLOAD:=$(call AutoLoad,06,directpath_dp)
endef

define KernelPackage/directpath-dp/description
  DirectPath datapath (DPDP) driver
endef

$(eval $(call KernelPackage,directpath-dp))

define KernelPackage/ebt-prio
 TITLE:=EBT Priority
 DEPENDS:=+ebtables +kmod-nf-ipt
 SUBMENU:=${NF_MENU}
 KCONFIG:= \
        CONFIG_BRIDGE_EBT_PRIO
 FILES:= \
        $(LINUX_DIR)/net/bridge/netfilter/ebt_prio.ko
 AUTOLOAD:= \
        $(call AutoProbe,ebt_prio)
endef

define KernelPackage/ebt-prio/description
 ebtables extension to support service prioritization
endef

$(eval $(call KernelPackage,ebt-prio))
