/*
 * elf_parser.h
 * Description: ELF binary format parser for FW
 *
 * SPDX-License-Identifier: GPL-2.0-only
 * Copyright (C) 2018 Intel Corporation
 */

#ifndef __ELF_PARSER_H__
#define __ELF_PARSER_H__

#include <linux/types.h>
#include <linux/firmware.h>

struct elf_sec {
	char	*name;		/*! Section name */
	void	*data;		/*! Source (Copy from this address) */
	u64	name_idx;	/*! Source (Copy from this address) */
	u64	addr;		/*! Dest (Copy to this address) */
	u64	size;		/*! Section size */
	u8	need_copy;	/*! Whether this section needs to be copied */
	u64	type;		/*! Section type (Internal) */
	u64	flags;		/*! Section flags (Internal) */
	u64	offset;		/*! Section offset from beginning (Internal) */
};

/**
 * @brief Parse ELF buffer
 * This function parses ELF buffer
 * @param elf:		ELF buffer to parse
 * @param len:		Size of the ELF buffer
 * @param secs:		ELF Sections array to be filled by this function
 * @param max_secs:	Number of sections available in secs
 * @param num_secs:	Number of total sections in ELF
 * @return 0 on success.
 */
s32 elf_parse(const unsigned char *elf, u32 len, struct elf_sec secs[],
	      u16 max_secs, u16 *num_secs);

#endif /* __ELF_PARSER_H__ */
