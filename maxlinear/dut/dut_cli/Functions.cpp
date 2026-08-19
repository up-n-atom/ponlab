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

#include "Functions.h"

#include "dut/Tools.h"

#include "tclap/CmdLine.h"

#include <functional>
#include <map>
#include <set>
#include <stdexcept>

#ifdef LINUX_HOST
#include <cstring>
#define sscanf_s sscanf
#endif

namespace dut_cli {

constexpr size_t maxBytesPerLine = 32;

const std::map<int, std::string> mcsMap11ag = {
    { 0, "Invalid Rx Rate" },
    { 1, "Invalid Rx Rate" },
    { 2, "Invalid Rx Rate" },
    { 3, "Invalid Rx Rate" },
    { 4, "Invalid Rx Rate" },
    { 5, "Invalid Rx Rate" },
    { 6, "Invalid Rx Rate" },
    { 7, "Invalid Rx Rate" },
    { 8, "6:64QAM 2/3: 48 Mbps" },
    { 9, "4:16QAM 1/2: 24 Mbps" },
    { 10, "2:QPSK 1/2: 12 Mbps" },
    { 11, "0:BPSK 1/2: 6 Mbps" },
    { 12, "7:64QAM 3/4: 54 Mbps" },
    { 13, "5:16QAM 3/4: 36 Mbps" },
    { 14, "3:QPSK 3/4: 18 Mbps" },
    { 15, "1:BPSK 3/4: 9 Mbps" }
};

const std::map<int, std::string> mcsMap11b = {
    { 0, "1 Mbps" },
    { 1, "2 Mbps" },
    { 2, "5.5 Mbps" },
    { 3, "11 Mbps" }
};

const std::set<dut::Band> validValuesForBand {
    dut::Band::BAND_5000MHZ,
    dut::Band::BAND_2400MHZ,
    dut::Band::BAND_6000MHZ,
};

const std::string validValuesForBandString = "5GHz = 0, 2.4GHz = 1 and 6GHz = 2";

const std::set<dut::Bandwidth> validValuesForBandwidth {
    dut::Bandwidth::BANDWIDTH_TWENTY,
    dut::Bandwidth::BANDWIDTH_FOURTY,
    dut::Bandwidth::BANDWIDTH_EIGHTY,
    dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY,
    dut::Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY,
};

const std::string validValuesForBandwidthString = "20Mhz = 0, 40Mhz = 1, 80Mhz = 2, 160Mhz = 3 and 320Mhz = 4";

const std::set<dut::Bandwidth> validValuesForRadarDetectionBandwidth {
    dut::Bandwidth::BANDWIDTH_EIGHTY,
    dut::Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY,
    dut::Bandwidth::BANDWIDTH_INVALID,
};

const std::string validValuesForRadarDetectionBandwidthString = "80Mhz = 2, 160Mhz = 3 and Invalid = 255";

const std::set<dut::ChipModule> validValuesForChipModule {
    dut::ChipModule::CHIP_MODULE_UMAC_MEM,
    dut::ChipModule::CHIP_MODULE_LMAC_MEM,
    dut::ChipModule::CHIP_MODULE_PHY,
    dut::ChipModule::CHIP_MODULE_RF,
    dut::ChipModule::CHIP_MODULE_AFE,
    dut::ChipModule::CHIP_MODULE_REGISTER,
};

const std::string validValuesForChipModuleString = "UMAC_MEM = 1, LMAC_MEM = 2, PHY = 3, RF = 4, AFE = 5 and REGISTER = 6";

const std::set<dut::Gi> validValuesForGi {
    dut::Gi::GI_0_8_US,
    dut::Gi::GI_0_4_US,
    dut::Gi::GI_1_6_US,
    dut::Gi::GI_3_2_US,
};

const std::string validValuesForGiString = "0.8us = 0, 0.4us = 1, 1.6us = 2 and 3.2us = 3";

const std::set<dut::Ltf> validValuesForLtf {
    dut::Ltf::LTF_SHORT,
    dut::Ltf::LTF_MEDIUM,
    dut::Ltf::LTF_LONG,
};

const std::string validValuesForLtfString = "Short (x1) = 0, Medium (x2) = 1 and Long (x4) = 2";

const std::set<dut::NvMemorySize> validValuesForNvMemorySize {
    dut::NvMemorySize::MEMORY_SIZE_BYTES_1K,
    dut::NvMemorySize::MEMORY_SIZE_BYTES_2K,
    dut::NvMemorySize::MEMORY_SIZE_BYTES_3K
};

const std::string validValuesForNvMemorySizeString = "1024 and 2048";

const std::set<dut::NvMemoryType> validValuesForNvMemoryType {
    dut::NvMemoryType::MEMORY_TYPE_EEPROM,
    dut::NvMemoryType::MEMORY_TYPE_FLASH
};

const std::set<dut::Mcs> validValuesForMcs {
    dut::Mcs::MCS_BPSK_12,
    dut::Mcs::MCS_QPSK_12,
    dut::Mcs::MCS_QPSK_34,
    dut::Mcs::MCS_16QAM_12,
    dut::Mcs::MCS_16QAM_34,
    dut::Mcs::MCS_64QAM_23,
    dut::Mcs::MCS_64QAM_34,
    dut::Mcs::MCS_64QAM_56,
    dut::Mcs::MCS_256QAM_34,
    dut::Mcs::MCS_256QAM_56,
    dut::Mcs::MCS_1024QAM_34,
    dut::Mcs::MCS_1024QAM_56,
    dut::Mcs::MCS_4096QAM_34,
    dut::Mcs::MCS_4096QAM_56,
    dut::Mcs::MCS_BPSK_34,
    dut::Mcs::MCS_80211b_1MBPS_SHORT,
    dut::Mcs::MCS_80211b_2MBPS_SHORT,
    dut::Mcs::MCS_80211b_5MBPS_SHORT,
    dut::Mcs::MCS_80211b_11MBPS_SHORT,
    dut::Mcs::MCS_80211b_1MBPS_LONG,
    dut::Mcs::MCS_80211b_2MBPS_LONG,
    dut::Mcs::MCS_80211b_5MBPS_LONG,
    dut::Mcs::MCS_80211b_11MBPS_LONG,
};

const std::string validValuesForMcsString = "BPSK 1/2 = 0, QPSK 1/2 = 1, QPSK 3/4 = 2, 16-QAM 1/2 = 3, 16-QAM 3/4 = 4, 64-QAM 2/3 = 5, 64-QAM 3/4 = 6, 64-QAM 5/6 = 7, 256-QAM 3/4 = 8, 256-QAM 5/6 = 9, 1024-QAM 3/4 = 10, 1024-QAM 5/6 = 11, 4096-QAM 3/4 = 12, 4096-QAM 5/6 = 13, BPSK 3/4 = 20, 802.11b 1Mbps Short = 30, 802.11b 2Mbps Short = 31, 802.11b 5Mbps Short = 32, 802.11b 11Mbps Short = 33, 802.11b 1Mbps Long = 34, 802.11b 2Mbps Long = 35, 802.11b 5Mbps Long = 36 and 802.11b 11Mbps Long = 37";

const std::string validValuesForNvMemoryTypeString = "EEPROM = 1 and FLASH = 2";

const std::set<dut::PhyMode> validValuesForPhyMode {
    dut::PhyMode::PHY_MODE_A,
    dut::PhyMode::PHY_MODE_B,
    dut::PhyMode::PHY_MODE_G,
    dut::PhyMode::PHY_MODE_N_5,
    dut::PhyMode::PHY_MODE_N_2_4,
    dut::PhyMode::PHY_MODE_AC,
    dut::PhyMode::PHY_MODE_AX,
    dut::PhyMode::PHY_MODE_BE,
};

const std::string validValuesForPhyModeString = "802.11a = 0, 802.11b = 1, 802.11g = 2, 802.11n 5GHz = 4, 802.11n 2.4GHz = 5, 802.11ac = 6, 802.11ax = 7 and 802.11be = 8";

const std::set<dut::RegulationType> validValuesForRegulationType {
    dut::RegulationType::REGULATION_TYPE_UNKNOWN,
    dut::RegulationType::REGULATION_TYPE_FCC_SP,
    dut::RegulationType::REGULATION_TYPE_FCC_LPI,
    dut::RegulationType::REGULATION_TYPE_DOC,
    dut::RegulationType::REGULATION_TYPE_ETSI,
    dut::RegulationType::REGULATION_TYPE_SPAIN,
    dut::RegulationType::REGULATION_TYPE_FRANCE,
    dut::RegulationType::REGULATION_TYPE_UAE,
    dut::RegulationType::REGULATION_TYPE_GERMANY,
    dut::RegulationType::REGULATION_TYPE_MKK,
    dut::RegulationType::REGULATION_TYPE_ISRAEL,
    dut::RegulationType::REGULATION_TYPE_SINGAPORE,
    dut::RegulationType::REGULATION_TYPE_AUSTRALIA,
    dut::RegulationType::REGULATION_TYPE_BRAZIL,
    dut::RegulationType::REGULATION_TYPE_CHINA,
    dut::RegulationType::REGULATION_TYPE_KOREA,
    dut::RegulationType::REGULATION_TYPE_APAC,
    dut::RegulationType::REGULATION_TYPE_JAPAN,
};

const std::string validValuesForRegulationTypeString = "UNKNOWN = 0x00, FCC_SP = 0x10, FCC_LPI = 0x11, DOC = 0x20, ETSI = 0x30, SPAIN = 0x31, FRANCE = 0x32, UAE = 0x33, GERMANY = 0x34, MKK = 0x40, ISRAEL = 0x41, SINGAPORE = 0x42, AUSTRALIA = 0x44, BRAZIL = 0x45, CHINA = 0x46, KOREA = 0x47, APAC = 0x50 and JAPAN = 0x51";

/**
 * A function factory is a std::function that creates a unique pointer to a `Function`
 */
using FunctionFactory = std::function<std::unique_ptr<Function>()>;

/**
 * Creates a function factory for the specified template `Function` type
 */
template <typename T>
FunctionFactory createFunctionFactory()
{
    return []() { return std::make_unique<T>(); };
}

const std::map<std::string, FunctionFactory, std::less<>> g_functions {
    { "driverInit", createFunctionFactory<DriverInitFunction>() },
    { "driverRelease", createFunctionFactory<DriverReleaseFunction>() },
    { "calculateMaxPacketLength", createFunctionFactory<CalculateMaxPacketLengthFunction>() },
    { "calculatePacketLength", createFunctionFactory<CalculatePacketLengthFunction>() },
    { "flushNvm", createFunctionFactory<FlushNvmFunction>() },
    { "getAvailableRxAntennaMask", createFunctionFactory<GetAvailableRxAntennaMaskFunction>() },
    { "getAvailableTxAntennaMask", createFunctionFactory<GetAvailableTxAntennaMaskFunction>() },
    { "getBand", createFunctionFactory<GetBandFunction>() },
    { "getSupportedBands", createFunctionFactory<GetSupportedBandsFunction>() },
    { "getBbicCddValues", createFunctionFactory<GetBbicCddValuesFunction>() },
    { "getCalibrationFileVersion", createFunctionFactory<GetCalibrationFileVersionFunction>() },
    { "getCardInfo", createFunctionFactory<GetCardInfoFunction>() },
    { "getChipId", createFunctionFactory<GetChipIdFunction>() },
    { "getComponentVersion", createFunctionFactory<GetComponentVersionFunction>() },
    { "getEnabledRxAntennaMask", createFunctionFactory<GetEnabledRxAntennaMaskFunction>() },
    { "getEnabledTxAntennaMask", createFunctionFactory<GetEnabledTxAntennaMaskFunction>() },
    { "getFemType", createFunctionFactory<GetFemTypeFunction>() },
    { "getHardwareType", createFunctionFactory<GetHardwareTypeFunction>() },
    { "getInbandRssi", createFunctionFactory<GetInbandRssiFunction>() },
    { "getMacPacketCounters", createFunctionFactory<GetMacPacketCountersFunction>() },
    { "getMpduPacketCounters", createFunctionFactory<GetMpduPacketCountersFunction>() },
    { "getNmseValues", createFunctionFactory<GetNmseValuesFunction>() },
    { "getNvmSize", createFunctionFactory<GetNvmSizeFunction>() },
    { "getNvmType", createFunctionFactory<GetNvmTypeFunction>() },
    { "getNvmVersion", createFunctionFactory<GetNvmVersionFunction>() },
    { "getPhyMode", createFunctionFactory<GetPhyModeFunction>() },
    { "getPhyPacketCounters", createFunctionFactory<GetPhyPacketCountersFunction>() },
    { "getProductionFlag", createFunctionFactory<GetProductionFlagFunction>() },
    { "getRxEvm", createFunctionFactory<GetRxEvmFunction>() },
    { "getRxRateInfo", createFunctionFactory<GetRxRateInfoFunction>() },
    { "getTemperature", createFunctionFactory<GetTemperatureFunction>() },
    { "getTransmitVoltages", createFunctionFactory<GetTransmitVoltagesFunction>() },
    { "getXtalCalValue", createFunctionFactory<GetXtalCalValueFunction>() },
    { "getXtalRegValue", createFunctionFactory<GetXtalRegValueFunction>() },
    { "getZwdfsStatus", createFunctionFactory<GetZwdfsStatusFunction>() },
    { "loadBeamformingMatrixFromFile", createFunctionFactory<LoadBeamformingMatrixFromFileFunction>() },
    { "loadNvmFromFile", createFunctionFactory<LoadNvmFromFileFunction>() },
    { "readMemory", createFunctionFactory<ReadMemoryFunction>() },
    { "readNvm", createFunctionFactory<ReadNvmFunction>() },
    { "readRegister", createFunctionFactory<ReadRegisterFunction>() },
    { "resetMacPacketCounters", createFunctionFactory<ResetMacPacketCountersFunction>() },
    { "resetMpduPacketCounters", createFunctionFactory<ResetMpduPacketCountersFunction>() },
    { "resetPhyPacketCounters", createFunctionFactory<ResetPhyPacketCountersFunction>() },
    { "rxMeasure", createFunctionFactory<RxMeasureFunction>() },
    { "saveNvmToFile", createFunctionFactory<SaveNvmToFileFunction>() },
    { "setBbicCddValues", createFunctionFactory<SetBbicCddValuesFunction>() },
    { "setCalibrationFileVersion", createFunctionFactory<SetCalibrationFileVersionFunction>() },
    { "setCardInfo", createFunctionFactory<SetCardInfoFunction>() },
    { "setChannel", createFunctionFactory<SetChannelFunction>() },
    { "setClipper", createFunctionFactory<SetClipperFunction>() },
    { "setEnabledRxAntennaMask", createFunctionFactory<SetEnabledRxAntennaMaskFunction>() },
    { "setEnabledTxAntennaMask", createFunctionFactory<SetEnabledTxAntennaMaskFunction>() },
    { "setIfs", createFunctionFactory<SetIfsFunction>() },
    { "setProductionFlag", createFunctionFactory<SetProductionFlagFunction>() },
    { "setRate", createFunctionFactory<SetRateFunction>() },
    { "setRuParams", createFunctionFactory<SetRuParamsFunction>() },
    { "setRxAggregationEnabled", createFunctionFactory<SetRxAggregationEnabledFunction>() },
    { "setSpacelessTxEnabled", createFunctionFactory<SetSpacelessTxEnabledFunction>() },
    { "setTransmitPowerControl", createFunctionFactory<SetTransmitPowerControlFunction>() },
    { "setTransmitPowerLevel", createFunctionFactory<SetTransmitPowerLevelFunction>() },
    { "setXtalCalValue", createFunctionFactory<SetXtalCalValueFunction>() },
    { "setXtalRegValue", createFunctionFactory<SetXtalRegValueFunction>() },
    { "setZwdfsConfiguration", createFunctionFactory<SetZwdfsConfigurationFunction>() },
    { "startCalibration", createFunctionFactory<StartCalibrationFunction>() },
    { "startCw", createFunctionFactory<StartCwFunction>() },
    { "startTx", createFunctionFactory<StartTxFunction>() },
    { "startRxPer", createFunctionFactory<StartRxPerFunction>() },
    { "stopCw", createFunctionFactory<StopCwFunction>() },
    { "stopTx", createFunctionFactory<StopTxFunction>() },
    { "stopRxPer", createFunctionFactory<StopRxPerFunction>() },
    { "validateBeamformingHeaderRegister", createFunctionFactory<ValidateBeamformingHeaderRegisterFunction>() },
    { "writeCalibrationFile", createFunctionFactory<WriteCalibrationFileFunction>() },
    { "writeMemory", createFunctionFactory<WriteMemoryFunction>() },
    { "writeNvm", createFunctionFactory<WriteNvmFunction>() },
    { "writeRegister", createFunctionFactory<WriteRegisterFunction>() },
};

std::vector<std::string> getFunctionNames()
{
    std::vector<std::string> functionNames;
    for (const auto& entry : g_functions) {
        functionNames.push_back(entry.first);
    }
    return functionNames;
}

std::unique_ptr<Function> createFunction(const std::string& name)
{
    if (g_functions.count(name) == 0) {
        throw std::runtime_error("Invalid function. Run the 'api' command to get a list of available functions");
    }

    return g_functions.at(name)();
}

bool parseCmdLine(std::vector<std::string>& args, TCLAP::CmdLine& cmd, const std::function<void()>& validator = nullptr)
{
    try {
        cmd.setExceptionHandling(false);
        cmd.parse(args);

        if (validator) {
            validator();
        }
    } catch (const TCLAP::ArgException& e) {
        std::cerr << "PARSE ERROR: " << e.argId() << std::endl
                  << "\t" << e.error() << std::endl;

        TCLAP::StdOutput out;
        out.usage(cmd);
        return false;
    } catch (const TCLAP::ExitException&) {
        return false;
    }

    return true;
}

bool StringToMacAddress(const std::string& s, std::array<uint8_t, dut::cardInfoMacAddressSize>& macAddress)
{
    if (s.length() != dut::cardInfoMacAddressSize * 3 - 1) {
        return false;
    }

    size_t index = 0;
    const char separators[] = { ':', '-', '\0' };

    bool result = false;
    while ((!result) && ('\0' != separators[index])) {
        int n;
        std::array<int, dut::cardInfoMacAddressSize> bytes;
        const std::string formatTemplate = "%x %x %x %x %x %x";
        std::string format = formatTemplate;

        std::replace(format.begin(), format.end(), ' ', separators[index]);

        n = sscanf_s(s.c_str(), format.c_str(), &bytes[0], &bytes[1], &bytes[2], &bytes[3], &bytes[4], &bytes[5]);
        if ((n > 0) && (dut::cardInfoMacAddressSize == static_cast<size_t>(n))) {
            for (size_t i = 0; i < dut::cardInfoMacAddressSize; i++) {
                macAddress[i] = static_cast<uint8_t>(bytes[i]);
            }
            result = true;
        } else {
            index++;
        }
    }

    return result;
}

bool StringToSerialNumber(const std::string& s, std::array<uint8_t, dut::cardInfoSerialNumberSize>& serialNumber)
{
    if (s.length() != dut::cardInfoSerialNumberSize * 2) {
        return false;
    }

    for (char c : s) {
        if (!isdigit(c)) {
            return false;
        }
    }

    // Convert the serial number string to a U32 number
    uint32_t sn = 0;
    int n = sscanf_s(s.c_str(), "%u", &sn);
    if (n != 1) {
        return false;
    }

    // Convert the U32 number to byte array (little endian)
    memcpy(serialNumber.data(), &sn, sizeof(sn));

    return true;
}

std::vector<uint8_t> HexStringToBytes(const std::string& hex)
{
    if ((hex.length() % 2) != 0) {
        throw std::invalid_argument("Odd sized hexadecimal string");
    }

    std::vector<uint8_t> bytes;

    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);

