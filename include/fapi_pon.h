/*****************************************************************************
 *
 * Copyright (c) 2020 - 2025 MaxLinear, Inc.
 * Copyright (c) 2017 - 2020 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/

/* Naming conventions
 * Generic functions used by any PON mode:
 * fapi_pon_<function>_<get|set|status_get|…>
 *
 * Functions which are only used in one mode and are thus not ambiguous are
 * handled the same:
 * fapi_pon_<function>_<get|set|status_get|…>
 *
 * Similar functions which are used in different ways by different modes:
 * fapi_pon_<gpon>_<function>_<get|set|status_get|…>
 *
 * Note: The definition "SWIG" is used to control the automatic CLI generation
 * from this header file. Functions which are inside such as
 *
 * #ifndef SWIG
 * enum fapi_pon_errorcode fapi_pon_<function_name>(struct pon_ctx <params>);
 * #endif
 *
 * are not automatically converted into CLI functions.
 *
 */

/**
 *  \file
 *  This is the PON library header file, defining the generic API functions
 *  and data structures for controlling the PON IP module in any of the PON
 *  operation modes.
 */

#ifndef _FAPI_PON_H_
#define _FAPI_PON_H_

#include <stddef.h>
#include <stdint.h>

/** \defgroup PON_FAPI_REFERENCE PON Library Functional API Reference
 *   This chapter describes the functional API interface
 *   to access the PON IP hardware module and control its hardware and
 *   firmware functionality.
 *
 *   If a function is not yet implemented or only partly implemented it will
 *   return an error code (PON_STATUS_SUPPORT), if called.
 *
 *   @{
 */

/** \defgroup PON_FAPI_REFERENCE_GLOBAL Global PON Library Functions
 *   This chapter describes the global functions of the PON IP library.
 *   These functions are not related to a specific PON type
 *   (such as GPON).
 *  @{
 */

/* PON global constant definitions */
/* =============================== */

/** PON library version concatenation */
#define PON_VERSION(a, b, c) (((a) << 16) + ((b) << 8) + (c))
/** PON library version list, needs pon_config.h included before */
#define PON_VERSION_CODE \
	PON_VERSION(PON_VER_MAJOR, PON_VER_MINOR, PON_VER_STEP)

/* PON feature capability definitions.
 *  These definitions can be used with \ref fapi_pon_cap_get.
 */

/** Debug mode operation */
#define PON_FEATURE_DBG      0x00000001
/** Encryption supports HMAC-SHA-512 */
#define PON_FEATURE_CRY3     0x00000002
/** Encryption supports HMAC-SHA-256 */
#define PON_FEATURE_CRY2     0x00000004
/** Encryption supports AES-CMAC-128 */
#define PON_FEATURE_CRY1     0x00000008
/** Dynamic bandwidth reporting mode 1 according to ITU-T G.984/987/989 */
#define PON_FEATURE_DBAM1    0x00000010
/** Dynamic bandwidth reporting mode 0 according to ITU-T G.984/987/989 */
#define PON_FEATURE_DBAM0    0x00000020
/** Watchful sleep mode for power saving */
#define PON_FEATURE_WSLP     0x00000040
/** Cyclic sleep mode for power saving */
#define PON_FEATURE_CSLP     0x00000080
/** Doze mode for power saving */
#define PON_FEATURE_DOZE     0x00000100
/** PON rogue ONU enhancements according to ITU-T G.987.3 Annex F */
#define PON_FEATURE_ANXF     0x00001000
/** PON ID maintenance enhancements according to ITU-T G.987.3 Annex E */
#define PON_FEATURE_ANXE     0x00002000
/** PON ranging adjustment enhancements according to ITU-T G.984.3 Annex D */
#define PON_FEATURE_ANXD     0x00004000
/** PON ID maintenance enhancements according to ITU-T G.984.3 Annex C */
#define PON_FEATURE_ANXC     0x00008000
/** XGS-PON mode according to ITU-T G.9807.1 */
#define PON_FEATURE_G9807    0x00010000
/** NG-PON2 mode according to ITU-T G.989.3 */
#define PON_FEATURE_G989     0x00020000
/** XG-PON mode according to ITU-T G.987.3 */
#define PON_FEATURE_G987     0x00040000
/** GPON mode according to ITU-T G.984.3 */
#define PON_FEATURE_G984     0x00080000
/** AMCC channel */
#define PON_FEATURE_AMCC     0x00100000
/** Secure debug mode */
#define PON_FEATURE_SDBG     0x00200000
/** Authentication table size */
#define MAX_AUTH_TABLE_SIZE	16
/** Maximum number of GEM ports which are supported. */
#define MAX_GEM_PORTS		256

/** PON type definition for GPON according to ITU-T G.984.3
 *  Used by \ref fapi_pon_version_get
 */
#define PON_TYPE_GPON		0x00000001
/** PON type definition for XG-PON according to ITU-T G.987.3
 *  Used by \ref fapi_pon_version_get
 */
#define PON_TYPE_XGPON		0x00000002
/** PON type definition for NG-PON2 according to ITU-T G.989.3
 *  Used by \ref fapi_pon_version_get
 */
#define PON_TYPE_NGPON2		0x00000004
/** PON type definition for XGS-PON according to ITU-T G.9807.1
 *  Used by \ref fapi_pon_version_get
 */
#define PON_TYPE_XGSPON		0x00000008

/* PON alarm event codes.
 * Used by \ref fapi_pon_alarm_status_get,
 * \ref fapi_pon_alarm_status_set and the callback functions registered
 * in \ref fapi_pon_register_alarm_report and
 * \ref fapi_pon_register_alarm_clear.
 */

/** Loss of signal */
#define PON_ALARM_STATIC_LOS   0x0000
/** Loss of frame (G984.3 only) */
#define PON_ALARM_STATIC_LOF   0x0001
/** Loss of downstream synchronization (GTC, XGTC, TWDM, IEEE) */
#define PON_ALARM_STATIC_LODS  0x0002
/** Signal failed */
#define PON_ALARM_STATIC_SF    0x0003
/** Signal degraded */
#define PON_ALARM_STATIC_SD    0x0004
/** Loss of GEM channel delineation */
#define PON_ALARM_STATIC_LCDG  0x0005
/** Transmitter failure */
#define PON_ALARM_STATIC_TF    0x0006
/** Start-up failure */
#define PON_ALARM_STATIC_SUF   0x0007
/** Link mismatch (GTC only) */
#define PON_ALARM_STATIC_MIS   0x0008
/** Emergency stop state entered (PLOAM state O7/07.1/07.2) */
#define PON_ALARM_STATIC_O7    0x0009
/** Loss of PMA lock */
#define PON_ALARM_STATIC_LOL   0x000A
/** Clock alarm reported by SoC */
#define PON_ALARM_STATIC_CLK   0x000B
/** Free spectral range mismatch (TWDM only) */
#define PON_ALARM_STATIC_FSR   0x000C
/** Channel spacing mismatch (TWDM only) */
#define PON_ALARM_STATIC_CSP   0x000D
/** Maximum spectral excursion mismatch (TWDM only) */
#define PON_ALARM_STATIC_MSE   0x000E
/** Unknown PLOAM message */
#define PON_ALARM_EDGE_UPM   0x000F
/** Deactivate ONU message received */
#define PON_ALARM_EDGE_DACT  0x0010
/** Physical equipment error (GTC only) */
#define PON_ALARM_STATIC_PEE   0x0011
/** Remote defect indication (GTC only) */
#define PON_ALARM_EDGE_RDI   0x0012
/** Rogue ONU detected */
#define PON_ALARM_EDGE_ROG   0x0013
/** PLOAM state changed */
#define PON_ALARM_EDGE_PSC   0x0014
/** Firmware Alarm */
#define PON_ALARM_EDGE_FWA   0x0015
/** OMCI Integrity Key has changed */
#define PON_ALARM_EDGE_OIKC  0x0016
/** Persistent crossbar access error */
#define PON_ALARM_STATIC_PCE  0x0017
/** Persistent SerDes error */
#define PON_ALARM_STATIC_PSE  0x0018
/** The mutual authentication status had changed */
#define PON_ALARM_EDGE_ASC   0x001A
/** Capture reload event has happened. */
#define PON_ALARM_EDGE_CAPREL   0x001B
/** Capture downstream tick event has happened. */
#define PON_ALARM_EDGE_CAPDST   0x001C
/** Capture external event has happened. */
#define PON_ALARM_EDGE_CAPEXT   0x001D
/**
 * The sum of the ONU response time value and the equalization delay has
 * changed.
 */
#define PON_ALARM_EDGE_EQDEL   0x001E
/** Channel Profile Index timeout event (TWDM only). */
#define PON_ALARM_EDGE_CPI_TO   0x001F
/** Forced Wake-up Indication received from the OLT while in a sleep mode. */
#define PON_ALARM_EDGE_FWI   0x0100
/**
 * Firmware version error, the loaded firmware does not fit to the PON IP
 * hardware version.
 */
#define PON_ALARM_EDGE_FWVERR   0x0101
/** NG-PON2 message AdjustTXWavelength parameter inconsistency. */
#define PON_ALARM_EDGE_NG_ATWLPI   0x0102
/** FW processing hit real-time. */
#define PON_ALARM_EDGE_FW_RTHIT   0x0103
/**
 * The firmware cannot access DPLL or PLL3 registers of the SoC. Firware-based
 * SyncE processing is stopped.
 */
#define PON_ALARM_STATIC_PLL_ERR   0x0104
/**
 * The downstream FEC indication in the TC header does not match the target
 * configuration. Probably the OLT needs to be re-configured.
 */
#define PON_ALARM_STATIC_DS_FEC_FAIL   0x0105
/** The PLOAMu FIFO shows an overflow. */
#define PON_ALARM_EDGE_PLU_OFL   0x0106
/**
 * Ranging time error, the OLT has sent a ranging time value which cannot be
 * handled by the firmware.
 */
#define PON_ALARM_EDGE_RTERR   0x0108
/**
 * The configuration of PLL3 is incorrect. The VCO frequency is set to a value
 * different than the allowed values of 5 GHz or 10 Ghz
 */
#define PON_ALARM_EDGE_PLL3ERR   0x0109
/**
 * ToD information is invalid. This alarm is set upon startup and cleared with
 * the first 1PPS pulse that is sent out once the time information is valid.
 * The alarm is set again, if the time information gets invalid (leaving
 * PLOAM state O5.x or reconfiguration of ToD hardware).
 */
