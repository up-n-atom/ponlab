include $(TOPDIR)/rules.mk

include $(INCLUDE_DIR)/kernel.mk
SUBTARGET_SUFFIX:=$(shell echo $(subst .,_,$(subst -,_,$(subst /,_,$(SUBTARGET)))) | cut -d_ -f2-)

PKG_NAME:=ppa-drv
PKG_VERSION:=1.0
PKG_RELEASE=$(PKG_SOURCE_VERSION)

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=ssh://git@gts-chd.intel.com:29418/sw_ugw/ppa_drv.git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=890ad290019f82f3763da7dff524fe327a9ec3fc
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz
PKG_MAINTAINER:=Intel

PKG_LICENSE:=GPL-2.0
PKG_LICENSE_FILES:=
PKG_BUILD_PARALLEL:=1
PKG_IS_KERNEL_MODULE:=y
PKG_DEFAULT_CUSTOM_SOURCE_DIR:=$(TOPDIR)/ppa-drv

include $(INCLUDE_DIR)/package-version-override.mk
include $(INCLUDE_DIR)/package.mk

PKG_CONFIG_DEPENDS := CONFIG_PACKAGE_kmod-nf-conntrack

define add_ppa_config
MAKE_FLAGS += $(if $(CONFIG_PACKAGE_kmod-$(1)),$(patsubst CONFIG_%,CONFIG_%=1,$(2)))
PKG_EXTRA_KCONFIG += $(if $(CONFIG_PACKAGE_kmod-$(1)),$(patsubst CONFIG_%,CONFIG_%=m,$(2)))
endef

define Package/ppacmd
 SECTION:=utils
 CATEGORY:=Intel
 TITLE:=Packet Protocol Acceleration Commandline tool
 DEPENDS:=+kmod-ppa-drv +libsafec3
 MAINTAINER:=Intel
 URL:=http://www.intel.com
endef

define Package/ppacmd/description
 Command line tool for PPA acceleration
endef

define Package/ppacmd/install
        $(INSTALL_DIR) $(1)/usr/sbin/; \
        $(INSTALL_BIN) $(PKG_BUILD_DIR)/tools/ppacmd/ppacmd $(1)/usr/sbin/;
endef

define KernelPackage/ppa-drv
 SUBMENU:=Intel
 TITLE:=Packet Protocol Acceleration Support
 DEPENDS:= +kmod-nf-conntrack 					\
	+kmod-ppa-drv-stack-al					\
	+TARGET_intel_mips_xrx500:kmod-ppa-drv-grx500		\
	+TARGET_intel_mips_prx300:kmod-ppa-drv-prx300	\
	+TARGET_intel_x86_lgm:kmod-ppa-drv-lgm			\
	PACKAGE_kmod-ppa-drv-grx500-mpe:kmod-ppa-drv-grx500-mpe	\
	PACKAGE_kmod-ppa-drv-prx300-mpe:kmod-ppa-drv-prx300-mpe	\
	+kmod-nf-ipt

 MAINTAINER:=Intel
 KCONFIG:= \
	CONFIG_PPA=y \
	CONFIG_PPA_IPTABLE_EVENT_HANDLING=y
 
 FILES:= \
	$(PKG_BUILD_DIR)/src/ppa_api/ppa_api.ko

 AUTOLOAD:=$(call AutoProbe, ppa_api)
 MENU:=1
endef

define KernelPackage/ppa-drv/description
 Kernel support for PPA acceleration
endef

define KernelPackage/ppa-drv/config
$(call Package/$(PKG_NAME_PREF)$(PKG_NAME)/override_version)
$(call Package/$(PKG_NAME_PREF)$(PKG_NAME)/override_source_path)
endef

define KernelPackage/ppa-drv-accel
 SUBMENU:=Intel
 TITLE:=Packet Protocol Acceleration Support
 DEPENDS:=+kmod-ppa-drv
 MAINTAINER:=Intel
 KCONFIG:= \
  	CONFIG_PPA_API_SW_FASTPATH=y \
	CONFIG_PPA_TCP_LITEPATH=y

 FILES:= \
	$(PKG_BUILD_DIR)/src/ppa_api/ppa_api_sw_accel_mod.ko

 AUTOLOAD:=$(call AutoProbe, ppa_api_sw_accel_mod)