        const char* nptr = byteString.c_str();
        char* endptr = nullptr;
        auto byte = static_cast<uint8_t>(strtol(nptr, &endptr, 16));
        if (endptr[0] != 0) {
            throw std::runtime_error("Invalid hexadecimal value at index #" + std::to_string(i / 2));
        }

        bytes.push_back(byte);
    }

    return bytes;
}

std::string ByteArrayToString(const std::vector<uint8_t>& data)
{
    std::stringstream stream;
    for (size_t i = 0; i < data.size(); i++) {
        if (i > 0) {
            if (i % maxBytesPerLine == 0) {
                stream << "\n";
            } else {
                stream << " ";
            }
        }
        stream << std::setfill('0') << std::setw(2) << std::hex << +data.at(i);
    }
    stream << "\n";

    return stream.str();
}

bool DriverInitFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::SwitchArg snifferModeArg(
        "",
        "sniffer-mode",
        "Sniffer mode.",
        cmd,
        m_snifferMode);

    TCLAP::ValueArg<uint32_t> memoryTypeArg(
        "t",
        "memory-type",
        "Memory type. Valid values: " + validValuesForNvMemoryTypeString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_memoryType)),
        false,
        static_cast<uint32_t>(m_memoryType),
        "Memory type",
        cmd);

    TCLAP::ValueArg<uint32_t> memorySizeArg(
        "s",
        "memory-size",
        "Memory size. Valid values: " + validValuesForNvMemorySizeString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_memorySize)),
        false,
        static_cast<uint32_t>(m_memorySize),
        "Memory size",
        cmd);

    TCLAP::ValueArg<uint32_t> bandArg(
        "b",
        "band",
        "Band. Valid values: " + validValuesForBandString + ". Default value: " + dut::toString(m_band),
        false,
        static_cast<uint32_t>(m_band),
        "Band",
        cmd);

    auto validator = [&memoryTypeArg, &memorySizeArg, &bandArg]() {
        if (validValuesForNvMemoryType.find(static_cast<dut::NvMemoryType>(memoryTypeArg.getValue())) == validValuesForNvMemoryType.end()) {
            throw TCLAP::ArgException("Invalid value", memoryTypeArg.longID());
        }

        if (validValuesForNvMemorySize.find(static_cast<dut::NvMemorySize>(memorySizeArg.getValue())) == validValuesForNvMemorySize.end()) {
            throw TCLAP::ArgException("Invalid value", memorySizeArg.longID());
        }

        auto band = static_cast<dut::Band>(bandArg.getValue());
        if ((band != dut::Band::BAND_INVALID) && (validValuesForBand.find(band) == validValuesForBand.end())) {
            throw TCLAP::ArgException("Invalid value", bandArg.longID());
        }
    };

    if (!parseCmdLine(args, cmd, validator)) {
        return false;
    }

    m_snifferMode = snifferModeArg.getValue();
    m_memoryType = static_cast<dut::NvMemoryType>(memoryTypeArg.getValue());
    m_memorySize = static_cast<dut::NvMemorySize>(memorySizeArg.getValue());
    m_band = static_cast<dut::Band>(bandArg.getValue());

    return true;
}

