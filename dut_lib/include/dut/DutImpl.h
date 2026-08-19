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

#ifndef _DUT_DUT_IMPL_H_
#define _DUT_DUT_IMPL_H_

#include "dut/Dut.h"

#include <array>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#ifdef LINUX_HOST
#include <climits>
#endif

namespace dut {

class CalibrationFile;
class NonVolatileMemory;
class Client;
class Transmitter;
class Status;

/** 
 * Status cache is a feature introduced when DUT was ported from C# to C++
 * It is intended to improve performance as it prevents some redundant commands to be sent to FW
 * (i.e.: DUT does not ask FW to set channel 36 if DUT knows that channel is already set).
 * Comment out this line to disable status cache.
 */
#define STATUS_CACHE_ENABLED

class DUT_API DutImpl : public Dut {
public:
    /**
     * @brief Connection based class constructor.
     * 
     * Uses the specified connection to create a DUT client to communicate with the DUT server and 
     * then calls the client based constructor.
     * 
     * @param wlanIndex Index of the WiFi card (0, 2, 4).
     * @param connection Connection to send and receive messages to DUT server.
     * @param logger Logger to trace messages.
     * @param throwExceptions If set true, errors in API function calls are reported by throwing 
     * an exception. If false, no exception is thrown and API function calls return false on error. 
     * The error cause can be obtained by calling getLastError(). 
     */
    DutImpl(uint8_t wlanIndex, std::shared_ptr<Connection> connection, std::shared_ptr<Logger> logger, bool throwExceptions = false);

    /**
     * @brief Client based class constructor.
     * 
     * Uses the specified DUT client to communicate with the DUT server.
     * 
     * @param client Client to exchange messages with DUT server.
     * @param logger Logger to trace messages.
     * @param throwExceptions If set true, errors in API function calls are reported by throwing 
     * an exception. If false, no exception is thrown and API function calls return false on error. 
     * The error cause can be obtained by calling getLastError(). 
     */
    DutImpl(std::shared_ptr<Client> client, std::shared_ptr<Logger> logger, bool throwExceptions = false);

    /**
     * @brief Copy constructor
     *
     * Delete copy constructor to avoid having multiple copies of the same instance.
     */
    DutImpl(const DutImpl&) = delete;

    /**
     * @brief Assignment operator
     *
     * Delete assignment operator to avoid having multiple copies of the same instance.
     */
    DutImpl& operator=(const DutImpl&) = delete;

    std::string getLastError() const override;

    void setThrowExceptions(bool throwExceptions) override;