#define PON_ALARM_STATIC_TOD_INVALID   0x010C
/**
 * (G.989.3 only): Unexpected, wrong downstream wavelength received from OLT.
 * The Rx wavelength channel configured to the transceiver does not match the
 * one indicated in the DS PHY frame.
 */
#define PON_ALARM_EDGE_DSWL_ERR		0x010D
/** Too many DS PLOAM MIC errors encountered in PLOAM state O1.2. */
#define PON_ALARM_EDGE_MIC_ERRS_O12	0x010E
/** Too many DS PLOAM MIC errors encountered in PLOAM state O7.1. */
#define PON_ALARM_EDGE_MIC_ERRS_O71	0x010F
/** Too many DS PLOAM MIC errors encountered in PLOAM state O8.2. */
#define PON_ALARM_EDGE_MIC_ERRS_O82	0x0110
/** MIC error encountered in a DS PLOAM message (any PLOAM state) */
#define PON_ALARM_EDGE_MIC_ERR		0x0111
/** Indicates TS FIFO-Overflow (write to full FIFO). */
#define PON_ALARM_EDGE_DS_ALLOCFIFO_OFL   0x0200
/** Indicates PLOAM-FIFO-Overflow (write to full FIFO). */
#define PON_ALARM_EDGE_DS_PLOAMFIFO_OFL   0x0201
/** Indicates LODS. */
#define PON_ALARM_STATIC_DS_LODS   0x0202
/** Indicates AES-Engine-Overflow, output data of AES is corrupted
 *  (Old-Decryption, not present in new PONIP-CRYPT module).
 */
#define PON_ALARM_STATIC_DS_AES_OFL   0x0203
/** Indicates GTC Byte-FIFO-Overflow (write to full FIFO, GPON-GPF module). */
#define PON_ALARM_EDGE_DS_BYTEFIFO_OFL   0x0204
/** Indicates TS-FIFO Error (FIFO was not read out completely by FW/SW). */
#define PON_ALARM_STATIC_DS_ALLOCFIFO_ERR   0x0205
/** Indicates PLOAM-FIFO Error (FIFO was not read out completely by FW/SW). */
#define PON_ALARM_STATIC_DS_PLOAMFIFO_ERR   0x0206
/** Indicates FSP-FIFO-Overflow (write to full FIFO, GPON-FSP module). */
#define PON_ALARM_EDGE_DS_FSPFIFO_OFL   0x0207
/** Error raised if KEY-INDEX=11 (reserved, GPON-GPF module). */
#define PON_ALARM_EDGE_DS_XGPONPROT0   0x020B
/** err_in_rx_busy_mst (master path fails to take over input data). */
#define PON_ALARM_EDGE_DS_PONIP_CRYPT1   0x0213
/** err_in_rx_busy_cryp (cryptographic path fails to take over input data). */
#define PON_ALARM_EDGE_DS_PONIP_CRYPT2   0x0214
/** err_in_rx_busy_byp (bypass path fails to take over input data). */
#define PON_ALARM_EDGE_DS_PONIP_CRYPT3   0x0215
/** err_out_rx_busy (external recipient fails to take over output data). */
#define PON_ALARM_EDGE_DS_PONIP_CRYPT4   0x0216
/** err_out_tx_lat_byp (bypass data not available exactly on time). */
#define PON_ALARM_EDGE_DS_PONIP_CRYPT5   0x0217
/** A GPON burst allocation series was available too late in the allocation FIFO
 *  to reach the requested burst start time.
 */
#define PON_ALARM_EDGE_US_GASTL   0x0400
/** The GPON playout buffer's meta word does not contain the mandatory magic
 *  word.
 */
#define PON_ALARM_EDGE_US_GPOBMWMI   0x0401
/** The GPON playout buffer's meta word traffic index does not match the
 *  allocation's allocation index.
 */
#define PON_ALARM_EDGE_US_GPOBMWAI   0x0402
/** The GPON allocation FIFO signaled an overflow (write to full FIFO). */
#define PON_ALARM_EDGE_US_GAFOFL   0x0403
/** The GPON PLOAMu FIFO GPLAOMUDATA signaled an overflow (write to full FIFO).
 */
#define PON_ALARM_EDGE_US_GPFOFL   0x0404
/** The GTC PLOAMu FIFO G4PLAOMUDATA signaled an overflow (write to full FIFO).
 */
#define PON_ALARM_EDGE_US_G4PFOFL   0x0405
/** The burst start was missed in the PSBu module. This indicates that
 *  GCFG.BURSTSTARTCLEAD is too small.
 */
#define PON_ALARM_EDGE_US_BSM   0x0406
/** The internal GPON timestamp SID FIFO signaled an overflow (write to full
 *  FIFO).
 */
#define PON_ALARM_EDGE_US_GTSIDOFL   0x0407
/** The GPON timestamp FIFO signaled an overflow (write to full FIFO). */
#define PON_ALARM_EDGE_US_GTSFOFL   0x0408
/** A frame has been dropped as the free space in memory is too small or because
 *  write accesses have been detected while a FIFO flush was ongoing.
 */
#define PON_ALARM_EDGE_POBUF_FD   0x0500
/** A write access to a Data FIFO (Alloc-Index) has been detected while
 *  a FIFO-flush of this Alloc-Index was ongoing.
 */
#define PON_ALARM_EDGE_POBUF_WWFF   0x0501
/** A read access to a Data FIFO (Alloc-Index) has been detected while
 *  a FIFO-flush of this Alloc-Index was ongoing.
 */
#define PON_ALARM_EDGE_POBUF_RWFF   0x0502
/** The Upstream module sent a freeze request. */
#define PON_ALARM_EDGE_POBUF_USFR   0x0507
/** The Requester FIFO detected an overflow (write to full FIFO). */
#define PON_ALARM_EDGE_POBUF_RFO   0x0508
/** The DBRu fetcher was not able to fetch all information within one frame. */
#define PON_ALARM_EDGE_POBUF_DFM   0x0518
/** The DBRu fetcher did not receive a response from the Crossbar master
 *  interface within the configured timeout threshold.
 */
#define PON_ALARM_EDGE_POBUF_DXT   0x0519
/** The fill-static values fetched by the DBRu fetcher needed saturation to fit
 *  into the parameter RAM.
 */
#define PON_ALARM_EDGE_POBUF_DSVO   0x051A
/** Indicates the reassembly input data-rate exceeded the possible output
 *  data-rate.
 */
#define PON_ALARM_EDGE_XRX_OFL   0x0700
/** Indicates that a frame could not be placed in any of the three buffers as
 *  they are all blocked. As a direct consequence the input is delayed (!ready)
 *  but not dropped. As an indirect consequence this can lead to an RBOFL.
 */
#define PON_ALARM_EDGE_XRX_RBNBA   0x0701
/** There was no free space in the buffer the frame was assigned to. As
 *  a consequence it was skipped.
 */
#define PON_ALARM_EDGE_XRX_RBLOST   0x0702
/** There was no free space in the buffer the frame was assigned to. As
 *  a consequence it was skipped.
 */
#define PON_ALARM_EDGE_XRX_RBLOST0   0x0703
/** There was no free space in the buffer 1 the frame was assigned to. As
 *  a consequence it was skipped.
 */
#define PON_ALARM_EDGE_XRX_RBLOST1   0x0704
/** There was no free space in the buffer 2 the frame was assigned to. As
 *  a consequence it was skipped.
 */
#define PON_ALARM_EDGE_XRX_RBLOST2   0x0705
/** There was an "undersized" packet dropped */
#define PON_ALARM_EDGE_XRX_RBPKTDROP 0x0706
/** There was a Magic Word Error (NOT found) */
#define PON_ALARM_EDGE_XRX_RBMWERR   0x0707
/** Indicates the reassembly input data-rate exceeded the possible output
 *  data-rate.
 */
#define PON_ALARM_EDGE_XRX_RBOFL   0x071F
/** Indicates that the FCSCHK module reported an error */
#define PON_ALARM_EDGE_XGT_FCSCHK   0x0800
/** Indicates that the FCSADD module reported an error */
#define PON_ALARM_EDGE_XGT_FCSADD   0x0801
/** Indicates that the STAG Ethertype received was not 0x88C3 */
#define PON_ALARM_EDGE_XGT_STAG   0x0802
/** Indicates that the length calculated did not match the length expected in
 *  USMW.
 */
#define PON_ALARM_EDGE_XGT_LENGTH   0x0803
/** Indicates whether the decapsulate reported an interrupt */
#define PON_ALARM_STATIC_XGT_DEC   0x081F
/** Indicates overflow on counter port 0 */
#define PON_ALARM_EDGE_CTRDS_OFLP0   0x0A00
/** Indicates overflow on counter port 1 */
#define PON_ALARM_EDGE_CTRDS_OFLP1   0x0A01
/** Indicates overflow on counter port 2 */
#define PON_ALARM_EDGE_CTRDS_OFLP2   0x0A02
/** Indicates overflow on counter port 3 */
#define PON_ALARM_EDGE_CTRDS_OFLP3   0x0A03
/** Indicates overflow on counter port 4 */
#define PON_ALARM_EDGE_CTRDS_OFLP4   0x0A04
/** Indicates overflow on counter port 0 */
#define PON_ALARM_EDGE_CTRUS_OFLP0   0x0B00
/** Indicates overflow on counter port 1 */
#define PON_ALARM_EDGE_CTRUS_OFLP1   0x0B01
/** Indicates overflow on counter port 2 */
#define PON_ALARM_EDGE_CTRUS_OFLP2   0x0B02
/** An access to an address outside the configured address range occurred on
 *  port 0.
 */
#define PON_ALARM_EDGE_XBM_ARV0   0x0C00
/** An access to an address outside the configured address range occurred on
 *  port 2.
 */
#define PON_ALARM_EDGE_XBM_ARV2   0x0C02
/** An invalid access was detected on port 0. Please refer to the design
 *  specification for a definition of invalid accesses.
 */
#define PON_ALARM_EDGE_XBM_AV0   0x0C08
/** An invalid access was detected on port 2. Please refer to the design
 *  specification for a definition of invalid accesses.
 */
#define PON_ALARM_EDGE_XBM_AV2   0x0C0A
/** An OCP access returned with status ERR (SResp = 0b11). */
#define PON_ALARM_EDGE_XBM_OCPE   0x0C1D
/** An OCP access returned with status FAIL (SResp = 0b10). */
#define PON_ALARM_EDGE_XBM_OCPF   0x0C1E
/** The OCP slave signaled that it is in reset (SRESET_N == 0). */
#define PON_ALARM_EDGE_XBM_OCPSR   0x0C1F
/** A clock fail of the PON clock was detected. The root-clock was switched back
 *  to the boot-clock.
 */
