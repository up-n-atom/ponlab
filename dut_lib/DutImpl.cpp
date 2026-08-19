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

#include "dut/DutImpl.h"
#include "SharedHeaders.h" // For hardware register bit masks and constants

#include "BeamformingUtils.h"
#include "CalibrationFile.h"
#include "CalibrationFileFactory.h"
#include "CalibrationFileVer6.h"
#include "CombinedVersion.h"
#include "DeviceNvm.h"
#include "Exceptions.h"
#include "Rates.h"
#include "ResizableBuffer.h"
#include "SharedHeaders.h"
#include "Status.h"
#include "Transmitter.h"
#include "dut/Channels.h"
#include "dut/ClientImpl.h"
#include "dut/Tools.h"

#include "RssiCalibrationDataVer6.h"
#include "RssiCalibrationDataVer7.h"
#include "TssiCalibrationDataVer6.h"
#include "TssiCalibrationDataVer7.h"

#include <cstring>
#include <fstream>
#include <stdexcept>

#ifdef LINUX_HOST
#include <algorithm>
#include <iostream>
#endif

namespace dut {

static void checkBandwidth(Bandwidth bandwidth)
{
    if ((bandwidth < Bandwidth::BANDWIDTH_TWENTY) || (bandwidth > Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY)) {
        throw std::invalid_argument("Invalid bandwidth (" + toString(bandwidth) + ")");
    }
}

static void checkLowestChannel(uint8_t lowestChannel, PhyMode phyMode, Band band, Bandwidth bandwidth)
{
#ifdef ALLOW_ILLEGAL_CHANNELS
    (void)lowestChannel;
    (void)phyMode;
    (void)band;
    (void)bandwidth;
#else
    auto channels = Channels::getChannels(phyMode, band, bandwidth);

    if (std::count(channels.begin(), channels.end(), lowestChannel) == 0) {
        throw std::invalid_argument("Invalid channel (" + toString(lowestChannel) + ")");
    }
#endif
}

static void checkZwdfsChannel(uint8_t channel, Bandwidth bandwidth)
{
    // Currently 160MHz only
    if (bandwidth != Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY) {
        throw std::invalid_argument("Invalid ZWDFS bandwidth (" + toString(bandwidth) + ")");
    }

    // This method is called when in 5GHz only
    try {
        checkLowestChannel(channel, PhyMode::PHY_MODE_AX, Band::BAND_5000MHZ, bandwidth);
    } catch (const std::invalid_argument&) {
        throw std::invalid_argument("Invalid ZWDFS channel (" + toString(channel) + ")");
    }
}

static void checkZwdfsRadarDetectionBandwidth(Bandwidth bandwidth)
{
    const std::vector<Bandwidth> validRadarDetectionBandwidths { Bandwidth::BANDWIDTH_EIGHTY, Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY, Bandwidth::BANDWIDTH_INVALID };

    if (std::count(validRadarDetectionBandwidths.begin(), validRadarDetectionBandwidths.end(), bandwidth) == 0) {
        throw std::invalid_argument("Invalid ZWDFS radar detection bandwidth (" + toString(bandwidth) + ")");
    }
}

static void checkPrimaryChannelIndex(Bandwidth bandwidth, uint8_t primaryChannelIndex)
{
    bool invalidPrimaryChannelIndex = false;

    if ((bandwidth == Bandwidth::BANDWIDTH_TWENTY) && (primaryChannelIndex > 0)) {
        invalidPrimaryChannelIndex = true;
    } else if ((bandwidth == Bandwidth::BANDWIDTH_FOURTY) && (primaryChannelIndex > 1)) {
        invalidPrimaryChannelIndex = true;
    } else if ((bandwidth == Bandwidth::BANDWIDTH_EIGHTY) && (primaryChannelIndex > 3)) {
        invalidPrimaryChannelIndex = true;
    } else if ((bandwidth == Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY) && (primaryChannelIndex > 7)) {
        invalidPrimaryChannelIndex = true;
    } else if ((bandwidth == Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY) && (primaryChannelIndex > 15)) {
        invalidPrimaryChannelIndex = true;
    }

    if (invalidPrimaryChannelIndex) {
        throw std::invalid_argument("Invalid primary channel index (" + toString(primaryChannelIndex) + ") for " + toString(bandwidth) + " bandwidth");
    }
}

static void checkNumberOfSpatialStreams(PhyMode phyMode, uint8_t nss)
{
    if ((nss == 0) || (nss > maxNumTxAntennas) || ((nss > 1) && (phyMode <= PhyMode::PHY_MODE_G))) {
        throw std::invalid_argument("Invalid number of spatial streams (" + toString(nss) + ")");
    }
}

static void checkGuardInterval(PhyMode phyMode, Gi gi)
{
    bool invalid = false;

    if ((phyMode == PhyMode::PHY_MODE_AX) || (phyMode == PhyMode::PHY_MODE_BE)) {
        if ((gi != Gi::GI_0_8_US) && (gi != Gi::GI_1_6_US) && (gi != Gi::GI_3_2_US)) {
            invalid = true;
        }
    } else if ((gi != Gi::GI_0_4_US) && (gi != Gi::GI_0_8_US)) {
        invalid = true;
    }

    if (invalid) {
        throw std::invalid_argument("Invalid combination of PHY mode (" + toString(phyMode) + ") and guard interval (" + toString(gi) + ")");
    }
}

static uint8_t getMaskShift(uint32_t mask)
{
    uint32_t tempMask = mask;
    uint8_t maskShift = 0;

    // find the first '1' in the mask
    while ((tempMask != 0) && ((tempMask & 0x1) == 0)) {
        tempMask = tempMask >> 1;
        maskShift++;
    }

    // make sure that the mask is valid (no "holes")
    while ((tempMask & 0x1) == 1) {
        tempMask = tempMask >> 1;
    }

    if (tempMask != 0) {
        throw std::invalid_argument("Invalid mask (" + toHexString(mask) + ")");
    }

    return maskShift;
}

static uint8_t bandToBitmask(Band band)
{
    return static_cast<uint8_t>(1 << static_cast<uint8_t>(band));
}

static bool isBandSupported(Band band, uint8_t supportedBandsMask)
{
    return bandToBitmask(band) & supportedBandsMask;
}

static std::vector<Band> getSupportedBands(uint8_t supportedBandsMask)
{
    std::vector<Band> supportedBands;

    for (auto band : { Band::BAND_2400MHZ, Band::BAND_5000MHZ, Band::BAND_6000MHZ }) {
        if (isBandSupported(band, supportedBandsMask)) {
            supportedBands.push_back(band);
        }
    }

    return supportedBands;
}

static HardwareType getHardwareType(ChipID chipId)
{
    switch (chipId) {
    case ChipID::CHIP_ID_GEN4_A:
    case ChipID::CHIP_ID_GEN4_B:
    case ChipID::CHIP_ID_GEN4_C:
        return HardwareType::HARDWARE_TYPE_GEN4;
    case ChipID::CHIP_ID_GEN5_A:
    case ChipID::CHIP_ID_GEN5_B:
        return HardwareType::HARDWARE_TYPE_GEN5;
    case ChipID::CHIP_ID_GEN6:
    case ChipID::CHIP_ID_GEN6_B:
    case ChipID::CHIP_ID_GEN6_D2:
    case ChipID::CHIP_ID_GEN6_D2B:
        return HardwareType::HARDWARE_TYPE_GEN6;
    case ChipID::CHIP_ID_GEN7:
    case ChipID::CHIP_ID_GEN7B:
        return HardwareType::HARDWARE_TYPE_GEN7;
    default:
        return HardwareType::HARDWARE_TYPE_INVALID;
    }
}

static CalibrationType getCalibrationType(HardwareType hardwareType)
{
    switch (hardwareType) {
    case HardwareType::HARDWARE_TYPE_GEN4:
        return CalibrationType::TSSI_CAL_TYPE_VER4;
    case HardwareType::HARDWARE_TYPE_GEN5:
        return CalibrationType::TSSI_CAL_TYPE_VER5;
    case HardwareType::HARDWARE_TYPE_GEN6:
    case HardwareType::HARDWARE_TYPE_GEN7:
        return CalibrationType::TSSI_CAL_TYPE_VER6;
    default:
        throw std::invalid_argument("Invalid hardware type (" + toString(static_cast<int32_t>(hardwareType)) + ")");
    }
}

DutImpl::DutImpl(uint8_t wlanIndex, std::shared_ptr<Connection> connection, std::shared_ptr<Logger> logger, bool throwExceptions)
    : DutImpl(std::make_shared<ClientImpl>(wlanIndex, connection, logger), logger, throwExceptions)
{
}

DutImpl::DutImpl(std::shared_ptr<Client> client, std::shared_ptr<Logger> logger, bool throwExceptions)
    : m_client(client)
    , m_logger(logger)
    , m_throwExceptions(throwExceptions)
{
    if (!client) {
        throw std::invalid_argument("Parameter 'client' cannot be a null pointer");
    }
    if (!logger) {
        throw std::invalid_argument("Parameter 'logger' cannot be a null pointer");
    }

    m_status = std::make_shared<Status>();
}

std::string DutImpl::getLastError() const
{
    return m_lastError;
}

void DutImpl::setThrowExceptions(bool throwExceptions)
{
    m_throwExceptions = throwExceptions;
}

std::string DutImpl::inputToString(const std::vector<std::pair<std::string, std::string>>& parameters)
{
    return "[in] " + toString(parameters);
}

std::string DutImpl::outputToString(const std::vector<std::pair<std::string, std::string>>& results)
{
    return "[out] " + toString(results);
}

void DutImpl::logInput(const std::vector<std::pair<std::string, std::string>>& parameters) const
{
    m_logger->log(LogLevel::LOG_LEVEL_DEBUG, inputToString(parameters));
}

void DutImpl::logOutput(const std::vector<std::pair<std::string, std::string>>& results) const
{
    m_logger->log(LogLevel::LOG_LEVEL_DEBUG, outputToString(results));
}

bool DutImpl::execute(const std::string& functionName, const std::function<void()>& functionCode)
{
    auto startTime = std::chrono::steady_clock::now();

    if (m_wlanIndex == UINT8_MAX) {
        m_wlanIndex = m_client->getWlanIndex();
    }
    std::string where = "(" + toString(m_wlanIndex) + ") ";

    m_logger->log(LogLevel::LOG_LEVEL_INFO, where + "Entry to " + functionName + "()");

    // Use a shared_ptr with a custom deleter and the RAII programming idiom to emulate the
    // `finally` block of a `try-finally` clause.
    std::shared_ptr<int> finally(nullptr, [this, functionName, where, startTime](const int*) {
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime);
        m_logger->log(LogLevel::LOG_LEVEL_INFO, where + "Exit from " + functionName + "(), elapsed time: " + toString(elapsedTime.count()) + " ms");
    });

