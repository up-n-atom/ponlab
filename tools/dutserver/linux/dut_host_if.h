/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation
         Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
         Copyright 2009 - 2014 Lantiq Deutschland GmbH
         Copyright 2007 - 2008 Infineon Technologies AG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#ifndef __DUT_HOST_IF_H__
#define __DUT_HOST_IF_H__

#include <stdint.h>
#include <stdlib.h>

#define DUT_MSG_MAX_MESSAGE_LENGTH        (1024)
#define DUT_MSG_HEADER_LENGTH             (8)
#define DUT_MSG_MAX_PAYLOAD_LENGTH        (DUT_MSG_MAX_MESSAGE_LENGTH - DUT_MSG_HEADER_LENGTH)

typedef BOOL(*dut_response_handler_t)(const uint8_t* buffer, size_t length, void *arg);

BOOL dut_hostif_request_contains_valid_header(const uint8_t* buffer, size_t bufferLength);
void dut_hostif_handle_request(uint8_t* buffer, size_t *bufferLength, dut_response_handler_t response_handler, void *arg);

#endif /* !__DUT_HOST_IF_H__ */