#define PON_ALARM_EDGE_SYSR_CF   0x0D00
/** A clock fail of the PON clock is signaled by clock-source SerDes. */
#define PON_ALARM_STATIC_SYSR_CFE   0x0D04
/** The period of the PON clock is too low. Reported by fast but inaccurate
 *  observation block.
 */
#define PON_ALARM_STATIC_SYSR_CFIL   0x0D05
/** The period of the PON clock is too high. Reported by fast but inaccurate
 *  observation block.
 */
#define PON_ALARM_STATIC_SYSR_CFIH   0x0D06
/** A clock fail of the PON clock is signaled by clock-source PLL. */
#define PON_ALARM_STATIC_SYSR_CFE2   0x0D08
/** The period of the PON clock is too low. Reported by slow but accurate
 *  observation block.
 */
#define PON_ALARM_STATIC_SYSR_CFI2L   0x0D09
/** The period of the PON clock is too high. Reported by slow but accurate
 *  observation block.
 */
#define PON_ALARM_STATIC_SYSR_CFI2H   0x0D0A
/** The Rogue Detection within PHYIF fired. Check ETXC registers for debugging
 */
#define PON_ALARM_EDGE_PHYIF_ROG   0x0E00
/** The data-valid signal went inactive in the middle of a burst. Check ETXC
 *  register for debugging.
 */
#define PON_ALARM_EDGE_PHYIF_HIB   0x0E01
/** The data-valid signal in 2.5G/1.25G modes is not equally spaced. Check ETXC
 *  register for debugging.
 */
#define PON_ALARM_EDGE_PHYIF_PS   0x0E02
/** The value of tx_sd_i can be observed here. */
#define PON_ALARM_STATIC_PHYIF_TXSD   0x0E1E
/** Same signal as XPCSLS just configured as edge sensitive. */
#define PON_ALARM_EDGE_EXT_XPCSL   0x0F00
/** Link status line from the external XPCS module. */
#define PON_ALARM_STATIC_EXT_XPCSLS   0x0F01
/** Same signal as XPCSS just configured as edge sensitive. */
#define PON_ALARM_EDGE_EXT_XPCS   0x0F02
/** Interrupt line from the external XPCS module */
#define PON_ALARM_STATIC_EXT_XPCSS   0x0F03
/** Same signal as XTCGS just configured as edge sensitive. */
#define PON_ALARM_EDGE_EXT_XTCG   0x0F08
/** MAC level signaling of EEE/LPI status. */
#define PON_ALARM_STATIC_EXT_XTCGS   0x0F09
/** Exceptions occurred in the QoS Engine Emulator module. FPGA only! */
#define PON_ALARM_STATIC_EXT_EQEMU   0x0F1B
/** The TX FIFO was empty. FPGA only!. */
#define PON_ALARM_EDGE_EXT_TXFE   0x0F1C
/** Same signal as TXFE just configured as level sensitive. */
#define PON_ALARM_STATIC_EXT_TXFES   0x0F1D
/** The TX FIFO was full. FPGA only!. */
#define PON_ALARM_EDGE_EXT_TXFF   0x0F1E
/** Same signal as TXFF just configured as level sensitive. */
#define PON_ALARM_STATIC_EXT_TXFFS   0x0F1F
/** Loss of Signal was signaled by the optics. */
#define PON_ALARM_EDGE_ERR_LOS   0x1000
/** Same signal as LOS just configured as level sensitive. */
#define PON_ALARM_STATIC_ERR_LOSS   0x1001
/** The block receiving the data PON IP sends via the XGMII RX interface was
 *  reset.
 */
#define PON_ALARM_EDGE_ERR_XGRCR   0x1008
/** Same signal as XGRCR just configured as level sensitive */
#define PON_ALARM_STATIC_ERR_XGRCRS   0x1009
/** The crossbar was reset. */
#define PON_ALARM_EDGE_ERR_XBAR   0x100C
/** Same signal as XBAR just configured as level sensitive. */
#define PON_ALARM_STATIC_ERR_XBARS   0x100D
/** The dying_gasp input shows a power fail. */
#define PON_ALARM_STATIC_ERR_DYG   0x100E
/** Transmitter Fault was signaled by the optics. */
#define PON_ALARM_EDGE_ERR_TXF   0x1010
/** Same signal as TXF just configured as level sensitive. */
#define PON_ALARM_STATIC_ERR_TXFS   0x1011
/** The block providing the data PON IP receives via the XGMII TX interface was
 *  reset.
 */
#define PON_ALARM_EDGE_ERR_XGTCR   0x101A
/** Same signal as XGTCR just configured as level sensitive. */
#define PON_ALARM_STATIC_ERR_XGTCRS   0x101B

/* PON alarm status.
 * Used by \ref fapi_pon_alarm_status_get.
 */

/** Alarm enabled */
#define PON_ALARM_EN 1
/** Alarm disabled */
#define PON_ALARM_DIS 0

/** PON allocation is valid.
 *  Used by \ref fapi_pon_gem_port_id_get and
 *  \ref fapi_pon_gem_port_index_get
 */
#define PON_ALLOC_VALID 1
/** PON allocation is invalid.
 *  Used by \ref fapi_pon_gem_port_id_get and
 *  \ref fapi_pon_gem_port_index_get
 */
#define PON_ALLOC_INVALID 0

/** XGEM key size, given in number of bits.
 *   Used by \ref fapi_pon_xgem_key_cfg_set
 */
#define PON_MAXIMUM_XGEM_KEY_SIZE	128
/** XGEM key size, given in number of double words.
 *   Used by \ref fapi_pon_xgem_key_cfg_set
 */
#define PON_MAXIMUM_XGEM_KEY_SIZE_BY_4	4

/** Time in ps per PON IP hardware clock cycle in GPON modes.
 *	Used by \ref fapi_pon_optic_cfg_set and \ref fapi_pon_optic_cfg_get
 */
#define GPON_CLOCK_CYCLE 3215

/** Downstream bit time in fs for GPON operation mode at 2.48832 Gbit/s */
#define GPON_DS_BIT_TIME_FS 401878
/** Downstream bit time in fs for XGS-PON operation mode at 9.95328 Gbit/s */
#define XGSPON_DS_BIT_TIME_FS 100470

/* Loop timing mode definitions. */

/** SW, The software informs the firmware about the loop timing lock status.
 *  This operation mode is deprecated and shall not be used.
 */
#define PON_LOOP_MODE_SW	0x2
/** The firmware performs the loop timing control handling, the software does
 *  not try to access the hardware. This is the recommended operation mode and
 *  is used by \ref fapi_pon_optic_cfg_set.
 */
#define PON_LOOP_MODE_FW	0x3

/** Doze mode.
 *  This can be selected by \ref fapi_pon_psm_cfg_set.
 */
#define PON_PSM_CONFIG_MODE_DOZE 0x1
/** Cyclic sleep mode
 *  This can be selected by \ref fapi_pon_psm_cfg_set.
 */
#define PON_PSM_CONFIG_MODE_CSL 0x2
/** Watchful sleep mode
 *  This can be selected by \ref fapi_pon_psm_cfg_set.
 */
#define PON_PSM_CONFIG_MODE_WSL 0x4

/* Interoperability option flags. */

/** ITU Interoperability option 0.
 *  Disable OMCI encryption in both directions.
 *  If this mode is activated, PON_IOP_CFG_OMCI_ENC_US_DIS is ignored.
 *  This is used by \ref fapi_pon_iop_cfg_set.
 */
#define PON_IOP_CFG_OMCI_ENC_DIS	0x00000001

/** ITU Interoperability option 1.
 *  Disable OMCI encryption in upstream direction.
 *  This is used by \ref fapi_pon_iop_cfg_set.
 */
#define PON_IOP_CFG_OMCI_ENC_US_DIS	0x00000002

/** Downstream flag.
 *  Used by \ref fapi_pon_get_gtc_log and \ref fapi_pon_get_xgtc_log.
 */
#define PON_DS 0

/** Upstream flag.
 *  Used by \ref fapi_pon_get_gtc_log and \ref fapi_pon_get_xgtc_log.
 */
#define PON_US 1

/** Serial number size in bytes */
#define PON_SERIAL_NO_SIZE 8

/** Password size */
#define PON_PASSWD_SIZE 10

/* PON global enumerations */
/* ======================= */

/** PON operation modes.
 *  The numbers are important, because they are used by
 *  \ref fapi_pon_gpon_cfg_set and \ref fapi_pon_gpon_cfg_get.
 */
enum pon_mode {
	/** The PON operation mode is unknown. */
	PON_MODE_UNKNOWN = 0,
	/** The PON operation mode is ITU-T G.984 (GPON). */
	PON_MODE_984_GPON = 1,
	/** The PON operation mode is ITU-T G.987 (XG-PON). */
	PON_MODE_987_XGPON = 2,
	/** The PON operation mode is ITU-T G.9807 (XGS-PON). */
	PON_MODE_9807_XGSPON = 3,
	/** The PON operation mode is ITU-T G.989 (NG-PON2 2.5G upstream). */
	PON_MODE_989_NGPON2_2G5 = 4,
	/** The PON operation mode is ITU-T G.989 (NG-PON2 10G upstream). */
	PON_MODE_989_NGPON2_10G = 5,
#ifndef SWIG
	/** The PON operation mode is IEEE 1G-EPON.
    	    Not supported! */
	PON_MODE_IEEE_1GEPON = 6,
	/** The PON operation mode is IEEE 10G-EPON (symmetrical).
    	    Not supported! */
	PON_MODE_IEEE_10GEPON = 7,
	/** The PON operation mode is IEEE 10G/1G-EPON (asymmetrical).
    	    Not supported! */
	PON_MODE_IEEE_10_1GEPON = 8,
#endif
	/** The PON operation mode is AON (Active Ethernet). */
	PON_MODE_AON = 9
};

/** PON upstream data rates. */
enum pon_us_rate {
	/** The upstream data rate is unknown. */
	PON_US_RATE_NONE = 0,
	/** The upstream data rate is 1.24416 Gbit/s */
	PON_US_RATE_1G2 = 1,
	/** The upstream data rate is 2.48832 Gbit/s */
	PON_US_RATE_2G4 = 2,
	/** The upstream data rate is 9.95328 Gbit/s */
	PON_US_RATE_9G9 = 10
};