    bool calibrateTssiS2d(uint8_t lowerPowerLevel, uint8_t upperPowerLevel, uint32_t tssiLow, uint32_t tssiHigh, uint8_t region, uint8_t powerThreshold, uint8_t& gain, uint8_t& offset) override;
    bool calculateLinearRegression(const std::array<float, numPowerPoints>& powerVector, const std::array<std::array<uint16_t, numPowerPoints>, maxNumRxAntennas>& voltages, const std::array<uint16_t, maxNumRxAntennas>& offsets, const std::array<float, maxNumRxAntennas>& lnaGains, std::array<RssiAB_t, maxNumRxAntennas>& result, std::array<float, maxNumRxAntennas>& maxErrors) override;
    bool calculateOptimalS2dGains(const std::array<uint16_t, maxNumRxAntennas>& rssiVoltage1, const std::array<uint16_t, maxNumRxAntennas>& rssiVoltage2, int32_t regionS2dGain, std::array<int32_t, maxNumRxAntennas>& optimalS2dGains) override;
    bool calculateOptimalS2dOffsets(const std::array<uint16_t, maxNumRxAntennas>& rssiVoltage1, const std::array<uint16_t, maxNumRxAntennas>& rssiVoltage2, int32_t regionS2dGain, uint8_t regionS2dOffset, std::array<uint8_t, maxNumRxAntennas>& optimalS2dOffsets) override;
    bool calculateMaxPacketLength(PhyMode phyMode, uint32_t& maxPacketLength) override;
    bool calculatePacketLength(PhyMode phyMode, Bandwidth signalBandwidth, Mcs mcs, uint8_t nss, uint32_t numSymbols, uint32_t& packetLength) override;
    bool calculatePhyDataRate(PhyMode phyMode, Bandwidth signalBandwidth, Mcs mcs, uint8_t nss, Gi gi, float& rateMbps) override;
    bool calibrateRxLnaMidGains(const std::array<float, maxNumRxAntennas>& targetGains, const std::array<float, maxNumRxAntennas>& prxin, uint16_t numSamples, int8_t pgc1, int8_t pgc2, std::array<float, maxNumRxAntennas>& calcGain, std::array<uint8_t, maxNumRxAntennas>& calcGainCtrl) override;
    bool calculateRxLnaGains(const std::array<CorrelationResults_t, maxNumRxAntennas>& correlationResults, const std::array<float, maxNumRxAntennas>& prxin, int8_t pgc1, int8_t pgc2, int8_t pgc3, std::array<float, maxNumRxAntennas>& gains) override;
    bool calculateRxLnaSubBandGains(std::array<std::array<uint16_t, maxNumLnaSubBands>, maxNumRxAntennas>& freqResultTable) override;
    bool calculateRxPowerVector(int32_t maxPower, const std::array<float, maxNumRxAntennas>& lnaGains, int32_t lowerPowerBoundary, int32_t upperPowerBoundary, std::array<float, numPowerPoints>& powerVector) override;
    bool calculateRxRfFlatness(const std::array<std::array<int32_t, maxNumLnaSubBands>, maxNumRxAntennas>& P_Adc_array, const std::array<float, maxNumRxAntennas>& refGain, int8_t pgc1, int8_t pgc2, int8_t pgc3, const std::array<std::array<float, maxNumLnaSubBands>, maxNumRxAntennas>& pin, std::array<std::array<float, maxNumLnaSubBands>, maxNumRxAntennas>& results) override;
    bool calculateTxABCoefficients(const AntennaMask& antennaMask, uint8_t numRegions, const std::array<std::array<std::vector<Point_t>, maxNumTxRegions>, maxNumTxAntennas>& pointsPerAntennaAndRegion, float nLog, std::array<std::array<TssiAB_t, maxNumTxRegions>, maxNumTxAntennas>& coefficientsPerAntennaAndRegion, std::array<std::array<float, maxNumTxRegions>, maxNumTxAntennas>& errorsPerAntennaAndRegion) override;
    bool driverInit(bool snifferMode, NvMemoryType memoryType, NvMemorySize memorySize, Band band) override;
    bool driverRelease() override;
    bool flushNvm() override;
    bool getAvailableRxAntennaMask(AntennaMask& antennaMask) override;
    bool getAvailableTxAntennaMask(AntennaMask& antennaMask) override;
    bool getBand(Band& band) override;
    bool getSupportedBands(std::vector<Band>& supportedBands) override;
    bool getBbicCddValues(uint8_t numTxAntennas, uint32_t& offset1, uint32_t& offset2, uint32_t& offset3) override;
    bool getCalibrationFileVersion(CalibrationFileVersion& version, CalibrationFileSubversion& subversion) override;
    bool getCardInfo(uint8_t& countryCode, std::array<uint8_t, cardInfoMacAddressSize>& macAddress, std::array<uint8_t, cardInfoSerialNumberSize>& serialNumber, uint8_t& week, uint8_t& year) override;
    bool getChipId(ChipID& chipId) override;
    bool getComponentVersion(VersionedComponent component, std::string& version) override;
    bool getEnabledRxAntennaMask(AntennaMask& antennaMask) override;
    bool getEnabledTxAntennaMask(AntennaMask& antennaMask) override;
    bool getFemType(FemType& femType) override;
    bool getHardwareType(HardwareType& hardwareType) override;
    bool getInbandRssi(std::array<int16_t, maxNumRxAntennas>& rssi) override;
    bool getMacPacketCounters(uint32_t& receivedPackets) override;
    bool getMpduPacketCounters(uint32_t& receivedPackets, uint32_t& errorPackets) override;
    bool getNmseValues(std::array<std::array<int32_t, dut::dpdTotalCalibrationPoints>, dut::maxNumTxAntennas>& values) override;
    bool getNvmSize(size_t& size) override;
    bool getNvmType(NvMemoryType& type) override;
    bool getNvmVersion(uint8_t& version) override;
    bool getPhyMode(PhyMode& phyMode) override;
    bool getPhyPacketCounters(uint32_t& receivedPackets, uint32_t& crcErrors, uint32_t& forwardedPackets) override;
    bool getProductionFlag(bool& productionFlag) override;
    bool getRfRssiPower(std::array<int8_t, maxNumRxAntennas>& power) override;
    bool getRssi(uint8_t method, uint16_t numSamples, std::array<uint16_t, maxNumRxAntennas>& rssi) override;
    bool getRssiCalibrationData(CalibrationFileVersion& version, std::vector<RssiCalibrationData_t>& data) override;
    bool getRxCwPower(uint8_t method, int32_t freqOffset, uint16_t numSamples, std::array<CorrelationResults_t, maxNumRxAntennas>& correlationResults) override;
    bool getRxEvm(std::array<uint8_t, maxNumRxAntennas>& rxEvm) override;
    bool getRxRateInfo(uint8_t& mcs, uint8_t& nss) override;
    bool getTemperature(float& temperature) override;
    bool getTransmitPowerTableOffset(uint8_t antenna, Bandwidth bandwidth, int16_t& offset) override;
    bool getTransmitVoltages(std::array<uint32_t, maxNumTxAntennas>& voltages) override;
    bool getTssiCalibrationData(CalibrationFileVersion& version, std::vector<TssiCalibrationData_t>& data) override;
    bool getXtalCalValue(uint16_t& xtalValue) override;
    bool getXtalRegValue(uint16_t& xtalValue) override;
    bool getZwdfsStatus(AntennaMask& antennaMask, bool& enabled) override;
    bool loadBeamformingMatrixFromFileSet(const BeamformingFilePathSet_t& primarySet, const BeamformingFilePathSet_t& secondarySet = BeamformingFilePathSet_t {}) override;
    bool loadNvmFromFile(const std::string& fileName) override;
    bool measureRxLnaSubBandGains() override;
    bool readMemory(ChipModule chipModule, size_t address, uint8_t* data, size_t length) override;
    bool readNvm(size_t address, uint8_t* data, size_t length, bool useCache = true) override;
    bool readRegister(ChipModule chipModule, size_t address, uint32_t mask, uint32_t& value) override;
    bool resetMacPacketCounters() override;
    bool resetMpduPacketCounters() override;
    bool resetPhyPacketCounters() override;
    bool rxMeasure(uint32_t numCaptures, uint32_t captureInterval, bool disabled) override;
    bool saveNvmToFile(const std::string& fileName) override;
    bool setBbicCddValues(uint8_t numTxAntennas, uint32_t offset1, uint32_t offset2, uint32_t offset3) override;
    bool setCalibrationFileVersion(CalibrationFileVersion version, CalibrationFileSubversion subversion) override;
    bool setCardInfo(uint8_t countryCode, const std::array<uint8_t, cardInfoMacAddressSize>& macAddress, const std::array<uint8_t, cardInfoSerialNumberSize>& serialNumber, uint8_t week, uint8_t year) override;
    bool setChannel(PhyMode phyMode, Bandwidth spectrumBandwidth, uint8_t lowestChannel, uint8_t primaryChannelIndex, RegulationType regulationType) override;
    bool setClipper(bool enabled) override;
    bool setEnabledRxAntennaMask(const AntennaMask& antennaMask) override;
    bool setEnabledTxAntennaMask(const AntennaMask& antennaMask) override;
    bool setHdkConfig(uint32_t offlineCalMask, uint32_t onlineCalMask) override;
    bool setIfs(uint32_t ifs) override;
    bool setProductionFlag(bool productionFlag, bool writeToNvm) override;
    bool setRate(Bandwidth signalBandwidth, Mcs mcs, uint8_t nss, Gi gi, Ltf ltf, float& rateMbps) override;
    bool setRssiCalibrationData(const std::vector<RssiCalibrationData_t>& data) override;
    bool setRxGainBlocks(uint8_t lnaIndex, int8_t pgc1, int8_t pgc2, int8_t pgc3) override;
    bool setRssiS2dInitials(uint8_t auxAdcRes, float ioffsStep) override;
    bool setRssiS2dParams(const AntennaMask& antennaMask, uint8_t region, uint8_t gain, uint8_t offset) override;
    bool setRuParams(uint32_t userOne, uint32_t userTwo) override;
    bool setRxAggregationEnabled(bool enabled) override;
    bool setRxBandLut(const AntennaMask& antennaMask, const std::array<std::array<uint16_t, maxNumLnaSubBandsCrossingPoints>, maxNumRxAntennas>& rxBandLut, const std::array<uint8_t, maxNumRxAntennas>& firstTunningIndex) override;
    bool setSpacelessTxEnabled(bool enabled) override;
    bool setTransmitPowerControl(bool closedLoop, uint8_t powerLimit) override;
    bool setTransmitPowerLevel(uint8_t powerLevel) override;
    bool setTransmitPowerTableOffset(uint8_t antenna, Bandwidth bandwidth, int16_t offset) override;
    bool setTransmitPowerVector(TransmitPowerVector_t& transmitPowerVector) override;
    bool setTssiCalibrationData(const std::vector<TssiCalibrationData_t>& data) override;
    bool setTssiS2dParams(uint8_t antenna, uint8_t region, uint8_t powerThreshold, uint8_t gain, uint8_t offset) override;
    bool setXtalCalValue(uint16_t xtalValue) override;
    bool setXtalRegValue(uint16_t xtalValue) override;
    bool setZwdfsConfiguration(uint8_t zwdfsChannel, Bandwidth zwdfsBandwidth, Bandwidth radarDetectionBandwidth) override;
    bool startCalibration(const StartCalibrationParams_t& params, uint8_t& status) override;
    bool startCw(int8_t amplitude, int16_t tone) override;
    bool startRxCalibration() override;
    bool startTx(uint16_t repetitions, uint32_t packetLength, bool longData, bool beamforming, CodingType codingType = CodingType::CODING_TYPE_AUTO) override;
    bool startRxPer(uint32_t packetLimit) override;
    bool stopCw() override;
    bool stopRxCalibration() override;
    bool stopTx() override;
    bool stopRxPer(bool calcRxPer) override;
    bool validateBeamformingHeaderRegister(PhyMode expectedPhyMode, Bandwidth expectedBandwidth) override;
    bool writeCalibrationFile(NvMemoryType memoryType, NvMemorySize memorySize) override;
    bool writeMemory(ChipModule chipModule, size_t address, const uint8_t* data, size_t length) override;
    bool writeNvm(size_t address, const uint8_t* data, size_t length) override;
    bool writeRegister(ChipModule chipModule, size_t address, uint32_t mask, uint32_t value) override;

private:
#pragma warning(disable : 4251) // class needs to have dll-interface to be used by clients of class 'dut::DutImpl'
    std::shared_ptr<Client> m_client;
    std::shared_ptr<Logger> m_logger;
    bool m_throwExceptions = false;