endef

$(eval $(call add_ppa_config,ppa-drv-accel, \
	CONFIG_PPA_ACCEL \
))

define KernelPackage/ppa-drv-accel/description
 Kernel support for PPA acceleration
endef

define KernelPackage/ppa-drv-stack-al
 SUBMENU:=Intel
 TITLE:=Packet Protocol Acceleration Stack Adaptation Layer
 DEPENDS:=+kmod-nf-conntrack +kmod-veth
 MAINTAINER:=Intel
 KCONFIG:= \
	CONFIG_PPA_QOS=n \
 	CONFIG_PPA_API_DIRECTPATH=n \
	CONFIG_PPA_TMU_MIB_SUPPORT=n \
  	CONFIG_PPA_API_SW_FASTPATH=n \
	CONFIG_PPA_BR_MAC_LEARNING=n \
	CONFIG_PPA_RT_SESS_LEARNING=n \
  	CONFIG_PPA_BR_SESS_LEARNING=n \
	CONFIG_PPA_EXT_PKT_LEARNING=n \
	CONFIG_PPA_API_DIRECTCONNECT=n \
	CONFIG_PPA_TCP_LITEPATH=n
 
 FILES:= \
	$(PKG_BUILD_DIR)/src/ppa_api/ppa_drv_stack_al.ko

 AUTOLOAD:=$(call AutoProbe,ppa_drv_stack_al)
endef

define KernelPackage/ppa-drv-stack-al/description
 PPA acceleration Stack Adaptation Layer
endef

define KernelPackage/ppa-drv-grx500
 SUBMENU:=Intel
 TITLE:=Packet Protocol Acceleration Support for xRX500
 DEPENDS:=@TARGET_intel_mips_xrx500 +kmod-ppa-drv-stack-al
 MAINTAINER:=Intel
 KCONFIG:= \
	CONFIG_PPA_QOS=y \
	CONFIG_PPA_BR_MAC_LEARNING=y \
	CONFIG_PPA_RT_SESS_LEARNING=y \
	CONFIG_PPA_TMU_MIB_SUPPORT=y \
	CONFIG_PPA_API_DIRECTCONNECT=y \
	CONFIG_PPA_API_DIRECTPATH=y

 FILES:= \
	$(PKG_BUILD_DIR)/src/platform/xrx500/ltq_pae_hal.ko \
	$(PKG_BUILD_DIR)/src/platform/xrx500/ltq_tmu_hal_drv.ko \

 AUTOLOAD:=$(call AutoProbe,ltq_pae_hal ltq_tmu_hal_drv)
endef

$(eval $(call add_ppa_config,ppa-drv-grx500, \
	CONFIG_PPA_TMU_HAL \
	CONFIG_NAT_LOOP_BACK \
))

define KernelPackage/ppa-drv-grx500/description
  PPA GRX500 Acceleration Support
endef

define KernelPackage/ppa-drv-prx300
 SUBMENU:=Intel
 TITLE:=Packet Protocol Acceleration Support for PRX300
 DEPENDS:=@TARGET_intel_mips_prx300 +kmod-ppa-drv-stack-al +kmod-sched-flower +kmod-sched-act-extra
 MAINTAINER:=Intel
 KCONFIG:= \
	CONFIG_PPA_QOS=y  \
	CONFIG_PPA_API_DIRECTCONNECT=y

 FILES:= \
	$(PKG_BUILD_DIR)/src/qos/qos_hal_drv.ko

 AUTOLOAD:=$(call AutoProbe,qos_hal_drv)

endef

$(eval $(call add_ppa_config,ppa-drv-prx300, \
	CONFIG_PPA_QOS_HAL \
	CONFIG_QOS_MGR_TC_SUPPORT \
))

define KernelPackage/ppa-drv-prx300/description
  PPA Acceleration Support for PRX300
endef

define KernelPackage/ppa-drv-grx750
 SUBMENU:=Intel
 TITLE:=Packet Protocol Acceleration Support for xRX750
 DEPENDS:=@TARGET_intel_x86_grx750 +kmod-ppa-drv-stack-al
 MAINTAINER:=Intel
 KCONFIG:= \
	CONFIG_PPA_PUMA7=y
