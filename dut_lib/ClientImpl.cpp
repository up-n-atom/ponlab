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

#include "dut/ClientImpl.h"

#include "CyclicPrefixMode.h"
#include "DutDriverFwGeneralMsg.h"
#include "DutMessage.h"
#include "Exceptions.h"
#include "Payload.h"
#include "Protocol.h"
#include "RawMessage.h"
#include "SharedHeaders.h"
#include "dut/AntennaMask.h"
#include "dut/Logger.h"
#include "dut/Tools.h"

#include <stdexcept>
#include <string>
#ifdef LINUX_HOST
#include <cstring>
#define _countof(a) (sizeof(a) / sizeof(*(a)))
#include <iostream>
#endif

namespace dut {

constexpr uint8_t txVoltageNumSamples = 5;

/** 
ReceiveTimeout: Set the timeout for synchronous receive methods
- Increase up to 3 seconds due to the case of multiple slave VAPs in which FW would spending 2.x seconds on handling RESET MAC at first connect.
- Increase up to 5 seconds due to timeout during calibration on Windows7 setup.
- Increase up to 10 seconds as a workaround for stability issues.
- Increase up to 22 seconds to align to driver-FW 20sec timeout.
*/
constexpr auto timeout = std::chrono::milliseconds(22000);

static uint8_t convertDutPhy_to_FwPhy(PhyMode phyMode)
{
    switch (phyMode) {
    case PhyMode::PHY_MODE_A:
    case PhyMode::PHY_MODE_G:
        return 0;

    case PhyMode::PHY_MODE_B:
        return 1;

    case PhyMode::PHY_MODE_N_5:
    case PhyMode::PHY_MODE_N_2_4:
        return 2;

    case PhyMode::PHY_MODE_AC:
        return 3;

    case PhyMode::PHY_MODE_AX:
        return 4; //single user Ext.

    case PhyMode::PHY_MODE_BE:
        return 8; //single user Ext.
    }

    throw std::invalid_argument("Invalid PHY mode (" + toString(phyMode) + ")");
}

static uint8_t convertDutPhy_to_PhyProtocol(PhyMode phyMode)
{
    switch (phyMode) {
    case PhyMode::PHY_MODE_A:
    case PhyMode::PHY_MODE_G:
        return 0;
    case PhyMode::PHY_MODE_N_5:
    case PhyMode::PHY_MODE_N_2_4:
        return 1;
    case PhyMode::PHY_MODE_B:
        return 2;

    case PhyMode::PHY_MODE_AC:
        return 3;

    case PhyMode::PHY_MODE_AX:
        return 4;

    case PhyMode::PHY_MODE_BE:
        return 5;
    }

    throw std::invalid_argument("Invalid PHY mode (" + toString(phyMode) + ")");
}

static uint8_t getTxRate(PhyMode phyMode, uint8_t mcsIndex, uint8_t nss)
{
    switch (phyMode) {
    case PhyMode::PHY_MODE_A:
    case PhyMode::PHY_MODE_B:
    case PhyMode::PHY_MODE_G:
    case PhyMode::PHY_MODE_N_2_4:
    case PhyMode::PHY_MODE_N_5:
        return mcsIndex;
    case PhyMode::PHY_MODE_AC:
    case PhyMode::PHY_MODE_AX:
    case PhyMode::PHY_MODE_BE:
        //Reduce 1 from SS to build txRate:
        uint8_t _nss = nss - 1;
        return static_cast<uint8_t>(mcsIndex | (_nss << 4));
    }

    throw std::invalid_argument("Unsupported PHY mode");
}

ClientImpl::ClientImpl(uint8_t wlanIndex, std::shared_ptr<Connection> connection, std::shared_ptr<Logger> logger)
    : m_wlanIndex(wlanIndex)
    , m_messenger(connection, logger)
    , m_logger(logger)
{
    if (!connection) {
        throw std::invalid_argument("Parameter 'connection' cannot be a null pointer");
    }
}

void ClientImpl::exchangeMessage(uint8_t wlanIndex, uint8_t msgId, uint32_t requestId, uint32_t responseId, Message& message, const std::chrono::milliseconds& receiveTimeout) const
{
    message.setMsgId(requestId);

    // We expect a response message with the same length as the request.
    // See WLANRTSYS-34708: "DUT request messages contain more bytes than necessary"
    size_t expectedLength = message.getHeaderLength() + message.getPayloadLength();

    // Parameter 'message' is an input and output parameter that contains both the request (on input) and the response (on output)
    m_messenger.sendReceive(wlanIndex, msgId, message.data(), message.length(), message.data(), message.length(), receiveTimeout);

    // If no receive timeout was specified is because no response was expected from the firmware and therefore no checks have to be performed
    if (receiveTimeout != std::chrono::milliseconds::zero()) {
        if (message.length() < expectedLength) {
            throw std::runtime_error("Invalid response received: response length (" + toString(message.length()) + ") is too small (expected length is " + toString(expectedLength) + ")");
        }

        /* TODO: Remove comparison of msgId to DUT_SERVER_MSG_DRIVER_FW_GENERAL after WLANRTSYS-37326 is fixed */
        if ((message.getMsgId() != responseId) && (msgId != dutDriverMessagesId_e::DUT_SERVER_MSG_DRIVER_FW_GENERAL)) {
            throw std::runtime_error("Invalid response received for message ID " + toHexString(msgId) + " and request ID " + toHexString(requestId) + ": expected response ID is " + toHexString(responseId) + ", received response ID is " + toHexString(message.getMsgId()));
        }
    }
}

void ClientImpl::addVap(uint8_t vapIndex, bool snifferMode)
{
    // Add VAP
    {
        UMI_ADD_VAP umiAddVap {};

        umiAddVap.vapId = vapIndex;
        umiAddVap.operationMode = static_cast<uint8_t>(snifferMode ? UmiOperationMode_e::OPERATION_MODE_SNIFFER : UmiOperationMode_e::OPERATION_MODE_DUT);

        Payload payload(reinterpret_cast<uint8_t*>(&umiAddVap), sizeof(umiAddVap));
        DutDriverFwGeneralMsg message(payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_DRIVER_FW_GENERAL,
            UM_MAN_ADD_VAP_REQ,
            MC_MAN_ADD_VAP_CFM,
            message,
            timeout);
    }

    // Set BSS
    {
        UMI_SET_BSS umiSetBss {};

        umiSetBss.vapId = vapIndex;
        umiSetBss.protectionMode = 0;
        umiSetBss.slotTime = 1;
        umiSetBss.u8Rates[0] = 2;
        umiSetBss.u8Rates[1] = 4;
        umiSetBss.u8Rates[2] = 11;
        umiSetBss.u8Rates[3] = 22;
        umiSetBss.u8Rates_Length = 4;
        umiSetBss.u8HT_MCS_Set[0] = 0xff;
        umiSetBss.u8HT_MCS_Set[1] = 0xff;
        umiSetBss.twtOperationMode = 0;
        umiSetBss.fixedMcsVapManagement = 0xff;

        Payload payload(reinterpret_cast<uint8_t*>(&umiSetBss), sizeof(umiSetBss));
        DutDriverFwGeneralMsg message(payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_DRIVER_FW_GENERAL,
            UM_BSS_SET_BSS_REQ,
            MC_BSS_SET_BSS_CFM,
            message,
            timeout);
    }

    // Set priorities (WMM)
    {
        UMI_SET_WMM_PARAMETERS umiSetWmmParameters {};

        umiSetWmmParameters.vapId = vapIndex;

        for (auto& params : umiSetWmmParameters.wmmParamsPerAc) {
            params.u16CWmin = 0;
            params.u16CWmax = 0;
            params.u16TXOPlimit = 0;
            params.u8Aifsn = 1;
            params.acm_flag = 0;
        }

        Payload payload(reinterpret_cast<uint8_t*>(&umiSetWmmParameters), sizeof(umiSetWmmParameters));
        DutDriverFwGeneralMsg message(payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_DRIVER_FW_GENERAL,
            UM_MAN_SET_WMM_PARAMETERS_REQ,
            MC_MAN_SET_WMM_PARAMETERS_CFM,
            message,
            timeout);
    }
}

void ClientImpl::exchangeMessage(packetCountersParams_t& packetCountersParams)
{
    Payload payload(reinterpret_cast<uint8_t*>(&packetCountersParams), sizeof(packetCountersParams));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_READ_PACKET_COUNTERS_REQ,
        dutMessagesId_e::DUT_READ_PACKET_COUNTERS_CFM,
        message,
        timeout);
}

