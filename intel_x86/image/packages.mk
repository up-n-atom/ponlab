
UGW_PACKAGES:= base-files-ugw libusbhosts ugw-devm kmod-gpio-button-hotplug

UGW_DIAG_DSL_PACKAGES:=libdiagnostics-dsl

UGW_DIAG_PACKAGES:=libdiagnostics

UGW_PACKAGES_IOT:=\
        iotivity iotivity-cpp iotivity-resource-directory-lib iotivity-oic-middle iotivity-resource-container-libiotivity-resource-container-sample \
        iotivity-resource-container-hue iotivity-example-garage iotivity-example-simple iotivity_DEBUG iotivity_SECURE

UGW_PACKAGES_SDL:=$(UGW_PACKAGES) base-files-ugw-sdl-lite

UGW_PACKAGES_SEC:=base-files-sdl system_service_cli ugw-devm

UGW_PACKAGES_SEC_DEBUG:= pm-util cap_provide

UGW_PACKAGE_OVL_WHITELIST_SEC:=base-files-sdl-ovl-whitelist

VOIP_PACKAGES_DXS_DEBUG:= \
        kmod-voice-lgm kmod-voice-lgm-cru kmod-voice-lgm-irq voice-c55-firmware \
        kmod-voice-lgm-no-slic kmod-voice-ni kmod-voice-tapi voice-tapi-lib-nlt \
        kmod-voice-duslic_xs voice-duslic_xs-coef voice-duslic_xs-firmware \
        voice-tapidemo-dxs voice-sipua-dxs voice-tapi-cli voice-tapidump \
        kmod-voice-lgm-c55

VOIP_PACKAGES_NO_SLIC:= \
        kmod-voice-lgm kmod-voice-lgm-cru kmod-voice-lgm-irq voice-c55-firmware \
        kmod-voice-lgm-no-slic kmod-voice-ni kmod-voice-tapi kmod-voice-lgm-c55

DEBUG_PACKAGES:=caltest csd-capitest csd-debug csd-csdutil_dbg pad pm-util mcastcli libmcastservices-dbg

PM_DEBUG_PACKAGES:=pm-util pciutils stress
PM_PACKAGES:=ppmd-bin pciutils
PCIUTILS_PACKAGE:=pciutils

DSL_CPE_PACKAGES:=dsl-vr11-firmware-xdsl kmod-dsl-cpe-mei-vrx kmod-vrx518_ep \
        kmod-vrx518_tc_drv vrx518_aca_fw vrx518_ppe_fw ppp-mod-pppoa

DSL_CPE_PACKAGES_R5:=dsl-vr11-firmware-xdsl-r5 kmod-dsl-cpe-mei-vrx kmod-vrx518_ep \
        kmod-vrx518_tc_drv vrx518_aca_fw vrx518_ppe_fw ppp-mod-pppoa

DSL_CPE_PACKAGES_DEBUG:=dsl-cpe-api-vrx-dbg dsl-cpe-control-vrx-dbg dsl-cpe-mei-vrx-dbg

DSL_CPE_PACKAGES_RELEASE:=dsl-cpe-api-vrx dsl-cpe-control-vrx dsl-cpe-mei-vrx

DSL_CPE_UGW_PACKAGE:=sl-dsl-cpe ugw-atm-oam

DSL_CPE_GFAST_PACKAGES:=dsl-gfast-drv-pciep dsl-vrx618-firmware \
	kmod-dsl-gfast-drv-pciep kmod-dsl-gfast-drv-pmi dsl-gfast-drv-dp \
	kmod-dsl-gfast-drv-dp ppp-mod-pppoa sl-dsl-cpe-vrx618

