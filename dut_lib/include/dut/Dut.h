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

#ifndef _DUT_DUT_H_
#define _DUT_DUT_H_

#include "dut/API.h"

#include "dut/AntennaMask.h"
#include "dut/Connection.h"
#include "dut/Logger.h"
#include "dut/Types.h"

#include <array>
#include <memory>
#include <string>
#include <vector>

namespace dut {

constexpr uint32_t defaultOfflineCalMask = 0xFFFF;
constexpr uint32_t defaultOnlineCalMask = 0x0;
constexpr uint32_t defaultIfs = 16;
constexpr RegulationType defaultRegulationType = RegulationType::REGULATION_TYPE_FCC_SP;
constexpr uint8_t defaultTransmitPowerLevel = 34;
constexpr bool defaultRxAggregationEnabled = false;
constexpr bool defaultClipperEnabled = true;
constexpr bool defaultSpacelessTxEnabled = false;
constexpr bool defaultClosedLoopEnabled = false;
constexpr uint8_t defaultZwdfsChannel = 100;
constexpr Bandwidth defaultZwdfsBandwidth = Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY;
constexpr Bandwidth defaultRadarDetectionBandwidth = Bandwidth::BANDWIDTH_INVALID;

/**
 * Device Under Test (DUT).
 * 
 * API for HW and SW debugging and calibration of WiFi cards.
 */
class DUT_API Dut {
public:
    /**
     * @brief Class destructor
     */
    virtual ~Dut() = default;

    /**
     * @brief Gets the version of the DUT library.
     * 
     * @return String containing the library version. Example: "2.6.5"
     */
    static std::string getVersion();

    /**
 * @brief Gets the tag of the branch in the `dut` repository
 *
 * The tag is usually appended to the version of the DUT library to build a full version string.
 *
 * @return String containing the build tag. Example: "r167"
 */
    static std::string getSharedHeaderTag();

    /**
     * @brief Gets the tag of the branch in the `dut` repository
     *
     * The tag is usually appended to the version of the DUT library to build a full version string.
     * 
     * @return String containing the build tag. Example: "r167"
     */
    static std::string getBuildTag();

    /**
     * @brief Gets the description of the error occurred during the last API function call, if any.
     * 
     * @return Description of the last error occurred or empty if there was no error. Examples: 
     * "Not initialized", "Channel not set"
     */
    virtual std::string getLastError() const = 0;

    /**
     * @brief Sets the throw-exception-on-error behavior.
     * 
     * @param throwExceptions If set true, errors in API function calls are reported by throwing 
     * an exception. If false, no exception is thrown and API function calls return false on error. 
     * The error cause can be obtained by calling getLastError(). 
     */
    virtual void setThrowExceptions(bool throwExceptions) = 0;

    /**
     * @brief Obtains optimum S2D gain and offset for the given region.
     * 
     * @param lowerPowerLevel Power index lower level (Example: 0).
     * @param upperPowerLevel Power index upper level (Example: powerThreshold - 1).
     * @param tssiLow Minimum PA detector feedback used in calculation (Example: 60).
     * @param tssiHigh Maximum PA detector feedback used in calculation (Example: 487).
     * @param region Region number (1, 2, or 3, i.e.: from 1 to maximum number of regions minus 1).
     * @param powerThreshold Power index threshold of the region.
     * @param[out] gain Calculated optimum S2D gain.
     * @param[out] offset Calculated optimum S2D offset.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool calibrateTssiS2d(uint8_t lowerPowerLevel, uint8_t upperPowerLevel, uint32_t tssiLow, uint32_t tssiHigh, uint8_t region, uint8_t powerThreshold, uint8_t& gain, uint8_t& offset) = 0;

    /**
     * @brief Performs linear regression on the specified data points (*P[dBm],**V[dec]).
     * 
     * @param powerVector Vector of K power levels at RFIC LNA output, obtained with function 
     * calculateRxPowerVector() during RX gain accuracy calibration. Values in -32:0.25:32 dBm
     * @param voltages Vector of K decimal values at AUX ADC output corresponding to K input 
     * power levels, per RX antenna. Values in 0:511
     * @param offsets DC offset at no signal condition measured at AUX ADC output, per RX antenna.
     * Values in 0:511
     * @param[out] lnaGains The calibrated LNA gain values, obtained with calculateRxLnaGains() 
     * during RX gain accuracy calibration, i.e.: LNA gain step configuration for RSSI a/b 
     * calculation, per RX antenna. 
     * @param[out] result A calculated (a, b) linear regression slope and offset parameters, per 
     * RX antenna.
     * @param[out] maxErrors Maximum (a, b) error, per RX antenna.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool calculateLinearRegression(const std::array<float, numPowerPoints>& powerVector, const std::array<std::array<uint16_t, numPowerPoints>, maxNumRxAntennas>& voltages, const std::array<uint16_t, maxNumRxAntennas>& offsets, const std::array<float, maxNumRxAntennas>& lnaGains, std::array<RssiAB_t, maxNumRxAntennas>& result, std::array<float, maxNumRxAntennas>& maxErrors) = 0;

    /**
     * @brief Calculates the optimum S2D gain index for a certain RSSI detector region based, per 
     * RX antenna.
     * 
     * @param rssiVoltage1 AUX ADC reading for the region Pmin power, per RX antenna. Values in 0:511
     * @param rssiVoltage2 AUX ADC reading for the region Pmax power, per RX antenna. Values in 0:511
     * @param regionS2dGain S2D Gain initial value. 
     * @param[out] optimalS2dGains Calculated optimum S2D gain index, per RX antenna. 
     * 
     * @return true on success and false otherwise.
     */
    virtual bool calculateOptimalS2dGains(const std::array<uint16_t, maxNumRxAntennas>& rssiVoltage1, const std::array<uint16_t, maxNumRxAntennas>& rssiVoltage2, int32_t regionS2dGain, std::array<int32_t, maxNumRxAntennas>& optimalS2dGains) = 0;

    /**
     * @brief Calculates the optimum S2D I Offset for a certain RSSI detector region based, per RX 
     * antenna.
     * 
     * @param rssiVoltage1 AUX ADC reading for the region Pmin power, per RX antenna. Values in 0:511
     * @param rssiVoltage2 AUX ADC reading for the region Pmax power, per RX antenna. Values in 0:511
     * @param regionS2dGain S2D Gain initial value. 
     * @param[out] optimalS2dGains Calculated optimum S2D gain index, per RX antenna. Values in 0:31
     * 
     * @return true on success and false otherwise.
     */
    virtual bool calculateOptimalS2dOffsets(const std::array<uint16_t, maxNumRxAntennas>& rssiVoltage1, const std::array<uint16_t, maxNumRxAntennas>& rssiVoltage2, int32_t regionS2dGain, uint8_t regionS2dOffset, std::array<uint8_t, maxNumRxAntennas>& optimalS2dOffsets) = 0;