void ClientImpl::flushNvMemory(ChipID chipId, NvMemoryType memoryType, FileType fileType)
{
    dutNvMemoryFlushParams_t nvMemoryFlushParams {};

    nvMemoryFlushParams.nvMemoryType = static_cast<uint32_t>(memoryType);
    nvMemoryFlushParams.fileType = static_cast<uint32_t>(fileType);
    nvMemoryFlushParams.verifyNvmData = 1;

    // B0 & D2
    {
        Payload payload(reinterpret_cast<uint8_t*>(&nvMemoryFlushParams), sizeof(nvMemoryFlushParams));
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_DRIVER_GENERAL,
            dutDriverGeneralMsgId_e::DUT_DGM_FLUSH_NV_MEMORY_REQ,
            dutDriverGeneralMsgId_e::DUT_DGM_FLUSH_NV_MEMORY_CFM,
            message,
            timeout);
    }

    if (chipId > ChipID::CHIP_ID_GEN6_B) {
        Payload payload(reinterpret_cast<uint8_t*>(&nvMemoryFlushParams), sizeof(nvMemoryFlushParams));
        DutMessage message(m_wlanIndex, payload);

        // TODO: firmware should respond to this request. Otherwise the library cannot be sure if the command succeeded (see WLANRTSYS-37327).
        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_DRIVER_GENERAL,
            dutDriverGeneralMsgId_e::DUT_DGM_RECOVERY_REQ,
            0,
            message,
            std::chrono::milliseconds::zero());
    }
}

void ClientImpl::getBbicCddValues(uint8_t numTxAntennas, uint32_t& offset1, uint32_t& offset2, uint32_t& offset3)
{
    dutGetBbicCdd_t getBbicCdd {};

    getBbicCdd.antNum = numTxAntennas;

    // Payload is updated in the destructor of the message (i.e.: when the variable 'message' goes out of scope)
    {
        Payload payload(reinterpret_cast<uint8_t*>(&getBbicCdd), sizeof(getBbicCdd));
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
            dutMessagesId_e::DUT_GET_BBIC_CDD_REQ,
            dutMessagesId_e::DUT_GET_BBIC_CDD_CFM,
            message,
            timeout);
    }

    offset1 = getBbicCdd.offset1_rtrn;
    offset2 = getBbicCdd.offset2_rtrn;
    offset3 = getBbicCdd.offset3_rtrn;
}

void ClientImpl::getCombinedVersion(std::array<uint8_t, combinedVersionSize>& buffer)
{
    Payload payload(buffer.data(), buffer.size());
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_PLATFORM_GENERAL,
        dutPlatformGeneralMsgId_e::DUT_PGM_GET_CV_REQ,
        dutPlatformGeneralMsgId_e::DUT_PGM_GET_CV_CFM,
        message,
        timeout);
}

void ClientImpl::getFemType(FemType& femType)
{
    FemType_t _femType {};

    // Payload is updated in the destructor of the message (i.e.: when the variable 'message' goes out of scope)
    {
        Payload payload(reinterpret_cast<uint8_t*>(&_femType), sizeof(_femType));
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
            dutMessagesId_e::DUT_GET_FEM_TYPE_REQ,
            dutMessagesId_e::DUT_GET_FEM_TYPE_CFM,
            message,
            timeout);
    }

    femType = static_cast<FemType>(_femType.type);
}

void ClientImpl::getFwInfo(struct dutGetFwInfo_tag& fwInfo)
{
    Payload payload(reinterpret_cast<uint8_t*>(&fwInfo), sizeof(fwInfo));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_GET_FW_INFO_REQ,
        dutMessagesId_e::DUT_GET_FW_INFO_CFM,
        message,
        timeout);
}

void ClientImpl::getInbandRssi(const AntennaMask& antennaMask, std::array<int16_t, maxNumRxAntennas>& rssi, uint16_t& numSamples)
{
    dutRssiParams_t rssiParams {};

    // Payload is updated in the destructor of the message (i.e.: when the variable 'message' goes out of scope)
    {
        Payload payload(reinterpret_cast<uint8_t*>(&rssiParams), sizeof(rssiParams));
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
            dutMessagesId_e::DUT_GET_RSSI_REQ,
            dutMessagesId_e::DUT_GET_RSSI_CFM,
            message,
            timeout);
    }

    numSamples = rssiParams.N;

    for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
        if (antennaMask.isBitSet(antenna)) {
            int16_t tmp = rssiParams.rssi[antenna];
            if (tmp >= 128) {
                tmp -= 256;
            }
            if (tmp == -128) {
                tmp = INT16_MIN;
            }
            rssi[antenna] = tmp;
        } else {
            rssi[antenna] = 0;
        }
    }
}

void ClientImpl::getMacPacketCounters(uint32_t& receivedPackets)
{
    Payload payload(reinterpret_cast<uint8_t*>(&receivedPackets), sizeof(receivedPackets));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_GET_RX_COUNTER_REQ,
        dutMessagesId_e::DUT_GET_RX_COUNTER_CFM,
        message,
        timeout);
}

void ClientImpl::getMpduPacketCounters(uint32_t& receivedPackets, uint32_t& errorPackets)
{
    packetCountersParams_t packetCountersParams {};
    packetCountersParams.doReset = 0;
    packetCountersParams.type = 1; // MPDU

    exchangeMessage(packetCountersParams);

    receivedPackets = packetCountersParams.rxPacketCounter;
    errorPackets = packetCountersParams.rxPacketErrorCounter;
}

void ClientImpl::getNmseValues(std::array<std::array<int32_t, dut::dpdTotalCalibrationPoints>, dut::maxNumTxAntennas>& values)
{
    dpdNmseMeasures_t nmseMeasures {};

    // Payload is updated in the destructor of the message (i.e.: when the variable 'message' goes out of scope)
    {
        Payload payload(reinterpret_cast<uint8_t*>(&nmseMeasures), sizeof(nmseMeasures));
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
            dutMessagesId_e::DUT_GET_DPD_NMSE_MEASURES_REQ,
            dutMessagesId_e::DUT_GET_DPD_NMSE_MEASURES_CFM,
            message,
            timeout);
    }

    for (size_t i = 0; i < dut::maxNumTxAntennas; i++) {
        for (size_t j = 0; j < dut::dpdTotalCalibrationPoints; j++) {
            values[i][j] = nmseMeasures.values[i][j];
        }
    }
}

void ClientImpl::getPhyPacketCounters(uint32_t& receivedPackets, uint32_t& crcErrors, uint32_t& forwardedPackets)
{
    packetCountersParams_t packetCountersParams {};

    packetCountersParams.doReset = 0;
    packetCountersParams.type = 0; // PHY

    exchangeMessage(packetCountersParams);

    receivedPackets = packetCountersParams.rxPacketCounter;
    crcErrors = packetCountersParams.rxPacketErrorCounter;
    forwardedPackets = packetCountersParams.fwPacketCounter;
}

