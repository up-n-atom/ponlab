/*
 *  <legal_notice>
 *   MaxLinear, Inc. retains all right, title and interest (including all intellectual
 *   property rights) in and to this computer program, which is protected by applicable
 *   intellectual property laws.  Unless you have obtained a separate written license from
 *   MaxLinear, Inc. or an authorized licensee of MaxLinear, Inc., you are not authorized
 *   to utilize all or a part of this computer program for any purpose (including
 *   reproduction, distribution, modification, and compilation into object code), and you
 *   must immediately destroy or return all copies of this computer program.  If you are
 *   licensed by MaxLinear, Inc. or an authorized licensee of MaxLinear, Inc., your rights
 *   to utilize this computer program are limited by the terms of that license.
 *  
 *   This computer program contains trade secrets owned by MaxLinear, Inc. and, unless
 *   authorized by MaxLinear, Inc. in writing, you agree to maintain the confidentiality
 *   of this computer program and related information and to not disclose this computer
 *   program and related information to any other person or entity.
 *  
 *   Misuse of this computer program or any information contained in it may results in
 *   violations of applicable law.  MaxLinear, Inc. vigorously enforces its copyright,
 *   trade secret, patent, contractual, and other legal rights.
 *  
 *   THIS COMPUTER PROGRAM IS PROVIDED "AS IS" WITHOUT ANY WARRANTIES, AND MAXLINEAR, INC.
 *   EXPRESSLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING THE WARRANTIES OF
 *   MERCHANTIBILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE, AND NONINFRINGEMENT.
 *  
 *  ***************************************************************************************
 *                                          Copyright (c) 2021/2022, MaxLinear, Inc.
 *  ***************************************************************************************
 *  </legal_notice>
 */

#ifndef _DUT_TYPES_H_
#define _DUT_TYPES_H_

#include "dut/AntennaMask.h"

#include <stdint.h>
#include <string>
#ifdef LINUX_HOST
#include <cstddef>
#endif

