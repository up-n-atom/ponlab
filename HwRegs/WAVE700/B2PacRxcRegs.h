
/***********************************************************************************
File:				B2PacRxcRegs.h
Module:				b2PacRxc
SOC Revision:		latest
Generated at:       2022-05-04 10:58:13
Purpose:
Description:		This File was auto generated using SOC Online

************************************************************************************/
#ifndef _B2_PAC_RXC_REGS_H_
#define _B2_PAC_RXC_REGS_H_

/*---------------------------------------------------------------------------------
/						Registers Addresses													 
/----------------------------------------------------------------------------------*/
#include "HwMemoryMap.h"

#define B2_PAC_RXC_BASE_ADDRESS                             MEMORY_MAP_UNIT_20009_BASE_ADDRESS
#define	REG_B2_PAC_RXC_RX_CONTROL                                 (B2_PAC_RXC_BASE_ADDRESS + 0x0)
#define	REG_B2_PAC_RXC_RXC_DLM_PUSH_LIST_IDX                      (B2_PAC_RXC_BASE_ADDRESS + 0x4)
#define	REG_B2_PAC_RXC_RX_BUFFER_OUT_POINTER_WR                   (B2_PAC_RXC_BASE_ADDRESS + 0x8)
#define	REG_B2_PAC_RXC_RX_BUFFER_OUT_POINTER_RD                   (B2_PAC_RXC_BASE_ADDRESS + 0xC)
#define	REG_B2_PAC_RXC_RX_BUFFER_IN_POINTER_RD                    (B2_PAC_RXC_BASE_ADDRESS + 0x10)
#define	REG_B2_PAC_RXC_CCA_IFS_COMPENSATION                       (B2_PAC_RXC_BASE_ADDRESS + 0x14)
#define	REG_B2_PAC_RXC_RXRDY_IFS_COMPENSATION                     (B2_PAC_RXC_BASE_ADDRESS + 0x18)
#define	REG_B2_PAC_RXC_RXC_DBG                                    (B2_PAC_RXC_BASE_ADDRESS + 0x1C)
#define	REG_B2_PAC_RXC_RXC_DBG2                                   (B2_PAC_RXC_BASE_ADDRESS + 0x20)
#define	REG_B2_PAC_RXC_MT_FORCE_CCA                               (B2_PAC_RXC_BASE_ADDRESS + 0x24)
#define	REG_B2_PAC_RXC_RXC_ERR_IRQ_STATUS                         (B2_PAC_RXC_BASE_ADDRESS + 0x28)
#define	REG_B2_PAC_RXC_RXC_ERR_IRQ_EN                             (B2_PAC_RXC_BASE_ADDRESS + 0x2C)
#define	REG_B2_PAC_RXC_RXC_ERR_IRQ_CLR                            (B2_PAC_RXC_BASE_ADDRESS + 0x30)
#define	REG_B2_PAC_RXC_MINIMAL_RSSI_THRESHOLD                     (B2_PAC_RXC_BASE_ADDRESS + 0x34)
#define	REG_B2_PAC_RXC_PHY_MAC_DBG                                (B2_PAC_RXC_BASE_ADDRESS + 0x38)
#define	REG_B2_PAC_RXC_PS_REQ_SETTING_TWT_CTL                     (B2_PAC_RXC_BASE_ADDRESS + 0x3C)
#define	REG_B2_PAC_RXC_FC_PROTOCOL_VERSION_FILTER                 (B2_PAC_RXC_BASE_ADDRESS + 0x40)
#define	REG_B2_PAC_RXC_MAX_HEADER_SIZE                            (B2_PAC_RXC_BASE_ADDRESS + 0x44)
#define	REG_B2_PAC_RXC_RXC_HALT_REQUEST                           (B2_PAC_RXC_BASE_ADDRESS + 0x48)
#define	REG_B2_PAC_RXC_RXC_IN_HALT                                (B2_PAC_RXC_BASE_ADDRESS + 0x4C)
#define	REG_B2_PAC_RXC_TYPE_SUBTYPE_FILTER0                       (B2_PAC_RXC_BASE_ADDRESS + 0x50)
#define	REG_B2_PAC_RXC_TYPE_SUBTYPE_FILTER1                       (B2_PAC_RXC_BASE_ADDRESS + 0x54)
#define	REG_B2_PAC_RXC_MPDU_LENGTH_FILTER_EN                      (B2_PAC_RXC_BASE_ADDRESS + 0x58)
#define	REG_B2_PAC_RXC_MAX_MPDU_LENGTH_11AG_B                     (B2_PAC_RXC_BASE_ADDRESS + 0x5C)
#define	REG_B2_PAC_RXC_MAX_MPDU_LENGTH_11N                        (B2_PAC_RXC_BASE_ADDRESS + 0x60)
#define	REG_B2_PAC_RXC_MAX_MPDU_LENGTH_11AC_AX                    (B2_PAC_RXC_BASE_ADDRESS + 0x64)
#define	REG_B2_PAC_RXC_MIN_MPDU_LENGTH                            (B2_PAC_RXC_BASE_ADDRESS + 0x68)
#define	REG_B2_PAC_RXC_RXC_STATE                                  (B2_PAC_RXC_BASE_ADDRESS + 0x6C)
#define	REG_B2_PAC_RXC_RXC_STATE2                                 (B2_PAC_RXC_BASE_ADDRESS + 0x70)
#define	REG_B2_PAC_RXC_RXC_STATE3                                 (B2_PAC_RXC_BASE_ADDRESS + 0x74)
#define	REG_B2_PAC_RXC_RXC_STATE4                                 (B2_PAC_RXC_BASE_ADDRESS + 0x78)
#define	REG_B2_PAC_RXC_DLM_IF_DBG0                                (B2_PAC_RXC_BASE_ADDRESS + 0x7C)
#define	REG_B2_PAC_RXC_DLM_IF_DBG1                                (B2_PAC_RXC_BASE_ADDRESS + 0x80)
#define	REG_B2_PAC_RXC_DLM_IF_DBG2                                (B2_PAC_RXC_BASE_ADDRESS + 0x84)
#define	REG_B2_PAC_RXC_DEBUG_CTR0                                 (B2_PAC_RXC_BASE_ADDRESS + 0x88)
#define	REG_B2_PAC_RXC_DEBUG_CTR1                                 (B2_PAC_RXC_BASE_ADDRESS + 0x8C)
#define	REG_B2_PAC_RXC_DEBUG_CTR2                                 (B2_PAC_RXC_BASE_ADDRESS + 0x90)
#define	REG_B2_PAC_RXC_DEBUG_CTR3                                 (B2_PAC_RXC_BASE_ADDRESS + 0x94)
#define	REG_B2_PAC_RXC_DEBUG_CTR4                                 (B2_PAC_RXC_BASE_ADDRESS + 0x98)
#define	REG_B2_PAC_RXC_DEBUG_CTR5                                 (B2_PAC_RXC_BASE_ADDRESS + 0x9C)
#define	REG_B2_PAC_RXC_MT_FORCE_CCA_BAND                          (B2_PAC_RXC_BASE_ADDRESS + 0xA0)
#define	REG_B2_PAC_RXC_PHY_MAC_DBG2                               (B2_PAC_RXC_BASE_ADDRESS + 0xA4)
#define	REG_B2_PAC_RXC_DEBUG_CTR_CLEAR                            (B2_PAC_RXC_BASE_ADDRESS + 0xB0)
#define	REG_B2_PAC_RXC_PHY_STATUS_BUF_OCCUPIED_LAST_MPDUS_CTR     (B2_PAC_RXC_BASE_ADDRESS + 0xB4)
#define	REG_B2_PAC_RXC_PHY_STATUS_BUF_INVALID_CTR                 (B2_PAC_RXC_BASE_ADDRESS + 0xB8)
#define	REG_B2_PAC_RXC_BSS_FROM_TO_DS_00_VAP_FILTER_EN            (B2_PAC_RXC_BASE_ADDRESS + 0xBC)
#define	REG_B2_PAC_RXC_BSS_FROM_TO_DS_01_VAP_FILTER_EN            (B2_PAC_RXC_BASE_ADDRESS + 0xC0)
#define	REG_B2_PAC_RXC_BSS_FROM_TO_DS_10_VAP_FILTER_EN            (B2_PAC_RXC_BASE_ADDRESS + 0xC4)
#define	REG_B2_PAC_RXC_BSS_FROM_TO_DS_11_VAP_FILTER_EN            (B2_PAC_RXC_BASE_ADDRESS + 0xC8)
#define	REG_B2_PAC_RXC_A_MPDU_NO_BA_AGREEMENT_FILTER_EN           (B2_PAC_RXC_BASE_ADDRESS + 0xCC)
#define	REG_B2_PAC_RXC_RXF_FIFO_BASE_ADDR                         (B2_PAC_RXC_BASE_ADDRESS + 0xD0)
#define	REG_B2_PAC_RXC_RXF_FIFO_DEPTH_MINUS_ONE                   (B2_PAC_RXC_BASE_ADDRESS + 0xD4)
#define	REG_B2_PAC_RXC_RXF_FIFO_CLEAR_STRB                        (B2_PAC_RXC_BASE_ADDRESS + 0xD8)
#define	REG_B2_PAC_RXC_RXF_FIFO_RD_ENTRIES_NUM                    (B2_PAC_RXC_BASE_ADDRESS + 0xDC)
#define	REG_B2_PAC_RXC_RXF_FIFO_NUM_ENTRIES_COUNT                 (B2_PAC_RXC_BASE_ADDRESS + 0xE0)
#define	REG_B2_PAC_RXC_RXF_FIFO_DEBUG                             (B2_PAC_RXC_BASE_ADDRESS + 0xE4)
#define	REG_B2_PAC_RXC_FRAGMENTED_A_MSDU_FILTER_EN                (B2_PAC_RXC_BASE_ADDRESS + 0xE8)
#define	REG_B2_PAC_RXC_HE_SINGLE_MPDU_FRAGMENT_VAP_FILTER_EN      (B2_PAC_RXC_BASE_ADDRESS + 0xEC)
#define	REG_B2_PAC_RXC_HE_AGG_MPDU_FRAGMENT_VAP_FILTER_EN         (B2_PAC_RXC_BASE_ADDRESS + 0xF0)
#define	REG_B2_PAC_RXC_PHY_STATUS_FIELDS_FILTER_MIN_MAX_N         (B2_PAC_RXC_BASE_ADDRESS + 0xF4)
#define	REG_B2_PAC_RXC_PHY_STATUS_FIELDS_FILTER_EN                (B2_PAC_RXC_BASE_ADDRESS + 0xF8)
#define	REG_B2_PAC_RXC_PHY_STATUS_FIELDS_FILTER_GE_LE_N           (B2_PAC_RXC_BASE_ADDRESS + 0xFC)
#define	REG_B2_PAC_RXC_SENDER_EXPECTED_EN                         (B2_PAC_RXC_BASE_ADDRESS + 0x100)
#define	REG_B2_PAC_RXC_SENDER_EXPECTED_CONTROL                    (B2_PAC_RXC_BASE_ADDRESS + 0x104)
#define	REG_B2_PAC_RXC_ERROR_IFS1                                 (B2_PAC_RXC_BASE_ADDRESS + 0x110)
#define	REG_B2_PAC_RXC_ERROR_IFS2                                 (B2_PAC_RXC_BASE_ADDRESS + 0x114)
#define	REG_B2_PAC_RXC_ERROR_IFS3                                 (B2_PAC_RXC_BASE_ADDRESS + 0x118)
#define	REG_B2_PAC_RXC_ERROR_IFS4                                 (B2_PAC_RXC_BASE_ADDRESS + 0x11C)
#define	REG_B2_PAC_RXC_ERROR_IFS5                                 (B2_PAC_RXC_BASE_ADDRESS + 0x120)
#define	REG_B2_PAC_RXC_SIFS_PLUS_PHY_RX_READY_AIR_TIME_DUR        (B2_PAC_RXC_BASE_ADDRESS + 0x124)
#define	REG_B2_PAC_RXC_NAV_ADJUST                                 (B2_PAC_RXC_BASE_ADDRESS + 0x128)
#define	REG_B2_PAC_RXC_PHY_RX_READY_RISE_DELAY_FROM_START_AIR1    (B2_PAC_RXC_BASE_ADDRESS + 0x12C)
#define	REG_B2_PAC_RXC_PHY_RX_READY_RISE_DELAY_FROM_START_AIR2    (B2_PAC_RXC_BASE_ADDRESS + 0x130)
#define	REG_B2_PAC_RXC_PHY_RX_READY_FALL_SIFS_COMPENSATION        (B2_PAC_RXC_BASE_ADDRESS + 0x134)
#define	REG_B2_PAC_RXC_RXC_ERR_IRQ_PHY_TEST_BUS_STOP_EN           (B2_PAC_RXC_BASE_ADDRESS + 0x13C)
#define	REG_B2_PAC_RXC_RXC_LOGGER                                 (B2_PAC_RXC_BASE_ADDRESS + 0x140)
#define	REG_B2_PAC_RXC_RXC_LOGGER_ACTIVE                          (B2_PAC_RXC_BASE_ADDRESS + 0x144)
#define	REG_B2_PAC_RXC_CRC_CALC_INIT                              (B2_PAC_RXC_BASE_ADDRESS + 0x150)
#define	REG_B2_PAC_RXC_CRC_CALC_CTL                               (B2_PAC_RXC_BASE_ADDRESS + 0x154)
#define	REG_B2_PAC_RXC_CRC_CALC_DATA                              (B2_PAC_RXC_BASE_ADDRESS + 0x158)
#define	REG_B2_PAC_RXC_CRC_CALC_STATUS                            (B2_PAC_RXC_BASE_ADDRESS + 0x15C)
#define	REG_B2_PAC_RXC_CRC_CALC_RESULT_ADDR                       (B2_PAC_RXC_BASE_ADDRESS + 0x160)
#define	REG_B2_PAC_RXC_MAX_NDPA_NDP_TIMER                         (B2_PAC_RXC_BASE_ADDRESS + 0x164)
#define	REG_B2_PAC_RXC_RSSI_NOT_MY_STA_FIFO_EN                    (B2_PAC_RXC_BASE_ADDRESS + 0x168)
#define	REG_B2_PAC_RXC_RSSI_NOT_MY_STA_FIFO_BASE_ADDR             (B2_PAC_RXC_BASE_ADDRESS + 0x16C)
#define	REG_B2_PAC_RXC_RSSI_NOT_MY_STA_FIFO_DEPTH_MINUS_ONE       (B2_PAC_RXC_BASE_ADDRESS + 0x170)
#define	REG_B2_PAC_RXC_RSSI_NOT_MY_STA_FIFO_CLEAR_STRB            (B2_PAC_RXC_BASE_ADDRESS + 0x174)
#define	REG_B2_PAC_RXC_RSSI_NOT_MY_STA_FIFO_RD_ENTRIES_NUM        (B2_PAC_RXC_BASE_ADDRESS + 0x178)
#define	REG_B2_PAC_RXC_RSSI_NOT_MY_STA_FIFO_NUM_ENTRIES_COUNT     (B2_PAC_RXC_BASE_ADDRESS + 0x17C)
#define	REG_B2_PAC_RXC_RSSI_NOT_MY_STA_FIFO_DEBUG                 (B2_PAC_RXC_BASE_ADDRESS + 0x180)
#define	REG_B2_PAC_RXC_OVERRIDE_PHY_DEBUG                         (B2_PAC_RXC_BASE_ADDRESS + 0x184)
#define	REG_B2_PAC_RXC_NAV_TIMER                                  (B2_PAC_RXC_BASE_ADDRESS + 0x188)
#define	REG_B2_PAC_RXC_RX_NAV_TIMER_CFG_SET_VALUE                 (B2_PAC_RXC_BASE_ADDRESS + 0x18C)
#define	REG_B2_PAC_RXC_PHY_NOISE_FLOOR_ANT_3TO0                   (B2_PAC_RXC_BASE_ADDRESS + 0x190)
#define	REG_B2_PAC_RXC_PHY_NOISE_FLOOR_ANT4                       (B2_PAC_RXC_BASE_ADDRESS + 0x194)
#define	REG_B2_PAC_RXC_TIMING_COUNTERS_WR_VALUE                   (B2_PAC_RXC_BASE_ADDRESS + 0x198)
#define	REG_B2_PAC_RXC_RX_TO_ME_AIR_TIME_CTR                      (B2_PAC_RXC_BASE_ADDRESS + 0x19C)
#define	REG_B2_PAC_RXC_RX_ALL_AIR_TIME_CTR                        (B2_PAC_RXC_BASE_ADDRESS + 0x1A0)
#define	REG_B2_PAC_RXC_CCA_20P_BUSY_AIR_TIME_CTR                  (B2_PAC_RXC_BASE_ADDRESS + 0x1A4)
#define	REG_B2_PAC_RXC_RX_NAV_BUSY_AIR_TIME_CTR                   (B2_PAC_RXC_BASE_ADDRESS + 0x1A8)
#define	REG_B2_PAC_RXC_RX_CONTROL2                                (B2_PAC_RXC_BASE_ADDRESS + 0x1AC)
#define	REG_B2_PAC_RXC_RX_STA_VAP_ID_DBG                          (B2_PAC_RXC_BASE_ADDRESS + 0x1B0)
#define	REG_B2_PAC_RXC_RXC_STA_IDX_VALID_USER0TO31                (B2_PAC_RXC_BASE_ADDRESS + 0x1B4)
#define	REG_B2_PAC_RXC_RXC_STA_IDX_VALID_USER32TO35               (B2_PAC_RXC_BASE_ADDRESS + 0x1B8)
#define	REG_B2_PAC_RXC_SPARE_REGISTERS                            (B2_PAC_RXC_BASE_ADDRESS + 0x1FC)
/*---------------------------------------------------------------------------------
/						Data Type Definition										
/----------------------------------------------------------------------------------*/
/*REG_B2_PAC_RXC_RX_CONTROL 0x0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcEnable : 1; //RXC  rx control, reset value: 0x0, access type: RW
		uint32 ignorePhyStatusBufOccupiedLastMpdusCtrs : 1; //Ignore PHY Status buffer occupied_last_mpdus counters, reset value: 0x0, access type: RW
		uint32 rxBufOutPtrRxDmaIncEn : 1; //Enable update RXC buffer out based on Rx DMA monitor address, reset value: 0x1, access type: RW
		uint32 phyRxAbortRssiEn : 1; //Enable abort PHY-MAC Rx session based on RSSI threshold, reset value: 0x0, access type: RW
		uint32 addr13MulticastBroadcastIndEn : 1; //Enable Multicast and Broadcast indications based addresses1/3 in the Rx Circular buffer, reset value: 0x1, access type: RW
		uint32 autoRespFifoUpdateEn : 1; //Enable filling the FIFO Auto response module, reset value: 0x1, access type: RW
		uint32 bsrcFifoUpdateEn : 1; //Enable filling the FIFO of Buffer status report module, reset value: 0x1, access type: RW
		uint32 txcRxAirIdleIncludeInternalDelayEn : 1; //Enable adding to TxC Rx AIR IDLE indication to cover also internal delays in RxC state machines and PIPE operation, reset value: 0x1, access type: RW
		uint32 reservedStorageSize : 4; //RXC  reserved storage size, 4Bytes resolution, reset value: 0x0, access type: RW
		uint32 mbfmFifoUpdateEn : 1; //Enable filling the FIFO of MAC BF module in case of expected BF report, reset value: 0x1, access type: RW
		uint32 bfRptMgmtActionNoAckEn : 1; //Enable indicate BF report valid on Management Action No ACK frame, reset value: 0x1, access type: RW
		uint32 bfRptMgmtActionAckEn : 1; //Enable indicate BF report valid on Management Action ACK frame, reset value: 0x0, access type: RW
		uint32 rxNavSettingEn : 1; //Enable setting Rx NAV timer, reset value: 0x1, access type: RW
		uint32 errorIfsSettingEn : 1; //Enable setting error IFS timer, reset value: 0x1, access type: RW
		uint32 rxNavEifsUnsupportedMcsEn : 1; //Enable setting Rx NAV or EIFS timer due to unsupported MCS, reset value: 0x1, access type: RW
		uint32 cca : 1; //RXC  cca, reset value: 0x1, access type: RO
		uint32 cfEndCmpAddr2ToVapAddrEn : 1; //Enable compare CF-end Address2 to one of the VAP MAC addresses, reset value: 0x1, access type: RW
		uint32 mbfmImplicitFifoUpdateEn : 1; //Enable filling the FIFO of MAC BF module in case of BF implicit metrics, reset value: 0x1, access type: RW
		uint32 mbfmBfeeNdpFifoUpdateEn : 1; //Enable filling the FIFO of MAC BF module in case of BFee PHY NDP metrics, reset value: 0x1, access type: RW
		uint32 baaFifoUpdateEn : 1; //Enable filling the FIFO BAA module, reset value: 0x1, access type: RW
		uint32 sequencerCheckNdpFbResponseEn : 1; //Enable check response bits for Sequencer indication on NDP feedback, reset value: 0x0, access type: RW
		uint32 tfDecFifoUpdateEn : 1; //Enable filling the FIFO TF decoder module, reset value: 0x0, access type: RW
		uint32 rtsClrRxNavTimerEn : 1; //Enable reset Rx NAV timer in case RTS duration timer expired, reset value: 0x1, access type: RW
		uint32 reserved0 : 2;
		uint32 phyRxAbortRssiStatus : 1; //Abort PHY-MAC Rx session based on RSSI threshold indication from RXC to Delineator, reset value: 0x0, access type: RO
		uint32 phyMacRxHaltOutPulseEn : 1; //Enable using the pulse signal from PHY to MAC when going out from halt, which indicates PHY ready to new Tx session., reset value: 0x0, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB2PacRxcRxControl_u;

/*REG_B2_PAC_RXC_RXC_DLM_PUSH_LIST_IDX 0x4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcDlmPushListIdx : 8; //RxC Push list index, supposed to be Rx Classifier IN DLM list index , reset value: 0x1, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB2PacRxcRxcDlmPushListIdx_u;

/*REG_B2_PAC_RXC_RX_BUFFER_OUT_POINTER_WR 0x8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxBufferOutPointerWr : 15; //RXC rx buffer out pointer WR value, reset value: 0x0, access type: RW
		uint32 rxBufferOutPointerWrUsrId : 7; //RXC rx buffer out pointer WR user index, reset value: 0x0, access type: RW
		uint32 reserved0 : 9;
		uint32 rxBufferOutPointerWrSet : 1; //RXC rx buffer out pointer WR set - according to the user ID field, reset value: 0x0, access type: WO
	} bitFields;
} RegB2PacRxcRxBufferOutPointerWr_u;

/*REG_B2_PAC_RXC_RX_BUFFER_OUT_POINTER_RD 0xC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxBufferOutPointerRd : 15; //RXC rx buffer out pointer RD value, reset value: 0x0, access type: RO
		uint32 rxBufferOutPointerRdUsrId : 7; //RXC rx buffer out pointer RD user index, reset value: 0x0, access type: RW
		uint32 reserved0 : 9;
		uint32 rxBufferOutPointerRdSet : 1; //RXC rx buffer out pointer RD set - Reflect the Out pointer to RO register according to the user ID field, reset value: 0x0, access type: WO
	} bitFields;
} RegB2PacRxcRxBufferOutPointerRd_u;

/*REG_B2_PAC_RXC_RX_BUFFER_IN_POINTER_RD 0x10 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxBufferInPointerRd : 15; //RXC rx buffer in pointer RD value, reset value: 0x0, access type: RO
		uint32 rxBufferInPointerRdUsrId : 7; //RXC rx buffer in pointer RD user index, reset value: 0x0, access type: RW
		uint32 reserved0 : 9;
		uint32 rxBufferInPointerRdSet : 1; //RXC rx buffer in pointer RD set - Reflect the In pointer to RO register according to the user ID field, reset value: 0x0, access type: WO
	} bitFields;
} RegB2PacRxcRxBufferInPointerRd_u;

/*REG_B2_PAC_RXC_CCA_IFS_COMPENSATION 0x14 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 ccaIfsCompensation : 16; //RXC  cca ifs compensation, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2PacRxcCcaIfsCompensation_u;

/*REG_B2_PAC_RXC_RXRDY_IFS_COMPENSATION 0x18 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxrdyIfsCompensation : 16; //RXC  rxrdy ifs compensation, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2PacRxcRxrdyIfsCompensation_u;

/*REG_B2_PAC_RXC_RXC_DBG 0x1C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 pacPhyRxGpIrq : 1; //MAC to PHY General purpose IRQ signal, reset value: 0x0, access type: RO
		uint32 cirBufWdataFifoNumWords : 3; //Circular buffer wdata FIFO number of entries, reset value: 0x0, access type: RO
		uint32 hdrCtrlFifoNumWords : 3; //Header control FIFO number of entries, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 pmFifoNumEntries : 8; //PHY-MAC FIFO number of entries, reset value: 0x0, access type: RO
		uint32 pmFifoNumEntriesMax : 8; //Store the maximum of PHY-MAC FIFO number of entries, reset value: 0x0, access type: RO
		uint32 pmFifoNumEntriesMaxClrStb : 1; //Clear maximum PHY-MAC FIFO number of entries, by write '1', reset value: 0x0, access type: WO
		uint32 cfgFifoRxPmClrStb : 1; //PHY-MAC FIFO clear, by write '1' , reset value: 0x0, access type: WO
		uint32 reserved1 : 4;
		uint32 rxPhyStatusBufferId : 1; //Rx PHY status buffer ID of the last Rx session, reset value: 0x1, access type: RO
		uint32 rxPhyStatusBufferValid : 1; //Rx PHY status buffer valid of the last Rx session, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacRxcRxcDbg_u;

/*REG_B2_PAC_RXC_RXC_DBG2 0x20 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcMbfmFifoFull : 1; //MBFM FIFO full indication, reset value: 0x0, access type: RO
		uint32 rxcBsrcFifoFull : 1; //BSRC FIFO full indication, reset value: 0x0, access type: RO
		uint32 rxcAutoRespFifoFull : 1; //Auto-resp FIFO full indication, reset value: 0x0, access type: RO
		uint32 coorFifoFull : 1; //Coordinator FIFO full indication, reset value: 0x0, access type: RO
		uint32 addr2IdxRxcCmdFifoFull : 1; //Addr2index FIFO full indication, reset value: 0x0, access type: RO
		uint32 hdrCtrlFifoFull : 1; //header control FIFO full indication, reset value: 0x0, access type: RO
		uint32 cirBufWdataFifoFull : 1; //circular buffer wdata FIFO full indication, reset value: 0x0, access type: RO
		uint32 rxcBaaFifoFull : 1; //BAA FIFO full indication, reset value: 0x0, access type: RO
		uint32 rxcTfDecFifoFull : 1; //TF decoder FIFO full indication, reset value: 0x0, access type: RO
		uint32 reserved0 : 7;
		uint32 rxcMbfmFifoFullSticky : 1; //MBFM FIFO full sticky indication, reset value: 0x0, access type: RO
		uint32 rxcBsrcFifoFullSticky : 1; //BSRC FIFO full sticky indication, reset value: 0x0, access type: RO
		uint32 rxcAutoRespFifoFullSticky : 1; //Auto-resp FIFO full sticky indication, reset value: 0x0, access type: RO
		uint32 coorFifoFullSticky : 1; //Coordinator FIFO full sticky indication, reset value: 0x0, access type: RO
		uint32 addr2IdxRxcCmdFifoFullSticky : 1; //Addr2index FIFO full sticky indication, reset value: 0x0, access type: RO
		uint32 hdrCtrlFifoFullSticky : 1; //header control FIFO full sticky indication, reset value: 0x0, access type: RO
		uint32 cirBufWdataFifoFullSticky : 1; //circular buffer wdata FIFO full sticky indication, reset value: 0x0, access type: RO
		uint32 dlmOutFifoFullSticky : 1; //DLM Out FIFO full sticky indication, reset value: 0x0, access type: RO
		uint32 rxcBaaFifoFullSticky : 1; //BAA FIFO full sticky indication, reset value: 0x0, access type: RO
		uint32 rxcTfDecFifoFullSticky : 1; //TF decoder FIFO full sticky indication, reset value: 0x0, access type: RO
		uint32 reserved1 : 5;
		uint32 fifoFullStickyClrStb : 1; //Clear FIFO full sticky indications, by write '1', reset value: 0x0, access type: WO
	} bitFields;
} RegB2PacRxcRxcDbg2_u;

/*REG_B2_PAC_RXC_MT_FORCE_CCA 0x24 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 reserved0 : 1;
		uint32 mtForceSwCcaPrimary : 1; //MT Force SW CCA primary, reset value: 0x0, access type: RW
		uint32 mtForcedSwCcaPrimary : 1; //MT SW Forced CCA primary value, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
		uint32 cca20PAdaptMode : 3; //CCA 20MHz primary is adapted according to the following: , 000: Only 20MHz primary CCA signal. , 001: AND between 20MHz primary and 20MHz secondary CCA signals. , 010: AND between 20MHz primary, 20MHz secondary and 40MHz secondary CCA signals. , 011: AND between 20MHz primary, 20MHz secondary, 40MHz secondary and 80MHz secondary CCA signals. , 100: AND between 20MHz primary, 20MHz secondary, 40MHz secondary and 80MHz secondary and 160MHz secondary CCA signals., reset value: 0x0, access type: RW
		uint32 forceCca20PFree : 1; //Force CCA 20MHz primary to be free, reset value: 0x0, access type: RW
		uint32 forceCca20SFree : 1; //Force CCA 20MHz secondary to be free, reset value: 0x0, access type: RW
		uint32 forceCca40SFree : 1; //Force CCA 40MHz secondary to be free, reset value: 0x0, access type: RW
		uint32 forceCca80SFree : 1; //Force CCA 80MHz secondary to be free, reset value: 0x0, access type: RW
		uint32 forceCca160SFree : 1; //Force CCA 20MHz secondary power to be free, reset value: 0x0, access type: RW
		uint32 forceCca20SPwrFree : 1; //no description, reset value: 0x0, access type: RW
		uint32 reserved2 : 17;
	} bitFields;
} RegB2PacRxcMtForceCca_u;

/*REG_B2_PAC_RXC_RXC_ERR_IRQ_STATUS 0x28 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifoRxPmClrWhenNotEmptyIrq : 1; //IRQ of clear of PHY-MAC FIFO when it was not empty, reset value: 0x0, access type: RO
		uint32 rxReadyNegationNoMetricsErrIrq : 1; //IRQ of Rx ready negation without metrics, reset value: 0x0, access type: RO
		uint32 rxGloblNegationMissingMetricsErrIrq : 1; //IRQ of Rx global negation without or missing metrics, reset value: 0x0, access type: RO
		uint32 mpduDropDlmInEmptyCtrNotZero : 1; //Counter of MPDU drop due to DLM in empty is not zero, reset value: 0x0, access type: RO
		uint32 phyMacFifoFullDropCtrNotZero : 1; //Counter of drop from PHY-MAC FIFO is not zero, reset value: 0x0, access type: RO
		uint32 bufferOverflowCtrNotZero : 1; //Counter of circular buffer overflow is not zero, reset value: 0x0, access type: RO
		uint32 rxfFifoFullDropIrq : 1; //RxF drop entry due to FIFO full IRQ, reset value: 0x0, access type: RO
		uint32 rxfFifoDecrementLessThanZeroIrq : 1; //RxF FIFO amount has been decremented less than zero IRQ, reset value: 0x0, access type: RO
		uint32 rxGloblNegationMissingBfMetricsErrIrq : 1; //IRQ of Rx global negation with missing BF metrics, reset value: 0x0, access type: RO
		uint32 rxGloblNegationMissingStatusErrIrq : 1; //IRQ of Rx global negation without or missing status, reset value: 0x0, access type: RO
		uint32 rxGloblNegationMissingStatusErr2Irq : 1; //IRQ of Rx global negation without or missing status, reset value: 0x0, access type: RO
		uint32 rxGloblNegationMissingMetricsErr2Irq : 1; //IRQ of Rx global negation without or missing metrics, reset value: 0x0, access type: RO
		uint32 rssiNotMyStaFifoFullDropIrq : 1; //RSSI Not My STA drop entry due to FIFO full IRQ, reset value: 0x0, access type: RO
		uint32 rssiNotMyStaFifoDecrementLessThanZeroIrq : 1; //RSSI Not My STA FIFO amount has been decremented less than zero IRQ, reset value: 0x0, access type: RO
		uint32 rxGloblNegationMissingNdpFbErrIrq : 1; //IRQ of Rx global negation without or missing NDP FB words, reset value: 0x0, access type: RO
		uint32 rxGloblNegationMissingNdpFbErr2Irq : 1; //IRQ of Rx global negation without or missing NDP FB words, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB2PacRxcRxcErrIrqStatus_u;

/*REG_B2_PAC_RXC_RXC_ERR_IRQ_EN 0x2C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifoRxPmClrWhenNotEmptyIrqEn : 1; //Enable IRQ of clear of PHY-MAC FIFO when it was not empty, reset value: 0x0, access type: RW
		uint32 rxReadyNegationNoMetricsErrIrqEn : 1; //Enable IRQ of Rx ready negation without metrics, reset value: 0x0, access type: RW
		uint32 rxGloblNegationMissingMetricsErrIrqEn : 1; //Enable IRQ of Rx global negation without or missing metrics, reset value: 0x0, access type: RW
		uint32 mpduDropDlmInEmptyCtrNotZeroEn : 1; //Enable IRQ of Counter of MPDU drop due to DLM in empty is not zero, reset value: 0x0, access type: RW
		uint32 phyMacFifoFullDropCtrNotZeroEn : 1; //Enable IRQ of Counter of drop from PHY-MAC FIFO is not zero, reset value: 0x0, access type: RW
		uint32 bufferOverflowCtrNotZeroEn : 1; //Enable IRQ of Counter of circular buffer overflow is not zero, reset value: 0x0, access type: RW
		uint32 rxfFifoFullDropIrqEn : 1; //RxF drop entry due to FIFO full IRQ enable, reset value: 0x0, access type: RW
		uint32 rxfFifoDecrementLessThanZeroIrqEn : 1; //RxF FIFO amount has been decremented less than zero IRQ enable, reset value: 0x0, access type: RW
		uint32 rxGloblNegationMissingBfMetricsErrIrqEn : 1; //Enable IRQ of Rx global negation with missing BF metrics, reset value: 0x0, access type: RW
		uint32 rxGloblNegationMissingStatusErrIrqEn : 1; //Enable IRQ of Rx global negation without or missing status, reset value: 0x0, access type: RW
		uint32 rxGloblNegationMissingStatusErr2IrqEn : 1; //Enable IRQ of Rx global negation without or missing status, reset value: 0x0, access type: RW
		uint32 rxGloblNegationMissingMetricsErr2IrqEn : 1; //Enable IRQ of Rx global negation without or missing metrics, reset value: 0x0, access type: RW
		uint32 rssiNotMyStaFifoFullDropIrqEn : 1; //RSSI Not My STA drop entry due to FIFO full IRQ enable, reset value: 0x0, access type: RW
		uint32 rssiNotMyStaFifoDecrementLessThanZeroIrqEn : 1; //RSSI Not My STA FIFO amount has been decremented less than zero IRQ enable, reset value: 0x0, access type: RW
		uint32 rxGloblNegationMissingNdpFbErrIrqEn : 1; //Enable IRQ of Rx global negation without or missing NDP FB words, reset value: 0x0, access type: RW
		uint32 rxGloblNegationMissingNdpFbErr2IrqEn : 1; //Enable IRQ of Rx global negation without or missing NDP FB words, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2PacRxcRxcErrIrqEn_u;

/*REG_B2_PAC_RXC_RXC_ERR_IRQ_CLR 0x30 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fifoRxPmClrWhenNotEmptyIrqClr : 1; //Clear IRQ of clear of PHY-MAC FIFO when it was not empty, by write '1', reset value: 0x0, access type: WO
		uint32 rxReadyNegationNoMetricsErrIrqClr : 1; //Clear IRQ of Rx ready negation without metrics, by write '1', reset value: 0x0, access type: WO
		uint32 rxGloblNegationMissingMetricsErrIrqClr : 1; //Clear IRQ of Rx global negation without or missing metrics, by write '1', reset value: 0x0, access type: WO
		uint32 reserved0 : 5;
		uint32 rxGloblNegationMissingBfMetricsErrIrqClr : 1; //Clear IRQ of Rx global negation with missing BF metrics, by write '1', reset value: 0x0, access type: WO
		uint32 rxGloblNegationMissingStatusErrIrqClr : 1; //Clear IRQ of Rx global negation without or missing status, by write '1', reset value: 0x0, access type: WO
		uint32 rxGloblNegationMissingStatusErr2IrqClr : 1; //Clear IRQ of Rx global negation without or missing status, by write '1', reset value: 0x0, access type: WO
		uint32 rxGloblNegationMissingMetricsErr2IrqClr : 1; //Clear IRQ of Rx global negation without or missing metrics, by write '1', reset value: 0x0, access type: WO
		uint32 reserved1 : 2;
		uint32 rxGloblNegationMissingNdpFbErrIrqClr : 1; //Clear IRQ of Rx global negation without or missing NDP FB words, by write '1', reset value: 0x0, access type: WO
		uint32 rxGloblNegationMissingNdpFbErr2IrqClr : 1; //Clear IRQ of Rx global negation without or missing NDP FB words, by write '1', reset value: 0x0, access type: WO
		uint32 reserved2 : 16;
	} bitFields;
} RegB2PacRxcRxcErrIrqClr_u;

/*REG_B2_PAC_RXC_MINIMAL_RSSI_THRESHOLD 0x34 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 minimalRssiThreshold : 8; //Minimal RSSI threshold. , Signed value. , Used for setting PHY abort and NAV update., reset value: 0x80, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB2PacRxcMinimalRssiThreshold_u;

/*REG_B2_PAC_RXC_PHY_MAC_DBG 0x38 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyPacRxPhyGlobal : 1; //Active Rx session from phy to mac, reset value: 0x0, access type: RO
		uint32 phyPacRxPhyReady : 1; //PHY MAC Rx ready signal, , From start of Rx session till almost end of SIFS (13usec/7usec after last sample), reset value: 0x0, access type: RO
		uint32 phyPacRxBusState : 4; //PHY MAC Rx bus state, reset value: 0x0, access type: RO
		uint32 phyPacRxDataIndex : 6; //PHY MAC Rx user ID, reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
		uint32 phyMacCca20PFree : 1; //CCA on the 20 primary due to detection or power, reset value: 0x1, access type: RO
		uint32 phyMacCca20SFree : 1; //CCA on the 20 secondary due to detection or power, reset value: 0x1, access type: RO
		uint32 phyMacCca20SPwrFree : 1; //CCA on the 20 secondary due to power only, reset value: 0x1, access type: RO
		uint32 phyMacCca40SFree : 1; //CCA on the 40 secondary due to detection or power, reset value: 0x1, access type: RO
		uint32 phyMacCca80SFree : 1; //CCA on the 80 secondary due to detection or power, reset value: 0x1, access type: RO
		uint32 phyMacCca160SFree : 1; //Regenerated in PRE_AGG, reset value: 0x1, access type: RO
		uint32 reserved1 : 10;
	} bitFields;
} RegB2PacRxcPhyMacDbg_u;

/*REG_B2_PAC_RXC_PS_REQ_SETTING_TWT_CTL 0x3C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 psSettingTwtPsReqCtrlFrameSubtypeEn : 16; //Enable setting PS request for STA in TWT for control frames, 1 bit per subtype, reset value: 0x400, access type: RW
		uint32 psSettingTwtPsReqMgmtFrameEn : 1; //Enable setting PS request for STA in TWT for management frames, reset value: 0x1, access type: RW
		uint32 reserved0 : 15;
	} bitFields;
} RegB2PacRxcPsReqSettingTwtCtl_u;

/*REG_B2_PAC_RXC_FC_PROTOCOL_VERSION_FILTER 0x40 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fcProtocolVersionFilter : 4; //Drop based protocol version field at frame control. 1bit for each value., reset value: 0xe, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2PacRxcFcProtocolVersionFilter_u;

/*REG_B2_PAC_RXC_MAX_HEADER_SIZE 0x44 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 maxHeaderSize : 7; //Maximum WLAN header + Security header byte size, reset value: 0x36, access type: RW
		uint32 reserved0 : 1;
		uint32 maxHeaderFieldsCatcherWordsNum : 6; //Maximum words from header that needed by fields catcher module, reset value: 0x8, access type: RW
		uint32 reserved1 : 18;
	} bitFields;
} RegB2PacRxcMaxHeaderSize_u;

/*REG_B2_PAC_RXC_RXC_HALT_REQUEST 0x48 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcHaltRequest : 1; //Rx Halt request, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2PacRxcRxcHaltRequest_u;

/*REG_B2_PAC_RXC_RXC_IN_HALT 0x4C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 macPhyRxHaltIndication : 1; //MAC to PHY Rx halt indication, reset value: 0x0, access type: RO
		uint32 rxcInHaltFwStatus : 1; //Rx in halt FW status, clear after indication from PHY of complete initialization, reset value: 0x0, access type: RO
		uint32 rxcInHaltFwStatusIrq : 1; //Rx in halt FW status IRQ, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
		uint32 rxcInHaltFwStatusIrqClr : 1; //no description, reset value: 0x0, access type: WO
	} bitFields;
} RegB2PacRxcRxcInHalt_u;

/*REG_B2_PAC_RXC_TYPE_SUBTYPE_FILTER0 0x50 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 type00SubtypeFilter : 16; //Enable {type, subtype} reject filter when type equals 0x0. , 1 bit per subtype. , 1 means to reject., reset value: 0x0, access type: RW
		uint32 type01SubtypeFilter : 16; //Enable {type, subtype} reject filter when type equals 0x1. , 1 bit per subtype. , 1 means to reject., reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacRxcTypeSubtypeFilter0_u;

/*REG_B2_PAC_RXC_TYPE_SUBTYPE_FILTER1 0x54 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 type10SubtypeFilter : 16; //Enable {type, subtype} reject filter when type equals 0x2. , 1 bit per subtype. , 1 means to reject., reset value: 0x0, access type: RW
		uint32 type11SubtypeFilter : 16; //Enable {type, subtype} reject filter when type equals 0x3. , 1 bit per subtype. , 1 means to reject., reset value: 0xffff, access type: RW
	} bitFields;
} RegB2PacRxcTypeSubtypeFilter1_u;

/*REG_B2_PAC_RXC_MPDU_LENGTH_FILTER_EN 0x58 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 maxMpduLengthFilterEn : 1; //Enable Max MPDU size reject filter, reset value: 0x1, access type: RW
		uint32 minMpduLengthFilterEn : 1; //Enable Min MPDU size reject filter, reset value: 0x1, access type: RW
		uint32 reserved0 : 30;
	} bitFields;
} RegB2PacRxcMpduLengthFilterEn_u;

/*REG_B2_PAC_RXC_MAX_MPDU_LENGTH_11AG_B 0x5C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 maxMpduLength11Ag : 14; //Max MPDU size in case of PHY mode 11a/g, reset value: 0xfff, access type: RW
		uint32 reserved0 : 2;
		uint32 maxMpduLength11B : 14; //Max MPDU size in case of PHY mode 11b, reset value: 0x900, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB2PacRxcMaxMpduLength11AgB_u;

/*REG_B2_PAC_RXC_MAX_MPDU_LENGTH_11N 0x60 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 maxMpduLength11NAgg : 14; //Max MPDU size in case of PHY mode 11n aggregated, reset value: 0xfff, access type: RW
		uint32 reserved0 : 2;
		uint32 maxMpduLength11NNonAgg : 14; //Max MPDU size in case of PHY mode 11n non aggregated, reset value: 0x1eff, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB2PacRxcMaxMpduLength11N_u;

/*REG_B2_PAC_RXC_MAX_MPDU_LENGTH_11AC_AX 0x64 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 maxMpduLength11Ac : 14; //Max MPDU size in case of PHY mode 11ac, reset value: 0x2cbe, access type: RW
		uint32 reserved0 : 2;
		uint32 maxMpduLength11Ax : 14; //Max MPDU size in case of PHY mode 11ax, reset value: 0x2cbe, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB2PacRxcMaxMpduLength11AcAx_u;

/*REG_B2_PAC_RXC_MIN_MPDU_LENGTH 0x68 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 minMpduLength : 14; //Min MPDU size (in bytes), reset value: 0xe, access type: RW
		uint32 reserved0 : 2;
		uint32 minTfMpduLength : 14; //Min TF MPDU size (in bytes), reset value: 0x21, access type: RW
		uint32 reserved1 : 2;
	} bitFields;
} RegB2PacRxcMinMpduLength_u;

/*REG_B2_PAC_RXC_RXC_STATE 0x6C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcMainSm : 5; //RxC main state machine, reset value: 0x0, access type: RO
		uint32 loggerSm : 3; //Logger state machine, reset value: 0x0, access type: RO
		uint32 rxcPhyStatusSm : 5; //RxC PHY status state machine, reset value: 0x0, access type: RO
		uint32 cirBufWdataSm : 1; //RxC circular buffer wdata state machine, reset value: 0x0, access type: RO
		uint32 ccaSm : 2; //CCA state machine , reset value: 0x0, access type: RO
		uint32 dlmIfSm : 2; //DLM interface state machine, reset value: 0x0, access type: RO
		uint32 hdrDoneSm : 4; //Header done state machine, reset value: 0x0, access type: RO
		uint32 hdrCtrlSm : 6; //Header control state machine, reset value: 0x0, access type: RO
		uint32 rxcNavSm : 2; //RxC NAV state machine, reset value: 0x0, access type: RO
		uint32 ndpaNdpSm : 2; //NDPA-NDP state machine, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacRxcRxcState_u;

/*REG_B2_PAC_RXC_RXC_STATE2 0x70 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cirBufMonSmUsr0To15 : 32; //Circular buffer monitor state machine - users 0-15, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacRxcRxcState2_u;

/*REG_B2_PAC_RXC_RXC_STATE3 0x74 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cirBufMonSmUsr16To31 : 32; //Circular buffer monitor state machine - users 16-31, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacRxcRxcState3_u;

/*REG_B2_PAC_RXC_RXC_STATE4 0x78 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cirBufMonSmUsr32To35 : 8; //Circular buffer monitor state machine - users 32-35, reset value: 0x0, access type: RO
		uint32 rxcSwFifoSm : 3; //RXC SW FIFOs state machine, reset value: 0x0, access type: RO
		uint32 addr2IdxDoneSm : 3; //Addr2index done state machine, reset value: 0x0, access type: RO
		uint32 rtsClrRxNavTimerSm : 2; //RTS duration clear Rx NAV timer state machine, reset value: 0x0, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB2PacRxcRxcState4_u;

/*REG_B2_PAC_RXC_DLM_IF_DBG0 0x7C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlmInFifoCleanStb : 1; //DLM In FIFO clean by write '1', reset value: 0x0, access type: WO
		uint32 dlmInFifoEmpty : 1; //DLM In FIFO empty, reset value: 0x1, access type: RO
		uint32 dlmInFifoNumEntries : 3; //DLM In FIFO number of entries, reset value: 0x0, access type: RO
		uint32 dlmInFifoRdPtr : 2; //DLM In FIFO read pointer, reset value: 0x0, access type: RO
		uint32 reserved0 : 9;
		uint32 dlmOutFifoCleanStb : 1; //DLM out FIFO clean by write '1', reset value: 0x0, access type: WO
		uint32 dlmOutFifoFull : 1; //DLM out FIFO full, reset value: 0x0, access type: RO
		uint32 dlmOutFifoNumEntries : 4; //DLM out FIFO number of entries, reset value: 0x0, access type: RO
		uint32 reserved1 : 10;
	} bitFields;
} RegB2PacRxcDlmIfDbg0_u;

/*REG_B2_PAC_RXC_DLM_IF_DBG1 0x80 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlmInFifoEntry0 : 16; //DLM In FIFO entry 0, reset value: 0x0, access type: RO
		uint32 dlmInFifoEntry1 : 16; //DLM In FIFO entry 1, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacRxcDlmIfDbg1_u;

/*REG_B2_PAC_RXC_DLM_IF_DBG2 0x84 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 dlmInFifoEntry2 : 16; //DLM In FIFO entry 2, reset value: 0x0, access type: RO
		uint32 dlmInFifoEntry3 : 16; //DLM In FIFO entry 3, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacRxcDlmIfDbg2_u;

/*REG_B2_PAC_RXC_DEBUG_CTR0 0x88 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyAbortCtr : 16; //PHY abort counter, reset value: 0x0, access type: RO
		uint32 phyHaltCtr : 16; //PHY halt counter, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacRxcDebugCtr0_u;

/*REG_B2_PAC_RXC_DEBUG_CTR1 0x8C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bufferOverflowCtr : 16; //Buffer overflow counter, reset value: 0x0, access type: RO
		uint32 phyErrorCtr : 16; //PHY error counter, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacRxcDebugCtr1_u;

/*REG_B2_PAC_RXC_DEBUG_CTR2 0x90 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyMacFifoFullDropCtr : 16; //PHY-MAC FIFO full drop counter, reset value: 0x0, access type: RO
		uint32 mpduDropDlmInEmptyCtr : 16; //MPDU drop due to DLM In list empty counter, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacRxcDebugCtr2_u;

/*REG_B2_PAC_RXC_DEBUG_CTR3 0x94 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 mpduDropLengthViolCtr : 16; //MPDU drop due to length violation counter, reset value: 0x0, access type: RO
		uint32 goodDelimiterCtr : 16; //Good delimiter counter, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacRxcDebugCtr3_u;

/*REG_B2_PAC_RXC_DEBUG_CTR4 0x98 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcUnsupportedMcsCtr : 16; //Unsupported MCS counter, reset value: 0x0, access type: RO
		uint32 mpduEndErrorCtr : 16; //MPDU end error counter, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacRxcDebugCtr4_u;

/*REG_B2_PAC_RXC_DEBUG_CTR5 0x9C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyMacRxSessionCtr : 16; //PHY-MAC Rx session counter, reset value: 0x0, access type: RO
		uint32 phyNdpCtr : 16; //PHY NDP counter, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacRxcDebugCtr5_u;

/*REG_B2_PAC_RXC_MT_FORCE_CCA_BAND 0xA0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 forceCcaBandBusy : 16; //Force PHY-MAC CCA band signals to be busy, 1 bit per band, reset value: 0x0, access type: RW
		uint32 forceCcaBandFree : 16; //Force PHY-MAC CCA band signals to be free, 1 bit per band, reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacRxcMtForceCcaBand_u;

/*REG_B2_PAC_RXC_PHY_MAC_DBG2 0xA4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyMacCcaBandsFree : 16; //PHY 8 CCA on each 20Mhz due to detection or power for noncontiguous HE, reset value: 0xffff, access type: RO
		uint32 reserved0 : 16;
	} bitFields;
} RegB2PacRxcPhyMacDbg2_u;

/*REG_B2_PAC_RXC_DEBUG_CTR_CLEAR 0xB0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bufferOverflowCtrClear : 1; //Buffer overflow counter clear, by write '1', reset value: 0x0, access type: WO
		uint32 phyErrorCtrClrStb : 1; //PHY error counter clear, by write '1', reset value: 0x0, access type: WO
		uint32 phyAbortCtrClrStb : 1; //PHY abort counter clear, by write '1', reset value: 0x0, access type: WO
		uint32 phyHaltCtrClrStb : 1; //PHY halt counter clear, by write '1', reset value: 0x0, access type: WO
		uint32 phyMacFifoFullDropCtrClrStb : 1; //PHY-MAC FIFO full drop counter clear, by write '1', reset value: 0x0, access type: WO
		uint32 mpduDropDlmInEmptyCtrClrStb : 1; //MPDU drop due to DLM in list empty counter clear, by write '1', reset value: 0x0, access type: WO
		uint32 mpduDropLengthViolCtrClrStb : 1; //MPDU drop due to length violation counter clear, by write '1', reset value: 0x0, access type: WO
		uint32 goodDelimiterCtrClrStb : 1; //Good delimiter counter clear, by write '1', reset value: 0x0, access type: WO
		uint32 rxcUnsupportedMcsCtrClrStb : 1; //Unsupported MCS counter clear, by write '1', reset value: 0x0, access type: WO
		uint32 mpduEndErrorCtrClrStb : 1; //MPDU end error counter clear, by write '1', reset value: 0x0, access type: WO
		uint32 phyMacRxSessionCtrClrStb : 1; //PHY-MAC Rx session counter clear, by write '1', reset value: 0x0, access type: WO
		uint32 phyNdpCtrClrStb : 1; //PHY NDP counter clear, by write '1', reset value: 0x0, access type: WO
		uint32 reserved0 : 20;
	} bitFields;
} RegB2PacRxcDebugCtrClear_u;

/*REG_B2_PAC_RXC_PHY_STATUS_BUF_OCCUPIED_LAST_MPDUS_CTR 0xB4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyStatusBuf0OccupiedLastMpdusCtr : 7; //PHY status buffer 0 occupied last MPDUs counter, reset value: 0x0, access type: RO
		uint32 reserved0 : 1;
		uint32 phyStatusBuf1OccupiedLastMpdusCtr : 7; //PHY status buffer 1 occupied last MPDUs counter, reset value: 0x0, access type: RO
		uint32 reserved1 : 9;
		uint32 phyStatusBuf0OccupiedLastMpdusDecStb : 1; //PHY status buffer 0 occupied last MPDUs decrement counter, reset value: 0x0, access type: WO
		uint32 phyStatusBuf1OccupiedLastMpdusDecStb : 1; //PHY status buffer 1 occupied last MPDUs decrement counter, reset value: 0x0, access type: WO
		uint32 reserved2 : 6;
	} bitFields;
} RegB2PacRxcPhyStatusBufOccupiedLastMpdusCtr_u;

/*REG_B2_PAC_RXC_PHY_STATUS_BUF_INVALID_CTR 0xB8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyStatusBufInvalidCtr : 16; //counter of number invalid PHY status buffer, reset value: 0x0, access type: RO
		uint32 reserved0 : 15;
		uint32 phyStatusBufInvalidCtrClrStb : 1; //clear counter of number invalid PHY status buffer, reset value: 0x0, access type: WO
	} bitFields;
} RegB2PacRxcPhyStatusBufInvalidCtr_u;

/*REG_B2_PAC_RXC_BSS_FROM_TO_DS_00_VAP_FILTER_EN 0xBC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bssFromToDs00VapFilterEn : 32; //Enable BSS reject filter for {from_ds, to_ds} == 00. , Per VAP., reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacRxcBssFromToDs00VapFilterEn_u;

/*REG_B2_PAC_RXC_BSS_FROM_TO_DS_01_VAP_FILTER_EN 0xC0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bssFromToDs01VapFilterEn : 32; //Enable BSS reject filter for {from_ds, to_ds} == 01. , Per VAP., reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacRxcBssFromToDs01VapFilterEn_u;

/*REG_B2_PAC_RXC_BSS_FROM_TO_DS_10_VAP_FILTER_EN 0xC4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bssFromToDs10VapFilterEn : 32; //Enable BSS reject filter for {from_ds, to_ds} == 10. , Per VAP., reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacRxcBssFromToDs10VapFilterEn_u;

/*REG_B2_PAC_RXC_BSS_FROM_TO_DS_11_VAP_FILTER_EN 0xC8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bssFromToDs11VapFilterEn : 32; //Enable BSS reject filter for {from_ds, to_ds} == 11. , Per VAP., reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacRxcBssFromToDs11VapFilterEn_u;

/*REG_B2_PAC_RXC_A_MPDU_NO_BA_AGREEMENT_FILTER_EN 0xCC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 aMpduNoBaAgreementFilterEn : 1; //Enable A-MPDU with no BA agreement reject filter, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2PacRxcAMpduNoBaAgreementFilterEn_u;

/*REG_B2_PAC_RXC_RXF_FIFO_BASE_ADDR 0xD0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxfFifoBaseAddr : 22; //RXF FIFO base address, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB2PacRxcRxfFifoBaseAddr_u;

/*REG_B2_PAC_RXC_RXF_FIFO_DEPTH_MINUS_ONE 0xD4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxfFifoDepthMinusOne : 8; //RXF FIFO depth minus one, reset value: 0x0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB2PacRxcRxfFifoDepthMinusOne_u;

/*REG_B2_PAC_RXC_RXF_FIFO_CLEAR_STRB 0xD8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxfFifoClearStrb : 1; //Clear RXF FIFO. , By write '1'., reset value: 0x0, access type: WO
		uint32 rxfFifoClearFullDropCtrStrb : 1; //Clear RXF FIFO full drop counter. , By write '1'., reset value: 0x0, access type: WO
		uint32 rxfFifoClearDecLessThanZeroStrb : 1; //Clear RXF FIFO decrement amount less than zero indication. , By write '1'.  , , reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegB2PacRxcRxfFifoClearStrb_u;

/*REG_B2_PAC_RXC_RXF_FIFO_RD_ENTRIES_NUM 0xDC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxfFifoRdEntriesNum : 9; //RXF FIFO number of entries to decrement., reset value: 0x0, access type: RW
		uint32 reserved0 : 23;
	} bitFields;
} RegB2PacRxcRxfFifoRdEntriesNum_u;

/*REG_B2_PAC_RXC_RXF_FIFO_NUM_ENTRIES_COUNT 0xE0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxfFifoNumEntriesCount : 9; //RXF FIFO number of entries count, reset value: 0x0, access type: RO
		uint32 reserved0 : 23;
	} bitFields;
} RegB2PacRxcRxfFifoNumEntriesCount_u;

/*REG_B2_PAC_RXC_RXF_FIFO_DEBUG 0xE4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxfFifoWrPtr : 8; //RXF FIFO write pointer index, reset value: 0x0, access type: RO
		uint32 reserved0 : 8;
		uint32 rxfFifoNotEmpty : 1; //RXF FIFO not empty indication, reset value: 0x0, access type: RO
		uint32 rxfFifoFull : 1; //RXF FIFO full indication, reset value: 0x0, access type: RO
		uint32 rxfFifoDecrementLessThanZero : 1; //RXF setting FIFO amount has been decremented less than zero, reset value: 0x0, access type: RO
		uint32 reserved1 : 5;
		uint32 rxfFifoFullDropCtr : 8; //RXF setting FIFO full drop counter, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacRxcRxfFifoDebug_u;

/*REG_B2_PAC_RXC_FRAGMENTED_A_MSDU_FILTER_EN 0xE8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 fragmentedAMsduFilterEn : 1; //Enable fragmented A-MSDU reject filter, reset value: 0x1, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2PacRxcFragmentedAMsduFilterEn_u;

/*REG_B2_PAC_RXC_HE_SINGLE_MPDU_FRAGMENT_VAP_FILTER_EN 0xEC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heSingleMpduFragmentVapFilterEn : 32; //Enable HE single MPDU fragment filter. 1 bit per VAP., reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacRxcHeSingleMpduFragmentVapFilterEn_u;

/*REG_B2_PAC_RXC_HE_AGG_MPDU_FRAGMENT_VAP_FILTER_EN 0xF0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heAggMpduFragmentVapFilterEn : 32; //Enable HE non-single MPDU fragment filter. 1 bit per VAP., reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacRxcHeAggMpduFragmentVapFilterEn_u;

/*REG_B2_PAC_RXC_PHY_STATUS_FIELDS_FILTER_MIN_MAX_N 0xF4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyStatusFieldsFdRssiFilterMinMaxN : 1; //PHY status FD RSSI field minimum or maximum between the antennas fields: , 0 - Maximum , 1 - Minimum, reset value: 0x0, access type: RW
		uint32 phyStatusFieldsTdRssiFilterMinMaxN : 1; //PHY status TD RSSI field minimum or maximum between the antennas fields: , 0 - Maximum , 1 - Minimum, reset value: 0x0, access type: RW
		uint32 phyStatusFieldsRfGainFilterMinMaxN : 1; //PHY status RF Gain field minimum or maximum between the antennas fields: , 0 - Maximum , 1 - Minimum, reset value: 0x0, access type: RW
		uint32 phyStatusFieldsNoiseEstFilterMinMaxN : 1; //PHY status Noise estimation field minimum or maximum between the antennas fields: , 0 - Maximum , 1 - Minimum, reset value: 0x0, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2PacRxcPhyStatusFieldsFilterMinMaxN_u;

/*REG_B2_PAC_RXC_PHY_STATUS_FIELDS_FILTER_EN 0xF8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyStatusFieldsFdRssiFilterEn : 8; //PHY status FD RSSI field filter enable.  , 1bit per Group index., reset value: 0x0, access type: RW
		uint32 phyStatusFieldsTdRssiFilterEn : 8; //PHY status TD RSSI field filter enable.  , 1bit per Group index., reset value: 0x0, access type: RW
		uint32 phyStatusFieldsRfGainFilterEn : 8; //PHY status RF Gain field filter enable.  , 1bit per Group index., reset value: 0x0, access type: RW
		uint32 phyStatusFieldsNoiseEstFilterEn : 8; //PHY status Noise estimation field filter enable.  , 1bit per Group index., reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacRxcPhyStatusFieldsFilterEn_u;

/*REG_B2_PAC_RXC_PHY_STATUS_FIELDS_FILTER_GE_LE_N 0xFC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyStatusFieldsFdRssiFilterGeLeN : 8; //PHY status FD RSSI field filter operation: , 0: Less or equal signed comparison (<=). , 1: Greater or equal signed comparison (>=). , 1bit per Group index., reset value: 0x0, access type: RW
		uint32 phyStatusFieldsTdRssiFilterGeLeN : 8; //PHY status TD RSSI field filter operation: , 0: Less or equal signed comparison (<=). , 1: Greater or equal signed comparison (>=). , 1bit per Group index., reset value: 0x0, access type: RW
		uint32 phyStatusFieldsRfGainFilterGeLeN : 8; //PHY status RF Gain field filter operation: , 0: Less or equal signed comparison (<=). , 1: Greater or equal signed comparison (>=). , 1bit per Group index., reset value: 0x0, access type: RW
		uint32 phyStatusFieldsNoiseEstFilterGeLeN : 8; //PHY status Noise estimation field filter operation: , 0: Less or equal signed comparison (<=). , 1: Greater or equal signed comparison (>=). , 1bit per Group index., reset value: 0x0, access type: RW
	} bitFields;
} RegB2PacRxcPhyStatusFieldsFilterGeLeN_u;

/*REG_B2_PAC_RXC_SENDER_EXPECTED_EN 0x100 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 heMuExpectedReceptionFilterEn : 1; //Enable HE MU reception reject filter, reset value: 0x1, access type: RW
		uint32 seqSenderExpectedCompareVapIdxEn : 1; //Enable compare sender expected VAP index for sequencer result, reset value: 0x1, access type: RW
		uint32 seqSenderExpectedCompareStaIdxEn : 1; //Enable compare sender expected STA index for sequencer result, reset value: 0x1, access type: RW
		uint32 reserved0 : 29;
	} bitFields;
} RegB2PacRxcSenderExpectedEn_u;

/*REG_B2_PAC_RXC_SENDER_EXPECTED_CONTROL 0x104 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 senderExpAckCtsAddr2IdxValid : 1; //Sender expected Addr2 index is valid value, reset value: 0x0, access type: RW
		uint32 senderExpMultibssidValid : 1; //Sender expected MultiBSSID transmitted MAC address valid indication, reset value: 0x0, access type: RW
		uint32 reserved0 : 6;
		uint32 senderExpMultibssidVapId : 5; //Sender expected MultiBSSID transmitted VAP ID, reset value: 0x0, access type: RW
		uint32 reserved1 : 19;
	} bitFields;
} RegB2PacRxcSenderExpectedControl_u;

/*REG_B2_PAC_RXC_ERROR_IFS1 0x110 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 errorIfs11BShort : 13; //error IFS for last Rx in PHY mode 11b short, 0.125[us] resolution, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 errorIfs11BLong : 13; //error IFS for last Rx in PHY mode 11b long, 0.125[us] resolution, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
	} bitFields;
} RegB2PacRxcErrorIfs1_u;

/*REG_B2_PAC_RXC_ERROR_IFS2 0x114 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 errorIfs11Ag : 13; //error IFS for last Rx in PHY mode 11a/g, 0.125[us] resolution, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 errorIfs11N : 13; //error IFS for last Rx in PHY mode 11n, 0.125[us] resolution, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
	} bitFields;
} RegB2PacRxcErrorIfs2_u;

/*REG_B2_PAC_RXC_ERROR_IFS3 0x118 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 errorIfs11Ac : 13; //error IFS for last Rx in PHY mode 11ac, 0.125[us] resolution, reset value: 0x0, access type: RW
		uint32 reserved0 : 19;
	} bitFields;
} RegB2PacRxcErrorIfs3_u;

/*REG_B2_PAC_RXC_ERROR_IFS4 0x11C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 errorIfs11AxSu : 13; //error IFS for last Rx in PHY mode 11ax SU, 0.125[us] resolution, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 errorIfs11AxSuEx : 13; //error IFS for last Rx in PHY mode 11ax SU EX, 0.125[us] resolution, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
	} bitFields;
} RegB2PacRxcErrorIfs4_u;

/*REG_B2_PAC_RXC_ERROR_IFS5 0x120 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 errorIfs11AxMuTb : 13; //error IFS for last Rx in PHY mode 11ax MU Trigger based, 0.125[us] resolution, reset value: 0x0, access type: RW
		uint32 reserved0 : 3;
		uint32 errorIfs11AxMuDl : 13; //error IFS for last Rx in PHY mode 11ax MU DL, 0.125[us] resolution, reset value: 0x0, access type: RW
		uint32 reserved1 : 3;
	} bitFields;
} RegB2PacRxcErrorIfs5_u;

/*REG_B2_PAC_RXC_SIFS_PLUS_PHY_RX_READY_AIR_TIME_DUR 0x124 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 sifsPlusPhyRxDelayAirTime11Ag : 8; //SIFS + PHY RX Delay, when the incoming Rx frame is PHY mode 11a/g/n/ac [microseconds]., reset value: 0x0, access type: RW
		uint32 sifsPlusPhyRxDelayAirTime11BShort : 8; //SIFS + PHY RX Delay, when the incoming Rx frame is PHY mode 11b short preamble [microseconds], reset value: 0x0, access type: RW
		uint32 sifsPlusPhyRxDelayAirTime11BLong : 8; //SIFS + PHY RX Delay, when the incoming Rx frame is PHY mode 11b long preamble [microseconds], reset value: 0x0, access type: RW
		uint32 reserved0 : 8;
	} bitFields;
} RegB2PacRxcSifsPlusPhyRxReadyAirTimeDur_u;

/*REG_B2_PAC_RXC_NAV_ADJUST 0x128 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 navAdjust : 7; //DUR  nav adjust, reset value: 0x0, access type: RW
		uint32 reserved0 : 25;
	} bitFields;
} RegB2PacRxcNavAdjust_u;

/*REG_B2_PAC_RXC_PHY_RX_READY_RISE_DELAY_FROM_START_AIR1 0x12C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyRxReadyRiseDelayFromStartAirHt : 7; //delay in [us] from start air busy till Rx ready rise in PHY mode HT, reset value: 0x36, access type: RW
		uint32 reserved0 : 1;
		uint32 phyRxReadyRiseDelayFromStartAirVht : 7; //delay in [us] from start air busy till Rx ready rise in PHY mode VHT, reset value: 0x36, access type: RW
		uint32 reserved1 : 1;
		uint32 phyRxReadyRiseDelayFromStartAirHeSu : 7; //delay in [us] from start air busy till Rx ready rise in PHY mode HE SU, reset value: 0x40, access type: RW
		uint32 reserved2 : 1;
		uint32 phyRxReadyRiseDelayFromStartAirHeSuEx : 7; //delay in [us] from start air busy till Rx ready rise in PHY mode HE SU EX, reset value: 0x48, access type: RW
		uint32 reserved3 : 1;
	} bitFields;
} RegB2PacRxcPhyRxReadyRiseDelayFromStartAir1_u;

/*REG_B2_PAC_RXC_PHY_RX_READY_RISE_DELAY_FROM_START_AIR2 0x130 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyRxReadyRiseDelayFromStartAirHeMuTb : 7; //delay in [us] from start air busy till Rx ready rise in PHY mode HE MU TB, reset value: 0x40, access type: RW
		uint32 reserved0 : 1;
		uint32 phyRxReadyRiseDelayFromStartAirHeMuDl : 7; //delay in [us] from start air busy till Rx ready rise in PHY mode HE MU DL, reset value: 0x40, access type: RW
		uint32 reserved1 : 17;
	} bitFields;
} RegB2PacRxcPhyRxReadyRiseDelayFromStartAir2_u;

/*REG_B2_PAC_RXC_PHY_RX_READY_FALL_SIFS_COMPENSATION 0x134 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyRxReadyFallSifsCompensationOfdm : 5; //SIFS compensation in [us] from Rx ready fall, in PHY mode non 11B, reset value: 0x13, access type: RW
		uint32 reserved0 : 27;
	} bitFields;
} RegB2PacRxcPhyRxReadyFallSifsCompensation_u;

/*REG_B2_PAC_RXC_RXC_ERR_IRQ_PHY_TEST_BUS_STOP_EN 0x13C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcErrIrqPhyTestBusStopEn : 16; //Enable stop PHY-MAC I/F test bus, 1 bit per RxC error IRQ, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2PacRxcRxcErrIrqPhyTestBusStopEn_u;

/*REG_B2_PAC_RXC_RXC_LOGGER 0x140 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcLoggerEn : 1; //RxC logger enable, reset value: 0x0, access type: RW
		uint32 reserved0 : 7;
		uint32 rxcLoggerPriority : 2; //RxC logger priority, reset value: 0x0, access type: RW
		uint32 reserved1 : 22;
	} bitFields;
} RegB2PacRxcRxcLogger_u;

/*REG_B2_PAC_RXC_RXC_LOGGER_ACTIVE 0x144 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcLoggerActive : 1; //RxC logger active, reset value: 0x0, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB2PacRxcRxcLoggerActive_u;

/*REG_B2_PAC_RXC_CRC_CALC_INIT 0x150 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crcCalcInit : 1; //CRC calculator init by write 0x1, reset value: 0x0, access type: WO
		uint32 reserved0 : 31;
	} bitFields;
} RegB2PacRxcCrcCalcInit_u;

/*REG_B2_PAC_RXC_CRC_CALC_CTL 0x154 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crcByteEnable : 4; //CRC byte enables, reset value: 0xf, access type: RW
		uint32 reserved0 : 28;
	} bitFields;
} RegB2PacRxcCrcCalcCtl_u;

/*REG_B2_PAC_RXC_CRC_CALC_DATA 0x158 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crcDataInOut : 32; //CRC read/write data, reset value: 0x0, access type: WO
	} bitFields;
} RegB2PacRxcCrcCalcData_u;

/*REG_B2_PAC_RXC_CRC_CALC_STATUS 0x15C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crcCalcError : 1; //CRC error, reset value: 0x1, access type: RO
		uint32 reserved0 : 31;
	} bitFields;
} RegB2PacRxcCrcCalcStatus_u;

/*REG_B2_PAC_RXC_CRC_CALC_RESULT_ADDR 0x160 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 crcCalcResult : 32; //CRC result, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacRxcCrcCalcResultAddr_u;

/*REG_B2_PAC_RXC_MAX_NDPA_NDP_TIMER 0x164 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 maxNdpaNdpTimer : 8; //Max NDPA NDP timer, resolution of [us], reset value: 0xb0, access type: RW
		uint32 reserved0 : 24;
	} bitFields;
} RegB2PacRxcMaxNdpaNdpTimer_u;

/*REG_B2_PAC_RXC_RSSI_NOT_MY_STA_FIFO_EN 0x168 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rssiNotMyStaFifoEn : 1; //Enable filling RSSI Not My STA FIFO, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2PacRxcRssiNotMyStaFifoEn_u;

/*REG_B2_PAC_RXC_RSSI_NOT_MY_STA_FIFO_BASE_ADDR 0x16C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rssiNotMyStaFifoBaseAddr : 22; //RSSI Not My STA FIFO base address, reset value: 0x0, access type: RW
		uint32 reserved0 : 10;
	} bitFields;
} RegB2PacRxcRssiNotMyStaFifoBaseAddr_u;

/*REG_B2_PAC_RXC_RSSI_NOT_MY_STA_FIFO_DEPTH_MINUS_ONE 0x170 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rssiNotMyStaFifoDepthMinusOne : 10; //RSSI Not My STA FIFO depth minus one, reset value: 0x0, access type: RW
		uint32 reserved0 : 22;
	} bitFields;
} RegB2PacRxcRssiNotMyStaFifoDepthMinusOne_u;

/*REG_B2_PAC_RXC_RSSI_NOT_MY_STA_FIFO_CLEAR_STRB 0x174 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rssiNotMyStaFifoClearStrb : 1; //Clear RSSI Not My STA FIFO. , By write '1'., reset value: 0x0, access type: WO
		uint32 rssiNotMyStaFifoClearFullDropCtrStrb : 1; //Clear RSSI Not My STA FIFO full drop counter. , By write '1'., reset value: 0x0, access type: WO
		uint32 rssiNotMyStaFifoClearDecLessThanZeroStrb : 1; //Clear RSSI Not My STA FIFO decrement amount less than zero indication. , By write '1'.  , , reset value: 0x0, access type: WO
		uint32 reserved0 : 29;
	} bitFields;
} RegB2PacRxcRssiNotMyStaFifoClearStrb_u;

/*REG_B2_PAC_RXC_RSSI_NOT_MY_STA_FIFO_RD_ENTRIES_NUM 0x178 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rssiNotMyStaFifoRdEntriesNum : 11; //RSSI Not My STA FIFO number of entries to decrement., reset value: 0x0, access type: RW
		uint32 reserved0 : 21;
	} bitFields;
} RegB2PacRxcRssiNotMyStaFifoRdEntriesNum_u;

/*REG_B2_PAC_RXC_RSSI_NOT_MY_STA_FIFO_NUM_ENTRIES_COUNT 0x17C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rssiNotMyStaFifoNumEntriesCount : 11; //RSSI Not My STA FIFO number of entries count, reset value: 0x0, access type: RO
		uint32 reserved0 : 21;
	} bitFields;
} RegB2PacRxcRssiNotMyStaFifoNumEntriesCount_u;

/*REG_B2_PAC_RXC_RSSI_NOT_MY_STA_FIFO_DEBUG 0x180 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rssiNotMyStaFifoWrPtr : 10; //RSSI Not My STA FIFO write pointer index, reset value: 0x0, access type: RO
		uint32 reserved0 : 6;
		uint32 rssiNotMyStaFifoNotEmpty : 1; //RSSI Not My STA FIFO not empty indication, reset value: 0x0, access type: RO
		uint32 rssiNotMyStaFifoFull : 1; //RSSI Not My STA FIFO full indication, reset value: 0x0, access type: RO
		uint32 rssiNotMyStaFifoDecrementLessThanZero : 1; //RSSI Not My STA setting FIFO amount has been decremented less than zero, reset value: 0x0, access type: RO
		uint32 reserved1 : 5;
		uint32 rssiNotMyStaFifoFullDropCtr : 8; //RSSI Not My STA setting FIFO full drop counter, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacRxcRssiNotMyStaFifoDebug_u;

/*REG_B2_PAC_RXC_OVERRIDE_PHY_DEBUG 0x184 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 bfeeSkipNdpaNdpTimerExpiredCheck : 1; //Skip the check of "NDPA-NDP timer expired" on PHY NDP reception, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2PacRxcOverridePhyDebug_u;

/*REG_B2_PAC_RXC_NAV_TIMER 0x188 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 navTimer : 16; //Rx nav timer, reset value: 0x0, access type: RO
		uint32 rtsClrRxNavTimer : 11; //RTS duration timer in order to reset Rx NAV timer, reset value: 0x0, access type: RO
		uint32 reserved0 : 5;
	} bitFields;
} RegB2PacRxcNavTimer_u;

/*REG_B2_PAC_RXC_RX_NAV_TIMER_CFG_SET_VALUE 0x18C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxNavTimerCfgSetValue : 16; //Setting Rx NAV timer value, reset value: 0x0, access type: RW
		uint32 reserved0 : 16;
	} bitFields;
} RegB2PacRxcRxNavTimerCfgSetValue_u;

/*REG_B2_PAC_RXC_PHY_NOISE_FLOOR_ANT_3TO0 0x190 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyNoiseFloorAnt0 : 8; //PHY metric of noise floor antenna 0, reset value: 0x80, access type: RO
		uint32 phyNoiseFloorAnt1 : 8; //PHY metric of noise floor antenna 1, reset value: 0x80, access type: RO
		uint32 phyNoiseFloorAnt2 : 8; //PHY metric of noise floor antenna 2, reset value: 0x80, access type: RO
		uint32 phyNoiseFloorAnt3 : 8; //PHY metric of noise floor antenna 3, reset value: 0x80, access type: RO
	} bitFields;
} RegB2PacRxcPhyNoiseFloorAnt3To0_u;

/*REG_B2_PAC_RXC_PHY_NOISE_FLOOR_ANT4 0x194 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 phyNoiseFloorAnt4 : 8; //PHY metric of noise floor antenna 4, reset value: 0x80, access type: RO
		uint32 reserved0 : 24;
	} bitFields;
} RegB2PacRxcPhyNoiseFloorAnt4_u;

/*REG_B2_PAC_RXC_TIMING_COUNTERS_WR_VALUE 0x198 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 timingCountersWrValue : 28; //Timing counters, write value, reset value: 0x0, access type: RW
		uint32 timingCountersWrCounterType : 2; //Timing counters, which air time counter to update:  , 00: Rx to me,  , 01: Rx all,  , 10: CCA 20MHz Primary busy,  , 11: Rx NAV busy., reset value: 0x0, access type: RW
		uint32 reserved0 : 2;
	} bitFields;
} RegB2PacRxcTimingCountersWrValue_u;

/*REG_B2_PAC_RXC_RX_TO_ME_AIR_TIME_CTR 0x19C */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxToMeAirTimeCtr : 28; //Rx to me air time counter, resolution of 1[us], reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
	} bitFields;
} RegB2PacRxcRxToMeAirTimeCtr_u;

