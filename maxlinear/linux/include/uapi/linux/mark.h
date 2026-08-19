/* SPDX-License-Identifier: GPL-2.0+ WITH Linux-syscall-note */
/******************************************************************************
 *	Copyright (c) 2022-2024 MaxLinear, Inc.
 *
 *	Note: Currently used by MXL acceleration framework.
 *****************************************************************************/
#ifndef _LINUX_MARK_H
#define _LINUX_MARK_H

/* Bit counting starts from Least significant bit, 0..31*/
#define MARK_QUEPRIO_START_BIT_POS 0
#define MARK_QUEPRIO_NO_OF_BITS 5
#define MARK_QUEPRIO_MASK 0x1f

#define MARK_ACCELSEL_START_BIT_POS 5
#define MARK_ACCELSEL_NO_OF_BITS 1
#define MARK_ACCELSEL_MASK 0x20

#define GET_DATA_FROM_MARK_OPT(mark, mask, pos, value) \
	(value = ((mark & mask) >> pos))
#define SET_DATA_FROM_MARK_OPT(mark, mask, pos, value) \
	do { \
		mark &= ~mask; \
		mark |= ((value << pos) & mask); \
	} while (0)

#define MASK(pos, len) (((1<<len)-1)<<pos)
#define GET_DATA_FROM_MARK(mark, pos, len, value) \
	GET_DATA_FROM_MARK_OPT(mark, MASK(pos, len), pos, value)
#define SET_DATA_FROM_MARK(mark, pos, len, value) \
	SET_DATA_FROM_MARK_OPT(mark, MASK(pos, len), pos, value)
#endif