namespace dut {

// Maximum number of RX antennas
constexpr uint8_t maxNumRxAntennas = 5; // DUT_RXANT_ARRAY

// Maximum number of TX antennas
constexpr uint8_t maxNumTxAntennas = 4; // DUT_4ANT_ARRAY

// sizeof(dutNvMemoryAccessParams_t.data)
constexpr size_t maxNvMemoryAccessLength = 484; // MAX_DUT_EEPROM_ACCESS_SIZE

// sizeof(dutMemoryAccessParams_t.data)
constexpr size_t maxMemoryAccessLength = 256; // MAX_DUT_MEMORY_ACCESS_SIZE

// Maximum packet length allowed for transmissions (when long data is not enabled)
constexpr uint32_t maxDefaultPacketLength = 4000; // 0xFA0

// Minimum IFS (Interframe Space) value (16us in 802.11a and 10us in 802.11b)
constexpr uint32_t minInterframeSpace = 10;

// Minimum number of ODFM symbols allowed for transmissions
constexpr uint32_t minNumOdfmSymbols = 1;

constexpr size_t transmitPowerTableSize = 64;

// Minimum transmit power level index
constexpr int8_t minPowerLevelIndex = -20;

// Maximum transmit power level index
constexpr int8_t maxPowerLevelIndex = transmitPowerTableSize - 1;

// Minimum transmit power level value in 0.5 dBm
constexpr float minPowerLevelValue = -10.0f;

// Maximum transmit power level value in 0.5 dBm
constexpr float maxPowerLevelValue = 31.5f;

constexpr uint8_t transmitGainTableSize = 192;

constexpr size_t maxRssiCalDataLength = 215;

constexpr size_t maxNumLnaFrequencies = 7;
constexpr size_t maxNumLnaSubBands = 16;
constexpr size_t maxNumLnaSubBandsCrossingPointsVer6 = 7; // DUT_NUMBER_OF_CROSSING_POINTS
constexpr size_t maxNumLnaSubBandsCrossingPointsVer7 = 3;
constexpr size_t maxNumLnaSubBandsCrossingPoints = maxNumLnaSubBandsCrossingPointsVer6;
constexpr size_t maxNumLnaGainStepsVer6 = 8; //gen5=6, gen6=8
constexpr size_t maxNumLnaGainStepsVer7 = 14; // A0 has only 8 LNA indexes but this will be fixed in B0

constexpr size_t maxNumRxRegionsVer6 = 3;
constexpr size_t maxNumRxRegionsVer7 = 2;
constexpr size_t maxNumTxRegions = 3;

constexpr size_t maxRxRfFlatnessPoints = 11;
constexpr size_t maxRxRssiFlatnessPoints = 7;

constexpr size_t numPowerPoints = 6;

constexpr size_t cardInfoMacAddressSize = 6;
constexpr size_t cardInfoSerialNumberSize = 3;

constexpr size_t dpdTotalCalibrationPoints = 15; //DPD_TOTAL_CALIBRATION_POINTS

// ruParams per user is a 9-bit value. Bits 0-7: RU Allocation, Bit 8: PS160
constexpr uint32_t maxRuParamsUserOneValue = 0x1ff;
constexpr uint32_t maxRuParamsUserTwoValue = 0x200; // 0x200 to indicate SU

enum class Band {
    BAND_5000MHZ = 0,
    BAND_2400MHZ = 1,
    BAND_6000MHZ = 2,
    BAND_INVALID = 0xff
};

enum class Bandwidth {
    BANDWIDTH_TWENTY = 0x0,
    BANDWIDTH_FOURTY = 0x1,
    BANDWIDTH_EIGHTY = 0x2,
    BANDWIDTH_ONE_HUNDRED_SIXTY = 0x3,
    BANDWIDTH_THREE_HUNDRED_TWENTY = 0x4,
    BANDWIDTH_INVALID = 0xff
};

enum class CalibrationFileVersion {
    CALIBRATION_FILE_VERSION_4 = 4,
    CALIBRATION_FILE_VERSION_5 = 5,
    CALIBRATION_FILE_VERSION_6 = 6,
    CALIBRATION_FILE_VERSION_7 = 7,
    CALIBRATION_FILE_VERSION_INVALID = 0xff
};

enum class CalibrationFileSubversion {
    CALIBRATION_FILE_SUBVERSION_0 = 0,
    CALIBRATION_FILE_SUBVERSION_1 = 1,
    CALIBRATION_FILE_SUBVERSION_INVALID = 0xff
};

enum class ChipID {
    CHIP_ID_GEN4_A = 0x7C0,
    CHIP_ID_GEN4_B = 0x7d0,
    CHIP_ID_GEN4_C = 0x7e0,
    CHIP_ID_GEN5_A = 0x800,
    CHIP_ID_GEN5_B = 0x810,
    CHIP_ID_GEN6 = 0x900,
    CHIP_ID_GEN6_B = 0x910,
    CHIP_ID_GEN6_D2 = 0x980,
    CHIP_ID_GEN6_D2B = 0x991,
    CHIP_ID_GEN7 = 0xA00,
    CHIP_ID_GEN7B = 0xA11,
    CHIP_ID_INVALID = 0
};

enum class ChipModule {
    CHIP_MODULE_UMAC_MEM = 1,
    CHIP_MODULE_LMAC_MEM,
    CHIP_MODULE_PHY,
    CHIP_MODULE_RF,
    CHIP_MODULE_AFE,
    CHIP_MODULE_REGISTER
};

enum class CodingType {
    CODING_TYPE_AUTO = 0,
    CODING_TYPE_BCC = 1,
    CODING_TYPE_LDPC = 2
};

enum class FemType {
    FEM_TYPE_LINEAR = 0,
    FEM_TYPE_NON_LINEAR = 1,
    FEM_TYPE_INVALID = 0xff
};

enum class FileType {
    FILE_TYPE_CALIBRATION,
    FILE_TYPE_SIGNATURE
};

enum class Gi {
    GI_0_8_US = 0x00,
    GI_0_4_US = 0x01,
    GI_1_6_US = 0x02,
    GI_3_2_US = 0x03,
};

enum class HardwareType {
    HARDWARE_TYPE_GEN2_PCI,
    HARDWARE_TYPE_GEN3_PCI,
    HARDWARE_TYPE_GEN3_PCIE,
    HARDWARE_TYPE_GEN4,
    HARDWARE_TYPE_GEN5,
    HARDWARE_TYPE_GEN6,
    HARDWARE_TYPE_GEN7,
    HARDWARE_TYPE_INVALID
};

enum class Ltf {
    LTF_SHORT = 0x00, // 1x LTF
    LTF_MEDIUM = 0x01, // 2x LTF
    LTF_LONG = 0x02, // 4x LTF
};

enum class Mcs {
    // The first block of this enum is defined so each value matches the corresponding MCS index
    // shown in https://mcsindex.com/
    // Some empty room has been left at the end of the block to accommodate future new values.
    MCS_BPSK_12 = 0,
    MCS_QPSK_12 = 1,
    MCS_QPSK_34 = 2,
    MCS_16QAM_12 = 3,
    MCS_16QAM_34 = 4,
    MCS_64QAM_23 = 5,
    MCS_64QAM_34 = 6,
    MCS_64QAM_56 = 7,
    MCS_256QAM_34 = 8,
    MCS_256QAM_56 = 9,
    MCS_1024QAM_34 = 10,
    MCS_1024QAM_56 = 11,

