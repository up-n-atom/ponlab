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

#ifndef _DUT_CLI_FUNCTIONS_H_
#define _DUT_CLI_FUNCTIONS_H_

#include "Function.h"

namespace dut_cli {

std::vector<std::string> getFunctionNames();
std::unique_ptr<Function> createFunction(const std::string& name);

class DriverInitFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    bool m_snifferMode { false };
    dut::NvMemoryType m_memoryType { dut::NvMemoryType::MEMORY_TYPE_FLASH };
    dut::NvMemorySize m_memorySize { dut::NvMemorySize::MEMORY_SIZE_BYTES_1K };
    dut::Band m_band { dut::Band::BAND_INVALID };
};

class DriverReleaseFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class CalculateMaxPacketLengthFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    dut::PhyMode m_phyMode { dut::PhyMode::PHY_MODE_AX };
};

class CalculatePacketLengthFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    dut::PhyMode m_phyMode { dut::PhyMode::PHY_MODE_AX };
    dut::Bandwidth m_signalBandwidth { dut::Bandwidth::BANDWIDTH_TWENTY };
    dut::Mcs m_mcs { dut::Mcs::MCS_QPSK_34 };
    uint8_t m_nss { 1 };
    uint32_t m_numSymbols { 20 };
};

class FlushNvmFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetAvailableRxAntennaMaskFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetAvailableTxAntennaMaskFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetBandFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetSupportedBandsFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetBbicCddValuesFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    uint8_t m_numTxAntennas { 0 };
};

class GetCalibrationFileVersionFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetCardInfoFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetChipIdFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetComponentVersionFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    dut::VersionedComponent m_component { dut::VersionedComponent::VERSIONED_COMPONENT_CV };
};

class GetEnabledRxAntennaMaskFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetEnabledTxAntennaMaskFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetFemTypeFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetHardwareTypeFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetInbandRssiFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetMacPacketCountersFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetMpduPacketCountersFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetNmseValuesFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetNvmSizeFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetNvmTypeFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetNvmVersionFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetPhyModeFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetPhyPacketCountersFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetProductionFlagFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetRxEvmFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetRxRateInfoFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetTemperatureFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetTransmitVoltagesFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetXtalCalValueFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetXtalRegValueFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class GetZwdfsStatusFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class LoadBeamformingMatrixFromFileFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    std::string m_headerFileName;
    std::string m_valuesFileName;
    std::string m_extValuesEhtFileName;
    std::string m_secondaryHeaderFileName;
    std::string m_secondaryValuesFileName;
    std::string m_secondaryExtValuesEhtFileName;
};

class LoadNvmFromFileFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    std::string m_fileName;
};

class ReadMemoryFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    dut::ChipModule m_module { dut::ChipModule::CHIP_MODULE_UMAC_MEM };
    size_t m_address { 0 };
    size_t m_length { 0 };
};

class ReadNvmFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    size_t m_address { 0 };
    size_t m_length { 0 };
};

class ReadRegisterFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    dut::ChipModule m_module { dut::ChipModule::CHIP_MODULE_UMAC_MEM };
    size_t m_address { 0 };
    uint32_t m_mask { 0 };
};

class ResetMacPacketCountersFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class ResetMpduPacketCountersFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class ResetPhyPacketCountersFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class RxMeasureFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    uint32_t m_numCapture { 1 };
    uint32_t m_interval { 1000 };
    bool m_disabled { false };
};

class SaveNvmToFileFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    std::string m_fileName;
};

class SetBbicCddValuesFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    uint8_t m_numTxAntennas { 1 };
    uint32_t m_offset1 { 0 };
    uint32_t m_offset2 { 0 };
    uint32_t m_offset3 { 0 };
};

class SetCalibrationFileVersionFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    dut::CalibrationFileVersion m_version { dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_INVALID };
    dut::CalibrationFileSubversion m_subversion { dut::CalibrationFileSubversion::CALIBRATION_FILE_SUBVERSION_INVALID };
};

class SetCardInfoFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    uint8_t m_countryCode { 0 };
    std::array<uint8_t, dut::cardInfoMacAddressSize> m_macAddress {};
    std::array<uint8_t, dut::cardInfoSerialNumberSize> m_serialNumber {};
    uint8_t m_week { 0 };
    uint8_t m_year { 0 };
};

class SetChannelFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    dut::PhyMode m_phyMode { dut::PhyMode::PHY_MODE_AX };
    dut::Bandwidth m_spectrumBandwidth { dut::Bandwidth::BANDWIDTH_TWENTY };
    uint8_t m_lowestChannel { 1 };
    uint8_t m_primaryChannelIndex { 0 };
    dut::RegulationType m_regulationType { dut::RegulationType::REGULATION_TYPE_FCC_SP };
};

class SetClipperFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    bool m_enabled { true };
};

class SetEnabledRxAntennaMaskFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    dut::AntennaMask m_antennaMask;
};

class SetEnabledTxAntennaMaskFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    dut::AntennaMask m_antennaMask;
};

class SetIfsFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    uint32_t m_ifs { 0 };
};

class SetProductionFlagFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    bool m_productionFlag { false };
    bool m_writeToNvm { false };
};

class SetRateFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    dut::Bandwidth m_signalBandwidth { dut::Bandwidth::BANDWIDTH_TWENTY };
    dut::Mcs m_mcs { dut::Mcs::MCS_QPSK_34 };
    uint8_t m_nss { 1 };
    dut::Gi m_gi { dut::Gi::GI_0_8_US };
    dut::Ltf m_ltf { dut::Ltf::LTF_MEDIUM };
};

class SetRuParamsFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    uint32_t m_userOne { 0 };
    uint32_t m_userTwo { 0 };
};

class SetRxAggregationEnabledFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    bool m_enabled { false };
};

class SetSpacelessTxEnabledFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    bool m_enabled { false };
};

class SetTransmitPowerControlFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    bool m_closedLoop { false };
    uint8_t m_powerLimit { 0 };
};

class SetTransmitPowerLevelFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    uint8_t m_powerLevel { 0 };
};

class SetXtalCalValueFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    uint16_t m_value { 0 };
};

class SetXtalRegValueFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    uint16_t m_value { 0 };
};

class SetZwdfsConfigurationFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    uint8_t m_zwdfsChannel { 100 };
    dut::Bandwidth m_zwdfsBandwidth { dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY };
    dut::Bandwidth m_radarDetectionBandwidth { dut::Bandwidth::BANDWIDTH_INVALID };
};

class StartCalibrationFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    uint8_t m_calibrationType { 1 }; // default offline calibration
    uint8_t m_maskType { 0 };
    uint32_t m_mask { 0 };
};

class StartCwFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    int8_t m_amplitude { 1 };
    int16_t m_tone { 1 };
};

class StartTxFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    uint16_t m_repetitions { 0xffff };
    uint32_t m_packetLength { 1000 };
    bool m_longData { false };
    bool m_beamforming { false };
    dut::CodingType m_codingType { dut::CodingType::CODING_TYPE_AUTO };
};

class StartRxPerFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    uint32_t m_packetLimit { 10000 };
    uint32_t m_durationLimit { 1 };
};

class StopCwFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class StopTxFunction : public Function {
public:
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;
};

class StopRxPerFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    bool m_calcRxPer { false };
};

class ValidateBeamformingHeaderRegisterFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    dut::PhyMode m_expectedPhyMode = dut::PhyMode::PHY_MODE_AX;
    dut::Bandwidth m_expectedBandwidth = dut::Bandwidth::BANDWIDTH_EIGHTY;
};

class WriteCalibrationFileFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    dut::NvMemoryType m_memoryType { dut::NvMemoryType::MEMORY_TYPE_FLASH };
    dut::NvMemorySize m_memorySize { dut::NvMemorySize::MEMORY_SIZE_BYTES_1K };
};

class WriteMemoryFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    dut::ChipModule m_module { dut::ChipModule::CHIP_MODULE_UMAC_MEM };
    size_t m_address { 0 };
    std::vector<uint8_t> m_data;
};

class WriteNvmFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    size_t m_address { 0 };
    std::vector<uint8_t> m_data;
};

class WriteRegisterFunction : public Function {
public:
    bool parse(std::vector<std::string>& args) override;
    bool execute(std::shared_ptr<dut::Dut> dut, Context& context) override;

private:
    dut::ChipModule m_module { dut::ChipModule::CHIP_MODULE_UMAC_MEM };
    size_t m_address { 0 };
    uint32_t m_mask { 0 };
    uint32_t m_value { 0 };
};
}

#endif