    try {

        if (functionName == "driverInit") {
            if (m_initialized) {
                throw std::logic_error("Already initialized");
            }
        } else if (!m_initialized) {
            throw std::logic_error("Not initialized");
        }

        m_lastError = "";

        functionCode();

    } catch (std::exception const& e) {
        m_lastError = e.what();

        m_logger->log(LogLevel::LOG_LEVEL_ERROR, where + "Exception at " + functionName + "() with message: " + m_lastError);

        if (m_throwExceptions) {
            throw;
        }

        return false;
    }

    return true;
}

bool DutImpl::driverInit(bool snifferMode, NvMemoryType memoryType, NvMemorySize memorySize, Band band)
{
    return execute("driverInit", [this, snifferMode, memoryType, memorySize, band]() {
        logInput({ { "snifferMode", toString(snifferMode) }, { "memoryType", toString(memoryType) }, { "memorySize", toString(memorySize) }, { "band", toString(band) } });

        m_transmitter = std::make_shared<Transmitter>(m_client);

        // TODO: memorySize might change after resetting MAC if memoryType is flash (see WLANRTSYS-37328)
        m_client->resetMac(memoryType, memorySize, true);

        // Reset status
        m_status->reset();
        m_status->snifferMode.setValue(snifferMode);
        m_status->regulationType.setValue(defaultRegulationType);
        m_status->closedLoop.setValue(defaultClosedLoopEnabled);
        m_status->transmitPowerLevel.setValue(defaultTransmitPowerLevel);
        m_status->zwdfsChannel.setValue(defaultZwdfsChannel);
        m_status->zwdfsBandwidth.setValue(defaultZwdfsBandwidth);
        m_status->radarDetectionBandwidth.setValue(defaultRadarDetectionBandwidth);

        dutGetFwInfo_t fwInfo {};
        m_client->getFwInfo(fwInfo);

        auto currentBand = static_cast<Band>(fwInfo.currentRfBandPsd);
        auto supportedBands = fwInfo.supportedBandsPsd;
        if ((band != Band::BAND_INVALID) && (band != currentBand)) {
            if (!isBandSupported(band, supportedBands)) {
                throw std::invalid_argument("Band " + dut::toString(band) + " not supported");
            }

            m_client->setBand(band);
            m_client->getFwInfo(fwInfo);
        }

        m_status->spectrumBandwidth.setValue(static_cast<Bandwidth>(fwInfo.spectrumMode));
        m_status->enabledRxAntennaMask.setValue(fwInfo.operationalRxAntMask);
        m_status->enabledTxAntennaMask.setValue(fwInfo.operationalTxAntMask);
        m_status->enabledZwdfsAntennaMask.setValue(0x0);
        m_status->availableRxAntennaMask.setValue(fwInfo.psdMaxRxAntMask);
        m_status->availableTxAntennaMask.setValue(fwInfo.psdMaxTxAntMask);
        m_status->availableZwdfsAntennaMask.setValue(fwInfo.zwdfsAntMask);
        m_status->band.setValue(static_cast<Band>(fwInfo.currentRfBandPsd));
        m_status->supportedBands.setValue(dut::getSupportedBands(fwInfo.supportedBandsPsd));

        addVap(snifferMode, fwInfo.operationMode);

        m_nvm = std::make_shared<DeviceNvm>(m_client, m_logger);
        if (!m_nvm->setType(memoryType)) {
            throw std::invalid_argument("Invalid NVM type (" + toString(memoryType) + ")");
        }
        if (!m_nvm->setSize(memorySize)) {
            throw std::invalid_argument("Invalid NVM size (" + toString(memorySize) + ")");
        }
        m_nvm->load();

        m_calibrationFile = createCalibrationFile(m_nvm->getVersion(), m_nvm->getData(), m_nvm->getSize());

        ChipID chipId {};
        m_client->readChipId(chipId);
        if (chipId < ChipID::CHIP_ID_GEN6) {
            throw std::runtime_error("Chip ID " + dut::toHexString(static_cast<uint16_t>(chipId)) + " not supported");
        }
        m_status->chipId.setValue(chipId);

        std::array<uint8_t, Client::combinedVersionSize> combinedVersion {};
        m_client->getCombinedVersion(combinedVersion);
        parseCombinedVersion(reinterpret_cast<char*>(combinedVersion.data()), m_componentVersions);

        m_client->setHdkConfig(defaultOfflineCalMask, defaultOnlineCalMask, m_status->band.getValue());
        m_status->offlineCalMask.setValue(defaultOfflineCalMask);
        m_status->onlineCalMask.setValue(defaultOnlineCalMask);

        m_client->setRxAggregationEnabled(defaultRxAggregationEnabled);
        m_status->rxAggregationEnabled.setValue(defaultRxAggregationEnabled);
        m_status->setClipper.setValue(defaultClipperEnabled);

        // Make sure device is not transmitting anything (transmitter status is initially idle)
        m_client->stopTx();
        m_client->stopCw();
        m_client->setSpacelessTxEnabled(defaultSpacelessTxEnabled);
        m_status->spacelessTxEnabled.setValue(defaultSpacelessTxEnabled);

        // Make sure RX calibration is not running
        m_status->rxCalibrationRunning.setValue(true);
        stopRxCalibrationImpl();

        m_client->setIfs(defaultIfs);
        m_status->ifs.setValue(defaultIfs);

        setEnabledRxAntennas(m_status->enabledRxAntennaMask.getValue());
        setEnabledTxAntennas(m_status->enabledTxAntennaMask.getValue());
        if (isZwdfsAvailable()) {
            setEnabledZwdfsAntenna(m_status->zwdfsChannel.getValue(), m_status->zwdfsBandwidth.getValue(), m_status->radarDetectionBandwidth.getValue(), m_status->enabledZwdfsAntennaMask.getValue());
        }

        m_initialized = true;
    });
}

bool DutImpl::driverRelease()
{
    return execute("driverRelease", [this]() {
        m_transmitter->stop();

        stopRxCalibrationImpl();
        removeVap();

        m_initialized = false;
    });
}

bool DutImpl::flushNvm()
{
    return execute("flushNvm", [this]() {
        m_transmitter->stop();
        stopRxCalibrationImpl();

        m_client->flushNvMemory(m_status->chipId.getValue(), m_nvm->getType(), FileType::FILE_TYPE_CALIBRATION);

        m_status->vapAdded.setValue(false);
        m_status->lowestChannel.reset();
        m_status->offlineCalMask.reset();
        m_status->onlineCalMask.reset();
    });
}

bool DutImpl::getAvailableRxAntennaMask(AntennaMask& antennaMask)
{
    return execute("getAvailableRxAntennaMask", [this, &antennaMask]() {
        antennaMask = m_status->availableRxAntennaMask.getValue();

        logOutput({ { "antennaMask", toHexString(antennaMask) } });
    });
}

bool DutImpl::getAvailableTxAntennaMask(AntennaMask& antennaMask)
{
    return execute("getAvailableTxAntennaMask", [this, &antennaMask]() {
        antennaMask = m_status->availableTxAntennaMask.getValue();

        logOutput({ { "antennaMask", toHexString(antennaMask) } });
    });
}

bool DutImpl::getBand(Band& band)
{
    return execute("getBand", [this, &band]() {
        band = m_status->band.getValue();

        logOutput({ { "band", toString(band) } });
    });
}

bool DutImpl::getSupportedBands(std::vector<Band>& supportedBands)
{
    return execute("getSupportedBands", [this, &supportedBands]() {
        supportedBands = m_status->supportedBands.getValue();

        logOutput({ { "bandSupport", toString(supportedBands.begin(), supportedBands.end()) } });
    });
}

bool DutImpl::getBbicCddValues(uint8_t numTxAntennas, uint32_t& offset1, uint32_t& offset2, uint32_t& offset3)
{
    return execute("getBbicCddValues", [this, numTxAntennas, &offset1, &offset2, &offset3]() {
        logInput({ { "numTxAntennas", toString(numTxAntennas) } });

        if (numTxAntennas > maxNumTxAntennas) {
            throw std::invalid_argument("The specified number of antennas (" + toString(numTxAntennas) + ") exceeds maximum allowed value (" + toString(maxNumTxAntennas) + ")");
        }

        m_client->getBbicCddValues(numTxAntennas, offset1, offset2, offset3);

        logOutput({ { "offset1", toString(offset1) }, { "offset2", toString(offset2) }, { "offset3", toString(offset3) } });
    });
}

bool DutImpl::getCalibrationFileVersion(CalibrationFileVersion& version, CalibrationFileSubversion& subversion)
{
    return execute("getCalibrationFileVersion", [this, &version, &subversion]() {
        version = m_calibrationFile->getVersion();
        subversion = m_calibrationFile->getSubversion();

        logOutput({ { "version", toString(version) }, { "subversion", toString(subversion) } });
    });
}

bool DutImpl::getCardInfo(uint8_t& countryCode, std::array<uint8_t, cardInfoMacAddressSize>& macAddress, std::array<uint8_t, cardInfoSerialNumberSize>& serialNumber, uint8_t& week, uint8_t& year)
{
    return execute("getCardInfo", [this, &countryCode, &macAddress, &serialNumber, &week, &year]() {
        m_calibrationFile->getCountryCode(countryCode);
        m_calibrationFile->getMacAddress(macAddress);
        m_calibrationFile->getSerialNumber(serialNumber);
        m_calibrationFile->getWeek(week);
        m_calibrationFile->getYear(year);

        logOutput({ { "countryCode", toString(countryCode) }, { "macAddress", toString(macAddress.data(), cardInfoMacAddressSize) }, { "serialNumber", toString(serialNumber.data(), cardInfoSerialNumberSize) }, { "week", toString(week) }, { "year", toString(year) } });
    });
}

bool DutImpl::getChipId(ChipID& chipId)
{
    return execute("getChipId", [this, &chipId]() {
        chipId = m_status->chipId.getValue();

        logOutput({ { "chipId", toString(chipId) } });
    });
}

bool DutImpl::getComponentVersion(VersionedComponent component, std::string& version)
{
    return execute("getComponentVersion", [this, component, &version]() {
        logInput({ { "component", toString(component) } });

        version = m_componentVersions[component];

        logOutput({ { "version", version } });
    });
}

bool DutImpl::getEnabledRxAntennaMask(AntennaMask& antennaMask)
{
    return execute("getEnabledRxAntennaMask", [this, &antennaMask]() {
        antennaMask = m_status->enabledRxAntennaMask.getValue();

        logOutput({ { "antennaMask", toHexString(antennaMask) } });
    });
}

