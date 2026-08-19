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

#ifndef _DUT_CALIBRATION_FILE_IMPL_H_
#define _DUT_CALIBRATION_FILE_IMPL_H_

#include "CalibrationFile.h"

#include "Cis.h"

namespace dut {

class CalibrationFileImpl : public CalibrationFile {
public:
    static constexpr uint8_t cisCodeNonCisData = 0x00;
    static constexpr uint8_t cisCodeCardInfo = 0x60;
    static constexpr uint8_t cisCodeTssi = 0x61;
    static constexpr uint8_t cisCodeRssi = 0x63;
    static constexpr uint8_t cisCodeXtal = 0x64;

    CalibrationFileVersion getVersion() const override;
    void setVersion(CalibrationFileVersion version) override;

    CalibrationFileSubversion getSubversion() const override;
    void setSubversion(CalibrationFileSubversion subversion) override;

    void getProductionFlag(bool& productionFlag) override;
    void setProductionFlag(bool productionFlag) override;

    uint8_t getProductionFlagValue() override;
    size_t getProductionFlagOffset() override;

    void getHardwareType(uint8_t& hardwareType) override;

    void getHardwareRevision(uint8_t& hardwareRevision) override;

    void getCountryCode(uint8_t& countryCode) override;
    void setCountryCode(uint8_t countryCode) override;

    void getMacAddress(std::array<uint8_t, cardInfoMacAddressSize>& macAddress) override;
    void setMacAddress(const std::array<uint8_t, cardInfoMacAddressSize>& macAddress) override;

    void getSerialNumber(std::array<uint8_t, cardInfoSerialNumberSize>& serialNumber) override;
    void setSerialNumber(const std::array<uint8_t, cardInfoSerialNumberSize>& serialNumber) override;

    void getWeek(uint8_t& week) override;
    void setWeek(uint8_t week) override;

    void getYear(uint8_t& year) override;
    void setYear(uint8_t year) override;

    void getXtalValue(uint16_t& xtalValue) override;
    void setXtalValue(uint16_t xtalValue) override;

    void getRssiCalibrationData(std::vector<std::shared_ptr<RssiCalibrationData>>& data) override;
    void setRssiCalibrationData(const std::vector<std::shared_ptr<RssiCalibrationData>>& data) override;

    void getTssiCalibrationData(std::vector<std::shared_ptr<TssiCalibrationData>>& data) override;
    void setTssiCalibrationData(const std::vector<std::shared_ptr<TssiCalibrationData>>& data) override;

    void getTssiFreqs(uint8_t channel, Bandwidth bw, Band band, const std::vector<std::shared_ptr<TssiCalibrationData>>& data, std::vector<TssiFreq>& tssiFreqs) override;

    std::unique_ptr<Buffer> serialize(size_t size) override;

    virtual std::unique_ptr<RssiCalibrationData> createRssiCalibrationData() = 0;
    virtual std::unique_ptr<TssiCalibrationData> createTssiCalibrationData() = 0;

protected:
    CalibrationFileImpl(const uint8_t* data, size_t length);

    void addCis(const Cis& cis);
    Cis getCis(uint8_t code) const;
    std::vector<Cis> getCisList(uint8_t code) const;
    void removeCis(uint8_t code);

private:
    std::vector<Cis> m_data;
};
}

#endif