/** PON downstream data rates. */
enum pon_ds_rate {
	/** The downstream data rate is unknown. */
	PON_DS_RATE_NONE = 0,
	/** The downstream data rate is 2.48832 Gbit/s */
	PON_DS_RATE_2G4 = 2,
	/** The downstream data rate is 9.95328 Gbit/s */
	PON_DS_RATE_9G9 = 10
};

/** DDMI memory page selection. */
enum pon_ddmi_page {
	/** Select the 0xA0/0x50 page. */
	PON_DDMI_A0 = 0,
	/** Select the 0xA2/0x51 page. */
	PON_DDMI_A2 = 1,
	/** Number of DDMI memory pages. */
	PON_DDMI_MAX = 2
};

/** GPIO port status */
enum pon_gpio_pin_status {
	/** Use default settings for pin */
	PON_GPIO_PIN_STATUS_DEFAULT = 0,
	/** Set pin to disable state */
	PON_GPIO_PIN_STATUS_DISABLE = 1,
	/** Set pin to enable state */
	PON_GPIO_PIN_STATUS_ENABLE = 2
};

/** GPIO port ID */
enum pon_gpio_pin_id {
	/** RX_LOS pin */
	PON_GPIO_PIN_ID_RX_LOS = 0,
	/** 1PPS pin */
	PON_GPIO_PIN_ID_1PPS = 1,
	/** NTR pin */
	PON_GPIO_PIN_ID_NTR = 2
};

/** UART0 output pin options. */
enum pon_uart_pin_mode {
	/** Disable the output (high impedance). */
	PON_UART0_OFF = 0,
	/** Select the regular UART0 (ASC0) output mode. */
	PON_UART0 = 1,
	/** Select UART1 (ASC1) data to be output on pin UART0. */
	PON_UART1 = 2,
};

/** Revoke/Grant user management commands */
enum pon_perm_revoke_grant {
	/** Revoke user access from command group */
	PON_PERM_REVOKE = 0,
	/** Grant user access for command group */
	PON_PERM_GRANT = 1
};

/** Command groups for user management */
enum pon_perm_command_group {
	/** Group for SYNCE related commands*/
	PON_PERM_SYNCE = 0,
	/** Group for broadcast key handling commands */
	PON_PERM_BC_KEY = 1,
};

/** Transmit power scale */
enum pon_tx_power_scale {
	/** 0.1 µW/LSB */
	TX_POWER_SCALE_0_1 = 0,
	/** 0.2 µW/LSB */
	TX_POWER_SCALE_0_2 = 1
};

/** OLT vendor types */
enum olt_type {
	/** OLT type not identified */
	PON_OLT_UNKNOWN = 0,
	/** Adtran TA5K or SDX OLT. */
	PON_OLT_ADTRAN = 1,
	/** Calix OLT. */
	PON_OLT_CALIX = 2,
	/** Ciena OLT. */
	PON_OLT_CIENA = 3,
	/** Dasan/Zhone (DZSI) OLT. */
	PON_OLT_DZS = 4,
	/** Huawei OLT. */
	PON_OLT_HUAWEI = 5,
	/** Nokia or Alcatel OLT. */
	PON_OLT_NOKIA = 6,
	/** Tibit OLT. */
	PON_OLT_TIBIT = 7,
	/** ZTE OLT. */
	PON_OLT_ZTE = 8
#define PON_OLT_LAST PON_OLT_ZTE
};

/* PON global structures */
/* ===================== */

/** PON library ctx structure.
 *  Used by \ref fapi_pon_open and \ref fapi_pon_close.
 */
struct pon_ctx;

/** PON capability structure.
 *  Used by \ref fapi_pon_cap_get.
 */
struct pon_cap {
	/** PON module feature list.
	 *  This is a bit vector where each bit identifies a certain
	 *  functionality which is provided or not.
	 *  The bit positions are identified by their related
	 *  definitions ("PON_FEATURE_...").
	 *  - 0: Unavailable, The feature is not available.
	 *  - 1: Available, The feature is available for use.
	 */
	uint32_t features;
	/** Total number of available GEM ports in ITU PON operation modes.
	 *  GEM ports are identified by individual GEM port index values,
	 *  which range from 0 to gem_ports-1.
	 */
	uint32_t gem_ports;
	/** Total number of available Allocation IDs (T-CONTs) in ITU PON
	 *  operation modes. Allocations are identified by individual
	 *  allocation index values, which range from 0 to alloc_ids-1.
	 */
	uint32_t alloc_ids;
	/** Value of the transceiver initialization time ITRANSINIT,
	 *  this is the ONU complete transceiver initialization time in ITU PON
	 *  operation modes.
	 *  It is the time required for the ONU to regain full functionality
	 *  when leaving the "asleep" state of the power saving state machine
	 *  (PSM). The value is given in multiples of 125 us.
	 */
	uint32_t itransinit;
	/** Value of the transmitter initialization time ITXINIT,
	 *  the time required for the ONU to regain full functionality when
	 *  leaving the "listen" state of the power saving state machine (PSM)
	 *  in ITU PON operation modes.
	 *  The value is given in multiples of 125 us.
	 */
	uint32_t itxinit;
	/** Maximum QoS indices. */
	uint32_t qos_max;
};

/** PON library version structure.
 *  Used by \ref fapi_pon_version_get.
 */
struct pon_version {
	/** PON hardware version */
	uint32_t hw_version;
	/** PON library software version */
	uint32_t sw_version;
	/** PON major version */
	uint32_t fw_version_major;
	/** PON minor version */
	uint32_t fw_version_minor;
	/** PON firmware standard indication.
	 *  - 0: XGSPON, This firmware supports the G.989, G.987 and G.9087
	 *       standards.
	 *  - 1: GPON, This firmware supports the G.984 standard.
	 *  - 3: ETH, This firmware supports active Ethernet WAN modes.
	 */
	uint32_t fw_version_standard;
	/** PON firmware platform indication.
	 *  - 0: FPGA, This firmware version is intended for FPGA use
	 *  - 1: SOC, This firmware version is intended for SoC use (default)
	 */
	uint32_t fw_version_platform;
	/** PON firmware patch version */
	uint32_t fw_version_patch;
	/** PON firmware test version indication
	 *  - 0: Regular, This is a regular firmware version.
	 *  - 1: Test, This is a test version, not for productive use.
	 */
	uint32_t fw_version_is_test;
	/** PON firmware time stamp, in UNIX time format,
	 *  given as number of seconds since January 1st, 1970, related to UTC.
	 */
	uint32_t fw_timestamp;
};

/** Structure to handle the optical interface timing configuration.
 *  All time values are given in multiples ps but are rounded to
 *  multiples of internal clock cycles.
 *  For GPON applications the clock runs at 311.04 MHz
 *  (3215 ps per clock cycle).
 *  Used by \ref fapi_pon_optic_cfg_set and \ref fapi_pon_optic_cfg_get.
 */
struct pon_optic_cfg {
	/** Laser setup time (ps). */
	uint32_t laser_setup_time;
	/** Laser hold time (ps). */
	uint32_t laser_hold_time;
	/** SerDes setup time (ps). */
	uint32_t serdes_setup_time;
	/** SerDes hold time (ps). */
	uint32_t serdes_hold_time;
	/** Bias current enable signal setup time (ps). */
	uint32_t bias_setup_time;
	/** Bias current enable signal hold time (ps). */
	uint32_t bias_hold_time;
	/** Inter-burst idle pattern.
	 *  This is a 32-bit pattern, sent to the optical transmitter while the
	 *  transmit burst is disabled. This is needed in if the electrical data
	 *  signal is AC-coupled.
	 */
	uint32_t burst_idle_pattern;
	/** Burst enable signal delay configuration mode.
	 *  - 0: DIRECT, Default operation mode, the burst enable signal is
	 *      directly controlled.
	 *  - 1: RES, Reserved, do not use.
	 *  - 2: STDE, Burst enable is ORed with the SerDes enable signal.
	 *  - 3: OTP, Burst enable is ORed with the transmit power enable
	 *      signal.
	 */
	uint32_t burst_en_mode;
	/** Transmitter enable signal operation mode (TX_BIAS).
	 *  - 0: AUTO, Automatic level detection mode.
	 *  - 1: LOW, Active low output level.
	 *  - 3: HIGH, Active high output level.
	 */
	uint32_t tx_en_mode;
	/** Transmitter pull-up signal operation mode (TX_PUP).
	 *  - 0: AUTO, Automatic level detection mode.
	 *  - 1: LOW, Active low output level.
	 *  - 3: HIGH, Active high output level.
	 */
	uint32_t tx_pup_mode;
	/** LOS input signal configuration.
	 *  - 0: LOS, The input pin operates as loss of signal indication,
	 *       providing a high level if no optical signal is present.
	 *  - 1: SD, The input pin operates as signal detect,
	 *       providing a high level if an optical signal is present.
	 */
	uint32_t sd_polarity;
	/** Loop timing Power Save Mode.
	 *  - 0: DIS, The Power Save Mode is disabled.
	 *  - 1: EN, The Power Save Mode is enabled.
	 */
	uint32_t loop_timing_power_save;
	/** Enable automatic rogue ONU detection.
	 *  - 0: DIS, Automatic rogue ONU detection is disabled.
	 *  - 1: EN, Automatic internal rogue ONU detection is enabled
	 *  (deprecated).
	 *  - 2: EXT, Automatic external PMD-based rogue ONU detection is
	 *  enabled.
	 */
	uint32_t rogue_auto_en;
	/** Rogue ONU lead time, given in ns.
	 *  This is the maximum time allowed between light detection by the PMD
	 *  in the laser transmitter and the start of transmission.
	 *  The valid range is from 0 to 3000 ns, the default value is 0 ns.
	 *  The hardware configuration granularity is about 3.2 ns.
	 */
	uint32_t rogue_lead_time;
	/** Rogue ONU lag time, given in ns (the hardware configuration
	 *  granularity is about 3.2 ns).
	 *  This is the maximum time allowed between the start of transmission
	 *  and light detection by the PMD in the laser transmitter.
	 *  The valid range is from 0 to 3000 ns, the default value is 3000 ns.
	 *  The hardware configuration granularity is about 3.2 ns.
	 */
	uint32_t rogue_lag_time;
	/** Select the polarity of the OPT_TX_SD input signal.
	 *  - 0: DIR, Direct signal detect function.
	 *  - 1: INV, Inverted signal detect function.
	 */
	uint32_t opt_tx_sd_pol;
	/** Control the PSE (Permanent SerDes Error) alarm handling.
	 *  In addition to the generic alarm enable, this is used to disable the
	 *  PSE alarm in case the LOS signal indication from the PMD is not
	 *  reliable (e.g., not calibrated). This will avoid continuous alarms.
	 *  - 0: DIS, The PSE alarm is disabled.
	 *  - 1: EN, The PSE alarm is enabled.
	 */
	uint32_t pse_en;
	/** Change the transmit power scale factor.
	 *  - TX_POWER_SCALE_0_1: 0.1 uW/LSB.
	 *  - TX_POWER_SCALE_0_2: 0.2 uW/LSB.
	 */
	enum pon_tx_power_scale tx_power_scale;
	/* PON mode of the transceiver */
	enum pon_mode pon_mode;
};