    /**
     * @brief Computes the maximum allowed data packet length for the specified PHY mode.
     * 
     * The maximum value for all the standards from VHT (11ac) and on is based on maximum packet 
     * time length of ~5.4ms.
     * 
     * @param phyMode IEEE Standard (that is, PHY mode: HT, VHT, HE, EHT). 
     * @param[out] maxPacketLength Packet length in bytes.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool calculateMaxPacketLength(PhyMode phyMode, uint32_t& maxPacketLength) = 0;

    /**
     * @brief Computes the data packet length for the specified configuration and number of symbols.
     * 
     * @param phyMode IEEE Standard (that is, PHY mode: HT, VHT, HE, EHT). 
     * @param signalBandwidth Channel width.
     * @param mcs Modulation Coding Scheme. 
     * @param nss Number of Spatial Streams, i.e.: number of unique MIMO data streams that can be 
     * sent in parallel (valid values range from 1 to the maximum number of TX antennas).
     * @param numSymbols Number of OFDM data symbols.
     * @param[out] packetLength Packet length in bytes.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool calculatePacketLength(PhyMode phyMode, Bandwidth signalBandwidth, Mcs mcs, uint8_t nss, uint32_t numSymbols, uint32_t& packetLength) = 0;

    /**
     * @brief Computes the PHY data rate for the specified parameters.
     * See https://mcsindex.com/
     * 
     * @param phyMode IEEE Standard (that is, PHY mode: HT, VHT, HE, EHT). 
     * @param signalBandwidth Channel width.
     * @param mcs Modulation Coding Scheme. 
     * @param nss Number of Spatial Streams, i.e.: number of unique MIMO data streams that can be sent 
     * in parallel (valid values range from 1 to the maximum number of TX antennas).
     * @param gi Guard Interval code. Valid values for 11ax are 0.8 us, 1.6 us and 3.2 us; valid values 
     * prior to 11ax - except 11b - are 0.8 us and 0.4 us.
     * @param[out] rateMbps PHY data rate in Mbps.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool calculatePhyDataRate(PhyMode phyMode, Bandwidth signalBandwidth, Mcs mcs, uint8_t nss, Gi gi, float& rateMbps) = 0;

    /**
     * @brief Gets the measured data and finds the optimum control word and corresponding gain for the LNA mid gain.
     * 
     * @param targetGains Target gain in dB for the LNA mid step, per RX antenna. 
     * @param prxin RX input power at the antenna, per RX antenna. Range: 8 bit signed, -128:128 dBm
     * @param numSamples Number of samples to be used in the accumulator mode. Example: 1024. 
     * @param pgc1 Programmable Gain Control 1 value in dB. Values in 0:1:18 dB plus extra gain step of 24 dB.
     * @param pgc2 Programmable Gain Control 2 value in dB. Values in -8:2:12 dB plus extra gain step of 18 dB.
     * @param[out] calcGain LNA mid gain value, per RX antenna. Values in -64:0.5:64 dB.
     * @param[out] calcGainCtrl Tuning control word for the target mid LNA gain, per RX antenna. Values in 0:1:15
     * 
     * @return true on success and false otherwise.
     */
    virtual bool calibrateRxLnaMidGains(const std::array<float, maxNumRxAntennas>& targetGains, const std::array<float, maxNumRxAntennas>& prxin, uint16_t numSamples, int8_t pgc1, int8_t pgc2, std::array<float, maxNumRxAntennas>& calcGain, std::array<uint8_t, maxNumRxAntennas>& calcGainCtrl) = 0;

    /**
     * @brief Calculates the LNA gains per RX antenna from the specified correlation results.
     * 
     * @param correlationResults Correlation results, per RX antenna. 
     * @param prxin RX input power at the antenna, per RX antenna. Range: 8 bit signed, -128:128 dBm
     * @param pgc1 Programmable Gain Control 1 value in dB. Values in 0:1:18 dB plus extra gain step of 24 dB.
     * @param pgc2 Programmable Gain Control 2 value in dB. Values in -8:2:12 dB plus extra gain step of 18 dB.
     * @param pgc3 Programmable Gain Control 3 value in dB. Set to 0 dB.
     * @param[out] gainsGain value in dB, per RX antenna. Values in -64:0.5:64 dB
     * 
     * @return true on success and false otherwise.
     */
    virtual bool calculateRxLnaGains(const std::array<CorrelationResults_t, maxNumRxAntennas>& correlationResults, const std::array<float, maxNumRxAntennas>& prxin, int8_t pgc1, int8_t pgc2, int8_t pgc3, std::array<float, maxNumRxAntennas>& gains) = 0;

    /**
     * @brief Calculates the frequency crossing points between each pair of adjacent inverted 
     * parabolas and returns the resulting vector.
     * 
     * @param[out] freqResultTable Vector of frequency crossing points, per RX antenna. Values 
     * from 4900 MHz in 5 MHz steps.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool calculateRxLnaSubBandGains(std::array<std::array<uint16_t, maxNumLnaSubBands>, maxNumRxAntennas>& freqResultTable) = 0;

    /**
     * @brief Calculates the AUX ADC input power vector per region to perform the RSSI curve 
     * approximation.
     * 
     * @param maxPower Maximum power for region X at the AUX ADC input.
     * @param lnaGains LNA gain step configuration for RSSI a/b calculation, per RX antenna, 
     * obtained with calculateRxLnaGains() during RX gain accuracy calibration.
     * @param lowerPowerBoundary Power back off for region x lower limit.
     * @param upperPowerBoundary Power back off for region x upper limit.
     * @param[out] powerVector Calculated power vector for the region.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool calculateRxPowerVector(int32_t maxPower, const std::array<float, maxNumRxAntennas>& lnaGains, int32_t lowerPowerBoundary, int32_t upperPowerBoundary, std::array<float, numPowerPoints>& powerVector) = 0;

    /**
     * @brief Calculates the RX RF flatness from the measured power levels over frequency.
     * 
     * @param P_Adc_array Vector of ADC measured power values at the frequency calibration points, 
     * per RX antenna.
     * @param refGain RX gain at the reference frequency with RFIC LNA at mid gain (calibrated 
     * values), per RX antenna.
     * @param pgc1 Programmable Gain Control 1 value in dB. Values in 0:1:18 dB plus extra gain step of 24 dB.
     * @param pgc2 Programmable Gain Control 2 value in dB. Values in -8:2:12 dB plus extra gain step of 18 dB.
     * @param pgc3 Programmable Gain Control 3 value in dB. Set to 0 dB.
     * @param pin RX input power at the RX antenna per each calibrated frequency, per RX antenna. 
     * Range: 8 bit signed, -128:128 dBm
     * @param[out] results Vector with the deltas [dB] between the gain at calibration frequency 
     * and the reference gain, per RX antenna. Values in -4:0.5:3.5 dB
     * 
     * @return true on success and false otherwise.
     */
    virtual bool calculateRxRfFlatness(const std::array<std::array<int32_t, maxNumLnaSubBands>, maxNumRxAntennas>& P_Adc_array, const std::array<float, maxNumRxAntennas>& refGain, int8_t pgc1, int8_t pgc2, int8_t pgc3, const std::array<std::array<float, maxNumLnaSubBands>, maxNumRxAntennas>& pin, std::array<std::array<float, maxNumLnaSubBands>, maxNumRxAntennas>& results) = 0;

