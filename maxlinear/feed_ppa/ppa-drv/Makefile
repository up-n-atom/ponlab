#
# Copyright © 2020-2024 MaxLinear, Inc.
# Makefile for ppa-drv
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ppa-drv
PKG_SOURCE_NAME:=ppa_drv
# Update last digit only for tag
PKG_SOURCE_VERSION:=4.2.3.60
PKG_PROJECT:=maxlinear
PKG_SOURCE_PROTO:=git
CONFIG_UGW_PKG_SOURCE_URL:=ssh://git@github.com
PKG_SOURCE_URL:=$(CONFIG_UGW_PKG_SOURCE_URL)/$(PKG_PROJECT)/$(PKG_SOURCE_NAME).git
PKG_MIRROR_HASH:=4f049c06d5f82a56a4907beb9191bd4974a8eb9e12dba4fce3ea1fd6e51fd6c6
PKG_MAINTAINER:=MaxLinear
PKG_LICENSE:=GPL-2.0
PKG_LICENSE_FILES:=
PKG_MXL_LICENSE:=GPL_BSD/LICENSE
PKG_BUILD_PARALLEL:=1
ifeq ($(CONFIG_LINUX_5_15),y)
PKG_EXTMOD_SUBDIRS:=src
endif

PKG_CONFIG_DEPENDS:=CONFIG_PACKAGE_kmod-nf-conntrack

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk

define Package/ppacmd
  SECTION:=utils
  CATEGORY:=MaxLinear
  TITLE:=Packet Protocol Acceleration Commandline tool
  DEPENDS:=+kmod-ppa-drv +libsafec3-7
  MAINTAINER:=MaxLinear
  URL:=http://www.maxlinear.com
endef

define Package/ppacmd/description
  Command line tool for PPA acceleration
endef

define Package/ppacmd/install
	$(INSTALL_DIR) $(1)/usr/sbin/; \
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/ppacmd/ppacmd $(1)/usr/sbin/;
endef

define Package/qoscmd
  SECTION:=utils
  CATEGORY:=MaxLinear
  TITLE:=QoS Commandline tool
  DEPENDS:=+kmod-ppa-drv +libsafec3-7
  MAINTAINER:=MaxLinear
  URL:=http://www.maxlinear.com
endef

define Package/qoscmd/description
  Command line tool for PPA acceleration
endef

define Package/qoscmd/install
	$(INSTALL_DIR) $(1)/usr/sbin/; \
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/qoscmd/qoscmd $(1)/usr/sbin/;
endef

define KernelPackage/ppa-drv
  SUBMENU:=MaxLinear
  TITLE:=Packet Protocol Acceleration Support
  DEPENDS:= +kmod-nf-conntrack \
	+kmod-ppa-drv-stack-al \
	+TARGET_intel_mips_xrx500:kmod-ppa-drv-grx500 \
	PACKAGE_kmod-ppa-drv-grx500-mpe:kmod-ppa-drv-grx500-mpe \
	PACKAGE_kmod-qos-tc:kmod-qos-tc \
	+TARGET_intel_x86_lgm:kmod-ppa-drv-lgm \
	+kmod-nf-ipt +kmod-gwdpa-dpm
  MAINTAINER:=MaxLinear
  KCONFIG:=CONFIG_PPA=y \
	CONFIG_PPA_IPTABLE_EVENT_HANDLING=y
  FILES:=$(PKG_BUILD_DIR)/src/ppa_api/ppa_api.ko
  AUTOLOAD:=$(call AutoProbe, ppa_api)
  MENU:=1
endef

define KernelPackage/ppa-drv/description
  Kernel support for PPA acceleration
endef

