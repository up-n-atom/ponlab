/******************************************************************************

         Copyright 2020 - 2021 MaxLinear Inc.
         Copyright 2016 - 2020 Intel Corporation
         Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
         Copyright 2012 - 2014 Lantiq Deutschland GmbH

  SPDX-License-Identifier: (BSD-3-Clause OR GPL-2.0-only)

******************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include<stdlib.h>
#include<string.h>

#include "gpy211_utility.h"
#include "gpy2xx_mdio.h"

struct gpy211_device phy;
struct gpy2xx_mdio mdio;

//! Byte swap unsigned int
u32 swap_uint32(u32 val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | (val >> 16);
}

/*variable to enable/disable debug prints*/
unsigned char debugenable = 0;
#define min(a, b)	((a)<(b) ? (a) : (b))

static inline int xisupper(char c)
{
	return (c >= 'A' && c <= 'Z');
}

static inline int xisalpha(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

static inline int xisspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\12');
}

static inline int xisdigit(char c)
{
	return (c >= '0' && c <= '9');
}

static void printMAC_Address(unsigned char *pMAC)
{
	printf("%02x:%02x:%02x:%02x:%02x:%02x",
	       pMAC[0],
	       pMAC[1],
	       pMAC[2],
	       pMAC[3],
	       pMAC[4],
	       pMAC[5]);
}

static int convert_mac_adr_str(char *mac_adr_str, unsigned char *mac_adr_ptr)
{
	char *str_ptr = mac_adr_str;
	char *endptr;
	int i;
	unsigned long int val;

	if (strlen(mac_adr_str) != (12 + 5)) {
		printf("ERROR: Invalid length of address string!\n");
		return 0;
	}

	for (i = 0; i < 6; i++) {
		val = strtoul(str_ptr, &endptr, 16);

		if ((*endptr != 0) && (*endptr != ':') && (*endptr != '-'))
			return 0;

		*(mac_adr_ptr + i) = (unsigned char)(val & 0xFF);
		str_ptr = endptr + 1;
	}

	return 1;
}

static int copy_key_to_dst(char *key_adr_str, u8 size, char *key_adr_ptr)
{
	int i;
	char buf[20];
	char *pEnd = NULL;
	char *in_key = (char *)key_adr_str;
	unsigned int *out_key = (unsigned int *)key_adr_ptr;

	if (strlen(key_adr_str) != (size * 2)) {
		printf("ERROR: Len mismatch %d != %d!\n", strlen(in_key), (size * 2));
		return 0;
	}

	for (i = 0; i < (size / 4); i++) {
		pEnd = NULL;
		snprintf(buf, sizeof(buf) / sizeof(buf[0]), "%c%c%c%c%c%c%c%c",
			 in_key[6], in_key[7], in_key[4], in_key[5],
			 in_key[2], in_key[3], in_key[0], in_key[1]);

		out_key[i] = (strtoll(buf, &pEnd, 16) & 0xFFFFFFFFu);
		in_key = in_key + 8;
		//printf("\nConverted int %s, %x", buf, out_key[i]);
	}

	return 0;
}

static struct {
	const char *cmd;
	const char *brief;
	const char *desc;
} help_text[] = {
	{
		"exit",
		"Exit GPY211 utility tool",
		NULL
	},
	{
		"help",
		"Print this help page",
		NULL
	},
	{
		"en_debug",
		"Enable debugging codes",
		"en_debug <option>\n"
		"  option: 0 - disable, 1 -enable\n"
	},
	{
		"gpy2xx_init",
		"Initialize GPY211 API",
		NULL
	},
	{
		"gpy2xx_uninit",
		"Clean up GPY211 API",
		NULL
	},
	{
		"gpy2xx_get_phy_id",
		"Read PHY/Chip/Firmware/Driver version number",
		NULL
	},
	{
		"gpy2xx_soft_reset",
		"Trigger soft reset via MDIO STD_CTRL",
		NULL
	},
	{
		"gpy2xx_poll_reset",
		"Poll soft reset status",
		NULL
	},
#if defined(EN_SMDIO_RW) && EN_SMDIO_RW
	{
		"smdio_read",
		"Read internal register (may be disabled by PHY firmware)",
		"smdio_read <regaddr=?>\n"
		"  regaddr: register address\n"
	},
	{
		"smdio_write",
		"Write internal register (may be disabled by PHY firmware)",
		"smdio_write <regaddr=?> <data=?>\n"
		"  regaddr: register address\n"
		"  data:    value\n"
	},
#endif
	{
		"gpy2xx_read",
		"Read MDIO Clause 22 registers",
		"gpy2xx_read <regaddr=?>\n"
		"  regaddr: register address\n"
	},
	{
		"gpy2xx_write",
		"Write MDIO Clause 22 registers",
		"gpy2xx_write <regaddr=?> <data=?>\n"
		"  regaddr: register address\n"
		"  data:    value\n"
	},
	{
		"gpy2xx_read_mmd",
		"Read MDIO Clause 45 MMD registers",
		"gpy2xx_read_mmd <devtype=?> <regaddr=?>\n"
		"  devtype: MMD device type\n"
		"  regaddr: register address\n"
	},
	{
		"gpy2xx_write_mmd",
		"Write MDIO Clause 45 MMD registers",
		"gpy2xx_write_mmd <devtype=?> <regaddr=?> <data=?>\n"
		"  devtype: MMD device type\n"
		"  regaddr: register address\n"
		"  data:    value\n"
	},
#if defined(P31G_IND_RW) && P31G_IND_RW
	{
		"pm_ind_read",
		"PM's indirect register read",
		"pm_ind_read <regaddr=?>\n"
		"  regaddr: register address\n"
	},
	{
		"pm_ind_write",
		"PM's indirect register write",
		"pm_ind_write <regaddr=?>\n"
		"  regaddr: register address\n"
		"  data:    value\n"
	},
#endif
	{
		"gpy2xx_mbox_read16",
		"PHY's Mbox 16-bit wide register read",
		"gpy2xx_mbox_read16 <regaddr=?>\n"
		"  regaddr: register address\n"
	},
	{
		"gpy2xx_mbox_write16",
		"PHY's Mbox 16-bit wide register write",
		"gpy2xx_mbox_write16 <regaddr=?>\n"
		"  regaddr: register address\n"
		"  data:    value\n"
	},
	{
		"gpy2xx_mbox_read32",
		"PHY's Mbox 32-bit wide register read",
		"gpy2xx_mbox_read32 <regaddr=?>\n"
		"  regaddr: register address\n"
	},
	{
		"gpy2xx_mbox_write32",
		"PHY's Mbox 32-bit wide register write",
		"gpy2xx_mbox_write32 <regaddr=?>\n"
		"  regaddr: register address\n"
		"  data:    value\n"
	},
	{
		"gpy2xx_xpcs_read16",
		"PHY's XPCS 16-bit wide register read",
		"gpy2xx_xpcs_read16 <regaddr=?>\n"
		"  regaddr: register address\n"
	},
	{
		"gpy2xx_xpcs_write16",
		"PHY's XPCS 16-bit wide register write",
		"gpy2xx_xpcs_write16 <regaddr=?>\n"
		"  regaddr: register address\n"
		"  data:    value\n"
	},
	{
		"gpy2xx_config_advert",
		"Advertise auto-negotiation parameters",
		"gpy2xx_config_advert <advertising=?>\n"
		"  advertising: 10baseT_Half|10baseT_Full|100baseT_Half|100baseT_Full\n"
		"               1000baseT_Half|1000baseT_Full|2500baseT_Full|2500baseT_FR\n"
		"               Autoneg|Pause|Asym_Pause\n"
	},
	{
		"gpy2xx_setup_forced",
		"Configures/forces speed/duplex",
		"gpy2xx_setup_forced <speed=?> <duplex=?> <pause=?> <asym_pause=?> <mstr_slave=?>\n"
		"  speed:      10/100/1000/2500\n"
		"  duplex:     full/half\n"
		"  pause:      true/false\n"
		"  asym_pause: true/false\n"
		"  mstr_slave: true/false\n"
	},
	{
		"gpy2xx_restart_aneg",
		"Enable and restart auto-negotiation",
		NULL
	},
	{
		"gpy2xx_config_aneg",
		"Restart auto-negotiation or force link",
		"gpy2xx_config_aneg <autoneg=?> [advertising=?] [<speed=?> <duplex=?> <pause=?> <asym_pause=?>]\n"
		"  autoneg:     true/false\n"
		"  advertising: 10baseT_Half|10baseT_Full|100baseT_Half|100baseT_Full\n"
		"               1000baseT_Half|1000baseT_Full|2500baseT_Full|2500baseT_FR\n"
		"               Autoneg|Pause|Asym_Pause\n"
		"  speed:       10/100/1000/2500\n"
		"  duplex:      full/half\n"
		"  pause:       true/false\n"
		"  asym_pause:  true/false\n"
	},
	{
		"gpy2xx_aneg_done",
		"Get auto-negotiation status",
		NULL
	},
	{
		"gpy2xx_update_link",
		"Get link status (up/down)",
		NULL
	},
	{
		"gpy2xx_read_status",
		"Get link status (detail)",
		NULL
	},
	{
		"gpy2xx_read_fw_info",
		"Get FW info (detail)",
		NULL
	},
	{
		"gpy2xx_gpio_cfg",
		"Configure GPIO pin",
		"For internal use.\n"
		"gpy2xx_gpio_cfg <pinid=?> <gpioflags=?>\n"
		"  pinid:     16-bit pin ID\n"
		"  gpioflags: GPIO configuration flags\n"
	},
	{
		"gpy2xx_gpio_get",
		"Get GPIO pin configuration",
		"For internal use.\n"
		"gpy2xx_gpio_get <pinid=?>\n"
		"  pinid:     16-bit pin ID\n"
	},
	{
		"gpy2xx_gpio_output",
		"Configure GPIO pin output value (low/high)",
		"For internal use.\n"
		"gpy2xx_gpio_output <pinid=?> <gpioflags=?>\n"
		"  pinid:     16-bit pin ID\n"
		"  gpioflags: 0 - low, 2 - high\n"
	},
	{
		"gpy2xx_gpio_input",
		"Get GPIO pin input value (low/high)",
		"For internal use.\n"
		"gpy2xx_gpio_input <pinid=?>\n"
		"  pinid:     16-bit pin ID\n"
	},
	{
		"gpy2xx_led_br_cfg",
		"Configure LED brightness",
		"For internal use.\n"
		"gpy2xx_led_br_cfg <brightMode=?> <brightlvlMin=?> <brightlvlMax=?> <swEdgeDetect=?>\n"
		"  brightMode: 0 - constant level at max brightness\n"
		"              1 - constant level given by brightlvlmax\n"
		"  brightlvlMin: (0 to 15) - min level of brightness\n"
		"  brightlvlMax: (0 to 15) - max level of brightness\n"
		"  swEdgeDetect: 0 - LED switch edge detection on falling edge\n"
		"				 1 - LED switch edge detection on rising edge\n"
	},
	{
		"gpy2xx_led_br_get",
		"Get LED brightness configuration",
		"For internal use.\n"
	},
#if defined(EN_LED_TOP_CFG) && EN_LED_TOP_CFG
	{
		"gpy2xx_led_top_cfg",
		"LED general configuration",
		"For internal use.\n"
		"gpy2xx_led_top_cfg <complexscan=?> <complexblink=?> <inversescan=?> <fastblinkfreq=?> <slowblinkfreq=?>\n"
		"  complexscan:  in which state the 'inverse complex SCAN' is activated, refer to below state table\n"
		"  complexblink: in which state the 'complex blinking' is activated, refer to below state table\n"
		"  inversescan:  in which state the 'complex SCAN' is activated, refer to below state table\n"
		"  fastblinkfreq:the slow-blinking frequency, refer to below frequency table\n"
		"  slowblinkfreq:the fast-blinking frequency, refer to below frequency table\n"
		"state table\n"
		"	0 - disable\n"
		"	1 - Link is UP\n"
		"	2 - Device is in power down\n"
		"	3 - Device is in EEE mode\n"
		"	4 - ANEG is running\n"
		"	5 - ABIST is running\n"
		"	6 - CDIAG is running\n"
		"	7 - TEST mode is running\n"
		"frequency table\n"
		"	0 - 2 Hz blinking frequency\n"
		"	1 - 4 Hz blinking frequency\n"
		"	2 - 8 Hz blinking frequency\n"
		"	3 - 16 Hz blinking frequency\n"
	},
	{
		"gpy2xx_led_top_get",
		"Get LED general configuration",
		"For internal use.\n"
	},
#endif
	{
		"gpy2xx_led_if_cfg",
		"Configure LED",
		"gpy2xx_led_if_cfg <ledid=?> <colormode=?> <slowblinksrc=?> <fastblinksrc=?> <constantlyon=?> <pulseflags=?>\n"
		"  ledid:        0~3\n"
		"  colormode:    internal use\n"
		"  slowblinksrc: event to trigger slow blink, refer to below trigger table\n"
		"  fastblinksrc: event to trigger fast blink, refer to below trigger table\n"
		"  constantlyon: event to trigger constant on, refer to below trigger table\n"
		"  pulseflags:   pulse on LED trigger by combination of following events\n"
		"                1 - transmit activity\n"
		"                2 - receive activity\n"
		"                4 - collision\n"
		"Trigger Table\n"
		"   0 - disable\n"
		"   1 - 10Mbps\n"
		"   2 - 100Mbps\n"
		"   4 - 1000Mbps\n"
		"   8 - 2500Mbps link\n"
	},
	{
		"gpy2xx_led_if_get",
		"Get LED configuration",
		"gpy2xx_led_if_get <ledid=?>\n"
		"  ledid:        0~3\n"
	},
#if defined(EN_SUPPORT_TOP_INT) && EN_SUPPORT_TOP_INT
	{
		"gpy2xx_topin_cfg",
		"Configure external interrupt",
		"For internal use."
	},
	{
		"gpy2xx_topin_get",
		"Get external interrupt configuration",
		"For internal use.\n"
	},
	{
		"gpy2xx_topin_clr",
		"Clear external interrupt events",
		"For internal use.\n"
	},
#endif /* EN_SUPPORT_TOP_INT */
	{
		"gpy2xx_extin_mask",
		"Enable/disable external interrupt event",
		"gpy2xx_extin_mask <stdImask=?> <extImask=?>\n"
		"  stdImask:  	0x0001 - link state change\n"
		"               0x0002 - link speed change\n"
		"               0x0004 - duplex mode change\n"
		"               0x0008 - MDI/MDIX crossover change\n"
		"               0x0010 - MDI polarity change\n"
		"               0x0020 - link's auto-downspeed change\n"
		"               0x0100 - SyncE lost of reference clock\n"
		"               0x0200 - mailbox transaction complete\n"
		"               0x0400 - auto-negotiation complete\n"
		"               0x0800 - auto-negotiation error\n"
		"               0x1000 - next page transmitted\n"
		"               0x2000 - next page received\n"
		"               0x4000 - master/slave resolution error\n"
		"               0x8000 - wake-on-lan event\n"
		"  extImask:	0x0002 - LPI interrupt\n"
		"				0x0008 - time stamp FIFO interrupt\n"
		"				0x0010 - MACSEC egress/ingress Interrupt\n"
	},
	{
		"gpy2xx_extin_get",
		"Get external interrupt event mask",
		NULL
	},
#if defined(EN_GMAC_DEBUG_ACCESS) && EN_GMAC_DEBUG_ACCESS
	{
		"gpy2xx_extin_clr",
		"Clear external interrupt event",
		"gpy2xx_extin_clr <std_istat=?>\n"
		"  std_istat:   0x0001 - link state change\n"
		"               0x0002 - link speed change\n"
		"               0x0004 - duplex mode change\n"
		"               0x0008 - MDI/MDIX crossover change\n"
		"               0x0020 - MDI polarity change\n"
		"               0x0040 - link's auto-downspeed change\n"
		"               0x0100 - SyncE lost of reference clock\n"
		"               0x0200 - mailbox transaction complete\n"
		"               0x0400 - auto-negotiation complete\n"
		"               0x0800 - auto-negotiation error\n"
		"               0x1000 - next page transmitted\n"
		"               0x2000 - next page received\n"
		"               0x4000 - master/slave resolution error\n"
		"               0x8000 - wake-on-lan event\n"
	},
#endif
	{
		"gpy2xx_test_mode_cfg",
		"Set test mode",
		"gpy2xx_test_mode_cfg <testtxmode=?>\n"
		"  testtxmode: 0 - normal operation\n"
		"              1 - Test Mode 1 (Transmit Waveform Test)\n"
		"              2 - Test Mode 2 (Transmit Jitter Test in MASTER Mode)\n"
		"              3 - Test Mode 3 (Transmit Jitter Test in SLAVE Mode)\n"
		"              4 - Test Mode 4 (Transmitter Distortion Test)\n"
	},
	{
		"gpy2xx_cdiag_start",
		"Start cable diagnostic - Check with firwmare developer for more detail.",
		NULL
	},
	{
		"gpy2xx_cdiag_read",
		"Read cable diagnostic result - Check with firwmare developer for more detail.",
		NULL
	},
	{
		"gpy2xx_cdiag_stop",
		"Stop cable diagnostic test",
		NULL
	},
	{
		"gpy2xx_abist_start",
		"Start analog self-test (ABIST) - Check with firwmare developer for more detail.",
		NULL
	},
	{
		"gpy2xx_abist_read",
		"Read analog self-test (ABIST) result - Check with firwmare developer for more detail.",
		NULL
	},
	{
		"gpy2xx_loopback_cfg",
		"Configure near/far end loopback",
		"gpy2xx_loopback_cfg <loopbackmode=?>\n"
		"  loopbackmode: 0 - disable test loop\n"
		"                1 - GMII near end test loop\n"
		"                2 - PCS far end test loop\n"
		"                3 - DEC (Digital Echo Canceler) test loop\n"
		"                4 - MDI (RJ45 near end) test loop\n"
		"                5 - far end test loop\n"
		"                8 - GMII near end test loop - LB=1\n"
	},
	{
		"gpy2xx_errcnt_cfg",
		"Configure errors/events to be counted",
		"gpy2xx_errcnt_cfg <errcountmode=?>\n"
		"  errcountmode: 0 - receive errors\n"
		"                1 - receive frames\n"
		"                2 - ESD errors\n"
		"                3 - SSD errors\n"
		"                4 - transmit errors\n"
		"                5 - transmit frames\n"
		"                6 - collision events\n"
		"                8 - link down events\n"
		"                9 - auto-downspeed events\n"
	},
	{
		"gpy2xx_errcnt_read",
		"Read error/event counter",
		NULL
	},
	{
		"gpy2xx_pcs_status_read",
		"Get PCS status",
		NULL
	},
	{
		"gpy2xx_ptp_adjfreq",
		"Adjust frequency of hardware clock",
		"gpy2xx_ptp_adjfreq <pbbTime=?>\n"
		"  pbbTime: value\n"
	},
	{
		"gpy2xx_ptp_adjtime",
		"Adjust the system timestamp",
		"gpy2xx_ptp_adjtime <deltaTime=?>\n"
		"  deltaTime: 	value\n"
	},
	{
		"gpy2xx_ptp_settime",
		"Initialize the system timestamp",
		"gpy2xx_ptp_settime <initSec=?> <initNsec=?>\n"
		"  initSec: value\n"
		"  initNsec:value\n"
	},
	{
		"gpy2xx_ptp_set_tsctrl",
		"Configures the timestamp control, second/sub-second increment value",
		"gpy2xx_ptp_set_tsctrl <enEvntMsg=?> <enMstrMsg=?> <snapType=? <gpcSel=?>\n"
		"  enEvntMsg:  	dis/en\n"
		"  enMstrMsg: 	dis/en\n"
		"  snapType: 	value\n"
		"  gpcSel: 	value\n"
	},
	{
		"gpy2xx_ptp_set_ppsctrl",
		"Configures the timestamp PPS control params",
		"gpy2xx_ptp_set_ppsctrl <ppsCtrlLow=?> <targetSec=?> <targetNsc=?> <ppsInterval=?> <ppsWidth=?>\n"
		"  ppsCtrlLow: 	value\n"
		"  targetSec:  	value\n"
		"  targetNsc: 	value\n"
		"  ppsInterval:	value\n"
		"  ppsWidth: 	value\n"
	},
	{
		"gpy2xx_ptp_set_ptoctrl",
		"Configures the timestamp PTO control params",
		"gpy2xx_ptp_set_ptoctrl <ptoCtrlLow=?> <srcPortID=?> <logmLvlLow=?> <logmLvlHi=?>\n"
		"  ptoCtrlLow: 	value\n"
		"  srcPortID:  	value\n"
		"  logmLvlLow: 	value\n"
		"  logmLvlHi:	value\n"
	},
	{
		"gpy2xx_ptp_gettime",
		"Provides the system timestamp",
		NULL
	},
	{
		"gpy2xx_ptp_enable",
		"Enable and configure PTP (1588) function",
		"gpy2xx_ptp_enable <enPhyTxTs=?> <enPhyRxTs=?> <enTxOstCor=?> <txPtpOffset=?> <txPtpProto=?> <txOstCorTime=?>\n"
		"  enPhyTxTs: 	 value\n"
		"  enPhyRxTs: 	 value\n"
		"  enTxOstCor: 	 value\n"
		"  txPtpOffset:  value\n"
		"  txPtpProto: 	 value\n"
		"  txOstCorTime: value\n"
	},
	{
		"gpy2xx_ptp_disable",
		"Disable and configure PTP (1588) function",
		NULL
	},
	{
		"gpy2xx_ptp_getcfg",
		"Get PTP (1588) configuration",
		NULL
	},
	{
		"gpy2xx_ptp_fifostat",
		"Get timestamp FIFO status",
		NULL
	},
	{
		"gpy2xx_ptp_resetfifo",
		"Resets the timestamp FIFO status",
		NULL
	},
#if defined(EN_GMAC_DEBUG_ACCESS) && EN_GMAC_DEBUG_ACCESS
	{
		"gpy2xx_ptp_txtsstat",
		"Get Tx timestamp status",
		NULL
	},
#endif
	{
		"gpy2xx_ptp_getrxts",
		"Get RX packet timestamp status and CRC",
		NULL
	},
	{
		"gpy2xx_ptp_gettxts",
		"Get TX packet timestamp and CRC status and CRC",
		NULL
	},
	{
		"gpy2xx_synce_cfg",
		"Configure SyncE function",
		"gpy2xx_synce_cfg <enSyncE=?> <synceRefClk=?> <masterSlave=?> <dataRate=?> <gpcSel=?>\n"
		"  enSyncE: 	0-disable,	  1-enable\n"
		"  synceRefClk: 0-8KHz,   1-2.048MHz,	2-1.544MHz\n"
		"  masterSlave:	0-slave mode, 1-master mode\n"
		"  dataRate: 	0-1G,		  1-2.5G\n"
		"  gpcSel:		0-GPC1,	1-GPC1,	2-GPC2\n"
	},
	{
		"gpy2xx_synce_get",
		"Get SyncE configuration",
		NULL
	},
	{
		"gpy2xx_wol_cfg",
		"Configure Wake-On-Lan function",
		"gpy2xx_wol_cfg <woloptions=?> <wolmac=?> <wolpassword=?>\n"
		"  woloptions:  combination of following flags\n"
		"               0x20 - wake up when receiving magic packet\n"
		"               0x40 - password enabled\n"
		"  wolmac:      6-byte MAC address of magic packet\n"
		"  wolpassword: 6-byte password\n"
	},
	{
		"gpy2xx_wol_get",
		"Get Wake-On-Lan configuration",
		NULL
	},
	{
		"gpy2xx_ads_cfg",
		"Configure Auto-Down-Speed",
		"gpy2xx_ads_cfg [<advertise_enable=?>] [<NBaseT_ds_enable=?>] [<downshift_cnt_thr=?>] [<force_reset=?>] [<rst_cntdown_timer=?>]\n"
		"advertise_enable:  0 Disable / 1 Enable\n"
		"NBaseT_ds_enable:  0 Disable / 1 Enable\n"
		"downshift_cnt_thr: value (5bits)\n"
		"force_reset:       0/1\n"
		"rst_cntdown_timer: value (8bits)\n"
	},
	{
		"gpy2xx_ads_get",
		"Get Auto-Down-Speed",
		NULL
	},
	{
		"gpy2xx_ulp_cfg",
		"Configure Ultra Low Power",
		"gpy2xx_ulp_cfg [<ulp_enable=?>] [<ulp_sta_block=?>]\n"
		"ulp_enable:  0 Disable / 1 Enable\n"
		"ulp_sta_block:  0 Disable / 1 Enable\n"
		"ulp_exit:  0 Not Exit / 1 Exit ULP\n"
		"master_slice:  Slice to become master <0 to 3>\n"
	},
	{
		"gpy2xx_ulp_get",
		"Get Ultra Low Power",
		NULL
	},
	{
		"gpy2xx_ads_detected",
		"Get auto-downspeed event",
		NULL
	},
	{
		"gpy2xx_sgmii_restart_aneg",
		"Restart SGMII auto-negotiation",
		NULL
	},
	{
		"gpy2xx_sgmii_config_aneg",
		"Configure SGMII auto-negotiation",
		"gpy2xx_sgmii_config_aneg <linkcfg_dir=?> [<aneg_mode=?>] [<eee_enable=?>] [<autoneg=?>] [<speed=?>] [fixed2G5=?] [<duplex=?>]\n"
		"  linkcfg_dir: 0 - SGMII_LINKCFG_TPI / 1 - SGMII_LINKCFG_SGMII\n"
		"  aneg_mode:   1 - SGMII_ANEG_1000BX\n"
		"               2 - SGMII_ANEG_CISCO_PHY\n"
		"               3 - SGMII_ANEG_CISCO_MAC\n"
		"  eee_enable:  0 - disable / 1 - enable\n"
		"  autoneg:     0 - disable / 1 - enable\n"
		"  speed:       10/100/1000/2500\n"
		"  fixed2G5:    0 - TPI speed / 1 - force 2.5G\n"
		"  duplex:      DUPLEX_FULL/DUPLEX_HALF\n"
	},
	{
		"gpy2xx_sgmii_aneg_done",
		"Check SGMII auto-negotiation status",
		NULL
	},
	{
		"gpy2xx_sgmii_read_status",
		"Get SGMII link status (detail)",
		NULL
	},
	{
		"gpy2xx_sgmii_opmode",
		"Configure SGMII operation mode",
		"gpy211_sgmii_opmode <opMode=?>\n"
		"  opMode: 0 - normal oeration\n"
		"		   1 - power down\n"
		"		   2 - loopback\n"
		"		   3 - reset\n"
	},
	{
		"gpy2xx_pvt_get",
		"Get SENSOR temparature",
		NULL
	},
#if defined(EN_GMAC_DEBUG_ACCESS) && EN_GMAC_DEBUG_ACCESS
	{
		"gpy2xx_gmacx_pm_hw_reset",
		"Set PM lavel HW reset",
		NULL
	},
#endif
	{
		"gpy2xx_macsec_enable",
		"Configure Smart-AZ to enable MACsec",
		"gpy2xx_macsec_enable [dropCrc=?] [dropPerr=?] [dropSfail=?] [dropClass=?]\n"
		"  dropCrc:    dis/en\n"
		"  dropPerr:   dis/en\n"
		"  dropSfail:  dis/en\n"
		"  dropClass:  dis/en\n"
	},
	{
		"gpy2xx_macsec_disable",
		"Unconfigure Smart-AZ to disable MACsec",
		NULL
	},
	{
		"gpy2xx_gmacx_pm_pdi_cfg",
		"Set PM PDI configuration",
		"gpy2xx_gmacx_pm_pdi_cfg <bpMac=?> <bpMsec=?> [dropCrc=?] [dropPerr=?] [dropSfail=?] [dropClass=?] [cust3gEn=?] [sgmiiLb=?]\n"
		"  bpMac:      dis/en\n"
		"  bpMsec:     dis/en\n"
		"  dropCrc:    dis/en\n"
		"  dropPerr:   dis/en\n"
		"  dropSfail:  dis/en\n"
		"  dropClass:  dis/en\n"
		"  cust3gEn:   dis/en\n"
		"  sgmiiLb:    dis/en\n"
	},
	{
		"gpy2xx_gmacx_pm_pdi_get",
		"Get PM PDI configuration",
		NULL
	},
	{
		"gpy2xx_gmacf_pm_cfg",
		"Set PM GMAC-Full configuration",
		"gpy2xx_gmacf_pm_cfg <crcPad=?> [txErr=?] [bigEndian=?]\n"
		"  crcPad:     0 - Insert both CRC and Paddding\n"
		"			   1 - Insert CRC only but no Paddding\n"
		"			   2 - Do not insert CRC or Paddding\n"
		"			   3 - Replace CRC but no Paddding\n"
		"  txErr:      dis/en\n"
		"  bigEndian:  dis/en\n"
	},
	{
		"gpy2xx_gmacf_pm_get",
		"Get PM GMAC-Full configuration",
		NULL
	},
	{
		"gpy2xx_gmacl_pm_cfg",
		"Set PM GMAC-Lite configuration",
		"gpy2xx_gmacl_pm_cfg <crcPad=?> [txErr=?] [bigEndian=?]\n"
		"  crcPad:     0 - Insert both CRC and Paddding\n"
		"			   1 - Insert CRC only but no Paddding\n"
		"			   2 - Do not insert CRC or Paddding\n"
		"			   3 - Replace CRC but no Paddding\n"
		"  txErr:      dis/en\n"
		"  bigEndian:  dis/en\n"
		"  pcStrip:    dis/en\n"
		"  crcStrip:   dis/en\n"
	},
	{
		"gpy2xx_gmacl_pm_get",
		"Get PM GMAC-Lite configuration",
		NULL
	},
#if defined(EN_GMAC_DEBUG_ACCESS) && EN_GMAC_DEBUG_ACCESS
	{
		"gpy2xx_gmacf_lpi_cfg",
		"Set PM GMAC-Full LPI configuration",
		"gpy2xx_gmacf_lpi_cfg <lpintEn=?> <lpiEn=?> <linkUp=?> [txautoEn=?] [timerEn=?] [clkStop=?] [ticTime=0196] [twTime=?] [lsTime=?] [entryTime=?]\n"
		"  lpintEn:    dis/en\n"
		"  lpiEn:      dis/en\n"
		"  linkUp:     dis/en\n"
		"  txautoEn:   dis/en\n"
		"  timerEn:    dis/en\n"
		"  clkStop:    dis/en\n"
		"  ticTime:    value\n"
		"  twTime:     value\n"
		"  lsTime:     value\n"
		"  entryTime:  value\n"
	},
	{
		"gpy2xx_gmacf_lpi_get",
		"Get PM GMAC-Full LPI configuration",
		NULL
	},
	{
		"gpy2xx_gmacl_lpi_cfg",
		"Set PM GMAC-Lite LPI configuration",
		"gpy2xx_gmacl_lpi_cfg <lpintEn=?> <lpiEn=?> <linkUp=?> [txautoEn=?] [timerEn=?] [clkStop=?] [ticTime=0196] [twTime=?] [lsTime=?] [entryTime=?]\n"
		"  lpintEn:    dis/en\n"
		"  lpiEn:      dis/en\n"
		"  linkUp:     dis/en\n"
		"  txautoEn:   dis/en\n"
		"  timerEn:    dis/en\n"
		"  clkStop:    dis/en\n"
		"  ticTime:    value\n"
		"  twTime:     value\n"
		"  lsTime:     value\n"
		"  entryTime:  value\n"
	},
	{
		"gpy2xx_gmacl_lpi_get",
		"Get PM GMAC-Lite LPI configuration",
		NULL
	},
	{
		"gpy2xx_gmacf_lpi_stat_get",
		"Get GMAC-Full LPI status",
		NULL
	},
	{
		"gpy2xx_gmacl_lpi_stat_get",
		"Get GMAC-Lite LPI status",
		NULL
	},
	{
		"gpy2xx_gmacf_lpi_imask_cfg",
		"Set GMAC-Full's LPI interrupt mask",
		"gpy2xx_gmacf_lpi_imask_cfg <rxUsc=?> <rxTrc=?> <txUsc=?> <txTrc=?>\n"
		"  rxUsc:  dis/en\n"
		"  rxTrc:  dis/en\n"
		"  txUsc:  dis/en\n"
		"  txTrc:  dis/en\n"
	},
	{
		"gpy2xx_gmacf_lpi_imask_get",
		"Get GMAC-Full's LPI interrupt mask",
		NULL
	},
	{
		"gpy2xx_gmacf_lpi_istat_get",
		"Get GMAC-Full's LPI interrupt status",
		NULL
	},
	{
		"gpy2xx_gmacf_lpi_count_get",
		"Get GMAC-Full's LPI counters",
		NULL
	},
#endif
	{
		"gpy2xx_gmacx_bm_cfg",
		"Set PM's Buffer configuration",
		"gpy2xx_gmacx_bm_cfg [sb0Start=?] [sb0End=?] [sb0Pthresh=?] [sb1Start=?] [sb1End=?] [sb1Pthresh=?]\n"
		"  sb0Start:   value\n"
		"  sb0End:     value\n"
		"  sb0Pthresh: value\n"
		"  sb1Start:   value\n"
		"  sb1End:	   value\n"
		"  sb1Pthresh: value\n"
	},
	{
		"gpy2xx_gmacx_bm_status_get",
		"Get PMs buffer status",
		NULL
	},
	{
		"gpy2xx_gmacx_bm_get",
		"Get PM's Buffer configuration",
		NULL
	},
	{
		"gpy2xx_gmacl_pause_cfg",
		"Set GMAC-Lite's Pause configuration",
		"gpy2xx_gmacl_pause_cfg <fcBusy=?> <gmaclFc=?> <txfcEn=?> <pauseTime=?> <pauseMac=?> [plThresh=?] [zqPause=?] [paThresh=?] [pdThresh=?]\n"
		"  fcBusy:   dis/en\n"
		"  gmaclFc:  dis/en\n"
		"  txfcEn:   dis/en\n"
		"  plThresh: 0 - PT-4 slot times\n"
		"			 1 - PT-28 slot times\n"
		"			 2 - PT-36 slot times\n"
		"			 3 - PT-144 slot times\n"
		"			 4 - PT-256 slot times\n"
		"			 5 - PT-512 slot times\n"
		"  zqPause:	 dis/en\n"
		"  pauseTime:value\n"
		"  pauseMac: value\n"
		"  paThresh: value\n"
		"  pdThresh: value\n"
	},
	{
		"gpy2xx_gmacl_pause_get",
		"Get GMAC-Lite's Pause configuration",
		NULL
	},
	{
		"gpy2xx_pm_freq_tune",
		"Tune the frequency of MACs within GPY in USXGMII Mode",
		"gpy2xx_pm_freq_tune <ppm=?>\n"
		"  ppm: 0 - MACs operate at frequency+000ppm\n"
		"		1 - MACs operate at frequency+100ppm\n"
		"		2 - MACs operate at frequency+300ppm\n"
	},
#if defined(EN_GMAC_DEBUG_ACCESS) && EN_GMAC_DEBUG_ACCESS
	{
		"gpy2xx_gmacf_pkt_filter_cfg",
		"Set GMAC-Full's packet filter control info",
		"gpy2xx_gmacf_pkt_filter_cfg <prEn=?> [passAmc=?] [blockBc=?] [passCtrl=?] [safltrEn=?] [rxAll=?]\n"
		"  prEn:       dis/en\n"
		"  passAmc:    dis/en\n"
		"  blockBc:    dis/en\n"
		"  passCtrl:   0 - Block all control packets\n"
		"			   1 - Forwards all control packets except Pause packets even if they fail the Address filter\n"
		"			   2 - Forwards all control packets even if they fail the Address filter\n"
		"			   3 - Forwards the control packets that pass the Address filter\n"
		"  safltrEn:   dis/en\n"
		"  rxAll:      dis/en\n"
	},
	{
		"gpy2xx_gmacf_pkt_filter_get",
		"Get GMAC-Full's packet filter control info",
		NULL
	},
	{
		"gpy2xx_gmacl_pkt_filter_cfg",
		"Set GMAC-Lite's packet filter control info",
		"gpy2xx_gmacl_pkt_filter_cfg <prEn=?> [passAmc=?] [blockBc=?] [passCtrl=?] [safltrEn=?] [rxAll=?]\n"
		"  prEn:       dis/en\n"
		"  passAmc:    dis/en\n"
		"  blockBc:    dis/en\n"
		"  passCtrl:   0 - Block all control packets\n"
		"			   1 - Forwards all control packets except Pause packets even if they fail the Address filter\n"
		"			   2 - Forwards all control packets even if they fail the Address filter\n"
		"			   3 - Forwards the control packets that pass the Address filter\n"
		"  safltrEn:   dis/en\n"
		"  rxAll:      dis/en\n"
	},
	{
		"gpy2xx_gmacl_pkt_filter_get",
		"Get GMAC-Lite's packet filter control info",
		NULL
	},
	{
		"gpy2xx_gmacf_debug_info_get",
		"Get GMAC-Full's debug info",
		NULL
	},
	{
		"gpy2xx_gmacl_pkt_cfg",
		"Set GMAC-Lite's packet config info",
		"gpy2xx_gmacl_pkt_cfg <pcStrip=?> <crcStrip=?> [jumboEn=?] [jabberDis=?] [wdogDis=?] [sup2kp=?] [macIpg=?]\n"
		"  jumboEn:    dis/en\n"
		"  jabberDis:  dis/en\n"
		"  wdogDis:    dis/en\n"
		"  pcStrip:    dis/en\n"
		"  crcStrip:   dis/en\n"
		"  sup2kp:     dis/en\n"
		"  macIpg:     0 - 12 Bytes\n"
		"  	           1 - 11 Bytes\n"
		"  	           2 - 10 Bytes\n"
		"  	           3 - 09 Bytes\n"
		"  	           4 - 08 Bytes\n"
		"			   5 - 07 Bytes\n"
		"			   6 - 06 Bytes\n"
		"			   7 - 05 Bytes\n"
	},
	{
		"gpy2xx_gmacl_pkt_get",
		"Get GMAC-Lite's packet config info",
		NULL
	},
	{
		"gpy2xx_gmacf_cfg",
		"Set GMAC-Full's basic config info",
		"gpy2xx_gmacf_cfg [reEn=?] [teEn=?] [preambLen=<enum>] [lbmEn=?] [fdupEn=?] [macSpeed=<enum>] [rcWrite1=?]\n"
		"  reEn:       dis/en\n"
		"  teEn:       dis/en\n"
		"  preambLen:  0 - 7 bytes of preamble\n"
		"			   1 - 5 bytes of preamble\n"
		"			   2 - 3 bytes of preamble\n"
		"  lbmEn:      dis/en\n"
		"  fdupEn:     dis/en\n"
		"  macSpeed:   0 - 1G\n"
		"			   1 - 2.5G\n"
		"			   2 - 10M\n"
		"			   3 - 100MG\n"
		"  rcWrite1:   dis/en\n"
	},
	{
		"gpy2xx_gmacf_get",
		"Get GMAC-Full's basic config info",
		NULL
	},
	{
		"gpy2xx_gmacl_cfg",
		"Set GMAC-Lite's basic config info",
		"gpy2xx_gmacl_cfg [reEn=?] [teEn=?] [preambLen=<enum>] [lbmEn=?] [fdupEn=?] [macSpeed=<enum>] [rcWrite1=?]\n"
		"  reEn:       dis/en\n"
		"  teEn:       dis/en\n"
		"  preambLen:  0 - 7 bytes of preamble\n"
		"			   1 - 5 bytes of preamble\n"
		"			   2 - 3 bytes of preamble\n"
		"  lbmEn:      dis/en\n"
		"  fdupEn:     dis/en\n"
		"  macSpeed:   0 - 1G\n"
		"			   1 - 2.5G\n"
		"			   2 - 10M\n"
		"			   3 - 100MG\n"
		"  rcWrite1:   dis/en\n"
	},
	{
		"gpy2xx_gmacl_get",
		"Get GMAC-Lite's basic config info",
		NULL
	},
#endif
	{
		"gpy2xx_gmacf_pkt_cfg",
		"Set GMAC-Full's packet config info",
		"gpy2xx_gmacf_pkt_cfg <pcStrip=?> <crcStrip=?> [jumboEn=?] [jabberDis=?] [wdogDis=?] [sup2kp=?] [macIpg=?]\n"
		"  jumboEn:    dis/en\n"
		"  jabberDis:  dis/en\n"
		"  wdogDis:    dis/en\n"
		"  pcStrip:    dis/en\n"
		"  crcStrip:   dis/en\n"
		"  sup2kp:     dis/en\n"
		"  macIpg:     0 - 12 Bytes\n"
		"  	           1 - 11 Bytes\n"
		"  	           2 - 10 Bytes\n"
		"  	           3 - 09 Bytes\n"
		"  	           4 - 08 Bytes\n"
		"  	           5 - 07 Bytes\n"
		"  	           6 - 06 Bytes\n"
		"  	           7 - 05 Bytes\n"
	},
	{
		"gpy2xx_gmacf_pkt_get",
		"Get GMAC-Full's packet config info",
		NULL
	},
	{
		"gpy2xx_gmacf_count_ctrl_cfg",
		"Set GMAC-Full's Rx/Tx counters control info",
		"gpy2xx_gmacf_count_ctrl_cfg <rstCnt=?> <stopRoll=?> <conRead=?> <frzCnt=?> <pstCnt=?> <psthFull=?> <cntbcDrop=?>\n"
		"  rstCnt:     dis/en\n"
		"  stopRoll:   dis/en\n"
		"  conRead:    dis/en\n"
		"  frzCnt:     dis/en\n"
		"  pstCnt:     dis/en\n"
		"  psthFull:   dis/en\n"
		"  cntbcDrop:  dis/en\n"
	},
	{
		"gpy2xx_gmacf_count_ctrl_get",
		"Get GMAC-Full's counters control info",
		NULL
	},
	{
		"gpy2xx_gmacf_count_get",
		"Get GMAC-Full's Rx/Tx counters",
		NULL
	},
#if defined(EN_GMAC_DEBUG_ACCESS) && EN_GMAC_DEBUG_ACCESS
	{
		"gpy2xx_gmacf_count_imask_cfg",
		"Set GMAC-Full's counters interrupt mask",
		"gpy2xx_gmacf_count_imask_cfg <txGb=?> <txUflow=?> <txGood=?> <txOsize=?> <rxGb=?> <rxCrc=?> <rxUsize=?> <rxOsize=?>\n"
		"  txGb:     dis/en\n"
		"  txUflow:  dis/en\n"
		"  txGood:   dis/en\n"
		"  txOsize:  dis/en\n"
		"  rxGb:     dis/en\n"
		"  rxCrc:    dis/en\n"
		"  rxUsize:  dis/en\n"
		"  rxOsize:  dis/en\n"
	},
	{
		"gpy2xx_gmacf_count_imask_get",
		"Get GMAC-Full's counters interrupt mask",
		NULL
	},
	{
		"gpy2xx_gmacf_count_istat_get",
		"Get GMAC-Full's counters interrupt status",
		NULL
	},
#endif
	{
		"gpy2xx_msec_init_ing_dev",
		"Init macsec ING device",
		NULL
	},
	{
		"gpy2xx_msec_init_egr_dev",
		"Init macsec EGR device",
		NULL
	},

	{
		"gpy2xx_msec_config_ing_tr",
		"Configure ingress transform record",
		"gpy2xx_msec_config_ing_tr [sa_index=?] [ca_type=?] [key=?] [sci=?] [seq_num=?] [rep_window=?] [salt=?] [short_sci=?]\n"
		"  sa_index:   value\n"
		"  ca_type:    value\n"
		"  key:        value\n"
		"  sci:        value\n"
		"  seq_num     value\n"
		"  salt:       value\n"
		"  short_sci:  value\n"
		"  rep_window: value\n",
	},
	{
		"gpy2xx_msec_config_egr_tr",
		"Configure egress transform record",
		"gpy2xx_msec_config_egr_tr [sa_index=?] [ca_type=?] [an=?] [key=?] [sci=?] [seq_num=?] [salt=?] [short_sci=?] [next_sa_ind=?] [sa_expirq=?] [sa_indvalid=?] [flow_index=?] [sa_indupen=?]\n"
		"  sa_index:    value\n"
		"  ca_type:     value\n"
		"  an:          value\n"
		"  key:         value\n"
		"  sci:         value\n"
		"  seq_num:        value\n"
		"  salt:        value\n"
		"  short_sci:    value\n"
		"  next_sa_ind:   value\n"
		"  sa_expirq: value\n"
		"  sa_indvalid:  value\n"
		"  flow_index:  value\n"
		"  sa_indupen:  value\n",
	},
	{
		"gpy2xx_msec_update_egr_sa_cw",
		"Update egress SA control word",
		"gpy2xx_msec_update_egr_sa_cw [sa_index=?] [ca_type=?] [sn_type=?] [next_sa_ind=?] [sa_expirq=?] [sa_indvalid=?] [flow_index=?] [sa_indupen=?]\n"
		"  sa_index:    value\n"
		"  ca_type:     value\n"
		"  sn_type:     value\n"
		"  next_sa_ind: value\n"
		"  sa_expirq:   value\n"
		"  sa_indvalid: value\n"
		"  flow_index:  value\n"
		"  sa_indupen:  value\n",
	},
	{
		"gpy2xx_msec_get_ing_tr",
		"Get ingress transform record",
		"gpy2xx_msec_get_ing_tr sa_index=0\n"
		"  sa_index:  value\n",
	},
	{
		"gpy2xx_msec_get_egr_tr",
		"Get egress transform record",
		"gpy2xx_msec_get_egr_tr sa_index=0\n"
		"  sa_index:  value\n",
	},
	{
		"gpy2xx_msec_get_ing_pn",
		"Get ingress current SN from transform record",
		"gpy2xx_msec_get_ing_pn sa_index=0\n"
		"  sa_index:  value\n",
	},
	{
		"gpy2xx_msec_get_egr_pn",
		"Get egress current SN from transform record",
		"gpy2xx_msec_get_egr_pn sa_index=0\n"
		"  sa_index:  value\n",
	},
	{
		"gpy2xx_msec_config_ing_sam_rule",
		"Configure ingress SA match rule parameters",
		"gpy2xx_msec_config_ing_sam_rule [rule_index=?] [sa_mac=?] [da_mac=?] [eth_type=?] [vlan_id=?] [misc=?] [sci=?] [mask=?] [flow_index=?]\n"
		"  rule_index:value\n"
		"  sa_mac:    value\n"
		"  da_mac:    value\n"
		"  eth_type:  value\n"
		"  vlan_id:   value\n"
		"  misc:      value\n"
		"  sci:       value\n"
		"  mask:      value\n"
		"  flow_index:value\n",
	},
	{
		"gpy2xx_msec_config_egr_sam_rule",
		"Configure egress SA match rule parameters",
		"gpy2xx_msec_config_egr_sam_rule [rule_index=?] [sa_mac=?] [da_mac=?] [eth_type=?] [vlan_id=?] [misc=?] [sci=?] [mask=?] [flow_index=?]\n"
		"  rule_index:value\n"
		"  sa_mac:    value\n"
		"  da_mac:    value\n"
		"  eth_type:  value\n"
		"  vlan_id:   value\n"
		"  misc:      value\n"
		"  sci:       value\n"
		"  mask:      value\n"
		"  flow_index:value\n",
	},
	{
		"gpy2xx_msec_get_ing_sam_rule",
		"Get ingress SA match rule parameters",
		"gpy2xx_msec_get_ing_sam_rule [rule_index=?]\n"
		"  rule_index: value\n",
	},
	{
		"gpy2xx_msec_get_egr_sam_rule",
		"Get egress SA match rule parameters",
		"gpy2xx_msec_get_egr_sam_rule [rule_index=?]\n"
		"  rule_index: value\n",
	},
	{
		"gpy2xx_msec_config_ing_sam_fca",
		"Configure ingress SA matching rule's flow control action parameters",
		"gpy2xx_msec_config_ing_sam_fca [flow_index=?] [flow_type=?] [dest_port=?] [drop_nonres=?] [crypt_auth=?] [drop_act=?] [sa_index=?] [rep_protect=?] [sa_inuse=?] [validate_level=?] [conf_offset=?]\n"
		"  flow_index: 	 value\n"
		"  flow_type:    value\n"
		"  dest_port:    value\n"
		"  drop_nonres:  value\n"
		"  crypt_auth:   value\n"
		"  drop_act:   	 value\n"
		"  sa_index:     value\n"
		"  rep_protect:  value\n"
		"  sa_inuse:     value\n"
		"  validate_level:value\n"
		"  conf_offset:  value\n",
	},
	{
		"gpy2xx_msec_config_egr_sam_fca",
		"Configure egress SA matching rule's flow control action parameters",
		"gpy2xx_msec_config_egr_sam_fca [flow_index=?] [flow_type=?] [dest_port=?] [drop_nonres=?] [crypt_auth=?] [drop_act=?] [sa_index=?] [frame_protect=?] [sa_inuse=?] [include_sci=?] [use_es=?] [use_scb=?] [tag_bpsize=?] [sa_indup=?] [conf_offset=?] [conf_protect=?]\n"
		"  flow_index:	 value\n"
		"  flow_type:	 value\n"
		"  dest_port:	 value\n"
		"  drop_nonres:  value\n"
		"  crypt_auth:	 value\n"
		"  drop_act:	 value\n"
		"  sa_index:	 value\n"
		"  frame_protect:value\n"
		"  sa_inuse:	 value\n"
		"  include_sci:  value\n"
		"  use_es:		 value\n"
		"  use_scb: 	 value\n"
		"  tag_bpsize:	 value\n"
		"  sa_indup:	 value\n"
		"  conf_offset:  value\n"
		"  conf_protect: value\n",
	},
	{
		"gpy2xx_msec_get_ing_sam_fca",
		"Get ingress SA matching rule's flow control action parameters",
		"gpy2xx_msec_get_ing_sam_fca [flow_index=?]\n"
		"  flow_index: 	value\n",
	},
	{
		"gpy2xx_msec_get_egr_sam_fca",
		"Get ingress SA matching rule's flow control action parameters",
		"gpy2xx_msec_get_egr_sam_fca [flow_index=?]\n"
		"  flow_index: 	value\n",
	},
	{
		"gpy2xx_msec_clear_ing_tr",
		"Clears ingress transform record",
		"gpy2xx_msec_clear_ing_tr [sa_index=?]\n"
		"  sa_index:  value\n",
	},
	{
		"gpy2xx_msec_clear_egr_tr",
		"Clears egress transform record",
		"gpy2xx_msec_clear_egr_tr [sa_index=?]\n"
		"  sa_index:  value\n",
	},
	{
		"gpy2xx_msec_clear_ing_sam_rule",
		"Clears ingress SAM rule parameters",
		"gpy2xx_msec_clear_ing_sam_rule [rule_index=?]\n"
		"  rule_index:  value\n",
	},
	{
		"gpy2xx_msec_clear_egr_sam_rule",
		"Clears egress SAM rule parameters",
		"gpy2xx_msec_clear_egr_sam_rule [rule_index=?]\n"
		"  rule_index:  value\n",
	},
	{
		"gpy2xx_msec_clear_ing_sam_fca",
		"Clears ingress SAM flow control parameters",
		"gpy2xx_msec_clear_ing_sam_fca [flow_index=?]\n"
		"  flow_index:  value\n",
	},
	{
		"gpy2xx_msec_clear_egr_sam_fca",
		"Clears egress SAM flow control parameters",
		"gpy2xx_msec_clear_egr_sam_fca [flow_index=?]\n"
		"  flow_index:  value\n",
	},
	{
		"gpy2xx_msec_config_ing_vlan_parse",
		"Configure ingress SA match VLAN control parameters",
		"gpy2xx_msec_config_ing_vlan_parse [qinq_en=?] [stag_en=?] [qtag_en=?]\n"
		"  qinq_en: value\n"
		"  stag_en: value\n"
		"  qtag_en: value\n",
	},
	{
		"gpy2xx_msec_config_egr_vlan_parse",
		"Configure egress SA match VLAN control parameters",
		"gpy2xx_msec_config_egr_vlan_parse [qinq_en=?] [stag_en=?] [qtag_en=?]\n"
		"  qinq_en: value\n"
		"  stag_en: value\n"
		"  qtag_en: value\n",
	},
	{
		"gpy2xx_msec_config_ing_sam_eex",
		"Configure ingress SA match entry enable flags",
		"gpy2xx_msec_config_ing_sam_eex [eex_type=?] [eex_word0=?]\n"
		"  eex_type:  value\n"
		"  eex_word0: value\n",
	},
	{
		"gpy2xx_msec_config_egr_sam_eex",
		"Configure egress SA match entry enable flags",
		"gpy2xx_msec_config_egr_sam_eex [eex_type=?] [eex_word0=?]\n"
		"  eex_type:  value\n"
		"  eex_word0: value\n",
	},
	{
		"gpy2xx_msec_get_ing_sam_eef",
		"Get ingress SA match entry enable flags",
		"gpy2xx_msec_get_ing_sam_eef [word_num=?]\n"
		"  word_num: value\n",
	},
	{
		"gpy2xx_msec_get_egr_sam_eef",
		"Get egress SA match entry enable flags",
		"gpy2xx_msec_get_egr_sam_eef [word_num=?]\n"
		"  word_num: value\n",
	},
	{
		"gpy2xx_msec_config_ing_sam_eec",
		"Configure ingress SA match entry enable clear",
		"gpy2xx_msec_config_ing_sam_eec [sam_ind_set=?] [set_single=?] [set_all=?] [sam_ind_clr=?] [clr_single=?] [clr_all=?]\n"
		"  sam_ind_set: value\n"
		"  set_single:  value\n"
		"  set_all:     value\n"
		"  sam_ind_clr: value\n"
		"  clr_single:  value\n"
		"  clr_all:     value\n",
	},
	{
		"gpy2xx_msec_config_egr_sam_eec",
		"Configure egress SA match entry enable clear",
		"gpy2xx_msec_config_egr_sam_eec [sam_ind_set=?] [set_single=?] [set_all=?] [sam_ind_clr=?] [clr_single=?] [clr_all=?]\n"
		"  sam_ind_set: value\n"
		"  set_single:  value\n"
		"  set_all:     value\n"
		"  sam_ind_clr: value\n"
		"  clr_single:  value\n"
		"  clr_all:     value\n",
	},
	{
		"gpy2xx_msec_config_ing_cc_rule",
		"Configure ingress consistency check match parameters",
		"gpy2xx_msec_config_ing_cc_rule [icc_index=?] [eth_type=?] [misc=?] \n"
		"  icc_index:   value\n"
		"  eth_type: 	value\n"
		"  misc:        value\n"
	},
	{
		"gpy2xx_msec_get_ing_cc_rule",
		"Configure ingress consistency check match parameters",
		"gpy2xx_msec_get_ing_cc_rule [icc_index=?]\n"
		"  icc_index:	value\n",
	},
	{
		"gpy2xx_msec_config_ing_cc_eef",
		"Configure ingress consistency check entry enable flags",
		"gpy2xx_msec_config_ing_cc_eef [eec_word0=?]\n"
		"  eec_word0:	value\n",
	},
	{
		"gpy2xx_msec_get_ing_cc_eef",
		"Get ingress consistency check entry enable flags",
		"gpy2xx_msec_get_ing_cc_eef \n"
	},
	{
		"gpy2xx_msec_config_ing_cc_eec",
		"Configure ingress CC match entry enable clear",
		"gpy2xx_msec_config_ing_cc_eec [icc_ind_set=?] [set_single=?] [set_all=?] [icc_ind_clr=?] [clr_single=?] [clr_all=?]\n"
		"  icc_ind_set:value\n"
		"  set_single: value\n"
		"  set_all:    value\n"
		"  icc_ind_clr:value\n"
		"  clr_single: value\n"
		"  clr_all:    value\n",
	},
	{
		"gpy2xx_msec_config_ing_cc_ctrl",
		"Configure ingress consistency check control",
		"gpy2xx_msec_config_ing_cc_ctrl [nm_act=?] [nm_ctrl_act=?] [etype_max_len=?]\n"
		"  nm_act:        value\n"
		"  nm_ctrl_act:   value\n"
		"  etype_max_len: value\n",
	},
	{
		"gpy2xx_msec_get_ing_sa_stats",
		"Get ingress SA statistics",
		"gpy2xx_msec_get_ing_sa_stats [rule_index=?]\n"
		"  rule_index:  value\n",
	},
	{
		"gpy2xx_msec_get_egr_sa_stats",
		"Get egress SA statistics",
		"gpy2xx_msec_get_egr_sa_stats [rule_index=?]\n"
		"  rule_index:  value\n",
	},
	{
		"gpy2xx_msec_get_ing_global_stats",
		"Get ingress global statistics",
		NULL
	},
	{
		"gpy2xx_msec_get_egr_global_stats",
		"Get egress global statistics",
		NULL
	},
#if defined(EN_MSEC_DEBUG_ACCESS) && EN_MSEC_DEBUG_ACCESS
	{
		"gpy2xx_msec_get_ing_debug_stats",
		"Get ingress debug statistics",
		NULL
	},
	{
		"gpy2xx_msec_get_egr_debug_stats",
		"Get egress debug statistics",
		NULL
	},
#endif
	{
		"gpy2xx_msec_config_ing_count_ctrl",
		"Configure ingress count control",
		"gpy2xx_msec_config_ing_count_ctrl [rst_all=?] [ctr_saturate=?] [ctr_conr=?] [rst_summary=?]\n"
		"  rst_all:       value\n"
		"  ctr_saturate:  value\n"
		"  ctr_conr:      value\n"
		"  rst_summary:   value\n",
	},
	{
		"gpy2xx_msec_config_egr_count_ctrl",
		"Configure egress count controls",
		"gpy2xx_msec_config_egr_count_ctrl [rst_all=?] [ctr_saturate=?] [ctr_conr=?] [rst_summary=?]\n"
		"  rst_all:       value\n"
		"  ctr_saturate:  value\n"
		"  ctr_conr:      value\n"
		"  rst_summary:   value\n",
	},
	{
		"gpy2xx_msec_config_ing_count_incen",
		"Configure ingress counter increment enables",
		"gpy2xx_msec_config_ing_count_incen [sa_inc=?] [vlan_inc=?] [global_inc=?]\n"
		"  sa_inc:        value\n"
		"  vlan_inc:      value\n"
		"  global_inc: 	  value\n",
	},
	{
		"gpy2xx_msec_config_egr_count_incen",
		"Configure egress counter increment enables",
		"gpy2xx_msec_config_egr_count_incen [sa_inc=?] [vlan_inc=?] [global_inc=?]\n"
		"  sa_inc:        value\n"
		"  vlan_inc:	  value\n"
		"  global_inc: 	  value\n",
	},
	{
		"gpy2xx_msec_config_ing_count_secfail",
		"Configure ingress secfail event trigger enables",
		"gpy2xx_msec_config_ing_count_secfail [misc_secfail=?] [global_secfail=?]\n"
		"  misc_secfail:    value\n"
		"  global_secfail:	value\n",
	},
	{
		"gpy2xx_msec_config_egr_count_secfail",
		"Configure egress secfail event trigger enables",
		"gpy2xx_msec_config_egr_count_secfail [misc_secfail=?] [global_secfail=?]\n"
		"  misc_secfail:    value\n"
		"  global_secfail:	value\n",
	},
	{
		"gpy2xx_msec_config_ing_count_thresh",
		"Configure ingress counter packet and octet thresholds",
		"gpy2xx_msec_config_ing_count_thresh [frame_thr=?] [octet_thr=?]\n"
		"  frame_thr:	 value\n"
		"  octet_thr:	 value\n",
	},
	{
		"gpy2xx_msec_get_ing_count_thresh",
		"Get ingress counter packet and octet thresholds",
		NULL
	},
	{
		"gpy2xx_msec_config_egr_count_thresh",
		"Configure egress counter packet and octet thresholds",
		"gpy2xx_msec_config_egr_count_thresh [frame_thr=?] [octet_thr=?]\n"
		"  frame_thr:	 value\n"
		"  octet_thr:	 value\n",
	},
	{
		"gpy2xx_msec_get_egr_count_thresh",
		"GEt egress counter packet and octet thresholds",
		NULL
	},
	{
		"gpy2xx_msec_config_ing_misc_ctrl",
		"Configure ingress misc control",
		"gpy2xx_msec_config_ing_misc_ctrl [mc_latency=?] [static_bp=?] [nm_macsec=?] [validate_frames=?] [sectag_avlan=?]\n"
		"  mc_latency:       value\n"
		"  static_bp         value\n"
		"  nm_macsec:        value\n"
		"  validate_frames:  value\n"
		"  sectag_avlan:     value\n",
	},
	{
		"gpy2xx_msec_config_egr_misc_ctrl",
		"Configure egress misc control",
		"gpy2xx_msec_config_egr_misc_ctrl [mc_latency=?] [static_bp=?] [nm_macsec=?] [validate_frames=?] [sectag_avlan=?]\n"
		"  mc_latency:       value\n"
		"  static_bp         value\n"
		"  nm_macsec:        value\n"
		"  validate_frames:  value\n"
		"  sectag_avlan:     value\n",
	},
	{
		"gpy2xx_msec_config_ing_sa_nm_ctrl",
		"Configure ingress no-match classifier control",
		"gpy2xx_msec_config_ing_sa_nm_ctrl [comp_etype=?] [check_ver=?] [check_kay=?] [check_ce=?] [check_sc=?] [check_sl=?] [check_pn=?] [msec_eth=?]\n"
		"  comp_etype:      dis/en\n"
		"  check_ver        dis/en\n"
		"  check_kay:       dis/en\n"
		"  check_ce:  		dis/en\n"
		"  check_sc:     	dis/en\n"
		"  check_sl:     	dis/en\n"
		"  check_pn:     	dis/en\n"
		"  msec_eth:        value\n",
	},
	{
		"gpy2xx_msec_config_egr_sa_nm_ctrl",
		"Configure egress no-match classifier control",
		"gpy2xx_msec_config_egr_sa_nm_ctrl [comp_etype=?] [check_ver=?] [check_kay=?] [check_ce=?] [check_sc=?] [check_sl=?] [check_pn=?] [msec_eth=?]\n"
		"  comp_etype:		dis/en\n"
		"  check_ver 		dis/en\n"
		"  check_kay:		dis/en\n"
		"  check_ce: 		dis/en\n"
		"  check_sc: 		dis/en\n"
		"  check_sl: 		dis/en\n"
		"  check_pn: 		dis/en\n"
		"  msec_eth: 		value\n",
	},
	{
		"gpy2xx_msec_config_ing_sa_nm_ncp",
		"Configure ingress SA non-match flow control action for non-control packet",
		"gpy2xx_msec_config_ing_sa_nm_ncp [pkt_type=?] [flow_type=?] [dest_port=?] [drop_nonres=?] [drop_action=?]\n"
		"  pkt_type:       value\n"
		"  flow_type       value\n"
		"  dest_port:      value\n"
		"  drop_nonres:    value\n"
		"  drop_action:    value\n",
	},
	{
		"gpy2xx_msec_config_egr_sa_nm_ncp",
		"Configure egress SA non-match flow control action for non-control packet",
		"gpy2xx_msec_config_egr_sa_nm_ncp [pkt_type=?] [flow_type=?] [dest_port=?] [drop_action=?]\n"
		"  pkt_type:       value\n"
		"  flow_type       value\n"
		"  dest_port:      value\n"
		"  drop_action:    value\n",
	},
	{
		"gpy2xx_msec_config_ing_sa_nm_cp",
		"Configure ingress SA non-match flow control action for control packet",
		"gpy2xx_msec_config_ing_sa_nm_cp [pkt_type=?] [flow_type=?] [dest_port=?] [drop_nonres=?] [drop_action=?]\n"
		"  pkt_type:       value\n"
		"  flow_type       value\n"
		"  dest_port:      value\n"
		"  drop_nonres:    value\n"
		"  drop_action:    value\n",
	},
	{
		"gpy2xx_msec_config_egr_sa_nm_cp",
		"Configure egress SA non-match flow control action for control packet",
		"gpy2xx_msec_config_egr_sa_nm_cp [pkt_type=?] [flow_type=?] [dest_port=?] [drop_action=?]\n"
		"  pkt_type:       value\n"
		"  flow_type       value\n"
		"  dest_port:      value\n"
		"  drop_action:    value\n",
	},
	{
		"gpy2xx_msec_config_ing_sa_nm_expl",
		"Configure ingress SA non-match flow control action for explicitly classified packet",
		"gpy2xx_msec_config_ing_sa_nm_expl [flow_type=?] [dest_port=?] [drop_nonres=?] [drop_action=?]\n"
		"  flow_type:      value\n"
		"  dest_port:      value\n"
		"  drop_nonres:    value\n"
		"  drop_action:    value\n",
	},
	{
		"gpy2xx_msec_config_egr_sa_nm_expl",
		"Configure egress SA non-match flow control action for explicitly classified packet",
		"gpy2xx_msec_config_egr_sa_nm_expl [flow_type=?] [dest_port=?] [drop_nonres==?] [drop_action=?]\n"
		"  flow_type:       value\n"
		"  dest_port:       value\n"
		"  drop_nonres:     value\n"
		"  drop_action:     value\n",
	},
	{
		"gpy2xx_msec_clear_ing_stats_summ",
		"Configure ingress SA expired/pn-thr/psa summary",
		"gpy2xx_msec_clear_ing_stats_summ [psa_summ0=?] [glb_summ=?]\n"
		"  psa_summ0:       value\n"
		"  glb_summ:        value\n",
	},
	{
		"gpy2xx_msec_clear_egr_stats_summ",
		"Configure egress SA expired/pn-thr/psa summary",
		"gpy2xx_msec_clear_egr_stats_summ [psa_summ0=?] [glb_summ=?]\n"
		"  psa_summ0:       value\n"
		"  glb_summ:        value\n",
	},
	{
		"gpy2xx_msec_get_ing_stats_summ",
		"Get ingress SA expired/pn-thr/psa summary",
		NULL
	},
	{
		"gpy2xx_msec_get_egr_stats_summ",
		"Get egress SA expired/pn-thr/psa summary",
		NULL
	},
	{
		"gpy2xx_msec_clear_ing_psa_stats_summ",
		"Clears ingress per-SA statistics summary",
		"gpy2xx_msec_clear_ing_psa_stats_summ [psa_type=?] [rule_index=?] [psa_summ0=?]\n"
		"  psa_type:         value\n"
		"  rule_index:       value\n"
		"  psa_summ0:        value\n",
	},
	{
		"gpy2xx_msec_get_ing_psa_stats_summ",
		"Get ingress per-SA statistics summary",
		"gpy2xx_msec_get_ing_psa_stats_summ [psa_type=?] [rule_index=?]\n"
		"  psa_type:         value\n"
		"  rule_index:       value\n",
	},
	{
		"gpy2xx_msec_clear_egr_psa_stats_summ",
		"Clears egress per-SA statistics summary",
		"gpy2xx_msec_clear_egr_psa_stats_summ [psa_type=?] [rule_index=?] [psa_summ0=?]\n"
		"  psa_type:         value\n"
		"  rule_index:       value\n"
		"  psa_summ0:        value\n",
	},
	{
		"gpy2xx_msec_get_egr_psa_stats_summ",
		"Get egress per-SA statistics summary",
		"gpy2xx_msec_get_egr_psa_stats_summ [psa_type=?] [rule_index=?]\n"
		"  psa_type:         value\n"
		"  rule_index:       value\n",
	},
	{
		"gpy2xx_msec_config_ing_cp_rule",
		"Configure ingress control packet classification rule",
		"gpy2xx_msec_config_ing_cp_rule [cpc_index=?] [entry_type=?] [da_mac=?] [da_end=?] [eth_type=?] [cpm_mode=?] [cpm_en=?]\n"
		"  cpc_index:       value\n"
		"  entry_type:      value\n"
		"  da_mac:          value\n"
		"  da_end:          value\n"
		"  eth_type:        value\n"
		"  cpm_mode:        value\n"
		"  cpm_en:          value\n",
	},
	{
		"gpy2xx_msec_config_egr_cp_rule",
		"Configure engress control packet classification rule",
		"gpy2xx_msec_config_egr_cp_rule [cpc_index=?] [entry_type=?] [da_mac=?] [da_end=?] [eth_type=?] [cpm_mode=?] [cpm_en=?]\n"
		"  cpc_index:       value\n"
		"  entry_type:      value\n"
		"  da_mac:          value\n"
		"  da_end:          value\n"
		"  eth_type:        value\n"
		"  cpm_mode:        value\n"
		"  cpm_en:          value\n",
	},
	{
		"gpy2xx_msec_clear_sa_pn_thr_summ",
		"Clear OPP's SA PN threshold summary flags",
		"gpy2xx_msec_clear_sa_pn_thr_summ [pnthr_summ0=?]\n"
		"  pnthr_summ0:     value\n",
	},
	{
		"gpy2xx_msec_get_sa_pn_thr_summ",
		"Get OPP's SA PN threshold summary flags",
		NULL
	},
	{
		"gpy2xx_msec_clear_egr_sa_exp_summ",
		"Clear SA expired summary flags",
		"gpy2xx_msec_clear_egr_sa_exp_summ [saexp_summ0=?]\n"
		"  saexp_summ0:     value\n",
	},
	{
		"gpy2xx_msec_get_egr_sa_exp_summ",
		"Gets SA expired summary flags",
		NULL
	},
	{
		"gpy2xx_msec_clear_ing_cc_int_stat",
		"Clear crypto-cores's context, HW / SW interrupt status",
		"gpy2xx_msec_clear_ing_cc_int_stat [ctx_stat=?] [int_stat=?]\n"
		"  ctx_stat:     value\n"
		"  int_stat:     value\n",
	},
	{
		"gpy2xx_msec_get_ing_cc_int_stat",
		"Get crypto-cores's context, HW / SW interrupt status",
		NULL
	},
	{
		"gpy2xx_msec_clear_egr_cc_int_stat",
		"Clear crypto-cores's context, HW / SW interrupt status",
		"gpy2xx_msec_clear_egr_cc_int_stat [ctx_stat=?] [int_stat=?]\n"
		"  ctx_stat:     value\n"
		"  int_stat:     value\n",
	},
	{
		"gpy2xx_msec_get_egr_cc_int_stat",
		"Get crypto-cores's context, HW / SW interrupt status",
		NULL
	},
	{
		"gpy2xx_msec_config_ing_sn_thresh",
		"Configures the ingress sequence number threshold",
		"gpy2xx_msec_config_ing_sn_thresh [sn_type=?] [sn_thr=?]\n"
		"  sn_type:    	value\n"
		"  sn_thr:     	value\n",
	},
	{
		"gpy2xx_msec_get_ing_sn_thresh",
		"Gets the ingress Sequence Number threshold",
		"gpy2xx_msec_get_ing_sn_thresh [sn_type=?]\n"
		"sn_type:    1 - SN_32_BIT / 2 - SN_64_BIT",
	},
	{
		"gpy2xx_msec_config_egr_sn_thresh",
		"Configures the egress sequence number threshold",
		"gpy2xx_msec_config_egr_sn_thresh [sn_type=?] [sn_thr=?]\n"
		"  sn_type:    	value\n"
		"  sn_thr:     	value\n",
	},
	{
		"gpy2xx_msec_get_egr_sn_thresh",
		"Gets the ingress squence number threshold",
		"gpy2xx_msec_get_egr_sn_thresh [sn_type=?]\n"
		"sn_type:	 1 - SN_32_BIT / 2 - SN_64_BIT",

	},
	{
		"gpy2xx_msec_config_ing_aic_csr",
		"Configures the ingress interrupt  controll and status options",
		"gpy2xx_msec_config_ing_aic_csr [enable_ctrl=?] [ack=?] [enable_set=?] [enable_clr=?]\n"
		"  enable_ctrl:		value\n"
		"  ack:    			value\n"
		"  enable_set:    	value\n"
		"  enable_clr:     	value\n",
	},
	{
		"gpy2xx_msec_get_ing_aic_csr",
		"Gets the ingress interrupt  controll and status options",
		NULL
	},
	{
		"gpy2xx_msec_config_egr_aic_csr",
		"Configures the egress interrupt  controll and status options",
		"gpy2xx_msec_config_egr_aic_csr [enable_ctrl=?] [ack=?] [enable_set=?] [enable_clr=?]\n"
		"  enable_ctrl:		value\n"
		"  ack:    			value\n"
		"  enable_set:    	value\n"
		"  enable_clr:     	value\n",
	},
	{
		"gpy2xx_msec_get_egr_aic_csr",
		"Gets the egress interrupt  controll and status options",
		NULL
	},
	{
		"gpy2xx_msec_get_ing_cap",
		"Gets the ingress HW capabilities",
		NULL
	},
	{
		"gpy2xx_msec_get_egr_cap",
		"Gets the egress HW capabilities",
		NULL
	},
	{
		"gpy2xx_usxgmii_reach_cfg",
		"Configures the USXGMII reach customer setting according to master slice (slice 0)",
		"gpy2xx_usxgmii_reach_cfg trace_len=? [tx_eq_main=?] [tx_eq_pre=?] [tx_eq_post=?] [tx_vboost_en=?] [tx_vboost_lvl=?] [tx_iboost_lvl=?] [rx_eq_att_lvl=?] [rx_eq_vga1_gain=?] [rx_eq_vga2_gain=?] [rx_eq_ctle_boost=?] [rx_eq_ctle_pole=?] [rx_eq_dfe_tap1=?] [rx_afe_adapt_en=?] [rx_dfe_adapt_en=?]\n"
		"trace_len:         0-short, 1-medium, 2-long, 3-custom\n"
		"tx_eq_main:        0x00 - 0x3F\n"
		"tx_eq_pre:         0x00 - 0x3F\n"
		"tx_eq_post:        0x00 - 0x3F\n"
		"tx_vboost_en:      0 - disable, 1 - enable\n"
		"tx_vboost_lvl:     0x00 - 0x07\n"
		"tx_iboost_lvl:     0x00 - 0x07\n"
		"rx_eq_att_lvl:     0x00 - 0x07\n"
		"rx_eq_vga1_gain:   0x00 - 0x0F\n"
		"rx_eq_vga2_gain:   0x00 - 0x0F\n"
		"rx_eq_ctle_boost:  0x00 - 0x1F\n"
		"rx_eq_ctle_pole:   0x00 - 0x07\n"
		"rx_eq_dfe_tap1:    0x00 - 0xFF\n"
		"rx_afe_adapt_en:	0 - disable, 1 - enable\n"
		"rx_dfe_adapt_en:	0 - disable, 1 - enable\n",
	},
	{
		"gpy2xx_usxgmii_reach_get",
		"Get the USXGMII reach setting",
		NULL
	},

	{
		"gpy2xx_usxgmii_loopback_cfg",
		"Configures the USXGMII serial and parrallel loop back",
		"gpy2xx_usxgmii_loopback_cfg [mode=?]\n"
		"Loopback Mode\n"
		"enum mode\n"
		"	USXGMII_LOOPBACK_DISABLE = 0,\n"
		"	USXGMII_LOOPBACK_TX2RX   = 1,\n"
		"	USXGMII_LOOPBACK_RX2TX   = 2,\n",
	},

	{
		"gpy2xx_usxgmii_loopback_get",
		"Get the USXGMII reach loop back setting",
		NULL
	},

	{
		"gpy2xx_usxgmii_tx_bert_cfg",
		"Configures TX bert",
		"gpy2xx_usxgmii_tx_bert_cfg  [mode=?] [nval_sel=?]\n"
		"tx bert mode configuration\n"
		"enum mode\n"
		"	BERT_SQUAREWAVE_TX = 0\n"
		"	BERT_PSEUDO_RANDOM_TX = 1\n"
		"	BERT_PRBS31_TX = 2\n"
		"	BERT_PRBS9_TX = 3\n"
		"	BERT_TX_MODE_DISABLE = 4\n"
		"Square Wave Control When square wave test pattern generation is enabled\n"
		"enum nval_sel\n"
		"	BERT_TX_NVAL_SEL_4 = 0\n"
		"	BERT_TX_NVAL_SEL_5 = 1\n"
		"	BERT_TX_NVAL_SEL_6 = 2\n"
		"	BERT_TX_NVAL_SEL_7 = 3\n"
		"	BERT_TX_NVAL_SEL_8 = 4\n"
		"	BERT_TX_NVAL_SEL_9 = 5\n"
		"	BERT_TX_NVAL_SEL_10 = 6\n"
		"	BERT_TX_NVAL_SEL_11 = 7\n"
		"	BERT_TX_NVAL_SEL_UNSUPPORTED = 8\n",
	},

	{
		"gpy2xx_usxgmii_rx_bert_cfg",
		"Configures RX bert",
		"gpy2xx_usxgmii_rx_bert_cfg  [mode=?]\n"
		"rx bert mode configuration\n"
		"enum mode\n"
		"	BERT_PSEUDO_RANDOM_RX = 0\n"
		"	BERT_PRBS31_RX = 1\n"
		"	BERT_PRBS9_RX = 2\n"
		"	BERT_RX_MODE_DISABLE = 3\n",
	},

	{
		"gpy2xx_usxgmii_tx_bert_get",
		"Get TX bert configuration setting",
		NULL
	},

	{
		"gpy2xx_usxgmii_rx_bert_get",
		"Get RX bert configuration setting",
		NULL
	},

	{
		"gpy2xx_usxgmii_mii_linksts",
		"Get Port mii linkstatus (port 0 to 3 link status based on the phy address)",
		NULL
	},

	{
		"gpy2xx_usxgmii_aneg_sts",
		"Get Port ANEG status (port 0 to 3 ANEG status based on the phy address)",
		NULL
	},

	{
		"gpy2xx_usxgmii_pcs_pma_linksts",
		"Get PMA and PCS link status",
		NULL
	},

	{
		"gpy2xx_usxgmii_vr_reset",
		"Trigger VR reset",
		NULL
	},

	{
		"gpy2xx_usxgmii_vr_reset_sts",
		"Get VR reset complete status",
		NULL
	},

	{
		"gpy2xx_usxgmii_aneg_rst",
		"Configures ANEG for Port 0 to 3 (based on phy address)",
		"gpy2xx_usxgmii_aneg_rst  [usxgmii_aneg_disable=?] [usxgmii_force_linkspeed_enable=?] [usxgmii_force_linkspeed=?]  [usxgmii_force_duplex=?] [usxgmii_restart_aneg=?]\n"
		"ANEG configuration\n"
		"aneg enable/disable for port 0 to 3 based on phy addr\n"
		"Valid 1 - disabled / 0 - enabled\n"
		"	usxgmii_aneg_disable\n"
		"restart aneg for port 0 to 3 based on phy addr\n"
		"	usxgmii_restart_aneg\n"
		"force link enable for port 0 to 3 based on phy addr\n"
		"Valid 0 - disabled / 1 - enabled\n"
		"	usxgmii_force_linkspeed\n"
		"force link speed for port 0 to 3 based on phy addr\n"
		"	usxgmii_force_linkspeed\n"
		"		SPEED_10	10\n"
		"		SPEED_100	100\n"
		"		SPEED_1000	1000\n"
		"		SPEED_2500	2500\n"
		"		SPEED_UNKNOWN	-1\n"
		"force duplex for port 0 to 3 based on phy addr\n"
		"Valid 0 - Half duplex / 1 - Full duplex\n"
		"	usxgmii_force_duplex\n",
	},

	{
		"gpy2xx_usxgmii_alignmentmarker_get",
		"Get AM_COUNT",
		NULL
	},

	{
		"gpy2xx_usxgmii_alignmentmarker_set",
		"Configures AM count",
		"gpy2xx_usxgmii_alignmentmarker_set [usxgmii_am_count=?]\n"
		"AM Count\n"
		"Valid 0x00 - 0x7FFF\n"
		"	usxgmii_am_count\n",
	},

	{
		"gpy2xx_usxgmii_rx_bert_stat",
		"Get BERT stats",
		NULL
	},

};

static void gpy211_menu(const char *cmd)
{
	unsigned int len, max_len;
	unsigned int i;

	if (cmd == NULL) {
		printf("\n\n\n\tGPY211 Utility Tool Version   : %s", TOOL_VERSION);
		printf("\n\t*************************************\n\n");

		max_len = 0;

		for (i = 0; i < ARRAY_SIZE(help_text); i++) {
			len = strlen(help_text[i].cmd);

			if (max_len < len)
				max_len = len;
		}

		for (i = 0; i < ARRAY_SIZE(help_text); i++) {
			printf("\t %-*s : %s\n", max_len, help_text[i].cmd, help_text[i].brief);
		}

		printf("\n\nIMPORTANT NOTE:\n");
		printf("\nFor hex decimal input param please add 0x before the hex decimal number or just give decimal number");
		printf("\nPrefix hex decimal value with 0x or GPY211 command execution will lead to wrong setting or malfunction\n");
		printf("\n");
	} else {
		for (i = 0; i < ARRAY_SIZE(help_text) && strcasecmp(cmd, help_text[i].cmd) != 0; i++);

		if (i >= ARRAY_SIZE(help_text))
			return;

		if (help_text[i].desc == NULL) {
			printf("%s does not have parameter.\n", help_text[i].cmd);
		} else {
			printf("%s", help_text[i].desc);
		}
	}
}

static unsigned int t_olower(u32 ch)
{
	if (ch >= 'A' && ch <= 'Z')
		return ch + 'a' - 'A';

	return ch;
}

static unsigned int xstrncasecmp(const char *s1, const char *s2, unsigned int n)
{
	int c1, c2;

	if (!s1 || !s2) return 1;

	for (; n > 0; s1++, s2++, --n) {
		c1 = t_olower(*s1);
		c2 = t_olower(*s2);

		if (c1 != c2) {
			return (c1 - c2);
		} else if (c1 == '\0') {
			return 0;
		}
	}

	return 0;
}

unsigned uart_hang = 1;

char *var_list[MAX_PARAM];
char *param_list1[MAX_PARAM];

int system_restart = 0;

unsigned int str_to_hex(unsigned char *str)
{
	unsigned int n = 0;
	int i = 0;

	if (!str)
		return 0;

	if (str[0] == 0)
		return 0;

	while (str[i]) {
		n = n * 16;

		if (('0' <= str[i] && str[i] <= '9')) {
			n += str[i] - '0';
		} else if (('A' <= str[i] && str[i] <= 'F')) {
			n += str[i] - 'A' + 10;
			;
		} else if (('a' <= str[i] && str[i] <= 'f')) {
			n += str[i] - 'a' + 10;
			;
		} else
			printf("Wrong value:%u\n", str[i]);

		i++;
	}

	return n;
}

static inline unsigned int hex2int(char c)
{
	if (c >= '0' && c <= '9')
		return (unsigned int)(c - '0');
	else
		return (unsigned int)(c - 'a' + 10);
}

#define UNIX_EPOCH 1970
int dayofweek(int d, int m, int y)
{
	char day;
	static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };

	y -= m < 3;
	day = (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;

	//If leap year it will be the remainder-1
	if (y % 4 == 0)
		day -= 1;

	return day;
}

static int print_sys_time(u32 sec, u32 nsec)
{
	int year,act_leap = 0,  leap_days = 0, act_mnum, mnum = 0, act_day, day, hour, minute, is_leap = UNIX_EPOCH;
	char *monName = "January";
	//PTP Unix time starts from 1st Jan 1970, Thirsday
	char *dayName[] = {"Thursday", "Friday", "Saturday", "Sunday", "Monday", "Tuesday", "Wednesday"};
	char *weekDay;

	minute = sec / 60;
	sec = sec % 60;

	hour = minute / 60;
	minute = minute % 60;

	day = act_day = hour / 24;
	weekDay = dayName[day % 7];
	hour = hour % 24;

	year  = day / 365;
	//printf("\n\tYrs %d days %d hours %d mins %d secs %d\n", year, day, hour, minute, sec);

	while ((is_leap < (UNIX_EPOCH + year))) {
		if (is_leap % 4 == 0)
			act_leap++;
		is_leap++;
	};
	//printf("\n\tLeap days %d\n", act_leap);

	year = (act_day - act_leap) / 365;
	act_day -= act_leap;
	day = act_day % 365;
	mnum = act_mnum = day / 30;
	//printf("\n\tAct yrs %d, act days %d\n", year, act_day);

	leap_days = 0;
	is_leap = UNIX_EPOCH;
	while ((is_leap < (UNIX_EPOCH + year))) {
		if (is_leap % 4 == 0)
			leap_days++;
		is_leap++;
	};

	//day += (((act_leap == leap_days) ? 0 : 1) + 1); //+1 for the current day
	day += ((act_leap - leap_days) + 1); //+1 for the current day
	//printf("\n\tAct yrs %d leap days %d days %d month num %d\n", year, leap_days, day, mnum);

	switch (act_mnum) {
	case 1:
		if (day > 31) {
			monName = "February";
			mnum += 1;
			day -= 31;
		}
	break;
	case 2:
	case 3:
		monName = "March";
		if (day > 90) {
			monName = "April";
			mnum += 1;
			day -= 90;
		} else
			day -= 59;
	break;
	case 4:
		monName = "April";
		if (day > 120) {
			monName = "May";
			mnum += 1;
			day -= 120;
		} else
			day -= 90;
	break;
	case 5:
		monName = "May";
		if (day > 151) {
			monName = "June";
			mnum += 1;
			day -= 151;
		} else
			day -= 120;
	break;
	case 6:
		monName = "June";
		if (day > 181) {
			monName = "July";
			mnum += 1;
			day -= 181;
		} else
			day -= 151;
	break;
	case 7:
		monName = "July";
		if (day > 212) {
			monName = "August";
			mnum += 1;
			day -= 212;
		} else
			day -= 181;
	break;
	case 8:
		monName = "August";
		if (day > 243) {
			monName = "September";
			mnum += 1;
			day -= 243;
		} else
			day -= 212;
	break;
	case 9:
		monName = "September";
		if (day > 273) {
			monName = "October";
			mnum += 1;
			day -= 273;
		} else
			day -= 243;
	break;
	case 10:
		monName = "October";
		if (day > 304) {
			monName = "November";
			mnum += 1;
			day -= 304;
		} else
			day -= 273;
	break;
	case 11:
		monName = "November";
		if (day > 334) {
			monName = "December";
			mnum += 1;
			day -= 334;
		} else
			day -= 304;
	break;
	case 12:
		monName = "December";
		day -= 334;
	break;
	default:
	break;
	}

	//If the current year is leap and beyond feb?
	if (((UNIX_EPOCH + year) % 4 == 0) && (act_mnum > 1)) {
		//printf("\n\tThe current year %d is leap", (UNIX_EPOCH + year));
		day -= 1;
	}

	//printf("\n\tYr %d day %d mon %d\n", (UNIX_EPOCH + year), day, mnum);
	printf("\n\tUnix system time: %s %s %d %d %02d:%02d:%02d GMT\n", weekDay, monName, day, (UNIX_EPOCH + year), hour, minute, sec);
	return 0;
}


static char *findArgParam(int argc, char *argv[], char *name)
{
	int i = 0;
	size_t len;

	len = strlen(name);

	for (i = 0; i < argc; i++) {
		if (xstrncasecmp(name, argv[i], len) == 0) {
			if (strlen(argv[i]) > (len + 1)) {

				if ('=' == argv[i][len]) {
					return argv[i] + len + 1;
				}
			}
		}
	}

	return NULL;
}

int scanParamArg(int argc, char *argv[], char *name, int size, void *param)
{
	unsigned long long int tmp;
	char *endptr;

	char *ptr = findArgParam(argc, argv, name);

	if (ptr == NULL) return 0;

	/* check if the given parameter value are the boolean "LTQ_TRUE" / "LTQ_FALSE" */

	if (0 == xstrncasecmp(ptr, "LTQ_TRUE", strlen(ptr))
	    || 0 == xstrncasecmp(ptr, "TRUE", strlen(ptr))
	    || 0 == xstrncasecmp(ptr, "FULL", strlen(ptr))) {
		tmp = 1;
	} else if (0 == xstrncasecmp(ptr, "LTQ_FALSE", strlen(ptr))
		   || 0 == xstrncasecmp(ptr, "FALSE", strlen(ptr))
		   || 0 == xstrncasecmp(ptr, "HALF", strlen(ptr))) {
		tmp = 0;
	} else {
		/* scan for a number */
		tmp = strtoull(ptr, &endptr, 0);

		/* parameter detection does not work in case there are more character after the provided number */
		if (*endptr != '\0')
			return 0;
	}

	if (size <= 4)
		size *= 8;

	if (size == 8)
		*((char *)param) = (char)tmp;
	else if (size == 16)
		*((short *)param) = (short)tmp;
	else if (size == 32)
		*((int *)param) = (int)tmp;
	else if (size == 64)
		*((long long *)param) = (long long)tmp;

	return 1;
}

int scanMAC_Arg(int argc, char *argv[], char *name, unsigned char *param)
{
	char *ptr = findArgParam(argc, argv, name);

	if (ptr == NULL) return 0;

	return convert_mac_adr_str(ptr, param);
}

int scanKey_Arg(int argc, char *argv[], char *name, u8 size, char *param)
{
	char *ptr = findArgParam(argc, argv, name);

	if (ptr == NULL) return 0;

	return copy_key_to_dst(ptr, size, param);
}

static struct {
	const char *name;
	enum link_mode_bit_indices bit;
} phy_advert[] = {
	{"10baseT_Half", LINK_MODE_10baseT_Half_BIT},
	{"10baseT_Full", LINK_MODE_10baseT_Full_BIT},
	{"100baseT_Half", LINK_MODE_100baseT_Half_BIT},
	{"100baseT_Full", LINK_MODE_100baseT_Full_BIT},
	{"1000baseT_Half", LINK_MODE_1000baseT_Half_BIT},
	{"1000baseT_Full", LINK_MODE_1000baseT_Full_BIT},
	{"2500baseT_Full", LINK_MODE_2500baseT_Full_BIT},
	{"2500baseT_FR", LINK_MODE_2500baseT_FR_BIT},
	{"5000baseT_Full", LINK_MODE_5000baseT_Full_BIT},
	{"5000baseT_FR", LINK_MODE_5000baseT_FR_BIT},
	{"Autoneg", LINK_MODE_Autoneg_BIT},
	{"Pause", LINK_MODE_Pause_BIT},
	{"Asym_Pause", LINK_MODE_Asym_Pause_BIT},
};

static int scan_advert(int argc, char *argv[], char *name, u64 *param)
{
	char *p, *p1, *p2;
	unsigned int i;

	p = findArgParam(argc, argv, name);

	if (p == NULL)
		return 0;

	*param = 0;

	for (p1 = p; *p1 != 0; p1 = p2) {
		for (p2 = p1; *p2 != 0 && *p2 != ',' && *p2 != '|' && *p2 != ';'; p2++);

		if (*p2 != 0) {
			*p2 = 0;
			p2++;
		}

		if (*p1 == 0)
			continue;

		for (i = 0; i < ARRAY_SIZE(phy_advert); i++) {
			if (strcasecmp(p1, phy_advert[i].name) == 0) {
				*param |= (u64)1 << phy_advert[i].bit;
				break;
			}
		}
	}

	return 1;
}

static int print_advert(char *buf, unsigned int size, u64 param)
{
	char *p = buf;
	unsigned int flag = 0;
	unsigned int total_len = 0;
	unsigned int len;
	unsigned int i;

	strncpy(buf, "none", size);
	buf[size - 1] = 0;

	for (i = 0; i < ARRAY_SIZE(phy_advert); i++) {
		if ((param & ((u64)1 << phy_advert[i].bit))) {
			len = strlen(phy_advert[i].name);

			if (flag == 0) {
				if (total_len + len + 1 >= size)
					break;

				flag++;
			} else {
				if (total_len + len + 4 >= size)
					break;

				p[0] = ' ';
				p[1] = '|';
				p[2] = ' ';
				p += 3;
				total_len += 3;
			}

			strcpy(p, phy_advert[i].name);
			p += len;
			total_len += len;
		}
	}

	return (int)total_len;
}

int gpy2xx_msec_main(int argc, char *argv[])
{
	s32 ret = 0;
	s32 cnt = 0;

	if (xstrncasecmp(argv[3], "gpy2xx_msec_init_ing_dev", strlen("gpy2xx_msec_init_ing_dev")) == 0) {
		ret = gpy2xx_msec_init_ing_dev(&phy);

		if (ret < 0) {
			printf("\nERROR: Init ING dev failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_init_egr_dev", strlen("gpy2xx_msec_init_egr_dev")) == 0) {
		ret = gpy2xx_msec_init_egr_dev(&phy);

		if (ret < 0) {
			printf("\nERROR: Init EGR dev failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_tr", strlen("gpy2xx_msec_config_ing_tr")) == 0) {
		struct transform_rec raw_tr;
		memset(&raw_tr, 0, sizeof(struct raw_trans_rec));

		cnt += scanParamArg(argc, argv, "sa_index", sizeof(raw_tr.sa_index), &raw_tr.sa_index);
		cnt += scanParamArg(argc, argv, "ca_type", sizeof(raw_tr.ccw.ca_type), &raw_tr.ccw.ca_type);
		cnt += scanKey_Arg(argc, argv, "key", (((raw_tr.ccw.ca_type == GCM_AES_128) || (raw_tr.ccw.ca_type == GCM_AES_XPN_128)) ? 16 : 32), (char *)raw_tr.cp.key);
		cnt += scanKey_Arg(argc, argv, "sci", sizeof(raw_tr.cp.sci), (char *)raw_tr.cp.sci);
		//cnt += scanParamArg(argc, argv, "seq0", sizeof(raw_tr.pn_rc.seq0), &raw_tr.pn_rc.seq0);
		//cnt += scanParamArg(argc, argv, "seq1", sizeof(raw_tr.pn_rc.seq1), &raw_tr.pn_rc.seq1);
		cnt += scanParamArg(argc, argv, "seq_num", 64, &raw_tr.pn_rc.seq_num);
		cnt += scanKey_Arg(argc, argv, "salt", sizeof(raw_tr.cp.c_salt.salt), (char *)raw_tr.cp.c_salt.salt);
		cnt += scanKey_Arg(argc, argv, "short_sci", sizeof(raw_tr.cp.c_salt.s_sci), (char *)raw_tr.cp.c_salt.s_sci);
		cnt += scanParamArg(argc, argv, "rep_window", sizeof(raw_tr.pn_rc.mask), &raw_tr.pn_rc.mask);

		ret = gpy2xx_msec_config_ing_tr(&phy, &raw_tr);

		if (ret < 0) {
			printf("\nERROR: Ingress TR set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_egr_tr", strlen("gpy2xx_msec_config_egr_tr")) == 0) {
		struct transform_rec raw_tr;
		memset(&raw_tr, 0, sizeof(struct raw_trans_rec));

		cnt += scanParamArg(argc, argv, "sa_index", sizeof(raw_tr.sa_index), &raw_tr.sa_index);
		cnt += scanParamArg(argc, argv, "ca_type", sizeof(raw_tr.ccw.ca_type), &raw_tr.ccw.ca_type);
		cnt += scanParamArg(argc, argv, "an", sizeof(raw_tr.ccw.an), &raw_tr.ccw.an);
		cnt += scanKey_Arg(argc, argv, "key", (((raw_tr.ccw.ca_type == GCM_AES_128) || (raw_tr.ccw.ca_type == GCM_AES_XPN_128)) ? 16 : 32), (char *)raw_tr.cp.key);
		cnt += scanKey_Arg(argc, argv, "sci", sizeof(raw_tr.cp.sci), (char *)raw_tr.cp.sci);
		//cnt += scanParamArg(argc, argv, "seq0", sizeof(raw_tr.pn_rc.seq0), &raw_tr.pn_rc.seq0);
		//cnt += scanParamArg(argc, argv, "seq1", sizeof(raw_tr.pn_rc.seq1), &raw_tr.pn_rc.seq1);
		cnt += scanParamArg(argc, argv, "seq_num", 64, &raw_tr.pn_rc.seq_num);
		cnt += scanKey_Arg(argc, argv, "salt", sizeof(raw_tr.cp.c_salt.salt), (char *)raw_tr.cp.c_salt.salt);
		cnt += scanKey_Arg(argc, argv, "short_sci", sizeof(raw_tr.cp.c_salt.s_sci), (char *)raw_tr.cp.c_salt.s_sci);
		cnt += scanParamArg(argc, argv, "next_sa_ind", sizeof(raw_tr.sa_update.sa_index), &raw_tr.sa_update.sa_index);
		cnt += scanParamArg(argc, argv, "sa_expirq", sizeof(raw_tr.sa_update.sa_expired_irq), &raw_tr.sa_update.sa_expired_irq);
		cnt += scanParamArg(argc, argv, "sa_indvalid", sizeof(raw_tr.sa_update.sa_index_valid), &raw_tr.sa_update.sa_index_valid);
		cnt += scanParamArg(argc, argv, "flow_index", sizeof(raw_tr.sa_update.flow_index), &raw_tr.sa_update.flow_index);
		cnt += scanParamArg(argc, argv, "sa_indupen", sizeof(raw_tr.sa_update.sa_ind_update_en), &raw_tr.sa_update.sa_ind_update_en);

		ret = gpy2xx_msec_config_egr_tr(&phy, &raw_tr);

		if (ret < 0) {
			printf("\nERROR: Egress TR set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_update_egr_sa_cw", strlen("gpy2xx_msec_update_egr_sa_cw")) == 0) {
		struct transform_rec raw_tr;
		memset(&raw_tr, 0, sizeof(struct raw_trans_rec));

		cnt += scanParamArg(argc, argv, "sa_index", sizeof(raw_tr.sa_index), &raw_tr.sa_index);
		cnt += scanParamArg(argc, argv, "ca_type", sizeof(raw_tr.ccw.ca_type), &raw_tr.ccw.ca_type);
		cnt += scanParamArg(argc, argv, "sn_type", sizeof(raw_tr.ccw.sn_type), &raw_tr.ccw.sn_type);
		cnt += scanParamArg(argc, argv, "next_sa_ind", sizeof(raw_tr.sa_update.sa_index), &raw_tr.sa_update.sa_index);
		cnt += scanParamArg(argc, argv, "sa_expirq", sizeof(raw_tr.sa_update.sa_expired_irq), &raw_tr.sa_update.sa_expired_irq);
		cnt += scanParamArg(argc, argv, "sa_indvalid", sizeof(raw_tr.sa_update.sa_index_valid), &raw_tr.sa_update.sa_index_valid);
		cnt += scanParamArg(argc, argv, "flow_index", sizeof(raw_tr.sa_update.flow_index), &raw_tr.sa_update.flow_index);
		cnt += scanParamArg(argc, argv, "sa_indupen", sizeof(raw_tr.sa_update.sa_ind_update_en), &raw_tr.sa_update.sa_ind_update_en);

		ret = gpy2xx_msec_update_egr_sa_cw(&phy, &raw_tr);

		if (ret < 0) {
			printf("\nERROR: Egress SA update set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_ing_tr", strlen("gpy2xx_msec_get_ing_tr")) == 0) {
		char buf[256];
		u32 _ca_type, _sn_type;
		struct raw_trans_rec raw_tr;
		memset(&raw_tr, 0, sizeof(struct raw_trans_rec));

		cnt += scanParamArg(argc, argv, "sa_index", sizeof(raw_tr.sa_index), &raw_tr.sa_index);

		ret = gpy2xx_msec_get_ing_tr(&phy, &raw_tr);

		if (ret < 0) {
			printf("\nERROR: Ingress TR get failed.\n");
			return -1;
		}

		int i;

		for (i = 0; i < ING_MAX_TR_SIZE_WORDS; i++) {
			printf("\nTRec[%02d] = %8x", i, raw_tr.raw_trans_rec[i]);
		}

		_sn_type = (raw_tr.raw_trans_rec[0] >> 28) & 0x3;
		_ca_type = (raw_tr.raw_trans_rec[0] >> 17) & 0x7;
		//salt= short_sci=7A30C118

		/* Compute Current Sequence Number word offset */
		if (_ca_type == 5) {
			if (_sn_type == SN_32_BIT) {
				struct ing_tr_32bpn_128bak *target_tr =
					(struct ing_tr_32bpn_128bak *) raw_tr.raw_trans_rec;
				printf("\nCrypto-Algorithm type = AES_CTR_128");
				printf("\n              SN type = 32_BIT");
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x%08x%08x", swap_uint32(target_tr->key[0]), swap_uint32(target_tr->key[1]), swap_uint32(target_tr->key[2]), swap_uint32(target_tr->key[3]));
				printf("\n                  Key = %s", buf);
				printf("\n                   SN = %08x", target_tr->seq);
				printf("\n   Replay Window size = %08x", target_tr->mask);
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x", swap_uint32(target_tr->iv0), swap_uint32(target_tr->iv1));
				printf("\n                  SCI = %s", buf);
			} else if (_sn_type == SN_64_BIT) {
				struct ing_tr_64bpn_128bak *target_tr =
					(struct ing_tr_64bpn_128bak *) raw_tr.raw_trans_rec;
				printf("\nCrypto-Algorithm type = AES_CTR_128");
				printf("\n              SN type = 32_BIT");
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x%08x%08x", swap_uint32(target_tr->key[0]), swap_uint32(target_tr->key[1]), swap_uint32(target_tr->key[2]), swap_uint32(target_tr->key[3]));
				printf("\n                  Key = %s", buf);
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x", target_tr->seq1, target_tr->seq0);
				printf("\n                   SN = %s", buf);
				printf("\n   Replay Window size = %08x", target_tr->mask);
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x%08x", swap_uint32(target_tr->iv0), swap_uint32(target_tr->iv1), swap_uint32(target_tr->iv2));
				printf("\n         Context-Salt = %s", buf);
			} else {
				printf("\nERROR: Invalid SN type.\n");
			}
		} else if (_ca_type == 7) {
			if (_sn_type == SN_32_BIT) {
				struct ing_tr_32bpn_256bak *target_tr =
					(struct ing_tr_32bpn_256bak *) raw_tr.raw_trans_rec;
				printf("\nCrypto-Algorithm type = AES_CTR_256");
				printf("\n              SN type = 32_BIT");
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x%08x%08x%08x%08x%08x%08x", swap_uint32(target_tr->key[0]), swap_uint32(target_tr->key[1]), swap_uint32(target_tr->key[2]), swap_uint32(target_tr->key[3]), swap_uint32(target_tr->key[4]), swap_uint32(target_tr->key[5]), swap_uint32(target_tr->key[6]), swap_uint32(target_tr->key[7]));
				printf("\n                  Key = %s", buf);
				printf("\n                   SN = %08x", target_tr->seq);
				printf("\n   Replay Window size = %08x", target_tr->mask);
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x", swap_uint32(target_tr->iv0), swap_uint32(target_tr->iv1));
				printf("\n                  SCI = %s", buf);
			} else if (_sn_type == SN_64_BIT) {
				struct ing_tr_64bpn_256bak *target_tr =
					(struct ing_tr_64bpn_256bak *) raw_tr.raw_trans_rec;
				printf("\nCrypto-Algorithm type = AES_CTR_128");
				printf("\n              SN type = 64_BIT");
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x%08x%08x%08x%08x%08x%08x", swap_uint32(target_tr->key[0]), swap_uint32(target_tr->key[1]), swap_uint32(target_tr->key[2]), swap_uint32(target_tr->key[3]), swap_uint32(target_tr->key[4]), swap_uint32(target_tr->key[5]), swap_uint32(target_tr->key[6]), swap_uint32(target_tr->key[7]));
				printf("\n                  Key = %s", buf);
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x", target_tr->seq1, target_tr->seq0);
				printf("\n                   SN = %s", buf);
				printf("\n   Replay Window size = %08x", target_tr->mask);
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x%08x", swap_uint32(target_tr->iv0), swap_uint32(target_tr->iv1), swap_uint32(target_tr->iv2));
				printf("\n         Context-Salt = %s", buf);
			} else {
				printf("\nERROR: Invalid SN type.\n");
			}
		} else {
			printf("\nERROR: Invalid Crypto-Algorithm type.\n");
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_egr_tr", strlen("gpy2xx_msec_get_egr_tr")) == 0) {
		char buf[256];
		u32 _ca_type, _sn_type;
		struct raw_trans_rec raw_tr;
		memset(&raw_tr, 0, sizeof(struct raw_trans_rec));

		cnt += scanParamArg(argc, argv, "sa_index", sizeof(raw_tr.sa_index), &raw_tr.sa_index);

		ret = gpy2xx_msec_get_egr_tr(&phy, &raw_tr);

		if (ret < 0) {
			printf("\nERROR: Egress TR get failed.\n");
			return -1;
		}

		int i;

		for (i = 0; i < EGR_MAX_TR_SIZE_WORDS; i++) {
			printf("\nTRec[%02d] = %8x", i, raw_tr.raw_trans_rec[i]);
		}

		_sn_type = (raw_tr.raw_trans_rec[0] >> 28) & 0x3;
		_ca_type = (raw_tr.raw_trans_rec[0] >> 17) & 0x7;

		/* Compute Current Sequence Number word offset */
		if (_ca_type == 5) {
			if (_sn_type == SN_32_BIT) {
				struct egr_tr_32bpn_128bak *target_tr =
					(struct egr_tr_32bpn_128bak *) raw_tr.raw_trans_rec;
				printf("\nCrypto-Algorithm type = AES_CTR_128");
				printf("\n              SN type = 32_BIT");
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x%08x%08x", swap_uint32(target_tr->key[0]), swap_uint32(target_tr->key[1]), swap_uint32(target_tr->key[2]), swap_uint32(target_tr->key[3]));
				printf("\n                  Key = %s", buf);
				printf("\n                   SN = %08x", target_tr->seq);
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x", swap_uint32(target_tr->iv0), swap_uint32(target_tr->iv1));
				printf("\n                  SCI = %s", buf);

				if (((target_tr->sa_upd_ctrl >> 15) & 1) == 1)
					printf("\n        Next SA index = %x", target_tr->sa_upd_ctrl & 0xFF);

				printf("\n       SA expired IRQ = %d", ((target_tr->sa_upd_ctrl >> 14) & 1));
				printf("\nFlow index of curr SA = %x", (target_tr->sa_upd_ctrl >> 16) & 0xFF);
				printf("\n   SA index update en = %d", ((target_tr->sa_upd_ctrl >> 31) & 1));
			} else if (_sn_type == SN_64_BIT) {
				struct egr_tr_64bpn_128bak *target_tr =
					(struct egr_tr_64bpn_128bak *) raw_tr.raw_trans_rec;
				printf("\nCrypto-Algorithm type = AES_CTR_128");
				printf("\n				SN type = 32_BIT");
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x%08x%08x", swap_uint32(target_tr->key[0]), swap_uint32(target_tr->key[1]), swap_uint32(target_tr->key[2]), swap_uint32(target_tr->key[3]));
				printf("\n					Key = %s", buf);
				printf("\n			         SN = %08x%08x", target_tr->seq1, target_tr->seq0);
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x", swap_uint32(target_tr->iv0), swap_uint32(target_tr->iv1));
				printf("\n                  SCI = %s", buf);
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x%08x", swap_uint32(target_tr->is0), swap_uint32(target_tr->is1), swap_uint32(target_tr->is2));
				printf("\n         Context-Salt = %s", buf);

				if (((target_tr->sa_upd_ctrl >> 15) & 1) == 1)
					printf("\n		  Next SA index = %x", target_tr->sa_upd_ctrl & 0xFF);

				printf("\n		 SA expired IRQ = %d", ((target_tr->sa_upd_ctrl >> 14) & 1));
				printf("\nFlow index of curr SA = %x", (target_tr->sa_upd_ctrl >> 16) & 0xFF);
				printf("\n   SA index update en	= %d", ((target_tr->sa_upd_ctrl >> 31) & 1));
			} else {
				printf("\nERROR: Invalid SN type.\n");
			}
		} else if (_ca_type == 7) {
			if (_sn_type == SN_32_BIT) {
				struct egr_tr_32bpn_256bak *target_tr =
					(struct egr_tr_32bpn_256bak *) raw_tr.raw_trans_rec;
				printf("\nCrypto-Algorithm type = AES_CTR_256");
				printf("\n				SN type = 32_BIT");
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x%08x%08x%08x%08x%08x%08x", swap_uint32(target_tr->key[0]), swap_uint32(target_tr->key[1]), swap_uint32(target_tr->key[2]), swap_uint32(target_tr->key[3]), swap_uint32(target_tr->key[4]), swap_uint32(target_tr->key[5]), swap_uint32(target_tr->key[6]), swap_uint32(target_tr->key[7]));
				printf("\n					Key = %s", buf);
				printf("\n					 SN = %08x", target_tr->seq);
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x", swap_uint32(target_tr->iv0), swap_uint32(target_tr->iv1));
				printf("\n					SCI = %s", buf);

				if (((target_tr->sa_upd_ctrl >> 15) & 1) == 1)
					printf("\n		  Next SA index = %x", target_tr->sa_upd_ctrl & 0xFF);

				printf("\n		 SA expired IRQ = %d", ((target_tr->sa_upd_ctrl >> 14) & 1));
				printf("\nFlow index of curr SA = %x", (target_tr->sa_upd_ctrl >> 16) & 0xFF);
				printf("\nSA index update en	= %d", ((target_tr->sa_upd_ctrl >> 31) & 1));
			} else if (_sn_type == SN_64_BIT) {
				struct egr_tr_64bpn_256bak *target_tr =
					(struct egr_tr_64bpn_256bak *) raw_tr.raw_trans_rec;
				printf("\nCrypto-Algorithm type = AES_CTR_128");
				printf("\n				SN type = 64_BIT");
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x%08x%08x%08x%08x%08x%08x", swap_uint32(target_tr->key[0]), swap_uint32(target_tr->key[1]), swap_uint32(target_tr->key[2]), swap_uint32(target_tr->key[3]), swap_uint32(target_tr->key[4]), swap_uint32(target_tr->key[5]), swap_uint32(target_tr->key[6]), swap_uint32(target_tr->key[7]));
				printf("\n					Key = %s", buf);
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x", target_tr->seq1, target_tr->seq0);
				printf("\n					 SN = %s", buf);
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x", swap_uint32(target_tr->iv0), swap_uint32(target_tr->iv1));
				printf("\n					SCI = %s", buf);
				snprintf(buf, ARRAY_SIZE(buf), "%08x%08x%08x", swap_uint32(target_tr->is0), swap_uint32(target_tr->is1), swap_uint32(target_tr->is2));
				printf("\n		   Context-Salt = %s", buf);

				if (((target_tr->sa_upd_ctrl >> 15) & 1) == 1)
					printf("\n		  Next SA index = %x", target_tr->sa_upd_ctrl & 0xFF);

				printf("\n		 SA expired IRQ = %d", ((target_tr->sa_upd_ctrl >> 14) & 1));
				printf("\nFlow index of curr SA = %x", (target_tr->sa_upd_ctrl >> 16) & 0xFF);
				printf("\nSA index update en	= %d", ((target_tr->sa_upd_ctrl >> 31) & 1));

			} else {
				printf("\nERROR: Invalid SN type.\n");
			}
		} else {
			printf("\nERROR: Invalid Crypto-Algorithm type.\n");
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_ing_pn", strlen("gpy2xx_msec_get_ing_pn")) == 0) {
		struct sa_current_pn curr_sn;
		memset(&curr_sn, 0, sizeof(struct sa_current_pn));

		cnt += scanParamArg(argc, argv, "sa_index", sizeof(curr_sn.sa_index), &curr_sn.sa_index);

		ret = gpy2xx_msec_get_ing_pn(&phy, &curr_sn);

		if (ret < 0) {
			printf("\nERROR: Ingress current SN get failed.\n");
			return -1;
		}

		printf("\nCurrent SN = 0x%llx", curr_sn.curr_pn);
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_egr_pn", strlen("gpy2xx_msec_get_egr_pn")) == 0) {
		struct sa_current_pn curr_sn;
		memset(&curr_sn, 0, sizeof(struct sa_current_pn));

		cnt += scanParamArg(argc, argv, "sa_index", sizeof(curr_sn.sa_index), &curr_sn.sa_index);

		ret = gpy2xx_msec_get_egr_pn(&phy, &curr_sn);

		if (ret < 0) {
			printf("\nERROR: Egress current SN get failed.\n");
			return -1;
		}

		printf("\nCurrent SN = 0x%llx", curr_sn.curr_pn);
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_clear_ing_tr", strlen("gpy2xx_msec_clear_ing_tr")) == 0) {
		struct transform_rec raw_tr;
		memset(&raw_tr, 0, sizeof(struct raw_trans_rec));

		cnt += scanParamArg(argc, argv, "sa_index", sizeof(raw_tr.sa_index), &raw_tr.sa_index);

		ret = gpy2xx_msec_clear_ing_tr(&phy, &raw_tr);

		if (ret < 0) {
			printf("\nERROR: Ingress TR clear failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_clear_egr_tr", strlen("gpy2xx_msec_clear_egr_tr")) == 0) {
		struct transform_rec raw_tr;
		memset(&raw_tr, 0, sizeof(struct raw_trans_rec));

		cnt += scanParamArg(argc, argv, "sa_index", sizeof(raw_tr.sa_index), &raw_tr.sa_index);

		ret = gpy2xx_msec_clear_egr_tr(&phy, &raw_tr);

		if (ret < 0) {
			printf("\nERROR: Egress TR clear failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_sam_eex", strlen("gpy2xx_msec_config_ing_sam_eex")) == 0) {
		struct raw_sam_ee raw_ee;
		memset(&raw_ee, 0, sizeof(struct raw_sam_ee));

		//cnt += scanParamArg(argc, argv, "word_num", sizeof(raw_ee.word_num), &raw_ee.word_num);
		cnt += scanParamArg(argc, argv, "eex_type", sizeof(raw_ee.eex_type), &raw_ee.eex_type);
		cnt += scanParamArg(argc, argv, "eex_word0", sizeof(raw_ee.raw_sam_ee[0]), &raw_ee.raw_sam_ee[0]);

		ret = gpy2xx_msec_config_ing_sam_eex(&phy, &raw_ee);

		if (ret < 0) {
			printf("\nERROR: Ingress SAM EE type set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_egr_sam_eex", strlen("gpy2xx_msec_config_egr_sam_eex")) == 0) {
		struct raw_sam_ee raw_ee;
		memset(&raw_ee, 0, sizeof(struct raw_sam_ee));

		//cnt += scanParamArg(argc, argv, "word_num", sizeof(raw_ee.word_num), &raw_ee.word_num);
		cnt += scanParamArg(argc, argv, "eex_type", sizeof(raw_ee.eex_type), &raw_ee.eex_type);
		cnt += scanParamArg(argc, argv, "eex_word0", sizeof(raw_ee.raw_sam_ee[0]), &raw_ee.raw_sam_ee[0]);

		ret = gpy2xx_msec_config_egr_sam_eex(&phy, &raw_ee);

		if (ret < 0) {
			printf("\nERROR: Egress SAM EE type set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_ing_sam_eef", strlen("gpy2xx_msec_get_ing_sam_eef")) == 0) {
		int i;
		struct raw_sam_ee raw_ee;
		memset(&raw_ee, 0, sizeof(struct raw_sam_ee));

		//cnt += scanParamArg(argc, argv, "word_num", sizeof(raw_ee.word_num), &raw_ee.word_num);

		ret = gpy2xx_msec_get_ing_sam_eef(&phy, &raw_ee);

		if (ret < 0) {
			printf("\nERROR: Ingress SAM EE flags get failed.\n");
			return -1;
		}

		for (i = 0; i < (MACSEC_MAX_SA_RULES / 32); i++) {
			printf("\nSAM EE flags[%02d] = %8x", i, raw_ee.raw_sam_ee[i]);
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_egr_sam_eef", strlen("gpy2xx_msec_get_egr_sam_eef")) == 0) {
		int i;
		struct raw_sam_ee raw_ee;
		memset(&raw_ee, 0, sizeof(struct raw_sam_ee));

		//cnt += scanParamArg(argc, argv, "word_num", sizeof(raw_ee.word_num), &raw_ee.word_num);

		ret = gpy2xx_msec_get_egr_sam_eef(&phy, &raw_ee);

		if (ret < 0) {
			printf("\nERROR: Egress SAM EE flags get failed.\n");
			return -1;
		}

		for (i = 0; i < (MACSEC_MAX_SA_RULES / 32); i++) {
			printf("\nSAM EE flags[%02d] = %8x", i, raw_ee.raw_sam_ee[i]);
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_sam_eec", strlen("gpy2xx_msec_config_ing_sam_eec")) == 0) {
		struct sam_ee_ctrl ee_ctrl;
		memset(&ee_ctrl, 0, sizeof(struct sam_ee_ctrl));

		cnt += scanParamArg(argc, argv, "sam_ind_set", sizeof(ee_ctrl.sam_index_set), &ee_ctrl.sam_index_set);
		cnt += scanParamArg(argc, argv, "set_single", sizeof(ee_ctrl.set_enable), &ee_ctrl.set_enable);
		cnt += scanParamArg(argc, argv, "set_all", sizeof(ee_ctrl.set_all), &ee_ctrl.set_all);
		cnt += scanParamArg(argc, argv, "sam_ind_clr", sizeof(ee_ctrl.sam_index_clear), &ee_ctrl.sam_index_clear);
		cnt += scanParamArg(argc, argv, "clr_single", sizeof(ee_ctrl.clear_enable), &ee_ctrl.clear_enable);
		cnt += scanParamArg(argc, argv, "clr_all", sizeof(ee_ctrl.clear_all), &ee_ctrl.clear_all);

		ret = gpy2xx_msec_config_ing_sam_eec(&phy, &ee_ctrl);

		if (ret < 0) {
			printf("\nERROR: Ingress SAM EE control set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_egr_sam_eec", strlen("gpy2xx_msec_config_egr_sam_eec")) == 0) {
		struct sam_ee_ctrl ee_ctrl;
		memset(&ee_ctrl, 0, sizeof(struct sam_ee_ctrl));

		cnt += scanParamArg(argc, argv, "sam_ind_set", sizeof(ee_ctrl.sam_index_set), &ee_ctrl.sam_index_set);
		cnt += scanParamArg(argc, argv, "set_single", sizeof(ee_ctrl.set_enable), &ee_ctrl.set_enable);
		cnt += scanParamArg(argc, argv, "set_all", sizeof(ee_ctrl.set_all), &ee_ctrl.set_all);
		cnt += scanParamArg(argc, argv, "sam_ind_clr", sizeof(ee_ctrl.sam_index_clear), &ee_ctrl.sam_index_clear);
		cnt += scanParamArg(argc, argv, "clr_single", sizeof(ee_ctrl.clear_enable), &ee_ctrl.clear_enable);
		cnt += scanParamArg(argc, argv, "clr_all", sizeof(ee_ctrl.clear_all), &ee_ctrl.clear_all);

		ret = gpy2xx_msec_config_egr_sam_eec(&phy, &ee_ctrl);

		if (ret < 0) {
			printf("\nERROR: Egress SAM EE control set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_sam_fca", strlen("gpy2xx_msec_config_ing_sam_fca")) == 0) {
		struct sa_match_fca sam_fca;
		memset(&sam_fca, 0, sizeof(struct sa_match_fca));

		cnt += scanParamArg(argc, argv, "flow_index", sizeof(sam_fca.flow_index), &sam_fca.flow_index);
		cnt += scanParamArg(argc, argv, "flow_type", sizeof(sam_fca.flow_type), &sam_fca.flow_type);
		cnt += scanParamArg(argc, argv, "dest_port", sizeof(sam_fca.dest_port), &sam_fca.dest_port);
		cnt += scanParamArg(argc, argv, "drop_nonres", sizeof(sam_fca.drop_non_reserved), &sam_fca.drop_non_reserved);
		cnt += scanParamArg(argc, argv, "crypt_auth", sizeof(sam_fca.flow_crypt_auth), &sam_fca.flow_crypt_auth);
		cnt += scanParamArg(argc, argv, "drop_act", sizeof(sam_fca.drop_action), &sam_fca.drop_action);
		cnt += scanParamArg(argc, argv, "sa_index", sizeof(sam_fca.sa_index), &sam_fca.sa_index);
		cnt += scanParamArg(argc, argv, "rep_protect", sizeof(sam_fca.replay_protect), &sam_fca.replay_protect);
		cnt += scanParamArg(argc, argv, "sa_inuse", sizeof(sam_fca.sa_in_use), &sam_fca.sa_in_use);
		cnt += scanParamArg(argc, argv, "validate_level", sizeof(sam_fca.validate_frames), &sam_fca.validate_frames);
		cnt += scanParamArg(argc, argv, "ivl_mode", sizeof(sam_fca.iv), &sam_fca.iv);
		cnt += scanParamArg(argc, argv, "verify_icv", sizeof(sam_fca.icv_verify), &sam_fca.icv_verify);
		cnt += scanParamArg(argc, argv, "ca_bp_len", sizeof(sam_fca.crypt_auth_bypass_len), &sam_fca.crypt_auth_bypass_len);

		ret = gpy2xx_msec_config_ing_sam_fca(&phy, &sam_fca);

		if (ret < 0) {
			printf("\nERROR: Ingress SAM flow control action set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_egr_sam_fca", strlen("gpy2xx_msec_config_egr_sam_fca")) == 0) {
		struct sa_match_fca sam_fca;
		memset(&sam_fca, 0, sizeof(struct sa_match_fca));

		cnt += scanParamArg(argc, argv, "flow_index", sizeof(sam_fca.flow_index), &sam_fca.flow_index);
		cnt += scanParamArg(argc, argv, "flow_type", sizeof(sam_fca.flow_type), &sam_fca.flow_type);
		cnt += scanParamArg(argc, argv, "dest_port", sizeof(sam_fca.dest_port), &sam_fca.dest_port);
		cnt += scanParamArg(argc, argv, "crypt_auth", sizeof(sam_fca.flow_crypt_auth), &sam_fca.flow_crypt_auth);
		cnt += scanParamArg(argc, argv, "drop_act", sizeof(sam_fca.drop_action), &sam_fca.drop_action);
		cnt += scanParamArg(argc, argv, "sa_index", sizeof(sam_fca.sa_index), &sam_fca.sa_index);
		cnt += scanParamArg(argc, argv, "frame_protect", sizeof(sam_fca.protect_frame), &sam_fca.protect_frame);
		cnt += scanParamArg(argc, argv, "sa_inuse", sizeof(sam_fca.sa_in_use), &sam_fca.sa_in_use);
		cnt += scanParamArg(argc, argv, "include_sci", sizeof(sam_fca.include_sci), &sam_fca.include_sci);
		cnt += scanParamArg(argc, argv, "use_es", sizeof(sam_fca.use_es), &sam_fca.use_es);
		cnt += scanParamArg(argc, argv, "use_scb", sizeof(sam_fca.use_scb), &sam_fca.use_scb);
		cnt += scanParamArg(argc, argv, "tag_bpsize", sizeof(sam_fca.tag_bypass_size), &sam_fca.tag_bypass_size);
		cnt += scanParamArg(argc, argv, "sa_indup", sizeof(sam_fca.sa_index_update), &sam_fca.sa_index_update);
		cnt += scanParamArg(argc, argv, "conf_offset", sizeof(sam_fca.conf_offset), &sam_fca.conf_offset);
		cnt += scanParamArg(argc, argv, "conf_protect", sizeof(sam_fca.conf_protect), &sam_fca.conf_protect);
		cnt += scanParamArg(argc, argv, "ivl_mode", sizeof(sam_fca.iv), &sam_fca.iv);
		cnt += scanParamArg(argc, argv, "append_icv", sizeof(sam_fca.icv_append), &sam_fca.icv_append);
		cnt += scanParamArg(argc, argv, "ca_bp_len", sizeof(sam_fca.crypt_auth_bypass_len), &sam_fca.crypt_auth_bypass_len);

		ret = gpy2xx_msec_config_egr_sam_fca(&phy, &sam_fca);

		if (ret < 0) {
			printf("\nERROR: Egress SAM flow control action set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_ing_sam_fca", strlen("gpy2xx_msec_get_ing_sam_fca")) == 0) {
		struct sam_pkt_fca sam_fca;
		memset(&sam_fca, 0, sizeof(struct sam_pkt_fca));

		cnt += scanParamArg(argc, argv, "flow_index", sizeof(sam_fca.flow_index), &sam_fca.flow_index);

		ret = gpy2xx_msec_get_ing_sam_fca(&phy, &sam_fca);

		if (ret < 0) {
			printf("\nERROR: Ingress SAM flow control action get failed.\n");
			return -1;
		}

		printf("\nIngress FCA raw %x", sam_fca.dec_enc.raw_fca_macsec);

		if (sam_fca.dec_enc.flow_type == SAM_FCA_FLOW_BYPASS) {
			if (sam_fca.dec_enc.flow_crypt_auth == 1) {
				printf("\nIngress FCA - Crypto_Auth");
				printf("\nIV mode %x", sam_fca.crypt_auth.iv);
				printf("\nRetrieve and verify the ICV %x", sam_fca.crypt_auth.icv_verify);
				printf("\nAAD length %x", sam_fca.crypt_auth.conf_offset);
			} else {
				printf("\nIngress FCA - Bypass");
				printf("\nSA in-use %x", sam_fca.bypass_drop.sa_in_use);
			}
		} else if (sam_fca.dec_enc.flow_type == SAM_FCA_FLOW_DROP) {
			printf("\nIngress FCA - Drop");
			printf("\nSA in-use %x", sam_fca.bypass_drop.sa_in_use);
		} else if (sam_fca.dec_enc.flow_type == SAM_FCA_FLOW_INGRESS) {
			printf("\nIngress FCA - Decryption");
			printf("\nReplay protection %x, SA in-use %x, Validation level %x, SA ind update %x, Conf offset %x, Conf protection %x",
			       sam_fca.dec_enc.rep_prot_fr, sam_fca.dec_enc.sa_in_use, sam_fca.dec_enc.vlevel_es_scb, sam_fca.dec_enc.sa_index_update, sam_fca.dec_enc.conf_offset, sam_fca.dec_enc.conf_protect);
		} else {
			printf("\nIngress FCA - Invalid");
		}

		printf("\nDst port %x, Drop NR %x, Drop act %x, SA ind %x",
		       sam_fca.dec_enc.dest_port, sam_fca.dec_enc.drop_non_reserved, sam_fca.dec_enc.drop_action, sam_fca.dec_enc.sa_index);
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_egr_sam_fca", strlen("gpy2xx_msec_get_egr_sam_fca")) == 0) {
		struct sam_pkt_fca sam_fca;
		memset(&sam_fca, 0, sizeof(struct sam_pkt_fca));

		cnt += scanParamArg(argc, argv, "flow_index", sizeof(sam_fca.flow_index), &sam_fca.flow_index);

		ret = gpy2xx_msec_get_egr_sam_fca(&phy, &sam_fca);

		if (ret < 0) {
			printf("\nERROR: Egress SAM flow control action get failed.\n");
			return -1;
		}

		printf("\nEgress FCA raw %x", sam_fca.dec_enc.raw_fca_macsec);

		if (sam_fca.dec_enc.flow_type == SAM_FCA_FLOW_BYPASS) {
			if (sam_fca.dec_enc.flow_crypt_auth == 1) {
				printf("\nEgress FCA - Crypto_Auth");
				printf("\nIV mode %x", sam_fca.crypt_auth.iv);
				printf("\nAppend the calculated ICV %x", sam_fca.crypt_auth.icv_append);
				printf("\nAAD length %x", sam_fca.crypt_auth.conf_offset);
				printf("\nConf protection %x", sam_fca.crypt_auth.conf_protect);
			} else {
				printf("\nEgress FCA - Bypass");
				printf("\nSA in-use %x", sam_fca.bypass_drop.sa_in_use);
			}
		} else if (sam_fca.dec_enc.flow_type == SAM_FCA_FLOW_DROP) {
			printf("\nEgress FCA - Drop");
			printf("\nSA in-use %x", sam_fca.bypass_drop.sa_in_use);
		} else if (sam_fca.dec_enc.flow_type == SAM_FCA_FLOW_EGRESS) {
			printf("\nEgress FCA - Encryption");
			printf("\nFrame protection %x, SA in-use %x, Expl SCI %x, ES %x, SCB %x, Tag bp size %x, SA ind update %x, Conf offset %x, Conf protection %x",
			       sam_fca.dec_enc.rep_prot_fr, sam_fca.dec_enc.sa_in_use, sam_fca.dec_enc.include_sci, sam_fca.dec_enc.vlevel_es_scb & 1, (sam_fca.dec_enc.vlevel_es_scb >> 1) & 1, sam_fca.dec_enc.tag_bypass_size, sam_fca.dec_enc.sa_index_update, sam_fca.dec_enc.conf_offset, sam_fca.dec_enc.conf_protect);
		} else {
			printf("\nEgress FCA - Invalid");
		}

		printf("\nDst port %x, Drop act %x, SA ind %x",
		       sam_fca.dec_enc.dest_port, sam_fca.dec_enc.drop_action, sam_fca.dec_enc.sa_index);
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_clear_ing_sam_fca", strlen("gpy2xx_msec_clear_ing_sam_fca")) == 0) {
		struct sa_match_fca sam_fca;
		memset(&sam_fca, 0, sizeof(struct sa_match_fca));

		cnt += scanParamArg(argc, argv, "flow_index", sizeof(sam_fca.flow_index), &sam_fca.flow_index);

		ret = gpy2xx_msec_clear_ing_sam_fca(&phy, &sam_fca);

		if (ret < 0) {
			printf("\nERROR: Ingress SAM flow control clear failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_clear_egr_sam_fca", strlen("gpy2xx_msec_clear_egr_sam_fca")) == 0) {
		struct sa_match_fca sam_fca;
		memset(&sam_fca, 0, sizeof(struct sa_match_fca));

		cnt += scanParamArg(argc, argv, "flow_index", sizeof(sam_fca.flow_index), &sam_fca.flow_index);

		ret = gpy2xx_msec_clear_egr_sam_fca(&phy, &sam_fca);

		if (ret < 0) {
			printf("\nERROR: Egress SAM flow control clear failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_sam_rule", strlen("gpy2xx_msec_config_ing_sam_rule")) == 0) {
		u8 in_data;
		struct sa_match_param match_par;
		memset(&match_par, 0, sizeof(struct sa_match_param));

		cnt += scanParamArg(argc, argv, "rule_index", sizeof(match_par.rule_index), &match_par.rule_index);
		cnt += scanMAC_Arg(argc, argv, "sa_mac", match_par.sa_mac);
		cnt += scanMAC_Arg(argc, argv, "da_mac", match_par.da_mac);
		cnt += scanParamArg(argc, argv, "eth_type", sizeof(match_par.eth_type), &match_par.eth_type);
		cnt += scanParamArg(argc, argv, "vlan_id", sizeof(match_par.vlan_id), &match_par.vlan_id);
		cnt += scanParamArg(argc, argv, "misc", sizeof(match_par.misc_par.misc), &match_par.misc_par.misc);
		cnt += scanKey_Arg(argc, argv, "sci", sizeof(match_par.sci), (char *)match_par.sci);
		cnt += scanParamArg(argc, argv, "mask", sizeof(match_par.mask_par.mask), &match_par.mask_par.mask);
		cnt += scanParamArg(argc, argv, "flow_index", sizeof(in_data), &in_data);
		match_par.extn_par.flow_index = in_data;

		ret = gpy2xx_msec_config_ing_sam_rule(&phy, &match_par);

		if (ret < 0) {
			printf("\nERROR: Ingress SAM set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_egr_sam_rule", strlen("gpy2xx_msec_config_egr_sam_rule")) == 0) {
		u8 in_data;
		struct sa_match_param match_par;
		memset(&match_par, 0, sizeof(struct sa_match_param));

		cnt += scanParamArg(argc, argv, "rule_index", sizeof(match_par.rule_index), &match_par.rule_index);
		cnt += scanMAC_Arg(argc, argv, "sa_mac", match_par.sa_mac);
		cnt += scanMAC_Arg(argc, argv, "da_mac", match_par.da_mac);
		cnt += scanParamArg(argc, argv, "eth_type", sizeof(match_par.eth_type), &match_par.eth_type);
		cnt += scanParamArg(argc, argv, "vlan_id", sizeof(match_par.vlan_id), &match_par.vlan_id);
		cnt += scanParamArg(argc, argv, "misc", sizeof(match_par.misc_par.misc), &match_par.misc_par.misc);
		cnt += scanKey_Arg(argc, argv, "sci", sizeof(match_par.sci), (char *)match_par.sci);
		cnt += scanParamArg(argc, argv, "mask", sizeof(match_par.mask_par.mask), &match_par.mask_par.mask);
		cnt += scanParamArg(argc, argv, "flow_index", sizeof(in_data), &in_data);
		match_par.extn_par.flow_index = in_data;

		ret = gpy2xx_msec_config_egr_sam_rule(&phy, &match_par);

		if (ret < 0) {
			printf("\nERROR: Egress SAM set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_clear_ing_sam_rule", strlen("gpy2xx_msec_clear_ing_sam_rule")) == 0) {
		struct sa_match_param match_par;
		memset(&match_par, 0, sizeof(struct sa_match_param));

		cnt += scanParamArg(argc, argv, "rule_index", sizeof(match_par.rule_index), &match_par.rule_index);

		ret = gpy2xx_msec_clear_ing_sam_rule(&phy, &match_par);

		if (ret < 0) {
			printf("\nERROR: Ingress SAM clear failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_clear_egr_sam_rule", strlen("gpy2xx_msec_clear_egr_sam_rule")) == 0) {
		struct sa_match_param match_par;
		memset(&match_par, 0, sizeof(struct sa_match_param));

		cnt += scanParamArg(argc, argv, "rule_index", sizeof(match_par.rule_index), &match_par.rule_index);

		ret = gpy2xx_msec_clear_egr_sam_rule(&phy, &match_par);

		if (ret < 0) {
			printf("\nERROR: Egress SAM clear failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_vlan_parse", strlen("gpy2xx_msec_config_ing_vlan_parse")) == 0) {
		u8 in_data;
		struct sam_vlan_parse match_par;
		memset(&match_par, 0, sizeof(struct sam_vlan_parse));

		cnt += scanParamArg(argc, argv, "qinq_en", sizeof(in_data), &in_data);
		match_par.outer_tag.parse_qinq = in_data;
		cnt += scanParamArg(argc, argv, "stag_en", sizeof(in_data), &in_data);
		match_par.outer_tag.parse_stag = in_data;
		cnt += scanParamArg(argc, argv, "qtag_en", sizeof(in_data), &in_data);
		match_par.outer_tag.parse_qtag = in_data;

		ret = gpy2xx_msec_config_ing_vlan_parse(&phy, &match_par);

		if (ret < 0) {
			printf("\nERROR: Ingress SAM VLAN control set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_egr_vlan_parse", strlen("gpy2xx_msec_config_egr_vlan_parse")) == 0) {
		u8 in_data;
		struct sam_vlan_parse match_par;
		memset(&match_par, 0, sizeof(struct sam_vlan_parse));

		cnt += scanParamArg(argc, argv, "qinq_en", sizeof(in_data), &in_data);
		match_par.outer_tag.parse_qinq = in_data;
		cnt += scanParamArg(argc, argv, "stag_en", sizeof(in_data), &in_data);
		match_par.outer_tag.parse_stag = in_data;
		cnt += scanParamArg(argc, argv, "qtag_en", sizeof(in_data), &in_data);
		match_par.outer_tag.parse_qtag = in_data;

		ret = gpy2xx_msec_config_egr_vlan_parse(&phy, &match_par);

		if (ret < 0) {
			printf("\nERROR: Egress SAM VLAN control set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_ing_sam_rule", strlen("gpy2xx_msec_get_ing_sam_rule")) == 0) {
		int i;
		struct raw_sam_param raw_sam;
		memset(&raw_sam, 0, sizeof(struct raw_sam_param));

		cnt += scanParamArg(argc, argv, "rule_index", sizeof(raw_sam.rule_index), &raw_sam.rule_index);

		ret = gpy2xx_msec_get_ing_sam_rule(&phy, &raw_sam);

		if (ret < 0) {
			printf("\nERROR: Ingress SAM get failed.\n");
			return -1;
		}

		for (i = 0; i < IE_MAX_SAM_SIZE_WORDS; i++) {
			printf("\nSAMParam[%02d] = %8x", i, raw_sam.raw_sam_param[i]);
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_egr_sam_rule", strlen("gpy2xx_msec_get_egr_sam_rule")) == 0) {
		int i;
		struct raw_sam_param raw_sam;
		memset(&raw_sam, 0, sizeof(struct raw_sam_param));

		cnt += scanParamArg(argc, argv, "rule_index", sizeof(raw_sam.rule_index), &raw_sam.rule_index);

		ret = gpy2xx_msec_get_egr_sam_rule(&phy, &raw_sam);

		if (ret < 0) {
			printf("\nERROR: Egress SAM get failed.\n");
			return -1;
		}

		for (i = 0; i < IE_MAX_SAM_SIZE_WORDS; i++) {
			printf("\nSAMParam[%02d] = %8x", i, raw_sam.raw_sam_param[i]);
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_cc_rule", strlen("gpy2xx_msec_config_ing_cc_rule")) == 0) {
		u32 data;
		struct icc_match_param cc_match;
		memset(&cc_match, 0, sizeof(struct icc_match_param));

		cnt += scanParamArg(argc, argv, "icc_index", sizeof(cc_match.icc_index), &cc_match.icc_index);
		//cnt += scanParamArg(argc, argv, "ovlan_id", sizeof(data), &data);
		//cc_match.vlan_eth.vlan_id = data;
		cnt += scanParamArg(argc, argv, "eth_type", sizeof(data), &data);
		cc_match.vlan_eth.payload_e_type = data;
		cnt += scanParamArg(argc, argv, "misc", sizeof(data), &data);
		cc_match.icc_misc.raw_iccm_misc = data;
		//cnt += scanParamArg(argc, argv, "ivlan_id", sizeof(data), &data);
		//cc_match.icc_ivl.vlan_id_inner = data;
		//cnt += scanParamArg(argc, argv, "ivlan_up", sizeof(data), &data);
		//cc_match.icc_ivl.vlan_up_inner = data;

		ret = gpy2xx_msec_config_ing_cc_rule(&phy, &cc_match);

		if (ret < 0) {
			printf("\nERROR: Ingress CC rule set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_ing_cc_rule", strlen("gpy2xx_msec_get_ing_cc_rule")) == 0) {
		struct icc_match_param cc_match;
		memset(&cc_match, 0, sizeof(struct icc_match_param));

		cnt += scanParamArg(argc, argv, "icc_index", sizeof(cc_match.icc_index), &cc_match.icc_index);

		ret = gpy2xx_msec_get_ing_cc_rule(&phy, &cc_match);

		if (ret < 0) {
			printf("\nERROR: Ingress CC rule get failed.\n");
			return -1;
		}

		printf("\nIngress CC match parameters:\n----------------\n");
		printf("\nICCParam[00] = %8x", cc_match.vlan_eth.raw_iccm_vleth);
		printf("\nICCParam[01] = %8x", cc_match.icc_misc.raw_iccm_misc);
		//printf("\nICCParam[02] = %8x", cc_match.icc_ivl.raw_iccm_ivl);
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_cc_eef", strlen("gpy2xx_msec_config_ing_cc_eef")) == 0) {
		struct raw_icc_ee raw_cc;
		memset(&raw_cc, 0, sizeof(struct raw_icc_ee));

		//cnt += scanParamArg(argc, argv, "word_num", sizeof(raw_ee.word_num), &raw_ee.word_num);
		cnt += scanParamArg(argc, argv, "eec_word0", sizeof(raw_cc.raw_icc_ee[0]), &raw_cc.raw_icc_ee[0]);

		ret = gpy2xx_msec_config_ing_cc_eef(&phy, &raw_cc);

		if (ret < 0) {
			printf("\nERROR: ICC EE flags set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_ing_cc_eef", strlen("gpy2xx_msec_get_ing_cc_eef")) == 0) {
		int i;
		struct raw_icc_ee raw_cc;
		memset(&raw_cc, 0, sizeof(struct raw_icc_ee));

		//cnt += scanParamArg(argc, argv, "word_num", sizeof(raw_ee.word_num), &raw_ee.word_num);

		ret = gpy2xx_msec_get_ing_cc_eef(&phy, &raw_cc);

		if (ret < 0) {
			printf("\nERROR: ICC EE flags get failed.\n");
			return -1;
		}

		for (i = 0; i < (MACSEC_MAX_SA_RULES / 32); i++) {
			printf("\nICC EE flags[%02d] = %8x", i, raw_cc.raw_icc_ee[i]);
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_cc_eec", strlen("gpy2xx_msec_config_ing_cc_eec")) == 0) {
		u32 data;
		struct icc_ee_ctrl ee_ctrl;
		memset(&ee_ctrl, 0, sizeof(struct icc_ee_ctrl));

		cnt += scanParamArg(argc, argv, "icc_ind_set", sizeof(data), &data);
		ee_ctrl.cc_index_set = data;
		cnt += scanParamArg(argc, argv, "set_single", sizeof(data), &data);
		ee_ctrl.set_enable = data;
		cnt += scanParamArg(argc, argv, "set_all", sizeof(data), &data);
		ee_ctrl.set_all = data;
		cnt += scanParamArg(argc, argv, "icc_ind_clr", sizeof(data), &data);
		ee_ctrl.cc_index_clear = data;
		cnt += scanParamArg(argc, argv, "clr_single", sizeof(data), &data);
		ee_ctrl.clear_enable = data;
		cnt += scanParamArg(argc, argv, "clr_all", sizeof(data), &data);
		ee_ctrl.clear_all = data;

		ret = gpy2xx_msec_config_ing_cc_eec(&phy, &ee_ctrl);

		if (ret < 0) {
			printf("\nERROR: Ingress CC EE control set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_cc_ctrl", strlen("gpy2xx_msec_config_ing_cc_ctrl")) == 0) {
		struct ing_cc_ctrl cc_ctrl;
		memset(&cc_ctrl, 0, sizeof(struct ing_cc_ctrl));

		cnt += scanParamArg(argc, argv, "nm_act", sizeof(cc_ctrl.nm_act), &cc_ctrl.nm_act);
		cnt += scanParamArg(argc, argv, "nm_ctrl_act", sizeof(cc_ctrl.nm_ctrl_act), &cc_ctrl.nm_ctrl_act);

		ret = gpy2xx_msec_config_ing_cc_ctrl(&phy, &cc_ctrl);

		if (ret < 0) {
			printf("\nERROR: Ingress CC Control flags set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_ing_stats_summ", strlen("gpy2xx_msec_get_ing_stats_summ")) == 0) {
		int i;
		struct stats_summary xxx_summ;
		struct ing_global_bmap counter_bmap;
		memset(&xxx_summ, 0, sizeof(struct stats_summary));

		ret = gpy2xx_msec_get_ing_stats_summ(&phy, &xxx_summ);

		if (ret < 0) {
			printf("\nERROR: Ingress stats summary get failed.\n");
			return -1;
		}

		printf("\nStatistics summary flags:\n----------------\n");

		for (i = 0; i < (MACSEC_MAX_SA_RULES / 32); i++) {
			printf("\nPer-SA summary[%02d] = %8x", i, xxx_summ.psa_sum_word[i]);
		}

		printf("\nGlobal statistics summary = %8x", xxx_summ.glb_summ_word);
		counter_bmap.raw_global_bmap = xxx_summ.glb_summ_word;

		if (counter_bmap.TransformErrorPkts)
			printf("\nTransformErrorPkts counter expired");

		if (counter_bmap.InPktsCtrl)
			printf("\nInPktsCtrl counter expired");

		if (counter_bmap.InPktsNoTag)
			printf("\nInPktsNoTag counter expired");

		if (counter_bmap.InPktsUntagged)
			printf("\nInPktsUntagged counter expired");

		if (counter_bmap.InPktsTagged)
			printf("\nInPktsTagged counter expired");

		if (counter_bmap.InPktsBadTag)
			printf("\nInPktsBadTag counter expired");

		if (counter_bmap.InPktsUntaggedMiss)
			printf("\nInPktsUntaggedMiss counter expired");

		if (counter_bmap.InPktsNoSCI)
			printf("\nInPktsNoSCI counter expired");

		if (counter_bmap.InPktsUnknownSCI)
			printf("\nInPktsUnknownSCI counter expired");

		if (counter_bmap.InConsistCheckControlledNotPass)
			printf("\nInConsistCheckControlledNotPass counter expired");

		if (counter_bmap.InConsistCheckUncontrolledNotPass)
			printf("\nInConsistCheckUncontrolledNotPass counter expired");

		if (counter_bmap.InConsistCheckControlledPass)
			printf("\nInConsistCheckControlledPass counter expired");

		if (counter_bmap.InConsistCheckUncontrolledPass)
			printf("\nInConsistCheckUncontrolledPass counter expired");

		if (counter_bmap.InOverSizePkts)
			printf("\nInOverSizePkts counter expired");
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_egr_stats_summ", strlen("gpy2xx_msec_get_egr_stats_summ")) == 0) {
		int i;
		struct stats_summary xxx_summ;
		struct egr_global_bmap counter_bmap;
		memset(&xxx_summ, 0, sizeof(struct stats_summary));

		ret = gpy2xx_msec_get_egr_stats_summ(&phy, &xxx_summ);

		if (ret < 0) {
			printf("\nERROR: Ingress stats summary get failed.\n");
			return -1;
		}

		printf("\nStatistics summary flags:\n----------------\n");

		for (i = 0; i < (MACSEC_MAX_SA_RULES / 32); i++) {
			printf("\nPer-SA summary[%02d] = %8x", i, xxx_summ.psa_sum_word[i]);
		}

		printf("\nGlobal statistics summary = %8x", xxx_summ.glb_summ_word);
		counter_bmap.raw_global_bmap = xxx_summ.glb_summ_word;

		if (counter_bmap.TransformErrorPkts)
			printf("\nTransformErrorPkts counter expired");

		if (counter_bmap.OutPktsCtrl)
			printf("\nOutPktsCtrl counter expired");

		if (counter_bmap.OutPktsUnknownSA)
			printf("\nOutPktsUnknownSA counter expired");

		if (counter_bmap.OutPktsUntagged)
			printf("\nOutPktsUntagged counter expired");

		if (counter_bmap.OutOverSizePkts)
			printf("\nOutOverSizePkts counter expired");
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_ing_sa_stats", strlen("gpy2xx_msec_get_ing_sa_stats")) == 0) {
		struct ing_sa_stats sa_ctrs;
		memset(&sa_ctrs, 0, sizeof(struct ing_sa_stats));

		cnt += scanParamArg(argc, argv, "rule_index", sizeof(sa_ctrs.rule_index), &sa_ctrs.rule_index);

		ret = gpy2xx_msec_get_ing_sa_stats(&phy, &sa_ctrs);

		if (ret < 0) {
			printf("\nERROR: SA counters get failed.\n");
			return -1;
		}

		printf("\nSA Counters:\n----------------\n");
		printf("\t%40s:\t0x%llx\n", "InOctetsDecrypted", sa_ctrs.InOctetsDecrypted);
		printf("\t%40s:\t0x%llx\n", "InOctetsDecrypted2", sa_ctrs.InOctetsDecrypted2);
		printf("\t%40s:\t0x%llx\n", "InPktsUnchecked", sa_ctrs.InPktsUnchecked);
		printf("\t%40s:\t0x%llx\n", "InPktsDelayed", sa_ctrs.InPktsDelayed);
		printf("\t%40s:\t0x%llx\n", "InPktsLate", sa_ctrs.InPktsLate);
		printf("\t%40s:\t0x%llx\n", "InPktsOK", sa_ctrs.InPktsOK);
		printf("\t%40s:\t0x%llx\n", "InPktsInvalid", sa_ctrs.InPktsInvalid);
		printf("\t%40s:\t0x%llx\n", "InPktsNotValid", sa_ctrs.InPktsNotValid);
		printf("\t%40s:\t0x%llx\n", "InPktsNotUsingSA", sa_ctrs.InPktsNotUsingSA);
		printf("\t%40s:\t0x%llx\n", "InPktsUnusedSA", sa_ctrs.InPktsUnusedSA);
		printf("\t%40s:\t0x%llx\n", "InPktsUntaggedHit", sa_ctrs.InPktsUntaggedHit);
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_egr_sa_stats", strlen("gpy2xx_msec_get_egr_sa_stats")) == 0) {
		struct egr_sa_stats sa_ctrs;
		memset(&sa_ctrs, 0, sizeof(struct egr_sa_stats));

		cnt += scanParamArg(argc, argv, "rule_index", sizeof(sa_ctrs.rule_index), &sa_ctrs.rule_index);

		ret = gpy2xx_msec_get_egr_sa_stats(&phy, &sa_ctrs);

		if (ret < 0) {
			printf("\nERROR: SA counters get failed.\n");
			return -1;
		}

		printf("\nSA Counters:\n----------------\n");
		printf("\t%40s:\t0x%llx\n", "OutOctetsEncrypted", sa_ctrs.OutOctetsEncrypted);
		printf("\t%40s:\t0x%llx\n", "OutOctetsEncrypted2", sa_ctrs.OutOctetsEncrypted2);
		printf("\t%40s:\t0x%llx\n", "OutPktsEncrypted", sa_ctrs.OutPktsEncrypted);
		printf("\t%40s:\t0x%llx\n", "OutPktsTooLong", sa_ctrs.OutPktsTooLong);
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_ing_global_stats", strlen("gpy2xx_msec_get_ing_global_stats")) == 0) {
		struct ing_global_stats global_ctrs;
		memset(&global_ctrs, 0, sizeof(struct ing_global_stats));

		ret = gpy2xx_msec_get_ing_global_stats(&phy, &global_ctrs);

		if (ret < 0) {
			printf("\nERROR: GLOBAL counters get failed.\n");
			return -1;
		}

		printf("\nGLOBAL Counters:\n----------------\n");
		printf("\t%40s:\t0x%llx\n", "TransformErrorPkts", global_ctrs.TransformErrorPkts);
		printf("\t%40s:\t0x%llx\n", "InPktsCtrl", global_ctrs.InPktsCtrl);
		printf("\t%40s:\t0x%llx\n", "InPktsNoTag", global_ctrs.InPktsNoTag);
		printf("\t%40s:\t0x%llx\n", "InPktsUntagged", global_ctrs.InPktsUntagged);
		printf("\t%40s:\t0x%llx\n", "InPktsTagged", global_ctrs.InPktsTagged);
		printf("\t%40s:\t0x%llx\n", "InPktsBadTag", global_ctrs.InPktsBadTag);
		printf("\t%40s:\t0x%llx\n", "InPktsUntaggedMiss", global_ctrs.InPktsUntaggedMiss);
		printf("\t%40s:\t0x%llx\n", "InPktsNoSCI", global_ctrs.InPktsNoSCI);
		printf("\t%40s:\t0x%llx\n", "InPktsUnknownSCI", global_ctrs.InPktsUnknownSCI);
		printf("\t%40s:\t0x%llx\n", "InConsistCheckControlledNotPass", global_ctrs.InConsistCheckControlledNotPass);
		printf("\t%40s:\t0x%llx\n", "InConsistCheckUncontrolledNotPass", global_ctrs.InConsistCheckUncontrolledNotPass);
		printf("\t%40s:\t0x%llx\n", "InConsistCheckControlledPass", global_ctrs.InConsistCheckControlledPass);
		printf("\t%40s:\t0x%llx\n", "InConsistCheckUncontrolledPass", global_ctrs.InConsistCheckUncontrolledPass);
		printf("\t%40s:\t0x%llx\n", "InOverSizePkts", global_ctrs.InOverSizePkts);
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_egr_global_stats", strlen("gpy2xx_msec_get_egr_global_stats")) == 0) {
		struct egr_global_stats global_ctrs;
		memset(&global_ctrs, 0, sizeof(struct egr_global_stats));

		ret = gpy2xx_msec_get_egr_global_stats(&phy, &global_ctrs);

		if (ret < 0) {
			printf("\nERROR: GLOBAL counters get failed.\n");
			return -1;
		}

		printf("\nGLOBAL Counters:\n----------------\n");
		printf("\t%40s:\t0x%llx\n", "TransformErrorPkts", global_ctrs.TransformErrorPkts);
		printf("\t%40s:\t0x%llx\n", "OutPktsCtrl", global_ctrs.OutPktsCtrl);
		printf("\t%40s:\t0x%llx\n", "OutPktsUnknownSA", global_ctrs.OutPktsUnknownSA);
		printf("\t%40s:\t0x%llx\n", "OutPktsUntagged", global_ctrs.OutPktsUntagged);
		printf("\t%40s:\t0x%llx\n", "OutOverSizePkts", global_ctrs.OutOverSizePkts);
#if defined(EN_MSEC_DEBUG_ACCESS) && EN_MSEC_DEBUG_ACCESS
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_ing_debug_stats", strlen("gpy2xx_msec_get_ing_debug_stats")) == 0) {
		struct stats_debug_regs dbg_ctrs;
		memset(&dbg_ctrs, 0, sizeof(struct stats_debug_regs));

		ret = gpy2xx_msec_get_ing_debug_stats(&phy, &dbg_ctrs);

		if (ret < 0) {
			printf("\nERROR: Debug counters get failed.\n");
			return -1;
		}

		printf("\nDebug Counters:\n----------------\n");
		printf("\t%40s:\t0x%x\n", "SA counters that were incremented", dbg_ctrs.cdb1.sa_inc_mask);
		printf("\t%40s:\t0x%x\n", "SA octet counters that were incremented", dbg_ctrs.cdb1.sa_octet_inc);
		printf("\t%40s:\t0x%x\n", "Raw CDB1", dbg_ctrs.cdb1.raw_cdb1);

		printf("\t%40s:\t0x%x\n", "VLAN counters that were incremented", dbg_ctrs.cdb2.vlan_inc_mask);
		printf("\t%40s:\t0x%x\n", "VLAN octet counters that were incremented", dbg_ctrs.cdb2.vlan_octet_inc);
		printf("\t%40s:\t0x%x\n", "Raw CDB2", dbg_ctrs.cdb2.raw_cdb2);

		printf("\t%40s:\t0x%x\n", "Global counters that were incremented", dbg_ctrs.global_inc_mask);

		printf("\t%40s:\t0x%x\n", "Global octet counters that were incremented", dbg_ctrs.cdb4.global_octet_inc);
		printf("\t%40s:\t0x%x\n", "COUNT_DEBUG1 written?", dbg_ctrs.cdb4.sa_test);
		printf("\t%40s:\t0x%x\n", "COUNT_DEBUG2 written?", dbg_ctrs.cdb4.vlan_test);
		printf("\t%40s:\t0x%x\n", "COUNT_DEBUG3 written?", dbg_ctrs.cdb4.global_test);
		printf("\t%40s:\t0x%x\n", "SA num for which counters were incremented", dbg_ctrs.cdb4.sa_nr);
		printf("\t%40s:\t0x%x\n", "VLAN UP for which counters were incremented", dbg_ctrs.cdb4.vlan_up);
		printf("\t%40s:\t0x%x\n", "Raw CDB4", dbg_ctrs.cdb4.raw_cdb4);
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_egr_debug_stats", strlen("gpy2xx_msec_get_egr_debug_stats")) == 0) {
		struct stats_debug_regs dbg_ctrs;
		memset(&dbg_ctrs, 0, sizeof(struct stats_debug_regs));

		ret = gpy2xx_msec_get_egr_debug_stats(&phy, &dbg_ctrs);

		if (ret < 0) {
			printf("\nERROR: Debug counters get failed.\n");
			return -1;
		}

		printf("\nDebug Counters:\n----------------\n");
		printf("\t%40s:\t0x%x\n", "SA counters that were incremented", dbg_ctrs.cdb1.sa_inc_mask);
		printf("\t%40s:\t0x%x\n", "SA octet counters that were incremented", dbg_ctrs.cdb1.sa_octet_inc);
		printf("\t%40s:\t0x%x\n", "Raw CDB1", dbg_ctrs.cdb1.raw_cdb1);

		printf("\t%40s:\t0x%x\n", "VLAN counters that were incremented", dbg_ctrs.cdb2.vlan_inc_mask);
		printf("\t%40s:\t0x%x\n", "VLAN octet counters that were incremented", dbg_ctrs.cdb2.vlan_octet_inc);
		printf("\t%40s:\t0x%x\n", "Raw CDB2", dbg_ctrs.cdb2.raw_cdb2);

		printf("\t%40s:\t0x%x\n", "Global counters that were incremented", dbg_ctrs.global_inc_mask);

		printf("\t%40s:\t0x%x\n", "Global octet counters that were incremented", dbg_ctrs.cdb4.global_octet_inc);
		printf("\t%40s:\t0x%x\n", "COUNT_DEBUG1 written?", dbg_ctrs.cdb4.sa_test);
		printf("\t%40s:\t0x%x\n", "COUNT_DEBUG2 written?", dbg_ctrs.cdb4.vlan_test);
		printf("\t%40s:\t0x%x\n", "COUNT_DEBUG3 written?", dbg_ctrs.cdb4.global_test);
		printf("\t%40s:\t0x%x\n", "SA num for which counters were incremented", dbg_ctrs.cdb4.sa_nr);
		printf("\t%40s:\t0x%x\n", "VLAN UP for which counters were incremented", dbg_ctrs.cdb4.vlan_up);
		printf("\t%40s:\t0x%x\n", "Raw CDB4", dbg_ctrs.cdb4.raw_cdb4);
#endif
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_count_ctrl", strlen("gpy2xx_msec_config_ing_count_ctrl")) == 0) {
		struct count_control cnt_ctrl;
		memset(&cnt_ctrl, 0, sizeof(struct count_control));

		cnt += scanParamArg(argc, argv, "rst_all", sizeof(cnt_ctrl.reset_all), &cnt_ctrl.reset_all);
		cnt += scanParamArg(argc, argv, "ctr_saturate", sizeof(cnt_ctrl.saturate_cntrs), &cnt_ctrl.saturate_cntrs);
		cnt += scanParamArg(argc, argv, "ctr_conr", sizeof(cnt_ctrl.auto_cntr_reset), &cnt_ctrl.auto_cntr_reset);
		cnt += scanParamArg(argc, argv, "rst_summary", sizeof(cnt_ctrl.reset_summary), &cnt_ctrl.reset_summary);

		ret = gpy2xx_msec_config_ing_count_ctrl(&phy, &cnt_ctrl);

		if (ret < 0) {
			printf("\nERROR: Count Control flags set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_egr_count_ctrl", strlen("gpy2xx_msec_config_egr_count_ctrl")) == 0) {
		struct count_control cnt_ctrl;
		memset(&cnt_ctrl, 0, sizeof(struct count_control));

		cnt += scanParamArg(argc, argv, "rst_all", sizeof(cnt_ctrl.reset_all), &cnt_ctrl.reset_all);
		cnt += scanParamArg(argc, argv, "ctr_saturate", sizeof(cnt_ctrl.saturate_cntrs), &cnt_ctrl.saturate_cntrs);
		cnt += scanParamArg(argc, argv, "ctr_conr", sizeof(cnt_ctrl.auto_cntr_reset), &cnt_ctrl.auto_cntr_reset);
		cnt += scanParamArg(argc, argv, "rst_summary", sizeof(cnt_ctrl.reset_summary), &cnt_ctrl.reset_summary);

		ret = gpy2xx_msec_config_egr_count_ctrl(&phy, &cnt_ctrl);

		if (ret < 0) {
			printf("\nERROR: Count Control flags set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_count_incen", strlen("gpy2xx_msec_config_ing_count_incen")) == 0) {
		struct count_incen cnt_inx;
		memset(&cnt_inx, 0, sizeof(struct count_incen));

		cnt += scanParamArg(argc, argv, "sa_inc", sizeof(cnt_inx.sa_inc_en), &cnt_inx.sa_inc_en);
		cnt += scanParamArg(argc, argv, "vlan_inc", sizeof(cnt_inx.vlan_inc_en), &cnt_inx.vlan_inc_en);
		cnt += scanParamArg(argc, argv, "global_inc", sizeof(cnt_inx.global_inc_en), &cnt_inx.global_inc_en);

		ret = gpy2xx_msec_config_ing_count_incen(&phy, &cnt_inx);

		if (ret < 0) {
			printf("\nERROR: Count Incx flags set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_egr_count_incen", strlen("gpy2xx_msec_config_egr_count_incen")) == 0) {
		struct count_incen cnt_inx;
		memset(&cnt_inx, 0, sizeof(struct count_incen));

		cnt += scanParamArg(argc, argv, "sa_inc", sizeof(cnt_inx.sa_inc_en), &cnt_inx.sa_inc_en);
		cnt += scanParamArg(argc, argv, "vlan_inc", sizeof(cnt_inx.vlan_inc_en), &cnt_inx.vlan_inc_en);
		cnt += scanParamArg(argc, argv, "global_inc", sizeof(cnt_inx.global_inc_en), &cnt_inx.global_inc_en);

		ret = gpy2xx_msec_config_egr_count_incen(&phy, &cnt_inx);

		if (ret < 0) {
			printf("\nERROR: Count Incx flags set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_count_secfail", strlen("gpy2xx_msec_config_ing_count_secfail")) == 0) {
		struct count_secfail cnt_sfail;
		memset(&cnt_sfail, 0, sizeof(struct count_secfail));

		cnt += scanParamArg(argc, argv, "misc_secfail", sizeof(cnt_sfail.secfail_1.raw_secfail1), &cnt_sfail.secfail_1.raw_secfail1);
		cnt += scanParamArg(argc, argv, "global_secfail", sizeof(cnt_sfail.global_secfaill_mask), &cnt_sfail.global_secfaill_mask);

		ret = gpy2xx_msec_config_ing_count_secfail(&phy, &cnt_sfail);

		if (ret < 0) {
			printf("\nERROR: Count secfail event flags set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_egr_count_secfail", strlen("gpy2xx_msec_config_egr_count_secfail")) == 0) {
		struct count_secfail cnt_sfail;
		memset(&cnt_sfail, 0, sizeof(struct count_secfail));

		cnt += scanParamArg(argc, argv, "misc_secfail", sizeof(cnt_sfail.secfail_1.raw_secfail1), &cnt_sfail.secfail_1.raw_secfail1);
		cnt += scanParamArg(argc, argv, "global_secfail", sizeof(cnt_sfail.global_secfaill_mask), &cnt_sfail.global_secfaill_mask);

		ret = gpy2xx_msec_config_egr_count_secfail(&phy, &cnt_sfail);

		if (ret < 0) {
			printf("\nERROR: Count secfail event flags set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_count_thresh", strlen("gpy2xx_msec_config_ing_count_thresh")) == 0) {
		struct frame_octet_thr cnt_thr;
		memset(&cnt_thr, 0, sizeof(struct frame_octet_thr));

		cnt += scanParamArg(argc, argv, "frame_thr", 64, &cnt_thr.frame_threshold);
		cnt += scanParamArg(argc, argv, "octet_thr", 64, &cnt_thr.octet_threshold);

		ret = gpy2xx_msec_config_ing_count_thresh(&phy, &cnt_thr);

		if (ret < 0) {
			printf("\nERROR: Counters threshold summary set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_egr_count_thresh", strlen("gpy2xx_msec_config_egr_count_thresh")) == 0) {
		struct frame_octet_thr cnt_thr;
		memset(&cnt_thr, 0, sizeof(struct frame_octet_thr));

		cnt += scanParamArg(argc, argv, "frame_thr", 64, &cnt_thr.frame_threshold);
		cnt += scanParamArg(argc, argv, "octet_thr", 64, &cnt_thr.octet_threshold);

		ret = gpy2xx_msec_config_egr_count_thresh(&phy, &cnt_thr);

		if (ret < 0) {
			printf("\nERROR: Counters threshold summary set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_ing_count_thresh", strlen("gpy2xx_msec_get_ing_count_thresh")) == 0) {
		struct frame_octet_thr cnt_thr;
		memset(&cnt_thr, 0, sizeof(struct frame_octet_thr));

		ret = gpy2xx_msec_get_ing_count_thresh(&phy, &cnt_thr);

		if (ret < 0) {
			printf("\nERROR: Counters threshold summary get failed.\n");
			return -1;
		}

		printf("\nFrame threshold summary = %llx", cnt_thr.frame_threshold);
		printf("\nOctet threshold summary = %llx", cnt_thr.octet_threshold);
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_egr_count_thresh", strlen("gpy2xx_msec_get_egr_count_thresh")) == 0) {
		struct frame_octet_thr cnt_thr;
		memset(&cnt_thr, 0, sizeof(struct frame_octet_thr));

		ret = gpy2xx_msec_get_egr_count_thresh(&phy, &cnt_thr);

		if (ret < 0) {
			printf("\nERROR: Counters threshold summary get failed.\n");
			return -1;
		}

		printf("\nFrame threshold summary = %llx", cnt_thr.frame_threshold);
		printf("\nOctet threshold summary = %llx", cnt_thr.octet_threshold);
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_misc_ctrl", strlen("gpy2xx_msec_config_ing_misc_ctrl")) == 0) {
		struct misc_control mics_ctrl;
		memset(&mics_ctrl, 0, sizeof(struct misc_control));

		cnt += scanParamArg(argc, argv, "mc_latency", sizeof(mics_ctrl.mc_latency_fix), &mics_ctrl.mc_latency_fix);
		cnt += scanParamArg(argc, argv, "static_bp", sizeof(mics_ctrl.static_bypass), &mics_ctrl.static_bypass);
		cnt += scanParamArg(argc, argv, "nm_macsec", sizeof(mics_ctrl.nm_macsec_en), &mics_ctrl.nm_macsec_en);
		cnt += scanParamArg(argc, argv, "validate_level", sizeof(mics_ctrl.validate_frames), &mics_ctrl.validate_frames);
		cnt += scanParamArg(argc, argv, "sectag_avlan", sizeof(mics_ctrl.sectag_after_vlan), &mics_ctrl.sectag_after_vlan);

		ret = gpy2xx_msec_config_ing_misc_ctrl(&phy, &mics_ctrl);

		if (ret < 0) {
			printf("\nERROR: Misc ctrl param set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_egr_misc_ctrl", strlen("gpy2xx_msec_config_egr_misc_ctrl")) == 0) {
		struct misc_control mics_ctrl;
		memset(&mics_ctrl, 0, sizeof(struct misc_control));

		cnt += scanParamArg(argc, argv, "mc_latency", sizeof(mics_ctrl.mc_latency_fix), &mics_ctrl.mc_latency_fix);
		cnt += scanParamArg(argc, argv, "static_bp", sizeof(mics_ctrl.static_bypass), &mics_ctrl.static_bypass);
		cnt += scanParamArg(argc, argv, "nm_macsec", sizeof(mics_ctrl.nm_macsec_en), &mics_ctrl.nm_macsec_en);
		cnt += scanParamArg(argc, argv, "validate_level", sizeof(mics_ctrl.validate_frames), &mics_ctrl.validate_frames);
		cnt += scanParamArg(argc, argv, "sectag_avlan", sizeof(mics_ctrl.sectag_after_vlan), &mics_ctrl.sectag_after_vlan);

		ret = gpy2xx_msec_config_egr_misc_ctrl(&phy, &mics_ctrl);

		if (ret < 0) {
			printf("\nERROR: Misc ctrl param set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_sa_nm_ctrl", strlen("gpy2xx_msec_config_ing_sa_nm_ctrl")) == 0) {
		u16 in_data;
		struct sa_nm_params nm_ctrl;
		memset(&nm_ctrl, 0, sizeof(struct sa_nm_params));

		cnt += scanParamArg(argc, argv, "comp_etype", sizeof(in_data), &in_data);
		nm_ctrl.comp_etype = in_data;
		cnt += scanParamArg(argc, argv, "check_ver", sizeof(in_data), &in_data);
		nm_ctrl.check_v = in_data;
		cnt += scanParamArg(argc, argv, "check_kay", sizeof(in_data), &in_data);
		nm_ctrl.check_kay = in_data;
		cnt += scanParamArg(argc, argv, "check_ce", sizeof(in_data), &in_data);
		nm_ctrl.check_c_e = in_data;
		cnt += scanParamArg(argc, argv, "check_sc", sizeof(in_data), &in_data);
		nm_ctrl.check_sc = in_data;
		cnt += scanParamArg(argc, argv, "check_sl", sizeof(in_data), &in_data);
		nm_ctrl.check_sl = in_data;
		cnt += scanParamArg(argc, argv, "check_pn", sizeof(in_data), &in_data);
		nm_ctrl.check_pn = in_data;
		cnt += scanParamArg(argc, argv, "msec_eth", sizeof(in_data), &in_data);
		nm_ctrl.msec_tag_value = in_data;
		//printf("Utility raw %x\n", nm_ctrl.raw_sanm_ctrl);

		ret = gpy2xx_msec_config_ing_sa_nm_ctrl(&phy, &nm_ctrl);

		if (ret < 0) {
			printf("\nERROR: SA Non-Match ctrl param set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_egr_sa_nm_ctrl", strlen("gpy2xx_msec_config_egr_sa_nm_ctrl")) == 0) {
		u16 in_data;
		struct sa_nm_params nm_ctrl;
		memset(&nm_ctrl, 0, sizeof(struct sa_nm_params));

		cnt += scanParamArg(argc, argv, "comp_etype", sizeof(in_data), &in_data);
		nm_ctrl.comp_etype = in_data;
		cnt += scanParamArg(argc, argv, "check_ver", sizeof(in_data), &in_data);
		nm_ctrl.check_v = in_data;
		cnt += scanParamArg(argc, argv, "check_kay", sizeof(in_data), &in_data);
		nm_ctrl.check_kay = in_data;
		cnt += scanParamArg(argc, argv, "check_ce", sizeof(in_data), &in_data);
		nm_ctrl.check_c_e = in_data;
		cnt += scanParamArg(argc, argv, "check_sc", sizeof(in_data), &in_data);
		nm_ctrl.check_sc = in_data;
		cnt += scanParamArg(argc, argv, "check_sl", sizeof(in_data), &in_data);
		nm_ctrl.check_sl = in_data;
		cnt += scanParamArg(argc, argv, "check_pn", sizeof(in_data), &in_data);
		nm_ctrl.check_pn = in_data;
		cnt += scanParamArg(argc, argv, "msec_eth", sizeof(in_data), &in_data);
		nm_ctrl.msec_tag_value = in_data;
		//printf("Utility raw %x\n", nm_ctrl.raw_sanm_ctrl);

		ret = gpy2xx_msec_config_egr_sa_nm_ctrl(&phy, &nm_ctrl);

		if (ret < 0) {
			printf("\nERROR: SA Non-Match ctrl param set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_sa_nm_ncp", strlen("gpy2xx_msec_config_ing_sa_nm_ncp")) == 0) {
		struct sa_nonmatch_fca nm_fca;
		memset(&nm_fca, 0, sizeof(struct sa_nonmatch_fca));

		cnt += scanParamArg(argc, argv, "pkt_type", sizeof(nm_fca.pkt_type), &nm_fca.pkt_type);
		cnt += scanParamArg(argc, argv, "flow_type", sizeof(nm_fca.flow_type), &nm_fca.flow_type);
		cnt += scanParamArg(argc, argv, "dest_port", sizeof(nm_fca.dest_port), &nm_fca.dest_port);
		cnt += scanParamArg(argc, argv, "drop_nonres", sizeof(nm_fca.drop_non_reserved), &nm_fca.drop_non_reserved);
		cnt += scanParamArg(argc, argv, "drop_action", sizeof(nm_fca.drop_action), &nm_fca.drop_action);

		ret = gpy2xx_msec_config_ing_sa_nm_ncp(&phy, &nm_fca);

		if (ret < 0) {
			printf("\nERROR: SA Non-Match NCP pkt action set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_egr_sa_nm_ncp", strlen("gpy2xx_msec_config_egr_sa_nm_ncp")) == 0) {
		struct sa_nonmatch_fca nm_fca;
		memset(&nm_fca, 0, sizeof(struct sa_nonmatch_fca));

		cnt += scanParamArg(argc, argv, "pkt_type", sizeof(nm_fca.pkt_type), &nm_fca.pkt_type);
		cnt += scanParamArg(argc, argv, "flow_type", sizeof(nm_fca.flow_type), &nm_fca.flow_type);
		cnt += scanParamArg(argc, argv, "dest_port", sizeof(nm_fca.dest_port), &nm_fca.dest_port);
		//cnt += scanParamArg(argc, argv, "drop_nonres", sizeof(nm_fca.drop_non_reserved), &nm_fca.drop_non_reserved);
		cnt += scanParamArg(argc, argv, "drop_action", sizeof(nm_fca.drop_action), &nm_fca.drop_action);

		ret = gpy2xx_msec_config_egr_sa_nm_ncp(&phy, &nm_fca);

		if (ret < 0) {
			printf("\nERROR: SA Non-Match NCP pkt action set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_sa_nm_cp", strlen("gpy2xx_msec_config_ing_sa_nm_cp")) == 0) {
		struct sa_nonmatch_fca nm_fca;
		memset(&nm_fca, 0, sizeof(struct sa_nonmatch_fca));

		cnt += scanParamArg(argc, argv, "pkt_type", sizeof(nm_fca.pkt_type), &nm_fca.pkt_type);
		cnt += scanParamArg(argc, argv, "flow_type", sizeof(nm_fca.flow_type), &nm_fca.flow_type);
		cnt += scanParamArg(argc, argv, "dest_port", sizeof(nm_fca.dest_port), &nm_fca.dest_port);
		cnt += scanParamArg(argc, argv, "drop_nonres", sizeof(nm_fca.drop_non_reserved), &nm_fca.drop_non_reserved);
		cnt += scanParamArg(argc, argv, "drop_action", sizeof(nm_fca.drop_action), &nm_fca.drop_action);

		ret = gpy2xx_msec_config_ing_sa_nm_cp(&phy, &nm_fca);

		if (ret < 0) {
			printf("\nERROR: SA Non-Match CP pkt action set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_egr_sa_nm_cp", strlen("gpy2xx_msec_config_egr_sa_nm_cp")) == 0) {
		struct sa_nonmatch_fca nm_fca;
		memset(&nm_fca, 0, sizeof(struct sa_nonmatch_fca));

		cnt += scanParamArg(argc, argv, "pkt_type", sizeof(nm_fca.pkt_type), &nm_fca.pkt_type);
		cnt += scanParamArg(argc, argv, "flow_type", sizeof(nm_fca.flow_type), &nm_fca.flow_type);
		cnt += scanParamArg(argc, argv, "dest_port", sizeof(nm_fca.dest_port), &nm_fca.dest_port);
		//cnt += scanParamArg(argc, argv, "drop_nonres", sizeof(nm_fca.drop_non_reserved), &nm_fca.drop_non_reserved);
		cnt += scanParamArg(argc, argv, "drop_action", sizeof(nm_fca.drop_action), &nm_fca.drop_action);

		ret = gpy2xx_msec_config_egr_sa_nm_cp(&phy, &nm_fca);

		if (ret < 0) {
			printf("\nERROR: SA Non-Match CP pkt action set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_clear_ing_stats_summ", strlen("gpy2xx_msec_clear_ing_stats_summ")) == 0) {
		struct stats_summary xxx_summ;
		memset(&xxx_summ, 0, sizeof(struct stats_summary));

		cnt += scanParamArg(argc, argv, "psa_summ0", sizeof(xxx_summ.psa_sum_word[0]), &xxx_summ.psa_sum_word[0]);
		cnt += scanParamArg(argc, argv, "glb_summ", sizeof(xxx_summ.glb_summ_word), &xxx_summ.glb_summ_word);

		ret = gpy2xx_msec_clear_ing_stats_summ(&phy, &xxx_summ);

		if (ret < 0) {
			printf("\nERROR: SA stats summary clear failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_clear_egr_stats_summ", strlen("gpy2xx_msec_clear_egr_stats_summ")) == 0) {
		struct stats_summary xxx_summ;
		memset(&xxx_summ, 0, sizeof(struct stats_summary));

		cnt += scanParamArg(argc, argv, "psa_summ0", sizeof(xxx_summ.psa_sum_word[0]), &xxx_summ.psa_sum_word[0]);
		cnt += scanParamArg(argc, argv, "glb_summ", sizeof(xxx_summ.glb_summ_word), &xxx_summ.glb_summ_word);

		ret = gpy2xx_msec_clear_egr_stats_summ(&phy, &xxx_summ);

		if (ret < 0) {
			printf("\nERROR: SA stats summary clear failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_clear_ing_psa_stats_summ", strlen("gpy2xx_msec_clear_ing_psa_stats_summ")) == 0) {
		struct psa_summary psa_summ;
		memset(&psa_summ, 0, sizeof(struct psa_summary));

		cnt += scanParamArg(argc, argv, "psa_type", sizeof(psa_summ.psa_type), &psa_summ.psa_type);
		cnt += scanParamArg(argc, argv, "rule_index", sizeof(psa_summ.rule_index), &psa_summ.rule_index);
		cnt += scanParamArg(argc, argv, "psa_summ0", sizeof(psa_summ.count_summ_psa[0]), &psa_summ.count_summ_psa[0]);

		ret = gpy2xx_msec_clear_ing_psa_stats_summ(&phy, &psa_summ);

		if (ret < 0) {
			printf("\nERROR: Per-SA stats summary set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_ing_psa_stats_summ", strlen("gpy2xx_msec_get_ing_psa_stats_summ")) == 0) {
		int i;
		struct psa_summary psa_summ;
		struct ing_psa_bmap counter_bmap;
		memset(&psa_summ, 0, sizeof(struct psa_summary));

		cnt += scanParamArg(argc, argv, "psa_type", sizeof(psa_summ.psa_type), &psa_summ.psa_type);
		cnt += scanParamArg(argc, argv, "rule_index", sizeof(psa_summ.rule_index), &psa_summ.rule_index);

		ret = gpy2xx_msec_get_ing_psa_stats_summ(&phy, &psa_summ);

		if (ret < 0) {
			printf("\nERROR: Per-SA stats summary get failed.\n");
			return -1;
		}

		printf("\nPer-SA per counter ingress statistics summary flags:\n----------------\n");

		for (i = 0; i < MACSEC_MAX_SA_RULES; i++) {
			printf("\nPer-SA per counter stats summary[%02d] = %8x", i, psa_summ.count_summ_psa[i]);
			counter_bmap.raw_psa_bmap = psa_summ.count_summ_psa[i];

			if (counter_bmap.InOctetsDecryptedValidated)
				printf("\nInOctetsDecrypted / InOctetsValidated counter expired");

			if (counter_bmap.InPktsUncheckedHitDropReserved)
				printf("\nInPktsUnchecked / InPktsHitDropReserved counter expired");

			if (counter_bmap.InPktsDelayed)
				printf("\nInPktsDelayed counter expired");

			if (counter_bmap.InPktsLate)
				printf("\nInPktsLate counter expired");

			if (counter_bmap.InPktsOK)
				printf("\nInPktsOK counter expired");

			if (counter_bmap.InPktsInvalid)
				printf("\nInPktsInvalid counter expired");

			if (counter_bmap.InPktsNotValid)
				printf("\nInPktsNotValid counter expired");

			if (counter_bmap.InPktsNotUsingSA)
				printf("\nInPktsNotUsingSA counter expired");

			if (counter_bmap.InPktsUnusedSA)
				printf("\nInPktsUnusedSA counter expired");

			if (counter_bmap.InPktsUntaggedHit)
				printf("\nInPktsUntaggedHit counter expired");

			if (psa_summ.psa_type == PSA_SUMM_GET_GIVEN)
				break;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_clear_egr_psa_stats_summ", strlen("gpy2xx_msec_clear_egr_psa_stats_summ")) == 0) {
		struct psa_summary psa_summ;
		memset(&psa_summ, 0, sizeof(struct psa_summary));

		cnt += scanParamArg(argc, argv, "psa_type", sizeof(psa_summ.psa_type), &psa_summ.psa_type);
		cnt += scanParamArg(argc, argv, "rule_index", sizeof(psa_summ.rule_index), &psa_summ.rule_index);
		cnt += scanParamArg(argc, argv, "psa_summ0", sizeof(psa_summ.count_summ_psa[0]), &psa_summ.count_summ_psa[0]);

		ret = gpy2xx_msec_clear_egr_psa_stats_summ(&phy, &psa_summ);

		if (ret < 0) {
			printf("\nERROR: Per-SA stats summary set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_egr_psa_stats_summ", strlen("gpy2xx_msec_get_egr_psa_stats_summ")) == 0) {
		int i;
		struct psa_summary psa_summ;
		struct egr_psa_bmap counter_bmap;
		memset(&psa_summ, 0, sizeof(struct psa_summary));

		cnt += scanParamArg(argc, argv, "psa_type", sizeof(psa_summ.psa_type), &psa_summ.psa_type);
		cnt += scanParamArg(argc, argv, "rule_index", sizeof(psa_summ.rule_index), &psa_summ.rule_index);

		ret = gpy2xx_msec_get_egr_psa_stats_summ(&phy, &psa_summ);

		if (ret < 0) {
			printf("\nERROR: Per-SA stats summary get failed.\n");
			return -1;
		}

		printf("\nPer-SA per counter egress statistics summary flags:\n----------------\n");

		for (i = 0; i < MACSEC_MAX_SA_RULES; i++) {
			printf("\nPer-SA per counter stats summary[%02d] = %8x", i, psa_summ.count_summ_psa[i]);
			counter_bmap.raw_psa_bmap = psa_summ.count_summ_psa[i];

			if (counter_bmap.OutOctetsEncryptedProtected)
				printf("\nOutOctetsEncrypted / OutOctetsProtected counter expired");

			if (counter_bmap.OutPktsEncryptedProtectedHitDropReserved)
				printf("\nOutPktsEncrypted / OutPktsProtected / OutPktsHitDropReserved counter expired");

			if (counter_bmap.OutPktsTooLong)
				printf("\nOutPktsTooLong counter expired");

			if (psa_summ.psa_type == PSA_SUMM_GET_GIVEN)
				break;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_cp_rule", strlen("gpy2xx_msec_config_ing_cp_rule")) == 0) {
		struct cp_class_param cpc_rule;
		memset(&cpc_rule, 0, sizeof(struct cp_class_param));

		cnt += scanParamArg(argc, argv, "cpc_index", sizeof(cpc_rule.cpc_index), &cpc_rule.cpc_index);
		cnt += scanParamArg(argc, argv, "entry_type", sizeof(cpc_rule.cpc_type), &cpc_rule.cpc_type);
		cnt += scanMAC_Arg(argc, argv, "da_mac", cpc_rule.mac_da);
		//cnt += scanMAC_Arg(argc, argv, "da_start", cpc_rule.mac_da_start);
		cnt += scanMAC_Arg(argc, argv, "da_end", cpc_rule.mac_da_end);
		//cnt += scanMAC_Arg(argc, argv, "da_const44", cpc_rule.mac_da_const44);
		//cnt += scanMAC_Arg(argc, argv, "da_const48", cpc_rule.mac_da_const48);
		cnt += scanParamArg(argc, argv, "eth_type", sizeof(cpc_rule.ether_type), &cpc_rule.ether_type);
		cnt += scanParamArg(argc, argv, "cpm_mode", sizeof(cpc_rule.cpm_mode), &cpc_rule.cpm_mode);
		cnt += scanParamArg(argc, argv, "cpm_en", sizeof(cpc_rule.cpm_enable), &cpc_rule.cpm_enable);

		ret = gpy2xx_msec_config_ing_cp_rule(&phy, &cpc_rule);

		if (ret < 0) {
			printf("\nERROR: CPC rule set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_egr_cp_rule", strlen("gpy2xx_msec_config_egr_cp_rule")) == 0) {
		struct cp_class_param cpc_rule;
		memset(&cpc_rule, 0, sizeof(struct cp_class_param));

		cnt += scanParamArg(argc, argv, "cpc_index", sizeof(cpc_rule.cpc_index), &cpc_rule.cpc_index);
		cnt += scanParamArg(argc, argv, "entry_type", sizeof(cpc_rule.cpc_type), &cpc_rule.cpc_type);
		cnt += scanMAC_Arg(argc, argv, "da_mac", cpc_rule.mac_da);
		//cnt += scanMAC_Arg(argc, argv, "da_start", cpc_rule.mac_da_start);
		cnt += scanMAC_Arg(argc, argv, "da_end", cpc_rule.mac_da_end);
		//cnt += scanMAC_Arg(argc, argv, "da_const44", cpc_rule.mac_da_const44);
		//cnt += scanMAC_Arg(argc, argv, "da_const48", cpc_rule.mac_da_const48);
		cnt += scanParamArg(argc, argv, "eth_type", sizeof(cpc_rule.ether_type), &cpc_rule.ether_type);
		cnt += scanParamArg(argc, argv, "cpm_mode", sizeof(cpc_rule.cpm_mode), &cpc_rule.cpm_mode);
		cnt += scanParamArg(argc, argv, "cpm_en", sizeof(cpc_rule.cpm_enable), &cpc_rule.cpm_enable);

		ret = gpy2xx_msec_config_egr_cp_rule(&phy, &cpc_rule);

		if (ret < 0) {
			printf("\nERROR: CPC rule set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_clear_sa_pn_thr_summ", strlen("gpy2xx_msec_clear_sa_pn_thr_summ")) == 0) {
		struct sa_pn_thr_summ xxx_summ;
		memset(&xxx_summ, 0, sizeof(struct sa_pn_thr_summ));

		//cnt += scanParamArg(argc, argv, "word_num", sizeof(raw_ee.word_num), &raw_ee.word_num);
		cnt += scanParamArg(argc, argv, "pnthr_summ0", sizeof(xxx_summ.pnthr_sum_word[0]), &xxx_summ.pnthr_sum_word[0]);

		ret = gpy2xx_msec_clear_sa_pn_thr_summ(&phy, &xxx_summ);

		if (ret < 0) {
			printf("\nERROR: SA PN Threshold summary clear failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_sa_pn_thr_summ", strlen("gpy2xx_msec_get_sa_pn_thr_summ")) == 0) {
		int i;
		struct sa_pn_thr_summ xxx_summ;
		memset(&xxx_summ, 0, sizeof(struct sa_pn_thr_summ));

		//cnt += scanParamArg(argc, argv, "word_num", sizeof(raw_ee.word_num), &raw_ee.word_num);
		ret = gpy2xx_msec_get_sa_pn_thr_summ(&phy, &xxx_summ);

		if (ret < 0) {
			printf("\nERROR: SA PN Threshold summary get failed.\n");
			return -1;
		}

		printf("\nSA PN Threshold summary flags:\n----------------\n");

		for (i = 0; i < (MACSEC_MAX_SA_RULES / 32); i++) {
			printf("\nPN threshold summary[%02d] = %8x", i, xxx_summ.pnthr_sum_word[i]);
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_clear_egr_sa_exp_summ", strlen("gpy2xx_msec_clear_egr_sa_exp_summ")) == 0) {
		struct sa_exp_summ xxx_summ;
		memset(&xxx_summ, 0, sizeof(struct sa_exp_summ));

		//cnt += scanParamArg(argc, argv, "word_num", sizeof(raw_ee.word_num), &raw_ee.word_num);
		cnt += scanParamArg(argc, argv, "saexp_summ0", sizeof(xxx_summ.sa_expsum_word[0]), &xxx_summ.sa_expsum_word[0]);

		ret = gpy2xx_msec_clear_egr_sa_exp_summ(&phy, &xxx_summ);

		if (ret < 0) {
			printf("\nERROR: SA expired summary clear failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_egr_sa_exp_summ", strlen("gpy2xx_msec_get_egr_sa_exp_summ")) == 0) {
		int i;
		struct sa_exp_summ xxx_summ;
		memset(&xxx_summ, 0, sizeof(struct sa_exp_summ));

		//cnt += scanParamArg(argc, argv, "word_num", sizeof(raw_ee.word_num), &raw_ee.word_num);
		ret = gpy2xx_msec_get_egr_sa_exp_summ(&phy, &xxx_summ);

		if (ret < 0) {
			printf("\nERROR: SA expired summary get failed.\n");
			return -1;
		}

		printf("\nSA PN Threshold summary flags:\n----------------\n");

		for (i = 0; i < (MACSEC_MAX_SA_RULES / 32); i++) {
			printf("\nPN threshold summary[%02d] = %8x", i, xxx_summ.sa_expsum_word[i]);
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_clear_ing_cc_int_stat", strlen("gpy2xx_msec_clear_ing_cc_int_stat")) == 0) {
		struct crypto_core_stat cc_stat;
		memset(&cc_stat, 0, sizeof(struct crypto_core_stat));

#if defined(EN_MSEC_DEBUG_ACCESS) && EN_MSEC_DEBUG_ACCESS
		cnt += scanParamArg(argc, argv, "ctx_stat", sizeof(cc_stat.ctx_stat.raw_ctx_stat), &cc_stat.ctx_stat.raw_ctx_stat);
#endif
		cnt += scanParamArg(argc, argv, "int_stat", sizeof(cc_stat.int_stat.raw_int_stat), &cc_stat.int_stat.raw_int_stat);

		ret = gpy2xx_msec_clear_ing_cc_int_stat(&phy, &cc_stat);

		if (ret < 0) {
			printf("\nERROR: Crpypto cores interrupt status clear failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_ing_cc_int_stat", strlen("gpy2xx_msec_get_ing_cc_int_stat")) == 0) {
		struct crypto_core_stat cc_stat;
		memset(&cc_stat, 0, sizeof(struct crypto_core_stat));

		ret = gpy2xx_msec_get_ing_cc_int_stat(&phy, &cc_stat);

		if (ret < 0) {
			printf("\nERROR: Crpypto cores interrupt status get failed.\n");
			return -1;
		}

#if defined(EN_MSEC_DEBUG_ACCESS) && EN_MSEC_DEBUG_ACCESS
		printf("\nCC context status (raw) = %8x", cc_stat.ctx_stat.raw_ctx_stat);
#endif
		printf("\nCC interrup status (raw) = %8x", cc_stat.int_stat.raw_int_stat);

		if (cc_stat.int_stat.input_error)
			printf("\nInput data does not properly receive all data (e2)");

		if (cc_stat.int_stat.processing_error)
			printf("\nAn error occurred while processing the frame or parsing the token. Combination of errors e0, e1, e3 and e4");

		if (cc_stat.int_stat.context_error)
			printf("\nAn invalid context has been fetched. Combination of errors e5 and e6");

		if (cc_stat.int_stat.seq_threshold)
			printf("\nIf a sequence number exceeds the programmed sequence number threshold (refer to 0) due to an outbound sequence number increment this interrupt is triggered (e8).");

		if (cc_stat.int_stat.seq_rollover)
			printf("\nIf a sequence number roll-over (increment from maximum to zero) due to an outbound sequence number increment this interrupt is triggered (e10 egress only)");
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_clear_egr_cc_int_stat", strlen("gpy2xx_msec_clear_egr_cc_int_stat")) == 0) {
		struct crypto_core_stat cc_stat;
		memset(&cc_stat, 0, sizeof(struct crypto_core_stat));
#if defined(EN_MSEC_DEBUG_ACCESS) && EN_MSEC_DEBUG_ACCESS
		cnt += scanParamArg(argc, argv, "ctx_stat", sizeof(cc_stat.ctx_stat.raw_ctx_stat), &cc_stat.ctx_stat.raw_ctx_stat);
#endif
		cnt += scanParamArg(argc, argv, "int_stat", sizeof(cc_stat.int_stat.raw_int_stat), &cc_stat.int_stat.raw_int_stat);

		ret = gpy2xx_msec_clear_egr_cc_int_stat(&phy, &cc_stat);

		if (ret < 0) {
			printf("\nERROR: Crpypto cores interrupt status clear failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_egr_cc_int_stat", strlen("gpy2xx_msec_get_egr_cc_int_stat")) == 0) {
		struct crypto_core_stat cc_stat;
		memset(&cc_stat, 0, sizeof(struct crypto_core_stat));

		ret = gpy2xx_msec_get_egr_cc_int_stat(&phy, &cc_stat);

		if (ret < 0) {
			printf("\nERROR: Crpypto cores interrupt status get failed.\n");
			return -1;
		}

#if defined(EN_MSEC_DEBUG_ACCESS) && EN_MSEC_DEBUG_ACCESS
		printf("\nCC context status (raw) = %8x", cc_stat.ctx_stat.raw_ctx_stat);
#endif
		printf("\nCC interrup status (raw) = %8x", cc_stat.int_stat.raw_int_stat);

		if (cc_stat.int_stat.input_error)
			printf("\nInput data does not properly receive all data (e2)");

		if (cc_stat.int_stat.processing_error)
			printf("\nAn error occurred while processing the frame or parsing the token. Combination of errors e0, e1, e3 and e4");

		if (cc_stat.int_stat.context_error)
			printf("\nAn invalid context has been fetched. Combination of errors e5 and e6");

		if (cc_stat.int_stat.seq_threshold)
			printf("\nIf a sequence number exceeds the programmed sequence number threshold (refer to 0) due to an outbound sequence number increment this interrupt is triggered (e8).");

		if (cc_stat.int_stat.seq_rollover)
			printf("\nIf a sequence number roll-over (increment from maximum to zero) due to an outbound sequence number increment this interrupt is triggered (e10 egress only)");
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_sn_thresh", strlen("gpy2xx_msec_config_ing_sn_thresh")) == 0) {
		struct seq_num_thr sn_thr;
		memset(&sn_thr, 0, sizeof(struct seq_num_thr));

		cnt += scanParamArg(argc, argv, "sn_type", sizeof(sn_thr.sn_type), &sn_thr.sn_type);
		cnt += scanParamArg(argc, argv, "sn_thr", 64, &sn_thr.sn_threshold);

		ret = gpy2xx_msec_config_ing_sn_thresh(&phy, &sn_thr);

		if (ret < 0) {
			printf("\nERROR: SN Threshold set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_ing_sn_thresh", strlen("gpy2xx_msec_get_ing_sn_thresh")) == 0) {
		struct seq_num_thr sn_thr;
		memset(&sn_thr, 0, sizeof(struct seq_num_thr));

		cnt += scanParamArg(argc, argv, "sn_type", sizeof(sn_thr.sn_type), &sn_thr.sn_type);

		ret = gpy2xx_msec_get_ing_sn_thresh(&phy, &sn_thr);

		if (ret < 0) {
			printf("\nERROR: SN Threshold set failed.\n");
			return -1;
		}

		printf("\nSN threshold = %llx", sn_thr.sn_threshold);
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_egr_sn_thresh", strlen("gpy2xx_msec_config_egr_sn_thresh")) == 0) {
		struct seq_num_thr sn_thr;
		memset(&sn_thr, 0, sizeof(struct seq_num_thr));

		cnt += scanParamArg(argc, argv, "sn_type", sizeof(sn_thr.sn_type), &sn_thr.sn_type);
		cnt += scanParamArg(argc, argv, "sn_thr", 64, &sn_thr.sn_threshold);

		ret = gpy2xx_msec_config_egr_sn_thresh(&phy, &sn_thr);

		if (ret < 0) {
			printf("\nERROR: SN Threshold set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_egr_sn_thresh", strlen("gpy2xx_msec_get_egr_sn_thresh")) == 0) {
		struct seq_num_thr sn_thr;
		memset(&sn_thr, 0, sizeof(struct seq_num_thr));

		cnt += scanParamArg(argc, argv, "sn_type", sizeof(sn_thr.sn_type), &sn_thr.sn_type);

		ret = gpy2xx_msec_get_egr_sn_thresh(&phy, &sn_thr);

		if (ret < 0) {
			printf("\nERROR: SN Threshold set failed.\n");
			return -1;
		}

		printf("\nSN threshold = %llx", sn_thr.sn_threshold);
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_ing_aic_csr", strlen("gpy2xx_msec_config_ing_aic_csr")) == 0) {
		struct aic_control_stat aic_csr;
		memset(&aic_csr, 0, sizeof(struct aic_control_stat));

		cnt += scanParamArg(argc, argv, "enable_ctrl", sizeof(aic_csr.enable_ctrl.raw_aic_bits), &aic_csr.enable_ctrl.raw_aic_bits);
		cnt += scanParamArg(argc, argv, "ack", sizeof(aic_csr.ack.raw_aic_bits), &aic_csr.ack.raw_aic_bits);
		cnt += scanParamArg(argc, argv, "enable_set", sizeof(aic_csr.enable_set.raw_aic_bits), &aic_csr.enable_set.raw_aic_bits);
		cnt += scanParamArg(argc, argv, "enable_clr", sizeof(aic_csr.enable_clr.raw_aic_bits), &aic_csr.enable_clr.raw_aic_bits);

		ret = gpy2xx_msec_config_ing_aic_csr(&phy, &aic_csr);

		if (ret < 0) {
			printf("\nERROR:AIC's CSR set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_ing_aic_csr", strlen("gpy2xx_msec_get_ing_aic_csr")) == 0) {
		struct aic_control_stat aic_csr;
		memset(&aic_csr, 0, sizeof(struct aic_control_stat));

		ret = gpy2xx_msec_get_ing_aic_csr(&phy, &aic_csr);

		if (ret < 0) {
			printf("\nERROR:AIC's CSR get failed.\n");
			return -1;
		}

		printf("\n            Interrupt bit assignments in Advanced Interrupt Controller");
		printf("\n------------------------------------------------------------------------------------");
		printf("\n| Bit | Interrupt Name |                      Description                          |");
		printf("\n------------------------------------------------------------------------------------");
		printf("\n| [0] | drop_class     | pkt drop pulse from classification logic                  |");
		printf("\n| [1] | drop_pp        | pkt drop pulse from post-processor logic                  |");
		printf("\n| [2] | drop_mtu       | pkt drop pulse from MTU checking logic                    |");
		printf("\n| [3] | eng_irq        | interrupt pulse from EIP-62 MACsec crypto-engine core     |");
		printf("\n| [4] | stat_sa_thr    | interrupt pulse from statistics module (SA statistics)    |");
		printf("\n| [5] | stat_global_thr| interrupt pulse from statistics module (global statistics)|");
		printf("\n| [6] | stat_vlan_thr  | interrupt pulse from statistics module (VLAN statistics)  |");
		printf("\n| [7] | drop_cc packet | (-i)pkt drop pulse from consistency checking logic        |");
		printf("\n| [8] | sa_pn_thr      | (-e)pulse from post processor                             |");
		printf("\n| [9] | sa_expired     | (-e)pulse from classifier                                 |");
		printf("\n------------------------------------------------------------------------------------");

		printf("\nIndividual 'type (as below)' bits per interrupt input:");
		printf("\n                                        Type Control = %8x (0b = level detect, 1b = edge detect)", aic_csr.type_ctrl.raw_aic_bits);
		printf("\n                                    Polarity Control = %8x (0b = low level or falling edge, 1b = high level or rising edge)", aic_csr.pol_ctrl.raw_aic_bits);
		printf("\n   Raw Interrupt Pending Status (i.e before masking) = %8x (0b = inactive, 1b = pending)", aic_csr.raw_stat.raw_aic_bits);
		printf("\n Masked Interrupt Pending Status (i.e after masking) = %8x (0b = inactive, 1b = pending)", aic_csr.enabled_stat.raw_aic_bits);
		printf("\n                            Interrupt Enable Control = %8x (0b = disabled, 1b = enabled)", aic_csr.enable_ctrl.raw_aic_bits);
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_config_egr_aic_csr", strlen("gpy2xx_msec_config_egr_aic_csr")) == 0) {
		struct aic_control_stat aic_csr;
		memset(&aic_csr, 0, sizeof(struct aic_control_stat));

		cnt += scanParamArg(argc, argv, "enable_ctrl", sizeof(aic_csr.enable_ctrl.raw_aic_bits), &aic_csr.enable_ctrl.raw_aic_bits);
		cnt += scanParamArg(argc, argv, "ack", sizeof(aic_csr.ack.raw_aic_bits), &aic_csr.ack.raw_aic_bits);
		cnt += scanParamArg(argc, argv, "enable_set", sizeof(aic_csr.enable_set.raw_aic_bits), &aic_csr.enable_set.raw_aic_bits);
		cnt += scanParamArg(argc, argv, "enable_clr", sizeof(aic_csr.enable_clr.raw_aic_bits), &aic_csr.enable_clr.raw_aic_bits);

		ret = gpy2xx_msec_config_egr_aic_csr(&phy, &aic_csr);

		if (ret < 0) {
			printf("\nERROR:AIC's CSR set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_egr_aic_csr", strlen("gpy2xx_msec_get_egr_aic_csr")) == 0) {
		struct aic_control_stat aic_csr;
		memset(&aic_csr, 0, sizeof(struct aic_control_stat));

		ret = gpy2xx_msec_get_egr_aic_csr(&phy, &aic_csr);

		if (ret < 0) {
			printf("\nERROR:AIC's CSR get failed.\n");
			return -1;
		}

		printf("\n            Interrupt bit assignments in Advanced Interrupt Controller");
		printf("\n------------------------------------------------------------------------------------");
		printf("\n| Bit | Interrupt Name |                      Description                          |");
		printf("\n------------------------------------------------------------------------------------");
		printf("\n| [0] | drop_class     | pkt drop pulse from classification logic                  |");
		printf("\n| [1] | drop_pp        | pkt drop pulse from post-processor logic                  |");
		printf("\n| [2] | drop_mtu       | pkt drop pulse from MTU checking logic                    |");
		printf("\n| [3] | eng_irq        | interrupt pulse from EIP-62 MACsec crypto-engine core     |");
		printf("\n| [4] | stat_sa_thr    | interrupt pulse from statistics module (SA statistics)    |");
		printf("\n| [5] | stat_global_thr| interrupt pulse from statistics module (global statistics)|");
		printf("\n| [6] | stat_vlan_thr  | interrupt pulse from statistics module (VLAN statistics)  |");
		printf("\n| [7] | drop_cc packet | (-i)pkt drop pulse from consistency checking logic        |");
		printf("\n| [8] | sa_pn_thr      | (-e)pulse from post processor                             |");
		printf("\n| [9] | sa_expired     | (-e)pulse from classifier                                 |");
		printf("\n------------------------------------------------------------------------------------");

		printf("\nIndividual 'type (as below)' bits per interrupt input:");
		printf("\n                                        Type Control = %8x (0b = level detect, 1b = edge detect)", aic_csr.type_ctrl.raw_aic_bits);
		printf("\n                                    Polarity Control = %8x (0b = low level or falling edge, 1b = high level or rising edge)", aic_csr.pol_ctrl.raw_aic_bits);
		printf("\n   Raw Interrupt Pending Status (i.e before masking) = %8x (0b = inactive, 1b = pending)", aic_csr.raw_stat.raw_aic_bits);
		printf("\n Masked Interrupt Pending Status (i.e after masking) = %8x (0b = inactive, 1b = pending)", aic_csr.enabled_stat.raw_aic_bits);
		printf("\n                            Interrupt Enable Control = %8x (0b = disabled, 1b = enabled)", aic_csr.enable_ctrl.raw_aic_bits);
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_ing_cap", strlen("gpy2xx_msec_get_ing_cap")) == 0) {
		struct eip160_hwcap hw_cap;
		memset(&hw_cap, 0, sizeof(struct eip160_hwcap));

		ret = gpy2xx_msec_get_ing_cap(&phy, &hw_cap);

		if (ret < 0) {
			printf("\nERROR:Ing HW cap get failed.\n");
			return -1;
		}

		printf("\nEIP num %x compliment %x, patch-level %x, minor-ver %x, major-ver %x",
		       hw_cap.eip_ver.eip_nr, hw_cap.eip_ver.eip_nr_complement, hw_cap.eip_ver.patch_level, hw_cap.eip_ver.minor_version, hw_cap.eip_ver.major_version);
		printf("\nSA nums %x CCs %x, match-sci %x",
		       hw_cap.eip_cfg.nr_of_sas, hw_cap.eip_cfg.consist_checks, hw_cap.eip_cfg.match_sci);
		//printf("\nRaw CFG2 %x ", hw_cap.eip_cfg2.raw_eip160_cfg2);
		printf("\nCounters:- SA %x, VL %x, GL %x, Saturate %x, Auto reset %x, SA octet %x, VL octet %x, GL octet %x, IG only %x, EG only %x, Tag bypass %x\n",
		       hw_cap.eip_cfg2.sa_counters, hw_cap.eip_cfg2.vl_counters, hw_cap.eip_cfg2.global_counters, hw_cap.eip_cfg2.saturate_cntrs,
		       hw_cap.eip_cfg2.auto_cntr_reset, hw_cap.eip_cfg2.sa_octet_ctrs, hw_cap.eip_cfg2.vl_octet_ctrs, hw_cap.eip_cfg2.glob_octet_ctrs,
		       hw_cap.eip_cfg2.ingress_only, hw_cap.eip_cfg2.egress_only, hw_cap.eip_cfg2.tag_bypass);
		printf("\nAIC's number of interrupt inputs %x ", hw_cap.aic_opt.number_of_inputs);
		printf("\nAIC num %x compliment %x, patch-level %x, minor-ver %x, major-ver %x",
		       hw_cap.aic_ver.eip_nr, hw_cap.aic_ver.eip_nr_complement, hw_cap.aic_ver.patch_level, hw_cap.aic_ver.minor_version, hw_cap.aic_ver.major_version);
	} else if (xstrncasecmp(argv[3], "gpy2xx_msec_get_egr_cap", strlen("gpy2xx_msec_get_egr_cap")) == 0) {
		struct eip160_hwcap hw_cap;
		memset(&hw_cap, 0, sizeof(struct eip160_hwcap));

		ret = gpy2xx_msec_get_egr_cap(&phy, &hw_cap);

		if (ret < 0) {
			printf("\nERROR:Egr HW cap get failed.\n");
			return -1;
		}

		printf("\nEIP num %x compliment %x, patch-level %x, minor-ver %x, major-ver %x",
		       hw_cap.eip_ver.eip_nr, hw_cap.eip_ver.eip_nr_complement, hw_cap.eip_ver.patch_level, hw_cap.eip_ver.minor_version, hw_cap.eip_ver.major_version);
		printf("\nSA nums %x", hw_cap.eip_cfg.nr_of_sas);
		//printf("\nRaw CFG2 %x ", hw_cap.eip_cfg2.raw_eip160_cfg2);
		printf("\nCounters:- SA %x, VL %x, GL %x, Saturate %x, Auto reset %x, SA octet %x, VL octet %x, GL octet %x, IG only %x, EG only %x, Tag bypass %x\n",
		       hw_cap.eip_cfg2.sa_counters, hw_cap.eip_cfg2.vl_counters, hw_cap.eip_cfg2.global_counters, hw_cap.eip_cfg2.saturate_cntrs,
		       hw_cap.eip_cfg2.auto_cntr_reset, hw_cap.eip_cfg2.sa_octet_ctrs, hw_cap.eip_cfg2.vl_octet_ctrs, hw_cap.eip_cfg2.glob_octet_ctrs,
		       hw_cap.eip_cfg2.ingress_only, hw_cap.eip_cfg2.egress_only, hw_cap.eip_cfg2.tag_bypass);
		printf("\nAIC's number of interrupt inputs %x ", hw_cap.aic_opt.number_of_inputs);
		printf("\nAIC num %x compliment %x, patch-level %x, minor-ver %x, major-ver %x",
		       hw_cap.aic_ver.eip_nr, hw_cap.aic_ver.eip_nr_complement, hw_cap.aic_ver.patch_level, hw_cap.aic_ver.minor_version, hw_cap.aic_ver.major_version);
	} else	{
		printf("Command Not Supported\n");
	}

	return ret;
}

int gpy211_main(int argc, char *argv[])
{
	s32 ret = 0;

	if (xstrncasecmp(argv[2], "gpy2xx_uninit", strlen("gpy2xx_uninit")) == 0) {
		ret = gpy2xx_uninit(&phy);

		if (ret < 0)
			printf("\nERROR: GPY211 uinit failed.\n");
	}  else if (xstrncasecmp(argv[2], "gpy2xx_get_phy_id", strlen("gpy2xx_get_phy_id")) == 0) {
		struct gpy211_id phyId = phy.id;

		printf("\nPopulated PHY ID parameters:\n----------------\n");
		printf("\t%40s:\t0x%x\n", "PHY Organizationally Unique Identifier", phyId.OUI);
		printf("\t%40s:\t0x%x\n", "PHY Manufacturer's Model Number", phyId.model_no);
		printf("\t%40s:\t0x%x\n", "PHY Revision Number", phyId.revision);
		printf("\t%40s:\t0x%x\n", "Chip Manufacturer ID", phyId.manufacturer_id);
		printf("\t%40s:\t0x%x\n", "Chip Part Number", phyId.part_no);
		printf("\t%40s:\t0x%x\n", "Chip Version", phyId.version);
		printf("\t%40s:\t0x%x\n", "Firmware Version", ((phyId.fw_release << 15) | (phyId.fw_major << 8) | (phyId.fw_minor)));
		printf("\t%40s:\t0x%x\n", "Firmware Major Version Number", phyId.fw_major);
		printf("\t%40s:\t0x%x\n", "Firmware Minor Version Number", phyId.fw_minor);
		printf("\t%40s:\t0x%x\n", "Firmware Release Indication", phyId.fw_release);
		printf("\t%40s:\t%s\n", "The memory target used for firmware execution", (phyId.fw_memory ? ((phyId.fw_memory == 1) ? "OTP" : "FLASH") : "ROM"));
		printf("\t%40s:\t0x%x\n", "Driver Major Version Number", phyId.drv_major);
		printf("\t%40s:\t0x%x\n", "Driver Minor Version Number", phyId.drv_minor);
		printf("\t%40s:\t0x%x\n", "Driver Release Indication", phyId.drv_release);
		printf("\t%40s:\t0x%x\n", "Driver Patch Indication", phyId.drv_patch);
	}	else if (xstrncasecmp(argv[2], "gpy2xx_read_fw_info", strlen("gpy2xx_read_fw_info")) == 0) {
		ret = gpy2xx_read_fw_info(&phy);

		if (ret < 0)
			printf("\nERROR: Reading FW info failed.\n");

		struct gpy211_id phyId = phy.id;

		printf("\t%40s:\t0x%x\n", "Firmware Version", ((phyId.fw_release << 15) | (phyId.fw_major << 8) | (phyId.fw_minor)));
		printf("\t%40s:\t0x%x\n", "Firmware Major Version Number", phyId.fw_major);
		printf("\t%40s:\t0x%x\n", "Firmware Minor Version Number", phyId.fw_minor);
		printf("\t%40s:\t0x%x\n", "Firmware Release Indication", phyId.fw_release);
		printf("\t%40s:\t%s\n", "The memory target used for firmware execution", (phyId.fw_memory ? ((phyId.fw_memory == 1) ? "OTP" : "FLASH") : "ROM"));
	} else if (xstrncasecmp(argv[2], "gpy2xx_soft_reset", strlen("gpy2xx_soft_reset")) == 0) {
		ret = gpy2xx_soft_reset(&phy);

		if (ret < 0)
			printf("\nERROR: SW reset failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_poll_reset", strlen("gpy2xx_poll_reset")) == 0) {
		ret = gpy2xx_poll_reset(&phy);

		if (ret > 0)
			printf("\nSW reset completed.\n");
		else if (ret == 0)
			printf("\nSW reset is pending.\n");
		else
			printf("\nERROR: SW reset polling failed.\n");

#if defined(EN_SMDIO_RW) && EN_SMDIO_RW
	} else if (xstrncasecmp(argv[2], "gpy2xx_smdio_read", strlen("gpy2xx_smdio_read")) == 0) {
		int cnt = 0;
		u32 reg_off;

		cnt += scanParamArg(argc, argv, "regaddr", sizeof(reg_off), &reg_off);

		ret = gpy2xx_smdio_read(&phy, reg_off);

		if (ret < 0) {
			printf("\nERROR: SMDIO read failed.\n");
		} else {
			printf("\nSmdio rd vale = 0x%x\n", ret);
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_smdio_write", strlen("gpy2xx_smdio_write")) == 0) {
		int cnt = 0;
		u32 reg_off;
		u16 data_val;

		cnt += scanParamArg(argc, argv, "regaddr", sizeof(reg_off), &reg_off);
		cnt += scanParamArg(argc, argv, "data", sizeof(data_val), &data_val);

		ret = gpy2xx_smdio_write(&phy, reg_off, data_val);

		if (ret < 0) {
			printf("\nERROR: SMDIO write failed\n");
		}

#endif
	} else if (xstrncasecmp(argv[2], "gpy2xx_read_mmd", strlen("gpy2xx_read_mmd")) == 0) {
		int cnt = 0;
		int dev_id;
		u32 reg_off;

		cnt += scanParamArg(argc, argv, "devtype", sizeof(dev_id), &dev_id);
		cnt += scanParamArg(argc, argv, "regaddr", sizeof(reg_off), &reg_off);

		ret = gpy2xx_read_mmd(&phy, dev_id, reg_off);

		if (ret < 0) {
			printf("\nERROR: MMD read failed.\n");
		} else {
			printf("\nMmd rd vale = 0x%x\n", ret);
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_write_mmd", strlen("gpy2xx_write_mmd")) == 0) {
		int cnt = 0;
		int dev_id;
		u32 reg_off;
		u16 data_val;

		cnt += scanParamArg(argc, argv, "devtype", sizeof(dev_id), &dev_id);
		cnt += scanParamArg(argc, argv, "regaddr", sizeof(reg_off), &reg_off);
		cnt += scanParamArg(argc, argv, "data", sizeof(data_val), &data_val);

		ret = gpy2xx_write_mmd(&phy, dev_id, reg_off, data_val);

		if (ret < 0) {
			printf("\nERROR: MMD write failed.\n");
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_read_status", strlen("gpy2xx_read_status")) == 0) {
		ret = gpy2xx_read_status(&phy);

		if (ret < 0)
			printf("\nERROR: Read link status failed.\n");
		else {
			struct gpy211_link linkConfig = phy.link;
			char buf[256];

			printf("\nPopulated link configuration parameters:\n----------------\n");
			printf("\t%40s:\t%s\n", "Auto-Negotiation (AN) state", linkConfig.autoneg ? "Enabled" : "Disabled");
			printf("\t%40s:\t%d\n", "Link Speed or Partner Link Speed (AN)", linkConfig.speed);
			printf("\t%40s:\t%s\n", "Duplex (Forced) or Partner Duplex (AN)", linkConfig.duplex ? "Full" : "Half");
			printf("\t%40s:\t%d\n", "Partner Pause (AN)", linkConfig.pause);
			printf("\t%40s:\t%d\n", "Partner Asym-pause (AN)", linkConfig.asym_pause);
			printf("\t%40s:\t%s\n", "Most recently read link state", linkConfig.link ? "Up" : "Down");
			print_advert(buf, sizeof(buf), linkConfig.supported);
			printf("\t%40s:\t0x%llx (%s)\n", "GPHY supported modes", linkConfig.supported, buf);
			print_advert(buf, sizeof(buf), linkConfig.advertising);
			printf("\t%40s:\t0x%llx (%s)\n", "GPHY advertising modes", linkConfig.advertising, buf);
			print_advert(buf, sizeof(buf), linkConfig.lp_advertising);
			printf("\t%40s:\t0x%llx (%s)\n", "Link partner advertising modes", linkConfig.lp_advertising, buf);
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_read", strlen("gpy2xx_read")) == 0) {
		int cnt = 0;
		u32 reg_off;

		cnt += scanParamArg(argc, argv, "regaddr", sizeof(reg_off), &reg_off);

		ret = gpy2xx_read(&phy, reg_off);

		if (ret < 0) {
			printf("\nERROR: MDIO read failed.\n");
		} else {
			printf("\nMdio rd vale = 0x%x\n", ret);
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_write", strlen("gpy2xx_write")) == 0) {
		int cnt = 0;
		u32 reg_off;
		u16 data_val;

		cnt += scanParamArg(argc, argv, "regaddr", sizeof(reg_off), &reg_off);
		cnt += scanParamArg(argc, argv, "data", sizeof(data_val), &data_val);

		ret = gpy2xx_write(&phy, reg_off, data_val);

		if (ret < 0) {
			printf("\nERROR: MDIO write failed.\n");
		}

#if defined(P31G_IND_RW) && P31G_IND_RW
	} else if (xstrncasecmp(argv[2], "pm_ind_read", strlen("pm_ind_read")) == 0) {
		int cnt = 0;
		u32 reg_off, data_val;

		cnt += scanParamArg(argc, argv, "regaddr", sizeof(reg_off), &reg_off);

		ret = pm_indirect_read(&phy, reg_off, &data_val);

		if (ret < 0) {
			printf("\nERROR: PM Indirect read failed.\n");
		} else {
			printf("\nPM rd vale = 0x%x\n", data_val);
		}
	} else if (xstrncasecmp(argv[2], "pm_ind_write", strlen("pm_ind_write")) == 0) {
		int cnt = 0;
		u32 reg_off, data_val;

		cnt += scanParamArg(argc, argv, "regaddr", sizeof(reg_off), &reg_off);
		cnt += scanParamArg(argc, argv, "data", sizeof(data_val), &data_val);

		ret = pm_indirect_write(&phy, reg_off, data_val);

		if (ret < 0) {
			printf("\nERROR: PM write failed.\n");
		}

#endif
	} else if (xstrncasecmp(argv[2], "gpy2xx_mbox_read16", strlen("gpy2xx_mbox_read16")) == 0) {
		int cnt = 0;
		u32 reg_off, data_val;

		cnt += scanParamArg(argc, argv, "regaddr", sizeof(reg_off), &reg_off);

		ret = gpy2xx_mbox_read16(&phy, reg_off, &data_val);

		if (ret < 0) {
			printf("\nERROR: PM MBOX read failed.\n");
		} else {
			printf("\nMBOX rd vale = 0x%x\n", data_val);
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_mbox_write16", strlen("gpy2xx_mbox_write16")) == 0) {
		int cnt = 0;
		u32 reg_off, data_val;

		cnt += scanParamArg(argc, argv, "regaddr", sizeof(reg_off), &reg_off);
		cnt += scanParamArg(argc, argv, "data", sizeof(data_val), &data_val);

		ret = gpy2xx_mbox_write16(&phy, reg_off, data_val);

		if (ret < 0) {
			printf("\nERROR: MBOX write failed.\n");
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_mbox_read32", strlen("gpy2xx_mbox_read32")) == 0) {
		int cnt = 0;
		u32 reg_off, data_val;

		cnt += scanParamArg(argc, argv, "regaddr", sizeof(reg_off), &reg_off);

		ret = gpy2xx_mbox_read32(&phy, reg_off, &data_val);

		if (ret < 0) {
			printf("\nERROR: PM MBOX read failed.\n");
		} else {
			printf("\nMBOX rd vale = 0x%x\n", data_val);
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_mbox_write32", strlen("gpy2xx_mbox_write32")) == 0) {
		int cnt = 0;
		u32 reg_off, data_val;

		cnt += scanParamArg(argc, argv, "regaddr", sizeof(reg_off), &reg_off);
		cnt += scanParamArg(argc, argv, "data", sizeof(data_val), &data_val);

		ret = gpy2xx_mbox_write32(&phy, reg_off, data_val);

		if (ret < 0) {
			printf("\nERROR: MBOX write failed.\n");
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_xpcs_read16", strlen("gpy2xx_xpcs_read16")) == 0) {
		int cnt = 0;
		u32 reg_off, data_val;

		cnt += scanParamArg(argc, argv, "regaddr", sizeof(reg_off), &reg_off);

		ret = PHY_XPCS_HWRD(&phy, reg_off, &data_val);

		if (ret < 0) {
			printf("\nERROR: PHY XPCS read failed.\n");
		} else {
			printf("\nPHY XPCS rd vale = 0x%x\n", data_val);
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_xpcs_write16", strlen("gpy2xx_xpcs_write16")) == 0) {
		int cnt = 0;
		u32 reg_off, data_val;

		cnt += scanParamArg(argc, argv, "regaddr", sizeof(reg_off), &reg_off);
		cnt += scanParamArg(argc, argv, "data", sizeof(data_val), &data_val);

		ret = PHY_XPCS_HWWR(&phy, reg_off, data_val);

		if (ret < 0) {
			printf("\nERROR: PHY XPCS write failed.\n");
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_config_advert", strlen("gpy2xx_config_advert")) == 0) {
		char buf[256];

		if (scan_advert(argc, argv, "advertising", &phy.link.advertising) == 0)
			phy.link.advertising = phy.link.supported;

		print_advert(buf, sizeof(buf), phy.link.supported);
		printf("\t%24s:\t0x%llx (%s)\n", "GPHY supported modes", phy.link.supported, buf);
		print_advert(buf, sizeof(buf), phy.link.advertising);
		printf("\t%24s:\t0x%llx (%s)\n", "GPHY advertising modes", phy.link.advertising, buf);

		ret = gpy2xx_config_advert(&phy);

		if (ret < 0)
			printf("\nERROR: Setting Auto-Negotiation's advertisement conf failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_setup_forced", strlen("gpy2xx_setup_forced")) == 0) {
		if (scanParamArg(argc, argv, "speed", sizeof(phy.link.speed), &phy.link.speed) == 0)
			phy.link.speed = SPEED_2500;

		if (scanParamArg(argc, argv, "duplex", sizeof(phy.link.duplex), &phy.link.duplex) == 0)
			phy.link.duplex = DUPLEX_FULL;

		if (scanParamArg(argc, argv, "pause", sizeof(phy.link.pause), &phy.link.pause) == 0)
			phy.link.pause = 1;

		if (scanParamArg(argc, argv, "asym_pause", sizeof(phy.link.asym_pause), &phy.link.asym_pause) == 0)
			phy.link.asym_pause = 1;

		if (scanParamArg(argc, argv, "mstr_slave", sizeof(phy.mstr_slave), &phy.mstr_slave) == 0)
			phy.mstr_slave = 1;

		ret = gpy2xx_setup_forced(&phy);

		if (ret < 0)
			printf("\nERROR: Setting forceful link speed failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_restart_aneg", strlen("gpy2xx_restart_aneg")) == 0) {
		ret = gpy2xx_restart_aneg(&phy);

		if (ret < 0)
			printf("\nERROR: Restarting Auto-Negotiation failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_config_aneg", strlen("gpy2xx_config_aneg")) == 0) {
		char buf[256];

		if (scanParamArg(argc, argv, "autoneg", sizeof(phy.link.autoneg), &phy.link.autoneg) == 0)
			phy.link.autoneg = 1;

		if (scan_advert(argc, argv, "advertising", &phy.link.advertising) == 0)
			phy.link.advertising = phy.link.supported;

		if (scanParamArg(argc, argv, "speed", sizeof(phy.link.speed), &phy.link.speed) == 0)
			phy.link.speed = SPEED_2500;

		if (scanParamArg(argc, argv, "duplex", sizeof(phy.link.duplex), &phy.link.duplex) == 0)
			phy.link.duplex = DUPLEX_FULL;

		if (scanParamArg(argc, argv, "pause", sizeof(phy.link.pause), &phy.link.pause) == 0)
			phy.link.pause = 1;

		if (scanParamArg(argc, argv, "asym_pause", sizeof(phy.link.asym_pause), &phy.link.asym_pause) == 0)
			phy.link.asym_pause = 1;

		print_advert(buf, sizeof(buf), phy.link.supported);
		printf("\t%24s:\t0x%llx (%s)\n", "GPHY supported modes", phy.link.supported, buf);
		print_advert(buf, sizeof(buf), phy.link.advertising);
		printf("\t%24s:\t0x%llx (%s)\n", "GPHY advertising modes", phy.link.advertising, buf);

		ret = gpy2xx_config_aneg(&phy);

		if (ret < 0)
			printf("\nERROR: Configuring Auto-Negotiation failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_aneg_done", strlen("gpy2xx_aneg_done")) == 0) {
		ret = gpy2xx_aneg_done(&phy);

		if (ret == 0)
			printf("\nAuto-Negotiation pending.\n");
		else if (ret == 1)
			printf("\nAuto-Negotiation completed.\n");
		else
			printf("\nERROR: Auto-Negotiation polling failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_update_link", strlen("gpy2xx_update_link")) == 0) {
		ret = gpy2xx_update_link(&phy);

		if (ret < 0)
			printf("\nERROR: Updating link status failed.\n");

		if (phy.link.link == 0)
			printf("\nLink Down.\n");
		else
			printf("\nLink Up.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_gpio_cfg", strlen("gpy2xx_gpio_cfg")) == 0) {
		int cnt = 0;
		struct gpy211_gpio gpioCfg;
		memset(&gpioCfg, 0, sizeof(struct gpy211_gpio));

		//cnt += scanParamArg(argc, argv, "portId", sizeof(gpioCfg.port), &gpioCfg.port);
		cnt += scanParamArg(argc, argv, "pinId", sizeof(gpioCfg.pin), &gpioCfg.pin);
		cnt += scanParamArg(argc, argv, "gpioFlags", sizeof(gpioCfg.flags), &gpioCfg.flags);

		ret = gpy2xx_gpio_cfg(&phy, &gpioCfg);

		if (ret < 0)
			printf("\nERROR: Set GPIO configuration failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_gpio_get", strlen("gpy2xx_gpio_get")) == 0) {
		int cnt = 0;
		struct gpy211_gpio gpioCfg;
		memset(&gpioCfg, 0, sizeof(struct gpy211_gpio));

		//cnt += scanParamArg(argc, argv, "portId", sizeof(gpioCfg.port), &gpioCfg.port);
		cnt += scanParamArg(argc, argv, "pinId", sizeof(gpioCfg.pin), &gpioCfg.pin);

		ret = gpy2xx_gpio_get(&phy, &gpioCfg);

		if (ret < 0) {
			printf("\nERROR: Get GPIO configuration failed.\n");
			return ret;
		}

		printf("\t%60s:\t0x%x\n", "gpioDirection: 0 = In, 1 = Out", gpioCfg.flags & 0x1);

		if (gpioCfg.flags & 0x1)
			printf("\t%60s:\t0x%x\n", "gpioOutLevel: 0 = Low, 1 = High", (gpioCfg.flags >> 1) & 0x1);
		else
			printf("\t%60s:\t0x%x\n", "gpioInLevel: 0 = Low, 1 = High", (gpioCfg.flags >> 2) & 0x1);

		printf("\t%60s:\t0x%x\n", "gpioOpenDrain: 0 = OD, 1 = PP", (gpioCfg.flags >> 3) & 0x1);
		printf("\t%60s:\t0x%x\n", "gpioPuPd: 2 = Pull-Up, 3 = Pull-Down", (gpioCfg.flags >> 8) & 0x3);
		printf("\t%60s:\t0x%x\n", "gpioAltFunc: 0 = , 1 = , 2 = , 3 =", (gpioCfg.flags >> 10) & 0x3);
		printf("\t%60s:\t0x%x\n", "gpioPadStrength: 0 = 2mA, 1 = 4mA, 2 = 8mA, 3 = 12mA", (gpioCfg.flags >> 12) & 0x3);
		printf("\t%60s:\t0x%x\n", "gpioSlewRate: 0 = Slow, 3 = Fast", (gpioCfg.flags >> 15) & 0x1);
	} else if (xstrncasecmp(argv[2], "gpy2xx_gpio_output", strlen("gpy2xx_gpio_output")) == 0) {
		int cnt = 0;
		struct gpy211_gpio gpioCfg;
		memset(&gpioCfg, 0, sizeof(struct gpy211_gpio));

		//cnt += scanParamArg(argc, argv, "portId", sizeof(gpioCfg.port), &gpioCfg.port);
		cnt += scanParamArg(argc, argv, "pinId", sizeof(gpioCfg.pin), &gpioCfg.pin);

		ret = gpy2xx_gpio_output(&phy, &gpioCfg);

		if (ret < 0)
			printf("\nERROR: Set GPIO pin as output failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_gpio_input", strlen("gpy2xx_gpio_input")) == 0) {
		int cnt = 0;
		struct gpy211_gpio gpioCfg;
		memset(&gpioCfg, 0, sizeof(struct gpy211_gpio));

		cnt += scanParamArg(argc, argv, "pinId", sizeof(gpioCfg.pin), &gpioCfg.pin);

		ret = gpy2xx_gpio_input(&phy, &gpioCfg);

		if (ret < 0) {
			printf("\nERROR: Get GPIO pin input failed.\n");
			return ret;
		}

		printf("\t%40s:\t0x%x\n", "gpioFlags", gpioCfg.flags);
	} else if (xstrncasecmp(argv[2], "gpy2xx_led_br_cfg", strlen("gpy2xx_led_br_cfg")) == 0) {
		int cnt = 0;
		struct gpy211_led_brlvl_cfg brightnessCfg;
		memset(&brightnessCfg, 0, sizeof(struct gpy211_led_brlvl_cfg));

		ret = gpy2xx_led_br_get(&phy, &brightnessCfg);

		if (ret < 0) {
			printf("\nERROR: Get LED Brigtness configuration failed.\n");
			return ret;
		}

		cnt += scanParamArg(argc, argv, "brightMode", sizeof(brightnessCfg.mode), &brightnessCfg.mode);
		cnt += scanParamArg(argc, argv, "brightLvlMin", sizeof(brightnessCfg.lvl_min), &brightnessCfg.lvl_min);
		cnt += scanParamArg(argc, argv, "brightLvlMax", sizeof(brightnessCfg.lvl_max), &brightnessCfg.lvl_max);
		cnt += scanParamArg(argc, argv, "swEdgeDetect", sizeof(brightnessCfg.sw_edge), &brightnessCfg.sw_edge);

		ret = gpy2xx_led_br_cfg(&phy, &brightnessCfg);

		if (ret < 0)
			printf("\nERROR: Set LED Brigtness configuration failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_led_br_get", strlen("gpy2xx_led_br_get")) == 0) {
		struct gpy211_led_brlvl_cfg brightnessCfg;
		memset(&brightnessCfg, 0, sizeof(struct gpy211_led_brlvl_cfg));

		printf("calling gpy2xx_led_br_get inside comapre\n");
		ret = gpy2xx_led_br_get(&phy, &brightnessCfg);

		if (ret < 0) {
			printf("\nERROR: Get LED Brigtness configuration failed.\n");
			return ret;
		}


		printf("\t%40s:\t0x%x\n", "brightMode", brightnessCfg.mode);
		printf("\t%40s:\t0x%x\n", "brightLvlMin", brightnessCfg.lvl_min);
		printf("\t%40s:\t0x%x\n", "brightLvlMax", brightnessCfg.lvl_max);
		printf("\t%40s:\t0x%x\n", "swEdgeDetect", brightnessCfg.sw_edge);
#if defined(EN_LED_TOP_CFG) && EN_LED_TOP_CFG
	} else if (xstrncasecmp(argv[2], "gpy2xx_led_top_cfg", strlen("gpy2xx_led_top_cfg")) == 0) {
		int cnt = 0;
		struct gpy211_led_gcfg genLedCfg;
		memset(&genLedCfg, 0, sizeof(struct gpy211_led_gcfg));

		ret = gpy2xx_led_top_get(&phy, &genLedCfg);

		if (ret < 0) {
			printf("\nERROR: Get LED's General configuration failed.\n");
			return ret;
		}

		cnt += scanParamArg(argc, argv, "complexScan", sizeof(genLedCfg.complex_scan), &genLedCfg.complex_scan);
		cnt += scanParamArg(argc, argv, "complexBlink", sizeof(genLedCfg.complex_blink), &genLedCfg.complex_blink);
		cnt += scanParamArg(argc, argv, "inverseScan", sizeof(genLedCfg.inverse_scan), &genLedCfg.inverse_scan);
		cnt += scanParamArg(argc, argv, "fastBlinkFreq", sizeof(genLedCfg.fast_blink_freq), &genLedCfg.fast_blink_freq);
		cnt += scanParamArg(argc, argv, "slowBlinkFreq", sizeof(genLedCfg.slow_blink_freq), &genLedCfg.slow_blink_freq);

		ret = gpy2xx_led_top_cfg(&phy, &genLedCfg);

		if (ret < 0)
			printf("\nERROR: Set LED's General configuration failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_led_top_get", strlen("gpy2xx_led_top_get")) == 0) {
		struct gpy211_led_gcfg genLedCfg;
		memset(&genLedCfg, 0, sizeof(struct gpy211_led_gcfg));

		ret = gpy2xx_led_top_get(&phy, &genLedCfg);

		if (ret < 0) {
			printf("\nERROR: Get LED's General configuration failed.\n");
			return ret;
		}

		printf("\t%40s:\t0x%x\n", "complexScan", genLedCfg.complex_scan);
		printf("\t%40s:\t0x%x\n", "complexBlink", genLedCfg.complex_blink);
		printf("\t%40s:\t0x%x\n", "inverseScan", genLedCfg.inverse_scan);
		printf("\t%40s:\t0x%x\n", "fastBlinkFreq", genLedCfg.fast_blink_freq);
		printf("\t%40s:\t0x%x\n", "slowBlinkFreq", genLedCfg.slow_blink_freq);
#endif
	} else if (xstrncasecmp(argv[2], "gpy2xx_led_if_cfg", strlen("gpy2xx_led_if_cfg")) == 0) {
		int cnt = 0;
		struct gpy211_led_cfg genIfCfg;
		memset(&genIfCfg, 0, sizeof(struct gpy211_led_cfg));

		cnt += scanParamArg(argc, argv, "ledId", sizeof(genIfCfg.id), &genIfCfg.id);

		ret = gpy2xx_led_if_get(&phy, &genIfCfg);

		if (ret < 0) {
			printf("\nERROR: Get LED's IF configuration failed.\n");
			return ret;
		}

		cnt += scanParamArg(argc, argv, "colorMode", sizeof(genIfCfg.color_mode), &genIfCfg.color_mode);
		cnt += scanParamArg(argc, argv, "slowBlinkSrc", sizeof(genIfCfg.slow_blink_src), &genIfCfg.slow_blink_src);
		cnt += scanParamArg(argc, argv, "fastBlinkSrc", sizeof(genIfCfg.fast_blink_src), &genIfCfg.fast_blink_src);
		cnt += scanParamArg(argc, argv, "constantlyOn", sizeof(genIfCfg.const_on), &genIfCfg.const_on);
		cnt += scanParamArg(argc, argv, "pulseFlags", sizeof(genIfCfg.pulse), &genIfCfg.pulse);

		ret = gpy2xx_led_if_cfg(&phy, &genIfCfg);

		if (ret < 0)
			printf("\nERROR: Set LED's IF configuration failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_led_if_get", strlen("gpy2xx_led_if_get")) == 0) {
		int cnt = 0;
		struct gpy211_led_cfg genIfCfg;
		memset(&genIfCfg, 0, sizeof(struct gpy211_led_cfg));

		cnt += scanParamArg(argc, argv, "ledId", sizeof(genIfCfg.id), &genIfCfg.id);

		ret = gpy2xx_led_if_get(&phy, &genIfCfg);

		if (ret < 0) {
			printf("\nERROR: Get LED's IF configuration failed.\n");
			return ret;
		}

		printf("\t%40s:\t0x%x\n", "ledId", genIfCfg.id);
		printf("\t%40s:\t0x%x\n", "colorMode", genIfCfg.color_mode);
		printf("\t%40s:\t0x%x\n", "slowBlinkSrc", genIfCfg.slow_blink_src);
		printf("\t%40s:\t0x%x\n", "fastBlinkSrc", genIfCfg.fast_blink_src);
		printf("\t%40s:\t0x%x\n", "constantlyOn", genIfCfg.const_on);
		printf("\t%40s:\t0x%x\n", "pulseFlags", genIfCfg.pulse);
#if defined(EN_SUPPORT_TOP_INT) && EN_SUPPORT_TOP_INT
	} else if (xstrncasecmp(argv[2], "gpy2xx_topin_cfg", strlen("gpy2xx_topin_cfg")) == 0) {
		int cnt = 0;
		struct gpy211_extin intTopCfg;
		memset(&intTopCfg, 0, sizeof(struct gpy211_extin));

		cnt += scanParamArg(argc, argv, "intId", sizeof(intTopCfg.id), &intTopCfg.id);

		ret = gpy2xx_topin_get(&phy, &intTopCfg);

		if (ret < 0) {
			printf("\nERROR: Get Global/Top Interrupt global/top configuration failed.\n");
			return ret;
		}

		cnt += scanParamArg(argc, argv, "gpioFlags", sizeof(intTopCfg.gpio_flags), &intTopCfg.gpio_flags);
		cnt += scanParamArg(argc, argv, "ipChar", sizeof(intTopCfg.ip_char), &intTopCfg.ip_char);
		cnt += scanParamArg(argc, argv, "intMod0", sizeof(intTopCfg.im0), &intTopCfg.im0);
		cnt += scanParamArg(argc, argv, "intMod1", sizeof(intTopCfg.im1), &intTopCfg.im1);
		cnt += scanParamArg(argc, argv, "intMod2", sizeof(intTopCfg.im2), &intTopCfg.im2);

		ret = gpy2xx_topin_cfg(&phy, &intTopCfg);

		if (ret < 0)
			printf("\nERROR: Set Global/Top Interrupt configuration failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_topin_get", strlen("gpy2xx_topin_get")) == 0) {
		int cnt = 0;
		struct gpy211_extin intTopCfg;
		memset(&intTopCfg, 0, sizeof(struct gpy211_extin));

		cnt += scanParamArg(argc, argv, "intId", sizeof(intTopCfg.id), &intTopCfg.id);

		ret = gpy2xx_topin_get(&phy, &intTopCfg);

		if (ret < 0) {
			printf("\nERROR: Get Global/Top Interrupt global/top configuration failed.\n");
			return ret;
		}

		printf("\t%40s:\t0x%x\n", "intId", intTopCfg.id);
		printf("\t%40s:\t0x%x\n", "gpioFlags", intTopCfg.gpio_flags);
		printf("\t%40s:\t0x%x\n", "ipChar", intTopCfg.ip_char);
		printf("\t%40s:\t0x%x\n", "intMod0", intTopCfg.im0);
		printf("\t%40s:\t0x%x\n", "intMod1", intTopCfg.im1);
		printf("\t%40s:\t0x%x\n", "intMod2", intTopCfg.im2);
	} else if (xstrncasecmp(argv[2], "gpy2xx_topin_clr", strlen("gpy2xx_topin_clr")) == 0) {
		int cnt = 0;
		struct gpy211_extin intTopCfg;
		memset(&intTopCfg, 0, sizeof(struct gpy211_extin));

		cnt += scanParamArg(argc, argv, "intId", sizeof(intTopCfg.id), &intTopCfg.id);
		cnt += scanParamArg(argc, argv, "intMod0", sizeof(intTopCfg.im0), &intTopCfg.im0);
		cnt += scanParamArg(argc, argv, "intMod1", sizeof(intTopCfg.im1), &intTopCfg.im1);
		cnt += scanParamArg(argc, argv, "intMod2", sizeof(intTopCfg.im2), &intTopCfg.im2);

		ret = gpy2xx_topin_clr(&phy, &intTopCfg);

		if (ret < 0)
			printf("\nERROR: Clear Global/Top Interrupt status failed.\n");

#endif /* EN_SUPPORT_TOP_INT */
	} else if (xstrncasecmp(argv[2], "gpy2xx_extin_mask", strlen("gpy2xx_extin_mask")) == 0) {
		int cnt = 0;
		struct gpy211_phy_extin extInterrupt;
		memset(&extInterrupt, 0, sizeof(struct gpy211_phy_extin));

		cnt += scanParamArg(argc, argv, "stdImask", sizeof(extInterrupt.std_imask), &extInterrupt.std_imask);
		cnt += scanParamArg(argc, argv, "extImask", sizeof(extInterrupt.ext_imask), &extInterrupt.ext_imask);

		ret = gpy2xx_extin_mask(&phy, &extInterrupt);

		if (ret < 0)
			printf("\nERROR: Set External Interrupt configuration failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_extin_get", strlen("gpy2xx_extin_get")) == 0) {
		struct gpy211_phy_extin extInterrupt;
		memset(&extInterrupt, 0, sizeof(struct gpy211_phy_extin));

		ret = gpy2xx_extin_get(&phy, &extInterrupt);

		if (ret < 0) {
			printf("\nERROR: Get External Interrupt configuration failed.\n");
			return ret;
		}

		printf("\t%40s:\t0x%x\n", "Standard interrupt mask", extInterrupt.std_imask);
		printf("\t%40s:\t0x%x\n", "Standard interrupt status", extInterrupt.std_istat);
		printf("\t%40s:\t0x%x\n", "Exntended interrupt mask", extInterrupt.ext_imask);
		printf("\t%40s:\t0x%x\n", "Exntended interrupt status", extInterrupt.ext_istat);
#if defined(EN_GMAC_DEBUG_ACCESS) && EN_GMAC_DEBUG_ACCESS
	} else if (xstrncasecmp(argv[2], "gpy2xx_extin_clr", strlen("gpy2xx_extin_clr")) == 0) {
		int cnt = 0;
		struct gpy211_phy_extin extInterrupt;
		memset(&extInterrupt, 0, sizeof(struct gpy211_phy_extin));

		cnt += scanParamArg(argc, argv, "stdIstat", sizeof(extInterrupt.std_istat), &extInterrupt.std_istat);

		ret = gpy2xx_extin_clr(&phy, &extInterrupt);

		if (ret < 0)
			printf("\nERROR: Clear External Interrupt Status failed.\n");

#endif
	} else if (xstrncasecmp(argv[2], "gpy2xx_test_mode_cfg", strlen("gpy2xx_test_mode_cfg")) == 0) {
		int cnt = 0;
		enum gpy211_test_mode testMode;

		cnt += scanParamArg(argc, argv, "testTxMode", sizeof(testMode), &testMode);
		ret = gpy2xx_test_mode_cfg(&phy, testMode);

		if (ret < 0)
			printf("\nERROR: Set Test Transmitter mode failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_cdiag_start", strlen("gpy2xx_cdiag_start")) == 0) {
		ret = gpy2xx_cdiag_start(&phy);

		if (ret < 0)
			printf("\nERROR: Set Cable diagnostic (CDIAG) start failed.\n");
	} else if (xstrncasecmp(argv[0], "gpy2xx_cdiag_stop", strlen("gpy2xx_cdiag_stop")) == 0) {
		ret = gpy2xx_cdiag_stop(&phy);

		if (ret < 0)
			printf("\nERROR: Cable diagnostic (CDIAG) test stop failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_cdiag_read", strlen("gpy2xx_cdiag_read")) == 0) {
		u8 pairState, cableLength, validResults;
		int i = 0;
		struct gpy211_cdiag_report cdiagReport;
		memset(&cdiagReport, 0, sizeof(struct gpy211_cdiag_report));

		ret = gpy2xx_cdiag_read(&phy, &cdiagReport);

		if (ret < 0) {
			printf("\nERROR: CDIAG test result read failed.\n");
			return ret;
		}

		printf("\nReturned values:\n----------------\n");

		for (i = 0; i < 4; i++) {
			printf("\nThe CDIAG test results for DSP = %s.\n", ((i == 0) ? "A" : (i == 1) ? "B" : (i == 2) ? "C" : "D"));
			printf("\nTotal valid diagnostic reports = %d.\n", cdiagReport.pair[i].num_valid_result);
			validResults = cdiagReport.pair[i].num_valid_result;

			if (validResults) {
				cableLength = cdiagReport.pair[i].results[validResults - 1].distance - cdiagReport.pair[i].results[0].distance;
				printf("\t%40s:\t%d\n", "Estimated distance of cable (in meters)", cableLength);
				pairState = cdiagReport.pair[i].results[validResults - 1].state;
				printf("\t%40s:\t%s\n", "Detected echo", ((pairState == 1) ? "REFLECTION" : (pairState == 2) ? "OPEN" : (pairState == 4) ? "SHORT" : (pairState == 8) ? "MATCHED" : "ERROR"));
				printf("\t%40s:\t%d\n", "Echo coefficient of first detected peak", (s16)(cdiagReport.pair[i].results[validResults - 1].peak));
			}

			int k = 0;

			for (k = 0; k < 3; k++) {
				printf("\tThe sum-of-square of all XC%1d coefficients :\t%d\n", k, cdiagReport.pair[i].xc_pwr[k]);
			}

		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_abist_start", strlen("gpy2xx_abist_start")) == 0) {
		int cnt = 0;
		struct gpy211_abist_param abisTest;
		memset(&abisTest, 0, sizeof(struct gpy211_abist_param));

		cnt += scanParamArg(argc, argv, "enRestart", sizeof(abisTest.restart), &abisTest.restart);
		cnt += scanParamArg(argc, argv, "enUartReport", sizeof(abisTest.uart_report), &abisTest.uart_report);
		cnt += scanParamArg(argc, argv, "txTestMode", sizeof(abisTest.test), &abisTest.test);

		ret = gpy2xx_abist_start(&phy, &abisTest);

		if (ret < 0)
			printf("\nERROR: ABIS test start failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_abist_read", strlen("gpy2xx_abist_read")) == 0) {
		int i = 0, j = 0;
		struct gpy211_abist_report abistReport;
		memset(&abistReport, 0, sizeof(struct gpy211_abist_report));

		ret = gpy2xx_abist_read(&phy, &abistReport);

		if (ret < 0) {
			printf("\nERROR: ABIS test result read failed.\n");
			return ret;
		}

		printf("\nReturned ABIS test results:\n----------------------------\n");

		for (i = 0; i < 4; i++) {
			printf("The ABIS test results for ASP %s\n", ((i == 0) ? "A" : (i == 1) ? "B" : (i == 2) ? "C" : "D"));
			printf("\nIdle-Channel Noice GMax values\n");
			printf("\t%40s:\t0x%x\n", "Magnetude Max", abistReport.pair[i].icn_gmax.mag_max);
			printf("\t%40s:\t0x%x\n", "Magnetude Min", abistReport.pair[i].icn_gmax.mag_min);
			printf("\t%40s:\t0x%x\n", "Magnetude Avg", abistReport.pair[i].icn_gmax.mag_avg);
			printf("\t%40s:\t0x%x\n", "Magnetude DC", abistReport.pair[i].icn_gmax.mag_dc);
			printf("\nIdle-Channel Noice GMin values\n");
			printf("\t%40s:\t0x%x\n", "Magnetude Max", abistReport.pair[i].icn_gmin.mag_max);
			printf("\t%40s:\t0x%x\n", "Magnetude Min", abistReport.pair[i].icn_gmin.mag_min);
			printf("\t%40s:\t0x%x\n", "Magnetude Avg", abistReport.pair[i].icn_gmin.mag_avg);
			printf("\t%40s:\t0x%x\n", "Magnetude DC", abistReport.pair[i].icn_gmin.mag_dc);

			printf("\nAnalog Gain Control values\n");

			for (j = 0; j < 6; j++) {
				printf("\t%38s-%d:\t0x%x\n", "Agc-Pwr-", j, abistReport.pair[i].agc_hyb.agc_pwr[j]);
			}

			printf("\t%38s:\t0x%x\n", "Agc-Pwr-Mean", abistReport.pair[i].agc_hyb.agc_mean);
			printf("\t%38s:\t0x%x\n", "Agc-Pwr-Std", abistReport.pair[i].agc_hyb.agc_std);

			printf("\n10BT details with hybrid OFF\n");
			printf("\t%38s:\t0x%x\n", "FFT magnetude at DC", abistReport.pair[i].nohyb_10bt.dc_mag);
			printf("\t%38s:\t0x%x\n", "FFT magnitude at nyquist frequency", abistReport.pair[i].nohyb_10bt.nyq_mag);
			printf("\t%38s:\t0x%x\n", "FFT magnetude at fundamental frequency", abistReport.pair[i].nohyb_10bt.k1_mag);
			printf("\t%38s:\t0x%x\n", "Harmonics K2", abistReport.pair[i].nohyb_10bt.k2_mag);
			printf("\t%38s:\t0x%x\n", "Harmonics K3", abistReport.pair[i].nohyb_10bt.k3_mag);
			printf("\t%38s:\t0x%x\n", "Harmonics K4", abistReport.pair[i].nohyb_10bt.k4_mag);

			printf("\n100BT details with hybrid OFF\n");
			printf("\t%38s:\t0x%x\n", "FFT magnetude at DC", abistReport.pair[i].nohyb_100bt.dc_mag);
			printf("\t%38s:\t0x%x\n", "FFT magnitude at nyquist frequency", abistReport.pair[i].nohyb_100bt.nyq_mag);
			printf("\t%38s:\t0x%x\n", "FFT magnetude at fundamental frequency", abistReport.pair[i].nohyb_100bt.k1_mag);
			printf("\t%38s:\t0x%x\n", "Harmonics K2", abistReport.pair[i].nohyb_100bt.k2_mag);
			printf("\t%38s:\t0x%x\n", "Harmonics K3", abistReport.pair[i].nohyb_100bt.k3_mag);
			printf("\t%38s:\t0x%x\n", "Harmonics K4", abistReport.pair[i].nohyb_100bt.k4_mag);

			printf("\n1000BT details with hybrid ON\n");
			printf("\t%38s:\t0x%x\n", "FFT magnetude at DC", abistReport.pair[i].hyb_1000bt.dc_mag);
			printf("\t%38s:\t0x%x\n", "FFT magnitude at nyquist frequency", abistReport.pair[i].hyb_1000bt.nyq_mag);
			printf("\t%38s:\t0x%x\n", "FFT magnetude at fundamental frequency", abistReport.pair[i].hyb_1000bt.k1_mag);
			printf("\t%38s:\t0x%x\n", "Harmonics K2", abistReport.pair[i].hyb_1000bt.k2_mag);
			printf("\t%38s:\t0x%x\n", "Harmonics K3", abistReport.pair[i].hyb_1000bt.k3_mag);
			printf("\t%38s:\t0x%x\n", "Harmonics K4", abistReport.pair[i].hyb_1000bt.k4_mag);

			printf("\n1000BT details with hybrid OFF\n");
			printf("\t%38s:\t0x%x\n", "FFT magnetude at DC", abistReport.pair[i].nohyb_1000bt.dc_mag);
			printf("\t%38s:\t0x%x\n", "FFT magnitude at nyquist frequency", abistReport.pair[i].nohyb_1000bt.nyq_mag);
			printf("\t%38s:\t0x%x\n", "FFT magnetude at fundamental frequency", abistReport.pair[i].nohyb_1000bt.k1_mag);
			printf("\t%38s:\t0x%x\n", "Harmonics K2", abistReport.pair[i].nohyb_1000bt.k2_mag);
			printf("\t%38s:\t0x%x\n", "Harmonics K3", abistReport.pair[i].nohyb_1000bt.k3_mag);
			printf("\t%38s:\t0x%x\n", "Harmonics K4", abistReport.pair[i].nohyb_1000bt.k4_mag);

			printf("\n2500BT details with hybrid ON\n");
			printf("\t%38s:\t0x%x\n", "FFT magnetude at DC", abistReport.pair[i].hyb_2500bt.dc_mag);
			printf("\t%38s:\t0x%x\n", "FFT magnitude at nyquist frequency", abistReport.pair[i].hyb_2500bt.nyq_mag);
			printf("\t%38s:\t0x%x\n", "FFT magnetude at fundamental frequency", abistReport.pair[i].hyb_2500bt.k1_mag);
			printf("\t%38s:\t0x%x\n", "Harmonics K2", abistReport.pair[i].hyb_2500bt.k2_mag);
			printf("\t%38s:\t0x%x\n", "Harmonics K3", abistReport.pair[i].hyb_2500bt.k3_mag);
			printf("\t%38s:\t0x%x\n", "Harmonics K4", abistReport.pair[i].hyb_2500bt.k4_mag);

			printf("\n2500BT details with hybrid OFF\n");
			printf("\t%38s:\t0x%x\n", "FFT magnetude at DC", abistReport.pair[i].nohyb_2500bt.dc_mag);
			printf("\t%38s:\t0x%x\n", "FFT magnitude at nyquist frequency", abistReport.pair[i].nohyb_2500bt.nyq_mag);
			printf("\t%38s:\t0x%x\n", "FFT magnetude at fundamental frequency", abistReport.pair[i].nohyb_2500bt.k1_mag);
			printf("\t%38s:\t0x%x\n", "Harmonics K2", abistReport.pair[i].nohyb_2500bt.k2_mag);
			printf("\t%38s:\t0x%x\n", "Harmonics K3", abistReport.pair[i].nohyb_2500bt.k3_mag);
			printf("\t%38s:\t0x%x\n", "Harmonics K4", abistReport.pair[i].nohyb_2500bt.k4_mag);
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_loopback_cfg", strlen("gpy2xx_loopback_cfg")) == 0) {
		int cnt = 0;
		enum gpy211_test_loop loopMode;

		cnt += scanParamArg(argc, argv, "loopbackMode", sizeof(loopMode), &loopMode);
		ret = gpy2xx_loopback_cfg(&phy, loopMode);

		if (ret < 0)
			printf("\nERROR: Set Loopback test mode failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_errcnt_cfg", strlen("gpy2xx_errcnt_cfg")) == 0) {
		int cnt = 0;
		enum gpy211_errcnt_event cntMode;

		cnt += scanParamArg(argc, argv, "errCountMode", sizeof(cntMode), &cntMode);
		ret = gpy2xx_errcnt_cfg(&phy, cntMode);

		if (ret < 0)
			printf("\nERROR: Set Error or Event count mode failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_errcnt_read", strlen("gpy2xx_errcnt_read")) == 0) {
		ret = gpy2xx_errcnt_read(&phy);

		if (ret >= 0)
			printf("\nError or Event counter = 0x%x.\n", ret);
		else
			printf("\nERROR: Get Error or Event count failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_pcs_status_read", strlen("gpy2xx_pcs_status_read")) == 0) {
		struct gpy211_pcs_status pcsStatus;
		memset(&pcsStatus, 0, sizeof(struct gpy211_pcs_status));

		ret = gpy2xx_pcs_status_read(&phy, &pcsStatus);

		if (ret < 0) {
			printf("\nERROR: PCS status read failed.\n");
			return ret;
		}

		printf("\nBASE-R or 10GBASE-T PCS Status:\n----------------\n");
		printf("\t%40s:\t0x%x\n", "Bit Error Rate (BER)", pcsStatus.ber);
		printf("\t%40s:\t0x%x\n", "Number of Errored Blocks", pcsStatus.errored_block);
		printf("\t%40s:\t0x%x\n", "Reporting high BER status", pcsStatus.high_ber);
		printf("\t%40s:\t0x%x\n", "Received Block Lock status", pcsStatus.block_lock);
		printf("\t%40s:\t0x%x\n", "Receive link status", pcsStatus.rcv_link_up);
	} else if (xstrncasecmp(argv[2], "gpy2xx_ptp_adjfreq", strlen("gpy2xx_ptp_adjfreq")) == 0) {
		int cnt = 0;
		s32 pbb = 0;

		cnt += scanParamArg(argc, argv, "pbbTime", sizeof(int), &pbb);

		ret = gpy2xx_ptp_adjfreq(&phy, pbb);

		if (ret < 0)
			printf("\nERROR: Set PTP Frequency adjusting/modify failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_ptp_adjtime", strlen("gpy2xx_ptp_adjtime")) == 0) {
		int cnt = 0;
		s64 delta;

		cnt += scanParamArg(argc, argv, "deltaTime", sizeof(delta), &delta);

		ret = gpy2xx_ptp_adjtime(&phy, delta);

		if (ret < 0)
			printf("\nERROR: Adjust PTP System Time failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_ptp_gettime", strlen("gpy2xx_ptp_gettime")) == 0) {
		struct timespec64 refTime;
		memset(&refTime, 0, sizeof(struct timespec64));

		ret = gpy2xx_ptp_gettime(&phy, &refTime);

		if (ret < 0) {
			printf("\nGet PTP System Time failed.\n");
			return ret;
		}

		printf("\t%s: %llx, \t%lx\n", "PTP system time sec n nsec", refTime.tv_sec, refTime.tv_nsec);
		print_sys_time(refTime.tv_sec, refTime.tv_nsec);
	} else if (xstrncasecmp(argv[2], "gpy2xx_ptp_settime", strlen("gpy2xx_ptp_settime")) == 0) {
		int cnt = 0;
		struct timespec64 ts;
		memset(&ts, 0, sizeof(struct timespec64));

		cnt += scanParamArg(argc, argv, "initSec", 64, &ts.tv_sec);
		cnt += scanParamArg(argc, argv, "initNsec", sizeof(ts.tv_nsec), &ts.tv_nsec);

		ret = gpy2xx_ptp_settime(&phy, &ts);

		printf("\nInfo: Current Unix Epoch Clock from \"https://www.epochconverter.com/clock\"\n");
		if (ret < 0)
			printf("\nERROR: Set PTP System Time failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_ptp_enable", strlen("gpy2xx_ptp_enable")) == 0) {
		u16 data;
		time64_t corr_time;
		int cnt = 0;
		struct gpy211_ptp_ctrl ptpCfg;
		memset(&ptpCfg, 0, sizeof(struct gpy211_ptp_ctrl));

		cnt += scanParamArg(argc, argv, "enPhyTxTs", sizeof(data), &data);
		ptpCfg.tx_ts_phy = data;
		cnt += scanParamArg(argc, argv, "enPhyRxTs", sizeof(data), &data);
		ptpCfg.rx_ts_phy = data;
		cnt += scanParamArg(argc, argv, "txPtpOffset", sizeof(data), &data);
		ptpCfg.tx_ptp_off = data;
		cnt += scanParamArg(argc, argv, "txPtpProto", sizeof(data), &data);
		ptpCfg.tx_ptp_tpt = data;
		cnt += scanParamArg(argc, argv, "enTxOstcEn", sizeof(data), &data);
		ptpCfg.ts_ostc_en = data;
		cnt += scanParamArg(argc, argv, "txOstCorTime", sizeof(corr_time), &corr_time);
		ptpCfg.tx_ost_corr = corr_time;

		ret = gpy2xx_ptp_enable(&phy, &ptpCfg);

		if (ret < 0)
			printf("\nERROR: PTP enable failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_ptp_disable", strlen("gpy2xx_ptp_disable")) == 0) {
		ret = gpy2xx_ptp_disable(&phy);

		if (ret < 0)
			printf("\nERROR: PTP disable failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_ptp_getcfg", strlen("gpy2xx_ptp_getcfg")) == 0) {
		struct gpy211_ptp_cfg ptpCfg;
		memset(&ptpCfg, 0, sizeof(struct gpy211_ptp_cfg));

		ret = gpy2xx_ptp_getcfg(&phy, &ptpCfg);

		if (ret < 0) {
			printf("\nERROR: Get PTP TS configuration failed.\n");
			return ret;
		}

		printf("\t%40s:\t0x%x\n", "enPhyTxTs", ptpCfg.ptp_ctrl.tx_ts_phy);
		printf("\t%40s:\t0x%x\n", "enPhyRxTs", ptpCfg.ptp_ctrl.rx_ts_phy);
		printf("\t%40s:\t0x%x\n", "txPtpOffset", ptpCfg.ptp_ctrl.tx_ptp_off);
		printf("\t%40s:\t0x%x\n", "txPtpProto", ptpCfg.ptp_ctrl.tx_ptp_tpt);
		printf("\t%40s:\t0x%x\n", "enTxOstcEn", ptpCfg.ptp_ctrl.ts_ostc_en);
		printf("\t%40s:\t0x%llx\n", "txOstCorTime", ptpCfg.ptp_ctrl.tx_ost_corr);

		printf("\t%40s:\t0x%x\n", "enEvntMsg", ptpCfg.ts_ctrl.ts_evtmsg_en);
		printf("\t%40s:\t0x%x\n", "enMstrMsg", ptpCfg.ts_ctrl.ts_master_en);
		printf("\t%40s:\t0x%x\n", "snapType", ptpCfg.ts_ctrl.snap_type);

		printf("\t%40s:\t0x%x\n", "ppsCtrlLow", ptpCfg.pps_ctrl.ppsctrl_low.raw_pps_ctrl_low);
		printf("\t%40s:\t0x%x\n", "targetSec", ptpCfg.pps_ctrl.sec_tgttime);
		printf("\t%40s:\t0x%x\n", "targetNsc", ptpCfg.pps_ctrl.nsec_tgttime);
		printf("\t%40s:\t0x%x\n", "ppsInterval", ptpCfg.pps_ctrl.pps0_interal);
		printf("\t%40s:\t0x%x\n", "ppsWidth", ptpCfg.pps_ctrl.pps0_width);

		printf("\t%40s:\t0x%x\n", "ptoCtrlLow", ptpCfg.pto_ctrl.ptoctrl_low.raw_pto_ctrl_low);
		printf("\t%40s:\t%2x%2x%2x%2x%2x%2x%2x%2x%2x%2x\n", "srcPortID", ptpCfg.pto_ctrl.sport_id[0], ptpCfg.pto_ctrl.sport_id[1], ptpCfg.pto_ctrl.sport_id[2], ptpCfg.pto_ctrl.sport_id[3], ptpCfg.pto_ctrl.sport_id[4], ptpCfg.pto_ctrl.sport_id[5], ptpCfg.pto_ctrl.sport_id[6], ptpCfg.pto_ctrl.sport_id[7], ptpCfg.pto_ctrl.sport_id[8], ptpCfg.pto_ctrl.sport_id[9]);
		printf("\t%40s:\t0x%x\n", "logmLvlLow", ptpCfg.pto_ctrl.logmsg_lvl.raw_logm_lvl_low);
		printf("\t%40s:\t0x%x\n", "logmLvlHi", ptpCfg.pto_ctrl.logmsg_lvl.raw_logm_lvl_hi);
	} else if (xstrncasecmp(argv[2], "gpy2xx_ptp_set_tsctrl", strlen("gpy2xx_ptp_set_tsctrl")) == 0) {
		u16 data;
		int cnt = 0;
		struct gpy2xx_ts_ctrl tsCfg;
		memset(&tsCfg, 0, sizeof(struct gpy2xx_ts_ctrl));

		cnt += scanParamArg(argc, argv, "enEvntMsg", sizeof(data), &data);
		tsCfg.ts_evtmsg_en = data;
		cnt += scanParamArg(argc, argv, "enMstrMsg", sizeof(data), &data);
		tsCfg.ts_master_en = data;
		cnt += scanParamArg(argc, argv, "snapType", sizeof(data), &data);
		tsCfg.snap_type = data;

		ret = gpy2xx_ptp_set_tsctrl(&phy, &tsCfg);

		if (ret < 0)
			printf("\nERROR: Set PTP TS configuration failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_ptp_set_ppsctrl", strlen("gpy2xx_ptp_set_ppsctrl")) == 0) {
		int cnt = 0;
		struct gpy2xx_pps_ctrl ppsCfg;
		memset(&ppsCfg, 0, sizeof(struct gpy2xx_pps_ctrl));

		ret = gpy2xx_ptp_get_ppsctrl(&phy, &ppsCfg);

		if (ret < 0) {
			LOG_CRIT("ERROR: api gpy2xx_ptp_get_ppsctrl fails\n");
			return ret;
		}

		cnt += scanParamArg(argc, argv, "ppsCtrlLow", sizeof(ppsCfg.ppsctrl_low.raw_pps_ctrl_low), &ppsCfg.ppsctrl_low.raw_pps_ctrl_low);
		cnt += scanParamArg(argc, argv, "targetSec", sizeof(ppsCfg.sec_tgttime), &ppsCfg.sec_tgttime);
		cnt += scanParamArg(argc, argv, "targetNsc", sizeof(ppsCfg.nsec_tgttime), &ppsCfg.nsec_tgttime);
		cnt += scanParamArg(argc, argv, "ppsInterval", sizeof(ppsCfg.pps0_interal), &ppsCfg.pps0_interal);
		cnt += scanParamArg(argc, argv, "ppsWidth", sizeof(ppsCfg.pps0_width), &ppsCfg.pps0_width);
		cnt += scanParamArg(argc, argv, "gpcSel", sizeof(ppsCfg.gpc_sel), &ppsCfg.gpc_sel);

		ret = gpy2xx_ptp_set_ppsctrl(&phy, &ppsCfg);

		if (ret < 0)
			printf("\nERROR: Set PPS configuration failed.\n");
	} else if (xstrncasecmp(argv[2], "test_gpy2xx_ptp_set_ppsctrl", strlen("test_gpy2xx_ptp_set_ppsctrl")) == 0) {
		int cnt = 0;
		struct gpy2xx_pps_ctrl ppsCfg;
		memset(&ppsCfg, 0, sizeof(struct gpy2xx_pps_ctrl));
		struct timespec64 refTime;
		memset(&refTime, 0, sizeof(struct timespec64));

		ret = gpy2xx_ptp_gettime(&phy, &refTime);

		if (ret < 0) {
			printf("\nGet PTP System Time failed.\n");
			return ret;
		}

		ret = gpy2xx_ptp_get_ppsctrl(&phy, &ppsCfg);

		if (ret < 0) {
			LOG_CRIT("ERROR: api gpy2xx_ptp_get_ppsctrl fails\n");
			return ret;
		}

		ppsCfg.sec_tgttime = refTime.tv_sec + 15;
		ppsCfg.nsec_tgttime = 0x0; //refTime.tv_nsec;
		printf("\n\t%s: %x, \t%x\n", "Set time sec n nsec", ppsCfg.sec_tgttime, ppsCfg.nsec_tgttime);
		cnt += scanParamArg(argc, argv, "gpcSel", sizeof(ppsCfg.gpc_sel), &ppsCfg.gpc_sel);
		ret = gpy2xx_ptp_set_ppsctrl(&phy, &ppsCfg);

		if (ret < 0)
			printf("\nERROR: Test PPS configuration feature failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_ptp_set_ptoctrl", strlen("gpy2xx_ptp_set_ptoctrl")) == 0) {
		int cnt = 0;
		struct gpy2xx_pto_ctrl ptoCfg;
		memset(&ptoCfg, 0, sizeof(struct gpy2xx_pto_ctrl));

		cnt += scanParamArg(argc, argv, "ptoCtrlLow", sizeof(ptoCfg.ptoctrl_low.raw_pto_ctrl_low), &ptoCfg.ptoctrl_low.raw_pto_ctrl_low);
		cnt += scanKey_Arg(argc, argv, "srcPortID", 10, (char *)ptoCfg.sport_id);
		cnt += scanParamArg(argc, argv, "logmLvlLow", sizeof(ptoCfg.logmsg_lvl.raw_logm_lvl_low), &ptoCfg.logmsg_lvl.raw_logm_lvl_low);
		cnt += scanParamArg(argc, argv, "logmLvlHi", sizeof(ptoCfg.logmsg_lvl.raw_logm_lvl_hi), &ptoCfg.logmsg_lvl.raw_logm_lvl_hi);

		ret = gpy2xx_ptp_set_ptoctrl(&phy, &ptoCfg);

		if (ret < 0) {
			printf("\nERROR: Set PTO configuration failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_ptp_fifostat", strlen("gpy2xx_ptp_fifostat")) == 0) {
		struct gpy211_ts_fifo_stat fifoStatus;
		memset(&fifoStatus, 0, sizeof(struct gpy211_ts_fifo_stat));

		ret = gpy2xx_ptp_fifostat(&phy, &fifoStatus);

		if (ret < 0) {
			printf("\nERROR: Get PTP TS Status failed.\n");
			return ret;
		}

		printf("\t%40s:\t0x%x\n", "rxOverflow", fifoStatus.rx_ovfl);
		printf("\t%40s:\t0x%x\n", "rxUnderflow", fifoStatus.rx_udfl);
		printf("\t%40s:\t0x%x\n", "rxFillLevel", fifoStatus.rx_fill_lvl);
		printf("\t%40s:\t0x%x\n", "txFillLevel", fifoStatus.tx_fill_lvl);
		printf("\t%40s:\t0x%x\n", "txOverflow", fifoStatus.tx_ovfl);
		printf("\t%40s:\t0x%x\n", "txUnderflow", fifoStatus.tx_udfl);
	} else if (xstrncasecmp(argv[2], "gpy2xx_ptp_resetfifo", strlen("gpy2xx_ptp_resetfifo")) == 0) {
		ret = gpy2xx_ptp_resetfifo(&phy);

		if (ret < 0)
			printf("\nERROR: TS's FIFO reset failed.\n");

#if defined(EN_GMAC_DEBUG_ACCESS) && EN_GMAC_DEBUG_ACCESS
	} else if (xstrncasecmp(argv[2], "gpy2xx_ptp_txtsstat", strlen("gpy2xx_ptp_txtsstat")) == 0) {
		struct gpy211_ts_stat tsStatus;
		memset(&tsStatus, 0, sizeof(struct gpy211_ts_stat));

		ret = gpy2xx_ptp_txtsstat(&phy, &tsStatus);

		if (ret < 0) {
			printf("\nERROR: Get PTP Tx TS Status failed.\n");
			return ret;
		}

		printf("\t%40s:\t0x%x\n", "Tx TS Stat low", tsStatus.tss_low.raw_ts_stat_low);
		printf("\t%40s:\t0x%x\n", "Tx TS Stat low", tsStatus.tss_high.raw_ts_stat_hi);
		printf("\t%40s:\t0x%llx\n", "Tx TS (in nsec)", (u64)(tsStatus.txts_stat.txtss_sec * 0x3B9ACA00 + tsStatus.txts_stat.txtss_nsec));
		printf("\t%40s:\t0x%x\n", "Tx TS Stat missed", tsStatus.txts_stat.txtss_mis);
#endif
	} else if (xstrncasecmp(argv[2], "gpy2xx_ptp_getrxts", strlen("gpy2xx_ptp_getrxts")) == 0) {
		struct gpy211_rx_ts rxTs;
		memset(&rxTs, 0, sizeof(struct gpy211_rx_ts));

		ret = gpy2xx_ptp_getrxts(&phy, &rxTs);

		if (ret < 0) {
			printf("\nERROR: Get PTP Receive Packet TimeStamp status and CRC failed.\n");
			return ret;
		}

		printf("\t%s:\t0x%llx\n", "PTP Receive Packet TimeStamp status", rxTs.rx_ts_stat);
		print_sys_time(rxTs.rx_ts_stat / 1000000000ULL, rxTs.rx_ts_stat % 1000000000ULL);
		printf("\t%s:\t0x%x\n", "PTP Receive Packet CRC ", rxTs.rx_crc_stat);
	} else if (xstrncasecmp(argv[2], "gpy2xx_ptp_gettxts", strlen("gpy2xx_ptp_gettxts")) == 0) {
		struct gpy211_tx_ts txTs;
		memset(&txTs, 0, sizeof(struct gpy211_tx_ts));

		ret = gpy2xx_ptp_gettxts(&phy, &txTs);

		if (ret < 0) {
			printf("\nERROR: Get PTP Trasmit Packet TimeStamp status and CRC failed.\n");
			return ret;
		}

		printf("\t%s:\t0x%llx\n", "PTP Trasmit Packet TimeStamp status", txTs.tx_ts_stat);
		print_sys_time(txTs.tx_ts_stat / 1000000000ULL, txTs.tx_ts_stat % 1000000000ULL);
		printf("\t%s:\t0x%x\n", "PTP Trasmit Packet CRC ", txTs.tx_crc_stat);
	} else if (xstrncasecmp(argv[2], "gpy2xx_synce_cfg", strlen("gpy2xx_synce_cfg")) == 0) {
		int cnt = 0;
		struct gpy211_synce synceCfg;
		memset(&synceCfg, 0, sizeof(struct gpy211_synce));

		cnt += scanParamArg(argc, argv, "enSyncE", sizeof(synceCfg.synce_enable), &synceCfg.synce_enable);
		cnt += scanParamArg(argc, argv, "synceRefClk", sizeof(synceCfg.synce_refclk), &synceCfg.synce_refclk);
		cnt += scanParamArg(argc, argv, "masterSlave", sizeof(synceCfg.master_sel), &synceCfg.master_sel);
		cnt += scanParamArg(argc, argv, "dataRate", sizeof(synceCfg.data_rate), &synceCfg.data_rate);
		cnt += scanParamArg(argc, argv, "gpcSel", sizeof(synceCfg.gpc_sel), &synceCfg.gpc_sel);

		ret = gpy2xx_synce_cfg(&phy, &synceCfg);

		if (ret < 0) {
			printf("\nERROR: Set SyncE configuration failed.\n");
			return ret;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_synce_get", strlen("gpy2xx_synce_get")) == 0) {
		struct gpy211_synce synceCfg;
		memset(&synceCfg, 0, sizeof(struct gpy211_synce));

		ret = gpy2xx_synce_get(&phy, &synceCfg);

		if (ret < 0)
			printf("\nERROR: Get SyncE configuration failed.\n");

		printf("\t%40s:\t0x%x\n", "enSyncE", synceCfg.synce_enable);
		printf("\t%40s:\t0x%x\n", "synceRefClk", synceCfg.synce_refclk);
		printf("\t%40s:\t0x%x\n", "masterSlave", synceCfg.master_sel);
		printf("\t%40s:\t0x%x\n", "dataRate", synceCfg.data_rate);
		printf("\t%40s:\t0x%x\n", "gpcSel", synceCfg.gpc_sel);
	} else if (xstrncasecmp(argv[2], "gpy2xx_wol_cfg", strlen("gpy2xx_wol_cfg")) == 0) {
		int cnt = 0;
		struct gpy211_wolinfo wolInfo;
		memset(&wolInfo, 0, sizeof(struct gpy211_wolinfo));

		cnt += scanParamArg(argc, argv, "wolOptions", sizeof(wolInfo.wolopts), &wolInfo.wolopts);
		cnt += scanMAC_Arg(argc, argv, "wolMAC", wolInfo.mac);
		cnt += scanMAC_Arg(argc, argv, "wolPassword", wolInfo.sopass);

		ret = gpy2xx_wol_cfg(&phy, &wolInfo);

		if (ret < 0)
			printf("\nERROR: Set WOL configuration failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_wol_get", strlen("gpy2xx_wol_get")) == 0) {
		struct gpy211_wolinfo wolInfo;
		memset(&wolInfo, 0, sizeof(struct gpy211_wolinfo));

		ret = gpy2xx_wol_get(&phy, &wolInfo);

		if (ret < 0) {
			printf("\nERROR: Get WOL configuration failed.\n");
			return ret;
		}

		printf("\nWake-On-Lan configuration parameters:\n----------------\n");
		printf("\t%s:\t0x%x\n", "WOL flags supported", phy.wol_supported);
		printf("\t%s:\t0x%x\n", "WOL flags set", wolInfo.wolopts);
		printf("\t%s:\t", "WOL device MAC");
		printMAC_Address(wolInfo.mac);
		printf("\n");
		printf("\t%s:\t", "WOL device SecureOn password");
		printMAC_Address(wolInfo.sopass);
		printf("\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_ads_cfg", strlen("gpy2xx_ads_cfg")) == 0) {
		struct gpy211_ads_ctrl ads;
		memset(&ads, 0, sizeof(struct gpy211_ads_ctrl));

		if (scanParamArg(argc, argv, "advertise_enable", sizeof(ads.no_nrg_rst), &ads.no_nrg_rst) == 0)
			ads.no_nrg_rst = 0;

		if (scanParamArg(argc, argv, "NBaseT_ds_enable", sizeof(ads.downshift_en), &ads.downshift_en) == 0)
			ads.downshift_en = 0;


		if (scanParamArg(argc, argv, "downshift_cnt_thr", sizeof(ads.downshift_thr), &ads.downshift_thr) == 0)
			ads.downshift_thr = 0;

		if (scanParamArg(argc, argv, "force_reset", sizeof(ads.force_rst), &ads.force_rst) == 0)
			ads.force_rst = 0;

		if (scanParamArg(argc, argv, "rst_cntdown_timer", sizeof(ads.nrg_rst_cnt), &ads.nrg_rst_cnt) == 0)
			ads.nrg_rst_cnt = 0;

		ret = gpy2xx_ads_cfg(&phy, &ads);

		if (ret < 0)
			printf("\nERROR: Set Link's Auto Down Speed (ADS) mode failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_ads_get", strlen("gpy2xx_ads_get")) == 0) {
		struct gpy211_ads_sta ads;
		memset(&ads, 0, sizeof(struct gpy211_ads_sta));

		ret = gpy2xx_ads_get(&phy, &ads);

		if (ret < 0) {
			printf("\nERROR: Get Link's Auto Down Speed (ADS) mode failed.\n");
			return ret;
		}

		printf("%s:\n", "Auto Downspeed Configur");
		printf("\t%40s:\t%s\n", "Advertise all Speeds if No Energy Detected = ", ((ads.ads_ctrl.no_nrg_rst == 0) ? "Disable" : "Enable"));
		printf("\t%40s:\t%s\n", "NBASE-T Downshift Enable = ", ((ads.ads_ctrl.downshift_en == 0) ? "Disable" : "Enable"));
		printf("\t%40s:\t0x%x\n", "NBASE-T Downshift Training Counter Threshold = ", ads.ads_ctrl.downshift_thr);
		printf("\t%40s:\t0x%x\n", "Force Reset of Downwshift Process = ", ads.ads_ctrl.force_rst);
		printf("\t%40s:\t0x%x\n", "Timer to Reset the Downshift process = ", ads.ads_ctrl.nrg_rst_cnt);
		printf("\n");
		printf("%s:\n", "Auto Downspeed Status");
		printf("\t%40s:\t0x%x\n", "ADS Training attempt counter = ", ads.downshift_cnt);
		printf("\t%40s:\t0x%x\n", "ADS Downshift from 2G5 to lower speed = ", ads.downshift_2G5);
		printf("\t%40s:\t0x%x\n", "ADS Downshift from 1G to lower speed = ", ads.downshift_1G);
	} else if (xstrncasecmp(argv[2], "gpy2xx_ulp_cfg", strlen("gpy2xx_ulp_cfg")) == 0) {
		int cnt = 0;
		struct gpy211_ulp_ctrl ulp;
		memset(&ulp, 0, sizeof(struct gpy211_ulp_ctrl));

		cnt += scanParamArg(argc, argv, "ulp_enable", sizeof(ulp.ulp_en), &ulp.ulp_en);
		cnt += scanParamArg(argc, argv, "ulp_sta_block", sizeof(ulp.ulp_sta_block), &ulp.ulp_sta_block);
		cnt += scanParamArg(argc, argv, "ulp_exit", sizeof(ulp.ulp_exit), &ulp.ulp_exit);
		cnt += scanParamArg(argc, argv, "master_slice", sizeof(ulp.master_slice), &ulp.master_slice);

		ret = gpy2xx_ulp_cfg(&phy, &ulp);

		if (ret < 0)
			printf("\nERROR: Set ULP mode failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_ulp_get", strlen("gpy2xx_ulp_get")) == 0) {
		struct gpy211_ulp_ctrl ulp;
		memset(&ulp, 0, sizeof(struct gpy211_ulp_ctrl));

		ret = gpy2xx_ulp_get(&phy, &ulp);

		if (ret < 0) {
			printf("\nERROR: ULP mode failed.\n");
			return ret;
		}

		printf("\t%40s:\t%s\n", "ULP enable = ", ((ulp.ulp_en == 0) ? "Disable" : "Enable"));
		printf("\t%40s:\t%s\n", "ULP station block = ", ((ulp.ulp_sta_block == 0) ? "Disable" : "Enable"));
	} else if (xstrncasecmp(argv[2], "gpy2xx_ads_detected", strlen("gpy2xx_ads_detected")) == 0) {
		ret = gpy2xx_ads_detected(&phy);

		if (ret == 1)
			printf("\nLink Auto Down Speed detected.\n");
		else if (ret == 0)
			printf("\nLink Auto Down Speed not detected.\n");
		else
			printf("\nERROR: Link ADS detected state get failed.\n");

	} else if (xstrncasecmp(argv[2], "gpy2xx_sgmii_restart_aneg", strlen("gpy2xx_sgmii_restart_aneg")) == 0) {
		int ret;

		ret = gpy2xx_sgmii_restart_aneg(&phy);

		if (ret < 0)
			printf("\nERROR: Restarting SGMII Auto-Negotiation failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_sgmii_aneg_done", strlen("gpy2xx_sgmii_aneg_done")) == 0) {
		ret = gpy2xx_sgmii_aneg_done(&phy);

		if (ret == 0)
			printf("\nSGMII Auto-Negotiation pending.\n");
		else if (ret == 1)
			printf("\nSGMII Auto-Negotiation completed.\n");
		else
			printf("\nERROR: SGMII Auto-Negotiation polling failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_sgmii_config_aneg", strlen("gpy2xx_sgmii_config_aneg")) == 0) {
		//char buf[256];
		struct gpy211_sgmii sgmiiConfig;
		memset(&sgmiiConfig, 0, sizeof(struct gpy211_sgmii));

		if (scanParamArg(argc, argv, "linkcfg_dir", sizeof(sgmiiConfig.linkcfg_dir), &sgmiiConfig.linkcfg_dir) == 0)
			sgmiiConfig.linkcfg_dir = 1;

		if (scanParamArg(argc, argv, "aneg_mode", sizeof(sgmiiConfig.aneg_mode), &sgmiiConfig.aneg_mode) == 0)
			sgmiiConfig.aneg_mode = 1;

		if (scanParamArg(argc, argv, "eee_enable", sizeof(sgmiiConfig.eee_enable), &sgmiiConfig.eee_enable) == 0)
			sgmiiConfig.eee_enable = 1;

		if (scanParamArg(argc, argv, "autoneg", sizeof(sgmiiConfig.link.autoneg), &sgmiiConfig.link.autoneg) == 0)
			sgmiiConfig.link.autoneg = 1;

		if (scanParamArg(argc, argv, "speed", sizeof(sgmiiConfig.link.speed), &sgmiiConfig.link.speed) == 0)
			sgmiiConfig.link.speed = SPEED_2500;

		if (scanParamArg(argc, argv, "fixed2G5", sizeof(sgmiiConfig.link.fixed2g5), &sgmiiConfig.link.fixed2g5) == 0)
			sgmiiConfig.link.fixed2g5 = 0;

		if (scanParamArg(argc, argv, "duplex", sizeof(sgmiiConfig.link.duplex), &sgmiiConfig.link.duplex) == 0)
			sgmiiConfig.link.duplex = DUPLEX_FULL;

		ret = gpy2xx_sgmii_config_aneg(&phy, &sgmiiConfig);

		if (ret < 0)
			printf("\nERROR: Configuring SGMII Auto-Negotiation failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_sgmii_read_status", strlen("gpy2xx_sgmii_read_status")) == 0) {
		char buf[256];
		struct gpy211_sgmii sgmiiConfig;
		memset(&sgmiiConfig, 0, sizeof(struct gpy211_sgmii));
		ret = gpy2xx_sgmii_read_status(&phy, &sgmiiConfig);

		if (ret < 0)
			printf("\nERROR: Read SGMII link status failed.\n");

		printf("\t%40s:\t%d\n", "SGMII Link configuration direction", sgmiiConfig.linkcfg_dir);
		printf("\t%40s:\t%d\n", "SGMII ANEG Mode", sgmiiConfig.aneg_mode);
		printf("\t%40s:\t%d\n", "EEE SGMII Capability is advertised in ANEG", sgmiiConfig.eee_enable);

		printf("\nPopulated link configuration parameters:\n----------------\n");
		printf("\t%40s:\t%s\n", "Auto-Negotiation (AN) state", sgmiiConfig.link.autoneg ? "Enabled" : "Disabled");
		printf("\t%40s:\t%d\n", "Link Speed or Partner Link Speed (AN)", sgmiiConfig.link.speed);
		printf("\t%40s:\t%s\n", "Rate Adaptation (SGMII Fixed 2.5G)", sgmiiConfig.link.fixed2g5 ? "Enabled" : "Disabled");

		if (sgmiiConfig.link.autoneg == 0) {
			printf("\t%40s:\t%s\n", "Duplex (Forced) or Partner Duplex (AN)", sgmiiConfig.link.duplex ? "Full" : "Half");
		}

		printf("\t%40s:\t%s\n", "Most recently read link state", sgmiiConfig.link.link ? "Up" : "Down");
		print_advert(buf, sizeof(buf), sgmiiConfig.link.supported);
		printf("\t%40s:\t0x%llx (%s)\n", "GPHY supported modes", sgmiiConfig.link.supported, buf);
	} else if (xstrncasecmp(argv[2], "gpy2xx_sgmii_opmode", strlen("gpy2xx_sgmii_opmode")) == 0) {
		int cnt = 0;
		enum gpy211_sgmii_operation opMode = 0;

		cnt += scanParamArg(argc, argv, "opMode", sizeof(opMode), &opMode);
		ret = gpy2xx_sgmii_opmode(&phy, opMode);

		if (ret < 0)
			printf("\nERROR: Set SGMII operation mode failed.\n");
	} else if (xstrncasecmp(argv[2], "gpy2xx_pvt_get", strlen("gpy2xx_pvt_get")) == 0) {
		struct gpy211_pvt sensInfo;

		memset(&sensInfo, 0, sizeof(struct gpy211_pvt));
		ret = gpy2xx_pvt_get(&phy, &sensInfo);

		if (ret < 0)
			printf("\nERROR: Get SENSOR temparature failed.\n");

		printf("\t%s:\t%d\n", "Sensor temparature", sensInfo.temperature);
#if defined(EN_GMAC_DEBUG_ACCESS) && EN_GMAC_DEBUG_ACCESS
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacx_pm_hw_reset", strlen("gpy2xx_gmacx_pm_hw_reset")) == 0) {
		ret = gpy2xx_gmacx_pm_hw_reset(&phy);

		if (ret < 0)
			printf("\nERROR: PM level HW reset failed.\n");

#endif
	} else if (xstrncasecmp(argv[2], "gpy2xx_macsec_enable", strlen("gpy2xx_macsec_enable")) == 0) {
		int cnt = 0;
		struct gpy211_macsec msec_cfg;
		memset(&msec_cfg, 0, sizeof(struct gpy211_macsec));

		cnt += scanParamArg(argc, argv, "dropCrc", sizeof(msec_cfg.drop_on_crc_err), &msec_cfg.drop_on_crc_err);
		cnt += scanParamArg(argc, argv, "dropPerr", sizeof(msec_cfg.drop_on_pkt_err), &msec_cfg.drop_on_pkt_err);
		cnt += scanParamArg(argc, argv, "dropSfail", sizeof(msec_cfg.drop_on_sec_fail), &msec_cfg.drop_on_sec_fail);
		cnt += scanParamArg(argc, argv, "dropClass", sizeof(msec_cfg.class_drop), &msec_cfg.class_drop);

		ret = gpy2xx_macsec_enable(&phy, &msec_cfg);

		if (ret < 0) {
			printf("\nERROR: MACsec enable failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_macsec_disable", strlen("gpy2xx_macsec_disable")) == 0) {
		ret = gpy2xx_macsec_disable(&phy);

		if (ret < 0) {
			printf("\nERROR: MACsec disable set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacx_pm_pdi_cfg", strlen("gpy2xx_gmacx_pm_pdi_cfg")) == 0) {
		int cnt = 0;
		struct pm_config pm_cfg;
		memset(&pm_cfg, 0, sizeof(struct pm_config));

		ret = gpy2xx_gmacx_pm_pdi_get(&phy, &pm_cfg);

		if (ret < 0) {
			printf("\nERROR: PM cfg info get failed.\n");
			return -1;
		}

		cnt += scanParamArg(argc, argv, "bpMac", sizeof(pm_cfg.bypass_gmac), &pm_cfg.bypass_gmac);
		cnt += scanParamArg(argc, argv, "bpMsec", sizeof(pm_cfg.bypass_macsec), &pm_cfg.bypass_macsec);
		cnt += scanParamArg(argc, argv, "dropCrc", sizeof(pm_cfg.drop_on_crc_err), &pm_cfg.drop_on_crc_err);
		cnt += scanParamArg(argc, argv, "dropPerr", sizeof(pm_cfg.drop_on_pkt_err), &pm_cfg.drop_on_pkt_err);
		cnt += scanParamArg(argc, argv, "dropSfail", sizeof(pm_cfg.drop_on_sec_fail), &pm_cfg.drop_on_sec_fail);
		cnt += scanParamArg(argc, argv, "dropClass", sizeof(pm_cfg.class_drop), &pm_cfg.class_drop);
		cnt += scanParamArg(argc, argv, "cust3gEn", sizeof(pm_cfg.cust_3g_en), &pm_cfg.cust_3g_en);
		cnt += scanParamArg(argc, argv, "sgmiiLb", sizeof(pm_cfg.sgmii_lb_en), &pm_cfg.sgmii_lb_en);

		ret = gpy2xx_gmacx_pm_pdi_cfg(&phy, &pm_cfg);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full MAC cfg info set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacx_pm_pdi_get", strlen("gpy2xx_gmacx_pm_pdi_get")) == 0) {
		struct pm_config pm_cfg;
		memset(&pm_cfg, 0, sizeof(struct pm_config));

		ret = gpy2xx_gmacx_pm_pdi_get(&phy, &pm_cfg);

		if (ret < 0) {
			printf("\nERROR: PM cfg info get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "Bypass GMAC", pm_cfg.bypass_gmac);
		printf("\t%40s:\t0x%x\n", "Bypass MACSec", pm_cfg.bypass_macsec);
		printf("\t%40s:\t0x%x\n", "Discard CRC error pkts", pm_cfg.drop_on_crc_err);
		printf("\t%40s:\t0x%x\n", "Discard PKT error pkts", pm_cfg.drop_on_pkt_err);
		printf("\t%40s:\t0x%x\n", "Discard SEC fail pkts", pm_cfg.drop_on_sec_fail);
		printf("\t%40s:\t0x%x\n", "Classification Drop pkts", pm_cfg.class_drop);
		printf("\t%40s:\t0x%x\n", "Custom3G Speed enable", pm_cfg.cust_3g_en);
		printf("\t%40s:\t0x%x\n", "SGMII Loopback enable", pm_cfg.sgmii_lb_en);
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_pm_cfg", strlen("gpy2xx_gmacf_pm_cfg")) == 0) {
		int cnt = 0;
		struct pm_gmac_cfg gmac_ctrl;
		memset(&gmac_ctrl, 0, sizeof(struct pm_gmac_cfg));

		ret = gpy2xx_gmacf_pm_get(&phy, &gmac_ctrl);

		if (ret < 0) {
			printf("\nERROR: PM GMAC-Full info get failed.\n");
			return -1;
		}

		cnt += scanParamArg(argc, argv, "crcPad", sizeof(gmac_ctrl.crc_pad_ctrl), &gmac_ctrl.crc_pad_ctrl);
		cnt += scanParamArg(argc, argv, "txErr", sizeof(gmac_ctrl.tx_err_input), &gmac_ctrl.tx_err_input);
		cnt += scanParamArg(argc, argv, "bigEndian", sizeof(gmac_ctrl.big_endian), &gmac_ctrl.big_endian);

		ret = gpy2xx_gmacf_pm_cfg(&phy, &gmac_ctrl);

		if (ret < 0) {
			printf("\nERROR: PM GMAC-Full info set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_pm_get", strlen("gpy2xx_gmacf_pm_get")) == 0) {
		struct pm_gmac_cfg gmac_ctrl;
		memset(&gmac_ctrl, 0, sizeof(struct pm_gmac_cfg));

		ret = gpy2xx_gmacf_pm_get(&phy, &gmac_ctrl);

		if (ret < 0) {
			printf("\nERROR: PM GMAC-Full info get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "GMAC CRC and Pad Control", gmac_ctrl.crc_pad_ctrl);
		printf("\t%40s:\t0x%x\n", "GMAC Tx Error Input", gmac_ctrl.tx_err_input);
		printf("\t%40s:\t0x%x\n", "Data Endianness", gmac_ctrl.big_endian);
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacl_pm_cfg", strlen("gpy2xx_gmacl_pm_cfg")) == 0) {
		int cnt = 0;
		struct pm_gmac_cfg gmac_ctrl;
		memset(&gmac_ctrl, 0, sizeof(struct pm_gmac_cfg));

		ret = gpy2xx_gmacl_pm_get(&phy, &gmac_ctrl);

		if (ret < 0) {
			printf("\nERROR: PM GMAC-Full info get failed.\n");
			return -1;
		}

		cnt += scanParamArg(argc, argv, "crcPad", sizeof(gmac_ctrl.crc_pad_ctrl), &gmac_ctrl.crc_pad_ctrl);
		cnt += scanParamArg(argc, argv, "txErr", sizeof(gmac_ctrl.tx_err_input), &gmac_ctrl.tx_err_input);
		cnt += scanParamArg(argc, argv, "bigEndian", sizeof(gmac_ctrl.big_endian), &gmac_ctrl.big_endian);

		ret = gpy2xx_gmacl_pm_cfg(&phy, &gmac_ctrl);

		if (ret < 0) {
			printf("\nERROR: PM GMAC-Full info set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacl_pm_get", strlen("gpy2xx_gmacl_pm_get")) == 0) {
		struct pm_gmac_cfg gmac_ctrl;
		memset(&gmac_ctrl, 0, sizeof(struct pm_gmac_cfg));

		ret = gpy2xx_gmacl_pm_get(&phy, &gmac_ctrl);

		if (ret < 0) {
			printf("\nERROR: PM GMAC-Full info get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "GMAC CRC and Pad Control", gmac_ctrl.crc_pad_ctrl);
		printf("\t%40s:\t0x%x\n", "GMAC Tx Error Input", gmac_ctrl.tx_err_input);
		printf("\t%40s:\t0x%x\n", "Data Endianness", gmac_ctrl.big_endian);
#if defined(EN_GMAC_DEBUG_ACCESS) && EN_GMAC_DEBUG_ACCESS
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_lpi_cfg", strlen("gpy2xx_gmacf_lpi_cfg")) == 0) {
		int cnt = 0;
		struct lpi_cfg lpi_ctrl;
		memset(&lpi_ctrl, 0, sizeof(struct lpi_cfg));

		ret = gpy2xx_gmacf_lpi_get(&phy, &lpi_ctrl);

		if (ret < 0) {
			printf("\nERROR: PM GMAC-Full LPI info get failed.\n");
			return -1;
		}

		cnt += scanParamArg(argc, argv, "lpintEn", sizeof(lpi_ctrl.lpi_int_en), &lpi_ctrl.lpi_int_en);
		cnt += scanParamArg(argc, argv, "lpiEn", sizeof(lpi_ctrl.lpi_en), &lpi_ctrl.lpi_en);
		cnt += scanParamArg(argc, argv, "linkUp", sizeof(lpi_ctrl.phy_link_status), &lpi_ctrl.phy_link_status);
		cnt += scanParamArg(argc, argv, "txautoEn", sizeof(lpi_ctrl.lpi_tx_auto), &lpi_ctrl.lpi_tx_auto);
		cnt += scanParamArg(argc, argv, "timerEn", sizeof(lpi_ctrl.lpi_timer_en), &lpi_ctrl.lpi_timer_en);
		cnt += scanParamArg(argc, argv, "clkStop", sizeof(lpi_ctrl.lpi_tx_clk_stop_en), &lpi_ctrl.lpi_tx_clk_stop_en);
		cnt += scanParamArg(argc, argv, "ticTime", sizeof(lpi_ctrl.ous_tic_time), &lpi_ctrl.ous_tic_time);
		cnt += scanParamArg(argc, argv, "twTime", sizeof(lpi_ctrl.lpi_tw_time), &lpi_ctrl.lpi_tw_time);
		cnt += scanParamArg(argc, argv, "lsTime", sizeof(lpi_ctrl.lpi_ls_time), &lpi_ctrl.lpi_ls_time);
		cnt += scanParamArg(argc, argv, "entryTime", sizeof(lpi_ctrl.lpi_entry_time), &lpi_ctrl.lpi_entry_time);

		ret = gpy2xx_gmacf_lpi_cfg(&phy, &lpi_ctrl);

		if (ret < 0) {
			printf("\nERROR: PM GMAC-Full LPI info set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_lpi_get", strlen("gpy2xx_gmacf_lpi_get")) == 0) {
		struct lpi_cfg lpi_ctrl;
		memset(&lpi_ctrl, 0, sizeof(struct lpi_cfg));

		ret = gpy2xx_gmacf_lpi_get(&phy, &lpi_ctrl);

		if (ret < 0) {
			printf("\nERROR: PM GMAC-Full LPI info get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "LPI Interrupt Enable", lpi_ctrl.lpi_int_en);
		printf("\t%40s:\t0x%x\n", "LPI Enable", lpi_ctrl.lpi_en);
		printf("\t%40s:\t0x%x\n", "PHY Link Status", lpi_ctrl.phy_link_status);
		printf("\t%40s:\t0x%x\n", "LPI Tx Automate", lpi_ctrl.lpi_tx_auto);
		printf("\t%40s:\t0x%x\n", "LPI Timer Enable", lpi_ctrl.lpi_timer_en);
		printf("\t%40s:\t0x%x\n", "LPI Tx Clock Stop Enable", lpi_ctrl.lpi_tx_clk_stop_en);
		printf("\t%40s:\t0x%x\n", "1US TIC Counter", lpi_ctrl.ous_tic_time);
		printf("\t%40s:\t0x%x\n", "LPI TW Timer (in milliseconds)", lpi_ctrl.lpi_tw_time);
		printf("\t%40s:\t0x%x\n", "LPI LS Timer (in milliseconds)", lpi_ctrl.lpi_ls_time);
		printf("\t%40s:\t0x%x\n", "LPI Entry Timer (in milliseconds)", lpi_ctrl.lpi_entry_time);
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacl_lpi_cfg", strlen("gpy2xx_gmacl_lpi_cfg")) == 0) {
		int cnt = 0;
		struct lpi_cfg lpi_ctrl;
		memset(&lpi_ctrl, 0, sizeof(struct lpi_cfg));

		ret = gpy2xx_gmacl_lpi_get(&phy, &lpi_ctrl);

		if (ret < 0) {
			printf("\nERROR: PM GMAC-Lite LPI info get failed.\n");
			return -1;
		}

		cnt += scanParamArg(argc, argv, "lpintEn", sizeof(lpi_ctrl.lpi_int_en), &lpi_ctrl.lpi_int_en);
		cnt += scanParamArg(argc, argv, "lpiEn", sizeof(lpi_ctrl.lpi_en), &lpi_ctrl.lpi_en);
		cnt += scanParamArg(argc, argv, "linkUp", sizeof(lpi_ctrl.phy_link_status), &lpi_ctrl.phy_link_status);
		cnt += scanParamArg(argc, argv, "txautoEn", sizeof(lpi_ctrl.lpi_tx_auto), &lpi_ctrl.lpi_tx_auto);
		cnt += scanParamArg(argc, argv, "timerEn", sizeof(lpi_ctrl.lpi_timer_en), &lpi_ctrl.lpi_timer_en);
		cnt += scanParamArg(argc, argv, "clkStop", sizeof(lpi_ctrl.lpi_tx_clk_stop_en), &lpi_ctrl.lpi_tx_clk_stop_en);
		cnt += scanParamArg(argc, argv, "ticTime", sizeof(lpi_ctrl.ous_tic_time), &lpi_ctrl.ous_tic_time);
		cnt += scanParamArg(argc, argv, "twTime", sizeof(lpi_ctrl.lpi_tw_time), &lpi_ctrl.lpi_tw_time);
		cnt += scanParamArg(argc, argv, "lsTime", sizeof(lpi_ctrl.lpi_ls_time), &lpi_ctrl.lpi_ls_time);
		cnt += scanParamArg(argc, argv, "entryTime", sizeof(lpi_ctrl.lpi_entry_time), &lpi_ctrl.lpi_entry_time);

		ret = gpy2xx_gmacl_lpi_cfg(&phy, &lpi_ctrl);

		if (ret < 0) {
			printf("\nERROR: PM GMAC-Lite LPI info set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacl_lpi_get", strlen("gpy2xx_gmacl_lpi_get")) == 0) {
		struct lpi_cfg lpi_ctrl;
		memset(&lpi_ctrl, 0, sizeof(struct lpi_cfg));

		ret = gpy2xx_gmacl_lpi_get(&phy, &lpi_ctrl);

		if (ret < 0) {
			printf("\nERROR: PM GMAC-Lite LPI info get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "LPI Interrupt Enable", lpi_ctrl.lpi_int_en);
		printf("\t%40s:\t0x%x\n", "LPI Enable", lpi_ctrl.lpi_en);
		printf("\t%40s:\t0x%x\n", "PHY Link Status", lpi_ctrl.phy_link_status);
		printf("\t%40s:\t0x%x\n", "LPI Tx Automate", lpi_ctrl.lpi_tx_auto);
		printf("\t%40s:\t0x%x\n", "LPI Timer Enable", lpi_ctrl.lpi_timer_en);
		printf("\t%40s:\t0x%x\n", "LPI Tx Clock Stop Enable", lpi_ctrl.lpi_tx_clk_stop_en);
		printf("\t%40s:\t0x%x\n", "1US TIC Counter", lpi_ctrl.ous_tic_time);
		printf("\t%40s:\t0x%x\n", "LPI TW Timer (in milliseconds)", lpi_ctrl.lpi_tw_time);
		printf("\t%40s:\t0x%x\n", "LPI LS Timer (in milliseconds)", lpi_ctrl.lpi_ls_time);
		printf("\t%40s:\t0x%x\n", "LPI Entry Timer (in milliseconds)", lpi_ctrl.lpi_entry_time);
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_lpi_stat_get", strlen("gpy2xx_gmacf_lpi_stat_get")) == 0) {
		struct lpi_status lpi_stat;
		memset(&lpi_stat, 0, sizeof(struct lpi_status));

		ret = gpy2xx_gmacf_lpi_stat_get(&phy, &lpi_stat);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full LPI status get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "LPI Interrupt Status", lpi_stat.lpi_int_status);
		printf("\t%40s:\t0x%x\n", "Tx LPI Entry hit", lpi_stat.tx_lpi_entry);
		printf("\t%40s:\t0x%x\n", "Tx LPI Exit hit", lpi_stat.tx_lpi_exit);
		printf("\t%40s:\t0x%x\n", "Rx LPI Entry hit", lpi_stat.rx_lpi_entry);
		printf("\t%40s:\t0x%x\n", "Rx LPI Exit hit", lpi_stat.rx_lpi_exit);
		printf("\t%40s:\t0x%x\n", "Tx LPI State", lpi_stat.tx_lpi_state);
		printf("\t%40s:\t0x%x\n", "Rx LPI State", lpi_stat.rx_lpi_state);
		printf("\t%40s:\t0x%x\n", "GMAC-Full's LPI state", lpi_stat.full_lpi_status);
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacl_lpi_stat_get", strlen("gpy2xx_gmacl_lpi_stat_get")) == 0) {
		struct lpi_status lpi_stat;
		memset(&lpi_stat, 0, sizeof(struct lpi_status));

		ret = gpy2xx_gmacl_lpi_stat_get(&phy, &lpi_stat);

		if (ret < 0) {
			printf("\nERROR: GMAC-Lite LPI status get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "LPI Interrupt Status", lpi_stat.lpi_int_status);
		printf("\t%40s:\t0x%x\n", "Tx LPI Entry hit", lpi_stat.tx_lpi_entry);
		printf("\t%40s:\t0x%x\n", "Tx LPI Exit hit", lpi_stat.tx_lpi_exit);
		printf("\t%40s:\t0x%x\n", "Rx LPI Entry hit", lpi_stat.rx_lpi_entry);
		printf("\t%40s:\t0x%x\n", "Rx LPI Exit hit", lpi_stat.rx_lpi_exit);
		printf("\t%40s:\t0x%x\n", "Tx LPI State", lpi_stat.tx_lpi_state);
		printf("\t%40s:\t0x%x\n", "Rx LPI State", lpi_stat.rx_lpi_state);
		printf("\t%40s:\t0x%x\n", "GMAC-Lite's LPI state", lpi_stat.lite_lpi_status);
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_lpi_imask_cfg", strlen("gpy2xx_gmacf_lpi_imask_cfg")) == 0) {
		int cnt = 0;
		struct lpi_int_mask lpi_imask;
		memset(&lpi_imask, 0, sizeof(struct lpi_int_mask));

		ret = gpy2xx_gmacf_lpi_imask_get(&phy, &lpi_imask);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full LPI interrupt mask get failed.\n");
			return -1;
		}

		cnt += scanParamArg(argc, argv, "rxUsc", sizeof(lpi_imask.rx_lpi_usc_im), &lpi_imask.rx_lpi_usc_im);
		cnt += scanParamArg(argc, argv, "rxTrc", sizeof(lpi_imask.rx_lpi_tc_im), &lpi_imask.rx_lpi_tc_im);
		cnt += scanParamArg(argc, argv, "txUsc", sizeof(lpi_imask.tx_lpi_usc_im), &lpi_imask.tx_lpi_usc_im);
		cnt += scanParamArg(argc, argv, "txTrc", sizeof(lpi_imask.tx_lpi_tc_im), &lpi_imask.tx_lpi_tc_im);

		ret = gpy2xx_gmacf_lpi_imask_cfg(&phy, &lpi_imask);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full LPI interrupt mask set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_lpi_imask_get", strlen("gpy2xx_gmacf_lpi_imask_get")) == 0) {
		struct lpi_int_mask lpi_imask;
		memset(&lpi_imask, 0, sizeof(struct lpi_int_mask));

		ret = gpy2xx_gmacf_lpi_imask_get(&phy, &lpi_imask);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full LPI interrupt mask get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "Rx LPI microsecond counter interrupt mask", lpi_imask.rx_lpi_usc_im);
		printf("\t%40s:\t0x%x\n", "Rx LPI transition counter interrupt mask", lpi_imask.rx_lpi_tc_im);
		printf("\t%40s:\t0x%x\n", "Tx LPI microsecond counter interrupt mask", lpi_imask.tx_lpi_usc_im);
		printf("\t%40s:\t0x%x\n", "Tx LPI transition counter interrupt mask", lpi_imask.tx_lpi_tc_im);
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_lpi_istat_get", strlen("gpy2xx_gmacf_lpi_istat_get")) == 0) {
		struct lpi_int_status lpi_istat;
		memset(&lpi_istat, 0, sizeof(struct lpi_int_status));

		ret = gpy2xx_gmacf_lpi_istat_get(&phy, &lpi_istat);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full LPI interrupt status get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "Rx LPI microsecond counter interrupt status", lpi_istat.rx_lpi_usc_is);
		printf("\t%40s:\t0x%x\n", "Rx LPI transition counter interrupt status", lpi_istat.rx_lpi_tc_is);
		printf("\t%40s:\t0x%x\n", "Tx LPI microsecond counter interrupt status", lpi_istat.tx_lpi_usc_is);
		printf("\t%40s:\t0x%x\n", "Tx LPI transition counter interrupt status", lpi_istat.tx_lpi_tc_is);
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_lpi_count_get", strlen("gpy2xx_gmacf_lpi_count_get")) == 0) {
		struct lpi_counters lpi_cntr;
		memset(&lpi_cntr, 0, sizeof(struct lpi_counters));

		ret = gpy2xx_gmacf_lpi_count_get(&phy, &lpi_cntr);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full LPI counters get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "Tx LPI Microseconds Counter", lpi_cntr.tx_lpi_usc);
		printf("\t%40s:\t0x%x\n", "Tx LPI Transition Counters", lpi_cntr.tx_lpi_usc);
		printf("\t%40s:\t0x%x\n", "Rx LPI Microseconds Counter", lpi_cntr.rx_lpi_usc);
		printf("\t%40s:\t0x%x\n", "Rx LPI Transition Counter", lpi_cntr.rx_lpi_trc);
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_cfg", strlen("gpy2xx_gmacf_cfg")) == 0) {
		int cnt = 0;
		struct gmac_cfg mac_cfg;
		memset(&mac_cfg, 0, sizeof(struct gmac_cfg));

		ret = gpy2xx_gmacf_get(&phy, &mac_cfg);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full MAC cfg info get failed.\n");
			return -1;
		}

		cnt += scanParamArg(argc, argv, "reEn", sizeof(mac_cfg.re_en), &mac_cfg.re_en);
		cnt += scanParamArg(argc, argv, "teEn", sizeof(mac_cfg.te_en), &mac_cfg.te_en);
		cnt += scanParamArg(argc, argv, "preambLen", sizeof(mac_cfg.preamble_len), &mac_cfg.preamble_len);
		cnt += scanParamArg(argc, argv, "lbmEn", sizeof(mac_cfg.loopback_en), &mac_cfg.loopback_en);
		cnt += scanParamArg(argc, argv, "fdupEn", sizeof(mac_cfg.fulldup_en), &mac_cfg.fulldup_en);
		cnt += scanParamArg(argc, argv, "macSpeed", sizeof(mac_cfg.mac_speed), &mac_cfg.mac_speed);
		cnt += scanParamArg(argc, argv, "rcWrite1", sizeof(mac_cfg.rc_write1_en), &mac_cfg.rc_write1_en);

		ret = gpy2xx_gmacf_cfg(&phy, &mac_cfg);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full MAC cfg info set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_get", strlen("gpy2xx_gmacf_get")) == 0) {
		struct gmac_cfg mac_cfg;
		memset(&mac_cfg, 0, sizeof(struct gmac_cfg));

		ret = gpy2xx_gmacf_get(&phy, &mac_cfg);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full MAC cfg info get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "Receiver Enable", mac_cfg.re_en);
		printf("\t%40s:\t0x%x\n", "Transmitter Enable", mac_cfg.te_en);
		printf("\t%40s:\t0x%x\n", "Preamble Length in Bytes", mac_cfg.preamble_len);
		printf("\t%40s:\t0x%x\n", "Loopback Enable", mac_cfg.loopback_en);
		printf("\t%40s:\t0x%x\n", "Full-duplex Enable", mac_cfg.fulldup_en);
		printf("\t%40s:\t0x%x\n", "MAC speed", mac_cfg.mac_speed);
		printf("\t%40s:\t0x%x\n", "Register Clear on Write 1 Enable", mac_cfg.rc_write1_en);
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacl_cfg", strlen("gpy2xx_gmacl_cfg")) == 0) {
		int cnt = 0;
		struct gmac_cfg mac_cfg;
		memset(&mac_cfg, 0, sizeof(struct gmac_cfg));

		ret = gpy2xx_gmacl_get(&phy, &mac_cfg);

		if (ret < 0) {
			printf("\nERROR: GMAC-Lite MAC cfg info get failed.\n");
			return -1;
		}

		cnt += scanParamArg(argc, argv, "reEn", sizeof(mac_cfg.re_en), &mac_cfg.re_en);
		cnt += scanParamArg(argc, argv, "teEn", sizeof(mac_cfg.te_en), &mac_cfg.te_en);
		cnt += scanParamArg(argc, argv, "preambLen", sizeof(mac_cfg.preamble_len), &mac_cfg.preamble_len);
		cnt += scanParamArg(argc, argv, "lbmEn", sizeof(mac_cfg.loopback_en), &mac_cfg.loopback_en);
		cnt += scanParamArg(argc, argv, "fdupEn", sizeof(mac_cfg.fulldup_en), &mac_cfg.fulldup_en);
		cnt += scanParamArg(argc, argv, "macSpeed", sizeof(mac_cfg.mac_speed), &mac_cfg.mac_speed);
		cnt += scanParamArg(argc, argv, "rcWrite1", sizeof(mac_cfg.rc_write1_en), &mac_cfg.rc_write1_en);

		ret = gpy2xx_gmacl_cfg(&phy, &mac_cfg);

		if (ret < 0) {
			printf("\nERROR: GMAC-Lite MAC cfg info set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacl_get", strlen("gpy2xx_gmacl_get")) == 0) {
		struct gmac_cfg mac_cfg;
		memset(&mac_cfg, 0, sizeof(struct gmac_cfg));

		ret = gpy2xx_gmacl_get(&phy, &mac_cfg);

		if (ret < 0) {
			printf("\nERROR: GMAC-Lite MAC cfg info get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "Receiver Enable", mac_cfg.re_en);
		printf("\t%40s:\t0x%x\n", "Transmitter Enable", mac_cfg.te_en);
		printf("\t%40s:\t0x%x\n", "Preamble Length in Bytes", mac_cfg.preamble_len);
		printf("\t%40s:\t0x%x\n", "Loopback Enable", mac_cfg.loopback_en);
		printf("\t%40s:\t0x%x\n", "Full-duplex Enable", mac_cfg.fulldup_en);
		printf("\t%40s:\t0x%x\n", "MAC speed", mac_cfg.mac_speed);
		printf("\t%40s:\t0x%x\n", "Register Clear on Write 1 Enable", mac_cfg.rc_write1_en);
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacl_pkt_cfg", strlen("gpy2xx_gmacl_pkt_cfg")) == 0) {
		int cnt = 0;
		struct gmac_pkt_cfg pkt_cfg;
		memset(&pkt_cfg, 0, sizeof(struct gmac_pkt_cfg));

		ret = gpy2xx_gmacl_pkt_get(&phy, &pkt_cfg);

		if (ret < 0) {
			printf("\nERROR: GMAC-Lite packet cfg info get failed.\n");
			return -1;
		}

		cnt += scanParamArg(argc, argv, "jumboEn", sizeof(pkt_cfg.jumbo_en), &pkt_cfg.jumbo_en);
		cnt += scanParamArg(argc, argv, "jabberDis", sizeof(pkt_cfg.tx_jabber_dis), &pkt_cfg.tx_jabber_dis);
		cnt += scanParamArg(argc, argv, "wdogDis", sizeof(pkt_cfg.watchdog_dis), &pkt_cfg.watchdog_dis);
		cnt += scanParamArg(argc, argv, "pcStrip", sizeof(pkt_cfg.pad_crc_strip_en), &pkt_cfg.pad_crc_strip_en);
		cnt += scanParamArg(argc, argv, "crcStrip", sizeof(pkt_cfg.crc_strip_en), &pkt_cfg.crc_strip_en);
		cnt += scanParamArg(argc, argv, "sup2kp", sizeof(pkt_cfg.support_2kp), &pkt_cfg.support_2kp);
		cnt += scanParamArg(argc, argv, "macIpg", sizeof(pkt_cfg.mac_ipg), &pkt_cfg.mac_ipg);

		ret = gpy2xx_gmacl_pkt_cfg(&phy, &pkt_cfg);

		if (ret < 0) {
			printf("\nERROR: GMAC-Lite packet cfg info set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacl_pkt_get", strlen("gpy2xx_gmacl_pkt_get")) == 0) {
		struct gmac_pkt_cfg pkt_cfg;
		memset(&pkt_cfg, 0, sizeof(struct gmac_pkt_cfg));

		ret = gpy2xx_gmacl_pkt_get(&phy, &pkt_cfg);

		if (ret < 0) {
			printf("\nERROR: GMAC-Lite packet cfg info get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "Jumbo Packet Enable", pkt_cfg.jumbo_en);
		printf("\t%40s:\t0x%x\n", "Jabber Disable", pkt_cfg.tx_jabber_dis);
		printf("\t%40s:\t0x%x\n", "Watchdog Disable", pkt_cfg.watchdog_dis);
		printf("\t%40s:\t0x%x\n", "Pad or CRC Stripping for Ether 'Length' packets", pkt_cfg.pad_crc_strip_en);
		printf("\t%40s:\t0x%x\n", "CRC Stripping for Ether 'Type' packets", pkt_cfg.crc_strip_en);
		printf("\t%40s:\t0x%x\n", "802.3as Support for 2K Packets", pkt_cfg.support_2kp);
		printf("\t%40s:\t0x%x\n", "Inter-Packet Gap in Bytes", (12 - pkt_cfg.mac_ipg));
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_pkt_filter_cfg", strlen("gpy2xx_gmacf_pkt_filter_cfg")) == 0) {
		int cnt = 0;
		struct gmac_pktflt_cfg flt_cfg;
		memset(&flt_cfg, 0, sizeof(struct gmac_pktflt_cfg));

		ret = gpy2xx_gmacf_pkt_filter_get(&phy, &flt_cfg);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full packet filter info get failed.\n");
			return -1;
		}

		cnt += scanParamArg(argc, argv, "prEn", sizeof(flt_cfg.promiscuous_en), &flt_cfg.promiscuous_en);
		cnt += scanParamArg(argc, argv, "passAmc", sizeof(flt_cfg.pass_all_mc), &flt_cfg.pass_all_mc);
		cnt += scanParamArg(argc, argv, "blockBc", sizeof(flt_cfg.block_bc), &flt_cfg.block_bc);
		cnt += scanParamArg(argc, argv, "passCtrl", sizeof(flt_cfg.pass_ctrl_pkts), &flt_cfg.pass_ctrl_pkts);
		cnt += scanParamArg(argc, argv, "safltrEn", sizeof(flt_cfg.sa_filter_en), &flt_cfg.sa_filter_en);
		cnt += scanParamArg(argc, argv, "rxAll", sizeof(flt_cfg.rx_all_en), &flt_cfg.rx_all_en);

		ret = gpy2xx_gmacf_pkt_filter_cfg(&phy, &flt_cfg);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full packet filter info set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_pkt_filter_get", strlen("gpy2xx_gmacf_pkt_filter_get")) == 0) {
		struct gmac_pktflt_cfg flt_cfg;
		memset(&flt_cfg, 0, sizeof(struct gmac_pktflt_cfg));

		ret = gpy2xx_gmacf_pkt_filter_get(&phy, &flt_cfg);

		if (ret < 0) {
			printf("\nERROR: GMAC-full packet filter info get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "Promiscuous Mode", flt_cfg.promiscuous_en);
		printf("\t%40s:\t0x%x\n", "Pass All Multicast", flt_cfg.pass_all_mc);
		printf("\t%40s:\t0x%x\n", "Block all BC incoming frames", flt_cfg.block_bc);
		printf("\t%40s:\t0x%x\n", "Pass Control Packets mode", flt_cfg.pass_ctrl_pkts);
		printf("\t%40s:\t0x%x\n", "Source Address Filter Enable", flt_cfg.sa_filter_en);
		printf("\t%40s:\t0x%x\n", "Receive All", flt_cfg.rx_all_en);
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacl_pkt_filter_cfg", strlen("gpy2xx_gmacl_pkt_filter_cfg")) == 0) {
		int cnt = 0;
		struct gmac_pktflt_cfg flt_cfg;
		memset(&flt_cfg, 0, sizeof(struct gmac_pktflt_cfg));

		ret = gpy2xx_gmacl_pkt_filter_get(&phy, &flt_cfg);

		if (ret < 0) {
			printf("\nERROR: GMAC-Lite packet filter info get failed.\n");
			return -1;
		}

		cnt += scanParamArg(argc, argv, "prEn", sizeof(flt_cfg.promiscuous_en), &flt_cfg.promiscuous_en);
		cnt += scanParamArg(argc, argv, "passAmc", sizeof(flt_cfg.pass_all_mc), &flt_cfg.pass_all_mc);
		cnt += scanParamArg(argc, argv, "blockBc", sizeof(flt_cfg.block_bc), &flt_cfg.block_bc);
		cnt += scanParamArg(argc, argv, "passCtrl", sizeof(flt_cfg.pass_ctrl_pkts), &flt_cfg.pass_ctrl_pkts);
		cnt += scanParamArg(argc, argv, "safltrEn", sizeof(flt_cfg.sa_filter_en), &flt_cfg.sa_filter_en);
		cnt += scanParamArg(argc, argv, "rxAll", sizeof(flt_cfg.rx_all_en), &flt_cfg.rx_all_en);

		ret = gpy2xx_gmacl_pkt_filter_cfg(&phy, &flt_cfg);

		if (ret < 0) {
			printf("\nERROR: GMAC-Lite packet filter info set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacl_pkt_filter_get", strlen("gpy2xx_gmacl_pkt_filter_get")) == 0) {
		struct gmac_pktflt_cfg flt_cfg;
		memset(&flt_cfg, 0, sizeof(struct gmac_pktflt_cfg));

		ret = gpy2xx_gmacl_pkt_filter_get(&phy, &flt_cfg);

		if (ret < 0) {
			printf("\nERROR: GMAC-Lite packet filter info get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "Promiscuous Mode", flt_cfg.promiscuous_en);
		printf("\t%40s:\t0x%x\n", "Pass All Multicast", flt_cfg.pass_all_mc);
		printf("\t%40s:\t0x%x\n", "Block all BC incoming frames", flt_cfg.block_bc);
		printf("\t%40s:\t0x%x\n", "Pass Control Packets mode", flt_cfg.pass_ctrl_pkts);
		printf("\t%40s:\t0x%x\n", "Source Address Filter Enable", flt_cfg.sa_filter_en);
		printf("\t%40s:\t0x%x\n", "Receive All", flt_cfg.rx_all_en);
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_debug_info_get", strlen("gpy2xx_gmacf_debug_info_get")) == 0) {
		struct gmac_debug_info dbg_info;
		memset(&dbg_info, 0, sizeof(struct gmac_debug_info));

		ret = gpy2xx_gmacf_debug_info_get(&phy, &dbg_info);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full debug info get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "Synopsys-defined Version", dbg_info.synop_ver);
		printf("\t%40s:\t0x%x\n", "User-defined Version", dbg_info.user_ver);
		printf("\t%40s:\t0x%x\n", "Tx Jabber Timeout", dbg_info.tx_jabber_to);
		printf("\t%40s:\t0x%x\n", "Rx Watchdog Timeout", dbg_info.rx_wdog_to);
		printf("\t%40s:\t0x%x\n", "GMII or MII Receive Protocol Engine Status", dbg_info.gmii_rpe_stat);
		printf("\t%40s:\t0x%x\n", "Receive Packet Controller FIFO Status", dbg_info.rpc_fifo_stat);
		printf("\t%40s:\t0x%x\n", "GMII or MII Transmit Protocol Engine Status", dbg_info.gmii_tpe_stat);
		printf("\t%40s:\t0x%x\n", "Transmit Packet Controller FIFO Status", dbg_info.tpc_fifo_stat);
#endif
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_pkt_cfg", strlen("gpy2xx_gmacf_pkt_cfg")) == 0) {
		int cnt = 0;
		struct gmac_pkt_cfg pkt_cfg;
		memset(&pkt_cfg, 0, sizeof(struct gmac_pkt_cfg));

		ret = gpy2xx_gmacf_pkt_get(&phy, &pkt_cfg);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full packet cfg info get failed.\n");
			return -1;
		}

		cnt += scanParamArg(argc, argv, "jumboEn", sizeof(pkt_cfg.jumbo_en), &pkt_cfg.jumbo_en);
		cnt += scanParamArg(argc, argv, "jabberDis", sizeof(pkt_cfg.tx_jabber_dis), &pkt_cfg.tx_jabber_dis);
		cnt += scanParamArg(argc, argv, "wdogDis", sizeof(pkt_cfg.watchdog_dis), &pkt_cfg.watchdog_dis);
		cnt += scanParamArg(argc, argv, "pcStrip", sizeof(pkt_cfg.pad_crc_strip_en), &pkt_cfg.pad_crc_strip_en);
		cnt += scanParamArg(argc, argv, "crcStrip", sizeof(pkt_cfg.crc_strip_en), &pkt_cfg.crc_strip_en);
		cnt += scanParamArg(argc, argv, "sup2kp", sizeof(pkt_cfg.support_2kp), &pkt_cfg.support_2kp);
		cnt += scanParamArg(argc, argv, "macIpg", sizeof(pkt_cfg.mac_ipg), &pkt_cfg.mac_ipg);

		ret = gpy2xx_gmacf_pkt_cfg(&phy, &pkt_cfg);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full packet cfg info set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_pkt_get", strlen("gpy2xx_gmacf_pkt_get")) == 0) {
		struct gmac_pkt_cfg pkt_cfg;
		memset(&pkt_cfg, 0, sizeof(struct gmac_pkt_cfg));

		ret = gpy2xx_gmacf_pkt_get(&phy, &pkt_cfg);

		if (ret < 0) {
			printf("\nERROR: GMAC-Fullpacket cfg info get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "Jumbo Packet Enable", pkt_cfg.jumbo_en);
		printf("\t%40s:\t0x%x\n", "Jabber Disable", pkt_cfg.tx_jabber_dis);
		printf("\t%40s:\t0x%x\n", "Watchdog Disable", pkt_cfg.watchdog_dis);
		printf("\t%40s:\t0x%x\n", "Pad or CRC Stripping for Ether 'Length' packets", pkt_cfg.pad_crc_strip_en);
		printf("\t%40s:\t0x%x\n", "CRC Stripping for Ether 'Type' packets", pkt_cfg.crc_strip_en);
		printf("\t%40s:\t0x%x\n", "802.3as Support for 2K Packets", pkt_cfg.support_2kp);
		printf("\t%40s:\t0x%x\n", "Inter-Packet Gap in Bytes", pkt_cfg.mac_ipg);
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_count_ctrl_cfg", strlen("gpy2xx_gmacf_count_ctrl_cfg")) == 0) {
		int cnt = 0;
		struct gmacf_counter_ctrl cnt_ctrl;
		memset(&cnt_ctrl, 0, sizeof(struct gmacf_counter_ctrl));

		ret = gpy2xx_gmacf_count_ctrl_get(&phy, &cnt_ctrl);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full counters control info get failed.\n");
			return -1;
		}

		cnt += scanParamArg(argc, argv, "rstCnt", sizeof(cnt_ctrl.reset_counters), &cnt_ctrl.reset_counters);
		cnt += scanParamArg(argc, argv, "stopRoll", sizeof(cnt_ctrl.stop_rollover), &cnt_ctrl.stop_rollover);
		cnt += scanParamArg(argc, argv, "conRead", sizeof(cnt_ctrl.clear_on_read), &cnt_ctrl.clear_on_read);
		cnt += scanParamArg(argc, argv, "frzCnt", sizeof(cnt_ctrl.freeze_counters), &cnt_ctrl.freeze_counters);
		cnt += scanParamArg(argc, argv, "pstCnt", sizeof(cnt_ctrl.preset_counters), &cnt_ctrl.preset_counters);
		cnt += scanParamArg(argc, argv, "psthFull", sizeof(cnt_ctrl.preset_half_full), &cnt_ctrl.preset_half_full);
		cnt += scanParamArg(argc, argv, "cntbcDrop", sizeof(cnt_ctrl.count_dropped_bc), &cnt_ctrl.count_dropped_bc);

		ret = gpy2xx_gmacf_count_ctrl_cfg(&phy, &cnt_ctrl);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full counters control info set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_count_ctrl_get", strlen("gpy2xx_gmacf_count_ctrl_get")) == 0) {
		struct gmacf_counter_ctrl cnt_ctrl;
		memset(&cnt_ctrl, 0, sizeof(struct gmacf_counter_ctrl));

		ret = gpy2xx_gmacf_count_ctrl_get(&phy, &cnt_ctrl);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full counters control info get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "Counters Reset", cnt_ctrl.reset_counters);
		printf("\t%40s:\t0x%x\n", "Counter Stop Rollover", cnt_ctrl.stop_rollover);
		printf("\t%40s:\t0x%x\n", "Reset on Read", cnt_ctrl.clear_on_read);
		printf("\t%40s:\t0x%x\n", "Counters Freeze", cnt_ctrl.freeze_counters);
		printf("\t%40s:\t0x%x\n", "Counters Preset", cnt_ctrl.preset_counters);
		printf("\t%40s:\t0x%x\n", "Full-Half Preset", cnt_ctrl.preset_half_full);
		printf("\t%40s:\t0x%x\n", "Update Counters for Dropped BC Packets", cnt_ctrl.count_dropped_bc);
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_count_get", strlen("gpy2xx_gmacf_count_get")) == 0) {
		struct gmacf_counters full_ctrs;
		memset(&full_ctrs, 0, sizeof(struct gmacf_counters));

		ret = gpy2xx_gmacf_count_get(&phy, &full_ctrs);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full counters get failed.\n");
			return -1;
		}

		printf("\nTx Counters:\n----------------\n");
		printf("\t%40s:\t0x%x\n", "Tx Good & Bad frame count", full_ctrs.tx_good_bad);
		printf("\t%40s:\t0x%x\n", "Tx Underflow error frame count", full_ctrs.tx_uflow_err);
		printf("\t%40s:\t0x%x\n", "Tx Good frame count", full_ctrs.tx_good);
		printf("\t%40s:\t0x%x\n", "Tx Oversize Good frame count", full_ctrs.tx_osize_good);
		printf("\nRx Counters:\n----------------\n");
		printf("\t%40s:\t0x%x\n", "Rx Good & Bad frame count", full_ctrs.rx_good_bad);
		printf("\t%40s:\t0x%x\n", "Rx CRC error frame count", full_ctrs.rx_crc_err);
		printf("\t%40s:\t0x%x\n", "Rx Undesize Good frame count", full_ctrs.rx_usize_good);
		printf("\t%40s:\t0x%x\n", "Rx Oversize Good frame count", full_ctrs.rx_osize_good);
#if defined(EN_GMAC_DEBUG_ACCESS) && EN_GMAC_DEBUG_ACCESS
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_count_imask_cfg", strlen("gpy2xx_gmacf_count_imask_cfg")) == 0) {
		int cnt = 0;
		struct counter_imask cnt_imask;
		memset(&cnt_imask, 0, sizeof(struct counter_imask));

		ret = gpy2xx_gmacf_count_imask_get(&phy, &cnt_imask);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full counters interrupt mask get failed.\n");
			return -1;
		}

		cnt += scanParamArg(argc, argv, "txGb", sizeof(cnt_imask.tx_good_bad_im), &cnt_imask.tx_good_bad_im);
		cnt += scanParamArg(argc, argv, "txUflow", sizeof(cnt_imask.tx_uflow_err_im), &cnt_imask.tx_uflow_err_im);
		cnt += scanParamArg(argc, argv, "txGood", sizeof(cnt_imask.tx_good_im), &cnt_imask.tx_good_im);
		cnt += scanParamArg(argc, argv, "txOsize", sizeof(cnt_imask.tx_osize_good_im), &cnt_imask.tx_osize_good_im);

		cnt += scanParamArg(argc, argv, "rxGb", sizeof(cnt_imask.rx_good_bad_im), &cnt_imask.rx_good_bad_im);
		cnt += scanParamArg(argc, argv, "rxCrc", sizeof(cnt_imask.rx_crc_err_im), &cnt_imask.rx_crc_err_im);
		cnt += scanParamArg(argc, argv, "rxUsize", sizeof(cnt_imask.rx_usize_good_im), &cnt_imask.rx_usize_good_im);
		cnt += scanParamArg(argc, argv, "rxOsize", sizeof(cnt_imask.rx_osize_good_im), &cnt_imask.rx_osize_good_im);

		ret = gpy2xx_gmacf_count_imask_cfg(&phy, &cnt_imask);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full counters interrupt mask set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_count_imask_get", strlen("gpy2xx_gmacf_count_imask_get")) == 0) {
		struct counter_imask cnt_imask;
		memset(&cnt_imask, 0, sizeof(struct counter_imask));

		ret = gpy2xx_gmacf_count_imask_get(&phy, &cnt_imask);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full counters interrupt mask get failed.\n");
			return -1;
		}

		printf("\nTx counters interrupt mask:\n----------------\n");
		printf("\t%40s:\t0x%x\n", "Tx Good & Bad frame imask", cnt_imask.tx_good_bad_im);
		printf("\t%40s:\t0x%x\n", "Tx Underflow error frame imask", cnt_imask.tx_uflow_err_im);
		printf("\t%40s:\t0x%x\n", "Tx Good frame imask", cnt_imask.tx_good_im);
		printf("\t%40s:\t0x%x\n", "Tx Oversize Good frame imask", cnt_imask.tx_osize_good_im);
		printf("\nRx counters interrupt mask:\n----------------\n");
		printf("\t%40s:\t0x%x\n", "Rx Good & Bad frame imask", cnt_imask.rx_good_bad_im);
		printf("\t%40s:\t0x%x\n", "Rx CRC error frame imask", cnt_imask.rx_crc_err_im);
		printf("\t%40s:\t0x%x\n", "Rx Undesize Good frame imask", cnt_imask.rx_usize_good_im);
		printf("\t%40s:\t0x%x\n", "Rx Oversize Good frame imask", cnt_imask.rx_osize_good_im);
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacf_count_istat_get", strlen("gpy2xx_gmacf_count_istat_get")) == 0) {
		struct counter_istat cnt_istat;
		memset(&cnt_istat, 0, sizeof(struct counter_istat));

		ret = gpy2xx_gmacf_count_istat_get(&phy, &cnt_istat);

		if (ret < 0) {
			printf("\nERROR: GMAC-Full counters interrupt status get failed.\n");
			return -1;
		}

		printf("\nTx counters interrupt status:\n----------------\n");
		printf("\t%40s:\t0x%x\n", "Tx Good & Bad frame istatus", cnt_istat.tx_good_bad_is);
		printf("\t%40s:\t0x%x\n", "Tx Underflow error frame istatus", cnt_istat.tx_uflow_err_is);
		printf("\t%40s:\t0x%x\n", "Tx Good frame istatus", cnt_istat.tx_good_is);
		printf("\t%40s:\t0x%x\n", "Tx Oversize Good frame istatus", cnt_istat.tx_osize_good_is);
		printf("\nRx counters interrupt status:\n----------------\n");
		printf("\t%40s:\t0x%x\n", "Rx Good & Bad frame istatus", cnt_istat.rx_good_bad_is);
		printf("\t%40s:\t0x%x\n", "Rx CRC error frame istatus", cnt_istat.rx_crc_err_is);
		printf("\t%40s:\t0x%x\n", "Rx Undesize Good frame istatus", cnt_istat.rx_usize_good_is);
		printf("\t%40s:\t0x%x\n", "Rx Oversize Good frame istatus", cnt_istat.rx_osize_good_is);
#endif
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacx_bm_cfg", strlen("gpy2xx_gmacx_bm_cfg")) == 0) {
		int cnt = 0;
		struct bm_cfg bmcfg;
		memset(&bmcfg, 0, sizeof(struct bm_cfg));

		ret = gpy2xx_gmacx_bm_get(&phy, &bmcfg);

		if (ret < 0) {
			printf("\nERROR: PM's Buffer info get failed.\n");
			return -1;
		}

		cnt += scanParamArg(argc, argv, "sb0Start", sizeof(bmcfg.sb0_start), &bmcfg.sb0_start);
		cnt += scanParamArg(argc, argv, "sb0End", sizeof(bmcfg.sb0_end), &bmcfg.sb0_end);
		cnt += scanParamArg(argc, argv, "sb0Pthresh", sizeof(bmcfg.sb0_pkt_thresh), &bmcfg.sb0_pkt_thresh);
		cnt += scanParamArg(argc, argv, "sb1Start", sizeof(bmcfg.sb1_start), &bmcfg.sb1_start);
		cnt += scanParamArg(argc, argv, "sb1End", sizeof(bmcfg.sb1_end), &bmcfg.sb1_end);
		cnt += scanParamArg(argc, argv, "sb1Pthresh", sizeof(bmcfg.sb1_pkt_thresh), &bmcfg.sb1_pkt_thresh);

		ret = gpy2xx_gmacx_bm_cfg(&phy, &bmcfg);

		if (ret < 0) {
			printf("\nERROR: PM's Buffer info set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacx_bm_get", strlen("gpy2xx_gmacx_bm_get")) == 0) {
		struct bm_cfg bmcfg;
		memset(&bmcfg, 0, sizeof(struct bm_cfg));

		ret = gpy2xx_gmacx_bm_get(&phy, &bmcfg);

		if (ret < 0) {
			printf("\nERROR: PM's Buffer info get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "SB 0 Start location", bmcfg.sb0_start);
		printf("\t%40s:\t0x%x\n", "SB 0 End location", bmcfg.sb0_end);
		printf("\t%40s:\t0x%x\n", "SB 0 Packet Threshold to start the dequeue", bmcfg.sb0_pkt_thresh);
		printf("\t%40s:\t0x%x\n", "SB 1 Start location", bmcfg.sb1_start);
		printf("\t%40s:\t0x%x\n", "SB 1 End location", bmcfg.sb1_end);
		printf("\t%40s:\t0x%x\n", "SB 1 Packet Threshold to start the dequeue", bmcfg.sb1_pkt_thresh);
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacx_bm_status_get", strlen("gpy2xx_gmacx_bm_status_get")) == 0) {
		struct pm_bm_status bm_stat;
		memset(&bm_stat, 0, sizeof(struct pm_bm_status));

		ret = gpy2xx_gmacx_bm_status_get(&phy, &bm_stat);

		if (ret < 0) {
			printf("\nERROR: PM's Buffer status get failed.\n");
			return -1;
		}

		printf("\nSB-0 Buffer Status:\n----------------\n");
		printf("\t%40s:\t0x%x\n", "SB 0 Overflow Indicator", bm_stat.sb0_ov);
		printf("\t%40s:\t0x%x\n", "SB 0 Tx Register Overflow Indicator", bm_stat.sb0_tx_reg_ov);
		printf("\t%40s:\t0x%x\n", "SB 0 Rx Register Overflow Indicator", bm_stat.sb0_rx_reg_ov);
		printf("\t%40s:\t0x%x\n", "SB 0 Retry Indicator", bm_stat.sb0_retry);
		printf("\t%40s:\t0x%x\n", "SB 0 Sent Indicator", bm_stat.sb0_sent);
		printf("\t%40s:\t0x%x\n", "SB 0 Complete Indicator", bm_stat.sb0_complete);

		printf("\nSB-1 Buffer Status:\n----------------\n");
		printf("\t%40s:\t0x%x\n", "SB 1 Overflow Indicator", bm_stat.sb1_ov);
		printf("\t%40s:\t0x%x\n", "SB 1 Tx Register Overflow Indicator", bm_stat.sb1_tx_reg_ov);
		printf("\t%40s:\t0x%x\n", "SB 1 Rx Register Overflow Indicator", bm_stat.sb1_rx_reg_ov);
		printf("\t%40s:\t0x%x\n", "SB 1 Retry Indicator", bm_stat.sb1_retry);
		printf("\t%40s:\t0x%x\n", "SB 1 Sent Indicator", bm_stat.sb1_sent);
		printf("\t%40s:\t0x%x\n", "SB 1 Complete Indicator", bm_stat.sb1_complete);
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacl_pause_cfg", strlen("gpy2xx_gmacl_pause_cfg")) == 0) {
		int cnt = 0;
		struct pause_cfg pcfg;
		memset(&pcfg, 0, sizeof(struct pause_cfg));

		ret = gpy2xx_gmacl_pause_get(&phy, &pcfg);

		if (ret < 0) {
			printf("\nERROR: PM's Buffer info get failed.\n");
			return -1;
		}

		cnt += scanParamArg(argc, argv, "fcBusy", sizeof(pcfg.flow_ctrl_busy), &pcfg.flow_ctrl_busy);
		cnt += scanParamArg(argc, argv, "gmaclFc", sizeof(pcfg.pm_gmacl_fc), &pcfg.pm_gmacl_fc);
		cnt += scanParamArg(argc, argv, "txfcEn", sizeof(pcfg.tx_flow_ctrl), &pcfg.tx_flow_ctrl);
		cnt += scanParamArg(argc, argv, "plThresh", sizeof(pcfg.pause_low_thresh), &pcfg.pause_low_thresh);
		cnt += scanParamArg(argc, argv, "zqPause", sizeof(pcfg.dis_zquanta_pause), &pcfg.dis_zquanta_pause);
		cnt += scanParamArg(argc, argv, "pauseTime", sizeof(pcfg.pause_time), &pcfg.pause_time);
		cnt += scanMAC_Arg(argc, argv, "pauseMac", pcfg.tx_fc_mac);
		cnt += scanParamArg(argc, argv, "paThresh", sizeof(pcfg.pause_assert_thresh), &pcfg.pause_assert_thresh);
		cnt += scanParamArg(argc, argv, "pdThresh", sizeof(pcfg.pause_deassert_thresh), &pcfg.pause_deassert_thresh);

		ret = gpy2xx_gmacl_pause_cfg(&phy, &pcfg);

		if (ret < 0) {
			printf("\nERROR: PM's Buffer info set failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_gmacl_pause_get", strlen("gpy2xx_gmacl_pause_get")) == 0) {
		struct pause_cfg pcfg;
		memset(&pcfg, 0, sizeof(struct pause_cfg));

		ret = gpy2xx_gmacl_pause_get(&phy, &pcfg);

		if (ret < 0) {
			printf("\nERROR: PM's Buffer info get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "Flow Control Busy", pcfg.flow_ctrl_busy);
		printf("\t%40s:\t0x%x\n", "PM's GMAC Flow Control Enable", pcfg.pm_gmacl_fc);
		printf("\t%40s:\t0x%x\n", "Tx Flow Control Enable", pcfg.tx_flow_ctrl);
		printf("\t%40s:\t0x%x\n", "Pause Low Threshold", pcfg.pause_low_thresh);
		printf("\t%40s:\t0x%x\n", "Disable Zero-Quanta Paus", pcfg.dis_zquanta_pause);
		printf("\t%40s:\t0x%x\n", "Pause Time", pcfg.pause_time);
		printf("\t%s:\t", "Pause frame's Src MAC to sent with");
		printMAC_Address(pcfg.tx_fc_mac);
		printf("\n");
		printf("\t%40s:\t0x%x\n", "Pause Assesrt Threshold", pcfg.pause_assert_thresh);
		printf("\t%40s:\t0x%x\n", "Pause De-Assesrt Threshold", pcfg.pause_deassert_thresh);
	} else if (xstrncasecmp(argv[2], "gpy2xx_pm_freq_tune", strlen("gpy2xx_pm_freq_tune")) == 0) {
		int cnt = 0;
		struct tune_freq freq;
		memset(&freq, 0, sizeof(struct tune_freq));

		cnt += scanParamArg(argc, argv, "ppm", sizeof(freq.ppm), &freq.ppm);

		ret = gpy2xx_pm_freq_tune(&phy, &freq);

		if (ret < 0) {
			printf("\nERROR: PM's frequency tuning failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_usxgmii_reach_cfg", strlen("gpy2xx_usxgmii_reach_cfg")) == 0) {
		int cnt = 0;
		struct gpy211_usxgmii_reach reach_cfg;
		memset(&reach_cfg, 0, sizeof(struct gpy211_usxgmii_reach));

		ret = gpy2xx_usxgmii_reach_get(&phy, &reach_cfg);

		if (ret < 0) {
			printf("\nERROR:Egr USXGMII reach get failed.\n");
			return -1;
		}

		if (scanParamArg(argc, argv, "trace_len", sizeof(reach_cfg.trace_len), &reach_cfg.trace_len) == 0)
			reach_cfg.trace_len = -1;

		cnt += scanParamArg(argc, argv, "tx_eq_main", sizeof(reach_cfg.tx_eq_main), &reach_cfg.tx_eq_main);
		cnt += scanParamArg(argc, argv, "tx_eq_pre", sizeof(reach_cfg.tx_eq_pre), &reach_cfg.tx_eq_pre);
		cnt += scanParamArg(argc, argv, "tx_eq_post", sizeof(reach_cfg.tx_eq_post), &reach_cfg.tx_eq_post);
		cnt += scanParamArg(argc, argv, "tx_vboost_en", sizeof(reach_cfg.tx_vboost_en), &reach_cfg.tx_vboost_en);
		cnt += scanParamArg(argc, argv, "tx_vboost_lvl", sizeof(reach_cfg.tx_vboost_lvl), &reach_cfg.tx_vboost_lvl);
		cnt += scanParamArg(argc, argv, "tx_iboost_lvl", sizeof(reach_cfg.tx_iboost_lvl), &reach_cfg.tx_iboost_lvl);
		cnt += scanParamArg(argc, argv, "rx_eq_att_lvl", sizeof(reach_cfg.rx_eq_att_lvl), &reach_cfg.rx_eq_att_lvl);
		cnt += scanParamArg(argc, argv, "rx_eq_vga1_gain", sizeof(reach_cfg.rx_eq_vga1_gain), &reach_cfg.rx_eq_vga1_gain);
		cnt += scanParamArg(argc, argv, "rx_eq_vga2_gain", sizeof(reach_cfg.rx_eq_vga2_gain), &reach_cfg.rx_eq_vga2_gain);
		cnt += scanParamArg(argc, argv, "rx_eq_ctle_boost", sizeof(reach_cfg.rx_eq_ctle_boost), &reach_cfg.rx_eq_ctle_boost);
		cnt += scanParamArg(argc, argv, "rx_eq_ctle_pole", sizeof(reach_cfg.rx_eq_ctle_pole), &reach_cfg.rx_eq_ctle_pole);
		cnt += scanParamArg(argc, argv, "rx_eq_dfe_tap1", sizeof(reach_cfg.rx_eq_dfe_tap1), &reach_cfg.rx_eq_dfe_tap1);
		cnt += scanParamArg(argc, argv, "rx_afe_adapt_en", sizeof(reach_cfg.rx_afe_adapt_en), &reach_cfg.rx_afe_adapt_en);
		cnt += scanParamArg(argc, argv, "rx_dfe_adapt_en", sizeof(reach_cfg.rx_dfe_adapt_en), &reach_cfg.rx_dfe_adapt_en);

		ret = gpy2xx_usxgmii_reach_cfg(&phy, &reach_cfg);

		if (ret < 0) {
			printf("\nERROR:Egr USXGMII reach configure failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_usxgmii_reach_get", strlen("gpy2xx_usxgmii_getch_get")) == 0) {
		struct gpy211_usxgmii_reach reach_cfg;
		memset(&reach_cfg, 0, sizeof(struct gpy211_usxgmii_reach));

		ret = gpy2xx_usxgmii_reach_get(&phy, &reach_cfg);

		if (ret < 0) {
			printf("\nERROR:Egr usxgmii reach get failed.\n");
			return -1;
		}

		printf("\t%40s:\t%s\n", "trace_len", (reach_cfg.trace_len) == 0 ? "SHORT" : ((reach_cfg.trace_len) == 1 ? "MEDIUM" : ((reach_cfg.trace_len) == 2 ? "LONG" : "CUSTOM")));
		printf("\t%40s:\t0x%x\n", "tx_eq_main", reach_cfg.tx_eq_main);
		printf("\t%40s:\t0x%x\n", "tx_eq_pre", reach_cfg.tx_eq_pre);
		printf("\t%40s:\t0x%x\n", "tx_eq_post", reach_cfg.tx_eq_post);
		printf("\t%40s:\t%s\n", "tx_vboost_en", (reach_cfg.tx_vboost_en == 0) ? "Disabled" : "Enabled");
		printf("\t%40s:\t0x%x\n", "tx_vboost_lvl", reach_cfg.tx_vboost_lvl);
		printf("\t%40s:\t0x%x\n", "tx_iboost_lvl", reach_cfg.tx_iboost_lvl);
		printf("\t%40s:\t0x%x\n", "rx_eq_att_lvl", reach_cfg.rx_eq_att_lvl);
		printf("\t%40s:\t0x%x\n", "rx_eq_vga1_gain", reach_cfg.rx_eq_vga1_gain);
		printf("\t%40s:\t0x%x\n", "rx_eq_vga2_gain", reach_cfg.rx_eq_vga2_gain);
		printf("\t%40s:\t0x%x\n", "rx_eq_ctle_boost", reach_cfg.rx_eq_ctle_boost);
		printf("\t%40s:\t0x%x\n", "rx_eq_ctle_pole", reach_cfg.rx_eq_ctle_pole);
		printf("\t%40s:\t0x%x\n", "rx_eq_dfe_tap1", reach_cfg.rx_eq_dfe_tap1);
		printf("\t%40s:\t%s\n", "rx_afe_adapt_en", (reach_cfg.rx_afe_adapt_en == 0) ? "Disabled" : "Enabled");
		printf("\t%40s:\t%s\n", "rx_dfe_adapt_en", (reach_cfg.rx_dfe_adapt_en == 0) ? "Disabled" : "Enabled");
	}  else if (xstrncasecmp(argv[2], "gpy2xx_usxgmii_aneg_sts", strlen("gpy2xx_usxgmii_aneg_sts")) == 0) {
		struct gpy211_usxgmii_aneg_status status;
		memset(&status, 0, sizeof(struct gpy211_usxgmii_aneg_status));

		ret = gpy2xx_usxgmii_aneg_sts(&phy, &status);

		if (ret < 0) {
			printf("\nERROR:Egr gpy2xx_usxgmii_aneg_sts failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "phy_addr", phy.phy_addr);
		printf("\t%40s:\t%s\n", "phy_side", status.phy_side ? "usxgmii_phy" : "usxgmii_mac");
		printf("\t%40s:\t%s\n", "force_mode", status.force_mode ? "Enabled" : "Disabled");
		printf("\t%40s:\t%s\n", "aneg_enable", status.aneg_enable ? "Enabled" : "Disabled");
		printf("\t%40s:\t%s\n", "aneg_complete", status.aneg_complete ? "Complete" : "Not Complete");
		printf("\t%40s:\t%s\n", "aneg_linksts", status.aneg_linksts ? "Link Up" : "Link Down");
		printf("\t%40s:\t%d\n", "aneg_speed", status.aneg_speed);
		printf("\t%40s:\t%s\n", "aneg_duplex", status.aneg_duplex ? "Full Duplex" : "Half Duplex");
		printf("\t%40s:\t%s\n", "mii_linksts", status.mii_linksts ? "Link Up" : "Link Down");
		printf("\t%40s:\t%d\n", "mii_speed", status.mii_speed);
		printf("\t%40s:\t%s\n", "mii_duplex", status.mii_duplex ? "Full Duplex" : "Half Duplex");

	}  else if (xstrncasecmp(argv[2], "gpy2xx_usxgmii_aneg_rst", strlen("gpy2xx_usxgmii_aneg_rst")) == 0) {
		ret = gpy2xx_usxgmii_aneg_rst(&phy);

		if (ret < 0) {
			printf("\nERROR:Egr gpy2xx_usxgmii_aneg_rst failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "Usxgmii Aneg configuration applied for phy_addr", phy.phy_addr);

	} else if (xstrncasecmp(argv[2], "gpy2xx_usxgmii_pcs_pma_linksts", strlen("gpy2xx_usxgmii_pcs_pma_linksts")) == 0) {
		struct gpy211_usxgmii_pcs_pma_linksts linkstatus;
		memset(&linkstatus, 0, sizeof(struct gpy211_usxgmii_pcs_pma_linksts));

		ret = gpy2xx_usxgmii_pcs_pma_linksts(&phy, &linkstatus);

		if (ret < 0) {
			printf("\nERROR:Egr gpy2xx_usxgmii_pcs_pma_linksts failed.\n");
			return -1;
		}

		printf("\t%40s:\t%s\n", "pma_latched_linksts", linkstatus.pma_latched_linksts ? "Link Up" : "Link Down");
		printf("\t%40s:\t%s\n", "pma_linksts", linkstatus.pma_linksts ? "Link Up" : "Link Down");
		printf("\t%40s:\t%s\n", "pcs_latched_linksts", linkstatus.pcs_latched_linksts ? "Link Up" : "Link Down");
		printf("\t%40s:\t%s\n", "pcs_linksts", linkstatus.pcs_linksts ? "Link Up" : "Link Down");

	}  else if (xstrncasecmp(argv[2], "gpy2xx_usxgmii_mii_linksts", strlen("gpy2xx_usxgmii_mii_linksts")) == 0) {
		struct gpy211_usxgmii_mii_linksts linkstatus;
		memset(&linkstatus, 0, sizeof(struct gpy211_usxgmii_mii_linksts));

		ret = gpy2xx_usxgmii_mii_linksts(&phy, &linkstatus);

		if (ret < 0) {
			printf("\nERROR:Egr gpy2xx_usxgmii_mii_linksts failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "phy_addr", phy.phy_addr);
		printf("\t%40s:\t%s\n", "latched_linksts", linkstatus.latched_linksts ? "Link Up" : "Link Down");
		printf("\t%40s:\t%s\n", "linksts", linkstatus.linksts ? "Link Up" : "Link Down");

	} else if (xstrncasecmp(argv[2], "gpy2xx_usxgmii_alignmentmarker_set", strlen("gpy2xx_usxgmii_alignmentmarker_set")) == 0) {
		int cnt = 0;
		u32 am = 0;

		cnt += scanParamArg(argc, argv, "usxgmii_am_count", sizeof(am), &am);
		ret = gpy2xx_usxgmii_alignmentmarker_set(&phy, am);

		if (ret < 0) {
			printf("\nERROR:Egr gpy2xx_usxgmii_alignmentmarker_set failed.\n");
			return -1;
		}

	} else if (xstrncasecmp(argv[2], "gpy2xx_usxgmii_alignmentmarker_get", strlen("gpy2xx_usxgmii_alignmentmarker_get")) == 0) {
		u32 am = 0;

		ret = gpy2xx_usxgmii_alignmentmarker_get(&phy, &am);

		if (ret < 0) {
			printf("\nERROR:Egr gpy2xx_usxgmii_alignmentmarker_get failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "usxgmii_am_count", am);

	} else if (xstrncasecmp(argv[2], "gpy2xx_usxgmii_vr_reset", strlen("gpy2xx_usxgmii_vr_reset")) == 0) {
		ret = gpy2xx_usxgmii_vr_reset(&phy);

		if (ret < 0) {
			printf("\nERROR:Egr gpy2xx_usxgmii_vr_reset failed.\n");
			return -1;
		}
	}  else if (xstrncasecmp(argv[2], "gpy2xx_usxgmii_vr_reset_sts", strlen("gpy2xx_usxgmii_vr_reset_sts")) == 0) {
		int success = 0;

		ret = gpy2xx_usxgmii_vr_reset_sts(&phy, &success);

		if (ret < 0) {
			printf("\nERROR:Egr gpy2xx_usxgmii_vr_reset_sts failed.\n");
			return -1;
		}

		printf("\t%40s:\t%s\n", "VR reset ", success ? "complete/sucessfull" : "not complete/unsucessfull");

	} else if (xstrncasecmp(argv[2], "gpy2xx_usxgmii_tx_bert_cfg", strlen("gpy2xx_usxgmii_tx_bert_cfg")) == 0) {
		int cnt = 0;
		struct gpy211_usxgmii_tx_bert_config bert_tx_config;
		memset(&bert_tx_config, 0, sizeof(struct gpy211_usxgmii_tx_bert_config));

		cnt += scanParamArg(argc, argv, "mode", sizeof(bert_tx_config.mode), &bert_tx_config.mode);
		cnt += scanParamArg(argc, argv, "nval_sel", sizeof(bert_tx_config.nval_sel), &bert_tx_config.nval_sel);

		ret = gpy2xx_usxgmii_tx_bert_cfg(&phy, &bert_tx_config);

		if (ret < 0) {
			printf("\nERROR:Egr gpy2xx_usxgmii_tx_bert_cfg failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_usxgmii_tx_bert_get", strlen("gpy2xx_usxgmii_tx_bert_get")) == 0) {
		struct gpy211_usxgmii_tx_bert_config bert_tx_config;
		memset(&bert_tx_config, 0, sizeof(struct gpy211_usxgmii_tx_bert_config));

		ret = gpy2xx_usxgmii_tx_bert_get(&phy, &bert_tx_config);

		if (ret < 0) {
			printf("\nERROR:Egr gpy2xx_usxgmii_tx_bert_get failed.\n");
			return -1;
		}

		switch (bert_tx_config.mode) {
		case BERT_SQUAREWAVE_TX:
			printf("\t%40s:\t%40s\n", "TX bert mode", "BERT_SQUAREWAVE_TX");

			if (bert_tx_config.nval_sel == 0)
				printf("\t%40s:\t%40s\n", "NVAL", "BERT_TX_NVAL_SEL_4");
			else if (bert_tx_config.nval_sel == 1)
				printf("\t%40s:\t%40s\n", "NVAL", "BERT_TX_NVAL_SEL_5");
			else if (bert_tx_config.nval_sel == 2)
				printf("\t%40s:\t%40s\n", "NVAL", "BERT_TX_NVAL_SEL_6");
			else if (bert_tx_config.nval_sel == 3)
				printf("\t%40s:\t%40s\n", "NVAL", "BERT_TX_NVAL_SEL_7");
			else if (bert_tx_config.nval_sel == 4)
				printf("\t%40s:\t%40s\n", "NVAL", "BERT_TX_NVAL_SEL_8");
			else if (bert_tx_config.nval_sel == 5)
				printf("\t%40s:\t%40s\n", "NVAL", "BERT_TX_NVAL_SEL_9");
			else if (bert_tx_config.nval_sel == 6)
				printf("\t%40s:\t%40s\n", "NVAL", "BERT_TX_NVAL_SEL_10");
			else if (bert_tx_config.nval_sel == 7)
				printf("\t%40s:\t%40s\n", "NVAL", "BERT_TX_NVAL_SEL_11");
			else
				printf("\t%40s:\t%40s\n", "NVAL", "Un-Supported NVAL");

			break;

		case BERT_PRBS31_TX:
			printf("\t%40s:\t%40s\n", "TX bert mode", "BERT_PRBS31_TX");
			break;

		case BERT_PRBS9_TX:
			printf("\t%40s:\t%40s\n", "TX bert mode", "BERT_PRBS9_TX");
			break;

		case BERT_TX_MODE_DISABLE:
			printf("\t%40s:\t%40s\n", "TX bert mode", "BERT_TX_MODE_DISABLE");
			break;

		case BERT_PSEUDO_RANDOM_TX:
			printf("ERROR: BERT_PSEUDO_RANDOM_TX mode not supported\n");
			break;

		default:
			printf("ERROR: Un-supported TX BERT mode\n");
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_usxgmii_rx_bert_cfg", strlen("gpy2xx_usxgmii_rx_bert_cfg")) == 0) {
		int cnt = 0;
		struct gpy211_usxgmii_rx_bert_config bert_rx_config;
		memset(&bert_rx_config, 0, sizeof(struct gpy211_usxgmii_rx_bert_config));

		cnt += scanParamArg(argc, argv, "mode", sizeof(bert_rx_config.mode), &bert_rx_config.mode);

		ret = gpy2xx_usxgmii_rx_bert_cfg(&phy, &bert_rx_config);

		if (ret < 0) {
			printf("\nERROR:Egr gpy2xx_usxgmii_rx_bert_cfg failed.\n");
			return -1;
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_usxgmii_rx_bert_get", strlen("gpy2xx_usxgmii_rx_bert_get")) == 0) {
		struct gpy211_usxgmii_rx_bert_config bert_rx_config;
		memset(&bert_rx_config, 0, sizeof(struct gpy211_usxgmii_rx_bert_config));

		ret = gpy2xx_usxgmii_rx_bert_get(&phy, &bert_rx_config);

		if (ret < 0) {
			printf("\nERROR:Egr gpy2xx_usxgmii_rx_bert_get failed.\n");
			return -1;
		}

		switch (bert_rx_config.mode) {
		case BERT_PRBS31_RX:
			printf("\t%40s:\t%40s\n", "RX bert mode", "BERT_PRBS31_RX");
			break;

		case BERT_PRBS9_RX:
			printf("\t%40s:\t%40s\n", "RX bert mode", "BERT_PRBS9_RX");
			break;

		case BERT_RX_MODE_DISABLE:
			printf("\t%40s:\t%40s\n", "RX bert mode", "BERT_RX_MODE_DISABLE");
			break;

		case BERT_PSEUDO_RANDOM_RX:
			printf("ERROR: BERT_PSEUDO_RANDOM_RX mode not supported\n");
			break;

		default:
			printf("ERROR: Un-supported RX BERT mode\n");
		}
	} else if (xstrncasecmp(argv[2], "gpy2xx_usxgmii_rx_bert_stat", strlen("gpy2xx_usxgmii_rx_bert_stat")) == 0) {
		struct gpy211_usxgmii_rx_bert_statistics rx_bert_statistics;
		memset(&rx_bert_statistics, 0, sizeof(struct gpy211_usxgmii_rx_bert_statistics));
		ret = gpy2xx_usxgmii_rx_bert_stat(&phy, &rx_bert_statistics);

		if (ret < 0) {
			printf("\nERROR:Egr gpy2xx_usxgmii_rx_bert_stat failed.\n");
			return -1;
		}

		printf("\t%40s:\t0x%x\n", "tp_error_count", rx_bert_statistics.tp_error_count);
		printf("\t%40s:\t0x%x\n", "error_block_count", rx_bert_statistics.error_block_count);
		printf("\t%40s:\t0x%x\n", "bit_error_rate_counter", rx_bert_statistics.bit_error_rate_counter);
		printf("\t%40s:\t0x%x\n", "latched_high_bit_err_rate", rx_bert_statistics.latched_high_bit_err_rate);
		printf("\t%40s:\t0x%x\n", "latched_block_lock", rx_bert_statistics.latched_block_lock);

	} else if (xstrncasecmp(argv[2], "gpy2xx_usxgmii_loopback_cfg", strlen("gpy2xx_usxgmii_loopback_cfg")) == 0) {
		int cnt = 0;
		enum gpy211_usxgmii_loopback_mode mode = USXGMII_LOOPBACK_DISABLE;

		cnt += scanParamArg(argc, argv, "mode", sizeof(mode), &mode);

		ret = gpy2xx_usxgmii_loopback_cfg(&phy, mode);

		if (ret < 0) {
			printf("\nERROR:Egr gpy2xx_usxgmii_loopback_cfg failed.\n");
			return -1;
		}
	}  else if (xstrncasecmp(argv[2], "gpy2xx_usxgmii_loopback_get", strlen("gpy2xx_usxgmii_loopback_get")) == 0) {
		enum gpy211_usxgmii_loopback_mode mode = USXGMII_LOOPBACK_INVALID;

		ret = gpy2xx_usxgmii_loopback_get(&phy, &mode);

		if (ret < 0) {
			printf("\nERROR:Egr gpy2xx_usxgmii_loopback_get failed.\n");
			return -1;
		}

		printf("\tmode:\t");

		switch (mode) {
		case USXGMII_LOOPBACK_DISABLE:
			printf("disabled\n");
			break;

		case USXGMII_LOOPBACK_TX2RX:
			printf("Tx to Rx Serial Loopback\n");
			break;

		case USXGMII_LOOPBACK_RX2TX:
			printf("Rx to Tx Parallel Loopback\n");
			break;

		default:
			printf("invalid configuration\n");
		}
	}else	{
		printf("Command Not Supported\n");
	}
	return ret;
}

static u32 rand_val(size_t size)
{
	if (size < 16)
		return rand() & ((1 << size) - 1);
	else if (size < 31)
		return ((rand() << 15) | rand()) & ((1 << size) - 1);
	else
		return (rand() << 30) | (rand() << 15) | rand();
}

static u64 rand_val64()
{
	u64 val = rand();

	val = (val << 15) | rand();
	val = (val << 15) | rand();
	val = (val << 15) | rand();
	return (val << 4) | (rand() * 3 / RAND_MAX);
}

static void rand_array(u8 *p, size_t size)
{
	size_t i;

	for (i = 0; i < size; i++)
		p[i] = rand() * 0xFF / RAND_MAX;
}

static void random_input(void)
{
	int ret;
	u32 val1, val2, val3;
	struct gpy211_link link = phy.link;
	struct gpy211_led_brlvl_cfg brightnesscfg;
	struct gpy211_led_cfg genifcfg;
	struct gpy211_phy_extin extint;
	struct timespec64 ts;
	struct gpy211_ptp_ctrl ptpcfg;
	struct gpy2xx_ts_ctrl tscfg;
	struct gpy2xx_pps_ctrl ppscfg;
	struct gpy211_synce syncecfg;
	struct gpy211_wolinfo wolinfo;
	struct gpy211_ads_ctrl ads;
	struct gpy211_sgmii sgmiicfg;
	struct gpy211_macsec msec_cfg;

	srand(time(NULL));

	val1 = rand_val(sizeof(u32) * 8);
	ret = gpy2xx_smdio_read(&phy, val1);
	printf("LOG: gpy2xx_smdio_read addr 0x%x returns %d\n", val1, ret);

	val1 = rand_val(sizeof(u32) * 8);
	val2 = rand_val(sizeof(u16) * 8);
	ret = gpy2xx_smdio_write(&phy, val1, val2);
	printf("LOG: gpy2xx_smdio_write addr 0x%x with 0x%x returns %d\n", val1, val2, ret);

	val1 = rand_val(sizeof(u32) * 8);
	val2 = rand_val(sizeof(u32) * 8);
	ret = gpy2xx_read_mmd(&phy, (int)val1, val2);
	printf("LOG: gpy2xx_read_mmd dev 0x%x addr 0x%x returns %d\n", val1, val2, ret);

	val1 = rand_val(sizeof(u32) * 8);
	val2 = rand_val(sizeof(u32) * 8);
	val3 = rand_val(sizeof(u16) * 8);
	ret = gpy2xx_write_mmd(&phy, (int)val1, val2, val3);
	printf("LOG: gpy2xx_write_mmd dev 0x%x addr 0x%x with 0x%x returns %d\n", val1, val2, val3, ret);

	val1 = rand_val(sizeof(u32) * 8);
	ret = gpy2xx_mbox_read16(&phy, val1, &val2);
	printf("LOG: gpy2xx_mbox_read16 addr 0x%x data 0x%x returns %d\n", val1, val2, ret);

	val1 = rand_val(sizeof(u32) * 8);
	val2 = rand_val(sizeof(u32) * 8);
	ret = gpy2xx_mbox_write16(&phy, val1, val2);
	printf("LOG: gpy2xx_mbox_write16 addr 0x%x with 0x%x returns %d\n", val1, val2, ret);

	val1 = rand_val(sizeof(u32) * 8);
	ret = gpy2xx_mbox_read32(&phy, val1, &val2);
	printf("LOG: gpy2xx_mbox_read32 addr 0x%x data 0x%x returns %d\n", val1, val2, ret);

	val1 = rand_val(sizeof(u32) * 8);
	val2 = rand_val(sizeof(u32) * 8);
	ret = gpy2xx_mbox_write32(&phy, val1, val2);
	printf("LOG: gpy2xx_mbox_write32 addr 0x%x with 0x%x returns %d\n", val1, val2, ret);

	phy.link.advertising = rand_val64();
	ret = gpy2xx_config_advert(&phy);
	printf("LOG: gpy2xx_config_advert returns %d\n", ret);
	phy.link = link;

	phy.link.speed = rand_val(sizeof(int) * 8);
	phy.link.duplex = rand_val(sizeof(int) * 8);
	phy.link.pause = rand_val(sizeof(int) * 8);
	phy.link.asym_pause = rand_val(sizeof(int) * 8);
	ret = gpy2xx_setup_forced(&phy);
	printf("LOG: gpy2xx_setup_forced returns %d\n", ret);
	phy.link = link;

	phy.link.autoneg = rand_val(1);
	phy.link.speed = rand_val64();
	phy.link.duplex = rand_val(sizeof(int) * 8);
	phy.link.pause = rand_val(sizeof(int) * 8);
	phy.link.asym_pause = rand_val(sizeof(int) * 8);
	phy.link.supported = rand_val64();
	phy.link.advertising = rand_val64();
	ret = gpy2xx_config_aneg(&phy);
	printf("LOG: gpy2xx_config_aneg returns %d\n", ret);
	phy.link = link;

	rand_array((u8 *)&brightnesscfg, sizeof(brightnesscfg));
	ret = gpy2xx_led_br_cfg(&phy, &brightnesscfg);
	printf("LOG: gpy2xx_led_br_cfg returns %d\n", ret);

	rand_array((u8 *)&genifcfg, sizeof(genifcfg));
	ret = gpy2xx_led_if_cfg(&phy, &genifcfg);
	printf("LOG: gpy2xx_led_if_cfg returns %d\n", ret);

	rand_array((u8 *)&extint, sizeof(extint));
	ret = gpy2xx_extin_mask(&phy, &extint);
	printf("LOG: gpy2xx_extin_mask returns %d\n", ret);

	val1 = rand_val(8);
	ret = gpy2xx_test_mode_cfg(&phy, val1);
	printf("LOG: gpy2xx_test_mode_cfg returns %d\n", ret);

	val1 = rand_val(8);
	ret = gpy2xx_loopback_cfg(&phy, val1);
	printf("LOG: gpy2xx_loopback_cfg returns %d\n", ret);

	val1 = rand_val(8);
	ret = gpy2xx_errcnt_cfg(&phy, val1);
	printf("LOG: gpy2xx_errcnt_cfg returns %d\n", ret);

	val1 = rand_val(sizeof(int) * 8);
	ret = gpy2xx_ptp_adjtime(&phy, val1);
	printf("LOG: gpy2xx_ptp_adjtime returns %d\n", ret);

	rand_array((u8 *)&ts, sizeof(ts));
	ret = gpy2xx_ptp_settime(&phy, &ts);
	printf("LOG: gpy2xx_ptp_settime returns %d\n", ret);

	rand_array((u8 *)&ptpcfg, sizeof(ptpcfg));
	ret = gpy2xx_ptp_enable(&phy, &ptpcfg);
	printf("LOG: gpy2xx_ptp_enable returns %d\n", ret);

	rand_array((u8 *)&tscfg, sizeof(tscfg));
	ret = gpy2xx_ptp_set_tsctrl(&phy, &tscfg);
	printf("LOG: gpy2xx_ptp_set_tsctrl returns %d\n", ret);

	rand_array((u8 *)&ppscfg, sizeof(ppscfg));
	ret = gpy2xx_ptp_set_ppsctrl(&phy, &ppscfg);
	printf("LOG: gpy2xx_ptp_set_ppsctrl returns %d\n", ret);

	gpy2xx_ptp_disable(&phy);

	rand_array((u8 *)&syncecfg, sizeof(syncecfg));
	ret = gpy2xx_synce_cfg(&phy, &syncecfg);
	printf("LOG: gpy2xx_synce_cfg returns %d\n", ret);

	rand_array((u8 *)&wolinfo, sizeof(wolinfo));
	ret = gpy2xx_wol_cfg(&phy, &wolinfo);
	printf("LOG: gpy2xx_wol_cfg returns %d\n", ret);

	rand_array((u8 *)&ads, sizeof(ads));
	ret = gpy2xx_ads_cfg(&phy, &ads);
	printf("LOG: gpy2xx_ads_cfg returns %d\n", ret);

	rand_array((u8 *)&sgmiicfg, sizeof(sgmiicfg));
	ret = gpy2xx_sgmii_config_aneg(&phy, &sgmiicfg);
	printf("LOG: gpy2xx_sgmii_config_aneg returns %d\n", ret);

	ret = gpy2xx_sgmii_opmode(&phy, rand_val(8));
	printf("LOG: gpy2xx_sgmii_opmode returns %d\n", ret);

	rand_array((u8 *)&msec_cfg, sizeof(msec_cfg));
	ret = gpy2xx_macsec_enable(&phy, &msec_cfg);
	printf("LOG: gpy2xx_macsec_enable returns %d\n", ret);

	gpy2xx_macsec_disable(&phy);

	memset(&msec_cfg, 0, sizeof(msec_cfg));
	ret = gpy2xx_macsec_enable(&phy, &msec_cfg);

	ret = gpy2xx_uninit(&phy);
	printf("LOG: GPY211 uninit returned %d\n", ret);
}

static void reset_rd_std_ctrl(void)
{
	int ret;

#if 0
	phy.phy_addr = 0x7;
        ret = gpy2xx_write(&phy, 0x0, 0xb040);

        if (ret < 0) {
                printf("\nERROR: MDIO write failed.\n");
        }

	phy.phy_addr = 0x6;
	ret = gpy2xx_write(&phy, 0x0, 0xb040);

	if (ret < 0) {
		printf("\nERROR: MDIO write failed.\n");
	}

        phy.phy_addr = 0x5;
        ret = gpy2xx_write(&phy, 0x0, 0xb040);

        if (ret < 0) {
                printf("\nERROR: MDIO write failed.\n");
        }

        phy.phy_addr = 0x4;
        ret = gpy2xx_write(&phy, 0x0, 0xb040);

        if (ret < 0) {
                printf("\nERROR: MDIO write failed.\n");
        }

	//udelay(1500u);

	phy.phy_addr = 0x4;
	ret = gpy2xx_read(&phy, 0x0);

	if (ret < 0) {
		printf("\nERROR: MDIO read failed.\n");
	} else
		printf("\nValue of STD_CTRL is %x\n", ret);

	ret = gpy2xx_read(&phy, 0x0);

        if (ret < 0) {
                printf("\nERROR: MDIO read failed.\n");
        } else
                printf("\nValue of STD_CTRL is %x\n", ret);
#endif
#if 0
        phy.phy_addr = 0x7;
        ret = gpy2xx_write_mmd(&phy, 0x1, 0x0, 0xa058);

        if (ret < 0) {
                printf("\nERROR: MMD write failed.\n");
        }

        phy.phy_addr = 0x6;
        ret = gpy2xx_write_mmd(&phy, 0x1, 0x0, 0xa058);

        if (ret < 0) {
                printf("\nERROR: MMD write failed.\n");
        }

        phy.phy_addr = 0x5;
        ret = gpy2xx_write_mmd(&phy, 0x1, 0x0, 0xa058);

        if (ret < 0) {
                printf("\nERROR: MMD write failed.\n");
        }

        phy.phy_addr = 0x4;
        ret = gpy2xx_write_mmd(&phy, 0x1, 0x0, 0xa058);

        if (ret < 0) {
                printf("\nERROR: MMD write failed.\n");
        }

        //udelay(1500u);

        phy.phy_addr = 0x4;
        ret = gpy2xx_read_mmd(&phy, 0x1, 0x0);

        if (ret < 0) {
                printf("\nERROR: MMD read failed.\n");
        } else
                printf("\nValue of PMA_CTRL1 is %x\n", ret);

        ret = gpy2xx_read_mmd(&phy, 0x1, 0x0);

        if (ret < 0) {
                printf("\nERROR: MMD read failed.\n");
        } else
                printf("\nValue of PMA_CTRL1 is %x\n", ret);
#endif
#if 1
       phy.phy_addr = 0x7;
        ret = gpy2xx_write_mmd(&phy, 0x3, 0x0, 0xa05c);

        if (ret < 0) {
                printf("\nERROR: MMD write failed.\n");
        }

        phy.phy_addr = 0x6;
        ret = gpy2xx_write_mmd(&phy, 0x3, 0x0, 0xa05c);

        if (ret < 0) {
                printf("\nERROR: MMD write failed.\n");
        }

        phy.phy_addr = 0x5;
        ret = gpy2xx_write_mmd(&phy, 0x3, 0x0, 0xa05c);

        if (ret < 0) {
                printf("\nERROR: MMD write failed.\n");
        }

        phy.phy_addr = 0x4;
        ret = gpy2xx_write_mmd(&phy, 0x3, 0x0, 0xa05c);

        if (ret < 0) {
                printf("\nERROR: MMD write failed.\n");
        }

        //udelay(1500u);

        phy.phy_addr = 0x4;
        ret = gpy2xx_read_mmd(&phy, 0x3, 0x0);

        if (ret < 0) {
                printf("\nERROR: MMD read failed.\n");
        } else
                printf("\nValue of PCS_CTRL1 is %x\n", ret);

        ret = gpy2xx_read_mmd(&phy, 0x3, 0x0);

        if (ret < 0) {
                printf("\nERROR: MMD read failed.\n");
        } else
                printf("\nValue of PCS_CTRL1 is %x\n", ret);
#endif
}

int cmdProcessor(int argc, char *argv[])
{
	if (xstrncasecmp(argv[2], "help", 4) == 0) {
		gpy211_menu(argc > 2 ? argv[3] : NULL);
	} else if (xstrncasecmp(argv[2], "msec", strlen("msec")) == 0) {
		gpy2xx_msec_main(argc, argv);
	} else if (xstrncasecmp(argv[2], "random", 6) == 0) {
		random_input();
	} else if (xstrncasecmp(argv[2], "resetrd", 7) == 0) {
		reset_rd_std_ctrl();
	} else {
		gpy211_main(argc, argv);
	}
	return 0;
}

int Search_in_File(char *fname, char *str) {
	FILE *fp;
	int line_num = 1;
	int find_result = 0,found =0,i,j,count;
	unsigned int num;
	char temp[512];
	char temp1[512];
	
	if((fp = fopen(fname, "r")) == NULL) {
		printf("\nSorry, config file not found.\n");
		exit(0);
	}

	while(fgets(temp, 512, fp) != NULL) {
		if((strstr(temp, str)) != NULL) {
			find_result =1;
			for(i=0,j=0,count=0;temp[i] != '\0';i++)
			{
				if(found)
				{
					temp1[j] = temp[i];
					j++;
				}
					
				if(temp[i] == ' '){
					count++;
					if(count == 2)
						found = 1;
				}
			}
			temp1[j] = '\0';
			break;
		}
		line_num++;
	}

	num = (int)strtol(temp1, NULL, 10); 
	if(find_result == 0) {
		printf("\nSorry, couldn't find a match.\n");
		return(0);
	}
	
	if(fp) {
		fclose(fp);
	}
   	return(num);
}

void Usage(char *filename) {
	printf("Usage: %s <file> <string>\n", filename);
	printf("%s version 1.0 \nCopyright(c) CodingUnit.com\n", filename);
}


int init(int phyId)
{
	int ret=0;
	const char *gsw_dev = "/dev/switch_api/0";
		
	mdio.xgmac_id = 2;
	mdio.fd = open(gsw_dev, O_RDONLY);
	phy.lock = NULL;
	phy.unlock = NULL;
	phy.lock_data = NULL;
	phy.mdiobus_read = gpy211_mdio_read; 
	phy.mdiobus_write = gpy211_mdio_write;
	phy.mdiobus_data = &mdio;
	phy.smdio_addr = 0;
	phy.phy_addr = phyId;
	phy.priv_data = NULL;
	phy.shared_data =NULL;
	phy.wol_supported =0x60;
	phy.ptp_clock = 0x1836E210ull; //406.25MHz
	phy.macsec_supported = 0;
	phy.nr_of_sas = 32;

	if (mdio.fd < 0) {
		printf("ERROR: Could not open switch dev node \"%s\"\n", gsw_dev);
		ret = -1;
	}

	ret = gpy2xx_init(&phy);
	if (ret < 0) {
		printf("api gpy2xx_init failed: %d\n", ret);
		return ret;
	}

	return ret;			   
}	

int main(int argc, char *argv[])
{
	int ret;
	int phyId = 0;

	if (xstrncasecmp(argv[1], "phyId", 5) != 0) {
		printf("ERROR: phyId missing\"%s\"\n");
		exit(0);
	}

	/* scan for device node index */
	scanParamArg(argc, argv, "phyId", 32, &phyId);
	ret = init(phyId);
	if (ret < 0)
		goto end;

	cmdProcessor(argc, argv);
end:

	close(mdio.fd);
	return ret;
}