    /**
     * @brief Calculates the TX AB coefficients using linear regression on the specified data points.
     * 
     * @param antennaMask.
     * @param numRegions.
     * @param pointsPerAntennaAndRegion.
     * @param nLog.
     * @param[out] coefficientsPerAntennaAndRegion. 
     * @param[out] errorsPerAntennaAndRegion
     * 
     * @return true on success and false otherwise.
     */
    virtual bool calculateTxABCoefficients(const AntennaMask& antennaMask,
        uint8_t numRegions,
        const std::array<std::array<std::vector<Point_t>, maxNumTxRegions>, maxNumTxAntennas>& pointsPerAntennaAndRegion,
        float nLog,
        std::array<std::array<TssiAB_t, maxNumTxRegions>, maxNumTxAntennas>& coefficientsPerAntennaAndRegion,
        std::array<std::array<float, maxNumTxRegions>, maxNumTxAntennas>& errorsPerAntennaAndRegion)
        = 0;

    /**
     * @brief Initializes the DUT driver.
     * 
     * This function must be called prior to any other function that interacts with the DUT.
     * 
     * @param snifferMode Sniffer mode. Not used, set to false.
     * @param memoryType The type of the non-volatile memory to read the calibration data from, 
     * either EEPROM or flash.
     * @param memorySize The size of the non-volatile memory to read the calibration data from, 
     * either 1024 or 2048 bytes.
     * @param band RF band, either 2.4, 5 or 6 GHz. Specify Band::BAND_INVALID to use default band.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool driverInit(bool snifferMode, NvMemoryType memoryType, NvMemorySize memorySize, Band band) = 0;

    /**
     * @brief Releases the DUT driver.
     * 
     * This function must be called when interaction with the DUT has finished to release any 
     * allocated resources and stop any started transmission.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool driverRelease() = 0;

    /**
     * @brief Flushes the calibration data to the non-volatile memory.
     * 
     * This function returns immediately but the driver runs a recovery process in the DUT to 
     * start the firmware with the new calibration data. So, after calling this function, 
     * allow at least 10s for the recovery process to complete, release the driver and make a 
     * new connection to the DUT to start afresh.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool flushNvm() = 0;

    /**
     * @brief Gets the RX antennas available in the hardware (according to the PSD).
     * 
     * @param[out] antennaMask Bitmask with one bit per antenna, set to 1 if the antenna is 
     * available and to 0 otherwise. Bit 0 corresponds to the first antenna.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getAvailableRxAntennaMask(AntennaMask& antennaMask) = 0;

    /**
     * @brief Gets the TX antennas available in the hardware (according to the PSD).
     * 
     * @param[out] antennaMask Bitmask with one bit per antenna, set to 1 if the antenna is 
     * available and to 0 otherwise. Bit 0 corresponds to the first antenna.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getAvailableTxAntennaMask(AntennaMask& antennaMask) = 0;

    /**
     * @brief Gets the current operating RF band.
     * 
     * @param[out] band Current operating RF band, either 2.4, 5 or 6 GHz.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getBand(Band& band) = 0;

    /**
     * @brief Gets the RF bands supported by the DUT.
     * 
     * @param[out] supportedBands List of supported RF bands.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getSupportedBands(std::vector<Band>& supportedBands) = 0;

    /**
     * @brief Gets the values for the CDD (Cyclic Delay Diversity) registers in the BB chip.
     * 
     * @param numTxAntennas List of supported RF bands.
     * @param[out] offset1 Value of the first register.
     * @param[out] offset2 Value of the second register.
     * @param[out] offset3 Value of the third register.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getBbicCddValues(uint8_t numTxAntennas, uint32_t& offset1, uint32_t& offset2, uint32_t& offset3) = 0;

    /**
     * @brief Gets the version and subversion numbers of the calibration file.
     * 
     * Version number is stored in byte at index 40 (0-based) of the calibration file header.
     * Subversion number is stored in CardInfo CIS (0x60) at offset 3. 
     * 
     * @param[out] version Calibration file version. 
     * @param[out] subversion Calibration file subversion. 
     *
     * @return true on success and false otherwise.
     */
    virtual bool getCalibrationFileVersion(CalibrationFileVersion& version, CalibrationFileSubversion& subversion) = 0;

    /**
     * @brief Gets the card information from the calibration file stored in the non-volatile memory.
     * 
     * @param[out] countryCode The WiFi Radio designated country code (affects the regulatory 
     * domain restrictions).
     * @param[out] macAddress MAC address.
     * @param[out] serialNumber Serial number.
     * @param[out] week Manufacturing week.
     * @param[out] year Manufacturing year.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getCardInfo(uint8_t& countryCode, std::array<uint8_t, cardInfoMacAddressSize>& macAddress, std::array<uint8_t, cardInfoSerialNumberSize>& serialNumber, uint8_t& week, uint8_t& year) = 0;

    /**
     * @brief Gets the BB chip identifier.
     * 
     * @param[out] chipId BB chip identifier.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getChipId(ChipID& chipId) = 0;

    /**
     * @brief Gets the version of the specified component.
     * 
     * @param component Versioned component. Example: CV
     * @param[out] version Version of the component. Example: "06.01.00.1613"
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getComponentVersion(VersionedComponent component, std::string& version) = 0;

    /**
     * @brief Gets the status (enabled / disabled) of the RX antennas.
     * 
     * @param[out] antennaMask Bitmask with one bit per antenna, set to 1 if the antenna is 
     * enabled and to 0 otherwise. Bit 0 corresponds to the first antenna.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getEnabledRxAntennaMask(AntennaMask& antennaMask) = 0;

    /**
     * @brief Gets the status (enabled / disabled) of the TX antennas.
     * 
     * @param[out] antennaMask Bitmask with one bit per antenna, set to 1 if the antenna is 
     * enabled and to 0 otherwise. Bit 0 corresponds to the first antenna.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getEnabledTxAntennaMask(AntennaMask& antennaMask) = 0;

    /**
     * @brief Gets the type of FEM (Front End Module) used by the hardware.
     * 
     * Currently, there are just two types of FEM available: linear and non-linear.
     * 
     * @param[out] femType Type of FEM. Examples: FEM_TYPE_LINEAR, FEM_TYPE_NON_LINEAR
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getFemType(FemType& femType) = 0;

    /**
     * @brief Gets the type of hardware.
     * 
     * @param[out] hardwareType Type of hardware. Examples: GEN6, GEN7
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getHardwareType(HardwareType& hardwareType) = 0;

    /**
     * @brief Gets the in-band RSSI values for each RX antenna.
     * 
     * @param[out] rssi RSSI values, per RX antenna.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getInbandRssi(std::array<int16_t, maxNumRxAntennas>& rssi) = 0;

    /**
     * @brief Gets the values for the packet counters at the MAC level.
     * 
     * @param[out] receivedPackets Number of packets received.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getMacPacketCounters(uint32_t& receivedPackets) = 0;

    /**
     * @brief Gets the values for the packet counters at the MPDU level.
     * 
     * PER (Packet Error Rate) percentage is computed as (errorPackets / receivedPackets) * 100.0.
     * 
     * @param[out] receivedPackets Total number of packets received (with and without error).
     * @param[out] errorPackets Number of packets with error.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getMpduPacketCounters(uint32_t& receivedPackets, uint32_t& errorPackets) = 0;

    /**
     * @brief Gets the NMSE values currently measured per calibration point and antenna.
     *
     * @param[out] values NMSE values per tx antennas in table disposition [Antennas][CalibrationPoints]
     *
     * @return true on success and false otherwise.
     */
    virtual bool getNmseValues(std::array<std::array<int32_t, dut::dpdTotalCalibrationPoints>, dut::maxNumTxAntennas>& values) = 0;

