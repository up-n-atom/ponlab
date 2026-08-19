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

#ifndef _DUT_CALIBRATION_FILE_H_
#define _DUT_CALIBRATION_FILE_H_

#include "dut/Buffer.h"
#include "dut/Types.h"

#include <array>
#include <memory>
#include <vector>

namespace dut {

class RssiCalibrationData;
class TssiCalibrationData;
struct TssiFreq;

class CalibrationFile {
public:
    virtual ~CalibrationFile() = default;

    virtual CalibrationFileVersion getVersion() const = 0;
    virtual void setVersion(CalibrationFileVersion version) = 0;

    virtual CalibrationFileSubversion getSubversion() const = 0;
    virtual void setSubversion(CalibrationFileSubversion subversion) = 0;

    virtual void getProductionFlag(bool& productionFlag) = 0;
    virtual void setProductionFlag(bool productionFlag) = 0;

    virtual uint8_t getProductionFlagValue() = 0;
    virtual size_t getProductionFlagOffset() = 0;

    virtual void getHardwareType(uint8_t& hardwareType) = 0;

    virtual void getHardwareRevision(uint8_t& hardwareRevision) = 0;

    virtual void getCountryCode(uint8_t& countryCode) = 0;
    virtual void setCountryCode(uint8_t countryCode) = 0;

    virtual void getMacAddress(std::array<uint8_t, cardInfoMacAddressSize>& macAddress) = 0;
    virtual void setMacAddress(const std::array<uint8_t, cardInfoMacAddressSize>& macAddress) = 0;

    virtual void getSerialNumber(std::array<uint8_t, cardInfoSerialNumberSize>& serialNumber) = 0;
    virtual void setSerialNumber(const std::array<uint8_t, cardInfoSerialNumberSize>& serialNumber) = 0;

    virtual void getWeek(uint8_t& week) = 0;
    virtual void setWeek(uint8_t week) = 0;

    virtual void getYear(uint8_t& year) = 0;
    virtual void setYear(uint8_t year) = 0;

    virtual void getXtalValue(uint16_t& xtalValue) = 0;
    virtual void setXtalValue(uint16_t xtalValue) = 0;

    virtual void getRssiCalibrationData(std::vector<std::shared_ptr<RssiCalibrationData>>& data) = 0;
    virtual void setRssiCalibrationData(const std::vector<std::shared_ptr<RssiCalibrationData>>& data) = 0;

    virtual void getTssiCalibrationData(std::vector<std::shared_ptr<TssiCalibrationData>>& data) = 0;
    virtual void setTssiCalibrationData(const std::vector<std::shared_ptr<TssiCalibrationData>>& data) = 0;

    virtual void getTssiFreqs(uint8_t channel, Bandwidth bw, Band band, const std::vector<std::shared_ptr<TssiCalibrationData>>& data, std::vector<TssiFreq>& tssiFreqs) = 0;

    virtual std::unique_ptr<Buffer> serialize(size_t size) = 0;
};
}

#endif