/** Structure to handle the PON SerDes configuration.
 *  The values are used to configure the properties of the SerDes output to
 *  the laser driver. The SerDes is configured by the PON firmware and the
 *  software by using these values.
 *  The values depend on the PCB layout and shall be set during the PON
 *  initialization.
 *
 *  The structure is used by \ref fapi_pon_serdes_cfg_set and
 *  \ref fapi_pon_serdes_cfg_set.
 */
struct pon_serdes_cfg {
	/** Receive Loss of Signal Threshold.
	 *  This value defines the threshold compared to the receive signal at
	 *  the SerDes receiver input below which a "SerDes Loss of Signal"
	 *  (SLOS) is indicated. Only values from 1 to 7 are valid.
	 *  - 0: Reserved
	 *  - 1: 90 mVpp
	 *  - 2: 120 mVpp
	 *  - 3: 150 mVpp
	 *  - 4: 180 mVpp
	 *  - 5: 210 mVpp
	 *  - 6: 240 mVpp
	 *  - 7: 270 mVpp
	 *  - Other values: Reserved
	 */
	uint32_t rx_slos_thr;
	/** Voltage Boost Mode Enable.
	 *  This bit is used to switch the voltage boost function on.
	 *  - 0: DIS, The voltage boost function is disabled.
	 *  - 1: EN, The voltage boost function is enabled.
	 */
	uint32_t vboost_en;
	/** Voltage Boost Level.
	 *  This value defines the voltage boost level, valid from 0 to 7.
	 *  It is ignored, if voltage boost mode is disabled.
	 */
	uint32_t vboost_lvl;
	/** Current Boost Level.
	 *  This value defines the current boost level, valid from 0 to 7.
	 *  Set this to 0, if current boost mode shall be disabled.
	 */
	uint32_t iboost_lvl;
	/** SerDes transmit equalizer "main" setting.
	 *  Integer value from 0 to 40. The default value is 40.
	 *  The sum of main + pre/4 + post/4 must not exceed 40.
	 */
	uint32_t tx_eq_main;
	/** SerDes transmit equalizer "pre" setting.
	 *  tx_eq_pre[5:2]: Integer value (0 to 10, the default is 0).
	 *  tx_en_pre[1:0]: Fraction value (0, 0.25, 0.5, 0.75, default is 0).
	 *  The maximum total value is 10, with fraction value = 0.
	 *  The sum of main + pre/4 + post/4 must not exceed 40.
	 */
	uint32_t tx_eq_pre;
	/** SerDes transmit equalizer "post" setting.
	 *  tx_eq_post[5:2]: Integer value (0 to 15, the default is 0)
	 *  tx_en_post[1:0]: Fraction value (0, 0.25, 0.5, 0.75, default is 0)
	 *  The maximum total value is 15, with fraction value = 0.
	 *  The sum of main + pre/4 + post/4 must not exceed 40.
	 */
	uint32_t tx_eq_post;
	/** Automatic SerDes receive path adaptation.
	 *  - 0: DIS, Automatic adaptation is disabled.
	 *  - 1: EN, Automatic adaptation is enabled.
	 */
	uint32_t rx_adapt_en;
	/** Enables the receiver adaptation circuitry and applies the input
	 *  receiver
	 *  AFE equalization settings as defined by:
	 *  rx_eq_att_lvl
	 *  rx_eq_vga1_gain
	 *  rx_eq_vga2_gain
	 *  rx_eq_ctle_pole
	 *  rx_eq_ctle_boost
	 *
	 *  - 0: DIS, AFE adaptation is disabled.
	 *  - 1: EN, AFE adaptation is enabled.
	 */
	uint32_t rx_adapt_afe_en;
	/** Enables the receiver adaptation and decision feedback equalization
	 *  (DFE) circuitry, and applies the input setting for DFE Tap 1
	 *  as defined by rx_eq_dfe_tap1.
	 *  - 0: DIS, DFE adaptation is disabled.
	 *  - 1: EN, DFE adaptation is enabled.
	 */
	uint32_t rx_adapt_dfe_en;
	/** Receiver Adaptation Continuous Operation.
	 *  This should be asserted if continuous receiver adaptation is
	 *  required.
	 *  - 0: DIS, Automatic adaptation is disabled.
	 *  - 1: EN, Automatic adaptation is enabled.
	 */
	uint32_t rx_adapt_cont;
	/** Receiver Equalization Attenuation Level.
	 *  Controls the AFE attenuation level from -2 dB (0), to -6 dB (7).
	 *  - 0: -2.0 dB
	 *  - 1: -2.5 dB (approximately)
	 *  - 2: -3.0 dB (approximately)
	 *  - 3: -3.5 dB (approximately)
	 *  - 4: -4.0 dB (approximately)
	 *  - 5: -4.5 dB (approximately)
	 *  - 6: -5.0 dB (approximately)
	 *  - 7: -6.0 dB
	 *  - Other values: Reserved
	 */
	uint32_t rx_eq_att_lvl;
	/** Controls the CTLE boost level, binary encoded.
	 *  Values from 0 to 31 are valid.
	 */
	uint32_t rx_eq_ctle_boost;
	/** Receiver Equalization CTLE Pole.
	 *  Controls the continuous time linear equalizer (CTLE) boost pole
	 *  location, binary encoded.
	 *  Values from 0 to 3 are valid.
	 */
	uint32_t rx_eq_ctle_pole;
	/** Receiver Equalization DFE Tap 1.
	 *  Controls the value of DFE data Tap1.
	 *  Two's complement encoded.
	 *  Values from 0 to 255 are valid.
	 */
	uint32_t rx_eq_dfe_tap1;
	/** Receiver Equalization VGA Gain 1.
	 *  Controls the AFE first stage VGA gain, binary encoded.
	 *  Values from 0 to 15 are valid.
	 */
	uint32_t rx_eq_vga1_gain;
	/** Receiver Equalization VGA Gain 2.
	 *  Controls the AFE second stage VGA gain, binary encoded.
	 *  Values from 0 to 15 are valid.
	 */
	uint32_t rx_eq_vga2_gain;
	/** Receiver Adaptation mode select.
	 *  Selects which receiver adaptation mode will be performed.
	 *  This value applies to URX800 systems only.
	 *  Set this to 3 for URX800 systems.
	 *  Set this to 0 for PRX300 systems.
	 *  - 0: PCIe4 100 MHz
	 *  - 1: PCIe3 100 MHz
	 *  - 2: PCIe4 100 MHz short channel
	 *  - 3: Long 156.25 MHz
	 *  - Other values: Do not use.
	 */
	uint32_t rx_eq_adapt_mode;
	/** Select Storage Bank for Receiver Adaptation.
	 *  The SerDes supports two internal register storage banks for saving
	 *  adaptation results.
	 *  This parameter specifies which one is selected.
	 *  The storage banks are used for restoring the adapted values when
	 *  needed (for example, when returning from another rate).
	 *  Values from 0 to 3 are valid.
	 *  This value shall always be set to 0.
	 */
	uint32_t rx_eq_adapt_sel;
	/** Receiver CDR Temperature Compensation Enable.
	 *  Controls the CDR VCO temperature compensation circuit
	 *  based on the data rate.
	 * - 0: DIS, Temperature compensation disabled.
	 * - 1: EN, Temperature compensation enabled.
	 */
	uint32_t rx_vco_temp_comp_en;
	/** Receiver CDR VCO Step Control.
	 *  Controls the CDR VCO biasing based on the data rate.
	 * - 0: DIS, The function is disabled.
	 * - 1: EN, The function is enabled.
	 */
	uint32_t rx_vco_step_ctrl;
	/** Receiver CDR VCO Frequency Band.
	 *  Controls the coarse-DAC step-size based on data rate and supply
	 *  voltage level.
	 *  Values from 0 to 3 are valid.
	 *  For more information refer to the SerDes hardware specification.
	 */
	uint32_t rx_vco_frqband;
	/** Receiver Miscellaneous Controls.
	 *  Values from 0 to 255 are valid.
	 *  For more information refer to the SerDes hardware specification.
	 */
	uint32_t rx_misc;
	/** IQ (Inphase/Quadrature) offset value.
	 *  Indicates the amount of additional offset to apply to the
	 *  Inphase/Quadrature separation.
	 *  This is an unsigned offset value and covers a range of 0 to 15
	 *  (0 UI to 0.1875 UI). The value is given in multiples of 0.0125 UI.
	 */
	uint32_t rx_delta_iq;
	/** Value for receiver IQ (Inphase/Quadrature) margining.
	 *  This is a signed value for skewing the IQ separation for the purpose
	 *  of receiver margining.
	 *  It covers a range from -0.5 UI to 0.5 UI. Each margin step
	 *  corresponds to approximately 0.022 UI phase shift in magnitude.
	 */
	uint32_t rx_margin_iq;
	/** Receiver DFE bypass enable.
	 *  Indicates bypassing of the DFE circuitry. The bypass path is meant
	 *  to save power for low data rates where the DFE is not required.
	 * - 0: DIS, Bypass disabled (DFE enabled).
	 * - 1: EN, Bypass enabled (DFE disabled).
	 */
	uint32_t rx_eq_dfe_bypass;
};

/** Structure to handle the PON-related GPIO pin configuration.
 *  Used by \ref fapi_pon_gpio_cfg_set and \ref fapi_pon_gpio_cfg_get.
 */
