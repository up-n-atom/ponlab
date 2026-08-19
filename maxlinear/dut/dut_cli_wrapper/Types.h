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

#pragma once

#include "dut/Types.h"

namespace dut_cli {

public
value struct Defines {
public:
    literal uint8_t maxNumRxAntennas = dut::maxNumRxAntennas;
    literal uint8_t maxNumTxAntennas = dut::maxNumTxAntennas;
    literal uint8_t maxNumLnaSubBandsCrossingPointsVer6 = dut::maxNumLnaSubBandsCrossingPointsVer6;
    literal uint8_t maxNumLnaSubBandsCrossingPointsVer7 = dut::maxNumLnaSubBandsCrossingPointsVer7;
    literal uint8_t maxNumLnaGainStepsVer6 = dut::maxNumLnaGainStepsVer6;
    literal uint8_t maxNumLnaGainStepsVer7 = dut::maxNumLnaGainStepsVer7;

    literal int8_t minPowerLevelIndex = dut::minPowerLevelIndex;
    literal int8_t maxPowerLevelIndex = dut::maxPowerLevelIndex;
    literal float minPowerLevelValue = dut::minPowerLevelValue;
    literal float maxPowerLevelValue = dut::maxPowerLevelValue;
};

public
enum class Band {
    BAND_5000MHZ = 0,
    BAND_2400MHZ = 1,
    BAND_6000MHZ = 2,
    BAND_INVALID = 0xff
};

public
enum class Bandwidth {
    BANDWIDTH_TWENTY = 0x0,
    BANDWIDTH_FOURTY = 0x1,
    BANDWIDTH_EIGHTY = 0x2,
    BANDWIDTH_ONE_HUNDRED_SIXTY = 0x3,
    BANDWIDTH_THREE_HUNDRED_TWENTY = 0x4,
    BANDWIDTH_INVALID = 0xff,
};

public
enum class CalibrationFileVersion {
    CALIBRATION_FILE_VERSION_4 = 4,
    CALIBRATION_FILE_VERSION_5 = 5,
    CALIBRATION_FILE_VERSION_6 = 6,
    CALIBRATION_FILE_VERSION_7 = 7,
    CALIBRATION_FILE_VERSION_INVALID = 0xff
};

public
enum class CalibrationFileSubversion {
    CALIBRATION_FILE_SUBVERSION_0 = 0,
    CALIBRATION_FILE_SUBVERSION_1 = 1,
    CALIBRATION_FILE_SUBVERSION_INVALID = 0xff
};

public
enum class ChipModule {
    CHIP_MODULE_UMAC_MEM = 1,
    CHIP_MODULE_LMAC_MEM,
    CHIP_MODULE_PHY,
    CHIP_MODULE_RF,
    CHIP_MODULE_AFE,
    DUT_CHIP_MODULE_REGISTER
};

public
enum class CodingType {
    CODING_TYPE_AUTO = 0,
    CODING_TYPE_BCC = 1,
    CODING_TYPE_LDPC = 2
};

public
enum class FemType {
    FEM_TYPE_LINEAR = 0,
    FEM_TYPE_NON_LINEAR = 1,
    FEM_TYPE_INVALID = 0xff
};

public
enum class Gi {
    GI_0_8_US = 0x00,
    GI_0_4_US = 0x01,
    GI_1_6_US = 0x02,
    GI_3_2_US = 0x03,
};

public
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

public
enum class Ltf {
    LTF_SHORT = 0x00, // 1x LTF
    LTF_MEDIUM = 0x01, // 2x LTF
    LTF_LONG = 0x02, // 4x LTF
};

public
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

public
enum class NvMemorySize {
    MEMORY_SIZE_BYTES_1K = 1024,
    MEMORY_SIZE_BYTES_2K = 2048,
    MEMORY_SIZE_BYTES_3K = 3072,
    MEMORY_SIZE_INVALID = 0
};

public
enum class NvMemoryType {
    MEMORY_TYPE_EEPROM = 1,
    MEMORY_TYPE_FLASH = 2,
    MEMORY_TYPE_EFUSE = 3
};

public
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

public
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

public
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

}