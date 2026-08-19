/******************************************************************************
 * Copyright (c) 2021 - 2022, MaxLinear, Inc.
 *
 * For licensing information, see the file 'LICENSE' in the root folder of
 * this software module.

*******************************************************************************/
#ifndef DATAPATH_DFLT_Q_MAP_LGM
#define DATAPATH_DFLT_Q_MAP_LGM

/* subif level queue map reset */
#define subif_q_map_reset_lookup_f ((CBM_QUEUE_MAP_F_TC_DONTCARE) | \
			     (CBM_QUEUE_MAP_F_COLOR_DONTCARE) | \
			     (CBM_QUEUE_MAP_F_EGFLAG_DONTCARE))

/* whole queue map table reset */
#define drop_lookup_f -1

/* LGM CPU rule */
static struct dp_q_map_entry q_map_cpu[] = {
};

/* ethernet based streaming port rule */
static struct dp_q_map_entry q_map_stream_ethernet[] = {
	/* Rule 1:
	 *  a) Normal traffic to streaming port
	 *  b) TSO/LRO ACK to this treaming port
	 *  c) VPN Outbound 1 pass direct streaming egress from VPN-FW
	 *  Note: in HGU, re-insertiion is not required for UNI interface
	 */
	{
		.lookup_f = 0,
		.egflag = 1,
		.cls1 = 0,
		.cls2 = MAX_TC_ID,
		.q_type = DP_MAP_Q_SUBIF,
	}
};

/* PON port rule */
static struct dp_q_map_entry q_map_pon[] = {
	/* Note: TSO/VPN is applied on vUNI port, not directly on PON port. */

	/* Rule 1: Normal traffic from vUNI to PON during 2nd US QOS stage */
	{
		.lookup_f = 0,
		.egflag = 1,
		.cls1 = 0, /* NA */
		.cls2 = 0, /* NA */
		.q_type = DP_MAP_Q_SUBIF,
	}
};

/* Reinsert port rule */
static struct dp_q_map_entry q_map_reinsert[] = {
	/* Rule 1: re-insertion traffic */
	{
		.lookup_f = (CBM_QUEUE_MAP_F_SUBIF_DONTCARE |
			     CBM_QUEUE_MAP_F_TC_DONTCARE),
		.egflag = 0,
		.cls1 = 0,
		.cls2 = MAX_TC_ID,
		.q_type = DP_MAP_Q_REINSERT,
	}
};

/* vUNI rule */
static struct dp_q_map_entry q_map_vUNI[] = {
	/* Rule 1:
	 *   a) Normal US traffic from vUNI to PON
	 *   b) VPN FW to PON
	 *   c) TOE HW -> PON
	 */
	{
		.lookup_f = 0,
		.egflag = 0,
		.cls1 = 0,
		.cls2 = MAX_TC_ID,
		.q_type = DP_MAP_Q_SUBIF,
	}
};

/* PRX300 Directpath (IP Host ) rule */
static struct dp_q_map_entry q_map_directpath[] = {
	/* not supported yet */
};

/* ACA/DC rule ( like DSL/WIFIF) */
static struct dp_q_map_entry q_map_aca[] = {
	/* Rule 1: TSO/LRO_ACK traffic from TOE HW engineer -> ACA device */
	{
		.lookup_f = 0,
		.egflag = 1,
		.cls1 = MAX_TC_ID,
		.cls2 = MAX_TC_ID,
		.q_type = DP_MAP_Q_TOE,
	},
	/* Rule 2:  VPN FW 1-pass to ACA/DC device directly */
	{
		.lookup_f = 0,
		.egflag = 1,
		.cls1 = 0,
		.cls2 = 7,
		.q_type = DP_MAP_Q_SUBIF,
	},

	/* Rule 3:  ACA to SOC via QOS: depends on CQM driver's return value of
	 *          port_alloc_complete API
	 */
	{
		.lookup_f = 0,
		.egflag = 0,
		.cls1 = 0,  /* per cls may use different deqeue port/queue */
		.cls2 = MAX_TC_ID,
		.q_type = DP_MAP_Q_DC_RXOUT_QOS,
	}

};

/* Docsis port rule*/
static struct dp_q_map_entry q_map_docsis[] = {

	/* Rule 1: TSO/LRO_ACK traffic from TOE HW engineer -> ACA device */
	{
		.lookup_f = 0,
		.egflag = 1,
		.cls1 = 3,
		.cls2 = 3,
		.q_type = DP_MAP_Q_TOE,
	},

	/* Rule 2:  VPN FW 1-pass to ACA/DC device directly */
	{
		.lookup_f = 0,
		.egflag = 1,
		.cls1 = 0,
		.cls2 = 0,
		.q_type = DP_MAP_Q_SUBIF,
	},

	/* Rule 3: From this device to CPU */
	{
		.lookup_f = 0,
		.egflag = 0,
		/* DMA descrfiptor has 4 bits for any HW lookup mode.
		 * But for lookup mode 4 we have only 2 bits for class.
		 * Hence only 2 bits are used for HW lookup in Mode 4.
		 * So max class range [0-3]
		 */
		.cls1 = 0,
		.cls2 = 3,
		.q_type = DP_MAP_Q_CPU,
	}

};

/* spl_conn's IGP'e egp queue map */
static struct dp_q_map_entry q_map_spl_conn_igp[] = {
	/* Rule 1: for igp's egp queue mapping if it is qos mode
	 */
	{
		.lookup_f = 0,
		.egflag = 0,
		.cls1 = 0,
		.cls2 = MAX_TC_ID,
		.q_type = DP_MAP_Q_SPL_CONN_IGP,
	},
};

#endif /* DATAPATH_DFLT_Q_MAP_LGM */