    /**
     * @brief Gets the size of the non-volatile memory.
     * 
     * @param[out] size The size of the non-volatile memory in bytes (either 1024 or 2048 bytes). 
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getNvmSize(size_t& size) = 0;

    /**
     * @brief Gets the type of the non-volatile memory.
     * 
     * @param[out] type The type of the non-volatile memory (either EEPROM or flash). 
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getNvmType(NvMemoryType& type) = 0;

    /**
     * @brief Gets the version of the calibration file stored in non-volatile memory.
     * 
     * @param[out] version The version of the calibration file. Examples: 6, 7. 
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getNvmVersion(uint8_t& version) = 0;

    /**
     * @brief Gets the currently selected PHY mode.
     * 
     * The PHY mode is changed when setting the channel.
     * 
     * @param[out] phyMode IEEE Standard (that is, PHY mode: HT, VHT, HE, EHT).
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getPhyMode(PhyMode& phyMode) = 0;

    /**
     * @brief Gets the values for the packet counters at the PHY level.
     * 
     * PER (Packet Error Rate) percentage is computed as (crcErrors / receivedPackets) * 100.0.
     * 
     * @param[out] receivedPackets Total number of packets received (with and without CRC error).
     * @param[out] crcErrors Number of packets with CRC error.
     * @param[out] forwardedPackets Number of packets forwarded.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getPhyPacketCounters(uint32_t& receivedPackets, uint32_t& crcErrors, uint32_t& forwardedPackets) = 0;

    /**
     * @brief Gets the value of the production flag. 
     * 
     * The production flag is included in the header of the calibration file stored in the 
     * non-volatile memory.
     * 
     * @param[out] productionFlag true if production flag is set (production mode) and false 
     * otherwise (operational mode).
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getProductionFlag(bool& productionFlag) = 0;

    /**
     * @brief Gets the RF RSSI power for each RX antenna.
     *
     * @param[out] power RF RSSI power values in dB, per RX antenna. 
     *
     * @return true on success and false otherwise.
     */
    virtual bool getRfRssiPower(std::array<int8_t, maxNumRxAntennas>& power) = 0;

    /**
     * @brief Gets the RSSI values for each RX antenna.
     * 
     * @param method Defines the PHY measurement block to use to get the RSSI values. If method is 
     * 0, measures the average power at AUX ADC input by using PHY measurement block (accumulator).
     * If method is 1, reads the PHY RSSI output.
     * @param numSamples Number of samples to be used in the accumulator method. Example: 1024
     * @param[out] rssi RSSI values, per RX antenna. Values in 0:511. RSSI is integer shifted left by 4!
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getRssi(uint8_t method, uint16_t numSamples, std::array<uint16_t, maxNumRxAntennas>& rssi) = 0;

    /**
     * @brief Gets the RX calibration data from the calibration file stored in the non-volatile memory.
     * 
     * @param[out] version The version of the calibration file. Example: VERSION_7
     * @param[out] data The calibration data (output array contains one item only).
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getRssiCalibrationData(CalibrationFileVersion& version, std::vector<RssiCalibrationData_t>& data) = 0;

    /**
     * @brief Gets the CW power at RXIQ ADC input by using PHY measurement blocks.
     * 
     * @param method Defines the PHY measurement block to use: 0 = accumulator, 1 = Goertzel, 
     * 2 = TD (Time Domain) capture, 3 = OLA
     * @param freqOffset CW BB frequency after down conversion; must be provided with Goertzel 
     * method, optional for OLA and TD capture and not required for the accumulator.
     * @param numSamples Number of samples to be used with the accumulator and TD capture methods;
     * or number of cycles (averaging) to be used with the Goertzel and OLA methods. Example: 1024
     * @param[out] correlationResults Correlation results: 
     * - Power level at "I" input of the RXIQ ADC [dBm].
     * - Power level at "Q" input of the RXIQ ADC [dBm].
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getRxCwPower(uint8_t method, int32_t freqOffset, uint16_t numSamples, std::array<CorrelationResults_t, maxNumRxAntennas>& correlationResults) = 0;

    /**
     * @brief Gets the EVM for each of the RX antennas.
     * 
     * @param[out] rxEvm EVM value, per RX antenna.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getRxEvm(std::array<uint8_t, maxNumRxAntennas>& rxEvm) = 0;

    /**
     * @brief Gets the current RX rate information (MCS and NSS).
     *
     * @param[out] mcs Modulation Coding Scheme. 
     * @param[out] nss Number of Spatial Streams.
     * @return true on success and false otherwise.
     */
    virtual bool getRxRateInfo(uint8_t& mcs, uint8_t& nss) = 0;

