
UGW_PACKAGES:= \
        base-files-ugw dbtool webcgi websockets ugw-devm urlfilterd \
        klish klish-xml-files servd csd polld libcal lighttpd \
        libdeviceinfo libdyndns libethservices sys_eth libfirewallnat libipv6 libuciframework\
        liblanservices liblogd libmanagementserver libmultiwan libnetwork \
        libqos libsysservices libupgrade libusbhosts libuser \
        libwms libhsfapi hscli sl_upnp libmcastservices libugwhelper libdiagnostics \
        wwan libcellwan libumbim firewall qoscli secure_upgrade_utils

UGW_PACKAGES_IOT:=\
        iotivity iotivity-cpp iotivity-resource-directory-lib iotivity-oic-middle iotivity-resource-container-libiotivity-resource-container-sample \
        iotivity-resource-container-hue iotivity-example-garage iotivity-example-simple iotivity_DEBUG iotivity_SECURE

UGW_PACKAGES_SDL:=$(UGW_PACKAGES) base-files-ugw-sdl-lite

UGW_PACKAGES_SEC:=base-files-sdl base-files-sdl-ugw-services ugw-devm lighttpd sys_eth firewall dbtool webcgi websockets \
	urlfilterd secure_upgrade_utils qoscli system_service_cli pecostat_interaptiv kmod-pecoevent libuciframework \
	liblanservices libuser libupgrade sl_upnp libmcastservices libfirewallnat libethservices libipv6 libnetwork \
	libmanagementserver libdeviceinfo liblogd libqos libsysservices libdiagnostics libusbhosts polld \
	base-files-sdl-lxc

UGW_PACKAGES_SEC_DEBUG:= csd-debug caltest csd-capitest csd-csdutil_dbg pad ltq_hanfun_agent ltq-pm-util \
	mcastcli libmcastservices-dbg cap_provide fapi_qos-dbg-conf

UGW_PACKAGE_OVL_WHITELIST_SEC:=base-files-sdl-ovl-whitelist

VOIP_PACKAGES_SEC:= libvoip ltq-voice-tapi ltq-voice-lib-nlt ltq-voice-sdd-mbx ltq-voice-vmmc-xrx500 ltq-voice-vmmc-xrx500-coef \
	ltq-voice-vmmc-xrx500-firmware kmod-voice-tapi kmod-voice-tapi-kpi2udp kmod-voice-tapi-sdd-mbx kmod-voice-vmmc-xrx500

DECT_PACKAGES_SEC:= ltq-dect

DEBUG_PACKAGES:=caltest csd-capitest csd-debug csd-csdutil_dbg pad ltq-pm-util mcastcli libmcastservices-dbg fapi_qos-dbg-conf

DECT_PACKAGES_SEC_DEBUG:=ltq_hanfun_agent

OWRT_PACKAGES:=luci firewall luci-theme-openwrt base-files-owrt \
        owrt-qos-scripts owrt-mcast-scripts owrt-sys-scripts owrt-dsl-scripts owrt-ppa-scripts

AX_PACKAGES:=base-files-sdl-axepoint kmod-gphy_event

GRX_PACKAGES:=base-files-sdl-grx500

PRX_SDL_SYS_PACKAGES:=base-files-sdl-prx321-eth

DSL_CPE_PACKAGES:=dsl-vr11-firmware-xdsl kmod-dsl-cpe-mei-vrx kmod-vrx518_ep \
        kmod-vrx518_tc_drv vrx518_aca_fw vrx518_ppe_fw ppp-mod-pppoa

DSL_CPE_PACKAGES_R5:=dsl-vr11-firmware-xdsl-r5 kmod-dsl-cpe-mei-vrx kmod-vrx518_ep \
        kmod-vrx518_tc_drv vrx518_aca_fw vrx518_ppe_fw ppp-mod-pppoa

DSL_CPE_PACKAGES_DEBUG:=dsl-cpe-api-vrx-dbg dsl-cpe-control-vrx-dbg dsl-cpe-mei-vrx-dbg dsl-cpe-dti-agent

DSL_CPE_PACKAGES_RELEASE:=dsl-cpe-api-vrx dsl-cpe-control-vrx dsl-cpe-mei-vrx

DSL_CPE_UGW_PACKAGE:=sl-dsl-cpe dsl-cpe-fapi ugw-atm-oam

DSL_CPE_GFAST_PACKAGES:=dsl-gfast-drv-pciep dsl-vrx618-firmware \
	kmod-dsl-gfast-drv-pciep kmod-dsl-gfast-drv-pmi dsl-gfast-drv-dp \
	kmod-dsl-gfast-drv-dp ppp-mod-pppoa sl-dsl-cpe-vrx618

DSL_CPE_GFAST_PACKAGES_PRX:=dsl-gfast-drv-pciep dsl-vrx618-firmware \
	kmod-dsl-gfast-drv-pciep kmod-dsl-gfast-drv-pmi dsl-gfast-drv-dp-prx \
	kmod-dsl-gfast-drv-dp-prx kmod-directconnect-dp sl-dsl-cpe-vrx618 dsl-gfast-api-vrx618 \
	ppp-mod-pppoa 