endef

define KernelPackage/ppa-drv-grx750/description
  PPA GRX750 Acceleration Support
endef

define KernelPackage/ppa-drv-lgm
 SUBMENU:=Intel
 TITLE:=Packet Protocol Acceleration Support for LGM
 DEPENDS:=@TARGET_intel_x86_lgm +kmod-ppa-drv-stack-al
 MAINTAINER:=Intel
 KCONFIG:= \
	CONFIG_PPA_QOS=y \
	CONFIG_PPA_EXT_PKT_LEARNING=y \
	CONFIG_PPA_API_DIRECTCONNECT=y

 FILES:= \
	$(PKG_BUILD_DIR)/src/platform/lgm/lgm_pp_hal.ko \
	$(PKG_BUILD_DIR)/src/qos/qos_hal_drv.ko

 AUTOLOAD:=$(call AutoProbe,lgm_pp_hal qos_hal_drv)
endef

$(eval $(call add_ppa_config,ppa-drv-lgm, \
	CONFIG_PPA_QOS_HAL \
	CONFIG_PPA_DBG \
))

define KernelPackage/ppa-drv-lgm/description
  PPA LGM Acceleration Support
endef

define KernelPackage/ppa-drv-grx500-mpe
 SUBMENU:=Intel
 TITLE:=Packet Protocol Acceleration Multi-Protocol Engine for GRX500
 DEPENDS:=@TARGET_intel_mips_xrx500 +kmod-ppa-drv-grx500
 MAINTAINER:=Intel
 
 FILES:= \
	$(PKG_BUILD_DIR)/src/platform/xrx500/ltq_mpe_hal_drv.ko

 AUTOLOAD:=$(call AutoProbe, ltq_mpe_hal_drv)
endef

$(eval $(call add_ppa_config,ppa-drv-grx500-mpe, \
	CONFIG_PPA_MPE_HAL \
))

define KernelPackage/ppa-drv-grx500-mpe/description
  PPA GRX500 Packet Protocol Acceleration Multi-Protocol Engine module
endef

# Install additional files from this.
define KernelPackage/ppa-drv-grx500-mpe/install
		$(INSTALL_DIR) $(1)/lib/firmware; \
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/platform/xrx500/mpe_fw_be.img $(1)/lib/firmware/;
endef


define KernelPackage/ppa-drv-prx300-mpe
 SUBMENU:=Intel
 TITLE:=Packet Protocol Acceleration Multi-Protocol Engine for PRX300
 DEPENDS:=@TARGET_intel_mips_prx300 +kmod-ppa-drv-prx300 +mpe_fw_prx300
 MAINTAINER:=Intel
 FILES:= \
	$(PKG_BUILD_DIR)/src/platform/prx300/ltq_mpe_hal_drv.ko

 AUTOLOAD:=$(call AutoProbe, ltq_mpe_hal_drv)
endef

$(eval $(call add_ppa_config,ppa-drv-prx300-mpe, \
	CONFIG_PPA_MPE_HAL \
))

define KernelPackage/ppa-drv-prx300-mpe/description
  PPA Packet Protocol Acceleration Multi-Protocol Engine module for PRX300
endef

# Install  additional files from this.
define KernelPackage/ppa-drv-prx300-mpe/install
endef

define KernelPackage/ppa-drv-mpe-ip97
 SUBMENU:=Intel
 TITLE:=LTQ PPA MPE IP97 Support for xRX500
 DEPENDS:=@TARGET_intel_mips_xrx500 @PACKAGE_kmod-ppa-drv +kmod-eip97
 KCONFIG:= \
	CONFIG_PPA_MPE_IP97=y
endef

define KernelPackage/ppa-drv-mpe-ip97/description
  PPA GRX500 MPE IP97 Support
endef

define KernelPackage/ppa-drv-grx500-eth-bond
 SUBMENU:=Intel
 TITLE:=Ethernet Bonding support for xRX500
 DEPENDS+=@TARGET_intel_mips_xrx500 @PACKAGE_kmod-ppa-drv-grx500
 KCONFIG+= \
	CONFIG_HGU_BONDING=y \
	CONFIG_LTQ_PPA_ETH_BOND
        
 $(if $(wildcard $(LINUX_DIR)/drivers/net/lantiq_ppa),PPAPATH:="lantiq_ppa",PPAPATH:="lantiq_ppa/src")
 FILES := \
	$(LINUX_DIR)/drivers/net/$(PPAPATH)/platform/$(SUBTARGET_SUFFIX)/ltq_eth_bond_dp.ko
 