bool DutImpl::getEnabledTxAntennaMask(AntennaMask& antennaMask)
{
    return execute("getEnabledTxAntennaMask", [this, &antennaMask]() {
        antennaMask = m_status->enabledTxAntennaMask.getValue();

        logOutput({ { "antennaMask", toHexString(antennaMask) } });
    });
}

bool DutImpl::getFemType(FemType& femType)
{
    return execute("getFemType", [this, &femType]() {
        m_client->getFemType(femType);

        logOutput({ { "femType", toString(femType) } });
    });
}

bool DutImpl::getHardwareType(HardwareType& hardwareType)
{
    return execute("getHardwareType", [this, &hardwareType]() {
        hardwareType = dut::getHardwareType(m_status->chipId.getValue());

        logOutput({ { "hardwareType", toString(static_cast<uint32_t>(hardwareType)) } });
    });
}

bool DutImpl::getMacPacketCounters(uint32_t& receivedPackets)
{
    return execute("getMacPacketCounters", [this, &receivedPackets]() {
        m_client->getMacPacketCounters(receivedPackets);

        logOutput({ { "receivedPackets", toString(receivedPackets) } });
    });
}

bool DutImpl::getMpduPacketCounters(uint32_t& receivedPackets, uint32_t& errorPackets)
{
    return execute("getMpduPacketCounters", [this, &receivedPackets, &errorPackets]() {
        m_client->getMpduPacketCounters(receivedPackets, errorPackets);

        logOutput({ { "receivedPackets", toString(receivedPackets) }, { "errorPackets", toString(errorPackets) } });
    });
}

bool DutImpl::getNmseValues(std::array<std::array<int32_t, dut::dpdTotalCalibrationPoints>, dut::maxNumTxAntennas>& values)
{
    return execute("getNmseValues", [this, &values]() {
        m_client->getNmseValues(values);
    });
}

bool DutImpl::getNvmSize(size_t& size)
{
    return execute("getNvmSize", [this, &size]() {
        size = m_nvm->getSize();

        logOutput({ { "size", toString(size) } });
    });
}

bool DutImpl::getNvmType(NvMemoryType& type)
{
    return execute("getNvmType", [this, &type]() {
        type = m_nvm->getType();

        logOutput({ { "type", toString(type) } });
    });
}

bool DutImpl::getNvmVersion(uint8_t& version)
{
    return execute("getNvmVersion", [this, &version]() {
        version = m_nvm->getVersion();

        logOutput({ { "version", toString(version) } });
    });
}

bool DutImpl::getPhyMode(PhyMode& phyMode)
{
    return execute("getPhyMode", [this, &phyMode]() {
        if (!m_status->phyMode.isValueSet()) {
            throw std::logic_error("PhyMode not set");
        }

        phyMode = m_status->phyMode.getValue();

        logOutput({ { "phyMode", toString(phyMode) } });
    });
}

bool DutImpl::getPhyPacketCounters(uint32_t& receivedPackets, uint32_t& crcErrors, uint32_t& forwardedPackets)
{
    return execute("getPhyPacketCounters", [this, &receivedPackets, &crcErrors, &forwardedPackets]() {
        m_client->getPhyPacketCounters(receivedPackets, crcErrors, forwardedPackets);

        logOutput({ { "receivedPackets", toString(receivedPackets) }, { "crcErrors", toString(crcErrors) }, { "forwardedPackets", toString(forwardedPackets) } });
    });
}

bool DutImpl::getProductionFlag(bool& productionFlag)
{
    return execute("getProductionFlag", [this, &productionFlag]() {
        m_calibrationFile->getProductionFlag(productionFlag);

        logOutput({ { "productionFlag", toString(productionFlag) } });
    });
}

bool DutImpl::calculateMaxPacketLength(PhyMode phyMode, uint32_t& maxPacketLength)
{
    return execute("calculateMaxPacketLength", [this, phyMode, &maxPacketLength]() {
        logInput({ { "phyMode", toString(phyMode) } });

        maxPacketLength = Rates::getMaxPacketLength(phyMode);

        logOutput({ { "maxPacketLength", toString(maxPacketLength) } });
    });
}

bool DutImpl::calculatePacketLength(PhyMode phyMode, Bandwidth signalBandwidth, Mcs mcs, uint8_t nss, uint32_t numSymbols, uint32_t& packetLength)
{
    return execute("calculatePacketLength", [this, phyMode, signalBandwidth, mcs, nss, numSymbols, &packetLength]() {
        logInput({ { "phyMode", toString(phyMode) }, { "signalBandwidth", toString(signalBandwidth) }, { "mcs", toString(mcs) }, { "nss", toString(nss) }, { "numSymbols", toString(numSymbols) } });

        checkBandwidth(signalBandwidth);
        checkNumberOfSpatialStreams(phyMode, nss);

        packetLength = Rates::getPacketLength(phyMode, signalBandwidth, mcs, nss, numSymbols);

        logOutput({ { "packetLength", toString(packetLength) } });
    });
}

bool DutImpl::calculatePhyDataRate(PhyMode phyMode, Bandwidth signalBandwidth, Mcs mcs, uint8_t nss, Gi gi, float& rateMbps)
{
    return execute("calculatePhyDataRate", [this, phyMode, signalBandwidth, mcs, nss, gi, &rateMbps]() {
        logInput({ { "phyMode", toString(phyMode) }, { "signalBandwidth", toString(signalBandwidth) }, { "mcs", toString(mcs) }, { "nss", toString(nss) }, { "gi", toString(gi) } });

        checkBandwidth(signalBandwidth);
        checkNumberOfSpatialStreams(phyMode, nss);
        checkGuardInterval(phyMode, gi);

        rateMbps = Rates::getRate(phyMode, signalBandwidth, mcs, nss, gi);

        logOutput({ { "rateMbps", toString(rateMbps, 1) } });
    });
}

bool DutImpl::getRssiCalibrationData(CalibrationFileVersion& version, std::vector<RssiCalibrationData_t>& data)
{
    return execute("getRssiCalibrationData", [this, &version, &data]() {
        version = m_calibrationFile->getVersion();

        std::vector<std::shared_ptr<RssiCalibrationData>> _data;
        m_calibrationFile->getRssiCalibrationData(_data);
        for (const auto& item : _data) {
            data.push_back(item->data());
        }

        logOutput({ { "version", toString(version) }, { "data.size()", toString(data.size()) } });
    });
}

bool DutImpl::getRxRateInfo(uint8_t& mcs, uint8_t& nss)
{
    return execute("RxRateInfo", [this, &mcs, &nss]() {
        if (!m_status->phyMode.isValueSet()) {
            throw std::logic_error("PhyMode not set");
        }

        m_client->getRxRateInfo(mcs, nss);

        logOutput({ { "mcs", toString(mcs) }, { "nss", toString(nss) } });
    });
}

bool DutImpl::getTemperature(float& temperature)
{
    return execute("getTemperature", [this, &temperature]() {
        m_client->getTemperature(temperature);

        logOutput({ { "temperature", toString(temperature, 2) } });
    });
}

bool DutImpl::getTssiCalibrationData(CalibrationFileVersion& version, std::vector<TssiCalibrationData_t>& data)
{
    return execute("getTssiCalibrationData", [this, &version, &data]() {
        version = m_calibrationFile->getVersion();

        std::vector<std::shared_ptr<TssiCalibrationData>> _data;
        m_calibrationFile->getTssiCalibrationData(_data);
        for (const auto& item : _data) {
            data.push_back(item->data());
        }

        logOutput({ { "version", toString(version) }, { "data.size()", toString(data.size()) } });
    });
}

bool DutImpl::getXtalCalValue(uint16_t& xtalValue)
{
    return execute("getXtalCalValue", [this, &xtalValue]() {
        m_calibrationFile->getXtalValue(xtalValue);

        logOutput({ { "xtalValue", toString(xtalValue) } });
    });
}

bool DutImpl::getXtalRegValue(uint16_t& xtalValue)
{
    return execute("getXtalRegValue", [this, &xtalValue]() {
        m_transmitter->pause();

        m_client->getXtalValue(xtalValue);

        m_transmitter->resume();

        logOutput({ { "xtalValue", toString(xtalValue) } });
    });
}

bool DutImpl::getZwdfsStatus(AntennaMask& antennaMask, bool& enabled)
{
    return execute("getZwdfsStatus", [this, &antennaMask, &enabled]() {
        antennaMask = m_status->availableZwdfsAntennaMask.getValue();
        enabled = m_status->enabledZwdfsAntennaMask.getValue() != 0x0;

        logOutput({ { "antennaMask", toHexString(antennaMask) }, { "enabled", toString(enabled) } });
    });
}