bool DriverInitFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->driverInit(m_snifferMode, m_memoryType, m_memorySize, m_band);
}

bool DriverReleaseFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->driverRelease();
}

bool CalculateMaxPacketLengthFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint32_t> phyModeArg(
        "p",
        "phy-mode",
        "PHY Mode. Valid values: " + validValuesForPhyModeString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_phyMode)),
        false,
        static_cast<uint32_t>(m_phyMode),
        "PHY mode",
        cmd);

    auto validator = [&phyModeArg]() {
        if (validValuesForPhyMode.find(static_cast<dut::PhyMode>(phyModeArg.getValue())) == validValuesForPhyMode.end()) {
            throw TCLAP::ArgException("Invalid value", phyModeArg.longID());
        }
    };

    if (!parseCmdLine(args, cmd, validator)) {
        return false;
    }

    m_phyMode = static_cast<dut::PhyMode>(phyModeArg.getValue());

    return true;
}

bool CalculateMaxPacketLengthFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    uint32_t maxPacketLength;

    bool ok = dut->calculateMaxPacketLength(m_phyMode, maxPacketLength);
    if (ok) {
        context.getConsole().cout("Maximum packet length: " + dut::toString(maxPacketLength) + " bytes\n");
    }

    return ok;
}

bool CalculatePacketLengthFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint32_t> phyModeArg(
        "p",
        "phy-mode",
        "PHY Mode. Valid values: " + validValuesForPhyModeString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_phyMode)),
        false,
        static_cast<uint32_t>(m_phyMode),
        "PHY mode",
        cmd);

    TCLAP::ValueArg<uint32_t> signalBandwidthArg(
        "b",
        "bandwidth",
        "Signal bandwidth. Valid values: " + validValuesForBandwidthString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_signalBandwidth)),
        false,
        static_cast<uint32_t>(m_signalBandwidth),
        "Bandwidth",
        cmd);

    TCLAP::ValueArg<uint32_t> mcsArg(
        "m",
        "mcs",
        "MCS (Modulation Coding Scheme). Valid values: " + validValuesForMcsString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_mcs)),
        false,
        static_cast<uint32_t>(m_mcs),
        "MCS",
        cmd);

    TCLAP::ValueArg<uint16_t> nssArg(
        "n",
        "nss",
        "Number of Spatial Streams. Default value: " + std::to_string(static_cast<uint16_t>(m_nss)),
        false,
        static_cast<uint16_t>(m_nss),
        "Number",
        cmd);

    TCLAP::ValueArg<uint32_t> numSymbolsArg(
        "s",
        "num-symbols",
        "Number of OFDM data symbols",
        false,
        m_numSymbols,
        "Number",
        cmd);

    auto validator = [&phyModeArg, &mcsArg]() {
        if (validValuesForPhyMode.find(static_cast<dut::PhyMode>(phyModeArg.getValue())) == validValuesForPhyMode.end()) {
            throw TCLAP::ArgException("Invalid value", phyModeArg.longID());
        }

        if (validValuesForMcs.find(static_cast<dut::Mcs>(mcsArg.getValue())) == validValuesForMcs.end()) {
            throw TCLAP::ArgException("Invalid value", mcsArg.longID());
        }
    };

    if (!parseCmdLine(args, cmd, validator)) {
        return false;
    }

    m_phyMode = static_cast<dut::PhyMode>(phyModeArg.getValue());
    m_signalBandwidth = static_cast<dut::Bandwidth>(signalBandwidthArg.getValue());
    m_mcs = static_cast<dut::Mcs>(mcsArg.getValue());
    m_nss = static_cast<uint8_t>(nssArg.getValue());
    m_numSymbols = numSymbolsArg.getValue();

    return true;
}

bool CalculatePacketLengthFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    uint32_t packetLength;

    bool ok = dut->calculatePacketLength(m_phyMode, m_signalBandwidth, m_mcs, m_nss, m_numSymbols, packetLength);
    if (ok) {
        context.getConsole().cout("Packet length: " + dut::toString(packetLength) + " bytes\n");
    }

    return ok;
}

bool FlushNvmFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->flushNvm();
}

bool GetAvailableRxAntennaMaskFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    dut::AntennaMask antennaMask;

    bool ok = dut->getAvailableRxAntennaMask(antennaMask);
    if (ok) {
        context.getConsole().cout("Available RX antenna mask: " + dut::toHexString(antennaMask) + "\n");
    }

    return ok;
}

bool GetAvailableTxAntennaMaskFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    dut::AntennaMask antennaMask;

    bool ok = dut->getAvailableTxAntennaMask(antennaMask);
    if (ok) {
        context.getConsole().cout("Available TX antenna mask: " + dut::toHexString(antennaMask) + "\n");
    }

    return ok;
}

bool GetBandFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    dut::Band band;

    bool ok = dut->getBand(band);
    if (ok) {
        context.getConsole().cout("Band: " + dut::toString(band) + "\n");
    }

    return ok;
}

bool GetSupportedBandsFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    std::vector<dut::Band> supportedBands;

    bool ok = dut->getSupportedBands(supportedBands);
    if (ok) {
        context.getConsole().cout("Supported bands: " + dut::toString(supportedBands.begin(), supportedBands.end()) + "\n");
    }

    return ok;
}

bool GetBbicCddValuesFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint16_t> numTxAntennasArg(
        "n",
        "num-tx-antennas",
        "Number of TX antennas. Default value: " + std::to_string(static_cast<uint16_t>(m_numTxAntennas)),
        false,
        static_cast<uint16_t>(m_numTxAntennas),
        "Number",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_numTxAntennas = static_cast<uint8_t>(numTxAntennasArg.getValue());

    return true;
}

bool GetBbicCddValuesFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    uint32_t offset1;
    uint32_t offset2;
    uint32_t offset3;

    bool ok = dut->getBbicCddValues(m_numTxAntennas, offset1, offset2, offset3);
    if (ok) {
        context.getConsole().cout("offset1: " + dut::toString(offset1) + ", offset2: " + dut::toString(offset2) + ", offset3: " + dut::toString(offset3) + "\n");
    }

    return ok;
}

bool GetCalibrationFileVersionFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    dut::CalibrationFileVersion version;
    dut::CalibrationFileSubversion subversion;

    bool ok = dut->getCalibrationFileVersion(version, subversion);
    if (ok) {
        context.getConsole().cout("Version: " + dut::toString(version) + ", Subversion: " + dut::toString(subversion) + "\n");
    }

    return ok;
}