define KernelPackage/ppa-drv-accel
  SUBMENU:=MaxLinear
  TITLE:=Packet Protocol Acceleration Support
  DEPENDS:=+kmod-ppa-drv
  MAINTAINER:=MaxLinear
  KCONFIG:=CONFIG_PPA_API_SW_FASTPATH=y \
	CONFIG_PPA_TCP_LITEPATH=y \
	CONFIG_PPA_UDP_LITEPATH=y
  FILES:=$(PKG_BUILD_DIR)/src/ppa_api/ppa_api_sw_accel_mod.ko
  AUTOLOAD:=$(call AutoProbe, ppa_api_sw_accel_mod)
endef

define KernelPackage/ppa-drv-accel/description
  Kernel support for PPA acceleration
endef

define KernelPackage/ppa-drv-stack-al
  SUBMENU:=MaxLinear
  TITLE:=Packet Protocol Acceleration Stack Adaptation Layer
  DEPENDS:=+kmod-nf-conntrack +kmod-veth
  MAINTAINER:=MaxLinear
  KCONFIG:=CONFIG_PPA_QOS=n \
 	CONFIG_PPA_API_DIRECTPATH=n \
	CONFIG_PPA_TMU_MIB_SUPPORT=n \
  	CONFIG_PPA_API_SW_FASTPATH=n \
	CONFIG_PPA_BR_MAC_LEARNING=n \
	CONFIG_PPA_RT_SESS_LEARNING=n \
  	CONFIG_PPA_BR_SESS_LEARNING=n \
	CONFIG_PPA_EXT_PKT_LEARNING=n \
	CONFIG_PPA_API_DIRECTCONNECT=n \
	CONFIG_PPA_TCP_LITEPATH=n
  FILES:=$(PKG_BUILD_DIR)/src/ppa_api/ppa_drv_stack_al.ko
  AUTOLOAD:=$(call AutoProbe,ppa_drv_stack_al)
endef

define KernelPackage/ppa-drv-stack-al/description
  PPA acceleration Stack Adaptation Layer
endef

define KernelPackage/ppa-drv-grx500
  SUBMENU:=MaxLinear
  TITLE:=Packet Protocol Acceleration Support for xRX500
  DEPENDS:=@TARGET_intel_mips_xrx500 +kmod-ppa-drv-stack-al
  MAINTAINER:=MaxLinear
  KCONFIG:=CONFIG_PPA_QOS=y \
	CONFIG_PPA_BR_MAC_LEARNING=y \
	CONFIG_PPA_RT_SESS_LEARNING=y \
	CONFIG_PPA_TMU_MIB_SUPPORT=y \
	CONFIG_INTEL_DATAPATH_HAL_GSWIP30_MIB=y \
	CONFIG_INTEL_DATAPATH_MIB_TMU_MPE_MIB=y \
	CONFIG_PPA_API_DIRECTCONNECT=y \
	CONFIG_PPA_API_DIRECTPATH=y \
	CONFIG_PPA_MIB_MODE=y \
	CONFIG_PPA_NAT_LOOP_BACK=y
  FILES:=$(PKG_BUILD_DIR)/src/platform/xrx500/ltq_pae_hal.ko \
	$(PKG_BUILD_DIR)/src/platform/xrx500/ltq_tmu_hal_drv.ko
  AUTOLOAD:=$(call AutoProbe,ltq_pae_hal ltq_tmu_hal_drv)
endef

define KernelPackage/ppa-drv-grx500/description
  PPA GRX500 Acceleration Support
endef

define KernelPackage/ppa-drv-prx300-gw
  SUBMENU:=MaxLinear
  TITLE:=Packet Protocol Acceleration Support for PRX300 GW
  DEPENDS:=@TARGET_intel_mips_prx300 +kmod-ppa-drv-stack-al \
	+kmod-sched-flower +kmod-sched-core
  MAINTAINER:=MaxLinear
  KCONFIG:=CONFIG_PPA_QOS=n  \
	CONFIG_PPA_RT_SESS_LEARNING=y \
	CONFIG_PPA_API_DIRECTPATH=y \
	CONFIG_PPA_API_DIRECTCONNECT=y