    /**
     * @brief Gets the current temperature of the RF chip.
     * 
     * @param[out] temperature Chip temperature in degrees Celsius (oC).
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getTemperature(float& temperature) = 0;

    /**
     * @brief Gets the value of the offset register for the gain table alignment.
     *
     * @param antenna The index of the TX antenna. Valid values range from 0 to the maximum number 
     * of TX antennas minus 1.
     * @param bandwidth Channel width.
     * @param[out] offset The transmit power table offset value. Values in -128:128
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getTransmitPowerTableOffset(uint8_t antenna, Bandwidth bandwidth, int16_t& offset) = 0;

    /**
     * @brief Gets the TSSI value for each TX antenna.
     * 
     * The TSSI is the voltage read by detector of the output signal at the PA.
     * 
     * @param[out] voltages The voltage value in mV, per TX antenna.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getTransmitVoltages(std::array<uint32_t, maxNumTxAntennas>& voltages) = 0;

    /**
     * @brief Gets the TX calibration data from the calibration file stored in the non-volatile memory.
     * 
     * @param[out] version The version of the calibration file. Example: VERSION_7
     * @param[out] data The calibration data (output array contains one item per calibrated channel).
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getTssiCalibrationData(CalibrationFileVersion& version, std::vector<TssiCalibrationData_t>& data) = 0;

    /**
     * @brief Gets the XTAL calibration value from the calibration file stored in the non-volatile memory.
     * 
     * @param[out] xtalValue The XTAL calibration value (= XTAL Value + XTAL Bias << 9) used for 
     * the internal clock.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getXtalCalValue(uint16_t& xtalValue) = 0;

    /**
     * @brief Gets the XTAL calibration value from the RFIC register.
     * 
     * @param[out] xtalValue The XTAL calibration value (= XTAL Value + XTAL Bias << 9) used for 
     * the internal clock.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getXtalRegValue(uint16_t& xtalValue) = 0;

    /**
     * @brief Gets the ZWDFS status.
     *
     * ZWDFS stands for Zero-Wait Dynamic Frequency Selection.
     * 
     * Returns which antennas are available in the hardware for ZWDFS and if any, whether ZWDFS is 
     * enabled or not.
     * 
     * @param[out] antennaMask Bitmask with one bit per antenna, set to 1 if the antenna is 
     * used for ZWDFS and to 0 otherwise. Bit 0 corresponds to the first antenna.
     * @param[out] enabled true if ZWDFS is enabled and false otherwise.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool getZwdfsStatus(AntennaMask& antennaMask, bool& enabled) = 0;

    /**
     * @brief Loads beamforming matrix from file set(s) to hardware.
     *
     * Supports both single and dual-segment beamforming operations:
     * - Single segment: Only primarySet is used (standard 2-file or EHT 3-file operation)
     * - Dual segment: Both primarySet and secondarySet are used (EHT 320MHz dual-segment operation)
     *
     * It must be run after setChannel() and setRate(), it cannot be run during an ongoing transmission.
     * If a transmission is ongoing, it must be stopped first by calling stopTx().
     * 
     * The function is band-agnostic and determines the appropriate hardware addresses based on the
     * beamforming data content and hardware configuration. Primary data uses standard addresses,
     * while secondary data (for EHT 320MHz) uses upper segment addresses automatically.
     *
     * The reason it is band-agnostic is because the HW translates the addresses based on the
     * the band we connect to.
     *
     * @param primarySet Primary file set containing header, values, and optional extended EHT values
     * @param secondarySet Optional secondary file set for EHT 320MHz dual-segment operations (empty if not used)
     * @return true on success and false otherwise.
     */
    virtual bool loadBeamformingMatrixFromFileSet(
        const BeamformingFilePathSet_t& primarySet,
        const BeamformingFilePathSet_t& secondarySet = BeamformingFilePathSet_t {})
        = 0;

    /**
     * @brief Loads calibration file from the specified file into non-volatile memory.
     * 
     * @param fileName Name of the file containing the calibration file.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool loadNvmFromFile(const std::string& fileName) = 0;

    /**
     * @brief Sweeps over 3-bit LNA sub-band control range and measures corresponding RX power.
     * 
     * The resulting vector of the RX powers per each of the RX antennas is cached internally. 
     * Function startRxCalibration() must have been called before calling this function.
     * Then this function must be called for each of the sub-band frequencies and eventually, 
     * function calculateRxLnaSubBandGains() must be called to get the results.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool measureRxLnaSubBandGains() = 0;

    /**
     * @brief Reads a block of memory from the specified module.
     * 
     * @param chipModule The chip module to read data from.
     * @param address Starting address.
     * @param data Destination buffer.
     * @param length Number of bytes to read.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool readMemory(ChipModule chipModule, size_t address, uint8_t* data, size_t length) = 0;

    /**
     * @brief Reads a block of memory from the non-volatile memory.
     * 
     * @param address Starting address.
     * @param data Destination buffer.
     * @param length Number of bytes to read.
     * @param useCache true to return cached data and false to fetch data from device.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool readNvm(size_t address, uint8_t* data, size_t length, bool useCache = true) = 0;

    /**
     * @brief Reads a register from the specified module.
     * 
     * @param chipModule The chip module to read data from.
     * @param address Register address.
     * @param mask Bits of interest.
     * @param[out] value Value read (4 bytes starting from the specified address).
     * 
     * @return true on success and false otherwise.
     */
    virtual bool readRegister(ChipModule chipModule, size_t address, uint32_t mask, uint32_t& value) = 0;

    /**
     * @brief Resets the counters for packets received at MAC layer.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool resetMacPacketCounters() = 0;

    /**
     * @brief Resets the counters for packets received at MPDU layer.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool resetMpduPacketCounters() = 0;

    /**
     * @brief Resets the counters for packets received at PHY layer.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool resetPhyPacketCounters() = 0;

    /**
     * @brief Set Rx Measure params.
     *
     * @param numCaptures Number of Rx measure captures.
     * @param captureInterval Interval between each capture.
     * @param disabled Boolean to disable the functionality.
     *
     * @return true on success and false otherwise.
     */
    virtual bool rxMeasure(uint32_t numCaptures, uint32_t captureInterval, bool disabled) = 0;

    /**
     * @brief Saves calibration file in non-volatile memory to the specified file.
     * 
     * @param fileName Name of the destination file.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool saveNvmToFile(const std::string& fileName) = 0;

    /**
     * @brief Sets the values for the CDD (Cyclic Delay Diversity) registers in the BB chip.
     * 
     * CDD is a kind of transmit diversity mechanism implemented by applying a different phase 
     * delay (cyclic phase delay) for each OFDM subcarrier. It is used in spatial multiplexing to 
     * increase diversity between the 2 spatial paths. 
     * Very simply put, in CDD one antenna is transmitting the original copy of data and the other 
     * antenna is transmitting the cyclic shifted version of the original data.
     * 
     * This function allows selecting the number of antennas to synchronize and the phase shift to 
     * apply. Offset 1/2/3 stand for CDD register values according to the number of antennas. If 
     * less than 4 antennas are selected, for example 2, the offset 2 and offset 3 fields will be 
     * ignored.
     * 
     * @param numTxAntennas Number of TX antennas.
     * @param offset1 Value for the first register.
     * @param offset2 Value for the second register.
     * @param offset3 Value for the third register.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setBbicCddValues(uint8_t numTxAntennas, uint32_t offset1, uint32_t offset2, uint32_t offset3) = 0;

    /**
     * @brief Sets the version and subversion numbers of the calibration file.
     * 
     * Version number is stored in byte at index 40 (0-based) of the calibration file header.
     * Subversion number is stored in CardInfo CIS (0x60) at offset 3. 
     * 
     * @param version Calibration file version. 
     * @param subversion Calibration file subversion. 
     * @return true on success and false otherwise.
     */
    virtual bool setCalibrationFileVersion(CalibrationFileVersion version, CalibrationFileSubversion subversion) = 0;