bool GetCardInfoFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    uint8_t countryCode;
    std::array<uint8_t, dut::cardInfoMacAddressSize> macAddress;
    std::array<uint8_t, dut::cardInfoSerialNumberSize> serialNumber;
    uint8_t week;
    uint8_t year;

    bool ok = dut->getCardInfo(countryCode, macAddress, serialNumber, week, year);
    if (ok) {
        std::stringstream macAddressStream;
        for (size_t i = 0; i < dut::cardInfoMacAddressSize; i++) {
            if (i > 0) {
                macAddressStream << ":";
            }
            macAddressStream << std::setfill('0') << std::setw(2) << std::hex << +macAddress.at(i);
        }
        std::stringstream serialNumberStream;
        for (size_t i = 0; i < dut::cardInfoSerialNumberSize; i++) {
            serialNumberStream << std::setfill('0') << std::setw(2) << std::hex << +serialNumber.at(i);
        }

        context.getConsole().cout("countryCode: " + dut::toString(countryCode) + ", macAddress: " + macAddressStream.str() + ", serialNumber: " + serialNumberStream.str() + ", week: " + dut::toString(week) + ", year: " + dut::toString(year) + "\n");
    }

    return ok;
}

bool GetChipIdFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    dut::ChipID chipId;

    bool ok = dut->getChipId(chipId);
    if (ok) {

        context.getConsole().cout("chipId: " + dut::toHexString(static_cast<uint16_t>(chipId)) + "\n");
    }

    return ok;
}

bool GetComponentVersionFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint32_t> componentArg(
        "c",
        "component",
        "Versioned component: CV = 0, MAC = 1, PSD = 2, REGULATORY = 3, TXSENDER = 4, GENRISC_HOST_INTERFACE = 5, GENRISC_RXHANDLER = 6, GENRISC_500B_PROGMODEL = 7, GENRISC_600_PROGMODEL = 8, GENRISC_600B_PROGMODEL = 9, GENRISC_600D2_PROGMODEL. Default value: " + std::to_string(static_cast<uint32_t>(m_component)),
        false,
        static_cast<uint32_t>(m_component),
        "Component",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_component = static_cast<dut::VersionedComponent>(componentArg.getValue());

    return true;
}

bool GetComponentVersionFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    std::string version;

    bool ok = dut->getComponentVersion(m_component, version);
    if (ok) {

        context.getConsole().cout("version: " + version + "\n");
    }

    return ok;
}

bool GetEnabledRxAntennaMaskFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    dut::AntennaMask antennaMask;

    bool ok = dut->getEnabledRxAntennaMask(antennaMask);
    if (ok) {
        context.getConsole().cout("Enabled RX antenna mask: " + dut::toHexString(antennaMask) + "\n");
    }

    return ok;
}

bool GetEnabledTxAntennaMaskFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    dut::AntennaMask antennaMask;

    bool ok = dut->getEnabledTxAntennaMask(antennaMask);
    if (ok) {
        context.getConsole().cout("Enabled TX antenna mask: " + dut::toHexString(antennaMask) + "\n");
    }

    return ok;
}

bool GetFemTypeFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    dut::FemType femType;

    bool ok = dut->getFemType(femType);
    if (ok) {
        context.getConsole().cout("FEM type: " + dut::toString(femType) + "\n");
    }

    return ok;
}

bool GetNmseValuesFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    std::array<std::array<int32_t, dut::dpdTotalCalibrationPoints>, dut::maxNumTxAntennas> values {};

    bool ok = dut->getNmseValues(values);
    if (ok) {
        std::string s {};
        s.append("DPD NMSE Values:\n");
        for (size_t i = 0; i < dut::maxNumTxAntennas; i++) {
            for (size_t j = 0; j < dut::dpdTotalCalibrationPoints; j++) {
                if (j > 0) {
                    s.append(",");
                }
                s.append(std::to_string(values[i][j]));
            }
            s.append("\n");
        }

        context.getConsole().cout(s);
    }

    return ok;
}

bool GetHardwareTypeFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    dut::HardwareType hardwareType;

    bool ok = dut->getHardwareType(hardwareType);
    if (ok) {
        std::string hwTypeString = "UNKNOWN";
        if (hardwareType == dut::HardwareType::HARDWARE_TYPE_GEN6) {
            hwTypeString = "GEN6";
        } else if (hardwareType == dut::HardwareType::HARDWARE_TYPE_GEN7) {
            hwTypeString = "GEN7";
        }
        context.getConsole().cout("Hardware type: " + hwTypeString + "\n");
    }

    return ok;
}

bool GetInbandRssiFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    std::array<int16_t, dut::maxNumRxAntennas> rssi;

    bool ok = dut->getInbandRssi(rssi);
    if (ok) {
        context.getConsole().cout("RSSI: " + dut::toString(rssi.begin(), rssi.end()) + "\n");
    }

    return ok;
}

bool GetMacPacketCountersFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    uint32_t receivedPackets;

    bool ok = dut->getMacPacketCounters(receivedPackets);
    if (ok) {
        context.getConsole().cout("Received packets: " + dut::toString(receivedPackets) + "\n");
    }

    return ok;
}

bool GetMpduPacketCountersFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    uint32_t receivedPackets;
    uint32_t receivedErrorPackets;

    bool ok = dut->getMpduPacketCounters(receivedPackets, receivedErrorPackets);
    if (ok) {
        context.getConsole().cout("Received packets: " + dut::toString(receivedPackets) + ", Error packets: " + dut::toString(receivedErrorPackets) + "\n");
    }

    return ok;
}

bool GetNvmSizeFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    size_t size;

    bool ok = dut->getNvmSize(size);
    if (ok) {
        context.getConsole().cout("NVM size: " + dut::toString(size) + "\n");
    }

    return ok;
}

bool GetNvmTypeFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    dut::NvMemoryType type;

    bool ok = dut->getNvmType(type);
    if (ok) {
        context.getConsole().cout("NVM type: " + dut::toString(type) + "\n");
    }

    return ok;
}

bool GetNvmVersionFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    uint8_t version;

    bool ok = dut->getNvmVersion(version);
    if (ok) {
        context.getConsole().cout("NVM version: " + dut::toString(version) + "\n");
    }

    return ok;
}

bool GetPhyModeFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    dut::PhyMode phyMode;

    bool ok = dut->getPhyMode(phyMode);
    if (ok) {
        context.getConsole().cout("PHY mode: " + dut::toString(phyMode) + "\n");
    }

    return ok;
}

bool GetPhyPacketCountersFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    uint32_t receivedPackets;
    uint32_t crcErrors;
    uint32_t forwardedPackets;

    bool ok = dut->getPhyPacketCounters(receivedPackets, crcErrors, forwardedPackets);
    if (ok) {
        context.getConsole().cout("Received packets: " + dut::toString(receivedPackets) + ", CRC errors: " + dut::toString(crcErrors) + ", forwarded packets: " + dut::toString(forwardedPackets) + "\n");
    }

    return ok;
}

bool GetProductionFlagFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    bool productionFlag;

    bool ok = dut->getProductionFlag(productionFlag);
    if (ok) {
        context.getConsole().cout("Production flag: " + dut::toString(productionFlag) + "\n");
    }

    return ok;
}

bool GetRxEvmFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    std::array<uint8_t, dut::maxNumRxAntennas> rxEvm;

    bool ok = dut->getRxEvm(rxEvm);
    if (ok) {
        std::array<double, dut::maxNumRxAntennas> result;
        std::transform(rxEvm.begin(), rxEvm.end(), result.begin(), [](uint8_t value) {
            // Convert to dB
            // The multiplying factor was 0.376 in Wave500 (rxEvm*10*log2/8).
            // In Wave600 and Wave700 we use 0.5
            return value * 0.5;
        });

        context.getConsole().cout("RX EVM: " + dut::toString(result.begin(), result.end()) + "\n");
    }

    return ok;
}

bool GetRxRateInfoFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    uint8_t mcs;
    uint8_t nss;
    dut::PhyMode phyMode;

    bool ok = dut->getPhyMode(phyMode) && dut->getRxRateInfo(mcs, nss);

    if (ok) {
        std::string rateInfo = "RX Rate Info: ";
        if ((phyMode == dut::PhyMode::PHY_MODE_A) || (phyMode == dut::PhyMode::PHY_MODE_G)) {
            if (mcsMap11ag.find(mcs) != mcsMap11ag.end()) {
                rateInfo += mcsMap11ag.at(mcs);
            }
        } else if (phyMode == dut::PhyMode::PHY_MODE_B) {
            if (mcsMap11b.find(mcs) != mcsMap11b.end()) {
                rateInfo += mcsMap11b.at(mcs);
            }
        } else {
            rateInfo += "MCS=" + dut::toString(mcs) + ", NSS=" + dut::toString(nss);
        }
        context.getConsole().cout(rateInfo + "\n");
    }

    return ok;
}

bool GetTemperatureFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    float temperature;

    bool ok = dut->getTemperature(temperature);
    if (ok) {
        context.getConsole().cout("Temperature: " + dut::toString(temperature, 2) + " degrees Celsius\n");
    }

    return ok;
}

bool GetTransmitVoltagesFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    std::array<uint32_t, dut::maxNumTxAntennas> voltages;

    bool ok = dut->getTransmitVoltages(voltages);
    if (ok) {
        context.getConsole().cout("TX voltages: " + dut::toString(voltages.begin(), voltages.end()) + "\n");
    }

    return ok;
}

bool GetXtalCalValueFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    uint16_t value;

    bool ok = dut->getXtalCalValue(value);
    if (ok) {
        context.getConsole().cout("Xtal cal value: " + dut::toHexString(value) + "\n");
    }

    return ok;
}

bool GetXtalRegValueFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    uint16_t value;

    bool ok = dut->getXtalRegValue(value);
    if (ok) {
        context.getConsole().cout("Xtal reg value: " + dut::toHexString(value) + "\n");
    }

    return ok;
}

bool GetZwdfsStatusFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    dut::AntennaMask antennaMask;
    bool enabled;

    bool ok = dut->getZwdfsStatus(antennaMask, enabled);
    if (ok) {
        context.getConsole().cout("ZWDFS antenna mask: " + dut::toHexString(antennaMask) + "\n");
        context.getConsole().cout("ZWDFS enabled: " + dut::toString(enabled) + "\n");
    }

    return ok;
}

bool LoadBeamformingMatrixFromFileFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<std::string> headerFileNameArg(
        "f",
        "header-filename",
        "Name of the file containing the beamforming matrix header.",
        true,
        "",
        "File name",
        cmd);

    TCLAP::ValueArg<std::string> valuesFileNameArg(
        "v",
        "values-filename",
        "Name of the file containing the beamforming matrix values.",
        true,
        "",
        "File name",
        cmd);

    TCLAP::ValueArg<std::string> extValuesEhtFileNameArg(
        "e",
        "ext-values-eht-filename",
        "Name of the file containing the extended EHT beamforming matrix values (optional, for EHT 160MHz/320MHz).",
        false,
        "",
        "File name",
        cmd);

    TCLAP::ValueArg<std::string> secondaryHeaderFileNameArg(
        "",
        "secondary-header-filename",
        "Name of the file containing the secondary segment beamforming matrix header (optional, for EHT 320MHz).",
        false,
        "",
        "File name",
        cmd);

    TCLAP::ValueArg<std::string> secondaryValuesFileNameArg(
        "",
        "secondary-values-filename",
        "Name of the file containing the secondary segment beamforming matrix values (optional, for EHT 320MHz).",
        false,
        "",
        "File name",
        cmd);

    TCLAP::ValueArg<std::string> secondaryExtValuesEhtFileNameArg(
        "",
        "secondary-ext-values-eht-filename",
        "Name of the file containing the secondary band extended EHT beamforming matrix values (optional, for EHT 320MHz).",
        false,
        "",
        "File name",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_headerFileName = static_cast<std::string>(headerFileNameArg.getValue());
    m_valuesFileName = static_cast<std::string>(valuesFileNameArg.getValue());
    m_extValuesEhtFileName = static_cast<std::string>(extValuesEhtFileNameArg.getValue());
    m_secondaryHeaderFileName = static_cast<std::string>(secondaryHeaderFileNameArg.getValue());
    m_secondaryValuesFileName = static_cast<std::string>(secondaryValuesFileNameArg.getValue());
    m_secondaryExtValuesEhtFileName = static_cast<std::string>(secondaryExtValuesEhtFileNameArg.getValue());

    return true;
}

bool LoadBeamformingMatrixFromFileFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    // Create primary file set
    dut::BeamformingFilePathSet_t primarySet;
    primarySet.headerFile = m_headerFileName.c_str();
    primarySet.valuesFile = m_valuesFileName.c_str();
    primarySet.extValuesEhtFile = m_extValuesEhtFileName.empty() ? nullptr : m_extValuesEhtFileName.c_str();

    // Create secondary file set if provided
    dut::BeamformingFilePathSet_t secondarySet;
    if (!m_secondaryHeaderFileName.empty() && !m_secondaryValuesFileName.empty()) {
        secondarySet.headerFile = m_secondaryHeaderFileName.c_str();
        secondarySet.valuesFile = m_secondaryValuesFileName.c_str();
        secondarySet.extValuesEhtFile = m_secondaryExtValuesEhtFileName.empty() ? nullptr : m_secondaryExtValuesEhtFileName.c_str();
    } else {
        secondarySet.headerFile = nullptr;
        secondarySet.valuesFile = nullptr;
        secondarySet.extValuesEhtFile = nullptr;
    }

    return dut->loadBeamformingMatrixFromFileSet(primarySet, secondarySet);
}

bool LoadNvmFromFileFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<std::string> fileNameArg(
        "f",
        "filename",
        "Name of the file containing the non-volatile-memory binary data.",
        true,
        "",
        "File name",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_fileName = static_cast<std::string>(fileNameArg.getValue());

    return true;
}

bool LoadNvmFromFileFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->loadNvmFromFile(m_fileName);
}

bool ReadMemoryFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint32_t> moduleArg(
        "m",
        "chip-module",
        "Chip module. Valid values: " + validValuesForChipModuleString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_module)),
        false,
        static_cast<uint32_t>(m_module),
        "Chip module",
        cmd);

    TCLAP::ValueArg<std::string> addressArg(
        "a",
        "address",
        "Starting address. Default value: " + std::to_string(m_address),
        false,
        std::to_string(m_address),
        "Address",
        cmd);

    TCLAP::ValueArg<size_t> lengthArg(
        "l",
        "length",
        "Number of bytes to read.",
        true,
        m_length,
        "Number",
        cmd);

    auto validator = [&moduleArg]() {
        if (validValuesForChipModule.find(static_cast<dut::ChipModule>(moduleArg.getValue())) == validValuesForChipModule.end()) {
            throw TCLAP::ArgException("Invalid value", moduleArg.longID());
        }
    };

    if (!parseCmdLine(args, cmd, validator)) {
        return false;
    }

    m_module = static_cast<dut::ChipModule>(moduleArg.getValue());
    m_address = static_cast<size_t>(strtol(addressArg.getValue().c_str(), nullptr, 0));
    m_length = lengthArg.getValue();

    return true;
}

bool ReadMemoryFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    std::vector<uint8_t> buffer(m_length);

    bool ok = dut->readMemory(m_module, m_address, buffer.data(), m_length);
    if (ok) {
        context.getConsole().cout(ByteArrayToString(buffer));
    }

    return ok;
}

bool ReadNvmFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<std::string> addressArg(
        "a",
        "address",
        "Starting address. Default value: " + std::to_string(m_address),
        false,
        std::to_string(m_address),
        "Address",
        cmd);

    TCLAP::ValueArg<size_t> lengthArg(
        "l",
        "length",
        "Number of bytes to read.",
        true,
        m_length,
        "Number",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_address = static_cast<size_t>(strtol(addressArg.getValue().c_str(), nullptr, 0));
    m_length = lengthArg.getValue();

    return true;
}

bool ReadNvmFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    std::vector<uint8_t> buffer(m_length);

    bool ok = dut->readNvm(m_address, buffer.data(), m_length, false);
    if (ok) {
        context.getConsole().cout(ByteArrayToString(buffer));
    }

    return ok;
}

bool ReadRegisterFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint32_t> moduleArg(
        "m",
        "chip-module",
        "Chip module. Valid values: " + validValuesForChipModuleString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_module)),
        false,
        static_cast<uint32_t>(m_module),
        "Chip module",
        cmd);

    TCLAP::ValueArg<std::string> addressArg(
        "a",
        "address",
        "Starting address. Default value: " + std::to_string(m_address),
        false,
        std::to_string(m_address),
        "Address",
        cmd);

    TCLAP::ValueArg<std::string> maskArg(
        "k",
        "mask",
        "Mask. Default value: " + dut::toHexString(m_mask),
        false,
        dut::toHexString(m_mask),
        "Mask",
        cmd);

    auto validator = [&moduleArg]() {
        if (validValuesForChipModule.find(static_cast<dut::ChipModule>(moduleArg.getValue())) == validValuesForChipModule.end()) {
            throw TCLAP::ArgException("Invalid value", moduleArg.longID());
        }
    };

    if (!parseCmdLine(args, cmd, validator)) {
        return false;
    }

    m_module = static_cast<dut::ChipModule>(moduleArg.getValue());
    m_address = static_cast<size_t>(strtol(addressArg.getValue().c_str(), nullptr, 0));
    m_mask = static_cast<uint32_t>(strtol(maskArg.getValue().c_str(), nullptr, 0));

    return true;
}

bool ReadRegisterFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    uint32_t value;

    bool ok = dut->readRegister(m_module, m_address, m_mask, value);
    if (ok) {
        context.getConsole().cout("Register value: " + dut::toHexString(value) + "\n");
    }

    return ok;
}

bool ResetMacPacketCountersFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->resetMacPacketCounters();
}

bool ResetMpduPacketCountersFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->resetMpduPacketCounters();
}

bool ResetPhyPacketCountersFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->resetPhyPacketCounters();
}

bool RxMeasureFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint32_t> numCaptureArg(
        "n",
        "num-captures",
        "Number of measurements. Default value: " + std::to_string(m_numCapture),
        false,
        m_numCapture,
        "Number",
        cmd);

    TCLAP::ValueArg<uint32_t> intervalArg(
        "i",
        "interval",
        "Interval between measurements in milliseconds. Default value: " + std::to_string(m_interval),
        false,
        m_interval,
        "Number",
        cmd);

    TCLAP::SwitchArg disabledArg(
        "",
        "disable",
        "Disable Rx (False if argument not set).",
        cmd,
        m_disabled);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_numCapture = numCaptureArg.getValue();
    m_interval = intervalArg.getValue();
    m_disabled = disabledArg.getValue();

    return true;
}

bool RxMeasureFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->rxMeasure(m_numCapture, m_interval, m_disabled);
}

bool SaveNvmToFileFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<std::string> fileNameArg(
        "f",
        "filename",
        "Name of the output file.",
        true,
        "",
        "File name",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_fileName = static_cast<std::string>(fileNameArg.getValue());

    return true;
}

bool SaveNvmToFileFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->saveNvmToFile(m_fileName);
}

bool SetBbicCddValuesFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint16_t> numTxAntennasArg(
        "n",
        "num-tx-antennas",
        "Number of TX antennas. Default value: " + std::to_string(static_cast<uint16_t>(m_numTxAntennas)),
        false,
        static_cast<uint16_t>(m_numTxAntennas),
        "Number",
        cmd);

    TCLAP::ValueArg<uint32_t> offset1Arg(
        "1",
        "offset1",
        "Offset 1. Default value: " + std::to_string(m_offset1),
        false,
        m_offset1,
        "Offset",
        cmd);

    TCLAP::ValueArg<uint32_t> offset2Arg(
        "2",
        "offset2",
        "Offset 2. Default value: " + std::to_string(m_offset2),
        false,
        m_offset2,
        "Offset",
        cmd);

    TCLAP::ValueArg<uint32_t> offset3Arg(
        "3",
        "offset3",
        "Offset 3. Default value: " + std::to_string(m_offset3),
        false,
        m_offset3,
        "Offset",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_numTxAntennas = static_cast<uint8_t>(numTxAntennasArg.getValue());
    m_offset1 = offset1Arg.getValue();
    m_offset2 = offset2Arg.getValue();
    m_offset3 = offset3Arg.getValue();

    return true;
}

bool SetBbicCddValuesFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->setBbicCddValues(m_numTxAntennas, m_offset1, m_offset2, m_offset3);
}

bool SetCalibrationFileVersionFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint16_t> versionArg(
        "r",
        "cal-file-version",
        "Calibration file version.",
        true,
        static_cast<uint16_t>(m_version),
        "Number",
        cmd);

    TCLAP::ValueArg<uint16_t> subversionArg(
        "s",
        "cal-file-subversion",
        "Calibration file subversion.",
        true,
        static_cast<uint16_t>(m_subversion),
        "Number",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_version = static_cast<dut::CalibrationFileVersion>(versionArg.getValue());
    m_subversion = static_cast<dut::CalibrationFileSubversion>(subversionArg.getValue());

    return true;
}

bool SetCalibrationFileVersionFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->setCalibrationFileVersion(m_version, m_subversion);
}

bool SetCardInfoFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint16_t> countryCodeArg(
        "c",
        "country-code",
        "Country code.",
        true,
        static_cast<uint16_t>(m_countryCode),
        "Country Code",
        cmd);

    TCLAP::ValueArg<std::string> macAddressArg(
        "a",
        "mac-address",
        "MAC address.",
        true,
        "",
        "MAC",
        cmd);

    TCLAP::ValueArg<std::string> serialNumberArg(
        "s",
        "serial-number",
        "Serial number.",
        true,
        "",
        "SN",
        cmd);

    TCLAP::ValueArg<uint16_t> weekArg(
        "w",
        "week",
        "Week of the year.",
        true,
        static_cast<uint16_t>(m_week),
        "Week",
        cmd);

    TCLAP::ValueArg<uint16_t> yearArg(
        "y",
        "year",
        "Year.",
        true,
        static_cast<uint16_t>(m_year),
        "Year",
        cmd);

    auto validator = [this, &macAddressArg, &serialNumberArg]() {
        if (!StringToMacAddress(macAddressArg.getValue(), m_macAddress)) {
            throw TCLAP::ArgException("Invalid value", macAddressArg.longID());
        }
        if (!StringToSerialNumber(serialNumberArg.getValue(), m_serialNumber)) {
            throw TCLAP::ArgException("Invalid value", serialNumberArg.longID());
        }
    };

    if (!parseCmdLine(args, cmd, validator)) {
        return false;
    }

    m_countryCode = static_cast<uint8_t>(countryCodeArg.getValue());
    m_week = static_cast<uint8_t>(weekArg.getValue());
    m_year = static_cast<uint8_t>(yearArg.getValue());

    return true;
}

bool SetCardInfoFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->setCardInfo(m_countryCode, m_macAddress, m_serialNumber, m_week, m_year);
}

bool SetChannelFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint32_t> phyModeArg(
        "p",
        "phy-mode",
        "PHY Mode. Valid values: " + validValuesForPhyModeString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_phyMode)),
        false,
        static_cast<uint32_t>(m_phyMode),
        "PHY mode",
        cmd);

    TCLAP::ValueArg<uint32_t> spectrumBandwidthArg(
        "b",
        "bandwidth",
        "Spectrum bandwidth. Valid values: " + validValuesForBandwidthString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_spectrumBandwidth)),
        false,
        static_cast<uint32_t>(m_spectrumBandwidth),
        "Bandwidth",
        cmd);

    TCLAP::ValueArg<uint16_t> lowestChannelArg(
        "c",
        "channel",
        "Lowest channel number. Default value: " + std::to_string(static_cast<uint16_t>(m_lowestChannel)),
        false,
        static_cast<uint16_t>(m_lowestChannel),
        "Channel",
        cmd);

    TCLAP::ValueArg<uint16_t> primaryChannelIndexArg(
        "i",
        "index",
        "Primary channel index. Valid values range from 0 to 7, both inclusive. Default value: " + std::to_string(static_cast<uint16_t>(m_primaryChannelIndex)),
        false,
        static_cast<uint16_t>(m_primaryChannelIndex),
        "Index",
        cmd);

    TCLAP::ValueArg<std::string> regulationTypeArg(
        "r",
        "regulation_type",
        "Regulation type. Valid values: " + validValuesForRegulationTypeString + ". Default value: " + dut::toHexString(static_cast<uint8_t>(m_regulationType)),
        false,
        dut::toHexString(static_cast<uint8_t>(m_regulationType)),
        "RegulationType",
        cmd);

    auto validator = [&phyModeArg, &spectrumBandwidthArg, &primaryChannelIndexArg, &regulationTypeArg]() {
        if (validValuesForPhyMode.find(static_cast<dut::PhyMode>(phyModeArg.getValue())) == validValuesForPhyMode.end()) {
            throw TCLAP::ArgException("Invalid value", phyModeArg.longID());
        }

        if (validValuesForBandwidth.find(static_cast<dut::Bandwidth>(spectrumBandwidthArg.getValue())) == validValuesForBandwidth.end()) {
            throw TCLAP::ArgException("Invalid value", spectrumBandwidthArg.longID());
        }

        if (primaryChannelIndexArg.getValue() > 7) {
            throw TCLAP::ArgException("Invalid value", primaryChannelIndexArg.longID());
        }

        auto value = strtol(regulationTypeArg.getValue().c_str(), nullptr, 0);
        auto regulationType = static_cast<dut::RegulationType>(value);
        if (validValuesForRegulationType.find(regulationType) == validValuesForRegulationType.end()) {
            throw TCLAP::ArgException("Invalid value", regulationTypeArg.longID());
        }
    };

    if (!parseCmdLine(args, cmd, validator)) {
        return false;
    }

    m_phyMode = static_cast<dut::PhyMode>(phyModeArg.getValue());
    m_spectrumBandwidth = static_cast<dut::Bandwidth>(spectrumBandwidthArg.getValue());
    m_lowestChannel = static_cast<uint8_t>(lowestChannelArg.getValue());
    m_primaryChannelIndex = static_cast<uint8_t>(primaryChannelIndexArg.getValue());
    m_regulationType = static_cast<dut::RegulationType>(strtol(regulationTypeArg.getValue().c_str(), nullptr, 0));

    return true;
}

bool SetChannelFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->setChannel(m_phyMode, m_spectrumBandwidth, m_lowestChannel, m_primaryChannelIndex, m_regulationType);
}

bool SetClipperFunction::parse(std::vector<std::string>& args)
{

    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::SwitchArg enabledArg(
        "",
        "enabled",
        "Set clipper enabled (disabled if argument not set).",
        cmd,
        false);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_enabled = enabledArg.getValue();

    return true;
}

bool SetClipperFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->setClipper(m_enabled);
}

bool SetEnabledRxAntennaMaskFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<std::string> antennaMaskArg(
        "m",
        "antenna-mask",
        "RX antenna mask.",
        true,
        "",
        "Antenna Mask",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_antennaMask = strtol(antennaMaskArg.getValue().c_str(), nullptr, 0);

    return true;
}

bool SetEnabledRxAntennaMaskFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->setEnabledRxAntennaMask(m_antennaMask);
}

bool SetEnabledTxAntennaMaskFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<std::string> antennaMaskArg(
        "m",
        "antenna-mask",
        "TX antenna mask.",
        true,
        "",
        "Antenna Mask",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_antennaMask = strtol(antennaMaskArg.getValue().c_str(), nullptr, 0);

    return true;
}

bool SetEnabledTxAntennaMaskFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->setEnabledTxAntennaMask(m_antennaMask);
}

bool SetIfsFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint32_t> ifsArg(
        "i",
        "ifs",
        "Inter-frame space.",
        true,
        m_ifs,
        "Number",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_ifs = ifsArg.getValue();

    return true;
}

bool SetIfsFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->setIfs(m_ifs);
}

bool SetProductionFlagFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::SwitchArg productionFlagArg(
        "",
        "production-flag",
        "Production flag enabled (disabled if argument not set).",
        cmd,
        m_productionFlag);

    TCLAP::SwitchArg writeToNvmArg(
        "",
        "write-to-nvm",
        "Write to non-volatile memory enabled (disabled if argument not set).",
        cmd,
        m_writeToNvm);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_productionFlag = productionFlagArg.getValue();
    m_writeToNvm = writeToNvmArg.getValue();

    return true;
}

bool SetProductionFlagFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->setProductionFlag(m_productionFlag, m_writeToNvm);
}

bool SetRateFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint32_t> signalBandwidthArg(
        "b",
        "bandwidth",
        "Signal bandwidth. Valid values: " + validValuesForBandwidthString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_signalBandwidth)),
        false,
        static_cast<uint32_t>(m_signalBandwidth),
        "Bandwidth",
        cmd);

    TCLAP::ValueArg<uint32_t> mcsArg(
        "m",
        "mcs",
        "MCS (Modulation Coding Scheme). Valid values: " + validValuesForMcsString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_mcs)),
        false,
        static_cast<uint32_t>(m_mcs),
        "MCS",
        cmd);

    TCLAP::ValueArg<uint16_t> nssArg(
        "n",
        "nss",
        "Number of Spatial Streams. Default value: " + std::to_string(static_cast<uint16_t>(m_nss)),
        false,
        static_cast<uint16_t>(m_nss),
        "Number",
        cmd);

    TCLAP::ValueArg<uint32_t> giArg(
        "g",
        "gi",
        "GI (Guard Interval). Valid values: " + validValuesForGiString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_gi)),
        false,
        static_cast<uint32_t>(m_gi),
        "GI",
        cmd);

    TCLAP::ValueArg<uint32_t> ltfArg(
        "l",
        "ltf",
        "LTF (Long Training Field) duration. Valid values: " + validValuesForLtfString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_ltf)),
        false,
        static_cast<uint32_t>(m_ltf),
        "LTF",
        cmd);

    auto validator = [&signalBandwidthArg, &mcsArg, &giArg, &ltfArg]() {
        if (validValuesForBandwidth.find(static_cast<dut::Bandwidth>(signalBandwidthArg.getValue())) == validValuesForBandwidth.end()) {
            throw TCLAP::ArgException("Invalid value", signalBandwidthArg.longID());
        }

        if (validValuesForMcs.find(static_cast<dut::Mcs>(mcsArg.getValue())) == validValuesForMcs.end()) {
            throw TCLAP::ArgException("Invalid value", mcsArg.longID());
        }

        if (validValuesForGi.find(static_cast<dut::Gi>(giArg.getValue())) == validValuesForGi.end()) {
            throw TCLAP::ArgException("Invalid value", giArg.longID());
        }

        if (validValuesForLtf.find(static_cast<dut::Ltf>(ltfArg.getValue())) == validValuesForLtf.end()) {
            throw TCLAP::ArgException("Invalid value", ltfArg.longID());
        }
    };

    if (!parseCmdLine(args, cmd, validator)) {
        return false;
    }

    m_signalBandwidth = static_cast<dut::Bandwidth>(signalBandwidthArg.getValue());
    m_mcs = static_cast<dut::Mcs>(mcsArg.getValue());
    m_nss = static_cast<uint8_t>(nssArg.getValue());
    m_gi = static_cast<dut::Gi>(giArg.getValue());
    m_ltf = static_cast<dut::Ltf>(ltfArg.getValue());

    return true;
}