endef

define KernelPackage/ppa-drv-lgm
 SUBMENU:=MaxLinear
 TITLE:=Packet Protocol Acceleration Support for LGM
 DEPENDS:=@TARGET_intel_x86_lgm +kmod-ppa-drv-stack-al +kmod-gwdpa-dpm +kmod-packet_learn \
	+PACKAGE_kmod-dpl:kmod-dpl +PACKAGE_kmod-qos-mgr-drv:kmod-qos-mgr-drv
 MAINTAINER:=MaxLinear
 KCONFIG:= \
	CONFIG_PPA_QOS=n \
	CONFIG_PPA_EXT_PKT_LEARNING=y \
	CONFIG_PPA_API_DIRECTCONNECT=y \
	CONFIG_PPA_BBF247_MODE1=y

 FILES:= $(PKG_BUILD_DIR)/src/platform/lgm/lgm_pp_hal_drv.ko
 AUTOLOAD:=$(call AutoProbe, lgm_pp_hal_drv)
endef

define KernelPackage/ppa-drv-lgm/description
  PPA LGM Acceleration Support
endef

define KernelPackage/qos-mgr-drv
  SUBMENU:=MaxLinear
  TITLE:=QoS Manager Support
  DEPENDS:=+TARGET_intel_mips_prx300:kmod-sched-flower \
	+TARGET_intel_mips_prx300:kmod-sched-core \
	+TARGET_intel_mips_prx300:kmod-nf-conntrack \
	+kmod-gwdpa-dpm
  MAINTAINER:=MaxLinear
  KCONFIG:=CONFIG_QOS_MGR=y
  FILES:=$(PKG_BUILD_DIR)/src/qos/qos_mgr_drv.ko
  AUTOLOAD:=$(call AutoProbe, qos_mgr_drv)
endef

define KernelPackage/qos-mgr-drv/description
  QoS Manager Support
endef

define KernelPackage/ppa-drv-grx500-mpe
  SUBMENU:=MaxLinear
  TITLE:=Packet Protocol Acceleration Multi-Protocol Engine for GRX500
  DEPENDS:=@TARGET_intel_mips_xrx500 +kmod-ppa-drv-grx500
  MAINTAINER:=MaxLinear
  FILES:=$(PKG_BUILD_DIR)/src/platform/xrx500/ltq_mpe_hal_drv.ko
  AUTOLOAD:=$(call AutoProbe, ltq_mpe_hal_drv)
endef

define KernelPackage/ppa-drv-grx500-mpe/description
  PPA GRX500 Packet Protocol Acceleration Multi-Protocol Engine module
endef

define KernelPackage/ppa-drv-grx500-mpe/install
	$(INSTALL_DIR) $(1)/lib/firmware; \
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/platform/xrx500/mpe_fw_be.img $(1)/lib/firmware/;
endef

define KernelPackage/ppa-drv-prx300-mpe
  SUBMENU:=MaxLinear
  TITLE:=Packet Protocol Acceleration Multi-Protocol Engine for PRX300
  DEPENDS:=@TARGET_intel_mips_prx300 +kmod-ppa-drv-prx300-gw +mpe_fw_prx300 +kmod-gwdpa-dpm \
	+PACKAGE_kmod-qos-tc:kmod-qos-tc
  FILES:=$(PKG_BUILD_DIR)/src/platform/prx300/ltq_mpe_hal_drv.ko
  AUTOLOAD:=$(call AutoProbe, ltq_mpe_hal_drv)
endef

define KernelPackage/ppa-drv-prx300-mpe/description
  PPA Packet Protocol Acceleration Multi-Protocol Engine module for PRX300
endef

define KernelPackage/ppa-drv-prx300-mpe/install
endef