DSL_CPE_GFAST_PACKAGES_DEBUG:=dsl-gfast-api-vrx618-dbg dsl-gfast-drv-pmi-dbg dsl-gfast-init-dbg dti-pmi

DSL_CPE_GFAST_PACKAGES_RELEASE:=dsl-gfast-api-vrx618 dsl-gfast-drv-pmi dsl-gfast-init

DSL_CPE_GFAST_PACKAGES_RELEASE_BND:=dsl-gfast-api-vrx618 dsl-gfast-drv-pmi dsl-gfast-init-bnd

GFAST_CO_PACKAGES:=dsl-vnx101-firmware dsl-gfast-api-vnx101 dsl-gfast-init-co

WAV500_PACKAGES:= ltq-wlan-wave_5_x ltq-wlan-wave_5_x-rflib kmod-Lantiq-wlan-wave-support-5_x

WAV500_UGW_PACKAGES:= fapi_wlan_vendor_wave fapi_wlan_common libwlan

WAV600_PACKAGES:=ltq-wlan-wave_6x iwlwav-driver kmod-iwlwav-driver-kernel iwlwav-hostap iwlwav-iw iwlwav-tools ltq-wlan-wave6x-rflib kmod-lantiq-wlan-wave-support_6x crda_wave_6x

WAV600_UGW_PACKAGES:=libwlan_6x fapi_wlan_vendor_wave_6x fapi_wlan_common_6x

WAV600_PACKAGES_UCI:=ltq-wlan-wave_6x-uci kmod-iwlwav-driver-uci iwlwav-hostap-uci iwlwav-iw iwlwav-tools ltq-wlan-wave6x-rflib kmod-lantiq-wlan-wave-support_6x crda_wave_6x

WAV600_UGW_PACKAGES_UCI:=libwlan_6x-uci swpal_6x-uci dwpal_6x-uci wav_api_6x-uci

WAV600_PACKAGES_UCI_DEBUG:=ltq-wlan-wave_6x-uci-debug kmod-iwlwav-driver-uci-debug \
	iwlwav-hostap-uci-debug iwlwav-iw-debug iwlwav-tools-debug ltq-wlan-wave6x-rflib \
	kmod-lantiq-wlan-wave-support_6x crda_wave_6x

WAV600_UGW_PACKAGES_UCI_DEBUG:=libwlan_6x-uci swpal_6x-uci-debug dwpal_6x-uci-debug wav_api_6x-uci-debug

VOIP_PACKAGES:= libvoip ltq-voip

DECT_PACKAGES:= ltq-dect

PON_ETH_PACKAGES:= pon-gpon-firmware-b pon-lib-daemon kmod-pon-mbox-drv \
	firewall kmod-i2c-intel_mips kmod-eeprom-at24

PON_VOICE_PACKAGES:=ltq-voice-vmmc-xrx500 ltq-voice-vmmc-prx300-coef \
	ltq-voice-tapi ltq-voice-tapi-lib-country ltq-voice-sdd-mbx \
	kmod-voice-tapi kmod-voice-tapi-kpi2udp kmod-voice-tapi-sdd-mbx \
	kmod-voice-vmmc-xrx500 ltq-voice-vmmc-prx300-firmware kmod-spi-intel_mips-grx500

PON_BASE_PACKAGES:= \
	gpon-omci-onu \
	kmod-pon-mbox-drv kmod-pon-mbox-drv-eth kmod-pon-mcc-drv \
	pon-base-files pon-base-files-common \
	pon-gpon-firmware-b pon-xpon-firmware-b \
	pon-lib pon-net-lib pon-mcc-lib pon-img-lib \
	libnl-core libnl-genl libnl-route \
	kmod-pps kmod-ptp kmod-pon-mbox-drv-ptp pon-tod \
	firewall kmod-i2c-intel_mips kmod-eeprom-at24 \
	kmod-qos-mgr-drv

PON_DEBUG_PACKAGES:= \
	pon-tools gpon-libs gpon-libs-linux \
	pon-mbox-drv pon-lib-cli pon-mcc-cli kmod-pon-mcc-drv-chk \
	ip-tiny tc ethtool tcpdump-mini testptp kmod-hwmon-ina2xx

PON_SFP_PACKAGES:=base-files-ieee1588 gpon-sfp-eeprom gpon-sfp-eeprom-ponip \
	kmod-pon-sfp-i2c linuxptp testptp

PON_SFU_PACKAGES:=ltq-gphy-fw-prx3xx $(PON_VOICE_PACKAGES)

PON_HGU_PACKAGES:=ltq-gphy-fw-prx3xx kmod-intel_pon_hgu_vuni

MPE_ACCEL_PACKAGES:=mpe_fw_prx300 kmod-ppa-drv-accel kmod-ppa-drv-prx300-gw \
	kmod-ppa-drv-prx300-mpe

COMMON_USER_FEATURES_PACKAGES:=sl_client_mode sl_guest_access

RT_PACKAGES_DEBUG:=openvpn-openssl kmod-cryptodev