bool SetRateFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    float rateMbps;

    bool ok = dut->setRate(m_signalBandwidth, m_mcs, m_nss, m_gi, m_ltf, rateMbps);
    if (ok) {
        context.getConsole().cout("PHY rate: " + dut::toString(rateMbps, 1) + " Mbps\n");
    }

    return ok;
}

bool SetRuParamsFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<std::string> userOneRuArg(
        "o",
        "user-one",
        "RU Params of User One. Bits: 0-7 -> RU Allocation, Bit : 8 -> PS 160, Bits: 9-31 -> Reserved",
        true,
        std::to_string(m_userOne),
        "Number",
        cmd);

    TCLAP::ValueArg<std::string> userTwoRuArg(
        "t",
        "user-two",
        "RU Params of User Two. Bits: 0-7 -> RU Allocation, Bit : 8 -> PS 160, Bits: 9-31 -> Reserved except for 0x200(SU - User 2 is not valid)",
        false,
        std::to_string(dut::maxRuParamsUserTwoValue),
        "Number",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_userOne = static_cast<uint32_t>(strtol(userOneRuArg.getValue().c_str(), nullptr, 0));
    m_userTwo = static_cast<uint32_t>(strtol(userTwoRuArg.getValue().c_str(), nullptr, 0));

    return true;
}

bool SetRuParamsFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->setRuParams(m_userOne, m_userTwo);
}

bool SetRxAggregationEnabledFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::SwitchArg enabledArg(
        "",
        "enabled",
        "RX aggregation enabled (disabled if argument not set).",
        cmd,
        false);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_enabled = enabledArg.getValue();

    return true;
}

bool SetRxAggregationEnabledFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->setRxAggregationEnabled(m_enabled);
}

bool SetSpacelessTxEnabledFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::SwitchArg enabledArg(
        "",
        "enabled",
        "Spaceless TX enabled (disabled if argument not set).",
        cmd,
        false);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_enabled = enabledArg.getValue();

    return true;
}

bool SetSpacelessTxEnabledFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->setSpacelessTxEnabled(m_enabled);
}

bool SetTransmitPowerControlFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::SwitchArg closedLoopArg(
        "",
        "closed-loop",
        "Closed loop enabled (disabled if argument not set).",
        cmd,
        false);

    TCLAP::ValueArg<uint16_t> powerLimitArg(
        "p",
        "power-limit",
        "Transmit power limit. Default value: " + std::to_string(static_cast<uint16_t>(m_powerLimit)),
        false,
        static_cast<uint16_t>(m_powerLimit),
        "Number",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_closedLoop = closedLoopArg.getValue();
    m_powerLimit = static_cast<uint8_t>(powerLimitArg.getValue());

    return true;
}

bool SetTransmitPowerControlFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->setTransmitPowerControl(m_closedLoop, m_powerLimit);
}

bool SetTransmitPowerLevelFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<int16_t> powerLevelArg(
        "p",
        "power-level",
        "Transmit power level. Default value: " + std::to_string(static_cast<int16_t>(m_powerLevel)),
        false,
        static_cast<int16_t>(m_powerLevel),
        "Number",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_powerLevel = static_cast<uint8_t>(powerLevelArg.getValue());

    return true;
}

bool SetTransmitPowerLevelFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->setTransmitPowerLevel(m_powerLevel);
}

bool SetXtalCalValueFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint16_t> valueArg(
        "x",
        "xtal-value",
        "XTal value. Default value: " + std::to_string(m_value),
        false,
        m_value,
        "Number",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_value = valueArg.getValue();

    return true;
}

bool SetXtalCalValueFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->setXtalCalValue(m_value);
}

bool SetXtalRegValueFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint16_t> valueArg(
        "x",
        "xtal-value",
        "XTal value. Default value: " + std::to_string(m_value),
        false,
        m_value,
        "Number",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_value = valueArg.getValue();

    return true;
}

bool SetXtalRegValueFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->setXtalRegValue(m_value);
}

bool SetZwdfsConfigurationFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint16_t> zwdfsChannelArg(
        "c",
        "channel",
        "ZWDFS channel number. Default value: " + std::to_string(static_cast<uint16_t>(m_zwdfsChannel)),
        false,
        static_cast<uint16_t>(m_zwdfsChannel),
        "Channel",
        cmd);

    TCLAP::ValueArg<uint32_t> zwdfsBandwidthArg(
        "b",
        "bandwidth",
        "ZWDFS channel width. Valid values: " + validValuesForBandwidthString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_zwdfsBandwidth)),
        false,
        static_cast<uint32_t>(m_zwdfsBandwidth),
        "Bandwidth",
        cmd);

    TCLAP::ValueArg<uint32_t> radarDetectionBandwidthArg(
        "r",
        "radarDetectionBandwidth",
        "Radar detection bandwidth. Valid values: " + validValuesForRadarDetectionBandwidthString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_radarDetectionBandwidth)),
        false,
        static_cast<uint32_t>(m_radarDetectionBandwidth),
        "Bandwidth",
        cmd);

    auto validator = [&zwdfsBandwidthArg, &radarDetectionBandwidthArg]() {
        if (validValuesForBandwidth.find(static_cast<dut::Bandwidth>(zwdfsBandwidthArg.getValue())) == validValuesForBandwidth.end()) {
            throw TCLAP::ArgException("Invalid value", zwdfsBandwidthArg.longID());
        }

        if (validValuesForRadarDetectionBandwidth.find(static_cast<dut::Bandwidth>(radarDetectionBandwidthArg.getValue())) == validValuesForRadarDetectionBandwidth.end()) {
            throw TCLAP::ArgException("Invalid value", radarDetectionBandwidthArg.longID());
        }
    };

    if (!parseCmdLine(args, cmd, validator)) {
        return false;
    }

    m_zwdfsChannel = static_cast<uint8_t>(zwdfsChannelArg.getValue());
    m_zwdfsBandwidth = static_cast<dut::Bandwidth>(zwdfsBandwidthArg.getValue());
    m_radarDetectionBandwidth = static_cast<dut::Bandwidth>(radarDetectionBandwidthArg.getValue());

    return true;
}

bool SetZwdfsConfigurationFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->setZwdfsConfiguration(m_zwdfsChannel, m_zwdfsBandwidth, m_radarDetectionBandwidth);
}

bool StartCalibrationFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint16_t> calibrationType(
        "c",
        "calibrationType",
        "Calibration type. Default value (offline): " + std::to_string(static_cast<uint16_t>(m_calibrationType)),
        false,
        static_cast<uint16_t>(m_calibrationType),
        "Calibration type",
        cmd);

    TCLAP::ValueArg<uint16_t> maskType(
        "t",
        "maskType",
        "Use default or custom calibration mask. Default value (default mask): " + std::to_string(static_cast<uint16_t>(m_maskType)),
        false,
        static_cast<uint16_t>(m_maskType),
        "Mask type",
        cmd);

    TCLAP::ValueArg<std::string> mask(
        "m",
        "mask",
        "Calibration mask. Default value: " + std::to_string(m_mask),
        false,
        std::to_string(m_mask),
        "Mask",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_calibrationType = static_cast<uint8_t>(calibrationType.getValue());
    m_maskType = static_cast<uint8_t>(maskType.getValue());
    m_mask = static_cast<uint32_t>(strtol(mask.getValue().c_str(), nullptr, 0));

    return true;
}

bool StartCalibrationFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    dut::StartCalibrationParams_t params {};
    uint8_t status {};

    params.mask = m_mask;
    params.maskType = m_maskType;
    params.type = m_calibrationType;

    bool ok = dut->startCalibration(params, status);
    if (ok) {
        context.getConsole().cout("Start Calibration: status " + std::to_string(status) + "\n");
    }

    return ok;
}

bool StartCwFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<int16_t> amplitudeArg(
        "a",
        "amplitude",
        "Amplitude. Default value: " + std::to_string(static_cast<int16_t>(m_amplitude)),
        false,
        static_cast<int16_t>(m_amplitude),
        "Number",
        cmd);

    TCLAP::ValueArg<int16_t> toneArg(
        "t",
        "tone",
        "Tone number. Default value: " + std::to_string(m_tone),
        false,
        m_tone,
        "Number",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_amplitude = static_cast<int8_t>(amplitudeArg.getValue());
    m_tone = toneArg.getValue();

    return true;
}

bool StartCwFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->startCw(m_amplitude, m_tone);
}