define KernelPackage/ppa-drv-mpe-ip97
  SUBMENU:=MaxLinear
  TITLE:=LTQ PPA MPE IP97 Support for xRX500
  DEPENDS:=@TARGET_intel_mips_xrx500 @PACKAGE_kmod-ppa-drv +kmod-eip97
  KCONFIG:=CONFIG_PPA_MPE_IP97=y
endef

define KernelPackage/ppa-drv-mpe-ip97/description
  PPA GRX500 MPE IP97 Support
endef

define KernelPackage/ppa-drv-gre
  SUBMENU:=MaxLinear
  TITLE:=Support GRE Acceleration in PPA
  DEPENDS:=+kmod-gre +IPV6:kmod-gre6 +kmod-ppa-drv
  KCONFIG:=CONFIG_PPA_BR_SESS_LEARNING=y \
	CONFIG_BRIDGE_NETFILTER=y
endef

define KernelPackage/ppa-drv-gre/description
  GRE/EoGRE Acceleration in PPA - Enable this to support GRE/EoGRE in PPA
endef

define KernelPackage/ppa-drv-vxlan
  SUBMENU:=MaxLinear
  TITLE:=Support VxLAN Acceleration in PPA
  DEPENDS:=+kmod-vxlan +kmod-ppa-drv
  KCONFIG:=CONFIG_PPA_BR_SESS_LEARNING=y \
	CONFIG_BRIDGE_NETFILTER=y
endef

define KernelPackage/ppa-drv-vxlan/description
  VxLAN Acceleration in PPA - Enable this to support VxLAN in PPA
endef

define KernelPackage/mac-violation-mirror
  SUBMENU:=MaxLinear
  TITLE:=MaxLinear Mac Violation Exception Handling Support
  DEPENDS:=@TARGET_intel_mips_xrx500 +kmod-ppa-drv
  FILES:=$(PKG_BUILD_DIR)/src/mac_violation_mirror/mac_violation_mirror.ko
  AUTOLOAD:=$(call AutoProbe,mac_violation_mirror)
endef

define KernelPackage/mac-violation-mirror/description
  Kernel support for handling Source Mac Violated Packets by Mirroring to PAE
  port for connectivity continuation
endef

define KernelPackage/ipv4-ipv6-counter-support
  SUBMENU:=MaxLinear
  TITLE:=IPv4 and IPv6 Counter Support in PPA
  DEPENDS:=+kmod-ppa-drv-grx500
  KCONFIG:=CONFIG_IPV4_IPV6_COUNTER_SUPPORT=y
endef

define KernelPackage/ipv4-ipv6-counter-support/description
  IPv4 and IPv6 Counter Support in PPA
endef

define KernelPackage/litepath-hwacc
  SUBMENU:=MaxLinear
  TITLE:=MaxLinear Litepath HW Acc Support
  DEPENDS:=@TARGET_intel_mips_xrx500 +kmod-ppa-drv +kmod-ppa-drv-accel \
	  +kmod-ppa-drv-grx500
  FILES:=$(PKG_BUILD_DIR)/src/platform/xrx500/hw_tcp_litepath.ko
  AUTOLOAD:=$(call AutoProbe,hw_tcp_litepath)
endef

define KernelPackage/litepath-hwacc/description
  Kernel support for tcp litepath hw acceleration
endef

define KernelPackage/atm_bond_drv
  SUBMENU:=MaxLinear
  TITLE:=ATM Bonding Driver
  DEPENDS:=@TARGET_intel_mips_xrx500 +@PACKAGE_kmod-vrx518_tc_drv:kmod-vrx518_tc_drv
  FILES:=$(PKG_BUILD_DIR)/src/bonding/ima_bonding_driver.ko
  AUTOLOAD:=$(call AutoProbe,ima_bonding_driver)
endef

define KernelPackage/atm_bond_drv/description
  ATM bonding driver with AAL5 segmentation and reassembly
endef

