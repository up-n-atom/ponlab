/******************************************************************************
 *
 * Copyright (c) 2018 Intel Corporation
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.
 *
 ******************************************************************************/

#ifndef __PON_MCC_CONV_H
#define __PON_MCC_CONV_H

#include <stddef.h>
#include <linux/kernel.h>
#include <net/switch_api/lantiq_gsw.h>

/*
 * Translate struct mcc_drv_l3tbl_key
 * to GSW_multicastTable_t
 */
int l3tbl_key_to_gsw_entry(const struct mcc_drv_l3tbl_key *key,
			   GSW_multicastTable_t *entry);

/*
 * Translate struct mcc_drv_l3tbl_key and bool initial
 * to GSW_multicastTableRead_t
 */
void l3tbl_key_to_gsw_entry_read(const struct mcc_drv_l3tbl_key *key,
				 bool initial,
				 GSW_multicastTableRead_t *entry_read);

/*
 * Translate GSW_multicastTableRead_t
 * to struct mcc_drv_l3tbl_key and bool last
 */
void gsw_entry_read_to_l3tbl_key(const GSW_multicastTableRead_t *entry_read,
				 struct mcc_drv_l3tbl_key *key, bool *last,
				 void *portmap);

#endif /* __PON_MCC_CONV_H */