void ClientImpl::getRfRssiPower(const AntennaMask& antennaMask, std::array<int8_t, maxNumRxAntennas>& power)
{
    RfRssiPower_t rfRssiPower {};

    rfRssiPower.antMask = antennaMask;

    // Payload is updated in the destructor of the message (i.e.: when the variable 'message' goes out of scope)
    {
        Payload payload(reinterpret_cast<uint8_t*>(&rfRssiPower), sizeof(rfRssiPower));
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
            dutMessagesId_e::DUT_GET_RF_RSSI_POWER_REQ,
            dutMessagesId_e::DUT_GET_RF_RSSI_POWER_CFM,
            message,
            timeout);
    }

    for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
        if (antennaMask.isBitSet(antenna)) {
            // Convert 0.5 dB to dB
            auto from_half_dB_to_dB = [](int16_t value) {
                int16_t result = value;

                if (value > 255) {
                    result = value - 512;
                }
                result = result / 2;

                return result;
            };

            power[antenna] = static_cast<int8_t>(from_half_dB_to_dB(rfRssiPower.power[antenna]));
        } else {
            power[antenna] = 0;
        }
    }
}

void ClientImpl::getRssi(uint8_t method, uint16_t numSamples, const AntennaMask& antennaMask, std::array<uint16_t, maxNumRxAntennas>& rssi)
{
    DutRssiOffsetGain_t rssiOffsetGain {};

    rssiOffsetGain.Method = method;
    rssiOffsetGain.NOS = numSamples;
    rssiOffsetGain.antMask = antennaMask;

    // Payload is updated in the destructor of the message (i.e.: when the variable 'message' goes out of scope)
    {
        Payload payload(reinterpret_cast<uint8_t*>(&rssiOffsetGain), sizeof(rssiOffsetGain));
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
            dutMessagesId_e::DUT_GET_AUX_ADC_RSSI_REQ,
            dutMessagesId_e::DUT_GET_AUX_ADC_RSSI_CFM,
            message,
            timeout);
    }

    for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
        if (antennaMask.isBitSet(antenna)) {
            rssi[antenna] = rssiOffsetGain.rssiResult[antenna];
        } else {
            rssi[antenna] = 0;
        }
    }
}

void ClientImpl::getRxCwPower(uint8_t method, int32_t freqOffset, uint16_t numSamples, const AntennaMask& antennaMask, std::array<CorrelationResults_t, maxNumRxAntennas>& results)
{
    RssiCwPower_t rssiCwPower {};

    rssiCwPower.method = method;
    rssiCwPower.FreqOffset = freqOffset;
    rssiCwPower.NOS = numSamples;
    rssiCwPower.antMask = antennaMask;

    // Payload is updated in the destructor of the message (i.e.: when the variable 'message' goes out of scope)
    {
        Payload payload(reinterpret_cast<uint8_t*>(&rssiCwPower), sizeof(rssiCwPower));
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
            dutMessagesId_e::DUT_GET_CW_POWER_REQ,
            dutMessagesId_e::DUT_GET_CW_POWER_CFM,
            message,
            timeout);
    }

    for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
        if (antennaMask.isBitSet(antenna)) {
            results[antenna].II = rssiCwPower.corrResults[antenna].II;
            results[antenna].QQ = rssiCwPower.corrResults[antenna].QQ;
            results[antenna].IQ = rssiCwPower.corrResults[antenna].IQ;
        } else {
            results[antenna].II = 0;
            results[antenna].QQ = 0;
            results[antenna].IQ = 0;
        }
    }
}

void ClientImpl::getRxEvm(std::array<uint8_t, maxNumRxAntennas>& rxEvm)
{
    dutGetRxEvm_t getRxEvm {};

    // Payload is updated in the destructor of the message (i.e.: when the variable 'message' goes out of scope)
    {
        Payload payload(reinterpret_cast<uint8_t*>(&getRxEvm), sizeof(getRxEvm));
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
            dutMessagesId_e::DUT_GET_RX_EVM_REQ,
            dutMessagesId_e::DUT_GET_RX_EVM_CFM,
            message,
            timeout);
    }

    for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
        rxEvm[antenna] = getRxEvm.rxEvm[antenna];
    }
}

void ClientImpl::getRxRateInfo(uint8_t& mcs, uint8_t& nss)
{
    dutGetRxRate_t rxRate {};

    // Payload is updated in the destructor of the message (i.e.: when the variable 'message' goes out of scope)
    {
        Payload payload(reinterpret_cast<uint8_t*>(&rxRate), sizeof(rxRate));
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
            dutMessagesId_e::DUT_GET_RX_RATE_REQ,
            dutMessagesId_e::DUT_GET_RX_RATE_CFM,
            message,
            timeout);
    }

    mcs = rxRate.mcs;
    nss = rxRate.nss;
}

void ClientImpl::getTemperature(float& temperature)
{
    dutGetTemperature_t dutGetTemperature {};

    // Payload is updated in the destructor of the message (i.e.: when the variable 'message' goes out of scope)
    {
        Payload payload(reinterpret_cast<uint8_t*>(&dutGetTemperature), sizeof(dutGetTemperature));
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
            DUT_GET_TEMPERATURE_SENSOR_REQ,
            DUT_GET_TEMPERATURE_SENSOR_CFM,
            message,
            timeout);
    }

    temperature = static_cast<float>(dutGetTemperature.inttempRes);
    temperature += static_cast<float>(dutGetTemperature.dectempRes) / 100.0f;
}

void ClientImpl::getTransmitPowerTableOffset(uint8_t antenna, Bandwidth bandwidth, int16_t& offset)
{
    dutPoutTableOffsetParams_t poutTableOffsetParams {};

    poutTableOffsetParams.antNum = antenna;
    poutTableOffsetParams.bw = static_cast<uint8_t>(bandwidth);

    // Payload is updated in the destructor of the message (i.e.: when the variable 'message' goes out of scope)
    {
        Payload payload(reinterpret_cast<uint8_t*>(&poutTableOffsetParams), sizeof(poutTableOffsetParams));
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
            dutMessagesId_e::DUT_GET_POUT_TABLE_OFFSET_REQ,
            dutMessagesId_e::DUT_GET_POUT_TABLE_OFFSET_CFM,
            message,
            timeout);
    }

    offset = poutTableOffsetParams.tableOffset;
}

void ClientImpl::getTransmitVoltages(const AntennaMask& antennaMask, std::array<uint32_t, maxNumTxAntennas>& voltages)
{
    dutTpcFeedbackparams_t tpcFeedbackParams {};

    tpcFeedbackParams.numOfSamples = txVoltageNumSamples;

    // Payload is updated in the destructor of the message (i.e.: when the variable 'message' goes out of scope)
    {
        Payload payload(reinterpret_cast<uint8_t*>(&tpcFeedbackParams), sizeof(tpcFeedbackParams));
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
            dutMessagesId_e::DUT_GET_TX_POWER_FEEDBACK_REQ,
            dutMessagesId_e::DUT_GET_TX_POWER_FEEDBACK_CFM,
            message,
            timeout);
    }

    voltages = {};
    for (uint8_t antenna = 0; antenna < maxNumTxAntennas; antenna++) {
        if (!antennaMask.isBitSet(antenna)) {
            continue;
        }
        if (tpcFeedbackParams.actualNumOfSamples > 0) {
            voltages[antenna] = (tpcFeedbackParams.valueSum[antenna] / tpcFeedbackParams.actualNumOfSamples);
        }
    }
}

void ClientImpl::getXtalValue(uint16_t& xtalValue)
{
    dutXtalParams_t xtalParams {};

    // Payload is updated in the destructor of the message (i.e.: when the variable 'message' goes out of scope)
    {
        Payload payload(reinterpret_cast<uint8_t*>(&xtalParams), sizeof(xtalParams));
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
            dutMessagesId_e::DUT_GET_XTAL_VALUE_REQ,
            dutMessagesId_e::DUT_GET_XTAL_VALUE_CFM,
            message,
            timeout);
    }

    xtalValue = static_cast<uint16_t>(xtalParams.xtalValue);
}

void ClientImpl::readChipId(ChipID& chipId)
{
    dutChipVersion_t chipVersion {};

    // Payload is updated in the destructor of the message (i.e.: when the variable 'message' goes out of scope)
    {
        Payload payload(reinterpret_cast<uint8_t*>(&chipVersion), sizeof(chipVersion));
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
            dutMessagesId_e::DUT_READ_CHIP_VERSION_REQ,
            dutMessagesId_e::DUT_READ_CHIP_VERSION_CFM,
            message,
            timeout);
    }

    chipId = static_cast<ChipID>(chipVersion.bbChipId);
}