# same set of modules applies to PRX and URX devices
DSL_CPE_GFAST_PACKAGES_PRX:=dsl-gfast-drv-pciep dsl-vrx618-firmware \
	kmod-dsl-gfast-drv-pciep kmod-dsl-gfast-drv-pmi \
	dsl-gfast-drv-dp-prx kmod-dsl-gfast-drv-dp-prx ppp-mod-pppoa \
	kmod-directconnect-dp sl-dsl-cpe sl-dsl-cpe-vrx618 dsl-gfast-api-vrx618

DSL_CPE_GFAST_PACKAGES_DEBUG:=dsl-gfast-api-vrx618-dbg dsl-gfast-drv-pmi-dbg dsl-gfast-init-dbg dti-pmi

DSL_CPE_GFAST_PACKAGES_RELEASE:=dsl-gfast-api-vrx618 dsl-gfast-drv-pmi dsl-gfast-init

GFAST_CO_PACKAGES:=dsl-vnx101-firmware dsl-gfast-api-vnx101 dsl-gfast-init-co

WAV500_PACKAGES:= ltq-wlan-wave_5_x ltq-wlan-wave_5_x-rflib kmod-Lantiq-wlan-wave-support-5_x

WAV500_UGW_PACKAGES:= fapi_wlan_vendor_wave fapi_wlan_common libwlan

WAV600_PACKAGES:=ltq-wlan-wave_6x iwlwav-driver kmod-iwlwav-driver-kernel iwlwav-hostap iwlwav-iw iwlwav-tools ltq-wlan-wave6x-rflib kmod-lantiq-wlan-wave-support_6x crda_wave_6x

WAV600_UGW_PACKAGES:=libwlan_6x fapi_wlan_vendor_wave_6x fapi_wlan_common_6x

WAV600_PACKAGES_UCI:=iwlwav-firmware kmod-iwlwav-driver-uci iwlwav-iw ltq-wlan-wave6x-rflib kmod-lantiq-wlan-wave-support_6x crda_wave_6x

WAV700_PACKAGES_UCI_OSP:=iwlwav-firmware kmod-iwlwav-driver-uci iwlwav-iw ltq-wlan-wave6x-rflib kmod-lantiq-wlan-wave-support_6x crda_wave_6x

UGW_PACKAGE_DCDP:=kmod-directconnect-dp

CBSP_WAV600_UGW_PACKAGES_UCI:=swpal_6x-uci-prpl dwpal_6x-uci libwlan_6x-uci dwpald_6x-uci \
        ltq-wlan-wave_6x-uci iwlwav-hostap-uci iwlwav-tools

WAV600_UGW_PACKAGES_UCI:=swpal_6x-uci dwpal_6x-uci libwlan_6x-uci dwpald_6x-uci wav_api_6x-uci ltq-wlan-wave_6x-uci iwlwav-hostap-uci iwlwav-tools afcd iwlwav-hostap-ng-uci

WAV700_UGW_PACKAGES_UCI_OSP:=swpal_6x-uci dwpal_6x-uci-osp libwlan_6x-uci dwpald_6x-uci wav_api_6x-uci-osp ltq-wlan-wave_6x-uci-osp iwlwav-hostap-ng-uci iwlwav-tools-osp afcd

WAV600_PACKAGES_UCI_DEBUG:=iwlwav-firmware-debug kmod-iwlwav-driver-uci-debug \
        iwlwav-iw-debug ltq-wlan-wave6x-rflib \
        kmod-lantiq-wlan-wave-support_6x crda_wave_6x

WAV600_UGW_PACKAGES_UCI_DEBUG:=swpal_6x-uci-debug dwpal_6x-uci-debug libwlan_6x-uci dwpald_6x-uci wav_api_6x-uci-debug \
        ltq-wlan-wave_6x-uci-debug iwlwav-hostap-uci-debug iwlwav-tools-debug afcd iwlwav-hostap-ng-uci-debug

CBSP_WAV600_UGW_PACKAGES_UCI_DEBUG:=swpal_6x-uci-prpl dwpal_6x-uci-debug libwlan_6x-uci dwpald_6x-uci \
        ltq-wlan-wave_6x-uci iwlwav-hostap-uci-debug iwlwav-tools-debug

