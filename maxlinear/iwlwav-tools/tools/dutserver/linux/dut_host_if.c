/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation
         Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
         Copyright 2009 - 2014 Lantiq Deutschland GmbH
         Copyright 2007 - 2008 Infineon Technologies AG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#include "mtlkinc.h"
#include "dut_host_if.h"
#include "dut_msg_clbk.h"

#define LOG_LOCAL_GID   GID_DUT_SRV_HOST_IF
#define LOG_LOCAL_FID   1

#define DUT_MSG_ID_RESPONSE_FLAG          0x80

#define DUT_MSG_VERIFY_SIGNATURE(hdr)     ((hdr)[0] == 'M' && (hdr)[1] == 'T')
#define DUT_MSG_SET_ID(hdr, id)           (((uint8*)(hdr))[3] = id)
#define DUT_MSG_SET_LENGTH(hdr, len)      (((uint32*)(hdr))[1] = HOST_TO_DUT32(len))
#define DUT_MSG_GET_ID(hdr)               (((uint8*)(hdr))[3] & 0xF)
#define DUT_MSG_GET_HW_IDX(hdr)           ((((uint8*)(hdr))[3] >> 4) & 0xF)

static __INLINE uint32
DUT_MSG_GET_LENGTH(const uint8_t* hdr)
{
    uint32 raw_length;
    wave_memcpy(&raw_length, sizeof(uint32), hdr + sizeof(uint32), sizeof(uint32));
    return DUT_TO_HOST32(raw_length);
}

BOOL dut_hostif_request_contains_valid_header(const uint8_t* buffer, size_t bufferLength)
{
  return (bufferLength >= DUT_MSG_HEADER_LENGTH) && DUT_MSG_VERIFY_SIGNATURE(buffer);
}

/**
 * The communication between the DUT client and the DUT server uses a very simple proprietary 
 * protocol in which messages have this structure:
 * - There are 2 signature bytes, with values 'M' and 'T' (which stand for Metalink).
 * - 1 byte for the version, which is set to one and not used by the DUT server at this moment.
 * - A 4-bit field containing the index of the WiFi card the message is intended for.
 * - A 4-bit field containing the message identifier
 * - A 4-byte data length, containing the length of the variable data field (in little endian)
 * - And the payload data, which contents depend on the message id. 
 */
void dut_hostif_handle_request(uint8_t* buffer, size_t *bufferLength, dut_response_handler_t response_handler, void *arg)
{
  uint8_t* data;
  size_t dataLength;
  size_t responseLength = 0;
  int msgID;
  int dutIndex;
  dut_msg_clbk_t msgHandler;

  // Check packet header validity
  if (*bufferLength < DUT_MSG_HEADER_LENGTH)
  {
    // This is not an error, just return and wait to receive the rest of the message
    ILOG1_V("Packet is not fully received yet");
    return;
  }

  if (!DUT_MSG_VERIFY_SIGNATURE(buffer))
  {
    ELOG_D("Invalid packet received. MT signature not found. Packet length = %d", *bufferLength);
    *bufferLength = 0;           
    return;
  }

  dataLength = DUT_MSG_GET_LENGTH(buffer);
  if (dataLength > DUT_MSG_MAX_PAYLOAD_LENGTH)
  {
    ELOG_D("Invalid packet received. Payload length is too large. Data length = %d", dataLength);
    *bufferLength = 0; 
    return;
  }
  if (dataLength + DUT_MSG_HEADER_LENGTH > *bufferLength)
  {
    // This is not an error, just return and wait to receive the rest of the message
    ILOG1_V("Packet is not fully received yet");
    return;
  }

  // Parse the packet and try to process it
  *bufferLength = 0; 
  msgID = DUT_MSG_GET_ID(buffer);
  dutIndex = DUT_MSG_GET_HW_IDX(buffer);
  data = buffer + DUT_MSG_HEADER_LENGTH;

  ILOG1_DDD("Received message ID 0x%X for HW #%d, length is %d",
            msgID, dutIndex, DUT_MSG_HEADER_LENGTH + dataLength);

  msgHandler = dut_msg_clbk_get_handler(msgID);
  if (NULL == msgHandler)
  {
    ELOG_D("Unknown message ID 0x%X", msgID);
    return;
  }

  if (!msgHandler(dutIndex, data, dataLength, &responseLength))
  {
    WLOG_V("Message processing failed so response will NOT be sent.");
    return;
  }

  ILOG1_DD("Sending response msgID 0x%X, length is %d",
          msgID, DUT_MSG_HEADER_LENGTH + responseLength);

  const uint8_t *header = buffer;
  DUT_MSG_SET_ID(header, msgID | DUT_MSG_ID_RESPONSE_FLAG);
  DUT_MSG_SET_LENGTH(header, responseLength);
  if (response_handler(header, DUT_MSG_HEADER_LENGTH + responseLength, arg))
  {
    ILOG2_V("Message is processed and response has been sent.");
  }
}
