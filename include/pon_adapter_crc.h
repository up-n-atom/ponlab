/******************************************************************************
 *
 *  Copyright (c) 2018 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter_crc.h
 *
 * This is the PON adapter header file for OMCI CRC calculation.
 */

#ifndef _palib_crc_h
#define _palib_crc_h

#include "pon_adapter_base.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \addtogroup PON_ADAPTER
 *  @{
 */

/** \defgroup PON_ADAPTER_CRC PON Adapter CRC check
 *
 * These functions are used to perform a CRC check according to OMCI when
 * operating in ITU-T G.984 mode.
 *
 * @{
 */

/** Update CRC-32 as defined in ITU-T I.363.5
 *  Can be also used for consecutive checks with previous value as initial.
 *
 *  \param[in]  crc           Initial CRC value (0xFFFFFFFFF)
 *  \param[in]  data          Data for which CRC will be calculated
 *  \param[in]  data_size     Data size
 *
 *  \return Returns calculated CRC
 */
uint32_t pa_omci_crc32(uint32_t crc,
		       const uint8_t *data,
		       size_t data_size);

/** @} */ /* PON_ADAPTER_CRC */
/** @} */ /* PON_ADAPTER */

#ifdef __cplusplus
}
#endif

#endif
