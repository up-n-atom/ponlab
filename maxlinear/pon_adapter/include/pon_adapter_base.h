/******************************************************************************
 *
 *  Copyright (c) 2025 MaxLinear, Inc.
 *  Copyright (c) 2017 - 2019 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

/**
 * \file pon_adapter_base.h
 *
 * This is the PON adapter standard header file, including basic headers and
 * types definitions.
 */

#ifndef _PON_ADAPTER_BASE_H_
#define _PON_ADAPTER_BASE_H_

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(__PA_RUST__)

#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#else
/**
 * IMPORTANT: This is required to generate easier the rust bindings as
 * otherwise the headers above make the bindings rust file very large.
 */
/** C99 type, signed integer 8 bit */
typedef signed char int8_t;
/** C99 type, unsigned integer 8 bit */
typedef unsigned char uint8_t;
/** C99 type, signed integer 16 bit */
typedef short int16_t;
/** C99 type, unsigned integer 16 bit */
typedef unsigned short uint16_t;
/** C99 type, signed integer 32 bit */
typedef int int32_t;
/** C99 type, unsigned integer 32 bit */
typedef unsigned int uint32_t;
/** C99 type, unsigned integer 64 bit */
typedef unsigned long uint64_t;

/** C99 type, boolean */
typedef unsigned char bool;

#define _Bool bool
#define true 1
#define false 0

#endif  /* __PA_RUST__ */

/** \addtogroup PON_ADAPTER
 *
 *  @{
 */

/** @} */ /* PON_ADAPTER */

#ifdef __cplusplus
}
#endif

#endif /* _PON_ADAPTER_BASE_H_ */