void ClientImpl::readMemory(ChipModule chipModule, size_t address, uint8_t* data, size_t length)
{
    dutMemoryAccessParams_t memoryAccessParams {};

    if (!data) {
        throw std::invalid_argument("Parameter 'data' cannot be a null pointer");
    }

    if (length == 0) {
        throw std::invalid_argument("Parameter 'length' cannot be 0");
    }

    if (length > maxMemoryAccessLength) {
        throw std::invalid_argument("The specified length (" + toString(length) + ") is too large (" + toString(maxMemoryAccessLength) + " bytes allowed)");
    }

    memoryAccessParams.module = static_cast<uint16_t>(chipModule);
    memoryAccessParams.address = static_cast<uint32_t>(address);
    memoryAccessParams.length = static_cast<uint16_t>(length);

    // Payload is updated in the destructor of the message (i.e.: when the variable 'message' goes out of scope)
    {
        size_t headerLength = reinterpret_cast<uint8_t*>(&(memoryAccessParams.data)) - reinterpret_cast<uint8_t*>(&memoryAccessParams);

        Payload payload(reinterpret_cast<uint8_t*>(&memoryAccessParams), headerLength + length);
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
            dutMessagesId_e::DUT_READ_MEMORY_REQ,
            dutMessagesId_e::DUT_READ_MEMORY_CFM,
            message,
            timeout);
    }

    memcpy(data, memoryAccessParams.data, length);
}

void ClientImpl::readNvMemory(size_t address, uint8_t* data, size_t length, NvMemoryType memoryType, NvMemorySize memorySize, FileType fileType)
{
    dutNvMemoryAccessParams_t nvMemoryAccessParams {};

    if (!data) {
        throw std::invalid_argument("Parameter 'data' cannot be a null pointer");
    }

    if (length == 0) {
        throw std::invalid_argument("Parameter 'length' cannot be 0");
    }

    if (length > maxNvMemoryAccessLength) {
        throw std::invalid_argument("The specified length (" + toString(length) + ") is too large (" + toString(maxNvMemoryAccessLength) + " bytes allowed)");
    }

    nvMemoryAccessParams.nvMemoryType = static_cast<uint32_t>(memoryType);
    nvMemoryAccessParams.address = static_cast<uint32_t>(address);
    nvMemoryAccessParams.eepromSize = static_cast<uint32_t>(memorySize);
    nvMemoryAccessParams.length = static_cast<uint32_t>(length);
    nvMemoryAccessParams.fileType = static_cast<uint32_t>(fileType);

    // Payload is updated in the destructor of the message (i.e.: when the variable 'message' goes out of scope)
    {
        size_t headerLength = reinterpret_cast<uint8_t*>(&(nvMemoryAccessParams.data)) - reinterpret_cast<uint8_t*>(&nvMemoryAccessParams);

        Payload payload(reinterpret_cast<uint8_t*>(&nvMemoryAccessParams), headerLength + length);
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_DRIVER_GENERAL,
            dutDriverGeneralMsgId_e::DUT_DGM_READ_NV_MEMORY_REQ,
            dutDriverGeneralMsgId_e::DUT_DGM_READ_NV_MEMORY_CFM,
            message,
            timeout);
    }

    memcpy(data, nvMemoryAccessParams.data, length);
}

void ClientImpl::removeVap(uint8_t vapIndex)
{
    // Stop VAP traffic
    {
        UMI_STOP_VAP_TRAFFIC umiStopVapTraffic {};

        umiStopVapTraffic.vapId = vapIndex;

        Payload payload(reinterpret_cast<uint8_t*>(&umiStopVapTraffic), sizeof(umiStopVapTraffic));
        DutDriverFwGeneralMsg message(payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_DRIVER_FW_GENERAL,
            UM_MAN_STOP_VAP_TRAFFIC_REQ,
            MC_MAN_STOP_VAP_TRAFFIC_CFM,
            message,
            timeout);
    }

    // Remove VAP
    {
        UMI_REMOVE_VAP umiRemoveVap {};

        umiRemoveVap.vapId = vapIndex;

        Payload payload(reinterpret_cast<uint8_t*>(&umiRemoveVap), sizeof(umiRemoveVap));
        DutDriverFwGeneralMsg message(payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_DRIVER_FW_GENERAL,
            UM_MAN_REMOVE_VAP_REQ,
            MC_MAN_REMOVE_VAP_CFM,
            message,
            timeout);
    }
}

void ClientImpl::resetMac(NvMemoryType memoryType, NvMemorySize memorySize, bool reset)
{
    dutResetParams_t resetParams {};

    resetParams.nvMemoryType = static_cast<uint32_t>(memoryType);
    resetParams.eepromSize = static_cast<uint32_t>(memorySize);
    resetParams.doReset = reset ? 1 : 0;

    Payload payload(reinterpret_cast<uint8_t*>(&resetParams), sizeof(resetParams));
    RawMessage message(payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_RESET_MAC,
        0,
        0,
        message,
        timeout);
}

void ClientImpl::resetMacPacketCounters()
{
    uint32_t receivedPackets = 0;

    Payload payload(reinterpret_cast<uint8_t*>(&receivedPackets), sizeof(receivedPackets));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_SET_RX_COUNTER_REQ,
        dutMessagesId_e::DUT_SET_RX_COUNTER_CFM,
        message,
        timeout);
}

void ClientImpl::resetMpduPacketCounters()
{
    packetCountersParams_t packetCountersParams {};

    packetCountersParams.doReset = 1;
    packetCountersParams.type = 1; // MPDU

    exchangeMessage(packetCountersParams);
}

void ClientImpl::resetPhyPacketCounters()
{
    packetCountersParams_t packetCountersParams {};

    packetCountersParams.doReset = 1;
    packetCountersParams.type = 0; // PHY

    exchangeMessage(packetCountersParams);
}

void ClientImpl::rxMeasure(uint32_t numCaptures, uint32_t captureInterval, bool disabled, PhyMode phyMode)
{
    dutRxMeasureParams_t rxMeasure {};
    rxMeasure.numCaptures = numCaptures;
    rxMeasure.interval = captureInterval * 1000; // Convert to usec
    rxMeasure.protocol = convertDutPhy_to_PhyProtocol(phyMode);
    rxMeasure.enabled = !disabled;

    Payload payload(reinterpret_cast<uint8_t*>(&rxMeasure), sizeof(rxMeasure));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_RX_MEASURE_PARAMS_REQ,
        dutMessagesId_e::DUT_RX_MEASURE_PARAMS_CFM,
        message,
        timeout);
}

void ClientImpl::setBand(Band band)
{
    dutChangeBand_t changeBand {};

    changeBand.changeToRadio = 0;
    if (Band::BAND_5000MHZ == band) {
        changeBand.changeToRadio = 1;
    } else if (Band::BAND_6000MHZ == band) {
        changeBand.changeToRadio = 2;
    }

    Payload payload(reinterpret_cast<uint8_t*>(&changeBand), sizeof(changeBand));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_DRIVER_GENERAL,
        dutDriverGeneralMsgId_e::DUT_DGM_CHANGE_BAND_REQ,
        dutDriverGeneralMsgId_e::DUT_DGM_CHANGE_BAND_CFM,
        message,
        timeout);
}

void ClientImpl::setBbicCddValues(uint8_t numTxAntennas, uint32_t offset1, uint32_t offset2, uint32_t offset3)
{
    dutSetBbicCdd_t setBbicCdd {};

    setBbicCdd.antNum = numTxAntennas;
    setBbicCdd.offset1_rtrn = offset1;
    setBbicCdd.offset2_rtrn = offset2;
    setBbicCdd.offset3_rtrn = offset3;

    Payload payload(reinterpret_cast<uint8_t*>(&setBbicCdd), sizeof(setBbicCdd));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_SET_BBIC_CDD_REQ,
        dutMessagesId_e::DUT_SET_BBIC_CDD_CFM,
        message,
        timeout);
}