endef

define KernelPackage/ppa-drv-grx500-eth-bond/description
  PPA GRX500 - HGU Board ethernet bonding dp enable pluggable module
endef

define KernelPackage/ppa-drv-grx500-eth-bond/install
        $(INSTALL_DIR) $(1)/etc/init.d/; \
        $(INSTALL_BIN) $(LINUX_DIR)/drivers/net/lantiq_ppa/platform/xrx500/eth_bond_init.sh $(1)/etc/init.d/;
endef

define KernelPackage/ppa-drv-grx500-eth-rgmii
 SUBMENU:=Intel
 TITLE:=ETH WAN RGMII mode enable for xRX500
 DEPENDS:=@PACKAGE_kmod-ppa-drv-grx500-eth-bond
 KCONFIG:= \
	CONFIG_LTQ_PPA_ETHWAN_RGMII=y
endef

define KernelPackage/ppa-drv-grx500-eth-rgmii/description
  PPA GRX500 ETH WAN RGMII mode Support
endef

define KernelPackage/ppa-drv-gre
  SUBMENU:=Intel
  TITLE:=Support GRE Acceleration in PPA
  DEPENDS:=+kmod-gre +kmod-gre6 +kmod-ppa-drv
  KCONFIG:= \
	CONFIG_PPA_BR_SESS_LEARNING=y \
	CONFIG_BRIDGE_NETFILTER=y
endef

define KernelPackage/ppa-drv-gre/description
  GRE/EoGRE Acceleration in PPA - Enable this to support GRE/EoGRE in PPA
endef

define KernelPackage/mac-violation-mirror
  SUBMENU:=Intel
  TITLE:=Intel Mac Violation Exception Handling Support
  DEPENDS:=@TARGET_intel_mips_xrx500 +kmod-ppa-drv
  KCONFIG:= \
	CONFIG_MAC_VIOLATION_MIRROR=m
  FILES:= \
	$(PKG_BUILD_DIR)/src/mac_violation_mirror/mac_violation_mirror.ko

 AUTOLOAD:=$(call AutoProbe,mac_violation_mirror)
endef

$(eval $(call add_ppa_config,mac-violation-mirror, \
	CONFIG_MAC_VIOLATION_MIRROR \
))

define KernelPackage/mac-violation-mirror/description
 Kernel support for handling Source Mac Violated Packets by Mirroring to PAE port for connectivity continuation
endef

define KernelPackage/ipv4-ipv6-counter-support
  SUBMENU:=Intel
  TITLE:=IPv4 and IPv6 Counter Support in PPA
  DEPENDS:=+kmod-ppa-drv
  KCONFIG:= \
	CONFIG_IPV4_IPV6_COUNTER_SUPPORT=y
endef

define KernelPackage/ipv4-ipv6-counter-support/description
  IPv4 and IPv6 Counter Support in PPA
endef

define KernelPackage/litepath-hwacc
  SUBMENU:=Intel
  TITLE:=Intel Litepath HW Acc Support
  DEPENDS:=@TARGET_intel_mips_xrx500 +kmod-ppa-drv +kmod-ppa-drv-accel +kmod-ppa-drv-grx500
  KCONFIG:= \
	CONFIG_LITEPATH_HWACC=m
  FILES:= \
	$(PKG_BUILD_DIR)/src/litepath_hwacc/hw_litepath.ko

  AUTOLOAD:=$(call AutoProbe,hw_litepath)
endef

$(eval $(call add_ppa_config,litepath-hwacc, \
	CONFIG_LITEPATH_HWACC \
))

define KernelPackage/litepath-hwacc/description
 Kernel support for tcp litepath hw acceleration
endef

