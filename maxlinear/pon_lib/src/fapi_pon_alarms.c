/******************************************************************************
 *
 *  Copyright (c) 2025 MaxLinear, Inc.
 *  Copyright (c) 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

#include "fapi_pon.h"
#include "fapi_pon_alarms.h"

#define DEFINE_ALARM(alarm_id, desc) {alarm_id, #alarm_id, desc}

static const struct alarm_type pon_alarm_level[] = {
	DEFINE_ALARM(PON_ALARM_STATIC_LOS, "Loss of signal"),
	DEFINE_ALARM(PON_ALARM_STATIC_LOF, "Loss of frame"),
	DEFINE_ALARM(PON_ALARM_STATIC_LODS,
		     "Loss of downstream synchronization"),
	DEFINE_ALARM(PON_ALARM_STATIC_SF, "Signal fail"),
	DEFINE_ALARM(PON_ALARM_STATIC_SD, "Signal degrade"),
	DEFINE_ALARM(PON_ALARM_STATIC_LCDG,
		     "Loss of GEM channel delineation"),
	DEFINE_ALARM(PON_ALARM_STATIC_TF, "Transmitter failure"),
	DEFINE_ALARM(PON_ALARM_STATIC_SUF, "Start-up failure"),
	DEFINE_ALARM(PON_ALARM_STATIC_MIS, "Link mismatch"),
	DEFINE_ALARM(PON_ALARM_STATIC_O7, "Emergency stop state entered"),
	DEFINE_ALARM(PON_ALARM_STATIC_LOL, "Loss of PMA lock"),
	DEFINE_ALARM(PON_ALARM_STATIC_CLK, "Clock alarm reported by SoC"),
	DEFINE_ALARM(PON_ALARM_STATIC_FSR, "Free spectral range mismatch"),
	DEFINE_ALARM(PON_ALARM_STATIC_CSP, "Channel spacing mismatch"),
	DEFINE_ALARM(PON_ALARM_STATIC_MSE,
		     "Maximum spectral excursion mismatch"),
	DEFINE_ALARM(PON_ALARM_STATIC_PEE, "Physical equipment error"),
	DEFINE_ALARM(PON_ALARM_STATIC_PCE,
		     "Persistent crossbar access error"),
	DEFINE_ALARM(PON_ALARM_STATIC_PSE, "Persistent SerDes error"),
	DEFINE_ALARM(PON_ALARM_STATIC_PLL_ERR,
		     "The firmware cannot access DPLL or PLL3 registers of the SoC. Firware-based SyncE processing is stopped."
		     ),
	DEFINE_ALARM(PON_ALARM_STATIC_DS_FEC_FAIL,
		     "The downstream FEC indication in the TC header does not match the target configuration."
		     ),
	DEFINE_ALARM(PON_ALARM_STATIC_TOD_INVALID,
		     "ToD information is invalid"
		     ),
	DEFINE_ALARM(PON_ALARM_STATIC_DS_LODS, "Indicates LODS"),
	DEFINE_ALARM(PON_ALARM_STATIC_DS_AES_OFL,
		     "Indicates AES-Engine-Overflow, output data of AES is corrupted (Old-Decryption, not present in new PONIP-CRYPT module)"
		     ),
	DEFINE_ALARM(PON_ALARM_STATIC_DS_ALLOCFIFO_ERR,
		     "Indicates ALLOC-FIFO Error (FIFO was not read out completely by FW/SW)"
		     ),
	DEFINE_ALARM(PON_ALARM_STATIC_DS_PLOAMFIFO_ERR,
		     "Indicates PLOAM-FIFO Error (FIFO was not read out completely by FW/SW)"
		     ),
	DEFINE_ALARM(PON_ALARM_STATIC_XGT_DEC,
		     "Indicates whether the decapsulation reported an interrupt"
		     ),
	DEFINE_ALARM(PON_ALARM_STATIC_SYSR_CFE,
		     "A clock fail of the PON clock is signaled by clock-source SerDes"
		     ),
	DEFINE_ALARM(PON_ALARM_STATIC_SYSR_CFIL,
		     "The period of the PON clock is too low"),
	DEFINE_ALARM(PON_ALARM_STATIC_SYSR_CFIH,
		     "The period of the PON clock is too high"),
	DEFINE_ALARM(PON_ALARM_STATIC_SYSR_CFE2,
		     "A clock fail of the PON clock is signaled by clock-source PLL"
		     ),
	DEFINE_ALARM(PON_ALARM_STATIC_SYSR_CFI2L,
		     "The period of the PON clock is too low"),
	DEFINE_ALARM(PON_ALARM_STATIC_SYSR_CFI2H,
		     "The period of the PON clock is too high"),
	DEFINE_ALARM(PON_ALARM_STATIC_PHYIF_TXSD,
		     "The value of tx_sd_i can be observed here"),
	DEFINE_ALARM(PON_ALARM_STATIC_EXT_XPCSLS,
		     "Link status line from the external XPCS module"),
	DEFINE_ALARM(PON_ALARM_STATIC_EXT_XPCSS,
		     "Interrupt line from the external XPCS module"),
	DEFINE_ALARM(PON_ALARM_STATIC_EXT_XTCGS,
		     "MAC level signaling of EEE/LPI status"),
	DEFINE_ALARM(PON_ALARM_STATIC_EXT_EQEMU,
		     "Exceptions occurred in the QoS Engine Emulator module"),
	DEFINE_ALARM(PON_ALARM_STATIC_EXT_TXFES,
		     "Same signal as TXFE just configured as level sensitive"),
	DEFINE_ALARM(PON_ALARM_STATIC_EXT_TXFFS,
		     "Same signal as TXFF just configured as level sensitive"),
	DEFINE_ALARM(PON_ALARM_STATIC_ERR_LOSS,
		     "Same signal as LOS just configured as level sensitive"),
	DEFINE_ALARM(PON_ALARM_STATIC_ERR_XGRCRS,
		     "Same signal as XGRCR just configured as level sensitive"),
	DEFINE_ALARM(PON_ALARM_STATIC_ERR_XBARS,
		     "Same signal as XBR just configured as level sensitive"),
	DEFINE_ALARM(PON_ALARM_STATIC_ERR_DYG,
		     "The dying_gasp input shows a power fail"),
	DEFINE_ALARM(PON_ALARM_STATIC_ERR_TXFS,
		     "Same signal as TXF just configured as level sensitive"),
	DEFINE_ALARM(PON_ALARM_STATIC_ERR_XGTCRS,
		     "Same signal as XGTCR just configured as level sensitive"),
};

static const struct alarm_type pon_alarm_edge[] = {
	DEFINE_ALARM(PON_ALARM_EDGE_DSWL_ERR,
		     "DWLCHID mismatch between the one selected by the transceiver and the one synchronized on."
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_MIC_ERRS_O12,
		     "Too many MIC errored PLOAMd messages received while in O1.2."
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_MIC_ERRS_O71,
		     "Too many MIC errored PLOAMd messages received while in O7.1."
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_MIC_ERRS_O82,
		     "Too many MIC errored PLOAMd messages received while in O8.2."
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_MIC_ERR,
		     "Too many MIC errored debug PLOAMd messages received while in any activation state."
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_UPM, "Unknown PLOAM message"),
	DEFINE_ALARM(PON_ALARM_EDGE_DACT, "Deactivate ONU message received"),
	DEFINE_ALARM(PON_ALARM_EDGE_RDI, "Remote defect indication"),
	DEFINE_ALARM(PON_ALARM_EDGE_ROG, "Rogue ONU detected"),
	DEFINE_ALARM(PON_ALARM_EDGE_PSC, "PLOAM state changed"),
	DEFINE_ALARM(PON_ALARM_EDGE_FWA, "Firmware Alarm"),
	DEFINE_ALARM(PON_ALARM_EDGE_OIKC, "OMCI Integrity Key has changed"),
	DEFINE_ALARM(PON_ALARM_EDGE_ASC,
		     "The mutual authentication status has changed."),
	DEFINE_ALARM(PON_ALARM_EDGE_CAPREL,
		     "Capture reload event has happened."),
	DEFINE_ALARM(PON_ALARM_EDGE_CAPDST,
		     "Capture downstream tick event has happened."),
	DEFINE_ALARM(PON_ALARM_EDGE_CAPEXT,
		     "Capture external event has happened."),
	DEFINE_ALARM(PON_ALARM_EDGE_EQDEL,
		     "The sum of the ONU response time value and the equalization delay has changed."
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_CPI_TO,
		     "Channel Profile Index timeout event."),
	DEFINE_ALARM(PON_ALARM_EDGE_FWI,
		     "Forced wake-up received while sleeping"),
	DEFINE_ALARM(PON_ALARM_EDGE_FWVERR,
		     "Firmware version error, the loaded firmware does not fit to the PON IP hardware version."
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_NG_ATWLPI,
		     "NG-PON2 message AdjustTXWavelangth parameter inconsistency."
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_FW_RTHIT,
		     "FW processing hit real-time."),
	DEFINE_ALARM(PON_ALARM_EDGE_PLU_OFL,
		     "The PLOAMu FIFO shows an overflow."),
	DEFINE_ALARM(PON_ALARM_EDGE_RTERR,
		     "Ranging time error, the OLT has sent a ranging time value which cannot be handled by the firmware."
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_PLL3ERR,
		     "The configuration of PLL3 is incorrect. The VCO frequency is set to a value different than the allowed values of 5 GHz or 10 Ghz"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_DS_ALLOCFIFO_OFL,
		     "Indicates TS FIFO-Overflow (write to full FIFO)"),
	DEFINE_ALARM(PON_ALARM_EDGE_DS_PLOAMFIFO_OFL,
		     "Indicates PLOAM-FIFO-Overflow (write to full FIFO)"),
	DEFINE_ALARM(PON_ALARM_EDGE_DS_BYTEFIFO_OFL,
		     "Indicates GTC Byte-FIFO-Overflow (write to full FIFO, GPON-GPF module)"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_DS_FSPFIFO_OFL,
		     "Indicates FSP-FIFO-Overflow (write to full FIFO, GPON-FSP module)"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_DS_XGPONPROT0,
		     "Error raised if KEY-INDEX=11 (reserved, GPON-GPF module)"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_DS_PONIP_CRYPT1,
		     "err_in_rx_busy_mst (master path fails to take over input data)"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_DS_PONIP_CRYPT2,
		     "err_in_rx_busy_cryp (cryptographic path fails to take over input data)"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_DS_PONIP_CRYPT3,
		     "err_in_rx_busy_byp (bypass path fails to take over input data)"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_DS_PONIP_CRYPT4,
		     "err_out_rx_busy (external recipient fails to take over output data)"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_DS_PONIP_CRYPT5,
		     "err_out_tx_lat_byp(bypass data not available exactly on time)."
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_US_GASTL,
		     "A GPON burst allocation series was available too late in the allocation FIFO to reach the requested burst start time"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_US_GPOBMWMI,
		     "The GPON playout buffer's meta word does not contain the mandatory magic word"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_US_GPOBMWAI,
		     "The GPON playout buffer's meta word traffic index does not match the allocation's allocation index"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_US_GAFOFL,
		     "The GPON allocation FIFO signaled an overflow (write to full FIFO)"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_US_GPFOFL,
		     "The GPON PLOAMu FIFO GPLAOMUDATA signaled an overflow (write to full FIFO)"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_US_G4PFOFL,
		     "The GTC PLOAMu FIFO G4PLAOMUDATA signaled an overflow (write to full FIFO)"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_US_BSM,
		     "The burst start was missed in the PSBu module"),
	DEFINE_ALARM(PON_ALARM_EDGE_US_GTSIDOFL,
		     "The internal GPON timestamp SID FIFO signaled an overflow (write to full FIFO)"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_US_GTSFOFL,
		     "The GPON timestamp FIFO signaled an overflow (write to full FIFO)"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_POBUF_FD,
		     "A frame has been dropped as the free space in memory is too small or because write accesses have been detected while a FIFO flush was ongoing"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_POBUF_WWFF,
		     "A write access to a Data FIFO (Alloc-Index) has been detected while a FIFO-flush of this Alloc-Index was ongoing"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_POBUF_RWFF,
		     "A read access to a Data FIFO (Alloc-Index) has been detected while a FIFO-flush of this Alloc-Index was ongoing"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_POBUF_USFR,
		     "The Upstream module sent a freeze request"),
	DEFINE_ALARM(PON_ALARM_EDGE_POBUF_RFO,
		     "The Requester FIFO detected an overflow (write to full FIFO)"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_POBUF_DFM,
		     "The DBRu fetcher was not able to fetch all information within one frame"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_POBUF_DXT,
		     "The DBRu fetcher did not receive a response from the Crossbar master interface within the configured timeout threshold"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_POBUF_DSVO,
		     "The fill-static values fetched by the DBRu fetcher needed saturation to fit into the parameter RAM"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_XRX_OFL,
		     "Indicates the reassembly input data-rate exceeded the possible output data-rate"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_XRX_RBNBA,
		     "Indicates that a frame could not be placed in any of the three buffers as they are all blocked"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_XRX_RBLOST,
		     "There was no free space in the buffer the frame was assigned to"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_XRX_RBLOST0,
		     "There was no free space in the buffer the frame was assigned to"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_XRX_RBLOST1,
		     "There was no free space in the buffer 1 the frame was assigned to"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_XRX_RBLOST2,
		     "There was no free space in the buffer 2 the frame was assigned to"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_XRX_RBPKTDROP,
		     "There was an undersized packet dropped"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_XRX_RBMWERR,
		     "There was a Magic Word Error (NOT found)"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_XRX_RBOFL,
		     "Indicates the reassembly input data-rate exceeded the possible output data-rate"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_XGT_FCSCHK,
		     "Indicates that the FCSCHK module reported an error"),
	DEFINE_ALARM(PON_ALARM_EDGE_XGT_FCSADD,
		     "Indicates that the FCSADD module reported an error"),
	DEFINE_ALARM(PON_ALARM_EDGE_XGT_STAG,
		     "Indicates that the STAG Ethertype received was not 0x88C3"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_XGT_LENGTH,
		     "Indicates that the length calculated did not match the length expected in USMW"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_CTRDS_OFLP0,
		     "Indicates overflow on counter port 0"),
	DEFINE_ALARM(PON_ALARM_EDGE_CTRDS_OFLP1,
		     "Indicates overflow on counter port 1"),
	DEFINE_ALARM(PON_ALARM_EDGE_CTRDS_OFLP2,
		     "Indicates overflow on counter port 2"),
	DEFINE_ALARM(PON_ALARM_EDGE_CTRDS_OFLP3,
		     "Indicates overflow on counter port 3"),
	DEFINE_ALARM(PON_ALARM_EDGE_CTRDS_OFLP4,
		     "Indicates overflow on counter port 4"),
	DEFINE_ALARM(PON_ALARM_EDGE_CTRUS_OFLP0,
		     "Indicates overflow on counter port 0"),
	DEFINE_ALARM(PON_ALARM_EDGE_CTRUS_OFLP1,
		     "Indicates overflow on counter port 1"),
	DEFINE_ALARM(PON_ALARM_EDGE_CTRUS_OFLP2,
		     "Indicates overflow on counter port 2"),
	DEFINE_ALARM(PON_ALARM_EDGE_XBM_ARV0,
		     "An access to an address outside the configured address range occurred on port 0"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_XBM_ARV2,
		     "An access to an address outside the configured address range occurred on port 2"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_XBM_AV0,
		     "An invalid access was detected on port 0"),
	DEFINE_ALARM(PON_ALARM_EDGE_XBM_AV2,
		     "An invalid access was detected on port 2"),
	DEFINE_ALARM(PON_ALARM_EDGE_XBM_OCPE,
		     "An OCP access returned with status ERR (SResp = 0b11)"),
	DEFINE_ALARM(PON_ALARM_EDGE_XBM_OCPF,
		     "An OCP access returned with status FAIL (SResp = 0b10)"),
	DEFINE_ALARM(PON_ALARM_EDGE_XBM_OCPSR,
		     "The OCP slave signaled that it is in reset (SRESET_N == 0)"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_SYSR_CF,
		     "A clock fail of the PON clock was detected"),
	DEFINE_ALARM(PON_ALARM_EDGE_PHYIF_ROG,
		     "The Rogue Detection within PHYIF fired"),
	DEFINE_ALARM(PON_ALARM_EDGE_PHYIF_HIB,
		     "The data-valid signal went inactive in the middle of a burst"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_PHYIF_PS,
		     "The data-valid signal in 2.5G/1.25G modes is not equally spaced. Check ETXC register for debugging."
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_EXT_XPCSL,
		     "Same signal as XPCSLS just configured as edge sensitive"),
	DEFINE_ALARM(PON_ALARM_EDGE_EXT_XPCS,
		     "Same signal as XPCSS just configured as edge sensitive"),
	DEFINE_ALARM(PON_ALARM_EDGE_EXT_XTCG,
		     "Same signal as XTCGS just configured as edge sensitive"),
	DEFINE_ALARM(PON_ALARM_EDGE_EXT_TXFE, "The TX FIFO was empty"),
	DEFINE_ALARM(PON_ALARM_EDGE_EXT_TXFF, "The TX FIFO was full"),
	DEFINE_ALARM(PON_ALARM_EDGE_ERR_LOS,
		     "Loss of Signal was signaled by the optics"),
	DEFINE_ALARM(PON_ALARM_EDGE_ERR_XGRCR,
		     "The block receiving the data PON IP sends via the XGMII RX interface was reset"
		     ),
	DEFINE_ALARM(PON_ALARM_EDGE_ERR_XBAR, "The crossbar was reset"),
	DEFINE_ALARM(PON_ALARM_EDGE_ERR_TXF,
		     "Transmitter Fault was signaled by the optics"),
	DEFINE_ALARM(PON_ALARM_EDGE_ERR_XGTCR,
		     "The block providing the data PON IP receives via the XGMII TX interface was reset"
		     ),
};

int fapi_pon_visit_alarms_level(void *ctx, alarms_visitor_t cb, void *data)
{
	int i;
	int ret = 0;

	for (i = 0; i < sizeof(pon_alarm_level) /
	     sizeof(struct alarm_type); i++) {
		ret = cb(ctx, &pon_alarm_level[i], data);
		if (ret != 0)
			break;
	}
	return ret;
}

int fapi_pon_visit_alarms_edge(void *ctx, alarms_visitor_t cb, void *data)
{
	int i;
	int ret = 0;

	for (i = 0; i < sizeof(pon_alarm_edge) /
	     sizeof(struct alarm_type); i++) {
		ret = cb(ctx, &pon_alarm_edge[i], data);
		if (ret != 0)
			break;
	}
	return ret;
}