    // These values are valid in 802.11be and following
    MCS_4096QAM_34 = 12,
    MCS_4096QAM_56 = 13,

    // BPSK 3/4 is valid in 802.11a and 80211g only
    MCS_BPSK_34 = 20,

    // These values are valid in 802.11b only
    MCS_80211b_1MBPS_SHORT = 30,
    MCS_80211b_2MBPS_SHORT = 31,
    MCS_80211b_5MBPS_SHORT = 32,
    MCS_80211b_11MBPS_SHORT = 33,
    MCS_80211b_1MBPS_LONG = 34,
    MCS_80211b_2MBPS_LONG = 35,
    MCS_80211b_5MBPS_LONG = 36,
    MCS_80211b_11MBPS_LONG = 37
};

enum class NvMemorySize {
    MEMORY_SIZE_BYTES_1K = 1024,
    MEMORY_SIZE_BYTES_2K = 2048,
    MEMORY_SIZE_BYTES_3K = 3072,
    MEMORY_SIZE_INVALID = 0
};

enum class NvMemoryType {
    MEMORY_TYPE_EEPROM = 1,
    MEMORY_TYPE_FLASH = 2,
    MEMORY_TYPE_EFUSE = 3
};

enum class PhyMode {
    PHY_MODE_A = 0,
    PHY_MODE_B = 1,
    PHY_MODE_G = 2,
    PHY_MODE_N_5 = 4,
    PHY_MODE_N_2_4 = 5,
    PHY_MODE_AC = 6,
    PHY_MODE_AX = 7,
    PHY_MODE_BE = 8
};

enum class RegulationType {
    REGULATION_TYPE_UNKNOWN = 0,
    REGULATION_TYPE_FCC_SP = 0x10,
    REGULATION_TYPE_FCC_LPI = 0x11,
    REGULATION_TYPE_DOC = 0x20,
    REGULATION_TYPE_ETSI = 0x30,
    REGULATION_TYPE_SPAIN = 0x31,
    REGULATION_TYPE_FRANCE = 0x32,
    REGULATION_TYPE_UAE = 0x33,
    REGULATION_TYPE_GERMANY = 0x34,
    REGULATION_TYPE_MKK = 0x40,
    REGULATION_TYPE_ISRAEL = 0x41,
    REGULATION_TYPE_SINGAPORE = 0x42,
    REGULATION_TYPE_AUSTRALIA = 0x44,
    REGULATION_TYPE_BRAZIL = 0x45,
    REGULATION_TYPE_CHINA = 0x46,
    REGULATION_TYPE_KOREA = 0x47,
    REGULATION_TYPE_APAC = 0x50,
    REGULATION_TYPE_JAPAN = 0x51,
};

enum class VersionedComponent {
    VERSIONED_COMPONENT_CV,
    VERSIONED_COMPONENT_PSD,
    VERSIONED_COMPONENT_REGULATORY,
    VERSIONED_COMPONENT_500B_PROGMODEL,
    VERSIONED_COMPONENT_600_PROGMODEL,
    VERSIONED_COMPONENT_600B_PROGMODEL,
    VERSIONED_COMPONENT_600D2_PROGMODEL,
    VERSIONED_COMPONENT_700_PROGMODEL,
    VERSIONED_COMPONENT_700B_PROGMODEL
};

struct CorrelationResults_t {
    int32_t II;
    int32_t QQ;
    int32_t IQ;
};

struct BeamformingHeaderInfo_t {
    PhyMode phyMode; // PHY mode (packet format)
    Bandwidth bandwidth; // Bandwidth (20MHz, 40MHz, 80MHz, 160MHz, 320MHz)
};

struct BeamformingFilePathSet_t {
    const char* headerFile = nullptr; // Header file path (always required)
    const char* valuesFile = nullptr; // Standard values file path (always required)
    const char* extValuesEhtFile = nullptr; // Extended EHT values file path (optional, NULL if not used)
};

struct TransmitPowerVector_t {
    uint16_t lengthOfPowerVector = 0; //how many power indexes.
    uint16_t startingPacketLength = 0; //length (in bytes) of first index's packets. afterwards - packet length is optionally incremented in each index.
    uint16_t packetLengthIncrementPerIndex = 0; //powerIndexArray[i] = startingPacketLength + packetLengthIncrementPerIndex*i
    uint8_t powerIndexArray[transmitPowerTableSize] = {}; //powerIndexArray[i] = startingPacketLength + packetLengthIncrementPerIndex*i
    uint8_t numOfTransmissionsPerPower[transmitPowerTableSize] = {}; //#repetitions for each power index
    uint16_t measuredVoltagePerPower[maxNumTxAntennas][transmitPowerTableSize] = {}; //measured results
};

struct StartCalibrationParams_t {
    uint32_t mask; // calibration mask, see ClbrProcBit_e (only if maskType is configurable)
    uint8_t maskType; // configurable = 0/ default = 1, /* dutCalibrationMaskType_e
    uint8_t type; // offline = 0/ online = 1 /* dutCalibrationType_e
};

struct Point_t {
    float x;
    float y;
};

struct RssiAB_t {
    float a = 0.0f; // Linear Fit Slope (Exponential growth rate parameter of the RSSI detector).
    float b = 0.0f; // Linear Fit Offset (Logarithmic intercept point)
};

struct RssiS2D_t {
    uint8_t gain = 0; // S2D index. The S2D gain of the RSSI path
    uint8_t offset = 0; // Ioffs index. The calibrated DC offset of the RSSI path
};

struct RssiAntennaDataVer6_t {
    uint16_t subBandsCrossingPoints[maxNumLnaSubBandsCrossingPointsVer6] = {}; // result of calculateRxLnaSubBandGains()
    uint8_t firstCrossingPointIndex = 0; // Index of the first crossing point
    uint8_t lnaMidGain = 0; // RX Middle Gain Tuning. The RFIC LNA tuning word for the nominal LNA Middle Gain. Result of: LNA Mid Gain Tuning Calibration
    float lnaGains[maxNumLnaGainStepsVer6] = {}; // RX Gain Steps. The measured RX RF gains for gain steps 1..N (Number of LNA steps). Result of: LNA Gain Steps calibration
    int8_t midGainDeltaPoints[maxRxRfFlatnessPoints] = {};
    int8_t rxRfFlatnessDeltaPointsHighGain[maxRxRfFlatnessPoints] = {};
    int8_t rxRfFlatnessDeltaPointsBypass[maxRxRfFlatnessPoints] = {};
    RssiS2D_t s2dGainOffset[maxNumRxRegionsVer6] = {};
    RssiAB_t pointsAB[maxNumRxRegionsVer6] = {}; // result of: RF RSSI Calibration at reference frequency
    int8_t rxRssiFlatnessDeltaPoints[maxRxRssiFlatnessPoints] = {}; // Output of: RSSI Flatness (rxRssiFlatnessFreqPoint shared)
};

struct RssiAntennaDataVer7_t {
    uint16_t subBandsCrossingPoints[maxNumLnaSubBandsCrossingPointsVer7] = {}; // result of calculateRxLnaSubBandGains()
    uint8_t firstCrossingPointIndex = 0; // Index of the first crossing point
    float lnaGains[maxNumLnaGainStepsVer7] = {}; // RX Gain Steps. The measured RX RF gains for gain steps 1..N (Number of LNA steps). Result of: LNA Gain Steps calibration
    int8_t rxRfFlatnessDeltaPointsHighGain[maxRxRfFlatnessPoints] = {};
    int8_t rxRfFlatnessDeltaPointsLowGain[maxRxRfFlatnessPoints] = {};
    int8_t rxRfFlatnessDeltaPointsBypass[maxRxRfFlatnessPoints] = {};
    RssiS2D_t s2dGainOffset[maxNumRxRegionsVer7] = {};
    RssiAB_t pointsAB[maxNumRxRegionsVer7] = {}; // result of: RF RSSI Calibration at reference frequency
    int8_t rxRssiFlatnessDeltaPoints[maxRxRssiFlatnessPoints] = {}; // Output of: RSSI Flatness (rxRssiFlatnessFreqPoint shared)
};

union RssiAntennaData_t {
    RssiAntennaDataVer6_t antennasVer6[maxNumRxAntennas];
    RssiAntennaDataVer7_t antennasVer7[maxNumRxAntennas];
};

struct RssiCalibrationData_t {
    AntennaMask_t antennaMask = 0;
    uint16_t startFreq = 0;
    uint16_t stopFreq = 0;
    uint16_t calibrationFreq = 0; // Reference Frequency. The reference frequency at which RX gain & gain steps calibration is performed
    uint8_t chipTemperature = 0; // Chip temperature at calibration time
    bool subBandsCrossingPoints = false;