bool DutImpl::loadBeamformingMatrixFromFileSet(
    const BeamformingFilePathSet_t& primarySet, const BeamformingFilePathSet_t& secondarySet)
{
    return execute("loadBeamformingMatrixFromFileSet", [this, primarySet, secondarySet]() {
        // Log input parameters
        std::vector<std::pair<std::string, std::string>> logParams = {
            { "primarySet.headerFile", primarySet.headerFile },
            { "primarySet.valuesFile", primarySet.valuesFile }
        };
        if (beamforming_utils::hasExtendedEhtValues(primarySet)) {
            logParams.emplace_back("primarySet.extValuesEhtFile", primarySet.extValuesEhtFile);
        }
        if (beamforming_utils::isValid(secondarySet)) {
            logParams.emplace_back("secondarySet.headerFile", secondarySet.headerFile);
            logParams.emplace_back("secondarySet.valuesFile", secondarySet.valuesFile);
            if (beamforming_utils::hasExtendedEhtValues(secondarySet)) {
                logParams.emplace_back("secondarySet.extValuesEhtFile", secondarySet.extValuesEhtFile);
            }
        }
        logInput(logParams);

        // Check that the DUT is not transmitting
        if (m_transmitter->getState() != Transmitter::State::IDLE) {
            throw std::logic_error("Cannot load beamforming matrix while transmitting, stop transmission");
        }

        // Check that phyMode has been set (via setChannel)
        if (!m_status->phyMode.isValueSet()) {
            throw std::logic_error("Channel not set");
        }

        // Check that signal bandwidth has been set (via setRate)
        if (!m_status->signalBandwidth.isValueSet()) {
            throw std::logic_error("Rate not set");
        }

        // Read beamforming data
        beamforming_utils::BeamformingData dataSet = beamforming_utils::readBeamformingFiles(primarySet, secondarySet);

        // Determine hardware type
        HardwareType hwType = dut::getHardwareType(m_status->chipId.getValue());

        // Helper lambda to validate header data
        auto validateHeader = [&](const std::vector<uint32_t>& headerData, const std::string& fileName, const std::string& setName) -> BeamformingHeaderInfo_t {
            uint32_t headerLSB = headerData[0];
            BeamformingHeaderInfo_t headerInfo;

            if (!beamforming_utils::extractBeamformingHeaderInfo(headerLSB, hwType, headerInfo)) {
                throw std::invalid_argument("Invalid beamforming header in " + setName + " file '" + fileName + "'");
            }

            // Ensure it matches current DUT PhyMode and Signal Bandwidth
            auto validation = beamforming_utils::validateBeamformingCompatibility(headerInfo,
                m_status->signalBandwidth.getValue(),
                m_status->phyMode.getValue());
            if (!validation.isValid) {
                throw std::invalid_argument(setName + ": " + validation.errorMessage);
            }

            return headerInfo;
        };

        // Validate primary header
        BeamformingHeaderInfo_t primaryHeaderInfo = validateHeader(dataSet.primary.header, primarySet.headerFile, "primary");

        // Validate secondary header if present
        if (dataSet.hasSecondary()) {
            validateHeader(dataSet.secondary.header, secondarySet.headerFile, "secondary");
        }

        // Write beamforming data to hardware based on hardware type
        switch (hwType) {
        case HardwareType::HARDWARE_TYPE_GEN6:

            // // Wave600 only supports single-band operation
            // if (dataSet.hasSecondary()) {
            //     throw std::invalid_argument("Wave600 hardware does not support EHT 320MHz beamforming");
            // }
            // beamforming_utils::writeWave600BeamformingHeader(m_client, dataSet.primary.header);
            // beamforming_utils::writeWave600BeamformingValues(m_client, dataSet.primary.values);

            throw std::runtime_error("The Wave600 loadBeamformingMatrixFromFileSet implementation hasn't been tested.");
            break;

        case HardwareType::HARDWARE_TYPE_GEN7: {
            beamforming_utils::Wave700BeamformingAddresses primaryAddresses = beamforming_utils::getWave700PrimaryAddresses();

            // Write primary band data
            beamforming_utils::writeWave700BeamformingHeader(m_client, dataSet.primary.header, primaryAddresses);

            uint8_t suPage = beamforming_utils::beamforming::wave700::suPage::vhtHeEht; // Default: 0
            beamforming_utils::writeWave700BeamformingValues(m_client, dataSet.primary.values, primaryAddresses, suPage);

            // Write EHT extra phases for higher bandwidths (160MHz, 320MHz)
            if ((primaryHeaderInfo.phyMode == PhyMode::PHY_MODE_BE) && ((primaryHeaderInfo.bandwidth == Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY) || (primaryHeaderInfo.bandwidth == Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY))) {
                uint8_t ehtExtraPage = beamforming_utils::beamforming::wave700::suPage::ehtExtra; // 4
                beamforming_utils::writeWave700BeamformingValues(m_client, dataSet.primary.extValues, primaryAddresses, ehtExtraPage);
            }

            // Write secondary segment data if present (for EHT 320MHz)
            if (dataSet.hasSecondary()) {
                // For secondary data (EHT 320MHz), use secondary addresses
                beamforming_utils::Wave700BeamformingAddresses secondaryAddresses = beamforming_utils::getWave700SecondaryAddresses();
                beamforming_utils::writeWave700BeamformingHeader(m_client, dataSet.secondary.header, secondaryAddresses);
                beamforming_utils::writeWave700BeamformingValues(m_client, dataSet.secondary.values, secondaryAddresses, suPage);

                // Write EHT extra phases for secondary as well
                uint8_t ehtExtraPage = beamforming_utils::beamforming::wave700::suPage::ehtExtra; // 4
                beamforming_utils::writeWave700BeamformingValues(m_client, dataSet.secondary.extValues, secondaryAddresses, ehtExtraPage);
            }
        } break;

        default:
            throw std::invalid_argument("Unsupported hardware type for beamforming: " + toString(static_cast<uint32_t>(hwType)));
        }
    });
}

bool DutImpl::loadNvmFromFile(const std::string& fileName)
{
    return execute("loadNvmFromFile", [this, fileName]() {
        logInput({ { "fileName", fileName } });

        m_nvm->loadFromFile(fileName);

        try {
            m_calibrationFile = createCalibrationFile(m_nvm->getVersion(), m_nvm->getData(), m_nvm->getSize());
        } catch (std::exception const& e) {
            throw std::invalid_argument("Invalid calibration file '" + fileName + "': " + e.what());
        }
    });
}

bool DutImpl::readMemory(ChipModule chipModule, size_t address, uint8_t* data, size_t length)
{
    return execute("readMemory", [this, chipModule, address, data, length]() {
        logInput({ { "chipModule", toString(chipModule) }, { "address", toHexString(address) }, { "length", toString(length) } });

        m_client->readMemory(chipModule, address, data, length);

        m_logger->log(LogLevel::LOG_LEVEL_TRACE, outputToString({ { "buffer", toString(data, length) } }));
    });
}

bool DutImpl::readNvm(size_t address, uint8_t* data, size_t length, bool useCache)
{
    return execute("readNvm", [this, address, data, length, useCache]() {
        logInput({ { "address", toString(address) }, { "length", toString(length) }, { "useCache", toString(useCache) } });

        m_nvm->read(address, data, length, useCache);
    });
}

bool DutImpl::readRegister(ChipModule chipModule, size_t address, uint32_t mask, uint32_t& value)
{
    return execute("readRegister", [this, chipModule, address, mask, &value]() {
        logInput({ { "chipModule", toString(chipModule) }, { "address", toHexString(address) }, { "mask", toHexString(mask) } });

        m_client->readMemory(chipModule, address, reinterpret_cast<uint8_t*>(&value), sizeof(value));

        uint8_t shift = getMaskShift(mask);
        value = (value & mask) >> shift;

        logOutput({ { "value", toHexString(value) } });
    });
}

bool DutImpl::resetMacPacketCounters()
{
    return execute("resetMacPacketCounters", [this]() {
        m_client->resetMacPacketCounters();
    });
}

bool DutImpl::resetMpduPacketCounters()
{
    return execute("resetMpduPacketCounters", [this]() {
        m_client->resetMpduPacketCounters();
    });
}

bool DutImpl::resetPhyPacketCounters()
{
    return execute("resetPhyPacketCounters", [this]() {
        m_client->resetPhyPacketCounters();
    });
}

bool DutImpl::rxMeasure(uint32_t numCaptures, uint32_t captureInterval, bool disabled)
{
    return execute("rxMeasure", [this, numCaptures, captureInterval, disabled]() {
        logInput({ { "numCaptures", toString(numCaptures) }, { "captureInterval", toString(captureInterval) }, { "disabled", toString(disabled) } });

        if (m_status->rxPacketLimit.isValueSet()) {
            throw std::logic_error("Rx Packet Limit running");
        }

        if (!disabled && m_status->rxMeasureRunning.isValueSet()) {
            throw std::logic_error("Rx Measure already running");
        }

        if (!m_status->phyMode.isValueSet()) {
            throw std::logic_error("PhyMode not set");
        }

        if (!m_status->lowestChannel.isValueSet()) {
            throw std::logic_error("Channel not set");
        }

        if (m_status->enabledRxAntennaMask.getValue().bitCount() == 0) {
            throw std::logic_error("Illegal antenna selection (at least one RX antenna must be selected)");
        }

        m_client->rxMeasure(numCaptures, captureInterval, disabled, m_status->phyMode.getValue());

        if (disabled)
            m_status->rxMeasureRunning.reset();
        else
            m_status->rxMeasureRunning.setValue(true);
    });
}

bool DutImpl::saveNvmToFile(const std::string& fileName)
{
    return execute("saveNvmToFile", [this, fileName]() {
        logInput({ { "fileName", fileName } });

        m_nvm->saveToFile(fileName);
    });
}

bool DutImpl::setBbicCddValues(uint8_t numTxAntennas, uint32_t offset1, uint32_t offset2, uint32_t offset3)
{
    return execute("setBbicCddValues", [this, numTxAntennas, offset1, offset2, offset3]() {
        logInput({ { "numTxAntennas", toString(numTxAntennas) }, { "offset1", toString(offset1) }, { "offset2", toString(offset2) }, { "offset3", toString(offset3) } });

        if (numTxAntennas > maxNumTxAntennas) {
            throw std::invalid_argument("The specified number of antennas (" + toString(numTxAntennas) + ") exceeds maximum allowed value (" + toString(maxNumTxAntennas) + ")");
        }

        m_client->setBbicCddValues(numTxAntennas, offset1, offset2, offset3);
    });
}

bool DutImpl::setCalibrationFileVersion(CalibrationFileVersion version, CalibrationFileSubversion subversion)
{
    return execute("getCalibrationFileVersion", [this, version, subversion]() {
        logInput({ { "version", toString(version) }, { "subversion", toString(subversion) } });

        m_calibrationFile->setVersion(version);
        m_calibrationFile->setSubversion(subversion);
    });
}

bool DutImpl::setCardInfo(uint8_t countryCode, const std::array<uint8_t, cardInfoMacAddressSize>& macAddress, const std::array<uint8_t, cardInfoSerialNumberSize>& serialNumber, uint8_t week, uint8_t year)
{
    return execute("setCardInfo", [this, countryCode, macAddress, serialNumber, week, year]() {
        logOutput({ { "countryCode", toString(countryCode) }, { "macAddress", toString(macAddress.data(), cardInfoMacAddressSize) }, { "serialNumber", toString(serialNumber.data(), cardInfoSerialNumberSize) }, { "week", toString(week) }, { "year", toString(year) } });

        m_calibrationFile->setCountryCode(countryCode);
        m_calibrationFile->setMacAddress(macAddress);
        m_calibrationFile->setSerialNumber(serialNumber);
        m_calibrationFile->setWeek(week);
        m_calibrationFile->setYear(year);
    });
}

