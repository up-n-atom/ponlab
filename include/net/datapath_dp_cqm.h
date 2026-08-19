// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2020-2022 MaxLinear, Inc.
 * Copyright (C) 2020 Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License, as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __DATAPATH_DP_CQM_H
#define __DATAPATH_DP_CQM_H

/**
 * \file datapath_dp_cqm.h
 * \brief This file contains all the constat define used in dpm and cqm
 */
#define DP_DC_NUM 2  /* Max number of DC ports supported per LPID */

#define DP_RX_RING_NUM DP_DC_NUM /*!< maximum number of ACA RX ring
			   *   For GRX500/PRX300, only support 1 ring
			   *   For LGM, maximum up to 2 rings
			   */
#define DP_TX_RING_NUM DP_DC_NUM /*!< maximum number of ACA RXOUT ring
			  *   For GRX500/PRX300, only support 1 ring
			  *   For 5G, it needs to support up to 16 ring.
			  */
#define DP_RX_RING_DEQ_NUM 8   /*!< maximum number of DEQ  port per  RX ring
				*   valid for LGM non QOS bypass mode
				*/
#define DP_MAX_POLICY_GPID 4   /*!< maximum number of policy per GPID
				*   for HW automatically free BM buffer
				*   In LGM, it is 4. Not valid for all other
				*   existing products
				*/

/*! @brief DP Special Connectivity Type*/
enum DP_SPL_TYPE {
	DP_NON_SPL = 0, /*!< NON Special connectivity type */
	DP_SPL_TOE,	/*!< Special connectivity type: LRO/TSO */
	DP_SPL_VOICE, /*!< Special connectivity type: VOICE */
	DP_SPL_VPNA, /*!< Special connectivity type: VPN Adaptor */
	DP_SPL_APP_LITEPATH, /*!< Special connectivity type: For Application
			      *   Litepath
			      */
	DP_SPL_PP_NF,    /*!< Special connectivity type:For PPV4 uCs-
			  *   Fragmenter, Reassembler, Multicast, TurboDox
			  */
	DP_SPL_PP_DUT, /*!< Special connectivity type: For PPV4 Application
			      *   unit test */
	DP_SPL_MAX, /*!< Special connectivity type: MAX */
	DP_SPL_INVAL = DP_SPL_MAX /*!< Special connectivity type: NOT VALID */
};

struct dp_gpid_tx_info {
	/* Note for GPID max packet length setting, DP should get it from
	 *      struct dp_tx_ring's tx_pkt_size
	 */
	u32 f_min_pkt_len : 1; /*!< flag to indicate whether min_pkt_len is
				*   set or not by caller
				*   1: min_pkt_len is set by caller
				*   0: min_pkt_len is not set by caller.
				*/
	u32 seg_en : 1; /*!< support FSQM buffer or not
			 *   For stream port: should set to 1.
			 *   For non_stream port: shoudl set to 0
			 */

	u16 min_pkt_len; /*!< minimum packet length in bytes
			  *   valid if f_min_pkt_len is set
			  */
};

struct dp_dc_rx_res {
	int in_alloc_ring_size; /*!< [out] rxin alloc buffer ring
				 *         burst size supported by SoC
				 */
	int out_enq_ring_size;  /*!< [out] rxout enqueue ring size
				 *         supported by SoC
				 */
	int rx_bufs;            /*!< [out] maximum rx buffers can be supported
				 *         for prefetching in RXIN
				 *         based on res_id
				 */
};

struct dp_dc_tx_res {
	int in_deq_ring_size;   /*!< [out] txin ring burst size
				 *         supported by SoC
				 */
	int out_free_ring_size; /*!< [out] txout free buffer ring burst size
				 *         supported by SoC
				 */
	int tx_bufs;            /*!< [out] maximum Tx buffers can be supported
				 *         in txin ring based on
				 *         specified res_id
				 */
};

/*! @brief struct dp_dev_opt_param, which used for top level to pass extra
 *  optional paramters to dpm
 */
struct dp_dev_opt_param {
	void *pcidata; /*!< for pci dev to pass pcidata in LGM b0 */
};

#endif
