/* SPDX-License-Identifier: GPL-2.0 */
/******************************************************************************
 *
 * Copyright (c) 2022 MaxLinear, Inc.
 *
 *****************************************************************************/
#ifndef _DP_OAM_TRANSCEIVE_
#define _DP_OAM_TRANSCEIVE_

struct tx_data {
	u32 ifi;
	u16 vid;
	u8 level;
	u8 opcode;
};

void dp_oam_transceive_init(void);
int dp_oam_transceive_load(u8 level, bool rx_ts_f);
void dp_oam_transceive_unload(void);

int dp_oam_tx(struct tx_data *data);

#endif