    uint8_t numRxRfFlatnessPoints = 0; // Number of RX RF Flatness calibration frequencies (M)
    uint8_t numRxRssiFlatnessPoints = 0; // Number of RX RSSI Flatness calibration frequencies (L)
    uint16_t rxRfFlatnessFreqs[maxRxRfFlatnessPoints] = {}; // RX RF Flatness Calibration Frequencies. Output of: Calibrate RFIC RX Gain Flatness (rxRfFlatnessDeltaPointsHighGain per ant.)
    uint16_t rxRssiFlatnessFreqs[maxRxRssiFlatnessPoints] = {}; // RX RSSI Flatness Calibration Frequencies. Output of: RSSI Flatness (rxRssiFlatnessDeltaPoints per ant.)

    RssiAntennaData_t antennaData {};
};

struct TssiAB_t {
    short a = 0;
    short b = 0;
};

struct TssiAntennaData_t {
    TssiAB_t pointsAB[maxNumTxRegions] = {};
    int32_t s2dGain[maxNumTxRegions]; //when s2dCalFlag != 0 dut calibrate per region S2D
    int32_t s2dOffset[maxNumTxRegions]; //when s2dCalFlag != 0 dut calibrate per region S2D
    uint8_t maxPower;
    uint8_t uEvm;
    uint8_t uEvmGain;
};

struct TssiCalibrationData_t {
    // Currently, maximum IEEE channel number is 233 (U-NII-8), so uint8_t should be enough
    uint16_t channel = 0;
    // BAND_5000MHZ or BAND_2400MHZ or BAND_6000MHZ
    Band band = Band::BAND_INVALID;

    AntennaMask_t antennaMask = 0;
    uint8_t numRegions = 0;
    Bandwidth bw = Bandwidth::BANDWIDTH_INVALID;

    TssiAntennaData_t antennas[maxNumTxAntennas] = {};
};

}

#endif