    /**
     * @brief Sets the card information to the calibration file stored in the non-volatile memory.
     * 
     * @param countryCode The WiFi Radio designated country code (affects the regulatory domain 
     * restrictions).
     * @param macAddress MAC address.
     * @param serialNumber Serial number.
     * @param week Manufacturing week.
     * @param year Manufacturing year.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setCardInfo(uint8_t countryCode, const std::array<uint8_t, cardInfoMacAddressSize>& macAddress, const std::array<uint8_t, cardInfoSerialNumberSize>& serialNumber, uint8_t week, uint8_t year) = 0;

    /**
     * @brief Sets channel number.
     * See https://en.wikipedia.org/wiki/List_of_WLAN_channels
     * 
     * @param phyMode IEEE Standard (that is, PHY mode: HT, VHT, HE). 
     * @param spectrumBandwidth Channel width.
     * @param lowestChannel Lowest channel number. 
     * @param primaryChannelIndex Primary channel index.
     * @param regulationType Type of regulation.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setChannel(PhyMode phyMode, Bandwidth spectrumBandwidth, uint8_t lowestChannel, uint8_t primaryChannelIndex, RegulationType regulationType) = 0;

    /**
     * @brief Sets the status (enabled / disabled) of the clipper operation. During calibration we want to have clipper disabled.
     *
     * @param enabled true to enable the option and false otherwise.
     *
     * @return true on success and false otherwise.
     */
    virtual bool setClipper(bool enabled) = 0;

    /**
     * @brief Sets the status (enabled / disabled) of the RX antennas.
     * 
     * @param antennaMask Bitmask with one bit per antenna, set to 1 to enable the antenna and to 
     * 0 to disable it. Bit 0 corresponds to the first antenna.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setEnabledRxAntennaMask(const AntennaMask& antennaMask) = 0;

    /**
     * @brief Sets the status (enabled / disabled) of the TX antennas.
     * 
     * @param antennaMask Bitmask with one bit per antenna, set to 1 to enable the antenna and to 
     * 0 to disable it. Bit 0 corresponds to the first antenna.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setEnabledTxAntennaMask(const AntennaMask& antennaMask) = 0;

    /**
     * @brief Sets the HDK calibration masks configuration.
     * 
     * Offline calibration mask is used to specify which calibrations must be performed during the 
     * initialization of normal operation mode. Set to 0xffff to do a full calibration.
     * Online calibration mask specifies the calibrations to run during normal operation mode, when
     * the chip temperature variations are above a specific threshold. These calibrations are 
     * divided into small fragments to minimize their impact in throughput.
     * 
     * @param offlineCalMask Offline calibration mask.
     * @param onlineCalMask Online calibration mask.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setHdkConfig(uint32_t offlineCalMask, uint32_t onlineCalMask) = 0;

    /**
     * @brief Sets the Interframe Spacing (period of time between transmissions of wireless frames).
     * 
     * @param ifs Value of interframe spacing in us (minimum allowed value is 10 us).
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setIfs(uint32_t ifs) = 0;

    /**
     * @brief Sets the value of the production flag. 
     * 
     * The production flag is included in the header of the calibration file stored in the 
     * non-volatile memory.
     * 
     * @param productionFlag true to set the production flag (production mode) and false 
     * otherwise (operational mode).
     * @param writeToNvm true to write changes to non-volatile memory and false to keep them 
     * in RAM only.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setProductionFlag(bool productionFlag, bool writeToNvm) = 0;

    /**
     * @brief Sets transmission rate.
     * See https://mcsindex.com/
     * 
     * Application must call the setChannel() function before calling this function.
     * 
     * @param signalBandwidth Channel width.
     * @param mcs Modulation Coding Scheme. 
     * @param nss Number of Spatial Streams, i.e.: number of unique MIMO data streams that can be 
     * sent in parallel (valid values range from 1 to the maximum number of TX antennas).
     * @param gi Guard Interval code. Valid values for 11ax are 0.8 us, 1.6 us and 3.2 us; valid 
     * values prior to 11ax - except 11b - are 0.8 us and 0.4 us.
     * @param ltf Long Training Field.
     * @param[out] rateMbps PHY data rate in Mbps.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setRate(Bandwidth signalBandwidth, Mcs mcs, uint8_t nss, Gi gi, Ltf ltf, float& rateMbps) = 0;

    /**
     * @brief Sets the RX calibration data to the calibration file stored in the non-volatile memory.
     * 
     * @param data The calibration data (input array should contain one item only).
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setRssiCalibrationData(const std::vector<RssiCalibrationData_t>& data) = 0;

    /**
     * @brief Sets the RX gain blocks.
     * 
     * Configures the RX gain for the full path from the antenna port to the ADC input, including 
     * RX DC offset compensation. That is, sets all RFIC RX gains (TRX switch, External LNA, RFIC 
     * LNA, PGC1, PGC2) and the appropriate DC cancellation DAC controls.
     * 
     * @param lnaIndex LNA gain index. Note: In the FW, the index is translated to a unique FEM 
     * and RFIC LNA gain combination. Values in range 0:5
     * @param pgc1 Programmable Gain Control 1 value in dB. Values in 0:1:18 dB plus extra gain step of 24 dB.
     * @param pgc2 Programmable Gain Control 2 value in dB. Values in -8:2:12 dB plus extra gain step of 18 dB.
     * @param pgc3 Programmable Gain Control 3 value in dB. Set to 0 dB.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setRxGainBlocks(uint8_t lnaIndex, int8_t pgc1, int8_t pgc2, int8_t pgc3) = 0;

    /**
     * @brief Sets AUX ADC configuration.
     * 
     * Application should call this function once during RX calibration, at the beginning of the 
     * S2D calibration step. 
     * 
     * @param auxAdcRes Auxiliary ADC number of bits. Example: 9
     * @param ioffsStep S2D I Offset resolution in [V]. Example: 0.0625f
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setRssiS2dInitials(uint8_t auxAdcRes, float ioffsStep) = 0;

    /**
     * @brief Sets RX S2D calibration parameters for the specified antennas and region.
     * 
     * Sets the long register in RFIC with the I Offset and S2D gain settings to be used in the RX 
     * S2D calibration for the specified region.
     * These settings are meant to initialize the RX S2D calibration process and later to configure 
     * the DUT once such calibration has finished.
     * 
     * @param antennaMask Bitmask with one bit per antenna, set to 1 to configure the antenna and 
     * to 0 to ignore it. Bit 0 corresponds to the first antenna.
     * @param region Region number (1, 2, or 3, i.e.: from 1 to maximum number of regions minus 1).
     * @param gain Initial S2D gain setting.
     * @param offset Initial I Offset setting.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setRssiS2dParams(const AntennaMask& antennaMask, uint8_t region, uint8_t gain, uint8_t offset) = 0;

    /**
     * @brief Sets RU Parameters for user
     *
     * @param userOne RU Param for user one. (Bits 0-7 : RU Allocation, Bit 8 : PS160, Bits 9-31 : Reserved)
     * @param userTwo RU Param for user two. (Bits 0-7 : RU Allocation, Bit 8 : PS160, Bits 9-31 : Reserved)
     *
     * @return true on success and false otherwise.
     */
    virtual bool setRuParams(uint32_t userOne, uint32_t userTwo) = 0;