void ClientImpl::setChannel(uint8_t lowestChannel, uint8_t primaryChannelIndex, Bandwidth spectrumBandwidth, const AntennaMask& txAntennaMask, RegulationType regulationType)
{
    UM_SET_CHAN_PARAMS umSetChanParams {};

    umSetChanParams.low_chan_num = lowestChannel;
    umSetChanParams.low_chan_num2 = 0;
    umSetChanParams.chan_width = static_cast<uint8_t>(spectrumBandwidth);
    umSetChanParams.primary_chan_idx = primaryChannelIndex;
    umSetChanParams.switch_type = ST_NORMAL; // 0 = "normal"
    umSetChanParams.antennaMask = txAntennaMask;
    umSetChanParams.ChannelNotificationMode = ChannelNotificationMode_e::CHANNEL_NOTIFICATION_NORMAL;
    umSetChanParams.RegulationType = static_cast<uint8_t>(regulationType);

    Payload payload(reinterpret_cast<uint8_t*>(&umSetChanParams), sizeof(umSetChanParams));
    DutDriverFwGeneralMsg message(payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_DRIVER_FW_GENERAL,
        UM_SET_CHAN_REQ,
        UM_SET_CHAN_CFM,
        message,
        timeout);
}

void ClientImpl::setClipper(bool enabled)
{
    dutSetClipperParams_t setClipperParams {};

    setClipperParams.enabled = enabled;

    Payload payload(reinterpret_cast<uint8_t*>(&setClipperParams), sizeof(setClipperParams));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_SET_CLIPPER_REQ,
        dutMessagesId_e::DUT_SET_CLIPPER_CFM,
        message,
        timeout);
}

void ClientImpl::setEnabledRxAntennas(const AntennaMask& antennaMask)
{
    dutSetAntennaParams_t setAntennaParams {};

    setAntennaParams.enabledAntMask = antennaMask;

    Payload payload(reinterpret_cast<uint8_t*>(&setAntennaParams), sizeof(setAntennaParams));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_ENABLE_RX_ANTENNA_REQ,
        dutMessagesId_e::DUT_ENABLE_RX_ANTENNA_CFM,
        message,
        timeout);
}

void ClientImpl::setEnabledTxAntennas(const AntennaMask& antennaMask)
{
    dutSetAntennaParams_t setAntennaParams {};

    setAntennaParams.enabledAntMask = antennaMask;

    Payload payload(reinterpret_cast<uint8_t*>(&setAntennaParams), sizeof(setAntennaParams));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_ENABLE_TX_ANTENNA_REQ,
        dutMessagesId_e::DUT_ENABLE_TX_ANTENNA_CFM,
        message,
        timeout);
}

void ClientImpl::setHdkConfig(uint32_t offlineCalMask, uint32_t onlineCalMask, Band band)
{
    UMI_HDK_CONFIG umiHdkConfig {};

    umiHdkConfig.calibrationBufferBaseAddress = 0;
    umiHdkConfig.getSetOperation = API_SET_OPERATION;
    umiHdkConfig.setChannelMode = 0;
    umiHdkConfig.hdkConf.numTxAnts = 0;
    umiHdkConfig.hdkConf.numRxAnts = 0;
    umiHdkConfig.hdkConf.eepromInfo.u16EEPROMVersion = 0;
    umiHdkConfig.hdkConf.eepromInfo.u8NumberOfPoints2GHz = 0;
    umiHdkConfig.hdkConf.eepromInfo.u8NumberOfPoints5GHz = 0;

    if (band == Band::BAND_2400MHZ) {
        umiHdkConfig.hdkConf.band = UMI_PHY_TYPE_BAND_2_4_GHZ;
    } else if (band == Band::BAND_5000MHZ) {
        umiHdkConfig.hdkConf.band = UMI_PHY_TYPE_BAND_5_2_GHZ;
    } else if (band == Band::BAND_6000MHZ) {
        umiHdkConfig.hdkConf.band = UMI_PHY_TYPE_BAND_6_GHZ;
    } else {
        throw std::invalid_argument("Invalid value for parameter 'band' (" + toString(band) + ")");
    }

    umiHdkConfig.hdkConf.calibrationMasks.offlineCalMask = offlineCalMask;
    umiHdkConfig.hdkConf.calibrationMasks.onlineCalMask = onlineCalMask;

    Payload payload(reinterpret_cast<uint8_t*>(&umiHdkConfig), sizeof(umiHdkConfig));
    DutDriverFwGeneralMsg message(payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_DRIVER_FW_GENERAL,
        UM_MAN_HDK_CONFIG_REQ,
        MC_MAN_HDK_CONFIG_CFM,
        message,
        timeout);
}

void ClientImpl::setIfs(uint32_t ifs)
{
    dutIfsParams_t ifsParams {};

    ifsParams.spacingUsec = ifs;

    Payload payload(reinterpret_cast<uint8_t*>(&ifsParams), sizeof(ifsParams));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_SET_IFS_REQ,
        dutMessagesId_e::DUT_SET_IFS_CFM,
        message,
        timeout);
}

void ClientImpl::setLnaMidGains(const AntennaMask& antennaMask, const std::array<uint8_t, maxNumRxAntennas>& midGains)
{
    dutLnaMidGain_t lnaMidGain {};

    for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
        if (antennaMask.isBitSet(antenna)) {
            lnaMidGain.lnaMidGainValue[antenna] = midGains[antenna];
        }
    }

    Payload payload(reinterpret_cast<uint8_t*>(&lnaMidGain), sizeof(lnaMidGain));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_SET_LNA_MID_GAIN_REQ,
        dutMessagesId_e::DUT_SET_LNA_MID_GAIN_CFM,
        message,
        timeout);
}

void ClientImpl::setRate(PhyMode phyMode, Bandwidth signalBandwidth, uint8_t mcsIndex, uint8_t nss, Gi gi, Ltf ltf)
{
    dutSetRate_t dutSetRate {};

    dutSetRate.phyMode = convertDutPhy_to_FwPhy(phyMode);
    dutSetRate.cpMode = static_cast<uint8_t>(getCyclicPrefixMode(phyMode, gi, ltf));
    dutSetRate.txRate = getTxRate(phyMode, mcsIndex, nss); //DCM +NSS + MCS = txRate {bit 7=dcm, bits6-4=NSS, bits 3-0=MCS}
    dutSetRate.signalBw = static_cast<uint8_t>(signalBandwidth);

    Payload payload(reinterpret_cast<uint8_t*>(&dutSetRate), sizeof(dutSetRate));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_SET_RATE_REQ,
        dutMessagesId_e::DUT_SET_RATE_CFM,
        message,
        timeout);
}

void ClientImpl::setRiscModeEnabled(bool enabled)
{
    riscModeParams_t riscModeParams {};

    riscModeParams.riscMode = enabled ? dutRiscMode::DUT_RISC_START : dutRiscMode::DUT_RISC_STOP;

    Payload payload(reinterpret_cast<uint8_t*>(&riscModeParams), sizeof(riscModeParams));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_SET_RISC_MODE_REQ,
        dutMessagesId_e::DUT_SET_RISC_MODE_CFM,
        message,
        timeout);
}

