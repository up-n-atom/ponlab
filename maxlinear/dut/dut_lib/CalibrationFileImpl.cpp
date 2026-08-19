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

#include "NonVolatileMemory.h"
#include "ResizableBuffer.h"
#include "RssiCalibrationData.h"
#include "TssiCalibrationDataVer6.h"
#include "dut/Tools.h"

#include <algorithm>
#ifdef LINUX_HOST
#include <cstring>
#endif

namespace dut {

constexpr size_t calibrationFileHeaderSize = NonVolatileMemory::nvmVersionAddress + 1;

constexpr size_t calibrationDataProductionFlagOffset = 2;
constexpr uint8_t calibrationDataProductionValue = 0x72;
constexpr uint8_t calibrationDataOperationalValue = 0x30;

constexpr size_t cardInfoHardwareTypeOffset = 0;
constexpr size_t cardInfoHardwareRevisionOffset = 1;
constexpr size_t cardInfoCountryCodeOffset = 2;
constexpr size_t cardInfoSubversionOffset = 3;
constexpr size_t cardInfoReservedOffset = 4; // This byte is currently not in use
constexpr size_t cardInfoMacAddressOffset = 5;
constexpr size_t cardInfoSerialNumberOffset = 11;
constexpr size_t cardInfoWeekOffset = 14;
constexpr size_t cardInfoYearOffset = 15;

// The 4th byte in the Card Info CIS as both a byte and as a struct of bit fields.
// The first 2 bits are used by the driver. The last 2 bits are currently reserved.
using CardInfo4thByte = union {
    uint8_t byte;
    struct
    {
        // cppcheck-suppress unusedStructMember
        uint8_t ap_disabled : 1;
        // cppcheck-suppress unusedStructMember
        uint8_t disable_sm_channels : 1;
        uint8_t subversion : 4;
        // cppcheck-suppress unusedStructMember
        uint8_t reserved : 2;
    } bitFields;
};

CalibrationFileImpl::CalibrationFileImpl(const uint8_t* data, size_t length)
{
    if (!data) {
        throw std::invalid_argument("Parameter 'data' cannot be a null pointer");
    }

    if (length < calibrationFileHeaderSize) {
        throw std::invalid_argument("The specified length (" + toString(length) + ") is too small (at least " + toString(calibrationFileHeaderSize) + " bytes required)");
    }

    // There's a non-cis data at the beginning of the NVM (i.e.: the calibration file header),
    // from byte #0 and up to and including the byte containing the version
    addCis(Cis(cisCodeNonCisData, calibrationFileHeaderSize, data));

    bool done = false;
    size_t offset = calibrationFileHeaderSize;
    while ((!done) && (offset < length)) {
        Cis cis = Cis::parse(data, length, offset);

        if (cis.getCode() == cisCodeInvalid) {
            done = true;
        } else {
            addCis(cis);

            if (cis.getCode() == cisCodeEof) {
                done = true;
            }
        }
    }
}

void CalibrationFileImpl::addCis(const Cis& cis)
{
    m_data.insert(std::upper_bound(m_data.begin(), m_data.end(), cis), cis);
}

Cis CalibrationFileImpl::getCis(uint8_t code) const
{
    auto it = std::find_if(m_data.begin(), m_data.end(), Cis::CodeComparator(code));
    if (it == m_data.end()) {
        throw std::invalid_argument("CIS code " + toHexString(code) + " not found");
    }

    return *it;
}

std::vector<Cis> CalibrationFileImpl::getCisList(uint8_t code) const
{
    std::vector<Cis> list;

    std::copy_if(m_data.begin(), m_data.end(), std::back_inserter(list), Cis::CodeComparator(code));

    return list;
}

void CalibrationFileImpl::removeCis(uint8_t code)
{
    m_data.erase(std::remove_if(m_data.begin(), m_data.end(), Cis::CodeComparator(code)), m_data.end());
}

CalibrationFileVersion CalibrationFileImpl::getVersion() const
{
    return static_cast<CalibrationFileVersion>(getCis(cisCodeNonCisData).getData()[NonVolatileMemory::nvmVersionAddress]);
}

void CalibrationFileImpl::setVersion(CalibrationFileVersion version)
{
    getCis(cisCodeNonCisData).getData()[NonVolatileMemory::nvmVersionAddress] = static_cast<uint8_t>(version);
}

CalibrationFileSubversion CalibrationFileImpl::getSubversion() const
{
    CardInfo4thByte value { getCis(cisCodeCardInfo).getData()[cardInfoSubversionOffset] };
    return static_cast<CalibrationFileSubversion>(value.bitFields.subversion);
}

void CalibrationFileImpl::setSubversion(CalibrationFileSubversion subversion)
{
    CardInfo4thByte value { getCis(cisCodeCardInfo).getData()[cardInfoSubversionOffset] };
    value.bitFields.subversion = static_cast<uint8_t>(subversion);
    getCis(cisCodeCardInfo).getData()[cardInfoSubversionOffset] = value.byte;
}

void CalibrationFileImpl::getProductionFlag(bool& productionFlag)
{
    productionFlag = calibrationDataProductionValue == getCis(cisCodeNonCisData).getData()[calibrationDataProductionFlagOffset];
}

void CalibrationFileImpl::setProductionFlag(bool productionFlag)
{
    getCis(cisCodeNonCisData).getData()[calibrationDataProductionFlagOffset] = productionFlag ? calibrationDataProductionValue : calibrationDataOperationalValue;
}

uint8_t CalibrationFileImpl::getProductionFlagValue()
{
    return getCis(cisCodeNonCisData).getData()[calibrationDataProductionFlagOffset];
}

size_t CalibrationFileImpl::getProductionFlagOffset()
{
    return calibrationDataProductionFlagOffset;
}

void CalibrationFileImpl::getHardwareType(uint8_t& hardwareType)
{
    hardwareType = getCis(cisCodeCardInfo).getData()[cardInfoHardwareTypeOffset];
}

void CalibrationFileImpl::getHardwareRevision(uint8_t& hardwareRevision)
{
    hardwareRevision = getCis(cisCodeCardInfo).getData()[cardInfoHardwareRevisionOffset];
}

void CalibrationFileImpl::getCountryCode(uint8_t& countryCode)
{
    countryCode = getCis(cisCodeCardInfo).getData()[cardInfoCountryCodeOffset];
}

void CalibrationFileImpl::setCountryCode(uint8_t countryCode)
{
    getCis(cisCodeCardInfo).getData()[cardInfoCountryCodeOffset] = countryCode;
}

void CalibrationFileImpl::getMacAddress(std::array<uint8_t, cardInfoMacAddressSize>& macAddress)
{
    uint8_t* p = &getCis(cisCodeCardInfo).getData()[cardInfoMacAddressOffset];

    std::copy_n(p, cardInfoMacAddressSize, std::begin(macAddress));
}

void CalibrationFileImpl::setMacAddress(const std::array<uint8_t, cardInfoMacAddressSize>& macAddress)
{
    uint8_t* p = &getCis(cisCodeCardInfo).getData()[cardInfoMacAddressOffset];

    std::copy_n(std::begin(macAddress), cardInfoMacAddressSize, p);
}

void CalibrationFileImpl::getSerialNumber(std::array<uint8_t, cardInfoSerialNumberSize>& serialNumber)
{
    uint8_t* p = &getCis(cisCodeCardInfo).getData()[cardInfoSerialNumberOffset];

    std::copy_n(p, cardInfoSerialNumberSize, std::begin(serialNumber));
}

void CalibrationFileImpl::setSerialNumber(const std::array<uint8_t, cardInfoSerialNumberSize>& serialNumber)
{
    uint8_t* p = &getCis(cisCodeCardInfo).getData()[cardInfoSerialNumberOffset];

    std::copy_n(std::begin(serialNumber), cardInfoSerialNumberSize, p);
}

void CalibrationFileImpl::getWeek(uint8_t& week)
{
    week = getCis(cisCodeCardInfo).getData()[cardInfoWeekOffset];
}

void CalibrationFileImpl::setWeek(uint8_t week)
{
    getCis(cisCodeCardInfo).getData()[cardInfoWeekOffset] = week;
}

void CalibrationFileImpl::getYear(uint8_t& year)
{
    year = getCis(cisCodeCardInfo).getData()[cardInfoYearOffset];
}

void CalibrationFileImpl::setYear(uint8_t year)
{
    getCis(cisCodeCardInfo).getData()[cardInfoYearOffset] = year;
}

void CalibrationFileImpl::getXtalValue(uint16_t& xtalValue)
{
    memcpy(&xtalValue, getCis(cisCodeXtal).getData(), sizeof(uint16_t));
}

void CalibrationFileImpl::setXtalValue(uint16_t xtalValue)
{
    memcpy(getCis(cisCodeXtal).getData(), &xtalValue, sizeof(uint16_t));
}

void CalibrationFileImpl::getRssiCalibrationData(std::vector<std::shared_ptr<RssiCalibrationData>>& output)
{
    std::vector<Cis> cisList = getCisList(cisCodeRssi);

    for (const auto& cis : cisList) {
        if (cis.getLength() > 0) {
            size_t offset = 0;

            std::shared_ptr<RssiCalibrationData> rssiCalibrationData = createRssiCalibrationData();
            rssiCalibrationData->parse(cis.getData(), cis.getLength(), offset);

            output.push_back(rssiCalibrationData);
        }
    }
}

void CalibrationFileImpl::setRssiCalibrationData(const std::vector<std::shared_ptr<RssiCalibrationData>>& input)
{
    removeCis(cisCodeRssi);

    for (const auto& rssiCalibrationData : input) {
        auto buffer = std::make_shared<ResizableBuffer>();
        rssiCalibrationData->serialize(buffer);

        addCis(Cis(cisCodeRssi, static_cast<uint16_t>(buffer->length()), buffer->data()));
    }
}

void CalibrationFileImpl::getTssiCalibrationData(std::vector<std::shared_ptr<TssiCalibrationData>>& output)
{
    std::vector<Cis> cisList = getCisList(cisCodeTssi);

    for (const auto& cis : cisList) {
        size_t offset = 1;

        while (offset < cis.getLength()) {
            std::shared_ptr<TssiCalibrationData> tssiCalibrationData = createTssiCalibrationData();
            tssiCalibrationData->parse(cis.getData(), cis.getLength(), offset);

            output.push_back(tssiCalibrationData);
        }
    }

    // Order the output vector by channel number
    std::sort(output.begin(), output.end(), [](std::shared_ptr<TssiCalibrationData> first, std::shared_ptr<TssiCalibrationData> second) {
        return first->data().channel < second->data().channel;
    });
}

void CalibrationFileImpl::setTssiCalibrationData(const std::vector<std::shared_ptr<TssiCalibrationData>>& input)
{
    // Create a modifiable copy of the input vector
    std::vector<std::shared_ptr<TssiCalibrationDataVer6>> sortedInput;
    for (const auto& tssiCalibrationData : input) {
        sortedInput.push_back(std::static_pointer_cast<TssiCalibrationDataVer6>(tssiCalibrationData));
    }

    // Reorder the input vector copy according to antenna mask and number of regions
    std::sort(sortedInput.begin(), sortedInput.end(), [](std::shared_ptr<TssiCalibrationDataVer6> first, std::shared_ptr<TssiCalibrationDataVer6> second) {
        if (first->data().antennaMask < second->data().antennaMask) {
            return true;
        }

        if (first->data().antennaMask == second->data().antennaMask) {
            if (first->data().numRegions < second->data().numRegions) {
                return true;
            }

            if (first->data().numRegions == second->data().numRegions) {
                if (first->data().bw < second->data().bw) {
                    return true;
                }

                if ((first->data().bw == second->data().bw) && (first->data().channel < second->data().channel)) {
                    return true;
                }
            }
        }

        return false;
    });

    removeCis(cisCodeTssi);

    AntennaMask currentAntennaMask = 0;
    int currentNumRegions = 0;

    auto buffer = std::make_shared<ResizableBuffer>();

    auto addCalibrationData = [&](std::shared_ptr<ResizableBuffer> calibrationData) {
        if (calibrationData->length() > 0) {
            addCis(Cis(cisCodeTssi, static_cast<uint16_t>(calibrationData->length()), calibrationData->data()));
            calibrationData->clear();
        }
    };

    for (const auto& tssiCalibrationData : sortedInput) {

        // Need to separate to new CIS for changes in antenna mask or number of regions
        if ((currentAntennaMask != tssiCalibrationData->data().antennaMask) || (currentNumRegions != tssiCalibrationData->data().numRegions)) {
            addCalibrationData(buffer);

            currentAntennaMask = tssiCalibrationData->data().antennaMask;
            currentNumRegions = tssiCalibrationData->data().numRegions;
        }

        tssiCalibrationData->serialize(buffer);
    }

    addCalibrationData(buffer);
}

void CalibrationFileImpl::getTssiFreqs(uint8_t channel, Bandwidth bw, Band band, const std::vector<std::shared_ptr<TssiCalibrationData>>& data, std::vector<TssiFreq>& tssiFreqs)
{
    TssiFreq tpcFreq_low { INT32_MAX, nullptr };
    TssiFreq tpcFreq_high { INT32_MAX, nullptr };

    // Create a modifiable copy of the data vector
    std::vector<std::shared_ptr<TssiCalibrationDataVer6>> sortedData;
    for (const auto& tssiCalibrationData : data) {
        sortedData.push_back(std::static_pointer_cast<TssiCalibrationDataVer6>(tssiCalibrationData));
    }

    // Reorder the data vector copy according to band-bandwidth-channel (in ascending order)
    std::sort(sortedData.begin(), sortedData.end(), [](std::shared_ptr<TssiCalibrationDataVer6> first, std::shared_ptr<TssiCalibrationDataVer6> second) {
        if (first->data().band < second->data().band) {
            return true;
        }

        if (first->data().band == second->data().band) {
            if (first->data().bw < second->data().bw) {
                return true;
            }

            if ((first->data().bw == second->data().bw) && (first->data().channel < second->data().channel)) {
                return true;
            }
        }

        return false;
    });

    for (const auto& tssiCalibrationData : sortedData) {
        // Set higher channel from cal data in tpcFreq_high
        if ((tssiCalibrationData->data().band == band) && (tssiCalibrationData->data().bw == bw) && (tssiCalibrationData->data().channel > channel)) {
            tpcFreq_high = { std::abs(channel - tssiCalibrationData->data().channel), tssiCalibrationData };
            break;
        }
    }

    // Reverse data vector so channels are ordered in descending order
    std::reverse(sortedData.begin(), sortedData.end());

    for (const auto& tssiCalibrationData : sortedData) {
        // Set same or lower channel from cal data in tpcFreq_low
        if ((tssiCalibrationData->data().band == band) && (tssiCalibrationData->data().bw == bw) && (tssiCalibrationData->data().channel <= channel)) {
            tpcFreq_low = { std::abs(channel - tssiCalibrationData->data().channel), tssiCalibrationData };
            break;
        }
    }

    // Check if tpcFreq_low has valid data
    if (tpcFreq_low.distance != INT32_MAX) {
        // tpcFreq_low will be in first tpc result if its distance is same or lower than tpcFreq_high
        if (tpcFreq_low.distance <= tpcFreq_high.distance) {
            tssiFreqs.push_back(tpcFreq_low);
        } else {
            tssiFreqs.push_back(tpcFreq_low);
            tssiFreqs.push_back(tpcFreq_high);
        }
    }

    // Check if tpcFreq_high has valid data and don't set tpcFreq_high at all if we found the exact channel (distance is 0)
    if ((tpcFreq_high.distance != INT32_MAX) && (tpcFreq_low.distance != 0)) {
        tssiFreqs.push_back(tpcFreq_high);
    }

    if (tssiFreqs.size() == 3) {
        tssiFreqs.pop_back();
    }
}

std::unique_ptr<Buffer> CalibrationFileImpl::serialize(size_t size)
{
    auto buffer = std::make_unique<ResizableBuffer>(size);

    Cis nonCisData = getCis(cisCodeNonCisData);

    buffer->append(nonCisData.getData(), nonCisData.getLength());

    for (const auto& cis : m_data) {
        if (cis.getCode() != cisCodeNonCisData) {
            buffer->append(*cis.serialize());
        }
    }

    return buffer;
}

}