    /**
     * @brief Sets the value of the RX aggregation option.
     * 
     * Application should call this function during RX verification (after calibration), if PHY 
     * mode is either 802.11ax or 802.11be.
     * 
     * @param enabled true to enable the option and false otherwise.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setRxAggregationEnabled(bool enabled) = 0;

    /**
     * @brief Sets the RX band LUT.
     * 
     * Sets the frequencies to LNA sub-band control look-up table using LNA sub-band calibration 
     * results.
     * 
     * @param antennaMask Bitmask with one bit per antenna, set to 1 to enable the antenna and to 
     * 0 to disable it. Bit 0 corresponds to the first antenna.
     * @param rxBandLut Vector of frequencies, per RX antenna.
     * @param firstTunningIndex LNA sub-band first tunning index, per RX antenna (i.e.: index of 
     * LNA sub-band for which frequency obtained with calculateRxLnaSubBandGains() is greater 
     * than 4900 MHz).
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setRxBandLut(const AntennaMask& antennaMask, const std::array<std::array<uint16_t, maxNumLnaSubBandsCrossingPoints>, maxNumRxAntennas>& rxBandLut, const std::array<uint8_t, maxNumRxAntennas>& firstTunningIndex) = 0;

    /**
     * @brief Sets the value of the spaceless TX option.
     * 
     * @param enabled true to enable the option and false otherwise.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setSpacelessTxEnabled(bool enabled) = 0;

    /**
     * @brief Sets the transmit power control configuration.
     *
     * Power control configuration includes:
     * - Flag for closed-loop transmission control mode.
     * - Transmit power limit value to use when auto-power is enabled with function 
     * setTransmitPowerLevel(), emulating the value that driver gets from regulatory.bin file. Set 
     * to value 0xff for no limit (i.e.: no auto-power). 
     * 
     * @param closedLoop true to enable the option and false otherwise.
     * @param powerLimit Transmit power limit (in eighth of dB). 
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setTransmitPowerControl(bool closedLoop, uint8_t powerLimit) = 0;

    /**
     * @brief Sets the transmit power level.
     * 
     * In open-loop mode, the specified value is the index of the power table to use (this implies 
     * half-dBm steps). In closed-loop mode, the value is in half-dBm (so range is equivalent to 
     * -10:0.5:31.5 [dBm]).
     * 
     * Set power level value to 0xff to let the FW automatically decide the power level to use.
     * 
     * @param powerLevel Power level value in the range -20:1:63 or 0xdd for auto-power. 
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setTransmitPowerLevel(uint8_t powerLevel) = 0;

    /**
     * @brief Sets the TX power table offset.
     * 
     * Sets the value of the offset register for the gain table used for output power control 
     * during transmission.
     * 
     * @param antenna The index of the TX antenna. Valid values range from 0 to the maximum number 
     * of TX antennas minus 1. Example: 0 for the first TX antenna.
     * @param bandwidth Channel width.
     * @param offset The transmit power table offset value. Value in range -128:128
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setTransmitPowerTableOffset(uint8_t antenna, Bandwidth bandwidth, int16_t offset) = 0;

    /**
     * @brief Helper function for fast TX calibration.
     * 
     * Transmits an open-loop power index array and acquires TSSI readings per power and TX 
     * antenna from the PA detector.
     * 
     * This function is to be used in Vector TX calibration sequence.
     * 
     * @param[in,out] transmitPowerVector Structure with the input parameters and return values 
     * required by the function: the array with the power indexes, the length of the array, the 
     * length (in bytes) of first index's packets (afterwards packet length is optionally 
     * incremented in each index), the number of repetitions for each power index, and finally, 
     * the measured results.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setTransmitPowerVector(TransmitPowerVector_t& transmitPowerVector) = 0;

    /**
     * @brief Sets the TX calibration data to the calibration file stored in the non-volatile memory.
     * 
     * @param data The calibration data (input array should contain one item per calibrated channel).
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setTssiCalibrationData(const std::vector<TssiCalibrationData_t>& data) = 0;

    /**
     * @brief Sets the TX S2D settings for the specified antenna and region.
     * 
     * These settings are meant configure the DUT with the parameters obtained during TX S2D 
     * calibration.
     * 
     * @param antenna The index of the TX antenna. Valid values range from 0 to the maximum number 
     * of TX antennas minus 1. Example: 0 for the first TX antenna.
     * @param region Region number (1, 2, or 3, i.e.: from 1 to maximum number of regions minus 1).
     * @param powerThreshold Power index threshold of the region.
     * @param gain Initial S2D gain setting.
     * @param offset Initial I Offset setting.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setTssiS2dParams(uint8_t antenna, uint8_t region, uint8_t powerThreshold, uint8_t gain, uint8_t offset) = 0;

    /**
     * @brief Sets the XTAL calibration value to the calibration file stored in the non-volatile memory.
     * 
     * @param xtalValue The XTAL calibration value (= XTAL Value + XTAL Bias << 9) used for the 
     * internal clock.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setXtalCalValue(uint16_t xtalValue) = 0;

    /**
     * @brief Sets the XTAL calibration value to the RFIC register.
     * 
     * @param xtalValue The XTAL calibration value (= XTAL Value + XTAL Bias << 9) used for the 
     * internal clock.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setXtalRegValue(uint16_t xtalValue) = 0;

    /**
     * @brief Configures the ZWDFS feature.
     * 
     * ZWDFS stands for Zero-Wait Dynamic Frequency Selection.
     * 
     * This function can be called only when ZWDFS feature is available, i.e. when chip is Wave700
     * and there is a ZWDFS antenna available. Use function getZwdfsStatus() to query the AP for 
     * ZWDFS availability.
     * 
     * If this function has not been called prior to enabling the ZWDFS antenna, then the following 
     * default configuration parameter values are used:
     * - Default ZWDFS channel: 100
     * - Default ZWDFS bandwidth: Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY
     * - Default radar detection bandwidth: Bandwidth::BANDWIDTH_INVALID
     * 
     * The ZWDFS antenna can be enabled or disabled with function setEnabledRxAntennaMask() by 
     * respectively setting or clearing the bit corresponding to the ZWDFS antenna in the mask 
     * parameter.
     * 
     * The status of the ZWDFS antenna (enabled or disabled) can be obtained with function 
     * getZwdfsStatus().
     * 
     * @param zwdfsChannel Lowest channel number for ZWDFS or 0 to use channel and bandwidth set 
     * with the `setChannel()` function.
     * @param zwdfsBandwidth ZWDFS channel width (currently 160MHz only).
     * @param radarDetectionBandwidth If set to a valid bandwidth, then start scanning for RADAR.
     * Otherwise stop the scan. Valid values are 160 and 80MHz.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool setZwdfsConfiguration(uint8_t zwdfsChannel, Bandwidth zwdfsBandwidth, Bandwidth radarDetectionBandwidth) = 0;

    /**
 * @brief Triggers calibration processes
 *
 * This function is intended to trigger recalibration on demand while in DUT mode.
 *
 * @param params Recalibration parameters (calibration type, mask, etc)
 * @param[out] status Recalibration status
 *
 * @return true on success and false otherwise.
 */
    virtual bool startCalibration(const StartCalibrationParams_t& params, uint8_t& status) = 0;