void ClientImpl::setRssiCalData(const uint8_t* data, size_t length)
{
    if (!data) {
        throw std::invalid_argument("Parameter 'data' cannot be a null pointer");
    }

    if (length == 0) {
        throw std::invalid_argument("Parameter 'length' cannot be 0");
    }

    if (length > maxRssiCalDataLength) {
        throw std::invalid_argument("The specified data length (" + toString(length) + ") is too large (" + toString(maxRssiCalDataLength) + " bytes allowed)");
    }

    // Must aligned with rssiPathCalibrationHeaderParams_t struct which is defined in RssiPathClbHndlr.c file (FW side)
#pragma pack(push, 1)
    struct RSSI_CAL_PARAMS {
        uint8_t totalSizeOfRssiSection;
        uint8_t lnaSubBandCrossingPoint[maxRssiCalDataLength];
    };
#pragma pack(pop)

    RSSI_CAL_PARAMS rssiCalParams {};

    rssiCalParams.totalSizeOfRssiSection = static_cast<uint8_t>(length);
    memcpy(rssiCalParams.lnaSubBandCrossingPoint, data, length);

    Payload payload(reinterpret_cast<uint8_t*>(&rssiCalParams), sizeof(rssiCalParams));
    DutDriverFwGeneralMsg message(payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_DRIVER_FW_GENERAL,
        UM_MAN_SET_RSSI_CAL_PARAMS_REQ,
        MC_MAN_SET_RSSI_CAL_PARAMS_CFM,
        message,
        timeout);
}

void ClientImpl::setRssiS2dParams(const AntennaMask& antennaMask, uint8_t region, uint8_t gain, uint8_t offset)
{
    S2dParams_t s2dParams {};

    s2dParams.antMask = antennaMask;
    s2dParams.gain = gain;
    s2dParams.iOffset = offset;
    s2dParams.regionIndex = region - 1;

    Payload payload(reinterpret_cast<uint8_t*>(&s2dParams), sizeof(s2dParams));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_SET_RSSI_S2D_GAIN_AND_OFFSET_REQ,
        dutMessagesId_e::DUT_SET_RSSI_S2D_GAIN_AND_OFFSET_CFM,
        message,
        timeout);
}

void ClientImpl::setRuParams(uint32_t userOne, uint32_t userTwo)
{
    dutRuParams_t ruParams {};

    ruParams.userOne = userOne;
    ruParams.userTwo = userTwo;

    Payload payload(reinterpret_cast<uint8_t*>(&ruParams), sizeof(ruParams));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_SET_RU_PARAMS_REQ,
        dutMessagesId_e::DUT_SET_RU_PARAMS_CFM,
        message,
        timeout);
}

void ClientImpl::setRxAggregationEnabled(bool enabled)
{
    dutRxEnableAggregate_t rxEnableAggregate {};

    rxEnableAggregate.aggregate = enabled ? 1 : 0;

    Payload payload(reinterpret_cast<uint8_t*>(&rxEnableAggregate), sizeof(rxEnableAggregate));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_ENABLE_RX_AGGREGATE_REQ,
        dutMessagesId_e::DUT_ENABLE_RX_AGGREGATE_CFM,
        message,
        timeout);
}

void ClientImpl::setRxBand(uint8_t rxBand)
{
    dutSetRxBand_t setRxBand {};

    setRxBand.rxBand = rxBand;

    Payload payload(reinterpret_cast<uint8_t*>(&setRxBand), sizeof(setRxBand));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_SET_RX_BAND_REQ,
        dutMessagesId_e::DUT_SET_RX_BAND_CFM,
        message,
        timeout);
}

void ClientImpl::setRxBandLut(const AntennaMask& antennaMask, const std::array<std::array<uint16_t, maxNumLnaSubBandsCrossingPoints>, maxNumRxAntennas>& rxBandLut, const std::array<uint8_t, maxNumRxAntennas>& firstTunningIndex)
{
    dutSetRxBandLUT_t setRxBandLut {};

    setRxBandLut.antMask = antennaMask;
    for (uint8_t antenna = 0; antenna < maxNumRxAntennas; antenna++) {
        if (antennaMask.isBitSet(antenna)) {
            for (uint8_t subBand = 0; subBand < maxNumLnaSubBandsCrossingPoints; subBand++) {
                setRxBandLut.rxBandLUT[antenna][subBand] = rxBandLut[antenna][subBand];
            }
            setRxBandLut.firstTuningIndex[antenna] = firstTunningIndex[antenna];
        }
    }

    Payload payload(reinterpret_cast<uint8_t*>(&setRxBandLut), sizeof(setRxBandLut));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_SET_RX_BAND_LUT_REQ,
        dutMessagesId_e::DUT_SET_RX_BAND_LUT_CFM,
        message,
        timeout);
}

void ClientImpl::setRxGainBlocks(const AntennaMask& antennaMask, uint8_t lnaIndex, int8_t pgc1, int8_t pgc2, int8_t pgc3)
{
    RssiGainBlock_t rssiGainBlock {};

    rssiGainBlock.antMask = antennaMask;
    rssiGainBlock.lnaIndex = lnaIndex;
    rssiGainBlock.pgc1 = pgc1;
    rssiGainBlock.pgc2 = pgc2;
    rssiGainBlock.pgc3 = pgc3;

    Payload payload(reinterpret_cast<uint8_t*>(&rssiGainBlock), sizeof(rssiGainBlock));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_SET_RSSI_GAIN_BLOCK_REQ,
        dutMessagesId_e::DUT_SET_RSSI_GAIN_BLOCK_CFM,
        message,
        timeout);
}

void ClientImpl::setSpacelessTxEnabled(bool enabled)
{
    dutTxSpacelessParams_t txSpacelessParams {};

    txSpacelessParams.onOff = enabled ? 1 : 0;

    Payload payload(reinterpret_cast<uint8_t*>(&txSpacelessParams), sizeof(txSpacelessParams));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_TX_SPACELESS_REQ,
        dutMessagesId_e::DUT_TX_SPACELESS_CFM,
        message,
        timeout);
}

void ClientImpl::setS2dParams(uint8_t antenna, Bandwidth signalBandwidth, uint8_t region, uint8_t powerThreshold, uint8_t gain, uint8_t offset)
{
    s2dParams s2dParams {};

    s2dParams.ant = antenna;
    s2dParams.bw = static_cast<uint8_t>(signalBandwidth);
    s2dParams.gain = gain;
    s2dParams.offset = offset;
    s2dParams.region = region;
    s2dParams.pThreshold = powerThreshold;

    Payload payload(reinterpret_cast<uint8_t*>(&s2dParams), sizeof(s2dParams));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_SET_S2D_GAIN_AND_OFFSET_REQ,
        dutMessagesId_e::DUT_SET_S2D_GAIN_AND_OFFSET_CFM,
        message,
        timeout);
}

void ClientImpl::setTransmitPowerControlAntennaParams(uint8_t antenna, uint8_t tpcFreqLen, const uint8_t* data, size_t length)
{
    if (!data) {
        throw std::invalid_argument("Parameter 'data' cannot be a null pointer");
    }

    if (length == 0) {
        throw std::invalid_argument("Parameter 'length' cannot be 0");
    }

#pragma pack(push, 1)
    struct tpcAntParamsHeader_t {
        uint8_t antenna;
        uint8_t tpcFreqLen;
        uint16_t padding;
        //uint8_t params[];
    };
#pragma pack(pop)

    tpcAntParamsHeader_t tpcAntParamsHeader {};

    tpcAntParamsHeader.antenna = antenna;
    tpcAntParamsHeader.tpcFreqLen = tpcFreqLen;

    std::shared_ptr<uint8_t> buffer;

    const size_t tpcAntParamsSize = sizeof(tpcAntParamsHeader_t) + length;
    auto p = static_cast<uint8_t*>(malloc(tpcAntParamsSize));
    if (p) {
        memcpy(p, &tpcAntParamsHeader, sizeof(tpcAntParamsHeader_t));
        memcpy(p + sizeof(tpcAntParamsHeader_t), data, length);

        buffer.reset(p, free);
    }

    Payload payload(buffer.get(), tpcAntParamsSize);
    DutDriverFwGeneralMsg message(payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_DRIVER_FW_GENERAL,
        UM_MAN_SET_TPC_ANT_PARAMS_REQ,
        MC_MAN_SET_TPC_ANT_PARAMS_CFM,
        message,
        timeout);
}

