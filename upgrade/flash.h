/*******************************************************************************

  Copyright © 2020 MaxLinear, Inc.

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

*******************************************************************************/

#ifndef _FLASH_H_
#define _FLASH_H_

int flash_sect_erase(unsigned long addr_first, unsigned long addr_last);

int flash_write(unsigned long srcAddr, unsigned long destAddr, int srcLen);

void flash_sect_protect(int mode, unsigned long addr_first,
			unsigned long addr_last);

#ifdef CONFIG_BOOT_FROM_NAND
int nand_flash_write(ulong srcAddr, ulong destAddr, int srcLen);
#endif

#endif				/* _FLASH_H_ */