    /**
     * @brief Starts transmitting a CW (Continuous Wave, i.e.: a tone).
     * 
     * @param amplitude Amplitude of the signal. Values in range -2:1:2, 9, 10
     * @param tone Tone number (in 312.5KHz bin resolution), signed, in [-256..255] range for 
     * in-band tones, full range is [-512..511].
     * 
     * @return true on success and false otherwise.
     */
    virtual bool startCw(int8_t amplitude, int16_t tone) = 0;

    /**
     * @brief Configures RX path/parameters for RX calibration. 
     * 
     * This function is called in the RX set channel sequence at the beginning of each RX 
     * calibration sub-process.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool startRxCalibration() = 0;

    /**
     * @brief Starts transmitting data packets.
     * 
     * @param repetitions Number of packets to send (or infinite using 0xFFFF).
     * @param packetLength Length of each packet in bytes. Use function calculatePacketLength() to
     * compute the number of bytes from a given number of OFDM symbols.
     * @param longData Long data flag: if long data is disabled, the maximum data length is 4000 
     * bytes, otherwise the maximum value depends on current PHY mode.
     * @param beamforming Set to true to use beamforming. A beamforming matrix must have been 
     * loaded previously using the function loadBeamformingMatrixFromFile(). 
     * @param codingType Optional parameter to specify coding type. Set to CODING_TYPE_LDPC to use LDPC coding, 
     * CODING_TYPE_BCC to use BCC coding, or omit for automatic selection based on PHY mode (LDPC for 11ax and later, BCC for earlier).
     * 
     * @return true on success and false otherwise.
     */
    virtual bool startTx(uint16_t repetitions, uint32_t packetLength, bool longData, bool beamforming, CodingType codingType = CodingType::CODING_TYPE_AUTO) = 0;

    /**
     * @brief Start Rx PER packet limit count.
     *
     * @param packetLimit Number of Rx packets to receive.
     *
     * @return true on success and false otherwise.
     */
    virtual bool startRxPer(uint32_t packetLimit) = 0;

    /**
     * @brief Stops transmitting a CW (a tone).
     * 
     * @return true on success and false otherwise.
     */
    virtual bool stopCw() = 0;

    /**
     * @brief Stops RX calibration.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool stopRxCalibration() = 0;

    /**
     * @brief Stops transmitting data packets.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool stopTx() = 0;

    /**
     * @brief Stop Rx packet limit and PER calculation.
     *
     * @param calcRxPer Flag tells whether to calculate PER.
     *
     * @return true on success and false otherwise.
     */
    virtual bool stopRxPer(bool calcRxPer) = 0;

    /**
     * @brief Validates that the beamforming header register matches the expected parameters
     *
     * This function reads the beamforming header register from hardware and validates that it
     * matches the expected PHY mode and bandwidth parameters. This is useful to verify the
     * beamforming configuration before starting a transmission to ensure compatibility and
     * prevent incorrect beamforming transmissions.
     *
     * @param expectedPhyMode Expected PHY mode (e.g., PHY_MODE_AC, PHY_MODE_AX, PHY_MODE_BE)
     * @param expectedBandwidth Expected bandwidth (e.g., BANDWIDTH_TWENTY, BANDWIDTH_EIGHTY, etc.)
     * @return true if the header matches the expected parameters, false otherwise
     */
    virtual bool validateBeamformingHeaderRegister(PhyMode expectedPhyMode, Bandwidth expectedBandwidth) = 0;

    /**
     * @brief Writes calibration data to non-volatile memory.
     * 
     * @param memoryType The type of the non-volatile memory to write the calibration data to, 
     * either EEPROM or flash.
     * @param memorySize The size of the non-volatile memory to write the calibration data to, 
     * either 1024 or 2048 bytes.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool writeCalibrationFile(NvMemoryType memoryType, NvMemorySize memorySize) = 0;

    /**
     * @brief Writes a block of memory to the specified module.
     * 
     * @param chipModule The chip module to write data to.
     * @param address Starting address.
     * @param data Source buffer.
     * @param length Number of bytes to write.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool writeMemory(ChipModule chipModule, size_t address, const uint8_t* data, size_t length) = 0;

    /**
     * @brief Writes a block of memory to the non-volatile memory.
     * 
     * @param address Starting address.
     * @param data Source buffer.
     * @param length Number of bytes to write.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool writeNvm(size_t address, const uint8_t* data, size_t length) = 0;

    /**
     * @brief Writes to a register in the specified module.
     * 
     * @param chipModule The chip module to write data to.
     * @param address Register address.
     * @param mask Bits of interest.
     * @param value Value to write.
     * 
     * @return true on success and false otherwise.
     */
    virtual bool writeRegister(ChipModule chipModule, size_t address, uint32_t mask, uint32_t value) = 0;
};

class DUT_API DutFactory {
public:
    /**
     * @brief Class destructor
     */
    virtual ~DutFactory() = default;

    /**
     * @brief Creates an instance of a concrete implementation of the Dut interface
     */
    virtual std::unique_ptr<Dut> createInstance(uint8_t wlanIndex, std::shared_ptr<Connection> connection, std::shared_ptr<Logger> logger, bool throwExceptions) = 0;
};
}

#endif