bool DutImpl::setChannel(PhyMode phyMode, Bandwidth spectrumBandwidth, uint8_t lowestChannel, uint8_t primaryChannelIndex, RegulationType regulationType)
{
    return execute("setChannel", [this, phyMode, spectrumBandwidth, lowestChannel, primaryChannelIndex, regulationType]() {
        logInput({ { "phyMode", toString(phyMode) }, { "spectrumBandwidth", toString(spectrumBandwidth) }, { "lowestChannel", toString(lowestChannel) }, { "primaryChannelIndex", toString(primaryChannelIndex) }, { "regulationType", toString(regulationType) } });

        checkBandwidth(spectrumBandwidth);
        checkPrimaryChannelIndex(spectrumBandwidth, primaryChannelIndex);
        checkLowestChannel(lowestChannel, phyMode, m_status->band.getValue(), spectrumBandwidth);

        // If ZWDFS is enabled and there's no specific ZWDFS channel to use, then check if the
        // specified channel and bandwith are valid for ZWDFS
        if ((m_status->enabledZwdfsAntennaMask.getValue() != 0x0) && (m_status->zwdfsChannel.getValue() == 0)) {
            checkZwdfsChannel(lowestChannel, spectrumBandwidth);
        }

        m_transmitter->stop();

#ifdef STATUS_CACHE_ENABLED
        bool cacheHit = m_status->phyMode.isValueSet() && (m_status->phyMode.getValue() == phyMode);
        cacheHit = cacheHit && m_status->spectrumBandwidth.isValueSet() && (m_status->spectrumBandwidth.getValue() == spectrumBandwidth);
        cacheHit = cacheHit && m_status->lowestChannel.isValueSet() && (m_status->lowestChannel.getValue() == lowestChannel);
        cacheHit = cacheHit && m_status->primaryChannelIndex.isValueSet() && (m_status->primaryChannelIndex.getValue() == primaryChannelIndex);
        cacheHit = cacheHit && m_status->regulationType.isValueSet() && (m_status->regulationType.getValue() == regulationType);
        if (cacheHit) {
            return;
        }
#endif

        // Configure antenna parameters using calibration data.
        // The data sent to FW when setting TPC antenna parameters is already contained in the
        // calibration file so, why is it necessary to set it again?
        // Even though it looks like unnecessary, we cannot skip setting antenna parameters
        // because otherwise setRate() fails after flushNvm() (i.e.: after calibration is
        // complete).
        // In Wave600D2 and Wave700 the file is in FW so there's no need to send the data again.
        if ((m_status->chipId.getValue() == ChipID::CHIP_ID_GEN6) || (m_status->chipId.getValue() == ChipID::CHIP_ID_GEN6_B)) {
            setTpcAntennaParams(lowestChannel);
        }

        // In Wave600 calibration file is not in FW. So send the RSSI calibration data to FW.
        // For "antenna mask per channel" feature we need to call writeRssiCalDataToFw every time
        // antenna mask is changed (in case that channel was changed and antenna mask was changed
        // accordingly).
        // In order to reduce code complexity, currently we call writeRssiCalDataToFw on every
        // set channel (without checking if antenna mask was changed and it is really needed).
        // In Wave600D2 and Wave700 the file is in FW so there's no need to send the data again.
        if ((m_status->chipId.getValue() == ChipID::CHIP_ID_GEN6) || (m_status->chipId.getValue() == ChipID::CHIP_ID_GEN6_B)) {
            writeRssiCalDataToFw(m_status->enabledRxAntennaMask.getValue());
        }

        m_client->setChannel(lowestChannel, primaryChannelIndex, spectrumBandwidth, m_status->enabledTxAntennaMask.getValue(), regulationType);

        // If ZWDFS is enabled and there's no specific ZWDFS channel to use, then update ZWDFS
        if ((m_status->enabledZwdfsAntennaMask.getValue() != 0x0) && (m_status->zwdfsChannel.getValue() == 0)) {
            setEnabledZwdfsAntenna(lowestChannel, spectrumBandwidth, m_status->radarDetectionBandwidth.getValue(), m_status->enabledZwdfsAntennaMask.getValue());
        }

        // Setting the channel on FW also enables all RX antennas as a side effect.
        // To avoid misalignment between cached value and FW value, set the RX antenna mask in FW to current cached value.
        m_client->setEnabledRxAntennas(m_status->enabledRxAntennaMask.getValue());

        m_status->phyMode.setValue(phyMode);
        m_status->spectrumBandwidth.setValue(spectrumBandwidth);
        m_status->lowestChannel.setValue(lowestChannel);
        m_status->primaryChannelIndex.setValue(primaryChannelIndex);
        m_status->regulationType.setValue(regulationType);

        // Invalidate current rate settings
        m_status->mcs.reset();
        m_status->nss.reset();
        m_status->gi.reset();
        m_status->ltf.reset();
    });
}

bool DutImpl::setEnabledRxAntennaMask(const AntennaMask& antennaMask)
{
    return execute("setEnabledRxAntennaMask", [this, antennaMask]() {
        logInput({ { "antennaMask", toHexString(antennaMask) } });

        AntennaMask availableAntennaMask = m_status->availableRxAntennaMask.getValue() | m_status->availableZwdfsAntennaMask.getValue();

        if (!antennaMask.isValid(availableAntennaMask)) {
            throw std::invalid_argument("Invalid antenna mask " + toHexString(antennaMask) + ", available RX antenna mask is " + toHexString(availableAntennaMask));
        }

        AntennaMask zwdfsAntennaMask = antennaMask & m_status->availableZwdfsAntennaMask.getValue();
        setEnabledZwdfsAntennaMaskImpl(zwdfsAntennaMask);

        setEnabledRxAntennaMaskImpl(antennaMask);
    });
}

bool DutImpl::setEnabledTxAntennaMask(const AntennaMask& antennaMask)
{
    return execute("setEnabledTxAntennaMask", [this, antennaMask]() {
        logInput({ { "antennaMask", toHexString(antennaMask) } });

        if (!antennaMask.isValid(m_status->availableTxAntennaMask.getValue())) {
            throw std::invalid_argument("Invalid antenna mask " + toHexString(antennaMask) + ", available TX antenna mask is " + toHexString(m_status->availableTxAntennaMask.getValue()));
        }

        setEnabledTxAntennaMaskImpl(antennaMask);
    });
}

void DutImpl::addVap(bool snifferMode, uint8_t fwInfoOperationMode) const
{
    bool vapAdded = false;
    auto operationMode = static_cast<UmiOperationMode_e>(fwInfoOperationMode);

    // 1. We're in normal operating mode
    // 2. We're configured to sniffer mode but operation mode is not sniffer
    // 3. We're configured to non-sniffer mode but operation mode is sniffer
    if ((operationMode == UmiOperationMode_e::OPERATION_MODE_NORMAL) || (snifferMode != (operationMode == UmiOperationMode_e::OPERATION_MODE_SNIFFER)) && (operationMode != UmiOperationMode_e::OPERATION_MODE_INVALID)) {
        m_client->removeVap(0);
        vapAdded = false;
    } else if (operationMode == UmiOperationMode_e::OPERATION_MODE_DUT) {
        vapAdded = true;
    } else if ((operationMode == UmiOperationMode_e::OPERATION_MODE_SNIFFER) && snifferMode) {
        vapAdded = true;
    }

    if (!vapAdded) {
        m_client->addVap(0, snifferMode);
        vapAdded = true;
    }

    m_status->vapAdded.setValue(vapAdded);
}

void DutImpl::removeVap() const
{
    if (m_status->vapAdded.getValue()) {
        m_client->removeVap(0);
        m_status->vapAdded.setValue(false);
    }
}

bool DutImpl::setClipper(bool enabled)
{
    return execute("setClipper", [this, enabled]() {
        logInput({ { "enabled", toString(enabled) } });

#ifdef STATUS_CACHE_ENABLED
        if (m_status->setClipper.getValue() == enabled) {
            return;
        }
#endif

        m_client->setClipper(enabled);

        m_status->setClipper.setValue(enabled);
    });
}

void DutImpl::setEnabledRxAntennas(const AntennaMask& antennaMask) const
{
    m_transmitter->pause();

    m_client->setEnabledRxAntennas(antennaMask);

    m_transmitter->resume();
}

void DutImpl::setEnabledRxAntennaMaskImpl(const AntennaMask& antennaMask) const
{
#ifdef STATUS_CACHE_ENABLED
    if (m_status->enabledRxAntennaMask.getValue() == antennaMask) {
        return;
    }
#endif

    setEnabledRxAntennas(antennaMask);

    m_status->enabledRxAntennaMask.setValue(antennaMask);
}

void DutImpl::setEnabledTxAntennas(const AntennaMask& antennaMask) const
{
    m_client->setEnabledTxAntennas(antennaMask);

    //TODO:check : Teseter add setPower to each send antenna, because power set to 0 by send..need to check why
    for (uint8_t antenna = 0; antenna < maxNumTxAntennas; antenna++) {
        if (antennaMask.isBitSet(antenna)) {
            m_client->setTransmitPowerLevel(antenna, m_status->spectrumBandwidth.getValue(), m_status->transmitPowerLevel.getValue());
        }
    }
}

void DutImpl::setEnabledTxAntennaMaskImpl(const AntennaMask& antennaMask) const
{
#ifdef STATUS_CACHE_ENABLED
    if (m_status->enabledTxAntennaMask.getValue() == antennaMask) {
        return;
    }
#endif

    if (antennaMask == 0) {
        m_transmitter->stop();
    } else {
        m_transmitter->pause();
    }

    setEnabledTxAntennas(antennaMask);

    m_status->enabledTxAntennaMask.setValue(antennaMask);

    // Don't resume TX if new antenna mask is 0
    if (antennaMask != 0) {
        m_transmitter->resume();
    }
}

void DutImpl::setEnabledZwdfsAntenna(uint8_t channel, Bandwidth bandwidth, Bandwidth radarDetectionBandwidth, const AntennaMask& antennaMask) const
{
    bool zwdfsEnabled = antennaMask != 0;

    m_client->setZwdfsEnabled(
        zwdfsEnabled,
        channel,
        bandwidth,
        radarDetectionBandwidth,
        m_status->regulationType.getValue());
}

void DutImpl::setEnabledZwdfsAntennaMaskImpl(const AntennaMask& antennaMask) const
{
#ifdef STATUS_CACHE_ENABLED
    if (m_status->enabledZwdfsAntennaMask.getValue() == antennaMask) {
        return;
    }
#endif

    bool validConfiguration = true;
    uint8_t channel = m_status->zwdfsChannel.getValue();
    Bandwidth bandwidth = m_status->zwdfsBandwidth.getValue();
    Bandwidth radarDetectionBandwidth = m_status->radarDetectionBandwidth.getValue();

    if (m_status->zwdfsChannel.getValue() == 0) {
        channel = m_status->lowestChannel.getValue();
        bandwidth = m_status->spectrumBandwidth.getValue();

        try {
            checkZwdfsChannel(channel, bandwidth);
        } catch (...) {
            // Execution reaches this point when trying to enable the ZWDFS antenna with
            // setEnabledRxAntennaMask() and current channel is not a valid ZWDFS channel.
            // In this case ignore the exception, do not enable the ZWFS antenna and save the
            // mask in the cache. The ZWDFS antenna will be enabled later, while on setChannel()
            validConfiguration = false;
        }
    }

    if (validConfiguration) {
        setEnabledZwdfsAntenna(channel, bandwidth, radarDetectionBandwidth, antennaMask);
    }

    m_status->enabledZwdfsAntennaMask.setValue(antennaMask);
}

