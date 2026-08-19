/******************************************************************************

         Copyright (c) 2020, MaxLinear, Inc.
         Copyright 2016 - 2020 Intel Corporation
         Copyright 2015 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
         Copyright 2009 - 2014 Lantiq Deutschland GmbH
         Copyright 2007 - 2008 Infineon Technologies AG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#ifndef __DUT_MSG_CLBK_H__
#define __DUT_MSG_CLBK_H__

typedef BOOL(*dut_msg_clbk_t)(int dutIndex, uint8_t* data, size_t length, size_t *out_length);

void dut_msg_clbk_init();

dut_msg_clbk_t dut_msg_clbk_get_handler(int msgID);

#endif /* !__DUT_MSG_CLBK_H__ */