struct pon_gpio_cfg {
	/** GPIO21 Operation mode.
	 *  This is reserved for future use and shall be set to 0.
	 */
	uint32_t gpio21_mode;
	/** GPIO24 Operation mode.
	 *  This selects the usage of the signal from the PON IP.
	 * - 0: HW, The signal is driven by the hardware.
	 * - 1: FW, The signal is driven by the hardware with optional firmware
	 *          override for enhanced optical power saving.
	 */
	uint32_t gpio24_mode;
	/** GPIO25 Operation mode.
	 *  This selects the usage of the signal from the PON IP.
	 * - 0: DEFAULT, The LOS output signal on this pin is driven by the
	 *               firmware based on the received LOS signal from the PMD.
	 * - 1: LOW, The signal is driven to a low voltage level.
	 * - 2: HIGH, The signal is driven to a high voltage level.
	 * - 3: HIMP, The signal is set to high impedance
	 *           (the output driver is switched off).
	 */
	uint32_t gpio25_mode;
};

/** A transmit and receive power of 0 mW will be reported as -32767. */
#define DMI_POWER_ZERO -32767

/** PON optical interface status structure.
 *  Used by \ref fapi_pon_optic_status_get.
 */
struct pon_optic_status {
	/** Optical transceiver temperature.
	 *  The signed value is given in a 32-bit signed 2s complement format,
	 *  in increments of 1/256 degree centigrade.
	 *  The output number range is from -32767 to 32767.
	 *  The physical temperature range is from -128 deg C to +128 deg C.
	 */
	int32_t temperature;
	/** Transceiver supply voltage.
	 *  The value is given as an unsigned 32-bit integer with an LSB
	 *  representing a step of 100 uV.
	 *  The valid number range is from 0 to 65535,
	 *  representing physical values from 0 V to 6.5535 V.
	 */
	uint32_t voltage;
	/** Transmit bias current.
	 *  The value is given as an unsigned 32-bit integer with an LSB
	 *  representing a step of 2 uA.
	 *  The valid number range is from 0 to 65535,
	 *  representing physical values from 0 mA to 131 mA.
	 */
	uint32_t bias;
	/** Transmit power.
	 *  The signed value is given in a 2s complement integer format,
	 *  in dBm (referred to 1 mW). The granularity is 0.002 dB.
	 *  The valid value number range is from -20000 to 4082.
	 *  To get dBm divide this value by 500. This will
	 *  represent physical values from -40 dBm (0.1 uW) to
	 *  +8.164 dBm (6.5535 mW).
	 *  A transmit power of 0 mW will be reported as -32767.
	 */
	int32_t tx_power;
	/** Receive power.
	 *  The signed value is given in a 2s complement integer format,
	 *  in dBm (referred to 1 mW). The granularity is 0.002 dB.
	 *  The valid value number range is from -20000 to 4082.
	 *  To get dBm divide this value by 500.
	 *  This will represent physical values from -40 dBm (0.1 uW) to
	 *  +8.164 dBm (6.5535 mW).
	 *  A receive power of 0 mW will be reported as -32767.
	 */
	int32_t rx_power;
	/** Receiver loss of signal status.
	 *  - 0: OK, The received signal level is above the LOS threshold.
	 *  - 1: LOS, The received signal level is below or equal to the LOS
	 *  threshold.
	 */
	uint32_t rx_los;
	/** Transmitter disable status.
	 *  - 0: EN, The transmitter is enabled.
	 *  - 1: DIS, The transmitter is disabled.
	 */
	uint32_t tx_disable;
	/** Transmitter fault status.
	 *  - 0: OK, The transmitter is ok.
	 *  - 1: FAULT, The transmitter is in fault state.
	 */
	uint32_t tx_fault;
};

/** Optical interface static properties information.
 *     Used by \ref fapi_pon_optic_properties_get.
 */
struct pon_optic_properties {
	/** Vendor name.
	 *  ASCII character string.
	 */
	char vendor_name[17];
	/** Vendor OUI.
	 *  The OUI is represented by three binary values.
	 */
	uint8_t vendor_oui[4];
	/** Part number.
	 *  ASCII character string.
	 */
	char part_number[17];
	/** Revision number.
	 *  ASCII character string.
	 */
	char revision[5];
	/** Serial number.
	 *  ASCII character string.
	 */
	char serial_number[17];
	/** Date code.
	 *  ASCII character string.
	 */
	char date_code[9];
	/**
	 * Type of transceiver.
	 *
	 * 00h Unknown or unspecified
	 * 01h GBIC
	 * 02h Module soldered to motherboard (ex: SFF)
	 * 03h SFP or SFP+
	 */
	uint8_t identifier;
	/**
	 * Code for connector type
	 * (see SFF-8024 SFF Module Management Reference Code Tables)
	 */
	uint8_t connector;
	/**
	 * Nominal signaling rate, units of 100 MBd.
	 * (see details for rates > 25.4 GBd)
	 */
	uint32_t signaling_rate;
	/** Transmitter wavelength at room temperature [nm]. */
	uint32_t tx_wavelength;
	/** High Power Level Declaration.
	 *  - 0: LVL12, Identifies standard Power Levels 1 and 2 as indicated by
	 *       power level declaration.
	 *  - 1: LVL3, Identifies Power Level 3 requirement.
	 */
	uint32_t high_power_lvl_decl;
	/** Paging implemented indicator.
	 *  - 0: NO_PGN, Indicates that paging is not implemented.
	 *  - 1: PGN, Indicates that paging is implemented and byte 127 of
	 *       device address 0xA2/0x51 is used for page selection.
	 */
	uint32_t paging_implemented_ind;
	/** Retimer or CDR indicator.
	 *  - 0: NO_RET, Indicates that the transceiver does not provide an
	 *       internal retimer or Clock and Data Recovery (CDR) circuit.
	 *  - 1: RET, Indicates that the transceiver has an internal
	 *       retimer or Clock and Data Recovery (CDR) circuit.
	 */
	uint32_t retimer_ind;
	/** Cooled Transceiver Declaration.
	 *  - 0: UNCOOL, Identifies a conventional uncooled (or unspecified)
	 *       laser implementation.
	 *  - 1: COOL, Identifies a cooled laser transmitter implementation.
	 */
	uint32_t cooled_transceiver_decl;
	/** Power Level Declaration.
	 *  - 0: PL1, Identifies Power Level 1 (or unspecified) requirements.
	 *  - 1: PL2, Identifies Power Level 2 requirement.
	 */
	uint32_t power_lvl_decl;
	/** Linear Receiver Output Implemented.
	 *  - 0: LIM, Identifies a conventional limiting (or unspecified)
	 *       receiver output.
	 *  - 1: LIN, Identifies a linear receiver output.
	 */
	uint32_t linear_rx_output_impl;
	/** Receiver decision threshold implemented.
	 *  - 0: NO_RDT, Indicates that RDT is not implemented.
	 *  - 1: RDT, Indicates that RDT is implemented.
	 */
	uint32_t rx_decision_thr_impl;
	/** Tunable transmitter technology.
	 *  - 0: FIX, Indicates that the transmitter wavelength/frequency is not
	 *       tunable.
	 *  - 1: TUNE, Indicates that the transmitter wavelength/frequency is
	 *       tunable.
	 */
	uint32_t tunable_transmitter;
	/** RATE_SELECT functionality implemented. */
	uint32_t rate_select;
	/** TX_DISABLE implemented and disables the high speed serial output. */
	uint32_t tx_disable;
	/** TX_FAULT implemented. */
	uint32_t tx_fault;
	/** Loss of Signal implemented, signal inverted from standard definition
	 *  in SFP MSA (often called "Signal Detect").
	 */
	uint32_t signal_detect;
	/** Loss of Signal implemented, signal as defined in SFP MSA (often
	 *  called "Rx_LOS").
	 */
	uint32_t rx_los;
	/** Digital monitoring - must be '1' for compliance with this
	 *  document.
	 */
	uint32_t digital_monitoring;
	/** Internally calibrated. */
	uint32_t int_calibrated;
	/** Externally calibrated. */
	uint32_t ext_calibrated;
	/** Received power measurement type.
	 *  - 0: OMA, OMA-based receive power measurement.
	 *  - 1: AVG, Average-based receive power measurement.
	 */
	uint32_t rx_power_measurement_type;
	/** Address change required. */
	uint32_t address_change_req;
	/** Optional Alarm/warning flags implemented for all monitored
	 *  quantities.
	 */
	uint32_t optional_flags_impl;
	/** Optional soft TX_DISABLE control and monitoring implemented. */
	uint32_t soft_tx_disable_monitor;
	/** Optional soft TX_FAULT monitoring implemented. */
	uint32_t soft_tx_fault_monitor;
	/** Optional soft RX_LOS monitoring implemented. */
	uint32_t soft_rx_los_monitor;
	/** Optional soft RATE_SELECT control and monitoring implemented. */
	uint32_t soft_rate_select_monitor;
	/** Optional Application Select control implemented */
	uint32_t app_select_impl;
	/** Optional soft Rate Select control implemented */
	uint32_t soft_rate_select_ctrl_impl;
	/** SFF-8472 compliance.
	 *  - 0: NONE, Not included or undefined.
	 *  - 1: V09_3, SFF-8472 Rev 9.3
	 *  - 2: V09_5, SFF-8472 Rev 9.5
	 *  - 3: V10_2, SFF-8472 Rev 10.2
	 *  - 4: V10_4, SFF-8472 Rev 10.4
	 *  - 5: V11_0, SFF-8472 Rev 11.0
	 *  - 6: V11_3, SFF-8472 Rev 11.3
	 *  - 7: V11_4, SFF-8472 Rev 11.4
	 *  - 8: V12_3, SFF-8472 Rev 12.3
	 *  - 9: V12_4, SFF-8472 Rev 12.4
	 *  - Others: Reserved.
	 */
	uint32_t compliance;
};

/** Get the Time of Day asynchronously. The time zone is defined by the OLT.
 *  Used by \ref fapi_pon_tod_get.
 */
struct pon_tod {
	/** Seconds [0-60] (including up to 1 leap second).*/
	uint32_t sec;
	/** Minutes [0-59].*/
	uint32_t min;
	/** Hours [0-23].*/
	uint32_t hour;
	/** Day [1-31].*/
	uint32_t mday;
	/** Month [1-12].*/
	uint32_t mon;
	/** Year [2010-2xxx].*/
	uint32_t year;
	/** Day of week [0-6].*/
	uint32_t wday;
	/** Days in year [0-365].*/
	uint32_t yday;
	/** Number of seconds, as defined by TAI.*/
	uint32_t sec_tai;
	/** Time of Day quality indication.
	 *  - 0: ASYNC, The ToD hardware is not in synchronization with the OLT.
	 *  - 1: SYNC, The ToD hardware is synchronized to the OLT.
	 */
	uint32_t tod_quality;
};

