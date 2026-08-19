#ifndef __LTQ_MPE_HAL_H__20081119_1144__
#define __LTQ_MPE_HAL_H__20081119_1144__

/********************************************************************************

  Copyright © 2020-2024 MaxLinear, Inc.
  Copyright (C) 2017-2019 Intel Corporation
  Lantiq Beteiligungs-GmbH & Co. KG
  Lilienthalstrasse 15, 85579 Neubiberg, Germany 
  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

********************************************************************************/

/*! \file mpe_hal.h
  \brief This file contains: MPE HAL API's to reserve memory and start feature.
 */

#include <net/ppa/ppa_api_common.h>
#include <net/datapath_api.h>

#define F_FEATURE_START		1
#define F_FEATURE_STOP		0

enum MPE_Feature_Type {
	DL_TX_1 = 0,
	DL_TX_2
};
struct device *mpe_hal_dl_get_dev(void);
void mpe_hal_dl_enable_gic(int irq_no);
int mpe_hal_dl_alloc_resource(
		uint32_t memSize,
		uint32_t *memAddr,
		uint32_t flags);
int mpe_hal_feature_start(
		enum MPE_Feature_Type mpeFeature,
		uint32_t port_id,
		uint32_t *featureCfgBase,
		uint32_t flags);

void mpe_hal_debugfs_destroy(void);
int mpe_hal_debugfs_create(void);

/* MPE debug */

#define ENABLE_DEBUG_PRINT			 1
#define DBG_ENABLE_MASK_ERR			(1 << 0)
#define DBG_ENABLE_MASK_DEBUG_PRINT		(1 << 1)
#define DBG_ENABLE_MASK_ALL			(DBG_ENABLE_MASK_ERR | DBG_ENABLE_MASK_DEBUG_PRINT)
#define NUM_ENTITY(x)				(sizeof(x) / sizeof(*(x)))


#if defined(ENABLE_DEBUG_PRINT) && ENABLE_DEBUG_PRINT
#undef ppa_debug
#define ppa_debug(flag, fmt, arg...) do {\
		if ((g_mpeh_dbg_enable & flag)) {\
			printk(KERN_ERR fmt, ##arg);\
		}\
	} while (0)
#else
#undef ppa_debug
#define ppa_debug(flag, fmt, arg...)
#endif
extern uint32_t g_mpeh_dbg_enable;

#endif  /*  ___MPE_HAL_H__20081119_1144__*/