    uint8_t m_wlanIndex = UINT8_MAX;

    std::shared_ptr<NonVolatileMemory> m_nvm;
    std::shared_ptr<Transmitter> m_transmitter;

    std::string m_lastError;
    bool m_initialized = false;

    std::shared_ptr<CalibrationFile> m_calibrationFile;

    std::unordered_map<VersionedComponent, std::string> m_componentVersions;

    std::shared_ptr<Status> m_status;

    // Initialized in setRssiS2dInitials. Used in calculateOptimalS2dGains() and calculateOptimalS2dOffsets()
    struct RssiS2dParams {
        float vmax; // = 2^AuxADC_Res;    // Vmax = 512
        float vmid; // = Vmax/2;          // Vmid = 256
        int32_t m; // = 1/Ioffs_Step;       // M = 16
        int32_t vp;
    } m_rssiS2dParams {};

    uint8_t m_lastSubBandIndex = 0;
    size_t m_lnaFrequencyIndex = ULONG_MAX;
    std::array<std::array<std::array<int32_t, maxNumRxAntennas>, maxNumLnaSubBands>, maxNumLnaFrequencies> m_lnaMeasureResult {};
    std::array<uint16_t, maxNumLnaSubBands> m_freqTableForLnaSubBand {};

    static std::string inputToString(const std::vector<std::pair<std::string, std::string>>& parameters);
    static std::string outputToString(const std::vector<std::pair<std::string, std::string>>& results);

