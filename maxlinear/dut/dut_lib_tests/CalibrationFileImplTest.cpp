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

#include "CalibrationFileImpl.h"

#include "CalibrationFileVer6.h"
#include "CalibrationFileVer7.h"
#include "EmbeddedResource.h"
#include "NonVolatileMemory.h"
#include "RssiCalibrationDataVer6.h"
#include "RssiCalibrationDataVer7.h"
#include "TssiCalibrationDataVer6.h"
#include "resource.h"

#include <gtest/gtest.h>

namespace {

TEST(CalibrationFileImplTest, getRssiCalibrationDataVer6ShouldSucceed)
{
    EmbeddedResource resource(IDR_CALIBRATIONFILE1, CALIBRATIONFILE);

    // Create calibration file version 6
    dut::CalibrationFileVer6 calibrationFileVer6(resource.getData(), resource.getSize());

    // Check version
    EXPECT_EQ(calibrationFileVer6.getVersion(), dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_6);

    // Get RSSI calibration data version 6
    std::vector<std::shared_ptr<dut::RssiCalibrationData>> data {};
    calibrationFileVer6.getRssiCalibrationData(data);

    // Check values
    ASSERT_EQ(data.size(), 1);

    const auto& rssiCalibrationData = static_cast<dut::RssiCalibrationDataVer6*>(data[0].get())->data();

    EXPECT_EQ(rssiCalibrationData.antennaMask, 0x1f);
    EXPECT_EQ(rssiCalibrationData.startFreq, 4900);
    EXPECT_EQ(rssiCalibrationData.stopFreq, 5900);
    EXPECT_EQ(rssiCalibrationData.calibrationFreq, 5180);
    EXPECT_EQ(rssiCalibrationData.chipTemperature, 38);
    EXPECT_EQ(rssiCalibrationData.numRxRfFlatnessPoints, 6);
    EXPECT_EQ(rssiCalibrationData.numRxRssiFlatnessPoints, 0);

    const std::array<uint16_t, dut::maxRxRfFlatnessPoints> expectedRxRfFlatnessFreqPoint { 5180, 5300, 5540, 5580, 5700, 5780, 0, 0, 0, 0, 0 };
    for (size_t i = 0; i < dut::maxRxRfFlatnessPoints; i++) {
        EXPECT_EQ(rssiCalibrationData.rxRfFlatnessFreqs[i], expectedRxRfFlatnessFreqPoint[i]);
    }
    const std::array<uint16_t, dut::maxRxRssiFlatnessPoints> expectedRxRssiFlatnessFreqPoint { 0, 0, 0, 0, 0, 0, 0 };
    for (size_t i = 0; i < dut::maxRxRssiFlatnessPoints; i++) {
        EXPECT_EQ(rssiCalibrationData.rxRssiFlatnessFreqs[i], expectedRxRssiFlatnessFreqPoint[i]);
    }

    {
        const dut::RssiAntennaDataVer6_t& antennaData = rssiCalibrationData.antennaData.antennasVer6[0];

        EXPECT_EQ(antennaData.lnaMidGain, 9);

        const std::array<float, dut::maxNumLnaGainStepsVer6> expectedLnaGains { 33.0, 27.5, 21.0, 15.0, 6.0, -0.5, 0.0, 0.0 };
        for (size_t i = 0; i < dut::maxNumLnaGainStepsVer6; i++) {
            EXPECT_EQ(antennaData.lnaGains[i], expectedLnaGains[i]);
        }

        const std::array<int8_t, dut::maxRxRfFlatnessPoints> expectedMidGainDelta { 0, 3, 0, 0, -1, -2, 0, 0, 0, 0, 0 };
        for (size_t i = 0; i < dut::maxRxRfFlatnessPoints; i++) {
            EXPECT_EQ(antennaData.midGainDeltaPoints[i], expectedMidGainDelta[i]);
        }

        const std::array<int8_t, dut::maxRxRfFlatnessPoints> expectedRxRfFlatnessDeltaPointsHighGain { 0, -3, 1, 2, 1, 3, 0, 0, 0, 0, 0 };
        for (size_t i = 0; i < dut::maxRxRfFlatnessPoints; i++) {
            EXPECT_EQ(antennaData.rxRfFlatnessDeltaPointsHighGain[i], expectedRxRfFlatnessDeltaPointsHighGain[i]);
        }

        const std::array<int8_t, dut::maxRxRfFlatnessPoints> expectedRxRfFlatnessDeltaPointsBypass { 0, 3, 0, -1, -1, -3, 0, 0, 0, 0, 0 };
        for (size_t i = 0; i < dut::maxRxRfFlatnessPoints; i++) {
            EXPECT_EQ(antennaData.rxRfFlatnessDeltaPointsBypass[i], expectedRxRfFlatnessDeltaPointsBypass[i]);
        }

        const std::array<dut::RssiS2D_t, dut::maxNumRxRegionsVer6> expectedS2dGainOffset { { { 245, 224 }, { 245, 224 }, { 245, 0 } } };
        for (size_t i = 0; i < dut::maxNumRxRegionsVer6; i++) {
            EXPECT_EQ(antennaData.s2dGainOffset[i].gain, expectedS2dGainOffset[i].gain);
            EXPECT_EQ(antennaData.s2dGainOffset[i].offset, expectedS2dGainOffset[i].offset);
        }

        const std::array<dut::RssiAB_t, dut::maxNumRxRegionsVer6> expectedPointsAB { { { 128.0, 0.0 }, { 128.0, 0.0 }, { 128.0, 0.0 } } };
        for (size_t i = 0; i < dut::maxNumRxRegionsVer6; i++) {
            EXPECT_EQ(antennaData.pointsAB[i].a, expectedPointsAB[i].a);
            EXPECT_EQ(antennaData.pointsAB[i].b, expectedPointsAB[i].b);
        }

        const std::array<int8_t, dut::maxRxRssiFlatnessPoints> expectedRxRssiFlatnessDeltaPoints { 0, 0, 0, 0, 0, 0, 0 };
        for (size_t i = 0; i < dut::maxRxRssiFlatnessPoints; i++) {
            EXPECT_EQ(antennaData.rxRssiFlatnessDeltaPoints[i], expectedRxRssiFlatnessDeltaPoints[i]);
        }

        const std::array<uint16_t, dut::maxNumLnaSubBandsCrossingPointsVer6> expectedSubBandsCrossPoints { 0, 0, 0, 0, 0, 0, 0 };
        for (size_t i = 0; i < dut::maxNumLnaSubBandsCrossingPointsVer6; i++) {
            EXPECT_EQ(antennaData.subBandsCrossingPoints[i], expectedSubBandsCrossPoints[i]);
        }
    }
}

TEST(CalibrationFileImplTest, getRssiCalibrationDataVer7ShouldSucceed)
{
    EmbeddedResource resource(IDR_CALIBRATIONFILE1, CALIBRATIONFILE);

    // Create calibration file version 6
    dut::CalibrationFileVer6 calibrationFileVer6(resource.getData(), resource.getSize());

    // Check version
    EXPECT_EQ(calibrationFileVer6.getVersion(), dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_6);

    // Get RSSI calibration data version 6
    std::vector<std::shared_ptr<dut::RssiCalibrationData>> rssiCalibrationDataVer6 {};
    calibrationFileVer6.getRssiCalibrationData(rssiCalibrationDataVer6);
    auto rssiCalibrationDataItemVer6 = std::static_pointer_cast<dut::RssiCalibrationDataVer6>(rssiCalibrationDataVer6.at(0));

    // Remove RSSI calibration data version 6
    calibrationFileVer6.setRssiCalibrationData({});

    // Serialize calibration file into a buffer (without RSSI calibration data)
    auto buffer = calibrationFileVer6.serialize(static_cast<size_t>(dut::NvMemorySize::MEMORY_SIZE_BYTES_1K));

    // Hack version number (change it from 6 to 7)
    buffer->data()[dut::NonVolatileMemory::nvmVersionAddress] = static_cast<uint8_t>(dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_7);

    // Create calibration file version 7
    dut::CalibrationFileVer7 calibrationFile(buffer->data(), buffer->size());

    // Create RSSI calibration data version 7
    auto rssiCalibrationDataItemVer7 = std::make_shared<dut::RssiCalibrationDataVer7>();

    // Fill in RSSI calibration data version 7 using values from version 6 where possible
    auto& calibrationDataVer7 = rssiCalibrationDataItemVer7->data();
    calibrationDataVer7.antennaMask = rssiCalibrationDataItemVer6->data().antennaMask;
    calibrationDataVer7.startFreq = rssiCalibrationDataItemVer6->data().startFreq;
    calibrationDataVer7.stopFreq = rssiCalibrationDataItemVer6->data().stopFreq;
    calibrationDataVer7.calibrationFreq = rssiCalibrationDataItemVer6->data().calibrationFreq;
    calibrationDataVer7.chipTemperature = rssiCalibrationDataItemVer6->data().chipTemperature;
    calibrationDataVer7.subBandsCrossingPoints = rssiCalibrationDataItemVer6->data().subBandsCrossingPoints;

    calibrationDataVer7.numRxRfFlatnessPoints = rssiCalibrationDataItemVer6->data().numRxRfFlatnessPoints;
    calibrationDataVer7.numRxRssiFlatnessPoints = rssiCalibrationDataItemVer6->data().numRxRssiFlatnessPoints;
    std::copy(std::begin(rssiCalibrationDataItemVer6->data().rxRfFlatnessFreqs),
        std::end(rssiCalibrationDataItemVer6->data().rxRfFlatnessFreqs),
        std::begin(calibrationDataVer7.rxRfFlatnessFreqs));
    std::copy(std::begin(rssiCalibrationDataItemVer6->data().rxRssiFlatnessFreqs),
        std::end(rssiCalibrationDataItemVer6->data().rxRssiFlatnessFreqs),
        std::begin(calibrationDataVer7.rxRssiFlatnessFreqs));

    auto& antennaData = calibrationDataVer7.antennaData.antennasVer7[0];
    const std::vector<uint16_t> subBandsCrossingPoints { 4900, 4920, 4940 };
    std::copy(std::begin(subBandsCrossingPoints), std::end(subBandsCrossingPoints),
        std::begin(antennaData.subBandsCrossingPoints));
    const std::vector<float> lnaGains { 1.0f, 2.0f, 3.0f };
    std::copy(std::begin(lnaGains), std::end(lnaGains),
        std::begin(antennaData.lnaGains));
    // First and last flatness delta gain values are out of the allowed bounds and are therefore
    // clipped before being written to calibration file
    const std::vector<int8_t> rxRfFlatnessDeltaPointsHighGain { -10, -1, 0, 1, 2, 10 };
    std::copy(std::begin(rxRfFlatnessDeltaPointsHighGain), std::end(rxRfFlatnessDeltaPointsHighGain),
        std::begin(antennaData.rxRfFlatnessDeltaPointsHighGain));
    const std::vector<int8_t> rxRfFlatnessDeltaPointsLowGain { -10, -2, -2, 0, 1, 10 };
    std::copy(std::begin(rxRfFlatnessDeltaPointsLowGain), std::end(rxRfFlatnessDeltaPointsLowGain),
        std::begin(antennaData.rxRfFlatnessDeltaPointsLowGain));
    const std::vector<int8_t> rxRfFlatnessDeltaPointsBypass { -10, 1, 2, 3, 4, 10 };
    std::copy(std::begin(rxRfFlatnessDeltaPointsBypass), std::end(rxRfFlatnessDeltaPointsBypass),
        std::begin(antennaData.rxRfFlatnessDeltaPointsBypass));
    const std::vector<dut::RssiS2D_t> s2dGainOffset { { 1, 1 }, { 2, 2 } };
    std::copy(std::begin(s2dGainOffset), std::end(s2dGainOffset),
        std::begin(antennaData.s2dGainOffset));
    const std::vector<dut::RssiAB_t> pointsAB { { 1.0f, 1.0f }, { 2.1033f, -32.5798f } };
    std::copy(std::begin(pointsAB), std::end(pointsAB),
        std::begin(antennaData.pointsAB));
    std::fill(std::begin(antennaData.rxRssiFlatnessDeltaPoints), std::end(antennaData.rxRssiFlatnessDeltaPoints), 0);

    // Add RSSI calibration data item to vector
    std::vector<std::shared_ptr<dut::RssiCalibrationData>> rssiCalibrationDataVer7 {};
    rssiCalibrationDataVer7.push_back(rssiCalibrationDataItemVer7);

    // Set RSSI calibration data (version 7)
    calibrationFile.setRssiCalibrationData(rssiCalibrationDataVer7);

    // Serialize calibration file version 7 into a buffer
    buffer = calibrationFile.serialize(static_cast<size_t>(dut::NvMemorySize::MEMORY_SIZE_BYTES_1K));

    // Create calibration file version 7
    dut::CalibrationFileVer7 calibrationFileVer7(buffer->data(), buffer->length());

    // Check version
    EXPECT_EQ(calibrationFileVer7.getVersion(), dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_7);

    // Get RSSI calibration data (version 7)
    std::vector<std::shared_ptr<dut::RssiCalibrationData>> data {};
    calibrationFileVer7.getRssiCalibrationData(data);

    // Check values
    ASSERT_EQ(data.size(), 1);

    const auto& actualCalibrationData = static_cast<dut::RssiCalibrationDataVer7*>(data[0].get())->data();

    EXPECT_EQ(actualCalibrationData.antennaMask, calibrationDataVer7.antennaMask);
    EXPECT_EQ(actualCalibrationData.startFreq, calibrationDataVer7.startFreq);
    EXPECT_EQ(actualCalibrationData.stopFreq, calibrationDataVer7.stopFreq);
    EXPECT_EQ(actualCalibrationData.calibrationFreq, calibrationDataVer7.calibrationFreq);
    EXPECT_EQ(actualCalibrationData.chipTemperature, calibrationDataVer7.chipTemperature);
    EXPECT_EQ(actualCalibrationData.numRxRfFlatnessPoints, calibrationDataVer7.numRxRfFlatnessPoints);
    EXPECT_EQ(actualCalibrationData.numRxRssiFlatnessPoints, calibrationDataVer7.numRxRssiFlatnessPoints);
    EXPECT_EQ(actualCalibrationData.subBandsCrossingPoints, true);

    for (size_t i = 0; i < dut::maxRxRfFlatnessPoints; i++) {
        EXPECT_EQ(actualCalibrationData.rxRfFlatnessFreqs[i], calibrationDataVer7.rxRfFlatnessFreqs[i]);
    }
    for (size_t i = 0; i < dut::maxRxRfFlatnessPoints; i++) {
        EXPECT_EQ(actualCalibrationData.rxRssiFlatnessFreqs[i], calibrationDataVer7.rxRssiFlatnessFreqs[i]);
    }

    {
        const auto& actualAntennaData = actualCalibrationData.antennaData.antennasVer7[0];

        for (size_t i = 0; i < dut::maxNumLnaGainStepsVer7; i++) {
            EXPECT_EQ(actualAntennaData.lnaGains[i], antennaData.lnaGains[i]);
        }

        for (size_t i = 0; i < dut::maxRxRfFlatnessPoints; i++) {
            // Test that clipping values works as expected
            if (antennaData.rxRfFlatnessDeltaPointsHighGain[i] < -8) {
                EXPECT_EQ(actualAntennaData.rxRfFlatnessDeltaPointsHighGain[i], -8);
            } else if (antennaData.rxRfFlatnessDeltaPointsHighGain[i] > 7) {
                EXPECT_EQ(actualAntennaData.rxRfFlatnessDeltaPointsHighGain[i], 7);
            } else {
                EXPECT_EQ(actualAntennaData.rxRfFlatnessDeltaPointsHighGain[i], antennaData.rxRfFlatnessDeltaPointsHighGain[i]);
            }
        }

        for (size_t i = 0; i < dut::maxRxRfFlatnessPoints; i++) {
            // Test that clipping values works as expected
            if (antennaData.rxRfFlatnessDeltaPointsLowGain[i] < -8) {
                EXPECT_EQ(actualAntennaData.rxRfFlatnessDeltaPointsLowGain[i], -8);
            } else if (antennaData.rxRfFlatnessDeltaPointsLowGain[i] > 7) {
                EXPECT_EQ(actualAntennaData.rxRfFlatnessDeltaPointsLowGain[i], 7);
            } else {
                EXPECT_EQ(actualAntennaData.rxRfFlatnessDeltaPointsLowGain[i], antennaData.rxRfFlatnessDeltaPointsLowGain[i]);
            }
        }

        for (size_t i = 0; i < dut::maxRxRfFlatnessPoints; i++) {
            // Test that clipping values works as expected
            if (antennaData.rxRfFlatnessDeltaPointsBypass[i] < -8) {
                EXPECT_EQ(actualAntennaData.rxRfFlatnessDeltaPointsBypass[i], -8);
            } else if (antennaData.rxRfFlatnessDeltaPointsBypass[i] > 7) {
                EXPECT_EQ(actualAntennaData.rxRfFlatnessDeltaPointsBypass[i], 7);
            } else {
                EXPECT_EQ(actualAntennaData.rxRfFlatnessDeltaPointsBypass[i], antennaData.rxRfFlatnessDeltaPointsBypass[i]);
            }
        }

        for (size_t i = 0; i < dut::maxNumRxRegionsVer7; i++) {
            EXPECT_EQ(actualAntennaData.s2dGainOffset[i].gain, antennaData.s2dGainOffset[i].gain);
            EXPECT_EQ(actualAntennaData.s2dGainOffset[i].offset, antennaData.s2dGainOffset[i].offset);
        }

        for (size_t i = 0; i < dut::maxNumRxRegionsVer7; i++) {
            EXPECT_NEAR(actualAntennaData.pointsAB[i].a, antennaData.pointsAB[i].a, 0.01f);
            EXPECT_NEAR(actualAntennaData.pointsAB[i].b, antennaData.pointsAB[i].b, 0.01f);
        }

        for (size_t i = 0; i < dut::maxRxRssiFlatnessPoints; i++) {
            EXPECT_EQ(actualAntennaData.rxRssiFlatnessDeltaPoints[i], antennaData.rxRssiFlatnessDeltaPoints[i]);
        }

        for (size_t i = 0; i < dut::maxNumLnaSubBandsCrossingPointsVer7; i++) {
            EXPECT_EQ(actualAntennaData.subBandsCrossingPoints[i], antennaData.subBandsCrossingPoints[i]);
        }
    }
}

TEST(CalibrationFileImplTest, getTssiCalibrationDataShouldSucceed)
{
    EmbeddedResource resource(IDR_CALIBRATIONFILE1, CALIBRATIONFILE);

    // Create calibration file version 6
    dut::CalibrationFileVer6 calibrationFileVer6(resource.getData(), resource.getSize());

    // Check version
    EXPECT_EQ(calibrationFileVer6.getVersion(), dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_6);

    // Get TSSI calibration data version 6
    std::vector<std::shared_ptr<dut::TssiCalibrationData>> data {};
    calibrationFileVer6.getTssiCalibrationData(data);

    // Check values
    ASSERT_EQ(data.size(), 1);

    const auto& tssiCalibrationData = static_cast<dut::TssiCalibrationDataVer6*>(data[0].get())->data();

    EXPECT_EQ(tssiCalibrationData.antennaMask, 0x0f);
    EXPECT_EQ(tssiCalibrationData.numRegions, 1);
    EXPECT_EQ(tssiCalibrationData.bw, dut::Bandwidth::BANDWIDTH_TWENTY);
    EXPECT_EQ(tssiCalibrationData.channel, 36);
    EXPECT_EQ(tssiCalibrationData.band, dut::Band::BAND_5000MHZ);

    auto checkPointsAB = [](const dut::TssiAB_t actual[], const std::array<dut::TssiAB_t, dut::maxNumTxRegions>& expected) {
        for (size_t i = 0; i < dut::maxNumTxRegions; i++) {
            EXPECT_EQ(actual[i].a, expected[i].a);
            EXPECT_EQ(actual[i].b, expected[i].b);
        }
    };

    auto checkS2dGain = [](const int32_t actual[], const std::array<int32_t, dut::maxNumTxRegions>& expected) {
        for (size_t i = 0; i < dut::maxNumTxRegions; i++) {
            EXPECT_EQ(actual[i], expected[i]);
        }
    };

    auto checkS2dOffset = checkS2dGain;

    {
        const auto& antennaData = tssiCalibrationData.antennas[0];

        EXPECT_EQ(antennaData.maxPower, 51);
        EXPECT_EQ(antennaData.uEvm, 44);
        EXPECT_EQ(antennaData.uEvmGain, 16);

        checkPointsAB(antennaData.pointsAB, { { { 0, 0 }, { 0, 0 }, { 0, 0 } } });
        checkS2dGain(antennaData.s2dGain, { 2, 0, 0 });
        checkS2dOffset(antennaData.s2dOffset, { 44, 0, 0 });
    }

    {
        const auto& antennaData = tssiCalibrationData.antennas[1];

        EXPECT_EQ(antennaData.maxPower, 50);
        EXPECT_EQ(antennaData.uEvm, 42);
        EXPECT_EQ(antennaData.uEvmGain, 14);

        checkPointsAB(antennaData.pointsAB, { { { 0, 0 }, { 0, 0 }, { 0, 0 } } });
        checkS2dGain(antennaData.s2dGain, { 3, 0, 0 });
        checkS2dOffset(antennaData.s2dOffset, { 44, 0, 0 });
    }

    {
        const auto& antennaData = tssiCalibrationData.antennas[2];

        EXPECT_EQ(antennaData.maxPower, 44);
        EXPECT_EQ(antennaData.uEvm, 35);
        EXPECT_EQ(antennaData.uEvmGain, 18);

        checkPointsAB(antennaData.pointsAB, { { { 0, 0 }, { 0, 0 }, { 0, 0 } } });
        checkS2dGain(antennaData.s2dGain, { 3, 0, 0 });
        checkS2dOffset(antennaData.s2dOffset, { 47, 0, 0 });
    }

    {
        const auto& antennaData = tssiCalibrationData.antennas[3];

        EXPECT_EQ(antennaData.maxPower, 51);
        EXPECT_EQ(antennaData.uEvm, 43);
        EXPECT_EQ(antennaData.uEvmGain, 13);

        checkPointsAB(antennaData.pointsAB, { { { 0, 0 }, { 0, 0 }, { 0, 0 } } });
        checkS2dGain(antennaData.s2dGain, { 3, 0, 0 });
        checkS2dOffset(antennaData.s2dOffset, { 44, 0, 0 });
    }
}

TEST(CalibrationFileImplTest, setRssiCalibrationDataVer6ShouldSucceed)
{
    EmbeddedResource resource(IDR_CALIBRATIONFILE1, CALIBRATIONFILE);

    // Create calibration file version 6
    dut::CalibrationFileVer6 calibrationFileVer6(resource.getData(), resource.getSize());

    // Check version
    EXPECT_EQ(calibrationFileVer6.getVersion(), dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_6);

    // Load initial RSSI calibration data
    std::vector<std::shared_ptr<dut::RssiCalibrationData>> initialData {};
    calibrationFileVer6.getRssiCalibrationData(initialData);

    // Check values
    ASSERT_EQ(initialData.size(), 1);

    // Remove RSSI calibration data
    calibrationFileVer6.setRssiCalibrationData({});

    // Check that RSSI calibration data has been removed
    std::vector<std::shared_ptr<dut::RssiCalibrationData>> data {};
    calibrationFileVer6.getRssiCalibrationData(data);
    ASSERT_EQ(data.size(), 0);

    // Set the initial RSSI calibration data back again
    calibrationFileVer6.setRssiCalibrationData(initialData);

    // Get the RSSI calibration data again
    std::vector<std::shared_ptr<dut::RssiCalibrationData>> currentData {};
    calibrationFileVer6.getRssiCalibrationData(currentData);
    ASSERT_EQ(currentData.size(), 1);

    // Check that the set/get methods are symmetrical by comparing initial data with current data

    const auto& initial = static_cast<dut::RssiCalibrationDataVer6*>(initialData[0].get())->data();
    const auto& current = static_cast<dut::RssiCalibrationDataVer6*>(currentData[0].get())->data();

    EXPECT_EQ(initial.antennaMask, current.antennaMask);
    EXPECT_EQ(initial.startFreq, current.startFreq);
    EXPECT_EQ(initial.stopFreq, current.stopFreq);
    EXPECT_EQ(initial.calibrationFreq, current.calibrationFreq);
    EXPECT_EQ(initial.chipTemperature, current.chipTemperature);
    EXPECT_EQ(initial.numRxRfFlatnessPoints, current.numRxRfFlatnessPoints);
    EXPECT_EQ(initial.numRxRssiFlatnessPoints, current.numRxRssiFlatnessPoints);

    for (size_t i = 0; i < dut::maxRxRfFlatnessPoints; i++) {
        EXPECT_EQ(initial.rxRfFlatnessFreqs[i], current.rxRfFlatnessFreqs[i]);
    }
    for (size_t i = 0; i < dut::maxRxRssiFlatnessPoints; i++) {
        EXPECT_EQ(initial.rxRssiFlatnessFreqs[i], current.rxRssiFlatnessFreqs[i]);
    }

    for (uint8_t index = 0; index < dut::maxNumRxAntennas; index++) {
        const auto& initialAntenna = initial.antennaData.antennasVer6[0];
        const auto& currentAntenna = current.antennaData.antennasVer6[0];

        EXPECT_EQ(initialAntenna.lnaMidGain, currentAntenna.lnaMidGain);

        for (size_t i = 0; i < dut::maxNumLnaGainStepsVer6; i++) {
            EXPECT_EQ(initialAntenna.lnaGains[i], currentAntenna.lnaGains[i]);
        }

        for (size_t i = 0; i < dut::maxRxRfFlatnessPoints; i++) {
            EXPECT_EQ(initialAntenna.midGainDeltaPoints[i], currentAntenna.midGainDeltaPoints[i]);
        }

        for (size_t i = 0; i < dut::maxRxRfFlatnessPoints; i++) {
            EXPECT_EQ(initialAntenna.rxRfFlatnessDeltaPointsHighGain[i], currentAntenna.rxRfFlatnessDeltaPointsHighGain[i]);
        }

        for (size_t i = 0; i < dut::maxRxRfFlatnessPoints; i++) {
            EXPECT_EQ(initialAntenna.rxRfFlatnessDeltaPointsBypass[i], currentAntenna.rxRfFlatnessDeltaPointsBypass[i]);
        }

        for (size_t i = 0; i < dut::maxNumRxRegionsVer6; i++) {
            // TODO
            //EXPECT_EQ(initialAntenna.s2dGainOffset[i].gain, currentAntenna.s2dGainOffset[i].gain);
            EXPECT_EQ(initialAntenna.s2dGainOffset[i].offset, currentAntenna.s2dGainOffset[i].offset);
        }

        for (size_t i = 0; i < dut::maxNumRxRegionsVer6; i++) {
            EXPECT_EQ(initialAntenna.pointsAB[i].a, currentAntenna.pointsAB[i].a);
            EXPECT_EQ(initialAntenna.pointsAB[i].b, currentAntenna.pointsAB[i].b);
        }

        for (size_t i = 0; i < dut::maxRxRssiFlatnessPoints; i++) {
            EXPECT_EQ(initialAntenna.rxRssiFlatnessDeltaPoints[i], currentAntenna.rxRssiFlatnessDeltaPoints[i]);
        }

        for (size_t i = 0; i < dut::maxNumLnaSubBandsCrossingPointsVer6; i++) {
            EXPECT_EQ(initialAntenna.subBandsCrossingPoints[i], currentAntenna.subBandsCrossingPoints[i]);
        }
    }
}

TEST(CalibrationFileImplTest, setTssiCalibrationDataShouldSucceed)
{
    EmbeddedResource resource(IDR_CALIBRATIONFILE1, CALIBRATIONFILE);

    // Create calibration file version 6
    dut::CalibrationFileVer6 calibrationFileVer6(resource.getData(), resource.getSize());

    // Check version
    EXPECT_EQ(calibrationFileVer6.getVersion(), dut::CalibrationFileVersion::CALIBRATION_FILE_VERSION_6);

    // Load initial TSSI calibration data
    std::vector<std::shared_ptr<dut::TssiCalibrationData>> initialData {};
    calibrationFileVer6.getTssiCalibrationData(initialData);

    // Check values
    ASSERT_EQ(initialData.size(), 1);

    // Remove TSSI calibration data
    calibrationFileVer6.setTssiCalibrationData({});

    // Check that TSSI calibration data has been removed
    std::vector<std::shared_ptr<dut::TssiCalibrationData>> data {};
    calibrationFileVer6.getTssiCalibrationData(data);
    ASSERT_EQ(data.size(), 0);

    // Set the initial TSSI calibration data back again
    calibrationFileVer6.setTssiCalibrationData(initialData);

    // Get the TSSI calibration data again
    std::vector<std::shared_ptr<dut::TssiCalibrationData>> currentData {};
    calibrationFileVer6.getTssiCalibrationData(currentData);
    ASSERT_EQ(currentData.size(), 1);

    // Check that the set/get methods are symmetrical by comparing initial data with current data

    const auto& initial = static_cast<dut::TssiCalibrationDataVer6*>(initialData[0].get())->data();
    const auto& current = static_cast<dut::TssiCalibrationDataVer6*>(currentData[0].get())->data();

    EXPECT_EQ(initial.antennaMask, current.antennaMask);
    EXPECT_EQ(initial.numRegions, current.numRegions);
    EXPECT_EQ(initial.channel, current.channel);
    EXPECT_EQ(initial.bw, current.bw);
    EXPECT_EQ(initial.band, current.band);

    for (uint8_t index = 0; index < dut::maxNumTxAntennas; index++) {
        const auto& initialAntenna = initial.antennas[index];
        const auto& currentAntenna = current.antennas[index];

        EXPECT_EQ(initialAntenna.maxPower, currentAntenna.maxPower);
        EXPECT_EQ(initialAntenna.uEvm, currentAntenna.uEvm);
        EXPECT_EQ(initialAntenna.uEvmGain, currentAntenna.uEvmGain);

        for (size_t i = 0; i < dut::maxNumTxRegions; i++) {
            EXPECT_EQ(initialAntenna.s2dGain[i], currentAntenna.s2dGain[i]);
        }

        for (size_t i = 0; i < dut::maxNumTxRegions; i++) {
            EXPECT_EQ(initialAntenna.s2dOffset[i], currentAntenna.s2dOffset[i]);
        }

        for (size_t i = 0; i < dut::maxNumTxRegions; i++) {
            EXPECT_EQ(initialAntenna.pointsAB[i].a, currentAntenna.pointsAB[i].a);
            EXPECT_EQ(initialAntenna.pointsAB[i].b, currentAntenna.pointsAB[i].b);
        }
    }
}

}