void ClientImpl::setTransmitPowerControl(CalibrationType calibrationType, bool closedLoop, uint8_t powerLimit)
{
    tpcConfig_t tpcConfig {};

    tpcConfig.calType = static_cast<uint8_t>(calibrationType);
    tpcConfig.tpcLoopType = closedLoop ? TPC_CLOSED_LOOP : TPC_OPEN_LOOP;
    tpcConfig.fixedGain = NO_FIXED_GAIN;
    for (auto i = 0; i < _countof(tpcConfig.regulationLimit); i++) {
        tpcConfig.regulationLimit[i] = powerLimit;
    }
    for (auto i = 0; i < _countof(tpcConfig.regulationLimitMU); i++) {
        tpcConfig.regulationLimitMU[i] = powerLimit;
    }
    for (auto i = 0; i < _countof(tpcConfig.regulationLimitBF); i++) {
        tpcConfig.regulationLimitBF[i] = powerLimit;
    }
    for (auto i = 0; i < _countof(tpcConfig.regulationLimitBF_AX); i++) {
        tpcConfig.regulationLimitBF_AX[i] = powerLimit;
    }
    for (auto i = 0; i < _countof(tpcConfig.regulationLimitAX); i++) {
        tpcConfig.regulationLimitAX[i] = powerLimit;
    }
    for (auto i = 0; i < _countof(tpcConfig.regulationLimitBF_BE); i++) {
        tpcConfig.regulationLimitBF_BE[i] = powerLimit;
    }
    for (auto i = 0; i < _countof(tpcConfig.regulationLimitBE); i++) {
        tpcConfig.regulationLimitBE[i] = powerLimit;
    }
    for (auto i = 0; i < _countof(tpcConfig.regulationLimitN); i++) {
        tpcConfig.regulationLimitN[i] = powerLimit;
    }
    tpcConfig.regulationLimitAG = powerLimit;
    tpcConfig.powerLimit11b = powerLimit;

    Payload payload(reinterpret_cast<uint8_t*>(&tpcConfig), sizeof(tpcConfig));
    DutDriverFwGeneralMsg message(payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_DRIVER_FW_GENERAL,
        UM_MAN_SET_TPC_CONFIG_REQ,
        MC_MAN_SET_TPC_CONFIG_CFM,
        message,
        timeout);
}

void ClientImpl::setTransmitPowerLevel(uint8_t antenna, Bandwidth bandwidth, uint8_t powerLevel)
{
    dutPowerOutVecParams_t powerOutVecParams {};

    powerOutVecParams.ant = antenna;
    powerOutVecParams.bandwidth = static_cast<uint8_t>(bandwidth);
    powerOutVecParams.powerVectorIndex = powerLevel;
    powerOutVecParams.powerVectorWord = (2 * powerLevel); // since every index is half DB, we need to multiply by 2 //not being used

    Payload payload(reinterpret_cast<uint8_t*>(&powerOutVecParams), sizeof(powerOutVecParams));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_SET_POWER_OUT_VEC_REQ,
        dutMessagesId_e::DUT_SET_POWER_OUT_VEC_CFM,
        message,
        timeout);
}

void ClientImpl::setTransmitPowerTableOffset(uint8_t antenna, Bandwidth bandwidth, int16_t offset)
{
    dutPoutTableOffsetParams_t poutTableOffsetParams {};

    poutTableOffsetParams.antNum = antenna;
    poutTableOffsetParams.bw = static_cast<uint8_t>(bandwidth);
    poutTableOffsetParams.tableOffset = offset;

    Payload payload(reinterpret_cast<uint8_t*>(&poutTableOffsetParams), sizeof(poutTableOffsetParams));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_SET_POUT_TABLE_OFFSET_REQ,
        dutMessagesId_e::DUT_SET_POUT_TABLE_OFFSET_CFM,
        message,
        timeout);
}

void ClientImpl::getTransmitPowerVectorForAntenna(uint8_t antenna, TransmitPowerVector_t& transmitPowerVector) const
{
    dutGetTransmitPowerVectorAntenna_t getTransmitPowerVectorAntenna {};

    getTransmitPowerVectorAntenna.antennaIndex = antenna;

    // Payload is updated in the destructor of the message (i.e.: when the variable 'message' goes out of scope)
    {
        Payload payload(reinterpret_cast<uint8_t*>(&getTransmitPowerVectorAntenna), sizeof(getTransmitPowerVectorAntenna));
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
            dutMessagesId_e::DUT_GET_TX_VECTOR_ANT_REQ,
            dutMessagesId_e::DUT_GET_TX_VECTOR_ANT_CFM,
            message,
            timeout);
    }

    for (int i = 0; i < transmitPowerTableSize; i++) {
        transmitPowerVector.measuredVoltagePerPower[antenna][i] = getTransmitPowerVectorAntenna.measuredVoltagePerPower[i];
    }
};

void ClientImpl::setTransmitPowerVector(const AntennaMask& antennaMask, TransmitPowerVector_t& transmitPowerVector)
{
    dutSetTransmitPowerVector_t setTransmitPowerVector {};
    setTransmitPowerVector.lengthOfPowerVector = transmitPowerVector.lengthOfPowerVector;
    setTransmitPowerVector.startingPacketLength = transmitPowerVector.startingPacketLength;
    setTransmitPowerVector.packetLengthIncrementPerIndex = transmitPowerVector.packetLengthIncrementPerIndex;

    for (int i = 0; i < transmitPowerTableSize; i++) {
        setTransmitPowerVector.powerIndexArray[i] = transmitPowerVector.powerIndexArray[i];
        setTransmitPowerVector.numOfTransmitionPerPower[i] = transmitPowerVector.numOfTransmissionsPerPower[i];
    }

    Payload payload(reinterpret_cast<uint8_t*>(&setTransmitPowerVector), sizeof(setTransmitPowerVector));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_START_TX_VECTOR_REQ,
        dutMessagesId_e::DUT_START_TX_VECTOR_CFM,
        message,
        timeout);

    for (uint8_t antenna = 0; antenna < maxNumTxAntennas; antenna++) {
        if (antennaMask.isBitSet(antenna)) {
            getTransmitPowerVectorForAntenna(antenna, transmitPowerVector);
        }
    }
}

void ClientImpl::setXtalValue(uint16_t xtalValue)
{
    dutXtalParams_t xtalParams {};

    xtalParams.xtalValue = xtalValue;

    Payload payload(reinterpret_cast<uint8_t*>(&xtalParams), sizeof(xtalParams));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_SET_XTAL_VALUE_REQ,
        dutMessagesId_e::DUT_SET_XTAL_VALUE_CFM,
        message,
        timeout);
}

void ClientImpl::setZwdfsEnabled(bool zwdfsEnabled, uint8_t zwdfsChannel, Bandwidth zwdfsBandwidth, Bandwidth radarDetectionBandwidth, RegulationType regulationType)
{
    UM_SET_CHAN_PARAMS umSetChanParams {};

    umSetChanParams.low_chan_num = zwdfsChannel;
    umSetChanParams.low_chan_num2 = 0;
    umSetChanParams.chan_width = static_cast<uint8_t>(zwdfsBandwidth);
    umSetChanParams.primary_chan_idx = 0;
    umSetChanParams.switch_type = ST_ZWDFS;
    umSetChanParams.antennaMask = zwdfsEnabled ? 0x01 : 0x00;
    umSetChanParams.RegulationType = static_cast<uint8_t>(regulationType);
    if (radarDetectionBandwidth == Bandwidth::BANDWIDTH_ONE_HUNDRED_SIXTY) {
        umSetChanParams.isRadarDetectionNeeded = 0xff;
    } else if (radarDetectionBandwidth == Bandwidth::BANDWIDTH_EIGHTY) {
        umSetChanParams.isRadarDetectionNeeded = 0x0f;
    } else {
        umSetChanParams.isRadarDetectionNeeded = 0x00;
    }

    Payload payload(reinterpret_cast<uint8_t*>(&umSetChanParams), sizeof(umSetChanParams));
    DutDriverFwGeneralMsg message(payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_DRIVER_FW_GENERAL,
        UM_SET_CHAN_REQ,
        UM_SET_CHAN_CFM,
        message,
        timeout);
}