    void logInput(const std::vector<std::pair<std::string, std::string>>& parameters) const;
    void logOutput(const std::vector<std::pair<std::string, std::string>>& results) const;

    bool execute(const std::string& functionName, const std::function<void()>& functionCode);

    void addVap(bool snifferMode, uint8_t fwInfoOperationMode) const;
    void removeVap() const;

    void setEnabledRxAntennas(const AntennaMask& antennaMask) const;
    void setEnabledTxAntennas(const AntennaMask& antennaMask) const;
    void setEnabledZwdfsAntenna(uint8_t channel, Bandwidth bandwidth, Bandwidth radarDetectionBandwidth, const AntennaMask& antennaMask) const;
    void setTpcAntennaParams(uint8_t channel) const;
    void writeRssiCalDataToFw(const AntennaMask& antennaMask) const;

    void setEnabledRxAntennaMaskImpl(const AntennaMask& antennaMask) const;
    void setEnabledTxAntennaMaskImpl(const AntennaMask& antennaMask) const;
    void setEnabledZwdfsAntennaMaskImpl(const AntennaMask& antennaMask) const;

    void startRxCalibrationImpl() const;
    void stopRxCalibrationImpl() const;

    void validateBeamformingHeaderRegisterImpl(PhyMode expectedPhyMode, Bandwidth expectedBandwidth) const;

    // Returns true if Wave700 and ZWDFS antenna mask is not 0x00
    bool isZwdfsAvailable() const;
#pragma warning(default : 4251)
};

class DutImplFactory : public DutFactory {
public:
    std::unique_ptr<Dut> createInstance(uint8_t wlanIndex, std::shared_ptr<Connection> connection, std::shared_ptr<Logger> logger, bool throwExceptions) override
    {
        return std::make_unique<DutImpl>(wlanIndex, connection, logger, throwExceptions);
    }
};

}

#endif