WAV700_PACKAGES_UCI_DEBUG:=ltq-wlan-wave_6x-uci-debug iwlwav-firmware-debug kmod-iwlwav-driver-uci-debug \
        iwlwav-hostap-uci-debug iwlwav-iw-debug iwlwav-tools-debug ltq-wlan-wave6x-rflib \
        kmod-lantiq-wlan-wave-support_6x crda_wave_6x

WAV700_UGW_PACKAGES_UCI_DEBUG:=swpal_6x-uci-debug dwpal_6x-uci-debug libwlan_6x-uci dwpald_6x-uci wav_api_6x-uci-debug

WAV700_PACKAGES_UCI_OSP_DEBUG:=iwlwav-firmware-debug kmod-iwlwav-driver-uci-debug \
        iwlwav-iw-debug ltq-wlan-wave6x-rflib \
        kmod-lantiq-wlan-wave-support_6x crda_wave_6x

WAV700_UGW_PACKAGES_UCI_OSP_DEBUG:=swpal_6x-uci-debug dwpal_6x-uci-osp-debug libwlan_6x-uci dwpald_6x-uci wav_api_6x-uci-osp-debug \
        ltq-wlan-wave_6x-uci-osp-debug iwlwav-hostap-ng-uci-debug iwlwav-tools-osp-debug afcd

CBSP_WAV700_PACKAGES_UCI:=iwlwav-firmware kmod-iwlwav-driver-uci iwlwav-iw \
        ltq-wlan-wave6x-rflib kmod-lantiq-wlan-wave-support_6x crda_wave_6x

CBSP_WAV700_UGW_PACKAGES_UCI:=swpal_6x-uci-prpl dwpal_6x-uci libwlan_6x-uci dwpald_6x-uci \
        ltq-wlan-wave_6x-uci iwlwav-hostap-uci iwlwav-tools

CBSP_WAV700_PACKAGES_UCI_DEBUG:=iwlwav-firmware-debug kmod-iwlwav-driver-uci-debug \
        iwlwav-iw-debug ltq-wlan-wave6x-rflib \
        kmod-lantiq-wlan-wave-support_6x crda_wave_6x
CBSP_WAV700_UGW_PACKAGES_UCI_DEBUG:=swpal_6x-uci-prpl dwpal_6x-uci-debug libwlan_6x-uci dwpald_6x-uci \
        ltq-wlan-wave_6x-uci iwlwav-hostap-uci-debug iwlwav-tools-debug

PON_BASE_PACKAGES:= \
	base-files-platform-lgm pon-base-files gpon-omci-onu \
	kmod-pon-mbox-drv kmod-pon-mbox-drv-eth kmod-pon-mcc-drv \
	pon-lib pon-net-lib pon-mcc-lib pon-img-lib \
	pon-serdes-firmware-urx-a pon-serdes-firmware-urx-b \
	pon-libnl-core pon-libnl-genl pon-libnl-route \
	kmod-pps kmod-ptp kmod-pon-mbox-drv-ptp pon-tod \
	kmod-qos-tc kmod-dp-eth-reinsert

PON_DEBUG_PACKAGES:= \
	pon-base-files-debug pon-tools gpon-libs gpon-libs-linux \
	pon-mbox-drv pon-lib-cli pon-mcc-cli kmod-pon-mcc-drv-chk \
	pon-ip-tiny pon-tc ethtool tcpdump-mini

WAV700-PD_PACKAGES:=libwlan_6x-uci-WAV700-PD-FPGA-DB kmod-use_wave700_palladium

WAV700-FPGA_PACKAGES:=libwlan_6x-uci-WAV700-PD-FPGA-DB kmod-use_wave700_fpga

EXTRA_OPENWRT_PACKAGES:=kmod-gpio-button-hotplug

REPEATER_PACKAGES:=sl_client_mode