define KernelPackage/phy-err-counter
  SUBMENU:=Intel
  TITLE:=Intel Phy Err Counter Support
  DEPENDS:=@TARGET_intel_mips_xrx500 +kmod-intel_eth_xrx500_fw
  KCONFIG:= \
	CONFIG_PHY_ERR_CTR=m
  FILES:= \
	$(PKG_BUILD_DIR)/src/platform/xrx500/phy_err_counter.ko

  AUTOLOAD:=$(call AutoProbe,xrx500_phy_fw phy_err_counter)
endef

$(eval $(call add_ppa_config,phy-err-counter, \
	CONFIG_PHY_ERR_CTR \
))

define KernelPackage/phy-err-counter/description
 Kernel support for phy level code violated packets
endef

#############################################################################
PKG_EXTRA_CFLAGS := $(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=m,%,$(filter %=m,$(PKG_EXTRA_KCONFIG))))

ifneq ($(CONFIG_PACKAGE_kmod-ipv4-ipv6-counter-support),)
   PKG_EXTRA_CFLAGS +=-DMIB_MODE_ENABLE
   export MIB_MODE_ENABLE=1
endif

PKG_EXTRA_CFLAGS += $(if $(CONFIG_PACKAGE_kmod-nf-conntrack),-DCONFIG_NF_CONNTRACK) -Wno-uninitialized -Wno-switch-unreachable

TARGET_CFLAGS += $(PKG_EXTRA_CFLAGS) -DCONFIG_PPA -I$(LINUX_DIR)/include/ -I$(LINUX_DIR)/user_headers/include $(if $(CONFIG_PACKAGE_kmod-ipv4-ipv6-counter-support),-DPPA_IP_COUNTER=1)

MAKE_FLAGS += \
        $(if $(CONFIG_TARGET_x86_puma_puma7),PLATFORM_PUMA7=1) \
        $(if $(CONFIG_PACKAGE_kmod-ppa-drv-grx500),PLATFORM_XRX500=1)

define Build/Compile
        $(MAKE) -C "$(LINUX_DIR)" \
                ARCH="$(LINUX_KARCH)" \
                CROSS_COMPILE="$(TARGET_CROSS)" \
                SUBDIRS="$(PKG_BUILD_DIR)/src" \
		$(PKG_EXTRA_KCONFIG) \
		EXTRA_CFLAGS="$(PKG_EXTRA_CFLAGS)" \
                V="$(if $(V),1)" \
                modules
	$(if $(CONFIG_PACKAGE_ppacmd), \
	+$(MAKE_VARS) \
	$(MAKE) $(PKG_JOBS) -C $(PKG_BUILD_DIR)/tools/ppacmd \
		$(MAKE_FLAGS) \
		all \
	)
endef

define Build/Clean	
	if [ -d $(PKG_BUILD_DIR) ]; then $(MAKE) -C "$(LINUX_DIR)" SUBDIRS="$(PKG_BUILD_DIR)/src" clean; \
        $(RM) -r $(PKG_BUILD_DIR)/ipkg-$(BOARD); \
	$(if $(CONFIG_PACKAGE_ppacmd), \
        $(MAKE) -C $(PKG_BUILD_DIR)/tools/ppacmd clean \
	); fi
endef

$(eval $(call KernelPackage,ppa-drv-grx500))
$(eval $(call KernelPackage,ppa-drv-grx750))
$(eval $(call KernelPackage,ppa-drv-prx300))
$(eval $(call KernelPackage,ppa-drv-lgm))
$(eval $(call KernelPackage,ppa-drv-grx500-mpe))
$(eval $(call KernelPackage,ppa-drv-prx300-mpe))
$(eval $(call KernelPackage,ppa-drv-mpe-ip97))
$(eval $(call KernelPackage,ppa-drv-grx500-eth-bond))
$(eval $(call KernelPackage,ppa-drv-grx500-eth-rgmii))
$(eval $(call KernelPackage,ppa-drv-gre))
$(eval $(call KernelPackage,ppa-drv-accel))
$(eval $(call KernelPackage,ppa-drv))
$(eval $(call KernelPackage,ppa-drv-stack-al))
$(eval $(call KernelPackage,mac-violation-mirror))
$(eval $(call KernelPackage,ipv4-ipv6-counter-support))
$(eval $(call KernelPackage,litepath-hwacc))
$(eval $(call KernelPackage,phy-err-counter))
$(eval $(call BuildPackage,ppacmd))