/*REG_B2_PAC_RXC_RX_ALL_AIR_TIME_CTR 0x1A0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxAllAirTimeCtr : 28; //Rx all air time counter, resolution of 1[us], reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
	} bitFields;
} RegB2PacRxcRxAllAirTimeCtr_u;

/*REG_B2_PAC_RXC_CCA_20P_BUSY_AIR_TIME_CTR 0x1A4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 cca20PBusyAirTimeCtr : 28; //CCA 20MHz Primary busy air time counter, resolution of 1[us], reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
	} bitFields;
} RegB2PacRxcCca20PBusyAirTimeCtr_u;

/*REG_B2_PAC_RXC_RX_NAV_BUSY_AIR_TIME_CTR 0x1A8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxNavBusyAirTimeCtr : 28; //Rx NAV busy air time counter, resolution of 1[us], reset value: 0x0, access type: RO
		uint32 reserved0 : 4;
	} bitFields;
} RegB2PacRxcRxNavBusyAirTimeCtr_u;

/*REG_B2_PAC_RXC_RX_CONTROL2 0x1AC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 htDelimiterLength14BitsWidthEn : 1; //Enable using delimiter length field of 14bits, while PHY mode is 11n, reset value: 0x0, access type: RW
		uint32 reserved0 : 31;
	} bitFields;
} RegB2PacRxcRxControl2_u;

/*REG_B2_PAC_RXC_RX_STA_VAP_ID_DBG 0x1B0 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxStaIdxUsr0 : 8; //Rx STA index of user 0, reset value: 0x0, access type: RO
		uint32 rxVapIdxUsr0 : 5; //Rx VAP index of user 0, reset value: 0x0, access type: RO
		uint32 rxStaIdxValidUsr0 : 1; //Rx STA index valid of user 0, reset value: 0x0, access type: RO
		uint32 rxVapIdxValidUsr0 : 1; //Rx VAP index valid of user 0, reset value: 0x0, access type: RO
		uint32 unicastMpduUsr0 : 1; //unicast MPDU of user 0, reset value: 0x0, access type: RO
		uint32 tfMpduVapIdx : 5; //TF MPDU VAP index, reset value: 0x0, access type: RO
		uint32 reserved0 : 11;
	} bitFields;
} RegB2PacRxcRxStaVapIdDbg_u;

/*REG_B2_PAC_RXC_RXC_STA_IDX_VALID_USER0TO31 0x1B4 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcStaIdxValidUser0To31 : 32; //STA index valid for users 0 to 31, reset value: 0x0, access type: RO
	} bitFields;
} RegB2PacRxcRxcStaIdxValidUser0To31_u;

/*REG_B2_PAC_RXC_RXC_STA_IDX_VALID_USER32TO35 0x1B8 */
typedef union
{
	uint32 val;
	struct
	{
		uint32 rxcStaIdxValidUser32To35 : 4; //STA index valid for users 32 to 35, reset value: 0x0, access type: RO
		uint32 reserved0 : 28;
	} bitFields;
} RegB2PacRxcRxcStaIdxValidUser32To35_u;

/*REG_B2_PAC_RXC_SPARE_REGISTERS 0x1FC */
typedef union
{
	uint32 val;
	struct
	{
		uint32 spareRegisters : 32; //Spare registers, reset value: 0x88888888, access type: RW
	} bitFields;
} RegB2PacRxcSpareRegisters_u;



#endif // _B2_PAC_RXC_REGS_H_
