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

#include "Dut.h"
#include "dut/DutImpl.h"

#include "marshal_dut.h"

using namespace System::Runtime::InteropServices;

namespace dut_cli {

Dut::Dut(uint8_t wlanIndex, Connection ^ connection, Logger ^ logger)
    : ManagedObject(std::make_shared<dut::DutImpl>(wlanIndex, connection->GetInstance(), logger->GetInstance()))
{
}

System::String ^ Dut::getVersion()
{
    auto version = dut::Dut::getVersion();
    auto _version = msclr::interop::marshal_as<System::String ^>(version);

    return _version;
}

System::String ^ Dut::getBuildTag()
{
    auto buildTag = dut::Dut::getBuildTag();
    auto _buildTag = msclr::interop::marshal_as<System::String ^>(buildTag);

    return _buildTag;
}

System::String ^ Dut::getLastError()
{
    auto lastError = GetInstance()->getLastError();
    auto _lastError = msclr::interop::marshal_as<System::String ^>(lastError);

    return _lastError;
}

bool Dut::driverInit(bool snifferMode, NvMemoryType memoryType, NvMemorySize memorySize, Band band)
{
    auto _memoryType = static_cast<dut::NvMemoryType>(memoryType);
    auto _memorySize = static_cast<dut::NvMemorySize>(memorySize);
    auto _band = static_cast<dut::Band>(band);

    return GetInstance()->driverInit(snifferMode, _memoryType, _memorySize, _band);
}

bool Dut::driverRelease()
{
    return GetInstance()->driverRelease();
}

bool Dut::calculateMaxPacketLength(PhyMode phyMode, [Out] uint32_t % maxPacketLength)
{
    auto _phyMode = static_cast<dut::PhyMode>(phyMode);
    uint32_t _maxPacketLength;

    bool result = GetInstance()->calculateMaxPacketLength(_phyMode, _maxPacketLength);
    if (result) {
        maxPacketLength = _maxPacketLength;
    }

    return result;
}

bool Dut::calculatePacketLength(PhyMode phyMode, Bandwidth signalBandwidth, Mcs mcs, uint8_t nss, uint32_t numSymbols, [Out] uint32_t % packetLength)
{
    auto _phyMode = static_cast<dut::PhyMode>(phyMode);
    auto _signalBandwidth = static_cast<dut::Bandwidth>(signalBandwidth);
    auto _mcs = static_cast<dut::Mcs>(mcs);
    uint32_t _packetLength;

    bool result = GetInstance()->calculatePacketLength(_phyMode, _signalBandwidth, _mcs, nss, numSymbols, _packetLength);
    if (result) {
        packetLength = _packetLength;
    }

    return result;
}

bool Dut::calculatePhyDataRate(PhyMode phyMode, Bandwidth signalBandwidth, Mcs mcs, uint8_t nss, Gi gi, [Out] float % rateMbps)
{
    auto _phyMode = static_cast<dut::PhyMode>(phyMode);
    auto _signalBandwidth = static_cast<dut::Bandwidth>(signalBandwidth);
    auto _mcs = static_cast<dut::Mcs>(mcs);
    auto _gi = static_cast<dut::Gi>(gi);
    float _rateMbps;

    bool result = GetInstance()->calculatePhyDataRate(_phyMode, _signalBandwidth, _mcs, nss, _gi, _rateMbps);
    if (result) {
        rateMbps = _rateMbps;
    }

    return result;
}

bool Dut::flushNvm()
{
    return GetInstance()->flushNvm();
}

bool Dut::getAvailableRxAntennaMask([Out] AntennaMask ^ % antennaMask)
{
    dut::AntennaMask _antennaMask(0);

    bool result = GetInstance()->getAvailableRxAntennaMask(_antennaMask);
    if (result) {
        antennaMask = gcnew AntennaMask(_antennaMask);
    }

    return result;
}

bool Dut::getAvailableTxAntennaMask([Out] AntennaMask ^ % antennaMask)
{
    dut::AntennaMask _antennaMask(0);

    bool result = GetInstance()->getAvailableTxAntennaMask(_antennaMask);
    if (result) {
        antennaMask = gcnew AntennaMask(_antennaMask);
    }

    return result;
}

bool Dut::getBand([Out] Band % band)
{
    dut::Band _band;

    bool result = GetInstance()->getBand(_band);
    if (result) {
        band = static_cast<Band>(_band);
    }

    return result;
}

bool Dut::getSupportedBands([Out] cli::array<Band> ^ % supportedBands)
{
    std::vector<dut::Band> _supportedBands;

    bool result = GetInstance()->getSupportedBands(_supportedBands);
    if (result) {
        supportedBands = gcnew cli::array<Band>(static_cast<int>(_supportedBands.size()));
        for (int i = 0; i < _supportedBands.size(); i++) {
            supportedBands[i] = static_cast<Band>(_supportedBands.at(i));
        }
    }

    return result;
}

bool Dut::getBbicCddValues(uint8_t numTxAntennas, [Out] uint32_t % offset1, [Out] uint32_t % offset2, [Out] uint32_t % offset3)
{
    uint32_t _offset1;
    uint32_t _offset2;
    uint32_t _offset3;

    bool result = GetInstance()->getBbicCddValues(numTxAntennas, _offset1, _offset2, _offset3);
    if (result) {
        offset1 = _offset1;
        offset2 = _offset2;
        offset3 = _offset3;
    }

    return result;
}

bool Dut::getCalibrationFileVersion([Out] CalibrationFileVersion % version, [Out] CalibrationFileSubversion % subversion)
{
    dut::CalibrationFileVersion _version;
    dut::CalibrationFileSubversion _subversion;

    bool result = GetInstance()->getCalibrationFileVersion(_version, _subversion);
    if (result) {
        version = static_cast<CalibrationFileVersion>(_version);
        subversion = static_cast<CalibrationFileSubversion>(_subversion);
    }

    return result;
}

bool Dut::getCardInfo([Out] uint8_t % countryCode, [Out] cli::array<uint8_t> ^ % macAddress, [Out] cli::array<uint8_t> ^ % serialNumber, [Out] uint8_t % week, [Out] uint8_t % year)
{
    uint8_t _countryCode;
    std::array<uint8_t, dut::cardInfoMacAddressSize> _macAddress;
    std::array<uint8_t, dut::cardInfoSerialNumberSize> _serialNumber;
    uint8_t _week;
    uint8_t _year;

    bool result = GetInstance()->getCardInfo(_countryCode, _macAddress, _serialNumber, _week, _year);
    if (result) {
        countryCode = _countryCode;
        macAddress = marshal_as_array<uint8_t, dut::cardInfoMacAddressSize>(_macAddress);
        serialNumber = marshal_as_array<uint8_t, dut::cardInfoSerialNumberSize>(_serialNumber);
        week = _week;
        year = _year;
    }

    return result;
}

bool Dut::getChipId([Out] uint16_t % chipId)
{
    dut::ChipID _chipId;

    bool result = GetInstance()->getChipId(_chipId);
    if (result) {
        chipId = static_cast<uint16_t>(_chipId);
    }

    return result;
}

bool Dut::getComponentVersion(VersionedComponent component, [Out] System::String ^ % version)
{
    auto _component = static_cast<dut::VersionedComponent>(component);
    std::string _version;

    bool result = GetInstance()->getComponentVersion(_component, _version);
    if (result) {
        version = msclr::interop::marshal_as<System::String ^>(_version);
    }

    return result;
}

bool Dut::getEnabledRxAntennaMask([Out] AntennaMask ^ % antennaMask)
{
    dut::AntennaMask _antennaMask(0);

    bool result = GetInstance()->getEnabledRxAntennaMask(_antennaMask);
    if (result) {
        antennaMask = gcnew AntennaMask(_antennaMask);
    }

    return result;
}

bool Dut::getEnabledTxAntennaMask([Out] AntennaMask ^ % antennaMask)
{
    dut::AntennaMask _antennaMask(0);

    bool result = GetInstance()->getEnabledTxAntennaMask(_antennaMask);
    if (result) {
        antennaMask = gcnew AntennaMask(_antennaMask);
    }

    return result;
}

bool Dut::getFemType([Out] FemType % femType)
{
    dut::FemType _femType;

    bool result = GetInstance()->getFemType(_femType);
    if (result) {
        femType = static_cast<FemType>(_femType);
    }

    return result;
}

bool Dut::getHardwareType([Out] HardwareType % hardwareType)
{
    dut::HardwareType _hwType;

    bool result = GetInstance()->getHardwareType(_hwType);
    if (result) {
        hardwareType = static_cast<HardwareType>(_hwType);
    }

    return result;
}

bool Dut::getInbandRssi([Out] cli::array<int16_t> ^ % rssi)
{
    std::array<int16_t, dut::maxNumRxAntennas> _rssi;

    bool result = GetInstance()->getInbandRssi(_rssi);
    if (result) {
        rssi = marshal_as_array<int16_t, dut::maxNumRxAntennas>(_rssi);
    }

    return result;
}

bool Dut::getMacPacketCounters([Out] uint32_t % receivedPackets)
{
    uint32_t _receivedPackets;

    bool result = GetInstance()->getMacPacketCounters(_receivedPackets);
    if (result) {
        receivedPackets = _receivedPackets;
    }

    return result;
}

bool Dut::getMpduPacketCounters([Out] uint32_t % receivedPackets, [Out] uint32_t % errorPackets)
{
    uint32_t _receivedPackets;
    uint32_t _errorPackets;

    bool result = GetInstance()->getMpduPacketCounters(_receivedPackets, _errorPackets);
    if (result) {
        receivedPackets = _receivedPackets;
        errorPackets = _errorPackets;
    }

    return result;
}

bool Dut::getNvmSize([Out] size_t % size)
{
    size_t _size;

    bool result = GetInstance()->getNvmSize(_size);
    if (result) {
        size = _size;
    }

    return result;
}

bool Dut::getNvmType([Out] NvMemoryType % type)
{
    dut::NvMemoryType _type;

    bool result = GetInstance()->getNvmType(_type);
    if (result) {
        type = static_cast<NvMemoryType>(_type);
    }

    return result;
}

bool Dut::getNvmVersion([Out] uint8_t % version)
{
    uint8_t _version;

    bool result = GetInstance()->getNvmVersion(_version);
    if (result) {
        version = _version;
    }

    return result;
}

bool Dut::getPhyMode([Out] PhyMode % phyMode)
{
    dut::PhyMode _phyMode;

    bool result = GetInstance()->getPhyMode(_phyMode);
    if (result) {
        phyMode = static_cast<PhyMode>(_phyMode);
    }

    return result;
}

bool Dut::getPhyPacketCounters([Out] uint32_t % receivedPackets, [Out] uint32_t % crcErrors, [Out] uint32_t % forwardedPackets)
{
    uint32_t _receivedPackets;
    uint32_t _crcErrors;
    uint32_t _forwardedPackets;

    bool result = GetInstance()->getPhyPacketCounters(_receivedPackets, _crcErrors, _forwardedPackets);
    if (result) {
        receivedPackets = _receivedPackets;
        crcErrors = _crcErrors;
        forwardedPackets = _forwardedPackets;
    }

    return result;
}

bool Dut::getProductionFlag([Out] bool % productionFlag)
{
    bool _productionFlag;

    bool result = GetInstance()->getProductionFlag(_productionFlag);
    if (result) {
        productionFlag = _productionFlag;
    }

    return result;
}

bool Dut::getRssiCalibrationData([Out] CalibrationFileVersion % version, [Out] cli::array<RssiCalibrationData ^> ^ % data)
{
    dut::CalibrationFileVersion _version;
    std::vector<dut::RssiCalibrationData_t> _data;

    bool result = GetInstance()->getRssiCalibrationData(_version, _data);
    if (result) {
        version = static_cast<CalibrationFileVersion>(_version);

        if (version == CalibrationFileVersion::CALIBRATION_FILE_VERSION_6) {
            data = msclr::interop::marshal_as<cli::array<RssiCalibrationDataVer6 ^> ^>(_data);
        } else if (version == CalibrationFileVersion::CALIBRATION_FILE_VERSION_7) {
            data = msclr::interop::marshal_as<cli::array<RssiCalibrationDataVer7 ^> ^>(_data);
        } else {
            result = false;
        }
    }

    return result;
}

bool Dut::getRxEvm([Out] cli::array<uint8_t> ^ % rxEvm)
{
    std::array<uint8_t, dut::maxNumRxAntennas> _rxEvm;

    bool result = GetInstance()->getRxEvm(_rxEvm);
    if (result) {
        rxEvm = marshal_as_array<uint8_t, dut::maxNumRxAntennas>(_rxEvm);
    }

    return result;
}

bool Dut::getRxRateInfo([Out] uint8_t % mcs, [Out] uint8_t % nss)
{
    uint8_t _mcs;
    uint8_t _nss;

    bool result = GetInstance()->getRxRateInfo(_mcs, _nss);
    if (result) {
        mcs = _mcs;
        nss = _nss;
    }

    return result;
}

bool Dut::getTemperature([Out] float % temperature)
{
    float _temperature;

    bool result = GetInstance()->getTemperature(_temperature);
    if (result) {
        temperature = _temperature;
    }

    return result;
}

bool Dut::getTransmitVoltages([Out] cli::array<uint32_t> ^ % voltages)
{
    std::array<uint32_t, dut::maxNumTxAntennas> _voltages;

    bool result = GetInstance()->getTransmitVoltages(_voltages);
    if (result) {
        voltages = marshal_as_array<uint32_t, dut::maxNumTxAntennas>(_voltages);
    }

    return result;
}

bool Dut::getTssiCalibrationData([Out] CalibrationFileVersion % version, [Out] cli::array<TssiCalibrationData ^> ^ % data)
{
    dut::CalibrationFileVersion _version;
    std::vector<dut::TssiCalibrationData_t> _data;

    bool result = GetInstance()->getTssiCalibrationData(_version, _data);
    if (result) {
        version = static_cast<CalibrationFileVersion>(_version);

        if (version == CalibrationFileVersion::CALIBRATION_FILE_VERSION_6) {
            data = msclr::interop::marshal_as<cli::array<TssiCalibrationDataVer6 ^> ^>(_data);
        } else if (version == CalibrationFileVersion::CALIBRATION_FILE_VERSION_7) {
            data = msclr::interop::marshal_as<cli::array<TssiCalibrationDataVer7 ^> ^>(_data);
        } else {
            result = false;
        }
    }

    return result;
}

bool Dut::getXtalCalValue([Out] uint16_t % xtalValue)
{
    uint16_t _xtalValue;

    bool result = GetInstance()->getXtalCalValue(_xtalValue);
    if (result) {
        xtalValue = _xtalValue;
    }

    return result;
}

bool Dut::getXtalRegValue([Out] uint16_t % xtalValue)
{
    uint16_t _xtalValue;

    bool result = GetInstance()->getXtalRegValue(_xtalValue);
    if (result) {
        xtalValue = _xtalValue;
    }

    return result;
}

bool Dut::getZwdfsStatus([Out] AntennaMask ^ % antennaMask, [Out] bool % enabled)
{
    dut::AntennaMask _antennaMask(0);
    bool _enabled;

    bool result = GetInstance()->getZwdfsStatus(_antennaMask, _enabled);
    if (result) {
        antennaMask = gcnew AntennaMask(_antennaMask);
        enabled = _enabled;
    }

    return result;
}

bool Dut::loadBeamformingMatrixFromFileSet(System::String ^ primaryHeaderFile, System::String ^ primaryValuesFile, System::String ^ primaryExtValuesEhtFile, System::String ^ secondaryHeaderFile, System::String ^ secondaryValuesFile, System::String ^ secondaryExtValuesEhtFile)
{
    if (!primaryHeaderFile || !primaryValuesFile) {
        return false;
    }

    // Convert managed strings to native strings
    std::string _primaryHeaderFile = msclr::interop::marshal_as<std::string>(primaryHeaderFile);
    std::string _primaryValuesFile = msclr::interop::marshal_as<std::string>(primaryValuesFile);
    std::string _primaryExtValuesEhtFile = primaryExtValuesEhtFile ? msclr::interop::marshal_as<std::string>(primaryExtValuesEhtFile) : "";

    // Create primary file set
    dut::BeamformingFilePathSet_t primaryFileSet;
    primaryFileSet.headerFile = _primaryHeaderFile.c_str();
    primaryFileSet.valuesFile = _primaryValuesFile.c_str();
    primaryFileSet.extValuesEhtFile = _primaryExtValuesEhtFile.empty() ? nullptr : _primaryExtValuesEhtFile.c_str();

    // Create secondary file set
    dut::BeamformingFilePathSet_t secondaryFileSet {};
    if (secondaryHeaderFile && secondaryValuesFile) {
        std::string _secondaryHeaderFile = msclr::interop::marshal_as<std::string>(secondaryHeaderFile);
        std::string _secondaryValuesFile = msclr::interop::marshal_as<std::string>(secondaryValuesFile);
        std::string _secondaryExtValuesEhtFile = secondaryExtValuesEhtFile ? msclr::interop::marshal_as<std::string>(secondaryExtValuesEhtFile) : "";

        secondaryFileSet.headerFile = _secondaryHeaderFile.c_str();
        secondaryFileSet.valuesFile = _secondaryValuesFile.c_str();
        secondaryFileSet.extValuesEhtFile = _secondaryExtValuesEhtFile.empty() ? nullptr : _secondaryExtValuesEhtFile.c_str();
    } else {
        secondaryFileSet.headerFile = nullptr;
        secondaryFileSet.valuesFile = nullptr;
        secondaryFileSet.extValuesEhtFile = nullptr;
    }

    return GetInstance()->loadBeamformingMatrixFromFileSet(primaryFileSet, secondaryFileSet);
}

bool Dut::loadNvmFromFile(System::String ^ fileName)
{
    auto _fileName = msclr::interop::marshal_as<std::string>(fileName);

    return GetInstance()->loadNvmFromFile(_fileName);
}

bool Dut::readMemory(ChipModule chipModule, size_t address, [Out] cli::array<uint8_t> ^ % data, size_t length)
{
    auto _module = static_cast<dut::ChipModule>(chipModule);
    auto _data = create_shared_ptr_to_array<uint8_t>(length);

    bool result = GetInstance()->readMemory(_module, address, _data.get(), length);
    if (result) {
        data = marshal_as_array<uint8_t>(_data.get(), length);
    }

    return result;
}

bool Dut::readNvm(size_t address, [Out] cli::array<uint8_t> ^ % data, size_t length, bool useCache)
{
    auto _data = create_shared_ptr_to_array<uint8_t>(length);

    bool result = GetInstance()->readNvm(address, _data.get(), length);
    if (result) {
        data = marshal_as_array<uint8_t>(_data.get(), length);
    }

    return result;
}

bool Dut::readRegister(ChipModule chipModule, size_t address, uint32_t mask, [Out] uint32_t % value)
{
    auto _module = static_cast<dut::ChipModule>(chipModule);
    uint32_t _value;

    bool result = GetInstance()->readRegister(_module, address, mask, _value);
    if (result) {
        value = _value;
    }

    return result;
}

bool Dut::resetMacPacketCounters()
{
    return GetInstance()->resetMacPacketCounters();
}

bool Dut::resetMpduPacketCounters()
{
    return GetInstance()->resetMpduPacketCounters();
}

bool Dut::resetPhyPacketCounters()
{
    return GetInstance()->resetPhyPacketCounters();
}

bool Dut::rxMeasure(uint32_t numCaptures, uint32_t captureInterval, bool disabled)
{
    return GetInstance()->rxMeasure(numCaptures, captureInterval, disabled);
}

bool Dut::saveNvmToFile(System::String ^ fileName)
{
    auto _fileName = msclr::interop::marshal_as<std::string>(fileName);

    return GetInstance()->saveNvmToFile(_fileName);
}

bool Dut::setBbicCddValues(uint8_t numTxAntennas, uint32_t offset1, uint32_t offset2, uint32_t offset3)
{
    return GetInstance()->setBbicCddValues(numTxAntennas, offset1, offset2, offset3);
}

bool Dut::setCalibrationFileVersion(CalibrationFileVersion version, CalibrationFileSubversion subversion)
{
    dut::CalibrationFileVersion _version = static_cast<dut::CalibrationFileVersion>(version);
    dut::CalibrationFileSubversion _subversion = static_cast<dut::CalibrationFileSubversion>(subversion);

    return GetInstance()->setCalibrationFileVersion(_version, _subversion);
}

bool Dut::setCardInfo(uint8_t countryCode, cli::array<uint8_t> ^ macAddress, cli::array<uint8_t> ^ serialNumber, uint8_t week, uint8_t year)
{
    std::array<uint8_t, dut::cardInfoMacAddressSize> _macAddress {};
    dut_cli::copy_array<uint8_t, dut::cardInfoMacAddressSize>(macAddress, _macAddress);
    std::array<uint8_t, dut::cardInfoSerialNumberSize> _serialNumber {};
    dut_cli::copy_array<uint8_t, dut::cardInfoSerialNumberSize>(serialNumber, _serialNumber);

    return GetInstance()->setCardInfo(countryCode, _macAddress, _serialNumber, week, year);
}

bool Dut::setChannel(PhyMode phyMode, Bandwidth spectrumBandwidth, uint8_t lowestChannel, uint8_t primaryChannelIndex, RegulationType regulationType)
{
    auto _phyMode = static_cast<dut::PhyMode>(phyMode);
    auto _spectrumBandwidth = static_cast<dut::Bandwidth>(spectrumBandwidth);
    auto _regulationType = static_cast<dut::RegulationType>(regulationType);

    return GetInstance()->setChannel(_phyMode, _spectrumBandwidth, lowestChannel, primaryChannelIndex, _regulationType);
}

bool Dut::setClipper(bool enabled)
{
    return GetInstance()->setClipper(enabled);
}

bool Dut::setEnabledRxAntennaMask(AntennaMask ^ antennaMask)
{
    return GetInstance()->setEnabledRxAntennaMask(antennaMask->Value);
}

bool Dut::setEnabledTxAntennaMask(AntennaMask ^ antennaMask)
{
    return GetInstance()->setEnabledTxAntennaMask(antennaMask->Value);
}

bool Dut::setIfs(uint32_t ifs)
{
    return GetInstance()->setIfs(ifs);
}

bool Dut::setProductionFlag(bool productionFlag, bool writeToNvm)
{
    return GetInstance()->setProductionFlag(productionFlag, writeToNvm);
}

bool Dut::setRate(Bandwidth signalBandwidth, Mcs mcs, uint8_t nss, Gi gi, Ltf ltf, [Out] float % rateMbps)
{
    auto _signalBandwidth = static_cast<dut::Bandwidth>(signalBandwidth);
    auto _mcs = static_cast<dut::Mcs>(mcs);
    auto _gi = static_cast<dut::Gi>(gi);
    auto _ltf = static_cast<dut::Ltf>(ltf);
    float _rateMbps;

    bool result = GetInstance()->setRate(_signalBandwidth, _mcs, nss, _gi, _ltf, _rateMbps);
    if (result) {
        rateMbps = _rateMbps;
    }

    return result;
}

bool Dut::setRuParams(uint32_t userOne, uint32_t userTwo)
{
    return GetInstance()->setRuParams(userOne, userTwo);
}

bool Dut::setRxAggregationEnabled(bool enabled)
{
    return GetInstance()->setRxAggregationEnabled(enabled);
}

bool Dut::setSpacelessTxEnabled(bool enabled)
{
    return GetInstance()->setSpacelessTxEnabled(enabled);
}

bool Dut::setTransmitPowerControl(bool closedLoop, uint8_t powerLimit)
{
    return GetInstance()->setTransmitPowerControl(closedLoop, powerLimit);
}

bool Dut::setTransmitPowerLevel(uint8_t powerLevel)
{
    return GetInstance()->setTransmitPowerLevel(powerLevel);
}

bool Dut::setXtalCalValue(uint16_t xtalValue)
{
    return GetInstance()->setXtalCalValue(xtalValue);
}

bool Dut::setXtalRegValue(uint16_t xtalValue)
{
    return GetInstance()->setXtalRegValue(xtalValue);
}

bool Dut::setZwdfsConfiguration(uint8_t zwdfsChannel, Bandwidth zwdfsBandwidth, Bandwidth radarDetectionBandwidth)
{
    auto _zwdfsBandwidth = static_cast<dut::Bandwidth>(zwdfsBandwidth);
    auto _radarDetectionBandwidth = static_cast<dut::Bandwidth>(radarDetectionBandwidth);

    return GetInstance()->setZwdfsConfiguration(zwdfsChannel, _zwdfsBandwidth, _radarDetectionBandwidth);
}

bool Dut::startCw(int8_t amplitude, int16_t tone)
{
    return GetInstance()->startCw(amplitude, tone);
}

bool Dut::startTx(uint16_t repetitions, uint32_t packetLength, bool longData, bool beamforming, CodingType codingType)
{
    auto _codingType = static_cast<dut::CodingType>(codingType);

    return GetInstance()->startTx(repetitions, packetLength, longData, beamforming, _codingType);
}

bool Dut::startRxPer(uint32_t packetLimit)
{
    return GetInstance()->startRxPer(packetLimit);
}

bool Dut::stopCw()
{
    return GetInstance()->stopCw();
}

bool Dut::stopTx()
{
    return GetInstance()->stopTx();
}

bool Dut::stopRxPer(bool calcRxPer)
{
    return GetInstance()->stopRxPer(calcRxPer);
}

bool Dut::validateBeamformingHeaderRegister(PhyMode expectedPhyMode, Bandwidth expectedBandwidth)
{
    auto _expectedPhyMode = static_cast<dut::PhyMode>(expectedPhyMode);
    auto _expectedBandwidth = static_cast<dut::Bandwidth>(expectedBandwidth);

    return GetInstance()->validateBeamformingHeaderRegister(_expectedPhyMode, _expectedBandwidth);
}

bool Dut::writeCalibrationFile(NvMemoryType memoryType, NvMemorySize memorySize)
{
    auto _type = static_cast<dut::NvMemoryType>(memoryType);
    auto _size = static_cast<dut::NvMemorySize>(memorySize);
    return GetInstance()->writeCalibrationFile(_type, _size);
}

bool Dut::writeMemory(ChipModule chipModule, size_t address, cli::array<uint8_t> ^ data, size_t length)
{
    auto _module = static_cast<dut::ChipModule>(chipModule);
    auto _data = marshal_as_shared_ptr_to_array<uint8_t>(data);

    return GetInstance()->writeMemory(_module, address, _data.get(), length);
}

bool Dut::writeNvm(size_t address, cli::array<uint8_t> ^ data, size_t length)
{
    auto _data = marshal_as_shared_ptr_to_array<uint8_t>(data);

    return GetInstance()->writeNvm(address, _data.get(), length);
}

bool Dut::writeRegister(ChipModule chipModule, size_t address, uint32_t mask, uint32_t value)
{
    auto _module = static_cast<dut::ChipModule>(chipModule);

    return GetInstance()->writeRegister(_module, address, mask, value);
}

}