/** Structure used to configure the Time of Day hardware module in the PON IP.
 *  Used by \ref fapi_pon_tod_cfg_set and \ref fapi_pon_tod_cfg_get.
 *  \remark There are additional Time of Day and 1PPS hardware modules provided
 *  in the LAN-side Ethernet interfaces which are not configured through this
 *  function.
 */
struct pon_tod_cfg {
	/** 1PPS signal pulse width, given in multiples of 100 us.
	 *  A value of 0 disables the 1PPS output.
	 *  The valid range is from 0 to 8191 (819.1 ms).
	 */
	uint32_t pps_width;
	/** 1PPS signal pulse polarity.
	 *  - 0: Low, active low polarity.
	 *  - 1: High, active high polarity (default).
	 *  - Others: Reserved, do not use.
	 */
	int32_t pps_level;
	/** ToD output sub-sampling factor.
	 *  - 0: RES, Invalid setting, do not use.
	 *  - 1: F1, Default factor of 1.
	 *  - 2: F2, Frequency is divided by 2.
	 *  - N: FN, Frequency is divided by N (N = 3 or greater).
	 */
	uint32_t pps_scale;
	/** ToD output delay correction (in ps).
	 *  This is the time for which the hardware shall delay the 1PPS signal.
	 */
	uint32_t pps_delay;
	/** 1PPS debug mode.
	 *  This is used to enable the free-running debug mode of the 1PPS
	 *  signal. The time value is invalid, not synchronized with the OLT.
	 */
	uint32_t pps_debug;
	/** Capture signal polarity for an external trigger signal.
	 *  - 0: Fall, capture with each falling edge of the capture signal.
	 *  - 1: Rise, capture with each rising edge of the capture signal.
	 *  - 2: Both, capture on both edges of the capture signal.
	 */
	int32_t capture_edge;
	/** Upstream time stamping delay correction.
	 *  This is the time in ps to be added to the upstream time stamps
	 *  by the hardware.
	 */
	uint32_t us_ts_corr;
	/** Downstream time stamping delay correction.
	 *  This is the time in ps to be added to the downstream time stamps
	 *  by the hardware.
	 */
	uint32_t ds_ts_corr;
};

/** Debug alarm event message configuration.
 *  This structure is used to enable individual debug alarm types to be sent as
 *  an event message. Used by \ref fapi_pon_debug_alarm_cfg_set and
 *  \ref fapi_pon_debug_alarm_cfg_get.
 */
struct pon_debug_alarm_cfg {
	/** Debug alarm enable.
	 *  - 0: DIS, Debug alarm event message sending is disabled.
	 *  - 1: EN, Debug alarm event message sending is enabled.
	 */
	uint8_t enable;
	/** Debug alarm logging on the PON IP UART.
	 *  - 0: DIS, Debug alarm event is not logged.
	 *  - 1: EN, Debug alarm event is logged.
	 */
	uint8_t logging;
};

/** Alarm event message configuration.
 *  This structure is used to enable individual alarm types to be sent as an
 *  event message. Used by \ref fapi_pon_alarm_cfg_set and
 *  \ref fapi_pon_alarm_cfg_get.
 */
struct pon_alarm_cfg {
	/** Alarm enable.
	 *  - 0: DIS, Alarm event message sending is disabled.
	 *  - 1: EN, Alarm event message sending is enabled.
	 */
	uint8_t enable;
	/** Alarm ID.
	 *  Each alarm is identified by a unique ID and can be enabled or
	 *  disabled individually.
	 */
	uint16_t alarm_id;
};

/** GPON alarm status information for checking alarms of level type.
 *  The alarm status can be checked for each level alarm, identified by its ID.
 *  Used by \ref fapi_pon_alarm_status_get.
 *  In addition, level or edge alarms can be simulated for testing by using
 *  \ref fapi_pon_alarm_status_set.
 */
struct pon_alarm_status {
	/** Alarm status.
	 * - 0: OFF, Alarm is inactive.
	 * - 1: ON, Alarm is active.
	 */
	uint8_t alarm_status;
	/** Alarm ID.
	 *  Each alarm is identified by a unique ID.
	 */
	uint16_t alarm_id;
};

/** Synchronous Ethernet operation mode configuration.
 *  Used by \ref fapi_pon_synce_cfg_set and
 *  \ref fapi_pon_synce_cfg_get.
 */
struct pon_synce_cfg {
	/** Synchronous Ethernet operation mode selection.
	 *  Other operation mode code points are reserved for future use and
	 *  shall not be used.
	 *  - 1: PON, Synchronous Ethernet operation mode is enabled for PON
	 *            operation mode.
	 */
	uint8_t synce_mode;
	/** Loop timing operation mode selection.
	 *  Other operation mode code points shall not be used.
	 *  - 0: MIX, The DPLL loop timing handling is done in hardware, the
	 *            holdover handling is done by firmware.
	 *  - 1: FW, The DPLL loop timing handling and the holdover handling
	 *           is done by firmware.
	 *  - 2: HW, The DPLL loop timing handling and the holdover handling
	 *           is done by hardware.
	 */
	uint8_t loop_mode;
	/** Synchronous Ethernet clock source selection.
	 *  - 2: WAN, The WAN interface is used as clock source.
	 *  - 4: GPC1, The GPC1 general purpose clock input pin is used as
	 *             clock source.
	 *  - 5: GPC2, The GPC2 general purpose clock input pin is used as
	 *             clock source.
	 *  - Other values are reserved and must not be used.
	 */
	uint8_t src;
	/** Synchronous Ethernet clock frequency selection.
	 *  The selection must ensure that the clock provided by the selected
	 *  clock source matches the selected frequency.
	 *  - 0: T1, 1.544 MHz (for GPC1/2 selection only).
	 *  - 1: E1, 2.048 MHz (for GPC1/2 selection only).
	 *  - 2: F10M, 10.0 MHz (for GPC1/2 selection only).
	 *  - 3: F25M, 25.0 MHz (for GPC1/2 selection only).
	 *  - 4: F40M, 40.0 MHz (for GPC1/2 selection only).
	 *  - 7: F155M, 155.52 MHz (for PON operation mode).
	 *  - 11: F622M, 622.08 MHz (for PON operation mode).
	 *  - Others: Reserved.
	 */
	uint8_t frq;
	/** Holdover averaging coefficient.
	 *  This value is used to configure the firmware holdover averaging
	 *  algorithm. The valid range is from 0 to 7.
	 */
	uint32_t n0_avg;
	/** Divider Deviance Threshold.
	 *  This value defines maximum allowed deviance of a divider value
	 *  from its predecessor. Exceeding this threshold leads to hold-over.
	 */
	uint32_t ddt;
	/** Limiter Threshold.
	 *  Frequency difference that shall be handled by the firmware
	 *  holdover algorithm.
	 *  The value is given in multiples of 10e-7 (0.1 ppm per unit). The
	 *  maximum accepted value is 996 ppm, typically used values
	 *  are far below.
	 *  A value of 0 or above the maximum value disables the limiter
	 *  function.
	 */
	uint32_t lim_thr;
	/** IIR Coefficients.
	 *  These values are used to configure the firmware IIR algorithm.
	 *  The 32-bit value is structured as eight 4-bit values as
	 *  iir(31:28) = B0(3:0)
	 *  to
	 *  iir(3:0)   = B7(3:0)
	 */
	uint32_t iir;
};

struct pon_synce_cfg;

/**
 *	Function to enable or disable synchronous Ethernet operation mode.
 *	This function applies to all PON operation modes.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined by
 *	\ref pon_synce_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_synce_cfg_set(struct pon_ctx *ctx,
		       const struct pon_synce_cfg *param);

/**
 *	Function to read the synchronous Ethernet operation mode configuration.
 *	This function applies to all PON operation modes.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_synce_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_synce_cfg_get(struct pon_ctx *ctx,
		       struct pon_synce_cfg *param);

struct pon_synce_status;
/**
 *	Function to read the synchronous Ethernet operation mode status.
 *	This function applies to all PON operation modes.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined by
 *	\ref pon_synce_status.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_synce_status_get(struct pon_ctx *ctx,
			  struct pon_synce_status *param);

/** PON UART output configuration.
 *  Used by \ref fapi_pon_uart_cfg_set and \ref fapi_pon_uart_cfg_get.
 */
struct pon_uart_cfg {
	/** This selects the output function of the UART0/ASC0 output signal. */
	enum pon_uart_pin_mode mode;
};

#include "fapi_pon_error.h"
#include "fapi_pon_gpon.h"
#include "fapi_pon_twdm.h"
#include "fapi_pon_events.h"
#include "fapi_pon_develop.h"
#include "fapi_pon_aon.h"

/** Synchronous Ethernet operation mode status.
 *  Used by \ref fapi_pon_synce_status_get.
 */
struct pon_synce_status {
	/** Synchronous Ethernet operation status.
	 *  This indicates the synchronous Ethernet operation status.
	 *  - 0 OFF, synchronous Ethernet mode is off.
	 *  - 1 SYNCING, synchronous Ethernet mode is trying to synchronize.
	 *  - 2 SYNC, synchronous Ethernet mode is synchronized.
	 *  - 3 HOLD, synchronous Ethernet mode is in hold-over mode.
	 */
	uint8_t stat;
};

/** Structure to define the OLT type to apply OLT-specific
 *  interoperability settings.
 *  This structure is used by \ref fapi_pon_olt_type_set.
 */
struct pon_olt_type {
	/** OLT type identification. */
	enum olt_type type;
};

/* Global PON library function definitions */
/* ======================================= */