bool StartTxFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint16_t> repetitionsArg(
        "r",
        "repetitions",
        "Number of repetitions. Default value: " + std::to_string(m_repetitions),
        false,
        m_repetitions,
        "Number",
        cmd);

    TCLAP::ValueArg<uint32_t> packetLengthArg(
        "l",
        "packet-length",
        "Packet length. Default value: " + std::to_string(m_packetLength),
        false,
        m_packetLength,
        "Number",
        cmd);

    TCLAP::SwitchArg longDataArg(
        "",
        "long-data",
        "Use long data.",
        cmd,
        m_longData);

    TCLAP::SwitchArg beamformingArg(
        "",
        "beamforming",
        "Use beamforming.",
        cmd,
        m_beamforming);

    TCLAP::ValueArg<uint32_t> codingTypeArg(
        "c",
        "coding-type",
        "Coding type. Valid values: 0=AUTO (auto-select based on PHY mode), 1=BCC, 2=LDPC. Default value: " + std::to_string(static_cast<uint32_t>(m_codingType)),
        false,
        static_cast<uint32_t>(m_codingType),
        "Coding type",
        cmd);

    auto validator = [&codingTypeArg]() {
        auto codingType = static_cast<dut::CodingType>(codingTypeArg.getValue());
        if (codingType != dut::CodingType::CODING_TYPE_AUTO && codingType != dut::CodingType::CODING_TYPE_BCC && codingType != dut::CodingType::CODING_TYPE_LDPC) {
            throw TCLAP::ArgException("Invalid value", codingTypeArg.longID());
        }
    };

    if (!parseCmdLine(args, cmd, validator)) {
        return false;
    }

    m_repetitions = repetitionsArg.getValue();
    m_packetLength = packetLengthArg.getValue();
    m_longData = longDataArg.getValue();
    m_beamforming = beamformingArg.getValue();
    m_codingType = static_cast<dut::CodingType>(codingTypeArg.getValue());

    return true;
}

bool StartTxFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->startTx(m_repetitions, m_packetLength, m_longData, m_beamforming, m_codingType);
}

bool StartRxPerFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint32_t> packetLimitArg(
        "p",
        "packet-limit",
        "Number of Rx Packets (PPDU) to receive. Default value: " + std::to_string(m_packetLimit),
        false,
        m_packetLimit,
        "Number",
        cmd);

    TCLAP::ValueArg<uint32_t> durationLimitArg(
        "d",
        "duration-limit",
        "Duration in milliseconds to listen for Rx Packets. Default value: " + std::to_string(m_durationLimit),
        false,
        m_durationLimit,
        "Number",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_packetLimit = packetLimitArg.getValue();
    m_durationLimit = durationLimitArg.getValue();

    return true;
}

bool StartRxPerFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->startRxPer(m_packetLimit);
}

bool StopCwFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->stopCw();
}

bool StopTxFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->stopTx();
}

bool StopRxPerFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::SwitchArg calcRxPerArg(
        "",
        "calc-per",
        "Calculate PER (FALSE if argument not set).",
        cmd,
        m_calcRxPer);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_calcRxPer = calcRxPerArg.getValue();

    return true;
}

bool StopRxPerFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->stopRxPer(m_calcRxPer);
}

bool ValidateBeamformingHeaderRegisterFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint32_t> expectedPhyModeArg(
        "p",
        "phy-mode",
        "Expected PHY mode. Valid values: " + validValuesForPhyModeString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_expectedPhyMode)),
        false,
        static_cast<uint32_t>(m_expectedPhyMode),
        "Expected PHY mode",
        cmd);

    TCLAP::ValueArg<uint32_t> expectedBandwidthArg(
        "b",
        "bandwidth",
        "Expected bandwidth. Valid values: " + validValuesForBandwidthString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_expectedBandwidth)),
        false,
        static_cast<uint32_t>(m_expectedBandwidth),
        "Expected bandwidth",
        cmd);

    if (!parseCmdLine(args, cmd)) {
        return false;
    }

    m_expectedPhyMode = static_cast<dut::PhyMode>(expectedPhyModeArg.getValue());
    m_expectedBandwidth = static_cast<dut::Bandwidth>(expectedBandwidthArg.getValue());

    return true;
}

bool ValidateBeamformingHeaderRegisterFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    bool valid = dut->validateBeamformingHeaderRegister(m_expectedPhyMode, m_expectedBandwidth);
    context.getConsole().cout(
        valid ? "Beamforming header register is valid.\n"
              : "Beamforming header register is NOT valid.\n");

    return valid;
}

bool WriteCalibrationFileFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint32_t> memoryTypeArg(
        "t",
        "memory-type",
        "Memory type. Valid values: " + validValuesForNvMemoryTypeString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_memoryType)),
        false,
        static_cast<uint32_t>(m_memoryType),
        "Memory type",
        cmd);

    TCLAP::ValueArg<uint32_t> memorySizeArg(
        "s",
        "memory-size",
        "Memory size. Valid values: " + validValuesForNvMemorySizeString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_memorySize)),
        false,
        static_cast<uint32_t>(m_memorySize),
        "Memory size",
        cmd);

    auto validator = [&memoryTypeArg, &memorySizeArg]() {
        if (validValuesForNvMemoryType.find(static_cast<dut::NvMemoryType>(memoryTypeArg.getValue())) == validValuesForNvMemoryType.end()) {
            throw TCLAP::ArgException("Invalid value", memoryTypeArg.longID());
        }

        if (validValuesForNvMemorySize.find(static_cast<dut::NvMemorySize>(memorySizeArg.getValue())) == validValuesForNvMemorySize.end()) {
            throw TCLAP::ArgException("Invalid value", memorySizeArg.longID());
        }
    };

    if (!parseCmdLine(args, cmd, validator)) {
        return false;
    }

    m_memoryType = static_cast<dut::NvMemoryType>(memoryTypeArg.getValue());
    m_memorySize = static_cast<dut::NvMemorySize>(memorySizeArg.getValue());

    return true;
}

bool WriteCalibrationFileFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->writeCalibrationFile(m_memoryType, m_memorySize);
}

bool WriteMemoryFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint32_t> moduleArg(
        "m",
        "chip-module",
        "Chip module. Valid values: " + validValuesForChipModuleString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_module)),
        false,
        static_cast<uint32_t>(m_module),
        "Chip module",
        cmd);

    TCLAP::ValueArg<std::string> addressArg(
        "a",
        "address",
        "Starting address. Default value: " + std::to_string(m_address),
        false,
        std::to_string(m_address),
        "Address",
        cmd);

    TCLAP::ValueArg<std::string> dataArg(
        "d",
        "data",
        "Hexadecimal string representing the array of bytes to write to memory.",
        true,
        "",
        "Data",
        cmd);

    auto validator = [this, &moduleArg, &dataArg]() {
        if (validValuesForChipModule.find(static_cast<dut::ChipModule>(moduleArg.getValue())) == validValuesForChipModule.end()) {
            throw TCLAP::ArgException("Invalid value", moduleArg.longID());
        }

        try {
            m_data = HexStringToBytes(dataArg.getValue());
        } catch (const std::exception& e) {
            throw TCLAP::ArgException("Invalid value (" + std::string(e.what()) + ")", dataArg.longID());
        }
    };

    if (!parseCmdLine(args, cmd, validator)) {
        return false;
    }

    m_module = static_cast<dut::ChipModule>(moduleArg.getValue());
    m_address = static_cast<size_t>(strtol(addressArg.getValue().c_str(), nullptr, 0));

    return true;
}

bool WriteMemoryFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->writeMemory(m_module, m_address, m_data.data(), m_data.size());
}

bool WriteNvmFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<std::string> addressArg(
        "a",
        "address",
        "Starting address. Default value: " + std::to_string(m_address),
        false,
        std::to_string(m_address),
        "Address",
        cmd);

    TCLAP::ValueArg<std::string> dataArg(
        "d",
        "data",
        "Hexadecimal string representing the array of bytes to write to memory.",
        true,
        "",
        "Data",
        cmd);

    auto validator = [this, &dataArg]() {
        try {
            m_data = HexStringToBytes(dataArg.getValue());
        } catch (const std::exception& e) {
            throw TCLAP::ArgException("Invalid value (" + std::string(e.what()) + ")", dataArg.longID());
        }
    };

    if (!parseCmdLine(args, cmd, validator)) {
        return false;
    }

    m_address = static_cast<size_t>(strtol(addressArg.getValue().c_str(), nullptr, 0));

    return true;
}

bool WriteNvmFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->writeNvm(m_address, m_data.data(), m_data.size());
}

bool WriteRegisterFunction::parse(std::vector<std::string>& args)
{
    TCLAP::CmdLine cmd(args.at(0), ' ');

    TCLAP::ValueArg<uint32_t> moduleArg(
        "m",
        "chip-module",
        "Chip module. Valid values: " + validValuesForChipModuleString + ". Default value: " + std::to_string(static_cast<uint32_t>(m_module)),
        false,
        static_cast<uint32_t>(m_module),
        "Chip module",
        cmd);

    TCLAP::ValueArg<std::string> addressArg(
        "a",
        "address",
        "Starting address. Default value: " + std::to_string(m_address),
        false,
        std::to_string(m_address),
        "Address",
        cmd);

    TCLAP::ValueArg<std::string> maskArg(
        "k",
        "mask",
        "Mask. Default value: " + dut::toHexString(m_mask),
        false,
        dut::toHexString(m_mask),
        "Mask",
        cmd);

    TCLAP::ValueArg<std::string> valueArg(
        "v",
        "value",
        "Value. Default value: " + dut::toHexString(m_value),
        false,
        dut::toHexString(m_value),
        "Value",
        cmd);

    auto validator = [&moduleArg]() {
        if (validValuesForChipModule.find(static_cast<dut::ChipModule>(moduleArg.getValue())) == validValuesForChipModule.end()) {
            throw TCLAP::ArgException("Invalid value", moduleArg.longID());
        }
    };

    if (!parseCmdLine(args, cmd, validator)) {
        return false;
    }

    m_module = static_cast<dut::ChipModule>(moduleArg.getValue());
    m_address = static_cast<size_t>(strtol(addressArg.getValue().c_str(), nullptr, 0));
    m_mask = static_cast<uint32_t>(strtol(maskArg.getValue().c_str(), nullptr, 0));
    m_value = static_cast<uint32_t>(strtol(valueArg.getValue().c_str(), nullptr, 0));

    return true;
}

bool WriteRegisterFunction::execute(std::shared_ptr<dut::Dut> dut, Context& context)
{
    return dut->writeRegister(m_module, m_address, m_mask, m_value);
}

}