void DutImpl::setTpcAntennaParams(uint8_t channel) const
{
    std::vector<std::shared_ptr<TssiCalibrationData>> tssiCalibrationData;
    m_calibrationFile->getTssiCalibrationData(tssiCalibrationData);
    if (tssiCalibrationData.empty()) {
        return;
    }

    for (uint8_t antenna = 0; antenna < maxNumTxAntennas; antenna++) {
        if (!m_status->enabledTxAntennaMask.getValue().isBitSet(antenna)) {
            continue;
        }

        size_t tpcFreqLen = 0;
        auto buffer = std::make_shared<ResizableBuffer>();

        // Go over all channel bandwidths and add antenna calibration data to the buffer
        const std::vector<Bandwidth> bandwidths = { Bandwidth::BANDWIDTH_TWENTY, Bandwidth::BANDWIDTH_FOURTY, Bandwidth::BANDWIDTH_EIGHTY, Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY };
        for (Bandwidth bw : bandwidths) {

            // Get the closest two channels for current channel and BW
            std::vector<TssiFreq> tssiFreqs;
            m_calibrationFile->getTssiFreqs(channel, bw, m_status->band.getValue(), tssiCalibrationData, tssiFreqs);

            constexpr int tpcFreqNum = 2;
            for (int freq = tpcFreqNum - 1; freq >= 0; freq--) {

                size_t lengthWithoutAntennaParams = buffer->length();

                if (tssiFreqs.size() > static_cast<size_t>(freq)) {
                    tssiFreqs[freq].calibrationData->serializeAntennaParams(antenna, buffer);
                }

                size_t lengthWithAntennaParams = buffer->length();

                if (tssiFreqs.size() > static_cast<size_t>(freq)) {
                    // getTssiAntSize() == ANT_DATA_CONST_SIZE + S2D_GAIN_BYTE_SIZE[numRegions] + S2D_OFFSET_BYTE_SIZE * numRegions + AB_BYTE_SIZE * numRegions
                    // Example: if numRegions = 1, then tpcFreqLen = 9
                    tpcFreqLen = lengthWithAntennaParams - lengthWithoutAntennaParams - 2;
                }

                // Add padding up to the maximum possible length (legacy value of calDataTssiVer4.TSSI_CIS_FREQ_XY_CAL_SIZE)
                constexpr size_t maximumAntennaParamsLength = 24;
                size_t paddingLength = maximumAntennaParamsLength - (lengthWithAntennaParams - lengthWithoutAntennaParams);
                for (size_t i = 0; i < paddingLength; i++) {
                    buffer->append(0);
                }
            }
        }

        m_client->setTransmitPowerControlAntennaParams(antenna, static_cast<uint8_t>(tpcFreqLen), buffer->data(), buffer->length());
    }
}

bool DutImpl::validateBeamformingHeaderRegister(PhyMode expectedPhyMode, Bandwidth expectedBandwidth)
{
    return execute("validateBeamformingHeaderRegister", [this, expectedPhyMode, expectedBandwidth]() {
        logInput({ { "expectedPhyMode", toString(expectedPhyMode) },
            { "expectedBandwidth", toString(expectedBandwidth) } });

        validateBeamformingHeaderRegisterImpl(expectedPhyMode, expectedBandwidth);
    });
}

void DutImpl::validateBeamformingHeaderRegisterImpl(PhyMode expectedPhyMode, Bandwidth expectedBandwidth) const
{
    HardwareType hwType = dut::getHardwareType(m_status->chipId.getValue());

    if (hwType != HardwareType::HARDWARE_TYPE_GEN6 && hwType != HardwareType::HARDWARE_TYPE_GEN7) {
        throw std::runtime_error("Beamforming is not supported on this hardware type: " + toString(static_cast<uint32_t>(hwType)));
    }

    auto validateHeader = [this, hwType, expectedPhyMode, expectedBandwidth](uint32_t headerAddress, const std::string& headerName = "header") -> std::string {
        uint32_t headerLSB = 0;
        m_client->readMemory(ChipModule::CHIP_MODULE_REGISTER, headerAddress,
            reinterpret_cast<uint8_t*>(&headerLSB), sizeof(headerLSB));

        // Check if header has been written at all (all zeros indicates uninitialized)
        // For Wave600, 0x00000000 is a valid HT 20MHz header, so skip this check
        if (hwType == HardwareType::HARDWARE_TYPE_GEN7 && headerLSB == 0) {
            return "Beamforming " + headerName + " has not been written (contains all zeros). Please configure beamforming before validation.";
        }

        BeamformingHeaderInfo_t headerInfo;
        if (!beamforming_utils::extractBeamformingHeaderInfo(headerLSB, hwType, headerInfo)) {
            return "Failed to extract beamforming header info from " + headerName + ". Header may be corrupted.";
        }

        std::vector<std::string> mismatches;

        // Check PHY mode compatibility
        if (!beamforming_utils::isBeamformingPhyModeCompatible(headerInfo.phyMode, expectedPhyMode)) {
            mismatches.push_back("PHY mode mismatch - Expected: " + toString(expectedPhyMode) + ", Found in " + headerName + ": " + toString(headerInfo.phyMode));
        }

        // Check bandwidth
        if (headerInfo.bandwidth != expectedBandwidth) {
            mismatches.push_back("Bandwidth mismatch - Expected: " + toString(expectedBandwidth) + ", Found in " + headerName + ": " + toString(headerInfo.bandwidth));
        }

        if (!mismatches.empty()) {
            std::string errorMsg = "Beamforming " + headerName + " validation failed:\n";
            for (const auto& mismatch : mismatches) {
                errorMsg += "  - " + mismatch + "\n";
            }
            return errorMsg;
        }

        return ""; // Success - no error message
    };

    // Validate headers based on hardware type and bandwidth
    std::string errorMessage;
    uint32_t primaryAddress = (hwType == HardwareType::HARDWARE_TYPE_GEN6) ? beamforming_utils::beamforming::wave600::headerAddress : beamforming_utils::beamforming::wave700::primaryBfHeaderAddress;

    if (hwType == HardwareType::HARDWARE_TYPE_GEN7 && expectedBandwidth == Bandwidth::BANDWIDTH_THREE_HUNDRED_TWENTY) {
        // EHT 320MHz: both headers must be valid
        uint32_t secondaryAddress = beamforming_utils::beamforming::wave700::secondaryBfHeaderAddress;

        std::string primaryError = validateHeader(primaryAddress, "primary header");
        std::string secondaryError = validateHeader(secondaryAddress, "secondary header");

        if (!primaryError.empty() || !secondaryError.empty()) {
            errorMessage = "EHT 320MHz validation failed:\n";
            if (!primaryError.empty()) {
                errorMessage += primaryError;
            }
            if (!secondaryError.empty()) {
                errorMessage += secondaryError;
            }
        }
    } else {
        // All other cases: only primary header
        errorMessage = validateHeader(primaryAddress);
    }

    if (!errorMessage.empty()) {
        throw std::runtime_error(errorMessage);
    }
}

void DutImpl::writeRssiCalDataToFw(const AntennaMask& antennaMask) const
{
    auto calibrationFile = static_cast<dut::CalibrationFileVer6*>(m_calibrationFile.get());

    auto buffer = calibrationFile->getRssiCalibrationDataBytes(antennaMask);

    if (buffer) {
        m_client->setRssiCalData(buffer->data(), buffer->length());
    }
}

bool DutImpl::setHdkConfig(uint32_t offlineCalMask, uint32_t onlineCalMask)
{
    return execute("setHdkConfig", [this, offlineCalMask, onlineCalMask]() {
        logInput({ { "offlineCalMask", toHexString(offlineCalMask) }, { "onlineCalMask", toHexString(onlineCalMask) } });

#ifdef STATUS_CACHE_ENABLED
        if ((m_status->offlineCalMask.getValue() == offlineCalMask) && (m_status->onlineCalMask.getValue() == onlineCalMask)) {
            return;
        }
#endif

        m_client->setHdkConfig(offlineCalMask, onlineCalMask, m_status->band.getValue());

        m_status->offlineCalMask.setValue(offlineCalMask);
        m_status->onlineCalMask.setValue(onlineCalMask);

        m_status->spectrumBandwidth.reset();
        m_status->lowestChannel.reset();
    });
}

bool DutImpl::setIfs(uint32_t ifs)
{
    return execute("setIfs", [this, ifs]() {
        logInput({ { "ifs", toString(ifs) } });

        if (ifs < minInterframeSpace) {
            throw std::invalid_argument("Invalid interframe space (minimum allowed value is " + toString(minInterframeSpace) + " us)");
        }

#ifdef STATUS_CACHE_ENABLED
        if (m_status->ifs.getValue() == ifs) {
            return;
        }
#endif

        m_client->setIfs(ifs);

        m_status->ifs.setValue(ifs);
    });
}

bool DutImpl::setProductionFlag(bool productionFlag, bool writeToNvm)
{
    return execute("setProductionFlag", [this, productionFlag, writeToNvm]() {
        logInput({ { "productionFlag", toString(productionFlag) }, { "writeToNvm", toString(writeToNvm) } });

        m_calibrationFile->setProductionFlag(productionFlag);

        if (writeToNvm) {
            size_t offset = m_calibrationFile->getProductionFlagOffset();
            uint8_t value = m_calibrationFile->getProductionFlagValue();
            m_nvm->write(offset, &value, 1);
        }
    });
}

