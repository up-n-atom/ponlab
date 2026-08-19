/******************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 * Copyright (c) 2017 - 2018 Intel Corporation
 * Copyright (c) 2012 - 2016 Lantiq Beteiligungs-GmbH & Co. KG
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 *****************************************************************************/
#ifndef __pontop_h
#define __pontop_h

#define UNUSED(x) (void)(x)

extern top_page_init_t pontop_page_init[];
extern unsigned int pontop_page_init_num;

extern const struct top_page_desc pontop_page[];
extern const unsigned int pontop_page_num;

extern enum pon_tx_power_scale pontop_tx_power_scale;

#endif