/**
 *	Function to create a PON library context which can be used later.
 *
 *	This allocates and initializes the \ref pon_ctx structure.
 *
 *	\param[out] param Pointer to a pointer of a structure as defined
 *                        by \ref pon_ctx.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
enum fapi_pon_errorcode fapi_pon_open(struct pon_ctx **param);
#endif

/**
 *	Function to close and free the PON library context.
 *
 *	\param[in] ctx Pointer to a structure as defined by \ref pon_ctx.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
enum fapi_pon_errorcode fapi_pon_close(struct pon_ctx *ctx);
#endif

/**
 *	Function to retrieve the PON module version information.
 *	Individual version codes are provided for the module hardware,
 *	PON library software, and module firmware.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_version.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_version_get(struct pon_ctx *ctx,
					     struct pon_version *param);

/**
 *	Function to retrieve the PON module capabilities.
 *	A structured value is returned to indicate the supported functions.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined by \ref pon_cap.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_cap_get(struct pon_ctx *ctx,
					 struct pon_cap *param);

/**
 *	Function to check the optical interface status by reading through
 *	the two-wire interface from the PMD device.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_optic_status.
 *	\param[in] scale TX power scaling factor used by the optical module
 *	TX_POWER_SCALE_0_1 = 0.1 uW/LSB, TX_POWER_SCALE_0_2 = 0.2 uW/LSB
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_optic_status_get(struct pon_ctx *ctx,
				struct pon_optic_status *param,
				enum pon_tx_power_scale scale);

/**
 *	Function to check the optical interface properties by reading through
 *	the two-wire interface from the PMD.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_optic_properties.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
	fapi_pon_optic_properties_get(struct pon_ctx *ctx,
				      struct pon_optic_properties *param);

/**
 *	Set the Time of Day configuration in the PON IP hardware module.
 *	\remark There are additional Time of Day and 1PPS hardware modules
 *	provided in the LAN-side Ethernet interfaces which are not configured
 *	through this function.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined
 *	by \ref pon_tod_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_tod_cfg_set(struct pon_ctx *ctx,
					     const struct pon_tod_cfg *param);

/**
 *	Read back the Time of Day configuration from the PON IP hardware module.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_tod_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_tod_cfg_get(struct pon_ctx *ctx,
					     struct pon_tod_cfg *param);

/**
 *	Read the Time of Day, referenced to TAI.
 *	The accuracy is delivered as 1 second, referenced to the latest 1PPS
 *	signal edge.
 *	The time zone of the returned date and time value is UTC, but leap
 *	seconds are not included.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined by \ref pon_tod.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_tod_get(struct pon_ctx *ctx,
					 struct pon_tod *param);

/**
 *	Enable automatic 1PPS event generation.
 *	This is a function to enable the automatic sending of a firmware event
 *	message each time the 1PPS active edge has been detected.
 *	Automatic 1PPS message sending can be disabled by calling
 *	\ref fapi_pon_1pps_event_disable.
 *
 *	By default, 1PPS event generation is disabled.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_1pps_event_enable(struct pon_ctx *ctx);

/**
 *	Disable automatic 1PPS event generation.
 *	This is a function to disable the automatic sending of a firmware event
 *	message each time the 1PPS active edge has been detected.
 *	Automatic 1PPS message sending can be enabled by calling
 *	\ref fapi_pon_1pps_event_enable.
 *
 *	By default, 1PPS event generation is disabled.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_1pps_event_disable(struct pon_ctx *ctx);

/**
 *	Enable the PON interface.
 *	This allows a fiber connection to the OLT.
 *	The PON interface can be disabled by calling
 *	\ref fapi_pon_link_disable.
 *
 *	By default, the PON interface is disabled.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_link_enable(struct pon_ctx *ctx);

/**
 *	Disable the PON interface.
 *	This function disables the connection to an OLT.
 *	Any existing connection is turned down.
 *
 *	By default, the PON interface is disabled.
 *	The PON interface can be enabled by calling
 *	\ref fapi_pon_link_enable.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_link_disable(struct pon_ctx *ctx);

/**
 *	Resets the PON IP hardware.
 *	The PON IP firmware is re-loaded and re-initialized.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] mode The mode which should be used for PON IP. If set
 *		to PON_MODE_UNKNOWN the mode will not be changed.
 *
 *	\return Returns a value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_reset(struct pon_ctx *ctx, enum pon_mode mode);

/**
 *	Function to open the EEPROM-memory-mapped configuration.
 *	This function closes the currently opened DDMI EEPROM file descriptor.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] ddmi_page DDMI memory page address.
 *	\param[in] filename Filename of the DDMI EEPROM file to open.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
enum fapi_pon_errorcode fapi_pon_eeprom_open(struct pon_ctx *ctx,
					     const enum pon_ddmi_page ddmi_page,
					     const char *filename);
#endif

/**
 *	Function to get data from the EEPROM-memory-mapped configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] ddmi_page DDMI memory page address.
 *	\param[in] data Pointer to the EEPROM memory buffer.
 *	\param[in] offset Address offset value.
 *	\param[in] data_size Size of the EEPROM memory buffer.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
enum fapi_pon_errorcode
	fapi_pon_eeprom_data_get(struct pon_ctx *ctx,
				 const enum pon_ddmi_page ddmi_page,
				 unsigned char *data,
				 long offset,
				 size_t data_size);
#endif

/**
 *	Function to set data into the EEPROM-memory-mapped configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] ddmi_page DDMI memory page address.
 *	\param[in] data Pointer to the EEPROM memory buffer.
 *	\param[in] offset Address offset value.
 *	\param[in] data_size Size of the EEPROM memory buffer.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
enum fapi_pon_errorcode
	fapi_pon_eeprom_data_set(struct pon_ctx *ctx,
				 const enum pon_ddmi_page ddmi_page,
				 unsigned char *data,
				 long offset,
				 size_t data_size);
#endif

/**
 *	Function to enable or disable individual debug alarm messages from the
 *	PON IP firmware to the software.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] pon_alarm_id Debug alarm ID.
 *	\param[in] param Pointer to a structure as defined by
 *		\ref pon_debug_alarm_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_debug_alarm_cfg_set(struct pon_ctx *ctx,
			     const uint16_t pon_alarm_id,
			     const struct pon_debug_alarm_cfg *param);

/**
 *	Function to read the debug alarm message configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] pon_alarm_id Debug alarm ID.
 *	\param[out] param Pointer to a structure as defined by
 *		\ref pon_debug_alarm_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_debug_alarm_cfg_get(struct pon_ctx *ctx,
			     uint16_t pon_alarm_id,
			     struct pon_debug_alarm_cfg *param);

/**
 *	Function to enable or disable individual alarm messages from the
 *	PON IP firmware to the software.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined by
 *		\ref pon_alarm_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
	fapi_pon_alarm_cfg_set(struct pon_ctx *ctx,
			       const struct pon_alarm_cfg *param);

/**
 *	Function to read the alarm message configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] pon_alarm_id Alarm ID.
 *	\param[out] param Pointer to a structure as defined
 *		by \ref pon_alarm_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
	fapi_pon_alarm_cfg_get(struct pon_ctx *ctx,
			       uint16_t pon_alarm_id,
			       struct pon_alarm_cfg *param);

/**
 *	Set the optical interface timing configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined
 *	by \ref pon_optic_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
enum fapi_pon_errorcode
fapi_pon_optic_cfg_set(struct pon_ctx *ctx, const struct pon_optic_cfg *param);
#endif

/**
 *	Read the optical interface timing configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_optic_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_optic_cfg_get(struct pon_ctx *ctx, struct pon_optic_cfg *param);

/**
 *	Set the SerDes configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined
 *	by \ref pon_serdes_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_serdes_cfg_set(struct pon_ctx *ctx,
					const struct pon_serdes_cfg *param);

/**
 *	Read the SerDes configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_serdes_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_serdes_cfg_get(struct pon_ctx *ctx,
						struct pon_serdes_cfg *param);

/**
 *	Set the PON-related GPIO pin configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined
 *	by \ref pon_gpio_cfg.
 *
 *	\remarks This function can only be called once and must be called
 *	before \ref fapi_pon_optic_cfg_set!
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
enum fapi_pon_errorcode fapi_pon_gpio_cfg_set(struct pon_ctx *ctx,
					      const struct pon_gpio_cfg *param);
#endif

/**
 *	Read the PON-related GPIO pin configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_gpio_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_gpio_cfg_get(struct pon_ctx *ctx,
					      struct pon_gpio_cfg *param);

/**
 *	Set GPIO port status
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] port_id number of port
 *	\param[in] status status of GPIO port (enable, disable, default)
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_pin_config_set(struct pon_ctx *ctx, enum pon_gpio_pin_id port_id,
			enum pon_gpio_pin_status status);

/**
 *	Set the UART output configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined
 *	by \ref pon_uart_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_uart_cfg_set(struct pon_ctx *ctx, const struct pon_uart_cfg *param);

/**
 *	Read the uart output configuration.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] param Pointer to a structure as defined
 *	by \ref pon_uart_cfg.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_uart_cfg_get(struct pon_ctx *ctx, struct pon_uart_cfg *param);

/**
 *	Grant/Revoke access of a given user to a given command group.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] revoke_grant Option for revoke or granting user access.
 *	\param[in] command_group Command group intended to grant/revoke access.
 *	\param[in] uid_val Linux user ID (uid) value to grant/revoke access.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode
fapi_pon_user_mngmt(struct pon_ctx *ctx,
		    enum pon_perm_revoke_grant revoke_grant,
		    enum pon_perm_command_group command_group,
		    uint32_t uid_val);

/**
 *	Read PON mode.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[out] pon_mode PON mode
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
#ifndef SWIG
enum fapi_pon_errorcode fapi_pon_mode_get(struct pon_ctx *ctx,
					  uint8_t *pon_mode);
#endif

/**
 *	Function to enable synchronous Ethernet operation mode.
 *	This function applies to all PON operation modes.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_synce_enable(struct pon_ctx *ctx);

/**
 *	Function to disable synchronous Ethernet operation mode.
 *	This function applies to all PON operation modes.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_synce_disable(struct pon_ctx *ctx);

/**
 *	Function to force synchronous Ethernet operation into hold mode.
 *	This function applies to all PON operation modes.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_synce_hold_enable(struct pon_ctx *ctx);

/**
 *	Function to force synchronous Ethernet operation out of hold mode.
 *	This function applies to all PON operation modes.
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_synce_hold_disable(struct pon_ctx *ctx);

/**
 *	Function to configure the OLT type interoperability configuration.
 *	This function is applicable to all ITU PON standards
 *	(GPON, XG-PON, XGS-PON, NG-PON2).
 *
 *	\param[in] ctx PON library context created by \ref fapi_pon_open.
 *	\param[in] param Pointer to a structure as defined
 *	by \ref pon_olt_type.
 *	\param[in] initial iop mask if available, that may be used to override
 *	OLT type bits
 *
 *	\remarks The function returns an error code in case of error.
 *	The error code is described in \ref fapi_pon_errorcode.
 *
 *	\return Return value as follows:
 *	- PON_STATUS_OK: If successful
 *	- Other: An error code in case of error.
 */
enum fapi_pon_errorcode fapi_pon_olt_type_set(struct pon_ctx *ctx,
					      const struct pon_olt_type *param,
					      const uint32_t iop_mask);
/*! @} */ /* End of global functions */

/*! @} */ /* End of PON library definitions */

#endif /* _FAPI_PON_H_ */