define KernelPackage/intel-qos
  TITLE:=MaxLinear support for QoS
  SUBMENU:=MaxLinear
  DEPENDS:=+kmod-sched
  KCONFIG:= CONFIG_INTEL_IPQOS=y \
        CONFIG_INTEL_IPQOS_ACCEL_DISABLE=y \
        CONFIG_INTEL_IPQOS_MARK_SKBPRIO=y \
        CONFIG_VLAN_8021Q_COPY_TO_EXTMARK=y \
        CONFIG_INTEL_IPQOS_MPE_DS_ACCEL=y \
        CONFIG_CLS_U32_EXTMARK=y
endef

define KernelPackage/intel-qos/description
  Kernel Support for QoS.
endef

define Package/atm_bond_cli
  SECTION:=utils
  CATEGORY:=MaxLinear
  TITLE:=ATM Bonding commandline debug tool
  DEPENDS:=@kmod-atm_bond_drv
  MAINTAINER:=MaxLinear Inc.
  URL:=http://www.maxlinear.com
endef

define Package/atm_bond_cli/description
  Command line debug tool for ATM bonding
endef

define Package/atm_bond_cli/install

ifneq ($(CONFIG_PACKAGE_atm_bond_cli),)
	$(INSTALL_DIR) $(1)/usr/sbin/;
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/bonding/src/tools/atm_bond_cli $(1)/usr/sbin/;
endif

endef

PKG_EXTRA_CFLAGS += -DPPA_SS_VERSION='\"$(PKG_SOURCE_VERSION)\"'
PKG_EXTRA_CFLAGS += -I$(STAGING_DIR)/usr/include/safeclib

ifneq ($(CONFIG_PACKAGE_kmod-ppa-drv-accel),)
  PKG_EXTRA_CFLAGS += -DCONFIG_PPA_ACCEL
  MAKE_OPTS += CONFIG_PPA_ACCEL=m
endif

ifeq ($(CONFIG_TARGET_intel_x86_lgm),y)
PKG_EXTRA_CFLAGS += -DWMM_QOS_CONFIG=1
PKG_EXTRA_CFLAGS += -DCONFIG_SOC_LGM
endif

MAKE_OPTS += M="$(PKG_BUILD_DIR)/src"

ifeq ($(CONFIG_TARGET_intel_mips),y)
ifneq ($(CONFIG_PACKAGE_kmod-ppa-drv-grx500),)
  PKG_EXTRA_CFLAGS += -DCONFIG_PPA_ACCEL -DCONFIG_PPA_TMU_HAL
  MAKE_OPTS += CONFIG_PPA_ACCEL=m CONFIG_PPA_TMU_HAL=m
endif
ifneq ($(CONFIG_PACKAGE_kmod-ppa-drv-grx500-mpe),)
  PKG_EXTRA_CFLAGS += -DCONFIG_PPA_MPE_HAL -DMIB_MODE_ENABLE
  MAKE_OPTS += CONFIG_PPA_MPE_HAL=m
endif
ifneq ($(CONFIG_PACKAGE_kmod-ppa-drv-prx300-mpe),)
  PKG_EXTRA_CFLAGS += -DCONFIG_PPA_MPE_HAL
  MAKE_OPTS += CONFIG_PPA_MPE_HAL=m
endif
ifneq ($(CONFIG_PACKAGE_kmod-litepath-hwacc),)
  PKG_EXTRA_CFLAGS += -DCONFIG_PPA_LITEPATH_HWACC
  MAKE_OPTS += CONFIG_PPA_LITEPATH_HWACC=m
endif
ifneq ($(CONFIG_PACKAGE_kmod-mac-violation-mirror),)
  PKG_EXTRA_CFLAGS += -DCONFIG_PPA_MAC_VIOLATION_MIRROR
  MAKE_OPTS += CONFIG_PPA_MAC_VIOLATION_MIRROR=m