bool DutImpl::setRate(Bandwidth signalBandwidth, Mcs mcs, uint8_t nss, Gi gi, Ltf ltf, float& rateMbps)
{
    return execute("setRate", [this, signalBandwidth, mcs, nss, gi, ltf, &rateMbps]() {
        logInput({ { "signalBandwidth", toString(signalBandwidth) }, { "mcs", toString(mcs) }, { "nss", toString(nss) }, { "gi", toString(gi) }, { "ltf", toString(ltf) } });

        if (!m_status->lowestChannel.isValueSet()) {
            throw std::logic_error("Channel not set");
        }

        checkBandwidth(signalBandwidth);
        checkNumberOfSpatialStreams(m_status->phyMode.getValue(), nss);

        // Compute PHY data rate for the specified parameters.
        // If any of the parameters or their combination is not valid, method throws an exception.
        rateMbps = Rates::getRate(m_status->phyMode.getValue(), signalBandwidth, mcs, nss, gi);

        // Compute MCS index for the specified parameters.
        // Packet transmission can start only after MCS index has been set.
        uint8_t mcsIndex = Rates::getMcsIndex(m_status->phyMode.getValue(), mcs, nss);

#ifdef STATUS_CACHE_ENABLED
        bool cacheHit = m_status->signalBandwidth.isValueSet() && (m_status->signalBandwidth.getValue() == signalBandwidth);
        cacheHit = cacheHit && m_status->mcs.isValueSet() && (m_status->mcs.getValue() == mcs);
        cacheHit = cacheHit && m_status->nss.isValueSet() && (m_status->nss.getValue() == nss);
        cacheHit = cacheHit && m_status->gi.isValueSet() && (m_status->gi.getValue() == gi);
        cacheHit = cacheHit && m_status->ltf.isValueSet() && (m_status->ltf.getValue() == ltf);
        if (cacheHit) {
            return;
        }
#endif

        m_transmitter->pause();

        m_client->setRate(m_status->phyMode.getValue(), signalBandwidth, mcsIndex, nss, gi, ltf);

        m_status->signalBandwidth.setValue(signalBandwidth);
        m_status->mcs.setValue(mcs);
        m_status->nss.setValue(nss);
        m_status->gi.setValue(gi);
        m_status->ltf.setValue(ltf);

        // The AP device changes output power after setting the rate so restore it.
        for (uint8_t antenna = 0; antenna < maxNumTxAntennas; antenna++) {
            if (m_status->enabledTxAntennaMask.getValue().isBitSet(antenna)) {
                m_client->setTransmitPowerLevel(antenna, m_status->spectrumBandwidth.getValue(), m_status->transmitPowerLevel.getValue());
            }
        }

        m_transmitter->resume();

        logOutput({ { "rateMbps", toString(rateMbps, 1) } });
    });
}

bool DutImpl::setRssiCalibrationData(const std::vector<RssiCalibrationData_t>& data)
{
    return execute("setRssiCalibrationData", [this, data]() {
        logInput({ { "data.size()", toString(data.size()) } });

        CalibrationFileVersion version = m_calibrationFile->getVersion();

        std::vector<std::shared_ptr<RssiCalibrationData>> _data;

        for (size_t i = 0; i < data.size(); i++) {
            std::shared_ptr<RssiCalibrationData> item;

            if (version == CalibrationFileVersion::CALIBRATION_FILE_VERSION_6) {
                item = std::make_shared<RssiCalibrationDataVer6>(data[i]);
            } else if (version == CalibrationFileVersion::CALIBRATION_FILE_VERSION_7) {
                item = std::make_shared<RssiCalibrationDataVer7>(data[i]);
            } else {
                throw std::invalid_argument("Invalid version " + toString(version));
            }
            _data.push_back(item);
        }

        m_calibrationFile->setRssiCalibrationData(_data);
    });
}

bool DutImpl::setRuParams(uint32_t userOne, uint32_t userTwo)
{
    return execute("setRuParams", [this, userOne, userTwo]() {
        logInput({ { "userOne", toHexString(userOne) }, { "userTwo", toHexString(userTwo) } });

        if (!m_status->signalBandwidth.isValueSet()) {
            throw std::logic_error("Rate not set");
        }

        if (userOne > maxRuParamsUserOneValue) {
            throw std::invalid_argument("Invalid User 1 Value! Maximum allowed value is " + toString(maxRuParamsUserOneValue));
        }

        if (userTwo > maxRuParamsUserTwoValue) {
            throw std::invalid_argument("Invalid User 2 Value! Maximum allowed value is " + toString(maxRuParamsUserTwoValue));
        }

#ifdef STATUS_CACHE_ENABLED
        if (m_status->userOne.isValueSet() && m_status->userOne.getValue() == userOne && m_status->userTwo.isValueSet() && m_status->userTwo.getValue() == userTwo) {
            return;
        }
#endif
        m_client->setRuParams(userOne, userTwo);
        m_status->userOne.setValue(userOne);
        m_status->userTwo.setValue(userTwo);
    });
}

bool DutImpl::setRxAggregationEnabled(bool enabled)
{
    return execute("setRxAggregationEnabled", [this, enabled]() {
        logInput({ { "enabled", toString(enabled) } });

#ifdef STATUS_CACHE_ENABLED
        if (m_status->rxAggregationEnabled.getValue() == enabled) {
            return;
        }
#endif

        m_client->setRxAggregationEnabled(enabled);

        m_status->rxAggregationEnabled.setValue(enabled);
    });
}

bool DutImpl::setSpacelessTxEnabled(bool enabled)
{
    return execute("setSpacelessTxEnabled", [this, enabled]() {
        logInput({ { "enabled", toString(enabled) } });

        if (!m_status->lowestChannel.isValueSet()) {
            throw std::logic_error("Channel not set");
        }

        if (!m_status->mcs.isValueSet()) {
            throw std::logic_error("Transmission rate not set");
        }

        if (m_status->spacelessTxEnabled.getValue() != enabled) {

            m_client->setSpacelessTxEnabled(enabled);

            m_status->spacelessTxEnabled.setValue(enabled);
        }
    });
}

bool DutImpl::setTransmitPowerControl(bool closedLoop, uint8_t powerLimit)
{
    return execute("setTransmitPowerControl", [this, closedLoop, powerLimit]() {
        logInput({ { "closedLoop", toString(closedLoop) }, { "powerLimit", toString(powerLimit) } });

        ChipID chipId = m_status->chipId.getValue();
        HardwareType hardwareType = dut::getHardwareType(chipId);
        CalibrationType calibrationType = dut::getCalibrationType(hardwareType);

#ifdef STATUS_CACHE_ENABLED
        bool cacheHit = m_status->closedLoop.isValueSet() && (closedLoop == m_status->closedLoop.getValue());
        cacheHit = cacheHit && m_status->transmitPowerLimit.isValueSet() && (m_status->transmitPowerLimit.getValue() == powerLimit);
        if (cacheHit) {
            return;
        }
#endif

        m_transmitter->pause();

        m_client->setTransmitPowerControl(calibrationType, closedLoop, powerLimit);

        // The FW recalculates the limits when auto-power is enabled (i.e.: a valid power limit
        // has been set), but on set channel only.
        // Therefore, set channel again if required.
        bool setChannelRequired = m_status->lowestChannel.isValueSet();
        setChannelRequired = setChannelRequired && (powerLimit != 0xff);
        setChannelRequired = setChannelRequired && (!m_status->transmitPowerLimit.isValueSet() || (m_status->transmitPowerLimit.getValue() != powerLimit));
        if (setChannelRequired) {
            m_client->setChannel(
                m_status->lowestChannel.getValue(),
                m_status->primaryChannelIndex.getValue(),
                m_status->spectrumBandwidth.getValue(),
                m_status->enabledTxAntennaMask.getValue(),
                m_status->regulationType.getValue());
        }

        m_status->closedLoop.setValue(closedLoop);
        m_status->transmitPowerLimit.setValue(powerLimit);

        m_transmitter->resume();
    });
}

bool DutImpl::setTssiCalibrationData(const std::vector<TssiCalibrationData_t>& data)
{
    return execute("setTssiCalibrationData", [this, data]() {
        logInput({ { "data.size()", toString(data.size()) } });

        CalibrationFileVersion version = m_calibrationFile->getVersion();

        std::vector<std::shared_ptr<TssiCalibrationData>> _data;

        for (size_t i = 0; i < data.size(); i++) {
            std::shared_ptr<TssiCalibrationData> item;

            if (version == CalibrationFileVersion::CALIBRATION_FILE_VERSION_6) {
                item = std::make_shared<TssiCalibrationDataVer6>(data[i]);
            } else if (version == CalibrationFileVersion::CALIBRATION_FILE_VERSION_7) {
                item = std::make_shared<TssiCalibrationDataVer7>(data[i]);
            } else {
                throw std::invalid_argument("Invalid version " + toString(version));
            }
            _data.push_back(item);
        }

        m_calibrationFile->setTssiCalibrationData(_data);
    });
}

bool DutImpl::setXtalCalValue(uint16_t xtalValue)
{
    return execute("setXtalCalValue", [this, xtalValue]() {
        logInput({ { "xtalValue", toString(xtalValue) } });

        m_calibrationFile->setXtalValue(xtalValue);
    });
}

bool DutImpl::setXtalRegValue(uint16_t xtalValue)
{
    return execute("setXtalRegValue", [this, xtalValue]() {
        logInput({ { "xtalValue", toString(xtalValue) } });

        m_transmitter->pause();

        m_client->setXtalValue(xtalValue);

        m_transmitter->resume();
    });
}

bool DutImpl::setZwdfsConfiguration(uint8_t zwdfsChannel, Bandwidth zwdfsBandwidth, Bandwidth radarDetectionBandwidth)
{
    return execute("setZwdfsConfiguration", [this, zwdfsChannel, zwdfsBandwidth, radarDetectionBandwidth]() {
        logInput({ { "zwdfsChannel", toString(zwdfsChannel) }, { "zwdfsBandwidth", toString(zwdfsBandwidth) }, { "radarDetectionBandwidth", toString(radarDetectionBandwidth) } });

        // Check if ZWDFS is available
        if (!isZwdfsAvailable()) {
            throw std::invalid_argument("The ZWDFS feature is not available");
        }

        if (zwdfsChannel != 0) {
            checkZwdfsChannel(zwdfsChannel, zwdfsBandwidth);
        }
        checkZwdfsRadarDetectionBandwidth(radarDetectionBandwidth);

#ifdef STATUS_CACHE_ENABLED
        bool cacheHit = m_status->zwdfsChannel.getValue() == zwdfsChannel;
        cacheHit = cacheHit && m_status->zwdfsBandwidth.getValue() == zwdfsBandwidth;
        cacheHit = cacheHit && m_status->radarDetectionBandwidth.getValue() == radarDetectionBandwidth;
        if (cacheHit) {
            return;
        }
#endif

        if (m_status->enabledZwdfsAntennaMask.getValue() != 0x0) {
            setEnabledZwdfsAntenna(zwdfsChannel, zwdfsBandwidth, radarDetectionBandwidth, m_status->enabledZwdfsAntennaMask.getValue());
        }

        m_status->zwdfsChannel.setValue(zwdfsChannel);
        m_status->zwdfsBandwidth.setValue(zwdfsBandwidth);
        m_status->radarDetectionBandwidth.setValue(radarDetectionBandwidth);
    });
}

bool DutImpl::startCalibration(const StartCalibrationParams_t& params, uint8_t& status)
{
    return execute("startCalibration", [this, &params, &status]() {
        logInput({ { "mask", toHexString(params.mask) }, { "maskType", toString(params.maskType) }, { "type", toString(params.type) } });

        constexpr uint8_t maxMaskType = 1;
        if (params.maskType > maxMaskType) {
            throw std::invalid_argument("Invalid maskType (" + toString(params.maskType) + "). Maximum allowed value is " + toString(maxMaskType));
        }
        constexpr uint8_t maxCalibrationType = 1;
        if (params.type > maxCalibrationType) {
            throw std::invalid_argument("Invalid type (" + toString(params.type) + "). Maximum allowed value is " + toString(maxCalibrationType));
        }

        m_client->startCalibration(params, status);

        logOutput({ { "status", toString(status) } });
    });
}