void ClientImpl::startCalibration(const StartCalibrationParams_t& params, uint8_t& status)
{
    dutStartCalibrationParams_t calibrationStart {};

    calibrationStart.mask = params.mask;
    calibrationStart.maskType = params.maskType;
    calibrationStart.type = params.type;

    {
        Payload payload(reinterpret_cast<uint8_t*>(&calibrationStart), sizeof(dutStartCalibrationParams_t));
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
            dutMessagesId_e::DUT_START_CALIBRATION_REQ,
            dutMessagesId_e::DUT_START_CALIBRATION_CFM,
            message,
            timeout);
    }

    status = calibrationStart.status;
}

void ClientImpl::startCw(int8_t amplitude, int16_t tone)
{
    dutTxToneParams_t txToneParams {};

    if (amplitude == MAX_INT8) {
        txToneParams.onOff = 0;
    } else {
        txToneParams.amplitude = amplitude;
        txToneParams.binNum = tone;
        txToneParams.onOff = 1;
    }

    Payload payload(reinterpret_cast<uint8_t*>(&txToneParams), sizeof(txToneParams));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_TX_TONE_REQ,
        dutMessagesId_e::DUT_TX_TONE_CFM,
        message,
        timeout);
}

void ClientImpl::stopCw()
{
    startCw(MAX_INT8, 0);
}

void ClientImpl::startRxCalibration()
{
    Payload payload(nullptr, 0);
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_START_RSSI_CALIBRATION_REQ,
        dutMessagesId_e::DUT_START_RSSI_CALIBRATION_CFM,
        message,
        timeout);
}

void ClientImpl::stopRxCalibration()
{
    Payload payload(nullptr, 0);
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_END_RSSI_CALIBRATION_REQ,
        dutMessagesId_e::DUT_END_RSSI_CALIBRATION_CFM,
        message,
        timeout);
}

void ClientImpl::startTx(uint16_t repetitions, uint32_t packetLength, bool longData, bool beamforming, bool ldpc)
{
    dutTxParams_t trafficParams {};

    trafficParams.repeats = repetitions;
    trafficParams.packetLength = packetLength;
    trafficParams.isDataLong = longData ? 1 : 0;
    trafficParams.isBeamforming = beamforming ? 1 : 0;
    trafficParams.isLdpc = ldpc ? 1 : 0;
    trafficParams.isTxEndless = (repetitions == UINT16_MAX) ? 1 : 0;

    Payload payload(reinterpret_cast<uint8_t*>(&trafficParams), sizeof(trafficParams));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_START_TX_REQ,
        dutMessagesId_e::DUT_START_TX_CFM,
        message,
        timeout);
}

void ClientImpl::startRxPer(uint32_t packetLimit, PhyMode phyMode)
{
    dutRxPacketLimit_t rxPacketLimitParams {};
    rxPacketLimitParams.packetLimit = packetLimit;
    rxPacketLimitParams.protocol = convertDutPhy_to_PhyProtocol(phyMode);

    Payload payload(reinterpret_cast<uint8_t*>(&rxPacketLimitParams), sizeof(rxPacketLimitParams));
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_RX_PACKET_LIMIT_REQ,
        dutMessagesId_e::DUT_RX_PACKET_LIMIT_CFM,
        message,
        timeout);
}

void ClientImpl::stopTx()
{
    Payload payload(nullptr, 0);
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_STOP_TX_REQ,
        dutMessagesId_e::DUT_STOP_TX_CFM,
        message,
        timeout);
}

void ClientImpl::stopRxPer(PhyMode phyMode, uint32_t& totalPacketsReceived, uint32_t& packetCounter)
{
    dutRxPerCounters_t rxPerCountersReq {};
    rxPerCountersReq.protocol = convertDutPhy_to_PhyProtocol(phyMode);
    rxPerCountersReq.stopRx = 1;

    // Payload is updated in the destructor of the message (i.e.: when the variable 'message' goes out of scope)
    {
        Payload payload(reinterpret_cast<uint8_t*>(&rxPerCountersReq), sizeof(rxPerCountersReq));
        DutMessage message(m_wlanIndex, payload);

        exchangeMessage(
            m_wlanIndex,
            dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
            dutMessagesId_e::DUT_RX_PER_COUNTERS_REQ,
            dutMessagesId_e::DUT_RX_PER_COUNTERS_CFM,
            message,
            timeout);
    }

    totalPacketsReceived = rxPerCountersReq.totalPackets;
    packetCounter = rxPerCountersReq.packetCounter;
}

void ClientImpl::writeMemory(ChipModule chipModule, size_t address, const uint8_t* data, size_t length)
{
    dutMemoryAccessParams_t memoryAccessParams {};

    if (!data) {
        throw std::invalid_argument("Parameter 'data' cannot be a null pointer");
    }

    if (length == 0) {
        throw std::invalid_argument("Parameter 'length' cannot be 0");
    }

    if (length > maxMemoryAccessLength) {
        throw std::invalid_argument("The specified length (" + toString(length) + ") is too large (" + toString(maxMemoryAccessLength) + " bytes allowed)");
    }

    memoryAccessParams.module = static_cast<uint16_t>(chipModule);
    memoryAccessParams.address = static_cast<uint32_t>(address);
    memoryAccessParams.length = static_cast<uint16_t>(length);
    memcpy(memoryAccessParams.data, data, length);

    size_t headerLength = reinterpret_cast<uint8_t*>(&(memoryAccessParams.data)) - reinterpret_cast<uint8_t*>(&memoryAccessParams);

    Payload payload(reinterpret_cast<uint8_t*>(&memoryAccessParams), headerLength + length);
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_MAC_C100,
        dutMessagesId_e::DUT_WRITE_MEMORY_REQ,
        dutMessagesId_e::DUT_WRITE_MEMORY_CFM,
        message,
        timeout);
}

void ClientImpl::writeNvMemory(size_t address, const uint8_t* data, size_t length, NvMemoryType memoryType, NvMemorySize memorySize, FileType fileType)
{
    dutNvMemoryAccessParams_t nvMemoryAccessParams {};

    if (!data) {
        throw std::invalid_argument("Parameter 'data' cannot be a null pointer");
    }

    if (length == 0) {
        throw std::invalid_argument("Parameter 'length' cannot be 0");
    }

    if (length > maxNvMemoryAccessLength) {
        throw std::invalid_argument("The specified length (" + toString(length) + ") is too large (" + toString(maxNvMemoryAccessLength) + " bytes allowed)");
    }

    nvMemoryAccessParams.nvMemoryType = static_cast<uint32_t>(memoryType);
    nvMemoryAccessParams.address = static_cast<uint32_t>(address);
    nvMemoryAccessParams.eepromSize = static_cast<uint32_t>(memorySize);
    nvMemoryAccessParams.length = static_cast<uint32_t>(length);
    nvMemoryAccessParams.fileType = static_cast<uint32_t>(fileType);
    memcpy(nvMemoryAccessParams.data, data, length);

    size_t headerLength = reinterpret_cast<uint8_t*>(&(nvMemoryAccessParams.data)) - reinterpret_cast<uint8_t*>(&nvMemoryAccessParams);

    Payload payload(reinterpret_cast<uint8_t*>(&nvMemoryAccessParams), headerLength + length);
    DutMessage message(m_wlanIndex, payload);

    exchangeMessage(
        m_wlanIndex,
        dutDriverMessagesId_e::DUT_SERVER_MSG_DRIVER_GENERAL,
        dutDriverGeneralMsgId_e::DUT_DGM_WRITE_NV_MEMORY_REQ,
        dutDriverGeneralMsgId_e::DUT_DGM_WRITE_NV_MEMORY_CFM,
        message,
        timeout);
}

}