endif
ifneq ($(CONFIG_PACKAGE_kmod-qos-mgr-drv),)
ifneq ($(CONFIG_TARGET_intel_mips_prx300),)
ifeq ($(CONFIG_LINUX_4_9),y)
  PKG_EXTRA_CFLAGS += -DCONFIG_QOS_MGR_TC_SUPPORT
  MAKE_OPTS += CONFIG_QOS_MGR_TC_SUPPORT=m
endif
endif
endif
ifneq ($(CONFIG_PACKAGE_kmod-atm_bond_drv),)
  MAKE_OPTS += CONFIG_PPA_ATM_BONDING=m
endif
ifneq ($(CONFIG_PACKAGE_atm_bond_cli),)
  PKG_EXTRA_CFLAGS += -I$(PKG_BUILD_DIR)/src/bonding/include/common
  PKG_EXTRA_CFLAGS += -DENABLE_IOCTL_DEBUG
endif
ifneq ($(CONFIG_PACKAGE_kmod-ppa-drv-prx300-gw),)
  PKG_EXTRA_CFLAGS += -DCONFIG_PRX300_HGU
  MAKE_OPTS += CONFIG_PRX300_HGU=m
endif
ifeq ($(CONFIG_TARGET_intel_mips_xrx500),y)
  PKG_EXTRA_CFLAGS += -DPLATFORM_XRX500
  export PLATFORM_XRX500=1
endif
PKG_EXTRA_CFLAGS += -Wno-uninitialized -Wno-switch-unreachable
endif

TARGET_CFLAGS += $(PKG_EXTRA_CFLAGS) -DCONFIG_PPA \
	-I$(LINUX_DIR)/include/ -I$(LINUX_DIR)/user_headers/include

define Build/Compile
	$(KERNEL_MAKE) $(MAKE_OPTS) \
		EXTRA_CFLAGS="$(PKG_EXTRA_CFLAGS)" \
		modules

	$(if $(CONFIG_PACKAGE_ppacmd), \
		+$(MAKE_VARS) \
		$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR)/tools/ppacmd \
		$(MAKE_FLAGS) \
		all \
	)

	$(if $(CONFIG_PACKAGE_qoscmd), \
		+$(MAKE_VARS) \
		$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR)/tools/qoscmd \
		$(MAKE_FLAGS) \
		all \
	)

	$(if $(CONFIG_PACKAGE_atm_bond_cli), \
		+$(MAKE_VARS) \
		$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR)/src/bonding/src/tools \
		$(MAKE_FLAGS) \
		all \
	)
endef

define Build/InstallDev

ifneq ($(CONFIG_PACKAGE_kmod-atm_bond_drv),)
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/src/bonding/include/common/ima_atm_bond_ioctl.h $(1)/usr/include/
endif

endef

$(eval $(call KernelPackage,ppa-drv-lgm))
$(eval $(call KernelPackage,ppa-drv-grx500))
$(eval $(call KernelPackage,qos-mgr-drv))
$(eval $(call KernelPackage,ppa-drv-prx300-gw))
$(eval $(call KernelPackage,ppa-drv-grx500-mpe))
$(eval $(call KernelPackage,ppa-drv-prx300-mpe))
$(eval $(call KernelPackage,ppa-drv-mpe-ip97))
$(eval $(call KernelPackage,ppa-drv-gre))
$(eval $(call KernelPackage,ppa-drv-vxlan))
$(eval $(call KernelPackage,ppa-drv-accel))
$(eval $(call KernelPackage,ppa-drv))
$(eval $(call KernelPackage,ppa-drv-stack-al))
$(eval $(call KernelPackage,mac-violation-mirror))
$(eval $(call KernelPackage,litepath-hwacc))
$(eval $(call KernelPackage,ipv4-ipv6-counter-support))
$(eval $(call KernelPackage,atm_bond_drv))
$(eval $(call BuildPackage,atm_bond_cli))
$(eval $(call BuildPackage,ppacmd))
$(eval $(call BuildPackage,qoscmd))
$(eval $(call KernelPackage,intel-qos))