bool DutImpl::startCw(int8_t amplitude, int16_t tone)
{
    return execute("startCw", [this, amplitude, tone]() {
        logInput({ { "amplitude", toString(amplitude) }, { "tone", toString(tone) } });

        const int8_t maxAmplitude = 2;
        const int8_t minAmplitude = -2;
        if (amplitude < minAmplitude) {
            throw std::invalid_argument("Invalid amplitude (" + toString(amplitude) + "). Minimum allowed value is " + toString(minAmplitude));
        } else if ((amplitude > 2) && (amplitude != 9) && (amplitude != 10)) {
            throw std::invalid_argument("Invalid amplitude (" + toString(amplitude) + "). Maximum allowed value is " + toString(maxAmplitude));
        }

        const int16_t maxTone = 511;
        const int16_t minTone = -512;
        if (tone < minTone) {
            throw std::invalid_argument("Invalid tone (" + toString(tone) + "). Minimum allowed value is " + toString(minTone));
        } else if (tone > maxTone) {
            throw std::invalid_argument("Invalid tone (" + toString(tone) + "). Maximum allowed value is " + toString(maxTone));
        }

        if (!m_status->lowestChannel.isValueSet()) {
            throw std::logic_error("Channel not set");
        }

        if (m_status->enabledTxAntennaMask.getValue().bitCount() == 0) {
            throw std::logic_error("Illegal antenna selection (at least one TX antenna must be selected)");
        }

        m_transmitter->startCw(amplitude, tone);
    });
}

bool DutImpl::startTx(uint16_t repetitions, uint32_t packetLength, bool longData, bool beamforming, CodingType codingType)
{
    return execute("startTx", [this, repetitions, packetLength, longData, beamforming, codingType]() {
        logInput({ { "repetitions", toString(repetitions) }, { "packetLength", toString(packetLength) }, { "longData", toString(longData) }, { "beamforming", toString(beamforming) }, { "codingType", toString(static_cast<uint32_t>(codingType)) } });

        if (!m_status->lowestChannel.isValueSet()) {
            throw std::logic_error("Channel not set");
        }

        if (m_status->enabledTxAntennaMask.getValue().bitCount() == 0) {
            throw std::logic_error("Illegal antenna selection (at least one TX antenna must be selected)");
        }

        auto maxPacketLength = longData ? Rates::getMaxPacketLength(m_status->phyMode.getValue()) : maxDefaultPacketLength;
        if (packetLength > maxPacketLength) {
            throw std::invalid_argument("Invalid packet length: the specified value (" + toString(packetLength) + ") exceeds maximum allowed value (" + toString(maxPacketLength) + ") for current PHY mode");
        }

        if (!m_status->mcs.isValueSet()) {
            throw std::logic_error("Transmission rate not set");
        }

        // Determine coding type based on default logic
        bool ldpc;
        switch (codingType) {
        case CodingType::CODING_TYPE_LDPC:
            ldpc = true;
            break;
        case CodingType::CODING_TYPE_BCC:
            ldpc = false;
            break;
        case CodingType::CODING_TYPE_AUTO: {
            // Use LDPC for 11ax (AX) and later, BCC for earlier standards
            PhyMode phyMode = m_status->phyMode.getValue();
            if (phyMode >= PhyMode::PHY_MODE_AX) {
                ldpc = true;
            } else {
                ldpc = false;
            }
            break;
        }
        default:
            throw std::invalid_argument("Invalid coding type (" + toString(static_cast<uint32_t>(codingType)) + ")");
            break;
        }

        // Validate coding type against PHY mode constraints
        PhyMode phyMode = m_status->phyMode.getValue();
        Bandwidth signalBandwidth = m_status->signalBandwidth.getValue();
        Mcs mcs = m_status->mcs.getValue();

        if (ldpc) {
            // LDPC validation
            if (phyMode == PhyMode::PHY_MODE_B) {
                throw std::invalid_argument("LDPC is not supported for 11b PHY mode");
            }
            if (phyMode == PhyMode::PHY_MODE_A || phyMode == PhyMode::PHY_MODE_G) {
                throw std::invalid_argument("LDPC is not supported for 11a/g PHY modes (only BCC is supported)");
            }
        } else {
            // BCC validation
            if (phyMode == PhyMode::PHY_MODE_AX || phyMode == PhyMode::PHY_MODE_BE) {
                // For 11ax/be: BCC only supported if BW <= 20MHz and MCS <= 9
                if (signalBandwidth > Bandwidth::BANDWIDTH_TWENTY) {
                    throw std::invalid_argument("BCC is not supported for 11ax/be with bandwidth greater than 20MHz");
                }
                if (mcs > Mcs::MCS_256QAM_56) {
                    throw std::invalid_argument("BCC is not supported for 11ax/be with MCS greater than 9 (QAM256)");
                }
            }
        }

        // Validate beamforming settings
        if (beamforming) {
            validateBeamformingHeaderRegisterImpl(phyMode, signalBandwidth);
        }

        m_transmitter->startTx(repetitions, packetLength, longData, beamforming, ldpc);
    });
}

bool DutImpl::startRxPer(uint32_t packetLimit)
{
    return execute("startRxPer", [this, packetLimit]() {
        logInput({ { "packetLimit Count", toString(packetLimit) } });

        if (m_status->rxPacketLimit.isValueSet()) {
            throw std::logic_error("Rx PER measure already running");
        }

        if (m_status->rxMeasureRunning.isValueSet()) {
            throw std::logic_error("Rx Measure running");
        }

        if (!m_status->lowestChannel.isValueSet()) {
            throw std::logic_error("Channel not set");
        }

        if (m_status->enabledRxAntennaMask.getValue().bitCount() == 0) {
            throw std::logic_error("Illegal antenna selection (at least one RX antenna must be selected)");
        }

        m_client->startRxPer(packetLimit, m_status->phyMode.getValue());
        m_status->rxPacketLimit.setValue(packetLimit);
    });
}

bool DutImpl::stopCw()
{
    return execute("stopCw", [this]() {
        m_transmitter->stop();
    });
}

bool DutImpl::stopTx()
{
    return execute("stopTx", [this]() {
        m_transmitter->stop();
    });
}

bool DutImpl::stopRxPer(bool calcRxPer)
{
    return execute("stopRxPer", [this, calcRxPer]() {
        logInput({ { "calcRxPer", toString(calcRxPer) } });

        if (!m_status->rxPacketLimit.isValueSet()) {
            throw std::logic_error("Rx PER measure not running");
        }

        if (m_status->enabledRxAntennaMask.getValue().bitCount() == 0) {
            throw std::logic_error("Illegal antenna selection (at least one RX antenna must be selected)");
        }

        uint32_t totalPacketsReceived;
        uint32_t packetCounter;
        uint32_t rxPacketLimit = m_status->rxPacketLimit.getValue();

        m_client->stopRxPer(m_status->phyMode.getValue(), totalPacketsReceived, packetCounter);

        if (calcRxPer) {
            uint32_t goodPackets = rxPacketLimit - packetCounter;
            uint32_t badPackets = totalPacketsReceived - goodPackets;
            uint32_t dropped = (rxPacketLimit > totalPacketsReceived) ? (rxPacketLimit - totalPacketsReceived) : 0;
            float packetErrorRate = ((static_cast<float>(packetCounter)) / (static_cast<float>(rxPacketLimit))) * 100;

            logInput({ { "packetErrorRate", toString(packetErrorRate) }, { "Good Packets", toString(goodPackets) }, { "Bad Packets", toString(badPackets) }, { "Dropped Frames", toString(dropped) } });
        }

        m_status->rxPacketLimit.reset();
    });
}

bool DutImpl::writeCalibrationFile(NvMemoryType memoryType, NvMemorySize memorySize)
{
    return execute("writeCalibrationFile", [this, memoryType, memorySize]() {
        logInput({ { "memoryType", toString(memoryType) }, { "memorySize", toString(memorySize) } });

        if (!m_nvm->setType(memoryType)) {
            throw std::logic_error("Invalid NVM type (" + toString(memoryType) + ")");
        }

        if (!m_nvm->setSize(memorySize)) {
            throw std::logic_error("Invalid NVM size (" + toString(memorySize) + ")");
        }

        auto buffer = m_calibrationFile->serialize(static_cast<size_t>(memorySize));
        m_nvm->write(0, buffer->data(), buffer->length());
    });
}

bool DutImpl::writeMemory(ChipModule chipModule, size_t address, const uint8_t* data, size_t length)
{
    return execute("writeMemory", [this, chipModule, address, data, length]() {
        logInput({ { "chipModule", toString(chipModule) }, { "address", toHexString(address) }, { "length", toString(length) }, { "buffer", toString(data, length) } });

        m_client->writeMemory(chipModule, address, data, length);
    });
}

bool DutImpl::writeNvm(size_t address, const uint8_t* data, size_t length)
{
    return execute("writeNvm", [this, address, data, length]() {
        if (m_logger->getLogLevel() == LogLevel::LOG_LEVEL_TRACE) {
            m_logger->log(LogLevel::LOG_LEVEL_TRACE, inputToString({ { "address", toString(address) }, { "length", toString(length) }, { "buffer", toString(data, length) } }));
        } else {
            logInput({ { "address", toString(address) }, { "length", toString(length) } });
        }

        m_nvm->write(address, data, length);
    });
}

bool DutImpl::writeRegister(ChipModule chipModule, size_t address, uint32_t mask, uint32_t value)
{
    return execute("writeRegister", [this, chipModule, address, mask, value]() {
        logInput({ { "chipModule", toString(chipModule) }, { "address", toHexString(address) }, { "mask", toHexString(mask) }, { "value", toHexString(value) } });

        uint32_t _value = value;

        if (mask != UINT32_MAX) {
            uint32_t currentValue;
            uint8_t shift = getMaskShift(mask);
            readRegister(chipModule, address, mask, currentValue);
            _value = (currentValue & ~mask) | ((value << shift) & mask);
        }

        m_client->writeMemory(chipModule, address, reinterpret_cast<uint8_t*>(&_value), sizeof(value));
    });
}

bool DutImpl::isZwdfsAvailable() const
{
    return //
        (dut::getHardwareType(m_status->chipId.getValue()) == HardwareType::HARDWARE_TYPE_GEN7) && //
        (m_status->availableZwdfsAntennaMask.getValue() != 0x0);
}

}
